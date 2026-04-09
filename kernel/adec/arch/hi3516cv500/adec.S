@
@ -- FILES --
@

@ adec.c
@ adec_init.c
@ hi3516cv500_adec.mod.c


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text
@ Size: 0x804
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN adec_poll
    /*00000000*/ MOV         R0, #0 
    /*00000004*/ BX          LR 
FUNC_END adec_poll


FUNC_BEGIN adec_restore
    /*00000008*/ MOV         R0, #0 
    /*0000000c*/ BX          LR 
FUNC_END adec_restore


FUNC_BEGIN ADEC_Exit
    /*00000010*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000014*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000018*/ LDR         R2, [R3] 
    /*0000001c*/ CMP         R2, #2 
    /*00000020*/ MOVNE       R2, #2 
    /*00000024*/ STRNE       R2, [R3] 
    /*00000028*/ BX          LR 
FUNC_END ADEC_Exit


FUNC_BEGIN ADEC_Notify
    /*0000002c*/ BX          LR 
FUNC_END ADEC_Notify


FUNC_BEGIN ADEC_GetVerMagic
    /*00000030*/ MOVW        R0, #61479                  @ 0xf027 
    /*00000034*/ MOVT        R0, #307                    @ 0x133 
    /*00000038*/ BX          LR 
FUNC_END ADEC_GetVerMagic


FUNC_BEGIN AdecProcShow
    /*0000003c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000040*/ MOVW        R1, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe4] 
    /*00000044*/ SUB         SP, SP, #36                 @ 0x24 
    /*00000048*/ MOV         R7, R0 
    /*0000004c*/ MOVT        R1, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe4] 
    /*00000050*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000054*/ BL          0x0000000000000054          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000058*/ MOV         R0, R7 
    /*0000005c*/ MOVW        R1, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x144] 
    /*00000060*/ MOVT        R1, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x144] 
    /*00000064*/ BL          0x0000000000000064          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000068*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d0] 
    /*0000006c*/ MOVW        R3, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d8] 
    /*00000070*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d0] 
    /*00000074*/ MOVT        R3, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d8] 
    /*00000078*/ STR         R2, [SP, #28] 
    /*0000007c*/ STR         R3, [SP, #24] 
    /*00000080*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e0] 
    /*00000084*/ MOVW        R3, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e8] 
    /*00000088*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e0] 
    /*0000008c*/ MOVT        R3, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e8] 
    /*00000090*/ STR         R2, [SP, #20] 
    /*00000094*/ STR         R3, [SP, #16] 
    /*00000098*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f4] 
    /*0000009c*/ MOVW        R3, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fc] 
    /*000000a0*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f4] 
    /*000000a4*/ MOVT        R3, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fc] 
    /*000000a8*/ STR         R2, [SP, #12] 
    /*000000ac*/ STR         R3, [SP, #8] 
    /*000000b0*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x208] 
    /*000000b4*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x210] 
    /*000000b8*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x208] 
    /*000000bc*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x210] 
    /*000000c0*/ STR         R2, [SP, #4] 
    /*000000c4*/ STR         R3, [SP] 
    /*000000c8*/ MOV         R0, R7 
    /*000000cc*/ MOVW        R3, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x198] 
    /*000000d0*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a0] 
    /*000000d4*/ MOVT        R3, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x198] 
    /*000000d8*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a0] 
    /*000000dc*/ MOVW        R1, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a8] 
    /*000000e0*/ MOVT        R1, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a8] 
    /*000000e4*/ MOVW        R6, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*000000e8*/ MOVW        R9, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd4] 
    /*000000ec*/ MOVW        R8, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*000000f0*/ BL          0x00000000000000F0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000000f4*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000000f8*/ MOVT        R6, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*000000fc*/ MOVT        R9, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd4] 
    /*00000100*/ MOVT        R8, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*00000104*/ MOV         R5, #0 
    /*00000108*/ B           AdecProcShow_x26c
    AdecProcShow_x10c:
    /*0000010c*/ CMP         R3, #20 
    /*00000110*/ BEQ         AdecProcShow_x290
    /*00000114*/ CMP         R3, #49                     @ 0x31 
    /*00000118*/ BEQ         AdecProcShow_x2b4
    /*0000011c*/ CMP         R3, #21 
    /*00000120*/ BEQ         AdecProcShow_x2c0
    /*00000124*/ MOVW        R2, #1001                   @ 0x3e9 
    /*00000128*/ CMP         R3, R2 
    /*0000012c*/ BEQ         AdecProcShow_x2d8
    /*00000130*/ MOVW        R2, #1003                   @ 0x3eb 
    /*00000134*/ CMP         R3, R2 
    /*00000138*/ BEQ         AdecProcShow_x2f0
    /*0000013c*/ CMP         R3, #37                     @ 0x25 
    /*00000140*/ BEQ         AdecProcShow_x308
    /*00000144*/ CMP         R3, #23 
    /*00000148*/ BEQ         AdecProcShow_x320
    /*0000014c*/ CMP         R3, #43                     @ 0x2b 
    /*00000150*/ MOVW        R1, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3c] 
    /*00000154*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x40] 
    /*00000158*/ MOVT        R1, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3c] 
    /*0000015c*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x40] 
    /*00000160*/ MOVEQ       R3, R1 
    /*00000164*/ MOVNE       R3, R2 
    AdecProcShow_x168:
    /*00000168*/ LDR         R2, [R4, #44]               @ 0x2c 
    /*0000016c*/ CMP         R2, #0 
    /*00000170*/ MOVWEQ      R0, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000174*/ MOVTEQ      R0, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000178*/ BEQ         AdecProcShow_x19c
    /*0000017c*/ CMP         R2, #1 
    /*00000180*/ BEQ         AdecProcShow_x2a8
    /*00000184*/ MOVW        R1, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5c] 
    /*00000188*/ CMP         R2, #2 
    /*0000018c*/ MOVT        R1, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5c] 
    /*00000190*/ MOVW        R0, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x50] 
    /*00000194*/ MOVT        R0, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x50] 
    /*00000198*/ MOVNE       R0, R1 
    AdecProcShow_x19c:
    /*0000019c*/ LDR         R2, [R4, #40]               @ 0x28 
    /*000001a0*/ LDR         FP, [R4, #12] 
    /*000001a4*/ CMP         R2, #0 
    /*000001a8*/ MOVWEQ      SL, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x64] 
    /*000001ac*/ MOVTEQ      SL, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x64] 
    /*000001b0*/ BEQ         AdecProcShow_x1fc
    /*000001b4*/ CMP         R2, #1 
    /*000001b8*/ BEQ         AdecProcShow_x29c
    /*000001bc*/ CMP         R2, #2 
    /*000001c0*/ BEQ         AdecProcShow_x2cc
    /*000001c4*/ CMP         R2, #3 
    /*000001c8*/ BEQ         AdecProcShow_x2e4
    /*000001cc*/ CMP         R2, #4 
    /*000001d0*/ BEQ         AdecProcShow_x314
    /*000001d4*/ CMP         R2, #5 
    /*000001d8*/ BEQ         AdecProcShow_x32c
    /*000001dc*/ CMP         R2, #6 
    /*000001e0*/ BEQ         AdecProcShow_x338
    /*000001e4*/ MOVW        R1, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5c] 
    /*000001e8*/ CMP         R2, #7 
    /*000001ec*/ MOVT        R1, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5c] 
    /*000001f0*/ MOVW        SL, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc4] 
    /*000001f4*/ MOVT        SL, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc4] 
    /*000001f8*/ MOVNE       SL, R1 
    AdecProcShow_x1fc:
    /*000001fc*/ LDR         R2, [R4, #48]               @ 0x30 
    /*00000200*/ CMP         R2, #1 
    /*00000204*/ MOVEQ       R1, R9 
    /*00000208*/ BEQ         AdecProcShow_x21c
    /*0000020c*/ CMP         R2, #0 
    /*00000210*/ MOVW        R1, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5c] 
    /*00000214*/ MOVT        R1, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5c] 
    /*00000218*/ MOVEQ       R1, R8 
    AdecProcShow_x21c:
    /*0000021c*/ LDR         IP, [R4, #32] 
    /*00000220*/ MOV         R2, R5 
    /*00000224*/ LDR         LR, [R4, #36]               @ 0x24 
    /*00000228*/ STR         R1, [SP, #12] 
    /*0000022c*/ MOV         R1, R6 
    /*00000230*/ STR         IP, [SP, #24] 
    /*00000234*/ LDR         IP, [R4, #28] 
    /*00000238*/ STR         R0, [SP] 
    /*0000023c*/ MOV         R0, R7 
    /*00000240*/ STR         SL, [SP, #8] 
    /*00000244*/ STR         IP, [SP, #20] 
    /*00000248*/ LDR         IP, [R4, #24] 
    /*0000024c*/ STR         FP, [SP, #4] 
    /*00000250*/ STR         LR, [SP, #28] 
    /*00000254*/ STR         IP, [SP, #16] 
    /*00000258*/ BL          0x0000000000000258          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    AdecProcShow_x25c:
    /*0000025c*/ ADD         R5, R5, #1 
    /*00000260*/ ADD         R4, R4, #52                 @ 0x34 
    /*00000264*/ CMP         R5, #32 
    /*00000268*/ BEQ         AdecProcShow_x2fc
    AdecProcShow_x26c:
    /*0000026c*/ LDR         R3, [R4, #4] 
    /*00000270*/ CMP         R3, #0 
    /*00000274*/ BEQ         AdecProcShow_x25c
    /*00000278*/ LDR         R3, [R4, #8] 
    /*0000027c*/ CMP         R3, #19 
    /*00000280*/ BNE         AdecProcShow_x10c
    /*00000284*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*00000288*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*0000028c*/ B           AdecProcShow_x168
    AdecProcShow_x290:
    /*00000290*/ MOVW        R3, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8] 
    /*00000294*/ MOVT        R3, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8] 
    /*00000298*/ B           AdecProcShow_x168
    AdecProcShow_x29c:
    /*0000029c*/ MOVW        SL, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x70] 
    /*000002a0*/ MOVT        SL, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x70] 
    /*000002a4*/ B           AdecProcShow_x1fc
    AdecProcShow_x2a8:
    /*000002a8*/ MOVW        R0, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4c] 
    /*000002ac*/ MOVT        R0, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4c] 
    /*000002b0*/ B           AdecProcShow_x19c
    AdecProcShow_x2b4:
    /*000002b4*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10] 
    /*000002b8*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10] 
    /*000002bc*/ B           AdecProcShow_x168
    AdecProcShow_x2c0:
    /*000002c0*/ MOVW        R3, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*000002c4*/ MOVT        R3, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*000002c8*/ B           AdecProcShow_x168
    AdecProcShow_x2cc:
    /*000002cc*/ MOVW        SL, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7c] 
    /*000002d0*/ MOVT        SL, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7c] 
    /*000002d4*/ B           AdecProcShow_x1fc
    AdecProcShow_x2d8:
    /*000002d8*/ MOVW        R3, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x20] 
    /*000002dc*/ MOVT        R3, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x20] 
    /*000002e0*/ B           AdecProcShow_x168
    AdecProcShow_x2e4:
    /*000002e4*/ MOVW        SL, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x88] 
    /*000002e8*/ MOVT        SL, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x88] 
    /*000002ec*/ B           AdecProcShow_x1fc
    AdecProcShow_x2f0:
    /*000002f0*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28] 
    /*000002f4*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28] 
    /*000002f8*/ B           AdecProcShow_x168
    AdecProcShow_x2fc:
    /*000002fc*/ MOV         R0, #0 
    /*00000300*/ ADD         SP, SP, #36                 @ 0x24 
    /*00000304*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AdecProcShow_x308:
    /*00000308*/ MOVW        R3, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*0000030c*/ MOVT        R3, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30] 
    /*00000310*/ B           AdecProcShow_x168
    AdecProcShow_x314:
    /*00000314*/ MOVW        SL, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x94] 
    /*00000318*/ MOVT        SL, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x94] 
    /*0000031c*/ B           AdecProcShow_x1fc
    AdecProcShow_x320:
    /*00000320*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x34] 
    /*00000324*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x34] 
    /*00000328*/ B           AdecProcShow_x168
    AdecProcShow_x32c:
    /*0000032c*/ MOVW        SL, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa4] 
    /*00000330*/ MOVT        SL, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa4] 
    /*00000334*/ B           AdecProcShow_x1fc
    AdecProcShow_x338:
    /*00000338*/ MOVW        SL, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb4] 
    /*0000033c*/ MOVT        SL, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb4] 
    /*00000340*/ B           AdecProcShow_x1fc
FUNC_END AdecProcShow


FUNC_BEGIN ADEC_QueryState
    /*00000344*/ PUSH        {R4, LR} 
    /*00000348*/ MOV         R4, R0 
    /*0000034c*/ LDR         R0, [PC, #20]               @ 0x0000000000000368 
    /*00000350*/ BL          0x0000000000000350          @ osal_atomic_read [R_ARM_CALL: +0x0] 
    /*00000354*/ CMP         R0, #0 
    /*00000358*/ MOVEQ       R3, #17 
    /*0000035c*/ MOVNE       R3, #34                     @ 0x22 
    /*00000360*/ STR         R3, [R4] 
    /*00000364*/ POP         {R4, PC} 
    /*00000368*/ ANDEQ       R0, R0, R0, LSL #13 
FUNC_END ADEC_QueryState


FUNC_BEGIN ADEC_Init
    /*0000036c*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00000370*/ MOVW        R7, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000374*/ MOVT        R7, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000378*/ SUB         SP, SP, #12 
    /*0000037c*/ LDR         R3, [R7] 
    /*00000380*/ CMP         R3, #0 
    /*00000384*/ BEQ         ADEC_Init_x3d4
    /*00000388*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000038c*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000390*/ ADD         R6, R4, #1664               @ 0x680 
    /*00000394*/ MOV         R5, #8 
    ADEC_Init_x398:
    /*00000398*/ MOV         R0, R4 
    /*0000039c*/ MOV         R2, #52                     @ 0x34 
    /*000003a0*/ MOV         R1, #0 
    /*000003a4*/ ADD         R4, R4, #52                 @ 0x34 
    /*000003a8*/ BL          0x00000000000003A8          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000003ac*/ STR         R5, [R4, #-12] 
    /*000003b0*/ MOV         R3, #3 
    /*000003b4*/ STR         R3, [R4, #-8] 
    /*000003b8*/ CMP         R4, R6 
    /*000003bc*/ BNE         ADEC_Init_x398
    /*000003c0*/ MOV         R3, #0 
    /*000003c4*/ MOV         R0, #0 
    /*000003c8*/ STR         R3, [R7] 
    /*000003cc*/ ADD         SP, SP, #12 
    /*000003d0*/ POP         {R4, R5, R6, R7, PC} 
    ADEC_Init_x3d4:
    /*000003d4*/ MOV         R2, #255                    @ 0xff 
    /*000003d8*/ MOV         R1, #24 
    /*000003dc*/ STR         R2, [SP] 
    /*000003e0*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000003e4*/ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*000003e8*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*000003ec*/ MOVT        R2, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*000003f0*/ MOV         R0, #4 
    /*000003f4*/ BL          0x00000000000003F4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000003f8*/ MOV         R1, #24 
    /*000003fc*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x260] 
    /*00000400*/ MOV         R0, #4 
    /*00000404*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x260] 
    /*00000408*/ BL          0x0000000000000408          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000040c*/ MOV         R0, #0 
    /*00000410*/ ADD         SP, SP, #12 
    /*00000414*/ POP         {R4, R5, R6, R7, PC} 
FUNC_END ADEC_Init


FUNC_BEGIN AdecIoctl
    /*00000418*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000041c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000420*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000424*/ SUB         SP, SP, #8 
    /*00000428*/ LDR         R6, [R3] 
    /*0000042c*/ CMP         R6, #0 
    /*00000430*/ BNE         AdecIoctl_x574
    /*00000434*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000438*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000043c*/ MOV         R5, R0 
    /*00000440*/ ADD         R0, R4, #1664               @ 0x680 
    /*00000444*/ MOV         R8, R2 
    /*00000448*/ MOV         R7, R1 
    /*0000044c*/ BL          0x000000000000044C          @ osal_atomic_inc_return [R_ARM_CALL: +0x0] 
    /*00000450*/ MOV         R0, #18432                  @ 0x4800 
    /*00000454*/ MOVT        R0, #16388                  @ 0x4004 
    /*00000458*/ LDR         R3, [R8] 
    /*0000045c*/ CMP         R5, R0 
    /*00000460*/ BEQ         AdecIoctl_x564
    /*00000464*/ BLS         AdecIoctl_x4d8
    /*00000468*/ MOVW        R2, #18433                  @ 0x4801 
    /*0000046c*/ MOVT        R2, #16400                  @ 0x4010 
    /*00000470*/ CMP         R5, R2 
    /*00000474*/ BEQ         AdecIoctl_x52c
    /*00000478*/ MOVW        R2, #18435                  @ 0x4803 
    /*0000047c*/ MOVT        R2, #16412                  @ 0x401c 
    /*00000480*/ CMP         R5, R2 
    /*00000484*/ BNE         AdecIoctl_x510
    /*00000488*/ CMP         R3, #31 
    /*0000048c*/ BHI         AdecIoctl_x5b4
    /*00000490*/ MOV         R1, #52                     @ 0x34 
    /*00000494*/ LDR         R2, [R7, #8] 
    /*00000498*/ MLA         R4, R1, R3, R4 
    /*0000049c*/ MOV         R5, R6 
    /*000004a0*/ STR         R2, [R4, #32] 
    /*000004a4*/ LDR         R3, [R7, #12] 
    /*000004a8*/ STR         R3, [R4, #36]               @ 0x24 
    /*000004ac*/ LDR         R3, [R7, #4] 
    /*000004b0*/ STR         R3, [R4, #28] 
    /*000004b4*/ LDR         R3, [R7] 
    /*000004b8*/ STR         R3, [R4, #24] 
    /*000004bc*/ LDR         R3, [R7, #16] 
    /*000004c0*/ STR         R3, [R4, #40]               @ 0x28 
    /*000004c4*/ LDR         R3, [R7, #20] 
    /*000004c8*/ STR         R3, [R4, #44]               @ 0x2c 
    /*000004cc*/ LDR         R3, [R7, #24] 
    /*000004d0*/ STR         R3, [R4, #48]               @ 0x30 
    /*000004d4*/ B           AdecIoctl_x4fc
    AdecIoctl_x4d8:
    /*000004d8*/ MOVW        R2, #18434                  @ 0x4802 
    /*000004dc*/ CMP         R5, R2 
    /*000004e0*/ BNE         AdecIoctl_x510
    /*000004e4*/ CMP         R3, #31 
    /*000004e8*/ BHI         AdecIoctl_x5b4
    /*000004ec*/ MOV         R2, #52                     @ 0x34 
    /*000004f0*/ MOV         R5, R6 
    /*000004f4*/ MLA         R4, R2, R3, R4 
    /*000004f8*/ STR         R6, [R4, #4] 
    AdecIoctl_x4fc:
    /*000004fc*/ LDR         R0, [PC, #188]              @ 0x00000000000005c0 
    /*00000500*/ BL          0x0000000000000500          @ osal_atomic_dec_return [R_ARM_CALL: +0x0] 
    AdecIoctl_x504:
    /*00000504*/ MOV         R0, R5 
    /*00000508*/ ADD         SP, SP, #8 
    /*0000050c*/ POP         {R4, R5, R6, R7, R8, PC} 
    AdecIoctl_x510:
    /*00000510*/ UXTB        R2, R5 
    /*00000514*/ MOV         R1, R5 
    /*00000518*/ MOVW        R0, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x294] 
    /*0000051c*/ MOVT        R0, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x294] 
    /*00000520*/ BL          0x0000000000000520          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00000524*/ MVN         R5, #0 
    /*00000528*/ B           AdecIoctl_x4fc
    AdecIoctl_x52c:
    /*0000052c*/ CMP         R3, #31 
    /*00000530*/ BHI         AdecIoctl_x5b4
    /*00000534*/ MOV         R2, #52                     @ 0x34 
    /*00000538*/ MOV         R1, R7 
    /*0000053c*/ MUL         R7, R2, R3 
    /*00000540*/ MOV         R2, #16 
    /*00000544*/ MOV         R5, R6 
    /*00000548*/ ADD         R0, R7, #8 
    /*0000054c*/ ADD         R0, R4, R0 
    /*00000550*/ ADD         R4, R4, R7 
    /*00000554*/ BL          0x0000000000000554          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000558*/ MOV         R3, #1 
    /*0000055c*/ STR         R3, [R4, #4] 
    /*00000560*/ B           AdecIoctl_x4fc
    AdecIoctl_x564:
    /*00000564*/ LDR         R3, [R7] 
    /*00000568*/ MOV         R5, R6 
    /*0000056c*/ STR         R3, [R8] 
    /*00000570*/ B           AdecIoctl_x4fc
    AdecIoctl_x574:
    /*00000574*/ MOV         R3, #184                    @ 0xb8 
    /*00000578*/ MOV         R1, #24 
    /*0000057c*/ STR         R3, [SP] 
    /*00000580*/ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*00000584*/ LDR         R3, [PC, #56]               @ 0x00000000000005c4 
    /*00000588*/ MOVT        R2, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*0000058c*/ MOV         R0, #4 
    /*00000590*/ MOVW        R5, #32784                  @ 0x8010 
    /*00000594*/ BL          0x0000000000000594          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000598*/ MOVW        R2, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x280] 
    /*0000059c*/ MOV         R1, #24 
    /*000005a0*/ MOVT        R2, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x280] 
    /*000005a4*/ MOV         R0, #4 
    /*000005a8*/ MOVT        R5, #40984                  @ 0xa018 
    /*000005ac*/ BL          0x00000000000005AC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000005b0*/ B           AdecIoctl_x504
    AdecIoctl_x5b4:
    /*000005b4*/ MOVW        R5, #32770                  @ 0x8002 
    /*000005b8*/ MOVT        R5, #40984                  @ 0xa018 
    /*000005bc*/ B           AdecIoctl_x4fc
    /*000005c0*/ ANDEQ       R0, R0, R0, LSL #13 
    /*000005c4*/ ANDEQ       R0, R0, IP 
FUNC_END AdecIoctl


FUNC_BEGIN adec_close
    /*000005c8*/ MOV         R0, #0 
    /*000005cc*/ BX          LR 
FUNC_END adec_close


FUNC_BEGIN adec_freeze
    /*000005d0*/ MOV         R0, #0 
    /*000005d4*/ BX          LR 
FUNC_END adec_freeze


FUNC_BEGIN adec_open
    /*000005d8*/ MOV         R0, #0 
    /*000005dc*/ BX          LR 
FUNC_END adec_open


FUNC_BEGIN ADEC_CreateChn
    /*000005e0*/ CMP         R0, #31 
    /*000005e4*/ BHI         ADEC_CreateChn_x620
    /*000005e8*/ PUSH        {R4, R5, R6, LR} 
    /*000005ec*/ MOV         R5, #52                     @ 0x34 
    /*000005f0*/ MUL         R5, R5, R0 
    /*000005f4*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000005f8*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000005fc*/ MOV         R2, #16 
    /*00000600*/ ADD         R0, R5, #8 
    /*00000604*/ ADD         R0, R4, R0 
    /*00000608*/ ADD         R4, R4, R5 
    /*0000060c*/ BL          0x000000000000060C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000610*/ MOV         R3, #1 
    /*00000614*/ MOV         R0, #0 
    /*00000618*/ STR         R3, [R4, #4] 
    /*0000061c*/ POP         {R4, R5, R6, PC} 
    ADEC_CreateChn_x620:
    /*00000620*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000624*/ MOVT        R0, #40984                  @ 0xa018 
    /*00000628*/ BX          LR 
FUNC_END ADEC_CreateChn


FUNC_BEGIN ADEC_DestroyChn
    /*0000062c*/ CMP         R0, #31 
    /*00000630*/ MOVWLS      R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000634*/ MOVTLS      R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000638*/ MOVLS       R1, #52                     @ 0x34 
    /*0000063c*/ MOVLS       R2, #0 
    /*00000640*/ MLALS       R3, R1, R0, R3 
    /*00000644*/ MOVLS       R0, R2 
    /*00000648*/ MOVWHI      R0, #32770                  @ 0x8002 
    /*0000064c*/ MOVTHI      R0, #40984                  @ 0xa018 
    /*00000650*/ STRLS       R2, [R3, #4] 
    /*00000654*/ BX          LR 
FUNC_END ADEC_DestroyChn


FUNC_BEGIN ADEC_SetDbgInfo
    /*00000658*/ CMP         R0, #31 
    /*0000065c*/ BHI         ADEC_SetDbgInfo_x6b0
    /*00000660*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000664*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000668*/ MOV         IP, #52                     @ 0x34 
    /*0000066c*/ LDR         R2, [R1, #8] 
    /*00000670*/ MLA         R3, IP, R0, R3 
    /*00000674*/ MOV         R0, #0 
    /*00000678*/ STR         R2, [R3, #32] 
    /*0000067c*/ LDR         R2, [R1, #12] 
    /*00000680*/ STR         R2, [R3, #36]               @ 0x24 
    /*00000684*/ LDR         R2, [R1, #4] 
    /*00000688*/ STR         R2, [R3, #28] 
    /*0000068c*/ LDR         R2, [R1] 
    /*00000690*/ STR         R2, [R3, #24] 
    /*00000694*/ LDR         R2, [R1, #16] 
    /*00000698*/ STR         R2, [R3, #40]               @ 0x28 
    /*0000069c*/ LDR         R2, [R1, #20] 
    /*000006a0*/ STR         R2, [R3, #44]               @ 0x2c 
    /*000006a4*/ LDR         R2, [R1, #24] 
    /*000006a8*/ STR         R2, [R3, #48]               @ 0x30 
    /*000006ac*/ BX          LR 
    ADEC_SetDbgInfo_x6b0:
    /*000006b0*/ MOVW        R0, #32770                  @ 0x8002 
    /*000006b4*/ MOVT        R0, #40984                  @ 0xa018 
    /*000006b8*/ BX          LR 
FUNC_END ADEC_SetDbgInfo


FUNC_BEGIN ADEC_ModInit
    /*000006bc*/ PUSH        {R4, R5, R6, LR} 
    /*000006c0*/ MOVW        R0, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2bc] 
    /*000006c4*/ SUB         SP, SP, #8 
    /*000006c8*/ MOV         R1, #0 
    /*000006cc*/ MOVT        R0, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2bc] 
    /*000006d0*/ BL          0x00000000000006D0          @ osal_create_proc_entry [R_ARM_CALL: +0x0] 
    /*000006d4*/ SUBS        R2, R0, #0 
    /*000006d8*/ BEQ         ADEC_ModInit_x794
    /*000006dc*/ MOVW        R3, #0                      @ AdecProcShow [R_ARM_MOVW_ABS_NC: .text+0x3c] 
    /*000006e0*/ MOVW        R0, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2bc] 
    /*000006e4*/ MOVT        R3, #0                      @ AdecProcShow [R_ARM_MOVT_ABS: .text+0x3c] 
    /*000006e8*/ MOVT        R0, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2bc] 
    /*000006ec*/ STR         R3, [R2, #60]               @ 0x3c 
    /*000006f0*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000006f4*/ BL          0x00000000000006F4          @ osal_createdev [R_ARM_CALL: +0x0] 
    /*000006f8*/ LDR         R5, [PC, #188]              @ 0x00000000000007bc 
    /*000006fc*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000700*/ ADD         R2, R5, #32 
    /*00000704*/ STR         R5, [R0, #56]               @ 0x38 
    /*00000708*/ STR         R2, [R0, #60]               @ 0x3c 
    /*0000070c*/ MOV         R2, #14 
    /*00000710*/ STR         R2, [R0, #52]               @ 0x34 
    /*00000714*/ STR         R0, [R4, #1668]             @ 0x684 
    /*00000718*/ BL          0x0000000000000718          @ osal_registerdevice [R_ARM_CALL: +0x0] 
    /*0000071c*/ SUBS        R6, R0, #0 
    /*00000720*/ BNE         ADEC_ModInit_x754
    /*00000724*/ ADD         R0, R5, #112                @ 0x70 
    /*00000728*/ BL          0x0000000000000728          @ CMPI_RegisterModule [R_ARM_CALL: +0x0] 
    /*0000072c*/ CMP         R0, #0 
    /*00000730*/ BNE         ADEC_ModInit_x76c
    /*00000734*/ ADD         R0, R4, #1664               @ 0x680 
    /*00000738*/ BL          0x0000000000000738          @ osal_atomic_init [R_ARM_CALL: +0x0] 
    /*0000073c*/ MOV         R1, R6 
    /*00000740*/ ADD         R0, R4, #1664               @ 0x680 
    /*00000744*/ BL          0x0000000000000744          @ osal_atomic_set [R_ARM_CALL: +0x0] 
    ADEC_ModInit_x748:
    /*00000748*/ MOV         R0, R6 
    /*0000074c*/ ADD         SP, SP, #8 
    /*00000750*/ POP         {R4, R5, R6, PC} 
    ADEC_ModInit_x754:
    /*00000754*/ MOVW        R0, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2bc] 
    /*00000758*/ MOV         R1, #0 
    /*0000075c*/ MOVT        R0, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2bc] 
    /*00000760*/ MVN         R6, #0 
    /*00000764*/ BL          0x0000000000000764          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*00000768*/ B           ADEC_ModInit_x748
    ADEC_ModInit_x76c:
    /*0000076c*/ MOV         R1, R6 
    /*00000770*/ MOVW        R0, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2bc] 
    /*00000774*/ MOVT        R0, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2bc] 
    /*00000778*/ MVN         R6, #0 
    /*0000077c*/ BL          0x000000000000077C          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*00000780*/ LDR         R0, [R4, #1668]             @ 0x684 
    /*00000784*/ BL          0x0000000000000784          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*00000788*/ LDR         R0, [R4, #1668]             @ 0x684 
    /*0000078c*/ BL          0x000000000000078C          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*00000790*/ B           ADEC_ModInit_x748
    ADEC_ModInit_x794:
    /*00000794*/ MOVW        R3, #365                    @ 0x16d 
    /*00000798*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c4] 
    /*0000079c*/ STR         R3, [SP] 
    /*000007a0*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c4] 
    /*000007a4*/ LDR         R3, [PC, #20]               @ 0x00000000000007c0 
    /*000007a8*/ MOV         R1, #22 
    /*000007ac*/ MOV         R0, #6 
    /*000007b0*/ MVN         R6, #0 
    /*000007b4*/ BL          0x00000000000007B4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000007b8*/ B           ADEC_ModInit_x748
    /*000007bc*/ ANDEQ       R0, R0, R4 
    /*000007c0*/ ANDEQ       R0, R0, R8, LSL R0 
FUNC_END ADEC_ModInit


FUNC_BEGIN ADEC_ModExit
    /*000007c4*/ PUSH        {R4, LR} 
    /*000007c8*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000007cc*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000007d0*/ ADD         R0, R4, #1664               @ 0x680 
    /*000007d4*/ BL          0x00000000000007D4          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*000007d8*/ MOV         R0, #24 
    /*000007dc*/ BL          0x00000000000007DC          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    /*000007e0*/ LDR         R0, [R4, #1668]             @ 0x684 
    /*000007e4*/ BL          0x00000000000007E4          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*000007e8*/ LDR         R0, [R4, #1668]             @ 0x684 
    /*000007ec*/ BL          0x00000000000007EC          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*000007f0*/ MOVW        R0, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2bc] 
    /*000007f4*/ MOV         R1, #0 
    /*000007f8*/ MOVT        R0, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2bc] 
    /*000007fc*/ POP         {R4, LR} 
    ADEC_ModExit_x800:
    /*00000800*/ B           ADEC_ModExit_x800           @ osal_remove_proc_entry [R_ARM_JUMP24: +0x0] 
FUNC_END ADEC_ModExit


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .init.text
@ Size: 0x10
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN adec_mod_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ BL          0x0000000000000004          @ ADEC_ModInit [R_ARM_CALL: .text+0x6bc] 
    /*00000008*/ MOV         R0, #0 
    /*0000000c*/ POP         {R4, PC} 
FUNC_END adec_mod_init




@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .exit.text
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN adec_mod_exit
    adec_mod_exit_x0:
    /*00000000*/ B           adec_mod_exit_x0            @ ADEC_ModExit [R_ARM_JUMP24: .text+0x7c4] 
FUNC_END adec_mod_exit




