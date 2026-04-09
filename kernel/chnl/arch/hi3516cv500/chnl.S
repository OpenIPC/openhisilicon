@
@ -- FILES --
@

@ chnl_linux.c
@ vedu_chn.c
@ chnl_init.c
@ hi3516cv500_chnl.mod.c


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text
@ Size: 0x4a7c
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN CHNL_GetVerMagic
    /*00000000*/ MOVW        R0, #61479                  @ 0xf027 
    /*00000004*/ MOVT        R0, #307                    @ 0x133 
    /*00000008*/ BX          LR 
FUNC_END CHNL_GetVerMagic


FUNC_BEGIN ChnlProcShow
    /*0000000c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000010*/ SUB         SP, SP, #84                 @ 0x54 
    /*00000014*/ MOV         R9, R0 
    /*00000018*/ MOVW        R1, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*0000001c*/ MOVT        R1, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*00000020*/ MOV         R4, #0 
    /*00000024*/ MOVW        SL, #0                      @ g_ChnlCtx [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000028*/ STR         R4, [SP, #76]               @ 0x4c 
    /*0000002c*/ MOVT        SL, #0                      @ g_ChnlCtx [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000030*/ STR         R4, [SP, #72]               @ 0x48 
    /*00000034*/ BL          0x0000000000000034          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000038*/ MOV         R0, R9 
    /*0000003c*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x60] 
    /*00000040*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x60] 
    /*00000044*/ BL          0x0000000000000044          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000048*/ ADD         R0, SL, #4800               @ 0x12c0 
    /*0000004c*/ MOV         R2, R4 
    /*00000050*/ MOV         R3, SL 
    ChnlProcShow_x54:
    /*00000054*/ LDR         R1, [R3, #952]              @ 0x3b8 
    /*00000058*/ ADD         R3, R3, #960                @ 0x3c0 
    /*0000005c*/ CMP         R1, #1 
    /*00000060*/ ADDEQ       R2, R2, #1 
    /*00000064*/ CMP         R0, R3 
    /*00000068*/ BNE         ChnlProcShow_x54
    /*0000006c*/ MOVW        R1, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x64] 
    /*00000070*/ MOV         R0, R9 
    /*00000074*/ MOVT        R1, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x64] 
    /*00000078*/ BL          0x0000000000000078          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000007c*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x60] 
    /*00000080*/ MOV         R0, R9 
    /*00000084*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x60] 
    /*00000088*/ BL          0x0000000000000088          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000008c*/ MOV         R3, #0 
    /*00000090*/ STR         R3, [SP, #52]               @ 0x34 
    /*00000094*/ STR         R3, [SP, #56]               @ 0x38 
    ChnlProcShow_x98:
    /*00000098*/ LDR         R3, [SL, #952]              @ 0x3b8 
    /*0000009c*/ CMP         R3, #1 
    /*000000a0*/ BEQ         ChnlProcShow_xc8
    ChnlProcShow_xa4:
    /*000000a4*/ LDR         R3, [SP, #52]               @ 0x34 
    /*000000a8*/ ADD         SL, SL, #960                @ 0x3c0 
    /*000000ac*/ ADD         R3, R3, #960                @ 0x3c0 
    /*000000b0*/ STR         R3, [SP, #52]               @ 0x34 
    /*000000b4*/ CMP         R3, #4800                   @ 0x12c0 
    /*000000b8*/ BNE         ChnlProcShow_x98
    /*000000bc*/ MOV         R0, #0 
    /*000000c0*/ ADD         SP, SP, #84                 @ 0x54 
    /*000000c4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    ChnlProcShow_xc8:
    /*000000c8*/ LDR         R2, [SP, #56]               @ 0x38 
    /*000000cc*/ MOVW        R1, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x88] 
    /*000000d0*/ MOV         R0, R9 
    /*000000d4*/ MOVT        R1, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x88] 
    /*000000d8*/ BL          0x00000000000000D8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000000dc*/ MOVW        R3, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*000000e0*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe4] 
    /*000000e4*/ MOVT        R3, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*000000e8*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe4] 
    /*000000ec*/ MOVW        R1, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xec] 
    /*000000f0*/ MOV         R0, R9 
    /*000000f4*/ MOVT        R1, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0xec] 
    /*000000f8*/ BL          0x00000000000000F8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000000fc*/ LDR         R3, [SL, #108]              @ 0x6c 
    /*00000100*/ LDR         R2, [SL] 
    /*00000104*/ MOVW        R1, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf8] 
    /*00000108*/ MOV         R0, R9 
    /*0000010c*/ MOVT        R1, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf8] 
    /*00000110*/ BL          0x0000000000000110          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000114*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x60] 
    /*00000118*/ MOV         R0, R9 
    /*0000011c*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x60] 
    /*00000120*/ BL          0x0000000000000120          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000124*/ MOVW        R1, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00000128*/ MOV         R0, R9 
    /*0000012c*/ MOVT        R1, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*00000130*/ BL          0x0000000000000130          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000134*/ MOVW        R3, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a8] 
    /*00000138*/ MOV         R1, R3 
    /*0000013c*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x198] 
    /*00000140*/ MOVW        R3, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a0] 
    /*00000144*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x198] 
    /*00000148*/ MOVT        R3, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a0] 
    /*0000014c*/ STR         R2, [SP, #36]               @ 0x24 
    /*00000150*/ STR         R3, [SP, #32] 
    /*00000154*/ MOV         R2, R1 
    /*00000158*/ MOVW        FP, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b0] 
    /*0000015c*/ MOVT        R2, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a8] 
    /*00000160*/ MOVT        FP, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b0] 
    /*00000164*/ STR         R2, [SP, #64]               @ 0x40 
    /*00000168*/ STR         R2, [SP, #28] 
    /*0000016c*/ MOVW        R7, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b8] 
    /*00000170*/ STR         FP, [SP, #24] 
    /*00000174*/ MOVT        R7, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b8] 
    /*00000178*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c0] 
    /*0000017c*/ STR         R7, [SP, #20] 
    /*00000180*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c0] 
    /*00000184*/ MOVW        R3, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c8] 
    /*00000188*/ STR         R2, [SP, #16] 
    /*0000018c*/ MOVT        R3, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c8] 
    /*00000190*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d0] 
    /*00000194*/ STR         R3, [SP, #12] 
    /*00000198*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d0] 
    /*0000019c*/ MOVW        R3, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d8] 
    /*000001a0*/ STR         R2, [SP, #8] 
    /*000001a4*/ MOVT        R3, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d8] 
    /*000001a8*/ MOVW        R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e0] 
    /*000001ac*/ STR         R3, [SP, #4] 
    /*000001b0*/ MOVT        R2, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e0] 
    /*000001b4*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x158] 
    /*000001b8*/ STR         R2, [SP, #60]               @ 0x3c 
    /*000001bc*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x158] 
    /*000001c0*/ STR         R2, [SP] 
    /*000001c4*/ MOVW        R1, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x168] 
    /*000001c8*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x160] 
    /*000001cc*/ MOVT        R1, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x168] 
    /*000001d0*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x160] 
    /*000001d4*/ MOV         R0, R9 
    /*000001d8*/ BL          0x00000000000001D8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000001dc*/ LDR         R3, [SL, #108]              @ 0x6c 
    /*000001e0*/ CMP         R3, #0 
    /*000001e4*/ BEQ         ChnlProcShow_x27c
    /*000001e8*/ MOVW        R8, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000001ec*/ MOVW        R6, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x288] 
    /*000001f0*/ MOVT        R8, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*000001f4*/ MOVT        R6, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x288] 
    /*000001f8*/ MOV         R4, SL 
    /*000001fc*/ MOV         R5, #0 
    ChnlProcShow_x200:
    /*00000200*/ LDR         IP, [R4, #120]              @ 0x78 
    /*00000204*/ ADD         R3, R4, #128                @ 0x80 
    /*00000208*/ LDR         LR, [R4, #276]              @ 0x114 
    /*0000020c*/ MOV         R1, R6 
    /*00000210*/ LDR         R0, [R4, #268]              @ 0x10c 
    /*00000214*/ ADD         R5, R5, #1 
    /*00000218*/ LDR         R2, [R4, #144]              @ 0x90 
    /*0000021c*/ ADD         IP, R8, IP, LSL #3 
    /*00000220*/ STR         IP, [SP] 
    /*00000224*/ ADD         R4, R4, #208                @ 0xd0 
    /*00000228*/ LDR         IP, [R4, #48]               @ 0x30 
    /*0000022c*/ STR         LR, [SP, #36]               @ 0x24 
    /*00000230*/ LDR         LR, [R4, #52]               @ 0x34 
    /*00000234*/ STR         R0, [SP, #32] 
    /*00000238*/ LDR         R0, [R4, #56]               @ 0x38 
    /*0000023c*/ STR         IP, [SP, #28] 
    /*00000240*/ LDR         IP, [R4, #44]               @ 0x2c 
    /*00000244*/ STR         LR, [SP, #24] 
    /*00000248*/ LDR         LR, [R4, #40]               @ 0x28 
    /*0000024c*/ STR         R0, [SP, #20] 
    /*00000250*/ LDR         R0, [R4, #36]               @ 0x24 
    /*00000254*/ STR         IP, [SP, #16] 
    /*00000258*/ LDR         IP, [R4, #32] 
    /*0000025c*/ STR         R0, [SP, #8] 
    /*00000260*/ MOV         R0, R9 
    /*00000264*/ STR         LR, [SP, #12] 
    /*00000268*/ STR         IP, [SP, #4] 
    /*0000026c*/ BL          0x000000000000026C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000270*/ LDR         R3, [SL, #108]              @ 0x6c 
    /*00000274*/ CMP         R3, R5 
    /*00000278*/ BHI         ChnlProcShow_x200
    ChnlProcShow_x27c:
    /*0000027c*/ MOVW        R3, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x280] 
    /*00000280*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x60] 
    /*00000284*/ MOV         R0, R9 
    /*00000288*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x60] 
    /*0000028c*/ MOV         R4, R3 
    /*00000290*/ BL          0x0000000000000290          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000294*/ LDR         R6, [SL, #44]               @ 0x2c 
    /*00000298*/ MOV         R2, R4 
    /*0000029c*/ LDR         R3, [SL, #64]               @ 0x40 
    /*000002a0*/ MOVT        R2, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x280] 
    /*000002a4*/ MOVW        R1, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e8] 
    /*000002a8*/ MOV         R0, R9 
    /*000002ac*/ MOVT        R1, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e8] 
    /*000002b0*/ MOV         R4, R2 
    /*000002b4*/ STR         R2, [SP, #68]               @ 0x44 
    /*000002b8*/ MOV         R8, R6 
    /*000002bc*/ STR         R6, [SP, #72]               @ 0x48 
    /*000002c0*/ MOV         R5, SL 
    /*000002c4*/ STR         R3, [SP, #76]               @ 0x4c 
    /*000002c8*/ BL          0x00000000000002C8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000002cc*/ LDR         R3, [SP, #64]               @ 0x40 
    /*000002d0*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x278] 
    /*000002d4*/ LDR         R1, [SP, #60]               @ 0x3c 
    /*000002d8*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x278] 
    /*000002dc*/ STR         FP, [SP, #16] 
    /*000002e0*/ MOV         R0, R9 
    /*000002e4*/ STR         R3, [SP, #20] 
    /*000002e8*/ MOVW        R3, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x270] 
    /*000002ec*/ STR         R1, [SP, #8] 
    /*000002f0*/ MOVT        R3, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x270] 
    /*000002f4*/ STR         R2, [SP, #4] 
    /*000002f8*/ MOVW        R1, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24c] 
    /*000002fc*/ STR         R3, [SP, #24] 
    /*00000300*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*00000304*/ STR         R7, [SP, #12] 
    /*00000308*/ MOVW        R3, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23c] 
    /*0000030c*/ STR         R4, [SP] 
    /*00000310*/ MOVT        R3, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23c] 
    /*00000314*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00000318*/ MOVT        R1, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24c] 
    /*0000031c*/ BL          0x000000000000031C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000320*/ MOV         R3, #0 
    /*00000324*/ STR         R6, [SP, #64]               @ 0x40 
    /*00000328*/ MOV         R6, R3 
    ChnlProcShow_x32c:
    /*0000032c*/ CMP         R8, #0 
    /*00000330*/ STRNE       R6, [SP, #60]               @ 0x3c 
    /*00000334*/ MOVWNE      R7, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b8] 
    /*00000338*/ MOVNE       R4, #0 
    /*0000033c*/ MOVTNE      R7, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b8] 
    /*00000340*/ BEQ         ChnlProcShow_x3ec
    ChnlProcShow_x344:
    /*00000344*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*00000348*/ MOV         LR, #12 
    /*0000034c*/ MOV         R1, R7 
    /*00000350*/ MOV         R0, R9 
    /*00000354*/ CMP         R4, R3 
    /*00000358*/ MOVGE       R2, #0 
    /*0000035c*/ MOVLT       R2, #1 
    /*00000360*/ CMP         R3, #0 
    /*00000364*/ MOVLE       R2, #0 
    /*00000368*/ CMP         R2, #0 
    /*0000036c*/ BEQ         ChnlProcShow_x3dc
    /*00000370*/ LDR         R3, [R5, #40]               @ 0x28 
    /*00000374*/ LDR         R6, [PC, #756]              @ 0x0000000000000670 
    /*00000378*/ SUB         R3, R3, R4 
    /*0000037c*/ SUBS        R3, R3, #1 
    /*00000380*/ LDRMI       R2, [R5, #32] 
    /*00000384*/ ADDMI       R3, R3, R2 
    /*00000388*/ LDR         R2, [R5, #28] 
    /*0000038c*/ LDR         IP, [R2, R3, LSL #2] 
    /*00000390*/ LDR         FP, [IP, #64]               @ 0x40 
    /*00000394*/ LDM         IP, {R2, R3} 
    /*00000398*/ STR         FP, [SP, #24] 
    /*0000039c*/ LDR         FP, [IP, #72]               @ 0x48 
    /*000003a0*/ STR         FP, [SP, #20] 
    /*000003a4*/ LDR         FP, [IP, #68]               @ 0x44 
    /*000003a8*/ STR         FP, [SP, #16] 
    /*000003ac*/ LDR         FP, [IP, #60]               @ 0x3c 
    /*000003b0*/ STR         FP, [SP, #12] 
    /*000003b4*/ LDR         FP, [IP, #48]               @ 0x30 
    /*000003b8*/ ADD         FP, R6, FP, LSL #3 
    /*000003bc*/ STR         FP, [SP, #8] 
    /*000003c0*/ LDR         FP, [IP, #40]               @ 0x28 
    /*000003c4*/ ADD         R6, R6, #48                 @ 0x30 
    /*000003c8*/ STR         FP, [SP, #4] 
    /*000003cc*/ LDR         IP, [IP, #8] 
    /*000003d0*/ MLA         LR, LR, IP, R6 
    /*000003d4*/ STR         LR, [SP] 
    /*000003d8*/ BL          0x00000000000003D8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    ChnlProcShow_x3dc:
    /*000003dc*/ ADD         R4, R4, #1 
    /*000003e0*/ CMP         R4, R8 
    /*000003e4*/ BNE         ChnlProcShow_x344
    /*000003e8*/ LDR         R6, [SP, #60]               @ 0x3c 
    ChnlProcShow_x3ec:
    /*000003ec*/ ADD         R6, R6, #1 
    /*000003f0*/ ADD         R5, R5, #20 
    /*000003f4*/ CMP         R6, #2 
    /*000003f8*/ LDRNE       R8, [SP, #76]               @ 0x4c 
    /*000003fc*/ BNE         ChnlProcShow_x32c
    /*00000400*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x60] 
    /*00000404*/ MOV         R0, R9 
    /*00000408*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x60] 
    /*0000040c*/ LDR         R6, [SP, #64]               @ 0x40 
    /*00000410*/ BL          0x0000000000000410          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000414*/ MOVW        R1, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2dc] 
    /*00000418*/ MOV         R0, R9 
    /*0000041c*/ MOVT        R1, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2dc] 
    /*00000420*/ BL          0x0000000000000420          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000424*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x358] 
    /*00000428*/ MOVW        R3, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x364] 
    /*0000042c*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x358] 
    /*00000430*/ MOVT        R3, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x364] 
    /*00000434*/ STR         R2, [SP, #24] 
    /*00000438*/ STR         R3, [SP, #20] 
    /*0000043c*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x36c] 
    /*00000440*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x374] 
    /*00000444*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x36c] 
    /*00000448*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x374] 
    /*0000044c*/ STR         R2, [SP, #16] 
    /*00000450*/ STR         R3, [SP, #12] 
    /*00000454*/ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x37c] 
    /*00000458*/ MOVW        R3, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x388] 
    /*0000045c*/ MOVT        R2, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x37c] 
    /*00000460*/ MOVT        R3, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x388] 
    /*00000464*/ STR         R2, [SP, #8] 
    /*00000468*/ STR         R3, [SP, #4] 
    /*0000046c*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x390] 
    /*00000470*/ MOVW        R3, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x280] 
    /*00000474*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x390] 
    /*00000478*/ MOVT        R3, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x280] 
    /*0000047c*/ STR         R2, [SP] 
    /*00000480*/ MOVW        R1, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x330] 
    /*00000484*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*00000488*/ MOVT        R1, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x330] 
    /*0000048c*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00000490*/ MOV         R0, R9 
    /*00000494*/ MOVW        R7, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x39c] 
    /*00000498*/ BL          0x0000000000000498          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000049c*/ MOVT        R7, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x39c] 
    /*000004a0*/ MOV         R5, SL 
    /*000004a4*/ MOV         FP, #0 
    ChnlProcShow_x4a8:
    /*000004a8*/ CMP         R6, #0 
    /*000004ac*/ STRNE       SL, [SP, #60]               @ 0x3c 
    /*000004b0*/ STRNE       FP, [SP, #64]               @ 0x40 
    /*000004b4*/ MOVNE       R4, #0 
    /*000004b8*/ MOVNE       R8, #12 
    /*000004bc*/ BEQ         ChnlProcShow_x564
    ChnlProcShow_x4c0:
    /*000004c0*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*000004c4*/ MOV         R1, R7 
    /*000004c8*/ MOV         R0, R9 
    /*000004cc*/ CMP         R4, R3 
    /*000004d0*/ MOVGE       R2, #0 
    /*000004d4*/ MOVLT       R2, #1 
    /*000004d8*/ CMP         R3, #0 
    /*000004dc*/ MOVLE       R2, #0 
    /*000004e0*/ CMP         R2, #0 
    /*000004e4*/ BEQ         ChnlProcShow_x550
    /*000004e8*/ LDR         R3, [R5, #40]               @ 0x28 
    /*000004ec*/ LDR         LR, [PC, #384]              @ 0x0000000000000674 
    /*000004f0*/ SUB         R3, R3, R4 
    /*000004f4*/ SUBS        R3, R3, #1 
    /*000004f8*/ LDRMI       R2, [R5, #32] 
    /*000004fc*/ ADDMI       R3, R3, R2 
    /*00000500*/ LDR         R2, [R5, #28] 
    /*00000504*/ LDR         IP, [R2, R3, LSL #2] 
    /*00000508*/ LDRD        SL, [IP, #112]              @ 0x70 
    /*0000050c*/ LDR         R3, [IP, #8] 
    /*00000510*/ LDR         R2, [IP] 
    /*00000514*/ STRD        SL, [SP, #40]               @ 0x28 
    /*00000518*/ LDRD        SL, [IP, #96]               @ 0x60 
    /*0000051c*/ MLA         R3, R8, R3, LR 
    /*00000520*/ STRD        SL, [SP, #32] 
    /*00000524*/ LDR         LR, [IP, #148]              @ 0x94 
    /*00000528*/ STR         LR, [SP, #24] 
    /*0000052c*/ LDRD        SL, [IP, #88]               @ 0x58 
    /*00000530*/ STRD        SL, [SP, #16] 
    /*00000534*/ LDR         LR, [IP, #104]              @ 0x68 
    /*00000538*/ STR         LR, [SP, #8] 
    /*0000053c*/ LDR         LR, [IP, #84]               @ 0x54 
    /*00000540*/ STR         LR, [SP, #4] 
    /*00000544*/ LDR         IP, [IP, #80]               @ 0x50 
    /*00000548*/ STR         IP, [SP] 
    /*0000054c*/ BL          0x000000000000054C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    ChnlProcShow_x550:
    /*00000550*/ ADD         R4, R4, #1 
    /*00000554*/ CMP         R4, R6 
    /*00000558*/ BNE         ChnlProcShow_x4c0
    /*0000055c*/ LDR         SL, [SP, #60]               @ 0x3c 
    /*00000560*/ LDR         FP, [SP, #64]               @ 0x40 
    ChnlProcShow_x564:
    /*00000564*/ ADD         FP, FP, #1 
    /*00000568*/ ADD         R5, R5, #20 
    /*0000056c*/ CMP         FP, #2 
    /*00000570*/ LDRNE       R6, [SP, #76]               @ 0x4c 
    /*00000574*/ BNE         ChnlProcShow_x4a8
    /*00000578*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x60] 
    /*0000057c*/ MOV         R0, R9 
    /*00000580*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x60] 
    /*00000584*/ BL          0x0000000000000584          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000588*/ MOVW        R1, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3c8] 
    /*0000058c*/ MOV         R0, R9 
    /*00000590*/ MOVT        R1, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3c8] 
    /*00000594*/ BL          0x0000000000000594          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000598*/ LDR         R3, [SP, #68]               @ 0x44 
    /*0000059c*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*000005a0*/ MOVW        R1, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x424] 
    /*000005a4*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*000005a8*/ MOVT        R1, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x424] 
    /*000005ac*/ STR         R3, [SP, #4] 
    /*000005b0*/ MOV         R0, R9 
    /*000005b4*/ STR         R2, [SP] 
    /*000005b8*/ MOVW        R3, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x41c] 
    /*000005bc*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x160] 
    /*000005c0*/ MOVT        R3, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x41c] 
    /*000005c4*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x160] 
    /*000005c8*/ BL          0x00000000000005C8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000005cc*/ LDR         R3, [SL, #108]              @ 0x6c 
    /*000005d0*/ CMP         R3, #0 
    /*000005d4*/ BEQ         ChnlProcShow_x640
    /*000005d8*/ MOVW        R7, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44c] 
    /*000005dc*/ MOVW        R6, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x438] 
    /*000005e0*/ MOVT        R7, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44c] 
    /*000005e4*/ MOVT        R6, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x438] 
    /*000005e8*/ MOV         R4, SL 
    /*000005ec*/ MOV         R5, #0 
    ChnlProcShow_x5f0:
    /*000005f0*/ LDR         IP, [R4, #184]              @ 0xb8 
    /*000005f4*/ MOV         R8, #12 
    /*000005f8*/ ADD         R3, R4, #128                @ 0x80 
    /*000005fc*/ MOV         R1, R6 
    /*00000600*/ CMP         IP, #0 
    /*00000604*/ MOV         R0, R9 
    /*00000608*/ BEQ         ChnlProcShow_x660
    /*0000060c*/ LDR         LR, [IP, #8] 
    /*00000610*/ LDR         FP, [PC, #92]               @ 0x0000000000000674 
    /*00000614*/ LDR         R2, [R4, #144]              @ 0x90 
    /*00000618*/ MLA         LR, R8, LR, FP 
    /*0000061c*/ STR         LR, [SP, #4] 
    /*00000620*/ LDR         IP, [IP] 
    /*00000624*/ STR         IP, [SP] 
    /*00000628*/ BL          0x0000000000000628          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    ChnlProcShow_x62c:
    /*0000062c*/ LDR         R3, [SL, #108]              @ 0x6c 
    /*00000630*/ ADD         R5, R5, #1 
    /*00000634*/ ADD         R4, R4, #208                @ 0xd0 
    /*00000638*/ CMP         R3, R5 
    /*0000063c*/ BHI         ChnlProcShow_x5f0
    ChnlProcShow_x640:
    /*00000640*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00000644*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x60] 
    /*00000648*/ MOV         R0, R9 
    /*0000064c*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x60] 
    /*00000650*/ ADD         R3, R3, #1 
    /*00000654*/ STR         R3, [SP, #56]               @ 0x38 
    /*00000658*/ BL          0x0000000000000658          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000065c*/ B           ChnlProcShow_xa4
    ChnlProcShow_x660:
    /*00000660*/ MOV         R1, R7 
    /*00000664*/ MOV         R0, R9 
    /*00000668*/ BL          0x0000000000000668          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000066c*/ B           ChnlProcShow_x62c
    /*00000670*/ ANDEQ       R0, R0, R0, LSL R0 
    /*00000674*/ ANDEQ       R0, R0, R0, ASR #32 
FUNC_END ChnlProcShow


FUNC_BEGIN CHNL_ModInit
    /*00000678*/ PUSH        {R4, R5, R6, R7, LR} 
    /*0000067c*/ MOVW        R0, #0                      @ g_ChnlCtx [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000680*/ SUB         SP, SP, #12 
    /*00000684*/ MOV         R2, #4800                   @ 0x12c0 
    /*00000688*/ MOVT        R0, #0                      @ g_ChnlCtx [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000068c*/ MOV         R1, #0 
    /*00000690*/ BL          0x0000000000000690          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00000694*/ LDR         R5, [PC, #360]              @ 0x0000000000000804 
    /*00000698*/ MOV         R4, #0 
    /*0000069c*/ MOV         R6, R4 
    CHNL_ModInit_x6a0:
    /*000006a0*/ MOV         R0, R5 
    /*000006a4*/ STR         R6, [R5, #840]              @ 0x348 
    /*000006a8*/ STR         R6, [R5, #-12] 
    /*000006ac*/ ADD         R5, R5, #960                @ 0x3c0 
    /*000006b0*/ STR         R4, [R5, #-1072]            @ 0xfffffbd0 
    /*000006b4*/ BL          0x00000000000006B4          @ osal_spin_lock_init [R_ARM_CALL: +0x0] 
    /*000006b8*/ SUBS        R7, R0, #0 
    /*000006bc*/ BNE         CHNL_ModInit_x7ac
    /*000006c0*/ ADD         R4, R4, #1 
    /*000006c4*/ CMP         R4, #5 
    /*000006c8*/ BNE         CHNL_ModInit_x6a0
    /*000006cc*/ MOVW        R0, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4a0] 
    /*000006d0*/ MOV         R1, R7 
    /*000006d4*/ MOVT        R0, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4a0] 
    /*000006d8*/ BL          0x00000000000006D8          @ osal_create_proc_entry [R_ARM_CALL: +0x0] 
    /*000006dc*/ SUBS        R2, R0, #0 
    /*000006e0*/ BEQ         CHNL_ModInit_x7e0
    /*000006e4*/ MOVW        R3, #0                      @ ChnlProcShow [R_ARM_MOVW_ABS_NC: .text+0xc] 
    /*000006e8*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000006ec*/ MOVT        R3, #0                      @ ChnlProcShow [R_ARM_MOVT_ABS: .text+0xc] 
    /*000006f0*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000006f4*/ STR         R3, [R2, #60]               @ 0x3c 
    /*000006f8*/ BL          0x00000000000006F8          @ CMPI_RegisterModule [R_ARM_CALL: +0x0] 
    /*000006fc*/ CMP         R0, #0 
    /*00000700*/ BEQ         CHNL_ModInit_x76c
    /*00000704*/ MOV         R3, #488                    @ 0x1e8 
    /*00000708*/ MOV         R1, #4 
    /*0000070c*/ STR         R3, [SP] 
    /*00000710*/ MOVW        R2, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4e4] 
    /*00000714*/ LDR         R3, [PC, #236]              @ 0x0000000000000808 
    /*00000718*/ MOVT        R2, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4e4] 
    /*0000071c*/ MOV         R0, #3 
    /*00000720*/ BL          0x0000000000000720          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000724*/ MOVW        R0, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4a0] 
    /*00000728*/ MOV         R1, R7 
    /*0000072c*/ MOVT        R0, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4a0] 
    /*00000730*/ BL          0x0000000000000730          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    CHNL_ModInit_x734:
    /*00000734*/ MOV         R4, #4 
    /*00000738*/ MVN         R7, #0 
    CHNL_ModInit_x73c:
    /*0000073c*/ LDR         R3, [PC, #192]              @ 0x0000000000000804 
    /*00000740*/ MOV         R5, #960                    @ 0x3c0 
    /*00000744*/ MLA         R5, R5, R4, R3 
    CHNL_ModInit_x748:
    /*00000748*/ MOV         R0, R5 
    /*0000074c*/ SUB         R4, R4, #1 
    /*00000750*/ BL          0x0000000000000750          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00000754*/ CMN         R4, #1 
    /*00000758*/ SUB         R5, R5, #960                @ 0x3c0 
    /*0000075c*/ BNE         CHNL_ModInit_x748
    CHNL_ModInit_x760:
    /*00000760*/ MOV         R0, R7 
    /*00000764*/ ADD         SP, SP, #12 
    /*00000768*/ POP         {R4, R5, R6, R7, PC} 
    CHNL_ModInit_x76c:
    /*0000076c*/ MOV         R0, R4 
    /*00000770*/ MOVW        R3, #491                    @ 0x1eb 
    /*00000774*/ MOV         R1, #4 
    /*00000778*/ STR         R3, [SP] 
    /*0000077c*/ MOVW        R2, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x530] 
    /*00000780*/ LDR         R3, [PC, #128]              @ 0x0000000000000808 
    /*00000784*/ MOVT        R2, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0x530] 
    /*00000788*/ BL          0x0000000000000788          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000078c*/ MOVW        R1, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x568] 
    /*00000790*/ MOVW        R0, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x574] 
    /*00000794*/ MOVT        R1, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0x568] 
    /*00000798*/ MOVT        R0, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0x574] 
    /*0000079c*/ BL          0x000000000000079C          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000007a0*/ MOV         R0, R7 
    /*000007a4*/ ADD         SP, SP, #12 
    /*000007a8*/ POP         {R4, R5, R6, R7, PC} 
    CHNL_ModInit_x7ac:
    /*000007ac*/ STR         R4, [SP, #4] 
    /*000007b0*/ MOVW        R3, #446                    @ 0x1be 
    /*000007b4*/ SUB         R4, R4, #1 
    /*000007b8*/ STR         R3, [SP] 
    /*000007bc*/ MOVW        R2, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x468] 
    /*000007c0*/ LDR         R3, [PC, #64]               @ 0x0000000000000808 
    /*000007c4*/ MOVT        R2, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x468] 
    /*000007c8*/ MOV         R1, #4 
    /*000007cc*/ MOV         R0, #3 
    /*000007d0*/ BL          0x00000000000007D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000007d4*/ CMN         R4, #1 
    /*000007d8*/ BNE         CHNL_ModInit_x73c
    /*000007dc*/ B           CHNL_ModInit_x760
    CHNL_ModInit_x7e0:
    /*000007e0*/ MOV         R3, #480                    @ 0x1e0 
    /*000007e4*/ MOVW        R2, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4a8] 
    /*000007e8*/ STR         R3, [SP] 
    /*000007ec*/ MOVT        R2, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4a8] 
    /*000007f0*/ LDR         R3, [PC, #16]               @ 0x0000000000000808 
    /*000007f4*/ MOV         R1, #4 
    /*000007f8*/ MOV         R0, #3 
    /*000007fc*/ BL          0x00000000000007FC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000800*/ B           CHNL_ModInit_x734
    /*00000804*/ ANDEQ       R0, R0, R0, ROR R0          @ g_ChnlCtx [R_ARM_ABS32: .bss+0x0] 
    /*00000808*/ MULEQ       R0, R4, R0 
FUNC_END CHNL_ModInit


FUNC_BEGIN CHNL_ModExit
    /*0000080c*/ PUSH        {R4, LR} 
    /*00000810*/ MOV         R0, #4 
    /*00000814*/ LDR         R4, [PC, #120]              @ 0x0000000000000894 
    /*00000818*/ SUB         SP, SP, #8 
    /*0000081c*/ BL          0x000000000000081C          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    /*00000820*/ MOV         R1, #0 
    /*00000824*/ MOVW        R0, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4a0] 
    /*00000828*/ MOVT        R0, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4a0] 
    /*0000082c*/ BL          0x000000000000082C          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*00000830*/ MOV         R0, R4 
    /*00000834*/ BL          0x0000000000000834          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00000838*/ ADD         R0, R4, #960                @ 0x3c0 
    /*0000083c*/ BL          0x000000000000083C          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00000840*/ ADD         R0, R4, #1920               @ 0x780 
    /*00000844*/ BL          0x0000000000000844          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00000848*/ ADD         R0, R4, #2880               @ 0xb40 
    /*0000084c*/ BL          0x000000000000084C          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00000850*/ ADD         R0, R4, #3840               @ 0xf00 
    /*00000854*/ BL          0x0000000000000854          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00000858*/ MOVW        R3, #554                    @ 0x22a 
    /*0000085c*/ MOV         R1, #4 
    /*00000860*/ STR         R3, [SP] 
    /*00000864*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x590] 
    /*00000868*/ LDR         R3, [PC, #40]               @ 0x0000000000000898 
    /*0000086c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x590] 
    /*00000870*/ MOV         R0, #7 
    /*00000874*/ BL          0x0000000000000874          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000878*/ MOVW        R1, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x568] 
    /*0000087c*/ MOVW        R0, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5c0] 
    /*00000880*/ MOVT        R1, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0x568] 
    /*00000884*/ MOVT        R0, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5c0] 
    /*00000888*/ ADD         SP, SP, #8 
    /*0000088c*/ POP         {R4, LR} 
    CHNL_ModExit_x890:
    /*00000890*/ B           CHNL_ModExit_x890           @ osal_printk [R_ARM_JUMP24: +0x0] 
    /*00000894*/ ANDEQ       R0, R0, R0, ROR R0          @ g_ChnlCtx [R_ARM_ABS32: .bss+0x0] 
    /*00000898*/ ANDEQ       R0, R0, R4, LSR #1 
FUNC_END CHNL_ModExit


FUNC_BEGIN CHNL_WaitConditionCallBack
    /*0000089c*/ LDR         R0, [R0, #48]               @ 0x30 
    /*000008a0*/ SUB         R0, R0, #6 
    /*000008a4*/ CLZ         R0, R0 
    /*000008a8*/ LSR         R0, R0, #5 
    /*000008ac*/ BX          LR 
FUNC_END CHNL_WaitConditionCallBack


FUNC_BEGIN ChnlCheckVpuInfo
    /*000008b0*/ CMP         R0, #0 
    /*000008b4*/ PUSH        {R4, LR} 
    /*000008b8*/ MOVEQ       R2, #83                     @ 0x53 
    /*000008bc*/ SUB         SP, SP, #16 
    /*000008c0*/ BEQ         ChnlCheckVpuInfo_x9d8
    /*000008c4*/ LDR         R3, [R0, #12] 
    /*000008c8*/ CMP         R3, #0 
    /*000008cc*/ MOVEQ       R2, #84                     @ 0x54 
    /*000008d0*/ BEQ         ChnlCheckVpuInfo_x9d8
    /*000008d4*/ LDR         IP, [R0, #4] 
    /*000008d8*/ SUB         R2, IP, #1 
    /*000008dc*/ CMP         R2, #3 
    /*000008e0*/ MOVWLS      R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000008e4*/ MOVTLS      R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000008e8*/ ADDLS       LR, R2, #4800               @ 0x12c0 
    /*000008ec*/ BHI         ChnlCheckVpuInfo_xa48
    ChnlCheckVpuInfo_x8f0:
    /*000008f0*/ LDR         R1, [R2, #952]              @ 0x3b8 
    /*000008f4*/ CMP         R1, #1 
    /*000008f8*/ BEQ         ChnlCheckVpuInfo_x994
    ChnlCheckVpuInfo_x8fc:
    /*000008fc*/ ADD         R2, R2, #960                @ 0x3c0 
    /*00000900*/ CMP         R2, LR 
    /*00000904*/ BNE         ChnlCheckVpuInfo_x8f0
    /*00000908*/ MOV         R2, #0 
    ChnlCheckVpuInfo_x90c:
    /*0000090c*/ LDR         R1, [R3, #40]               @ 0x28 
    /*00000910*/ ADD         R2, R2, #1 
    /*00000914*/ CMP         R1, #0 
    /*00000918*/ BEQ         ChnlCheckVpuInfo_xa08
    /*0000091c*/ LDR         R1, [R3, #36]               @ 0x24 
    /*00000920*/ CMP         R1, #0 
    /*00000924*/ BEQ         ChnlCheckVpuInfo_xa10
    /*00000928*/ LDR         R1, [R3, #32] 
    /*0000092c*/ CMP         R1, #0 
    /*00000930*/ BEQ         ChnlCheckVpuInfo_xa18
    /*00000934*/ LDR         R1, [R3, #64]               @ 0x40 
    /*00000938*/ CMP         R1, #0 
    /*0000093c*/ BEQ         ChnlCheckVpuInfo_xa20
    /*00000940*/ LDR         R1, [R3, #44]               @ 0x2c 
    /*00000944*/ CMP         R1, #0 
    /*00000948*/ BEQ         ChnlCheckVpuInfo_xa28
    /*0000094c*/ LDR         R1, [R3, #52]               @ 0x34 
    /*00000950*/ CMP         R1, #0 
    /*00000954*/ BEQ         ChnlCheckVpuInfo_xa30
    /*00000958*/ LDR         R1, [R3, #68]               @ 0x44 
    /*0000095c*/ CMP         R1, #0 
    /*00000960*/ BEQ         ChnlCheckVpuInfo_xa38
    /*00000964*/ LDR         R1, [R3, #48]               @ 0x30 
    /*00000968*/ CMP         R1, #0 
    /*0000096c*/ BEQ         ChnlCheckVpuInfo_xa40
    /*00000970*/ LDR         R1, [R3, #56]               @ 0x38 
    /*00000974*/ ADD         R3, R3, #84                 @ 0x54 
    /*00000978*/ CMP         R1, #0 
    /*0000097c*/ BEQ         ChnlCheckVpuInfo_x9d4
    /*00000980*/ CMP         IP, R2 
    /*00000984*/ BNE         ChnlCheckVpuInfo_x90c
    /*00000988*/ MOV         R0, #0 
    /*0000098c*/ ADD         SP, SP, #16 
    /*00000990*/ POP         {R4, PC} 
    ChnlCheckVpuInfo_x994:
    /*00000994*/ LDR         R4, [R2, #4] 
    /*00000998*/ LDR         R1, [R0] 
    /*0000099c*/ CMP         R4, R1 
    /*000009a0*/ BNE         ChnlCheckVpuInfo_x8fc
    /*000009a4*/ MOV         R2, #113                    @ 0x71 
    /*000009a8*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0xb4] 
    /*000009ac*/ STR         R2, [SP] 
    /*000009b0*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0xb4] 
    /*000009b4*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*000009b8*/ MOV         R1, #4 
    /*000009bc*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*000009c0*/ MOV         R0, #3 
    /*000009c4*/ BL          0x00000000000009C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000009c8*/ MOVW        R0, #32771                  @ 0x8003 
    /*000009cc*/ MOVT        R0, #40964                  @ 0xa004 
    /*000009d0*/ B           ChnlCheckVpuInfo_xa00
    ChnlCheckVpuInfo_x9d4:
    /*000009d4*/ MOV         R2, #131                    @ 0x83 
    ChnlCheckVpuInfo_x9d8:
    /*000009d8*/ STR         R2, [SP] 
    /*000009dc*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0xb4] 
    /*000009e0*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5e0] 
    /*000009e4*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0xb4] 
    /*000009e8*/ MOV         R1, #4 
    /*000009ec*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5e0] 
    /*000009f0*/ MOV         R0, #3 
    /*000009f4*/ BL          0x00000000000009F4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000009f8*/ MOVW        R0, #32771                  @ 0x8003 
    /*000009fc*/ MOVT        R0, #40964                  @ 0xa004 
    ChnlCheckVpuInfo_xa00:
    /*00000a00*/ ADD         SP, SP, #16 
    /*00000a04*/ POP         {R4, PC} 
    ChnlCheckVpuInfo_xa08:
    /*00000a08*/ MOV         R2, #123                    @ 0x7b 
    /*00000a0c*/ B           ChnlCheckVpuInfo_x9d8
    ChnlCheckVpuInfo_xa10:
    /*00000a10*/ MOV         R2, #124                    @ 0x7c 
    /*00000a14*/ B           ChnlCheckVpuInfo_x9d8
    ChnlCheckVpuInfo_xa18:
    /*00000a18*/ MOV         R2, #125                    @ 0x7d 
    /*00000a1c*/ B           ChnlCheckVpuInfo_x9d8
    ChnlCheckVpuInfo_xa20:
    /*00000a20*/ MOV         R2, #126                    @ 0x7e 
    /*00000a24*/ B           ChnlCheckVpuInfo_x9d8
    ChnlCheckVpuInfo_xa28:
    /*00000a28*/ MOV         R2, #127                    @ 0x7f 
    /*00000a2c*/ B           ChnlCheckVpuInfo_x9d8
    ChnlCheckVpuInfo_xa30:
    /*00000a30*/ MOV         R2, #128                    @ 0x80 
    /*00000a34*/ B           ChnlCheckVpuInfo_x9d8
    ChnlCheckVpuInfo_xa38:
    /*00000a38*/ MOV         R2, #129                    @ 0x81 
    /*00000a3c*/ B           ChnlCheckVpuInfo_x9d8
    ChnlCheckVpuInfo_xa40:
    /*00000a40*/ MOV         R2, #130                    @ 0x82 
    /*00000a44*/ B           ChnlCheckVpuInfo_x9d8
    ChnlCheckVpuInfo_xa48:
    /*00000a48*/ MOV         R1, #4 
    /*00000a4c*/ MOV         R3, #0 
    /*00000a50*/ MOV         R2, #102                    @ 0x66 
    /*00000a54*/ STR         R3, [SP, #8] 
    /*00000a58*/ STR         R2, [SP] 
    /*00000a5c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0xb4] 
    /*00000a60*/ STR         IP, [SP, #4] 
    /*00000a64*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0xb4] 
    /*00000a68*/ STR         R1, [SP, #12] 
    /*00000a6c*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x614] 
    /*00000a70*/ MOV         R0, #3 
    /*00000a74*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x614] 
    /*00000a78*/ BL          0x0000000000000A78          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000a7c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000a80*/ MOVT        R0, #40964                  @ 0xa004 
    /*00000a84*/ B           ChnlCheckVpuInfo_xa00
FUNC_END ChnlCheckVpuInfo


FUNC_BEGIN ChnlCheckRegInfo
    /*00000a88*/ CMP         R0, #0 
    /*00000a8c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000a90*/ SUB         SP, SP, #12 
    /*00000a94*/ MOVEQ       R3, #156                    @ 0x9c 
    /*00000a98*/ MOVWEQ      R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5e0] 
    /*00000a9c*/ STREQ       R3, [SP] 
    /*00000aa0*/ MOVTEQ      R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5e0] 
    /*00000aa4*/ BEQ         ChnlCheckRegInfo_xb5c
    /*00000aa8*/ LDR         R3, [R0, #8] 
    /*00000aac*/ CMP         R3, #0 
    /*00000ab0*/ MOVEQ       R3, #157                    @ 0x9d 
    /*00000ab4*/ MOVWEQ      R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5e0] 
    /*00000ab8*/ STREQ       R3, [SP] 
    /*00000abc*/ MOVTEQ      R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5e0] 
    /*00000ac0*/ BEQ         ChnlCheckRegInfo_xb5c
    /*00000ac4*/ LDR         R3, [R0, #16] 
    /*00000ac8*/ CMP         R3, #0 
    /*00000acc*/ MOVEQ       R3, #158                    @ 0x9e 
    /*00000ad0*/ MOVWEQ      R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5e0] 
    /*00000ad4*/ STREQ       R3, [SP] 
    /*00000ad8*/ MOVTEQ      R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5e0] 
    /*00000adc*/ BEQ         ChnlCheckRegInfo_xb5c
    /*00000ae0*/ LDR         R3, [R0, #12] 
    /*00000ae4*/ CMP         R3, #0 
    /*00000ae8*/ MOVEQ       R3, #159                    @ 0x9f 
    /*00000aec*/ MOVWEQ      R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5e0] 
    /*00000af0*/ STREQ       R3, [SP] 
    /*00000af4*/ MOVTEQ      R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5e0] 
    /*00000af8*/ BEQ         ChnlCheckRegInfo_xb5c
    /*00000afc*/ LDR         R3, [R0, #20] 
    /*00000b00*/ CMP         R3, #0 
    /*00000b04*/ MOVEQ       R3, #160                    @ 0xa0 
    /*00000b08*/ MOVWEQ      R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5e0] 
    /*00000b0c*/ STREQ       R3, [SP] 
    /*00000b10*/ MOVTEQ      R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5e0] 
    /*00000b14*/ BEQ         ChnlCheckRegInfo_xb5c
    /*00000b18*/ LDR         R3, [R0, #24] 
    /*00000b1c*/ CMP         R3, #0 
    /*00000b20*/ MOVEQ       R3, #161                    @ 0xa1 
    /*00000b24*/ MOVWEQ      R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5e0] 
    /*00000b28*/ STREQ       R3, [SP] 
    /*00000b2c*/ MOVTEQ      R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5e0] 
    /*00000b30*/ BEQ         ChnlCheckRegInfo_xb5c
    /*00000b34*/ LDR         R3, [R0] 
    /*00000b38*/ CMP         R3, #6 
    /*00000b3c*/ MOVLS       R0, #0 
    /*00000b40*/ BHI         ChnlCheckRegInfo_xb4c
    ChnlCheckRegInfo_xb44:
    /*00000b44*/ ADD         SP, SP, #12 
    /*00000b48*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    ChnlCheckRegInfo_xb4c:
    /*00000b4c*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x69c] 
    /*00000b50*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x69c] 
    /*00000b54*/ MOV         R3, #172                    @ 0xac 
    /*00000b58*/ STR         R3, [SP] 
    ChnlCheckRegInfo_xb5c:
    /*00000b5c*/ LDR         R3, [PC, #20]               @ 0x0000000000000b78 
    /*00000b60*/ MOV         R1, #4 
    /*00000b64*/ MOV         R0, #3 
    /*00000b68*/ BL          0x0000000000000B68          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000b6c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000b70*/ MOVT        R0, #40964                  @ 0xa004 
    /*00000b74*/ B           ChnlCheckRegInfo_xb44
    /*00000b78*/ ANDEQ       R0, R0, R8, ASR #1 
FUNC_END ChnlCheckRegInfo


FUNC_BEGIN ChnlCheckUnregInfo
    /*00000b7c*/ CMP         R0, #0 
    /*00000b80*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000b84*/ SUB         SP, SP, #12 
    /*00000b88*/ MOVEQ       R3, #203                    @ 0xcb 
    /*00000b8c*/ MOVWEQ      R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5e0] 
    /*00000b90*/ STREQ       R3, [SP] 
    /*00000b94*/ MOVTEQ      R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5e0] 
    /*00000b98*/ BEQ         ChnlCheckUnregInfo_xbc4
    /*00000b9c*/ LDR         R3, [R0] 
    /*00000ba0*/ CMP         R3, #6 
    /*00000ba4*/ MOVLS       R0, #0 
    /*00000ba8*/ BHI         ChnlCheckUnregInfo_xbb4
    ChnlCheckUnregInfo_xbac:
    /*00000bac*/ ADD         SP, SP, #12 
    /*00000bb0*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    ChnlCheckUnregInfo_xbb4:
    /*00000bb4*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6ec] 
    /*00000bb8*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6ec] 
    /*00000bbc*/ MOV         R3, #215                    @ 0xd7 
    /*00000bc0*/ STR         R3, [SP] 
    ChnlCheckUnregInfo_xbc4:
    /*00000bc4*/ LDR         R3, [PC, #20]               @ 0x0000000000000be0 
    /*00000bc8*/ MOV         R1, #4 
    /*00000bcc*/ MOV         R0, #3 
    /*00000bd0*/ BL          0x0000000000000BD0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000bd4*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000bd8*/ MOVT        R0, #40964                  @ 0xa004 
    /*00000bdc*/ B           ChnlCheckUnregInfo_xbac
    /*00000be0*/ LDRDEQ      R0, [R0], -IP 
FUNC_END ChnlCheckUnregInfo


FUNC_BEGIN ChnlCheckVpuLowPower
    /*00000be4*/ LDR         R3, [R0, #4] 
    /*00000be8*/ TST         R3, #49                     @ 0x31 
    /*00000bec*/ MOVNE       R0, #1 
    /*00000bf0*/ MOVEQ       R0, #0 
    /*00000bf4*/ BX          LR 
FUNC_END ChnlCheckVpuLowPower


FUNC_BEGIN ChnlGetCtxByType
    /*00000bf8*/ MOVW        IP, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000bfc*/ MOVT        IP, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000c00*/ MOV         R3, #1 
    /*00000c04*/ LSL         R0, R3, R0 
    /*00000c08*/ MOV         R2, IP 
    /*00000c0c*/ MOV         R3, #0 
    ChnlGetCtxByType_xc10:
    /*00000c10*/ LDR         R1, [R2, #952]              @ 0x3b8 
    /*00000c14*/ CMP         R1, #1 
    /*00000c18*/ BEQ         ChnlGetCtxByType_xc34
    ChnlGetCtxByType_xc1c:
    /*00000c1c*/ ADD         R3, R3, #1 
    /*00000c20*/ ADD         R2, R2, #960                @ 0x3c0 
    /*00000c24*/ CMP         R3, #5 
    /*00000c28*/ BNE         ChnlGetCtxByType_xc10
    /*00000c2c*/ MOV         R0, #0 
    /*00000c30*/ BX          LR 
    ChnlGetCtxByType_xc34:
    /*00000c34*/ LDR         R1, [R2, #4] 
    /*00000c38*/ TST         R0, R1 
    /*00000c3c*/ BEQ         ChnlGetCtxByType_xc1c
    /*00000c40*/ MOV         R0, #960                    @ 0x3c0 
    /*00000c44*/ MLA         R0, R0, R3, IP 
    /*00000c48*/ BX          LR 
FUNC_END ChnlGetCtxByType


FUNC_BEGIN ChnlUpdatePriorDiff
    /*00000c4c*/ LDR         R3, [R0, #36]               @ 0x24 
    /*00000c50*/ LDR         R2, [R0, #56]               @ 0x38 
    /*00000c54*/ CMP         R3, #0 
    /*00000c58*/ MOVGT       R3, #1 
    /*00000c5c*/ MOVLE       R3, #0 
    /*00000c60*/ CMP         R2, #0 
    /*00000c64*/ ADDGT       R3, R3, #1 
    /*00000c68*/ CMP         R3, #1 
    /*00000c6c*/ MOVLE       R3, #0 
    /*00000c70*/ MOVGT       R3, #1 
    /*00000c74*/ STR         R3, [R0, #84]               @ 0x54 
    /*00000c78*/ BX          LR 
FUNC_END ChnlUpdatePriorDiff


FUNC_BEGIN ChnlGetCtx
    /*00000c7c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000c80*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000c84*/ MOV         R3, #0 
    /*00000c88*/ MOV         R2, R0 
    ChnlGetCtx_xc8c:
    /*00000c8c*/ LDR         R1, [R2, #952]              @ 0x3b8 
    /*00000c90*/ ADD         R2, R2, #960                @ 0x3c0 
    /*00000c94*/ CMP         R1, #0 
    /*00000c98*/ BEQ         ChnlGetCtx_xcb0
    /*00000c9c*/ ADD         R3, R3, #1 
    /*00000ca0*/ CMP         R3, #5 
    /*00000ca4*/ BNE         ChnlGetCtx_xc8c
    /*00000ca8*/ MOV         R0, #0 
    /*00000cac*/ BX          LR 
    ChnlGetCtx_xcb0:
    /*00000cb0*/ MOV         R2, #960                    @ 0x3c0 
    /*00000cb4*/ MLA         R0, R2, R3, R0 
    /*00000cb8*/ BX          LR 
FUNC_END ChnlGetCtx


FUNC_BEGIN CHNL_IRQInit
    /*00000cbc*/ SUBS        R3, R2, #0 
    /*00000cc0*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000cc4*/ SUB         SP, SP, #12 
    /*00000cc8*/ MOVNE       R2, #0 
    /*00000ccc*/ STR         R3, [SP] 
    /*00000cd0*/ MOVWEQ      R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x740] 
    /*00000cd4*/ ADDNE       R3, R3, #8 
    /*00000cd8*/ MOVTEQ      R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x740] 
    /*00000cdc*/ BL          0x0000000000000CDC          @ osal_request_irq [R_ARM_CALL: +0x0] 
    /*00000ce0*/ ADD         SP, SP, #12 
    /*00000ce4*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END CHNL_IRQInit


FUNC_BEGIN CHNL_IRQDelete
    /*00000ce8*/ PUSH        {R4, LR} 
    /*00000cec*/ BL          0x0000000000000CEC          @ osal_free_irq [R_ARM_CALL: +0x0] 
    /*00000cf0*/ MOV         R0, #0 
    /*00000cf4*/ POP         {R4, PC} 
FUNC_END CHNL_IRQDelete


FUNC_BEGIN ChnlGetSchedTabLen
    /*00000cf8*/ PUSH        {R4, LR} 
    /*00000cfc*/ SUBS        R4, R0, #0 
    /*00000d00*/ BEQ         ChnlGetSchedTabLen_xd14
    ChnlGetSchedTabLen_xd04:
    /*00000d04*/ LDR         R0, [R4, #36]               @ 0x24 
    /*00000d08*/ LDR         R3, [R4, #56]               @ 0x38 
    /*00000d0c*/ ADD         R0, R0, R3 
    /*00000d10*/ POP         {R4, PC} 
    ChnlGetSchedTabLen_xd14:
    /*00000d14*/ MOVW        R3, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x748] 
    /*00000d18*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00000d1c*/ MOVT        R3, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x748] 
    /*00000d20*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00000d24*/ MOVW        R2, #867                    @ 0x363 
    /*00000d28*/ LDR         R1, [PC, #4]                @ 0x0000000000000d34 
    /*00000d2c*/ BL          0x0000000000000D2C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000d30*/ B           ChnlGetSchedTabLen_xd04
    /*00000d34*/ STRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
FUNC_END ChnlGetSchedTabLen


FUNC_BEGIN ChnlGetOneTask
    /*00000d38*/ PUSH        {R4, R5, R6, LR} 
    /*00000d3c*/ SUBS        R4, R0, #0 
    /*00000d40*/ MOV         R6, R1 
    /*00000d44*/ MOV         R5, R2 
    /*00000d48*/ BEQ         ChnlGetOneTask_xdb8
    ChnlGetOneTask_xd4c:
    /*00000d4c*/ LDR         R3, [R4, #16] 
    /*00000d50*/ CMP         R3, #0 
    /*00000d54*/ BEQ         ChnlGetOneTask_xdd8
    /*00000d58*/ BIC         R2, R6, R6, ASR #31 
    /*00000d5c*/ CMP         R3, R2 
    /*00000d60*/ BLE         ChnlGetOneTask_xd8c
    /*00000d64*/ LDR         R1, [R4, #12] 
    /*00000d68*/ MOV         R0, #0 
    /*00000d6c*/ SUB         R1, R1, R6 
    /*00000d70*/ SUBS        R1, R1, #1 
    /*00000d74*/ LDRMI       R3, [R4, #4] 
    /*00000d78*/ ADDMI       R1, R1, R3 
    /*00000d7c*/ LDR         R3, [R4] 
    /*00000d80*/ LDR         R3, [R3, R1, LSL #2] 
    /*00000d84*/ STR         R3, [R5] 
    /*00000d88*/ POP         {R4, R5, R6, PC} 
    ChnlGetOneTask_xd8c:
    /*00000d8c*/ MOV         R4, #0 
    /*00000d90*/ MOVW        R2, #899                    @ 0x383 
    /*00000d94*/ STR         R4, [R5] 
    /*00000d98*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00000d9c*/ LDR         R1, [PC, #60]               @ 0x0000000000000de0 
    /*00000da0*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*00000da4*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00000da8*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00000dac*/ BL          0x0000000000000DAC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000db0*/ MOV         R0, R4 
    /*00000db4*/ POP         {R4, R5, R6, PC} 
    ChnlGetOneTask_xdb8:
    /*00000db8*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7a0] 
    /*00000dbc*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00000dc0*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7a0] 
    /*00000dc4*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00000dc8*/ MOV         R2, #888                    @ 0x378 
    /*00000dcc*/ LDR         R1, [PC, #12]               @ 0x0000000000000de0 
    /*00000dd0*/ BL          0x0000000000000DD0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000dd4*/ B           ChnlGetOneTask_xd4c
    ChnlGetOneTask_xdd8:
    /*00000dd8*/ MVN         R0, #0 
    /*00000ddc*/ POP         {R4, R5, R6, PC} 
    /*00000de0*/ ANDEQ       R0, R0, R4, LSL #2 
FUNC_END ChnlGetOneTask


FUNC_BEGIN ChnlUpdateSchdTbl
    /*00000de4*/ PUSH        {R4, R5, R6, LR} 
    /*00000de8*/ SUBS        R5, R0, #0 
    /*00000dec*/ MOV         R4, R1 
    /*00000df0*/ BEQ         ChnlUpdateSchdTbl_xe20
    ChnlUpdateSchdTbl_xdf4:
    /*00000df4*/ MOV         R3, #20 
    /*00000df8*/ ADD         R2, R5, R4, LSL #2 
    /*00000dfc*/ MLA         R4, R3, R4, R5 
    /*00000e00*/ LDR         R3, [R2, #64]               @ 0x40 
    /*00000e04*/ MOV         R0, #0 
    /*00000e08*/ ADD         R3, R3, #1 
    /*00000e0c*/ LDR         R1, [R4, #36]               @ 0x24 
    /*00000e10*/ CMP         R1, R3 
    /*00000e14*/ MOVEQ       R3, #0 
    /*00000e18*/ STR         R3, [R2, #64]               @ 0x40 
    /*00000e1c*/ POP         {R4, R5, R6, PC} 
    ChnlUpdateSchdTbl_xe20:
    /*00000e20*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7cc] 
    /*00000e24*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00000e28*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7cc] 
    /*00000e2c*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00000e30*/ MOVW        R2, #941                    @ 0x3ad 
    /*00000e34*/ LDR         R1, [PC, #4]                @ 0x0000000000000e40 
    /*00000e38*/ BL          0x0000000000000E38          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000e3c*/ B           ChnlUpdateSchdTbl_xdf4
    /*00000e40*/ ANDEQ       R0, R0, R4, LSL R1 
FUNC_END ChnlUpdateSchdTbl


FUNC_BEGIN ChnlScheduler
    /*00000e44*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000e48*/ SUBS        R9, R0, #0 
    /*00000e4c*/ SUB         SP, SP, #44                 @ 0x2c 
    /*00000e50*/ MOV         R3, #0 
    /*00000e54*/ MOV         R4, R1 
    /*00000e58*/ MOV         R6, R2 
    /*00000e5c*/ STR         R3, [SP, #28] 
    /*00000e60*/ BEQ         ChnlScheduler_x114c
    ChnlScheduler_xe64:
    /*00000e64*/ CMP         R6, #0 
    /*00000e68*/ BEQ         ChnlScheduler_x116c
    ChnlScheduler_xe6c:
    /*00000e6c*/ CMP         R4, #1 
    /*00000e70*/ BNE         ChnlScheduler_xff4
    /*00000e74*/ MOVW        R3, #0                      @ g_proc_enable [R_ARM_MOVW_ABS_NC: +0x0] 
    /*00000e78*/ MOVT        R3, #0                      @ g_proc_enable [R_ARM_MOVT_ABS: +0x0] 
    /*00000e7c*/ STR         R3, [SP, #20] 
    /*00000e80*/ MOV         R2, R3 
    /*00000e84*/ MOVW        R3, #2316                   @ 0x90c 
    /*00000e88*/ LDR         R2, [R2] 
    /*00000e8c*/ MOVT        R3, #10 
    /*00000e90*/ CMP         R2, R3 
    /*00000e94*/ BEQ         ChnlScheduler_x1124
    /*00000e98*/ MOV         R2, #0 
    /*00000e9c*/ MOV         R3, #0 
    /*00000ea0*/ STRD        R2, [SP, #8] 
    ChnlScheduler_xea4:
    /*00000ea4*/ ADD         SL, R9, #68                 @ 0x44 
    /*00000ea8*/ ADD         R7, R9, #40                 @ 0x28 
    /*00000eac*/ MOV         R8, #1 
    /*00000eb0*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00000eb4*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00000eb8*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*00000ebc*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00000ec0*/ STR         R3, [SP, #16] 
    /*00000ec4*/ STR         R2, [SP, #4] 
    ChnlScheduler_xec8:
    /*00000ec8*/ LDR         FP, [R7, #16] 
    /*00000ecc*/ CMP         FP, #0 
    /*00000ed0*/ MOVGT       R5, #0 
    /*00000ed4*/ BGT         ChnlScheduler_xef4
    /*00000ed8*/ B           ChnlScheduler_xfe0
    ChnlScheduler_xedc:
    /*00000edc*/ MOV         R1, R8 
    /*00000ee0*/ MOV         R0, R9 
    /*00000ee4*/ ADD         R5, R5, #1 
    /*00000ee8*/ BL          0x0000000000000EE8          @ ChnlUpdateSchdTbl [R_ARM_CALL: .text+0xde4] 
    /*00000eec*/ CMP         R5, FP 
    /*00000ef0*/ BEQ         ChnlScheduler_xfe0
    ChnlScheduler_xef4:
    /*00000ef4*/ ADD         R2, SP, #28 
    /*00000ef8*/ LDR         R1, [SL] 
    /*00000efc*/ MOV         R0, R7 
    /*00000f00*/ BL          0x0000000000000F00          @ ChnlGetOneTask [R_ARM_CALL: .text+0xd38] 
    /*00000f04*/ CMP         R0, #0 
    /*00000f08*/ BNE         ChnlScheduler_x1004
    ChnlScheduler_xf0c:
    /*00000f0c*/ LDR         R3, [SP, #28] 
    /*00000f10*/ LDR         R4, [R3, #52]               @ 0x34 
    /*00000f14*/ LDR         R2, [R3, #60]               @ 0x3c 
    /*00000f18*/ CMP         R4, #0 
    /*00000f1c*/ ADD         R2, R2, #1 
    /*00000f20*/ STR         R2, [R3, #60]               @ 0x3c 
    /*00000f24*/ MOVNE       R2, #1 
    /*00000f28*/ STRNE       R2, [R4] 
    /*00000f2c*/ LDRNE       R3, [SP, #28] 
    /*00000f30*/ STRNE       R2, [R4, #16] 
    /*00000f34*/ LDR         R2, [R6, #24] 
    /*00000f38*/ ADD         R2, R2, #1 
    /*00000f3c*/ STR         R2, [R6, #24] 
    /*00000f40*/ LDR         R2, [R3, #48]               @ 0x30 
    /*00000f44*/ SUB         R2, R2, #2 
    /*00000f48*/ CMP         R2, #1 
    /*00000f4c*/ BHI         ChnlScheduler_xedc
    /*00000f50*/ LDR         R2, [R3, #28] 
    /*00000f54*/ CMP         R2, #0 
    /*00000f58*/ BEQ         ChnlScheduler_x101c
    ChnlScheduler_xf5c:
    /*00000f5c*/ LDR         R1, [R3, #52]               @ 0x34 
    /*00000f60*/ LDR         R0, [R3] 
    /*00000f64*/ STR         R3, [SP] 
    /*00000f68*/ BLX         R2 
    /*00000f6c*/ LDR         R3, [SP] 
    /*00000f70*/ LDR         R2, [SP, #28] 
    /*00000f74*/ STR         R0, [R3, #40]               @ 0x28 
    /*00000f78*/ LDR         R3, [R2, #40]               @ 0x28 
    /*00000f7c*/ CMP         R3, #0 
    /*00000f80*/ BLE         ChnlScheduler_xedc
    /*00000f84*/ LDR         R3, [R2, #16] 
    /*00000f88*/ CMP         R3, #0 
    /*00000f8c*/ BEQ         ChnlScheduler_x1040
    ChnlScheduler_xf90:
    /*00000f90*/ LDR         R1, [R2, #52]               @ 0x34 
    /*00000f94*/ LDR         R0, [R2] 
    /*00000f98*/ BLX         R3 
    /*00000f9c*/ CMP         R0, #0 
    /*00000fa0*/ BEQ         ChnlScheduler_x1064
    /*00000fa4*/ LDR         R3, [SP, #28] 
    /*00000fa8*/ MOV         IP, #2 
    /*00000fac*/ MOV         R1, R8 
    /*00000fb0*/ MOV         R0, R9 
    /*00000fb4*/ ADD         R5, R5, #1 
    /*00000fb8*/ LDR         R2, [R3, #72]               @ 0x48 
    /*00000fbc*/ ADD         R2, R2, #1 
    /*00000fc0*/ STR         R2, [R3, #72]               @ 0x48 
    /*00000fc4*/ LDR         R2, [R6, #16] 
    /*00000fc8*/ ADD         R2, R2, #1 
    /*00000fcc*/ STR         R2, [R6, #16] 
    /*00000fd0*/ STR         IP, [R3, #48]               @ 0x30 
    /*00000fd4*/ BL          0x0000000000000FD4          @ ChnlUpdateSchdTbl [R_ARM_CALL: .text+0xde4] 
    /*00000fd8*/ CMP         R5, FP 
    /*00000fdc*/ BNE         ChnlScheduler_xef4
    ChnlScheduler_xfe0:
    /*00000fe0*/ SUB         R8, R8, #1 
    /*00000fe4*/ SUB         SL, SL, #4 
    /*00000fe8*/ CMN         R8, #1 
    /*00000fec*/ SUB         R7, R7, #20 
    /*00000ff0*/ BNE         ChnlScheduler_xec8
    ChnlScheduler_xff4:
    /*00000ff4*/ MOV         R5, #0 
    ChnlScheduler_xff8:
    /*00000ff8*/ MOV         R0, R5 
    /*00000ffc*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00001000*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    ChnlScheduler_x1004:
    /*00001004*/ LDR         R3, [SP, #16] 
    /*00001008*/ MOVW        R2, #1013                   @ 0x3f5 
    /*0000100c*/ LDR         R1, [PC, #376]              @ 0x000000000000118c 
    /*00001010*/ LDR         R0, [SP, #4] 
    /*00001014*/ BL          0x0000000000001014          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001018*/ B           ChnlScheduler_xf0c
    ChnlScheduler_x101c:
    /*0000101c*/ MOVW        R2, #1034                   @ 0x40a 
    /*00001020*/ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7fc] 
    /*00001024*/ LDR         R1, [PC, #352]              @ 0x000000000000118c 
    /*00001028*/ MOVT        R3, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7fc] 
    /*0000102c*/ LDR         R0, [SP, #4] 
    /*00001030*/ BL          0x0000000000001030          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001034*/ LDR         R3, [SP, #28] 
    /*00001038*/ LDR         R2, [R3, #28] 
    /*0000103c*/ B           ChnlScheduler_xf5c
    ChnlScheduler_x1040:
    /*00001040*/ MOVW        R2, #1050                   @ 0x41a 
    /*00001044*/ MOVW        R3, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x81c] 
    /*00001048*/ LDR         R1, [PC, #316]              @ 0x000000000000118c 
    /*0000104c*/ MOVT        R3, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x81c] 
    /*00001050*/ LDR         R0, [SP, #4] 
    /*00001054*/ BL          0x0000000000001054          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001058*/ LDR         R2, [SP, #28] 
    /*0000105c*/ LDR         R3, [R2, #16] 
    /*00001060*/ B           ChnlScheduler_xf90
    ChnlScheduler_x1064:
    /*00001064*/ LDR         R5, [SP, #28] 
    /*00001068*/ CMP         R4, #0 
    /*0000106c*/ LDR         R3, [R6, #20] 
    /*00001070*/ ADD         R3, R3, #1 
    /*00001074*/ STR         R3, [R6, #20] 
    /*00001078*/ LDR         R3, [R5, #68]               @ 0x44 
    /*0000107c*/ ADD         R3, R3, #1 
    /*00001080*/ STR         R3, [R5, #68]               @ 0x44 
    /*00001084*/ BEQ         ChnlScheduler_x110c
    /*00001088*/ LDR         R3, [R4] 
    /*0000108c*/ CMP         R3, #1 
    /*00001090*/ MOVNE       R3, #3 
    /*00001094*/ STRNE       R3, [R5, #48]               @ 0x30 
    /*00001098*/ BEQ         ChnlScheduler_x110c
    ChnlScheduler_x109c:
    /*0000109c*/ CMP         R5, #0 
    /*000010a0*/ BEQ         ChnlScheduler_xff4
    /*000010a4*/ LDR         R3, [SP, #20] 
    /*000010a8*/ MOVW        R1, #2316                   @ 0x90c 
    /*000010ac*/ LDRD        R6, [SP, #8] 
    /*000010b0*/ MOVT        R1, #10 
    /*000010b4*/ LDR         R3, [R3] 
    /*000010b8*/ ORR         R2, R6, R7 
    /*000010bc*/ CMP         R2, #0 
    /*000010c0*/ SUB         R3, R3, R1 
    /*000010c4*/ CLZ         R3, R3 
    /*000010c8*/ LSR         R3, R3, #5 
    /*000010cc*/ MOVEQ       R3, #0 
    /*000010d0*/ CMP         R3, #0 
    /*000010d4*/ BEQ         ChnlScheduler_xff8
    /*000010d8*/ ADD         R0, SP, #32 
    /*000010dc*/ BL          0x00000000000010DC          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000010e0*/ LDR         R2, [SP, #36]               @ 0x24 
    /*000010e4*/ LDR         R1, [SP, #32] 
    /*000010e8*/ MOVW        R3, #16960                  @ 0x4240 
    /*000010ec*/ MOVT        R3, #15 
    /*000010f0*/ MOV         R0, R5 
    /*000010f4*/ MLA         R3, R1, R3, R2 
    /*000010f8*/ LDR         R2, [SP, #8] 
    /*000010fc*/ SUB         R3, R3, R2 
    /*00001100*/ STR         R3, [R5, #80]               @ 0x50 
    /*00001104*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00001108*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    ChnlScheduler_x110c:
    /*0000110c*/ MOV         R3, #4 
    /*00001110*/ MOV         R1, R8 
    /*00001114*/ MOV         R0, R9 
    /*00001118*/ STR         R3, [R5, #48]               @ 0x30 
    /*0000111c*/ BL          0x000000000000111C          @ ChnlUpdateSchdTbl [R_ARM_CALL: .text+0xde4] 
    /*00001120*/ B           ChnlScheduler_x109c
    ChnlScheduler_x1124:
    /*00001124*/ ADD         R0, SP, #32 
    /*00001128*/ BL          0x0000000000001128          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*0000112c*/ LDR         R2, [SP, #36]               @ 0x24 
    /*00001130*/ LDR         R0, [SP, #32] 
    /*00001134*/ MOVW        R1, #16960                  @ 0x4240 
    /*00001138*/ MOVT        R1, #15 
    /*0000113c*/ ASR         R3, R2, #31 
    /*00001140*/ SMLAL       R2, R3, R1, R0 
    /*00001144*/ STRD        R2, [SP, #8] 
    /*00001148*/ B           ChnlScheduler_xea4
    ChnlScheduler_x114c:
    /*0000114c*/ MOVW        R3, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x748] 
    /*00001150*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00001154*/ MOVT        R3, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x748] 
    /*00001158*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*0000115c*/ MOVW        R2, #989                    @ 0x3dd 
    /*00001160*/ LDR         R1, [PC, #36]               @ 0x000000000000118c 
    /*00001164*/ BL          0x0000000000001164          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001168*/ B           ChnlScheduler_xe64
    ChnlScheduler_x116c:
    /*0000116c*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7e4] 
    /*00001170*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00001174*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7e4] 
    /*00001178*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*0000117c*/ MOVW        R2, #990                    @ 0x3de 
    /*00001180*/ LDR         R1, [PC, #4]                @ 0x000000000000118c 
    /*00001184*/ BL          0x0000000000001184          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001188*/ B           ChnlScheduler_xe6c
    /*0000118c*/ ANDEQ       R0, R0, R8, LSR #2 
FUNC_END ChnlScheduler


FUNC_BEGIN ChnlTimerIntHandler
    /*00001190*/ LDR         R3, [R0] 
    /*00001194*/ CMP         R3, #1 
    /*00001198*/ BXEQ        LR 
    /*0000119c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000011a0*/ MOVW        R5, #0                      @ g_proc_enable [R_ARM_MOVW_ABS_NC: +0x0] 
    /*000011a4*/ MOVT        R5, #0                      @ g_proc_enable [R_ARM_MOVT_ABS: +0x0] 
    /*000011a8*/ MOVW        R3, #2316                   @ 0x90c 
    /*000011ac*/ MOVT        R3, #10 
    /*000011b0*/ SUB         SP, SP, #36                 @ 0x24 
    /*000011b4*/ LDR         R2, [R5] 
    /*000011b8*/ ADD         R9, R0, #120                @ 0x78 
    /*000011bc*/ MOV         R4, R0 
    /*000011c0*/ CMP         R2, R3 
    /*000011c4*/ MOVNE       R6, #0 
    /*000011c8*/ MOVNE       R7, #0 
    /*000011cc*/ BEQ         ChnlTimerIntHandler_x153c
    ChnlTimerIntHandler_x11d0:
    /*000011d0*/ LDR         R8, [R4, #28] 
    /*000011d4*/ CMP         R8, #0 
    /*000011d8*/ BEQ         ChnlTimerIntHandler_x16c8
    ChnlTimerIntHandler_x11dc:
    /*000011dc*/ CMP         R4, #0 
    /*000011e0*/ BEQ         ChnlTimerIntHandler_x16e8
    ChnlTimerIntHandler_x11e4:
    /*000011e4*/ LDR         R1, [R4, #28] 
    /*000011e8*/ LDR         SL, [R1, #96]               @ 0x60 
    /*000011ec*/ CMP         SL, #0 
    /*000011f0*/ BEQ         ChnlTimerIntHandler_x13d8
    /*000011f4*/ LDR         R2, [SL, #48]               @ 0x30 
    /*000011f8*/ CMP         R2, #1 
    /*000011fc*/ BEQ         ChnlTimerIntHandler_x13d8
    /*00001200*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x68] 
    /*00001204*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x68] 
    /*00001208*/ LDR         R3, [R3] 
    /*0000120c*/ CMP         R3, #0 
    /*00001210*/ BEQ         ChnlTimerIntHandler_x1240
    /*00001214*/ LDR         R3, [R1, #4] 
    /*00001218*/ TST         R3, #49                     @ 0x31 
    /*0000121c*/ BEQ         ChnlTimerIntHandler_x1240
    /*00001220*/ LDR         R3, [R4, #108]              @ 0x6c 
    /*00001224*/ MOV         R1, #1 
    /*00001228*/ LDR         R0, [R4, #24] 
    /*0000122c*/ BLX         R3 
    /*00001230*/ BL          0x0000000000001230          @ osal_isb [R_ARM_CALL: +0x0] 
    /*00001234*/ BL          0x0000000000001234          @ osal_dsb [R_ARM_CALL: +0x0] 
    /*00001238*/ BL          0x0000000000001238          @ osal_dmb [R_ARM_CALL: +0x0] 
    /*0000123c*/ LDR         R2, [SL, #48]               @ 0x30 
    ChnlTimerIntHandler_x1240:
    /*00001240*/ SUB         R3, R2, #3 
    /*00001244*/ CMP         R3, #1 
    /*00001248*/ BLS         ChnlTimerIntHandler_x1294
    /*0000124c*/ LDR         R3, [SL] 
    /*00001250*/ MOVW        R0, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x86c] 
    /*00001254*/ STR         R2, [SP] 
    /*00001258*/ MOVT        R0, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x86c] 
    /*0000125c*/ MOVW        R2, #1407                   @ 0x57f 
    /*00001260*/ LDR         R1, [PC, #1460]             @ 0x000000000000181c 
    /*00001264*/ BL          0x0000000000001264          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00001268*/ LDR         R3, [SL, #48]               @ 0x30 
    /*0000126c*/ SUB         R3, R3, #3 
    /*00001270*/ CMP         R3, #1 
    /*00001274*/ BLS         ChnlTimerIntHandler_x1294
    /*00001278*/ MOVW        R3, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8a0] 
    /*0000127c*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00001280*/ MOVT        R3, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8a0] 
    /*00001284*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00001288*/ MOV         R2, #1408                   @ 0x580 
    /*0000128c*/ LDR         R1, [PC, #1416]             @ 0x000000000000181c 
    /*00001290*/ BL          0x0000000000001290          @ osal_panic [R_ARM_CALL: +0x0] 
    ChnlTimerIntHandler_x1294:
    /*00001294*/ STR         SL, [R4, #64]               @ 0x40 
    /*00001298*/ LDR         R2, [SL, #48]               @ 0x30 
    /*0000129c*/ SUB         R3, R2, #3 
    /*000012a0*/ CMP         R3, #1 
    /*000012a4*/ BLS         ChnlTimerIntHandler_x12f0
    /*000012a8*/ LDR         R3, [SL] 
    /*000012ac*/ MOVW        R0, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8fc] 
    /*000012b0*/ STR         R2, [SP] 
    /*000012b4*/ MOVT        R0, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8fc] 
    /*000012b8*/ MOV         R2, #628                    @ 0x274 
    /*000012bc*/ LDR         R1, [PC, #1372]             @ 0x0000000000001820 
    /*000012c0*/ BL          0x00000000000012C0          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000012c4*/ LDR         R3, [SL, #48]               @ 0x30 
    /*000012c8*/ SUB         R3, R3, #3 
    /*000012cc*/ CMP         R3, #1 
    /*000012d0*/ BLS         ChnlTimerIntHandler_x12f0
    /*000012d4*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x92c] 
    /*000012d8*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000012dc*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x92c] 
    /*000012e0*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000012e4*/ MOVW        R2, #629                    @ 0x275 
    /*000012e8*/ LDR         R1, [PC, #1328]             @ 0x0000000000001820 
    /*000012ec*/ BL          0x00000000000012EC          @ osal_panic [R_ARM_CALL: +0x0] 
    ChnlTimerIntHandler_x12f0:
    /*000012f0*/ LDR         R3, [R4, #92]               @ 0x5c 
    /*000012f4*/ MVN         R1, #0 
    /*000012f8*/ LDR         R0, [R4, #24] 
    /*000012fc*/ BLX         R3 
    /*00001300*/ LDR         R2, [SL, #52]               @ 0x34 
    /*00001304*/ CMP         R2, #0 
    /*00001308*/ LDRNE       R3, [R4, #24] 
    /*0000130c*/ STRNE       R3, [R2, #12] 
    /*00001310*/ LDRNE       R2, [SL, #52]               @ 0x34 
    /*00001314*/ LDR         R3, [SL, #36]               @ 0x24 
    /*00001318*/ LDR         R1, [R4, #68]               @ 0x44 
    /*0000131c*/ LDR         R0, [SL] 
    /*00001320*/ BLX         R3 
    /*00001324*/ CMP         R0, #0 
    /*00001328*/ BNE         ChnlTimerIntHandler_x13b8
    ChnlTimerIntHandler_x132c:
    /*0000132c*/ LDR         R3, [R4, #112]              @ 0x70 
    /*00001330*/ LDR         R0, [R4, #24] 
    /*00001334*/ BLX         R3 
    /*00001338*/ LDR         FP, [R5] 
    /*0000133c*/ MOVW        R3, #2316                   @ 0x90c 
    /*00001340*/ MOVT        R3, #10 
    /*00001344*/ CMP         FP, R3 
    /*00001348*/ BEQ         ChnlTimerIntHandler_x1560
    /*0000134c*/ MOV         R2, #0 
    /*00001350*/ MOV         R3, #0 
    /*00001354*/ STR         SL, [R4, #64]               @ 0x40 
    /*00001358*/ STRD        R2, [R4, #192]              @ 0xc0 
    ChnlTimerIntHandler_x135c:
    /*0000135c*/ MOV         R2, #0 
    /*00001360*/ MOV         R3, #0 
    /*00001364*/ STRD        R2, [SL, #128]              @ 0x80 
    ChnlTimerIntHandler_x1368:
    /*00001368*/ LDR         R3, [R4, #148]              @ 0x94 
    /*0000136c*/ MOV         R2, R9 
    /*00001370*/ ADD         R0, R8, #8 
    /*00001374*/ ADD         R3, R3, #1 
    /*00001378*/ STR         R3, [R4, #148]              @ 0x94 
    /*0000137c*/ LDR         R1, [R8, #100]              @ 0x64 
    /*00001380*/ BL          0x0000000000001380          @ ChnlScheduler [R_ARM_CALL: .text+0xe44] 
    /*00001384*/ LDR         R2, [R5] 
    /*00001388*/ MOVW        R3, #2316                   @ 0x90c 
    /*0000138c*/ MOVT        R3, #10 
    /*00001390*/ CMP         R2, R3 
    /*00001394*/ STR         R0, [R8, #96]               @ 0x60 
    /*00001398*/ BEQ         ChnlTimerIntHandler_x15c8
    ChnlTimerIntHandler_x139c:
    /*0000139c*/ LDR         R1, [R4, #64]               @ 0x40 
    /*000013a0*/ CMP         R1, #0 
    /*000013a4*/ MOVNE       R3, #1 
    /*000013a8*/ STRNE       R3, [R4] 
    /*000013ac*/ BEQ         ChnlTimerIntHandler_x173c
    ChnlTimerIntHandler_x13b0:
    /*000013b0*/ ADD         SP, SP, #36                 @ 0x24 
    /*000013b4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    ChnlTimerIntHandler_x13b8:
    /*000013b8*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*000013bc*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000013c0*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*000013c4*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000013c8*/ MOVW        R2, #646                    @ 0x286 
    /*000013cc*/ LDR         R1, [PC, #1100]             @ 0x0000000000001820 
    /*000013d0*/ BL          0x00000000000013D0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000013d4*/ B           ChnlTimerIntHandler_x132c
    ChnlTimerIntHandler_x13d8:
    /*000013d8*/ ADD         FP, R8, #8 
    /*000013dc*/ MOV         R2, R9 
    /*000013e0*/ LDR         R1, [R8, #100]              @ 0x64 
    /*000013e4*/ MOV         R0, FP 
    /*000013e8*/ BL          0x00000000000013E8          @ ChnlScheduler [R_ARM_CALL: .text+0xe44] 
    /*000013ec*/ CMP         R0, #0 
    /*000013f0*/ MOV         SL, R0 
    /*000013f4*/ STR         R0, [R4, #64]               @ 0x40 
    /*000013f8*/ BEQ         ChnlTimerIntHandler_x1784
    /*000013fc*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x68] 
    /*00001400*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x68] 
    /*00001404*/ LDR         R3, [R3] 
    /*00001408*/ CMP         R3, #0 
    /*0000140c*/ BNE         ChnlTimerIntHandler_x1708
    ChnlTimerIntHandler_x1410:
    /*00001410*/ LDR         R2, [SL, #48]               @ 0x30 
    /*00001414*/ SUB         R3, R2, #3 
    /*00001418*/ CMP         R3, #1 
    /*0000141c*/ BLS         ChnlTimerIntHandler_x1468
    /*00001420*/ LDR         R3, [SL] 
    /*00001424*/ MOVW        R0, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8fc] 
    /*00001428*/ STR         R2, [SP] 
    /*0000142c*/ MOVT        R0, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8fc] 
    /*00001430*/ MOV         R2, #628                    @ 0x274 
    /*00001434*/ LDR         R1, [PC, #996]              @ 0x0000000000001820 
    /*00001438*/ BL          0x0000000000001438          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000143c*/ LDR         R3, [SL, #48]               @ 0x30 
    /*00001440*/ SUB         R3, R3, #3 
    /*00001444*/ CMP         R3, #1 
    /*00001448*/ BLS         ChnlTimerIntHandler_x1468
    /*0000144c*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x92c] 
    /*00001450*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00001454*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x92c] 
    /*00001458*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*0000145c*/ MOVW        R2, #629                    @ 0x275 
    /*00001460*/ LDR         R1, [PC, #952]              @ 0x0000000000001820 
    /*00001464*/ BL          0x0000000000001464          @ osal_panic [R_ARM_CALL: +0x0] 
    ChnlTimerIntHandler_x1468:
    /*00001468*/ LDR         R3, [R4, #92]               @ 0x5c 
    /*0000146c*/ MVN         R1, #0 
    /*00001470*/ LDR         R0, [R4, #24] 
    /*00001474*/ BLX         R3 
    /*00001478*/ LDR         R2, [SL, #52]               @ 0x34 
    /*0000147c*/ CMP         R2, #0 
    /*00001480*/ LDRNE       R3, [R4, #24] 
    /*00001484*/ STRNE       R3, [R2, #12] 
    /*00001488*/ LDRNE       R2, [SL, #52]               @ 0x34 
    /*0000148c*/ LDR         R3, [SL, #36]               @ 0x24 
    /*00001490*/ LDR         R1, [R4, #68]               @ 0x44 
    /*00001494*/ LDR         R0, [SL] 
    /*00001498*/ BLX         R3 
    /*0000149c*/ CMP         R0, #0 
    /*000014a0*/ BNE         ChnlTimerIntHandler_x178c
    ChnlTimerIntHandler_x14a4:
    /*000014a4*/ LDR         R3, [R4, #112]              @ 0x70 
    /*000014a8*/ LDR         R0, [R4, #24] 
    /*000014ac*/ BLX         R3 
    /*000014b0*/ LDR         R3, [R5] 
    /*000014b4*/ MOVW        R2, #2316                   @ 0x90c 
    /*000014b8*/ MOVT        R2, #10 
    /*000014bc*/ CMP         R3, R2 
    /*000014c0*/ BEQ         ChnlTimerIntHandler_x17ac
    /*000014c4*/ MOV         R2, #0 
    /*000014c8*/ MOV         R3, #0 
    /*000014cc*/ STR         SL, [R4, #64]               @ 0x40 
    /*000014d0*/ STRD        R2, [R4, #192]              @ 0xc0 
    ChnlTimerIntHandler_x14d4:
    /*000014d4*/ MOV         R2, #0 
    /*000014d8*/ MOV         R3, #0 
    /*000014dc*/ STRD        R2, [SL, #128]              @ 0x80 
    ChnlTimerIntHandler_x14e0:
    /*000014e0*/ LDR         R3, [R4, #148]              @ 0x94 
    /*000014e4*/ MOV         R2, R9 
    /*000014e8*/ MOV         R0, FP 
    /*000014ec*/ ADD         R3, R3, #1 
    /*000014f0*/ STR         R3, [R4, #148]              @ 0x94 
    /*000014f4*/ LDR         R1, [R8, #100]              @ 0x64 
    /*000014f8*/ BL          0x00000000000014F8          @ ChnlScheduler [R_ARM_CALL: .text+0xe44] 
    /*000014fc*/ LDR         R2, [R5] 
    /*00001500*/ MOVW        R3, #2316                   @ 0x90c 
    /*00001504*/ MOVT        R3, #10 
    /*00001508*/ CMP         R2, R3 
    /*0000150c*/ STR         R0, [R8, #96]               @ 0x60 
    /*00001510*/ BNE         ChnlTimerIntHandler_x139c
    /*00001514*/ ADD         R8, SP, #24 
    /*00001518*/ MOV         R0, R8 
    /*0000151c*/ BL          0x000000000000151C          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00001520*/ LDR         R0, [SP, #28] 
    /*00001524*/ LDR         R2, [SP, #24] 
    /*00001528*/ MOVW        R3, #16960                  @ 0x4240 
    /*0000152c*/ MOVT        R3, #15 
    /*00001530*/ ASR         R1, R0, #31 
    /*00001534*/ SMLAL       R0, R1, R3, R2 
    /*00001538*/ B           ChnlTimerIntHandler_x15ec
    ChnlTimerIntHandler_x153c:
    /*0000153c*/ ADD         R0, SP, #24 
    /*00001540*/ BL          0x0000000000001540          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00001544*/ LDR         R6, [SP, #28] 
    /*00001548*/ LDR         R2, [SP, #24] 
    /*0000154c*/ MOVW        R3, #16960                  @ 0x4240 
    /*00001550*/ MOVT        R3, #15 
    /*00001554*/ ASR         R7, R6, #31 
    /*00001558*/ SMLAL       R6, R7, R3, R2 
    /*0000155c*/ B           ChnlTimerIntHandler_x11d0
    ChnlTimerIntHandler_x1560:
    /*00001560*/ ADD         R0, SP, #24 
    /*00001564*/ MOVW        R1, #16960                  @ 0x4240 
    /*00001568*/ MOVT        R1, #15 
    /*0000156c*/ STR         R1, [SP, #12] 
    /*00001570*/ STR         R1, [SP, #16] 
    /*00001574*/ BL          0x0000000000001574          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00001578*/ LDR         R2, [SP, #28] 
    /*0000157c*/ LDR         IP, [SP, #24] 
    /*00001580*/ LDR         R1, [SP, #12] 
    /*00001584*/ ASR         R3, R2, #31 
    /*00001588*/ LDR         R0, [R5] 
    /*0000158c*/ STR         SL, [R4, #64]               @ 0x40 
    /*00001590*/ SMLAL       R2, R3, R1, IP 
    /*00001594*/ CMP         R0, FP 
    /*00001598*/ STRD        R2, [R4, #192]              @ 0xc0 
    /*0000159c*/ BNE         ChnlTimerIntHandler_x135c
    /*000015a0*/ ADD         R0, SP, #24 
    /*000015a4*/ STR         R1, [SP, #12] 
    /*000015a8*/ BL          0x00000000000015A8          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000015ac*/ LDR         R2, [SP, #28] 
    /*000015b0*/ LDR         R0, [SP, #24] 
    /*000015b4*/ LDR         R1, [SP, #12] 
    /*000015b8*/ ASR         R3, R2, #31 
    /*000015bc*/ SMLAL       R2, R3, R1, R0 
    /*000015c0*/ STRD        R2, [SL, #128]              @ 0x80 
    /*000015c4*/ B           ChnlTimerIntHandler_x1368
    ChnlTimerIntHandler_x15c8:
    /*000015c8*/ ADD         R8, SP, #24 
    /*000015cc*/ MOV         R0, R8 
    /*000015d0*/ BL          0x00000000000015D0          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000015d4*/ LDR         R0, [SP, #28] 
    /*000015d8*/ LDR         IP, [SP, #24] 
    /*000015dc*/ MOVW        R3, #16960                  @ 0x4240 
    /*000015e0*/ MOVT        R3, #15 
    /*000015e4*/ ASR         R1, R0, #31 
    /*000015e8*/ SMLAL       R0, R1, R3, IP 
    ChnlTimerIntHandler_x15ec:
    /*000015ec*/ LDRD        R2, [R4, #176]              @ 0xb0 
    /*000015f0*/ LDRD        SL, [R4, #192]              @ 0xc0 
    /*000015f4*/ ADDS        R2, R2, R0 
    /*000015f8*/ LDR         R9, [R4, #64]               @ 0x40 
    /*000015fc*/ ADC         R3, R3, R1 
    /*00001600*/ SUBS        R2, R2, SL 
    /*00001604*/ SBC         R3, R3, FP 
    /*00001608*/ CMP         R9, #0 
    /*0000160c*/ STRD        R2, [R4, #176]              @ 0xb0 
    /*00001610*/ BEQ         ChnlTimerIntHandler_x1784
    /*00001614*/ ORRS        R3, R6, R7 
    /*00001618*/ LDR         R1, [R5] 
    /*0000161c*/ MOVW        R2, #2316                   @ 0x90c 
    /*00001620*/ MOVT        R2, #10 
    /*00001624*/ MOVNE       R3, #1 
    /*00001628*/ MOVEQ       R3, #0 
    /*0000162c*/ CMP         R1, R2 
    /*00001630*/ MOVNE       R3, #0 
    /*00001634*/ ANDEQ       R3, R3, #1 
    /*00001638*/ CMP         R3, #0 
    /*0000163c*/ BEQ         ChnlTimerIntHandler_x167c
    /*00001640*/ MOV         R0, R8 
    /*00001644*/ BL          0x0000000000001644          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00001648*/ LDR         R2, [SP, #28] 
    /*0000164c*/ LDR         R1, [SP, #24] 
    /*00001650*/ MOVW        R3, #16960                  @ 0x4240 
    /*00001654*/ MOVT        R3, #15 
    /*00001658*/ MLA         R3, R1, R3, R2 
    /*0000165c*/ SUB         R3, R3, R6 
    /*00001660*/ STR         R3, [R9, #84]               @ 0x54 
    /*00001664*/ LDR         R3, [R4, #64]               @ 0x40 
    /*00001668*/ LDR         R1, [R3, #104]              @ 0x68 
    /*0000166c*/ LDR         R2, [R3, #84]               @ 0x54 
    /*00001670*/ CMP         R2, R1 
    /*00001674*/ LDR         R1, [R5] 
    /*00001678*/ STRGT       R2, [R3, #104]              @ 0x68 
    ChnlTimerIntHandler_x167c:
    /*0000167c*/ MOVW        R3, #2316                   @ 0x90c 
    /*00001680*/ MOVT        R3, #10 
    /*00001684*/ CMP         R1, R3 
    /*00001688*/ BNE         ChnlTimerIntHandler_x139c
    /*0000168c*/ ADD         R0, SP, #24 
    /*00001690*/ BL          0x0000000000001690          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00001694*/ LDR         R2, [SP, #28] 
    /*00001698*/ LDR         R0, [SP, #24] 
    /*0000169c*/ MOVW        R1, #16960                  @ 0x4240 
    /*000016a0*/ MOVT        R1, #15 
    /*000016a4*/ ASR         R3, R2, #31 
    /*000016a8*/ SMLAL       R2, R3, R1, R0 
    /*000016ac*/ LDRD        R0, [R4, #168]              @ 0xa8 
    /*000016b0*/ ADDS        R2, R2, R0 
    /*000016b4*/ ADC         R3, R3, R1 
    /*000016b8*/ SUBS        R6, R2, R6 
    /*000016bc*/ SBC         R7, R3, R7 
    /*000016c0*/ STRD        R6, [R4, #168]              @ 0xa8 
    /*000016c4*/ B           ChnlTimerIntHandler_x139c
    ChnlTimerIntHandler_x16c8:
    /*000016c8*/ MOVW        R3, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x840] 
    /*000016cc*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000016d0*/ MOVT        R3, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x840] 
    /*000016d4*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000016d8*/ MOVW        R2, #1389                   @ 0x56d 
    /*000016dc*/ LDR         R1, [PC, #312]              @ 0x000000000000181c 
    /*000016e0*/ BL          0x00000000000016E0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000016e4*/ B           ChnlTimerIntHandler_x11dc
    ChnlTimerIntHandler_x16e8:
    /*000016e8*/ MOVW        R3, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x858] 
    /*000016ec*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000016f0*/ MOVT        R3, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x858] 
    /*000016f4*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000016f8*/ MOVW        R2, #465                    @ 0x1d1 
    /*000016fc*/ LDR         R1, [PC, #288]              @ 0x0000000000001824 
    /*00001700*/ BL          0x0000000000001700          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001704*/ B           ChnlTimerIntHandler_x11e4
    ChnlTimerIntHandler_x1708:
    /*00001708*/ LDR         R3, [R4, #28] 
    /*0000170c*/ LDR         R3, [R3, #4] 
    /*00001710*/ TST         R3, #49                     @ 0x31 
    /*00001714*/ BEQ         ChnlTimerIntHandler_x1410
    /*00001718*/ LDR         R3, [R4, #108]              @ 0x6c 
    /*0000171c*/ MOV         R1, #1 
    /*00001720*/ LDR         R0, [R4, #24] 
    /*00001724*/ BLX         R3 
    /*00001728*/ BL          0x0000000000001728          @ osal_isb [R_ARM_CALL: +0x0] 
    /*0000172c*/ BL          0x000000000000172C          @ osal_dsb [R_ARM_CALL: +0x0] 
    /*00001730*/ BL          0x0000000000001730          @ osal_dmb [R_ARM_CALL: +0x0] 
    /*00001734*/ LDR         SL, [R4, #64]               @ 0x40 
    /*00001738*/ B           ChnlTimerIntHandler_x1410
    ChnlTimerIntHandler_x173c:
    /*0000173c*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x68] 
    /*00001740*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x68] 
    /*00001744*/ LDR         R3, [R3] 
    /*00001748*/ CMP         R3, #0 
    /*0000174c*/ BEQ         ChnlTimerIntHandler_x176c
    /*00001750*/ LDR         R3, [R4, #28] 
    /*00001754*/ LDR         R3, [R3, #4] 
    /*00001758*/ TST         R3, #49                     @ 0x31 
    /*0000175c*/ BEQ         ChnlTimerIntHandler_x176c
    /*00001760*/ LDR         R3, [R4, #108]              @ 0x6c 
    /*00001764*/ LDR         R0, [R4, #24] 
    /*00001768*/ BLX         R3 
    ChnlTimerIntHandler_x176c:
    /*0000176c*/ MOV         R1, #10 
    /*00001770*/ ADD         R0, R4, #36                 @ 0x24 
    /*00001774*/ BL          0x0000000000001774          @ osal_set_timer [R_ARM_CALL: +0x0] 
    /*00001778*/ MOV         R3, #0 
    /*0000177c*/ STR         R3, [R4] 
    /*00001780*/ B           ChnlTimerIntHandler_x13b0
    ChnlTimerIntHandler_x1784:
    /*00001784*/ LDR         R1, [R5] 
    /*00001788*/ B           ChnlTimerIntHandler_x167c
    ChnlTimerIntHandler_x178c:
    /*0000178c*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00001790*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00001794*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*00001798*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*0000179c*/ MOVW        R2, #646                    @ 0x286 
    /*000017a0*/ LDR         R1, [PC, #120]              @ 0x0000000000001820 
    /*000017a4*/ BL          0x00000000000017A4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000017a8*/ B           ChnlTimerIntHandler_x14a4
    ChnlTimerIntHandler_x17ac:
    /*000017ac*/ ADD         R0, SP, #24 
    /*000017b0*/ STR         R3, [SP, #16] 
    /*000017b4*/ MOVW        R2, #16960                  @ 0x4240 
    /*000017b8*/ MOVT        R2, #15 
    /*000017bc*/ STR         R2, [SP, #12] 
    /*000017c0*/ STR         R2, [SP, #20] 
    /*000017c4*/ BL          0x00000000000017C4          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000017c8*/ LDR         R0, [SP, #28] 
    /*000017cc*/ LDR         LR, [SP, #24] 
    /*000017d0*/ LDR         R2, [SP, #12] 
    /*000017d4*/ ASR         R1, R0, #31 
    /*000017d8*/ LDR         IP, [R5] 
    /*000017dc*/ LDR         R3, [SP, #16] 
    /*000017e0*/ SMLAL       R0, R1, R2, LR 
    /*000017e4*/ CMP         IP, R3 
    /*000017e8*/ STR         SL, [R4, #64]               @ 0x40 
    /*000017ec*/ STRD        R0, [R4, #192]              @ 0xc0 
    /*000017f0*/ BNE         ChnlTimerIntHandler_x14d4
    /*000017f4*/ ADD         R0, SP, #24 
    /*000017f8*/ STR         R2, [SP, #12] 
    /*000017fc*/ BL          0x00000000000017FC          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00001800*/ LDR         R0, [SP, #28] 
    /*00001804*/ LDR         R3, [SP, #24] 
    /*00001808*/ LDR         R2, [SP, #12] 
    /*0000180c*/ ASR         R1, R0, #31 
    /*00001810*/ SMLAL       R0, R1, R2, R3 
    /*00001814*/ STRD        R0, [SL, #128]              @ 0x80 
    /*00001818*/ B           ChnlTimerIntHandler_x14e0
    /*0000181c*/ ANDEQ       R0, R0, R8, LSR R1 
    /*00001820*/ ANDEQ       R0, R0, IP, ASR R1 
    /*00001824*/ ANDEQ       R0, R0, IP, ASR #2 
FUNC_END ChnlTimerIntHandler


FUNC_BEGIN ChnlTimerISR
    /*00001828*/ PUSH        {R4, R5, R6, LR} 
    /*0000182c*/ SUBS        R5, R0, #0 
    /*00001830*/ SUB         SP, SP, #16 
    /*00001834*/ BEQ         ChnlTimerISR_x18c8
    /*00001838*/ LDR         R4, [R5, #28] 
    /*0000183c*/ CMP         R4, #0 
    /*00001840*/ BEQ         ChnlTimerISR_x18ec
    ChnlTimerISR_x1844:
    /*00001844*/ ADD         R6, R4, #112                @ 0x70 
    /*00001848*/ ADD         R1, SP, #12 
    /*0000184c*/ MOV         R0, R6 
    /*00001850*/ BL          0x0000000000001850          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001854*/ LDR         R3, [R4, #952]              @ 0x3b8 
    /*00001858*/ CMP         R3, #1 
    /*0000185c*/ BEQ         ChnlTimerISR_x1894
    /*00001860*/ MOVW        R3, #1573                   @ 0x625 
    /*00001864*/ MOV         R1, #4 
    /*00001868*/ STR         R3, [SP] 
    /*0000186c*/ MOVW        R2, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9c8] 
    /*00001870*/ LDR         R3, [PC, #148]              @ 0x000000000000190c 
    /*00001874*/ MOVT        R2, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9c8] 
    /*00001878*/ MOV         R0, #3 
    /*0000187c*/ BL          0x000000000000187C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001880*/ ADD         R1, SP, #12 
    /*00001884*/ MOV         R0, R6 
    /*00001888*/ BL          0x0000000000001888          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    ChnlTimerISR_x188c:
    /*0000188c*/ ADD         SP, SP, #16 
    /*00001890*/ POP         {R4, R5, R6, PC} 
    ChnlTimerISR_x1894:
    /*00001894*/ LDR         R2, [R5, #120]              @ 0x78 
    /*00001898*/ MOV         R0, R5 
    /*0000189c*/ LDR         R3, [R5, #124]              @ 0x7c 
    /*000018a0*/ ADD         R2, R2, #1 
    /*000018a4*/ STR         R2, [R5, #120]              @ 0x78 
    /*000018a8*/ ADD         R3, R3, #1 
    /*000018ac*/ STR         R3, [R5, #124]              @ 0x7c 
    /*000018b0*/ BL          0x00000000000018B0          @ ChnlTimerIntHandler [R_ARM_CALL: .text+0x1190] 
    /*000018b4*/ ADD         R1, SP, #12 
    /*000018b8*/ MOV         R0, R6 
    /*000018bc*/ BL          0x00000000000018BC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000018c0*/ ADD         SP, SP, #16 
    /*000018c4*/ POP         {R4, R5, R6, PC} 
    ChnlTimerISR_x18c8:
    /*000018c8*/ MOVW        R3, #1560                   @ 0x618 
    /*000018cc*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x980] 
    /*000018d0*/ STR         R3, [SP] 
    /*000018d4*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x980] 
    /*000018d8*/ LDR         R3, [PC, #44]               @ 0x000000000000190c 
    /*000018dc*/ MOV         R1, #4 
    /*000018e0*/ MOV         R0, #3 
    /*000018e4*/ BL          0x00000000000018E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000018e8*/ B           ChnlTimerISR_x188c
    ChnlTimerISR_x18ec:
    /*000018ec*/ MOVW        R3, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9b4] 
    /*000018f0*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000018f4*/ MOVT        R3, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9b4] 
    /*000018f8*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000018fc*/ MOVW        R2, #1567                   @ 0x61f 
    /*00001900*/ LDR         R1, [PC, #4]                @ 0x000000000000190c 
    /*00001904*/ BL          0x0000000000001904          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001908*/ B           ChnlTimerISR_x1844
    /*0000190c*/ ANDEQ       R0, R0, IP, ROR #2 
FUNC_END ChnlTimerISR


FUNC_BEGIN ChnlWatchDogTimerISR
    /*00001910*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00001914*/ SUBS        R4, R0, #0 
    /*00001918*/ SUB         SP, SP, #20 
    /*0000191c*/ BEQ         ChnlWatchDogTimerISR_x1aa0
    /*00001920*/ LDR         R5, [R4, #28] 
    /*00001924*/ CMP         R5, #0 
    /*00001928*/ BEQ         ChnlWatchDogTimerISR_x1ac4
    ChnlWatchDogTimerISR_x192c:
    /*0000192c*/ ADD         R6, R5, #112                @ 0x70 
    /*00001930*/ ADD         R1, SP, #12 
    /*00001934*/ MOV         R0, R6 
    /*00001938*/ BL          0x0000000000001938          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000193c*/ LDR         R3, [R5, #952]              @ 0x3b8 
    /*00001940*/ CMP         R3, #1 
    /*00001944*/ BEQ         ChnlWatchDogTimerISR_x195c
    ChnlWatchDogTimerISR_x1948:
    /*00001948*/ ADD         R1, SP, #12 
    /*0000194c*/ MOV         R0, R6 
    /*00001950*/ BL          0x0000000000001950          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    ChnlWatchDogTimerISR_x1954:
    /*00001954*/ ADD         SP, SP, #20 
    /*00001958*/ POP         {R4, R5, R6, R7, PC} 
    ChnlWatchDogTimerISR_x195c:
    /*0000195c*/ LDR         R3, [R4] 
    /*00001960*/ CMP         R3, #1 
    /*00001964*/ LDR         R3, [R4, #144]              @ 0x90 
    /*00001968*/ BEQ         ChnlWatchDogTimerISR_x1980
    ChnlWatchDogTimerISR_x196c:
    /*0000196c*/ STR         R3, [R4, #152]              @ 0x98 
    /*00001970*/ ADD         R0, R4, #48                 @ 0x30 
    /*00001974*/ MOVW        R1, #10000                  @ 0x2710 
    /*00001978*/ BL          0x0000000000001978          @ osal_set_timer [R_ARM_CALL: +0x0] 
    /*0000197c*/ B           ChnlWatchDogTimerISR_x1948
    ChnlWatchDogTimerISR_x1980:
    /*00001980*/ LDR         R2, [R4, #152]              @ 0x98 
    /*00001984*/ CMP         R3, R2 
    /*00001988*/ BNE         ChnlWatchDogTimerISR_x196c
    /*0000198c*/ MOVW        R3, #1645                   @ 0x66d 
    /*00001990*/ ADD         R7, R4, #8 
    /*00001994*/ STR         R3, [SP] 
    /*00001998*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa04] 
    /*0000199c*/ LDR         R3, [PC, #428]              @ 0x0000000000001b50 
    /*000019a0*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa04] 
    /*000019a4*/ STR         R7, [SP, #4] 
    /*000019a8*/ MOV         R1, #4 
    /*000019ac*/ MOV         R0, #3 
    /*000019b0*/ BL          0x00000000000019B0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000019b4*/ LDR         R3, [R4, #116]              @ 0x74 
    /*000019b8*/ CMP         R3, #0 
    /*000019bc*/ BEQ         ChnlWatchDogTimerISR_x19c8
    /*000019c0*/ LDR         R0, [R4, #24] 
    /*000019c4*/ BLX         R3 
    ChnlWatchDogTimerISR_x19c8:
    /*000019c8*/ LDR         R3, [R4, #104]              @ 0x68 
    /*000019cc*/ MOV         R1, #0 
    /*000019d0*/ LDR         R0, [R4, #24] 
    /*000019d4*/ BLX         R3 
    /*000019d8*/ LDR         R5, [R4, #64]               @ 0x40 
    /*000019dc*/ CMP         R5, #0 
    /*000019e0*/ STR         R5, [R4, #60]               @ 0x3c 
    /*000019e4*/ BEQ         ChnlWatchDogTimerISR_x1b2c
    ChnlWatchDogTimerISR_x19e8:
    /*000019e8*/ LDR         R1, [R5, #52]               @ 0x34 
    /*000019ec*/ CMP         R1, #0 
    /*000019f0*/ BEQ         ChnlWatchDogTimerISR_x1a14
    /*000019f4*/ MOV         R3, #1 
    /*000019f8*/ STR         R3, [R1, #4] 
    /*000019fc*/ LDR         R2, [R5, #76]               @ 0x4c 
    /*00001a00*/ STR         R2, [R1, #8] 
    /*00001a04*/ LDR         R1, [R5, #52]               @ 0x34 
    /*00001a08*/ CMP         R1, #0 
    /*00001a0c*/ STRNE       R3, [R1] 
    /*00001a10*/ LDRNE       R1, [R5, #52]               @ 0x34 
    ChnlWatchDogTimerISR_x1a14:
    /*00001a14*/ LDR         R3, [R5, #24] 
    /*00001a18*/ LDR         R0, [R5] 
    /*00001a1c*/ BLX         R3 
    /*00001a20*/ LDR         R3, [R5, #64]               @ 0x40 
    /*00001a24*/ ADD         R3, R3, #1 
    /*00001a28*/ STR         R3, [R5, #64]               @ 0x40 
    /*00001a2c*/ SUBS        R1, R0, #0 
    /*00001a30*/ BNE         ChnlWatchDogTimerISR_x1b14
    ChnlWatchDogTimerISR_x1a34:
    /*00001a34*/ LDR         R3, [R5, #52]               @ 0x34 
    /*00001a38*/ MOV         R2, #0 
    /*00001a3c*/ STR         R2, [R5, #76]               @ 0x4c 
    /*00001a40*/ CMP         R3, R2 
    /*00001a44*/ BEQ         ChnlWatchDogTimerISR_x1ae4
    /*00001a48*/ LDR         R3, [R3] 
    /*00001a4c*/ CMP         R3, #1 
    /*00001a50*/ BEQ         ChnlWatchDogTimerISR_x1ae4
    /*00001a54*/ LDR         R3, [R5, #152]              @ 0x98 
    /*00001a58*/ CMP         R3, #1 
    /*00001a5c*/ BNE         ChnlWatchDogTimerISR_x1a6c
    /*00001a60*/ LDR         R3, [R5, #48]               @ 0x30 
    /*00001a64*/ CMP         R3, #2 
    /*00001a68*/ BEQ         ChnlWatchDogTimerISR_x1af8
    ChnlWatchDogTimerISR_x1a6c:
    /*00001a6c*/ MOV         R1, R7 
    /*00001a70*/ MOVW        R0, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa78] 
    /*00001a74*/ MOVT        R0, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa78] 
    /*00001a78*/ BL          0x0000000000001A78          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00001a7c*/ MOV         R3, #0 
    /*00001a80*/ MOV         R0, R4 
    /*00001a84*/ STR         R3, [R4] 
    /*00001a88*/ BL          0x0000000000001A88          @ ChnlTimerIntHandler [R_ARM_CALL: .text+0x1190] 
    /*00001a8c*/ LDR         R2, [R4, #156]              @ 0x9c 
    /*00001a90*/ LDR         R3, [R4, #144]              @ 0x90 
    /*00001a94*/ ADD         R2, R2, #1 
    /*00001a98*/ STR         R2, [R4, #156]              @ 0x9c 
    /*00001a9c*/ B           ChnlWatchDogTimerISR_x196c
    ChnlWatchDogTimerISR_x1aa0:
    /*00001aa0*/ MOVW        R3, #1621                   @ 0x655 
    /*00001aa4*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x980] 
    /*00001aa8*/ STR         R3, [SP] 
    /*00001aac*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x980] 
    /*00001ab0*/ LDR         R3, [PC, #152]              @ 0x0000000000001b50 
    /*00001ab4*/ MOV         R1, #4 
    /*00001ab8*/ MOV         R0, #3 
    /*00001abc*/ BL          0x0000000000001ABC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001ac0*/ B           ChnlWatchDogTimerISR_x1954
    ChnlWatchDogTimerISR_x1ac4:
    /*00001ac4*/ MOVW        R3, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9b4] 
    /*00001ac8*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00001acc*/ MOVT        R3, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9b4] 
    /*00001ad0*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00001ad4*/ MOVW        R2, #1628                   @ 0x65c 
    /*00001ad8*/ LDR         R1, [PC, #112]              @ 0x0000000000001b50 
    /*00001adc*/ BL          0x0000000000001ADC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001ae0*/ B           ChnlWatchDogTimerISR_x192c
    ChnlWatchDogTimerISR_x1ae4:
    /*00001ae4*/ LDR         R3, [R5, #152]              @ 0x98 
    /*00001ae8*/ MOV         R2, #2 
    /*00001aec*/ STR         R2, [R5, #48]               @ 0x30 
    /*00001af0*/ CMP         R3, #1 
    /*00001af4*/ BNE         ChnlWatchDogTimerISR_x1a6c
    ChnlWatchDogTimerISR_x1af8:
    /*00001af8*/ MOV         R2, #6 
    /*00001afc*/ MOV         R3, #0 
    /*00001b00*/ STR         R2, [R5, #48]               @ 0x30 
    /*00001b04*/ ADD         R0, R5, #156                @ 0x9c 
    /*00001b08*/ STR         R3, [R5, #152]              @ 0x98 
    /*00001b0c*/ BL          0x0000000000001B0C          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00001b10*/ B           ChnlWatchDogTimerISR_x1a6c
    ChnlWatchDogTimerISR_x1b14:
    /*00001b14*/ MOVW        R0, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa5c] 
    /*00001b18*/ MOVT        R0, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa5c] 
    /*00001b1c*/ BL          0x0000000000001B1C          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00001b20*/ MOV         R3, #2 
    /*00001b24*/ STR         R3, [R5, #48]               @ 0x30 
    /*00001b28*/ B           ChnlWatchDogTimerISR_x1a34
    ChnlWatchDogTimerISR_x1b2c:
    /*00001b2c*/ MOVW        R3, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa3c] 
    /*00001b30*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00001b34*/ MOVT        R3, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa3c] 
    /*00001b38*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00001b3c*/ MOVW        R2, #549                    @ 0x225 
    /*00001b40*/ LDR         R1, [PC, #12]               @ 0x0000000000001b54 
    /*00001b44*/ BL          0x0000000000001B44          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001b48*/ LDR         R5, [R4, #60]               @ 0x3c 
    /*00001b4c*/ B           ChnlWatchDogTimerISR_x19e8
    /*00001b50*/ ANDEQ       R0, R0, IP, ROR R1 
    /*00001b54*/ MULEQ       R0, R4, R1 
FUNC_END ChnlWatchDogTimerISR


FUNC_BEGIN ChnlInitSchdTab
    /*00001b58*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00001b5c*/ SUBS        R4, R0, #0 
    /*00001b60*/ SUB         SP, SP, #8 
    /*00001b64*/ MOV         R5, R1 
    /*00001b68*/ BEQ         ChnlInitSchdTab_x1cfc
    ChnlInitSchdTab_x1b6c:
    /*00001b6c*/ CMP         R5, #0 
    /*00001b70*/ BLE         ChnlInitSchdTab_x1cdc
    ChnlInitSchdTab_x1b74:
    /*00001b74*/ MOV         R6, #160                    @ 0xa0 
    /*00001b78*/ LSL         R7, R5, #2 
    /*00001b7c*/ MUL         R6, R6, R5 
    /*00001b80*/ ADD         R8, R7, R5, LSL #3 
    /*00001b84*/ ADD         R8, R8, R6 
    /*00001b88*/ MOV         R0, R8 
    /*00001b8c*/ BL          0x0000000000001B8C          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*00001b90*/ SUBS        R9, R0, #0 
    /*00001b94*/ BEQ         ChnlInitSchdTab_x1d1c
    /*00001b98*/ MOV         R2, R8 
    /*00001b9c*/ MOV         R1, #0 
    /*00001ba0*/ BL          0x0000000000001BA0          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00001ba4*/ CMP         R5, #0 
    /*00001ba8*/ ADD         R3, R9, R7 
    /*00001bac*/ MOV         R2, #0 
    /*00001bb0*/ ADD         LR, R3, R7 
    /*00001bb4*/ STR         R9, [R4, #88]               @ 0x58 
    /*00001bb8*/ STR         R5, [R4, #68]               @ 0x44 
    /*00001bbc*/ STR         R9, [R4, #8] 
    /*00001bc0*/ STR         R3, [R4, #28] 
    /*00001bc4*/ STR         LR, [R4, #48]               @ 0x30 
    /*00001bc8*/ STR         R5, [R4, #12] 
    /*00001bcc*/ STR         R5, [R4, #32] 
    /*00001bd0*/ STR         R5, [R4, #52]               @ 0x34 
    /*00001bd4*/ STR         R2, [R4, #92]               @ 0x5c 
    /*00001bd8*/ STR         R2, [R4, #16] 
    /*00001bdc*/ STR         R2, [R4, #20] 
    /*00001be0*/ STR         R2, [R4, #24] 
    /*00001be4*/ STR         R2, [R4, #36]               @ 0x24 
    /*00001be8*/ STR         R2, [R4, #40]               @ 0x28 
    /*00001bec*/ STR         R2, [R4, #44]               @ 0x2c 
    /*00001bf0*/ STR         R2, [R4, #72]               @ 0x48 
    /*00001bf4*/ STR         R2, [R4, #56]               @ 0x38 
    /*00001bf8*/ STR         R2, [R4, #60]               @ 0x3c 
    /*00001bfc*/ STR         R2, [R4, #64]               @ 0x40 
    /*00001c00*/ STR         R2, [R4, #76]               @ 0x4c 
    /*00001c04*/ BLE         ChnlInitSchdTab_x1cd0
    /*00001c08*/ ADD         R6, R6, R7 
    /*00001c0c*/ ADD         R3, LR, R7 
    /*00001c10*/ MVN         SL, #0 
    /*00001c14*/ ADD         LR, LR, R6 
    /*00001c18*/ MOV         R7, #0 
    /*00001c1c*/ MOV         R6, #0 
    /*00001c20*/ MOV         R9, #7 
    /*00001c24*/ MOV         R8, #128                    @ 0x80 
    /*00001c28*/ MOV         R5, #1 
    ChnlInitSchdTab_x1c2c:
    /*00001c2c*/ STR         SL, [R3] 
    /*00001c30*/ STR         R9, [R3, #8] 
    /*00001c34*/ STR         R8, [R3, #12] 
    /*00001c38*/ STR         R2, [R3, #16] 
    /*00001c3c*/ STR         R2, [R3, #28] 
    /*00001c40*/ STR         R2, [R3, #24] 
    /*00001c44*/ STR         R2, [R3, #32] 
    /*00001c48*/ STR         R2, [R3, #36]               @ 0x24 
    /*00001c4c*/ STR         R2, [R3, #20] 
    /*00001c50*/ STR         R2, [R3, #44]               @ 0x2c 
    /*00001c54*/ STR         R2, [R3, #52]               @ 0x34 
    /*00001c58*/ STR         R2, [R3, #40]               @ 0x28 
    /*00001c5c*/ STR         R2, [R3, #60]               @ 0x3c 
    /*00001c60*/ STR         R2, [R3, #68]               @ 0x44 
    /*00001c64*/ STR         R2, [R3, #72]               @ 0x48 
    /*00001c68*/ STR         R2, [R3, #64]               @ 0x40 
    /*00001c6c*/ STR         R2, [R3, #80]               @ 0x50 
    /*00001c70*/ STR         R2, [R3, #84]               @ 0x54 
    /*00001c74*/ STRD        R6, [R3, #88]               @ 0x58 
    /*00001c78*/ STRD        R6, [R3, #96]               @ 0x60 
    /*00001c7c*/ STR         R2, [R3, #104]              @ 0x68 
    /*00001c80*/ STRD        R6, [R3, #112]              @ 0x70 
    /*00001c84*/ STR         R2, [R3, #148]              @ 0x94 
    /*00001c88*/ STR         R2, [R3, #152]              @ 0x98 
    /*00001c8c*/ STR         R2, [R3, #4] 
    /*00001c90*/ STR         R5, [R3, #48]               @ 0x30 
    /*00001c94*/ LDR         R1, [R4, #24] 
    /*00001c98*/ LDR         R0, [R4, #12] 
    /*00001c9c*/ ADD         IP, R1, #1 
    /*00001ca0*/ CMP         R1, R0 
    /*00001ca4*/ BGE         ChnlInitSchdTab_x1cc4
    /*00001ca8*/ LDR         R1, [R4, #16] 
    /*00001cac*/ SUBS        R1, R1, #1 
    /*00001cb0*/ ADDMI       R1, R1, R0 
    /*00001cb4*/ LDR         R0, [R4, #8] 
    /*00001cb8*/ STR         R3, [R0, R1, LSL #2] 
    /*00001cbc*/ STR         R1, [R4, #16] 
    /*00001cc0*/ STR         IP, [R4, #24] 
    ChnlInitSchdTab_x1cc4:
    /*00001cc4*/ ADD         R3, R3, #160                @ 0xa0 
    /*00001cc8*/ CMP         R3, LR 
    /*00001ccc*/ BNE         ChnlInitSchdTab_x1c2c
    ChnlInitSchdTab_x1cd0:
    /*00001cd0*/ MOV         R0, #0 
    ChnlInitSchdTab_x1cd4:
    /*00001cd4*/ ADD         SP, SP, #8 
    /*00001cd8*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    ChnlInitSchdTab_x1cdc:
    /*00001cdc*/ MOVW        R3, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa90] 
    /*00001ce0*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00001ce4*/ MOVT        R3, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa90] 
    /*00001ce8*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00001cec*/ MOVW        R2, #1697                   @ 0x6a1 
    /*00001cf0*/ LDR         R1, [PC, #80]               @ 0x0000000000001d48 
    /*00001cf4*/ BL          0x0000000000001CF4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001cf8*/ B           ChnlInitSchdTab_x1b74
    ChnlInitSchdTab_x1cfc:
    /*00001cfc*/ MOVW        R3, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x840] 
    /*00001d00*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00001d04*/ MOVT        R3, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x840] 
    /*00001d08*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00001d0c*/ MOV         R2, #1696                   @ 0x6a0 
    /*00001d10*/ LDR         R1, [PC, #48]               @ 0x0000000000001d48 
    /*00001d14*/ BL          0x0000000000001D14          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001d18*/ B           ChnlInitSchdTab_x1b6c
    ChnlInitSchdTab_x1d1c:
    /*00001d1c*/ MOVW        R3, #1708                   @ 0x6ac 
    /*00001d20*/ MOVW        R2, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xaa4] 
    /*00001d24*/ STR         R3, [SP] 
    /*00001d28*/ MOVT        R2, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0xaa4] 
    /*00001d2c*/ LDR         R3, [PC, #20]               @ 0x0000000000001d48 
    /*00001d30*/ MOV         R1, #4 
    /*00001d34*/ MOV         R0, #3 
    /*00001d38*/ BL          0x0000000000001D38          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001d3c*/ MOVW        R0, #32780                  @ 0x800c 
    /*00001d40*/ MOVT        R0, #40964                  @ 0xa004 
    /*00001d44*/ B           ChnlInitSchdTab_x1cd4
    /*00001d48*/ ANDEQ       R0, R0, R4, LSR #3 
FUNC_END ChnlInitSchdTab


FUNC_BEGIN ChnlExitSchdTab
    /*00001d4c*/ PUSH        {R4, LR} 
    /*00001d50*/ SUBS        R4, R0, #0 
    /*00001d54*/ BEQ         ChnlExitSchdTab_x1d98
    ChnlExitSchdTab_x1d58:
    /*00001d58*/ LDR         R0, [R4, #80]               @ 0x50 
    /*00001d5c*/ CMP         R0, #0 
    /*00001d60*/ BEQ         ChnlExitSchdTab_x1d74
    ChnlExitSchdTab_x1d64:
    /*00001d64*/ BL          0x0000000000001D64          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*00001d68*/ MOV         R3, #0 
    /*00001d6c*/ STR         R3, [R4, #80]               @ 0x50 
    /*00001d70*/ POP         {R4, PC} 
    ChnlExitSchdTab_x1d74:
    /*00001d74*/ MOVW        R3, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xae4] 
    /*00001d78*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00001d7c*/ MOVT        R3, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0xae4] 
    /*00001d80*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00001d84*/ MOVW        R2, #1793                   @ 0x701 
    /*00001d88*/ LDR         R1, [PC, #40]               @ 0x0000000000001db8 
    /*00001d8c*/ BL          0x0000000000001D8C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001d90*/ LDR         R0, [R4, #80]               @ 0x50 
    /*00001d94*/ B           ChnlExitSchdTab_x1d64
    ChnlExitSchdTab_x1d98:
    /*00001d98*/ MOVW        R3, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xad0] 
    /*00001d9c*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00001da0*/ MOVT        R3, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0xad0] 
    /*00001da4*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00001da8*/ MOV         R2, #1792                   @ 0x700 
    /*00001dac*/ LDR         R1, [PC, #4]                @ 0x0000000000001db8 
    /*00001db0*/ BL          0x0000000000001DB0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001db4*/ B           ChnlExitSchdTab_x1d58
    /*00001db8*/ @           <UNDEFINED> instruction: 0x000001b4 
FUNC_END ChnlExitSchdTab


FUNC_BEGIN ChnlGetOneTskForPriority
    /*00001dbc*/ PUSH        {R3, R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00001dc0*/ MOV         R4, #20 
    /*00001dc4*/ MLA         R4, R4, R3, R0 
    /*00001dc8*/ LDR         R8, [R4, #36]               @ 0x24 
    /*00001dcc*/ CMP         R8, #0 
    /*00001dd0*/ BEQ         ChnlGetOneTskForPriority_x1eec
    /*00001dd4*/ MOVGT       R5, #0 
    /*00001dd8*/ MOV         SL, R3 
    /*00001ddc*/ MOV         R9, R2 
    /*00001de0*/ MOV         R7, R1 
    /*00001de4*/ MOV         FP, R0 
    /*00001de8*/ MOVGT       R6, R5 
    /*00001dec*/ MOVGT       R3, R8 
    /*00001df0*/ BGT         ChnlGetOneTskForPriority_x1e34
    /*00001df4*/ B           ChnlGetOneTskForPriority_x1ef4
    ChnlGetOneTskForPriority_x1df8:
    /*00001df8*/ LDR         R3, [R4, #36]               @ 0x24 
    /*00001dfc*/ LDR         R1, [R4, #24] 
    /*00001e00*/ CMP         R3, R1 
    /*00001e04*/ BGE         ChnlGetOneTskForPriority_x1e28
    /*00001e08*/ LDR         R2, [R4, #28] 
    /*00001e0c*/ ADD         R3, R3, #1 
    /*00001e10*/ SUBS        R2, R2, #1 
    /*00001e14*/ ADDMI       R2, R2, R1 
    /*00001e18*/ LDR         R1, [R4, #20] 
    /*00001e1c*/ STR         R5, [R1, R2, LSL #2] 
    /*00001e20*/ STR         R3, [R4, #36]               @ 0x24 
    /*00001e24*/ STR         R2, [R4, #28] 
    ChnlGetOneTskForPriority_x1e28:
    /*00001e28*/ ADD         R6, R6, #1 
    /*00001e2c*/ CMP         R6, R8 
    /*00001e30*/ BEQ         ChnlGetOneTskForPriority_x1ecc
    ChnlGetOneTskForPriority_x1e34:
    /*00001e34*/ CMP         R3, #0 
    /*00001e38*/ BLE         ChnlGetOneTskForPriority_x1eac
    /*00001e3c*/ LDR         R2, [R4, #32] 
    /*00001e40*/ SUB         R3, R3, #1 
    /*00001e44*/ SUBS        R2, R2, #1 
    /*00001e48*/ LDRMI       R1, [R4, #24] 
    /*00001e4c*/ ADDMI       R2, R2, R1 
    /*00001e50*/ LDR         R1, [R4, #20] 
    /*00001e54*/ LDR         R5, [R1, R2, LSL #2] 
    /*00001e58*/ STR         R3, [R4, #36]               @ 0x24 
    /*00001e5c*/ STR         R2, [R4, #32] 
    ChnlGetOneTskForPriority_x1e60:
    /*00001e60*/ LDR         R3, [R5, #8] 
    /*00001e64*/ MOV         R0, R5 
    /*00001e68*/ CMP         R3, R7 
    /*00001e6c*/ BNE         ChnlGetOneTskForPriority_x1df8
    /*00001e70*/ LDR         R3, [R5] 
    /*00001e74*/ CMP         R3, R9 
    /*00001e78*/ BNE         ChnlGetOneTskForPriority_x1df8
    /*00001e7c*/ ADD         R3, FP, SL, LSL #2 
    /*00001e80*/ LDR         R2, [R3, #64]               @ 0x40 
    /*00001e84*/ CMP         R2, R6 
    /*00001e88*/ SUBGT       R2, R2, #1 
    /*00001e8c*/ STRGT       R2, [R3, #64]               @ 0x40 
    /*00001e90*/ MOVLE       R3, #20 
    /*00001e94*/ MOVGT       R1, #20 
    /*00001e98*/ MLALE       R3, R3, SL, FP 
    /*00001e9c*/ MLAGT       R1, R1, SL, FP 
    /*00001ea0*/ LDRLE       R3, [R3, #36]               @ 0x24 
    /*00001ea4*/ LDRGT       R3, [R1, #36]               @ 0x24 
    /*00001ea8*/ B           ChnlGetOneTskForPriority_x1ed8
    ChnlGetOneTskForPriority_x1eac:
    /*00001eac*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00001eb0*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00001eb4*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*00001eb8*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00001ebc*/ MOVW        R2, #1827                   @ 0x723 
    /*00001ec0*/ LDR         R1, [PC, #64]               @ 0x0000000000001f08 
    /*00001ec4*/ BL          0x0000000000001EC4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001ec8*/ B           ChnlGetOneTskForPriority_x1e60
    ChnlGetOneTskForPriority_x1ecc:
    /*00001ecc*/ ADD         R2, SL, #16 
    /*00001ed0*/ MOV         R0, #0 
    /*00001ed4*/ LDR         R2, [FP, R2, LSL #2] 
    ChnlGetOneTskForPriority_x1ed8:
    /*00001ed8*/ CMP         R3, R2 
    /*00001edc*/ ADDLE       SL, SL, #16 
    /*00001ee0*/ MOVLE       R3, #0 
    /*00001ee4*/ STRLE       R3, [FP, SL, LSL #2] 
    /*00001ee8*/ POP         {R3, R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    ChnlGetOneTskForPriority_x1eec:
    /*00001eec*/ MOV         R0, R8 
    /*00001ef0*/ POP         {R3, R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    ChnlGetOneTskForPriority_x1ef4:
    /*00001ef4*/ ADD         R2, SL, #16 
    /*00001ef8*/ MOV         R3, R8 
    /*00001efc*/ MOV         R0, #0 
    /*00001f00*/ LDR         R2, [FP, R2, LSL #2] 
    /*00001f04*/ B           ChnlGetOneTskForPriority_x1ed8
    /*00001f08*/ ANDEQ       R0, R0, R4, ASR #3 
FUNC_END ChnlGetOneTskForPriority


FUNC_BEGIN ChnlQueryOneTsk
    /*00001f0c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00001f10*/ MOVW        R9, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00001f14*/ LDR         FP, [PC, #224]              @ 0x0000000000001ffc 
    /*00001f18*/ SUB         SP, SP, #12 
    /*00001f1c*/ MOVW        R8, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00001f20*/ MOVT        R9, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*00001f24*/ MOVT        R8, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00001f28*/ MOV         R6, R0 
    /*00001f2c*/ MOV         SL, R1 
    /*00001f30*/ MOV         R5, #0 
    /*00001f34*/ SUB         R3, R0, #40                 @ 0x28 
    /*00001f38*/ STR         R2, [SP] 
    /*00001f3c*/ STR         R3, [SP, #4] 
    ChnlQueryOneTsk_x1f40:
    /*00001f40*/ LDR         R7, [R6, #64]               @ 0x40 
    /*00001f44*/ CMP         R7, #0 
    /*00001f48*/ BLE         ChnlQueryOneTsk_x1fe0
    /*00001f4c*/ MOV         R3, R7 
    /*00001f50*/ MOV         R4, #0 
    /*00001f54*/ B           ChnlQueryOneTsk_x1f94
    ChnlQueryOneTsk_x1f58:
    /*00001f58*/ LDR         R3, [R6, #60]               @ 0x3c 
    /*00001f5c*/ SUB         R3, R3, R4 
    /*00001f60*/ SUBS        R3, R3, #1 
    /*00001f64*/ LDRMI       R2, [R6, #52]               @ 0x34 
    /*00001f68*/ ADDMI       R3, R3, R2 
    /*00001f6c*/ LDR         R2, [R6, #48]               @ 0x30 
    /*00001f70*/ LDR         R5, [R2, R3, LSL #2] 
    ChnlQueryOneTsk_x1f74:
    /*00001f74*/ LDR         R3, [R5, #8] 
    /*00001f78*/ ADD         R4, R4, #1 
    /*00001f7c*/ MOV         R0, R5 
    /*00001f80*/ CMP         R3, SL 
    /*00001f84*/ BEQ         ChnlQueryOneTsk_x1fc8
    ChnlQueryOneTsk_x1f88:
    /*00001f88*/ CMP         R7, R4 
    /*00001f8c*/ BEQ         ChnlQueryOneTsk_x1fe0
    /*00001f90*/ LDR         R3, [R6, #64]               @ 0x40 
    ChnlQueryOneTsk_x1f94:
    /*00001f94*/ CMP         R4, R3 
    /*00001f98*/ MOV         R1, FP 
    /*00001f9c*/ MOV         R0, R8 
    /*00001fa0*/ MOVGE       R2, #0 
    /*00001fa4*/ MOVLT       R2, #1 
    /*00001fa8*/ CMP         R3, #0 
    /*00001fac*/ MOV         R3, R9 
    /*00001fb0*/ MOVLE       R2, #0 
    /*00001fb4*/ CMP         R2, #0 
    /*00001fb8*/ MOVW        R2, #1880                   @ 0x758 
    /*00001fbc*/ BNE         ChnlQueryOneTsk_x1f58
    /*00001fc0*/ BL          0x0000000000001FC0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001fc4*/ B           ChnlQueryOneTsk_x1f74
    ChnlQueryOneTsk_x1fc8:
    /*00001fc8*/ LDR         R3, [R5] 
    /*00001fcc*/ LDR         R2, [SP] 
    /*00001fd0*/ CMP         R3, R2 
    /*00001fd4*/ BNE         ChnlQueryOneTsk_x1f88
    /*00001fd8*/ ADD         SP, SP, #12 
    /*00001fdc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    ChnlQueryOneTsk_x1fe0:
    /*00001fe0*/ LDR         R3, [SP, #4] 
    /*00001fe4*/ SUB         R6, R6, #20 
    /*00001fe8*/ CMP         R3, R6 
    /*00001fec*/ BNE         ChnlQueryOneTsk_x1f40
    /*00001ff0*/ MOV         R0, #0 
    /*00001ff4*/ ADD         SP, SP, #12 
    /*00001ff8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    /*00001ffc*/ ANDEQ       R0, R0, R0, ROR #3 
FUNC_END ChnlQueryOneTsk


FUNC_BEGIN ChnlDelOneTask
    /*00002000*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00002004*/ SUBS        R4, R0, #0 
    /*00002008*/ MOV         R5, R1 
    /*0000200c*/ BEQ         ChnlDelOneTask_x216c
    ChnlDelOneTask_x2010:
    /*00002010*/ CMP         R5, #0 
    /*00002014*/ BEQ         ChnlDelOneTask_x218c
    ChnlDelOneTask_x2018:
    /*00002018*/ MOV         R0, R4 
    /*0000201c*/ BL          0x000000000000201C          @ ChnlGetSchedTabLen [R_ARM_CALL: .text+0xcf8] 
    /*00002020*/ CMP         R0, #0 
    /*00002024*/ BEQ         ChnlDelOneTask_x2164
    /*00002028*/ MOV         R8, #0 
    /*0000202c*/ MOV         R2, #1 
    /*00002030*/ MOV         R3, #7 
    /*00002034*/ STR         R2, [R5, #48]               @ 0x30 
    /*00002038*/ STR         R3, [R5, #8] 
    /*0000203c*/ ADD         R0, R5, #156                @ 0x9c 
    /*00002040*/ STR         R8, [R5, #16] 
    /*00002044*/ ADD         R7, R4, #64                 @ 0x40 
    /*00002048*/ STR         R8, [R5, #28] 
    /*0000204c*/ ADD         R9, R4, #40                 @ 0x28 
    /*00002050*/ STR         R8, [R5, #24] 
    /*00002054*/ STR         R8, [R5, #20] 
    /*00002058*/ STR         R8, [R5, #44]               @ 0x2c 
    /*0000205c*/ STR         R8, [R5, #52]               @ 0x34 
    /*00002060*/ STR         R8, [R5, #40]               @ 0x28 
    /*00002064*/ STR         R8, [R5, #4] 
    /*00002068*/ BL          0x0000000000002068          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*0000206c*/ MOV         R2, R4 
    ChnlDelOneTask_x2070:
    /*00002070*/ LDR         R6, [R2, #36]               @ 0x24 
    /*00002074*/ SUBS        R0, R6, #0 
    /*00002078*/ BLE         ChnlDelOneTask_x2140
    /*0000207c*/ MOV         R1, #0 
    /*00002080*/ B           ChnlDelOneTask_x20b8
    ChnlDelOneTask_x2084:
    /*00002084*/ LDR         SL, [R2, #24] 
    /*00002088*/ CMP         IP, SL 
    /*0000208c*/ MOVGE       R0, IP 
    /*00002090*/ BGE         ChnlDelOneTask_x20ac
    /*00002094*/ LDR         R3, [R2, #28] 
    /*00002098*/ SUBS        R3, R3, #1 
    /*0000209c*/ ADDMI       R3, R3, SL 
    /*000020a0*/ STR         LR, [R5, R3, LSL #2] 
    /*000020a4*/ STR         R3, [R2, #28] 
    /*000020a8*/ STR         R0, [R2, #36]               @ 0x24 
    ChnlDelOneTask_x20ac:
    /*000020ac*/ ADD         R1, R1, #1 
    /*000020b0*/ CMP         R6, R1 
    /*000020b4*/ BEQ         ChnlDelOneTask_x2140
    ChnlDelOneTask_x20b8:
    /*000020b8*/ CMP         R0, #0 
    /*000020bc*/ BLE         ChnlDelOneTask_x20ac
    /*000020c0*/ LDR         R3, [R2, #32] 
    /*000020c4*/ LDR         R5, [R2, #20] 
    /*000020c8*/ SUBS        R3, R3, #1 
    /*000020cc*/ LDRMI       IP, [R2, #24] 
    /*000020d0*/ ADDMI       R3, R3, IP 
    /*000020d4*/ SUB         IP, R0, #1 
    /*000020d8*/ LDR         LR, [R5, R3, LSL #2] 
    /*000020dc*/ STR         R3, [R2, #32] 
    /*000020e0*/ STR         IP, [R2, #36]               @ 0x24 
    /*000020e4*/ LDR         R3, [LR, #48]               @ 0x30 
    /*000020e8*/ CMP         R3, #1 
    /*000020ec*/ BNE         ChnlDelOneTask_x2084
    /*000020f0*/ LDR         R3, [R4, #16] 
    /*000020f4*/ LDR         IP, [R4, #4] 
    /*000020f8*/ CMP         R3, IP 
    /*000020fc*/ BGE         ChnlDelOneTask_x2120
    /*00002100*/ LDR         R0, [R4, #8] 
    /*00002104*/ ADD         R3, R3, #1 
    /*00002108*/ SUBS        R0, R0, #1 
    /*0000210c*/ ADDMI       R0, R0, IP 
    /*00002110*/ LDR         IP, [R4] 
    /*00002114*/ STR         LR, [IP, R0, LSL #2] 
    /*00002118*/ STR         R3, [R4, #16] 
    /*0000211c*/ STR         R0, [R4, #8] 
    ChnlDelOneTask_x2120:
    /*00002120*/ LDR         R3, [R7] 
    /*00002124*/ CMP         R3, R1 
    /*00002128*/ ADD         R1, R1, #1 
    /*0000212c*/ SUBGT       R3, R3, #1 
    /*00002130*/ STRGT       R3, [R7] 
    /*00002134*/ CMP         R6, R1 
    /*00002138*/ LDR         R0, [R2, #36]               @ 0x24 
    /*0000213c*/ BNE         ChnlDelOneTask_x20b8
    ChnlDelOneTask_x2140:
    /*00002140*/ LDR         R3, [R7] 
    /*00002144*/ ADD         R2, R2, #20 
    /*00002148*/ ADD         R7, R7, #4 
    /*0000214c*/ CMP         R3, R0 
    /*00002150*/ STRGE       R8, [R7, #-4] 
    /*00002154*/ CMP         R9, R2 
    /*00002158*/ BNE         ChnlDelOneTask_x2070
    /*0000215c*/ MOV         R0, #0 
    /*00002160*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    ChnlDelOneTask_x2164:
    /*00002164*/ MVN         R0, #0 
    /*00002168*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    ChnlDelOneTask_x216c:
    /*0000216c*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7cc] 
    /*00002170*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00002174*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7cc] 
    /*00002178*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*0000217c*/ MOVW        R2, #1918                   @ 0x77e 
    /*00002180*/ LDR         R1, [PC, #36]               @ 0x00000000000021ac 
    /*00002184*/ BL          0x0000000000002184          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002188*/ B           ChnlDelOneTask_x2010
    ChnlDelOneTask_x218c:
    /*0000218c*/ MOVW        R3, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb04] 
    /*00002190*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00002194*/ MOVT        R3, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb04] 
    /*00002198*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*0000219c*/ MOVW        R2, #1919                   @ 0x77f 
    /*000021a0*/ LDR         R1, [PC, #4]                @ 0x00000000000021ac 
    /*000021a4*/ BL          0x00000000000021A4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000021a8*/ B           ChnlDelOneTask_x2018
    /*000021ac*/ STRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
FUNC_END ChnlDelOneTask


FUNC_BEGIN ChnlIsTblFull
    /*000021b0*/ PUSH        {R4, LR} 
    /*000021b4*/ SUBS        R4, R0, #0 
    /*000021b8*/ BEQ         ChnlIsTblFull_x21d0
    ChnlIsTblFull_x21bc:
    /*000021bc*/ LDR         R0, [R4, #16] 
    /*000021c0*/ CMP         R0, #0 
    /*000021c4*/ MOVGT       R0, #0 
    /*000021c8*/ MOVLE       R0, #1 
    /*000021cc*/ POP         {R4, PC} 
    ChnlIsTblFull_x21d0:
    /*000021d0*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7cc] 
    /*000021d4*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000021d8*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7cc] 
    /*000021dc*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000021e0*/ MOVW        R2, #1999                   @ 0x7cf 
    /*000021e4*/ LDR         R1, [PC, #4]                @ 0x00000000000021f0 
    /*000021e8*/ BL          0x00000000000021E8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000021ec*/ B           ChnlIsTblFull_x21bc
    /*000021f0*/ ANDEQ       R0, R0, R0, LSL #4 
FUNC_END ChnlIsTblFull


FUNC_BEGIN ChnlAddOneTask
    /*000021f4*/ LDR         R2, [R0, #16] 
    /*000021f8*/ CMP         R2, #0 
    /*000021fc*/ BLE         ChnlAddOneTask_x22b4
    /*00002200*/ PUSH        {R4, R5, R6, LR} 
    /*00002204*/ MOV         R4, R0 
    /*00002208*/ LDR         R3, [R0, #12] 
    /*0000220c*/ MOV         R5, R1 
    /*00002210*/ SUBS        R3, R3, #1 
    /*00002214*/ LDRMI       IP, [R0, #4] 
    /*00002218*/ ADDMI       R3, R3, IP 
    /*0000221c*/ LDR         IP, [R0] 
    /*00002220*/ SUB         R0, R2, #1 
    /*00002224*/ MOV         R2, #160                    @ 0xa0 
    /*00002228*/ LDR         R6, [IP, R3, LSL #2] 
    /*0000222c*/ STR         R3, [R4, #12] 
    /*00002230*/ STR         R0, [R4, #16] 
    /*00002234*/ MOV         R0, R6 
    /*00002238*/ BL          0x0000000000002238          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000223c*/ ADD         R0, R6, #156                @ 0x9c 
    /*00002240*/ BL          0x0000000000002240          @ osal_wait_init [R_ARM_CALL: +0x0] 
    /*00002244*/ LDR         R0, [R5, #4] 
    /*00002248*/ MOV         R3, #20 
    /*0000224c*/ MLA         R3, R3, R0, R4 
    /*00002250*/ LDR         R2, [R3, #36]               @ 0x24 
    /*00002254*/ LDR         R1, [R3, #24] 
    /*00002258*/ CMP         R2, R1 
    /*0000225c*/ BGE         ChnlAddOneTask_x2290
    /*00002260*/ LDR         R3, [R3, #28] 
    /*00002264*/ ADD         R2, R2, #1 
    /*00002268*/ SUBS        R3, R3, #1 
    /*0000226c*/ ADDMI       R3, R3, R1 
    /*00002270*/ MOV         R1, #20 
    /*00002274*/ MLA         R4, R1, R0, R4 
    /*00002278*/ MOV         R0, #0 
    /*0000227c*/ LDR         R1, [R4, #20] 
    /*00002280*/ STR         R6, [R1, R3, LSL #2] 
    /*00002284*/ STR         R2, [R4, #36]               @ 0x24 
    /*00002288*/ STR         R3, [R4, #28] 
    /*0000228c*/ POP         {R4, R5, R6, PC} 
    ChnlAddOneTask_x2290:
    /*00002290*/ MOVW        R2, #2036                   @ 0x7f4 
    /*00002294*/ LDR         R1, [PC, #32]               @ 0x00000000000022bc 
    /*00002298*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*0000229c*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000022a0*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*000022a4*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000022a8*/ BL          0x00000000000022A8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000022ac*/ MOV         R0, #0 
    /*000022b0*/ POP         {R4, R5, R6, PC} 
    ChnlAddOneTask_x22b4:
    /*000022b4*/ MVN         R0, #0 
    /*000022b8*/ BX          LR 
    /*000022bc*/ ANDEQ       R0, R0, R0, LSL R2 
FUNC_END ChnlAddOneTask


FUNC_BEGIN CHNL_SetSchedule
    /*000022c0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000022c4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000022c8*/ ADD         R1, R3, #4800               @ 0x12c0 
    CHNL_SetSchedule_x22cc:
    /*000022cc*/ LDR         R2, [R3, #952]              @ 0x3b8 
    /*000022d0*/ ADD         R3, R3, #960                @ 0x3c0 
    /*000022d4*/ CMP         R2, #1 
    /*000022d8*/ STREQ       R0, [R3, #-860]             @ 0xfffffca4 
    /*000022dc*/ CMP         R3, R1 
    /*000022e0*/ BNE         CHNL_SetSchedule_x22cc
    /*000022e4*/ MOV         R0, #0 
    /*000022e8*/ BX          LR 
FUNC_END CHNL_SetSchedule


FUNC_BEGIN CHNL_GetSchedule
    /*000022ec*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000022f0*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000022f4*/ PUSH        {R4, LR} 
    /*000022f8*/ ADD         R4, R2, #4800               @ 0x12c0 
    /*000022fc*/ MOV         R3, #0 
    /*00002300*/ STR         R3, [R0] 
    CHNL_GetSchedule_x2304:
    /*00002304*/ LDR         R3, [R2, #952]              @ 0x3b8 
    /*00002308*/ CMP         R3, #1 
    /*0000230c*/ BEQ         CHNL_GetSchedule_x2324
    CHNL_GetSchedule_x2310:
    /*00002310*/ ADD         R2, R2, #960                @ 0x3c0 
    /*00002314*/ CMP         R2, R4 
    /*00002318*/ BNE         CHNL_GetSchedule_x2304
    /*0000231c*/ MOV         R0, #0 
    /*00002320*/ POP         {R4, PC} 
    CHNL_GetSchedule_x2324:
    /*00002324*/ LDR         R1, [R2, #100]              @ 0x64 
    /*00002328*/ LDR         R3, [R0] 
    /*0000232c*/ ORR         R3, R3, R1 
    /*00002330*/ STR         R3, [R0] 
    /*00002334*/ LDR         R1, [R2, #108]              @ 0x6c 
    /*00002338*/ CMP         R1, #0 
    /*0000233c*/ BLE         CHNL_GetSchedule_x2310
    /*00002340*/ MOV         IP, R2 
    /*00002344*/ MOV         R1, #0 
    CHNL_GetSchedule_x2348:
    /*00002348*/ LDR         LR, [IP, #120]              @ 0x78 
    /*0000234c*/ ADD         R1, R1, #1 
    /*00002350*/ ADD         IP, IP, #208                @ 0xd0 
    /*00002354*/ CMP         LR, #0 
    /*00002358*/ ORRNE       R3, R3, #1 
    /*0000235c*/ STR         R3, [R0] 
    /*00002360*/ LDR         LR, [R2, #108]              @ 0x6c 
    /*00002364*/ CMP         LR, R1 
    /*00002368*/ BGT         CHNL_GetSchedule_x2348
    /*0000236c*/ B           CHNL_GetSchedule_x2310
FUNC_END CHNL_GetSchedule


FUNC_BEGIN CHNL_StopSchedule
    /*00002370*/ CMP         R0, #6 
    /*00002374*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00002378*/ MOV         R5, R0 
    /*0000237c*/ SUB         SP, SP, #16 
    /*00002380*/ MOV         R7, R1 
    /*00002384*/ BHI         CHNL_StopSchedule_x24f0
    /*00002388*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000238c*/ MOV         R3, #1 
    /*00002390*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002394*/ LSL         R3, R3, R0 
    /*00002398*/ MOV         R1, #0 
    /*0000239c*/ MOV         R2, R4 
    CHNL_StopSchedule_x23a0:
    /*000023a0*/ LDR         R0, [R2, #952]              @ 0x3b8 
    /*000023a4*/ CMP         R0, #1 
    /*000023a8*/ BEQ         CHNL_StopSchedule_x2408
    CHNL_StopSchedule_x23ac:
    /*000023ac*/ ADD         R1, R1, #1 
    /*000023b0*/ ADD         R2, R2, #960                @ 0x3c0 
    /*000023b4*/ CMP         R1, #5 
    /*000023b8*/ BNE         CHNL_StopSchedule_x23a0
    /*000023bc*/ MOV         R3, R5 
    /*000023c0*/ MOVW        R2, #2167                   @ 0x877 
    /*000023c4*/ STR         R7, [SP] 
    /*000023c8*/ MOVW        R0, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc68] 
    /*000023cc*/ LDR         R1, [PC, #416]              @ 0x0000000000002574 
    /*000023d0*/ MOVT        R0, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc68] 
    /*000023d4*/ BL          0x00000000000023D4          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000023d8*/ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcb0] 
    /*000023dc*/ MOVT        R2, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcb0] 
    /*000023e0*/ MOVW        R3, #2168                   @ 0x878 
    /*000023e4*/ STR         R3, [SP] 
    CHNL_StopSchedule_x23e8:
    /*000023e8*/ LDR         R3, [PC, #388]              @ 0x0000000000002574 
    /*000023ec*/ MOV         R1, #4 
    /*000023f0*/ MOV         R0, #3 
    /*000023f4*/ BL          0x00000000000023F4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000023f8*/ MOVW        R0, #32771                  @ 0x8003 
    /*000023fc*/ MOVT        R0, #40964                  @ 0xa004 
    CHNL_StopSchedule_x2400:
    /*00002400*/ ADD         SP, SP, #16 
    /*00002404*/ POP         {R4, R5, R6, R7, R8, PC} 
    CHNL_StopSchedule_x2408:
    /*00002408*/ LDR         R0, [R2, #4] 
    /*0000240c*/ TST         R3, R0 
    /*00002410*/ BEQ         CHNL_StopSchedule_x23ac
    /*00002414*/ MOV         R8, #960                    @ 0x3c0 
    /*00002418*/ MUL         R8, R8, R1 
    /*0000241c*/ ADD         R1, SP, #12 
    /*00002420*/ ADD         R6, R8, #112                @ 0x70 
    /*00002424*/ ADD         R6, R4, R6 
    /*00002428*/ MOV         R0, R6 
    /*0000242c*/ BL          0x000000000000242C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00002430*/ ADD         R0, R4, R8 
    /*00002434*/ MOV         R2, R7 
    /*00002438*/ MOV         R1, R5 
    /*0000243c*/ BL          0x000000000000243C          @ ChnlQueryOneTsk [R_ARM_CALL: .text+0x1f0c] 
    /*00002440*/ SUBS        R4, R0, #0 
    /*00002444*/ BEQ         CHNL_StopSchedule_x2520
    /*00002448*/ LDR         R3, [R4, #48]               @ 0x30 
    /*0000244c*/ CMP         R3, #1 
    /*00002450*/ BEQ         CHNL_StopSchedule_x24cc
    CHNL_StopSchedule_x2454:
    /*00002454*/ CMP         R3, #6 
    /*00002458*/ ADD         R1, SP, #12 
    /*0000245c*/ MOV         R0, R6 
    /*00002460*/ BEQ         CHNL_StopSchedule_x24c0
    /*00002464*/ CMP         R3, #2 
    /*00002468*/ BEQ         CHNL_StopSchedule_x24b8
    /*0000246c*/ MOV         R3, #1 
    /*00002470*/ STR         R3, [R4, #152]              @ 0x98 
    /*00002474*/ MOVW        R5, #0                      @ CHNL_WaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x89c] 
    /*00002478*/ BL          0x0000000000002478          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000247c*/ ADD         R6, R4, #156                @ 0x9c 
    /*00002480*/ MOVT        R5, #0                      @ CHNL_WaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x89c] 
    /*00002484*/ B           CHNL_StopSchedule_x2494
    CHNL_StopSchedule_x2488:
    /*00002488*/ BL          0x0000000000002488          @ osal_wait_uninterruptible [R_ARM_CALL: +0x0] 
    /*0000248c*/ CMP         R0, #0 
    /*00002490*/ BLT         CHNL_StopSchedule_x24ac
    CHNL_StopSchedule_x2494:
    /*00002494*/ LDR         R3, [R4, #48]               @ 0x30 
    /*00002498*/ MOV         R2, R4 
    /*0000249c*/ MOV         R1, R5 
    /*000024a0*/ MOV         R0, R6 
    /*000024a4*/ CMP         R3, #6 
    /*000024a8*/ BNE         CHNL_StopSchedule_x2488
    CHNL_StopSchedule_x24ac:
    /*000024ac*/ MOV         R0, #0 
    CHNL_StopSchedule_x24b0:
    /*000024b0*/ ADD         SP, SP, #16 
    /*000024b4*/ POP         {R4, R5, R6, R7, R8, PC} 
    CHNL_StopSchedule_x24b8:
    /*000024b8*/ MOV         R3, #6 
    /*000024bc*/ STR         R3, [R4, #48]               @ 0x30 
    CHNL_StopSchedule_x24c0:
    /*000024c0*/ BL          0x00000000000024C0          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000024c4*/ MOV         R0, #0 
    /*000024c8*/ B           CHNL_StopSchedule_x24b0
    CHNL_StopSchedule_x24cc:
    /*000024cc*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc44] 
    /*000024d0*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000024d4*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc44] 
    /*000024d8*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000024dc*/ MOVW        R2, #2187                   @ 0x88b 
    /*000024e0*/ LDR         R1, [PC, #140]              @ 0x0000000000002574 
    /*000024e4*/ BL          0x00000000000024E4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000024e8*/ LDR         R3, [R4, #48]               @ 0x30 
    /*000024ec*/ B           CHNL_StopSchedule_x2454
    CHNL_StopSchedule_x24f0:
    /*000024f0*/ MOV         R3, R0 
    /*000024f4*/ STR         R1, [SP] 
    /*000024f8*/ MOVW        R2, #2157                   @ 0x86d 
    /*000024fc*/ MOVW        R0, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb18] 
    /*00002500*/ LDR         R1, [PC, #108]              @ 0x0000000000002574 
    /*00002504*/ MOVT        R0, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb18] 
    /*00002508*/ BL          0x0000000000002508          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000250c*/ MOVW        R3, #2158                   @ 0x86e 
    /*00002510*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb60] 
    /*00002514*/ STR         R3, [SP] 
    /*00002518*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb60] 
    /*0000251c*/ B           CHNL_StopSchedule_x23e8
    CHNL_StopSchedule_x2520:
    /*00002520*/ MOV         R3, R5 
    /*00002524*/ MOVW        R2, #2178                   @ 0x882 
    /*00002528*/ LDR         R1, [PC, #68]               @ 0x0000000000002574 
    /*0000252c*/ MOVW        R0, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbac] 
    /*00002530*/ STR         R7, [SP] 
    /*00002534*/ MOVT        R0, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbac] 
    /*00002538*/ BL          0x0000000000002538          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000253c*/ MOVW        R3, #2180                   @ 0x884 
    /*00002540*/ MOV         R1, #4 
    /*00002544*/ STR         R3, [SP] 
    /*00002548*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbfc] 
    /*0000254c*/ LDR         R3, [PC, #32]               @ 0x0000000000002574 
    /*00002550*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbfc] 
    /*00002554*/ MOV         R0, #3 
    /*00002558*/ BL          0x0000000000002558          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000255c*/ MOV         R0, R6 
    /*00002560*/ ADD         R1, SP, #12 
    /*00002564*/ BL          0x0000000000002564          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002568*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000256c*/ MOVT        R0, #40964                  @ 0xa004 
    /*00002570*/ B           CHNL_StopSchedule_x2400
    /*00002574*/ ANDEQ       R0, R0, R0, LSR #4 
FUNC_END CHNL_StopSchedule


FUNC_BEGIN CHNL_SetTaskPriority
    /*00002578*/ CMP         R1, #6 
    /*0000257c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00002580*/ SUB         SP, SP, #20 
    /*00002584*/ MOVWHI      R3, #2226                   @ 0x8b2 
    /*00002588*/ MOVWHI      R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb60] 
    /*0000258c*/ STRHI       R3, [SP] 
    /*00002590*/ MOVTHI      R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb60] 
    /*00002594*/ BHI         CHNL_SetTaskPriority_x25ec
    /*00002598*/ MOV         SL, R2 
    /*0000259c*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000025a0*/ MOV         R2, #1 
    /*000025a4*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000025a8*/ LSL         R2, R2, R1 
    /*000025ac*/ MOV         R8, R3 
    /*000025b0*/ MOV         R7, R1 
    /*000025b4*/ MOV         R9, R0 
    /*000025b8*/ MOV         R4, #0 
    /*000025bc*/ MOV         R3, R6 
    CHNL_SetTaskPriority_x25c0:
    /*000025c0*/ LDR         R1, [R3, #952]              @ 0x3b8 
    /*000025c4*/ CMP         R1, #1 
    /*000025c8*/ BEQ         CHNL_SetTaskPriority_x260c
    CHNL_SetTaskPriority_x25cc:
    /*000025cc*/ ADD         R4, R4, #1 
    /*000025d0*/ ADD         R3, R3, #960                @ 0x3c0 
    /*000025d4*/ CMP         R4, #5 
    /*000025d8*/ BNE         CHNL_SetTaskPriority_x25c0
    /*000025dc*/ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcb0] 
    /*000025e0*/ MOVT        R2, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcb0] 
    /*000025e4*/ MOVW        R3, #2236                   @ 0x8bc 
    /*000025e8*/ STR         R3, [SP] 
    CHNL_SetTaskPriority_x25ec:
    /*000025ec*/ LDR         R3, [PC, #284]              @ 0x0000000000002710 
    /*000025f0*/ MOV         R1, #4 
    /*000025f4*/ MOV         R0, #3 
    /*000025f8*/ BL          0x00000000000025F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000025fc*/ MOVW        R0, #32771                  @ 0x8003 
    /*00002600*/ MOVT        R0, #40964                  @ 0xa004 
    /*00002604*/ ADD         SP, SP, #20 
    /*00002608*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    CHNL_SetTaskPriority_x260c:
    /*0000260c*/ LDR         R1, [R3, #4] 
    /*00002610*/ TST         R2, R1 
    /*00002614*/ BEQ         CHNL_SetTaskPriority_x25cc
    /*00002618*/ MOV         R5, #960                    @ 0x3c0 
    /*0000261c*/ ADD         R1, SP, #12 
    /*00002620*/ MUL         R5, R5, R4 
    /*00002624*/ ADD         FP, R5, #112                @ 0x70 
    /*00002628*/ ADD         FP, R6, FP 
    /*0000262c*/ MOV         R0, FP 
    /*00002630*/ BL          0x0000000000002630          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00002634*/ ADD         R0, R5, #8 
    /*00002638*/ MOV         R1, R7 
    /*0000263c*/ MOV         R3, SL 
    /*00002640*/ MOV         R2, R9 
    /*00002644*/ ADD         R0, R6, R0 
    /*00002648*/ BL          0x0000000000002648          @ ChnlGetOneTskForPriority [R_ARM_CALL: .text+0x1dbc] 
    /*0000264c*/ SUBS        R7, R0, #0 
    /*00002650*/ BEQ         CHNL_SetTaskPriority_x26c0
    /*00002654*/ MOV         R3, #20 
    /*00002658*/ MLA         R3, R3, R8, R5 
    /*0000265c*/ ADD         R3, R6, R3 
    /*00002660*/ LDR         R2, [R3, #44]               @ 0x2c 
    /*00002664*/ LDR         R0, [R3, #32] 
    /*00002668*/ CMP         R2, R0 
    /*0000266c*/ BLT         CHNL_SetTaskPriority_x26d8
    /*00002670*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00002674*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00002678*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*0000267c*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00002680*/ MOVW        R2, #2248                   @ 0x8c8 
    /*00002684*/ LDR         R1, [PC, #132]              @ 0x0000000000002710 
    /*00002688*/ BL          0x0000000000002688          @ osal_panic [R_ARM_CALL: +0x0] 
    CHNL_SetTaskPriority_x268c:
    /*0000268c*/ ADD         R5, R6, R5 
    /*00002690*/ STR         R8, [R7, #4] 
    /*00002694*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*00002698*/ LDR         R2, [R5, #64]               @ 0x40 
    /*0000269c*/ CMP         R3, #0 
    /*000026a0*/ MOVGT       R3, #1 
    /*000026a4*/ MOVLE       R3, #0 
    /*000026a8*/ CMP         R2, #0 
    /*000026ac*/ ADDGT       R3, R3, #1 
    /*000026b0*/ CMP         R3, #1 
    /*000026b4*/ MOVLE       R3, #0 
    /*000026b8*/ MOVGT       R3, #1 
    /*000026bc*/ STR         R3, [R5, #92]               @ 0x5c 
    CHNL_SetTaskPriority_x26c0:
    /*000026c0*/ MOV         R0, FP 
    /*000026c4*/ ADD         R1, SP, #12 
    /*000026c8*/ BL          0x00000000000026C8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000026cc*/ MOV         R0, #0 
    /*000026d0*/ ADD         SP, SP, #20 
    /*000026d4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    CHNL_SetTaskPriority_x26d8:
    /*000026d8*/ LDR         R1, [R3, #36]               @ 0x24 
    /*000026dc*/ MOV         R3, #20 
    /*000026e0*/ ADD         R2, R2, #1 
    /*000026e4*/ SUBS        R1, R1, #1 
    /*000026e8*/ ADDMI       R1, R1, R0 
    /*000026ec*/ MOV         R0, #960                    @ 0x3c0 
    /*000026f0*/ MUL         R4, R0, R4 
    /*000026f4*/ MLA         R4, R3, R8, R4 
    /*000026f8*/ ADD         R4, R6, R4 
    /*000026fc*/ LDR         R3, [R4, #28] 
    /*00002700*/ STR         R7, [R3, R1, LSL #2] 
    /*00002704*/ STR         R2, [R4, #44]               @ 0x2c 
    /*00002708*/ STR         R1, [R4, #36]               @ 0x24 
    /*0000270c*/ B           CHNL_SetTaskPriority_x268c
    /*00002710*/ ANDEQ       R0, R0, R4, LSR R2 
FUNC_END CHNL_SetTaskPriority


FUNC_BEGIN CHNL_ResumeSchedule
    /*00002714*/ CMP         R0, #6 
    /*00002718*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000271c*/ MOVWHI      R3, #2293                   @ 0x8f5 
    /*00002720*/ SUB         SP, SP, #16 
    /*00002724*/ BHI         CHNL_ResumeSchedule_x2768
    /*00002728*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000272c*/ MOV         IP, #1 
    /*00002730*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002734*/ LSL         IP, IP, R0 
    /*00002738*/ MOV         R8, R1 
    /*0000273c*/ MOV         R7, R0 
    /*00002740*/ MOV         R1, #0 
    /*00002744*/ MOV         R3, R4 
    CHNL_ResumeSchedule_x2748:
    /*00002748*/ LDR         R0, [R3, #952]              @ 0x3b8 
    /*0000274c*/ CMP         R0, #1 
    /*00002750*/ BEQ         CHNL_ResumeSchedule_x2794
    CHNL_ResumeSchedule_x2754:
    /*00002754*/ ADD         R1, R1, #1 
    /*00002758*/ ADD         R3, R3, #960                @ 0x3c0 
    /*0000275c*/ CMP         R1, #5 
    /*00002760*/ BNE         CHNL_ResumeSchedule_x2748
    /*00002764*/ MOVW        R3, #2311                   @ 0x907 
    CHNL_ResumeSchedule_x2768:
    /*00002768*/ STR         R3, [SP] 
    /*0000276c*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb60] 
    /*00002770*/ LDR         R3, [PC, #248]              @ 0x0000000000002870 
    /*00002774*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb60] 
    /*00002778*/ MOV         R1, #4 
    /*0000277c*/ MOV         R0, #3 
    /*00002780*/ BL          0x0000000000002780          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002784*/ MOVW        R0, #32771                  @ 0x8003 
    /*00002788*/ MOVT        R0, #40964                  @ 0xa004 
    CHNL_ResumeSchedule_x278c:
    /*0000278c*/ ADD         SP, SP, #16 
    /*00002790*/ POP         {R4, R5, R6, R7, R8, PC} 
    CHNL_ResumeSchedule_x2794:
    /*00002794*/ LDR         R2, [R3, #4] 
    /*00002798*/ TST         IP, R2 
    /*0000279c*/ BEQ         CHNL_ResumeSchedule_x2754
    /*000027a0*/ MOV         R6, #960                    @ 0x3c0 
    /*000027a4*/ MUL         R6, R6, R1 
    /*000027a8*/ ADD         R1, SP, #12 
    /*000027ac*/ ADD         R5, R6, #112                @ 0x70 
    /*000027b0*/ ADD         R5, R4, R5 
    /*000027b4*/ MOV         R0, R5 
    /*000027b8*/ BL          0x00000000000027B8          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000027bc*/ MOV         R2, R8 
    /*000027c0*/ MOV         R1, R7 
    /*000027c4*/ ADD         R0, R4, R6 
    /*000027c8*/ BL          0x00000000000027C8          @ ChnlQueryOneTsk [R_ARM_CALL: .text+0x1f0c] 
    /*000027cc*/ SUBS        R3, R0, #0 
    /*000027d0*/ BEQ         CHNL_ResumeSchedule_x2838
    /*000027d4*/ LDR         R2, [R3, #48]               @ 0x30 
    /*000027d8*/ CMP         R2, #6 
    /*000027dc*/ BNE         CHNL_ResumeSchedule_x2800
    /*000027e0*/ MOV         R0, R5 
    /*000027e4*/ MOV         R2, #2 
    /*000027e8*/ ADD         R1, SP, #12 
    /*000027ec*/ STR         R2, [R3, #48]               @ 0x30 
    /*000027f0*/ BL          0x00000000000027F0          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000027f4*/ MOV         R0, #0 
    /*000027f8*/ ADD         SP, SP, #16 
    /*000027fc*/ POP         {R4, R5, R6, R7, R8, PC} 
    CHNL_ResumeSchedule_x2800:
    /*00002800*/ MOVW        R3, #2332                   @ 0x91c 
    /*00002804*/ MOV         R1, #4 
    /*00002808*/ STR         R3, [SP] 
    /*0000280c*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd00] 
    /*00002810*/ LDR         R3, [PC, #88]               @ 0x0000000000002870 
    /*00002814*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd00] 
    /*00002818*/ MOV         R0, #3 
    /*0000281c*/ BL          0x000000000000281C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002820*/ MOV         R0, R5 
    /*00002824*/ ADD         R1, SP, #12 
    /*00002828*/ BL          0x0000000000002828          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000282c*/ MOVW        R0, #32777                  @ 0x8009 
    /*00002830*/ MOVT        R0, #40964                  @ 0xa004 
    /*00002834*/ B           CHNL_ResumeSchedule_x278c
    CHNL_ResumeSchedule_x2838:
    /*00002838*/ MOVW        R3, #2323                   @ 0x913 
    /*0000283c*/ MOV         R1, #4 
    /*00002840*/ STR         R3, [SP] 
    /*00002844*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbfc] 
    /*00002848*/ LDR         R3, [PC, #32]               @ 0x0000000000002870 
    /*0000284c*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbfc] 
    /*00002850*/ MOV         R0, #3 
    /*00002854*/ BL          0x0000000000002854          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002858*/ MOV         R0, R5 
    /*0000285c*/ ADD         R1, SP, #12 
    /*00002860*/ BL          0x0000000000002860          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002864*/ MOVW        R0, #32771                  @ 0x8003 
    /*00002868*/ MOVT        R0, #40964                  @ 0xa004 
    /*0000286c*/ B           CHNL_ResumeSchedule_x278c
    /*00002870*/ ANDEQ       R0, R0, IP, ASR #4 
FUNC_END CHNL_ResumeSchedule


FUNC_BEGIN ScheduleAtOnce
    /*00002874*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00002878*/ MOVW        R7, #0                      @ g_proc_enable [R_ARM_MOVW_ABS_NC: +0x0] 
    /*0000287c*/ MOVT        R7, #0                      @ g_proc_enable [R_ARM_MOVT_ABS: +0x0] 
    /*00002880*/ MOVW        R3, #2316                   @ 0x90c 
    /*00002884*/ MOVT        R3, #10 
    /*00002888*/ SUB         SP, SP, #36                 @ 0x24 
    /*0000288c*/ LDR         R2, [R7] 
    /*00002890*/ MOV         R5, R0 
    /*00002894*/ CMP         R2, R3 
    /*00002898*/ BEQ         ScheduleAtOnce_x2ce4
    /*0000289c*/ MOV         R2, #0 
    /*000028a0*/ MOV         R3, #0 
    /*000028a4*/ STRD        R2, [SP, #16] 
    ScheduleAtOnce_x28a8:
    /*000028a8*/ LDR         R3, [R5, #108]              @ 0x6c 
    /*000028ac*/ CMP         R3, #0 
    /*000028b0*/ BLE         ScheduleAtOnce_x2ab0
    /*000028b4*/ MOVW        R9, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x68] 
    /*000028b8*/ MOVW        R8, #2316                   @ 0x90c 
    /*000028bc*/ MOVT        R9, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x68] 
    /*000028c0*/ MOVT        R8, #10 
    /*000028c4*/ ADD         R4, R5, #240                @ 0xf0 
    /*000028c8*/ MOV         R6, #0 
    /*000028cc*/ MOVW        FP, #16960                  @ 0x4240 
    /*000028d0*/ MOVT        FP, #15 
    /*000028d4*/ STR         FP, [SP, #12] 
    ScheduleAtOnce_x28d8:
    /*000028d8*/ LDR         R2, [R4, #-120]             @ 0xffffff88 
    /*000028dc*/ CMP         R2, #1 
    /*000028e0*/ BEQ         ScheduleAtOnce_x2aa0
    /*000028e4*/ LDR         R3, [R9] 
    /*000028e8*/ CMP         R3, #0 
    /*000028ec*/ BEQ         ScheduleAtOnce_x291c
    /*000028f0*/ LDR         R3, [R4, #-92]              @ 0xffffffa4 
    /*000028f4*/ LDR         R3, [R3, #4] 
    /*000028f8*/ TST         R3, #49                     @ 0x31 
    /*000028fc*/ BEQ         ScheduleAtOnce_x291c
    /*00002900*/ LDR         R3, [R4, #-12] 
    /*00002904*/ MOV         R1, #1 
    /*00002908*/ LDR         R0, [R4, #-96]              @ 0xffffffa0 
    /*0000290c*/ BLX         R3 
    /*00002910*/ BL          0x0000000000002910          @ osal_isb [R_ARM_CALL: +0x0] 
    /*00002914*/ BL          0x0000000000002914          @ osal_dsb [R_ARM_CALL: +0x0] 
    /*00002918*/ BL          0x0000000000002918          @ osal_dmb [R_ARM_CALL: +0x0] 
    ScheduleAtOnce_x291c:
    /*0000291c*/ CMP         R4, #120                    @ 0x78 
    /*00002920*/ BEQ         ScheduleAtOnce_x2cc4
    ScheduleAtOnce_x2924:
    /*00002924*/ LDR         R3, [R4, #-92]              @ 0xffffffa4 
    /*00002928*/ LDR         SL, [R3, #96]               @ 0x60 
    /*0000292c*/ CMP         SL, #0 
    /*00002930*/ BEQ         ScheduleAtOnce_x2ad8
    /*00002934*/ LDR         R2, [SL, #48]               @ 0x30 
    /*00002938*/ CMP         R2, #1 
    /*0000293c*/ BEQ         ScheduleAtOnce_x2ad8
    /*00002940*/ SUB         R3, R2, #3 
    /*00002944*/ CMP         R3, #1 
    /*00002948*/ BLS         ScheduleAtOnce_x2994
    /*0000294c*/ LDR         R3, [SL] 
    /*00002950*/ MOVW        R0, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x86c] 
    /*00002954*/ STR         R2, [SP] 
    /*00002958*/ MOVT        R0, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x86c] 
    /*0000295c*/ MOVW        R2, #2389                   @ 0x955 
    /*00002960*/ LDR         R1, [PC, #1048]             @ 0x0000000000002d80 
    /*00002964*/ BL          0x0000000000002964          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00002968*/ LDR         R3, [SL, #48]               @ 0x30 
    /*0000296c*/ SUB         R3, R3, #3 
    /*00002970*/ CMP         R3, #1 
    /*00002974*/ BLS         ScheduleAtOnce_x2994
    /*00002978*/ MOVW        R3, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8a0] 
    /*0000297c*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00002980*/ MOVT        R3, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8a0] 
    /*00002984*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00002988*/ MOVW        R2, #2390                   @ 0x956 
    /*0000298c*/ LDR         R1, [PC, #1004]             @ 0x0000000000002d80 
    /*00002990*/ BL          0x0000000000002990          @ osal_panic [R_ARM_CALL: +0x0] 
    ScheduleAtOnce_x2994:
    /*00002994*/ STR         SL, [R4, #-56]              @ 0xffffffc8 
    /*00002998*/ LDR         R2, [SL, #48]               @ 0x30 
    /*0000299c*/ SUB         R3, R2, #3 
    /*000029a0*/ CMP         R3, #1 
    /*000029a4*/ BLS         ScheduleAtOnce_x29f0
    /*000029a8*/ LDR         R3, [SL] 
    /*000029ac*/ MOVW        R0, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8fc] 
    /*000029b0*/ STR         R2, [SP] 
    /*000029b4*/ MOVT        R0, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8fc] 
    /*000029b8*/ MOV         R2, #628                    @ 0x274 
    /*000029bc*/ LDR         R1, [PC, #960]              @ 0x0000000000002d84 
    /*000029c0*/ BL          0x00000000000029C0          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000029c4*/ LDR         R3, [SL, #48]               @ 0x30 
    /*000029c8*/ SUB         R3, R3, #3 
    /*000029cc*/ CMP         R3, #1 
    /*000029d0*/ BLS         ScheduleAtOnce_x29f0
    /*000029d4*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x92c] 
    /*000029d8*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000029dc*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x92c] 
    /*000029e0*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000029e4*/ MOVW        R2, #629                    @ 0x275 
    /*000029e8*/ LDR         R1, [PC, #916]              @ 0x0000000000002d84 
    /*000029ec*/ BL          0x00000000000029EC          @ osal_panic [R_ARM_CALL: +0x0] 
    ScheduleAtOnce_x29f0:
    /*000029f0*/ LDR         R3, [R4, #-28]              @ 0xffffffe4 
    /*000029f4*/ MVN         R1, #0 
    /*000029f8*/ LDR         R0, [R4, #-96]              @ 0xffffffa0 
    /*000029fc*/ BLX         R3 
    /*00002a00*/ LDR         R2, [SL, #52]               @ 0x34 
    /*00002a04*/ CMP         R2, #0 
    /*00002a08*/ LDRNE       R3, [R4, #-96]              @ 0xffffffa0 
    /*00002a0c*/ STRNE       R3, [R2, #12] 
    /*00002a10*/ LDRNE       R2, [SL, #52]               @ 0x34 
    /*00002a14*/ LDR         R3, [SL, #36]               @ 0x24 
    /*00002a18*/ LDR         R1, [R4, #-52]              @ 0xffffffcc 
    /*00002a1c*/ LDR         R0, [SL] 
    /*00002a20*/ BLX         R3 
    /*00002a24*/ CMP         R0, #0 
    /*00002a28*/ BNE         ScheduleAtOnce_x2ab8
    ScheduleAtOnce_x2a2c:
    /*00002a2c*/ LDR         R3, [R4, #-8] 
    /*00002a30*/ LDR         R0, [R4, #-96]              @ 0xffffffa0 
    /*00002a34*/ BLX         R3 
    /*00002a38*/ LDR         R3, [R7] 
    /*00002a3c*/ CMP         R3, R8 
    /*00002a40*/ BEQ         ScheduleAtOnce_x2c70
    /*00002a44*/ MOV         R2, #0 
    /*00002a48*/ MOV         R3, #0 
    /*00002a4c*/ STR         SL, [R4, #-56]              @ 0xffffffc8 
    /*00002a50*/ STRD        R2, [R4, #72]               @ 0x48 
    ScheduleAtOnce_x2a54:
    /*00002a54*/ MOV         R2, #0 
    /*00002a58*/ MOV         R3, #0 
    /*00002a5c*/ STRD        R2, [SL, #128]              @ 0x80 
    ScheduleAtOnce_x2a60:
    /*00002a60*/ LDR         R3, [R4, #28] 
    /*00002a64*/ ADD         R0, R5, #8 
    /*00002a68*/ MOV         R2, R4 
    ScheduleAtOnce_x2a6c:
    /*00002a6c*/ ADD         R3, R3, #1 
    /*00002a70*/ STR         R3, [R4, #28] 
    /*00002a74*/ LDR         R1, [R5, #100]              @ 0x64 
    /*00002a78*/ BL          0x0000000000002A78          @ ChnlScheduler [R_ARM_CALL: .text+0xe44] 
    /*00002a7c*/ LDR         R3, [R7] 
    /*00002a80*/ CMP         R3, R8 
    /*00002a84*/ STR         R0, [R5, #96]               @ 0x60 
    /*00002a88*/ BEQ         ScheduleAtOnce_x2bd8
    ScheduleAtOnce_x2a8c:
    /*00002a8c*/ LDR         R3, [R4, #-56]              @ 0xffffffc8 
    /*00002a90*/ CMP         R3, #0 
    /*00002a94*/ MOVNE       R3, #1 
    /*00002a98*/ STR         R3, [R4, #-120]             @ 0xffffff88 
    /*00002a9c*/ LDR         R3, [R5, #108]              @ 0x6c 
    ScheduleAtOnce_x2aa0:
    /*00002aa0*/ ADD         R6, R6, #1 
    /*00002aa4*/ ADD         R4, R4, #208                @ 0xd0 
    /*00002aa8*/ CMP         R3, R6 
    /*00002aac*/ BGT         ScheduleAtOnce_x28d8
    ScheduleAtOnce_x2ab0:
    /*00002ab0*/ ADD         SP, SP, #36                 @ 0x24 
    /*00002ab4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    ScheduleAtOnce_x2ab8:
    /*00002ab8*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00002abc*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00002ac0*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*00002ac4*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00002ac8*/ MOVW        R2, #646                    @ 0x286 
    /*00002acc*/ LDR         R1, [PC, #688]              @ 0x0000000000002d84 
    /*00002ad0*/ BL          0x0000000000002AD0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002ad4*/ B           ScheduleAtOnce_x2a2c
    ScheduleAtOnce_x2ad8:
    /*00002ad8*/ ADD         R3, R5, #8 
    /*00002adc*/ MOV         R2, R4 
    /*00002ae0*/ LDR         R1, [R5, #100]              @ 0x64 
    /*00002ae4*/ MOV         R0, R3 
    /*00002ae8*/ MOV         FP, R3 
    /*00002aec*/ BL          0x0000000000002AEC          @ ChnlScheduler [R_ARM_CALL: .text+0xe44] 
    /*00002af0*/ CMP         R0, #0 
    /*00002af4*/ MOV         SL, R0 
    /*00002af8*/ STR         R0, [R4, #-56]              @ 0xffffffc8 
    /*00002afc*/ BEQ         ScheduleAtOnce_x2c18
    /*00002b00*/ LDR         R2, [SL, #48]               @ 0x30 
    /*00002b04*/ SUB         R3, R2, #3 
    /*00002b08*/ CMP         R3, #1 
    /*00002b0c*/ BLS         ScheduleAtOnce_x2b58
    /*00002b10*/ LDR         R3, [SL] 
    /*00002b14*/ MOVW        R0, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8fc] 
    /*00002b18*/ STR         R2, [SP] 
    /*00002b1c*/ MOVT        R0, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8fc] 
    /*00002b20*/ MOV         R2, #628                    @ 0x274 
    /*00002b24*/ LDR         R1, [PC, #600]              @ 0x0000000000002d84 
    /*00002b28*/ BL          0x0000000000002B28          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00002b2c*/ LDR         R3, [SL, #48]               @ 0x30 
    /*00002b30*/ SUB         R3, R3, #3 
    /*00002b34*/ CMP         R3, #1 
    /*00002b38*/ BLS         ScheduleAtOnce_x2b58
    /*00002b3c*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x92c] 
    /*00002b40*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00002b44*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x92c] 
    /*00002b48*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00002b4c*/ MOVW        R2, #629                    @ 0x275 
    /*00002b50*/ LDR         R1, [PC, #556]              @ 0x0000000000002d84 
    /*00002b54*/ BL          0x0000000000002B54          @ osal_panic [R_ARM_CALL: +0x0] 
    ScheduleAtOnce_x2b58:
    /*00002b58*/ LDR         R3, [R4, #-28]              @ 0xffffffe4 
    /*00002b5c*/ MVN         R1, #0 
    /*00002b60*/ LDR         R0, [R4, #-96]              @ 0xffffffa0 
    /*00002b64*/ BLX         R3 
    /*00002b68*/ LDR         R2, [SL, #52]               @ 0x34 
    /*00002b6c*/ CMP         R2, #0 
    /*00002b70*/ LDRNE       R3, [R4, #-96]              @ 0xffffffa0 
    /*00002b74*/ STRNE       R3, [R2, #12] 
    /*00002b78*/ LDRNE       R2, [SL, #52]               @ 0x34 
    /*00002b7c*/ LDR         R3, [SL, #36]               @ 0x24 
    /*00002b80*/ LDR         R1, [R4, #-52]              @ 0xffffffcc 
    /*00002b84*/ LDR         R0, [SL] 
    /*00002b88*/ BLX         R3 
    /*00002b8c*/ CMP         R0, #0 
    /*00002b90*/ BNE         ScheduleAtOnce_x2d0c
    ScheduleAtOnce_x2b94:
    /*00002b94*/ LDR         R3, [R4, #-8] 
    /*00002b98*/ LDR         R0, [R4, #-96]              @ 0xffffffa0 
    /*00002b9c*/ BLX         R3 
    /*00002ba0*/ LDR         R3, [R7] 
    /*00002ba4*/ CMP         R3, R8 
    /*00002ba8*/ BEQ         ScheduleAtOnce_x2d2c
    /*00002bac*/ MOV         R2, #0 
    /*00002bb0*/ MOV         R3, #0 
    /*00002bb4*/ STR         SL, [R4, #-56]              @ 0xffffffc8 
    /*00002bb8*/ STRD        R2, [R4, #72]               @ 0x48 
    ScheduleAtOnce_x2bbc:
    /*00002bbc*/ MOV         R2, #0 
    /*00002bc0*/ MOV         R3, #0 
    /*00002bc4*/ STRD        R2, [SL, #128]              @ 0x80 
    ScheduleAtOnce_x2bc8:
    /*00002bc8*/ LDR         R3, [R4, #28] 
    /*00002bcc*/ MOV         R0, FP 
    /*00002bd0*/ MOV         R2, R4 
    /*00002bd4*/ B           ScheduleAtOnce_x2a6c
    ScheduleAtOnce_x2bd8:
    /*00002bd8*/ ADD         R0, SP, #24 
    /*00002bdc*/ BL          0x0000000000002BDC          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00002be0*/ LDR         R0, [SP, #28] 
    /*00002be4*/ LDR         IP, [SP, #24] 
    /*00002be8*/ LDR         LR, [SP, #12] 
    /*00002bec*/ ASR         R1, R0, #31 
    /*00002bf0*/ LDRD        R2, [R4, #56]               @ 0x38 
    /*00002bf4*/ SMLAL       R0, R1, LR, IP 
    /*00002bf8*/ ADDS        SL, R2, R0 
    /*00002bfc*/ ADC         FP, R3, R1 
    /*00002c00*/ LDRD        R2, [R4, #72]               @ 0x48 
    /*00002c04*/ SUBS        R0, SL, R2 
    /*00002c08*/ SBC         R1, FP, R3 
    /*00002c0c*/ MOV         R2, R0 
    /*00002c10*/ MOV         R3, R1 
    /*00002c14*/ STRD        R2, [R4, #56]               @ 0x38 
    ScheduleAtOnce_x2c18:
    /*00002c18*/ LDR         R3, [R7] 
    /*00002c1c*/ CMP         R3, R8 
    /*00002c20*/ BNE         ScheduleAtOnce_x2a8c
    /*00002c24*/ ADD         R0, SP, #24 
    /*00002c28*/ BL          0x0000000000002C28          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00002c2c*/ LDRD        R2, [R4, #48]               @ 0x30 
    /*00002c30*/ LDR         R0, [SP, #28] 
    /*00002c34*/ LDR         IP, [SP, #24] 
    /*00002c38*/ MOV         FP, R3 
    /*00002c3c*/ LDR         R3, [SP, #12] 
    /*00002c40*/ ASR         R1, R0, #31 
    /*00002c44*/ MOV         SL, R2 
    /*00002c48*/ SMLAL       R0, R1, R3, IP 
    /*00002c4c*/ LDRD        R2, [SP, #16] 
    /*00002c50*/ SUBS        R2, R0, R2 
    /*00002c54*/ SBC         R3, R1, R3 
    /*00002c58*/ MOV         R0, R2 
    /*00002c5c*/ ADDS        R2, SL, R0 
    /*00002c60*/ MOV         R1, R3 
    /*00002c64*/ ADC         R3, FP, R1 
    /*00002c68*/ STRD        R2, [R4, #48]               @ 0x30 
    /*00002c6c*/ B           ScheduleAtOnce_x2a8c
    ScheduleAtOnce_x2c70:
    /*00002c70*/ ADD         R0, SP, #24 
    /*00002c74*/ BL          0x0000000000002C74          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00002c78*/ LDR         R2, [SP, #28] 
    /*00002c7c*/ LDR         R1, [SP, #24] 
    /*00002c80*/ LDR         R0, [SP, #12] 
    /*00002c84*/ ASR         R3, R2, #31 
    /*00002c88*/ SMLAL       R2, R3, R0, R1 
    /*00002c8c*/ STRD        R2, [R4, #72]               @ 0x48 
    /*00002c90*/ LDR         R3, [R7] 
    /*00002c94*/ STR         SL, [R4, #-56]              @ 0xffffffc8 
    /*00002c98*/ CMP         R3, R8 
    /*00002c9c*/ BNE         ScheduleAtOnce_x2a54
    /*00002ca0*/ ADD         R0, SP, #24 
    /*00002ca4*/ BL          0x0000000000002CA4          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00002ca8*/ LDR         R2, [SP, #28] 
    /*00002cac*/ LDR         R1, [SP, #24] 
    /*00002cb0*/ LDR         R0, [SP, #12] 
    /*00002cb4*/ ASR         R3, R2, #31 
    /*00002cb8*/ SMLAL       R2, R3, R0, R1 
    /*00002cbc*/ STRD        R2, [SL, #128]              @ 0x80 
    /*00002cc0*/ B           ScheduleAtOnce_x2a60
    ScheduleAtOnce_x2cc4:
    /*00002cc4*/ MOVW        R3, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x858] 
    /*00002cc8*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00002ccc*/ MOVT        R3, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x858] 
    /*00002cd0*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00002cd4*/ MOVW        R2, #465                    @ 0x1d1 
    /*00002cd8*/ LDR         R1, [PC, #168]              @ 0x0000000000002d88 
    /*00002cdc*/ BL          0x0000000000002CDC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002ce0*/ B           ScheduleAtOnce_x2924
    ScheduleAtOnce_x2ce4:
    /*00002ce4*/ ADD         R0, SP, #24 
    /*00002ce8*/ BL          0x0000000000002CE8          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00002cec*/ LDR         R2, [SP, #28] 
    /*00002cf0*/ LDR         R0, [SP, #24] 
    /*00002cf4*/ MOVW        R1, #16960                  @ 0x4240 
    /*00002cf8*/ MOVT        R1, #15 
    /*00002cfc*/ ASR         R3, R2, #31 
    /*00002d00*/ SMLAL       R2, R3, R1, R0 
    /*00002d04*/ STRD        R2, [SP, #16] 
    /*00002d08*/ B           ScheduleAtOnce_x28a8
    ScheduleAtOnce_x2d0c:
    /*00002d0c*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00002d10*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00002d14*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*00002d18*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00002d1c*/ MOVW        R2, #646                    @ 0x286 
    /*00002d20*/ LDR         R1, [PC, #92]               @ 0x0000000000002d84 
    /*00002d24*/ BL          0x0000000000002D24          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002d28*/ B           ScheduleAtOnce_x2b94
    ScheduleAtOnce_x2d2c:
    /*00002d2c*/ ADD         R0, SP, #24 
    /*00002d30*/ BL          0x0000000000002D30          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00002d34*/ LDR         R2, [SP, #28] 
    /*00002d38*/ LDR         R1, [SP, #24] 
    /*00002d3c*/ LDR         R0, [SP, #12] 
    /*00002d40*/ ASR         R3, R2, #31 
    /*00002d44*/ SMLAL       R2, R3, R0, R1 
    /*00002d48*/ STRD        R2, [R4, #72]               @ 0x48 
    /*00002d4c*/ LDR         R3, [R7] 
    /*00002d50*/ STR         SL, [R4, #-56]              @ 0xffffffc8 
    /*00002d54*/ CMP         R3, R8 
    /*00002d58*/ BNE         ScheduleAtOnce_x2bbc
    /*00002d5c*/ ADD         R0, SP, #24 
    /*00002d60*/ BL          0x0000000000002D60          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00002d64*/ LDR         R2, [SP, #28] 
    /*00002d68*/ LDR         R1, [SP, #24] 
    /*00002d6c*/ LDR         R0, [SP, #12] 
    /*00002d70*/ ASR         R3, R2, #31 
    /*00002d74*/ SMLAL       R2, R3, R0, R1 
    /*00002d78*/ STRD        R2, [SL, #128]              @ 0x80 
    /*00002d7c*/ B           ScheduleAtOnce_x2bc8
    /*00002d80*/ ANDEQ       R0, R0, R0, ROR #4 
    /*00002d84*/ ANDEQ       R0, R0, IP, ASR R1 
    /*00002d88*/ ANDEQ       R0, R0, IP, ASR #2 
FUNC_END ScheduleAtOnce


FUNC_BEGIN ChnlVeduIntHandler
    /*00002d8c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00002d90*/ MOV         R4, R0 
    /*00002d94*/ LDR         R6, [R0, #28] 
    /*00002d98*/ SUB         SP, SP, #52                 @ 0x34 
    /*00002d9c*/ CMP         R6, #0 
    /*00002da0*/ BEQ         ChnlVeduIntHandler_x37b0
    ChnlVeduIntHandler_x2da4:
    /*00002da4*/ LDR         R5, [R4, #64]               @ 0x40 
    /*00002da8*/ CMP         R5, #0 
    /*00002dac*/ BEQ         ChnlVeduIntHandler_x310c
    /*00002db0*/ LDR         R3, [R4, #72]               @ 0x48 
    /*00002db4*/ LDR         R0, [R4, #24] 
    /*00002db8*/ LDR         R8, [R6, #92]               @ 0x5c 
    /*00002dbc*/ BLX         R3 
    /*00002dc0*/ STR         R0, [R5, #76]               @ 0x4c 
    /*00002dc4*/ LDR         R3, [R4, #96]               @ 0x60 
    /*00002dc8*/ LDR         R0, [R4, #24] 
    /*00002dcc*/ BLX         R3 
    /*00002dd0*/ STR         R0, [R5, #148]              @ 0x94 
    /*00002dd4*/ LDR         R3, [R4, #88]               @ 0x58 
    /*00002dd8*/ LDR         R0, [R4, #24] 
    /*00002ddc*/ BLX         R3 
    /*00002de0*/ CMP         R0, #1 
    /*00002de4*/ MOV         R9, R0 
    /*00002de8*/ BNE         ChnlVeduIntHandler_x3754
    /*00002dec*/ LDR         R5, [R4, #64]               @ 0x40 
    /*00002df0*/ CMP         R5, #0 
    /*00002df4*/ BEQ         ChnlVeduIntHandler_x38b0
    ChnlVeduIntHandler_x2df8:
    /*00002df8*/ MOVW        R7, #0                      @ g_proc_enable [R_ARM_MOVW_ABS_NC: +0x0] 
    /*00002dfc*/ MOVT        R7, #0                      @ g_proc_enable [R_ARM_MOVT_ABS: +0x0] 
    /*00002e00*/ MOVW        R3, #2316                   @ 0x90c 
    /*00002e04*/ MOVT        R3, #10 
    /*00002e08*/ LDR         R2, [R7] 
    /*00002e0c*/ STR         R5, [R4, #60]               @ 0x3c 
    /*00002e10*/ CMP         R2, R3 
    /*00002e14*/ BEQ         ChnlVeduIntHandler_x3368
    /*00002e18*/ MOV         R3, #0 
    /*00002e1c*/ MOV         R2, #0 
    /*00002e20*/ STR         R3, [SP, #32] 
    /*00002e24*/ MOV         R3, #0 
    /*00002e28*/ STRD        R2, [SP, #24] 
    ChnlVeduIntHandler_x2e2c:
    /*00002e2c*/ LDR         R2, [R5, #52]               @ 0x34 
    /*00002e30*/ CMP         R2, #0 
    /*00002e34*/ LDRNE       R3, [R4, #24] 
    /*00002e38*/ STRNE       R3, [R2, #12] 
    /*00002e3c*/ LDRNE       R2, [R5, #52]               @ 0x34 
    /*00002e40*/ LDR         R3, [R5, #32] 
    /*00002e44*/ LDR         R1, [R4, #68]               @ 0x44 
    /*00002e48*/ LDR         R0, [R5] 
    /*00002e4c*/ BLX         R3 
    /*00002e50*/ LDR         R3, [R4, #100]              @ 0x64 
    /*00002e54*/ LDR         R0, [R4, #24] 
    /*00002e58*/ BLX         R3 
    /*00002e5c*/ CMP         R0, #1 
    /*00002e60*/ BEQ         ChnlVeduIntHandler_x35a0
    ChnlVeduIntHandler_x2e64:
    /*00002e64*/ LDR         R3, [R4, #76]               @ 0x4c 
    /*00002e68*/ LDR         R0, [R4, #24] 
    /*00002e6c*/ BLX         R3 
    /*00002e70*/ CMP         R4, #0 
    /*00002e74*/ BEQ         ChnlVeduIntHandler_x37fc
    ChnlVeduIntHandler_x2e78:
    /*00002e78*/ LDR         R3, [R4, #28] 
    /*00002e7c*/ ADD         FP, R4, #120                @ 0x78 
    /*00002e80*/ LDR         SL, [R3, #96]               @ 0x60 
    /*00002e84*/ CMP         SL, #0 
    /*00002e88*/ BEQ         ChnlVeduIntHandler_x33b8
    /*00002e8c*/ LDR         R3, [SL, #48]               @ 0x30 
    /*00002e90*/ CMP         R3, #1 
    /*00002e94*/ BEQ         ChnlVeduIntHandler_x33b8
    /*00002e98*/ CMP         R8, #1 
    /*00002e9c*/ BEQ         ChnlVeduIntHandler_x3114
    /*00002ea0*/ STR         SL, [R4, #64]               @ 0x40 
    /*00002ea4*/ LDR         R2, [SL, #48]               @ 0x30 
    /*00002ea8*/ SUB         R3, R2, #3 
    /*00002eac*/ CMP         R3, #1 
    /*00002eb0*/ BLS         ChnlVeduIntHandler_x2efc
    /*00002eb4*/ LDR         R3, [SL] 
    /*00002eb8*/ MOVW        R0, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8fc] 
    /*00002ebc*/ STR         R2, [SP] 
    /*00002ec0*/ MOVT        R0, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8fc] 
    /*00002ec4*/ MOV         R2, #628                    @ 0x274 
    /*00002ec8*/ LDR         R1, [PC, #3528]             @ 0x0000000000003c98 
    /*00002ecc*/ BL          0x0000000000002ECC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00002ed0*/ LDR         R3, [SL, #48]               @ 0x30 
    /*00002ed4*/ SUB         R3, R3, #3 
    /*00002ed8*/ CMP         R3, #1 
    /*00002edc*/ BLS         ChnlVeduIntHandler_x2efc
    /*00002ee0*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x92c] 
    /*00002ee4*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00002ee8*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x92c] 
    /*00002eec*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00002ef0*/ MOVW        R2, #629                    @ 0x275 
    /*00002ef4*/ LDR         R1, [PC, #3484]             @ 0x0000000000003c98 
    /*00002ef8*/ BL          0x0000000000002EF8          @ osal_panic [R_ARM_CALL: +0x0] 
    ChnlVeduIntHandler_x2efc:
    /*00002efc*/ LDR         R3, [R4, #92]               @ 0x5c 
    /*00002f00*/ MVN         R1, #0 
    /*00002f04*/ LDR         R0, [R4, #24] 
    /*00002f08*/ BLX         R3 
    /*00002f0c*/ LDR         R2, [SL, #52]               @ 0x34 
    /*00002f10*/ CMP         R2, #0 
    /*00002f14*/ LDRNE       R3, [R4, #24] 
    /*00002f18*/ STRNE       R3, [R2, #12] 
    /*00002f1c*/ LDRNE       R2, [SL, #52]               @ 0x34 
    /*00002f20*/ LDR         R3, [SL, #36]               @ 0x24 
    /*00002f24*/ LDR         R1, [R4, #68]               @ 0x44 
    /*00002f28*/ LDR         R0, [SL] 
    /*00002f2c*/ BLX         R3 
    /*00002f30*/ CMP         R0, #0 
    /*00002f34*/ BNE         ChnlVeduIntHandler_x3580
    ChnlVeduIntHandler_x2f38:
    /*00002f38*/ LDR         R3, [R4, #112]              @ 0x70 
    /*00002f3c*/ LDR         R0, [R4, #24] 
    /*00002f40*/ BLX         R3 
    /*00002f44*/ LDR         R5, [R7] 
    /*00002f48*/ MOVW        R3, #2316                   @ 0x90c 
    /*00002f4c*/ MOVT        R3, #10 
    /*00002f50*/ CMP         R5, R3 
    /*00002f54*/ BEQ         ChnlVeduIntHandler_x38d4
    /*00002f58*/ MOV         R2, #0 
    /*00002f5c*/ MOV         R3, #0 
    /*00002f60*/ STR         SL, [R4, #64]               @ 0x40 
    /*00002f64*/ STRD        R2, [R4, #192]              @ 0xc0 
    ChnlVeduIntHandler_x2f68:
    /*00002f68*/ MOV         R2, #0 
    /*00002f6c*/ MOV         R3, #0 
    /*00002f70*/ STRD        R2, [SL, #128]              @ 0x80 
    ChnlVeduIntHandler_x2f74:
    /*00002f74*/ LDR         R5, [R4, #60]               @ 0x3c 
    /*00002f78*/ LDR         R3, [R4, #148]              @ 0x94 
    /*00002f7c*/ CMP         R5, #0 
    /*00002f80*/ ADD         R3, R3, #1 
    /*00002f84*/ STR         R3, [R4, #148]              @ 0x94 
    /*00002f88*/ BEQ         ChnlVeduIntHandler_x3aa8
    ChnlVeduIntHandler_x2f8c:
    /*00002f8c*/ LDR         R1, [R5, #52]               @ 0x34 
    /*00002f90*/ CMP         R1, #0 
    /*00002f94*/ BEQ         ChnlVeduIntHandler_x2fbc
    /*00002f98*/ MOV         R3, #0 
    /*00002f9c*/ STR         R3, [R1, #4] 
    /*00002fa0*/ LDR         R3, [R5, #76]               @ 0x4c 
    /*00002fa4*/ STR         R3, [R1, #8] 
    /*00002fa8*/ LDR         R1, [R5, #52]               @ 0x34 
    /*00002fac*/ CMP         R1, #0 
    /*00002fb0*/ MOVNE       R3, #1 
    /*00002fb4*/ STRNE       R3, [R1] 
    /*00002fb8*/ LDRNE       R1, [R5, #52]               @ 0x34 
    ChnlVeduIntHandler_x2fbc:
    /*00002fbc*/ LDR         R3, [R5, #24] 
    /*00002fc0*/ LDR         R0, [R5] 
    /*00002fc4*/ BLX         R3 
    /*00002fc8*/ LDR         R3, [R5, #64]               @ 0x40 
    /*00002fcc*/ ADD         R3, R3, #1 
    /*00002fd0*/ STR         R3, [R5, #64]               @ 0x40 
    /*00002fd4*/ SUBS        R1, R0, #0 
    /*00002fd8*/ BNE         ChnlVeduIntHandler_x3568
    ChnlVeduIntHandler_x2fdc:
    /*00002fdc*/ LDR         R2, [R5, #52]               @ 0x34 
    /*00002fe0*/ MOV         R3, #0 
    /*00002fe4*/ STR         R3, [R5, #76]               @ 0x4c 
    /*00002fe8*/ CMP         R2, R3 
    /*00002fec*/ BEQ         ChnlVeduIntHandler_x3394
    /*00002ff0*/ LDR         R2, [R2] 
    /*00002ff4*/ CMP         R2, #1 
    /*00002ff8*/ BEQ         ChnlVeduIntHandler_x3394
    /*00002ffc*/ LDR         R2, [R5, #152]              @ 0x98 
    /*00003000*/ CMP         R2, #1 
    /*00003004*/ BEQ         ChnlVeduIntHandler_x32d4
    ChnlVeduIntHandler_x3008:
    /*00003008*/ MOV         R9, R3 
    ChnlVeduIntHandler_x300c:
    /*0000300c*/ MOV         R2, FP 
    /*00003010*/ LDR         R1, [R6, #100]              @ 0x64 
    /*00003014*/ ADD         R0, R6, #8 
    /*00003018*/ BL          0x0000000000003018          @ ChnlScheduler [R_ARM_CALL: .text+0xe44] 
    /*0000301c*/ STR         R0, [R6, #96]               @ 0x60 
    ChnlVeduIntHandler_x3020:
    /*00003020*/ CMP         R9, #0 
    /*00003024*/ BEQ         ChnlVeduIntHandler_x3034
    ChnlVeduIntHandler_x3028:
    /*00003028*/ LDR         R3, [R6, #4] 
    /*0000302c*/ TST         R3, #4 
    /*00003030*/ BNE         ChnlVeduIntHandler_x33ac
    ChnlVeduIntHandler_x3034:
    /*00003034*/ LDR         R3, [R4, #64]               @ 0x40 
    /*00003038*/ CMP         R3, #0 
    /*0000303c*/ MOVNE       R3, #1 
    /*00003040*/ STRNE       R3, [R4] 
    /*00003044*/ BEQ         ChnlVeduIntHandler_x3864
    ChnlVeduIntHandler_x3048:
    /*00003048*/ LDR         R3, [R7] 
    /*0000304c*/ MOVW        R2, #2316                   @ 0x90c 
    /*00003050*/ LDR         R1, [SP, #32] 
    /*00003054*/ MOVT        R2, #10 
    /*00003058*/ CMP         R3, R2 
    /*0000305c*/ MOVNE       R1, #0 
    /*00003060*/ ANDEQ       R1, R1, #1 
    /*00003064*/ CMP         R1, #0 
    /*00003068*/ BEQ         ChnlVeduIntHandler_x310c
    /*0000306c*/ ADD         R0, SP, #40                 @ 0x28 
    /*00003070*/ LDR         R5, [R4, #60]               @ 0x3c 
    /*00003074*/ BL          0x0000000000003074          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00003078*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*0000307c*/ LDR         R1, [SP, #40]               @ 0x28 
    /*00003080*/ MOVW        R3, #16960                  @ 0x4240 
    /*00003084*/ MOVT        R3, #15 
    /*00003088*/ MLA         R3, R1, R3, R2 
    /*0000308c*/ LDR         R2, [SP, #24] 
    /*00003090*/ SUB         R3, R3, R2 
    /*00003094*/ STR         R3, [R5, #84]               @ 0x54 
    /*00003098*/ LDR         R3, [R4, #60]               @ 0x3c 
    /*0000309c*/ LDR         R2, [R3, #84]               @ 0x54 
    /*000030a0*/ LDR         R1, [R3, #104]              @ 0x68 
    /*000030a4*/ CMP         R2, R1 
    /*000030a8*/ STRGT       R2, [R3, #104]              @ 0x68 
    /*000030ac*/ LDR         R3, [R4, #64]               @ 0x40 
    /*000030b0*/ CMP         R3, #0 
    /*000030b4*/ BEQ         ChnlVeduIntHandler_x30f4
    /*000030b8*/ ADD         R0, SP, #40                 @ 0x28 
    /*000030bc*/ BL          0x00000000000030BC          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000030c0*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*000030c4*/ LDR         R0, [SP, #40]               @ 0x28 
    /*000030c8*/ MOVW        R1, #16960                  @ 0x4240 
    /*000030cc*/ MOVT        R1, #15 
    /*000030d0*/ LDRD        R6, [R4, #176]              @ 0xb0 
    /*000030d4*/ ASR         R3, R2, #31 
    /*000030d8*/ SMLAL       R2, R3, R1, R0 
    /*000030dc*/ LDRD        R0, [R4, #192]              @ 0xc0 
    /*000030e0*/ ADDS        R2, R2, R6 
    /*000030e4*/ ADC         R3, R3, R7 
    /*000030e8*/ SUBS        R2, R2, R0 
    /*000030ec*/ SBC         R3, R3, R1 
    /*000030f0*/ STRD        R2, [R4, #176]              @ 0xb0 
    ChnlVeduIntHandler_x30f4:
    /*000030f4*/ LDR         R1, [R4, #60]               @ 0x3c 
    /*000030f8*/ LDRD        R2, [R4, #168]              @ 0xa8 
    /*000030fc*/ LDR         R1, [R1, #84]               @ 0x54 
    /*00003100*/ ADDS        R2, R2, R1 
    /*00003104*/ ADC         R3, R3, R1, ASR #31 
    /*00003108*/ STRD        R2, [R4, #168]              @ 0xa8 
    ChnlVeduIntHandler_x310c:
    /*0000310c*/ ADD         SP, SP, #52                 @ 0x34 
    /*00003110*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    ChnlVeduIntHandler_x3114:
    /*00003114*/ LDR         R2, [R5, #4] 
    /*00003118*/ LDR         R3, [SL, #4] 
    /*0000311c*/ CMP         R2, R3 
    /*00003120*/ MOVLS       R9, #0 
    /*00003124*/ BHI         ChnlVeduIntHandler_x35dc
    ChnlVeduIntHandler_x3128:
    /*00003128*/ MOV         R2, FP 
    /*0000312c*/ LDR         R1, [R6, #100]              @ 0x64 
    /*00003130*/ ADD         R0, R6, #8 
    /*00003134*/ BL          0x0000000000003134          @ ChnlScheduler [R_ARM_CALL: .text+0xe44] 
    /*00003138*/ SUBS        R8, R0, #0 
    /*0000313c*/ BEQ         ChnlVeduIntHandler_x3150
    /*00003140*/ LDR         R2, [R8, #4] 
    /*00003144*/ LDR         R3, [SL, #4] 
    /*00003148*/ CMP         R2, R3 
    /*0000314c*/ BHI         ChnlVeduIntHandler_x366c
    ChnlVeduIntHandler_x3150:
    /*00003150*/ STR         SL, [R4, #64]               @ 0x40 
    /*00003154*/ LDR         R2, [SL, #48]               @ 0x30 
    /*00003158*/ SUB         R3, R2, #3 
    /*0000315c*/ CMP         R3, #1 
    /*00003160*/ BLS         ChnlVeduIntHandler_x31ac
    /*00003164*/ LDR         R3, [SL] 
    /*00003168*/ MOVW        R0, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8fc] 
    /*0000316c*/ STR         R2, [SP] 
    /*00003170*/ MOVT        R0, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8fc] 
    /*00003174*/ MOV         R2, #628                    @ 0x274 
    /*00003178*/ LDR         R1, [PC, #2840]             @ 0x0000000000003c98 
    /*0000317c*/ BL          0x000000000000317C          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00003180*/ LDR         R3, [SL, #48]               @ 0x30 
    /*00003184*/ SUB         R3, R3, #3 
    /*00003188*/ CMP         R3, #1 
    /*0000318c*/ BLS         ChnlVeduIntHandler_x31ac
    /*00003190*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x92c] 
    /*00003194*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00003198*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x92c] 
    /*0000319c*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000031a0*/ MOVW        R2, #629                    @ 0x275 
    /*000031a4*/ LDR         R1, [PC, #2796]             @ 0x0000000000003c98 
    /*000031a8*/ BL          0x00000000000031A8          @ osal_panic [R_ARM_CALL: +0x0] 
    ChnlVeduIntHandler_x31ac:
    /*000031ac*/ LDR         R3, [R4, #92]               @ 0x5c 
    /*000031b0*/ MVN         R1, #0 
    /*000031b4*/ LDR         R0, [R4, #24] 
    /*000031b8*/ BLX         R3 
    /*000031bc*/ LDR         R2, [SL, #52]               @ 0x34 
    /*000031c0*/ CMP         R2, #0 
    /*000031c4*/ LDRNE       R3, [R4, #24] 
    /*000031c8*/ STRNE       R3, [R2, #12] 
    /*000031cc*/ LDRNE       R2, [SL, #52]               @ 0x34 
    /*000031d0*/ LDR         R3, [SL, #36]               @ 0x24 
    /*000031d4*/ LDR         R1, [R4, #68]               @ 0x44 
    /*000031d8*/ LDR         R0, [SL] 
    /*000031dc*/ BLX         R3 
    /*000031e0*/ CMP         R0, #0 
    /*000031e4*/ BNE         ChnlVeduIntHandler_x3300
    ChnlVeduIntHandler_x31e8:
    /*000031e8*/ LDR         R3, [R4, #112]              @ 0x70 
    /*000031ec*/ LDR         R0, [R4, #24] 
    /*000031f0*/ BLX         R3 
    /*000031f4*/ LDR         FP, [R7] 
    /*000031f8*/ MOVW        R3, #2316                   @ 0x90c 
    /*000031fc*/ MOVT        R3, #10 
    /*00003200*/ CMP         FP, R3 
    /*00003204*/ BEQ         ChnlVeduIntHandler_x3944
    /*00003208*/ MOV         R2, #0 
    /*0000320c*/ MOV         R3, #0 
    /*00003210*/ STR         SL, [R4, #64]               @ 0x40 
    /*00003214*/ STRD        R2, [R4, #192]              @ 0xc0 
    ChnlVeduIntHandler_x3218:
    /*00003218*/ MOV         R2, #0 
    /*0000321c*/ MOV         R3, #0 
    /*00003220*/ STRD        R2, [SL, #128]              @ 0x80 
    ChnlVeduIntHandler_x3224:
    /*00003224*/ LDR         R3, [R4, #148]              @ 0x94 
    /*00003228*/ ADD         R3, R3, #1 
    /*0000322c*/ STR         R3, [R4, #148]              @ 0x94 
    /*00003230*/ STR         R8, [R6, #96]               @ 0x60 
    ChnlVeduIntHandler_x3234:
    /*00003234*/ CMP         R5, #0 
    /*00003238*/ BEQ         ChnlVeduIntHandler_x3020
    /*0000323c*/ LDR         R5, [R4, #60]               @ 0x3c 
    /*00003240*/ CMP         R5, #0 
    /*00003244*/ BEQ         ChnlVeduIntHandler_x3b08
    ChnlVeduIntHandler_x3248:
    /*00003248*/ LDR         R1, [R5, #52]               @ 0x34 
    /*0000324c*/ CMP         R1, #0 
    /*00003250*/ BEQ         ChnlVeduIntHandler_x3278
    /*00003254*/ MOV         R3, #0 
    /*00003258*/ STR         R3, [R1, #4] 
    /*0000325c*/ LDR         R3, [R5, #76]               @ 0x4c 
    /*00003260*/ STR         R3, [R1, #8] 
    /*00003264*/ LDR         R1, [R5, #52]               @ 0x34 
    /*00003268*/ CMP         R1, #0 
    /*0000326c*/ MOVNE       R3, #1 
    /*00003270*/ STRNE       R3, [R1] 
    /*00003274*/ LDRNE       R1, [R5, #52]               @ 0x34 
    ChnlVeduIntHandler_x3278:
    /*00003278*/ LDR         R3, [R5, #24] 
    /*0000327c*/ LDR         R0, [R5] 
    /*00003280*/ BLX         R3 
    /*00003284*/ LDR         R3, [R5, #64]               @ 0x40 
    /*00003288*/ ADD         R3, R3, #1 
    /*0000328c*/ STR         R3, [R5, #64]               @ 0x40 
    /*00003290*/ SUBS        R1, R0, #0 
    /*00003294*/ BNE         ChnlVeduIntHandler_x3320
    ChnlVeduIntHandler_x3298:
    /*00003298*/ LDR         R3, [R5, #52]               @ 0x34 
    /*0000329c*/ MOV         R9, #0 
    /*000032a0*/ STR         R9, [R5, #76]               @ 0x4c 
    /*000032a4*/ CMP         R3, R9 
    /*000032a8*/ BEQ         ChnlVeduIntHandler_x3338
    /*000032ac*/ LDR         R3, [R3] 
    /*000032b0*/ CMP         R3, #1 
    /*000032b4*/ BEQ         ChnlVeduIntHandler_x3338
    /*000032b8*/ LDR         R3, [R5, #152]              @ 0x98 
    /*000032bc*/ CMP         R3, #1 
    /*000032c0*/ BNE         ChnlVeduIntHandler_x3034
    /*000032c4*/ LDR         R3, [R5, #48]               @ 0x30 
    /*000032c8*/ CMP         R3, #2 
    /*000032cc*/ BNE         ChnlVeduIntHandler_x3034
    /*000032d0*/ B           ChnlVeduIntHandler_x334c
    ChnlVeduIntHandler_x32d4:
    /*000032d4*/ LDR         R2, [R5, #48]               @ 0x30 
    /*000032d8*/ CMP         R2, #2 
    /*000032dc*/ BNE         ChnlVeduIntHandler_x3008
    ChnlVeduIntHandler_x32e0:
    /*000032e0*/ MOV         R2, #6 
    /*000032e4*/ ADD         R0, R5, #156                @ 0x9c 
    /*000032e8*/ STR         R2, [R5, #48]               @ 0x30 
    /*000032ec*/ MOV         R2, #0 
    /*000032f0*/ STR         R2, [R5, #152]              @ 0x98 
    /*000032f4*/ MOV         R9, R3 
    /*000032f8*/ BL          0x00000000000032F8          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*000032fc*/ B           ChnlVeduIntHandler_x300c
    ChnlVeduIntHandler_x3300:
    /*00003300*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00003304*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00003308*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*0000330c*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00003310*/ MOVW        R2, #646                    @ 0x286 
    /*00003314*/ LDR         R1, [PC, #2428]             @ 0x0000000000003c98 
    /*00003318*/ BL          0x0000000000003318          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000331c*/ B           ChnlVeduIntHandler_x31e8
    ChnlVeduIntHandler_x3320:
    /*00003320*/ MOVW        R0, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa5c] 
    /*00003324*/ MOVT        R0, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa5c] 
    /*00003328*/ BL          0x0000000000003328          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000332c*/ MOV         R3, #2 
    /*00003330*/ STR         R3, [R5, #48]               @ 0x30 
    /*00003334*/ B           ChnlVeduIntHandler_x3298
    ChnlVeduIntHandler_x3338:
    /*00003338*/ LDR         R9, [R5, #152]              @ 0x98 
    /*0000333c*/ MOV         R3, #2 
    /*00003340*/ STR         R3, [R5, #48]               @ 0x30 
    /*00003344*/ CMP         R9, #1 
    /*00003348*/ BNE         ChnlVeduIntHandler_x3028
    ChnlVeduIntHandler_x334c:
    /*0000334c*/ MOV         R3, #6 
    /*00003350*/ ADD         R0, R5, #156                @ 0x9c 
    /*00003354*/ STR         R3, [R5, #48]               @ 0x30 
    /*00003358*/ MOV         R3, #0 
    /*0000335c*/ STR         R3, [R5, #152]              @ 0x98 
    /*00003360*/ BL          0x0000000000003360          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00003364*/ B           ChnlVeduIntHandler_x3020
    ChnlVeduIntHandler_x3368:
    /*00003368*/ LDRD        R2, [R5, #128]              @ 0x80 
    /*0000336c*/ STRD        R2, [SP, #24] 
    /*00003370*/ ORRS        R3, R2, R3 
    /*00003374*/ BEQ         ChnlVeduIntHandler_x3388
    /*00003378*/ LDRD        R2, [R4, #192]              @ 0xc0 
    /*0000337c*/ STRD        R2, [SP, #24] 
    /*00003380*/ ORRS        R3, R2, R3 
    /*00003384*/ BNE         ChnlVeduIntHandler_x39ac
    ChnlVeduIntHandler_x3388:
    /*00003388*/ MOV         R3, #0 
    /*0000338c*/ STR         R3, [SP, #32] 
    /*00003390*/ B           ChnlVeduIntHandler_x2e2c
    ChnlVeduIntHandler_x3394:
    /*00003394*/ LDR         R3, [R5, #152]              @ 0x98 
    /*00003398*/ MOV         R2, #2 
    /*0000339c*/ STR         R2, [R5, #48]               @ 0x30 
    /*000033a0*/ CMP         R3, #1 
    /*000033a4*/ BNE         ChnlVeduIntHandler_x300c
    /*000033a8*/ B           ChnlVeduIntHandler_x32e0
    ChnlVeduIntHandler_x33ac:
    /*000033ac*/ MOV         R0, R6 
    /*000033b0*/ BL          0x00000000000033B0          @ ScheduleAtOnce [R_ARM_CALL: .text+0x2874] 
    /*000033b4*/ B           ChnlVeduIntHandler_x3034
    ChnlVeduIntHandler_x33b8:
    /*000033b8*/ LDR         R5, [R4, #60]               @ 0x3c 
    /*000033bc*/ CMP         R5, #0 
    /*000033c0*/ BEQ         ChnlVeduIntHandler_x3c74
    ChnlVeduIntHandler_x33c4:
    /*000033c4*/ LDR         R1, [R5, #52]               @ 0x34 
    /*000033c8*/ CMP         R1, #0 
    /*000033cc*/ BEQ         ChnlVeduIntHandler_x33f4
    /*000033d0*/ MOV         R3, #0 
    /*000033d4*/ STR         R3, [R1, #4] 
    /*000033d8*/ LDR         R3, [R5, #76]               @ 0x4c 
    /*000033dc*/ STR         R3, [R1, #8] 
    /*000033e0*/ LDR         R1, [R5, #52]               @ 0x34 
    /*000033e4*/ CMP         R1, #0 
    /*000033e8*/ MOVNE       R3, #1 
    /*000033ec*/ STRNE       R3, [R1] 
    /*000033f0*/ LDRNE       R1, [R5, #52]               @ 0x34 
    ChnlVeduIntHandler_x33f4:
    /*000033f4*/ LDR         R3, [R5, #24] 
    /*000033f8*/ LDR         R0, [R5] 
    /*000033fc*/ BLX         R3 
    /*00003400*/ LDR         R3, [R5, #64]               @ 0x40 
    /*00003404*/ ADD         R3, R3, #1 
    /*00003408*/ STR         R3, [R5, #64]               @ 0x40 
    /*0000340c*/ SUBS        R1, R0, #0 
    /*00003410*/ BNE         ChnlVeduIntHandler_x3b48
    ChnlVeduIntHandler_x3414:
    /*00003414*/ LDR         R3, [R5, #52]               @ 0x34 
    /*00003418*/ MOV         SL, #0 
    /*0000341c*/ STR         SL, [R5, #76]               @ 0x4c 
    /*00003420*/ CMP         R3, SL 
    /*00003424*/ BEQ         ChnlVeduIntHandler_x3acc
    /*00003428*/ LDR         R3, [R3] 
    /*0000342c*/ CMP         R3, #1 
    /*00003430*/ BEQ         ChnlVeduIntHandler_x3acc
    /*00003434*/ LDR         R3, [R5, #152]              @ 0x98 
    /*00003438*/ CMP         R3, #1 
    /*0000343c*/ BNE         ChnlVeduIntHandler_x344c
    /*00003440*/ LDR         R3, [R5, #48]               @ 0x30 
    /*00003444*/ CMP         R3, #2 
    /*00003448*/ BEQ         ChnlVeduIntHandler_x381c
    ChnlVeduIntHandler_x344c:
    /*0000344c*/ ADD         R8, R6, #8 
    /*00003450*/ MOV         R2, FP 
    /*00003454*/ LDR         R1, [R6, #100]              @ 0x64 
    /*00003458*/ MOV         R0, R8 
    /*0000345c*/ BL          0x000000000000345C          @ ChnlScheduler [R_ARM_CALL: .text+0xe44] 
    /*00003460*/ CMP         R0, #0 
    /*00003464*/ MOV         R5, R0 
    /*00003468*/ STR         R0, [R4, #64]               @ 0x40 
    /*0000346c*/ MOVNE       R9, #0 
    /*00003470*/ BEQ         ChnlVeduIntHandler_x3864
    ChnlVeduIntHandler_x3474:
    /*00003474*/ LDR         R2, [R5, #48]               @ 0x30 
    /*00003478*/ SUB         R3, R2, #3 
    /*0000347c*/ CMP         R3, #1 
    /*00003480*/ BLS         ChnlVeduIntHandler_x34cc
    /*00003484*/ LDR         R3, [R5] 
    /*00003488*/ MOVW        R0, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8fc] 
    /*0000348c*/ STR         R2, [SP] 
    /*00003490*/ MOVT        R0, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8fc] 
    /*00003494*/ MOV         R2, #628                    @ 0x274 
    /*00003498*/ LDR         R1, [PC, #2040]             @ 0x0000000000003c98 
    /*0000349c*/ BL          0x000000000000349C          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000034a0*/ LDR         R3, [R5, #48]               @ 0x30 
    /*000034a4*/ SUB         R3, R3, #3 
    /*000034a8*/ CMP         R3, #1 
    /*000034ac*/ BLS         ChnlVeduIntHandler_x34cc
    /*000034b0*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x92c] 
    /*000034b4*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000034b8*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x92c] 
    /*000034bc*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000034c0*/ MOVW        R2, #629                    @ 0x275 
    /*000034c4*/ LDR         R1, [PC, #1996]             @ 0x0000000000003c98 
    /*000034c8*/ BL          0x00000000000034C8          @ osal_panic [R_ARM_CALL: +0x0] 
    ChnlVeduIntHandler_x34cc:
    /*000034cc*/ LDR         R3, [R4, #92]               @ 0x5c 
    /*000034d0*/ MVN         R1, #0 
    /*000034d4*/ LDR         R0, [R4, #24] 
    /*000034d8*/ BLX         R3 
    /*000034dc*/ LDR         R2, [R5, #52]               @ 0x34 
    /*000034e0*/ CMP         R2, #0 
    /*000034e4*/ LDRNE       R3, [R4, #24] 
    /*000034e8*/ STRNE       R3, [R2, #12] 
    /*000034ec*/ LDRNE       R2, [R5, #52]               @ 0x34 
    /*000034f0*/ LDR         R3, [R5, #36]               @ 0x24 
    /*000034f4*/ LDR         R1, [R4, #68]               @ 0x44 
    /*000034f8*/ LDR         R0, [R5] 
    /*000034fc*/ BLX         R3 
    /*00003500*/ CMP         R0, #0 
    /*00003504*/ BNE         ChnlVeduIntHandler_x3bc8
    ChnlVeduIntHandler_x3508:
    /*00003508*/ LDR         R3, [R4, #112]              @ 0x70 
    /*0000350c*/ LDR         R0, [R4, #24] 
    /*00003510*/ BLX         R3 
    /*00003514*/ LDR         SL, [R7] 
    /*00003518*/ MOVW        R3, #2316                   @ 0x90c 
    /*0000351c*/ MOVT        R3, #10 
    /*00003520*/ CMP         SL, R3 
    /*00003524*/ BEQ         ChnlVeduIntHandler_x3c0c
    /*00003528*/ MOV         R2, #0 
    /*0000352c*/ MOV         R3, #0 
    /*00003530*/ STR         R5, [R4, #64]               @ 0x40 
    /*00003534*/ STRD        R2, [R4, #192]              @ 0xc0 
    ChnlVeduIntHandler_x3538:
    /*00003538*/ MOV         R2, #0 
    /*0000353c*/ MOV         R3, #0 
    /*00003540*/ STRD        R2, [R5, #128]              @ 0x80 
    ChnlVeduIntHandler_x3544:
    /*00003544*/ LDR         R3, [R4, #148]              @ 0x94 
    /*00003548*/ MOV         R2, FP 
    /*0000354c*/ MOV         R0, R8 
    /*00003550*/ ADD         R3, R3, #1 
    /*00003554*/ STR         R3, [R4, #148]              @ 0x94 
    /*00003558*/ LDR         R1, [R6, #100]              @ 0x64 
    /*0000355c*/ BL          0x000000000000355C          @ ChnlScheduler [R_ARM_CALL: .text+0xe44] 
    /*00003560*/ STR         R0, [R6, #96]               @ 0x60 
    /*00003564*/ B           ChnlVeduIntHandler_x3020
    ChnlVeduIntHandler_x3568:
    /*00003568*/ MOVW        R0, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa5c] 
    /*0000356c*/ MOVT        R0, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa5c] 
    /*00003570*/ BL          0x0000000000003570          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00003574*/ MOV         R3, #2 
    /*00003578*/ STR         R3, [R5, #48]               @ 0x30 
    /*0000357c*/ B           ChnlVeduIntHandler_x2fdc
    ChnlVeduIntHandler_x3580:
    /*00003580*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00003584*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00003588*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*0000358c*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00003590*/ MOVW        R2, #646                    @ 0x286 
    /*00003594*/ LDR         R1, [PC, #1788]             @ 0x0000000000003c98 
    /*00003598*/ BL          0x0000000000003598          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000359c*/ B           ChnlVeduIntHandler_x2f38
    ChnlVeduIntHandler_x35a0:
    /*000035a0*/ LDR         R1, [R4, #24] 
    /*000035a4*/ MOVW        R3, #1217                   @ 0x4c1 
    /*000035a8*/ MOVW        R2, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdb0] 
    /*000035ac*/ STR         R3, [SP] 
    /*000035b0*/ MOVT        R2, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdb0] 
    /*000035b4*/ LDR         R3, [PC, #1760]             @ 0x0000000000003c9c 
    /*000035b8*/ STR         R1, [SP, #4] 
    /*000035bc*/ MOV         R0, #3 
    /*000035c0*/ MOV         R1, #4 
    /*000035c4*/ BL          0x00000000000035C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000035c8*/ LDR         R3, [R4, #104]              @ 0x68 
    /*000035cc*/ MOV         R1, #0 
    /*000035d0*/ LDR         R0, [R4, #24] 
    /*000035d4*/ BLX         R3 
    /*000035d8*/ B           ChnlVeduIntHandler_x2e64
    ChnlVeduIntHandler_x35dc:
    /*000035dc*/ LDR         R8, [R4, #60]               @ 0x3c 
    /*000035e0*/ CMP         R8, #0 
    /*000035e4*/ BEQ         ChnlVeduIntHandler_x3be8
    ChnlVeduIntHandler_x35e8:
    /*000035e8*/ LDR         R1, [R8, #52]               @ 0x34 
    /*000035ec*/ CMP         R1, #0 
    /*000035f0*/ BEQ         ChnlVeduIntHandler_x3618
    /*000035f4*/ MOV         R3, #0 
    /*000035f8*/ STR         R3, [R1, #4] 
    /*000035fc*/ LDR         R3, [R8, #76]               @ 0x4c 
    /*00003600*/ STR         R3, [R1, #8] 
    /*00003604*/ LDR         R1, [R8, #52]               @ 0x34 
    /*00003608*/ CMP         R1, #0 
    /*0000360c*/ MOVNE       R3, #1 
    /*00003610*/ STRNE       R3, [R1] 
    /*00003614*/ LDRNE       R1, [R8, #52]               @ 0x34 
    ChnlVeduIntHandler_x3618:
    /*00003618*/ LDR         R3, [R8, #24] 
    /*0000361c*/ LDR         R0, [R8] 
    /*00003620*/ BLX         R3 
    /*00003624*/ LDR         R3, [R8, #64]               @ 0x40 
    /*00003628*/ ADD         R3, R3, #1 
    /*0000362c*/ STR         R3, [R8, #64]               @ 0x40 
    /*00003630*/ SUBS        R1, R0, #0 
    /*00003634*/ BNE         ChnlVeduIntHandler_x3a70
    ChnlVeduIntHandler_x3638:
    /*00003638*/ LDR         R3, [R8, #52]               @ 0x34 
    /*0000363c*/ MOV         R9, #0 
    /*00003640*/ STR         R9, [R8, #76]               @ 0x4c 
    /*00003644*/ CMP         R3, R9 
    /*00003648*/ BEQ         ChnlVeduIntHandler_x3928
    /*0000364c*/ LDR         R3, [R3] 
    /*00003650*/ CMP         R3, #1 
    /*00003654*/ BEQ         ChnlVeduIntHandler_x3928
    /*00003658*/ LDR         R3, [R8, #152]              @ 0x98 
    /*0000365c*/ CMP         R3, #1 
    /*00003660*/ BEQ         ChnlVeduIntHandler_x37d0
    ChnlVeduIntHandler_x3664:
    /*00003664*/ MOV         R5, #0 
    /*00003668*/ B           ChnlVeduIntHandler_x3128
    ChnlVeduIntHandler_x366c:
    /*0000366c*/ STR         R8, [R4, #64]               @ 0x40 
    /*00003670*/ LDR         R2, [R8, #48]               @ 0x30 
    /*00003674*/ SUB         R3, R2, #3 
    /*00003678*/ CMP         R3, #1 
    /*0000367c*/ BLS         ChnlVeduIntHandler_x36c8
    /*00003680*/ LDR         R3, [R8] 
    /*00003684*/ MOVW        R0, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8fc] 
    /*00003688*/ STR         R2, [SP] 
    /*0000368c*/ MOVT        R0, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8fc] 
    /*00003690*/ MOV         R2, #628                    @ 0x274 
    /*00003694*/ LDR         R1, [PC, #1532]             @ 0x0000000000003c98 
    /*00003698*/ BL          0x0000000000003698          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000369c*/ LDR         R3, [R8, #48]               @ 0x30 
    /*000036a0*/ SUB         R3, R3, #3 
    /*000036a4*/ CMP         R3, #1 
    /*000036a8*/ BLS         ChnlVeduIntHandler_x36c8
    /*000036ac*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x92c] 
    /*000036b0*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000036b4*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x92c] 
    /*000036b8*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000036bc*/ MOVW        R2, #629                    @ 0x275 
    /*000036c0*/ LDR         R1, [PC, #1488]             @ 0x0000000000003c98 
    /*000036c4*/ BL          0x00000000000036C4          @ osal_panic [R_ARM_CALL: +0x0] 
    ChnlVeduIntHandler_x36c8:
    /*000036c8*/ LDR         R3, [R4, #92]               @ 0x5c 
    /*000036cc*/ MVN         R1, #0 
    /*000036d0*/ LDR         R0, [R4, #24] 
    /*000036d4*/ BLX         R3 
    /*000036d8*/ LDR         R2, [R8, #52]               @ 0x34 
    /*000036dc*/ CMP         R2, #0 
    /*000036e0*/ LDRNE       R3, [R4, #24] 
    /*000036e4*/ STRNE       R3, [R2, #12] 
    /*000036e8*/ LDRNE       R2, [R8, #52]               @ 0x34 
    /*000036ec*/ LDR         R3, [R8, #36]               @ 0x24 
    /*000036f0*/ LDR         R1, [R4, #68]               @ 0x44 
    /*000036f4*/ LDR         R0, [R8] 
    /*000036f8*/ BLX         R3 
    /*000036fc*/ CMP         R0, #0 
    /*00003700*/ BNE         ChnlVeduIntHandler_x3a88
    ChnlVeduIntHandler_x3704:
    /*00003704*/ LDR         R3, [R4, #112]              @ 0x70 
    /*00003708*/ LDR         R0, [R4, #24] 
    /*0000370c*/ BLX         R3 
    /*00003710*/ LDR         FP, [R7] 
    /*00003714*/ MOVW        R3, #2316                   @ 0x90c 
    /*00003718*/ MOVT        R3, #10 
    /*0000371c*/ CMP         FP, R3 
    /*00003720*/ BEQ         ChnlVeduIntHandler_x3b60
    /*00003724*/ MOV         R2, #0 
    /*00003728*/ MOV         R3, #0 
    /*0000372c*/ STR         R8, [R4, #64]               @ 0x40 
    /*00003730*/ STRD        R2, [R4, #192]              @ 0xc0 
    ChnlVeduIntHandler_x3734:
    /*00003734*/ MOV         R2, #0 
    /*00003738*/ MOV         R3, #0 
    /*0000373c*/ STRD        R2, [R8, #128]              @ 0x80 
    ChnlVeduIntHandler_x3740:
    /*00003740*/ LDR         R3, [R4, #148]              @ 0x94 
    /*00003744*/ ADD         R3, R3, #1 
    /*00003748*/ STR         R3, [R4, #148]              @ 0x94 
    /*0000374c*/ STR         SL, [R6, #96]               @ 0x60 
    /*00003750*/ B           ChnlVeduIntHandler_x3234
    ChnlVeduIntHandler_x3754:
    /*00003754*/ LDR         R1, [R5, #8] 
    /*00003758*/ MOVW        LR, #302                    @ 0x12e 
    /*0000375c*/ ADD         R6, R4, #8 
    /*00003760*/ LDR         R3, [PC, #1336]             @ 0x0000000000003ca0 
    /*00003764*/ MOVW        R2, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd4c] 
    /*00003768*/ MOV         R0, #3 
    /*0000376c*/ STR         R1, [SP, #16] 
    /*00003770*/ MOVT        R2, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd4c] 
    /*00003774*/ LDR         IP, [R5] 
    /*00003778*/ MOV         R1, #4 
    /*0000377c*/ STR         IP, [SP, #12] 
    /*00003780*/ LDR         IP, [R5, #76]               @ 0x4c 
    /*00003784*/ STR         R6, [SP, #4] 
    /*00003788*/ STR         LR, [SP] 
    /*0000378c*/ STR         IP, [SP, #8] 
    /*00003790*/ BL          0x0000000000003790          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003794*/ LDR         R3, [R4, #132]              @ 0x84 
    /*00003798*/ LDR         R2, [R4, #80]               @ 0x50 
    /*0000379c*/ ADD         R3, R3, #1 
    /*000037a0*/ LDR         R0, [R4, #24] 
    /*000037a4*/ STR         R3, [R4, #132]              @ 0x84 
    /*000037a8*/ BLX         R2 
    /*000037ac*/ B           ChnlVeduIntHandler_x310c
    ChnlVeduIntHandler_x37b0:
    /*000037b0*/ MOVW        R3, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x840] 
    /*000037b4*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000037b8*/ MOVT        R3, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x840] 
    /*000037bc*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000037c0*/ MOVW        R2, #1162                   @ 0x48a 
    /*000037c4*/ LDR         R1, [PC, #1232]             @ 0x0000000000003c9c 
    /*000037c8*/ BL          0x00000000000037C8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000037cc*/ B           ChnlVeduIntHandler_x2da4
    ChnlVeduIntHandler_x37d0:
    /*000037d0*/ LDR         R3, [R8, #48]               @ 0x30 
    /*000037d4*/ CMP         R3, #2 
    /*000037d8*/ MOVNE       R5, R9 
    /*000037dc*/ BNE         ChnlVeduIntHandler_x3128
    ChnlVeduIntHandler_x37e0:
    /*000037e0*/ MOV         R3, #6 
    /*000037e4*/ MOV         R5, #0 
    /*000037e8*/ STR         R3, [R8, #48]               @ 0x30 
    /*000037ec*/ ADD         R0, R8, #156                @ 0x9c 
    /*000037f0*/ STR         R5, [R8, #152]              @ 0x98 
    /*000037f4*/ BL          0x00000000000037F4          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*000037f8*/ B           ChnlVeduIntHandler_x3128
    ChnlVeduIntHandler_x37fc:
    /*000037fc*/ MOVW        R3, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x858] 
    /*00003800*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00003804*/ MOVT        R3, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x858] 
    /*00003808*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*0000380c*/ MOVW        R2, #465                    @ 0x1d1 
    /*00003810*/ LDR         R1, [PC, #1164]             @ 0x0000000000003ca4 
    /*00003814*/ BL          0x0000000000003814          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003818*/ B           ChnlVeduIntHandler_x2e78
    ChnlVeduIntHandler_x381c:
    /*0000381c*/ ADD         R8, R6, #8 
    /*00003820*/ MOV         R2, #6 
    /*00003824*/ MOV         R3, #0 
    /*00003828*/ STR         R2, [R5, #48]               @ 0x30 
    /*0000382c*/ STR         R3, [R5, #152]              @ 0x98 
    /*00003830*/ ADD         R0, R5, #156                @ 0x9c 
    /*00003834*/ BL          0x0000000000003834          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00003838*/ MOV         R0, R8 
    /*0000383c*/ MOV         R2, FP 
    /*00003840*/ LDR         R1, [R6, #100]              @ 0x64 
    /*00003844*/ BL          0x0000000000003844          @ ChnlScheduler [R_ARM_CALL: .text+0xe44] 
    /*00003848*/ CMP         R0, #0 
    /*0000384c*/ MOV         R5, R0 
    /*00003850*/ STR         R0, [R4, #64]               @ 0x40 
    /*00003854*/ MOVNE       R9, SL 
    /*00003858*/ BNE         ChnlVeduIntHandler_x3474
    /*0000385c*/ CMP         SL, #0 
    /*00003860*/ BNE         ChnlVeduIntHandler_x3028
    ChnlVeduIntHandler_x3864:
    /*00003864*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x68] 
    /*00003868*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x68] 
    /*0000386c*/ LDR         R3, [R3] 
    /*00003870*/ CMP         R3, #0 
    /*00003874*/ BEQ         ChnlVeduIntHandler_x3898
    /*00003878*/ LDR         R3, [R4, #28] 
    /*0000387c*/ LDR         R3, [R3, #4] 
    /*00003880*/ TST         R3, #49                     @ 0x31 
    /*00003884*/ BEQ         ChnlVeduIntHandler_x3898
    /*00003888*/ LDR         R3, [R4, #108]              @ 0x6c 
    /*0000388c*/ MOV         R1, #0 
    /*00003890*/ LDR         R0, [R4, #24] 
    /*00003894*/ BLX         R3 
    ChnlVeduIntHandler_x3898:
    /*00003898*/ MOV         R1, #10 
    /*0000389c*/ ADD         R0, R4, #36                 @ 0x24 
    /*000038a0*/ BL          0x00000000000038A0          @ osal_set_timer [R_ARM_CALL: +0x0] 
    /*000038a4*/ MOV         R3, #0 
    /*000038a8*/ STR         R3, [R4] 
    /*000038ac*/ B           ChnlVeduIntHandler_x3048
    ChnlVeduIntHandler_x38b0:
    /*000038b0*/ MOVW        R3, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd90] 
    /*000038b4*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000038b8*/ MOVT        R3, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd90] 
    /*000038bc*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000038c0*/ MOVW        R2, #1183                   @ 0x49f 
    /*000038c4*/ LDR         R1, [PC, #976]              @ 0x0000000000003c9c 
    /*000038c8*/ BL          0x00000000000038C8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000038cc*/ LDR         R5, [R4, #64]               @ 0x40 
    /*000038d0*/ B           ChnlVeduIntHandler_x2df8
    ChnlVeduIntHandler_x38d4:
    /*000038d4*/ ADD         R0, SP, #40                 @ 0x28 
    /*000038d8*/ MOVW        R8, #16960                  @ 0x4240 
    /*000038dc*/ BL          0x00000000000038DC          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000038e0*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*000038e4*/ LDR         R0, [SP, #40]               @ 0x28 
    /*000038e8*/ MOVT        R8, #15 
    /*000038ec*/ LDR         R1, [R7] 
    /*000038f0*/ ASR         R3, R2, #31 
    /*000038f4*/ STR         SL, [R4, #64]               @ 0x40 
    /*000038f8*/ CMP         R1, R5 
    /*000038fc*/ SMLAL       R2, R3, R8, R0 
    /*00003900*/ STRD        R2, [R4, #192]              @ 0xc0 
    /*00003904*/ BNE         ChnlVeduIntHandler_x2f68
    /*00003908*/ ADD         R0, SP, #40                 @ 0x28 
    /*0000390c*/ BL          0x000000000000390C          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00003910*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*00003914*/ LDR         R1, [SP, #40]               @ 0x28 
    /*00003918*/ ASR         R3, R2, #31 
    /*0000391c*/ SMLAL       R2, R3, R8, R1 
    /*00003920*/ STRD        R2, [SL, #128]              @ 0x80 
    /*00003924*/ B           ChnlVeduIntHandler_x2f74
    ChnlVeduIntHandler_x3928:
    /*00003928*/ LDR         R9, [R8, #152]              @ 0x98 
    /*0000392c*/ MOV         R3, #2 
    /*00003930*/ STR         R3, [R8, #48]               @ 0x30 
    /*00003934*/ CMP         R9, #1 
    /*00003938*/ BEQ         ChnlVeduIntHandler_x37e0
    /*0000393c*/ MOV         R9, #1 
    /*00003940*/ B           ChnlVeduIntHandler_x3664
    ChnlVeduIntHandler_x3944:
    /*00003944*/ ADD         R0, SP, #40                 @ 0x28 
    /*00003948*/ MOVW        R3, #16960                  @ 0x4240 
    /*0000394c*/ MOVT        R3, #15 
    /*00003950*/ STR         R3, [SP, #36]               @ 0x24 
    /*00003954*/ BL          0x0000000000003954          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00003958*/ LDR         R0, [SP, #44]               @ 0x2c 
    /*0000395c*/ LDR         IP, [SP, #40]               @ 0x28 
    /*00003960*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00003964*/ ASR         R1, R0, #31 
    /*00003968*/ LDR         R2, [R7] 
    /*0000396c*/ STR         SL, [R4, #64]               @ 0x40 
    /*00003970*/ SMLAL       R0, R1, R3, IP 
    /*00003974*/ CMP         R2, FP 
    /*00003978*/ STRD        R0, [R4, #192]              @ 0xc0 
    /*0000397c*/ BNE         ChnlVeduIntHandler_x3218
    /*00003980*/ ADD         R0, SP, #40                 @ 0x28 
    /*00003984*/ STR         R3, [SP, #36]               @ 0x24 
    /*00003988*/ BL          0x0000000000003988          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*0000398c*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*00003990*/ LDR         IP, [SP, #40]               @ 0x28 
    /*00003994*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00003998*/ ASR         R1, R2, #31 
    /*0000399c*/ MOV         R0, R2 
    /*000039a0*/ SMLAL       R0, R1, R3, IP 
    /*000039a4*/ STRD        R0, [SL, #128]              @ 0x80 
    /*000039a8*/ B           ChnlVeduIntHandler_x3224
    ChnlVeduIntHandler_x39ac:
    /*000039ac*/ ADD         R0, SP, #40                 @ 0x28 
    /*000039b0*/ BL          0x00000000000039B0          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000039b4*/ LDR         R0, [SP, #44]               @ 0x2c 
    /*000039b8*/ LDR         LR, [SP, #40]               @ 0x28 
    /*000039bc*/ MOVW        IP, #16960                  @ 0x4240 
    /*000039c0*/ MOVT        IP, #15 
    /*000039c4*/ LDRD        R2, [R5, #128]              @ 0x80 
    /*000039c8*/ ASR         R1, R0, #31 
    /*000039cc*/ SMLAL       R0, R1, IP, LR 
    /*000039d0*/ MOV         SL, R0 
    /*000039d4*/ SUBS        R0, R0, R2 
    /*000039d8*/ MOV         FP, R1 
    /*000039dc*/ SBC         R1, R1, R3 
    /*000039e0*/ MOV         R2, R0 
    /*000039e4*/ STRD        SL, [SP, #24] 
    /*000039e8*/ MOV         R3, R1 
    /*000039ec*/ STRD        R2, [R5, #88]               @ 0x58 
    /*000039f0*/ LDRD        R2, [R4, #160]              @ 0xa0 
    /*000039f4*/ LDRD        R0, [R4, #192]              @ 0xc0 
    /*000039f8*/ ADDS        SL, SL, R2 
    /*000039fc*/ ADC         FP, FP, R3 
    /*00003a00*/ MOV         R2, SL 
    /*00003a04*/ SUBS        R2, R2, R0 
    /*00003a08*/ MOV         R3, FP 
    /*00003a0c*/ LDRD        SL, [SP, #24] 
    /*00003a10*/ SBC         R3, R3, R1 
    /*00003a14*/ STRD        R2, [R4, #160]              @ 0xa0 
    /*00003a18*/ MOV         R3, #0 
    /*00003a1c*/ MOVW        R2, #30000                  @ 0x7530 
    /*00003a20*/ STRD        SL, [R4, #200]              @ 0xc8 
    /*00003a24*/ LDRD        R0, [R5, #88]               @ 0x58 
    /*00003a28*/ CMP         R1, R3 
    /*00003a2c*/ CMPEQ       R0, R2 
    /*00003a30*/ BLS         ChnlVeduIntHandler_x3a44
    /*00003a34*/ LDRD        R2, [R5, #112]              @ 0x70 
    /*00003a38*/ ADDS        R2, R2, #1 
    /*00003a3c*/ ADC         R3, R3, #0 
    /*00003a40*/ STRD        R2, [R5, #112]              @ 0x70 
    ChnlVeduIntHandler_x3a44:
    /*00003a44*/ LDRD        R2, [R5, #96]               @ 0x60 
    /*00003a48*/ CMP         R1, R3 
    /*00003a4c*/ CMPEQ       R0, R2 
    /*00003a50*/ LDRD        R2, [SP, #24] 
    /*00003a54*/ BHI         ChnlVeduIntHandler_x3b2c
    /*00003a58*/ MOV         R1, R2 
    /*00003a5c*/ ORRS        R3, R1, R3 
    /*00003a60*/ MOVNE       R3, #1 
    /*00003a64*/ MOVEQ       R3, #0 
    /*00003a68*/ STR         R3, [SP, #32] 
    /*00003a6c*/ B           ChnlVeduIntHandler_x2e2c
    ChnlVeduIntHandler_x3a70:
    /*00003a70*/ MOVW        R0, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa5c] 
    /*00003a74*/ MOVT        R0, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa5c] 
    /*00003a78*/ BL          0x0000000000003A78          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00003a7c*/ MOV         R3, #2 
    /*00003a80*/ STR         R3, [R8, #48]               @ 0x30 
    /*00003a84*/ B           ChnlVeduIntHandler_x3638
    ChnlVeduIntHandler_x3a88:
    /*00003a88*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00003a8c*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00003a90*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*00003a94*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00003a98*/ MOVW        R2, #646                    @ 0x286 
    /*00003a9c*/ LDR         R1, [PC, #500]              @ 0x0000000000003c98 
    /*00003aa0*/ BL          0x0000000000003AA0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003aa4*/ B           ChnlVeduIntHandler_x3704
    ChnlVeduIntHandler_x3aa8:
    /*00003aa8*/ MOVW        R3, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa3c] 
    /*00003aac*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00003ab0*/ MOVT        R3, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa3c] 
    /*00003ab4*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00003ab8*/ MOVW        R2, #549                    @ 0x225 
    /*00003abc*/ LDR         R1, [PC, #484]              @ 0x0000000000003ca8 
    /*00003ac0*/ BL          0x0000000000003AC0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003ac4*/ LDR         R5, [R4, #60]               @ 0x3c 
    /*00003ac8*/ B           ChnlVeduIntHandler_x2f8c
    ChnlVeduIntHandler_x3acc:
    /*00003acc*/ LDR         SL, [R5, #152]              @ 0x98 
    /*00003ad0*/ MOV         R3, #2 
    /*00003ad4*/ STR         R3, [R5, #48]               @ 0x30 
    /*00003ad8*/ CMP         SL, #1 
    /*00003adc*/ BEQ         ChnlVeduIntHandler_x381c
    /*00003ae0*/ ADD         R8, R6, #8 
    /*00003ae4*/ MOV         R2, FP 
    /*00003ae8*/ LDR         R1, [R6, #100]              @ 0x64 
    /*00003aec*/ MOV         R0, R8 
    /*00003af0*/ BL          0x0000000000003AF0          @ ChnlScheduler [R_ARM_CALL: .text+0xe44] 
    /*00003af4*/ CMP         R0, #0 
    /*00003af8*/ MOV         R5, R0 
    /*00003afc*/ STR         R0, [R4, #64]               @ 0x40 
    /*00003b00*/ BNE         ChnlVeduIntHandler_x3474
    /*00003b04*/ B           ChnlVeduIntHandler_x3028
    ChnlVeduIntHandler_x3b08:
    /*00003b08*/ MOVW        R3, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa3c] 
    /*00003b0c*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00003b10*/ MOVT        R3, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa3c] 
    /*00003b14*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00003b18*/ MOVW        R2, #549                    @ 0x225 
    /*00003b1c*/ LDR         R1, [PC, #388]              @ 0x0000000000003ca8 
    /*00003b20*/ BL          0x0000000000003B20          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003b24*/ LDR         R5, [R4, #60]               @ 0x3c 
    /*00003b28*/ B           ChnlVeduIntHandler_x3248
    ChnlVeduIntHandler_x3b2c:
    /*00003b2c*/ MOV         IP, R2 
    /*00003b30*/ ORRS        R3, IP, R3 
    /*00003b34*/ STRD        R0, [R5, #96]               @ 0x60 
    /*00003b38*/ MOVNE       R3, #1 
    /*00003b3c*/ MOVEQ       R3, #0 
    /*00003b40*/ STR         R3, [SP, #32] 
    /*00003b44*/ B           ChnlVeduIntHandler_x2e2c
    ChnlVeduIntHandler_x3b48:
    /*00003b48*/ MOVW        R0, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa5c] 
    /*00003b4c*/ MOVT        R0, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa5c] 
    /*00003b50*/ BL          0x0000000000003B50          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00003b54*/ MOV         R3, #2 
    /*00003b58*/ STR         R3, [R5, #48]               @ 0x30 
    /*00003b5c*/ B           ChnlVeduIntHandler_x3414
    ChnlVeduIntHandler_x3b60:
    /*00003b60*/ ADD         R0, SP, #40                 @ 0x28 
    /*00003b64*/ MOVW        R3, #16960                  @ 0x4240 
    /*00003b68*/ MOVT        R3, #15 
    /*00003b6c*/ STR         R3, [SP, #36]               @ 0x24 
    /*00003b70*/ BL          0x0000000000003B70          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00003b74*/ LDR         R0, [SP, #44]               @ 0x2c 
    /*00003b78*/ LDR         IP, [SP, #40]               @ 0x28 
    /*00003b7c*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00003b80*/ ASR         R1, R0, #31 
    /*00003b84*/ LDR         R2, [R7] 
    /*00003b88*/ STR         R8, [R4, #64]               @ 0x40 
    /*00003b8c*/ SMLAL       R0, R1, R3, IP 
    /*00003b90*/ CMP         R2, FP 
    /*00003b94*/ STRD        R0, [R4, #192]              @ 0xc0 
    /*00003b98*/ BNE         ChnlVeduIntHandler_x3734
    /*00003b9c*/ ADD         R0, SP, #40                 @ 0x28 
    /*00003ba0*/ STR         R3, [SP, #36]               @ 0x24 
    /*00003ba4*/ BL          0x0000000000003BA4          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00003ba8*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*00003bac*/ LDR         IP, [SP, #40]               @ 0x28 
    /*00003bb0*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00003bb4*/ ASR         R1, R2, #31 
    /*00003bb8*/ MOV         R0, R2 
    /*00003bbc*/ SMLAL       R0, R1, R3, IP 
    /*00003bc0*/ STRD        R0, [R8, #128]              @ 0x80 
    /*00003bc4*/ B           ChnlVeduIntHandler_x3740
    ChnlVeduIntHandler_x3bc8:
    /*00003bc8*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00003bcc*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00003bd0*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*00003bd4*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00003bd8*/ MOVW        R2, #646                    @ 0x286 
    /*00003bdc*/ LDR         R1, [PC, #180]              @ 0x0000000000003c98 
    /*00003be0*/ BL          0x0000000000003BE0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003be4*/ B           ChnlVeduIntHandler_x3508
    ChnlVeduIntHandler_x3be8:
    /*00003be8*/ MOVW        R3, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa3c] 
    /*00003bec*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00003bf0*/ MOVT        R3, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa3c] 
    /*00003bf4*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00003bf8*/ MOVW        R2, #549                    @ 0x225 
    /*00003bfc*/ LDR         R1, [PC, #164]              @ 0x0000000000003ca8 
    /*00003c00*/ BL          0x0000000000003C00          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003c04*/ LDR         R8, [R4, #60]               @ 0x3c 
    /*00003c08*/ B           ChnlVeduIntHandler_x35e8
    ChnlVeduIntHandler_x3c0c:
    /*00003c0c*/ ADD         R0, SP, #40                 @ 0x28 
    /*00003c10*/ MOVW        R3, #16960                  @ 0x4240 
    /*00003c14*/ MOVT        R3, #15 
    /*00003c18*/ STR         R3, [SP, #36]               @ 0x24 
    /*00003c1c*/ BL          0x0000000000003C1C          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00003c20*/ LDR         R0, [SP, #44]               @ 0x2c 
    /*00003c24*/ LDR         IP, [SP, #40]               @ 0x28 
    /*00003c28*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00003c2c*/ ASR         R1, R0, #31 
    /*00003c30*/ LDR         R2, [R7] 
    /*00003c34*/ STR         R5, [R4, #64]               @ 0x40 
    /*00003c38*/ SMLAL       R0, R1, R3, IP 
    /*00003c3c*/ CMP         R2, SL 
    /*00003c40*/ STRD        R0, [R4, #192]              @ 0xc0 
    /*00003c44*/ BNE         ChnlVeduIntHandler_x3538
    /*00003c48*/ ADD         R0, SP, #40                 @ 0x28 
    /*00003c4c*/ STR         R3, [SP, #36]               @ 0x24 
    /*00003c50*/ BL          0x0000000000003C50          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00003c54*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*00003c58*/ LDR         IP, [SP, #40]               @ 0x28 
    /*00003c5c*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00003c60*/ ASR         R1, R2, #31 
    /*00003c64*/ MOV         R0, R2 
    /*00003c68*/ SMLAL       R0, R1, R3, IP 
    /*00003c6c*/ STRD        R0, [R5, #128]              @ 0x80 
    /*00003c70*/ B           ChnlVeduIntHandler_x3544
    ChnlVeduIntHandler_x3c74:
    /*00003c74*/ MOVW        R3, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa3c] 
    /*00003c78*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00003c7c*/ MOVT        R3, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa3c] 
    /*00003c80*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00003c84*/ MOVW        R2, #549                    @ 0x225 
    /*00003c88*/ LDR         R1, [PC, #24]               @ 0x0000000000003ca8 
    /*00003c8c*/ BL          0x0000000000003C8C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003c90*/ LDR         R5, [R4, #60]               @ 0x3c 
    /*00003c94*/ B           ChnlVeduIntHandler_x33c4
    /*00003c98*/ ANDEQ       R0, R0, IP, ASR R1 
    /*00003c9c*/ ANDEQ       R0, R0, R0, ROR R2 
    /*00003ca0*/ ANDEQ       R0, R0, R4, LSL #5 
    /*00003ca4*/ ANDEQ       R0, R0, IP, ASR #2 
    /*00003ca8*/ MULEQ       R0, R4, R1 
FUNC_END ChnlVeduIntHandler


FUNC_BEGIN ChnlVeduISR
    /*00003cac*/ PUSH        {R4, R5, R6, LR} 
    /*00003cb0*/ SUBS        R5, R1, #0 
    /*00003cb4*/ SUB         SP, SP, #16 
    /*00003cb8*/ BEQ         ChnlVeduISR_x3d68
    ChnlVeduISR_x3cbc:
    /*00003cbc*/ LDR         R3, [R5, #28] 
    /*00003cc0*/ CMP         R3, #0 
    /*00003cc4*/ BEQ         ChnlVeduISR_x3d88
    /*00003cc8*/ LDR         R6, [R3, #952]              @ 0x3b8 
    /*00003ccc*/ CMP         R6, #1 
    /*00003cd0*/ BNE         ChnlVeduISR_x3d3c
    /*00003cd4*/ ADD         R4, R3, #112                @ 0x70 
    /*00003cd8*/ ADD         R1, SP, #12 
    /*00003cdc*/ MOV         R0, R4 
    /*00003ce0*/ BL          0x0000000000003CE0          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00003ce4*/ MOV         R1, #4 
    /*00003ce8*/ MOVW        R3, #1517                   @ 0x5ed 
    /*00003cec*/ MOVW        R2, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdf4] 
    /*00003cf0*/ STR         R3, [SP] 
    /*00003cf4*/ MOVT        R2, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdf4] 
    /*00003cf8*/ LDR         R3, [PC, #180]              @ 0x0000000000003db4 
    /*00003cfc*/ MOV         R0, #5 
    /*00003d00*/ BL          0x0000000000003D00          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003d04*/ LDR         R2, [R5, #120]              @ 0x78 
    /*00003d08*/ LDR         R3, [R5, #128]              @ 0x80 
    /*00003d0c*/ MOV         R0, R5 
    /*00003d10*/ ADD         R2, R2, #1 
    /*00003d14*/ STR         R2, [R5, #120]              @ 0x78 
    /*00003d18*/ ADD         R3, R3, #1 
    /*00003d1c*/ STR         R3, [R5, #128]              @ 0x80 
    /*00003d20*/ BL          0x0000000000003D20          @ ChnlVeduIntHandler [R_ARM_CALL: .text+0x2d8c] 
    /*00003d24*/ MOV         R0, R4 
    /*00003d28*/ ADD         R1, SP, #12 
    /*00003d2c*/ BL          0x0000000000003D2C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00003d30*/ MOV         R0, R6 
    ChnlVeduISR_x3d34:
    /*00003d34*/ ADD         SP, SP, #16 
    /*00003d38*/ POP         {R4, R5, R6, PC} 
    ChnlVeduISR_x3d3c:
    /*00003d3c*/ MOVW        R3, #1513                   @ 0x5e9 
    /*00003d40*/ MOVW        R2, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9c8] 
    /*00003d44*/ STR         R3, [SP] 
    /*00003d48*/ MOVT        R2, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9c8] 
    /*00003d4c*/ LDR         R3, [PC, #96]               @ 0x0000000000003db4 
    /*00003d50*/ MOV         R1, #4 
    /*00003d54*/ MOV         R0, #3 
    /*00003d58*/ BL          0x0000000000003D58          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003d5c*/ MOVW        R0, #32773                  @ 0x8005 
    /*00003d60*/ MOVT        R0, #40964                  @ 0xa004 
    /*00003d64*/ B           ChnlVeduISR_x3d34
    ChnlVeduISR_x3d68:
    /*00003d68*/ MOVW        R3, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xde0] 
    /*00003d6c*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00003d70*/ MOVT        R3, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0xde0] 
    /*00003d74*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00003d78*/ MOVW        R2, #1506                   @ 0x5e2 
    /*00003d7c*/ LDR         R1, [PC, #48]               @ 0x0000000000003db4 
    /*00003d80*/ BL          0x0000000000003D80          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003d84*/ B           ChnlVeduISR_x3cbc
    ChnlVeduISR_x3d88:
    /*00003d88*/ MOVW        R3, #1512                   @ 0x5e8 
    /*00003d8c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5e0] 
    /*00003d90*/ STR         R3, [SP] 
    /*00003d94*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5e0] 
    /*00003d98*/ LDR         R3, [PC, #20]               @ 0x0000000000003db4 
    /*00003d9c*/ MOV         R1, #4 
    /*00003da0*/ MOV         R0, #3 
    /*00003da4*/ BL          0x0000000000003DA4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003da8*/ MOVW        R0, #32771                  @ 0x8003 
    /*00003dac*/ MOVT        R0, #40964                  @ 0xa004 
    /*00003db0*/ B           ChnlVeduISR_x3d34
    /*00003db4*/ MULEQ       R0, R0, R2 
FUNC_END ChnlVeduISR


FUNC_BEGIN CHNL_ScheduleAtOnce
    /*00003db8*/ PUSH        {R4, R5, R6, LR} 
    /*00003dbc*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003dc0*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003dc4*/ MOV         R3, #1 
    /*00003dc8*/ SUB         SP, SP, #16 
    /*00003dcc*/ LSL         R0, R3, R0 
    /*00003dd0*/ MOV         R2, #0 
    /*00003dd4*/ MOV         R3, R4 
    CHNL_ScheduleAtOnce_x3dd8:
    /*00003dd8*/ LDR         R1, [R3, #952]              @ 0x3b8 
    /*00003ddc*/ CMP         R1, #1 
    /*00003de0*/ BEQ         CHNL_ScheduleAtOnce_x3e24
    CHNL_ScheduleAtOnce_x3de4:
    /*00003de4*/ ADD         R2, R2, #1 
    /*00003de8*/ ADD         R3, R3, #960                @ 0x3c0 
    /*00003dec*/ CMP         R2, #5 
    /*00003df0*/ BNE         CHNL_ScheduleAtOnce_x3dd8
    /*00003df4*/ MOVW        R3, #2468                   @ 0x9a4 
    /*00003df8*/ MOV         R1, #4 
    /*00003dfc*/ STR         R3, [SP] 
    /*00003e00*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe28] 
    /*00003e04*/ LDR         R3, [PC, #96]               @ 0x0000000000003e6c 
    /*00003e08*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe28] 
    /*00003e0c*/ MOV         R0, #3 
    /*00003e10*/ BL          0x0000000000003E10          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003e14*/ MOVW        R0, #32771                  @ 0x8003 
    /*00003e18*/ MOVT        R0, #40964                  @ 0xa004 
    /*00003e1c*/ ADD         SP, SP, #16 
    /*00003e20*/ POP         {R4, R5, R6, PC} 
    CHNL_ScheduleAtOnce_x3e24:
    /*00003e24*/ LDR         R1, [R3, #4] 
    /*00003e28*/ TST         R0, R1 
    /*00003e2c*/ BEQ         CHNL_ScheduleAtOnce_x3de4
    /*00003e30*/ MOV         R6, #960                    @ 0x3c0 
    /*00003e34*/ ADD         R1, SP, #12 
    /*00003e38*/ MUL         R6, R6, R2 
    /*00003e3c*/ ADD         R5, R6, #112                @ 0x70 
    /*00003e40*/ ADD         R5, R4, R5 
    /*00003e44*/ MOV         R0, R5 
    /*00003e48*/ BL          0x0000000000003E48          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00003e4c*/ ADD         R0, R4, R6 
    /*00003e50*/ BL          0x0000000000003E50          @ ScheduleAtOnce [R_ARM_CALL: .text+0x2874] 
    /*00003e54*/ MOV         R0, R5 
    /*00003e58*/ ADD         R1, SP, #12 
    /*00003e5c*/ BL          0x0000000000003E5C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00003e60*/ MOV         R0, #0 
    /*00003e64*/ ADD         SP, SP, #16 
    /*00003e68*/ POP         {R4, R5, R6, PC} 
    /*00003e6c*/ MULEQ       R0, IP, R2 
FUNC_END CHNL_ScheduleAtOnce


FUNC_BEGIN CHNL_RegistVpu
    /*00003e70*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00003e74*/ SUB         SP, SP, #28 
    /*00003e78*/ MOV         R4, R0 
    /*00003e7c*/ BL          0x0000000000003E7C          @ ChnlCheckVpuInfo [R_ARM_CALL: .text+0x8b0] 
    /*00003e80*/ CMP         R0, #0 
    /*00003e84*/ BNE         CHNL_RegistVpu_x3ed4
    /*00003e88*/ MOV         R1, R0 
    /*00003e8c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003e90*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003e94*/ STR         R3, [SP, #12] 
    CHNL_RegistVpu_x3e98:
    /*00003e98*/ LDR         R2, [R3, #952]              @ 0x3b8 
    /*00003e9c*/ ADD         R3, R3, #960                @ 0x3c0 
    /*00003ea0*/ CMP         R2, #0 
    /*00003ea4*/ BEQ         CHNL_RegistVpu_x3ee0
    /*00003ea8*/ ADD         R1, R1, #1 
    /*00003eac*/ CMP         R1, #5 
    /*00003eb0*/ BNE         CHNL_RegistVpu_x3e98
    /*00003eb4*/ MOVW        R3, #2503                   @ 0x9c7 
    /*00003eb8*/ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xeb0] 
    /*00003ebc*/ STR         R3, [SP] 
    /*00003ec0*/ MOVT        R2, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0xeb0] 
    /*00003ec4*/ LDR         R3, [PC, #776]              @ 0x00000000000041d4 
    /*00003ec8*/ MOV         R1, #4 
    /*00003ecc*/ MOV         R0, #7 
    /*00003ed0*/ BL          0x0000000000003ED0          @ HI_LOG [R_ARM_CALL: +0x0] 
    CHNL_RegistVpu_x3ed4:
    /*00003ed4*/ MOV         R0, #0 
    CHNL_RegistVpu_x3ed8:
    /*00003ed8*/ ADD         SP, SP, #28 
    /*00003edc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    CHNL_RegistVpu_x3ee0:
    /*00003ee0*/ MOV         R0, #960                    @ 0x3c0 
    /*00003ee4*/ STR         R1, [SP, #20] 
    /*00003ee8*/ MUL         R1, R0, R1 
    /*00003eec*/ LDR         R0, [SP, #12] 
    /*00003ef0*/ LDR         R3, [R4] 
    /*00003ef4*/ MOV         R6, #1 
    /*00003ef8*/ ADD         SL, R0, R1 
    /*00003efc*/ STR         R3, [SL, #4] 
    /*00003f00*/ MOV         R0, SL 
    /*00003f04*/ LDR         R3, [R4, #4] 
    /*00003f08*/ STR         R1, [SP, #16] 
    /*00003f0c*/ STR         R2, [SL, #96]               @ 0x60 
    /*00003f10*/ STR         R6, [SL, #104]              @ 0x68 
    /*00003f14*/ STR         R3, [SL, #108]              @ 0x6c 
    /*00003f18*/ LDR         R1, [R4, #8] 
    /*00003f1c*/ BL          0x0000000000003F1C          @ ChnlInitSchdTab [R_ARM_CALL: .text+0x1b58] 
    /*00003f20*/ SUBS        R7, R0, #0 
    /*00003f24*/ BNE         CHNL_RegistVpu_x3ed4
    /*00003f28*/ LDR         R3, [SL, #108]              @ 0x6c 
    /*00003f2c*/ LDR         R5, [R4, #12] 
    /*00003f30*/ CMP         R3, #0 
    /*00003f34*/ BLE         CHNL_RegistVpu_x41c4
    /*00003f38*/ LDR         R3, [SP, #16] 
    /*00003f3c*/ MOV         R4, SL 
    /*00003f40*/ MOV         R8, SL 
    /*00003f44*/ STR         SL, [SP, #8] 
    /*00003f48*/ ADD         R6, R3, #120                @ 0x78 
    /*00003f4c*/ LDR         R3, [SP, #12] 
    /*00003f50*/ ADD         R6, R3, R6 
    /*00003f54*/ B           CHNL_RegistVpu_x4020
    CHNL_RegistVpu_x3f58:
    /*00003f58*/ BL          0x0000000000003F58          @ osal_timer_init [R_ARM_CALL: +0x0] 
    /*00003f5c*/ MOVW        R3, #0                      @ ChnlTimerISR [R_ARM_MOVW_ABS_NC: .text+0x1828] 
    /*00003f60*/ MOVT        R3, #0                      @ ChnlTimerISR [R_ARM_MOVT_ABS: .text+0x1828] 
    /*00003f64*/ ADD         R9, R4, #296                @ 0x128 
    /*00003f68*/ MOV         SL, #0 
    /*00003f6c*/ MOV         FP, #0 
    /*00003f70*/ CMP         R0, #0 
    /*00003f74*/ ADD         R0, R6, #48                 @ 0x30 
    /*00003f78*/ STREQ       R3, [R4, #160]              @ 0xa0 
    /*00003f7c*/ STREQ       R6, [R4, #164]              @ 0xa4 
    /*00003f80*/ BL          0x0000000000003F80          @ osal_timer_init [R_ARM_CALL: +0x0] 
    /*00003f84*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x68] 
    /*00003f88*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x68] 
    /*00003f8c*/ MOV         R1, #0 
    /*00003f90*/ ADD         LR, R4, #280                @ 0x118 
    /*00003f94*/ ADD         IP, R4, #288                @ 0x120 
    /*00003f98*/ LDR         R3, [R3] 
    /*00003f9c*/ MOVW        R2, #0                      @ ChnlWatchDogTimerISR [R_ARM_MOVW_ABS_NC: .text+0x1910] 
    /*00003fa0*/ MOVT        R2, #0                      @ ChnlWatchDogTimerISR [R_ARM_MOVT_ABS: .text+0x1910] 
    /*00003fa4*/ STR         R1, [R4, #240]              @ 0xf0 
    /*00003fa8*/ STR         R1, [R4, #244]              @ 0xf4 
    /*00003fac*/ STR         R1, [R4, #248]              @ 0xf8 
    /*00003fb0*/ STR         R1, [R4, #252]              @ 0xfc 
    /*00003fb4*/ STR         R1, [R4, #256]              @ 0x100 
    /*00003fb8*/ STR         R1, [R4, #260]              @ 0x104 
    /*00003fbc*/ STR         R1, [R4, #264]              @ 0x108 
    /*00003fc0*/ STR         R1, [R4, #268]              @ 0x10c 
    /*00003fc4*/ STR         R1, [R4, #272]              @ 0x110 
    /*00003fc8*/ STR         R1, [R4, #276]              @ 0x114 
    /*00003fcc*/ CMP         R0, R1 
    /*00003fd0*/ STREQ       R6, [R4, #176]              @ 0xb0 
    /*00003fd4*/ STREQ       R2, [R4, #172]              @ 0xac 
    /*00003fd8*/ CMP         R3, #0 
    /*00003fdc*/ STRD        SL, [R9] 
    /*00003fe0*/ STRD        SL, [LR] 
    /*00003fe4*/ STRD        SL, [IP] 
    /*00003fe8*/ BEQ         CHNL_RegistVpu_x4004
    /*00003fec*/ LDR         R3, [R8, #4] 
    /*00003ff0*/ TST         R3, #49                     @ 0x31 
    /*00003ff4*/ BEQ         CHNL_RegistVpu_x4004
    /*00003ff8*/ LDR         R3, [R4, #228]              @ 0xe4 
    /*00003ffc*/ LDR         R0, [R4, #144]              @ 0x90 
    /*00004000*/ BLX         R3 
    CHNL_RegistVpu_x4004:
    /*00004004*/ LDR         R3, [R8, #108]              @ 0x6c 
    /*00004008*/ ADD         R7, R7, #1 
    /*0000400c*/ ADD         R5, R5, #84                 @ 0x54 
    /*00004010*/ ADD         R4, R4, #208                @ 0xd0 
    /*00004014*/ CMP         R3, R7 
    /*00004018*/ ADD         R6, R6, #208                @ 0xd0 
    /*0000401c*/ BLE         CHNL_RegistVpu_x4154
    CHNL_RegistVpu_x4020:
    /*00004020*/ MOV         R1, R5 
    /*00004024*/ MOV         R2, #16 
    /*00004028*/ ADD         R0, R6, #8 
    /*0000402c*/ MOV         R9, #0 
    /*00004030*/ BL          0x0000000000004030          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004034*/ LDR         R0, [R5, #16] 
    /*00004038*/ LDR         R2, [SP, #8] 
    /*0000403c*/ STR         R0, [R4, #144]              @ 0x90 
    /*00004040*/ LDR         R3, [R5, #28] 
    /*00004044*/ STR         R2, [R4, #148]              @ 0x94 
    /*00004048*/ STR         R9, [R4, #184]              @ 0xb8 
    /*0000404c*/ STR         R3, [R4, #188]              @ 0xbc 
    /*00004050*/ STR         R9, [R4, #124]              @ 0x7c 
    /*00004054*/ STR         R9, [R4, #180]              @ 0xb4 
    /*00004058*/ STR         R9, [R4, #120]              @ 0x78 
    /*0000405c*/ LDR         R3, [R5, #20] 
    /*00004060*/ STR         R3, [R4, #152]              @ 0x98 
    /*00004064*/ LDR         R3, [R5, #32] 
    /*00004068*/ STR         R3, [R4, #192]              @ 0xc0 
    /*0000406c*/ LDR         R3, [R5, #36]               @ 0x24 
    /*00004070*/ STR         R3, [R4, #196]              @ 0xc4 
    /*00004074*/ LDR         R3, [R5, #40]               @ 0x28 
    /*00004078*/ STR         R3, [R4, #200]              @ 0xc8 
    /*0000407c*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*00004080*/ STR         R3, [R4, #204]              @ 0xcc 
    /*00004084*/ LDR         R2, [R5, #48]               @ 0x30 
    /*00004088*/ STR         R2, [R4, #208]              @ 0xd0 
    /*0000408c*/ LDR         R2, [R5, #52]               @ 0x34 
    /*00004090*/ STR         R2, [R4, #212]              @ 0xd4 
    /*00004094*/ LDR         R2, [R5, #56]               @ 0x38 
    /*00004098*/ STR         R2, [R4, #216]              @ 0xd8 
    /*0000409c*/ LDR         R2, [R5, #60]               @ 0x3c 
    /*000040a0*/ STR         R2, [R4, #220]              @ 0xdc 
    /*000040a4*/ LDR         R2, [R5, #64]               @ 0x40 
    /*000040a8*/ STR         R2, [R4, #224]              @ 0xe0 
    /*000040ac*/ LDR         R2, [R5, #68]               @ 0x44 
    /*000040b0*/ STR         R2, [R4, #228]              @ 0xe4 
    /*000040b4*/ LDR         R2, [R5, #72]               @ 0x48 
    /*000040b8*/ STR         R2, [R4, #232]              @ 0xe8 
    /*000040bc*/ LDR         R2, [R5, #76]               @ 0x4c 
    /*000040c0*/ STR         R2, [R4, #236]              @ 0xec 
    /*000040c4*/ BLX         R3 
    /*000040c8*/ LDR         R3, [R4, #196]              @ 0xc4 
    /*000040cc*/ LDR         R0, [R4, #144]              @ 0x90 
    /*000040d0*/ BLX         R3 
    /*000040d4*/ MOV         R2, R6 
    /*000040d8*/ MOVW        R1, #0                      @ ChnlVeduISR [R_ARM_MOVW_ABS_NC: .text+0x3cac] 
    /*000040dc*/ LDR         R0, [R4, #152]              @ 0x98 
    /*000040e0*/ MOVT        R1, #0                      @ ChnlVeduISR [R_ARM_MOVT_ABS: .text+0x3cac] 
    /*000040e4*/ BL          0x00000000000040E4          @ CHNL_IRQInit [R_ARM_CALL: .text+0xcbc] 
    /*000040e8*/ CMP         R0, R9 
    /*000040ec*/ ADD         R0, R6, #36                 @ 0x24 
    /*000040f0*/ BEQ         CHNL_RegistVpu_x3f58
    /*000040f4*/ LDR         R3, [SP, #20] 
    /*000040f8*/ MOV         R0, #960                    @ 0x3c0 
    /*000040fc*/ MOV         R2, #208                    @ 0xd0 
    /*00004100*/ LDR         R4, [SP, #12] 
    /*00004104*/ MOV         R1, #4 
    /*00004108*/ MUL         R0, R0, R3 
    /*0000410c*/ MOVW        R3, #2555                   @ 0x9fb 
    /*00004110*/ MLA         R7, R2, R7, R0 
    /*00004114*/ STR         R3, [SP] 
    /*00004118*/ MOVW        R2, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe7c] 
    /*0000411c*/ LDR         R3, [PC, #176]              @ 0x00000000000041d4 
    /*00004120*/ MOVT        R2, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe7c] 
    /*00004124*/ MOV         R0, #3 
    /*00004128*/ ADD         R7, R4, R7 
    /*0000412c*/ LDR         IP, [R7, #152]              @ 0x98 
    /*00004130*/ STR         IP, [SP, #4] 
    /*00004134*/ BL          0x0000000000004134          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004138*/ LDR         R3, [SP, #16] 
    /*0000413c*/ ADD         R0, R3, #8 
    /*00004140*/ ADD         R0, R4, R0 
    /*00004144*/ BL          0x0000000000004144          @ ChnlExitSchdTab [R_ARM_CALL: .text+0x1d4c] 
    /*00004148*/ MOV         R0, R9 
    /*0000414c*/ ADD         SP, SP, #28 
    /*00004150*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    CHNL_RegistVpu_x4154:
    /*00004154*/ CMP         R3, #0 
    /*00004158*/ LDR         SL, [SP, #8] 
    /*0000415c*/ MOV         R3, #1 
    /*00004160*/ STR         R3, [R8, #952]              @ 0x3b8 
    /*00004164*/ STR         R3, [R8, #100]              @ 0x64 
    /*00004168*/ BLE         CHNL_RegistVpu_x41b8
    /*0000416c*/ LDR         R3, [SP, #16] 
    /*00004170*/ MOV         R5, #960                    @ 0x3c0 
    /*00004174*/ LDR         R2, [SP, #20] 
    /*00004178*/ MOV         R6, #0 
    /*0000417c*/ ADD         R4, R3, #156                @ 0x9c 
    /*00004180*/ LDR         R3, [SP, #12] 
    /*00004184*/ ADD         R4, R3, R4 
    /*00004188*/ MLA         R5, R5, R2, R3 
    CHNL_RegistVpu_x418c:
    /*0000418c*/ MOV         R1, #10 
    /*00004190*/ MOV         R0, R4 
    /*00004194*/ ADD         R6, R6, #1 
    /*00004198*/ BL          0x0000000000004198          @ osal_set_timer [R_ARM_CALL: +0x0] 
    /*0000419c*/ ADD         R0, R4, #12 
    /*000041a0*/ MOVW        R1, #10000                  @ 0x2710 
    /*000041a4*/ ADD         R4, R4, #208                @ 0xd0 
    /*000041a8*/ BL          0x00000000000041A8          @ osal_set_timer [R_ARM_CALL: +0x0] 
    /*000041ac*/ LDR         R3, [R5, #108]              @ 0x6c 
    /*000041b0*/ CMP         R3, R6 
    /*000041b4*/ BGT         CHNL_RegistVpu_x418c
    CHNL_RegistVpu_x41b8:
    /*000041b8*/ MOV         R0, SL 
    /*000041bc*/ ADD         SP, SP, #28 
    /*000041c0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    CHNL_RegistVpu_x41c4:
    /*000041c4*/ MOV         R0, SL 
    /*000041c8*/ STR         R6, [SL, #952]              @ 0x3b8 
    /*000041cc*/ STR         R6, [SL, #100]              @ 0x64 
    /*000041d0*/ B           CHNL_RegistVpu_x3ed8
    /*000041d4*/ @           <UNDEFINED> instruction: 0x000002b0 
FUNC_END CHNL_RegistVpu


FUNC_BEGIN CHNL_UnregistVpu
    /*000041d8*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000041dc*/ SUBS        R6, R0, #0 
    /*000041e0*/ SUB         SP, SP, #20 
    /*000041e4*/ BEQ         CHNL_UnregistVpu_x42ac
    CHNL_UnregistVpu_x41e8:
    /*000041e8*/ LDR         R3, [R6, #952]              @ 0x3b8 
    /*000041ec*/ CMP         R3, #1 
    /*000041f0*/ BNE         CHNL_UnregistVpu_x4284
    /*000041f4*/ ADD         R4, R6, #112                @ 0x70 
    /*000041f8*/ ADD         R1, SP, #12 
    /*000041fc*/ MOV         R5, #0 
    /*00004200*/ MOV         R0, R4 
    /*00004204*/ BL          0x0000000000004204          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00004208*/ MOV         R0, R4 
    /*0000420c*/ ADD         R1, SP, #12 
    /*00004210*/ STR         R5, [R6, #952]              @ 0x3b8 
    /*00004214*/ STR         R5, [R6, #100]              @ 0x64 
    /*00004218*/ BL          0x0000000000004218          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000421c*/ LDR         R3, [R6, #108]              @ 0x6c 
    /*00004220*/ CMP         R3, R5 
    /*00004224*/ ADDGT       R4, R6, #156                @ 0x9c 
    /*00004228*/ BLE         CHNL_UnregistVpu_x4270
    CHNL_UnregistVpu_x422c:
    /*0000422c*/ ADD         R7, R4, #12 
    /*00004230*/ MOV         R0, R4 
    /*00004234*/ BL          0x0000000000004234          @ osal_del_timer [R_ARM_CALL: +0x0] 
    /*00004238*/ MOV         R0, R4 
    /*0000423c*/ BL          0x000000000000423C          @ osal_timer_destory [R_ARM_CALL: +0x0] 
    /*00004240*/ MOV         R0, R7 
    /*00004244*/ BL          0x0000000000004244          @ osal_del_timer [R_ARM_CALL: +0x0] 
    /*00004248*/ MOV         R0, R7 
    /*0000424c*/ BL          0x000000000000424C          @ osal_timer_destory [R_ARM_CALL: +0x0] 
    /*00004250*/ SUB         R1, R4, #36                 @ 0x24 
    /*00004254*/ LDR         R0, [R4, #-4] 
    /*00004258*/ ADD         R5, R5, #1 
    /*0000425c*/ BL          0x000000000000425C          @ osal_free_irq [R_ARM_CALL: +0x0] 
    /*00004260*/ LDR         R3, [R6, #108]              @ 0x6c 
    /*00004264*/ ADD         R4, R4, #208                @ 0xd0 
    /*00004268*/ CMP         R3, R5 
    /*0000426c*/ BGT         CHNL_UnregistVpu_x422c
    CHNL_UnregistVpu_x4270:
    /*00004270*/ ADD         R0, R6, #8 
    /*00004274*/ BL          0x0000000000004274          @ ChnlExitSchdTab [R_ARM_CALL: .text+0x1d4c] 
    /*00004278*/ MOV         R0, #0 
    CHNL_UnregistVpu_x427c:
    /*0000427c*/ ADD         SP, SP, #20 
    /*00004280*/ POP         {R4, R5, R6, R7, PC} 
    CHNL_UnregistVpu_x4284:
    /*00004284*/ MOVW        R3, #2637                   @ 0xa4d 
    /*00004288*/ MOVW        R2, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9c8] 
    /*0000428c*/ STR         R3, [SP] 
    /*00004290*/ MOVT        R2, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9c8] 
    /*00004294*/ LDR         R3, [PC, #48]               @ 0x00000000000042cc 
    /*00004298*/ MOV         R1, #4 
    /*0000429c*/ MOV         R0, #3 
    /*000042a0*/ BL          0x00000000000042A0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000042a4*/ MVN         R0, #0 
    /*000042a8*/ B           CHNL_UnregistVpu_x427c
    CHNL_UnregistVpu_x42ac:
    /*000042ac*/ MOVW        R3, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9b4] 
    /*000042b0*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000042b4*/ MOVT        R3, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9b4] 
    /*000042b8*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000042bc*/ MOVW        R2, #2629                   @ 0xa45 
    /*000042c0*/ LDR         R1, [PC, #4]                @ 0x00000000000042cc 
    /*000042c4*/ BL          0x00000000000042C4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000042c8*/ B           CHNL_UnregistVpu_x41e8
    /*000042cc*/ ANDEQ       R0, R0, R0, ASR #5 
FUNC_END CHNL_UnregistVpu


FUNC_BEGIN CHNL_Register
    /*000042d0*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000042d4*/ MOV         R7, R1 
    /*000042d8*/ LDR         IP, [R7] 
    /*000042dc*/ MOV         R1, #1 
    /*000042e0*/ MOVW        R8, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000042e4*/ MOVT        R8, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000042e8*/ SUB         SP, SP, #180                @ 0xb4 
    /*000042ec*/ LSL         R1, R1, IP 
    /*000042f0*/ MOV         R9, R0 
    /*000042f4*/ MOV         R3, #0 
    /*000042f8*/ MOV         R2, R8 
    CHNL_Register_x42fc:
    /*000042fc*/ LDR         R5, [R2, #952]              @ 0x3b8 
    /*00004300*/ CMP         R5, #1 
    /*00004304*/ BEQ         CHNL_Register_x4350
    CHNL_Register_x4308:
    /*00004308*/ ADD         R3, R3, #1 
    /*0000430c*/ ADD         R2, R2, #960                @ 0x3c0 
    /*00004310*/ CMP         R3, #5 
    /*00004314*/ BNE         CHNL_Register_x42fc
    /*00004318*/ MOVW        R3, #2694                   @ 0xa86 
    /*0000431c*/ STR         IP, [SP, #4] 
    /*00004320*/ STR         R3, [SP] 
    /*00004324*/ MOVW        R2, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf90] 
    /*00004328*/ LDR         R3, [PC, #588]              @ 0x000000000000457c 
    /*0000432c*/ MOVT        R2, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf90] 
    /*00004330*/ MOV         R1, #4 
    /*00004334*/ MOV         R0, #3 
    /*00004338*/ BL          0x0000000000004338          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000433c*/ MOVW        R6, #32771                  @ 0x8003 
    /*00004340*/ MOVT        R6, #40964                  @ 0xa004 
    CHNL_Register_x4344:
    /*00004344*/ MOV         R0, R6 
    /*00004348*/ ADD         SP, SP, #180                @ 0xb4 
    /*0000434c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    CHNL_Register_x4350:
    /*00004350*/ LDR         R0, [R2, #4] 
    /*00004354*/ TST         R1, R0 
    /*00004358*/ BEQ         CHNL_Register_x4308
    /*0000435c*/ CMP         R9, #0 
    /*00004360*/ BLT         CHNL_Register_x44cc
    /*00004364*/ MOV         R4, #960                    @ 0x3c0 
    /*00004368*/ MUL         R4, R4, R3 
    /*0000436c*/ ADD         SL, R8, R4 
    /*00004370*/ LDR         R3, [SL, #68]               @ 0x44 
    /*00004374*/ CMP         R9, R3 
    /*00004378*/ BGE         CHNL_Register_x44cc
    /*0000437c*/ MOV         R0, R7 
    /*00004380*/ BL          0x0000000000004380          @ ChnlCheckRegInfo [R_ARM_CALL: .text+0xa88] 
    /*00004384*/ SUBS        R6, R0, #0 
    /*00004388*/ BNE         CHNL_Register_x4344
    /*0000438c*/ ADD         FP, R4, #112                @ 0x70 
    /*00004390*/ ADD         R1, SP, #12 
    /*00004394*/ ADD         FP, R8, FP 
    /*00004398*/ MOV         R0, FP 
    /*0000439c*/ BL          0x000000000000439C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000043a0*/ MOV         R2, R9 
    /*000043a4*/ LDR         R1, [R7] 
    /*000043a8*/ MOV         R0, SL 
    /*000043ac*/ BL          0x00000000000043AC          @ ChnlQueryOneTsk [R_ARM_CALL: .text+0x1f0c] 
    /*000043b0*/ CMP         R0, #0 
    /*000043b4*/ BNE         CHNL_Register_x4558
    /*000043b8*/ LDR         R2, [SL, #24] 
    /*000043bc*/ ADD         R3, R4, #8 
    /*000043c0*/ ADD         R0, R8, R3 
    /*000043c4*/ CMP         R2, #0 
    /*000043c8*/ BLE         CHNL_Register_x4518
    /*000043cc*/ LDR         R2, [R7, #28] 
    /*000043d0*/ ADD         R1, SP, #16 
    /*000043d4*/ LDR         R3, [R7, #32] 
    /*000043d8*/ LDR         LR, [R7, #8] 
    /*000043dc*/ LDR         IP, [R7, #16] 
    /*000043e0*/ MUL         R3, R3, R2 
    /*000043e4*/ LDR         R4, [R7, #12] 
    /*000043e8*/ LDR         R2, [R7] 
    /*000043ec*/ STR         LR, [SP, #32] 
    /*000043f0*/ STR         IP, [SP, #44]               @ 0x2c 
    /*000043f4*/ LSL         R5, R5, R2 
    /*000043f8*/ LDR         LR, [R7, #24] 
    /*000043fc*/ LDR         IP, [R7, #36]               @ 0x24 
    /*00004400*/ STR         R3, [SP, #60]               @ 0x3c 
    /*00004404*/ LDR         R3, [R7, #20] 
    /*00004408*/ STR         R6, [SP, #56]               @ 0x38 
    /*0000440c*/ STR         R6, [SP, #76]               @ 0x4c 
    /*00004410*/ STR         R6, [SP, #84]               @ 0x54 
    /*00004414*/ STR         R6, [SP, #80]               @ 0x50 
    /*00004418*/ STR         R6, [SP, #88]               @ 0x58 
    /*0000441c*/ STR         R6, [SP, #96]               @ 0x60 
    /*00004420*/ STR         R6, [SP, #100]              @ 0x64 
    /*00004424*/ STR         R6, [SP, #120]              @ 0x78 
    /*00004428*/ STR         R6, [SP, #164]              @ 0xa4 
    /*0000442c*/ STR         R2, [SP, #24] 
    /*00004430*/ MOV         R2, #0 
    /*00004434*/ STR         R3, [SP, #48]               @ 0x30 
    /*00004438*/ MOV         R3, #0 
    /*0000443c*/ STR         LR, [SP, #52]               @ 0x34 
    /*00004440*/ STRD        R2, [SP, #104]              @ 0x68 
    /*00004444*/ STRD        R2, [SP, #112]              @ 0x70 
    /*00004448*/ STRD        R2, [SP, #128]              @ 0x80 
    /*0000444c*/ STRD        R2, [SP, #144]              @ 0x90 
    /*00004450*/ STR         IP, [SP, #68]               @ 0x44 
    /*00004454*/ MOV         IP, #2 
    /*00004458*/ STR         R9, [SP, #16] 
    /*0000445c*/ STR         R5, [SP, #28] 
    /*00004460*/ STR         R4, [SP, #40]               @ 0x28 
    /*00004464*/ STR         R6, [SP, #168]              @ 0xa8 
    /*00004468*/ LDR         R3, [R7, #4] 
    /*0000446c*/ STR         R6, [SP, #156]              @ 0x9c 
    /*00004470*/ AND         R3, R3, #1 
    /*00004474*/ STR         IP, [SP, #64]               @ 0x40 
    /*00004478*/ STR         R3, [SP, #20] 
    /*0000447c*/ BL          0x000000000000447C          @ ChnlAddOneTask [R_ARM_CALL: .text+0x21f4] 
    /*00004480*/ SUBS        R6, R0, #0 
    /*00004484*/ BNE         CHNL_Register_x44f8
    CHNL_Register_x4488:
    /*00004488*/ LDR         R3, [SL, #44]               @ 0x2c 
    /*0000448c*/ MOV         R0, FP 
    /*00004490*/ LDR         R2, [SL, #64]               @ 0x40 
    /*00004494*/ ADD         R1, SP, #12 
    /*00004498*/ CMP         R3, #0 
    /*0000449c*/ MOVGT       R3, #1 
    /*000044a0*/ MOVLE       R3, #0 
    /*000044a4*/ CMP         R2, #0 
    /*000044a8*/ ADDGT       R3, R3, #1 
    /*000044ac*/ CMP         R3, #1 
    /*000044b0*/ MOVLE       R3, #0 
    /*000044b4*/ MOVGT       R3, #1 
    /*000044b8*/ STR         R3, [SL, #92]               @ 0x5c 
    /*000044bc*/ BL          0x00000000000044BC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000044c0*/ MOV         R0, R6 
    /*000044c4*/ ADD         SP, SP, #180                @ 0xb4 
    /*000044c8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    CHNL_Register_x44cc:
    /*000044cc*/ MOVW        R3, #2699                   @ 0xa8b 
    /*000044d0*/ MOVW        R2, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xeec] 
    /*000044d4*/ STR         R3, [SP] 
    /*000044d8*/ MOVT        R2, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0xeec] 
    /*000044dc*/ LDR         R3, [PC, #152]              @ 0x000000000000457c 
    /*000044e0*/ MOV         R1, #4 
    /*000044e4*/ MOV         R0, #3 
    /*000044e8*/ MOVW        R6, #32771                  @ 0x8003 
    /*000044ec*/ BL          0x00000000000044EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000044f0*/ MOVT        R6, #40964                  @ 0xa004 
    /*000044f4*/ B           CHNL_Register_x4344
    CHNL_Register_x44f8:
    /*000044f8*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*000044fc*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00004500*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*00004504*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00004508*/ MOVW        R2, #2759                   @ 0xac7 
    /*0000450c*/ LDR         R1, [PC, #104]              @ 0x000000000000457c 
    /*00004510*/ BL          0x0000000000004510          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004514*/ B           CHNL_Register_x4488
    CHNL_Register_x4518:
    /*00004518*/ MOV         R3, #2720                   @ 0xaa0 
    /*0000451c*/ STR         R3, [SP] 
    /*00004520*/ LDR         R3, [PC, #84]               @ 0x000000000000457c 
    /*00004524*/ MOVW        R2, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfe4] 
    /*00004528*/ MOVT        R2, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfe4] 
    /*0000452c*/ MOV         R1, #4 
    /*00004530*/ MOV         R0, #3 
    /*00004534*/ MOVW        R6, #32783                  @ 0x800f 
    CHNL_Register_x4538:
    /*00004538*/ BL          0x0000000000004538          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000453c*/ MOV         R0, FP 
    /*00004540*/ ADD         R1, SP, #12 
    /*00004544*/ MOVT        R6, #40964                  @ 0xa004 
    /*00004548*/ BL          0x0000000000004548          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000454c*/ MOV         R0, R6 
    /*00004550*/ ADD         SP, SP, #180                @ 0xb4 
    /*00004554*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    CHNL_Register_x4558:
    /*00004558*/ MOVW        R3, #2711                   @ 0xa97 
    /*0000455c*/ MOVW        R2, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf40] 
    /*00004560*/ STR         R3, [SP] 
    /*00004564*/ MOVT        R2, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf40] 
    /*00004568*/ LDR         R3, [PC, #12]               @ 0x000000000000457c 
    /*0000456c*/ MOV         R1, #4 
    /*00004570*/ MOV         R0, #3 
    /*00004574*/ MOVW        R6, #32772                  @ 0x8004 
    /*00004578*/ B           CHNL_Register_x4538
    /*0000457c*/ LDRDEQ      R0, [R0], -R4 
FUNC_END CHNL_Register


FUNC_BEGIN CHNL_UnRegister
    /*00004580*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00004584*/ MOV         R7, R1 
    /*00004588*/ LDR         R3, [R7] 
    /*0000458c*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00004590*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00004594*/ MOV         R1, #1 
    /*00004598*/ SUB         SP, SP, #16 
    /*0000459c*/ MOV         R8, R0 
    /*000045a0*/ LSL         R1, R1, R3 
    /*000045a4*/ MOV         R0, #0 
    /*000045a8*/ MOV         R3, R5 
    CHNL_UnRegister_x45ac:
    /*000045ac*/ LDR         R2, [R3, #952]              @ 0x3b8 
    /*000045b0*/ CMP         R2, #1 
    /*000045b4*/ BEQ         CHNL_UnRegister_x45fc
    CHNL_UnRegister_x45b8:
    /*000045b8*/ ADD         R0, R0, #1 
    /*000045bc*/ ADD         R3, R3, #960                @ 0x3c0 
    /*000045c0*/ CMP         R0, #5 
    /*000045c4*/ BNE         CHNL_UnRegister_x45ac
    /*000045c8*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe28] 
    /*000045cc*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe28] 
    /*000045d0*/ MOVW        R3, #2798                   @ 0xaee 
    /*000045d4*/ STR         R3, [SP] 
    CHNL_UnRegister_x45d8:
    /*000045d8*/ LDR         R3, [PC, #472]              @ 0x00000000000047b8 
    /*000045dc*/ MOV         R1, #4 
    /*000045e0*/ MOV         R0, #3 
    /*000045e4*/ MOVW        R6, #32771                  @ 0x8003 
    /*000045e8*/ BL          0x00000000000045E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000045ec*/ MOVT        R6, #40964                  @ 0xa004 
    CHNL_UnRegister_x45f0:
    /*000045f0*/ MOV         R0, R6 
    /*000045f4*/ ADD         SP, SP, #16 
    /*000045f8*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    CHNL_UnRegister_x45fc:
    /*000045fc*/ LDR         R2, [R3, #4] 
    /*00004600*/ TST         R1, R2 
    /*00004604*/ BEQ         CHNL_UnRegister_x45b8
    /*00004608*/ CMP         R8, #0 
    /*0000460c*/ BLT         CHNL_UnRegister_x476c
    /*00004610*/ MOV         R4, #960                    @ 0x3c0 
    /*00004614*/ MUL         R4, R4, R0 
    /*00004618*/ ADD         R9, R5, R4 
    /*0000461c*/ LDR         R3, [R9, #68]               @ 0x44 
    /*00004620*/ CMP         R8, R3 
    /*00004624*/ BGE         CHNL_UnRegister_x476c
    /*00004628*/ MOV         R0, R7 
    /*0000462c*/ BL          0x000000000000462C          @ ChnlCheckUnregInfo [R_ARM_CALL: .text+0xb7c] 
    /*00004630*/ SUBS        R6, R0, #0 
    /*00004634*/ BNE         CHNL_UnRegister_x45f0
    /*00004638*/ ADD         SL, R4, #112                @ 0x70 
    /*0000463c*/ ADD         R1, SP, #12 
    /*00004640*/ ADD         SL, R5, SL 
    /*00004644*/ MOV         R0, SL 
    /*00004648*/ BL          0x0000000000004648          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000464c*/ MOV         R2, R8 
    /*00004650*/ LDR         R1, [R7] 
    /*00004654*/ MOV         R0, R9 
    /*00004658*/ BL          0x0000000000004658          @ ChnlQueryOneTsk [R_ARM_CALL: .text+0x1f0c] 
    /*0000465c*/ SUBS        R6, R0, #0 
    /*00004660*/ BEQ         CHNL_UnRegister_x4780
    /*00004664*/ ADD         R1, SP, #12 
    /*00004668*/ MOV         R0, SL 
    /*0000466c*/ BL          0x000000000000466C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00004670*/ MOV         R1, R8 
    /*00004674*/ LDR         R0, [R7] 
    /*00004678*/ BL          0x0000000000004678          @ CHNL_StopSchedule [R_ARM_CALL: .text+0x2370] 
    /*0000467c*/ CMP         R0, #0 
    /*00004680*/ BNE         CHNL_UnRegister_x472c
    CHNL_UnRegister_x4684:
    /*00004684*/ ADD         R1, SP, #12 
    /*00004688*/ MOV         R0, SL 
    /*0000468c*/ BL          0x000000000000468C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00004690*/ LDR         R3, [R6, #48]               @ 0x30 
    /*00004694*/ CMP         R3, #6 
    /*00004698*/ BEQ         CHNL_UnRegister_x46b8
    /*0000469c*/ MOVW        R3, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a8] 
    /*000046a0*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*000046a4*/ MOVT        R3, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a8] 
    /*000046a8*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*000046ac*/ MOV         R2, #2832                   @ 0xb10 
    /*000046b0*/ LDR         R1, [PC, #256]              @ 0x00000000000047b8 
    /*000046b4*/ BL          0x00000000000046B4          @ osal_panic [R_ARM_CALL: +0x0] 
    CHNL_UnRegister_x46b8:
    /*000046b8*/ LDR         R3, [R7, #4] 
    /*000046bc*/ CMP         R3, #0 
    /*000046c0*/ BEQ         CHNL_UnRegister_x46d0
    /*000046c4*/ LDR         R1, [R6, #52]               @ 0x34 
    /*000046c8*/ LDR         R0, [R6] 
    /*000046cc*/ BLX         R3 
    CHNL_UnRegister_x46d0:
    /*000046d0*/ ADD         R0, R4, #8 
    /*000046d4*/ MOV         R1, R6 
    /*000046d8*/ ADD         R0, R5, R0 
    /*000046dc*/ BL          0x00000000000046DC          @ ChnlDelOneTask [R_ARM_CALL: .text+0x2000] 
    /*000046e0*/ SUBS        R6, R0, #0 
    /*000046e4*/ BNE         CHNL_UnRegister_x474c
    CHNL_UnRegister_x46e8:
    /*000046e8*/ LDR         R3, [R9, #44]               @ 0x2c 
    /*000046ec*/ MOV         R0, SL 
    /*000046f0*/ LDR         R2, [R9, #64]               @ 0x40 
    /*000046f4*/ ADD         R1, SP, #12 
    /*000046f8*/ CMP         R3, #0 
    /*000046fc*/ MOVGT       R3, #1 
    /*00004700*/ MOVLE       R3, #0 
    /*00004704*/ CMP         R2, #0 
    /*00004708*/ ADDGT       R3, R3, #1 
    /*0000470c*/ CMP         R3, #1 
    /*00004710*/ MOVLE       R3, #0 
    /*00004714*/ MOVGT       R3, #1 
    /*00004718*/ STR         R3, [R9, #92]               @ 0x5c 
    /*0000471c*/ BL          0x000000000000471C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00004720*/ MOV         R0, R6 
    /*00004724*/ ADD         SP, SP, #16 
    /*00004728*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    CHNL_UnRegister_x472c:
    /*0000472c*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00004730*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00004734*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*00004738*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*0000473c*/ MOVW        R2, #2828                   @ 0xb0c 
    /*00004740*/ LDR         R1, [PC, #112]              @ 0x00000000000047b8 
    /*00004744*/ BL          0x0000000000004744          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004748*/ B           CHNL_UnRegister_x4684
    CHNL_UnRegister_x474c:
    /*0000474c*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00004750*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00004754*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*00004758*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*0000475c*/ MOVW        R2, #2843                   @ 0xb1b 
    /*00004760*/ LDR         R1, [PC, #80]               @ 0x00000000000047b8 
    /*00004764*/ BL          0x0000000000004764          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004768*/ B           CHNL_UnRegister_x46e8
    CHNL_UnRegister_x476c:
    /*0000476c*/ MOVW        R3, #2803                   @ 0xaf3 
    /*00004770*/ MOVW        R2, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xeec] 
    /*00004774*/ STR         R3, [SP] 
    /*00004778*/ MOVT        R2, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0xeec] 
    /*0000477c*/ B           CHNL_UnRegister_x45d8
    CHNL_UnRegister_x4780:
    /*00004780*/ MOVW        R3, #2817                   @ 0xb01 
    /*00004784*/ MOV         R1, #4 
    /*00004788*/ STR         R3, [SP] 
    /*0000478c*/ MOVW        R2, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x105c] 
    /*00004790*/ LDR         R3, [PC, #32]               @ 0x00000000000047b8 
    /*00004794*/ MOVT        R2, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0x105c] 
    /*00004798*/ MOV         R0, #3 
    /*0000479c*/ MOVW        R6, #32773                  @ 0x8005 
    /*000047a0*/ BL          0x00000000000047A0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000047a4*/ ADD         R1, SP, #12 
    /*000047a8*/ MOV         R0, SL 
    /*000047ac*/ MOVT        R6, #40964                  @ 0xa004 
    /*000047b0*/ BL          0x00000000000047B0          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000047b4*/ B           CHNL_UnRegister_x45f0
    /*000047b8*/ ANDEQ       R0, R0, R4, ROR #5 
FUNC_END CHNL_UnRegister


FUNC_BEGIN CHNL_Notify
    /*000047bc*/ CMP         R0, #17 
    /*000047c0*/ BXNE        LR 
    /*000047c4*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000047c8*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000047cc*/ ADD         R1, R3, #4800               @ 0x12c0 
    /*000047d0*/ MOV         R0, #0 
    CHNL_Notify_x47d4:
    /*000047d4*/ LDR         R2, [R3, #952]              @ 0x3b8 
    /*000047d8*/ ADD         R3, R3, #960                @ 0x3c0 
    /*000047dc*/ CMP         R2, #1 
    /*000047e0*/ STREQ       R0, [R3, #-860]             @ 0xfffffca4 
    /*000047e4*/ CMP         R3, R1 
    /*000047e8*/ BNE         CHNL_Notify_x47d4
    /*000047ec*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x68] 
    /*000047f0*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x68] 
    /*000047f4*/ MOV         R2, #1 
    /*000047f8*/ STR         R2, [R3, #4] 
    /*000047fc*/ BX          LR 
FUNC_END CHNL_Notify


FUNC_BEGIN CHNL_QueryState
    /*00004800*/ PUSH        {R4, LR} 
    /*00004804*/ SUB         SP, SP, #8 
    /*00004808*/ MOV         R4, R0 
    /*0000480c*/ ADD         R0, SP, #4 
    /*00004810*/ BL          0x0000000000004810          @ CHNL_GetSchedule [R_ARM_CALL: .text+0x22ec] 
    /*00004814*/ LDR         R3, [SP, #4] 
    /*00004818*/ CMP         R3, #0 
    /*0000481c*/ MOVNE       R3, #34                     @ 0x22 
    /*00004820*/ MOVEQ       R3, #17 
    /*00004824*/ STR         R3, [R4] 
    /*00004828*/ ADD         SP, SP, #8 
    /*0000482c*/ POP         {R4, PC} 
FUNC_END CHNL_QueryState


FUNC_BEGIN CHNL_CheckRegistered
    /*00004830*/ CMP         R0, #127                    @ 0x7f 
    /*00004834*/ PUSH        {R4, LR} 
    /*00004838*/ MOV         R4, R0 
    /*0000483c*/ BHI         CHNL_CheckRegistered_x487c
    CHNL_CheckRegistered_x4840:
    /*00004840*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00004844*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00004848*/ ADD         R2, R3, #4800               @ 0x12c0 
    CHNL_CheckRegistered_x484c:
    /*0000484c*/ LDR         R0, [R3, #952]              @ 0x3b8 
    /*00004850*/ CMP         R0, #1 
    /*00004854*/ BEQ         CHNL_CheckRegistered_x486c
    CHNL_CheckRegistered_x4858:
    /*00004858*/ ADD         R3, R3, #960                @ 0x3c0 
    /*0000485c*/ CMP         R3, R2 
    /*00004860*/ BNE         CHNL_CheckRegistered_x484c
    /*00004864*/ MOV         R0, #0 
    /*00004868*/ POP         {R4, PC} 
    CHNL_CheckRegistered_x486c:
    /*0000486c*/ LDR         R1, [R3, #4] 
    /*00004870*/ TST         R4, R1 
    /*00004874*/ BEQ         CHNL_CheckRegistered_x4858
    /*00004878*/ POP         {R4, PC} 
    CHNL_CheckRegistered_x487c:
    /*0000487c*/ MOVW        R3, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10d0] 
    /*00004880*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00004884*/ MOVT        R3, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10d0] 
    /*00004888*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*0000488c*/ MOVW        R2, #2914                   @ 0xb62 
    /*00004890*/ LDR         R1, [PC, #4]                @ 0x000000000000489c 
    /*00004894*/ BL          0x0000000000004894          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004898*/ B           CHNL_CheckRegistered_x4840
    /*0000489c*/ STRDEQ      R0, [R0], -R4 
FUNC_END CHNL_CheckRegistered


FUNC_BEGIN CHNL_GetVpuState
    /*000048a0*/ CMP         R0, #127                    @ 0x7f 
    /*000048a4*/ MOV         R3, #0 
    /*000048a8*/ PUSH        {R4, LR} 
    /*000048ac*/ SUB         SP, SP, #8 
    /*000048b0*/ STR         R3, [R1] 
    /*000048b4*/ MOVWLS      R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000048b8*/ MOVTLS      R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000048bc*/ ADDLS       R4, R3, #4800               @ 0x12c0 
    /*000048c0*/ BHI         CHNL_GetVpuState_x4958
    CHNL_GetVpuState_x48c4:
    /*000048c4*/ LDR         R2, [R3, #952]              @ 0x3b8 
    /*000048c8*/ CMP         R2, #1 
    /*000048cc*/ BEQ         CHNL_GetVpuState_x48e8
    CHNL_GetVpuState_x48d0:
    /*000048d0*/ ADD         R3, R3, #960                @ 0x3c0 
    /*000048d4*/ CMP         R3, R4 
    /*000048d8*/ BNE         CHNL_GetVpuState_x48c4
    CHNL_GetVpuState_x48dc:
    /*000048dc*/ MOV         R0, #0 
    /*000048e0*/ ADD         SP, SP, #8 
    /*000048e4*/ POP         {R4, PC} 
    CHNL_GetVpuState_x48e8:
    /*000048e8*/ LDR         R2, [R3, #4] 
    /*000048ec*/ TST         R0, R2 
    /*000048f0*/ BEQ         CHNL_GetVpuState_x48d0
    /*000048f4*/ LDR         R2, [R3, #108]              @ 0x6c 
    /*000048f8*/ CMP         R2, #0 
    /*000048fc*/ BLE         CHNL_GetVpuState_x48d0
    /*00004900*/ LDR         R2, [R3, #120]              @ 0x78 
    /*00004904*/ LDR         IP, [R1] 
    /*00004908*/ CMP         R2, #0 
    /*0000490c*/ ORRNE       IP, IP, #1 
    /*00004910*/ STR         IP, [R1] 
    /*00004914*/ CMP         IP, #0 
    /*00004918*/ BNE         CHNL_GetVpuState_x48dc
    /*0000491c*/ MOV         LR, R3 
    CHNL_GetVpuState_x4920:
    /*00004920*/ LDR         R2, [R3, #108]              @ 0x6c 
    /*00004924*/ ADD         IP, IP, #1 
    /*00004928*/ CMP         R2, IP 
    /*0000492c*/ BLE         CHNL_GetVpuState_x48d0
    /*00004930*/ LDR         R2, [LR, #328]              @ 0x148 
    /*00004934*/ ADD         LR, LR, #208                @ 0xd0 
    /*00004938*/ ADDS        R2, R2, #0 
    /*0000493c*/ MOVNE       R2, #1 
    /*00004940*/ CMP         R2, #0 
    /*00004944*/ STR         R2, [R1] 
    /*00004948*/ BEQ         CHNL_GetVpuState_x4920
    /*0000494c*/ MOV         R0, #0 
    /*00004950*/ ADD         SP, SP, #8 
    /*00004954*/ POP         {R4, PC} 
    CHNL_GetVpuState_x4958:
    /*00004958*/ MOV         R4, R1 
    /*0000495c*/ MOVW        R3, #2948                   @ 0xb84 
    /*00004960*/ MOV         R1, #4 
    /*00004964*/ STR         R3, [SP] 
    /*00004968*/ MOVW        R2, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10d4] 
    /*0000496c*/ LDR         R3, [PC, #28]               @ 0x0000000000004990 
    /*00004970*/ MOVT        R2, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10d4] 
    /*00004974*/ MOV         R0, #3 
    /*00004978*/ BL          0x0000000000004978          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000497c*/ MOV         R3, #1 
    /*00004980*/ MOV         R0, #0 
    /*00004984*/ STR         R3, [R4] 
    /*00004988*/ ADD         SP, SP, #8 
    /*0000498c*/ POP         {R4, PC} 
    /*00004990*/ ANDEQ       R0, R0, IP, LSL #6 
FUNC_END CHNL_GetVpuState


FUNC_BEGIN CHNL_Init
    /*00004994*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x68] 
    /*00004998*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x68] 
    /*0000499c*/ PUSH        {R4, LR} 
    /*000049a0*/ SUB         SP, SP, #8 
    /*000049a4*/ LDR         R4, [R3, #4] 
    /*000049a8*/ CMP         R4, #0 
    /*000049ac*/ BEQ         CHNL_Init_x49cc
    /*000049b0*/ CMP         R4, #1 
    /*000049b4*/ MOVNE       R4, #0 
    /*000049b8*/ STRNE       R4, [R3, #4] 
    /*000049bc*/ BEQ         CHNL_Init_x49f8
    CHNL_Init_x49c0:
    /*000049c0*/ MOV         R0, R4 
    /*000049c4*/ ADD         SP, SP, #8 
    /*000049c8*/ POP         {R4, PC} 
    CHNL_Init_x49cc:
    /*000049cc*/ MOVW        R3, #3003                   @ 0xbbb 
    /*000049d0*/ MOV         R1, #4 
    /*000049d4*/ STR         R3, [SP] 
    /*000049d8*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1100] 
    /*000049dc*/ LDR         R3, [PC, #64]               @ 0x0000000000004a24 
    /*000049e0*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1100] 
    /*000049e4*/ MOV         R0, #5 
    /*000049e8*/ BL          0x00000000000049E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000049ec*/ MOV         R0, R4 
    /*000049f0*/ ADD         SP, SP, #8 
    /*000049f4*/ POP         {R4, PC} 
    CHNL_Init_x49f8:
    /*000049f8*/ MOVW        R3, #3009                   @ 0xbc1 
    /*000049fc*/ MOVW        R2, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1134] 
    /*00004a00*/ STR         R3, [SP] 
    /*00004a04*/ MOVT        R2, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1134] 
    /*00004a08*/ LDR         R3, [PC, #20]               @ 0x0000000000004a24 
    /*00004a0c*/ MOV         R1, #4 
    /*00004a10*/ MOV         R0, #3 
    /*00004a14*/ MOVW        R4, #32777                  @ 0x8009 
    /*00004a18*/ BL          0x0000000000004A18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004a1c*/ MOVT        R4, #40964                  @ 0xa004 
    /*00004a20*/ B           CHNL_Init_x49c0
    /*00004a24*/ ANDEQ       R0, R0, R0, LSR #6 
FUNC_END CHNL_Init


FUNC_BEGIN CHNL_Exit
    /*00004a28*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x68] 
    /*00004a2c*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x68] 
    /*00004a30*/ LDR         R2, [R3, #4] 
    /*00004a34*/ CMP         R2, #2 
    /*00004a38*/ BEQ         CHNL_Exit_x4a48
    /*00004a3c*/ MOV         R2, #2 
    /*00004a40*/ STR         R2, [R3, #4] 
    /*00004a44*/ BX          LR 
    CHNL_Exit_x4a48:
    /*00004a48*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00004a4c*/ SUB         SP, SP, #12 
    /*00004a50*/ MOV         R3, #3040                   @ 0xbe0 
    /*00004a54*/ MOV         R1, #4 
    /*00004a58*/ STR         R3, [SP] 
    /*00004a5c*/ MOVW        R2, #0                      @ .LC58 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x116c] 
    /*00004a60*/ LDR         R3, [PC, #16]               @ 0x0000000000004a78 
    /*00004a64*/ MOVT        R2, #0                      @ .LC58 [R_ARM_MOVT_ABS: .rodata.str1.4+0x116c] 
    /*00004a68*/ MOV         R0, #3 
    /*00004a6c*/ BL          0x0000000000004A6C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004a70*/ ADD         SP, SP, #12 
    /*00004a74*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00004a78*/ ANDEQ       R0, R0, IP, LSR #6 
FUNC_END CHNL_Exit


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .init.text
@ Size: 0x10
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN chnl_mod_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ BL          0x0000000000000004          @ CHNL_ModInit [R_ARM_CALL: .text+0x678] 
    /*00000008*/ MOV         R0, #0 
    /*0000000c*/ POP         {R4, PC} 
FUNC_END chnl_mod_init




@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .exit.text
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN chnl_mod_exit
    chnl_mod_exit_x0:
    /*00000000*/ B           chnl_mod_exit_x0            @ CHNL_ModExit [R_ARM_JUMP24: .text+0x80c] 
FUNC_END chnl_mod_exit




