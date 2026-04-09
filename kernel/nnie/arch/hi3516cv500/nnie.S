@
@ -- FILES --
@

@ nnie.c
@ nnie_check_param_kernel.c
@ nnie_fill_task.c
@ nnie_drv.c
@ nnie_hal.c
@ nnie_init.c
@ hi3516cv500_nnie.mod.c


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text
@ Size: 0xbe20
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN SVP_NNIE_Poll
    /*00000000*/ MOV         R0, #0 
    /*00000004*/ BX          LR 
FUNC_END SVP_NNIE_Poll


FUNC_BEGIN SVP_NNIE_GetVerMagic
    /*00000008*/ MOVW        R0, #61479                  @ 0xf027 
    /*0000000c*/ MOVT        R0, #307                    @ 0x133 
    /*00000010*/ BX          LR 
FUNC_END SVP_NNIE_GetVerMagic


FUNC_BEGIN SVP_NNIE_Freeze
    /*00000014*/ MOV         R0, #0 
    /*00000018*/ BX          LR 
FUNC_END SVP_NNIE_Freeze


FUNC_BEGIN SVP_NNIE_Notify
    /*0000001c*/ CMP         R0, #17 
    /*00000020*/ LDR         R0, [PC, #16]               @ 0x0000000000000038 
    /*00000024*/ MOVWEQ      R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000028*/ MOVTEQ      R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000002c*/ MOVEQ       R2, #2 
    /*00000030*/ STREQ       R2, [R3] 
    SVP_NNIE_Notify_x34:
    /*00000034*/ B           SVP_NNIE_Notify_x34         @ osal_wakeup [R_ARM_JUMP24: +0x0] 
    /*00000038*/ LDRDEQ      R0, [R0], -IP 
FUNC_END SVP_NNIE_Notify


FUNC_BEGIN SVP_NNIE_ProcShow
    /*0000003c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00000040*/ MOVW        R1, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*00000044*/ SUB         SP, SP, #40                 @ 0x28 
    /*00000048*/ MOV         R5, R0 
    /*0000004c*/ MOVT        R1, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*00000050*/ MOVW        R8, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000054*/ BL          0x0000000000000054          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000058*/ MOV         R0, R5 
    /*0000005c*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x60] 
    /*00000060*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x60] 
    /*00000064*/ BL          0x0000000000000064          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000068*/ MOVT        R8, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x0] 
    /*0000006c*/ MOV         R0, R5 
    /*00000070*/ MOVW        R1, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x64] 
    /*00000074*/ MOVT        R1, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x64] 
    /*00000078*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000007c*/ BL          0x000000000000007C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000080*/ MOV         R0, R5 
    /*00000084*/ MOVW        R3, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbc] 
    /*00000088*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd0] 
    /*0000008c*/ MOVT        R3, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbc] 
    /*00000090*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd0] 
    /*00000094*/ MOVW        R1, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe0] 
    /*00000098*/ MOVT        R1, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe0] 
    /*0000009c*/ BL          0x000000000000009C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000000a0*/ LDRH        R3, [R8] 
    /*000000a4*/ LDRB        R2, [R8, #2] 
    /*000000a8*/ MOV         R0, R5 
    /*000000ac*/ MOVW        R1, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xec] 
    /*000000b0*/ MOVT        R1, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0xec] 
    /*000000b4*/ BL          0x00000000000000B4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000000b8*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000000bc*/ MOV         R0, R5 
    /*000000c0*/ MOVW        R1, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf8] 
    /*000000c4*/ MOVT        R1, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf8] 
    /*000000c8*/ BL          0x00000000000000C8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000000cc*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x188] 
    /*000000d0*/ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x194] 
    /*000000d4*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x188] 
    /*000000d8*/ MOVT        R3, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x194] 
    /*000000dc*/ STR         R2, [SP, #16] 
    /*000000e0*/ STR         R3, [SP, #12] 
    /*000000e4*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a0] 
    /*000000e8*/ MOVW        R3, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ac] 
    /*000000ec*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a0] 
    /*000000f0*/ MOVT        R3, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ac] 
    /*000000f4*/ STR         R2, [SP, #8] 
    /*000000f8*/ STR         R3, [SP, #4] 
    /*000000fc*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b8] 
    /*00000100*/ MOV         R0, R5 
    /*00000104*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b8] 
    /*00000108*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x150] 
    /*0000010c*/ STR         R2, [SP] 
    /*00000110*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x150] 
    /*00000114*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15c] 
    /*00000118*/ MOVW        R1, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x168] 
    /*0000011c*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15c] 
    /*00000120*/ MOVT        R1, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x168] 
    /*00000124*/ BL          0x0000000000000124          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000128*/ LDR         R3, [R4] 
    /*0000012c*/ CMP         R3, #1 
    /*00000130*/ BNE         SVP_NNIE_ProcShow_x1a0
    /*00000134*/ ADD         R0, SP, #32 
    /*00000138*/ BL          0x0000000000000138          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*0000013c*/ LDR         R3, [R4, #8] 
    /*00000140*/ LDR         R2, [R4, #12] 
    /*00000144*/ MOVW        R1, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x524] 
    /*00000148*/ RSB         R0, R3, #2 
    /*0000014c*/ ADD         IP, R3, #1 
    /*00000150*/ LDR         LR, [SP, #32] 
    /*00000154*/ MOVT        R1, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0x524] 
    /*00000158*/ ADD         R0, R4, R0, LSL #3 
    /*0000015c*/ STR         R2, [SP] 
    /*00000160*/ ADD         IP, R4, IP, LSL #3 
    /*00000164*/ MOV         R2, #0 
    /*00000168*/ LDR         R7, [R0, #16] 
    /*0000016c*/ LDR         R6, [R0, #12] 
    /*00000170*/ MOV         R0, R5 
    /*00000174*/ LDR         R9, [IP, #16] 
    /*00000178*/ LDR         IP, [IP, #12] 
    /*0000017c*/ STR         R7, [SP, #16] 
    /*00000180*/ STR         R6, [SP, #12] 
    /*00000184*/ LDRD        R6, [R4, #120]              @ 0x78 
    /*00000188*/ STR         R9, [SP, #8] 
    /*0000018c*/ SUBS        R6, LR, R6 
    /*00000190*/ STR         IP, [SP, #4] 
    /*00000194*/ RSC         R7, R7, LR, ASR #31 
    /*00000198*/ STRD        R6, [R4, #128]              @ 0x80 
    /*0000019c*/ BL          0x000000000000019C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    SVP_NNIE_ProcShow_x1a0:
    /*000001a0*/ MOVW        R1, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c4] 
    /*000001a4*/ MOV         R0, R5 
    /*000001a8*/ MOVT        R1, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c4] 
    /*000001ac*/ BL          0x00000000000001AC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000001b0*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x250] 
    /*000001b4*/ MOVW        R3, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x260] 
    /*000001b8*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x250] 
    /*000001bc*/ MOVT        R3, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x260] 
    /*000001c0*/ STR         R2, [SP, #24] 
    /*000001c4*/ STR         R3, [SP, #20] 
    /*000001c8*/ MOVW        R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x270] 
    /*000001cc*/ MOVW        R3, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x27c] 
    /*000001d0*/ MOVT        R2, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x270] 
    /*000001d4*/ MOVT        R3, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x27c] 
    /*000001d8*/ STR         R2, [SP, #16] 
    /*000001dc*/ STR         R3, [SP, #12] 
    /*000001e0*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x288] 
    /*000001e4*/ MOVW        R3, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x294] 
    /*000001e8*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x288] 
    /*000001ec*/ MOVT        R3, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x294] 
    /*000001f0*/ STR         R2, [SP, #8] 
    /*000001f4*/ STR         R3, [SP, #4] 
    /*000001f8*/ MOVW        R2, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a0] 
    /*000001fc*/ MOVW        R3, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*00000200*/ MOVT        R2, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a0] 
    /*00000204*/ MOVT        R3, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*00000208*/ STR         R2, [SP] 
    /*0000020c*/ MOVW        R1, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x228] 
    /*00000210*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15c] 
    /*00000214*/ MOVT        R1, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x228] 
    /*00000218*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15c] 
    /*0000021c*/ MOV         R0, R5 
    /*00000220*/ BL          0x0000000000000220          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000224*/ LDR         R3, [R4] 
    /*00000228*/ CMP         R3, #1 
    /*0000022c*/ BNE         SVP_NNIE_ProcShow_x278
    /*00000230*/ LDRH        IP, [R8] 
    /*00000234*/ MOVW        R1, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4fc] 
    /*00000238*/ LDR         LR, [R4, #56]               @ 0x38 
    /*0000023c*/ MOVT        R1, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4fc] 
    /*00000240*/ LDR         SL, [R4, #160]              @ 0xa0 
    /*00000244*/ MOV         R2, #0 
    /*00000248*/ LDR         R9, [R4, #156]              @ 0x9c 
    /*0000024c*/ SUB         IP, IP, LR 
    /*00000250*/ LDR         R7, [R4, #48]               @ 0x30 
    /*00000254*/ MOV         R0, R5 
    /*00000258*/ LDR         R6, [R4, #44]               @ 0x2c 
    /*0000025c*/ LDR         R8, [R4, #40]               @ 0x28 
    /*00000260*/ LDR         R3, [R4, #36]               @ 0x24 
    /*00000264*/ STR         LR, [SP, #24] 
    /*00000268*/ STR         IP, [SP, #20] 
    /*0000026c*/ STMIB       SP, {R6, R7, R9, SL} 
    /*00000270*/ STR         R8, [SP] 
    /*00000274*/ BL          0x0000000000000274          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    SVP_NNIE_ProcShow_x278:
    /*00000278*/ MOVW        R1, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ac] 
    /*0000027c*/ MOV         R0, R5 
    /*00000280*/ MOVT        R1, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ac] 
    /*00000284*/ BL          0x0000000000000284          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000288*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x334] 
    /*0000028c*/ MOVW        R3, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x344] 
    /*00000290*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x334] 
    /*00000294*/ MOVT        R3, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x344] 
    /*00000298*/ STR         R2, [SP, #24] 
    /*0000029c*/ STR         R3, [SP, #20] 
    /*000002a0*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x350] 
    /*000002a4*/ MOVW        R3, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x35c] 
    /*000002a8*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x350] 
    /*000002ac*/ MOVT        R3, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x35c] 
    /*000002b0*/ STR         R2, [SP, #16] 
    /*000002b4*/ STR         R3, [SP, #12] 
    /*000002b8*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x36c] 
    /*000002bc*/ MOVW        R3, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x384] 
    /*000002c0*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x36c] 
    /*000002c4*/ MOVT        R3, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x384] 
    /*000002c8*/ STR         R2, [SP, #8] 
    /*000002cc*/ STR         R3, [SP, #4] 
    /*000002d0*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x394] 
    /*000002d4*/ MOVW        R3, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x304] 
    /*000002d8*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x394] 
    /*000002dc*/ MOVT        R3, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x304] 
    /*000002e0*/ STR         R2, [SP] 
    /*000002e4*/ MOVW        R1, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x310] 
    /*000002e8*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15c] 
    /*000002ec*/ MOVT        R1, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x310] 
    /*000002f0*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15c] 
    /*000002f4*/ MOV         R0, R5 
    /*000002f8*/ BL          0x00000000000002F8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000002fc*/ LDR         R3, [R4] 
    /*00000300*/ CMP         R3, #1 
    /*00000304*/ BNE         SVP_NNIE_ProcShow_x350
    /*00000308*/ ADD         R6, R4, #84                 @ 0x54 
    /*0000030c*/ LDR         SL, [R4, #148]              @ 0x94 
    /*00000310*/ LDR         R9, [R4, #144]              @ 0x90 
    /*00000314*/ MOVW        R1, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4d8] 
    /*00000318*/ LDM         R6, {R6, IP, LR} 
    /*0000031c*/ MOVT        R1, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4d8] 
    /*00000320*/ LDR         R8, [R4, #152]              @ 0x98 
    /*00000324*/ MOV         R2, #0 
    /*00000328*/ LDR         R7, [R4, #80]               @ 0x50 
    /*0000032c*/ MOV         R0, R5 
    /*00000330*/ LDR         R3, [R4, #60]               @ 0x3c 
    /*00000334*/ STR         SL, [SP, #24] 
    /*00000338*/ STR         R9, [SP, #20] 
    /*0000033c*/ STR         R8, [SP, #16] 
    /*00000340*/ STR         R7, [SP, #12] 
    /*00000344*/ STR         R6, [SP, #8] 
    /*00000348*/ STM         SP, {IP, LR} 
    /*0000034c*/ BL          0x000000000000034C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    SVP_NNIE_ProcShow_x350:
    /*00000350*/ MOVW        R3, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3d4] 
    /*00000354*/ MOVW        R2, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3e4] 
    /*00000358*/ MOVT        R3, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3d4] 
    /*0000035c*/ MOVT        R2, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3e4] 
    /*00000360*/ STR         R3, [SP, #12] 
    /*00000364*/ MOVW        R1, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b8] 
    /*00000368*/ STR         R2, [SP, #8] 
    /*0000036c*/ MOVW        R3, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3f8] 
    /*00000370*/ MOVW        R2, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x408] 
    /*00000374*/ MOVT        R3, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3f8] 
    /*00000378*/ MOVT        R2, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x408] 
    /*0000037c*/ STR         R3, [SP, #4] 
    /*00000380*/ STR         R2, [SP] 
    /*00000384*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3a0] 
    /*00000388*/ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3ac] 
    /*0000038c*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3a0] 
    /*00000390*/ MOVT        R2, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3ac] 
    /*00000394*/ MOVT        R1, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b8] 
    /*00000398*/ MOV         R0, R5 
    /*0000039c*/ BL          0x000000000000039C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000003a0*/ LDR         R3, [R4] 
    /*000003a4*/ CMP         R3, #1 
    /*000003a8*/ BNE         SVP_NNIE_ProcShow_x3e0
    /*000003ac*/ LDRD        R6, [R4, #128]              @ 0x80 
    /*000003b0*/ MOVW        R1, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4bc] 
    /*000003b4*/ LDR         R8, [R4, #96]               @ 0x60 
    /*000003b8*/ MOVT        R1, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4bc] 
    /*000003bc*/ LDR         LR, [R4, #116]              @ 0x74 
    /*000003c0*/ MOV         R0, R5 
    /*000003c4*/ LDR         IP, [R4, #112]              @ 0x70 
    /*000003c8*/ LDR         R3, [R4, #108]              @ 0x6c 
    /*000003cc*/ LDR         R2, [R4, #104]              @ 0x68 
    /*000003d0*/ STRD        R6, [SP, #16] 
    /*000003d4*/ STR         R8, [SP, #8] 
    /*000003d8*/ STM         SP, {IP, LR} 
    /*000003dc*/ BL          0x00000000000003DC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    SVP_NNIE_ProcShow_x3e0:
    /*000003e0*/ MOVW        R1, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x418] 
    /*000003e4*/ MOV         R0, R5 
    /*000003e8*/ MOVT        R1, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x418] 
    /*000003ec*/ BL          0x00000000000003EC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000003f0*/ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x498] 
    /*000003f4*/ MOVW        R3, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x470] 
    /*000003f8*/ MOVT        R2, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x498] 
    /*000003fc*/ MOVT        R3, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x470] 
    /*00000400*/ STR         R2, [SP] 
    /*00000404*/ MOVW        R1, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x488] 
    /*00000408*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x47c] 
    /*0000040c*/ MOVT        R1, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x488] 
    /*00000410*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x47c] 
    /*00000414*/ MOV         R0, R5 
    /*00000418*/ BL          0x0000000000000418          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000041c*/ LDR         R3, [R4] 
    /*00000420*/ CMP         R3, #1 
    /*00000424*/ BNE         SVP_NNIE_ProcShow_x448
    /*00000428*/ LDR         IP, [R4, #168]              @ 0xa8 
    /*0000042c*/ MOVW        R1, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4ac] 
    /*00000430*/ LDR         R3, [R4, #164]              @ 0xa4 
    /*00000434*/ MOV         R0, R5 
    /*00000438*/ MOV         R2, #0 
    /*0000043c*/ MOVT        R1, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4ac] 
    /*00000440*/ STR         IP, [SP] 
    /*00000444*/ BL          0x0000000000000444          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    SVP_NNIE_ProcShow_x448:
    /*00000448*/ MOV         R0, #0 
    /*0000044c*/ ADD         SP, SP, #40                 @ 0x28 
    /*00000450*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
FUNC_END SVP_NNIE_ProcShow


FUNC_BEGIN SVP_NNIE_QueryState
    /*00000454*/ PUSH        {R4, LR} 
    /*00000458*/ MOV         R4, R0 
    /*0000045c*/ LDR         R0, [PC, #20]               @ 0x0000000000000478 
    /*00000460*/ BL          0x0000000000000460          @ osal_atomic_read [R_ARM_CALL: +0x0] 
    /*00000464*/ CMP         R0, #0 
    /*00000468*/ MOVEQ       R3, #17 
    /*0000046c*/ MOVNE       R3, #34                     @ 0x22 
    /*00000470*/ STR         R3, [R4] 
    /*00000474*/ POP         {R4, PC} 
    /*00000478*/ ANDEQ       R0, R0, R0, ROR #1 
FUNC_END SVP_NNIE_QueryState


FUNC_BEGIN SVP_NNIE_WaitConditionCallBack
    /*0000047c*/ CMP         R0, #0 
    /*00000480*/ BEQ         SVP_NNIE_WaitConditionCallBack_x508
    /*00000484*/ LDR         IP, [R0, #4] 
    /*00000488*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000048c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000490*/ MOV         R2, #216                    @ 0xd8 
    /*00000494*/ LDR         R1, [R0] 
    /*00000498*/ MLA         R2, R2, IP, R3 
    /*0000049c*/ LDR         R2, [R2, #40]               @ 0x28 
    /*000004a0*/ CMP         R1, R2 
    /*000004a4*/ BCS         SVP_NNIE_WaitConditionCallBack_x4ec
    /*000004a8*/ SUB         R2, R2, R1 
    /*000004ac*/ MOVW        R1, #65535                  @ 0xffff 
    /*000004b0*/ MOVT        R1, #4094                   @ 0xffe 
    /*000004b4*/ CMP         R2, R1 
    /*000004b8*/ BLS         SVP_NNIE_WaitConditionCallBack_x500
    SVP_NNIE_WaitConditionCallBack_x4bc:
    /*000004bc*/ MOV         R2, #216                    @ 0xd8 
    /*000004c0*/ MLA         IP, R2, IP, R3 
    /*000004c4*/ LDR         R2, [IP, #64]               @ 0x40 
    /*000004c8*/ CMP         R2, #0 
    /*000004cc*/ BNE         SVP_NNIE_WaitConditionCallBack_x500
    /*000004d0*/ LDR         R2, [IP, #68]               @ 0x44 
    /*000004d4*/ CMP         R2, #0 
    /*000004d8*/ BNE         SVP_NNIE_WaitConditionCallBack_x500
    /*000004dc*/ LDR         R0, [R3] 
    /*000004e0*/ SUBS        R0, R0, #1 
    /*000004e4*/ MOVNE       R0, #1 
    /*000004e8*/ BX          LR 
    SVP_NNIE_WaitConditionCallBack_x4ec:
    /*000004ec*/ SUB         R2, R1, R2 
    /*000004f0*/ MOVW        R1, #65535                  @ 0xffff 
    /*000004f4*/ MOVT        R1, #4094                   @ 0xffe 
    /*000004f8*/ CMP         R2, R1 
    /*000004fc*/ BLS         SVP_NNIE_WaitConditionCallBack_x4bc
    SVP_NNIE_WaitConditionCallBack_x500:
    /*00000500*/ MOV         R0, #1 
    /*00000504*/ BX          LR 
    SVP_NNIE_WaitConditionCallBack_x508:
    /*00000508*/ PUSH        {R4, LR} 
    /*0000050c*/ SUB         SP, SP, #8 
    /*00000510*/ MOV         R4, R0 
    /*00000514*/ MOVW        R2, #1308                   @ 0x51c 
    /*00000518*/ MOV         R1, #51                     @ 0x33 
    /*0000051c*/ STR         R2, [SP] 
    /*00000520*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000524*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00000528*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*0000052c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00000530*/ MOV         R0, #3 
    /*00000534*/ BL          0x0000000000000534          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000538*/ MOV         R1, #51                     @ 0x33 
    /*0000053c*/ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x560] 
    /*00000540*/ MOV         R0, #3 
    /*00000544*/ MOVT        R2, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x560] 
    /*00000548*/ BL          0x0000000000000548          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000054c*/ MOV         R0, R4 
    /*00000550*/ ADD         SP, SP, #8 
    /*00000554*/ POP         {R4, PC} 
FUNC_END SVP_NNIE_WaitConditionCallBack


FUNC_BEGIN SvpNnieStartTask
    /*00000558*/ PUSH        {R4, R5, R6, R7, LR} 
    /*0000055c*/ SUB         SP, SP, #20 
    /*00000560*/ MOV         R5, R0 
    /*00000564*/ BL          0x0000000000000564          @ SVP_NNIE_DRV_PrepareNnie [R_ARM_CALL: .text+0xb874] 
    /*00000568*/ SUBS        R6, R0, #0 
    /*0000056c*/ BEQ         SvpNnieStartTask_x578
    SvpNnieStartTask_x570:
    /*00000570*/ ADD         SP, SP, #20 
    /*00000574*/ POP         {R4, R5, R6, R7, PC} 
    SvpNnieStartTask_x578:
    /*00000578*/ MOV         R0, #37                     @ 0x25 
    /*0000057c*/ BL          0x000000000000057C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000580*/ SUBS        R4, R0, #0 
    /*00000584*/ BEQ         SvpNnieStartTask_x648
    /*00000588*/ LDR         R3, [R4, #124]              @ 0x7c 
    /*0000058c*/ CMP         R3, #0 
    /*00000590*/ BEQ         SvpNnieStartTask_x648
    /*00000594*/ LDR         R3, [R4, #128]              @ 0x80 
    /*00000598*/ CMP         R3, #0 
    /*0000059c*/ BEQ         SvpNnieStartTask_x648
    /*000005a0*/ LDR         R3, [R4, #132]              @ 0x84 
    /*000005a4*/ CMP         R3, #0 
    /*000005a8*/ BEQ         SvpNnieStartTask_x648
    /*000005ac*/ LDR         R2, [R4, #136]              @ 0x88 
    /*000005b0*/ CMP         R2, #0 
    /*000005b4*/ BEQ         SvpNnieStartTask_x648
    /*000005b8*/ ADD         R0, SP, #12 
    /*000005bc*/ BLX         R3 
    /*000005c0*/ MOV         R0, R6 
    /*000005c4*/ LDR         R3, [R4, #128]              @ 0x80 
    /*000005c8*/ ADD         R1, SP, #8 
    /*000005cc*/ BLX         R3 
    /*000005d0*/ SUBS        R6, R0, #0 
    /*000005d4*/ BNE         SvpNnieStartTask_x600
    /*000005d8*/ LDR         R6, [SP, #8] 
    /*000005dc*/ CMP         R6, #0 
    /*000005e0*/ BEQ         SvpNnieStartTask_x6e8
    /*000005e4*/ LDR         R3, [R4, #136]              @ 0x88 
    /*000005e8*/ ADD         R0, SP, #12 
    /*000005ec*/ BLX         R3 
    /*000005f0*/ CMP         R6, #1 
    /*000005f4*/ BEQ         SvpNnieStartTask_x680
    /*000005f8*/ ADD         SP, SP, #20 
    /*000005fc*/ POP         {R4, R5, R6, R7, PC} 
    SvpNnieStartTask_x600:
    /*00000600*/ MOVW        R3, #386                    @ 0x182 
    /*00000604*/ MOV         R1, #51                     @ 0x33 
    /*00000608*/ STR         R3, [SP] 
    /*0000060c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00000610*/ LDR         R3, [PC, #308]              @ 0x000000000000074c 
    /*00000614*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00000618*/ MOV         R0, #3 
    /*0000061c*/ BL          0x000000000000061C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000620*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5d0] 
    /*00000624*/ MOV         R3, R6 
    /*00000628*/ MOV         R1, #51                     @ 0x33 
    /*0000062c*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5d0] 
    /*00000630*/ MOV         R0, #3 
    /*00000634*/ BL          0x0000000000000634          @ HI_LOG [R_ARM_CALL: +0x0] 
    SvpNnieStartTask_x638:
    /*00000638*/ LDR         R3, [R4, #136]              @ 0x88 
    /*0000063c*/ ADD         R0, SP, #12 
    /*00000640*/ BLX         R3 
    /*00000644*/ B           SvpNnieStartTask_x570
    SvpNnieStartTask_x648:
    /*00000648*/ MOVW        R3, #374                    @ 0x176 
    /*0000064c*/ MOV         R1, #51                     @ 0x33 
    /*00000650*/ STR         R3, [SP] 
    /*00000654*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00000658*/ LDR         R3, [PC, #236]              @ 0x000000000000074c 
    /*0000065c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00000660*/ MOV         R0, #4 
    /*00000664*/ BL          0x0000000000000664          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000668*/ MOVW        R2, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x578] 
    /*0000066c*/ MOV         R1, #51                     @ 0x33 
    /*00000670*/ MOVT        R2, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x578] 
    /*00000674*/ MOV         R0, #4 
    /*00000678*/ BL          0x0000000000000678          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000067c*/ BL          0x000000000000067C          @ SVP_NNIE_DRV_SelectRam [R_ARM_CALL: .text+0xb7e0] 
    SvpNnieStartTask_x680:
    /*00000680*/ MOV         R0, R5 
    /*00000684*/ MOV         R1, #1 
    /*00000688*/ BL          0x0000000000000688          @ SVP_NNIE_DRV_EnableSysClk [R_ARM_CALL: .text+0xb54c] 
    /*0000068c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000690*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000694*/ MOV         R2, #216                    @ 0xd8 
    /*00000698*/ MOV         IP, #27 
    /*0000069c*/ MOV         LR, #1 
    /*000006a0*/ MLA         R2, R2, R5, R3 
    /*000006a4*/ MOV         R0, R5 
    /*000006a8*/ LDR         R1, [R2, #12] 
    /*000006ac*/ STR         LR, [R2, #16] 
    /*000006b0*/ ADD         R2, R1, R5, LSL LR 
    /*000006b4*/ MLA         R5, IP, R5, R1 
    /*000006b8*/ ADD         R2, R3, R2, LSL #3 
    /*000006bc*/ LDRD        R6, [R2, #232]              @ 0xe8 
    /*000006c0*/ ADD         R5, R3, R5, LSL #3 
    /*000006c4*/ LDR         R3, [R5, #20] 
    /*000006c8*/ LSL         R3, R3, #6 
    /*000006cc*/ ADDS        R4, R6, R3 
    /*000006d0*/ ADC         R5, R7, #0 
    /*000006d4*/ MOV         R2, R4 
    /*000006d8*/ MOV         R3, R5 
    /*000006dc*/ BL          0x00000000000006DC          @ SVP_NNIE_DRV_Start [R_ARM_CALL: .text+0xb630] 
    /*000006e0*/ ADD         SP, SP, #20 
    /*000006e4*/ POP         {R4, R5, R6, R7, PC} 
    SvpNnieStartTask_x6e8:
    /*000006e8*/ LDR         R3, [R4, #124]              @ 0x7c 
    /*000006ec*/ MOV         R1, #1 
    /*000006f0*/ BLX         R3 
    /*000006f4*/ SUBS        R6, R0, #0 
    /*000006f8*/ BEQ         SvpNnieStartTask_x738
    /*000006fc*/ MOV         R3, #396                    @ 0x18c 
    /*00000700*/ MOV         R1, #51                     @ 0x33 
    /*00000704*/ STR         R3, [SP] 
    /*00000708*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*0000070c*/ LDR         R3, [PC, #56]               @ 0x000000000000074c 
    /*00000710*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00000714*/ MOV         R0, #3 
    /*00000718*/ BL          0x0000000000000718          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000071c*/ MOV         R3, R6 
    /*00000720*/ MOVW        R2, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5f8] 
    /*00000724*/ MOV         R1, #51                     @ 0x33 
    /*00000728*/ MOVT        R2, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5f8] 
    /*0000072c*/ MOV         R0, #3 
    /*00000730*/ BL          0x0000000000000730          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000734*/ B           SvpNnieStartTask_x638
    SvpNnieStartTask_x738:
    /*00000738*/ BL          0x0000000000000738          @ SVP_NNIE_DRV_SelectRam [R_ARM_CALL: .text+0xb7e0] 
    /*0000073c*/ LDR         R3, [R4, #136]              @ 0x88 
    /*00000740*/ ADD         R0, SP, #12 
    /*00000744*/ BLX         R3 
    /*00000748*/ B           SvpNnieStartTask_x680
    /*0000074c*/ ANDEQ       R0, R0, R0, LSR #32 
FUNC_END SvpNnieStartTask


FUNC_BEGIN SVP_NNIE_Close
    /*00000750*/ MOV         R0, #0 
    /*00000754*/ BX          LR 
FUNC_END SVP_NNIE_Close


FUNC_BEGIN SVP_NNIE_Restore
    /*00000758*/ MOV         R0, #0 
    /*0000075c*/ BX          LR 
FUNC_END SVP_NNIE_Restore


FUNC_BEGIN SVP_NNIE_Open
    /*00000760*/ MOV         R0, #0 
    /*00000764*/ BX          LR 
FUNC_END SVP_NNIE_Open


FUNC_BEGIN SvpNniePostProcess
    /*00000768*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000076c*/ SUBS        R8, R1, #0 
    /*00000770*/ SUB         SP, SP, #36                 @ 0x24 
    /*00000774*/ MOV         R4, R0 
    /*00000778*/ STR         R2, [SP, #16] 
    /*0000077c*/ STR         R3, [SP, #12] 
    /*00000780*/ BEQ         SvpNniePostProcess_x9f0
    SvpNniePostProcess_x784:
    /*00000784*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000788*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000078c*/ MOV         R9, #216                    @ 0xd8 
    /*00000790*/ ADD         R7, R5, #248                @ 0xf8 
    /*00000794*/ MLA         R9, R9, R4, R5 
    /*00000798*/ ADD         R7, R7, R4, LSL #2 
    /*0000079c*/ ADD         R1, SP, #28 
    /*000007a0*/ MOV         R0, R7 
    /*000007a4*/ BL          0x00000000000007A4          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000007a8*/ MOV         R3, #27 
    /*000007ac*/ LDR         R2, [R9, #8] 
    /*000007b0*/ MLA         R3, R3, R4, R2 
    /*000007b4*/ ADD         R3, R5, R3, LSL #3 
    /*000007b8*/ LDR         R6, [R3, #24] 
    /*000007bc*/ CMP         R6, #512                    @ 0x200 
    /*000007c0*/ BCS         SvpNniePostProcess_x8b4
    /*000007c4*/ ADD         FP, R2, R4, LSL #1 
    /*000007c8*/ MOV         R1, R8 
    /*000007cc*/ LSL         SL, R6, #6 
    /*000007d0*/ MOV         R2, #64                     @ 0x40 
    /*000007d4*/ ADD         R3, R5, FP, LSL #2 
    /*000007d8*/ STR         R3, [SP, #20] 
    /*000007dc*/ LDR         R8, [R3, #252]              @ 0xfc 
    /*000007e0*/ ADD         R8, R8, SL 
    /*000007e4*/ MOV         R0, R8 
    /*000007e8*/ BL          0x00000000000007E8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000007ec*/ CMP         R6, #0 
    /*000007f0*/ STR         R6, [R8, #4] 
    /*000007f4*/ BNE         SvpNniePostProcess_x884
    SvpNniePostProcess_x7f8:
    /*000007f8*/ MOV         R2, #216                    @ 0xd8 
    /*000007fc*/ MOV         R3, #27 
    /*00000800*/ MLA         R2, R2, R4, R5 
    /*00000804*/ LDR         R1, [SP, #12] 
    /*00000808*/ LDR         R0, [R2, #8] 
    /*0000080c*/ STR         R1, [R2, #60]               @ 0x3c 
    /*00000810*/ LDR         R1, [R2, #12] 
    /*00000814*/ MLA         R3, R3, R4, R0 
    /*00000818*/ CMN         R1, #1 
    /*0000081c*/ ADD         R3, R3, #1 
    /*00000820*/ ADD         R3, R5, R3, LSL #3 
    /*00000824*/ LDR         R1, [R3, #16] 
    /*00000828*/ ADD         R1, R1, #1 
    /*0000082c*/ STR         R1, [R3, #16] 
    /*00000830*/ BEQ         SvpNniePostProcess_x954
    SvpNniePostProcess_x834:
    /*00000834*/ MOV         R3, #216                    @ 0xd8 
    /*00000838*/ MOV         R0, R7 
    /*0000083c*/ MLA         R4, R3, R4, R5 
    /*00000840*/ LDR         R2, [R4, #36]               @ 0x24 
    /*00000844*/ ADD         R3, R2, #1 
    /*00000848*/ CMN         R3, #-268435455             @ 0xf0000001 
    /*0000084c*/ STRLS       R3, [R4, #36]               @ 0x24 
    /*00000850*/ ADDHI       R3, R2, #-268435454         @ 0xf0000002 
    /*00000854*/ STRHI       R3, [R4, #36]               @ 0x24 
    /*00000858*/ LDRHI       R1, [R4, #156]              @ 0x9c 
    /*0000085c*/ SUB         R3, R3, #1 
    /*00000860*/ ADDHI       R2, R1, #1 
    /*00000864*/ STRHI       R2, [R4, #156]              @ 0x9c 
    /*00000868*/ LDR         R2, [SP, #16] 
    /*0000086c*/ ADD         R1, SP, #28 
    /*00000870*/ STR         R3, [R2] 
    /*00000874*/ BL          0x0000000000000874          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000878*/ MOV         R0, #0 
    /*0000087c*/ ADD         SP, SP, #36                 @ 0x24 
    /*00000880*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SvpNniePostProcess_x884:
    /*00000884*/ LDR         R2, [R9, #60]               @ 0x3c 
    /*00000888*/ CMP         R2, #0 
    /*0000088c*/ BNE         SvpNniePostProcess_x7f8
    /*00000890*/ LDR         R3, [SP, #20] 
    /*00000894*/ ADD         R2, R5, FP, LSL #3 
    /*00000898*/ LDRD        R0, [R2, #232]              @ 0xe8 
    /*0000089c*/ LDR         R3, [R3, #252]              @ 0xfc 
    /*000008a0*/ ADDS        R0, R0, SL 
    /*000008a4*/ ADD         R3, R3, SL 
    /*000008a8*/ ADC         R1, R1, #0 
    /*000008ac*/ STRD        R0, [R3, #-56]              @ 0xffffffc8 
    /*000008b0*/ B           SvpNniePostProcess_x7f8
    SvpNniePostProcess_x8b4:
    /*000008b4*/ MOVW        R3, #869                    @ 0x365 
    /*000008b8*/ MOV         R1, #51                     @ 0x33 
    /*000008bc*/ STR         R3, [SP] 
    /*000008c0*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000008c4*/ LDR         R3, [PC, #372]              @ 0x0000000000000a40 
    /*000008c8*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000008cc*/ MOV         R0, #3 
    /*000008d0*/ BL          0x00000000000008D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000008d4*/ STR         R6, [SP] 
    /*000008d8*/ MOV         R2, #512                    @ 0x200 
    /*000008dc*/ MOV         R1, #51                     @ 0x33 
    /*000008e0*/ STR         R2, [SP, #4] 
    /*000008e4*/ MOVW        R3, #32783                  @ 0x800f 
    /*000008e8*/ MOVW        R2, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x674] 
    /*000008ec*/ MOVT        R3, #41011                  @ 0xa033 
    /*000008f0*/ MOVT        R2, #0                      @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0x674] 
    /*000008f4*/ MOV         R0, #3 
    /*000008f8*/ BL          0x00000000000008F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000008fc*/ MOVW        R3, #966                    @ 0x3c6 
    /*00000900*/ MOV         R1, #51                     @ 0x33 
    /*00000904*/ STR         R3, [SP] 
    /*00000908*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*0000090c*/ LDR         R3, [PC, #304]              @ 0x0000000000000a44 
    /*00000910*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00000914*/ MOV         R0, #3 
    /*00000918*/ BL          0x0000000000000918          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000091c*/ MOV         R1, #51                     @ 0x33 
    /*00000920*/ MOVW        R3, #32783                  @ 0x800f 
    /*00000924*/ MOVW        R2, #0                      @ .LC61 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6c0] 
    /*00000928*/ MOVT        R3, #41011                  @ 0xa033 
    /*0000092c*/ MOVT        R2, #0                      @ .LC61 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6c0] 
    /*00000930*/ MOV         R0, #3 
    /*00000934*/ BL          0x0000000000000934          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000938*/ MOV         R0, R7 
    /*0000093c*/ ADD         R1, SP, #28 
    /*00000940*/ BL          0x0000000000000940          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000944*/ MOVW        R0, #32783                  @ 0x800f 
    /*00000948*/ MOVT        R0, #41011                  @ 0xa033 
    /*0000094c*/ ADD         SP, SP, #36                 @ 0x24 
    /*00000950*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SvpNniePostProcess_x954:
    /*00000954*/ LDR         R3, [R2, #72]               @ 0x48 
    /*00000958*/ STR         R0, [R2, #12] 
    /*0000095c*/ RSB         R0, R0, #1 
    /*00000960*/ CMP         R3, #0 
    /*00000964*/ STR         R0, [R2, #8] 
    /*00000968*/ BEQ         SvpNniePostProcess_x978
    SvpNniePostProcess_x96c:
    /*0000096c*/ MOV         R0, R4 
    /*00000970*/ BL          0x0000000000000558 
    /*00000974*/ B           SvpNniePostProcess_x834
    SvpNniePostProcess_x978:
    /*00000978*/ CMP         R4, #0 
    /*0000097c*/ BNE         SvpNniePostProcess_xa20
    SvpNniePostProcess_x980:
    /*00000980*/ MOV         R1, #1 
    /*00000984*/ MOV         R0, R4 
    /*00000988*/ BL          0x0000000000000988          @ SVP_NNIE_DRV_EnableSysClk [R_ARM_CALL: .text+0xb54c] 
    /*0000098c*/ MOV         R1, #0 
    /*00000990*/ MOV         R0, R4 
    /*00000994*/ BL          0x0000000000000994          @ SVP_NNIE_DRV_SetReset [R_ARM_CALL: .text+0xb5b8] 
    /*00000998*/ MOV         R0, R4 
    /*0000099c*/ BL          0x000000000000099C          @ SVP_NNIE_DRV_EnInt [R_ARM_CALL: .text+0xb708] 
    /*000009a0*/ MOV         R3, #255                    @ 0xff 
    /*000009a4*/ MVN         R2, #0 
    /*000009a8*/ MOV         R0, R4 
    /*000009ac*/ BL          0x00000000000009AC          @ SVP_NNIE_DRV_SetTimeOut [R_ARM_CALL: .text+0xb70c] 
    /*000009b0*/ MOV         R0, R4 
    /*000009b4*/ BL          0x00000000000009B4          @ SVP_NNIE_DRV_EnClkGt [R_ARM_CALL: .text+0xb710] 
    /*000009b8*/ MOV         R0, R4 
    /*000009bc*/ BL          0x00000000000009BC          @ SVP_NNIE_DRV_SetMemSpeed [R_ARM_CALL: .text+0xb714] 
    /*000009c0*/ MOV         R0, R4 
    /*000009c4*/ BL          0x00000000000009C4          @ SVP_NNIE_DRV_DisableCheckSum [R_ARM_CALL: .text+0xb7c8] 
    /*000009c8*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000009cc*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000009d0*/ LDRB        R3, [R3, #2] 
    /*000009d4*/ CMP         R3, #0 
    /*000009d8*/ BNE         SvpNniePostProcess_xa10
    SvpNniePostProcess_x9dc:
    /*000009dc*/ MOV         R3, #216                    @ 0xd8 
    /*000009e0*/ MOV         R2, #1 
    /*000009e4*/ MLA         R3, R3, R4, R5 
    /*000009e8*/ STR         R2, [R3, #72]               @ 0x48 
    /*000009ec*/ B           SvpNniePostProcess_x96c
    SvpNniePostProcess_x9f0:
    /*000009f0*/ MOVW        R3, #0                      @ .LC58 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x620] 
    /*000009f4*/ MOVW        R0, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x634] 
    /*000009f8*/ MOVT        R3, #0                      @ .LC58 [R_ARM_MOVT_ABS: .rodata.str1.4+0x620] 
    /*000009fc*/ MOVT        R0, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0x634] 
    /*00000a00*/ MOVW        R2, #959                    @ 0x3bf 
    /*00000a04*/ LDR         R1, [PC, #56]               @ 0x0000000000000a44 
    /*00000a08*/ BL          0x0000000000000A08          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000a0c*/ B           SvpNniePostProcess_x784
    SvpNniePostProcess_xa10:
    /*00000a10*/ MOV         R1, #0 
    /*00000a14*/ MOV         R0, R4 
    /*00000a18*/ BL          0x0000000000000A18          @ SVP_NNIE_DRV_EnableSysClk [R_ARM_CALL: .text+0xb54c] 
    /*00000a1c*/ B           SvpNniePostProcess_x9dc
    SvpNniePostProcess_xa20:
    /*00000a20*/ MOVW        R3, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6e0] 
    /*00000a24*/ MOVW        R0, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x634] 
    /*00000a28*/ MOVT        R3, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6e0] 
    /*00000a2c*/ MOVT        R0, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0x634] 
    /*00000a30*/ MOV         R2, #256                    @ 0x100 
    /*00000a34*/ LDR         R1, [PC, #12]               @ 0x0000000000000a48 
    /*00000a38*/ BL          0x0000000000000A38          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000a3c*/ B           SvpNniePostProcess_x980
    /*00000a40*/ ANDEQ       R0, R0, R8, ASR #32 
    /*00000a44*/ ANDEQ       R0, R0, R4, LSR R0 
    /*00000a48*/ ANDEQ       R0, R0, IP, ASR R0 
FUNC_END SvpNniePostProcess


FUNC_BEGIN SVP_NNIE_DeInit
    /*00000a4c*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000a50*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000a54*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000a58*/ LDR         R3, [R5] 
    /*00000a5c*/ CMP         R3, #0 
    /*00000a60*/ POPEQ       {R4, R5, R6, R7, R8, PC} 
    /*00000a64*/ MOVW        R3, #0                      @ g_au32GdcIrq [R_ARM_MOVW_ABS_NC: .data+0xb4] 
    /*00000a68*/ MOVT        R3, #0                      @ g_au32GdcIrq [R_ARM_MOVT_ABS: .data+0xb4] 
    /*00000a6c*/ MOVW        R1, #0                      @ SVP_NNIE_GdcIntRoute [R_ARM_MOVW_ABS_NC: .text+0xc40] 
    /*00000a70*/ MOVT        R1, #0                      @ SVP_NNIE_GdcIntRoute [R_ARM_MOVT_ABS: .text+0xc40] 
    /*00000a74*/ LDR         R0, [R3] 
    /*00000a78*/ BL          0x0000000000000A78          @ osal_free_irq [R_ARM_CALL: +0x0] 
    /*00000a7c*/ MOV         R1, #1 
    /*00000a80*/ MOV         R0, #0 
    /*00000a84*/ LDR         R6, [PC, #408]              @ 0x0000000000000c24 
    /*00000a88*/ BL          0x0000000000000A88          @ SVP_NNIE_DRV_SetReset [R_ARM_CALL: .text+0xb5b8] 
    /*00000a8c*/ LDR         R3, [R5, #72]               @ 0x48 
    /*00000a90*/ MOVW        R1, #0                      @ g_au32SvpNnieCoreId [R_ARM_MOVW_ABS_NC: .bss+0x12c] 
    /*00000a94*/ MOVT        R1, #0                      @ g_au32SvpNnieCoreId [R_ARM_MOVT_ABS: .bss+0x12c] 
    /*00000a98*/ CMP         R3, #1 
    /*00000a9c*/ ADD         R7, R6, #4 
    /*00000aa0*/ MOVEQ       R3, #0 
    /*00000aa4*/ STREQ       R3, [R5, #72]               @ 0x48 
    /*00000aa8*/ MOVW        R3, #0                      @ g_au32NnieIrg [R_ARM_MOVW_ABS_NC: .data+0xbc] 
    /*00000aac*/ MOVT        R3, #0                      @ g_au32NnieIrg [R_ARM_MOVT_ABS: .data+0xbc] 
    /*00000ab0*/ LDR         R0, [R3] 
    /*00000ab4*/ BL          0x0000000000000AB4          @ osal_free_irq [R_ARM_CALL: +0x0] 
    /*00000ab8*/ ADD         R0, R6, #32 
    /*00000abc*/ BL          0x0000000000000ABC          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00000ac0*/ MOV         R0, R6 
    /*00000ac4*/ BL          0x0000000000000AC4          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*00000ac8*/ MOV         R0, R7 
    /*00000acc*/ BL          0x0000000000000ACC          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00000ad0*/ MOV         R0, #0 
    /*00000ad4*/ BL          0x0000000000000AD4          @ SVP_NNIE_DRV_DeInit [R_ARM_CALL: .text+0xb548] 
    /*00000ad8*/ LDR         R0, [R5, #192]              @ 0xc0 
    /*00000adc*/ CMP         R0, #0 
    /*00000ae0*/ BEQ         SVP_NNIE_DeInit_xaf0
    /*00000ae4*/ BL          0x0000000000000AE4          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*00000ae8*/ MOV         R3, #0 
    /*00000aec*/ STR         R3, [R5, #192]              @ 0xc0 
    SVP_NNIE_DeInit_xaf0:
    /*00000af0*/ MOV         R1, #0 
    /*00000af4*/ LDR         R0, [PC, #300]              @ 0x0000000000000c28 
    /*00000af8*/ MOV         R4, R1 
    /*00000afc*/ BL          0x0000000000000AFC          @ SVP_NNIE_TSK_ADDR_DeInitList [R_ARM_CALL: .text+0x81f0] 
    /*00000b00*/ MOV         R1, R4 
    /*00000b04*/ MOV         R2, #16 
    /*00000b08*/ LDR         R0, [PC, #284]              @ 0x0000000000000c2c 
    /*00000b0c*/ MVN         R3, #0 
    /*00000b10*/ LDR         R8, [PC, #280]              @ 0x0000000000000c30 
    /*00000b14*/ STR         R3, [R5, #12] 
    /*00000b18*/ STR         R4, [R5, #8] 
    /*00000b1c*/ STR         R4, [R5, #16] 
    /*00000b20*/ BL          0x0000000000000B20          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00000b24*/ MOV         R1, R4 
    /*00000b28*/ MOV         R2, #8 
    /*00000b2c*/ LDR         R0, [PC, #256]              @ 0x0000000000000c34 
    /*00000b30*/ STR         R4, [R5, #36]               @ 0x24 
    /*00000b34*/ STR         R4, [R5, #40]               @ 0x28 
    /*00000b38*/ STR         R4, [R5, #44]               @ 0x2c 
    /*00000b3c*/ STR         R4, [R5, #48]               @ 0x30 
    /*00000b40*/ STR         R4, [R5, #60]               @ 0x3c 
    /*00000b44*/ STR         R4, [R5, #64]               @ 0x40 
    /*00000b48*/ STR         R4, [R5, #68]               @ 0x44 
    /*00000b4c*/ STR         R4, [R5, #72]               @ 0x48 
    /*00000b50*/ STR         R4, [R5, #52]               @ 0x34 
    /*00000b54*/ STR         R4, [R5, #56]               @ 0x38 
    /*00000b58*/ STR         R4, [R5, #196]              @ 0xc4 
    /*00000b5c*/ STR         R4, [R5, #192]              @ 0xc0 
    /*00000b60*/ BL          0x0000000000000B60          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00000b64*/ MOV         R1, R4 
    /*00000b68*/ MOV         R2, #8 
    /*00000b6c*/ LDR         R0, [PC, #196]              @ 0x0000000000000c38 
    /*00000b70*/ BL          0x0000000000000B70          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00000b74*/ MOV         R1, R4 
    /*00000b78*/ MOV         R2, #96                     @ 0x60 
    /*00000b7c*/ SUB         R0, R8, #192                @ 0xc0 
    /*00000b80*/ BL          0x0000000000000B80          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00000b84*/ MOV         R1, R4 
    /*00000b88*/ MOV         R2, #4 
    /*00000b8c*/ MOV         R0, R7 
    /*00000b90*/ BL          0x0000000000000B90          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00000b94*/ MOV         R1, R4 
    /*00000b98*/ MOV         R2, #4 
    /*00000b9c*/ MOV         R0, R6 
    /*00000ba0*/ BL          0x0000000000000BA0          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00000ba4*/ LDRD        R2, [R8, #-8] 
    /*00000ba8*/ ORRS        R1, R2, R3 
    /*00000bac*/ BNE         SVP_NNIE_DeInit_xbec
    SVP_NNIE_DeInit_xbb0:
    /*00000bb0*/ LDR         R4, [PC, #132]              @ 0x0000000000000c3c 
    /*00000bb4*/ LDRD        R0, [R4, #-8] 
    /*00000bb8*/ ORRS        R3, R0, R1 
    /*00000bbc*/ BNE         SVP_NNIE_DeInit_xbcc
    SVP_NNIE_DeInit_xbc0:
    /*00000bc0*/ MOV         R3, #0 
    /*00000bc4*/ STR         R3, [R5] 
    /*00000bc8*/ POP         {R4, R5, R6, R7, R8, PC} 
    SVP_NNIE_DeInit_xbcc:
    /*00000bcc*/ LDR         R2, [R5, #288]              @ 0x120 
    /*00000bd0*/ BL          0x0000000000000BD0          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*00000bd4*/ MOV         R0, #0 
    /*00000bd8*/ MOV         R1, #0 
    /*00000bdc*/ MOV         R3, #0 
    /*00000be0*/ STRD        R0, [R4, #-8] 
    /*00000be4*/ STR         R3, [R5, #288]              @ 0x120 
    /*00000be8*/ B           SVP_NNIE_DeInit_xbc0
    SVP_NNIE_DeInit_xbec:
    /*00000bec*/ MOV         R1, R2 
    /*00000bf0*/ LDR         R0, [R5, #272]              @ 0x110 
    /*00000bf4*/ LDR         R2, [R5, #276]              @ 0x114 
    /*00000bf8*/ BL          0x0000000000000BF8          @ osal_flush_dcache_area [R_ARM_CALL: +0x0] 
    /*00000bfc*/ LDRD        R0, [R8, #-8] 
    /*00000c00*/ ORRS        R3, R0, R1 
    /*00000c04*/ BEQ         SVP_NNIE_DeInit_xbb0
    /*00000c08*/ LDR         R2, [R5, #272]              @ 0x110 
    /*00000c0c*/ BL          0x0000000000000C0C          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*00000c10*/ MOV         R2, #0 
    /*00000c14*/ MOV         R3, #0 
    /*00000c18*/ STR         R4, [R5, #272]              @ 0x110 
    /*00000c1c*/ STRD        R2, [R8, #-8] 
    /*00000c20*/ B           SVP_NNIE_DeInit_xbb0
    /*00000c24*/ LDRDEQ      R0, [R0], -R8 
    /*00000c28*/ ANDEQ       R0, R0, R8 
    /*00000c2c*/ ANDEQ       R0, R0, R4, LSL R0 
    /*00000c30*/ ANDEQ       R0, R0, R0, LSL R1 
    /*00000c34*/ LDRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
    /*00000c38*/ ANDEQ       R0, R0, R8, ASR #1 
    /*00000c3c*/ ANDEQ       R0, R0, R0, LSR #2 
FUNC_END SVP_NNIE_DeInit


FUNC_BEGIN SVP_NNIE_GdcIntRoute
    /*00000c40*/ PUSH        {R4, R5, LR} 
    /*00000c44*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000c48*/ SUB         SP, SP, #12 
    /*00000c4c*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000c50*/ ADD         R0, R4, #248                @ 0xf8 
    /*00000c54*/ ADD         R1, SP, #4 
    /*00000c58*/ BL          0x0000000000000C58          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000c5c*/ LDR         R3, [R4, #12] 
    /*00000c60*/ CMN         R3, #1 
    /*00000c64*/ BEQ         SVP_NNIE_GdcIntRoute_xcbc
    /*00000c68*/ LDR         R2, [R4, #16] 
    /*00000c6c*/ CMP         R2, #0 
    /*00000c70*/ BEQ         SVP_NNIE_GdcIntRoute_xc8c
    SVP_NNIE_GdcIntRoute_xc74:
    /*00000c74*/ ADD         R1, SP, #4 
    /*00000c78*/ LDR         R0, [PC, #268]              @ 0x0000000000000d8c 
    /*00000c7c*/ BL          0x0000000000000C7C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    SVP_NNIE_GdcIntRoute_xc80:
    /*00000c80*/ MOV         R0, #1 
    /*00000c84*/ ADD         SP, SP, #12 
    /*00000c88*/ POP         {R4, R5, PC} 
    SVP_NNIE_GdcIntRoute_xc8c:
    /*00000c8c*/ ADD         R3, R3, #1 
    /*00000c90*/ ADD         R3, R4, R3, LSL #3 
    /*00000c94*/ LDR         R2, [R3, #12] 
    /*00000c98*/ LDR         R3, [R3, #16] 
    /*00000c9c*/ CMP         R2, R3 
    /*00000ca0*/ BCS         SVP_NNIE_GdcIntRoute_xc74
    /*00000ca4*/ LDR         R5, [R4, #72]               @ 0x48 
    /*00000ca8*/ CMP         R5, #0 
    /*00000cac*/ BEQ         SVP_NNIE_GdcIntRoute_xcfc
    SVP_NNIE_GdcIntRoute_xcb0:
    /*00000cb0*/ MOV         R0, #0 
    /*00000cb4*/ BL          0x0000000000000558 
    /*00000cb8*/ B           SVP_NNIE_GdcIntRoute_xc74
    SVP_NNIE_GdcIntRoute_xcbc:
    /*00000cbc*/ LDR         IP, [R4, #8] 
    /*00000cc0*/ ADD         R2, IP, #1 
    /*00000cc4*/ RSB         R1, IP, #1 
    /*00000cc8*/ STR         R1, [R4, #8] 
    /*00000ccc*/ ADD         R2, R4, R2, LSL #3 
    /*00000cd0*/ STR         IP, [R4, #12] 
    /*00000cd4*/ LDR         R1, [R2, #12] 
    /*00000cd8*/ LDR         R2, [R2, #16] 
    /*00000cdc*/ CMP         R1, R2 
    /*00000ce0*/ BCS         SVP_NNIE_GdcIntRoute_xd64
    /*00000ce4*/ LDR         R3, [R4, #16] 
    /*00000ce8*/ CMP         R3, #0 
    /*00000cec*/ BNE         SVP_NNIE_GdcIntRoute_xc74
    /*00000cf0*/ LDR         R5, [R4, #72]               @ 0x48 
    /*00000cf4*/ CMP         R5, #0 
    /*00000cf8*/ BNE         SVP_NNIE_GdcIntRoute_xcb0
    SVP_NNIE_GdcIntRoute_xcfc:
    /*00000cfc*/ MOV         R1, #1 
    /*00000d00*/ MOV         R0, R5 
    /*00000d04*/ BL          0x0000000000000D04          @ SVP_NNIE_DRV_EnableSysClk [R_ARM_CALL: .text+0xb54c] 
    /*00000d08*/ MOV         R1, R5 
    /*00000d0c*/ MOV         R0, R5 
    /*00000d10*/ BL          0x0000000000000D10          @ SVP_NNIE_DRV_SetReset [R_ARM_CALL: .text+0xb5b8] 
    /*00000d14*/ MOV         R0, R5 
    /*00000d18*/ BL          0x0000000000000D18          @ SVP_NNIE_DRV_EnInt [R_ARM_CALL: .text+0xb708] 
    /*00000d1c*/ MOV         R3, #255                    @ 0xff 
    /*00000d20*/ MVN         R2, #0 
    /*00000d24*/ MOV         R0, R5 
    /*00000d28*/ BL          0x0000000000000D28          @ SVP_NNIE_DRV_SetTimeOut [R_ARM_CALL: .text+0xb70c] 
    /*00000d2c*/ MOV         R0, R5 
    /*00000d30*/ BL          0x0000000000000D30          @ SVP_NNIE_DRV_EnClkGt [R_ARM_CALL: .text+0xb710] 
    /*00000d34*/ MOV         R0, R5 
    /*00000d38*/ BL          0x0000000000000D38          @ SVP_NNIE_DRV_SetMemSpeed [R_ARM_CALL: .text+0xb714] 
    /*00000d3c*/ MOV         R0, R5 
    /*00000d40*/ BL          0x0000000000000D40          @ SVP_NNIE_DRV_DisableCheckSum [R_ARM_CALL: .text+0xb7c8] 
    /*00000d44*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000d48*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000d4c*/ LDRB        R3, [R3, #2] 
    /*00000d50*/ CMP         R3, #0 
    /*00000d54*/ BNE         SVP_NNIE_GdcIntRoute_xd7c
    SVP_NNIE_GdcIntRoute_xd58:
    /*00000d58*/ MOV         R3, #1 
    /*00000d5c*/ STR         R3, [R4, #72]               @ 0x48 
    /*00000d60*/ B           SVP_NNIE_GdcIntRoute_xcb0
    SVP_NNIE_GdcIntRoute_xd64:
    /*00000d64*/ ADD         R1, SP, #4 
    /*00000d68*/ ADD         R0, R4, #248                @ 0xf8 
    /*00000d6c*/ STR         R3, [R4, #12] 
    /*00000d70*/ STR         IP, [R4, #8] 
    /*00000d74*/ BL          0x0000000000000D74          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000d78*/ B           SVP_NNIE_GdcIntRoute_xc80
    SVP_NNIE_GdcIntRoute_xd7c:
    /*00000d7c*/ MOV         R1, R5 
    /*00000d80*/ MOV         R0, R5 
    /*00000d84*/ BL          0x0000000000000D84          @ SVP_NNIE_DRV_EnableSysClk [R_ARM_CALL: .text+0xb54c] 
    /*00000d88*/ B           SVP_NNIE_GdcIntRoute_xd58
    /*00000d8c*/ STRDEQ      R0, [R0], -R8 
FUNC_END SVP_NNIE_GdcIntRoute


FUNC_BEGIN SVP_NNIE_Ioctl
    /*00000d90*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000d94*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000d98*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000d9c*/ SUB         SP, SP, #124                @ 0x7c 
    /*00000da0*/ LDR         R3, [R4] 
    /*00000da4*/ CMP         R3, #1 
    /*00000da8*/ BNE         SVP_NNIE_Ioctl_x14a0
    /*00000dac*/ MOV         R5, R0 
    /*00000db0*/ ADD         R0, R4, #224                @ 0xe0 
    /*00000db4*/ MOV         SL, R1 
    /*00000db8*/ BL          0x0000000000000DB8          @ osal_atomic_inc_return [R_ARM_CALL: +0x0] 
    /*00000dbc*/ MOVW        R3, #0                      @ reg_otp_base_va [R_ARM_MOVW_ABS_NC: +0x0] 
    /*00000dc0*/ MOVT        R3, #0                      @ reg_otp_base_va [R_ARM_MOVT_ABS: +0x0] 
    /*00000dc4*/ LDR         R3, [R3] 
    /*00000dc8*/ LDRB        R3, [R3, #40]               @ 0x28 
    /*00000dcc*/ CMP         R3, #2 
    /*00000dd0*/ BHI         SVP_NNIE_Ioctl_xf24
    SVP_NNIE_Ioctl_xdd4:
    /*00000dd4*/ BL          0x0000000000000DD4          @ SVP_NNIE_DRV_PrepareNnie [R_ARM_CALL: .text+0xb874] 
    /*00000dd8*/ SUBS        FP, R0, #0 
    /*00000ddc*/ BNE         SVP_NNIE_Ioctl_x1b2c
    /*00000de0*/ MOVW        R3, #19716                  @ 0x4d04 
    /*00000de4*/ MOVT        R3, #49176                  @ 0xc018 
    /*00000de8*/ CMP         R5, R3 
    /*00000dec*/ BEQ         SVP_NNIE_Ioctl_x13c4
    /*00000df0*/ BHI         SVP_NNIE_Ioctl_xf7c
    /*00000df4*/ MOVW        R3, #19714                  @ 0x4d02 
    /*00000df8*/ MOVT        R3, #49176                  @ 0xc018 
    /*00000dfc*/ CMP         R5, R3 
    /*00000e00*/ BEQ         SVP_NNIE_Ioctl_x110c
    /*00000e04*/ MOVW        R3, #19715                  @ 0x4d03 
    /*00000e08*/ MOVT        R3, #49176                  @ 0xc018 
    /*00000e0c*/ CMP         R5, R3 
    /*00000e10*/ BNE         SVP_NNIE_Ioctl_x1094
    /*00000e14*/ CMP         SL, #0 
    /*00000e18*/ BEQ         SVP_NNIE_Ioctl_x2100
    /*00000e1c*/ LDRD        R6, [SL] 
    /*00000e20*/ ORRS        R3, R6, R7 
    /*00000e24*/ BEQ         SVP_NNIE_Ioctl_x1ff8
    /*00000e28*/ AND         R2, R6, #15 
    /*00000e2c*/ MOV         R3, #0 
    /*00000e30*/ ORRS        R3, R2, R3 
    /*00000e34*/ BNE         SVP_NNIE_Ioctl_x1fe0
    /*00000e38*/ LDR         R5, [SL, #16] 
    /*00000e3c*/ CMP         R5, #0 
    /*00000e40*/ BEQ         SVP_NNIE_Ioctl_x1e48
    /*00000e44*/ MOV         R1, R5 
    /*00000e48*/ MOV         R0, R6 
    /*00000e4c*/ BL          0x0000000000000E4C          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00000e50*/ CMP         R0, #0 
    /*00000e54*/ BNE         SVP_NNIE_Ioctl_x1570
    SVP_NNIE_Ioctl_xe58:
    /*00000e58*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000e5c*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000e60*/ LDRH        R3, [R6] 
    /*00000e64*/ CMP         R3, #0 
    /*00000e68*/ BEQ         SVP_NNIE_Ioctl_x1efc
    /*00000e6c*/ LDR         R5, [PC, #3960]             @ 0x0000000000001dec 
    /*00000e70*/ MOV         R0, R5 
    /*00000e74*/ BL          0x0000000000000E74          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000e78*/ CMP         R0, #0 
    /*00000e7c*/ BNE         SVP_NNIE_Ioctl_x1494
    /*00000e80*/ LDR         R3, [R4, #52]               @ 0x34 
    /*00000e84*/ CMP         R3, #0 
    /*00000e88*/ BNE         SVP_NNIE_Ioctl_x10b0
    /*00000e8c*/ LDR         R3, [R4, #56]               @ 0x38 
    /*00000e90*/ CMP         R3, #0 
    /*00000e94*/ BEQ         SVP_NNIE_Ioctl_x109c
    SVP_NNIE_Ioctl_xe98:
    /*00000e98*/ LDRD        R2, [SL] 
    /*00000e9c*/ LDR         R0, [PC, #3916]             @ 0x0000000000001df0 
    /*00000ea0*/ BL          0x0000000000000EA0          @ SVP_NNIE_TSK_ADDR_FindNodeFromBusyList [R_ARM_CALL: .text+0x7ff8] 
    /*00000ea4*/ SUBS        R4, R0, #0 
    /*00000ea8*/ BEQ         SVP_NNIE_Ioctl_x10bc
    /*00000eac*/ MOV         R0, R5 
    /*00000eb0*/ MOVW        FP, #32772                  @ 0x8004 
    /*00000eb4*/ BL          0x0000000000000EB4          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000eb8*/ MOVW        R3, #1524                   @ 0x5f4 
    /*00000ebc*/ MOV         R1, #51                     @ 0x33 
    /*00000ec0*/ STR         R3, [SP] 
    /*00000ec4*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00000ec8*/ LDR         R3, [PC, #3888]             @ 0x0000000000001e00 
    /*00000ecc*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00000ed0*/ MOV         R0, #3 
    /*00000ed4*/ BL          0x0000000000000ED4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000ed8*/ MOV         R3, #0 
    /*00000edc*/ STR         R3, [SP, #12] 
    /*00000ee0*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbf8] 
    /*00000ee4*/ LDR         IP, [R4, #20] 
    /*00000ee8*/ MOV         R3, FP 
    /*00000eec*/ MOV         R1, #51                     @ 0x33 
    /*00000ef0*/ MOVT        R3, #41011                  @ 0xa033 
    /*00000ef4*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbf8] 
    /*00000ef8*/ MOV         R0, #3 
    /*00000efc*/ STR         IP, [SP, #8] 
    /*00000f00*/ MOVT        FP, #41011                  @ 0xa033 
    /*00000f04*/ LDRD        R4, [R4, #8] 
    /*00000f08*/ STRD        R4, [SP] 
    /*00000f0c*/ BL          0x0000000000000F0C          @ HI_LOG [R_ARM_CALL: +0x0] 
    SVP_NNIE_Ioctl_xf10:
    /*00000f10*/ LDR         R0, [PC, #3804]             @ 0x0000000000001df4 
    /*00000f14*/ BL          0x0000000000000F14          @ osal_atomic_dec_return [R_ARM_CALL: +0x0] 
    /*00000f18*/ MOV         R0, FP 
    SVP_NNIE_Ioctl_xf1c:
    /*00000f1c*/ ADD         SP, SP, #124                @ 0x7c 
    /*00000f20*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_Ioctl_xf24:
    /*00000f24*/ SUB         R3, R3, #5 
    /*00000f28*/ CMP         R3, #1 
    /*00000f2c*/ BLS         SVP_NNIE_Ioctl_xdd4
    /*00000f30*/ MOVW        R3, #2735                   @ 0xaaf 
    /*00000f34*/ MOV         R1, #51                     @ 0x33 
    /*00000f38*/ STR         R3, [SP] 
    /*00000f3c*/ MOVW        FP, #32776                  @ 0x8008 
    /*00000f40*/ LDR         R3, [PC, #3760]             @ 0x0000000000001df8 
    /*00000f44*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00000f48*/ MOV         R0, #3 
    /*00000f4c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00000f50*/ BL          0x0000000000000F50          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000f54*/ MOV         R3, FP 
    /*00000f58*/ STR         R5, [SP] 
    /*00000f5c*/ MOVW        R2, #0                      @ .LC64 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x728] 
    /*00000f60*/ MOVT        R3, #41011                  @ 0xa033 
    /*00000f64*/ MOVT        R2, #0                      @ .LC64 [R_ARM_MOVT_ABS: .rodata.str1.4+0x728] 
    /*00000f68*/ MOV         R1, #51                     @ 0x33 
    /*00000f6c*/ MOV         R0, #3 
    /*00000f70*/ BL          0x0000000000000F70          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000f74*/ MOVT        FP, #41011                  @ 0xa033 
    /*00000f78*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_xf7c:
    /*00000f7c*/ MOV         R3, #19712                  @ 0x4d00 
    /*00000f80*/ MOVT        R3, #50776                  @ 0xc658 
    /*00000f84*/ CMP         R5, R3 
    /*00000f88*/ BEQ         SVP_NNIE_Ioctl_x12ac
    /*00000f8c*/ MOVW        R3, #19713                  @ 0x4d01 
    /*00000f90*/ MOVT        R3, #50880                  @ 0xc6c0 
    /*00000f94*/ CMP         R5, R3 
    /*00000f98*/ BNE         SVP_NNIE_Ioctl_x1094
    /*00000f9c*/ ADD         R9, SP, #56                 @ 0x38 
    /*00000fa0*/ MOV         R2, #64                     @ 0x40 
    /*00000fa4*/ MOV         R1, FP 
    /*00000fa8*/ MOV         R0, R9 
    /*00000fac*/ BL          0x0000000000000FAC          @ memset [R_ARM_CALL: +0x0] 
    /*00000fb0*/ CMP         SL, #0 
    /*00000fb4*/ BEQ         SVP_NNIE_Ioctl_x21a4
    /*00000fb8*/ LDR         R3, [SL, #1664]             @ 0x680 
    /*00000fbc*/ CMP         R3, #0 
    /*00000fc0*/ BNE         SVP_NNIE_Ioctl_x1e14
    /*00000fc4*/ LDR         R5, [PC, #3616]             @ 0x0000000000001dec 
    /*00000fc8*/ LDR         R3, [SL, #1720]             @ 0x6b8 
    /*00000fcc*/ MOV         R0, R5 
    /*00000fd0*/ STR         R3, [SP, #28] 
    /*00000fd4*/ BL          0x0000000000000FD4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000fd8*/ CMP         R0, #0 
    /*00000fdc*/ BNE         SVP_NNIE_Ioctl_x1494
    /*00000fe0*/ MOVW        R2, #13992                  @ 0x36a8 
    /*00000fe4*/ LDR         R1, [SL, #872]              @ 0x368 
    /*00000fe8*/ LDR         R0, [R4, #184]              @ 0xb8 
    /*00000fec*/ BL          0x0000000000000FEC          @ osal_copy_from_user [R_ARM_CALL: +0x0] 
    /*00000ff0*/ CMP         R0, #0 
    /*00000ff4*/ BNE         SVP_NNIE_Ioctl_x1ecc
    /*00000ff8*/ LDR         R3, [R4, #184]              @ 0xb8 
    /*00000ffc*/ ADD         R7, SL, #8 
    /*00001000*/ ADD         R6, SL, #880                @ 0x370 
    /*00001004*/ ADD         R1, SL, #776                @ 0x308 
    /*00001008*/ MOV         R0, R7 
    /*0000100c*/ ADD         R8, SL, #1648               @ 0x670 
    /*00001010*/ MOV         R2, R3 
    /*00001014*/ STR         R3, [SP, #32] 
    /*00001018*/ LDR         R3, [SL, #1720]             @ 0x6b8 
    /*0000101c*/ STR         R8, [SP] 
    /*00001020*/ STR         R1, [SP, #36]               @ 0x24 
    /*00001024*/ STR         R3, [SP, #4] 
    /*00001028*/ MOV         R3, R6 
    /*0000102c*/ BL          0x000000000000102C          @ SVP_NNIE_CheckForwardWithBboxParamKernel [R_ARM_CALL: .text+0x6d60] 
    /*00001030*/ SUBS        FP, R0, #0 
    /*00001034*/ BNE         SVP_NNIE_Ioctl_x17f0
    /*00001038*/ LDR         R2, [SP, #28] 
    /*0000103c*/ MOV         R1, R7 
    /*00001040*/ STR         R9, [SP, #12] 
    /*00001044*/ LDR         R3, [SP, #32] 
    /*00001048*/ STR         R2, [SP, #8] 
    /*0000104c*/ STR         R8, [SP, #4] 
    /*00001050*/ LDR         R2, [SP, #36]               @ 0x24 
    /*00001054*/ STR         R6, [SP] 
    /*00001058*/ LDR         R0, [PC, #3472]             @ 0x0000000000001df0 
    /*0000105c*/ BL          0x000000000000105C          @ SVP_NNIE_FillForwardWithRoiTask [R_ARM_CALL: .text+0x8d68] 
    /*00001060*/ SUBS        FP, R0, #0 
    /*00001064*/ BNE         SVP_NNIE_Ioctl_x1720
    /*00001068*/ LDR         IP, [R4, #168]              @ 0xa8 
    /*0000106c*/ MOV         R2, SL 
    /*00001070*/ LDR         R3, [SP, #28] 
    /*00001074*/ MOV         R1, R9 
    /*00001078*/ ADD         IP, IP, #1 
    /*0000107c*/ STR         IP, [R4, #168]              @ 0xa8 
    /*00001080*/ BL          0x0000000000000768 
    /*00001084*/ MOV         FP, R0 
    /*00001088*/ MOV         R0, R5 
    /*0000108c*/ BL          0x000000000000108C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001090*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1094:
    /*00001094*/ MVN         FP, #0 
    /*00001098*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x109c:
    /*0000109c*/ LDRH        R1, [R6] 
    /*000010a0*/ SUB         R0, R5, #208                @ 0xd0 
    /*000010a4*/ BL          0x00000000000010A4          @ SVP_NNIE_TSK_ADDR_InitList [R_ARM_CALL: .text+0x80f0] 
    /*000010a8*/ SUBS        R6, R0, #0 
    /*000010ac*/ BNE         SVP_NNIE_Ioctl_x17a8
    SVP_NNIE_Ioctl_x10b0:
    /*000010b0*/ LDR         R3, [R4, #56]               @ 0x38 
    /*000010b4*/ CMP         R3, #0 
    /*000010b8*/ BNE         SVP_NNIE_Ioctl_xe98
    SVP_NNIE_Ioctl_x10bc:
    /*000010bc*/ LDR         R2, [SL, #16] 
    /*000010c0*/ LDRD        R0, [SL] 
    /*000010c4*/ BL          0x00000000000010C4          @ CMPI_Remap_Cached [R_ARM_CALL: +0x0] 
    /*000010c8*/ SUBS        R4, R0, #0 
    /*000010cc*/ BEQ         SVP_NNIE_Ioctl_x19a0
    /*000010d0*/ LDR         R0, [PC, #3352]             @ 0x0000000000001df0 
    /*000010d4*/ BL          0x00000000000010D4          @ SVP_NNIE_TSK_ADDR_GetNodeFromFreeList [R_ARM_CALL: .text+0x7f9c] 
    /*000010d8*/ SUBS        IP, R0, #0 
    /*000010dc*/ BEQ         SVP_NNIE_Ioctl_x19f8
    /*000010e0*/ LDRD        R2, [SL] 
    /*000010e4*/ MOV         R1, IP 
    /*000010e8*/ LDR         R0, [PC, #3328]             @ 0x0000000000001df0 
    /*000010ec*/ STRD        R2, [IP, #8] 
    /*000010f0*/ LDR         R3, [SL, #16] 
    /*000010f4*/ STR         R4, [IP, #16] 
    /*000010f8*/ STR         R3, [IP, #20] 
    /*000010fc*/ BL          0x00000000000010FC          @ SVP_NNIE_TSK_ADDR_PutNodeToBusyList [R_ARM_CALL: .text+0x7f78] 
    /*00001100*/ MOV         R0, R5 
    /*00001104*/ BL          0x0000000000001104          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001108*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x110c:
    /*0000110c*/ CMP         SL, #0 
    /*00001110*/ BEQ         SVP_NNIE_Ioctl_x2054
    /*00001114*/ LDM         SL, {R0, R1, R2} 
    /*00001118*/ BL          0x0000000000001118          @ SVP_NNIE_CheckQueryParamKernel [R_ARM_CALL: .text+0x7e28] 
    /*0000111c*/ SUBS        FP, R0, #0 
    /*00001120*/ BNE         SVP_NNIE_Ioctl_x1534
    /*00001124*/ LDR         R3, [SL] 
    /*00001128*/ MOV         R6, #216                    @ 0xd8 
    /*0000112c*/ LDR         R1, [SL, #8] 
    /*00001130*/ LDR         R2, [SL, #4] 
    /*00001134*/ STR         R3, [SP, #52]               @ 0x34 
    /*00001138*/ MOV         R7, R3 
    /*0000113c*/ MUL         R3, R6, R3 
    /*00001140*/ STR         R1, [SP, #28] 
    /*00001144*/ LDR         R1, [PC, #3236]             @ 0x0000000000001df0 
    /*00001148*/ STR         R2, [SP, #36]               @ 0x24 
    /*0000114c*/ STR         R3, [SP, #32] 
    /*00001150*/ MOV         R9, R3 
    /*00001154*/ ADD         R3, R3, #208                @ 0xd0 
    /*00001158*/ ADD         R3, R1, R3 
    /*0000115c*/ STR         R3, [SP, #48]               @ 0x30 
    /*00001160*/ MOV         R0, R3 
    /*00001164*/ MOV         R5, R3 
    /*00001168*/ BL          0x0000000000001168          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*0000116c*/ CMP         R0, #0 
    /*00001170*/ BNE         SVP_NNIE_Ioctl_x1494
    /*00001174*/ MOVW        R8, #65535                  @ 0xffff 
    /*00001178*/ STR         FP, [SP, #40]               @ 0x28 
    /*0000117c*/ STR         SL, [SP, #44]               @ 0x2c 
    /*00001180*/ ADD         R9, R4, R9 
    /*00001184*/ MOVT        R8, #4094                   @ 0xffe 
    /*00001188*/ MOV         FP, R7 
    /*0000118c*/ LDR         SL, [SP, #36]               @ 0x24 
    /*00001190*/ B           SVP_NNIE_Ioctl_x11c8
    SVP_NNIE_Ioctl_x1194:
    /*00001194*/ CMP         LR, R8 
    /*00001198*/ BLS         SVP_NNIE_Ioctl_x124c
    SVP_NNIE_Ioctl_x119c:
    /*0000119c*/ BL          0x000000000000119C          @ osal_msecs_to_jiffies [R_ARM_CALL: +0x0] 
    SVP_NNIE_Ioctl_x11a0:
    /*000011a0*/ LDR         R3, [R4] 
    /*000011a4*/ CMP         R3, #1 
    /*000011a8*/ BNE         SVP_NNIE_Ioctl_x1c5c
    /*000011ac*/ CMP         R0, #0 
    /*000011b0*/ BLT         SVP_NNIE_Ioctl_x1494
    /*000011b4*/ BEQ         SVP_NNIE_Ioctl_x1cac
    /*000011b8*/ MOV         R0, R5 
    /*000011bc*/ BL          0x00000000000011BC          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000011c0*/ CMP         R0, #0 
    /*000011c4*/ BNE         SVP_NNIE_Ioctl_x1494
    SVP_NNIE_Ioctl_x11c8:
    /*000011c8*/ LDR         R3, [R9, #40]               @ 0x28 
    /*000011cc*/ CMP         R3, SL 
    /*000011d0*/ BHI         SVP_NNIE_Ioctl_x14e8
    /*000011d4*/ SUB         R3, SL, R3 
    /*000011d8*/ CMP         R3, R8 
    /*000011dc*/ BHI         SVP_NNIE_Ioctl_x14f4
    SVP_NNIE_Ioctl_x11e0:
    /*000011e0*/ LDR         R3, [R9, #64]               @ 0x40 
    /*000011e4*/ CMP         R3, #0 
    /*000011e8*/ BNE         SVP_NNIE_Ioctl_x2148
    /*000011ec*/ LDR         R3, [R9, #68]               @ 0x44 
    /*000011f0*/ CMP         R3, #0 
    /*000011f4*/ BNE         SVP_NNIE_Ioctl_x20a4
    /*000011f8*/ MOV         R0, R5 
    /*000011fc*/ BL          0x00000000000011FC          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001200*/ LDR         R3, [SP, #28] 
    /*00001204*/ CMP         R3, #0 
    /*00001208*/ BEQ         SVP_NNIE_Ioctl_x1c9c
    /*0000120c*/ LDR         R3, [SP, #32] 
    /*00001210*/ MOV         R2, SL 
    /*00001214*/ LDR         R1, [R9, #40]               @ 0x28 
    /*00001218*/ MOV         IP, FP 
    /*0000121c*/ ADD         R7, R3, #212                @ 0xd4 
    /*00001220*/ LDR         R3, [PC, #3016]             @ 0x0000000000001df0 
    /*00001224*/ MOVW        R0, #50000                  @ 0xc350 
    /*00001228*/ STR         SL, [SP, #56]               @ 0x38 
    /*0000122c*/ ADD         R7, R3, R7 
    /*00001230*/ STR         FP, [SP, #60]               @ 0x3c 
    SVP_NNIE_Ioctl_x1234:
    /*00001234*/ CMP         R2, R1 
    /*00001238*/ SUB         LR, R2, R1 
    /*0000123c*/ SUB         R3, R1, R2 
    /*00001240*/ BCS         SVP_NNIE_Ioctl_x1194
    /*00001244*/ CMP         R3, R8 
    /*00001248*/ BLS         SVP_NNIE_Ioctl_x119c
    SVP_NNIE_Ioctl_x124c:
    /*0000124c*/ MLA         IP, R6, IP, R4 
    /*00001250*/ LDR         R3, [IP, #64]               @ 0x40 
    /*00001254*/ CMP         R3, #0 
    /*00001258*/ BNE         SVP_NNIE_Ioctl_x119c
    /*0000125c*/ LDR         R3, [IP, #68]               @ 0x44 
    /*00001260*/ CMP         R3, #0 
    /*00001264*/ BNE         SVP_NNIE_Ioctl_x119c
    /*00001268*/ LDR         R3, [R4] 
    /*0000126c*/ CMP         R3, #1 
    /*00001270*/ BNE         SVP_NNIE_Ioctl_x119c
    /*00001274*/ MOV         R3, R0 
    /*00001278*/ MOVW        R1, #0                      @ SVP_NNIE_WaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x47c] 
    /*0000127c*/ ADD         R2, SP, #56                 @ 0x38 
    /*00001280*/ MOVT        R1, #0                      @ SVP_NNIE_WaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x47c] 
    /*00001284*/ MOV         R0, R7 
    /*00001288*/ BL          0x0000000000001288          @ osal_wait_timeout_interruptible [R_ARM_CALL: +0x0] 
    /*0000128c*/ ADD         R3, R0, #512                @ 0x200 
    /*00001290*/ BICS        R3, R3, #512                @ 0x200 
    /*00001294*/ BEQ         SVP_NNIE_Ioctl_x11a0
    /*00001298*/ LDR         IP, [SP, #60]               @ 0x3c 
    /*0000129c*/ LDR         R2, [SP, #56]               @ 0x38 
    /*000012a0*/ MLA         R3, R6, IP, R4 
    /*000012a4*/ LDR         R1, [R3, #40]               @ 0x28 
    /*000012a8*/ B           SVP_NNIE_Ioctl_x1234
    SVP_NNIE_Ioctl_x12ac:
    /*000012ac*/ ADD         R9, SP, #56                 @ 0x38 
    /*000012b0*/ MOV         R2, #64                     @ 0x40 
    /*000012b4*/ MOV         R1, FP 
    /*000012b8*/ MOV         R0, R9 
    /*000012bc*/ BL          0x00000000000012BC          @ memset [R_ARM_CALL: +0x0] 
    /*000012c0*/ CMP         SL, #0 
    /*000012c4*/ BEQ         SVP_NNIE_Ioctl_x2254
    /*000012c8*/ LDR         R3, [SL, #1564]             @ 0x61c 
    /*000012cc*/ CMP         R3, #0 
    /*000012d0*/ BNE         SVP_NNIE_Ioctl_x1d90
    /*000012d4*/ LDR         R5, [PC, #2832]             @ 0x0000000000001dec 
    /*000012d8*/ LDR         R3, [SL, #1616]             @ 0x650 
    /*000012dc*/ MOV         R0, R5 
    /*000012e0*/ STR         R3, [SP, #28] 
    /*000012e4*/ BL          0x00000000000012E4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000012e8*/ CMP         R0, #0 
    /*000012ec*/ BNE         SVP_NNIE_Ioctl_x1494
    /*000012f0*/ MOVW        R2, #13992                  @ 0x36a8 
    /*000012f4*/ LDR         R1, [SL, #776]              @ 0x308 
    /*000012f8*/ LDR         R0, [R4, #184]              @ 0xb8 
    /*000012fc*/ BL          0x00000000000012FC          @ osal_copy_from_user [R_ARM_CALL: +0x0] 
    /*00001300*/ CMP         R0, #0 
    /*00001304*/ BNE         SVP_NNIE_Ioctl_x1e84
    /*00001308*/ ADD         R3, SL, #8 
    /*0000130c*/ LDR         R8, [R4, #184]              @ 0xb8 
    /*00001310*/ ADD         R1, SL, #1552               @ 0x610 
    /*00001314*/ STR         R3, [SP, #36]               @ 0x24 
    /*00001318*/ MOV         R0, R3 
    /*0000131c*/ LDR         R3, [SL, #1616]             @ 0x650 
    /*00001320*/ MOV         IP, R1 
    /*00001324*/ ADD         R2, SL, #784                @ 0x310 
    /*00001328*/ STR         R1, [SP, #40]               @ 0x28 
    /*0000132c*/ MOV         R1, R8 
    /*00001330*/ STR         R3, [SP] 
    /*00001334*/ MOV         R3, IP 
    /*00001338*/ STR         R2, [SP, #32] 
    /*0000133c*/ BL          0x000000000000133C          @ SVP_NNIE_CheckForwardParamKernel [R_ARM_CALL: .text+0x64a0] 
    /*00001340*/ SUBS        FP, R0, #0 
    /*00001344*/ BNE         SVP_NNIE_Ioctl_x195c
    /*00001348*/ LDR         R3, [SL, #1560]             @ 0x618 
    /*0000134c*/ MOVW        R2, #1692                   @ 0x69c 
    /*00001350*/ MLA         R3, R2, R3, R8 
    /*00001354*/ LDR         R3, [R3, #12] 
    /*00001358*/ CMP         R3, #2 
    /*0000135c*/ BEQ         SVP_NNIE_Ioctl_x1608
    SVP_NNIE_Ioctl_x1360:
    /*00001360*/ LDR         R0, [R4, #192]              @ 0xc0 
    /*00001364*/ MOV         R2, R8 
    /*00001368*/ LDR         R1, [SP, #28] 
    /*0000136c*/ LDR         IP, [SP, #40]               @ 0x28 
    /*00001370*/ STR         R0, [SP, #8] 
    /*00001374*/ STR         R1, [SP, #4] 
    /*00001378*/ LDR         R3, [SP, #32] 
    /*0000137c*/ STR         R9, [SP, #12] 
    /*00001380*/ LDR         R1, [SP, #36]               @ 0x24 
    /*00001384*/ STR         IP, [SP] 
    /*00001388*/ LDR         R0, [PC, #2656]             @ 0x0000000000001df0 
    /*0000138c*/ BL          0x000000000000138C          @ SVP_NNIE_FillForwardTask [R_ARM_CALL: .text+0x82f4] 
    /*00001390*/ SUBS        FP, R0, #0 
    /*00001394*/ BNE         SVP_NNIE_Ioctl_x1764
    /*00001398*/ LDR         IP, [R4, #164]              @ 0xa4 
    /*0000139c*/ MOV         R2, SL 
    /*000013a0*/ LDR         R3, [SP, #28] 
    /*000013a4*/ MOV         R1, R9 
    /*000013a8*/ ADD         IP, IP, #1 
    /*000013ac*/ STR         IP, [R4, #164]              @ 0xa4 
    /*000013b0*/ BL          0x0000000000000768 
    /*000013b4*/ MOV         FP, R0 
    /*000013b8*/ MOV         R0, R5 
    /*000013bc*/ BL          0x00000000000013BC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000013c0*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x13c4:
    /*000013c4*/ CMP         SL, #0 
    /*000013c8*/ BEQ         SVP_NNIE_Ioctl_x223c
    /*000013cc*/ LDRD        R6, [SL] 
    /*000013d0*/ ORRS        R3, R6, R7 
    /*000013d4*/ BEQ         SVP_NNIE_Ioctl_x1f90
    /*000013d8*/ AND         R2, R6, #15 
    /*000013dc*/ MOV         R3, #0 
    /*000013e0*/ ORRS        R3, R2, R3 
    /*000013e4*/ BNE         SVP_NNIE_Ioctl_x1f38
    /*000013e8*/ LDR         R5, [SL, #16] 
    /*000013ec*/ CMP         R5, #0 
    /*000013f0*/ BEQ         SVP_NNIE_Ioctl_x1ee4
    /*000013f4*/ MOV         R1, R5 
    /*000013f8*/ MOV         R0, R6 
    /*000013fc*/ BL          0x00000000000013FC          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00001400*/ CMP         R0, #0 
    /*00001404*/ BNE         SVP_NNIE_Ioctl_x159c
    SVP_NNIE_Ioctl_x1408:
    /*00001408*/ LDR         R5, [PC, #2524]             @ 0x0000000000001dec 
    /*0000140c*/ MOV         R0, R5 
    /*00001410*/ BL          0x0000000000001410          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001414*/ CMP         R0, #0 
    /*00001418*/ BNE         SVP_NNIE_Ioctl_x1494
    /*0000141c*/ LDR         R3, [R4, #52]               @ 0x34 
    /*00001420*/ CMP         R3, #0 
    /*00001424*/ BNE         SVP_NNIE_Ioctl_x1434
    /*00001428*/ LDR         R4, [R4, #56]               @ 0x38 
    /*0000142c*/ CMP         R4, #0 
    /*00001430*/ BEQ         SVP_NNIE_Ioctl_x1a50
    SVP_NNIE_Ioctl_x1434:
    /*00001434*/ LDRD        R2, [SL] 
    /*00001438*/ LDR         R0, [PC, #2480]             @ 0x0000000000001df0 
    /*0000143c*/ BL          0x000000000000143C          @ SVP_NNIE_TSK_ADDR_DeleteNodeFromBusyList [R_ARM_CALL: .text+0x8058] 
    /*00001440*/ SUBS        R4, R0, #0 
    /*00001444*/ BEQ         SVP_NNIE_Ioctl_x18a8
    /*00001448*/ LDR         R3, [R4, #16] 
    /*0000144c*/ CMP         R3, #0 
    /*00001450*/ BEQ         SVP_NNIE_Ioctl_x1904
    /*00001454*/ LDR         R0, [PC, #2452]             @ 0x0000000000001df0 
    /*00001458*/ MOV         R1, R4 
    /*0000145c*/ BL          0x000000000000145C          @ SVP_NNIE_TSK_ADDR_PutNodeToFreeList [R_ARM_CALL: .text+0x7f54] 
    /*00001460*/ LDR         R0, [R4, #16] 
    /*00001464*/ CMP         R0, #0 
    /*00001468*/ BEQ         SVP_NNIE_Ioctl_x1470
    /*0000146c*/ BL          0x000000000000146C          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    SVP_NNIE_Ioctl_x1470:
    /*00001470*/ MOV         R3, #0 
    /*00001474*/ MOV         R6, #0 
    /*00001478*/ MOV         R7, #0 
    /*0000147c*/ MOV         R0, R5 
    /*00001480*/ STR         R3, [R4, #16] 
    /*00001484*/ STR         R3, [R4, #20] 
    /*00001488*/ STRD        R6, [R4, #8] 
    /*0000148c*/ BL          0x000000000000148C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001490*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1494:
    /*00001494*/ MOV         FP, #65024                  @ 0xfe00 
    /*00001498*/ MOVT        FP, #65535                  @ 0xffff 
    /*0000149c*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x14a0:
    /*000014a0*/ MOVW        R3, #2923                   @ 0xb6b 
    /*000014a4*/ MOV         R1, #51                     @ 0x33 
    /*000014a8*/ STR         R3, [SP] 
    /*000014ac*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000014b0*/ LDR         R3, [PC, #2372]             @ 0x0000000000001dfc 
    /*000014b4*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000014b8*/ MOV         R0, #3 
    /*000014bc*/ BL          0x00000000000014BC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000014c0*/ MOVW        R3, #32784                  @ 0x8010 
    /*000014c4*/ MOVW        R2, #0                      @ .LC63 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x704] 
    /*000014c8*/ MOVT        R3, #41011                  @ 0xa033 
    /*000014cc*/ MOVT        R2, #0                      @ .LC63 [R_ARM_MOVT_ABS: .rodata.str1.4+0x704] 
    /*000014d0*/ MOV         R1, #51                     @ 0x33 
    /*000014d4*/ MOV         R0, #3 
    /*000014d8*/ BL          0x00000000000014D8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000014dc*/ MOVW        R0, #32784                  @ 0x8010 
    /*000014e0*/ MOVT        R0, #41011                  @ 0xa033 
    /*000014e4*/ B           SVP_NNIE_Ioctl_xf1c
    SVP_NNIE_Ioctl_x14e8:
    /*000014e8*/ SUB         R3, R3, SL 
    /*000014ec*/ CMP         R3, R8 
    /*000014f0*/ BHI         SVP_NNIE_Ioctl_x11e0
    SVP_NNIE_Ioctl_x14f4:
    /*000014f4*/ LDR         R2, [SP, #52]               @ 0x34 
    /*000014f8*/ MOV         R3, #216                    @ 0xd8 
    /*000014fc*/ LDR         FP, [SP, #40]               @ 0x28 
    /*00001500*/ LDR         SL, [SP, #44]               @ 0x2c 
    /*00001504*/ MLA         R3, R3, R2, R4 
    /*00001508*/ LDR         R2, [R3, #64]               @ 0x40 
    /*0000150c*/ CMP         R2, #0 
    /*00001510*/ BNE         SVP_NNIE_Ioctl_x2148
    /*00001514*/ LDR         R3, [R3, #68]               @ 0x44 
    /*00001518*/ CMP         R3, #0 
    /*0000151c*/ BNE         SVP_NNIE_Ioctl_x20a4
    /*00001520*/ LDR         R0, [SP, #48]               @ 0x30 
    /*00001524*/ BL          0x0000000000001524          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001528*/ MOV         R3, #1 
    /*0000152c*/ STR         R3, [SL, #12] 
    /*00001530*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1534:
    /*00001534*/ MOVW        R3, #1352                   @ 0x548 
    /*00001538*/ MOV         R1, #51                     @ 0x33 
    /*0000153c*/ STR         R3, [SP] 
    /*00001540*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001544*/ LDR         R3, [PC, #2232]             @ 0x0000000000001e04 
    /*00001548*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*0000154c*/ MOV         R0, #3 
    /*00001550*/ BL          0x0000000000001550          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001554*/ MOV         R3, FP 
    /*00001558*/ MOVW        R2, #0                      @ .LC78 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9b8] 
    /*0000155c*/ MOV         R1, #51                     @ 0x33 
    /*00001560*/ MOVT        R2, #0                      @ .LC78 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9b8] 
    /*00001564*/ MOV         R0, #3 
    /*00001568*/ BL          0x0000000000001568          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000156c*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1570:
    /*00001570*/ MOV         R1, R5 
    /*00001574*/ MOV         R0, R6 
    /*00001578*/ BL          0x0000000000001578          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*0000157c*/ CMP         R0, #0 
    /*00001580*/ BEQ         SVP_NNIE_Ioctl_xe58
    /*00001584*/ MOVW        R3, #1484                   @ 0x5cc 
    /*00001588*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*0000158c*/ STR         R3, [SP] 
    /*00001590*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001594*/ LDR         R3, [PC, #2148]             @ 0x0000000000001e00 
    /*00001598*/ B           SVP_NNIE_Ioctl_x15c4
    SVP_NNIE_Ioctl_x159c:
    /*0000159c*/ MOV         R1, R5 
    /*000015a0*/ MOV         R0, R6 
    /*000015a4*/ BL          0x00000000000015A4          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*000015a8*/ CMP         R0, #0 
    /*000015ac*/ BEQ         SVP_NNIE_Ioctl_x1408
    /*000015b0*/ MOVW        R3, #1608                   @ 0x648 
    /*000015b4*/ STR         R3, [SP] 
    /*000015b8*/ LDR         R3, [PC, #2124]             @ 0x0000000000001e0c 
    /*000015bc*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000015c0*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    SVP_NNIE_Ioctl_x15c4:
    /*000015c4*/ MOV         R1, #51                     @ 0x33 
    /*000015c8*/ MOV         R0, #3 
    /*000015cc*/ BL          0x00000000000015CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000015d0*/ LDRD        R0, [SL] 
    /*000015d4*/ LDR         IP, [SL, #16] 
    /*000015d8*/ MOVW        FP, #32771                  @ 0x8003 
    /*000015dc*/ MOV         R3, FP 
    /*000015e0*/ MOVW        R2, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe70] 
    /*000015e4*/ STRD        R0, [SP] 
    /*000015e8*/ MOVT        R3, #41011                  @ 0xa033 
    /*000015ec*/ STR         IP, [SP, #8] 
    /*000015f0*/ MOVT        R2, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe70] 
    /*000015f4*/ MOV         R1, #51                     @ 0x33 
    /*000015f8*/ MOV         R0, #3 
    /*000015fc*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001600*/ BL          0x0000000000001600          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001604*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1608:
    /*00001608*/ LDR         R3, [SL, #8] 
    /*0000160c*/ CMP         R3, #5 
    /*00001610*/ BNE         SVP_NNIE_Ioctl_x2010
    /*00001614*/ LDR         R6, [R4, #192]              @ 0xc0 
    /*00001618*/ CMP         R6, #0 
    /*0000161c*/ BEQ         SVP_NNIE_Ioctl_x1d20
    SVP_NNIE_Ioctl_x1620:
    /*00001620*/ LDR         R2, [SL, #32] 
    /*00001624*/ MOV         R0, R6 
    /*00001628*/ LDR         R1, [SL, #48]               @ 0x30 
    /*0000162c*/ LSL         R2, R2, #2 
    /*00001630*/ BL          0x0000000000001630          @ osal_copy_from_user [R_ARM_CALL: +0x0] 
    /*00001634*/ CMP         R0, #0 
    /*00001638*/ BNE         SVP_NNIE_Ioctl_x21ec
    /*0000163c*/ LDR         R1, [SL, #32] 
    /*00001640*/ CMP         R1, #0 
    /*00001644*/ BEQ         SVP_NNIE_Ioctl_x209c
    /*00001648*/ LDR         R3, [SL, #1560]             @ 0x618 
    /*0000164c*/ MOVW        R2, #1692                   @ 0x69c 
    /*00001650*/ LDR         FP, [R6] 
    /*00001654*/ MLA         R3, R2, R3, R8 
    /*00001658*/ CLZ         R7, FP 
    /*0000165c*/ LSR         R7, R7, #5 
    /*00001660*/ LDRH        IP, [R3, #22] 
    /*00001664*/ CMP         IP, FP 
    /*00001668*/ ORRCC       R7, R7, #1 
    /*0000166c*/ CMP         R7, #0 
    /*00001670*/ ADDEQ       R0, R6, #4 
    /*00001674*/ BEQ         SVP_NNIE_Ioctl_x169c
    /*00001678*/ B           SVP_NNIE_Ioctl_x1834
    SVP_NNIE_Ioctl_x167c:
    /*0000167c*/ LDR         R2, [R0], #4 
    /*00001680*/ CLZ         R3, R2 
    /*00001684*/ ADD         FP, FP, R2 
    /*00001688*/ LSR         R3, R3, #5 
    /*0000168c*/ CMP         R2, IP 
    /*00001690*/ ORRHI       R3, R3, #1 
    /*00001694*/ CMP         R3, #0 
    /*00001698*/ BNE         SVP_NNIE_Ioctl_x1838
    SVP_NNIE_Ioctl_x169c:
    /*0000169c*/ ADD         R7, R7, #1 
    /*000016a0*/ MOV         R6, R0 
    /*000016a4*/ CMP         R7, R1 
    /*000016a8*/ BNE         SVP_NNIE_Ioctl_x167c
    SVP_NNIE_Ioctl_x16ac:
    /*000016ac*/ LDR         R2, [SL, #1552]             @ 0x610 
    /*000016b0*/ CMP         R2, #0 
    /*000016b4*/ BEQ         SVP_NNIE_Ioctl_x1b7c
    /*000016b8*/ MOV         R6, #0 
    /*000016bc*/ LSL         R3, FP, #2 
    /*000016c0*/ STR         R4, [SP, #52]               @ 0x34 
    /*000016c4*/ MOV         R7, SL 
    /*000016c8*/ STR         R3, [SP, #44]               @ 0x2c 
    /*000016cc*/ MOV         R4, R6 
    /*000016d0*/ STR         R5, [SP, #48]               @ 0x30 
    /*000016d4*/ B           SVP_NNIE_Ioctl_x16e8
    SVP_NNIE_Ioctl_x16d8:
    /*000016d8*/ ADD         R4, R4, #1 
    /*000016dc*/ ADD         R7, R7, #48                 @ 0x30 
    /*000016e0*/ CMP         R4, R2 
    /*000016e4*/ BCS         SVP_NNIE_Ioctl_x1b74
    SVP_NNIE_Ioctl_x16e8:
    /*000016e8*/ LDR         R1, [R7, #8] 
    /*000016ec*/ CMP         R1, #5 
    /*000016f0*/ BNE         SVP_NNIE_Ioctl_x16d8
    /*000016f4*/ LDR         R6, [R7, #40]               @ 0x28 
    /*000016f8*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*000016fc*/ LDR         R5, [R7, #24] 
    /*00001700*/ MUL         R6, R6, R3 
    /*00001704*/ MOV         R0, R5 
    /*00001708*/ MOV         R1, R6 
    /*0000170c*/ BL          0x000000000000170C          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00001710*/ CMP         R0, #0 
    /*00001714*/ BNE         SVP_NNIE_Ioctl_x1aa4
    SVP_NNIE_Ioctl_x1718:
    /*00001718*/ LDR         R2, [SL, #1552]             @ 0x610 
    /*0000171c*/ B           SVP_NNIE_Ioctl_x16d8
    SVP_NNIE_Ioctl_x1720:
    /*00001720*/ MOVW        R3, #1215                   @ 0x4bf 
    /*00001724*/ MOV         R1, #51                     @ 0x33 
    /*00001728*/ STR         R3, [SP] 
    /*0000172c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001730*/ LDR         R3, [PC, #1744]             @ 0x0000000000001e08 
    /*00001734*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001738*/ MOV         R0, #3 
    /*0000173c*/ BL          0x000000000000173C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001740*/ MOV         R3, FP 
    /*00001744*/ MOV         R1, #51                     @ 0x33 
    /*00001748*/ MOVW        R2, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x988] 
    /*0000174c*/ MOV         R0, #3 
    /*00001750*/ MOVT        R2, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x988] 
    /*00001754*/ BL          0x0000000000001754          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001758*/ MOV         R0, R5 
    /*0000175c*/ BL          0x000000000000175C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001760*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1764:
    /*00001764*/ MOVW        R3, #1138                   @ 0x472 
    /*00001768*/ MOV         R1, #51                     @ 0x33 
    /*0000176c*/ STR         R3, [SP] 
    /*00001770*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001774*/ LDR         R3, [PC, #1684]             @ 0x0000000000001e10 
    /*00001778*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*0000177c*/ MOV         R0, #3 
    /*00001780*/ BL          0x0000000000001780          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001784*/ MOV         R3, FP 
    /*00001788*/ MOV         R1, #51                     @ 0x33 
    /*0000178c*/ MOVW        R2, #0                      @ .LC74 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x900] 
    /*00001790*/ MOV         R0, #3 
    /*00001794*/ MOVT        R2, #0                      @ .LC74 [R_ARM_MOVT_ABS: .rodata.str1.4+0x900] 
    /*00001798*/ BL          0x0000000000001798          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000179c*/ MOV         R0, R5 
    /*000017a0*/ BL          0x00000000000017A0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000017a4*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x17a8:
    /*000017a8*/ MOV         R0, R5 
    /*000017ac*/ MOV         FP, R6 
    /*000017b0*/ BL          0x00000000000017B0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000017b4*/ MOVW        R3, #1509                   @ 0x5e5 
    /*000017b8*/ MOV         R1, #51                     @ 0x33 
    /*000017bc*/ STR         R3, [SP] 
    /*000017c0*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000017c4*/ LDR         R3, [PC, #1588]             @ 0x0000000000001e00 
    /*000017c8*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000017cc*/ MOV         R0, #3 
    /*000017d0*/ BL          0x00000000000017D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000017d4*/ MOV         R3, R6 
    /*000017d8*/ MOVW        R2, #0                      @ .LC88 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbc8] 
    /*000017dc*/ MOV         R1, #51                     @ 0x33 
    /*000017e0*/ MOVT        R2, #0                      @ .LC88 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbc8] 
    /*000017e4*/ MOV         R0, #3 
    /*000017e8*/ BL          0x00000000000017E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000017ec*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x17f0:
    /*000017f0*/ MOVW        R3, #1206                   @ 0x4b6 
    /*000017f4*/ MOV         R1, #51                     @ 0x33 
    /*000017f8*/ STR         R3, [SP] 
    /*000017fc*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001800*/ LDR         R3, [PC, #1536]             @ 0x0000000000001e08 
    /*00001804*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001808*/ MOV         R0, #3 
    /*0000180c*/ BL          0x000000000000180C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001810*/ MOV         R3, FP 
    /*00001814*/ MOV         R1, #51                     @ 0x33 
    /*00001818*/ MOVW        R2, #0                      @ .LC76 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x954] 
    /*0000181c*/ MOV         R0, #3 
    /*00001820*/ MOVT        R2, #0                      @ .LC76 [R_ARM_MOVT_ABS: .rodata.str1.4+0x954] 
    /*00001824*/ BL          0x0000000000001824          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001828*/ MOV         R0, R5 
    /*0000182c*/ BL          0x000000000000182C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001830*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1834:
    /*00001834*/ MOV         R7, R0 
    SVP_NNIE_Ioctl_x1838:
    /*00001838*/ MOVW        R3, #1090                   @ 0x442 
    /*0000183c*/ MOV         R1, #51                     @ 0x33 
    /*00001840*/ STR         R3, [SP] 
    /*00001844*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001848*/ LDR         R3, [PC, #1472]             @ 0x0000000000001e10 
    /*0000184c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001850*/ MOV         R0, #3 
    /*00001854*/ MOVW        FP, #32771                  @ 0x8003 
    /*00001858*/ BL          0x0000000000001858          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000185c*/ LDR         IP, [SL, #1560]             @ 0x618 
    /*00001860*/ MOVW        R1, #1692                   @ 0x69c 
    /*00001864*/ MOV         R3, FP 
    /*00001868*/ MOVW        R2, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8a0] 
    /*0000186c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00001870*/ MLA         R8, R1, IP, R8 
    /*00001874*/ MOVT        R2, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8a0] 
    /*00001878*/ MOV         R1, #51                     @ 0x33 
    /*0000187c*/ MOV         R0, #3 
    /*00001880*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001884*/ LDRH        LR, [R8, #22] 
    /*00001888*/ STR         IP, [SP, #8] 
    /*0000188c*/ STR         LR, [SP, #12] 
    /*00001890*/ LDR         IP, [R6] 
    /*00001894*/ STM         SP, {R7, IP} 
    /*00001898*/ BL          0x0000000000001898          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000189c*/ MOV         R0, R5 
    /*000018a0*/ BL          0x00000000000018A0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000018a4*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x18a8:
    /*000018a8*/ MOV         R0, R5 
    /*000018ac*/ MOVW        FP, #32771                  @ 0x8003 
    /*000018b0*/ BL          0x00000000000018B0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000018b4*/ MOVW        R3, #1639                   @ 0x667 
    /*000018b8*/ MOV         R1, #51                     @ 0x33 
    /*000018bc*/ STR         R3, [SP] 
    /*000018c0*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000018c4*/ LDR         R3, [PC, #1344]             @ 0x0000000000001e0c 
    /*000018c8*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000018cc*/ MOV         R0, #3 
    /*000018d0*/ BL          0x00000000000018D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000018d4*/ LDRD        R0, [SL] 
    /*000018d8*/ MOV         R3, FP 
    /*000018dc*/ STR         R4, [SP, #8] 
    /*000018e0*/ MOVW        R2, #0                      @ .LC93 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd1c] 
    /*000018e4*/ MOVT        R3, #41011                  @ 0xa033 
    /*000018e8*/ STRD        R0, [SP] 
    /*000018ec*/ MOVT        R2, #0                      @ .LC93 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd1c] 
    /*000018f0*/ MOV         R1, #51                     @ 0x33 
    /*000018f4*/ MOV         R0, #3 
    /*000018f8*/ BL          0x00000000000018F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000018fc*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001900*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1904:
    /*00001904*/ MOV         R0, R5 
    /*00001908*/ MOVW        FP, #32771                  @ 0x8003 
    /*0000190c*/ BL          0x000000000000190C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001910*/ MOV         R3, #1648                   @ 0x670 
    /*00001914*/ MOV         R1, #51                     @ 0x33 
    /*00001918*/ STR         R3, [SP] 
    /*0000191c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001920*/ LDR         R3, [PC, #1252]             @ 0x0000000000001e0c 
    /*00001924*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001928*/ MOV         R0, #3 
    /*0000192c*/ BL          0x000000000000192C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001930*/ LDR         R1, [R4, #16] 
    /*00001934*/ MOV         R3, FP 
    /*00001938*/ MOVW        R2, #0                      @ .LC94 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd68] 
    /*0000193c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00001940*/ MOVT        R2, #0                      @ .LC94 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd68] 
    SVP_NNIE_Ioctl_x1944:
    /*00001944*/ STR         R1, [SP] 
    /*00001948*/ MOV         R0, #3 
    /*0000194c*/ MOV         R1, #51                     @ 0x33 
    /*00001950*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001954*/ BL          0x0000000000001954          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001958*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x195c:
    /*0000195c*/ MOVW        R3, #1041                   @ 0x411 
    /*00001960*/ MOV         R1, #51                     @ 0x33 
    /*00001964*/ STR         R3, [SP] 
    /*00001968*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*0000196c*/ LDR         R3, [PC, #1180]             @ 0x0000000000001e10 
    /*00001970*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001974*/ MOV         R0, #3 
    /*00001978*/ BL          0x0000000000001978          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000197c*/ MOV         R3, FP 
    /*00001980*/ MOV         R1, #51                     @ 0x33 
    /*00001984*/ MOVW        R2, #0                      @ .LC69 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7d8] 
    /*00001988*/ MOV         R0, #3 
    /*0000198c*/ MOVT        R2, #0                      @ .LC69 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7d8] 
    /*00001990*/ BL          0x0000000000001990          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001994*/ MOV         R0, R5 
    /*00001998*/ BL          0x0000000000001998          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000199c*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x19a0:
    /*000019a0*/ MOV         R0, R5 
    /*000019a4*/ MOVW        FP, #32780                  @ 0x800c 
    /*000019a8*/ BL          0x00000000000019A8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000019ac*/ MOV         R3, #1536                   @ 0x600 
    /*000019b0*/ MOV         R1, #51                     @ 0x33 
    /*000019b4*/ STR         R3, [SP] 
    /*000019b8*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000019bc*/ LDR         R3, [PC, #1084]             @ 0x0000000000001e00 
    /*000019c0*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000019c4*/ MOV         R0, #3 
    /*000019c8*/ BL          0x00000000000019C8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000019cc*/ LDR         R3, [SL, #16] 
    /*000019d0*/ LDRD        R4, [SL] 
    /*000019d4*/ MOVW        R2, #0                      @ .LC90 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc54] 
    /*000019d8*/ MOV         R1, #51                     @ 0x33 
    /*000019dc*/ MOVT        R2, #0                      @ .LC90 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc54] 
    /*000019e0*/ STR         R3, [SP, #8] 
    /*000019e4*/ MOV         R0, #3 
    /*000019e8*/ STRD        R4, [SP] 
    /*000019ec*/ MOVT        FP, #41011                  @ 0xa033 
    /*000019f0*/ BL          0x00000000000019F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000019f4*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x19f8:
    /*000019f8*/ MOV         R0, R5 
    /*000019fc*/ MOVW        FP, #32783                  @ 0x800f 
    /*00001a00*/ BL          0x0000000000001A00          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001a04*/ MOV         R0, R4 
    /*00001a08*/ BL          0x0000000000001A08          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    /*00001a0c*/ MOVW        R3, #1551                   @ 0x60f 
    /*00001a10*/ MOV         R1, #51                     @ 0x33 
    /*00001a14*/ STR         R3, [SP] 
    /*00001a18*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001a1c*/ LDR         R3, [PC, #988]              @ 0x0000000000001e00 
    /*00001a20*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001a24*/ MOV         R0, #3 
    /*00001a28*/ BL          0x0000000000001A28          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001a2c*/ MOV         R3, FP 
    /*00001a30*/ MOVW        R2, #0                      @ .LC91 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc90] 
    /*00001a34*/ MOVT        R3, #41011                  @ 0xa033 
    /*00001a38*/ MOVT        R2, #0                      @ .LC91 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc90] 
    /*00001a3c*/ MOV         R1, #51                     @ 0x33 
    /*00001a40*/ MOV         R0, #3 
    /*00001a44*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001a48*/ BL          0x0000000000001A48          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001a4c*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1a50:
    /*00001a50*/ MOV         R0, R5 
    /*00001a54*/ MOVW        FP, #32784                  @ 0x8010 
    /*00001a58*/ BL          0x0000000000001A58          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001a5c*/ MOVW        R3, #1628                   @ 0x65c 
    /*00001a60*/ MOV         R1, #51                     @ 0x33 
    /*00001a64*/ STR         R3, [SP] 
    /*00001a68*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001a6c*/ LDR         R3, [PC, #920]              @ 0x0000000000001e0c 
    /*00001a70*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001a74*/ MOV         R0, #3 
    /*00001a78*/ BL          0x0000000000001A78          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001a7c*/ MOV         R3, FP 
    /*00001a80*/ STR         R4, [SP] 
    /*00001a84*/ MOVW        R2, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xce4] 
    /*00001a88*/ MOVT        R3, #41011                  @ 0xa033 
    /*00001a8c*/ MOVT        R2, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0xce4] 
    /*00001a90*/ MOV         R1, #51                     @ 0x33 
    /*00001a94*/ MOV         R0, #3 
    /*00001a98*/ BL          0x0000000000001A98          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001a9c*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001aa0*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1aa4:
    /*00001aa4*/ MOV         R0, R5 
    /*00001aa8*/ MOV         R1, R6 
    /*00001aac*/ BL          0x0000000000001AAC          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*00001ab0*/ CMP         R0, #0 
    /*00001ab4*/ BEQ         SVP_NNIE_Ioctl_x1718
    /*00001ab8*/ MOVW        R3, #1108                   @ 0x454 
    /*00001abc*/ MOV         R1, #51                     @ 0x33 
    /*00001ac0*/ STR         R3, [SP] 
    /*00001ac4*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001ac8*/ LDR         R3, [PC, #832]              @ 0x0000000000001e10 
    /*00001acc*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001ad0*/ MOV         R0, #3 
    /*00001ad4*/ LDR         R5, [SP, #48]               @ 0x30 
    /*00001ad8*/ BL          0x0000000000001AD8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001adc*/ MOV         IP, #48                     @ 0x30 
    /*00001ae0*/ MLA         IP, IP, R4, SL 
    /*00001ae4*/ MOV         R7, R6 
    /*00001ae8*/ MOVW        FP, #32771                  @ 0x8003 
    /*00001aec*/ STR         R7, [SP, #16] 
    /*00001af0*/ MOV         R3, FP 
    /*00001af4*/ MOVW        R2, #0                      @ .LC95 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdb0] 
    /*00001af8*/ MOVT        R3, #41011                  @ 0xa033 
    /*00001afc*/ MOVT        R2, #0                      @ .LC95 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdb0] 
    /*00001b00*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001b04*/ MOV         R6, R4 
    /*00001b08*/ LDRD        R8, [IP, #24] 
    /*00001b0c*/ MOV         R1, #51                     @ 0x33 
    /*00001b10*/ MOV         R0, #3 
    SVP_NNIE_Ioctl_x1b14:
    /*00001b14*/ STR         R6, [SP] 
    /*00001b18*/ STRD        R8, [SP, #8] 
    /*00001b1c*/ BL          0x0000000000001B1C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001b20*/ MOV         R0, R5 
    /*00001b24*/ BL          0x0000000000001B24          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001b28*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1b2c:
    /*00001b2c*/ MOVW        R3, #2742                   @ 0xab6 
    /*00001b30*/ MOV         R1, #51                     @ 0x33 
    /*00001b34*/ STR         R3, [SP] 
    /*00001b38*/ MOVW        FP, #32777                  @ 0x8009 
    /*00001b3c*/ LDR         R3, [PC, #692]              @ 0x0000000000001df8 
    /*00001b40*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001b44*/ MOV         R0, #3 
    /*00001b48*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001b4c*/ BL          0x0000000000001B4C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001b50*/ MOV         R3, FP 
    /*00001b54*/ MOVW        R2, #0                      @ .LC65 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x754] 
    /*00001b58*/ MOVT        R3, #41011                  @ 0xa033 
    /*00001b5c*/ MOVT        R2, #0                      @ .LC65 [R_ARM_MOVT_ABS: .rodata.str1.4+0x754] 
    /*00001b60*/ MOV         R1, #51                     @ 0x33 
    /*00001b64*/ MOV         R0, #3 
    /*00001b68*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001b6c*/ BL          0x0000000000001B6C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001b70*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1b74:
    /*00001b74*/ LDR         R4, [SP, #52]               @ 0x34 
    /*00001b78*/ LDR         R5, [SP, #48]               @ 0x30 
    SVP_NNIE_Ioctl_x1b7c:
    /*00001b7c*/ LDR         R2, [SL, #1556]             @ 0x614 
    /*00001b80*/ CMP         R2, #0 
    /*00001b84*/ BEQ         SVP_NNIE_Ioctl_x1360
    /*00001b88*/ LSL         R3, FP, #2 
    /*00001b8c*/ MOV         R6, #0 
    /*00001b90*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00001b94*/ LDR         FP, [SP, #32] 
    /*00001b98*/ STR         R4, [SP, #48]               @ 0x30 
    /*00001b9c*/ B           SVP_NNIE_Ioctl_x1bb0
    SVP_NNIE_Ioctl_x1ba0:
    /*00001ba0*/ ADD         R6, R6, #1 
    /*00001ba4*/ ADD         FP, FP, #48                 @ 0x30 
    /*00001ba8*/ CMP         R6, R2 
    /*00001bac*/ BCS         SVP_NNIE_Ioctl_x1c54
    SVP_NNIE_Ioctl_x1bb0:
    /*00001bb0*/ LDR         R3, [FP] 
    /*00001bb4*/ CMP         R3, #5 
    /*00001bb8*/ BNE         SVP_NNIE_Ioctl_x1ba0
    /*00001bbc*/ LDR         R3, [FP, #32] 
    /*00001bc0*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*00001bc4*/ LDR         R4, [FP, #16] 
    /*00001bc8*/ MUL         R7, R3, R2 
    /*00001bcc*/ MOV         R0, R4 
    /*00001bd0*/ MOV         R1, R7 
    /*00001bd4*/ BL          0x0000000000001BD4          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00001bd8*/ CMP         R0, #0 
    /*00001bdc*/ BNE         SVP_NNIE_Ioctl_x1be8
    SVP_NNIE_Ioctl_x1be0:
    /*00001be0*/ LDR         R2, [SL, #1556]             @ 0x614 
    /*00001be4*/ B           SVP_NNIE_Ioctl_x1ba0
    SVP_NNIE_Ioctl_x1be8:
    /*00001be8*/ MOV         R0, R4 
    /*00001bec*/ MOV         R1, R7 
    /*00001bf0*/ BL          0x0000000000001BF0          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*00001bf4*/ CMP         R0, #0 
    /*00001bf8*/ BEQ         SVP_NNIE_Ioctl_x1be0
    /*00001bfc*/ MOVW        R3, #1126                   @ 0x466 
    /*00001c00*/ MOV         R1, #51                     @ 0x33 
    /*00001c04*/ STR         R3, [SP] 
    /*00001c08*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001c0c*/ LDR         R3, [PC, #508]              @ 0x0000000000001e10 
    /*00001c10*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001c14*/ MOV         R0, #3 
    /*00001c18*/ MOVW        FP, #32771                  @ 0x8003 
    /*00001c1c*/ BL          0x0000000000001C1C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001c20*/ MOV         IP, #48                     @ 0x30 
    /*00001c24*/ MLA         IP, IP, R6, SL 
    /*00001c28*/ MOV         R3, FP 
    /*00001c2c*/ STR         R7, [SP, #16] 
    /*00001c30*/ MOVW        R2, #0                      @ .LC96 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe10] 
    /*00001c34*/ MOVT        R3, #41011                  @ 0xa033 
    /*00001c38*/ MOVT        R2, #0                      @ .LC96 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe10] 
    /*00001c3c*/ ADD         IP, IP, #800                @ 0x320 
    /*00001c40*/ MOV         R1, #51                     @ 0x33 
    /*00001c44*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001c48*/ MOV         R0, #3 
    /*00001c4c*/ LDRD        R8, [IP] 
    /*00001c50*/ B           SVP_NNIE_Ioctl_x1b14
    SVP_NNIE_Ioctl_x1c54:
    /*00001c54*/ LDR         R4, [SP, #48]               @ 0x30 
    /*00001c58*/ B           SVP_NNIE_Ioctl_x1360
    SVP_NNIE_Ioctl_x1c5c:
    /*00001c5c*/ MOVW        R3, #1389                   @ 0x56d 
    /*00001c60*/ MOV         R1, #51                     @ 0x33 
    /*00001c64*/ STR         R3, [SP] 
    /*00001c68*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001c6c*/ LDR         R3, [PC, #400]              @ 0x0000000000001e04 
    /*00001c70*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001c74*/ MOV         R0, #0 
    /*00001c78*/ MOVW        FP, #32773                  @ 0x8005 
    /*00001c7c*/ BL          0x0000000000001C7C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001c80*/ MOVW        R2, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9e0] 
    /*00001c84*/ MOV         R1, #51                     @ 0x33 
    /*00001c88*/ MOVT        R2, #0                      @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9e0] 
    /*00001c8c*/ MOV         R0, #0 
    /*00001c90*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001c94*/ BL          0x0000000000001C94          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001c98*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1c9c:
    /*00001c9c*/ LDR         SL, [SP, #44]               @ 0x2c 
    /*00001ca0*/ LDR         FP, [SP, #40]               @ 0x28 
    /*00001ca4*/ STR         R3, [SL, #12] 
    /*00001ca8*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1cac:
    /*00001cac*/ MOV         R5, R0 
    /*00001cb0*/ LDR         R0, [SP, #48]               @ 0x30 
    /*00001cb4*/ BL          0x0000000000001CB4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001cb8*/ CMP         R0, #0 
    /*00001cbc*/ BNE         SVP_NNIE_Ioctl_x1494
    /*00001cc0*/ LDR         R2, [SP, #52]               @ 0x34 
    /*00001cc4*/ MOV         R3, #216                    @ 0xd8 
    /*00001cc8*/ LDR         R0, [SP, #48]               @ 0x30 
    /*00001ccc*/ MOVW        FP, #32833                  @ 0x8041 
    /*00001cd0*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001cd4*/ MLA         R4, R3, R2, R4 
    /*00001cd8*/ LDR         R3, [R4, #152]              @ 0x98 
    /*00001cdc*/ ADD         R3, R3, #1 
    /*00001ce0*/ STR         R3, [R4, #152]              @ 0x98 
    /*00001ce4*/ BL          0x0000000000001CE4          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001ce8*/ MOV         R3, #1408                   @ 0x580 
    /*00001cec*/ MOV         R1, #51                     @ 0x33 
    /*00001cf0*/ STR         R3, [SP] 
    /*00001cf4*/ MOV         R0, R5 
    /*00001cf8*/ LDR         R3, [PC, #260]              @ 0x0000000000001e04 
    /*00001cfc*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001d00*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001d04*/ BL          0x0000000000001D04          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001d08*/ MOV         R0, R5 
    /*00001d0c*/ MOVW        R2, #0                      @ .LC80 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9f4] 
    /*00001d10*/ MOV         R1, #51                     @ 0x33 
    /*00001d14*/ MOVT        R2, #0                      @ .LC80 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9f4] 
    /*00001d18*/ BL          0x0000000000001D18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001d1c*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1d20:
    /*00001d20*/ MOV         R0, #1024                   @ 0x400 
    /*00001d24*/ BL          0x0000000000001D24          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*00001d28*/ CMP         R0, #0 
    /*00001d2c*/ MOV         R6, R0 
    /*00001d30*/ STR         R0, [R4, #192]              @ 0xc0 
    /*00001d34*/ BNE         SVP_NNIE_Ioctl_x1620
    /*00001d38*/ MOVW        R3, #1065                   @ 0x429 
    /*00001d3c*/ MOV         R1, #51                     @ 0x33 
    /*00001d40*/ STR         R3, [SP] 
    /*00001d44*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001d48*/ LDR         R3, [PC, #192]              @ 0x0000000000001e10 
    /*00001d4c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001d50*/ MOV         R0, #3 
    /*00001d54*/ MOVW        FP, #32780                  @ 0x800c 
    /*00001d58*/ BL          0x0000000000001D58          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001d5c*/ LDR         R1, [SL, #1564]             @ 0x61c 
    /*00001d60*/ MOV         R3, FP 
    /*00001d64*/ MOVW        R2, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x834] 
    /*00001d68*/ MOVT        R3, #41011                  @ 0xa033 
    /*00001d6c*/ MOVT        R2, #0                      @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0x834] 
    /*00001d70*/ STR         R1, [SP] 
    /*00001d74*/ MOV         R1, #51                     @ 0x33 
    SVP_NNIE_Ioctl_x1d78:
    /*00001d78*/ MOV         R0, #3 
    /*00001d7c*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001d80*/ BL          0x0000000000001D80          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001d84*/ MOV         R0, R5 
    /*00001d88*/ BL          0x0000000000001D88          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001d8c*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1d90:
    /*00001d90*/ MOVW        R3, #1019                   @ 0x3fb 
    /*00001d94*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001d98*/ STR         R3, [SP] 
    /*00001d9c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001da0*/ LDR         R3, [PC, #104]              @ 0x0000000000001e10 
    /*00001da4*/ MOV         R1, #51                     @ 0x33 
    /*00001da8*/ MOV         R0, #3 
    /*00001dac*/ BL          0x0000000000001DAC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001db0*/ STR         FP, [SP, #4] 
    /*00001db4*/ MOV         R3, #1 
    /*00001db8*/ STR         R3, [SP, #8] 
    /*00001dbc*/ LDR         R0, [SL, #1564]             @ 0x61c 
    SVP_NNIE_Ioctl_x1dc0:
    /*00001dc0*/ STR         R0, [SP] 
    /*00001dc4*/ MOVW        R3, #32771                  @ 0x8003 
    /*00001dc8*/ MOVW        R2, #0                      @ .LC67 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x794] 
    /*00001dcc*/ MOVT        R3, #41011                  @ 0xa033 
    /*00001dd0*/ MOVT        R2, #0                      @ .LC67 [R_ARM_MOVT_ABS: .rodata.str1.4+0x794] 
    /*00001dd4*/ MOV         R1, #51                     @ 0x33 
    /*00001dd8*/ MOV         R0, #3 
    /*00001ddc*/ MOVW        FP, #32771                  @ 0x8003 
    /*00001de0*/ BL          0x0000000000001DE0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001de4*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001de8*/ B           SVP_NNIE_Ioctl_xf10
    /*00001dec*/ LDRDEQ      R0, [R0], -R8 
    /*00001df0*/ ANDEQ       R0, R0, R8 
    /*00001df4*/ ANDEQ       R0, R0, R0, ROR #1 
    /*00001df8*/ ANDEQ       R0, R0, IP, ROR R0 
    /*00001dfc*/ ANDEQ       R0, R0, IP, RRX 
    /*00001e00*/ ANDEQ       R0, R0, R8, ASR #1 
    /*00001e04*/ STRHEQ      R0, [R0], -R8 
    /*00001e08*/ ANDEQ       R0, R0, R0, LSR #1 
    /*00001e0c*/ LDRDEQ      R0, [R0], -IP 
    /*00001e10*/ MULEQ       R0, R0, R0 
    SVP_NNIE_Ioctl_x1e14:
    /*00001e14*/ MOVW        R3, #1183                   @ 0x49f 
    /*00001e18*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001e1c*/ STR         R3, [SP] 
    /*00001e20*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001e24*/ LDR         R3, [PC, #-36]              @ 0x0000000000001e08 
    /*00001e28*/ MOV         R1, #51                     @ 0x33 
    /*00001e2c*/ MOV         R0, #3 
    /*00001e30*/ BL          0x0000000000001E30          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001e34*/ STR         FP, [SP, #4] 
    /*00001e38*/ MOV         R3, #1 
    /*00001e3c*/ STR         R3, [SP, #8] 
    /*00001e40*/ LDR         R0, [SL, #1664]             @ 0x680 
    /*00001e44*/ B           SVP_NNIE_Ioctl_x1dc0
    SVP_NNIE_Ioctl_x1e48:
    /*00001e48*/ MOVW        R3, #1480                   @ 0x5c8 
    /*00001e4c*/ STR         R3, [SP] 
    /*00001e50*/ LDR         R3, [PC, #-88]              @ 0x0000000000001e00 
    /*00001e54*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001e58*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    SVP_NNIE_Ioctl_x1e5c:
    /*00001e5c*/ MOV         R1, #51                     @ 0x33 
    /*00001e60*/ MOV         R0, #3 
    /*00001e64*/ MOVW        FP, #32771                  @ 0x8003 
    /*00001e68*/ BL          0x0000000000001E68          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001e6c*/ MOV         R3, FP 
    /*00001e70*/ MOVW        R2, #0                      @ .LC86 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb2c] 
    /*00001e74*/ LDR         R1, [SL, #16] 
    /*00001e78*/ MOVT        R3, #41011                  @ 0xa033 
    /*00001e7c*/ MOVT        R2, #0                      @ .LC86 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb2c] 
    /*00001e80*/ B           SVP_NNIE_Ioctl_x1944
    SVP_NNIE_Ioctl_x1e84:
    /*00001e84*/ MOVW        R3, #1032                   @ 0x408 
    /*00001e88*/ STR         R3, [SP] 
    /*00001e8c*/ LDR         R3, [PC, #-132]             @ 0x0000000000001e10 
    /*00001e90*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001e94*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    SVP_NNIE_Ioctl_x1e98:
    /*00001e98*/ MOV         R1, #51                     @ 0x33 
    /*00001e9c*/ MOV         R0, #3 
    /*00001ea0*/ BL          0x0000000000001EA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001ea4*/ MOV         R1, #51                     @ 0x33 
    /*00001ea8*/ MOVW        R2, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7c0] 
    /*00001eac*/ MOV         R0, #3 
    /*00001eb0*/ MOVT        R2, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7c0] 
    /*00001eb4*/ MOVW        FP, #32785                  @ 0x8011 
    /*00001eb8*/ BL          0x0000000000001EB8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001ebc*/ MOV         R0, R5 
    /*00001ec0*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001ec4*/ BL          0x0000000000001EC4          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001ec8*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1ecc:
    /*00001ecc*/ MOVW        R3, #1197                   @ 0x4ad 
    /*00001ed0*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001ed4*/ STR         R3, [SP] 
    /*00001ed8*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001edc*/ LDR         R3, [PC, #-220]             @ 0x0000000000001e08 
    /*00001ee0*/ B           SVP_NNIE_Ioctl_x1e98
    SVP_NNIE_Ioctl_x1ee4:
    /*00001ee4*/ MOVW        R3, #1604                   @ 0x644 
    /*00001ee8*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001eec*/ STR         R3, [SP] 
    /*00001ef0*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001ef4*/ LDR         R3, [PC, #-240]             @ 0x0000000000001e0c 
    /*00001ef8*/ B           SVP_NNIE_Ioctl_x1e5c
    SVP_NNIE_Ioctl_x1efc:
    /*00001efc*/ MOVW        R3, #1489                   @ 0x5d1 
    /*00001f00*/ MOV         R1, #51                     @ 0x33 
    /*00001f04*/ STR         R3, [SP] 
    /*00001f08*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001f0c*/ LDR         R3, [PC, #-276]             @ 0x0000000000001e00 
    /*00001f10*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001f14*/ MOV         R0, #3 
    /*00001f18*/ MOVW        FP, #32771                  @ 0x8003 
    /*00001f1c*/ BL          0x0000000000001F1C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001f20*/ MOV         R3, FP 
    /*00001f24*/ MOVW        R2, #0                      @ .LC87 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb6c] 
    /*00001f28*/ LDRH        R1, [R6] 
    /*00001f2c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00001f30*/ MOVT        R2, #0                      @ .LC87 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb6c] 
    /*00001f34*/ B           SVP_NNIE_Ioctl_x1944
    SVP_NNIE_Ioctl_x1f38:
    /*00001f38*/ MOVW        R3, #1601                   @ 0x641 
    /*00001f3c*/ STR         R3, [SP] 
    /*00001f40*/ LDR         R3, [PC, #-316]             @ 0x0000000000001e0c 
    /*00001f44*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001f48*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    SVP_NNIE_Ioctl_x1f4c:
    /*00001f4c*/ MOV         R1, #51                     @ 0x33 
    /*00001f50*/ MOV         R0, #3 
    /*00001f54*/ BL          0x0000000000001F54          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001f58*/ LDRD        R0, [SL] 
    /*00001f5c*/ MOVW        FP, #32771                  @ 0x8003 
    /*00001f60*/ MOV         R2, #16 
    /*00001f64*/ MOV         R3, FP 
    /*00001f68*/ STR         R2, [SP, #8] 
    /*00001f6c*/ STRD        R0, [SP] 
    /*00001f70*/ MOVT        R3, #41011                  @ 0xa033 
    /*00001f74*/ MOVW        R2, #0                      @ .LC85 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xad8] 
    /*00001f78*/ MOV         R1, #51                     @ 0x33 
    /*00001f7c*/ MOVT        R2, #0                      @ .LC85 [R_ARM_MOVT_ABS: .rodata.str1.4+0xad8] 
    /*00001f80*/ MOV         R0, #3 
    /*00001f84*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001f88*/ BL          0x0000000000001F88          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001f8c*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1f90:
    /*00001f90*/ MOVW        R3, #1598                   @ 0x63e 
    /*00001f94*/ STR         R3, [SP] 
    /*00001f98*/ LDR         R3, [PC, #-404]             @ 0x0000000000001e0c 
    /*00001f9c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001fa0*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    SVP_NNIE_Ioctl_x1fa4:
    /*00001fa4*/ MOV         R1, #51                     @ 0x33 
    /*00001fa8*/ MOV         R0, #3 
    /*00001fac*/ BL          0x0000000000001FAC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001fb0*/ LDRD        R0, [SL] 
    /*00001fb4*/ MOVW        FP, #32771                  @ 0x8003 
    /*00001fb8*/ MOVW        R2, #0                      @ .LC84 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa94] 
    /*00001fbc*/ MOV         R3, FP 
    /*00001fc0*/ MOVT        R2, #0                      @ .LC84 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa94] 
    /*00001fc4*/ STRD        R0, [SP] 
    /*00001fc8*/ MOVT        R3, #41011                  @ 0xa033 
    /*00001fcc*/ MOV         R1, #51                     @ 0x33 
    /*00001fd0*/ MOV         R0, #3 
    /*00001fd4*/ MOVT        FP, #41011                  @ 0xa033 
    /*00001fd8*/ BL          0x0000000000001FD8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001fdc*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x1fe0:
    /*00001fe0*/ MOVW        R3, #1477                   @ 0x5c5 
    /*00001fe4*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001fe8*/ STR         R3, [SP] 
    /*00001fec*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001ff0*/ LDR         R3, [PC, #-504]             @ 0x0000000000001e00 
    /*00001ff4*/ B           SVP_NNIE_Ioctl_x1f4c
    SVP_NNIE_Ioctl_x1ff8:
    /*00001ff8*/ MOVW        R3, #1474                   @ 0x5c2 
    /*00001ffc*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002000*/ STR         R3, [SP] 
    /*00002004*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00002008*/ LDR         R3, [PC, #-528]             @ 0x0000000000001e00 
    /*0000200c*/ B           SVP_NNIE_Ioctl_x1fa4
    SVP_NNIE_Ioctl_x2010:
    /*00002010*/ MOVW        R3, #1053                   @ 0x41d 
    /*00002014*/ MOV         R1, #51                     @ 0x33 
    /*00002018*/ STR         R3, [SP] 
    /*0000201c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002020*/ LDR         R3, [PC, #-536]             @ 0x0000000000001e10 
    /*00002024*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00002028*/ MOV         R0, #3 
    /*0000202c*/ MOVW        FP, #32771                  @ 0x8003 
    /*00002030*/ BL          0x0000000000002030          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002034*/ MOV         R2, #5 
    /*00002038*/ MOV         R3, FP 
    /*0000203c*/ STR         R2, [SP] 
    /*00002040*/ MOVT        R3, #41011                  @ 0xa033 
    /*00002044*/ MOVW        R2, #0                      @ .LC70 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x800] 
    /*00002048*/ MOV         R1, #51                     @ 0x33 
    /*0000204c*/ MOVT        R2, #0                      @ .LC70 [R_ARM_MOVT_ABS: .rodata.str1.4+0x800] 
    /*00002050*/ B           SVP_NNIE_Ioctl_x1d78
    SVP_NNIE_Ioctl_x2054:
    /*00002054*/ MOVW        R3, #1350                   @ 0x546 
    /*00002058*/ MOV         R1, #51                     @ 0x33 
    /*0000205c*/ STR         R3, [SP] 
    /*00002060*/ MOVW        FP, #32774                  @ 0x8006 
    /*00002064*/ LDR         R3, [PC, #-616]             @ 0x0000000000001e04 
    /*00002068*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*0000206c*/ MOV         R0, #3 
    /*00002070*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00002074*/ BL          0x0000000000002074          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002078*/ MOV         R3, FP 
    /*0000207c*/ MOVW        R2, #0                      @ .LC78 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9b8] 
    /*00002080*/ MOVT        R3, #41011                  @ 0xa033 
    /*00002084*/ MOVT        R2, #0                      @ .LC78 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9b8] 
    /*00002088*/ MOV         R1, #51                     @ 0x33 
    /*0000208c*/ MOV         R0, #3 
    /*00002090*/ MOVT        FP, #41011                  @ 0xa033 
    /*00002094*/ BL          0x0000000000002094          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002098*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x209c:
    /*0000209c*/ MOV         FP, R1 
    /*000020a0*/ B           SVP_NNIE_Ioctl_x16ac
    SVP_NNIE_Ioctl_x20a4:
    /*000020a4*/ LDR         R2, [SP, #52]               @ 0x34 
    /*000020a8*/ MOV         R3, #216                    @ 0xd8 
    /*000020ac*/ LDR         R0, [SP, #48]               @ 0x30 
    /*000020b0*/ MOVW        FP, #32834                  @ 0x8042 
    /*000020b4*/ MOVT        FP, #41011                  @ 0xa033 
    /*000020b8*/ MLA         R3, R3, R2, R4 
    /*000020bc*/ MOV         R4, #0 
    /*000020c0*/ STR         R4, [R3, #68]               @ 0x44 
    /*000020c4*/ BL          0x00000000000020C4          @ osal_up [R_ARM_CALL: +0x0] 
    /*000020c8*/ MOV         R0, R4 
    /*000020cc*/ MOVW        R3, #1432                   @ 0x598 
    /*000020d0*/ MOV         R1, #51                     @ 0x33 
    /*000020d4*/ STR         R3, [SP] 
    /*000020d8*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000020dc*/ LDR         R3, [PC, #-736]             @ 0x0000000000001e04 
    /*000020e0*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000020e4*/ BL          0x00000000000020E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000020e8*/ MOV         R0, R4 
    /*000020ec*/ MOVW        R2, #0                      @ .LC82 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa3c] 
    /*000020f0*/ MOV         R1, #51                     @ 0x33 
    /*000020f4*/ MOVT        R2, #0                      @ .LC82 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa3c] 
    /*000020f8*/ BL          0x00000000000020F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000020fc*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x2100:
    /*00002100*/ MOVW        R3, #1469                   @ 0x5bd 
    /*00002104*/ STR         R3, [SP] 
    /*00002108*/ LDR         R3, [PC, #-784]             @ 0x0000000000001e00 
    /*0000210c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002110*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    SVP_NNIE_Ioctl_x2114:
    /*00002114*/ MOV         R1, #51                     @ 0x33 
    /*00002118*/ MOVW        FP, #32774                  @ 0x8006 
    /*0000211c*/ MOV         R0, #3 
    /*00002120*/ BL          0x0000000000002120          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002124*/ MOV         R3, FP 
    /*00002128*/ MOVW        R2, #0                      @ .LC83 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa68] 
    /*0000212c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00002130*/ MOVT        R2, #0                      @ .LC83 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa68] 
    /*00002134*/ MOV         R1, #51                     @ 0x33 
    /*00002138*/ MOV         R0, #3 
    /*0000213c*/ MOVT        FP, #41011                  @ 0xa033 
    /*00002140*/ BL          0x0000000000002140          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002144*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x2148:
    /*00002148*/ LDR         R2, [SP, #52]               @ 0x34 
    /*0000214c*/ MOV         R3, #216                    @ 0xd8 
    /*00002150*/ LDR         R0, [SP, #48]               @ 0x30 
    /*00002154*/ MOVW        FP, #32832                  @ 0x8040 
    /*00002158*/ MOVT        FP, #41011                  @ 0xa033 
    /*0000215c*/ MLA         R3, R3, R2, R4 
    /*00002160*/ MOV         R4, #0 
    /*00002164*/ STR         R4, [R3, #64]               @ 0x40 
    /*00002168*/ BL          0x0000000000002168          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000216c*/ MOV         R0, R4 
    /*00002170*/ MOVW        R3, #1425                   @ 0x591 
    /*00002174*/ MOV         R1, #51                     @ 0x33 
    /*00002178*/ STR         R3, [SP] 
    /*0000217c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002180*/ LDR         R3, [PC, #-900]             @ 0x0000000000001e04 
    /*00002184*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00002188*/ BL          0x0000000000002188          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000218c*/ MOV         R0, R4 
    /*00002190*/ MOVW        R2, #0                      @ .LC81 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa0c] 
    /*00002194*/ MOV         R1, #51                     @ 0x33 
    /*00002198*/ MOVT        R2, #0                      @ .LC81 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa0c] 
    /*0000219c*/ BL          0x000000000000219C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000021a0*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x21a4:
    /*000021a4*/ MOVW        R3, #1181                   @ 0x49d 
    /*000021a8*/ MOV         R1, #51                     @ 0x33 
    /*000021ac*/ STR         R3, [SP] 
    /*000021b0*/ MOVW        FP, #32774                  @ 0x8006 
    /*000021b4*/ LDR         R3, [PC, #-948]             @ 0x0000000000001e08 
    /*000021b8*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000021bc*/ MOV         R0, #3 
    /*000021c0*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000021c4*/ BL          0x00000000000021C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000021c8*/ MOV         R3, FP 
    /*000021cc*/ MOVW        R2, #0                      @ .LC75 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x928] 
    /*000021d0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000021d4*/ MOVT        R2, #0                      @ .LC75 [R_ARM_MOVT_ABS: .rodata.str1.4+0x928] 
    /*000021d8*/ MOV         R1, #51                     @ 0x33 
    /*000021dc*/ MOV         R0, #3 
    /*000021e0*/ MOVT        FP, #41011                  @ 0xa033 
    /*000021e4*/ BL          0x00000000000021E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000021e8*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x21ec:
    /*000021ec*/ MOVW        R3, #1076                   @ 0x434 
    /*000021f0*/ MOV         R1, #51                     @ 0x33 
    /*000021f4*/ STR         R3, [SP] 
    /*000021f8*/ MOVW        FP, #32785                  @ 0x8011 
    /*000021fc*/ LDR         R3, [PC, #-1012]            @ 0x0000000000001e10 
    /*00002200*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002204*/ MOV         R0, #3 
    /*00002208*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*0000220c*/ BL          0x000000000000220C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002210*/ MOV         R3, FP 
    /*00002214*/ MOV         R1, #51                     @ 0x33 
    /*00002218*/ MOVT        R3, #41011                  @ 0xa033 
    /*0000221c*/ MOVW        R2, #0                      @ .LC72 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x870] 
    /*00002220*/ MOV         R0, #3 
    /*00002224*/ MOVT        R2, #0                      @ .LC72 [R_ARM_MOVT_ABS: .rodata.str1.4+0x870] 
    /*00002228*/ MOVT        FP, #41011                  @ 0xa033 
    /*0000222c*/ BL          0x000000000000222C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002230*/ MOV         R0, R5 
    /*00002234*/ BL          0x0000000000002234          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002238*/ B           SVP_NNIE_Ioctl_xf10
    SVP_NNIE_Ioctl_x223c:
    /*0000223c*/ MOVW        R3, #1593                   @ 0x639 
    /*00002240*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002244*/ STR         R3, [SP] 
    /*00002248*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*0000224c*/ LDR         R3, [PC, #-1096]            @ 0x0000000000001e0c 
    /*00002250*/ B           SVP_NNIE_Ioctl_x2114
    SVP_NNIE_Ioctl_x2254:
    /*00002254*/ MOVW        R3, #1017                   @ 0x3f9 
    /*00002258*/ MOV         R1, #51                     @ 0x33 
    /*0000225c*/ STR         R3, [SP] 
    /*00002260*/ MOVW        FP, #32774                  @ 0x8006 
    /*00002264*/ LDR         R3, [PC, #-1116]            @ 0x0000000000001e10 
    /*00002268*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*0000226c*/ MOV         R0, #3 
    /*00002270*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00002274*/ BL          0x0000000000002274          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002278*/ MOV         R3, FP 
    /*0000227c*/ MOVW        R2, #0                      @ .LC66 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x770] 
    /*00002280*/ MOVT        R3, #41011                  @ 0xa033 
    /*00002284*/ MOVT        R2, #0                      @ .LC66 [R_ARM_MOVT_ABS: .rodata.str1.4+0x770] 
    /*00002288*/ MOV         R1, #51                     @ 0x33 
    /*0000228c*/ MOV         R0, #3 
    /*00002290*/ MOVT        FP, #41011                  @ 0xa033 
    /*00002294*/ BL          0x0000000000002294          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002298*/ B           SVP_NNIE_Ioctl_xf10
FUNC_END SVP_NNIE_Ioctl


FUNC_BEGIN SVP_NNIE_Init
    /*0000229c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000022a0*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000022a4*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000022a8*/ SUB         SP, SP, #52                 @ 0x34 
    /*000022ac*/ MOV         R5, #0 
    /*000022b0*/ LDR         R3, [R4] 
    /*000022b4*/ STR         R5, [SP, #24] 
    /*000022b8*/ CMP         R3, #1 
    /*000022bc*/ BEQ         SVP_NNIE_Init_x2510
    /*000022c0*/ CMP         R3, #2 
    /*000022c4*/ BEQ         SVP_NNIE_Init_x2a28
    /*000022c8*/ MOV         R0, #2 
    /*000022cc*/ BL          0x00000000000022CC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000022d0*/ CMP         R0, #0 
    /*000022d4*/ BEQ         SVP_NNIE_Init_x29e0
    /*000022d8*/ MOV         R0, #2 
    /*000022dc*/ BL          0x00000000000022DC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000022e0*/ LDR         R3, [R0, #60]               @ 0x3c 
    /*000022e4*/ CMP         R3, #0 
    /*000022e8*/ BEQ         SVP_NNIE_Init_x29e0
    /*000022ec*/ MOV         R0, #2 
    /*000022f0*/ STR         R5, [SP, #44]               @ 0x2c 
    /*000022f4*/ STR         R5, [SP, #40]               @ 0x28 
    /*000022f8*/ MOV         R6, #51                     @ 0x33 
    /*000022fc*/ STR         R6, [SP, #36]               @ 0x24 
    /*00002300*/ BL          0x0000000000002300          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002304*/ ADD         R1, SP, #24 
    /*00002308*/ LDR         R3, [R0, #60]               @ 0x3c 
    /*0000230c*/ ADD         R0, SP, #36                 @ 0x24 
    /*00002310*/ BLX         R3 
    /*00002314*/ CMP         R0, #0 
    /*00002318*/ BNE         SVP_NNIE_Init_x2998
    /*0000231c*/ ADD         R9, R4, #288                @ 0x120 
    /*00002320*/ MOV         R2, #65536                  @ 0x10000 
    /*00002324*/ LDR         R0, [SP, #24] 
    /*00002328*/ MOVW        R1, #0                      @ .LC101 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf58] 
    /*0000232c*/ STR         R2, [SP] 
    /*00002330*/ MOV         R3, R9 
    /*00002334*/ MOVT        R1, #0                      @ .LC101 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf58] 
    /*00002338*/ ADD         R2, R4, #280                @ 0x118 
    /*0000233c*/ BL          0x000000000000233C          @ CMPI_MmzMallocNocache [R_ARM_CALL: +0x0] 
    /*00002340*/ SUBS        R7, R0, #0 
    /*00002344*/ BNE         SVP_NNIE_Init_x24d0
    /*00002348*/ ADD         R8, R4, #272                @ 0x110 
    /*0000234c*/ LDR         R0, [SP, #24] 
    /*00002350*/ MOVW        SL, #13992                  @ 0x36a8 
    /*00002354*/ MOVW        R1, #0                      @ .LC103 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf8c] 
    /*00002358*/ MOV         R3, R8 
    /*0000235c*/ STR         SL, [SP] 
    /*00002360*/ MOVT        R1, #0                      @ .LC103 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf8c] 
    /*00002364*/ ADD         R2, R4, #264                @ 0x108 
    /*00002368*/ BL          0x0000000000002368          @ CMPI_MmzMallocCached [R_ARM_CALL: +0x0] 
    /*0000236c*/ SUBS        R5, R0, #0 
    /*00002370*/ BNE         SVP_NNIE_Init_x251c
    /*00002374*/ MOV         R1, R5 
    /*00002378*/ MOV         R2, #16 
    /*0000237c*/ STR         SL, [R4, #276]              @ 0x114 
    /*00002380*/ ADD         SL, R4, #220                @ 0xdc 
    /*00002384*/ LDR         R0, [PC, #1764]             @ 0x0000000000002a70 
    /*00002388*/ MVN         R3, #0 
    /*0000238c*/ STR         R5, [R4, #8] 
    /*00002390*/ ADD         FP, R4, #216                @ 0xd8 
    /*00002394*/ STR         R3, [R4, #12] 
    /*00002398*/ STR         R5, [R4, #16] 
    /*0000239c*/ BL          0x000000000000239C          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000023a0*/ MOV         R1, R5 
    /*000023a4*/ MOV         R2, #8 
    /*000023a8*/ SUB         R0, SL, #12 
    /*000023ac*/ STR         R5, [R4, #36]               @ 0x24 
    /*000023b0*/ STR         R5, [R4, #40]               @ 0x28 
    /*000023b4*/ STR         R5, [R4, #44]               @ 0x2c 
    /*000023b8*/ STR         R5, [R4, #48]               @ 0x30 
    /*000023bc*/ STR         R5, [R4, #60]               @ 0x3c 
    /*000023c0*/ STR         R5, [R4, #64]               @ 0x40 
    /*000023c4*/ STR         R5, [R4, #68]               @ 0x44 
    /*000023c8*/ STR         R5, [R4, #72]               @ 0x48 
    /*000023cc*/ STR         R5, [R4, #52]               @ 0x34 
    /*000023d0*/ STR         R5, [R4, #56]               @ 0x38 
    /*000023d4*/ STR         R5, [R4, #196]              @ 0xc4 
    /*000023d8*/ STR         R5, [R4, #192]              @ 0xc0 
    /*000023dc*/ BL          0x00000000000023DC          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000023e0*/ MOV         R1, R5 
    /*000023e4*/ MOV         R2, #8 
    /*000023e8*/ SUB         R0, SL, #20 
    /*000023ec*/ BL          0x00000000000023EC          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000023f0*/ MOV         R1, R5 
    /*000023f4*/ MOV         R2, #96                     @ 0x60 
    /*000023f8*/ SUB         R0, SL, #140                @ 0x8c 
    /*000023fc*/ BL          0x00000000000023FC          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00002400*/ MOV         R1, R5 
    /*00002404*/ MOV         R2, #4 
    /*00002408*/ MOV         R0, SL 
    /*0000240c*/ BL          0x000000000000240C          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00002410*/ MOV         R1, R5 
    /*00002414*/ MOV         R2, #4 
    /*00002418*/ MOV         R0, FP 
    /*0000241c*/ BL          0x000000000000241C          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00002420*/ LDR         R3, [R4, #288]              @ 0x120 
    /*00002424*/ MOV         R0, R5 
    /*00002428*/ ADD         R2, R3, #32768              @ 0x8000 
    /*0000242c*/ STR         R2, [R4, #256]              @ 0x100 
    /*00002430*/ STR         R3, [R4, #252]              @ 0xfc 
    /*00002434*/ LDRD        R2, [R9, #-8] 
    /*00002438*/ ADDS        R2, R2, #32768              @ 0x8000 
    /*0000243c*/ ADC         R3, R3, #0 
    /*00002440*/ STRD        R2, [SP, #8] 
    /*00002444*/ LDRD        R2, [R9, #-8] 
    /*00002448*/ STRD        R2, [R4, #232]              @ 0xe8 
    /*0000244c*/ LDRD        R2, [SP, #8] 
    /*00002450*/ STRD        R2, [R4, #240]              @ 0xf0 
    /*00002454*/ BL          0x0000000000002454          @ SVP_NNIE_DRV_Init [R_ARM_CALL: .text+0xb4ec] 
    /*00002458*/ SUBS        R5, R0, #0 
    /*0000245c*/ BNE         SVP_NNIE_Init_x2588
    /*00002460*/ MOV         R0, SL 
    /*00002464*/ BL          0x0000000000002464          @ osal_wait_init [R_ARM_CALL: +0x0] 
    /*00002468*/ CMP         R0, #0 
    /*0000246c*/ BNE         SVP_NNIE_Init_x264c
    /*00002470*/ MOV         R1, #1 
    /*00002474*/ MOV         R0, FP 
    /*00002478*/ BL          0x0000000000002478          @ osal_sema_init [R_ARM_CALL: +0x0] 
    /*0000247c*/ CMP         R0, #0 
    /*00002480*/ BEQ         SVP_NNIE_Init_x2698
    /*00002484*/ MOVW        R3, #2212                   @ 0x8a4 
    /*00002488*/ MOV         R1, #51                     @ 0x33 
    /*0000248c*/ STR         R3, [SP] 
    /*00002490*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002494*/ LDR         R3, [PC, #1496]             @ 0x0000000000002a74 
    /*00002498*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*0000249c*/ MOV         R0, #0 
    /*000024a0*/ MOVW        R5, #32780                  @ 0x800c 
    /*000024a4*/ BL          0x00000000000024A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000024a8*/ MOV         R3, #0 
    /*000024ac*/ MOV         R0, R3 
    /*000024b0*/ MOVW        R2, #0                      @ .LC107 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x103c] 
    /*000024b4*/ MOV         R1, #51                     @ 0x33 
    /*000024b8*/ MOVT        R2, #0                      @ .LC107 [R_ARM_MOVT_ABS: .rodata.str1.4+0x103c] 
    /*000024bc*/ BL          0x00000000000024BC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000024c0*/ MOVT        R5, #41011                  @ 0xa033 
    SVP_NNIE_Init_x24c4:
    /*000024c4*/ MOV         R0, SL 
    /*000024c8*/ BL          0x00000000000024C8          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*000024cc*/ B           SVP_NNIE_Init_x268c
    SVP_NNIE_Init_x24d0:
    /*000024d0*/ MOVW        R3, #2420                   @ 0x974 
    /*000024d4*/ MOV         R1, R6 
    /*000024d8*/ STR         R3, [SP] 
    /*000024dc*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000024e0*/ LDR         R3, [PC, #1424]             @ 0x0000000000002a78 
    /*000024e4*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000024e8*/ MOV         R0, #3 
    /*000024ec*/ MOVW        R5, #32780                  @ 0x800c 
    /*000024f0*/ BL          0x00000000000024F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000024f4*/ MOV         R3, R7 
    /*000024f8*/ MOV         R1, R6 
    /*000024fc*/ MOVW        R2, #0                      @ .LC102 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf68] 
    /*00002500*/ MOV         R0, #3 
    /*00002504*/ MOVT        R2, #0                      @ .LC102 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf68] 
    /*00002508*/ BL          0x0000000000002508          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000250c*/ MOVT        R5, #41011                  @ 0xa033 
    SVP_NNIE_Init_x2510:
    /*00002510*/ MOV         R0, R5 
    /*00002514*/ ADD         SP, SP, #52                 @ 0x34 
    /*00002518*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_Init_x251c:
    /*0000251c*/ MOVW        R3, #2427                   @ 0x97b 
    /*00002520*/ MOV         R1, R6 
    /*00002524*/ STR         R3, [SP] 
    /*00002528*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*0000252c*/ LDR         R3, [PC, #1348]             @ 0x0000000000002a78 
    /*00002530*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00002534*/ MOV         R0, #3 
    /*00002538*/ BL          0x0000000000002538          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000253c*/ MOVW        R2, #0                      @ .LC104 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfac] 
    /*00002540*/ MOV         R3, R5 
    /*00002544*/ MOV         R1, R6 
    /*00002548*/ MOV         R0, #3 
    /*0000254c*/ MOVT        R2, #0                      @ .LC104 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfac] 
    /*00002550*/ BL          0x0000000000002550          @ HI_LOG [R_ARM_CALL: +0x0] 
    SVP_NNIE_Init_x2554:
    /*00002554*/ LDRD        R0, [R9, #-8] 
    /*00002558*/ ORRS        R3, R0, R1 
    /*0000255c*/ BEQ         SVP_NNIE_Init_x2510
    /*00002560*/ LDR         R2, [R4, #288]              @ 0x120 
    /*00002564*/ BL          0x0000000000002564          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*00002568*/ MOV         R0, #0 
    /*0000256c*/ MOV         R1, #0 
    /*00002570*/ MOV         R3, #0 
    /*00002574*/ STRD        R0, [R9, #-8] 
    /*00002578*/ MOV         R0, R5 
    /*0000257c*/ STR         R3, [R4, #288]              @ 0x120 
    /*00002580*/ ADD         SP, SP, #52                 @ 0x34 
    /*00002584*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_Init_x2588:
    /*00002588*/ MOVW        R3, #2193                   @ 0x891 
    /*0000258c*/ MOV         R1, R6 
    /*00002590*/ STR         R3, [SP] 
    /*00002594*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002598*/ LDR         R3, [PC, #1236]             @ 0x0000000000002a74 
    /*0000259c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000025a0*/ MOV         R0, #3 
    /*000025a4*/ BL          0x00000000000025A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000025a8*/ MOVW        R2, #0                      @ .LC105 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfd8] 
    /*000025ac*/ STR         R5, [SP] 
    /*000025b0*/ MOV         R3, R7 
    /*000025b4*/ MOV         R1, R6 
    /*000025b8*/ MOV         R0, #3 
    /*000025bc*/ MOVT        R2, #0                      @ .LC105 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfd8] 
    /*000025c0*/ BL          0x00000000000025C0          @ HI_LOG [R_ARM_CALL: +0x0] 
    SVP_NNIE_Init_x25c4:
    /*000025c4*/ MOVW        R3, #2443                   @ 0x98b 
    /*000025c8*/ MOV         R1, #51                     @ 0x33 
    /*000025cc*/ STR         R3, [SP] 
    /*000025d0*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000025d4*/ LDR         R3, [PC, #1180]             @ 0x0000000000002a78 
    /*000025d8*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000025dc*/ MOV         R0, #3 
    /*000025e0*/ BL          0x00000000000025E0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000025e4*/ MOV         R3, #0 
    /*000025e8*/ MOVW        R2, #0                      @ .LC111 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10e0] 
    /*000025ec*/ STR         R3, [SP] 
    /*000025f0*/ MOVT        R2, #0                      @ .LC111 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10e0] 
    /*000025f4*/ MOV         R3, R5 
    /*000025f8*/ MOV         R1, #51                     @ 0x33 
    /*000025fc*/ MOV         R0, #3 
    /*00002600*/ BL          0x0000000000002600          @ HI_LOG [R_ARM_CALL: +0x0] 
    SVP_NNIE_Init_x2604:
    /*00002604*/ LDRD        R2, [R8, #-8] 
    /*00002608*/ ORRS        R1, R2, R3 
    /*0000260c*/ BEQ         SVP_NNIE_Init_x2554
    /*00002610*/ MOV         R1, R2 
    /*00002614*/ LDR         R0, [R4, #272]              @ 0x110 
    /*00002618*/ LDR         R2, [R4, #276]              @ 0x114 
    /*0000261c*/ BL          0x000000000000261C          @ osal_flush_dcache_area [R_ARM_CALL: +0x0] 
    /*00002620*/ LDRD        R0, [R8, #-8] 
    /*00002624*/ ORRS        R3, R0, R1 
    /*00002628*/ BEQ         SVP_NNIE_Init_x2554
    /*0000262c*/ LDR         R2, [R4, #272]              @ 0x110 
    /*00002630*/ BL          0x0000000000002630          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*00002634*/ MOV         R0, #0 
    /*00002638*/ MOV         R1, #0 
    /*0000263c*/ MOV         R3, #0 
    /*00002640*/ STRD        R0, [R8, #-8] 
    /*00002644*/ STR         R3, [R4, #272]              @ 0x110 
    /*00002648*/ B           SVP_NNIE_Init_x2554
    SVP_NNIE_Init_x264c:
    /*0000264c*/ MOVW        R3, #2201                   @ 0x899 
    /*00002650*/ MOV         R1, R6 
    /*00002654*/ STR         R3, [SP] 
    /*00002658*/ MOV         R0, R5 
    /*0000265c*/ LDR         R3, [PC, #1040]             @ 0x0000000000002a74 
    /*00002660*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002664*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00002668*/ BL          0x0000000000002668          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000266c*/ MOV         R3, R5 
    /*00002670*/ MOV         R0, R5 
    /*00002674*/ MOV         R1, R6 
    /*00002678*/ MOVW        R2, #0                      @ .LC106 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x100c] 
    /*0000267c*/ MOVT        R2, #0                      @ .LC106 [R_ARM_MOVT_ABS: .rodata.str1.4+0x100c] 
    /*00002680*/ BL          0x0000000000002680          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002684*/ MOVW        R5, #32780                  @ 0x800c 
    /*00002688*/ MOVT        R5, #41011                  @ 0xa033 
    SVP_NNIE_Init_x268c:
    /*0000268c*/ MOV         R0, #0 
    /*00002690*/ BL          0x0000000000002690          @ SVP_NNIE_DRV_DeInit [R_ARM_CALL: .text+0xb548] 
    /*00002694*/ B           SVP_NNIE_Init_x25c4
    SVP_NNIE_Init_x2698:
    /*00002698*/ LDR         R0, [PC, #988]              @ 0x0000000000002a7c 
    /*0000269c*/ BL          0x000000000000269C          @ osal_spin_lock_init [R_ARM_CALL: +0x0] 
    /*000026a0*/ SUBS        R5, R0, #0 
    /*000026a4*/ BNE         SVP_NNIE_Init_x2744
    /*000026a8*/ MOVW        R2, #0                      @ g_au32NnieIrg [R_ARM_MOVW_ABS_NC: .data+0xbc] 
    /*000026ac*/ MOVT        R2, #0                      @ g_au32NnieIrg [R_ARM_MOVT_ABS: .data+0xbc] 
    /*000026b0*/ MOVW        R3, #0                      @ g_apszSvpNnieIrqName [R_ARM_MOVW_ABS_NC: .data+0xc0] 
    /*000026b4*/ MOVT        R3, #0                      @ g_apszSvpNnieIrqName [R_ARM_MOVT_ABS: .data+0xc0] 
    /*000026b8*/ MOV         R0, R2 
    /*000026bc*/ STR         R2, [SP, #20] 
    /*000026c0*/ LDR         R3, [R3] 
    /*000026c4*/ MOVW        R2, #0                      @ g_au32SvpNnieCoreId [R_ARM_MOVW_ABS_NC: .bss+0x12c] 
    /*000026c8*/ LDR         R0, [R0] 
    /*000026cc*/ MOVT        R2, #0                      @ g_au32SvpNnieCoreId [R_ARM_MOVT_ABS: .bss+0x12c] 
    /*000026d0*/ MOVW        R1, #0                      @ SVP_NNIE_IntRoute [R_ARM_MOVW_ABS_NC: .text+0x2a94] 
    /*000026d4*/ STR         R2, [SP] 
    /*000026d8*/ MOVT        R1, #0                      @ SVP_NNIE_IntRoute [R_ARM_MOVT_ABS: .text+0x2a94] 
    /*000026dc*/ MOV         R2, R5 
    /*000026e0*/ BL          0x00000000000026E0          @ osal_request_irq [R_ARM_CALL: +0x0] 
    /*000026e4*/ SUBS        R3, R0, #0 
    /*000026e8*/ STR         R3, [SP, #8] 
    /*000026ec*/ BEQ         SVP_NNIE_Init_x2788
    /*000026f0*/ MOVW        R3, #2232                   @ 0x8b8 
    /*000026f4*/ MOV         R1, #51                     @ 0x33 
    /*000026f8*/ STR         R3, [SP] 
    /*000026fc*/ MOV         R0, R5 
    /*00002700*/ LDR         R3, [PC, #876]              @ 0x0000000000002a74 
    /*00002704*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002708*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*0000270c*/ BL          0x000000000000270C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002710*/ MOV         R3, R5 
    /*00002714*/ MOV         R0, R5 
    /*00002718*/ MOV         R1, #51                     @ 0x33 
    /*0000271c*/ MOVW        R2, #0                      @ .LC109 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1098] 
    /*00002720*/ MOVT        R2, #0                      @ .LC109 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1098] 
    /*00002724*/ BL          0x0000000000002724          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002728*/ LDR         R0, [PC, #844]              @ 0x0000000000002a7c 
    /*0000272c*/ BL          0x000000000000272C          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00002730*/ MOVW        R5, #32773                  @ 0x8005 
    /*00002734*/ MOVT        R5, #41011                  @ 0xa033 
    SVP_NNIE_Init_x2738:
    /*00002738*/ MOV         R0, FP 
    /*0000273c*/ BL          0x000000000000273C          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*00002740*/ B           SVP_NNIE_Init_x24c4
    SVP_NNIE_Init_x2744:
    /*00002744*/ MOVW        R3, #2222                   @ 0x8ae 
    /*00002748*/ MOV         R1, #51                     @ 0x33 
    /*0000274c*/ STR         R3, [SP] 
    /*00002750*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002754*/ LDR         R3, [PC, #792]              @ 0x0000000000002a74 
    /*00002758*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*0000275c*/ MOV         R0, #0 
    /*00002760*/ MOVW        R5, #32780                  @ 0x800c 
    /*00002764*/ BL          0x0000000000002764          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002768*/ MOV         R3, #0 
    /*0000276c*/ MOV         R0, R3 
    /*00002770*/ MOVW        R2, #0                      @ .LC108 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1068] 
    /*00002774*/ MOV         R1, #51                     @ 0x33 
    /*00002778*/ MOVT        R2, #0                      @ .LC108 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1068] 
    /*0000277c*/ BL          0x000000000000277C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002780*/ MOVT        R5, #41011                  @ 0xa033 
    /*00002784*/ B           SVP_NNIE_Init_x2738
    SVP_NNIE_Init_x2788:
    /*00002788*/ LDRD        R2, [R8, #-8] 
    /*0000278c*/ LDR         R1, [R4, #272]              @ 0x110 
    /*00002790*/ STRD        R2, [R4, #176]              @ 0xb0 
    /*00002794*/ STR         R1, [R4, #184]              @ 0xb8 
    /*00002798*/ BL          0x0000000000002798          @ SVP_NNIE_DRV_SetClkFreq [R_ARM_CALL: .text+0xb624] 
    /*0000279c*/ ADD         R0, SP, #28 
    /*000027a0*/ BL          0x00000000000027A0          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000027a4*/ LDR         R6, [SP, #28] 
    /*000027a8*/ MOVW        R3, #0                      @ g_apszNnieGdcIrqName [R_ARM_MOVW_ABS_NC: .data+0xb8] 
    /*000027ac*/ MOVW        R2, #0                      @ g_au32GdcIrq [R_ARM_MOVW_ABS_NC: .data+0xb4] 
    /*000027b0*/ MOVT        R3, #0                      @ g_apszNnieGdcIrqName [R_ARM_MOVT_ABS: .data+0xb8] 
    /*000027b4*/ MOVT        R2, #0                      @ g_au32GdcIrq [R_ARM_MOVT_ABS: .data+0xb4] 
    /*000027b8*/ MOVW        IP, #0                      @ SVP_NNIE_GdcIntRoute [R_ARM_MOVW_ABS_NC: .text+0xc40] 
    /*000027bc*/ MOVT        IP, #0                      @ SVP_NNIE_GdcIntRoute [R_ARM_MOVT_ABS: .text+0xc40] 
    /*000027c0*/ LDR         R3, [R3] 
    /*000027c4*/ ASR         R7, R6, #31 
    /*000027c8*/ LDR         R0, [R2] 
    /*000027cc*/ MOV         R1, IP 
    /*000027d0*/ MOV         R2, R5 
    /*000027d4*/ STR         IP, [SP] 
    /*000027d8*/ STRD        R6, [R4, #120]              @ 0x78 
    /*000027dc*/ BL          0x00000000000027DC          @ osal_request_irq [R_ARM_CALL: +0x0] 
    /*000027e0*/ CMP         R0, #0 
    /*000027e4*/ MOVEQ       R3, #1 
    /*000027e8*/ STREQ       R3, [R4] 
    /*000027ec*/ BEQ         SVP_NNIE_Init_x2510
    /*000027f0*/ LDR         R5, [SP, #8] 
    /*000027f4*/ MOV         R3, #592                    @ 0x250 
    /*000027f8*/ MOV         R1, #51                     @ 0x33 
    /*000027fc*/ STR         R3, [SP] 
    /*00002800*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002804*/ LDR         R3, [PC, #628]              @ 0x0000000000002a80 
    /*00002808*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*0000280c*/ MOV         R0, R5 
    /*00002810*/ BL          0x0000000000002810          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002814*/ MOV         R3, R5 
    /*00002818*/ MOV         R1, #51                     @ 0x33 
    /*0000281c*/ MOVW        R2, #0                      @ .LC110 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10bc] 
    /*00002820*/ MOV         R0, R5 
    /*00002824*/ MOVT        R2, #0                      @ .LC110 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10bc] 
    /*00002828*/ BL          0x0000000000002828          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000282c*/ MOVW        R3, #2451                   @ 0x993 
    /*00002830*/ MOV         R1, #51                     @ 0x33 
    /*00002834*/ STR         R3, [SP] 
    /*00002838*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*0000283c*/ LDR         R3, [PC, #564]              @ 0x0000000000002a78 
    /*00002840*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00002844*/ MOV         R0, #3 
    /*00002848*/ BL          0x0000000000002848          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000284c*/ MOV         R1, #51                     @ 0x33 
    /*00002850*/ STR         R5, [SP] 
    /*00002854*/ MOVW        R3, #32773                  @ 0x8005 
    /*00002858*/ MOVW        R2, #0                      @ .LC111 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10e0] 
    /*0000285c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00002860*/ MOVT        R2, #0                      @ .LC111 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10e0] 
    /*00002864*/ MOV         R0, #3 
    /*00002868*/ BL          0x0000000000002868          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000286c*/ MOV         R1, #1 
    /*00002870*/ MOV         R0, R5 
    /*00002874*/ BL          0x0000000000002874          @ SVP_NNIE_DRV_SetReset [R_ARM_CALL: .text+0xb5b8] 
    /*00002878*/ LDR         R3, [R4, #72]               @ 0x48 
    /*0000287c*/ CMP         R3, #1 
    /*00002880*/ BEQ         SVP_NNIE_Init_x298c
    SVP_NNIE_Init_x2884:
    /*00002884*/ LDR         R3, [SP, #20] 
    /*00002888*/ MOVW        R1, #0                      @ g_au32SvpNnieCoreId [R_ARM_MOVW_ABS_NC: .bss+0x12c] 
    /*0000288c*/ MOVT        R1, #0                      @ g_au32SvpNnieCoreId [R_ARM_MOVT_ABS: .bss+0x12c] 
    /*00002890*/ LDR         R0, [R3] 
    /*00002894*/ BL          0x0000000000002894          @ osal_free_irq [R_ARM_CALL: +0x0] 
    /*00002898*/ LDR         R0, [PC, #476]              @ 0x0000000000002a7c 
    /*0000289c*/ BL          0x000000000000289C          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*000028a0*/ MOV         R0, FP 
    /*000028a4*/ BL          0x00000000000028A4          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*000028a8*/ MOV         R0, SL 
    /*000028ac*/ BL          0x00000000000028AC          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*000028b0*/ MOV         R0, #0 
    /*000028b4*/ BL          0x00000000000028B4          @ SVP_NNIE_DRV_DeInit [R_ARM_CALL: .text+0xb548] 
    /*000028b8*/ LDR         R0, [R4, #192]              @ 0xc0 
    /*000028bc*/ CMP         R0, #0 
    /*000028c0*/ BEQ         SVP_NNIE_Init_x28d0
    /*000028c4*/ BL          0x00000000000028C4          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*000028c8*/ MOV         R3, #0 
    /*000028cc*/ STR         R3, [R4, #192]              @ 0xc0 
    SVP_NNIE_Init_x28d0:
    /*000028d0*/ MOV         R1, #0 
    /*000028d4*/ LDR         R0, [PC, #424]              @ 0x0000000000002a84 
    /*000028d8*/ MOV         R6, R1 
    /*000028dc*/ BL          0x00000000000028DC          @ SVP_NNIE_TSK_ADDR_DeInitList [R_ARM_CALL: .text+0x81f0] 
    /*000028e0*/ MOV         R1, R6 
    /*000028e4*/ MOV         R2, #16 
    /*000028e8*/ LDR         R0, [PC, #384]              @ 0x0000000000002a70 
    /*000028ec*/ MVN         R3, #0 
    /*000028f0*/ STR         R6, [R4, #8] 
    /*000028f4*/ MOVW        R5, #32773                  @ 0x8005 
    /*000028f8*/ STR         R3, [R4, #12] 
    /*000028fc*/ MOVT        R5, #41011                  @ 0xa033 
    /*00002900*/ STR         R6, [R4, #16] 
    /*00002904*/ BL          0x0000000000002904          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00002908*/ MOV         R1, R6 
    /*0000290c*/ MOV         R2, #8 
    /*00002910*/ LDR         R0, [PC, #368]              @ 0x0000000000002a88 
    /*00002914*/ STR         R6, [R4, #36]               @ 0x24 
    /*00002918*/ STR         R6, [R4, #40]               @ 0x28 
    /*0000291c*/ STR         R6, [R4, #44]               @ 0x2c 
    /*00002920*/ STR         R6, [R4, #48]               @ 0x30 
    /*00002924*/ STR         R6, [R4, #60]               @ 0x3c 
    /*00002928*/ STR         R6, [R4, #64]               @ 0x40 
    /*0000292c*/ STR         R6, [R4, #68]               @ 0x44 
    /*00002930*/ STR         R6, [R4, #72]               @ 0x48 
    /*00002934*/ STR         R6, [R4, #52]               @ 0x34 
    /*00002938*/ STR         R6, [R4, #56]               @ 0x38 
    /*0000293c*/ STR         R6, [R4, #196]              @ 0xc4 
    /*00002940*/ STR         R6, [R4, #192]              @ 0xc0 
    /*00002944*/ BL          0x0000000000002944          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00002948*/ MOV         R1, R6 
    /*0000294c*/ MOV         R2, #8 
    /*00002950*/ LDR         R0, [PC, #308]              @ 0x0000000000002a8c 
    /*00002954*/ BL          0x0000000000002954          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00002958*/ MOV         R1, R6 
    /*0000295c*/ MOV         R2, #96                     @ 0x60 
    /*00002960*/ LDR         R0, [PC, #296]              @ 0x0000000000002a90 
    /*00002964*/ BL          0x0000000000002964          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00002968*/ MOV         R1, R6 
    /*0000296c*/ MOV         R0, SL 
    /*00002970*/ MOV         R2, #4 
    /*00002974*/ BL          0x0000000000002974          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00002978*/ MOV         R1, R6 
    /*0000297c*/ MOV         R0, FP 
    /*00002980*/ MOV         R2, #4 
    /*00002984*/ BL          0x0000000000002984          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00002988*/ B           SVP_NNIE_Init_x2604
    SVP_NNIE_Init_x298c:
    /*0000298c*/ LDR         R3, [SP, #8] 
    /*00002990*/ STR         R3, [R4, #72]               @ 0x48 
    /*00002994*/ B           SVP_NNIE_Init_x2884
    SVP_NNIE_Init_x2998:
    /*00002998*/ MOV         R1, R6 
    /*0000299c*/ MOVW        R3, #2406                   @ 0x966 
    /*000029a0*/ MOVW        R5, #32780                  @ 0x800c 
    /*000029a4*/ STR         R3, [SP] 
    /*000029a8*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000029ac*/ LDR         R3, [PC, #196]              @ 0x0000000000002a78 
    /*000029b0*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000029b4*/ MOV         R0, #3 
    /*000029b8*/ BL          0x00000000000029B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000029bc*/ MOV         R3, R5 
    /*000029c0*/ MOV         R1, R6 
    /*000029c4*/ MOVW        R2, #0                      @ .LC100 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf30] 
    /*000029c8*/ MOVT        R3, #41011                  @ 0xa033 
    /*000029cc*/ MOVT        R2, #0                      @ .LC100 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf30] 
    /*000029d0*/ MOV         R0, #3 
    /*000029d4*/ MOVT        R5, #41011                  @ 0xa033 
    /*000029d8*/ BL          0x00000000000029D8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000029dc*/ B           SVP_NNIE_Init_x2510
    SVP_NNIE_Init_x29e0:
    /*000029e0*/ MOVW        R3, #2398                   @ 0x95e 
    /*000029e4*/ MOV         R1, #51                     @ 0x33 
    /*000029e8*/ STR         R3, [SP] 
    /*000029ec*/ MOVW        R5, #32784                  @ 0x8010 
    /*000029f0*/ LDR         R3, [PC, #128]              @ 0x0000000000002a78 
    /*000029f4*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000029f8*/ MOV         R0, #3 
    /*000029fc*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00002a00*/ BL          0x0000000000002A00          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002a04*/ MOV         R3, R5 
    /*00002a08*/ MOVW        R2, #0                      @ .LC99 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf10] 
    /*00002a0c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00002a10*/ MOVT        R2, #0                      @ .LC99 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf10] 
    /*00002a14*/ MOV         R1, #51                     @ 0x33 
    /*00002a18*/ MOV         R0, #3 
    /*00002a1c*/ MOVT        R5, #41011                  @ 0xa033 
    /*00002a20*/ BL          0x0000000000002A20          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002a24*/ B           SVP_NNIE_Init_x2510
    SVP_NNIE_Init_x2a28:
    /*00002a28*/ MOVW        R3, #2393                   @ 0x959 
    /*00002a2c*/ MOV         R1, #51                     @ 0x33 
    /*00002a30*/ STR         R3, [SP] 
    /*00002a34*/ MOVW        R5, #32786                  @ 0x8012 
    /*00002a38*/ LDR         R3, [PC, #56]               @ 0x0000000000002a78 
    /*00002a3c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002a40*/ MOV         R0, #3 
    /*00002a44*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00002a48*/ BL          0x0000000000002A48          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002a4c*/ MOV         R3, R5 
    /*00002a50*/ MOVW        R2, #0                      @ .LC98 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xeec] 
    /*00002a54*/ MOVT        R3, #41011                  @ 0xa033 
    /*00002a58*/ MOVT        R2, #0                      @ .LC98 [R_ARM_MOVT_ABS: .rodata.str1.4+0xeec] 
    /*00002a5c*/ MOV         R1, #51                     @ 0x33 
    /*00002a60*/ MOV         R0, #3 
    /*00002a64*/ MOVT        R5, #41011                  @ 0xa033 
    /*00002a68*/ BL          0x0000000000002A68          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002a6c*/ B           SVP_NNIE_Init_x2510
    /*00002a70*/ ANDEQ       R0, R0, R4, LSL R0 
    /*00002a74*/ ANDEQ       R0, R0, R0, LSL #2 
    /*00002a78*/ STRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
    /*00002a7c*/ STRDEQ      R0, [R0], -R8 
    /*00002a80*/ ANDEQ       R0, R0, IP, LSL #2 
    /*00002a84*/ ANDEQ       R0, R0, R8 
    /*00002a88*/ LDRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
    /*00002a8c*/ ANDEQ       R0, R0, R8, ASR #1 
    /*00002a90*/ ANDEQ       R0, R0, R0, ASR R0 
FUNC_END SVP_NNIE_Init


FUNC_BEGIN SVP_NNIE_IntRoute
    /*00002a94*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00002a98*/ SUB         SP, SP, #36                 @ 0x24 
    /*00002a9c*/ ADD         R0, SP, #24 
    /*00002aa0*/ LDR         R4, [R1] 
    /*00002aa4*/ BL          0x0000000000002AA4          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00002aa8*/ LDR         R6, [SP, #28] 
    /*00002aac*/ LDR         R2, [SP, #24] 
    /*00002ab0*/ MOVW        R3, #16960                  @ 0x4240 
    /*00002ab4*/ MOVT        R3, #15 
    /*00002ab8*/ MOV         R0, R4 
    /*00002abc*/ ASR         R7, R6, #31 
    /*00002ac0*/ SMLAL       R6, R7, R3, R2 
    /*00002ac4*/ BL          0x0000000000002AC4          @ SVP_NNIE_DRV_GetIntStatus [R_ARM_CALL: .text+0xb64c] 
    /*00002ac8*/ MOV         SL, R0 
    /*00002acc*/ BL          0x0000000000002ACC          @ SVP_NNIE_DRV_IsCfgErrInt [R_ARM_CALL: .text+0xb6a8] 
    /*00002ad0*/ SUBS        R9, R0, #0 
    /*00002ad4*/ BNE         SVP_NNIE_IntRoute_x2e5c
    SVP_NNIE_IntRoute_x2ad8:
    /*00002ad8*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002adc*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002ae0*/ ADD         R8, R5, #248                @ 0xf8 
    /*00002ae4*/ MOV         R0, R4 
    /*00002ae8*/ ADD         R8, R8, R4, LSL #2 
    /*00002aec*/ BL          0x0000000000002AEC          @ SVP_NNIE_DRV_ClearInt [R_ARM_CALL: .text+0xb650] 
    /*00002af0*/ ADD         R1, SP, #16 
    /*00002af4*/ MOV         R0, R8 
    /*00002af8*/ BL          0x0000000000002AF8          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00002afc*/ MOV         R3, #216                    @ 0xd8 
    /*00002b00*/ MLA         FP, R3, R4, R5 
    /*00002b04*/ MOV         R2, #0 
    /*00002b08*/ MOV         R0, SL 
    /*00002b0c*/ STR         R2, [FP, #16] 
    /*00002b10*/ BL          0x0000000000002B10          @ SVP_NNIE_DRV_IsFinishInt [R_ARM_CALL: .text+0xb6fc] 
    /*00002b14*/ SUBS        R3, R0, #0 
    /*00002b18*/ STR         R3, [SP, #12] 
    /*00002b1c*/ BNE         SVP_NNIE_IntRoute_x2ce4
    /*00002b20*/ MOV         R0, SL 
    /*00002b24*/ BL          0x0000000000002B24          @ SVP_NNIE_DRV_IsTimeoutInt [R_ARM_CALL: .text+0xb654] 
    /*00002b28*/ SUBS        R3, R0, #0 
    /*00002b2c*/ MOV         FP, R3 
    /*00002b30*/ BNE         SVP_NNIE_IntRoute_x2c2c
    /*00002b34*/ MOV         R0, SL 
    /*00002b38*/ BL          0x0000000000002B38          @ SVP_NNIE_DRV_IsCfgErrInt [R_ARM_CALL: .text+0xb6a8] 
    /*00002b3c*/ CMP         R0, #0 
    /*00002b40*/ BNE         SVP_NNIE_IntRoute_x2ed8
    SVP_NNIE_IntRoute_x2b44:
    /*00002b44*/ ADD         R1, SP, #16 
    /*00002b48*/ MOV         R0, R8 
    /*00002b4c*/ BL          0x0000000000002B4C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002b50*/ MOV         IP, #216                    @ 0xd8 
    /*00002b54*/ MUL         R8, IP, R4 
    /*00002b58*/ LDR         R0, [PC, #872]              @ 0x0000000000002ec8 
    /*00002b5c*/ ADD         R0, R8, R0 
    /*00002b60*/ BL          0x0000000000002B60          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00002b64*/ ADD         R0, SP, #24 
    /*00002b68*/ BL          0x0000000000002B68          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00002b6c*/ LDR         R3, [SP, #28] 
    /*00002b70*/ LDR         R2, [SP, #24] 
    /*00002b74*/ MOVW        LR, #16960                  @ 0x4240 
    /*00002b78*/ MOVT        LR, #15 
    /*00002b7c*/ ADD         IP, R5, R8 
    /*00002b80*/ ADD         R1, PC, #824                @ 0x338 
    /*00002b84*/ LDRD        R0, [R1] 
    /*00002b88*/ MLA         LR, R2, LR, R3 
    /*00002b8c*/ LDR         R8, [IP, #108]              @ 0x6c 
    /*00002b90*/ LDRD        R2, [IP, #136]              @ 0x88 
    /*00002b94*/ SUB         LR, LR, R6 
    /*00002b98*/ STR         LR, [IP, #104]              @ 0x68 
    /*00002b9c*/ CMP         R8, LR 
    /*00002ba0*/ MOVCC       R8, LR 
    /*00002ba4*/ SUBS        R2, R6, R2 
    /*00002ba8*/ SBC         R3, R7, R3 
    /*00002bac*/ STR         R8, [IP, #108]              @ 0x6c 
    /*00002bb0*/ CMP         R3, R1 
    /*00002bb4*/ LDR         R3, [IP, #80]               @ 0x50 
    /*00002bb8*/ CMPEQ       R2, R0 
    /*00002bbc*/ LDR         R2, [IP, #96]               @ 0x60 
    /*00002bc0*/ ADD         R3, R3, #1 
    /*00002bc4*/ STR         R3, [IP, #80]               @ 0x50 
    /*00002bc8*/ ADD         LR, LR, R2 
    /*00002bcc*/ STR         LR, [IP, #96]               @ 0x60 
    /*00002bd0*/ BLS         SVP_NNIE_IntRoute_x2c20
    /*00002bd4*/ LDR         R2, [IP, #84]               @ 0x54 
    /*00002bd8*/ LDR         R1, [IP, #92]               @ 0x5c 
    /*00002bdc*/ SUB         R2, R3, R2 
    /*00002be0*/ STR         R2, [IP, #88]               @ 0x58 
    /*00002be4*/ CMP         R2, R1 
    /*00002be8*/ STRHI       R2, [IP, #92]               @ 0x5c 
    /*00002bec*/ MOV         R2, #216                    @ 0xd8 
    /*00002bf0*/ MLA         R2, R2, R4, R5 
    /*00002bf4*/ STR         R3, [R2, #84]               @ 0x54 
    /*00002bf8*/ MOV         R3, #216                    @ 0xd8 
    /*00002bfc*/ MLA         R4, R3, R4, R5 
    /*00002c00*/ LDR         R1, [R2, #100]              @ 0x64 
    /*00002c04*/ LDR         R0, [R2, #116]              @ 0x74 
    /*00002c08*/ SUB         R1, LR, R1 
    /*00002c0c*/ STR         R1, [R2, #112]              @ 0x70 
    /*00002c10*/ CMP         R1, R0 
    /*00002c14*/ STRHI       R1, [R2, #116]              @ 0x74 
    /*00002c18*/ STR         LR, [R4, #100]              @ 0x64 
    /*00002c1c*/ STRD        R6, [R4, #136]              @ 0x88 
    SVP_NNIE_IntRoute_x2c20:
    /*00002c20*/ MOV         R0, #1 
    /*00002c24*/ ADD         SP, SP, #36                 @ 0x24 
    /*00002c28*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_IntRoute_x2c2c:
    /*00002c2c*/ MOV         R1, #1 
    /*00002c30*/ MOV         R0, R4 
    /*00002c34*/ BL          0x0000000000002C34          @ SVP_NNIE_DRV_SetReset [R_ARM_CALL: .text+0xb5b8] 
    /*00002c38*/ MOV         R0, #1000                   @ 0x3e8 
    /*00002c3c*/ BL          0x0000000000002C3C          @ osal_udelay [R_ARM_CALL: +0x0] 
    /*00002c40*/ LDR         R1, [SP, #12] 
    /*00002c44*/ MOV         R0, R4 
    /*00002c48*/ BL          0x0000000000002C48          @ SVP_NNIE_DRV_SetReset [R_ARM_CALL: .text+0xb5b8] 
    /*00002c4c*/ MOV         R0, R4 
    /*00002c50*/ BL          0x0000000000002C50          @ SVP_NNIE_DRV_EnInt [R_ARM_CALL: .text+0xb708] 
    /*00002c54*/ MOV         R3, #255                    @ 0xff 
    /*00002c58*/ MVN         R2, #0 
    /*00002c5c*/ MOV         R0, R4 
    /*00002c60*/ BL          0x0000000000002C60          @ SVP_NNIE_DRV_SetTimeOut [R_ARM_CALL: .text+0xb70c] 
    /*00002c64*/ MOV         R0, R4 
    /*00002c68*/ BL          0x0000000000002C68          @ SVP_NNIE_DRV_EnClkGt [R_ARM_CALL: .text+0xb710] 
    /*00002c6c*/ MOV         R0, R4 
    /*00002c70*/ BL          0x0000000000002C70          @ SVP_NNIE_DRV_SetMemSpeed [R_ARM_CALL: .text+0xb714] 
    /*00002c74*/ MOV         R0, R4 
    /*00002c78*/ BL          0x0000000000002C78          @ SVP_NNIE_DRV_DisableCheckSum [R_ARM_CALL: .text+0xb7c8] 
    /*00002c7c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00002c80*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00002c84*/ LDRB        R3, [R3, #2] 
    /*00002c88*/ CMP         R3, #0 
    /*00002c8c*/ BNE         SVP_NNIE_IntRoute_x2e8c
    SVP_NNIE_IntRoute_x2c90:
    /*00002c90*/ MOV         R3, #216                    @ 0xd8 
    /*00002c94*/ MVN         R1, #0 
    /*00002c98*/ MLA         R3, R3, R4, R5 
    /*00002c9c*/ MOV         R2, #0 
    /*00002ca0*/ MOV         R0, #1 
    /*00002ca4*/ STR         R1, [R3, #12] 
    /*00002ca8*/ LDR         R1, [R3, #144]              @ 0x90 
    /*00002cac*/ STR         R0, [R3, #64]               @ 0x40 
    /*00002cb0*/ ADD         R1, R1, R0 
    /*00002cb4*/ STR         R2, [R3, #8] 
    /*00002cb8*/ STR         R1, [R3, #144]              @ 0x90 
    /*00002cbc*/ STR         R2, [R3, #20] 
    /*00002cc0*/ STR         R2, [R3, #24] 
    /*00002cc4*/ STR         R2, [R3, #28] 
    /*00002cc8*/ STR         R2, [R3, #32] 
    /*00002ccc*/ STR         R2, [R3, #36]               @ 0x24 
    /*00002cd0*/ STR         R2, [R3, #40]               @ 0x28 
    /*00002cd4*/ STR         R2, [R3, #44]               @ 0x2c 
    /*00002cd8*/ STR         R2, [R3, #48]               @ 0x30 
    /*00002cdc*/ STR         R2, [R3, #60]               @ 0x3c 
    /*00002ce0*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x2ce4:
    /*00002ce4*/ LDR         R2, [FP, #12] 
    /*00002ce8*/ CMN         R2, #1 
    /*00002cec*/ BEQ         SVP_NNIE_IntRoute_x2b44
    /*00002cf0*/ MOV         R0, R4 
    /*00002cf4*/ BL          0x0000000000002CF4          @ SVP_NNIE_DRV_GetTaskId [R_ARM_CALL: .text+0xb704] 
    /*00002cf8*/ LDR         R2, [FP, #40]               @ 0x28 
    /*00002cfc*/ LDR         R1, [FP, #44]               @ 0x2c 
    /*00002d00*/ MOV         R3, #27 
    /*00002d04*/ SUB         R2, R2, R1 
    /*00002d08*/ ADD         R2, R2, R0 
    /*00002d0c*/ ADD         R9, R0, #1 
    /*00002d10*/ ADD         R1, R2, #1 
    /*00002d14*/ STR         R0, [FP, #48]               @ 0x30 
    /*00002d18*/ CMN         R1, #-268435455             @ 0xf0000001 
    /*00002d1c*/ STRLS       R1, [FP, #40]               @ 0x28 
    /*00002d20*/ ADDHI       R2, R2, #-268435454         @ 0xf0000002 
    /*00002d24*/ STRHI       R2, [FP, #40]               @ 0x28 
    /*00002d28*/ LDRHI       R1, [FP, #160]              @ 0xa0 
    /*00002d2c*/ ADDHI       R2, R1, #1 
    /*00002d30*/ STRHI       R2, [FP, #160]              @ 0xa0 
    /*00002d34*/ MOV         R2, #216                    @ 0xd8 
    /*00002d38*/ MLA         R2, R2, R4, R5 
    /*00002d3c*/ LDR         R2, [R2, #12] 
    /*00002d40*/ MLA         R3, R3, R4, R2 
    /*00002d44*/ ADD         R3, R3, #1 
    /*00002d48*/ ADD         R3, R5, R3, LSL #3 
    /*00002d4c*/ LDR         SL, [R3, #16] 
    /*00002d50*/ STR         R9, [R3, #12] 
    /*00002d54*/ CMP         R9, SL 
    /*00002d58*/ BHI         SVP_NNIE_IntRoute_x2e6c
    SVP_NNIE_IntRoute_x2d5c:
    /*00002d5c*/ CMP         R9, SL 
    /*00002d60*/ BCC         SVP_NNIE_IntRoute_x2e9c
    /*00002d64*/ MOV         R0, #216                    @ 0xd8 
    /*00002d68*/ MOV         R3, #27 
    /*00002d6c*/ MLA         R0, R0, R4, R5 
    /*00002d70*/ MOV         R1, #0 
    /*00002d74*/ MUL         R3, R3, R4 
    /*00002d78*/ LDR         R2, [R0, #12] 
    /*00002d7c*/ LDR         IP, [R0, #8] 
    /*00002d80*/ ADD         R2, R3, R2 
    /*00002d84*/ STR         R1, [R0, #44]               @ 0x2c 
    /*00002d88*/ ADD         R3, R3, IP 
    /*00002d8c*/ ADD         R2, R2, #1 
    /*00002d90*/ ADD         R3, R3, #1 
    /*00002d94*/ ADD         R2, R5, R2, LSL #3 
    /*00002d98*/ ADD         R3, R5, R3, LSL #3 
    /*00002d9c*/ STR         R1, [R2, #12] 
    /*00002da0*/ STR         R1, [R2, #16] 
    /*00002da4*/ LDR         SL, [R3, #12] 
    /*00002da8*/ LDR         R9, [R3, #16] 
    /*00002dac*/ CMP         SL, R9 
    /*00002db0*/ BHI         SVP_NNIE_IntRoute_x317c
    SVP_NNIE_IntRoute_x2db4:
    /*00002db4*/ CMP         SL, R9 
    /*00002db8*/ MOV         R3, #216                    @ 0xd8 
    /*00002dbc*/ BCC         SVP_NNIE_IntRoute_x31f4
    /*00002dc0*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00002dc4*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00002dc8*/ MLA         R3, R3, R4, R5 
    /*00002dcc*/ LDRB        R2, [R2, #2] 
    /*00002dd0*/ MVN         R1, #0 
    /*00002dd4*/ CMP         R2, #0 
    /*00002dd8*/ STR         R1, [R3, #12] 
    /*00002ddc*/ BNE         SVP_NNIE_IntRoute_x31d4
    SVP_NNIE_IntRoute_x2de0:
    /*00002de0*/ MOV         R0, #37                     @ 0x25 
    /*00002de4*/ BL          0x0000000000002DE4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002de8*/ SUBS        R9, R0, #0 
    /*00002dec*/ BEQ         SVP_NNIE_IntRoute_x310c
    /*00002df0*/ LDR         R3, [R9, #124]              @ 0x7c 
    /*00002df4*/ CMP         R3, #0 
    /*00002df8*/ BEQ         SVP_NNIE_IntRoute_x310c
    /*00002dfc*/ LDR         R3, [R9, #128]              @ 0x80 
    /*00002e00*/ CMP         R3, #0 
    /*00002e04*/ BEQ         SVP_NNIE_IntRoute_x310c
    /*00002e08*/ LDR         R3, [R9, #132]              @ 0x84 
    /*00002e0c*/ CMP         R3, #0 
    /*00002e10*/ BEQ         SVP_NNIE_IntRoute_x310c
    /*00002e14*/ LDR         R2, [R9, #136]              @ 0x88 
    /*00002e18*/ CMP         R2, #0 
    /*00002e1c*/ BEQ         SVP_NNIE_IntRoute_x310c
    /*00002e20*/ ADD         R0, SP, #24 
    /*00002e24*/ BLX         R3 
    /*00002e28*/ LDR         R3, [R9, #128]              @ 0x80 
    /*00002e2c*/ ADD         R1, SP, #20 
    /*00002e30*/ MOV         R0, #0 
    /*00002e34*/ BLX         R3 
    /*00002e38*/ SUBS        SL, R0, #0 
    /*00002e3c*/ BNE         SVP_NNIE_IntRoute_x324c
    /*00002e40*/ LDR         R3, [SP, #20] 
    /*00002e44*/ CMP         R3, #1 
    /*00002e48*/ BEQ         SVP_NNIE_IntRoute_x32c0
    SVP_NNIE_IntRoute_x2e4c:
    /*00002e4c*/ LDR         R3, [R9, #136]              @ 0x88 
    /*00002e50*/ ADD         R0, SP, #24 
    /*00002e54*/ BLX         R3 
    /*00002e58*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x2e5c:
    /*00002e5c*/ MOV         R0, R4 
    /*00002e60*/ BL          0x0000000000002E60          @ SVP_NNIE_DRV_GetCfgErrInfo [R_ARM_CALL: .text+0xb718] 
    /*00002e64*/ MOV         R9, R0 
    /*00002e68*/ B           SVP_NNIE_IntRoute_x2ad8
    SVP_NNIE_IntRoute_x2e6c:
    /*00002e6c*/ MOVW        R3, #0                      @ .LC112 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1104] 
    /*00002e70*/ MOVW        R0, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x634] 
    /*00002e74*/ MOVT        R3, #0                      @ .LC112 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1104] 
    /*00002e78*/ MOVT        R0, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0x634] 
    /*00002e7c*/ MOV         R2, #1984                   @ 0x7c0 
    /*00002e80*/ LDR         R1, [PC, #68]               @ 0x0000000000002ecc 
    /*00002e84*/ BL          0x0000000000002E84          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002e88*/ B           SVP_NNIE_IntRoute_x2d5c
    SVP_NNIE_IntRoute_x2e8c:
    /*00002e8c*/ LDR         R1, [SP, #12] 
    /*00002e90*/ MOV         R0, R4 
    /*00002e94*/ BL          0x0000000000002E94          @ SVP_NNIE_DRV_EnableSysClk [R_ARM_CALL: .text+0xb54c] 
    /*00002e98*/ B           SVP_NNIE_IntRoute_x2c90
    SVP_NNIE_IntRoute_x2e9c:
    /*00002e9c*/ MOV         R3, #216                    @ 0xd8 
    /*00002ea0*/ MOV         R0, R4 
    /*00002ea4*/ MLA         R3, R3, R4, R5 
    /*00002ea8*/ LDR         R2, [R3, #48]               @ 0x30 
    /*00002eac*/ ADD         R2, R2, #1 
    /*00002eb0*/ STR         R2, [R3, #44]               @ 0x2c 
    /*00002eb4*/ BL          0x0000000000000558 
    /*00002eb8*/ B           SVP_NNIE_IntRoute_x2b44
    /*00002ebc*/ NOP         {0} 
    /*00002ec0*/ ANDEQ       R4, PC, PC, LSR R2          @ <UNPREDICTABLE> 
    /*00002ec4*/ ANDEQ       R0, R0, R0 
    /*00002ec8*/ LDRDEQ      R0, [R0], -IP 
    /*00002ecc*/ ANDEQ       R0, R0, R0, LSR #2 
    /*00002ed0*/ ANDEQ       R0, R0, R4, LSR R1 
    /*00002ed4*/ ANDEQ       R0, R0, R8, ASR #2 
    SVP_NNIE_IntRoute_x2ed8:
    /*00002ed8*/ MOV         R1, #1 
    /*00002edc*/ MOV         R0, R4 
    /*00002ee0*/ BL          0x0000000000002EE0          @ SVP_NNIE_DRV_SetReset [R_ARM_CALL: .text+0xb5b8] 
    /*00002ee4*/ MOV         R0, #1000                   @ 0x3e8 
    /*00002ee8*/ BL          0x0000000000002EE8          @ osal_udelay [R_ARM_CALL: +0x0] 
    /*00002eec*/ MOV         R1, FP 
    /*00002ef0*/ MOV         R0, R4 
    /*00002ef4*/ BL          0x0000000000002EF4          @ SVP_NNIE_DRV_SetReset [R_ARM_CALL: .text+0xb5b8] 
    /*00002ef8*/ MOV         R0, R4 
    /*00002efc*/ BL          0x0000000000002EFC          @ SVP_NNIE_DRV_EnInt [R_ARM_CALL: .text+0xb708] 
    /*00002f00*/ MOV         R3, #255                    @ 0xff 
    /*00002f04*/ MVN         R2, #0 
    /*00002f08*/ MOV         R0, R4 
    /*00002f0c*/ BL          0x0000000000002F0C          @ SVP_NNIE_DRV_SetTimeOut [R_ARM_CALL: .text+0xb70c] 
    /*00002f10*/ MOV         R0, R4 
    /*00002f14*/ BL          0x0000000000002F14          @ SVP_NNIE_DRV_EnClkGt [R_ARM_CALL: .text+0xb710] 
    /*00002f18*/ MOV         R0, R4 
    /*00002f1c*/ BL          0x0000000000002F1C          @ SVP_NNIE_DRV_SetMemSpeed [R_ARM_CALL: .text+0xb714] 
    /*00002f20*/ MOV         R0, R4 
    /*00002f24*/ BL          0x0000000000002F24          @ SVP_NNIE_DRV_DisableCheckSum [R_ARM_CALL: .text+0xb7c8] 
    /*00002f28*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00002f2c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00002f30*/ LDRB        R3, [R3, #2] 
    /*00002f34*/ CMP         R3, #0 
    /*00002f38*/ BNE         SVP_NNIE_IntRoute_x31e4
    SVP_NNIE_IntRoute_x2f3c:
    /*00002f3c*/ MOV         IP, #216                    @ 0xd8 
    /*00002f40*/ MOVW        R3, #1849                   @ 0x739 
    /*00002f44*/ MLA         IP, IP, R4, R5 
    /*00002f48*/ STR         R3, [SP] 
    /*00002f4c*/ MVN         LR, #0 
    /*00002f50*/ LDR         R3, [PC, #-132]             @ 0x0000000000002ed4 
    /*00002f54*/ MOV         R1, #51                     @ 0x33 
    /*00002f58*/ MOV         SL, #1 
    /*00002f5c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00002f60*/ MOV         R0, #3 
    /*00002f64*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00002f68*/ STR         LR, [IP, #12] 
    /*00002f6c*/ LDR         LR, [IP, #148]              @ 0x94 
    /*00002f70*/ STR         SL, [IP, #68]               @ 0x44 
    /*00002f74*/ ADD         LR, LR, SL 
    /*00002f78*/ STR         LR, [IP, #148]              @ 0x94 
    /*00002f7c*/ MOV         LR, #0 
    /*00002f80*/ STR         LR, [IP, #8] 
    /*00002f84*/ STR         LR, [IP, #20] 
    /*00002f88*/ STR         LR, [IP, #24] 
    /*00002f8c*/ STR         LR, [IP, #28] 
    /*00002f90*/ STR         LR, [IP, #32] 
    /*00002f94*/ STR         LR, [IP, #36]               @ 0x24 
    /*00002f98*/ STR         LR, [IP, #40]               @ 0x28 
    /*00002f9c*/ STR         LR, [IP, #44]               @ 0x2c 
    /*00002fa0*/ STR         LR, [IP, #48]               @ 0x30 
    /*00002fa4*/ STR         LR, [IP, #60]               @ 0x3c 
    /*00002fa8*/ BL          0x0000000000002FA8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002fac*/ MOV         R3, R9 
    /*00002fb0*/ MOV         R1, #51                     @ 0x33 
    /*00002fb4*/ MOVW        R2, #0                      @ .LC114 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1154] 
    /*00002fb8*/ MOV         R0, #3 
    /*00002fbc*/ MOVT        R2, #0                      @ .LC114 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1154] 
    /*00002fc0*/ BL          0x0000000000002FC0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002fc4*/ MOV         R0, R9 
    /*00002fc8*/ BL          0x0000000000002FC8          @ SVP_NNIE_DRV_IsCfgErrEmarInstrLen [R_ARM_CALL: .text+0xb71c] 
    /*00002fcc*/ CMP         R0, #0 
    /*00002fd0*/ BNE         SVP_NNIE_IntRoute_x3144
    /*00002fd4*/ MOV         R0, R9 
    /*00002fd8*/ BL          0x0000000000002FD8          @ SVP_NNIE_DRV_IsCfgErrGoto [R_ARM_CALL: .text+0xb724] 
    /*00002fdc*/ CMP         R0, #0 
    /*00002fe0*/ BNE         SVP_NNIE_IntRoute_x3214
    /*00002fe4*/ MOV         R0, R9 
    /*00002fe8*/ BL          0x0000000000002FE8          @ SVP_NNIE_DRV_IsCfgErrEmarRtnContinue [R_ARM_CALL: .text+0xb72c] 
    /*00002fec*/ CMP         R0, #0 
    /*00002ff0*/ BNE         SVP_NNIE_IntRoute_x319c
    /*00002ff4*/ MOV         R0, R9 
    /*00002ff8*/ BL          0x0000000000002FF8          @ SVP_NNIE_DRV_IsCfgErrEmarInstrNull [R_ARM_CALL: .text+0xb734] 
    /*00002ffc*/ CMP         R0, #0 
    /*00003000*/ BNE         SVP_NNIE_IntRoute_x3288
    /*00003004*/ MOV         R0, R9 
    /*00003008*/ BL          0x0000000000003008          @ SVP_NNIE_DRV_IsCfgErrDataBlockLenIsZeroErr [R_ARM_CALL: .text+0xb73c] 
    /*0000300c*/ CMP         R0, #0 
    /*00003010*/ BNE         SVP_NNIE_IntRoute_x3310
    /*00003014*/ MOV         R0, R9 
    /*00003018*/ BL          0x0000000000003018          @ SVP_NNIE_DRV_IsCfgErrDataBlockLenIsTooLongErr [R_ARM_CALL: .text+0xb744] 
    /*0000301c*/ CMP         R0, #0 
    /*00003020*/ BNE         SVP_NNIE_IntRoute_x3348
    /*00003024*/ MOV         R0, R9 
    /*00003028*/ BL          0x0000000000003028          @ SVP_NNIE_DRV_IsCfgErrStoreNotMatch [R_ARM_CALL: .text+0xb74c] 
    /*0000302c*/ CMP         R0, #0 
    /*00003030*/ BNE         SVP_NNIE_IntRoute_x3380
    /*00003034*/ MOV         R0, R9 
    /*00003038*/ BL          0x0000000000003038          @ SVP_NNIE_DRV_IsCfgErrStoreBlockIsNull [R_ARM_CALL: .text+0xb754] 
    /*0000303c*/ CMP         R0, #0 
    /*00003040*/ BNE         SVP_NNIE_IntRoute_x33b8
    /*00003044*/ MOV         R0, R9 
    /*00003048*/ BL          0x0000000000003048          @ SVP_NNIE_DRV_IsCfgErrCompressBlockIsNull [R_ARM_CALL: .text+0xb75c] 
    /*0000304c*/ CMP         R0, #0 
    /*00003050*/ BNE         SVP_NNIE_IntRoute_x33f0
    /*00003054*/ MOV         R0, R9 
    /*00003058*/ BL          0x0000000000003058          @ SVP_NNIE_DRV_IsCfgErrCheckIsErr [R_ARM_CALL: .text+0xb76c] 
    /*0000305c*/ CMP         R0, #0 
    /*00003060*/ BNE         SVP_NNIE_IntRoute_x3428
    /*00003064*/ MOV         R0, R9 
    /*00003068*/ BL          0x0000000000003068          @ SVP_NNIE_DRV_IsCfgErrCompressDataTooSmall [R_ARM_CALL: .text+0xb774] 
    /*0000306c*/ CMP         R0, #0 
    /*00003070*/ BNE         SVP_NNIE_IntRoute_x3460
    /*00003074*/ MOV         R0, R9 
    /*00003078*/ BL          0x0000000000003078          @ SVP_NNIE_DRV_IsCfgErrCompressDataLenNotMacth [R_ARM_CALL: .text+0xb784] 
    /*0000307c*/ CMP         R0, #0 
    /*00003080*/ BNE         SVP_NNIE_IntRoute_x34d0
    /*00003084*/ MOV         R0, R9 
    /*00003088*/ BL          0x0000000000003088          @ SVP_NNIE_DRV_IsCfgErrCompressDataLenTooLong [R_ARM_CALL: .text+0xb794] 
    /*0000308c*/ CMP         R0, #0 
    /*00003090*/ BNE         SVP_NNIE_IntRoute_x3498
    /*00003094*/ MOV         R0, R9 
    /*00003098*/ BL          0x0000000000003098          @ SVP_NNIE_DRV_IsCfgErrDscpCheckErr [R_ARM_CALL: .text+0xb7a4] 
    /*0000309c*/ CMP         R0, #0 
    /*000030a0*/ BNE         SVP_NNIE_IntRoute_x3540
    /*000030a4*/ MOV         R0, R9 
    /*000030a8*/ BL          0x00000000000030A8          @ SVP_NNIE_DRV_IsCfgErrDscpFunctionCheckErr [R_ARM_CALL: .text+0xb7ac] 
    /*000030ac*/ CMP         R0, #0 
    /*000030b0*/ BNE         SVP_NNIE_IntRoute_x3508
    /*000030b4*/ MOV         R0, R9 
    /*000030b8*/ BL          0x00000000000030B8          @ SVP_NNIE_DRV_IsCfgErrAddrErr [R_ARM_CALL: .text+0xb7b4] 
    /*000030bc*/ CMP         R0, #0 
    /*000030c0*/ BNE         SVP_NNIE_IntRoute_x35b0
    /*000030c4*/ MOV         R0, R9 
    /*000030c8*/ BL          0x00000000000030C8          @ SVP_NNIE_DRV_IsCfgErrNodeErr [R_ARM_CALL: .text+0xb7bc] 
    /*000030cc*/ CMP         R0, #0 
    /*000030d0*/ BNE         SVP_NNIE_IntRoute_x3578
    /*000030d4*/ MOVW        R3, #1922                   @ 0x782 
    /*000030d8*/ MOV         R1, #51                     @ 0x33 
    /*000030dc*/ STR         R3, [SP] 
    /*000030e0*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000030e4*/ LDR         R3, [PC, #-536]             @ 0x0000000000002ed4 
    /*000030e8*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000030ec*/ MOV         R0, #3 
    /*000030f0*/ BL          0x00000000000030F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000030f4*/ MOVW        R2, #0                      @ .LC132 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1590] 
    /*000030f8*/ MOV         R1, #51                     @ 0x33 
    /*000030fc*/ MOVT        R2, #0                      @ .LC132 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1590] 
    /*00003100*/ MOV         R0, #3 
    /*00003104*/ BL          0x0000000000003104          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003108*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x310c:
    /*0000310c*/ MOVW        R3, #454                    @ 0x1c6 
    /*00003110*/ MOV         R1, #51                     @ 0x33 
    /*00003114*/ STR         R3, [SP] 
    /*00003118*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*0000311c*/ LDR         R3, [PC, #-596]             @ 0x0000000000002ed0 
    /*00003120*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003124*/ MOV         R0, #4 
    /*00003128*/ BL          0x0000000000003128          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000312c*/ MOVW        R2, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x578] 
    /*00003130*/ MOV         R1, #51                     @ 0x33 
    /*00003134*/ MOVT        R2, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x578] 
    /*00003138*/ MOV         R0, #4 
    /*0000313c*/ BL          0x000000000000313C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003140*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x3144:
    /*00003144*/ MOVW        R3, #1852                   @ 0x73c 
    /*00003148*/ MOV         R1, #51                     @ 0x33 
    /*0000314c*/ STR         R3, [SP] 
    /*00003150*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003154*/ LDR         R3, [PC, #-648]             @ 0x0000000000002ed4 
    /*00003158*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*0000315c*/ MOV         R0, #3 
    /*00003160*/ BL          0x0000000000003160          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003164*/ MOVW        R2, #0                      @ .LC115 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x117c] 
    /*00003168*/ MOV         R1, #51                     @ 0x33 
    /*0000316c*/ MOVT        R2, #0                      @ .LC115 [R_ARM_MOVT_ABS: .rodata.str1.4+0x117c] 
    /*00003170*/ MOV         R0, #3 
    /*00003174*/ BL          0x0000000000003174          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003178*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x317c:
    /*0000317c*/ MOVW        R3, #0                      @ .LC113 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x112c] 
    /*00003180*/ MOVW        R0, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x634] 
    /*00003184*/ MOVT        R3, #0                      @ .LC113 [R_ARM_MOVT_ABS: .rodata.str1.4+0x112c] 
    /*00003188*/ MOVT        R0, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0x634] 
    /*0000318c*/ MOVW        R2, #2004                   @ 0x7d4 
    /*00003190*/ LDR         R1, [PC, #-716]             @ 0x0000000000002ecc 
    /*00003194*/ BL          0x0000000000003194          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003198*/ B           SVP_NNIE_IntRoute_x2db4
    SVP_NNIE_IntRoute_x319c:
    /*0000319c*/ MOVW        R3, #1861                   @ 0x745 
    /*000031a0*/ MOV         R1, #51                     @ 0x33 
    /*000031a4*/ STR         R3, [SP] 
    /*000031a8*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000031ac*/ LDR         R3, [PC, #-736]             @ 0x0000000000002ed4 
    /*000031b0*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000031b4*/ MOV         R0, #3 
    /*000031b8*/ BL          0x00000000000031B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000031bc*/ MOVW        R2, #0                      @ .LC117 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11f4] 
    /*000031c0*/ MOV         R1, #51                     @ 0x33 
    /*000031c4*/ MOVT        R2, #0                      @ .LC117 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11f4] 
    /*000031c8*/ MOV         R0, #3 
    /*000031cc*/ BL          0x00000000000031CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000031d0*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x31d4:
    /*000031d4*/ MOV         R1, #0 
    /*000031d8*/ MOV         R0, R4 
    /*000031dc*/ BL          0x00000000000031DC          @ SVP_NNIE_DRV_EnableSysClk [R_ARM_CALL: .text+0xb54c] 
    /*000031e0*/ B           SVP_NNIE_IntRoute_x2de0
    SVP_NNIE_IntRoute_x31e4:
    /*000031e4*/ MOV         R1, FP 
    /*000031e8*/ MOV         R0, R4 
    /*000031ec*/ BL          0x00000000000031EC          @ SVP_NNIE_DRV_EnableSysClk [R_ARM_CALL: .text+0xb54c] 
    /*000031f0*/ B           SVP_NNIE_IntRoute_x2f3c
    SVP_NNIE_IntRoute_x31f4:
    /*000031f4*/ MLA         R3, R3, R4, R5 
    /*000031f8*/ MOV         R0, R4 
    /*000031fc*/ LDR         R2, [R3, #8] 
    /*00003200*/ STR         R2, [R3, #12] 
    /*00003204*/ RSB         R2, R2, #1 
    /*00003208*/ STR         R2, [R3, #8] 
    /*0000320c*/ BL          0x0000000000000558 
    /*00003210*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x3214:
    /*00003214*/ MOV         R3, #1856                   @ 0x740 
    /*00003218*/ MOV         R1, #51                     @ 0x33 
    /*0000321c*/ STR         R3, [SP] 
    /*00003220*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003224*/ LDR         R3, [PC, #-856]             @ 0x0000000000002ed4 
    /*00003228*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*0000322c*/ MOV         R0, #3 
    /*00003230*/ BL          0x0000000000003230          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003234*/ MOVW        R2, #0                      @ .LC116 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11bc] 
    /*00003238*/ MOV         R1, #51                     @ 0x33 
    /*0000323c*/ MOVT        R2, #0                      @ .LC116 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11bc] 
    /*00003240*/ MOV         R0, #3 
    /*00003244*/ BL          0x0000000000003244          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003248*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x324c:
    /*0000324c*/ MOVW        R3, #462                    @ 0x1ce 
    /*00003250*/ MOV         R1, #51                     @ 0x33 
    /*00003254*/ STR         R3, [SP] 
    /*00003258*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*0000325c*/ LDR         R3, [PC, #-916]             @ 0x0000000000002ed0 
    /*00003260*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003264*/ MOV         R0, #3 
    /*00003268*/ BL          0x0000000000003268          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000326c*/ MOV         R3, SL 
    /*00003270*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5d0] 
    /*00003274*/ MOV         R1, #51                     @ 0x33 
    /*00003278*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5d0] 
    /*0000327c*/ MOV         R0, #3 
    /*00003280*/ BL          0x0000000000003280          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003284*/ B           SVP_NNIE_IntRoute_x2e4c
    SVP_NNIE_IntRoute_x3288:
    /*00003288*/ MOVW        R3, #1866                   @ 0x74a 
    /*0000328c*/ MOV         R1, #51                     @ 0x33 
    /*00003290*/ STR         R3, [SP] 
    /*00003294*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003298*/ LDR         R3, [PC, #-972]             @ 0x0000000000002ed4 
    /*0000329c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000032a0*/ MOV         R0, #3 
    /*000032a4*/ BL          0x00000000000032A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000032a8*/ MOVW        R2, #0                      @ .LC118 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x123c] 
    /*000032ac*/ MOV         R1, #51                     @ 0x33 
    /*000032b0*/ MOVT        R2, #0                      @ .LC118 [R_ARM_MOVT_ABS: .rodata.str1.4+0x123c] 
    /*000032b4*/ MOV         R0, #3 
    /*000032b8*/ BL          0x00000000000032B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000032bc*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x32c0:
    /*000032c0*/ MOV         R1, SL 
    /*000032c4*/ LDR         R3, [R9, #124]              @ 0x7c 
    /*000032c8*/ BLX         R3 
    /*000032cc*/ SUBS        SL, R0, #0 
    /*000032d0*/ BEQ         SVP_NNIE_IntRoute_x2e4c
    /*000032d4*/ MOVW        R3, #471                    @ 0x1d7 
    /*000032d8*/ MOV         R1, #51                     @ 0x33 
    /*000032dc*/ STR         R3, [SP] 
    /*000032e0*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000032e4*/ LDR         R3, [PC, #-1052]            @ 0x0000000000002ed0 
    /*000032e8*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000032ec*/ MOV         R0, #3 
    /*000032f0*/ BL          0x00000000000032F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000032f4*/ MOV         R3, SL 
    /*000032f8*/ MOVW        R2, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5f8] 
    /*000032fc*/ MOV         R1, #51                     @ 0x33 
    /*00003300*/ MOVT        R2, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5f8] 
    /*00003304*/ MOV         R0, #3 
    /*00003308*/ BL          0x0000000000003308          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000330c*/ B           SVP_NNIE_IntRoute_x2e4c
    SVP_NNIE_IntRoute_x3310:
    /*00003310*/ MOVW        R3, #1870                   @ 0x74e 
    /*00003314*/ MOV         R1, #51                     @ 0x33 
    /*00003318*/ STR         R3, [SP] 
    /*0000331c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003320*/ LDR         R3, [PC, #-1108]            @ 0x0000000000002ed4 
    /*00003324*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003328*/ MOV         R0, #3 
    /*0000332c*/ BL          0x000000000000332C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003330*/ MOVW        R2, #0                      @ .LC119 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1274] 
    /*00003334*/ MOV         R1, #51                     @ 0x33 
    /*00003338*/ MOVT        R2, #0                      @ .LC119 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1274] 
    /*0000333c*/ MOV         R0, #3 
    /*00003340*/ BL          0x0000000000003340          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003344*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x3348:
    /*00003348*/ MOVW        R3, #1874                   @ 0x752 
    /*0000334c*/ MOV         R1, #51                     @ 0x33 
    /*00003350*/ STR         R3, [SP] 
    /*00003354*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003358*/ LDR         R3, [PC, #-1164]            @ 0x0000000000002ed4 
    /*0000335c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003360*/ MOV         R0, #3 
    /*00003364*/ BL          0x0000000000003364          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003368*/ MOVW        R2, #0                      @ .LC120 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12ac] 
    /*0000336c*/ MOV         R1, #51                     @ 0x33 
    /*00003370*/ MOVT        R2, #0                      @ .LC120 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12ac] 
    /*00003374*/ MOV         R0, #3 
    /*00003378*/ BL          0x0000000000003378          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000337c*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x3380:
    /*00003380*/ MOVW        R3, #1878                   @ 0x756 
    /*00003384*/ MOV         R1, #51                     @ 0x33 
    /*00003388*/ STR         R3, [SP] 
    /*0000338c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003390*/ LDR         R3, [PC, #-1220]            @ 0x0000000000002ed4 
    /*00003394*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003398*/ MOV         R0, #3 
    /*0000339c*/ BL          0x000000000000339C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000033a0*/ MOVW        R2, #0                      @ .LC121 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12ec] 
    /*000033a4*/ MOV         R1, #51                     @ 0x33 
    /*000033a8*/ MOVT        R2, #0                      @ .LC121 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12ec] 
    /*000033ac*/ MOV         R0, #3 
    /*000033b0*/ BL          0x00000000000033B0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000033b4*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x33b8:
    /*000033b8*/ MOVW        R3, #1882                   @ 0x75a 
    /*000033bc*/ MOV         R1, #51                     @ 0x33 
    /*000033c0*/ STR         R3, [SP] 
    /*000033c4*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000033c8*/ LDR         R3, [PC, #-1276]            @ 0x0000000000002ed4 
    /*000033cc*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000033d0*/ MOV         R0, #3 
    /*000033d4*/ BL          0x00000000000033D4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000033d8*/ MOVW        R2, #0                      @ .LC122 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1348] 
    /*000033dc*/ MOV         R1, #51                     @ 0x33 
    /*000033e0*/ MOVT        R2, #0                      @ .LC122 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1348] 
    /*000033e4*/ MOV         R0, #3 
    /*000033e8*/ BL          0x00000000000033E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000033ec*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x33f0:
    /*000033f0*/ MOVW        R3, #1886                   @ 0x75e 
    /*000033f4*/ MOV         R1, #51                     @ 0x33 
    /*000033f8*/ STR         R3, [SP] 
    /*000033fc*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003400*/ LDR         R3, [PC, #-1332]            @ 0x0000000000002ed4 
    /*00003404*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003408*/ MOV         R0, #3 
    /*0000340c*/ BL          0x000000000000340C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003410*/ MOVW        R2, #0                      @ .LC123 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1374] 
    /*00003414*/ MOV         R1, #51                     @ 0x33 
    /*00003418*/ MOVT        R2, #0                      @ .LC123 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1374] 
    /*0000341c*/ MOV         R0, #3 
    /*00003420*/ BL          0x0000000000003420          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003424*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x3428:
    /*00003428*/ MOVW        R3, #1890                   @ 0x762 
    /*0000342c*/ MOV         R1, #51                     @ 0x33 
    /*00003430*/ STR         R3, [SP] 
    /*00003434*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003438*/ LDR         R3, [PC, #-1388]            @ 0x0000000000002ed4 
    /*0000343c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003440*/ MOV         R0, #3 
    /*00003444*/ BL          0x0000000000003444          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003448*/ MOVW        R2, #0                      @ .LC124 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13a4] 
    /*0000344c*/ MOV         R1, #51                     @ 0x33 
    /*00003450*/ MOVT        R2, #0                      @ .LC124 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13a4] 
    /*00003454*/ MOV         R0, #3 
    /*00003458*/ BL          0x0000000000003458          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000345c*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x3460:
    /*00003460*/ MOVW        R3, #1894                   @ 0x766 
    /*00003464*/ MOV         R1, #51                     @ 0x33 
    /*00003468*/ STR         R3, [SP] 
    /*0000346c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003470*/ LDR         R3, [PC, #-1444]            @ 0x0000000000002ed4 
    /*00003474*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003478*/ MOV         R0, #3 
    /*0000347c*/ BL          0x000000000000347C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003480*/ MOVW        R2, #0                      @ .LC125 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13cc] 
    /*00003484*/ MOV         R1, #51                     @ 0x33 
    /*00003488*/ MOVT        R2, #0                      @ .LC125 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13cc] 
    /*0000348c*/ MOV         R0, #3 
    /*00003490*/ BL          0x0000000000003490          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003494*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x3498:
    /*00003498*/ MOVW        R3, #1902                   @ 0x76e 
    /*0000349c*/ MOV         R1, #51                     @ 0x33 
    /*000034a0*/ STR         R3, [SP] 
    /*000034a4*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000034a8*/ LDR         R3, [PC, #-1500]            @ 0x0000000000002ed4 
    /*000034ac*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000034b0*/ MOV         R0, #3 
    /*000034b4*/ BL          0x00000000000034B4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000034b8*/ MOVW        R2, #0                      @ .LC127 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x146c] 
    /*000034bc*/ MOV         R1, #51                     @ 0x33 
    /*000034c0*/ MOVT        R2, #0                      @ .LC127 [R_ARM_MOVT_ABS: .rodata.str1.4+0x146c] 
    /*000034c4*/ MOV         R0, #3 
    /*000034c8*/ BL          0x00000000000034C8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000034cc*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x34d0:
    /*000034d0*/ MOVW        R3, #1898                   @ 0x76a 
    /*000034d4*/ MOV         R1, #51                     @ 0x33 
    /*000034d8*/ STR         R3, [SP] 
    /*000034dc*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000034e0*/ LDR         R3, [PC, #-1556]            @ 0x0000000000002ed4 
    /*000034e4*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000034e8*/ MOV         R0, #3 
    /*000034ec*/ BL          0x00000000000034EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000034f0*/ MOVW        R2, #0                      @ .LC126 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1410] 
    /*000034f4*/ MOV         R1, #51                     @ 0x33 
    /*000034f8*/ MOVT        R2, #0                      @ .LC126 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1410] 
    /*000034fc*/ MOV         R0, #3 
    /*00003500*/ BL          0x0000000000003500          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003504*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x3508:
    /*00003508*/ MOVW        R3, #1910                   @ 0x776 
    /*0000350c*/ MOV         R1, #51                     @ 0x33 
    /*00003510*/ STR         R3, [SP] 
    /*00003514*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003518*/ LDR         R3, [PC, #-1612]            @ 0x0000000000002ed4 
    /*0000351c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003520*/ MOV         R0, #3 
    /*00003524*/ BL          0x0000000000003524          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003528*/ MOVW        R2, #0                      @ .LC129 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14e4] 
    /*0000352c*/ MOV         R1, #51                     @ 0x33 
    /*00003530*/ MOVT        R2, #0                      @ .LC129 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14e4] 
    /*00003534*/ MOV         R0, #3 
    /*00003538*/ BL          0x0000000000003538          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000353c*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x3540:
    /*00003540*/ MOVW        R3, #1906                   @ 0x772 
    /*00003544*/ MOV         R1, #51                     @ 0x33 
    /*00003548*/ STR         R3, [SP] 
    /*0000354c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003550*/ LDR         R3, [PC, #-1668]            @ 0x0000000000002ed4 
    /*00003554*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003558*/ MOV         R0, #3 
    /*0000355c*/ BL          0x000000000000355C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003560*/ MOVW        R2, #0                      @ .LC128 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14b0] 
    /*00003564*/ MOV         R1, #51                     @ 0x33 
    /*00003568*/ MOVT        R2, #0                      @ .LC128 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14b0] 
    /*0000356c*/ MOV         R0, #3 
    /*00003570*/ BL          0x0000000000003570          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003574*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x3578:
    /*00003578*/ MOVW        R3, #1918                   @ 0x77e 
    /*0000357c*/ MOV         R1, #51                     @ 0x33 
    /*00003580*/ STR         R3, [SP] 
    /*00003584*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003588*/ LDR         R3, [PC, #-1724]            @ 0x0000000000002ed4 
    /*0000358c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003590*/ MOV         R0, #3 
    /*00003594*/ BL          0x0000000000003594          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003598*/ MOVW        R2, #0                      @ .LC131 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1564] 
    /*0000359c*/ MOV         R1, #51                     @ 0x33 
    /*000035a0*/ MOVT        R2, #0                      @ .LC131 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1564] 
    /*000035a4*/ MOV         R0, #3 
    /*000035a8*/ BL          0x00000000000035A8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000035ac*/ B           SVP_NNIE_IntRoute_x2b44
    SVP_NNIE_IntRoute_x35b0:
    /*000035b0*/ MOVW        R3, #1914                   @ 0x77a 
    /*000035b4*/ MOV         R1, #51                     @ 0x33 
    /*000035b8*/ STR         R3, [SP] 
    /*000035bc*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000035c0*/ LDR         R3, [PC, #-1780]            @ 0x0000000000002ed4 
    /*000035c4*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000035c8*/ MOV         R0, #3 
    /*000035cc*/ BL          0x00000000000035CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000035d0*/ MOVW        R2, #0                      @ .LC130 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1528] 
    /*000035d4*/ MOV         R1, #51                     @ 0x33 
    /*000035d8*/ MOVT        R2, #0                      @ .LC130 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1528] 
    /*000035dc*/ MOV         R0, #3 
    /*000035e0*/ BL          0x00000000000035E0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000035e4*/ B           SVP_NNIE_IntRoute_x2b44
FUNC_END SVP_NNIE_IntRoute


FUNC_BEGIN SVP_NNIE_ModInit
    /*000035e8*/ MOVW        R3, #0                      @ reg_otp_base_va [R_ARM_MOVW_ABS_NC: +0x0] 
    /*000035ec*/ MOVT        R3, #0                      @ reg_otp_base_va [R_ARM_MOVT_ABS: +0x0] 
    /*000035f0*/ PUSH        {R4, R5, LR} 
    /*000035f4*/ SUB         SP, SP, #44                 @ 0x2c 
    /*000035f8*/ LDR         R3, [R3] 
    /*000035fc*/ LDRB        R3, [R3, #40]               @ 0x28 
    /*00003600*/ CMP         R3, #2 
    /*00003604*/ BLS         SVP_NNIE_ModInit_x3614
    /*00003608*/ SUB         R3, R3, #5 
    /*0000360c*/ CMP         R3, #1 
    /*00003610*/ BHI         SVP_NNIE_ModInit_x383c
    SVP_NNIE_ModInit_x3614:
    /*00003614*/ BL          0x0000000000003614          @ SVP_NNIE_DRV_IsNnieReady [R_ARM_CALL: .text+0xb87c] 
    /*00003618*/ CMP         R0, #0 
    /*0000361c*/ BEQ         SVP_NNIE_ModInit_x3884
    /*00003620*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00003624*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00003628*/ LDRB        R1, [R4, #2] 
    /*0000362c*/ CMP         R1, #1 
    /*00003630*/ BLS         SVP_NNIE_ModInit_x3648
    /*00003634*/ MOVW        R0, #0                      @ .LC134 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15d8] 
    /*00003638*/ MOVT        R0, #0                      @ .LC134 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15d8] 
    /*0000363c*/ BL          0x000000000000363C          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00003640*/ MOV         R3, #1 
    /*00003644*/ STRB        R3, [R4, #2] 
    SVP_NNIE_ModInit_x3648:
    /*00003648*/ MOV         R1, #32 
    /*0000364c*/ MOVW        R3, #0                      @ .LC135 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x163c] 
    /*00003650*/ MOVW        R2, #0                      @ .LC136 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1644] 
    /*00003654*/ MOVT        R3, #0                      @ .LC135 [R_ARM_MOVT_ABS: .rodata.str1.4+0x163c] 
    /*00003658*/ MOVT        R2, #0                      @ .LC136 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1644] 
    /*0000365c*/ ADD         R0, SP, #8 
    /*00003660*/ BL          0x0000000000003660          @ osal_snprintf [R_ARM_CALL: +0x0] 
    /*00003664*/ ADD         R0, SP, #8 
    /*00003668*/ BL          0x0000000000003668          @ osal_createdev [R_ARM_CALL: +0x0] 
    /*0000366c*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003670*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003674*/ CMP         R0, #0 
    /*00003678*/ STR         R0, [R4, #296]              @ 0x128 
    /*0000367c*/ BEQ         SVP_NNIE_ModInit_x38cc
    /*00003680*/ LDR         R5, [PC, #640]              @ 0x0000000000003908 
    /*00003684*/ MOV         R2, #37                     @ 0x25 
    /*00003688*/ STR         R2, [R0, #52]               @ 0x34 
    /*0000368c*/ ADD         R2, R5, #32 
    /*00003690*/ STR         R2, [R0, #60]               @ 0x3c 
    /*00003694*/ STR         R5, [R0, #56]               @ 0x38 
    /*00003698*/ BL          0x0000000000003698          @ osal_registerdevice [R_ARM_CALL: +0x0] 
    /*0000369c*/ CMP         R0, #0 
    /*000036a0*/ BLT         SVP_NNIE_ModInit_x375c
    /*000036a4*/ ADD         R0, R5, #112                @ 0x70 
    /*000036a8*/ BL          0x00000000000036A8          @ CMPI_RegisterModule [R_ARM_CALL: +0x0] 
    /*000036ac*/ CMP         R0, #0 
    /*000036b0*/ BLT         SVP_NNIE_ModInit_x3798
    /*000036b4*/ MOVW        R0, #0                      @ .LC135 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x163c] 
    /*000036b8*/ MOV         R1, #0 
    /*000036bc*/ MOVT        R0, #0                      @ .LC135 [R_ARM_MOVT_ABS: .rodata.str1.4+0x163c] 
    /*000036c0*/ BL          0x00000000000036C0          @ osal_create_proc_entry [R_ARM_CALL: +0x0] 
    /*000036c4*/ SUBS        R2, R0, #0 
    /*000036c8*/ BEQ         SVP_NNIE_ModInit_x3800
    /*000036cc*/ MOVW        R3, #0                      @ SVP_NNIE_ProcShow [R_ARM_MOVW_ABS_NC: .text+0x3c] 
    /*000036d0*/ ADD         R0, R4, #224                @ 0xe0 
    /*000036d4*/ MOVT        R3, #0                      @ SVP_NNIE_ProcShow [R_ARM_MOVT_ABS: .text+0x3c] 
    /*000036d8*/ STR         R3, [R2, #60]               @ 0x3c 
    /*000036dc*/ BL          0x00000000000036DC          @ osal_atomic_init [R_ARM_CALL: +0x0] 
    /*000036e0*/ SUBS        R5, R0, #0 
    /*000036e4*/ BEQ         SVP_NNIE_ModInit_x37d4
    /*000036e8*/ MOVW        R3, #3202                   @ 0xc82 
    /*000036ec*/ MOV         R1, #51                     @ 0x33 
    /*000036f0*/ STR         R3, [SP] 
    /*000036f4*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000036f8*/ LDR         R3, [PC, #524]              @ 0x000000000000390c 
    /*000036fc*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003700*/ MOV         R0, #3 
    /*00003704*/ BL          0x0000000000003704          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003708*/ MOV         R1, #51                     @ 0x33 
    /*0000370c*/ MOV         R3, R5 
    /*00003710*/ MOVW        R2, #0                      @ .LC141 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e8] 
    /*00003714*/ MOV         R0, #3 
    /*00003718*/ MOVT        R2, #0                      @ .LC141 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e8] 
    /*0000371c*/ BL          0x000000000000371C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003720*/ MOVW        R0, #0                      @ .LC135 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x163c] 
    /*00003724*/ MOV         R1, #0 
    /*00003728*/ MOVT        R0, #0                      @ .LC135 [R_ARM_MOVT_ABS: .rodata.str1.4+0x163c] 
    /*0000372c*/ BL          0x000000000000372C          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    SVP_NNIE_ModInit_x3730:
    /*00003730*/ MOV         R0, #51                     @ 0x33 
    /*00003734*/ BL          0x0000000000003734          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    SVP_NNIE_ModInit_x3738:
    /*00003738*/ LDR         R0, [R4, #296]              @ 0x128 
    /*0000373c*/ BL          0x000000000000373C          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    SVP_NNIE_ModInit_x3740:
    /*00003740*/ LDR         R0, [R4, #296]              @ 0x128 
    /*00003744*/ BL          0x0000000000003744          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*00003748*/ MOV         R3, #0 
    /*0000374c*/ STR         R3, [R4, #296]              @ 0x128 
    SVP_NNIE_ModInit_x3750:
    /*00003750*/ MOV         R0, R5 
    /*00003754*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00003758*/ POP         {R4, R5, PC} 
    SVP_NNIE_ModInit_x375c:
    /*0000375c*/ MOVW        R3, #3186                   @ 0xc72 
    /*00003760*/ MOV         R1, #51                     @ 0x33 
    /*00003764*/ STR         R3, [SP] 
    /*00003768*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*0000376c*/ LDR         R3, [PC, #408]              @ 0x000000000000390c 
    /*00003770*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003774*/ MOV         R0, #3 
    /*00003778*/ MVN         R5, #0 
    /*0000377c*/ BL          0x000000000000377C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003780*/ MOVW        R2, #0                      @ .LC138 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1668] 
    /*00003784*/ MOV         R1, #51                     @ 0x33 
    /*00003788*/ MOVT        R2, #0                      @ .LC138 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1668] 
    /*0000378c*/ MOV         R0, #3 
    /*00003790*/ BL          0x0000000000003790          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003794*/ B           SVP_NNIE_ModInit_x3740
    SVP_NNIE_ModInit_x3798:
    /*00003798*/ MOVW        R3, #3191                   @ 0xc77 
    /*0000379c*/ MOV         R1, #51                     @ 0x33 
    /*000037a0*/ STR         R3, [SP] 
    /*000037a4*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000037a8*/ LDR         R3, [PC, #348]              @ 0x000000000000390c 
    /*000037ac*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000037b0*/ MOV         R0, #3 
    /*000037b4*/ MVN         R5, #0 
    /*000037b8*/ BL          0x00000000000037B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000037bc*/ MOVW        R2, #0                      @ .LC139 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1690] 
    /*000037c0*/ MOV         R1, #51                     @ 0x33 
    /*000037c4*/ MOVT        R2, #0                      @ .LC139 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1690] 
    /*000037c8*/ MOV         R0, #3 
    /*000037cc*/ BL          0x00000000000037CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000037d0*/ B           SVP_NNIE_ModInit_x3738
    SVP_NNIE_ModInit_x37d4:
    /*000037d4*/ ADD         R0, R4, #224                @ 0xe0 
    /*000037d8*/ MOV         R1, R5 
    /*000037dc*/ BL          0x00000000000037DC          @ osal_atomic_set [R_ARM_CALL: +0x0] 
    /*000037e0*/ MOVW        R1, #0                      @ .LC142 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1710] 
    /*000037e4*/ MOVW        R0, #0                      @ .LC143 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x171c] 
    /*000037e8*/ MOVT        R1, #0                      @ .LC142 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1710] 
    /*000037ec*/ MOVT        R0, #0                      @ .LC143 [R_ARM_MOVT_ABS: .rodata.str1.4+0x171c] 
    /*000037f0*/ BL          0x00000000000037F0          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000037f4*/ MOV         R0, R5 
    /*000037f8*/ ADD         SP, SP, #44                 @ 0x2c 
    /*000037fc*/ POP         {R4, R5, PC} 
    SVP_NNIE_ModInit_x3800:
    /*00003800*/ MOVW        R3, #3196                   @ 0xc7c 
    /*00003804*/ MOV         R1, #51                     @ 0x33 
    /*00003808*/ STR         R3, [SP] 
    /*0000380c*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003810*/ LDR         R3, [PC, #244]              @ 0x000000000000390c 
    /*00003814*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00003818*/ MOV         R0, #3 
    /*0000381c*/ MVN         R5, #0 
    /*00003820*/ BL          0x0000000000003820          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003824*/ MOVW        R2, #0                      @ .LC140 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16b8] 
    /*00003828*/ MOV         R1, #51                     @ 0x33 
    /*0000382c*/ MOVT        R2, #0                      @ .LC140 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16b8] 
    /*00003830*/ MOV         R0, #3 
    /*00003834*/ BL          0x0000000000003834          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003838*/ B           SVP_NNIE_ModInit_x3730
    SVP_NNIE_ModInit_x383c:
    /*0000383c*/ MOVW        R3, #3162                   @ 0xc5a 
    /*00003840*/ MOV         R1, #51                     @ 0x33 
    /*00003844*/ STR         R3, [SP] 
    /*00003848*/ MOVW        R5, #32776                  @ 0x8008 
    /*0000384c*/ LDR         R3, [PC, #184]              @ 0x000000000000390c 
    /*00003850*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00003854*/ MOV         R0, #3 
    /*00003858*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*0000385c*/ BL          0x000000000000385C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003860*/ MOV         R3, R5 
    /*00003864*/ MOVW        R2, #0                      @ .LC133 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15b8] 
    /*00003868*/ MOVT        R3, #41011                  @ 0xa033 
    /*0000386c*/ MOVT        R2, #0                      @ .LC133 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15b8] 
    /*00003870*/ MOV         R1, #51                     @ 0x33 
    /*00003874*/ MOV         R0, #3 
    /*00003878*/ MOVT        R5, #41011                  @ 0xa033 
    /*0000387c*/ BL          0x000000000000387C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003880*/ B           SVP_NNIE_ModInit_x3750
    SVP_NNIE_ModInit_x3884:
    /*00003884*/ MOVW        R3, #3167                   @ 0xc5f 
    /*00003888*/ MOV         R1, #51                     @ 0x33 
    /*0000388c*/ STR         R3, [SP] 
    /*00003890*/ MOVW        R5, #32777                  @ 0x8009 
    /*00003894*/ LDR         R3, [PC, #112]              @ 0x000000000000390c 
    /*00003898*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*0000389c*/ MOV         R0, #3 
    /*000038a0*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000038a4*/ BL          0x00000000000038A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000038a8*/ MOV         R3, R5 
    /*000038ac*/ MOVW        R2, #0                      @ .LC65 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x754] 
    /*000038b0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000038b4*/ MOVT        R2, #0                      @ .LC65 [R_ARM_MOVT_ABS: .rodata.str1.4+0x754] 
    /*000038b8*/ MOV         R1, #51                     @ 0x33 
    /*000038bc*/ MOV         R0, #3 
    /*000038c0*/ MOVT        R5, #41011                  @ 0xa033 
    /*000038c4*/ BL          0x00000000000038C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000038c8*/ B           SVP_NNIE_ModInit_x3750
    SVP_NNIE_ModInit_x38cc:
    /*000038cc*/ MOVW        R3, #3181                   @ 0xc6d 
    /*000038d0*/ MOV         R1, #51                     @ 0x33 
    /*000038d4*/ STR         R3, [SP] 
    /*000038d8*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*000038dc*/ LDR         R3, [PC, #40]               @ 0x000000000000390c 
    /*000038e0*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*000038e4*/ MOV         R0, #3 
    /*000038e8*/ MVN         R5, #0 
    /*000038ec*/ BL          0x00000000000038EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000038f0*/ MOVW        R2, #0                      @ .LC137 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1648] 
    /*000038f4*/ MOV         R1, #51                     @ 0x33 
    /*000038f8*/ MOVT        R2, #0                      @ .LC137 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1648] 
    /*000038fc*/ MOV         R0, #3 
    /*00003900*/ BL          0x0000000000003900          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003904*/ B           SVP_NNIE_ModInit_x3750
    /*00003908*/ ANDEQ       R0, R0, R4 
    /*0000390c*/ ANDEQ       R0, R0, R0, ROR #2 
FUNC_END SVP_NNIE_ModInit


FUNC_BEGIN SVP_NNIE_ModExit
    /*00003910*/ PUSH        {R4, LR} 
    /*00003914*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003918*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000391c*/ ADD         R0, R4, #224                @ 0xe0 
    /*00003920*/ BL          0x0000000000003920          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*00003924*/ MOV         R1, #0 
    /*00003928*/ MOVW        R0, #0                      @ .LC135 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x163c] 
    /*0000392c*/ MOVT        R0, #0                      @ .LC135 [R_ARM_MOVT_ABS: .rodata.str1.4+0x163c] 
    /*00003930*/ BL          0x0000000000003930          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*00003934*/ MOV         R0, #51                     @ 0x33 
    /*00003938*/ BL          0x0000000000003938          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    /*0000393c*/ LDR         R0, [R4, #296]              @ 0x128 
    /*00003940*/ CMP         R0, #0 
    /*00003944*/ BEQ         SVP_NNIE_ModExit_x395c
    /*00003948*/ BL          0x0000000000003948          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*0000394c*/ LDR         R0, [R4, #296]              @ 0x128 
    /*00003950*/ BL          0x0000000000003950          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*00003954*/ MOV         R3, #0 
    /*00003958*/ STR         R3, [R4, #296]              @ 0x128 
    SVP_NNIE_ModExit_x395c:
    /*0000395c*/ MOVW        R1, #0                      @ .LC142 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1710] 
    /*00003960*/ MOVW        R0, #0                      @ .LC144 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1738] 
    /*00003964*/ MOVT        R1, #0                      @ .LC142 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1710] 
    /*00003968*/ MOVT        R0, #0                      @ .LC144 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1738] 
    /*0000396c*/ POP         {R4, LR} 
    SVP_NNIE_ModExit_x3970:
    /*00003970*/ B           SVP_NNIE_ModExit_x3970      @ osal_printk [R_ARM_JUMP24: +0x0] 
FUNC_END SVP_NNIE_ModExit


FUNC_BEGIN SVP_NNIE_CalcSegTskBufSizeKernel
    /*00003974*/ MOVW        IP, #1692                   @ 0x69c 
    /*00003978*/ MLA         IP, IP, R2, R3 
    /*0000397c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00003980*/ SUB         SP, SP, #28 
    /*00003984*/ LDRH        FP, [IP, #20] 
    /*00003988*/ LDR         LR, [IP, #12] 
    /*0000398c*/ LDRH        R8, [IP, #16] 
    /*00003990*/ LSL         IP, FP, #3 
    /*00003994*/ CMP         LR, #2 
    /*00003998*/ ADD         IP, IP, #15 
    /*0000399c*/ STR         LR, [SP, #20] 
    /*000039a0*/ BIC         IP, IP, #15 
    /*000039a4*/ STR         IP, [SP, #16] 
    /*000039a8*/ ADDEQ       IP, IP, #8 
    /*000039ac*/ STREQ       IP, [SP, #16] 
    /*000039b0*/ CMP         R8, #0 
    /*000039b4*/ MOVEQ       R6, R8 
    /*000039b8*/ BEQ         SVP_NNIE_CalcSegTskBufSizeKernel_x3b08
    /*000039bc*/ MOVW        R5, #1692                   @ 0x69c 
    /*000039c0*/ MOV         LR, R2 
    /*000039c4*/ MLA         IP, R5, R2, R3 
    /*000039c8*/ MOV         R9, R0 
    /*000039cc*/ LDR         R4, [IP, #32] 
    /*000039d0*/ LDR         R7, [IP, #44]               @ 0x2c 
    /*000039d4*/ CMP         R4, #5 
    /*000039d8*/ MOVLS       LR, #0 
    /*000039dc*/ MOVLS       SL, #1 
    /*000039e0*/ MOVLS       R6, LR 
    /*000039e4*/ BLS         SVP_NNIE_CalcSegTskBufSizeKernel_x3a4c
    /*000039e8*/ B           SVP_NNIE_CalcSegTskBufSizeKernel_x3a68
    SVP_NNIE_CalcSegTskBufSizeKernel_x39ec:
    /*000039ec*/ TST         R5, #12 
    /*000039f0*/ BNE         SVP_NNIE_CalcSegTskBufSizeKernel_x3a60
    /*000039f4*/ TST         R5, #2 
    /*000039f8*/ BEQ         SVP_NNIE_CalcSegTskBufSizeKernel_x3a68
    /*000039fc*/ CMP         R7, #1 
    /*00003a00*/ BEQ         SVP_NNIE_CalcSegTskBufSizeKernel_x3a58
    /*00003a04*/ CMP         R7, #2 
    /*00003a08*/ BEQ         SVP_NNIE_CalcSegTskBufSizeKernel_x3a60
    /*00003a0c*/ SUB         R4, R7, #3 
    /*00003a10*/ CMP         R4, #1 
    /*00003a14*/ BHI         SVP_NNIE_CalcSegTskBufSizeKernel_x3d20
    /*00003a18*/ MOV         R4, #32 
    SVP_NNIE_CalcSegTskBufSizeKernel_x3a1c:
    /*00003a1c*/ MUL         R4, R4, R9 
    /*00003a20*/ ADD         LR, LR, #1 
    /*00003a24*/ CMP         R8, LR 
    /*00003a28*/ ADD         R4, R4, #15 
    /*00003a2c*/ BIC         R4, R4, #15 
    /*00003a30*/ ADD         R6, R6, R4 
    /*00003a34*/ BEQ         SVP_NNIE_CalcSegTskBufSizeKernel_x3b08
    /*00003a38*/ LDR         R4, [IP, #84]               @ 0x54 
    /*00003a3c*/ ADD         IP, IP, #52                 @ 0x34 
    /*00003a40*/ CMP         R4, #5 
    /*00003a44*/ LDR         R7, [IP, #44]               @ 0x2c 
    /*00003a48*/ BHI         SVP_NNIE_CalcSegTskBufSizeKernel_x3a68
    SVP_NNIE_CalcSegTskBufSizeKernel_x3a4c:
    /*00003a4c*/ LSL         R5, SL, R4 
    /*00003a50*/ TST         R5, #49                     @ 0x31 
    /*00003a54*/ BEQ         SVP_NNIE_CalcSegTskBufSizeKernel_x39ec
    SVP_NNIE_CalcSegTskBufSizeKernel_x3a58:
    /*00003a58*/ MOV         R4, #8 
    /*00003a5c*/ B           SVP_NNIE_CalcSegTskBufSizeKernel_x3a1c
    SVP_NNIE_CalcSegTskBufSizeKernel_x3a60:
    /*00003a60*/ MOV         R4, #16 
    /*00003a64*/ B           SVP_NNIE_CalcSegTskBufSizeKernel_x3a1c
    SVP_NNIE_CalcSegTskBufSizeKernel_x3a68:
    /*00003a68*/ MOVW        R2, #819                    @ 0x333 
    /*00003a6c*/ MOV         R1, #51                     @ 0x33 
    /*00003a70*/ STR         R2, [SP] 
    /*00003a74*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x174] 
    /*00003a78*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00003a7c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x174] 
    /*00003a80*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00003a84*/ MOV         R0, #3 
    /*00003a88*/ BL          0x0000000000003A88          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003a8c*/ MOV         R3, #4 
    /*00003a90*/ MOV         R2, #0 
    /*00003a94*/ STR         R3, [SP, #8] 
    /*00003a98*/ STR         R2, [SP, #4] 
    /*00003a9c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00003aa0*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17a4] 
    /*00003aa4*/ STR         R4, [SP] 
    /*00003aa8*/ MOVT        R3, #41011                  @ 0xa033 
    /*00003aac*/ MOV         R1, #51                     @ 0x33 
    /*00003ab0*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17a4] 
    /*00003ab4*/ MOV         R0, #3 
    /*00003ab8*/ BL          0x0000000000003AB8          @ HI_LOG [R_ARM_CALL: +0x0] 
    SVP_NNIE_CalcSegTskBufSizeKernel_x3abc:
    /*00003abc*/ MOVW        R3, #877                    @ 0x36d 
    /*00003ac0*/ MOV         R1, #51                     @ 0x33 
    /*00003ac4*/ STR         R3, [SP] 
    /*00003ac8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00003acc*/ LDR         R3, [PC, #688]              @ 0x0000000000003d84 
    /*00003ad0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00003ad4*/ MOV         R0, #3 
    /*00003ad8*/ BL          0x0000000000003AD8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003adc*/ MOV         R1, #51                     @ 0x33 
    /*00003ae0*/ MOVW        R3, #32771                  @ 0x8003 
    /*00003ae4*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17d0] 
    /*00003ae8*/ MOVT        R3, #41011                  @ 0xa033 
    /*00003aec*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17d0] 
    /*00003af0*/ MOV         R0, #3 
    /*00003af4*/ BL          0x0000000000003AF4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003af8*/ MOVW        R0, #32771                  @ 0x8003 
    /*00003afc*/ MOVT        R0, #41011                  @ 0xa033 
    /*00003b00*/ ADD         SP, SP, #28 
    /*00003b04*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_CalcSegTskBufSizeKernel_x3b08:
    /*00003b08*/ CMP         FP, #0 
    /*00003b0c*/ BEQ         SVP_NNIE_CalcSegTskBufSizeKernel_x3bf4
    /*00003b10*/ MOVW        IP, #1692                   @ 0x69c 
    /*00003b14*/ MOV         SL, #104                    @ 0x68 
    /*00003b18*/ MLA         IP, IP, R2, R3 
    /*00003b1c*/ MOVW        R9, #13564                  @ 0x34fc 
    /*00003b20*/ MOVW        R7, #13560                  @ 0x34f8 
    /*00003b24*/ MOVW        R5, #13616                  @ 0x3530 
    /*00003b28*/ SUB         LR, R1, #1 
    /*00003b2c*/ LSL         R4, R1, #2 
    /*00003b30*/ ADD         R4, R4, #15 
    /*00003b34*/ LSR         LR, LR, #2 
    /*00003b38*/ BIC         R4, R4, #15 
    /*00003b3c*/ LDR         IP, [IP, #1696]             @ 0x6a0 
    /*00003b40*/ ADD         LR, LR, #1 
    /*00003b44*/ ADD         R4, R4, R1, LSL #4 
    /*00003b48*/ LSL         LR, LR, #4 
    /*00003b4c*/ MLA         IP, SL, IP, R3 
    /*00003b50*/ LDR         R9, [IP, R9] 
    /*00003b54*/ LDR         R7, [IP, R7] 
    /*00003b58*/ LDR         R5, [IP, R5] 
    /*00003b5c*/ MUL         R9, R7, R9 
    /*00003b60*/ CMP         R5, #0 
    /*00003b64*/ MUL         R9, R1, R9 
    /*00003b68*/ BEQ         SVP_NNIE_CalcSegTskBufSizeKernel_x3c74
    /*00003b6c*/ CMP         R5, #1 
    /*00003b70*/ MOVNE       IP, #0 
    /*00003b74*/ BEQ         SVP_NNIE_CalcSegTskBufSizeKernel_x3cec
    SVP_NNIE_CalcSegTskBufSizeKernel_x3b78:
    /*00003b78*/ CMP         FP, #1 
    /*00003b7c*/ ADD         IP, IP, R4 
    /*00003b80*/ BEQ         SVP_NNIE_CalcSegTskBufSizeKernel_x3d7c
    /*00003b84*/ MOVW        R5, #1692                   @ 0x69c 
    /*00003b88*/ MOV         FP, #104                    @ 0x68 
    /*00003b8c*/ MLA         R5, R2, R5, R5 
    /*00003b90*/ MOVW        SL, #13560                  @ 0x34f8 
    /*00003b94*/ MOVW        R9, #13564                  @ 0x34fc 
    /*00003b98*/ MOVW        R7, #13616                  @ 0x3530 
    /*00003b9c*/ ADD         R5, R3, R5 
    /*00003ba0*/ LDR         R5, [R5, #8] 
    /*00003ba4*/ MLA         FP, FP, R5, R3 
    /*00003ba8*/ LDR         R5, [FP, SL] 
    /*00003bac*/ LDR         R9, [FP, R9] 
    /*00003bb0*/ LDR         R7, [FP, R7] 
    /*00003bb4*/ MUL         R5, R9, R5 
    /*00003bb8*/ CMP         R7, #0 
    /*00003bbc*/ MUL         R5, R1, R5 
    /*00003bc0*/ BNE         SVP_NNIE_CalcSegTskBufSizeKernel_x3ca4
    /*00003bc4*/ MOVW        R7, #13592                  @ 0x3518 
    /*00003bc8*/ LDR         R7, [FP, R7] 
    /*00003bcc*/ CMN         R7, #1 
    /*00003bd0*/ LSRNE       R7, R7, #2 
    /*00003bd4*/ ADDNE       R7, R7, #2 
    /*00003bd8*/ MOVEQ       R7, #2 
    /*00003bdc*/ ADD         R5, R7, R5 
    /*00003be0*/ CMP         R1, #0 
    /*00003be4*/ ADD         IP, IP, R5, LSL #4 
    /*00003be8*/ MOVEQ       LR, #16 
    /*00003bec*/ ADD         IP, LR, IP 
    SVP_NNIE_CalcSegTskBufSizeKernel_x3bf0:
    /*00003bf0*/ ADD         FP, IP, R4 
    SVP_NNIE_CalcSegTskBufSizeKernel_x3bf4:
    /*00003bf4*/ LDR         R1, [SP, #20] 
    /*00003bf8*/ LDR         IP, [SP, #16] 
    /*00003bfc*/ CMP         R1, #2 
    /*00003c00*/ ADDEQ       R1, R0, #15 
    /*00003c04*/ ADD         IP, IP, #15 
    /*00003c08*/ MOVEQ       R0, #80                     @ 0x50 
    /*00003c0c*/ BIC         IP, IP, #15 
    /*00003c10*/ LSREQ       R1, R1, #4 
    /*00003c14*/ MOVNE       R1, #0 
    /*00003c18*/ MULEQ       R1, R0, R1 
    /*00003c1c*/ MOVW        R0, #1692                   @ 0x69c 
    /*00003c20*/ MLA         R2, R0, R2, R3 
    /*00003c24*/ MOV         R0, #0 
    /*00003c28*/ LDRH        R2, [R2, #18] 
    /*00003c2c*/ ADD         R3, R8, R2 
    /*00003c30*/ LSL         R2, R2, #3 
    /*00003c34*/ LSL         R3, R3, #2 
    /*00003c38*/ ADD         R2, R2, #15 
    /*00003c3c*/ ADD         R3, R3, #15 
    /*00003c40*/ BIC         R2, R2, #15 
    /*00003c44*/ BIC         R3, R3, #15 
    /*00003c48*/ ADD         R3, R3, R2 
    /*00003c4c*/ ADD         R3, R3, IP 
    /*00003c50*/ ADD         R6, R3, R6 
    /*00003c54*/ LDR         R3, [SP, #64]               @ 0x40 
    /*00003c58*/ STR         R6, [R3] 
    /*00003c5c*/ LDR         R3, [SP, #68]               @ 0x44 
    /*00003c60*/ STR         FP, [R3] 
    /*00003c64*/ LDR         R3, [SP, #72]               @ 0x48 
    /*00003c68*/ STR         R1, [R3] 
    /*00003c6c*/ ADD         SP, SP, #28 
    /*00003c70*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_CalcSegTskBufSizeKernel_x3c74:
    /*00003c74*/ MOVW        R5, #13592                  @ 0x3518 
    /*00003c78*/ LDR         IP, [IP, R5] 
    /*00003c7c*/ CMN         IP, #1 
    /*00003c80*/ LSRNE       IP, IP, #2 
    /*00003c84*/ ADDNE       IP, IP, #2 
    /*00003c88*/ MOVEQ       IP, #2 
    /*00003c8c*/ CMP         R1, #0 
    /*00003c90*/ ADD         R9, IP, R9 
    /*00003c94*/ MOVNE       IP, LR 
    /*00003c98*/ MOVEQ       IP, #16 
    /*00003c9c*/ ADD         IP, IP, R9, LSL #4 
    /*00003ca0*/ B           SVP_NNIE_CalcSegTskBufSizeKernel_x3b78
    SVP_NNIE_CalcSegTskBufSizeKernel_x3ca4:
    /*00003ca4*/ CMP         R7, #1 
    /*00003ca8*/ BNE         SVP_NNIE_CalcSegTskBufSizeKernel_x3bf0
    /*00003cac*/ MOVW        R1, #13592                  @ 0x3518 
    /*00003cb0*/ LDR         R1, [FP, R1] 
    /*00003cb4*/ CMN         R1, #1 
    /*00003cb8*/ LSRNE       R1, R1, #2 
    /*00003cbc*/ ADDNE       R1, R1, #2 
    /*00003cc0*/ MOVEQ       R1, #2 
    /*00003cc4*/ ADD         R1, R1, R5 
    /*00003cc8*/ CMP         R5, #0 
    /*00003ccc*/ ADD         R1, IP, R1, LSL #4 
    /*00003cd0*/ SUBNE       IP, R5, #1 
    /*00003cd4*/ MOVEQ       IP, #16 
    /*00003cd8*/ LSRNE       IP, IP, #2 
    /*00003cdc*/ ADDNE       IP, IP, #1 
    /*00003ce0*/ LSLNE       IP, IP, #4 
    /*00003ce4*/ ADD         IP, IP, R1 
    /*00003ce8*/ B           SVP_NNIE_CalcSegTskBufSizeKernel_x3bf0
    SVP_NNIE_CalcSegTskBufSizeKernel_x3cec:
    /*00003cec*/ MOVW        R5, #13592                  @ 0x3518 
    /*00003cf0*/ LDR         IP, [IP, R5] 
    /*00003cf4*/ CMN         IP, #1 
    /*00003cf8*/ LSRNE       IP, IP, #2 
    /*00003cfc*/ ADDNE       IP, IP, #2 
    /*00003d00*/ MOVEQ       IP, #2 
    /*00003d04*/ CMP         R9, #0 
    /*00003d08*/ ADD         IP, IP, R9 
    /*00003d0c*/ LSL         R5, IP, #4 
    /*00003d10*/ BNE         SVP_NNIE_CalcSegTskBufSizeKernel_x3d68
    /*00003d14*/ MOV         IP, #16 
    SVP_NNIE_CalcSegTskBufSizeKernel_x3d18:
    /*00003d18*/ ADD         IP, IP, R5 
    /*00003d1c*/ B           SVP_NNIE_CalcSegTskBufSizeKernel_x3b78
    SVP_NNIE_CalcSegTskBufSizeKernel_x3d20:
    /*00003d20*/ MOVW        R2, #805                    @ 0x325 
    /*00003d24*/ MOV         R1, #51                     @ 0x33 
    /*00003d28*/ STR         R2, [SP] 
    /*00003d2c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x174] 
    /*00003d30*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00003d34*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x174] 
    /*00003d38*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00003d3c*/ MOV         R0, #3 
    /*00003d40*/ BL          0x0000000000003D40          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003d44*/ STR         R7, [SP] 
    /*00003d48*/ MOVW        R3, #32771                  @ 0x8003 
    /*00003d4c*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1774] 
    /*00003d50*/ MOVT        R3, #41011                  @ 0xa033 
    /*00003d54*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1774] 
    /*00003d58*/ MOV         R1, #51                     @ 0x33 
    /*00003d5c*/ MOV         R0, #3 
    /*00003d60*/ BL          0x0000000000003D60          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003d64*/ B           SVP_NNIE_CalcSegTskBufSizeKernel_x3abc
    SVP_NNIE_CalcSegTskBufSizeKernel_x3d68:
    /*00003d68*/ SUB         IP, R9, #1 
    /*00003d6c*/ LSR         IP, IP, #2 
    /*00003d70*/ ADD         IP, IP, #1 
    /*00003d74*/ LSL         IP, IP, #4 
    /*00003d78*/ B           SVP_NNIE_CalcSegTskBufSizeKernel_x3d18
    SVP_NNIE_CalcSegTskBufSizeKernel_x3d7c:
    /*00003d7c*/ MOV         FP, IP 
    /*00003d80*/ B           SVP_NNIE_CalcSegTskBufSizeKernel_x3bf4
    /*00003d84*/ MULEQ       R0, R0, R1 
FUNC_END SVP_NNIE_CalcSegTskBufSizeKernel


FUNC_BEGIN SVP_NNIE_CheckSrcBlob
    /*00003d88*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00003d8c*/ SUBS        R8, R1, #0 
    /*00003d90*/ SUB         SP, SP, #36                 @ 0x24 
    /*00003d94*/ BEQ         SVP_NNIE_CheckSrcBlob_x40ac
    /*00003d98*/ MOV         R1, R3 
    /*00003d9c*/ MOVW        R5, #1692                   @ 0x69c 
    /*00003da0*/ MLA         R5, R5, R1, R2 
    /*00003da4*/ STR         R3, [SP, #28] 
    /*00003da8*/ LDR         R3, [R0] 
    /*00003dac*/ MOV         R9, R0 
    /*00003db0*/ STR         R2, [SP, #24] 
    /*00003db4*/ LDR         IP, [R5, #32] 
    /*00003db8*/ CMP         IP, R3 
    /*00003dbc*/ MOVEQ       FP, #0 
    /*00003dc0*/ MOVEQ       R4, R0 
    /*00003dc4*/ MOVEQ       R6, FP 
    /*00003dc8*/ BNE         SVP_NNIE_CheckSrcBlob_x4490
    SVP_NNIE_CheckSrcBlob_x3dcc:
    /*00003dcc*/ CMP         IP, #5 
    /*00003dd0*/ BEQ         SVP_NNIE_CheckSrcBlob_x3ffc
    /*00003dd4*/ LDR         R1, [R4, #40]               @ 0x28 
    /*00003dd8*/ LDR         R2, [R5, #44]               @ 0x2c 
    /*00003ddc*/ CMP         R1, R2 
    /*00003de0*/ BNE         SVP_NNIE_CheckSrcBlob_x42d0
    /*00003de4*/ LDR         R2, [R4, #32] 
    /*00003de8*/ LDR         R1, [R5, #36]               @ 0x24 
    /*00003dec*/ CMP         R2, R1 
    /*00003df0*/ BNE         SVP_NNIE_CheckSrcBlob_x4330
    /*00003df4*/ LDR         R0, [R4, #36]               @ 0x24 
    /*00003df8*/ LDR         R1, [R5, #40]               @ 0x28 
    /*00003dfc*/ CMP         R0, R1 
    /*00003e00*/ BNE         SVP_NNIE_CheckSrcBlob_x4390
    /*00003e04*/ SUB         R1, IP, #4 
    /*00003e08*/ CMP         IP, #0 
    /*00003e0c*/ CMPNE       R1, #1 
    /*00003e10*/ MOVHI       R7, #1 
    /*00003e14*/ MOVLS       R7, #4 
    SVP_NNIE_CheckSrcBlob_x3e18:
    /*00003e18*/ LDR         R1, [R4, #4] 
    /*00003e1c*/ CMP         R1, #0 
    /*00003e20*/ BEQ         SVP_NNIE_CheckSrcBlob_x40b4
    /*00003e24*/ TST         R1, #15 
    /*00003e28*/ BNE         SVP_NNIE_CheckSrcBlob_x410c
    /*00003e2c*/ MUL         R2, R2, R7 
    /*00003e30*/ ADD         R2, R2, #15 
    /*00003e34*/ BIC         R2, R2, #15 
    /*00003e38*/ CMP         R1, R2 
    /*00003e3c*/ BCC         SVP_NNIE_CheckSrcBlob_x4168
    /*00003e40*/ CMP         IP, #5 
    /*00003e44*/ BEQ         SVP_NNIE_CheckSrcBlob_x3e94
    /*00003e48*/ CMP         IP, #2 
    /*00003e4c*/ BEQ         SVP_NNIE_CheckSrcBlob_x408c
    /*00003e50*/ CMP         IP, #3 
    /*00003e54*/ LDRNE       R7, [R4, #24] 
    /*00003e58*/ LDREQ       R2, [R4, #24] 
    /*00003e5c*/ LDRNE       R2, [R4, #40]               @ 0x28 
    /*00003e60*/ MULNE       R1, R7, R1 
    /*00003e64*/ LDREQ       R7, [R4, #36]               @ 0x24 
    /*00003e68*/ MULEQ       R1, R2, R1 
    /*00003e6c*/ LDRNE       R7, [R4, #36]               @ 0x24 
    /*00003e70*/ MULNE       R1, R2, R1 
    /*00003e74*/ LSLEQ       R1, R1, #1 
    /*00003e78*/ MUL         R7, R7, R1 
    SVP_NNIE_CheckSrcBlob_x3e7c:
    /*00003e7c*/ LDR         SL, [R4, #16] 
    /*00003e80*/ MOV         R1, R7 
    /*00003e84*/ MOV         R0, SL 
    /*00003e88*/ BL          0x0000000000003E88          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00003e8c*/ CMP         R0, #0 
    /*00003e90*/ BNE         SVP_NNIE_CheckSrcBlob_x4020
    SVP_NNIE_CheckSrcBlob_x3e94:
    /*00003e94*/ LDRD        R0, [R4, #16] 
    /*00003e98*/ MOV         R1, #0 
    /*00003e9c*/ AND         R0, R0, #15 
    /*00003ea0*/ ORRS        R3, R0, R1 
    /*00003ea4*/ BNE         SVP_NNIE_CheckSrcBlob_x41d0
    /*00003ea8*/ LDR         R2, [R4, #24] 
    /*00003eac*/ CMP         R2, #0 
    /*00003eb0*/ BEQ         SVP_NNIE_CheckSrcBlob_x422c
    /*00003eb4*/ LDR         R1, [R4] 
    /*00003eb8*/ SUB         R0, R1, #1 
    /*00003ebc*/ CMP         R0, #2 
    /*00003ec0*/ BLS         SVP_NNIE_CheckSrcBlob_x3f98
    /*00003ec4*/ CMP         R2, #256                    @ 0x100 
    /*00003ec8*/ MOVLS       R0, #0 
    /*00003ecc*/ MOVHI       R0, #1 
    /*00003ed0*/ CMP         R1, #5 
    /*00003ed4*/ MOVNE       R0, #0 
    /*00003ed8*/ CMP         R0, #0 
    /*00003edc*/ BNE         SVP_NNIE_CheckSrcBlob_x449c
    SVP_NNIE_CheckSrcBlob_x3ee0:
    /*00003ee0*/ CMP         R6, #0 
    /*00003ee4*/ BEQ         SVP_NNIE_CheckSrcBlob_x3ef0
    /*00003ee8*/ CMP         R2, FP 
    /*00003eec*/ BNE         SVP_NNIE_CheckSrcBlob_x426c
    SVP_NNIE_CheckSrcBlob_x3ef0:
    /*00003ef0*/ ADD         R6, R6, #1 
    /*00003ef4*/ CMP         R8, R6 
    /*00003ef8*/ BEQ         SVP_NNIE_CheckSrcBlob_x40ac
    /*00003efc*/ ADD         R5, R5, #52                 @ 0x34 
    /*00003f00*/ LDR         IP, [R4, #48]!              @ 0x30 
    /*00003f04*/ MOV         FP, R2 
    /*00003f08*/ LDR         R2, [R5, #32] 
    /*00003f0c*/ CMP         IP, R2 
    /*00003f10*/ BEQ         SVP_NNIE_CheckSrcBlob_x3dcc
    SVP_NNIE_CheckSrcBlob_x3f14:
    /*00003f14*/ MOVW        R3, #426                    @ 0x1aa 
    /*00003f18*/ MOV         R1, #51                     @ 0x33 
    /*00003f1c*/ STR         R3, [SP] 
    /*00003f20*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00003f24*/ LDR         R3, [PC, #1400]             @ 0x00000000000044a4 
    /*00003f28*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00003f2c*/ MOV         R0, #3 
    /*00003f30*/ BL          0x0000000000003F30          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003f34*/ LDR         R5, [SP, #28] 
    /*00003f38*/ MOVW        R1, #1692                   @ 0x69c 
    /*00003f3c*/ LDR         R0, [SP, #24] 
    /*00003f40*/ LDR         LR, [R4] 
    /*00003f44*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17fc] 
    /*00003f48*/ MUL         R3, R1, R5 
    /*00003f4c*/ MOV         R1, #52                     @ 0x34 
    /*00003f50*/ MLA         R1, R1, R6, R3 
    /*00003f54*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17fc] 
    /*00003f58*/ MOVW        R3, #32771                  @ 0x8003 
    /*00003f5c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00003f60*/ ADD         FP, R0, R1 
    /*00003f64*/ MOV         R1, #51                     @ 0x33 
    /*00003f68*/ MOV         R0, #3 
    /*00003f6c*/ LDR         IP, [FP, #32] 
    SVP_NNIE_CheckSrcBlob_x3f70:
    /*00003f70*/ STR         LR, [SP, #4] 
    /*00003f74*/ STR         R6, [SP, #12] 
    /*00003f78*/ STR         IP, [SP, #16] 
    /*00003f7c*/ STR         R5, [SP, #8] 
    /*00003f80*/ STR         R6, [SP] 
    /*00003f84*/ BL          0x0000000000003F84          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003f88*/ MOVW        R0, #32771                  @ 0x8003 
    /*00003f8c*/ MOVT        R0, #41011                  @ 0xa033 
    SVP_NNIE_CheckSrcBlob_x3f90:
    /*00003f90*/ ADD         SP, SP, #36                 @ 0x24 
    /*00003f94*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_CheckSrcBlob_x3f98:
    /*00003f98*/ CMP         R2, #256                    @ 0x100 
    /*00003f9c*/ BLS         SVP_NNIE_CheckSrcBlob_x3ee0
    /*00003fa0*/ MOVW        R3, #518                    @ 0x206 
    SVP_NNIE_CheckSrcBlob_x3fa4:
    /*00003fa4*/ STR         R3, [SP] 
    /*00003fa8*/ MOV         R1, #51                     @ 0x33 
    /*00003fac*/ LDR         R3, [PC, #1264]             @ 0x00000000000044a4 
    /*00003fb0*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00003fb4*/ MOV         R0, #3 
    /*00003fb8*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00003fbc*/ BL          0x0000000000003FBC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003fc0*/ LDR         R2, [R4, #24] 
    /*00003fc4*/ MOV         R1, #256                    @ 0x100 
    /*00003fc8*/ STR         R6, [SP] 
    /*00003fcc*/ STR         R1, [SP, #8] 
    /*00003fd0*/ MOVW        R3, #32771                  @ 0x8003 
    /*00003fd4*/ STR         R2, [SP, #4] 
    /*00003fd8*/ MOVT        R3, #41011                  @ 0xa033 
    /*00003fdc*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1be4] 
    /*00003fe0*/ MOV         R1, #51                     @ 0x33 
    /*00003fe4*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1be4] 
    /*00003fe8*/ MOV         R0, #3 
    /*00003fec*/ BL          0x0000000000003FEC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003ff0*/ MOVW        R0, #32771                  @ 0x8003 
    /*00003ff4*/ MOVT        R0, #41011                  @ 0xa033 
    /*00003ff8*/ B           SVP_NNIE_CheckSrcBlob_x3f90
    SVP_NNIE_CheckSrcBlob_x3ffc:
    /*00003ffc*/ LDR         R2, [R4, #32] 
    /*00004000*/ LDR         R1, [R5, #36]               @ 0x24 
    /*00004004*/ CMP         R2, R1 
    /*00004008*/ BNE         SVP_NNIE_CheckSrcBlob_x43f0
    /*0000400c*/ LDRD        R0, [R4, #40]               @ 0x28 
    /*00004010*/ ORRS        R3, R0, R1 
    /*00004014*/ BEQ         SVP_NNIE_CheckSrcBlob_x443c
    /*00004018*/ MOV         R7, #4 
    /*0000401c*/ B           SVP_NNIE_CheckSrcBlob_x3e18
    SVP_NNIE_CheckSrcBlob_x4020:
    /*00004020*/ MOV         R0, SL 
    /*00004024*/ MOV         R1, R7 
    /*00004028*/ BL          0x0000000000004028          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*0000402c*/ CMP         R0, #0 
    /*00004030*/ BEQ         SVP_NNIE_CheckSrcBlob_x3e94
    /*00004034*/ MOVW        R3, #503                    @ 0x1f7 
    /*00004038*/ MOV         R1, #51                     @ 0x33 
    /*0000403c*/ STR         R3, [SP] 
    /*00004040*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004044*/ LDR         R3, [PC, #1112]             @ 0x00000000000044a4 
    /*00004048*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*0000404c*/ MOV         R0, #3 
    /*00004050*/ BL          0x0000000000004050          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004054*/ LDRD        R0, [R4, #16] 
    /*00004058*/ MOVW        R3, #32771                  @ 0x8003 
    /*0000405c*/ STR         R7, [SP, #16] 
    /*00004060*/ STR         R6, [SP] 
    /*00004064*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004068*/ STRD        R0, [SP, #8] 
    /*0000406c*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c6c] 
    /*00004070*/ MOV         R1, #51                     @ 0x33 
    /*00004074*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c6c] 
    /*00004078*/ MOV         R0, #3 
    /*0000407c*/ BL          0x000000000000407C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004080*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004084*/ MOVT        R0, #41011                  @ 0xa033 
    /*00004088*/ B           SVP_NNIE_CheckSrcBlob_x3f90
    SVP_NNIE_CheckSrcBlob_x408c:
    /*0000408c*/ LDR         R7, [R4, #24] 
    /*00004090*/ LDR         R2, [R4, #40]               @ 0x28 
    /*00004094*/ MUL         R1, R7, R1 
    /*00004098*/ LDR         R7, [R4, #36]               @ 0x24 
    /*0000409c*/ MUL         R1, R2, R1 
    /*000040a0*/ MUL         R7, R7, R1 
    /*000040a4*/ LSR         R7, R7, #1 
    /*000040a8*/ B           SVP_NNIE_CheckSrcBlob_x3e7c
    SVP_NNIE_CheckSrcBlob_x40ac:
    /*000040ac*/ MOV         R0, #0 
    /*000040b0*/ B           SVP_NNIE_CheckSrcBlob_x3f90
    SVP_NNIE_CheckSrcBlob_x40b4:
    /*000040b4*/ MOVW        R3, #470                    @ 0x1d6 
    /*000040b8*/ MOV         R1, #51                     @ 0x33 
    /*000040bc*/ STR         R3, [SP] 
    /*000040c0*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000040c4*/ LDR         R3, [PC, #984]              @ 0x00000000000044a4 
    /*000040c8*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000040cc*/ MOV         R0, #3 
    /*000040d0*/ BL          0x00000000000040D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000040d4*/ LDR         R1, [R4, #4] 
    /*000040d8*/ MOVW        R3, #32771                  @ 0x8003 
    /*000040dc*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ab0] 
    /*000040e0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000040e4*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ab0] 
    /*000040e8*/ STR         R6, [SP] 
    /*000040ec*/ STR         R1, [SP, #4] 
    SVP_NNIE_CheckSrcBlob_x40f0:
    /*000040f0*/ MOV         R1, #51                     @ 0x33 
    /*000040f4*/ MOV         R0, #3 
    /*000040f8*/ BL          0x00000000000040F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000040fc*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004100*/ MOVT        R0, #41011                  @ 0xa033 
    /*00004104*/ ADD         SP, SP, #36                 @ 0x24 
    /*00004108*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_CheckSrcBlob_x410c:
    /*0000410c*/ MOVW        R3, #473                    @ 0x1d9 
    /*00004110*/ MOV         R1, #51                     @ 0x33 
    /*00004114*/ STR         R3, [SP] 
    /*00004118*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*0000411c*/ LDR         R3, [PC, #896]              @ 0x00000000000044a4 
    /*00004120*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004124*/ MOV         R0, #3 
    /*00004128*/ BL          0x0000000000004128          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000412c*/ LDR         R2, [R4, #4] 
    /*00004130*/ MOV         R1, #16 
    /*00004134*/ STR         R6, [SP] 
    /*00004138*/ STR         R1, [SP, #8] 
    /*0000413c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004140*/ STR         R2, [SP, #4] 
    /*00004144*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004148*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ae4] 
    /*0000414c*/ MOV         R1, #51                     @ 0x33 
    /*00004150*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ae4] 
    /*00004154*/ MOV         R0, #3 
    /*00004158*/ BL          0x0000000000004158          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000415c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004160*/ MOVT        R0, #41011                  @ 0xa033 
    /*00004164*/ B           SVP_NNIE_CheckSrcBlob_x3f90
    SVP_NNIE_CheckSrcBlob_x4168:
    /*00004168*/ MOV         R3, #476                    @ 0x1dc 
    /*0000416c*/ MOV         R1, #51                     @ 0x33 
    /*00004170*/ STR         R3, [SP] 
    /*00004174*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004178*/ LDR         R3, [PC, #804]              @ 0x00000000000044a4 
    /*0000417c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004180*/ MOV         R0, #3 
    /*00004184*/ BL          0x0000000000004184          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004188*/ LDR         IP, [R4, #32] 
    /*0000418c*/ LDR         R2, [R4, #4] 
    /*00004190*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004194*/ STR         R6, [SP] 
    /*00004198*/ MOVT        R3, #41011                  @ 0xa033 
    /*0000419c*/ MUL         IP, IP, R7 
    /*000041a0*/ STR         R2, [SP, #4] 
    /*000041a4*/ MOV         R1, #51                     @ 0x33 
    /*000041a8*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b24] 
    /*000041ac*/ MOV         R0, #3 
    /*000041b0*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b24] 
    /*000041b4*/ ADD         IP, IP, #15 
    /*000041b8*/ BIC         IP, IP, #15 
    /*000041bc*/ STR         IP, [SP, #8] 
    /*000041c0*/ BL          0x00000000000041C0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000041c4*/ MOVW        R0, #32771                  @ 0x8003 
    /*000041c8*/ MOVT        R0, #41011                  @ 0xa033 
    /*000041cc*/ B           SVP_NNIE_CheckSrcBlob_x3f90
    SVP_NNIE_CheckSrcBlob_x41d0:
    /*000041d0*/ MOVW        R3, #507                    @ 0x1fb 
    /*000041d4*/ MOV         R1, #51                     @ 0x33 
    /*000041d8*/ STR         R3, [SP] 
    /*000041dc*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000041e0*/ LDR         R3, [PC, #700]              @ 0x00000000000044a4 
    /*000041e4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000041e8*/ MOV         R0, #3 
    /*000041ec*/ BL          0x00000000000041EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000041f0*/ LDRD        R0, [R4, #16] 
    /*000041f4*/ MOV         R2, #16 
    /*000041f8*/ STR         R6, [SP] 
    /*000041fc*/ STR         R2, [SP, #16] 
    /*00004200*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004204*/ STRD        R0, [SP, #8] 
    /*00004208*/ MOVT        R3, #41011                  @ 0xa033 
    /*0000420c*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b70] 
    /*00004210*/ MOV         R1, #51                     @ 0x33 
    /*00004214*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b70] 
    /*00004218*/ MOV         R0, #3 
    /*0000421c*/ BL          0x000000000000421C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004220*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004224*/ MOVT        R0, #41011                  @ 0xa033 
    /*00004228*/ B           SVP_NNIE_CheckSrcBlob_x3f90
    SVP_NNIE_CheckSrcBlob_x422c:
    /*0000422c*/ MOV         R3, #512                    @ 0x200 
    /*00004230*/ MOV         R1, #51                     @ 0x33 
    /*00004234*/ STR         R3, [SP] 
    /*00004238*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*0000423c*/ LDR         R3, [PC, #608]              @ 0x00000000000044a4 
    /*00004240*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004244*/ MOV         R0, #3 
    /*00004248*/ BL          0x0000000000004248          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000424c*/ LDR         R1, [R4, #24] 
    /*00004250*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004254*/ MOVW        R2, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bb4] 
    /*00004258*/ STR         R6, [SP] 
    /*0000425c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004260*/ STR         R1, [SP, #4] 
    /*00004264*/ MOVT        R2, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bb4] 
    /*00004268*/ B           SVP_NNIE_CheckSrcBlob_x40f0
    SVP_NNIE_CheckSrcBlob_x426c:
    /*0000426c*/ MOVW        R3, #534                    @ 0x216 
    /*00004270*/ MOV         R1, #51                     @ 0x33 
    /*00004274*/ STR         R3, [SP] 
    /*00004278*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*0000427c*/ LDR         R3, [PC, #544]              @ 0x00000000000044a4 
    /*00004280*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004284*/ MOV         R0, #3 
    /*00004288*/ BL          0x0000000000004288          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000428c*/ LDR         R0, [R9, #24] 
    /*00004290*/ LDR         R2, [R4, #24] 
    /*00004294*/ MOV         R1, #0 
    /*00004298*/ STR         R6, [SP] 
    /*0000429c*/ MOVW        R3, #32771                  @ 0x8003 
    /*000042a0*/ STR         R0, [SP, #12] 
    /*000042a4*/ MOVT        R3, #41011                  @ 0xa033 
    /*000042a8*/ STR         R2, [SP, #4] 
    /*000042ac*/ MOV         R0, #3 
    /*000042b0*/ STR         R1, [SP, #8] 
    /*000042b4*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c20] 
    /*000042b8*/ MOV         R1, #51                     @ 0x33 
    /*000042bc*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c20] 
    /*000042c0*/ BL          0x00000000000042C0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000042c4*/ MOVW        R0, #32771                  @ 0x8003 
    /*000042c8*/ MOVT        R0, #41011                  @ 0xa033 
    /*000042cc*/ B           SVP_NNIE_CheckSrcBlob_x3f90
    SVP_NNIE_CheckSrcBlob_x42d0:
    /*000042d0*/ MOVW        R3, #433                    @ 0x1b1 
    /*000042d4*/ MOV         R1, #51                     @ 0x33 
    /*000042d8*/ STR         R3, [SP] 
    /*000042dc*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000042e0*/ LDR         R3, [PC, #444]              @ 0x00000000000044a4 
    /*000042e4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000042e8*/ MOV         R0, #3 
    /*000042ec*/ BL          0x00000000000042EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000042f0*/ LDR         R5, [SP, #28] 
    /*000042f4*/ MOVW        R3, #1692                   @ 0x69c 
    /*000042f8*/ MOV         R1, #52                     @ 0x34 
    /*000042fc*/ LDR         R0, [SP, #24] 
    /*00004300*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1860] 
    /*00004304*/ MUL         R3, R3, R5 
    /*00004308*/ LDR         LR, [R4, #40]               @ 0x28 
    /*0000430c*/ MLA         R1, R1, R6, R3 
    /*00004310*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1860] 
    /*00004314*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004318*/ MOVT        R3, #41011                  @ 0xa033 
    /*0000431c*/ ADD         FP, R0, R1 
    /*00004320*/ MOV         R1, #51                     @ 0x33 
    /*00004324*/ MOV         R0, #3 
    /*00004328*/ LDR         IP, [FP, #44]               @ 0x2c 
    /*0000432c*/ B           SVP_NNIE_CheckSrcBlob_x3f70
    SVP_NNIE_CheckSrcBlob_x4330:
    /*00004330*/ MOVW        R3, #438                    @ 0x1b6 
    /*00004334*/ MOV         R1, #51                     @ 0x33 
    /*00004338*/ STR         R3, [SP] 
    /*0000433c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004340*/ LDR         R3, [PC, #348]              @ 0x00000000000044a4 
    /*00004344*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004348*/ MOV         R0, #3 
    /*0000434c*/ BL          0x000000000000434C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004350*/ LDR         R5, [SP, #28] 
    /*00004354*/ MOVW        R3, #1692                   @ 0x69c 
    /*00004358*/ MOV         R1, #52                     @ 0x34 
    /*0000435c*/ LDR         LR, [R4, #32] 
    /*00004360*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18e0] 
    /*00004364*/ MUL         R3, R3, R5 
    /*00004368*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18e0] 
    /*0000436c*/ MLA         R1, R1, R6, R3 
    /*00004370*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004374*/ MOVT        R3, #41011                  @ 0xa033 
    SVP_NNIE_CheckSrcBlob_x4378:
    /*00004378*/ LDR         R0, [SP, #24] 
    /*0000437c*/ ADD         FP, R0, R1 
    /*00004380*/ MOV         R1, #51                     @ 0x33 
    /*00004384*/ MOV         R0, #3 
    /*00004388*/ LDR         IP, [FP, #36]               @ 0x24 
    /*0000438c*/ B           SVP_NNIE_CheckSrcBlob_x3f70
    SVP_NNIE_CheckSrcBlob_x4390:
    /*00004390*/ MOVW        R3, #443                    @ 0x1bb 
    /*00004394*/ MOV         R1, #51                     @ 0x33 
    /*00004398*/ STR         R3, [SP] 
    /*0000439c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000043a0*/ LDR         R3, [PC, #252]              @ 0x00000000000044a4 
    /*000043a4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000043a8*/ MOV         R0, #3 
    /*000043ac*/ BL          0x00000000000043AC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000043b0*/ LDR         R5, [SP, #28] 
    /*000043b4*/ MOVW        R3, #1692                   @ 0x69c 
    /*000043b8*/ MOV         R1, #52                     @ 0x34 
    /*000043bc*/ LDR         R0, [SP, #24] 
    /*000043c0*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1964] 
    /*000043c4*/ MUL         R3, R3, R5 
    /*000043c8*/ LDR         LR, [R4, #36]               @ 0x24 
    /*000043cc*/ MLA         R1, R1, R6, R3 
    /*000043d0*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1964] 
    /*000043d4*/ MOVW        R3, #32771                  @ 0x8003 
    /*000043d8*/ MOVT        R3, #41011                  @ 0xa033 
    /*000043dc*/ ADD         FP, R0, R1 
    /*000043e0*/ MOV         R1, #51                     @ 0x33 
    /*000043e4*/ MOV         R0, #3 
    /*000043e8*/ LDR         IP, [FP, #40]               @ 0x28 
    /*000043ec*/ B           SVP_NNIE_CheckSrcBlob_x3f70
    SVP_NNIE_CheckSrcBlob_x43f0:
    /*000043f0*/ MOVW        R3, #451                    @ 0x1c3 
    /*000043f4*/ MOV         R1, #51                     @ 0x33 
    /*000043f8*/ STR         R3, [SP] 
    /*000043fc*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004400*/ LDR         R3, [PC, #156]              @ 0x00000000000044a4 
    /*00004404*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004408*/ MOV         R0, #3 
    /*0000440c*/ BL          0x000000000000440C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004410*/ LDR         R5, [SP, #28] 
    /*00004414*/ MOVW        R3, #1692                   @ 0x69c 
    /*00004418*/ MOV         R1, #52                     @ 0x34 
    /*0000441c*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19ec] 
    /*00004420*/ LDR         LR, [R4, #32] 
    /*00004424*/ MUL         R3, R3, R5 
    /*00004428*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19ec] 
    /*0000442c*/ MLA         R1, R1, R6, R3 
    /*00004430*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004434*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004438*/ B           SVP_NNIE_CheckSrcBlob_x4378
    SVP_NNIE_CheckSrcBlob_x443c:
    /*0000443c*/ MOVW        R3, #455                    @ 0x1c7 
    /*00004440*/ MOV         R1, #51                     @ 0x33 
    /*00004444*/ STR         R3, [SP] 
    /*00004448*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*0000444c*/ LDR         R3, [PC, #80]               @ 0x00000000000044a4 
    /*00004450*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004454*/ MOV         R0, #3 
    /*00004458*/ BL          0x0000000000004458          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000445c*/ LDRD        R0, [R4, #40]               @ 0x28 
    /*00004460*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004464*/ STR         R6, [SP] 
    /*00004468*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a68] 
    /*0000446c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004470*/ STRD        R0, [SP, #8] 
    /*00004474*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a68] 
    /*00004478*/ MOV         R1, #51                     @ 0x33 
    /*0000447c*/ MOV         R0, #3 
    /*00004480*/ BL          0x0000000000004480          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004484*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004488*/ MOVT        R0, #41011                  @ 0xa033 
    /*0000448c*/ B           SVP_NNIE_CheckSrcBlob_x3f90
    SVP_NNIE_CheckSrcBlob_x4490:
    /*00004490*/ MOV         R4, R0 
    /*00004494*/ MOV         R6, #0 
    /*00004498*/ B           SVP_NNIE_CheckSrcBlob_x3f14
    SVP_NNIE_CheckSrcBlob_x449c:
    /*0000449c*/ MOV         R3, #524                    @ 0x20c 
    /*000044a0*/ B           SVP_NNIE_CheckSrcBlob_x3fa4
    /*000044a4*/ @           <UNDEFINED> instruction: 0x000001b4 
FUNC_END SVP_NNIE_CheckSrcBlob


FUNC_BEGIN SVP_NNIE_CheckDstBlob
    /*000044a8*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000044ac*/ SUBS        R8, R1, #0 
    /*000044b0*/ SUB         SP, SP, #36                 @ 0x24 
    /*000044b4*/ BEQ         SVP_NNIE_CheckDstBlob_x4784
    /*000044b8*/ MOV         R1, R3 
    /*000044bc*/ MOVW        R5, #1692                   @ 0x69c 
    /*000044c0*/ MLA         R5, R5, R1, R2 
    /*000044c4*/ STR         R3, [SP, #28] 
    /*000044c8*/ LDR         R3, [R0] 
    /*000044cc*/ MOV         R9, R0 
    /*000044d0*/ STR         R2, [SP, #24] 
    /*000044d4*/ LDR         R1, [R5, #864]              @ 0x360 
    /*000044d8*/ CMP         R1, R3 
    /*000044dc*/ MOVEQ       FP, #0 
    /*000044e0*/ MOVEQ       R4, R0 
    /*000044e4*/ MOVEQ       R6, FP 
    /*000044e8*/ BNE         SVP_NNIE_CheckDstBlob_x4b14
    SVP_NNIE_CheckDstBlob_x44ec:
    /*000044ec*/ CMP         R1, #5 
    /*000044f0*/ BEQ         SVP_NNIE_CheckDstBlob_x4700
    /*000044f4*/ LDR         R0, [R4, #40]               @ 0x28 
    /*000044f8*/ LDR         R2, [R5, #876]              @ 0x36c 
    /*000044fc*/ CMP         R0, R2 
    /*00004500*/ BNE         SVP_NNIE_CheckDstBlob_x49a8
    /*00004504*/ LDR         R2, [R4, #32] 
    /*00004508*/ LDR         R0, [R5, #868]              @ 0x364 
    /*0000450c*/ CMP         R2, R0 
    /*00004510*/ BNE         SVP_NNIE_CheckDstBlob_x4a08
    /*00004514*/ LDR         IP, [R4, #36]               @ 0x24 
    /*00004518*/ LDR         R0, [R5, #872]              @ 0x368 
    /*0000451c*/ CMP         IP, R0 
    /*00004520*/ BNE         SVP_NNIE_CheckDstBlob_x4a68
    /*00004524*/ SUB         R0, R1, #4 
    /*00004528*/ CMP         R1, #0 
    /*0000452c*/ CMPNE       R0, #1 
    /*00004530*/ MOVHI       R7, #1 
    /*00004534*/ MOVLS       R7, #4 
    SVP_NNIE_CheckDstBlob_x4538:
    /*00004538*/ LDR         R0, [R4, #4] 
    /*0000453c*/ CMP         R0, #0 
    /*00004540*/ BEQ         SVP_NNIE_CheckDstBlob_x478c
    /*00004544*/ TST         R0, #15 
    /*00004548*/ BNE         SVP_NNIE_CheckDstBlob_x47e4
    /*0000454c*/ MUL         R2, R2, R7 
    /*00004550*/ ADD         R2, R2, #15 
    /*00004554*/ BIC         R2, R2, #15 
    /*00004558*/ CMP         R0, R2 
    /*0000455c*/ BCC         SVP_NNIE_CheckDstBlob_x4840
    /*00004560*/ CMP         R1, #5 
    /*00004564*/ BEQ         SVP_NNIE_CheckDstBlob_x4598
    /*00004568*/ LDR         IP, [R4, #24] 
    /*0000456c*/ LDR         R7, [R4, #40]               @ 0x28 
    /*00004570*/ LDR         R1, [R4, #36]               @ 0x24 
    /*00004574*/ MUL         IP, IP, R0 
    /*00004578*/ LDR         SL, [R4, #16] 
    /*0000457c*/ MOV         R0, SL 
    /*00004580*/ MUL         R7, R7, IP 
    /*00004584*/ MUL         R7, R1, R7 
    /*00004588*/ MOV         R1, R7 
    /*0000458c*/ BL          0x000000000000458C          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00004590*/ CMP         R0, #0 
    /*00004594*/ BNE         SVP_NNIE_CheckDstBlob_x4718
    SVP_NNIE_CheckDstBlob_x4598:
    /*00004598*/ LDRD        R0, [R4, #16] 
    /*0000459c*/ MOV         R1, #0 
    /*000045a0*/ AND         R0, R0, #15 
    /*000045a4*/ ORRS        R3, R0, R1 
    /*000045a8*/ BNE         SVP_NNIE_CheckDstBlob_x48a8
    /*000045ac*/ LDR         R2, [R4, #24] 
    /*000045b0*/ CMP         R2, #0 
    /*000045b4*/ BEQ         SVP_NNIE_CheckDstBlob_x4904
    /*000045b8*/ LDR         R1, [R4] 
    /*000045bc*/ SUB         R0, R1, #1 
    /*000045c0*/ CMP         R0, #2 
    /*000045c4*/ BLS         SVP_NNIE_CheckDstBlob_x469c
    /*000045c8*/ CMP         R2, #256                    @ 0x100 
    /*000045cc*/ MOVLS       R0, #0 
    /*000045d0*/ MOVHI       R0, #1 
    /*000045d4*/ CMP         R1, #5 
    /*000045d8*/ MOVNE       R0, #0 
    /*000045dc*/ CMP         R0, #0 
    /*000045e0*/ BNE         SVP_NNIE_CheckDstBlob_x4b20
    SVP_NNIE_CheckDstBlob_x45e4:
    /*000045e4*/ CMP         R6, #0 
    /*000045e8*/ BEQ         SVP_NNIE_CheckDstBlob_x45f4
    /*000045ec*/ CMP         R2, FP 
    /*000045f0*/ BNE         SVP_NNIE_CheckDstBlob_x4944
    SVP_NNIE_CheckDstBlob_x45f4:
    /*000045f4*/ ADD         R6, R6, #1 
    /*000045f8*/ CMP         R8, R6 
    /*000045fc*/ BEQ         SVP_NNIE_CheckDstBlob_x4784
    /*00004600*/ ADD         R5, R5, #52                 @ 0x34 
    /*00004604*/ LDR         R1, [R4, #48]!              @ 0x30 
    /*00004608*/ MOV         FP, R2 
    /*0000460c*/ LDR         R2, [R5, #864]              @ 0x360 
    /*00004610*/ CMP         R1, R2 
    /*00004614*/ BEQ         SVP_NNIE_CheckDstBlob_x44ec
    SVP_NNIE_CheckDstBlob_x4618:
    /*00004618*/ MOVW        R3, #571                    @ 0x23b 
    /*0000461c*/ MOV         R1, #51                     @ 0x33 
    /*00004620*/ STR         R3, [SP] 
    /*00004624*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004628*/ LDR         R3, [PC, #1272]             @ 0x0000000000004b28 
    /*0000462c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004630*/ MOV         R0, #3 
    /*00004634*/ BL          0x0000000000004634          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004638*/ LDR         R5, [SP, #28] 
    /*0000463c*/ MOVW        R1, #1692                   @ 0x69c 
    /*00004640*/ LDR         R0, [SP, #24] 
    /*00004644*/ LDR         LR, [R4] 
    /*00004648*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1cbc] 
    /*0000464c*/ MUL         R3, R1, R5 
    /*00004650*/ MOV         R1, #52                     @ 0x34 
    /*00004654*/ MLA         R1, R1, R6, R3 
    /*00004658*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1cbc] 
    /*0000465c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004660*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004664*/ ADD         FP, R0, R1 
    /*00004668*/ MOV         R1, #51                     @ 0x33 
    /*0000466c*/ MOV         R0, #3 
    /*00004670*/ LDR         IP, [FP, #864]              @ 0x360 
    SVP_NNIE_CheckDstBlob_x4674:
    /*00004674*/ STR         LR, [SP, #4] 
    /*00004678*/ STR         R6, [SP, #12] 
    /*0000467c*/ STR         IP, [SP, #16] 
    /*00004680*/ STR         R5, [SP, #8] 
    /*00004684*/ STR         R6, [SP] 
    /*00004688*/ BL          0x0000000000004688          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000468c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004690*/ MOVT        R0, #41011                  @ 0xa033 
    SVP_NNIE_CheckDstBlob_x4694:
    /*00004694*/ ADD         SP, SP, #36                 @ 0x24 
    /*00004698*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_CheckDstBlob_x469c:
    /*0000469c*/ CMP         R2, #256                    @ 0x100 
    /*000046a0*/ BLS         SVP_NNIE_CheckDstBlob_x45e4
    /*000046a4*/ MOVW        R3, #646                    @ 0x286 
    SVP_NNIE_CheckDstBlob_x46a8:
    /*000046a8*/ STR         R3, [SP] 
    /*000046ac*/ MOV         R1, #51                     @ 0x33 
    /*000046b0*/ LDR         R3, [PC, #1136]             @ 0x0000000000004b28 
    /*000046b4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000046b8*/ MOV         R0, #3 
    /*000046bc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000046c0*/ BL          0x00000000000046C0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000046c4*/ LDR         R2, [R4, #24] 
    /*000046c8*/ MOV         R1, #256                    @ 0x100 
    /*000046cc*/ STR         R6, [SP] 
    /*000046d0*/ STR         R1, [SP, #8] 
    /*000046d4*/ MOVW        R3, #32771                  @ 0x8003 
    /*000046d8*/ STR         R2, [SP, #4] 
    /*000046dc*/ MOVT        R3, #41011                  @ 0xa033 
    /*000046e0*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1be4] 
    /*000046e4*/ MOV         R1, #51                     @ 0x33 
    /*000046e8*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1be4] 
    /*000046ec*/ MOV         R0, #3 
    /*000046f0*/ BL          0x00000000000046F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000046f4*/ MOVW        R0, #32771                  @ 0x8003 
    /*000046f8*/ MOVT        R0, #41011                  @ 0xa033 
    /*000046fc*/ B           SVP_NNIE_CheckDstBlob_x4694
    SVP_NNIE_CheckDstBlob_x4700:
    /*00004700*/ LDR         R2, [R5, #868]              @ 0x364 
    /*00004704*/ LDR         R0, [R4, #32] 
    /*00004708*/ CMP         R0, R2 
    /*0000470c*/ BNE         SVP_NNIE_CheckDstBlob_x4ac8
    /*00004710*/ MOV         R7, #4 
    /*00004714*/ B           SVP_NNIE_CheckDstBlob_x4538
    SVP_NNIE_CheckDstBlob_x4718:
    /*00004718*/ MOV         R0, SL 
    /*0000471c*/ MOV         R1, R7 
    /*00004720*/ BL          0x0000000000004720          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*00004724*/ CMP         R0, #0 
    /*00004728*/ BEQ         SVP_NNIE_CheckDstBlob_x4598
    /*0000472c*/ MOV         R3, #632                    @ 0x278 
    /*00004730*/ MOV         R1, #51                     @ 0x33 
    /*00004734*/ STR         R3, [SP] 
    /*00004738*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*0000473c*/ LDR         R3, [PC, #996]              @ 0x0000000000004b28 
    /*00004740*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004744*/ MOV         R0, #3 
    /*00004748*/ BL          0x0000000000004748          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000474c*/ LDRD        R0, [R4, #16] 
    /*00004750*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004754*/ STR         R7, [SP, #16] 
    /*00004758*/ STR         R6, [SP] 
    /*0000475c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004760*/ STRD        R0, [SP, #8] 
    /*00004764*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c6c] 
    /*00004768*/ MOV         R1, #51                     @ 0x33 
    /*0000476c*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c6c] 
    /*00004770*/ MOV         R0, #3 
    /*00004774*/ BL          0x0000000000004774          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004778*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000477c*/ MOVT        R0, #41011                  @ 0xa033 
    /*00004780*/ B           SVP_NNIE_CheckDstBlob_x4694
    SVP_NNIE_CheckDstBlob_x4784:
    /*00004784*/ MOV         R0, #0 
    /*00004788*/ B           SVP_NNIE_CheckDstBlob_x4694
    SVP_NNIE_CheckDstBlob_x478c:
    /*0000478c*/ MOVW        R3, #611                    @ 0x263 
    /*00004790*/ MOV         R1, #51                     @ 0x33 
    /*00004794*/ STR         R3, [SP] 
    /*00004798*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*0000479c*/ LDR         R3, [PC, #900]              @ 0x0000000000004b28 
    /*000047a0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000047a4*/ MOV         R0, #3 
    /*000047a8*/ BL          0x00000000000047A8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000047ac*/ LDR         R1, [R4, #4] 
    /*000047b0*/ MOVW        R3, #32771                  @ 0x8003 
    /*000047b4*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ab0] 
    /*000047b8*/ MOVT        R3, #41011                  @ 0xa033 
    /*000047bc*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ab0] 
    /*000047c0*/ STR         R6, [SP] 
    /*000047c4*/ STR         R1, [SP, #4] 
    SVP_NNIE_CheckDstBlob_x47c8:
    /*000047c8*/ MOV         R1, #51                     @ 0x33 
    /*000047cc*/ MOV         R0, #3 
    /*000047d0*/ BL          0x00000000000047D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000047d4*/ MOVW        R0, #32771                  @ 0x8003 
    /*000047d8*/ MOVT        R0, #41011                  @ 0xa033 
    /*000047dc*/ ADD         SP, SP, #36                 @ 0x24 
    /*000047e0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_CheckDstBlob_x47e4:
    /*000047e4*/ MOVW        R3, #614                    @ 0x266 
    /*000047e8*/ MOV         R1, #51                     @ 0x33 
    /*000047ec*/ STR         R3, [SP] 
    /*000047f0*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000047f4*/ LDR         R3, [PC, #812]              @ 0x0000000000004b28 
    /*000047f8*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000047fc*/ MOV         R0, #3 
    /*00004800*/ BL          0x0000000000004800          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004804*/ LDR         R2, [R4, #4] 
    /*00004808*/ MOV         R1, #16 
    /*0000480c*/ STR         R6, [SP] 
    /*00004810*/ STR         R1, [SP, #8] 
    /*00004814*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004818*/ STR         R2, [SP, #4] 
    /*0000481c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004820*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ae4] 
    /*00004824*/ MOV         R1, #51                     @ 0x33 
    /*00004828*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ae4] 
    /*0000482c*/ MOV         R0, #3 
    /*00004830*/ BL          0x0000000000004830          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004834*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004838*/ MOVT        R0, #41011                  @ 0xa033 
    /*0000483c*/ B           SVP_NNIE_CheckDstBlob_x4694
    SVP_NNIE_CheckDstBlob_x4840:
    /*00004840*/ MOVW        R3, #617                    @ 0x269 
    /*00004844*/ MOV         R1, #51                     @ 0x33 
    /*00004848*/ STR         R3, [SP] 
    /*0000484c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004850*/ LDR         R3, [PC, #720]              @ 0x0000000000004b28 
    /*00004854*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004858*/ MOV         R0, #3 
    /*0000485c*/ BL          0x000000000000485C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004860*/ LDR         IP, [R4, #32] 
    /*00004864*/ LDR         R2, [R4, #4] 
    /*00004868*/ MOVW        R3, #32771                  @ 0x8003 
    /*0000486c*/ STR         R6, [SP] 
    /*00004870*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004874*/ MUL         IP, IP, R7 
    /*00004878*/ STR         R2, [SP, #4] 
    /*0000487c*/ MOV         R1, #51                     @ 0x33 
    /*00004880*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b24] 
    /*00004884*/ MOV         R0, #3 
    /*00004888*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b24] 
    /*0000488c*/ ADD         IP, IP, #15 
    /*00004890*/ BIC         IP, IP, #15 
    /*00004894*/ STR         IP, [SP, #8] 
    /*00004898*/ BL          0x0000000000004898          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000489c*/ MOVW        R0, #32771                  @ 0x8003 
    /*000048a0*/ MOVT        R0, #41011                  @ 0xa033 
    /*000048a4*/ B           SVP_NNIE_CheckDstBlob_x4694
    SVP_NNIE_CheckDstBlob_x48a8:
    /*000048a8*/ MOV         R3, #636                    @ 0x27c 
    /*000048ac*/ MOV         R1, #51                     @ 0x33 
    /*000048b0*/ STR         R3, [SP] 
    /*000048b4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000048b8*/ LDR         R3, [PC, #616]              @ 0x0000000000004b28 
    /*000048bc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000048c0*/ MOV         R0, #3 
    /*000048c4*/ BL          0x00000000000048C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000048c8*/ LDRD        R0, [R4, #16] 
    /*000048cc*/ MOV         R2, #16 
    /*000048d0*/ STR         R6, [SP] 
    /*000048d4*/ STR         R2, [SP, #16] 
    /*000048d8*/ MOVW        R3, #32771                  @ 0x8003 
    /*000048dc*/ STRD        R0, [SP, #8] 
    /*000048e0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000048e4*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b70] 
    /*000048e8*/ MOV         R1, #51                     @ 0x33 
    /*000048ec*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b70] 
    /*000048f0*/ MOV         R0, #3 
    /*000048f4*/ BL          0x00000000000048F4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000048f8*/ MOVW        R0, #32771                  @ 0x8003 
    /*000048fc*/ MOVT        R0, #41011                  @ 0xa033 
    /*00004900*/ B           SVP_NNIE_CheckDstBlob_x4694
    SVP_NNIE_CheckDstBlob_x4904:
    /*00004904*/ MOVW        R3, #641                    @ 0x281 
    /*00004908*/ MOV         R1, #51                     @ 0x33 
    /*0000490c*/ STR         R3, [SP] 
    /*00004910*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004914*/ LDR         R3, [PC, #524]              @ 0x0000000000004b28 
    /*00004918*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*0000491c*/ MOV         R0, #3 
    /*00004920*/ BL          0x0000000000004920          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004924*/ LDR         R1, [R4, #24] 
    /*00004928*/ MOVW        R3, #32771                  @ 0x8003 
    /*0000492c*/ MOVW        R2, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eac] 
    /*00004930*/ STR         R6, [SP] 
    /*00004934*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004938*/ STR         R1, [SP, #4] 
    /*0000493c*/ MOVT        R2, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eac] 
    /*00004940*/ B           SVP_NNIE_CheckDstBlob_x47c8
    SVP_NNIE_CheckDstBlob_x4944:
    /*00004944*/ MOVW        R3, #662                    @ 0x296 
    /*00004948*/ MOV         R1, #51                     @ 0x33 
    /*0000494c*/ STR         R3, [SP] 
    /*00004950*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004954*/ LDR         R3, [PC, #460]              @ 0x0000000000004b28 
    /*00004958*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*0000495c*/ MOV         R0, #3 
    /*00004960*/ BL          0x0000000000004960          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004964*/ LDR         R0, [R9, #24] 
    /*00004968*/ LDR         R2, [R4, #24] 
    /*0000496c*/ MOV         R1, #0 
    /*00004970*/ STR         R6, [SP] 
    /*00004974*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004978*/ STR         R0, [SP, #12] 
    /*0000497c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004980*/ STR         R2, [SP, #4] 
    /*00004984*/ MOV         R0, #3 
    /*00004988*/ STR         R1, [SP, #8] 
    /*0000498c*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c20] 
    /*00004990*/ MOV         R1, #51                     @ 0x33 
    /*00004994*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c20] 
    /*00004998*/ BL          0x0000000000004998          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000499c*/ MOVW        R0, #32771                  @ 0x8003 
    /*000049a0*/ MOVT        R0, #41011                  @ 0xa033 
    /*000049a4*/ B           SVP_NNIE_CheckDstBlob_x4694
    SVP_NNIE_CheckDstBlob_x49a8:
    /*000049a8*/ MOVW        R3, #578                    @ 0x242 
    /*000049ac*/ MOV         R1, #51                     @ 0x33 
    /*000049b0*/ STR         R3, [SP] 
    /*000049b4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000049b8*/ LDR         R3, [PC, #360]              @ 0x0000000000004b28 
    /*000049bc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000049c0*/ MOV         R0, #3 
    /*000049c4*/ BL          0x00000000000049C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000049c8*/ LDR         R5, [SP, #28] 
    /*000049cc*/ MOVW        R3, #1692                   @ 0x69c 
    /*000049d0*/ MOV         R1, #52                     @ 0x34 
    /*000049d4*/ LDR         R0, [SP, #24] 
    /*000049d8*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d20] 
    /*000049dc*/ MUL         R3, R3, R5 
    /*000049e0*/ LDR         LR, [R4, #40]               @ 0x28 
    /*000049e4*/ MLA         R1, R1, R6, R3 
    /*000049e8*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d20] 
    /*000049ec*/ MOVW        R3, #32771                  @ 0x8003 
    /*000049f0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000049f4*/ ADD         FP, R0, R1 
    /*000049f8*/ MOV         R1, #51                     @ 0x33 
    /*000049fc*/ MOV         R0, #3 
    /*00004a00*/ LDR         IP, [FP, #876]              @ 0x36c 
    /*00004a04*/ B           SVP_NNIE_CheckDstBlob_x4674
    SVP_NNIE_CheckDstBlob_x4a08:
    /*00004a08*/ MOVW        R3, #583                    @ 0x247 
    /*00004a0c*/ MOV         R1, #51                     @ 0x33 
    /*00004a10*/ STR         R3, [SP] 
    /*00004a14*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004a18*/ LDR         R3, [PC, #264]              @ 0x0000000000004b28 
    /*00004a1c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004a20*/ MOV         R0, #3 
    /*00004a24*/ BL          0x0000000000004A24          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004a28*/ LDR         R5, [SP, #28] 
    /*00004a2c*/ MOVW        R3, #1692                   @ 0x69c 
    /*00004a30*/ MOV         R1, #52                     @ 0x34 
    /*00004a34*/ LDR         LR, [R4, #32] 
    /*00004a38*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1da0] 
    /*00004a3c*/ MUL         R3, R3, R5 
    /*00004a40*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1da0] 
    /*00004a44*/ MLA         R1, R1, R6, R3 
    /*00004a48*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004a4c*/ MOVT        R3, #41011                  @ 0xa033 
    SVP_NNIE_CheckDstBlob_x4a50:
    /*00004a50*/ LDR         R0, [SP, #24] 
    /*00004a54*/ ADD         FP, R0, R1 
    /*00004a58*/ MOV         R1, #51                     @ 0x33 
    /*00004a5c*/ MOV         R0, #3 
    /*00004a60*/ LDR         IP, [FP, #868]              @ 0x364 
    /*00004a64*/ B           SVP_NNIE_CheckDstBlob_x4674
    SVP_NNIE_CheckDstBlob_x4a68:
    /*00004a68*/ MOV         R3, #588                    @ 0x24c 
    /*00004a6c*/ MOV         R1, #51                     @ 0x33 
    /*00004a70*/ STR         R3, [SP] 
    /*00004a74*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004a78*/ LDR         R3, [PC, #168]              @ 0x0000000000004b28 
    /*00004a7c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004a80*/ MOV         R0, #3 
    /*00004a84*/ BL          0x0000000000004A84          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004a88*/ LDR         R5, [SP, #28] 
    /*00004a8c*/ MOVW        R3, #1692                   @ 0x69c 
    /*00004a90*/ MOV         R1, #52                     @ 0x34 
    /*00004a94*/ LDR         R0, [SP, #24] 
    /*00004a98*/ MOVW        R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e24] 
    /*00004a9c*/ MUL         R3, R3, R5 
    /*00004aa0*/ LDR         LR, [R4, #36]               @ 0x24 
    /*00004aa4*/ MLA         R1, R1, R6, R3 
    /*00004aa8*/ MOVT        R2, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e24] 
    /*00004aac*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004ab0*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004ab4*/ ADD         FP, R0, R1 
    /*00004ab8*/ MOV         R1, #51                     @ 0x33 
    /*00004abc*/ MOV         R0, #3 
    /*00004ac0*/ LDR         IP, [FP, #872]              @ 0x368 
    /*00004ac4*/ B           SVP_NNIE_CheckDstBlob_x4674
    SVP_NNIE_CheckDstBlob_x4ac8:
    /*00004ac8*/ MOVW        R3, #595                    @ 0x253 
    /*00004acc*/ MOV         R1, #51                     @ 0x33 
    /*00004ad0*/ STR         R3, [SP] 
    /*00004ad4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004ad8*/ LDR         R3, [PC, #72]               @ 0x0000000000004b28 
    /*00004adc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004ae0*/ MOV         R0, #3 
    /*00004ae4*/ BL          0x0000000000004AE4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004ae8*/ LDR         R5, [SP, #28] 
    /*00004aec*/ MOVW        R3, #1692                   @ 0x69c 
    /*00004af0*/ MOV         R1, #52                     @ 0x34 
    /*00004af4*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19ec] 
    /*00004af8*/ LDR         LR, [R4, #32] 
    /*00004afc*/ MUL         R3, R3, R5 
    /*00004b00*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19ec] 
    /*00004b04*/ MLA         R1, R1, R6, R3 
    /*00004b08*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004b0c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004b10*/ B           SVP_NNIE_CheckDstBlob_x4a50
    SVP_NNIE_CheckDstBlob_x4b14:
    /*00004b14*/ MOV         R4, R0 
    /*00004b18*/ MOV         R6, #0 
    /*00004b1c*/ B           SVP_NNIE_CheckDstBlob_x4618
    SVP_NNIE_CheckDstBlob_x4b20:
    /*00004b20*/ MOV         R3, #652                    @ 0x28c 
    /*00004b24*/ B           SVP_NNIE_CheckDstBlob_x46a8
    /*00004b28*/ ANDEQ       R0, R0, IP, ASR #3 
FUNC_END SVP_NNIE_CheckDstBlob


FUNC_BEGIN SVP_NNIE_CheckModel
    /*00004b2c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00004b30*/ MOV         R6, R0 
    /*00004b34*/ LDR         R3, [R0, #4] 
    /*00004b38*/ SUB         SP, SP, #36                 @ 0x24 
    /*00004b3c*/ CMP         R3, #0 
    /*00004b40*/ BEQ         SVP_NNIE_CheckModel_x5b10
    /*00004b44*/ LDR         R3, [R0] 
    /*00004b48*/ CMP         R3, #0 
    /*00004b4c*/ BNE         SVP_NNIE_CheckModel_x5b50
    /*00004b50*/ LDR         R3, [R0, #8] 
    /*00004b54*/ SUB         R3, R3, #1 
    /*00004b58*/ CMP         R3, #7 
    /*00004b5c*/ BHI         SVP_NNIE_CheckModel_x5c00
    /*00004b60*/ MOVW        R8, #13968                  @ 0x3690 
    /*00004b64*/ MOV         R3, #0 
    /*00004b68*/ LDRD        R4, [R0, R8] 
    /*00004b6c*/ AND         R2, R4, #15 
    /*00004b70*/ ORRS        R3, R2, R3 
    /*00004b74*/ BNE         SVP_NNIE_CheckModel_x5ba8
    /*00004b78*/ ADD         R9, R0, #12288              @ 0x3000 
    /*00004b7c*/ LDR         R7, [R9, #1696]             @ 0x6a0 
    /*00004b80*/ CMP         R7, #0 
    /*00004b84*/ BEQ         SVP_NNIE_CheckModel_x52e8
    /*00004b88*/ MOV         R1, R7 
    /*00004b8c*/ MOV         R0, R4 
    /*00004b90*/ BL          0x0000000000004B90          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00004b94*/ CMP         R0, #0 
    /*00004b98*/ BNE         SVP_NNIE_CheckModel_x4e74
    SVP_NNIE_CheckModel_x4b9c:
    /*00004b9c*/ LDR         R0, [R6, #8] 
    /*00004ba0*/ CMP         R0, #0 
    /*00004ba4*/ BEQ         SVP_NNIE_CheckModel_x4e08
    /*00004ba8*/ LDR         R7, [R6, #12] 
    /*00004bac*/ CMP         R7, #2 
    /*00004bb0*/ BHI         SVP_NNIE_CheckModel_x5a60
    /*00004bb4*/ LDRH        R2, [R6, #16] 
    /*00004bb8*/ SUB         R3, R2, #1 
    /*00004bbc*/ CMP         R3, #15 
    /*00004bc0*/ BHI         SVP_NNIE_CheckModel_x5ab8
    /*00004bc4*/ LDRH        R9, [R6, #18] 
    /*00004bc8*/ SUB         R3, R9, #1 
    /*00004bcc*/ CMP         R3, #15 
    /*00004bd0*/ MOVLS       LR, R6 
    /*00004bd4*/ MOVLS       R8, #0 
    /*00004bd8*/ MOVWLS      SL, #13556                  @ 0x34f4 
    /*00004bdc*/ BHI         SVP_NNIE_CheckModel_x5b48
    SVP_NNIE_CheckModel_x4be0:
    /*00004be0*/ CMP         R7, #2 
    /*00004be4*/ BEQ         SVP_NNIE_CheckModel_x4ee0
    SVP_NNIE_CheckModel_x4be8:
    /*00004be8*/ LDR         R3, [LR, #24] 
    /*00004bec*/ CMP         R3, #0 
    /*00004bf0*/ BEQ         SVP_NNIE_CheckModel_x5500
    /*00004bf4*/ LDR         R3, [LR, #28] 
    /*00004bf8*/ CMP         R3, #0 
    /*00004bfc*/ BEQ         SVP_NNIE_CheckModel_x5500
    /*00004c00*/ LDR         R4, [LR, #32] 
    /*00004c04*/ CMP         R4, #5 
    /*00004c08*/ BHI         SVP_NNIE_CheckModel_x50c8
    /*00004c0c*/ MOV         R3, LR 
    /*00004c10*/ MOV         R5, #0 
    /*00004c14*/ B           SVP_NNIE_CheckModel_x4c68
    SVP_NNIE_CheckModel_x4c18:
    /*00004c18*/ LDR         R1, [R3, #44]               @ 0x2c 
    /*00004c1c*/ CMP         R1, #0 
    /*00004c20*/ BEQ         SVP_NNIE_CheckModel_x514c
    /*00004c24*/ CMP         R4, #4 
    /*00004c28*/ BEQ         SVP_NNIE_CheckModel_x4d6c
    /*00004c2c*/ CMP         R4, #0 
    /*00004c30*/ LDR         R1, [R3, #36]               @ 0x24 
    /*00004c34*/ BNE         SVP_NNIE_CheckModel_x4ce4
    /*00004c38*/ CMP         R1, #0 
    /*00004c3c*/ BEQ         SVP_NNIE_CheckModel_x5408
    /*00004c40*/ LDR         R1, [R3, #40]               @ 0x28 
    /*00004c44*/ CMP         R1, #0 
    /*00004c48*/ BEQ         SVP_NNIE_CheckModel_x54ac
    SVP_NNIE_CheckModel_x4c4c:
    /*00004c4c*/ ADD         R5, R5, #1 
    /*00004c50*/ CMP         R5, R2 
    /*00004c54*/ BEQ         SVP_NNIE_CheckModel_x4f5c
    /*00004c58*/ ADD         R3, R3, #52                 @ 0x34 
    /*00004c5c*/ LDR         R4, [R3, #32] 
    /*00004c60*/ CMP         R4, #5 
    /*00004c64*/ BHI         SVP_NNIE_CheckModel_x50cc
    SVP_NNIE_CheckModel_x4c68:
    /*00004c68*/ CMP         R4, #5 
    /*00004c6c*/ BNE         SVP_NNIE_CheckModel_x4c18
    /*00004c70*/ LDR         R1, [R3, #36]               @ 0x24 
    /*00004c74*/ CMP         R1, #0 
    /*00004c78*/ BEQ         SVP_NNIE_CheckModel_x5298
    /*00004c7c*/ CMP         R5, #0 
    /*00004c80*/ BNE         SVP_NNIE_CheckModel_x5338
    /*00004c84*/ CMP         R7, #2 
    /*00004c88*/ BEQ         SVP_NNIE_CheckModel_x4c4c
    /*00004c8c*/ MOV         R3, #201                    @ 0xc9 
    /*00004c90*/ MOV         R1, #51                     @ 0x33 
    /*00004c94*/ STR         R3, [SP] 
    /*00004c98*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004c9c*/ LDR         R3, [PC, #4028]             @ 0x0000000000005c60 
    /*00004ca0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004ca4*/ MOV         R0, #3 
    /*00004ca8*/ BL          0x0000000000004CA8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004cac*/ MOVW        R3, #1692                   @ 0x69c 
    /*00004cb0*/ MLA         R6, R3, R8, R6 
    /*00004cb4*/ STR         R8, [SP, #12] 
    /*00004cb8*/ MOV         R3, #2 
    /*00004cbc*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x264c] 
    /*00004cc0*/ STR         R3, [SP, #16] 
    /*00004cc4*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x264c] 
    /*00004cc8*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004ccc*/ MOV         R1, #51                     @ 0x33 
    /*00004cd0*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004cd4*/ MOV         R0, #3 
    /*00004cd8*/ LDR         IP, [R6, #32] 
    /*00004cdc*/ STR         R5, [SP, #4] 
    /*00004ce0*/ B           SVP_NNIE_CheckModel_x5130
    SVP_NNIE_CheckModel_x4ce4:
    /*00004ce4*/ SUB         IP, R1, #16 
    /*00004ce8*/ CMP         IP, #4080                   @ 0xff0 
    /*00004cec*/ BHI         SVP_NNIE_CheckModel_x53a8
    /*00004cf0*/ LDR         IP, [R3, #40]               @ 0x28 
    /*00004cf4*/ SUB         FP, IP, #16 
    /*00004cf8*/ CMP         FP, #4080                   @ 0xff0 
    /*00004cfc*/ BHI         SVP_NNIE_CheckModel_x544c
    /*00004d00*/ CMP         R4, #2 
    /*00004d04*/ BNE         SVP_NNIE_CheckModel_x4e10
    /*00004d08*/ TST         R1, #1 
    /*00004d0c*/ BNE         SVP_NNIE_CheckModel_x563c
    /*00004d10*/ TST         IP, #1 
    /*00004d14*/ BEQ         SVP_NNIE_CheckModel_x4c4c
    /*00004d18*/ MOV         R3, #177                    @ 0xb1 
    /*00004d1c*/ MOV         R1, #51                     @ 0x33 
    /*00004d20*/ STR         R3, [SP] 
    /*00004d24*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004d28*/ LDR         R3, [PC, #3888]             @ 0x0000000000005c60 
    /*00004d2c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004d30*/ MOV         R0, #3 
    /*00004d34*/ BL          0x0000000000004D34          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004d38*/ MOVW        R3, #1692                   @ 0x69c 
    /*00004d3c*/ MUL         R3, R3, R8 
    /*00004d40*/ MOV         R0, #52                     @ 0x34 
    /*00004d44*/ MLA         R0, R0, R5, R3 
    /*00004d48*/ MOVW        R2, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2528] 
    /*00004d4c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004d50*/ MOVT        R2, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2528] 
    /*00004d54*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004d58*/ MOV         R1, #51                     @ 0x33 
    /*00004d5c*/ ADD         R6, R6, R0 
    /*00004d60*/ MOV         R0, #3 
    /*00004d64*/ LDR         IP, [R6, #40]               @ 0x28 
    /*00004d68*/ B           SVP_NNIE_CheckModel_x519c
    SVP_NNIE_CheckModel_x4d6c:
    /*00004d6c*/ LDR         IP, [R3, #36]               @ 0x24 
    /*00004d70*/ CMP         IP, #0 
    /*00004d74*/ BEQ         SVP_NNIE_CheckModel_x5564
    /*00004d78*/ LDR         IP, [R3, #40]               @ 0x28 
    /*00004d7c*/ CMP         IP, #1 
    /*00004d80*/ BNE         SVP_NNIE_CheckModel_x55d4
    /*00004d84*/ CMP         R1, #1 
    /*00004d88*/ BEQ         SVP_NNIE_CheckModel_x4c4c
    /*00004d8c*/ MOV         R3, #139                    @ 0x8b 
    /*00004d90*/ MOV         R1, #51                     @ 0x33 
    /*00004d94*/ STR         R3, [SP] 
    /*00004d98*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004d9c*/ LDR         R3, [PC, #3772]             @ 0x0000000000005c60 
    /*00004da0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004da4*/ MOV         R0, #3 
    /*00004da8*/ BL          0x0000000000004DA8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004dac*/ MOVW        R3, #1692                   @ 0x69c 
    /*00004db0*/ MUL         R3, R3, R8 
    /*00004db4*/ MOV         R1, #52                     @ 0x34 
    /*00004db8*/ MLA         R1, R1, R5, R3 
    /*00004dbc*/ STR         R4, [SP, #24] 
    /*00004dc0*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004dc4*/ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2324] 
    /*00004dc8*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004dcc*/ MOVT        R2, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2324] 
    /*00004dd0*/ ADD         R6, R6, R1 
    /*00004dd4*/ MOV         R0, #3 
    /*00004dd8*/ MOV         R1, #51                     @ 0x33 
    /*00004ddc*/ LDR         IP, [R6, #32] 
    /*00004de0*/ STR         R5, [SP, #16] 
    /*00004de4*/ STR         R8, [SP, #12] 
    /*00004de8*/ STR         IP, [SP, #20] 
    /*00004dec*/ LDR         IP, [R6, #44]               @ 0x2c 
    SVP_NNIE_CheckModel_x4df0:
    /*00004df0*/ STR         R5, [SP, #4] 
    SVP_NNIE_CheckModel_x4df4:
    /*00004df4*/ STR         R8, [SP] 
    /*00004df8*/ STR         IP, [SP, #8] 
    /*00004dfc*/ BL          0x0000000000004DFC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004e00*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004e04*/ MOVT        R0, #41011                  @ 0xa033 
    SVP_NNIE_CheckModel_x4e08:
    /*00004e08*/ ADD         SP, SP, #36                 @ 0x24 
    /*00004e0c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_CheckModel_x4e10:
    /*00004e10*/ CMP         R4, #3 
    /*00004e14*/ BNE         SVP_NNIE_CheckModel_x4c4c
    /*00004e18*/ TST         R1, #1 
    /*00004e1c*/ BEQ         SVP_NNIE_CheckModel_x4c4c
    /*00004e20*/ MOV         R3, #186                    @ 0xba 
    /*00004e24*/ MOV         R1, #51                     @ 0x33 
    /*00004e28*/ STR         R3, [SP] 
    /*00004e2c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004e30*/ LDR         R3, [PC, #3624]             @ 0x0000000000005c60 
    /*00004e34*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004e38*/ MOV         R0, R4 
    /*00004e3c*/ BL          0x0000000000004E3C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004e40*/ MOVW        R3, #1692                   @ 0x69c 
    /*00004e44*/ MUL         R3, R3, R8 
    /*00004e48*/ MOV         R1, #52                     @ 0x34 
    /*00004e4c*/ MLA         R1, R1, R5, R3 
    /*00004e50*/ MOVW        R2, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24c4] 
    /*00004e54*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004e58*/ MOVT        R2, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24c4] 
    /*00004e5c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004e60*/ MOV         R0, R4 
    /*00004e64*/ ADD         R6, R6, R1 
    /*00004e68*/ MOV         R1, #51                     @ 0x33 
    SVP_NNIE_CheckModel_x4e6c:
    /*00004e6c*/ LDR         IP, [R6, #36]               @ 0x24 
    /*00004e70*/ B           SVP_NNIE_CheckModel_x519c
    SVP_NNIE_CheckModel_x4e74:
    /*00004e74*/ MOV         R1, R7 
    /*00004e78*/ MOV         R0, R4 
    /*00004e7c*/ BL          0x0000000000004E7C          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*00004e80*/ CMP         R0, #0 
    /*00004e84*/ BEQ         SVP_NNIE_CheckModel_x4b9c
    /*00004e88*/ MOV         R3, #77                     @ 0x4d 
    /*00004e8c*/ MOV         R1, #51                     @ 0x33 
    /*00004e90*/ STR         R3, [SP] 
    /*00004e94*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004e98*/ LDR         R3, [PC, #3520]             @ 0x0000000000005c60 
    /*00004e9c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004ea0*/ MOV         R0, #3 
    /*00004ea4*/ BL          0x0000000000004EA4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004ea8*/ LDRD        R0, [R6, R8] 
    /*00004eac*/ LDR         IP, [R9, #1696]             @ 0x6a0 
    /*00004eb0*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004eb4*/ MOVW        R2, #0                      @ .LC75 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x306c] 
    /*00004eb8*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004ebc*/ STRD        R0, [SP] 
    /*00004ec0*/ MOVT        R2, #0                      @ .LC75 [R_ARM_MOVT_ABS: .rodata.str1.4+0x306c] 
    /*00004ec4*/ STR         IP, [SP, #8] 
    /*00004ec8*/ MOV         R1, #51                     @ 0x33 
    /*00004ecc*/ MOV         R0, #3 
    /*00004ed0*/ BL          0x0000000000004ED0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004ed4*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004ed8*/ MOVT        R0, #41011                  @ 0xa033 
    /*00004edc*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x4ee0:
    /*00004ee0*/ LDRH        R3, [LR, #22] 
    /*00004ee4*/ SUB         R3, R3, #1 
    /*00004ee8*/ CMP         R3, #1024                   @ 0x400 
    /*00004eec*/ BCC         SVP_NNIE_CheckModel_x4be8
    /*00004ef0*/ MOV         R3, #101                    @ 0x65 
    /*00004ef4*/ MOV         R1, #51                     @ 0x33 
    /*00004ef8*/ STR         R3, [SP] 
    /*00004efc*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004f00*/ LDR         R3, [PC, #3416]             @ 0x0000000000005c60 
    /*00004f04*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004f08*/ MOV         R0, #3 
    /*00004f0c*/ BL          0x0000000000004F0C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004f10*/ MOVW        R3, #1692                   @ 0x69c 
    /*00004f14*/ MLA         R6, R3, R8, R6 
    /*00004f18*/ MOV         LR, #1024                   @ 0x400 
    /*00004f1c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00004f20*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x20d0] 
    /*00004f24*/ MOVT        R3, #41011                  @ 0xa033 
    /*00004f28*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x20d0] 
    /*00004f2c*/ MOV         R1, #51                     @ 0x33 
    /*00004f30*/ MOV         R0, #3 
    /*00004f34*/ LDR         IP, [R6, #12] 
    /*00004f38*/ STR         R8, [SP, #12] 
    /*00004f3c*/ STR         LR, [SP, #8] 
    /*00004f40*/ STR         IP, [SP, #16] 
    /*00004f44*/ LDRH        IP, [R6, #22] 
    SVP_NNIE_CheckModel_x4f48:
    /*00004f48*/ STM         SP, {R8, IP} 
    /*00004f4c*/ BL          0x0000000000004F4C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004f50*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004f54*/ MOVT        R0, #41011                  @ 0xa033 
    /*00004f58*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x4f5c:
    /*00004f5c*/ LDR         R2, [LR, #864]              @ 0x360 
    /*00004f60*/ CMP         R2, #5 
    /*00004f64*/ BHI         SVP_NNIE_CheckModel_x556c
    /*00004f68*/ CMP         R7, #2 
    /*00004f6c*/ MOV         R3, LR 
    /*00004f70*/ MOV         R4, #0 
    /*00004f74*/ BEQ         SVP_NNIE_CheckModel_x4fcc
    /*00004f78*/ B           SVP_NNIE_CheckModel_x520c
    SVP_NNIE_CheckModel_x4f7c:
    /*00004f7c*/ LDR         R1, [R3, #876]              @ 0x36c 
    /*00004f80*/ CMP         R1, #0 
    /*00004f84*/ BEQ         SVP_NNIE_CheckModel_x5a08
    /*00004f88*/ CMP         R2, #4 
    /*00004f8c*/ BEQ         SVP_NNIE_CheckModel_x5038
    /*00004f90*/ CMP         R2, #0 
    /*00004f94*/ BNE         SVP_NNIE_CheckModel_x5c64
    /*00004f98*/ LDR         R2, [R3, #868]              @ 0x364 
    /*00004f9c*/ CMP         R2, #0 
    /*00004fa0*/ BEQ         SVP_NNIE_CheckModel_x5d40
    /*00004fa4*/ LDR         R2, [R3, #872]              @ 0x368 
    /*00004fa8*/ CMP         R2, #0 
    /*00004fac*/ BEQ         SVP_NNIE_CheckModel_x5ce8
    SVP_NNIE_CheckModel_x4fb0:
    /*00004fb0*/ ADD         R4, R4, #1 
    /*00004fb4*/ CMP         R9, R4 
    /*00004fb8*/ BEQ         SVP_NNIE_CheckModel_x5680
    /*00004fbc*/ ADD         R3, R3, #52                 @ 0x34 
    /*00004fc0*/ LDR         R2, [R3, #864]              @ 0x360 
    /*00004fc4*/ CMP         R2, #5 
    /*00004fc8*/ BHI         SVP_NNIE_CheckModel_x5570
    SVP_NNIE_CheckModel_x4fcc:
    /*00004fcc*/ CMP         R2, #5 
    /*00004fd0*/ BNE         SVP_NNIE_CheckModel_x4f7c
    /*00004fd4*/ LDR         R2, [R3, #868]              @ 0x364 
    /*00004fd8*/ CMP         R2, #0 
    /*00004fdc*/ BNE         SVP_NNIE_CheckModel_x4fb0
    SVP_NNIE_CheckModel_x4fe0:
    /*00004fe0*/ MOVW        R3, #263                    @ 0x107 
    /*00004fe4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00004fe8*/ STR         R3, [SP] 
    /*00004fec*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00004ff0*/ LDR         R3, [PC, #3176]             @ 0x0000000000005c60 
    /*00004ff4*/ MOV         R1, #51                     @ 0x33 
    /*00004ff8*/ MOV         R0, #3 
    /*00004ffc*/ BL          0x0000000000004FFC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005000*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005004*/ MUL         R3, R3, R8 
    /*00005008*/ MOV         R0, #52                     @ 0x34 
    /*0000500c*/ MLA         R0, R0, R4, R3 
    /*00005010*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2974] 
    /*00005014*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005018*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2974] 
    /*0000501c*/ MOVT        R3, #41011                  @ 0xa033 
    SVP_NNIE_CheckModel_x5020:
    /*00005020*/ ADD         R6, R6, R0 
    /*00005024*/ MOV         R1, #51                     @ 0x33 
    /*00005028*/ MOV         R0, #3 
    /*0000502c*/ LDR         IP, [R6, #868]              @ 0x364 
    /*00005030*/ STR         R4, [SP, #4] 
    /*00005034*/ B           SVP_NNIE_CheckModel_x51a0
    SVP_NNIE_CheckModel_x5038:
    /*00005038*/ LDR         R2, [R3, #868]              @ 0x364 
    /*0000503c*/ CMP         R2, #0 
    /*00005040*/ BEQ         SVP_NNIE_CheckModel_x5df4
    /*00005044*/ LDR         R2, [R3, #872]              @ 0x368 
    /*00005048*/ CMP         R2, #1 
    /*0000504c*/ BNE         SVP_NNIE_CheckModel_x5d84
    /*00005050*/ CMP         R1, #1 
    /*00005054*/ BEQ         SVP_NNIE_CheckModel_x4fb0
    SVP_NNIE_CheckModel_x5058:
    /*00005058*/ MOV         R3, #236                    @ 0xec 
    /*0000505c*/ MOV         R1, #51                     @ 0x33 
    /*00005060*/ STR         R3, [SP] 
    /*00005064*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005068*/ LDR         R3, [PC, #3056]             @ 0x0000000000005c60 
    /*0000506c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005070*/ MOV         R0, #3 
    /*00005074*/ BL          0x0000000000005074          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005078*/ MOVW        R3, #1692                   @ 0x69c 
    /*0000507c*/ MUL         R3, R3, R8 
    /*00005080*/ MOV         R1, #52                     @ 0x34 
    /*00005084*/ MLA         R1, R1, R4, R3 
    /*00005088*/ MOV         R3, #4 
    /*0000508c*/ STR         R3, [SP, #24] 
    /*00005090*/ MOVW        R2, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x283c] 
    /*00005094*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005098*/ MOVT        R2, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0x283c] 
    /*0000509c*/ ADD         R6, R6, R1 
    /*000050a0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000050a4*/ MOV         R1, #51                     @ 0x33 
    /*000050a8*/ MOV         R0, #3 
    /*000050ac*/ LDR         IP, [R6, #864]              @ 0x360 
    /*000050b0*/ STR         R4, [SP, #16] 
    /*000050b4*/ STR         R8, [SP, #12] 
    /*000050b8*/ STR         IP, [SP, #20] 
    /*000050bc*/ LDR         IP, [R6, #876]              @ 0x36c 
    /*000050c0*/ STR         R4, [SP, #4] 
    /*000050c4*/ B           SVP_NNIE_CheckModel_x4df4
    SVP_NNIE_CheckModel_x50c8:
    /*000050c8*/ MOV         R5, #0 
    SVP_NNIE_CheckModel_x50cc:
    /*000050cc*/ MOV         R3, #115                    @ 0x73 
    /*000050d0*/ MOV         R1, #51                     @ 0x33 
    /*000050d4*/ STR         R3, [SP] 
    /*000050d8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000050dc*/ LDR         R3, [PC, #2940]             @ 0x0000000000005c60 
    /*000050e0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000050e4*/ MOV         R0, #3 
    /*000050e8*/ BL          0x00000000000050E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000050ec*/ MOVW        R2, #1692                   @ 0x69c 
    /*000050f0*/ MUL         R3, R2, R8 
    /*000050f4*/ MOV         R2, #52                     @ 0x34 
    /*000050f8*/ MLA         R2, R2, R5, R3 
    /*000050fc*/ MOV         R1, #6 
    /*00005100*/ MOV         R3, #0 
    /*00005104*/ STR         R1, [SP, #16] 
    /*00005108*/ STR         R3, [SP, #12] 
    /*0000510c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005110*/ ADD         R6, R6, R2 
    /*00005114*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005118*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21a4] 
    /*0000511c*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21a4] 
    /*00005120*/ LDR         IP, [R6, #32] 
    /*00005124*/ MOV         R1, #51                     @ 0x33 
    SVP_NNIE_CheckModel_x5128:
    /*00005128*/ STR         R5, [SP, #4] 
    SVP_NNIE_CheckModel_x512c:
    /*0000512c*/ MOV         R0, #3 
    SVP_NNIE_CheckModel_x5130:
    /*00005130*/ STR         R8, [SP] 
    /*00005134*/ STR         IP, [SP, #8] 
    /*00005138*/ BL          0x0000000000005138          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000513c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005140*/ MOVT        R0, #41011                  @ 0xa033 
    /*00005144*/ ADD         SP, SP, #36                 @ 0x24 
    /*00005148*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_CheckModel_x514c:
    /*0000514c*/ MOV         R3, #122                    @ 0x7a 
    /*00005150*/ MOV         R1, #51                     @ 0x33 
    /*00005154*/ STR         R3, [SP] 
    /*00005158*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*0000515c*/ LDR         R3, [PC, #2812]             @ 0x0000000000005c60 
    /*00005160*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005164*/ MOV         R0, #3 
    /*00005168*/ BL          0x0000000000005168          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000516c*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005170*/ MUL         R3, R3, R8 
    /*00005174*/ MOV         R0, #52                     @ 0x34 
    /*00005178*/ MLA         R0, R0, R5, R3 
    /*0000517c*/ MOVW        R2, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21f0] 
    /*00005180*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005184*/ MOVT        R2, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21f0] 
    /*00005188*/ MOVT        R3, #41011                  @ 0xa033 
    /*0000518c*/ MOV         R1, #51                     @ 0x33 
    /*00005190*/ ADD         R6, R6, R0 
    /*00005194*/ MOV         R0, #3 
    /*00005198*/ LDR         IP, [R6, #44]               @ 0x2c 
    SVP_NNIE_CheckModel_x519c:
    /*0000519c*/ STR         R5, [SP, #4] 
    SVP_NNIE_CheckModel_x51a0:
    /*000051a0*/ STR         R8, [SP] 
    /*000051a4*/ STR         IP, [SP, #8] 
    /*000051a8*/ BL          0x00000000000051A8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000051ac*/ MOVW        R0, #32771                  @ 0x8003 
    /*000051b0*/ MOVT        R0, #41011                  @ 0xa033 
    /*000051b4*/ ADD         SP, SP, #36                 @ 0x24 
    /*000051b8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_CheckModel_x51bc:
    /*000051bc*/ LDR         R1, [R3, #876]              @ 0x36c 
    /*000051c0*/ CMP         R1, #0 
    /*000051c4*/ BEQ         SVP_NNIE_CheckModel_x5a08
    /*000051c8*/ CMP         R2, #4 
    /*000051cc*/ BEQ         SVP_NNIE_CheckModel_x5dfc
    /*000051d0*/ CMP         R2, #0 
    /*000051d4*/ BNE         SVP_NNIE_CheckModel_x5c64
    /*000051d8*/ LDR         R2, [R3, #868]              @ 0x364 
    /*000051dc*/ CMP         R2, #0 
    /*000051e0*/ BEQ         SVP_NNIE_CheckModel_x5d40
    /*000051e4*/ LDR         R2, [R3, #872]              @ 0x368 
    /*000051e8*/ CMP         R2, #0 
    /*000051ec*/ BEQ         SVP_NNIE_CheckModel_x5ce8
    SVP_NNIE_CheckModel_x51f0:
    /*000051f0*/ ADD         R4, R4, #1 
    /*000051f4*/ CMP         R4, R9 
    /*000051f8*/ BEQ         SVP_NNIE_CheckModel_x5e20
    /*000051fc*/ ADD         R3, R3, #52                 @ 0x34 
    /*00005200*/ LDR         R2, [R3, #864]              @ 0x360 
    /*00005204*/ CMP         R2, #5 
    /*00005208*/ BHI         SVP_NNIE_CheckModel_x5570
    SVP_NNIE_CheckModel_x520c:
    /*0000520c*/ CMP         R2, #5 
    /*00005210*/ BNE         SVP_NNIE_CheckModel_x51bc
    /*00005214*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005218*/ MOV         R5, #52                     @ 0x34 
    /*0000521c*/ MUL         R3, R3, R8 
    /*00005220*/ MLA         R5, R5, R4, R3 
    /*00005224*/ ADD         R5, R6, R5 
    /*00005228*/ LDR         R3, [R5, #868]              @ 0x364 
    /*0000522c*/ CMP         R3, #0 
    /*00005230*/ BEQ         SVP_NNIE_CheckModel_x4fe0
    /*00005234*/ MOVW        R3, #267                    @ 0x10b 
    /*00005238*/ MOV         R1, #51                     @ 0x33 
    /*0000523c*/ STR         R3, [SP] 
    /*00005240*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005244*/ LDR         R3, [PC, #2580]             @ 0x0000000000005c60 
    /*00005248*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*0000524c*/ MOV         R0, #3 
    /*00005250*/ BL          0x0000000000005250          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005254*/ STR         R8, [SP, #12] 
    /*00005258*/ MOV         R3, #2 
    /*0000525c*/ STR         R3, [SP, #16] 
    /*00005260*/ LDR         R1, [R5, #864]              @ 0x360 
    /*00005264*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005268*/ STR         R4, [SP, #4] 
    /*0000526c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005270*/ STR         R8, [SP] 
    /*00005274*/ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c4] 
    /*00005278*/ STR         R1, [SP, #8] 
    /*0000527c*/ MOVT        R2, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c4] 
    /*00005280*/ MOV         R1, #51                     @ 0x33 
    /*00005284*/ MOV         R0, #3 
    /*00005288*/ BL          0x0000000000005288          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000528c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005290*/ MOVT        R0, #41011                  @ 0xa033 
    /*00005294*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x5298:
    /*00005298*/ MOV         R3, #194                    @ 0xc2 
    /*0000529c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000052a0*/ STR         R3, [SP] 
    /*000052a4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000052a8*/ LDR         R3, [PC, #2480]             @ 0x0000000000005c60 
    /*000052ac*/ MOV         R1, #51                     @ 0x33 
    /*000052b0*/ MOV         R0, #3 
    /*000052b4*/ BL          0x00000000000052B4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000052b8*/ MOVW        R3, #1692                   @ 0x69c 
    /*000052bc*/ MUL         R3, R3, R8 
    /*000052c0*/ MOV         R0, #52                     @ 0x34 
    /*000052c4*/ MLA         R0, R0, R5, R3 
    /*000052c8*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x258c] 
    /*000052cc*/ MOVW        R3, #32771                  @ 0x8003 
    /*000052d0*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x258c] 
    /*000052d4*/ MOVT        R3, #41011                  @ 0xa033 
    SVP_NNIE_CheckModel_x52d8:
    /*000052d8*/ ADD         R6, R6, R0 
    /*000052dc*/ MOV         R1, #51                     @ 0x33 
    /*000052e0*/ MOV         R0, #3 
    /*000052e4*/ B           SVP_NNIE_CheckModel_x4e6c
    SVP_NNIE_CheckModel_x52e8:
    /*000052e8*/ MOV         R3, #73                     @ 0x49 
    /*000052ec*/ MOV         R1, #51                     @ 0x33 
    /*000052f0*/ STR         R3, [SP] 
    /*000052f4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000052f8*/ LDR         R3, [PC, #2400]             @ 0x0000000000005c60 
    /*000052fc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005300*/ MOV         R0, #3 
    /*00005304*/ BL          0x0000000000005304          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005308*/ LDR         R1, [R9, #1696]             @ 0x6a0 
    /*0000530c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005310*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fc8] 
    /*00005314*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005318*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fc8] 
    SVP_NNIE_CheckModel_x531c:
    /*0000531c*/ STR         R1, [SP] 
    /*00005320*/ MOV         R0, #3 
    /*00005324*/ MOV         R1, #51                     @ 0x33 
    /*00005328*/ BL          0x0000000000005328          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000532c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005330*/ MOVT        R0, #41011                  @ 0xa033 
    /*00005334*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x5338:
    /*00005338*/ MOV         R3, #197                    @ 0xc5 
    /*0000533c*/ MOV         R1, #51                     @ 0x33 
    /*00005340*/ STR         R3, [SP] 
    /*00005344*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005348*/ LDR         R3, [PC, #2320]             @ 0x0000000000005c60 
    /*0000534c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005350*/ MOV         R0, #3 
    /*00005354*/ BL          0x0000000000005354          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005358*/ MOVW        R3, #1692                   @ 0x69c 
    /*0000535c*/ MUL         R3, R3, R8 
    /*00005360*/ MOV         R1, #52                     @ 0x34 
    /*00005364*/ MLA         R1, R1, R5, R3 
    /*00005368*/ STR         R4, [SP, #12] 
    /*0000536c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005370*/ MOVW        R2, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x25dc] 
    /*00005374*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005378*/ MOVT        R2, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x25dc] 
    /*0000537c*/ ADD         R6, R6, R1 
    /*00005380*/ MOV         R0, #3 
    /*00005384*/ MOV         R1, #51                     @ 0x33 
    /*00005388*/ LDR         IP, [R6, #32] 
    /*0000538c*/ STR         R5, [SP, #4] 
    /*00005390*/ STR         R8, [SP] 
    /*00005394*/ STR         IP, [SP, #8] 
    /*00005398*/ BL          0x0000000000005398          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000539c*/ MOVW        R0, #32771                  @ 0x8003 
    /*000053a0*/ MOVT        R0, #41011                  @ 0xa033 
    /*000053a4*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x53a8:
    /*000053a8*/ MOV         R3, #158                    @ 0x9e 
    /*000053ac*/ MOV         R1, #51                     @ 0x33 
    /*000053b0*/ STR         R3, [SP] 
    /*000053b4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000053b8*/ LDR         R3, [PC, #2208]             @ 0x0000000000005c60 
    /*000053bc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000053c0*/ MOV         R0, #3 
    /*000053c4*/ BL          0x00000000000053C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000053c8*/ MOVW        R3, #1692                   @ 0x69c 
    /*000053cc*/ MUL         R3, R3, R8 
    /*000053d0*/ MOV         R2, #52                     @ 0x34 
    /*000053d4*/ MLA         R2, R2, R5, R3 
    /*000053d8*/ MOV         R1, #4096                   @ 0x1000 
    /*000053dc*/ MOV         R3, #16 
    /*000053e0*/ STR         R1, [SP, #16] 
    /*000053e4*/ STR         R3, [SP, #12] 
    /*000053e8*/ MOV         R1, #51                     @ 0x33 
    /*000053ec*/ ADD         R6, R6, R2 
    /*000053f0*/ MOVW        R3, #32771                  @ 0x8003 
    /*000053f4*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x240c] 
    /*000053f8*/ MOVT        R3, #41011                  @ 0xa033 
    /*000053fc*/ LDR         IP, [R6, #36]               @ 0x24 
    /*00005400*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x240c] 
    /*00005404*/ B           SVP_NNIE_CheckModel_x5128
    SVP_NNIE_CheckModel_x5408:
    /*00005408*/ MOV         R3, #146                    @ 0x92 
    SVP_NNIE_CheckModel_x540c:
    /*0000540c*/ STR         R3, [SP] 
    /*00005410*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005414*/ LDR         R3, [PC, #2116]             @ 0x0000000000005c60 
    /*00005418*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*0000541c*/ MOV         R1, #51                     @ 0x33 
    /*00005420*/ MOV         R0, #3 
    /*00005424*/ BL          0x0000000000005424          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005428*/ MOVW        R3, #1692                   @ 0x69c 
    /*0000542c*/ MUL         R3, R3, R8 
    /*00005430*/ MOV         R0, #52                     @ 0x34 
    /*00005434*/ MLA         R0, R0, R5, R3 
    /*00005438*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2238] 
    /*0000543c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005440*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2238] 
    /*00005444*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005448*/ B           SVP_NNIE_CheckModel_x52d8
    SVP_NNIE_CheckModel_x544c:
    /*0000544c*/ MOV         R3, #163                    @ 0xa3 
    /*00005450*/ MOV         R1, #51                     @ 0x33 
    /*00005454*/ STR         R3, [SP] 
    /*00005458*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*0000545c*/ LDR         R3, [PC, #2044]             @ 0x0000000000005c60 
    /*00005460*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005464*/ MOV         R0, #3 
    /*00005468*/ BL          0x0000000000005468          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000546c*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005470*/ MUL         R3, R3, R8 
    /*00005474*/ MOV         R2, #52                     @ 0x34 
    /*00005478*/ MLA         R2, R2, R5, R3 
    /*0000547c*/ MOV         R1, #4096                   @ 0x1000 
    /*00005480*/ MOV         R3, #16 
    /*00005484*/ STR         R1, [SP, #16] 
    /*00005488*/ STR         R3, [SP, #12] 
    /*0000548c*/ MOV         R1, #51                     @ 0x33 
    /*00005490*/ ADD         R6, R6, R2 
    /*00005494*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005498*/ MOVW        R2, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2468] 
    /*0000549c*/ MOVT        R3, #41011                  @ 0xa033 
    /*000054a0*/ LDR         IP, [R6, #40]               @ 0x28 
    /*000054a4*/ MOVT        R2, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2468] 
    /*000054a8*/ B           SVP_NNIE_CheckModel_x5128
    SVP_NNIE_CheckModel_x54ac:
    /*000054ac*/ MOV         R3, #150                    @ 0x96 
    /*000054b0*/ MOV         R1, #51                     @ 0x33 
    /*000054b4*/ STR         R3, [SP] 
    /*000054b8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000054bc*/ LDR         R3, [PC, #1948]             @ 0x0000000000005c60 
    /*000054c0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000054c4*/ MOV         R0, #3 
    /*000054c8*/ BL          0x00000000000054C8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000054cc*/ MOVW        R3, #1692                   @ 0x69c 
    /*000054d0*/ MUL         R3, R3, R8 
    /*000054d4*/ MOV         R0, #52                     @ 0x34 
    /*000054d8*/ MLA         R0, R0, R5, R3 
    /*000054dc*/ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23b4] 
    /*000054e0*/ MOVW        R3, #32771                  @ 0x8003 
    /*000054e4*/ MOVT        R2, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23b4] 
    /*000054e8*/ MOVT        R3, #41011                  @ 0xa033 
    /*000054ec*/ MOV         R1, #51                     @ 0x33 
    /*000054f0*/ ADD         R6, R6, R0 
    /*000054f4*/ MOV         R0, #3 
    /*000054f8*/ LDR         IP, [R6, #40]               @ 0x28 
    /*000054fc*/ B           SVP_NNIE_CheckModel_x519c
    SVP_NNIE_CheckModel_x5500:
    /*00005500*/ MOV         R3, #106                    @ 0x6a 
    /*00005504*/ MOV         R1, #51                     @ 0x33 
    /*00005508*/ STR         R3, [SP] 
    /*0000550c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005510*/ LDR         R3, [PC, #1864]             @ 0x0000000000005c60 
    /*00005514*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005518*/ MOV         R0, #3 
    /*0000551c*/ BL          0x000000000000551C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005520*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005524*/ MLA         R6, R3, R8, R6 
    /*00005528*/ MOVW        R2, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x213c] 
    /*0000552c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005530*/ MOVT        R2, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x213c] 
    /*00005534*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005538*/ MOV         R1, #51                     @ 0x33 
    /*0000553c*/ MOV         R0, #3 
    /*00005540*/ LDR         IP, [R6, #28] 
    /*00005544*/ STR         R8, [SP, #8] 
    /*00005548*/ STR         IP, [SP, #12] 
    /*0000554c*/ LDR         IP, [R6, #24] 
    SVP_NNIE_CheckModel_x5550:
    /*00005550*/ STM         SP, {R8, IP} 
    /*00005554*/ BL          0x0000000000005554          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005558*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000555c*/ MOVT        R0, #41011                  @ 0xa033 
    /*00005560*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x5564:
    /*00005564*/ MOV         R3, #129                    @ 0x81 
    /*00005568*/ B           SVP_NNIE_CheckModel_x540c
    SVP_NNIE_CheckModel_x556c:
    /*0000556c*/ MOV         R4, #0 
    SVP_NNIE_CheckModel_x5570:
    /*00005570*/ MOV         R3, #212                    @ 0xd4 
    /*00005574*/ MOV         R1, #51                     @ 0x33 
    /*00005578*/ STR         R3, [SP] 
    /*0000557c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005580*/ LDR         R3, [PC, #1752]             @ 0x0000000000005c60 
    /*00005584*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005588*/ MOV         R0, #3 
    /*0000558c*/ BL          0x000000000000558C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005590*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005594*/ MUL         R3, R3, R8 
    /*00005598*/ MOV         R2, #52                     @ 0x34 
    /*0000559c*/ MLA         R2, R2, R4, R3 
    /*000055a0*/ MOV         R1, #6 
    /*000055a4*/ MOV         R3, #0 
    /*000055a8*/ STR         R1, [SP, #16] 
    /*000055ac*/ STR         R3, [SP, #12] 
    /*000055b0*/ MOV         R1, #51                     @ 0x33 
    /*000055b4*/ ADD         R6, R6, R2 
    /*000055b8*/ MOVW        R3, #32771                  @ 0x8003 
    /*000055bc*/ MOVW        R2, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x26bc] 
    /*000055c0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000055c4*/ LDR         IP, [R6, #864]              @ 0x360 
    /*000055c8*/ MOVT        R2, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x26bc] 
    /*000055cc*/ STR         R4, [SP, #4] 
    /*000055d0*/ B           SVP_NNIE_CheckModel_x512c
    SVP_NNIE_CheckModel_x55d4:
    /*000055d4*/ MOV         R3, #134                    @ 0x86 
    /*000055d8*/ MOV         R1, #51                     @ 0x33 
    /*000055dc*/ STR         R3, [SP] 
    /*000055e0*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000055e4*/ LDR         R3, [PC, #1652]             @ 0x0000000000005c60 
    /*000055e8*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000055ec*/ MOV         R0, #3 
    /*000055f0*/ BL          0x00000000000055F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000055f4*/ MOVW        R3, #1692                   @ 0x69c 
    /*000055f8*/ MUL         R3, R3, R8 
    /*000055fc*/ MOV         R1, #52                     @ 0x34 
    /*00005600*/ MLA         R1, R1, R5, R3 
    /*00005604*/ STR         R4, [SP, #24] 
    /*00005608*/ MOVW        R3, #32771                  @ 0x8003 
    /*0000560c*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2290] 
    /*00005610*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005614*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2290] 
    /*00005618*/ ADD         R6, R6, R1 
    /*0000561c*/ MOV         R0, #3 
    /*00005620*/ MOV         R1, #51                     @ 0x33 
    /*00005624*/ LDR         IP, [R6, #32] 
    /*00005628*/ STR         R5, [SP, #16] 
    /*0000562c*/ STR         R8, [SP, #12] 
    /*00005630*/ STR         IP, [SP, #20] 
    /*00005634*/ LDR         IP, [R6, #40]               @ 0x28 
    /*00005638*/ B           SVP_NNIE_CheckModel_x4df0
    SVP_NNIE_CheckModel_x563c:
    /*0000563c*/ MOV         R3, #172                    @ 0xac 
    /*00005640*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005644*/ STR         R3, [SP] 
    /*00005648*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*0000564c*/ LDR         R3, [PC, #1548]             @ 0x0000000000005c60 
    /*00005650*/ MOV         R1, #51                     @ 0x33 
    /*00005654*/ MOV         R0, #3 
    /*00005658*/ BL          0x0000000000005658          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000565c*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005660*/ MUL         R3, R3, R8 
    /*00005664*/ MOV         R0, #52                     @ 0x34 
    /*00005668*/ MLA         R0, R0, R5, R3 
    /*0000566c*/ MOVW        R2, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24c4] 
    /*00005670*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005674*/ MOVT        R2, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24c4] 
    /*00005678*/ MOVT        R3, #41011                  @ 0xa033 
    /*0000567c*/ B           SVP_NNIE_CheckModel_x52d8
    SVP_NNIE_CheckModel_x5680:
    /*00005680*/ LDRH        R1, [LR, #20] 
    SVP_NNIE_CheckModel_x5684:
    /*00005684*/ CMP         R1, #0 
    /*00005688*/ BEQ         SVP_NNIE_CheckModel_x5968
    /*0000568c*/ LDR         R5, [LR, #1696]             @ 0x6a0 
    /*00005690*/ CMP         R5, #3 
    /*00005694*/ BHI         SVP_NNIE_CheckModel_x6310
    /*00005698*/ MOV         R3, #104                    @ 0x68 
    /*0000569c*/ MOVW        R2, #13604                  @ 0x3524 
    /*000056a0*/ MLA         R3, R3, R5, R6 
    /*000056a4*/ LDR         R2, [R3, R2] 
    /*000056a8*/ CMP         R2, #1 
    /*000056ac*/ BHI         SVP_NNIE_CheckModel_x62b8
    /*000056b0*/ MOVW        R2, #13612                  @ 0x352c 
    /*000056b4*/ LDR         R2, [R3, R2] 
    /*000056b8*/ CMP         R2, #1 
    /*000056bc*/ BHI         SVP_NNIE_CheckModel_x6260
    /*000056c0*/ MOVW        R2, #13608                  @ 0x3528 
    /*000056c4*/ LDR         R2, [R3, R2] 
    /*000056c8*/ CMP         R2, #1 
    /*000056cc*/ BHI         SVP_NNIE_CheckModel_x6208
    /*000056d0*/ MOVW        R2, #13616                  @ 0x3530 
    /*000056d4*/ LDR         R2, [R3, R2] 
    /*000056d8*/ CMP         R2, #1 
    /*000056dc*/ BHI         SVP_NNIE_CheckModel_x61b0
    /*000056e0*/ MOVW        R2, #13548                  @ 0x34ec 
    /*000056e4*/ LDR         IP, [R3, R2] 
    /*000056e8*/ SUB         R2, IP, #1 
    /*000056ec*/ CMP         R2, #4096                   @ 0x1000 
    /*000056f0*/ BCS         SVP_NNIE_CheckModel_x6158
    /*000056f4*/ MOVW        R2, #13552                  @ 0x34f0 
    /*000056f8*/ LDR         R4, [R3, R2] 
    /*000056fc*/ SUB         R2, R4, #1 
    /*00005700*/ CMP         R2, #4096                   @ 0x1000 
    /*00005704*/ BCS         SVP_NNIE_CheckModel_x60e8
    /*00005708*/ LDR         R2, [R3, SL] 
    /*0000570c*/ CMP         R2, #0 
    /*00005710*/ BEQ         SVP_NNIE_CheckModel_x60a8
    /*00005714*/ MOVW        R2, #13560                  @ 0x34f8 
    /*00005718*/ LDR         R2, [R3, R2] 
    /*0000571c*/ CLZ         R3, R2 
    /*00005720*/ LSR         R3, R3, #5 
    /*00005724*/ CMP         IP, R2 
    /*00005728*/ ORRCC       R3, R3, #1 
    /*0000572c*/ CMP         R3, #0 
    /*00005730*/ BNE         SVP_NNIE_CheckModel_x5e90
    /*00005734*/ MOV         R3, #104                    @ 0x68 
    /*00005738*/ MOVW        R2, #13564                  @ 0x34fc 
    /*0000573c*/ MLA         R3, R3, R5, R6 
    /*00005740*/ LDR         IP, [R3, R2] 
    /*00005744*/ CLZ         R2, IP 
    /*00005748*/ LSR         R2, R2, #5 
    /*0000574c*/ CMP         R4, IP 
    /*00005750*/ ORRCC       R2, R2, #1 
    /*00005754*/ CMP         R2, #0 
    /*00005758*/ BNE         SVP_NNIE_CheckModel_x5efc
    /*0000575c*/ ADD         R2, R3, #13568              @ 0x3500 
    /*00005760*/ LDR         R2, [R2] 
    /*00005764*/ CMP         R2, #0 
    /*00005768*/ BEQ         SVP_NNIE_CheckModel_x5f9c
    /*0000576c*/ MOVW        R2, #13572                  @ 0x3504 
    /*00005770*/ LDR         R2, [R3, R2] 
    /*00005774*/ CMP         R2, #0 
    /*00005778*/ BEQ         SVP_NNIE_CheckModel_x5f40
    /*0000577c*/ MOVW        R2, #13576                  @ 0x3508 
    /*00005780*/ LDR         R2, [R3, R2] 
    /*00005784*/ CMP         R2, #0 
    /*00005788*/ BEQ         SVP_NNIE_CheckModel_x6068
    /*0000578c*/ MOVW        R2, #13580                  @ 0x350c 
    /*00005790*/ LDR         R2, [R3, R2] 
    /*00005794*/ CMP         R2, #0 
    /*00005798*/ BEQ         SVP_NNIE_CheckModel_x6028
    /*0000579c*/ MOVW        R2, #13584                  @ 0x3510 
    /*000057a0*/ LDR         R2, [R3, R2] 
    /*000057a4*/ CMP         R2, #0 
    /*000057a8*/ BEQ         SVP_NNIE_CheckModel_x5fe8
    /*000057ac*/ MOVW        R2, #13588                  @ 0x3514 
    /*000057b0*/ LDR         R2, [R3, R2] 
    /*000057b4*/ CMP         R2, #0 
    /*000057b8*/ BEQ         SVP_NNIE_CheckModel_x636c
    /*000057bc*/ MOVW        R2, #13592                  @ 0x3518 
    /*000057c0*/ LDR         R2, [R3, R2] 
    /*000057c4*/ CMP         R2, #0 
    /*000057c8*/ BEQ         SVP_NNIE_CheckModel_x6450
    /*000057cc*/ MOVW        R2, #13596                  @ 0x351c 
    /*000057d0*/ MOVW        IP, #4999                   @ 0x1387 
    /*000057d4*/ LDR         R2, [R3, R2] 
    /*000057d8*/ SUB         R2, R2, #1 
    /*000057dc*/ CMP         R2, IP 
    /*000057e0*/ BHI         SVP_NNIE_CheckModel_x63ec
    /*000057e4*/ MOVW        R2, #13600                  @ 0x3520 
    /*000057e8*/ LDR         R3, [R3, R2] 
    /*000057ec*/ CMP         R3, #0 
    /*000057f0*/ BEQ         SVP_NNIE_CheckModel_x63ac
    /*000057f4*/ CMP         R1, #1 
    /*000057f8*/ BEQ         SVP_NNIE_CheckModel_x5968
    /*000057fc*/ LDR         R5, [LR, #1700]             @ 0x6a4 
    /*00005800*/ CMP         R5, #3 
    /*00005804*/ BHI         SVP_NNIE_CheckModel_x6498
    /*00005808*/ MOV         R3, #104                    @ 0x68 
    /*0000580c*/ MOVW        R2, #13604                  @ 0x3524 
    /*00005810*/ MLA         R3, R3, R5, R6 
    /*00005814*/ LDR         R2, [R3, R2] 
    /*00005818*/ CMP         R2, #1 
    /*0000581c*/ BHI         SVP_NNIE_CheckModel_x62b8
    /*00005820*/ MOVW        R2, #13612                  @ 0x352c 
    /*00005824*/ LDR         R2, [R3, R2] 
    /*00005828*/ CMP         R2, #1 
    /*0000582c*/ BHI         SVP_NNIE_CheckModel_x6260
    /*00005830*/ MOVW        R2, #13608                  @ 0x3528 
    /*00005834*/ LDR         R2, [R3, R2] 
    /*00005838*/ CMP         R2, #1 
    /*0000583c*/ BHI         SVP_NNIE_CheckModel_x6208
    /*00005840*/ MOVW        R2, #13616                  @ 0x3530 
    /*00005844*/ LDR         R2, [R3, R2] 
    /*00005848*/ CMP         R2, #1 
    /*0000584c*/ BHI         SVP_NNIE_CheckModel_x61b0
    /*00005850*/ MOVW        R2, #13548                  @ 0x34ec 
    /*00005854*/ LDR         IP, [R3, R2] 
    /*00005858*/ SUB         R2, IP, #1 
    /*0000585c*/ CMP         R2, #4096                   @ 0x1000 
    /*00005860*/ BCS         SVP_NNIE_CheckModel_x6158
    /*00005864*/ MOVW        R2, #13552                  @ 0x34f0 
    /*00005868*/ LDR         R2, [R3, R2] 
    /*0000586c*/ SUB         R1, R2, #1 
    /*00005870*/ CMP         R1, #4096                   @ 0x1000 
    /*00005874*/ BCS         SVP_NNIE_CheckModel_x60e8
    /*00005878*/ LDR         R1, [R3, SL] 
    /*0000587c*/ CMP         R1, #0 
    /*00005880*/ BEQ         SVP_NNIE_CheckModel_x60a8
    /*00005884*/ MOVW        R1, #13560                  @ 0x34f8 
    /*00005888*/ LDR         R1, [R3, R1] 
    /*0000588c*/ CLZ         R3, R1 
    /*00005890*/ LSR         R3, R3, #5 
    /*00005894*/ CMP         IP, R1 
    /*00005898*/ ORRCC       R3, R3, #1 
    /*0000589c*/ CMP         R3, #0 
    /*000058a0*/ BNE         SVP_NNIE_CheckModel_x5e90
    /*000058a4*/ MOV         R3, #104                    @ 0x68 
    /*000058a8*/ MOVW        R1, #13564                  @ 0x34fc 
    /*000058ac*/ MLA         R3, R3, R5, R6 
    /*000058b0*/ LDR         R1, [R3, R1] 
    /*000058b4*/ CMP         R2, R1 
    /*000058b8*/ MOVCS       R2, #0 
    /*000058bc*/ MOVCC       R2, #1 
    /*000058c0*/ CMP         R1, #0 
    /*000058c4*/ ORREQ       R2, R2, #1 
    /*000058c8*/ CMP         R2, #0 
    /*000058cc*/ BNE         SVP_NNIE_CheckModel_x5efc
    /*000058d0*/ ADD         R2, R3, #13568              @ 0x3500 
    /*000058d4*/ LDR         R2, [R2] 
    /*000058d8*/ CMP         R2, #0 
    /*000058dc*/ BEQ         SVP_NNIE_CheckModel_x5f9c
    /*000058e0*/ MOVW        R2, #13572                  @ 0x3504 
    /*000058e4*/ LDR         R2, [R3, R2] 
    /*000058e8*/ CMP         R2, #0 
    /*000058ec*/ BEQ         SVP_NNIE_CheckModel_x5f40
    /*000058f0*/ MOVW        R2, #13576                  @ 0x3508 
    /*000058f4*/ LDR         R2, [R3, R2] 
    /*000058f8*/ CMP         R2, #0 
    /*000058fc*/ BEQ         SVP_NNIE_CheckModel_x6068
    /*00005900*/ MOVW        R2, #13580                  @ 0x350c 
    /*00005904*/ LDR         R2, [R3, R2] 
    /*00005908*/ CMP         R2, #0 
    /*0000590c*/ BEQ         SVP_NNIE_CheckModel_x6028
    /*00005910*/ MOVW        R2, #13584                  @ 0x3510 
    /*00005914*/ LDR         R2, [R3, R2] 
    /*00005918*/ CMP         R2, #0 
    /*0000591c*/ BEQ         SVP_NNIE_CheckModel_x5fe8
    /*00005920*/ MOVW        R2, #13588                  @ 0x3514 
    /*00005924*/ LDR         R2, [R3, R2] 
    /*00005928*/ CMP         R2, #0 
    /*0000592c*/ BEQ         SVP_NNIE_CheckModel_x636c
    /*00005930*/ MOVW        R2, #13592                  @ 0x3518 
    /*00005934*/ LDR         R2, [R3, R2] 
    /*00005938*/ CMP         R2, #0 
    /*0000593c*/ BEQ         SVP_NNIE_CheckModel_x6450
    /*00005940*/ MOVW        R2, #13596                  @ 0x351c 
    /*00005944*/ MOVW        R1, #4999                   @ 0x1387 
    /*00005948*/ LDR         R2, [R3, R2] 
    /*0000594c*/ SUB         R2, R2, #1 
    /*00005950*/ CMP         R2, R1 
    /*00005954*/ BHI         SVP_NNIE_CheckModel_x63ec
    /*00005958*/ MOVW        R2, #13600                  @ 0x3520 
    /*0000595c*/ LDR         R3, [R3, R2] 
    /*00005960*/ CMP         R3, #0 
    /*00005964*/ BEQ         SVP_NNIE_CheckModel_x63ac
    SVP_NNIE_CheckModel_x5968:
    /*00005968*/ ADD         R8, R8, #1 
    /*0000596c*/ CMP         R8, R0 
    /*00005970*/ BEQ         SVP_NNIE_CheckModel_x6490
    /*00005974*/ LDR         R7, [LR, #1704]             @ 0x6a8 
    /*00005978*/ CMP         R7, #2 
    /*0000597c*/ BHI         SVP_NNIE_CheckModel_x5a64
    /*00005980*/ MOVW        R3, #1708                   @ 0x6ac 
    /*00005984*/ LDRH        R2, [LR, R3] 
    /*00005988*/ SUB         R3, R2, #1 
    /*0000598c*/ CMP         R3, #15 
    /*00005990*/ BHI         SVP_NNIE_CheckModel_x5abc
    /*00005994*/ MOVW        R3, #1710                   @ 0x6ae 
    /*00005998*/ LDRH        R9, [LR, R3] 
    /*0000599c*/ ADD         LR, LR, #1680               @ 0x690 
    /*000059a0*/ ADD         LR, LR, #12 
    /*000059a4*/ SUB         R3, R9, #1 
    /*000059a8*/ CMP         R3, #15 
    /*000059ac*/ BLS         SVP_NNIE_CheckModel_x4be0
    SVP_NNIE_CheckModel_x59b0:
    /*000059b0*/ MOV         R3, #95                     @ 0x5f 
    /*000059b4*/ MOV         R1, #51                     @ 0x33 
    /*000059b8*/ STR         R3, [SP] 
    /*000059bc*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000059c0*/ LDR         R3, [PC, #664]              @ 0x0000000000005c60 
    /*000059c4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000059c8*/ MOV         R0, #3 
    /*000059cc*/ BL          0x00000000000059CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000059d0*/ MOVW        R3, #1692                   @ 0x69c 
    /*000059d4*/ MLA         R6, R3, R8, R6 
    /*000059d8*/ MOV         R2, #16 
    /*000059dc*/ MOV         R3, #1 
    /*000059e0*/ STR         R2, [SP, #12] 
    /*000059e4*/ STR         R3, [SP, #8] 
    /*000059e8*/ MOVW        R2, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x208c] 
    /*000059ec*/ MOVW        R3, #32771                  @ 0x8003 
    /*000059f0*/ MOVT        R2, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x208c] 
    /*000059f4*/ MOVT        R3, #41011                  @ 0xa033 
    /*000059f8*/ MOV         R1, #51                     @ 0x33 
    /*000059fc*/ LDRH        IP, [R6, #18] 
    SVP_NNIE_CheckModel_x5a00:
    /*00005a00*/ MOV         R0, #3 
    /*00005a04*/ B           SVP_NNIE_CheckModel_x5550
    SVP_NNIE_CheckModel_x5a08:
    /*00005a08*/ MOV         R3, #219                    @ 0xdb 
    /*00005a0c*/ MOV         R1, #51                     @ 0x33 
    /*00005a10*/ STR         R3, [SP] 
    /*00005a14*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005a18*/ LDR         R3, [PC, #576]              @ 0x0000000000005c60 
    /*00005a1c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005a20*/ MOV         R0, #3 
    /*00005a24*/ BL          0x0000000000005A24          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005a28*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005a2c*/ MUL         R3, R3, R8 
    /*00005a30*/ MOV         R0, #52                     @ 0x34 
    /*00005a34*/ MLA         R0, R0, R4, R3 
    /*00005a38*/ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2708] 
    /*00005a3c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005a40*/ MOVT        R2, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2708] 
    /*00005a44*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005a48*/ MOV         R1, #51                     @ 0x33 
    /*00005a4c*/ ADD         R6, R6, R0 
    /*00005a50*/ MOV         R0, #3 
    /*00005a54*/ LDR         IP, [R6, #876]              @ 0x36c 
    /*00005a58*/ STR         R4, [SP, #4] 
    /*00005a5c*/ B           SVP_NNIE_CheckModel_x51a0
    SVP_NNIE_CheckModel_x5a60:
    /*00005a60*/ MOV         R8, #0 
    SVP_NNIE_CheckModel_x5a64:
    /*00005a64*/ MOV         R3, #85                     @ 0x55 
    /*00005a68*/ MOV         R1, #51                     @ 0x33 
    /*00005a6c*/ STR         R3, [SP] 
    /*00005a70*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005a74*/ LDR         R3, [PC, #484]              @ 0x0000000000005c60 
    /*00005a78*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005a7c*/ MOV         R0, #3 
    /*00005a80*/ BL          0x0000000000005A80          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005a84*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005a88*/ MLA         R6, R3, R8, R6 
    /*00005a8c*/ MOV         R0, #3 
    /*00005a90*/ MOV         R3, #0 
    /*00005a94*/ STR         R0, [SP, #12] 
    /*00005a98*/ STR         R3, [SP, #8] 
    /*00005a9c*/ MOVW        R2, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ffc] 
    /*00005aa0*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005aa4*/ MOVT        R2, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ffc] 
    /*00005aa8*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005aac*/ MOV         R1, #51                     @ 0x33 
    /*00005ab0*/ LDR         IP, [R6, #12] 
    /*00005ab4*/ B           SVP_NNIE_CheckModel_x5550
    SVP_NNIE_CheckModel_x5ab8:
    /*00005ab8*/ MOV         R8, #0 
    SVP_NNIE_CheckModel_x5abc:
    /*00005abc*/ MOV         R3, #90                     @ 0x5a 
    /*00005ac0*/ MOV         R1, #51                     @ 0x33 
    /*00005ac4*/ STR         R3, [SP] 
    /*00005ac8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005acc*/ LDR         R3, [PC, #396]              @ 0x0000000000005c60 
    /*00005ad0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005ad4*/ MOV         R0, #3 
    /*00005ad8*/ BL          0x0000000000005AD8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005adc*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005ae0*/ MLA         R6, R3, R8, R6 
    /*00005ae4*/ MOV         R2, #16 
    /*00005ae8*/ MOV         R3, #1 
    /*00005aec*/ STR         R2, [SP, #12] 
    /*00005af0*/ STR         R3, [SP, #8] 
    /*00005af4*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2048] 
    /*00005af8*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005afc*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2048] 
    /*00005b00*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005b04*/ MOV         R1, #51                     @ 0x33 
    /*00005b08*/ LDRH        IP, [R6, #16] 
    /*00005b0c*/ B           SVP_NNIE_CheckModel_x5a00
    SVP_NNIE_CheckModel_x5b10:
    /*00005b10*/ MOV         R3, #55                     @ 0x37 
    /*00005b14*/ MOV         R1, #51                     @ 0x33 
    /*00005b18*/ STR         R3, [SP] 
    /*00005b1c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005b20*/ LDR         R3, [PC, #312]              @ 0x0000000000005c60 
    /*00005b24*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005b28*/ MOV         R0, #3 
    /*00005b2c*/ BL          0x0000000000005B2C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005b30*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005b34*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1edc] 
    /*00005b38*/ LDR         R1, [R6, #4] 
    /*00005b3c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005b40*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1edc] 
    /*00005b44*/ B           SVP_NNIE_CheckModel_x531c
    SVP_NNIE_CheckModel_x5b48:
    /*00005b48*/ MOV         R8, #0 
    /*00005b4c*/ B           SVP_NNIE_CheckModel_x59b0
    SVP_NNIE_CheckModel_x5b50:
    /*00005b50*/ MOV         R3, #59                     @ 0x3b 
    /*00005b54*/ MOV         R1, #51                     @ 0x33 
    /*00005b58*/ STR         R3, [SP] 
    /*00005b5c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005b60*/ LDR         R3, [PC, #248]              @ 0x0000000000005c60 
    /*00005b64*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005b68*/ MOV         R0, #3 
    /*00005b6c*/ BL          0x0000000000005B6C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005b70*/ LDR         R2, [R6] 
    /*00005b74*/ MOV         R1, #0 
    /*00005b78*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005b7c*/ STR         R1, [SP, #4] 
    /*00005b80*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005b84*/ STR         R2, [SP] 
    /*00005b88*/ MOV         R1, #51                     @ 0x33 
    /*00005b8c*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f10] 
    /*00005b90*/ MOV         R0, #3 
    /*00005b94*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f10] 
    /*00005b98*/ BL          0x0000000000005B98          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005b9c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005ba0*/ MOVT        R0, #41011                  @ 0xa033 
    /*00005ba4*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x5ba8:
    /*00005ba8*/ MOV         R3, #70                     @ 0x46 
    /*00005bac*/ MOV         R1, #51                     @ 0x33 
    /*00005bb0*/ STR         R3, [SP] 
    /*00005bb4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005bb8*/ LDR         R3, [PC, #160]              @ 0x0000000000005c60 
    /*00005bbc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005bc0*/ MOV         R0, #3 
    /*00005bc4*/ BL          0x0000000000005BC4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005bc8*/ LDRD        R0, [R6, R8] 
    /*00005bcc*/ MOV         R2, #16 
    /*00005bd0*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005bd4*/ STR         R2, [SP, #8] 
    /*00005bd8*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005bdc*/ STRD        R0, [SP] 
    /*00005be0*/ MOVW        R2, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f80] 
    /*00005be4*/ MOV         R1, #51                     @ 0x33 
    /*00005be8*/ MOVT        R2, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f80] 
    /*00005bec*/ MOV         R0, #3 
    /*00005bf0*/ BL          0x0000000000005BF0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005bf4*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005bf8*/ MOVT        R0, #41011                  @ 0xa033 
    /*00005bfc*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x5c00:
    /*00005c00*/ MOV         R3, #63                     @ 0x3f 
    /*00005c04*/ MOV         R1, #51                     @ 0x33 
    /*00005c08*/ STR         R3, [SP] 
    /*00005c0c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005c10*/ LDR         R3, [PC, #72]               @ 0x0000000000005c60 
    /*00005c14*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005c18*/ MOV         R0, #3 
    /*00005c1c*/ BL          0x0000000000005C1C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005c20*/ LDR         R1, [R6, #8] 
    /*00005c24*/ MOV         R3, #8 
    /*00005c28*/ MOV         R2, #1 
    /*00005c2c*/ STR         R3, [SP, #8] 
    /*00005c30*/ MOV         R0, #3 
    /*00005c34*/ STR         R1, [SP] 
    /*00005c38*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005c3c*/ STR         R2, [SP, #4] 
    /*00005c40*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005c44*/ MOVW        R2, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f44] 
    /*00005c48*/ MOV         R1, #51                     @ 0x33 
    /*00005c4c*/ MOVT        R2, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f44] 
    /*00005c50*/ BL          0x0000000000005C50          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005c54*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005c58*/ MOVT        R0, #41011                  @ 0xa033 
    /*00005c5c*/ B           SVP_NNIE_CheckModel_x4e08
    /*00005c60*/ ANDEQ       R0, R0, R4, ROR #3 
    SVP_NNIE_CheckModel_x5c64:
    /*00005c64*/ MOV         R3, #254                    @ 0xfe 
    /*00005c68*/ MOV         R1, #51                     @ 0x33 
    /*00005c6c*/ STR         R3, [SP] 
    /*00005c70*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005c74*/ LDR         R3, [PC, #-28]              @ 0x0000000000005c60 
    /*00005c78*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005c7c*/ MOV         R0, #3 
    /*00005c80*/ BL          0x0000000000005C80          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005c84*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005c88*/ MUL         R2, R3, R8 
    /*00005c8c*/ MOV         R3, #52                     @ 0x34 
    /*00005c90*/ MLA         R3, R3, R4, R2 
    /*00005c94*/ MOV         R1, #5 
    /*00005c98*/ MOV         R2, #4 
    /*00005c9c*/ STR         R1, [SP, #20] 
    /*00005ca0*/ STR         R2, [SP, #16] 
    /*00005ca4*/ MOV         R2, #0 
    /*00005ca8*/ ADD         R6, R6, R3 
    /*00005cac*/ STR         R2, [SP, #12] 
    /*00005cb0*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005cb4*/ MOVW        R2, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2924] 
    /*00005cb8*/ LDR         R0, [R6, #864]              @ 0x360 
    /*00005cbc*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005cc0*/ STR         R4, [SP, #4] 
    /*00005cc4*/ MOVT        R2, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2924] 
    /*00005cc8*/ STR         R8, [SP] 
    /*00005ccc*/ MOV         R1, #51                     @ 0x33 
    /*00005cd0*/ STR         R0, [SP, #8] 
    /*00005cd4*/ MOV         R0, #3 
    /*00005cd8*/ BL          0x0000000000005CD8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005cdc*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005ce0*/ MOVT        R0, #41011                  @ 0xa033 
    /*00005ce4*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x5ce8:
    /*00005ce8*/ MOV         R3, #247                    @ 0xf7 
    /*00005cec*/ MOV         R1, #51                     @ 0x33 
    /*00005cf0*/ STR         R3, [SP] 
    /*00005cf4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005cf8*/ LDR         R3, [PC, #-160]             @ 0x0000000000005c60 
    /*00005cfc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005d00*/ MOV         R0, #3 
    /*00005d04*/ BL          0x0000000000005D04          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005d08*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005d0c*/ MUL         R3, R3, R8 
    /*00005d10*/ MOV         R0, #52                     @ 0x34 
    /*00005d14*/ MLA         R0, R0, R4, R3 
    /*00005d18*/ MOVW        R2, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28cc] 
    /*00005d1c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005d20*/ MOVT        R2, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28cc] 
    /*00005d24*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005d28*/ MOV         R1, #51                     @ 0x33 
    /*00005d2c*/ ADD         R6, R6, R0 
    /*00005d30*/ MOV         R0, #3 
    /*00005d34*/ LDR         IP, [R6, #872]              @ 0x368 
    /*00005d38*/ STR         R4, [SP, #4] 
    /*00005d3c*/ B           SVP_NNIE_CheckModel_x51a0
    SVP_NNIE_CheckModel_x5d40:
    /*00005d40*/ MOV         R3, #243                    @ 0xf3 
    SVP_NNIE_CheckModel_x5d44:
    /*00005d44*/ STR         R3, [SP] 
    /*00005d48*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005d4c*/ LDR         R3, [PC, #-244]             @ 0x0000000000005c60 
    /*00005d50*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005d54*/ MOV         R1, #51                     @ 0x33 
    /*00005d58*/ MOV         R0, #3 
    /*00005d5c*/ BL          0x0000000000005D5C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005d60*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005d64*/ MUL         R3, R3, R8 
    /*00005d68*/ MOV         R0, #52                     @ 0x34 
    /*00005d6c*/ MLA         R0, R0, R4, R3 
    /*00005d70*/ MOVW        R2, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2750] 
    /*00005d74*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005d78*/ MOVT        R2, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2750] 
    /*00005d7c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005d80*/ B           SVP_NNIE_CheckModel_x5020
    SVP_NNIE_CheckModel_x5d84:
    /*00005d84*/ MOV         R3, #231                    @ 0xe7 
    /*00005d88*/ MOV         R1, #51                     @ 0x33 
    /*00005d8c*/ STR         R3, [SP] 
    /*00005d90*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005d94*/ LDR         R3, [PC, #-316]             @ 0x0000000000005c60 
    /*00005d98*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005d9c*/ MOV         R0, #3 
    /*00005da0*/ BL          0x0000000000005DA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005da4*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005da8*/ MUL         R3, R3, R8 
    /*00005dac*/ MOV         R1, #52                     @ 0x34 
    /*00005db0*/ MLA         R1, R1, R4, R3 
    /*00005db4*/ MOV         R3, #4 
    /*00005db8*/ STR         R3, [SP, #24] 
    /*00005dbc*/ MOVW        R2, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x27a8] 
    /*00005dc0*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005dc4*/ MOVT        R2, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0x27a8] 
    /*00005dc8*/ ADD         R6, R6, R1 
    /*00005dcc*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005dd0*/ MOV         R1, #51                     @ 0x33 
    /*00005dd4*/ MOV         R0, #3 
    /*00005dd8*/ LDR         IP, [R6, #864]              @ 0x360 
    /*00005ddc*/ STR         R4, [SP, #16] 
    /*00005de0*/ STR         R8, [SP, #12] 
    /*00005de4*/ STR         IP, [SP, #20] 
    /*00005de8*/ LDR         IP, [R6, #872]              @ 0x368 
    /*00005dec*/ STR         R4, [SP, #4] 
    /*00005df0*/ B           SVP_NNIE_CheckModel_x4df4
    SVP_NNIE_CheckModel_x5df4:
    /*00005df4*/ MOV         R3, #226                    @ 0xe2 
    /*00005df8*/ B           SVP_NNIE_CheckModel_x5d44
    SVP_NNIE_CheckModel_x5dfc:
    /*00005dfc*/ LDR         R2, [R3, #868]              @ 0x364 
    /*00005e00*/ CMP         R2, #0 
    /*00005e04*/ BEQ         SVP_NNIE_CheckModel_x5df4
    /*00005e08*/ LDR         R2, [R3, #872]              @ 0x368 
    /*00005e0c*/ CMP         R2, #1 
    /*00005e10*/ BNE         SVP_NNIE_CheckModel_x5d84
    /*00005e14*/ CMP         R1, #1 
    /*00005e18*/ BEQ         SVP_NNIE_CheckModel_x51f0
    /*00005e1c*/ B           SVP_NNIE_CheckModel_x5058
    SVP_NNIE_CheckModel_x5e20:
    /*00005e20*/ CMP         R7, #1 
    /*00005e24*/ BNE         SVP_NNIE_CheckModel_x5680
    /*00005e28*/ LDRH        R1, [LR, #20] 
    /*00005e2c*/ SUB         R3, R1, #1 
    /*00005e30*/ CMP         R3, #1 
    /*00005e34*/ BLS         SVP_NNIE_CheckModel_x5684
    /*00005e38*/ MOVW        R3, #275                    @ 0x113 
    /*00005e3c*/ MOV         R1, #51                     @ 0x33 
    /*00005e40*/ STR         R3, [SP] 
    /*00005e44*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005e48*/ LDR         R3, [PC, #-496]             @ 0x0000000000005c60 
    /*00005e4c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005e50*/ MOV         R0, #3 
    /*00005e54*/ BL          0x0000000000005E54          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005e58*/ MOVW        R3, #1692                   @ 0x69c 
    /*00005e5c*/ MLA         R6, R3, R8, R6 
    /*00005e60*/ STR         R7, [SP, #16] 
    /*00005e64*/ MOV         R3, #2 
    /*00005e68*/ STR         R8, [SP, #12] 
    /*00005e6c*/ STR         R3, [SP, #8] 
    /*00005e70*/ MOVW        R2, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a34] 
    /*00005e74*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005e78*/ MOVT        R2, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a34] 
    /*00005e7c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005e80*/ MOV         R1, #51                     @ 0x33 
    /*00005e84*/ LDRH        IP, [R6, #20] 
    /*00005e88*/ MOV         R0, #3 
    /*00005e8c*/ B           SVP_NNIE_CheckModel_x4f48
    SVP_NNIE_CheckModel_x5e90:
    /*00005e90*/ MOVW        R3, #335                    @ 0x14f 
    /*00005e94*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005e98*/ STR         R3, [SP] 
    /*00005e9c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005ea0*/ LDR         R3, [PC, #-584]             @ 0x0000000000005c60 
    /*00005ea4*/ MOV         R1, #51                     @ 0x33 
    /*00005ea8*/ MOV         R0, #3 
    /*00005eac*/ BL          0x0000000000005EAC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005eb0*/ MOV         R3, #104                    @ 0x68 
    /*00005eb4*/ MLA         R6, R3, R5, R6 
    /*00005eb8*/ MOVW        R2, #0                      @ .LC64 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ce8] 
    /*00005ebc*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005ec0*/ MOVT        R2, #0                      @ .LC64 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ce8] 
    /*00005ec4*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005ec8*/ MOVW        LR, #13548                  @ 0x34ec 
    /*00005ecc*/ MOVW        IP, #13560                  @ 0x34f8 
    SVP_NNIE_CheckModel_x5ed0:
    /*00005ed0*/ LDR         LR, [R6, LR] 
    /*00005ed4*/ MOV         R1, #51                     @ 0x33 
    /*00005ed8*/ STR         R5, [SP, #8] 
    /*00005edc*/ MOV         R0, #3 
    /*00005ee0*/ STR         LR, [SP, #12] 
    /*00005ee4*/ LDR         IP, [R6, IP] 
    /*00005ee8*/ STM         SP, {R5, IP} 
    /*00005eec*/ BL          0x0000000000005EEC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005ef0*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005ef4*/ MOVT        R0, #41011                  @ 0xa033 
    /*00005ef8*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x5efc:
    /*00005efc*/ MOVW        R3, #342                    @ 0x156 
    /*00005f00*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005f04*/ STR         R3, [SP] 
    /*00005f08*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005f0c*/ LDR         R3, [PC, #-692]             @ 0x0000000000005c60 
    /*00005f10*/ MOV         R1, #51                     @ 0x33 
    /*00005f14*/ MOV         R0, #3 
    /*00005f18*/ BL          0x0000000000005F18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005f1c*/ MOV         R3, #104                    @ 0x68 
    /*00005f20*/ MLA         R6, R3, R5, R6 
    /*00005f24*/ MOVW        R2, #0                      @ .LC65 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2d64] 
    /*00005f28*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005f2c*/ MOVW        LR, #13552                  @ 0x34f0 
    /*00005f30*/ MOVW        IP, #13564                  @ 0x34fc 
    /*00005f34*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005f38*/ MOVT        R2, #0                      @ .LC65 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2d64] 
    /*00005f3c*/ B           SVP_NNIE_CheckModel_x5ed0
    SVP_NNIE_CheckModel_x5f40:
    /*00005f40*/ MOV         R3, #352                    @ 0x160 
    /*00005f44*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005f48*/ STR         R3, [SP] 
    /*00005f4c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005f50*/ LDR         R3, [PC, #-760]             @ 0x0000000000005c60 
    /*00005f54*/ MOV         R1, #51                     @ 0x33 
    /*00005f58*/ MOV         R0, #3 
    /*00005f5c*/ BL          0x0000000000005F5C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005f60*/ MOV         R3, #104                    @ 0x68 
    /*00005f64*/ MLA         R6, R3, R5, R6 
    /*00005f68*/ MOVW        R2, #0                      @ .LC67 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2e24] 
    /*00005f6c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005f70*/ MOVT        R2, #0                      @ .LC67 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2e24] 
    /*00005f74*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005f78*/ MOVW        IP, #13572                  @ 0x3504 
    SVP_NNIE_CheckModel_x5f7c:
    /*00005f7c*/ LDR         IP, [R6, IP] 
    /*00005f80*/ MOV         R1, #51                     @ 0x33 
    /*00005f84*/ MOV         R0, #3 
    SVP_NNIE_CheckModel_x5f88:
    /*00005f88*/ STM         SP, {R5, IP} 
    /*00005f8c*/ BL          0x0000000000005F8C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005f90*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005f94*/ MOVT        R0, #41011                  @ 0xa033 
    /*00005f98*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x5f9c:
    /*00005f9c*/ MOVW        R3, #347                    @ 0x15b 
    /*00005fa0*/ MOV         R1, #51                     @ 0x33 
    /*00005fa4*/ STR         R3, [SP] 
    /*00005fa8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005fac*/ LDR         R3, [PC, #-852]             @ 0x0000000000005c60 
    /*00005fb0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005fb4*/ MOV         R0, #3 
    /*00005fb8*/ BL          0x0000000000005FB8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005fbc*/ MOV         IP, #104                    @ 0x68 
    /*00005fc0*/ MLA         IP, IP, R5, R6 
    /*00005fc4*/ MOVW        R3, #32771                  @ 0x8003 
    /*00005fc8*/ MOVW        R2, #0                      @ .LC66 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2de0] 
    /*00005fcc*/ MOVT        R3, #41011                  @ 0xa033 
    /*00005fd0*/ MOVT        R2, #0                      @ .LC66 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2de0] 
    /*00005fd4*/ MOV         R1, #51                     @ 0x33 
    /*00005fd8*/ ADD         IP, IP, #13568              @ 0x3500 
    /*00005fdc*/ MOV         R0, #3 
    /*00005fe0*/ LDR         IP, [IP] 
    /*00005fe4*/ B           SVP_NNIE_CheckModel_x5f88
    SVP_NNIE_CheckModel_x5fe8:
    /*00005fe8*/ MOVW        R3, #367                    @ 0x16f 
    /*00005fec*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00005ff0*/ STR         R3, [SP] 
    /*00005ff4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00005ff8*/ LDR         R3, [PC, #-928]             @ 0x0000000000005c60 
    /*00005ffc*/ MOV         R1, #51                     @ 0x33 
    /*00006000*/ MOV         R0, #3 
    /*00006004*/ BL          0x0000000000006004          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006008*/ MOV         R3, #104                    @ 0x68 
    /*0000600c*/ MLA         R6, R3, R5, R6 
    /*00006010*/ MOVW        R2, #0                      @ .LC70 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2f08] 
    /*00006014*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006018*/ MOVW        IP, #13584                  @ 0x3510 
    /*0000601c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006020*/ MOVT        R2, #0                      @ .LC70 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2f08] 
    /*00006024*/ B           SVP_NNIE_CheckModel_x5f7c
    SVP_NNIE_CheckModel_x6028:
    /*00006028*/ MOVW        R3, #362                    @ 0x16a 
    /*0000602c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006030*/ STR         R3, [SP] 
    /*00006034*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006038*/ LDR         R3, [PC, #-992]             @ 0x0000000000005c60 
    /*0000603c*/ MOV         R1, #51                     @ 0x33 
    /*00006040*/ MOV         R0, #3 
    /*00006044*/ BL          0x0000000000006044          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006048*/ MOV         R3, #104                    @ 0x68 
    /*0000604c*/ MLA         R6, R3, R5, R6 
    /*00006050*/ MOVW        R2, #0                      @ .LC69 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2eb8] 
    /*00006054*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006058*/ MOVW        IP, #13580                  @ 0x350c 
    /*0000605c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006060*/ MOVT        R2, #0                      @ .LC69 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2eb8] 
    /*00006064*/ B           SVP_NNIE_CheckModel_x5f7c
    SVP_NNIE_CheckModel_x6068:
    /*00006068*/ MOVW        R3, #357                    @ 0x165 
    /*0000606c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006070*/ STR         R3, [SP] 
    /*00006074*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006078*/ LDR         R3, [PC, #-1056]            @ 0x0000000000005c60 
    /*0000607c*/ MOV         R1, #51                     @ 0x33 
    /*00006080*/ MOV         R0, #3 
    /*00006084*/ BL          0x0000000000006084          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006088*/ MOV         R3, #104                    @ 0x68 
    /*0000608c*/ MLA         R6, R3, R5, R6 
    /*00006090*/ MOVW        R2, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2e6c] 
    /*00006094*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006098*/ MOVW        IP, #13576                  @ 0x3508 
    /*0000609c*/ MOVT        R3, #41011                  @ 0xa033 
    /*000060a0*/ MOVT        R2, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2e6c] 
    /*000060a4*/ B           SVP_NNIE_CheckModel_x5f7c
    SVP_NNIE_CheckModel_x60a8:
    /*000060a8*/ MOV         R3, #328                    @ 0x148 
    /*000060ac*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000060b0*/ STR         R3, [SP] 
    /*000060b4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000060b8*/ LDR         R3, [PC, #-1120]            @ 0x0000000000005c60 
    /*000060bc*/ MOV         R1, #51                     @ 0x33 
    /*000060c0*/ MOV         R0, #3 
    /*000060c4*/ BL          0x00000000000060C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000060c8*/ MOV         R3, #104                    @ 0x68 
    /*000060cc*/ MLA         R6, R3, R5, R6 
    /*000060d0*/ MOVW        R2, #0                      @ .LC63 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ca4] 
    /*000060d4*/ MOVW        R3, #32771                  @ 0x8003 
    /*000060d8*/ MOVW        IP, #13556                  @ 0x34f4 
    /*000060dc*/ MOVT        R3, #41011                  @ 0xa033 
    /*000060e0*/ MOVT        R2, #0                      @ .LC63 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ca4] 
    /*000060e4*/ B           SVP_NNIE_CheckModel_x5f7c
    SVP_NNIE_CheckModel_x60e8:
    /*000060e8*/ MOVW        R3, #323                    @ 0x143 
    /*000060ec*/ MOV         R1, #51                     @ 0x33 
    /*000060f0*/ STR         R3, [SP] 
    /*000060f4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000060f8*/ LDR         R3, [PC, #-1184]            @ 0x0000000000005c60 
    /*000060fc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006100*/ MOV         R0, #3 
    /*00006104*/ BL          0x0000000000006104          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006108*/ MOV         R3, #104                    @ 0x68 
    /*0000610c*/ MLA         R6, R3, R5, R6 
    /*00006110*/ MOVW        R0, #13552                  @ 0x34f0 
    /*00006114*/ MOV         R3, #4096                   @ 0x1000 
    /*00006118*/ MOV         R2, #1 
    /*0000611c*/ STR         R3, [SP, #12] 
    /*00006120*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006124*/ STR         R2, [SP, #8] 
    /*00006128*/ MOVT        R3, #41011                  @ 0xa033 
    /*0000612c*/ MOVW        R2, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c5c] 
    /*00006130*/ MOVT        R2, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c5c] 
    /*00006134*/ LDR         IP, [R6, R0] 
    /*00006138*/ MOV         R1, #51                     @ 0x33 
    SVP_NNIE_CheckModel_x613c:
    /*0000613c*/ STR         R5, [SP] 
    /*00006140*/ MOV         R0, #3 
    /*00006144*/ STR         IP, [SP, #4] 
    /*00006148*/ BL          0x0000000000006148          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000614c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00006150*/ MOVT        R0, #41011                  @ 0xa033 
    /*00006154*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x6158:
    /*00006158*/ MOVW        R3, #317                    @ 0x13d 
    /*0000615c*/ MOV         R1, #51                     @ 0x33 
    /*00006160*/ STR         R3, [SP] 
    /*00006164*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006168*/ LDR         R3, [PC, #-1296]            @ 0x0000000000005c60 
    /*0000616c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006170*/ MOV         R0, #3 
    /*00006174*/ BL          0x0000000000006174          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006178*/ MOV         R3, #104                    @ 0x68 
    /*0000617c*/ MLA         R6, R3, R5, R6 
    /*00006180*/ MOVW        R0, #13548                  @ 0x34ec 
    /*00006184*/ MOV         R3, #4096                   @ 0x1000 
    /*00006188*/ MOV         R2, #1 
    /*0000618c*/ STR         R3, [SP, #12] 
    /*00006190*/ MOV         R1, #51                     @ 0x33 
    /*00006194*/ STR         R2, [SP, #8] 
    /*00006198*/ MOVW        R3, #32771                  @ 0x8003 
    /*0000619c*/ MOVW        R2, #0                      @ .LC61 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c14] 
    /*000061a0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000061a4*/ LDR         IP, [R6, R0] 
    /*000061a8*/ MOVT        R2, #0                      @ .LC61 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c14] 
    /*000061ac*/ B           SVP_NNIE_CheckModel_x613c
    SVP_NNIE_CheckModel_x61b0:
    /*000061b0*/ MOVW        R3, #311                    @ 0x137 
    /*000061b4*/ MOV         R1, #51                     @ 0x33 
    /*000061b8*/ STR         R3, [SP] 
    /*000061bc*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000061c0*/ LDR         R3, [PC, #-1384]            @ 0x0000000000005c60 
    /*000061c4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000061c8*/ MOV         R0, #3 
    /*000061cc*/ BL          0x00000000000061CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000061d0*/ MOV         R3, #104                    @ 0x68 
    /*000061d4*/ MLA         R6, R3, R5, R6 
    /*000061d8*/ MOVW        R0, #13608                  @ 0x3528 
    /*000061dc*/ MOV         R3, #2 
    /*000061e0*/ MOV         R2, #0 
    /*000061e4*/ STR         R3, [SP, #12] 
    /*000061e8*/ MOV         R1, #51                     @ 0x33 
    /*000061ec*/ STR         R2, [SP, #8] 
    /*000061f0*/ MOVW        R3, #32771                  @ 0x8003 
    /*000061f4*/ MOVW        R2, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2bd0] 
    /*000061f8*/ MOVT        R3, #41011                  @ 0xa033 
    /*000061fc*/ LDR         IP, [R6, R0] 
    /*00006200*/ MOVT        R2, #0                      @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2bd0] 
    /*00006204*/ B           SVP_NNIE_CheckModel_x613c
    SVP_NNIE_CheckModel_x6208:
    /*00006208*/ MOV         R3, #304                    @ 0x130 
    /*0000620c*/ MOV         R1, #51                     @ 0x33 
    /*00006210*/ STR         R3, [SP] 
    /*00006214*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006218*/ LDR         R3, [PC, #-1472]            @ 0x0000000000005c60 
    /*0000621c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006220*/ MOV         R0, #3 
    /*00006224*/ BL          0x0000000000006224          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006228*/ MOV         R3, #104                    @ 0x68 
    /*0000622c*/ MLA         R6, R3, R5, R6 
    /*00006230*/ MOVW        R0, #13608                  @ 0x3528 
    /*00006234*/ MOV         R3, #1 
    /*00006238*/ MOV         R2, #0 
    /*0000623c*/ STR         R3, [SP, #12] 
    /*00006240*/ MOV         R1, #51                     @ 0x33 
    /*00006244*/ STR         R2, [SP, #8] 
    /*00006248*/ MOVW        R3, #32771                  @ 0x8003 
    /*0000624c*/ MOVW        R2, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b88] 
    /*00006250*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006254*/ LDR         IP, [R6, R0] 
    /*00006258*/ MOVT        R2, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b88] 
    /*0000625c*/ B           SVP_NNIE_CheckModel_x613c
    SVP_NNIE_CheckModel_x6260:
    /*00006260*/ MOVW        R3, #297                    @ 0x129 
    /*00006264*/ MOV         R1, #51                     @ 0x33 
    /*00006268*/ STR         R3, [SP] 
    /*0000626c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006270*/ LDR         R3, [PC, #-1560]            @ 0x0000000000005c60 
    /*00006274*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006278*/ MOV         R0, #3 
    /*0000627c*/ BL          0x000000000000627C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006280*/ MOV         R3, #104                    @ 0x68 
    /*00006284*/ MLA         R6, R3, R5, R6 
    /*00006288*/ MOVW        R0, #13612                  @ 0x352c 
    /*0000628c*/ MOV         R3, #1 
    /*00006290*/ MOV         R2, #0 
    /*00006294*/ STR         R3, [SP, #12] 
    /*00006298*/ MOV         R1, #51                     @ 0x33 
    /*0000629c*/ STR         R2, [SP, #8] 
    /*000062a0*/ MOVW        R3, #32771                  @ 0x8003 
    /*000062a4*/ MOVW        R2, #0                      @ .LC58 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b44] 
    /*000062a8*/ MOVT        R3, #41011                  @ 0xa033 
    /*000062ac*/ LDR         IP, [R6, R0] 
    /*000062b0*/ MOVT        R2, #0                      @ .LC58 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b44] 
    /*000062b4*/ B           SVP_NNIE_CheckModel_x613c
    SVP_NNIE_CheckModel_x62b8:
    /*000062b8*/ MOVW        R3, #290                    @ 0x122 
    /*000062bc*/ MOV         R1, #51                     @ 0x33 
    /*000062c0*/ STR         R3, [SP] 
    /*000062c4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000062c8*/ LDR         R3, [PC, #-1648]            @ 0x0000000000005c60 
    /*000062cc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000062d0*/ MOV         R0, #3 
    /*000062d4*/ BL          0x00000000000062D4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000062d8*/ MOV         R3, #104                    @ 0x68 
    /*000062dc*/ MLA         R6, R3, R5, R6 
    /*000062e0*/ MOVW        R0, #13604                  @ 0x3524 
    /*000062e4*/ MOV         R3, #1 
    /*000062e8*/ MOV         R2, #0 
    /*000062ec*/ STR         R3, [SP, #12] 
    /*000062f0*/ MOV         R1, #51                     @ 0x33 
    /*000062f4*/ STR         R2, [SP, #8] 
    /*000062f8*/ MOVW        R3, #32771                  @ 0x8003 
    /*000062fc*/ MOVW        R2, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2afc] 
    /*00006300*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006304*/ LDR         IP, [R6, R0] 
    /*00006308*/ MOVT        R2, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2afc] 
    /*0000630c*/ B           SVP_NNIE_CheckModel_x613c
    SVP_NNIE_CheckModel_x6310:
    /*00006310*/ MOV         R4, #0 
    SVP_NNIE_CheckModel_x6314:
    /*00006314*/ MOV         R3, #284                    @ 0x11c 
    /*00006318*/ MOV         R1, #51                     @ 0x33 
    /*0000631c*/ STR         R3, [SP] 
    /*00006320*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006324*/ LDR         R3, [PC, #-1740]            @ 0x0000000000005c60 
    /*00006328*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*0000632c*/ MOV         R0, #3 
    /*00006330*/ BL          0x0000000000006330          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006334*/ MOV         R2, #4 
    /*00006338*/ STMIB       SP, {R4, R5} 
    /*0000633c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006340*/ STR         R2, [SP, #12] 
    /*00006344*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006348*/ STR         R8, [SP] 
    /*0000634c*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ab0] 
    /*00006350*/ MOV         R1, #51                     @ 0x33 
    /*00006354*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ab0] 
    /*00006358*/ MOV         R0, #3 
    /*0000635c*/ BL          0x000000000000635C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006360*/ MOVW        R0, #32771                  @ 0x8003 
    /*00006364*/ MOVT        R0, #41011                  @ 0xa033 
    /*00006368*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x636c:
    /*0000636c*/ MOV         R3, #372                    @ 0x174 
    /*00006370*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006374*/ STR         R3, [SP] 
    /*00006378*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*0000637c*/ LDR         R3, [PC, #-1828]            @ 0x0000000000005c60 
    /*00006380*/ MOV         R1, #51                     @ 0x33 
    /*00006384*/ MOV         R0, #3 
    /*00006388*/ BL          0x0000000000006388          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000638c*/ MOV         R3, #104                    @ 0x68 
    /*00006390*/ MLA         R6, R3, R5, R6 
    /*00006394*/ MOVW        R2, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2f58] 
    /*00006398*/ MOVW        R3, #32771                  @ 0x8003 
    /*0000639c*/ MOVW        IP, #13588                  @ 0x3514 
    /*000063a0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000063a4*/ MOVT        R2, #0                      @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2f58] 
    /*000063a8*/ B           SVP_NNIE_CheckModel_x5f7c
    SVP_NNIE_CheckModel_x63ac:
    /*000063ac*/ MOV         R3, #388                    @ 0x184 
    /*000063b0*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000063b4*/ STR         R3, [SP] 
    /*000063b8*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000063bc*/ LDR         R3, [PC, #-1892]            @ 0x0000000000005c60 
    /*000063c0*/ MOV         R1, #51                     @ 0x33 
    /*000063c4*/ MOV         R0, #3 
    /*000063c8*/ BL          0x00000000000063C8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000063cc*/ MOV         R3, #104                    @ 0x68 
    /*000063d0*/ MLA         R6, R3, R5, R6 
    /*000063d4*/ MOVW        R2, #0                      @ .LC74 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3028] 
    /*000063d8*/ MOVW        R3, #32771                  @ 0x8003 
    /*000063dc*/ MOVW        IP, #13600                  @ 0x3520 
    /*000063e0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000063e4*/ MOVT        R2, #0                      @ .LC74 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3028] 
    /*000063e8*/ B           SVP_NNIE_CheckModel_x5f7c
    SVP_NNIE_CheckModel_x63ec:
    /*000063ec*/ MOVW        R3, #383                    @ 0x17f 
    /*000063f0*/ MOV         R1, #51                     @ 0x33 
    /*000063f4*/ STR         R3, [SP] 
    /*000063f8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000063fc*/ LDR         R3, [PC, #-1956]            @ 0x0000000000005c60 
    /*00006400*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006404*/ MOV         R0, #3 
    /*00006408*/ BL          0x0000000000006408          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000640c*/ MOV         R3, #104                    @ 0x68 
    /*00006410*/ MLA         R6, R3, R5, R6 
    /*00006414*/ MOVW        IP, #13596                  @ 0x351c 
    /*00006418*/ MOVW        R3, #5000                   @ 0x1388 
    /*0000641c*/ STR         R3, [SP, #8] 
    /*00006420*/ MOVW        R2, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2fe0] 
    /*00006424*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006428*/ MOVT        R2, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2fe0] 
    /*0000642c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006430*/ MOV         R1, #51                     @ 0x33 
    /*00006434*/ MOV         R0, #3 
    /*00006438*/ LDR         IP, [R6, IP] 
    /*0000643c*/ STM         SP, {R5, IP} 
    /*00006440*/ BL          0x0000000000006440          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006444*/ MOVW        R0, #32771                  @ 0x8003 
    /*00006448*/ MOVT        R0, #41011                  @ 0xa033 
    /*0000644c*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x6450:
    /*00006450*/ MOVW        R3, #377                    @ 0x179 
    /*00006454*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006458*/ STR         R3, [SP] 
    /*0000645c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006460*/ LDR         R3, [PC, #-2056]            @ 0x0000000000005c60 
    /*00006464*/ MOV         R1, #51                     @ 0x33 
    /*00006468*/ MOV         R0, #3 
    /*0000646c*/ BL          0x000000000000646C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006470*/ MOV         R3, #104                    @ 0x68 
    /*00006474*/ MLA         R6, R3, R5, R6 
    /*00006478*/ MOVW        R2, #0                      @ .LC72 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2fa0] 
    /*0000647c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006480*/ MOVW        IP, #13592                  @ 0x3518 
    /*00006484*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006488*/ MOVT        R2, #0                      @ .LC72 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2fa0] 
    /*0000648c*/ B           SVP_NNIE_CheckModel_x5f7c
    SVP_NNIE_CheckModel_x6490:
    /*00006490*/ MOV         R0, #0 
    /*00006494*/ B           SVP_NNIE_CheckModel_x4e08
    SVP_NNIE_CheckModel_x6498:
    /*00006498*/ MOV         R4, #1 
    /*0000649c*/ B           SVP_NNIE_CheckModel_x6314
FUNC_END SVP_NNIE_CheckModel


FUNC_BEGIN SVP_NNIE_CheckForwardParamKernel
    /*000064a0*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*000064a4*/ SUBS        R8, R0, #0 
    /*000064a8*/ SUB         SP, SP, #32 
    /*000064ac*/ MOV         R4, R3 
    /*000064b0*/ MOV         R3, #0 
    /*000064b4*/ LDR         R9, [SP, #64]               @ 0x40 
    /*000064b8*/ STR         R3, [SP, #20] 
    /*000064bc*/ STR         R3, [SP, #24] 
    /*000064c0*/ STR         R3, [SP, #28] 
    /*000064c4*/ BEQ         SVP_NNIE_CheckForwardParamKernel_x6d10
    /*000064c8*/ CMP         R1, #0 
    /*000064cc*/ MOV         R5, R1 
    /*000064d0*/ BEQ         SVP_NNIE_CheckForwardParamKernel_x6c80
    /*000064d4*/ CMP         R2, #0 
    /*000064d8*/ MOV         R7, R2 
    /*000064dc*/ BEQ         SVP_NNIE_CheckForwardParamKernel_x6c38
    /*000064e0*/ CMP         R4, #0 
    /*000064e4*/ BEQ         SVP_NNIE_CheckForwardParamKernel_x6cc8
    /*000064e8*/ CMP         R9, #1 
    /*000064ec*/ BHI         SVP_NNIE_CheckForwardParamKernel_x67a8
    /*000064f0*/ MOV         R0, R1 
    /*000064f4*/ BL          0x0000000000004B2C 
    /*000064f8*/ SUBS        R6, R0, #0 
    /*000064fc*/ BNE         SVP_NNIE_CheckForwardParamKernel_x6674
    /*00006500*/ LDR         R3, [R4, #8] 
    /*00006504*/ LDR         R2, [R5, #8] 
    /*00006508*/ CMP         R3, R2 
    /*0000650c*/ BCS         SVP_NNIE_CheckForwardParamKernel_x6a48
    /*00006510*/ MOVW        R9, #1692                   @ 0x69c 
    /*00006514*/ MLA         R2, R9, R3, R5 
    /*00006518*/ LDR         SL, [R2, #12] 
    /*0000651c*/ CMP         SL, #1 
    /*00006520*/ BEQ         SVP_NNIE_CheckForwardParamKernel_x6be4
    /*00006524*/ LDR         R1, [R4, #12] 
    /*00006528*/ CMP         R1, #0 
    /*0000652c*/ BNE         SVP_NNIE_CheckForwardParamKernel_x6a98
    /*00006530*/ LDRH        R2, [R2, #16] 
    /*00006534*/ LDR         R1, [R4] 
    /*00006538*/ CMP         R1, R2 
    /*0000653c*/ BNE         SVP_NNIE_CheckForwardParamKernel_x6868
    /*00006540*/ MOVW        R6, #1692                   @ 0x69c 
    /*00006544*/ LDR         R2, [R4, #4] 
    /*00006548*/ MLA         R3, R6, R3, R5 
    /*0000654c*/ LDRH        R3, [R3, #18] 
    /*00006550*/ CMP         R2, R3 
    /*00006554*/ BNE         SVP_NNIE_CheckForwardParamKernel_x69a8
    /*00006558*/ LDR         R6, [R4, #32] 
    /*0000655c*/ LDR         R3, [R5, #4] 
    /*00006560*/ CMP         R6, R3 
    /*00006564*/ BCC         SVP_NNIE_CheckForwardParamKernel_x69f8
    /*00006568*/ LDR         R9, [R4, #16] 
    /*0000656c*/ MOV         R1, R6 
    /*00006570*/ MOV         R0, R9 
    /*00006574*/ BL          0x0000000000006574          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00006578*/ CMP         R0, #0 
    /*0000657c*/ BNE         SVP_NNIE_CheckForwardParamKernel_x6744
    SVP_NNIE_CheckForwardParamKernel_x6580:
    /*00006580*/ LDRD        R2, [R4, #16] 
    /*00006584*/ MOV         R3, #0 
    /*00006588*/ AND         R2, R2, #15 
    /*0000658c*/ ORRS        R3, R2, R3 
    /*00006590*/ BNE         SVP_NNIE_CheckForwardParamKernel_x6aec
    /*00006594*/ LDRD        R2, [R4, #40]               @ 0x28 
    /*00006598*/ MOV         R3, #0 
    /*0000659c*/ AND         R2, R2, #15 
    /*000065a0*/ ORRS        R3, R2, R3 
    /*000065a4*/ BNE         SVP_NNIE_CheckForwardParamKernel_x6b3c
    /*000065a8*/ LDR         R3, [R4, #8] 
    /*000065ac*/ MOVW        R9, #1692                   @ 0x69c 
    /*000065b0*/ MLA         R2, R9, R3, R5 
    /*000065b4*/ LDRH        R2, [R2, #20] 
    /*000065b8*/ CMP         R2, #0 
    /*000065bc*/ BNE         SVP_NNIE_CheckForwardParamKernel_x6950
    /*000065c0*/ MOV         R2, R5 
    /*000065c4*/ LDR         R1, [R4] 
    /*000065c8*/ MOV         R0, R8 
    /*000065cc*/ BL          0x0000000000003D88 
    /*000065d0*/ SUBS        R6, R0, #0 
    /*000065d4*/ BNE         SVP_NNIE_CheckForwardParamKernel_x6700
    /*000065d8*/ MOV         R0, R7 
    /*000065dc*/ LDR         R3, [R4, #8] 
    /*000065e0*/ MOV         R2, R5 
    /*000065e4*/ LDR         R1, [R4, #4] 
    /*000065e8*/ BL          0x00000000000044A8 
    /*000065ec*/ SUBS        R6, R0, #0 
    /*000065f0*/ BNE         SVP_NNIE_CheckForwardParamKernel_x66bc
    /*000065f4*/ LDR         R0, [R8, #24] 
    /*000065f8*/ MOV         R1, R6 
    /*000065fc*/ LDR         R2, [R4, #8] 
    /*00006600*/ ADD         LR, SP, #28 
    /*00006604*/ ADD         IP, SP, #24 
    /*00006608*/ MOV         R3, R5 
    /*0000660c*/ STMIB       SP, {IP, LR} 
    /*00006610*/ ADD         IP, SP, #20 
    /*00006614*/ STR         IP, [SP] 
    /*00006618*/ BL          0x0000000000003974 
    /*0000661c*/ SUBS        R6, R0, #0 
    /*00006620*/ BNE         SVP_NNIE_CheckForwardParamKernel_x6914
    /*00006624*/ LDR         R3, [SP, #24] 
    /*00006628*/ LDR         R1, [SP, #28] 
    /*0000662c*/ LDR         R2, [SP, #20] 
    /*00006630*/ ADD         R3, R3, R1 
    /*00006634*/ LDR         R5, [R4, #56]               @ 0x38 
    /*00006638*/ ADD         R3, R3, R2 
    /*0000663c*/ STR         R3, [SP, #20] 
    /*00006640*/ CMP         R3, R5 
    /*00006644*/ BHI         SVP_NNIE_CheckForwardParamKernel_x6b8c
    /*00006648*/ LDR         R7, [R4, #40]               @ 0x28 
    /*0000664c*/ MOV         R1, R5 
    /*00006650*/ MOV         R0, R7 
    /*00006654*/ BL          0x0000000000006654          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00006658*/ CMP         R0, #0 
    /*0000665c*/ BNE         SVP_NNIE_CheckForwardParamKernel_x67fc
    SVP_NNIE_CheckForwardParamKernel_x6660:
    /*00006660*/ LDR         R3, [SP, #20] 
    /*00006664*/ MOV         R0, R6 
    /*00006668*/ STR         R3, [R4, #56]               @ 0x38 
    /*0000666c*/ ADD         SP, SP, #32 
    /*00006670*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    SVP_NNIE_CheckForwardParamKernel_x6674:
    /*00006674*/ MOVW        R3, #1133                   @ 0x46d 
    /*00006678*/ MOV         R1, #51                     @ 0x33 
    /*0000667c*/ STR         R3, [SP] 
    /*00006680*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006684*/ LDR         R3, [PC, #1740]             @ 0x0000000000006d58 
    /*00006688*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*0000668c*/ MOV         R0, #3 
    /*00006690*/ BL          0x0000000000006690          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006694*/ MOVW        R2, #0                      @ .LC81 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3184] 
    /*00006698*/ STR         R5, [SP] 
    /*0000669c*/ MOV         R3, R6 
    /*000066a0*/ MOVT        R2, #0                      @ .LC81 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3184] 
    /*000066a4*/ MOV         R1, #51                     @ 0x33 
    /*000066a8*/ MOV         R0, #3 
    /*000066ac*/ BL          0x00000000000066AC          @ HI_LOG [R_ARM_CALL: +0x0] 
    SVP_NNIE_CheckForwardParamKernel_x66b0:
    /*000066b0*/ MOV         R0, R6 
    /*000066b4*/ ADD         SP, SP, #32 
    /*000066b8*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    SVP_NNIE_CheckForwardParamKernel_x66bc:
    /*000066bc*/ MOVW        R3, #1149                   @ 0x47d 
    /*000066c0*/ MOV         R1, #51                     @ 0x33 
    /*000066c4*/ STR         R3, [SP] 
    /*000066c8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000066cc*/ LDR         R3, [PC, #1668]             @ 0x0000000000006d58 
    /*000066d0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000066d4*/ MOV         R0, #3 
    /*000066d8*/ BL          0x00000000000066D8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000066dc*/ MOV         R3, R6 
    /*000066e0*/ MOV         R1, #51                     @ 0x33 
    /*000066e4*/ MOVW        R2, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x34b4] 
    /*000066e8*/ MOV         R0, #3 
    /*000066ec*/ MOVT        R2, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x34b4] 
    /*000066f0*/ BL          0x00000000000066F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000066f4*/ MOV         R0, R6 
    /*000066f8*/ ADD         SP, SP, #32 
    /*000066fc*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    SVP_NNIE_CheckForwardParamKernel_x6700:
    /*00006700*/ MOVW        R3, #1145                   @ 0x479 
    /*00006704*/ MOV         R1, #51                     @ 0x33 
    /*00006708*/ STR         R3, [SP] 
    /*0000670c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006710*/ LDR         R3, [PC, #1600]             @ 0x0000000000006d58 
    /*00006714*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006718*/ MOV         R0, #3 
    /*0000671c*/ BL          0x000000000000671C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006720*/ MOV         R3, R6 
    /*00006724*/ MOV         R1, #51                     @ 0x33 
    /*00006728*/ MOVW        R2, #0                      @ .LC91 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348c] 
    /*0000672c*/ MOV         R0, #3 
    /*00006730*/ MOVT        R2, #0                      @ .LC91 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348c] 
    /*00006734*/ BL          0x0000000000006734          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006738*/ MOV         R0, R6 
    /*0000673c*/ ADD         SP, SP, #32 
    /*00006740*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    SVP_NNIE_CheckForwardParamKernel_x6744:
    /*00006744*/ MOV         R1, R6 
    /*00006748*/ MOV         R0, R9 
    /*0000674c*/ BL          0x000000000000674C          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*00006750*/ CMP         R0, #0 
    /*00006754*/ BEQ         SVP_NNIE_CheckForwardParamKernel_x6580
    /*00006758*/ MOVW        R3, #978                    @ 0x3d2 
    /*0000675c*/ MOV         R1, #51                     @ 0x33 
    /*00006760*/ STR         R3, [SP] 
    /*00006764*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006768*/ LDR         R3, [PC, #1516]             @ 0x0000000000006d5c 
    /*0000676c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006770*/ MOV         R0, #3 
    /*00006774*/ BL          0x0000000000006774          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006778*/ LDRD        R0, [R4, #16] 
    /*0000677c*/ LDR         IP, [R4, #32] 
    /*00006780*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006784*/ MOVW        R2, #0                      @ .LC95 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x356c] 
    /*00006788*/ MOVT        R3, #41011                  @ 0xa033 
    /*0000678c*/ STRD        R0, [SP] 
    /*00006790*/ MOVT        R2, #0                      @ .LC95 [R_ARM_MOVT_ABS: .rodata.str1.4+0x356c] 
    /*00006794*/ STR         IP, [SP, #8] 
    /*00006798*/ MOV         R1, #51                     @ 0x33 
    /*0000679c*/ MOV         R0, #3 
    /*000067a0*/ BL          0x00000000000067A0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000067a4*/ B           SVP_NNIE_CheckForwardParamKernel_x68c0
    SVP_NNIE_CheckForwardParamKernel_x67a8:
    /*000067a8*/ MOVW        R3, #1128                   @ 0x468 
    /*000067ac*/ MOV         R1, #51                     @ 0x33 
    /*000067b0*/ STR         R3, [SP] 
    /*000067b4*/ MOVW        R6, #32771                  @ 0x8003 
    /*000067b8*/ LDR         R3, [PC, #1432]             @ 0x0000000000006d58 
    /*000067bc*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000067c0*/ MOV         R0, #3 
    /*000067c4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000067c8*/ BL          0x00000000000067C8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000067cc*/ STR         R9, [SP] 
    /*000067d0*/ MOV         R3, R6 
    /*000067d4*/ MOV         R1, #51                     @ 0x33 
    /*000067d8*/ MOVT        R3, #41011                  @ 0xa033 
    /*000067dc*/ MOVW        R2, #0                      @ .LC80 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x314c] 
    /*000067e0*/ MOV         R0, #3 
    /*000067e4*/ MOVT        R2, #0                      @ .LC80 [R_ARM_MOVT_ABS: .rodata.str1.4+0x314c] 
    /*000067e8*/ BL          0x00000000000067E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000067ec*/ MOVT        R6, #41011                  @ 0xa033 
    /*000067f0*/ MOV         R0, R6 
    /*000067f4*/ ADD         SP, SP, #32 
    /*000067f8*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    SVP_NNIE_CheckForwardParamKernel_x67fc:
    /*000067fc*/ MOV         R1, R5 
    /*00006800*/ MOV         R0, R7 
    /*00006804*/ BL          0x0000000000006804          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*00006808*/ CMP         R0, #0 
    /*0000680c*/ BEQ         SVP_NNIE_CheckForwardParamKernel_x6660
    /*00006810*/ MOVW        R3, #1163                   @ 0x48b 
    /*00006814*/ MOV         R1, #51                     @ 0x33 
    /*00006818*/ STR         R3, [SP] 
    /*0000681c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006820*/ LDR         R3, [PC, #1328]             @ 0x0000000000006d58 
    /*00006824*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006828*/ MOV         R0, #3 
    /*0000682c*/ MOVW        R6, #32771                  @ 0x8003 
    /*00006830*/ BL          0x0000000000006830          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006834*/ LDRD        R0, [R4, #40]               @ 0x28 
    /*00006838*/ LDR         IP, [R4, #56]               @ 0x38 
    /*0000683c*/ MOV         R3, R6 
    /*00006840*/ MOVW        R2, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3618] 
    /*00006844*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006848*/ STRD        R0, [SP] 
    /*0000684c*/ MOVT        R2, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3618] 
    /*00006850*/ STR         IP, [SP, #8] 
    /*00006854*/ MOV         R1, #51                     @ 0x33 
    /*00006858*/ MOV         R0, #3 
    /*0000685c*/ MOVT        R6, #41011                  @ 0xa033 
    /*00006860*/ BL          0x0000000000006860          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006864*/ B           SVP_NNIE_CheckForwardParamKernel_x66b0
    SVP_NNIE_CheckForwardParamKernel_x6868:
    /*00006868*/ MOVW        R3, #958                    @ 0x3be 
    /*0000686c*/ MOV         R1, #51                     @ 0x33 
    /*00006870*/ STR         R3, [SP] 
    /*00006874*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006878*/ LDR         R3, [PC, #1244]             @ 0x0000000000006d5c 
    /*0000687c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006880*/ MOV         R0, #3 
    /*00006884*/ BL          0x0000000000006884          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006888*/ LDR         LR, [R4, #8] 
    /*0000688c*/ MOVW        IP, #1692                   @ 0x69c 
    /*00006890*/ LDR         R4, [R4] 
    /*00006894*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006898*/ MOVW        R2, #0                      @ .LC85 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3264] 
    /*0000689c*/ MLA         IP, IP, LR, R5 
    /*000068a0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000068a4*/ MOVT        R2, #0                      @ .LC85 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3264] 
    /*000068a8*/ MOV         R1, #51                     @ 0x33 
    /*000068ac*/ MOV         R0, #3 
    /*000068b0*/ LDRH        IP, [IP, #16] 
    /*000068b4*/ STM         SP, {R4, LR} 
    /*000068b8*/ STR         IP, [SP, #8] 
    /*000068bc*/ BL          0x00000000000068BC          @ HI_LOG [R_ARM_CALL: +0x0] 
    SVP_NNIE_CheckForwardParamKernel_x68c0:
    /*000068c0*/ MOVW        R3, #1138                   @ 0x472 
    /*000068c4*/ MOV         R1, #51                     @ 0x33 
    /*000068c8*/ STR         R3, [SP] 
    /*000068cc*/ MOVW        R6, #32771                  @ 0x8003 
    /*000068d0*/ LDR         R3, [PC, #1152]             @ 0x0000000000006d58 
    /*000068d4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000068d8*/ MOV         R0, #3 
    /*000068dc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000068e0*/ BL          0x00000000000068E0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000068e4*/ STR         R5, [SP] 
    /*000068e8*/ MOV         R3, R6 
    /*000068ec*/ MOV         R1, #51                     @ 0x33 
    /*000068f0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000068f4*/ MOVW        R2, #0                      @ .LC96 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x35e4] 
    /*000068f8*/ MOV         R0, #3 
    /*000068fc*/ MOVT        R2, #0                      @ .LC96 [R_ARM_MOVT_ABS: .rodata.str1.4+0x35e4] 
    /*00006900*/ BL          0x0000000000006900          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006904*/ MOVT        R6, #41011                  @ 0xa033 
    /*00006908*/ MOV         R0, R6 
    /*0000690c*/ ADD         SP, SP, #32 
    /*00006910*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    SVP_NNIE_CheckForwardParamKernel_x6914:
    /*00006914*/ MOVW        R3, #1154                   @ 0x482 
    /*00006918*/ MOV         R1, #51                     @ 0x33 
    /*0000691c*/ STR         R3, [SP] 
    /*00006920*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006924*/ LDR         R3, [PC, #1068]             @ 0x0000000000006d58 
    /*00006928*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*0000692c*/ MOV         R0, #3 
    /*00006930*/ BL          0x0000000000006930          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006934*/ MOV         R3, R6 
    /*00006938*/ MOVW        R2, #0                      @ .LC93 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x34dc] 
    /*0000693c*/ MOV         R1, #51                     @ 0x33 
    /*00006940*/ MOVT        R2, #0                      @ .LC93 [R_ARM_MOVT_ABS: .rodata.str1.4+0x34dc] 
    /*00006944*/ MOV         R0, #3 
    /*00006948*/ BL          0x0000000000006948          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000694c*/ B           SVP_NNIE_CheckForwardParamKernel_x66b0
    SVP_NNIE_CheckForwardParamKernel_x6950:
    /*00006950*/ MOVW        R3, #1141                   @ 0x475 
    /*00006954*/ MOV         R1, #51                     @ 0x33 
    /*00006958*/ STR         R3, [SP] 
    /*0000695c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006960*/ LDR         R3, [PC, #1008]             @ 0x0000000000006d58 
    /*00006964*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006968*/ MOV         R0, #3 
    /*0000696c*/ MOVW        R6, #32771                  @ 0x8003 
    /*00006970*/ BL          0x0000000000006970          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006974*/ LDR         IP, [R4, #8] 
    /*00006978*/ MOV         R3, R6 
    /*0000697c*/ MOVW        R2, #0                      @ .LC90 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3434] 
    /*00006980*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006984*/ MOVT        R2, #0                      @ .LC90 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3434] 
    /*00006988*/ MLA         R5, R9, IP, R5 
    /*0000698c*/ MOV         R1, #51                     @ 0x33 
    /*00006990*/ MOV         R0, #3 
    /*00006994*/ MOVT        R6, #41011                  @ 0xa033 
    /*00006998*/ LDRH        LR, [R5, #20] 
    /*0000699c*/ STM         SP, {IP, LR} 
    /*000069a0*/ BL          0x00000000000069A0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000069a4*/ B           SVP_NNIE_CheckForwardParamKernel_x66b0
    SVP_NNIE_CheckForwardParamKernel_x69a8:
    /*000069a8*/ MOVW        R3, #963                    @ 0x3c3 
    /*000069ac*/ MOV         R1, #51                     @ 0x33 
    /*000069b0*/ STR         R3, [SP] 
    /*000069b4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000069b8*/ LDR         R3, [PC, #924]              @ 0x0000000000006d5c 
    /*000069bc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000069c0*/ MOV         R0, #3 
    /*000069c4*/ BL          0x00000000000069C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000069c8*/ LDMIB       R4, {R4, IP} 
    /*000069cc*/ MOVW        R3, #32771                  @ 0x8003 
    /*000069d0*/ MOVW        R2, #0                      @ .LC86 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x32c4] 
    /*000069d4*/ MOVT        R3, #41011                  @ 0xa033 
    /*000069d8*/ MOVT        R2, #0                      @ .LC86 [R_ARM_MOVT_ABS: .rodata.str1.4+0x32c4] 
    /*000069dc*/ MLA         R6, R6, IP, R5 
    /*000069e0*/ MOV         R1, #51                     @ 0x33 
    /*000069e4*/ MOV         R0, #3 
    /*000069e8*/ LDRH        LR, [R6, #18] 
    /*000069ec*/ STM         SP, {R4, IP, LR} 
    /*000069f0*/ BL          0x00000000000069F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000069f4*/ B           SVP_NNIE_CheckForwardParamKernel_x68c0
    SVP_NNIE_CheckForwardParamKernel_x69f8:
    /*000069f8*/ MOVW        R3, #973                    @ 0x3cd 
    /*000069fc*/ MOV         R1, #51                     @ 0x33 
    /*00006a00*/ STR         R3, [SP] 
    /*00006a04*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006a08*/ LDR         R3, [PC, #844]              @ 0x0000000000006d5c 
    /*00006a0c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006a10*/ MOV         R0, #3 
    /*00006a14*/ BL          0x0000000000006A14          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006a18*/ LDR         R0, [R4, #32] 
    /*00006a1c*/ LDR         R1, [R5, #4] 
    /*00006a20*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006a24*/ MOVW        R2, #0                      @ .LC87 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3324] 
    /*00006a28*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006a2c*/ STR         R0, [SP] 
    /*00006a30*/ MOVT        R2, #0                      @ .LC87 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3324] 
    /*00006a34*/ STR         R1, [SP, #4] 
    /*00006a38*/ MOV         R0, #3 
    /*00006a3c*/ MOV         R1, #51                     @ 0x33 
    /*00006a40*/ BL          0x0000000000006A40          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006a44*/ B           SVP_NNIE_CheckForwardParamKernel_x68c0
    SVP_NNIE_CheckForwardParamKernel_x6a48:
    /*00006a48*/ MOVW        R3, #942                    @ 0x3ae 
    /*00006a4c*/ MOV         R1, #51                     @ 0x33 
    /*00006a50*/ STR         R3, [SP] 
    /*00006a54*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006a58*/ LDR         R3, [PC, #764]              @ 0x0000000000006d5c 
    /*00006a5c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006a60*/ MOV         R0, #3 
    /*00006a64*/ BL          0x0000000000006A64          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006a68*/ LDR         R0, [R4, #8] 
    /*00006a6c*/ LDR         R1, [R5, #8] 
    /*00006a70*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006a74*/ MOVW        R2, #0                      @ .LC82 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x31b0] 
    /*00006a78*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006a7c*/ STR         R0, [SP] 
    /*00006a80*/ MOVT        R2, #0                      @ .LC82 [R_ARM_MOVT_ABS: .rodata.str1.4+0x31b0] 
    /*00006a84*/ STR         R1, [SP, #4] 
    /*00006a88*/ MOV         R0, #3 
    /*00006a8c*/ MOV         R1, #51                     @ 0x33 
    /*00006a90*/ BL          0x0000000000006A90          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006a94*/ B           SVP_NNIE_CheckForwardParamKernel_x68c0
    SVP_NNIE_CheckForwardParamKernel_x6a98:
    /*00006a98*/ MOVW        R3, #954                    @ 0x3ba 
    /*00006a9c*/ MOV         R1, #51                     @ 0x33 
    /*00006aa0*/ STR         R3, [SP] 
    /*00006aa4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006aa8*/ LDR         R3, [PC, #684]              @ 0x0000000000006d5c 
    /*00006aac*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006ab0*/ MOV         R0, #3 
    /*00006ab4*/ BL          0x0000000000006AB4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006ab8*/ LDR         R2, [R4, #12] 
    /*00006abc*/ MOV         R1, #1 
    /*00006ac0*/ STR         R6, [SP, #4] 
    /*00006ac4*/ STR         R1, [SP, #8] 
    /*00006ac8*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006acc*/ STR         R2, [SP] 
    /*00006ad0*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006ad4*/ MOVW        R2, #0                      @ .LC84 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3228] 
    /*00006ad8*/ MOV         R1, #51                     @ 0x33 
    /*00006adc*/ MOVT        R2, #0                      @ .LC84 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3228] 
    /*00006ae0*/ MOV         R0, #3 
    /*00006ae4*/ BL          0x0000000000006AE4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006ae8*/ B           SVP_NNIE_CheckForwardParamKernel_x68c0
    SVP_NNIE_CheckForwardParamKernel_x6aec:
    /*00006aec*/ MOVW        R3, #993                    @ 0x3e1 
    /*00006af0*/ MOV         R1, #51                     @ 0x33 
    /*00006af4*/ STR         R3, [SP] 
    /*00006af8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006afc*/ LDR         R3, [PC, #600]              @ 0x0000000000006d5c 
    /*00006b00*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006b04*/ MOV         R0, #3 
    /*00006b08*/ BL          0x0000000000006B08          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006b0c*/ LDRD        R0, [R4, #16] 
    /*00006b10*/ MOV         R2, #16 
    /*00006b14*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006b18*/ STR         R2, [SP, #8] 
    /*00006b1c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006b20*/ STRD        R0, [SP] 
    /*00006b24*/ MOVW        R2, #0                      @ .LC88 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3394] 
    /*00006b28*/ MOV         R1, #51                     @ 0x33 
    /*00006b2c*/ MOVT        R2, #0                      @ .LC88 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3394] 
    /*00006b30*/ MOV         R0, #3 
    /*00006b34*/ BL          0x0000000000006B34          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006b38*/ B           SVP_NNIE_CheckForwardParamKernel_x68c0
    SVP_NNIE_CheckForwardParamKernel_x6b3c:
    /*00006b3c*/ MOVW        R3, #997                    @ 0x3e5 
    /*00006b40*/ MOV         R1, #51                     @ 0x33 
    /*00006b44*/ STR         R3, [SP] 
    /*00006b48*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006b4c*/ LDR         R3, [PC, #520]              @ 0x0000000000006d5c 
    /*00006b50*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006b54*/ MOV         R0, #3 
    /*00006b58*/ BL          0x0000000000006B58          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006b5c*/ LDRD        R0, [R4, #40]               @ 0x28 
    /*00006b60*/ MOV         R2, #16 
    /*00006b64*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006b68*/ STR         R2, [SP, #8] 
    /*00006b6c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006b70*/ STRD        R0, [SP] 
    /*00006b74*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x33e4] 
    /*00006b78*/ MOV         R1, #51                     @ 0x33 
    /*00006b7c*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x33e4] 
    /*00006b80*/ MOV         R0, #3 
    /*00006b84*/ BL          0x0000000000006B84          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006b88*/ B           SVP_NNIE_CheckForwardParamKernel_x68c0
    SVP_NNIE_CheckForwardParamKernel_x6b8c:
    /*00006b8c*/ MOVW        R3, #1159                   @ 0x487 
    /*00006b90*/ MOV         R1, #51                     @ 0x33 
    /*00006b94*/ STR         R3, [SP] 
    /*00006b98*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006b9c*/ LDR         R3, [PC, #436]              @ 0x0000000000006d58 
    /*00006ba0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006ba4*/ MOV         R0, #3 
    /*00006ba8*/ MOVW        R6, #32771                  @ 0x8003 
    /*00006bac*/ BL          0x0000000000006BAC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006bb0*/ LDR         R0, [R4, #56]               @ 0x38 
    /*00006bb4*/ LDR         R1, [SP, #20] 
    /*00006bb8*/ MOV         R3, R6 
    /*00006bbc*/ MOVW        R2, #0                      @ .LC94 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3514] 
    /*00006bc0*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006bc4*/ STR         R0, [SP] 
    /*00006bc8*/ MOVT        R2, #0                      @ .LC94 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3514] 
    /*00006bcc*/ STR         R1, [SP, #4] 
    /*00006bd0*/ MOV         R0, #3 
    /*00006bd4*/ MOV         R1, #51                     @ 0x33 
    /*00006bd8*/ MOVT        R6, #41011                  @ 0xa033 
    /*00006bdc*/ BL          0x0000000000006BDC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006be0*/ B           SVP_NNIE_CheckForwardParamKernel_x66b0
    SVP_NNIE_CheckForwardParamKernel_x6be4:
    /*00006be4*/ MOVW        R3, #949                    @ 0x3b5 
    /*00006be8*/ MOV         R1, #51                     @ 0x33 
    /*00006bec*/ STR         R3, [SP] 
    /*00006bf0*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006bf4*/ LDR         R3, [PC, #352]              @ 0x0000000000006d5c 
    /*00006bf8*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006bfc*/ MOV         R0, #3 
    /*00006c00*/ BL          0x0000000000006C00          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006c04*/ LDR         IP, [R4, #8] 
    /*00006c08*/ STR         SL, [SP, #8] 
    /*00006c0c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00006c10*/ MOVW        R2, #0                      @ .LC83 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x31ec] 
    /*00006c14*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006c18*/ MLA         R9, R9, IP, R5 
    /*00006c1c*/ MOVT        R2, #0                      @ .LC83 [R_ARM_MOVT_ABS: .rodata.str1.4+0x31ec] 
    /*00006c20*/ MOV         R1, #51                     @ 0x33 
    /*00006c24*/ MOV         R0, #3 
    /*00006c28*/ LDR         LR, [R9, #12] 
    /*00006c2c*/ STM         SP, {IP, LR} 
    /*00006c30*/ BL          0x0000000000006C30          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006c34*/ B           SVP_NNIE_CheckForwardParamKernel_x68c0
    SVP_NNIE_CheckForwardParamKernel_x6c38:
    /*00006c38*/ MOVW        R3, #1125                   @ 0x465 
    /*00006c3c*/ MOV         R1, #51                     @ 0x33 
    /*00006c40*/ STR         R3, [SP] 
    /*00006c44*/ MOVW        R6, #32774                  @ 0x8006 
    /*00006c48*/ LDR         R3, [PC, #264]              @ 0x0000000000006d58 
    /*00006c4c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006c50*/ MOV         R0, #3 
    /*00006c54*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006c58*/ BL          0x0000000000006C58          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006c5c*/ MOV         R3, R6 
    /*00006c60*/ MOVW        R2, #0                      @ .LC78 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x310c] 
    /*00006c64*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006c68*/ MOVT        R2, #0                      @ .LC78 [R_ARM_MOVT_ABS: .rodata.str1.4+0x310c] 
    /*00006c6c*/ MOV         R1, #51                     @ 0x33 
    /*00006c70*/ MOV         R0, #3 
    /*00006c74*/ MOVT        R6, #41011                  @ 0xa033 
    /*00006c78*/ BL          0x0000000000006C78          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006c7c*/ B           SVP_NNIE_CheckForwardParamKernel_x66b0
    SVP_NNIE_CheckForwardParamKernel_x6c80:
    /*00006c80*/ MOVW        R3, #1124                   @ 0x464 
    /*00006c84*/ MOV         R1, #51                     @ 0x33 
    /*00006c88*/ STR         R3, [SP] 
    /*00006c8c*/ MOVW        R6, #32774                  @ 0x8006 
    /*00006c90*/ LDR         R3, [PC, #192]              @ 0x0000000000006d58 
    /*00006c94*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006c98*/ MOV         R0, #3 
    /*00006c9c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006ca0*/ BL          0x0000000000006CA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006ca4*/ MOV         R3, R6 
    /*00006ca8*/ MOVW        R2, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30ec] 
    /*00006cac*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006cb0*/ MOVT        R2, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30ec] 
    /*00006cb4*/ MOV         R1, #51                     @ 0x33 
    /*00006cb8*/ MOV         R0, #3 
    /*00006cbc*/ MOVT        R6, #41011                  @ 0xa033 
    /*00006cc0*/ BL          0x0000000000006CC0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006cc4*/ B           SVP_NNIE_CheckForwardParamKernel_x66b0
    SVP_NNIE_CheckForwardParamKernel_x6cc8:
    /*00006cc8*/ MOVW        R3, #1126                   @ 0x466 
    /*00006ccc*/ MOV         R1, #51                     @ 0x33 
    /*00006cd0*/ STR         R3, [SP] 
    /*00006cd4*/ MOVW        R6, #32774                  @ 0x8006 
    /*00006cd8*/ LDR         R3, [PC, #120]              @ 0x0000000000006d58 
    /*00006cdc*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006ce0*/ MOV         R0, #3 
    /*00006ce4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006ce8*/ BL          0x0000000000006CE8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006cec*/ MOV         R3, R6 
    /*00006cf0*/ MOVW        R2, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3128] 
    /*00006cf4*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006cf8*/ MOVT        R2, #0                      @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3128] 
    /*00006cfc*/ MOV         R1, #51                     @ 0x33 
    /*00006d00*/ MOV         R0, #3 
    /*00006d04*/ MOVT        R6, #41011                  @ 0xa033 
    /*00006d08*/ BL          0x0000000000006D08          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006d0c*/ B           SVP_NNIE_CheckForwardParamKernel_x66b0
    SVP_NNIE_CheckForwardParamKernel_x6d10:
    /*00006d10*/ MOVW        R3, #1123                   @ 0x463 
    /*00006d14*/ MOV         R1, #51                     @ 0x33 
    /*00006d18*/ STR         R3, [SP] 
    /*00006d1c*/ MOVW        R6, #32774                  @ 0x8006 
    /*00006d20*/ LDR         R3, [PC, #48]               @ 0x0000000000006d58 
    /*00006d24*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00006d28*/ MOV         R0, #3 
    /*00006d2c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00006d30*/ BL          0x0000000000006D30          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006d34*/ MOV         R3, R6 
    /*00006d38*/ MOVW        R2, #0                      @ .LC76 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30d0] 
    /*00006d3c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00006d40*/ MOVT        R2, #0                      @ .LC76 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30d0] 
    /*00006d44*/ MOV         R1, #51                     @ 0x33 
    /*00006d48*/ MOV         R0, #3 
    /*00006d4c*/ MOVT        R6, #41011                  @ 0xa033 
    /*00006d50*/ BL          0x0000000000006D50          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006d54*/ B           SVP_NNIE_CheckForwardParamKernel_x66b0
    /*00006d58*/ STRDEQ      R0, [R0], -R8 
    /*00006d5c*/ ANDEQ       R0, R0, IP, LSL R2 
FUNC_END SVP_NNIE_CheckForwardParamKernel


FUNC_BEGIN SVP_NNIE_CheckForwardWithBboxParamKernel
    /*00006d60*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00006d64*/ CMP         R0, #0 
    /*00006d68*/ SUB         SP, SP, #60                 @ 0x3c 
    /*00006d6c*/ MOV         R4, R3 
    /*00006d70*/ MOV         R3, #0 
    /*00006d74*/ STR         R0, [SP, #28] 
    /*00006d78*/ STR         R1, [SP, #24] 
    /*00006d7c*/ LDR         R8, [SP, #96]               @ 0x60 
    /*00006d80*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00006d84*/ STR         R3, [SP, #48]               @ 0x30 
    /*00006d88*/ STR         R3, [SP, #52]               @ 0x34 
    /*00006d8c*/ LDR         R6, [SP, #100]              @ 0x64 
    /*00006d90*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x7d78
    /*00006d94*/ CMP         R1, #0 
    /*00006d98*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x7d30
    /*00006d9c*/ CMP         R2, #0 
    /*00006da0*/ MOV         R9, R2 
    /*00006da4*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x7ce8
    /*00006da8*/ CMP         R4, #0 
    /*00006dac*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x7ca0
    /*00006db0*/ CMP         R8, #0 
    /*00006db4*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x7dc0
    /*00006db8*/ CMP         R6, #1 
    /*00006dbc*/ BHI         SVP_NNIE_CheckForwardWithBboxParamKernel_x7170
    /*00006dc0*/ MOV         R0, R2 
    /*00006dc4*/ BL          0x0000000000004B2C 
    /*00006dc8*/ CMP         R0, #0 
    /*00006dcc*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x789c
    /*00006dd0*/ LDR         R3, [R8, #12] 
    /*00006dd4*/ LDR         R2, [R9, #8] 
    /*00006dd8*/ CMP         R3, R2 
    /*00006ddc*/ BCS         SVP_NNIE_CheckForwardWithBboxParamKernel_x7b20
    /*00006de0*/ MOVW        R6, #1692                   @ 0x69c 
    /*00006de4*/ MLA         R2, R6, R3, R9 
    /*00006de8*/ LDR         R2, [R2, #12] 
    /*00006dec*/ CMP         R2, #1 
    /*00006df0*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x7844
    /*00006df4*/ LDR         R2, [R8, #16] 
    /*00006df8*/ CMP         R2, #0 
    /*00006dfc*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x7b78
    /*00006e00*/ MOVW        R6, #1692                   @ 0x69c 
    /*00006e04*/ LDR         R1, [R8] 
    /*00006e08*/ MLA         R3, R6, R3, R9 
    /*00006e0c*/ LDRH        R2, [R3, #16] 
    /*00006e10*/ CMP         R1, R2 
    /*00006e14*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x78e8
    /*00006e18*/ LDRH        R2, [R3, #18] 
    /*00006e1c*/ LDR         R1, [R8, #4] 
    /*00006e20*/ CMP         R1, R2 
    /*00006e24*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x793c
    /*00006e28*/ LDR         R2, [R8, #8] 
    /*00006e2c*/ CMP         R2, #0 
    /*00006e30*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x77e8
    /*00006e34*/ LDRH        R3, [R3, #20] 
    /*00006e38*/ CMP         R2, R3 
    /*00006e3c*/ BHI         SVP_NNIE_CheckForwardWithBboxParamKernel_x77e8
    /*00006e40*/ MOVNE       R3, #1 
    /*00006e44*/ MOVEQ       R3, #0 
    /*00006e48*/ CMP         R2, #1 
    /*00006e4c*/ MOVEQ       R2, #0 
    /*00006e50*/ ANDNE       R2, R3, #1 
    /*00006e54*/ CMP         R2, #0 
    /*00006e58*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x73c0
    /*00006e5c*/ LDR         R6, [R8, #40]               @ 0x28 
    /*00006e60*/ LDR         R3, [R9, #4] 
    /*00006e64*/ CMP         R6, R3 
    /*00006e68*/ BCC         SVP_NNIE_CheckForwardWithBboxParamKernel_x7a20
    /*00006e6c*/ LDR         R7, [R8, #24] 
    /*00006e70*/ MOV         R1, R6 
    /*00006e74*/ MOV         R0, R7 
    /*00006e78*/ BL          0x0000000000006E78          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00006e7c*/ CMP         R0, #0 
    /*00006e80*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x710c
    SVP_NNIE_CheckForwardWithBboxParamKernel_x6e84:
    /*00006e84*/ LDRD        R2, [R8, #24] 
    /*00006e88*/ MOV         R3, #0 
    /*00006e8c*/ AND         R2, R2, #15 
    /*00006e90*/ ORRS        R3, R2, R3 
    /*00006e94*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x7c00
    /*00006e98*/ LDRD        R2, [R8, #48]               @ 0x30 
    /*00006e9c*/ MOV         R3, #0 
    /*00006ea0*/ AND         R2, R2, #15 
    /*00006ea4*/ ORRS        R3, R2, R3 
    /*00006ea8*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x7c50
    /*00006eac*/ LDR         R3, [R8, #12] 
    /*00006eb0*/ MOVW        R6, #1692                   @ 0x69c 
    /*00006eb4*/ MLA         R2, R6, R3, R9 
    /*00006eb8*/ LDRH        R2, [R2, #20] 
    /*00006ebc*/ CMP         R2, #0 
    /*00006ec0*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x7ac8
    /*00006ec4*/ LDR         R2, [SP, #28] 
    /*00006ec8*/ LDR         R2, [R2, #24] 
    /*00006ecc*/ CMP         R2, #1 
    /*00006ed0*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x71c4
    /*00006ed4*/ MOV         R2, R9 
    /*00006ed8*/ LDR         R1, [R8] 
    /*00006edc*/ LDR         R0, [SP, #28] 
    /*00006ee0*/ BL          0x0000000000003D88 
    /*00006ee4*/ CMP         R0, #0 
    /*00006ee8*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x7990
    /*00006eec*/ MOV         R0, R4 
    /*00006ef0*/ LDR         R3, [R8, #12] 
    /*00006ef4*/ MOV         R2, R9 
    /*00006ef8*/ LDR         R1, [R8, #4] 
    /*00006efc*/ BL          0x00000000000044A8 
    /*00006f00*/ CMP         R0, #0 
    /*00006f04*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x79d8
    /*00006f08*/ LDR         R3, [R8, #8] 
    /*00006f0c*/ LDR         R2, [R8, #12] 
    /*00006f10*/ CMP         R3, #0 
    /*00006f14*/ MOV         R1, R3 
    /*00006f18*/ STR         R2, [SP, #32] 
    /*00006f1c*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x7b70
    /*00006f20*/ LDR         R2, [SP, #24] 
    /*00006f24*/ LDR         R3, [R2] 
    /*00006f28*/ CMP         R3, #0 
    /*00006f2c*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x7bd0
    /*00006f30*/ LDR         IP, [R2, #4] 
    /*00006f34*/ CMP         IP, #0 
    /*00006f38*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x7bdc
    /*00006f3c*/ ANDS        R7, IP, #15 
    /*00006f40*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x7be8
    /*00006f44*/ CMP         IP, #15 
    /*00006f48*/ BLS         SVP_NNIE_CheckForwardWithBboxParamKernel_x7bf4
    /*00006f4c*/ LDR         R3, [SP, #32] 
    /*00006f50*/ MOVW        FP, #1692                   @ 0x69c 
    /*00006f54*/ MOV         R5, #0 
    /*00006f58*/ STR         R7, [SP, #36]               @ 0x24 
    /*00006f5c*/ STR         R8, [SP, #96]               @ 0x60 
    /*00006f60*/ MOV         R6, R2 
    /*00006f64*/ MLA         FP, FP, R3, FP 
    /*00006f68*/ MOV         R7, R1 
    /*00006f6c*/ MOV         R8, R5 
    /*00006f70*/ ADD         FP, R9, FP 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x6f74:
    /*00006f74*/ LDR         R0, [R6, #24] 
    /*00006f78*/ LDR         R1, [R6, #40]               @ 0x28 
    /*00006f7c*/ LDR         R4, [R6, #36]               @ 0x24 
    /*00006f80*/ LDR         SL, [R6, #16] 
    /*00006f84*/ MUL         R1, R1, R0 
    /*00006f88*/ MOV         R0, SL 
    /*00006f8c*/ MUL         R4, R4, R1 
    /*00006f90*/ MUL         R4, IP, R4 
    /*00006f94*/ MOV         R1, R4 
    /*00006f98*/ BL          0x0000000000006F98          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00006f9c*/ CMP         R0, #0 
    /*00006fa0*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x70a8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x6fa4:
    /*00006fa4*/ LDRD        R0, [R6, #16] 
    /*00006fa8*/ MOV         R1, #0 
    /*00006fac*/ AND         R0, R0, #15 
    /*00006fb0*/ ORRS        R3, R0, R1 
    /*00006fb4*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x7228
    /*00006fb8*/ LDR         R4, [R6, #24] 
    /*00006fbc*/ CMP         R4, #1 
    /*00006fc0*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x72c8
    /*00006fc4*/ EOR         R5, R5, #1 
    /*00006fc8*/ CMP         R8, #0 
    /*00006fcc*/ MOVEQ       R5, #0 
    /*00006fd0*/ ANDNE       R5, R5, #1 
    /*00006fd4*/ CMP         R5, #0 
    /*00006fd8*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x731c
    /*00006fdc*/ LDR         R5, [R6, #40]               @ 0x28 
    /*00006fe0*/ CMP         R5, #1 
    /*00006fe4*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x7370
    /*00006fe8*/ LDR         R2, [R6, #32] 
    /*00006fec*/ CMP         R2, #4 
    /*00006ff0*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x7468
    /*00006ff4*/ LDR         R2, [FP, #4]! 
    /*00006ff8*/ MOV         R3, #104                    @ 0x68 
    /*00006ffc*/ LDR         R1, [R6, #36]               @ 0x24 
    /*00007000*/ MLA         R2, R3, R2, R9 
    /*00007004*/ MOVW        R3, #13596                  @ 0x351c 
    /*00007008*/ LDR         R2, [R2, R3] 
    /*0000700c*/ CMP         R1, R2 
    /*00007010*/ BHI         SVP_NNIE_CheckForwardWithBboxParamKernel_x74bc
    /*00007014*/ CMP         R1, #0 
    /*00007018*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x769c
    /*0000701c*/ ADD         R8, R8, #1 
    /*00007020*/ CMP         R7, R8 
    /*00007024*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x7530
    /*00007028*/ LDR         R2, [R6, #48]!              @ 0x30 
    /*0000702c*/ CMP         R2, #0 
    /*00007030*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x76e8
    /*00007034*/ LDR         IP, [R6, #4] 
    /*00007038*/ CMP         IP, #0 
    /*0000703c*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x7740
    /*00007040*/ TST         IP, #15 
    /*00007044*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x7790
    /*00007048*/ CMP         IP, #15 
    /*0000704c*/ BHI         SVP_NNIE_CheckForwardWithBboxParamKernel_x6f74
    /*00007050*/ MOV         SL, R8 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7054:
    /*00007054*/ MOVW        R3, #709                    @ 0x2c5 
    /*00007058*/ MOV         R1, #51                     @ 0x33 
    /*0000705c*/ STR         R3, [SP] 
    /*00007060*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007064*/ LDR         R3, [PC, #3504]             @ 0x0000000000007e1c 
    /*00007068*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*0000706c*/ MOV         R0, #3 
    /*00007070*/ BL          0x0000000000007070          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007074*/ LDR         R2, [R6, #4] 
    /*00007078*/ MOV         R1, #16 
    /*0000707c*/ STR         SL, [SP] 
    /*00007080*/ STR         R1, [SP, #8] 
    /*00007084*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007088*/ STR         R2, [SP, #4] 
    /*0000708c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007090*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b24] 
    /*00007094*/ MOV         R1, #51                     @ 0x33 
    /*00007098*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b24] 
    /*0000709c*/ MOV         R0, #3 
    /*000070a0*/ BL          0x00000000000070A0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000070a4*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7278
    SVP_NNIE_CheckForwardWithBboxParamKernel_x70a8:
    /*000070a8*/ MOV         R0, SL 
    /*000070ac*/ MOV         R1, R4 
    /*000070b0*/ BL          0x00000000000070B0          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*000070b4*/ CMP         R0, #0 
    /*000070b8*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x6fa4
    /*000070bc*/ MOVW        R3, #721                    @ 0x2d1 
    /*000070c0*/ MOV         R1, #51                     @ 0x33 
    /*000070c4*/ STR         R3, [SP] 
    /*000070c8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000070cc*/ LDR         R3, [PC, #3400]             @ 0x0000000000007e1c 
    /*000070d0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000070d4*/ MOV         R0, #3 
    /*000070d8*/ BL          0x00000000000070D8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000070dc*/ LDRD        R0, [R6, #16] 
    /*000070e0*/ MOVW        R3, #32771                  @ 0x8003 
    /*000070e4*/ STR         R4, [SP, #16] 
    /*000070e8*/ STR         R8, [SP] 
    /*000070ec*/ MOVT        R3, #41011                  @ 0xa033 
    /*000070f0*/ STRD        R0, [SP, #8] 
    /*000070f4*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c6c] 
    /*000070f8*/ MOV         R1, #51                     @ 0x33 
    /*000070fc*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c6c] 
    /*00007100*/ MOV         R0, #3 
    /*00007104*/ BL          0x0000000000007104          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007108*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7278
    SVP_NNIE_CheckForwardWithBboxParamKernel_x710c:
    /*0000710c*/ MOV         R1, R6 
    /*00007110*/ MOV         R0, R7 
    /*00007114*/ BL          0x0000000000007114          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*00007118*/ CMP         R0, #0 
    /*0000711c*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x6e84
    /*00007120*/ MOV         R3, #1072                   @ 0x430 
    /*00007124*/ MOV         R1, #51                     @ 0x33 
    /*00007128*/ STR         R3, [SP] 
    /*0000712c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007130*/ LDR         R3, [PC, #3304]             @ 0x0000000000007e20 
    /*00007134*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007138*/ MOV         R0, #3 
    /*0000713c*/ BL          0x000000000000713C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007140*/ LDRD        R0, [R8, #24] 
    /*00007144*/ LDR         IP, [R8, #40]               @ 0x28 
    /*00007148*/ MOVW        R3, #32771                  @ 0x8003 
    /*0000714c*/ MOVW        R2, #0                      @ .LC95 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x356c] 
    /*00007150*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007154*/ STRD        R0, [SP] 
    /*00007158*/ MOVT        R2, #0                      @ .LC95 [R_ARM_MOVT_ABS: .rodata.str1.4+0x356c] 
    /*0000715c*/ STR         IP, [SP, #8] 
    /*00007160*/ MOV         R1, #51                     @ 0x33 
    /*00007164*/ MOV         R0, #3 
    /*00007168*/ BL          0x0000000000007168          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000716c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7414
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7170:
    /*00007170*/ MOVW        R3, #1207                   @ 0x4b7 
    /*00007174*/ MOV         R1, #51                     @ 0x33 
    /*00007178*/ STR         R3, [SP] 
    /*0000717c*/ MOVW        R4, #32771                  @ 0x8003 
    /*00007180*/ LDR         R3, [PC, #3228]             @ 0x0000000000007e24 
    /*00007184*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007188*/ MOV         R0, #3 
    /*0000718c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007190*/ BL          0x0000000000007190          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007194*/ MOV         R3, R4 
    /*00007198*/ STR         R6, [SP] 
    /*0000719c*/ MOVW        R2, #0                      @ .LC80 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x314c] 
    /*000071a0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000071a4*/ MOVT        R2, #0                      @ .LC80 [R_ARM_MOVT_ABS: .rodata.str1.4+0x314c] 
    /*000071a8*/ MOV         R1, #51                     @ 0x33 
    /*000071ac*/ MOV         R0, #3 
    /*000071b0*/ BL          0x00000000000071B0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000071b4*/ MOVT        R4, #41011                  @ 0xa033 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8:
    /*000071b8*/ MOV         R0, R4 
    /*000071bc*/ ADD         SP, SP, #60                 @ 0x3c 
    /*000071c0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x71c4:
    /*000071c4*/ MOVW        R3, #1225                   @ 0x4c9 
    /*000071c8*/ MOV         R1, #51                     @ 0x33 
    /*000071cc*/ STR         R3, [SP] 
    /*000071d0*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000071d4*/ LDR         R3, [PC, #3144]             @ 0x0000000000007e24 
    /*000071d8*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000071dc*/ MOV         R0, #3 
    /*000071e0*/ MOVW        R4, #32771                  @ 0x8003 
    /*000071e4*/ BL          0x00000000000071E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000071e8*/ MOV         R3, #1 
    /*000071ec*/ STR         R3, [SP, #8] 
    /*000071f0*/ MOV         R2, #0 
    /*000071f4*/ LDR         R3, [SP, #28] 
    /*000071f8*/ MOV         R1, #51                     @ 0x33 
    /*000071fc*/ LDR         R0, [R3, #24] 
    /*00007200*/ MOV         R3, R4 
    /*00007204*/ STR         R2, [SP] 
    /*00007208*/ MOVT        R3, #41011                  @ 0xa033 
    /*0000720c*/ MOVW        R2, #0                      @ .LC103 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3814] 
    /*00007210*/ MOVT        R2, #0                      @ .LC103 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3814] 
    /*00007214*/ STR         R0, [SP, #4] 
    /*00007218*/ MOV         R0, #3 
    /*0000721c*/ MOVT        R4, #41011                  @ 0xa033 
    /*00007220*/ BL          0x0000000000007220          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007224*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7228:
    /*00007228*/ MOV         R3, #724                    @ 0x2d4 
    /*0000722c*/ MOV         R1, #51                     @ 0x33 
    /*00007230*/ STR         R3, [SP] 
    /*00007234*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007238*/ LDR         R3, [PC, #3036]             @ 0x0000000000007e1c 
    /*0000723c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007240*/ MOV         R0, #3 
    /*00007244*/ BL          0x0000000000007244          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007248*/ LDRD        R0, [R6, #16] 
    /*0000724c*/ MOV         R2, #16 
    /*00007250*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007254*/ STR         R2, [SP, #16] 
    /*00007258*/ MOVT        R3, #41011                  @ 0xa033 
    /*0000725c*/ STRD        R0, [SP, #8] 
    /*00007260*/ MOVW        R2, #0                      @ .LC105 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3874] 
    /*00007264*/ STR         R8, [SP] 
    /*00007268*/ MOV         R1, #51                     @ 0x33 
    /*0000726c*/ MOVT        R2, #0                      @ .LC105 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3874] 
    /*00007270*/ MOV         R0, #3 
    /*00007274*/ BL          0x0000000000007274          @ HI_LOG [R_ARM_CALL: +0x0] 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7278:
    /*00007278*/ MOVW        R3, #1238                   @ 0x4d6 
    /*0000727c*/ MOV         R1, #51                     @ 0x33 
    /*00007280*/ STR         R3, [SP] 
    /*00007284*/ MOVW        R4, #32771                  @ 0x8003 
    /*00007288*/ LDR         R3, [PC, #2964]             @ 0x0000000000007e24 
    /*0000728c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007290*/ MOV         R0, #3 
    /*00007294*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007298*/ BL          0x0000000000007298          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000729c*/ MOV         R3, R4 
    /*000072a0*/ MOV         R1, #51                     @ 0x33 
    /*000072a4*/ MOVT        R3, #41011                  @ 0xa033 
    /*000072a8*/ MOVW        R2, #0                      @ .LC112 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3a70] 
    /*000072ac*/ MOV         R0, #3 
    /*000072b0*/ MOVT        R2, #0                      @ .LC112 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3a70] 
    /*000072b4*/ MOVT        R4, #41011                  @ 0xa033 
    /*000072b8*/ BL          0x00000000000072B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000072bc*/ MOV         R0, R4 
    /*000072c0*/ ADD         SP, SP, #60                 @ 0x3c 
    /*000072c4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x72c8:
    /*000072c8*/ MOVW        R3, #729                    @ 0x2d9 
    /*000072cc*/ MOV         R1, #51                     @ 0x33 
    /*000072d0*/ STR         R3, [SP] 
    /*000072d4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000072d8*/ LDR         R3, [PC, #2876]             @ 0x0000000000007e1c 
    /*000072dc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000072e0*/ MOV         R0, #3 
    /*000072e4*/ BL          0x00000000000072E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000072e8*/ LDR         R2, [R6, #24] 
    /*000072ec*/ MOV         R1, #1 
    /*000072f0*/ STR         R8, [SP] 
    /*000072f4*/ STR         R1, [SP, #8] 
    /*000072f8*/ MOVW        R3, #32771                  @ 0x8003 
    /*000072fc*/ STR         R2, [SP, #4] 
    /*00007300*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007304*/ MOVW        R2, #0                      @ .LC106 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x38b8] 
    /*00007308*/ MOV         R1, #51                     @ 0x33 
    /*0000730c*/ MOVT        R2, #0                      @ .LC106 [R_ARM_MOVT_ABS: .rodata.str1.4+0x38b8] 
    /*00007310*/ MOV         R0, #3 
    /*00007314*/ BL          0x0000000000007314          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007318*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7278
    SVP_NNIE_CheckForwardWithBboxParamKernel_x731c:
    /*0000731c*/ MOVW        R3, #739                    @ 0x2e3 
    /*00007320*/ MOV         R1, #51                     @ 0x33 
    /*00007324*/ STR         R3, [SP] 
    /*00007328*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*0000732c*/ LDR         R3, [PC, #2792]             @ 0x0000000000007e1c 
    /*00007330*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007334*/ MOV         R0, #3 
    /*00007338*/ BL          0x0000000000007338          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000733c*/ LDR         R2, [R6, #24] 
    /*00007340*/ MOV         R1, #0 
    /*00007344*/ STR         R8, [SP] 
    /*00007348*/ STR         R1, [SP, #8] 
    /*0000734c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007350*/ STR         R2, [SP, #4] 
    /*00007354*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007358*/ MOVW        R2, #0                      @ .LC107 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x38fc] 
    /*0000735c*/ MOV         R1, #51                     @ 0x33 
    /*00007360*/ MOVT        R2, #0                      @ .LC107 [R_ARM_MOVT_ABS: .rodata.str1.4+0x38fc] 
    /*00007364*/ MOV         R0, #3 
    /*00007368*/ BL          0x0000000000007368          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000736c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7278
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7370:
    /*00007370*/ MOVW        R3, #745                    @ 0x2e9 
    /*00007374*/ MOV         R1, #51                     @ 0x33 
    /*00007378*/ STR         R3, [SP] 
    /*0000737c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007380*/ LDR         R3, [PC, #2708]             @ 0x0000000000007e1c 
    /*00007384*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007388*/ MOV         R0, #3 
    /*0000738c*/ BL          0x000000000000738C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007390*/ LDR         R2, [R6, #40]               @ 0x28 
    /*00007394*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007398*/ STR         R4, [SP, #8] 
    /*0000739c*/ STR         R8, [SP] 
    /*000073a0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000073a4*/ STR         R2, [SP, #4] 
    /*000073a8*/ MOV         R1, #51                     @ 0x33 
    /*000073ac*/ MOVW        R2, #0                      @ .LC108 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x395c] 
    /*000073b0*/ MOV         R0, #3 
    /*000073b4*/ MOVT        R2, #0                      @ .LC108 [R_ARM_MOVT_ABS: .rodata.str1.4+0x395c] 
    /*000073b8*/ BL          0x00000000000073B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000073bc*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7278
    SVP_NNIE_CheckForwardWithBboxParamKernel_x73c0:
    /*000073c0*/ MOVW        R3, #1059                   @ 0x423 
    /*000073c4*/ MOV         R1, #51                     @ 0x33 
    /*000073c8*/ STR         R3, [SP] 
    /*000073cc*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000073d0*/ LDR         R3, [PC, #2632]             @ 0x0000000000007e20 
    /*000073d4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000073d8*/ MOV         R0, #3 
    /*000073dc*/ BL          0x00000000000073DC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000073e0*/ LDR         R1, [R8, #12] 
    /*000073e4*/ LDR         IP, [R8, #8] 
    /*000073e8*/ MOVW        R3, #32771                  @ 0x8003 
    /*000073ec*/ MOVW        R2, #0                      @ .LC101 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3738] 
    /*000073f0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000073f4*/ MLA         R6, R6, R1, R9 
    /*000073f8*/ STR         IP, [SP, #8] 
    /*000073fc*/ MOVT        R2, #0                      @ .LC101 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3738] 
    /*00007400*/ MOV         R1, #51                     @ 0x33 
    /*00007404*/ MOV         R0, #3 
    /*00007408*/ LDRH        LR, [R6, #20] 
    /*0000740c*/ STM         SP, {IP, LR} 
    /*00007410*/ BL          0x0000000000007410          @ HI_LOG [R_ARM_CALL: +0x0] 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7414:
    /*00007414*/ MOVW        R3, #1217                   @ 0x4c1 
    /*00007418*/ MOV         R1, #51                     @ 0x33 
    /*0000741c*/ STR         R3, [SP] 
    /*00007420*/ MOVW        R4, #32771                  @ 0x8003 
    /*00007424*/ LDR         R3, [PC, #2552]             @ 0x0000000000007e24 
    /*00007428*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*0000742c*/ MOV         R0, #3 
    /*00007430*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007434*/ BL          0x0000000000007434          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007438*/ STR         R9, [SP] 
    /*0000743c*/ MOV         R3, R4 
    /*00007440*/ MOV         R1, #51                     @ 0x33 
    /*00007444*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007448*/ MOVW        R2, #0                      @ .LC96 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x35e4] 
    /*0000744c*/ MOV         R0, #3 
    /*00007450*/ MOVT        R2, #0                      @ .LC96 [R_ARM_MOVT_ABS: .rodata.str1.4+0x35e4] 
    /*00007454*/ BL          0x0000000000007454          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007458*/ MOVT        R4, #41011                  @ 0xa033 
    /*0000745c*/ MOV         R0, R4 
    /*00007460*/ ADD         SP, SP, #60                 @ 0x3c 
    /*00007464*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7468:
    /*00007468*/ MOVW        R3, #749                    @ 0x2ed 
    /*0000746c*/ MOV         R1, #51                     @ 0x33 
    /*00007470*/ STR         R3, [SP] 
    /*00007474*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007478*/ LDR         R3, [PC, #2460]             @ 0x0000000000007e1c 
    /*0000747c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007480*/ MOV         R0, #3 
    /*00007484*/ BL          0x0000000000007484          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007488*/ LDR         R2, [R6, #32] 
    /*0000748c*/ MOV         R1, #4 
    /*00007490*/ STR         R8, [SP] 
    /*00007494*/ STR         R1, [SP, #8] 
    /*00007498*/ MOVW        R3, #32771                  @ 0x8003 
    /*0000749c*/ STR         R2, [SP, #4] 
    /*000074a0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000074a4*/ MOVW        R2, #0                      @ .LC109 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3998] 
    /*000074a8*/ MOV         R1, #51                     @ 0x33 
    /*000074ac*/ MOVT        R2, #0                      @ .LC109 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3998] 
    /*000074b0*/ MOV         R0, #3 
    /*000074b4*/ BL          0x00000000000074B4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000074b8*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7278
    SVP_NNIE_CheckForwardWithBboxParamKernel_x74bc:
    /*000074bc*/ MOVW        R3, #754                    @ 0x2f2 
    /*000074c0*/ MOV         R1, #51                     @ 0x33 
    /*000074c4*/ STR         R3, [SP] 
    /*000074c8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000074cc*/ LDR         R3, [PC, #2376]             @ 0x0000000000007e1c 
    /*000074d0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000074d4*/ MOV         R0, #3 
    /*000074d8*/ MOV         R4, #104                    @ 0x68 
    /*000074dc*/ BL          0x00000000000074DC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000074e0*/ LDR         R3, [SP, #32] 
    /*000074e4*/ MOVW        IP, #423                    @ 0x1a7 
    /*000074e8*/ LDR         LR, [R6, #36]               @ 0x24 
    /*000074ec*/ MOV         SL, R8 
    /*000074f0*/ MOVW        R2, #0                      @ .LC110 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x39d8] 
    /*000074f4*/ MLA         IP, IP, R3, R8 
    /*000074f8*/ MOVT        R2, #0                      @ .LC110 [R_ARM_MOVT_ABS: .rodata.str1.4+0x39d8] 
    /*000074fc*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007500*/ MOV         R1, #51                     @ 0x33 
    /*00007504*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007508*/ MOV         R0, #3 
    /*0000750c*/ ADD         IP, R9, IP, LSL #2 
    /*00007510*/ LDR         IP, [IP, #1696]             @ 0x6a0 
    /*00007514*/ MLA         R9, R4, IP, R9 
    /*00007518*/ MOVW        IP, #13596                  @ 0x351c 
    /*0000751c*/ LDR         IP, [R9, IP] 
    /*00007520*/ STM         SP, {SL, LR} 
    /*00007524*/ STR         IP, [SP, #8] 
    /*00007528*/ BL          0x0000000000007528          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000752c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7278
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7530:
    /*00007530*/ LDR         R8, [SP, #96]               @ 0x60 
    /*00007534*/ LDR         R7, [SP, #36]               @ 0x24 
    /*00007538*/ LDR         R2, [R8, #8] 
    /*0000753c*/ CMP         R2, #0 
    /*00007540*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x7e08
    /*00007544*/ LDR         R5, [SP, #24] 
    /*00007548*/ MOV         R3, #48                     @ 0x30 
    /*0000754c*/ MLA         R2, R3, R2, R5 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7550:
    /*00007550*/ LDR         R3, [R5, #36]               @ 0x24 
    /*00007554*/ ADD         R5, R5, #48                 @ 0x30 
    /*00007558*/ CMP         R7, R3 
    /*0000755c*/ MOVCC       R7, R3 
    /*00007560*/ CMP         R5, R2 
    /*00007564*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x7550
    /*00007568*/ LDR         R3, [R8, #12] 
    /*0000756c*/ MOV         R2, R3 
    /*00007570*/ STR         R3, [SP, #32] 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7574:
    /*00007574*/ LDR         R3, [SP, #28] 
    /*00007578*/ MOV         R1, R7 
    /*0000757c*/ ADD         LR, SP, #52                 @ 0x34 
    /*00007580*/ ADD         IP, SP, #48                 @ 0x30 
    /*00007584*/ LDR         R0, [R3, #24] 
    /*00007588*/ MOV         R3, R9 
    /*0000758c*/ STMIB       SP, {IP, LR} 
    /*00007590*/ ADD         IP, SP, #44                 @ 0x2c 
    /*00007594*/ STR         IP, [SP] 
    /*00007598*/ BL          0x0000000000003974 
    /*0000759c*/ LDR         R3, [SP, #48]               @ 0x30 
    /*000075a0*/ LDR         R1, [SP, #52]               @ 0x34 
    /*000075a4*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*000075a8*/ ADD         R3, R3, R1 
    /*000075ac*/ ADD         R3, R3, R2 
    /*000075b0*/ STR         R3, [SP, #44]               @ 0x2c 
    /*000075b4*/ SUBS        R4, R0, #0 
    /*000075b8*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x7660
    /*000075bc*/ LDR         R5, [R8, #64]               @ 0x40 
    /*000075c0*/ CMP         R3, R5 
    /*000075c4*/ BHI         SVP_NNIE_CheckForwardWithBboxParamKernel_x7a70
    /*000075c8*/ LDR         R6, [R8, #48]               @ 0x30 
    /*000075cc*/ MOV         R1, R5 
    /*000075d0*/ MOV         R0, R6 
    /*000075d4*/ BL          0x00000000000075D4          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*000075d8*/ CMP         R0, #0 
    /*000075dc*/ BNE         SVP_NNIE_CheckForwardWithBboxParamKernel_x75f4
    SVP_NNIE_CheckForwardWithBboxParamKernel_x75e0:
    /*000075e0*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*000075e4*/ MOV         R0, R4 
    /*000075e8*/ STR         R3, [R8, #64]               @ 0x40 
    /*000075ec*/ ADD         SP, SP, #60                 @ 0x3c 
    /*000075f0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x75f4:
    /*000075f4*/ MOV         R1, R5 
    /*000075f8*/ MOV         R0, R6 
    /*000075fc*/ BL          0x00000000000075FC          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*00007600*/ CMP         R0, #0 
    /*00007604*/ BEQ         SVP_NNIE_CheckForwardWithBboxParamKernel_x75e0
    /*00007608*/ MOVW        R3, #1257                   @ 0x4e9 
    /*0000760c*/ MOV         R1, #51                     @ 0x33 
    /*00007610*/ STR         R3, [SP] 
    /*00007614*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007618*/ LDR         R3, [PC, #2052]             @ 0x0000000000007e24 
    /*0000761c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007620*/ MOV         R0, #3 
    /*00007624*/ MOVW        R4, #32771                  @ 0x8003 
    /*00007628*/ BL          0x0000000000007628          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000762c*/ LDRD        R0, [R8, #48]               @ 0x30 
    /*00007630*/ LDR         IP, [R8, #64]               @ 0x40 
    /*00007634*/ MOV         R3, R4 
    /*00007638*/ MOVW        R2, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3618] 
    /*0000763c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007640*/ STRD        R0, [SP] 
    /*00007644*/ MOVT        R2, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3618] 
    /*00007648*/ STR         IP, [SP, #8] 
    /*0000764c*/ MOV         R1, #51                     @ 0x33 
    /*00007650*/ MOV         R0, #3 
    /*00007654*/ MOVT        R4, #41011                  @ 0xa033 
    /*00007658*/ BL          0x0000000000007658          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000765c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7660:
    /*00007660*/ MOVW        R3, #1250                   @ 0x4e2 
    /*00007664*/ MOV         R1, #51                     @ 0x33 
    /*00007668*/ STR         R3, [SP] 
    /*0000766c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007670*/ LDR         R3, [PC, #1964]             @ 0x0000000000007e24 
    /*00007674*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007678*/ MOV         R0, #3 
    /*0000767c*/ BL          0x000000000000767C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007680*/ MOV         R3, R4 
    /*00007684*/ MOVW        R2, #0                      @ .LC93 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x34dc] 
    /*00007688*/ MOV         R1, #51                     @ 0x33 
    /*0000768c*/ MOVT        R2, #0                      @ .LC93 [R_ARM_MOVT_ABS: .rodata.str1.4+0x34dc] 
    /*00007690*/ MOV         R0, #3 
    /*00007694*/ BL          0x0000000000007694          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007698*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x769c:
    /*0000769c*/ MOVW        R3, #757                    @ 0x2f5 
    /*000076a0*/ MOV         R1, #51                     @ 0x33 
    /*000076a4*/ STR         R3, [SP] 
    /*000076a8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000076ac*/ LDR         R3, [PC, #1896]             @ 0x0000000000007e1c 
    /*000076b0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000076b4*/ MOV         R0, #3 
    /*000076b8*/ BL          0x00000000000076B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000076bc*/ LDR         R1, [R6, #36]               @ 0x24 
    /*000076c0*/ MOVW        R3, #32771                  @ 0x8003 
    /*000076c4*/ STR         R8, [SP] 
    /*000076c8*/ MOVW        R2, #0                      @ .LC111 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3a30] 
    /*000076cc*/ MOVT        R3, #41011                  @ 0xa033 
    /*000076d0*/ STR         R1, [SP, #4] 
    /*000076d4*/ MOVT        R2, #0                      @ .LC111 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3a30] 
    /*000076d8*/ MOV         R1, #51                     @ 0x33 
    /*000076dc*/ MOV         R0, #3 
    /*000076e0*/ BL          0x00000000000076E0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000076e4*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7278
    SVP_NNIE_CheckForwardWithBboxParamKernel_x76e8:
    /*000076e8*/ MOV         SL, R8 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x76ec:
    /*000076ec*/ MOVW        R3, #699                    @ 0x2bb 
    /*000076f0*/ MOV         R1, #51                     @ 0x33 
    /*000076f4*/ STR         R3, [SP] 
    /*000076f8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000076fc*/ LDR         R3, [PC, #1816]             @ 0x0000000000007e1c 
    /*00007700*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007704*/ MOV         R0, #3 
    /*00007708*/ BL          0x0000000000007708          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000770c*/ LDR         R2, [R6] 
    /*00007710*/ MOV         R1, #0 
    /*00007714*/ STR         SL, [SP] 
    /*00007718*/ STR         R1, [SP, #8] 
    /*0000771c*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007720*/ STR         R2, [SP, #4] 
    /*00007724*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007728*/ MOVW        R2, #0                      @ .LC104 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3844] 
    /*0000772c*/ MOV         R1, #51                     @ 0x33 
    /*00007730*/ MOVT        R2, #0                      @ .LC104 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3844] 
    /*00007734*/ MOV         R0, #3 
    /*00007738*/ BL          0x0000000000007738          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000773c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7278
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7740:
    /*00007740*/ MOV         SL, R8 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7744:
    /*00007744*/ MOVW        R3, #703                    @ 0x2bf 
    /*00007748*/ MOV         R1, #51                     @ 0x33 
    /*0000774c*/ STR         R3, [SP] 
    /*00007750*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007754*/ LDR         R3, [PC, #1728]             @ 0x0000000000007e1c 
    /*00007758*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*0000775c*/ MOV         R0, #3 
    /*00007760*/ BL          0x0000000000007760          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007764*/ LDR         R1, [R6, #4] 
    /*00007768*/ MOVW        R3, #32771                  @ 0x8003 
    /*0000776c*/ STR         SL, [SP] 
    /*00007770*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ab0] 
    /*00007774*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007778*/ STR         R1, [SP, #4] 
    /*0000777c*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ab0] 
    /*00007780*/ MOV         R1, #51                     @ 0x33 
    /*00007784*/ MOV         R0, #3 
    /*00007788*/ BL          0x0000000000007788          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000778c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7278
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7790:
    /*00007790*/ MOV         SL, R8 
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7794:
    /*00007794*/ MOVW        R3, #706                    @ 0x2c2 
    /*00007798*/ MOV         R1, #51                     @ 0x33 
    /*0000779c*/ STR         R3, [SP] 
    /*000077a0*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000077a4*/ LDR         R3, [PC, #1648]             @ 0x0000000000007e1c 
    /*000077a8*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000077ac*/ MOV         R0, #3 
    /*000077b0*/ BL          0x00000000000077B0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000077b4*/ LDR         R2, [R6, #4] 
    /*000077b8*/ MOV         R1, #16 
    /*000077bc*/ STR         SL, [SP] 
    /*000077c0*/ STR         R1, [SP, #8] 
    /*000077c4*/ MOVW        R3, #32771                  @ 0x8003 
    /*000077c8*/ STR         R2, [SP, #4] 
    /*000077cc*/ MOVT        R3, #41011                  @ 0xa033 
    /*000077d0*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ae4] 
    /*000077d4*/ MOV         R1, #51                     @ 0x33 
    /*000077d8*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ae4] 
    /*000077dc*/ MOV         R0, #3 
    /*000077e0*/ BL          0x00000000000077E0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000077e4*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7278
    SVP_NNIE_CheckForwardWithBboxParamKernel_x77e8:
    /*000077e8*/ MOVW        R3, #1053                   @ 0x41d 
    /*000077ec*/ MOV         R1, #51                     @ 0x33 
    /*000077f0*/ STR         R3, [SP] 
    /*000077f4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000077f8*/ LDR         R3, [PC, #1568]             @ 0x0000000000007e20 
    /*000077fc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007800*/ MOV         R0, #3 
    /*00007804*/ BL          0x0000000000007804          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007808*/ LDR         IP, [R8, #12] 
    /*0000780c*/ MOVW        R1, #1692                   @ 0x69c 
    /*00007810*/ LDR         LR, [R8, #8] 
    /*00007814*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007818*/ MOVW        R2, #0                      @ .LC100 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x36e8] 
    /*0000781c*/ MLA         IP, R1, IP, R9 
    /*00007820*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007824*/ MOVT        R2, #0                      @ .LC100 [R_ARM_MOVT_ABS: .rodata.str1.4+0x36e8] 
    /*00007828*/ MOV         R1, #51                     @ 0x33 
    /*0000782c*/ MOV         R0, #3 
    /*00007830*/ LDRH        IP, [IP, #20] 
    /*00007834*/ STR         LR, [SP] 
    /*00007838*/ STR         IP, [SP, #4] 
    /*0000783c*/ BL          0x000000000000783C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007840*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7414
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7844:
    /*00007844*/ MOVW        R3, #1032                   @ 0x408 
    /*00007848*/ MOV         R1, #51                     @ 0x33 
    /*0000784c*/ STR         R3, [SP] 
    /*00007850*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007854*/ LDR         R3, [PC, #1476]             @ 0x0000000000007e20 
    /*00007858*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*0000785c*/ MOV         R0, #3 
    /*00007860*/ BL          0x0000000000007860          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007864*/ LDR         IP, [R8, #12] 
    /*00007868*/ MOV         R3, #1 
    /*0000786c*/ STR         R3, [SP, #8] 
    /*00007870*/ MOVW        R2, #0                      @ .LC99 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x36ac] 
    /*00007874*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007878*/ MLA         R6, R6, IP, R9 
    /*0000787c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007880*/ MOVT        R2, #0                      @ .LC99 [R_ARM_MOVT_ABS: .rodata.str1.4+0x36ac] 
    /*00007884*/ MOV         R1, #51                     @ 0x33 
    /*00007888*/ MOV         R0, #3 
    /*0000788c*/ LDR         LR, [R6, #12] 
    /*00007890*/ STM         SP, {IP, LR} 
    /*00007894*/ BL          0x0000000000007894          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007898*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7414
    SVP_NNIE_CheckForwardWithBboxParamKernel_x789c:
    /*0000789c*/ MOVW        R3, #1212                   @ 0x4bc 
    /*000078a0*/ MOV         R1, #51                     @ 0x33 
    /*000078a4*/ STR         R3, [SP] 
    /*000078a8*/ MOVW        R4, #32771                  @ 0x8003 
    /*000078ac*/ LDR         R3, [PC, #1392]             @ 0x0000000000007e24 
    /*000078b0*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000078b4*/ MOV         R0, #3 
    /*000078b8*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000078bc*/ BL          0x00000000000078BC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000078c0*/ MOV         R3, R4 
    /*000078c4*/ STR         R9, [SP] 
    /*000078c8*/ MOVW        R2, #0                      @ .LC81 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3184] 
    /*000078cc*/ MOVT        R3, #41011                  @ 0xa033 
    /*000078d0*/ MOVT        R2, #0                      @ .LC81 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3184] 
    /*000078d4*/ MOV         R1, #51                     @ 0x33 
    /*000078d8*/ MOV         R0, #3 
    /*000078dc*/ BL          0x00000000000078DC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000078e0*/ MOVT        R4, #41011                  @ 0xa033 
    /*000078e4*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x78e8:
    /*000078e8*/ MOVW        R3, #1042                   @ 0x412 
    /*000078ec*/ MOV         R1, #51                     @ 0x33 
    /*000078f0*/ STR         R3, [SP] 
    /*000078f4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000078f8*/ LDR         R3, [PC, #1312]             @ 0x0000000000007e20 
    /*000078fc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007900*/ MOV         R0, #3 
    /*00007904*/ BL          0x0000000000007904          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007908*/ LDR         IP, [R8, #12] 
    /*0000790c*/ LDR         R4, [R8] 
    /*00007910*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007914*/ MOVW        R2, #0                      @ .LC85 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3264] 
    /*00007918*/ MOVT        R3, #41011                  @ 0xa033 
    /*0000791c*/ MLA         R6, R6, IP, R9 
    /*00007920*/ MOVT        R2, #0                      @ .LC85 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3264] 
    /*00007924*/ MOV         R1, #51                     @ 0x33 
    /*00007928*/ MOV         R0, #3 
    /*0000792c*/ LDRH        LR, [R6, #16] 
    /*00007930*/ STM         SP, {R4, IP, LR} 
    /*00007934*/ BL          0x0000000000007934          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007938*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7414
    SVP_NNIE_CheckForwardWithBboxParamKernel_x793c:
    /*0000793c*/ MOVW        R3, #1047                   @ 0x417 
    /*00007940*/ MOV         R1, #51                     @ 0x33 
    /*00007944*/ STR         R3, [SP] 
    /*00007948*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*0000794c*/ LDR         R3, [PC, #1228]             @ 0x0000000000007e20 
    /*00007950*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007954*/ MOV         R0, #3 
    /*00007958*/ BL          0x0000000000007958          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000795c*/ LDR         IP, [R8, #12] 
    /*00007960*/ LDR         R4, [R8, #4] 
    /*00007964*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007968*/ MOVW        R2, #0                      @ .LC86 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x32c4] 
    /*0000796c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007970*/ MLA         R6, R6, IP, R9 
    /*00007974*/ MOVT        R2, #0                      @ .LC86 [R_ARM_MOVT_ABS: .rodata.str1.4+0x32c4] 
    /*00007978*/ MOV         R1, #51                     @ 0x33 
    /*0000797c*/ MOV         R0, #3 
    /*00007980*/ LDRH        LR, [R6, #18] 
    /*00007984*/ STM         SP, {R4, IP, LR} 
    /*00007988*/ BL          0x0000000000007988          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000798c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7414
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7990:
    /*00007990*/ MOVW        R3, #1228                   @ 0x4cc 
    /*00007994*/ MOV         R1, #51                     @ 0x33 
    /*00007998*/ STR         R3, [SP] 
    /*0000799c*/ MOVW        R4, #32771                  @ 0x8003 
    /*000079a0*/ LDR         R3, [PC, #1148]             @ 0x0000000000007e24 
    /*000079a4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000079a8*/ MOV         R0, #3 
    /*000079ac*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000079b0*/ BL          0x00000000000079B0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000079b4*/ MOV         R3, R4 
    /*000079b8*/ MOVW        R2, #0                      @ .LC91 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348c] 
    /*000079bc*/ MOVT        R3, #41011                  @ 0xa033 
    /*000079c0*/ MOVT        R2, #0                      @ .LC91 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348c] 
    /*000079c4*/ MOV         R1, #51                     @ 0x33 
    /*000079c8*/ MOV         R0, #3 
    /*000079cc*/ MOVT        R4, #41011                  @ 0xa033 
    /*000079d0*/ BL          0x00000000000079D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000079d4*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x79d8:
    /*000079d8*/ MOVW        R3, #1233                   @ 0x4d1 
    /*000079dc*/ MOV         R1, #51                     @ 0x33 
    /*000079e0*/ STR         R3, [SP] 
    /*000079e4*/ MOVW        R4, #32771                  @ 0x8003 
    /*000079e8*/ LDR         R3, [PC, #1076]             @ 0x0000000000007e24 
    /*000079ec*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*000079f0*/ MOV         R0, #3 
    /*000079f4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*000079f8*/ BL          0x00000000000079F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000079fc*/ MOV         R3, R4 
    /*00007a00*/ MOVW        R2, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x34b4] 
    /*00007a04*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007a08*/ MOVT        R2, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x34b4] 
    /*00007a0c*/ MOV         R1, #51                     @ 0x33 
    /*00007a10*/ MOV         R0, #3 
    /*00007a14*/ MOVT        R4, #41011                  @ 0xa033 
    /*00007a18*/ BL          0x0000000000007A18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007a1c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7a20:
    /*00007a20*/ MOVW        R3, #1068                   @ 0x42c 
    /*00007a24*/ MOV         R1, #51                     @ 0x33 
    /*00007a28*/ STR         R3, [SP] 
    /*00007a2c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007a30*/ LDR         R3, [PC, #1000]             @ 0x0000000000007e20 
    /*00007a34*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007a38*/ MOV         R0, #3 
    /*00007a3c*/ BL          0x0000000000007A3C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007a40*/ LDR         R0, [R8, #40]               @ 0x28 
    /*00007a44*/ LDR         R1, [R9, #4] 
    /*00007a48*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007a4c*/ MOVW        R2, #0                      @ .LC87 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3324] 
    /*00007a50*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007a54*/ STR         R0, [SP] 
    /*00007a58*/ MOVT        R2, #0                      @ .LC87 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3324] 
    /*00007a5c*/ STR         R1, [SP, #4] 
    /*00007a60*/ MOV         R0, #3 
    /*00007a64*/ MOV         R1, #51                     @ 0x33 
    /*00007a68*/ BL          0x0000000000007A68          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007a6c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7414
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7a70:
    /*00007a70*/ MOVW        R3, #1253                   @ 0x4e5 
    /*00007a74*/ MOV         R1, #51                     @ 0x33 
    /*00007a78*/ STR         R3, [SP] 
    /*00007a7c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007a80*/ LDR         R3, [PC, #924]              @ 0x0000000000007e24 
    /*00007a84*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007a88*/ MOV         R0, #3 
    /*00007a8c*/ MOVW        R4, #32771                  @ 0x8003 
    /*00007a90*/ BL          0x0000000000007A90          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007a94*/ LDR         R0, [R8, #64]               @ 0x40 
    /*00007a98*/ LDR         R1, [SP, #44]               @ 0x2c 
    /*00007a9c*/ MOV         R3, R4 
    /*00007aa0*/ MOVW        R2, #0                      @ .LC94 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3514] 
    /*00007aa4*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007aa8*/ STR         R0, [SP] 
    /*00007aac*/ MOVT        R2, #0                      @ .LC94 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3514] 
    /*00007ab0*/ STR         R1, [SP, #4] 
    /*00007ab4*/ MOV         R0, #3 
    /*00007ab8*/ MOV         R1, #51                     @ 0x33 
    /*00007abc*/ MOVT        R4, #41011                  @ 0xa033 
    /*00007ac0*/ BL          0x0000000000007AC0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007ac4*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7ac8:
    /*00007ac8*/ MOVW        R3, #1220                   @ 0x4c4 
    /*00007acc*/ MOV         R1, #51                     @ 0x33 
    /*00007ad0*/ STR         R3, [SP] 
    /*00007ad4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007ad8*/ LDR         R3, [PC, #836]              @ 0x0000000000007e24 
    /*00007adc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007ae0*/ MOV         R0, #3 
    /*00007ae4*/ MOVW        R4, #32771                  @ 0x8003 
    /*00007ae8*/ BL          0x0000000000007AE8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007aec*/ LDR         IP, [R8, #12] 
    /*00007af0*/ MOV         R3, R4 
    /*00007af4*/ MOVW        R2, #0                      @ .LC102 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x37b8] 
    /*00007af8*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007afc*/ MOVT        R2, #0                      @ .LC102 [R_ARM_MOVT_ABS: .rodata.str1.4+0x37b8] 
    /*00007b00*/ MLA         R6, R6, IP, R9 
    /*00007b04*/ MOV         R1, #51                     @ 0x33 
    /*00007b08*/ MOV         R0, #3 
    /*00007b0c*/ MOVT        R4, #41011                  @ 0xa033 
    /*00007b10*/ LDRH        LR, [R6, #20] 
    /*00007b14*/ STM         SP, {IP, LR} 
    /*00007b18*/ BL          0x0000000000007B18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007b1c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7b20:
    /*00007b20*/ MOVW        R3, #1025                   @ 0x401 
    /*00007b24*/ MOV         R1, #51                     @ 0x33 
    /*00007b28*/ STR         R3, [SP] 
    /*00007b2c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007b30*/ LDR         R3, [PC, #744]              @ 0x0000000000007e20 
    /*00007b34*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007b38*/ MOV         R0, #3 
    /*00007b3c*/ BL          0x0000000000007B3C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007b40*/ LDR         R0, [R8, #12] 
    /*00007b44*/ LDR         R1, [R9, #8] 
    /*00007b48*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007b4c*/ MOVW        R2, #0                      @ .LC82 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x31b0] 
    /*00007b50*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007b54*/ STR         R0, [SP] 
    /*00007b58*/ MOVT        R2, #0                      @ .LC82 [R_ARM_MOVT_ABS: .rodata.str1.4+0x31b0] 
    /*00007b5c*/ STR         R1, [SP, #4] 
    /*00007b60*/ MOV         R0, #3 
    /*00007b64*/ MOV         R1, #51                     @ 0x33 
    /*00007b68*/ BL          0x0000000000007B68          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007b6c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7414
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7b70:
    /*00007b70*/ MOV         R7, R3 
    /*00007b74*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7574
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7b78:
    /*00007b78*/ MOVW        R3, #1037                   @ 0x40d 
    /*00007b7c*/ MOV         R1, #51                     @ 0x33 
    /*00007b80*/ STR         R3, [SP] 
    /*00007b84*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007b88*/ LDR         R3, [PC, #656]              @ 0x0000000000007e20 
    /*00007b8c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007b90*/ MOV         R0, #3 
    /*00007b94*/ BL          0x0000000000007B94          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007b98*/ LDR         R1, [R8, #16] 
    /*00007b9c*/ MOV         R3, #1 
    /*00007ba0*/ MOV         R2, #0 
    /*00007ba4*/ STR         R3, [SP, #8] 
    /*00007ba8*/ MOV         R0, #3 
    /*00007bac*/ STR         R1, [SP] 
    /*00007bb0*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007bb4*/ STR         R2, [SP, #4] 
    /*00007bb8*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007bbc*/ MOVW        R2, #0                      @ .LC84 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3228] 
    /*00007bc0*/ MOV         R1, #51                     @ 0x33 
    /*00007bc4*/ MOVT        R2, #0                      @ .LC84 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3228] 
    /*00007bc8*/ BL          0x0000000000007BC8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007bcc*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7414
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7bd0:
    /*00007bd0*/ MOV         R6, R2 
    /*00007bd4*/ MOV         SL, R0 
    /*00007bd8*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x76ec
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7bdc:
    /*00007bdc*/ MOV         R6, R2 
    /*00007be0*/ MOV         SL, IP 
    /*00007be4*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7744
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7be8:
    /*00007be8*/ MOV         R6, R2 
    /*00007bec*/ MOV         SL, #0 
    /*00007bf0*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7794
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7bf4:
    /*00007bf4*/ MOV         R6, R2 
    /*00007bf8*/ MOV         SL, #0 
    /*00007bfc*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7054
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7c00:
    /*00007c00*/ MOVW        R3, #1086                   @ 0x43e 
    /*00007c04*/ MOV         R1, #51                     @ 0x33 
    /*00007c08*/ STR         R3, [SP] 
    /*00007c0c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007c10*/ LDR         R3, [PC, #520]              @ 0x0000000000007e20 
    /*00007c14*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007c18*/ MOV         R0, #3 
    /*00007c1c*/ BL          0x0000000000007C1C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007c20*/ LDRD        R0, [R8, #24] 
    /*00007c24*/ MOV         R2, #16 
    /*00007c28*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007c2c*/ STR         R2, [SP, #8] 
    /*00007c30*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007c34*/ STRD        R0, [SP] 
    /*00007c38*/ MOVW        R2, #0                      @ .LC88 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3394] 
    /*00007c3c*/ MOV         R1, #51                     @ 0x33 
    /*00007c40*/ MOVT        R2, #0                      @ .LC88 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3394] 
    /*00007c44*/ MOV         R0, #3 
    /*00007c48*/ BL          0x0000000000007C48          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007c4c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7414
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7c50:
    /*00007c50*/ MOVW        R3, #1090                   @ 0x442 
    /*00007c54*/ MOV         R1, #51                     @ 0x33 
    /*00007c58*/ STR         R3, [SP] 
    /*00007c5c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007c60*/ LDR         R3, [PC, #440]              @ 0x0000000000007e20 
    /*00007c64*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007c68*/ MOV         R0, #3 
    /*00007c6c*/ BL          0x0000000000007C6C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007c70*/ LDRD        R0, [R8, #48]               @ 0x30 
    /*00007c74*/ MOV         R2, #16 
    /*00007c78*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007c7c*/ STR         R2, [SP, #8] 
    /*00007c80*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007c84*/ STRD        R0, [SP] 
    /*00007c88*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x33e4] 
    /*00007c8c*/ MOV         R1, #51                     @ 0x33 
    /*00007c90*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x33e4] 
    /*00007c94*/ MOV         R0, #3 
    /*00007c98*/ BL          0x0000000000007C98          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007c9c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7414
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7ca0:
    /*00007ca0*/ MOVW        R3, #1204                   @ 0x4b4 
    /*00007ca4*/ MOV         R1, #51                     @ 0x33 
    /*00007ca8*/ STR         R3, [SP] 
    /*00007cac*/ MOVW        R4, #32774                  @ 0x8006 
    /*00007cb0*/ LDR         R3, [PC, #364]              @ 0x0000000000007e24 
    /*00007cb4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007cb8*/ MOV         R0, #3 
    /*00007cbc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007cc0*/ BL          0x0000000000007CC0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007cc4*/ MOV         R3, R4 
    /*00007cc8*/ MOVW        R2, #0                      @ .LC78 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x310c] 
    /*00007ccc*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007cd0*/ MOVT        R2, #0                      @ .LC78 [R_ARM_MOVT_ABS: .rodata.str1.4+0x310c] 
    /*00007cd4*/ MOV         R1, #51                     @ 0x33 
    /*00007cd8*/ MOV         R0, #3 
    /*00007cdc*/ MOVT        R4, #41011                  @ 0xa033 
    /*00007ce0*/ BL          0x0000000000007CE0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007ce4*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7ce8:
    /*00007ce8*/ MOVW        R3, #1203                   @ 0x4b3 
    /*00007cec*/ MOV         R1, #51                     @ 0x33 
    /*00007cf0*/ STR         R3, [SP] 
    /*00007cf4*/ MOVW        R4, #32774                  @ 0x8006 
    /*00007cf8*/ LDR         R3, [PC, #292]              @ 0x0000000000007e24 
    /*00007cfc*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007d00*/ MOV         R0, #3 
    /*00007d04*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007d08*/ BL          0x0000000000007D08          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007d0c*/ MOV         R3, R4 
    /*00007d10*/ MOVW        R2, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30ec] 
    /*00007d14*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007d18*/ MOVT        R2, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30ec] 
    /*00007d1c*/ MOV         R1, #51                     @ 0x33 
    /*00007d20*/ MOV         R0, #3 
    /*00007d24*/ MOVT        R4, #41011                  @ 0xa033 
    /*00007d28*/ BL          0x0000000000007D28          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007d2c*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7d30:
    /*00007d30*/ MOVW        R3, #1202                   @ 0x4b2 
    /*00007d34*/ MOV         R1, #51                     @ 0x33 
    /*00007d38*/ STR         R3, [SP] 
    /*00007d3c*/ MOVW        R4, #32774                  @ 0x8006 
    /*00007d40*/ LDR         R3, [PC, #220]              @ 0x0000000000007e24 
    /*00007d44*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007d48*/ MOV         R0, #3 
    /*00007d4c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007d50*/ BL          0x0000000000007D50          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007d54*/ MOV         R3, R4 
    /*00007d58*/ MOVW        R2, #0                      @ .LC98 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3690] 
    /*00007d5c*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007d60*/ MOVT        R2, #0                      @ .LC98 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3690] 
    /*00007d64*/ MOV         R1, #51                     @ 0x33 
    /*00007d68*/ MOV         R0, #3 
    /*00007d6c*/ MOVT        R4, #41011                  @ 0xa033 
    /*00007d70*/ BL          0x0000000000007D70          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007d74*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7d78:
    /*00007d78*/ MOVW        R3, #1201                   @ 0x4b1 
    /*00007d7c*/ MOV         R1, #51                     @ 0x33 
    /*00007d80*/ STR         R3, [SP] 
    /*00007d84*/ MOVW        R4, #32774                  @ 0x8006 
    /*00007d88*/ LDR         R3, [PC, #148]              @ 0x0000000000007e24 
    /*00007d8c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007d90*/ MOV         R0, #3 
    /*00007d94*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007d98*/ BL          0x0000000000007D98          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007d9c*/ MOV         R3, R4 
    /*00007da0*/ MOVW        R2, #0                      @ .LC76 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30d0] 
    /*00007da4*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007da8*/ MOVT        R2, #0                      @ .LC76 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30d0] 
    /*00007dac*/ MOV         R1, #51                     @ 0x33 
    /*00007db0*/ MOV         R0, #3 
    /*00007db4*/ MOVT        R4, #41011                  @ 0xa033 
    /*00007db8*/ BL          0x0000000000007DB8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007dbc*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7dc0:
    /*00007dc0*/ MOVW        R3, #1205                   @ 0x4b5 
    /*00007dc4*/ MOV         R1, #51                     @ 0x33 
    /*00007dc8*/ STR         R3, [SP] 
    /*00007dcc*/ MOVW        R4, #32774                  @ 0x8006 
    /*00007dd0*/ LDR         R3, [PC, #76]               @ 0x0000000000007e24 
    /*00007dd4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007dd8*/ MOV         R0, #3 
    /*00007ddc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007de0*/ BL          0x0000000000007DE0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007de4*/ MOV         R3, R4 
    /*00007de8*/ MOVW        R2, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3128] 
    /*00007dec*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007df0*/ MOVT        R2, #0                      @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3128] 
    /*00007df4*/ MOV         R1, #51                     @ 0x33 
    /*00007df8*/ MOV         R0, #3 
    /*00007dfc*/ MOVT        R4, #41011                  @ 0xa033 
    /*00007e00*/ BL          0x0000000000007E00          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007e04*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x71b8
    SVP_NNIE_CheckForwardWithBboxParamKernel_x7e08:
    /*00007e08*/ LDR         R3, [R8, #12] 
    /*00007e0c*/ MOV         R7, R2 
    /*00007e10*/ STR         R3, [SP, #32] 
    /*00007e14*/ MOV         R2, R3 
    /*00007e18*/ B           SVP_NNIE_CheckForwardWithBboxParamKernel_x7574
    /*00007e1c*/ ANDEQ       R0, R0, R8, LSL #5 
    /*00007e20*/ ANDEQ       R0, R0, R4, ROR #4 
    /*00007e24*/ ANDEQ       R0, R0, R8, LSR R2 
FUNC_END SVP_NNIE_CheckForwardWithBboxParamKernel


FUNC_BEGIN SVP_NNIE_CheckQueryParamKernel
    /*00007e28*/ PUSH        {R4, R5, R6, LR} 
    /*00007e2c*/ SUBS        R4, R0, #0 
    /*00007e30*/ SUB         SP, SP, #16 
    /*00007e34*/ BNE         SVP_NNIE_CheckQueryParamKernel_x7ea0
    /*00007e38*/ CMN         R1, #-268435454             @ 0xf0000002 
    /*00007e3c*/ MOV         R6, R1 
    /*00007e40*/ BHI         SVP_NNIE_CheckQueryParamKernel_x7efc
    /*00007e44*/ CMP         R2, #1 
    /*00007e48*/ MOV         R5, R2 
    /*00007e4c*/ MOVLS       R0, R4 
    /*00007e50*/ BHI         SVP_NNIE_CheckQueryParamKernel_x7e5c
    SVP_NNIE_CheckQueryParamKernel_x7e54:
    /*00007e54*/ ADD         SP, SP, #16 
    /*00007e58*/ POP         {R4, R5, R6, PC} 
    SVP_NNIE_CheckQueryParamKernel_x7e5c:
    /*00007e5c*/ MOVW        R3, #1287                   @ 0x507 
    /*00007e60*/ MOV         R1, #51                     @ 0x33 
    /*00007e64*/ STR         R3, [SP] 
    /*00007e68*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007e6c*/ LDR         R3, [PC, #220]              @ 0x0000000000007f50 
    /*00007e70*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007e74*/ MOV         R0, #3 
    /*00007e78*/ BL          0x0000000000007E78          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007e7c*/ MOV         R3, R5 
    /*00007e80*/ MOVW        R2, #0                      @ .LC115 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3ae8] 
    /*00007e84*/ MOV         R1, #51                     @ 0x33 
    /*00007e88*/ MOVT        R2, #0                      @ .LC115 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3ae8] 
    /*00007e8c*/ MOV         R0, #3 
    /*00007e90*/ BL          0x0000000000007E90          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007e94*/ MOVW        R0, #32771                  @ 0x8003 
    /*00007e98*/ MOVT        R0, #41011                  @ 0xa033 
    /*00007e9c*/ B           SVP_NNIE_CheckQueryParamKernel_x7e54
    SVP_NNIE_CheckQueryParamKernel_x7ea0:
    /*00007ea0*/ MOVW        R3, #1284                   @ 0x504 
    /*00007ea4*/ MOV         R1, #51                     @ 0x33 
    /*00007ea8*/ STR         R3, [SP] 
    /*00007eac*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007eb0*/ LDR         R3, [PC, #152]              @ 0x0000000000007f50 
    /*00007eb4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007eb8*/ MOV         R0, #3 
    /*00007ebc*/ BL          0x0000000000007EBC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007ec0*/ MOV         R3, #1 
    /*00007ec4*/ MOV         R2, #0 
    /*00007ec8*/ STR         R3, [SP, #8] 
    /*00007ecc*/ STR         R2, [SP, #4] 
    /*00007ed0*/ MOVW        R3, #32771                  @ 0x8003 
    /*00007ed4*/ STR         R4, [SP] 
    /*00007ed8*/ MOVT        R3, #41011                  @ 0xa033 
    /*00007edc*/ MOVW        R2, #0                      @ .LC113 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3a98] 
    /*00007ee0*/ MOV         R1, #51                     @ 0x33 
    /*00007ee4*/ MOVT        R2, #0                      @ .LC113 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3a98] 
    /*00007ee8*/ MOV         R0, #3 
    /*00007eec*/ BL          0x0000000000007EEC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007ef0*/ MOVW        R0, #32771                  @ 0x8003 
    /*00007ef4*/ MOVT        R0, #41011                  @ 0xa033 
    /*00007ef8*/ B           SVP_NNIE_CheckQueryParamKernel_x7e54
    SVP_NNIE_CheckQueryParamKernel_x7efc:
    /*00007efc*/ MOVW        R3, #1285                   @ 0x505 
    /*00007f00*/ MOV         R1, #51                     @ 0x33 
    /*00007f04*/ STR         R3, [SP] 
    /*00007f08*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1758] 
    /*00007f0c*/ LDR         R3, [PC, #60]               @ 0x0000000000007f50 
    /*00007f10*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1758] 
    /*00007f14*/ MOV         R0, #3 
    /*00007f18*/ BL          0x0000000000007F18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007f1c*/ MVN         R2, #-268435456             @ 0xf0000000 
    /*00007f20*/ MVN         R1, #0 
    /*00007f24*/ STR         R2, [SP, #4] 
    /*00007f28*/ STR         R1, [SP] 
    /*00007f2c*/ MOV         R3, R6 
    /*00007f30*/ MOVW        R2, #0                      @ .LC114 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3ac4] 
    /*00007f34*/ MOV         R1, #51                     @ 0x33 
    /*00007f38*/ MOVT        R2, #0                      @ .LC114 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3ac4] 
    /*00007f3c*/ MOV         R0, #3 
    /*00007f40*/ BL          0x0000000000007F40          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007f44*/ MOVW        R0, #32771                  @ 0x8003 
    /*00007f48*/ MOVT        R0, #41011                  @ 0xa033 
    /*00007f4c*/ B           SVP_NNIE_CheckQueryParamKernel_x7e54
    /*00007f50*/ ANDEQ       R0, R0, R0, LSR #5 
FUNC_END SVP_NNIE_CheckQueryParamKernel


FUNC_BEGIN SVP_NNIE_TSK_ADDR_PutNodeToFreeList
    /*00007f54*/ LDR         R3, [R0, #204]              @ 0xcc 
    /*00007f58*/ ADD         R2, R0, #200                @ 0xc8 
    /*00007f5c*/ STR         R1, [R0, #204]              @ 0xcc 
    /*00007f60*/ STM         R1, {R2, R3} 
    /*00007f64*/ STR         R1, [R3] 
    /*00007f68*/ LDR         R3, [R0, #44]               @ 0x2c 
    /*00007f6c*/ ADD         R3, R3, #1 
    /*00007f70*/ STR         R3, [R0, #44]               @ 0x2c 
    /*00007f74*/ BX          LR 
FUNC_END SVP_NNIE_TSK_ADDR_PutNodeToFreeList


FUNC_BEGIN SVP_NNIE_TSK_ADDR_PutNodeToBusyList
    /*00007f78*/ LDR         R3, [R0, #196]              @ 0xc4 
    /*00007f7c*/ ADD         R2, R0, #192                @ 0xc0 
    /*00007f80*/ STR         R1, [R0, #196]              @ 0xc4 
    /*00007f84*/ STM         R1, {R2, R3} 
    /*00007f88*/ STR         R1, [R3] 
    /*00007f8c*/ LDR         R3, [R0, #48]               @ 0x30 
    /*00007f90*/ ADD         R3, R3, #1 
    /*00007f94*/ STR         R3, [R0, #48]               @ 0x30 
    /*00007f98*/ BX          LR 
FUNC_END SVP_NNIE_TSK_ADDR_PutNodeToBusyList


FUNC_BEGIN SVP_NNIE_TSK_ADDR_GetNodeFromFreeList
    /*00007f9c*/ MOV         R2, R0 
    /*00007fa0*/ LDR         R3, [R2, #200]!             @ 0xc8 
    /*00007fa4*/ CMP         R3, R2 
    /*00007fa8*/ BEQ         SVP_NNIE_TSK_ADDR_GetNodeFromFreeList_x7ff0
    /*00007fac*/ PUSH        {R4, LR} 
    /*00007fb0*/ MOV         R1, R0 
    /*00007fb4*/ LDR         LR, [R3, #4] 
    /*00007fb8*/ MOV         R0, R3 
    /*00007fbc*/ LDR         R4, [R3] 
    /*00007fc0*/ MOV         IP, #256                    @ 0x100 
    /*00007fc4*/ MOV         R2, #512                    @ 0x200 
    /*00007fc8*/ MOVT        IP, #16 
    /*00007fcc*/ MOVT        R2, #32 
    /*00007fd0*/ STR         LR, [R4, #4] 
    /*00007fd4*/ STR         R4, [LR] 
    /*00007fd8*/ STR         IP, [R3] 
    /*00007fdc*/ STR         R2, [R3, #4] 
    /*00007fe0*/ LDR         R3, [R1, #44]               @ 0x2c 
    /*00007fe4*/ SUB         R3, R3, #1 
    /*00007fe8*/ STR         R3, [R1, #44]               @ 0x2c 
    /*00007fec*/ POP         {R4, PC} 
    SVP_NNIE_TSK_ADDR_GetNodeFromFreeList_x7ff0:
    /*00007ff0*/ MOV         R0, #0 
    /*00007ff4*/ BX          LR 
FUNC_END SVP_NNIE_TSK_ADDR_GetNodeFromFreeList


FUNC_BEGIN SVP_NNIE_TSK_ADDR_FindNodeFromBusyList
    /*00007ff8*/ PUSH        {R4, R5, R6, R7} 
    /*00007ffc*/ MOV         R7, R3 
    /*00008000*/ LDR         R3, [R0, #192]!             @ 0xc0 
    /*00008004*/ CMP         R3, R0 
    /*00008008*/ BEQ         SVP_NNIE_TSK_ADDR_FindNodeFromBusyList_x8040
    /*0000800c*/ LDRD        R4, [R3, #8] 
    /*00008010*/ MOV         R6, R2 
    /*00008014*/ CMP         R7, R5 
    /*00008018*/ CMPEQ       R2, R4 
    /*0000801c*/ BNE         SVP_NNIE_TSK_ADDR_FindNodeFromBusyList_x8034
    /*00008020*/ B           SVP_NNIE_TSK_ADDR_FindNodeFromBusyList_x804c
    SVP_NNIE_TSK_ADDR_FindNodeFromBusyList_x8024:
    /*00008024*/ LDRD        R4, [R3, #8] 
    /*00008028*/ CMP         R5, R7 
    /*0000802c*/ CMPEQ       R4, R6 
    /*00008030*/ BEQ         SVP_NNIE_TSK_ADDR_FindNodeFromBusyList_x804c
    SVP_NNIE_TSK_ADDR_FindNodeFromBusyList_x8034:
    /*00008034*/ LDR         R3, [R3] 
    /*00008038*/ CMP         R3, R0 
    /*0000803c*/ BNE         SVP_NNIE_TSK_ADDR_FindNodeFromBusyList_x8024
    SVP_NNIE_TSK_ADDR_FindNodeFromBusyList_x8040:
    /*00008040*/ MOV         R0, #0 
    /*00008044*/ POP         {R4, R5, R6, R7} 
    /*00008048*/ BX          LR 
    SVP_NNIE_TSK_ADDR_FindNodeFromBusyList_x804c:
    /*0000804c*/ MOV         R0, R3 
    /*00008050*/ POP         {R4, R5, R6, R7} 
    /*00008054*/ BX          LR 
FUNC_END SVP_NNIE_TSK_ADDR_FindNodeFromBusyList


FUNC_BEGIN SVP_NNIE_TSK_ADDR_DeleteNodeFromBusyList
    /*00008058*/ MOV         R1, R0 
    /*0000805c*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00008060*/ MOV         R7, R3 
    /*00008064*/ LDR         R3, [R1, #192]!             @ 0xc0 
    /*00008068*/ CMP         R3, R1 
    /*0000806c*/ BEQ         SVP_NNIE_TSK_ADDR_DeleteNodeFromBusyList_x80a4
    /*00008070*/ LDRD        R4, [R3, #8] 
    /*00008074*/ MOV         R6, R2 
    /*00008078*/ CMP         R7, R5 
    /*0000807c*/ CMPEQ       R2, R4 
    /*00008080*/ BNE         SVP_NNIE_TSK_ADDR_DeleteNodeFromBusyList_x8098
    /*00008084*/ B           SVP_NNIE_TSK_ADDR_DeleteNodeFromBusyList_x80b0
    SVP_NNIE_TSK_ADDR_DeleteNodeFromBusyList_x8088:
    /*00008088*/ LDRD        R4, [R3, #8] 
    /*0000808c*/ CMP         R5, R7 
    /*00008090*/ CMPEQ       R4, R6 
    /*00008094*/ BEQ         SVP_NNIE_TSK_ADDR_DeleteNodeFromBusyList_x80b0
    SVP_NNIE_TSK_ADDR_DeleteNodeFromBusyList_x8098:
    /*00008098*/ LDR         R3, [R3] 
    /*0000809c*/ CMP         R3, R1 
    /*000080a0*/ BNE         SVP_NNIE_TSK_ADDR_DeleteNodeFromBusyList_x8088
    SVP_NNIE_TSK_ADDR_DeleteNodeFromBusyList_x80a4:
    /*000080a4*/ MOV         R2, #0 
    /*000080a8*/ MOV         R0, R2 
    /*000080ac*/ POP         {R4, R5, R6, R7, PC} 
    SVP_NNIE_TSK_ADDR_DeleteNodeFromBusyList_x80b0:
    /*000080b0*/ LDR         LR, [R3, #4] 
    /*000080b4*/ MOV         R2, R3 
    /*000080b8*/ LDR         R4, [R3] 
    /*000080bc*/ MOV         IP, #256                    @ 0x100 
    /*000080c0*/ MOV         R1, #512                    @ 0x200 
    /*000080c4*/ MOVT        IP, #16 
    /*000080c8*/ MOVT        R1, #32 
    /*000080cc*/ STR         LR, [R4, #4] 
    /*000080d0*/ STR         R4, [LR] 
    /*000080d4*/ STR         IP, [R3] 
    /*000080d8*/ STR         R1, [R3, #4] 
    /*000080dc*/ LDR         R3, [R0, #48]               @ 0x30 
    /*000080e0*/ SUB         R3, R3, #1 
    /*000080e4*/ STR         R3, [R0, #48]               @ 0x30 
    /*000080e8*/ MOV         R0, R2 
    /*000080ec*/ POP         {R4, R5, R6, R7, PC} 
FUNC_END SVP_NNIE_TSK_ADDR_DeleteNodeFromBusyList


FUNC_BEGIN SVP_NNIE_TSK_ADDR_InitList
    /*000080f0*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000080f4*/ MOV         R4, R0 
    /*000080f8*/ MOV         R0, #24 
    /*000080fc*/ ADD         R3, R4, #192                @ 0xc0 
    /*00008100*/ ADD         R5, R4, #200                @ 0xc8 
    /*00008104*/ SUB         SP, SP, #8 
    /*00008108*/ STR         R5, [R4, #200]              @ 0xc8 
    /*0000810c*/ MOV         R8, R1 
    /*00008110*/ STR         R5, [R4, #204]              @ 0xcc 
    /*00008114*/ STR         R3, [R4, #192]              @ 0xc0 
    /*00008118*/ STR         R3, [R4, #196]              @ 0xc4 
    /*0000811c*/ MUL         R0, R0, R1 
    /*00008120*/ BL          0x0000000000008120          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*00008124*/ CMP         R0, #0 
    /*00008128*/ BEQ         SVP_NNIE_TSK_ADDR_InitList_x81a4
    /*0000812c*/ CMP         R8, #0 
    /*00008130*/ STR         R0, [R4, #188]              @ 0xbc 
    /*00008134*/ BEQ         SVP_NNIE_TSK_ADDR_InitList_x8198
    /*00008138*/ MOV         R2, #0 
    /*0000813c*/ MOV         R6, #0 
    /*00008140*/ MOV         R7, #0 
    /*00008144*/ MOV         IP, R2 
    /*00008148*/ MOV         LR, R2 
    /*0000814c*/ B           SVP_NNIE_TSK_ADDR_InitList_x8154
    SVP_NNIE_TSK_ADDR_InitList_x8150:
    /*00008150*/ LDR         R0, [R4, #188]              @ 0xbc 
    SVP_NNIE_TSK_ADDR_InitList_x8154:
    /*00008154*/ ADD         R3, R0, R2 
    /*00008158*/ ADD         IP, IP, #1 
    /*0000815c*/ CMP         R8, IP 
    /*00008160*/ STR         LR, [R3, #16] 
    /*00008164*/ STRD        R6, [R3, #8] 
    /*00008168*/ STR         R3, [R0, R2] 
    /*0000816c*/ STR         R3, [R3, #4] 
    /*00008170*/ LDR         R1, [R4, #204]              @ 0xcc 
    /*00008174*/ STR         R3, [R4, #204]              @ 0xcc 
    /*00008178*/ STR         R5, [R0, R2] 
    /*0000817c*/ ADD         R2, R2, #24 
    /*00008180*/ STR         R1, [R3, #4] 
    /*00008184*/ STR         R3, [R1] 
    /*00008188*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*0000818c*/ ADD         R3, R3, #1 
    /*00008190*/ STR         R3, [R4, #44]               @ 0x2c 
    /*00008194*/ BNE         SVP_NNIE_TSK_ADDR_InitList_x8150
    SVP_NNIE_TSK_ADDR_InitList_x8198:
    /*00008198*/ MOV         R0, #0 
    SVP_NNIE_TSK_ADDR_InitList_x819c:
    /*0000819c*/ ADD         SP, SP, #8 
    /*000081a0*/ POP         {R4, R5, R6, R7, R8, PC} 
    SVP_NNIE_TSK_ADDR_InitList_x81a4:
    /*000081a4*/ MOV         R2, #227                    @ 0xe3 
    /*000081a8*/ MOV         R1, #51                     @ 0x33 
    /*000081ac*/ STR         R2, [SP] 
    /*000081b0*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x2c0] 
    /*000081b4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*000081b8*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x2c0] 
    /*000081bc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*000081c0*/ MOV         R0, #3 
    /*000081c4*/ BL          0x00000000000081C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000081c8*/ MOVW        R3, #32781                  @ 0x800d 
    /*000081cc*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b30] 
    /*000081d0*/ MOVT        R3, #41011                  @ 0xa033 
    /*000081d4*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b30] 
    /*000081d8*/ MOV         R1, #51                     @ 0x33 
    /*000081dc*/ MOV         R0, #3 
    /*000081e0*/ BL          0x00000000000081E0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000081e4*/ MOVW        R0, #32781                  @ 0x800d 
    /*000081e8*/ MOVT        R0, #41011                  @ 0xa033 
    /*000081ec*/ B           SVP_NNIE_TSK_ADDR_InitList_x819c
FUNC_END SVP_NNIE_TSK_ADDR_InitList


FUNC_BEGIN SVP_NNIE_TSK_ADDR_DeInitList
    /*000081f0*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*000081f4*/ MOV         R9, R1 
    /*000081f8*/ LDR         R1, [R0, #48]               @ 0x30 
    /*000081fc*/ MOV         R3, #0 
    /*00008200*/ MOV         R8, #256                    @ 0x100 
    /*00008204*/ MOV         R7, #512                    @ 0x200 
    /*00008208*/ ADD         R6, R0, #192                @ 0xc0 
    /*0000820c*/ MOVT        R8, #16 
    /*00008210*/ MOVT        R7, #32 
    /*00008214*/ MOV         R5, R0 
    /*00008218*/ MOV         R4, R3 
    SVP_NNIE_TSK_ADDR_DeInitList_x821c:
    /*0000821c*/ CMP         R1, #0 
    /*00008220*/ BEQ         SVP_NNIE_TSK_ADDR_DeInitList_x82cc
    SVP_NNIE_TSK_ADDR_DeInitList_x8224:
    /*00008224*/ CMP         R4, R6 
    /*00008228*/ BEQ         SVP_NNIE_TSK_ADDR_DeInitList_x82cc
    /*0000822c*/ LDR         R4, [R5, #192]              @ 0xc0 
    /*00008230*/ CMP         R6, R4 
    /*00008234*/ BEQ         SVP_NNIE_TSK_ADDR_DeInitList_x82e8
    /*00008238*/ CMP         R4, #0 
    /*0000823c*/ BNE         SVP_NNIE_TSK_ADDR_DeInitList_x825c
    /*00008240*/ MOV         R3, R4 
    /*00008244*/ LDR         R4, [R4] 
    /*00008248*/ CMP         R4, R6 
    /*0000824c*/ BEQ         SVP_NNIE_TSK_ADDR_DeInitList_x821c
    /*00008250*/ CMP         R4, #0 
    /*00008254*/ BNE         SVP_NNIE_TSK_ADDR_DeInitList_x825c
    SVP_NNIE_TSK_ADDR_DeInitList_x8258:
    /*00008258*/ B           SVP_NNIE_TSK_ADDR_DeInitList_x8258
    SVP_NNIE_TSK_ADDR_DeInitList_x825c:
    /*0000825c*/ CMP         R9, #0 
    /*00008260*/ BNE         SVP_NNIE_TSK_ADDR_DeInitList_x8274
    /*00008264*/ LDR         R0, [R4, #16] 
    /*00008268*/ CMP         R0, #0 
    /*0000826c*/ BEQ         SVP_NNIE_TSK_ADDR_DeInitList_x8274
    /*00008270*/ BL          0x0000000000008270          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    SVP_NNIE_TSK_ADDR_DeInitList_x8274:
    /*00008274*/ LDR         R2, [R4, #4] 
    /*00008278*/ MOV         R3, R4 
    /*0000827c*/ LDR         R1, [R4] 
    /*00008280*/ STR         R2, [R1, #4] 
    /*00008284*/ STR         R1, [R2] 
    /*00008288*/ STR         R8, [R4] 
    /*0000828c*/ STR         R7, [R4, #4] 
    /*00008290*/ LDR         R2, [R5, #48]               @ 0x30 
    /*00008294*/ SUB         R2, R2, #1 
    /*00008298*/ STR         R2, [R5, #48]               @ 0x30 
    SVP_NNIE_TSK_ADDR_DeInitList_x829c:
    /*0000829c*/ LDR         R1, [R5, #204]              @ 0xcc 
    /*000082a0*/ ADD         R2, R5, #200                @ 0xc8 
    /*000082a4*/ STR         R3, [R5, #204]              @ 0xcc 
    /*000082a8*/ STR         R2, [R3] 
    /*000082ac*/ STR         R1, [R3, #4] 
    /*000082b0*/ STR         R3, [R1] 
    /*000082b4*/ LDR         R1, [R5, #48]               @ 0x30 
    /*000082b8*/ LDR         R2, [R5, #44]               @ 0x2c 
    /*000082bc*/ CMP         R1, #0 
    /*000082c0*/ ADD         R2, R2, #1 
    /*000082c4*/ STR         R2, [R5, #44]               @ 0x2c 
    /*000082c8*/ BNE         SVP_NNIE_TSK_ADDR_DeInitList_x8224
    SVP_NNIE_TSK_ADDR_DeInitList_x82cc:
    /*000082cc*/ LDR         R0, [R5, #188]              @ 0xbc 
    /*000082d0*/ CMP         R0, #0 
    /*000082d4*/ POPEQ       {R4, R5, R6, R7, R8, R9, SL, PC} 
    /*000082d8*/ BL          0x00000000000082D8          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*000082dc*/ MOV         R3, #0 
    /*000082e0*/ STR         R3, [R5, #188]              @ 0xbc 
    /*000082e4*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    SVP_NNIE_TSK_ADDR_DeInitList_x82e8:
    /*000082e8*/ CMP         R3, #0 
    /*000082ec*/ BNE         SVP_NNIE_TSK_ADDR_DeInitList_x829c
    /*000082f0*/ B           SVP_NNIE_TSK_ADDR_DeInitList_x821c
FUNC_END SVP_NNIE_TSK_ADDR_DeInitList


FUNC_BEGIN SVP_NNIE_FillForwardTask
    /*000082f4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000082f8*/ SUB         SP, SP, #44                 @ 0x2c 
    /*000082fc*/ MOV         R4, R2 
    /*00008300*/ MOVW        R5, #13968                  @ 0x3690 
    /*00008304*/ LDR         LR, [SP, #84]               @ 0x54 
    /*00008308*/ MOV         R2, #0 
    /*0000830c*/ LDR         IP, [SP, #92]               @ 0x5c 
    /*00008310*/ MOV         R6, #0 
    /*00008314*/ SUB         LR, LR, #1 
    /*00008318*/ MOV         R7, #0 
    /*0000831c*/ CLZ         LR, LR 
    /*00008320*/ LDR         FP, [SP, #80]               @ 0x50 
    /*00008324*/ STRH        R2, [IP, #2] 
    /*00008328*/ MOV         SL, R1 
    /*0000832c*/ STR         R2, [IP, #4] 
    /*00008330*/ LSR         LR, LR, #5 
    /*00008334*/ STRD        R6, [IP, #8] 
    /*00008338*/ STRH        LR, [IP] 
    /*0000833c*/ MOVW        LR, #1692                   @ 0x69c 
    /*00008340*/ LDRD        R8, [R4, R5] 
    /*00008344*/ MOV         R5, R3 
    /*00008348*/ STRD        R8, [IP, #16] 
    /*0000834c*/ LDR         R3, [FP, #8] 
    /*00008350*/ MLA         R3, LR, R3, R4 
    /*00008354*/ LDR         R3, [R3, #24] 
    /*00008358*/ STR         R3, [IP, #24] 
    /*0000835c*/ LDR         R3, [FP, #8] 
    /*00008360*/ MLA         LR, LR, R3, R4 
    /*00008364*/ LDR         R3, [LR, #28] 
    /*00008368*/ STR         R3, [IP, #28] 
    /*0000836c*/ LDRD        R8, [FP, #40]               @ 0x28 
    /*00008370*/ STRD        R6, [IP, #40]               @ 0x28 
    /*00008374*/ STRD        R8, [IP, #32] 
    /*00008378*/ LDRD        R6, [FP, #16] 
    /*0000837c*/ STRD        R6, [IP, #48]               @ 0x30 
    /*00008380*/ LDR         R3, [R1, #24] 
    /*00008384*/ STR         R2, [IP, #60]               @ 0x3c 
    /*00008388*/ STR         R3, [IP, #56]               @ 0x38 
    /*0000838c*/ LDR         R3, [R0, #48]               @ 0x30 
    /*00008390*/ CMP         R3, R2 
    /*00008394*/ BEQ         SVP_NNIE_FillForwardTask_x88d4
    /*00008398*/ MOV         LR, R0 
    /*0000839c*/ LDRD        R0, [FP, #40]               @ 0x28 
    /*000083a0*/ LDR         IP, [LR, #192]!             @ 0xc0 
    /*000083a4*/ CMP         IP, LR 
    /*000083a8*/ BNE         SVP_NNIE_FillForwardTask_x853c
    SVP_NNIE_FillForwardTask_x83ac:
    /*000083ac*/ LDR         R2, [FP, #56]               @ 0x38 
    SVP_NNIE_FillForwardTask_x83b0:
    /*000083b0*/ BL          0x00000000000083B0          @ CMPI_Remap_Cached [R_ARM_CALL: +0x0] 
    /*000083b4*/ SUBS        R8, R0, #0 
    /*000083b8*/ MOVNE       R3, #0 
    /*000083bc*/ STRNE       R3, [SP, #36]               @ 0x24 
    /*000083c0*/ BEQ         SVP_NNIE_FillForwardTask_x8d14
    SVP_NNIE_FillForwardTask_x83c4:
    /*000083c4*/ LDR         R2, [FP] 
    /*000083c8*/ CMP         R2, #0 
    /*000083cc*/ BEQ         SVP_NNIE_FillForwardTask_x8c9c
    /*000083d0*/ MOV         R3, #0 
    /*000083d4*/ SUB         R0, R8, #4 
    /*000083d8*/ MOV         R2, R3 
    /*000083dc*/ MOV         R1, SL 
    SVP_NNIE_FillForwardTask_x83e0:
    /*000083e0*/ LDR         IP, [R1, #4] 
    /*000083e4*/ ADD         R3, R3, #1 
    /*000083e8*/ ADD         R2, R2, #4 
    /*000083ec*/ ADD         R1, R1, #48                 @ 0x30 
    /*000083f0*/ STR         IP, [R0, #4]! 
    /*000083f4*/ LDR         IP, [FP] 
    /*000083f8*/ CMP         IP, R3 
    /*000083fc*/ BHI         SVP_NNIE_FillForwardTask_x83e0
    /*00008400*/ LDR         R3, [FP, #4] 
    /*00008404*/ CMP         R3, #0 
    /*00008408*/ BEQ         SVP_NNIE_FillForwardTask_x8438
    SVP_NNIE_FillForwardTask_x840c:
    /*0000840c*/ ADD         R0, R8, R2 
    /*00008410*/ MOV         R1, R5 
    /*00008414*/ MOV         R3, #0 
    SVP_NNIE_FillForwardTask_x8418:
    /*00008418*/ LDR         R2, [R1, #4] 
    /*0000841c*/ ADD         R3, R3, #1 
    /*00008420*/ ADD         R1, R1, #48                 @ 0x30 
    /*00008424*/ STR         R2, [R0], #4 
    /*00008428*/ SUB         R2, R0, R8 
    /*0000842c*/ LDR         IP, [FP, #4] 
    /*00008430*/ CMP         IP, R3 
    /*00008434*/ BHI         SVP_NNIE_FillForwardTask_x8418
    SVP_NNIE_FillForwardTask_x8438:
    /*00008438*/ ADD         R3, R2, #15 
    /*0000843c*/ ADD         R9, R8, R2 
    /*00008440*/ BIC         R3, R3, #15 
    /*00008444*/ SUBS        R2, R3, R2 
    /*00008448*/ BEQ         SVP_NNIE_FillForwardTask_x845c
    /*0000844c*/ MOV         R0, R9 
    /*00008450*/ MOV         R1, #0 
    /*00008454*/ ADD         R9, R8, R3 
    /*00008458*/ BL          0x0000000000008458          @ memset [R_ARM_CALL: +0x0] 
    SVP_NNIE_FillForwardTask_x845c:
    /*0000845c*/ LDR         R3, [FP, #8] 
    /*00008460*/ MOVW        R2, #1692                   @ 0x69c 
    /*00008464*/ MLA         R4, R2, R3, R4 
    /*00008468*/ LDR         R3, [R4, #12] 
    /*0000846c*/ CMP         R3, #2 
    /*00008470*/ BEQ         SVP_NNIE_FillForwardTask_x89f8
    SVP_NNIE_FillForwardTask_x8474:
    /*00008474*/ LDR         R3, [FP, #4] 
    /*00008478*/ CMP         R3, #0 
    /*0000847c*/ BEQ         SVP_NNIE_FillForwardTask_x84dc
    /*00008480*/ MOV         R1, #0 
    /*00008484*/ MOV         R3, R5 
    /*00008488*/ MOV         R0, R1 
    /*0000848c*/ SUB         IP, R9, #8 
    /*00008490*/ B           SVP_NNIE_FillForwardTask_x8498
    SVP_NNIE_FillForwardTask_x8494:
    /*00008494*/ MOV         R0, R2 
    SVP_NNIE_FillForwardTask_x8498:
    /*00008498*/ LDRD        R4, [R3, #16] 
    /*0000849c*/ ADD         R1, R1, #1 
    /*000084a0*/ ADD         R3, R3, #48                 @ 0x30 
    /*000084a4*/ ADD         R2, R0, #8 
    /*000084a8*/ STRD        R4, [IP, #8]! 
    /*000084ac*/ LDR         LR, [FP, #4] 
    /*000084b0*/ CMP         LR, R1 
    /*000084b4*/ BHI         SVP_NNIE_FillForwardTask_x8494
    /*000084b8*/ ADD         R3, R0, #23 
    /*000084bc*/ ADD         R0, R9, R2 
    /*000084c0*/ BIC         R3, R3, #15 
    /*000084c4*/ SUBS        R2, R3, R2 
    /*000084c8*/ MOVEQ       R9, R0 
    /*000084cc*/ BEQ         SVP_NNIE_FillForwardTask_x84dc
    /*000084d0*/ MOV         R1, #0 
    /*000084d4*/ ADD         R9, R9, R3 
    /*000084d8*/ BL          0x00000000000084D8          @ memset [R_ARM_CALL: +0x0] 
    SVP_NNIE_FillForwardTask_x84dc:
    /*000084dc*/ LDR         R1, [FP] 
    /*000084e0*/ CMP         R1, #0 
    /*000084e4*/ BEQ         SVP_NNIE_FillForwardTask_x85f0
    /*000084e8*/ MOVW        R3, #43691                  @ 0xaaab 
    /*000084ec*/ MOVT        R3, #43690                  @ 0xaaaa 
    /*000084f0*/ STR         R3, [SP, #32] 
    /*000084f4*/ MOV         R3, #0 
    /*000084f8*/ STR         R8, [SP, #28] 
    /*000084fc*/ MOV         R8, R3 
    /*00008500*/ STR         FP, [SP, #80]               @ 0x50 
    /*00008504*/ MOV         FP, R9 
    SVP_NNIE_FillForwardTask_x8508:
    /*00008508*/ LDR         R3, [SL] 
    /*0000850c*/ CMP         R3, #5 
    /*00008510*/ LDRLS       PC, [PC, R3, LSL #2] 
    /*00008514*/ B           SVP_NNIE_FillForwardTask_x8838
    /*00008518*/ ANDEQ       R8, R0, IP, ROR #15 
    /*0000851c*/ MULEQ       R0, R4, R7 
    /*00008520*/ STRDEQ      R8, [R0], -R0 
    /*00008524*/ ANDEQ       R8, R0, IP, LSR R6 
    /*00008528*/ STRDEQ      R8, [R0], -R8               @ <UNPREDICTABLE> 
    /*0000852c*/ ANDEQ       R8, R0, R0, ROR R5 
    SVP_NNIE_FillForwardTask_x8530:
    /*00008530*/ LDR         IP, [IP] 
    /*00008534*/ CMP         IP, LR 
    /*00008538*/ BEQ         SVP_NNIE_FillForwardTask_x83ac
    SVP_NNIE_FillForwardTask_x853c:
    /*0000853c*/ LDRD        R2, [IP, #8] 
    /*00008540*/ CMP         R1, R3 
    /*00008544*/ CMPEQ       R0, R2 
    /*00008548*/ BNE         SVP_NNIE_FillForwardTask_x8530
    /*0000854c*/ CMP         IP, #0 
    /*00008550*/ STR         IP, [SP, #36]               @ 0x24 
    /*00008554*/ LDR         R2, [FP, #56]               @ 0x38 
    /*00008558*/ BEQ         SVP_NNIE_FillForwardTask_x83b0
    /*0000855c*/ LDR         R3, [IP, #20] 
    /*00008560*/ CMP         R2, R3 
    /*00008564*/ LDRLS       R8, [IP, #16] 
    /*00008568*/ BLS         SVP_NNIE_FillForwardTask_x83c4
    /*0000856c*/ B           SVP_NNIE_FillForwardTask_x83b0
    /*00008570*/ LDR         R9, [SL, #24] 
    /*00008574*/ CMP         R9, #0 
    /*00008578*/ BEQ         SVP_NNIE_FillForwardTask_x85d4
    /*0000857c*/ LDR         R3, [SP, #88]               @ 0x58 
    /*00008580*/ MOV         R0, #0 
    /*00008584*/ MOV         LR, R0 
    /*00008588*/ MOV         IP, FP 
    /*0000858c*/ SUB         R4, R3, #4 
    SVP_NNIE_FillForwardTask_x8590:
    /*00008590*/ LDR         R1, [SL, #4] 
    /*00008594*/ ADD         R0, R0, #1 
    /*00008598*/ LDRD        R2, [SL, #16] 
    /*0000859c*/ MUL         R1, R1, LR 
    /*000085a0*/ ADDS        R6, R2, R1 
    /*000085a4*/ ADC         R7, R3, #0 
    /*000085a8*/ STRD        R6, [IP], #8 
    /*000085ac*/ LDR         R2, [SL, #24] 
    /*000085b0*/ SUB         R9, IP, FP 
    /*000085b4*/ LDR         R3, [R4, #4]! 
    /*000085b8*/ CMP         R2, R0 
    /*000085bc*/ ADD         LR, LR, R3 
    /*000085c0*/ BHI         SVP_NNIE_FillForwardTask_x8590
    SVP_NNIE_FillForwardTask_x85c4:
    /*000085c4*/ LDR         R3, [SP, #80]               @ 0x50 
    /*000085c8*/ ADD         R9, R9, #15 
    /*000085cc*/ BIC         R9, R9, #15 
    /*000085d0*/ LDR         R1, [R3] 
    SVP_NNIE_FillForwardTask_x85d4:
    /*000085d4*/ ADD         R8, R8, #1 
    /*000085d8*/ ADD         FP, FP, R9 
    /*000085dc*/ CMP         R8, R1 
    /*000085e0*/ ADD         SL, SL, #48                 @ 0x30 
    /*000085e4*/ BCC         SVP_NNIE_FillForwardTask_x8508
    /*000085e8*/ LDR         R8, [SP, #28] 
    /*000085ec*/ LDR         FP, [SP, #80]               @ 0x50 
    SVP_NNIE_FillForwardTask_x85f0:
    /*000085f0*/ MOV         R4, #0 
    /*000085f4*/ B           SVP_NNIE_FillForwardTask_x889c
    /*000085f8*/ LDR         R9, [SL, #24] 
    /*000085fc*/ LDR         R3, [SL, #36]               @ 0x24 
    /*00008600*/ LDR         IP, [SL, #4] 
    /*00008604*/ CMP         R9, #0 
    /*00008608*/ MUL         IP, IP, R3 
    /*0000860c*/ BEQ         SVP_NNIE_FillForwardTask_x85d4
    /*00008610*/ MOV         R0, FP 
    /*00008614*/ MOV         R1, #0 
    SVP_NNIE_FillForwardTask_x8618:
    /*00008618*/ LDRD        R2, [SL, #16] 
    /*0000861c*/ UMLAL       R2, R3, IP, R1 
    /*00008620*/ ADD         R1, R1, #1 
    /*00008624*/ STRD        R2, [R0], #8 
    /*00008628*/ SUB         R9, R0, FP 
    /*0000862c*/ LDR         R3, [SL, #24] 
    /*00008630*/ CMP         R3, R1 
    /*00008634*/ BHI         SVP_NNIE_FillForwardTask_x8618
    /*00008638*/ B           SVP_NNIE_FillForwardTask_x85c4
    /*0000863c*/ LDR         R9, [SL, #24] 
    /*00008640*/ LDR         R3, [SL, #36]               @ 0x24 
    /*00008644*/ LDR         R6, [SL, #4] 
    /*00008648*/ CMP         R9, #0 
    /*0000864c*/ MUL         R6, R6, R3 
    /*00008650*/ BEQ         SVP_NNIE_FillForwardTask_x85d4
    /*00008654*/ ADDS        R0, R6, R6 
    /*00008658*/ MOV         R3, #0 
    /*0000865c*/ ADC         R1, R3, R3 
    /*00008660*/ MOV         IP, #0 
    /*00008664*/ ADD         LR, FP, #16 
    /*00008668*/ STR         FP, [SP, #16] 
    /*0000866c*/ MOV         R9, IP 
    /*00008670*/ MOV         FP, R8 
    /*00008674*/ MOV         R7, R1 
    /*00008678*/ MOV         R8, R0 
    SVP_NNIE_FillForwardTask_x867c:
    /*0000867c*/ MOV         R3, #2 
    /*00008680*/ MOV         R0, #1 
    /*00008684*/ MOV         R1, #0 
    /*00008688*/ ADD         R9, R9, #16 
    /*0000868c*/ UMLAL       R0, R1, R3, IP 
    /*00008690*/ ADD         LR, LR, #16 
    /*00008694*/ UMULL       R2, R3, IP, R8 
    /*00008698*/ UMULL       R4, R5, R0, R6 
    /*0000869c*/ MLA         R3, IP, R7, R3 
    /*000086a0*/ ADD         IP, IP, #1 
    /*000086a4*/ MLA         R5, R6, R1, R5 
    /*000086a8*/ LDRD        R0, [SL, #16] 
    /*000086ac*/ ADDS        R0, R0, R2 
    /*000086b0*/ ADC         R1, R1, R3 
    /*000086b4*/ MOV         R2, R0 
    /*000086b8*/ MOV         R3, R1 
    /*000086bc*/ STRD        R2, [LR, #-32]              @ 0xffffffe0 
    /*000086c0*/ LDRD        R2, [SL, #16] 
    /*000086c4*/ ADDS        R0, R2, R4 
    /*000086c8*/ ADC         R1, R3, R5 
    /*000086cc*/ STRD        R0, [LR, #-24]              @ 0xffffffe8 
    /*000086d0*/ LDR         R3, [SL, #24] 
    /*000086d4*/ CMP         R3, IP 
    /*000086d8*/ BHI         SVP_NNIE_FillForwardTask_x867c
    /*000086dc*/ LDR         R3, [SP, #80]               @ 0x50 
    /*000086e0*/ MOV         R8, FP 
    /*000086e4*/ LDR         FP, [SP, #16] 
    /*000086e8*/ LDR         R1, [R3] 
    /*000086ec*/ B           SVP_NNIE_FillForwardTask_x85d4
    /*000086f0*/ LDR         R0, [SL, #36]               @ 0x24 
    /*000086f4*/ LDR         R3, [SL, #4] 
    /*000086f8*/ LDR         R2, [SL, #40]               @ 0x28 
    /*000086fc*/ LDR         R9, [SL, #24] 
    /*00008700*/ MUL         R3, R3, R0 
    /*00008704*/ CMP         R9, #0 
    /*00008708*/ MUL         R2, R2, R3 
    /*0000870c*/ LSR         R4, R2, #1 
    /*00008710*/ BEQ         SVP_NNIE_FillForwardTask_x85d4
    /*00008714*/ LDR         R3, [SP, #32] 
    /*00008718*/ MOV         IP, #0 
    /*0000871c*/ ADD         LR, FP, #16 
    /*00008720*/ MOV         R9, IP 
    /*00008724*/ MOV         R7, #0 
    /*00008728*/ STR         R4, [SP, #16] 
    /*0000872c*/ UMULL       R2, R3, R2, R3 
    /*00008730*/ LSR         R6, R3, #2 
    /*00008734*/ LSL         R6, R6, #1 
    SVP_NNIE_FillForwardTask_x8738:
    /*00008738*/ LDR         R3, [SP, #16] 
    /*0000873c*/ ADD         R9, R9, #16 
    /*00008740*/ ADD         LR, LR, #16 
    /*00008744*/ UMULL       R0, R1, IP, R3 
    /*00008748*/ LDRD        R2, [SL, #16] 
    /*0000874c*/ ADD         IP, IP, #1 
    /*00008750*/ ADDS        R4, R2, R0 
    /*00008754*/ ADC         R5, R3, R1 
    /*00008758*/ MOV         R2, R4 
    /*0000875c*/ MOV         R3, R5 
    /*00008760*/ STRD        R2, [LR, #-32]              @ 0xffffffe0 
    /*00008764*/ LDRD        R2, [SL, #16] 
    /*00008768*/ ADDS        R4, R2, R6 
    /*0000876c*/ ADC         R5, R3, R7 
    /*00008770*/ ADDS        R2, R4, R0 
    /*00008774*/ ADC         R3, R5, R1 
    /*00008778*/ STRD        R2, [LR, #-24]              @ 0xffffffe8 
    /*0000877c*/ LDR         R3, [SL, #24] 
    /*00008780*/ CMP         R3, IP 
    /*00008784*/ BHI         SVP_NNIE_FillForwardTask_x8738
    /*00008788*/ LDR         R3, [SP, #80]               @ 0x50 
    /*0000878c*/ LDR         R1, [R3] 
    /*00008790*/ B           SVP_NNIE_FillForwardTask_x85d4
    /*00008794*/ LDR         R0, [SL, #40]               @ 0x28 
    /*00008798*/ LDR         R6, [SL, #36]               @ 0x24 
    /*0000879c*/ LDR         R3, [SL, #4] 
    /*000087a0*/ CMP         R0, #3 
    /*000087a4*/ MUL         R6, R3, R6 
    /*000087a8*/ BEQ         SVP_NNIE_FillForwardTask_x8914
    /*000087ac*/ CMP         R0, #1 
    /*000087b0*/ BNE         SVP_NNIE_FillForwardTask_x8cbc
    /*000087b4*/ LDR         R9, [SL, #24] 
    /*000087b8*/ CMP         R9, #0 
    /*000087bc*/ BEQ         SVP_NNIE_FillForwardTask_x85d4
    /*000087c0*/ MOV         R0, FP 
    /*000087c4*/ MOV         R1, #0 
    SVP_NNIE_FillForwardTask_x87c8:
    /*000087c8*/ LDRD        R2, [SL, #16] 
    /*000087cc*/ UMLAL       R2, R3, R6, R1 
    /*000087d0*/ ADD         R1, R1, #1 
    /*000087d4*/ STRD        R2, [R0], #8 
    /*000087d8*/ SUB         R9, R0, FP 
    /*000087dc*/ LDR         R3, [SL, #24] 
    /*000087e0*/ CMP         R3, R1 
    /*000087e4*/ BHI         SVP_NNIE_FillForwardTask_x87c8
    /*000087e8*/ B           SVP_NNIE_FillForwardTask_x85c4
    /*000087ec*/ LDR         IP, [SL, #4] 
    /*000087f0*/ LDR         R3, [SL, #36]               @ 0x24 
    /*000087f4*/ LDR         R9, [SL, #24] 
    /*000087f8*/ MUL         R3, IP, R3 
    /*000087fc*/ LDR         IP, [SL, #40]               @ 0x28 
    /*00008800*/ CMP         R9, #0 
    /*00008804*/ MUL         IP, IP, R3 
    /*00008808*/ BEQ         SVP_NNIE_FillForwardTask_x85d4
    /*0000880c*/ MOV         R0, FP 
    /*00008810*/ MOV         R1, #0 
    SVP_NNIE_FillForwardTask_x8814:
    /*00008814*/ LDRD        R2, [SL, #16] 
    /*00008818*/ UMLAL       R2, R3, IP, R1 
    /*0000881c*/ ADD         R1, R1, #1 
    /*00008820*/ STRD        R2, [R0], #8 
    /*00008824*/ SUB         R9, R0, FP 
    /*00008828*/ LDR         R3, [SL, #24] 
    /*0000882c*/ CMP         R3, R1 
    /*00008830*/ BHI         SVP_NNIE_FillForwardTask_x8814
    /*00008834*/ B           SVP_NNIE_FillForwardTask_x85c4
    SVP_NNIE_FillForwardTask_x8838:
    /*00008838*/ MOVW        R3, #606                    @ 0x25e 
    /*0000883c*/ MOV         R1, #51                     @ 0x33 
    /*00008840*/ STR         R3, [SP] 
    /*00008844*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*00008848*/ LDR         R3, [PC, #1300]             @ 0x0000000000008d64 
    /*0000884c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*00008850*/ MOV         R0, #3 
    /*00008854*/ MOV         R5, R8 
    /*00008858*/ LDR         FP, [SP, #80]               @ 0x50 
    /*0000885c*/ MOVW        R4, #32771                  @ 0x8003 
    /*00008860*/ LDR         R8, [SP, #28] 
    /*00008864*/ BL          0x0000000000008864          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008868*/ LDR         R2, [SL] 
    /*0000886c*/ MOV         R0, #6 
    /*00008870*/ MOV         R1, #0 
    /*00008874*/ STR         R0, [SP, #8] 
    /*00008878*/ STR         R1, [SP, #4] 
    /*0000887c*/ MOV         R3, R5 
    /*00008880*/ STR         R2, [SP] 
    /*00008884*/ MOV         R1, #51                     @ 0x33 
    /*00008888*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3bf0] 
    /*0000888c*/ MOV         R0, #3 
    /*00008890*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3bf0] 
    /*00008894*/ MOVT        R4, #41011                  @ 0xa033 
    /*00008898*/ BL          0x0000000000008898          @ HI_LOG [R_ARM_CALL: +0x0] 
    SVP_NNIE_FillForwardTask_x889c:
    /*0000889c*/ CMP         R8, #0 
    /*000088a0*/ BEQ         SVP_NNIE_FillForwardTask_x88c8
    /*000088a4*/ LDR         R2, [FP, #56]               @ 0x38 
    /*000088a8*/ MOV         R0, R8 
    /*000088ac*/ LDR         R1, [FP, #40]               @ 0x28 
    /*000088b0*/ BL          0x00000000000088B0          @ hil_mmb_flush_dcache_byaddr [R_ARM_CALL: +0x0] 
    /*000088b4*/ CMP         R0, #0 
    /*000088b8*/ BNE         SVP_NNIE_FillForwardTask_x88dc
    SVP_NNIE_FillForwardTask_x88bc:
    /*000088bc*/ LDR         R3, [SP, #36]               @ 0x24 
    /*000088c0*/ CMP         R3, #0 
    /*000088c4*/ BEQ         SVP_NNIE_FillForwardTask_x8cb0
    SVP_NNIE_FillForwardTask_x88c8:
    /*000088c8*/ MOV         R0, R4 
    /*000088cc*/ ADD         SP, SP, #44                 @ 0x2c 
    /*000088d0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_FillForwardTask_x88d4:
    /*000088d4*/ LDRD        R0, [FP, #40]               @ 0x28 
    /*000088d8*/ B           SVP_NNIE_FillForwardTask_x83ac
    SVP_NNIE_FillForwardTask_x88dc:
    /*000088dc*/ MOVW        R3, #619                    @ 0x26b 
    /*000088e0*/ MOV         R1, #51                     @ 0x33 
    /*000088e4*/ STR         R3, [SP] 
    /*000088e8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*000088ec*/ LDR         R3, [PC, #1136]             @ 0x0000000000008d64 
    /*000088f0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*000088f4*/ MOV         R0, #3 
    /*000088f8*/ BL          0x00000000000088F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000088fc*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3c20] 
    /*00008900*/ MOV         R1, #51                     @ 0x33 
    /*00008904*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3c20] 
    /*00008908*/ MOV         R0, #3 
    /*0000890c*/ BL          0x000000000000890C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008910*/ B           SVP_NNIE_FillForwardTask_x88bc
    SVP_NNIE_FillForwardTask_x8914:
    /*00008914*/ LDR         R9, [SL, #24] 
    /*00008918*/ CMP         R9, #0 
    /*0000891c*/ BEQ         SVP_NNIE_FillForwardTask_x85d4
    /*00008920*/ MOV         IP, #0 
    /*00008924*/ ADD         LR, FP, #32 
    /*00008928*/ MOV         R9, IP 
    /*0000892c*/ MOV         R2, R0 
    /*00008930*/ B           SVP_NNIE_FillForwardTask_x8938
    SVP_NNIE_FillForwardTask_x8934:
    /*00008934*/ LDR         R2, [SL, #40]               @ 0x28 
    SVP_NNIE_FillForwardTask_x8938:
    /*00008938*/ UMULL       R2, R3, R2, R6 
    /*0000893c*/ LDRD        R4, [SL, #16] 
    /*00008940*/ ADD         R9, R9, #32 
    /*00008944*/ ADD         LR, LR, #32 
    /*00008948*/ UMULL       R0, R1, R2, IP 
    /*0000894c*/ MLA         R3, IP, R3, R1 
    /*00008950*/ STRD        R0, [SP, #16] 
    /*00008954*/ STR         R3, [SP, #20] 
    /*00008958*/ LDRD        R0, [SP, #16] 
    /*0000895c*/ ADDS        R0, R0, R4 
    /*00008960*/ ADC         R1, R1, R5 
    /*00008964*/ STRD        R0, [LR, #-64]              @ 0xffffffc0 
    /*00008968*/ LDR         R3, [SL, #40]               @ 0x28 
    /*0000896c*/ MOV         R0, #1 
    /*00008970*/ MOV         R1, #0 
    /*00008974*/ LDRD        R4, [SL, #16] 
    /*00008978*/ UMLAL       R0, R1, R3, IP 
    /*0000897c*/ UMULL       R2, R3, R0, R6 
    /*00008980*/ STRD        R2, [SP, #16] 
    /*00008984*/ MLA         R3, R6, R1, R3 
    /*00008988*/ STR         R3, [SP, #20] 
    /*0000898c*/ LDRD        R2, [SP, #16] 
    /*00008990*/ ADDS        R2, R2, R4 
    /*00008994*/ MOV         R4, #0 
    /*00008998*/ ADC         R3, R3, R5 
    /*0000899c*/ STRD        R2, [LR, #-56]              @ 0xffffffc8 
    /*000089a0*/ LDR         R3, [SL, #40]               @ 0x28 
    /*000089a4*/ MOV         R5, #0 
    /*000089a8*/ LDRD        R0, [SL, #16] 
    /*000089ac*/ STRD        R4, [LR, #-40]              @ 0xffffffd8 
    /*000089b0*/ MOV         R4, #2 
    /*000089b4*/ MOV         R5, #0 
    /*000089b8*/ UMLAL       R4, R5, R3, IP 
    /*000089bc*/ ADD         IP, IP, #1 
    /*000089c0*/ UMULL       R2, R3, R4, R6 
    /*000089c4*/ STRD        R2, [SP, #16] 
    /*000089c8*/ MLA         R3, R6, R5, R3 
    /*000089cc*/ STR         R3, [SP, #20] 
    /*000089d0*/ LDRD        R2, [SP, #16] 
    /*000089d4*/ ADDS        R2, R2, R0 
    /*000089d8*/ ADC         R3, R3, R1 
    /*000089dc*/ STRD        R2, [LR, #-48]              @ 0xffffffd0 
    /*000089e0*/ LDR         R3, [SL, #24] 
    /*000089e4*/ CMP         R3, IP 
    /*000089e8*/ BHI         SVP_NNIE_FillForwardTask_x8934
    /*000089ec*/ LDR         R3, [SP, #80]               @ 0x50 
    /*000089f0*/ LDR         R1, [R3] 
    /*000089f4*/ B           SVP_NNIE_FillForwardTask_x85d4
    SVP_NNIE_FillForwardTask_x89f8:
    /*000089f8*/ LDR         R1, [SL, #24] 
    /*000089fc*/ MOV         R0, #80                     @ 0x50 
    /*00008a00*/ LDRD        R2, [FP, #40]               @ 0x28 
    /*00008a04*/ ADD         R1, R1, #15 
    /*00008a08*/ LDR         IP, [FP, #56]               @ 0x38 
    /*00008a0c*/ LSR         R1, R1, #4 
    /*00008a10*/ ADDS        R6, R2, IP 
    /*00008a14*/ ADC         R7, R3, #0 
    /*00008a18*/ MUL         R1, R0, R1 
    /*00008a1c*/ SUBS        R2, R6, R1 
    /*00008a20*/ SBC         R3, R7, #0 
    /*00008a24*/ STRD        R2, [R9] 
    /*00008a28*/ LDR         R6, [SL, #24] 
    /*00008a2c*/ LDR         R3, [FP, #56]               @ 0x38 
    /*00008a30*/ ADD         R2, R6, #15 
    /*00008a34*/ SUB         R3, R3, R1 
    /*00008a38*/ LSRS        R2, R2, #4 
    /*00008a3c*/ ADD         R3, R8, R3 
    /*00008a40*/ BEQ         SVP_NNIE_FillForwardTask_x8c78
    /*00008a44*/ MLA         R2, R2, R0, R0 
    /*00008a48*/ ADD         R1, R3, #80                 @ 0x50 
    /*00008a4c*/ STR         R8, [SP, #16] 
    /*00008a50*/ AND         R7, R6, #15 
    /*00008a54*/ LDR         R0, [SP, #88]               @ 0x58 
    /*00008a58*/ ADD         R3, R3, R2 
    /*00008a5c*/ MOV         R8, R3 
    /*00008a60*/ B           SVP_NNIE_FillForwardTask_x8c3c
    SVP_NNIE_FillForwardTask_x8a64:
    /*00008a64*/ LDR         R2, [R0] 
    /*00008a68*/ MOV         R4, #16 
    /*00008a6c*/ SUB         R6, R6, #16 
    /*00008a70*/ MOV         IP, R4 
    /*00008a74*/ CMP         R2, #0 
    /*00008a78*/ STR         R2, [R1, #-64]              @ 0xffffffc0 
    /*00008a7c*/ MOVCS       R3, R2 
    /*00008a80*/ MOVCC       R3, #0 
    SVP_NNIE_FillForwardTask_x8a84:
    /*00008a84*/ LDR         LR, [R0, #4] 
    /*00008a88*/ CMP         R3, LR 
    /*00008a8c*/ ADD         R2, R2, LR 
    /*00008a90*/ STR         LR, [R1, #-60]              @ 0xffffffc4 
    /*00008a94*/ MOVCC       R3, LR 
    /*00008a98*/ CMP         IP, #2 
    /*00008a9c*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008aa0*/ LDR         LR, [R0, #8] 
    /*00008aa4*/ CMP         R3, LR 
    /*00008aa8*/ ADD         R2, R2, LR 
    /*00008aac*/ STR         LR, [R1, #-56]              @ 0xffffffc8 
    /*00008ab0*/ MOVCC       R3, LR 
    /*00008ab4*/ CMP         IP, #3 
    /*00008ab8*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008abc*/ LDR         LR, [R0, #12] 
    /*00008ac0*/ CMP         R3, LR 
    /*00008ac4*/ ADD         R2, R2, LR 
    /*00008ac8*/ STR         LR, [R1, #-52]              @ 0xffffffcc 
    /*00008acc*/ MOVCC       R3, LR 
    /*00008ad0*/ CMP         IP, #4 
    /*00008ad4*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008ad8*/ LDR         LR, [R0, #16] 
    /*00008adc*/ CMP         R3, LR 
    /*00008ae0*/ ADD         R2, R2, LR 
    /*00008ae4*/ STR         LR, [R1, #-48]              @ 0xffffffd0 
    /*00008ae8*/ MOVCC       R3, LR 
    /*00008aec*/ CMP         IP, #5 
    /*00008af0*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008af4*/ LDR         LR, [R0, #20] 
    /*00008af8*/ CMP         R3, LR 
    /*00008afc*/ ADD         R2, R2, LR 
    /*00008b00*/ STR         LR, [R1, #-44]              @ 0xffffffd4 
    /*00008b04*/ MOVCC       R3, LR 
    /*00008b08*/ CMP         IP, #6 
    /*00008b0c*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008b10*/ LDR         LR, [R0, #24] 
    /*00008b14*/ CMP         R3, LR 
    /*00008b18*/ ADD         R2, R2, LR 
    /*00008b1c*/ STR         LR, [R1, #-40]              @ 0xffffffd8 
    /*00008b20*/ MOVCC       R3, LR 
    /*00008b24*/ CMP         IP, #7 
    /*00008b28*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008b2c*/ LDR         LR, [R0, #28] 
    /*00008b30*/ CMP         R3, LR 
    /*00008b34*/ ADD         R2, R2, LR 
    /*00008b38*/ STR         LR, [R1, #-36]              @ 0xffffffdc 
    /*00008b3c*/ MOVCC       R3, LR 
    /*00008b40*/ CMP         IP, #8 
    /*00008b44*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008b48*/ LDR         LR, [R0, #32] 
    /*00008b4c*/ CMP         R3, LR 
    /*00008b50*/ ADD         R2, R2, LR 
    /*00008b54*/ STR         LR, [R1, #-32]              @ 0xffffffe0 
    /*00008b58*/ MOVCC       R3, LR 
    /*00008b5c*/ CMP         IP, #9 
    /*00008b60*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008b64*/ LDR         LR, [R0, #36]               @ 0x24 
    /*00008b68*/ CMP         R3, LR 
    /*00008b6c*/ ADD         R2, R2, LR 
    /*00008b70*/ STR         LR, [R1, #-28]              @ 0xffffffe4 
    /*00008b74*/ MOVCC       R3, LR 
    /*00008b78*/ CMP         IP, #10 
    /*00008b7c*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008b80*/ LDR         LR, [R0, #40]               @ 0x28 
    /*00008b84*/ CMP         R3, LR 
    /*00008b88*/ ADD         R2, R2, LR 
    /*00008b8c*/ STR         LR, [R1, #-24]              @ 0xffffffe8 
    /*00008b90*/ MOVCC       R3, LR 
    /*00008b94*/ CMP         IP, #11 
    /*00008b98*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008b9c*/ LDR         LR, [R0, #44]               @ 0x2c 
    /*00008ba0*/ CMP         R3, LR 
    /*00008ba4*/ ADD         R2, R2, LR 
    /*00008ba8*/ STR         LR, [R1, #-20]              @ 0xffffffec 
    /*00008bac*/ MOVCC       R3, LR 
    /*00008bb0*/ CMP         IP, #12 
    /*00008bb4*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008bb8*/ LDR         LR, [R0, #48]               @ 0x30 
    /*00008bbc*/ CMP         R3, LR 
    /*00008bc0*/ ADD         R2, R2, LR 
    /*00008bc4*/ STR         LR, [R1, #-16] 
    /*00008bc8*/ MOVCC       R3, LR 
    /*00008bcc*/ CMP         IP, #13 
    /*00008bd0*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008bd4*/ LDR         LR, [R0, #52]               @ 0x34 
    /*00008bd8*/ CMP         R3, LR 
    /*00008bdc*/ ADD         R2, R2, LR 
    /*00008be0*/ STR         LR, [R1, #-12] 
    /*00008be4*/ MOVCC       R3, LR 
    /*00008be8*/ CMP         IP, #14 
    /*00008bec*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008bf0*/ LDR         LR, [R0, #56]               @ 0x38 
    /*00008bf4*/ CMP         R3, LR 
    /*00008bf8*/ ADD         R2, R2, LR 
    /*00008bfc*/ STR         LR, [R1, #-8] 
    /*00008c00*/ MOVCC       R3, LR 
    /*00008c04*/ CMP         IP, #16 
    /*00008c08*/ BNE         SVP_NNIE_FillForwardTask_x8c20
    /*00008c0c*/ LDR         IP, [R0, #60]               @ 0x3c 
    /*00008c10*/ CMP         R3, IP 
    /*00008c14*/ ADD         R2, R2, IP 
    /*00008c18*/ STR         IP, [R1, #-4] 
    /*00008c1c*/ MOVCC       R3, IP 
    SVP_NNIE_FillForwardTask_x8c20:
    /*00008c20*/ STR         R3, [R1, #-80]              @ 0xffffffb0 
    /*00008c24*/ ADD         R1, R1, #80                 @ 0x50 
    /*00008c28*/ STR         R4, [R1, #-156]             @ 0xffffff64 
    /*00008c2c*/ ADD         R0, R0, #64                 @ 0x40 
    /*00008c30*/ STR         R2, [R1, #-152]             @ 0xffffff68 
    /*00008c34*/ CMP         R1, R8 
    /*00008c38*/ BEQ         SVP_NNIE_FillForwardTask_x8c74
    SVP_NNIE_FillForwardTask_x8c3c:
    /*00008c3c*/ CMP         R6, #15 
    /*00008c40*/ BHI         SVP_NNIE_FillForwardTask_x8a64
    /*00008c44*/ CMP         R7, #0 
    /*00008c48*/ BEQ         SVP_NNIE_FillForwardTask_x8c8c
    /*00008c4c*/ LDR         R2, [R0] 
    /*00008c50*/ MOV         R4, R7 
    /*00008c54*/ CMP         R2, #0 
    /*00008c58*/ STR         R2, [R1, #-64]              @ 0xffffffc0 
    /*00008c5c*/ MOVCS       R3, R2 
    /*00008c60*/ MOVCC       R3, #0 
    /*00008c64*/ CMP         R7, #1 
    /*00008c68*/ BEQ         SVP_NNIE_FillForwardTask_x8c20
    /*00008c6c*/ MOV         IP, R7 
    /*00008c70*/ B           SVP_NNIE_FillForwardTask_x8a84
    SVP_NNIE_FillForwardTask_x8c74:
    /*00008c74*/ LDR         R8, [SP, #16] 
    SVP_NNIE_FillForwardTask_x8c78:
    /*00008c78*/ MOV         R3, #0 
    /*00008c7c*/ ADD         R9, R9, #16 
    /*00008c80*/ STR         R3, [R9, #-8] 
    /*00008c84*/ STR         R3, [R9, #-4] 
    /*00008c88*/ B           SVP_NNIE_FillForwardTask_x8474
    SVP_NNIE_FillForwardTask_x8c8c:
    /*00008c8c*/ MOV         R4, R7 
    /*00008c90*/ MOV         R2, R7 
    /*00008c94*/ MOV         R3, R7 
    /*00008c98*/ B           SVP_NNIE_FillForwardTask_x8c20
    SVP_NNIE_FillForwardTask_x8c9c:
    /*00008c9c*/ LDR         R3, [FP, #4] 
    /*00008ca0*/ CMP         R3, #0 
    /*00008ca4*/ MOVEQ       R9, R8 
    /*00008ca8*/ BNE         SVP_NNIE_FillForwardTask_x840c
    /*00008cac*/ B           SVP_NNIE_FillForwardTask_x845c
    SVP_NNIE_FillForwardTask_x8cb0:
    /*00008cb0*/ MOV         R0, R8 
    /*00008cb4*/ BL          0x0000000000008CB4          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    /*00008cb8*/ B           SVP_NNIE_FillForwardTask_x88c8
    SVP_NNIE_FillForwardTask_x8cbc:
    /*00008cbc*/ MOV         R3, #572                    @ 0x23c 
    /*00008cc0*/ MOV         R1, #51                     @ 0x33 
    /*00008cc4*/ STR         R3, [SP] 
    /*00008cc8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*00008ccc*/ LDR         R3, [PC, #144]              @ 0x0000000000008d64 
    /*00008cd0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*00008cd4*/ MOV         R0, #3 
    /*00008cd8*/ MOV         R5, R8 
    /*00008cdc*/ LDR         FP, [SP, #80]               @ 0x50 
    /*00008ce0*/ MOVW        R4, #32771                  @ 0x8003 
    /*00008ce4*/ LDR         R8, [SP, #28] 
    /*00008ce8*/ BL          0x0000000000008CE8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008cec*/ LDR         R0, [SL, #40]               @ 0x28 
    /*00008cf0*/ MOV         R3, R5 
    /*00008cf4*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3bb0] 
    /*00008cf8*/ MOV         R1, #51                     @ 0x33 
    /*00008cfc*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3bb0] 
    /*00008d00*/ MOVT        R4, #41011                  @ 0xa033 
    /*00008d04*/ STR         R0, [SP] 
    /*00008d08*/ MOV         R0, #3 
    /*00008d0c*/ BL          0x0000000000008D0C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008d10*/ B           SVP_NNIE_FillForwardTask_x889c
    SVP_NNIE_FillForwardTask_x8d14:
    /*00008d14*/ MOVW        R3, #434                    @ 0x1b2 
    /*00008d18*/ MOV         R1, #51                     @ 0x33 
    /*00008d1c*/ STR         R3, [SP] 
    /*00008d20*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*00008d24*/ LDR         R3, [PC, #56]               @ 0x0000000000008d64 
    /*00008d28*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*00008d2c*/ MOV         R0, #3 
    /*00008d30*/ MOVW        R4, #32780                  @ 0x800c 
    /*00008d34*/ BL          0x0000000000008D34          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008d38*/ LDR         R3, [FP, #56]               @ 0x38 
    /*00008d3c*/ LDRD        R6, [FP, #40]               @ 0x28 
    /*00008d40*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b70] 
    /*00008d44*/ MOV         R1, #51                     @ 0x33 
    /*00008d48*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b70] 
    /*00008d4c*/ STR         R3, [SP, #8] 
    /*00008d50*/ MOV         R0, #3 
    /*00008d54*/ STRD        R6, [SP] 
    /*00008d58*/ MOVT        R4, #41011                  @ 0xa033 
    /*00008d5c*/ BL          0x0000000000008D5C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008d60*/ B           SVP_NNIE_FillForwardTask_x88c8
    /*00008d64*/ LDRDEQ      R0, [R0], -IP 
FUNC_END SVP_NNIE_FillForwardTask


FUNC_BEGIN SVP_NNIE_FillForwardWithRoiTask
    /*00008d68*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00008d6c*/ SUB         SP, SP, #276                @ 0x114 
    /*00008d70*/ MOV         R8, R3 
    /*00008d74*/ MOVW        R6, #13968                  @ 0x3690 
    /*00008d78*/ LDR         LR, [SP, #320]              @ 0x140 
    /*00008d7c*/ ADD         R6, R8, R6 
    /*00008d80*/ LDR         IP, [SP, #324]              @ 0x144 
    /*00008d84*/ MOV         R4, #0 
    /*00008d88*/ SUB         LR, LR, #1 
    /*00008d8c*/ STR         R3, [SP, #80]               @ 0x50 
    /*00008d90*/ CLZ         LR, LR 
    /*00008d94*/ MOV         R3, #0 
    /*00008d98*/ MOV         R5, #0 
    /*00008d9c*/ STRH        R3, [IP, #2] 
    /*00008da0*/ STR         R3, [IP, #4] 
    /*00008da4*/ LSR         LR, LR, #5 
    /*00008da8*/ STRD        R4, [IP, #8] 
    /*00008dac*/ STRH        LR, [IP] 
    /*00008db0*/ MOVW        LR, #1692                   @ 0x69c 
    /*00008db4*/ STR         R1, [SP, #232]              @ 0xe8 
    /*00008db8*/ LDRD        R6, [R6] 
    /*00008dbc*/ STR         R2, [SP, #148]              @ 0x94 
    /*00008dc0*/ LDR         R2, [SP, #316]              @ 0x13c 
    /*00008dc4*/ STRD        R6, [IP, #16] 
    /*00008dc8*/ STR         R3, [SP, #248]              @ 0xf8 
    /*00008dcc*/ LDR         R2, [R2, #12] 
    /*00008dd0*/ STR         R3, [SP, #256]              @ 0x100 
    /*00008dd4*/ STR         R3, [SP, #264]              @ 0x108 
    /*00008dd8*/ MLA         R2, LR, R2, R8 
    /*00008ddc*/ STR         R3, [SP, #252]              @ 0xfc 
    /*00008de0*/ STR         R3, [SP, #260]              @ 0x104 
    /*00008de4*/ STR         R3, [SP, #268]              @ 0x10c 
    /*00008de8*/ LDR         R2, [R2, #24] 
    /*00008dec*/ STR         R2, [IP, #24] 
    /*00008df0*/ LDR         R2, [SP, #316]              @ 0x13c 
    /*00008df4*/ LDR         R2, [R2, #12] 
    /*00008df8*/ MLA         LR, LR, R2, R8 
    /*00008dfc*/ LDR         R2, [LR, #28] 
    /*00008e00*/ STR         R2, [IP, #28] 
    /*00008e04*/ LDR         R2, [SP, #316]              @ 0x13c 
    /*00008e08*/ LDRD        R6, [R2, #48]               @ 0x30 
    /*00008e0c*/ STRD        R4, [IP, #40]               @ 0x28 
    /*00008e10*/ STRD        R6, [IP, #32] 
    /*00008e14*/ LDRD        R4, [R2, #24] 
    /*00008e18*/ STRD        R4, [IP, #48]               @ 0x30 
    /*00008e1c*/ LDR         R2, [R1, #24] 
    /*00008e20*/ STR         R3, [IP, #60]               @ 0x3c 
    /*00008e24*/ STR         R2, [IP, #56]               @ 0x38 
    /*00008e28*/ LDR         R3, [R0, #48]               @ 0x30 
    /*00008e2c*/ CMP         R3, #0 
    /*00008e30*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x984c
    /*00008e34*/ MOV         LR, R0 
    /*00008e38*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*00008e3c*/ LDR         IP, [LR, #192]!             @ 0xc0 
    /*00008e40*/ CMP         IP, LR 
    /*00008e44*/ LDRD        R0, [R3, #48]               @ 0x30 
    /*00008e48*/ BNE         SVP_NNIE_FillForwardWithRoiTask_x9718
    SVP_NNIE_FillForwardWithRoiTask_x8e4c:
    /*00008e4c*/ LDR         R2, [R3, #64]               @ 0x40 
    SVP_NNIE_FillForwardWithRoiTask_x8e50:
    /*00008e50*/ BL          0x0000000000008E50          @ CMPI_Remap_Cached [R_ARM_CALL: +0x0] 
    /*00008e54*/ SUBS        R3, R0, #0 
    /*00008e58*/ STR         R3, [SP, #144]              @ 0x90 
    /*00008e5c*/ MOVNE       R3, #0 
    /*00008e60*/ STRNE       R3, [SP, #236]              @ 0xec 
    /*00008e64*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb460
    SVP_NNIE_FillForwardWithRoiTask_x8e68:
    /*00008e68*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*00008e6c*/ LDR         R3, [R3] 
    /*00008e70*/ CMP         R3, #0 
    /*00008e74*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xaf70
    /*00008e78*/ LDR         R3, [SP, #144]              @ 0x90 
    /*00008e7c*/ MOV         R2, #0 
    /*00008e80*/ LDR         R1, [SP, #232]              @ 0xe8 
    /*00008e84*/ LDR         LR, [SP, #316]              @ 0x13c 
    /*00008e88*/ SUB         R0, R3, #4 
    /*00008e8c*/ MOV         R3, R2 
    SVP_NNIE_FillForwardWithRoiTask_x8e90:
    /*00008e90*/ LDR         IP, [R1, #4] 
    /*00008e94*/ ADD         R2, R2, #1 
    /*00008e98*/ ADD         R3, R3, #4 
    /*00008e9c*/ ADD         R1, R1, #48                 @ 0x30 
    /*00008ea0*/ STR         IP, [R0, #4]! 
    /*00008ea4*/ LDR         IP, [LR] 
    /*00008ea8*/ CMP         IP, R2 
    /*00008eac*/ BHI         SVP_NNIE_FillForwardWithRoiTask_x8e90
    /*00008eb0*/ LDR         R2, [SP, #316]              @ 0x13c 
    /*00008eb4*/ LDR         R2, [R2, #4] 
    /*00008eb8*/ CMP         R2, #0 
    /*00008ebc*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x8ef4
    SVP_NNIE_FillForwardWithRoiTask_x8ec0:
    /*00008ec0*/ LDR         LR, [SP, #144]              @ 0x90 
    /*00008ec4*/ MOV         R2, #0 
    /*00008ec8*/ LDR         R1, [SP, #312]              @ 0x138 
    /*00008ecc*/ LDR         R4, [SP, #316]              @ 0x13c 
    /*00008ed0*/ ADD         R0, LR, R3 
    SVP_NNIE_FillForwardWithRoiTask_x8ed4:
    /*00008ed4*/ LDR         R3, [R1, #4] 
    /*00008ed8*/ ADD         R2, R2, #1 
    /*00008edc*/ ADD         R1, R1, #48                 @ 0x30 
    /*00008ee0*/ STR         R3, [R0], #4 
    /*00008ee4*/ SUB         R3, R0, LR 
    /*00008ee8*/ LDR         IP, [R4, #4] 
    /*00008eec*/ CMP         IP, R2 
    /*00008ef0*/ BHI         SVP_NNIE_FillForwardWithRoiTask_x8ed4
    SVP_NNIE_FillForwardWithRoiTask_x8ef4:
    /*00008ef4*/ LDR         LR, [SP, #144]              @ 0x90 
    /*00008ef8*/ ADD         IP, R3, #15 
    /*00008efc*/ BIC         IP, IP, #15 
    /*00008f00*/ ADD         R2, LR, R3 
    /*00008f04*/ STR         R2, [SP, #224]              @ 0xe0 
    /*00008f08*/ MOV         R0, R2 
    /*00008f0c*/ SUBS        R2, IP, R3 
    /*00008f10*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x8f24
    /*00008f14*/ MOV         R1, #0 
    /*00008f18*/ ADD         R3, LR, IP 
    /*00008f1c*/ STR         R3, [SP, #224]              @ 0xe0 
    /*00008f20*/ BL          0x0000000000008F20          @ memset [R_ARM_CALL: +0x0] 
    SVP_NNIE_FillForwardWithRoiTask_x8f24:
    /*00008f24*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*00008f28*/ MOVW        R2, #1692                   @ 0x69c 
    /*00008f2c*/ LDR         R1, [SP, #80]               @ 0x50 
    /*00008f30*/ LDR         R3, [R3, #12] 
    /*00008f34*/ MLA         R2, R2, R3, R1 
    /*00008f38*/ LDRH        R1, [R2, #20] 
    /*00008f3c*/ CMP         R1, #0 
    /*00008f40*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x9778
    /*00008f44*/ LDR         R2, [SP, #316]              @ 0x13c 
    /*00008f48*/ LDR         IP, [R2, #8] 
    /*00008f4c*/ LDR         R2, [SP, #148]              @ 0x94 
    /*00008f50*/ CMP         IP, #1 
    /*00008f54*/ LDR         R2, [R2, #36]               @ 0x24 
    /*00008f58*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa888
    /*00008f5c*/ CMP         R1, #1 
    /*00008f60*/ STR         R2, [SP, #248]              @ 0xf8 
    /*00008f64*/ LDRNE       R0, [SP, #148]              @ 0x94 
    /*00008f68*/ LDRNE       R0, [R0, #84]               @ 0x54 
    /*00008f6c*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x8f74
    SVP_NNIE_FillForwardWithRoiTask_x8f70:
    /*00008f70*/ STR         R0, [SP, #252]              @ 0xfc 
    SVP_NNIE_FillForwardWithRoiTask_x8f74:
    /*00008f74*/ LDR         R5, [SP, #80]               @ 0x50 
    /*00008f78*/ MOVW        LR, #1692                   @ 0x69c 
    /*00008f7c*/ MOV         R0, #104                    @ 0x68 
    /*00008f80*/ MOVW        R4, #13616                  @ 0x3530 
    /*00008f84*/ MLA         LR, LR, R3, R5 
    /*00008f88*/ LDR         LR, [LR, #1696]             @ 0x6a0 
    /*00008f8c*/ MLA         R0, R0, LR, R5 
    /*00008f90*/ LDR         R4, [R0, R4] 
    /*00008f94*/ CMP         R4, #0 
    /*00008f98*/ MOVW        R4, #13592                  @ 0x3518 
    /*00008f9c*/ LDR         R0, [R0, R4] 
    /*00008fa0*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x9af8
    /*00008fa4*/ CMN         R0, #1 
    /*00008fa8*/ MOVW        R4, #13560                  @ 0x34f8 
    /*00008fac*/ LSRNE       R0, R0, #2 
    /*00008fb0*/ MOVNE       R6, R5 
    /*00008fb4*/ MOVEQ       R6, R5 
    /*00008fb8*/ MOV         R5, #104                    @ 0x68 
    /*00008fbc*/ ADDNE       R0, R0, #2 
    /*00008fc0*/ MLA         LR, R5, LR, R6 
    /*00008fc4*/ MOVW        R5, #13564                  @ 0x34fc 
    /*00008fc8*/ MOVEQ       R0, #2 
    /*00008fcc*/ LDR         R4, [LR, R4] 
    /*00008fd0*/ LDR         LR, [LR, R5] 
    /*00008fd4*/ MUL         LR, LR, R4 
    /*00008fd8*/ MUL         LR, R2, LR 
    /*00008fdc*/ ADD         R0, R0, LR 
    /*00008fe0*/ CMP         LR, #0 
    /*00008fe4*/ LSL         R0, R0, #4 
    /*00008fe8*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa794
    /*00008fec*/ SUB         LR, LR, #1 
    /*00008ff0*/ LSR         LR, LR, #2 
    /*00008ff4*/ ADD         LR, LR, #1 
    /*00008ff8*/ LSL         LR, LR, #4 
    SVP_NNIE_FillForwardWithRoiTask_x8ffc:
    /*00008ffc*/ ADD         R0, LR, R0 
    /*00009000*/ LSL         LR, R2, #2 
    /*00009004*/ ADD         LR, LR, #15 
    /*00009008*/ CMP         R1, #1 
    /*0000900c*/ STR         R0, [SP, #256]              @ 0x100 
    /*00009010*/ BIC         R0, LR, #15 
    /*00009014*/ ADD         R2, R0, R2, LSL #4 
    /*00009018*/ STR         R2, [SP, #264]              @ 0x108 
    /*0000901c*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x90c0
    /*00009020*/ MOVW        R2, #1692                   @ 0x69c 
    /*00009024*/ LDR         R5, [SP, #80]               @ 0x50 
    /*00009028*/ MLA         R2, R3, R2, R2 
    /*0000902c*/ MOV         LR, #104                    @ 0x68 
    /*00009030*/ MOVW        R4, #13616                  @ 0x3530 
    /*00009034*/ ADD         R2, R5, R2 
    /*00009038*/ LDR         R0, [R2, #8] 
    /*0000903c*/ MLA         LR, LR, R0, R5 
    /*00009040*/ LDR         R2, [LR, R4] 
    /*00009044*/ CMP         R2, #0 
    /*00009048*/ MOVW        R2, #13592                  @ 0x3518 
    /*0000904c*/ LDR         R2, [LR, R2] 
    /*00009050*/ BNE         SVP_NNIE_FillForwardWithRoiTask_xa79c
    /*00009054*/ MOV         LR, #104                    @ 0x68 
    /*00009058*/ MOVW        R4, #13560                  @ 0x34f8 
    /*0000905c*/ MLA         R0, LR, R0, R5 
    /*00009060*/ MOVW        R5, #13564                  @ 0x34fc 
    /*00009064*/ CMN         R2, #1 
    /*00009068*/ LDR         LR, [SP, #252]              @ 0xfc 
    /*0000906c*/ LSRNE       R2, R2, #2 
    /*00009070*/ ADDNE       R2, R2, #2 
    /*00009074*/ MOVEQ       R2, #2 
    /*00009078*/ CMP         LR, #0 
    /*0000907c*/ LDR         R4, [R0, R4] 
    /*00009080*/ LDR         R0, [R0, R5] 
    /*00009084*/ MUL         R0, R0, R4 
    /*00009088*/ MLA         R0, LR, R0, R2 
    /*0000908c*/ SUBNE       R2, LR, #1 
    /*00009090*/ MOVEQ       R2, #16 
    /*00009094*/ LSRNE       R2, R2, #2 
    /*00009098*/ ADDNE       R2, R2, #1 
    /*0000909c*/ LSL         R0, R0, #4 
    /*000090a0*/ LSLNE       R2, R2, #4 
    SVP_NNIE_FillForwardWithRoiTask_x90a4:
    /*000090a4*/ ADD         R0, R2, R0 
    /*000090a8*/ LSL         R2, LR, #2 
    /*000090ac*/ ADD         R2, R2, #15 
    /*000090b0*/ STR         R0, [SP, #260]              @ 0x104 
    /*000090b4*/ BIC         R2, R2, #15 
    /*000090b8*/ ADD         LR, R2, LR, LSL #4 
    /*000090bc*/ STR         LR, [SP, #268]              @ 0x10c 
    SVP_NNIE_FillForwardWithRoiTask_x90c0:
    /*000090c0*/ ADD         R2, SP, #248                @ 0xf8 
    /*000090c4*/ STR         R2, [SP, #92]               @ 0x5c 
    /*000090c8*/ LDR         R2, [SP, #224]              @ 0xe0 
    /*000090cc*/ MOV         R7, #61440                  @ 0xf000 
    /*000090d0*/ MOVT        R7, #65535                  @ 0xffff 
    /*000090d4*/ MOV         R0, R3 
    /*000090d8*/ SUB         R2, R2, #8 
    /*000090dc*/ STR         R2, [SP, #128]              @ 0x80 
    /*000090e0*/ LDR         R2, [SP, #148]              @ 0x94 
    /*000090e4*/ STR         R2, [SP, #88]               @ 0x58 
    /*000090e8*/ MOV         R2, #0 
    /*000090ec*/ MOV         LR, R2 
    /*000090f0*/ STR         R2, [SP, #84]               @ 0x54 
    SVP_NNIE_FillForwardWithRoiTask_x90f4:
    /*000090f4*/ MOVW        R3, #423                    @ 0x1a7 
    /*000090f8*/ LDR         R5, [SP, #88]               @ 0x58 
    /*000090fc*/ MLA         R3, R3, R0, LR 
    /*00009100*/ ADD         R0, SP, #256                @ 0x100 
    /*00009104*/ CMP         IP, #1 
    /*00009108*/ LSL         R2, LR, #3 
    /*0000910c*/ LDR         R0, [R0, LR, LSL #2] 
    /*00009110*/ LDR         R9, [SP, #148]              @ 0x94 
    /*00009114*/ MOVNE       R9, R5 
    /*00009118*/ MOV         R4, R0 
    /*0000911c*/ STR         R0, [SP, #52]               @ 0x34 
    /*00009120*/ ADD         R0, SP, #264                @ 0x108 
    /*00009124*/ LDR         R0, [R0, LR, LSL #2] 
    /*00009128*/ ADD         LR, LR, #1 
    /*0000912c*/ CMP         LR, R1 
    /*00009130*/ STR         LR, [SP, #84]               @ 0x54 
    /*00009134*/ ADD         R1, R4, R0 
    /*00009138*/ STR         R1, [SP, #24] 
    /*0000913c*/ MOV         IP, R1 
    /*00009140*/ LDR         R1, [SP, #80]               @ 0x50 
    /*00009144*/ ADD         R3, R1, R3, LSL #2 
    /*00009148*/ LDR         R1, [R3, #1696]             @ 0x6a0 
    /*0000914c*/ BCS         SVP_NNIE_FillForwardWithRoiTask_x916c
    /*00009150*/ ADD         R3, SP, #256                @ 0x100 
    /*00009154*/ LDR         R0, [R3, LR, LSL #2] 
    /*00009158*/ ADD         R3, SP, #264                @ 0x108 
    /*0000915c*/ LDR         R3, [R3, LR, LSL #2] 
    /*00009160*/ ADD         R0, IP, R0 
    /*00009164*/ ADD         R3, R0, R3 
    /*00009168*/ STR         R3, [SP, #24] 
    SVP_NNIE_FillForwardWithRoiTask_x916c:
    /*0000916c*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*00009170*/ MOV         IP, #104                    @ 0x68 
    /*00009174*/ MOVW        R0, #13616                  @ 0x3530 
    /*00009178*/ LDR         LR, [R3, #64]               @ 0x40 
    /*0000917c*/ LDR         R3, [SP, #80]               @ 0x50 
    /*00009180*/ MLA         IP, IP, R1, R3 
    /*00009184*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*00009188*/ LDRD        R4, [R3, #48]               @ 0x30 
    /*0000918c*/ ADD         R3, R2, #8 
    /*00009190*/ STR         R3, [SP, #168]              @ 0xa8 
    /*00009194*/ ADDS        SL, R4, LR 
    /*00009198*/ LDR         LR, [SP, #24] 
    /*0000919c*/ LDR         R4, [SP, #128]              @ 0x80 
    /*000091a0*/ ADC         FP, R5, #0 
    /*000091a4*/ SUBS        R2, SL, LR 
    /*000091a8*/ SBC         R3, FP, #0 
    /*000091ac*/ STRD        R2, [R4, #8]! 
    /*000091b0*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*000091b4*/ LDR         R2, [IP, R0] 
    /*000091b8*/ STR         R4, [SP, #128]              @ 0x80 
    /*000091bc*/ LDR         R3, [R3, #64]               @ 0x40 
    /*000091c0*/ CMP         R2, #0 
    /*000091c4*/ LDR         R2, [SP, #52]               @ 0x34 
    /*000091c8*/ STR         R3, [SP, #56]               @ 0x38 
    /*000091cc*/ SUB         R3, R3, LR 
    /*000091d0*/ ADD         R5, R3, R2 
    /*000091d4*/ LDR         R2, [SP, #144]              @ 0x90 
    /*000091d8*/ ADD         R0, R2, R5 
    /*000091dc*/ ADD         R3, R2, R3 
    /*000091e0*/ STR         R0, [SP, #68]               @ 0x44 
    /*000091e4*/ STR         R3, [SP, #108]              @ 0x6c 
    /*000091e8*/ BNE         SVP_NNIE_FillForwardWithRoiTask_x9b50
    /*000091ec*/ MOVW        R3, #13592                  @ 0x3518 
    /*000091f0*/ LDR         R2, [SP, #92]               @ 0x5c 
    /*000091f4*/ LDR         R3, [IP, R3] 
    /*000091f8*/ MOVW        FP, #13564                  @ 0x34fc 
    /*000091fc*/ LDR         IP, [SP, #80]               @ 0x50 
    /*00009200*/ MOVW        SL, #13572                  @ 0x3504 
    /*00009204*/ CMN         R3, #1 
    /*00009208*/ LDR         R2, [R2] 
    /*0000920c*/ STR         R3, [SP, #104]              @ 0x68 
    /*00009210*/ LSRNE       R3, R3, #2 
    /*00009214*/ ADDNE       R3, R3, #1 
    /*00009218*/ MOVEQ       R3, #16 
    /*0000921c*/ LSL         R0, R2, #2 
    /*00009220*/ MOVW        R8, #13580                  @ 0x350c 
    /*00009224*/ LSLNE       R3, R3, #4 
    /*00009228*/ STR         R3, [SP, #156]              @ 0x9c 
    /*0000922c*/ MOV         R3, #104                    @ 0x68 
    /*00009230*/ MOVW        R6, #13548                  @ 0x34ec 
    /*00009234*/ MLA         R3, R3, R1, IP 
    /*00009238*/ LDR         R1, [SP, #68]               @ 0x44 
    /*0000923c*/ LSL         IP, R2, #3 
    /*00009240*/ MOVW        R4, #13588                  @ 0x3514 
    /*00009244*/ ADD         R1, R1, R0 
    /*00009248*/ STR         R1, [SP, #124]              @ 0x7c 
    /*0000924c*/ ADD         R1, R1, IP 
    /*00009250*/ STR         R1, [SP, #120]              @ 0x78 
    /*00009254*/ SUB         R1, R1, R0 
    /*00009258*/ STR         R2, [SP, #32] 
    /*0000925c*/ STR         R0, [SP, #64]               @ 0x40 
    /*00009260*/ LSL         R2, R2, #4 
    /*00009264*/ MOV         R5, R1 
    /*00009268*/ ADD         IP, R5, IP 
    /*0000926c*/ STR         IP, [SP, #100]              @ 0x64 
    /*00009270*/ MOVW        IP, #13560                  @ 0x34f8 
    /*00009274*/ LDR         IP, [R3, IP] 
    /*00009278*/ MOVW        LR, #13600                  @ 0x3520 
    /*0000927c*/ STR         R1, [SP, #116]              @ 0x74 
    /*00009280*/ LDRD        R0, [R9, #16] 
    /*00009284*/ MOV         R5, IP 
    /*00009288*/ STR         IP, [SP, #112]              @ 0x70 
    /*0000928c*/ LDR         IP, [R3, FP] 
    /*00009290*/ STR         R2, [SP, #152]              @ 0x98 
    /*00009294*/ MOV         FP, IP 
    /*00009298*/ STR         IP, [SP, #44]               @ 0x2c 
    /*0000929c*/ LDR         IP, [R3, SL] 
    /*000092a0*/ STR         IP, [SP, #136]              @ 0x88 
    /*000092a4*/ MOVW        IP, #13576                  @ 0x3508 
    /*000092a8*/ LDR         IP, [R3, IP] 
    /*000092ac*/ STR         IP, [SP, #192]              @ 0xc0 
    /*000092b0*/ LDR         IP, [R3, R8] 
    /*000092b4*/ STR         IP, [SP, #48]               @ 0x30 
    /*000092b8*/ MOVW        IP, #13584                  @ 0x3510 
    /*000092bc*/ LDR         IP, [R3, IP] 
    /*000092c0*/ STR         IP, [SP, #160]              @ 0xa0 
    /*000092c4*/ LDR         IP, [R3, R6] 
    /*000092c8*/ STR         IP, [SP, #60]               @ 0x3c 
    /*000092cc*/ MOVW        IP, #13552                  @ 0x34f0 
    /*000092d0*/ LDR         IP, [R3, IP] 
    /*000092d4*/ STR         IP, [SP, #132]              @ 0x84 
    /*000092d8*/ LDR         IP, [R3, R4] 
    /*000092dc*/ STR         IP, [SP, #140]              @ 0x8c 
    /*000092e0*/ LDR         R4, [R3, LR] 
    /*000092e4*/ BL          0x00000000000092E4          @ CMPI_Remap_Nocache [R_ARM_CALL: +0x0] 
    /*000092e8*/ MUL         R3, FP, R5 
    /*000092ec*/ LDR         R2, [SP, #32] 
    /*000092f0*/ STR         R3, [SP, #196]              @ 0xc4 
    /*000092f4*/ MUL         R3, R3, R2 
    /*000092f8*/ STR         R3, [SP, #96]               @ 0x60 
    /*000092fc*/ SUBS        R3, R0, #0 
    /*00009300*/ STR         R3, [SP, #72]               @ 0x48 
    /*00009304*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb2b4
    /*00009308*/ LDR         R3, [SP, #104]              @ 0x68 
    /*0000930c*/ MOV         R1, R2 
    /*00009310*/ LDR         R2, [SP, #96]               @ 0x60 
    /*00009314*/ CMN         R3, #1 
    /*00009318*/ LSRNE       R3, R3, #2 
    /*0000931c*/ ADDNE       R3, R3, #2 
    /*00009320*/ MOVEQ       R3, #2 
    /*00009324*/ CMP         R1, #0 
    /*00009328*/ ADD         R3, R2, R3 
    /*0000932c*/ LSL         R3, R3, #4 
    /*00009330*/ BNE         SVP_NNIE_FillForwardWithRoiTask_x9ec4
    /*00009334*/ LDR         R2, [SP, #132]              @ 0x84 
    /*00009338*/ LDR         R0, [SP, #156]              @ 0x9c 
    /*0000933c*/ CMP         R2, #255                    @ 0xff 
    /*00009340*/ LDR         R2, [SP, #108]              @ 0x6c 
    /*00009344*/ ADD         R1, R2, #16 
    /*00009348*/ STR         R1, [SP, #220]              @ 0xdc 
    /*0000934c*/ STR         R3, [R2, #12] 
    /*00009350*/ ADD         R3, R3, #16 
    /*00009354*/ STR         R3, [R2] 
    /*00009358*/ ADD         R1, R1, R0 
    /*0000935c*/ LDR         R3, [SP, #112]              @ 0x70 
    /*00009360*/ LDR         R0, [SP, #32] 
    /*00009364*/ STR         R1, [SP, #176]              @ 0xb0 
    /*00009368*/ LSL         R3, R3, #12 
    /*0000936c*/ STR         R3, [SP, #208]              @ 0xd0 
    /*00009370*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*00009374*/ STR         R0, [R2, #4] 
    /*00009378*/ LDR         R0, [SP, #96]               @ 0x60 
    /*0000937c*/ LSL         R3, R3, #12 
    /*00009380*/ STR         R3, [SP, #204]              @ 0xcc 
    /*00009384*/ LDR         R3, [SP, #140]              @ 0x8c 
    /*00009388*/ ADD         R1, R1, R0, LSL #4 
    /*0000938c*/ STR         R1, [SP, #216]              @ 0xd8 
    /*00009390*/ LSR         R3, R3, #1 
    /*00009394*/ STR         R3, [SP, #212]              @ 0xd4 
    /*00009398*/ BHI         SVP_NNIE_FillForwardWithRoiTask_xaf98
    SVP_NNIE_FillForwardWithRoiTask_x939c:
    /*0000939c*/ LDR         R3, [SP, #104]              @ 0x68 
    /*000093a0*/ LDR         R2, [SP, #32] 
    /*000093a4*/ CMP         R3, #1 
    /*000093a8*/ MVN         R3, #11 
    /*000093ac*/ MUL         IP, R3, R2 
    /*000093b0*/ BLS         SVP_NNIE_FillForwardWithRoiTask_xa854
    /*000093b4*/ LDR         R3, [SP, #104]              @ 0x68 
    /*000093b8*/ CMP         R2, #0 
    /*000093bc*/ SUB         R8, R3, #1 
    /*000093c0*/ LDR         R3, [SP, #140]              @ 0x8c 
    /*000093c4*/ LSL         SL, R3, #12 
    /*000093c8*/ LDR         R3, [SP, #212]              @ 0xd4 
    /*000093cc*/ LSL         R9, R3, #12 
    /*000093d0*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb4d0
    /*000093d4*/ LDR         R3, [SP, #100]              @ 0x64 
    /*000093d8*/ MOV         R4, SL 
    /*000093dc*/ LDR         R2, [SP, #116]              @ 0x74 
    /*000093e0*/ MOV         R5, #0 
    /*000093e4*/ STR         R7, [SP, #24] 
    /*000093e8*/ ADD         IP, R3, IP 
    /*000093ec*/ LDR         R7, [SP, #68]               @ 0x44 
    /*000093f0*/ SUB         R6, R2, #4 
    /*000093f4*/ MOV         FP, R3 
    SVP_NNIE_FillForwardWithRoiTask_x93f8:
    /*000093f8*/ ADD         R5, R5, #1 
    /*000093fc*/ SUB         LR, R4, SL 
    /*00009400*/ MOV         R1, R6 
    /*00009404*/ MOV         R2, FP 
    /*00009408*/ MOV         R3, R7 
    SVP_NNIE_FillForwardWithRoiTask_x940c:
    /*0000940c*/ LDR         R0, [R1, #4]! 
    /*00009410*/ CMP         R0, LR 
    /*00009414*/ BCC         SVP_NNIE_FillForwardWithRoiTask_x9434
    /*00009418*/ LDR         R0, [R2] 
    /*0000941c*/ ADD         R0, R0, #4096               @ 0x1000 
    /*00009420*/ CMP         R0, R4 
    /*00009424*/ BCS         SVP_NNIE_FillForwardWithRoiTask_x9434
    /*00009428*/ LDR         R0, [R3] 
    /*0000942c*/ CMP         R0, #0 
    /*00009430*/ STREQ       R5, [R3] 
    SVP_NNIE_FillForwardWithRoiTask_x9434:
    /*00009434*/ ADD         R3, R3, #4 
    /*00009438*/ ADD         R2, R2, #4 
    /*0000943c*/ CMP         IP, R3 
    /*00009440*/ BNE         SVP_NNIE_FillForwardWithRoiTask_x940c
    /*00009444*/ CMP         R8, R5 
    /*00009448*/ ADD         R4, R4, R9 
    /*0000944c*/ BNE         SVP_NNIE_FillForwardWithRoiTask_x93f8
    /*00009450*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*00009454*/ LDR         R7, [SP, #24] 
    /*00009458*/ LSL         R3, R3, #12 
    /*0000945c*/ STR         R3, [SP, #200]              @ 0xc8 
    /*00009460*/ SUB         R0, R3, SL 
    /*00009464*/ MOV         LR, R3 
    SVP_NNIE_FillForwardWithRoiTask_x9468:
    /*00009468*/ LDR         R2, [SP, #100]              @ 0x64 
    /*0000946c*/ LDR         R3, [SP, #68]               @ 0x44 
    SVP_NNIE_FillForwardWithRoiTask_x9470:
    /*00009470*/ LDR         R1, [R6, #4]! 
    /*00009474*/ CMP         R1, R0 
    /*00009478*/ BCC         SVP_NNIE_FillForwardWithRoiTask_x949c
    /*0000947c*/ LDR         R1, [R2] 
    /*00009480*/ ADD         R1, R1, #4096               @ 0x1000 
    /*00009484*/ CMP         R1, LR 
    /*00009488*/ BCS         SVP_NNIE_FillForwardWithRoiTask_x949c
    /*0000948c*/ LDR         R1, [R3] 
    /*00009490*/ CMP         R1, #0 
    /*00009494*/ LDREQ       R1, [SP, #104]              @ 0x68 
    /*00009498*/ STREQ       R1, [R3] 
    SVP_NNIE_FillForwardWithRoiTask_x949c:
    /*0000949c*/ ADD         R3, R3, #4 
    /*000094a0*/ ADD         R2, R2, #4 
    /*000094a4*/ CMP         IP, R3 
    /*000094a8*/ BNE         SVP_NNIE_FillForwardWithRoiTask_x9470
    SVP_NNIE_FillForwardWithRoiTask_x94ac:
    /*000094ac*/ LDR         R3, [SP, #208]              @ 0xd0 
    /*000094b0*/ MOV         SL, #0 
    /*000094b4*/ LDR         FP, [SP, #124]              @ 0x7c 
    /*000094b8*/ STR         SL, [SP, #228]              @ 0xe4 
    /*000094bc*/ LSR         R3, R3, #12 
    /*000094c0*/ STR         R3, [SP, #172]              @ 0xac 
    /*000094c4*/ LDR         R3, [SP, #204]              @ 0xcc 
    /*000094c8*/ MOV         R8, FP 
    /*000094cc*/ STR         SL, [SP, #184]              @ 0xb8 
    /*000094d0*/ LSR         R5, R3, #12 
    /*000094d4*/ LDR         R3, [SP, #132]              @ 0x84 
    /*000094d8*/ MOV         R6, R5 
    /*000094dc*/ LSL         R9, R3, #12 
    /*000094e0*/ LDR         R3, [SP, #68]               @ 0x44 
    /*000094e4*/ SUB         R3, R3, #4 
    /*000094e8*/ STR         R3, [SP, #188]              @ 0xbc 
    /*000094ec*/ LDR         R3, [SP, #116]              @ 0x74 
    /*000094f0*/ STR         R3, [SP, #180]              @ 0xb4 
    SVP_NNIE_FillForwardWithRoiTask_x94f4:
    /*000094f4*/ LDR         R2, [SP, #188]              @ 0xbc 
    /*000094f8*/ LDR         R3, [R2, #4]! 
    /*000094fc*/ CMP         R3, #0 
    /*00009500*/ STR         R2, [SP, #188]              @ 0xbc 
    /*00009504*/ BNE         SVP_NNIE_FillForwardWithRoiTask_xa148
    /*00009508*/ LDR         R1, [SP, #120]              @ 0x78 
    /*0000950c*/ LDR         R2, [SP, #184]              @ 0xb8 
    /*00009510*/ LDR         R0, [SP, #100]              @ 0x64 
    /*00009514*/ LDR         R5, [R1, R2, LSL #2] 
    /*00009518*/ LDR         R1, [R8] 
    /*0000951c*/ ADD         R5, R5, #4096               @ 0x1000 
    /*00009520*/ LDR         R2, [R0, R2, LSL #2] 
    /*00009524*/ SUB         R5, R5, R1 
    /*00009528*/ LDR         R1, [SP, #180]              @ 0xb4 
    /*0000952c*/ CMP         R5, #4096                   @ 0x1000 
    /*00009530*/ ADD         R2, R2, #4096               @ 0x1000 
    /*00009534*/ LDR         FP, [R1] 
    /*00009538*/ MOVCC       R5, #4096                   @ 0x1000 
    /*0000953c*/ SUB         R2, R2, FP 
    /*00009540*/ CMP         R2, #4096                   @ 0x1000 
    /*00009544*/ MOVCC       R2, #4096                   @ 0x1000 
    /*00009548*/ STR         R2, [SP, #164]              @ 0xa4 
    /*0000954c*/ LDR         R2, [SP, #112]              @ 0x70 
    /*00009550*/ CMP         R2, #0 
    /*00009554*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa130
    /*00009558*/ MOV         R0, R3 
    /*0000955c*/ STR         R3, [SP, #152]              @ 0x98 
    SVP_NNIE_FillForwardWithRoiTask_x9560:
    /*00009560*/ LDR         R3, [SP, #164]              @ 0xa4 
    /*00009564*/ LSL         R4, R0, #12 
    /*00009568*/ ADD         R0, R0, #1 
    /*0000956c*/ STR         R0, [SP, #152]              @ 0x98 
    /*00009570*/ LDR         R1, [SP, #172]              @ 0xac 
    /*00009574*/ UMULL       R2, R3, R4, R3 
    /*00009578*/ LSR         R0, R2, #12 
    /*0000957c*/ ORR         R0, R0, R3, LSL #20 
    /*00009580*/ BL          0x0000000000009580          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00009584*/ LDR         R3, [SP, #164]              @ 0xa4 
    /*00009588*/ ADD         R2, R4, #4096               @ 0x1000 
    /*0000958c*/ LDR         R1, [SP, #172]              @ 0xac 
    /*00009590*/ UMULL       R2, R3, R2, R3 
    /*00009594*/ STRD        R2, [SP, #24] 
    /*00009598*/ LSR         R2, R2, #12 
    /*0000959c*/ LDR         R3, [SP, #28] 
    /*000095a0*/ AND         R4, R0, R7 
    /*000095a4*/ ORR         R0, R2, R3, LSL #20 
    /*000095a8*/ BL          0x00000000000095A8          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000095ac*/ ADD         R4, R4, FP 
    /*000095b0*/ BICS        R3, R0, R7 
    /*000095b4*/ LDR         R3, [SP, #200]              @ 0xc8 
    /*000095b8*/ AND         R0, R0, R7 
    /*000095bc*/ ADDNE       R0, R0, #4096               @ 0x1000 
    /*000095c0*/ CMP         R3, R4 
    /*000095c4*/ ADD         R0, R0, FP 
    /*000095c8*/ MOVCC       R4, R3 
    /*000095cc*/ CMP         R0, R3 
    /*000095d0*/ RSBLS       R0, R4, R0 
    /*000095d4*/ RSBHI       R0, R4, R3 
    /*000095d8*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*000095dc*/ CMP         R3, #0 
    /*000095e0*/ LSR         R3, R0, #12 
    /*000095e4*/ STR         R3, [SP, #64]               @ 0x40 
    /*000095e8*/ MOV         R2, R3 
    /*000095ec*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa094
    /*000095f0*/ LDR         R1, [SP, #176]              @ 0xb0 
    /*000095f4*/ LSR         R3, R4, #12 
    /*000095f8*/ CLZ         R2, R2 
    /*000095fc*/ MOV         FP, #0 
    /*00009600*/ ADD         R4, R1, SL, LSL #2 
    /*00009604*/ LDR         R1, [SP, #160]              @ 0xa0 
    /*00009608*/ STR         R8, [SP, #52]               @ 0x34 
    /*0000960c*/ MUL         R3, R1, R3 
    /*00009610*/ STR         R3, [SP, #56]               @ 0x38 
    /*00009614*/ LSR         R3, R2, #5 
    /*00009618*/ STR         R3, [SP, #60]               @ 0x3c 
    /*0000961c*/ B           SVP_NNIE_FillForwardWithRoiTask_x9648
    SVP_NNIE_FillForwardWithRoiTask_x9620:
    /*00009620*/ CMP         R2, #1024                   @ 0x400 
    /*00009624*/ BHI         SVP_NNIE_FillForwardWithRoiTask_xa360
    SVP_NNIE_FillForwardWithRoiTask_x9628:
    /*00009628*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*0000962c*/ ADD         R4, R4, #16 
    /*00009630*/ STR         R3, [R4, #-16] 
    /*00009634*/ LDR         R3, [SP, #64]               @ 0x40 
    /*00009638*/ CMP         R2, FP 
    /*0000963c*/ STR         R0, [R4, #-12] 
    /*00009640*/ STR         R3, [R4, #-8] 
    /*00009644*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa090
    SVP_NNIE_FillForwardWithRoiTask_x9648:
    /*00009648*/ LSL         R2, FP, #12 
    /*0000964c*/ MOV         R1, R6 
    /*00009650*/ ADD         FP, FP, #1 
    /*00009654*/ ADD         SL, SL, #4 
    /*00009658*/ MOV         R8, R2 
    /*0000965c*/ UMULL       R2, R3, R2, R5 
    /*00009660*/ LSR         R0, R2, #12 
    /*00009664*/ ORR         R0, R0, R3, LSL #20 
    /*00009668*/ BL          0x0000000000009668          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000966c*/ ADD         R2, R8, #4096               @ 0x1000 
    /*00009670*/ MOV         R1, R6 
    /*00009674*/ UMULL       R2, R3, R2, R5 
    /*00009678*/ LSR         IP, R2, #12 
    /*0000967c*/ AND         LR, R0, R7 
    /*00009680*/ ORR         R0, IP, R3, LSL #20 
    /*00009684*/ STR         LR, [SP, #24] 
    /*00009688*/ BL          0x0000000000009688          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000968c*/ LDR         LR, [SP, #24] 
    /*00009690*/ LDR         R8, [SP, #56]               @ 0x38 
    /*00009694*/ BICS        R3, R0, R7 
    /*00009698*/ LDR         R3, [SP, #52]               @ 0x34 
    /*0000969c*/ AND         R0, R0, R7 
    /*000096a0*/ ADDNE       R0, R0, #4096               @ 0x1000 
    /*000096a4*/ LDR         R3, [R3] 
    /*000096a8*/ ADD         R2, LR, R3 
    /*000096ac*/ ADD         R0, R3, R0 
    /*000096b0*/ CMP         R2, R9 
    /*000096b4*/ LDR         LR, [SP, #48]               @ 0x30 
    /*000096b8*/ MOVCS       R2, R9 
    /*000096bc*/ CMP         R0, R9 
    /*000096c0*/ RSBLS       R0, R2, R0 
    /*000096c4*/ RSBHI       R0, R2, R9 
    /*000096c8*/ UBFX        IP, R2, #12, #4 
    /*000096cc*/ LSR         R2, R2, #16 
    /*000096d0*/ LSR         R0, R0, #12 
    /*000096d4*/ MLA         R2, LR, R2, R8 
    /*000096d8*/ LSL         R3, IP, #1 
    /*000096dc*/ ADD         R1, R3, #1 
    /*000096e0*/ CMP         R2, #1024                   @ 0x400 
    /*000096e4*/ ADD         R3, R3, R2, LSL #5 
    /*000096e8*/ SUBHI       R2, R2, #1024               @ 0x400 
    /*000096ec*/ ADDHI       R3, R1, R2, LSL #5 
    /*000096f0*/ LDR         R1, [SP, #60]               @ 0x3c 
    /*000096f4*/ CMP         R0, #0 
    /*000096f8*/ ORREQ       R1, R1, #1 
    /*000096fc*/ CMP         R1, #0 
    /*00009700*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x9620
    /*00009704*/ MOV         R3, #0 
    /*00009708*/ B           SVP_NNIE_FillForwardWithRoiTask_x9628
    SVP_NNIE_FillForwardWithRoiTask_x970c:
    /*0000970c*/ LDR         IP, [IP] 
    /*00009710*/ CMP         IP, LR 
    /*00009714*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa89c
    SVP_NNIE_FillForwardWithRoiTask_x9718:
    /*00009718*/ LDRD        R2, [IP, #8] 
    /*0000971c*/ CMP         R1, R3 
    /*00009720*/ CMPEQ       R0, R2 
    /*00009724*/ BNE         SVP_NNIE_FillForwardWithRoiTask_x970c
    /*00009728*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*0000972c*/ CMP         IP, #0 
    /*00009730*/ STR         IP, [SP, #236]              @ 0xec 
    /*00009734*/ LDR         R2, [R3, #64]               @ 0x40 
    /*00009738*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x8e50
    /*0000973c*/ LDR         R3, [IP, #20] 
    /*00009740*/ CMP         R2, R3 
    /*00009744*/ BHI         SVP_NNIE_FillForwardWithRoiTask_x8e50
    /*00009748*/ LDR         R3, [IP, #16] 
    /*0000974c*/ STR         R3, [SP, #144]              @ 0x90 
    /*00009750*/ B           SVP_NNIE_FillForwardWithRoiTask_x8e68
    SVP_NNIE_FillForwardWithRoiTask_x9754:
    /*00009754*/ LDR         R3, [SP, #168]              @ 0xa8 
    /*00009758*/ LDR         IP, [SP, #224]              @ 0xe0 
    /*0000975c*/ RSBS        R2, R3, #16 
    /*00009760*/ ADD         R0, IP, R3 
    /*00009764*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb350
    /*00009768*/ MOV         R1, #0 
    /*0000976c*/ ADD         R3, IP, #16 
    /*00009770*/ STR         R3, [SP, #224]              @ 0xe0 
    /*00009774*/ BL          0x0000000000009774          @ memset [R_ARM_CALL: +0x0] 
    SVP_NNIE_FillForwardWithRoiTask_x9778:
    /*00009778*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*0000977c*/ LDR         R3, [R3, #4] 
    /*00009780*/ CMP         R3, #0 
    /*00009784*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x97f4
    /*00009788*/ LDR         R1, [SP, #224]              @ 0xe0 
    /*0000978c*/ MOV         R3, #0 
    /*00009790*/ LDR         R2, [SP, #312]              @ 0x138 
    /*00009794*/ MOV         IP, R3 
    /*00009798*/ SUB         R4, R1, #8 
    /*0000979c*/ LDR         R5, [SP, #316]              @ 0x13c 
    /*000097a0*/ B           SVP_NNIE_FillForwardWithRoiTask_x97a8
    SVP_NNIE_FillForwardWithRoiTask_x97a4:
    /*000097a4*/ MOV         IP, LR 
    SVP_NNIE_FillForwardWithRoiTask_x97a8:
    /*000097a8*/ LDRD        R0, [R2, #16] 
    /*000097ac*/ ADD         R3, R3, #1 
    /*000097b0*/ ADD         R2, R2, #48                 @ 0x30 
    /*000097b4*/ ADD         LR, IP, #8 
    /*000097b8*/ STRD        R0, [R4, #8]! 
    /*000097bc*/ LDR         R1, [R5, #4] 
    /*000097c0*/ CMP         R1, R3 
    /*000097c4*/ BHI         SVP_NNIE_FillForwardWithRoiTask_x97a4
    /*000097c8*/ ADD         R3, IP, #23 
    /*000097cc*/ LDR         IP, [SP, #224]              @ 0xe0 
    /*000097d0*/ BIC         R3, R3, #15 
    /*000097d4*/ SUBS        R2, R3, LR 
    /*000097d8*/ ADD         R0, IP, LR 
    /*000097dc*/ STREQ       R0, [SP, #224]              @ 0xe0 
    /*000097e0*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x97f4
    /*000097e4*/ MOV         R1, #0 
    /*000097e8*/ ADD         R3, IP, R3 
    /*000097ec*/ STR         R3, [SP, #224]              @ 0xe0 
    /*000097f0*/ BL          0x00000000000097F0          @ memset [R_ARM_CALL: +0x0] 
    SVP_NNIE_FillForwardWithRoiTask_x97f4:
    /*000097f4*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*000097f8*/ LDR         R1, [R3] 
    /*000097fc*/ CMP         R1, #0 
    /*00009800*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x98bc
    /*00009804*/ LDR         SL, [SP, #232]              @ 0xe8 
    /*00009808*/ MOV         R8, #2 
    /*0000980c*/ LDR         R9, [SP, #224]              @ 0xe0 
    /*00009810*/ MOVW        R3, #43691                  @ 0xaaab 
    /*00009814*/ MOV         FP, #0 
    /*00009818*/ MOVT        R3, #43690                  @ 0xaaaa 
    /*0000981c*/ STR         FP, [SP, #24] 
    /*00009820*/ STR         R3, [SP, #44]               @ 0x2c 
    SVP_NNIE_FillForwardWithRoiTask_x9824:
    /*00009824*/ LDR         R3, [SL] 
    /*00009828*/ CMP         R3, #5 
    /*0000982c*/ LDRLS       PC, [PC, R3, LSL #2] 
    /*00009830*/ B           SVP_NNIE_FillForwardWithRoiTask_xa7f4
    /*00009834*/ ANDEQ       R9, R0, IP, LSR #21 
    /*00009838*/ ANDEQ       R9, R0, R4, ASR SL 
    /*0000983c*/ @           <UNDEFINED> instruction: 0x000099b0 
    /*00009840*/ ANDEQ       R9, R0, R8, LSL #18 
    /*00009844*/ ANDEQ       R9, R0, R4, ASR #17 
    /*00009848*/ ANDEQ       R9, R0, R8, ASR R8 
    SVP_NNIE_FillForwardWithRoiTask_x984c:
    /*0000984c*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*00009850*/ LDRD        R0, [R3, #48]               @ 0x30 
    /*00009854*/ B           SVP_NNIE_FillForwardWithRoiTask_x8e4c
    /*00009858*/ LDR         FP, [SL, #24] 
    /*0000985c*/ LDR         IP, [SL, #4] 
    /*00009860*/ CMP         FP, #0 
    /*00009864*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x98a0
    /*00009868*/ MOV         R0, R9 
    /*0000986c*/ MOV         R1, #0 
    SVP_NNIE_FillForwardWithRoiTask_x9870:
    /*00009870*/ LDRD        R2, [SL, #16] 
    /*00009874*/ UMLAL       R2, R3, IP, R1 
    /*00009878*/ ADD         R1, R1, #1 
    /*0000987c*/ STRD        R2, [R0], #8 
    /*00009880*/ SUB         R2, R0, R9 
    /*00009884*/ LDR         R3, [SL, #24] 
    /*00009888*/ CMP         R3, R1 
    /*0000988c*/ BHI         SVP_NNIE_FillForwardWithRoiTask_x9870
    SVP_NNIE_FillForwardWithRoiTask_x9890:
    /*00009890*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*00009894*/ ADD         R2, R2, #15 
    /*00009898*/ BIC         FP, R2, #15 
    /*0000989c*/ LDR         R1, [R3] 
    SVP_NNIE_FillForwardWithRoiTask_x98a0:
    /*000098a0*/ LDR         R3, [SP, #24] 
    /*000098a4*/ ADD         R9, R9, FP 
    /*000098a8*/ ADD         SL, SL, #48                 @ 0x30 
    /*000098ac*/ ADD         R3, R3, #1 
    /*000098b0*/ STR         R3, [SP, #24] 
    /*000098b4*/ CMP         R3, R1 
    /*000098b8*/ BCC         SVP_NNIE_FillForwardWithRoiTask_x9824
    SVP_NNIE_FillForwardWithRoiTask_x98bc:
    /*000098bc*/ MOV         R4, #0 
    /*000098c0*/ B           SVP_NNIE_FillForwardWithRoiTask_x9e80
    /*000098c4*/ LDR         FP, [SL, #24] 
    /*000098c8*/ LDR         R3, [SL, #36]               @ 0x24 
    /*000098cc*/ LDR         IP, [SL, #4] 
    /*000098d0*/ CMP         FP, #0 
    /*000098d4*/ MUL         IP, IP, R3 
    /*000098d8*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x98a0
    /*000098dc*/ MOV         R0, R9 
    /*000098e0*/ MOV         R1, #0 
    SVP_NNIE_FillForwardWithRoiTask_x98e4:
    /*000098e4*/ LDRD        R2, [SL, #16] 
    /*000098e8*/ UMLAL       R2, R3, IP, R1 
    /*000098ec*/ ADD         R1, R1, #1 
    /*000098f0*/ STRD        R2, [R0], #8 
    /*000098f4*/ SUB         R2, R0, R9 
    /*000098f8*/ LDR         R3, [SL, #24] 
    /*000098fc*/ CMP         R3, R1 
    /*00009900*/ BHI         SVP_NNIE_FillForwardWithRoiTask_x98e4
    /*00009904*/ B           SVP_NNIE_FillForwardWithRoiTask_x9890
    /*00009908*/ LDR         FP, [SL, #24] 
    /*0000990c*/ LDR         R6, [SL, #36]               @ 0x24 
    /*00009910*/ LDR         R3, [SL, #4] 
    /*00009914*/ CMP         FP, #0 
    /*00009918*/ MUL         R6, R3, R6 
    /*0000991c*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x98a0
    /*00009920*/ ADDS        R0, R6, R6 
    /*00009924*/ MOV         R3, #0 
    /*00009928*/ ADC         R1, R3, R3 
    /*0000992c*/ MOV         IP, #0 
    /*00009930*/ ADD         LR, R9, #16 
    /*00009934*/ STR         R9, [SP, #32] 
    /*00009938*/ MOV         FP, IP 
    /*0000993c*/ MOV         R7, R0 
    /*00009940*/ MOV         R9, R1 
    SVP_NNIE_FillForwardWithRoiTask_x9944:
    /*00009944*/ MOV         R0, #1 
    /*00009948*/ MOV         R1, #0 
    /*0000994c*/ UMLAL       R0, R1, R8, IP 
    /*00009950*/ ADD         FP, FP, #16 
    /*00009954*/ UMULL       R2, R3, IP, R7 
    /*00009958*/ ADD         LR, LR, #16 
    /*0000995c*/ UMULL       R4, R5, R0, R6 
    /*00009960*/ MLA         R3, IP, R9, R3 
    /*00009964*/ ADD         IP, IP, #1 
    /*00009968*/ MLA         R5, R6, R1, R5 
    /*0000996c*/ LDRD        R0, [SL, #16] 
    /*00009970*/ ADDS        R0, R0, R2 
    /*00009974*/ ADC         R1, R1, R3 
    /*00009978*/ MOV         R2, R0 
    /*0000997c*/ MOV         R3, R1 
    /*00009980*/ STRD        R2, [LR, #-32]              @ 0xffffffe0 
    /*00009984*/ LDRD        R2, [SL, #16] 
    /*00009988*/ ADDS        R0, R2, R4 
    /*0000998c*/ ADC         R1, R3, R5 
    /*00009990*/ STRD        R0, [LR, #-24]              @ 0xffffffe8 
    /*00009994*/ LDR         R3, [SL, #24] 
    /*00009998*/ CMP         R3, IP 
    /*0000999c*/ BHI         SVP_NNIE_FillForwardWithRoiTask_x9944
    /*000099a0*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*000099a4*/ LDR         R9, [SP, #32] 
    /*000099a8*/ LDR         R1, [R3] 
    /*000099ac*/ B           SVP_NNIE_FillForwardWithRoiTask_x98a0
    /*000099b0*/ LDR         R0, [SL, #36]               @ 0x24 
    /*000099b4*/ LDR         R3, [SL, #4] 
    /*000099b8*/ LDR         R2, [SL, #40]               @ 0x28 
    /*000099bc*/ LDR         FP, [SL, #24] 
    /*000099c0*/ MUL         R3, R3, R0 
    /*000099c4*/ CMP         FP, #0 
    /*000099c8*/ MUL         R2, R2, R3 
    /*000099cc*/ LSR         R4, R2, #1 
    /*000099d0*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x98a0
    /*000099d4*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*000099d8*/ MOV         IP, #0 
    /*000099dc*/ ADD         LR, R9, #16 
    /*000099e0*/ MOV         FP, IP 
    /*000099e4*/ MOV         R7, #0 
    /*000099e8*/ STR         R4, [SP, #32] 
    /*000099ec*/ UMULL       R2, R3, R2, R3 
    /*000099f0*/ LSR         R6, R3, #2 
    /*000099f4*/ LSL         R6, R6, #1 
    SVP_NNIE_FillForwardWithRoiTask_x99f8:
    /*000099f8*/ LDR         R3, [SP, #32] 
    /*000099fc*/ ADD         FP, FP, #16 
    /*00009a00*/ ADD         LR, LR, #16 
    /*00009a04*/ UMULL       R0, R1, IP, R3 
    /*00009a08*/ LDRD        R2, [SL, #16] 
    /*00009a0c*/ ADD         IP, IP, #1 
    /*00009a10*/ ADDS        R4, R2, R0 
    /*00009a14*/ ADC         R5, R3, R1 
    /*00009a18*/ MOV         R2, R4 
    /*00009a1c*/ MOV         R3, R5 
    /*00009a20*/ STRD        R2, [LR, #-32]              @ 0xffffffe0 
    /*00009a24*/ LDRD        R2, [SL, #16] 
    /*00009a28*/ ADDS        R4, R2, R6 
    /*00009a2c*/ ADC         R5, R3, R7 
    /*00009a30*/ ADDS        R2, R4, R0 
    /*00009a34*/ ADC         R3, R5, R1 
    /*00009a38*/ STRD        R2, [LR, #-24]              @ 0xffffffe8 
    /*00009a3c*/ LDR         R3, [SL, #24] 
    /*00009a40*/ CMP         R3, IP 
    /*00009a44*/ BHI         SVP_NNIE_FillForwardWithRoiTask_x99f8
    /*00009a48*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*00009a4c*/ LDR         R1, [R3] 
    /*00009a50*/ B           SVP_NNIE_FillForwardWithRoiTask_x98a0
    /*00009a54*/ LDR         R0, [SL, #40]               @ 0x28 
    /*00009a58*/ LDR         R6, [SL, #36]               @ 0x24 
    /*00009a5c*/ LDR         R3, [SL, #4] 
    /*00009a60*/ CMP         R0, #3 
    /*00009a64*/ MUL         R6, R3, R6 
    /*00009a68*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xaba8
    /*00009a6c*/ CMP         R0, #1 
    /*00009a70*/ BNE         SVP_NNIE_FillForwardWithRoiTask_xb410
    /*00009a74*/ LDR         FP, [SL, #24] 
    /*00009a78*/ CMP         FP, #0 
    /*00009a7c*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x98a0
    /*00009a80*/ MOV         R0, R9 
    /*00009a84*/ MOV         R1, #0 
    SVP_NNIE_FillForwardWithRoiTask_x9a88:
    /*00009a88*/ LDRD        R2, [SL, #16] 
    /*00009a8c*/ UMLAL       R2, R3, R6, R1 
    /*00009a90*/ ADD         R1, R1, #1 
    /*00009a94*/ STRD        R2, [R0], #8 
    /*00009a98*/ SUB         R2, R0, R9 
    /*00009a9c*/ LDR         R3, [SL, #24] 
    /*00009aa0*/ CMP         R3, R1 
    /*00009aa4*/ BHI         SVP_NNIE_FillForwardWithRoiTask_x9a88
    /*00009aa8*/ B           SVP_NNIE_FillForwardWithRoiTask_x9890
    /*00009aac*/ LDR         IP, [SL, #36]               @ 0x24 
    /*00009ab0*/ LDR         R3, [SL, #4] 
    /*00009ab4*/ LDR         FP, [SL, #24] 
    /*00009ab8*/ MUL         R3, R3, IP 
    /*00009abc*/ LDR         IP, [SL, #40]               @ 0x28 
    /*00009ac0*/ CMP         FP, #0 
    /*00009ac4*/ MUL         IP, IP, R3 
    /*00009ac8*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x98a0
    /*00009acc*/ MOV         R0, R9 
    /*00009ad0*/ MOV         R1, #0 
    SVP_NNIE_FillForwardWithRoiTask_x9ad4:
    /*00009ad4*/ LDRD        R2, [SL, #16] 
    /*00009ad8*/ UMLAL       R2, R3, IP, R1 
    /*00009adc*/ ADD         R1, R1, #1 
    /*00009ae0*/ STRD        R2, [R0], #8 
    /*00009ae4*/ SUB         R2, R0, R9 
    /*00009ae8*/ LDR         R3, [SL, #24] 
    /*00009aec*/ CMP         R3, R1 
    /*00009af0*/ BHI         SVP_NNIE_FillForwardWithRoiTask_x9ad4
    /*00009af4*/ B           SVP_NNIE_FillForwardWithRoiTask_x9890
    SVP_NNIE_FillForwardWithRoiTask_x9af8:
    /*00009af8*/ CMN         R0, #1 
    /*00009afc*/ MOVW        R4, #13560                  @ 0x34f8 
    /*00009b00*/ LSRNE       R0, R0, #2 
    /*00009b04*/ MOVNE       R6, R5 
    /*00009b08*/ MOVEQ       R6, R5 
    /*00009b0c*/ MOV         R5, #104                    @ 0x68 
    /*00009b10*/ ADDNE       R0, R0, #2 
    /*00009b14*/ MLA         LR, R5, LR, R6 
    /*00009b18*/ MOVW        R5, #13564                  @ 0x34fc 
    /*00009b1c*/ MOVEQ       R0, #2 
    /*00009b20*/ CMP         R2, #0 
    /*00009b24*/ LDR         R4, [LR, R4] 
    /*00009b28*/ LDR         LR, [LR, R5] 
    /*00009b2c*/ MUL         LR, LR, R4 
    /*00009b30*/ MLA         R0, R2, LR, R0 
    /*00009b34*/ LSL         R0, R0, #4 
    /*00009b38*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa794
    /*00009b3c*/ SUB         LR, R2, #1 
    /*00009b40*/ LSR         LR, LR, #2 
    /*00009b44*/ ADD         LR, LR, #1 
    /*00009b48*/ LSL         LR, LR, #4 
    /*00009b4c*/ B           SVP_NNIE_FillForwardWithRoiTask_x8ffc
    SVP_NNIE_FillForwardWithRoiTask_x9b50:
    /*00009b50*/ LDR         R3, [SP, #92]               @ 0x5c 
    /*00009b54*/ MOV         LR, R0 
    /*00009b58*/ LDR         R3, [R3] 
    /*00009b5c*/ MOV         R4, R3 
    /*00009b60*/ STR         R3, [SP, #32] 
    /*00009b64*/ MOVW        R3, #13592                  @ 0x3518 
    /*00009b68*/ LSL         R2, R4, #3 
    /*00009b6c*/ LDR         R3, [IP, R3] 
    /*00009b70*/ MOV         R5, R3 
    /*00009b74*/ STR         R3, [SP, #184]              @ 0xb8 
    /*00009b78*/ LSL         R3, R4, #2 
    /*00009b7c*/ CMN         R5, #1 
    /*00009b80*/ STR         R3, [SP, #180]              @ 0xb4 
    /*00009b84*/ MOV         R1, R3 
    /*00009b88*/ ADD         R3, R3, R2 
    /*00009b8c*/ MOV         R0, R1 
    /*00009b90*/ ADD         R1, R1, R3 
    /*00009b94*/ ADD         R2, LR, R2 
    /*00009b98*/ ADD         R3, LR, R3 
    /*00009b9c*/ STR         R2, [SP, #156]              @ 0x9c 
    /*00009ba0*/ ADD         R2, LR, R1 
    /*00009ba4*/ STR         R3, [SP, #160]              @ 0xa0 
    /*00009ba8*/ ADD         R3, LR, R0 
    /*00009bac*/ STR         R2, [SP, #164]              @ 0xa4 
    /*00009bb0*/ STR         R3, [SP, #152]              @ 0x98 
    /*00009bb4*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa0b4
    /*00009bb8*/ MOVW        R8, #13584                  @ 0x3510 
    /*00009bbc*/ LSR         R5, R5, #2 
    /*00009bc0*/ LDR         R8, [IP, R8] 
    /*00009bc4*/ LSL         FP, R4, #4 
    /*00009bc8*/ LSL         SL, R5, #4 
    /*00009bcc*/ MOVW        R6, #13564                  @ 0x34fc 
    /*00009bd0*/ ADD         SL, SL, #32 
    /*00009bd4*/ MOVW        R4, #13548                  @ 0x34ec 
    /*00009bd8*/ STR         SL, [SP, #56]               @ 0x38 
    /*00009bdc*/ MOVW        LR, #13552                  @ 0x34f0 
    /*00009be0*/ STR         R8, [SP, #100]              @ 0x64 
    /*00009be4*/ MOVW        SL, #13580                  @ 0x350c 
    /*00009be8*/ MOVW        R8, #13560                  @ 0x34f8 
    /*00009bec*/ LDR         R4, [IP, R4] 
    /*00009bf0*/ LDR         SL, [IP, SL] 
    /*00009bf4*/ MOVW        R3, #13600                  @ 0x3520 
    /*00009bf8*/ LDR         R8, [IP, R8] 
    /*00009bfc*/ MOV         R2, FP 
    /*00009c00*/ LDR         R6, [IP, R6] 
    /*00009c04*/ LDR         LR, [IP, LR] 
    /*00009c08*/ LDRD        R0, [R9, #16] 
    /*00009c0c*/ STR         R4, [SP, #52]               @ 0x34 
    /*00009c10*/ STR         SL, [SP, #104]              @ 0x68 
    /*00009c14*/ LDR         R4, [IP, R3] 
    /*00009c18*/ STR         R8, [SP, #140]              @ 0x8c 
    /*00009c1c*/ STR         R6, [SP, #136]              @ 0x88 
    /*00009c20*/ STR         LR, [SP, #48]               @ 0x30 
    /*00009c24*/ BL          0x0000000000009C24          @ CMPI_Remap_Nocache [R_ARM_CALL: +0x0] 
    /*00009c28*/ SUBS        R3, R0, #0 
    /*00009c2c*/ STR         R3, [SP, #132]              @ 0x84 
    /*00009c30*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb304
    /*00009c34*/ ADD         R5, R5, #2 
    SVP_NNIE_FillForwardWithRoiTask_x9c38:
    /*00009c38*/ LDR         R2, [SP, #140]              @ 0x8c 
    /*00009c3c*/ LDR         R3, [SP, #136]              @ 0x88 
    /*00009c40*/ MUL         R3, R3, R2 
    /*00009c44*/ LDR         R2, [SP, #32] 
    /*00009c48*/ STR         R3, [SP, #176]              @ 0xb0 
    /*00009c4c*/ MUL         R3, R3, R2 
    /*00009c50*/ CMP         R3, #0 
    /*00009c54*/ ADD         R5, R3, R5 
    /*00009c58*/ STR         R3, [SP, #188]              @ 0xbc 
    /*00009c5c*/ SUBNE       R3, R3, #1 
    /*00009c60*/ LSL         R5, R5, #4 
    /*00009c64*/ MOVEQ       R3, #16 
    /*00009c68*/ LSRNE       R3, R3, #2 
    /*00009c6c*/ ADDNE       R3, R3, #1 
    /*00009c70*/ LSLNE       R3, R3, #4 
    /*00009c74*/ CMP         R2, #0 
    /*00009c78*/ ADD         R5, R5, R3 
    /*00009c7c*/ LDR         R3, [SP, #108]              @ 0x6c 
    /*00009c80*/ STR         R2, [R3, #4] 
    /*00009c84*/ STR         R5, [R3] 
    /*00009c88*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa934
    /*00009c8c*/ LDR         R3, [SP, #132]              @ 0x84 
    /*00009c90*/ ADD         R9, R3, #8 
    /*00009c94*/ LDR         R2, [R3] 
    /*00009c98*/ LDR         R3, [R3, #8] 
    /*00009c9c*/ CMP         R2, R3 
    /*00009ca0*/ BGT         SVP_NNIE_FillForwardWithRoiTask_xa918
    /*00009ca4*/ LDR         R3, [SP, #68]               @ 0x44 
    /*00009ca8*/ MOV         R8, #0 
    /*00009cac*/ LDR         FP, [SP, #132]              @ 0x84 
    /*00009cb0*/ SUB         R5, R3, #4 
    /*00009cb4*/ LDR         R3, [SP, #164]              @ 0xa4 
    /*00009cb8*/ SUB         R1, R3, #4 
    /*00009cbc*/ LDR         R3, [SP, #156]              @ 0x9c 
    /*00009cc0*/ SUB         LR, R3, #4 
    /*00009cc4*/ LDR         R3, [SP, #160]              @ 0xa0 
    /*00009cc8*/ STR         LR, [SP, #24] 
    /*00009ccc*/ SUB         IP, R3, #4 
    /*00009cd0*/ LDR         R3, [SP, #152]              @ 0x98 
    /*00009cd4*/ SUB         R0, R3, #4 
    /*00009cd8*/ MOV         R6, IP 
    /*00009cdc*/ MOV         IP, R1 
    /*00009ce0*/ MOV         LR, R0 
    /*00009ce4*/ B           SVP_NNIE_FillForwardWithRoiTask_x9db0
    SVP_NNIE_FillForwardWithRoiTask_x9ce8:
    /*00009ce8*/ CMP         R2, #0 
    /*00009cec*/ ADD         R8, R8, #1 
    /*00009cf0*/ ADDNE       R0, R2, #2048               @ 0x800 
    /*00009cf4*/ ANDNE       R0, R0, R7 
    /*00009cf8*/ UMULLNE     R0, R1, R0, R4 
    /*00009cfc*/ LSRNE       R2, R0, #12 
    /*00009d00*/ ORRNE       R2, R2, R1, LSL #20 
    /*00009d04*/ STR         R2, [LR, #4]! 
    /*00009d08*/ LDR         R2, [FP, #8] 
    /*00009d0c*/ CMP         R2, #0 
    /*00009d10*/ ADDNE       R2, R2, #2048               @ 0x800 
    /*00009d14*/ ANDNE       R2, R2, R7 
    /*00009d18*/ MOVEQ       R2, #4096                   @ 0x1000 
    /*00009d1c*/ ADDNE       R2, R2, #4096               @ 0x1000 
    /*00009d20*/ UMULL       R0, R1, R2, R4 
    /*00009d24*/ LSR         R2, R0, #12 
    /*00009d28*/ ORR         R2, R2, R1, LSL #20 
    /*00009d2c*/ STR         R2, [R6, #4]! 
    /*00009d30*/ LDR         R2, [FP, #4] 
    /*00009d34*/ CMP         R2, #0 
    /*00009d38*/ ADDNE       R2, R2, #2048               @ 0x800 
    /*00009d3c*/ ANDNE       R2, R2, R7 
    /*00009d40*/ MOVEQ       R1, R2 
    /*00009d44*/ UMULLNE     R2, R3, R2, R4 
    /*00009d48*/ LSRNE       R1, R2, #12 
    /*00009d4c*/ ORRNE       R1, R1, R3, LSL #20 
    /*00009d50*/ LDR         R3, [SP, #24] 
    /*00009d54*/ STR         R1, [R3, #4]! 
    /*00009d58*/ LDR         R2, [FP, #12] 
    /*00009d5c*/ STR         R3, [SP, #24] 
    /*00009d60*/ CMP         R2, #0 
    /*00009d64*/ LDR         R3, [SP, #32] 
    /*00009d68*/ ADDNE       R2, R2, #2048               @ 0x800 
    /*00009d6c*/ ANDNE       R2, R2, R7 
    /*00009d70*/ MOVEQ       R2, #4096                   @ 0x1000 
    /*00009d74*/ ADDNE       R2, R2, #4096               @ 0x1000 
    /*00009d78*/ CMP         R3, R8 
    /*00009d7c*/ MOV         R3, #0 
    /*00009d80*/ UMULL       R0, R1, R2, R4 
    /*00009d84*/ LSR         R2, R0, #12 
    /*00009d88*/ ORR         R2, R2, R1, LSL #20 
    /*00009d8c*/ STR         R2, [IP, #4]! 
    /*00009d90*/ STR         R3, [R5, #4]! 
    /*00009d94*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa3ec
    /*00009d98*/ LDR         R3, [FP, #24] 
    /*00009d9c*/ ADD         R9, R9, #16 
    /*00009da0*/ LDR         R2, [FP, #16]! 
    /*00009da4*/ LSL         R1, R8, #4 
    /*00009da8*/ CMP         R3, R2 
    /*00009dac*/ BLT         SVP_NNIE_FillForwardWithRoiTask_xa774
    SVP_NNIE_FillForwardWithRoiTask_x9db0:
    /*00009db0*/ LDR         R1, [FP, #4] 
    /*00009db4*/ ADD         R3, FP, #4 
    /*00009db8*/ LDR         SL, [FP, #12] 
    /*00009dbc*/ ADD         R0, FP, #12 
    /*00009dc0*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00009dc4*/ LSR         R3, R1, #31 
    /*00009dc8*/ CMP         SL, R1 
    /*00009dcc*/ ORRLT       R3, R3, #1 
    /*00009dd0*/ ORRS        R3, R3, R2, LSR #31 
    /*00009dd4*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x9ce8
    /*00009dd8*/ LDR         SL, [SP, #44]               @ 0x2c 
    /*00009ddc*/ MOV         R5, R8 
    /*00009de0*/ MOV         R6, R0 
    SVP_NNIE_FillForwardWithRoiTask_x9de4:
    /*00009de4*/ MOVW        R3, #1178                   @ 0x49a 
    /*00009de8*/ MOV         R1, #51                     @ 0x33 
    /*00009dec*/ STR         R3, [SP] 
    /*00009df0*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*00009df4*/ LDR         R3, [PC, #3728]             @ 0x000000000000ac8c 
    /*00009df8*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*00009dfc*/ MOV         R0, #3 
    /*00009e00*/ MOVW        R4, #32771                  @ 0x8003 
    /*00009e04*/ BL          0x0000000000009E04          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00009e08*/ LDR         R0, [R6] 
    /*00009e0c*/ MOV         R3, R5 
    /*00009e10*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3cbc] 
    /*00009e14*/ MOV         R1, #51                     @ 0x33 
    /*00009e18*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3cbc] 
    /*00009e1c*/ STR         R0, [SP, #12] 
    /*00009e20*/ MOV         R0, #3 
    /*00009e24*/ LDR         IP, [SL] 
    /*00009e28*/ MOVT        R4, #41011                  @ 0xa033 
    /*00009e2c*/ STR         IP, [SP, #8] 
    /*00009e30*/ LDR         IP, [R9] 
    /*00009e34*/ STR         IP, [SP, #4] 
    /*00009e38*/ LDR         IP, [FP] 
    /*00009e3c*/ STR         IP, [SP] 
    /*00009e40*/ BL          0x0000000000009E40          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00009e44*/ LDR         R0, [SP, #132]              @ 0x84 
    /*00009e48*/ BL          0x0000000000009E48          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    SVP_NNIE_FillForwardWithRoiTask_x9e4c:
    /*00009e4c*/ MOVW        R3, #1475                   @ 0x5c3 
    /*00009e50*/ MOV         R1, #51                     @ 0x33 
    /*00009e54*/ STR         R3, [SP] 
    /*00009e58*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*00009e5c*/ LDR         R3, [PC, #3628]             @ 0x000000000000ac90 
    /*00009e60*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*00009e64*/ MOV         R0, #3 
    /*00009e68*/ BL          0x0000000000009E68          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00009e6c*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3de4] 
    /*00009e70*/ MOV         R1, #51                     @ 0x33 
    /*00009e74*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3de4] 
    /*00009e78*/ MOV         R0, #3 
    /*00009e7c*/ BL          0x0000000000009E7C          @ HI_LOG [R_ARM_CALL: +0x0] 
    SVP_NNIE_FillForwardWithRoiTask_x9e80:
    /*00009e80*/ LDR         R3, [SP, #144]              @ 0x90 
    /*00009e84*/ CMP         R3, #0 
    /*00009e88*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x9eb8
    /*00009e8c*/ LDR         R2, [SP, #316]              @ 0x13c 
    /*00009e90*/ MOV         R0, R3 
    /*00009e94*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*00009e98*/ LDR         R2, [R2, #64]               @ 0x40 
    /*00009e9c*/ LDR         R1, [R3, #48]               @ 0x30 
    /*00009ea0*/ BL          0x0000000000009EA0          @ hil_mmb_flush_dcache_byaddr [R_ARM_CALL: +0x0] 
    /*00009ea4*/ CMP         R0, #0 
    /*00009ea8*/ BNE         SVP_NNIE_FillForwardWithRoiTask_xa8a8
    SVP_NNIE_FillForwardWithRoiTask_x9eac:
    /*00009eac*/ LDR         R3, [SP, #236]              @ 0xec 
    /*00009eb0*/ CMP         R3, #0 
    /*00009eb4*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xaf8c
    SVP_NNIE_FillForwardWithRoiTask_x9eb8:
    /*00009eb8*/ MOV         R0, R4 
    /*00009ebc*/ ADD         SP, SP, #276                @ 0x114 
    /*00009ec0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    SVP_NNIE_FillForwardWithRoiTask_x9ec4:
    /*00009ec4*/ LDR         R0, [SP, #108]              @ 0x6c 
    /*00009ec8*/ SUB         R2, R1, #1 
    /*00009ecc*/ LSR         R2, R2, #2 
    /*00009ed0*/ STR         R1, [R0, #4] 
    /*00009ed4*/ ADD         R2, R2, #1 
    /*00009ed8*/ LDR         R1, [SP, #72]               @ 0x48 
    /*00009edc*/ STR         R3, [R0, #12] 
    /*00009ee0*/ ADD         R3, R3, R2, LSL #4 
    /*00009ee4*/ STR         R3, [R0] 
    /*00009ee8*/ ADD         SL, R1, #8 
    /*00009eec*/ LDR         R2, [R1] 
    /*00009ef0*/ LDR         R3, [R1, #8] 
    /*00009ef4*/ CMP         R3, R2 
    /*00009ef8*/ BLT         SVP_NNIE_FillForwardWithRoiTask_xa8e0
    /*00009efc*/ LDR         R3, [SP, #100]              @ 0x64 
    /*00009f00*/ MOV         FP, R1 
    /*00009f04*/ MOV         R8, #0 
    /*00009f08*/ SUB         IP, R3, #4 
    /*00009f0c*/ LDR         R3, [SP, #116]              @ 0x74 
    /*00009f10*/ SUB         R6, R3, #4 
    /*00009f14*/ LDR         R3, [SP, #120]              @ 0x78 
    /*00009f18*/ SUB         R5, R3, #4 
    /*00009f1c*/ LDR         R3, [SP, #124]              @ 0x7c 
    /*00009f20*/ SUB         LR, R3, #4 
    /*00009f24*/ B           SVP_NNIE_FillForwardWithRoiTask_x9fc4
    SVP_NNIE_FillForwardWithRoiTask_x9f28:
    /*00009f28*/ UMULL       R0, R1, R4, R2 
    /*00009f2c*/ ASR         R3, R2, #31 
    /*00009f30*/ ADD         R8, R8, #1 
    /*00009f34*/ MLA         R1, R4, R3, R1 
    /*00009f38*/ LSR         R3, R0, #12 
    /*00009f3c*/ ORRS        R3, R3, R1, LSL #20 
    /*00009f40*/ ADDNE       R3, R3, #2048               @ 0x800 
    /*00009f44*/ ANDNE       R3, R3, R7 
    /*00009f48*/ STR         R3, [LR, #4]! 
    /*00009f4c*/ LDR         R2, [FP, #8] 
    /*00009f50*/ UMULL       R0, R1, R4, R2 
    /*00009f54*/ LSR         R2, R0, #12 
    /*00009f58*/ ORRS        R2, R2, R1, LSL #20 
    /*00009f5c*/ ADDNE       R2, R2, #2048               @ 0x800 
    /*00009f60*/ ANDNE       R2, R2, R7 
    /*00009f64*/ STR         R2, [R5, #4]! 
    /*00009f68*/ LDR         R2, [FP, #4] 
    /*00009f6c*/ UMULL       R0, R1, R4, R2 
    /*00009f70*/ LSR         R2, R0, #12 
    /*00009f74*/ ORRS        R2, R2, R1, LSL #20 
    /*00009f78*/ ADDNE       R2, R2, #2048               @ 0x800 
    /*00009f7c*/ ANDNE       R2, R2, R7 
    /*00009f80*/ STR         R2, [R6, #4]! 
    /*00009f84*/ LDR         R2, [FP, #12] 
    /*00009f88*/ UMULL       R0, R1, R4, R2 
    /*00009f8c*/ LSR         R2, R0, #12 
    /*00009f90*/ ORRS        R3, R2, R1, LSL #20 
    /*00009f94*/ ADDNE       R3, R3, #2048               @ 0x800 
    /*00009f98*/ ANDNE       R3, R3, R7 
    /*00009f9c*/ STR         R3, [IP, #4]! 
    /*00009fa0*/ LDR         R3, [SP, #32] 
    /*00009fa4*/ CMP         R3, R8 
    /*00009fa8*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa2d0
    /*00009fac*/ LDR         R3, [FP, #24] 
    /*00009fb0*/ ADD         SL, FP, #24 
    /*00009fb4*/ LDR         R2, [FP, #16]! 
    /*00009fb8*/ LSL         R1, R8, #4 
    /*00009fbc*/ CMP         R3, R2 
    /*00009fc0*/ BLT         SVP_NNIE_FillForwardWithRoiTask_xa758
    SVP_NNIE_FillForwardWithRoiTask_x9fc4:
    /*00009fc4*/ LDR         R1, [FP, #4] 
    /*00009fc8*/ ADD         SL, FP, #8 
    /*00009fcc*/ LDR         R9, [FP, #12] 
    /*00009fd0*/ ADD         R0, FP, #12 
    /*00009fd4*/ LSR         R3, R1, #31 
    /*00009fd8*/ CMP         R9, R1 
    /*00009fdc*/ ORRLT       R3, R3, #1 
    /*00009fe0*/ ADD         R9, FP, #4 
    /*00009fe4*/ ORRS        R3, R3, R2, LSR #31 
    /*00009fe8*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x9f28
    /*00009fec*/ MOV         R5, R0 
    SVP_NNIE_FillForwardWithRoiTask_x9ff0:
    /*00009ff0*/ MOVW        R3, #779                    @ 0x30b 
    /*00009ff4*/ MOV         R1, #51                     @ 0x33 
    /*00009ff8*/ STR         R3, [SP] 
    /*00009ffc*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*0000a000*/ LDR         R3, [PC, #3200]             @ 0x000000000000ac88 
    /*0000a004*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*0000a008*/ MOV         R0, #3 
    /*0000a00c*/ MOVW        R4, #32771                  @ 0x8003 
    /*0000a010*/ BL          0x000000000000A010          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000a014*/ LDR         R0, [R5] 
    /*0000a018*/ MOV         R3, R8 
    /*0000a01c*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3cbc] 
    /*0000a020*/ MOV         R1, #51                     @ 0x33 
    /*0000a024*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3cbc] 
    /*0000a028*/ STR         R0, [SP, #12] 
    /*0000a02c*/ MOV         R0, #3 
    /*0000a030*/ LDR         IP, [R9] 
    /*0000a034*/ MOVT        R4, #41011                  @ 0xa033 
    /*0000a038*/ STR         IP, [SP, #8] 
    /*0000a03c*/ LDR         IP, [SL] 
    /*0000a040*/ STR         IP, [SP, #4] 
    /*0000a044*/ LDR         IP, [FP] 
    /*0000a048*/ STR         IP, [SP] 
    /*0000a04c*/ BL          0x000000000000A04C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000a050*/ LDR         R0, [SP, #72]               @ 0x48 
    /*0000a054*/ BL          0x000000000000A054          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    SVP_NNIE_FillForwardWithRoiTask_xa058:
    /*0000a058*/ MOVW        R3, #1470                   @ 0x5be 
    /*0000a05c*/ MOV         R1, #51                     @ 0x33 
    /*0000a060*/ STR         R3, [SP] 
    /*0000a064*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*0000a068*/ LDR         R3, [PC, #3104]             @ 0x000000000000ac90 
    /*0000a06c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*0000a070*/ MOV         R0, #3 
    /*0000a074*/ BL          0x000000000000A074          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000a078*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3dc0] 
    /*0000a07c*/ MOV         R1, #51                     @ 0x33 
    /*0000a080*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3dc0] 
    /*0000a084*/ MOV         R0, #3 
    /*0000a088*/ BL          0x000000000000A088          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000a08c*/ B           SVP_NNIE_FillForwardWithRoiTask_x9e80
    SVP_NNIE_FillForwardWithRoiTask_xa090:
    /*0000a090*/ LDR         R8, [SP, #52]               @ 0x34 
    SVP_NNIE_FillForwardWithRoiTask_xa094:
    /*0000a094*/ LDR         R3, [SP, #112]              @ 0x70 
    /*0000a098*/ LDR         R2, [SP, #152]              @ 0x98 
    /*0000a09c*/ CMP         R3, R2 
    /*0000a0a0*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa130
    /*0000a0a4*/ LDR         R3, [SP, #180]              @ 0xb4 
    /*0000a0a8*/ MOV         R0, R2 
    /*0000a0ac*/ LDR         FP, [R3] 
    /*0000a0b0*/ B           SVP_NNIE_FillForwardWithRoiTask_x9560
    SVP_NNIE_FillForwardWithRoiTask_xa0b4:
    /*0000a0b4*/ MOVW        R8, #13580                  @ 0x350c 
    /*0000a0b8*/ LSL         FP, R4, #4 
    /*0000a0bc*/ LDR         R8, [IP, R8] 
    /*0000a0c0*/ MOVW        SL, #13584                  @ 0x3510 
    /*0000a0c4*/ MOVW        R6, #13560                  @ 0x34f8 
    /*0000a0c8*/ MOVW        R5, #13564                  @ 0x34fc 
    /*0000a0cc*/ MOVW        R4, #13548                  @ 0x34ec 
    /*0000a0d0*/ MOVW        LR, #13552                  @ 0x34f0 
    /*0000a0d4*/ LDR         R4, [IP, R4] 
    /*0000a0d8*/ MOVW        R3, #13600                  @ 0x3520 
    /*0000a0dc*/ STR         R8, [SP, #104]              @ 0x68 
    /*0000a0e0*/ MOV         R2, FP 
    /*0000a0e4*/ LDR         R8, [IP, SL] 
    /*0000a0e8*/ LDR         R6, [IP, R6] 
    /*0000a0ec*/ LDR         R5, [IP, R5] 
    /*0000a0f0*/ LDR         LR, [IP, LR] 
    /*0000a0f4*/ LDRD        R0, [R9, #16] 
    /*0000a0f8*/ STR         R4, [SP, #52]               @ 0x34 
    /*0000a0fc*/ STR         R8, [SP, #100]              @ 0x64 
    /*0000a100*/ LDR         R4, [IP, R3] 
    /*0000a104*/ STR         R6, [SP, #140]              @ 0x8c 
    /*0000a108*/ STR         R5, [SP, #136]              @ 0x88 
    /*0000a10c*/ STR         LR, [SP, #48]               @ 0x30 
    /*0000a110*/ BL          0x000000000000A110          @ CMPI_Remap_Nocache [R_ARM_CALL: +0x0] 
    /*0000a114*/ SUBS        R3, R0, #0 
    /*0000a118*/ STR         R3, [SP, #132]              @ 0x84 
    /*0000a11c*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb304
    /*0000a120*/ MOV         R3, #32 
    /*0000a124*/ MOV         R5, #2 
    /*0000a128*/ STR         R3, [SP, #56]               @ 0x38 
    /*0000a12c*/ B           SVP_NNIE_FillForwardWithRoiTask_x9c38
    SVP_NNIE_FillForwardWithRoiTask_xa130:
    /*0000a130*/ LDR         R3, [SP, #228]              @ 0xe4 
    /*0000a134*/ LDR         R2, [SP, #216]              @ 0xd8 
    /*0000a138*/ LDR         R1, [SP, #184]              @ 0xb8 
    /*0000a13c*/ STR         R1, [R2, R3, LSL #2] 
    /*0000a140*/ ADD         R3, R3, #1 
    /*0000a144*/ STR         R3, [SP, #228]              @ 0xe4 
    SVP_NNIE_FillForwardWithRoiTask_xa148:
    /*0000a148*/ LDR         R3, [SP, #184]              @ 0xb8 
    /*0000a14c*/ ADD         R8, R8, #4 
    /*0000a150*/ LDR         R2, [SP, #32] 
    /*0000a154*/ ADD         R3, R3, #1 
    /*0000a158*/ STR         R3, [SP, #184]              @ 0xb8 
    /*0000a15c*/ CMP         R2, R3 
    /*0000a160*/ LDR         R3, [SP, #180]              @ 0xb4 
    /*0000a164*/ ADD         R3, R3, #4 
    /*0000a168*/ STR         R3, [SP, #180]              @ 0xb4 
    /*0000a16c*/ BHI         SVP_NNIE_FillForwardWithRoiTask_x94f4
    /*0000a170*/ LDR         R3, [SP, #196]              @ 0xc4 
    /*0000a174*/ LDR         R2, [SP, #228]              @ 0xe4 
    /*0000a178*/ MUL         R3, R3, R2 
    SVP_NNIE_FillForwardWithRoiTask_xa17c:
    /*0000a17c*/ LDR         R2, [SP, #104]              @ 0x68 
    /*0000a180*/ LDR         R1, [SP, #108]              @ 0x6c 
    /*0000a184*/ CMN         R2, #1 
    /*0000a188*/ STR         R3, [R1, #16] 
    /*0000a18c*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb06c
    /*0000a190*/ ADD         R3, R3, R2, LSR #2 
    /*0000a194*/ CMP         R2, #1 
    /*0000a198*/ ADD         R3, R3, #2 
    /*0000a19c*/ LSL         R3, R3, #4 
    /*0000a1a0*/ STR         R3, [R1, #8] 
    /*0000a1a4*/ MOVLS       R3, #1 
    /*0000a1a8*/ STRLS       R3, [SP, #152]              @ 0x98 
    /*0000a1ac*/ BLS         SVP_NNIE_FillForwardWithRoiTask_xafc4
    /*0000a1b0*/ SUB         R3, R2, #1 
    /*0000a1b4*/ STR         R3, [SP, #240]              @ 0xf0 
    SVP_NNIE_FillForwardWithRoiTask_xa1b8:
    /*0000a1b8*/ LDR         R3, [SP, #212]              @ 0xd4 
    /*0000a1bc*/ MOV         R5, R7 
    /*0000a1c0*/ LSL         R3, R3, #12 
    /*0000a1c4*/ STR         R3, [SP, #244]              @ 0xf4 
    /*0000a1c8*/ LDR         R3, [SP, #208]              @ 0xd0 
    /*0000a1cc*/ LSR         R3, R3, #12 
    /*0000a1d0*/ STR         R3, [SP, #180]              @ 0xb4 
    /*0000a1d4*/ LDR         R3, [SP, #204]              @ 0xcc 
    /*0000a1d8*/ LSR         R6, R3, #12 
    /*0000a1dc*/ LDR         R3, [SP, #132]              @ 0x84 
    /*0000a1e0*/ MOV         R9, R6 
    /*0000a1e4*/ LSL         R8, R3, #12 
    /*0000a1e8*/ LDR         R3, [SP, #220]              @ 0xdc 
    /*0000a1ec*/ MOV         R7, R8 
    /*0000a1f0*/ MOV         R8, SL 
    /*0000a1f4*/ STR         R3, [SP, #212]              @ 0xd4 
    /*0000a1f8*/ MOV         R3, #0 
    /*0000a1fc*/ STR         R3, [SP, #184]              @ 0xb8 
    /*0000a200*/ MOV         R3, #1 
    /*0000a204*/ STR         R3, [SP, #172]              @ 0xac 
    SVP_NNIE_FillForwardWithRoiTask_xa208:
    /*0000a208*/ LDR         R3, [SP, #32] 
    /*0000a20c*/ CMP         R3, #0 
    /*0000a210*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa298
    /*0000a214*/ LDR         R6, [SP, #124]              @ 0x7c 
    /*0000a218*/ MOV         R3, #0 
    /*0000a21c*/ STR         R3, [SP, #188]              @ 0xbc 
    /*0000a220*/ MOV         SL, R8 
    /*0000a224*/ STR         R3, [SP, #60]               @ 0x3c 
    /*0000a228*/ MOV         R8, R9 
    /*0000a22c*/ LDR         R3, [SP, #116]              @ 0x74 
    /*0000a230*/ MOV         R9, R6 
    /*0000a234*/ LDR         R2, [SP, #68]               @ 0x44 
    /*0000a238*/ STR         R3, [SP, #64]               @ 0x40 
    /*0000a23c*/ SUB         R2, R2, #4 
    /*0000a240*/ STR         R2, [SP, #160]              @ 0xa0 
    SVP_NNIE_FillForwardWithRoiTask_xa244:
    /*0000a244*/ LDR         R2, [SP, #160]              @ 0xa0 
    /*0000a248*/ LDR         R3, [R2, #4]! 
    /*0000a24c*/ STR         R2, [SP, #160]              @ 0xa0 
    /*0000a250*/ LDR         R2, [SP, #172]              @ 0xac 
    /*0000a254*/ CMP         R3, R2 
    /*0000a258*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa940
    SVP_NNIE_FillForwardWithRoiTask_xa25c:
    /*0000a25c*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*0000a260*/ ADD         R9, R9, #4 
    /*0000a264*/ LDR         R2, [SP, #32] 
    /*0000a268*/ ADD         R3, R3, #1 
    /*0000a26c*/ STR         R3, [SP, #60]               @ 0x3c 
    /*0000a270*/ CMP         R2, R3 
    /*0000a274*/ LDR         R3, [SP, #64]               @ 0x40 
    /*0000a278*/ ADD         R3, R3, #4 
    /*0000a27c*/ STR         R3, [SP, #64]               @ 0x40 
    /*0000a280*/ BNE         SVP_NNIE_FillForwardWithRoiTask_xa244
    /*0000a284*/ LDR         R3, [SP, #196]              @ 0xc4 
    /*0000a288*/ MOV         R9, R8 
    /*0000a28c*/ LDR         R2, [SP, #188]              @ 0xbc 
    /*0000a290*/ MOV         R8, SL 
    /*0000a294*/ MUL         R3, R3, R2 
    SVP_NNIE_FillForwardWithRoiTask_xa298:
    /*0000a298*/ LDR         R2, [SP, #172]              @ 0xac 
    /*0000a29c*/ LDR         R1, [SP, #240]              @ 0xf0 
    /*0000a2a0*/ CMP         R1, R2 
    /*0000a2a4*/ LDR         R1, [SP, #212]              @ 0xd4 
    /*0000a2a8*/ STR         R3, [R1, #4]! 
    /*0000a2ac*/ ADD         R3, R2, #1 
    /*0000a2b0*/ STR         R1, [SP, #212]              @ 0xd4 
    /*0000a2b4*/ LDR         R2, [SP, #184]              @ 0xb8 
    /*0000a2b8*/ LDR         R1, [SP, #244]              @ 0xf4 
    /*0000a2bc*/ ADD         R2, R2, R1 
    /*0000a2c0*/ STR         R2, [SP, #184]              @ 0xb8 
    /*0000a2c4*/ BLS         SVP_NNIE_FillForwardWithRoiTask_xafb8
    /*0000a2c8*/ STR         R3, [SP, #172]              @ 0xac 
    /*0000a2cc*/ B           SVP_NNIE_FillForwardWithRoiTask_xa208
    SVP_NNIE_FillForwardWithRoiTask_xa2d0:
    /*0000a2d0*/ LDR         R3, [SP, #56]               @ 0x38 
    /*0000a2d4*/ MOV         R1, #0 
    /*0000a2d8*/ LDR         R2, [SP, #52]               @ 0x34 
    /*0000a2dc*/ ADD         R0, R3, R2 
    /*0000a2e0*/ LDR         R3, [SP, #24] 
    /*0000a2e4*/ LDR         R2, [SP, #64]               @ 0x40 
    /*0000a2e8*/ SUB         R0, R0, R3 
    /*0000a2ec*/ LDR         R3, [SP, #144]              @ 0x90 
    /*0000a2f0*/ ADD         R0, R3, R0 
    /*0000a2f4*/ LDR         R3, [SP, #108]              @ 0x6c 
    /*0000a2f8*/ ADD         R3, R3, #16 
    /*0000a2fc*/ STR         R3, [SP, #220]              @ 0xdc 
    /*0000a300*/ MOV         R4, R3 
    /*0000a304*/ BL          0x000000000000A304          @ memset [R_ARM_CALL: +0x0] 
    /*0000a308*/ LDR         R3, [SP, #132]              @ 0x84 
    /*0000a30c*/ LDR         R2, [SP, #156]              @ 0x9c 
    /*0000a310*/ CMP         R3, #255                    @ 0xff 
    /*0000a314*/ ADD         R3, R4, R2 
    /*0000a318*/ LDR         R2, [SP, #112]              @ 0x70 
    /*0000a31c*/ STR         R3, [SP, #176]              @ 0xb0 
    /*0000a320*/ LSL         R2, R2, #12 
    /*0000a324*/ STR         R2, [SP, #208]              @ 0xd0 
    /*0000a328*/ LDR         R2, [SP, #96]               @ 0x60 
    /*0000a32c*/ ADD         R3, R3, R2, LSL #4 
    /*0000a330*/ STR         R3, [SP, #216]              @ 0xd8 
    /*0000a334*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*0000a338*/ LSL         R3, R3, #12 
    /*0000a33c*/ STR         R3, [SP, #204]              @ 0xcc 
    /*0000a340*/ LDR         R3, [SP, #140]              @ 0x8c 
    /*0000a344*/ LSR         R3, R3, #1 
    /*0000a348*/ STR         R3, [SP, #212]              @ 0xd4 
    /*0000a34c*/ BLS         SVP_NNIE_FillForwardWithRoiTask_x939c
    /*0000a350*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*0000a354*/ LSL         R3, R3, #12 
    /*0000a358*/ STR         R3, [SP, #200]              @ 0xc8 
    /*0000a35c*/ B           SVP_NNIE_FillForwardWithRoiTask_x94ac
    SVP_NNIE_FillForwardWithRoiTask_xa360:
    /*0000a360*/ MOV         R8, R2 
    /*0000a364*/ MOVW        R3, #899                    @ 0x383 
    /*0000a368*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*0000a36c*/ STR         R3, [SP] 
    /*0000a370*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*0000a374*/ LDR         R3, [PC, #2316]             @ 0x000000000000ac88 
    /*0000a378*/ MOV         R1, #51                     @ 0x33 
    /*0000a37c*/ MOV         R0, #3 
    /*0000a380*/ MOV         R6, IP 
    /*0000a384*/ BL          0x000000000000A384          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000a388*/ LDR         R2, [SP, #184]              @ 0xb8 
    /*0000a38c*/ MOVW        R4, #32771                  @ 0x8003 
    /*0000a390*/ LDR         R5, [SP, #72]               @ 0x48 
    /*0000a394*/ STR         R6, [SP, #20] 
    /*0000a398*/ ADD         IP, R5, R2, LSL #4 
    /*0000a39c*/ MOV         R6, R2 
    /*0000a3a0*/ MOV         R3, R2 
    /*0000a3a4*/ STR         R8, [SP, #16] 
    SVP_NNIE_FillForwardWithRoiTask_xa3a8:
    /*0000a3a8*/ LDR         LR, [IP, #12] 
    /*0000a3ac*/ MOV         R1, #51                     @ 0x33 
    /*0000a3b0*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3d54] 
    /*0000a3b4*/ MOV         R0, #3 
    /*0000a3b8*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3d54] 
    /*0000a3bc*/ MOVT        R4, #41011                  @ 0xa033 
    /*0000a3c0*/ STR         LR, [SP, #12] 
    /*0000a3c4*/ LDR         LR, [IP, #8] 
    /*0000a3c8*/ STR         LR, [SP, #8] 
    /*0000a3cc*/ LDR         IP, [IP, #4] 
    /*0000a3d0*/ STR         IP, [SP, #4] 
    /*0000a3d4*/ LDR         IP, [R5, R6, LSL #4] 
    /*0000a3d8*/ STR         IP, [SP] 
    /*0000a3dc*/ BL          0x000000000000A3DC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000a3e0*/ MOV         R0, R5 
    /*0000a3e4*/ BL          0x000000000000A3E4          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    /*0000a3e8*/ B           SVP_NNIE_FillForwardWithRoiTask_xa058
    SVP_NNIE_FillForwardWithRoiTask_xa3ec:
    /*0000a3ec*/ LDR         R2, [SP, #108]              @ 0x6c 
    SVP_NNIE_FillForwardWithRoiTask_xa3f0:
    /*0000a3f0*/ LDR         R3, [SP, #140]              @ 0x8c 
    /*0000a3f4*/ LDR         R1, [SP, #56]               @ 0x38 
    /*0000a3f8*/ CMP         R3, #0 
    /*0000a3fc*/ ADD         R2, R2, R1 
    /*0000a400*/ LSL         R3, R3, #12 
    /*0000a404*/ STR         R2, [SP, #172]              @ 0xac 
    /*0000a408*/ STR         R3, [SP, #200]              @ 0xc8 
    /*0000a40c*/ LDR         R2, [SP, #52]               @ 0x34 
    /*0000a410*/ LDR         R3, [SP, #136]              @ 0x88 
    /*0000a414*/ LSL         R2, R2, #12 
    /*0000a418*/ STR         R2, [SP, #96]               @ 0x60 
    /*0000a41c*/ LSL         R3, R3, #12 
    /*0000a420*/ LDR         R2, [SP, #48]               @ 0x30 
    /*0000a424*/ STR         R3, [SP, #204]              @ 0xcc 
    /*0000a428*/ LDREQ       R3, [SP, #140]              @ 0x8c 
    /*0000a42c*/ LSL         R9, R2, #12 
    /*0000a430*/ STREQ       R3, [SP, #56]               @ 0x38 
    /*0000a434*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xac9c
    /*0000a438*/ LSR         R3, R3, #12 
    /*0000a43c*/ STR         R3, [SP, #60]               @ 0x3c 
    /*0000a440*/ LDR         R3, [SP, #200]              @ 0xc8 
    /*0000a444*/ MOV         R6, R9 
    /*0000a448*/ STR         R8, [SP, #208]              @ 0xd0 
    /*0000a44c*/ LSR         R3, R3, #12 
    /*0000a450*/ STR         R3, [SP, #64]               @ 0x40 
    /*0000a454*/ MOV         R3, #1 
    /*0000a458*/ STR         R3, [SP, #196]              @ 0xc4 
    /*0000a45c*/ MOV         R3, #0 
    /*0000a460*/ STR         R3, [SP, #56]               @ 0x38 
    SVP_NNIE_FillForwardWithRoiTask_xa464:
    /*0000a464*/ LDR         R3, [SP, #136]              @ 0x88 
    /*0000a468*/ CMP         R3, #0 
    /*0000a46c*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa8fc
    /*0000a470*/ LDR         R3, [SP, #196]              @ 0xc4 
    /*0000a474*/ MOV         R8, R6 
    /*0000a478*/ MOV         R2, #1 
    /*0000a47c*/ STR         R2, [SP, #192]              @ 0xc0 
    /*0000a480*/ SUB         R3, R3, #-67108863          @ 0xfc000001 
    /*0000a484*/ SUB         R3, R3, #66060288           @ 0x3f00000 
    /*0000a488*/ LSL         R3, R3, #12 
    /*0000a48c*/ STR         R3, [SP, #124]              @ 0x7c 
    /*0000a490*/ ADD         R3, R3, #4096               @ 0x1000 
    /*0000a494*/ STR         R3, [SP, #120]              @ 0x78 
    SVP_NNIE_FillForwardWithRoiTask_xa498:
    /*0000a498*/ LDR         R3, [SP, #32] 
    /*0000a49c*/ CMP         R3, #0 
    /*0000a4a0*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa6b4
    /*0000a4a4*/ LDR         R2, [SP, #152]              @ 0x98 
    /*0000a4a8*/ MOV         SL, R7 
    /*0000a4ac*/ LDR         R3, [SP, #192]              @ 0xc0 
    /*0000a4b0*/ SUB         R2, R2, #4 
    /*0000a4b4*/ STR         R2, [SP, #52]               @ 0x34 
    /*0000a4b8*/ LDR         R2, [SP, #160]              @ 0xa0 
    /*0000a4bc*/ SUB         R3, R3, #-67108863          @ 0xfc000001 
    /*0000a4c0*/ SUB         R3, R3, #66060288           @ 0x3f00000 
    /*0000a4c4*/ LDR         R1, [SP, #56]               @ 0x38 
    /*0000a4c8*/ SUB         R2, R2, #4 
    /*0000a4cc*/ STR         R2, [SP, #48]               @ 0x30 
    /*0000a4d0*/ LDR         R2, [SP, #156]              @ 0x9c 
    /*0000a4d4*/ LSL         R3, R3, #12 
    /*0000a4d8*/ STR         R3, [SP, #116]              @ 0x74 
    /*0000a4dc*/ ADD         R3, R3, #4096               @ 0x1000 
    /*0000a4e0*/ SUB         R2, R2, #4 
    /*0000a4e4*/ STR         R2, [SP, #44]               @ 0x2c 
    /*0000a4e8*/ LDR         R2, [SP, #164]              @ 0xa4 
    /*0000a4ec*/ STR         R3, [SP, #112]              @ 0x70 
    /*0000a4f0*/ MOV         R3, #0 
    /*0000a4f4*/ SUB         R2, R2, #4 
    /*0000a4f8*/ STR         R2, [SP, #24] 
    /*0000a4fc*/ LDR         R2, [SP, #172]              @ 0xac 
    /*0000a500*/ STR         R3, [SP, #68]               @ 0x44 
    /*0000a504*/ ADD         R9, R2, R1, LSL #2 
    /*0000a508*/ B           SVP_NNIE_FillForwardWithRoiTask_xa558
    SVP_NNIE_FillForwardWithRoiTask_xa50c:
    /*0000a50c*/ SUB         R7, R7, #1024               @ 0x400 
    /*0000a510*/ LSL         FP, R4, #1 
    /*0000a514*/ CMP         R7, #1024                   @ 0x400 
    /*0000a518*/ ADD         FP, FP, #1 
    /*0000a51c*/ ADD         FP, FP, R7, LSL #5 
    /*0000a520*/ BHI         SVP_NNIE_FillForwardWithRoiTask_xa6d0
    SVP_NNIE_FillForwardWithRoiTask_xa524:
    /*0000a524*/ LDR         R2, [SP, #68]               @ 0x44 
    /*0000a528*/ ADD         R9, R9, #16 
    /*0000a52c*/ LDR         R1, [SP, #32] 
    /*0000a530*/ ADD         R2, R2, #1 
    /*0000a534*/ STR         R3, [R9, #-8] 
    /*0000a538*/ CMP         R1, R2 
    /*0000a53c*/ LDR         R3, [SP, #56]               @ 0x38 
    /*0000a540*/ STR         FP, [R9, #-16] 
    /*0000a544*/ STR         R6, [R9, #-12] 
    /*0000a548*/ ADD         R3, R3, #4 
    /*0000a54c*/ STR         R2, [SP, #68]               @ 0x44 
    /*0000a550*/ STR         R3, [SP, #56]               @ 0x38 
    /*0000a554*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xa6b0
    SVP_NNIE_FillForwardWithRoiTask_xa558:
    /*0000a558*/ LDR         R3, [SP, #52]               @ 0x34 
    /*0000a55c*/ LDR         R1, [SP, #60]               @ 0x3c 
    /*0000a560*/ LDR         R4, [R3, #4]! 
    /*0000a564*/ STR         R3, [SP, #52]               @ 0x34 
    /*0000a568*/ LDR         R3, [SP, #48]               @ 0x30 
    /*0000a56c*/ LDR         R6, [R3, #4]! 
    /*0000a570*/ SUB         R6, R6, R4 
    /*0000a574*/ STR         R3, [SP, #48]               @ 0x30 
    /*0000a578*/ CMP         R6, #1 
    /*0000a57c*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*0000a580*/ MOVCC       R6, #1 
    /*0000a584*/ LDR         R5, [R3, #4]! 
    /*0000a588*/ STR         R3, [SP, #44]               @ 0x2c 
    /*0000a58c*/ LDR         R3, [SP, #24] 
    /*0000a590*/ LDR         R2, [R3, #4]! 
    /*0000a594*/ SUB         FP, R2, R5 
    /*0000a598*/ STR         R3, [SP, #24] 
    /*0000a59c*/ CMP         FP, #1 
    /*0000a5a0*/ LDR         R3, [SP, #116]              @ 0x74 
    /*0000a5a4*/ MOVCC       FP, #1 
    /*0000a5a8*/ UMULL       R2, R3, R6, R3 
    /*0000a5ac*/ LSR         R0, R2, #12 
    /*0000a5b0*/ ORR         R0, R0, R3, LSL #20 
    /*0000a5b4*/ BL          0x000000000000A5B4          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000a5b8*/ LDR         R3, [SP, #112]              @ 0x70 
    /*0000a5bc*/ LDR         R1, [SP, #60]               @ 0x3c 
    /*0000a5c0*/ UMULL       R6, R7, R6, R3 
    /*0000a5c4*/ LSR         R2, R6, #12 
    /*0000a5c8*/ ADD         R3, R0, R4 
    /*0000a5cc*/ ORR         R0, R2, R7, LSL #20 
    /*0000a5d0*/ AND         R7, R3, SL 
    /*0000a5d4*/ BL          0x000000000000A5D4          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000a5d8*/ LDR         R3, [SP, #124]              @ 0x7c 
    /*0000a5dc*/ LDR         R1, [SP, #64]               @ 0x40 
    /*0000a5e0*/ UMULL       R2, R3, FP, R3 
    /*0000a5e4*/ ADD         R0, R0, R4 
    /*0000a5e8*/ AND         R6, R0, SL 
    /*0000a5ec*/ CMP         R0, R6 
    /*0000a5f0*/ LDR         R0, [SP, #104]              @ 0x68 
    /*0000a5f4*/ ADDNE       R6, R6, #4096               @ 0x1000 
    /*0000a5f8*/ CMP         R8, R7 
    /*0000a5fc*/ MOVCC       R4, R8 
    /*0000a600*/ MOVCS       R4, R7 
    /*0000a604*/ CMP         R8, R6 
    /*0000a608*/ RSBLS       R6, R4, R8 
    /*0000a60c*/ RSBHI       R6, R4, R6 
    /*0000a610*/ LSRS        R6, R6, #12 
    /*0000a614*/ SUBEQ       R4, R4, #4096               @ 0x1000 
    /*0000a618*/ MOVEQ       R6, #1 
    /*0000a61c*/ LSR         R7, R4, #16 
    /*0000a620*/ UBFX        R4, R4, #12, #4 
    /*0000a624*/ MUL         R7, R0, R7 
    /*0000a628*/ LSR         R0, R2, #12 
    /*0000a62c*/ ORR         R0, R0, R3, LSL #20 
    /*0000a630*/ BL          0x000000000000A630          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000a634*/ LDR         R3, [SP, #120]              @ 0x78 
    /*0000a638*/ LDR         R1, [SP, #64]               @ 0x40 
    /*0000a63c*/ UMULL       R2, R3, FP, R3 
    /*0000a640*/ STRD        R2, [SP, #72]               @ 0x48 
    /*0000a644*/ LSR         R2, R2, #12 
    /*0000a648*/ LDR         R3, [SP, #76]               @ 0x4c 
    /*0000a64c*/ ADD         FP, R0, R5 
    /*0000a650*/ ORR         R0, R2, R3, LSL #20 
    /*0000a654*/ BL          0x000000000000A654          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000a658*/ LDR         R2, [SP, #96]               @ 0x60 
    /*0000a65c*/ AND         FP, FP, SL 
    /*0000a660*/ ADD         R0, R0, R5 
    /*0000a664*/ AND         R3, R0, SL 
    /*0000a668*/ CMP         R0, R3 
    /*0000a66c*/ ADDNE       R3, R3, #4096               @ 0x1000 
    /*0000a670*/ CMP         R2, FP 
    /*0000a674*/ MOVCC       FP, R2 
    /*0000a678*/ CMP         R2, R3 
    /*0000a67c*/ RSBLS       R3, FP, R2 
    /*0000a680*/ RSBHI       R3, FP, R3 
    /*0000a684*/ LDR         R2, [SP, #100]              @ 0x64 
    /*0000a688*/ LSRS        R3, R3, #12 
    /*0000a68c*/ SUBEQ       FP, FP, #4096               @ 0x1000 
    /*0000a690*/ MOVEQ       R3, #1 
    /*0000a694*/ LSR         FP, FP, #12 
    /*0000a698*/ MLA         R7, R2, FP, R7 
    /*0000a69c*/ CMP         R7, #1024                   @ 0x400 
    /*0000a6a0*/ BHI         SVP_NNIE_FillForwardWithRoiTask_xa50c
    /*0000a6a4*/ LSL         R4, R4, #1 
    /*0000a6a8*/ ADD         FP, R4, R7, LSL #5 
    /*0000a6ac*/ B           SVP_NNIE_FillForwardWithRoiTask_xa524
    SVP_NNIE_FillForwardWithRoiTask_xa6b0:
    /*0000a6b0*/ MOV         R7, SL 
    SVP_NNIE_FillForwardWithRoiTask_xa6b4:
    /*0000a6b4*/ LDR         R3, [SP, #192]              @ 0xc0 
    /*0000a6b8*/ LDR         R2, [SP, #136]              @ 0x88 
    /*0000a6bc*/ CMP         R2, R3 
    /*0000a6c0*/ ADD         R3, R3, #1 
    /*0000a6c4*/ BLS         SVP_NNIE_FillForwardWithRoiTask_xa8f8
    /*0000a6c8*/ STR         R3, [SP, #192]              @ 0xc0 
    /*0000a6cc*/ B           SVP_NNIE_FillForwardWithRoiTask_xa498
    SVP_NNIE_FillForwardWithRoiTask_xa6d0:
    /*0000a6d0*/ MOVW        R3, #1254                   @ 0x4e6 
    /*0000a6d4*/ MOV         R1, #51                     @ 0x33 
    /*0000a6d8*/ STR         R3, [SP] 
    /*0000a6dc*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*0000a6e0*/ LDR         R3, [PC, #1444]             @ 0x000000000000ac8c 
    /*0000a6e4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*0000a6e8*/ MOV         R0, #3 
    /*0000a6ec*/ MOV         R6, R7 
    /*0000a6f0*/ BL          0x000000000000A6F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000a6f4*/ LDR         R7, [SP, #68]               @ 0x44 
    /*0000a6f8*/ LDR         R5, [SP, #132]              @ 0x84 
    /*0000a6fc*/ MOV         R8, R4 
    /*0000a700*/ STR         R6, [SP, #16] 
    /*0000a704*/ MOV         R1, #51                     @ 0x33 
    /*0000a708*/ ADD         IP, R5, R7, LSL #4 
    /*0000a70c*/ STR         R8, [SP, #20] 
    /*0000a710*/ MOV         R3, R7 
    /*0000a714*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3d54] 
    /*0000a718*/ LDR         LR, [IP, #12] 
    /*0000a71c*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3d54] 
    /*0000a720*/ MOV         R0, #3 
    /*0000a724*/ MOVW        R4, #32771                  @ 0x8003 
    /*0000a728*/ MOVT        R4, #41011                  @ 0xa033 
    /*0000a72c*/ STR         LR, [SP, #12] 
    /*0000a730*/ LDR         LR, [IP, #8] 
    /*0000a734*/ STR         LR, [SP, #8] 
    /*0000a738*/ LDR         IP, [IP, #4] 
    /*0000a73c*/ STR         IP, [SP, #4] 
    /*0000a740*/ LDR         IP, [R5, R7, LSL #4] 
    /*0000a744*/ STR         IP, [SP] 
    /*0000a748*/ BL          0x000000000000A748          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000a74c*/ MOV         R0, R5 
    /*0000a750*/ BL          0x000000000000A750          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    /*0000a754*/ B           SVP_NNIE_FillForwardWithRoiTask_x9e4c
    SVP_NNIE_FillForwardWithRoiTask_xa758:
    /*0000a758*/ LDR         R2, [SP, #72]               @ 0x48 
    /*0000a75c*/ ADD         R3, R1, #4 
    /*0000a760*/ ADD         R1, R1, #12 
    /*0000a764*/ ADD         R9, R2, R3 
    /*0000a768*/ ADD         R3, R2, R1 
    /*0000a76c*/ MOV         R5, R3 
    /*0000a770*/ B           SVP_NNIE_FillForwardWithRoiTask_x9ff0
    SVP_NNIE_FillForwardWithRoiTask_xa774:
    /*0000a774*/ LDR         R3, [SP, #132]              @ 0x84 
    /*0000a778*/ MOV         R5, R8 
    /*0000a77c*/ ADD         SL, R1, #4 
    /*0000a780*/ ADD         R8, R1, #12 
    /*0000a784*/ ADD         SL, R3, SL 
    /*0000a788*/ ADD         R3, R3, R8 
    /*0000a78c*/ MOV         R6, R3 
    /*0000a790*/ B           SVP_NNIE_FillForwardWithRoiTask_x9de4
    SVP_NNIE_FillForwardWithRoiTask_xa794:
    /*0000a794*/ MOV         LR, #16 
    /*0000a798*/ B           SVP_NNIE_FillForwardWithRoiTask_x8ffc
    SVP_NNIE_FillForwardWithRoiTask_xa79c:
    /*0000a79c*/ MOV         LR, #104                    @ 0x68 
    /*0000a7a0*/ MOVW        R4, #13560                  @ 0x34f8 
    /*0000a7a4*/ MLA         R0, LR, R0, R5 
    /*0000a7a8*/ MOVW        R5, #13564                  @ 0x34fc 
    /*0000a7ac*/ LDR         LR, [SP, #252]              @ 0xfc 
    /*0000a7b0*/ CMN         R2, #1 
    /*0000a7b4*/ LSRNE       R2, R2, #2 
    /*0000a7b8*/ ADDNE       R2, R2, #2 
    /*0000a7bc*/ MOVEQ       R2, #2 
    /*0000a7c0*/ LDR         R4, [R0, R4] 
    /*0000a7c4*/ LDR         R0, [R0, R5] 
    /*0000a7c8*/ MUL         R0, R0, R4 
    /*0000a7cc*/ MUL         R0, LR, R0 
    /*0000a7d0*/ CMP         R0, #0 
    /*0000a7d4*/ ADD         R2, R0, R2 
    /*0000a7d8*/ SUBNE       R0, R0, #1 
    /*0000a7dc*/ LSL         R2, R2, #4 
    /*0000a7e0*/ MOVEQ       R0, #16 
    /*0000a7e4*/ LSRNE       R0, R0, #2 
    /*0000a7e8*/ ADDNE       R0, R0, #1 
    /*0000a7ec*/ LSLNE       R0, R0, #4 
    /*0000a7f0*/ B           SVP_NNIE_FillForwardWithRoiTask_x90a4
    SVP_NNIE_FillForwardWithRoiTask_xa7f4:
    /*0000a7f4*/ MOVW        R3, #1599                   @ 0x63f 
    /*0000a7f8*/ MOV         R1, #51                     @ 0x33 
    /*0000a7fc*/ STR         R3, [SP] 
    /*0000a800*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*0000a804*/ LDR         R3, [PC, #1156]             @ 0x000000000000ac90 
    /*0000a808*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*0000a80c*/ MOV         R0, #3 
    /*0000a810*/ LDR         FP, [SP, #24] 
    /*0000a814*/ BL          0x000000000000A814          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000a818*/ LDR         R2, [SL] 
    /*0000a81c*/ MOV         R0, #6 
    /*0000a820*/ MOV         R1, #0 
    /*0000a824*/ STR         R0, [SP, #8] 
    /*0000a828*/ MOV         R3, FP 
    /*0000a82c*/ STR         R2, [SP] 
    /*0000a830*/ MOV         R0, #3 
    /*0000a834*/ STR         R1, [SP, #4] 
    /*0000a838*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3bf0] 
    /*0000a83c*/ MOV         R1, #51                     @ 0x33 
    /*0000a840*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3bf0] 
    /*0000a844*/ MOVW        R4, #32771                  @ 0x8003 
    /*0000a848*/ BL          0x000000000000A848          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000a84c*/ MOVT        R4, #41011                  @ 0xa033 
    /*0000a850*/ B           SVP_NNIE_FillForwardWithRoiTask_x9e80
    SVP_NNIE_FillForwardWithRoiTask_xa854:
    /*0000a854*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*0000a858*/ CMP         R2, #0 
    /*0000a85c*/ LDR         R2, [SP, #140]              @ 0x8c 
    /*0000a860*/ LSL         R3, R3, #12 
    /*0000a864*/ STR         R3, [SP, #200]              @ 0xc8 
    /*0000a868*/ SUB         R0, R3, R2, LSL #12 
    /*0000a86c*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xafa4
    /*0000a870*/ LDR         R3, [SP, #100]              @ 0x64 
    /*0000a874*/ LDR         LR, [SP, #200]              @ 0xc8 
    /*0000a878*/ ADD         IP, R3, IP 
    /*0000a87c*/ LDR         R3, [SP, #116]              @ 0x74 
    /*0000a880*/ SUB         R6, R3, #4 
    /*0000a884*/ B           SVP_NNIE_FillForwardWithRoiTask_x9468
    SVP_NNIE_FillForwardWithRoiTask_xa888:
    /*0000a888*/ CMP         R1, #1 
    /*0000a88c*/ STR         R2, [SP, #248]              @ 0xf8 
    /*0000a890*/ MOVNE       R0, R2 
    /*0000a894*/ BNE         SVP_NNIE_FillForwardWithRoiTask_x8f70
    /*0000a898*/ B           SVP_NNIE_FillForwardWithRoiTask_x8f74
    SVP_NNIE_FillForwardWithRoiTask_xa89c:
    /*0000a89c*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*0000a8a0*/ LDR         R2, [R3, #64]               @ 0x40 
    /*0000a8a4*/ B           SVP_NNIE_FillForwardWithRoiTask_x8e50
    SVP_NNIE_FillForwardWithRoiTask_xa8a8:
    /*0000a8a8*/ MOVW        R3, #1612                   @ 0x64c 
    /*0000a8ac*/ MOV         R1, #51                     @ 0x33 
    /*0000a8b0*/ STR         R3, [SP] 
    /*0000a8b4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*0000a8b8*/ LDR         R3, [PC, #976]              @ 0x000000000000ac90 
    /*0000a8bc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*0000a8c0*/ MOV         R0, #3 
    /*0000a8c4*/ BL          0x000000000000A8C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000a8c8*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3e0c] 
    /*0000a8cc*/ MOV         R1, #51                     @ 0x33 
    /*0000a8d0*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3e0c] 
    /*0000a8d4*/ MOV         R0, #3 
    /*0000a8d8*/ BL          0x000000000000A8D8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000a8dc*/ B           SVP_NNIE_FillForwardWithRoiTask_x9eac
    SVP_NNIE_FillForwardWithRoiTask_xa8e0:
    /*0000a8e0*/ ADD         R2, R1, #12 
    /*0000a8e4*/ ADD         R9, R1, #4 
    /*0000a8e8*/ MOV         FP, R1 
    /*0000a8ec*/ MOV         R8, #0 
    /*0000a8f0*/ MOV         R5, R2 
    /*0000a8f4*/ B           SVP_NNIE_FillForwardWithRoiTask_x9ff0
    SVP_NNIE_FillForwardWithRoiTask_xa8f8:
    /*0000a8f8*/ MOV         R6, R8 
    SVP_NNIE_FillForwardWithRoiTask_xa8fc:
    /*0000a8fc*/ LDR         R3, [SP, #196]              @ 0xc4 
    /*0000a900*/ LDR         R2, [SP, #140]              @ 0x8c 
    /*0000a904*/ CMP         R2, R3 
    /*0000a908*/ ADD         R3, R3, #1 
    /*0000a90c*/ BLS         SVP_NNIE_FillForwardWithRoiTask_xac94
    /*0000a910*/ STR         R3, [SP, #196]              @ 0xc4 
    /*0000a914*/ B           SVP_NNIE_FillForwardWithRoiTask_xa464
    SVP_NNIE_FillForwardWithRoiTask_xa918:
    /*0000a918*/ LDR         R3, [SP, #132]              @ 0x84 
    /*0000a91c*/ MOV         R5, #0 
    /*0000a920*/ ADD         R2, R3, #12 
    /*0000a924*/ ADD         SL, R3, #4 
    /*0000a928*/ MOV         FP, R3 
    /*0000a92c*/ MOV         R6, R2 
    /*0000a930*/ B           SVP_NNIE_FillForwardWithRoiTask_x9de4
    SVP_NNIE_FillForwardWithRoiTask_xa934:
    /*0000a934*/ MOV         R8, R2 
    /*0000a938*/ MOV         R2, R3 
    /*0000a93c*/ B           SVP_NNIE_FillForwardWithRoiTask_xa3f0
    SVP_NNIE_FillForwardWithRoiTask_xa940:
    /*0000a940*/ LDR         R2, [SP, #120]              @ 0x78 
    /*0000a944*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*0000a948*/ LDR         R1, [SP, #100]              @ 0x64 
    /*0000a94c*/ LDR         R6, [R2, R3, LSL #2] 
    /*0000a950*/ LDR         R2, [R9] 
    /*0000a954*/ ADD         R6, R6, #4096               @ 0x1000 
    /*0000a958*/ LDR         R3, [R1, R3, LSL #2] 
    /*0000a95c*/ SUB         R6, R6, R2 
    /*0000a960*/ LDR         R2, [SP, #64]               @ 0x40 
    /*0000a964*/ CMP         R6, #4096                   @ 0x1000 
    /*0000a968*/ ADD         R3, R3, #4096               @ 0x1000 
    /*0000a96c*/ LDR         FP, [R2] 
    /*0000a970*/ MOVCC       R6, #4096                   @ 0x1000 
    /*0000a974*/ SUB         R3, R3, FP 
    /*0000a978*/ CMP         R3, #4096                   @ 0x1000 
    /*0000a97c*/ MOVCC       R3, #4096                   @ 0x1000 
    /*0000a980*/ STR         R3, [SP, #164]              @ 0xa4 
    /*0000a984*/ LDR         R3, [SP, #112]              @ 0x70 
    /*0000a988*/ CMP         R3, #0 
    /*0000a98c*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb358
    /*0000a990*/ MOV         R3, #0 
    /*0000a994*/ STR         R3, [SP, #152]              @ 0x98 
    /*0000a998*/ MOV         R0, R3 
    SVP_NNIE_FillForwardWithRoiTask_xa99c:
    /*0000a99c*/ LDR         R3, [SP, #164]              @ 0xa4 
    /*0000a9a0*/ LSL         R4, R0, #12 
    /*0000a9a4*/ ADD         R0, R0, #1 
    /*0000a9a8*/ STR         R0, [SP, #152]              @ 0x98 
    /*0000a9ac*/ LDR         R1, [SP, #180]              @ 0xb4 
    /*0000a9b0*/ UMULL       R2, R3, R4, R3 
    /*0000a9b4*/ LSR         R0, R2, #12 
    /*0000a9b8*/ ORR         R0, R0, R3, LSL #20 
    /*0000a9bc*/ BL          0x000000000000A9BC          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000a9c0*/ LDR         R3, [SP, #164]              @ 0xa4 
    /*0000a9c4*/ ADD         R2, R4, #4096               @ 0x1000 
    /*0000a9c8*/ LDR         R1, [SP, #180]              @ 0xb4 
    /*0000a9cc*/ UMULL       R2, R3, R2, R3 
    /*0000a9d0*/ STRD        R2, [SP, #24] 
    /*0000a9d4*/ LSR         R2, R2, #12 
    /*0000a9d8*/ LDR         R3, [SP, #28] 
    /*0000a9dc*/ AND         R4, R0, R5 
    /*0000a9e0*/ ORR         R0, R2, R3, LSL #20 
    /*0000a9e4*/ BL          0x000000000000A9E4          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000a9e8*/ LDR         R1, [SP, #200]              @ 0xc8 
    /*0000a9ec*/ BICS        R3, R0, R5 
    /*0000a9f0*/ ADD         R3, R4, FP 
    /*0000a9f4*/ AND         R0, R0, R5 
    /*0000a9f8*/ ADDNE       R0, R0, #4096               @ 0x1000 
    /*0000a9fc*/ CMP         R1, R3 
    /*0000aa00*/ ADD         R2, R0, FP 
    /*0000aa04*/ MOVCC       R3, R1 
    /*0000aa08*/ CMP         R2, R1 
    /*0000aa0c*/ RSBLS       R2, R3, R2 
    /*0000aa10*/ RSBHI       R2, R3, R1 
    /*0000aa14*/ LDR         R1, [SP, #44]               @ 0x2c 
    /*0000aa18*/ LSR         R2, R2, #12 
    /*0000aa1c*/ STR         R2, [SP, #56]               @ 0x38 
    /*0000aa20*/ CMP         R1, #0 
    /*0000aa24*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xab3c
    /*0000aa28*/ LDR         R1, [SP, #184]              @ 0xb8 
    /*0000aa2c*/ CLZ         R2, R2 
    /*0000aa30*/ MOV         FP, #0 
    /*0000aa34*/ SUB         R3, R3, R1 
    /*0000aa38*/ LSR         R2, R2, #5 
    /*0000aa3c*/ STR         R2, [SP, #52]               @ 0x34 
    /*0000aa40*/ LDR         R2, [SP, #192]              @ 0xc0 
    /*0000aa44*/ LSR         R3, R3, #12 
    /*0000aa48*/ LDR         R1, [SP, #176]              @ 0xb0 
    /*0000aa4c*/ MUL         R3, R2, R3 
    /*0000aa50*/ ADD         R4, R1, SL, LSL #2 
    /*0000aa54*/ STR         R3, [SP, #48]               @ 0x30 
    /*0000aa58*/ B           SVP_NNIE_FillForwardWithRoiTask_xaa80
    SVP_NNIE_FillForwardWithRoiTask_xaa5c:
    /*0000aa5c*/ CMP         R3, #1024                   @ 0x400 
    /*0000aa60*/ BHI         SVP_NNIE_FillForwardWithRoiTask_xab5c
    SVP_NNIE_FillForwardWithRoiTask_xaa64:
    /*0000aa64*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*0000aa68*/ STM         R4, {R0, R2} 
    /*0000aa6c*/ ADD         R4, R4, #16 
    /*0000aa70*/ CMP         R3, FP 
    /*0000aa74*/ LDR         R3, [SP, #56]               @ 0x38 
    /*0000aa78*/ STR         R3, [R4, #-8] 
    /*0000aa7c*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xab3c
    SVP_NNIE_FillForwardWithRoiTask_xaa80:
    /*0000aa80*/ LSL         R2, FP, #12 
    /*0000aa84*/ STR         R2, [SP, #24] 
    /*0000aa88*/ MOV         R1, R8 
    /*0000aa8c*/ ADD         FP, FP, #1 
    /*0000aa90*/ UMULL       R2, R3, R2, R6 
    /*0000aa94*/ ADD         SL, SL, #4 
    /*0000aa98*/ LSR         R0, R2, #12 
    /*0000aa9c*/ ORR         R0, R0, R3, LSL #20 
    /*0000aaa0*/ BL          0x000000000000AAA0          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000aaa4*/ LDR         R3, [SP, #24] 
    /*0000aaa8*/ MOV         R1, R8 
    /*0000aaac*/ ADD         R2, R3, #4096               @ 0x1000 
    /*0000aab0*/ UMULL       R2, R3, R2, R6 
    /*0000aab4*/ LSR         IP, R2, #12 
    /*0000aab8*/ AND         LR, R0, R5 
    /*0000aabc*/ ORR         R0, IP, R3, LSL #20 
    /*0000aac0*/ STR         LR, [SP, #24] 
    /*0000aac4*/ BL          0x000000000000AAC4          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000aac8*/ LDR         R1, [R9] 
    /*0000aacc*/ LDR         LR, [SP, #24] 
    /*0000aad0*/ BICS        R3, R0, R5 
    /*0000aad4*/ ADD         R3, LR, R1 
    /*0000aad8*/ AND         R0, R0, R5 
    /*0000aadc*/ ADDNE       R0, R0, #4096               @ 0x1000 
    /*0000aae0*/ CMP         R3, R7 
    /*0000aae4*/ ADD         R2, R1, R0 
    /*0000aae8*/ LDR         R1, [SP, #136]              @ 0x88 
    /*0000aaec*/ MOVCS       R3, R7 
    /*0000aaf0*/ LDR         R0, [SP, #48]               @ 0x30 
    /*0000aaf4*/ CMP         R2, R7 
    /*0000aaf8*/ RSBLS       R2, R3, R2 
    /*0000aafc*/ RSBHI       R2, R3, R7 
    /*0000ab00*/ LSR         R3, R3, #12 
    /*0000ab04*/ MLA         R3, R1, R3, R0 
    /*0000ab08*/ LDR         R1, [SP, #52]               @ 0x34 
    /*0000ab0c*/ LSR         R2, R2, #12 
    /*0000ab10*/ CMP         R3, #1024                   @ 0x400 
    /*0000ab14*/ LSL         R0, R3, #5 
    /*0000ab18*/ SUBHI       R3, R3, #1024               @ 0x400 
    /*0000ab1c*/ LSLHI       R0, R3, #5 
    /*0000ab20*/ ADDHI       R0, R0, #1 
    /*0000ab24*/ CMP         R2, #0 
    /*0000ab28*/ ORREQ       R1, R1, #1 
    /*0000ab2c*/ CMP         R1, #0 
    /*0000ab30*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xaa5c
    /*0000ab34*/ MOV         R0, #0 
    /*0000ab38*/ B           SVP_NNIE_FillForwardWithRoiTask_xaa64
    SVP_NNIE_FillForwardWithRoiTask_xab3c:
    /*0000ab3c*/ LDR         R3, [SP, #112]              @ 0x70 
    /*0000ab40*/ LDR         R2, [SP, #152]              @ 0x98 
    /*0000ab44*/ CMP         R3, R2 
    /*0000ab48*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb358
    /*0000ab4c*/ LDR         R3, [SP, #64]               @ 0x40 
    /*0000ab50*/ MOV         R0, R2 
    /*0000ab54*/ LDR         FP, [R3] 
    /*0000ab58*/ B           SVP_NNIE_FillForwardWithRoiTask_xa99c
    SVP_NNIE_FillForwardWithRoiTask_xab5c:
    /*0000ab5c*/ MOV         R9, R3 
    /*0000ab60*/ MOV         R6, R1 
    /*0000ab64*/ MOV         R3, #980                    @ 0x3d4 
    SVP_NNIE_FillForwardWithRoiTask_xab68:
    /*0000ab68*/ STR         R3, [SP] 
    /*0000ab6c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*0000ab70*/ LDR         R3, [PC, #272]              @ 0x000000000000ac88 
    /*0000ab74*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*0000ab78*/ MOV         R1, #51                     @ 0x33 
    /*0000ab7c*/ MOV         R0, #3 
    /*0000ab80*/ BL          0x000000000000AB80          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000ab84*/ LDR         R2, [SP, #60]               @ 0x3c 
    /*0000ab88*/ LDR         R5, [SP, #72]               @ 0x48 
    /*0000ab8c*/ MOVW        R4, #32771                  @ 0x8003 
    /*0000ab90*/ STR         R6, [SP, #20] 
    /*0000ab94*/ ADD         IP, R5, R2, LSL #4 
    /*0000ab98*/ MOV         R6, R2 
    /*0000ab9c*/ MOV         R3, R2 
    /*0000aba0*/ STR         R9, [SP, #16] 
    /*0000aba4*/ B           SVP_NNIE_FillForwardWithRoiTask_xa3a8
    SVP_NNIE_FillForwardWithRoiTask_xaba8:
    /*0000aba8*/ LDR         FP, [SL, #24] 
    /*0000abac*/ CMP         FP, #0 
    /*0000abb0*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_x98a0
    /*0000abb4*/ MOV         IP, #0 
    /*0000abb8*/ ADD         LR, R9, #32 
    /*0000abbc*/ MOV         FP, IP 
    /*0000abc0*/ B           SVP_NNIE_FillForwardWithRoiTask_xabc8
    SVP_NNIE_FillForwardWithRoiTask_xabc4:
    /*0000abc4*/ LDR         R0, [SL, #40]               @ 0x28 
    SVP_NNIE_FillForwardWithRoiTask_xabc8:
    /*0000abc8*/ UMULL       R2, R3, R0, R6 
    /*0000abcc*/ LDRD        R4, [SL, #16] 
    /*0000abd0*/ ADD         FP, FP, #32 
    /*0000abd4*/ ADD         LR, LR, #32 
    /*0000abd8*/ UMULL       R0, R1, R2, IP 
    /*0000abdc*/ MLA         R3, IP, R3, R1 
    /*0000abe0*/ STRD        R0, [SP, #32] 
    /*0000abe4*/ STR         R3, [SP, #36]               @ 0x24 
    /*0000abe8*/ LDRD        R0, [SP, #32] 
    /*0000abec*/ ADDS        R0, R0, R4 
    /*0000abf0*/ ADC         R1, R1, R5 
    /*0000abf4*/ STRD        R0, [LR, #-64]              @ 0xffffffc0 
    /*0000abf8*/ LDR         R3, [SL, #40]               @ 0x28 
    /*0000abfc*/ MOV         R0, #1 
    /*0000ac00*/ MOV         R1, #0 
    /*0000ac04*/ LDRD        R4, [SL, #16] 
    /*0000ac08*/ UMLAL       R0, R1, R3, IP 
    /*0000ac0c*/ UMULL       R2, R3, R0, R6 
    /*0000ac10*/ MOV         R0, #0 
    /*0000ac14*/ STRD        R2, [SP, #32] 
    /*0000ac18*/ MLA         R3, R6, R1, R3 
    /*0000ac1c*/ MOV         R1, #0 
    /*0000ac20*/ STR         R3, [SP, #36]               @ 0x24 
    /*0000ac24*/ LDRD        R2, [SP, #32] 
    /*0000ac28*/ ADDS        R2, R2, R4 
    /*0000ac2c*/ ADC         R3, R3, R5 
    /*0000ac30*/ STRD        R2, [LR, #-56]              @ 0xffffffc8 
    /*0000ac34*/ LDR         R3, [SL, #40]               @ 0x28 
    /*0000ac38*/ LDRD        R4, [SL, #16] 
    /*0000ac3c*/ STRD        R0, [LR, #-40]              @ 0xffffffd8 
    /*0000ac40*/ MOV         R0, #2 
    /*0000ac44*/ MOV         R1, #0 
    /*0000ac48*/ UMLAL       R0, R1, R3, IP 
    /*0000ac4c*/ ADD         IP, IP, #1 
    /*0000ac50*/ UMULL       R2, R3, R0, R6 
    /*0000ac54*/ STRD        R2, [SP, #32] 
    /*0000ac58*/ MLA         R3, R6, R1, R3 
    /*0000ac5c*/ STR         R3, [SP, #36]               @ 0x24 
    /*0000ac60*/ LDRD        R2, [SP, #32] 
    /*0000ac64*/ ADDS        R2, R2, R4 
    /*0000ac68*/ ADC         R3, R3, R5 
    /*0000ac6c*/ STRD        R2, [LR, #-48]              @ 0xffffffd0 
    /*0000ac70*/ LDR         R3, [SL, #24] 
    /*0000ac74*/ CMP         R3, IP 
    /*0000ac78*/ BHI         SVP_NNIE_FillForwardWithRoiTask_xabc4
    /*0000ac7c*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*0000ac80*/ LDR         R1, [R3] 
    /*0000ac84*/ B           SVP_NNIE_FillForwardWithRoiTask_x98a0
    /*0000ac88*/ ANDEQ       R0, R0, R8, LSL R3 
    /*0000ac8c*/ ANDEQ       R0, R0, IP, LSR #6 
    /*0000ac90*/ STRDEQ      R0, [R0], -R8 
    SVP_NNIE_FillForwardWithRoiTask_xac94:
    /*0000ac94*/ LDR         R8, [SP, #208]              @ 0xd0 
    /*0000ac98*/ MOV         R9, R6 
    SVP_NNIE_FillForwardWithRoiTask_xac9c:
    /*0000ac9c*/ LDR         R3, [SP, #176]              @ 0xb0 
    /*0000aca0*/ LDR         R1, [SP, #108]              @ 0x6c 
    /*0000aca4*/ LDR         R2, [SP, #184]              @ 0xb8 
    /*0000aca8*/ MUL         R3, R8, R3 
    /*0000acac*/ CMN         R2, #1 
    /*0000acb0*/ STR         R3, [R1, #16] 
    /*0000acb4*/ LSRNE       R1, R2, #2 
    /*0000acb8*/ ADD         R2, R3, #1 
    /*0000acbc*/ LDR         R3, [SP, #188]              @ 0xbc 
    /*0000acc0*/ ADDNE       R1, R1, #1 
    /*0000acc4*/ MOVEQ       R1, #1 
    /*0000acc8*/ ADD         R3, R3, #1 
    /*0000accc*/ ADD         R2, R2, R1 
    /*0000acd0*/ ADD         R3, R3, R1 
    /*0000acd4*/ LDR         R1, [SP, #32] 
    /*0000acd8*/ LSL         R2, R2, #4 
    /*0000acdc*/ CMP         R1, #0 
    /*0000ace0*/ LDR         R1, [SP, #108]              @ 0x6c 
    /*0000ace4*/ LSL         R3, R3, #4 
    /*0000ace8*/ STR         R2, [R1, #8] 
    /*0000acec*/ STR         R3, [R1, #12] 
    /*0000acf0*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xaf1c
    /*0000acf4*/ LDR         R3, [SP, #140]              @ 0x8c 
    /*0000acf8*/ CMP         R3, #0 
    /*0000acfc*/ LDR         R3, [SP, #200]              @ 0xc8 
    /*0000ad00*/ LSR         R3, R3, #12 
    /*0000ad04*/ STR         R3, [SP, #48]               @ 0x30 
    /*0000ad08*/ LDR         R3, [SP, #204]              @ 0xcc 
    /*0000ad0c*/ LSR         FP, R3, #12 
    /*0000ad10*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xaf1c
    /*0000ad14*/ LDR         R2, [SP, #160]              @ 0xa0 
    /*0000ad18*/ LDR         R3, [SP, #152]              @ 0x98 
    /*0000ad1c*/ SUB         R2, R2, #4 
    /*0000ad20*/ STR         R2, [SP, #100]              @ 0x64 
    /*0000ad24*/ LDR         R2, [SP, #180]              @ 0xb4 
    /*0000ad28*/ STR         R3, [SP, #64]               @ 0x40 
    /*0000ad2c*/ ADD         R3, R3, R2 
    /*0000ad30*/ STR         R3, [SP, #104]              @ 0x68 
    /*0000ad34*/ LDR         R3, [SP, #156]              @ 0x9c 
    /*0000ad38*/ LDR         R5, [SP, #56]               @ 0x38 
    /*0000ad3c*/ SUB         R3, R3, #4 
    /*0000ad40*/ STR         R3, [SP, #72]               @ 0x48 
    /*0000ad44*/ LDR         R3, [SP, #164]              @ 0xa4 
    /*0000ad48*/ SUB         R3, R3, #4 
    /*0000ad4c*/ STR         R3, [SP, #68]               @ 0x44 
    SVP_NNIE_FillForwardWithRoiTask_xad50:
    /*0000ad50*/ LDR         R3, [SP, #64]               @ 0x40 
    /*0000ad54*/ LDR         R1, [SP, #68]               @ 0x44 
    /*0000ad58*/ LDR         SL, [R3], #4 
    /*0000ad5c*/ STR         R3, [SP, #64]               @ 0x40 
    /*0000ad60*/ MOV         R3, #0 
    /*0000ad64*/ STR         R3, [SP, #44]               @ 0x2c 
    /*0000ad68*/ LDR         R3, [SP, #100]              @ 0x64 
    /*0000ad6c*/ LDR         R8, [R3, #4]! 
    /*0000ad70*/ SUB         R8, R8, SL 
    /*0000ad74*/ STR         R3, [SP, #100]              @ 0x64 
    /*0000ad78*/ CMP         R8, #1 
    /*0000ad7c*/ LDR         R3, [SP, #72]               @ 0x48 
    /*0000ad80*/ MOVCC       R8, #1 
    /*0000ad84*/ LDR         R2, [R3, #4]! 
    /*0000ad88*/ STR         R3, [SP, #72]               @ 0x48 
    /*0000ad8c*/ LDR         R3, [R1, #4]! 
    /*0000ad90*/ STR         R2, [SP, #56]               @ 0x38 
    /*0000ad94*/ SUB         R3, R3, R2 
    /*0000ad98*/ CMP         R3, #1 
    /*0000ad9c*/ STR         R1, [SP, #68]               @ 0x44 
    /*0000ada0*/ MOVCC       R3, #1 
    /*0000ada4*/ STR         R3, [SP, #60]               @ 0x3c 
    SVP_NNIE_FillForwardWithRoiTask_xada8:
    /*0000ada8*/ LDR         R0, [SP, #44]               @ 0x2c 
    /*0000adac*/ LDR         R6, [SP, #60]               @ 0x3c 
    /*0000adb0*/ LDR         R1, [SP, #48]               @ 0x30 
    /*0000adb4*/ LSL         R4, R0, #12 
    /*0000adb8*/ ADD         R0, R0, #1 
    /*0000adbc*/ STR         R0, [SP, #44]               @ 0x2c 
    /*0000adc0*/ UMULL       R2, R3, R4, R6 
    /*0000adc4*/ LSR         R0, R2, #12 
    /*0000adc8*/ ORR         R0, R0, R3, LSL #20 
    /*0000adcc*/ BL          0x000000000000ADCC          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000add0*/ ADD         R2, R4, #4096               @ 0x1000 
    /*0000add4*/ LDR         R1, [SP, #48]               @ 0x30 
    /*0000add8*/ UMULL       R2, R3, R2, R6 
    /*0000addc*/ LDR         R6, [SP, #56]               @ 0x38 
    /*0000ade0*/ STRD        R2, [SP, #24] 
    /*0000ade4*/ LSR         R2, R2, #12 
    /*0000ade8*/ LDR         R3, [SP, #28] 
    /*0000adec*/ ADD         R4, R0, R6 
    /*0000adf0*/ ORR         R0, R2, R3, LSL #20 
    /*0000adf4*/ BL          0x000000000000ADF4          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000adf8*/ LDR         R2, [SP, #96]               @ 0x60 
    /*0000adfc*/ AND         R4, R4, R7 
    /*0000ae00*/ ADD         R0, R0, R6 
    /*0000ae04*/ AND         R3, R0, R7 
    /*0000ae08*/ CMP         R0, R3 
    /*0000ae0c*/ ADDNE       R3, R3, #4096               @ 0x1000 
    /*0000ae10*/ CMP         R2, R4 
    /*0000ae14*/ MOVCC       R4, R2 
    /*0000ae18*/ CMP         R2, R3 
    /*0000ae1c*/ RSBLS       R4, R4, R2 
    /*0000ae20*/ RSBHI       R4, R4, R3 
    /*0000ae24*/ LSRS        R4, R4, #12 
    /*0000ae28*/ LSLNE       R3, R4, #12 
    /*0000ae2c*/ MOVEQ       R3, #4096                   @ 0x1000 
    /*0000ae30*/ STR         R3, [SP, #24] 
    /*0000ae34*/ LDR         R3, [SP, #136]              @ 0x88 
    /*0000ae38*/ CMP         R3, #0 
    /*0000ae3c*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xaefc
    /*0000ae40*/ LDR         R2, [SP, #172]              @ 0xac 
    /*0000ae44*/ ADD         R3, R3, R5 
    /*0000ae48*/ MOV         R4, #4096                   @ 0x1000 
    /*0000ae4c*/ STR         R3, [SP, #52]               @ 0x34 
    /*0000ae50*/ ADD         R5, R2, R5, LSL #2 
    /*0000ae54*/ ADD         R3, R2, R3, LSL #2 
    /*0000ae58*/ STR         R3, [SP, #32] 
    SVP_NNIE_FillForwardWithRoiTask_xae5c:
    /*0000ae5c*/ SUB         R2, R4, #4096               @ 0x1000 
    /*0000ae60*/ MOV         R1, FP 
    /*0000ae64*/ UMULL       R2, R3, R2, R8 
    /*0000ae68*/ LSR         R0, R2, #12 
    /*0000ae6c*/ ORR         R0, R0, R3, LSL #20 
    /*0000ae70*/ BL          0x000000000000AE70          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000ae74*/ UMULL       R2, R3, R4, R8 
    /*0000ae78*/ MOV         R1, FP 
    /*0000ae7c*/ LSR         IP, R2, #12 
    /*0000ae80*/ ADD         R6, R0, SL 
    /*0000ae84*/ ORR         R0, IP, R3, LSL #20 
    /*0000ae88*/ BL          0x000000000000AE88          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000ae8c*/ MOV         R2, #4096                   @ 0x1000 
    /*0000ae90*/ AND         R6, R6, R7 
    /*0000ae94*/ ADD         R0, R0, SL 
    /*0000ae98*/ AND         R1, R0, R7 
    /*0000ae9c*/ ADD         IP, R1, R2 
    /*0000aea0*/ CMP         R0, R1 
    /*0000aea4*/ MOV         R0, #4096                   @ 0x1000 
    /*0000aea8*/ ADD         R4, R4, R0 
    /*0000aeac*/ MOVNE       R1, IP 
    /*0000aeb0*/ CMP         R9, R6 
    /*0000aeb4*/ MOVCC       R3, R9 
    /*0000aeb8*/ MOVCS       R3, R6 
    /*0000aebc*/ CMP         R1, R9 
    /*0000aec0*/ RSBLS       R3, R3, R1 
    /*0000aec4*/ RSBHI       R3, R3, R9 
    /*0000aec8*/ LSRS        R3, R3, #12 
    /*0000aecc*/ LSLNE       R2, R3, #12 
    /*0000aed0*/ LDR         R3, [SP, #24] 
    /*0000aed4*/ UMULL       R2, R3, R2, R3 
    /*0000aed8*/ LSR         R1, R2, #12 
    /*0000aedc*/ ORR         R1, R1, R3, LSL #20 
    /*0000aee0*/ LSR         R1, R1, #12 
    /*0000aee4*/ BL          0x000000000000AEE4          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000aee8*/ LDR         R3, [SP, #32] 
    /*0000aeec*/ STR         R0, [R5], #4 
    /*0000aef0*/ CMP         R3, R5 
    /*0000aef4*/ BNE         SVP_NNIE_FillForwardWithRoiTask_xae5c
    /*0000aef8*/ LDR         R5, [SP, #52]               @ 0x34 
    SVP_NNIE_FillForwardWithRoiTask_xaefc:
    /*0000aefc*/ LDR         R3, [SP, #140]              @ 0x8c 
    /*0000af00*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*0000af04*/ CMP         R3, R2 
    /*0000af08*/ BNE         SVP_NNIE_FillForwardWithRoiTask_xada8
    /*0000af0c*/ LDR         R3, [SP, #104]              @ 0x68 
    /*0000af10*/ LDR         R2, [SP, #64]               @ 0x40 
    /*0000af14*/ CMP         R3, R2 
    /*0000af18*/ BNE         SVP_NNIE_FillForwardWithRoiTask_xad50
    SVP_NNIE_FillForwardWithRoiTask_xaf1c:
    /*0000af1c*/ LDR         R0, [SP, #132]              @ 0x84 
    /*0000af20*/ BL          0x000000000000AF20          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    SVP_NNIE_FillForwardWithRoiTask_xaf24:
    /*0000af24*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*0000af28*/ MOVW        R2, #1692                   @ 0x69c 
    /*0000af2c*/ LDR         R0, [R3, #12] 
    /*0000af30*/ LDR         R3, [SP, #92]               @ 0x5c 
    /*0000af34*/ ADD         R3, R3, #4 
    /*0000af38*/ STR         R3, [SP, #92]               @ 0x5c 
    /*0000af3c*/ LDR         R3, [SP, #88]               @ 0x58 
    /*0000af40*/ ADD         R3, R3, #48                 @ 0x30 
    /*0000af44*/ STR         R3, [SP, #88]               @ 0x58 
    /*0000af48*/ LDR         R3, [SP, #80]               @ 0x50 
    /*0000af4c*/ MLA         R2, R2, R0, R3 
    /*0000af50*/ LDR         R3, [SP, #84]               @ 0x54 
    /*0000af54*/ LDRH        R1, [R2, #20] 
    /*0000af58*/ CMP         R1, R3 
    /*0000af5c*/ BLS         SVP_NNIE_FillForwardWithRoiTask_x9754
    /*0000af60*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*0000af64*/ LDR         LR, [SP, #84]               @ 0x54 
    /*0000af68*/ LDR         IP, [R3, #8] 
    /*0000af6c*/ B           SVP_NNIE_FillForwardWithRoiTask_x90f4
    SVP_NNIE_FillForwardWithRoiTask_xaf70:
    /*0000af70*/ LDR         R2, [SP, #316]              @ 0x13c 
    /*0000af74*/ LDR         R2, [R2, #4] 
    /*0000af78*/ CMP         R2, #0 
    /*0000af7c*/ LDREQ       R3, [SP, #144]              @ 0x90 
    /*0000af80*/ STREQ       R3, [SP, #224]              @ 0xe0 
    /*0000af84*/ BNE         SVP_NNIE_FillForwardWithRoiTask_x8ec0
    /*0000af88*/ B           SVP_NNIE_FillForwardWithRoiTask_x8f24
    SVP_NNIE_FillForwardWithRoiTask_xaf8c:
    /*0000af8c*/ LDR         R0, [SP, #144]              @ 0x90 
    /*0000af90*/ BL          0x000000000000AF90          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    /*0000af94*/ B           SVP_NNIE_FillForwardWithRoiTask_x9eb8
    SVP_NNIE_FillForwardWithRoiTask_xaf98:
    /*0000af98*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*0000af9c*/ LSL         R3, R3, #12 
    /*0000afa0*/ STR         R3, [SP, #200]              @ 0xc8 
    SVP_NNIE_FillForwardWithRoiTask_xafa4:
    /*0000afa4*/ LDR         R2, [SP, #32] 
    /*0000afa8*/ MOV         R3, R2 
    /*0000afac*/ MOV         SL, R2 
    /*0000afb0*/ STR         R2, [SP, #228]              @ 0xe4 
    /*0000afb4*/ B           SVP_NNIE_FillForwardWithRoiTask_xa17c
    SVP_NNIE_FillForwardWithRoiTask_xafb8:
    /*0000afb8*/ MOV         SL, R8 
    /*0000afbc*/ MOV         R7, R5 
    /*0000afc0*/ STR         R3, [SP, #152]              @ 0x98 
    SVP_NNIE_FillForwardWithRoiTask_xafc4:
    /*0000afc4*/ LDR         R3, [SP, #32] 
    /*0000afc8*/ LDR         R2, [SP, #140]              @ 0x8c 
    /*0000afcc*/ CMP         R3, #0 
    /*0000afd0*/ LDR         R3, [SP, #200]              @ 0xc8 
    /*0000afd4*/ SUB         R3, R3, R2, LSL #12 
    /*0000afd8*/ STR         R3, [SP, #164]              @ 0xa4 
    /*0000afdc*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb4b8
    /*0000afe0*/ LDR         R2, [SP, #208]              @ 0xd0 
    /*0000afe4*/ MOV         R3, #0 
    /*0000afe8*/ LDR         R8, [SP, #124]              @ 0x7c 
    /*0000afec*/ STR         R3, [SP, #140]              @ 0x8c 
    /*0000aff0*/ LSR         R2, R2, #12 
    /*0000aff4*/ STR         R2, [SP, #160]              @ 0xa0 
    /*0000aff8*/ LDR         R2, [SP, #204]              @ 0xcc 
    /*0000affc*/ MOV         R9, R8 
    /*0000b000*/ STR         R3, [SP, #60]               @ 0x3c 
    /*0000b004*/ LSR         R6, R2, #12 
    /*0000b008*/ LDR         R2, [SP, #132]              @ 0x84 
    /*0000b00c*/ MOV         R8, R6 
    /*0000b010*/ LSL         R5, R2, #12 
    /*0000b014*/ LDR         R2, [SP, #68]               @ 0x44 
    /*0000b018*/ SUB         R2, R2, #4 
    /*0000b01c*/ STR         R2, [SP, #132]              @ 0x84 
    /*0000b020*/ LDR         R2, [SP, #116]              @ 0x74 
    /*0000b024*/ STR         R2, [SP, #64]               @ 0x40 
    SVP_NNIE_FillForwardWithRoiTask_xb028:
    /*0000b028*/ LDR         R2, [SP, #132]              @ 0x84 
    /*0000b02c*/ LDR         R3, [R2, #4]! 
    /*0000b030*/ STR         R2, [SP, #132]              @ 0x84 
    /*0000b034*/ LDR         R2, [SP, #104]              @ 0x68 
    /*0000b038*/ CMP         R2, R3 
    /*0000b03c*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb088
    SVP_NNIE_FillForwardWithRoiTask_xb040:
    /*0000b040*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*0000b044*/ ADD         R9, R9, #4 
    /*0000b048*/ LDR         R2, [SP, #32] 
    /*0000b04c*/ ADD         R3, R3, #1 
    /*0000b050*/ CMP         R2, R3 
    /*0000b054*/ LDR         R2, [SP, #64]               @ 0x40 
    /*0000b058*/ ADD         R2, R2, #4 
    /*0000b05c*/ STR         R2, [SP, #64]               @ 0x40 
    /*0000b060*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb3a8
    /*0000b064*/ STR         R3, [SP, #60]               @ 0x3c 
    /*0000b068*/ B           SVP_NNIE_FillForwardWithRoiTask_xb028
    SVP_NNIE_FillForwardWithRoiTask_xb06c:
    /*0000b06c*/ MVN         R2, #1 
    /*0000b070*/ STR         R2, [SP, #240]              @ 0xf0 
    /*0000b074*/ LDR         R2, [SP, #108]              @ 0x6c 
    /*0000b078*/ ADD         R3, R3, #2 
    /*0000b07c*/ LSL         R3, R3, #4 
    /*0000b080*/ STR         R3, [R2, #8] 
    /*0000b084*/ B           SVP_NNIE_FillForwardWithRoiTask_xa1b8
    SVP_NNIE_FillForwardWithRoiTask_xb088:
    /*0000b088*/ LDR         R2, [SP, #120]              @ 0x78 
    /*0000b08c*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*0000b090*/ LDR         R1, [SP, #100]              @ 0x64 
    /*0000b094*/ LDR         R6, [R2, R3, LSL #2] 
    /*0000b098*/ LDR         R2, [R9] 
    /*0000b09c*/ ADD         R6, R6, #4096               @ 0x1000 
    /*0000b0a0*/ LDR         R3, [R1, R3, LSL #2] 
    /*0000b0a4*/ SUB         R6, R6, R2 
    /*0000b0a8*/ LDR         R2, [SP, #64]               @ 0x40 
    /*0000b0ac*/ CMP         R6, #4096                   @ 0x1000 
    /*0000b0b0*/ ADD         R3, R3, #4096               @ 0x1000 
    /*0000b0b4*/ LDR         FP, [R2] 
    /*0000b0b8*/ MOVCC       R6, #4096                   @ 0x1000 
    /*0000b0bc*/ SUB         R3, R3, FP 
    /*0000b0c0*/ CMP         R3, #4096                   @ 0x1000 
    /*0000b0c4*/ MOVCC       R3, #4096                   @ 0x1000 
    /*0000b0c8*/ STR         R3, [SP, #116]              @ 0x74 
    /*0000b0cc*/ LDR         R3, [SP, #112]              @ 0x70 
    /*0000b0d0*/ CMP         R3, #0 
    /*0000b0d4*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb380
    /*0000b0d8*/ MOV         R3, #0 
    /*0000b0dc*/ STR         R3, [SP, #68]               @ 0x44 
    /*0000b0e0*/ MOV         R0, R3 
    SVP_NNIE_FillForwardWithRoiTask_xb0e4:
    /*0000b0e4*/ LDR         R3, [SP, #116]              @ 0x74 
    /*0000b0e8*/ LSL         R4, R0, #12 
    /*0000b0ec*/ ADD         R0, R0, #1 
    /*0000b0f0*/ STR         R0, [SP, #68]               @ 0x44 
    /*0000b0f4*/ LDR         R1, [SP, #160]              @ 0xa0 
    /*0000b0f8*/ UMULL       R2, R3, R4, R3 
    /*0000b0fc*/ LSR         R0, R2, #12 
    /*0000b100*/ ORR         R0, R0, R3, LSL #20 
    /*0000b104*/ BL          0x000000000000B104          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000b108*/ LDR         R3, [SP, #116]              @ 0x74 
    /*0000b10c*/ ADD         R2, R4, #4096               @ 0x1000 
    /*0000b110*/ LDR         R1, [SP, #160]              @ 0xa0 
    /*0000b114*/ UMULL       R2, R3, R2, R3 
    /*0000b118*/ STRD        R2, [SP, #24] 
    /*0000b11c*/ LSR         R2, R2, #12 
    /*0000b120*/ LDR         R3, [SP, #28] 
    /*0000b124*/ AND         R4, R0, R7 
    /*0000b128*/ ORR         R0, R2, R3, LSL #20 
    /*0000b12c*/ BL          0x000000000000B12C          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000b130*/ LDR         R1, [SP, #200]              @ 0xc8 
    /*0000b134*/ BICS        R3, R0, R7 
    /*0000b138*/ ADD         R3, R4, FP 
    /*0000b13c*/ AND         R0, R0, R7 
    /*0000b140*/ ADDNE       R0, R0, #4096               @ 0x1000 
    /*0000b144*/ CMP         R1, R3 
    /*0000b148*/ ADD         R2, R0, FP 
    /*0000b14c*/ MOVCC       R3, R1 
    /*0000b150*/ CMP         R2, R1 
    /*0000b154*/ RSBLS       R2, R3, R2 
    /*0000b158*/ RSBHI       R2, R3, R1 
    /*0000b15c*/ LDR         R1, [SP, #44]               @ 0x2c 
    /*0000b160*/ LSR         R2, R2, #12 
    /*0000b164*/ STR         R2, [SP, #56]               @ 0x38 
    /*0000b168*/ CMP         R1, #0 
    /*0000b16c*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb284
    /*0000b170*/ LDR         R1, [SP, #164]              @ 0xa4 
    /*0000b174*/ CLZ         R2, R2 
    /*0000b178*/ MOV         FP, #0 
    /*0000b17c*/ SUB         R3, R3, R1 
    /*0000b180*/ LSR         R2, R2, #5 
    /*0000b184*/ STR         R2, [SP, #52]               @ 0x34 
    /*0000b188*/ LDR         R2, [SP, #192]              @ 0xc0 
    /*0000b18c*/ LSR         R3, R3, #12 
    /*0000b190*/ LDR         R1, [SP, #176]              @ 0xb0 
    /*0000b194*/ MUL         R3, R2, R3 
    /*0000b198*/ ADD         R4, R1, SL, LSL #2 
    /*0000b19c*/ STR         R3, [SP, #48]               @ 0x30 
    /*0000b1a0*/ B           SVP_NNIE_FillForwardWithRoiTask_xb1c8
    SVP_NNIE_FillForwardWithRoiTask_xb1a4:
    /*0000b1a4*/ CMP         R3, #1024                   @ 0x400 
    /*0000b1a8*/ BHI         SVP_NNIE_FillForwardWithRoiTask_xb2a4
    SVP_NNIE_FillForwardWithRoiTask_xb1ac:
    /*0000b1ac*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*0000b1b0*/ STM         R4, {R0, R2} 
    /*0000b1b4*/ ADD         R4, R4, #16 
    /*0000b1b8*/ CMP         R3, FP 
    /*0000b1bc*/ LDR         R3, [SP, #56]               @ 0x38 
    /*0000b1c0*/ STR         R3, [R4, #-8] 
    /*0000b1c4*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb284
    SVP_NNIE_FillForwardWithRoiTask_xb1c8:
    /*0000b1c8*/ LSL         R2, FP, #12 
    /*0000b1cc*/ STR         R2, [SP, #24] 
    /*0000b1d0*/ MOV         R1, R8 
    /*0000b1d4*/ ADD         FP, FP, #1 
    /*0000b1d8*/ UMULL       R2, R3, R2, R6 
    /*0000b1dc*/ ADD         SL, SL, #4 
    /*0000b1e0*/ LSR         R0, R2, #12 
    /*0000b1e4*/ ORR         R0, R0, R3, LSL #20 
    /*0000b1e8*/ BL          0x000000000000B1E8          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000b1ec*/ LDR         R3, [SP, #24] 
    /*0000b1f0*/ MOV         R1, R8 
    /*0000b1f4*/ ADD         R2, R3, #4096               @ 0x1000 
    /*0000b1f8*/ UMULL       R2, R3, R2, R6 
    /*0000b1fc*/ LSR         IP, R2, #12 
    /*0000b200*/ AND         LR, R0, R7 
    /*0000b204*/ ORR         R0, IP, R3, LSL #20 
    /*0000b208*/ STR         LR, [SP, #24] 
    /*0000b20c*/ BL          0x000000000000B20C          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000b210*/ LDR         R1, [R9] 
    /*0000b214*/ LDR         LR, [SP, #24] 
    /*0000b218*/ BICS        R3, R0, R7 
    /*0000b21c*/ ADD         R3, LR, R1 
    /*0000b220*/ AND         R0, R0, R7 
    /*0000b224*/ ADDNE       R0, R0, #4096               @ 0x1000 
    /*0000b228*/ CMP         R3, R5 
    /*0000b22c*/ ADD         R2, R1, R0 
    /*0000b230*/ LDR         R1, [SP, #136]              @ 0x88 
    /*0000b234*/ MOVCS       R3, R5 
    /*0000b238*/ LDR         R0, [SP, #48]               @ 0x30 
    /*0000b23c*/ CMP         R2, R5 
    /*0000b240*/ RSBLS       R2, R3, R2 
    /*0000b244*/ RSBHI       R2, R3, R5 
    /*0000b248*/ LSR         R3, R3, #12 
    /*0000b24c*/ MLA         R3, R1, R3, R0 
    /*0000b250*/ LDR         R1, [SP, #52]               @ 0x34 
    /*0000b254*/ LSR         R2, R2, #12 
    /*0000b258*/ CMP         R3, #1024                   @ 0x400 
    /*0000b25c*/ LSL         R0, R3, #5 
    /*0000b260*/ SUBHI       R3, R3, #1024               @ 0x400 
    /*0000b264*/ LSLHI       R0, R3, #5 
    /*0000b268*/ ADDHI       R0, R0, #1 
    /*0000b26c*/ CMP         R2, #0 
    /*0000b270*/ ORREQ       R1, R1, #1 
    /*0000b274*/ CMP         R1, #0 
    /*0000b278*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb1a4
    /*0000b27c*/ MOV         R0, #0 
    /*0000b280*/ B           SVP_NNIE_FillForwardWithRoiTask_xb1ac
    SVP_NNIE_FillForwardWithRoiTask_xb284:
    /*0000b284*/ LDR         R3, [SP, #112]              @ 0x70 
    /*0000b288*/ LDR         R2, [SP, #68]               @ 0x44 
    /*0000b28c*/ CMP         R3, R2 
    /*0000b290*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb380
    /*0000b294*/ LDR         R3, [SP, #64]               @ 0x40 
    /*0000b298*/ MOV         R0, R2 
    /*0000b29c*/ LDR         FP, [R3] 
    /*0000b2a0*/ B           SVP_NNIE_FillForwardWithRoiTask_xb0e4
    SVP_NNIE_FillForwardWithRoiTask_xb2a4:
    /*0000b2a4*/ MOV         R9, R3 
    /*0000b2a8*/ MOV         R6, R1 
    /*0000b2ac*/ MOVW        R3, #1057                   @ 0x421 
    /*0000b2b0*/ B           SVP_NNIE_FillForwardWithRoiTask_xab68
    SVP_NNIE_FillForwardWithRoiTask_xb2b4:
    /*0000b2b4*/ LDR         R8, [SP, #152]              @ 0x98 
    /*0000b2b8*/ MOV         R3, #760                    @ 0x2f8 
    /*0000b2bc*/ MOV         R1, #51                     @ 0x33 
    /*0000b2c0*/ STR         R3, [SP] 
    /*0000b2c4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*0000b2c8*/ LDR         R3, [PC, #-1608]            @ 0x000000000000ac88 
    /*0000b2cc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*0000b2d0*/ MOV         R0, #3 
    /*0000b2d4*/ BL          0x000000000000B2D4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b2d8*/ STR         R8, [SP, #8] 
    /*0000b2dc*/ LDRD        R6, [R9, #16] 
    /*0000b2e0*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3c84] 
    /*0000b2e4*/ MOV         R1, #51                     @ 0x33 
    /*0000b2e8*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3c84] 
    /*0000b2ec*/ MOV         R0, #3 
    /*0000b2f0*/ MOVW        R4, #32780                  @ 0x800c 
    /*0000b2f4*/ STRD        R6, [SP] 
    /*0000b2f8*/ MOVT        R4, #41011                  @ 0xa033 
    /*0000b2fc*/ BL          0x000000000000B2FC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b300*/ B           SVP_NNIE_FillForwardWithRoiTask_xa058
    SVP_NNIE_FillForwardWithRoiTask_xb304:
    /*0000b304*/ MOVW        R3, #1157                   @ 0x485 
    /*0000b308*/ MOV         R1, #51                     @ 0x33 
    /*0000b30c*/ STR         R3, [SP] 
    /*0000b310*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*0000b314*/ LDR         R3, [PC, #-1680]            @ 0x000000000000ac8c 
    /*0000b318*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*0000b31c*/ MOV         R0, #3 
    /*0000b320*/ MOVW        R4, #32780                  @ 0x800c 
    /*0000b324*/ BL          0x000000000000B324          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b328*/ STR         FP, [SP, #8] 
    /*0000b32c*/ LDRD        R6, [R9, #16] 
    /*0000b330*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3c84] 
    /*0000b334*/ MOV         R1, #51                     @ 0x33 
    /*0000b338*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3c84] 
    /*0000b33c*/ MOV         R0, #3 
    /*0000b340*/ MOVT        R4, #41011                  @ 0xa033 
    /*0000b344*/ STRD        R6, [SP] 
    /*0000b348*/ BL          0x000000000000B348          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b34c*/ B           SVP_NNIE_FillForwardWithRoiTask_x9e4c
    SVP_NNIE_FillForwardWithRoiTask_xb350:
    /*0000b350*/ STR         R0, [SP, #224]              @ 0xe0 
    /*0000b354*/ B           SVP_NNIE_FillForwardWithRoiTask_x9778
    SVP_NNIE_FillForwardWithRoiTask_xb358:
    /*0000b358*/ LDR         R3, [SP, #228]              @ 0xe4 
    /*0000b35c*/ LDR         R2, [SP, #216]              @ 0xd8 
    /*0000b360*/ LDR         R1, [SP, #60]               @ 0x3c 
    /*0000b364*/ STR         R1, [R2, R3, LSL #2] 
    /*0000b368*/ ADD         R3, R3, #1 
    /*0000b36c*/ LDR         R2, [SP, #188]              @ 0xbc 
    /*0000b370*/ STR         R3, [SP, #228]              @ 0xe4 
    /*0000b374*/ ADD         R2, R2, #1 
    /*0000b378*/ STR         R2, [SP, #188]              @ 0xbc 
    /*0000b37c*/ B           SVP_NNIE_FillForwardWithRoiTask_xa25c
    SVP_NNIE_FillForwardWithRoiTask_xb380:
    /*0000b380*/ LDR         R3, [SP, #228]              @ 0xe4 
    /*0000b384*/ LDR         R2, [SP, #216]              @ 0xd8 
    /*0000b388*/ LDR         R1, [SP, #60]               @ 0x3c 
    /*0000b38c*/ STR         R1, [R2, R3, LSL #2] 
    /*0000b390*/ ADD         R3, R3, #1 
    /*0000b394*/ LDR         R2, [SP, #140]              @ 0x8c 
    /*0000b398*/ STR         R3, [SP, #228]              @ 0xe4 
    /*0000b39c*/ ADD         R2, R2, #1 
    /*0000b3a0*/ STR         R2, [SP, #140]              @ 0x8c 
    /*0000b3a4*/ B           SVP_NNIE_FillForwardWithRoiTask_xb040
    SVP_NNIE_FillForwardWithRoiTask_xb3a8:
    /*0000b3a8*/ LDR         R2, [SP, #140]              @ 0x8c 
    /*0000b3ac*/ LDR         R3, [SP, #196]              @ 0xc4 
    /*0000b3b0*/ LDR         R1, [SP, #32] 
    /*0000b3b4*/ LDR         R0, [SP, #152]              @ 0x98 
    /*0000b3b8*/ MUL         R3, R3, R2 
    /*0000b3bc*/ LDR         R2, [SP, #60]               @ 0x3c 
    /*0000b3c0*/ BIC         R2, R2, #3 
    /*0000b3c4*/ ADD         R2, R2, #4 
    /*0000b3c8*/ SUBS        R2, R2, R1 
    /*0000b3cc*/ LDR         R1, [SP, #220]              @ 0xdc 
    /*0000b3d0*/ STR         R3, [R1, R0, LSL #2] 
    /*0000b3d4*/ BEQ         SVP_NNIE_FillForwardWithRoiTask_xb404
    SVP_NNIE_FillForwardWithRoiTask_xb3d8:
    /*0000b3d8*/ LDR         R1, [SP, #96]               @ 0x60 
    /*0000b3dc*/ LSL         R2, R2, #2 
    /*0000b3e0*/ LDR         R3, [SP, #228]              @ 0xe4 
    /*0000b3e4*/ ADD         R3, R3, R1, LSL #2 
    /*0000b3e8*/ LDR         R1, [SP, #156]              @ 0x9c 
    /*0000b3ec*/ ADD         R0, R1, #16 
    /*0000b3f0*/ MOV         R1, #0 
    /*0000b3f4*/ ADD         R0, R0, R3, LSL #2 
    /*0000b3f8*/ LDR         R3, [SP, #108]              @ 0x6c 
    /*0000b3fc*/ ADD         R0, R3, R0 
    /*0000b400*/ BL          0x000000000000B400          @ memset [R_ARM_CALL: +0x0] 
    SVP_NNIE_FillForwardWithRoiTask_xb404:
    /*0000b404*/ LDR         R0, [SP, #72]               @ 0x48 
    /*0000b408*/ BL          0x000000000000B408          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    /*0000b40c*/ B           SVP_NNIE_FillForwardWithRoiTask_xaf24
    SVP_NNIE_FillForwardWithRoiTask_xb410:
    /*0000b410*/ MOVW        R3, #1567                   @ 0x61f 
    /*0000b414*/ MOV         R1, #51                     @ 0x33 
    /*0000b418*/ STR         R3, [SP] 
    /*0000b41c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*0000b420*/ LDR         R3, [PC, #-1944]            @ 0x000000000000ac90 
    /*0000b424*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*0000b428*/ MOV         R0, #3 
    /*0000b42c*/ LDR         FP, [SP, #24] 
    /*0000b430*/ BL          0x000000000000B430          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b434*/ LDR         R0, [SL, #40]               @ 0x28 
    /*0000b438*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3bb0] 
    /*0000b43c*/ MOV         R1, #51                     @ 0x33 
    /*0000b440*/ MOV         R3, FP 
    /*0000b444*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3bb0] 
    /*0000b448*/ STR         R0, [SP] 
    /*0000b44c*/ MOV         R0, #3 
    /*0000b450*/ MOVW        R4, #32771                  @ 0x8003 
    /*0000b454*/ BL          0x000000000000B454          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b458*/ MOVT        R4, #41011                  @ 0xa033 
    /*0000b45c*/ B           SVP_NNIE_FillForwardWithRoiTask_x9e80
    SVP_NNIE_FillForwardWithRoiTask_xb460:
    /*0000b460*/ MOVW        R3, #1402                   @ 0x57a 
    /*0000b464*/ MOV         R1, #51                     @ 0x33 
    /*0000b468*/ STR         R3, [SP] 
    /*0000b46c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b14] 
    /*0000b470*/ LDR         R3, [PC, #-2024]            @ 0x000000000000ac90 
    /*0000b474*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b14] 
    /*0000b478*/ MOV         R0, #3 
    /*0000b47c*/ MOVW        R4, #32780                  @ 0x800c 
    /*0000b480*/ BL          0x000000000000B480          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b484*/ LDR         R2, [SP, #316]              @ 0x13c 
    /*0000b488*/ LDR         R3, [SP, #316]              @ 0x13c 
    /*0000b48c*/ MOV         R1, #51                     @ 0x33 
    /*0000b490*/ MOV         R0, #3 
    /*0000b494*/ MOVT        R4, #41011                  @ 0xa033 
    /*0000b498*/ LDRD        R6, [R2, #48]               @ 0x30 
    /*0000b49c*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3c44] 
    /*0000b4a0*/ LDR         R3, [R3, #64]               @ 0x40 
    /*0000b4a4*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3c44] 
    /*0000b4a8*/ STRD        R6, [SP] 
    /*0000b4ac*/ STR         R3, [SP, #8] 
    /*0000b4b0*/ BL          0x000000000000B4B0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b4b4*/ B           SVP_NNIE_FillForwardWithRoiTask_x9eb8
    SVP_NNIE_FillForwardWithRoiTask_xb4b8:
    /*0000b4b8*/ LDR         R2, [SP, #152]              @ 0x98 
    /*0000b4bc*/ LDR         R3, [SP, #220]              @ 0xdc 
    /*0000b4c0*/ LDR         R1, [SP, #32] 
    /*0000b4c4*/ STR         R1, [R3, R2, LSL #2] 
    /*0000b4c8*/ MOV         R2, #4 
    /*0000b4cc*/ B           SVP_NNIE_FillForwardWithRoiTask_xb3d8
    SVP_NNIE_FillForwardWithRoiTask_xb4d0:
    /*0000b4d0*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*0000b4d4*/ MOV         SL, R2 
    /*0000b4d8*/ STR         R2, [SP, #228]              @ 0xe4 
    /*0000b4dc*/ LSL         R3, R3, #12 
    /*0000b4e0*/ STR         R3, [SP, #200]              @ 0xc8 
    /*0000b4e4*/ MOV         R3, R2 
    /*0000b4e8*/ B           SVP_NNIE_FillForwardWithRoiTask_xa17c
FUNC_END SVP_NNIE_FillForwardWithRoiTask


FUNC_BEGIN SVP_NNIE_DRV_Init
    /*0000b4ec*/ PUSH        {R4, LR} 
    /*0000b4f0*/ SUB         SP, SP, #8 
    /*0000b4f4*/ BL          0x000000000000B4F4          @ SVP_NNIE_HAL_Init [R_ARM_CALL: .text+0xba48] 
    /*0000b4f8*/ SUBS        R4, R0, #0 
    /*0000b4fc*/ BEQ         SVP_NNIE_DRV_Init_xb53c
    /*0000b500*/ MOV         R2, #66                     @ 0x42 
    /*0000b504*/ MOV         R1, #51                     @ 0x33 
    /*0000b508*/ STR         R2, [SP] 
    /*0000b50c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x344] 
    /*0000b510*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3e38] 
    /*0000b514*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x344] 
    /*0000b518*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3e38] 
    /*0000b51c*/ MOV         R0, #3 
    /*0000b520*/ BL          0x000000000000B520          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b524*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3e54] 
    /*0000b528*/ MOV         R3, R4 
    /*0000b52c*/ MOV         R1, #51                     @ 0x33 
    /*0000b530*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3e54] 
    /*0000b534*/ MOV         R0, #3 
    /*0000b538*/ BL          0x000000000000B538          @ HI_LOG [R_ARM_CALL: +0x0] 
    SVP_NNIE_DRV_Init_xb53c:
    /*0000b53c*/ MOV         R0, R4 
    /*0000b540*/ ADD         SP, SP, #8 
    /*0000b544*/ POP         {R4, PC} 
FUNC_END SVP_NNIE_DRV_Init


FUNC_BEGIN SVP_NNIE_DRV_DeInit
    SVP_NNIE_DRV_DeInit_xb548:
    /*0000b548*/ B           SVP_NNIE_DRV_DeInit_xb548   @ SVP_NNIE_HAL_DeInit [R_ARM_JUMP24: .text+0xbb24] 
FUNC_END SVP_NNIE_DRV_DeInit


FUNC_BEGIN SVP_NNIE_DRV_EnableSysClk
    /*0000b54c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000b550*/ CMP         R0, #0 
    /*0000b554*/ SUB         SP, SP, #28 
    /*0000b558*/ MOV         R3, #0 
    /*0000b55c*/ MOV         R2, #51                     @ 0x33 
    /*0000b560*/ STR         R1, [SP, #4] 
    /*0000b564*/ STR         R2, [SP, #12] 
    /*0000b568*/ STR         R3, [SP, #16] 
    /*0000b56c*/ STR         R3, [SP, #20] 
    /*0000b570*/ BEQ         SVP_NNIE_DRV_EnableSysClk_xb590
    /*0000b574*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3e7c] 
    /*0000b578*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3ea0] 
    /*0000b57c*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3e7c] 
    /*0000b580*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3ea0] 
    /*0000b584*/ MOV         R2, #130                    @ 0x82 
    /*0000b588*/ LDR         R1, [PC, #36]               @ 0x000000000000b5b4 
    /*0000b58c*/ BL          0x000000000000B58C          @ osal_panic [R_ARM_CALL: +0x0] 
    SVP_NNIE_DRV_EnableSysClk_xb590:
    /*0000b590*/ MOV         R0, #2 
    /*0000b594*/ BL          0x000000000000B594          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000b598*/ ADD         R2, SP, #4 
    /*0000b59c*/ MOV         R1, #177                    @ 0xb1 
    /*0000b5a0*/ LDR         R3, [R0, #20] 
    /*0000b5a4*/ ADD         R0, SP, #12 
    /*0000b5a8*/ BLX         R3 
    /*0000b5ac*/ ADD         SP, SP, #28 
    /*0000b5b0*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*0000b5b4*/ ANDEQ       R0, R0, R8, ASR R3 
FUNC_END SVP_NNIE_DRV_EnableSysClk


FUNC_BEGIN SVP_NNIE_DRV_SetReset
    /*0000b5b8*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000b5bc*/ CMP         R0, #0 
    /*0000b5c0*/ SUB         SP, SP, #28 
    /*0000b5c4*/ MOV         R3, #0 
    /*0000b5c8*/ MOV         R2, #51                     @ 0x33 
    /*0000b5cc*/ STR         R1, [SP, #4] 
    /*0000b5d0*/ STR         R2, [SP, #12] 
    /*0000b5d4*/ STR         R3, [SP, #16] 
    /*0000b5d8*/ STR         R3, [SP, #20] 
    /*0000b5dc*/ BEQ         SVP_NNIE_DRV_SetReset_xb5fc
    /*0000b5e0*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3e7c] 
    /*0000b5e4*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3ea0] 
    /*0000b5e8*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3e7c] 
    /*0000b5ec*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3ea0] 
    /*0000b5f0*/ MOV         R2, #164                    @ 0xa4 
    /*0000b5f4*/ LDR         R1, [PC, #36]               @ 0x000000000000b620 
    /*0000b5f8*/ BL          0x000000000000B5F8          @ osal_panic [R_ARM_CALL: +0x0] 
    SVP_NNIE_DRV_SetReset_xb5fc:
    /*0000b5fc*/ MOV         R0, #2 
    /*0000b600*/ BL          0x000000000000B600          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000b604*/ ADD         R2, SP, #4 
    /*0000b608*/ MOV         R1, #176                    @ 0xb0 
    /*0000b60c*/ LDR         R3, [R0, #20] 
    /*0000b610*/ ADD         R0, SP, #12 
    /*0000b614*/ BLX         R3 
    /*0000b618*/ ADD         SP, SP, #28 
    /*0000b61c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*0000b620*/ ANDEQ       R0, R0, R4, ROR R3 
FUNC_END SVP_NNIE_DRV_SetReset


FUNC_BEGIN SVP_NNIE_DRV_SetClkFreq
    /*0000b624*/ BX          LR 
FUNC_END SVP_NNIE_DRV_SetClkFreq


FUNC_BEGIN SVP_NNIE_DRV_ConfigClkFreq
    /*0000b628*/ BX          LR 
FUNC_END SVP_NNIE_DRV_ConfigClkFreq


FUNC_BEGIN SVP_NNIE_DRV_CheckClkFreq
    /*0000b62c*/ BX          LR 
FUNC_END SVP_NNIE_DRV_CheckClkFreq


FUNC_BEGIN SVP_NNIE_DRV_Start
    /*0000b630*/ PUSH        {R4, LR} 
    /*0000b634*/ MOV         R4, R0 
    /*0000b638*/ BL          0x000000000000B638          @ SVP_NNIE_HAL_WriteTaskAddr [R_ARM_CALL: .text+0xbb94] 
    /*0000b63c*/ BL          0x000000000000B63C          @ osal_dmb [R_ARM_CALL: +0x0] 
    /*0000b640*/ MOV         R0, R4 
    /*0000b644*/ POP         {R4, LR} 
    SVP_NNIE_DRV_Start_xb648:
    /*0000b648*/ B           SVP_NNIE_DRV_Start_xb648    @ SVP_NNIE_HAL_Start [R_ARM_JUMP24: .text+0xbbb0] 
FUNC_END SVP_NNIE_DRV_Start


FUNC_BEGIN SVP_NNIE_DRV_GetIntStatus
    SVP_NNIE_DRV_GetIntStatus_xb64c:
    /*0000b64c*/ B           SVP_NNIE_DRV_GetIntStatus_xb64c@ SVP_NNIE_HAL_GetIntStatus [R_ARM_JUMP24: .text+0xbbd0] 
FUNC_END SVP_NNIE_DRV_GetIntStatus


FUNC_BEGIN SVP_NNIE_DRV_ClearInt
    SVP_NNIE_DRV_ClearInt_xb650:
    /*0000b650*/ B           SVP_NNIE_DRV_ClearInt_xb650 @ SVP_NNIE_HAL_ClearInt [R_ARM_JUMP24: .text+0xbbe8] 
FUNC_END SVP_NNIE_DRV_ClearInt


FUNC_BEGIN SVP_NNIE_DRV_IsTimeoutInt
    /*0000b654*/ ANDS        R0, R0, #2 
    /*0000b658*/ BXEQ        LR 
    /*0000b65c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000b660*/ SUB         SP, SP, #12 
    /*0000b664*/ MOVW        R3, #358                    @ 0x166 
    /*0000b668*/ MOV         R1, #51                     @ 0x33 
    /*0000b66c*/ STR         R3, [SP] 
    /*0000b670*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3e38] 
    /*0000b674*/ LDR         R3, [PC, #40]               @ 0x000000000000b6a4 
    /*0000b678*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3e38] 
    /*0000b67c*/ MOV         R0, #0 
    /*0000b680*/ BL          0x000000000000B680          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b684*/ MOV         R1, #51                     @ 0x33 
    /*0000b688*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3ee0] 
    /*0000b68c*/ MOV         R0, #0 
    /*0000b690*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3ee0] 
    /*0000b694*/ BL          0x000000000000B694          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b698*/ MOV         R0, #1 
    /*0000b69c*/ ADD         SP, SP, #12 
    /*0000b6a0*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*0000b6a4*/ ANDEQ       R0, R0, IP, LSL #7 
FUNC_END SVP_NNIE_DRV_IsTimeoutInt


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrInt
    /*0000b6a8*/ ANDS        R0, R0, #4 
    /*0000b6ac*/ BXEQ        LR 
    /*0000b6b0*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000b6b4*/ SUB         SP, SP, #12 
    /*0000b6b8*/ MOVW        R3, #389                    @ 0x185 
    /*0000b6bc*/ MOV         R1, #51                     @ 0x33 
    /*0000b6c0*/ STR         R3, [SP] 
    /*0000b6c4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3e38] 
    /*0000b6c8*/ LDR         R3, [PC, #40]               @ 0x000000000000b6f8 
    /*0000b6cc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3e38] 
    /*0000b6d0*/ MOV         R0, #0 
    /*0000b6d4*/ BL          0x000000000000B6D4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b6d8*/ MOV         R1, #51                     @ 0x33 
    /*0000b6dc*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3f04] 
    /*0000b6e0*/ MOV         R0, #0 
    /*0000b6e4*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3f04] 
    /*0000b6e8*/ BL          0x000000000000B6E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b6ec*/ MOV         R0, #1 
    /*0000b6f0*/ ADD         SP, SP, #12 
    /*0000b6f4*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*0000b6f8*/ ANDEQ       R0, R0, R8, LSR #7 
FUNC_END SVP_NNIE_DRV_IsCfgErrInt


FUNC_BEGIN SVP_NNIE_DRV_IsFinishInt
    /*0000b6fc*/ AND         R0, R0, #1 
    /*0000b700*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsFinishInt


FUNC_BEGIN SVP_NNIE_DRV_GetTaskId
    SVP_NNIE_DRV_GetTaskId_xb704:
    /*0000b704*/ B           SVP_NNIE_DRV_GetTaskId_xb704@ SVP_NNIE_HAL_GetTaskId [R_ARM_JUMP24: .text+0xbc20] 
FUNC_END SVP_NNIE_DRV_GetTaskId


FUNC_BEGIN SVP_NNIE_DRV_EnInt
    SVP_NNIE_DRV_EnInt_xb708:
    /*0000b708*/ B           SVP_NNIE_DRV_EnInt_xb708    @ SVP_NNIE_HAL_EnInt [R_ARM_JUMP24: .text+0xbc38] 
FUNC_END SVP_NNIE_DRV_EnInt


FUNC_BEGIN SVP_NNIE_DRV_SetTimeOut
    SVP_NNIE_DRV_SetTimeOut_xb70c:
    /*0000b70c*/ B           SVP_NNIE_DRV_SetTimeOut_xb70c@ SVP_NNIE_HAL_SetTimeOut [R_ARM_JUMP24: .text+0xbc70] 
FUNC_END SVP_NNIE_DRV_SetTimeOut


FUNC_BEGIN SVP_NNIE_DRV_EnClkGt
    SVP_NNIE_DRV_EnClkGt_xb710:
    /*0000b710*/ B           SVP_NNIE_DRV_EnClkGt_xb710  @ SVP_NNIE_HAL_EnClkGt [R_ARM_JUMP24: .text+0xbc8c] 
FUNC_END SVP_NNIE_DRV_EnClkGt


FUNC_BEGIN SVP_NNIE_DRV_SetMemSpeed
    SVP_NNIE_DRV_SetMemSpeed_xb714:
    /*0000b714*/ B           SVP_NNIE_DRV_SetMemSpeed_xb714@ SVP_NNIE_HAL_SetMemSpeed [R_ARM_JUMP24: .text+0xbcac] 
FUNC_END SVP_NNIE_DRV_SetMemSpeed


FUNC_BEGIN SVP_NNIE_DRV_GetCfgErrInfo
    SVP_NNIE_DRV_GetCfgErrInfo_xb718:
    /*0000b718*/ B           SVP_NNIE_DRV_GetCfgErrInfo_xb718@ SVP_NNIE_HAL_GetCfgErrInfo [R_ARM_JUMP24: .text+0xbcb0] 
FUNC_END SVP_NNIE_DRV_GetCfgErrInfo


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrEmarInstrLen
    /*0000b71c*/ AND         R0, R0, #1 
    /*0000b720*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrEmarInstrLen


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrGoto
    /*0000b724*/ UBFX        R0, R0, #1, #1 
    /*0000b728*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrGoto


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrEmarRtnContinue
    /*0000b72c*/ UBFX        R0, R0, #2, #1 
    /*0000b730*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrEmarRtnContinue


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrEmarInstrNull
    /*0000b734*/ UBFX        R0, R0, #3, #1 
    /*0000b738*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrEmarInstrNull


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrDataBlockLenIsZeroErr
    /*0000b73c*/ UBFX        R0, R0, #8, #1 
    /*0000b740*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrDataBlockLenIsZeroErr


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrDataBlockLenIsTooLongErr
    /*0000b744*/ UBFX        R0, R0, #9, #1 
    /*0000b748*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrDataBlockLenIsTooLongErr


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrStoreNotMatch
    /*0000b74c*/ UBFX        R0, R0, #12, #1 
    /*0000b750*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrStoreNotMatch


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrStoreBlockIsNull
    /*0000b754*/ UBFX        R0, R0, #13, #1 
    /*0000b758*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrStoreBlockIsNull


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrCompressBlockIsNull
    /*0000b75c*/ TST         R0, #12288                  @ 0x3000 
    /*0000b760*/ MOVNE       R0, #1 
    /*0000b764*/ MOVEQ       R0, #0 
    /*0000b768*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrCompressBlockIsNull


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrCheckIsErr
    /*0000b76c*/ UBFX        R0, R0, #14, #1 
    /*0000b770*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrCheckIsErr


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrCompressDataTooSmall
    /*0000b774*/ TST         R0, #20480                  @ 0x5000 
    /*0000b778*/ MOVNE       R0, #1 
    /*0000b77c*/ MOVEQ       R0, #0 
    /*0000b780*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrCompressDataTooSmall


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrCompressDataLenNotMacth
    /*0000b784*/ TST         R0, #24576                  @ 0x6000 
    /*0000b788*/ MOVNE       R0, #1 
    /*0000b78c*/ MOVEQ       R0, #0 
    /*0000b790*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrCompressDataLenNotMacth


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrCompressDataLenTooLong
    /*0000b794*/ TST         R0, #28672                  @ 0x7000 
    /*0000b798*/ MOVNE       R0, #1 
    /*0000b79c*/ MOVEQ       R0, #0 
    /*0000b7a0*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrCompressDataLenTooLong


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrDscpCheckErr
    /*0000b7a4*/ UBFX        R0, R0, #28, #1 
    /*0000b7a8*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrDscpCheckErr


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrDscpFunctionCheckErr
    /*0000b7ac*/ UBFX        R0, R0, #29, #1 
    /*0000b7b0*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrDscpFunctionCheckErr


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrAddrErr
    /*0000b7b4*/ UBFX        R0, R0, #30, #1 
    /*0000b7b8*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrAddrErr


FUNC_BEGIN SVP_NNIE_DRV_IsCfgErrNodeErr
    /*0000b7bc*/ LSR         R0, R0, #31 
    /*0000b7c0*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsCfgErrNodeErr


FUNC_BEGIN SVP_NNIE_DRV_GetCycle
    SVP_NNIE_DRV_GetCycle_xb7c4:
    /*0000b7c4*/ B           SVP_NNIE_DRV_GetCycle_xb7c4 @ SVP_NNIE_HAL_GetCycle [R_ARM_JUMP24: .text+0xbcc8] 
FUNC_END SVP_NNIE_DRV_GetCycle


FUNC_BEGIN SVP_NNIE_DRV_DisableCheckSum
    SVP_NNIE_DRV_DisableCheckSum_xb7c8:
    /*0000b7c8*/ B           SVP_NNIE_DRV_DisableCheckSum_xb7c8@ SVP_NNIE_HAL_DisableCheckSum [R_ARM_JUMP24: .text+0xbcf0] 
FUNC_END SVP_NNIE_DRV_DisableCheckSum


FUNC_BEGIN SVP_NNIE_DRV_GetGdcClkState
    SVP_NNIE_DRV_GetGdcClkState_xb7cc:
    /*0000b7cc*/ B           SVP_NNIE_DRV_GetGdcClkState_xb7cc@ SVP_NNIE_HAL_GetGdcClkState [R_ARM_JUMP24: .text+0xb884] 
FUNC_END SVP_NNIE_DRV_GetGdcClkState


FUNC_BEGIN SVP_NNIE_DRV_SetGdcClk
    SVP_NNIE_DRV_SetGdcClk_xb7d0:
    /*0000b7d0*/ B           SVP_NNIE_DRV_SetGdcClk_xb7d0@ SVP_NNIE_HAL_SetGdcClk [R_ARM_JUMP24: .text+0xb8c4] 
FUNC_END SVP_NNIE_DRV_SetGdcClk


FUNC_BEGIN SVP_NNIE_DRV_GetNnieClkState
    SVP_NNIE_DRV_GetNnieClkState_xb7d4:
    /*0000b7d4*/ B           SVP_NNIE_DRV_GetNnieClkState_xb7d4@ SVP_NNIE_HAL_GetNnieClkState [R_ARM_JUMP24: .text+0xb904] 
FUNC_END SVP_NNIE_DRV_GetNnieClkState


FUNC_BEGIN SVP_NNIE_DRV_SetNnieClk
    SVP_NNIE_DRV_SetNnieClk_xb7d8:
    /*0000b7d8*/ B           SVP_NNIE_DRV_SetNnieClk_xb7d8@ SVP_NNIE_HAL_SetNnieClk [R_ARM_JUMP24: .text+0xb944] 
FUNC_END SVP_NNIE_DRV_SetNnieClk


FUNC_BEGIN SVP_NNIE_DRV_EnableRam
    SVP_NNIE_DRV_EnableRam_xb7dc:
    /*0000b7dc*/ B           SVP_NNIE_DRV_EnableRam_xb7dc@ SVP_NNIE_HAL_EnableRam [R_ARM_JUMP24: .text+0xb984] 
FUNC_END SVP_NNIE_DRV_EnableRam


FUNC_BEGIN SVP_NNIE_DRV_SelectRam
    /*0000b7e0*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000b7e4*/ SUB         SP, SP, #20 
    /*0000b7e8*/ MOV         R3, #0 
    /*0000b7ec*/ MOV         R0, SP 
    /*0000b7f0*/ STR         R3, [SP] 
    /*0000b7f4*/ STR         R3, [SP, #4] 
    /*0000b7f8*/ STR         R3, [SP, #8] 
    /*0000b7fc*/ STR         R3, [SP, #12] 
    /*0000b800*/ BL          0x000000000000B800          @ SVP_NNIE_HAL_GetGdcClkState [R_ARM_CALL: .text+0xb884] 
    /*0000b804*/ LDR         R3, [SP] 
    /*0000b808*/ CMP         R3, #0 
    /*0000b80c*/ BEQ         SVP_NNIE_DRV_SelectRam_xb85c
    SVP_NNIE_DRV_SelectRam_xb810:
    /*0000b810*/ ADD         R0, SP, #8 
    /*0000b814*/ BL          0x000000000000B814          @ SVP_NNIE_HAL_GetNnieClkState [R_ARM_CALL: .text+0xb904] 
    /*0000b818*/ LDR         R3, [SP, #8] 
    /*0000b81c*/ CMP         R3, #0 
    /*0000b820*/ BEQ         SVP_NNIE_DRV_SelectRam_xb848
    SVP_NNIE_DRV_SelectRam_xb824:
    /*0000b824*/ BL          0x000000000000B824          @ SVP_NNIE_HAL_EnableRam [R_ARM_CALL: .text+0xb984] 
    /*0000b828*/ LDR         R3, [SP] 
    /*0000b82c*/ CMP         R3, #0 
    /*0000b830*/ BNE         SVP_NNIE_DRV_SelectRam_xb840
    /*0000b834*/ ADD         R0, SP, #16 
    /*0000b838*/ STR         R3, [R0, #-12]! 
    /*0000b83c*/ BL          0x000000000000B83C          @ SVP_NNIE_HAL_SetGdcClk [R_ARM_CALL: .text+0xb8c4] 
    SVP_NNIE_DRV_SelectRam_xb840:
    /*0000b840*/ ADD         SP, SP, #20 
    /*0000b844*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    SVP_NNIE_DRV_SelectRam_xb848:
    /*0000b848*/ ADD         R0, SP, #16 
    /*0000b84c*/ MOV         R3, #1 
    /*0000b850*/ STR         R3, [R0, #-4]! 
    /*0000b854*/ BL          0x000000000000B854          @ SVP_NNIE_HAL_SetNnieClk [R_ARM_CALL: .text+0xb944] 
    /*0000b858*/ B           SVP_NNIE_DRV_SelectRam_xb824
    SVP_NNIE_DRV_SelectRam_xb85c:
    /*0000b85c*/ ADD         R0, SP, #16 
    /*0000b860*/ MOV         R3, #1 
    /*0000b864*/ STR         R3, [R0, #-12]! 
    /*0000b868*/ BL          0x000000000000B868          @ SVP_NNIE_HAL_SetGdcClk [R_ARM_CALL: .text+0xb8c4] 
    /*0000b86c*/ B           SVP_NNIE_DRV_SelectRam_xb810
FUNC_END SVP_NNIE_DRV_SelectRam


FUNC_BEGIN SVP_NNIE_DRV_ConfigRam
    /*0000b870*/ BX          LR 
FUNC_END SVP_NNIE_DRV_ConfigRam


FUNC_BEGIN SVP_NNIE_DRV_PrepareNnie
    /*0000b874*/ MOV         R0, #0 
    /*0000b878*/ BX          LR 
FUNC_END SVP_NNIE_DRV_PrepareNnie


FUNC_BEGIN SVP_NNIE_DRV_IsNnieReady
    /*0000b87c*/ MOV         R0, #1 
    /*0000b880*/ BX          LR 
FUNC_END SVP_NNIE_DRV_IsNnieReady


FUNC_BEGIN SVP_NNIE_HAL_GetGdcClkState
    /*0000b884*/ PUSH        {R4, LR} 
    /*0000b888*/ SUB         SP, SP, #16 
    /*0000b88c*/ MOV         R4, R0 
    /*0000b890*/ MOV         R0, #2 
    /*0000b894*/ MOV         R3, #0 
    /*0000b898*/ MOV         R2, #51                     @ 0x33 
    /*0000b89c*/ STR         R3, [SP, #12] 
    /*0000b8a0*/ STMIB       SP, {R2, R3} 
    /*0000b8a4*/ BL          0x000000000000B8A4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000b8a8*/ MOV         R2, R4 
    /*0000b8ac*/ MOV         R1, #204                    @ 0xcc 
    /*0000b8b0*/ LDR         R3, [R0, #20] 
    /*0000b8b4*/ ADD         R0, SP, #4 
    /*0000b8b8*/ BLX         R3 
    /*0000b8bc*/ ADD         SP, SP, #16 
    /*0000b8c0*/ POP         {R4, PC} 
FUNC_END SVP_NNIE_HAL_GetGdcClkState


FUNC_BEGIN SVP_NNIE_HAL_SetGdcClk
    /*0000b8c4*/ PUSH        {R4, LR} 
    /*0000b8c8*/ SUB         SP, SP, #16 
    /*0000b8cc*/ MOV         R4, R0 
    /*0000b8d0*/ MOV         R0, #2 
    /*0000b8d4*/ MOV         R3, #0 
    /*0000b8d8*/ MOV         R2, #51                     @ 0x33 
    /*0000b8dc*/ STR         R3, [SP, #12] 
    /*0000b8e0*/ STMIB       SP, {R2, R3} 
    /*0000b8e4*/ BL          0x000000000000B8E4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000b8e8*/ MOV         R2, R4 
    /*0000b8ec*/ MOV         R1, #203                    @ 0xcb 
    /*0000b8f0*/ LDR         R3, [R0, #20] 
    /*0000b8f4*/ ADD         R0, SP, #4 
    /*0000b8f8*/ BLX         R3 
    /*0000b8fc*/ ADD         SP, SP, #16 
    /*0000b900*/ POP         {R4, PC} 
FUNC_END SVP_NNIE_HAL_SetGdcClk


FUNC_BEGIN SVP_NNIE_HAL_GetNnieClkState
    /*0000b904*/ PUSH        {R4, LR} 
    /*0000b908*/ SUB         SP, SP, #16 
    /*0000b90c*/ MOV         R4, R0 
    /*0000b910*/ MOV         R0, #2 
    /*0000b914*/ MOV         R3, #0 
    /*0000b918*/ MOV         R2, #51                     @ 0x33 
    /*0000b91c*/ STR         R3, [SP, #12] 
    /*0000b920*/ STMIB       SP, {R2, R3} 
    /*0000b924*/ BL          0x000000000000B924          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000b928*/ MOV         R2, R4 
    /*0000b92c*/ MOV         R1, #178                    @ 0xb2 
    /*0000b930*/ LDR         R3, [R0, #20] 
    /*0000b934*/ ADD         R0, SP, #4 
    /*0000b938*/ BLX         R3 
    /*0000b93c*/ ADD         SP, SP, #16 
    /*0000b940*/ POP         {R4, PC} 
FUNC_END SVP_NNIE_HAL_GetNnieClkState


FUNC_BEGIN SVP_NNIE_HAL_SetNnieClk
    /*0000b944*/ PUSH        {R4, LR} 
    /*0000b948*/ SUB         SP, SP, #16 
    /*0000b94c*/ MOV         R4, R0 
    /*0000b950*/ MOV         R0, #2 
    /*0000b954*/ MOV         R3, #0 
    /*0000b958*/ MOV         R2, #51                     @ 0x33 
    /*0000b95c*/ STR         R3, [SP, #12] 
    /*0000b960*/ STMIB       SP, {R2, R3} 
    /*0000b964*/ BL          0x000000000000B964          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000b968*/ MOV         R2, R4 
    /*0000b96c*/ MOV         R1, #177                    @ 0xb1 
    /*0000b970*/ LDR         R3, [R0, #20] 
    /*0000b974*/ ADD         R0, SP, #4 
    /*0000b978*/ BLX         R3 
    /*0000b97c*/ ADD         SP, SP, #16 
    /*0000b980*/ POP         {R4, PC} 
FUNC_END SVP_NNIE_HAL_SetNnieClk


FUNC_BEGIN SVP_NNIE_HAL_EnableRam
    /*0000b984*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000b988*/ SUB         SP, SP, #20 
    /*0000b98c*/ MOV         R0, #2 
    /*0000b990*/ MOV         R3, #0 
    /*0000b994*/ MOV         R2, #51                     @ 0x33 
    /*0000b998*/ STR         R3, [SP] 
    /*0000b99c*/ STR         R3, [SP, #8] 
    /*0000b9a0*/ STR         R3, [SP, #12] 
    /*0000b9a4*/ STR         R2, [SP, #4] 
    /*0000b9a8*/ BL          0x000000000000B9A8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000b9ac*/ MOV         R2, SP 
    /*0000b9b0*/ MOV         R1, #205                    @ 0xcd 
    /*0000b9b4*/ LDR         R3, [R0, #20] 
    /*0000b9b8*/ ADD         R0, SP, #4 
    /*0000b9bc*/ BLX         R3 
    /*0000b9c0*/ ADD         SP, SP, #20 
    /*0000b9c4*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END SVP_NNIE_HAL_EnableRam


FUNC_BEGIN SVP_NNIE_HAL_GetMutexState
    /*0000b9c8*/ PUSH        {R4, LR} 
    /*0000b9cc*/ SUB         SP, SP, #16 
    /*0000b9d0*/ MOV         R4, R0 
    /*0000b9d4*/ MOV         R0, #2 
    /*0000b9d8*/ MOV         R3, #0 
    /*0000b9dc*/ MOV         R2, #51                     @ 0x33 
    /*0000b9e0*/ STR         R3, [SP, #12] 
    /*0000b9e4*/ STMIB       SP, {R2, R3} 
    /*0000b9e8*/ BL          0x000000000000B9E8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000b9ec*/ MOV         R2, R4 
    /*0000b9f0*/ MOV         R1, #218                    @ 0xda 
    /*0000b9f4*/ LDR         R3, [R0, #20] 
    /*0000b9f8*/ ADD         R0, SP, #4 
    /*0000b9fc*/ BLX         R3 
    /*0000ba00*/ ADD         SP, SP, #16 
    /*0000ba04*/ POP         {R4, PC} 
FUNC_END SVP_NNIE_HAL_GetMutexState


FUNC_BEGIN SVP_NNIE_HAL_GetGdcMutexState
    /*0000ba08*/ PUSH        {R4, LR} 
    /*0000ba0c*/ SUB         SP, SP, #16 
    /*0000ba10*/ MOV         R4, R0 
    /*0000ba14*/ MOV         R0, #2 
    /*0000ba18*/ MOV         R3, #0 
    /*0000ba1c*/ MOV         R2, #51                     @ 0x33 
    /*0000ba20*/ STR         R3, [SP, #12] 
    /*0000ba24*/ STMIB       SP, {R2, R3} 
    /*0000ba28*/ BL          0x000000000000BA28          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000ba2c*/ MOV         R2, R4 
    /*0000ba30*/ MOV         R1, #219                    @ 0xdb 
    /*0000ba34*/ LDR         R3, [R0, #20] 
    /*0000ba38*/ ADD         R0, SP, #4 
    /*0000ba3c*/ BLX         R3 
    /*0000ba40*/ ADD         SP, SP, #16 
    /*0000ba44*/ POP         {R4, PC} 
FUNC_END SVP_NNIE_HAL_GetGdcMutexState


FUNC_BEGIN SVP_NNIE_HAL_Init
    /*0000ba48*/ PUSH        {R4, R5, LR} 
    /*0000ba4c*/ SUBS        R5, R0, #0 
    /*0000ba50*/ SUB         SP, SP, #12 
    /*0000ba54*/ BNE         SVP_NNIE_HAL_Init_xba80
    SVP_NNIE_HAL_Init_xba58:
    /*0000ba58*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000ba5c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000ba60*/ LDR         R2, [R3, R5, LSL #2] 
    /*0000ba64*/ CMP         R2, #0 
    /*0000ba68*/ BEQ         SVP_NNIE_HAL_Init_xbaa4
    /*0000ba6c*/ ADD         R3, R3, R5, LSL #2 
    /*0000ba70*/ MOV         R0, #0 
    /*0000ba74*/ STR         R2, [R3, #4] 
    SVP_NNIE_HAL_Init_xba78:
    /*0000ba78*/ ADD         SP, SP, #12 
    /*0000ba7c*/ POP         {R4, R5, PC} 
    SVP_NNIE_HAL_Init_xba80:
    /*0000ba80*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3f40] 
    /*0000ba84*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x3c4] 
    /*0000ba88*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3f40] 
    /*0000ba8c*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x3c4] 
    /*0000ba90*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3f64] 
    /*0000ba94*/ MOVW        R2, #358                    @ 0x166 
    /*0000ba98*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3f64] 
    /*0000ba9c*/ BL          0x000000000000BA9C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000baa0*/ B           SVP_NNIE_HAL_Init_xba58
    SVP_NNIE_HAL_Init_xbaa4:
    /*0000baa4*/ ADD         R4, R3, R5, LSL #2 
    /*0000baa8*/ LDR         R3, [R4, #4] 
    /*0000baac*/ CMP         R3, #0 
    /*0000bab0*/ BNE         SVP_NNIE_HAL_Init_xbad0
    /*0000bab4*/ MOV         R0, #0 
    /*0000bab8*/ MOV         R1, #65536                  @ 0x10000 
    /*0000babc*/ MOVT        R0, #4368                   @ 0x1110 
    /*0000bac0*/ BL          0x000000000000BAC0          @ osal_ioremap [R_ARM_CALL: +0x0] 
    /*0000bac4*/ CMP         R0, #0 
    /*0000bac8*/ STR         R0, [R4, #4] 
    /*0000bacc*/ BEQ         SVP_NNIE_HAL_Init_xbadc
    SVP_NNIE_HAL_Init_xbad0:
    /*0000bad0*/ MOV         R0, #0 
    /*0000bad4*/ ADD         SP, SP, #12 
    /*0000bad8*/ POP         {R4, R5, PC} 
    SVP_NNIE_HAL_Init_xbadc:
    /*0000badc*/ MOVW        R2, #367                    @ 0x16f 
    /*0000bae0*/ MOV         R1, #51                     @ 0x33 
    /*0000bae4*/ STR         R2, [SP] 
    /*0000bae8*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x3c4] 
    /*0000baec*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3fa4] 
    /*0000baf0*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x3c4] 
    /*0000baf4*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3fa4] 
    /*0000baf8*/ MOV         R0, #3 
    /*0000bafc*/ BL          0x000000000000BAFC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000bb00*/ MOV         R3, R5 
    /*0000bb04*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3fc0] 
    /*0000bb08*/ MOV         R1, #51                     @ 0x33 
    /*0000bb0c*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3fc0] 
    /*0000bb10*/ MOV         R0, #3 
    /*0000bb14*/ BL          0x000000000000BB14          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000bb18*/ MOVW        R0, #32780                  @ 0x800c 
    /*0000bb1c*/ MOVT        R0, #41011                  @ 0xa033 
    /*0000bb20*/ B           SVP_NNIE_HAL_Init_xba78
FUNC_END SVP_NNIE_HAL_Init


FUNC_BEGIN SVP_NNIE_HAL_DeInit
    /*0000bb24*/ PUSH        {R4, R5, R6, LR} 
    /*0000bb28*/ SUBS        R5, R0, #0 
    /*0000bb2c*/ BNE         SVP_NNIE_HAL_DeInit_xbb58
    SVP_NNIE_HAL_DeInit_xbb30:
    /*0000bb30*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bb34*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bb38*/ LDR         R3, [R4, R5, LSL #2] 
    /*0000bb3c*/ LSL         R5, R5, #2 
    /*0000bb40*/ CMP         R3, #0 
    /*0000bb44*/ BEQ         SVP_NNIE_HAL_DeInit_xbb78
    SVP_NNIE_HAL_DeInit_xbb48:
    /*0000bb48*/ ADD         R4, R4, R5 
    /*0000bb4c*/ MOV         R3, #0 
    /*0000bb50*/ STR         R3, [R4, #4] 
    /*0000bb54*/ POP         {R4, R5, R6, PC} 
    SVP_NNIE_HAL_DeInit_xbb58:
    /*0000bb58*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3f40] 
    /*0000bb5c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3f64] 
    /*0000bb60*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3f40] 
    /*0000bb64*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3f64] 
    /*0000bb68*/ MOV         R2, #396                    @ 0x18c 
    /*0000bb6c*/ LDR         R1, [PC, #28]               @ 0x000000000000bb90 
    /*0000bb70*/ BL          0x000000000000BB70          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000bb74*/ B           SVP_NNIE_HAL_DeInit_xbb30
    SVP_NNIE_HAL_DeInit_xbb78:
    /*0000bb78*/ ADD         R3, R4, R5 
    /*0000bb7c*/ LDR         R0, [R3, #4] 
    /*0000bb80*/ CMP         R0, #0 
    /*0000bb84*/ BEQ         SVP_NNIE_HAL_DeInit_xbb48
    /*0000bb88*/ BL          0x000000000000BB88          @ osal_iounmap [R_ARM_CALL: +0x0] 
    /*0000bb8c*/ B           SVP_NNIE_HAL_DeInit_xbb48
    /*0000bb90*/ LDRDEQ      R0, [R0], -R8 
FUNC_END SVP_NNIE_HAL_DeInit


FUNC_BEGIN SVP_NNIE_HAL_WriteTaskAddr
    /*0000bb94*/ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bb98*/ MOVT        R1, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bb9c*/ ADD         R0, R1, R0, LSL #2 
    /*0000bba0*/ LDR         R1, [R0, #4] 
    /*0000bba4*/ STR         R2, [R1, #32] 
    /*0000bba8*/ STR         R3, [R1, #36]               @ 0x24 
    /*0000bbac*/ BX          LR 
FUNC_END SVP_NNIE_HAL_WriteTaskAddr


FUNC_BEGIN SVP_NNIE_HAL_Start
    /*0000bbb0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bbb4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bbb8*/ ADD         R0, R3, R0, LSL #2 
    /*0000bbbc*/ LDR         R2, [R0, #4] 
    /*0000bbc0*/ LDR         R3, [R2, #48]               @ 0x30 
    /*0000bbc4*/ ORR         R3, R3, #1 
    /*0000bbc8*/ STR         R3, [R2, #48]               @ 0x30 
    /*0000bbcc*/ BX          LR 
FUNC_END SVP_NNIE_HAL_Start


FUNC_BEGIN SVP_NNIE_HAL_GetIntStatus
    /*0000bbd0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bbd4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bbd8*/ ADD         R0, R3, R0, LSL #2 
    /*0000bbdc*/ LDR         R3, [R0, #4] 
    /*0000bbe0*/ LDR         R0, [R3, #60]               @ 0x3c 
    /*0000bbe4*/ BX          LR 
FUNC_END SVP_NNIE_HAL_GetIntStatus


FUNC_BEGIN SVP_NNIE_HAL_ClearInt
    /*0000bbe8*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bbec*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bbf0*/ ADD         R0, R3, R0, LSL #2 
    /*0000bbf4*/ LDR         R3, [R0, #4] 
    /*0000bbf8*/ LDR         R2, [R3, #56]               @ 0x38 
    /*0000bbfc*/ ORR         R2, R2, #1 
    /*0000bc00*/ STR         R2, [R3, #56]               @ 0x38 
    /*0000bc04*/ LDR         R2, [R3, #56]               @ 0x38 
    /*0000bc08*/ ORR         R2, R2, #2 
    /*0000bc0c*/ STR         R2, [R3, #56]               @ 0x38 
    /*0000bc10*/ LDR         R2, [R3, #56]               @ 0x38 
    /*0000bc14*/ ORR         R2, R2, #4 
    /*0000bc18*/ STR         R2, [R3, #56]               @ 0x38 
    /*0000bc1c*/ BX          LR 
FUNC_END SVP_NNIE_HAL_ClearInt


FUNC_BEGIN SVP_NNIE_HAL_GetTaskId
    /*0000bc20*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bc24*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bc28*/ ADD         R0, R3, R0, LSL #2 
    /*0000bc2c*/ LDR         R3, [R0, #4] 
    /*0000bc30*/ LDR         R0, [R3, #72]               @ 0x48 
    /*0000bc34*/ BX          LR 
FUNC_END SVP_NNIE_HAL_GetTaskId


FUNC_BEGIN SVP_NNIE_HAL_EnInt
    /*0000bc38*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bc3c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bc40*/ ADD         R0, R3, R0, LSL #2 
    /*0000bc44*/ LDR         R3, [R0, #4] 
    /*0000bc48*/ LDR         R2, [R3, #52]               @ 0x34 
    /*0000bc4c*/ ORR         R2, R2, #1 
    /*0000bc50*/ STR         R2, [R3, #52]               @ 0x34 
    /*0000bc54*/ LDR         R2, [R3, #52]               @ 0x34 
    /*0000bc58*/ ORR         R2, R2, #2 
    /*0000bc5c*/ STR         R2, [R3, #52]               @ 0x34 
    /*0000bc60*/ LDR         R2, [R3, #52]               @ 0x34 
    /*0000bc64*/ ORR         R2, R2, #4 
    /*0000bc68*/ STR         R2, [R3, #52]               @ 0x34 
    /*0000bc6c*/ BX          LR 
FUNC_END SVP_NNIE_HAL_EnInt


FUNC_BEGIN SVP_NNIE_HAL_SetTimeOut
    /*0000bc70*/ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bc74*/ MOVT        R1, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bc78*/ ADD         R0, R1, R0, LSL #2 
    /*0000bc7c*/ LDR         R1, [R0, #4] 
    /*0000bc80*/ STR         R2, [R1, #40]               @ 0x28 
    /*0000bc84*/ STR         R3, [R1, #44]               @ 0x2c 
    /*0000bc88*/ BX          LR 
FUNC_END SVP_NNIE_HAL_SetTimeOut


FUNC_BEGIN SVP_NNIE_HAL_EnClkGt
    /*0000bc8c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bc90*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bc94*/ ADD         R0, R3, R0, LSL #2 
    /*0000bc98*/ LDR         R2, [R0, #4] 
    /*0000bc9c*/ LDR         R3, [R2, #80]               @ 0x50 
    /*0000bca0*/ ORR         R3, R3, #128                @ 0x80 
    /*0000bca4*/ STR         R3, [R2, #80]               @ 0x50 
    /*0000bca8*/ BX          LR 
FUNC_END SVP_NNIE_HAL_EnClkGt


FUNC_BEGIN SVP_NNIE_HAL_SetMemSpeed
    /*0000bcac*/ BX          LR 
FUNC_END SVP_NNIE_HAL_SetMemSpeed


FUNC_BEGIN SVP_NNIE_HAL_GetCfgErrInfo
    /*0000bcb0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bcb4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bcb8*/ ADD         R0, R3, R0, LSL #2 
    /*0000bcbc*/ LDR         R3, [R0, #4] 
    /*0000bcc0*/ LDR         R0, [R3, #64]               @ 0x40 
    /*0000bcc4*/ BX          LR 
FUNC_END SVP_NNIE_HAL_GetCfgErrInfo


FUNC_BEGIN SVP_NNIE_HAL_GetCycle
    /*0000bcc8*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bccc*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bcd0*/ ADD         R3, R3, R0, LSL #2 
    /*0000bcd4*/ MOV         R0, #0 
    /*0000bcd8*/ LDR         R3, [R3, #4] 
    /*0000bcdc*/ LDR         R2, [R3, #200]              @ 0xc8 
    /*0000bce0*/ LDR         R3, [R3, #196]              @ 0xc4 
    /*0000bce4*/ MOV         R1, R2 
    /*0000bce8*/ ORR         R0, R0, R3 
    /*0000bcec*/ BX          LR 
FUNC_END SVP_NNIE_HAL_GetCycle


FUNC_BEGIN SVP_NNIE_HAL_DisableCheckSum
    /*0000bcf0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bcf4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bcf8*/ ADD         R0, R3, R0, LSL #2 
    /*0000bcfc*/ LDR         R3, [R0, #4] 
    /*0000bd00*/ LDR         R2, [R3, #104]              @ 0x68 
    /*0000bd04*/ BFC         R2, #0, #1 
    /*0000bd08*/ STR         R2, [R3, #104]              @ 0x68 
    /*0000bd0c*/ BX          LR 
FUNC_END SVP_NNIE_HAL_DisableCheckSum


FUNC_BEGIN hi35xx_svp_nnie_remove
    /*0000bd10*/ PUSH        {R4, LR} 
    /*0000bd14*/ BL          0x000000000000BD14          @ SVP_NNIE_ModExit [R_ARM_CALL: .text+0x3910] 
    /*0000bd18*/ MOVW        R3, #0                      @ g_apvNnieReg [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bd1c*/ MOVT        R3, #0                      @ g_apvNnieReg [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bd20*/ MOV         R2, #0 
    /*0000bd24*/ MOV         R0, R2 
    /*0000bd28*/ STR         R2, [R3] 
    /*0000bd2c*/ POP         {R4, PC} 
FUNC_END hi35xx_svp_nnie_remove


FUNC_BEGIN hi35xx_svp_nnie_probe
    /*0000bd30*/ PUSH        {R4, R5, LR} 
    /*0000bd34*/ SUB         SP, SP, #20 
    /*0000bd38*/ MOV         IP, #0 
    /*0000bd3c*/ MOV         R4, R0 
    /*0000bd40*/ MOV         R3, IP 
    /*0000bd44*/ MOV         R1, #8 
    /*0000bd48*/ MOV         R0, SP 
    /*0000bd4c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3ff4] 
    /*0000bd50*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3ff4] 
    /*0000bd54*/ STR         IP, [SP, #4] 
    /*0000bd58*/ STR         IP, [SP] 
    /*0000bd5c*/ MOVW        LR, #25703                  @ 0x6467 
    /*0000bd60*/ STR         IP, [SP, #12] 
    /*0000bd64*/ MOVT        LR, #99                     @ 0x63 
    /*0000bd68*/ STR         LR, [SP, #8] 
    /*0000bd6c*/ BL          0x000000000000BD6C          @ snprintf [R_ARM_CALL: +0x0] 
    /*0000bd70*/ MOV         R1, #512                    @ 0x200 
    /*0000bd74*/ MOV         R0, R4 
    /*0000bd78*/ ADD         R5, R4, #16 
    /*0000bd7c*/ MOV         R2, SP 
    /*0000bd80*/ BL          0x000000000000BD80          @ osal_platform_get_resource_byname [R_ARM_CALL: +0x0] 
    /*0000bd84*/ MOV         R1, R0 
    /*0000bd88*/ MOV         R0, R5 
    /*0000bd8c*/ BL          0x000000000000BD8C          @ devm_ioremap_resource [R_ARM_CALL: +0x0] 
    /*0000bd90*/ MOVW        R3, #0                      @ g_apvNnieReg [R_ARM_MOVW_ABS_NC: .bss+0x130] 
    /*0000bd94*/ MOVT        R3, #0                      @ g_apvNnieReg [R_ARM_MOVT_ABS: .bss+0x130] 
    /*0000bd98*/ CMN         R0, #4096                   @ 0x1000 
    /*0000bd9c*/ STR         R0, [R3] 
    /*0000bda0*/ BHI         hi35xx_svp_nnie_probe_xbde8
    /*0000bda4*/ MOV         R1, SP 
    /*0000bda8*/ MOV         R0, R4 
    /*0000bdac*/ BL          0x000000000000BDAC          @ osal_platform_get_irq_byname [R_ARM_CALL: +0x0] 
    /*0000bdb0*/ MOVW        R3, #0                      @ g_au32NnieIrg [R_ARM_MOVW_ABS_NC: .data+0xbc] 
    /*0000bdb4*/ MOVT        R3, #0                      @ g_au32NnieIrg [R_ARM_MOVT_ABS: .data+0xbc] 
    /*0000bdb8*/ CMP         R0, #0 
    /*0000bdbc*/ STR         R0, [R3] 
    /*0000bdc0*/ BEQ         hi35xx_svp_nnie_probe_xbdf0
    hi35xx_svp_nnie_probe_xbdc4:
    /*0000bdc4*/ MOV         R0, R4 
    /*0000bdc8*/ ADD         R1, SP, #8 
    /*0000bdcc*/ BL          0x000000000000BDCC          @ osal_platform_get_irq_byname [R_ARM_CALL: +0x0] 
    /*0000bdd0*/ MOVW        R3, #0                      @ g_au32GdcIrq [R_ARM_MOVW_ABS_NC: .data+0xb4] 
    /*0000bdd4*/ MOVT        R3, #0                      @ g_au32GdcIrq [R_ARM_MOVT_ABS: .data+0xb4] 
    /*0000bdd8*/ CMP         R0, #0 
    /*0000bddc*/ STR         R0, [R3] 
    /*0000bde0*/ BEQ         hi35xx_svp_nnie_probe_xbe08
    hi35xx_svp_nnie_probe_xbde4:
    /*0000bde4*/ BL          0x000000000000BDE4          @ SVP_NNIE_ModInit [R_ARM_CALL: .text+0x35e8] 
    hi35xx_svp_nnie_probe_xbde8:
    /*0000bde8*/ ADD         SP, SP, #20 
    /*0000bdec*/ POP         {R4, R5, PC} 
    hi35xx_svp_nnie_probe_xbdf0:
    /*0000bdf0*/ MOV         R2, R0 
    /*0000bdf4*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3ffc] 
    /*0000bdf8*/ MOV         R0, R5 
    /*0000bdfc*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3ffc] 
    /*0000be00*/ BL          0x000000000000BE00          @ dev_err [R_ARM_CALL: +0x0] 
    /*0000be04*/ B           hi35xx_svp_nnie_probe_xbdc4
    hi35xx_svp_nnie_probe_xbe08:
    /*0000be08*/ MOV         R2, R0 
    /*0000be0c*/ MOVW        R1, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4014] 
    /*0000be10*/ MOV         R0, R5 
    /*0000be14*/ MOVT        R1, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4014] 
    /*0000be18*/ BL          0x000000000000BE18          @ dev_err [R_ARM_CALL: +0x0] 
    /*0000be1c*/ B           hi35xx_svp_nnie_probe_xbde4
FUNC_END hi35xx_svp_nnie_probe


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .init.text
@ Size: 0xc
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN hi35xx_svp_nnie_driver_init
    /*00000000*/ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0xc4] 
    /*00000004*/ MOVT        R0, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0xc4] 
    hi35xx_svp_nnie_driver_init_x8:
    /*00000008*/ B           hi35xx_svp_nnie_driver_init_x8@ osal_platform_driver_register [R_ARM_JUMP24: +0x0] 
FUNC_END hi35xx_svp_nnie_driver_init




@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .exit.text
@ Size: 0xc
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN hi35xx_svp_nnie_driver_exit
    /*00000000*/ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0xc4] 
    /*00000004*/ MOVT        R0, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0xc4] 
    hi35xx_svp_nnie_driver_exit_x8:
    /*00000008*/ B           hi35xx_svp_nnie_driver_exit_x8@ osal_platform_driver_unregister [R_ARM_JUMP24: +0x0] 
FUNC_END hi35xx_svp_nnie_driver_exit




