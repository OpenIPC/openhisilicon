@
@ -- FILES --
@

@ aenc.c
@ audio_fb.c
@ aenc_drv.c
@ aenc_init.c
@ hi3516cv500_aenc.mod.c


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text
@ Size: 0x2eac
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN aenc_restore
    /*00000000*/ MOV         R0, #0 
    /*00000004*/ BX          LR 
FUNC_END aenc_restore


FUNC_BEGIN AENC_GetVerMagic
    /*00000008*/ MOVW        R0, #61479                  @ 0xf027 
    /*0000000c*/ MOVT        R0, #307                    @ 0x133 
    /*00000010*/ BX          LR 
FUNC_END AENC_GetVerMagic


FUNC_BEGIN AENC_Notify
    /*00000014*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000018*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000001c*/ LDR         R5, [PC, #104]              @ 0x000000000000008c 
    /*00000020*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000024*/ MOV         R2, #1 
    /*00000028*/ MOV         R4, #0 
    /*0000002c*/ SUB         R7, R5, #208                @ 0xd0 
    /*00000030*/ MOV         R6, #352                    @ 0x160 
    /*00000034*/ STR         R2, [R3] 
    /*00000038*/ B           AENC_Notify_x50
    AENC_Notify_x3c:
    /*0000003c*/ MOV         R0, R5 
    /*00000040*/ BL          0x0000000000000040          @ osal_up [R_ARM_CALL: +0x0] 
    AENC_Notify_x44:
    /*00000044*/ CMP         R4, #32 
    /*00000048*/ ADD         R5, R5, #352                @ 0x160 
    /*0000004c*/ BEQ         AENC_Notify_x88
    AENC_Notify_x50:
    /*00000050*/ MOV         R0, R5 
    /*00000054*/ BL          0x0000000000000054          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000058*/ MLA         R3, R6, R4, R7 
    /*0000005c*/ ADD         R4, R4, #1 
    /*00000060*/ CMP         R0, #0 
    /*00000064*/ BNE         AENC_Notify_x44
    /*00000068*/ LDR         R3, [R3, #248]              @ 0xf8 
    /*0000006c*/ CMP         R3, #1 
    /*00000070*/ BNE         AENC_Notify_x3c
    /*00000074*/ ADD         R0, R5, #36                 @ 0x24 
    /*00000078*/ BL          0x0000000000000078          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*0000007c*/ ADD         R0, R5, #4 
    /*00000080*/ BL          0x0000000000000080          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00000084*/ B           AENC_Notify_x3c
    AENC_Notify_x88:
    /*00000088*/ POP         {R4, R5, R6, R7, R8, PC} 
    /*0000008c*/ LDRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
FUNC_END AENC_Notify


FUNC_BEGIN AencAddVb
    /*00000090*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000094*/ MOVW        SL, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*00000098*/ LDR         R3, [R0, #4] 
    /*0000009c*/ MOVW        R9, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000000a0*/ MOVW        R8, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*000000a4*/ SUB         SP, SP, #12 
    /*000000a8*/ CMP         R3, #1 
    /*000000ac*/ ADD         R4, R0, #48                 @ 0x30 
    /*000000b0*/ ADD         R6, R0, #16 
    /*000000b4*/ MOVT        SL, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*000000b8*/ MOVNE       R7, #52                     @ 0x34 
    /*000000bc*/ MOVEQ       R7, #56                     @ 0x38 
    /*000000c0*/ MOVT        R9, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*000000c4*/ ADD         R7, R0, R7 
    /*000000c8*/ MOVT        R8, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*000000cc*/ MOV         R5, R0 
    /*000000d0*/ MOV         FP, R1 
    AencAddVb_xd4:
    /*000000d4*/ MOV         R0, #1 
    /*000000d8*/ BL          0x00000000000000D8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000000dc*/ LDR         IP, [R4], #4 
    /*000000e0*/ LDRD        R2, [R6], #8 
    /*000000e4*/ STR         FP, [SP] 
    /*000000e8*/ LDR         R1, [R0, #60]               @ 0x3c 
    /*000000ec*/ MOV         R0, IP 
    /*000000f0*/ BLX         R1 
    /*000000f4*/ CMP         R0, #0 
    /*000000f8*/ BNE         AencAddVb_x11c
    /*000000fc*/ LDR         R0, [R5, #112]              @ 0x70 
    /*00000100*/ CMP         R0, #1 
    /*00000104*/ BEQ         AencAddVb_x13c
    AencAddVb_x108:
    /*00000108*/ CMP         R4, R7 
    /*0000010c*/ BNE         AencAddVb_xd4
    /*00000110*/ MOV         R0, #0 
    /*00000114*/ ADD         SP, SP, #12 
    /*00000118*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AencAddVb_x11c:
    /*0000011c*/ MOV         R0, R8 
    /*00000120*/ MOV         R3, SL 
    /*00000124*/ MOVW        R2, #407                    @ 0x197 
    /*00000128*/ MOV         R1, R9 
    /*0000012c*/ BL          0x000000000000012C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000130*/ LDR         R0, [R5, #112]              @ 0x70 
    /*00000134*/ CMP         R0, #1 
    /*00000138*/ BNE         AencAddVb_x108
    AencAddVb_x13c:
    /*0000013c*/ BL          0x000000000000013C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000140*/ LDR         IP, [R4, #52]               @ 0x34 
    /*00000144*/ LDRD        R2, [R6, #48]               @ 0x30 
    /*00000148*/ STR         FP, [SP] 
    /*0000014c*/ LDR         R1, [R0, #60]               @ 0x3c 
    /*00000150*/ MOV         R0, IP 
    /*00000154*/ BLX         R1 
    /*00000158*/ CMP         R0, #0 
    /*0000015c*/ BEQ         AencAddVb_x108
    /*00000160*/ MOV         R3, SL 
    /*00000164*/ MOV         R2, #412                    @ 0x19c 
    /*00000168*/ MOV         R1, R9 
    /*0000016c*/ MOV         R0, R8 
    /*00000170*/ BL          0x0000000000000170          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000174*/ B           AencAddVb_x108
FUNC_END AencAddVb


FUNC_BEGIN AencProcShow
    /*00000178*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000017c*/ MOVW        R1, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000180*/ SUB         SP, SP, #36                 @ 0x24 
    /*00000184*/ MOV         R9, R0 
    /*00000188*/ MOVT        R1, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*0000018c*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000190*/ BL          0x0000000000000190          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000194*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000198*/ MOV         R0, R9 
    /*0000019c*/ MOVW        R1, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18c] 
    /*000001a0*/ MOVT        R1, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18c] 
    /*000001a4*/ MOVW        SL, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x230] 
    /*000001a8*/ BL          0x00000000000001A8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000001ac*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x208] 
    /*000001b0*/ MOVW        R3, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x214] 
    /*000001b4*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x208] 
    /*000001b8*/ MOVT        R3, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x214] 
    /*000001bc*/ STR         R2, [SP, #12] 
    /*000001c0*/ STR         R3, [SP, #8] 
    /*000001c4*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*000001c8*/ MOVW        R3, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x224] 
    /*000001cc*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*000001d0*/ MOVT        R3, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x224] 
    /*000001d4*/ STR         R2, [SP, #4] 
    /*000001d8*/ STR         R3, [SP] 
    /*000001dc*/ MOV         R0, R9 
    /*000001e0*/ MOVW        R3, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e0] 
    /*000001e4*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e8] 
    /*000001e8*/ MOVT        R3, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e0] 
    /*000001ec*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e8] 
    /*000001f0*/ MOVW        R1, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f0] 
    /*000001f4*/ MOVT        R1, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f0] 
    /*000001f8*/ BL          0x00000000000001F8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000001fc*/ MOVW        FP, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbc] 
    /*00000200*/ MOVW        R3, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc8] 
    /*00000204*/ ADD         R8, R5, #11264              @ 0x2c00 
    /*00000208*/ MOVT        R3, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc8] 
    /*0000020c*/ MOVT        SL, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x230] 
    /*00000210*/ MOVT        FP, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbc] 
    /*00000214*/ STR         R3, [SP, #24] 
    /*00000218*/ MOV         R7, R5 
    /*0000021c*/ MOV         R4, R5 
    /*00000220*/ MOV         R6, #0 
    /*00000224*/ STR         R5, [SP, #28] 
    /*00000228*/ B           AencProcShow_x34c
    AencProcShow_x22c:
    /*0000022c*/ CMP         R2, #20 
    /*00000230*/ BEQ         AencProcShow_x378
    /*00000234*/ CMP         R2, #49                     @ 0x31 
    /*00000238*/ BEQ         AencProcShow_x398
    /*0000023c*/ CMP         R2, #21 
    /*00000240*/ BEQ         AencProcShow_x3a4
    /*00000244*/ MOVW        R3, #1001                   @ 0x3e9 
    /*00000248*/ CMP         R2, R3 
    /*0000024c*/ BEQ         AencProcShow_x3bc
    /*00000250*/ MOVW        R3, #1003                   @ 0x3eb 
    /*00000254*/ CMP         R2, R3 
    /*00000258*/ BEQ         AencProcShow_x3d4
    /*0000025c*/ CMP         R2, #37                     @ 0x25 
    /*00000260*/ BEQ         AencProcShow_x4ec
    /*00000264*/ CMP         R2, #23 
    /*00000268*/ BEQ         AencProcShow_x504
    /*0000026c*/ MOVW        R1, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x94] 
    /*00000270*/ CMP         R2, #43                     @ 0x2b 
    /*00000274*/ MOVT        R1, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x94] 
    /*00000278*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x98] 
    /*0000027c*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x98] 
    /*00000280*/ MOVEQ       R3, R1 
    AencProcShow_x284:
    /*00000284*/ LDR         R2, [R4, #300]              @ 0x12c 
    /*00000288*/ CMP         R2, #0 
    /*0000028c*/ MOVWEQ      R0, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9c] 
    /*00000290*/ MOVTEQ      R0, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9c] 
    /*00000294*/ BEQ         AencProcShow_x2b8
    /*00000298*/ CMP         R2, #1 
    /*0000029c*/ BEQ         AencProcShow_x38c
    /*000002a0*/ MOVW        R1, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb4] 
    /*000002a4*/ CMP         R2, #2 
    /*000002a8*/ MOVT        R1, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb4] 
    /*000002ac*/ MOVW        R0, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*000002b0*/ MOVT        R0, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*000002b4*/ MOVNE       R0, R1 
    AencProcShow_x2b8:
    /*000002b8*/ LDR         R2, [R4, #296]              @ 0x128 
    /*000002bc*/ LDR         IP, [R4, #256]              @ 0x100 
    /*000002c0*/ CMP         R2, #0 
    /*000002c4*/ LDR         LR, [R4, #260]              @ 0x104 
    /*000002c8*/ MOVEQ       R2, FP 
    /*000002cc*/ BEQ         AencProcShow_x31c
    /*000002d0*/ CMP         R2, #1 
    /*000002d4*/ BEQ         AencProcShow_x384
    /*000002d8*/ CMP         R2, #2 
    /*000002dc*/ BEQ         AencProcShow_x3b0
    /*000002e0*/ CMP         R2, #3 
    /*000002e4*/ BEQ         AencProcShow_x3c8
    /*000002e8*/ CMP         R2, #4 
    /*000002ec*/ BEQ         AencProcShow_x4f8
    /*000002f0*/ CMP         R2, #5 
    /*000002f4*/ BEQ         AencProcShow_x510
    /*000002f8*/ CMP         R2, #6 
    /*000002fc*/ BEQ         AencProcShow_x51c
    /*00000300*/ CMP         R2, #7 
    /*00000304*/ MOVW        R1, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11c] 
    /*00000308*/ MOVW        R5, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb4] 
    /*0000030c*/ MOVT        R1, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11c] 
    /*00000310*/ MOVT        R5, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb4] 
    /*00000314*/ MOVEQ       R2, R1 
    /*00000318*/ MOVNE       R2, R5 
    AencProcShow_x31c:
    /*0000031c*/ STR         R2, [SP, #12] 
    /*00000320*/ MOV         R1, SL 
    /*00000324*/ STR         R0, [SP] 
    /*00000328*/ MOV         R2, R6 
    /*0000032c*/ STR         LR, [SP, #8] 
    /*00000330*/ MOV         R0, R9 
    /*00000334*/ STR         IP, [SP, #4] 
    /*00000338*/ BL          0x0000000000000338          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    AencProcShow_x33c:
    /*0000033c*/ ADD         R4, R4, #352                @ 0x160 
    /*00000340*/ ADD         R6, R6, #1 
    /*00000344*/ CMP         R8, R4 
    /*00000348*/ BEQ         AencProcShow_x3e0
    AencProcShow_x34c:
    /*0000034c*/ MOV         R3, #352                    @ 0x160 
    /*00000350*/ MLA         R3, R3, R6, R7 
    /*00000354*/ LDR         R3, [R3, #248]              @ 0xf8 
    /*00000358*/ CMP         R3, #0 
    /*0000035c*/ BEQ         AencProcShow_x33c
    /*00000360*/ LDR         R2, [R4, #252]              @ 0xfc 
    /*00000364*/ CMP         R2, #19 
    /*00000368*/ BNE         AencProcShow_x22c
    /*0000036c*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x58] 
    /*00000370*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x58] 
    /*00000374*/ B           AencProcShow_x284
    AencProcShow_x378:
    /*00000378*/ MOVW        R3, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x60] 
    /*0000037c*/ MOVT        R3, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x60] 
    /*00000380*/ B           AencProcShow_x284
    AencProcShow_x384:
    /*00000384*/ LDR         R2, [SP, #24] 
    /*00000388*/ B           AencProcShow_x31c
    AencProcShow_x38c:
    /*0000038c*/ MOVW        R0, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa4] 
    /*00000390*/ MOVT        R0, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa4] 
    /*00000394*/ B           AencProcShow_x2b8
    AencProcShow_x398:
    /*00000398*/ MOVW        R3, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x68] 
    /*0000039c*/ MOVT        R3, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x68] 
    /*000003a0*/ B           AencProcShow_x284
    AencProcShow_x3a4:
    /*000003a4*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x70] 
    /*000003a8*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x70] 
    /*000003ac*/ B           AencProcShow_x284
    AencProcShow_x3b0:
    /*000003b0*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd4] 
    /*000003b4*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd4] 
    /*000003b8*/ B           AencProcShow_x31c
    AencProcShow_x3bc:
    /*000003bc*/ MOVW        R3, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x78] 
    /*000003c0*/ MOVT        R3, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x78] 
    /*000003c4*/ B           AencProcShow_x284
    AencProcShow_x3c8:
    /*000003c8*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe0] 
    /*000003cc*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe0] 
    /*000003d0*/ B           AencProcShow_x31c
    AencProcShow_x3d4:
    /*000003d4*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x80] 
    /*000003d8*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x80] 
    /*000003dc*/ B           AencProcShow_x284
    AencProcShow_x3e0:
    /*000003e0*/ MOVW        R1, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x248] 
    /*000003e4*/ MOV         R0, R9 
    /*000003e8*/ MOVT        R1, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x248] 
    /*000003ec*/ LDR         R5, [SP, #28] 
    /*000003f0*/ BL          0x00000000000003F0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000003f4*/ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c8] 
    /*000003f8*/ MOVW        R3, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2d0] 
    /*000003fc*/ MOVT        R2, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c8] 
    /*00000400*/ MOVT        R3, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2d0] 
    /*00000404*/ STR         R2, [SP, #20] 
    /*00000408*/ STR         R3, [SP, #16] 
    /*0000040c*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2d8] 
    /*00000410*/ MOVW        R3, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2e0] 
    /*00000414*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2d8] 
    /*00000418*/ MOVT        R3, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2e0] 
    /*0000041c*/ STR         R2, [SP, #12] 
    /*00000420*/ STR         R3, [SP, #8] 
    /*00000424*/ MOVW        R2, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2e8] 
    /*00000428*/ MOVW        R3, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2f0] 
    /*0000042c*/ MOVT        R2, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2e8] 
    /*00000430*/ MOVT        R3, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2f0] 
    /*00000434*/ STR         R2, [SP, #4] 
    /*00000438*/ STR         R3, [SP] 
    /*0000043c*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e8] 
    /*00000440*/ MOVW        R3, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*00000444*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e8] 
    /*00000448*/ MOVT        R3, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c] 
    /*0000044c*/ MOVW        R1, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a4] 
    /*00000450*/ MOV         R0, R9 
    /*00000454*/ MOVT        R1, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a4] 
    /*00000458*/ MOVW        SL, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2fc] 
    /*0000045c*/ BL          0x000000000000045C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00000460*/ MOVT        SL, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2fc] 
    /*00000464*/ MOV         R6, #0 
    /*00000468*/ MOV         R8, #352                    @ 0x160 
    /*0000046c*/ B           AencProcShow_x480
    AencProcShow_x470:
    /*00000470*/ ADD         R5, R5, #352                @ 0x160 
    /*00000474*/ ADD         R6, R6, #1 
    /*00000478*/ CMP         R4, R5 
    /*0000047c*/ BEQ         AencProcShow_x4e0
    AencProcShow_x480:
    /*00000480*/ MLA         R3, R8, R6, R7 
    /*00000484*/ LDR         R3, [R3, #248]              @ 0xf8 
    /*00000488*/ CMP         R3, #0 
    /*0000048c*/ BEQ         AencProcShow_x470
    /*00000490*/ LDR         FP, [R5, #292]              @ 0x124 
    /*00000494*/ MOV         R2, R6 
    /*00000498*/ LDR         LR, [R5, #288]              @ 0x120 
    /*0000049c*/ MOV         R1, SL 
    /*000004a0*/ LDR         IP, [R5, #284]              @ 0x11c 
    /*000004a4*/ MOV         R0, R9 
    /*000004a8*/ LDR         R3, [R5, #272]              @ 0x110 
    /*000004ac*/ ADD         R5, R5, #352                @ 0x160 
    /*000004b0*/ STR         FP, [SP, #20] 
    /*000004b4*/ ADD         R6, R6, #1 
    /*000004b8*/ LDR         FP, [R5, #-76]              @ 0xffffffb4 
    /*000004bc*/ STR         LR, [SP, #16] 
    /*000004c0*/ LDR         LR, [R5, #-44]              @ 0xffffffd4 
    /*000004c4*/ STR         IP, [SP, #12] 
    /*000004c8*/ LDR         IP, [R5, #-72]              @ 0xffffffb8 
    /*000004cc*/ STR         LR, [SP] 
    /*000004d0*/ STMIB       SP, {FP, IP} 
    /*000004d4*/ BL          0x00000000000004D4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000004d8*/ CMP         R4, R5 
    /*000004dc*/ BNE         AencProcShow_x480
    AencProcShow_x4e0:
    /*000004e0*/ MOV         R0, #0 
    /*000004e4*/ ADD         SP, SP, #36                 @ 0x24 
    /*000004e8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AencProcShow_x4ec:
    /*000004ec*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x88] 
    /*000004f0*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x88] 
    /*000004f4*/ B           AencProcShow_x284
    AencProcShow_x4f8:
    /*000004f8*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xec] 
    /*000004fc*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0xec] 
    /*00000500*/ B           AencProcShow_x31c
    AencProcShow_x504:
    /*00000504*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8c] 
    /*00000508*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8c] 
    /*0000050c*/ B           AencProcShow_x284
    AencProcShow_x510:
    /*00000510*/ MOVW        R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00000514*/ MOVT        R2, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00000518*/ B           AencProcShow_x31c
    AencProcShow_x51c:
    /*0000051c*/ MOVW        R2, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10c] 
    /*00000520*/ MOVT        R2, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10c] 
    /*00000524*/ B           AencProcShow_x31c
FUNC_END AencProcShow


FUNC_BEGIN AencPoll
    /*00000528*/ PUSH        {R4, R5, R6, R7, LR} 
    /*0000052c*/ MOV         R4, R1 
    /*00000530*/ SUB         SP, SP, #12 
    /*00000534*/ MOV         R7, R0 
    /*00000538*/ LDR         R0, [PC, #220]              @ 0x000000000000061c 
    /*0000053c*/ BL          0x000000000000053C          @ osal_atomic_inc_return [R_ARM_CALL: +0x0] 
    /*00000540*/ LDR         R6, [R4] 
    /*00000544*/ CMP         R6, #31 
    /*00000548*/ BHI         AencPoll_x5bc
    AencPoll_x54c:
    /*0000054c*/ MOV         R5, #352                    @ 0x160 
    /*00000550*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000554*/ MUL         R5, R5, R6 
    /*00000558*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000055c*/ MOV         R0, R7 
    /*00000560*/ ADD         R1, R5, #212                @ 0xd4 
    /*00000564*/ ADD         R1, R4, R1 
    /*00000568*/ BL          0x0000000000000568          @ osal_poll_wait [R_ARM_CALL: +0x0] 
    /*0000056c*/ LDR         IP, [R4, R5] 
    /*00000570*/ ADD         R4, R4, R5 
    /*00000574*/ CMP         IP, #0 
    /*00000578*/ BEQ         AencPoll_x5dc
    /*0000057c*/ LDR         R3, [R4, #168]              @ 0xa8 
    /*00000580*/ LDR         R2, [R4, #180]              @ 0xb4 
    /*00000584*/ LDR         R3, [R3] 
    /*00000588*/ LDR         R2, [R2] 
    /*0000058c*/ CMP         R3, R2 
    /*00000590*/ LDRHI       R1, [R4, #16] 
    /*00000594*/ ADDHI       R2, R2, R1 
    /*00000598*/ SUB         R3, R2, R3 
    /*0000059c*/ CMP         R3, #0 
    /*000005a0*/ MOVNE       R4, #65                     @ 0x41 
    /*000005a4*/ MOVEQ       R4, #0 
    AencPoll_x5a8:
    /*000005a8*/ LDR         R0, [PC, #108]              @ 0x000000000000061c 
    /*000005ac*/ BL          0x00000000000005AC          @ osal_atomic_dec_return [R_ARM_CALL: +0x0] 
    /*000005b0*/ MOV         R0, R4 
    /*000005b4*/ ADD         SP, SP, #12 
    /*000005b8*/ POP         {R4, R5, R6, R7, PC} 
    AencPoll_x5bc:
    /*000005bc*/ MOVW        R3, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x320] 
    /*000005c0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*000005c4*/ MOVT        R3, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x320] 
    /*000005c8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*000005cc*/ MOVW        R2, #1203                   @ 0x4b3 
    /*000005d0*/ LDR         R1, [PC, #72]               @ 0x0000000000000620 
    /*000005d4*/ BL          0x00000000000005D4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000005d8*/ B           AencPoll_x54c
    AencPoll_x5dc:
    /*000005dc*/ MOVW        R3, #1211                   @ 0x4bb 
    /*000005e0*/ MOV         R1, #23 
    /*000005e4*/ STR         R3, [SP] 
    /*000005e8*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*000005ec*/ LDR         R3, [PC, #44]               @ 0x0000000000000620 
    /*000005f0*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*000005f4*/ MOV         R0, #5 
    /*000005f8*/ MOV         R4, IP 
    /*000005fc*/ BL          0x00000000000005FC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000600*/ MOV         R3, R6 
    /*00000604*/ MOVW        R2, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x364] 
    /*00000608*/ MOV         R1, #23 
    /*0000060c*/ MOVT        R2, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x364] 
    /*00000610*/ MOV         R0, #5 
    /*00000614*/ BL          0x0000000000000614          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000618*/ B           AencPoll_x5a8
    /*0000061c*/ ANDEQ       R2, R0, R0, LSL #24 
    /*00000620*/ ANDEQ       R0, R0, IP 
FUNC_END AencPoll


FUNC_BEGIN AencGetStream_WaitConditionCallBack
    /*00000624*/ CMP         R0, #0 
    /*00000628*/ BXEQ        LR 
    /*0000062c*/ LDR         R3, [R0, #248]              @ 0xf8 
    /*00000630*/ CMP         R3, #1 
    /*00000634*/ BEQ         AencGetStream_WaitConditionCallBack_x640
    AencGetStream_WaitConditionCallBack_x638:
    /*00000638*/ MOV         R0, #1 
    /*0000063c*/ BX          LR 
    AencGetStream_WaitConditionCallBack_x640:
    /*00000640*/ LDR         R3, [R0, #168]              @ 0xa8 
    /*00000644*/ LDR         R2, [R0, #180]              @ 0xb4 
    /*00000648*/ LDR         R3, [R3] 
    /*0000064c*/ LDR         R2, [R2] 
    /*00000650*/ CMP         R3, R2 
    /*00000654*/ LDRHI       R1, [R0, #16] 
    /*00000658*/ ADDHI       R2, R2, R1 
    /*0000065c*/ SUB         R3, R2, R3 
    /*00000660*/ CMP         R3, #0 
    /*00000664*/ BNE         AencGetStream_WaitConditionCallBack_x638
    /*00000668*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000066c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000670*/ LDR         R0, [R3] 
    /*00000674*/ ADDS        R0, R0, #0 
    /*00000678*/ MOVNE       R0, #1 
    /*0000067c*/ BX          LR 
FUNC_END AencGetStream_WaitConditionCallBack


FUNC_BEGIN AencGetFrame_WaitConditionCallBack
    /*00000680*/ PUSH        {R4, LR} 
    /*00000684*/ SUBS        R4, R0, #0 
    /*00000688*/ BEQ         AencGetFrame_WaitConditionCallBack_x6d4
    /*0000068c*/ LDR         R0, [R4, #248]              @ 0xf8 
    /*00000690*/ CMP         R0, #1 
    /*00000694*/ BEQ         AencGetFrame_WaitConditionCallBack_x6a0
    /*00000698*/ MOV         R0, #1 
    /*0000069c*/ POP         {R4, PC} 
    AencGetFrame_WaitConditionCallBack_x6a0:
    /*000006a0*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000006a4*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000006a8*/ LDR         R2, [R2] 
    /*000006ac*/ CMP         R2, #0 
    /*000006b0*/ POPNE       {R4, PC} 
    /*000006b4*/ ADD         R0, R4, #216                @ 0xd8 
    /*000006b8*/ BL          0x00000000000006B8          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x2e94] 
    /*000006bc*/ CMP         R0, #0 
    /*000006c0*/ BEQ         AencGetFrame_WaitConditionCallBack_x6d4
    /*000006c4*/ LDR         R0, [R4, #268]              @ 0x10c 
    /*000006c8*/ CLZ         R0, R0 
    /*000006cc*/ LSR         R0, R0, #5 
    /*000006d0*/ POP         {R4, PC} 
    AencGetFrame_WaitConditionCallBack_x6d4:
    /*000006d4*/ MOV         R0, #0 
    /*000006d8*/ POP         {R4, PC} 
FUNC_END AencGetFrame_WaitConditionCallBack


FUNC_BEGIN AENC_Init
    /*000006dc*/ PUSH        {R4, LR} 
    /*000006e0*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000006e4*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000006e8*/ SUB         SP, SP, #32 
    /*000006ec*/ LDR         R3, [R4] 
    /*000006f0*/ CMP         R3, #0 
    /*000006f4*/ BEQ         AENC_Init_x75c
    /*000006f8*/ MOV         R0, #2 
    /*000006fc*/ MOV         R1, #1 
    /*00000700*/ MOV         R2, #23 
    /*00000704*/ STR         R1, [SP, #12] 
    /*00000708*/ STR         R2, [SP, #8] 
    /*0000070c*/ MOVW        R3, #0                      @ AencReceiveFrm [R_ARM_MOVW_ABS_NC: .text+0x1708] 
    /*00000710*/ MOV         R2, #32 
    /*00000714*/ MOVT        R3, #0                      @ AencReceiveFrm [R_ARM_MOVT_ABS: .text+0x1708] 
    /*00000718*/ STR         R2, [SP, #16] 
    /*0000071c*/ STR         R3, [SP, #20] 
    /*00000720*/ MOV         R3, #0 
    /*00000724*/ STR         R3, [SP, #28] 
    /*00000728*/ BL          0x0000000000000728          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000072c*/ LDR         R3, [R0, #32] 
    /*00000730*/ ADD         R0, SP, #8 
    /*00000734*/ BLX         R3 
    /*00000738*/ CMP         R0, #0 
    /*0000073c*/ BNE         AENC_Init_x79c
    AENC_Init_x740:
    /*00000740*/ LDR         R0, [PC, #116]              @ 0x00000000000007bc 
    /*00000744*/ BL          0x0000000000000744          @ osal_wait_init [R_ARM_CALL: +0x0] 
    /*00000748*/ MOV         R3, #0 
    /*0000074c*/ MOV         R0, #0 
    /*00000750*/ STR         R3, [R4] 
    /*00000754*/ ADD         SP, SP, #32 
    /*00000758*/ POP         {R4, PC} 
    AENC_Init_x75c:
    /*0000075c*/ MOVW        R3, #1302                   @ 0x516 
    /*00000760*/ MOV         R1, #23 
    /*00000764*/ STR         R3, [SP] 
    /*00000768*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*0000076c*/ LDR         R3, [PC, #76]               @ 0x00000000000007c0 
    /*00000770*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00000774*/ MOV         R0, #4 
    /*00000778*/ BL          0x0000000000000778          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000077c*/ MOV         R1, #23 
    /*00000780*/ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x384] 
    /*00000784*/ MOV         R0, #4 
    /*00000788*/ MOVT        R2, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x384] 
    /*0000078c*/ BL          0x000000000000078C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000790*/ MOV         R0, #0 
    /*00000794*/ ADD         SP, SP, #32 
    /*00000798*/ POP         {R4, PC} 
    AENC_Init_x79c:
    /*0000079c*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*000007a0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*000007a4*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*000007a8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*000007ac*/ MOVW        R2, #1313                   @ 0x521 
    /*000007b0*/ LDR         R1, [PC, #8]                @ 0x00000000000007c0 
    /*000007b4*/ BL          0x00000000000007B4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000007b8*/ B           AENC_Init_x740
    /*000007bc*/ ANDEQ       R2, R0, R4, LSL #24 
    /*000007c0*/ ANDEQ       R0, R0, R8, LSL R0 
FUNC_END AENC_Init


FUNC_BEGIN aenc_freeze
    /*000007c4*/ MOV         R0, #0 
    /*000007c8*/ BX          LR 
FUNC_END aenc_freeze


FUNC_BEGIN aenc_open
    /*000007cc*/ MOV         R0, #0 
    /*000007d0*/ BX          LR 
FUNC_END aenc_open


FUNC_BEGIN aenc_close
    /*000007d4*/ MOV         R0, #0 
    /*000007d8*/ BX          LR 
FUNC_END aenc_close


FUNC_BEGIN N_VOIE_Init
    /*000007dc*/ MOV         R0, #0 
    /*000007e0*/ BX          LR 
FUNC_END N_VOIE_Init


FUNC_BEGIN N_VOIE_Exit
    /*000007e4*/ BX          LR 
FUNC_END N_VOIE_Exit


FUNC_BEGIN N_VOIE_Open
    /*000007e8*/ MOV         R0, #0 
    /*000007ec*/ BX          LR 
FUNC_END N_VOIE_Open


FUNC_BEGIN N_VOIE_Close
    /*000007f0*/ BX          LR 
FUNC_END N_VOIE_Close


FUNC_BEGIN N_VOIE_Get_Type
    /*000007f4*/ MOV         R0, #0 
    /*000007f8*/ BX          LR 
FUNC_END N_VOIE_Get_Type


FUNC_BEGIN AENC_StrmBufInit
    /*000007fc*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000800*/ SUBS        R5, R1, #0 
    /*00000804*/ SUB         SP, SP, #52                 @ 0x34 
    /*00000808*/ BEQ         AENC_StrmBufInit_xae0
    /*0000080c*/ CMP         R0, #31 
    /*00000810*/ BHI         AENC_StrmBufInit_xa40
    /*00000814*/ MOV         R4, #352                    @ 0x160 
    /*00000818*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000081c*/ MUL         R4, R4, R0 
    /*00000820*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000824*/ MOV         R9, R0 
    /*00000828*/ ADD         SL, R4, #208                @ 0xd0 
    /*0000082c*/ ADD         R3, R6, SL 
    /*00000830*/ STR         R3, [SP, #20] 
    /*00000834*/ MOV         R0, R3 
    /*00000838*/ BL          0x0000000000000838          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*0000083c*/ CMP         R0, #0 
    /*00000840*/ BNE         AENC_StrmBufInit_xa4c
    /*00000844*/ LDR         R7, [R5, #16] 
    /*00000848*/ LDR         R8, [R5, #12] 
    /*0000084c*/ LDR         FP, [R5] 
    /*00000850*/ ADD         R8, R7, R8 
    /*00000854*/ MOV         R0, FP 
    /*00000858*/ MOV         R1, R8 
    /*0000085c*/ BL          0x000000000000085C          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00000860*/ CMP         R0, #0 
    /*00000864*/ BNE         AENC_StrmBufInit_x9d0
    AENC_StrmBufInit_x868:
    /*00000868*/ MOV         R2, R7 
    /*0000086c*/ LDRD        R0, [R5] 
    /*00000870*/ BL          0x0000000000000870          @ CMPI_Remap_Nocache [R_ARM_CALL: +0x0] 
    /*00000874*/ SUBS        R3, R0, #0 
    /*00000878*/ STR         R3, [SP, #32] 
    /*0000087c*/ BEQ         AENC_StrmBufInit_xb40
    /*00000880*/ LDRD        R0, [R5] 
    /*00000884*/ MOV         R3, #0 
    /*00000888*/ MOV         FP, R3 
    /*0000088c*/ MOV         SL, R7 
    /*00000890*/ STRD        SL, [SP, #24] 
    /*00000894*/ ADDS        SL, R7, R0 
    /*00000898*/ ADC         FP, R3, R1 
    /*0000089c*/ LDR         R2, [R5, #12] 
    /*000008a0*/ MOV         R0, SL 
    /*000008a4*/ MOV         R1, FP 
    /*000008a8*/ BL          0x00000000000008A8          @ CMPI_Remap_Nocache [R_ARM_CALL: +0x0] 
    /*000008ac*/ SUBS        R3, R0, #0 
    /*000008b0*/ STR         R3, [SP, #16] 
    /*000008b4*/ BEQ         AENC_StrmBufInit_xaec
    /*000008b8*/ LDR         R2, [R5, #12] 
    /*000008bc*/ ADD         IP, R4, #40                 @ 0x28 
    /*000008c0*/ LDR         R3, [R5, #8] 
    /*000008c4*/ ADD         R1, R4, #72                 @ 0x48 
    /*000008c8*/ ANDS        R0, R2, #3 
    /*000008cc*/ ADD         LR, R4, #136                @ 0x88 
    /*000008d0*/ ADD         FP, R4, #104                @ 0x68 
    /*000008d4*/ ADD         IP, R6, IP 
    /*000008d8*/ MOVNE       R0, #1 
    /*000008dc*/ CMP         R2, R3 
    /*000008e0*/ ORRCC       R0, R0, #1 
    /*000008e4*/ CMP         R0, #0 
    /*000008e8*/ STR         IP, [SP, #36]               @ 0x24 
    /*000008ec*/ ADD         R1, R6, R1 
    /*000008f0*/ ADD         R0, R6, LR 
    /*000008f4*/ ADD         FP, R6, FP 
    /*000008f8*/ BNE         AENC_StrmBufInit_xa58
    /*000008fc*/ CMP         R3, #0 
    /*00000900*/ BEQ         AENC_StrmBufInit_xac8
    /*00000904*/ ANDS        R8, R3, #3 
    /*00000908*/ BNE         AENC_StrmBufInit_xac8
    /*0000090c*/ MOV         IP, #352                    @ 0x160 
    /*00000910*/ LDR         SL, [SP, #16] 
    /*00000914*/ MUL         R9, IP, R9 
    /*00000918*/ ADD         R4, R4, #312                @ 0x138 
    /*0000091c*/ ADD         R4, R6, R4 
    /*00000920*/ ADD         IP, R6, R9 
    /*00000924*/ STR         SL, [IP, #184]              @ 0xb8 
    /*00000928*/ LDR         SL, [SP, #36]               @ 0x24 
    /*0000092c*/ STR         FP, [IP, #180]              @ 0xb4 
    /*00000930*/ STR         R1, [IP, #172]              @ 0xac 
    /*00000934*/ ADD         R1, SP, #44                 @ 0x2c 
    /*00000938*/ STR         SL, [IP, #168]              @ 0xa8 
    /*0000093c*/ STR         R0, [IP, #176]              @ 0xb0 
    /*00000940*/ MOV         R0, R4 
    /*00000944*/ STR         R2, [IP, #16] 
    /*00000948*/ MOV         R2, #0 
    /*0000094c*/ STR         R3, [IP, #20] 
    /*00000950*/ MOV         R3, #0 
    /*00000954*/ STR         R8, [IP, #40]               @ 0x28 
    /*00000958*/ STRD        R2, [IP, #8] 
    /*0000095c*/ STR         R8, [IP, #72]               @ 0x48 
    /*00000960*/ STR         R8, [R6, LR] 
    /*00000964*/ LDR         R3, [IP, #180]              @ 0xb4 
    /*00000968*/ LDRD        SL, [SP, #24] 
    /*0000096c*/ LDR         LR, [SP, #32] 
    /*00000970*/ STR         R8, [R3] 
    /*00000974*/ LDRD        R2, [R5] 
    /*00000978*/ STR         LR, [IP, #196]              @ 0xc4 
    /*0000097c*/ ADDS        SL, SL, R2 
    /*00000980*/ ADC         FP, FP, R3 
    /*00000984*/ MOV         R2, SL 
    /*00000988*/ LDR         SL, [SP, #16] 
    /*0000098c*/ MOV         R3, FP 
    /*00000990*/ STRD        R2, [IP, #8] 
    /*00000994*/ STR         SL, [IP, #200]              @ 0xc8 
    /*00000998*/ LDR         R3, [R5, #8] 
    /*0000099c*/ STR         R7, [IP, #192]              @ 0xc0 
    /*000009a0*/ STR         R3, [IP, #4] 
    /*000009a4*/ BL          0x00000000000009A4          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000009a8*/ MOV         R0, R4 
    /*000009ac*/ ADD         R1, SP, #44                 @ 0x2c 
    /*000009b0*/ MOV         R3, #1 
    /*000009b4*/ STR         R3, [R6, R9] 
    /*000009b8*/ BL          0x00000000000009B8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000009bc*/ LDR         R0, [SP, #20] 
    /*000009c0*/ BL          0x00000000000009C0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000009c4*/ MOV         R0, R8 
    AENC_StrmBufInit_x9c8:
    /*000009c8*/ ADD         SP, SP, #52                 @ 0x34 
    /*000009cc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AENC_StrmBufInit_x9d0:
    /*000009d0*/ MOV         R1, R8 
    /*000009d4*/ MOV         R0, FP 
    /*000009d8*/ BL          0x00000000000009D8          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*000009dc*/ CMP         R0, #0 
    /*000009e0*/ BEQ         AENC_StrmBufInit_x868
    /*000009e4*/ LDR         R0, [SP, #20] 
    /*000009e8*/ BL          0x00000000000009E8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000009ec*/ MOV         R3, #82                     @ 0x52 
    /*000009f0*/ MOV         R1, #23 
    /*000009f4*/ STR         R3, [SP] 
    /*000009f8*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*000009fc*/ LDR         R3, [PC, #392]              @ 0x0000000000000b8c 
    /*00000a00*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00000a04*/ MOV         R0, #3 
    /*00000a08*/ BL          0x0000000000000A08          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000a0c*/ LDR         R3, [R5, #12] 
    /*00000a10*/ MOVW        R2, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3a4] 
    /*00000a14*/ MOV         R1, #23 
    /*00000a18*/ ADD         R7, R7, R3 
    /*00000a1c*/ STR         R7, [SP, #8] 
    /*00000a20*/ LDRD        R4, [R5] 
    /*00000a24*/ MOVT        R2, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3a4] 
    /*00000a28*/ MOV         R0, #3 
    /*00000a2c*/ STRD        R4, [SP] 
    /*00000a30*/ BL          0x0000000000000A30          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000a34*/ MOVW        R0, #32780                  @ 0x800c 
    /*00000a38*/ MOVT        R0, #40983                  @ 0xa017 
    /*00000a3c*/ B           AENC_StrmBufInit_x9c8
    AENC_StrmBufInit_xa40:
    /*00000a40*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000a44*/ MOVT        R0, #40983                  @ 0xa017 
    /*00000a48*/ B           AENC_StrmBufInit_x9c8
    AENC_StrmBufInit_xa4c:
    /*00000a4c*/ MOV         R0, #65024                  @ 0xfe00 
    /*00000a50*/ MOVT        R0, #65535                  @ 0xffff 
    /*00000a54*/ B           AENC_StrmBufInit_x9c8
    AENC_StrmBufInit_xa58:
    /*00000a58*/ MOVW        R2, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x440] 
    /*00000a5c*/ MOV         R1, #18 
    /*00000a60*/ MOVT        R2, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0x440] 
    /*00000a64*/ MOV         R0, #3 
    /*00000a68*/ BL          0x0000000000000A68          @ HI_LOG [R_ARM_CALL: +0x0] 
    AENC_StrmBufInit_xa6c:
    /*00000a6c*/ LDR         R0, [SP, #32] 
    /*00000a70*/ BL          0x0000000000000A70          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    /*00000a74*/ LDR         R0, [SP, #16] 
    /*00000a78*/ BL          0x0000000000000A78          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    /*00000a7c*/ LDR         R0, [SP, #20] 
    /*00000a80*/ BL          0x0000000000000A80          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000a84*/ MOV         R3, #110                    @ 0x6e 
    /*00000a88*/ MOV         R1, #23 
    /*00000a8c*/ STR         R3, [SP] 
    /*00000a90*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*00000a94*/ LDR         R3, [PC, #240]              @ 0x0000000000000b8c 
    /*00000a98*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00000a9c*/ MOV         R0, #3 
    /*00000aa0*/ BL          0x0000000000000AA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000aa4*/ MOV         R3, R9 
    /*00000aa8*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x480] 
    /*00000aac*/ MOV         R1, #23 
    /*00000ab0*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x480] 
    /*00000ab4*/ MOV         R0, #3 
    /*00000ab8*/ BL          0x0000000000000AB8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000abc*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000ac0*/ MOVT        R0, #40983                  @ 0xa017 
    /*00000ac4*/ B           AENC_StrmBufInit_x9c8
    AENC_StrmBufInit_xac8:
    /*00000ac8*/ MOVW        R2, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x460] 
    /*00000acc*/ MOV         R1, #18 
    /*00000ad0*/ MOVT        R2, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0x460] 
    /*00000ad4*/ MOV         R0, #3 
    /*00000ad8*/ BL          0x0000000000000AD8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000adc*/ B           AENC_StrmBufInit_xa6c
    AENC_StrmBufInit_xae0:
    /*00000ae0*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000ae4*/ MOVT        R0, #40983                  @ 0xa017 
    /*00000ae8*/ B           AENC_StrmBufInit_x9c8
    AENC_StrmBufInit_xaec:
    /*00000aec*/ LDR         R0, [SP, #32] 
    /*00000af0*/ BL          0x0000000000000AF0          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    /*00000af4*/ LDR         R0, [SP, #20] 
    /*00000af8*/ BL          0x0000000000000AF8          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000afc*/ MOV         R3, #99                     @ 0x63 
    /*00000b00*/ MOV         R1, #23 
    /*00000b04*/ STR         R3, [SP] 
    /*00000b08*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*00000b0c*/ LDR         R3, [PC, #120]              @ 0x0000000000000b8c 
    /*00000b10*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00000b14*/ MOV         R0, #3 
    /*00000b18*/ BL          0x0000000000000B18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000b1c*/ MOV         R3, R9 
    /*00000b20*/ MOVW        R2, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x408] 
    /*00000b24*/ MOV         R1, #23 
    /*00000b28*/ MOVT        R2, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0x408] 
    /*00000b2c*/ MOV         R0, #3 
    /*00000b30*/ BL          0x0000000000000B30          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000b34*/ MOVW        R0, #32780                  @ 0x800c 
    /*00000b38*/ MOVT        R0, #40983                  @ 0xa017 
    /*00000b3c*/ B           AENC_StrmBufInit_x9c8
    AENC_StrmBufInit_xb40:
    /*00000b40*/ LDR         R0, [SP, #20] 
    /*00000b44*/ BL          0x0000000000000B44          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000b48*/ MOV         R3, #91                     @ 0x5b 
    /*00000b4c*/ MOV         R1, #23 
    /*00000b50*/ STR         R3, [SP] 
    /*00000b54*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*00000b58*/ LDR         R3, [PC, #44]               @ 0x0000000000000b8c 
    /*00000b5c*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00000b60*/ MOV         R0, #3 
    /*00000b64*/ BL          0x0000000000000B64          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000b68*/ MOV         R3, R9 
    /*00000b6c*/ MOVW        R2, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3d4] 
    /*00000b70*/ MOV         R1, #23 
    /*00000b74*/ MOVT        R2, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3d4] 
    /*00000b78*/ MOV         R0, #3 
    /*00000b7c*/ BL          0x0000000000000B7C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000b80*/ MOVW        R0, #32780                  @ 0x800c 
    /*00000b84*/ MOVT        R0, #40983                  @ 0xa017 
    /*00000b88*/ B           AENC_StrmBufInit_x9c8
    /*00000b8c*/ ANDEQ       R0, R0, R4, LSR #32 
FUNC_END AENC_StrmBufInit


FUNC_BEGIN AENC_StrmBufExit
    /*00000b90*/ CMP         R0, #31 
    /*00000b94*/ BHI         AENC_StrmBufExit_xc38
    /*00000b98*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00000b9c*/ MOV         R4, #352                    @ 0x160 
    /*00000ba0*/ MUL         R4, R4, R0 
    /*00000ba4*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000ba8*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000bac*/ SUB         SP, SP, #8 
    /*00000bb0*/ ADD         R6, R4, #208                @ 0xd0 
    /*00000bb4*/ ADD         R6, R5, R6 
    /*00000bb8*/ MOV         R0, R6 
    /*00000bbc*/ BL          0x0000000000000BBC          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000bc0*/ SUBS        R8, R0, #0 
    /*00000bc4*/ BNE         AENC_StrmBufExit_xc44
    /*00000bc8*/ ADD         R7, R4, #312                @ 0x138 
    /*00000bcc*/ ADD         R1, SP, #4 
    /*00000bd0*/ ADD         R7, R5, R7 
    /*00000bd4*/ ADD         SL, R5, R4 
    /*00000bd8*/ MOV         R0, R7 
    /*00000bdc*/ BL          0x0000000000000BDC          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000be0*/ LDR         R9, [R5, R4] 
    /*00000be4*/ ADD         R1, SP, #4 
    /*00000be8*/ MOV         R0, R7 
    /*00000bec*/ CMP         R9, #0 
    /*00000bf0*/ BEQ         AENC_StrmBufExit_xc20
    /*00000bf4*/ STR         R8, [R5, R4] 
    /*00000bf8*/ BL          0x0000000000000BF8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000bfc*/ LDR         R0, [SL, #196]              @ 0xc4 
    /*00000c00*/ BL          0x0000000000000C00          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    /*00000c04*/ LDR         R0, [SL, #200]              @ 0xc8 
    /*00000c08*/ BL          0x0000000000000C08          @ CMPI_Unmap [R_ARM_CALL: +0x0] 
    /*00000c0c*/ MOV         R0, R6 
    /*00000c10*/ BL          0x0000000000000C10          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000c14*/ MOV         R0, R8 
    AENC_StrmBufExit_xc18:
    /*00000c18*/ ADD         SP, SP, #8 
    /*00000c1c*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AENC_StrmBufExit_xc20:
    /*00000c20*/ BL          0x0000000000000C20          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000c24*/ MOV         R0, R6 
    /*00000c28*/ BL          0x0000000000000C28          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000c2c*/ MOV         R0, R9 
    /*00000c30*/ ADD         SP, SP, #8 
    /*00000c34*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AENC_StrmBufExit_xc38:
    /*00000c38*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000c3c*/ MOVT        R0, #40983                  @ 0xa017 
    /*00000c40*/ BX          LR 
    AENC_StrmBufExit_xc44:
    /*00000c44*/ MOV         R0, #65024                  @ 0xfe00 
    /*00000c48*/ MOVT        R0, #65535                  @ 0xffff 
    /*00000c4c*/ B           AENC_StrmBufExit_xc18
FUNC_END AENC_StrmBufExit


FUNC_BEGIN AENC_GetStream
    /*00000c50*/ CMP         R1, #0 
    /*00000c54*/ BEQ         AENC_GetStream_x11fc
    /*00000c58*/ CMP         R0, #31 
    /*00000c5c*/ BHI         AENC_GetStream_x11d0
    /*00000c60*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000c64*/ MOV         FP, #352                    @ 0x160 
    /*00000c68*/ MOV         R9, R2 
    /*00000c6c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000c70*/ MUL         R2, FP, R0 
    /*00000c74*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000c78*/ SUB         SP, SP, #44                 @ 0x2c 
    /*00000c7c*/ ADD         R4, R3, R2 
    /*00000c80*/ STR         R3, [SP, #12] 
    /*00000c84*/ STR         R0, [SP, #24] 
    /*00000c88*/ LDR         R3, [R4, #168]              @ 0xa8 
    /*00000c8c*/ STR         R1, [SP, #28] 
    /*00000c90*/ CMP         R3, #0 
    /*00000c94*/ STR         R2, [SP, #16] 
    /*00000c98*/ BEQ         AENC_GetStream_x1190
    /*00000c9c*/ LDR         R3, [R4, #172]              @ 0xac 
    /*00000ca0*/ CMP         R3, #0 
    /*00000ca4*/ BEQ         AENC_GetStream_x1190
    /*00000ca8*/ LDR         R3, [R4, #176]              @ 0xb0 
    /*00000cac*/ CMP         R3, #0 
    /*00000cb0*/ BEQ         AENC_GetStream_x1190
    /*00000cb4*/ LDR         R3, [R4, #180]              @ 0xb4 
    /*00000cb8*/ CMP         R3, #0 
    /*00000cbc*/ BEQ         AENC_GetStream_x1190
    /*00000cc0*/ LDR         R3, [R4, #184]              @ 0xb8 
    /*00000cc4*/ CMP         R3, #0 
    /*00000cc8*/ BEQ         AENC_GetStream_x1190
    /*00000ccc*/ LDR         R3, [SP, #16] 
    /*00000cd0*/ ADD         SL, R3, #208                @ 0xd0 
    /*00000cd4*/ LDR         R3, [SP, #12] 
    /*00000cd8*/ ADD         SL, R3, SL 
    /*00000cdc*/ MOV         R0, SL 
    /*00000ce0*/ BL          0x0000000000000CE0          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000ce4*/ CMP         R0, #0 
    /*00000ce8*/ BNE         AENC_GetStream_xed8
    /*00000cec*/ LDR         R5, [SP, #16] 
    /*00000cf0*/ ADD         R1, SP, #36                 @ 0x24 
    /*00000cf4*/ LDR         R6, [SP, #12] 
    /*00000cf8*/ ADD         R3, R5, #312                @ 0x138 
    /*00000cfc*/ ADD         FP, R6, R3 
    /*00000d00*/ MOV         R0, FP 
    /*00000d04*/ BL          0x0000000000000D04          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000d08*/ LDR         R3, [R4, #248]              @ 0xf8 
    /*00000d0c*/ CMP         R3, #0 
    /*00000d10*/ BEQ         AENC_GetStream_x1138
    /*00000d14*/ LDR         R3, [R6, R5] 
    /*00000d18*/ CMP         R3, #0 
    /*00000d1c*/ BEQ         AENC_GetStream_x1138
    /*00000d20*/ LDR         R3, [SP, #24] 
    /*00000d24*/ MOV         R8, #352                    @ 0x160 
    /*00000d28*/ LDR         R2, [SP, #12] 
    /*00000d2c*/ MOVW        R6, #0                      @ AencGetStream_WaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x624] 
    /*00000d30*/ MOVT        R6, #0                      @ AencGetStream_WaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x624] 
    /*00000d34*/ MLA         R8, R8, R3, R2 
    AENC_GetStream_xd38:
    /*00000d38*/ LDR         R3, [R8, #180]              @ 0xb4 
    /*00000d3c*/ LDR         R1, [R8, #168]              @ 0xa8 
    /*00000d40*/ LDR         R2, [R3] 
    /*00000d44*/ LDR         R5, [R1] 
    /*00000d48*/ CMP         R5, R2 
    /*00000d4c*/ SUBLS       R3, R2, R5 
    /*00000d50*/ LDRHI       R3, [R8, #16] 
    /*00000d54*/ ADDHI       R3, R2, R3 
    /*00000d58*/ SUBHI       R3, R3, R5 
    /*00000d5c*/ CMP         R3, #0 
    /*00000d60*/ BNE         AENC_GetStream_xf24
    /*00000d64*/ ADD         R1, SP, #36                 @ 0x24 
    /*00000d68*/ MOV         R0, FP 
    /*00000d6c*/ BL          0x0000000000000D6C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000d70*/ MOV         R0, SL 
    /*00000d74*/ BL          0x0000000000000D74          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000d78*/ CMP         R9, #0 
    /*00000d7c*/ BEQ         AENC_GetStream_xfd8
    /*00000d80*/ CMN         R9, #1 
    /*00000d84*/ BNE         AENC_GetStream_xe6c
    /*00000d88*/ LDR         R3, [SP, #16] 
    /*00000d8c*/ ADD         R5, R3, #212                @ 0xd4 
    /*00000d90*/ LDR         R3, [SP, #12] 
    /*00000d94*/ ADD         R5, R3, R5 
    /*00000d98*/ B           AENC_GetStream_xda8
    AENC_GetStream_xd9c:
    /*00000d9c*/ BL          0x0000000000000D9C          @ osal_wait_timeout_interruptible [R_ARM_CALL: +0x0] 
    /*00000da0*/ CMP         R0, #0 
    /*00000da4*/ BLT         AENC_GetStream_xee8
    AENC_GetStream_xda8:
    /*00000da8*/ MOV         R0, R4 
    /*00000dac*/ BL          0x0000000000000DAC          @ AencGetStream_WaitConditionCallBack [R_ARM_CALL: .text+0x624] 
    /*00000db0*/ MOV         R3, #100                    @ 0x64 
    /*00000db4*/ MOV         R2, R4 
    /*00000db8*/ MOV         R1, R6 
    /*00000dbc*/ CMP         R0, #0 
    /*00000dc0*/ MOV         R0, R5 
    /*00000dc4*/ BEQ         AENC_GetStream_xd9c
    AENC_GetStream_xdc8:
    /*00000dc8*/ MOV         R0, SL 
    /*00000dcc*/ BL          0x0000000000000DCC          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000dd0*/ CMP         R0, #0 
    /*00000dd4*/ BNE         AENC_GetStream_xed8
    /*00000dd8*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000ddc*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000de0*/ LDR         R3, [R3] 
    /*00000de4*/ CMP         R3, #0 
    /*00000de8*/ BNE         AENC_GetStream_xfe8
    /*00000dec*/ ADD         R1, SP, #36                 @ 0x24 
    /*00000df0*/ MOV         R0, FP 
    /*00000df4*/ BL          0x0000000000000DF4          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000df8*/ LDR         R3, [R8, #248]              @ 0xf8 
    /*00000dfc*/ CMP         R3, #0 
    /*00000e00*/ BEQ         AENC_GetStream_xe10
    /*00000e04*/ LDR         R3, [R8] 
    /*00000e08*/ CMP         R3, #0 
    /*00000e0c*/ BNE         AENC_GetStream_xd38
    AENC_GetStream_xe10:
    /*00000e10*/ ADD         R1, SP, #36                 @ 0x24 
    /*00000e14*/ MOV         R0, FP 
    /*00000e18*/ BL          0x0000000000000E18          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000e1c*/ MOV         R0, SL 
    /*00000e20*/ BL          0x0000000000000E20          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000e24*/ MOVW        R3, #275                    @ 0x113 
    /*00000e28*/ MOV         R1, #23 
    /*00000e2c*/ STR         R3, [SP] 
    /*00000e30*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*00000e34*/ LDR         R3, [PC, #972]              @ 0x0000000000001208 
    /*00000e38*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00000e3c*/ MOV         R0, #3 
    /*00000e40*/ BL          0x0000000000000E40          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000e44*/ LDR         R3, [SP, #24] 
    /*00000e48*/ MOV         R1, #23 
    /*00000e4c*/ MOVW        R2, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x508] 
    /*00000e50*/ MOV         R0, #3 
    /*00000e54*/ MOVT        R2, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0x508] 
    /*00000e58*/ BL          0x0000000000000E58          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000e5c*/ MOVW        R0, #32773                  @ 0x8005 
    /*00000e60*/ MOVT        R0, #40983                  @ 0xa017 
    /*00000e64*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00000e68*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AENC_GetStream_xe6c:
    /*00000e6c*/ MOV         R0, R4 
    /*00000e70*/ MOV         R5, R9 
    /*00000e74*/ BL          0x0000000000000E74          @ AencGetStream_WaitConditionCallBack [R_ARM_CALL: .text+0x624] 
    /*00000e78*/ LDR         R3, [SP, #16] 
    /*00000e7c*/ ADD         R7, R3, #212                @ 0xd4 
    /*00000e80*/ LDR         R3, [SP, #12] 
    /*00000e84*/ ADD         R7, R3, R7 
    /*00000e88*/ B           AENC_GetStream_xea0
    AENC_GetStream_xe8c:
    /*00000e8c*/ BL          0x0000000000000E8C          @ osal_wait_timeout_interruptible [R_ARM_CALL: +0x0] 
    /*00000e90*/ ADD         R3, R0, #512                @ 0x200 
    /*00000e94*/ MOV         R5, R0 
    /*00000e98*/ BICS        R3, R3, #512                @ 0x200 
    /*00000e9c*/ BEQ         AENC_GetStream_xecc
    AENC_GetStream_xea0:
    /*00000ea0*/ MOV         R0, R4 
    /*00000ea4*/ BL          0x0000000000000EA4          @ AencGetStream_WaitConditionCallBack [R_ARM_CALL: .text+0x624] 
    /*00000ea8*/ MOV         R3, R5 
    /*00000eac*/ MOV         R2, R4 
    /*00000eb0*/ MOV         R1, R6 
    /*00000eb4*/ CMP         R0, #0 
    /*00000eb8*/ MOV         R0, R7 
    /*00000ebc*/ BEQ         AENC_GetStream_xe8c
    /*00000ec0*/ MOV         R0, R5 
    /*00000ec4*/ BL          0x0000000000000EC4          @ osal_msecs_to_jiffies [R_ARM_CALL: +0x0] 
    /*00000ec8*/ MOV         R5, R0 
    AENC_GetStream_xecc:
    /*00000ecc*/ CMP         R5, #0 
    /*00000ed0*/ BEQ         AENC_GetStream_x1000
    /*00000ed4*/ BGE         AENC_GetStream_xdc8
    AENC_GetStream_xed8:
    /*00000ed8*/ MOV         R0, #65024                  @ 0xfe00 
    /*00000edc*/ MOVT        R0, #65535                  @ 0xffff 
    AENC_GetStream_xee0:
    /*00000ee0*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00000ee4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AENC_GetStream_xee8:
    /*00000ee8*/ MOV         R3, #238                    @ 0xee 
    /*00000eec*/ MOV         R1, #23 
    /*00000ef0*/ STR         R3, [SP] 
    /*00000ef4*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*00000ef8*/ LDR         R3, [PC, #776]              @ 0x0000000000001208 
    /*00000efc*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00000f00*/ MOV         R0, #4 
    /*00000f04*/ BL          0x0000000000000F04          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000f08*/ LDR         R3, [SP, #24] 
    /*00000f0c*/ MOVW        R2, #0                      @ .LC61 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x574] 
    /*00000f10*/ MOV         R1, #23 
    /*00000f14*/ MOVT        R2, #0                      @ .LC61 [R_ARM_MOVT_ABS: .rodata.str1.4+0x574] 
    /*00000f18*/ MOV         R0, #4 
    /*00000f1c*/ BL          0x0000000000000F1C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000f20*/ B           AENC_GetStream_xed8
    AENC_GetStream_xf24:
    /*00000f24*/ LDR         R0, [SP, #24] 
    /*00000f28*/ MOV         R3, #352                    @ 0x160 
    /*00000f2c*/ LDR         IP, [SP, #12] 
    /*00000f30*/ CMP         R5, R2 
    /*00000f34*/ MLA         R3, R3, R0, IP 
    /*00000f38*/ LDR         R8, [R3, #184]              @ 0xb8 
    /*00000f3c*/ LDR         IP, [R3, #4] 
    /*00000f40*/ ADD         R4, R8, R5 
    /*00000f44*/ ADD         R0, R5, IP 
    /*00000f48*/ BHI         AENC_GetStream_xfc4
    /*00000f4c*/ CMP         R2, R0 
    /*00000f50*/ BCC         AENC_GetStream_x10f8
    /*00000f54*/ LDR         R3, [R3, #16] 
    /*00000f58*/ CMP         R0, R3 
    /*00000f5c*/ BCC         AENC_GetStream_xfd0
    AENC_GetStream_xf60:
    /*00000f60*/ SUB         R3, R3, R5 
    /*00000f64*/ SUB         R0, IP, R3 
    /*00000f68*/ MOV         IP, R3 
    /*00000f6c*/ MOV         R9, R0 
    AENC_GetStream_xf70:
    /*00000f70*/ LDR         R2, [SP, #24] 
    /*00000f74*/ MOV         R3, #352                    @ 0x160 
    /*00000f78*/ LDR         LR, [SP, #12] 
    /*00000f7c*/ MLA         R3, R3, R2, LR 
    /*00000f80*/ LDRD        R6, [R3, #8] 
    /*00000f84*/ ORRS        R3, R6, R7 
    /*00000f88*/ BNE         AENC_GetStream_x1044
    /*00000f8c*/ CMP         IP, #0 
    /*00000f90*/ MOV         R2, #16 
    /*00000f94*/ MOV         R3, #0 
    /*00000f98*/ STR         R0, [R1] 
    /*00000f9c*/ STRD        R2, [SP, #16] 
    /*00000fa0*/ BEQ         AENC_GetStream_x1118
    AENC_GetStream_xfa4:
    /*00000fa4*/ MOVW        R3, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x540] 
    /*00000fa8*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*00000fac*/ MOVT        R3, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0x540] 
    /*00000fb0*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*00000fb4*/ MOVW        R2, #294                    @ 0x126 
    /*00000fb8*/ LDR         R1, [PC, #584]              @ 0x0000000000001208 
    /*00000fbc*/ BL          0x0000000000000FBC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000fc0*/ B           AENC_GetStream_x1074
    AENC_GetStream_xfc4:
    /*00000fc4*/ LDR         R3, [R3, #16] 
    /*00000fc8*/ CMP         R0, R3 
    /*00000fcc*/ BCS         AENC_GetStream_x10ec
    AENC_GetStream_xfd0:
    /*00000fd0*/ MOV         R9, #0 
    /*00000fd4*/ B           AENC_GetStream_xf70
    AENC_GetStream_xfd8:
    /*00000fd8*/ MOVW        R0, #32782                  @ 0x800e 
    /*00000fdc*/ MOVT        R0, #40983                  @ 0xa017 
    /*00000fe0*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00000fe4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AENC_GetStream_xfe8:
    /*00000fe8*/ MOV         R0, SL 
    /*00000fec*/ BL          0x0000000000000FEC          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000ff0*/ MOVW        R0, #32784                  @ 0x8010 
    /*00000ff4*/ MOVT        R0, #40983                  @ 0xa017 
    /*00000ff8*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00000ffc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AENC_GetStream_x1000:
    /*00001000*/ MOV         R3, #249                    @ 0xf9 
    /*00001004*/ MOV         R1, #23 
    /*00001008*/ STR         R3, [SP] 
    /*0000100c*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*00001010*/ LDR         R3, [PC, #496]              @ 0x0000000000001208 
    /*00001014*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00001018*/ MOV         R0, #3 
    /*0000101c*/ BL          0x000000000000101C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001020*/ LDR         R3, [SP, #24] 
    /*00001024*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4d8] 
    /*00001028*/ MOV         R1, #23 
    /*0000102c*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4d8] 
    /*00001030*/ MOV         R0, #3 
    /*00001034*/ BL          0x0000000000001034          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001038*/ MOVW        R0, #32782                  @ 0x800e 
    /*0000103c*/ MOVT        R0, #40983                  @ 0xa017 
    /*00001040*/ B           AENC_GetStream_xee0
    AENC_GetStream_x1044:
    /*00001044*/ SUBS        R6, R6, R8 
    /*00001048*/ STR         R0, [R1] 
    /*0000104c*/ SBC         R7, R7, #0 
    /*00001050*/ ADDS        R6, R6, R4 
    /*00001054*/ ADC         R7, R7, #0 
    /*00001058*/ ADDS        R2, R6, #16 
    /*0000105c*/ ADC         R3, R7, #0 
    /*00001060*/ CMP         IP, #0 
    /*00001064*/ STRD        R2, [SP, #16] 
    /*00001068*/ BEQ         AENC_GetStream_x1118
    AENC_GetStream_x106c:
    /*0000106c*/ ORRS        R3, R6, R7 
    /*00001070*/ BEQ         AENC_GetStream_xfa4
    AENC_GetStream_x1074:
    /*00001074*/ CMP         R9, #0 
    /*00001078*/ BNE         AENC_GetStream_x11dc
    AENC_GetStream_x107c:
    /*0000107c*/ LDR         R1, [SP, #28] 
    /*00001080*/ MOV         IP, #352                    @ 0x160 
    /*00001084*/ LDRD        R2, [SP, #16] 
    /*00001088*/ LDR         R0, [SP, #24] 
    /*0000108c*/ LDR         LR, [SP, #12] 
    /*00001090*/ STRD        R2, [R1, #8] 
    /*00001094*/ ADD         R3, R4, #16 
    /*00001098*/ STR         R3, [R1] 
    /*0000109c*/ MOV         R2, R1 
    /*000010a0*/ LDR         R3, [R4, #8] 
    /*000010a4*/ ADD         R1, SP, #36                 @ 0x24 
    /*000010a8*/ MLA         IP, IP, R0, LR 
    /*000010ac*/ MOV         LR, R2 
    /*000010b0*/ STR         R3, [R2, #16] 
    /*000010b4*/ MOV         R0, FP 
    /*000010b8*/ LDR         R3, [R4, #12] 
    /*000010bc*/ STR         R3, [R2, #32] 
    /*000010c0*/ LDRD        R2, [R8, R5] 
    /*000010c4*/ STRD        R2, [LR, #24] 
    /*000010c8*/ LDR         R3, [IP, #288]              @ 0x120 
    /*000010cc*/ ADD         R3, R3, #1 
    /*000010d0*/ STR         R3, [IP, #288]              @ 0x120 
    /*000010d4*/ BL          0x00000000000010D4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000010d8*/ MOV         R0, SL 
    /*000010dc*/ BL          0x00000000000010DC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000010e0*/ MOV         R0, #0 
    /*000010e4*/ ADD         SP, SP, #44                 @ 0x2c 
    /*000010e8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AENC_GetStream_x10ec:
    /*000010ec*/ SUB         R0, R0, R3 
    /*000010f0*/ CMP         R2, R0 
    /*000010f4*/ BCS         AENC_GetStream_xf60
    AENC_GetStream_x10f8:
    /*000010f8*/ ADD         R1, SP, #36                 @ 0x24 
    /*000010fc*/ MOV         R0, FP 
    /*00001100*/ BL          0x0000000000001100          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001104*/ MOV         R0, SL 
    /*00001108*/ BL          0x0000000000001108          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000110c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001110*/ MOVT        R0, #40983                  @ 0xa017 
    /*00001114*/ B           AENC_GetStream_xee0
    AENC_GetStream_x1118:
    /*00001118*/ MOVW        R3, #0                      @ .LC58 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x528] 
    /*0000111c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*00001120*/ MOVT        R3, #0                      @ .LC58 [R_ARM_MOVT_ABS: .rodata.str1.4+0x528] 
    /*00001124*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*00001128*/ MOVW        R2, #293                    @ 0x125 
    /*0000112c*/ LDR         R1, [PC, #212]              @ 0x0000000000001208 
    /*00001130*/ BL          0x0000000000001130          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001134*/ B           AENC_GetStream_x106c
    AENC_GetStream_x1138:
    /*00001138*/ ADD         R1, SP, #36                 @ 0x24 
    /*0000113c*/ MOV         R0, FP 
    /*00001140*/ BL          0x0000000000001140          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001144*/ MOV         R0, SL 
    /*00001148*/ BL          0x0000000000001148          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000114c*/ MOV         R3, #220                    @ 0xdc 
    /*00001150*/ MOV         R1, #23 
    /*00001154*/ STR         R3, [SP] 
    /*00001158*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*0000115c*/ LDR         R3, [PC, #164]              @ 0x0000000000001208 
    /*00001160*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00001164*/ MOV         R0, #3 
    /*00001168*/ BL          0x0000000000001168          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000116c*/ LDR         R3, [SP, #24] 
    /*00001170*/ MOVW        R2, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4b4] 
    /*00001174*/ MOV         R1, #23 
    /*00001178*/ MOVT        R2, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4b4] 
    /*0000117c*/ MOV         R0, #3 
    /*00001180*/ BL          0x0000000000001180          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001184*/ MOVW        R0, #32773                  @ 0x8005 
    /*00001188*/ MOVT        R0, #40983                  @ 0xa017 
    /*0000118c*/ B           AENC_GetStream_xee0
    AENC_GetStream_x1190:
    /*00001190*/ MOV         R3, #208                    @ 0xd0 
    /*00001194*/ MOV         R1, #23 
    /*00001198*/ STR         R3, [SP] 
    /*0000119c*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*000011a0*/ LDR         R3, [PC, #96]               @ 0x0000000000001208 
    /*000011a4*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*000011a8*/ MOV         R0, #3 
    /*000011ac*/ BL          0x00000000000011AC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000011b0*/ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4a0] 
    /*000011b4*/ MOV         R1, #23 
    /*000011b8*/ MOVT        R2, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4a0] 
    /*000011bc*/ MOV         R0, #3 
    /*000011c0*/ BL          0x00000000000011C0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000011c4*/ MOVW        R0, #32774                  @ 0x8006 
    /*000011c8*/ MOVT        R0, #40983                  @ 0xa017 
    /*000011cc*/ B           AENC_GetStream_xee0
    AENC_GetStream_x11d0:
    /*000011d0*/ MOVW        R0, #32770                  @ 0x8002 
    /*000011d4*/ MOVT        R0, #40983                  @ 0xa017 
    /*000011d8*/ BX          LR 
    AENC_GetStream_x11dc:
    /*000011dc*/ MOVW        R3, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x55c] 
    /*000011e0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*000011e4*/ MOVT        R3, #0                      @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0x55c] 
    /*000011e8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*000011ec*/ MOVW        R2, #295                    @ 0x127 
    /*000011f0*/ LDR         R1, [PC, #16]               @ 0x0000000000001208 
    /*000011f4*/ BL          0x00000000000011F4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000011f8*/ B           AENC_GetStream_x107c
    AENC_GetStream_x11fc:
    /*000011fc*/ MOVW        R0, #32774                  @ 0x8006 
    /*00001200*/ MOVT        R0, #40983                  @ 0xa017 
    /*00001204*/ BX          LR 
    /*00001208*/ ANDEQ       R0, R0, R8, LSR R0 
FUNC_END AENC_GetStream


FUNC_BEGIN AENC_PutStream
    /*0000120c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00001210*/ SUBS        R7, R1, #0 
    /*00001214*/ SUB         SP, SP, #24 
    /*00001218*/ BEQ         AENC_PutStream_x1514
    /*0000121c*/ CMP         R0, #31 
    /*00001220*/ BHI         AENC_PutStream_x1414
    /*00001224*/ MOV         SL, #352                    @ 0x160 
    /*00001228*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000122c*/ MUL         SL, SL, R0 
    /*00001230*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001234*/ MOV         R4, R0 
    /*00001238*/ ADD         R6, R5, SL 
    /*0000123c*/ LDR         R3, [R6, #168]              @ 0xa8 
    /*00001240*/ CMP         R3, #0 
    /*00001244*/ BEQ         AENC_PutStream_x13d4
    /*00001248*/ LDR         R3, [R6, #172]              @ 0xac 
    /*0000124c*/ CMP         R3, #0 
    /*00001250*/ BEQ         AENC_PutStream_x13d4
    /*00001254*/ LDR         R3, [R6, #176]              @ 0xb0 
    /*00001258*/ CMP         R3, #0 
    /*0000125c*/ BEQ         AENC_PutStream_x13d4
    /*00001260*/ LDR         R3, [R6, #180]              @ 0xb4 
    /*00001264*/ CMP         R3, #0 
    /*00001268*/ BEQ         AENC_PutStream_x13d4
    /*0000126c*/ LDR         R3, [R6, #184]              @ 0xb8 
    /*00001270*/ CMP         R3, #0 
    /*00001274*/ BEQ         AENC_PutStream_x13d4
    /*00001278*/ ADD         R8, SL, #208                @ 0xd0 
    /*0000127c*/ ADD         R8, R5, R8 
    /*00001280*/ MOV         R0, R8 
    /*00001284*/ BL          0x0000000000001284          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001288*/ CMP         R0, #0 
    /*0000128c*/ BNE         AENC_PutStream_x1368
    /*00001290*/ ADD         R9, SL, #312                @ 0x138 
    /*00001294*/ ADD         R1, SP, #20 
    /*00001298*/ ADD         R9, R5, R9 
    /*0000129c*/ MOV         R0, R9 
    /*000012a0*/ BL          0x00000000000012A0          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000012a4*/ LDR         R3, [R5, SL] 
    /*000012a8*/ CMP         R3, #1 
    /*000012ac*/ BNE         AENC_PutStream_x1420
    /*000012b0*/ LDRD        R0, [R7, #8] 
    /*000012b4*/ LDRD        R2, [R6, #8] 
    /*000012b8*/ CMP         R1, R3 
    /*000012bc*/ CMPEQ       R0, R2 
    /*000012c0*/ BCC         AENC_PutStream_x1374
    /*000012c4*/ MOV         R6, #352                    @ 0x160 
    /*000012c8*/ MLA         R6, R6, R4, R5 
    /*000012cc*/ LDR         IP, [R6, #16] 
    /*000012d0*/ ADDS        R2, R2, IP 
    /*000012d4*/ ADC         R3, R3, #0 
    /*000012d8*/ CMP         R1, R3 
    /*000012dc*/ CMPEQ       R0, R2 
    /*000012e0*/ BHI         AENC_PutStream_x1374
    /*000012e4*/ LDR         R3, [R6, #4] 
    /*000012e8*/ LDR         R2, [R7, #16] 
    /*000012ec*/ CMP         R2, R3 
    /*000012f0*/ BHI         AENC_PutStream_x14b0
    /*000012f4*/ LDR         R2, [R6, #168]              @ 0xa8 
    /*000012f8*/ TST         R3, #3 
    /*000012fc*/ LDR         R1, [R6, #172]              @ 0xac 
    /*00001300*/ LDR         R0, [R2] 
    /*00001304*/ LDR         R2, [R1] 
    /*00001308*/ BNE         AENC_PutStream_x1478
    /*0000130c*/ CMP         R0, R2 
    /*00001310*/ ADD         R3, R3, R2 
    /*00001314*/ BCC         AENC_PutStream_x1358
    AENC_PutStream_x1318:
    /*00001318*/ CMP         R0, R3 
    /*0000131c*/ BCC         AENC_PutStream_x1490
    AENC_PutStream_x1320:
    /*00001320*/ MOV         R0, #352                    @ 0x160 
    /*00001324*/ STR         R3, [R1] 
    /*00001328*/ MLA         R4, R0, R4, R5 
    /*0000132c*/ ADD         R1, SP, #20 
    /*00001330*/ MOV         R0, R9 
    /*00001334*/ LDR         R3, [R4, #292]              @ 0x124 
    /*00001338*/ ADD         R3, R3, #1 
    /*0000133c*/ STR         R3, [R4, #292]              @ 0x124 
    /*00001340*/ BL          0x0000000000001340          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001344*/ MOV         R0, R8 
    /*00001348*/ BL          0x0000000000001348          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000134c*/ MOV         R0, #0 
    AENC_PutStream_x1350:
    /*00001350*/ ADD         SP, SP, #24 
    /*00001354*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AENC_PutStream_x1358:
    /*00001358*/ CMP         IP, R3 
    /*0000135c*/ BHI         AENC_PutStream_x1320
    /*00001360*/ SUB         R3, R3, IP 
    /*00001364*/ B           AENC_PutStream_x1318
    AENC_PutStream_x1368:
    /*00001368*/ MOV         R0, #65024                  @ 0xfe00 
    /*0000136c*/ MOVT        R0, #65535                  @ 0xffff 
    /*00001370*/ B           AENC_PutStream_x1350
    AENC_PutStream_x1374:
    /*00001374*/ ADD         R1, SP, #20 
    /*00001378*/ MOV         R0, R9 
    /*0000137c*/ BL          0x000000000000137C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001380*/ MOV         R0, R8 
    /*00001384*/ BL          0x0000000000001384          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001388*/ MOVW        R3, #342                    @ 0x156 
    /*0000138c*/ MOV         R1, #23 
    /*00001390*/ STR         R3, [SP] 
    /*00001394*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*00001398*/ LDR         R3, [PC, #384]              @ 0x0000000000001520 
    /*0000139c*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*000013a0*/ MOV         R0, #3 
    /*000013a4*/ BL          0x00000000000013A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000013a8*/ STR         R4, [SP, #8] 
    /*000013ac*/ LDRD        R4, [R7, #8] 
    /*000013b0*/ MOVW        R2, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x598] 
    /*000013b4*/ MOV         R1, #23 
    /*000013b8*/ MOVT        R2, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0x598] 
    /*000013bc*/ MOV         R0, #3 
    /*000013c0*/ STRD        R4, [SP] 
    /*000013c4*/ BL          0x00000000000013C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000013c8*/ MOVW        R0, #32771                  @ 0x8003 
    /*000013cc*/ MOVT        R0, #40983                  @ 0xa017 
    /*000013d0*/ B           AENC_PutStream_x1350
    AENC_PutStream_x13d4:
    /*000013d4*/ MOVW        R3, #319                    @ 0x13f 
    /*000013d8*/ MOV         R1, #23 
    /*000013dc*/ STR         R3, [SP] 
    /*000013e0*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*000013e4*/ LDR         R3, [PC, #308]              @ 0x0000000000001520 
    /*000013e8*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*000013ec*/ MOV         R0, #3 
    /*000013f0*/ BL          0x00000000000013F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000013f4*/ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4a0] 
    /*000013f8*/ MOV         R1, #23 
    /*000013fc*/ MOVT        R2, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4a0] 
    /*00001400*/ MOV         R0, #3 
    /*00001404*/ BL          0x0000000000001404          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001408*/ MOVW        R0, #32774                  @ 0x8006 
    /*0000140c*/ MOVT        R0, #40983                  @ 0xa017 
    /*00001410*/ B           AENC_PutStream_x1350
    AENC_PutStream_x1414:
    /*00001414*/ MOVW        R0, #32770                  @ 0x8002 
    /*00001418*/ MOVT        R0, #40983                  @ 0xa017 
    /*0000141c*/ B           AENC_PutStream_x1350
    AENC_PutStream_x1420:
    /*00001420*/ ADD         R1, SP, #20 
    /*00001424*/ MOV         R0, R9 
    /*00001428*/ BL          0x0000000000001428          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000142c*/ MOV         R0, R8 
    /*00001430*/ BL          0x0000000000001430          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001434*/ MOVW        R3, #331                    @ 0x14b 
    /*00001438*/ MOV         R1, #23 
    /*0000143c*/ STR         R3, [SP] 
    /*00001440*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*00001444*/ LDR         R3, [PC, #212]              @ 0x0000000000001520 
    /*00001448*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*0000144c*/ MOV         R0, #3 
    /*00001450*/ BL          0x0000000000001450          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001454*/ MOV         R3, R4 
    /*00001458*/ MOVW        R2, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4b4] 
    /*0000145c*/ MOV         R1, #23 
    /*00001460*/ MOVT        R2, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4b4] 
    /*00001464*/ MOV         R0, #3 
    /*00001468*/ BL          0x0000000000001468          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000146c*/ MOVW        R0, #32773                  @ 0x8005 
    /*00001470*/ MOVT        R0, #40983                  @ 0xa017 
    /*00001474*/ B           AENC_PutStream_x1350
    AENC_PutStream_x1478:
    /*00001478*/ MOVW        R2, #0                      @ .LC64 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x61c] 
    /*0000147c*/ LDR         R3, [PC, #160]              @ 0x0000000000001524 
    /*00001480*/ MOVT        R2, #0                      @ .LC64 [R_ARM_MOVT_ABS: .rodata.str1.4+0x61c] 
    /*00001484*/ MOV         R1, #18 
    /*00001488*/ MOV         R0, #3 
    /*0000148c*/ BL          0x000000000000148C          @ HI_LOG [R_ARM_CALL: +0x0] 
    AENC_PutStream_x1490:
    /*00001490*/ ADD         R1, SP, #20 
    /*00001494*/ MOV         R0, R9 
    /*00001498*/ BL          0x0000000000001498          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000149c*/ MOV         R0, R8 
    /*000014a0*/ BL          0x00000000000014A0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000014a4*/ MOVW        R0, #32777                  @ 0x8009 
    /*000014a8*/ MOVT        R0, #40983                  @ 0xa017 
    /*000014ac*/ B           AENC_PutStream_x1350
    AENC_PutStream_x14b0:
    /*000014b0*/ ADD         R1, SP, #20 
    /*000014b4*/ MOV         R0, R9 
    /*000014b8*/ BL          0x00000000000014B8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000014bc*/ MOV         R0, R8 
    /*000014c0*/ BL          0x00000000000014C0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000014c4*/ MOVW        R3, #350                    @ 0x15e 
    /*000014c8*/ MOV         R1, #23 
    /*000014cc*/ STR         R3, [SP] 
    /*000014d0*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*000014d4*/ LDR         R3, [PC, #68]               @ 0x0000000000001520 
    /*000014d8*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*000014dc*/ MOV         R0, #3 
    /*000014e0*/ BL          0x00000000000014E0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000014e4*/ LDR         R0, [R6, #4] 
    /*000014e8*/ LDR         R3, [R7, #16] 
    /*000014ec*/ MOVW        R2, #0                      @ .LC63 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5d4] 
    /*000014f0*/ STR         R4, [SP, #4] 
    /*000014f4*/ MOVT        R2, #0                      @ .LC63 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5d4] 
    /*000014f8*/ STR         R0, [SP] 
    /*000014fc*/ MOV         R1, #23 
    /*00001500*/ MOV         R0, #3 
    /*00001504*/ BL          0x0000000000001504          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001508*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000150c*/ MOVT        R0, #40983                  @ 0xa017 
    /*00001510*/ B           AENC_PutStream_x1350
    AENC_PutStream_x1514:
    /*00001514*/ MOVW        R0, #32774                  @ 0x8006 
    /*00001518*/ MOVT        R0, #40983                  @ 0xa017 
    /*0000151c*/ B           AENC_PutStream_x1350
    /*00001520*/ ANDEQ       R0, R0, R8, ASR #32 
    /*00001524*/ ANDEQ       R0, R0, R8, ASR R0 
FUNC_END AENC_PutStream


FUNC_BEGIN AENC_WakeupGetStream
    /*00001528*/ CMP         R0, #31 
    /*0000152c*/ BHI         AENC_WakeupGetStream_x15d0
    /*00001530*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00001534*/ MOVW        LR, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001538*/ MOVT        LR, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000153c*/ MOV         R2, #352                    @ 0x160 
    /*00001540*/ SUB         SP, SP, #12 
    /*00001544*/ MOV         IP, R0 
    /*00001548*/ MLA         R2, R2, R0, LR 
    /*0000154c*/ LDR         R3, [R2, #168]              @ 0xa8 
    /*00001550*/ CMP         R3, #0 
    /*00001554*/ BEQ         AENC_WakeupGetStream_x15dc
    /*00001558*/ LDR         R1, [R2, #172]              @ 0xac 
    /*0000155c*/ CMP         R1, #0 
    /*00001560*/ BEQ         AENC_WakeupGetStream_x15dc
    /*00001564*/ LDR         R1, [R2, #176]              @ 0xb0 
    /*00001568*/ CMP         R1, #0 
    /*0000156c*/ BEQ         AENC_WakeupGetStream_x15dc
    /*00001570*/ LDR         R1, [R2, #180]              @ 0xb4 
    /*00001574*/ CMP         R1, #0 
    /*00001578*/ BEQ         AENC_WakeupGetStream_x15dc
    /*0000157c*/ LDR         R0, [R2, #184]              @ 0xb8 
    /*00001580*/ CMP         R0, #0 
    /*00001584*/ BEQ         AENC_WakeupGetStream_x15dc
    /*00001588*/ LDR         R3, [R3] 
    /*0000158c*/ LDR         R1, [R1] 
    /*00001590*/ CMP         R3, R1 
    /*00001594*/ LDRHI       R0, [R2, #16] 
    /*00001598*/ ADDHI       R1, R1, R0 
    /*0000159c*/ SUB         R0, R1, R3 
    /*000015a0*/ MOV         R3, #352                    @ 0x160 
    /*000015a4*/ MUL         R3, R3, IP 
    /*000015a8*/ CMP         R0, #0 
    /*000015ac*/ ADD         R2, LR, R3 
    /*000015b0*/ STR         R0, [R2, #204]              @ 0xcc 
    /*000015b4*/ BEQ         AENC_WakeupGetStream_x15c8
    /*000015b8*/ ADD         R3, R3, #212                @ 0xd4 
    /*000015bc*/ ADD         R0, LR, R3 
    /*000015c0*/ BL          0x00000000000015C0          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*000015c4*/ MOV         R0, #0 
    AENC_WakeupGetStream_x15c8:
    /*000015c8*/ ADD         SP, SP, #12 
    /*000015cc*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    AENC_WakeupGetStream_x15d0:
    /*000015d0*/ MOVW        R0, #32770                  @ 0x8002 
    /*000015d4*/ MOVT        R0, #40983                  @ 0xa017 
    /*000015d8*/ BX          LR 
    AENC_WakeupGetStream_x15dc:
    /*000015dc*/ MOVW        R3, #375                    @ 0x177 
    /*000015e0*/ MOV         R1, #23 
    /*000015e4*/ STR         R3, [SP] 
    /*000015e8*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*000015ec*/ LDR         R3, [PC, #40]               @ 0x000000000000161c 
    /*000015f0*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*000015f4*/ MOV         R0, #3 
    /*000015f8*/ BL          0x00000000000015F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000015fc*/ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4a0] 
    /*00001600*/ MOV         R1, #23 
    /*00001604*/ MOVT        R2, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4a0] 
    /*00001608*/ MOV         R0, #3 
    /*0000160c*/ BL          0x000000000000160C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001610*/ MOVW        R0, #32774                  @ 0x8006 
    /*00001614*/ MOVT        R0, #40983                  @ 0xa017 
    /*00001618*/ B           AENC_WakeupGetStream_x15c8
    /*0000161c*/ ANDEQ       R0, R0, IP, RRX 
FUNC_END AENC_WakeupGetStream


FUNC_BEGIN AencSubVb
    /*00001620*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00001624*/ MOVW        R9, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*00001628*/ LDR         R3, [R0, #4] 
    /*0000162c*/ MOVW        R8, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*00001630*/ LDR         FP, [PC, #204]              @ 0x0000000000001704 
    /*00001634*/ SUB         SP, SP, #12 
    /*00001638*/ CMP         R3, #1 
    /*0000163c*/ ADD         R4, R0, #48                 @ 0x30 
    /*00001640*/ ADD         R6, R0, #16 
    /*00001644*/ MOVT        R9, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*00001648*/ MOVNE       R7, #52                     @ 0x34 
    /*0000164c*/ MOVEQ       R7, #56                     @ 0x38 
    /*00001650*/ MOVT        R8, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*00001654*/ ADD         R7, R0, R7 
    /*00001658*/ MOV         R5, R0 
    /*0000165c*/ MOV         SL, R1 
    AencSubVb_x1660:
    /*00001660*/ MOV         R0, #1 
    /*00001664*/ BL          0x0000000000001664          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00001668*/ LDR         IP, [R4], #4 
    /*0000166c*/ LDRD        R2, [R6], #8 
    /*00001670*/ STR         SL, [SP] 
    /*00001674*/ LDR         R1, [R0, #64]               @ 0x40 
    /*00001678*/ MOV         R0, IP 
    /*0000167c*/ BLX         R1 
    /*00001680*/ CMP         R0, #0 
    /*00001684*/ BNE         AencSubVb_x16a8
    /*00001688*/ LDR         R0, [R5, #112]              @ 0x70 
    /*0000168c*/ CMP         R0, #1 
    /*00001690*/ BEQ         AencSubVb_x16c8
    AencSubVb_x1694:
    /*00001694*/ CMP         R4, R7 
    /*00001698*/ BNE         AencSubVb_x1660
    /*0000169c*/ MOV         R0, #0 
    /*000016a0*/ ADD         SP, SP, #12 
    /*000016a4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AencSubVb_x16a8:
    /*000016a8*/ MOV         R0, R8 
    /*000016ac*/ MOV         R3, R9 
    /*000016b0*/ MOVW        R2, #427                    @ 0x1ab 
    /*000016b4*/ MOV         R1, FP 
    /*000016b8*/ BL          0x00000000000016B8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000016bc*/ LDR         R0, [R5, #112]              @ 0x70 
    /*000016c0*/ CMP         R0, #1 
    /*000016c4*/ BNE         AencSubVb_x1694
    AencSubVb_x16c8:
    /*000016c8*/ BL          0x00000000000016C8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000016cc*/ LDR         IP, [R4, #52]               @ 0x34 
    /*000016d0*/ LDRD        R2, [R6, #48]               @ 0x30 
    /*000016d4*/ STR         SL, [SP] 
    /*000016d8*/ LDR         R1, [R0, #64]               @ 0x40 
    /*000016dc*/ MOV         R0, IP 
    /*000016e0*/ BLX         R1 
    /*000016e4*/ CMP         R0, #0 
    /*000016e8*/ BEQ         AencSubVb_x1694
    /*000016ec*/ MOV         R3, R9 
    /*000016f0*/ MOV         R2, #432                    @ 0x1b0 
    /*000016f4*/ MOV         R1, FP 
    /*000016f8*/ MOV         R0, R8 
    /*000016fc*/ BL          0x00000000000016FC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001700*/ B           AencSubVb_x1694
    /*00001704*/ ANDEQ       R0, R0, R4, LSL #1 
FUNC_END AencSubVb


FUNC_BEGIN AencReceiveFrm
    /*00001708*/ CMP         R3, #5 
    /*0000170c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00001710*/ MOV         R7, R1 
    /*00001714*/ SUB         SP, SP, #36                 @ 0x24 
    /*00001718*/ BEQ         AencReceiveFrm_x1738
    /*0000171c*/ MOVW        R3, #0                      @ .LC65 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x64c] 
    /*00001720*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*00001724*/ MOVT        R3, #0                      @ .LC65 [R_ARM_MOVT_ABS: .rodata.str1.4+0x64c] 
    /*00001728*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*0000172c*/ MOVW        R2, #638                    @ 0x27e 
    /*00001730*/ LDR         R1, [PC, #808]              @ 0x0000000000001a60 
    /*00001734*/ BL          0x0000000000001734          @ osal_panic [R_ARM_CALL: +0x0] 
    AencReceiveFrm_x1738:
    /*00001738*/ LDR         R3, [SP, #72]               @ 0x48 
    /*0000173c*/ CMP         R3, #0 
    /*00001740*/ BEQ         AencReceiveFrm_x1958
    AencReceiveFrm_x1744:
    /*00001744*/ CMP         R7, #31 
    /*00001748*/ BHI         AencReceiveFrm_x1978
    /*0000174c*/ MOV         R3, #352                    @ 0x160 
    /*00001750*/ MOVW        R8, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001754*/ MUL         R3, R3, R7 
    /*00001758*/ MOVT        R8, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000175c*/ ADD         R2, R8, R3 
    /*00001760*/ LDR         R1, [R2, #248]              @ 0xf8 
    /*00001764*/ CMP         R1, #1 
    /*00001768*/ BNE         AencReceiveFrm_x1984
    /*0000176c*/ LDR         R3, [R2, #268]              @ 0x10c 
    /*00001770*/ CMP         R3, #0 
    /*00001774*/ BEQ         AencReceiveFrm_x17a4
    /*00001778*/ LDR         R3, [SP, #72]               @ 0x48 
    /*0000177c*/ LDR         R1, [R3] 
    /*00001780*/ LDR         R3, [SP, #72]               @ 0x48 
    /*00001784*/ CMP         R1, #2 
    /*00001788*/ ADDNE       R1, R1, #1 
    /*0000178c*/ MOVEQ       R1, #4 
    /*00001790*/ LDR         R0, [R3, #44]               @ 0x2c 
    /*00001794*/ BL          0x0000000000001794          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00001798*/ CMP         R0, #480                    @ 0x1e0 
    /*0000179c*/ MOV         R4, R0 
    /*000017a0*/ BHI         AencReceiveFrm_x19f4
    AencReceiveFrm_x17a4:
    /*000017a4*/ MOV         R9, #352                    @ 0x160 
    /*000017a8*/ ADD         R1, SP, #28 
    /*000017ac*/ MUL         R9, R9, R7 
    /*000017b0*/ ADD         FP, R9, #312                @ 0x138 
    /*000017b4*/ ADD         R4, R9, #216                @ 0xd8 
    /*000017b8*/ ADD         FP, R8, FP 
    /*000017bc*/ ADD         R4, R8, R4 
    /*000017c0*/ MOV         R0, FP 
    /*000017c4*/ BL          0x00000000000017C4          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000017c8*/ ADD         R3, R8, R9 
    /*000017cc*/ MOV         R0, R4 
    /*000017d0*/ LDR         R3, [R3, #260]              @ 0x104 
    /*000017d4*/ STR         R3, [SP, #12] 
    /*000017d8*/ BL          0x00000000000017D8          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x2e94] 
    /*000017dc*/ MOV         SL, R0 
    /*000017e0*/ MOV         R0, R4 
    /*000017e4*/ BL          0x00000000000017E4          @ AUDIO_FB_QueryFreeNum [R_ARM_CALL: .text+0x2e84] 
    /*000017e8*/ SUBS        R5, R0, #0 
    /*000017ec*/ BNE         AencReceiveFrm_x1894
    /*000017f0*/ LDR         R3, [SP, #12] 
    /*000017f4*/ CMP         R3, SL 
    /*000017f8*/ BHI         AencReceiveFrm_x185c
    /*000017fc*/ SUBS        SL, SL, R3 
    /*00001800*/ BEQ         AencReceiveFrm_x1850
    /*00001804*/ MOVW        R3, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6d8] 
    /*00001808*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*0000180c*/ MOVT        R3, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6d8] 
    /*00001810*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*00001814*/ STR         R3, [SP, #16] 
    /*00001818*/ STR         R2, [SP, #20] 
    AencReceiveFrm_x181c:
    /*0000181c*/ MOV         R0, R4 
    /*00001820*/ ADD         R5, R5, #1 
    /*00001824*/ BL          0x0000000000001824          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x2d84] 
    /*00001828*/ SUBS        R6, R0, #0 
    /*0000182c*/ BEQ         AencReceiveFrm_x18e0
    AencReceiveFrm_x1830:
    /*00001830*/ MOV         R1, #15 
    /*00001834*/ ADD         R0, R6, #8 
    /*00001838*/ BL          0x0000000000001838          @ AencSubVb [R_ARM_CALL: .text+0x1620] 
    /*0000183c*/ MOV         R1, R6 
    /*00001840*/ MOV         R0, R4 
    /*00001844*/ BL          0x0000000000001844          @ AUDIO_FB_PutFree [R_ARM_CALL: .text+0x2de4] 
    /*00001848*/ CMP         R5, SL 
    /*0000184c*/ BNE         AencReceiveFrm_x181c
    AencReceiveFrm_x1850:
    /*00001850*/ LDR         R3, [SP, #12] 
    /*00001854*/ CMP         R3, #0 
    /*00001858*/ BNE         AencReceiveFrm_x18f8
    AencReceiveFrm_x185c:
    /*0000185c*/ MOV         R3, #352                    @ 0x160 
    /*00001860*/ MLA         R7, R3, R7, R8 
    /*00001864*/ LDR         R3, [R7, #268]              @ 0x10c 
    /*00001868*/ CMP         R3, #0 
    /*0000186c*/ BEQ         AencReceiveFrm_x18d0
    /*00001870*/ LDR         R3, [R7, #272]              @ 0x110 
    /*00001874*/ ADD         R3, R3, #1 
    /*00001878*/ STR         R3, [R7, #272]              @ 0x110 
    AencReceiveFrm_x187c:
    /*0000187c*/ MOV         R0, FP 
    /*00001880*/ ADD         R1, SP, #28 
    /*00001884*/ BL          0x0000000000001884          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001888*/ MOV         R0, #0 
    AencReceiveFrm_x188c:
    /*0000188c*/ ADD         SP, SP, #36                 @ 0x24 
    /*00001890*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AencReceiveFrm_x1894:
    /*00001894*/ MOV         R0, R4 
    /*00001898*/ BL          0x0000000000001898          @ AUDIO_FB_GetFree [R_ARM_CALL: .text+0x2d24] 
    /*0000189c*/ SUBS        R5, R0, #0 
    /*000018a0*/ BEQ         AencReceiveFrm_x19d4
    AencReceiveFrm_x18a4:
    /*000018a4*/ MOV         R2, #128                    @ 0x80 
    /*000018a8*/ LDR         R1, [SP, #72]               @ 0x48 
    /*000018ac*/ ADD         R0, R5, #8 
    /*000018b0*/ BL          0x00000000000018B0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000018b4*/ MOV         R1, R5 
    /*000018b8*/ MOV         R0, R4 
    /*000018bc*/ BL          0x00000000000018BC          @ AUDIO_FB_PutBusy [R_ARM_CALL: .text+0x2e34] 
    /*000018c0*/ LDR         R0, [SP, #72]               @ 0x48 
    /*000018c4*/ MOV         R1, #15 
    /*000018c8*/ BL          0x0000000000000090 
    /*000018cc*/ B           AencReceiveFrm_x185c
    AencReceiveFrm_x18d0:
    /*000018d0*/ ADD         R0, R9, #244                @ 0xf4 
    /*000018d4*/ ADD         R0, R8, R0 
    /*000018d8*/ BL          0x00000000000018D8          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*000018dc*/ B           AencReceiveFrm_x187c
    AencReceiveFrm_x18e0:
    /*000018e0*/ LDR         R3, [SP, #16] 
    /*000018e4*/ MOVW        R2, #689                    @ 0x2b1 
    /*000018e8*/ LDR         R1, [PC, #368]              @ 0x0000000000001a60 
    /*000018ec*/ LDR         R0, [SP, #20] 
    /*000018f0*/ BL          0x00000000000018F0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000018f4*/ B           AencReceiveFrm_x1830
    AencReceiveFrm_x18f8:
    /*000018f8*/ MOV         R0, R4 
    /*000018fc*/ BL          0x00000000000018FC          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x2d84] 
    /*00001900*/ SUBS        R5, R0, #0 
    /*00001904*/ BEQ         AencReceiveFrm_x1a40
    AencReceiveFrm_x1908:
    /*00001908*/ ADD         R6, R5, #8 
    /*0000190c*/ MOV         R1, #15 
    /*00001910*/ MOV         R0, R6 
    /*00001914*/ BL          0x0000000000001914          @ AencSubVb [R_ARM_CALL: .text+0x1620] 
    /*00001918*/ MOV         R2, #128                    @ 0x80 
    /*0000191c*/ LDR         R1, [SP, #72]               @ 0x48 
    /*00001920*/ MOV         R0, R6 
    /*00001924*/ BL          0x0000000000001924          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00001928*/ MOV         R1, R5 
    /*0000192c*/ MOV         R0, R4 
    /*00001930*/ BL          0x0000000000001930          @ AUDIO_FB_PutBusy [R_ARM_CALL: .text+0x2e34] 
    /*00001934*/ LDR         R0, [SP, #72]               @ 0x48 
    /*00001938*/ MOV         R1, #15 
    /*0000193c*/ BL          0x0000000000000090 
    /*00001940*/ MOV         R3, #352                    @ 0x160 
    /*00001944*/ MLA         R3, R3, R7, R8 
    /*00001948*/ LDR         R2, [R3, #308]              @ 0x134 
    /*0000194c*/ ADD         R2, R2, #1 
    /*00001950*/ STR         R2, [R3, #308]              @ 0x134 
    /*00001954*/ B           AencReceiveFrm_x185c
    AencReceiveFrm_x1958:
    /*00001958*/ MOVW        R3, #0                      @ .LC66 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x670] 
    /*0000195c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*00001960*/ MOVT        R3, #0                      @ .LC66 [R_ARM_MOVT_ABS: .rodata.str1.4+0x670] 
    /*00001964*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*00001968*/ MOVW        R2, #639                    @ 0x27f 
    /*0000196c*/ LDR         R1, [PC, #236]              @ 0x0000000000001a60 
    /*00001970*/ BL          0x0000000000001970          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001974*/ B           AencReceiveFrm_x1744
    AencReceiveFrm_x1978:
    /*00001978*/ MOVW        R0, #32770                  @ 0x8002 
    /*0000197c*/ MOVT        R0, #40983                  @ 0xa017 
    /*00001980*/ B           AencReceiveFrm_x188c
    AencReceiveFrm_x1984:
    /*00001984*/ ADD         R3, R3, #244                @ 0xf4 
    /*00001988*/ ADD         R0, R8, R3 
    /*0000198c*/ BL          0x000000000000198C          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00001990*/ MOV         R3, #648                    @ 0x288 
    /*00001994*/ MOV         R1, #23 
    /*00001998*/ STR         R3, [SP] 
    /*0000199c*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*000019a0*/ LDR         R3, [PC, #184]              @ 0x0000000000001a60 
    /*000019a4*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*000019a8*/ MOV         R0, #4 
    /*000019ac*/ BL          0x00000000000019AC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000019b0*/ MOV         R3, R7 
    /*000019b4*/ MOVW        R2, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4b4] 
    /*000019b8*/ MOV         R1, #23 
    /*000019bc*/ MOVT        R2, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4b4] 
    /*000019c0*/ MOV         R0, #4 
    /*000019c4*/ BL          0x00000000000019C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000019c8*/ MOVW        R0, #32784                  @ 0x8010 
    /*000019cc*/ MOVT        R0, #40983                  @ 0xa017 
    /*000019d0*/ B           AencReceiveFrm_x188c
    AencReceiveFrm_x19d4:
    /*000019d4*/ MOVW        R3, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6d8] 
    /*000019d8*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*000019dc*/ MOVT        R3, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6d8] 
    /*000019e0*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*000019e4*/ MOVW        R2, #678                    @ 0x2a6 
    /*000019e8*/ LDR         R1, [PC, #112]              @ 0x0000000000001a60 
    /*000019ec*/ BL          0x00000000000019EC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000019f0*/ B           AencReceiveFrm_x18a4
    AencReceiveFrm_x19f4:
    /*000019f4*/ MOV         R3, #660                    @ 0x294 
    /*000019f8*/ MOV         R1, #23 
    /*000019fc*/ STR         R3, [SP] 
    /*00001a00*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*00001a04*/ LDR         R3, [PC, #84]               @ 0x0000000000001a60 
    /*00001a08*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00001a0c*/ MOV         R0, #3 
    /*00001a10*/ BL          0x0000000000001A10          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001a14*/ MOV         R1, #480                    @ 0x1e0 
    /*00001a18*/ MOV         R3, R4 
    /*00001a1c*/ STR         R1, [SP] 
    /*00001a20*/ MOVW        R2, #0                      @ .LC67 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x680] 
    /*00001a24*/ MOV         R1, #23 
    /*00001a28*/ MOVT        R2, #0                      @ .LC67 [R_ARM_MOVT_ABS: .rodata.str1.4+0x680] 
    /*00001a2c*/ MOV         R0, #3 
    /*00001a30*/ BL          0x0000000000001A30          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001a34*/ MOVW        R0, #32776                  @ 0x8008 
    /*00001a38*/ MOVT        R0, #40983                  @ 0xa017 
    /*00001a3c*/ B           AencReceiveFrm_x188c
    AencReceiveFrm_x1a40:
    /*00001a40*/ MOVW        R3, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6d8] 
    /*00001a44*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*00001a48*/ MOVT        R3, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6d8] 
    /*00001a4c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*00001a50*/ MOVW        R2, #698                    @ 0x2ba 
    /*00001a54*/ LDR         R1, [PC, #4]                @ 0x0000000000001a60 
    /*00001a58*/ BL          0x0000000000001A58          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001a5c*/ B           AencReceiveFrm_x1908
    /*00001a60*/ MULEQ       R0, R0, R0 
FUNC_END AencReceiveFrm


FUNC_BEGIN AENC_CreateChn
    /*00001a64*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00001a68*/ SUBS        R9, R1, #0 
    /*00001a6c*/ SUB         SP, SP, #16 
    /*00001a70*/ BEQ         AENC_CreateChn_x1c30
    /*00001a74*/ CMP         R0, #31 
    /*00001a78*/ BHI         AENC_CreateChn_x1b9c
    /*00001a7c*/ MOV         R5, #352                    @ 0x160 
    /*00001a80*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001a84*/ MUL         R5, R5, R0 
    /*00001a88*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001a8c*/ MOV         R6, R0 
    /*00001a90*/ ADD         R7, R5, #208                @ 0xd0 
    /*00001a94*/ ADD         R7, R4, R7 
    /*00001a98*/ MOV         R0, R7 
    /*00001a9c*/ BL          0x0000000000001A9C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001aa0*/ SUBS        R8, R0, #0 
    /*00001aa4*/ BNE         AENC_CreateChn_x1ba8
    /*00001aa8*/ ADD         SL, R4, R5 
    /*00001aac*/ LDR         R3, [SL, #248]              @ 0xf8 
    /*00001ab0*/ CMP         R3, #1 
    /*00001ab4*/ BEQ         AENC_CreateChn_x1b68
    /*00001ab8*/ ADD         R0, R5, #252                @ 0xfc 
    /*00001abc*/ MOV         R2, #16 
    /*00001ac0*/ ADD         R0, R4, R0 
    /*00001ac4*/ MOV         R1, R9 
    /*00001ac8*/ BL          0x0000000000001AC8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00001acc*/ LDR         R0, [R9, #8] 
    /*00001ad0*/ MOVW        R3, #298                    @ 0x12a 
    /*00001ad4*/ SUB         R2, R0, #2 
    /*00001ad8*/ CMP         R2, R3 
    /*00001adc*/ BHI         AENC_CreateChn_x1bb4
    /*00001ae0*/ ADD         R9, R5, #216                @ 0xd8 
    /*00001ae4*/ ADD         R9, R4, R9 
    /*00001ae8*/ MOV         R1, R9 
    /*00001aec*/ BL          0x0000000000001AEC          @ AUDIO_FB_Init [R_ARM_CALL: .text+0x2c58] 
    /*00001af0*/ SUBS        R8, R0, #0 
    /*00001af4*/ BNE         AENC_CreateChn_x1c1c
    /*00001af8*/ LDR         R3, [SL, #268]              @ 0x10c 
    /*00001afc*/ CMP         R3, #0 
    /*00001b00*/ CMPNE       R3, #68                     @ 0x44 
    /*00001b04*/ BNE         AENC_CreateChn_x1b7c
    AENC_CreateChn_x1b08:
    /*00001b08*/ ADD         R0, R5, #272                @ 0x110 
    /*00001b0c*/ MOV         R2, #32 
    /*00001b10*/ ADD         R0, R4, R0 
    /*00001b14*/ MOV         R1, #0 
    /*00001b18*/ BL          0x0000000000001B18          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00001b1c*/ MOV         R0, #352                    @ 0x160 
    /*00001b20*/ MLA         R6, R0, R6, R4 
    /*00001b24*/ ADD         R5, R5, #312                @ 0x138 
    /*00001b28*/ ADD         R4, R4, R5 
    /*00001b2c*/ MOV         R3, #8 
    /*00001b30*/ MOV         R2, #3 
    /*00001b34*/ ADD         R1, SP, #12 
    /*00001b38*/ MOV         R0, R4 
    /*00001b3c*/ STR         R3, [R6, #296]              @ 0x128 
    /*00001b40*/ MOV         R3, #0 
    /*00001b44*/ STR         R2, [R6, #300]              @ 0x12c 
    /*00001b48*/ STR         R3, [R6, #344]              @ 0x158 
    /*00001b4c*/ STR         R3, [R6, #308]              @ 0x134 
    /*00001b50*/ BL          0x0000000000001B50          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001b54*/ MOV         R3, #1 
    /*00001b58*/ MOV         R0, R4 
    /*00001b5c*/ ADD         R1, SP, #12 
    /*00001b60*/ STR         R3, [R6, #248]              @ 0xf8 
    /*00001b64*/ BL          0x0000000000001B64          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    AENC_CreateChn_x1b68:
    /*00001b68*/ MOV         R0, R7 
    /*00001b6c*/ BL          0x0000000000001B6C          @ osal_up [R_ARM_CALL: +0x0] 
    AENC_CreateChn_x1b70:
    /*00001b70*/ MOV         R0, R8 
    /*00001b74*/ ADD         SP, SP, #16 
    /*00001b78*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AENC_CreateChn_x1b7c:
    /*00001b7c*/ ADD         R1, R5, #316                @ 0x13c 
    /*00001b80*/ MOV         R0, #16 
    /*00001b84*/ ADD         R1, R4, R1 
    /*00001b88*/ BL          0x0000000000001B88          @ AUDIO_FB_Init [R_ARM_CALL: .text+0x2c58] 
    /*00001b8c*/ CMP         R0, #0 
    /*00001b90*/ BNE         AENC_CreateChn_x1c00
    /*00001b94*/ BL          0x0000000000001B94          @ AENC_IsSupportVoie [R_ARM_CALL: .text+0x2ea4] 
    /*00001b98*/ B           AENC_CreateChn_x1b08
    AENC_CreateChn_x1b9c:
    /*00001b9c*/ MOVW        R8, #32770                  @ 0x8002 
    /*00001ba0*/ MOVT        R8, #40983                  @ 0xa017 
    /*00001ba4*/ B           AENC_CreateChn_x1b70
    AENC_CreateChn_x1ba8:
    /*00001ba8*/ MOV         R8, #65024                  @ 0xfe00 
    /*00001bac*/ MOVT        R8, #65535                  @ 0xffff 
    /*00001bb0*/ B           AENC_CreateChn_x1b70
    AENC_CreateChn_x1bb4:
    /*00001bb4*/ MOV         R3, #472                    @ 0x1d8 
    /*00001bb8*/ MOV         R1, #23 
    /*00001bbc*/ STR         R3, [SP] 
    /*00001bc0*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*00001bc4*/ LDR         R3, [PC, #112]              @ 0x0000000000001c3c 
    /*00001bc8*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00001bcc*/ MOV         R0, #3 
    /*00001bd0*/ MOVW        R8, #32771                  @ 0x8003 
    /*00001bd4*/ BL          0x0000000000001BD4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001bd8*/ LDR         R3, [R9, #8] 
    /*00001bdc*/ MOV         R1, #23 
    /*00001be0*/ MOVW        R2, #0                      @ .LC69 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6e8] 
    /*00001be4*/ MOV         R0, #3 
    /*00001be8*/ MOVT        R2, #0                      @ .LC69 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6e8] 
    /*00001bec*/ BL          0x0000000000001BEC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001bf0*/ MOV         R0, R7 
    /*00001bf4*/ BL          0x0000000000001BF4          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001bf8*/ MOVT        R8, #40983                  @ 0xa017 
    /*00001bfc*/ B           AENC_CreateChn_x1b70
    AENC_CreateChn_x1c00:
    /*00001c00*/ MOV         R0, R9 
    /*00001c04*/ MOVW        R8, #32780                  @ 0x800c 
    /*00001c08*/ BL          0x0000000000001C08          @ AUDIO_FB_Release [R_ARM_CALL: .text+0x2cf0] 
    /*00001c0c*/ MOV         R0, R7 
    /*00001c10*/ BL          0x0000000000001C10          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001c14*/ MOVT        R8, #40983                  @ 0xa017 
    /*00001c18*/ B           AENC_CreateChn_x1b70
    AENC_CreateChn_x1c1c:
    /*00001c1c*/ MOV         R0, R7 
    /*00001c20*/ MOVW        R8, #32780                  @ 0x800c 
    /*00001c24*/ BL          0x0000000000001C24          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001c28*/ MOVT        R8, #40983                  @ 0xa017 
    /*00001c2c*/ B           AENC_CreateChn_x1b70
    AENC_CreateChn_x1c30:
    /*00001c30*/ MOVW        R8, #32774                  @ 0x8006 
    /*00001c34*/ MOVT        R8, #40983                  @ 0xa017 
    /*00001c38*/ B           AENC_CreateChn_x1b70
    /*00001c3c*/ ANDEQ       R0, R0, R0, LSR #1 
FUNC_END AENC_CreateChn


FUNC_BEGIN AENC_DestroyChn
    /*00001c40*/ CMP         R0, #31 
    /*00001c44*/ BHI         AENC_DestroyChn_x1e68
    /*00001c48*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00001c4c*/ MOV         R5, #352                    @ 0x160 
    /*00001c50*/ MUL         R5, R5, R0 
    /*00001c54*/ MOVW        R7, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001c58*/ MOVT        R7, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001c5c*/ SUB         SP, SP, #28 
    /*00001c60*/ MOV         FP, R0 
    /*00001c64*/ ADD         R8, R5, #208                @ 0xd0 
    /*00001c68*/ ADD         R8, R7, R8 
    /*00001c6c*/ MOV         R0, R8 
    /*00001c70*/ BL          0x0000000000001C70          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001c74*/ SUBS        R4, R0, #0 
    /*00001c78*/ BNE         AENC_DestroyChn_x1e9c
    /*00001c7c*/ ADD         R6, R7, R5 
    /*00001c80*/ LDR         R3, [R6, #248]              @ 0xf8 
    /*00001c84*/ CMP         R3, #1 
    /*00001c88*/ BEQ         AENC_DestroyChn_x1ca0
    AENC_DestroyChn_x1c8c:
    /*00001c8c*/ MOV         R0, R8 
    /*00001c90*/ BL          0x0000000000001C90          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001c94*/ MOV         R0, #0 
    AENC_DestroyChn_x1c98:
    /*00001c98*/ ADD         SP, SP, #28 
    /*00001c9c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AENC_DestroyChn_x1ca0:
    /*00001ca0*/ ADD         R3, R5, #312                @ 0x138 
    /*00001ca4*/ ADD         R1, SP, #20 
    /*00001ca8*/ ADD         R3, R7, R3 
    /*00001cac*/ STR         R3, [SP] 
    /*00001cb0*/ MOV         R9, R3 
    /*00001cb4*/ MOV         R0, R3 
    /*00001cb8*/ BL          0x0000000000001CB8          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001cbc*/ ADD         R1, SP, #20 
    /*00001cc0*/ MOV         R0, R9 
    /*00001cc4*/ STR         R4, [R6, #248]              @ 0xf8 
    /*00001cc8*/ BL          0x0000000000001CC8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001ccc*/ ADD         R0, R5, #244                @ 0xf4 
    /*00001cd0*/ ADD         R0, R7, R0 
    /*00001cd4*/ BL          0x0000000000001CD4          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00001cd8*/ ADD         R0, R5, #212                @ 0xd4 
    /*00001cdc*/ ADD         R0, R7, R0 
    /*00001ce0*/ BL          0x0000000000001CE0          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00001ce4*/ LDR         R3, [R6, #268]              @ 0x10c 
    /*00001ce8*/ CMP         R3, #0 
    /*00001cec*/ CMPNE       R3, #68                     @ 0x44 
    /*00001cf0*/ BNE         AENC_DestroyChn_x1dac
    AENC_DestroyChn_x1cf4:
    /*00001cf4*/ ADD         R5, R5, #216                @ 0xd8 
    /*00001cf8*/ ADD         R1, SP, #20 
    /*00001cfc*/ ADD         R5, R7, R5 
    /*00001d00*/ LDR         R0, [SP] 
    /*00001d04*/ BL          0x0000000000001D04          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001d08*/ MOV         R0, R5 
    /*00001d0c*/ BL          0x0000000000001D0C          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x2e94] 
    /*00001d10*/ SUBS        R9, R0, #0 
    /*00001d14*/ BEQ         AENC_DestroyChn_x1d60
    /*00001d18*/ MOVW        SL, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*00001d1c*/ MOVT        SL, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*00001d20*/ MOVW        R3, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6d8] 
    /*00001d24*/ MOVT        R3, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6d8] 
    /*00001d28*/ STR         R3, [SP, #4] 
    AENC_DestroyChn_x1d2c:
    /*00001d2c*/ MOV         R0, R5 
    /*00001d30*/ ADD         R4, R4, #1 
    /*00001d34*/ BL          0x0000000000001D34          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x2d84] 
    /*00001d38*/ SUBS        R6, R0, #0 
    /*00001d3c*/ BEQ         AENC_DestroyChn_x1d94
    AENC_DestroyChn_x1d40:
    /*00001d40*/ MOV         R1, #15 
    /*00001d44*/ ADD         R0, R6, #8 
    /*00001d48*/ BL          0x0000000000001D48          @ AencSubVb [R_ARM_CALL: .text+0x1620] 
    /*00001d4c*/ MOV         R1, R6 
    /*00001d50*/ MOV         R0, R5 
    /*00001d54*/ BL          0x0000000000001D54          @ AUDIO_FB_PutFree [R_ARM_CALL: .text+0x2de4] 
    /*00001d58*/ CMP         R4, R9 
    /*00001d5c*/ BNE         AENC_DestroyChn_x1d2c
    AENC_DestroyChn_x1d60:
    /*00001d60*/ MOV         R0, #352                    @ 0x160 
    /*00001d64*/ MLA         R7, R0, FP, R7 
    /*00001d68*/ LDR         R4, [R7, #216]              @ 0xd8 
    /*00001d6c*/ CMP         R4, #0 
    /*00001d70*/ BEQ         AENC_DestroyChn_x1e8c
    /*00001d74*/ LDR         R0, [SP] 
    /*00001d78*/ MOV         R3, #0 
    /*00001d7c*/ ADD         R1, SP, #20 
    /*00001d80*/ STR         R3, [R7, #216]              @ 0xd8 
    /*00001d84*/ BL          0x0000000000001D84          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001d88*/ MOV         R0, R4 
    /*00001d8c*/ BL          0x0000000000001D8C          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*00001d90*/ B           AENC_DestroyChn_x1c8c
    AENC_DestroyChn_x1d94:
    /*00001d94*/ LDR         R3, [SP, #4] 
    /*00001d98*/ MOVW        R2, #599                    @ 0x257 
    /*00001d9c*/ LDR         R1, [PC, #276]              @ 0x0000000000001eb8 
    /*00001da0*/ MOV         R0, SL 
    /*00001da4*/ BL          0x0000000000001DA4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001da8*/ B           AENC_DestroyChn_x1d40
    AENC_DestroyChn_x1dac:
    /*00001dac*/ ADD         SL, R5, #316                @ 0x13c 
    /*00001db0*/ BL          0x0000000000001DB0          @ AENC_IsSupportVoie [R_ARM_CALL: .text+0x2ea4] 
    /*00001db4*/ ADD         SL, R7, SL 
    /*00001db8*/ ADD         R1, SP, #20 
    /*00001dbc*/ LDR         R0, [SP] 
    /*00001dc0*/ BL          0x0000000000001DC0          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001dc4*/ MOV         R0, SL 
    /*00001dc8*/ BL          0x0000000000001DC8          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x2e94] 
    /*00001dcc*/ SUBS        R3, R0, #0 
    /*00001dd0*/ MOV         R1, R3 
    /*00001dd4*/ BEQ         AENC_DestroyChn_x1e34
    /*00001dd8*/ MOV         R6, R4 
    /*00001ddc*/ MOV         R9, R1 
    /*00001de0*/ MOVW        R3, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6d8] 
    /*00001de4*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*00001de8*/ MOVT        R3, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6d8] 
    /*00001dec*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*00001df0*/ STR         R3, [SP, #8] 
    /*00001df4*/ STR         R2, [SP, #12] 
    /*00001df8*/ STR         R8, [SP, #4] 
    AENC_DestroyChn_x1dfc:
    /*00001dfc*/ MOV         R0, SL 
    /*00001e00*/ ADD         R6, R6, #1 
    /*00001e04*/ BL          0x0000000000001E04          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x2d84] 
    /*00001e08*/ SUBS        R8, R0, #0 
    /*00001e0c*/ BEQ         AENC_DestroyChn_x1e74
    AENC_DestroyChn_x1e10:
    /*00001e10*/ MOV         R1, #15 
    /*00001e14*/ ADD         R0, R8, #8 
    /*00001e18*/ BL          0x0000000000001E18          @ AencSubVb [R_ARM_CALL: .text+0x1620] 
    /*00001e1c*/ MOV         R1, R8 
    /*00001e20*/ MOV         R0, SL 
    /*00001e24*/ BL          0x0000000000001E24          @ AUDIO_FB_PutFree [R_ARM_CALL: .text+0x2de4] 
    /*00001e28*/ CMP         R9, R6 
    /*00001e2c*/ BNE         AENC_DestroyChn_x1dfc
    /*00001e30*/ LDR         R8, [SP, #4] 
    AENC_DestroyChn_x1e34:
    /*00001e34*/ MOV         R3, #352                    @ 0x160 
    /*00001e38*/ MLA         R3, R3, FP, R7 
    /*00001e3c*/ LDR         R6, [R3, #316]              @ 0x13c 
    /*00001e40*/ CMP         R6, #0 
    /*00001e44*/ BEQ         AENC_DestroyChn_x1ea8
    /*00001e48*/ MOV         R2, #0 
    /*00001e4c*/ LDR         R0, [SP] 
    /*00001e50*/ ADD         R1, SP, #20 
    /*00001e54*/ STR         R2, [R3, #316]              @ 0x13c 
    /*00001e58*/ BL          0x0000000000001E58          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001e5c*/ MOV         R0, R6 
    /*00001e60*/ BL          0x0000000000001E60          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*00001e64*/ B           AENC_DestroyChn_x1cf4
    AENC_DestroyChn_x1e68:
    /*00001e68*/ MOVW        R0, #32770                  @ 0x8002 
    /*00001e6c*/ MOVT        R0, #40983                  @ 0xa017 
    /*00001e70*/ BX          LR 
    AENC_DestroyChn_x1e74:
    /*00001e74*/ LDR         R3, [SP, #8] 
    /*00001e78*/ MOVW        R2, #571                    @ 0x23b 
    /*00001e7c*/ LDR         R1, [PC, #52]               @ 0x0000000000001eb8 
    /*00001e80*/ LDR         R0, [SP, #12] 
    /*00001e84*/ BL          0x0000000000001E84          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001e88*/ B           AENC_DestroyChn_x1e10
    AENC_DestroyChn_x1e8c:
    /*00001e8c*/ ADD         R1, SP, #20 
    /*00001e90*/ LDR         R0, [SP] 
    /*00001e94*/ BL          0x0000000000001E94          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001e98*/ B           AENC_DestroyChn_x1c8c
    AENC_DestroyChn_x1e9c:
    /*00001e9c*/ MOV         R0, #65024                  @ 0xfe00 
    /*00001ea0*/ MOVT        R0, #65535                  @ 0xffff 
    /*00001ea4*/ B           AENC_DestroyChn_x1c98
    AENC_DestroyChn_x1ea8:
    /*00001ea8*/ ADD         R1, SP, #20 
    /*00001eac*/ LDR         R0, [SP] 
    /*00001eb0*/ BL          0x0000000000001EB0          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001eb4*/ B           AENC_DestroyChn_x1cf4
    /*00001eb8*/ STRHEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
FUNC_END AENC_DestroyChn


FUNC_BEGIN AENC_Exit
    /*00001ebc*/ PUSH        {R4, R5, R6, LR} 
    /*00001ec0*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00001ec4*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00001ec8*/ LDR         R3, [R5] 
    /*00001ecc*/ CMP         R3, #2 
    /*00001ed0*/ POPEQ       {R4, R5, R6, PC} 
    /*00001ed4*/ MOV         R0, #2 
    /*00001ed8*/ MOV         R4, #0 
    /*00001edc*/ BL          0x0000000000001EDC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00001ee0*/ LDR         R3, [R0, #36]               @ 0x24 
    /*00001ee4*/ MOV         R0, #23 
    /*00001ee8*/ BLX         R3 
    AENC_Exit_x1eec:
    /*00001eec*/ MOV         R0, R4 
    /*00001ef0*/ BL          0x0000000000001EF0          @ AENC_StrmBufExit [R_ARM_CALL: .text+0xb90] 
    /*00001ef4*/ MOV         R0, R4 
    /*00001ef8*/ ADD         R4, R4, #1 
    /*00001efc*/ BL          0x0000000000001EFC          @ AENC_DestroyChn [R_ARM_CALL: .text+0x1c40] 
    /*00001f00*/ CMP         R4, #32 
    /*00001f04*/ BNE         AENC_Exit_x1eec
    /*00001f08*/ LDR         R0, [PC, #12]               @ 0x0000000000001f1c 
    /*00001f0c*/ BL          0x0000000000001F0C          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00001f10*/ MOV         R3, #2 
    /*00001f14*/ STR         R3, [R5] 
    /*00001f18*/ POP         {R4, R5, R6, PC} 
    /*00001f1c*/ ANDEQ       R2, R0, R4, LSL #24 
FUNC_END AENC_Exit


FUNC_BEGIN AencGetFrame
    /*00001f20*/ CMP         R1, #0 
    /*00001f24*/ BEQ         AencGetFrame_x216c
    /*00001f28*/ CMP         R0, #31 
    /*00001f2c*/ BHI         AencGetFrame_x2114
    /*00001f30*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00001f34*/ MOV         SL, #352                    @ 0x160 
    /*00001f38*/ MUL         SL, SL, R0 
    /*00001f3c*/ MOVW        R9, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001f40*/ MOVT        R9, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001f44*/ SUB         SP, SP, #28 
    /*00001f48*/ ADD         R7, SL, #208                @ 0xd0 
    /*00001f4c*/ STR         R0, [SP, #8] 
    /*00001f50*/ ADD         R7, R9, R7 
    /*00001f54*/ STR         R1, [SP, #12] 
    /*00001f58*/ ADD         R4, R9, SL 
    /*00001f5c*/ MOV         R0, R7 
    /*00001f60*/ BL          0x0000000000001F60          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001f64*/ CMP         R0, #0 
    /*00001f68*/ BNE         AencGetFrame_x206c
    /*00001f6c*/ ADD         R8, SL, #216                @ 0xd8 
    /*00001f70*/ MOVW        R6, #0                      @ AencGetFrame_WaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x680] 
    /*00001f74*/ ADD         R8, R9, R8 
    /*00001f78*/ MOVT        R6, #0                      @ AencGetFrame_WaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x680] 
    /*00001f7c*/ MOV         FP, R4 
    AencGetFrame_x1f80:
    /*00001f80*/ LDR         R3, [FP, #248]              @ 0xf8 
    /*00001f84*/ CMP         R3, #1 
    /*00001f88*/ BNE         AencGetFrame_x207c
    /*00001f8c*/ MOV         R0, R8 
    /*00001f90*/ BL          0x0000000000001F90          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x2e94] 
    /*00001f94*/ CMP         R0, #0 
    /*00001f98*/ BEQ         AencGetFrame_x1fb8
    /*00001f9c*/ MOV         R0, R8 
    /*00001fa0*/ BL          0x0000000000001FA0          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x2e94] 
    /*00001fa4*/ CMP         R0, #0 
    /*00001fa8*/ BEQ         AencGetFrame_x207c
    /*00001fac*/ LDR         R3, [FP, #268]              @ 0x10c 
    /*00001fb0*/ CMP         R3, #0 
    /*00001fb4*/ BEQ         AencGetFrame_x207c
    AencGetFrame_x1fb8:
    /*00001fb8*/ MOV         R0, R7 
    /*00001fbc*/ ADD         R5, SL, #244                @ 0xf4 
    /*00001fc0*/ BL          0x0000000000001FC0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001fc4*/ ADD         R5, R9, R5 
    /*00001fc8*/ B           AencGetFrame_x1fd8
    AencGetFrame_x1fcc:
    /*00001fcc*/ BL          0x0000000000001FCC          @ osal_wait_timeout_interruptible [R_ARM_CALL: +0x0] 
    /*00001fd0*/ CMP         R0, #0 
    /*00001fd4*/ BLT         AencGetFrame_x2034
    AencGetFrame_x1fd8:
    /*00001fd8*/ MOV         R0, R4 
    /*00001fdc*/ BL          0x0000000000001FDC          @ AencGetFrame_WaitConditionCallBack [R_ARM_CALL: .text+0x680] 
    /*00001fe0*/ MOV         R3, #100                    @ 0x64 
    /*00001fe4*/ MOV         R2, R4 
    /*00001fe8*/ MOV         R1, R6 
    /*00001fec*/ CMP         R0, #0 
    /*00001ff0*/ MOV         R0, R5 
    /*00001ff4*/ BEQ         AencGetFrame_x1fcc
    /*00001ff8*/ MOV         R0, R7 
    /*00001ffc*/ BL          0x0000000000001FFC          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00002000*/ CMP         R0, #0 
    /*00002004*/ BNE         AencGetFrame_x206c
    /*00002008*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000200c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00002010*/ LDR         R3, [R3] 
    /*00002014*/ CMP         R3, #0 
    /*00002018*/ BEQ         AencGetFrame_x1f80
    /*0000201c*/ MOV         R0, R7 
    /*00002020*/ BL          0x0000000000002020          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002024*/ MOVW        R0, #32784                  @ 0x8010 
    /*00002028*/ MOVT        R0, #40983                  @ 0xa017 
    /*0000202c*/ ADD         SP, SP, #28 
    /*00002030*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AencGetFrame_x2034:
    /*00002034*/ MOV         R3, #764                    @ 0x2fc 
    /*00002038*/ MOV         R1, #23 
    /*0000203c*/ STR         R3, [SP] 
    /*00002040*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*00002044*/ LDR         R3, [PC, #384]              @ 0x00000000000021cc 
    /*00002048*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*0000204c*/ MOV         R0, #4 
    /*00002050*/ BL          0x0000000000002050          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002054*/ MOVW        R2, #0                      @ .LC72 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x730] 
    /*00002058*/ LDR         R3, [SP, #8] 
    /*0000205c*/ MOV         R1, #23 
    /*00002060*/ MOVT        R2, #0                      @ .LC72 [R_ARM_MOVT_ABS: .rodata.str1.4+0x730] 
    /*00002064*/ MOV         R0, #4 
    /*00002068*/ BL          0x0000000000002068          @ HI_LOG [R_ARM_CALL: +0x0] 
    AencGetFrame_x206c:
    /*0000206c*/ MOV         R0, #65024                  @ 0xfe00 
    /*00002070*/ MOVT        R0, #65535                  @ 0xffff 
    AencGetFrame_x2074:
    /*00002074*/ ADD         SP, SP, #28 
    /*00002078*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AencGetFrame_x207c:
    /*0000207c*/ LDR         R3, [SP, #8] 
    /*00002080*/ MOV         R4, #352                    @ 0x160 
    /*00002084*/ MLA         R4, R4, R3, R9 
    /*00002088*/ LDR         R5, [R4, #248]              @ 0xf8 
    /*0000208c*/ CMP         R5, #1 
    /*00002090*/ BNE         AencGetFrame_x2120
    /*00002094*/ ADD         SL, SL, #312                @ 0x138 
    /*00002098*/ ADD         R1, SP, #20 
    /*0000209c*/ ADD         R9, R9, SL 
    /*000020a0*/ MOV         R0, R9 
    /*000020a4*/ BL          0x00000000000020A4          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000020a8*/ MOV         R0, R8 
    /*000020ac*/ BL          0x00000000000020AC          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x2d84] 
    /*000020b0*/ SUBS        R6, R0, #0 
    /*000020b4*/ BEQ         AencGetFrame_x2178
    /*000020b8*/ LDR         SL, [SP, #12] 
    /*000020bc*/ MOV         R2, #128                    @ 0x80 
    /*000020c0*/ ADD         R1, R6, #8 
    /*000020c4*/ MOV         R0, SL 
    /*000020c8*/ BL          0x00000000000020C8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000020cc*/ MOV         R1, #12 
    /*000020d0*/ MOV         R0, SL 
    /*000020d4*/ BL          0x0000000000000090 
    /*000020d8*/ MOV         R0, SL 
    /*000020dc*/ MOV         R1, #15 
    /*000020e0*/ BL          0x00000000000020E0          @ AencSubVb [R_ARM_CALL: .text+0x1620] 
    /*000020e4*/ MOV         R1, R6 
    /*000020e8*/ MOV         R0, R8 
    /*000020ec*/ BL          0x00000000000020EC          @ AUDIO_FB_PutFree [R_ARM_CALL: .text+0x2de4] 
    /*000020f0*/ ADD         R1, SP, #20 
    /*000020f4*/ MOV         R0, R9 
    /*000020f8*/ STR         R5, [R4, #344]              @ 0x158 
    /*000020fc*/ BL          0x00000000000020FC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002100*/ MOV         R0, R7 
    /*00002104*/ BL          0x0000000000002104          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002108*/ MOV         R0, #0 
    /*0000210c*/ ADD         SP, SP, #28 
    /*00002110*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AencGetFrame_x2114:
    /*00002114*/ MOVW        R0, #32770                  @ 0x8002 
    /*00002118*/ MOVT        R0, #40983                  @ 0xa017 
    /*0000211c*/ BX          LR 
    AencGetFrame_x2120:
    /*00002120*/ MOV         R0, R7 
    /*00002124*/ BL          0x0000000000002124          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002128*/ MOV         R3, #784                    @ 0x310 
    /*0000212c*/ MOV         R1, #23 
    /*00002130*/ STR         R3, [SP] 
    /*00002134*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*00002138*/ LDR         R3, [PC, #140]              @ 0x00000000000021cc 
    /*0000213c*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00002140*/ MOV         R0, #4 
    /*00002144*/ BL          0x0000000000002144          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002148*/ LDR         R3, [SP, #8] 
    /*0000214c*/ MOVW        R2, #0                      @ .LC70 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x700] 
    /*00002150*/ MOV         R1, #23 
    /*00002154*/ MOVT        R2, #0                      @ .LC70 [R_ARM_MOVT_ABS: .rodata.str1.4+0x700] 
    /*00002158*/ MOV         R0, #4 
    /*0000215c*/ BL          0x000000000000215C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002160*/ MOVW        R0, #32784                  @ 0x8010 
    /*00002164*/ MOVT        R0, #40983                  @ 0xa017 
    /*00002168*/ B           AencGetFrame_x2074
    AencGetFrame_x216c:
    /*0000216c*/ MOVW        R0, #32774                  @ 0x8006 
    /*00002170*/ MOVT        R0, #40983                  @ 0xa017 
    /*00002174*/ BX          LR 
    AencGetFrame_x2178:
    /*00002178*/ ADD         R1, SP, #20 
    /*0000217c*/ MOV         R0, R9 
    /*00002180*/ BL          0x0000000000002180          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002184*/ MOV         R0, R7 
    /*00002188*/ BL          0x0000000000002188          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000218c*/ MOVW        R3, #794                    @ 0x31a 
    /*00002190*/ MOV         R1, #23 
    /*00002194*/ STR         R3, [SP] 
    /*00002198*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*0000219c*/ LDR         R3, [PC, #40]               @ 0x00000000000021cc 
    /*000021a0*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*000021a4*/ MOV         R0, #3 
    /*000021a8*/ BL          0x00000000000021A8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000021ac*/ MOVW        R2, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x71c] 
    /*000021b0*/ MOV         R1, #23 
    /*000021b4*/ MOVT        R2, #0                      @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0x71c] 
    /*000021b8*/ MOV         R0, #3 
    /*000021bc*/ BL          0x00000000000021BC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000021c0*/ MOVW        R0, #32782                  @ 0x800e 
    /*000021c4*/ MOVT        R0, #40983                  @ 0xa017 
    /*000021c8*/ B           AencGetFrame_x2074
    /*000021cc*/ ANDEQ       R0, R0, R0, ASR #1 
FUNC_END AencGetFrame


FUNC_BEGIN AencReleaseFrame
    /*000021d0*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000021d4*/ SUBS        R7, R1, #0 
    /*000021d8*/ SUB         SP, SP, #12 
    /*000021dc*/ BEQ         AencReleaseFrame_x2360
    /*000021e0*/ CMP         R0, #31 
    /*000021e4*/ BHI         AencReleaseFrame_x2348
    /*000021e8*/ MOVW        FP, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000021ec*/ MOVT        FP, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000021f0*/ MOV         SL, #352                    @ 0x160 
    /*000021f4*/ LDR         R3, [R7, #4] 
    /*000021f8*/ MLA         SL, SL, R0, FP 
    /*000021fc*/ CMP         R3, #1 
    /*00002200*/ MOV         R9, R0 
    /*00002204*/ MOVNE       R8, #1 
    /*00002208*/ MOVEQ       R8, #2 
    /*0000220c*/ ADD         SL, SL, #208                @ 0xd0 
    /*00002210*/ MOV         R0, SL 
    /*00002214*/ BL          0x0000000000002214          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00002218*/ SUBS        R4, R0, #0 
    /*0000221c*/ BNE         AencReleaseFrame_x2354
    /*00002220*/ ADD         R6, R7, #48                 @ 0x30 
    /*00002224*/ ADD         R5, R7, #16 
    /*00002228*/ B           AencReleaseFrame_x2238
    AencReleaseFrame_x222c:
    /*0000222c*/ ADD         R4, R4, #1 
    /*00002230*/ CMP         R8, R4 
    /*00002234*/ BEQ         AencReleaseFrame_x22b4
    AencReleaseFrame_x2238:
    /*00002238*/ MOV         R0, #1 
    /*0000223c*/ BL          0x000000000000223C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002240*/ LDRD        R2, [R5], #8 
    /*00002244*/ LDR         R1, [R0, #84]               @ 0x54 
    /*00002248*/ LDR         R0, [R6], #4 
    /*0000224c*/ BLX         R1 
    /*00002250*/ CMP         R0, #1 
    /*00002254*/ BNE         AencReleaseFrame_x22ec
    /*00002258*/ LDR         R3, [R7, #112]              @ 0x70 
    /*0000225c*/ CMP         R3, #1 
    /*00002260*/ BNE         AencReleaseFrame_x222c
    /*00002264*/ BL          0x0000000000002264          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002268*/ LDRD        R2, [R5, #48]               @ 0x30 
    /*0000226c*/ LDR         R1, [R0, #84]               @ 0x54 
    /*00002270*/ LDR         R0, [R6, #52]               @ 0x34 
    /*00002274*/ BLX         R1 
    /*00002278*/ CMP         R0, #1 
    /*0000227c*/ BEQ         AencReleaseFrame_x222c
    /*00002280*/ ADD         R4, R7, R4, LSL #3 
    /*00002284*/ MOVW        R3, #849                    @ 0x351 
    /*00002288*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*0000228c*/ STR         R3, [SP] 
    /*00002290*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00002294*/ LDR         R3, [PC, #208]              @ 0x000000000000236c 
    /*00002298*/ MOV         R1, #23 
    /*0000229c*/ MOV         R0, #3 
    /*000022a0*/ BL          0x00000000000022A0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000022a4*/ MOV         R3, R9 
    /*000022a8*/ MOVW        R2, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74c] 
    /*000022ac*/ LDRD        R4, [R4, #72]               @ 0x48 
    /*000022b0*/ B           AencReleaseFrame_x231c
    AencReleaseFrame_x22b4:
    /*000022b4*/ MOV         R1, #12 
    /*000022b8*/ MOV         R0, R7 
    /*000022bc*/ BL          0x00000000000022BC          @ AencSubVb [R_ARM_CALL: .text+0x1620] 
    /*000022c0*/ MOV         R3, #352                    @ 0x160 
    /*000022c4*/ MLA         R9, R3, R9, FP 
    /*000022c8*/ LDR         R0, [PC, #160]              @ 0x0000000000002370 
    /*000022cc*/ MOV         R4, #0 
    /*000022d0*/ STR         R4, [R9, #344]              @ 0x158 
    /*000022d4*/ BL          0x00000000000022D4          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*000022d8*/ MOV         R0, SL 
    /*000022dc*/ BL          0x00000000000022DC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000022e0*/ MOV         R0, R4 
    AencReleaseFrame_x22e4:
    /*000022e4*/ ADD         SP, SP, #12 
    /*000022e8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AencReleaseFrame_x22ec:
    /*000022ec*/ ADD         R4, R7, R4, LSL #3 
    /*000022f0*/ MOVW        R3, #838                    @ 0x346 
    /*000022f4*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*000022f8*/ STR         R3, [SP] 
    /*000022fc*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00002300*/ LDR         R3, [PC, #100]              @ 0x000000000000236c 
    /*00002304*/ MOV         R1, #23 
    /*00002308*/ MOV         R0, #3 
    /*0000230c*/ BL          0x000000000000230C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002310*/ LDRD        R4, [R4, #16] 
    /*00002314*/ MOV         R3, R9 
    /*00002318*/ MOVW        R2, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74c] 
    AencReleaseFrame_x231c:
    /*0000231c*/ MOVT        R2, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74c] 
    /*00002320*/ MOV         R1, #23 
    /*00002324*/ STRD        R4, [SP] 
    /*00002328*/ MOV         R0, #3 
    /*0000232c*/ BL          0x000000000000232C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002330*/ MOV         R0, SL 
    /*00002334*/ BL          0x0000000000002334          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002338*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000233c*/ MOVT        R0, #40983                  @ 0xa017 
    /*00002340*/ ADD         SP, SP, #12 
    /*00002344*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AencReleaseFrame_x2348:
    /*00002348*/ MOVW        R0, #32770                  @ 0x8002 
    /*0000234c*/ MOVT        R0, #40983                  @ 0xa017 
    /*00002350*/ B           AencReleaseFrame_x22e4
    AencReleaseFrame_x2354:
    /*00002354*/ MOV         R0, #65024                  @ 0xfe00 
    /*00002358*/ MOVT        R0, #65535                  @ 0xffff 
    /*0000235c*/ B           AencReleaseFrame_x22e4
    AencReleaseFrame_x2360:
    /*00002360*/ MOVW        R0, #32774                  @ 0x8006 
    /*00002364*/ MOVT        R0, #40983                  @ 0xa017 
    /*00002368*/ B           AencReleaseFrame_x22e4
    /*0000236c*/ LDRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
    /*00002370*/ ANDEQ       R2, R0, R4, LSL #24 
FUNC_END AencReleaseFrame


FUNC_BEGIN AencSendFrmToVoie
    /*00002374*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00002378*/ SUBS        R8, R1, #0 
    /*0000237c*/ SUB         SP, SP, #20 
    /*00002380*/ BEQ         AencSendFrmToVoie_x25d0
    /*00002384*/ CMP         R0, #31 
    /*00002388*/ BHI         AencSendFrmToVoie_x2514
    /*0000238c*/ LDR         R3, [R8, #4] 
    /*00002390*/ ADD         R4, R8, #48                 @ 0x30 
    /*00002394*/ ADD         R5, R8, #16 
    /*00002398*/ MOV         R9, R0 
    /*0000239c*/ CMP         R3, #1 
    /*000023a0*/ MOVNE       R7, #52                     @ 0x34 
    /*000023a4*/ MOVEQ       R7, #56                     @ 0x38 
    /*000023a8*/ ADD         R7, R8, R7 
    AencSendFrmToVoie_x23ac:
    /*000023ac*/ MOV         R0, #1 
    /*000023b0*/ BL          0x00000000000023B0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000023b4*/ LDRD        R2, [R5], #8 
    /*000023b8*/ LDR         R6, [R0, #84]               @ 0x54 
    /*000023bc*/ LDR         R0, [R4], #4 
    /*000023c0*/ BLX         R6 
    /*000023c4*/ CMP         R0, #1 
    /*000023c8*/ BNE         AencSendFrmToVoie_x24b0
    /*000023cc*/ CMP         R4, R7 
    /*000023d0*/ BNE         AencSendFrmToVoie_x23ac
    /*000023d4*/ MOV         R4, #352                    @ 0x160 
    /*000023d8*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000023dc*/ MUL         R4, R4, R9 
    /*000023e0*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000023e4*/ ADD         R6, R4, #208                @ 0xd0 
    /*000023e8*/ ADD         R6, R5, R6 
    /*000023ec*/ MOV         R0, R6 
    /*000023f0*/ BL          0x00000000000023F0          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000023f4*/ CMP         R0, #0 
    /*000023f8*/ BNE         AencSendFrmToVoie_x2520
    /*000023fc*/ ADD         R3, R5, R4 
    /*00002400*/ LDR         R3, [R3, #248]              @ 0xf8 
    /*00002404*/ CMP         R3, #0 
    /*00002408*/ BEQ         AencSendFrmToVoie_x2584
    /*0000240c*/ ADD         R7, R4, #312                @ 0x138 
    /*00002410*/ ADD         R4, R4, #316                @ 0x13c 
    /*00002414*/ ADD         R7, R5, R7 
    /*00002418*/ ADD         R4, R5, R4 
    /*0000241c*/ ADD         R1, SP, #12 
    /*00002420*/ MOV         R0, R7 
    /*00002424*/ BL          0x0000000000002424          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00002428*/ MOV         R0, R4 
    /*0000242c*/ BL          0x000000000000242C          @ AUDIO_FB_QueryFreeNum [R_ARM_CALL: .text+0x2e84] 
    /*00002430*/ CMP         R0, #0 
    /*00002434*/ BEQ         AencSendFrmToVoie_x252c
    /*00002438*/ MOV         R0, R4 
    /*0000243c*/ BL          0x000000000000243C          @ AUDIO_FB_GetFree [R_ARM_CALL: .text+0x2d24] 
    /*00002440*/ SUBS        SL, R0, #0 
    /*00002444*/ BEQ         AencSendFrmToVoie_x24f4
    AencSendFrmToVoie_x2448:
    /*00002448*/ ADD         FP, SL, #8 
    /*0000244c*/ MOV         R2, #56                     @ 0x38 
    /*00002450*/ MOV         R1, R8 
    /*00002454*/ MOV         R8, #0 
    /*00002458*/ MOV         R0, FP 
    /*0000245c*/ BL          0x000000000000245C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002460*/ MOV         R1, SL 
    /*00002464*/ MOV         R0, R4 
    /*00002468*/ STR         R8, [SL, #120]              @ 0x78 
    /*0000246c*/ BL          0x000000000000246C          @ AUDIO_FB_PutBusy [R_ARM_CALL: .text+0x2e34] 
    /*00002470*/ MOV         R1, #15 
    /*00002474*/ MOV         R0, FP 
    /*00002478*/ BL          0x0000000000000090 
    /*0000247c*/ MOV         R3, #352                    @ 0x160 
    /*00002480*/ MLA         R9, R3, R9, R5 
    /*00002484*/ ADD         R1, SP, #12 
    /*00002488*/ MOV         R0, R7 
    /*0000248c*/ LDR         R3, [R9, #272]              @ 0x110 
    /*00002490*/ ADD         R3, R3, #1 
    /*00002494*/ STR         R3, [R9, #272]              @ 0x110 
    /*00002498*/ BL          0x0000000000002498          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000249c*/ MOV         R0, R6 
    /*000024a0*/ BL          0x00000000000024A0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000024a4*/ MOV         R0, R8 
    AencSendFrmToVoie_x24a8:
    /*000024a8*/ ADD         SP, SP, #20 
    /*000024ac*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AencSendFrmToVoie_x24b0:
    /*000024b0*/ MOVW        R3, #885                    @ 0x375 
    /*000024b4*/ MOV         R1, #23 
    /*000024b8*/ STR         R3, [SP] 
    /*000024bc*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*000024c0*/ LDR         R3, [PC, #276]              @ 0x00000000000025dc 
    /*000024c4*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*000024c8*/ MOV         R0, #3 
    /*000024cc*/ BL          0x00000000000024CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000024d0*/ MOV         R1, #23 
    /*000024d4*/ MOVW        R2, #0                      @ .LC74 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x774] 
    /*000024d8*/ MOV         R0, #3 
    /*000024dc*/ MOVT        R2, #0                      @ .LC74 [R_ARM_MOVT_ABS: .rodata.str1.4+0x774] 
    /*000024e0*/ BL          0x00000000000024E0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000024e4*/ MOVW        R0, #32771                  @ 0x8003 
    /*000024e8*/ MOVT        R0, #40983                  @ 0xa017 
    /*000024ec*/ ADD         SP, SP, #20 
    /*000024f0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AencSendFrmToVoie_x24f4:
    /*000024f4*/ MOVW        R3, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6d8] 
    /*000024f8*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*000024fc*/ MOVT        R3, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6d8] 
    /*00002500*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*00002504*/ MOVW        R2, #909                    @ 0x38d 
    /*00002508*/ LDR         R1, [PC, #204]              @ 0x00000000000025dc 
    /*0000250c*/ BL          0x000000000000250C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002510*/ B           AencSendFrmToVoie_x2448
    AencSendFrmToVoie_x2514:
    /*00002514*/ MOVW        R0, #32770                  @ 0x8002 
    /*00002518*/ MOVT        R0, #40983                  @ 0xa017 
    /*0000251c*/ B           AencSendFrmToVoie_x24a8
    AencSendFrmToVoie_x2520:
    /*00002520*/ MOV         R0, #65024                  @ 0xfe00 
    /*00002524*/ MOVT        R0, #65535                  @ 0xffff 
    /*00002528*/ B           AencSendFrmToVoie_x24a8
    AencSendFrmToVoie_x252c:
    /*0000252c*/ ADD         R1, SP, #12 
    /*00002530*/ MOV         R0, R7 
    /*00002534*/ BL          0x0000000000002534          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002538*/ MOVW        R3, #921                    @ 0x399 
    /*0000253c*/ MOV         R1, #23 
    /*00002540*/ STR         R3, [SP] 
    /*00002544*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*00002548*/ LDR         R3, [PC, #140]              @ 0x00000000000025dc 
    /*0000254c*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00002550*/ MOV         R0, #4 
    /*00002554*/ BL          0x0000000000002554          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002558*/ MOV         R3, R9 
    /*0000255c*/ MOV         R1, #23 
    /*00002560*/ MOVW        R2, #0                      @ .LC76 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b4] 
    /*00002564*/ MOV         R0, #4 
    /*00002568*/ MOVT        R2, #0                      @ .LC76 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b4] 
    /*0000256c*/ BL          0x000000000000256C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002570*/ MOV         R0, R6 
    /*00002574*/ BL          0x0000000000002574          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002578*/ MOVW        R0, #32781                  @ 0x800d 
    /*0000257c*/ MOVT        R0, #40983                  @ 0xa017 
    /*00002580*/ B           AencSendFrmToVoie_x24a8
    AencSendFrmToVoie_x2584:
    /*00002584*/ MOV         R0, R6 
    /*00002588*/ BL          0x0000000000002588          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000258c*/ MOV         R3, #900                    @ 0x384 
    /*00002590*/ MOV         R1, #23 
    /*00002594*/ STR         R3, [SP] 
    /*00002598*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*0000259c*/ LDR         R3, [PC, #56]               @ 0x00000000000025dc 
    /*000025a0*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*000025a4*/ MOV         R0, #3 
    /*000025a8*/ BL          0x00000000000025A8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000025ac*/ MOV         R3, R9 
    /*000025b0*/ MOVW        R2, #0                      @ .LC75 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x79c] 
    /*000025b4*/ MOV         R1, #23 
    /*000025b8*/ MOVT        R2, #0                      @ .LC75 [R_ARM_MOVT_ABS: .rodata.str1.4+0x79c] 
    /*000025bc*/ MOV         R0, #3 
    /*000025c0*/ BL          0x00000000000025C0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000025c4*/ MOVW        R0, #32775                  @ 0x8007 
    /*000025c8*/ MOVT        R0, #40983                  @ 0xa017 
    /*000025cc*/ B           AencSendFrmToVoie_x24a8
    AencSendFrmToVoie_x25d0:
    /*000025d0*/ MOVW        R0, #32774                  @ 0x8006 
    /*000025d4*/ MOVT        R0, #40983                  @ 0xa017 
    /*000025d8*/ B           AencSendFrmToVoie_x24a8
    /*000025dc*/ ANDEQ       R0, R0, R4, ROR #1 
FUNC_END AencSendFrmToVoie


FUNC_BEGIN AencSetSendInfo
    /*000025e0*/ CMP         R1, #0 
    /*000025e4*/ BEQ         AencSetSendInfo_x2644
    /*000025e8*/ CMP         R0, #31 
    /*000025ec*/ BHI         AencSetSendInfo_x2638
    /*000025f0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000025f4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000025f8*/ MOV         IP, #352                    @ 0x160 
    /*000025fc*/ LDR         R2, [R1] 
    /*00002600*/ MLA         R3, IP, R0, R3 
    /*00002604*/ MOV         R0, #0 
    /*00002608*/ STR         R2, [R3, #272]              @ 0x110 
    /*0000260c*/ LDR         R2, [R1, #4] 
    /*00002610*/ STR         R2, [R3, #276]              @ 0x114 
    /*00002614*/ LDR         R2, [R1, #12] 
    /*00002618*/ STR         R2, [R3, #284]              @ 0x11c 
    /*0000261c*/ LDR         R2, [R1, #8] 
    /*00002620*/ STR         R2, [R3, #280]              @ 0x118 
    /*00002624*/ LDR         R2, [R1, #24] 
    /*00002628*/ STR         R2, [R3, #296]              @ 0x128 
    /*0000262c*/ LDR         R2, [R1, #28] 
    /*00002630*/ STR         R2, [R3, #300]              @ 0x12c 
    /*00002634*/ BX          LR 
    AencSetSendInfo_x2638:
    /*00002638*/ MOVW        R0, #32770                  @ 0x8002 
    /*0000263c*/ MOVT        R0, #40983                  @ 0xa017 
    /*00002640*/ BX          LR 
    AencSetSendInfo_x2644:
    /*00002644*/ MOVW        R0, #32774                  @ 0x8006 
    /*00002648*/ MOVT        R0, #40983                  @ 0xa017 
    /*0000264c*/ BX          LR 
FUNC_END AencSetSendInfo


FUNC_BEGIN AencUpdateStreamBufWrTail
    /*00002650*/ CMP         R1, #0 
    /*00002654*/ BEQ         AencUpdateStreamBufWrTail_x273c
    /*00002658*/ CMP         R0, #31 
    /*0000265c*/ BHI         AencUpdateStreamBufWrTail_x26f0
    /*00002660*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002664*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002668*/ MOV         R3, #352                    @ 0x160 
    /*0000266c*/ MLA         R3, R3, R0, R2 
    /*00002670*/ PUSH        {R4, LR} 
    /*00002674*/ SUB         SP, SP, #8 
    /*00002678*/ LDR         IP, [R3, #168]              @ 0xa8 
    /*0000267c*/ CMP         IP, #0 
    /*00002680*/ BEQ         AencUpdateStreamBufWrTail_x26fc
    /*00002684*/ LDR         IP, [R3, #172]              @ 0xac 
    /*00002688*/ CMP         IP, #0 
    /*0000268c*/ BEQ         AencUpdateStreamBufWrTail_x26fc
    /*00002690*/ LDR         IP, [R3, #176]              @ 0xb0 
    /*00002694*/ CMP         IP, #0 
    /*00002698*/ BEQ         AencUpdateStreamBufWrTail_x26fc
    /*0000269c*/ LDR         LR, [R3, #180]              @ 0xb4 
    /*000026a0*/ CMP         LR, #0 
    /*000026a4*/ BEQ         AencUpdateStreamBufWrTail_x26fc
    /*000026a8*/ LDR         IP, [R3, #184]              @ 0xb8 
    /*000026ac*/ CMP         IP, #0 
    /*000026b0*/ BEQ         AencUpdateStreamBufWrTail_x26fc
    /*000026b4*/ LDR         IP, [R1] 
    /*000026b8*/ LDR         R4, [R3, #16] 
    /*000026bc*/ CMP         R4, IP 
    /*000026c0*/ MOVEQ       IP, #0 
    /*000026c4*/ STREQ       IP, [R1] 
    /*000026c8*/ LDREQ       LR, [R3, #180]              @ 0xb4 
    /*000026cc*/ MOV         R3, #352                    @ 0x160 
    /*000026d0*/ MLA         R3, R3, R0, R2 
    /*000026d4*/ MOV         R0, #0 
    /*000026d8*/ STR         IP, [LR] 
    /*000026dc*/ LDR         R2, [R1] 
    /*000026e0*/ LDR         R3, [R3, #176]              @ 0xb0 
    /*000026e4*/ STR         R2, [R3] 
    AencUpdateStreamBufWrTail_x26e8:
    /*000026e8*/ ADD         SP, SP, #8 
    /*000026ec*/ POP         {R4, PC} 
    AencUpdateStreamBufWrTail_x26f0:
    /*000026f0*/ MOVW        R0, #32770                  @ 0x8002 
    /*000026f4*/ MOVT        R0, #40983                  @ 0xa017 
    /*000026f8*/ BX          LR 
    AencUpdateStreamBufWrTail_x26fc:
    /*000026fc*/ MOVW        R3, #962                    @ 0x3c2 
    /*00002700*/ MOV         R1, #23 
    /*00002704*/ STR         R3, [SP] 
    /*00002708*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*0000270c*/ LDR         R3, [PC, #52]               @ 0x0000000000002748 
    /*00002710*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00002714*/ MOV         R0, #3 
    /*00002718*/ BL          0x0000000000002718          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000271c*/ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4a0] 
    /*00002720*/ MOV         R1, #23 
    /*00002724*/ MOVT        R2, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4a0] 
    /*00002728*/ MOV         R0, #3 
    /*0000272c*/ BL          0x000000000000272C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002730*/ MOVW        R0, #32774                  @ 0x8006 
    /*00002734*/ MOVT        R0, #40983                  @ 0xa017 
    /*00002738*/ B           AencUpdateStreamBufWrTail_x26e8
    AencUpdateStreamBufWrTail_x273c:
    /*0000273c*/ MOVW        R0, #32774                  @ 0x8006 
    /*00002740*/ MOVT        R0, #40983                  @ 0xa017 
    /*00002744*/ BX          LR 
    /*00002748*/ STRDEQ      R0, [R0], -R8 
FUNC_END AencUpdateStreamBufWrTail


FUNC_BEGIN AencIoctl
    /*0000274c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00002750*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00002754*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00002758*/ SUB         SP, SP, #12 
    /*0000275c*/ LDR         R4, [R3] 
    /*00002760*/ CMP         R4, #0 
    /*00002764*/ BNE         AencIoctl_x29e8
    /*00002768*/ MOV         R5, R0 
    /*0000276c*/ LDR         R0, [PC, #716]              @ 0x0000000000002a40 
    /*00002770*/ MOV         R7, R2 
    /*00002774*/ MOV         R6, R1 
    /*00002778*/ BL          0x0000000000002778          @ osal_atomic_inc_return [R_ARM_CALL: +0x0] 
    /*0000277c*/ MOVW        R3, #16647                  @ 0x4107 
    /*00002780*/ MOVT        R3, #16408                  @ 0x4018 
    /*00002784*/ LDR         R0, [R7] 
    /*00002788*/ CMP         R5, R3 
    /*0000278c*/ BEQ         AencIoctl_x2990
    /*00002790*/ BLS         AencIoctl_x2828
    /*00002794*/ MOVW        R3, #16643                  @ 0x4103 
    /*00002798*/ MOVT        R3, #16512                  @ 0x4080 
    /*0000279c*/ CMP         R5, R3 
    /*000027a0*/ BEQ         AencIoctl_x2974
    /*000027a4*/ BHI         AencIoctl_x2874
    /*000027a8*/ MOVW        R3, #16646                  @ 0x4106 
    /*000027ac*/ MOVT        R3, #16424                  @ 0x4028 
    /*000027b0*/ CMP         R5, R3 
    /*000027b4*/ BEQ         AencIoctl_x28e8
    /*000027b8*/ MOVW        R3, #16650                  @ 0x410a 
    /*000027bc*/ MOVT        R3, #16440                  @ 0x4038 
    /*000027c0*/ CMP         R5, R3 
    /*000027c4*/ BEQ         AencIoctl_x29b0
    /*000027c8*/ MOVW        R3, #16644                  @ 0x4104 
    /*000027cc*/ MOVT        R3, #16416                  @ 0x4020 
    /*000027d0*/ CMP         R5, R3 
    /*000027d4*/ BEQ         AencIoctl_x28f8
    AencIoctl_x27d8:
    /*000027d8*/ MOVW        R3, #1091                   @ 0x443 
    /*000027dc*/ MOV         R1, #23 
    /*000027e0*/ STR         R3, [SP] 
    /*000027e4*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*000027e8*/ LDR         R3, [PC, #596]              @ 0x0000000000002a44 
    /*000027ec*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*000027f0*/ MOV         R0, #3 
    /*000027f4*/ MOV         R4, #0 
    /*000027f8*/ BL          0x00000000000027F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000027fc*/ UXTB        R2, R5 
    /*00002800*/ UBFX        R3, R5, #8, #8 
    /*00002804*/ STR         R2, [SP, #4] 
    /*00002808*/ STR         R3, [SP] 
    /*0000280c*/ MOVW        R2, #0                      @ .LC78 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7e8] 
    /*00002810*/ MOV         R3, R5 
    /*00002814*/ MOV         R1, #23 
    /*00002818*/ MOVT        R2, #0                      @ .LC78 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7e8] 
    /*0000281c*/ MOV         R0, #3 
    /*00002820*/ BL          0x0000000000002820          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002824*/ B           AencIoctl_x28c4
    AencIoctl_x2828:
    /*00002828*/ MOVW        R3, #16653                  @ 0x410d 
    /*0000282c*/ CMP         R5, R3 
    /*00002830*/ BEQ         AencIoctl_x29c0
    /*00002834*/ BLS         AencIoctl_x2950
    /*00002838*/ MOVW        R3, #16652                  @ 0x410c 
    /*0000283c*/ MOVT        R3, #16388                  @ 0x4004 
    /*00002840*/ CMP         R5, R3 
    /*00002844*/ BEQ         AencIoctl_x28d8
    /*00002848*/ MOV         R3, #16640                  @ 0x4100 
    /*0000284c*/ MOVT        R3, #16400                  @ 0x4010 
    /*00002850*/ CMP         R5, R3 
    /*00002854*/ BEQ         AencIoctl_x29a0
    /*00002858*/ MOVW        R3, #16649                  @ 0x4109 
    /*0000285c*/ MOVT        R3, #16388                  @ 0x4004 
    /*00002860*/ CMP         R5, R3 
    /*00002864*/ BNE         AencIoctl_x27d8
    /*00002868*/ LDR         R3, [R6] 
    /*0000286c*/ STR         R3, [R7] 
    /*00002870*/ B           AencIoctl_x28c4
    AencIoctl_x2874:
    /*00002874*/ MOVW        R3, #16651                  @ 0x410b 
    /*00002878*/ MOVT        R3, #49172                  @ 0xc014 
    /*0000287c*/ CMP         R5, R3 
    /*00002880*/ BEQ         AencIoctl_x28b4
    /*00002884*/ MOVW        R3, #16645                  @ 0x4105 
    /*00002888*/ MOVT        R3, #49200                  @ 0xc030 
    /*0000288c*/ CMP         R5, R3 
    /*00002890*/ BEQ         AencIoctl_x29cc
    /*00002894*/ MOVW        R3, #16642                  @ 0x4102 
    /*00002898*/ MOVT        R3, #32896                  @ 0x8080 
    /*0000289c*/ CMP         R5, R3 
    /*000028a0*/ BNE         AencIoctl_x27d8
    /*000028a4*/ MOV         R1, R6 
    /*000028a8*/ BL          0x00000000000028A8          @ AencGetFrame [R_ARM_CALL: .text+0x1f20] 
    /*000028ac*/ MOV         R4, R0 
    /*000028b0*/ B           AencIoctl_x28c4
    AencIoctl_x28b4:
    /*000028b4*/ CMP         R6, #0 
    /*000028b8*/ BEQ         AencIoctl_x2a28
    /*000028bc*/ STR         R4, [R6, #16] 
    /*000028c0*/ BL          0x00000000000028C0          @ AENC_IsSupportVoie [R_ARM_CALL: .text+0x2ea4] 
    AencIoctl_x28c4:
    /*000028c4*/ LDR         R0, [PC, #372]              @ 0x0000000000002a40 
    /*000028c8*/ BL          0x00000000000028C8          @ osal_atomic_dec_return [R_ARM_CALL: +0x0] 
    /*000028cc*/ MOV         R0, R4 
    AencIoctl_x28d0:
    /*000028d0*/ ADD         SP, SP, #12 
    /*000028d4*/ POP         {R4, R5, R6, R7, PC} 
    AencIoctl_x28d8:
    /*000028d8*/ MOV         R1, R6 
    /*000028dc*/ BL          0x00000000000028DC          @ AencUpdateStreamBufWrTail [R_ARM_CALL: .text+0x2650] 
    /*000028e0*/ MOV         R4, R0 
    /*000028e4*/ B           AencIoctl_x28c4
    AencIoctl_x28e8:
    /*000028e8*/ MOV         R1, R6 
    /*000028ec*/ BL          0x00000000000028EC          @ AENC_PutStream [R_ARM_CALL: .text+0x120c] 
    /*000028f0*/ MOV         R4, R0 
    /*000028f4*/ B           AencIoctl_x28c4
    AencIoctl_x28f8:
    /*000028f8*/ CMP         R6, #0 
    /*000028fc*/ BEQ         AencIoctl_x2a28
    /*00002900*/ CMP         R0, #31 
    /*00002904*/ BHI         AencIoctl_x2a34
    /*00002908*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000290c*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002910*/ MOV         R3, #352                    @ 0x160 
    /*00002914*/ LDR         R1, [R6] 
    /*00002918*/ MLA         R0, R3, R0, R2 
    /*0000291c*/ MOV         R4, #0 
    /*00002920*/ STR         R1, [R0, #272]              @ 0x110 
    /*00002924*/ LDR         R3, [R6, #4] 
    /*00002928*/ STR         R3, [R0, #276]              @ 0x114 
    /*0000292c*/ LDR         R3, [R6, #12] 
    /*00002930*/ STR         R3, [R0, #284]              @ 0x11c 
    /*00002934*/ LDR         R3, [R6, #8] 
    /*00002938*/ STR         R3, [R0, #280]              @ 0x118 
    /*0000293c*/ LDR         R3, [R6, #24] 
    /*00002940*/ STR         R3, [R0, #296]              @ 0x128 
    /*00002944*/ LDR         R3, [R6, #28] 
    /*00002948*/ STR         R3, [R0, #300]              @ 0x12c 
    /*0000294c*/ B           AencIoctl_x28c4
    AencIoctl_x2950:
    /*00002950*/ MOVW        R3, #16641                  @ 0x4101 
    /*00002954*/ CMP         R5, R3 
    /*00002958*/ BEQ         AencIoctl_x2984
    /*0000295c*/ MOVW        R3, #16648                  @ 0x4108 
    /*00002960*/ CMP         R5, R3 
    /*00002964*/ BNE         AencIoctl_x27d8
    /*00002968*/ BL          0x0000000000002968          @ AENC_StrmBufExit [R_ARM_CALL: .text+0xb90] 
    /*0000296c*/ MOV         R4, R0 
    /*00002970*/ B           AencIoctl_x28c4
    AencIoctl_x2974:
    /*00002974*/ MOV         R1, R6 
    /*00002978*/ BL          0x0000000000002978          @ AencReleaseFrame [R_ARM_CALL: .text+0x21d0] 
    /*0000297c*/ MOV         R4, R0 
    /*00002980*/ B           AencIoctl_x28c4
    AencIoctl_x2984:
    /*00002984*/ BL          0x0000000000002984          @ AENC_DestroyChn [R_ARM_CALL: .text+0x1c40] 
    /*00002988*/ MOV         R4, R0 
    /*0000298c*/ B           AencIoctl_x28c4
    AencIoctl_x2990:
    /*00002990*/ MOV         R1, R6 
    /*00002994*/ BL          0x0000000000002994          @ AENC_StrmBufInit [R_ARM_CALL: .text+0x7fc] 
    /*00002998*/ MOV         R4, R0 
    /*0000299c*/ B           AencIoctl_x28c4
    AencIoctl_x29a0:
    /*000029a0*/ MOV         R1, R6 
    /*000029a4*/ BL          0x00000000000029A4          @ AENC_CreateChn [R_ARM_CALL: .text+0x1a64] 
    /*000029a8*/ MOV         R4, R0 
    /*000029ac*/ B           AencIoctl_x28c4
    AencIoctl_x29b0:
    /*000029b0*/ MOV         R1, R6 
    /*000029b4*/ BL          0x00000000000029B4          @ AencSendFrmToVoie [R_ARM_CALL: .text+0x2374] 
    /*000029b8*/ MOV         R4, R0 
    /*000029bc*/ B           AencIoctl_x28c4
    AencIoctl_x29c0:
    /*000029c0*/ BL          0x00000000000029C0          @ AENC_WakeupGetStream [R_ARM_CALL: .text+0x1528] 
    /*000029c4*/ MOV         R4, R0 
    /*000029c8*/ B           AencIoctl_x28c4
    AencIoctl_x29cc:
    /*000029cc*/ CMP         R6, #0 
    /*000029d0*/ BEQ         AencIoctl_x2a28
    /*000029d4*/ MOV         R1, R6 
    /*000029d8*/ LDR         R2, [R1], #8 
    /*000029dc*/ BL          0x00000000000029DC          @ AENC_GetStream [R_ARM_CALL: .text+0xc50] 
    /*000029e0*/ MOV         R4, R0 
    /*000029e4*/ B           AencIoctl_x28c4
    AencIoctl_x29e8:
    /*000029e8*/ MOVW        R3, #1229                   @ 0x4cd 
    /*000029ec*/ MOV         R1, #23 
    /*000029f0*/ STR         R3, [SP] 
    /*000029f4*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x348] 
    /*000029f8*/ LDR         R3, [PC, #72]               @ 0x0000000000002a48 
    /*000029fc*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x348] 
    /*00002a00*/ MOV         R0, #4 
    /*00002a04*/ BL          0x0000000000002A04          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002a08*/ MOVW        R2, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7d4] 
    /*00002a0c*/ MOV         R1, #23 
    /*00002a10*/ MOVT        R2, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7d4] 
    /*00002a14*/ MOV         R0, #4 
    /*00002a18*/ BL          0x0000000000002A18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002a1c*/ MOVW        R0, #32784                  @ 0x8010 
    /*00002a20*/ MOVT        R0, #40983                  @ 0xa017 
    /*00002a24*/ B           AencIoctl_x28d0
    AencIoctl_x2a28:
    /*00002a28*/ MOVW        R4, #32774                  @ 0x8006 
    /*00002a2c*/ MOVT        R4, #40983                  @ 0xa017 
    /*00002a30*/ B           AencIoctl_x28c4
    AencIoctl_x2a34:
    /*00002a34*/ MOVW        R4, #32770                  @ 0x8002 
    /*00002a38*/ MOVT        R4, #40983                  @ 0xa017 
    /*00002a3c*/ B           AencIoctl_x28c4
    /*00002a40*/ ANDEQ       R2, R0, R0, LSL #24 
    /*00002a44*/ ANDEQ       R0, R0, R0, LSR #2 
    /*00002a48*/ ANDEQ       R0, R0, R4, LSL R1 
FUNC_END AencIoctl


FUNC_BEGIN AENC_ModInit
    /*00002a4c*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00002a50*/ MOVW        R0, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x81c] 
    /*00002a54*/ SUB         SP, SP, #12 
    /*00002a58*/ MOV         R1, #0 
    /*00002a5c*/ MOVT        R0, #0                      @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0x81c] 
    /*00002a60*/ BL          0x0000000000002A60          @ osal_create_proc_entry [R_ARM_CALL: +0x0] 
    /*00002a64*/ SUBS        R2, R0, #0 
    /*00002a68*/ BEQ         AENC_ModInit_x2b8c
    /*00002a6c*/ MOVW        R3, #0                      @ AencProcShow [R_ARM_MOVW_ABS_NC: .text+0x178] 
    /*00002a70*/ MOVW        R0, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x81c] 
    /*00002a74*/ MOVT        R3, #0                      @ AencProcShow [R_ARM_MOVT_ABS: .text+0x178] 
    /*00002a78*/ MOVT        R0, #0                      @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0x81c] 
    /*00002a7c*/ STR         R3, [R2, #60]               @ 0x3c 
    /*00002a80*/ MOVW        R4, #0                      @ .LANCHOR3 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00002a84*/ BL          0x0000000000002A84          @ osal_createdev [R_ARM_CALL: +0x0] 
    /*00002a88*/ LDR         R5, [PC, #292]              @ 0x0000000000002bb4 
    /*00002a8c*/ MOVT        R4, #0                      @ .LANCHOR3 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00002a90*/ ADD         R2, R5, #32 
    /*00002a94*/ STR         R5, [R0, #56]               @ 0x38 
    /*00002a98*/ STR         R2, [R0, #60]               @ 0x3c 
    /*00002a9c*/ MOV         R2, #13 
    /*00002aa0*/ STR         R2, [R0, #52]               @ 0x34 
    /*00002aa4*/ STR         R0, [R4, #3088]             @ 0xc10 
    /*00002aa8*/ BL          0x0000000000002AA8          @ osal_registerdevice [R_ARM_CALL: +0x0] 
    /*00002aac*/ SUBS        R9, R0, #0 
    /*00002ab0*/ BNE         AENC_ModInit_x2b4c
    /*00002ab4*/ ADD         R0, R5, #112                @ 0x70 
    /*00002ab8*/ BL          0x0000000000002AB8          @ CMPI_RegisterModule [R_ARM_CALL: +0x0] 
    /*00002abc*/ CMP         R0, #0 
    /*00002ac0*/ BNE         AENC_ModInit_x2b64
    /*00002ac4*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002ac8*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002acc*/ MOV         R5, R9 
    /*00002ad0*/ MOV         R8, #352                    @ 0x160 
    /*00002ad4*/ MOV         R4, R6 
    /*00002ad8*/ MOV         R7, R9 
    AENC_ModInit_x2adc:
    /*00002adc*/ MOV         R2, #352                    @ 0x160 
    /*00002ae0*/ MOV         R1, #0 
    /*00002ae4*/ MOV         R0, R4 
    /*00002ae8*/ BL          0x0000000000002AE8          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00002aec*/ MLA         R3, R8, R5, R6 
    /*00002af0*/ MOV         R1, #1 
    /*00002af4*/ ADD         R0, R4, #208                @ 0xd0 
    /*00002af8*/ ADD         R5, R5, R1 
    /*00002afc*/ STR         R7, [R3, #248]              @ 0xf8 
    /*00002b00*/ BL          0x0000000000002B00          @ osal_sema_init [R_ARM_CALL: +0x0] 
    /*00002b04*/ ADD         R0, R4, #212                @ 0xd4 
    /*00002b08*/ BL          0x0000000000002B08          @ osal_wait_init [R_ARM_CALL: +0x0] 
    /*00002b0c*/ ADD         R0, R4, #244                @ 0xf4 
    /*00002b10*/ BL          0x0000000000002B10          @ osal_wait_init [R_ARM_CALL: +0x0] 
    /*00002b14*/ ADD         R0, R4, #312                @ 0x138 
    /*00002b18*/ BL          0x0000000000002B18          @ osal_spin_lock_init [R_ARM_CALL: +0x0] 
    /*00002b1c*/ CMP         R5, #32 
    /*00002b20*/ ADD         R4, R4, #352                @ 0x160 
    /*00002b24*/ BNE         AENC_ModInit_x2adc
    /*00002b28*/ BL          0x0000000000002B28          @ AENC_IsSupportVoie [R_ARM_CALL: .text+0x2ea4] 
    /*00002b2c*/ LDR         R0, [PC, #132]              @ 0x0000000000002bb8 
    /*00002b30*/ BL          0x0000000000002B30          @ osal_atomic_init [R_ARM_CALL: +0x0] 
    /*00002b34*/ MOV         R1, #0 
    /*00002b38*/ LDR         R0, [PC, #120]              @ 0x0000000000002bb8 
    /*00002b3c*/ BL          0x0000000000002B3C          @ osal_atomic_set [R_ARM_CALL: +0x0] 
    AENC_ModInit_x2b40:
    /*00002b40*/ MOV         R0, R9 
    /*00002b44*/ ADD         SP, SP, #12 
    /*00002b48*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AENC_ModInit_x2b4c:
    /*00002b4c*/ MOVW        R0, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x81c] 
    /*00002b50*/ MOV         R1, #0 
    /*00002b54*/ MOVT        R0, #0                      @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0x81c] 
    /*00002b58*/ MVN         R9, #0 
    /*00002b5c*/ BL          0x0000000000002B5C          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*00002b60*/ B           AENC_ModInit_x2b40
    AENC_ModInit_x2b64:
    /*00002b64*/ MOV         R1, R9 
    /*00002b68*/ MOVW        R0, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x81c] 
    /*00002b6c*/ MOVT        R0, #0                      @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0x81c] 
    /*00002b70*/ MVN         R9, #0 
    /*00002b74*/ BL          0x0000000000002B74          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*00002b78*/ LDR         R0, [R4, #3088]             @ 0xc10 
    /*00002b7c*/ BL          0x0000000000002B7C          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*00002b80*/ LDR         R0, [R4, #3088]             @ 0xc10 
    /*00002b84*/ BL          0x0000000000002B84          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*00002b88*/ B           AENC_ModInit_x2b40
    AENC_ModInit_x2b8c:
    /*00002b8c*/ MOVW        R3, #1447                   @ 0x5a7 
    /*00002b90*/ MOVW        R2, #0                      @ .LC80 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x824] 
    /*00002b94*/ STR         R3, [SP] 
    /*00002b98*/ MOVT        R2, #0                      @ .LC80 [R_ARM_MOVT_ABS: .rodata.str1.4+0x824] 
    /*00002b9c*/ LDR         R3, [PC, #24]               @ 0x0000000000002bbc 
    /*00002ba0*/ MOV         R1, #22 
    /*00002ba4*/ MOV         R0, #6 
    /*00002ba8*/ MVN         R9, #0 
    /*00002bac*/ BL          0x0000000000002BAC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002bb0*/ B           AENC_ModInit_x2b40
    /*00002bb4*/ ANDEQ       R0, R0, R4 
    /*00002bb8*/ ANDEQ       R2, R0, R0, LSL #24 
    /*00002bbc*/ ANDEQ       R0, R0, IP, LSR #2 
FUNC_END AENC_ModInit


FUNC_BEGIN AENC_ModExit
    /*00002bc0*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00002bc4*/ MOV         R0, #23 
    /*00002bc8*/ BL          0x0000000000002BC8          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    /*00002bcc*/ LDR         R0, [PC, #124]              @ 0x0000000000002c50 
    /*00002bd0*/ BL          0x0000000000002BD0          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*00002bd4*/ LDR         R4, [PC, #120]              @ 0x0000000000002c54 
    /*00002bd8*/ MOV         R5, #0 
    /*00002bdc*/ MOV         R7, #352                    @ 0x160 
    /*00002be0*/ SUB         R8, R4, #208                @ 0xd0 
    /*00002be4*/ MOV         R6, R5 
    AENC_ModExit_x2be8:
    /*00002be8*/ MLA         R3, R7, R5, R8 
    /*00002bec*/ MOV         R0, R4 
    /*00002bf0*/ ADD         R5, R5, #1 
    /*00002bf4*/ STR         R6, [R3, #248]              @ 0xf8 
    /*00002bf8*/ BL          0x0000000000002BF8          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*00002bfc*/ ADD         R0, R4, #4 
    /*00002c00*/ BL          0x0000000000002C00          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00002c04*/ ADD         R0, R4, #36                 @ 0x24 
    /*00002c08*/ BL          0x0000000000002C08          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00002c0c*/ ADD         R0, R4, #104                @ 0x68 
    /*00002c10*/ BL          0x0000000000002C10          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00002c14*/ CMP         R5, #32 
    /*00002c18*/ ADD         R4, R4, #352                @ 0x160 
    /*00002c1c*/ BNE         AENC_ModExit_x2be8
    /*00002c20*/ MOVW        R4, #0                      @ .LANCHOR3 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00002c24*/ MOVT        R4, #0                      @ .LANCHOR3 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00002c28*/ LDR         R0, [R4, #3088]             @ 0xc10 
    /*00002c2c*/ BL          0x0000000000002C2C          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*00002c30*/ LDR         R0, [R4, #3088]             @ 0xc10 
    /*00002c34*/ BL          0x0000000000002C34          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*00002c38*/ MOVW        R0, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x81c] 
    /*00002c3c*/ MOV         R1, #0 
    /*00002c40*/ MOVT        R0, #0                      @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0x81c] 
    /*00002c44*/ BL          0x0000000000002C44          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*00002c48*/ POP         {R4, R5, R6, R7, R8, LR} 
    AENC_ModExit_x2c4c:
    /*00002c4c*/ B           AENC_ModExit_x2c4c          @ AENC_IsSupportVoie [R_ARM_JUMP24: .text+0x2ea4] 
    /*00002c50*/ ANDEQ       R2, R0, R0, LSL #24 
    /*00002c54*/ LDRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
FUNC_END AENC_ModExit


FUNC_BEGIN AUDIO_FB_Init
    /*00002c58*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00002c5c*/ SUBS        R5, R1, #0 
    /*00002c60*/ BEQ         AUDIO_FB_Init_x2ce8
    /*00002c64*/ MOV         R8, #136                    @ 0x88 
    /*00002c68*/ ADD         R3, R5, #12 
    /*00002c6c*/ MUL         R8, R8, R0 
    /*00002c70*/ ADD         R7, R5, #4 
    /*00002c74*/ MOV         R6, R0 
    /*00002c78*/ STR         R7, [R5, #4] 
    /*00002c7c*/ STR         R7, [R5, #8] 
    /*00002c80*/ STR         R3, [R5, #12] 
    /*00002c84*/ STR         R3, [R5, #16] 
    /*00002c88*/ MOV         R0, R8 
    /*00002c8c*/ BL          0x0000000000002C8C          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*00002c90*/ SUBS        R4, R0, #0 
    /*00002c94*/ BEQ         AUDIO_FB_Init_x2ce8
    /*00002c98*/ MOV         R2, R8 
    /*00002c9c*/ MOV         R1, #0 
    /*00002ca0*/ BL          0x0000000000002CA0          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00002ca4*/ CMP         R6, #0 
    /*00002ca8*/ STR         R4, [R5] 
    /*00002cac*/ MOVNE       R3, #0 
    /*00002cb0*/ BEQ         AUDIO_FB_Init_x2cd8
    AUDIO_FB_Init_x2cb4:
    /*00002cb4*/ LDR         R2, [R5, #8] 
    /*00002cb8*/ ADD         R3, R3, #1 
    /*00002cbc*/ CMP         R3, R6 
    /*00002cc0*/ STR         R4, [R5, #8] 
    /*00002cc4*/ STR         R7, [R4] 
    /*00002cc8*/ STR         R2, [R4, #4] 
    /*00002ccc*/ STR         R4, [R2] 
    /*00002cd0*/ ADD         R4, R4, #136                @ 0x88 
    /*00002cd4*/ BNE         AUDIO_FB_Init_x2cb4
    AUDIO_FB_Init_x2cd8:
    /*00002cd8*/ MOV         R0, #0 
    /*00002cdc*/ STR         R0, [R5, #24] 
    /*00002ce0*/ STR         R6, [R5, #20] 
    /*00002ce4*/ POP         {R4, R5, R6, R7, R8, PC} 
    AUDIO_FB_Init_x2ce8:
    /*00002ce8*/ MVN         R0, #0 
    /*00002cec*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END AUDIO_FB_Init


FUNC_BEGIN AUDIO_FB_Release
    /*00002cf0*/ PUSH        {R4, LR} 
    /*00002cf4*/ SUBS        R4, R0, #0 
    /*00002cf8*/ BEQ         AUDIO_FB_Release_x2d1c
    /*00002cfc*/ LDR         R0, [R4] 
    /*00002d00*/ CMP         R0, #0 
    /*00002d04*/ POPEQ       {R4, PC} 
    /*00002d08*/ BL          0x0000000000002D08          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*00002d0c*/ MOV         R3, #0 
    /*00002d10*/ MOV         R0, R3 
    /*00002d14*/ STR         R3, [R4] 
    /*00002d18*/ POP         {R4, PC} 
    AUDIO_FB_Release_x2d1c:
    /*00002d1c*/ MVN         R0, #0 
    /*00002d20*/ POP         {R4, PC} 
FUNC_END AUDIO_FB_Release


FUNC_BEGIN AUDIO_FB_GetFree
    /*00002d24*/ SUBS        R2, R0, #0 
    /*00002d28*/ BEQ         AUDIO_FB_GetFree_x2d7c
    /*00002d2c*/ MOV         R1, R2 
    /*00002d30*/ LDR         R3, [R1, #4]! 
    /*00002d34*/ CMP         R3, R1 
    /*00002d38*/ BEQ         AUDIO_FB_GetFree_x2d7c
    /*00002d3c*/ PUSH        {R4, LR} 
    /*00002d40*/ MOV         R0, R3 
    /*00002d44*/ LDR         LR, [R3, #4] 
    /*00002d48*/ MOV         IP, #256                    @ 0x100 
    /*00002d4c*/ LDR         R4, [R3] 
    /*00002d50*/ MOVT        IP, #16 
    /*00002d54*/ MOV         R1, #512                    @ 0x200 
    /*00002d58*/ MOVT        R1, #32 
    /*00002d5c*/ STR         LR, [R4, #4] 
    /*00002d60*/ STR         R4, [LR] 
    /*00002d64*/ STR         IP, [R3] 
    /*00002d68*/ STR         R1, [R3, #4] 
    /*00002d6c*/ LDR         R3, [R2, #20] 
    /*00002d70*/ SUB         R3, R3, #1 
    /*00002d74*/ STR         R3, [R2, #20] 
    /*00002d78*/ POP         {R4, PC} 
    AUDIO_FB_GetFree_x2d7c:
    /*00002d7c*/ MOV         R0, #0 
    /*00002d80*/ BX          LR 
FUNC_END AUDIO_FB_GetFree


FUNC_BEGIN AUDIO_FB_GetBusy
    /*00002d84*/ SUBS        R2, R0, #0 
    /*00002d88*/ BEQ         AUDIO_FB_GetBusy_x2ddc
    /*00002d8c*/ MOV         R1, R2 
    /*00002d90*/ LDR         R3, [R1, #12]! 
    /*00002d94*/ CMP         R3, R1 
    /*00002d98*/ BEQ         AUDIO_FB_GetBusy_x2ddc
    /*00002d9c*/ PUSH        {R4, LR} 
    /*00002da0*/ MOV         R0, R3 
    /*00002da4*/ LDR         LR, [R3, #4] 
    /*00002da8*/ MOV         IP, #256                    @ 0x100 
    /*00002dac*/ LDR         R4, [R3] 
    /*00002db0*/ MOVT        IP, #16 
    /*00002db4*/ MOV         R1, #512                    @ 0x200 
    /*00002db8*/ MOVT        R1, #32 
    /*00002dbc*/ STR         LR, [R4, #4] 
    /*00002dc0*/ STR         R4, [LR] 
    /*00002dc4*/ STR         IP, [R3] 
    /*00002dc8*/ STR         R1, [R3, #4] 
    /*00002dcc*/ LDR         R3, [R2, #24] 
    /*00002dd0*/ SUB         R3, R3, #1 
    /*00002dd4*/ STR         R3, [R2, #24] 
    /*00002dd8*/ POP         {R4, PC} 
    AUDIO_FB_GetBusy_x2ddc:
    /*00002ddc*/ MOV         R0, #0 
    /*00002de0*/ BX          LR 
FUNC_END AUDIO_FB_GetBusy


FUNC_BEGIN AUDIO_FB_PutFree
    /*00002de4*/ CMP         R1, #0 
    /*00002de8*/ CMPNE       R0, #0 
    /*00002dec*/ MOVEQ       R2, #1 
    /*00002df0*/ MOVNE       R2, #0 
    /*00002df4*/ BEQ         AUDIO_FB_PutFree_x2e2c
    /*00002df8*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00002dfc*/ MOV         R3, R0 
    /*00002e00*/ LDR         IP, [R0, #8] 
    /*00002e04*/ ADD         LR, R0, #4 
    /*00002e08*/ STR         R1, [R0, #8] 
    /*00002e0c*/ MOV         R0, R2 
    /*00002e10*/ STR         LR, [R1] 
    /*00002e14*/ STR         IP, [R1, #4] 
    /*00002e18*/ STR         R1, [IP] 
    /*00002e1c*/ LDR         R2, [R3, #20] 
    /*00002e20*/ ADD         R2, R2, #1 
    /*00002e24*/ STR         R2, [R3, #20] 
    /*00002e28*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    AUDIO_FB_PutFree_x2e2c:
    /*00002e2c*/ MVN         R0, #0 
    /*00002e30*/ BX          LR 
FUNC_END AUDIO_FB_PutFree


FUNC_BEGIN AUDIO_FB_PutBusy
    /*00002e34*/ CMP         R1, #0 
    /*00002e38*/ CMPNE       R0, #0 
    /*00002e3c*/ MOVEQ       R2, #1 
    /*00002e40*/ MOVNE       R2, #0 
    /*00002e44*/ BEQ         AUDIO_FB_PutBusy_x2e7c
    /*00002e48*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00002e4c*/ MOV         R3, R0 
    /*00002e50*/ LDR         IP, [R0, #16] 
    /*00002e54*/ ADD         LR, R0, #12 
    /*00002e58*/ STR         R1, [R0, #16] 
    /*00002e5c*/ MOV         R0, R2 
    /*00002e60*/ STR         LR, [R1] 
    /*00002e64*/ STR         IP, [R1, #4] 
    /*00002e68*/ STR         R1, [IP] 
    /*00002e6c*/ LDR         R2, [R3, #24] 
    /*00002e70*/ ADD         R2, R2, #1 
    /*00002e74*/ STR         R2, [R3, #24] 
    /*00002e78*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    AUDIO_FB_PutBusy_x2e7c:
    /*00002e7c*/ MVN         R0, #0 
    /*00002e80*/ BX          LR 
FUNC_END AUDIO_FB_PutBusy


FUNC_BEGIN AUDIO_FB_QueryFreeNum
    /*00002e84*/ CMP         R0, #0 
    /*00002e88*/ LDRNE       R0, [R0, #20] 
    /*00002e8c*/ MVNEQ       R0, #0 
    /*00002e90*/ BX          LR 
FUNC_END AUDIO_FB_QueryFreeNum


FUNC_BEGIN AUDIO_FB_QueryBusyNum
    /*00002e94*/ CMP         R0, #0 
    /*00002e98*/ LDRNE       R0, [R0, #24] 
    /*00002e9c*/ MVNEQ       R0, #0 
    /*00002ea0*/ BX          LR 
FUNC_END AUDIO_FB_QueryBusyNum


FUNC_BEGIN AENC_IsSupportVoie
    /*00002ea4*/ MOV         R0, #0 
    /*00002ea8*/ BX          LR 
FUNC_END AENC_IsSupportVoie


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .init.text
@ Size: 0x10
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN aenc_mod_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ BL          0x0000000000000004          @ AENC_ModInit [R_ARM_CALL: .text+0x2a4c] 
    /*00000008*/ MOV         R0, #0 
    /*0000000c*/ POP         {R4, PC} 
FUNC_END aenc_mod_init




@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .exit.text
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN aenc_mod_exit
    aenc_mod_exit_x0:
    /*00000000*/ B           aenc_mod_exit_x0            @ AENC_ModExit [R_ARM_JUMP24: .text+0x2bc0] 
FUNC_END aenc_mod_exit




