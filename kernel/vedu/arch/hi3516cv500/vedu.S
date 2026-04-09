@
@ -- FILES --
@

@ vedu_ext.c
@ vedu_drv_comm.c
@ vedu_dec.c
@ vedu_drv.c
@ vedu_hal.c
@ vedu_dec_hal_h264.c
@ vedu_dec_hal_h265.c
@ vedu_init.c
@ hi3516cv500_vedu.mod.c


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text
@ Size: 0x5a64
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN VPU_Notify
    /*00000000*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000004*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000008*/ MOV         R2, #1 
    /*0000000c*/ STR         R2, [R3] 
    /*00000010*/ BX          LR 
FUNC_END VPU_Notify


FUNC_BEGIN VPU_GetVerMagic
    /*00000014*/ MOVW        R0, #61479                  @ 0xf027 
    /*00000018*/ MOVT        R0, #307                    @ 0x133 
    /*0000001c*/ BX          LR 
FUNC_END VPU_GetVerMagic


FUNC_BEGIN VEDU_RegisterVpu
    /*00000020*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00000024*/ SUB         SP, SP, #148                @ 0x94 
    /*00000028*/ ADD         R0, SP, #32 
    /*0000002c*/ BL          0x000000000000002C          @ VEDU_GetInfo [R_ARM_CALL: .text+0x195c] 
    /*00000030*/ CMP         R0, #1 
    /*00000034*/ BEQ         VEDU_RegisterVpu_x78
    /*00000038*/ STR         R0, [SP, #4] 
    /*0000003c*/ MOV         R2, #1 
    /*00000040*/ MOV         R3, #91                     @ 0x5b 
    /*00000044*/ STR         R2, [SP, #8] 
    /*00000048*/ STR         R3, [SP] 
    /*0000004c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*00000050*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000054*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*00000058*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*0000005c*/ MOV         R1, #25 
    /*00000060*/ MOV         R0, #3 
    /*00000064*/ BL          0x0000000000000064          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000068*/ MOVW        R0, #32777                  @ 0x8009 
    /*0000006c*/ MOVT        R0, #40985                  @ 0xa019 
    VEDU_RegisterVpu_x70:
    /*00000070*/ ADD         SP, SP, #148                @ 0x94 
    /*00000074*/ POP         {R4, R5, R6, R7, PC} 
    VEDU_RegisterVpu_x78:
    /*00000078*/ LDR         R3, [SP, #32] 
    /*0000007c*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000080*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000084*/ MOVW        R4, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000088*/ CMP         R3, #0 
    /*0000008c*/ MOVT        R4, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000090*/ STR         R0, [R5, #4] 
    /*00000094*/ BNE         VEDU_RegisterVpu_x130
    VEDU_RegisterVpu_x98:
    /*00000098*/ MOV         R0, #0 
    /*0000009c*/ ADD         R6, SP, #60                 @ 0x3c 
    /*000000a0*/ BL          0x00000000000000A0          @ VEDU_DRV_ClkandPwrDown [R_ARM_CALL: .text+0x1d48] 
    VEDU_RegisterVpu_xa4:
    /*000000a4*/ MOV         R0, #4 
    /*000000a8*/ MOV         R2, #15 
    /*000000ac*/ MOV         R3, #16 
    /*000000b0*/ STR         R6, [SP, #28] 
    /*000000b4*/ STR         R2, [SP, #16] 
    /*000000b8*/ MOV         R6, #1 
    /*000000bc*/ STR         R3, [SP, #24] 
    /*000000c0*/ STR         R6, [SP, #20] 
    /*000000c4*/ BL          0x00000000000000C4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000000c8*/ LDR         R3, [R0, #8] 
    /*000000cc*/ ADD         R0, SP, #16 
    /*000000d0*/ BLX         R3 
    /*000000d4*/ CMP         R0, #0 
    /*000000d8*/ STRNE       R0, [R4, #8] 
    /*000000dc*/ STRNE       R6, [R4, #12] 
    /*000000e0*/ MOVNE       R0, #0 
    /*000000e4*/ BNE         VEDU_RegisterVpu_x70
    /*000000e8*/ MOV         R2, #169                    @ 0xa9 
    /*000000ec*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000000f0*/ STR         R2, [SP] 
    /*000000f4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*000000f8*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x84] 
    /*000000fc*/ MOV         R1, #25 
    /*00000100*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x84] 
    /*00000104*/ MOV         R0, #3 
    /*00000108*/ BL          0x0000000000000108          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000010c*/ LDR         R0, [R4] 
    /*00000110*/ CMP         R0, #0 
    /*00000114*/ BEQ         VEDU_RegisterVpu_x124
    /*00000118*/ LDR         R5, [R5, #4] 
    /*0000011c*/ CMP         R5, #0 
    /*00000120*/ BEQ         VEDU_RegisterVpu_x268
    VEDU_RegisterVpu_x124:
    /*00000124*/ MOVW        R0, #32777                  @ 0x8009 
    /*00000128*/ MOVT        R0, #40985                  @ 0xa019 
    /*0000012c*/ B           VEDU_RegisterVpu_x70
    VEDU_RegisterVpu_x130:
    /*00000130*/ LDR         IP, [R4] 
    /*00000134*/ CMP         IP, #0 
    /*00000138*/ BEQ         VEDU_RegisterVpu_x234
    VEDU_RegisterVpu_x13c:
    /*0000013c*/ LDR         R3, [R4, #4] 
    /*00000140*/ MOVW        R1, #0                      @ VEDU_ReadInt [R_ARM_MOVW_ABS_NC: .text+0xabc] 
    /*00000144*/ MOVW        R2, #0                      @ VEDU_ClearInt [R_ARM_MOVW_ABS_NC: .text+0xaf4] 
    /*00000148*/ MOVT        R1, #0                      @ VEDU_ReadInt [R_ARM_MOVT_ABS: .text+0xabc] 
    /*0000014c*/ CMP         R3, #0 
    /*00000150*/ MOVT        R2, #0                      @ VEDU_ClearInt [R_ARM_MOVT_ABS: .text+0xaf4] 
    /*00000154*/ STR         R1, [SP, #92]               @ 0x5c 
    /*00000158*/ MOVW        R1, #0                      @ VEDU_ClearErrInt [R_ARM_MOVW_ABS_NC: .text+0xb2c] 
    /*0000015c*/ STR         R2, [SP, #96]               @ 0x60 
    /*00000160*/ MOVT        R1, #0                      @ VEDU_ClearErrInt [R_ARM_MOVT_ABS: .text+0xb2c] 
    /*00000164*/ MOVW        R2, #0                      @ VEDU_SetIntMask [R_ARM_MOVW_ABS_NC: .text+0xb64] 
    /*00000168*/ STR         R1, [SP, #100]              @ 0x64 
    /*0000016c*/ MOVT        R2, #0                      @ VEDU_SetIntMask [R_ARM_MOVT_ABS: .text+0xb64] 
    /*00000170*/ MOVW        R1, #0                      @ VEDU_GetIsIntOk [R_ARM_MOVW_ABS_NC: .text+0xb6c] 
    /*00000174*/ STR         R2, [SP, #104]              @ 0x68 
    /*00000178*/ MOVT        R1, #0                      @ VEDU_GetIsIntOk [R_ARM_MOVT_ABS: .text+0xb6c] 
    /*0000017c*/ MOVW        R2, #0                      @ VEDU_SetTimeOut [R_ARM_MOVW_ABS_NC: .text+0xd5c] 
    /*00000180*/ STR         R1, [SP, #108]              @ 0x6c 
    /*00000184*/ MOVT        R2, #0                      @ VEDU_SetTimeOut [R_ARM_MOVT_ABS: .text+0xd5c] 
    /*00000188*/ MOVW        R1, #0                      @ VEDU_GetTimeOut [R_ARM_MOVW_ABS_NC: .text+0xd64] 
    /*0000018c*/ STR         R2, [SP, #112]              @ 0x70 
    /*00000190*/ MOVT        R1, #0                      @ VEDU_GetTimeOut [R_ARM_MOVT_ABS: .text+0xd64] 
    /*00000194*/ MOVW        R2, #0                      @ VEDU_IsNeedReset [R_ARM_MOVW_ABS_NC: .text+0xd24] 
    /*00000198*/ STR         R1, [SP, #116]              @ 0x74 
    /*0000019c*/ MOVT        R2, #0                      @ VEDU_IsNeedReset [R_ARM_MOVT_ABS: .text+0xd24] 
    /*000001a0*/ MOVW        R1, #0                      @ VEDU_Reset [R_ARM_MOVW_ABS_NC: .text+0xe3c] 
    /*000001a4*/ STR         R2, [SP, #120]              @ 0x78 
    /*000001a8*/ MOVT        R1, #0                      @ VEDU_Reset [R_ARM_MOVT_ABS: .text+0xe3c] 
    /*000001ac*/ MOVW        R2, #0                      @ VEDU_SetVpuClk [R_ARM_MOVW_ABS_NC: .text+0xbe8] 
    /*000001b0*/ STR         R1, [SP, #124]              @ 0x7c 
    /*000001b4*/ MOVT        R2, #0                      @ VEDU_SetVpuClk [R_ARM_MOVT_ABS: .text+0xbe8] 
    /*000001b8*/ MOVW        R1, #0                      @ VEDU_StartVedu [R_ARM_MOVW_ABS_NC: .text+0xd6c] 
    /*000001bc*/ STR         R2, [SP, #128]              @ 0x80 
    /*000001c0*/ MOVT        R1, #0                      @ VEDU_StartVedu [R_ARM_MOVT_ABS: .text+0xd6c] 
    /*000001c4*/ MOVW        R2, #0                      @ VEDU_OutputDebugInfo [R_ARM_MOVW_ABS_NC: .text+0xda4] 
    /*000001c8*/ STR         R1, [SP, #132]              @ 0x84 
    /*000001cc*/ MOVT        R2, #0                      @ VEDU_OutputDebugInfo [R_ARM_MOVT_ABS: .text+0xda4] 
    /*000001d0*/ MOV         R1, #0 
    /*000001d4*/ STR         R2, [SP, #136]              @ 0x88 
    /*000001d8*/ MOV         R2, #1 
    /*000001dc*/ STR         R1, [SP, #76]               @ 0x4c 
    /*000001e0*/ MOVNE       R1, R3 
    /*000001e4*/ STR         R2, [SP, #84]               @ 0x54 
    /*000001e8*/ LDREQ       R1, [SP, #36]               @ 0x24 
    /*000001ec*/ ADD         R6, SP, #60                 @ 0x3c 
    /*000001f0*/ MOV         R7, #0 
    /*000001f4*/ STR         R1, [SP, #80]               @ 0x50 
    /*000001f8*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*000001fc*/ MOV         R1, #16 
    /*00000200*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00000204*/ STR         R7, [SP] 
    /*00000208*/ MOV         R0, R6 
    /*0000020c*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7c] 
    /*00000210*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7c] 
    /*00000214*/ STR         IP, [SP, #88]               @ 0x58 
    /*00000218*/ STR         R7, [SP, #140]              @ 0x8c 
    /*0000021c*/ BL          0x000000000000021C          @ osal_snprintf [R_ARM_CALL: +0x0] 
    /*00000220*/ MOV         R0, R7 
    /*00000224*/ BL          0x0000000000000224          @ VEDU_DRV_ClkandPwrUp [R_ARM_CALL: .text+0x1d24] 
    /*00000228*/ LDR         R0, [R4] 
    /*0000022c*/ BL          0x000000000000022C          @ VEDU_DRV_Init [R_ARM_CALL: .text+0x1d78] 
    /*00000230*/ B           VEDU_RegisterVpu_xa4
    VEDU_RegisterVpu_x234:
    /*00000234*/ LDR         R1, [SP, #48]               @ 0x30 
    /*00000238*/ LDR         R0, [SP, #40]               @ 0x28 
    /*0000023c*/ BL          0x000000000000023C          @ osal_ioremap [R_ARM_CALL: +0x0] 
    /*00000240*/ CMP         R0, #0 
    /*00000244*/ MOV         IP, R0 
    /*00000248*/ STR         R0, [R4] 
    /*0000024c*/ BEQ         VEDU_RegisterVpu_x27c
    /*00000250*/ LDR         R3, [SP, #32] 
    /*00000254*/ MOV         R2, #0 
    /*00000258*/ STR         R2, [R5, #4] 
    /*0000025c*/ CMP         R3, R2 
    /*00000260*/ BNE         VEDU_RegisterVpu_x13c
    /*00000264*/ B           VEDU_RegisterVpu_x98
    VEDU_RegisterVpu_x268:
    /*00000268*/ BL          0x0000000000000268          @ osal_iounmap [R_ARM_CALL: +0x0] 
    /*0000026c*/ MOVW        R0, #32777                  @ 0x8009 
    /*00000270*/ STR         R5, [R4] 
    /*00000274*/ MOVT        R0, #40985                  @ 0xa019 
    /*00000278*/ B           VEDU_RegisterVpu_x70
    VEDU_RegisterVpu_x27c:
    /*0000027c*/ MOV         R2, #111                    @ 0x6f 
    /*00000280*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000284*/ STR         R2, [SP] 
    /*00000288*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*0000028c*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000290*/ MOV         R1, #25 
    /*00000294*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000298*/ MOV         R0, #3 
    /*0000029c*/ BL          0x000000000000029C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000002a0*/ MOVW        R0, #32777                  @ 0x8009 
    /*000002a4*/ MOVT        R0, #40985                  @ 0xa019 
    /*000002a8*/ B           VEDU_RegisterVpu_x70
FUNC_END VEDU_RegisterVpu


FUNC_BEGIN VEDU_UnRegisterVpu
    /*000002ac*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*000002b0*/ MOVW        R4, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000002b4*/ MOVT        R4, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000002b8*/ LDR         R3, [R4, #8] 
    /*000002bc*/ CMP         R3, #0 
    /*000002c0*/ BEQ         VEDU_UnRegisterVpu_x3ac
    VEDU_UnRegisterVpu_x2c4:
    /*000002c4*/ MOV         R0, #4 
    /*000002c8*/ BL          0x00000000000002C8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000002cc*/ LDR         R3, [R0, #12] 
    /*000002d0*/ LDR         R0, [R4, #8] 
    /*000002d4*/ BLX         R3 
    /*000002d8*/ CMP         R0, #0 
    /*000002dc*/ BNE         VEDU_UnRegisterVpu_x380
    VEDU_UnRegisterVpu_x2e0:
    /*000002e0*/ LDR         R3, [R4, #12] 
    /*000002e4*/ MOV         R2, #0 
    /*000002e8*/ STR         R2, [R4, #8] 
    /*000002ec*/ CMP         R3, #1 
    /*000002f0*/ BEQ         VEDU_UnRegisterVpu_x328
    /*000002f4*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*000002f8*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd8] 
    /*000002fc*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000300*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd8] 
    /*00000304*/ MOV         R2, #216                    @ 0xd8 
    /*00000308*/ LDR         R1, [PC, #200]              @ 0x00000000000003d8 
    /*0000030c*/ BL          0x000000000000030C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000310*/ LDR         R3, [R4, #12] 
    /*00000314*/ CMP         R3, #0 
    /*00000318*/ BGT         VEDU_UnRegisterVpu_x328
    VEDU_UnRegisterVpu_x31c:
    /*0000031c*/ MOV         R3, #0 
    /*00000320*/ STR         R3, [R4, #12] 
    /*00000324*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    VEDU_UnRegisterVpu_x328:
    /*00000328*/ MOVW        R6, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000032c*/ MOVW        R7, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000330*/ MOVT        R6, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000334*/ MOVT        R7, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000338*/ MOV         R5, #0 
    VEDU_UnRegisterVpu_x33c:
    /*0000033c*/ MOV         R0, R6 
    /*00000340*/ LSL         R9, R5, #2 
    /*00000344*/ BL          0x0000000000000344          @ VEDU_DRV_Exit [R_ARM_CALL: .text+0x1dd8] 
    /*00000348*/ MOV         R0, R5 
    /*0000034c*/ MOV         R1, #0 
    /*00000350*/ BL          0x0000000000000350          @ VEDU_SetVpuClk [R_ARM_CALL: .text+0xbe8] 
    /*00000354*/ LDR         R0, [R4, R5, LSL #2] 
    /*00000358*/ ADD         R5, R5, #1 
    /*0000035c*/ CMP         R0, #0 
    /*00000360*/ BEQ         VEDU_UnRegisterVpu_x370
    /*00000364*/ LDR         R8, [R7, #4] 
    /*00000368*/ CMP         R8, #0 
    /*0000036c*/ BEQ         VEDU_UnRegisterVpu_x3a0
    VEDU_UnRegisterVpu_x370:
    /*00000370*/ LDR         R3, [R4, #12] 
    /*00000374*/ CMP         R3, R5 
    /*00000378*/ BGT         VEDU_UnRegisterVpu_x33c
    /*0000037c*/ B           VEDU_UnRegisterVpu_x31c
    VEDU_UnRegisterVpu_x380:
    /*00000380*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x118] 
    /*00000384*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd8] 
    /*00000388*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x118] 
    /*0000038c*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd8] 
    /*00000390*/ MOV         R2, #212                    @ 0xd4 
    /*00000394*/ LDR         R1, [PC, #60]               @ 0x00000000000003d8 
    /*00000398*/ BL          0x0000000000000398          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000039c*/ B           VEDU_UnRegisterVpu_x2e0
    VEDU_UnRegisterVpu_x3a0:
    /*000003a0*/ BL          0x00000000000003A0          @ osal_iounmap [R_ARM_CALL: +0x0] 
    /*000003a4*/ STR         R8, [R4, R9] 
    /*000003a8*/ B           VEDU_UnRegisterVpu_x370
    VEDU_UnRegisterVpu_x3ac:
    /*000003ac*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb4] 
    /*000003b0*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd8] 
    /*000003b4*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb4] 
    /*000003b8*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd8] 
    /*000003bc*/ MOV         R2, #207                    @ 0xcf 
    /*000003c0*/ LDR         R1, [PC, #16]               @ 0x00000000000003d8 
    /*000003c4*/ BL          0x00000000000003C4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000003c8*/ LDR         R3, [R4, #8] 
    /*000003cc*/ CMP         R3, #0 
    /*000003d0*/ POPEQ       {R4, R5, R6, R7, R8, R9, SL, PC} 
    /*000003d4*/ B           VEDU_UnRegisterVpu_x2c4
    /*000003d8*/ ANDEQ       R0, R0, R4, LSL R0 
FUNC_END VEDU_UnRegisterVpu


FUNC_BEGIN VEDU_Init
    /*000003dc*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000003e0*/ SUB         SP, SP, #12 
    /*000003e4*/ BL          0x0000000000000020 
    /*000003e8*/ CMP         R0, #0 
    /*000003ec*/ MOVWNE      R3, #262                    @ 0x106 
    /*000003f0*/ MOVWNE      R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x84] 
    /*000003f4*/ STRNE       R3, [SP] 
    /*000003f8*/ MOVTNE      R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x84] 
    /*000003fc*/ BNE         VEDU_Init_x430
    /*00000400*/ MOVW        R0, #0                      @ stVdeuDecSem [R_ARM_MOVW_ABS_NC: .bss+0x28] 
    /*00000404*/ MOV         R1, #1 
    /*00000408*/ MOVT        R0, #0                      @ stVdeuDecSem [R_ARM_MOVT_ABS: .bss+0x28] 
    /*0000040c*/ BL          0x000000000000040C          @ osal_sema_init [R_ARM_CALL: +0x0] 
    /*00000410*/ CMP         R0, #0 
    /*00000414*/ BNE         VEDU_Init_x420
    VEDU_Init_x418:
    /*00000418*/ ADD         SP, SP, #12 
    /*0000041c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    VEDU_Init_x420:
    /*00000420*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15c] 
    /*00000424*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15c] 
    /*00000428*/ MOV         R3, #272                    @ 0x110 
    /*0000042c*/ STR         R3, [SP] 
    VEDU_Init_x430:
    /*00000430*/ LDR         R3, [PC, #16]               @ 0x0000000000000448 
    /*00000434*/ MOV         R1, #25 
    /*00000438*/ MOV         R0, #3 
    /*0000043c*/ BL          0x000000000000043C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000440*/ MVN         R0, #0 
    /*00000444*/ B           VEDU_Init_x418
    /*00000448*/ ANDEQ       R0, R0, R8, LSR #32 
FUNC_END VEDU_Init


FUNC_BEGIN VEDU_Exit
    /*0000044c*/ PUSH        {R4, LR} 
    /*00000450*/ BL          0x00000000000002AC 
    /*00000454*/ MOVW        R0, #0                      @ stVdeuDecSem [R_ARM_MOVW_ABS_NC: .bss+0x28] 
    /*00000458*/ POP         {R4, LR} 
    /*0000045c*/ MOVT        R0, #0                      @ stVdeuDecSem [R_ARM_MOVT_ABS: .bss+0x28] 
    VEDU_Exit_x460:
    /*00000460*/ B           VEDU_Exit_x460              @ osal_sema_destory [R_ARM_JUMP24: +0x0] 
FUNC_END VEDU_Exit


FUNC_BEGIN JPGE_Init
    /*00000464*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00000468*/ MOVW        R4, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000046c*/ MOVT        R4, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000470*/ SUB         SP, SP, #116                @ 0x74 
    /*00000474*/ LDR         R5, [R4, #16] 
    /*00000478*/ CMP         R5, #0 
    /*0000047c*/ BEQ         JPGE_Init_x4ac
    /*00000480*/ MOVW        R3, #337                    @ 0x151 
    /*00000484*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x190] 
    /*00000488*/ STR         R3, [SP] 
    /*0000048c*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x190] 
    /*00000490*/ LDR         R3, [PC, #524]              @ 0x00000000000006a4 
    /*00000494*/ MOV         R1, #25 
    /*00000498*/ MOV         R0, #3 
    /*0000049c*/ BL          0x000000000000049C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000004a0*/ MOV         R0, #0 
    JPGE_Init_x4a4:
    /*000004a4*/ ADD         SP, SP, #116                @ 0x74 
    /*000004a8*/ POP         {R4, R5, R6, R7, PC} 
    JPGE_Init_x4ac:
    /*000004ac*/ LDR         R3, [R4, #20] 
    /*000004b0*/ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000004b4*/ MOVT        R6, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000004b8*/ MOV         R2, #1 
    /*000004bc*/ CMP         R3, #0 
    /*000004c0*/ STR         R2, [R6, #8] 
    /*000004c4*/ BEQ         JPGE_Init_x640
    JPGE_Init_x4c8:
    /*000004c8*/ LDR         IP, [R4, #24] 
    /*000004cc*/ ADD         R2, SP, #28 
    /*000004d0*/ MOV         LR, #16 
    /*000004d4*/ MOV         R5, #0 
    /*000004d8*/ CMP         IP, #0 
    /*000004dc*/ MOV         R7, #1 
    /*000004e0*/ MOV         R1, LR 
    /*000004e4*/ MOV         R0, R2 
    /*000004e8*/ STR         R3, [SP, #56]               @ 0x38 
    /*000004ec*/ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f8] 
    /*000004f0*/ STR         R2, [SP, #24] 
    /*000004f4*/ MOVT        R3, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f8] 
    /*000004f8*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x200] 
    /*000004fc*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x200] 
    /*00000500*/ MOVEQ       IP, #68                     @ 0x44 
    /*00000504*/ STR         LR, [SP, #20] 
    /*00000508*/ STR         IP, [SP, #48]               @ 0x30 
    /*0000050c*/ MOVW        LR, #0                      @ JPGE_ClearErrInt [R_ARM_MOVW_ABS_NC: .text+0x1014] 
    /*00000510*/ MOVW        IP, #0                      @ JPGE_ReadInt [R_ARM_MOVW_ABS_NC: .text+0x1000] 
    /*00000514*/ MOVT        LR, #0                      @ JPGE_ClearErrInt [R_ARM_MOVT_ABS: .text+0x1014] 
    /*00000518*/ MOVT        IP, #0                      @ JPGE_ReadInt [R_ARM_MOVT_ABS: .text+0x1000] 
    /*0000051c*/ STR         LR, [SP, #68]               @ 0x44 
    /*00000520*/ STR         IP, [SP, #60]               @ 0x3c 
    /*00000524*/ MOVW        IP, #0                      @ JPGE_ClearInt [R_ARM_MOVW_ABS_NC: .text+0x1004] 
    /*00000528*/ STR         R7, [SP, #16] 
    /*0000052c*/ MOVT        IP, #0                      @ JPGE_ClearInt [R_ARM_MOVT_ABS: .text+0x1004] 
    /*00000530*/ STR         R7, [SP, #52]               @ 0x34 
    /*00000534*/ STR         IP, [SP, #64]               @ 0x40 
    /*00000538*/ MOVW        IP, #0                      @ JPGE_SetIntMask [R_ARM_MOVW_ABS_NC: .text+0x1028] 
    /*0000053c*/ STR         R5, [SP, #44]               @ 0x2c 
    /*00000540*/ MOVT        IP, #0                      @ JPGE_SetIntMask [R_ARM_MOVT_ABS: .text+0x1028] 
    /*00000544*/ STR         R5, [SP, #108]              @ 0x6c 
    /*00000548*/ STR         IP, [SP, #72]               @ 0x48 
    /*0000054c*/ MOVW        IP, #0                      @ JPGE_GetIsIntOk [R_ARM_MOVW_ABS_NC: .text+0x1024] 
    /*00000550*/ MOVT        IP, #0                      @ JPGE_GetIsIntOk [R_ARM_MOVT_ABS: .text+0x1024] 
    /*00000554*/ STR         IP, [SP, #76]               @ 0x4c 
    /*00000558*/ MOVW        IP, #0                      @ JPGE_SetTimeOut [R_ARM_MOVW_ABS_NC: .text+0x1130] 
    /*0000055c*/ MOVT        IP, #0                      @ JPGE_SetTimeOut [R_ARM_MOVT_ABS: .text+0x1130] 
    /*00000560*/ STR         IP, [SP, #80]               @ 0x50 
    /*00000564*/ MOVW        IP, #0                      @ JPGE_GetTimeOut [R_ARM_MOVW_ABS_NC: .text+0x1138] 
    /*00000568*/ MOVT        IP, #0                      @ JPGE_GetTimeOut [R_ARM_MOVT_ABS: .text+0x1138] 
    /*0000056c*/ STR         IP, [SP, #84]               @ 0x54 
    /*00000570*/ MOVW        IP, #0                      @ JPGE_IsNeedReset [R_ARM_MOVW_ABS_NC: .text+0x10b0] 
    /*00000574*/ MOVT        IP, #0                      @ JPGE_IsNeedReset [R_ARM_MOVT_ABS: .text+0x10b0] 
    /*00000578*/ STR         IP, [SP, #88]               @ 0x58 
    /*0000057c*/ MOVW        IP, #0                      @ JPGE_Reset [R_ARM_MOVW_ABS_NC: .text+0x10b4] 
    /*00000580*/ MOVT        IP, #0                      @ JPGE_Reset [R_ARM_MOVT_ABS: .text+0x10b4] 
    /*00000584*/ STR         IP, [SP, #92]               @ 0x5c 
    /*00000588*/ MOVW        IP, #0                      @ JPGE_SetClk [R_ARM_MOVW_ABS_NC: .text+0x1030] 
    /*0000058c*/ MOVT        IP, #0                      @ JPGE_SetClk [R_ARM_MOVT_ABS: .text+0x1030] 
    /*00000590*/ STR         IP, [SP, #96]               @ 0x60 
    /*00000594*/ MOVW        IP, #0                      @ JPGE_StartVpu [R_ARM_MOVW_ABS_NC: .text+0x1114] 
    /*00000598*/ MOVT        IP, #0                      @ JPGE_StartVpu [R_ARM_MOVT_ABS: .text+0x1114] 
    /*0000059c*/ STR         IP, [SP, #100]              @ 0x64 
    /*000005a0*/ MOVW        IP, #0                      @ JPGE_OutputDebugInfo [R_ARM_MOVW_ABS_NC: .text+0x1140] 
    /*000005a4*/ MOVT        IP, #0                      @ JPGE_OutputDebugInfo [R_ARM_MOVT_ABS: .text+0x1140] 
    /*000005a8*/ STR         IP, [SP, #104]              @ 0x68 
    /*000005ac*/ MOV         IP, #80                     @ 0x50 
    /*000005b0*/ STR         IP, [SP, #12] 
    /*000005b4*/ BL          0x00000000000005B4          @ osal_snprintf [R_ARM_CALL: +0x0] 
    /*000005b8*/ MOV         R1, R7 
    /*000005bc*/ MOV         R0, R5 
    /*000005c0*/ BL          0x00000000000005C0          @ JPGE_SetClk [R_ARM_CALL: .text+0x1030] 
    /*000005c4*/ MOV         R1, R5 
    /*000005c8*/ MOV         R0, R5 
    /*000005cc*/ BL          0x00000000000005CC          @ JPGE_RstSel [R_ARM_CALL: .text+0x1070] 
    /*000005d0*/ LDR         R0, [R4, #20] 
    /*000005d4*/ BL          0x00000000000005D4          @ JPGE_HAL_Init [R_ARM_CALL: .text+0xf6c] 
    /*000005d8*/ MOV         R0, #4 
    /*000005dc*/ BL          0x00000000000005DC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000005e0*/ LDR         R3, [R0, #8] 
    /*000005e4*/ ADD         R0, SP, #12 
    /*000005e8*/ BLX         R3 
    /*000005ec*/ CMP         R0, #0 
    /*000005f0*/ STRNE       R0, [R4, #16] 
    /*000005f4*/ MOVNE       R0, R5 
    /*000005f8*/ BNE         JPGE_Init_x4a4
    /*000005fc*/ MOV         R3, #392                    @ 0x188 
    /*00000600*/ MOVW        R2, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x204] 
    /*00000604*/ STR         R3, [SP] 
    /*00000608*/ MOVT        R2, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x204] 
    /*0000060c*/ LDR         R3, [PC, #144]              @ 0x00000000000006a4 
    /*00000610*/ MOV         R1, #25 
    /*00000614*/ MOV         R0, #3 
    /*00000618*/ BL          0x0000000000000618          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000061c*/ LDR         R0, [R4, #20] 
    /*00000620*/ CMP         R0, R5 
    /*00000624*/ BEQ         JPGE_Init_x634
    /*00000628*/ LDR         R5, [R6, #8] 
    /*0000062c*/ CMP         R5, #0 
    /*00000630*/ BEQ         JPGE_Init_x690
    JPGE_Init_x634:
    /*00000634*/ MOVW        R0, #32777                  @ 0x8009 
    /*00000638*/ MOVT        R0, #40985                  @ 0xa019 
    /*0000063c*/ B           JPGE_Init_x4a4
    JPGE_Init_x640:
    /*00000640*/ MOV         R0, #0 
    /*00000644*/ MOV         R1, #65536                  @ 0x10000 
    /*00000648*/ MOVT        R0, #4386                   @ 0x1122 
    /*0000064c*/ BL          0x000000000000064C          @ osal_ioremap [R_ARM_CALL: +0x0] 
    /*00000650*/ CMP         R0, #0 
    /*00000654*/ MOV         R3, R0 
    /*00000658*/ STR         R0, [R4, #20] 
    /*0000065c*/ STRNE       R5, [R6, #8] 
    /*00000660*/ BNE         JPGE_Init_x4c8
    /*00000664*/ MOVW        R3, #350                    @ 0x15e 
    /*00000668*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c4] 
    /*0000066c*/ STR         R3, [SP] 
    /*00000670*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c4] 
    /*00000674*/ LDR         R3, [PC, #40]               @ 0x00000000000006a4 
    /*00000678*/ MOV         R1, #25 
    /*0000067c*/ MOV         R0, #3 
    /*00000680*/ BL          0x0000000000000680          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000684*/ MOVW        R0, #32777                  @ 0x8009 
    /*00000688*/ MOVT        R0, #40985                  @ 0xa019 
    /*0000068c*/ B           JPGE_Init_x4a4
    JPGE_Init_x690:
    /*00000690*/ BL          0x0000000000000690          @ osal_iounmap [R_ARM_CALL: +0x0] 
    /*00000694*/ MOVW        R0, #32777                  @ 0x8009 
    /*00000698*/ STR         R5, [R4, #20] 
    /*0000069c*/ MOVT        R0, #40985                  @ 0xa019 
    /*000006a0*/ B           JPGE_Init_x4a4
    /*000006a4*/ ANDEQ       R0, R0, R4, LSR R0 
FUNC_END JPGE_Init


FUNC_BEGIN VPU_Init
    /*000006a8*/ PUSH        {R4, LR} 
    /*000006ac*/ MOV         R0, #4 
    /*000006b0*/ SUB         SP, SP, #8 
    /*000006b4*/ BL          0x00000000000006B4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000006b8*/ CMP         R0, #0 
    /*000006bc*/ BEQ         VPU_Init_x720
    /*000006c0*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000006c4*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000006c8*/ LDR         R3, [R4] 
    /*000006cc*/ CMP         R3, #2 
    /*000006d0*/ BEQ         VPU_Init_x700
    /*000006d4*/ MOV         R3, #440                    @ 0x1b8 
    /*000006d8*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23c] 
    /*000006dc*/ STR         R3, [SP] 
    /*000006e0*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23c] 
    /*000006e4*/ LDR         R3, [PC, #64]               @ 0x000000000000072c 
    /*000006e8*/ MOV         R1, #25 
    /*000006ec*/ MOV         R0, #5 
    /*000006f0*/ BL          0x00000000000006F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000006f4*/ MOV         R0, #0 
    VPU_Init_x6f8:
    /*000006f8*/ ADD         SP, SP, #8 
    /*000006fc*/ POP         {R4, PC} 
    VPU_Init_x700:
    /*00000700*/ BL          0x0000000000000700          @ VEDU_Init [R_ARM_CALL: .text+0x3dc] 
    /*00000704*/ CMP         R0, #0 
    /*00000708*/ BNE         VPU_Init_x6f8
    /*0000070c*/ BL          0x000000000000070C          @ JPGE_Init [R_ARM_CALL: .text+0x464] 
    /*00000710*/ MOV         R3, #0 
    /*00000714*/ STR         R3, [R4] 
    /*00000718*/ ADD         SP, SP, #8 
    /*0000071c*/ POP         {R4, PC} 
    VPU_Init_x720:
    /*00000720*/ MOVW        R0, #32784                  @ 0x8010 
    /*00000724*/ MOVT        R0, #40985                  @ 0xa019 
    /*00000728*/ B           VPU_Init_x6f8
    /*0000072c*/ ANDEQ       R0, R0, R0, ASR #32 
FUNC_END VPU_Init


FUNC_BEGIN JPGE_Exit
    /*00000730*/ PUSH        {R4, R5, R6, LR} 
    /*00000734*/ MOVW        R4, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000738*/ MOVT        R4, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000073c*/ LDR         R3, [R4, #16] 
    /*00000740*/ CMP         R3, #0 
    /*00000744*/ POPEQ       {R4, R5, R6, PC} 
    /*00000748*/ MOV         R0, #4 
    /*0000074c*/ BL          0x000000000000074C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000750*/ LDR         R3, [R0, #12] 
    /*00000754*/ LDR         R0, [R4, #16] 
    /*00000758*/ BLX         R3 
    /*0000075c*/ CMP         R0, #0 
    /*00000760*/ BNE         JPGE_Exit_x7a0
    JPGE_Exit_x764:
    /*00000764*/ MOV         R1, #0 
    /*00000768*/ STR         R1, [R4, #16] 
    /*0000076c*/ MOV         R0, R1 
    /*00000770*/ BL          0x0000000000000770          @ JPGE_SetClk [R_ARM_CALL: .text+0x1030] 
    /*00000774*/ LDR         R0, [R4, #20] 
    /*00000778*/ CMP         R0, #0 
    /*0000077c*/ POPEQ       {R4, R5, R6, PC} 
    /*00000780*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000784*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000788*/ LDR         R5, [R3, #8] 
    /*0000078c*/ CMP         R5, #0 
    /*00000790*/ POPNE       {R4, R5, R6, PC} 
    /*00000794*/ BL          0x0000000000000794          @ osal_iounmap [R_ARM_CALL: +0x0] 
    /*00000798*/ STR         R5, [R4, #20] 
    /*0000079c*/ POP         {R4, R5, R6, PC} 
    JPGE_Exit_x7a0:
    /*000007a0*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x118] 
    /*000007a4*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd8] 
    /*000007a8*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x118] 
    /*000007ac*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd8] 
    /*000007b0*/ MOVW        R2, #414                    @ 0x19e 
    /*000007b4*/ LDR         R1, [PC, #4]                @ 0x00000000000007c0 
    /*000007b8*/ BL          0x00000000000007B8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000007bc*/ B           JPGE_Exit_x764
    /*000007c0*/ ANDEQ       R0, R0, IP, ASR #32 
FUNC_END JPGE_Exit


FUNC_BEGIN VPU_Exit
    /*000007c4*/ PUSH        {R4, LR} 
    /*000007c8*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000007cc*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000007d0*/ SUB         SP, SP, #8 
    /*000007d4*/ LDR         R3, [R4] 
    /*000007d8*/ CMP         R3, #2 
    /*000007dc*/ BEQ         VPU_Exit_x804
    /*000007e0*/ BL          0x00000000000002AC 
    /*000007e4*/ MOVW        R0, #0                      @ stVdeuDecSem [R_ARM_MOVW_ABS_NC: .bss+0x28] 
    /*000007e8*/ MOVT        R0, #0                      @ stVdeuDecSem [R_ARM_MOVT_ABS: .bss+0x28] 
    /*000007ec*/ BL          0x00000000000007EC          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*000007f0*/ BL          0x00000000000007F0          @ JPGE_Exit [R_ARM_CALL: .text+0x730] 
    /*000007f4*/ MOV         R3, #2 
    /*000007f8*/ STR         R3, [R4] 
    /*000007fc*/ ADD         SP, SP, #8 
    /*00000800*/ POP         {R4, PC} 
    VPU_Exit_x804:
    /*00000804*/ MOVW        R3, #463                    @ 0x1cf 
    /*00000808*/ MOV         R1, #25 
    /*0000080c*/ STR         R3, [SP] 
    /*00000810*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x26c] 
    /*00000814*/ LDR         R3, [PC, #16]               @ 0x000000000000082c 
    /*00000818*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x26c] 
    /*0000081c*/ MOV         R0, #5 
    /*00000820*/ BL          0x0000000000000820          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000824*/ ADD         SP, SP, #8 
    /*00000828*/ POP         {R4, PC} 
    /*0000082c*/ ANDEQ       R0, R0, R8, ASR R0 
FUNC_END VPU_Exit


FUNC_BEGIN VPU_QueryVeduState
    /*00000830*/ PUSH        {R4, R5, LR} 
    /*00000834*/ SUB         SP, SP, #20 
    /*00000838*/ MOV         R5, R0 
    /*0000083c*/ MOV         R0, #4 
    /*00000840*/ MOV         R4, #0 
    /*00000844*/ STR         R4, [SP, #12] 
    /*00000848*/ BL          0x0000000000000848          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000084c*/ LDR         R3, [R0, #36]               @ 0x24 
    /*00000850*/ CMP         R3, R4 
    /*00000854*/ BEQ         VPU_QueryVeduState_x890
    /*00000858*/ MOV         R0, #4 
    /*0000085c*/ BL          0x000000000000085C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000860*/ ADD         R1, SP, #12 
    /*00000864*/ LDR         R3, [R0, #36]               @ 0x24 
    /*00000868*/ MOV         R0, #7 
    /*0000086c*/ BLX         R3 
    /*00000870*/ LDR         R0, [SP, #12] 
    /*00000874*/ CMP         R0, #0 
    /*00000878*/ MOVNE       R0, R4 
    /*0000087c*/ MOVEQ       R3, #17 
    /*00000880*/ MOVNE       R3, #34                     @ 0x22 
    /*00000884*/ STR         R3, [R5] 
    VPU_QueryVeduState_x888:
    /*00000888*/ ADD         SP, SP, #20 
    /*0000088c*/ POP         {R4, R5, PC} 
    VPU_QueryVeduState_x890:
    /*00000890*/ MOVW        R3, #555                    @ 0x22b 
    /*00000894*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*00000898*/ STR         R3, [SP] 
    /*0000089c*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c] 
    /*000008a0*/ LDR         R3, [PC, #24]               @ 0x00000000000008c0 
    /*000008a4*/ MOV         R1, #25 
    /*000008a8*/ MOV         R0, #3 
    /*000008ac*/ BL          0x00000000000008AC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000008b0*/ MOV         R3, #17 
    /*000008b4*/ MVN         R0, #0 
    /*000008b8*/ STR         R3, [R5] 
    /*000008bc*/ B           VPU_QueryVeduState_x888
    /*000008c0*/ ANDEQ       R0, R0, R4, RRX 
FUNC_END VPU_QueryVeduState


FUNC_BEGIN VPU_QueryJpgeState
    /*000008c4*/ PUSH        {R4, R5, LR} 
    /*000008c8*/ SUB         SP, SP, #20 
    /*000008cc*/ MOV         R5, R0 
    /*000008d0*/ MOV         R0, #4 
    /*000008d4*/ MOV         R4, #0 
    /*000008d8*/ STR         R4, [SP, #12] 
    /*000008dc*/ BL          0x00000000000008DC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000008e0*/ LDR         R3, [R0, #36]               @ 0x24 
    /*000008e4*/ CMP         R3, R4 
    /*000008e8*/ BEQ         VPU_QueryJpgeState_x924
    /*000008ec*/ MOV         R0, #4 
    /*000008f0*/ BL          0x00000000000008F0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000008f4*/ ADD         R1, SP, #12 
    /*000008f8*/ LDR         R3, [R0, #36]               @ 0x24 
    /*000008fc*/ MOV         R0, #80                     @ 0x50 
    /*00000900*/ BLX         R3 
    /*00000904*/ LDR         R0, [SP, #12] 
    /*00000908*/ CMP         R0, #0 
    /*0000090c*/ MOVNE       R0, R4 
    /*00000910*/ MOVEQ       R3, #17 
    /*00000914*/ MOVNE       R3, #34                     @ 0x22 
    /*00000918*/ STR         R3, [R5] 
    VPU_QueryJpgeState_x91c:
    /*0000091c*/ ADD         SP, SP, #20 
    /*00000920*/ POP         {R4, R5, PC} 
    VPU_QueryJpgeState_x924:
    /*00000924*/ MOVW        R3, #585                    @ 0x249 
    /*00000928*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*0000092c*/ STR         R3, [SP] 
    /*00000930*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c] 
    /*00000934*/ LDR         R3, [PC, #24]               @ 0x0000000000000954 
    /*00000938*/ MOV         R1, #25 
    /*0000093c*/ MOV         R0, #3 
    /*00000940*/ BL          0x0000000000000940          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000944*/ MOV         R3, #17 
    /*00000948*/ MVN         R0, #0 
    /*0000094c*/ STR         R3, [R5] 
    /*00000950*/ B           VPU_QueryJpgeState_x91c
    /*00000954*/ ANDEQ       R0, R0, R8, ROR R0 
FUNC_END VPU_QueryJpgeState


FUNC_BEGIN VPU_QueryState
    /*00000958*/ PUSH        {R4, LR} 
    /*0000095c*/ SUB         SP, SP, #8 
    /*00000960*/ MOV         R4, R0 
    /*00000964*/ MOV         R0, SP 
    /*00000968*/ BL          0x0000000000000968          @ VPU_QueryVeduState [R_ARM_CALL: .text+0x830] 
    /*0000096c*/ ADD         R0, SP, #4 
    /*00000970*/ BL          0x0000000000000970          @ VPU_QueryJpgeState [R_ARM_CALL: .text+0x8c4] 
    /*00000974*/ LDR         R3, [SP] 
    /*00000978*/ CMP         R3, #17 
    /*0000097c*/ BEQ         VPU_QueryState_x990
    VPU_QueryState_x980:
    /*00000980*/ MOV         R3, #34                     @ 0x22 
    VPU_QueryState_x984:
    /*00000984*/ STR         R3, [R4] 
    /*00000988*/ ADD         SP, SP, #8 
    /*0000098c*/ POP         {R4, PC} 
    VPU_QueryState_x990:
    /*00000990*/ LDR         R3, [SP, #4] 
    /*00000994*/ CMP         R3, #17 
    /*00000998*/ BNE         VPU_QueryState_x980
    /*0000099c*/ B           VPU_QueryState_x984
FUNC_END VPU_QueryState


FUNC_BEGIN VPU_ModInit
    /*000009a0*/ PUSH        {R4, LR} 
    /*000009a4*/ SUB         SP, SP, #8 
    /*000009a8*/ LDR         R0, [PC, #80]               @ 0x0000000000000a00 
    /*000009ac*/ BL          0x00000000000009AC          @ CMPI_RegisterModule [R_ARM_CALL: +0x0] 
    /*000009b0*/ SUBS        R4, R0, #0 
    /*000009b4*/ BNE         VPU_ModInit_x9d8
    /*000009b8*/ MOVW        R1, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30c] 
    /*000009bc*/ MOVW        R0, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x318] 
    /*000009c0*/ MOVT        R1, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30c] 
    /*000009c4*/ MOVT        R0, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x318] 
    /*000009c8*/ BL          0x00000000000009C8          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000009cc*/ MOV         R0, R4 
    VPU_ModInit_x9d0:
    /*000009d0*/ ADD         SP, SP, #8 
    /*000009d4*/ POP         {R4, PC} 
    VPU_ModInit_x9d8:
    /*000009d8*/ MOVW        R3, #705                    @ 0x2c1 
    /*000009dc*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2d8] 
    /*000009e0*/ STR         R3, [SP] 
    /*000009e4*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2d8] 
    /*000009e8*/ LDR         R3, [PC, #20]               @ 0x0000000000000a04 
    /*000009ec*/ MOV         R1, #25 
    /*000009f0*/ MOV         R0, #3 
    /*000009f4*/ BL          0x00000000000009F4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000009f8*/ MVN         R0, #0 
    /*000009fc*/ B           VPU_ModInit_x9d0
    /*00000a00*/ ANDEQ       R0, R0, IP 
    /*00000a04*/ ANDEQ       R0, R0, IP, LSL #1 
FUNC_END VPU_ModInit


FUNC_BEGIN VPU_ModExit
    /*00000a08*/ PUSH        {R4, LR} 
    /*00000a0c*/ MOV         R0, #25 
    /*00000a10*/ BL          0x0000000000000A10          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000a14*/ CMP         R0, #0 
    /*00000a18*/ BEQ         VPU_ModExit_xa3c
    /*00000a1c*/ MOV         R0, #25 
    /*00000a20*/ BL          0x0000000000000A20          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    /*00000a24*/ MOVW        R1, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30c] 
    /*00000a28*/ MOVW        R0, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x36c] 
    /*00000a2c*/ MOVT        R1, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30c] 
    /*00000a30*/ MOVT        R0, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x36c] 
    /*00000a34*/ POP         {R4, LR} 
    VPU_ModExit_xa38:
    /*00000a38*/ B           VPU_ModExit_xa38            @ osal_printk [R_ARM_JUMP24: +0x0] 
    VPU_ModExit_xa3c:
    /*00000a3c*/ MOVW        R0, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x334] 
    /*00000a40*/ MOVT        R0, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x334] 
    /*00000a44*/ POP         {R4, LR} 
    VPU_ModExit_xa48:
    /*00000a48*/ B           VPU_ModExit_xa48            @ osal_printk [R_ARM_JUMP24: +0x0] 
FUNC_END VPU_ModExit


FUNC_BEGIN VEDU_GetRegAddr.part.0
    /*00000a4c*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x38c] 
    /*00000a50*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x98] 
    /*00000a54*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3a0] 
    /*00000a58*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x38c] 
    /*00000a5c*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x98] 
    /*00000a60*/ MOV         R2, #58                     @ 0x3a 
    /*00000a64*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3a0] 
    VEDU_GetRegAddr.part.0_xa68:
    /*00000a68*/ B           VEDU_GetRegAddr.part.0_xa68 @ osal_panic [R_ARM_JUMP24: +0x0] 
FUNC_END VEDU_GetRegAddr.part.0


FUNC_BEGIN JPGE_GetRegAddr.part.1
    /*00000a6c*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3e0] 
    /*00000a70*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3a0] 
    /*00000a74*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3e0] 
    /*00000a78*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3a0] 
    /*00000a7c*/ MOVW        R2, #621                    @ 0x26d 
    /*00000a80*/ LDR         R1, [PC]                    @ 0x0000000000000a88 
    JPGE_GetRegAddr.part.1_xa84:
    /*00000a84*/ B           JPGE_GetRegAddr.part.1_xa84 @ osal_panic [R_ARM_JUMP24: +0x0] 
    /*00000a88*/ ANDEQ       R0, R0, R8, LSR #1 
FUNC_END JPGE_GetRegAddr.part.1


FUNC_BEGIN VEDU_GetRegAddr
    /*00000a8c*/ MOVW        R3, #0                      @ s_apVeduAddr [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000a90*/ MOVT        R3, #0                      @ s_apVeduAddr [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000a94*/ LDR         R0, [R3, R0, LSL #2] 
    /*00000a98*/ CMP         R0, #0 
    /*00000a9c*/ BXNE        LR 
    /*00000aa0*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000aa4*/ SUB         SP, SP, #12 
    /*00000aa8*/ STR         R0, [SP, #4] 
    /*00000aac*/ BL          0x0000000000000A4C 
    /*00000ab0*/ LDR         R0, [SP, #4] 
    /*00000ab4*/ ADD         SP, SP, #12 
    /*00000ab8*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END VEDU_GetRegAddr


FUNC_BEGIN VEDU_ReadInt
    /*00000abc*/ MOVW        R3, #0                      @ s_apVeduAddr [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000ac0*/ MOVT        R3, #0                      @ s_apVeduAddr [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000ac4*/ LDR         R0, [R3, R0, LSL #2] 
    /*00000ac8*/ CMP         R0, #0 
    /*00000acc*/ BEQ         VEDU_ReadInt_xad4
    VEDU_ReadInt_xad0:
    /*00000ad0*/ B           VEDU_ReadInt_xad0           @ VEDU_DRV_ReadInt [R_ARM_JUMP24: .text+0x19b4] 
    VEDU_ReadInt_xad4:
    /*00000ad4*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000ad8*/ SUB         SP, SP, #12 
    /*00000adc*/ STR         R0, [SP, #4] 
    /*00000ae0*/ BL          0x0000000000000A4C 
    /*00000ae4*/ LDR         R0, [SP, #4] 
    /*00000ae8*/ ADD         SP, SP, #12 
    /*00000aec*/ POP         {LR}                        @ (ldr lr, [sp], #4) 
    VEDU_ReadInt_xaf0:
    /*00000af0*/ B           VEDU_ReadInt_xaf0           @ VEDU_DRV_ReadInt [R_ARM_JUMP24: .text+0x19b4] 
FUNC_END VEDU_ReadInt


FUNC_BEGIN VEDU_ClearInt
    /*00000af4*/ MOVW        R3, #0                      @ s_apVeduAddr [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000af8*/ MOVT        R3, #0                      @ s_apVeduAddr [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000afc*/ LDR         R0, [R3, R0, LSL #2] 
    /*00000b00*/ CMP         R0, #0 
    /*00000b04*/ BEQ         VEDU_ClearInt_xb0c
    VEDU_ClearInt_xb08:
    /*00000b08*/ B           VEDU_ClearInt_xb08          @ VEDU_DRV_ClearInt [R_ARM_JUMP24: .text+0x1a3c] 
    VEDU_ClearInt_xb0c:
    /*00000b0c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000b10*/ SUB         SP, SP, #12 
    /*00000b14*/ STR         R0, [SP, #4] 
    /*00000b18*/ BL          0x0000000000000A4C 
    /*00000b1c*/ LDR         R0, [SP, #4] 
    /*00000b20*/ ADD         SP, SP, #12 
    /*00000b24*/ POP         {LR}                        @ (ldr lr, [sp], #4) 
    VEDU_ClearInt_xb28:
    /*00000b28*/ B           VEDU_ClearInt_xb28          @ VEDU_DRV_ClearInt [R_ARM_JUMP24: .text+0x1a3c] 
FUNC_END VEDU_ClearInt


FUNC_BEGIN VEDU_ClearErrInt
    /*00000b2c*/ MOVW        R3, #0                      @ s_apVeduAddr [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000b30*/ MOVT        R3, #0                      @ s_apVeduAddr [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000b34*/ LDR         R0, [R3, R0, LSL #2] 
    /*00000b38*/ CMP         R0, #0 
    /*00000b3c*/ BEQ         VEDU_ClearErrInt_xb44
    VEDU_ClearErrInt_xb40:
    /*00000b40*/ B           VEDU_ClearErrInt_xb40       @ VEDU_DRV_ClearErrInt [R_ARM_JUMP24: .text+0x1a88] 
    VEDU_ClearErrInt_xb44:
    /*00000b44*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000b48*/ SUB         SP, SP, #12 
    /*00000b4c*/ STR         R0, [SP, #4] 
    /*00000b50*/ BL          0x0000000000000A4C 
    /*00000b54*/ LDR         R0, [SP, #4] 
    /*00000b58*/ ADD         SP, SP, #12 
    /*00000b5c*/ POP         {LR}                        @ (ldr lr, [sp], #4) 
    VEDU_ClearErrInt_xb60:
    /*00000b60*/ B           VEDU_ClearErrInt_xb60       @ VEDU_DRV_ClearErrInt [R_ARM_JUMP24: .text+0x1a88] 
FUNC_END VEDU_ClearErrInt


FUNC_BEGIN VEDU_SetIntMask
    /*00000b64*/ MOV         R0, #0 
    /*00000b68*/ BX          LR 
FUNC_END VEDU_SetIntMask


FUNC_BEGIN VEDU_GetIsIntOk
    /*00000b6c*/ MOVW        R3, #0                      @ s_apVeduAddr [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000b70*/ MOVT        R3, #0                      @ s_apVeduAddr [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000b74*/ LDR         R0, [R3, R0, LSL #2] 
    /*00000b78*/ CMP         R0, #0 
    /*00000b7c*/ BEQ         VEDU_GetIsIntOk_xb84
    VEDU_GetIsIntOk_xb80:
    /*00000b80*/ B           VEDU_GetIsIntOk_xb80        @ VEDU_DRV_IsIntOk [R_ARM_JUMP24: .text+0x1ad4] 
    VEDU_GetIsIntOk_xb84:
    /*00000b84*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000b88*/ SUB         SP, SP, #12 
    /*00000b8c*/ STR         R0, [SP, #4] 
    /*00000b90*/ BL          0x0000000000000A4C 
    /*00000b94*/ LDR         R0, [SP, #4] 
    /*00000b98*/ ADD         SP, SP, #12 
    /*00000b9c*/ POP         {LR}                        @ (ldr lr, [sp], #4) 
    VEDU_GetIsIntOk_xba0:
    /*00000ba0*/ B           VEDU_GetIsIntOk_xba0        @ VEDU_DRV_IsIntOk [R_ARM_JUMP24: .text+0x1ad4] 
FUNC_END VEDU_GetIsIntOk


FUNC_BEGIN VEDU_SelClk
    /*00000ba4*/ PUSH        {R4, LR} 
    /*00000ba8*/ SUB         SP, SP, #16 
    /*00000bac*/ MOV         R4, #0 
    /*00000bb0*/ MOV         R3, #25 
    /*00000bb4*/ STR         R0, [SP, #8] 
    /*00000bb8*/ MOV         R0, #2 
    /*00000bbc*/ STR         R3, [SP, #4] 
    /*00000bc0*/ STR         R4, [SP, #12] 
    /*00000bc4*/ BL          0x0000000000000BC4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000bc8*/ MOV         R2, R4 
    /*00000bcc*/ MOV         R1, #118                    @ 0x76 
    /*00000bd0*/ LDR         R3, [R0, #20] 
    /*00000bd4*/ ADD         R0, SP, #4 
    /*00000bd8*/ BLX         R3 
    /*00000bdc*/ MOV         R0, R4 
    /*00000be0*/ ADD         SP, SP, #16 
    /*00000be4*/ POP         {R4, PC} 
FUNC_END VEDU_SelClk


FUNC_BEGIN VEDU_SetVpuClk
    /*00000be8*/ PUSH        {R4, LR} 
    /*00000bec*/ SUB         SP, SP, #24 
    /*00000bf0*/ MOV         R3, #25 
    /*00000bf4*/ MOV         R4, #0 
    /*00000bf8*/ STR         R0, [SP, #16] 
    /*00000bfc*/ MOV         R0, #2 
    /*00000c00*/ STR         R1, [SP, #4] 
    /*00000c04*/ STR         R3, [SP, #12] 
    /*00000c08*/ STR         R4, [SP, #20] 
    /*00000c0c*/ BL          0x0000000000000C0C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000c10*/ ADD         R2, SP, #4 
    /*00000c14*/ MOV         R1, #117                    @ 0x75 
    /*00000c18*/ LDR         R3, [R0, #20] 
    /*00000c1c*/ ADD         R0, SP, #12 
    /*00000c20*/ BLX         R3 
    /*00000c24*/ MOV         R0, #2 
    /*00000c28*/ BL          0x0000000000000C28          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000c2c*/ ADD         R2, SP, #4 
    /*00000c30*/ MOV         R1, #120                    @ 0x78 
    /*00000c34*/ LDR         R3, [R0, #20] 
    /*00000c38*/ ADD         R0, SP, #12 
    /*00000c3c*/ BLX         R3 
    /*00000c40*/ MOV         R0, R4 
    /*00000c44*/ ADD         SP, SP, #24 
    /*00000c48*/ POP         {R4, PC} 
FUNC_END VEDU_SetVpuClk


FUNC_BEGIN VEDU_SetClk
    /*00000c4c*/ PUSH        {R4, LR} 
    /*00000c50*/ SUB         SP, SP, #24 
    /*00000c54*/ MOV         R3, #25 
    /*00000c58*/ MOV         R4, #0 
    /*00000c5c*/ STR         R0, [SP, #16] 
    /*00000c60*/ MOV         R0, #2 
    /*00000c64*/ STR         R1, [SP, #4] 
    /*00000c68*/ STR         R3, [SP, #12] 
    /*00000c6c*/ STR         R4, [SP, #20] 
    /*00000c70*/ BL          0x0000000000000C70          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000c74*/ ADD         R2, SP, #4 
    /*00000c78*/ MOV         R1, #117                    @ 0x75 
    /*00000c7c*/ LDR         R3, [R0, #20] 
    /*00000c80*/ ADD         R0, SP, #12 
    /*00000c84*/ BLX         R3 
    /*00000c88*/ MOV         R0, R4 
    /*00000c8c*/ ADD         SP, SP, #24 
    /*00000c90*/ POP         {R4, PC} 
FUNC_END VEDU_SetClk


FUNC_BEGIN VEDU_RstSel
    /*00000c94*/ PUSH        {R4, LR} 
    /*00000c98*/ SUB         SP, SP, #24 
    /*00000c9c*/ MOV         R3, #25 
    /*00000ca0*/ MOV         R4, #0 
    /*00000ca4*/ STR         R0, [SP, #16] 
    /*00000ca8*/ MOV         R0, #2 
    /*00000cac*/ STR         R1, [SP, #4] 
    /*00000cb0*/ STR         R3, [SP, #12] 
    /*00000cb4*/ STR         R4, [SP, #20] 
    /*00000cb8*/ BL          0x0000000000000CB8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000cbc*/ ADD         R2, SP, #4 
    /*00000cc0*/ MOV         R1, #116                    @ 0x74 
    /*00000cc4*/ LDR         R3, [R0, #20] 
    /*00000cc8*/ ADD         R0, SP, #12 
    /*00000ccc*/ BLX         R3 
    /*00000cd0*/ MOV         R0, R4 
    /*00000cd4*/ ADD         SP, SP, #24 
    /*00000cd8*/ POP         {R4, PC} 
FUNC_END VEDU_RstSel


FUNC_BEGIN VEDU_SetSedClk
    /*00000cdc*/ PUSH        {R4, LR} 
    /*00000ce0*/ SUB         SP, SP, #24 
    /*00000ce4*/ MOV         R3, #25 
    /*00000ce8*/ MOV         R4, #0 
    /*00000cec*/ STR         R0, [SP, #16] 
    /*00000cf0*/ MOV         R0, #2 
    /*00000cf4*/ STR         R1, [SP, #4] 
    /*00000cf8*/ STR         R3, [SP, #12] 
    /*00000cfc*/ STR         R4, [SP, #20] 
    /*00000d00*/ BL          0x0000000000000D00          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000d04*/ ADD         R2, SP, #4 
    /*00000d08*/ MOV         R1, #120                    @ 0x78 
    /*00000d0c*/ LDR         R3, [R0, #20] 
    /*00000d10*/ ADD         R0, SP, #12 
    /*00000d14*/ BLX         R3 
    /*00000d18*/ MOV         R0, R4 
    /*00000d1c*/ ADD         SP, SP, #24 
    /*00000d20*/ POP         {R4, PC} 
FUNC_END VEDU_SetSedClk


FUNC_BEGIN VEDU_IsNeedReset
    /*00000d24*/ MOVW        R3, #0                      @ s_apVeduAddr [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000d28*/ MOVT        R3, #0                      @ s_apVeduAddr [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000d2c*/ LDR         R0, [R3, R0, LSL #2] 
    /*00000d30*/ CMP         R0, #0 
    /*00000d34*/ BEQ         VEDU_IsNeedReset_xd3c
    VEDU_IsNeedReset_xd38:
    /*00000d38*/ B           VEDU_IsNeedReset_xd38       @ VEDU_DRV_IsNeedReset [R_ARM_JUMP24: .text+0x1b74] 
    VEDU_IsNeedReset_xd3c:
    /*00000d3c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000d40*/ SUB         SP, SP, #12 
    /*00000d44*/ STR         R0, [SP, #4] 
    /*00000d48*/ BL          0x0000000000000A4C 
    /*00000d4c*/ LDR         R0, [SP, #4] 
    /*00000d50*/ ADD         SP, SP, #12 
    /*00000d54*/ POP         {LR}                        @ (ldr lr, [sp], #4) 
    VEDU_IsNeedReset_xd58:
    /*00000d58*/ B           VEDU_IsNeedReset_xd58       @ VEDU_DRV_IsNeedReset [R_ARM_JUMP24: .text+0x1b74] 
FUNC_END VEDU_IsNeedReset


FUNC_BEGIN VEDU_SetTimeOut
    /*00000d5c*/ MOV         R0, #0 
    /*00000d60*/ BX          LR 
FUNC_END VEDU_SetTimeOut


FUNC_BEGIN VEDU_GetTimeOut
    /*00000d64*/ MOV         R0, #0 
    /*00000d68*/ BX          LR 
FUNC_END VEDU_GetTimeOut


FUNC_BEGIN VEDU_StartVedu
    /*00000d6c*/ MOVW        R3, #0                      @ s_apVeduAddr [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000d70*/ MOVT        R3, #0                      @ s_apVeduAddr [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000d74*/ PUSH        {R4, LR} 
    /*00000d78*/ LDR         R4, [R3, R0, LSL #2] 
    /*00000d7c*/ CMP         R4, #0 
    /*00000d80*/ BEQ         VEDU_StartVedu_xd9c
    VEDU_StartVedu_xd84:
    /*00000d84*/ BL          0x0000000000000D84          @ osal_isb [R_ARM_CALL: +0x0] 
    /*00000d88*/ BL          0x0000000000000D88          @ osal_dsb [R_ARM_CALL: +0x0] 
    /*00000d8c*/ BL          0x0000000000000D8C          @ osal_dmb [R_ARM_CALL: +0x0] 
    /*00000d90*/ MOV         R0, R4 
    /*00000d94*/ POP         {R4, LR} 
    VEDU_StartVedu_xd98:
    /*00000d98*/ B           VEDU_StartVedu_xd98         @ VEDU_DRV_StartVedu [R_ARM_JUMP24: .text+0x1c4c] 
    VEDU_StartVedu_xd9c:
    /*00000d9c*/ BL          0x0000000000000A4C 
    /*00000da0*/ B           VEDU_StartVedu_xd84
FUNC_END VEDU_StartVedu


FUNC_BEGIN VEDU_OutputDebugInfo
    /*00000da4*/ MOVW        R3, #0                      @ s_apVeduAddr [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000da8*/ MOVT        R3, #0                      @ s_apVeduAddr [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000dac*/ PUSH        {R4, R5, R6, LR} 
    /*00000db0*/ SUB         SP, SP, #8 
    /*00000db4*/ LDR         R4, [R3, R0, LSL #2] 
    /*00000db8*/ CMP         R4, #0 
    /*00000dbc*/ BEQ         VEDU_OutputDebugInfo_xe34
    /*00000dc0*/ MOV         R1, R0 
    /*00000dc4*/ MOVW        R0, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x418] 
    /*00000dc8*/ MOVT        R0, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x418] 
    /*00000dcc*/ MOVW        R6, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3f4] 
    /*00000dd0*/ BL          0x0000000000000DD0          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00000dd4*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44c] 
    /*00000dd8*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44c] 
    /*00000ddc*/ MOVT        R6, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3f4] 
    /*00000de0*/ BL          0x0000000000000DE0          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00000de4*/ MOV         R5, #0 
    VEDU_OutputDebugInfo_xde8:
    /*00000de8*/ LDR         IP, [R4, #12] 
    /*00000dec*/ MOV         R1, R5 
    /*00000df0*/ LDR         R3, [R4, #4] 
    /*00000df4*/ ADD         R5, R5, #16 
    /*00000df8*/ LDR         R2, [R4] 
    /*00000dfc*/ MOV         R0, R6 
    /*00000e00*/ STR         IP, [SP, #4] 
    /*00000e04*/ ADD         R4, R4, #16 
    /*00000e08*/ LDR         IP, [R4, #-8] 
    /*00000e0c*/ STR         IP, [SP] 
    /*00000e10*/ BL          0x0000000000000E10          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00000e14*/ CMP         R5, #32768                  @ 0x8000 
    /*00000e18*/ BNE         VEDU_OutputDebugInfo_xde8
    /*00000e1c*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x414] 
    /*00000e20*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x414] 
    /*00000e24*/ BL          0x0000000000000E24          @ osal_printk [R_ARM_CALL: +0x0] 
    VEDU_OutputDebugInfo_xe28:
    /*00000e28*/ MOV         R0, #0 
    /*00000e2c*/ ADD         SP, SP, #8 
    /*00000e30*/ POP         {R4, R5, R6, PC} 
    VEDU_OutputDebugInfo_xe34:
    /*00000e34*/ BL          0x0000000000000A4C 
    /*00000e38*/ B           VEDU_OutputDebugInfo_xe28
FUNC_END VEDU_OutputDebugInfo


FUNC_BEGIN VEDU_Reset
    /*00000e3c*/ MOVW        R3, #0                      @ s_apVeduAddr [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000e40*/ MOVT        R3, #0                      @ s_apVeduAddr [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000e44*/ PUSH        {R4, R5, LR} 
    /*00000e48*/ MOV         R4, R0 
    /*00000e4c*/ LDR         R5, [R3, R0, LSL #2] 
    /*00000e50*/ SUB         SP, SP, #12 
    /*00000e54*/ CMP         R5, #0 
    /*00000e58*/ BEQ         VEDU_Reset_xed4
    VEDU_Reset_xe5c:
    /*00000e5c*/ MOV         R1, #1 
    /*00000e60*/ MOV         R0, R4 
    /*00000e64*/ BL          0x0000000000000E64          @ VEDU_SetVpuClk [R_ARM_CALL: .text+0xbe8] 
    /*00000e68*/ MOV         R0, R5 
    /*00000e6c*/ BL          0x0000000000000E6C          @ VEDU_DRV_ResetRequest [R_ARM_CALL: .text+0x1c44] 
    /*00000e70*/ MOV         R0, R5 
    /*00000e74*/ BL          0x0000000000000E74          @ VEDU_DRV_ReadBusIdleFlag [R_ARM_CALL: .text+0x1c48] 
    /*00000e78*/ CMP         R0, #0 
    /*00000e7c*/ BNE         VEDU_Reset_xea8
    /*00000e80*/ MOVW        R3, #347                    @ 0x15b 
    /*00000e84*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x470] 
    /*00000e88*/ STR         R3, [SP] 
    /*00000e8c*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x470] 
    /*00000e90*/ LDR         R3, [PC, #68]               @ 0x0000000000000edc 
    /*00000e94*/ MOV         R1, #25 
    /*00000e98*/ MOV         R0, #3 
    /*00000e9c*/ BL          0x0000000000000E9C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000ea0*/ MOV         R0, R4 
    /*00000ea4*/ BL          0x0000000000000EA4          @ VEDU_OutputDebugInfo [R_ARM_CALL: .text+0xda4] 
    VEDU_Reset_xea8:
    /*00000ea8*/ MOV         R1, #1 
    /*00000eac*/ MOV         R0, R4 
    /*00000eb0*/ BL          0x0000000000000EB0          @ VEDU_RstSel [R_ARM_CALL: .text+0xc94] 
    /*00000eb4*/ MOV         R1, #0 
    /*00000eb8*/ MOV         R0, R4 
    /*00000ebc*/ BL          0x0000000000000EBC          @ VEDU_RstSel [R_ARM_CALL: .text+0xc94] 
    /*00000ec0*/ MOV         R0, R5 
    /*00000ec4*/ BL          0x0000000000000EC4          @ VEDU_DRV_Init [R_ARM_CALL: .text+0x1d78] 
    /*00000ec8*/ MOV         R0, #0 
    /*00000ecc*/ ADD         SP, SP, #12 
    /*00000ed0*/ POP         {R4, R5, PC} 
    VEDU_Reset_xed4:
    /*00000ed4*/ BL          0x0000000000000A4C 
    /*00000ed8*/ B           VEDU_Reset_xe5c
    /*00000edc*/ STRHEQ      R0, [R0], -R8 
FUNC_END VEDU_Reset


FUNC_BEGIN VEDU_DRV_Restore
    /*00000ee0*/ PUSH        {R4, R5, R6, LR} 
    /*00000ee4*/ MOVW        R5, #0                      @ s_s32VeduNum [R_ARM_MOVW_ABS_NC: .bss+0xc] 
    /*00000ee8*/ MOVT        R5, #0                      @ s_s32VeduNum [R_ARM_MOVT_ABS: .bss+0xc] 
    /*00000eec*/ LDR         R3, [R5] 
    /*00000ef0*/ CMP         R3, #0 
    /*00000ef4*/ BLE         VEDU_DRV_Restore_xf40
    /*00000ef8*/ MOVW        R6, #0                      @ s_apVeduAddr [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000efc*/ MOVT        R6, #0                      @ s_apVeduAddr [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000f00*/ MOV         R4, #0 
    VEDU_DRV_Restore_xf04:
    /*00000f04*/ MOV         R1, #1 
    /*00000f08*/ MOV         R0, R4 
    /*00000f0c*/ BL          0x0000000000000F0C          @ VEDU_SetClk [R_ARM_CALL: .text+0xc4c] 
    /*00000f10*/ MOV         R1, #1 
    /*00000f14*/ MOV         R0, R4 
    /*00000f18*/ BL          0x0000000000000F18          @ VEDU_SetSedClk [R_ARM_CALL: .text+0xcdc] 
    /*00000f1c*/ MOV         R1, #0 
    /*00000f20*/ MOV         R0, R4 
    /*00000f24*/ BL          0x0000000000000F24          @ VEDU_RstSel [R_ARM_CALL: .text+0xc94] 
    /*00000f28*/ LDR         R0, [R6, R4, LSL #2] 
    /*00000f2c*/ BL          0x0000000000000F2C          @ VEDU_DRV_Init [R_ARM_CALL: .text+0x1d78] 
    /*00000f30*/ LDR         R3, [R5] 
    /*00000f34*/ ADD         R4, R4, #1 
    /*00000f38*/ CMP         R3, R4 
    /*00000f3c*/ BGT         VEDU_DRV_Restore_xf04
    VEDU_DRV_Restore_xf40:
    /*00000f40*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4b0] 
    /*00000f44*/ MOVW        R2, #479                    @ 0x1df 
    /*00000f48*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4b0] 
    /*00000f4c*/ LDR         R1, [PC, #4]                @ 0x0000000000000f58 
    /*00000f50*/ POP         {R4, R5, R6, LR} 
    VEDU_DRV_Restore_xf54:
    /*00000f54*/ B           VEDU_DRV_Restore_xf54       @ osal_printk [R_ARM_JUMP24: +0x0] 
    /*00000f58*/ ANDEQ       R0, R0, R4, ASR #1 
FUNC_END VEDU_DRV_Restore


FUNC_BEGIN JPGE_HAL_SetPowerMode
    JPGE_HAL_SetPowerMode_xf5c:
    /*00000f5c*/ B           JPGE_HAL_SetPowerMode_xf5c  @ JPGE_DRV_SetPowerMode [R_ARM_JUMP24: .text+0x3048] 
FUNC_END JPGE_HAL_SetPowerMode


FUNC_BEGIN JPGE_HAL_SetTimerMode
    JPGE_HAL_SetTimerMode_xf60:
    /*00000f60*/ B           JPGE_HAL_SetTimerMode_xf60  @ JPGE_DRV_SetTimerMode [R_ARM_JUMP24: .text+0x30b8] 
FUNC_END JPGE_HAL_SetTimerMode


FUNC_BEGIN JPGE_HAL_SetOutstanding
    JPGE_HAL_SetOutstanding_xf64:
    /*00000f64*/ B           JPGE_HAL_SetOutstanding_xf64@ JPGE_DRV_SetOutstanding [R_ARM_JUMP24: .text+0x3120] 
FUNC_END JPGE_HAL_SetOutstanding


FUNC_BEGIN JPGE_HAL_IntEn
    JPGE_HAL_IntEn_xf68:
    /*00000f68*/ B           JPGE_HAL_IntEn_xf68         @ JPGE_DRV_IntEn [R_ARM_JUMP24: .text+0x3140] 
FUNC_END JPGE_HAL_IntEn


FUNC_BEGIN JPGE_HAL_Init
    /*00000f6c*/ PUSH        {R4, LR} 
    /*00000f70*/ SUBS        R4, R0, #0 
    /*00000f74*/ BEQ         JPGE_HAL_Init_xfac
    JPGE_HAL_Init_xf78:
    /*00000f78*/ MOV         R0, R4 
    /*00000f7c*/ BL          0x0000000000000F7C          @ JPGE_DRV_IntEn [R_ARM_CALL: .text+0x3140] 
    /*00000f80*/ MOV         R0, R4 
    /*00000f84*/ MOV         R1, #1 
    /*00000f88*/ BL          0x0000000000000F88          @ JPGE_DRV_SetTimerMode [R_ARM_CALL: .text+0x30b8] 
    /*00000f8c*/ MOVW        R3, #0                      @ g_bJpgePowerSaveEn [R_ARM_MOVW_ABS_NC: .data+0xf8] 
    /*00000f90*/ MOVT        R3, #0                      @ g_bJpgePowerSaveEn [R_ARM_MOVT_ABS: .data+0xf8] 
    /*00000f94*/ MOV         R0, R4 
    /*00000f98*/ LDR         R1, [R3] 
    /*00000f9c*/ BL          0x0000000000000F9C          @ JPGE_DRV_SetPowerMode [R_ARM_CALL: .text+0x3048] 
    /*00000fa0*/ MOV         R0, R4 
    /*00000fa4*/ POP         {R4, LR} 
    JPGE_HAL_Init_xfa8:
    /*00000fa8*/ B           JPGE_HAL_Init_xfa8          @ JPGE_DRV_SetOutstanding [R_ARM_JUMP24: .text+0x3120] 
    JPGE_HAL_Init_xfac:
    /*00000fac*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3e0] 
    /*00000fb0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3a0] 
    /*00000fb4*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3e0] 
    /*00000fb8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3a0] 
    /*00000fbc*/ MOVW        R2, #585                    @ 0x249 
    /*00000fc0*/ LDR         R1, [PC, #4]                @ 0x0000000000000fcc 
    /*00000fc4*/ BL          0x0000000000000FC4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000fc8*/ B           JPGE_HAL_Init_xf78
    /*00000fcc*/ LDRDEQ      R0, [R0], -R8 
FUNC_END JPGE_HAL_Init


FUNC_BEGIN JPGE_GetRegAddr
    /*00000fd0*/ MOVW        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVW_ABS_NC: .bss+0x14] 
    /*00000fd4*/ MOVT        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVT_ABS: .bss+0x14] 
    /*00000fd8*/ LDR         R0, [R3, R0, LSL #2] 
    /*00000fdc*/ CMP         R0, #0 
    /*00000fe0*/ BXNE        LR 
    /*00000fe4*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000fe8*/ SUB         SP, SP, #12 
    /*00000fec*/ STR         R0, [SP, #4] 
    /*00000ff0*/ BL          0x0000000000000A6C 
    /*00000ff4*/ LDR         R0, [SP, #4] 
    /*00000ff8*/ ADD         SP, SP, #12 
    /*00000ffc*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END JPGE_GetRegAddr


FUNC_BEGIN JPGE_ReadInt
    JPGE_ReadInt_x1000:
    /*00001000*/ B           JPGE_ReadInt_x1000          @ JPGE_DRV_ReadInt [R_ARM_JUMP24: .text+0x3180] 
FUNC_END JPGE_ReadInt


FUNC_BEGIN JPGE_ClearInt
    /*00001004*/ PUSH        {R4, LR} 
    /*00001008*/ BL          0x0000000000001008          @ JPGE_DRV_ClearInt [R_ARM_CALL: .text+0x3194] 
    /*0000100c*/ MOV         R0, #0 
    /*00001010*/ POP         {R4, PC} 
FUNC_END JPGE_ClearInt


FUNC_BEGIN JPGE_ClearErrInt
    /*00001014*/ PUSH        {R4, LR} 
    /*00001018*/ BL          0x0000000000001018          @ JPGE_DRV_ClearErrInt [R_ARM_CALL: .text+0x31b0] 
    /*0000101c*/ MOV         R0, #0 
    /*00001020*/ POP         {R4, PC} 
FUNC_END JPGE_ClearErrInt


FUNC_BEGIN JPGE_GetIsIntOk
    JPGE_GetIsIntOk_x1024:
    /*00001024*/ B           JPGE_GetIsIntOk_x1024       @ JPGE_DRV_GetIsIntOk [R_ARM_JUMP24: .text+0x31cc] 
FUNC_END JPGE_GetIsIntOk


FUNC_BEGIN JPGE_SetIntMask
    /*00001028*/ MOV         R0, #0 
    /*0000102c*/ BX          LR 
FUNC_END JPGE_SetIntMask


FUNC_BEGIN JPGE_SetClk
    /*00001030*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00001034*/ SUB         SP, SP, #28 
    /*00001038*/ MOV         R3, #11 
    /*0000103c*/ STR         R0, [SP, #16] 
    /*00001040*/ MOV         R0, #2 
    /*00001044*/ STR         R1, [SP, #4] 
    /*00001048*/ STR         R3, [SP, #12] 
    /*0000104c*/ BL          0x000000000000104C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00001050*/ ADD         R2, SP, #4 
    /*00001054*/ MOV         R1, #137                    @ 0x89 
    /*00001058*/ LDR         R3, [R0, #20] 
    /*0000105c*/ ADD         R0, SP, #12 
    /*00001060*/ BLX         R3 
    /*00001064*/ MOV         R0, #0 
    /*00001068*/ ADD         SP, SP, #28 
    /*0000106c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END JPGE_SetClk


FUNC_BEGIN JPGE_RstSel
    /*00001070*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00001074*/ SUB         SP, SP, #28 
    /*00001078*/ MOV         R3, #11 
    /*0000107c*/ STR         R0, [SP, #16] 
    /*00001080*/ MOV         R0, #2 
    /*00001084*/ STR         R1, [SP, #4] 
    /*00001088*/ STR         R3, [SP, #12] 
    /*0000108c*/ BL          0x000000000000108C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00001090*/ ADD         R2, SP, #4 
    /*00001094*/ MOV         R1, #136                    @ 0x88 
    /*00001098*/ LDR         R3, [R0, #20] 
    /*0000109c*/ ADD         R0, SP, #12 
    /*000010a0*/ BLX         R3 
    /*000010a4*/ MOV         R0, #0 
    /*000010a8*/ ADD         SP, SP, #28 
    /*000010ac*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END JPGE_RstSel


FUNC_BEGIN JPGE_IsNeedReset
    JPGE_IsNeedReset_x10b0:
    /*000010b0*/ B           JPGE_IsNeedReset_x10b0      @ JPGE_DRV_IsNeedReset [R_ARM_JUMP24: .text+0x3210] 
FUNC_END JPGE_IsNeedReset


FUNC_BEGIN JPGE_Reset
    /*000010b4*/ PUSH        {R4, LR} 
    /*000010b8*/ MOV         R1, #1 
    /*000010bc*/ MOV         R4, R0 
    /*000010c0*/ SUB         SP, SP, #8 
    /*000010c4*/ BL          0x00000000000010C4          @ JPGE_RstSel [R_ARM_CALL: .text+0x1070] 
    /*000010c8*/ MOV         R0, R4 
    /*000010cc*/ MOV         R1, #0 
    /*000010d0*/ BL          0x00000000000010D0          @ JPGE_RstSel [R_ARM_CALL: .text+0x1070] 
    /*000010d4*/ MOVW        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVW_ABS_NC: .bss+0x14] 
    /*000010d8*/ MOVT        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVT_ABS: .bss+0x14] 
    /*000010dc*/ LDR         R0, [R3, R4, LSL #2] 
    /*000010e0*/ CMP         R0, #0 
    /*000010e4*/ BEQ         JPGE_Reset_x10f8
    /*000010e8*/ BL          0x00000000000010E8          @ JPGE_HAL_Init [R_ARM_CALL: .text+0xf6c] 
    /*000010ec*/ MOV         R0, #0 
    /*000010f0*/ ADD         SP, SP, #8 
    /*000010f4*/ POP         {R4, PC} 
    JPGE_Reset_x10f8:
    /*000010f8*/ STR         R0, [SP, #4] 
    /*000010fc*/ BL          0x0000000000000A6C 
    /*00001100*/ LDR         R0, [SP, #4] 
    /*00001104*/ BL          0x0000000000001104          @ JPGE_HAL_Init [R_ARM_CALL: .text+0xf6c] 
    /*00001108*/ MOV         R0, #0 
    /*0000110c*/ ADD         SP, SP, #8 
    /*00001110*/ POP         {R4, PC} 
FUNC_END JPGE_Reset


FUNC_BEGIN JPGE_StartVpu
    /*00001114*/ PUSH        {R4, LR} 
    /*00001118*/ MOV         R4, R0 
    /*0000111c*/ BL          0x000000000000111C          @ osal_dmb [R_ARM_CALL: +0x0] 
    /*00001120*/ MOV         R0, R4 
    /*00001124*/ BL          0x0000000000001124          @ JPGE_DRV_StartVpu [R_ARM_CALL: .text+0x31ec] 
    /*00001128*/ MOV         R0, #0 
    /*0000112c*/ POP         {R4, PC} 
FUNC_END JPGE_StartVpu


FUNC_BEGIN JPGE_SetTimeOut
    /*00001130*/ MOV         R0, #0 
    /*00001134*/ BX          LR 
FUNC_END JPGE_SetTimeOut


FUNC_BEGIN JPGE_GetTimeOut
    /*00001138*/ MOV         R0, #0 
    /*0000113c*/ BX          LR 
FUNC_END JPGE_GetTimeOut


FUNC_BEGIN JPGE_OutputDebugInfo
    /*00001140*/ MOVW        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVW_ABS_NC: .bss+0x14] 
    /*00001144*/ MOVT        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVT_ABS: .bss+0x14] 
    /*00001148*/ PUSH        {R4, LR} 
    /*0000114c*/ SUB         SP, SP, #8 
    /*00001150*/ LDR         R4, [R3, R0, LSL #2] 
    /*00001154*/ CMP         R4, #0 
    /*00001158*/ BEQ         JPGE_OutputDebugInfo_x1190
    JPGE_OutputDebugInfo_x115c:
    /*0000115c*/ LDR         R1, [R4, #3072]             @ 0xc00 
    /*00001160*/ MOV         R3, #848                    @ 0x350 
    /*00001164*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4b8] 
    /*00001168*/ STR         R3, [SP] 
    /*0000116c*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4b8] 
    /*00001170*/ LDR         R3, [PC, #32]               @ 0x0000000000001198 
    /*00001174*/ STR         R1, [SP, #4] 
    /*00001178*/ MOV         R0, #3 
    /*0000117c*/ MOV         R1, #25 
    /*00001180*/ BL          0x0000000000001180          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001184*/ MOV         R0, #0 
    /*00001188*/ ADD         SP, SP, #8 
    /*0000118c*/ POP         {R4, PC} 
    JPGE_OutputDebugInfo_x1190:
    /*00001190*/ BL          0x0000000000000A6C 
    /*00001194*/ B           JPGE_OutputDebugInfo_x115c
    /*00001198*/ ANDEQ       R0, R0, R8, ROR #1 
FUNC_END JPGE_OutputDebugInfo


FUNC_BEGIN JPGE_DRV_Restore
    /*0000119c*/ PUSH        {R4, LR} 
    /*000011a0*/ MOV         R1, #1 
    /*000011a4*/ MOV         R0, #0 
    /*000011a8*/ BL          0x00000000000011A8          @ JPGE_SetClk [R_ARM_CALL: .text+0x1030] 
    /*000011ac*/ MOV         R1, #0 
    /*000011b0*/ MOV         R0, R1 
    /*000011b4*/ BL          0x00000000000011B4          @ JPGE_RstSel [R_ARM_CALL: .text+0x1070] 
    /*000011b8*/ MOVW        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVW_ABS_NC: .bss+0x14] 
    /*000011bc*/ MOVT        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVT_ABS: .bss+0x14] 
    /*000011c0*/ POP         {R4, LR} 
    /*000011c4*/ LDR         R0, [R3] 
    JPGE_DRV_Restore_x11c8:
    /*000011c8*/ B           JPGE_DRV_Restore_x11c8      @ JPGE_HAL_Init [R_ARM_JUMP24: .text+0xf6c] 
FUNC_END JPGE_DRV_Restore


FUNC_BEGIN VPU_DRV_Restore
    /*000011cc*/ PUSH        {R4, LR} 
    /*000011d0*/ BL          0x00000000000011D0          @ VEDU_DRV_Restore [R_ARM_CALL: .text+0xee0] 
    /*000011d4*/ POP         {R4, LR} 
    VPU_DRV_Restore_x11d8:
    /*000011d8*/ B           VPU_DRV_Restore_x11d8       @ JPGE_DRV_Restore [R_ARM_JUMP24: .text+0x119c] 
FUNC_END VPU_DRV_Restore


FUNC_BEGIN VEDU_DEC_ReportErr
    /*000011dc*/ LDR         R3, [R0, #148]              @ 0x94 
    /*000011e0*/ LDR         R3, [R3, #4] 
    /*000011e4*/ CMP         R3, #1 
    /*000011e8*/ BEQ         VEDU_DEC_ReportErr_x1238
    /*000011ec*/ MOV         R3, #0 
    /*000011f0*/ STR         R3, [R1, #4] 
    /*000011f4*/ LDR         R3, [R0, #140]              @ 0x8c 
    /*000011f8*/ CMP         R3, #3 
    /*000011fc*/ LDRLS       PC, [PC, R3, LSL #2] 
    /*00001200*/ B           VEDU_DEC_ReportErr_x121c
    /*00001204*/ ANDEQ       R1, R0, R4, LSL R2 
    /*00001208*/ ANDEQ       R1, R0, R0, LSR #4 
    /*0000120c*/ ANDEQ       R1, R0, R0, LSR #4 
    /*00001210*/ ANDEQ       R1, R0, IP, LSR #4 
    /*00001214*/ MOV         R3, #0 
    /*00001218*/ STR         R3, [R1] 
    VEDU_DEC_ReportErr_x121c:
    /*0000121c*/ BX          LR 
    /*00001220*/ LDR         R3, [R0, #144]              @ 0x90 
    /*00001224*/ STR         R3, [R1] 
    /*00001228*/ BX          LR 
    /*0000122c*/ MOV         R3, #100                    @ 0x64 
    /*00001230*/ STR         R3, [R1] 
    /*00001234*/ BX          LR 
    VEDU_DEC_ReportErr_x1238:
    /*00001238*/ MOV         R2, #100                    @ 0x64 
    /*0000123c*/ MOV         R3, #0 
    /*00001240*/ STM         R1, {R2, R3} 
    /*00001244*/ BX          LR 
FUNC_END VEDU_DEC_ReportErr


FUNC_BEGIN VEDU_DEC_GetHandle
    /*00001248*/ PUSH        {R4, R5, R6, R7, LR} 
    /*0000124c*/ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1c] 
    /*00001250*/ MOVT        R6, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1c] 
    /*00001254*/ SUB         SP, SP, #12 
    /*00001258*/ ADD         R7, R6, #8 
    /*0000125c*/ MOV         R4, #0 
    VEDU_DEC_GetHandle_x1260:
    /*00001260*/ LDR         R5, [R6, R4, LSL #2] 
    /*00001264*/ ADD         R1, SP, #4 
    /*00001268*/ MOV         R0, R7 
    /*0000126c*/ ADD         R4, R4, #1 
    /*00001270*/ BL          0x0000000000001270          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001274*/ CMP         R5, #0 
    /*00001278*/ ADD         R1, SP, #4 
    /*0000127c*/ MOV         R0, R7 
    /*00001280*/ BEQ         VEDU_DEC_GetHandle_x1290
    /*00001284*/ LDR         R3, [R5, #4] 
    /*00001288*/ CMP         R3, #0 
    /*0000128c*/ BEQ         VEDU_DEC_GetHandle_x12a8
    VEDU_DEC_GetHandle_x1290:
    /*00001290*/ BL          0x0000000000001290          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001294*/ CMP         R4, #2 
    /*00001298*/ BNE         VEDU_DEC_GetHandle_x1260
    /*0000129c*/ MOV         R0, #0 
    VEDU_DEC_GetHandle_x12a0:
    /*000012a0*/ ADD         SP, SP, #12 
    /*000012a4*/ POP         {R4, R5, R6, R7, PC} 
    VEDU_DEC_GetHandle_x12a8:
    /*000012a8*/ MOV         R3, #1 
    /*000012ac*/ LDR         R0, [PC, #12]               @ 0x00000000000012c0 
    /*000012b0*/ STR         R3, [R5, #4] 
    /*000012b4*/ BL          0x00000000000012B4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000012b8*/ MOV         R0, R5 
    /*000012bc*/ B           VEDU_DEC_GetHandle_x12a0
    /*000012c0*/ ANDEQ       R0, R0, R4, LSR #32 
FUNC_END VEDU_DEC_GetHandle


FUNC_BEGIN VEDU_DEC_ReleaseHandle
    /*000012c4*/ PUSH        {R4, LR} 
    /*000012c8*/ SUBS        R4, R0, #0 
    /*000012cc*/ SUB         SP, SP, #8 
    /*000012d0*/ BEQ         VEDU_DEC_ReleaseHandle_x12f4
    /*000012d4*/ LDR         R0, [PC, #32]               @ 0x00000000000012fc 
    /*000012d8*/ ADD         R1, SP, #4 
    /*000012dc*/ BL          0x00000000000012DC          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000012e0*/ MOV         R3, #0 
    /*000012e4*/ ADD         R1, SP, #4 
    /*000012e8*/ LDR         R0, [PC, #12]               @ 0x00000000000012fc 
    /*000012ec*/ STR         R3, [R4, #4] 
    /*000012f0*/ BL          0x00000000000012F0          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    VEDU_DEC_ReleaseHandle_x12f4:
    /*000012f4*/ ADD         SP, SP, #8 
    /*000012f8*/ POP         {R4, PC} 
    /*000012fc*/ ANDEQ       R0, R0, R4, LSR #32 
FUNC_END VEDU_DEC_ReleaseHandle


FUNC_BEGIN VEDU_DEC_QueryOneFrame
    /*00001300*/ PUSH        {R4, LR} 
    /*00001304*/ SUBS        R4, R0, #0 
    /*00001308*/ SUB         SP, SP, #8 
    /*0000130c*/ BEQ         VEDU_DEC_QueryOneFrame_x1350
    /*00001310*/ MOV         R0, #35                     @ 0x23 
    /*00001314*/ BL          0x0000000000001314          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00001318*/ LDR         R3, [R0, #28] 
    /*0000131c*/ CMP         R3, #0 
    /*00001320*/ BEQ         VEDU_DEC_QueryOneFrame_x1344
    /*00001324*/ MOV         R0, #35                     @ 0x23 
    /*00001328*/ BL          0x0000000000001328          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000132c*/ ADD         R1, R4, #16 
    /*00001330*/ LDR         R3, [R0, #28] 
    /*00001334*/ LDR         R0, [R4, #12] 
    /*00001338*/ BLX         R3 
    /*0000133c*/ CMP         R0, #0 
    /*00001340*/ BNE         VEDU_DEC_QueryOneFrame_x1358
    VEDU_DEC_QueryOneFrame_x1344:
    /*00001344*/ MOV         R0, #0 
    VEDU_DEC_QueryOneFrame_x1348:
    /*00001348*/ ADD         SP, SP, #8 
    /*0000134c*/ POP         {R4, PC} 
    VEDU_DEC_QueryOneFrame_x1350:
    /*00001350*/ MVN         R0, #0 
    /*00001354*/ B           VEDU_DEC_QueryOneFrame_x1348
    VEDU_DEC_QueryOneFrame_x1358:
    /*00001358*/ MOV         R2, #161                    @ 0xa1 
    /*0000135c*/ MOV         R1, #25 
    /*00001360*/ STR         R2, [SP] 
    /*00001364*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x100] 
    /*00001368*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4e8] 
    /*0000136c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x100] 
    /*00001370*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4e8] 
    /*00001374*/ MOV         R0, #6 
    /*00001378*/ BL          0x0000000000001378          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000137c*/ MOV         R0, R4 
    /*00001380*/ BL          0x0000000000001380          @ VEDU_DEC_ReleaseHandle [R_ARM_CALL: .text+0x12c4] 
    /*00001384*/ MVN         R0, #0 
    /*00001388*/ B           VEDU_DEC_QueryOneFrame_x1348
FUNC_END VEDU_DEC_QueryOneFrame


FUNC_BEGIN VEDU_DEC_StartOneFrame
    /*0000138c*/ LDR         R3, [R0, #12] 
    /*00001390*/ CMP         R3, #15 
    /*00001394*/ BHI         VEDU_DEC_StartOneFrame_x1458
    /*00001398*/ PUSH        {R4, R5, LR} 
    /*0000139c*/ MOVW        R1, #265                    @ 0x109 
    /*000013a0*/ LDR         R2, [R0] 
    /*000013a4*/ SUB         SP, SP, #12 
    /*000013a8*/ MOV         R4, R0 
    /*000013ac*/ STR         R3, [R0, #120]              @ 0x78 
    /*000013b0*/ CMP         R2, R1 
    /*000013b4*/ BEQ         VEDU_DEC_StartOneFrame_x13f8
    /*000013b8*/ CMP         R2, #96                     @ 0x60 
    /*000013bc*/ BNE         VEDU_DEC_StartOneFrame_x142c
    /*000013c0*/ ADD         R5, R0, #16 
    /*000013c4*/ ADD         R1, R0, #80                 @ 0x50 
    /*000013c8*/ MOV         R0, R5 
    /*000013cc*/ BL          0x00000000000013CC          @ VEDU_DEC_HAL_H264_GetBufferCfg [R_ARM_CALL: .text+0x3d2c] 
    /*000013d0*/ ADD         R1, R4, #116                @ 0x74 
    /*000013d4*/ MOV         R0, R5 
    /*000013d8*/ BL          0x00000000000013D8          @ VEDU_DEC_HAL_H264_GetFrameType [R_ARM_CALL: .text+0x3d98] 
    /*000013dc*/ MOV         R0, R5 
    /*000013e0*/ BL          0x00000000000013E0          @ VEDU_DEC_HAL_H264_UpdateRefInfo [R_ARM_CALL: .text+0x3b20] 
    /*000013e4*/ MOV         R0, R5 
    /*000013e8*/ BL          0x00000000000013E8          @ VEDU_DEC_HAL_H264_WriteSliceMsg [R_ARM_CALL: .text+0x3e00] 
    /*000013ec*/ MOV         R0, #0 
    VEDU_DEC_StartOneFrame_x13f0:
    /*000013f0*/ ADD         SP, SP, #12 
    /*000013f4*/ POP         {R4, R5, PC} 
    VEDU_DEC_StartOneFrame_x13f8:
    /*000013f8*/ ADD         R5, R0, #16 
    /*000013fc*/ ADD         R1, R0, #80                 @ 0x50 
    /*00001400*/ MOV         R0, R5 
    /*00001404*/ BL          0x0000000000001404          @ VEDU_DEC_HAL_H265_GetBufferCfg [R_ARM_CALL: .text+0x4b98] 
    /*00001408*/ ADD         R1, R4, #116                @ 0x74 
    /*0000140c*/ MOV         R0, R5 
    /*00001410*/ BL          0x0000000000001410          @ VEDU_DEC_HAL_H265_GetFrameType [R_ARM_CALL: .text+0x4bfc] 
    /*00001414*/ MOV         R0, R5 
    /*00001418*/ BL          0x0000000000001418          @ VEDU_DEC_HAL_H265_UpdateRefInfo [R_ARM_CALL: .text+0x4700] 
    /*0000141c*/ MOV         R0, R5 
    /*00001420*/ BL          0x0000000000001420          @ VEDU_DEC_HAL_H265_WriteSliceMsg [R_ARM_CALL: .text+0x4c74] 
    /*00001424*/ MOV         R0, #0 
    /*00001428*/ B           VEDU_DEC_StartOneFrame_x13f0
    VEDU_DEC_StartOneFrame_x142c:
    /*0000142c*/ STR         R2, [SP, #4] 
    /*00001430*/ MOV         R3, #201                    @ 0xc9 
    /*00001434*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x528] 
    /*00001438*/ STR         R3, [SP] 
    /*0000143c*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x528] 
    /*00001440*/ LDR         R3, [PC, #24]               @ 0x0000000000001460 
    /*00001444*/ MOV         R1, #25 
    /*00001448*/ MOV         R0, #3 
    /*0000144c*/ BL          0x000000000000144C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001450*/ MVN         R0, #0 
    /*00001454*/ B           VEDU_DEC_StartOneFrame_x13f0
    VEDU_DEC_StartOneFrame_x1458:
    /*00001458*/ MVN         R0, #0 
    /*0000145c*/ BX          LR 
    /*00001460*/ ANDEQ       R0, R0, R8, LSL R1 
FUNC_END VEDU_DEC_StartOneFrame


FUNC_BEGIN VEDU_DEC_WriteRegs
    /*00001464*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00001468*/ MOVW        R2, #265                    @ 0x109 
    /*0000146c*/ LDR         R3, [R0] 
    /*00001470*/ SUB         SP, SP, #12 
    /*00001474*/ CMP         R3, R2 
    /*00001478*/ BEQ         VEDU_DEC_WriteRegs_x1498
    /*0000147c*/ CMP         R3, #96                     @ 0x60 
    /*00001480*/ BNE         VEDU_DEC_WriteRegs_x14a8
    /*00001484*/ ADD         R0, R0, #16 
    /*00001488*/ BL          0x0000000000001488          @ VEDU_DEC_HAL_H264_ConfigReg [R_ARM_CALL: .text+0x41c4] 
    /*0000148c*/ MOV         R0, #0 
    VEDU_DEC_WriteRegs_x1490:
    /*00001490*/ ADD         SP, SP, #12 
    /*00001494*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    VEDU_DEC_WriteRegs_x1498:
    /*00001498*/ ADD         R0, R0, #16 
    /*0000149c*/ BL          0x000000000000149C          @ VEDU_DEC_HAL_H265_ConfigReg [R_ARM_CALL: .text+0x5114] 
    /*000014a0*/ MOV         R0, #0 
    /*000014a4*/ B           VEDU_DEC_WriteRegs_x1490
    VEDU_DEC_WriteRegs_x14a8:
    /*000014a8*/ STR         R3, [SP, #4] 
    /*000014ac*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x528] 
    /*000014b0*/ MOV         R3, #231                    @ 0xe7 
    /*000014b4*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x528] 
    /*000014b8*/ STR         R3, [SP] 
    /*000014bc*/ MOV         R1, #25 
    /*000014c0*/ LDR         R3, [PC, #12]               @ 0x00000000000014d4 
    /*000014c4*/ MOV         R0, #3 
    /*000014c8*/ BL          0x00000000000014C8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000014cc*/ MVN         R0, #0 
    /*000014d0*/ B           VEDU_DEC_WriteRegs_x1490
    /*000014d4*/ ANDEQ       R0, R0, R0, LSR R1 
FUNC_END VEDU_DEC_WriteRegs


FUNC_BEGIN VEDU_DEC_ReadRegs
    /*000014d8*/ PUSH        {R4, R5, R6, LR} 
    /*000014dc*/ MOV         R4, R0 
    /*000014e0*/ SUB         SP, SP, #8 
    /*000014e4*/ MOV         R0, R1 
    /*000014e8*/ MOV         R5, R1 
    /*000014ec*/ BL          0x00000000000014EC          @ VEDU_DRV_ReadRawInt [R_ARM_CALL: .text+0x19f8] 
    /*000014f0*/ LDR         R3, [R4] 
    /*000014f4*/ MOVW        R2, #265                    @ 0x109 
    /*000014f8*/ CMP         R3, R2 
    /*000014fc*/ STR         R0, [R4, #136]              @ 0x88 
    /*00001500*/ BEQ         VEDU_DEC_ReadRegs_x1534
    /*00001504*/ CMP         R3, #96                     @ 0x60 
    /*00001508*/ BNE         VEDU_DEC_ReadRegs_x1558
    /*0000150c*/ ADD         R6, R4, #136                @ 0x88 
    /*00001510*/ MOV         R1, R5 
    /*00001514*/ MOV         R0, R6 
    /*00001518*/ BL          0x0000000000001518          @ VEDU_DEC_HAL_H264_ReadRegs [R_ARM_CALL: .text+0x4554] 
    /*0000151c*/ MOV         R0, R6 
    /*00001520*/ ADD         R1, R4, #16 
    /*00001524*/ BL          0x0000000000001524          @ VEDU_DEC_HAL_H264_ReadUpMsg [R_ARM_CALL: .text+0x4628] 
    /*00001528*/ MOV         R0, #0 
    VEDU_DEC_ReadRegs_x152c:
    /*0000152c*/ ADD         SP, SP, #8 
    /*00001530*/ POP         {R4, R5, R6, PC} 
    VEDU_DEC_ReadRegs_x1534:
    /*00001534*/ ADD         R6, R4, #136                @ 0x88 
    /*00001538*/ MOV         R1, R5 
    /*0000153c*/ MOV         R0, R6 
    /*00001540*/ BL          0x0000000000001540          @ VEDU_DEC_HAL_H265_ReadRegs [R_ARM_CALL: .text+0x5648] 
    /*00001544*/ MOV         R0, R6 
    /*00001548*/ ADD         R1, R4, #16 
    /*0000154c*/ BL          0x000000000000154C          @ VEDU_DEC_HAL_H265_ReadUpMsg [R_ARM_CALL: .text+0x5758] 
    /*00001550*/ MOV         R0, #0 
    /*00001554*/ B           VEDU_DEC_ReadRegs_x152c
    VEDU_DEC_ReadRegs_x1558:
    /*00001558*/ STR         R3, [SP, #4] 
    /*0000155c*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x528] 
    /*00001560*/ MOVW        R3, #267                    @ 0x10b 
    /*00001564*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x528] 
    /*00001568*/ STR         R3, [SP] 
    /*0000156c*/ MOV         R1, #25 
    /*00001570*/ LDR         R3, [PC, #12]               @ 0x0000000000001584 
    /*00001574*/ MOV         R0, #3 
    /*00001578*/ BL          0x0000000000001578          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000157c*/ MVN         R0, #0 
    /*00001580*/ B           VEDU_DEC_ReadRegs_x152c
    /*00001584*/ ANDEQ       R0, R0, R4, ASR #2 
FUNC_END VEDU_DEC_ReadRegs


FUNC_BEGIN VEDU_DEC_IntProcess
    /*00001588*/ PUSH        {R4, LR} 
    /*0000158c*/ SUB         SP, SP, #8 
    /*00001590*/ MOV         R4, R0 
    /*00001594*/ MOV         R1, SP 
    /*00001598*/ BL          0x0000000000001598          @ VEDU_DEC_ReportErr [R_ARM_CALL: .text+0x11dc] 
    /*0000159c*/ MOV         R0, #35                     @ 0x23 
    /*000015a0*/ BL          0x00000000000015A0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000015a4*/ LDR         R3, [R0, #32] 
    /*000015a8*/ CMP         R3, #0 
    /*000015ac*/ BEQ         VEDU_DEC_IntProcess_x15c8
    /*000015b0*/ MOV         R0, #35                     @ 0x23 
    /*000015b4*/ BL          0x00000000000015B4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000015b8*/ MOV         R1, SP 
    /*000015bc*/ LDR         R3, [R0, #32] 
    /*000015c0*/ LDR         R0, [R4, #12] 
    /*000015c4*/ BLX         R3 
    VEDU_DEC_IntProcess_x15c8:
    /*000015c8*/ MOV         R0, #0 
    /*000015cc*/ ADD         SP, SP, #8 
    /*000015d0*/ POP         {R4, PC} 
FUNC_END VEDU_DEC_IntProcess


FUNC_BEGIN VEDU_DEC_Destroy
    /*000015d4*/ PUSH        {R4, LR} 
    /*000015d8*/ SUBS        R4, R0, #0 
    /*000015dc*/ POPEQ       {R4, PC} 
    /*000015e0*/ LDRD        R0, [R4, #64]               @ 0x40 
    /*000015e4*/ ORRS        R3, R0, R1 
    /*000015e8*/ BNE         VEDU_DEC_Destroy_x1628
    VEDU_DEC_Destroy_x15ec:
    /*000015ec*/ LDRD        R0, [R4, #48]               @ 0x30 
    /*000015f0*/ ORRS        R3, R0, R1 
    /*000015f4*/ BEQ         VEDU_DEC_Destroy_x161c
    /*000015f8*/ LDR         R2, [R4, #56]               @ 0x38 
    /*000015fc*/ BL          0x00000000000015FC          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*00001600*/ MOV         R0, #0 
    /*00001604*/ MOV         R1, #0 
    /*00001608*/ MOV         R3, #0 
    /*0000160c*/ STRD        R0, [R4, #48]               @ 0x30 
    /*00001610*/ STRD        R0, [R4, #32] 
    /*00001614*/ STR         R3, [R4, #56]               @ 0x38 
    /*00001618*/ STR         R3, [R4, #40]               @ 0x28 
    VEDU_DEC_Destroy_x161c:
    /*0000161c*/ MOV         R0, R4 
    /*00001620*/ POP         {R4, LR} 
    VEDU_DEC_Destroy_x1624:
    /*00001624*/ B           VEDU_DEC_Destroy_x1624      @ osal_vfree [R_ARM_JUMP24: +0x0] 
    VEDU_DEC_Destroy_x1628:
    /*00001628*/ LDR         R2, [R4, #72]               @ 0x48 
    /*0000162c*/ BL          0x000000000000162C          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*00001630*/ MOV         R0, #0 
    /*00001634*/ MOV         R1, #0 
    /*00001638*/ MOV         R3, #0 
    /*0000163c*/ STRD        R0, [R4, #64]               @ 0x40 
    /*00001640*/ STR         R3, [R4, #72]               @ 0x48 
    /*00001644*/ B           VEDU_DEC_Destroy_x15ec
FUNC_END VEDU_DEC_Destroy


FUNC_BEGIN VEDU_DEC_Create
    /*00001648*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*0000164c*/ MOV         R9, R0 
    /*00001650*/ SUB         SP, SP, #40                 @ 0x28 
    /*00001654*/ MOV         R0, #160                    @ 0xa0 
    /*00001658*/ BL          0x0000000000001658          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*0000165c*/ SUBS        R4, R0, #0 
    /*00001660*/ BEQ         VEDU_DEC_Create_x17c8
    /*00001664*/ MOV         R2, #160                    @ 0xa0 
    /*00001668*/ MOV         R1, #0 
    /*0000166c*/ MOVW        R8, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0xfc] 
    /*00001670*/ MOVT        R8, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0xfc] 
    /*00001674*/ BL          0x0000000000001674          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00001678*/ MOV         SL, #0 
    /*0000167c*/ MOV         R3, R9 
    /*00001680*/ STR         SL, [R4, #4] 
    /*00001684*/ MOV         R1, #16 
    /*00001688*/ ADD         R0, SP, #24 
    /*0000168c*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x588] 
    /*00001690*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x588] 
    /*00001694*/ BL          0x0000000000001694          @ osal_snprintf [R_ARM_CALL: +0x0] 
    /*00001698*/ LDR         R5, [R8] 
    /*0000169c*/ LDR         R2, [R8, #4] 
    /*000016a0*/ MOVW        R6, #4095                   @ 0xfff 
    /*000016a4*/ MOV         R7, #61440                  @ 0xf000 
    /*000016a8*/ MOVT        R7, #65535                  @ 0xffff 
    /*000016ac*/ LSL         R5, R5, #8 
    /*000016b0*/ MOV         R0, SL 
    /*000016b4*/ LSL         R2, R2, #8 
    /*000016b8*/ ADD         R1, SP, #24 
    /*000016bc*/ CMP         R5, R2 
    /*000016c0*/ ADDCS       R5, R6, R5 
    /*000016c4*/ ADDCC       R5, R6, R2 
    /*000016c8*/ ADD         R3, SP, #12 
    /*000016cc*/ ADD         R2, SP, #16 
    /*000016d0*/ AND         R5, R5, R7 
    /*000016d4*/ STR         R5, [SP] 
    /*000016d8*/ BL          0x00000000000016D8          @ CMPI_MmzMallocCached [R_ARM_CALL: +0x0] 
    /*000016dc*/ SUBS        SL, R0, #0 
    /*000016e0*/ BNE         VEDU_DEC_Create_x1774
    /*000016e4*/ LDRD        R0, [SP, #16] 
    /*000016e8*/ MOV         R3, R9 
    /*000016ec*/ LDR         IP, [SP, #12] 
    /*000016f0*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x598] 
    /*000016f4*/ STR         R5, [R4, #60]               @ 0x3c 
    /*000016f8*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x598] 
    /*000016fc*/ STRD        R0, [R4, #48]               @ 0x30 
    /*00001700*/ ADDS        R0, R0, R5 
    /*00001704*/ ADC         R1, R1, #0 
    /*00001708*/ ADD         R5, IP, R5 
    /*0000170c*/ STRD        R0, [R4, #32] 
    /*00001710*/ MOV         R1, #16 
    /*00001714*/ STR         IP, [R4, #56]               @ 0x38 
    /*00001718*/ ADD         R0, SP, #24 
    /*0000171c*/ STR         SL, [R4, #44]               @ 0x2c 
    /*00001720*/ STR         R5, [R4, #40]               @ 0x28 
    /*00001724*/ BL          0x0000000000001724          @ osal_snprintf [R_ARM_CALL: +0x0] 
    /*00001728*/ LDM         R8, {IP, LR} 
    /*0000172c*/ ADD         R1, SP, #24 
    /*00001730*/ MOV         R0, SL 
    /*00001734*/ ADD         IP, R6, IP, LSL #4 
    /*00001738*/ ADD         R3, R4, #72                 @ 0x48 
    /*0000173c*/ ADD         R6, R6, LR, LSL #4 
    /*00001740*/ AND         IP, IP, R7 
    /*00001744*/ AND         R6, R6, R7 
    /*00001748*/ ADD         R2, R4, #64                 @ 0x40 
    /*0000174c*/ CMP         IP, R6 
    /*00001750*/ MOVCC       IP, R6 
    /*00001754*/ STR         IP, [R4, #76]               @ 0x4c 
    /*00001758*/ STR         IP, [SP] 
    /*0000175c*/ BL          0x000000000000175C          @ CMPI_MmzMallocNocache [R_ARM_CALL: +0x0] 
    /*00001760*/ CMP         R0, #0 
    /*00001764*/ MOVEQ       R0, R4 
    /*00001768*/ BNE         VEDU_DEC_Create_x17b0
    VEDU_DEC_Create_x176c:
    /*0000176c*/ ADD         SP, SP, #40                 @ 0x28 
    /*00001770*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    VEDU_DEC_Create_x1774:
    /*00001774*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5e0] 
    /*00001778*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5e0] 
    /*0000177c*/ LDR         R1, [R4, #60]               @ 0x3c 
    /*00001780*/ MOVW        R3, #459                    @ 0x1cb 
    /*00001784*/ STR         R3, [SP] 
    VEDU_DEC_Create_x1788:
    /*00001788*/ LDR         R3, [PC, #96]               @ 0x00000000000017f0 
    /*0000178c*/ MOV         R0, #3 
    /*00001790*/ STR         R1, [SP, #4] 
    /*00001794*/ MOV         R1, #25 
    /*00001798*/ BL          0x0000000000001798          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000179c*/ MOV         R0, R4 
    /*000017a0*/ BL          0x00000000000017A0          @ VEDU_DEC_Destroy [R_ARM_CALL: .text+0x15d4] 
    /*000017a4*/ MOV         R0, #0 
    /*000017a8*/ ADD         SP, SP, #40                 @ 0x28 
    /*000017ac*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    VEDU_DEC_Create_x17b0:
    /*000017b0*/ LDR         R1, [R4, #76]               @ 0x4c 
    /*000017b4*/ MOVW        R3, #466                    @ 0x1d2 
    /*000017b8*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5a8] 
    /*000017bc*/ STR         R3, [SP] 
    /*000017c0*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5a8] 
    /*000017c4*/ B           VEDU_DEC_Create_x1788
    VEDU_DEC_Create_x17c8:
    /*000017c8*/ MOVW        R3, #449                    @ 0x1c1 
    /*000017cc*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x558] 
    /*000017d0*/ STR         R3, [SP] 
    /*000017d4*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x558] 
    /*000017d8*/ LDR         R3, [PC, #16]               @ 0x00000000000017f0 
    /*000017dc*/ MOV         R1, #25 
    /*000017e0*/ MOV         R0, #3 
    /*000017e4*/ BL          0x00000000000017E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000017e8*/ MOV         R0, R4 
    /*000017ec*/ B           VEDU_DEC_Create_x176c
    /*000017f0*/ ANDEQ       R0, R0, R8, ASR R1 
FUNC_END VEDU_DEC_Create


FUNC_BEGIN VEDU_DEC_Exit
    /*000017f4*/ PUSH        {R4, R5, R6, LR} 
    /*000017f8*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1c] 
    /*000017fc*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1c] 
    /*00001800*/ ADD         R0, R4, #12 
    /*00001804*/ BL          0x0000000000001804          @ osal_down [R_ARM_CALL: +0x0] 
    /*00001808*/ CMP         R0, #0 
    /*0000180c*/ POPNE       {R4, R5, R6, PC} 
    /*00001810*/ LDR         R5, [R4, #16] 
    /*00001814*/ CMP         R5, #0 
    /*00001818*/ BLE         VEDU_DEC_Exit_x182c
    /*0000181c*/ SUB         R5, R5, #1 
    /*00001820*/ STR         R5, [R4, #16] 
    /*00001824*/ CMP         R5, #0 
    /*00001828*/ BEQ         VEDU_DEC_Exit_x1838
    VEDU_DEC_Exit_x182c:
    /*0000182c*/ LDR         R0, [PC, #52]               @ 0x0000000000001868 
    /*00001830*/ POP         {R4, R5, R6, LR} 
    VEDU_DEC_Exit_x1834:
    /*00001834*/ B           VEDU_DEC_Exit_x1834         @ osal_up [R_ARM_JUMP24: +0x0] 
    VEDU_DEC_Exit_x1838:
    /*00001838*/ LDR         R0, [R4, #4] 
    /*0000183c*/ BL          0x000000000000183C          @ VEDU_DEC_Destroy [R_ARM_CALL: .text+0x15d4] 
    /*00001840*/ LDR         R0, [R4] 
    /*00001844*/ STR         R5, [R4, #4] 
    /*00001848*/ BL          0x0000000000001848          @ VEDU_DEC_Destroy [R_ARM_CALL: .text+0x15d4] 
    /*0000184c*/ MOV         R0, R4 
    /*00001850*/ STR         R5, [R0], #8 
    /*00001854*/ BL          0x0000000000001854          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00001858*/ STR         R5, [R4, #16] 
    /*0000185c*/ LDR         R0, [PC, #4]                @ 0x0000000000001868 
    /*00001860*/ POP         {R4, R5, R6, LR} 
    VEDU_DEC_Exit_x1864:
    /*00001864*/ B           VEDU_DEC_Exit_x1864         @ osal_up [R_ARM_JUMP24: +0x0] 
    /*00001868*/ ANDEQ       R0, R0, R8, LSR #32 
FUNC_END VEDU_DEC_Exit


FUNC_BEGIN VEDU_DEC_Init
    /*0000186c*/ PUSH        {R4, R5, R6, LR} 
    /*00001870*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1c] 
    /*00001874*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1c] 
    /*00001878*/ SUB         SP, SP, #8 
    /*0000187c*/ MOV         R5, R0 
    /*00001880*/ ADD         R0, R4, #12 
    /*00001884*/ BL          0x0000000000001884          @ osal_down [R_ARM_CALL: +0x0] 
    /*00001888*/ SUBS        R6, R0, #0 
    /*0000188c*/ BNE         VEDU_DEC_Init_x194c
    /*00001890*/ LDR         R3, [R4, #16] 
    /*00001894*/ CMP         R3, #0 
    /*00001898*/ ADDGT       R3, R3, #1 
    /*0000189c*/ STRGT       R3, [R4, #16] 
    /*000018a0*/ BLE         VEDU_DEC_Init_x18b8
    VEDU_DEC_Init_x18a4:
    /*000018a4*/ LDR         R0, [PC, #168]              @ 0x0000000000001954 
    /*000018a8*/ BL          0x00000000000018A8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000018ac*/ MOV         R0, #0 
    VEDU_DEC_Init_x18b0:
    /*000018b0*/ ADD         SP, SP, #8 
    /*000018b4*/ POP         {R4, R5, R6, PC} 
    VEDU_DEC_Init_x18b8:
    /*000018b8*/ ADD         R0, R4, #8 
    /*000018bc*/ ADD         R3, R3, #1 
    /*000018c0*/ STR         R3, [R4, #16] 
    /*000018c4*/ BL          0x00000000000018C4          @ osal_spin_lock_init [R_ARM_CALL: +0x0] 
    /*000018c8*/ LDR         R2, [R5] 
    /*000018cc*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0xfc] 
    /*000018d0*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0xfc] 
    /*000018d4*/ MOV         R0, R6 
    /*000018d8*/ CMP         R2, #300                    @ 0x12c 
    /*000018dc*/ MOVCS       R2, #300                    @ 0x12c 
    /*000018e0*/ STR         R2, [R3] 
    /*000018e4*/ LDR         R2, [R5] 
    /*000018e8*/ CMP         R2, #200                    @ 0xc8 
    /*000018ec*/ MOVCS       R2, #200                    @ 0xc8 
    /*000018f0*/ STR         R2, [R3, #4] 
    /*000018f4*/ BL          0x00000000000018F4          @ VEDU_DEC_Create [R_ARM_CALL: .text+0x1648] 
    /*000018f8*/ CMP         R0, #0 
    /*000018fc*/ STR         R0, [R4] 
    /*00001900*/ BEQ         VEDU_DEC_Init_x1918
    /*00001904*/ MOV         R0, #1 
    /*00001908*/ BL          0x0000000000001908          @ VEDU_DEC_Create [R_ARM_CALL: .text+0x1648] 
    /*0000190c*/ CMP         R0, #0 
    /*00001910*/ STR         R0, [R4, #4] 
    /*00001914*/ BNE         VEDU_DEC_Init_x18a4
    VEDU_DEC_Init_x1918:
    /*00001918*/ MOV         R1, #25 
    /*0000191c*/ MOV         R3, #540                    @ 0x21c 
    /*00001920*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x618] 
    /*00001924*/ STR         R3, [SP] 
    /*00001928*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x618] 
    /*0000192c*/ LDR         R3, [PC, #36]               @ 0x0000000000001958 
    /*00001930*/ MOV         R0, #3 
    /*00001934*/ BL          0x0000000000001934          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001938*/ BL          0x0000000000001938          @ VEDU_DEC_Exit [R_ARM_CALL: .text+0x17f4] 
    /*0000193c*/ LDR         R0, [PC, #16]               @ 0x0000000000001954 
    /*00001940*/ BL          0x0000000000001940          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001944*/ MVN         R0, #0 
    /*00001948*/ B           VEDU_DEC_Init_x18b0
    VEDU_DEC_Init_x194c:
    /*0000194c*/ MVN         R0, #0 
    /*00001950*/ B           VEDU_DEC_Init_x18b0
    /*00001954*/ ANDEQ       R0, R0, R8, LSR #32 
    /*00001958*/ ANDEQ       R0, R0, R8, ROR #2 
FUNC_END VEDU_DEC_Init


FUNC_BEGIN VEDU_GetInfo
    /*0000195c*/ PUSH        {R4, R6, R7, LR} 
    /*00001960*/ SUBS        R4, R0, #0 
    /*00001964*/ BEQ         VEDU_GetInfo_x1990
    VEDU_GetInfo_x1968:
    /*00001968*/ MOV         R0, #1 
    /*0000196c*/ MOV         R3, #72                     @ 0x48 
    /*00001970*/ MOV         R7, #0 
    /*00001974*/ STM         R4, {R0, R3} 
    /*00001978*/ MOV         R6, #0 
    /*0000197c*/ MOV         R3, #65536                  @ 0x10000 
    /*00001980*/ MOVT        R6, #4432                   @ 0x1150 
    /*00001984*/ STR         R3, [R4, #16] 
    /*00001988*/ STRD        R6, [R4, #8] 
    /*0000198c*/ POP         {R4, R6, R7, PC} 
    VEDU_GetInfo_x1990:
    /*00001990*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x644] 
    /*00001994*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x178] 
    /*00001998*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x644] 
    /*0000199c*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x178] 
    /*000019a0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*000019a4*/ MOV         R2, #44                     @ 0x2c 
    /*000019a8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*000019ac*/ BL          0x00000000000019AC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000019b0*/ B           VEDU_GetInfo_x1968
FUNC_END VEDU_GetInfo


FUNC_BEGIN VEDU_DRV_ReadInt
    /*000019b4*/ PUSH        {R4, LR} 
    /*000019b8*/ SUBS        R4, R0, #0 
    /*000019bc*/ BEQ         VEDU_DRV_ReadInt_x19cc
    /*000019c0*/ MOV         R0, R4 
    /*000019c4*/ POP         {R4, LR} 
    VEDU_DRV_ReadInt_x19c8:
    /*000019c8*/ B           VEDU_DRV_ReadInt_x19c8      @ VEDU_HAL_ReadInt [R_ARM_JUMP24: .text+0x3234] 
    VEDU_DRV_ReadInt_x19cc:
    /*000019cc*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x69c] 
    /*000019d0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*000019d4*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x69c] 
    /*000019d8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*000019dc*/ MOV         R2, #60                     @ 0x3c 
    /*000019e0*/ LDR         R1, [PC, #12]               @ 0x00000000000019f4 
    /*000019e4*/ BL          0x00000000000019E4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000019e8*/ MOV         R0, R4 
    /*000019ec*/ POP         {R4, LR} 
    VEDU_DRV_ReadInt_x19f0:
    /*000019f0*/ B           VEDU_DRV_ReadInt_x19f0      @ VEDU_HAL_ReadInt [R_ARM_JUMP24: .text+0x3234] 
    /*000019f4*/ ANDEQ       R0, R0, R8, LSL #3 
FUNC_END VEDU_DRV_ReadInt


FUNC_BEGIN VEDU_DRV_ReadRawInt
    /*000019f8*/ PUSH        {R4, LR} 
    /*000019fc*/ SUBS        R4, R0, #0 
    /*00001a00*/ BEQ         VEDU_DRV_ReadRawInt_x1a10
    /*00001a04*/ MOV         R0, R4 
    /*00001a08*/ POP         {R4, LR} 
    VEDU_DRV_ReadRawInt_x1a0c:
    /*00001a0c*/ B           VEDU_DRV_ReadRawInt_x1a0c   @ VEDU_HAL_ReadRawInt [R_ARM_JUMP24: .text+0x34b0] 
    VEDU_DRV_ReadRawInt_x1a10:
    /*00001a10*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x69c] 
    /*00001a14*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00001a18*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x69c] 
    /*00001a1c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00001a20*/ MOV         R2, #67                     @ 0x43 
    /*00001a24*/ LDR         R1, [PC, #12]               @ 0x0000000000001a38 
    /*00001a28*/ BL          0x0000000000001A28          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001a2c*/ MOV         R0, R4 
    /*00001a30*/ POP         {R4, LR} 
    VEDU_DRV_ReadRawInt_x1a34:
    /*00001a34*/ B           VEDU_DRV_ReadRawInt_x1a34   @ VEDU_HAL_ReadRawInt [R_ARM_JUMP24: .text+0x34b0] 
    /*00001a38*/ MULEQ       R0, IP, R1 
FUNC_END VEDU_DRV_ReadRawInt


FUNC_BEGIN VEDU_DRV_ClearInt
    /*00001a3c*/ PUSH        {R4, LR} 
    /*00001a40*/ SUBS        R4, R0, #0 
    /*00001a44*/ BEQ         VEDU_DRV_ClearInt_x1a58
    /*00001a48*/ MOV         R0, R4 
    /*00001a4c*/ MVN         R1, #0 
    /*00001a50*/ POP         {R4, LR} 
    VEDU_DRV_ClearInt_x1a54:
    /*00001a54*/ B           VEDU_DRV_ClearInt_x1a54     @ VEDU_HAL_ClearInt [R_ARM_JUMP24: .text+0x372c] 
    VEDU_DRV_ClearInt_x1a58:
    /*00001a58*/ LDR         R1, [PC, #36]               @ 0x0000000000001a84 
    /*00001a5c*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x69c] 
    /*00001a60*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00001a64*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x69c] 
    /*00001a68*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00001a6c*/ MOV         R2, #75                     @ 0x4b 
    /*00001a70*/ BL          0x0000000000001A70          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001a74*/ MOV         R0, R4 
    /*00001a78*/ MVN         R1, #0 
    /*00001a7c*/ POP         {R4, LR} 
    VEDU_DRV_ClearInt_x1a80:
    /*00001a80*/ B           VEDU_DRV_ClearInt_x1a80     @ VEDU_HAL_ClearInt [R_ARM_JUMP24: .text+0x372c] 
    /*00001a84*/ @           <UNDEFINED> instruction: 0x000001b0 
FUNC_END VEDU_DRV_ClearInt


FUNC_BEGIN VEDU_DRV_ClearErrInt
    /*00001a88*/ PUSH        {R4, LR} 
    /*00001a8c*/ SUBS        R4, R0, #0 
    /*00001a90*/ BEQ         VEDU_DRV_ClearErrInt_x1aa4
    /*00001a94*/ MOV         R0, R4 
    /*00001a98*/ MVN         R1, #3 
    /*00001a9c*/ POP         {R4, LR} 
    VEDU_DRV_ClearErrInt_x1aa0:
    /*00001aa0*/ B           VEDU_DRV_ClearErrInt_x1aa0  @ VEDU_HAL_ClearInt [R_ARM_JUMP24: .text+0x372c] 
    VEDU_DRV_ClearErrInt_x1aa4:
    /*00001aa4*/ LDR         R1, [PC, #36]               @ 0x0000000000001ad0 
    /*00001aa8*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x69c] 
    /*00001aac*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00001ab0*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x69c] 
    /*00001ab4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00001ab8*/ MOV         R2, #86                     @ 0x56 
    /*00001abc*/ BL          0x0000000000001ABC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001ac0*/ MOV         R0, R4 
    /*00001ac4*/ MVN         R1, #3 
    /*00001ac8*/ POP         {R4, LR} 
    VEDU_DRV_ClearErrInt_x1acc:
    /*00001acc*/ B           VEDU_DRV_ClearErrInt_x1acc  @ VEDU_HAL_ClearInt [R_ARM_JUMP24: .text+0x372c] 
    /*00001ad0*/ ANDEQ       R0, R0, R4, ASR #3 
FUNC_END VEDU_DRV_ClearErrInt


FUNC_BEGIN VEDU_DRV_IsIntOk
    /*00001ad4*/ PUSH        {R4, R5, R6, LR} 
    /*00001ad8*/ SUBS        R5, R0, #0 
    /*00001adc*/ BEQ         VEDU_DRV_IsIntOk_x1b50
    VEDU_DRV_IsIntOk_x1ae0:
    /*00001ae0*/ MOV         R0, R5 
    /*00001ae4*/ BL          0x0000000000001AE4          @ VEDU_HAL_ReadBufFullInt [R_ARM_CALL: .text+0x32b0] 
    /*00001ae8*/ MOV         R4, R0 
    /*00001aec*/ MOV         R0, R5 
    /*00001af0*/ BL          0x0000000000001AF0          @ VEDU_HAL_ReadEndOfPicInt [R_ARM_CALL: .text+0x3270] 
    /*00001af4*/ ORR         R4, R4, R0 
    /*00001af8*/ MOV         R0, R5 
    /*00001afc*/ BL          0x0000000000001AFC          @ VEDU_HAL_ReadBitsOverInt [R_ARM_CALL: .text+0x32f0] 
    /*00001b00*/ ORR         R4, R4, R0 
    /*00001b04*/ MOV         R0, R5 
    /*00001b08*/ BL          0x0000000000001B08          @ VEDU_HAL_ReadAxiWrBusErrInt [R_ARM_CALL: .text+0x3330] 
    /*00001b0c*/ ORR         R4, R4, R0 
    /*00001b10*/ MOV         R0, R5 
    /*00001b14*/ BL          0x0000000000001B14          @ VEDU_HAL_ReadAxiRdBusErrInt [R_ARM_CALL: .text+0x3370] 
    /*00001b18*/ ORR         R4, R4, R0 
    /*00001b1c*/ MOV         R0, R5 
    /*00001b20*/ BL          0x0000000000001B20          @ VEDU_HAL_ReadTimeoutInt [R_ARM_CALL: .text+0x33b0] 
    /*00001b24*/ ORR         R4, R4, R0 
    /*00001b28*/ MOV         R0, R5 
    /*00001b2c*/ BL          0x0000000000001B2C          @ VEDU_HAL_ReadLineNumInt [R_ARM_CALL: .text+0x33f0] 
    /*00001b30*/ ORR         R4, R4, R0 
    /*00001b34*/ MOV         R0, R5 
    /*00001b38*/ BL          0x0000000000001B38          @ VEDU_HAL_ReadSedErrCoreInt [R_ARM_CALL: .text+0x3430] 
    /*00001b3c*/ ORR         R4, R4, R0 
    /*00001b40*/ MOV         R0, R5 
    /*00001b44*/ BL          0x0000000000001B44          @ VEDU_HAL_ReadSedErrExtInt [R_ARM_CALL: .text+0x3470] 
    /*00001b48*/ ORR         R0, R4, R0 
    /*00001b4c*/ POP         {R4, R5, R6, PC} 
    VEDU_DRV_IsIntOk_x1b50:
    /*00001b50*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x69c] 
    /*00001b54*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00001b58*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x69c] 
    /*00001b5c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00001b60*/ MOV         R2, #99                     @ 0x63 
    /*00001b64*/ LDR         R1, [PC, #4]                @ 0x0000000000001b70 
    /*00001b68*/ BL          0x0000000000001B68          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001b6c*/ B           VEDU_DRV_IsIntOk_x1ae0
    /*00001b70*/ LDRDEQ      R0, [R0], -IP 
FUNC_END VEDU_DRV_IsIntOk


FUNC_BEGIN VEDU_DRV_IsNeedReset
    /*00001b74*/ PUSH        {R4, R5, R6, LR} 
    /*00001b78*/ SUBS        R5, R0, #0 
    /*00001b7c*/ SUB         SP, SP, #8 
    /*00001b80*/ BEQ         VEDU_DRV_IsNeedReset_x1c20
    VEDU_DRV_IsNeedReset_x1b84:
    /*00001b84*/ MOV         R0, R5 
    /*00001b88*/ BL          0x0000000000001B88          @ VEDU_HAL_ReadBitsOverRawInt [R_ARM_CALL: .text+0x356c] 
    /*00001b8c*/ MOV         R4, R0 
    /*00001b90*/ MOV         R0, R5 
    /*00001b94*/ BL          0x0000000000001B94          @ VEDU_HAL_ReadTimeoutRawInt [R_ARM_CALL: .text+0x362c] 
    /*00001b98*/ ORR         R4, R4, R0 
    /*00001b9c*/ MOV         R0, R5 
    /*00001ba0*/ BL          0x0000000000001BA0          @ VEDU_HAL_ReadSedErrCoreRawInt [R_ARM_CALL: .text+0x36ac] 
    /*00001ba4*/ ORR         R4, R0, R4 
    /*00001ba8*/ MOV         R0, R5 
    /*00001bac*/ BL          0x0000000000001BAC          @ VEDU_HAL_ReadSedErrExtRawInt [R_ARM_CALL: .text+0x36ec] 
    /*00001bb0*/ ORR         R4, R0, R4 
    /*00001bb4*/ MOV         R0, R5 
    /*00001bb8*/ BL          0x0000000000001BB8          @ VEDU_HAL_ReadAxiWrBusErrRawInt [R_ARM_CALL: .text+0x35ac] 
    /*00001bbc*/ MOV         R6, R0 
    /*00001bc0*/ MOV         R0, R5 
    /*00001bc4*/ BL          0x0000000000001BC4          @ VEDU_HAL_ReadAxiRdBusErrRawInt [R_ARM_CALL: .text+0x35ec] 
    /*00001bc8*/ ORR         R4, R6, R4 
    /*00001bcc*/ ORRS        R4, R4, R0 
    /*00001bd0*/ BEQ         VEDU_DRV_IsNeedReset_x1c14
    /*00001bd4*/ LDR         R1, [R5, #16] 
    /*00001bd8*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6b4] 
    /*00001bdc*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6b4] 
    /*00001be0*/ MOVW        R3, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6ac] 
    /*00001be4*/ TST         R1, #1 
    /*00001be8*/ MOVT        R3, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6ac] 
    /*00001bec*/ MOV         R1, #154                    @ 0x9a 
    /*00001bf0*/ MOV         R0, #3 
    /*00001bf4*/ MOVEQ       R3, R2 
    /*00001bf8*/ STR         R1, [SP] 
    /*00001bfc*/ STR         R3, [SP, #4] 
    /*00001c00*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6bc] 
    /*00001c04*/ LDR         R3, [PC, #52]               @ 0x0000000000001c40 
    /*00001c08*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6bc] 
    /*00001c0c*/ MOV         R1, #25 
    /*00001c10*/ BL          0x0000000000001C10          @ HI_LOG [R_ARM_CALL: +0x0] 
    VEDU_DRV_IsNeedReset_x1c14:
    /*00001c14*/ MOV         R0, R4 
    /*00001c18*/ ADD         SP, SP, #8 
    /*00001c1c*/ POP         {R4, R5, R6, PC} 
    VEDU_DRV_IsNeedReset_x1c20:
    /*00001c20*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x69c] 
    /*00001c24*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00001c28*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x69c] 
    /*00001c2c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00001c30*/ MOV         R2, #120                    @ 0x78 
    /*00001c34*/ LDR         R1, [PC, #4]                @ 0x0000000000001c40 
    /*00001c38*/ BL          0x0000000000001C38          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001c3c*/ B           VEDU_DRV_IsNeedReset_x1b84
    /*00001c40*/ STRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
FUNC_END VEDU_DRV_IsNeedReset


FUNC_BEGIN VEDU_DRV_ResetRequest
    VEDU_DRV_ResetRequest_x1c44:
    /*00001c44*/ B           VEDU_DRV_ResetRequest_x1c44 @ VEDU_HAL_ResetRequest [R_ARM_JUMP24: .text+0x376c] 
FUNC_END VEDU_DRV_ResetRequest


FUNC_BEGIN VEDU_DRV_ReadBusIdleFlag
    VEDU_DRV_ReadBusIdleFlag_x1c48:
    /*00001c48*/ B           VEDU_DRV_ReadBusIdleFlag_x1c48@ VEDU_HAL_ReadBusIdleFlag [R_ARM_JUMP24: .text+0x37a8] 
FUNC_END VEDU_DRV_ReadBusIdleFlag


FUNC_BEGIN VEDU_DRV_StartVedu
    /*00001c4c*/ PUSH        {R4, LR} 
    /*00001c50*/ SUBS        R4, R0, #0 
    /*00001c54*/ BEQ         VEDU_DRV_StartVedu_x1c64
    /*00001c58*/ MOV         R0, R4 
    /*00001c5c*/ POP         {R4, LR} 
    VEDU_DRV_StartVedu_x1c60:
    /*00001c60*/ B           VEDU_DRV_StartVedu_x1c60    @ VEDU_HAL_StartVedu [R_ARM_JUMP24: .text+0x37e8] 
    VEDU_DRV_StartVedu_x1c64:
    /*00001c64*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x69c] 
    /*00001c68*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00001c6c*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x69c] 
    /*00001c70*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00001c74*/ MOV         R2, #172                    @ 0xac 
    /*00001c78*/ LDR         R1, [PC, #12]               @ 0x0000000000001c8c 
    /*00001c7c*/ BL          0x0000000000001C7C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001c80*/ MOV         R0, R4 
    /*00001c84*/ POP         {R4, LR} 
    VEDU_DRV_StartVedu_x1c88:
    /*00001c88*/ B           VEDU_DRV_StartVedu_x1c88    @ VEDU_HAL_StartVedu [R_ARM_JUMP24: .text+0x37e8] 
    /*00001c8c*/ ANDEQ       R0, R0, R8, LSL #4 
FUNC_END VEDU_DRV_StartVedu


FUNC_BEGIN VEDU_DRV_InitIntMask
    /*00001c90*/ MOV         R1, #0 
    VEDU_DRV_InitIntMask_x1c94:
    /*00001c94*/ B           VEDU_DRV_InitIntMask_x1c94  @ VEDU_HAL_WriteIntMask [R_ARM_JUMP24: .text+0x3864] 
FUNC_END VEDU_DRV_InitIntMask


FUNC_BEGIN VEDU_DRV_SetPowerMode
    /*00001c98*/ PUSH        {R4, R5, R6, LR} 
    /*00001c9c*/ SUBS        R4, R0, #0 
    /*00001ca0*/ MOV         R5, R1 
    /*00001ca4*/ BEQ         VEDU_DRV_SetPowerMode_x1d00
    VEDU_DRV_SetPowerMode_x1ca8:
    /*00001ca8*/ CMP         R5, #1 
    /*00001cac*/ BEQ         VEDU_DRV_SetPowerMode_x1cd8
    /*00001cb0*/ MOV         R0, R4 
    /*00001cb4*/ MOV         R1, #0 
    /*00001cb8*/ BL          0x0000000000001CB8          @ VEDU_HAL_WriteClkGateMode [R_ARM_CALL: .text+0x3920] 
    /*00001cbc*/ MOV         R0, R4 
    /*00001cc0*/ MOV         R1, #0 
    /*00001cc4*/ BL          0x0000000000001CC4          @ VEDU_HAL_WriteMemClkGateMode [R_ARM_CALL: .text+0x3964] 
    /*00001cc8*/ MOV         R0, R4 
    /*00001ccc*/ MOV         R1, #0 
    /*00001cd0*/ POP         {R4, R5, R6, LR} 
    VEDU_DRV_SetPowerMode_x1cd4:
    /*00001cd4*/ B           VEDU_DRV_SetPowerMode_x1cd4 @ VEDU_HAL_SetPowerMode [R_ARM_JUMP24: .text+0x39a8] 
    VEDU_DRV_SetPowerMode_x1cd8:
    /*00001cd8*/ MOV         R0, R4 
    /*00001cdc*/ MOV         R1, #2 
    /*00001ce0*/ BL          0x0000000000001CE0          @ VEDU_HAL_WriteClkGateMode [R_ARM_CALL: .text+0x3920] 
    /*00001ce4*/ MOV         R1, R5 
    /*00001ce8*/ MOV         R0, R4 
    /*00001cec*/ BL          0x0000000000001CEC          @ VEDU_HAL_WriteMemClkGateMode [R_ARM_CALL: .text+0x3964] 
    /*00001cf0*/ MOV         R1, R5 
    /*00001cf4*/ MOV         R0, R4 
    /*00001cf8*/ POP         {R4, R5, R6, LR} 
    VEDU_DRV_SetPowerMode_x1cfc:
    /*00001cfc*/ B           VEDU_DRV_SetPowerMode_x1cfc @ VEDU_HAL_SetPowerMode [R_ARM_JUMP24: .text+0x39a8] 
    VEDU_DRV_SetPowerMode_x1d00:
    /*00001d00*/ MOVW        R3, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6f0] 
    /*00001d04*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00001d08*/ MOVT        R3, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6f0] 
    /*00001d0c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00001d10*/ MOV         R2, #200                    @ 0xc8 
    /*00001d14*/ LDR         R1, [PC, #4]                @ 0x0000000000001d20 
    /*00001d18*/ BL          0x0000000000001D18          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001d1c*/ B           VEDU_DRV_SetPowerMode_x1ca8
    /*00001d20*/ ANDEQ       R0, R0, IP, LSL R2 
FUNC_END VEDU_DRV_SetPowerMode


FUNC_BEGIN VEDU_DRV_ClkandPwrUp
    /*00001d24*/ PUSH        {R4, LR} 
    /*00001d28*/ MOV         R1, #1 
    /*00001d2c*/ MOV         R4, R0 
    /*00001d30*/ BL          0x0000000000001D30          @ VEDU_SetClk [R_ARM_CALL: .text+0xc4c] 
    /*00001d34*/ MOV         R0, R4 
    /*00001d38*/ MOV         R1, #0 
    /*00001d3c*/ BL          0x0000000000001D3C          @ VEDU_RstSel [R_ARM_CALL: .text+0xc94] 
    /*00001d40*/ MOV         R0, #0 
    /*00001d44*/ POP         {R4, PC} 
FUNC_END VEDU_DRV_ClkandPwrUp


FUNC_BEGIN VEDU_DRV_ClkandPwrDown
    /*00001d48*/ PUSH        {R4, LR} 
    /*00001d4c*/ MOV         R1, #1 
    /*00001d50*/ MOV         R4, R0 
    /*00001d54*/ BL          0x0000000000001D54          @ VEDU_SetClk [R_ARM_CALL: .text+0xc4c] 
    /*00001d58*/ MOV         R0, R4 
    /*00001d5c*/ MOV         R1, #0 
    /*00001d60*/ BL          0x0000000000001D60          @ VEDU_RstSel [R_ARM_CALL: .text+0xc94] 
    /*00001d64*/ MOV         R0, R4 
    /*00001d68*/ MOV         R1, #0 
    /*00001d6c*/ BL          0x0000000000001D6C          @ VEDU_SetClk [R_ARM_CALL: .text+0xc4c] 
    /*00001d70*/ MOV         R0, #0 
    /*00001d74*/ POP         {R4, PC} 
FUNC_END VEDU_DRV_ClkandPwrDown


FUNC_BEGIN VEDU_DRV_Init
    /*00001d78*/ PUSH        {R4, LR} 
    /*00001d7c*/ SUBS        R4, R0, #0 
    /*00001d80*/ BEQ         VEDU_DRV_Init_x1db4
    VEDU_DRV_Init_x1d84:
    /*00001d84*/ MOV         R0, R4 
    /*00001d88*/ MOV         R1, #0 
    /*00001d8c*/ BL          0x0000000000001D8C          @ VEDU_HAL_WriteIntMask [R_ARM_CALL: .text+0x3864] 
    /*00001d90*/ MOV         R1, #1 
    /*00001d94*/ MOV         R0, R4 
    /*00001d98*/ BL          0x0000000000001D98          @ VEDU_DRV_SetPowerMode [R_ARM_CALL: .text+0x1c98] 
    /*00001d9c*/ MOV         R0, R4 
    /*00001da0*/ BL          0x0000000000001DA0          @ VEDU_HAL_WriteOutStanding [R_ARM_CALL: .text+0x3aac] 
    /*00001da4*/ MOV         R0, R4 
    /*00001da8*/ MOV         R1, #2 
    /*00001dac*/ POP         {R4, LR} 
    VEDU_DRV_Init_x1db0:
    /*00001db0*/ B           VEDU_DRV_Init_x1db0         @ VEDU_HAL_WriteTimerMode [R_ARM_JUMP24: .text+0x3a00] 
    VEDU_DRV_Init_x1db4:
    /*00001db4*/ MOVW        R3, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6f0] 
    /*00001db8*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00001dbc*/ MOVT        R3, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6f0] 
    /*00001dc0*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00001dc4*/ MOV         R2, #243                    @ 0xf3 
    /*00001dc8*/ LDR         R1, [PC, #4]                @ 0x0000000000001dd4 
    /*00001dcc*/ BL          0x0000000000001DCC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001dd0*/ B           VEDU_DRV_Init_x1d84
    /*00001dd4*/ ANDEQ       R0, R0, R4, LSR R2 
FUNC_END VEDU_DRV_Init


FUNC_BEGIN VEDU_DRV_Exit
    /*00001dd8*/ BX          LR 
FUNC_END VEDU_DRV_Exit


FUNC_BEGIN VEDU_DRV_UpdateROI
    /*00001ddc*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00001de0*/ SUBS        R4, R0, #0 
    /*00001de4*/ MOV         R5, R1 
    /*00001de8*/ MOV         R6, R2 
    /*00001dec*/ BEQ         VEDU_DRV_UpdateROI_x2078
    VEDU_DRV_UpdateROI_x1df0:
    /*00001df0*/ CMP         R5, #0 
    /*00001df4*/ BEQ         VEDU_DRV_UpdateROI_x2098
    VEDU_DRV_UpdateROI_x1df8:
    /*00001df8*/ CMP         R6, #1 
    /*00001dfc*/ BEQ         VEDU_DRV_UpdateROI_x2068
    /*00001e00*/ ADD         R0, R5, #256                @ 0x100 
    /*00001e04*/ MOV         R3, R5 
    /*00001e08*/ MOV         R2, R5 
    /*00001e0c*/ MOV         R1, #0 
    /*00001e10*/ B           VEDU_DRV_UpdateROI_x1e2c
    VEDU_DRV_UpdateROI_x1e14:
    /*00001e14*/ ADD         R2, R2, #32 
    /*00001e18*/ CMP         R2, R0 
    /*00001e1c*/ BEQ         VEDU_DRV_UpdateROI_x1e54
    /*00001e20*/ ADD         R2, R2, #32 
    /*00001e24*/ CMP         R2, R0 
    /*00001e28*/ BEQ         VEDU_DRV_UpdateROI_x1e54
    VEDU_DRV_UpdateROI_x1e2c:
    /*00001e2c*/ CMP         R1, #0 
    /*00001e30*/ MOV         IP, #1 
    /*00001e34*/ BNE         VEDU_DRV_UpdateROI_x1e14
    /*00001e38*/ LDR         R1, [R2, #4] 
    /*00001e3c*/ ADD         R2, R2, #32 
    /*00001e40*/ ADDS        R1, R1, #0 
    /*00001e44*/ MOVNE       R1, #1 
    /*00001e48*/ CMP         R2, R0 
    /*00001e4c*/ MOV         IP, R1 
    /*00001e50*/ BNE         VEDU_DRV_UpdateROI_x1e2c
    VEDU_DRV_UpdateROI_x1e54:
    /*00001e54*/ LDRB        R1, [R4, #1667]             @ 0x683 
    /*00001e58*/ MOV         R0, #0 
    /*00001e5c*/ LDRB        R2, [R4, #1664]             @ 0x680 
    /*00001e60*/ MOVW        LR, #418                    @ 0x1a2 
    /*00001e64*/ BFI         R1, IP, #0, #1 
    /*00001e68*/ STRB        R1, [R4, #1667]             @ 0x683 
    /*00001e6c*/ LDR         R1, [R5, #4] 
    /*00001e70*/ MOVW        IP, #426                    @ 0x1aa 
    /*00001e74*/ BFI         R2, R1, #0, #1 
    /*00001e78*/ STRB        R2, [R4, #1664]             @ 0x680 
    /*00001e7c*/ LDR         R1, [R5, #36]               @ 0x24 
    /*00001e80*/ UXTB        R2, R2 
    /*00001e84*/ BFI         R2, R1, #1, #1 
    /*00001e88*/ STRB        R2, [R4, #1664]             @ 0x680 
    /*00001e8c*/ LDR         R1, [R5, #68]               @ 0x44 
    /*00001e90*/ UXTB        R2, R2 
    /*00001e94*/ BFI         R2, R1, #2, #1 
    /*00001e98*/ STRB        R2, [R4, #1664]             @ 0x680 
    /*00001e9c*/ LDR         R1, [R5, #100]              @ 0x64 
    /*00001ea0*/ UXTB        R2, R2 
    /*00001ea4*/ BFI         R2, R1, #3, #1 
    /*00001ea8*/ STRB        R2, [R4, #1664]             @ 0x680 
    /*00001eac*/ LDR         R1, [R5, #132]              @ 0x84 
    /*00001eb0*/ UXTB        R2, R2 
    /*00001eb4*/ BFI         R2, R1, #4, #1 
    /*00001eb8*/ STRB        R2, [R4, #1664]             @ 0x680 
    /*00001ebc*/ LDR         R1, [R5, #164]              @ 0xa4 
    /*00001ec0*/ UXTB        R2, R2 
    /*00001ec4*/ BFI         R2, R1, #5, #1 
    /*00001ec8*/ STRB        R2, [R4, #1664]             @ 0x680 
    /*00001ecc*/ LDR         R1, [R5, #196]              @ 0xc4 
    /*00001ed0*/ UXTB        R2, R2 
    /*00001ed4*/ BFI         R2, R1, #6, #1 
    /*00001ed8*/ STRB        R2, [R4, #1664]             @ 0x680 
    /*00001edc*/ LDR         R2, [R5, #228]              @ 0xe4 
    /*00001ee0*/ LDRB        R1, [R4, #1664]             @ 0x680 
    /*00001ee4*/ BFI         R1, R2, #7, #1 
    /*00001ee8*/ LDRB        R2, [R4, #1665]             @ 0x681 
    /*00001eec*/ STRB        R1, [R4, #1664]             @ 0x680 
    /*00001ef0*/ LDR         R1, [R5, #8] 
    /*00001ef4*/ BFI         R2, R1, #0, #1 
    /*00001ef8*/ STRB        R2, [R4, #1665]             @ 0x681 
    /*00001efc*/ LDR         R1, [R5, #40]               @ 0x28 
    /*00001f00*/ UXTB        R2, R2 
    /*00001f04*/ BFI         R2, R1, #1, #1 
    /*00001f08*/ STRB        R2, [R4, #1665]             @ 0x681 
    /*00001f0c*/ LDR         R1, [R5, #72]               @ 0x48 
    /*00001f10*/ UXTB        R2, R2 
    /*00001f14*/ BFI         R2, R1, #2, #1 
    /*00001f18*/ STRB        R2, [R4, #1665]             @ 0x681 
    /*00001f1c*/ LDR         R1, [R5, #104]              @ 0x68 
    /*00001f20*/ UXTB        R2, R2 
    /*00001f24*/ BFI         R2, R1, #3, #1 
    /*00001f28*/ STRB        R2, [R4, #1665]             @ 0x681 
    /*00001f2c*/ LDR         R1, [R5, #136]              @ 0x88 
    /*00001f30*/ UXTB        R2, R2 
    /*00001f34*/ BFI         R2, R1, #4, #1 
    /*00001f38*/ STRB        R2, [R4, #1665]             @ 0x681 
    /*00001f3c*/ LDR         R1, [R5, #168]              @ 0xa8 
    /*00001f40*/ UXTB        R2, R2 
    /*00001f44*/ BFI         R2, R1, #5, #1 
    /*00001f48*/ STRB        R2, [R4, #1665]             @ 0x681 
    /*00001f4c*/ LDR         R1, [R5, #200]              @ 0xc8 
    /*00001f50*/ UXTB        R2, R2 
    /*00001f54*/ BFI         R2, R1, #6, #1 
    /*00001f58*/ STRB        R2, [R4, #1665]             @ 0x681 
    /*00001f5c*/ LDR         R2, [R5, #232]              @ 0xe8 
    /*00001f60*/ LDRB        R1, [R4, #1665]             @ 0x681 
    /*00001f64*/ BFI         R1, R2, #7, #1 
    /*00001f68*/ LDRB        R2, [R4, #1668]             @ 0x684 
    /*00001f6c*/ STRB        R1, [R4, #1665]             @ 0x681 
    /*00001f70*/ LDR         R1, [R5, #12] 
    /*00001f74*/ BFI         R2, R1, #0, #7 
    /*00001f78*/ LDRB        R1, [R4, #1669]             @ 0x685 
    /*00001f7c*/ STRB        R2, [R4, #1668]             @ 0x684 
    /*00001f80*/ LDR         R2, [R5, #44]               @ 0x2c 
    /*00001f84*/ BFI         R1, R2, #0, #7 
    /*00001f88*/ LDRB        R2, [R4, #1670]             @ 0x686 
    /*00001f8c*/ STRB        R1, [R4, #1669]             @ 0x685 
    /*00001f90*/ LDR         R1, [R5, #76]               @ 0x4c 
    /*00001f94*/ BFI         R2, R1, #0, #7 
    /*00001f98*/ LDRB        R1, [R4, #1671]             @ 0x687 
    /*00001f9c*/ STRB        R2, [R4, #1670]             @ 0x686 
    /*00001fa0*/ LDR         R2, [R5, #108]              @ 0x6c 
    /*00001fa4*/ BFI         R1, R2, #0, #7 
    /*00001fa8*/ LDRB        R2, [R4, #1672]             @ 0x688 
    /*00001fac*/ STRB        R1, [R4, #1671]             @ 0x687 
    /*00001fb0*/ LDR         R1, [R5, #140]              @ 0x8c 
    /*00001fb4*/ BFI         R2, R1, #0, #7 
    /*00001fb8*/ LDRB        R1, [R4, #1673]             @ 0x689 
    /*00001fbc*/ STRB        R2, [R4, #1672]             @ 0x688 
    /*00001fc0*/ LDR         R2, [R5, #172]              @ 0xac 
    /*00001fc4*/ BFI         R1, R2, #0, #7 
    /*00001fc8*/ LDRB        R2, [R4, #1674]             @ 0x68a 
    /*00001fcc*/ STRB        R1, [R4, #1673]             @ 0x689 
    /*00001fd0*/ LDR         R1, [R5, #204]              @ 0xcc 
    /*00001fd4*/ BFI         R2, R1, #0, #7 
    /*00001fd8*/ STRB        R2, [R4, #1674]             @ 0x68a 
    /*00001fdc*/ LDRB        R2, [R4, #1675]             @ 0x68b 
    /*00001fe0*/ LDR         R1, [R5, #236]              @ 0xec 
    /*00001fe4*/ BFI         R2, R1, #0, #7 
    /*00001fe8*/ STRB        R2, [R4, #1675]             @ 0x68b 
    VEDU_DRV_UpdateROI_x1fec:
    /*00001fec*/ ADD         R1, R0, LR 
    /*00001ff0*/ LDR         R6, [R3, #24] 
    /*00001ff4*/ ADD         R2, R0, IP 
    /*00001ff8*/ ADD         R0, R0, #1 
    /*00001ffc*/ ADD         R1, R4, R1, LSL #2 
    /*00002000*/ CMP         R0, #8 
    /*00002004*/ LSR         R6, R6, #4 
    /*00002008*/ ADD         R7, R1, #4 
    /*0000200c*/ LDRH        R8, [R1, #4] 
    /*00002010*/ ADD         R2, R4, R2, LSL #2 
    /*00002014*/ ADD         R5, R2, #4 
    /*00002018*/ ADD         R3, R3, #32 
    /*0000201c*/ BFI         R8, R6, #0, #13 
    /*00002020*/ STRH        R8, [R1, #4] 
    /*00002024*/ LDR         R1, [R3, #-4] 
    /*00002028*/ LDRH        R6, [R7, #2] 
    /*0000202c*/ LSR         R1, R1, #4 
    /*00002030*/ BFI         R6, R1, #0, #13 
    /*00002034*/ STRH        R6, [R7, #2] 
    /*00002038*/ LDR         R1, [R3, #-16] 
    /*0000203c*/ LDRH        R6, [R2, #4] 
    /*00002040*/ LSR         R1, R1, #4 
    /*00002044*/ BFI         R6, R1, #0, #12 
    /*00002048*/ STRH        R6, [R2, #4] 
    /*0000204c*/ LDR         R2, [R3, #-12] 
    /*00002050*/ LDRH        R1, [R5, #2] 
    /*00002054*/ LSR         R2, R2, #4 
    /*00002058*/ BFI         R1, R2, #0, #12 
    /*0000205c*/ STRH        R1, [R5, #2] 
    /*00002060*/ BNE         VEDU_DRV_UpdateROI_x1fec
    /*00002064*/ POP         {R4, R5, R6, R7, R8, PC} 
    VEDU_DRV_UpdateROI_x2068:
    /*00002068*/ LDRB        R3, [R4, #1667]             @ 0x683 
    /*0000206c*/ BFC         R3, #0, #1 
    /*00002070*/ STRB        R3, [R4, #1667]             @ 0x683 
    /*00002074*/ POP         {R4, R5, R6, R7, R8, PC} 
    VEDU_DRV_UpdateROI_x2078:
    /*00002078*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x708] 
    /*0000207c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002080*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x708] 
    /*00002084*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00002088*/ MOVW        R2, #273                    @ 0x111 
    /*0000208c*/ LDR         R1, [PC, #36]               @ 0x00000000000020b8 
    /*00002090*/ BL          0x0000000000002090          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002094*/ B           VEDU_DRV_UpdateROI_x1df0
    VEDU_DRV_UpdateROI_x2098:
    /*00002098*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x71c] 
    /*0000209c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*000020a0*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x71c] 
    /*000020a4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*000020a8*/ MOVW        R2, #274                    @ 0x112 
    /*000020ac*/ LDR         R1, [PC, #4]                @ 0x00000000000020b8 
    /*000020b0*/ BL          0x00000000000020B0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000020b4*/ B           VEDU_DRV_UpdateROI_x1df8
    /*000020b8*/ ANDEQ       R0, R0, R4, ASR #4 
FUNC_END VEDU_DRV_UpdateROI


FUNC_BEGIN VEDU_DRV_UpdateOSD
    /*000020bc*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000020c0*/ SUB         SP, SP, #244                @ 0xf4 
    /*000020c4*/ MOV         R4, R1 
    /*000020c8*/ MOV         R2, #32 
    /*000020cc*/ MOV         R1, #0 
    /*000020d0*/ MOV         R5, R0 
    /*000020d4*/ ADD         R0, SP, #112                @ 0x70 
    /*000020d8*/ BL          0x00000000000020D8          @ memset [R_ARM_CALL: +0x0] 
    /*000020dc*/ MOV         R2, #32 
    /*000020e0*/ MOV         R1, #0 
    /*000020e4*/ ADD         R0, SP, #144                @ 0x90 
    /*000020e8*/ BL          0x00000000000020E8          @ memset [R_ARM_CALL: +0x0] 
    /*000020ec*/ MOV         R2, #32 
    /*000020f0*/ MOV         R1, #0 
    /*000020f4*/ ADD         R0, SP, #176                @ 0xb0 
    /*000020f8*/ BL          0x00000000000020F8          @ memset [R_ARM_CALL: +0x0] 
    /*000020fc*/ MOV         R2, #32 
    /*00002100*/ MOV         R1, #0 
    /*00002104*/ ADD         R0, SP, #208                @ 0xd0 
    /*00002108*/ BL          0x0000000000002108          @ memset [R_ARM_CALL: +0x0] 
    /*0000210c*/ CMP         R4, #0 
    /*00002110*/ BEQ         VEDU_DRV_UpdateOSD_x291c
    VEDU_DRV_UpdateOSD_x2114:
    /*00002114*/ CMP         R5, #0 
    /*00002118*/ BEQ         VEDU_DRV_UpdateOSD_x293c
    VEDU_DRV_UpdateOSD_x211c:
    /*0000211c*/ LDR         R2, [R4, #8] 
    /*00002120*/ CMP         R2, #0 
    /*00002124*/ BNE         VEDU_DRV_UpdateOSD_x282c
    /*00002128*/ LDRB        R3, [R5, #572]              @ 0x23c 
    /*0000212c*/ BFI         R3, R2, #0, #1 
    /*00002130*/ BFI         R3, R2, #1, #1 
    /*00002134*/ BFI         R3, R2, #2, #1 
    /*00002138*/ BFI         R3, R2, #3, #1 
    /*0000213c*/ BFI         R3, R2, #4, #1 
    /*00002140*/ BFI         R3, R2, #5, #1 
    /*00002144*/ BFI         R3, R2, #6, #1 
    /*00002148*/ BFI         R3, R2, #7, #1 
    /*0000214c*/ STRB        R3, [R5, #572]              @ 0x23c 
    /*00002150*/ LDR         R3, [R4, #116]              @ 0x74 
    /*00002154*/ CMP         R3, #8 
    /*00002158*/ BEQ         VEDU_DRV_UpdateOSD_x2174
    /*0000215c*/ CMP         R3, #9 
    /*00002160*/ BEQ         VEDU_DRV_UpdateOSD_x2870
    /*00002164*/ CMP         R3, #11 
    /*00002168*/ BEQ         VEDU_DRV_UpdateOSD_x2884
    /*0000216c*/ CMP         R3, #12 
    /*00002170*/ BEQ         VEDU_DRV_UpdateOSD_x2860
    VEDU_DRV_UpdateOSD_x2174:
    /*00002174*/ LDRB        R3, [R5, #575]              @ 0x23f 
    /*00002178*/ BFC         R3, #6, #2 
    /*0000217c*/ STRB        R3, [R5, #575]              @ 0x23f 
    VEDU_DRV_UpdateOSD_x2180:
    /*00002180*/ LDR         IP, [R4, #16] 
    /*00002184*/ CMP         IP, #0 
    /*00002188*/ BEQ         VEDU_DRV_UpdateOSD_x2898
    /*0000218c*/ MOV         R3, #96                     @ 0x60 
    /*00002190*/ ADD         LR, SP, #172                @ 0xac 
    /*00002194*/ ADD         R0, SP, #204                @ 0xcc 
    /*00002198*/ ADD         R1, SP, #108                @ 0x6c 
    /*0000219c*/ ADD         R2, SP, #140                @ 0x8c 
    /*000021a0*/ MOV         R9, #1 
    /*000021a4*/ MLA         IP, R3, IP, R4 
    /*000021a8*/ MOV         R3, R4 
    VEDU_DRV_UpdateOSD_x21ac:
    /*000021ac*/ LDR         R8, [R3, #68]               @ 0x44 
    /*000021b0*/ ADD         R3, R3, #96                 @ 0x60 
    /*000021b4*/ LDR         R7, [R3, #-24]              @ 0xffffffe8 
    /*000021b8*/ LDR         R6, [R3, #-20]              @ 0xffffffec 
    /*000021bc*/ CMP         IP, R3 
    /*000021c0*/ STR         R9, [R1, #4]! 
    /*000021c4*/ STR         R8, [LR, #4]! 
    /*000021c8*/ STR         R7, [R0, #4]! 
    /*000021cc*/ STR         R6, [R2, #4]! 
    /*000021d0*/ BNE         VEDU_DRV_UpdateOSD_x21ac
    /*000021d4*/ LDR         FP, [SP, #176]              @ 0xb0 
    /*000021d8*/ MOV         R3, #1 
    /*000021dc*/ LDR         SL, [SP, #180]              @ 0xb4 
    /*000021e0*/ STR         R3, [SP, #80]               @ 0x50 
    /*000021e4*/ AND         FP, FP, R3 
    /*000021e8*/ LDR         R1, [SP, #196]              @ 0xc4 
    /*000021ec*/ AND         SL, SL, R3 
    /*000021f0*/ LDR         R2, [SP, #200]              @ 0xc8 
    /*000021f4*/ LDR         R3, [SP, #192]              @ 0xc0 
    /*000021f8*/ AND         R1, R1, #1 
    /*000021fc*/ AND         R2, R2, #1 
    /*00002200*/ STR         R1, [SP, #4] 
    /*00002204*/ AND         R3, R3, #1 
    /*00002208*/ LDR         R1, [SP, #112]              @ 0x70 
    /*0000220c*/ STR         R3, [SP] 
    /*00002210*/ STR         R2, [SP, #8] 
    /*00002214*/ AND         R1, R1, #1 
    /*00002218*/ LDR         R3, [SP, #204]              @ 0xcc 
    /*0000221c*/ LDR         R2, [SP, #116]              @ 0x74 
    /*00002220*/ AND         R3, R3, #1 
    /*00002224*/ STR         R1, [SP, #16] 
    /*00002228*/ STR         R3, [SP, #12] 
    /*0000222c*/ AND         R2, R2, #1 
    /*00002230*/ LDR         R3, [SP, #120]              @ 0x78 
    /*00002234*/ LDR         R1, [SP, #124]              @ 0x7c 
    /*00002238*/ STR         R2, [SP, #20] 
    /*0000223c*/ AND         R3, R3, #1 
    /*00002240*/ LDR         R2, [SP, #128]              @ 0x80 
    /*00002244*/ AND         R1, R1, #1 
    /*00002248*/ STR         R3, [SP, #24] 
    /*0000224c*/ STR         R1, [SP, #28] 
    /*00002250*/ AND         R2, R2, #1 
    /*00002254*/ LDR         R3, [SP, #132]              @ 0x84 
    /*00002258*/ LDR         R1, [SP, #136]              @ 0x88 
    /*0000225c*/ STR         R2, [SP, #32] 
    /*00002260*/ AND         R3, R3, #1 
    /*00002264*/ LDR         R2, [SP, #140]              @ 0x8c 
    /*00002268*/ AND         R1, R1, #1 
    /*0000226c*/ STR         R3, [SP, #36]               @ 0x24 
    /*00002270*/ STR         R1, [SP, #40]               @ 0x28 
    /*00002274*/ AND         R2, R2, #1 
    /*00002278*/ LDR         R3, [SP, #144]              @ 0x90 
    /*0000227c*/ LDR         R1, [SP, #148]              @ 0x94 
    /*00002280*/ STR         R2, [SP, #44]               @ 0x2c 
    /*00002284*/ AND         R3, R3, #1 
    /*00002288*/ LDR         R2, [SP, #152]              @ 0x98 
    /*0000228c*/ AND         R1, R1, #1 
    /*00002290*/ LDR         R9, [SP, #184]              @ 0xb8 
    /*00002294*/ LDR         R8, [SP, #188]              @ 0xbc 
    /*00002298*/ AND         R2, R2, #1 
    /*0000229c*/ STR         R3, [SP, #48]               @ 0x30 
    /*000022a0*/ AND         R9, R9, #1 
    /*000022a4*/ LDR         R3, [SP, #156]              @ 0x9c 
    /*000022a8*/ AND         R8, R8, #1 
    /*000022ac*/ STR         R1, [SP, #52]               @ 0x34 
    /*000022b0*/ STR         R2, [SP, #56]               @ 0x38 
    /*000022b4*/ AND         R3, R3, #1 
    /*000022b8*/ LDR         R1, [SP, #160]              @ 0xa0 
    /*000022bc*/ LDR         R2, [SP, #164]              @ 0xa4 
    /*000022c0*/ AND         R1, R1, #1 
    /*000022c4*/ STR         R3, [SP, #60]               @ 0x3c 
    /*000022c8*/ AND         R2, R2, #1 
    /*000022cc*/ LDR         R3, [SP, #168]              @ 0xa8 
    /*000022d0*/ STR         R1, [SP, #64]               @ 0x40 
    /*000022d4*/ STR         R2, [SP, #68]               @ 0x44 
    /*000022d8*/ AND         R3, R3, #1 
    /*000022dc*/ LDR         R1, [SP, #172]              @ 0xac 
    /*000022e0*/ LDR         R2, [SP, #208]              @ 0xd0 
    /*000022e4*/ AND         R1, R1, #1 
    /*000022e8*/ STR         R3, [SP, #72]               @ 0x48 
    /*000022ec*/ AND         R2, R2, #127                @ 0x7f 
    /*000022f0*/ LDR         R3, [SP, #212]              @ 0xd4 
    /*000022f4*/ STR         R1, [SP, #76]               @ 0x4c 
    /*000022f8*/ STR         R2, [SP, #84]               @ 0x54 
    /*000022fc*/ AND         R3, R3, #127                @ 0x7f 
    /*00002300*/ LDR         R1, [SP, #216]              @ 0xd8 
    /*00002304*/ LDR         R2, [SP, #220]              @ 0xdc 
    /*00002308*/ LDR         IP, [SP, #236]              @ 0xec 
    /*0000230c*/ AND         R1, R1, #127                @ 0x7f 
    /*00002310*/ AND         R2, R2, #127                @ 0x7f 
    /*00002314*/ STR         R3, [SP, #88]               @ 0x58 
    /*00002318*/ STR         R1, [SP, #92]               @ 0x5c 
    /*0000231c*/ AND         IP, IP, #127                @ 0x7f 
    /*00002320*/ LDR         R3, [SP, #224]              @ 0xe0 
    /*00002324*/ LDR         R1, [SP, #228]              @ 0xe4 
    /*00002328*/ STR         R2, [SP, #96]               @ 0x60 
    /*0000232c*/ AND         R3, R3, #127                @ 0x7f 
    /*00002330*/ LDR         R2, [SP, #232]              @ 0xe8 
    /*00002334*/ STR         R3, [SP, #100]              @ 0x64 
    /*00002338*/ AND         R3, R1, #127                @ 0x7f 
    /*0000233c*/ STR         R3, [SP, #104]              @ 0x68 
    /*00002340*/ AND         R3, R2, #127                @ 0x7f 
    /*00002344*/ STR         R3, [SP, #108]              @ 0x6c 
    VEDU_DRV_UpdateOSD_x2348:
    /*00002348*/ LDR         R2, [R4, #52]               @ 0x34 
    /*0000234c*/ MOV         R1, #0 
    /*00002350*/ LDR         R3, [R5, #848]              @ 0x350 
    /*00002354*/ MOV         R0, R1 
    /*00002358*/ LDR         LR, [R4, #48]               @ 0x30 
    /*0000235c*/ BFI         R3, R2, #0, #8 
    /*00002360*/ LDR         R2, [R4, #148]              @ 0x94 
    /*00002364*/ BFI         R3, LR, #8, #8 
    /*00002368*/ LDR         LR, [R4, #144]              @ 0x90 
    /*0000236c*/ BFI         R3, R2, #16, #8 
    /*00002370*/ LDR         R2, [R5, #852]              @ 0x354 
    /*00002374*/ BFI         R3, LR, #24, #8 
    /*00002378*/ STR         R3, [R5, #848]              @ 0x350 
    /*0000237c*/ LDR         LR, [R4, #244]              @ 0xf4 
    /*00002380*/ ADD         R3, R5, #772                @ 0x304 
    /*00002384*/ LDR         R6, [R4, #240]              @ 0xf0 
    /*00002388*/ LDR         R7, [R4, #340]              @ 0x154 
    /*0000238c*/ BFI         R2, LR, #0, #8 
    /*00002390*/ BFI         R2, R6, #8, #8 
    /*00002394*/ LDR         LR, [R4, #336]              @ 0x150 
    /*00002398*/ BFI         R2, R7, #16, #8 
    /*0000239c*/ ADD         R7, R5, #804                @ 0x324 
    /*000023a0*/ BFI         R2, LR, #24, #8 
    /*000023a4*/ LDR         LR, [R5, #856]              @ 0x358 
    /*000023a8*/ STR         R2, [R5, #852]              @ 0x354 
    /*000023ac*/ LDR         R2, [R4, #436]              @ 0x1b4 
    /*000023b0*/ BFI         LR, R2, #0, #8 
    /*000023b4*/ LDR         R2, [R4, #432]              @ 0x1b0 
    /*000023b8*/ BFI         LR, R2, #8, #8 
    /*000023bc*/ LDR         R2, [R4, #532]              @ 0x214 
    /*000023c0*/ BFI         LR, R2, #16, #8 
    /*000023c4*/ LDR         R2, [R4, #528]              @ 0x210 
    /*000023c8*/ BFI         LR, R2, #24, #8 
    /*000023cc*/ LDR         R2, [R5, #860]              @ 0x35c 
    /*000023d0*/ STR         LR, [R5, #856]              @ 0x358 
    /*000023d4*/ LDR         LR, [R4, #628]              @ 0x274 
    /*000023d8*/ BFI         R2, LR, #0, #8 
    /*000023dc*/ LDR         LR, [R4, #624]              @ 0x270 
    /*000023e0*/ BFI         R2, LR, #8, #8 
    /*000023e4*/ LDR         LR, [R4, #724]              @ 0x2d4 
    /*000023e8*/ BFI         R2, LR, #16, #8 
    /*000023ec*/ LDR         LR, [R4, #720]              @ 0x2d0 
    /*000023f0*/ BFI         R2, LR, #24, #8 
    /*000023f4*/ STR         R2, [R5, #860]              @ 0x35c 
    /*000023f8*/ LDR         LR, [R4, #44]               @ 0x2c 
    /*000023fc*/ MOV         R2, R4 
    /*00002400*/ BFI         R1, LR, #0, #8 
    /*00002404*/ LDR         LR, [R4, #140]              @ 0x8c 
    /*00002408*/ BFI         R1, LR, #8, #8 
    /*0000240c*/ LDR         LR, [R4, #236]              @ 0xec 
    /*00002410*/ BFI         R1, LR, #16, #8 
    /*00002414*/ LDR         LR, [R4, #332]              @ 0x14c 
    /*00002418*/ BFI         R1, LR, #24, #8 
    /*0000241c*/ STR         R1, [R5, #864]              @ 0x360 
    /*00002420*/ LDR         LR, [R4, #428]              @ 0x1ac 
    /*00002424*/ MOV         R1, R4 
    /*00002428*/ BFI         R0, LR, #0, #8 
    /*0000242c*/ LDR         LR, [R4, #524]              @ 0x20c 
    /*00002430*/ BFI         R0, LR, #8, #8 
    /*00002434*/ LDR         LR, [R4, #620]              @ 0x26c 
    /*00002438*/ BFI         R0, LR, #16, #8 
    /*0000243c*/ LDR         LR, [R4, #716]              @ 0x2cc 
    /*00002440*/ BFI         R0, LR, #24, #8 
    /*00002444*/ STR         R0, [R5, #868]              @ 0x364 
    VEDU_DRV_UpdateOSD_x2448:
    /*00002448*/ LDR         R6, [R1, #24] 
    /*0000244c*/ ADD         R1, R1, #96                 @ 0x60 
    /*00002450*/ LDR         R0, [R3] 
    /*00002454*/ LDR         LR, [R1, #-68]              @ 0xffffffbc 
    /*00002458*/ BFI         R0, R6, #0, #16 
    /*0000245c*/ BFI         R0, LR, #16, #16 
    /*00002460*/ STR         R0, [R3], #4 
    /*00002464*/ CMP         R7, R3 
    /*00002468*/ BNE         VEDU_DRV_UpdateOSD_x2448
    /*0000246c*/ ADD         R6, R5, #836                @ 0x344 
    VEDU_DRV_UpdateOSD_x2470:
    /*00002470*/ LDR         LR, [R2, #32] 
    /*00002474*/ ADD         R2, R2, #96                 @ 0x60 
    /*00002478*/ LDR         R0, [R2, #-60]              @ 0xffffffc4 
    /*0000247c*/ LDR         R1, [R3] 
    /*00002480*/ SUB         LR, LR, #1 
    /*00002484*/ SUB         R0, R0, #1 
    /*00002488*/ BFI         R1, LR, #0, #16 
    /*0000248c*/ BFI         R1, R0, #16, #16 
    /*00002490*/ STR         R1, [R3], #4 
    /*00002494*/ CMP         R6, R3 
    /*00002498*/ BNE         VEDU_DRV_UpdateOSD_x2470
    /*0000249c*/ LDR         R3, [R4, #56]               @ 0x38 
    /*000024a0*/ ADD         R0, R5, #904                @ 0x388 
    /*000024a4*/ MOVW        R1, #906                    @ 0x38a 
    /*000024a8*/ ADD         R2, R5, #908                @ 0x38c 
    /*000024ac*/ STR         R3, [R5, #872]              @ 0x368 
    /*000024b0*/ MOVW        R3, #910                    @ 0x38e 
    /*000024b4*/ LDR         LR, [R4, #152]              @ 0x98 
    /*000024b8*/ STR         LR, [R5, #876]              @ 0x36c 
    /*000024bc*/ LDR         LR, [R4, #248]              @ 0xf8 
    /*000024c0*/ STR         LR, [R5, #880]              @ 0x370 
    /*000024c4*/ LDR         LR, [R4, #344]              @ 0x158 
    /*000024c8*/ STR         LR, [R5, #884]              @ 0x374 
    /*000024cc*/ LDR         LR, [R4, #440]              @ 0x1b8 
    /*000024d0*/ STR         LR, [R5, #888]              @ 0x378 
    /*000024d4*/ LDR         LR, [R4, #536]              @ 0x218 
    /*000024d8*/ STR         LR, [R5, #892]              @ 0x37c 
    /*000024dc*/ LDR         LR, [R4, #632]              @ 0x278 
    /*000024e0*/ STR         LR, [R5, #896]              @ 0x380 
    /*000024e4*/ LDR         LR, [R4, #728]              @ 0x2d8 
    /*000024e8*/ STR         LR, [R5, #900]              @ 0x384 
    /*000024ec*/ LDR         LR, [R4, #64]               @ 0x40 
    /*000024f0*/ STRH        LR, [R0] 
    /*000024f4*/ ADD         R0, R5, #912                @ 0x390 
    /*000024f8*/ LDR         LR, [R4, #160]              @ 0xa0 
    /*000024fc*/ STRH        LR, [R5, R1] 
    /*00002500*/ MOVW        R1, #914                    @ 0x392 
    /*00002504*/ LDR         LR, [R4, #256]              @ 0x100 
    /*00002508*/ STRH        LR, [R2] 
    /*0000250c*/ ADD         R2, R5, #916                @ 0x394 
    /*00002510*/ LDR         LR, [R4, #352]              @ 0x160 
    /*00002514*/ STRH        LR, [R5, R3] 
    /*00002518*/ MOVW        R3, #918                    @ 0x396 
    /*0000251c*/ LDR         LR, [R4, #448]              @ 0x1c0 
    /*00002520*/ STRH        LR, [R0] 
    /*00002524*/ MOV         R0, #77                     @ 0x4d 
    /*00002528*/ LDR         LR, [R4, #544]              @ 0x220 
    /*0000252c*/ STRH        LR, [R5, R1] 
    /*00002530*/ MOV         R1, #0 
    /*00002534*/ LDR         LR, [R4, #640]              @ 0x280 
    /*00002538*/ BFI         R1, R0, #0, #10 
    /*0000253c*/ MOV         R0, #43                     @ 0x2b 
    /*00002540*/ STRH        LR, [R2] 
    /*00002544*/ LDR         R2, [R4, #736]              @ 0x2e0 
    /*00002548*/ STRH        R2, [R5, R3] 
    /*0000254c*/ LDR         R2, [R5, #836]              @ 0x344 
    /*00002550*/ LDR         R3, [R5, #768]              @ 0x300 
    /*00002554*/ LDR         LR, [R4, #40]               @ 0x28 
    /*00002558*/ BFI         R3, FP, #8, #1 
    /*0000255c*/ LDR         R7, [SP] 
    /*00002560*/ BFI         R3, SL, #9, #1 
    /*00002564*/ BFI         R2, LR, #0, #3 
    /*00002568*/ LDR         LR, [R4, #136]              @ 0x88 
    /*0000256c*/ BFI         R3, R9, #10, #1 
    /*00002570*/ BFI         R3, R8, #11, #1 
    /*00002574*/ LDR         R6, [R4, #232]              @ 0xe8 
    /*00002578*/ BFI         R2, LR, #4, #3 
    /*0000257c*/ LDR         LR, [SP, #4] 
    /*00002580*/ BFI         R3, R7, #12, #1 
    /*00002584*/ BFI         R2, R6, #8, #3 
    /*00002588*/ BFI         R3, LR, #13, #1 
    /*0000258c*/ LDR         LR, [SP, #8] 
    /*00002590*/ LDR         R7, [R4, #328]              @ 0x148 
    /*00002594*/ MOV         FP, #150                    @ 0x96 
    /*00002598*/ BFI         R3, LR, #14, #1 
    /*0000259c*/ LDR         LR, [SP, #12] 
    /*000025a0*/ LDR         R6, [SP, #20] 
    /*000025a4*/ BFI         R2, R7, #12, #3 
    /*000025a8*/ BFI         R3, LR, #15, #1 
    /*000025ac*/ LDR         LR, [SP, #16] 
    /*000025b0*/ LDR         R9, [SP, #24] 
    /*000025b4*/ BFI         R1, FP, #10, #10 
    /*000025b8*/ BFI         R3, LR, #0, #1 
    /*000025bc*/ LDR         R8, [R4, #424]              @ 0x1a8 
    /*000025c0*/ LDR         R7, [SP, #28] 
    /*000025c4*/ BFI         R3, R6, #1, #1 
    /*000025c8*/ BFI         R3, R9, #2, #1 
    /*000025cc*/ LDR         R9, [SP, #32] 
    /*000025d0*/ BFI         R3, R7, #3, #1 
    /*000025d4*/ BFI         R2, R8, #16, #3 
    /*000025d8*/ LDR         R8, [SP, #36]               @ 0x24 
    /*000025dc*/ BFI         R3, R9, #4, #1 
    /*000025e0*/ LDR         LR, [R5, #840]              @ 0x348 
    /*000025e4*/ MOV         R9, #128                    @ 0x80 
    /*000025e8*/ BFI         R3, R8, #5, #1 
    /*000025ec*/ LDR         R8, [SP, #40]               @ 0x28 
    /*000025f0*/ LDR         R6, [R5, #844]              @ 0x34c 
    /*000025f4*/ MOV         FP, #0 
    /*000025f8*/ BFI         R3, R8, #6, #1 
    /*000025fc*/ LDR         R8, [SP, #84]               @ 0x54 
    /*00002600*/ LDR         R7, [R4, #520]              @ 0x208 
    /*00002604*/ BFI         LR, R8, #0, #7 
    /*00002608*/ LDR         R8, [SP, #44]               @ 0x2c 
    /*0000260c*/ BFI         R2, R7, #20, #3 
    /*00002610*/ LDR         R7, [SP, #52]               @ 0x34 
    /*00002614*/ BFI         R3, R8, #7, #1 
    /*00002618*/ LDR         R8, [SP, #100]              @ 0x64 
    /*0000261c*/ LDR         SL, [R4, #616]              @ 0x268 
    /*00002620*/ BFI         R6, R8, #0, #7 
    /*00002624*/ LDR         R8, [SP, #48]               @ 0x30 
    /*00002628*/ BFI         R2, SL, #24, #3 
    /*0000262c*/ BFI         R3, R8, #16, #1 
    /*00002630*/ BFI         R3, R7, #17, #1 
    /*00002634*/ LDR         R7, [SP, #56]               @ 0x38 
    /*00002638*/ BFI         R3, R7, #18, #1 
    /*0000263c*/ LDR         R7, [SP, #88]               @ 0x58 
    /*00002640*/ BFI         LR, R7, #8, #7 
    /*00002644*/ LDR         R7, [SP, #60]               @ 0x3c 
    /*00002648*/ BFI         R3, R7, #19, #1 
    /*0000264c*/ LDR         R7, [SP, #104]              @ 0x68 
    /*00002650*/ BFI         R6, R7, #8, #7 
    /*00002654*/ LDR         R7, [SP, #64]               @ 0x40 
    /*00002658*/ LDR         R8, [SP, #68]               @ 0x44 
    /*0000265c*/ BFI         R3, R7, #20, #1 
    /*00002660*/ MOV         R7, #0 
    /*00002664*/ BFI         R7, R0, #0, #10 
    /*00002668*/ LDR         R0, [SP, #72]               @ 0x48 
    /*0000266c*/ BFI         R3, R8, #21, #1 
    /*00002670*/ MOV         R8, #0 
    /*00002674*/ LDR         SL, [SP, #76]               @ 0x4c 
    /*00002678*/ BFI         R3, R0, #22, #1 
    /*0000267c*/ MOV         R0, R8 
    /*00002680*/ BFC         R0, #0, #10 
    /*00002684*/ BFI         R8, R9, #0, #10 
    /*00002688*/ BFI         R0, R9, #10, #10 
    /*0000268c*/ LDR         R9, [R4, #712]              @ 0x2c8 
    /*00002690*/ BFI         R3, SL, #23, #1 
    /*00002694*/ LDR         SL, [SP, #92]               @ 0x5c 
    /*00002698*/ BFI         R2, R9, #28, #3 
    /*0000269c*/ LDR         R9, [SP, #80]               @ 0x50 
    /*000026a0*/ BFI         LR, SL, #16, #7 
    /*000026a4*/ LDR         SL, [SP, #108]              @ 0x6c 
    /*000026a8*/ BFI         R3, R9, #24, #1 
    /*000026ac*/ STR         R3, [R5, #768]              @ 0x300 
    /*000026b0*/ LDR         R3, [SP, #96]               @ 0x60 
    /*000026b4*/ BFI         R6, SL, #16, #7 
    /*000026b8*/ MOV         SL, #85                     @ 0x55 
    /*000026bc*/ BFI         R7, SL, #10, #10 
    /*000026c0*/ MOV         SL, #107                    @ 0x6b 
    /*000026c4*/ BFI         R8, SL, #10, #10 
    /*000026c8*/ MOV         SL, FP 
    /*000026cc*/ BFC         FP, #0, #10 
    /*000026d0*/ BFC         SL, #0, #10 
    /*000026d4*/ STR         R2, [R5, #836]              @ 0x344 
    /*000026d8*/ BFI         LR, R3, #24, #7 
    /*000026dc*/ BFI         R6, IP, #24, #7 
    /*000026e0*/ STR         LR, [R5, #840]              @ 0x348 
    /*000026e4*/ MOV         R2, #128                    @ 0x80 
    /*000026e8*/ STR         R6, [R5, #844]              @ 0x34c 
    /*000026ec*/ BFI         R7, R2, #20, #10 
    /*000026f0*/ BFI         R0, R2, #20, #10 
    /*000026f4*/ STR         R7, [R5, #652]              @ 0x28c 
    /*000026f8*/ STR         R0, [R5, #660]              @ 0x294 
    /*000026fc*/ MOV         R3, #29 
    /*00002700*/ MOV         R2, #235                    @ 0xeb 
    /*00002704*/ BFI         R1, R3, #20, #10 
    /*00002708*/ BFI         FP, R2, #16, #10 
    /*0000270c*/ STR         R1, [R5, #648]              @ 0x288 
    /*00002710*/ STR         FP, [R5, #664]              @ 0x298 
    /*00002714*/ MOV         R3, #21 
    /*00002718*/ BFI         R8, R3, #20, #10 
    /*0000271c*/ MOV         R3, #255                    @ 0xff 
    /*00002720*/ STR         R8, [R5, #656]              @ 0x290 
    /*00002724*/ BFI         SL, R3, #16, #10 
    /*00002728*/ MOV         R3, #8 
    /*0000272c*/ STR         SL, [R5, #668]              @ 0x29c 
    /*00002730*/ STR         R3, [R5, #672]              @ 0x2a0 
    /*00002734*/ MOV         R2, #0 
    /*00002738*/ LDRH        IP, [R4, #104]              @ 0x68 
    /*0000273c*/ MOV         R3, R2 
    /*00002740*/ LDRH        R1, [R4, #106]              @ 0x6a 
    /*00002744*/ ADD         R0, R4, #296                @ 0x128 
    /*00002748*/ BFI         R2, IP, #0, #15 
    /*0000274c*/ MOVW        R6, #298                    @ 0x12a 
    /*00002750*/ BFI         R2, R1, #16, #15 
    /*00002754*/ STR         R2, [R5, #920]              @ 0x398 
    /*00002758*/ LDRH        LR, [R4, #200]              @ 0xc8 
    /*0000275c*/ MOV         R2, R3 
    /*00002760*/ LDRH        IP, [R4, #202]              @ 0xca 
    /*00002764*/ ADD         R1, R4, #392                @ 0x188 
    /*00002768*/ BFI         R3, LR, #0, #15 
    /*0000276c*/ MOVW        LR, #394                    @ 0x18a 
    /*00002770*/ BFI         R3, IP, #16, #15 
    /*00002774*/ STR         R3, [R5, #924]              @ 0x39c 
    /*00002778*/ LDRH        R0, [R0] 
    /*0000277c*/ MOV         R3, R2 
    /*00002780*/ LDRH        R6, [R4, R6] 
    /*00002784*/ ADD         IP, R4, #488                @ 0x1e8 
    /*00002788*/ BFI         R2, R0, #0, #15 
    /*0000278c*/ MOVW        R0, #490                    @ 0x1ea 
    /*00002790*/ BFI         R2, R6, #16, #15 
    /*00002794*/ STR         R2, [R5, #928]              @ 0x3a0 
    /*00002798*/ LDRH        R6, [R1] 
    /*0000279c*/ MOV         R2, R3 
    /*000027a0*/ LDRH        LR, [R4, LR] 
    /*000027a4*/ ADD         R1, R4, #584                @ 0x248 
    /*000027a8*/ BFI         R3, R6, #0, #15 
    /*000027ac*/ MOVW        R6, #586                    @ 0x24a 
    /*000027b0*/ BFI         R3, LR, #16, #15 
    /*000027b4*/ STR         R3, [R5, #932]              @ 0x3a4 
    /*000027b8*/ LDRH        IP, [IP] 
    /*000027bc*/ MOV         R3, R2 
    /*000027c0*/ LDRH        R0, [R4, R0] 
    /*000027c4*/ ADD         LR, R4, #680                @ 0x2a8 
    /*000027c8*/ BFI         R2, IP, #0, #15 
    /*000027cc*/ MOVW        IP, #682                    @ 0x2aa 
    /*000027d0*/ BFI         R2, R0, #16, #15 
    /*000027d4*/ STR         R2, [R5, #936]              @ 0x3a8 
    /*000027d8*/ LDRH        R0, [R1] 
    /*000027dc*/ MOV         R2, R3 
    /*000027e0*/ LDRH        R6, [R4, R6] 
    /*000027e4*/ ADD         R1, R4, #776                @ 0x308 
    /*000027e8*/ BFI         R3, R0, #0, #15 
    /*000027ec*/ MOVW        R0, #778                    @ 0x30a 
    /*000027f0*/ BFI         R3, R6, #16, #15 
    /*000027f4*/ STR         R3, [R5, #940]              @ 0x3ac 
    /*000027f8*/ LDRH        LR, [LR] 
    /*000027fc*/ MOV         R3, R2 
    /*00002800*/ LDRH        IP, [R4, IP] 
    /*00002804*/ BFI         R2, LR, #0, #15 
    /*00002808*/ BFI         R2, IP, #16, #15 
    /*0000280c*/ STR         R2, [R5, #944]              @ 0x3b0 
    /*00002810*/ LDRH        R1, [R1] 
    /*00002814*/ LDRH        R2, [R4, R0] 
    /*00002818*/ BFI         R3, R1, #0, #15 
    /*0000281c*/ BFI         R3, R2, #16, #15 
    /*00002820*/ STR         R3, [R5, #948]              @ 0x3b4 
    /*00002824*/ ADD         SP, SP, #244                @ 0xf4 
    /*00002828*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    VEDU_DRV_UpdateOSD_x282c:
    /*0000282c*/ LDR         R3, [R5, #768]              @ 0x300 
    /*00002830*/ BFC         R3, #24, #1 
    /*00002834*/ BFC         R3, #0, #1 
    /*00002838*/ BFC         R3, #1, #1 
    /*0000283c*/ BFC         R3, #2, #1 
    /*00002840*/ BFC         R3, #3, #1 
    /*00002844*/ BFC         R3, #4, #1 
    /*00002848*/ BFC         R3, #5, #1 
    /*0000284c*/ BFC         R3, #6, #1 
    /*00002850*/ BFC         R3, #7, #1 
    /*00002854*/ STR         R3, [R5, #768]              @ 0x300 
    /*00002858*/ ADD         SP, SP, #244                @ 0xf4 
    /*0000285c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    VEDU_DRV_UpdateOSD_x2860:
    /*00002860*/ LDRB        R3, [R5, #575]              @ 0x23f 
    /*00002864*/ ORR         R3, R3, #192                @ 0xc0 
    /*00002868*/ STRB        R3, [R5, #575]              @ 0x23f 
    /*0000286c*/ B           VEDU_DRV_UpdateOSD_x2180
    VEDU_DRV_UpdateOSD_x2870:
    /*00002870*/ LDRB        R3, [R5, #575]              @ 0x23f 
    /*00002874*/ MOV         R2, #1 
    /*00002878*/ BFI         R3, R2, #6, #2 
    /*0000287c*/ STRB        R3, [R5, #575]              @ 0x23f 
    /*00002880*/ B           VEDU_DRV_UpdateOSD_x2180
    VEDU_DRV_UpdateOSD_x2884:
    /*00002884*/ LDRB        R3, [R5, #575]              @ 0x23f 
    /*00002888*/ MOV         R2, #2 
    /*0000288c*/ BFI         R3, R2, #6, #2 
    /*00002890*/ STRB        R3, [R5, #575]              @ 0x23f 
    /*00002894*/ B           VEDU_DRV_UpdateOSD_x2180
    VEDU_DRV_UpdateOSD_x2898:
    /*00002898*/ STR         IP, [SP, #108]              @ 0x6c 
    /*0000289c*/ MOV         R8, IP 
    /*000028a0*/ STR         IP, [SP, #104]              @ 0x68 
    /*000028a4*/ MOV         R9, IP 
    /*000028a8*/ STR         IP, [SP, #100]              @ 0x64 
    /*000028ac*/ MOV         SL, IP 
    /*000028b0*/ STR         IP, [SP, #96]               @ 0x60 
    /*000028b4*/ MOV         FP, IP 
    /*000028b8*/ STR         IP, [SP, #92]               @ 0x5c 
    /*000028bc*/ STR         IP, [SP, #88]               @ 0x58 
    /*000028c0*/ STR         IP, [SP, #84]               @ 0x54 
    /*000028c4*/ STR         IP, [SP, #80]               @ 0x50 
    /*000028c8*/ STR         IP, [SP, #76]               @ 0x4c 
    /*000028cc*/ STR         IP, [SP, #72]               @ 0x48 
    /*000028d0*/ STR         IP, [SP, #68]               @ 0x44 
    /*000028d4*/ STR         IP, [SP, #64]               @ 0x40 
    /*000028d8*/ STR         IP, [SP, #60]               @ 0x3c 
    /*000028dc*/ STR         IP, [SP, #56]               @ 0x38 
    /*000028e0*/ STR         IP, [SP, #52]               @ 0x34 
    /*000028e4*/ STR         IP, [SP, #48]               @ 0x30 
    /*000028e8*/ STR         IP, [SP, #44]               @ 0x2c 
    /*000028ec*/ STR         IP, [SP, #40]               @ 0x28 
    /*000028f0*/ STR         IP, [SP, #36]               @ 0x24 
    /*000028f4*/ STR         IP, [SP, #32] 
    /*000028f8*/ STR         IP, [SP, #28] 
    /*000028fc*/ STR         IP, [SP, #24] 
    /*00002900*/ STR         IP, [SP, #20] 
    /*00002904*/ STR         IP, [SP, #16] 
    /*00002908*/ STR         IP, [SP, #12] 
    /*0000290c*/ STR         IP, [SP, #8] 
    /*00002910*/ STR         IP, [SP, #4] 
    /*00002914*/ STR         IP, [SP] 
    /*00002918*/ B           VEDU_DRV_UpdateOSD_x2348
    VEDU_DRV_UpdateOSD_x291c:
    /*0000291c*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x734] 
    /*00002920*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002924*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x734] 
    /*00002928*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*0000292c*/ MOVW        R2, #337                    @ 0x151 
    /*00002930*/ LDR         R1, [PC, #36]               @ 0x000000000000295c 
    /*00002934*/ BL          0x0000000000002934          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002938*/ B           VEDU_DRV_UpdateOSD_x2114
    VEDU_DRV_UpdateOSD_x293c:
    /*0000293c*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x708] 
    /*00002940*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002944*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x708] 
    /*00002948*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*0000294c*/ MOVW        R2, #338                    @ 0x152 
    /*00002950*/ LDR         R1, [PC, #4]                @ 0x000000000000295c 
    /*00002954*/ BL          0x0000000000002954          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002958*/ B           VEDU_DRV_UpdateOSD_x211c
    /*0000295c*/ ANDEQ       R0, R0, R8, ASR R2 
FUNC_END VEDU_DRV_UpdateOSD


FUNC_BEGIN VEDU_DRV_UpdateSkip
    /*00002960*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00002964*/ SUBS        R4, R0, #0 
    /*00002968*/ MOV         R5, R1 
    /*0000296c*/ BEQ         VEDU_DRV_UpdateSkip_x2a64
    VEDU_DRV_UpdateSkip_x2970:
    /*00002970*/ LDRB        R3, [R4, #1665]             @ 0x681 
    /*00002974*/ AND         R1, R5, #1 
    /*00002978*/ LDRB        R7, [R4, #1666]             @ 0x682 
    /*0000297c*/ MOV         R0, #0 
    /*00002980*/ AND         R3, R3, #128                @ 0x80 
    /*00002984*/ LDRB        R5, [R4, #1667]             @ 0x683 
    /*00002988*/ LDRB        IP, [R4, #1668]             @ 0x684 
    /*0000298c*/ BFC         R3, #7, #1 
    /*00002990*/ LDRB        R2, [R4, #1670]             @ 0x686 
    /*00002994*/ BFI         R5, R1, #0, #1 
    /*00002998*/ STRB        R3, [R4, #1665]             @ 0x681 
    /*0000299c*/ BFI         R7, R1, #0, #1 
    /*000029a0*/ LDRB        R3, [R4, #1671]             @ 0x687 
    /*000029a4*/ BFC         IP, #0, #7 
    /*000029a8*/ LDRB        LR, [R4, #1669]             @ 0x685 
    /*000029ac*/ BFC         R2, #0, #7 
    /*000029b0*/ BFC         R3, #0, #7 
    /*000029b4*/ STRB        R1, [R4, #1664]             @ 0x680 
    /*000029b8*/ STRB        IP, [R4, #1668]             @ 0x684 
    /*000029bc*/ MOVW        R6, #426                    @ 0x1aa 
    /*000029c0*/ LDRB        IP, [R4, #1672]             @ 0x688 
    /*000029c4*/ BFC         LR, #0, #7 
    /*000029c8*/ LDRB        R1, [R4, #1673]             @ 0x689 
    /*000029cc*/ STRB        R2, [R4, #1670]             @ 0x686 
    /*000029d0*/ BFC         IP, #0, #7 
    /*000029d4*/ LDRB        R2, [R4, #1674]             @ 0x68a 
    /*000029d8*/ BFC         R1, #0, #7 
    /*000029dc*/ STRB        R3, [R4, #1671]             @ 0x687 
    /*000029e0*/ LDRB        R3, [R4, #1675]             @ 0x68b 
    /*000029e4*/ BFC         R2, #0, #7 
    /*000029e8*/ STRB        R5, [R4, #1667]             @ 0x683 
    /*000029ec*/ MOVW        R5, #418                    @ 0x1a2 
    /*000029f0*/ BFC         R3, #0, #7 
    /*000029f4*/ STRB        R7, [R4, #1666]             @ 0x682 
    /*000029f8*/ STRB        LR, [R4, #1669]             @ 0x685 
    /*000029fc*/ STRB        IP, [R4, #1672]             @ 0x688 
    /*00002a00*/ STRB        R1, [R4, #1673]             @ 0x689 
    /*00002a04*/ STRB        R2, [R4, #1674]             @ 0x68a 
    /*00002a08*/ STRB        R3, [R4, #1675]             @ 0x68b 
    VEDU_DRV_UpdateSkip_x2a0c:
    /*00002a0c*/ ADD         R2, R0, R6 
    /*00002a10*/ ADD         R3, R0, R5 
    /*00002a14*/ ADD         R0, R0, #1 
    /*00002a18*/ ADD         R2, R4, R2, LSL #2 
    /*00002a1c*/ ADD         R3, R4, R3, LSL #2 
    /*00002a20*/ ADD         IP, R2, #4 
    /*00002a24*/ ADD         R1, R3, #4 
    /*00002a28*/ LDRH        LR, [R2, #4] 
    /*00002a2c*/ CMP         R0, #8 
    /*00002a30*/ BFC         LR, #0, #12 
    /*00002a34*/ STRH        LR, [R2, #4] 
    /*00002a38*/ LDRH        R2, [R2, #6] 
    /*00002a3c*/ BFC         R2, #0, #12 
    /*00002a40*/ STRH        R2, [IP, #2] 
    /*00002a44*/ LDRH        R2, [R3, #4] 
    /*00002a48*/ BFC         R2, #0, #13 
    /*00002a4c*/ STRH        R2, [R3, #4] 
    /*00002a50*/ LDRH        R3, [R3, #6] 
    /*00002a54*/ BFC         R3, #0, #13 
    /*00002a58*/ STRH        R3, [R1, #2] 
    /*00002a5c*/ BNE         VEDU_DRV_UpdateSkip_x2a0c
    /*00002a60*/ POP         {R4, R5, R6, R7, R8, PC} 
    VEDU_DRV_UpdateSkip_x2a64:
    /*00002a64*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x708] 
    /*00002a68*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002a6c*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x708] 
    /*00002a70*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00002a74*/ MOVW        R2, #698                    @ 0x2ba 
    /*00002a78*/ LDR         R1, [PC, #4]                @ 0x0000000000002a84 
    /*00002a7c*/ BL          0x0000000000002A7C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002a80*/ B           VEDU_DRV_UpdateSkip_x2970
    /*00002a84*/ ANDEQ       R0, R0, IP, ROR #4 
FUNC_END VEDU_DRV_UpdateSkip


FUNC_BEGIN VEDU_DRV_UpdateLowDelay
    /*00002a88*/ PUSH        {R4, R5, R6, LR} 
    /*00002a8c*/ SUBS        R4, R0, #0 
    /*00002a90*/ MOV         R5, R1 
    /*00002a94*/ BEQ         VEDU_DRV_UpdateLowDelay_x2afc
    VEDU_DRV_UpdateLowDelay_x2a98:
    /*00002a98*/ CMP         R5, #0 
    /*00002a9c*/ BEQ         VEDU_DRV_UpdateLowDelay_x2b1c
    VEDU_DRV_UpdateLowDelay_x2aa0:
    /*00002aa0*/ LDR         R3, [R5] 
    /*00002aa4*/ ADD         R2, R4, #572                @ 0x23c 
    /*00002aa8*/ CMP         R3, #1 
    /*00002aac*/ BEQ         VEDU_DRV_UpdateLowDelay_x2ad4
    /*00002ab0*/ LDRH        R3, [R2, #2] 
    /*00002ab4*/ MOV         R1, #0 
    /*00002ab8*/ BFC         R3, #4, #8 
    /*00002abc*/ STRH        R3, [R2, #2] 
    /*00002ac0*/ STR         R1, [R4, #3072]             @ 0xc00 
    /*00002ac4*/ LSR         R3, R3, #8 
    /*00002ac8*/ BFC         R3, #4, #1 
    /*00002acc*/ STRB        R3, [R4, #575]              @ 0x23f 
    /*00002ad0*/ POP         {R4, R5, R6, PC} 
    VEDU_DRV_UpdateLowDelay_x2ad4:
    /*00002ad4*/ LDRH        R3, [R2, #2] 
    /*00002ad8*/ MOV         R1, #50                     @ 0x32 
    /*00002adc*/ BFI         R3, R1, #4, #8 
    /*00002ae0*/ STRH        R3, [R2, #2] 
    /*00002ae4*/ UBFX        R3, R3, #8, #8 
    /*00002ae8*/ ORR         R3, R3, #16 
    /*00002aec*/ STRB        R3, [R4, #575]              @ 0x23f 
    /*00002af0*/ LDR         R3, [R5, #8] 
    /*00002af4*/ STR         R3, [R4, #3072]             @ 0xc00 
    /*00002af8*/ POP         {R4, R5, R6, PC} 
    VEDU_DRV_UpdateLowDelay_x2afc:
    /*00002afc*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x708] 
    /*00002b00*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002b04*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x708] 
    /*00002b08*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00002b0c*/ MOV         R2, #744                    @ 0x2e8 
    /*00002b10*/ LDR         R1, [PC, #36]               @ 0x0000000000002b3c 
    /*00002b14*/ BL          0x0000000000002B14          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002b18*/ B           VEDU_DRV_UpdateLowDelay_x2a98
    VEDU_DRV_UpdateLowDelay_x2b1c:
    /*00002b1c*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x748] 
    /*00002b20*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002b24*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x748] 
    /*00002b28*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00002b2c*/ MOVW        R2, #747                    @ 0x2eb 
    /*00002b30*/ LDR         R1, [PC, #4]                @ 0x0000000000002b3c 
    /*00002b34*/ BL          0x0000000000002B34          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002b38*/ B           VEDU_DRV_UpdateLowDelay_x2aa0
    /*00002b3c*/ ANDEQ       R0, R0, R0, LSL #5 
FUNC_END VEDU_DRV_UpdateLowDelay


FUNC_BEGIN VEDU_DRV_UpdateCompress
    /*00002b40*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00002b44*/ SUBS        R5, R0, #0 
    /*00002b48*/ SUB         SP, SP, #40                 @ 0x28 
    /*00002b4c*/ MOV         R4, R1 
    /*00002b50*/ MOV         R7, R2 
    /*00002b54*/ MOV         R8, R3 
    /*00002b58*/ BEQ         VEDU_DRV_UpdateCompress_x2c4c
    VEDU_DRV_UpdateCompress_x2b5c:
    /*00002b5c*/ CMP         R4, #0 
    /*00002b60*/ BEQ         VEDU_DRV_UpdateCompress_x2c6c
    VEDU_DRV_UpdateCompress_x2b64:
    /*00002b64*/ CMP         R7, #0 
    /*00002b68*/ BEQ         VEDU_DRV_UpdateCompress_x2c8c
    VEDU_DRV_UpdateCompress_x2b6c:
    /*00002b6c*/ CMP         R8, #0 
    /*00002b70*/ BEQ         VEDU_DRV_UpdateCompress_x2cac
    VEDU_DRV_UpdateCompress_x2b74:
    /*00002b74*/ LDR         R6, [R4, #20] 
    /*00002b78*/ CMP         R6, #1 
    /*00002b7c*/ MOVNE       R3, #0 
    /*00002b80*/ STRNE       R3, [R5, #1204]             @ 0x4b4 
    /*00002b84*/ BEQ         VEDU_DRV_UpdateCompress_x2b90
    /*00002b88*/ ADD         SP, SP, #40                 @ 0x28 
    /*00002b8c*/ POP         {R4, R5, R6, R7, R8, PC} 
    VEDU_DRV_UpdateCompress_x2b90:
    /*00002b90*/ LDR         R2, [R4, #24] 
    /*00002b94*/ MOV         R0, #2 
    /*00002b98*/ LDR         LR, [R4, #332]              @ 0x14c 
    /*00002b9c*/ MOV         R3, #0 
    /*00002ba0*/ SUB         R2, R2, #1 
    /*00002ba4*/ LDR         IP, [R4, #12] 
    /*00002ba8*/ CMP         R2, #1 
    /*00002bac*/ LDR         R1, [R4] 
    /*00002bb0*/ LDR         R2, [R4, #4] 
    /*00002bb4*/ STR         R3, [SP, #28] 
    /*00002bb8*/ STR         R3, [SP, #32] 
    /*00002bbc*/ STR         R3, [SP, #36]               @ 0x24 
    /*00002bc0*/ MOVLS       R3, R6 
    /*00002bc4*/ STR         LR, [SP] 
    /*00002bc8*/ STR         R3, [SP, #12] 
    /*00002bcc*/ STR         IP, [SP, #4] 
    /*00002bd0*/ STR         R1, [SP, #20] 
    /*00002bd4*/ STR         R2, [SP, #24] 
    /*00002bd8*/ STR         R6, [SP, #8] 
    /*00002bdc*/ BL          0x0000000000002BDC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002be0*/ MOV         R2, R8 
    /*00002be4*/ MOV         R1, R7 
    /*00002be8*/ LDR         R3, [R0, #128]              @ 0x80 
    /*00002bec*/ MOV         R0, SP 
    /*00002bf0*/ BLX         R3 
    /*00002bf4*/ LDR         R3, [R4, #32] 
    /*00002bf8*/ CMP         R3, #16 
    /*00002bfc*/ BEQ         VEDU_DRV_UpdateCompress_x2c18
    /*00002c00*/ CMP         R3, #32 
    /*00002c04*/ BEQ         VEDU_DRV_UpdateCompress_x2c3c
    /*00002c08*/ CMP         R3, #48                     @ 0x30 
    /*00002c0c*/ BEQ         VEDU_DRV_UpdateCompress_x2ccc
    /*00002c10*/ CMP         R3, #64                     @ 0x40 
    /*00002c14*/ BEQ         VEDU_DRV_UpdateCompress_x2ce0
    VEDU_DRV_UpdateCompress_x2c18:
    /*00002c18*/ LDRB        R3, [R5, #1206]             @ 0x4b6 
    /*00002c1c*/ BFC         R3, #0, #2 
    /*00002c20*/ STRB        R3, [R5, #1206]             @ 0x4b6 
    VEDU_DRV_UpdateCompress_x2c24:
    /*00002c24*/ LDR         R2, [R7, #8] 
    /*00002c28*/ LDRB        R3, [R5, #1204]             @ 0x4b4 
    /*00002c2c*/ BFI         R3, R2, #1, #1 
    /*00002c30*/ STRB        R3, [R5, #1204]             @ 0x4b4 
    /*00002c34*/ ADD         SP, SP, #40                 @ 0x28 
    /*00002c38*/ POP         {R4, R5, R6, R7, R8, PC} 
    VEDU_DRV_UpdateCompress_x2c3c:
    /*00002c3c*/ LDRB        R3, [R5, #1206]             @ 0x4b6 
    /*00002c40*/ BFI         R3, R6, #0, #2 
    /*00002c44*/ STRB        R3, [R5, #1206]             @ 0x4b6 
    /*00002c48*/ B           VEDU_DRV_UpdateCompress_x2c24
    VEDU_DRV_UpdateCompress_x2c4c:
    /*00002c4c*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x708] 
    /*00002c50*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002c54*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x708] 
    /*00002c58*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00002c5c*/ MOVW        R2, #775                    @ 0x307 
    /*00002c60*/ LDR         R1, [PC, #136]              @ 0x0000000000002cf0 
    /*00002c64*/ BL          0x0000000000002C64          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002c68*/ B           VEDU_DRV_UpdateCompress_x2b5c
    VEDU_DRV_UpdateCompress_x2c6c:
    /*00002c6c*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x760] 
    /*00002c70*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002c74*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x760] 
    /*00002c78*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00002c7c*/ MOV         R2, #776                    @ 0x308 
    /*00002c80*/ LDR         R1, [PC, #104]              @ 0x0000000000002cf0 
    /*00002c84*/ BL          0x0000000000002C84          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002c88*/ B           VEDU_DRV_UpdateCompress_x2b64
    VEDU_DRV_UpdateCompress_x2c8c:
    /*00002c8c*/ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x778] 
    /*00002c90*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002c94*/ MOVT        R3, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x778] 
    /*00002c98*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00002c9c*/ MOVW        R2, #777                    @ 0x309 
    /*00002ca0*/ LDR         R1, [PC, #72]               @ 0x0000000000002cf0 
    /*00002ca4*/ BL          0x0000000000002CA4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002ca8*/ B           VEDU_DRV_UpdateCompress_x2b6c
    VEDU_DRV_UpdateCompress_x2cac:
    /*00002cac*/ MOVW        R3, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x790] 
    /*00002cb0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002cb4*/ MOVT        R3, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x790] 
    /*00002cb8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00002cbc*/ MOVW        R2, #778                    @ 0x30a 
    /*00002cc0*/ LDR         R1, [PC, #40]               @ 0x0000000000002cf0 
    /*00002cc4*/ BL          0x0000000000002CC4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002cc8*/ B           VEDU_DRV_UpdateCompress_x2b74
    VEDU_DRV_UpdateCompress_x2ccc:
    /*00002ccc*/ LDRB        R3, [R5, #1206]             @ 0x4b6 
    /*00002cd0*/ MOV         R2, #2 
    /*00002cd4*/ BFI         R3, R2, #0, #2 
    /*00002cd8*/ STRB        R3, [R5, #1206]             @ 0x4b6 
    /*00002cdc*/ B           VEDU_DRV_UpdateCompress_x2c24
    VEDU_DRV_UpdateCompress_x2ce0:
    /*00002ce0*/ LDRB        R3, [R5, #1206]             @ 0x4b6 
    /*00002ce4*/ ORR         R3, R3, #3 
    /*00002ce8*/ STRB        R3, [R5, #1206]             @ 0x4b6 
    /*00002cec*/ B           VEDU_DRV_UpdateCompress_x2c24
    /*00002cf0*/ MULEQ       R0, R8, R2 
FUNC_END VEDU_DRV_UpdateCompress


FUNC_BEGIN VEDU_DRV_UpdateSkipBias
    /*00002cf4*/ PUSH        {R4, R5, R6, LR} 
    /*00002cf8*/ SUBS        R4, R0, #0 
    /*00002cfc*/ MOV         R5, R1 
    /*00002d00*/ BEQ         VEDU_DRV_UpdateSkipBias_x2e0c
    VEDU_DRV_UpdateSkipBias_x2d04:
    /*00002d04*/ CMP         R5, #0 
    /*00002d08*/ BEQ         VEDU_DRV_UpdateSkipBias_x2dec
    VEDU_DRV_UpdateSkipBias_x2d0c:
    /*00002d0c*/ LDRB        R3, [R4, #1360]             @ 0x550 
    /*00002d10*/ MOVW        R1, #1836                   @ 0x72c 
    /*00002d14*/ LDRB        R2, [R4, #1829]             @ 0x725 
    /*00002d18*/ ORR         R3, R3, #15 
    /*00002d1c*/ LDRB        IP, [R4, #1832]             @ 0x728 
    /*00002d20*/ BFC         R3, #6, #1 
    /*00002d24*/ ORR         R2, R2, #16 
    /*00002d28*/ STRB        R3, [R4, #1360]             @ 0x550 
    /*00002d2c*/ STRB        R2, [R4, #1829]             @ 0x725 
    /*00002d30*/ LDR         R2, [R5, #8] 
    /*00002d34*/ LDRB        R3, [R4, #1829]             @ 0x725 
    /*00002d38*/ LDRB        R0, [R4, #1833]             @ 0x729 
    /*00002d3c*/ STRB        R2, [R4, #1828]             @ 0x724 
    /*00002d40*/ LDR         LR, [R5, #4] 
    /*00002d44*/ LDRB        R2, [R4, #1834]             @ 0x72a 
    /*00002d48*/ BFI         R3, LR, #0, #4 
    /*00002d4c*/ STRB        R3, [R4, #1829]             @ 0x725 
    /*00002d50*/ LDR         LR, [R5, #12] 
    /*00002d54*/ LDRB        R3, [R4, #1835]             @ 0x72b 
    /*00002d58*/ BFI         IP, LR, #0, #4 
    /*00002d5c*/ STRB        IP, [R4, #1832]             @ 0x728 
    /*00002d60*/ LDR         LR, [R5, #12] 
    /*00002d64*/ UXTB        IP, IP 
    /*00002d68*/ BFI         IP, LR, #4, #4 
    /*00002d6c*/ STRB        IP, [R4, #1832]             @ 0x728 
    /*00002d70*/ LDR         IP, [R5, #12] 
    /*00002d74*/ BFI         R0, IP, #0, #4 
    /*00002d78*/ STRB        R0, [R4, #1833]             @ 0x729 
    /*00002d7c*/ LDR         IP, [R5, #12] 
    /*00002d80*/ UXTB        R0, R0 
    /*00002d84*/ BFI         R0, IP, #4, #4 
    /*00002d88*/ STRB        R0, [R4, #1833]             @ 0x729 
    /*00002d8c*/ LDR         R0, [R5, #16] 
    /*00002d90*/ BFI         R2, R0, #0, #4 
    /*00002d94*/ STRB        R2, [R4, #1834]             @ 0x72a 
    /*00002d98*/ LDR         R0, [R5, #16] 
    /*00002d9c*/ UXTB        R2, R2 
    /*00002da0*/ BFI         R2, R0, #4, #4 
    /*00002da4*/ STRB        R2, [R4, #1834]             @ 0x72a 
    /*00002da8*/ LDR         R2, [R5, #16] 
    /*00002dac*/ BFI         R3, R2, #0, #4 
    /*00002db0*/ STRB        R3, [R4, #1835]             @ 0x72b 
    /*00002db4*/ LDR         R2, [R5, #16] 
    /*00002db8*/ UXTB        R3, R3 
    /*00002dbc*/ BFI         R3, R2, #4, #4 
    /*00002dc0*/ STRB        R3, [R4, #1835]             @ 0x72b 
    /*00002dc4*/ LDRB        R3, [R4, #1836]             @ 0x72c 
    /*00002dc8*/ BFC         R3, #0, #1 
    /*00002dcc*/ STRB        R3, [R4, #1836]             @ 0x72c 
    /*00002dd0*/ LDRH        R3, [R4, R1] 
    /*00002dd4*/ BFC         R3, #4, #5 
    /*00002dd8*/ STRH        R3, [R4, R1] 
    /*00002ddc*/ LDR         R3, [R4, #1836]             @ 0x72c 
    /*00002de0*/ BFC         R3, #12, #5 
    /*00002de4*/ STR         R3, [R4, #1836]             @ 0x72c 
    /*00002de8*/ POP         {R4, R5, R6, PC} 
    VEDU_DRV_UpdateSkipBias_x2dec:
    /*00002dec*/ MOVW        R3, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7a8] 
    /*00002df0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002df4*/ MOVT        R3, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7a8] 
    /*00002df8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00002dfc*/ MOVW        R2, #851                    @ 0x353 
    /*00002e00*/ LDR         R1, [PC, #36]               @ 0x0000000000002e2c 
    /*00002e04*/ BL          0x0000000000002E04          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002e08*/ B           VEDU_DRV_UpdateSkipBias_x2d0c
    VEDU_DRV_UpdateSkipBias_x2e0c:
    /*00002e0c*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x708] 
    /*00002e10*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002e14*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x708] 
    /*00002e18*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00002e1c*/ MOVW        R2, #850                    @ 0x352 
    /*00002e20*/ LDR         R1, [PC, #4]                @ 0x0000000000002e2c 
    /*00002e24*/ BL          0x0000000000002E24          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002e28*/ B           VEDU_DRV_UpdateSkipBias_x2d04
    /*00002e2c*/ @           <UNDEFINED> instruction: 0x000002b0 
FUNC_END VEDU_DRV_UpdateSkipBias


FUNC_BEGIN VEDU_DRV_UpdateAwpress
    /*00002e30*/ PUSH        {R4, LR} 
    /*00002e34*/ SUBS        R4, R0, #0 
    /*00002e38*/ BEQ         VEDU_DRV_UpdateAwpress_x2eb0
    VEDU_DRV_UpdateAwpress_x2e3c:
    /*00002e3c*/ MOV         R3, #0 
    /*00002e40*/ STR         R3, [R4, #3328]             @ 0xd00 
    /*00002e44*/ STR         R3, [R4, #3332]             @ 0xd04 
    /*00002e48*/ STR         R3, [R4, #3336]             @ 0xd08 
    /*00002e4c*/ STR         R3, [R4, #3340]             @ 0xd0c 
    /*00002e50*/ STR         R3, [R4, #3344]             @ 0xd10 
    /*00002e54*/ STR         R3, [R4, #3348]             @ 0xd14 
    /*00002e58*/ STR         R3, [R4, #3352]             @ 0xd18 
    /*00002e5c*/ STR         R3, [R4, #3356]             @ 0xd1c 
    /*00002e60*/ STR         R3, [R4, #3360]             @ 0xd20 
    /*00002e64*/ STR         R3, [R4, #3364]             @ 0xd24 
    /*00002e68*/ STR         R3, [R4, #3368]             @ 0xd28 
    /*00002e6c*/ STR         R3, [R4, #3372]             @ 0xd2c 
    /*00002e70*/ STR         R3, [R4, #3376]             @ 0xd30 
    /*00002e74*/ STR         R3, [R4, #3380]             @ 0xd34 
    /*00002e78*/ STR         R3, [R4, #3384]             @ 0xd38 
    /*00002e7c*/ STR         R3, [R4, #3388]             @ 0xd3c 
    /*00002e80*/ STR         R3, [R4, #3392]             @ 0xd40 
    /*00002e84*/ STR         R3, [R4, #3396]             @ 0xd44 
    /*00002e88*/ STR         R3, [R4, #3400]             @ 0xd48 
    /*00002e8c*/ STR         R3, [R4, #3404]             @ 0xd4c 
    /*00002e90*/ STR         R3, [R4, #3408]             @ 0xd50 
    /*00002e94*/ STR         R3, [R4, #3412]             @ 0xd54 
    /*00002e98*/ STR         R3, [R4, #3416]             @ 0xd58 
    /*00002e9c*/ STR         R3, [R4, #3420]             @ 0xd5c 
    /*00002ea0*/ STR         R3, [R4, #3424]             @ 0xd60 
    /*00002ea4*/ STR         R3, [R4, #3428]             @ 0xd64 
    /*00002ea8*/ STR         R3, [R4, #3432]             @ 0xd68 
    /*00002eac*/ POP         {R4, PC} 
    VEDU_DRV_UpdateAwpress_x2eb0:
    /*00002eb0*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x708] 
    /*00002eb4*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002eb8*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x708] 
    /*00002ebc*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00002ec0*/ MOVW        R2, #886                    @ 0x376 
    /*00002ec4*/ LDR         R1, [PC, #4]                @ 0x0000000000002ed0 
    /*00002ec8*/ BL          0x0000000000002EC8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002ecc*/ B           VEDU_DRV_UpdateAwpress_x2e3c
    /*00002ed0*/ ANDEQ       R0, R0, R8, ASR #5 
FUNC_END VEDU_DRV_UpdateAwpress


FUNC_BEGIN VEDU_DRV_UpdatePrediction
    /*00002ed4*/ PUSH        {R4, R5, R6, LR} 
    /*00002ed8*/ SUBS        R4, R0, #0 
    /*00002edc*/ MOV         R5, R1 
    /*00002ee0*/ BEQ         VEDU_DRV_UpdatePrediction_x3024
    VEDU_DRV_UpdatePrediction_x2ee4:
    /*00002ee4*/ CMP         R5, #0 
    /*00002ee8*/ BEQ         VEDU_DRV_UpdatePrediction_x3004
    VEDU_DRV_UpdatePrediction_x2eec:
    /*00002eec*/ LDRB        R1, [R4, #93]               @ 0x5d 
    /*00002ef0*/ LDRB        R2, [R4, #92]               @ 0x5c 
    /*00002ef4*/ LDRB        R3, [R4, #2944]             @ 0xb80 
    /*00002ef8*/ AND         R1, R1, #191                @ 0xbf 
    /*00002efc*/ LDRB        LR, [R4, #94]               @ 0x5e 
    /*00002f00*/ AND         R2, R2, #251                @ 0xfb 
    /*00002f04*/ LDRB        IP, [R4, #2108]             @ 0x83c 
    /*00002f08*/ AND         R3, R3, #248                @ 0xf8 
    /*00002f0c*/ LDRB        R0, [R4, #17] 
    /*00002f10*/ BFC         R1, #0, #1 
    /*00002f14*/ BFC         R2, #4, #1 
    /*00002f18*/ STRB        R1, [R4, #93]               @ 0x5d 
    /*00002f1c*/ STRB        R2, [R4, #92]               @ 0x5c 
    /*00002f20*/ BFC         R3, #4, #1 
    /*00002f24*/ BFC         R0, #1, #1 
    /*00002f28*/ STRB        R3, [R4, #2944]             @ 0xb80 
    /*00002f2c*/ STRB        R0, [R4, #17] 
    /*00002f30*/ BFC         LR, #2, #1 
    /*00002f34*/ LDRB        R3, [R4, #1876]             @ 0x754 
    /*00002f38*/ BFC         IP, #0, #1 
    /*00002f3c*/ STRB        LR, [R4, #94]               @ 0x5e 
    /*00002f40*/ STRB        IP, [R4, #2108]             @ 0x83c 
    /*00002f44*/ LDR         R2, [R5, #32] 
    /*00002f48*/ LDRB        R1, [R4, #1877]             @ 0x755 
    /*00002f4c*/ BFI         R3, R2, #0, #4 
    /*00002f50*/ STRB        R3, [R4, #1876]             @ 0x754 
    /*00002f54*/ LDR         R0, [R5, #28] 
    /*00002f58*/ UXTB        R3, R3 
    /*00002f5c*/ LDRB        R2, [R4, #1864]             @ 0x748 
    /*00002f60*/ BFI         R3, R0, #4, #4 
    /*00002f64*/ STRB        R3, [R4, #1876]             @ 0x754 
    /*00002f68*/ LDR         R0, [R5, #24] 
    /*00002f6c*/ LDRB        R3, [R4, #1865]             @ 0x749 
    /*00002f70*/ BFI         R1, R0, #0, #4 
    /*00002f74*/ STRB        R1, [R4, #1877]             @ 0x755 
    /*00002f78*/ LDR         R0, [R5, #20] 
    /*00002f7c*/ UXTB        R1, R1 
    /*00002f80*/ BFI         R1, R0, #4, #4 
    /*00002f84*/ STRB        R1, [R4, #1877]             @ 0x755 
    /*00002f88*/ LDR         R1, [R5, #32] 
    /*00002f8c*/ BFI         R2, R1, #0, #4 
    /*00002f90*/ STRB        R2, [R4, #1864]             @ 0x748 
    /*00002f94*/ LDR         R1, [R5, #28] 
    /*00002f98*/ UXTB        R2, R2 
    /*00002f9c*/ BFI         R2, R1, #4, #4 
    /*00002fa0*/ STRB        R2, [R4, #1864]             @ 0x748 
    /*00002fa4*/ LDR         R2, [R5, #24] 
    /*00002fa8*/ BFI         R3, R2, #0, #4 
    /*00002fac*/ STRB        R3, [R4, #1865]             @ 0x749 
    /*00002fb0*/ LDR         R1, [R5, #20] 
    /*00002fb4*/ UXTB        R3, R3 
    /*00002fb8*/ LDRB        R2, [R4, #1800]             @ 0x708 
    /*00002fbc*/ BFI         R3, R1, #4, #4 
    /*00002fc0*/ STRB        R3, [R4, #1865]             @ 0x749 
    /*00002fc4*/ LDR         R1, [R5, #16] 
    /*00002fc8*/ LDRB        R3, [R4, #1801]             @ 0x709 
    /*00002fcc*/ BFI         R2, R1, #0, #4 
    /*00002fd0*/ STRB        R2, [R4, #1800]             @ 0x708 
    /*00002fd4*/ LDR         R1, [R5, #12] 
    /*00002fd8*/ UXTB        R2, R2 
    /*00002fdc*/ BFI         R2, R1, #4, #4 
    /*00002fe0*/ STRB        R2, [R4, #1800]             @ 0x708 
    /*00002fe4*/ LDR         R2, [R5, #8] 
    /*00002fe8*/ BFI         R3, R2, #0, #4 
    /*00002fec*/ STRB        R3, [R4, #1801]             @ 0x709 
    /*00002ff0*/ LDR         R2, [R5, #4] 
    /*00002ff4*/ UXTB        R3, R3 
    /*00002ff8*/ BFI         R3, R2, #4, #4 
    /*00002ffc*/ STRB        R3, [R4, #1801]             @ 0x709 
    /*00003000*/ POP         {R4, R5, R6, PC} 
    VEDU_DRV_UpdatePrediction_x3004:
    /*00003004*/ MOVW        R3, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7c0] 
    /*00003008*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*0000300c*/ MOVT        R3, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7c0] 
    /*00003010*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00003014*/ MOVW        R2, #929                    @ 0x3a1 
    /*00003018*/ LDR         R1, [PC, #36]               @ 0x0000000000003044 
    /*0000301c*/ BL          0x000000000000301C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003020*/ B           VEDU_DRV_UpdatePrediction_x2eec
    VEDU_DRV_UpdatePrediction_x3024:
    /*00003024*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x708] 
    /*00003028*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*0000302c*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x708] 
    /*00003030*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00003034*/ MOV         R2, #928                    @ 0x3a0 
    /*00003038*/ LDR         R1, [PC, #4]                @ 0x0000000000003044 
    /*0000303c*/ BL          0x000000000000303C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003040*/ B           VEDU_DRV_UpdatePrediction_x2ee4
    /*00003044*/ ANDEQ       R0, R0, R0, ROR #5 
FUNC_END VEDU_DRV_UpdatePrediction


FUNC_BEGIN JPGE_DRV_SetPowerMode
    /*00003048*/ PUSH        {R4, R5, R6, LR} 
    /*0000304c*/ SUBS        R5, R0, #0 
    /*00003050*/ MOV         R4, R1 
    /*00003054*/ BEQ         JPGE_DRV_SetPowerMode_x3094
    JPGE_DRV_SetPowerMode_x3058:
    /*00003058*/ CMP         R4, #1 
    /*0000305c*/ BLS         JPGE_DRV_SetPowerMode_x307c
    /*00003060*/ MOVW        R3, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7f0] 
    /*00003064*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00003068*/ MOVT        R3, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7f0] 
    /*0000306c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00003070*/ MOVW        R2, #990                    @ 0x3de 
    /*00003074*/ LDR         R1, [PC, #56]               @ 0x00000000000030b4 
    /*00003078*/ BL          0x0000000000003078          @ osal_panic [R_ARM_CALL: +0x0] 
    JPGE_DRV_SetPowerMode_x307c:
    /*0000307c*/ UXTB        R4, R4 
    /*00003080*/ MOV         R3, #0 
    /*00003084*/ BFI         R3, R4, #0, #2 
    /*00003088*/ BFI         R3, R4, #2, #1 
    /*0000308c*/ STR         R3, [R5, #60]               @ 0x3c 
    /*00003090*/ POP         {R4, R5, R6, PC} 
    JPGE_DRV_SetPowerMode_x3094:
    /*00003094*/ MOVW        R3, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7dc] 
    /*00003098*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*0000309c*/ MOVT        R3, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7dc] 
    /*000030a0*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*000030a4*/ MOVW        R2, #989                    @ 0x3dd 
    /*000030a8*/ LDR         R1, [PC, #4]                @ 0x00000000000030b4 
    /*000030ac*/ BL          0x00000000000030AC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000030b0*/ B           JPGE_DRV_SetPowerMode_x3058
    /*000030b4*/ STRDEQ      R0, [R0], -IP 
FUNC_END JPGE_DRV_SetPowerMode


FUNC_BEGIN JPGE_DRV_SetTimerMode
    /*000030b8*/ PUSH        {R4, R5, R6, LR} 
    /*000030bc*/ SUBS        R4, R0, #0 
    /*000030c0*/ MOV         R5, R1 
    /*000030c4*/ BEQ         JPGE_DRV_SetTimerMode_x30fc
    JPGE_DRV_SetTimerMode_x30c8:
    /*000030c8*/ CMP         R5, #1 
    /*000030cc*/ BLS         JPGE_DRV_SetTimerMode_x30ec
    /*000030d0*/ MOVW        R3, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x80c] 
    /*000030d4*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*000030d8*/ MOVT        R3, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x80c] 
    /*000030dc*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*000030e0*/ MOVW        R2, #1023                   @ 0x3ff 
    /*000030e4*/ LDR         R1, [PC, #48]               @ 0x000000000000311c 
    /*000030e8*/ BL          0x00000000000030E8          @ osal_panic [R_ARM_CALL: +0x0] 
    JPGE_DRV_SetTimerMode_x30ec:
    /*000030ec*/ LDR         R3, [R4, #48]               @ 0x30 
    /*000030f0*/ BFI         R3, R5, #16, #1 
    /*000030f4*/ STR         R3, [R4, #48]               @ 0x30 
    /*000030f8*/ POP         {R4, R5, R6, PC} 
    JPGE_DRV_SetTimerMode_x30fc:
    /*000030fc*/ MOVW        R3, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7dc] 
    /*00003100*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00003104*/ MOVT        R3, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7dc] 
    /*00003108*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*0000310c*/ MOVW        R2, #1022                   @ 0x3fe 
    /*00003110*/ LDR         R1, [PC, #4]                @ 0x000000000000311c 
    /*00003114*/ BL          0x0000000000003114          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003118*/ B           JPGE_DRV_SetTimerMode_x30c8
    /*0000311c*/ ANDEQ       R0, R0, R4, LSL R3 
FUNC_END JPGE_DRV_SetTimerMode


FUNC_BEGIN JPGE_DRV_SetOutstanding
    /*00003120*/ LDR         R3, [R0, #80]               @ 0x50 
    /*00003124*/ MOV         R2, #7 
    /*00003128*/ BFI         R3, R2, #0, #4 
    /*0000312c*/ STR         R3, [R0, #80]               @ 0x50 
    /*00003130*/ LDR         R3, [R0, #80]               @ 0x50 
    /*00003134*/ ORR         R3, R3, #240                @ 0xf0 
    /*00003138*/ STR         R3, [R0, #80]               @ 0x50 
    /*0000313c*/ BX          LR 
FUNC_END JPGE_DRV_SetOutstanding


FUNC_BEGIN JPGE_DRV_IntEn
    /*00003140*/ PUSH        {R4, LR} 
    /*00003144*/ SUBS        R4, R0, #0 
    /*00003148*/ BEQ         JPGE_DRV_IntEn_x315c
    JPGE_DRV_IntEn_x314c:
    /*0000314c*/ MOV         R3, #3 
    /*00003150*/ BFC         R3, #6, #1 
    /*00003154*/ STR         R3, [R4, #4] 
    /*00003158*/ POP         {R4, PC} 
    JPGE_DRV_IntEn_x315c:
    /*0000315c*/ MOVW        R3, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7dc] 
    /*00003160*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00003164*/ MOVT        R3, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7dc] 
    /*00003168*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*0000316c*/ MOVW        R2, #1065                   @ 0x429 
    /*00003170*/ LDR         R1, [PC, #4]                @ 0x000000000000317c 
    /*00003174*/ BL          0x0000000000003174          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003178*/ B           JPGE_DRV_IntEn_x314c
    /*0000317c*/ ANDEQ       R0, R0, IP, LSR #6 
FUNC_END JPGE_DRV_IntEn


FUNC_BEGIN JPGE_DRV_ReadInt
    /*00003180*/ MOVW        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVW_ABS_NC: .bss+0x14] 
    /*00003184*/ MOVT        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVT_ABS: .bss+0x14] 
    /*00003188*/ LDR         R3, [R3, R0, LSL #2] 
    /*0000318c*/ LDR         R0, [R3] 
    /*00003190*/ BX          LR 
FUNC_END JPGE_DRV_ReadInt


FUNC_BEGIN JPGE_DRV_ClearInt
    /*00003194*/ MOVW        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVW_ABS_NC: .bss+0x14] 
    /*00003198*/ MOVT        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVT_ABS: .bss+0x14] 
    /*0000319c*/ MVN         R2, #0 
    /*000031a0*/ LDR         R3, [R3, R0, LSL #2] 
    /*000031a4*/ MOV         R0, #0 
    /*000031a8*/ STR         R2, [R3, #12] 
    /*000031ac*/ BX          LR 
FUNC_END JPGE_DRV_ClearInt


FUNC_BEGIN JPGE_DRV_ClearErrInt
    /*000031b0*/ MOVW        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVW_ABS_NC: .bss+0x14] 
    /*000031b4*/ MOVT        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVT_ABS: .bss+0x14] 
    /*000031b8*/ MVN         R2, #3 
    /*000031bc*/ LDR         R3, [R3, R0, LSL #2] 
    /*000031c0*/ MOV         R0, #0 
    /*000031c4*/ STR         R2, [R3, #12] 
    /*000031c8*/ BX          LR 
FUNC_END JPGE_DRV_ClearErrInt


FUNC_BEGIN JPGE_DRV_GetIsIntOk
    /*000031cc*/ MOVW        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVW_ABS_NC: .bss+0x14] 
    /*000031d0*/ MOVT        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVT_ABS: .bss+0x14] 
    /*000031d4*/ LDR         R3, [R3, R0, LSL #2] 
    /*000031d8*/ LDR         R0, [R3] 
    /*000031dc*/ ANDS        R0, R0, #1 
    /*000031e0*/ LDREQ       R0, [R3] 
    /*000031e4*/ UBFXEQ      R0, R0, #1, #1 
    /*000031e8*/ BX          LR 
FUNC_END JPGE_DRV_GetIsIntOk


FUNC_BEGIN JPGE_DRV_StartVpu
    /*000031ec*/ MOVW        R2, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVW_ABS_NC: .bss+0x14] 
    /*000031f0*/ MOVT        R2, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVT_ABS: .bss+0x14] 
    /*000031f4*/ MOV         R3, #0 
    /*000031f8*/ LDR         R2, [R2, R0, LSL #2] 
    /*000031fc*/ MOV         R0, R3 
    /*00003200*/ STR         R3, [R2, #44]               @ 0x2c 
    /*00003204*/ MOV         R3, #1 
    /*00003208*/ STR         R3, [R2, #44]               @ 0x2c 
    /*0000320c*/ BX          LR 
FUNC_END JPGE_DRV_StartVpu


FUNC_BEGIN JPGE_DRV_IsNeedReset
    /*00003210*/ MOVW        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVW_ABS_NC: .bss+0x14] 
    /*00003214*/ MOVT        R3, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVT_ABS: .bss+0x14] 
    /*00003218*/ LDR         R3, [R3, R0, LSL #2] 
    /*0000321c*/ LDR         R0, [R3, #8] 
    /*00003220*/ UBFX        R0, R0, #3, #1 
    /*00003224*/ CMP         R0, #1 
    /*00003228*/ LDRNE       R0, [R3, #8] 
    /*0000322c*/ UBFXNE      R0, R0, #2, #1 
    /*00003230*/ BX          LR 
FUNC_END JPGE_DRV_IsNeedReset


FUNC_BEGIN VEDU_HAL_ReadInt
    /*00003234*/ PUSH        {R4, LR} 
    /*00003238*/ SUBS        R4, R0, #0 
    /*0000323c*/ BEQ         VEDU_HAL_ReadInt_x324c
    VEDU_HAL_ReadInt_x3240:
    /*00003240*/ ADD         R4, R4, #12288              @ 0x3000 
    /*00003244*/ LDR         R0, [R4] 
    /*00003248*/ POP         {R4, PC} 
    VEDU_HAL_ReadInt_x324c:
    /*0000324c*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003250*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x33c] 
    /*00003254*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003258*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x33c] 
    /*0000325c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003260*/ MOV         R2, #18 
    /*00003264*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003268*/ BL          0x0000000000003268          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000326c*/ B           VEDU_HAL_ReadInt_x3240
FUNC_END VEDU_HAL_ReadInt


FUNC_BEGIN VEDU_HAL_ReadEndOfPicInt
    /*00003270*/ PUSH        {R4, LR} 
    /*00003274*/ SUBS        R4, R0, #0 
    /*00003278*/ BEQ         VEDU_HAL_ReadEndOfPicInt_x328c
    VEDU_HAL_ReadEndOfPicInt_x327c:
    /*0000327c*/ ADD         R4, R4, #12288              @ 0x3000 
    /*00003280*/ LDR         R0, [R4] 
    /*00003284*/ AND         R0, R0, #1 
    /*00003288*/ POP         {R4, PC} 
    VEDU_HAL_ReadEndOfPicInt_x328c:
    /*0000328c*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003290*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003294*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003298*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*0000329c*/ MOV         R2, #26 
    /*000032a0*/ LDR         R1, [PC, #4]                @ 0x00000000000032ac 
    /*000032a4*/ BL          0x00000000000032A4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000032a8*/ B           VEDU_HAL_ReadEndOfPicInt_x327c
    /*000032ac*/ ANDEQ       R0, R0, R0, ASR R3 
FUNC_END VEDU_HAL_ReadEndOfPicInt


FUNC_BEGIN VEDU_HAL_ReadBufFullInt
    /*000032b0*/ PUSH        {R4, LR} 
    /*000032b4*/ SUBS        R4, R0, #0 
    /*000032b8*/ BEQ         VEDU_HAL_ReadBufFullInt_x32cc
    VEDU_HAL_ReadBufFullInt_x32bc:
    /*000032bc*/ ADD         R4, R4, #12288              @ 0x3000 
    /*000032c0*/ LDR         R0, [R4] 
    /*000032c4*/ UBFX        R0, R0, #2, #1 
    /*000032c8*/ POP         {R4, PC} 
    VEDU_HAL_ReadBufFullInt_x32cc:
    /*000032cc*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*000032d0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*000032d4*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*000032d8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*000032dc*/ MOV         R2, #34                     @ 0x22 
    /*000032e0*/ LDR         R1, [PC, #4]                @ 0x00000000000032ec 
    /*000032e4*/ BL          0x00000000000032E4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000032e8*/ B           VEDU_HAL_ReadBufFullInt_x32bc
    /*000032ec*/ ANDEQ       R0, R0, IP, ROR #6 
FUNC_END VEDU_HAL_ReadBufFullInt


FUNC_BEGIN VEDU_HAL_ReadBitsOverInt
    /*000032f0*/ PUSH        {R4, LR} 
    /*000032f4*/ SUBS        R4, R0, #0 
    /*000032f8*/ BEQ         VEDU_HAL_ReadBitsOverInt_x330c
    VEDU_HAL_ReadBitsOverInt_x32fc:
    /*000032fc*/ ADD         R4, R4, #12288              @ 0x3000 
    /*00003300*/ LDR         R0, [R4] 
    /*00003304*/ UBFX        R0, R0, #3, #1 
    /*00003308*/ POP         {R4, PC} 
    VEDU_HAL_ReadBitsOverInt_x330c:
    /*0000330c*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003310*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003314*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003318*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*0000331c*/ MOV         R2, #42                     @ 0x2a 
    /*00003320*/ LDR         R1, [PC, #4]                @ 0x000000000000332c 
    /*00003324*/ BL          0x0000000000003324          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003328*/ B           VEDU_HAL_ReadBitsOverInt_x32fc
    /*0000332c*/ ANDEQ       R0, R0, R4, LSL #7 
FUNC_END VEDU_HAL_ReadBitsOverInt


FUNC_BEGIN VEDU_HAL_ReadAxiWrBusErrInt
    /*00003330*/ PUSH        {R4, LR} 
    /*00003334*/ SUBS        R4, R0, #0 
    /*00003338*/ BEQ         VEDU_HAL_ReadAxiWrBusErrInt_x334c
    VEDU_HAL_ReadAxiWrBusErrInt_x333c:
    /*0000333c*/ ADD         R4, R4, #12288              @ 0x3000 
    /*00003340*/ LDR         R0, [R4] 
    /*00003344*/ UBFX        R0, R0, #5, #1 
    /*00003348*/ POP         {R4, PC} 
    VEDU_HAL_ReadAxiWrBusErrInt_x334c:
    /*0000334c*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003350*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003354*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003358*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*0000335c*/ MOV         R2, #51                     @ 0x33 
    /*00003360*/ LDR         R1, [PC, #4]                @ 0x000000000000336c 
    /*00003364*/ BL          0x0000000000003364          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003368*/ B           VEDU_HAL_ReadAxiWrBusErrInt_x333c
    /*0000336c*/ ANDEQ       R0, R0, R0, LSR #7 
FUNC_END VEDU_HAL_ReadAxiWrBusErrInt


FUNC_BEGIN VEDU_HAL_ReadAxiRdBusErrInt
    /*00003370*/ PUSH        {R4, LR} 
    /*00003374*/ SUBS        R4, R0, #0 
    /*00003378*/ BEQ         VEDU_HAL_ReadAxiRdBusErrInt_x338c
    VEDU_HAL_ReadAxiRdBusErrInt_x337c:
    /*0000337c*/ ADD         R4, R4, #12288              @ 0x3000 
    /*00003380*/ LDR         R0, [R4] 
    /*00003384*/ UBFX        R0, R0, #4, #1 
    /*00003388*/ POP         {R4, PC} 
    VEDU_HAL_ReadAxiRdBusErrInt_x338c:
    /*0000338c*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003390*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003394*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003398*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*0000339c*/ MOV         R2, #59                     @ 0x3b 
    /*000033a0*/ LDR         R1, [PC, #4]                @ 0x00000000000033ac 
    /*000033a4*/ BL          0x00000000000033A4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000033a8*/ B           VEDU_HAL_ReadAxiRdBusErrInt_x337c
    /*000033ac*/ @           <UNDEFINED> instruction: 0x000003bc 
FUNC_END VEDU_HAL_ReadAxiRdBusErrInt


FUNC_BEGIN VEDU_HAL_ReadTimeoutInt
    /*000033b0*/ PUSH        {R4, LR} 
    /*000033b4*/ SUBS        R4, R0, #0 
    /*000033b8*/ BEQ         VEDU_HAL_ReadTimeoutInt_x33cc
    VEDU_HAL_ReadTimeoutInt_x33bc:
    /*000033bc*/ ADD         R4, R4, #12288              @ 0x3000 
    /*000033c0*/ LDR         R0, [R4] 
    /*000033c4*/ UBFX        R0, R0, #6, #1 
    /*000033c8*/ POP         {R4, PC} 
    VEDU_HAL_ReadTimeoutInt_x33cc:
    /*000033cc*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*000033d0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*000033d4*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*000033d8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*000033dc*/ MOV         R2, #68                     @ 0x44 
    /*000033e0*/ LDR         R1, [PC, #4]                @ 0x00000000000033ec 
    /*000033e4*/ BL          0x00000000000033E4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000033e8*/ B           VEDU_HAL_ReadTimeoutInt_x33bc
    /*000033ec*/ LDRDEQ      R0, [R0], -R8 
FUNC_END VEDU_HAL_ReadTimeoutInt


FUNC_BEGIN VEDU_HAL_ReadLineNumInt
    /*000033f0*/ PUSH        {R4, LR} 
    /*000033f4*/ SUBS        R4, R0, #0 
    /*000033f8*/ BEQ         VEDU_HAL_ReadLineNumInt_x340c
    VEDU_HAL_ReadLineNumInt_x33fc:
    /*000033fc*/ ADD         R4, R4, #12288              @ 0x3000 
    /*00003400*/ LDR         R0, [R4] 
    /*00003404*/ UBFX        R0, R0, #7, #1 
    /*00003408*/ POP         {R4, PC} 
    VEDU_HAL_ReadLineNumInt_x340c:
    /*0000340c*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003410*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003414*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003418*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*0000341c*/ MOV         R2, #76                     @ 0x4c 
    /*00003420*/ LDR         R1, [PC, #4]                @ 0x000000000000342c 
    /*00003424*/ BL          0x0000000000003424          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003428*/ B           VEDU_HAL_ReadLineNumInt_x33fc
    /*0000342c*/ STRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
FUNC_END VEDU_HAL_ReadLineNumInt


FUNC_BEGIN VEDU_HAL_ReadSedErrCoreInt
    /*00003430*/ PUSH        {R4, LR} 
    /*00003434*/ SUBS        R4, R0, #0 
    /*00003438*/ BEQ         VEDU_HAL_ReadSedErrCoreInt_x344c
    VEDU_HAL_ReadSedErrCoreInt_x343c:
    /*0000343c*/ ADD         R4, R4, #12288              @ 0x3000 
    /*00003440*/ LDR         R0, [R4] 
    /*00003444*/ UBFX        R0, R0, #8, #1 
    /*00003448*/ POP         {R4, PC} 
    VEDU_HAL_ReadSedErrCoreInt_x344c:
    /*0000344c*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003450*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003454*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003458*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*0000345c*/ MOV         R2, #84                     @ 0x54 
    /*00003460*/ LDR         R1, [PC, #4]                @ 0x000000000000346c 
    /*00003464*/ BL          0x0000000000003464          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003468*/ B           VEDU_HAL_ReadSedErrCoreInt_x343c
    /*0000346c*/ ANDEQ       R0, R0, R8, LSL #8 
FUNC_END VEDU_HAL_ReadSedErrCoreInt


FUNC_BEGIN VEDU_HAL_ReadSedErrExtInt
    /*00003470*/ PUSH        {R4, LR} 
    /*00003474*/ SUBS        R4, R0, #0 
    /*00003478*/ BEQ         VEDU_HAL_ReadSedErrExtInt_x348c
    VEDU_HAL_ReadSedErrExtInt_x347c:
    /*0000347c*/ ADD         R4, R4, #12288              @ 0x3000 
    /*00003480*/ LDR         R0, [R4] 
    /*00003484*/ UBFX        R0, R0, #9, #1 
    /*00003488*/ POP         {R4, PC} 
    VEDU_HAL_ReadSedErrExtInt_x348c:
    /*0000348c*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003490*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003494*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003498*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*0000349c*/ MOV         R2, #92                     @ 0x5c 
    /*000034a0*/ LDR         R1, [PC, #4]                @ 0x00000000000034ac 
    /*000034a4*/ BL          0x00000000000034A4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000034a8*/ B           VEDU_HAL_ReadSedErrExtInt_x347c
    /*000034ac*/ ANDEQ       R0, R0, R4, LSR #8 
FUNC_END VEDU_HAL_ReadSedErrExtInt


FUNC_BEGIN VEDU_HAL_ReadRawInt
    /*000034b0*/ PUSH        {R4, LR} 
    /*000034b4*/ SUBS        R4, R0, #0 
    /*000034b8*/ BEQ         VEDU_HAL_ReadRawInt_x34c8
    VEDU_HAL_ReadRawInt_x34bc:
    /*000034bc*/ ADD         R4, R4, #12288              @ 0x3000 
    /*000034c0*/ LDR         R0, [R4, #4] 
    /*000034c4*/ POP         {R4, PC} 
    VEDU_HAL_ReadRawInt_x34c8:
    /*000034c8*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*000034cc*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*000034d0*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*000034d4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*000034d8*/ MOV         R2, #103                    @ 0x67 
    /*000034dc*/ LDR         R1, [PC, #4]                @ 0x00000000000034e8 
    /*000034e0*/ BL          0x00000000000034E0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000034e4*/ B           VEDU_HAL_ReadRawInt_x34bc
    /*000034e8*/ ANDEQ       R0, R0, R0, ASR #8 
FUNC_END VEDU_HAL_ReadRawInt


FUNC_BEGIN VEDU_HAL_ReadEndOfPicRawInt
    /*000034ec*/ PUSH        {R4, LR} 
    /*000034f0*/ SUBS        R4, R0, #0 
    /*000034f4*/ BEQ         VEDU_HAL_ReadEndOfPicRawInt_x3508
    VEDU_HAL_ReadEndOfPicRawInt_x34f8:
    /*000034f8*/ ADD         R4, R4, #12288              @ 0x3000 
    /*000034fc*/ LDR         R0, [R4, #4] 
    /*00003500*/ AND         R0, R0, #1 
    /*00003504*/ POP         {R4, PC} 
    VEDU_HAL_ReadEndOfPicRawInt_x3508:
    /*00003508*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*0000350c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003510*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003514*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003518*/ MOV         R2, #112                    @ 0x70 
    /*0000351c*/ LDR         R1, [PC, #4]                @ 0x0000000000003528 
    /*00003520*/ BL          0x0000000000003520          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003524*/ B           VEDU_HAL_ReadEndOfPicRawInt_x34f8
    /*00003528*/ ANDEQ       R0, R0, R4, ASR R4 
FUNC_END VEDU_HAL_ReadEndOfPicRawInt


FUNC_BEGIN VEDU_HAL_ReadBufFullRawInt
    /*0000352c*/ PUSH        {R4, LR} 
    /*00003530*/ SUBS        R4, R0, #0 
    /*00003534*/ BEQ         VEDU_HAL_ReadBufFullRawInt_x3548
    VEDU_HAL_ReadBufFullRawInt_x3538:
    /*00003538*/ ADD         R4, R4, #12288              @ 0x3000 
    /*0000353c*/ LDR         R0, [R4, #4] 
    /*00003540*/ UBFX        R0, R0, #2, #1 
    /*00003544*/ POP         {R4, PC} 
    VEDU_HAL_ReadBufFullRawInt_x3548:
    /*00003548*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*0000354c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003550*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003554*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003558*/ MOV         R2, #120                    @ 0x78 
    /*0000355c*/ LDR         R1, [PC, #4]                @ 0x0000000000003568 
    /*00003560*/ BL          0x0000000000003560          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003564*/ B           VEDU_HAL_ReadBufFullRawInt_x3538
    /*00003568*/ ANDEQ       R0, R0, R0, ROR R4 
FUNC_END VEDU_HAL_ReadBufFullRawInt


FUNC_BEGIN VEDU_HAL_ReadBitsOverRawInt
    /*0000356c*/ PUSH        {R4, LR} 
    /*00003570*/ SUBS        R4, R0, #0 
    /*00003574*/ BEQ         VEDU_HAL_ReadBitsOverRawInt_x3588
    VEDU_HAL_ReadBitsOverRawInt_x3578:
    /*00003578*/ ADD         R4, R4, #12288              @ 0x3000 
    /*0000357c*/ LDR         R0, [R4, #4] 
    /*00003580*/ UBFX        R0, R0, #3, #1 
    /*00003584*/ POP         {R4, PC} 
    VEDU_HAL_ReadBitsOverRawInt_x3588:
    /*00003588*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*0000358c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003590*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003594*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003598*/ MOV         R2, #128                    @ 0x80 
    /*0000359c*/ LDR         R1, [PC, #4]                @ 0x00000000000035a8 
    /*000035a0*/ BL          0x00000000000035A0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000035a4*/ B           VEDU_HAL_ReadBitsOverRawInt_x3578
    /*000035a8*/ ANDEQ       R0, R0, IP, LSL #9 
FUNC_END VEDU_HAL_ReadBitsOverRawInt


FUNC_BEGIN VEDU_HAL_ReadAxiWrBusErrRawInt
    /*000035ac*/ PUSH        {R4, LR} 
    /*000035b0*/ SUBS        R4, R0, #0 
    /*000035b4*/ BEQ         VEDU_HAL_ReadAxiWrBusErrRawInt_x35c8
    VEDU_HAL_ReadAxiWrBusErrRawInt_x35b8:
    /*000035b8*/ ADD         R4, R4, #12288              @ 0x3000 
    /*000035bc*/ LDR         R0, [R4, #4] 
    /*000035c0*/ UBFX        R0, R0, #5, #1 
    /*000035c4*/ POP         {R4, PC} 
    VEDU_HAL_ReadAxiWrBusErrRawInt_x35c8:
    /*000035c8*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*000035cc*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*000035d0*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*000035d4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*000035d8*/ MOV         R2, #137                    @ 0x89 
    /*000035dc*/ LDR         R1, [PC, #4]                @ 0x00000000000035e8 
    /*000035e0*/ BL          0x00000000000035E0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000035e4*/ B           VEDU_HAL_ReadAxiWrBusErrRawInt_x35b8
    /*000035e8*/ ANDEQ       R0, R0, R8, LSR #9 
FUNC_END VEDU_HAL_ReadAxiWrBusErrRawInt


FUNC_BEGIN VEDU_HAL_ReadAxiRdBusErrRawInt
    /*000035ec*/ PUSH        {R4, LR} 
    /*000035f0*/ SUBS        R4, R0, #0 
    /*000035f4*/ BEQ         VEDU_HAL_ReadAxiRdBusErrRawInt_x3608
    VEDU_HAL_ReadAxiRdBusErrRawInt_x35f8:
    /*000035f8*/ ADD         R4, R4, #12288              @ 0x3000 
    /*000035fc*/ LDR         R0, [R4, #4] 
    /*00003600*/ UBFX        R0, R0, #4, #1 
    /*00003604*/ POP         {R4, PC} 
    VEDU_HAL_ReadAxiRdBusErrRawInt_x3608:
    /*00003608*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*0000360c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003610*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003614*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003618*/ MOV         R2, #145                    @ 0x91 
    /*0000361c*/ LDR         R1, [PC, #4]                @ 0x0000000000003628 
    /*00003620*/ BL          0x0000000000003620          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003624*/ B           VEDU_HAL_ReadAxiRdBusErrRawInt_x35f8
    /*00003628*/ ANDEQ       R0, R0, R8, ASR #9 
FUNC_END VEDU_HAL_ReadAxiRdBusErrRawInt


FUNC_BEGIN VEDU_HAL_ReadTimeoutRawInt
    /*0000362c*/ PUSH        {R4, LR} 
    /*00003630*/ SUBS        R4, R0, #0 
    /*00003634*/ BEQ         VEDU_HAL_ReadTimeoutRawInt_x3648
    VEDU_HAL_ReadTimeoutRawInt_x3638:
    /*00003638*/ ADD         R4, R4, #12288              @ 0x3000 
    /*0000363c*/ LDR         R0, [R4, #4] 
    /*00003640*/ UBFX        R0, R0, #6, #1 
    /*00003644*/ POP         {R4, PC} 
    VEDU_HAL_ReadTimeoutRawInt_x3648:
    /*00003648*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*0000364c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003650*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003654*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003658*/ MOV         R2, #154                    @ 0x9a 
    /*0000365c*/ LDR         R1, [PC, #4]                @ 0x0000000000003668 
    /*00003660*/ BL          0x0000000000003660          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003664*/ B           VEDU_HAL_ReadTimeoutRawInt_x3638
    /*00003668*/ ANDEQ       R0, R0, R8, ROR #9 
FUNC_END VEDU_HAL_ReadTimeoutRawInt


FUNC_BEGIN VEDU_HAL_ReadLineNumRawInt
    /*0000366c*/ PUSH        {R4, LR} 
    /*00003670*/ SUBS        R4, R0, #0 
    /*00003674*/ BEQ         VEDU_HAL_ReadLineNumRawInt_x3688
    VEDU_HAL_ReadLineNumRawInt_x3678:
    /*00003678*/ ADD         R4, R4, #12288              @ 0x3000 
    /*0000367c*/ LDR         R0, [R4, #4] 
    /*00003680*/ UBFX        R0, R0, #7, #1 
    /*00003684*/ POP         {R4, PC} 
    VEDU_HAL_ReadLineNumRawInt_x3688:
    /*00003688*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*0000368c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003690*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003694*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003698*/ MOV         R2, #162                    @ 0xa2 
    /*0000369c*/ LDR         R1, [PC, #4]                @ 0x00000000000036a8 
    /*000036a0*/ BL          0x00000000000036A0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000036a4*/ B           VEDU_HAL_ReadLineNumRawInt_x3678
    /*000036a8*/ ANDEQ       R0, R0, R4, LSL #10 
FUNC_END VEDU_HAL_ReadLineNumRawInt


FUNC_BEGIN VEDU_HAL_ReadSedErrCoreRawInt
    /*000036ac*/ PUSH        {R4, LR} 
    /*000036b0*/ SUBS        R4, R0, #0 
    /*000036b4*/ BEQ         VEDU_HAL_ReadSedErrCoreRawInt_x36c8
    VEDU_HAL_ReadSedErrCoreRawInt_x36b8:
    /*000036b8*/ ADD         R4, R4, #12288              @ 0x3000 
    /*000036bc*/ LDR         R0, [R4, #4] 
    /*000036c0*/ UBFX        R0, R0, #8, #1 
    /*000036c4*/ POP         {R4, PC} 
    VEDU_HAL_ReadSedErrCoreRawInt_x36c8:
    /*000036c8*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*000036cc*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*000036d0*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*000036d4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*000036d8*/ MOV         R2, #170                    @ 0xaa 
    /*000036dc*/ LDR         R1, [PC, #4]                @ 0x00000000000036e8 
    /*000036e0*/ BL          0x00000000000036E0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000036e4*/ B           VEDU_HAL_ReadSedErrCoreRawInt_x36b8
    /*000036e8*/ ANDEQ       R0, R0, R0, LSR #10 
FUNC_END VEDU_HAL_ReadSedErrCoreRawInt


FUNC_BEGIN VEDU_HAL_ReadSedErrExtRawInt
    /*000036ec*/ PUSH        {R4, LR} 
    /*000036f0*/ SUBS        R4, R0, #0 
    /*000036f4*/ BEQ         VEDU_HAL_ReadSedErrExtRawInt_x3708
    VEDU_HAL_ReadSedErrExtRawInt_x36f8:
    /*000036f8*/ ADD         R4, R4, #12288              @ 0x3000 
    /*000036fc*/ LDR         R0, [R4, #4] 
    /*00003700*/ UBFX        R0, R0, #9, #1 
    /*00003704*/ POP         {R4, PC} 
    VEDU_HAL_ReadSedErrExtRawInt_x3708:
    /*00003708*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*0000370c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003710*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003714*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003718*/ MOV         R2, #178                    @ 0xb2 
    /*0000371c*/ LDR         R1, [PC, #4]                @ 0x0000000000003728 
    /*00003720*/ BL          0x0000000000003720          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003724*/ B           VEDU_HAL_ReadSedErrExtRawInt_x36f8
    /*00003728*/ ANDEQ       R0, R0, R0, ASR #10 
FUNC_END VEDU_HAL_ReadSedErrExtRawInt


FUNC_BEGIN VEDU_HAL_ClearInt
    /*0000372c*/ PUSH        {R4, R5, R6, LR} 
    /*00003730*/ SUBS        R4, R0, #0 
    /*00003734*/ MOV         R5, R1 
    /*00003738*/ BEQ         VEDU_HAL_ClearInt_x3748
    VEDU_HAL_ClearInt_x373c:
    /*0000373c*/ STR         R5, [R4, #4] 
    /*00003740*/ MOV         R0, #0 
    /*00003744*/ POP         {R4, R5, R6, PC} 
    VEDU_HAL_ClearInt_x3748:
    /*00003748*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*0000374c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003750*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003754*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003758*/ MOV         R2, #188                    @ 0xbc 
    /*0000375c*/ LDR         R1, [PC, #4]                @ 0x0000000000003768 
    /*00003760*/ BL          0x0000000000003760          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003764*/ B           VEDU_HAL_ClearInt_x373c
    /*00003768*/ ANDEQ       R0, R0, R0, ROR #10 
FUNC_END VEDU_HAL_ClearInt


FUNC_BEGIN VEDU_HAL_ResetRequest
    /*0000376c*/ PUSH        {R4, LR} 
    /*00003770*/ SUBS        R4, R0, #0 
    /*00003774*/ BEQ         VEDU_HAL_ResetRequest_x3784
    VEDU_HAL_ResetRequest_x3778:
    /*00003778*/ MOV         R3, #1 
    /*0000377c*/ STR         R3, [R4, #76]               @ 0x4c 
    /*00003780*/ POP         {R4, PC} 
    VEDU_HAL_ResetRequest_x3784:
    /*00003784*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003788*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*0000378c*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003790*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003794*/ MOV         R2, #199                    @ 0xc7 
    /*00003798*/ LDR         R1, [PC, #4]                @ 0x00000000000037a4 
    /*0000379c*/ BL          0x000000000000379C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000037a0*/ B           VEDU_HAL_ResetRequest_x3778
    /*000037a4*/ ANDEQ       R0, R0, R4, ROR R5 
FUNC_END VEDU_HAL_ResetRequest


FUNC_BEGIN VEDU_HAL_ReadBusIdleFlag
    /*000037a8*/ PUSH        {R4, R5, R6, LR} 
    /*000037ac*/ MOVW        R4, #34464                  @ 0x86a0 
    /*000037b0*/ ADD         R5, R0, #12288              @ 0x3000 
    /*000037b4*/ MOVT        R4, #1 
    /*000037b8*/ B           VEDU_HAL_ReadBusIdleFlag_x37c8
    VEDU_HAL_ReadBusIdleFlag_x37bc:
    /*000037bc*/ BL          0x00000000000037BC          @ osal_udelay [R_ARM_CALL: +0x0] 
    /*000037c0*/ SUBS        R4, R4, #1 
    /*000037c4*/ BEQ         VEDU_HAL_ReadBusIdleFlag_x37e0
    VEDU_HAL_ReadBusIdleFlag_x37c8:
    /*000037c8*/ LDR         R3, [R5, #16] 
    /*000037cc*/ MOV         R0, #1 
    /*000037d0*/ ANDS        R3, R3, R0 
    /*000037d4*/ BEQ         VEDU_HAL_ReadBusIdleFlag_x37bc
    /*000037d8*/ MOV         R0, R3 
    /*000037dc*/ POP         {R4, R5, R6, PC} 
    VEDU_HAL_ReadBusIdleFlag_x37e0:
    /*000037e0*/ MOV         R0, R4 
    /*000037e4*/ POP         {R4, R5, R6, PC} 
FUNC_END VEDU_HAL_ReadBusIdleFlag


FUNC_BEGIN VEDU_HAL_StartVedu
    /*000037e8*/ PUSH        {R4, LR} 
    /*000037ec*/ SUBS        R4, R0, #0 
    /*000037f0*/ BEQ         VEDU_HAL_StartVedu_x3804
    VEDU_HAL_StartVedu_x37f4:
    /*000037f4*/ MOV         R3, #1 
    /*000037f8*/ MOV         R0, #0 
    /*000037fc*/ STR         R3, [R4, #8] 
    /*00003800*/ POP         {R4, PC} 
    VEDU_HAL_StartVedu_x3804:
    /*00003804*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003808*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*0000380c*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003810*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003814*/ MOV         R2, #236                    @ 0xec 
    /*00003818*/ LDR         R1, [PC, #4]                @ 0x0000000000003824 
    /*0000381c*/ BL          0x000000000000381C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003820*/ B           VEDU_HAL_StartVedu_x37f4
    /*00003824*/ ANDEQ       R0, R0, IP, LSL #11 
FUNC_END VEDU_HAL_StartVedu


FUNC_BEGIN VEDU_HAL_ReadIntMask
    /*00003828*/ PUSH        {R4, LR} 
    /*0000382c*/ SUBS        R4, R0, #0 
    /*00003830*/ BEQ         VEDU_HAL_ReadIntMask_x383c
    /*00003834*/ LDR         R0, [R4] 
    /*00003838*/ POP         {R4, PC} 
    VEDU_HAL_ReadIntMask_x383c:
    /*0000383c*/ MOV         R2, #248                    @ 0xf8 
    /*00003840*/ LDR         R1, [PC, #24]               @ 0x0000000000003860 
    /*00003844*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003848*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*0000384c*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003850*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003854*/ BL          0x0000000000003854          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003858*/ LDR         R0, [R4] 
    /*0000385c*/ POP         {R4, PC} 
    /*00003860*/ ANDEQ       R0, R0, R0, LSR #11 
FUNC_END VEDU_HAL_ReadIntMask


FUNC_BEGIN VEDU_HAL_WriteIntMask
    /*00003864*/ PUSH        {R4, R5, R6, LR} 
    /*00003868*/ SUBS        R4, R0, #0 
    /*0000386c*/ MOV         R5, R1 
    /*00003870*/ BEQ         VEDU_HAL_WriteIntMask_x387c
    /*00003874*/ STR         R5, [R4] 
    /*00003878*/ POP         {R4, R5, R6, PC} 
    VEDU_HAL_WriteIntMask_x387c:
    /*0000387c*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003880*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003884*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003888*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*0000388c*/ MOV         R2, #255                    @ 0xff 
    /*00003890*/ LDR         R1, [PC, #8]                @ 0x00000000000038a0 
    /*00003894*/ BL          0x0000000000003894          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003898*/ STR         R5, [R4] 
    /*0000389c*/ POP         {R4, R5, R6, PC} 
    /*000038a0*/ @           <UNDEFINED> instruction: 0x000005b8 
FUNC_END VEDU_HAL_WriteIntMask


FUNC_BEGIN VEDU_HAL_ReadMode
    /*000038a4*/ PUSH        {R4, LR} 
    /*000038a8*/ SUBS        R4, R0, #0 
    /*000038ac*/ BEQ         VEDU_HAL_ReadMode_x38b8
    /*000038b0*/ LDR         R0, [R4, #16] 
    /*000038b4*/ POP         {R4, PC} 
    VEDU_HAL_ReadMode_x38b8:
    /*000038b8*/ MOVW        R2, #311                    @ 0x137 
    /*000038bc*/ LDR         R1, [PC, #24]               @ 0x00000000000038dc 
    /*000038c0*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*000038c4*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*000038c8*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*000038cc*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*000038d0*/ BL          0x00000000000038D0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000038d4*/ LDR         R0, [R4, #16] 
    /*000038d8*/ POP         {R4, PC} 
    /*000038dc*/ LDRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
FUNC_END VEDU_HAL_ReadMode


FUNC_BEGIN VEDU_HAL_WriteMode
    /*000038e0*/ PUSH        {R4, R5, R6, LR} 
    /*000038e4*/ SUBS        R4, R0, #0 
    /*000038e8*/ MOV         R5, R1 
    /*000038ec*/ BEQ         VEDU_HAL_WriteMode_x38f8
    /*000038f0*/ STR         R5, [R4, #16] 
    /*000038f4*/ POP         {R4, R5, R6, PC} 
    VEDU_HAL_WriteMode_x38f8:
    /*000038f8*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*000038fc*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003900*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003904*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003908*/ MOVW        R2, #318                    @ 0x13e 
    /*0000390c*/ LDR         R1, [PC, #8]                @ 0x000000000000391c 
    /*00003910*/ BL          0x0000000000003910          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003914*/ STR         R5, [R4, #16] 
    /*00003918*/ POP         {R4, R5, R6, PC} 
    /*0000391c*/ ANDEQ       R0, R0, R4, ROR #11 
FUNC_END VEDU_HAL_WriteMode


FUNC_BEGIN VEDU_HAL_WriteClkGateMode
    /*00003920*/ PUSH        {R4, R5, R6, LR} 
    /*00003924*/ SUBS        R4, R0, #0 
    /*00003928*/ MOV         R5, R1 
    /*0000392c*/ BEQ         VEDU_HAL_WriteClkGateMode_x3940
    VEDU_HAL_WriteClkGateMode_x3930:
    /*00003930*/ LDR         R3, [R4, #1280]             @ 0x500 
    /*00003934*/ BFI         R3, R5, #19, #2 
    /*00003938*/ STR         R3, [R4, #1280]             @ 0x500 
    /*0000393c*/ POP         {R4, R5, R6, PC} 
    VEDU_HAL_WriteClkGateMode_x3940:
    /*00003940*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003944*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003948*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*0000394c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003950*/ MOVW        R2, #329                    @ 0x149 
    /*00003954*/ LDR         R1, [PC, #4]                @ 0x0000000000003960 
    /*00003958*/ BL          0x0000000000003958          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000395c*/ B           VEDU_HAL_WriteClkGateMode_x3930
    /*00003960*/ STRDEQ      R0, [R0], -R8 
FUNC_END VEDU_HAL_WriteClkGateMode


FUNC_BEGIN VEDU_HAL_WriteMemClkGateMode
    /*00003964*/ PUSH        {R4, R5, R6, LR} 
    /*00003968*/ SUBS        R4, R0, #0 
    /*0000396c*/ MOV         R5, R1 
    /*00003970*/ BEQ         VEDU_HAL_WriteMemClkGateMode_x3984
    VEDU_HAL_WriteMemClkGateMode_x3974:
    /*00003974*/ LDR         R3, [R4, #1280]             @ 0x500 
    /*00003978*/ BFI         R3, R5, #21, #1 
    /*0000397c*/ STR         R3, [R4, #1280]             @ 0x500 
    /*00003980*/ POP         {R4, R5, R6, PC} 
    VEDU_HAL_WriteMemClkGateMode_x3984:
    /*00003984*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003988*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*0000398c*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003990*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003994*/ MOVW        R2, #343                    @ 0x157 
    /*00003998*/ LDR         R1, [PC, #4]                @ 0x00000000000039a4 
    /*0000399c*/ BL          0x000000000000399C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000039a0*/ B           VEDU_HAL_WriteMemClkGateMode_x3974
    /*000039a4*/ ANDEQ       R0, R0, R4, LSL R6 
FUNC_END VEDU_HAL_WriteMemClkGateMode


FUNC_BEGIN VEDU_HAL_SetPowerMode
    /*000039a8*/ PUSH        {R4, R5, R6, LR} 
    /*000039ac*/ SUBS        R5, R0, #0 
    /*000039b0*/ MOV         R4, R1 
    /*000039b4*/ BEQ         VEDU_HAL_SetPowerMode_x39dc
    VEDU_HAL_SetPowerMode_x39b8:
    /*000039b8*/ LDR         R3, [R5, #1280]             @ 0x500 
    /*000039bc*/ AND         R1, R4, #1 
    /*000039c0*/ BFI         R3, R1, #17, #1 
    /*000039c4*/ BFI         R3, R1, #16, #1 
    /*000039c8*/ BFI         R3, R1, #2, #1 
    /*000039cc*/ BFI         R3, R1, #1, #1 
    /*000039d0*/ BFI         R3, R1, #0, #1 
    /*000039d4*/ STR         R3, [R5, #1280]             @ 0x500 
    /*000039d8*/ POP         {R4, R5, R6, PC} 
    VEDU_HAL_SetPowerMode_x39dc:
    /*000039dc*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*000039e0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*000039e4*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*000039e8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*000039ec*/ MOV         R2, #360                    @ 0x168 
    /*000039f0*/ LDR         R1, [PC, #4]                @ 0x00000000000039fc 
    /*000039f4*/ BL          0x00000000000039F4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000039f8*/ B           VEDU_HAL_SetPowerMode_x39b8
    /*000039fc*/ ANDEQ       R0, R0, R4, LSR R6 
FUNC_END VEDU_HAL_SetPowerMode


FUNC_BEGIN VEDU_HAL_WriteTimerMode
    /*00003a00*/ PUSH        {R4, R5, R6, LR} 
    /*00003a04*/ SUBS        R5, R0, #0 
    /*00003a08*/ MOV         R6, R1 
    /*00003a0c*/ LDRNE       R4, [R5, #16] 
    /*00003a10*/ BFINE       R4, R1, #28, #1 
    /*00003a14*/ BEQ         VEDU_HAL_WriteTimerMode_x3a20
    /*00003a18*/ STR         R4, [R5, #16] 
    /*00003a1c*/ POP         {R4, R5, R6, PC} 
    VEDU_HAL_WriteTimerMode_x3a20:
    /*00003a20*/ MOVW        R2, #311                    @ 0x137 
    /*00003a24*/ LDR         R1, [PC, #60]               @ 0x0000000000003a68 
    /*00003a28*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003a2c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003a30*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003a34*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003a38*/ BL          0x0000000000003A38          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003a3c*/ LDR         R4, [R5, #16] 
    /*00003a40*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003a44*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003a48*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003a4c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003a50*/ MOVW        R2, #318                    @ 0x13e 
    /*00003a54*/ LDR         R1, [PC, #16]               @ 0x0000000000003a6c 
    /*00003a58*/ BFI         R4, R6, #28, #1 
    /*00003a5c*/ BL          0x0000000000003A5C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003a60*/ STR         R4, [R5, #16] 
    /*00003a64*/ POP         {R4, R5, R6, PC} 
    /*00003a68*/ LDRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
    /*00003a6c*/ ANDEQ       R0, R0, R4, ROR #11 
FUNC_END VEDU_HAL_WriteTimerMode


FUNC_BEGIN VEDU_HAL_ReadOutStanding
    /*00003a70*/ PUSH        {R4, LR} 
    /*00003a74*/ SUBS        R4, R0, #0 
    /*00003a78*/ BEQ         VEDU_HAL_ReadOutStanding_x3a84
    /*00003a7c*/ LDR         R0, [R4, #28] 
    /*00003a80*/ POP         {R4, PC} 
    VEDU_HAL_ReadOutStanding_x3a84:
    /*00003a84*/ MOVW        R2, #393                    @ 0x189 
    /*00003a88*/ LDR         R1, [PC, #24]               @ 0x0000000000003aa8 
    /*00003a8c*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003a90*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003a94*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003a98*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003a9c*/ BL          0x0000000000003A9C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003aa0*/ LDR         R0, [R4, #28] 
    /*00003aa4*/ POP         {R4, PC} 
    /*00003aa8*/ ANDEQ       R0, R0, IP, ASR #12 
FUNC_END VEDU_HAL_ReadOutStanding


FUNC_BEGIN VEDU_HAL_WriteOutStanding
    /*00003aac*/ PUSH        {R4, LR} 
    /*00003ab0*/ SUBS        R4, R0, #0 
    /*00003ab4*/ BEQ         VEDU_HAL_WriteOutStanding_x3afc
    VEDU_HAL_WriteOutStanding_x3ab8:
    /*00003ab8*/ LDR         R3, [R4, #28] 
    /*00003abc*/ MOV         R2, #7 
    /*00003ac0*/ BFI         R3, R2, #8, #4 
    /*00003ac4*/ STR         R3, [R4, #28] 
    /*00003ac8*/ LDR         R3, [R4, #28] 
    /*00003acc*/ ORR         R3, R3, #31 
    /*00003ad0*/ STR         R3, [R4, #28] 
    /*00003ad4*/ LDR         R3, [R4, #28] 
    /*00003ad8*/ BFC         R3, #16, #1 
    /*00003adc*/ STR         R3, [R4, #28] 
    /*00003ae0*/ LDR         R3, [R4, #28] 
    /*00003ae4*/ BFC         R3, #20, #1 
    /*00003ae8*/ STR         R3, [R4, #28] 
    /*00003aec*/ LDR         R3, [R4, #28] 
    /*00003af0*/ BFC         R3, #21, #1 
    /*00003af4*/ STR         R3, [R4, #28] 
    /*00003af8*/ POP         {R4, PC} 
    VEDU_HAL_WriteOutStanding_x3afc:
    /*00003afc*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*00003b00*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00003b04*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*00003b08*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00003b0c*/ MOV         R2, #400                    @ 0x190 
    /*00003b10*/ LDR         R1, [PC, #4]                @ 0x0000000000003b1c 
    /*00003b14*/ BL          0x0000000000003B14          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003b18*/ B           VEDU_HAL_WriteOutStanding_x3ab8
    /*00003b1c*/ ANDEQ       R0, R0, R8, ROR #12 
FUNC_END VEDU_HAL_WriteOutStanding


FUNC_BEGIN VEDU_DEC_HAL_H264_UpdateRefInfo
    /*00003b20*/ PUSH        {R4, R5, R6, LR} 
    /*00003b24*/ MOV         R2, #0 
    /*00003b28*/ LDR         R3, [R0, #8] 
    /*00003b2c*/ SUB         SP, SP, #16 
    /*00003b30*/ LDR         IP, [R0, #4] 
    /*00003b34*/ CMP         R3, R2 
    /*00003b38*/ LDR         LR, [R0, #104]              @ 0x68 
    /*00003b3c*/ STR         R2, [SP, #4] 
    /*00003b40*/ STR         R2, [SP] 
    /*00003b44*/ STR         R2, [SP, #8] 
    /*00003b48*/ STR         R2, [SP, #12] 
    /*00003b4c*/ BEQ         VEDU_DEC_HAL_H264_UpdateRefInfo_x3cd0
    /*00003b50*/ LDR         R2, [R0, #100]              @ 0x64 
    /*00003b54*/ B           VEDU_DEC_HAL_H264_UpdateRefInfo_x3b6c
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3b58:
    /*00003b58*/ CMP         R2, #2 
    /*00003b5c*/ BEQ         VEDU_DEC_HAL_H264_UpdateRefInfo_x3c04
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3b60:
    /*00003b60*/ LDR         R3, [R3, #1504]             @ 0x5e0 
    /*00003b64*/ CMP         R3, #0 
    /*00003b68*/ BEQ         VEDU_DEC_HAL_H264_UpdateRefInfo_x3cd0
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3b6c:
    /*00003b6c*/ CMP         R2, #1 
    /*00003b70*/ BNE         VEDU_DEC_HAL_H264_UpdateRefInfo_x3b58
    /*00003b74*/ LDRB        R1, [R3, #36]               @ 0x24 
    /*00003b78*/ CMP         R1, #0 
    /*00003b7c*/ BNE         VEDU_DEC_HAL_H264_UpdateRefInfo_x3b60
    /*00003b80*/ LDRB        R2, [R3, #560]              @ 0x230 
    /*00003b84*/ CMP         R2, #0 
    /*00003b88*/ BEQ         VEDU_DEC_HAL_H264_UpdateRefInfo_x3ccc
    /*00003b8c*/ ADD         R0, R2, #820                @ 0x334 
    /*00003b90*/ ADD         R2, R3, #820                @ 0x334 
    /*00003b94*/ ADD         R3, R3, R0 
    /*00003b98*/ B           VEDU_DEC_HAL_H264_UpdateRefInfo_x3ba8
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3b9c:
    /*00003b9c*/ ADD         R2, R2, #1 
    /*00003ba0*/ CMP         R2, R3 
    /*00003ba4*/ BEQ         VEDU_DEC_HAL_H264_UpdateRefInfo_x3be0
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3ba8:
    /*00003ba8*/ CMP         R1, #1 
    /*00003bac*/ BGT         VEDU_DEC_HAL_H264_UpdateRefInfo_x3b9c
    /*00003bb0*/ LDRB        R0, [R2] 
    /*00003bb4*/ ADD         R4, SP, #16 
    /*00003bb8*/ ADD         R2, R2, #1 
    /*00003bbc*/ ADD         R4, R4, R1, LSL #2 
    /*00003bc0*/ CMP         R2, R3 
    /*00003bc4*/ ADD         R1, R1, #1 
    /*00003bc8*/ ADD         R0, IP, R0, LSL #2 
    /*00003bcc*/ LDR         R5, [R0, #120]              @ 0x78 
    /*00003bd0*/ LDR         R0, [R0, #248]              @ 0xf8 
    /*00003bd4*/ STR         R5, [R4, #-16] 
    /*00003bd8*/ STR         R0, [R4, #-8] 
    /*00003bdc*/ BNE         VEDU_DEC_HAL_H264_UpdateRefInfo_x3ba8
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3be0:
    /*00003be0*/ CMP         R1, #1 
    /*00003be4*/ LDR         R4, [SP] 
    /*00003be8*/ LDREQ       R5, [SP, #8] 
    /*00003bec*/ LDRNE       R6, [SP, #4] 
    /*00003bf0*/ MOVEQ       R6, R4 
    /*00003bf4*/ LDRNE       R5, [SP, #8] 
    /*00003bf8*/ LDRNE       R3, [SP, #12] 
    /*00003bfc*/ MOVEQ       R3, R5 
    /*00003c00*/ B           VEDU_DEC_HAL_H264_UpdateRefInfo_x3ce0
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3c04:
    /*00003c04*/ LDRB        R1, [R3, #36]               @ 0x24 
    /*00003c08*/ CMP         R1, #1 
    /*00003c0c*/ BNE         VEDU_DEC_HAL_H264_UpdateRefInfo_x3b60
    /*00003c10*/ LDRB        R1, [R3, #560]              @ 0x230 
    /*00003c14*/ CMP         R1, #0 
    /*00003c18*/ BEQ         VEDU_DEC_HAL_H264_UpdateRefInfo_x3d0c
    /*00003c1c*/ ADD         R1, R1, #820                @ 0x334 
    /*00003c20*/ ADD         R2, R3, #820                @ 0x334 
    /*00003c24*/ ADD         R0, R3, R1 
    /*00003c28*/ MOV         R1, #0 
    /*00003c2c*/ B           VEDU_DEC_HAL_H264_UpdateRefInfo_x3c3c
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3c30:
    /*00003c30*/ ADD         R2, R2, #1 
    /*00003c34*/ CMP         R0, R2 
    /*00003c38*/ BEQ         VEDU_DEC_HAL_H264_UpdateRefInfo_x3c74
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3c3c:
    /*00003c3c*/ CMP         R1, #1 
    /*00003c40*/ BGT         VEDU_DEC_HAL_H264_UpdateRefInfo_x3c30
    /*00003c44*/ LDRB        R4, [R2] 
    /*00003c48*/ ADD         R5, SP, #16 
    /*00003c4c*/ ADD         R2, R2, #1 
    /*00003c50*/ ADD         R5, R5, R1, LSL #2 
    /*00003c54*/ CMP         R0, R2 
    /*00003c58*/ ADD         R1, R1, #1 
    /*00003c5c*/ ADD         R4, IP, R4, LSL #2 
    /*00003c60*/ LDR         R6, [R4, #120]              @ 0x78 
    /*00003c64*/ LDR         R4, [R4, #248]              @ 0xf8 
    /*00003c68*/ STR         R6, [R5, #-16] 
    /*00003c6c*/ STR         R4, [R5, #-8] 
    /*00003c70*/ BNE         VEDU_DEC_HAL_H264_UpdateRefInfo_x3c3c
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3c74:
    /*00003c74*/ LDRB        R4, [R3, #561]              @ 0x231 
    /*00003c78*/ CMP         R4, #0 
    /*00003c7c*/ BEQ         VEDU_DEC_HAL_H264_UpdateRefInfo_x3be0
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3c80:
    /*00003c80*/ ADD         R3, R3, #852                @ 0x354 
    /*00003c84*/ MOV         R2, #0 
    /*00003c88*/ B           VEDU_DEC_HAL_H264_UpdateRefInfo_x3c98
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3c8c:
    /*00003c8c*/ CMP         R2, R4 
    /*00003c90*/ ADD         R3, R3, #1 
    /*00003c94*/ BGE         VEDU_DEC_HAL_H264_UpdateRefInfo_x3be0
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3c98:
    /*00003c98*/ CMP         R1, #1 
    /*00003c9c*/ ADD         R2, R2, #1 
    /*00003ca0*/ BGT         VEDU_DEC_HAL_H264_UpdateRefInfo_x3c8c
    /*00003ca4*/ LDRB        R0, [R3] 
    /*00003ca8*/ ADD         R5, SP, #16 
    /*00003cac*/ ADD         R5, R5, R1, LSL #2 
    /*00003cb0*/ ADD         R1, R1, #1 
    /*00003cb4*/ ADD         R0, IP, R0, LSL #2 
    /*00003cb8*/ LDR         R6, [R0, #120]              @ 0x78 
    /*00003cbc*/ LDR         R0, [R0, #248]              @ 0xf8 
    /*00003cc0*/ STR         R6, [R5, #-16] 
    /*00003cc4*/ STR         R0, [R5, #-8] 
    /*00003cc8*/ B           VEDU_DEC_HAL_H264_UpdateRefInfo_x3c8c
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3ccc:
    /*00003ccc*/ MOV         R3, R2 
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3cd0:
    /*00003cd0*/ MOV         R5, R3 
    /*00003cd4*/ MOV         R6, R3 
    /*00003cd8*/ MOV         R4, R3 
    /*00003cdc*/ MOV         R1, R3 
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3ce0:
    /*00003ce0*/ MOV         R0, #20 
    /*00003ce4*/ MOVW        IP, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x34] 
    /*00003ce8*/ MUL         R0, R0, LR 
    /*00003cec*/ MOVT        IP, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x34] 
    /*00003cf0*/ ADD         R2, IP, R0 
    /*00003cf4*/ STR         R1, [IP, R0] 
    /*00003cf8*/ STMIB       R2, {R4, R6} 
    /*00003cfc*/ STR         R5, [R2, #12] 
    /*00003d00*/ STR         R3, [R2, #16] 
    /*00003d04*/ ADD         SP, SP, #16 
    /*00003d08*/ POP         {R4, R5, R6, PC} 
    VEDU_DEC_HAL_H264_UpdateRefInfo_x3d0c:
    /*00003d0c*/ LDRB        R4, [R3, #561]              @ 0x231 
    /*00003d10*/ CMP         R4, #0 
    /*00003d14*/ BNE         VEDU_DEC_HAL_H264_UpdateRefInfo_x3c80
    /*00003d18*/ MOV         R1, R4 
    /*00003d1c*/ MOV         R3, R4 
    /*00003d20*/ MOV         R5, R4 
    /*00003d24*/ MOV         R6, R4 
    /*00003d28*/ B           VEDU_DEC_HAL_H264_UpdateRefInfo_x3ce0
FUNC_END VEDU_DEC_HAL_H264_UpdateRefInfo


FUNC_BEGIN VEDU_DEC_HAL_H264_GetBufferCfg
    /*00003d2c*/ PUSH        {R4, LR} 
    /*00003d30*/ SUB         SP, SP, #40                 @ 0x28 
    /*00003d34*/ LDR         LR, [R0, #4] 
    /*00003d38*/ MOV         R3, #0 
    /*00003d3c*/ MOV         R2, #26 
    /*00003d40*/ STR         R3, [SP, #28] 
    /*00003d44*/ STR         R3, [SP, #24] 
    /*00003d48*/ MOV         R0, #2 
    /*00003d4c*/ STR         R3, [SP, #36]               @ 0x24 
    /*00003d50*/ MOV         R4, R1 
    /*00003d54*/ STR         R3, [SP, #20] 
    /*00003d58*/ STR         R2, [SP, #4] 
    /*00003d5c*/ LDR         IP, [LR] 
    /*00003d60*/ LSL         IP, IP, #4 
    /*00003d64*/ STR         IP, [SP, #8] 
    /*00003d68*/ LDR         IP, [LR, #4] 
    /*00003d6c*/ STR         R3, [SP, #16] 
    /*00003d70*/ LSL         R3, IP, #4 
    /*00003d74*/ STR         R3, [SP, #12] 
    /*00003d78*/ BL          0x0000000000003D78          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003d7c*/ MOV         R2, R4 
    /*00003d80*/ ADD         R1, SP, #4 
    /*00003d84*/ LDR         R3, [R0, #124]              @ 0x7c 
    /*00003d88*/ MOV         R0, #96                     @ 0x60 
    /*00003d8c*/ BLX         R3 
    /*00003d90*/ ADD         SP, SP, #40                 @ 0x28 
    /*00003d94*/ POP         {R4, PC} 
FUNC_END VEDU_DEC_HAL_H264_GetBufferCfg


FUNC_BEGIN VEDU_DEC_HAL_H264_GetFrameType
    /*00003d98*/ MOV         R3, #0 
    /*00003d9c*/ LDR         R2, [R0, #8] 
    /*00003da0*/ PUSH        {R4, LR} 
    /*00003da4*/ STR         R3, [R1] 
    /*00003da8*/ LDRH        LR, [R0, #12] 
    /*00003dac*/ CMP         LR, R3 
    /*00003db0*/ POPEQ       {R4, PC} 
    /*00003db4*/ MOV         R4, #1 
    /*00003db8*/ B           VEDU_DEC_HAL_H264_GetFrameType_x3ddc
    VEDU_DEC_HAL_H264_GetFrameType_x3dbc:
    /*00003dbc*/ LDR         IP, [R1] 
    /*00003dc0*/ CMP         IP, #0 
    /*00003dc4*/ STREQ       R4, [R1] 
    /*00003dc8*/ LDRHEQ      LR, [R0, #12] 
    VEDU_DEC_HAL_H264_GetFrameType_x3dcc:
    /*00003dcc*/ ADD         R3, R3, #1 
    /*00003dd0*/ LDR         R2, [R2, #1504]             @ 0x5e0 
    /*00003dd4*/ CMP         LR, R3 
    /*00003dd8*/ BLS         VEDU_DEC_HAL_H264_GetFrameType_x3dfc
    VEDU_DEC_HAL_H264_GetFrameType_x3ddc:
    /*00003ddc*/ LDRB        IP, [R2, #36]               @ 0x24 
    /*00003de0*/ CMP         IP, #0 
    /*00003de4*/ BEQ         VEDU_DEC_HAL_H264_GetFrameType_x3dbc
    /*00003de8*/ CMP         IP, #1 
    /*00003dec*/ BNE         VEDU_DEC_HAL_H264_GetFrameType_x3dcc
    /*00003df0*/ MOV         R3, #2 
    /*00003df4*/ STR         R3, [R1] 
    /*00003df8*/ POP         {R4, PC} 
    VEDU_DEC_HAL_H264_GetFrameType_x3dfc:
    /*00003dfc*/ POP         {R4, PC} 
FUNC_END VEDU_DEC_HAL_H264_GetFrameType


FUNC_BEGIN VEDU_DEC_HAL_H264_WriteSliceMsg
    /*00003e00*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00003e04*/ MOV         R4, #0 
    /*00003e08*/ LDMIB       R0, {R3, R5} 
    /*00003e0c*/ SUB         SP, SP, #12 
    /*00003e10*/ MOV         R8, R0 
    /*00003e14*/ LDR         SL, [R0, #40]               @ 0x28 
    /*00003e18*/ LDRH        FP, [R0, #12] 
    /*00003e1c*/ LDM         R3, {R1, R2} 
    /*00003e20*/ STRB        R4, [R0, #112]              @ 0x70 
    /*00003e24*/ ADD         R2, R2, #1 
    /*00003e28*/ MLA         R2, R1, R2, R2 
    /*00003e2c*/ STR         R2, [R0, #108]              @ 0x6c 
    /*00003e30*/ LDR         R6, [R5, #28] 
    /*00003e34*/ CMP         R6, R4 
    /*00003e38*/ MOVEQ       R7, R6 
    /*00003e3c*/ BNE         VEDU_DEC_HAL_H264_WriteSliceMsg_x4098
    VEDU_DEC_HAL_H264_WriteSliceMsg_x3e40:
    /*00003e40*/ ADD         R2, FP, R6 
    /*00003e44*/ CMP         R2, R6 
    /*00003e48*/ BLS         VEDU_DEC_HAL_H264_WriteSliceMsg_x4080
    /*00003e4c*/ ADD         R9, R7, #1 
    /*00003e50*/ LSL         R9, R9, #8 
    /*00003e54*/ B           VEDU_DEC_HAL_H264_WriteSliceMsg_x3ff0
    VEDU_DEC_HAL_H264_WriteSliceMsg_x3e58:
    /*00003e58*/ LDR         R2, [R5, #24] 
    /*00003e5c*/ LDR         R0, [R3, #40]               @ 0x28 
    /*00003e60*/ BIC         R2, R2, #15 
    /*00003e64*/ LDRB        R1, [R4, #16] 
    /*00003e68*/ BIC         R0, R0, #15 
    /*00003e6c*/ SUB         R2, R2, R0 
    /*00003e70*/ STR         R2, [R4, #12] 
    /*00003e74*/ LDR         R2, [R5, #24] 
    /*00003e78*/ LDRB        R0, [R5, #17] 
    /*00003e7c*/ LSL         R2, R2, #3 
    /*00003e80*/ AND         R2, R2, #120                @ 0x78 
    /*00003e84*/ ADD         R2, R2, R0 
    /*00003e88*/ BFI         R1, R2, #0, #7 
    /*00003e8c*/ STRB        R1, [R4, #16] 
    /*00003e90*/ LDR         R2, [R5, #12] 
    /*00003e94*/ STR         R2, [R4, #20] 
    VEDU_DEC_HAL_H264_WriteSliceMsg_x3e98:
    /*00003e98*/ LDR         R1, [R5, #28] 
    /*00003e9c*/ LDR         R2, [R4, #24] 
    /*00003ea0*/ BFI         R2, R1, #0, #20 
    /*00003ea4*/ STR         R2, [R4, #24] 
    /*00003ea8*/ LDRB        R1, [R5, #37]               @ 0x25 
    /*00003eac*/ LSR         R2, R2, #24 
    /*00003eb0*/ BFI         R2, R1, #0, #2 
    /*00003eb4*/ STRB        R2, [R4, #27] 
    /*00003eb8*/ LDRB        R1, [R5, #42]               @ 0x2a 
    /*00003ebc*/ BFI         R2, R1, #2, #6 
    /*00003ec0*/ STRB        R2, [R4, #27] 
    /*00003ec4*/ LDRB        R2, [R5, #36]               @ 0x24 
    /*00003ec8*/ CMP         R2, #2 
    /*00003ecc*/ BEQ         VEDU_DEC_HAL_H264_WriteSliceMsg_x4070
    /*00003ed0*/ CMP         R2, #0 
    /*00003ed4*/ LDRB        R2, [R4, #28] 
    /*00003ed8*/ MOVEQ       R1, #1 
    /*00003edc*/ MOVNE       R1, #2 
    /*00003ee0*/ BFI         R2, R1, #0, #2 
    /*00003ee4*/ STRB        R2, [R4, #28] 
    VEDU_DEC_HAL_H264_WriteSliceMsg_x3ee8:
    /*00003ee8*/ LDRB        R0, [R5, #560]              @ 0x230 
    /*00003eec*/ LDRB        R1, [R4, #28] 
    /*00003ef0*/ LDRB        R2, [R4, #29] 
    /*00003ef4*/ BFI         R1, R0, #2, #6 
    /*00003ef8*/ STRB        R1, [R4, #28] 
    /*00003efc*/ LDRB        R1, [R5, #561]              @ 0x231 
    /*00003f00*/ LDRB        R0, [R4, #30] 
    /*00003f04*/ BFI         R2, R1, #0, #6 
    /*00003f08*/ STRB        R2, [R4, #29] 
    /*00003f0c*/ LDRB        IP, [R5, #41]               @ 0x29 
    /*00003f10*/ LDRB        R1, [R4, #40]               @ 0x28 
    /*00003f14*/ BFI         R2, IP, #6, #1 
    /*00003f18*/ STRB        R2, [R4, #29] 
    /*00003f1c*/ LDRB        IP, [R5, #40]               @ 0x28 
    /*00003f20*/ BFI         R2, IP, #7, #1 
    /*00003f24*/ STRB        R2, [R4, #29] 
    /*00003f28*/ LDRB        R2, [R5, #38]               @ 0x26 
    /*00003f2c*/ LDRB        IP, [R4, #42]               @ 0x2a 
    /*00003f30*/ BFI         R0, R2, #0, #5 
    /*00003f34*/ STRB        R0, [R4, #30] 
    /*00003f38*/ LDRB        R0, [R5, #39]               @ 0x27 
    /*00003f3c*/ LDRH        R2, [R4, #30] 
    /*00003f40*/ BFI         R2, R0, #5, #5 
    /*00003f44*/ STRH        R2, [R4, #30] 
    /*00003f48*/ LDRB        R2, [R5, #44]               @ 0x2c 
    /*00003f4c*/ LDRB        R0, [R4, #44]               @ 0x2c 
    /*00003f50*/ BFI         R1, R2, #0, #5 
    /*00003f54*/ STRB        R1, [R4, #40]               @ 0x28 
    /*00003f58*/ LDRB        R1, [R5, #43]               @ 0x2b 
    /*00003f5c*/ LDRH        R2, [R4, #40]               @ 0x28 
    /*00003f60*/ BFI         R2, R1, #5, #5 
    /*00003f64*/ STRH        R2, [R4, #40]               @ 0x28 
    /*00003f68*/ LDRB        R1, [R5, #924]              @ 0x39c 
    /*00003f6c*/ LDRB        R2, [R4, #45]               @ 0x2d 
    /*00003f70*/ BFI         IP, R1, #0, #2 
    /*00003f74*/ STRB        IP, [R4, #42]               @ 0x2a 
    /*00003f78*/ LDRB        IP, [R5, #45]               @ 0x2d 
    /*00003f7c*/ LDRB        R1, [R4, #46]               @ 0x2e 
    /*00003f80*/ BFI         R0, IP, #0, #2 
    /*00003f84*/ STRB        R0, [R4, #44]               @ 0x2c 
    /*00003f88*/ LDRB        R0, [R5, #47]               @ 0x2f 
    /*00003f8c*/ BFI         R2, R0, #0, #4 
    /*00003f90*/ STRB        R2, [R4, #45]               @ 0x2d 
    /*00003f94*/ LDRB        R0, [R5, #46]               @ 0x2e 
    /*00003f98*/ LDR         R2, [R4, #176]              @ 0xb0 
    /*00003f9c*/ BFI         R1, R0, #0, #4 
    /*00003fa0*/ STRB        R1, [R4, #46]               @ 0x2e 
    /*00003fa4*/ LDR         R1, [R5, #32] 
    /*00003fa8*/ BFI         R2, R1, #0, #20 
    /*00003fac*/ STR         R2, [R4, #176]              @ 0xb0 
    /*00003fb0*/ LDRB        R2, [R8, #112]              @ 0x70 
    /*00003fb4*/ ADD         R2, R2, FP 
    /*00003fb8*/ SUB         R1, R2, #1 
    /*00003fbc*/ CMP         R1, R6 
    /*00003fc0*/ BLS         VEDU_DEC_HAL_H264_WriteSliceMsg_x4080
    /*00003fc4*/ LDR         R2, [R8, #32] 
    /*00003fc8*/ ADD         R7, R7, #1 
    /*00003fcc*/ ADD         R2, R2, R9 
    /*00003fd0*/ STR         R2, [R4, #252]              @ 0xfc 
    /*00003fd4*/ LDRB        R2, [R8, #112]              @ 0x70 
    /*00003fd8*/ ADD         R9, R9, #256                @ 0x100 
    /*00003fdc*/ LDR         R5, [R5, #1504]             @ 0x5e0 
    /*00003fe0*/ MOV         R6, R7 
    /*00003fe4*/ ADD         R2, R2, FP 
    /*00003fe8*/ CMP         R2, R7 
    /*00003fec*/ BLS         VEDU_DEC_HAL_H264_WriteSliceMsg_x4080
    VEDU_DEC_HAL_H264_WriteSliceMsg_x3ff0:
    /*00003ff0*/ ADD         R4, SL, R6, LSL #8 
    /*00003ff4*/ MOV         R2, #256                    @ 0x100 
    /*00003ff8*/ MOV         R1, #0 
    /*00003ffc*/ STR         R3, [SP, #4] 
    /*00004000*/ MOV         R0, R4 
    /*00004004*/ BL          0x0000000000004004          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00004008*/ LDR         R3, [SP, #4] 
    /*0000400c*/ LDR         R2, [R5, #20] 
    /*00004010*/ LDR         R1, [R3, #40]               @ 0x28 
    /*00004014*/ BIC         R2, R2, #15 
    /*00004018*/ BIC         R1, R1, #15 
    /*0000401c*/ SUB         R2, R2, R1 
    /*00004020*/ STR         R2, [SL, R6, LSL #8] 
    /*00004024*/ LDR         R2, [R5, #20] 
    /*00004028*/ LDRB        R0, [R5, #16] 
    /*0000402c*/ LDRB        R1, [R4, #4] 
    /*00004030*/ LSL         R2, R2, #3 
    /*00004034*/ AND         R2, R2, #120                @ 0x78 
    /*00004038*/ ADD         R2, R2, R0 
    /*0000403c*/ BFI         R1, R2, #0, #7 
    /*00004040*/ STRB        R1, [R4, #4] 
    /*00004044*/ LDR         R2, [R5, #8] 
    /*00004048*/ STR         R2, [R4, #8] 
    /*0000404c*/ LDR         R2, [R5, #12] 
    /*00004050*/ CMP         R2, #0 
    /*00004054*/ BNE         VEDU_DEC_HAL_H264_WriteSliceMsg_x3e58
    /*00004058*/ LDRB        R1, [R4, #16] 
    /*0000405c*/ STR         R2, [R4, #12] 
    /*00004060*/ BFI         R1, R2, #0, #7 
    /*00004064*/ STR         R2, [R4, #20] 
    /*00004068*/ STRB        R1, [R4, #16] 
    /*0000406c*/ B           VEDU_DEC_HAL_H264_WriteSliceMsg_x3e98
    VEDU_DEC_HAL_H264_WriteSliceMsg_x4070:
    /*00004070*/ LDRB        R2, [R4, #28] 
    /*00004074*/ BFC         R2, #0, #2 
    /*00004078*/ STRB        R2, [R4, #28] 
    /*0000407c*/ B           VEDU_DEC_HAL_H264_WriteSliceMsg_x3ee8
    VEDU_DEC_HAL_H264_WriteSliceMsg_x4080:
    /*00004080*/ LSL         R2, R2, #8 
    /*00004084*/ LDR         R1, [R8, #32] 
    /*00004088*/ LDR         R0, [R8, #40]               @ 0x28 
    /*0000408c*/ ADD         SP, SP, #12 
    /*00004090*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    VEDU_DEC_HAL_H264_WriteSliceMsg_x4094:
    /*00004094*/ B           VEDU_DEC_HAL_H264_WriteSliceMsg_x4094@ osal_flush_dcache_area [R_ARM_JUMP24: +0x0] 
    VEDU_DEC_HAL_H264_WriteSliceMsg_x4098:
    /*00004098*/ MOV         R2, #256                    @ 0x100 
    /*0000409c*/ MOV         R1, R4 
    /*000040a0*/ MOV         R0, SL 
    /*000040a4*/ STR         R3, [SP, #4] 
    /*000040a8*/ BL          0x00000000000040A8          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000040ac*/ LDR         R1, [R5, #20] 
    /*000040b0*/ LDR         R0, [SL, #24] 
    /*000040b4*/ MOV         R2, #1 
    /*000040b8*/ BIC         R1, R1, #15 
    /*000040bc*/ STR         R1, [SL] 
    /*000040c0*/ BFI         R0, R4, #0, #20 
    /*000040c4*/ STR         R0, [SL, #24] 
    /*000040c8*/ LDR         R1, [R5, #28] 
    /*000040cc*/ MOV         R6, R2 
    /*000040d0*/ LDR         R0, [SL, #176]              @ 0xb0 
    /*000040d4*/ MOV         R7, R2 
    /*000040d8*/ SUB         R1, R1, #1 
    /*000040dc*/ STR         R2, [SL, #8] 
    /*000040e0*/ BFI         R0, R1, #0, #20 
    /*000040e4*/ STR         R0, [SL, #176]              @ 0xb0 
    /*000040e8*/ LDR         R1, [R8, #32] 
    /*000040ec*/ LDR         R3, [SP, #4] 
    /*000040f0*/ ADD         R1, R1, #256                @ 0x100 
    /*000040f4*/ STR         R1, [SL, #252]              @ 0xfc 
    /*000040f8*/ STRB        R2, [R8, #112]              @ 0x70 
    /*000040fc*/ B           VEDU_DEC_HAL_H264_WriteSliceMsg_x3e40
FUNC_END VEDU_DEC_HAL_H264_WriteSliceMsg


FUNC_BEGIN VEDU_DEC_HAL_H264_GetSlice
    /*00004100*/ LDR         R1, [R0, #100]              @ 0x64 
    /*00004104*/ LDR         R0, [R0, #8] 
    /*00004108*/ CMP         R1, #0 
    /*0000410c*/ BEQ         VEDU_DEC_HAL_H264_GetSlice_x4190
    /*00004110*/ CMP         R0, #0 
    /*00004114*/ BXEQ        LR 
    /*00004118*/ MOVW        R3, #0                      @ g_u32H264MaxSliceNum [R_ARM_MOVW_ABS_NC: .data+0xfc] 
    /*0000411c*/ MOVT        R3, #0                      @ g_u32H264MaxSliceNum [R_ARM_MOVT_ABS: .data+0xfc] 
    /*00004120*/ LDR         R3, [R3] 
    /*00004124*/ CMP         R3, #0 
    /*00004128*/ SUB         R3, R3, #1 
    /*0000412c*/ BGT         VEDU_DEC_HAL_H264_GetSlice_x4154
    /*00004130*/ B           VEDU_DEC_HAL_H264_GetSlice_x4194
    VEDU_DEC_HAL_H264_GetSlice_x4134:
    /*00004134*/ CMP         R1, #2 
    /*00004138*/ BEQ         VEDU_DEC_HAL_H264_GetSlice_x4170
    VEDU_DEC_HAL_H264_GetSlice_x413c:
    /*0000413c*/ LDR         R2, [R0, #1504]             @ 0x5e0 
    /*00004140*/ CMP         R2, #0 
    /*00004144*/ BEQ         VEDU_DEC_HAL_H264_GetSlice_x4188
    VEDU_DEC_HAL_H264_GetSlice_x4148:
    /*00004148*/ CMN         R3, #1 
    /*0000414c*/ MOV         R0, R2 
    /*00004150*/ BEQ         VEDU_DEC_HAL_H264_GetSlice_x418c
    VEDU_DEC_HAL_H264_GetSlice_x4154:
    /*00004154*/ CMP         R1, #1 
    /*00004158*/ SUB         R3, R3, #1 
    /*0000415c*/ BNE         VEDU_DEC_HAL_H264_GetSlice_x4134
    /*00004160*/ LDRB        R2, [R0, #36]               @ 0x24 
    /*00004164*/ CMP         R2, #0 
    /*00004168*/ BNE         VEDU_DEC_HAL_H264_GetSlice_x413c
    /*0000416c*/ BX          LR 
    VEDU_DEC_HAL_H264_GetSlice_x4170:
    /*00004170*/ LDRB        R2, [R0, #36]               @ 0x24 
    /*00004174*/ CMP         R2, #1 
    /*00004178*/ BXEQ        LR 
    /*0000417c*/ LDR         R2, [R0, #1504]             @ 0x5e0 
    /*00004180*/ CMP         R2, #0 
    /*00004184*/ BNE         VEDU_DEC_HAL_H264_GetSlice_x4148
    VEDU_DEC_HAL_H264_GetSlice_x4188:
    /*00004188*/ BX          LR 
    VEDU_DEC_HAL_H264_GetSlice_x418c:
    /*0000418c*/ BX          LR 
    VEDU_DEC_HAL_H264_GetSlice_x4190:
    /*00004190*/ BX          LR 
    VEDU_DEC_HAL_H264_GetSlice_x4194:
    /*00004194*/ BX          LR 
FUNC_END VEDU_DEC_HAL_H264_GetSlice


FUNC_BEGIN VEDU_DEC_HAL_H264_ConfigMask
    /*00004198*/ LDR         R3, [R0] 
    /*0000419c*/ ORR         R3, R3, #1 
    /*000041a0*/ BFC         R3, #1, #1 
    /*000041a4*/ BFC         R3, #2, #1 
    /*000041a8*/ BFC         R3, #3, #1 
    /*000041ac*/ BFC         R3, #4, #1 
    /*000041b0*/ BFC         R3, #5, #1 
    /*000041b4*/ BFC         R3, #7, #1 
    /*000041b8*/ ORR         R3, R3, #1792               @ 0x700 
    /*000041bc*/ STR         R3, [R0] 
    /*000041c0*/ BX          LR 
FUNC_END VEDU_DEC_HAL_H264_ConfigMask


FUNC_BEGIN VEDU_DEC_HAL_H264_ConfigReg
    /*000041c4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*000041c8*/ MOV         R6, R0 
    /*000041cc*/ MOV         R4, R1 
    /*000041d0*/ BL          0x00000000000041D0          @ VEDU_DEC_HAL_H264_GetSlice [R_ARM_CALL: .text+0x4100] 
    /*000041d4*/ LDR         R5, [R6, #4] 
    /*000041d8*/ LDR         R8, [R6, #104]              @ 0x68 
    /*000041dc*/ SUBS        R7, R0, #0 
    /*000041e0*/ BEQ         VEDU_DEC_HAL_H264_ConfigReg_x4530
    VEDU_DEC_HAL_H264_ConfigReg_x41e4:
    /*000041e4*/ LDR         R2, [R4] 
    /*000041e8*/ MOV         R0, #1 
    /*000041ec*/ MOV         R3, #0 
    /*000041f0*/ MOV         R1, #20 
    /*000041f4*/ ORR         R2, R2, R0 
    /*000041f8*/ MOVW        IP, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x34] 
    /*000041fc*/ BFC         R2, #1, #1 
    /*00004200*/ MOVT        IP, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x34] 
    /*00004204*/ BFC         R2, #2, #1 
    /*00004208*/ BFC         R2, #3, #1 
    /*0000420c*/ BFC         R2, #4, #1 
    /*00004210*/ BFC         R2, #5, #1 
    /*00004214*/ BFC         R2, #7, #1 
    /*00004218*/ ORR         R2, R2, #1792               @ 0x700 
    /*0000421c*/ STR         R2, [R4] 
    /*00004220*/ STR         R3, [R4, #12] 
    /*00004224*/ LDR         R2, [R4, #16] 
    /*00004228*/ MUL         R1, R1, R8 
    /*0000422c*/ ORR         R2, R2, #3 
    /*00004230*/ BFI         R2, R0, #2, #2 
    /*00004234*/ LDR         R9, [R6, #100]              @ 0x64 
    /*00004238*/ BFI         R2, R3, #4, #1 
    /*0000423c*/ LDRB        LR, [R5, #11] 
    /*00004240*/ BFI         R2, R3, #5, #1 
    /*00004244*/ BFI         R2, R3, #6, #1 
    /*00004248*/ BFI         R2, R3, #7, #1 
    /*0000424c*/ LDR         R1, [IP, R1] 
    /*00004250*/ BFI         R2, R3, #8, #1 
    /*00004254*/ MVN         LR, LR 
    /*00004258*/ BFI         R2, R3, #9, #1 
    /*0000425c*/ SUB         R1, R1, #1 
    /*00004260*/ BFI         R2, R9, #10, #2 
    /*00004264*/ BFI         R2, LR, #12, #1 
    /*00004268*/ MOV         LR, R3 
    /*0000426c*/ BFI         R2, R3, #13, #1 
    /*00004270*/ BFI         R2, R1, #14, #1 
    /*00004274*/ MOV         R1, R3 
    /*00004278*/ BFI         R2, R3, #15, #1 
    /*0000427c*/ BFI         R2, R3, #16, #1 
    /*00004280*/ BFI         R2, R3, #17, #1 
    /*00004284*/ BFI         R2, R3, #18, #2 
    /*00004288*/ BFI         R2, R3, #20, #1 
    /*0000428c*/ BFI         R2, R3, #21, #1 
    /*00004290*/ BFI         R2, R3, #22, #2 
    /*00004294*/ BFI         R2, R3, #30, #1 
    /*00004298*/ BFI         R2, R3, #31, #1 
    /*0000429c*/ STR         R2, [R4, #16] 
    /*000042a0*/ LDR         R9, [R5] 
    /*000042a4*/ LDR         R2, [R5, #4] 
    /*000042a8*/ LSL         R9, R9, #4 
    /*000042ac*/ SUB         R9, R9, #1 
    /*000042b0*/ LSL         R2, R2, #4 
    /*000042b4*/ BFI         LR, R9, #0, #16 
    /*000042b8*/ SUB         R2, R2, #1 
    /*000042bc*/ BFI         LR, R2, #16, #16 
    /*000042c0*/ STR         LR, [R4, #20] 
    /*000042c4*/ LDRB        R9, [R7, #44]               @ 0x2c 
    /*000042c8*/ MOVW        LR, #0                      @ g_u32H264MaxSliceNum [R_ARM_MOVW_ABS_NC: .data+0xfc] 
    /*000042cc*/ LDRB        R2, [R7, #43]               @ 0x2b 
    /*000042d0*/ MOVT        LR, #0                      @ g_u32H264MaxSliceNum [R_ARM_MOVT_ABS: .data+0xfc] 
    /*000042d4*/ BFI         R1, R9, #0, #5 
    /*000042d8*/ BFI         R1, R2, #5, #5 
    /*000042dc*/ STR         R1, [R4, #24] 
    /*000042e0*/ STR         R3, [R4, #576]              @ 0x240 
    /*000042e4*/ MOV         R2, R3 
    /*000042e8*/ LDR         R9, [R4, #28] 
    /*000042ec*/ LDR         R1, [LR] 
    /*000042f0*/ MOV         LR, #8192                   @ 0x2000 
    /*000042f4*/ BFI         R9, R3, #16, #1 
    /*000042f8*/ BFI         R9, R3, #20, #1 
    /*000042fc*/ ADD         R1, R1, #63                 @ 0x3f 
    /*00004300*/ BFI         R9, R3, #21, #1 
    /*00004304*/ STR         R9, [R4, #28] 
    /*00004308*/ LDR         R9, [R6, #108]              @ 0x6c 
    /*0000430c*/ LSR         R1, R1, #6 
    /*00004310*/ SUB         R1, R1, #1 
    /*00004314*/ SUB         R9, R9, #1 
    /*00004318*/ BFI         LR, R1, #16, #12 
    /*0000431c*/ BFI         R2, R9, #0, #18 
    /*00004320*/ BFI         LR, R3, #28, #1 
    /*00004324*/ BFI         R2, R3, #22, #1 
    /*00004328*/ BFI         LR, R3, #29, #1 
    /*0000432c*/ BFI         R2, R3, #23, #1 
    /*00004330*/ MOV         R1, R3 
    /*00004334*/ ORR         R2, R2, #16777216           @ 0x1000000 
    /*00004338*/ BFI         R1, R3, #18, #1 
    /*0000433c*/ BFI         R2, R3, #25, #1 
    /*00004340*/ BFI         R2, R3, #26, #2 
    /*00004344*/ BFI         R2, R3, #30, #1 
    /*00004348*/ STR         R2, [R4, #32] 
    /*0000434c*/ STR         LR, [R4, #36]               @ 0x24 
    /*00004350*/ MOV         LR, R3 
    /*00004354*/ LDRB        R2, [R5, #8] 
    /*00004358*/ BFI         LR, R3, #0, #1 
    /*0000435c*/ BFI         R1, R2, #19, #2 
    /*00004360*/ LDRB        R2, [R5, #15] 
    /*00004364*/ BFI         R1, R3, #21, #1 
    /*00004368*/ BFI         R1, R3, #22, #1 
    /*0000436c*/ BFI         R1, R3, #23, #1 
    /*00004370*/ BFI         R1, R3, #24, #1 
    /*00004374*/ BFI         R1, R2, #26, #1 
    /*00004378*/ STR         R1, [R4, #40]               @ 0x28 
    /*0000437c*/ STR         R3, [R4, #56]               @ 0x38 
    /*00004380*/ MOV         R1, #65536                  @ 0x10000 
    /*00004384*/ STR         R3, [R4, #60]               @ 0x3c 
    /*00004388*/ LDR         R2, [R5, #40]               @ 0x28 
    /*0000438c*/ BIC         R2, R2, #15 
    /*00004390*/ STR         R2, [R4, #64]               @ 0x40 
    /*00004394*/ LDR         R2, [R6, #32] 
    /*00004398*/ STR         R2, [R4, #68]               @ 0x44 
    /*0000439c*/ LDR         R2, [R6, #48]               @ 0x30 
    /*000043a0*/ STR         R2, [R4, #72]               @ 0x48 
    /*000043a4*/ STR         R1, [R4, #204]              @ 0xcc 
    /*000043a8*/ STR         LR, [R4, #520]              @ 0x208 
    /*000043ac*/ LDR         R2, [R5, #48]               @ 0x30 
    /*000043b0*/ CMP         R2, R3 
    /*000043b4*/ MOVEQ       R3, R2 
    /*000043b8*/ BEQ         VEDU_DEC_HAL_H264_ConfigReg_x43cc
    /*000043bc*/ LDR         R2, [R6, #100]              @ 0x64 
    /*000043c0*/ CMP         R2, #2 
    /*000043c4*/ BFIEQ       R3, R2, #0, #2 
    /*000043c8*/ BFINE       R3, R0, #0, #2 
    VEDU_DEC_HAL_H264_ConfigReg_x43cc:
    /*000043cc*/ STR         R3, [R4, #524]              @ 0x20c 
    /*000043d0*/ MOV         R2, #20 
    /*000043d4*/ LDR         R1, [R5, #24] 
    /*000043d8*/ MLA         R8, R2, R8, IP 
    /*000043dc*/ MOV         R2, #0 
    /*000043e0*/ STR         R1, [R4, #528]              @ 0x210 
    /*000043e4*/ MOV         R9, R2 
    /*000043e8*/ LDRB        R1, [R7, #47]               @ 0x2f 
    /*000043ec*/ MOV         R3, R2 
    /*000043f0*/ MOV         LR, R2 
    /*000043f4*/ BFI         R3, R2, #4, #1 
    /*000043f8*/ BFI         R2, R1, #0, #4 
    /*000043fc*/ LDRB        R1, [R7, #46]               @ 0x2e 
    /*00004400*/ MOV         IP, R9 
    /*00004404*/ MOV         R0, R9 
    /*00004408*/ BFI         R2, R1, #4, #4 
    /*0000440c*/ LDRB        R1, [R7, #45]               @ 0x2d 
    /*00004410*/ BFI         R2, R1, #8, #2 
    /*00004414*/ STR         R2, [R4, #564]              @ 0x234 
    /*00004418*/ LDR         R2, [R8, #12] 
    /*0000441c*/ LDRB        R1, [R5, #14] 
    /*00004420*/ BFI         R3, R1, #0, #1 
    /*00004424*/ STR         R3, [R4, #568]              @ 0x238 
    /*00004428*/ STR         R2, [R4, #3192]             @ 0xc78 
    /*0000442c*/ LDR         R1, [R6, #88]               @ 0x58 
    /*00004430*/ LDR         R3, [R8, #16] 
    /*00004434*/ ADD         R1, R2, R1 
    /*00004438*/ STR         R1, [R4, #3196]             @ 0xc7c 
    /*0000443c*/ STR         R2, [R4, #3200]             @ 0xc80 
    /*00004440*/ LDR         R1, [R6, #88]               @ 0x58 
    /*00004444*/ ADD         R2, R2, R1 
    /*00004448*/ STR         R2, [R4, #3204]             @ 0xc84 
    /*0000444c*/ STR         R3, [R4, #3208]             @ 0xc88 
    /*00004450*/ MOV         R1, R9 
    /*00004454*/ LDR         R2, [R6, #88]               @ 0x58 
    /*00004458*/ ADD         R2, R3, R2 
    /*0000445c*/ STR         R2, [R4, #3212]             @ 0xc8c 
    /*00004460*/ STR         R3, [R4, #3216]             @ 0xc90 
    /*00004464*/ LDR         R2, [R6, #88]               @ 0x58 
    /*00004468*/ ADD         R3, R3, R2 
    /*0000446c*/ STR         R3, [R4, #3220]             @ 0xc94 
    /*00004470*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*00004474*/ STR         R3, [R4, #3224]             @ 0xc98 
    /*00004478*/ LDR         R2, [R6, #88]               @ 0x58 
    /*0000447c*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*00004480*/ ADD         R3, R3, R2 
    /*00004484*/ STR         R3, [R4, #3228]             @ 0xc9c 
    /*00004488*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*0000448c*/ STR         R3, [R4, #3232]             @ 0xca0 
    /*00004490*/ LDR         R2, [R6, #88]               @ 0x58 
    /*00004494*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*00004498*/ ADD         R3, R3, R2 
    /*0000449c*/ STR         R3, [R4, #3236]             @ 0xca4 
    /*000044a0*/ LDR         R3, [R5, #4] 
    /*000044a4*/ MOV         R2, R9 
    /*000044a8*/ LSL         R3, R3, #4 
    /*000044ac*/ BFI         R9, R3, #0, #16 
    /*000044b0*/ LSR         R3, R3, #1 
    /*000044b4*/ BFI         R9, R3, #16, #16 
    /*000044b8*/ STR         R9, [R4, #3240]             @ 0xca8 
    /*000044bc*/ LDR         R3, [R5, #4] 
    /*000044c0*/ LSL         R3, R3, #4 
    /*000044c4*/ BFI         LR, R3, #0, #16 
    /*000044c8*/ LSR         R3, R3, #1 
    /*000044cc*/ BFI         LR, R3, #16, #16 
    /*000044d0*/ STR         LR, [R4, #3244]             @ 0xcac 
    /*000044d4*/ LDR         R3, [R5, #4] 
    /*000044d8*/ LSL         R3, R3, #4 
    /*000044dc*/ BFI         IP, R3, #0, #16 
    /*000044e0*/ LSR         R3, R3, #1 
    /*000044e4*/ BFI         IP, R3, #16, #16 
    /*000044e8*/ STR         IP, [R4, #3248]             @ 0xcb0 
    /*000044ec*/ LDRH        R3, [R6, #80]               @ 0x50 
    /*000044f0*/ BFI         R0, R3, #0, #16 
    /*000044f4*/ BFI         R0, R3, #16, #16 
    /*000044f8*/ STR         R0, [R4, #3168]             @ 0xc60 
    /*000044fc*/ LDR         R3, [R5, #48]               @ 0x30 
    /*00004500*/ STR         R3, [R4, #3172]             @ 0xc64 
    /*00004504*/ LDR         R3, [R7, #920]              @ 0x398 
    /*00004508*/ STR         R3, [R4, #3176]             @ 0xc68 
    /*0000450c*/ LDRH        R3, [R6, #80]               @ 0x50 
    /*00004510*/ BFI         R1, R3, #0, #16 
    /*00004514*/ BFI         R1, R3, #16, #16 
    /*00004518*/ STR         R1, [R4, #3184]             @ 0xc70 
    /*0000451c*/ LDRH        R3, [R6, #80]               @ 0x50 
    /*00004520*/ BFI         R2, R3, #0, #16 
    /*00004524*/ BFI         R2, R3, #16, #16 
    /*00004528*/ STR         R2, [R4, #3188]             @ 0xc74 
    /*0000452c*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    VEDU_DEC_HAL_H264_ConfigReg_x4530:
    /*00004530*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x87c] 
    /*00004534*/ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x684] 
    /*00004538*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x87c] 
    /*0000453c*/ MOVT        R1, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x684] 
    /*00004540*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x898] 
    /*00004544*/ MOVW        R2, #375                    @ 0x177 
    /*00004548*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x898] 
    /*0000454c*/ BL          0x000000000000454C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004550*/ B           VEDU_DEC_HAL_H264_ConfigReg_x41e4
FUNC_END VEDU_DEC_HAL_H264_ConfigReg


FUNC_BEGIN VEDU_DEC_HAL_H264_ReadRegs
    /*00004554*/ ADD         R1, R1, #12288              @ 0x3000 
    /*00004558*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000455c*/ SUB         SP, SP, #12 
    /*00004560*/ LDR         R3, [R1, #4] 
    /*00004564*/ TST         R3, #256                    @ 0x100 
    /*00004568*/ BNE         VEDU_DEC_HAL_H264_ReadRegs_x4594
    /*0000456c*/ LDR         R3, [R1, #4] 
    /*00004570*/ TST         R3, #512                    @ 0x200 
    /*00004574*/ BNE         VEDU_DEC_HAL_H264_ReadRegs_x45f4
    /*00004578*/ LDR         R3, [R1, #4] 
    /*0000457c*/ UBFX        R3, R3, #6, #1 
    /*00004580*/ CMP         R3, #0 
    /*00004584*/ STREQ       R3, [R0, #4] 
    /*00004588*/ BNE         VEDU_DEC_HAL_H264_ReadRegs_x45c4
    /*0000458c*/ ADD         SP, SP, #12 
    /*00004590*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    VEDU_DEC_HAL_H264_ReadRegs_x4594:
    /*00004594*/ MOV         R2, #1 
    /*00004598*/ MOVW        R3, #854                    @ 0x356 
    /*0000459c*/ STR         R2, [R0, #4] 
    /*000045a0*/ MOV         R1, #25 
    /*000045a4*/ STR         R3, [SP] 
    /*000045a8*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8d8] 
    /*000045ac*/ LDR         R3, [PC, #112]              @ 0x0000000000004624 
    /*000045b0*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8d8] 
    /*000045b4*/ MOV         R0, #3 
    /*000045b8*/ BL          0x00000000000045B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000045bc*/ ADD         SP, SP, #12 
    /*000045c0*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    VEDU_DEC_HAL_H264_ReadRegs_x45c4:
    /*000045c4*/ MOV         R3, #3 
    /*000045c8*/ MOV         R2, #864                    @ 0x360 
    /*000045cc*/ STR         R3, [R0, #4] 
    /*000045d0*/ MOV         R1, #25 
    /*000045d4*/ STR         R2, [SP] 
    /*000045d8*/ MOV         R0, R3 
    /*000045dc*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x954] 
    /*000045e0*/ LDR         R3, [PC, #60]               @ 0x0000000000004624 
    /*000045e4*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x954] 
    /*000045e8*/ BL          0x00000000000045E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000045ec*/ ADD         SP, SP, #12 
    /*000045f0*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    VEDU_DEC_HAL_H264_ReadRegs_x45f4:
    /*000045f4*/ MOV         R2, #2 
    /*000045f8*/ MOVW        R3, #859                    @ 0x35b 
    /*000045fc*/ STR         R2, [R0, #4] 
    /*00004600*/ MOV         R1, #25 
    /*00004604*/ STR         R3, [SP] 
    /*00004608*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x910] 
    /*0000460c*/ LDR         R3, [PC, #16]               @ 0x0000000000004624 
    /*00004610*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x910] 
    /*00004614*/ MOV         R0, #3 
    /*00004618*/ BL          0x0000000000004618          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000461c*/ ADD         SP, SP, #12 
    /*00004620*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00004624*/ ANDEQ       R0, R0, R0, LSR #13 
FUNC_END VEDU_DEC_HAL_H264_ReadRegs


FUNC_BEGIN VEDU_DEC_HAL_H264_ReadUpMsg
    /*00004628*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000462c*/ LDRH        LR, [R1, #12] 
    /*00004630*/ LDRB        R3, [R1, #112]              @ 0x70 
    /*00004634*/ LDR         IP, [R1, #56]               @ 0x38 
    /*00004638*/ ADDS        LR, LR, R3 
    /*0000463c*/ BEQ         VEDU_DEC_HAL_H264_ReadUpMsg_x46f8
    /*00004640*/ LDR         R3, [IP, #4] 
    /*00004644*/ LDR         R1, [R1, #108]              @ 0x6c 
    /*00004648*/ UBFX        R3, R3, #0, #20 
    /*0000464c*/ CMP         R1, R3 
    /*00004650*/ BCC         VEDU_DEC_HAL_H264_ReadUpMsg_x46cc
    /*00004654*/ LDR         R2, [IP, #8] 
    /*00004658*/ UBFX        R2, R2, #0, #20 
    /*0000465c*/ CMP         R2, R1 
    /*00004660*/ BHI         VEDU_DEC_HAL_H264_ReadUpMsg_x46cc
    /*00004664*/ CMP         R3, R2 
    /*00004668*/ ADDLE       IP, IP, #16 
    /*0000466c*/ MOVLE       R5, #0 
    /*00004670*/ MOVLE       R4, R5 
    /*00004674*/ BLE         VEDU_DEC_HAL_H264_ReadUpMsg_x46a8
    /*00004678*/ B           VEDU_DEC_HAL_H264_ReadUpMsg_x46cc
    VEDU_DEC_HAL_H264_ReadUpMsg_x467c:
    /*0000467c*/ LDR         R3, [IP, #4] 
    /*00004680*/ UBFX        R3, R3, #0, #20 
    /*00004684*/ CMP         R3, R1 
    /*00004688*/ BHI         VEDU_DEC_HAL_H264_ReadUpMsg_x46cc
    /*0000468c*/ LDR         R2, [IP, #8] 
    /*00004690*/ ADD         IP, IP, #16 
    /*00004694*/ UBFX        R2, R2, #0, #20 
    /*00004698*/ CMP         R2, R1 
    /*0000469c*/ BHI         VEDU_DEC_HAL_H264_ReadUpMsg_x46cc
    /*000046a0*/ CMP         R3, R2 
    /*000046a4*/ BGT         VEDU_DEC_HAL_H264_ReadUpMsg_x46cc
    VEDU_DEC_HAL_H264_ReadUpMsg_x46a8:
    /*000046a8*/ CMP         R3, R2 
    /*000046ac*/ ADD         R4, R4, #1 
    /*000046b0*/ SUB         R7, R2, R3 
    /*000046b4*/ ADD         R6, R5, #1 
    /*000046b8*/ ADDLT       R5, R7, R6 
    /*000046bc*/ CMP         LR, R4 
    /*000046c0*/ BNE         VEDU_DEC_HAL_H264_ReadUpMsg_x467c
    /*000046c4*/ CMP         R5, R1 
    /*000046c8*/ BLS         VEDU_DEC_HAL_H264_ReadUpMsg_x46d8
    VEDU_DEC_HAL_H264_ReadUpMsg_x46cc:
    /*000046cc*/ MOV         R3, #100                    @ 0x64 
    /*000046d0*/ STR         R3, [R0, #8] 
    /*000046d4*/ POP         {R4, R5, R6, R7, R8, PC} 
    VEDU_DEC_HAL_H264_ReadUpMsg_x46d8:
    /*000046d8*/ MOV         LR, #100                    @ 0x64 
    /*000046dc*/ MUL         LR, LR, R5 
    VEDU_DEC_HAL_H264_ReadUpMsg_x46e0:
    /*000046e0*/ MOV         R4, R0 
    /*000046e4*/ MOV         R0, LR 
    /*000046e8*/ BL          0x00000000000046E8          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000046ec*/ RSB         R0, R0, #100                @ 0x64 
    /*000046f0*/ STR         R0, [R4, #8] 
    /*000046f4*/ POP         {R4, R5, R6, R7, R8, PC} 
    VEDU_DEC_HAL_H264_ReadUpMsg_x46f8:
    /*000046f8*/ LDR         R1, [R1, #108]              @ 0x6c 
    /*000046fc*/ B           VEDU_DEC_HAL_H264_ReadUpMsg_x46e0
FUNC_END VEDU_DEC_HAL_H264_ReadUpMsg


FUNC_BEGIN VEDU_DEC_HAL_H265_UpdateRefInfo
    /*00004700*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00004704*/ SUB         SP, SP, #28 
    /*00004708*/ LDR         R3, [R0, #8] 
    /*0000470c*/ LDR         R2, [R0, #104]              @ 0x68 
    /*00004710*/ LDR         R4, [R0, #4] 
    /*00004714*/ MOV         R1, R3 
    /*00004718*/ STR         R3, [SP, #4] 
    /*0000471c*/ MOV         R3, #0 
    /*00004720*/ CMP         R1, R3 
    /*00004724*/ STR         R2, [SP] 
    /*00004728*/ STR         R3, [SP, #12] 
    /*0000472c*/ STR         R3, [SP, #8] 
    /*00004730*/ STR         R3, [SP, #16] 
    /*00004734*/ STR         R3, [SP, #20] 
    /*00004738*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x4b68
    /*0000473c*/ LDR         R2, [R0, #100]              @ 0x64 
    /*00004740*/ MOV         R3, R1 
    /*00004744*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x475c
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4748:
    /*00004748*/ CMP         R2, #2 
    /*0000474c*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x47f4
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4750:
    /*00004750*/ LDR         R3, [R3, #564]              @ 0x234 
    /*00004754*/ CMP         R3, #0 
    /*00004758*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x48bc
    VEDU_DEC_HAL_H265_UpdateRefInfo_x475c:
    /*0000475c*/ CMP         R2, #1 
    /*00004760*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x4748
    /*00004764*/ LDRB        R1, [R3, #53]               @ 0x35 
    /*00004768*/ CMP         R1, #1 
    /*0000476c*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x4750
    /*00004770*/ LDRB        R2, [R3, #76]               @ 0x4c 
    /*00004774*/ CMP         R2, #0 
    /*00004778*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x48b8
    /*0000477c*/ ADD         R1, R2, #240                @ 0xf0 
    /*00004780*/ ADD         R2, R3, #240                @ 0xf0 
    /*00004784*/ ADD         R3, R3, R1 
    /*00004788*/ MOV         R1, #0 
    /*0000478c*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x479c
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4790:
    /*00004790*/ ADD         R2, R2, #1 
    /*00004794*/ CMP         R2, R3 
    /*00004798*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x47d4
    VEDU_DEC_HAL_H265_UpdateRefInfo_x479c:
    /*0000479c*/ CMP         R1, #1 
    /*000047a0*/ BGT         VEDU_DEC_HAL_H265_UpdateRefInfo_x4790
    /*000047a4*/ LDRB        R0, [R2] 
    /*000047a8*/ ADD         IP, SP, #24 
    /*000047ac*/ ADD         R2, R2, #1 
    /*000047b0*/ ADD         IP, IP, R1, LSL #2 
    /*000047b4*/ CMP         R2, R3 
    /*000047b8*/ ADD         R1, R1, #1 
    /*000047bc*/ ADD         R0, R4, R0, LSL #2 
    /*000047c0*/ LDR         LR, [R0, #144]              @ 0x90 
    /*000047c4*/ LDR         R0, [R0, #212]              @ 0xd4 
    /*000047c8*/ STR         LR, [IP, #-16] 
    /*000047cc*/ STR         R0, [IP, #-8] 
    /*000047d0*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x479c
    VEDU_DEC_HAL_H265_UpdateRefInfo_x47d4:
    /*000047d4*/ CMP         R1, #1 
    /*000047d8*/ LDREQ       R3, [SP, #8] 
    /*000047dc*/ LDREQ       R0, [SP, #16] 
    /*000047e0*/ LDRNE       R0, [SP, #16] 
    /*000047e4*/ STREQ       R3, [SP, #12] 
    /*000047e8*/ LDRNE       R3, [SP, #20] 
    /*000047ec*/ MOVEQ       R3, R0 
    /*000047f0*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x48c4
    VEDU_DEC_HAL_H265_UpdateRefInfo_x47f4:
    /*000047f4*/ LDRB        R1, [R3, #53]               @ 0x35 
    /*000047f8*/ CMP         R1, #0 
    /*000047fc*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x4750
    /*00004800*/ LDRB        R2, [R3, #76]               @ 0x4c 
    /*00004804*/ CMP         R2, #0 
    /*00004808*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x4b78
    /*0000480c*/ ADD         R0, R2, #240                @ 0xf0 
    /*00004810*/ ADD         R2, R3, #240                @ 0xf0 
    /*00004814*/ ADD         R0, R3, R0 
    /*00004818*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x4828
    VEDU_DEC_HAL_H265_UpdateRefInfo_x481c:
    /*0000481c*/ ADD         R2, R2, #1 
    /*00004820*/ CMP         R2, R0 
    /*00004824*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x4860
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4828:
    /*00004828*/ CMP         R1, #1 
    /*0000482c*/ BGT         VEDU_DEC_HAL_H265_UpdateRefInfo_x481c
    /*00004830*/ LDRB        IP, [R2] 
    /*00004834*/ ADD         LR, SP, #24 
    /*00004838*/ ADD         R2, R2, #1 
    /*0000483c*/ ADD         LR, LR, R1, LSL #2 
    /*00004840*/ CMP         R2, R0 
    /*00004844*/ ADD         R1, R1, #1 
    /*00004848*/ ADD         IP, R4, IP, LSL #2 
    /*0000484c*/ LDR         R5, [IP, #144]              @ 0x90 
    /*00004850*/ LDR         IP, [IP, #212]              @ 0xd4 
    /*00004854*/ STR         R5, [LR, #-16] 
    /*00004858*/ STR         IP, [LR, #-8] 
    /*0000485c*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x4828
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4860:
    /*00004860*/ LDRB        R0, [R3, #77]               @ 0x4d 
    /*00004864*/ CMP         R0, #0 
    /*00004868*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x47d4
    VEDU_DEC_HAL_H265_UpdateRefInfo_x486c:
    /*0000486c*/ ADD         R3, R3, #256                @ 0x100 
    /*00004870*/ MOV         R2, #0 
    /*00004874*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x4884
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4878:
    /*00004878*/ CMP         R2, R0 
    /*0000487c*/ ADD         R3, R3, #1 
    /*00004880*/ BGE         VEDU_DEC_HAL_H265_UpdateRefInfo_x47d4
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4884:
    /*00004884*/ CMP         R1, #1 
    /*00004888*/ ADD         R2, R2, #1 
    /*0000488c*/ BGT         VEDU_DEC_HAL_H265_UpdateRefInfo_x4878
    /*00004890*/ LDRB        IP, [R3] 
    /*00004894*/ ADD         LR, SP, #24 
    /*00004898*/ ADD         LR, LR, R1, LSL #2 
    /*0000489c*/ ADD         R1, R1, #1 
    /*000048a0*/ ADD         IP, R4, IP, LSL #2 
    /*000048a4*/ LDR         R5, [IP, #144]              @ 0x90 
    /*000048a8*/ LDR         IP, [IP, #212]              @ 0xd4 
    /*000048ac*/ STR         R5, [LR, #-16] 
    /*000048b0*/ STR         IP, [LR, #-8] 
    /*000048b4*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x4878
    VEDU_DEC_HAL_H265_UpdateRefInfo_x48b8:
    /*000048b8*/ MOV         R3, R2 
    VEDU_DEC_HAL_H265_UpdateRefInfo_x48bc:
    /*000048bc*/ MOV         R0, R3 
    /*000048c0*/ MOV         R1, R3 
    VEDU_DEC_HAL_H265_UpdateRefInfo_x48c4:
    /*000048c4*/ LDR         IP, [SP] 
    /*000048c8*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x174] 
    /*000048cc*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x174] 
    /*000048d0*/ MOV         R2, #244                    @ 0xf4 
    /*000048d4*/ MLA         R2, R2, IP, R5 
    /*000048d8*/ STR         R1, [R2, #8] 
    /*000048dc*/ STR         R0, [R2, #12] 
    /*000048e0*/ STR         R3, [R2, #16] 
    /*000048e4*/ LDRH        IP, [R4, #208]              @ 0xd0 
    /*000048e8*/ CMP         IP, #15 
    /*000048ec*/ BHI         VEDU_DEC_HAL_H265_UpdateRefInfo_x4b38
    VEDU_DEC_HAL_H265_UpdateRefInfo_x48f0:
    /*000048f0*/ LDR         R2, [SP] 
    /*000048f4*/ MOV         R3, #244                    @ 0xf4 
    /*000048f8*/ MOV         FP, #0 
    /*000048fc*/ MUL         R3, R3, R2 
    /*00004900*/ ADD         R6, R3, #20 
    /*00004904*/ ADD         R8, R3, #40                 @ 0x28 
    /*00004908*/ ADD         R6, R5, R6 
    /*0000490c*/ ADD         R8, R5, R8 
    /*00004910*/ ADD         SL, R3, #37                 @ 0x25 
    /*00004914*/ ADD         SL, R5, SL 
    /*00004918*/ MOV         LR, R6 
    /*0000491c*/ MOV         R9, R8 
    /*00004920*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x4934
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4924:
    /*00004924*/ ADD         LR, LR, #1 
    /*00004928*/ ADD         R9, R9, #4 
    /*0000492c*/ CMP         LR, SL 
    /*00004930*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x4994
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4934:
    /*00004934*/ LDRSB       R2, [LR] 
    /*00004938*/ CMP         R2, #1 
    /*0000493c*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x4924
    /*00004940*/ CMP         IP, #0 
    /*00004944*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x497c
    /*00004948*/ LDR         R2, [R4, #144]              @ 0x90 
    /*0000494c*/ LDR         R7, [R9] 
    /*00004950*/ CMP         R2, R7 
    /*00004954*/ ADDNE       R1, R4, #144                @ 0x90 
    /*00004958*/ MOVNE       R2, #0 
    /*0000495c*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x4970
    /*00004960*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x4924
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4964:
    /*00004964*/ LDR         R0, [R1, #4]! 
    /*00004968*/ CMP         R7, R0 
    /*0000496c*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x4924
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4970:
    /*00004970*/ ADD         R2, R2, #1 
    /*00004974*/ CMP         R2, IP 
    /*00004978*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x4964
    VEDU_DEC_HAL_H265_UpdateRefInfo_x497c:
    /*0000497c*/ STRB        FP, [LR] 
    /*00004980*/ ADD         LR, LR, #1 
    /*00004984*/ CMP         LR, SL 
    /*00004988*/ LDRH        IP, [R4, #208]              @ 0xd0 
    /*0000498c*/ ADD         R9, R9, #4 
    /*00004990*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x4934
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4994:
    /*00004994*/ CMP         IP, #0 
    /*00004998*/ BLE         VEDU_DEC_HAL_H265_UpdateRefInfo_x4b60
    /*0000499c*/ LDR         R2, [SP] 
    /*000049a0*/ MOV         SL, #244                    @ 0xf4 
    /*000049a4*/ MOV         FP, #61                     @ 0x3d 
    /*000049a8*/ ADD         R3, R3, #36                 @ 0x24 
    /*000049ac*/ ADD         R0, R5, R3 
    /*000049b0*/ SUB         R6, R6, #1 
    /*000049b4*/ MLA         SL, SL, R2, R5 
    /*000049b8*/ ADD         R7, R4, #144                @ 0x90 
    /*000049bc*/ MUL         FP, FP, R2 
    /*000049c0*/ MOV         R9, #0 
    VEDU_DEC_HAL_H265_UpdateRefInfo_x49c4:
    /*000049c4*/ LDR         LR, [R7] 
    /*000049c8*/ LDR         R3, [R4, #56]               @ 0x38 
    /*000049cc*/ CMP         LR, R3 
    /*000049d0*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x4a04
    /*000049d4*/ MOV         R2, R8 
    /*000049d8*/ MOV         R3, R6 
    /*000049dc*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x49ec
    VEDU_DEC_HAL_H265_UpdateRefInfo_x49e0:
    /*000049e0*/ CMP         R3, R0 
    /*000049e4*/ ADD         R2, R2, #4 
    /*000049e8*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x4a8c
    VEDU_DEC_HAL_H265_UpdateRefInfo_x49ec:
    /*000049ec*/ LDRSB       R1, [R3, #1]! 
    /*000049f0*/ CMP         R1, #1 
    /*000049f4*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x49e0
    /*000049f8*/ LDR         R1, [R2] 
    /*000049fc*/ CMP         LR, R1 
    /*00004a00*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x49e0
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4a04:
    /*00004a04*/ ADD         R9, R9, #1 
    /*00004a08*/ ADD         R7, R7, #4 
    /*00004a0c*/ CMP         IP, R9 
    /*00004a10*/ BGT         VEDU_DEC_HAL_H265_UpdateRefInfo_x49c4
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4a14:
    /*00004a14*/ MOV         R3, #0 
    /*00004a18*/ MOV         R2, R6 
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4a1c:
    /*00004a1c*/ LDRSB       R1, [R2, #1]! 
    /*00004a20*/ CMP         R1, #0 
    /*00004a24*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x4aec
    /*00004a28*/ ADD         R3, R3, #1 
    /*00004a2c*/ CMP         R3, #17 
    /*00004a30*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x4a1c
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4a34:
    /*00004a34*/ MOV         R3, #0 
    /*00004a38*/ LDR         R0, [SP, #4] 
    /*00004a3c*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x4a50
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4a40:
    /*00004a40*/ ADD         R3, R3, #1 
    /*00004a44*/ ADD         R8, R8, #4 
    /*00004a48*/ CMP         R3, #17 
    /*00004a4c*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x4a84
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4a50:
    /*00004a50*/ LDRSB       R2, [R6, #1]! 
    /*00004a54*/ CMP         R2, #1 
    /*00004a58*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x4a40
    /*00004a5c*/ LDRB        R2, [R0, #70]               @ 0x46 
    /*00004a60*/ LDR         R1, [R8] 
    /*00004a64*/ ADD         R2, R2, #36                 @ 0x24 
    /*00004a68*/ LDR         R2, [R4, R2, LSL #2] 
    /*00004a6c*/ CMP         R1, R2 
    /*00004a70*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x4a40
    /*00004a74*/ LDR         R1, [SP] 
    /*00004a78*/ MOV         R2, #244                    @ 0xf4 
    /*00004a7c*/ MUL         R2, R2, R1 
    /*00004a80*/ STR         R3, [R5, R2] 
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4a84:
    /*00004a84*/ ADD         SP, SP, #28 
    /*00004a88*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4a8c:
    /*00004a8c*/ MOV         R2, R6 
    /*00004a90*/ MOV         R3, #0 
    /*00004a94*/ LDRSB       R1, [R2, #1]! 
    /*00004a98*/ CMP         R1, #0 
    /*00004a9c*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x4ab8
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4aa0:
    /*00004aa0*/ ADD         R3, R3, #1 
    /*00004aa4*/ CMP         R3, #17 
    /*00004aa8*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x4a04
    /*00004aac*/ LDRSB       R1, [R2, #1]! 
    /*00004ab0*/ CMP         R1, #0 
    /*00004ab4*/ BNE         VEDU_DEC_HAL_H265_UpdateRefInfo_x4aa0
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4ab8:
    /*00004ab8*/ ADD         R2, SL, R3 
    /*00004abc*/ MOV         R1, #1 
    /*00004ac0*/ ADD         R3, FP, R3 
    /*00004ac4*/ ADD         R9, R9, #1 
    /*00004ac8*/ STRB        R1, [R2, #20] 
    /*00004acc*/ ADD         R3, R3, #10 
    /*00004ad0*/ LDR         R2, [R7] 
    /*00004ad4*/ ADD         R7, R7, #4 
    /*00004ad8*/ STR         R2, [R5, R3, LSL #2] 
    /*00004adc*/ LDRH        IP, [R4, #208]              @ 0xd0 
    /*00004ae0*/ CMP         IP, R9 
    /*00004ae4*/ BGT         VEDU_DEC_HAL_H265_UpdateRefInfo_x49c4
    /*00004ae8*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x4a14
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4aec:
    /*00004aec*/ LDR         R0, [SP] 
    /*00004af0*/ MOV         R1, #244                    @ 0xf4 
    /*00004af4*/ MOV         R2, #61                     @ 0x3d 
    /*00004af8*/ MOV         LR, #1 
    /*00004afc*/ LDR         IP, [SP, #8] 
    /*00004b00*/ MLA         R1, R1, R0, R5 
    /*00004b04*/ MLA         R2, R2, R0, R3 
    /*00004b08*/ ADD         R0, R1, R3 
    /*00004b0c*/ STR         R3, [R1, #4] 
    /*00004b10*/ ADD         R1, R2, #10 
    /*00004b14*/ STRB        LR, [R0, #20] 
    /*00004b18*/ LDR         R3, [SP, #12] 
    /*00004b1c*/ ADD         LR, R5, R2, LSL #2 
    /*00004b20*/ LDR         R0, [R4, #56]               @ 0x38 
    /*00004b24*/ ADD         R2, R2, #44                 @ 0x2c 
    /*00004b28*/ STR         IP, [LR, #108]              @ 0x6c 
    /*00004b2c*/ STR         R3, [R5, R2, LSL #2] 
    /*00004b30*/ STR         R0, [R5, R1, LSL #2] 
    /*00004b34*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x4a34
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4b38:
    /*00004b38*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x980] 
    /*00004b3c*/ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x6bc] 
    /*00004b40*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x980] 
    /*00004b44*/ MOVT        R1, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x6bc] 
    /*00004b48*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9a8] 
    /*00004b4c*/ MOV         R2, #143                    @ 0x8f 
    /*00004b50*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9a8] 
    /*00004b54*/ BL          0x0000000000004B54          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004b58*/ LDRH        IP, [R4, #208]              @ 0xd0 
    /*00004b5c*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x48f0
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4b60:
    /*00004b60*/ SUB         R6, R6, #1 
    /*00004b64*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x4a14
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4b68:
    /*00004b68*/ LDR         R1, [SP, #4] 
    /*00004b6c*/ MOV         R3, R1 
    /*00004b70*/ MOV         R0, R1 
    /*00004b74*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x48c4
    VEDU_DEC_HAL_H265_UpdateRefInfo_x4b78:
    /*00004b78*/ LDRB        R1, [R3, #77]               @ 0x4d 
    /*00004b7c*/ CMP         R1, #0 
    /*00004b80*/ MOVEQ       R3, R1 
    /*00004b84*/ MOVEQ       R0, R1 
    /*00004b88*/ BEQ         VEDU_DEC_HAL_H265_UpdateRefInfo_x48c4
    /*00004b8c*/ MOV         R0, R1 
    /*00004b90*/ MOV         R1, R2 
    /*00004b94*/ B           VEDU_DEC_HAL_H265_UpdateRefInfo_x486c
FUNC_END VEDU_DEC_HAL_H265_UpdateRefInfo


FUNC_BEGIN VEDU_DEC_HAL_H265_GetBufferCfg
    /*00004b98*/ PUSH        {R4, LR} 
    /*00004b9c*/ SUB         SP, SP, #40                 @ 0x28 
    /*00004ba0*/ LDR         IP, [R0, #4] 
    /*00004ba4*/ MOV         R3, #0 
    /*00004ba8*/ STR         R3, [SP, #28] 
    /*00004bac*/ MOV         LR, #26 
    /*00004bb0*/ STR         R3, [SP, #24] 
    /*00004bb4*/ MOV         R0, #2 
    /*00004bb8*/ STR         R3, [SP, #36]               @ 0x24 
    /*00004bbc*/ MOV         R4, R1 
    /*00004bc0*/ STR         R3, [SP, #20] 
    /*00004bc4*/ LDR         R2, [IP, #4] 
    /*00004bc8*/ STR         LR, [SP, #4] 
    /*00004bcc*/ STR         R2, [SP, #8] 
    /*00004bd0*/ LDR         R2, [IP] 
    /*00004bd4*/ STR         R3, [SP, #16] 
    /*00004bd8*/ STR         R2, [SP, #12] 
    /*00004bdc*/ BL          0x0000000000004BDC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00004be0*/ MOV         R2, R4 
    /*00004be4*/ ADD         R1, SP, #4 
    /*00004be8*/ LDR         R3, [R0, #124]              @ 0x7c 
    /*00004bec*/ MOVW        R0, #265                    @ 0x109 
    /*00004bf0*/ BLX         R3 
    /*00004bf4*/ ADD         SP, SP, #40                 @ 0x28 
    /*00004bf8*/ POP         {R4, PC} 
FUNC_END VEDU_DEC_HAL_H265_GetBufferCfg


FUNC_BEGIN VEDU_DEC_HAL_H265_GetFrameType
    /*00004bfc*/ MOV         R3, #0 
    /*00004c00*/ LDR         R2, [R0, #8] 
    /*00004c04*/ PUSH        {R4, LR} 
    /*00004c08*/ STR         R3, [R1] 
    /*00004c0c*/ LDRH        LR, [R0, #12] 
    /*00004c10*/ CMP         LR, R3 
    /*00004c14*/ BNE         VEDU_DEC_HAL_H265_GetFrameType_x4c34
    /*00004c18*/ POP         {R4, PC} 
    VEDU_DEC_HAL_H265_GetFrameType_x4c1c:
    /*00004c1c*/ CMP         IP, #0 
    /*00004c20*/ BEQ         VEDU_DEC_HAL_H265_GetFrameType_x4c68
    VEDU_DEC_HAL_H265_GetFrameType_x4c24:
    /*00004c24*/ ADD         R3, R3, #1 
    /*00004c28*/ LDR         R2, [R2, #564]              @ 0x234 
    /*00004c2c*/ CMP         LR, R3 
    /*00004c30*/ BLS         VEDU_DEC_HAL_H265_GetFrameType_x4c64
    VEDU_DEC_HAL_H265_GetFrameType_x4c34:
    /*00004c34*/ LDRB        IP, [R2, #53]               @ 0x35 
    /*00004c38*/ CMP         IP, #1 
    /*00004c3c*/ BNE         VEDU_DEC_HAL_H265_GetFrameType_x4c1c
    /*00004c40*/ LDR         R4, [R1] 
    /*00004c44*/ CMP         R4, #0 
    /*00004c48*/ BNE         VEDU_DEC_HAL_H265_GetFrameType_x4c24
    /*00004c4c*/ STR         IP, [R1] 
    /*00004c50*/ ADD         R3, R3, #1 
    /*00004c54*/ LDRH        LR, [R0, #12] 
    /*00004c58*/ LDR         R2, [R2, #564]              @ 0x234 
    /*00004c5c*/ CMP         LR, R3 
    /*00004c60*/ BHI         VEDU_DEC_HAL_H265_GetFrameType_x4c34
    VEDU_DEC_HAL_H265_GetFrameType_x4c64:
    /*00004c64*/ POP         {R4, PC} 
    VEDU_DEC_HAL_H265_GetFrameType_x4c68:
    /*00004c68*/ MOV         R3, #2 
    /*00004c6c*/ STR         R3, [R1] 
    /*00004c70*/ POP         {R4, PC} 
FUNC_END VEDU_DEC_HAL_H265_GetFrameType


FUNC_BEGIN VEDU_DEC_HAL_H265_WriteSliceMsg
    /*00004c74*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00004c78*/ SUB         SP, SP, #12 
    /*00004c7c*/ LDR         R9, [R0, #4] 
    /*00004c80*/ MOV         R8, R0 
    /*00004c84*/ LDR         R3, [R0, #40]               @ 0x28 
    /*00004c88*/ MOV         R1, #0 
    /*00004c8c*/ LDR         R5, [R0, #8] 
    /*00004c90*/ LDR         R2, [R9, #12] 
    /*00004c94*/ STR         R3, [SP, #4] 
    /*00004c98*/ LDR         R3, [R9, #8] 
    /*00004c9c*/ STRB        R1, [R0, #112]              @ 0x70 
    /*00004ca0*/ ADD         R3, R3, #1 
    /*00004ca4*/ LDRH        R0, [R0, #12] 
    /*00004ca8*/ MLA         R3, R2, R3, R3 
    /*00004cac*/ STR         R0, [SP] 
    /*00004cb0*/ STR         R3, [R8, #108]              @ 0x6c 
    /*00004cb4*/ LDR         R6, [R5, #36]               @ 0x24 
    /*00004cb8*/ CMP         R6, R1 
    /*00004cbc*/ MOVEQ       R7, R6 
    /*00004cc0*/ BNE         VEDU_DEC_HAL_H265_WriteSliceMsg_x500c
    VEDU_DEC_HAL_H265_WriteSliceMsg_x4cc4:
    /*00004cc4*/ LDR         R3, [SP] 
    /*00004cc8*/ ADD         R2, R3, R6 
    /*00004ccc*/ CMP         R2, R6 
    /*00004cd0*/ BLS         VEDU_DEC_HAL_H265_WriteSliceMsg_x4ff4
    /*00004cd4*/ ADD         SL, R7, #1 
    /*00004cd8*/ LSL         SL, SL, #8 
    /*00004cdc*/ B           VEDU_DEC_HAL_H265_WriteSliceMsg_x4f60
    VEDU_DEC_HAL_H265_WriteSliceMsg_x4ce0:
    /*00004ce0*/ LDR         R3, [R5, #24] 
    /*00004ce4*/ LDR         R1, [R9, #68]               @ 0x44 
    /*00004ce8*/ BIC         R3, R3, #15 
    /*00004cec*/ LDRB        R2, [R4, #16] 
    /*00004cf0*/ BIC         R1, R1, #15 
    /*00004cf4*/ SUB         R3, R3, R1 
    /*00004cf8*/ STR         R3, [R4, #12] 
    /*00004cfc*/ LDR         R3, [R5, #24] 
    /*00004d00*/ LDRB        R1, [R5, #17] 
    /*00004d04*/ LSL         R3, R3, #3 
    /*00004d08*/ AND         R3, R3, #120                @ 0x78 
    /*00004d0c*/ ADD         R3, R3, R1 
    /*00004d10*/ BFI         R2, R3, #0, #7 
    /*00004d14*/ STRB        R2, [R4, #16] 
    /*00004d18*/ LDR         R3, [R5, #12] 
    /*00004d1c*/ STR         R3, [R4, #20] 
    /*00004d20*/ LDRB        R3, [R5, #53]               @ 0x35 
    /*00004d24*/ CMP         R3, #2 
    /*00004d28*/ BEQ         VEDU_DEC_HAL_H265_WriteSliceMsg_x4fe4
    VEDU_DEC_HAL_H265_WriteSliceMsg_x4d2c:
    /*00004d2c*/ CMP         R3, #1 
    /*00004d30*/ LDRBEQ      R2, [R4, #28] 
    /*00004d34*/ MOVNE       R2, #2 
    /*00004d38*/ LDRBNE      R3, [R4, #28] 
    /*00004d3c*/ BFIEQ       R2, R3, #0, #2 
    /*00004d40*/ BFINE       R3, R2, #0, #2 
    /*00004d44*/ STRBEQ      R2, [R4, #28] 
    /*00004d48*/ STRBNE      R3, [R4, #28] 
    VEDU_DEC_HAL_H265_WriteSliceMsg_x4d4c:
    /*00004d4c*/ LDRB        R2, [R5, #63]               @ 0x3f 
    /*00004d50*/ LDRB        R3, [R4, #26] 
    /*00004d54*/ LDRB        R0, [R4, #28] 
    /*00004d58*/ BFI         R3, R2, #2, #1 
    /*00004d5c*/ STRB        R3, [R4, #26] 
    /*00004d60*/ LDRB        R1, [R5, #52]               @ 0x34 
    /*00004d64*/ LDRH        R3, [R4, #26] 
    /*00004d68*/ LDRB        R2, [R4, #29] 
    /*00004d6c*/ BFI         R3, R1, #3, #7 
    /*00004d70*/ STRH        R3, [R4, #26] 
    /*00004d74*/ LDRB        R1, [R5, #28] 
    /*00004d78*/ LSR         R3, R3, #8 
    /*00004d7c*/ BFI         R3, R1, #2, #1 
    /*00004d80*/ STRB        R3, [R4, #27] 
    /*00004d84*/ LDRB        R1, [R5, #62]               @ 0x3e 
    /*00004d88*/ BFI         R3, R1, #3, #1 
    /*00004d8c*/ STRB        R3, [R4, #27] 
    /*00004d90*/ LDRB        R3, [R5, #76]               @ 0x4c 
    /*00004d94*/ LDRB        R1, [R4, #30] 
    /*00004d98*/ BFI         R0, R3, #2, #6 
    /*00004d9c*/ STRB        R0, [R4, #28] 
    /*00004da0*/ LDRB        R0, [R5, #77]               @ 0x4d 
    /*00004da4*/ LDRB        R3, [R4, #31] 
    /*00004da8*/ BFI         R2, R0, #0, #6 
    /*00004dac*/ STRB        R2, [R4, #29] 
    /*00004db0*/ LDRB        R0, [R5, #67]               @ 0x43 
    /*00004db4*/ BFI         R2, R0, #6, #1 
    /*00004db8*/ STRB        R2, [R4, #29] 
    /*00004dbc*/ LDRB        R0, [R5, #66]               @ 0x42 
    /*00004dc0*/ BFI         R2, R0, #7, #1 
    /*00004dc4*/ STRB        R2, [R4, #29] 
    /*00004dc8*/ LDRB        R2, [R5, #64]               @ 0x40 
    /*00004dcc*/ BFI         R1, R2, #0, #4 
    /*00004dd0*/ STRB        R1, [R4, #30] 
    /*00004dd4*/ LDRB        R2, [R5, #65]               @ 0x41 
    /*00004dd8*/ BFI         R1, R2, #4, #4 
    /*00004ddc*/ STRB        R1, [R4, #30] 
    /*00004de0*/ LDRB        R2, [R5, #69]               @ 0x45 
    /*00004de4*/ BFI         R3, R2, #0, #3 
    /*00004de8*/ STRB        R3, [R4, #31] 
    /*00004dec*/ LDRB        R2, [R5, #68]               @ 0x44 
    /*00004df0*/ BFI         R3, R2, #3, #1 
    /*00004df4*/ STRB        R3, [R4, #31] 
    /*00004df8*/ LDRB        R2, [R5, #70]               @ 0x46 
    /*00004dfc*/ BFI         R3, R2, #4, #4 
    /*00004e00*/ STRB        R3, [R4, #31] 
    /*00004e04*/ LDR         R3, [R4, #32] 
    /*00004e08*/ LDR         R2, [R5, #36]               @ 0x24 
    /*00004e0c*/ BFI         R3, R2, #0, #18 
    /*00004e10*/ STR         R3, [R4, #32] 
    /*00004e14*/ LDR         R1, [R9, #8] 
    /*00004e18*/ LDR         R0, [R5, #36]               @ 0x24 
    /*00004e1c*/ ADD         R1, R1, #1 
    /*00004e20*/ BL          0x0000000000004E20          @ __aeabi_uidivmod [R_ARM_CALL: +0x0] 
    /*00004e24*/ LDRH        R3, [R4, #36]               @ 0x24 
    /*00004e28*/ BFI         R3, R1, #0, #9 
    /*00004e2c*/ STRH        R3, [R4, #36]               @ 0x24 
    /*00004e30*/ LDR         R1, [R9, #8] 
    /*00004e34*/ LDR         R0, [R5, #36]               @ 0x24 
    /*00004e38*/ ADD         R1, R1, #1 
    /*00004e3c*/ BL          0x0000000000004E3C          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00004e40*/ LDRH        FP, [R4, #38]               @ 0x26 
    /*00004e44*/ LDR         LR, [R4, #40]               @ 0x28 
    /*00004e48*/ LDRB        IP, [R4, #44]               @ 0x2c 
    /*00004e4c*/ LDRB        R1, [R4, #45]               @ 0x2d 
    /*00004e50*/ LDRB        R3, [R4, #48]               @ 0x30 
    /*00004e54*/ LDRB        R2, [R4, #49]               @ 0x31 
    /*00004e58*/ BFI         FP, R0, #0, #9 
    /*00004e5c*/ STRH        FP, [R4, #38]               @ 0x26 
    /*00004e60*/ LDR         R0, [R5, #40]               @ 0x28 
    /*00004e64*/ BFI         LR, R0, #0, #18 
    /*00004e68*/ STR         LR, [R4, #40]               @ 0x28 
    /*00004e6c*/ LDRB        R0, [R5, #55]               @ 0x37 
    /*00004e70*/ BFI         IP, R0, #0, #5 
    /*00004e74*/ STRB        IP, [R4, #44]               @ 0x2c 
    /*00004e78*/ LDRB        R0, [R5, #54]               @ 0x36 
    /*00004e7c*/ BFI         R1, R0, #0, #5 
    /*00004e80*/ STRB        R1, [R4, #45]               @ 0x2d 
    /*00004e84*/ LDRB        R1, [R5, #59]               @ 0x3b 
    /*00004e88*/ BFI         R3, R1, #0, #1 
    /*00004e8c*/ STRB        R3, [R4, #48]               @ 0x30 
    /*00004e90*/ LDRB        R1, [R5, #58]               @ 0x3a 
    /*00004e94*/ BFI         R3, R1, #1, #1 
    /*00004e98*/ STRB        R3, [R4, #48]               @ 0x30 
    /*00004e9c*/ LDRB        R1, [R5, #57]               @ 0x39 
    /*00004ea0*/ BFI         R3, R1, #2, #1 
    /*00004ea4*/ STRB        R3, [R4, #48]               @ 0x30 
    /*00004ea8*/ LDRB        R1, [R5, #56]               @ 0x38 
    /*00004eac*/ BFI         R3, R1, #3, #1 
    /*00004eb0*/ STRB        R3, [R4, #48]               @ 0x30 
    /*00004eb4*/ LDRB        R3, [R5, #60]               @ 0x3c 
    /*00004eb8*/ BFI         R2, R3, #0, #4 
    /*00004ebc*/ STRB        R2, [R4, #49]               @ 0x31 
    /*00004ec0*/ LDRB        R1, [R4, #50]               @ 0x32 
    /*00004ec4*/ LDRB        R2, [R4, #96]               @ 0x60 
    /*00004ec8*/ LDRB        R3, [R4, #97]               @ 0x61 
    /*00004ecc*/ LDRB        R0, [R5, #61]               @ 0x3d 
    /*00004ed0*/ AND         R2, R2, #248                @ 0xf8 
    /*00004ed4*/ LDRB        IP, [R4, #98]               @ 0x62 
    /*00004ed8*/ AND         R3, R3, #192                @ 0xc0 
    /*00004edc*/ BFI         R1, R0, #0, #4 
    /*00004ee0*/ BFC         R2, #3, #1 
    /*00004ee4*/ STRB        R1, [R4, #50]               @ 0x32 
    /*00004ee8*/ BFC         R3, #6, #1 
    /*00004eec*/ STRB        R2, [R4, #96]               @ 0x60 
    /*00004ef0*/ BFC         IP, #0, #5 
    /*00004ef4*/ STRB        R3, [R4, #97]               @ 0x61 
    /*00004ef8*/ STRB        IP, [R4, #98]               @ 0x62 
    /*00004efc*/ LDR         R2, [R4, #172]              @ 0xac 
    /*00004f00*/ LDR         R1, [R5, #44]               @ 0x2c 
    /*00004f04*/ LDR         R3, [R4, #176]              @ 0xb0 
    /*00004f08*/ BFI         R2, R1, #0, #18 
    /*00004f0c*/ STR         R2, [R4, #172]              @ 0xac 
    /*00004f10*/ LDR         R2, [R5, #48]               @ 0x30 
    /*00004f14*/ BFI         R3, R2, #0, #18 
    /*00004f18*/ STR         R3, [R4, #176]              @ 0xb0 
    /*00004f1c*/ LDR         R1, [SP] 
    /*00004f20*/ LDRB        R2, [R8, #112]              @ 0x70 
    /*00004f24*/ ADD         R2, R2, R1 
    /*00004f28*/ SUB         R3, R2, #1 
    /*00004f2c*/ CMP         R3, R6 
    /*00004f30*/ BLS         VEDU_DEC_HAL_H265_WriteSliceMsg_x4ff4
    /*00004f34*/ LDR         R3, [R8, #32] 
    /*00004f38*/ ADD         R7, R7, #1 
    /*00004f3c*/ ADD         R3, R3, SL 
    /*00004f40*/ STR         R3, [R4, #252]              @ 0xfc 
    /*00004f44*/ LDRB        R2, [R8, #112]              @ 0x70 
    /*00004f48*/ ADD         SL, SL, #256                @ 0x100 
    /*00004f4c*/ LDR         R5, [R5, #564]              @ 0x234 
    /*00004f50*/ MOV         R6, R7 
    /*00004f54*/ ADD         R2, R2, R1 
    /*00004f58*/ CMP         R2, R7 
    /*00004f5c*/ BLS         VEDU_DEC_HAL_H265_WriteSliceMsg_x4ff4
    VEDU_DEC_HAL_H265_WriteSliceMsg_x4f60:
    /*00004f60*/ LDR         FP, [SP, #4] 
    /*00004f64*/ MOV         R2, #256                    @ 0x100 
    /*00004f68*/ MOV         R1, #0 
    /*00004f6c*/ ADD         R4, FP, R6, LSL #8 
    /*00004f70*/ MOV         R0, R4 
    /*00004f74*/ BL          0x0000000000004F74          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00004f78*/ LDR         R3, [R5, #20] 
    /*00004f7c*/ LDR         R2, [R9, #68]               @ 0x44 
    /*00004f80*/ BIC         R3, R3, #15 
    /*00004f84*/ BIC         R2, R2, #15 
    /*00004f88*/ SUB         R3, R3, R2 
    /*00004f8c*/ STR         R3, [FP, R6, LSL #8] 
    /*00004f90*/ LDR         R3, [R5, #20] 
    /*00004f94*/ LDRB        R1, [R5, #16] 
    /*00004f98*/ LDRB        R2, [R4, #4] 
    /*00004f9c*/ LSL         R3, R3, #3 
    /*00004fa0*/ AND         R3, R3, #120                @ 0x78 
    /*00004fa4*/ ADD         R3, R3, R1 
    /*00004fa8*/ BFI         R2, R3, #0, #7 
    /*00004fac*/ STRB        R2, [R4, #4] 
    /*00004fb0*/ LDR         R3, [R5, #8] 
    /*00004fb4*/ STR         R3, [R4, #8] 
    /*00004fb8*/ LDR         R3, [R5, #12] 
    /*00004fbc*/ CMP         R3, #0 
    /*00004fc0*/ BNE         VEDU_DEC_HAL_H265_WriteSliceMsg_x4ce0
    /*00004fc4*/ LDRB        R2, [R4, #16] 
    /*00004fc8*/ STR         R3, [R4, #12] 
    /*00004fcc*/ BFI         R2, R3, #0, #7 
    /*00004fd0*/ STR         R3, [R4, #20] 
    /*00004fd4*/ STRB        R2, [R4, #16] 
    /*00004fd8*/ LDRB        R3, [R5, #53]               @ 0x35 
    /*00004fdc*/ CMP         R3, #2 
    /*00004fe0*/ BNE         VEDU_DEC_HAL_H265_WriteSliceMsg_x4d2c
    VEDU_DEC_HAL_H265_WriteSliceMsg_x4fe4:
    /*00004fe4*/ LDRB        R3, [R4, #28] 
    /*00004fe8*/ BFC         R3, #0, #2 
    /*00004fec*/ STRB        R3, [R4, #28] 
    /*00004ff0*/ B           VEDU_DEC_HAL_H265_WriteSliceMsg_x4d4c
    VEDU_DEC_HAL_H265_WriteSliceMsg_x4ff4:
    /*00004ff4*/ LSL         R2, R2, #8 
    /*00004ff8*/ LDR         R1, [R8, #32] 
    /*00004ffc*/ LDR         R0, [R8, #40]               @ 0x28 
    /*00005000*/ ADD         SP, SP, #12 
    /*00005004*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    VEDU_DEC_HAL_H265_WriteSliceMsg_x5008:
    /*00005008*/ B           VEDU_DEC_HAL_H265_WriteSliceMsg_x5008@ osal_flush_dcache_area [R_ARM_JUMP24: +0x0] 
    VEDU_DEC_HAL_H265_WriteSliceMsg_x500c:
    /*0000500c*/ LDR         R4, [SP, #4] 
    /*00005010*/ MOV         R2, #256                    @ 0x100 
    /*00005014*/ MOV         R0, R4 
    /*00005018*/ BL          0x0000000000005018          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000501c*/ LDR         R2, [R5, #20] 
    /*00005020*/ MOV         R3, #1 
    /*00005024*/ STR         R3, [R4, #8] 
    /*00005028*/ BIC         R2, R2, #15 
    /*0000502c*/ STR         R2, [R4] 
    /*00005030*/ LDR         R2, [R8, #32] 
    /*00005034*/ MOV         R6, R3 
    /*00005038*/ LDR         R1, [R4, #176]              @ 0xb0 
    /*0000503c*/ MOV         R7, R3 
    /*00005040*/ ADD         R2, R2, #256                @ 0x100 
    /*00005044*/ STR         R2, [R4, #252]              @ 0xfc 
    /*00005048*/ LDR         R2, [R5, #40]               @ 0x28 
    /*0000504c*/ SUB         R2, R2, #1 
    /*00005050*/ BFI         R1, R2, #0, #18 
    /*00005054*/ STR         R1, [R4, #176]              @ 0xb0 
    /*00005058*/ LDR         R2, [R5, #40]               @ 0x28 
    /*0000505c*/ LDR         R1, [R4, #172]              @ 0xac 
    /*00005060*/ SUB         R2, R2, #1 
    /*00005064*/ BFI         R1, R2, #0, #18 
    /*00005068*/ STR         R1, [R4, #172]              @ 0xac 
    /*0000506c*/ STRB        R3, [R8, #112]              @ 0x70 
    /*00005070*/ B           VEDU_DEC_HAL_H265_WriteSliceMsg_x4cc4
FUNC_END VEDU_DEC_HAL_H265_WriteSliceMsg


FUNC_BEGIN VEDU_DEC_HAL_H265_GetSlice
    /*00005074*/ MOVW        R3, #0                      @ g_u32H265MaxSliceNum [R_ARM_MOVW_ABS_NC: .data+0x100] 
    /*00005078*/ MOVT        R3, #0                      @ g_u32H265MaxSliceNum [R_ARM_MOVT_ABS: .data+0x100] 
    /*0000507c*/ LDR         R2, [R0, #100]              @ 0x64 
    /*00005080*/ LDR         R3, [R3] 
    /*00005084*/ LDR         R0, [R0, #8] 
    /*00005088*/ CMP         R2, #0 
    /*0000508c*/ CMPNE       R3, #0 
    /*00005090*/ BLE         VEDU_DEC_HAL_H265_GetSlice_x50e4
    /*00005094*/ CMP         R0, #0 
    /*00005098*/ BNE         VEDU_DEC_HAL_H265_GetSlice_x50b8
    /*0000509c*/ B           VEDU_DEC_HAL_H265_GetSlice_x50e0
    VEDU_DEC_HAL_H265_GetSlice_x50a0:
    /*000050a0*/ CMP         R2, #2 
    /*000050a4*/ BEQ         VEDU_DEC_HAL_H265_GetSlice_x50d0
    VEDU_DEC_HAL_H265_GetSlice_x50a8:
    /*000050a8*/ LDR         R3, [R0, #564]              @ 0x234 
    /*000050ac*/ CMP         R3, #0 
    /*000050b0*/ BXEQ        LR 
    /*000050b4*/ MOV         R0, R3 
    VEDU_DEC_HAL_H265_GetSlice_x50b8:
    /*000050b8*/ CMP         R2, #1 
    /*000050bc*/ BNE         VEDU_DEC_HAL_H265_GetSlice_x50a0
    /*000050c0*/ LDRB        R3, [R0, #53]               @ 0x35 
    /*000050c4*/ CMP         R3, #1 
    /*000050c8*/ BNE         VEDU_DEC_HAL_H265_GetSlice_x50a8
    /*000050cc*/ BX          LR 
    VEDU_DEC_HAL_H265_GetSlice_x50d0:
    /*000050d0*/ LDRB        R3, [R0, #53]               @ 0x35 
    /*000050d4*/ CMP         R3, #0 
    /*000050d8*/ BNE         VEDU_DEC_HAL_H265_GetSlice_x50a8
    /*000050dc*/ BX          LR 
    VEDU_DEC_HAL_H265_GetSlice_x50e0:
    /*000050e0*/ BX          LR 
    VEDU_DEC_HAL_H265_GetSlice_x50e4:
    /*000050e4*/ BX          LR 
FUNC_END VEDU_DEC_HAL_H265_GetSlice


FUNC_BEGIN VEDU_DEC_HAL_H265_ConfigMask
    /*000050e8*/ LDR         R3, [R0] 
    /*000050ec*/ ORR         R3, R3, #1 
    /*000050f0*/ BFC         R3, #1, #1 
    /*000050f4*/ BFC         R3, #2, #1 
    /*000050f8*/ BFC         R3, #3, #1 
    /*000050fc*/ BFC         R3, #4, #1 
    /*00005100*/ BFC         R3, #5, #1 
    /*00005104*/ BFC         R3, #7, #1 
    /*00005108*/ ORR         R3, R3, #1792               @ 0x700 
    /*0000510c*/ STR         R3, [R0] 
    /*00005110*/ BX          LR 
FUNC_END VEDU_DEC_HAL_H265_ConfigMask


FUNC_BEGIN VEDU_DEC_HAL_H265_ConfigReg
    /*00005114*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00005118*/ MOV         R6, R0 
    /*0000511c*/ MOV         R4, R1 
    /*00005120*/ BL          0x0000000000005120          @ VEDU_DEC_HAL_H265_GetSlice [R_ARM_CALL: .text+0x5074] 
    /*00005124*/ LDR         R5, [R6, #4] 
    /*00005128*/ LDR         R8, [R6, #104]              @ 0x68 
    /*0000512c*/ SUBS        R7, R0, #0 
    /*00005130*/ BEQ         VEDU_DEC_HAL_H265_ConfigReg_x5624
    VEDU_DEC_HAL_H265_ConfigReg_x5134:
    /*00005134*/ LDR         R2, [R4] 
    /*00005138*/ MOV         R3, #0 
    /*0000513c*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x174] 
    /*00005140*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x174] 
    /*00005144*/ ORR         R2, R2, #1 
    /*00005148*/ MOV         R0, #244                    @ 0xf4 
    /*0000514c*/ BFC         R2, #1, #1 
    /*00005150*/ BFC         R2, #2, #1 
    /*00005154*/ BFC         R2, #3, #1 
    /*00005158*/ BFC         R2, #4, #1 
    /*0000515c*/ BFC         R2, #5, #1 
    /*00005160*/ BFC         R2, #7, #1 
    /*00005164*/ ORR         R2, R2, #1792               @ 0x700 
    /*00005168*/ STR         R2, [R4] 
    /*0000516c*/ STR         R3, [R4, #12] 
    /*00005170*/ LDR         R2, [R4, #16] 
    /*00005174*/ LDRB        IP, [R7, #56]               @ 0x38 
    /*00005178*/ ORR         R2, R2, #3 
    /*0000517c*/ BFI         R2, R3, #2, #2 
    /*00005180*/ BFI         R2, R3, #4, #1 
    /*00005184*/ MLA         R0, R0, R8, R1 
    /*00005188*/ BFI         R2, R3, #5, #1 
    /*0000518c*/ BFI         R2, IP, #6, #1 
    /*00005190*/ LDRB        IP, [R7, #57]               @ 0x39 
    /*00005194*/ BFI         R2, IP, #7, #1 
    /*00005198*/ LDR         IP, [R6, #100]              @ 0x64 
    /*0000519c*/ BFI         R2, R3, #8, #1 
    /*000051a0*/ BFI         R2, R3, #9, #1 
    /*000051a4*/ LDR         R0, [R0, #8] 
    /*000051a8*/ BFI         R2, IP, #10, #2 
    /*000051ac*/ LDRB        IP, [R5, #28] 
    /*000051b0*/ BFI         R2, R3, #12, #1 
    /*000051b4*/ SUB         R0, R0, #1 
    /*000051b8*/ BFI         R2, R3, #13, #1 
    /*000051bc*/ BFI         R2, R0, #14, #1 
    /*000051c0*/ MOV         R0, R3 
    /*000051c4*/ BFI         R2, R3, #15, #1 
    /*000051c8*/ BFI         R2, R3, #16, #1 
    /*000051cc*/ BFI         R2, R3, #17, #1 
    /*000051d0*/ BFI         R2, R3, #18, #2 
    /*000051d4*/ BFI         R2, R3, #20, #1 
    /*000051d8*/ BFI         R2, R3, #21, #1 
    /*000051dc*/ BFI         R2, IP, #22, #2 
    /*000051e0*/ MOV         IP, R3 
    /*000051e4*/ BFI         R2, R3, #30, #1 
    /*000051e8*/ BFI         R2, R3, #31, #1 
    /*000051ec*/ STR         R2, [R4, #16] 
    /*000051f0*/ LDM         R5, {R2, LR} 
    /*000051f4*/ SUB         LR, LR, #1 
    /*000051f8*/ SUB         R2, R2, #1 
    /*000051fc*/ BFI         R0, LR, #0, #16 
    /*00005200*/ BFI         R0, R2, #16, #16 
    /*00005204*/ STR         R0, [R4, #20] 
    /*00005208*/ LDRB        LR, [R5, #45]               @ 0x2d 
    /*0000520c*/ MOVW        R0, #0                      @ g_u32H265MaxSliceNum [R_ARM_MOVW_ABS_NC: .data+0x100] 
    /*00005210*/ LDRB        R2, [R5, #44]               @ 0x2c 
    /*00005214*/ MOVT        R0, #0                      @ g_u32H265MaxSliceNum [R_ARM_MOVT_ABS: .data+0x100] 
    /*00005218*/ BFI         IP, LR, #0, #5 
    /*0000521c*/ BFI         IP, R2, #5, #5 
    /*00005220*/ STR         IP, [R4, #24] 
    /*00005224*/ LDR         LR, [R4, #28] 
    /*00005228*/ MOV         R2, R3 
    /*0000522c*/ LDR         R0, [R0] 
    /*00005230*/ MOV         IP, #8192                   @ 0x2000 
    /*00005234*/ BFI         LR, R3, #16, #1 
    /*00005238*/ BFI         LR, R3, #20, #1 
    /*0000523c*/ ADD         R0, R0, #63                 @ 0x3f 
    /*00005240*/ BFI         LR, R3, #21, #1 
    /*00005244*/ STR         LR, [R4, #28] 
    /*00005248*/ LDR         LR, [R6, #108]              @ 0x6c 
    /*0000524c*/ LSR         R0, R0, #6 
    /*00005250*/ SUB         R0, R0, #1 
    /*00005254*/ SUB         LR, LR, #1 
    /*00005258*/ BFI         IP, R0, #16, #12 
    /*0000525c*/ BFI         R2, LR, #0, #18 
    /*00005260*/ BFI         IP, R3, #28, #1 
    /*00005264*/ BFI         R2, R3, #22, #1 
    /*00005268*/ BFI         IP, R3, #29, #1 
    /*0000526c*/ BFI         R2, R3, #23, #1 
    /*00005270*/ MOV         R0, R3 
    /*00005274*/ ORR         R2, R2, #16777216           @ 0x1000000 
    /*00005278*/ BFI         R2, R3, #25, #1 
    /*0000527c*/ BFI         R2, R3, #26, #2 
    /*00005280*/ BFI         R2, R3, #30, #1 
    /*00005284*/ STR         R2, [R4, #32] 
    /*00005288*/ STR         IP, [R4, #36]               @ 0x24 
    /*0000528c*/ MOV         R2, R3 
    /*00005290*/ LDRB        LR, [R5, #31] 
    /*00005294*/ MOV         IP, R3 
    /*00005298*/ BFI         R0, LR, #18, #1 
    /*0000529c*/ LDRB        LR, [R5, #16] 
    /*000052a0*/ BFI         R0, LR, #19, #2 
    /*000052a4*/ LDRB        LR, [R5, #30] 
    /*000052a8*/ BFI         R0, LR, #21, #1 
    /*000052ac*/ LDRB        LR, [R5, #29] 
    /*000052b0*/ BFI         R0, LR, #22, #1 
    /*000052b4*/ LDRB        LR, [R5, #36]               @ 0x24 
    /*000052b8*/ BFI         R0, LR, #23, #1 
    /*000052bc*/ LDRB        LR, [R5, #28] 
    /*000052c0*/ BFI         R0, LR, #24, #1 
    /*000052c4*/ BFI         R0, R3, #26, #1 
    /*000052c8*/ STR         R0, [R4, #40]               @ 0x28 
    /*000052cc*/ LDRB        LR, [R5, #21] 
    /*000052d0*/ MOV         R0, R3 
    /*000052d4*/ BFI         R2, LR, #0, #3 
    /*000052d8*/ LDRB        LR, [R5, #22] 
    /*000052dc*/ BFI         R2, LR, #3, #3 
    /*000052e0*/ LDRB        LR, [R5, #23] 
    /*000052e4*/ BFI         R2, LR, #6, #3 
    /*000052e8*/ LDRB        LR, [R5, #25] 
    /*000052ec*/ BFI         R2, LR, #9, #3 
    /*000052f0*/ LDRB        LR, [R5, #24] 
    /*000052f4*/ BFI         R2, LR, #12, #3 
    /*000052f8*/ LDRB        LR, [R5, #35]               @ 0x23 
    /*000052fc*/ BFI         R2, LR, #15, #3 
    /*00005300*/ LDRB        LR, [R5, #34]               @ 0x22 
    /*00005304*/ BFI         R2, LR, #18, #3 
    /*00005308*/ LDRB        LR, [R5, #27] 
    /*0000530c*/ BFI         R2, LR, #21, #3 
    /*00005310*/ LDRB        LR, [R5, #26] 
    /*00005314*/ BFI         R2, LR, #24, #3 
    /*00005318*/ LDRB        LR, [R5, #46]               @ 0x2e 
    /*0000531c*/ BFI         R2, LR, #27, #1 
    /*00005320*/ LDRB        LR, [R5, #47]               @ 0x2f 
    /*00005324*/ BFI         R2, LR, #28, #1 
    /*00005328*/ LDRB        LR, [R5, #55]               @ 0x37 
    /*0000532c*/ BFI         R2, LR, #29, #3 
    /*00005330*/ STR         R2, [R4, #44]               @ 0x2c 
    /*00005334*/ LDRB        R2, [R5, #17] 
    /*00005338*/ MOV         LR, #65536                  @ 0x10000 
    /*0000533c*/ BFI         IP, R2, #0, #4 
    /*00005340*/ LDRB        R2, [R5, #18] 
    /*00005344*/ BFI         IP, R2, #4, #4 
    /*00005348*/ LDRB        R2, [R5, #33]               @ 0x21 
    /*0000534c*/ BFI         IP, R2, #8, #4 
    /*00005350*/ LDRB        R2, [R5, #32] 
    /*00005354*/ BFI         IP, R2, #12, #4 
    /*00005358*/ LDRB        R2, [R5, #20] 
    /*0000535c*/ BFI         IP, R2, #16, #6 
    /*00005360*/ LDRB        R2, [R5, #19] 
    /*00005364*/ BFI         IP, R2, #22, #6 
    /*00005368*/ STR         IP, [R4, #48]               @ 0x30 
    /*0000536c*/ LDRB        IP, [R5, #48]               @ 0x30 
    /*00005370*/ MOV         R2, R3 
    /*00005374*/ BFI         R0, IP, #11, #1 
    /*00005378*/ LDRB        IP, [R5, #41]               @ 0x29 
    /*0000537c*/ BFI         R0, IP, #12, #1 
    /*00005380*/ LDRB        IP, [R5, #42]               @ 0x2a 
    /*00005384*/ BFI         R0, IP, #13, #3 
    /*00005388*/ LDRB        IP, [R5, #43]               @ 0x2b 
    /*0000538c*/ BFI         R0, IP, #16, #3 
    /*00005390*/ LDRB        IP, [R5, #50]               @ 0x32 
    /*00005394*/ BFI         R0, IP, #21, #1 
    /*00005398*/ LDRB        IP, [R5, #38]               @ 0x26 
    /*0000539c*/ BFI         R0, IP, #22, #1 
    /*000053a0*/ LDRB        IP, [R5, #40]               @ 0x28 
    /*000053a4*/ BFI         R0, IP, #23, #1 
    /*000053a8*/ STR         R0, [R4, #52]               @ 0x34 
    /*000053ac*/ STR         R3, [R4, #56]               @ 0x38 
    /*000053b0*/ MOV         R0, R3 
    /*000053b4*/ STR         R3, [R4, #60]               @ 0x3c 
    /*000053b8*/ LDR         IP, [R5, #68]               @ 0x44 
    /*000053bc*/ BIC         IP, IP, #15 
    /*000053c0*/ STR         IP, [R4, #64]               @ 0x40 
    /*000053c4*/ LDR         IP, [R6, #32] 
    /*000053c8*/ STR         IP, [R4, #68]               @ 0x44 
    /*000053cc*/ LDR         IP, [R6, #48]               @ 0x30 
    /*000053d0*/ STR         IP, [R4, #72]               @ 0x48 
    /*000053d4*/ STR         LR, [R4, #204]              @ 0xcc 
    /*000053d8*/ LDRB        IP, [R7, #59]               @ 0x3b 
    /*000053dc*/ BFI         R2, IP, #0, #1 
    /*000053e0*/ BFI         R2, R3, #1, #1 
    /*000053e4*/ STR         R2, [R4, #512]              @ 0x200 
    /*000053e8*/ LDRB        R3, [R7, #79]               @ 0x4f 
    /*000053ec*/ LDR         IP, [R6, #100]              @ 0x64 
    /*000053f0*/ LDRB        R2, [R7, #78]               @ 0x4e 
    /*000053f4*/ CMP         IP, #2 
    /*000053f8*/ LSL         R3, R3, #1 
    /*000053fc*/ AND         R2, R2, #1 
    /*00005400*/ AND         R3, R3, #2 
    /*00005404*/ ORR         R3, R3, R2 
    /*00005408*/ LDRBEQ      R2, [R7, #95]               @ 0x5f 
    /*0000540c*/ ANDEQ       R3, R3, #1 
    /*00005410*/ LDRB        LR, [R7, #68]               @ 0x44 
    /*00005414*/ MOV         IP, #244                    @ 0xf4 
    /*00005418*/ LSLEQ       R2, R2, #1 
    /*0000541c*/ ANDEQ       R2, R2, #2 
    /*00005420*/ ORREQ       R3, R2, R3 
    /*00005424*/ MOV         R2, #61                     @ 0x3d 
    /*00005428*/ BFI         R0, R3, #2, #2 
    /*0000542c*/ MOV         R3, #0 
    /*00005430*/ BFI         R0, LR, #1, #1 
    /*00005434*/ STR         R0, [R4, #516]              @ 0x204 
    /*00005438*/ LDRB        LR, [R7, #66]               @ 0x42 
    /*0000543c*/ MOV         R0, R3 
    /*00005440*/ MUL         IP, IP, R8 
    /*00005444*/ BFI         R3, LR, #0, #1 
    /*00005448*/ STR         R3, [R4, #520]              @ 0x208 
    /*0000544c*/ MOV         SL, R0 
    /*00005450*/ LDR         R3, [R5, #76]               @ 0x4c 
    /*00005454*/ ADD         R9, R1, IP 
    /*00005458*/ CMP         R3, R0 
    /*0000545c*/ MUL         R8, R2, R8 
    /*00005460*/ MOVEQ       R3, R0 
    /*00005464*/ MOVNE       R3, #3 
    /*00005468*/ STR         R3, [R4, #524]              @ 0x20c 
    /*0000546c*/ LDR         R3, [R5, #56]               @ 0x38 
    /*00005470*/ MOV         R2, R0 
    /*00005474*/ STR         R3, [R4, #528]              @ 0x210 
    /*00005478*/ LDR         R3, [R9, #4] 
    /*0000547c*/ ADD         R3, R8, R3 
    /*00005480*/ ADD         R3, R1, R3, LSL #2 
    /*00005484*/ LDR         LR, [R3, #108]              @ 0x6c 
    /*00005488*/ MOV         R3, R0 
    /*0000548c*/ STR         LR, [R4, #532]              @ 0x214 
    /*00005490*/ LDR         LR, [R9, #4] 
    /*00005494*/ ADD         LR, R8, LR 
    /*00005498*/ ADD         LR, LR, #44                 @ 0x2c 
    /*0000549c*/ LDR         LR, [R1, LR, LSL #2] 
    /*000054a0*/ STR         LR, [R4, #536]              @ 0x218 
    /*000054a4*/ LDRB        LR, [R7, #70]               @ 0x46 
    /*000054a8*/ ADD         LR, LR, #36                 @ 0x24 
    /*000054ac*/ LDR         LR, [R5, LR, LSL #2] 
    /*000054b0*/ STR         LR, [R4, #540]              @ 0x21c 
    /*000054b4*/ LDR         LR, [R1, IP] 
    /*000054b8*/ ADD         LR, R8, LR 
    /*000054bc*/ ADD         LR, R1, LR, LSL #2 
    /*000054c0*/ LDR         LR, [LR, #108]              @ 0x6c 
    /*000054c4*/ STR         LR, [R4, #544]              @ 0x220 
    /*000054c8*/ MOV         LR, R0 
    /*000054cc*/ LDR         IP, [R1, IP] 
    /*000054d0*/ ADD         R8, R8, IP 
    /*000054d4*/ MOV         IP, R2 
    /*000054d8*/ ADD         R8, R8, #44                 @ 0x2c 
    /*000054dc*/ LDR         R1, [R1, R8, LSL #2] 
    /*000054e0*/ STR         R1, [R4, #548]              @ 0x224 
    /*000054e4*/ LDRB        R1, [R7, #69]               @ 0x45 
    /*000054e8*/ BFI         R0, R1, #8, #3 
    /*000054ec*/ STR         R0, [R4, #552]              @ 0x228 
    /*000054f0*/ LDRB        R1, [R7, #60]               @ 0x3c 
    /*000054f4*/ MOV         R0, SL 
    /*000054f8*/ BFI         R2, R1, #0, #4 
    /*000054fc*/ LDRB        R1, [R7, #61]               @ 0x3d 
    /*00005500*/ BFI         R2, R1, #4, #4 
    /*00005504*/ LDRB        R1, [R7, #58]               @ 0x3a 
    /*00005508*/ BFI         R2, R1, #8, #2 
    /*0000550c*/ STR         R2, [R4, #564]              @ 0x234 
    /*00005510*/ LDRB        R2, [R5, #39]               @ 0x27 
    /*00005514*/ MOV         R1, R3 
    /*00005518*/ BFI         SL, R2, #0, #1 
    /*0000551c*/ LDRB        R2, [R5, #37]               @ 0x25 
    /*00005520*/ BFI         SL, R2, #4, #1 
    /*00005524*/ STR         SL, [R4, #568]              @ 0x238 
    /*00005528*/ STR         R3, [R4, #576]              @ 0x240 
    /*0000552c*/ MOV         R2, R3 
    /*00005530*/ LDR         R8, [R6, #16] 
    /*00005534*/ STR         R8, [R4, #3140]             @ 0xc44 
    /*00005538*/ LDR         R8, [R9, #12] 
    /*0000553c*/ STR         R8, [R4, #3192]             @ 0xc78 
    /*00005540*/ LDR         R8, [R9, #12] 
    /*00005544*/ LDR         SL, [R6, #88]               @ 0x58 
    /*00005548*/ ADD         R8, R8, SL 
    /*0000554c*/ STR         R8, [R4, #3196]             @ 0xc7c 
    /*00005550*/ LDR         R8, [R9, #16] 
    /*00005554*/ STR         R8, [R4, #3208]             @ 0xc88 
    /*00005558*/ LDR         R8, [R9, #16] 
    /*0000555c*/ LDR         R9, [R6, #88]               @ 0x58 
    /*00005560*/ ADD         R8, R8, R9 
    /*00005564*/ STR         R8, [R4, #3212]             @ 0xc8c 
    /*00005568*/ LDR         R8, [R5, #72]               @ 0x48 
    /*0000556c*/ STR         R8, [R4, #3224]             @ 0xc98 
    /*00005570*/ LDR         R9, [R6, #88]               @ 0x58 
    /*00005574*/ LDR         R8, [R5, #72]               @ 0x48 
    /*00005578*/ ADD         R8, R8, R9 
    /*0000557c*/ STR         R8, [R4, #3228]             @ 0xc9c 
    /*00005580*/ LDR         R8, [R5] 
    /*00005584*/ ADD         R8, R8, #15 
    /*00005588*/ BIC         R9, R8, #15 
    /*0000558c*/ LSR         R8, R8, #1 
    /*00005590*/ BFI         R3, R9, #0, #16 
    /*00005594*/ BIC         R8, R8, #-2147483641        @ 0x80000007 
    /*00005598*/ BFI         R3, R8, #16, #16 
    /*0000559c*/ STR         R3, [R4, #3240]             @ 0xca8 
    /*000055a0*/ LDR         R3, [R5] 
    /*000055a4*/ ADD         R3, R3, #15 
    /*000055a8*/ BIC         R8, R3, #15 
    /*000055ac*/ LSR         R3, R3, #1 
    /*000055b0*/ BFI         LR, R8, #0, #16 
    /*000055b4*/ BIC         R3, R3, #-2147483641        @ 0x80000007 
    /*000055b8*/ BFI         LR, R3, #16, #16 
    /*000055bc*/ STR         LR, [R4, #3244]             @ 0xcac 
    /*000055c0*/ LDR         R3, [R5] 
    /*000055c4*/ ADD         R3, R3, #15 
    /*000055c8*/ BIC         LR, R3, #15 
    /*000055cc*/ LSR         R3, R3, #1 
    /*000055d0*/ BFI         IP, LR, #0, #16 
    /*000055d4*/ BIC         R3, R3, #-2147483641        @ 0x80000007 
    /*000055d8*/ BFI         IP, R3, #16, #16 
    /*000055dc*/ STR         IP, [R4, #3248]             @ 0xcb0 
    /*000055e0*/ LDRH        R3, [R6, #80]               @ 0x50 
    /*000055e4*/ BFI         R0, R3, #0, #16 
    /*000055e8*/ BFI         R0, R3, #16, #16 
    /*000055ec*/ STR         R0, [R4, #3168]             @ 0xc60 
    /*000055f0*/ LDR         R3, [R5, #76]               @ 0x4c 
    /*000055f4*/ STR         R3, [R4, #3172]             @ 0xc64 
    /*000055f8*/ LDR         R3, [R7, #72]               @ 0x48 
    /*000055fc*/ STR         R3, [R4, #3176]             @ 0xc68 
    /*00005600*/ LDRH        R3, [R6, #80]               @ 0x50 
    /*00005604*/ BFI         R1, R3, #0, #16 
    /*00005608*/ BFI         R1, R3, #16, #16 
    /*0000560c*/ STR         R1, [R4, #3184]             @ 0xc70 
    /*00005610*/ LDRH        R3, [R6, #80]               @ 0x50 
    /*00005614*/ BFI         R2, R3, #0, #16 
    /*00005618*/ BFI         R2, R3, #16, #16 
    /*0000561c*/ STR         R2, [R4, #3188]             @ 0xc74 
    /*00005620*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    VEDU_DEC_HAL_H265_ConfigReg_x5624:
    /*00005624*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9e8] 
    /*00005628*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9a8] 
    /*0000562c*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9e8] 
    /*00005630*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9a8] 
    /*00005634*/ MOV         R2, #480                    @ 0x1e0 
    /*00005638*/ LDR         R1, [PC, #4]                @ 0x0000000000005644 
    /*0000563c*/ BL          0x000000000000563C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005640*/ B           VEDU_DEC_HAL_H265_ConfigReg_x5134
    /*00005644*/ LDRDEQ      R0, [R0], -IP 
FUNC_END VEDU_DEC_HAL_H265_ConfigReg


FUNC_BEGIN VEDU_DEC_HAL_H265_ReadRegs
    /*00005648*/ ADD         R1, R1, #12288              @ 0x3000 
    /*0000564c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00005650*/ SUB         SP, SP, #12 
    /*00005654*/ LDR         R3, [R1, #4] 
    /*00005658*/ TST         R3, #256                    @ 0x100 
    /*0000565c*/ BNE         VEDU_DEC_HAL_H265_ReadRegs_x5694
    /*00005660*/ LDR         R3, [R1, #4] 
    /*00005664*/ TST         R3, #512                    @ 0x200 
    /*00005668*/ BNE         VEDU_DEC_HAL_H265_ReadRegs_x56f4
    /*0000566c*/ LDR         R3, [R1, #4] 
    /*00005670*/ TST         R3, #64                     @ 0x40 
    /*00005674*/ BNE         VEDU_DEC_HAL_H265_ReadRegs_x56c4
    /*00005678*/ LDR         R3, [R1, #4] 
    /*0000567c*/ UBFX        R3, R3, #10, #1 
    /*00005680*/ CMP         R3, #0 
    /*00005684*/ STREQ       R3, [R0, #4] 
    /*00005688*/ BNE         VEDU_DEC_HAL_H265_ReadRegs_x5724
    /*0000568c*/ ADD         SP, SP, #12 
    /*00005690*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    VEDU_DEC_HAL_H265_ReadRegs_x5694:
    /*00005694*/ MOV         R2, #1 
    /*00005698*/ MOV         R3, #996                    @ 0x3e4 
    /*0000569c*/ STR         R2, [R0, #4] 
    /*000056a0*/ MOV         R1, #25 
    /*000056a4*/ STR         R3, [SP] 
    /*000056a8*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa04] 
    /*000056ac*/ LDR         R3, [PC, #160]              @ 0x0000000000005754 
    /*000056b0*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa04] 
    /*000056b4*/ MOV         R0, #3 
    /*000056b8*/ BL          0x00000000000056B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000056bc*/ ADD         SP, SP, #12 
    /*000056c0*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    VEDU_DEC_HAL_H265_ReadRegs_x56c4:
    /*000056c4*/ MOV         R3, #3 
    /*000056c8*/ MOVW        R2, #1006                   @ 0x3ee 
    /*000056cc*/ STR         R3, [R0, #4] 
    /*000056d0*/ MOV         R1, #25 
    /*000056d4*/ STR         R2, [SP] 
    /*000056d8*/ MOV         R0, R3 
    /*000056dc*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa80] 
    /*000056e0*/ LDR         R3, [PC, #108]              @ 0x0000000000005754 
    /*000056e4*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa80] 
    /*000056e8*/ BL          0x00000000000056E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000056ec*/ ADD         SP, SP, #12 
    /*000056f0*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    VEDU_DEC_HAL_H265_ReadRegs_x56f4:
    /*000056f4*/ MOV         R2, #2 
    /*000056f8*/ MOVW        R3, #1001                   @ 0x3e9 
    /*000056fc*/ STR         R2, [R0, #4] 
    /*00005700*/ MOV         R1, #25 
    /*00005704*/ STR         R3, [SP] 
    /*00005708*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa3c] 
    /*0000570c*/ LDR         R3, [PC, #64]               @ 0x0000000000005754 
    /*00005710*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa3c] 
    /*00005714*/ MOV         R0, #3 
    /*00005718*/ BL          0x0000000000005718          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000571c*/ ADD         SP, SP, #12 
    /*00005720*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    VEDU_DEC_HAL_H265_ReadRegs_x5724:
    /*00005724*/ MOV         R2, #1 
    /*00005728*/ MOVW        R3, #1011                   @ 0x3f3 
    /*0000572c*/ STR         R2, [R0, #4] 
    /*00005730*/ MOV         R1, #25 
    /*00005734*/ STR         R3, [SP] 
    /*00005738*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xaac] 
    /*0000573c*/ LDR         R3, [PC, #16]               @ 0x0000000000005754 
    /*00005740*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0xaac] 
    /*00005744*/ MOV         R0, #3 
    /*00005748*/ BL          0x0000000000005748          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000574c*/ ADD         SP, SP, #12 
    /*00005750*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00005754*/ STRDEQ      R0, [R0], -R8 
FUNC_END VEDU_DEC_HAL_H265_ReadRegs


FUNC_BEGIN VEDU_DEC_HAL_H265_ReadUpMsg
    /*00005758*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000575c*/ LDRH        LR, [R1, #12] 
    /*00005760*/ LDRB        R3, [R1, #112]              @ 0x70 
    /*00005764*/ LDR         IP, [R1, #56]               @ 0x38 
    /*00005768*/ ADDS        LR, LR, R3 
    /*0000576c*/ BEQ         VEDU_DEC_HAL_H265_ReadUpMsg_x5828
    /*00005770*/ LDR         R3, [IP, #4] 
    /*00005774*/ LDR         R1, [R1, #108]              @ 0x6c 
    /*00005778*/ UBFX        R3, R3, #0, #20 
    /*0000577c*/ CMP         R1, R3 
    /*00005780*/ BCC         VEDU_DEC_HAL_H265_ReadUpMsg_x57fc
    /*00005784*/ LDR         R2, [IP, #8] 
    /*00005788*/ UBFX        R2, R2, #0, #20 
    /*0000578c*/ CMP         R2, R1 
    /*00005790*/ BHI         VEDU_DEC_HAL_H265_ReadUpMsg_x57fc
    /*00005794*/ CMP         R3, R2 
    /*00005798*/ ADDLE       IP, IP, #16 
    /*0000579c*/ MOVLE       R5, #0 
    /*000057a0*/ MOVLE       R4, R5 
    /*000057a4*/ BLE         VEDU_DEC_HAL_H265_ReadUpMsg_x57d8
    /*000057a8*/ B           VEDU_DEC_HAL_H265_ReadUpMsg_x57fc
    VEDU_DEC_HAL_H265_ReadUpMsg_x57ac:
    /*000057ac*/ LDR         R3, [IP, #4] 
    /*000057b0*/ UBFX        R3, R3, #0, #20 
    /*000057b4*/ CMP         R3, R1 
    /*000057b8*/ BHI         VEDU_DEC_HAL_H265_ReadUpMsg_x57fc
    /*000057bc*/ LDR         R2, [IP, #8] 
    /*000057c0*/ ADD         IP, IP, #16 
    /*000057c4*/ UBFX        R2, R2, #0, #20 
    /*000057c8*/ CMP         R2, R1 
    /*000057cc*/ BHI         VEDU_DEC_HAL_H265_ReadUpMsg_x57fc
    /*000057d0*/ CMP         R3, R2 
    /*000057d4*/ BGT         VEDU_DEC_HAL_H265_ReadUpMsg_x57fc
    VEDU_DEC_HAL_H265_ReadUpMsg_x57d8:
    /*000057d8*/ CMP         R3, R2 
    /*000057dc*/ ADD         R4, R4, #1 
    /*000057e0*/ SUB         R7, R2, R3 
    /*000057e4*/ ADD         R6, R5, #1 
    /*000057e8*/ ADDLT       R5, R7, R6 
    /*000057ec*/ CMP         LR, R4 
    /*000057f0*/ BNE         VEDU_DEC_HAL_H265_ReadUpMsg_x57ac
    /*000057f4*/ CMP         R5, R1 
    /*000057f8*/ BLS         VEDU_DEC_HAL_H265_ReadUpMsg_x5808
    VEDU_DEC_HAL_H265_ReadUpMsg_x57fc:
    /*000057fc*/ MOV         R3, #100                    @ 0x64 
    /*00005800*/ STR         R3, [R0, #8] 
    /*00005804*/ POP         {R4, R5, R6, R7, R8, PC} 
    VEDU_DEC_HAL_H265_ReadUpMsg_x5808:
    /*00005808*/ MOV         LR, #100                    @ 0x64 
    /*0000580c*/ MUL         LR, LR, R5 
    VEDU_DEC_HAL_H265_ReadUpMsg_x5810:
    /*00005810*/ MOV         R4, R0 
    /*00005814*/ MOV         R0, LR 
    /*00005818*/ BL          0x0000000000005818          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000581c*/ RSB         R0, R0, #100                @ 0x64 
    /*00005820*/ STR         R0, [R4, #8] 
    /*00005824*/ POP         {R4, R5, R6, R7, R8, PC} 
    VEDU_DEC_HAL_H265_ReadUpMsg_x5828:
    /*00005828*/ LDR         R1, [R1, #108]              @ 0x6c 
    /*0000582c*/ B           VEDU_DEC_HAL_H265_ReadUpMsg_x5810
FUNC_END VEDU_DEC_HAL_H265_ReadUpMsg


FUNC_BEGIN hi35xx_vpu_remove
    /*00005830*/ PUSH        {R4, LR} 
    /*00005834*/ BL          0x0000000000005834          @ VPU_ModExit [R_ARM_CALL: .text+0xa08] 
    /*00005838*/ MOV         R0, #0 
    /*0000583c*/ POP         {R4, PC} 
FUNC_END hi35xx_vpu_remove


FUNC_BEGIN hi35xx_vpu_probe
    /*00005840*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00005844*/ MOV         R4, #0 
    /*00005848*/ SUB         SP, SP, #20 
    /*0000584c*/ MOV         R5, R0 
    /*00005850*/ MOV         R3, R4 
    /*00005854*/ MOV         R1, #10 
    /*00005858*/ ADD         R0, SP, #6 
    /*0000585c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xaf0] 
    /*00005860*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0xaf0] 
    /*00005864*/ STR         R4, [SP, #10] 
    /*00005868*/ STR         R4, [SP, #6] 
    /*0000586c*/ ADD         R7, R5, #16 
    /*00005870*/ STRH        R4, [SP, #14] 
    /*00005874*/ BL          0x0000000000005874          @ snprintf [R_ARM_CALL: +0x0] 
    /*00005878*/ MOV         R1, #512                    @ 0x200 
    /*0000587c*/ MOV         R0, R5 
    /*00005880*/ MOVW        R6, #0                      @ s_apVeduAddr [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005884*/ MOVT        R6, #0                      @ s_apVeduAddr [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005888*/ ADD         R2, SP, #6 
    /*0000588c*/ BL          0x000000000000588C          @ osal_platform_get_resource_byname [R_ARM_CALL: +0x0] 
    /*00005890*/ MOV         R1, R0 
    /*00005894*/ MOV         R0, R7 
    /*00005898*/ BL          0x0000000000005898          @ devm_ioremap_resource [R_ARM_CALL: +0x0] 
    /*0000589c*/ CMN         R0, #4096                   @ 0x1000 
    /*000058a0*/ STR         R0, [R6] 
    /*000058a4*/ BHI         hi35xx_vpu_probe_x5954
    /*000058a8*/ ADD         R1, SP, #6 
    /*000058ac*/ MOV         R0, R5 
    /*000058b0*/ BL          0x00000000000058B0          @ osal_platform_get_irq_byname [R_ARM_CALL: +0x0] 
    /*000058b4*/ MOVW        R2, #0                      @ vedu_irq [R_ARM_MOVW_ABS_NC: .bss+0x4] 
    /*000058b8*/ MOVT        R2, #0                      @ vedu_irq [R_ARM_MOVT_ABS: .bss+0x4] 
    /*000058bc*/ CMP         R0, #0 
    /*000058c0*/ STR         R0, [R2] 
    /*000058c4*/ BEQ         hi35xx_vpu_probe_x59fc
    hi35xx_vpu_probe_x58c8:
    /*000058c8*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb68] 
    /*000058cc*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb68] 
    /*000058d0*/ ADD         R2, SP, #6 
    /*000058d4*/ MOV         R1, #512                    @ 0x200 
    /*000058d8*/ LDR         R0, [R3] 
    /*000058dc*/ MOV         IP, #0 
    /*000058e0*/ LDRB        R3, [R3, #4] 
    /*000058e4*/ MOVW        R4, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVW_ABS_NC: .bss+0x14] 
    /*000058e8*/ STR         IP, [SP, #10] 
    /*000058ec*/ MOVT        R4, #0                      @ g_pstJpgeCHIP_Reg [R_ARM_MOVT_ABS: .bss+0x14] 
    /*000058f0*/ STR         R0, [SP, #6] 
    /*000058f4*/ MOV         R0, R5 
    /*000058f8*/ STRB        R3, [SP, #10] 
    /*000058fc*/ STRH        IP, [SP, #14] 
    /*00005900*/ BL          0x0000000000005900          @ osal_platform_get_resource_byname [R_ARM_CALL: +0x0] 
    /*00005904*/ MOV         R1, R0 
    /*00005908*/ MOV         R0, R7 
    /*0000590c*/ BL          0x000000000000590C          @ devm_ioremap_resource [R_ARM_CALL: +0x0] 
    /*00005910*/ CMN         R0, #4096                   @ 0x1000 
    /*00005914*/ STR         R0, [R4] 
    /*00005918*/ BHI         hi35xx_vpu_probe_x59bc
    /*0000591c*/ ADD         R1, SP, #6 
    /*00005920*/ MOV         R0, R5 
    /*00005924*/ BL          0x0000000000005924          @ osal_platform_get_irq_byname [R_ARM_CALL: +0x0] 
    /*00005928*/ MOVW        R3, #0                      @ jpeg_irq [R_ARM_MOVW_ABS_NC: .bss+0x18] 
    /*0000592c*/ MOVT        R3, #0                      @ jpeg_irq [R_ARM_MOVT_ABS: .bss+0x18] 
    /*00005930*/ CMP         R0, #0 
    /*00005934*/ STR         R0, [R3] 
    /*00005938*/ BEQ         hi35xx_vpu_probe_x5a30
    hi35xx_vpu_probe_x593c:
    /*0000593c*/ BL          0x000000000000593C          @ VPU_ModInit [R_ARM_CALL: .text+0x9a0] 
    /*00005940*/ SUBS        R4, R0, #0 
    /*00005944*/ BNE         hi35xx_vpu_probe_x599c
    /*00005948*/ MOV         R0, R4 
    /*0000594c*/ ADD         SP, SP, #20 
    /*00005950*/ POP         {R4, R5, R6, R7, PC} 
    hi35xx_vpu_probe_x5954:
    /*00005954*/ MOV         R3, R4 
    /*00005958*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x714] 
    /*0000595c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xaf8] 
    /*00005960*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x714] 
    /*00005964*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xaf8] 
    /*00005968*/ MOV         R2, #48                     @ 0x30 
    /*0000596c*/ BL          0x000000000000596C          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00005970*/ LDR         R4, [R6] 
    /*00005974*/ CMP         R4, #0 
    /*00005978*/ BEQ         hi35xx_vpu_probe_x58c8
    hi35xx_vpu_probe_x597c:
    /*0000597c*/ MOV         R2, #98                     @ 0x62 
    /*00005980*/ LDR         R1, [PC, #212]              @ 0x0000000000005a5c 
    /*00005984*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb50] 
    /*00005988*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb50] 
    /*0000598c*/ BL          0x000000000000598C          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00005990*/ MOV         R0, R4 
    /*00005994*/ ADD         SP, SP, #20 
    /*00005998*/ POP         {R4, R5, R6, R7, PC} 
    hi35xx_vpu_probe_x599c:
    /*0000599c*/ MOV         R2, #112                    @ 0x70 
    /*000059a0*/ LDR         R1, [PC, #180]              @ 0x0000000000005a5c 
    /*000059a4*/ MOVW        R0, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbd8] 
    /*000059a8*/ MOVT        R0, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbd8] 
    /*000059ac*/ BL          0x00000000000059AC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000059b0*/ MOV         R0, R4 
    /*000059b4*/ ADD         SP, SP, #20 
    /*000059b8*/ POP         {R4, R5, R6, R7, PC} 
    hi35xx_vpu_probe_x59bc:
    /*000059bc*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb70] 
    /*000059c0*/ MOV         R2, #76                     @ 0x4c 
    /*000059c4*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb70] 
    /*000059c8*/ LDR         R1, [PC, #144]              @ 0x0000000000005a60 
    /*000059cc*/ BL          0x00000000000059CC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000059d0*/ LDR         R4, [R4] 
    /*000059d4*/ CMP         R4, #0 
    /*000059d8*/ BEQ         hi35xx_vpu_probe_x593c
    hi35xx_vpu_probe_x59dc:
    /*000059dc*/ MOV         R2, #105                    @ 0x69 
    /*000059e0*/ LDR         R1, [PC, #116]              @ 0x0000000000005a5c 
    /*000059e4*/ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbc0] 
    /*000059e8*/ MOVT        R0, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbc0] 
    /*000059ec*/ BL          0x00000000000059EC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000059f0*/ MOV         R0, R4 
    /*000059f4*/ ADD         SP, SP, #20 
    /*000059f8*/ POP         {R4, R5, R6, R7, PC} 
    hi35xx_vpu_probe_x59fc:
    /*000059fc*/ MOV         R3, R0 
    /*00005a00*/ MOV         R2, #54                     @ 0x36 
    /*00005a04*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x714] 
    /*00005a08*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb14] 
    /*00005a0c*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x714] 
    /*00005a10*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb14] 
    /*00005a14*/ BL          0x0000000000005A14          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00005a18*/ MOV         R0, R7 
    /*00005a1c*/ MOVW        R1, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb38] 
    /*00005a20*/ MOVT        R1, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb38] 
    /*00005a24*/ BL          0x0000000000005A24          @ dev_err [R_ARM_CALL: +0x0] 
    /*00005a28*/ MVN         R4, #0 
    /*00005a2c*/ B           hi35xx_vpu_probe_x597c
    hi35xx_vpu_probe_x5a30:
    /*00005a30*/ MOV         R2, #82                     @ 0x52 
    /*00005a34*/ LDR         R1, [PC, #36]               @ 0x0000000000005a60 
    /*00005a38*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb88] 
    /*00005a3c*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb88] 
    /*00005a40*/ BL          0x0000000000005A40          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00005a44*/ MOV         R0, R7 
    /*00005a48*/ MOVW        R1, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xba8] 
    /*00005a4c*/ MOVT        R1, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0xba8] 
    /*00005a50*/ BL          0x0000000000005A50          @ dev_err [R_ARM_CALL: +0x0] 
    /*00005a54*/ MVN         R4, #0 
    /*00005a58*/ B           hi35xx_vpu_probe_x59dc
    /*00005a5c*/ ANDEQ       R0, R0, R8, LSR #14 
    /*00005a60*/ ANDEQ       R0, R0, IP, LSR R7 
FUNC_END hi35xx_vpu_probe


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .init.text
@ Size: 0xc
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN hi35xx_vpu_driver_init
    /*00000000*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x104] 
    /*00000004*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x104] 
    hi35xx_vpu_driver_init_x8:
    /*00000008*/ B           hi35xx_vpu_driver_init_x8   @ osal_platform_driver_register [R_ARM_JUMP24: +0x0] 
FUNC_END hi35xx_vpu_driver_init




@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .exit.text
@ Size: 0xc
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN hi35xx_vpu_driver_exit
    /*00000000*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x104] 
    /*00000004*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x104] 
    hi35xx_vpu_driver_exit_x8:
    /*00000008*/ B           hi35xx_vpu_driver_exit_x8   @ osal_platform_driver_unregister [R_ARM_JUMP24: +0x0] 
FUNC_END hi35xx_vpu_driver_exit




