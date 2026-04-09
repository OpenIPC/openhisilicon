@
@ -- FILES --
@

@ acodec.c
@ acodec_init.c
@ hi3516cv500_acodec.mod.c


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text
@ Size: 0x11e0
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN acodec_open
    /*00000000*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000004*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000008*/ LDR         R0, [R3] 
    /*0000000c*/ CMP         R0, #0 
    /*00000010*/ MOVEQ       R2, #1 
    /*00000014*/ MVNNE       R0, #0 
    /*00000018*/ STREQ       R2, [R3] 
    /*0000001c*/ BX          LR 
FUNC_END acodec_open


FUNC_BEGIN acodec_close
    /*00000020*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000024*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000028*/ MOV         R0, #0 
    /*0000002c*/ LDR         R2, [R3] 
    /*00000030*/ SUB         R2, R2, #1 
    /*00000034*/ STR         R2, [R3] 
    /*00000038*/ BX          LR 
FUNC_END acodec_close


FUNC_BEGIN acodec_getadcmodesel
    /*0000003c*/ CMP         R0, #11 
    /*00000040*/ BLS         acodec_getadcmodesel_x60
    acodec_getadcmodesel_x44:
    /*00000044*/ MOV         R1, R0 
    /*00000048*/ MOVW        R0, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*0000004c*/ PUSH        {R4, LR} 
    /*00000050*/ MOVT        R0, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*00000054*/ BL          0x0000000000000054          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00000058*/ MOV         R0, #28 
    /*0000005c*/ POP         {R4, PC} 
    acodec_getadcmodesel_x60:
    /*00000060*/ MOV         R3, #1 
    /*00000064*/ MOVW        R2, #1170                   @ 0x492 
    /*00000068*/ LSL         R1, R3, R0 
    /*0000006c*/ ANDS        R2, R1, R2 
    /*00000070*/ BNE         acodec_getadcmodesel_x88
    /*00000074*/ MOVW        R3, #2924                   @ 0xb6c 
    /*00000078*/ TST         R1, R3 
    /*0000007c*/ BEQ         acodec_getadcmodesel_x44
    /*00000080*/ MOV         R0, R2 
    /*00000084*/ BX          LR 
    acodec_getadcmodesel_x88:
    /*00000088*/ MOV         R0, R3 
    /*0000008c*/ BX          LR 
FUNC_END acodec_getadcmodesel


FUNC_BEGIN AcodecProcShow
    /*00000090*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000094*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000098*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000009c*/ SUB         SP, SP, #100                @ 0x64 
    /*000000a0*/ LDR         R3, [R3, #4] 
    /*000000a4*/ MOV         R6, R0 
    /*000000a8*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c] 
    /*000000ac*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c] 
    /*000000b0*/ MOVW        SL, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x120] 
    /*000000b4*/ MOVW        R9, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x128] 
    /*000000b8*/ LDR         R2, [R3, #20] 
    /*000000bc*/ MOVT        SL, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x120] 
    /*000000c0*/ MOVT        R9, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x128] 
    /*000000c4*/ MOVW        R8, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*000000c8*/ MOVW        R7, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*000000cc*/ MOVT        R8, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*000000d0*/ STR         R2, [SP, #68]               @ 0x44 
    /*000000d4*/ MOVT        R7, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*000000d8*/ LDR         R2, [R3, #24] 
    /*000000dc*/ MOVW        R5, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x148] 
    /*000000e0*/ LDR         FP, [R3, #32] 
    /*000000e4*/ MOVT        R5, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x148] 
    /*000000e8*/ MOVW        R4, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x154] 
    /*000000ec*/ MOVT        R4, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x154] 
    /*000000f0*/ STR         R2, [SP, #64]               @ 0x40 
    /*000000f4*/ LDR         R2, [R3, #56]               @ 0x38 
    /*000000f8*/ LDR         R3, [R3, #60]               @ 0x3c 
    /*000000fc*/ STR         R2, [SP, #72]               @ 0x48 
    /*00000100*/ UBFX        R2, FP, #16, #5 
    /*00000104*/ CMP         R2, #16 
    /*00000108*/ STR         R3, [SP, #76]               @ 0x4c 
    /*0000010c*/ MOVNE       R3, R2 
    /*00000110*/ MOVEQ       R3, #5 
    /*00000114*/ LSLNE       R3, R3, #1 
    /*00000118*/ STREQ       R3, [SP, #60]               @ 0x3c 
    /*0000011c*/ MVNEQ       R3, #0 
    /*00000120*/ STR         R3, [SP, #56]               @ 0x38 
    /*00000124*/ UBFX        R3, FP, #24, #5 
    /*00000128*/ MOVNE       R2, #0 
    /*0000012c*/ STRNE       R2, [SP, #60]               @ 0x3c 
    /*00000130*/ CMP         R3, #16 
    /*00000134*/ LSLNE       R3, R3, #1 
    /*00000138*/ MOVEQ       R3, #5 
    /*0000013c*/ MOVNE       R2, #0 
    /*00000140*/ STREQ       R3, [SP, #92]               @ 0x5c 
    /*00000144*/ MVNEQ       R3, #0 
    /*00000148*/ STRNE       R2, [SP, #92]               @ 0x5c 
    /*0000014c*/ STR         R3, [SP, #88]               @ 0x58 
    /*00000150*/ BL          0x0000000000000150          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000154*/ MOV         R0, R6 
    /*00000158*/ MOVW        R1, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x80] 
    /*0000015c*/ MOVT        R1, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x80] 
    /*00000160*/ BL          0x0000000000000160          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000164*/ MOVW        R3, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x118] 
    /*00000168*/ MOVT        R3, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x118] 
    /*0000016c*/ MOV         R0, R6 
    /*00000170*/ MOVW        LR, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x160] 
    /*00000174*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe0] 
    /*00000178*/ MOV         IP, R3 
    /*0000017c*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x140] 
    /*00000180*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x140] 
    /*00000184*/ MOVT        LR, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x160] 
    /*00000188*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe0] 
    /*0000018c*/ MOVW        R1, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xec] 
    /*00000190*/ MOVT        R1, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xec] 
    /*00000194*/ STR         R3, [SP, #80]               @ 0x50 
    /*00000198*/ MOVW        R3, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*0000019c*/ STR         IP, [SP, #36]               @ 0x24 
    /*000001a0*/ MOVT        R3, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*000001a4*/ LDR         IP, [SP, #80]               @ 0x50 
    /*000001a8*/ STR         R3, [SP, #84]               @ 0x54 
    /*000001ac*/ MOVW        R3, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd4] 
    /*000001b0*/ STR         SL, [SP, #32] 
    /*000001b4*/ MOVT        R3, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd4] 
    /*000001b8*/ STR         IP, [SP, #16] 
    /*000001bc*/ LDR         IP, [SP, #84]               @ 0x54 
    /*000001c0*/ STR         R9, [SP, #28] 
    /*000001c4*/ STR         R8, [SP, #24] 
    /*000001c8*/ STR         R7, [SP, #20] 
    /*000001cc*/ STR         R5, [SP, #12] 
    /*000001d0*/ STR         R4, [SP, #8] 
    /*000001d4*/ STR         LR, [SP, #4] 
    /*000001d8*/ STR         IP, [SP] 
    /*000001dc*/ BL          0x00000000000001DC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000001e0*/ MOVW        LR, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x178] 
    /*000001e4*/ MOVT        LR, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x178] 
    /*000001e8*/ MOV         R0, R6 
    /*000001ec*/ UBFX        R6, FP, #9, #1 
    /*000001f0*/ UBFX        FP, FP, #10, #1 
    /*000001f4*/ LDR         R3, [SP, #76]               @ 0x4c 
    /*000001f8*/ LDR         R1, [SP, #64]               @ 0x40 
    /*000001fc*/ UBFX        IP, R3, #24, #7 
    /*00000200*/ UBFX        R5, R3, #16, #7 
    /*00000204*/ UBFX        SL, R1, #22, #1 
    /*00000208*/ UBFX        R9, R1, #23, #1 
    /*0000020c*/ RSB         R1, IP, #30 
    /*00000210*/ LDR         IP, [SP, #92]               @ 0x5c 
    /*00000214*/ LDR         R3, [SP, #72]               @ 0x48 
    /*00000218*/ RSB         R5, R5, #30 
    /*0000021c*/ LDR         R2, [SP, #56]               @ 0x38 
    /*00000220*/ STR         IP, [SP, #12] 
    /*00000224*/ UBFX        R4, R3, #16, #7 
    /*00000228*/ LDR         IP, [SP, #88]               @ 0x58 
    /*0000022c*/ UBFX        R3, R3, #24, #7 
    /*00000230*/ RSB         R3, R3, #6 
    /*00000234*/ STR         R3, [SP, #16] 
    /*00000238*/ LDR         R3, [SP, #68]               @ 0x44 
    /*0000023c*/ RSB         R4, R4, #6 
    /*00000240*/ STR         IP, [SP, #4] 
    /*00000244*/ LDR         IP, [SP, #60]               @ 0x3c 
    /*00000248*/ UBFX        R8, R3, #14, #1 
    /*0000024c*/ UBFX        R7, R3, #13, #1 
    /*00000250*/ STR         R1, [SP, #24] 
    /*00000254*/ MOV         R3, LR 
    /*00000258*/ STR         R6, [SP, #36]               @ 0x24 
    /*0000025c*/ STR         IP, [SP] 
    /*00000260*/ MOVW        R1, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17c] 
    /*00000264*/ STR         FP, [SP, #32] 
    /*00000268*/ MOVT        R1, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17c] 
    /*0000026c*/ STR         R5, [SP, #28] 
    /*00000270*/ STR         R4, [SP, #20] 
    /*00000274*/ STR         LR, [SP, #8] 
    /*00000278*/ STR         SL, [SP, #52]               @ 0x34 
    /*0000027c*/ STR         R9, [SP, #48]               @ 0x30 
    /*00000280*/ STR         R8, [SP, #44]               @ 0x2c 
    /*00000284*/ STR         R7, [SP, #40]               @ 0x28 
    /*00000288*/ BL          0x0000000000000288          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000028c*/ MOV         R0, #0 
    /*00000290*/ ADD         SP, SP, #100                @ 0x64 
    /*00000294*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
FUNC_END AcodecProcShow


FUNC_BEGIN acodec_reboot
    /*00000298*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000029c*/ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000002a0*/ MOVT        R6, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000002a4*/ MOVW        R3, #9252                   @ 0x2424 
    /*000002a8*/ MOV         R0, #50                     @ 0x32 
    /*000002ac*/ MOVT        R3, #32639                  @ 0x7f7f 
    /*000002b0*/ LDR         R2, [R6, #4] 
    /*000002b4*/ STR         R3, [R2, #56]               @ 0x38 
    /*000002b8*/ BL          0x00000000000002B8          @ osal_msleep [R_ARM_CALL: +0x0] 
    /*000002bc*/ LDR         R3, [R6, #4] 
    /*000002c0*/ MOV         R0, #1000                   @ 0x3e8 
    /*000002c4*/ LDR         R2, [R3, #20] 
    /*000002c8*/ LDR         R7, [R3, #28] 
    /*000002cc*/ LDR         R1, [R3, #32] 
    /*000002d0*/ ORR         R5, R2, #24576              @ 0x6000 
    /*000002d4*/ ORR         R4, R1, #120                @ 0x78 
    /*000002d8*/ STR         R4, [R3, #32] 
    /*000002dc*/ ORR         R4, R1, #125                @ 0x7d 
    /*000002e0*/ STR         R4, [R3, #32] 
    /*000002e4*/ BFC         R4, #2, #1 
    /*000002e8*/ STR         R5, [R3, #20] 
    /*000002ec*/ BFC         R4, #0, #1 
    /*000002f0*/ ORR         R5, R2, #30720              @ 0x7800 
    /*000002f4*/ STR         R5, [R3, #20] 
    /*000002f8*/ ORR         R5, R5, #1 
    /*000002fc*/ STR         R4, [R3, #32] 
    /*00000300*/ BFC         R4, #29, #3 
    /*00000304*/ BL          0x0000000000000304          @ osal_msleep [R_ARM_CALL: +0x0] 
    /*00000308*/ LDR         R3, [R6, #4] 
    /*0000030c*/ MOV         R0, #50                     @ 0x32 
    /*00000310*/ BFC         R4, #21, #3 
    /*00000314*/ STR         R5, [R3, #20] 
    /*00000318*/ ORR         R5, R5, #32 
    /*0000031c*/ BL          0x000000000000031C          @ osal_msleep [R_ARM_CALL: +0x0] 
    /*00000320*/ LDR         R3, [R6, #4] 
    /*00000324*/ MOV         R0, #0 
    /*00000328*/ STR         R7, [R3, #28] 
    /*0000032c*/ STR         R5, [R3, #20] 
    /*00000330*/ STR         R4, [R3, #32] 
    /*00000334*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END acodec_reboot


FUNC_BEGIN ACODEC_HAL_ReadReg
    /*00000338*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000033c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000340*/ LDR         R3, [R3, #4] 
    /*00000344*/ LDR         R0, [R3, R0] 
    /*00000348*/ BX          LR 
FUNC_END ACODEC_HAL_ReadReg


FUNC_BEGIN ACODEC_HAL_WriteReg
    /*0000034c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000350*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000354*/ LDR         R3, [R3, #4] 
    /*00000358*/ STR         R1, [R3, R0] 
    /*0000035c*/ BX          LR 
FUNC_END ACODEC_HAL_WriteReg


FUNC_BEGIN acodec_soft_reset
    /*00000360*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000364*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000368*/ LDR         R3, [R3, #4] 
    /*0000036c*/ CMP         R3, #0 
    /*00000370*/ BEQ         acodec_soft_reset_x3e8
    /*00000374*/ MOV         R1, #67108866               @ 0x4000002 
    /*00000378*/ MOV         R0, #0 
    /*0000037c*/ STR         R1, [R3, #20] 
    /*00000380*/ MOV         R2, #4 
    /*00000384*/ MOV         R1, #24 
    /*00000388*/ MOVT        R2, #64800                  @ 0xfd20 
    /*0000038c*/ MOVT        R1, #24 
    /*00000390*/ STR         R2, [R3, #24] 
    /*00000394*/ STR         R1, [R3, #28] 
    /*00000398*/ MOV         R2, #40                     @ 0x28 
    /*0000039c*/ MOVW        R1, #23644                  @ 0x5c5c 
    /*000003a0*/ MOVT        R2, #33667                  @ 0x8383 
    /*000003a4*/ STR         R2, [R3, #32] 
    /*000003a8*/ MOV         R2, #23040                  @ 0x5a00 
    /*000003ac*/ STR         R1, [R3, #36]               @ 0x24 
    /*000003b0*/ MOVT        R2, #65283                  @ 0xff03 
    /*000003b4*/ MOV         R1, #134217729              @ 0x8000001 
    /*000003b8*/ STR         R2, [R3, #48]               @ 0x30 
    /*000003bc*/ STR         R1, [R3, #52]               @ 0x34 
    /*000003c0*/ MOVW        R2, #9252                   @ 0x2424 
    /*000003c4*/ MOVW        R1, #49153                  @ 0xc001 
    /*000003c8*/ MOVT        R2, #1542                   @ 0x606 
    /*000003cc*/ MOVT        R1, #7710                   @ 0x1e1e 
    /*000003d0*/ STR         R2, [R3, #56]               @ 0x38 
    /*000003d4*/ STR         R1, [R3, #60]               @ 0x3c 
    /*000003d8*/ MOVW        R2, #9252                   @ 0x2424 
    /*000003dc*/ MOVT        R2, #9252                   @ 0x2424 
    /*000003e0*/ STR         R2, [R3, #64]               @ 0x40 
    /*000003e4*/ BX          LR 
    acodec_soft_reset_x3e8:
    /*000003e8*/ PUSH        {R4, LR} 
    /*000003ec*/ MOV         R3, #187                    @ 0xbb 
    /*000003f0*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000003f4*/ MOVW        R1, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*000003f8*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*000003fc*/ MOVT        R1, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*00000400*/ MOVW        R0, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bc] 
    /*00000404*/ MOVT        R0, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bc] 
    /*00000408*/ BL          0x0000000000000408          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000040c*/ MVN         R0, #0 
    /*00000410*/ POP         {R4, PC} 
FUNC_END acodec_soft_reset


FUNC_BEGIN acodec_ioctl
    /*00000414*/ PUSH        {R4, R5, R6, LR} 
    /*00000418*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000041c*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000420*/ SUB         SP, SP, #8 
    /*00000424*/ LDR         R3, [R4, #4] 
    /*00000428*/ CMP         R3, #0 
    /*0000042c*/ BEQ         acodec_ioctl_xdb0
    /*00000430*/ MOVW        R2, #16662                  @ 0x4116 
    /*00000434*/ MOVT        R2, #49156                  @ 0xc004 
    /*00000438*/ CMP         R0, R2 
    /*0000043c*/ MOV         IP, R1 
    /*00000440*/ BEQ         acodec_ioctl_xc58
    /*00000444*/ BHI         acodec_ioctl_x4c4
    /*00000448*/ MOVW        R2, #16648                  @ 0x4108 
    /*0000044c*/ MOVT        R2, #49156                  @ 0xc004 
    /*00000450*/ CMP         R0, R2 
    /*00000454*/ BEQ         acodec_ioctl_xc6c
    /*00000458*/ BLS         acodec_ioctl_x560
    /*0000045c*/ MOVW        R2, #16657                  @ 0x4111 
    /*00000460*/ MOVT        R2, #49156                  @ 0xc004 
    /*00000464*/ CMP         R0, R2 
    /*00000468*/ BEQ         acodec_ioctl_xc8c
    /*0000046c*/ BHI         acodec_ioctl_x698
    /*00000470*/ MOVW        R2, #16650                  @ 0x410a 
    /*00000474*/ MOVT        R2, #49156                  @ 0xc004 
    /*00000478*/ CMP         R0, R2 
    /*0000047c*/ BEQ         acodec_ioctl_xbb8
    /*00000480*/ BCC         acodec_ioctl_xb98
    /*00000484*/ MOVW        R2, #16651                  @ 0x410b 
    /*00000488*/ MOVT        R2, #49156                  @ 0xc004 
    /*0000048c*/ CMP         R0, R2 
    /*00000490*/ BEQ         acodec_ioctl_x860
    /*00000494*/ MOVW        R2, #16656                  @ 0x4110 
    /*00000498*/ MOVT        R2, #49156                  @ 0xc004 
    /*0000049c*/ CMP         R0, R2 
    /*000004a0*/ BNE         acodec_ioctl_x960
    /*000004a4*/ LDR         R1, [R1] 
    /*000004a8*/ CMP         R1, #1 
    /*000004ac*/ BHI         acodec_ioctl_x960
    /*000004b0*/ LDR         R2, [R3, #32] 
    /*000004b4*/ MOV         R0, #0 
    /*000004b8*/ BFI         R2, R1, #10, #1 
    /*000004bc*/ STR         R2, [R3, #32] 
    /*000004c0*/ B           acodec_ioctl_x75c
    acodec_ioctl_x4c4:
    /*000004c4*/ MOVW        R2, #16675                  @ 0x4123 
    /*000004c8*/ MOVT        R2, #49156                  @ 0xc004 
    /*000004cc*/ CMP         R0, R2 
    /*000004d0*/ BEQ         acodec_ioctl_xcac
    /*000004d4*/ BLS         acodec_ioctl_x5c8
    /*000004d8*/ MOVW        R2, #16655                  @ 0x410f 
    /*000004dc*/ MOVT        R2, #49160                  @ 0xc008 
    /*000004e0*/ CMP         R0, R2 
    /*000004e4*/ BEQ         acodec_ioctl_xcd4
    /*000004e8*/ BHI         acodec_ioctl_x638
    /*000004ec*/ MOVW        R2, #16652                  @ 0x410c 
    /*000004f0*/ MOVT        R2, #49160                  @ 0xc008 
    /*000004f4*/ CMP         R0, R2 
    /*000004f8*/ BEQ         acodec_ioctl_xa68
    /*000004fc*/ BLS         acodec_ioctl_x968
    /*00000500*/ MOVW        R3, #16653                  @ 0x410d 
    /*00000504*/ MOVT        R3, #49160                  @ 0xc008 
    /*00000508*/ CMP         R0, R3 
    /*0000050c*/ BEQ         acodec_ioctl_x90c
    /*00000510*/ MOVW        R3, #16654                  @ 0x410e 
    /*00000514*/ MOVT        R3, #49160                  @ 0xc008 
    /*00000518*/ CMP         R0, R3 
    /*0000051c*/ BNE         acodec_ioctl_x960
    /*00000520*/ MOV         R2, #8 
    /*00000524*/ MOV         R0, SP 
    /*00000528*/ BL          0x0000000000000528          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000052c*/ LDR         R3, [SP] 
    /*00000530*/ CMP         R3, #127                    @ 0x7f 
    /*00000534*/ BHI         acodec_ioctl_xd38
    /*00000538*/ LDR         R2, [SP, #4] 
    /*0000053c*/ CMP         R2, #1 
    /*00000540*/ BHI         acodec_ioctl_xd38
    /*00000544*/ LDR         IP, [R4, #4] 
    /*00000548*/ MOV         R0, #0 
    /*0000054c*/ LDR         R1, [IP, #60]               @ 0x3c 
    /*00000550*/ BFI         R1, R2, #31, #1 
    /*00000554*/ BFI         R1, R3, #24, #7 
    /*00000558*/ STR         R1, [IP, #60]               @ 0x3c 
    /*0000055c*/ B           acodec_ioctl_x75c
    acodec_ioctl_x560:
    /*00000560*/ MOVW        R2, #16643                  @ 0x4103 
    /*00000564*/ MOVT        R2, #49156                  @ 0xc004 
    /*00000568*/ CMP         R0, R2 
    /*0000056c*/ BEQ         acodec_ioctl_x7d4
    /*00000570*/ BLS         acodec_ioctl_x724
    /*00000574*/ MOVW        R2, #16645                  @ 0x4105 
    /*00000578*/ MOVT        R2, #49156                  @ 0xc004 
    /*0000057c*/ CMP         R0, R2 
    /*00000580*/ BEQ         acodec_ioctl_xac0
    /*00000584*/ BCC         acodec_ioctl_xaa8
    /*00000588*/ MOVW        R2, #16646                  @ 0x4106 
    /*0000058c*/ MOVT        R2, #49156                  @ 0xc004 
    /*00000590*/ CMP         R0, R2 
    /*00000594*/ BEQ         acodec_ioctl_x880
    /*00000598*/ MOVW        R2, #16647                  @ 0x4107 
    /*0000059c*/ MOVT        R2, #49156                  @ 0xc004 
    /*000005a0*/ CMP         R0, R2 
    /*000005a4*/ BNE         acodec_ioctl_x960
    /*000005a8*/ LDR         R1, [R1] 
    /*000005ac*/ CMP         R1, #1 
    /*000005b0*/ BHI         acodec_ioctl_x960
    /*000005b4*/ LDR         R2, [R3, #20] 
    /*000005b8*/ MOV         R0, #0 
    /*000005bc*/ BFI         R2, R1, #28, #1 
    /*000005c0*/ STR         R2, [R3, #20] 
    /*000005c4*/ B           acodec_ioctl_x75c
    acodec_ioctl_x5c8:
    /*000005c8*/ MOVW        R2, #16670                  @ 0x411e 
    /*000005cc*/ MOVT        R2, #49156                  @ 0xc004 
    /*000005d0*/ CMP         R0, R2 
    /*000005d4*/ BEQ         acodec_ioctl_x8dc
    /*000005d8*/ BLS         acodec_ioctl_x6ec
    /*000005dc*/ MOVW        R2, #16672                  @ 0x4120 
    /*000005e0*/ MOVT        R2, #49156                  @ 0xc004 
    /*000005e4*/ CMP         R0, R2 
    /*000005e8*/ BEQ         acodec_ioctl_xc38
    /*000005ec*/ BCC         acodec_ioctl_xc08
    /*000005f0*/ MOVW        R2, #16673                  @ 0x4121 
    /*000005f4*/ MOVT        R2, #49156                  @ 0xc004 
    /*000005f8*/ CMP         R0, R2 
    /*000005fc*/ BEQ         acodec_ioctl_x764
    /*00000600*/ MOVW        R2, #16674                  @ 0x4122 
    /*00000604*/ MOVT        R2, #49156                  @ 0xc004 
    /*00000608*/ CMP         R0, R2 
    /*0000060c*/ BNE         acodec_ioctl_x960
    /*00000610*/ LDR         R2, [R1] 
    /*00000614*/ CMP         R2, #3 
    /*00000618*/ BHI         acodec_ioctl_xd38
    /*0000061c*/ LDR         R1, [R3, #52]               @ 0x34 
    /*00000620*/ AND         R2, R2, #3 
    /*00000624*/ MOV         R0, #0 
    /*00000628*/ BFI         R1, R2, #21, #2 
    /*0000062c*/ BFI         R1, R2, #19, #2 
    /*00000630*/ STR         R1, [R3, #52]               @ 0x34 
    /*00000634*/ B           acodec_ioctl_x75c
    acodec_ioctl_x638:
    /*00000638*/ MOVW        R2, #16665                  @ 0x4119 
    /*0000063c*/ MOVT        R2, #49160                  @ 0xc008 
    /*00000640*/ CMP         R0, R2 
    /*00000644*/ BEQ         acodec_ioctl_xa60
    /*00000648*/ BLS         acodec_ioctl_x94c
    /*0000064c*/ MOVW        R2, #16666                  @ 0x411a 
    /*00000650*/ MOVT        R2, #49160                  @ 0xc008 
    /*00000654*/ CMP         R0, R2 
    /*00000658*/ BEQ         acodec_ioctl_x8b0
    /*0000065c*/ MOVW        R2, #16667                  @ 0x411b 
    /*00000660*/ MOVT        R2, #49160                  @ 0xc008 
    /*00000664*/ CMP         R0, R2 
    /*00000668*/ LDREQ       R3, [R3, #60]               @ 0x3c 
    /*0000066c*/ BNE         acodec_ioctl_x960
    acodec_ioctl_x670:
    /*00000670*/ ADD         R1, SP, #8 
    /*00000674*/ MOV         R0, IP 
    /*00000678*/ MOV         R2, #8 
    /*0000067c*/ UBFX        IP, R3, #16, #7 
    /*00000680*/ STR         IP, [R1, #-8]! 
    /*00000684*/ UBFX        R3, R3, #23, #1 
    /*00000688*/ STR         R3, [SP, #4] 
    /*0000068c*/ BL          0x000000000000068C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000690*/ MOV         R0, #0 
    /*00000694*/ B           acodec_ioctl_x75c
    acodec_ioctl_x698:
    /*00000698*/ MOVW        R2, #16659                  @ 0x4113 
    /*0000069c*/ MOVT        R2, #49156                  @ 0xc004 
    /*000006a0*/ CMP         R0, R2 
    /*000006a4*/ BEQ         acodec_ioctl_xa40
    /*000006a8*/ BCC         acodec_ioctl_xa20
    /*000006ac*/ MOVW        R2, #16660                  @ 0x4114 
    /*000006b0*/ MOVT        R2, #49156                  @ 0xc004 
    /*000006b4*/ CMP         R0, R2 
    /*000006b8*/ BEQ         acodec_ioctl_x7b4
    /*000006bc*/ MOVW        R2, #16661                  @ 0x4115 
    /*000006c0*/ MOVT        R2, #49156                  @ 0xc004 
    /*000006c4*/ CMP         R0, R2 
    /*000006c8*/ BNE         acodec_ioctl_x960
    /*000006cc*/ LDR         R2, [R1] 
    /*000006d0*/ CMP         R2, #1 
    /*000006d4*/ BHI         acodec_ioctl_x960
    /*000006d8*/ LDR         LR, [R3, #24] 
    /*000006dc*/ MOV         R0, #0 
    /*000006e0*/ BFI         LR, R2, #22, #1 
    /*000006e4*/ STR         LR, [R3, #24] 
    /*000006e8*/ B           acodec_ioctl_x75c
    acodec_ioctl_x6ec:
    /*000006ec*/ MOVW        R2, #16668                  @ 0x411c 
    /*000006f0*/ MOVT        R2, #49156                  @ 0xc004 
    /*000006f4*/ CMP         R0, R2 
    /*000006f8*/ BEQ         acodec_ioctl_xbd8
    /*000006fc*/ BHI         acodec_ioctl_x830
    /*00000700*/ MOVW        R2, #16663                  @ 0x4117 
    /*00000704*/ MOVT        R2, #49156                  @ 0xc004 
    /*00000708*/ CMP         R0, R2 
    /*0000070c*/ BNE         acodec_ioctl_x960
    /*00000710*/ LDR         R3, [R3, #32] 
    /*00000714*/ MOV         R0, #0 
    /*00000718*/ UBFX        R3, R3, #24, #5 
    /*0000071c*/ STR         R3, [R1] 
    /*00000720*/ B           acodec_ioctl_x75c
    acodec_ioctl_x724:
    /*00000724*/ MOVW        R2, #16641                  @ 0x4101 
    /*00000728*/ MOVT        R2, #49156                  @ 0xc004 
    /*0000072c*/ CMP         R0, R2 
    /*00000730*/ BEQ         acodec_ioctl_x998
    /*00000734*/ BHI         acodec_ioctl_x784
    /*00000738*/ CMP         R0, #16640                  @ 0x4100 
    /*0000073c*/ BNE         acodec_ioctl_x960
    /*00000740*/ BL          0x0000000000000740          @ acodec_soft_reset [R_ARM_CALL: .text+0x360] 
    /*00000744*/ LDR         R3, [R4, #4] 
    /*00000748*/ MOV         R0, #0 
    /*0000074c*/ LDR         R1, [R3, #60]               @ 0x3c 
    /*00000750*/ BFC         R1, #23, #1 
    /*00000754*/ BFC         R1, #31, #1 
    /*00000758*/ STR         R1, [R3, #60]               @ 0x3c 
    acodec_ioctl_x75c:
    /*0000075c*/ ADD         SP, SP, #8 
    /*00000760*/ POP         {R4, R5, R6, PC} 
    acodec_ioctl_x764:
    /*00000764*/ LDR         R1, [R1] 
    /*00000768*/ CMP         R1, #1 
    /*0000076c*/ BHI         acodec_ioctl_x960
    /*00000770*/ LDR         R2, [R3, #32] 
    /*00000774*/ MOV         R0, #0 
    /*00000778*/ BFI         R2, R1, #12, #1 
    /*0000077c*/ STR         R2, [R3, #32] 
    /*00000780*/ B           acodec_ioctl_x75c
    acodec_ioctl_x784:
    /*00000784*/ LDR         R1, [R1] 
    /*00000788*/ ADD         R2, R1, #121                @ 0x79 
    /*0000078c*/ CMP         R2, #127                    @ 0x7f 
    /*00000790*/ BHI         acodec_ioctl_xd38
    /*00000794*/ LDR         R2, [R3, #56]               @ 0x38 
    /*00000798*/ RSB         R1, R1, #6 
    /*0000079c*/ AND         R1, R1, #127                @ 0x7f 
    /*000007a0*/ MOV         R0, #0 
    /*000007a4*/ BFI         R2, R1, #24, #7 
    /*000007a8*/ BFI         R2, R1, #16, #7 
    /*000007ac*/ STR         R2, [R3, #56]               @ 0x38 
    /*000007b0*/ B           acodec_ioctl_x75c
    acodec_ioctl_x7b4:
    /*000007b4*/ LDR         R2, [R1] 
    /*000007b8*/ CMP         R2, #1 
    /*000007bc*/ BHI         acodec_ioctl_x960
    /*000007c0*/ LDR         LR, [R3, #24] 
    /*000007c4*/ MOV         R0, #0 
    /*000007c8*/ BFI         LR, R2, #23, #1 
    /*000007cc*/ STR         LR, [R3, #24] 
    /*000007d0*/ B           acodec_ioctl_x75c
    acodec_ioctl_x7d4:
    /*000007d4*/ MOVW        R2, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000007d8*/ MOVT        R2, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000007dc*/ LDR         R2, [R2] 
    /*000007e0*/ ADD         R1, R2, #78                 @ 0x4e 
    /*000007e4*/ CMP         R1, #158                    @ 0x9e 
    /*000007e8*/ BLS         acodec_ioctl_x824
    /*000007ec*/ LDR         R1, [R3, #24] 
    /*000007f0*/ LDR         R2, [R3, #32] 
    /*000007f4*/ TST         R1, #8388608                @ 0x800000 
    /*000007f8*/ LDR         R1, [R3, #60]               @ 0x3c 
    /*000007fc*/ UBFX        R0, R2, #16, #5 
    /*00000800*/ MOVEQ       R3, #0 
    /*00000804*/ MOVNE       R3, #20 
    /*00000808*/ CMP         R0, #16 
    /*0000080c*/ UBFXNE      R2, R2, #16, #5 
    /*00000810*/ SUBEQ       R3, R3, #1 
    /*00000814*/ ADDNE       R3, R3, R2, LSL #1 
    /*00000818*/ UBFX        R2, R1, #24, #7 
    /*0000081c*/ RSB         R2, R2, #30 
    /*00000820*/ ADD         R2, R2, R3 
    acodec_ioctl_x824:
    /*00000824*/ STR         R2, [IP] 
    /*00000828*/ MOV         R0, #0 
    /*0000082c*/ B           acodec_ioctl_x75c
    acodec_ioctl_x830:
    /*00000830*/ LDR         R1, [R1] 
    /*00000834*/ CMP         R1, #1 
    /*00000838*/ BHI         acodec_ioctl_x960
    /*0000083c*/ LDR         R2, [R3, #20] 
    /*00000840*/ EOR         IP, R1, #1 
    /*00000844*/ MOV         R0, #0 
    /*00000848*/ BFI         R2, R1, #12, #1 
    /*0000084c*/ STR         R2, [R3, #20] 
    /*00000850*/ LDR         R5, [R3, #48]               @ 0x30 
    /*00000854*/ BFI         R5, IP, #26, #1 
    /*00000858*/ STR         R5, [R3, #48]               @ 0x30 
    /*0000085c*/ B           acodec_ioctl_x75c
    acodec_ioctl_x860:
    /*00000860*/ LDR         R1, [R1] 
    /*00000864*/ CMP         R1, #16 
    /*00000868*/ BHI         acodec_ioctl_xde8
    /*0000086c*/ LDR         R2, [R3, #32] 
    /*00000870*/ MOV         R0, #0 
    /*00000874*/ BFI         R2, R1, #24, #5 
    /*00000878*/ STR         R2, [R3, #32] 
    /*0000087c*/ B           acodec_ioctl_x75c
    acodec_ioctl_x880:
    /*00000880*/ LDR         R1, [R1] 
    /*00000884*/ CMP         R1, #2 
    /*00000888*/ BHI         acodec_ioctl_xdfc
    /*0000088c*/ CMP         R1, #0 
    /*00000890*/ LDR         R2, [R3, #32] 
    /*00000894*/ BNE         acodec_ioctl_xd4c
    /*00000898*/ MOV         R1, #1 
    /*0000089c*/ BFI         R2, R1, #29, #3 
    /*000008a0*/ BFI         R2, R1, #21, #3 
    acodec_ioctl_x8a4:
    /*000008a4*/ STR         R2, [R3, #32] 
    /*000008a8*/ MOV         R0, #0 
    /*000008ac*/ B           acodec_ioctl_x75c
    acodec_ioctl_x8b0:
    /*000008b0*/ LDR         R3, [R3, #60]               @ 0x3c 
    acodec_ioctl_x8b4:
    /*000008b4*/ ADD         R1, SP, #8 
    /*000008b8*/ MOV         R0, IP 
    /*000008bc*/ MOV         R2, #8 
    /*000008c0*/ UBFX        IP, R3, #24, #7 
    /*000008c4*/ STR         IP, [R1, #-8]! 
    /*000008c8*/ LSR         R3, R3, #31 
    /*000008cc*/ STR         R3, [SP, #4] 
    /*000008d0*/ BL          0x00000000000008D0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000008d4*/ MOV         R0, #0 
    /*000008d8*/ B           acodec_ioctl_x75c
    acodec_ioctl_x8dc:
    /*000008dc*/ LDR         R1, [R1] 
    /*000008e0*/ CMP         R1, #1 
    /*000008e4*/ BHI         acodec_ioctl_x960
    /*000008e8*/ LDR         R2, [R3, #32] 
    /*000008ec*/ EOR         IP, R1, #1 
    /*000008f0*/ MOV         R0, #0 
    /*000008f4*/ BFI         R2, R1, #15, #1 
    /*000008f8*/ STR         R2, [R3, #32] 
    /*000008fc*/ LDR         R5, [R3, #48]               @ 0x30 
    /*00000900*/ BFI         R5, IP, #25, #1 
    /*00000904*/ STR         R5, [R3, #48]               @ 0x30 
    /*00000908*/ B           acodec_ioctl_x75c
    acodec_ioctl_x90c:
    /*0000090c*/ MOV         R2, #8 
    /*00000910*/ MOV         R0, SP 
    /*00000914*/ BL          0x0000000000000914          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000918*/ LDR         R3, [SP] 
    /*0000091c*/ CMP         R3, #127                    @ 0x7f 
    /*00000920*/ BHI         acodec_ioctl_xd38
    /*00000924*/ LDR         R1, [SP, #4] 
    /*00000928*/ CMP         R1, #1 
    /*0000092c*/ BHI         acodec_ioctl_xd38
    /*00000930*/ LDR         IP, [R4, #4] 
    /*00000934*/ MOV         R0, #0 
    /*00000938*/ LDR         R2, [IP, #56]               @ 0x38 
    /*0000093c*/ BFI         R2, R1, #23, #1 
    /*00000940*/ BFI         R2, R3, #16, #7 
    /*00000944*/ STR         R2, [IP, #56]               @ 0x38 
    /*00000948*/ B           acodec_ioctl_x75c
    acodec_ioctl_x94c:
    /*0000094c*/ MOVW        R2, #16664                  @ 0x4118 
    /*00000950*/ MOVT        R2, #49160                  @ 0xc008 
    /*00000954*/ CMP         R0, R2 
    /*00000958*/ LDREQ       R3, [R3, #56]               @ 0x38 
    /*0000095c*/ BEQ         acodec_ioctl_x8b4
    acodec_ioctl_x960:
    /*00000960*/ MVN         R0, #0 
    /*00000964*/ B           acodec_ioctl_x75c
    acodec_ioctl_x968:
    /*00000968*/ MOVW        R2, #16676                  @ 0x4124 
    /*0000096c*/ MOVT        R2, #49156                  @ 0xc004 
    /*00000970*/ CMP         R0, R2 
    /*00000974*/ BNE         acodec_ioctl_x960
    /*00000978*/ LDR         R2, [R1] 
    /*0000097c*/ CMP         R2, #3 
    /*00000980*/ BHI         acodec_ioctl_x960
    /*00000984*/ LDR         R5, [R3, #48]               @ 0x30 
    /*00000988*/ MOV         R0, #0 
    /*0000098c*/ BFI         R5, R2, #22, #2 
    /*00000990*/ STR         R5, [R3, #48]               @ 0x30 
    /*00000994*/ B           acodec_ioctl_x75c
    acodec_ioctl_x998:
    /*00000998*/ LDR         R4, [R1] 
    /*0000099c*/ LDR         LR, [R3, #24] 
    /*000009a0*/ ADD         R0, R4, #78                 @ 0x4e 
    /*000009a4*/ LDR         R2, [R3, #32] 
    /*000009a8*/ CMP         R0, #158                    @ 0x9e 
    /*000009ac*/ LDR         R1, [R3, #60]               @ 0x3c 
    /*000009b0*/ BHI         acodec_ioctl_xd38
    /*000009b4*/ CMP         R0, #96                     @ 0x60 
    /*000009b8*/ RSBLS       R0, R4, #48                 @ 0x30 
    /*000009bc*/ MOVLS       IP, #16 
    /*000009c0*/ BLS         acodec_ioctl_xd1c
    /*000009c4*/ SUB         R0, R4, #19 
    /*000009c8*/ CMP         R0, #31 
    /*000009cc*/ BHI         acodec_ioctl_xd14
    /*000009d0*/ CMP         R4, #19 
    /*000009d4*/ MOV         IP, #30 
    /*000009d8*/ SUBNE       R0, R4, #20 
    /*000009dc*/ BFI         R1, IP, #24, #7 
    /*000009e0*/ MOVEQ       R0, #16 
    /*000009e4*/ ORR         LR, LR, #12582912           @ 0xc00000 
    /*000009e8*/ ADDNE       R0, R0, R0, LSR #31 
    /*000009ec*/ BFI         R1, IP, #16, #7 
    /*000009f0*/ ASRNE       R0, R0, #1 
    /*000009f4*/ AND         R0, R0, #31 
    /*000009f8*/ BFI         R2, R0, #24, #5 
    /*000009fc*/ BFI         R2, R0, #16, #5 
    acodec_ioctl_xa00:
    /*00000a00*/ MOVW        IP, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000a04*/ MOVT        IP, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000a08*/ STR         LR, [R3, #24] 
    /*00000a0c*/ MOV         R0, #0 
    /*00000a10*/ STR         R2, [R3, #32] 
    /*00000a14*/ STR         R4, [IP] 
    /*00000a18*/ STR         R1, [R3, #60]               @ 0x3c 
    /*00000a1c*/ B           acodec_ioctl_x75c
    acodec_ioctl_xa20:
    /*00000a20*/ LDR         R1, [R1] 
    /*00000a24*/ CMP         R1, #1 
    /*00000a28*/ BHI         acodec_ioctl_x960
    /*00000a2c*/ LDR         R2, [R3, #20] 
    /*00000a30*/ MOV         R0, #0 
    /*00000a34*/ BFI         R2, R1, #13, #1 
    /*00000a38*/ STR         R2, [R3, #20] 
    /*00000a3c*/ B           acodec_ioctl_x75c
    acodec_ioctl_xa40:
    /*00000a40*/ LDR         R1, [R1] 
    /*00000a44*/ CMP         R1, #1 
    /*00000a48*/ BHI         acodec_ioctl_x960
    /*00000a4c*/ LDR         R2, [R3, #20] 
    /*00000a50*/ MOV         R0, #0 
    /*00000a54*/ BFI         R2, R1, #14, #1 
    /*00000a58*/ STR         R2, [R3, #20] 
    /*00000a5c*/ B           acodec_ioctl_x75c
    acodec_ioctl_xa60:
    /*00000a60*/ LDR         R3, [R3, #56]               @ 0x38 
    /*00000a64*/ B           acodec_ioctl_x670
    acodec_ioctl_xa68:
    /*00000a68*/ MOV         R2, #8 
    /*00000a6c*/ MOV         R0, SP 
    /*00000a70*/ BL          0x0000000000000A70          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000a74*/ LDR         R3, [SP] 
    /*00000a78*/ CMP         R3, #127                    @ 0x7f 
    /*00000a7c*/ BHI         acodec_ioctl_xd38
    /*00000a80*/ LDR         R1, [SP, #4] 
    /*00000a84*/ CMP         R1, #1 
    /*00000a88*/ BHI         acodec_ioctl_xd38
    /*00000a8c*/ LDR         IP, [R4, #4] 
    /*00000a90*/ MOV         R0, #0 
    /*00000a94*/ LDR         R2, [IP, #56]               @ 0x38 
    /*00000a98*/ BFI         R2, R1, #31, #1 
    /*00000a9c*/ BFI         R2, R3, #24, #7 
    /*00000aa0*/ STR         R2, [IP, #56]               @ 0x38 
    /*00000aa4*/ B           acodec_ioctl_x75c
    acodec_ioctl_xaa8:
    /*00000aa8*/ LDR         R3, [R3, #56]               @ 0x38 
    /*00000aac*/ MOV         R0, #0 
    /*00000ab0*/ UBFX        R3, R3, #24, #7 
    /*00000ab4*/ RSB         R3, R3, #6 
    /*00000ab8*/ STR         R3, [R1] 
    /*00000abc*/ B           acodec_ioctl_x75c
    acodec_ioctl_xac0:
    /*00000ac0*/ LDR         R6, [R1] 
    /*00000ac4*/ CMP         R6, #27 
    /*00000ac8*/ BHI         acodec_ioctl_xdd4
    /*00000acc*/ SUB         R2, R6, #1 
    /*00000ad0*/ LDR         R5, [R3, #48]               @ 0x30 
    /*00000ad4*/ CMP         R2, #10 
    /*00000ad8*/ LDRLS       PC, [PC, R2, LSL #2] 
    /*00000adc*/ B           acodec_ioctl_xd74
    /*00000ae0*/ ANDEQ       R0, R0, R0, LSL #23 
    /*00000ae4*/ ANDEQ       R0, R0, R0, LSL #23 
    /*00000ae8*/ ANDEQ       R0, R0, R0, LSL #23 
    /*00000aec*/ ANDEQ       R0, R0, IP, LSL #22 
    /*00000af0*/ ANDEQ       R0, R0, IP, LSL #22 
    /*00000af4*/ ANDEQ       R0, R0, IP, LSL #22 
    /*00000af8*/ MULEQ       R0, R0, FP 
    /*00000afc*/ MULEQ       R0, R0, FP 
    /*00000b00*/ MULEQ       R0, R0, FP 
    /*00000b04*/ ANDEQ       R0, R0, R8, LSL #23 
    /*00000b08*/ ANDEQ       R0, R0, R8, LSL #23 
    /*00000b0c*/ MOV         R2, #25 
    acodec_ioctl_xb10:
    /*00000b10*/ BFI         R5, R2, #13, #5 
    /*00000b14*/ STR         R5, [R3, #48]               @ 0x30 
    /*00000b18*/ LDR         R3, [R3, #28] 
    /*00000b1c*/ MOV         R0, R6 
    /*00000b20*/ STR         R3, [SP] 
    /*00000b24*/ BL          0x000000000000003C 
    /*00000b28*/ LDRB        R3, [SP] 
    /*00000b2c*/ BFI         R3, R0, #6, #1 
    /*00000b30*/ MOV         R0, R6 
    /*00000b34*/ STRB        R3, [SP] 
    /*00000b38*/ BL          0x000000000000003C 
    /*00000b3c*/ LDR         R2, [R4, #4] 
    /*00000b40*/ LDRB        R3, [SP] 
    /*00000b44*/ BFI         R3, R0, #7, #1 
    /*00000b48*/ STRB        R3, [SP] 
    /*00000b4c*/ LDR         R3, [SP] 
    /*00000b50*/ MOV         R0, #30 
    /*00000b54*/ STR         R3, [R2, #28] 
    /*00000b58*/ LDR         R3, [R2, #36]               @ 0x24 
    /*00000b5c*/ BFC         R3, #11, #1 
    /*00000b60*/ STR         R3, [R2, #36]               @ 0x24 
    /*00000b64*/ BL          0x0000000000000B64          @ osal_udelay [R_ARM_CALL: +0x0] 
    /*00000b68*/ LDR         R2, [R4, #4] 
    /*00000b6c*/ MOV         R0, #0 
    /*00000b70*/ LDR         R3, [R2, #36]               @ 0x24 
    /*00000b74*/ ORR         R3, R3, #2048               @ 0x800 
    /*00000b78*/ STR         R3, [R2, #36]               @ 0x24 
    /*00000b7c*/ B           acodec_ioctl_x75c
    /*00000b80*/ MOV         R2, #24 
    /*00000b84*/ B           acodec_ioctl_xb10
    /*00000b88*/ MOV         R2, #27 
    /*00000b8c*/ B           acodec_ioctl_xb10
    /*00000b90*/ MOV         R2, #26 
    /*00000b94*/ B           acodec_ioctl_xb10
    acodec_ioctl_xb98:
    /*00000b98*/ LDR         R2, [R1] 
    /*00000b9c*/ CMP         R2, #1 
    /*00000ba0*/ BHI         acodec_ioctl_x960
    /*00000ba4*/ LDR         LR, [R3, #24] 
    /*00000ba8*/ MOV         R0, #0 
    /*00000bac*/ BFI         LR, R2, #7, #1 
    /*00000bb0*/ STR         LR, [R3, #24] 
    /*00000bb4*/ B           acodec_ioctl_x75c
    acodec_ioctl_xbb8:
    /*00000bb8*/ LDR         R1, [R1] 
    /*00000bbc*/ CMP         R1, #16 
    /*00000bc0*/ BHI         acodec_ioctl_xde8
    /*00000bc4*/ LDR         R2, [R3, #32] 
    /*00000bc8*/ MOV         R0, #0 
    /*00000bcc*/ BFI         R2, R1, #16, #5 
    /*00000bd0*/ STR         R2, [R3, #32] 
    /*00000bd4*/ B           acodec_ioctl_x75c
    acodec_ioctl_xbd8:
    /*00000bd8*/ LDR         R1, [R1] 
    /*00000bdc*/ CMP         R1, #1 
    /*00000be0*/ BHI         acodec_ioctl_x960
    /*00000be4*/ LDR         R2, [R3, #20] 
    /*00000be8*/ EOR         IP, R1, #1 
    /*00000bec*/ MOV         R0, #0 
    /*00000bf0*/ BFI         R2, R1, #11, #1 
    /*00000bf4*/ STR         R2, [R3, #20] 
    /*00000bf8*/ LDR         R5, [R3, #48]               @ 0x30 
    /*00000bfc*/ BFI         R5, IP, #27, #1 
    /*00000c00*/ STR         R5, [R3, #48]               @ 0x30 
    /*00000c04*/ B           acodec_ioctl_x75c
    acodec_ioctl_xc08:
    /*00000c08*/ LDR         R1, [R1] 
    /*00000c0c*/ CMP         R1, #1 
    /*00000c10*/ BHI         acodec_ioctl_x960
    /*00000c14*/ LDR         R2, [R3, #32] 
    /*00000c18*/ EOR         IP, R1, #1 
    /*00000c1c*/ MOV         R0, #0 
    /*00000c20*/ BFI         R2, R1, #14, #1 
    /*00000c24*/ STR         R2, [R3, #32] 
    /*00000c28*/ LDR         R5, [R3, #48]               @ 0x30 
    /*00000c2c*/ BFI         R5, IP, #24, #1 
    /*00000c30*/ STR         R5, [R3, #48]               @ 0x30 
    /*00000c34*/ B           acodec_ioctl_x75c
    acodec_ioctl_xc38:
    /*00000c38*/ LDR         R1, [R1] 
    /*00000c3c*/ CMP         R1, #1 
    /*00000c40*/ BHI         acodec_ioctl_x960
    /*00000c44*/ LDR         R2, [R3, #32] 
    /*00000c48*/ MOV         R0, #0 
    /*00000c4c*/ BFI         R2, R1, #13, #1 
    /*00000c50*/ STR         R2, [R3, #32] 
    /*00000c54*/ B           acodec_ioctl_x75c
    acodec_ioctl_xc58:
    /*00000c58*/ LDR         R3, [R3, #32] 
    /*00000c5c*/ MOV         R0, #0 
    /*00000c60*/ UBFX        R3, R3, #16, #5 
    /*00000c64*/ STR         R3, [R1] 
    /*00000c68*/ B           acodec_ioctl_x75c
    acodec_ioctl_xc6c:
    /*00000c6c*/ LDR         R2, [R1] 
    /*00000c70*/ CMP         R2, #1 
    /*00000c74*/ BHI         acodec_ioctl_x960
    /*00000c78*/ LDR         LR, [R3, #24] 
    /*00000c7c*/ MOV         R0, #0 
    /*00000c80*/ BFI         LR, R2, #6, #1 
    /*00000c84*/ STR         LR, [R3, #24] 
    /*00000c88*/ B           acodec_ioctl_x75c
    acodec_ioctl_xc8c:
    /*00000c8c*/ LDR         R1, [R1] 
    /*00000c90*/ CMP         R1, #1 
    /*00000c94*/ BHI         acodec_ioctl_x960
    /*00000c98*/ LDR         R2, [R3, #32] 
    /*00000c9c*/ MOV         R0, #0 
    /*00000ca0*/ BFI         R2, R1, #9, #1 
    /*00000ca4*/ STR         R2, [R3, #32] 
    /*00000ca8*/ B           acodec_ioctl_x75c
    acodec_ioctl_xcac:
    /*00000cac*/ LDR         R2, [R1] 
    /*00000cb0*/ CMP         R2, #1 
    /*00000cb4*/ BHI         acodec_ioctl_x960
    /*00000cb8*/ LDR         R1, [R3, #60]               @ 0x3c 
    /*00000cbc*/ AND         R2, R2, #1 
    /*00000cc0*/ MOV         R0, #0 
    /*00000cc4*/ BFI         R1, R2, #14, #1 
    /*00000cc8*/ BFI         R1, R2, #15, #1 
    /*00000ccc*/ STR         R1, [R3, #60]               @ 0x3c 
    /*00000cd0*/ B           acodec_ioctl_x75c
    acodec_ioctl_xcd4:
    /*00000cd4*/ MOV         R2, #8 
    /*00000cd8*/ MOV         R0, SP 
    /*00000cdc*/ BL          0x0000000000000CDC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000ce0*/ LDR         R3, [SP] 
    /*00000ce4*/ CMP         R3, #127                    @ 0x7f 
    /*00000ce8*/ BHI         acodec_ioctl_xd38
    /*00000cec*/ LDR         R2, [SP, #4] 
    /*00000cf0*/ CMP         R2, #1 
    /*00000cf4*/ BHI         acodec_ioctl_xd38
    /*00000cf8*/ LDR         IP, [R4, #4] 
    /*00000cfc*/ MOV         R0, #0 
    /*00000d00*/ LDR         R1, [IP, #60]               @ 0x3c 
    /*00000d04*/ BFI         R1, R2, #23, #1 
    /*00000d08*/ BFI         R1, R3, #16, #7 
    /*00000d0c*/ STR         R1, [IP, #60]               @ 0x3c 
    /*00000d10*/ B           acodec_ioctl_x75c
    acodec_ioctl_xd14:
    /*00000d14*/ RSB         R0, R4, #80                 @ 0x50 
    /*00000d18*/ MOV         IP, #15 
    acodec_ioctl_xd1c:
    /*00000d1c*/ AND         R0, R0, #127                @ 0x7f 
    /*00000d20*/ BFI         R2, IP, #24, #5 
    /*00000d24*/ BFI         R1, R0, #24, #7 
    /*00000d28*/ ORR         LR, LR, #12582912           @ 0xc00000 
    /*00000d2c*/ BFI         R1, R0, #16, #7 
    /*00000d30*/ BFI         R2, IP, #16, #5 
    /*00000d34*/ B           acodec_ioctl_xa00
    acodec_ioctl_xd38:
    /*00000d38*/ MOVW        R0, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x254] 
    /*00000d3c*/ MOVT        R0, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x254] 
    /*00000d40*/ BL          0x0000000000000D40          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00000d44*/ MVN         R0, #0 
    /*00000d48*/ B           acodec_ioctl_x75c
    acodec_ioctl_xd4c:
    /*00000d4c*/ CMP         R1, #1 
    /*00000d50*/ BEQ         acodec_ioctl_xd90
    /*00000d54*/ MOV         R1, #4 
    /*00000d58*/ BFI         R2, R1, #29, #3 
    /*00000d5c*/ BFI         R2, R1, #21, #3 
    /*00000d60*/ MOVW        R1, #0                      @ g_bSoftDifferential [R_ARM_MOVW_ABS_NC: +0x0] 
    /*00000d64*/ MOVT        R1, #0                      @ g_bSoftDifferential [R_ARM_MOVT_ABS: +0x0] 
    /*00000d68*/ MOV         R0, #1 
    /*00000d6c*/ STR         R0, [R1] 
    /*00000d70*/ B           acodec_ioctl_x8a4
    acodec_ioctl_xd74:
    /*00000d74*/ MOVW        R0, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*00000d78*/ MOV         R1, R6 
    /*00000d7c*/ MOVT        R0, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*00000d80*/ BL          0x0000000000000D80          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00000d84*/ LDR         R3, [R4, #4] 
    /*00000d88*/ MOV         R2, #28 
    /*00000d8c*/ B           acodec_ioctl_xb10
    acodec_ioctl_xd90:
    /*00000d90*/ MOVW        R1, #0                      @ g_bSoftDifferential [R_ARM_MOVW_ABS_NC: +0x0] 
    /*00000d94*/ MOVT        R1, #0                      @ g_bSoftDifferential [R_ARM_MOVT_ABS: +0x0] 
    /*00000d98*/ MOV         R0, #4 
    /*00000d9c*/ BFI         R2, R0, #29, #3 
    /*00000da0*/ MOV         IP, #0 
    /*00000da4*/ BFI         R2, R0, #21, #3 
    /*00000da8*/ STR         IP, [R1] 
    /*00000dac*/ B           acodec_ioctl_x8a4
    acodec_ioctl_xdb0:
    /*00000db0*/ MOVW        R1, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*00000db4*/ MOVW        R0, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bc] 
    /*00000db8*/ MOVT        R1, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*00000dbc*/ MOVT        R0, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bc] 
    /*00000dc0*/ MOVW        R3, #273                    @ 0x111 
    /*00000dc4*/ LDR         R2, [PC, #68]               @ 0x0000000000000e10 
    /*00000dc8*/ BL          0x0000000000000DC8          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00000dcc*/ MVN         R0, #0 
    /*00000dd0*/ B           acodec_ioctl_x75c
    acodec_ioctl_xdd4:
    /*00000dd4*/ MOVW        R0, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e8] 
    /*00000dd8*/ MOVT        R0, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e8] 
    /*00000ddc*/ BL          0x0000000000000DDC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00000de0*/ MVN         R0, #0 
    /*00000de4*/ B           acodec_ioctl_x75c
    acodec_ioctl_xde8:
    /*00000de8*/ MOVW        R0, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x238] 
    /*00000dec*/ MOVT        R0, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x238] 
    /*00000df0*/ BL          0x0000000000000DF0          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00000df4*/ MVN         R0, #0 
    /*00000df8*/ B           acodec_ioctl_x75c
    acodec_ioctl_xdfc:
    /*00000dfc*/ MOVW        R0, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x214] 
    /*00000e00*/ MOVT        R0, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x214] 
    /*00000e04*/ BL          0x0000000000000E04          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00000e08*/ MVN         R0, #0 
    /*00000e0c*/ B           acodec_ioctl_x75c
    /*00000e10*/ ANDEQ       R0, R0, R4, LSL R0 
FUNC_END acodec_ioctl


FUNC_BEGIN acodec_upTimerCallBack
    /*00000e14*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000e18*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000e1c*/ LDR         R2, [R3, #4] 
    /*00000e20*/ LDR         R3, [R2, #20] 
    /*00000e24*/ BFC         R3, #11, #1 
    /*00000e28*/ BFC         R3, #12, #1 
    /*00000e2c*/ STR         R3, [R2, #20] 
    /*00000e30*/ BFC         R3, #13, #1 
    /*00000e34*/ BFC         R3, #14, #1 
    /*00000e38*/ STR         R3, [R2, #20] 
    acodec_upTimerCallBack_xe3c:
    /*00000e3c*/ B           acodec_upTimerCallBack_xe3c @ acodec_soft_reset [R_ARM_JUMP24: .text+0x360] 
FUNC_END acodec_upTimerCallBack


FUNC_BEGIN acodec_init
    /*00000e40*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000e44*/ MOV         R4, #0 
    /*00000e48*/ SUB         SP, SP, #24 
    /*00000e4c*/ MOVW        R0, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*00000e50*/ MOV         R1, R4 
    /*00000e54*/ MOVT        R0, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*00000e58*/ STR         R4, [SP] 
    /*00000e5c*/ BL          0x0000000000000E5C          @ osal_create_proc_entry [R_ARM_CALL: +0x0] 
    /*00000e60*/ SUBS        R2, R0, #0 
    /*00000e64*/ BEQ         acodec_init_x10b4
    /*00000e68*/ MOVW        R3, #0                      @ AcodecProcShow [R_ARM_MOVW_ABS_NC: .text+0x90] 
    /*00000e6c*/ MOVW        R0, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*00000e70*/ MOVT        R3, #0                      @ AcodecProcShow [R_ARM_MOVT_ABS: .text+0x90] 
    /*00000e74*/ MOVT        R0, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*00000e78*/ STR         R3, [R2, #60]               @ 0x3c 
    /*00000e7c*/ MOVW        R7, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000e80*/ BL          0x0000000000000E80          @ osal_createdev [R_ARM_CALL: +0x0] 
    /*00000e84*/ LDR         R2, [PC, #588]              @ 0x00000000000010d8 
    /*00000e88*/ MOVT        R7, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000e8c*/ MOV         R1, #31 
    /*00000e90*/ STR         R2, [R0, #56]               @ 0x38 
    /*00000e94*/ STR         R1, [R0, #52]               @ 0x34 
    /*00000e98*/ STR         R0, [R7, #8] 
    /*00000e9c*/ BL          0x0000000000000E9C          @ osal_registerdevice [R_ARM_CALL: +0x0] 
    /*00000ea0*/ SUBS        R8, R0, #0 
    /*00000ea4*/ BNE         acodec_init_x1080
    /*00000ea8*/ MOV         R0, #2 
    /*00000eac*/ BL          0x0000000000000EAC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000eb0*/ MOV         R1, #43                     @ 0x2b 
    /*00000eb4*/ LDR         R3, [R0, #20] 
    /*00000eb8*/ MOV         R2, SP 
    /*00000ebc*/ ADD         R0, SP, #12 
    /*00000ec0*/ BLX         R3 
    /*00000ec4*/ MOVW        R3, #0                      @ pAcodec [R_ARM_MOVW_ABS_NC: +0x0] 
    /*00000ec8*/ MOVT        R3, #0                      @ pAcodec [R_ARM_MOVT_ABS: +0x0] 
    /*00000ecc*/ LDR         R3, [R3] 
    /*00000ed0*/ CMP         R3, #0 
    /*00000ed4*/ STRNE       R3, [R7, #4] 
    /*00000ed8*/ BEQ         acodec_init_x1008
    acodec_init_xedc:
    /*00000edc*/ MOV         R0, #10 
    /*00000ee0*/ MOV         R6, #65024                  @ 0xfe00 
    /*00000ee4*/ BL          0x0000000000000EE4          @ osal_msleep [R_ARM_CALL: +0x0] 
    /*00000ee8*/ LDR         R3, [R7, #4] 
    /*00000eec*/ MOV         R1, #64                     @ 0x40 
    /*00000ef0*/ MOV         R2, #27 
    /*00000ef4*/ MOVT        R2, #16536                  @ 0x4098 
    /*00000ef8*/ MOVT        R6, #33667                  @ 0x8383 
    /*00000efc*/ MOV         R0, #1 
    /*00000f00*/ MOVW        R4, #30945                  @ 0x78e1 
    /*00000f04*/ MOVT        R4, #1029                   @ 0x405 
    /*00000f08*/ STR         R4, [R3, #20] 
    /*00000f0c*/ MOV         R5, #0 
    /*00000f10*/ MOVW        R4, #30947                  @ 0x78e3 
    /*00000f14*/ MOVT        R4, #1029                   @ 0x405 
    /*00000f18*/ STR         R2, [SP, #4] 
    /*00000f1c*/ STRB        R1, [SP, #7] 
    /*00000f20*/ MOV         R1, #4 
    /*00000f24*/ LDR         IP, [SP, #4] 
    /*00000f28*/ MOVT        R1, #64802                  @ 0xfd22 
    /*00000f2c*/ STR         R1, [R3, #24] 
    /*00000f30*/ BFI         R1, R5, #17, #1 
    /*00000f34*/ STR         R2, [R3, #28] 
    /*00000f38*/ MOVW        R2, #20572                  @ 0x505c 
    /*00000f3c*/ STR         R6, [R3, #32] 
    /*00000f40*/ BFI         R6, R0, #3, #2 
    /*00000f44*/ STR         R2, [R3, #36]               @ 0x24 
    /*00000f48*/ BFI         R6, R0, #5, #2 
    /*00000f4c*/ STR         R5, [R3, #40]               @ 0x28 
    /*00000f50*/ STR         IP, [R3, #28] 
    /*00000f54*/ STR         R6, [R3, #32] 
    /*00000f58*/ ORR         R6, R6, #5 
    /*00000f5c*/ LDRH        R0, [SP, #6] 
    /*00000f60*/ LDRB        R2, [SP, #4] 
    /*00000f64*/ BFI         R0, R5, #4, #5 
    /*00000f68*/ STRH        R0, [SP, #6] 
    /*00000f6c*/ LDR         IP, [SP, #4] 
    /*00000f70*/ BFI         R2, R5, #0, #1 
    /*00000f74*/ STRB        R2, [SP, #4] 
    /*00000f78*/ MOV         R0, #30 
    /*00000f7c*/ LDR         R2, [SP, #4] 
    /*00000f80*/ STR         IP, [R3, #28] 
    /*00000f84*/ STR         R4, [R3, #20] 
    /*00000f88*/ BFI         R4, R5, #0, #1 
    /*00000f8c*/ STR         R1, [R3, #24] 
    /*00000f90*/ STR         R2, [R3, #28] 
    /*00000f94*/ STR         R4, [R3, #20] 
    /*00000f98*/ BFI         R4, R5, #12, #1 
    /*00000f9c*/ STR         R6, [R3, #32] 
    /*00000fa0*/ BFI         R4, R5, #11, #1 
    /*00000fa4*/ BL          0x0000000000000FA4          @ osal_msleep [R_ARM_CALL: +0x0] 
    /*00000fa8*/ LDR         R3, [R7, #4] 
    /*00000fac*/ BFI         R4, R5, #14, #1 
    /*00000fb0*/ BFI         R6, R5, #11, #1 
    /*00000fb4*/ BFI         R4, R5, #13, #1 
    /*00000fb8*/ STR         R4, [R3, #20] 
    /*00000fbc*/ BFI         R4, R5, #16, #1 
    /*00000fc0*/ BFI         R4, R5, #18, #1 
    /*00000fc4*/ STR         R4, [R3, #20] 
    /*00000fc8*/ LDRB        R2, [SP, #4] 
    /*00000fcc*/ BFI         R2, R5, #1, #1 
    /*00000fd0*/ STRB        R2, [SP, #4] 
    /*00000fd4*/ LDR         R1, [SP, #4] 
    /*00000fd8*/ MOVW        R2, #21596                  @ 0x545c 
    /*00000fdc*/ STR         R1, [R3, #28] 
    /*00000fe0*/ STR         R6, [R3, #32] 
    /*00000fe4*/ STR         R2, [R3, #36]               @ 0x24 
    /*00000fe8*/ MOVW        R2, #23644                  @ 0x5c5c 
    /*00000fec*/ STR         R2, [R3, #36]               @ 0x24 
    /*00000ff0*/ BL          0x0000000000000FF0          @ acodec_soft_reset [R_ARM_CALL: .text+0x360] 
    /*00000ff4*/ LDR         R0, [PC, #224]              @ 0x00000000000010dc 
    /*00000ff8*/ BL          0x0000000000000FF8          @ osal_register_reboot_notifier [R_ARM_CALL: +0x0] 
    acodec_init_xffc:
    /*00000ffc*/ MOV         R0, R8 
    /*00001000*/ ADD         SP, SP, #24 
    /*00001004*/ POP         {R4, R5, R6, R7, R8, PC} 
    acodec_init_x1008:
    /*00001008*/ MOV         R0, #0 
    /*0000100c*/ MOVT        R0, #4412                   @ 0x113c 
    /*00001010*/ BL          0x0000000000001010          @ osal_ioaddress [R_ARM_CALL: +0x0] 
    /*00001014*/ CMP         R0, #0 
    /*00001018*/ STR         R0, [R7, #4] 
    /*0000101c*/ BNE         acodec_init_xedc
    /*00001020*/ MOVW        R3, #1014                   @ 0x3f6 
    /*00001024*/ LDR         R2, [PC, #180]              @ 0x00000000000010e0 
    /*00001028*/ MOVW        R1, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*0000102c*/ MOVW        R0, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2bc] 
    /*00001030*/ MOVT        R1, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*00001034*/ MOVT        R0, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2bc] 
    /*00001038*/ BL          0x0000000000001038          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000103c*/ LDR         R0, [R7, #8] 
    /*00001040*/ BL          0x0000000000001040          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*00001044*/ LDR         R0, [R7, #8] 
    /*00001048*/ BL          0x0000000000001048          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*0000104c*/ MOV         R1, R8 
    /*00001050*/ MOVW        R0, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*00001054*/ MOVT        R0, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*00001058*/ BL          0x0000000000001058          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*0000105c*/ MOVW        R1, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*00001060*/ MOVW        R0, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ec] 
    /*00001064*/ MOVT        R1, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*00001068*/ MOVW        R3, #1375                   @ 0x55f 
    /*0000106c*/ LDR         R2, [PC, #112]              @ 0x00000000000010e4 
    /*00001070*/ MOVT        R0, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ec] 
    /*00001074*/ MVN         R8, #0 
    /*00001078*/ BL          0x0000000000001078          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000107c*/ B           acodec_init_xffc
    acodec_init_x1080:
    /*00001080*/ MOV         R1, R4 
    /*00001084*/ MOVW        R0, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*00001088*/ MOVT        R0, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*0000108c*/ MVN         R8, #0 
    /*00001090*/ BL          0x0000000000001090          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*00001094*/ MOVW        R1, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*00001098*/ MOVW        R0, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x290] 
    /*0000109c*/ MOVT        R1, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*000010a0*/ MOVW        R3, #1363                   @ 0x553 
    /*000010a4*/ LDR         R2, [PC, #56]               @ 0x00000000000010e4 
    /*000010a8*/ MOVT        R0, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x290] 
    /*000010ac*/ BL          0x00000000000010AC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000010b0*/ B           acodec_init_xffc
    acodec_init_x10b4:
    /*000010b4*/ MOVW        R1, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*000010b8*/ MOVW        R0, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x264] 
    /*000010bc*/ MOVT        R1, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*000010c0*/ MOVT        R0, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x264] 
    /*000010c4*/ MOVW        R3, #1342                   @ 0x53e 
    /*000010c8*/ LDR         R2, [PC, #20]               @ 0x00000000000010e4 
    /*000010cc*/ BL          0x00000000000010CC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000010d0*/ MVN         R8, #0 
    /*000010d4*/ B           acodec_init_xffc
    /*000010d8*/ ANDEQ       R0, R0, R4 
    /*000010dc*/ ANDEQ       R0, R0, R4, LSR #32 
    /*000010e0*/ ANDEQ       R0, R0, R0, LSR R0 
    /*000010e4*/ ANDEQ       R0, R0, R4, LSR #32 
FUNC_END acodec_init


FUNC_BEGIN acodec_exit
    /*000010e8*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*000010ec*/ SUB         SP, SP, #20 
    /*000010f0*/ ADD         R7, SP, #16 
    /*000010f4*/ LDR         R0, [PC, #224]              @ 0x00000000000011dc 
    /*000010f8*/ MOV         R4, #1 
    /*000010fc*/ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001100*/ STR         R4, [R7, #-16]! 
    /*00001104*/ MOVT        R6, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001108*/ BL          0x0000000000001108          @ osal_unregister_reboot_notifier [R_ARM_CALL: +0x0] 
    /*0000110c*/ BL          0x000000000000110C          @ acodec_soft_reset [R_ARM_CALL: .text+0x360] 
    /*00001110*/ MOV         R0, #50                     @ 0x32 
    /*00001114*/ BL          0x0000000000001114          @ osal_msleep [R_ARM_CALL: +0x0] 
    /*00001118*/ LDR         R3, [R6, #4] 
    /*0000111c*/ MOV         R0, R4 
    /*00001120*/ LDR         R8, [R3, #20] 
    /*00001124*/ LDR         R9, [R3, #28] 
    /*00001128*/ LDR         R5, [R3, #32] 
    /*0000112c*/ ORR         R4, R8, #24576              @ 0x6000 
    /*00001130*/ ORR         R5, R5, #125                @ 0x7d 
    /*00001134*/ STR         R5, [R3, #32] 
    /*00001138*/ BL          0x0000000000001138          @ osal_msleep [R_ARM_CALL: +0x0] 
    /*0000113c*/ LDR         R3, [R6, #4] 
    /*00001140*/ BFC         R5, #2, #1 
    /*00001144*/ MOV         R0, #1000                   @ 0x3e8 
    /*00001148*/ BFC         R5, #0, #1 
    /*0000114c*/ STR         R4, [R3, #20] 
    /*00001150*/ ORR         R4, R8, #352256             @ 0x56000 
    /*00001154*/ STR         R4, [R3, #20] 
    /*00001158*/ ORR         R4, R4, #6144               @ 0x1800 
    /*0000115c*/ STR         R4, [R3, #20] 
    /*00001160*/ ORR         R4, R4, #1 
    /*00001164*/ STR         R5, [R3, #32] 
    /*00001168*/ BFC         R5, #21, #3 
    /*0000116c*/ BL          0x000000000000116C          @ osal_msleep [R_ARM_CALL: +0x0] 
    /*00001170*/ LDR         R3, [R6, #4] 
    /*00001174*/ MOV         R0, #50                     @ 0x32 
    /*00001178*/ BFC         R5, #29, #3 
    /*0000117c*/ STR         R4, [R3, #20] 
    /*00001180*/ ORR         R4, R4, #32 
    /*00001184*/ BL          0x0000000000001184          @ osal_msleep [R_ARM_CALL: +0x0] 
    /*00001188*/ LDR         R3, [R6, #4] 
    /*0000118c*/ MOV         R0, #2 
    /*00001190*/ STR         R9, [R3, #28] 
    /*00001194*/ STR         R4, [R3, #20] 
    /*00001198*/ STR         R5, [R3, #32] 
    /*0000119c*/ BL          0x000000000000119C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000011a0*/ MOV         R2, R7 
    /*000011a4*/ MOV         R1, #43                     @ 0x2b 
    /*000011a8*/ LDR         R3, [R0, #20] 
    /*000011ac*/ ADD         R0, SP, #4 
    /*000011b0*/ BLX         R3 
    /*000011b4*/ LDR         R0, [R6, #8] 
    /*000011b8*/ BL          0x00000000000011B8          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*000011bc*/ LDR         R0, [R6, #8] 
    /*000011c0*/ BL          0x00000000000011C0          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*000011c4*/ MOV         R1, #0 
    /*000011c8*/ MOVW        R0, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*000011cc*/ MOVT        R0, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*000011d0*/ BL          0x00000000000011D0          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*000011d4*/ ADD         SP, SP, #20 
    /*000011d8*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    /*000011dc*/ ANDEQ       R0, R0, R4, LSR #32 
FUNC_END acodec_exit


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .init.text
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN acodec_mod_init
    acodec_mod_init_x0:
    /*00000000*/ B           acodec_mod_init_x0          @ acodec_init [R_ARM_JUMP24: .text+0xe40] 
FUNC_END acodec_mod_init




@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .exit.text
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN acodec_mod_exit
    acodec_mod_exit_x0:
    /*00000000*/ B           acodec_mod_exit_x0          @ acodec_exit [R_ARM_JUMP24: .text+0x10e8] 
FUNC_END acodec_mod_exit




