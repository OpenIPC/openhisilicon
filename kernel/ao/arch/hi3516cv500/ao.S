@
@ -- FILES --
@

@ ao.c
@ ao_trans.c
@ ao_drv.c
@ ao_init.c
@ hi3516cv500_ao.mod.c


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text
@ Size: 0x8994
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN ao_restore
    /*00000000*/ MOV         R0, #0 
    /*00000004*/ BX          LR 
FUNC_END ao_restore


FUNC_BEGIN AO_GetVerMagic
    /*00000008*/ MOVW        R0, #61479                  @ 0xf027 
    /*0000000c*/ MOVT        R0, #307                    @ 0x133 
    /*00000010*/ BX          LR 
FUNC_END AO_GetVerMagic


FUNC_BEGIN AOReceiveFrm
    /*00000014*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000018*/ CMP         R3, #5 
    /*0000001c*/ SUB         SP, SP, #52                 @ 0x34 
    /*00000020*/ MOV         FP, R0 
    /*00000024*/ STR         R1, [SP, #24] 
    /*00000028*/ LDR         R9, [SP, #88]               @ 0x58 
    /*0000002c*/ BEQ         AOReceiveFrm_x50
    /*00000030*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*00000034*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000038*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*0000003c*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*00000040*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*00000044*/ MOV         R2, #69                     @ 0x45 
    /*00000048*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*0000004c*/ BL          0x000000000000004C          @ osal_panic [R_ARM_CALL: +0x0] 
    AOReceiveFrm_x50:
    /*00000050*/ CMP         R9, #0 
    /*00000054*/ BEQ         AOReceiveFrm_x310
    AOReceiveFrm_x58:
    /*00000058*/ CMP         FP, #1 
    /*0000005c*/ BHI         AOReceiveFrm_x358
    /*00000060*/ LDR         R3, [SP, #24] 
    /*00000064*/ CMP         R3, #2 
    /*00000068*/ BHI         AOReceiveFrm_x3b0
    /*0000006c*/ MOV         R0, FP 
    /*00000070*/ BL          0x0000000000000070          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*00000074*/ SUBS        R7, R0, #0 
    /*00000078*/ BEQ         AOReceiveFrm_x334
    AOReceiveFrm_x7c:
    /*0000007c*/ LDR         R3, [SP, #24] 
    /*00000080*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000084*/ ADD         R1, SP, #44                 @ 0x2c 
    /*00000088*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000008c*/ CMP         R3, #2 
    /*00000090*/ LDRNE       R5, [R7, #28] 
    /*00000094*/ MOVEQ       R5, #0 
    /*00000098*/ BL          0x0000000000000098          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000009c*/ LDR         R3, [R7, #12] 
    /*000000a0*/ CMP         R3, #1 
    /*000000a4*/ BNE         AOReceiveFrm_x38c
    /*000000a8*/ LDR         R3, [R7, #28] 
    /*000000ac*/ CMP         R3, #1 
    /*000000b0*/ BEQ         AOReceiveFrm_x2ac
    AOReceiveFrm_xb4:
    /*000000b4*/ LDR         R1, [R7, #20] 
    /*000000b8*/ LDR         R3, [R9] 
    /*000000bc*/ CMP         R1, R3 
    /*000000c0*/ BNE         AOReceiveFrm_x3e4
    /*000000c4*/ CMP         R1, #2 
    /*000000c8*/ LDREQ       R4, [R9, #44]               @ 0x2c 
    /*000000cc*/ LSREQ       R4, R4, #2 
    /*000000d0*/ BEQ         AOReceiveFrm_xe4
    /*000000d4*/ ADD         R1, R1, #1 
    /*000000d8*/ LDR         R0, [R9, #44]               @ 0x2c 
    /*000000dc*/ BL          0x00000000000000DC          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000000e0*/ MOV         R4, R0 
    AOReceiveFrm_xe4:
    /*000000e4*/ LDR         R3, [R7, #40]               @ 0x28 
    /*000000e8*/ CMP         R4, R3 
    /*000000ec*/ BNE         AOReceiveFrm_x430
    /*000000f0*/ CMP         R5, #1 
    /*000000f4*/ LDR         R3, [SP, #24] 
    /*000000f8*/ ADD         R8, FP, FP, LSL #1 
    /*000000fc*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000100*/ ADD         R8, R8, R3 
    /*00000104*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*00000108*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*0000010c*/ MOVNE       SL, #1 
    /*00000110*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00000114*/ MOVEQ       SL, #2 
    /*00000118*/ STR         R3, [SP, #28] 
    /*0000011c*/ ADD         R6, R9, #8 
    /*00000120*/ MOVW        R3, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00000124*/ STR         R2, [SP, #32] 
    /*00000128*/ MOVT        R3, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*0000012c*/ MOV         R5, #0 
    /*00000130*/ STR         R3, [SP, #36]               @ 0x24 
    /*00000134*/ B           AOReceiveFrm_x18c
    AOReceiveFrm_x138:
    /*00000138*/ LDR         R1, [R6], #4 
    /*0000013c*/ CMP         R1, #0 
    /*00000140*/ BEQ         AOReceiveFrm_x15c
    /*00000144*/ ADD         R3, R3, #2 
    /*00000148*/ LDR         R2, [R9, #44]               @ 0x2c 
    /*0000014c*/ LDR         R0, [R4, R3, LSL #2] 
    /*00000150*/ BL          0x0000000000000150          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000154*/ LDR         R3, [R4, #1224]             @ 0x4c8 
    /*00000158*/ LDR         R2, [R4, #1228]             @ 0x4cc 
    AOReceiveFrm_x15c:
    /*0000015c*/ LDR         R1, [R4, #1208]             @ 0x4b8 
    /*00000160*/ ADD         R3, R3, #1 
    /*00000164*/ STR         R3, [R4, #1224]             @ 0x4c8 
    /*00000168*/ ADD         R5, R5, #1 
    /*0000016c*/ CMP         R3, R1 
    /*00000170*/ MOVEQ       R3, #0 
    /*00000174*/ STREQ       R3, [R4, #1224]             @ 0x4c8 
    /*00000178*/ CMP         R3, R2 
    /*0000017c*/ MOVEQ       R3, #1 
    /*00000180*/ STREQ       R3, [R4, #1236]             @ 0x4d4 
    /*00000184*/ CMP         SL, R5 
    /*00000188*/ BEQ         AOReceiveFrm_x290
    AOReceiveFrm_x18c:
    /*0000018c*/ LDR         R0, [R7, #44]               @ 0x2c 
    /*00000190*/ LSR         R0, R0, #1 
    /*00000194*/ MLA         R0, R5, R0, R8 
    /*00000198*/ BL          0x0000000000000198          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*0000019c*/ SUBS        R4, R0, #0 
    /*000001a0*/ BEQ         AOReceiveFrm_x21c
    /*000001a4*/ LDR         R3, [R4, #1240]             @ 0x4d8 
    /*000001a8*/ SUB         R3, R3, #1 
    /*000001ac*/ CMP         R3, #1 
    /*000001b0*/ BHI         AOReceiveFrm_x240
    AOReceiveFrm_x1b4:
    /*000001b4*/ LDR         R2, [R4, #1228]             @ 0x4cc 
    /*000001b8*/ LDR         R3, [R4, #1224]             @ 0x4c8 
    /*000001bc*/ CMP         R2, R3 
    /*000001c0*/ BNE         AOReceiveFrm_x138
    /*000001c4*/ LDR         R1, [R4, #1236]             @ 0x4d4 
    /*000001c8*/ CMP         R1, #1 
    /*000001cc*/ BNE         AOReceiveFrm_x138
    /*000001d0*/ ADD         R1, SP, #44                 @ 0x2c 
    /*000001d4*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000001d8*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000001dc*/ BL          0x00000000000001DC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000001e0*/ LDR         R3, [SP, #24] 
    /*000001e4*/ MOV         R2, #142                    @ 0x8e 
    /*000001e8*/ STR         FP, [SP, #4] 
    /*000001ec*/ STR         R2, [SP] 
    /*000001f0*/ MOV         R1, #22 
    /*000001f4*/ STR         R3, [SP, #8] 
    /*000001f8*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x258] 
    /*000001fc*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000200*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x258] 
    /*00000204*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*00000208*/ MOV         R0, #4 
    /*0000020c*/ BL          0x000000000000020C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000210*/ MOVW        R0, #32783                  @ 0x800f 
    /*00000214*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000218*/ B           AOReceiveFrm_x2a4
    AOReceiveFrm_x21c:
    /*0000021c*/ LDR         R3, [SP, #28] 
    /*00000220*/ MOV         R2, #128                    @ 0x80 
    /*00000224*/ LDR         R1, [SP, #32] 
    /*00000228*/ LDR         R0, [SP, #36]               @ 0x24 
    /*0000022c*/ BL          0x000000000000022C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000230*/ LDR         R3, [R4, #1240]             @ 0x4d8 
    /*00000234*/ SUB         R3, R3, #1 
    /*00000238*/ CMP         R3, #1 
    /*0000023c*/ BLS         AOReceiveFrm_x1b4
    AOReceiveFrm_x240:
    /*00000240*/ ADD         R1, SP, #44                 @ 0x2c 
    /*00000244*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000248*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000024c*/ BL          0x000000000000024C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000250*/ LDR         R3, [SP, #24] 
    /*00000254*/ MOV         R2, #134                    @ 0x86 
    /*00000258*/ STR         FP, [SP, #4] 
    /*0000025c*/ STR         R3, [SP, #8] 
    AOReceiveFrm_x260:
    /*00000260*/ STR         R2, [SP] 
    /*00000264*/ MOV         R1, #22 
    /*00000268*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*0000026c*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf0] 
    /*00000270*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*00000274*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf0] 
    /*00000278*/ MOV         R0, #3 
    /*0000027c*/ BL          0x000000000000027C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000280*/ MOVW        R0, #32773                  @ 0x8005 
    /*00000284*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000288*/ ADD         SP, SP, #52                 @ 0x34 
    /*0000028c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOReceiveFrm_x290:
    /*00000290*/ ADD         R1, SP, #44                 @ 0x2c 
    /*00000294*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000298*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000029c*/ BL          0x000000000000029C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000002a0*/ MOV         R0, #0 
    AOReceiveFrm_x2a4:
    /*000002a4*/ ADD         SP, SP, #52                 @ 0x34 
    /*000002a8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOReceiveFrm_x2ac:
    /*000002ac*/ LDR         R3, [R7, #44]               @ 0x2c 
    /*000002b0*/ LDR         R2, [SP, #24] 
    /*000002b4*/ CMP         R2, R3, LSR #1 
    /*000002b8*/ BCC         AOReceiveFrm_xb4
    /*000002bc*/ ADD         R1, SP, #44                 @ 0x2c 
    /*000002c0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000002c4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000002c8*/ BL          0x00000000000002C8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000002cc*/ LDR         R3, [SP, #24] 
    /*000002d0*/ MOV         R1, #93                     @ 0x5d 
    /*000002d4*/ MOV         R0, #3 
    /*000002d8*/ STR         R3, [SP, #12] 
    /*000002dc*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000002e0*/ LDR         R2, [R7, #44]               @ 0x2c 
    /*000002e4*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*000002e8*/ STR         R1, [SP] 
    /*000002ec*/ MOV         R1, #22 
    /*000002f0*/ STR         FP, [SP, #4] 
    /*000002f4*/ STR         R2, [SP, #8] 
    /*000002f8*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x134] 
    /*000002fc*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x134] 
    /*00000300*/ BL          0x0000000000000300          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000304*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000308*/ MOVT        R0, #40982                  @ 0xa016 
    /*0000030c*/ B           AOReceiveFrm_x2a4
    AOReceiveFrm_x310:
    /*00000310*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x64] 
    /*00000314*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000318*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x64] 
    /*0000031c*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*00000320*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00000324*/ MOV         R2, #70                     @ 0x46 
    /*00000328*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*0000032c*/ BL          0x000000000000032C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000330*/ B           AOReceiveFrm_x58
    AOReceiveFrm_x334:
    /*00000334*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*00000338*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*0000033c*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*00000340*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*00000344*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00000348*/ MOV         R2, #76                     @ 0x4c 
    /*0000034c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00000350*/ BL          0x0000000000000350          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000354*/ B           AOReceiveFrm_x7c
    AOReceiveFrm_x358:
    /*00000358*/ MOV         R2, #73                     @ 0x49 
    /*0000035c*/ STR         FP, [SP, #4] 
    /*00000360*/ STR         R2, [SP] 
    /*00000364*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000368*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*0000036c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*00000370*/ MOV         R1, #22 
    /*00000374*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00000378*/ MOV         R0, #3 
    /*0000037c*/ BL          0x000000000000037C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000380*/ MOVW        R0, #32769                  @ 0x8001 
    /*00000384*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000388*/ B           AOReceiveFrm_x2a4
    AOReceiveFrm_x38c:
    /*0000038c*/ ADD         R1, SP, #44                 @ 0x2c 
    /*00000390*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000394*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000398*/ BL          0x0000000000000398          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000039c*/ LDR         R3, [SP, #24] 
    /*000003a0*/ STR         FP, [SP, #4] 
    /*000003a4*/ MOV         R2, #85                     @ 0x55 
    /*000003a8*/ STR         R3, [SP, #8] 
    /*000003ac*/ B           AOReceiveFrm_x260
    AOReceiveFrm_x3b0:
    /*000003b0*/ STR         R3, [SP, #4] 
    /*000003b4*/ MOV         R2, #74                     @ 0x4a 
    /*000003b8*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000003bc*/ STR         R2, [SP] 
    /*000003c0*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*000003c4*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*000003c8*/ MOV         R1, #20 
    /*000003cc*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*000003d0*/ MOV         R0, #3 
    /*000003d4*/ BL          0x00000000000003D4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000003d8*/ MOVW        R0, #32770                  @ 0x8002 
    /*000003dc*/ MOVT        R0, #40982                  @ 0xa016 
    /*000003e0*/ B           AOReceiveFrm_x2a4
    AOReceiveFrm_x3e4:
    /*000003e4*/ ADD         R1, SP, #44                 @ 0x2c 
    /*000003e8*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000003ec*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000003f0*/ BL          0x00000000000003F0          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000003f4*/ LDR         R3, [SP, #24] 
    /*000003f8*/ MOV         R2, #101                    @ 0x65 
    /*000003fc*/ STR         FP, [SP, #4] 
    /*00000400*/ STR         R2, [SP] 
    /*00000404*/ MOV         R1, #22 
    /*00000408*/ STR         R3, [SP, #8] 
    /*0000040c*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x188] 
    /*00000410*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000414*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x188] 
    /*00000418*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*0000041c*/ MOV         R0, #3 
    /*00000420*/ BL          0x0000000000000420          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000424*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000428*/ MOVT        R0, #40982                  @ 0xa016 
    /*0000042c*/ B           AOReceiveFrm_x2a4
    AOReceiveFrm_x430:
    /*00000430*/ ADD         R1, SP, #44                 @ 0x2c 
    /*00000434*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000438*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000043c*/ BL          0x000000000000043C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000440*/ LDR         R2, [R7, #40]               @ 0x28 
    /*00000444*/ LDR         R3, [SP, #24] 
    /*00000448*/ MOV         R1, #118                    @ 0x76 
    /*0000044c*/ STR         R4, [SP, #12] 
    /*00000450*/ MOV         R0, #3 
    /*00000454*/ STR         R2, [SP, #16] 
    /*00000458*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e0] 
    /*0000045c*/ STR         R3, [SP, #8] 
    /*00000460*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e0] 
    /*00000464*/ STR         R1, [SP] 
    /*00000468*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*0000046c*/ STR         FP, [SP, #4] 
    /*00000470*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*00000474*/ MOV         R1, #22 
    /*00000478*/ BL          0x0000000000000478          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000047c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000480*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000484*/ B           AOReceiveFrm_x2a4
FUNC_END AOReceiveFrm


FUNC_BEGIN AO_Notify
    /*00000488*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000048c*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000490*/ PUSH        {R4, LR} 
    /*00000494*/ MOV         R2, #1 
    /*00000498*/ LDR         R0, [PC, #112]              @ 0x0000000000000510 
    /*0000049c*/ STR         R2, [R3] 
    /*000004a0*/ BL          0x00000000000004A0          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000004a4*/ CMP         R0, #0 
    /*000004a8*/ BNE         AO_Notify_x4c8
    /*000004ac*/ MOVW        R0, #0                      @ g_stAoDev [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*000004b0*/ MOVT        R0, #0                      @ g_stAoDev [R_ARM_MOVT_ABS: .bss+0x10] 
    /*000004b4*/ LDR         R3, [R0, #4] 
    /*000004b8*/ CMP         R3, #1 
    /*000004bc*/ BEQ         AO_Notify_x504
    AO_Notify_x4c0:
    /*000004c0*/ LDR         R0, [PC, #72]               @ 0x0000000000000510 
    /*000004c4*/ BL          0x00000000000004C4          @ osal_up [R_ARM_CALL: +0x0] 
    AO_Notify_x4c8:
    /*000004c8*/ LDR         R0, [PC, #68]               @ 0x0000000000000514 
    /*000004cc*/ BL          0x00000000000004CC          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000004d0*/ CMP         R0, #0 
    /*000004d4*/ POPNE       {R4, PC} 
    /*000004d8*/ MOVW        R0, #0                      @ g_stAoDev [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*000004dc*/ MOVT        R0, #0                      @ g_stAoDev [R_ARM_MOVT_ABS: .bss+0x10] 
    /*000004e0*/ LDR         R3, [R0, #140]              @ 0x8c 
    /*000004e4*/ CMP         R3, #1 
    /*000004e8*/ BEQ         AO_Notify_x4f8
    AO_Notify_x4ec:
    /*000004ec*/ LDR         R0, [PC, #32]               @ 0x0000000000000514 
    /*000004f0*/ POP         {R4, LR} 
    AO_Notify_x4f4:
    /*000004f4*/ B           AO_Notify_x4f4              @ osal_up [R_ARM_JUMP24: +0x0] 
    AO_Notify_x4f8:
    /*000004f8*/ ADD         R0, R0, #256                @ 0x100 
    /*000004fc*/ BL          0x00000000000004FC          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00000500*/ B           AO_Notify_x4ec
    AO_Notify_x504:
    /*00000504*/ ADD         R0, R0, #120                @ 0x78 
    /*00000508*/ BL          0x0000000000000508          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*0000050c*/ B           AO_Notify_x4c0
    /*00000510*/ ANDEQ       R0, R0, IP, ROR R0          @ g_stAoDev [R_ARM_ABS32: .bss+0x10] 
    /*00000514*/ ANDEQ       R0, R0, R4, LSL #2          @ g_stAoDev [R_ARM_ABS32: .bss+0x10] 
FUNC_END AO_Notify


FUNC_BEGIN AO_QueryState
    /*00000518*/ PUSH        {R4, LR} 
    /*0000051c*/ MOV         R4, R0 
    /*00000520*/ LDR         R0, [PC, #20]               @ 0x000000000000053c 
    /*00000524*/ BL          0x0000000000000524          @ osal_atomic_read [R_ARM_CALL: +0x0] 
    /*00000528*/ CMP         R0, #0 
    /*0000052c*/ MOVEQ       R3, #17 
    /*00000530*/ MOVNE       R3, #34                     @ 0x22 
    /*00000534*/ STR         R3, [R4] 
    /*00000538*/ POP         {R4, PC} 
    /*0000053c*/ ANDEQ       R0, R0, R4 
FUNC_END AO_QueryState


FUNC_BEGIN ao_poll
    /*00000540*/ PUSH        {R4, R5, R6, LR} 
    /*00000544*/ MOV         R6, R0 
    /*00000548*/ LDR         R4, [R1] 
    /*0000054c*/ SUB         SP, SP, #8 
    /*00000550*/ MOV         R0, R4 
    /*00000554*/ BL          0x0000000000000554          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00000558*/ MOVW        R2, #21846                  @ 0x5556 
    /*0000055c*/ MOVT        R2, #21845                  @ 0x5555 
    /*00000560*/ SMULL       R2, R3, R4, R2 
    /*00000564*/ MOV         R5, R0 
    /*00000568*/ SUB         R0, R3, R4, ASR #31 
    /*0000056c*/ BL          0x000000000000056C          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*00000570*/ CMP         R0, #0 
    /*00000574*/ CMPNE       R5, #0 
    /*00000578*/ MOVEQ       R4, #1 
    /*0000057c*/ MOVNE       R4, #0 
    /*00000580*/ MOVEQ       R4, #0 
    /*00000584*/ BEQ         ao_poll_x594
    /*00000588*/ LDR         R3, [R0, #12] 
    /*0000058c*/ CMP         R3, #1 
    /*00000590*/ BEQ         ao_poll_x5a0
    ao_poll_x594:
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ ADD         SP, SP, #8 
    /*0000059c*/ POP         {R4, R5, R6, PC} 
    ao_poll_x5a0:
    /*000005a0*/ LDR         R3, [R5, #1240]             @ 0x4d8 
    /*000005a4*/ SUB         R3, R3, #1 
    /*000005a8*/ CMP         R3, #1 
    /*000005ac*/ BHI         ao_poll_x594
    /*000005b0*/ ADD         R1, R0, #120                @ 0x78 
    /*000005b4*/ MOV         R0, R6 
    /*000005b8*/ BL          0x00000000000005B8          @ osal_poll_wait [R_ARM_CALL: +0x0] 
    /*000005bc*/ ADD         R1, SP, #4 
    /*000005c0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000005c4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000005c8*/ BL          0x00000000000005C8          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000005cc*/ LDR         R2, [R5, #1228]             @ 0x4cc 
    /*000005d0*/ LDR         R3, [R5, #1224]             @ 0x4c8 
    /*000005d4*/ CMP         R2, R3 
    /*000005d8*/ MOVNE       R4, #68                     @ 0x44 
    /*000005dc*/ BEQ         ao_poll_x5f4
    ao_poll_x5e0:
    /*000005e0*/ ADD         R1, SP, #4 
    /*000005e4*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000005e8*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000005ec*/ BL          0x00000000000005EC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000005f0*/ B           ao_poll_x594
    ao_poll_x5f4:
    /*000005f4*/ LDR         R3, [R5, #1236]             @ 0x4d4 
    /*000005f8*/ CMP         R3, #1 
    /*000005fc*/ MOVNE       R4, #68                     @ 0x44 
    /*00000600*/ MOVEQ       R4, #0 
    /*00000604*/ B           ao_poll_x5e0
FUNC_END ao_poll


FUNC_BEGIN AoPutFrame_WaitConditionCallBack
    /*00000608*/ CMP         R0, #0 
    /*0000060c*/ BXEQ        LR 
    /*00000610*/ LDR         R2, [R0, #1228]             @ 0x4cc 
    /*00000614*/ LDR         R3, [R0, #1224]             @ 0x4c8 
    /*00000618*/ CMP         R2, R3 
    /*0000061c*/ BEQ         AoPutFrame_WaitConditionCallBack_x634
    AoPutFrame_WaitConditionCallBack_x620:
    /*00000620*/ LDR         R3, [R0, #1240]             @ 0x4d8 
    /*00000624*/ CMP         R3, #2 
    /*00000628*/ BEQ         AoPutFrame_WaitConditionCallBack_x650
    AoPutFrame_WaitConditionCallBack_x62c:
    /*0000062c*/ MOV         R0, #1 
    /*00000630*/ BX          LR 
    AoPutFrame_WaitConditionCallBack_x634:
    /*00000634*/ LDR         R3, [R0, #1236]             @ 0x4d4 
    /*00000638*/ CMP         R3, #1 
    /*0000063c*/ BNE         AoPutFrame_WaitConditionCallBack_x620
    /*00000640*/ LDR         R2, [R0, #1240]             @ 0x4d8 
    /*00000644*/ SUB         R2, R2, #1 
    /*00000648*/ CMP         R2, #1 
    /*0000064c*/ BHI         AoPutFrame_WaitConditionCallBack_x62c
    AoPutFrame_WaitConditionCallBack_x650:
    /*00000650*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000654*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000658*/ LDR         R0, [R3] 
    /*0000065c*/ ADDS        R0, R0, #0 
    /*00000660*/ MOVNE       R0, #1 
    /*00000664*/ BX          LR 
FUNC_END AoPutFrame_WaitConditionCallBack


FUNC_BEGIN AO_Init
    /*00000668*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*0000066c*/ MOVW        R6, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000670*/ MOVT        R6, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000674*/ SUB         SP, SP, #36                 @ 0x24 
    /*00000678*/ LDR         R4, [R6] 
    /*0000067c*/ CMP         R4, #0 
    /*00000680*/ BEQ         AO_Init_x7d4
    /*00000684*/ CMP         R4, #1 
    /*00000688*/ BEQ         AO_Init_x800
    /*0000068c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000690*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000694*/ BL          0x0000000000000694          @ osal_spin_lock_init [R_ARM_CALL: +0x0] 
    /*00000698*/ MOV         R0, #0 
    /*0000069c*/ BL          0x000000000000069C          @ AO_DEV_Init [R_ARM_CALL: .text+0x48c0] 
    /*000006a0*/ SUBS        R4, R0, #0 
    /*000006a4*/ BNE         AO_Init_x6e0
    /*000006a8*/ LDR         R9, [PC, #380]              @ 0x000000000000082c 
    /*000006ac*/ MOVW        R8, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2f0] 
    /*000006b0*/ MOVW        R7, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*000006b4*/ MOVT        R8, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2f0] 
    /*000006b8*/ MOVT        R7, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    AO_Init_x6bc:
    /*000006bc*/ MOV         R0, R4 
    /*000006c0*/ ADD         R4, R4, #1 
    /*000006c4*/ BL          0x00000000000006C4          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*000006c8*/ SUBS        R5, R0, #0 
    /*000006cc*/ BEQ         AO_Init_x784
    AO_Init_x6d0:
    /*000006d0*/ CMP         R4, #3 
    /*000006d4*/ MOV         R3, #0 
    /*000006d8*/ STR         R3, [R5, #1244]             @ 0x4dc 
    /*000006dc*/ BNE         AO_Init_x6bc
    AO_Init_x6e0:
    /*000006e0*/ MOV         R0, #1 
    /*000006e4*/ BL          0x00000000000006E4          @ AO_DEV_Init [R_ARM_CALL: .text+0x48c0] 
    /*000006e8*/ CMP         R0, #0 
    /*000006ec*/ BNE         AO_Init_x72c
    /*000006f0*/ LDR         R9, [PC, #308]              @ 0x000000000000082c 
    /*000006f4*/ MOVW        R8, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2f0] 
    /*000006f8*/ MOVW        R7, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*000006fc*/ MOVT        R8, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2f0] 
    /*00000700*/ MOVT        R7, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00000704*/ MOV         R4, #3 
    AO_Init_x708:
    /*00000708*/ MOV         R0, R4 
    /*0000070c*/ ADD         R4, R4, #1 
    /*00000710*/ BL          0x0000000000000710          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00000714*/ SUBS        R5, R0, #0 
    /*00000718*/ BEQ         AO_Init_x79c
    AO_Init_x71c:
    /*0000071c*/ CMP         R4, #6 
    /*00000720*/ MOV         R3, #0 
    /*00000724*/ STR         R3, [R5, #1244]             @ 0x4dc 
    /*00000728*/ BNE         AO_Init_x708
    AO_Init_x72c:
    /*0000072c*/ MOV         R2, #2 
    /*00000730*/ MOV         R1, #22 
    /*00000734*/ MOV         R0, R2 
    /*00000738*/ STR         R2, [SP, #12] 
    /*0000073c*/ STR         R1, [SP, #8] 
    /*00000740*/ MOV         R2, #3 
    /*00000744*/ MOVW        R3, #0                      @ AOReceiveFrm [R_ARM_MOVW_ABS_NC: .text+0x14] 
    /*00000748*/ STR         R2, [SP, #16] 
    /*0000074c*/ MOVT        R3, #0                      @ AOReceiveFrm [R_ARM_MOVT_ABS: .text+0x14] 
    /*00000750*/ STR         R3, [SP, #20] 
    /*00000754*/ MOV         R3, #0 
    /*00000758*/ STR         R3, [SP, #28] 
    /*0000075c*/ BL          0x000000000000075C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000760*/ LDR         R3, [R0, #32] 
    /*00000764*/ ADD         R0, SP, #8 
    /*00000768*/ BLX         R3 
    /*0000076c*/ CMP         R0, #0 
    /*00000770*/ BNE         AO_Init_x7b4
    AO_Init_x774:
    /*00000774*/ MOV         R0, #0 
    /*00000778*/ STR         R0, [R6] 
    AO_Init_x77c:
    /*0000077c*/ ADD         SP, SP, #36                 @ 0x24 
    /*00000780*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AO_Init_x784:
    /*00000784*/ MOV         R3, R8 
    /*00000788*/ MOVW        R2, #1486                   @ 0x5ce 
    /*0000078c*/ MOV         R1, R9 
    /*00000790*/ MOV         R0, R7 
    /*00000794*/ BL          0x0000000000000794          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000798*/ B           AO_Init_x6d0
    AO_Init_x79c:
    /*0000079c*/ MOV         R3, R8 
    /*000007a0*/ MOVW        R2, #1486                   @ 0x5ce 
    /*000007a4*/ MOV         R1, R9 
    /*000007a8*/ MOV         R0, R7 
    /*000007ac*/ BL          0x00000000000007AC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000007b0*/ B           AO_Init_x71c
    AO_Init_x7b4:
    /*000007b4*/ MOVW        R3, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x304] 
    /*000007b8*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*000007bc*/ MOVT        R3, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x304] 
    /*000007c0*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*000007c4*/ MOVW        R2, #1531                   @ 0x5fb 
    /*000007c8*/ LDR         R1, [PC, #96]               @ 0x0000000000000830 
    /*000007cc*/ BL          0x00000000000007CC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000007d0*/ B           AO_Init_x774
    AO_Init_x7d4:
    /*000007d4*/ MOVW        R3, #1507                   @ 0x5e3 
    /*000007d8*/ MOV         R1, #22 
    /*000007dc*/ STR         R3, [SP] 
    /*000007e0*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x294] 
    /*000007e4*/ LDR         R3, [PC, #68]               @ 0x0000000000000830 
    /*000007e8*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x294] 
    /*000007ec*/ MOV         R0, #6 
    /*000007f0*/ BL          0x00000000000007F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000007f4*/ MOV         R0, R4 
    /*000007f8*/ ADD         SP, SP, #36                 @ 0x24 
    /*000007fc*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AO_Init_x800:
    /*00000800*/ MOVW        R3, #1512                   @ 0x5e8 
    /*00000804*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c4] 
    /*00000808*/ STR         R3, [SP] 
    /*0000080c*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c4] 
    /*00000810*/ LDR         R3, [PC, #24]               @ 0x0000000000000830 
    /*00000814*/ MOV         R1, #22 
    /*00000818*/ MOV         R0, #3 
    /*0000081c*/ BL          0x000000000000081C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000820*/ MOVW        R0, #32786                  @ 0x8012 
    /*00000824*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000828*/ B           AO_Init_x77c
    /*0000082c*/ ANDEQ       R0, R0, R8, LSL R0 
    /*00000830*/ ANDEQ       R0, R0, R0, LSL R0 
FUNC_END AO_Init


FUNC_BEGIN AO_Exit
    /*00000834*/ PUSH        {R4, LR} 
    /*00000838*/ MOVW        R4, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000083c*/ MOVT        R4, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000840*/ LDR         R3, [R4] 
    /*00000844*/ CMP         R3, #2 
    /*00000848*/ POPEQ       {R4, PC} 
    /*0000084c*/ MOV         R0, #2 
    /*00000850*/ BL          0x0000000000000850          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000854*/ CMP         R0, #0 
    /*00000858*/ BEQ         AO_Exit_x8ac
    AO_Exit_x85c:
    /*0000085c*/ MOV         R0, #2 
    /*00000860*/ BL          0x0000000000000860          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000864*/ LDR         R3, [R0, #36]               @ 0x24 
    /*00000868*/ CMP         R3, #0 
    /*0000086c*/ BEQ         AO_Exit_x8cc
    AO_Exit_x870:
    /*00000870*/ MOV         R0, #2 
    /*00000874*/ BL          0x0000000000000874          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000878*/ LDR         R3, [R0, #36]               @ 0x24 
    /*0000087c*/ MOV         R0, #22 
    /*00000880*/ BLX         R3 
    /*00000884*/ MOV         R0, #0 
    /*00000888*/ BL          0x0000000000000888          @ AO_DEV_Exit [R_ARM_CALL: .text+0x49ec] 
    /*0000088c*/ MOV         R0, #1 
    /*00000890*/ BL          0x0000000000000890          @ AO_DEV_Exit [R_ARM_CALL: .text+0x49ec] 
    /*00000894*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000898*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000089c*/ BL          0x000000000000089C          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*000008a0*/ MOV         R3, #2 
    /*000008a4*/ STR         R3, [R4] 
    /*000008a8*/ POP         {R4, PC} 
    AO_Exit_x8ac:
    /*000008ac*/ MOVW        R3, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x31c] 
    /*000008b0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*000008b4*/ MOVT        R3, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x31c] 
    /*000008b8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*000008bc*/ MOVW        R2, #1548                   @ 0x60c 
    /*000008c0*/ LDR         R1, [PC, #36]               @ 0x00000000000008ec 
    /*000008c4*/ BL          0x00000000000008C4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000008c8*/ B           AO_Exit_x85c
    AO_Exit_x8cc:
    /*000008cc*/ MOVW        R3, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x31c] 
    /*000008d0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*000008d4*/ MOVT        R3, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x31c] 
    /*000008d8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*000008dc*/ MOVW        R2, #1551                   @ 0x60f 
    /*000008e0*/ LDR         R1, [PC, #4]                @ 0x00000000000008ec 
    /*000008e4*/ BL          0x00000000000008E4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000008e8*/ B           AO_Exit_x870
    /*000008ec*/ ANDEQ       R0, R0, R0, LSR #32 
FUNC_END AO_Exit


FUNC_BEGIN ao_close
    /*000008f0*/ MOV         R0, #0 
    /*000008f4*/ BX          LR 
FUNC_END ao_close


FUNC_BEGIN ao_freeze
    /*000008f8*/ MOV         R0, #0 
    /*000008fc*/ BX          LR 
FUNC_END ao_freeze


FUNC_BEGIN ao_open
    /*00000900*/ MOV         R0, #0 
    /*00000904*/ BX          LR 
FUNC_END ao_open


FUNC_BEGIN AOEnableChn
    /*00000908*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000090c*/ MOV         R7, R0 
    /*00000910*/ SUB         SP, SP, #44                 @ 0x2c 
    /*00000914*/ MOV         R0, #20 
    /*00000918*/ MOV         R4, R1 
    /*0000091c*/ BL          0x000000000000091C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000920*/ CMP         R0, #0 
    /*00000924*/ BEQ         AOEnableChn_xc38
    /*00000928*/ CMP         R7, #1 
    /*0000092c*/ BHI         AOEnableChn_xb3c
    /*00000930*/ CMP         R4, #2 
    /*00000934*/ BHI         AOEnableChn_xb6c
    /*00000938*/ MOV         R0, R7 
    /*0000093c*/ BL          0x000000000000093C          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*00000940*/ SUBS        R8, R0, #0 
    /*00000944*/ BEQ         AOEnableChn_xb1c
    AOEnableChn_x948:
    /*00000948*/ ADD         R3, R8, #124                @ 0x7c 
    /*0000094c*/ CMP         R4, #2 
    /*00000950*/ STR         R3, [SP, #20] 
    /*00000954*/ MOV         R0, R3 
    /*00000958*/ LDRNE       R6, [R8, #28] 
    /*0000095c*/ MOVEQ       R6, #0 
    /*00000960*/ BL          0x0000000000000960          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000964*/ SUBS        R5, R0, #0 
    /*00000968*/ BNE         AOEnableChn_xbd4
    /*0000096c*/ LDR         R3, [R8, #12] 
    /*00000970*/ CMP         R3, #1 
    /*00000974*/ BNE         AOEnableChn_xb9c
    /*00000978*/ CMP         R4, #2 
    /*0000097c*/ BEQ         AOEnableChn_xa70
    /*00000980*/ CMP         R6, #1 
    /*00000984*/ LDR         R6, [R8, #44]               @ 0x2c 
    /*00000988*/ MOVNE       R9, #1 
    /*0000098c*/ MOVEQ       R9, #2 
    /*00000990*/ MOV         R0, R6 
    /*00000994*/ MOV         R1, R9 
    /*00000998*/ BL          0x0000000000000998          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000099c*/ CMP         R4, R0 
    /*000009a0*/ BGE         AOEnableChn_xbe0
    /*000009a4*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*000009a8*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*000009ac*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*000009b0*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*000009b4*/ LDR         SL, [PC, #648]              @ 0x0000000000000c44 
    /*000009b8*/ ADD         FP, R7, R7, LSL #1 
    /*000009bc*/ STR         R3, [SP, #24] 
    /*000009c0*/ STR         R2, [SP, #28] 
    /*000009c4*/ B           AOEnableChn_xa3c
    AOEnableChn_x9c8:
    /*000009c8*/ MOV         R0, R6 
    /*000009cc*/ BL          0x00000000000009CC          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000009d0*/ ADD         R1, SP, #36                 @ 0x24 
    /*000009d4*/ CMP         R0, #0 
    /*000009d8*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000009dc*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000009e0*/ BNE         AOEnableChn_xb04
    /*000009e4*/ BL          0x00000000000009E4          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000009e8*/ MOV         R3, #1 
    /*000009ec*/ ADD         R1, SP, #36                 @ 0x24 
    /*000009f0*/ STR         R3, [R6, #1240]             @ 0x4d8 
    /*000009f4*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000009f8*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000009fc*/ BL          0x00000000000009FC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000a00*/ MOV         R0, R6 
    /*00000a04*/ BL          0x0000000000000A04          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000a08*/ MOV         R3, #239                    @ 0xef 
    /*00000a0c*/ STR         R4, [SP, #8] 
    /*00000a10*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x364] 
    /*00000a14*/ STR         R3, [SP] 
    /*00000a18*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x364] 
    /*00000a1c*/ STR         R7, [SP, #4] 
    /*00000a20*/ MOV         R3, SL 
    /*00000a24*/ MOV         R1, #22 
    /*00000a28*/ MOV         R0, #6 
    /*00000a2c*/ BL          0x0000000000000A2C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000a30*/ CMP         R9, R5 
    /*00000a34*/ BEQ         AOEnableChn_xaf0
    /*00000a38*/ LDR         R6, [R8, #44]               @ 0x2c 
    AOEnableChn_xa3c:
    /*00000a3c*/ LSR         R6, R6, #1 
    /*00000a40*/ MLA         R4, R5, R6, R4 
    /*00000a44*/ ADD         R5, R5, #1 
    /*00000a48*/ ADD         R0, R4, FP 
    /*00000a4c*/ BL          0x0000000000000A4C          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00000a50*/ SUBS        R6, R0, #0 
    /*00000a54*/ BNE         AOEnableChn_x9c8
    /*00000a58*/ LDR         R3, [SP, #24] 
    /*00000a5c*/ MOV         R2, #227                    @ 0xe3 
    /*00000a60*/ MOV         R1, SL 
    /*00000a64*/ LDR         R0, [SP, #28] 
    /*00000a68*/ BL          0x0000000000000A68          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000a6c*/ B           AOEnableChn_x9c8
    AOEnableChn_xa70:
    /*00000a70*/ ADD         R0, R7, R7, LSL #1 
    /*00000a74*/ ADD         R0, R0, #2 
    /*00000a78*/ BL          0x0000000000000A78          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00000a7c*/ SUBS        R4, R0, #0 
    /*00000a80*/ BEQ         AOEnableChn_xc18
    AOEnableChn_xa84:
    /*00000a84*/ MOV         R0, R4 
    /*00000a88*/ BL          0x0000000000000A88          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000a8c*/ CMP         R0, #0 
    /*00000a90*/ BNE         AOEnableChn_xb04
    /*00000a94*/ ADD         R1, SP, #36                 @ 0x24 
    /*00000a98*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000a9c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000aa0*/ BL          0x0000000000000AA0          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000aa4*/ MOV         R3, #1 
    /*00000aa8*/ ADD         R1, SP, #36                 @ 0x24 
    /*00000aac*/ STR         R3, [R4, #1240]             @ 0x4d8 
    /*00000ab0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000ab4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000ab8*/ BL          0x0000000000000AB8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000abc*/ MOV         R0, R4 
    /*00000ac0*/ BL          0x0000000000000AC0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000ac4*/ MOV         R2, #2 
    /*00000ac8*/ MOV         R3, #209                    @ 0xd1 
    /*00000acc*/ STR         R2, [SP, #8] 
    /*00000ad0*/ STR         R3, [SP] 
    /*00000ad4*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x364] 
    /*00000ad8*/ STR         R7, [SP, #4] 
    /*00000adc*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x364] 
    /*00000ae0*/ LDR         R3, [PC, #348]              @ 0x0000000000000c44 
    /*00000ae4*/ MOV         R1, #22 
    /*00000ae8*/ MOV         R0, #6 
    /*00000aec*/ BL          0x0000000000000AEC          @ HI_LOG [R_ARM_CALL: +0x0] 
    AOEnableChn_xaf0:
    /*00000af0*/ LDR         R0, [SP, #20] 
    /*00000af4*/ BL          0x0000000000000AF4          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000af8*/ MOV         R0, #0 
    AOEnableChn_xafc:
    /*00000afc*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00000b00*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOEnableChn_xb04:
    /*00000b04*/ LDR         R0, [SP, #20] 
    /*00000b08*/ BL          0x0000000000000B08          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000b0c*/ MOV         R0, #65024                  @ 0xfe00 
    /*00000b10*/ MOVT        R0, #65535                  @ 0xffff 
    /*00000b14*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00000b18*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOEnableChn_xb1c:
    /*00000b1c*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*00000b20*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00000b24*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*00000b28*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00000b2c*/ MOV         R2, #174                    @ 0xae 
    /*00000b30*/ LDR         R1, [PC, #268]              @ 0x0000000000000c44 
    /*00000b34*/ BL          0x0000000000000B34          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000b38*/ B           AOEnableChn_x948
    AOEnableChn_xb3c:
    /*00000b3c*/ MOV         R3, #170                    @ 0xaa 
    /*00000b40*/ STR         R7, [SP, #4] 
    /*00000b44*/ STR         R3, [SP] 
    /*00000b48*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*00000b4c*/ LDR         R3, [PC, #240]              @ 0x0000000000000c44 
    /*00000b50*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00000b54*/ MOV         R1, #22 
    /*00000b58*/ MOV         R0, #3 
    /*00000b5c*/ BL          0x0000000000000B5C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000b60*/ MOVW        R0, #32769                  @ 0x8001 
    /*00000b64*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000b68*/ B           AOEnableChn_xafc
    AOEnableChn_xb6c:
    /*00000b6c*/ MOV         R3, #171                    @ 0xab 
    /*00000b70*/ STR         R4, [SP, #4] 
    /*00000b74*/ STR         R3, [SP] 
    /*00000b78*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*00000b7c*/ LDR         R3, [PC, #192]              @ 0x0000000000000c44 
    /*00000b80*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*00000b84*/ MOV         R1, #20 
    /*00000b88*/ MOV         R0, #3 
    /*00000b8c*/ BL          0x0000000000000B8C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000b90*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000b94*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000b98*/ B           AOEnableChn_xafc
    AOEnableChn_xb9c:
    /*00000b9c*/ LDR         R0, [SP, #20] 
    /*00000ba0*/ BL          0x0000000000000BA0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000ba4*/ MOV         R3, #188                    @ 0xbc 
    /*00000ba8*/ STR         R7, [SP, #4] 
    /*00000bac*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x330] 
    /*00000bb0*/ STR         R3, [SP] 
    /*00000bb4*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x330] 
    /*00000bb8*/ LDR         R3, [PC, #132]              @ 0x0000000000000c44 
    /*00000bbc*/ MOV         R1, #22 
    /*00000bc0*/ MOV         R0, #3 
    /*00000bc4*/ BL          0x0000000000000BC4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000bc8*/ MOVW        R0, #32773                  @ 0x8005 
    /*00000bcc*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000bd0*/ B           AOEnableChn_xafc
    AOEnableChn_xbd4:
    /*00000bd4*/ MOV         R0, #65024                  @ 0xfe00 
    /*00000bd8*/ MOVT        R0, #65535                  @ 0xffff 
    /*00000bdc*/ B           AOEnableChn_xafc
    AOEnableChn_xbe0:
    /*00000be0*/ LDR         R0, [SP, #20] 
    /*00000be4*/ BL          0x0000000000000BE4          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000be8*/ MOV         R3, #218                    @ 0xda 
    /*00000bec*/ STMIB       SP, {R4, R7} 
    /*00000bf0*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x39c] 
    /*00000bf4*/ STR         R3, [SP] 
    /*00000bf8*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x39c] 
    /*00000bfc*/ LDR         R3, [PC, #64]               @ 0x0000000000000c44 
    /*00000c00*/ MOV         R1, #22 
    /*00000c04*/ MOV         R0, #3 
    /*00000c08*/ BL          0x0000000000000C08          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000c0c*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000c10*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000c14*/ B           AOEnableChn_xafc
    AOEnableChn_xc18:
    /*00000c18*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*00000c1c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00000c20*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00000c24*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00000c28*/ MOV         R2, #197                    @ 0xc5 
    /*00000c2c*/ LDR         R1, [PC, #16]               @ 0x0000000000000c44 
    /*00000c30*/ BL          0x0000000000000C30          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000c34*/ B           AOEnableChn_xa84
    AOEnableChn_xc38:
    /*00000c38*/ MOVW        R0, #32784                  @ 0x8010 
    /*00000c3c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000c40*/ B           AOEnableChn_xafc
    /*00000c44*/ ANDEQ       R0, R0, R8, LSR #32 
FUNC_END AOEnableChn


FUNC_BEGIN AODisableChn
    /*00000c48*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000c4c*/ MOV         R9, R0 
    /*00000c50*/ SUB         SP, SP, #44                 @ 0x2c 
    /*00000c54*/ MOV         R0, #20 
    /*00000c58*/ MOV         R6, R1 
    /*00000c5c*/ BL          0x0000000000000C5C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000c60*/ CMP         R0, #0 
    /*00000c64*/ BEQ         AODisableChn_xec8
    /*00000c68*/ CMP         R9, #1 
    /*00000c6c*/ BHI         AODisableChn_xe68
    /*00000c70*/ CMP         R6, #2 
    /*00000c74*/ BHI         AODisableChn_xe98
    /*00000c78*/ MOV         R0, R9 
    /*00000c7c*/ BL          0x0000000000000C7C          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*00000c80*/ SUBS        R8, R0, #0 
    /*00000c84*/ BEQ         AODisableChn_xe48
    AODisableChn_xc88:
    /*00000c88*/ CMP         R6, #2 
    /*00000c8c*/ ADD         R3, R8, #124                @ 0x7c 
    /*00000c90*/ STR         R3, [SP, #20] 
    /*00000c94*/ BEQ         AODisableChn_xdec
    /*00000c98*/ MOV         R0, R3 
    /*00000c9c*/ LDR         R4, [R8, #28] 
    /*00000ca0*/ BL          0x0000000000000CA0          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000ca4*/ CMP         R0, #0 
    /*00000ca8*/ BNE         AODisableChn_xe0c
    /*00000cac*/ CMP         R4, #1 
    /*00000cb0*/ MOVNE       SL, #1 
    /*00000cb4*/ MOVEQ       SL, #2 
    AODisableChn_xcb8:
    /*00000cb8*/ LDR         R4, [R8, #44]               @ 0x2c 
    /*00000cbc*/ MOV         R1, SL 
    /*00000cc0*/ MOV         R0, R4 
    /*00000cc4*/ BL          0x0000000000000CC4          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*00000cc8*/ CMP         R6, R0 
    /*00000ccc*/ BGE         AODisableChn_xdd8
    /*00000cd0*/ LDR         R3, [R8, #12] 
    /*00000cd4*/ CMP         R3, #1 
    /*00000cd8*/ BNE         AODisableChn_xdd8
    /*00000cdc*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*00000ce0*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00000ce4*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00000ce8*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00000cec*/ STR         R3, [SP, #24] 
    /*00000cf0*/ MOV         R7, #0 
    /*00000cf4*/ ADD         R3, R9, R9, LSL #1 
    /*00000cf8*/ LDR         FP, [PC, #468]              @ 0x0000000000000ed4 
    /*00000cfc*/ STR         R2, [SP, #28] 
    /*00000d00*/ STR         R3, [SP, #16] 
    /*00000d04*/ B           AODisableChn_xda0
    AODisableChn_xd08:
    /*00000d08*/ MOV         R0, R5 
    /*00000d0c*/ BL          0x0000000000000D0C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000d10*/ ADD         R1, SP, #36                 @ 0x24 
    /*00000d14*/ SUBS        R3, R0, #0 
    /*00000d18*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000d1c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000d20*/ BNE         AODisableChn_xe04
    /*00000d24*/ LDR         R4, [R5, #1240]             @ 0x4d8 
    /*00000d28*/ CMP         R4, #0 
    /*00000d2c*/ CMPNE       R4, #3 
    /*00000d30*/ MOVEQ       R4, #1 
    /*00000d34*/ MOVNE       R4, #0 
    /*00000d38*/ BEQ         AODisableChn_xe1c
    /*00000d3c*/ BL          0x0000000000000D3C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000d40*/ MOV         R3, #3 
    /*00000d44*/ ADD         R1, SP, #36                 @ 0x24 
    /*00000d48*/ STR         R3, [R5, #1240]             @ 0x4d8 
    /*00000d4c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000d50*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000d54*/ BL          0x0000000000000D54          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000d58*/ ADD         R0, R8, #120                @ 0x78 
    /*00000d5c*/ STR         R4, [R5, #2372]             @ 0x944 
    /*00000d60*/ BL          0x0000000000000D60          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00000d64*/ MOV         R0, R5 
    /*00000d68*/ BL          0x0000000000000D68          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000d6c*/ MOVW        R3, #315                    @ 0x13b 
    /*00000d70*/ STR         R6, [SP, #8] 
    /*00000d74*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x404] 
    /*00000d78*/ STR         R3, [SP] 
    /*00000d7c*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x404] 
    /*00000d80*/ STR         R9, [SP, #4] 
    /*00000d84*/ MOV         R3, FP 
    /*00000d88*/ MOV         R1, #22 
    /*00000d8c*/ MOV         R0, #6 
    /*00000d90*/ BL          0x0000000000000D90          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000d94*/ CMP         SL, R7 
    /*00000d98*/ BEQ         AODisableChn_xdd8
    /*00000d9c*/ LDR         R4, [R8, #44]               @ 0x2c 
    AODisableChn_xda0:
    /*00000da0*/ LSR         R4, R4, #1 
    /*00000da4*/ LDR         R3, [SP, #16] 
    /*00000da8*/ MLA         R6, R7, R4, R6 
    /*00000dac*/ ADD         R7, R7, #1 
    /*00000db0*/ ADD         R0, R6, R3 
    /*00000db4*/ BL          0x0000000000000DB4          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00000db8*/ SUBS        R5, R0, #0 
    /*00000dbc*/ BNE         AODisableChn_xd08
    /*00000dc0*/ LDR         R3, [SP, #24] 
    /*00000dc4*/ MOVW        R2, #289                    @ 0x121 
    /*00000dc8*/ MOV         R1, FP 
    /*00000dcc*/ LDR         R0, [SP, #28] 
    /*00000dd0*/ BL          0x0000000000000DD0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000dd4*/ B           AODisableChn_xd08
    AODisableChn_xdd8:
    /*00000dd8*/ LDR         R0, [SP, #20] 
    /*00000ddc*/ BL          0x0000000000000DDC          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000de0*/ MOV         R0, #0 
    AODisableChn_xde4:
    /*00000de4*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00000de8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AODisableChn_xdec:
    /*00000dec*/ MOV         R0, R3 
    /*00000df0*/ BL          0x0000000000000DF0          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000df4*/ CMP         R0, #0 
    /*00000df8*/ BNE         AODisableChn_xe0c
    /*00000dfc*/ MOV         SL, #1 
    /*00000e00*/ B           AODisableChn_xcb8
    AODisableChn_xe04:
    /*00000e04*/ LDR         R0, [SP, #20] 
    /*00000e08*/ BL          0x0000000000000E08          @ osal_up [R_ARM_CALL: +0x0] 
    AODisableChn_xe0c:
    /*00000e0c*/ MOV         R0, #65024                  @ 0xfe00 
    /*00000e10*/ MOVT        R0, #65535                  @ 0xffff 
    /*00000e14*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00000e18*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AODisableChn_xe1c:
    /*00000e1c*/ MOV         R2, #3 
    /*00000e20*/ MOV         R0, R5 
    /*00000e24*/ STR         R2, [R5, #1240]             @ 0x4d8 
    /*00000e28*/ STR         R3, [SP, #16] 
    /*00000e2c*/ BL          0x0000000000000E2C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000e30*/ LDR         R0, [SP, #20] 
    /*00000e34*/ BL          0x0000000000000E34          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000e38*/ LDR         R3, [SP, #16] 
    /*00000e3c*/ MOV         R0, R3 
    /*00000e40*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00000e44*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AODisableChn_xe48:
    /*00000e48*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*00000e4c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00000e50*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*00000e54*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00000e58*/ MOV         R2, #260                    @ 0x104 
    /*00000e5c*/ LDR         R1, [PC, #112]              @ 0x0000000000000ed4 
    /*00000e60*/ BL          0x0000000000000E60          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000e64*/ B           AODisableChn_xc88
    AODisableChn_xe68:
    /*00000e68*/ MOV         R3, #256                    @ 0x100 
    /*00000e6c*/ STR         R9, [SP, #4] 
    /*00000e70*/ STR         R3, [SP] 
    /*00000e74*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*00000e78*/ LDR         R3, [PC, #84]               @ 0x0000000000000ed4 
    /*00000e7c*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00000e80*/ MOV         R1, #22 
    /*00000e84*/ MOV         R0, #3 
    /*00000e88*/ BL          0x0000000000000E88          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000e8c*/ MOVW        R0, #32769                  @ 0x8001 
    /*00000e90*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000e94*/ B           AODisableChn_xde4
    AODisableChn_xe98:
    /*00000e98*/ MOVW        R3, #257                    @ 0x101 
    /*00000e9c*/ STR         R6, [SP, #4] 
    /*00000ea0*/ STR         R3, [SP] 
    /*00000ea4*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*00000ea8*/ LDR         R3, [PC, #36]               @ 0x0000000000000ed4 
    /*00000eac*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*00000eb0*/ MOV         R1, #20 
    /*00000eb4*/ MOV         R0, #3 
    /*00000eb8*/ BL          0x0000000000000EB8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000ebc*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000ec0*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000ec4*/ B           AODisableChn_xde4
    AODisableChn_xec8:
    /*00000ec8*/ MOVW        R0, #32784                  @ 0x8010 
    /*00000ecc*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000ed0*/ B           AODisableChn_xde4
    /*00000ed4*/ ANDEQ       R0, R0, R4, LSR R0 
FUNC_END AODisableChn


FUNC_BEGIN AOPauseChn
    /*00000ed8*/ CMP         R0, #1 
    /*00000edc*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00000ee0*/ MOV         R4, R0 
    /*00000ee4*/ SUB         SP, SP, #28 
    /*00000ee8*/ BHI         AOPauseChn_x104c
    /*00000eec*/ CMP         R1, #2 
    /*00000ef0*/ MOV         R5, R1 
    /*00000ef4*/ BHI         AOPauseChn_x107c
    /*00000ef8*/ ADD         R7, R0, R0, LSL #1 
    /*00000efc*/ ADD         R7, R7, R1 
    /*00000f00*/ MOV         R0, R7 
    /*00000f04*/ BL          0x0000000000000F04          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00000f08*/ MOV         R6, R0 
    /*00000f0c*/ MOV         R0, R4 
    /*00000f10*/ BL          0x0000000000000F10          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*00000f14*/ CMP         R6, #0 
    /*00000f18*/ MOV         R8, R0 
    /*00000f1c*/ BEQ         AOPauseChn_x102c
    AOPauseChn_xf20:
    /*00000f20*/ CMP         R8, #0 
    /*00000f24*/ BEQ         AOPauseChn_x100c
    AOPauseChn_xf28:
    /*00000f28*/ CMP         R5, #2 
    /*00000f2c*/ BEQ         AOPauseChn_xfd8
    /*00000f30*/ LDR         R9, [R8, #28] 
    /*00000f34*/ LDR         R0, [R8, #44]               @ 0x2c 
    /*00000f38*/ CMP         R9, #1 
    /*00000f3c*/ MOVEQ       R1, #2 
    /*00000f40*/ MOVNE       R1, #1 
    /*00000f44*/ BL          0x0000000000000F44          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*00000f48*/ CMP         R5, R0 
    /*00000f4c*/ BGE         AOPauseChn_x110c
    AOPauseChn_xf50:
    /*00000f50*/ MOV         R0, R6 
    /*00000f54*/ BL          0x0000000000000F54          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000f58*/ CMP         R0, #0 
    /*00000f5c*/ BNE         AOPauseChn_x10ac
    /*00000f60*/ LDR         R3, [R6, #1240]             @ 0x4d8 
    /*00000f64*/ CMP         R3, #0 
    /*00000f68*/ CMPNE       R3, #3 
    /*00000f6c*/ BEQ         AOPauseChn_x10b8
    /*00000f70*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000f74*/ ADD         R1, SP, #20 
    /*00000f78*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000f7c*/ BL          0x0000000000000F7C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000f80*/ CMP         R9, #1 
    /*00000f84*/ MOV         R3, #2 
    /*00000f88*/ STR         R3, [R6, #1240]             @ 0x4d8 
    /*00000f8c*/ BEQ         AOPauseChn_xfe0
    AOPauseChn_xf90:
    /*00000f90*/ ADD         R1, SP, #20 
    /*00000f94*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000f98*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000f9c*/ BL          0x0000000000000F9C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000fa0*/ MOV         R0, R6 
    /*00000fa4*/ BL          0x0000000000000FA4          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000fa8*/ MOVW        R3, #383                    @ 0x17f 
    /*00000fac*/ STMIB       SP, {R4, R5} 
    /*00000fb0*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x498] 
    /*00000fb4*/ STR         R3, [SP] 
    /*00000fb8*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x498] 
    /*00000fbc*/ LDR         R3, [PC, #380]              @ 0x0000000000001140 
    /*00000fc0*/ MOV         R1, #22 
    /*00000fc4*/ MOV         R0, #6 
    /*00000fc8*/ BL          0x0000000000000FC8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000fcc*/ MOV         R0, #0 
    AOPauseChn_xfd0:
    /*00000fd0*/ ADD         SP, SP, #28 
    /*00000fd4*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AOPauseChn_xfd8:
    /*00000fd8*/ MOV         R9, #0 
    /*00000fdc*/ B           AOPauseChn_xf50
    AOPauseChn_xfe0:
    /*00000fe0*/ LDR         R0, [R8, #44]               @ 0x2c 
    /*00000fe4*/ LSR         R0, R0, #1 
    /*00000fe8*/ CMP         R5, R0 
    /*00000fec*/ ADDCC       R0, R7, R0 
    /*00000ff0*/ SUBCS       R0, R7, R0 
    /*00000ff4*/ BL          0x0000000000000FF4          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00000ff8*/ SUBS        R7, R0, #0 
    /*00000ffc*/ BEQ         AOPauseChn_x10ec
    AOPauseChn_x1000:
    /*00001000*/ MOV         R3, #2 
    /*00001004*/ STR         R3, [R7, #1240]             @ 0x4d8 
    /*00001008*/ B           AOPauseChn_xf90
    AOPauseChn_x100c:
    /*0000100c*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*00001010*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00001014*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*00001018*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*0000101c*/ MOVW        R2, #338                    @ 0x152 
    /*00001020*/ LDR         R1, [PC, #280]              @ 0x0000000000001140 
    /*00001024*/ BL          0x0000000000001024          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001028*/ B           AOPauseChn_xf28
    AOPauseChn_x102c:
    /*0000102c*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*00001030*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00001034*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00001038*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*0000103c*/ MOVW        R2, #337                    @ 0x151 
    /*00001040*/ LDR         R1, [PC, #248]              @ 0x0000000000001140 
    /*00001044*/ BL          0x0000000000001044          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001048*/ B           AOPauseChn_xf20
    AOPauseChn_x104c:
    /*0000104c*/ STR         R0, [SP, #4] 
    /*00001050*/ MOVW        R3, #331                    @ 0x14b 
    /*00001054*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*00001058*/ STR         R3, [SP] 
    /*0000105c*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00001060*/ LDR         R3, [PC, #216]              @ 0x0000000000001140 
    /*00001064*/ MOV         R1, #22 
    /*00001068*/ MOV         R0, #3 
    /*0000106c*/ BL          0x000000000000106C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001070*/ MOVW        R0, #32769                  @ 0x8001 
    /*00001074*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001078*/ B           AOPauseChn_xfd0
    AOPauseChn_x107c:
    /*0000107c*/ STR         R1, [SP, #4] 
    /*00001080*/ MOV         R3, #332                    @ 0x14c 
    /*00001084*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*00001088*/ STR         R3, [SP] 
    /*0000108c*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*00001090*/ LDR         R3, [PC, #168]              @ 0x0000000000001140 
    /*00001094*/ MOV         R1, #20 
    /*00001098*/ MOV         R0, #3 
    /*0000109c*/ BL          0x000000000000109C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000010a0*/ MOVW        R0, #32770                  @ 0x8002 
    /*000010a4*/ MOVT        R0, #40982                  @ 0xa016 
    /*000010a8*/ B           AOPauseChn_xfd0
    AOPauseChn_x10ac:
    /*000010ac*/ MOV         R0, #65024                  @ 0xfe00 
    /*000010b0*/ MOVT        R0, #65535                  @ 0xffff 
    /*000010b4*/ B           AOPauseChn_xfd0
    AOPauseChn_x10b8:
    /*000010b8*/ MOV         R0, R6 
    /*000010bc*/ BL          0x00000000000010BC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000010c0*/ MOV         R3, #356                    @ 0x164 
    /*000010c4*/ MOVW        R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x43c] 
    /*000010c8*/ STR         R3, [SP] 
    /*000010cc*/ MOVT        R2, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x43c] 
    /*000010d0*/ LDR         R3, [PC, #104]              @ 0x0000000000001140 
    /*000010d4*/ MOV         R1, #22 
    /*000010d8*/ MOV         R0, #3 
    /*000010dc*/ BL          0x00000000000010DC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000010e0*/ MOVW        R0, #32777                  @ 0x8009 
    /*000010e4*/ MOVT        R0, #40982                  @ 0xa016 
    /*000010e8*/ B           AOPauseChn_xfd0
    AOPauseChn_x10ec:
    /*000010ec*/ MOVW        R3, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x480] 
    /*000010f0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*000010f4*/ MOVT        R3, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x480] 
    /*000010f8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*000010fc*/ MOV         R2, #376                    @ 0x178 
    /*00001100*/ LDR         R1, [PC, #56]               @ 0x0000000000001140 
    /*00001104*/ BL          0x0000000000001104          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001108*/ B           AOPauseChn_x1000
    AOPauseChn_x110c:
    /*0000110c*/ MOV         R3, #344                    @ 0x158 
    /*00001110*/ STR         R4, [SP, #8] 
    /*00001114*/ STR         R3, [SP] 
    /*00001118*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x39c] 
    /*0000111c*/ STR         R5, [SP, #4] 
    /*00001120*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x39c] 
    /*00001124*/ LDR         R3, [PC, #20]               @ 0x0000000000001140 
    /*00001128*/ MOV         R1, #20 
    /*0000112c*/ MOV         R0, #3 
    /*00001130*/ BL          0x0000000000001130          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001134*/ MOVW        R0, #32770                  @ 0x8002 
    /*00001138*/ MOVT        R0, #40982                  @ 0xa016 
    /*0000113c*/ B           AOPauseChn_xfd0
    /*00001140*/ ANDEQ       R0, R0, R4, ASR #32 
FUNC_END AOPauseChn


FUNC_BEGIN AOResumeChn
    /*00001144*/ CMP         R0, #1 
    /*00001148*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*0000114c*/ ADD         R8, R0, R0, LSL #1 
    /*00001150*/ SUB         SP, SP, #28 
    /*00001154*/ MOV         R4, R0 
    /*00001158*/ ADD         R8, R8, R1 
    /*0000115c*/ BHI         AOResumeChn_x12b4
    /*00001160*/ CMP         R1, #2 
    /*00001164*/ MOV         R5, R1 
    /*00001168*/ BHI         AOResumeChn_x12e4
    /*0000116c*/ MOV         R0, R8 
    /*00001170*/ BL          0x0000000000001170          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00001174*/ MOV         R6, R0 
    /*00001178*/ MOV         R0, R4 
    /*0000117c*/ BL          0x000000000000117C          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*00001180*/ CMP         R6, #0 
    /*00001184*/ MOV         R7, R0 
    /*00001188*/ BEQ         AOResumeChn_x1274
    AOResumeChn_x118c:
    /*0000118c*/ CMP         R7, #0 
    /*00001190*/ BEQ         AOResumeChn_x1294
    AOResumeChn_x1194:
    /*00001194*/ CMP         R5, #2 
    /*00001198*/ BEQ         AOResumeChn_x124c
    /*0000119c*/ LDR         R9, [R7, #28] 
    /*000011a0*/ LDR         R0, [R7, #44]               @ 0x2c 
    /*000011a4*/ CMP         R9, #1 
    /*000011a8*/ MOVEQ       R1, #2 
    /*000011ac*/ MOVNE       R1, #1 
    /*000011b0*/ BL          0x00000000000011B0          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*000011b4*/ CMP         R5, R0 
    /*000011b8*/ BGE         AOResumeChn_x1354
    AOResumeChn_x11bc:
    /*000011bc*/ MOV         R0, R6 
    /*000011c0*/ BL          0x00000000000011C0          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000011c4*/ CMP         R0, #0 
    /*000011c8*/ BNE         AOResumeChn_x1314
    /*000011cc*/ LDR         R3, [R6, #1240]             @ 0x4d8 
    /*000011d0*/ CMP         R3, #0 
    /*000011d4*/ CMPNE       R3, #3 
    /*000011d8*/ BEQ         AOResumeChn_x1320
    /*000011dc*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000011e0*/ ADD         R1, SP, #20 
    /*000011e4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000011e8*/ BL          0x00000000000011E8          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000011ec*/ CMP         R9, #1 
    /*000011f0*/ MOV         R3, #1 
    /*000011f4*/ STR         R3, [R6, #1240]             @ 0x4d8 
    /*000011f8*/ BEQ         AOResumeChn_x1254
    AOResumeChn_x11fc:
    /*000011fc*/ ADD         R1, SP, #20 
    /*00001200*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001204*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001208*/ BL          0x0000000000001208          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000120c*/ ADD         R0, R7, #120                @ 0x78 
    /*00001210*/ BL          0x0000000000001210          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00001214*/ MOV         R0, R6 
    /*00001218*/ BL          0x0000000000001218          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000121c*/ MOVW        R3, #441                    @ 0x1b9 
    /*00001220*/ STMIB       SP, {R4, R5} 
    /*00001224*/ MOVW        R2, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x534] 
    /*00001228*/ STR         R3, [SP] 
    /*0000122c*/ MOVT        R2, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x534] 
    /*00001230*/ LDR         R3, [PC, #368]              @ 0x00000000000013a8 
    /*00001234*/ MOV         R1, #22 
    /*00001238*/ MOV         R0, #6 
    /*0000123c*/ BL          0x000000000000123C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001240*/ MOV         R0, #0 
    AOResumeChn_x1244:
    /*00001244*/ ADD         SP, SP, #28 
    /*00001248*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AOResumeChn_x124c:
    /*0000124c*/ MOV         R9, #0 
    /*00001250*/ B           AOResumeChn_x11bc
    AOResumeChn_x1254:
    /*00001254*/ LDR         R0, [R7, #44]               @ 0x2c 
    /*00001258*/ ADD         R0, R8, R0, LSR R3 
    /*0000125c*/ BL          0x000000000000125C          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00001260*/ SUBS        R8, R0, #0 
    /*00001264*/ BEQ         AOResumeChn_x1388
    AOResumeChn_x1268:
    /*00001268*/ MOV         R3, #1 
    /*0000126c*/ STR         R3, [R8, #1240]             @ 0x4d8 
    /*00001270*/ B           AOResumeChn_x11fc
    AOResumeChn_x1274:
    /*00001274*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*00001278*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*0000127c*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00001280*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00001284*/ MOVW        R2, #401                    @ 0x191 
    /*00001288*/ LDR         R1, [PC, #280]              @ 0x00000000000013a8 
    /*0000128c*/ BL          0x000000000000128C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001290*/ B           AOResumeChn_x118c
    AOResumeChn_x1294:
    /*00001294*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*00001298*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*0000129c*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*000012a0*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*000012a4*/ MOVW        R2, #402                    @ 0x192 
    /*000012a8*/ LDR         R1, [PC, #248]              @ 0x00000000000013a8 
    /*000012ac*/ BL          0x00000000000012AC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000012b0*/ B           AOResumeChn_x1194
    AOResumeChn_x12b4:
    /*000012b4*/ STR         R0, [SP, #4] 
    /*000012b8*/ MOV         R3, #396                    @ 0x18c 
    /*000012bc*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*000012c0*/ STR         R3, [SP] 
    /*000012c4*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*000012c8*/ LDR         R3, [PC, #216]              @ 0x00000000000013a8 
    /*000012cc*/ MOV         R1, #22 
    /*000012d0*/ MOV         R0, #3 
    /*000012d4*/ BL          0x00000000000012D4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000012d8*/ MOVW        R0, #32769                  @ 0x8001 
    /*000012dc*/ MOVT        R0, #40982                  @ 0xa016 
    /*000012e0*/ B           AOResumeChn_x1244
    AOResumeChn_x12e4:
    /*000012e4*/ STR         R1, [SP, #4] 
    /*000012e8*/ MOVW        R3, #397                    @ 0x18d 
    /*000012ec*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*000012f0*/ STR         R3, [SP] 
    /*000012f4*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*000012f8*/ LDR         R3, [PC, #168]              @ 0x00000000000013a8 
    /*000012fc*/ MOV         R1, #20 
    /*00001300*/ MOV         R0, #3 
    /*00001304*/ BL          0x0000000000001304          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001308*/ MOVW        R0, #32770                  @ 0x8002 
    /*0000130c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001310*/ B           AOResumeChn_x1244
    AOResumeChn_x1314:
    /*00001314*/ MOV         R0, #65024                  @ 0xfe00 
    /*00001318*/ MOVT        R0, #65535                  @ 0xffff 
    /*0000131c*/ B           AOResumeChn_x1244
    AOResumeChn_x1320:
    /*00001320*/ MOV         R0, R6 
    /*00001324*/ BL          0x0000000000001324          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001328*/ MOVW        R3, #419                    @ 0x1a3 
    /*0000132c*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4cc] 
    /*00001330*/ STR         R3, [SP] 
    /*00001334*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4cc] 
    /*00001338*/ LDR         R3, [PC, #104]              @ 0x00000000000013a8 
    /*0000133c*/ MOV         R1, #22 
    /*00001340*/ MOV         R0, #4 
    /*00001344*/ BL          0x0000000000001344          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001348*/ MOVW        R0, #32777                  @ 0x8009 
    /*0000134c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001350*/ B           AOResumeChn_x1244
    AOResumeChn_x1354:
    /*00001354*/ MOV         R3, #408                    @ 0x198 
    /*00001358*/ STR         R4, [SP, #8] 
    /*0000135c*/ STR         R3, [SP] 
    /*00001360*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x39c] 
    /*00001364*/ STR         R5, [SP, #4] 
    /*00001368*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x39c] 
    /*0000136c*/ LDR         R3, [PC, #52]               @ 0x00000000000013a8 
    /*00001370*/ MOV         R1, #20 
    /*00001374*/ MOV         R0, #3 
    /*00001378*/ BL          0x0000000000001378          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000137c*/ MOVW        R0, #32770                  @ 0x8002 
    /*00001380*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001384*/ B           AOResumeChn_x1244
    AOResumeChn_x1388:
    /*00001388*/ MOVW        R3, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x51c] 
    /*0000138c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00001390*/ MOVT        R3, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x51c] 
    /*00001394*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00001398*/ MOVW        R2, #431                    @ 0x1af 
    /*0000139c*/ LDR         R1, [PC, #4]                @ 0x00000000000013a8 
    /*000013a0*/ BL          0x00000000000013A0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000013a4*/ B           AOResumeChn_x1268
    /*000013a8*/ ANDEQ       R0, R0, R0, ASR R0 
FUNC_END AOResumeChn


FUNC_BEGIN AOClearChnBuf
    /*000013ac*/ CMP         R0, #1 
    /*000013b0*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000013b4*/ MOV         SL, R0 
    /*000013b8*/ SUB         SP, SP, #36                 @ 0x24 
    /*000013bc*/ BHI         AOClearChnBuf_x15a8
    /*000013c0*/ CMP         R1, #2 
    /*000013c4*/ MOV         R9, R1 
    /*000013c8*/ BHI         AOClearChnBuf_x15d8
    /*000013cc*/ BL          0x00000000000013CC          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*000013d0*/ SUBS        R6, R0, #0 
    /*000013d4*/ BEQ         AOClearChnBuf_x1588
    AOClearChnBuf_x13d8:
    /*000013d8*/ CMP         R9, #2 
    /*000013dc*/ BEQ         AOClearChnBuf_x14d8
    /*000013e0*/ LDR         R3, [R6, #28] 
    /*000013e4*/ LDR         R4, [R6, #44]               @ 0x2c 
    /*000013e8*/ CMP         R3, #1 
    /*000013ec*/ MOVEQ       R8, #2 
    /*000013f0*/ MOVNE       R8, #1 
    /*000013f4*/ MOV         R0, R4 
    /*000013f8*/ MOV         R1, R8 
    /*000013fc*/ BL          0x00000000000013FC          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*00001400*/ CMP         R9, R0 
    /*00001404*/ BGE         AOClearChnBuf_x1608
    AOClearChnBuf_x1408:
    /*00001408*/ ADD         R7, SL, SL, LSL #1 
    /*0000140c*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*00001410*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00001414*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00001418*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*0000141c*/ ADD         R7, R7, R9 
    /*00001420*/ STR         R3, [SP, #16] 
    /*00001424*/ MOV         R5, #0 
    /*00001428*/ STR         R2, [SP, #20] 
    /*0000142c*/ MOV         R0, R4 
    /*00001430*/ B           AOClearChnBuf_x14ac
    AOClearChnBuf_x1434:
    /*00001434*/ MOV         R0, R4 
    /*00001438*/ BL          0x0000000000001438          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*0000143c*/ ADD         R1, SP, #28 
    /*00001440*/ SUBS        FP, R0, #0 
    /*00001444*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001448*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000144c*/ BNE         AOClearChnBuf_x14f0
    /*00001450*/ LDR         R3, [R4, #1244]             @ 0x4dc 
    /*00001454*/ CMP         R3, #1 
    /*00001458*/ BNE         AOClearChnBuf_x1500
    /*0000145c*/ LDR         R3, [R4, #1240]             @ 0x4d8 
    /*00001460*/ SUB         R3, R3, #1 
    /*00001464*/ CMP         R3, #1 
    /*00001468*/ BHI         AOClearChnBuf_x153c
    /*0000146c*/ BL          0x000000000000146C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001470*/ ADD         R1, SP, #28 
    /*00001474*/ STR         FP, [R4, #1224]             @ 0x4c8 
    /*00001478*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000147c*/ STR         FP, [R4, #1228]             @ 0x4cc 
    /*00001480*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001484*/ STR         FP, [R4, #1236]             @ 0x4d4 
    /*00001488*/ ADD         R5, R5, #1 
    /*0000148c*/ BL          0x000000000000148C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001490*/ ADD         R0, R6, #120                @ 0x78 
    /*00001494*/ BL          0x0000000000001494          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00001498*/ MOV         R0, R4 
    /*0000149c*/ BL          0x000000000000149C          @ osal_up [R_ARM_CALL: +0x0] 
    /*000014a0*/ CMP         R8, R5 
    /*000014a4*/ BEQ         AOClearChnBuf_x14e4
    /*000014a8*/ LDR         R0, [R6, #44]               @ 0x2c 
    AOClearChnBuf_x14ac:
    /*000014ac*/ LSR         R0, R0, #1 
    /*000014b0*/ MLA         R0, R5, R0, R7 
    /*000014b4*/ BL          0x00000000000014B4          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*000014b8*/ SUBS        R4, R0, #0 
    /*000014bc*/ BNE         AOClearChnBuf_x1434
    /*000014c0*/ LDR         R3, [SP, #16] 
    /*000014c4*/ MOVW        R2, #470                    @ 0x1d6 
    /*000014c8*/ LDR         R1, [PC, #364]              @ 0x000000000000163c 
    /*000014cc*/ LDR         R0, [SP, #20] 
    /*000014d0*/ BL          0x00000000000014D0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000014d4*/ B           AOClearChnBuf_x1434
    AOClearChnBuf_x14d8:
    /*000014d8*/ LDR         R4, [R6, #44]               @ 0x2c 
    /*000014dc*/ MOV         R8, #1 
    /*000014e0*/ B           AOClearChnBuf_x1408
    AOClearChnBuf_x14e4:
    /*000014e4*/ MOV         R0, FP 
    AOClearChnBuf_x14e8:
    /*000014e8*/ ADD         SP, SP, #36                 @ 0x24 
    /*000014ec*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOClearChnBuf_x14f0:
    /*000014f0*/ MOV         R0, #65024                  @ 0xfe00 
    /*000014f4*/ MOVT        R0, #65535                  @ 0xffff 
    /*000014f8*/ ADD         SP, SP, #36                 @ 0x24 
    /*000014fc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOClearChnBuf_x1500:
    /*00001500*/ MOV         R0, R4 
    /*00001504*/ BL          0x0000000000001504          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001508*/ STR         SL, [SP, #4] 
    /*0000150c*/ MOV         R3, #480                    @ 0x1e0 
    /*00001510*/ MOV         R1, #22 
    /*00001514*/ STR         R3, [SP] 
    /*00001518*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x564] 
    /*0000151c*/ LDR         R3, [PC, #280]              @ 0x000000000000163c 
    /*00001520*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x564] 
    /*00001524*/ MOV         R0, #3 
    /*00001528*/ BL          0x0000000000001528          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000152c*/ MOVW        R0, #32773                  @ 0x8005 
    /*00001530*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001534*/ ADD         SP, SP, #36                 @ 0x24 
    /*00001538*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOClearChnBuf_x153c:
    /*0000153c*/ MOV         R0, R4 
    /*00001540*/ BL          0x0000000000001540          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001544*/ STR         SL, [SP, #8] 
    /*00001548*/ LDR         IP, [R6, #44]               @ 0x2c 
    /*0000154c*/ MOVW        R3, #489                    @ 0x1e9 
    /*00001550*/ MOV         R1, #22 
    /*00001554*/ STR         R3, [SP] 
    /*00001558*/ MOVW        R2, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x59c] 
    /*0000155c*/ LDR         R3, [PC, #216]              @ 0x000000000000163c 
    /*00001560*/ LSR         IP, IP, #1 
    /*00001564*/ MOVT        R2, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x59c] 
    /*00001568*/ MOV         R0, #3 
    /*0000156c*/ MLA         R5, R5, IP, R9 
    /*00001570*/ STR         R5, [SP, #4] 
    /*00001574*/ BL          0x0000000000001574          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001578*/ MOVW        R0, #32773                  @ 0x8005 
    /*0000157c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001580*/ ADD         SP, SP, #36                 @ 0x24 
    /*00001584*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOClearChnBuf_x1588:
    /*00001588*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*0000158c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00001590*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*00001594*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00001598*/ MOVW        R2, #458                    @ 0x1ca 
    /*0000159c*/ LDR         R1, [PC, #152]              @ 0x000000000000163c 
    /*000015a0*/ BL          0x00000000000015A0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000015a4*/ B           AOClearChnBuf_x13d8
    AOClearChnBuf_x15a8:
    /*000015a8*/ STR         R0, [SP, #4] 
    /*000015ac*/ MOVW        R3, #454                    @ 0x1c6 
    /*000015b0*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*000015b4*/ STR         R3, [SP] 
    /*000015b8*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*000015bc*/ LDR         R3, [PC, #120]              @ 0x000000000000163c 
    /*000015c0*/ MOV         R1, #22 
    /*000015c4*/ MOV         R0, #3 
    /*000015c8*/ BL          0x00000000000015C8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000015cc*/ MOVW        R0, #32769                  @ 0x8001 
    /*000015d0*/ MOVT        R0, #40982                  @ 0xa016 
    /*000015d4*/ B           AOClearChnBuf_x14e8
    AOClearChnBuf_x15d8:
    /*000015d8*/ STR         R1, [SP, #4] 
    /*000015dc*/ MOVW        R3, #455                    @ 0x1c7 
    /*000015e0*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*000015e4*/ STR         R3, [SP] 
    /*000015e8*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*000015ec*/ LDR         R3, [PC, #72]               @ 0x000000000000163c 
    /*000015f0*/ MOV         R1, #20 
    /*000015f4*/ MOV         R0, #3 
    /*000015f8*/ BL          0x00000000000015F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000015fc*/ MOVW        R0, #32770                  @ 0x8002 
    /*00001600*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001604*/ B           AOClearChnBuf_x14e8
    AOClearChnBuf_x1608:
    /*00001608*/ MOV         R3, #464                    @ 0x1d0 
    /*0000160c*/ STR         SL, [SP, #8] 
    /*00001610*/ STR         R3, [SP] 
    /*00001614*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x39c] 
    /*00001618*/ STR         R9, [SP, #4] 
    /*0000161c*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x39c] 
    /*00001620*/ LDR         R3, [PC, #20]               @ 0x000000000000163c 
    /*00001624*/ MOV         R1, #20 
    /*00001628*/ MOV         R0, #3 
    /*0000162c*/ BL          0x000000000000162C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001630*/ MOVW        R0, #32770                  @ 0x8002 
    /*00001634*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001638*/ B           AOClearChnBuf_x14e8
    /*0000163c*/ ANDEQ       R0, R0, IP, ASR R0 
FUNC_END AOClearChnBuf


FUNC_BEGIN AOGetChnState
    /*00001640*/ CMP         R0, #1 
    /*00001644*/ PUSH        {R4, R5, LR} 
    /*00001648*/ SUB         SP, SP, #12 
    /*0000164c*/ BHI         AOGetChnState_x16ac
    /*00001650*/ CMP         R1, #2 
    /*00001654*/ BHI         AOGetChnState_x16e8
    /*00001658*/ CMP         R2, #0 
    /*0000165c*/ MOV         R4, R2 
    /*00001660*/ BEQ         AOGetChnState_x16dc
    /*00001664*/ ADD         R0, R0, R0, LSL #1 
    /*00001668*/ ADD         R0, R0, R1 
    /*0000166c*/ BL          0x000000000000166C          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00001670*/ SUBS        R5, R0, #0 
    /*00001674*/ BEQ         AOGetChnState_x168c
    AOGetChnState_x1678:
    /*00001678*/ LDR         R3, [R5, #1240]             @ 0x4d8 
    /*0000167c*/ MOV         R0, #0 
    /*00001680*/ STR         R3, [R4] 
    AOGetChnState_x1684:
    /*00001684*/ ADD         SP, SP, #12 
    /*00001688*/ POP         {R4, R5, PC} 
    AOGetChnState_x168c:
    /*0000168c*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*00001690*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00001694*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00001698*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*0000169c*/ MOV         R2, #520                    @ 0x208 
    /*000016a0*/ LDR         R1, [PC, #112]              @ 0x0000000000001718 
    /*000016a4*/ BL          0x00000000000016A4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000016a8*/ B           AOGetChnState_x1678
    AOGetChnState_x16ac:
    /*000016ac*/ STR         R0, [SP, #4] 
    /*000016b0*/ MOVW        R3, #514                    @ 0x202 
    /*000016b4*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*000016b8*/ STR         R3, [SP] 
    /*000016bc*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*000016c0*/ LDR         R3, [PC, #80]               @ 0x0000000000001718 
    /*000016c4*/ MOV         R1, #22 
    /*000016c8*/ MOV         R0, #3 
    /*000016cc*/ BL          0x00000000000016CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000016d0*/ MOVW        R0, #32769                  @ 0x8001 
    /*000016d4*/ MOVT        R0, #40982                  @ 0xa016 
    /*000016d8*/ B           AOGetChnState_x1684
    AOGetChnState_x16dc:
    /*000016dc*/ MOVW        R0, #32774                  @ 0x8006 
    /*000016e0*/ MOVT        R0, #40982                  @ 0xa016 
    /*000016e4*/ B           AOGetChnState_x1684
    AOGetChnState_x16e8:
    /*000016e8*/ STR         R1, [SP, #4] 
    /*000016ec*/ MOVW        R3, #515                    @ 0x203 
    /*000016f0*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*000016f4*/ STR         R3, [SP] 
    /*000016f8*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*000016fc*/ LDR         R3, [PC, #20]               @ 0x0000000000001718 
    /*00001700*/ MOV         R1, #20 
    /*00001704*/ MOV         R0, #3 
    /*00001708*/ BL          0x0000000000001708          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000170c*/ MOVW        R0, #32770                  @ 0x8002 
    /*00001710*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001714*/ B           AOGetChnState_x1684
    /*00001718*/ ANDEQ       R0, R0, IP, RRX 
FUNC_END AOGetChnState


FUNC_BEGIN AOQueryChnState
    /*0000171c*/ CMP         R0, #1 
    /*00001720*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00001724*/ MOV         R5, R0 
    /*00001728*/ SUB         SP, SP, #20 
    /*0000172c*/ BHI         AOQueryChnState_x1854
    /*00001730*/ CMP         R1, #2 
    /*00001734*/ MOV         R7, R1 
    /*00001738*/ BHI         AOQueryChnState_x18f4
    /*0000173c*/ CMP         R2, #0 
    /*00001740*/ MOV         R6, R2 
    /*00001744*/ BEQ         AOQueryChnState_x18e8
    /*00001748*/ ADD         R0, R0, R0, LSL #1 
    /*0000174c*/ ADD         R0, R0, R1 
    /*00001750*/ BL          0x0000000000001750          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00001754*/ SUBS        R4, R0, #0 
    /*00001758*/ BEQ         AOQueryChnState_x1834
    AOQueryChnState_x175c:
    /*0000175c*/ MOV         R0, R4 
    /*00001760*/ BL          0x0000000000001760          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001764*/ CMP         R0, #0 
    /*00001768*/ BNE         AOQueryChnState_x18bc
    /*0000176c*/ LDR         R3, [R4, #1244]             @ 0x4dc 
    /*00001770*/ CMP         R3, #1 
    /*00001774*/ BNE         AOQueryChnState_x1884
    /*00001778*/ LDR         R3, [R4, #1240]             @ 0x4d8 
    /*0000177c*/ CMP         R3, #0 
    /*00001780*/ CMPNE       R3, #3 
    /*00001784*/ MOVEQ       R5, #1 
    /*00001788*/ MOVNE       R5, #0 
    /*0000178c*/ BEQ         AOQueryChnState_x18c8
    /*00001790*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001794*/ ADD         R1, SP, #12 
    /*00001798*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000179c*/ BL          0x000000000000179C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000017a0*/ LDR         R3, [R4, #1208]             @ 0x4b8 
    /*000017a4*/ STR         R3, [R6] 
    /*000017a8*/ LDR         R3, [R4, #1228]             @ 0x4cc 
    /*000017ac*/ LDR         R2, [R4, #1224]             @ 0x4c8 
    /*000017b0*/ CMP         R3, R2 
    /*000017b4*/ BEQ         AOQueryChnState_x180c
    /*000017b8*/ LDRLS       R1, [R4, #1208]             @ 0x4b8 
    /*000017bc*/ ADDLS       R3, R3, R1 
    /*000017c0*/ SUB         R3, R3, R2 
    AOQueryChnState_x17c4:
    /*000017c4*/ STR         R3, [R6, #4] 
    /*000017c8*/ LDR         R3, [R4, #1228]             @ 0x4cc 
    /*000017cc*/ LDR         R2, [R4, #1224]             @ 0x4c8 
    /*000017d0*/ CMP         R3, R2 
    /*000017d4*/ BEQ         AOQueryChnState_x1820
    /*000017d8*/ LDRHI       R1, [R4, #1208]             @ 0x4b8 
    /*000017dc*/ ADDHI       R2, R2, R1 
    /*000017e0*/ SUB         R3, R2, R3 
    AOQueryChnState_x17e4:
    /*000017e4*/ STR         R3, [R6, #8] 
    /*000017e8*/ ADD         R1, SP, #12 
    /*000017ec*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000017f0*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000017f4*/ BL          0x00000000000017F4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000017f8*/ MOV         R0, R4 
    /*000017fc*/ BL          0x00000000000017FC          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001800*/ MOV         R0, #0 
    AOQueryChnState_x1804:
    /*00001804*/ ADD         SP, SP, #20 
    /*00001808*/ POP         {R4, R5, R6, R7, PC} 
    AOQueryChnState_x180c:
    /*0000180c*/ LDR         R3, [R4, #1236]             @ 0x4d4 
    /*00001810*/ CMP         R3, #0 
    /*00001814*/ LDREQ       R3, [R4, #1208]             @ 0x4b8 
    /*00001818*/ MOVNE       R3, R5 
    /*0000181c*/ B           AOQueryChnState_x17c4
    AOQueryChnState_x1820:
    /*00001820*/ LDR         R3, [R4, #1236]             @ 0x4d4 
    /*00001824*/ CMP         R3, #1 
    /*00001828*/ LDREQ       R3, [R4, #1208]             @ 0x4b8 
    /*0000182c*/ MOVNE       R3, #0 
    /*00001830*/ B           AOQueryChnState_x17e4
    AOQueryChnState_x1834:
    /*00001834*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*00001838*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*0000183c*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00001840*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00001844*/ MOVW        R2, #539                    @ 0x21b 
    /*00001848*/ LDR         R1, [PC, #212]              @ 0x0000000000001924 
    /*0000184c*/ BL          0x000000000000184C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001850*/ B           AOQueryChnState_x175c
    AOQueryChnState_x1854:
    /*00001854*/ STR         R0, [SP, #4] 
    /*00001858*/ MOVW        R3, #533                    @ 0x215 
    /*0000185c*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*00001860*/ STR         R3, [SP] 
    /*00001864*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00001868*/ LDR         R3, [PC, #180]              @ 0x0000000000001924 
    /*0000186c*/ MOV         R1, #22 
    /*00001870*/ MOV         R0, #3 
    /*00001874*/ BL          0x0000000000001874          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001878*/ MOVW        R0, #32769                  @ 0x8001 
    /*0000187c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001880*/ B           AOQueryChnState_x1804
    AOQueryChnState_x1884:
    /*00001884*/ MOV         R0, R4 
    /*00001888*/ BL          0x0000000000001888          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000188c*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x564] 
    /*00001890*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x564] 
    /*00001894*/ MOVW        R3, #549                    @ 0x225 
    /*00001898*/ STR         R5, [SP, #4] 
    /*0000189c*/ STR         R3, [SP] 
    AOQueryChnState_x18a0:
    /*000018a0*/ LDR         R3, [PC, #124]              @ 0x0000000000001924 
    /*000018a4*/ MOV         R1, #22 
    /*000018a8*/ MOV         R0, #3 
    /*000018ac*/ BL          0x00000000000018AC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000018b0*/ MOVW        R0, #32773                  @ 0x8005 
    /*000018b4*/ MOVT        R0, #40982                  @ 0xa016 
    /*000018b8*/ B           AOQueryChnState_x1804
    AOQueryChnState_x18bc:
    /*000018bc*/ MOV         R0, #65024                  @ 0xfe00 
    /*000018c0*/ MOVT        R0, #65535                  @ 0xffff 
    /*000018c4*/ B           AOQueryChnState_x1804
    AOQueryChnState_x18c8:
    /*000018c8*/ MOV         R0, R4 
    /*000018cc*/ BL          0x00000000000018CC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000018d0*/ MOVW        R3, #557                    @ 0x22d 
    /*000018d4*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5e0] 
    /*000018d8*/ STR         R7, [SP, #4] 
    /*000018dc*/ STR         R3, [SP] 
    /*000018e0*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5e0] 
    /*000018e4*/ B           AOQueryChnState_x18a0
    AOQueryChnState_x18e8:
    /*000018e8*/ MOVW        R0, #32774                  @ 0x8006 
    /*000018ec*/ MOVT        R0, #40982                  @ 0xa016 
    /*000018f0*/ B           AOQueryChnState_x1804
    AOQueryChnState_x18f4:
    /*000018f4*/ STR         R1, [SP, #4] 
    /*000018f8*/ MOVW        R3, #534                    @ 0x216 
    /*000018fc*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*00001900*/ STR         R3, [SP] 
    /*00001904*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*00001908*/ LDR         R3, [PC, #20]               @ 0x0000000000001924 
    /*0000190c*/ MOV         R1, #20 
    /*00001910*/ MOV         R0, #3 
    /*00001914*/ BL          0x0000000000001914          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001918*/ MOVW        R0, #32770                  @ 0x8002 
    /*0000191c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001920*/ B           AOQueryChnState_x1804
    /*00001924*/ ANDEQ       R0, R0, IP, ROR R0 
FUNC_END AOQueryChnState


FUNC_BEGIN AOPutFrame
    /*00001928*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000192c*/ CMP         R0, #1 
    /*00001930*/ SUB         SP, SP, #68                 @ 0x44 
    /*00001934*/ STR         R0, [SP, #32] 
    /*00001938*/ STR         R1, [SP, #28] 
    /*0000193c*/ BHI         AOPutFrame_x1db0
    /*00001940*/ LDR         R3, [SP, #28] 
    /*00001944*/ CMP         R3, #2 
    /*00001948*/ BHI         AOPutFrame_x1e44
    /*0000194c*/ CMP         R2, #0 
    /*00001950*/ MOV         R9, R2 
    /*00001954*/ BEQ         AOPutFrame_x1eec
    /*00001958*/ BL          0x0000000000001958          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*0000195c*/ SUBS        SL, R0, #0 
    /*00001960*/ BEQ         AOPutFrame_x1d4c
    AOPutFrame_x1964:
    /*00001964*/ LDR         R4, [SP, #28] 
    /*00001968*/ CMP         R4, #2 
    /*0000196c*/ BEQ         AOPutFrame_x1c54
    /*00001970*/ LDR         R3, [SL, #28] 
    /*00001974*/ LDR         R0, [SL, #44]               @ 0x2c 
    /*00001978*/ CMP         R3, #1 
    /*0000197c*/ MOVEQ       R3, #2 
    /*00001980*/ MOVNE       R3, #1 
    /*00001984*/ MOV         R1, R3 
    /*00001988*/ STR         R3, [SP, #48]               @ 0x30 
    /*0000198c*/ BL          0x000000000000198C          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*00001990*/ CMP         R4, R0 
    /*00001994*/ BGE         AOPutFrame_x1ea4
    AOPutFrame_x1998:
    /*00001998*/ LDR         R3, [SL, #20] 
    /*0000199c*/ LDR         R2, [R9] 
    /*000019a0*/ CMP         R3, R2 
    /*000019a4*/ BNE         AOPutFrame_x1d6c
    /*000019a8*/ LDR         R0, [R9, #44]               @ 0x2c 
    /*000019ac*/ CMP         R0, #16384                  @ 0x4000 
    /*000019b0*/ BHI         AOPutFrame_x1e74
    /*000019b4*/ CMP         R3, #2 
    /*000019b8*/ ADDNE       R1, R3, #1 
    /*000019bc*/ MOVEQ       R1, #4 
    /*000019c0*/ BL          0x00000000000019C0          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000019c4*/ LDR         R3, [SL, #40]               @ 0x28 
    /*000019c8*/ CMP         R0, R3 
    /*000019cc*/ BNE         AOPutFrame_x1de0
    /*000019d0*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*000019d4*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*000019d8*/ STR         R3, [SP, #52]               @ 0x34 
    /*000019dc*/ MOVW        R6, #0                      @ AoPutFrame_WaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x608] 
    /*000019e0*/ LDR         R3, [SP, #32] 
    /*000019e4*/ MOVW        FP, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000019e8*/ MOVT        R6, #0                      @ AoPutFrame_WaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x608] 
    /*000019ec*/ MOVT        FP, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000019f0*/ ADD         R3, R3, R3, LSL #1 
    /*000019f4*/ STR         R3, [SP, #40]               @ 0x28 
    /*000019f8*/ ADD         R3, R9, #8 
    /*000019fc*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00001a00*/ MOV         R3, #0 
    /*00001a04*/ STR         R3, [SP, #36]               @ 0x24 
    AOPutFrame_x1a08:
    /*00001a08*/ LDR         R3, [SL, #44]               @ 0x2c 
    /*00001a0c*/ LDR         R2, [SP, #36]               @ 0x24 
    /*00001a10*/ LDR         R1, [SP, #28] 
    /*00001a14*/ LSR         R3, R3, #1 
    /*00001a18*/ MLA         R3, R2, R3, R1 
    /*00001a1c*/ LDR         R2, [SP, #40]               @ 0x28 
    /*00001a20*/ ADD         R0, R3, R2 
    /*00001a24*/ STR         R3, [SP, #28] 
    /*00001a28*/ BL          0x0000000000001A28          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00001a2c*/ SUBS        R4, R0, #0 
    /*00001a30*/ BEQ         AOPutFrame_x1d30
    AOPutFrame_x1a34:
    /*00001a34*/ MOV         R0, R4 
    /*00001a38*/ BL          0x0000000000001A38          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001a3c*/ CMP         R0, #0 
    /*00001a40*/ BNE         AOPutFrame_x1c44
    /*00001a44*/ LDR         R3, [R4, #1240]             @ 0x4d8 
    /*00001a48*/ SUB         R3, R3, #1 
    /*00001a4c*/ CMP         R3, #1 
    /*00001a50*/ BHI         AOPutFrame_x1e00
    /*00001a54*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001a58*/ ADD         R1, SP, #60                 @ 0x3c 
    /*00001a5c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001a60*/ BL          0x0000000000001A60          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001a64*/ LDR         R3, [R4, #1228]             @ 0x4cc 
    /*00001a68*/ LDR         R5, [R4, #1224]             @ 0x4c8 
    /*00001a6c*/ CMP         R3, R5 
    /*00001a70*/ BNE         AOPutFrame_x1b58
    /*00001a74*/ LDR         R2, [R4, #1236]             @ 0x4d4 
    /*00001a78*/ CMP         R2, #1 
    /*00001a7c*/ BNE         AOPutFrame_x1b58
    /*00001a80*/ MOVW        R8, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001a84*/ MOVT        R8, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    AOPutFrame_x1a88:
    /*00001a88*/ ADD         R1, SP, #60                 @ 0x3c 
    /*00001a8c*/ MOV         R0, R8 
    /*00001a90*/ BL          0x0000000000001A90          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001a94*/ MOV         R0, R4 
    /*00001a98*/ BL          0x0000000000001A98          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001a9c*/ LDR         R5, [R9, #128]              @ 0x80 
    /*00001aa0*/ CMN         R5, #1 
    /*00001aa4*/ BEQ         AOPutFrame_x1bdc
    /*00001aa8*/ CMP         R5, #0 
    /*00001aac*/ BEQ         AOPutFrame_x1cc0
    /*00001ab0*/ ADD         R7, SL, #120                @ 0x78 
    /*00001ab4*/ B           AOPutFrame_x1acc
    AOPutFrame_x1ab8:
    /*00001ab8*/ BL          0x0000000000001AB8          @ osal_wait_timeout_interruptible [R_ARM_CALL: +0x0] 
    /*00001abc*/ ADD         R3, R0, #512                @ 0x200 
    /*00001ac0*/ MOV         R5, R0 
    /*00001ac4*/ BICS        R3, R3, #512                @ 0x200 
    /*00001ac8*/ BEQ         AOPutFrame_x1af8
    AOPutFrame_x1acc:
    /*00001acc*/ MOV         R0, R4 
    /*00001ad0*/ BL          0x0000000000001AD0          @ AoPutFrame_WaitConditionCallBack [R_ARM_CALL: .text+0x608] 
    /*00001ad4*/ MOV         R3, R5 
    /*00001ad8*/ MOV         R2, R4 
    /*00001adc*/ MOV         R1, R6 
    /*00001ae0*/ CMP         R0, #0 
    /*00001ae4*/ MOV         R0, R7 
    /*00001ae8*/ BEQ         AOPutFrame_x1ab8
    /*00001aec*/ MOV         R0, R5 
    /*00001af0*/ BL          0x0000000000001AF0          @ osal_msecs_to_jiffies [R_ARM_CALL: +0x0] 
    /*00001af4*/ MOV         R5, R0 
    AOPutFrame_x1af8:
    /*00001af8*/ CMP         R5, #0 
    /*00001afc*/ BLT         AOPutFrame_x1d00
    /*00001b00*/ BEQ         AOPutFrame_x1d0c
    AOPutFrame_x1b04:
    /*00001b04*/ MOV         R0, R4 
    /*00001b08*/ BL          0x0000000000001B08          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001b0c*/ CMP         R0, #0 
    /*00001b10*/ BNE         AOPutFrame_x1c44
    /*00001b14*/ LDR         R3, [FP] 
    /*00001b18*/ CMP         R3, #0 
    /*00001b1c*/ BNE         AOPutFrame_x1c60
    /*00001b20*/ LDR         R3, [R4, #1240]             @ 0x4d8 
    /*00001b24*/ SUB         R3, R3, #1 
    /*00001b28*/ CMP         R3, #1 
    /*00001b2c*/ BHI         AOPutFrame_x1c78
    /*00001b30*/ ADD         R1, SP, #60                 @ 0x3c 
    /*00001b34*/ MOV         R0, R8 
    /*00001b38*/ BL          0x0000000000001B38          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001b3c*/ LDR         R3, [R4, #1228]             @ 0x4cc 
    /*00001b40*/ LDR         R5, [R4, #1224]             @ 0x4c8 
    /*00001b44*/ CMP         R3, R5 
    /*00001b48*/ BNE         AOPutFrame_x1b58
    /*00001b4c*/ LDR         R2, [R4, #1236]             @ 0x4d4 
    /*00001b50*/ CMP         R2, #1 
    /*00001b54*/ BEQ         AOPutFrame_x1a88
    AOPutFrame_x1b58:
    /*00001b58*/ LDR         R1, [R4, #1208]             @ 0x4b8 
    /*00001b5c*/ ADD         R2, R5, #1 
    /*00001b60*/ STR         R2, [R4, #1224]             @ 0x4c8 
    /*00001b64*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001b68*/ CMP         R2, R1 
    /*00001b6c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001b70*/ ADD         R1, SP, #60                 @ 0x3c 
    /*00001b74*/ ADD         R5, R5, #2 
    /*00001b78*/ MOVEQ       R2, #0 
    /*00001b7c*/ STREQ       R2, [R4, #1224]             @ 0x4c8 
    /*00001b80*/ CMP         R3, R2 
    /*00001b84*/ MOVEQ       R3, #1 
    /*00001b88*/ STREQ       R3, [R4, #1236]             @ 0x4d4 
    /*00001b8c*/ BL          0x0000000000001B8C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001b90*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*00001b94*/ LDR         R0, [R4, R5, LSL #2] 
    /*00001b98*/ LDR         R2, [R9, #44]               @ 0x2c 
    /*00001b9c*/ LDR         R1, [R3], #4 
    /*00001ba0*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00001ba4*/ BL          0x0000000000001BA4          @ osal_copy_from_user [R_ARM_CALL: +0x0] 
    /*00001ba8*/ SUBS        R5, R0, #0 
    /*00001bac*/ MOV         R0, R4 
    /*00001bb0*/ BNE         AOPutFrame_x1ee0
    /*00001bb4*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00001bb8*/ ADD         R3, R3, #1 
    /*00001bbc*/ STR         R3, [SP, #36]               @ 0x24 
    /*00001bc0*/ MOV         R4, R3 
    /*00001bc4*/ BL          0x0000000000001BC4          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001bc8*/ LDR         R2, [SP, #48]               @ 0x30 
    /*00001bcc*/ CMP         R4, R2 
    /*00001bd0*/ BNE         AOPutFrame_x1a08
    /*00001bd4*/ MOV         R0, R5 
    /*00001bd8*/ B           AOPutFrame_x1c4c
    AOPutFrame_x1bdc:
    /*00001bdc*/ ADD         R5, SL, #120                @ 0x78 
    AOPutFrame_x1be0:
    /*00001be0*/ MOV         R0, R4 
    /*00001be4*/ BL          0x0000000000001BE4          @ AoPutFrame_WaitConditionCallBack [R_ARM_CALL: .text+0x608] 
    /*00001be8*/ MOV         R3, #100                    @ 0x64 
    /*00001bec*/ MOV         R2, R4 
    /*00001bf0*/ MOV         R1, R6 
    /*00001bf4*/ CMP         R0, #0 
    /*00001bf8*/ MOV         R0, R5 
    /*00001bfc*/ BNE         AOPutFrame_x1b04
    /*00001c00*/ BL          0x0000000000001C00          @ osal_wait_timeout_interruptible [R_ARM_CALL: +0x0] 
    /*00001c04*/ CMP         R0, #0 
    /*00001c08*/ BGE         AOPutFrame_x1be0
    /*00001c0c*/ LDR         R1, [R4, #1240]             @ 0x4d8 
    /*00001c10*/ MOV         R3, #684                    @ 0x2ac 
    AOPutFrame_x1c14:
    /*00001c14*/ LDR         R2, [SP, #28] 
    /*00001c18*/ LDR         R0, [SP, #32] 
    /*00001c1c*/ STR         R1, [SP, #12] 
    /*00001c20*/ MOV         R1, #22 
    /*00001c24*/ STR         R2, [SP, #8] 
    /*00001c28*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7b8] 
    /*00001c2c*/ STR         R0, [SP, #4] 
    /*00001c30*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7b8] 
    /*00001c34*/ STR         R3, [SP] 
    /*00001c38*/ MOV         R0, #3 
    /*00001c3c*/ LDR         R3, [PC, #740]              @ 0x0000000000001f28 
    /*00001c40*/ BL          0x0000000000001C40          @ HI_LOG [R_ARM_CALL: +0x0] 
    AOPutFrame_x1c44:
    /*00001c44*/ MOV         R0, #65024                  @ 0xfe00 
    /*00001c48*/ MOVT        R0, #65535                  @ 0xffff 
    AOPutFrame_x1c4c:
    /*00001c4c*/ ADD         SP, SP, #68                 @ 0x44 
    /*00001c50*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOPutFrame_x1c54:
    /*00001c54*/ MOV         R3, #1 
    /*00001c58*/ STR         R3, [SP, #48]               @ 0x30 
    /*00001c5c*/ B           AOPutFrame_x1998
    AOPutFrame_x1c60:
    /*00001c60*/ MOV         R0, R4 
    /*00001c64*/ BL          0x0000000000001C64          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001c68*/ MOVW        R0, #32784                  @ 0x8010 
    /*00001c6c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001c70*/ ADD         SP, SP, #68                 @ 0x44 
    /*00001c74*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOPutFrame_x1c78:
    /*00001c78*/ MOV         R0, R4 
    /*00001c7c*/ BL          0x0000000000001C7C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001c80*/ LDR         R3, [SP, #28] 
    /*00001c84*/ MOV         R1, #22 
    /*00001c88*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x73c] 
    /*00001c8c*/ MOV         R0, #3 
    /*00001c90*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x73c] 
    /*00001c94*/ STR         R3, [SP, #8] 
    /*00001c98*/ LDR         R3, [SP, #32] 
    /*00001c9c*/ STR         R3, [SP, #4] 
    /*00001ca0*/ MOV         R3, #724                    @ 0x2d4 
    /*00001ca4*/ STR         R3, [SP] 
    /*00001ca8*/ LDR         R3, [PC, #632]              @ 0x0000000000001f28 
    /*00001cac*/ BL          0x0000000000001CAC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001cb0*/ MOVW        R0, #32773                  @ 0x8005 
    /*00001cb4*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001cb8*/ ADD         SP, SP, #68                 @ 0x44 
    /*00001cbc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOPutFrame_x1cc0:
    /*00001cc0*/ LDR         R2, [SP, #32] 
    /*00001cc4*/ LDR         R3, [SP, #28] 
    /*00001cc8*/ STR         R2, [SP, #4] 
    /*00001ccc*/ MOVW        R2, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x780] 
    /*00001cd0*/ MOVT        R2, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x780] 
    /*00001cd4*/ STR         R3, [SP, #8] 
    /*00001cd8*/ MOVW        R3, #690                    @ 0x2b2 
    /*00001cdc*/ STR         R3, [SP] 
    AOPutFrame_x1ce0:
    /*00001ce0*/ LDR         R3, [PC, #576]              @ 0x0000000000001f28 
    /*00001ce4*/ MOV         R1, #22 
    /*00001ce8*/ MOV         R0, #3 
    /*00001cec*/ BL          0x0000000000001CEC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001cf0*/ MOVW        R0, #32783                  @ 0x800f 
    /*00001cf4*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001cf8*/ ADD         SP, SP, #68                 @ 0x44 
    /*00001cfc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOPutFrame_x1d00:
    /*00001d00*/ LDR         R1, [R4, #1240]             @ 0x4d8 
    /*00001d04*/ MOV         R3, #700                    @ 0x2bc 
    /*00001d08*/ B           AOPutFrame_x1c14
    AOPutFrame_x1d0c:
    /*00001d0c*/ LDR         R3, [SP, #28] 
    /*00001d10*/ LDR         R2, [SP, #32] 
    /*00001d14*/ STR         R3, [SP, #8] 
    /*00001d18*/ MOVW        R3, #705                    @ 0x2c1 
    /*00001d1c*/ STR         R2, [SP, #4] 
    /*00001d20*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7e8] 
    /*00001d24*/ STR         R3, [SP] 
    /*00001d28*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7e8] 
    /*00001d2c*/ B           AOPutFrame_x1ce0
    AOPutFrame_x1d30:
    /*00001d30*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00001d34*/ LDR         R3, [SP, #52]               @ 0x34 
    /*00001d38*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00001d3c*/ MOVW        R2, #657                    @ 0x291 
    /*00001d40*/ LDR         R1, [PC, #480]              @ 0x0000000000001f28 
    /*00001d44*/ BL          0x0000000000001D44          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001d48*/ B           AOPutFrame_x1a34
    AOPutFrame_x1d4c:
    /*00001d4c*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*00001d50*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00001d54*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*00001d58*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00001d5c*/ MOVW        R2, #621                    @ 0x26d 
    /*00001d60*/ LDR         R1, [PC, #448]              @ 0x0000000000001f28 
    /*00001d64*/ BL          0x0000000000001D64          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001d68*/ B           AOPutFrame_x1964
    AOPutFrame_x1d6c:
    /*00001d6c*/ LDR         R1, [SP, #28] 
    /*00001d70*/ STR         R2, [SP, #12] 
    /*00001d74*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6a0] 
    /*00001d78*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6a0] 
    /*00001d7c*/ STR         R3, [SP, #16] 
    /*00001d80*/ MOV         R3, #632                    @ 0x278 
    /*00001d84*/ STR         R1, [SP, #8] 
    AOPutFrame_x1d88:
    /*00001d88*/ LDR         R1, [SP, #32] 
    /*00001d8c*/ MOV         R0, #3 
    /*00001d90*/ STR         R3, [SP] 
    /*00001d94*/ LDR         R3, [PC, #396]              @ 0x0000000000001f28 
    /*00001d98*/ STR         R1, [SP, #4] 
    /*00001d9c*/ MOV         R1, #22 
    /*00001da0*/ BL          0x0000000000001DA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001da4*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001da8*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001dac*/ B           AOPutFrame_x1c4c
    AOPutFrame_x1db0:
    /*00001db0*/ STR         R0, [SP, #4] 
    /*00001db4*/ MOV         R3, #608                    @ 0x260 
    /*00001db8*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*00001dbc*/ STR         R3, [SP] 
    /*00001dc0*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00001dc4*/ LDR         R3, [PC, #348]              @ 0x0000000000001f28 
    /*00001dc8*/ MOV         R1, #22 
    /*00001dcc*/ MOV         R0, #3 
    /*00001dd0*/ BL          0x0000000000001DD0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001dd4*/ MOVW        R0, #32769                  @ 0x8001 
    /*00001dd8*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001ddc*/ B           AOPutFrame_x1c4c
    AOPutFrame_x1de0:
    /*00001de0*/ LDR         R1, [SP, #28] 
    /*00001de4*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e0] 
    /*00001de8*/ STR         R3, [SP, #16] 
    /*00001dec*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e0] 
    /*00001df0*/ MOVW        R3, #647                    @ 0x287 
    /*00001df4*/ STR         R0, [SP, #12] 
    /*00001df8*/ STR         R1, [SP, #8] 
    /*00001dfc*/ B           AOPutFrame_x1d88
    AOPutFrame_x1e00:
    /*00001e00*/ MOV         R0, R4 
    /*00001e04*/ BL          0x0000000000001E04          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001e08*/ LDR         R3, [SP, #28] 
    /*00001e0c*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x73c] 
    /*00001e10*/ MOV         R1, #22 
    /*00001e14*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x73c] 
    /*00001e18*/ MOV         R0, #4 
    /*00001e1c*/ STR         R3, [SP, #8] 
    /*00001e20*/ LDR         R3, [SP, #32] 
    /*00001e24*/ STR         R3, [SP, #4] 
    /*00001e28*/ MOV         R3, #668                    @ 0x29c 
    /*00001e2c*/ STR         R3, [SP] 
    /*00001e30*/ LDR         R3, [PC, #240]              @ 0x0000000000001f28 
    /*00001e34*/ BL          0x0000000000001E34          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001e38*/ MOVW        R0, #32773                  @ 0x8005 
    /*00001e3c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001e40*/ B           AOPutFrame_x1c4c
    AOPutFrame_x1e44:
    /*00001e44*/ STR         R3, [SP, #4] 
    /*00001e48*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*00001e4c*/ MOVW        R3, #609                    @ 0x261 
    /*00001e50*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*00001e54*/ STR         R3, [SP] 
    /*00001e58*/ MOV         R1, #20 
    /*00001e5c*/ LDR         R3, [PC, #196]              @ 0x0000000000001f28 
    /*00001e60*/ MOV         R0, #3 
    /*00001e64*/ BL          0x0000000000001E64          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001e68*/ MOVW        R0, #32770                  @ 0x8002 
    /*00001e6c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001e70*/ B           AOPutFrame_x1c4c
    AOPutFrame_x1e74:
    /*00001e74*/ STR         R0, [SP, #4] 
    /*00001e78*/ MOVW        R3, #638                    @ 0x27e 
    /*00001e7c*/ MOVW        R2, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x700] 
    /*00001e80*/ STR         R3, [SP] 
    /*00001e84*/ MOVT        R2, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x700] 
    /*00001e88*/ LDR         R3, [PC, #152]              @ 0x0000000000001f28 
    /*00001e8c*/ MOV         R1, #22 
    /*00001e90*/ MOV         R0, #3 
    /*00001e94*/ BL          0x0000000000001E94          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001e98*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001e9c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001ea0*/ B           AOPutFrame_x1c4c
    AOPutFrame_x1ea4:
    /*00001ea4*/ LDR         R3, [SP, #32] 
    /*00001ea8*/ MOV         R1, #20 
    /*00001eac*/ LDR         R2, [SP, #28] 
    /*00001eb0*/ MOV         R0, #3 
    /*00001eb4*/ STR         R3, [SP, #8] 
    /*00001eb8*/ MOVW        R3, #627                    @ 0x273 
    /*00001ebc*/ STR         R2, [SP, #4] 
    /*00001ec0*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x39c] 
    /*00001ec4*/ STR         R3, [SP] 
    /*00001ec8*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x39c] 
    /*00001ecc*/ LDR         R3, [PC, #84]               @ 0x0000000000001f28 
    /*00001ed0*/ BL          0x0000000000001ED0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001ed4*/ MOVW        R0, #32770                  @ 0x8002 
    /*00001ed8*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001edc*/ B           AOPutFrame_x1c4c
    AOPutFrame_x1ee0:
    /*00001ee0*/ BL          0x0000000000001EE0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001ee4*/ MVN         R0, #0 
    /*00001ee8*/ B           AOPutFrame_x1c4c
    AOPutFrame_x1eec:
    /*00001eec*/ LDR         R3, [SP, #28] 
    /*00001ef0*/ MOV         R1, #22 
    /*00001ef4*/ LDR         R2, [SP, #32] 
    /*00001ef8*/ MOV         R0, #3 
    /*00001efc*/ STR         R3, [SP, #8] 
    /*00001f00*/ MOVW        R3, #613                    @ 0x265 
    /*00001f04*/ STR         R2, [SP, #4] 
    /*00001f08*/ MOVW        R2, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x64c] 
    /*00001f0c*/ STR         R3, [SP] 
    /*00001f10*/ MOVT        R2, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x64c] 
    /*00001f14*/ LDR         R3, [PC, #12]               @ 0x0000000000001f28 
    /*00001f18*/ BL          0x0000000000001F18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001f1c*/ MOVW        R0, #32774                  @ 0x8006 
    /*00001f20*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001f24*/ B           AOPutFrame_x1c4c
    /*00001f28*/ ANDEQ       R0, R0, IP, LSL #1 
FUNC_END AOPutFrame


FUNC_BEGIN AoSetChnResmpInfo
    /*00001f2c*/ CMP         R0, #1 
    /*00001f30*/ PUSH        {R4, R5, LR} 
    /*00001f34*/ SUB         SP, SP, #12 
    /*00001f38*/ BHI         AoSetChnResmpInfo_x1fb0
    /*00001f3c*/ CMP         R1, #2 
    /*00001f40*/ BHI         AoSetChnResmpInfo_x1fec
    /*00001f44*/ CMP         R2, #0 
    /*00001f48*/ MOV         R4, R2 
    /*00001f4c*/ BEQ         AoSetChnResmpInfo_x1fe0
    /*00001f50*/ ADD         R0, R0, R0, LSL #1 
    /*00001f54*/ ADD         R0, R0, R1 
    /*00001f58*/ BL          0x0000000000001F58          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00001f5c*/ SUBS        R5, R0, #0 
    /*00001f60*/ BEQ         AoSetChnResmpInfo_x1f90
    AoSetChnResmpInfo_x1f64:
    /*00001f64*/ LDR         R3, [R4] 
    /*00001f68*/ MOV         R0, #0 
    /*00001f6c*/ STR         R3, [R5, #2356]             @ 0x934 
    /*00001f70*/ LDR         R3, [R4, #4] 
    /*00001f74*/ STR         R3, [R5, #2360]             @ 0x938 
    /*00001f78*/ LDR         R3, [R4, #8] 
    /*00001f7c*/ STR         R3, [R5, #2364]             @ 0x93c 
    /*00001f80*/ LDR         R3, [R4, #12] 
    /*00001f84*/ STR         R3, [R5, #2368]             @ 0x940 
    AoSetChnResmpInfo_x1f88:
    /*00001f88*/ ADD         SP, SP, #12 
    /*00001f8c*/ POP         {R4, R5, PC} 
    AoSetChnResmpInfo_x1f90:
    /*00001f90*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*00001f94*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00001f98*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00001f9c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00001fa0*/ MOVW        R2, #759                    @ 0x2f7 
    /*00001fa4*/ LDR         R1, [PC, #112]              @ 0x000000000000201c 
    /*00001fa8*/ BL          0x0000000000001FA8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001fac*/ B           AoSetChnResmpInfo_x1f64
    AoSetChnResmpInfo_x1fb0:
    /*00001fb0*/ STR         R0, [SP, #4] 
    /*00001fb4*/ MOVW        R3, #753                    @ 0x2f1 
    /*00001fb8*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*00001fbc*/ STR         R3, [SP] 
    /*00001fc0*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00001fc4*/ LDR         R3, [PC, #80]               @ 0x000000000000201c 
    /*00001fc8*/ MOV         R1, #22 
    /*00001fcc*/ MOV         R0, #3 
    /*00001fd0*/ BL          0x0000000000001FD0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001fd4*/ MOVW        R0, #32769                  @ 0x8001 
    /*00001fd8*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001fdc*/ B           AoSetChnResmpInfo_x1f88
    AoSetChnResmpInfo_x1fe0:
    /*00001fe0*/ MOVW        R0, #32774                  @ 0x8006 
    /*00001fe4*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001fe8*/ B           AoSetChnResmpInfo_x1f88
    AoSetChnResmpInfo_x1fec:
    /*00001fec*/ STR         R1, [SP, #4] 
    /*00001ff0*/ MOVW        R3, #754                    @ 0x2f2 
    /*00001ff4*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*00001ff8*/ STR         R3, [SP] 
    /*00001ffc*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*00002000*/ LDR         R3, [PC, #20]               @ 0x000000000000201c 
    /*00002004*/ MOV         R1, #20 
    /*00002008*/ MOV         R0, #3 
    /*0000200c*/ BL          0x000000000000200C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002010*/ MOVW        R0, #32770                  @ 0x8002 
    /*00002014*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002018*/ B           AoSetChnResmpInfo_x1f88
    /*0000201c*/ MULEQ       R0, R8, R0 
FUNC_END AoSetChnResmpInfo


FUNC_BEGIN AOSetChnVqeInfo
    /*00002020*/ CMP         R0, #1 
    /*00002024*/ PUSH        {R4, R5, LR} 
    /*00002028*/ SUB         SP, SP, #12 
    /*0000202c*/ BHI         AOSetChnVqeInfo_x2098
    /*00002030*/ CMP         R1, #2 
    /*00002034*/ BHI         AOSetChnVqeInfo_x20d4
    /*00002038*/ CMP         R2, #0 
    /*0000203c*/ MOV         R4, R2 
    /*00002040*/ BEQ         AOSetChnVqeInfo_x20c8
    /*00002044*/ ADD         R0, R0, R0, LSL #1 
    /*00002048*/ ADD         R0, R0, R1 
    /*0000204c*/ BL          0x000000000000204C          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00002050*/ SUBS        R5, R0, #0 
    /*00002054*/ BEQ         AOSetChnVqeInfo_x2078
    AOSetChnVqeInfo_x2058:
    /*00002058*/ ADD         R0, R5, #1264               @ 0x4f0 
    /*0000205c*/ MOV         R1, R4 
    /*00002060*/ ADD         R0, R0, #4 
    /*00002064*/ MOV         R2, #1088                   @ 0x440 
    /*00002068*/ BL          0x0000000000002068          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000206c*/ MOV         R0, #0 
    AOSetChnVqeInfo_x2070:
    /*00002070*/ ADD         SP, SP, #12 
    /*00002074*/ POP         {R4, R5, PC} 
    AOSetChnVqeInfo_x2078:
    /*00002078*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*0000207c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00002080*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00002084*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00002088*/ MOV         R2, #780                    @ 0x30c 
    /*0000208c*/ LDR         R1, [PC, #112]              @ 0x0000000000002104 
    /*00002090*/ BL          0x0000000000002090          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002094*/ B           AOSetChnVqeInfo_x2058
    AOSetChnVqeInfo_x2098:
    /*00002098*/ STR         R0, [SP, #4] 
    /*0000209c*/ MOVW        R3, #774                    @ 0x306 
    /*000020a0*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*000020a4*/ STR         R3, [SP] 
    /*000020a8*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*000020ac*/ LDR         R3, [PC, #80]               @ 0x0000000000002104 
    /*000020b0*/ MOV         R1, #22 
    /*000020b4*/ MOV         R0, #3 
    /*000020b8*/ BL          0x00000000000020B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000020bc*/ MOVW        R0, #32769                  @ 0x8001 
    /*000020c0*/ MOVT        R0, #40982                  @ 0xa016 
    /*000020c4*/ B           AOSetChnVqeInfo_x2070
    AOSetChnVqeInfo_x20c8:
    /*000020c8*/ MOVW        R0, #32774                  @ 0x8006 
    /*000020cc*/ MOVT        R0, #40982                  @ 0xa016 
    /*000020d0*/ B           AOSetChnVqeInfo_x2070
    AOSetChnVqeInfo_x20d4:
    /*000020d4*/ STR         R1, [SP, #4] 
    /*000020d8*/ MOVW        R3, #775                    @ 0x307 
    /*000020dc*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*000020e0*/ STR         R3, [SP] 
    /*000020e4*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*000020e8*/ LDR         R3, [PC, #20]               @ 0x0000000000002104 
    /*000020ec*/ MOV         R1, #20 
    /*000020f0*/ MOV         R0, #3 
    /*000020f4*/ BL          0x00000000000020F4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000020f8*/ MOVW        R0, #32770                  @ 0x8002 
    /*000020fc*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002100*/ B           AOSetChnVqeInfo_x2070
    /*00002104*/ ANDEQ       R0, R0, IP, LSR #1 
FUNC_END AOSetChnVqeInfo


FUNC_BEGIN AoSetTrackMode
    /*00002108*/ PUSH        {R4, R5, R6, R7, LR} 
    /*0000210c*/ MOV         R4, R0 
    /*00002110*/ SUB         SP, SP, #12 
    /*00002114*/ MOV         R0, #20 
    /*00002118*/ MOV         R7, R1 
    /*0000211c*/ BL          0x000000000000211C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002120*/ CMP         R0, #0 
    /*00002124*/ BEQ         AoSetTrackMode_x2220
    /*00002128*/ CMP         R4, #1 
    /*0000212c*/ BHI         AoSetTrackMode_x2184
    /*00002130*/ MOV         R0, R4 
    /*00002134*/ BL          0x0000000000002134          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*00002138*/ SUBS        R5, R0, #0 
    /*0000213c*/ BEQ         AoSetTrackMode_x21f4
    AoSetTrackMode_x2140:
    /*00002140*/ ADD         R6, R5, #124                @ 0x7c 
    /*00002144*/ MOV         R0, R6 
    /*00002148*/ BL          0x0000000000002148          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*0000214c*/ CMP         R0, #0 
    /*00002150*/ BNE         AoSetTrackMode_x2214
    /*00002154*/ LDR         R3, [R5, #12] 
    /*00002158*/ CMP         R3, #1 
    /*0000215c*/ BNE         AoSetTrackMode_x21bc
    /*00002160*/ MOV         R1, R7 
    /*00002164*/ MOV         R0, R4 
    /*00002168*/ BL          0x0000000000002168          @ AO_DRV_SetTrackMode [R_ARM_CALL: .text+0x78cc] 
    /*0000216c*/ MOV         R5, R0 
    /*00002170*/ MOV         R0, R6 
    /*00002174*/ BL          0x0000000000002174          @ osal_up [R_ARM_CALL: +0x0] 
    AoSetTrackMode_x2178:
    /*00002178*/ MOV         R0, R5 
    /*0000217c*/ ADD         SP, SP, #12 
    /*00002180*/ POP         {R4, R5, R6, R7, PC} 
    AoSetTrackMode_x2184:
    /*00002184*/ STR         R4, [SP, #4] 
    /*00002188*/ MOVW        R3, #825                    @ 0x339 
    /*0000218c*/ MOV         R1, #22 
    /*00002190*/ STR         R3, [SP] 
    /*00002194*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*00002198*/ LDR         R3, [PC, #140]              @ 0x000000000000222c 
    /*0000219c*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*000021a0*/ MOV         R0, #3 
    /*000021a4*/ BL          0x00000000000021A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000021a8*/ MOVW        R5, #32769                  @ 0x8001 
    /*000021ac*/ MOVT        R5, #40982                  @ 0xa016 
    /*000021b0*/ MOV         R0, R5 
    /*000021b4*/ ADD         SP, SP, #12 
    /*000021b8*/ POP         {R4, R5, R6, R7, PC} 
    AoSetTrackMode_x21bc:
    /*000021bc*/ MOV         R0, R6 
    /*000021c0*/ MOVW        R5, #32773                  @ 0x8005 
    /*000021c4*/ BL          0x00000000000021C4          @ osal_up [R_ARM_CALL: +0x0] 
    /*000021c8*/ MOVW        R3, #838                    @ 0x346 
    /*000021cc*/ STR         R4, [SP, #4] 
    /*000021d0*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x330] 
    /*000021d4*/ STR         R3, [SP] 
    /*000021d8*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x330] 
    /*000021dc*/ LDR         R3, [PC, #72]               @ 0x000000000000222c 
    /*000021e0*/ MOV         R1, #22 
    /*000021e4*/ MOV         R0, #3 
    /*000021e8*/ MOVT        R5, #40982                  @ 0xa016 
    /*000021ec*/ BL          0x00000000000021EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000021f0*/ B           AoSetTrackMode_x2178
    AoSetTrackMode_x21f4:
    /*000021f4*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*000021f8*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*000021fc*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*00002200*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00002204*/ MOVW        R2, #827                    @ 0x33b 
    /*00002208*/ LDR         R1, [PC, #28]               @ 0x000000000000222c 
    /*0000220c*/ BL          0x000000000000220C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002210*/ B           AoSetTrackMode_x2140
    AoSetTrackMode_x2214:
    /*00002214*/ MOV         R5, #65024                  @ 0xfe00 
    /*00002218*/ MOVT        R5, #65535                  @ 0xffff 
    /*0000221c*/ B           AoSetTrackMode_x2178
    AoSetTrackMode_x2220:
    /*00002220*/ MOVW        R5, #32784                  @ 0x8010 
    /*00002224*/ MOVT        R5, #40982                  @ 0xa016 
    /*00002228*/ B           AoSetTrackMode_x2178
    /*0000222c*/ STRHEQ      R0, [R0], -IP 
FUNC_END AoSetTrackMode


FUNC_BEGIN AoGetTrackMode
    /*00002230*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00002234*/ MOV         R4, R0 
    /*00002238*/ SUB         SP, SP, #12 
    /*0000223c*/ MOV         R0, #20 
    /*00002240*/ MOV         R7, R1 
    /*00002244*/ BL          0x0000000000002244          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002248*/ CMP         R0, #0 
    /*0000224c*/ BEQ         AoGetTrackMode_x2354
    /*00002250*/ CMP         R4, #1 
    /*00002254*/ BHI         AoGetTrackMode_x230c
    /*00002258*/ CMP         R7, #0 
    /*0000225c*/ BEQ         AoGetTrackMode_x2348
    /*00002260*/ MOV         R0, R4 
    /*00002264*/ BL          0x0000000000002264          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*00002268*/ SUBS        R6, R0, #0 
    /*0000226c*/ BEQ         AoGetTrackMode_x22ec
    AoGetTrackMode_x2270:
    /*00002270*/ ADD         R5, R6, #124                @ 0x7c 
    /*00002274*/ MOV         R0, R5 
    /*00002278*/ BL          0x0000000000002278          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*0000227c*/ CMP         R0, #0 
    /*00002280*/ BNE         AoGetTrackMode_x233c
    /*00002284*/ LDR         R3, [R6, #12] 
    /*00002288*/ CMP         R3, #1 
    /*0000228c*/ BNE         AoGetTrackMode_x22b4
    /*00002290*/ MOV         R1, R7 
    /*00002294*/ MOV         R0, R4 
    /*00002298*/ BL          0x0000000000002298          @ AO_DRV_GetTrackMode [R_ARM_CALL: .text+0x7984] 
    /*0000229c*/ MOV         R6, R0 
    /*000022a0*/ MOV         R0, R5 
    /*000022a4*/ BL          0x00000000000022A4          @ osal_up [R_ARM_CALL: +0x0] 
    AoGetTrackMode_x22a8:
    /*000022a8*/ MOV         R0, R6 
    /*000022ac*/ ADD         SP, SP, #12 
    /*000022b0*/ POP         {R4, R5, R6, R7, PC} 
    AoGetTrackMode_x22b4:
    /*000022b4*/ MOV         R0, R5 
    /*000022b8*/ MOVW        R6, #32773                  @ 0x8005 
    /*000022bc*/ BL          0x00000000000022BC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000022c0*/ MOVW        R3, #869                    @ 0x365 
    /*000022c4*/ STR         R4, [SP, #4] 
    /*000022c8*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x330] 
    /*000022cc*/ STR         R3, [SP] 
    /*000022d0*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x330] 
    /*000022d4*/ LDR         R3, [PC, #132]              @ 0x0000000000002360 
    /*000022d8*/ MOV         R1, #22 
    /*000022dc*/ MOV         R0, #3 
    /*000022e0*/ MOVT        R6, #40982                  @ 0xa016 
    /*000022e4*/ BL          0x00000000000022E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000022e8*/ B           AoGetTrackMode_x22a8
    AoGetTrackMode_x22ec:
    /*000022ec*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*000022f0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*000022f4*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*000022f8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*000022fc*/ MOVW        R2, #858                    @ 0x35a 
    /*00002300*/ LDR         R1, [PC, #88]               @ 0x0000000000002360 
    /*00002304*/ BL          0x0000000000002304          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002308*/ B           AoGetTrackMode_x2270
    AoGetTrackMode_x230c:
    /*0000230c*/ MOVW        R3, #854                    @ 0x356 
    /*00002310*/ STR         R4, [SP, #4] 
    /*00002314*/ STR         R3, [SP] 
    /*00002318*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*0000231c*/ LDR         R3, [PC, #60]               @ 0x0000000000002360 
    /*00002320*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00002324*/ MOV         R1, #22 
    /*00002328*/ MOV         R0, #3 
    /*0000232c*/ MOVW        R6, #32769                  @ 0x8001 
    /*00002330*/ BL          0x0000000000002330          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002334*/ MOVT        R6, #40982                  @ 0xa016 
    /*00002338*/ B           AoGetTrackMode_x22a8
    AoGetTrackMode_x233c:
    /*0000233c*/ MOV         R6, #65024                  @ 0xfe00 
    /*00002340*/ MOVT        R6, #65535                  @ 0xffff 
    /*00002344*/ B           AoGetTrackMode_x22a8
    AoGetTrackMode_x2348:
    /*00002348*/ MOVW        R6, #32774                  @ 0x8006 
    /*0000234c*/ MOVT        R6, #40982                  @ 0xa016 
    /*00002350*/ B           AoGetTrackMode_x22a8
    AoGetTrackMode_x2354:
    /*00002354*/ MOVW        R6, #32784                  @ 0x8010 
    /*00002358*/ MOVT        R6, #40982                  @ 0xa016 
    /*0000235c*/ B           AoGetTrackMode_x22a8
    /*00002360*/ ANDEQ       R0, R0, IP, ASR #1 
FUNC_END AoGetTrackMode


FUNC_BEGIN AoSetVolume
    /*00002364*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00002368*/ MOV         R4, R0 
    /*0000236c*/ SUB         SP, SP, #12 
    /*00002370*/ MOV         R0, #20 
    /*00002374*/ MOV         R7, R1 
    /*00002378*/ BL          0x0000000000002378          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000237c*/ CMP         R0, #0 
    /*00002380*/ BEQ         AoSetVolume_x247c
    /*00002384*/ CMP         R4, #1 
    /*00002388*/ BHI         AoSetVolume_x23e0
    /*0000238c*/ MOV         R0, R4 
    /*00002390*/ BL          0x0000000000002390          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*00002394*/ SUBS        R5, R0, #0 
    /*00002398*/ BEQ         AoSetVolume_x2450
    AoSetVolume_x239c:
    /*0000239c*/ ADD         R6, R5, #124                @ 0x7c 
    /*000023a0*/ MOV         R0, R6 
    /*000023a4*/ BL          0x00000000000023A4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000023a8*/ CMP         R0, #0 
    /*000023ac*/ BNE         AoSetVolume_x2470
    /*000023b0*/ LDR         R3, [R5, #12] 
    /*000023b4*/ CMP         R3, #1 
    /*000023b8*/ BNE         AoSetVolume_x2418
    /*000023bc*/ MOV         R1, R7 
    /*000023c0*/ MOV         R0, R4 
    /*000023c4*/ BL          0x00000000000023C4          @ AO_DRV_SetVolume [R_ARM_CALL: .text+0x7b8c] 
    /*000023c8*/ MOV         R5, R0 
    /*000023cc*/ MOV         R0, R6 
    /*000023d0*/ BL          0x00000000000023D0          @ osal_up [R_ARM_CALL: +0x0] 
    AoSetVolume_x23d4:
    /*000023d4*/ MOV         R0, R5 
    /*000023d8*/ ADD         SP, SP, #12 
    /*000023dc*/ POP         {R4, R5, R6, R7, PC} 
    AoSetVolume_x23e0:
    /*000023e0*/ STR         R4, [SP, #4] 
    /*000023e4*/ MOVW        R3, #885                    @ 0x375 
    /*000023e8*/ MOV         R1, #22 
    /*000023ec*/ STR         R3, [SP] 
    /*000023f0*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*000023f4*/ LDR         R3, [PC, #140]              @ 0x0000000000002488 
    /*000023f8*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*000023fc*/ MOV         R0, #3 
    /*00002400*/ BL          0x0000000000002400          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002404*/ MOVW        R5, #32769                  @ 0x8001 
    /*00002408*/ MOVT        R5, #40982                  @ 0xa016 
    /*0000240c*/ MOV         R0, R5 
    /*00002410*/ ADD         SP, SP, #12 
    /*00002414*/ POP         {R4, R5, R6, R7, PC} 
    AoSetVolume_x2418:
    /*00002418*/ MOV         R0, R6 
    /*0000241c*/ MOVW        R5, #32773                  @ 0x8005 
    /*00002420*/ BL          0x0000000000002420          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002424*/ MOVW        R3, #898                    @ 0x382 
    /*00002428*/ STR         R4, [SP, #4] 
    /*0000242c*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x330] 
    /*00002430*/ STR         R3, [SP] 
    /*00002434*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x330] 
    /*00002438*/ LDR         R3, [PC, #72]               @ 0x0000000000002488 
    /*0000243c*/ MOV         R1, #22 
    /*00002440*/ MOV         R0, #3 
    /*00002444*/ MOVT        R5, #40982                  @ 0xa016 
    /*00002448*/ BL          0x0000000000002448          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000244c*/ B           AoSetVolume_x23d4
    AoSetVolume_x2450:
    /*00002450*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*00002454*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00002458*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*0000245c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00002460*/ MOVW        R2, #887                    @ 0x377 
    /*00002464*/ LDR         R1, [PC, #28]               @ 0x0000000000002488 
    /*00002468*/ BL          0x0000000000002468          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000246c*/ B           AoSetVolume_x239c
    AoSetVolume_x2470:
    /*00002470*/ MOV         R5, #65024                  @ 0xfe00 
    /*00002474*/ MOVT        R5, #65535                  @ 0xffff 
    /*00002478*/ B           AoSetVolume_x23d4
    AoSetVolume_x247c:
    /*0000247c*/ MOVW        R5, #32784                  @ 0x8010 
    /*00002480*/ MOVT        R5, #40982                  @ 0xa016 
    /*00002484*/ B           AoSetVolume_x23d4
    /*00002488*/ LDRDEQ      R0, [R0], -IP 
FUNC_END AoSetVolume


FUNC_BEGIN AoGetVolume
    /*0000248c*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00002490*/ MOV         R4, R0 
    /*00002494*/ SUB         SP, SP, #12 
    /*00002498*/ MOV         R0, #20 
    /*0000249c*/ MOV         R7, R1 
    /*000024a0*/ BL          0x00000000000024A0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000024a4*/ CMP         R0, #0 
    /*000024a8*/ BEQ         AoGetVolume_x25b0
    /*000024ac*/ CMP         R4, #1 
    /*000024b0*/ BHI         AoGetVolume_x2568
    /*000024b4*/ CMP         R7, #0 
    /*000024b8*/ BEQ         AoGetVolume_x25a4
    /*000024bc*/ MOV         R0, R4 
    /*000024c0*/ BL          0x00000000000024C0          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*000024c4*/ SUBS        R6, R0, #0 
    /*000024c8*/ BEQ         AoGetVolume_x2548
    AoGetVolume_x24cc:
    /*000024cc*/ ADD         R5, R6, #124                @ 0x7c 
    /*000024d0*/ MOV         R0, R5 
    /*000024d4*/ BL          0x00000000000024D4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000024d8*/ CMP         R0, #0 
    /*000024dc*/ BNE         AoGetVolume_x2598
    /*000024e0*/ LDR         R3, [R6, #12] 
    /*000024e4*/ CMP         R3, #1 
    /*000024e8*/ BNE         AoGetVolume_x2510
    /*000024ec*/ MOV         R1, R7 
    /*000024f0*/ MOV         R0, R4 
    /*000024f4*/ BL          0x00000000000024F4          @ AO_DRV_GetVolume [R_ARM_CALL: .text+0x7c08] 
    /*000024f8*/ MOV         R6, R0 
    /*000024fc*/ MOV         R0, R5 
    /*00002500*/ BL          0x0000000000002500          @ osal_up [R_ARM_CALL: +0x0] 
    AoGetVolume_x2504:
    /*00002504*/ MOV         R0, R6 
    /*00002508*/ ADD         SP, SP, #12 
    /*0000250c*/ POP         {R4, R5, R6, R7, PC} 
    AoGetVolume_x2510:
    /*00002510*/ MOV         R0, R5 
    /*00002514*/ MOVW        R6, #32773                  @ 0x8005 
    /*00002518*/ BL          0x0000000000002518          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000251c*/ MOVW        R3, #929                    @ 0x3a1 
    /*00002520*/ STR         R4, [SP, #4] 
    /*00002524*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x330] 
    /*00002528*/ STR         R3, [SP] 
    /*0000252c*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x330] 
    /*00002530*/ LDR         R3, [PC, #132]              @ 0x00000000000025bc 
    /*00002534*/ MOV         R1, #22 
    /*00002538*/ MOV         R0, #3 
    /*0000253c*/ MOVT        R6, #40982                  @ 0xa016 
    /*00002540*/ BL          0x0000000000002540          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002544*/ B           AoGetVolume_x2504
    AoGetVolume_x2548:
    /*00002548*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*0000254c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00002550*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*00002554*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00002558*/ MOVW        R2, #918                    @ 0x396 
    /*0000255c*/ LDR         R1, [PC, #88]               @ 0x00000000000025bc 
    /*00002560*/ BL          0x0000000000002560          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002564*/ B           AoGetVolume_x24cc
    AoGetVolume_x2568:
    /*00002568*/ MOVW        R3, #914                    @ 0x392 
    /*0000256c*/ STR         R4, [SP, #4] 
    /*00002570*/ STR         R3, [SP] 
    /*00002574*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*00002578*/ LDR         R3, [PC, #60]               @ 0x00000000000025bc 
    /*0000257c*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00002580*/ MOV         R1, #22 
    /*00002584*/ MOV         R0, #3 
    /*00002588*/ MOVW        R6, #32769                  @ 0x8001 
    /*0000258c*/ BL          0x000000000000258C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002590*/ MOVT        R6, #40982                  @ 0xa016 
    /*00002594*/ B           AoGetVolume_x2504
    AoGetVolume_x2598:
    /*00002598*/ MOV         R6, #65024                  @ 0xfe00 
    /*0000259c*/ MOVT        R6, #65535                  @ 0xffff 
    /*000025a0*/ B           AoGetVolume_x2504
    AoGetVolume_x25a4:
    /*000025a4*/ MOVW        R6, #32774                  @ 0x8006 
    /*000025a8*/ MOVT        R6, #40982                  @ 0xa016 
    /*000025ac*/ B           AoGetVolume_x2504
    AoGetVolume_x25b0:
    /*000025b0*/ MOVW        R6, #32784                  @ 0x8010 
    /*000025b4*/ MOVT        R6, #40982                  @ 0xa016 
    /*000025b8*/ B           AoGetVolume_x2504
    /*000025bc*/ ANDEQ       R0, R0, R8, ROR #1 
FUNC_END AoGetVolume


FUNC_BEGIN AoSetClkDir
    /*000025c0*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000025c4*/ MOV         R4, R0 
    /*000025c8*/ SUB         SP, SP, #12 
    /*000025cc*/ MOV         R0, #20 
    /*000025d0*/ MOV         R7, R1 
    /*000025d4*/ BL          0x00000000000025D4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000025d8*/ CMP         R0, #0 
    /*000025dc*/ BEQ         AoSetClkDir_x26e4
    /*000025e0*/ CMP         R4, #1 
    /*000025e4*/ BHI         AoSetClkDir_x2648
    /*000025e8*/ MOV         R0, R4 
    /*000025ec*/ BL          0x00000000000025EC          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*000025f0*/ SUBS        R5, R0, #0 
    /*000025f4*/ BEQ         AoSetClkDir_x2680
    AoSetClkDir_x25f8:
    /*000025f8*/ ADD         R6, R5, #124                @ 0x7c 
    /*000025fc*/ MOV         R0, R6 
    /*00002600*/ BL          0x0000000000002600          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00002604*/ CMP         R0, #0 
    /*00002608*/ BNE         AoSetClkDir_x26d8
    /*0000260c*/ LDR         R3, [R5, #8] 
    /*00002610*/ CMP         R3, #0 
    /*00002614*/ BEQ         AoSetClkDir_x26a0
    /*00002618*/ LDR         R3, [R5, #12] 
    /*0000261c*/ CMP         R3, #1 
    /*00002620*/ BEQ         AoSetClkDir_x26a0
    /*00002624*/ MOV         R1, R7 
    /*00002628*/ MOV         R0, R4 
    /*0000262c*/ BL          0x000000000000262C          @ AO_DRV_SetClkDir [R_ARM_CALL: .text+0x7c78] 
    /*00002630*/ MOV         R5, R0 
    /*00002634*/ MOV         R0, R6 
    /*00002638*/ BL          0x0000000000002638          @ osal_up [R_ARM_CALL: +0x0] 
    AoSetClkDir_x263c:
    /*0000263c*/ MOV         R0, R5 
    /*00002640*/ ADD         SP, SP, #12 
    /*00002644*/ POP         {R4, R5, R6, R7, PC} 
    AoSetClkDir_x2648:
    /*00002648*/ STR         R4, [SP, #4] 
    /*0000264c*/ MOVW        R3, #945                    @ 0x3b1 
    /*00002650*/ MOV         R1, #22 
    /*00002654*/ STR         R3, [SP] 
    /*00002658*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*0000265c*/ LDR         R3, [PC, #140]              @ 0x00000000000026f0 
    /*00002660*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00002664*/ MOV         R0, #3 
    /*00002668*/ BL          0x0000000000002668          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000266c*/ MOVW        R5, #32769                  @ 0x8001 
    /*00002670*/ MOVT        R5, #40982                  @ 0xa016 
    /*00002674*/ MOV         R0, R5 
    /*00002678*/ ADD         SP, SP, #12 
    /*0000267c*/ POP         {R4, R5, R6, R7, PC} 
    AoSetClkDir_x2680:
    /*00002680*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*00002684*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00002688*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*0000268c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00002690*/ MOV         R2, #948                    @ 0x3b4 
    /*00002694*/ LDR         R1, [PC, #84]               @ 0x00000000000026f0 
    /*00002698*/ BL          0x0000000000002698          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000269c*/ B           AoSetClkDir_x25f8
    AoSetClkDir_x26a0:
    /*000026a0*/ MOV         R0, R6 
    /*000026a4*/ MOVW        R5, #32777                  @ 0x8009 
    /*000026a8*/ BL          0x00000000000026A8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000026ac*/ MOVW        R3, #959                    @ 0x3bf 
    /*000026b0*/ STR         R4, [SP, #4] 
    /*000026b4*/ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x82c] 
    /*000026b8*/ STR         R3, [SP] 
    /*000026bc*/ MOVT        R2, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x82c] 
    /*000026c0*/ LDR         R3, [PC, #40]               @ 0x00000000000026f0 
    /*000026c4*/ MOV         R1, #22 
    /*000026c8*/ MOV         R0, #3 
    /*000026cc*/ MOVT        R5, #40982                  @ 0xa016 
    /*000026d0*/ BL          0x00000000000026D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000026d4*/ B           AoSetClkDir_x263c
    AoSetClkDir_x26d8:
    /*000026d8*/ MOV         R5, #65024                  @ 0xfe00 
    /*000026dc*/ MOVT        R5, #65535                  @ 0xffff 
    /*000026e0*/ B           AoSetClkDir_x263c
    AoSetClkDir_x26e4:
    /*000026e4*/ MOVW        R5, #32784                  @ 0x8010 
    /*000026e8*/ MOVT        R5, #40982                  @ 0xa016 
    /*000026ec*/ B           AoSetClkDir_x263c
    /*000026f0*/ STRDEQ      R0, [R0], -R4 
FUNC_END AoSetClkDir


FUNC_BEGIN AoGetClkDir
    /*000026f4*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000026f8*/ MOV         R4, R0 
    /*000026fc*/ SUB         SP, SP, #12 
    /*00002700*/ MOV         R0, #20 
    /*00002704*/ MOV         R6, R1 
    /*00002708*/ BL          0x0000000000002708          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000270c*/ CMP         R0, #0 
    /*00002710*/ BEQ         AoGetClkDir_x2824
    /*00002714*/ CMP         R4, #1 
    /*00002718*/ BHI         AoGetClkDir_x27dc
    /*0000271c*/ CMP         R6, #0 
    /*00002720*/ BEQ         AoGetClkDir_x2818
    /*00002724*/ MOV         R0, R4 
    /*00002728*/ BL          0x0000000000002728          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*0000272c*/ SUBS        R5, R0, #0 
    /*00002730*/ BEQ         AoGetClkDir_x2784
    AoGetClkDir_x2734:
    /*00002734*/ ADD         R7, R5, #124                @ 0x7c 
    /*00002738*/ MOV         R0, R7 
    /*0000273c*/ BL          0x000000000000273C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00002740*/ CMP         R0, #0 
    /*00002744*/ BNE         AoGetClkDir_x280c
    /*00002748*/ LDR         R3, [R5, #8] 
    /*0000274c*/ CMP         R3, #0 
    /*00002750*/ BEQ         AoGetClkDir_x27a4
    /*00002754*/ LDR         R3, [R5, #12] 
    /*00002758*/ CMP         R3, #1 
    /*0000275c*/ BEQ         AoGetClkDir_x27a4
    /*00002760*/ MOV         R1, R6 
    /*00002764*/ MOV         R0, R4 
    /*00002768*/ BL          0x0000000000002768          @ AO_DRV_GetClkDir [R_ARM_CALL: .text+0x7cf4] 
    /*0000276c*/ MOV         R5, R0 
    /*00002770*/ MOV         R0, R7 
    /*00002774*/ BL          0x0000000000002774          @ osal_up [R_ARM_CALL: +0x0] 
    AoGetClkDir_x2778:
    /*00002778*/ MOV         R0, R5 
    /*0000277c*/ ADD         SP, SP, #12 
    /*00002780*/ POP         {R4, R5, R6, R7, PC} 
    AoGetClkDir_x2784:
    /*00002784*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*00002788*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*0000278c*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*00002790*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00002794*/ MOVW        R2, #979                    @ 0x3d3 
    /*00002798*/ LDR         R1, [PC, #144]              @ 0x0000000000002830 
    /*0000279c*/ BL          0x000000000000279C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000027a0*/ B           AoGetClkDir_x2734
    AoGetClkDir_x27a4:
    /*000027a4*/ MOV         R0, R7 
    /*000027a8*/ MOVW        R5, #32777                  @ 0x8009 
    /*000027ac*/ BL          0x00000000000027AC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000027b0*/ MOVW        R3, #990                    @ 0x3de 
    /*000027b4*/ STR         R4, [SP, #4] 
    /*000027b8*/ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x870] 
    /*000027bc*/ STR         R3, [SP] 
    /*000027c0*/ MOVT        R2, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x870] 
    /*000027c4*/ LDR         R3, [PC, #100]              @ 0x0000000000002830 
    /*000027c8*/ MOV         R1, #22 
    /*000027cc*/ MOV         R0, #3 
    /*000027d0*/ MOVT        R5, #40982                  @ 0xa016 
    /*000027d4*/ BL          0x00000000000027D4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000027d8*/ B           AoGetClkDir_x2778
    AoGetClkDir_x27dc:
    /*000027dc*/ MOVW        R3, #975                    @ 0x3cf 
    /*000027e0*/ STR         R4, [SP, #4] 
    /*000027e4*/ STR         R3, [SP] 
    /*000027e8*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*000027ec*/ LDR         R3, [PC, #60]               @ 0x0000000000002830 
    /*000027f0*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*000027f4*/ MOV         R1, #22 
    /*000027f8*/ MOV         R0, #3 
    /*000027fc*/ MOVW        R5, #32769                  @ 0x8001 
    /*00002800*/ BL          0x0000000000002800          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002804*/ MOVT        R5, #40982                  @ 0xa016 
    /*00002808*/ B           AoGetClkDir_x2778
    AoGetClkDir_x280c:
    /*0000280c*/ MOV         R5, #65024                  @ 0xfe00 
    /*00002810*/ MOVT        R5, #65535                  @ 0xffff 
    /*00002814*/ B           AoGetClkDir_x2778
    AoGetClkDir_x2818:
    /*00002818*/ MOVW        R5, #32774                  @ 0x8006 
    /*0000281c*/ MOVT        R5, #40982                  @ 0xa016 
    /*00002820*/ B           AoGetClkDir_x2778
    AoGetClkDir_x2824:
    /*00002824*/ MOVW        R5, #32784                  @ 0x8010 
    /*00002828*/ MOVT        R5, #40982                  @ 0xa016 
    /*0000282c*/ B           AoGetClkDir_x2778
    /*00002830*/ ANDEQ       R0, R0, R0, LSL #2 
FUNC_END AoGetClkDir


FUNC_BEGIN AoSetMute
    /*00002834*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00002838*/ MOV         R4, R0 
    /*0000283c*/ SUB         SP, SP, #12 
    /*00002840*/ MOV         R0, #20 
    /*00002844*/ MOV         R5, R1 
    /*00002848*/ BL          0x0000000000002848          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000284c*/ CMP         R0, #0 
    /*00002850*/ BEQ         AoSetMute_x295c
    /*00002854*/ CMP         R4, #1 
    /*00002858*/ BHI         AoSetMute_x2914
    /*0000285c*/ CMP         R5, #0 
    /*00002860*/ BEQ         AoSetMute_x2950
    /*00002864*/ MOV         R0, R4 
    /*00002868*/ BL          0x0000000000002868          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*0000286c*/ SUBS        R7, R0, #0 
    /*00002870*/ BEQ         AoSetMute_x28f4
    AoSetMute_x2874:
    /*00002874*/ ADD         R6, R7, #124                @ 0x7c 
    /*00002878*/ MOV         R0, R6 
    /*0000287c*/ BL          0x000000000000287C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00002880*/ CMP         R0, #0 
    /*00002884*/ BNE         AoSetMute_x2944
    /*00002888*/ LDR         R3, [R7, #12] 
    /*0000288c*/ CMP         R3, #1 
    /*00002890*/ BNE         AoSetMute_x28bc
    /*00002894*/ ADD         R2, R5, #4 
    /*00002898*/ LDR         R1, [R5] 
    /*0000289c*/ MOV         R0, R4 
    /*000028a0*/ BL          0x00000000000028A0          @ AO_DRV_SetMute [R_ARM_CALL: .text+0x7a2c] 
    /*000028a4*/ MOV         R5, R0 
    /*000028a8*/ MOV         R0, R6 
    /*000028ac*/ BL          0x00000000000028AC          @ osal_up [R_ARM_CALL: +0x0] 
    AoSetMute_x28b0:
    /*000028b0*/ MOV         R0, R5 
    /*000028b4*/ ADD         SP, SP, #12 
    /*000028b8*/ POP         {R4, R5, R6, R7, PC} 
    AoSetMute_x28bc:
    /*000028bc*/ MOV         R0, R6 
    /*000028c0*/ MOVW        R5, #32773                  @ 0x8005 
    /*000028c4*/ BL          0x00000000000028C4          @ osal_up [R_ARM_CALL: +0x0] 
    /*000028c8*/ MOVW        R3, #1022                   @ 0x3fe 
    /*000028cc*/ STR         R4, [SP, #4] 
    /*000028d0*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x330] 
    /*000028d4*/ STR         R3, [SP] 
    /*000028d8*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x330] 
    /*000028dc*/ LDR         R3, [PC, #132]              @ 0x0000000000002968 
    /*000028e0*/ MOV         R1, #22 
    /*000028e4*/ MOV         R0, #3 
    /*000028e8*/ MOVT        R5, #40982                  @ 0xa016 
    /*000028ec*/ BL          0x00000000000028EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000028f0*/ B           AoSetMute_x28b0
    AoSetMute_x28f4:
    /*000028f4*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*000028f8*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*000028fc*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*00002900*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00002904*/ MOVW        R2, #1011                   @ 0x3f3 
    /*00002908*/ LDR         R1, [PC, #88]               @ 0x0000000000002968 
    /*0000290c*/ BL          0x000000000000290C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002910*/ B           AoSetMute_x2874
    AoSetMute_x2914:
    /*00002914*/ MOVW        R3, #1007                   @ 0x3ef 
    /*00002918*/ STR         R4, [SP, #4] 
    /*0000291c*/ STR         R3, [SP] 
    /*00002920*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*00002924*/ LDR         R3, [PC, #60]               @ 0x0000000000002968 
    /*00002928*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*0000292c*/ MOV         R1, #22 
    /*00002930*/ MOV         R0, #3 
    /*00002934*/ MOVW        R5, #32769                  @ 0x8001 
    /*00002938*/ BL          0x0000000000002938          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000293c*/ MOVT        R5, #40982                  @ 0xa016 
    /*00002940*/ B           AoSetMute_x28b0
    AoSetMute_x2944:
    /*00002944*/ MOV         R5, #65024                  @ 0xfe00 
    /*00002948*/ MOVT        R5, #65535                  @ 0xffff 
    /*0000294c*/ B           AoSetMute_x28b0
    AoSetMute_x2950:
    /*00002950*/ MOVW        R5, #32774                  @ 0x8006 
    /*00002954*/ MOVT        R5, #40982                  @ 0xa016 
    /*00002958*/ B           AoSetMute_x28b0
    AoSetMute_x295c:
    /*0000295c*/ MOVW        R5, #32784                  @ 0x8010 
    /*00002960*/ MOVT        R5, #40982                  @ 0xa016 
    /*00002964*/ B           AoSetMute_x28b0
    /*00002968*/ ANDEQ       R0, R0, IP, LSL #2 
FUNC_END AoSetMute


FUNC_BEGIN AoSetSaveFile
    /*0000296c*/ CMP         R0, #1 
    /*00002970*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00002974*/ MOV         R7, R0 
    /*00002978*/ SUB         SP, SP, #44                 @ 0x2c 
    /*0000297c*/ BHI         AoSetSaveFile_x2b10
    /*00002980*/ CMP         R1, #2 
    /*00002984*/ MOV         R8, R1 
    /*00002988*/ BHI         AoSetSaveFile_x2c6c
    /*0000298c*/ CMP         R2, #0 
    /*00002990*/ MOV         R4, R2 
    /*00002994*/ BEQ         AoSetSaveFile_x2c60
    /*00002998*/ ADD         R0, R0, R0, LSL #1 
    /*0000299c*/ ADD         R0, R0, R1 
    /*000029a0*/ BL          0x00000000000029A0          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*000029a4*/ SUBS        R5, R0, #0 
    /*000029a8*/ BEQ         AoSetSaveFile_x2af0
    AoSetSaveFile_x29ac:
    /*000029ac*/ MOV         R0, R5 
    /*000029b0*/ BL          0x00000000000029B0          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000029b4*/ CMP         R0, #0 
    /*000029b8*/ BNE         AoSetSaveFile_x2bb8
    /*000029bc*/ LDR         R3, [R5, #1240]             @ 0x4d8 
    /*000029c0*/ CMP         R3, #1 
    /*000029c4*/ BNE         AoSetSaveFile_x2b80
    /*000029c8*/ LDR         R3, [R4] 
    /*000029cc*/ CMP         R3, #1 
    /*000029d0*/ BHI         AoSetSaveFile_x2b40
    /*000029d4*/ LDR         R9, [R5, #2372]             @ 0x944 
    /*000029d8*/ CMP         R9, #0 
    /*000029dc*/ MOVEQ       R9, #0 
    /*000029e0*/ ANDNE       R9, R3, #1 
    /*000029e4*/ CMP         R9, #0 
    /*000029e8*/ BNE         AoSetSaveFile_x2d20
    /*000029ec*/ LDRB        R3, [R4, #4] 
    /*000029f0*/ CMP         R3, #0 
    /*000029f4*/ BEQ         AoSetSaveFile_x2bc4
    /*000029f8*/ ADD         R6, R4, #4 
    /*000029fc*/ MOV         R1, #256                    @ 0x100 
    /*00002a00*/ MOV         R0, R6 
    /*00002a04*/ BL          0x0000000000002A04          @ osal_strnlen [R_ARM_CALL: +0x0] 
    /*00002a08*/ CMP         R0, #255                    @ 0xff 
    /*00002a0c*/ BGT         AoSetSaveFile_x2c9c
    /*00002a10*/ LDRB        R3, [R4, #260]              @ 0x104 
    /*00002a14*/ CMP         R3, #0 
    /*00002a18*/ BEQ         AoSetSaveFile_x2bfc
    /*00002a1c*/ ADD         R6, R4, #260                @ 0x104 
    /*00002a20*/ MOV         R1, #256                    @ 0x100 
    /*00002a24*/ MOV         R0, R6 
    /*00002a28*/ BL          0x0000000000002A28          @ osal_strnlen [R_ARM_CALL: +0x0] 
    /*00002a2c*/ CMP         R0, #255                    @ 0xff 
    /*00002a30*/ BGT         AoSetSaveFile_x2cec
    /*00002a34*/ LDR         R3, [R4, #516]              @ 0x204 
    /*00002a38*/ SUB         R3, R3, #1 
    /*00002a3c*/ CMP         R3, #10240                  @ 0x2800 
    /*00002a40*/ BCS         AoSetSaveFile_x2c1c
    /*00002a44*/ ADD         R6, SP, #40                 @ 0x28 
    /*00002a48*/ MOV         R0, #2 
    /*00002a4c*/ MOV         R3, #22 
    /*00002a50*/ STR         R7, [SP, #32] 
    /*00002a54*/ STR         R9, [R6, #-24]!             @ 0xffffffe8 
    /*00002a58*/ STR         R3, [SP, #28] 
    /*00002a5c*/ STR         R8, [SP, #36]               @ 0x24 
    /*00002a60*/ BL          0x0000000000002A60          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002a64*/ MOV         R1, R6 
    /*00002a68*/ LDR         R3, [R0, #56]               @ 0x38 
    /*00002a6c*/ ADD         R0, SP, #28 
    /*00002a70*/ BLX         R3 
    /*00002a74*/ CMP         R0, #0 
    /*00002a78*/ BEQ         AoSetSaveFile_x2ab0
    AoSetSaveFile_x2a7c:
    /*00002a7c*/ LDR         R3, [R5, #1268]             @ 0x4f4 
    /*00002a80*/ CMP         R3, #0 
    /*00002a84*/ BEQ         AoSetSaveFile_x2c44
    /*00002a88*/ ADD         R0, R5, #2368               @ 0x940 
    /*00002a8c*/ MOV         R1, R4 
    /*00002a90*/ MOV         R2, #520                    @ 0x208 
    /*00002a94*/ ADD         R0, R0, #4 
    /*00002a98*/ BL          0x0000000000002A98          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002a9c*/ MOV         R0, R5 
    /*00002aa0*/ BL          0x0000000000002AA0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002aa4*/ MOV         R0, #0 
    AoSetSaveFile_x2aa8:
    /*00002aa8*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00002aac*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AoSetSaveFile_x2ab0:
    /*00002ab0*/ LDR         R3, [SP, #16] 
    /*00002ab4*/ CMP         R3, #21 
    /*00002ab8*/ BNE         AoSetSaveFile_x2a7c
    /*00002abc*/ MOV         R0, R5 
    /*00002ac0*/ BL          0x0000000000002AC0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002ac4*/ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb10] 
    /*00002ac8*/ MOVT        R2, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb10] 
    /*00002acc*/ MOVW        R3, #1125                   @ 0x465 
    /*00002ad0*/ STR         R3, [SP] 
    AoSetSaveFile_x2ad4:
    /*00002ad4*/ LDR         R3, [PC, #636]              @ 0x0000000000002d58 
    /*00002ad8*/ MOV         R1, #22 
    /*00002adc*/ MOV         R0, #3 
    /*00002ae0*/ BL          0x0000000000002AE0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002ae4*/ MOVW        R0, #32777                  @ 0x8009 
    /*00002ae8*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002aec*/ B           AoSetSaveFile_x2aa8
    AoSetSaveFile_x2af0:
    /*00002af0*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*00002af4*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00002af8*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00002afc*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00002b00*/ MOVW        R2, #1050                   @ 0x41a 
    /*00002b04*/ LDR         R1, [PC, #588]              @ 0x0000000000002d58 
    /*00002b08*/ BL          0x0000000000002B08          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002b0c*/ B           AoSetSaveFile_x29ac
    AoSetSaveFile_x2b10:
    /*00002b10*/ STR         R0, [SP, #4] 
    /*00002b14*/ MOVW        R3, #1044                   @ 0x414 
    /*00002b18*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*00002b1c*/ STR         R3, [SP] 
    /*00002b20*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00002b24*/ LDR         R3, [PC, #556]              @ 0x0000000000002d58 
    /*00002b28*/ MOV         R1, #22 
    /*00002b2c*/ MOV         R0, #3 
    /*00002b30*/ BL          0x0000000000002B30          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002b34*/ MOVW        R0, #32769                  @ 0x8001 
    /*00002b38*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002b3c*/ B           AoSetSaveFile_x2aa8
    AoSetSaveFile_x2b40:
    /*00002b40*/ MOV         R0, R5 
    /*00002b44*/ BL          0x0000000000002B44          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002b48*/ LDR         R3, [R4] 
    /*00002b4c*/ MOVW        R2, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8e8] 
    /*00002b50*/ MOVT        R2, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8e8] 
    /*00002b54*/ MOVW        R1, #1068                   @ 0x42c 
    /*00002b58*/ STR         R8, [SP, #4] 
    /*00002b5c*/ STR         R1, [SP] 
    /*00002b60*/ STR         R3, [SP, #8] 
    AoSetSaveFile_x2b64:
    /*00002b64*/ LDR         R3, [PC, #492]              @ 0x0000000000002d58 
    /*00002b68*/ MOV         R1, #22 
    /*00002b6c*/ MOV         R0, #3 
    /*00002b70*/ BL          0x0000000000002B70          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002b74*/ MOVW        R0, #32771                  @ 0x8003 
    /*00002b78*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002b7c*/ B           AoSetSaveFile_x2aa8
    AoSetSaveFile_x2b80:
    /*00002b80*/ MOV         R0, R5 
    /*00002b84*/ BL          0x0000000000002B84          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002b88*/ MOVW        R3, #1061                   @ 0x425 
    /*00002b8c*/ STR         R8, [SP, #4] 
    /*00002b90*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8b4] 
    /*00002b94*/ STR         R3, [SP] 
    /*00002b98*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8b4] 
    /*00002b9c*/ LDR         R3, [PC, #436]              @ 0x0000000000002d58 
    /*00002ba0*/ MOV         R1, #22 
    /*00002ba4*/ MOV         R0, #3 
    /*00002ba8*/ BL          0x0000000000002BA8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002bac*/ MOVW        R0, #32773                  @ 0x8005 
    /*00002bb0*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002bb4*/ B           AoSetSaveFile_x2aa8
    AoSetSaveFile_x2bb8:
    /*00002bb8*/ MOV         R0, #65024                  @ 0xfe00 
    /*00002bbc*/ MOVT        R0, #65535                  @ 0xffff 
    /*00002bc0*/ B           AoSetSaveFile_x2aa8
    AoSetSaveFile_x2bc4:
    /*00002bc4*/ MOV         R0, R5 
    /*00002bc8*/ BL          0x0000000000002BC8          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002bcc*/ MOVW        R2, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x980] 
    /*00002bd0*/ MOVT        R2, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x980] 
    /*00002bd4*/ MOVW        R3, #1082                   @ 0x43a 
    /*00002bd8*/ STR         R8, [SP, #4] 
    /*00002bdc*/ STR         R3, [SP] 
    AoSetSaveFile_x2be0:
    /*00002be0*/ LDR         R3, [PC, #368]              @ 0x0000000000002d58 
    /*00002be4*/ MOV         R1, #22 
    /*00002be8*/ MOV         R0, #3 
    /*00002bec*/ BL          0x0000000000002BEC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002bf0*/ MOVW        R0, #32771                  @ 0x8003 
    /*00002bf4*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002bf8*/ B           AoSetSaveFile_x2aa8
    AoSetSaveFile_x2bfc:
    /*00002bfc*/ MOV         R0, R5 
    /*00002c00*/ BL          0x0000000000002C00          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002c04*/ MOVW        R3, #1097                   @ 0x449 
    /*00002c08*/ MOVW        R2, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa18] 
    /*00002c0c*/ STR         R8, [SP, #4] 
    /*00002c10*/ STR         R3, [SP] 
    /*00002c14*/ MOVT        R2, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa18] 
    /*00002c18*/ B           AoSetSaveFile_x2be0
    AoSetSaveFile_x2c1c:
    /*00002c1c*/ MOV         R0, R5 
    /*00002c20*/ BL          0x0000000000002C20          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002c24*/ LDR         R3, [R4, #516]              @ 0x204 
    /*00002c28*/ MOVW        R1, #1112                   @ 0x458 
    /*00002c2c*/ MOVW        R2, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xab0] 
    /*00002c30*/ STR         R8, [SP, #4] 
    /*00002c34*/ MOVT        R2, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0xab0] 
    /*00002c38*/ STR         R1, [SP] 
    /*00002c3c*/ STR         R3, [SP, #8] 
    /*00002c40*/ B           AoSetSaveFile_x2b64
    AoSetSaveFile_x2c44:
    /*00002c44*/ MOV         R0, R5 
    /*00002c48*/ BL          0x0000000000002C48          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002c4c*/ MOVW        R3, #1134                   @ 0x46e 
    /*00002c50*/ MOVW        R2, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb50] 
    /*00002c54*/ STR         R3, [SP] 
    /*00002c58*/ MOVT        R2, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb50] 
    /*00002c5c*/ B           AoSetSaveFile_x2ad4
    AoSetSaveFile_x2c60:
    /*00002c60*/ MOVW        R0, #32774                  @ 0x8006 
    /*00002c64*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002c68*/ B           AoSetSaveFile_x2aa8
    AoSetSaveFile_x2c6c:
    /*00002c6c*/ STR         R1, [SP, #4] 
    /*00002c70*/ MOVW        R3, #1045                   @ 0x415 
    /*00002c74*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*00002c78*/ STR         R3, [SP] 
    /*00002c7c*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*00002c80*/ LDR         R3, [PC, #208]              @ 0x0000000000002d58 
    /*00002c84*/ MOV         R1, #20 
    /*00002c88*/ MOV         R0, #3 
    /*00002c8c*/ BL          0x0000000000002C8C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002c90*/ MOVW        R0, #32770                  @ 0x8002 
    /*00002c94*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002c98*/ B           AoSetSaveFile_x2aa8
    AoSetSaveFile_x2c9c:
    /*00002c9c*/ MOV         R0, R5 
    /*00002ca0*/ BL          0x0000000000002CA0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002ca4*/ MOV         R0, R6 
    /*00002ca8*/ MOV         R1, #256                    @ 0x100 
    /*00002cac*/ BL          0x0000000000002CAC          @ osal_strnlen [R_ARM_CALL: +0x0] 
    /*00002cb0*/ MOV         R2, #256                    @ 0x100 
    /*00002cb4*/ STR         R2, [SP, #12] 
    /*00002cb8*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9c4] 
    /*00002cbc*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9c4] 
    /*00002cc0*/ MOVW        R3, #1090                   @ 0x442 
    /*00002cc4*/ STR         R8, [SP, #4] 
    /*00002cc8*/ STR         R3, [SP] 
    AoSetSaveFile_x2ccc:
    /*00002ccc*/ STR         R0, [SP, #8] 
    /*00002cd0*/ MOV         R1, #22 
    /*00002cd4*/ LDR         R3, [PC, #124]              @ 0x0000000000002d58 
    /*00002cd8*/ MOV         R0, #3 
    /*00002cdc*/ BL          0x0000000000002CDC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002ce0*/ MOVW        R0, #32771                  @ 0x8003 
    /*00002ce4*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002ce8*/ B           AoSetSaveFile_x2aa8
    AoSetSaveFile_x2cec:
    /*00002cec*/ MOV         R0, R5 
    /*00002cf0*/ BL          0x0000000000002CF0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002cf4*/ MOV         R0, R6 
    /*00002cf8*/ MOV         R1, #256                    @ 0x100 
    /*00002cfc*/ BL          0x0000000000002CFC          @ osal_strnlen [R_ARM_CALL: +0x0] 
    /*00002d00*/ MOV         R2, #256                    @ 0x100 
    /*00002d04*/ MOVW        R3, #1105                   @ 0x451 
    /*00002d08*/ STR         R2, [SP, #12] 
    /*00002d0c*/ STR         R8, [SP, #4] 
    /*00002d10*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa5c] 
    /*00002d14*/ STR         R3, [SP] 
    /*00002d18*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa5c] 
    /*00002d1c*/ B           AoSetSaveFile_x2ccc
    AoSetSaveFile_x2d20:
    /*00002d20*/ MOV         R0, R5 
    /*00002d24*/ BL          0x0000000000002D24          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002d28*/ MOVW        R3, #1075                   @ 0x433 
    /*00002d2c*/ STR         R8, [SP, #4] 
    /*00002d30*/ MOVW        R2, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x93c] 
    /*00002d34*/ STR         R3, [SP] 
    /*00002d38*/ MOVT        R2, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x93c] 
    /*00002d3c*/ LDR         R3, [PC, #20]               @ 0x0000000000002d58 
    /*00002d40*/ MOV         R1, #22 
    /*00002d44*/ MOV         R0, #3 
    /*00002d48*/ BL          0x0000000000002D48          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002d4c*/ MOVW        R0, #32777                  @ 0x8009 
    /*00002d50*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002d54*/ B           AoSetSaveFile_x2aa8
    /*00002d58*/ ANDEQ       R0, R0, R8, LSL R1 
FUNC_END AoSetSaveFile


FUNC_BEGIN AoGetSaveFile
    /*00002d5c*/ CMP         R0, #1 
    /*00002d60*/ PUSH        {R4, R5, R6, LR} 
    /*00002d64*/ SUB         SP, SP, #8 
    /*00002d68*/ BHI         AoGetSaveFile_x2df8
    /*00002d6c*/ CMP         R1, #2 
    /*00002d70*/ BHI         AoGetSaveFile_x2e34
    /*00002d74*/ CMP         R2, #0 
    /*00002d78*/ MOV         R6, R2 
    /*00002d7c*/ BEQ         AoGetSaveFile_x2e28
    /*00002d80*/ ADD         R0, R0, R0, LSL #1 
    /*00002d84*/ ADD         R0, R0, R1 
    /*00002d88*/ BL          0x0000000000002D88          @ AO_GetChnCtx [R_ARM_CALL: .text+0x4b48] 
    /*00002d8c*/ SUBS        R5, R0, #0 
    /*00002d90*/ BEQ         AoGetSaveFile_x2dd8
    AoGetSaveFile_x2d94:
    /*00002d94*/ MOV         R0, R5 
    /*00002d98*/ BL          0x0000000000002D98          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00002d9c*/ SUBS        R4, R0, #0 
    /*00002da0*/ BNE         AoGetSaveFile_x2dcc
    /*00002da4*/ ADD         R1, R5, #2368               @ 0x940 
    /*00002da8*/ MOV         R2, #520                    @ 0x208 
    /*00002dac*/ ADD         R1, R1, #4 
    /*00002db0*/ MOV         R0, R6 
    /*00002db4*/ BL          0x0000000000002DB4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002db8*/ MOV         R0, R5 
    /*00002dbc*/ BL          0x0000000000002DBC          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002dc0*/ MOV         R0, R4 
    AoGetSaveFile_x2dc4:
    /*00002dc4*/ ADD         SP, SP, #8 
    /*00002dc8*/ POP         {R4, R5, R6, PC} 
    AoGetSaveFile_x2dcc:
    /*00002dcc*/ MOV         R0, #65024                  @ 0xfe00 
    /*00002dd0*/ MOVT        R0, #65535                  @ 0xffff 
    /*00002dd4*/ B           AoGetSaveFile_x2dc4
    AoGetSaveFile_x2dd8:
    /*00002dd8*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x244] 
    /*00002ddc*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00002de0*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x244] 
    /*00002de4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00002de8*/ MOVW        R2, #1157                   @ 0x485 
    /*00002dec*/ LDR         R1, [PC, #112]              @ 0x0000000000002e64 
    /*00002df0*/ BL          0x0000000000002DF0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002df4*/ B           AoGetSaveFile_x2d94
    AoGetSaveFile_x2df8:
    /*00002df8*/ STR         R0, [SP, #4] 
    /*00002dfc*/ MOVW        R3, #1151                   @ 0x47f 
    /*00002e00*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*00002e04*/ STR         R3, [SP] 
    /*00002e08*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00002e0c*/ LDR         R3, [PC, #80]               @ 0x0000000000002e64 
    /*00002e10*/ MOV         R1, #22 
    /*00002e14*/ MOV         R0, #3 
    /*00002e18*/ BL          0x0000000000002E18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002e1c*/ MOVW        R0, #32769                  @ 0x8001 
    /*00002e20*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002e24*/ B           AoGetSaveFile_x2dc4
    AoGetSaveFile_x2e28:
    /*00002e28*/ MOVW        R0, #32774                  @ 0x8006 
    /*00002e2c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002e30*/ B           AoGetSaveFile_x2dc4
    AoGetSaveFile_x2e34:
    /*00002e34*/ STR         R1, [SP, #4] 
    /*00002e38*/ MOV         R3, #1152                   @ 0x480 
    /*00002e3c*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa8] 
    /*00002e40*/ STR         R3, [SP] 
    /*00002e44*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa8] 
    /*00002e48*/ LDR         R3, [PC, #20]               @ 0x0000000000002e64 
    /*00002e4c*/ MOV         R1, #20 
    /*00002e50*/ MOV         R0, #3 
    /*00002e54*/ BL          0x0000000000002E54          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002e58*/ MOVW        R0, #32770                  @ 0x8002 
    /*00002e5c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002e60*/ B           AoGetSaveFile_x2dc4
    /*00002e64*/ ANDEQ       R0, R0, R8, LSR #2 
FUNC_END AoGetSaveFile


FUNC_BEGIN AoGetMute
    /*00002e68*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00002e6c*/ MOV         R4, R0 
    /*00002e70*/ SUB         SP, SP, #12 
    /*00002e74*/ MOV         R0, #20 
    /*00002e78*/ MOV         R5, R1 
    /*00002e7c*/ BL          0x0000000000002E7C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002e80*/ CMP         R0, #0 
    /*00002e84*/ BEQ         AoGetMute_x2f90
    /*00002e88*/ CMP         R4, #1 
    /*00002e8c*/ BHI         AoGetMute_x2f48
    /*00002e90*/ CMP         R5, #0 
    /*00002e94*/ BEQ         AoGetMute_x2f84
    /*00002e98*/ MOV         R0, R4 
    /*00002e9c*/ BL          0x0000000000002E9C          @ AO_GetDevCtx [R_ARM_CALL: .text+0x4b64] 
    /*00002ea0*/ SUBS        R7, R0, #0 
    /*00002ea4*/ BEQ         AoGetMute_x2f28
    AoGetMute_x2ea8:
    /*00002ea8*/ ADD         R6, R7, #124                @ 0x7c 
    /*00002eac*/ MOV         R0, R6 
    /*00002eb0*/ BL          0x0000000000002EB0          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00002eb4*/ CMP         R0, #0 
    /*00002eb8*/ BNE         AoGetMute_x2f78
    /*00002ebc*/ LDR         R3, [R7, #12] 
    /*00002ec0*/ CMP         R3, #1 
    /*00002ec4*/ BNE         AoGetMute_x2ef0
    /*00002ec8*/ ADD         R2, R5, #4 
    /*00002ecc*/ MOV         R1, R5 
    /*00002ed0*/ MOV         R0, R4 
    /*00002ed4*/ BL          0x0000000000002ED4          @ AO_DRV_GetMute [R_ARM_CALL: .text+0x7ad4] 
    /*00002ed8*/ MOV         R5, R0 
    /*00002edc*/ MOV         R0, R6 
    /*00002ee0*/ BL          0x0000000000002EE0          @ osal_up [R_ARM_CALL: +0x0] 
    AoGetMute_x2ee4:
    /*00002ee4*/ MOV         R0, R5 
    /*00002ee8*/ ADD         SP, SP, #12 
    /*00002eec*/ POP         {R4, R5, R6, R7, PC} 
    AoGetMute_x2ef0:
    /*00002ef0*/ MOV         R0, R6 
    /*00002ef4*/ MOVW        R5, #32773                  @ 0x8005 
    /*00002ef8*/ BL          0x0000000000002EF8          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002efc*/ MOVW        R3, #1193                   @ 0x4a9 
    /*00002f00*/ STR         R4, [SP, #4] 
    /*00002f04*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x330] 
    /*00002f08*/ STR         R3, [SP] 
    /*00002f0c*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x330] 
    /*00002f10*/ LDR         R3, [PC, #132]              @ 0x0000000000002f9c 
    /*00002f14*/ MOV         R1, #22 
    /*00002f18*/ MOV         R0, #3 
    /*00002f1c*/ MOVT        R5, #40982                  @ 0xa016 
    /*00002f20*/ BL          0x0000000000002F20          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002f24*/ B           AoGetMute_x2ee4
    AoGetMute_x2f28:
    /*00002f28*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdc] 
    /*00002f2c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*00002f30*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdc] 
    /*00002f34*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*00002f38*/ MOVW        R2, #1182                   @ 0x49e 
    /*00002f3c*/ LDR         R1, [PC, #88]               @ 0x0000000000002f9c 
    /*00002f40*/ BL          0x0000000000002F40          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002f44*/ B           AoGetMute_x2ea8
    AoGetMute_x2f48:
    /*00002f48*/ MOVW        R3, #1178                   @ 0x49a 
    /*00002f4c*/ STR         R4, [SP, #4] 
    /*00002f50*/ STR         R3, [SP] 
    /*00002f54*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*00002f58*/ LDR         R3, [PC, #60]               @ 0x0000000000002f9c 
    /*00002f5c*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*00002f60*/ MOV         R1, #22 
    /*00002f64*/ MOV         R0, #3 
    /*00002f68*/ MOVW        R5, #32769                  @ 0x8001 
    /*00002f6c*/ BL          0x0000000000002F6C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002f70*/ MOVT        R5, #40982                  @ 0xa016 
    /*00002f74*/ B           AoGetMute_x2ee4
    AoGetMute_x2f78:
    /*00002f78*/ MOV         R5, #65024                  @ 0xfe00 
    /*00002f7c*/ MOVT        R5, #65535                  @ 0xffff 
    /*00002f80*/ B           AoGetMute_x2ee4
    AoGetMute_x2f84:
    /*00002f84*/ MOVW        R5, #32774                  @ 0x8006 
    /*00002f88*/ MOVT        R5, #40982                  @ 0xa016 
    /*00002f8c*/ B           AoGetMute_x2ee4
    AoGetMute_x2f90:
    /*00002f90*/ MOVW        R5, #32784                  @ 0x8010 
    /*00002f94*/ MOVT        R5, #40982                  @ 0xa016 
    /*00002f98*/ B           AoGetMute_x2ee4
    /*00002f9c*/ ANDEQ       R0, R0, R8, LSR R1 
FUNC_END AoGetMute


FUNC_BEGIN AoIoctl
    /*00002fa0*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00002fa4*/ MOVW        R3, #21846                  @ 0x5556 
    /*00002fa8*/ LDR         IP, [R2] 
    /*00002fac*/ MOVT        R3, #21845                  @ 0x5555 
    /*00002fb0*/ MOV         R6, R0 
    /*00002fb4*/ MOV         R8, R1 
    /*00002fb8*/ MOV         R7, R2 
    /*00002fbc*/ MOVW        R2, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00002fc0*/ SMULL       R0, R1, IP, R3 
    /*00002fc4*/ MOVT        R2, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00002fc8*/ SUB         SP, SP, #12 
    /*00002fcc*/ LDR         R4, [R2] 
    /*00002fd0*/ SUB         R9, R1, IP, ASR #31 
    /*00002fd4*/ CMP         R4, #0 
    /*00002fd8*/ ADD         R5, R9, R9, LSL #1 
    /*00002fdc*/ SUB         R5, IP, R5 
    /*00002fe0*/ BNE         AoIoctl_x3338
    /*00002fe4*/ LDR         R0, [PC, #1076]             @ 0x0000000000003420 
    /*00002fe8*/ BL          0x0000000000002FE8          @ osal_atomic_inc_return [R_ARM_CALL: +0x0] 
    /*00002fec*/ MOVW        R3, #22542                  @ 0x580e 
    /*00002ff0*/ MOVT        R3, #16400                  @ 0x4010 
    /*00002ff4*/ CMP         R6, R3 
    /*00002ff8*/ BEQ         AoIoctl_x3364
    /*00002ffc*/ BLS         AoIoctl_x3080
    /*00003000*/ MOVW        R3, #22549                  @ 0x5815 
    /*00003004*/ MOVT        R3, #32772                  @ 0x8004 
    /*00003008*/ CMP         R6, R3 
    /*0000300c*/ BEQ         AoIoctl_x33bc
    /*00003010*/ BHI         AoIoctl_x30f0
    /*00003014*/ MOVW        R3, #22553                  @ 0x5819 
    /*00003018*/ MOVT        R3, #16904                  @ 0x4208 
    /*0000301c*/ CMP         R6, R3 
    /*00003020*/ BEQ         AoIoctl_x3298
    /*00003024*/ BLS         AoIoctl_x3210
    /*00003028*/ MOVW        R3, #22545                  @ 0x5811 
    /*0000302c*/ MOVT        R3, #32772                  @ 0x8004 
    /*00003030*/ CMP         R6, R3 
    /*00003034*/ BEQ         AoIoctl_x33d0
    /*00003038*/ MOVW        R3, #22547                  @ 0x5813 
    /*0000303c*/ MOVT        R3, #32772                  @ 0x8004 
    /*00003040*/ CMP         R6, R3 
    /*00003044*/ BEQ         AoIoctl_x330c
    /*00003048*/ MOVW        R3, #22543                  @ 0x580f 
    /*0000304c*/ MOVT        R3, #17472                  @ 0x4440 
    /*00003050*/ CMP         R6, R3 
    /*00003054*/ BEQ         AoIoctl_x3390
    AoIoctl_x3058:
    /*00003058*/ MOVW        R3, #1377                   @ 0x561 
    /*0000305c*/ MOVW        R2, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbd0] 
    /*00003060*/ STR         R3, [SP] 
    /*00003064*/ MOVT        R2, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbd0] 
    /*00003068*/ LDR         R3, [PC, #948]              @ 0x0000000000003424 
    /*0000306c*/ MOV         R1, #22 
    /*00003070*/ MOV         R0, #3 
    /*00003074*/ MVN         R4, #0 
    /*00003078*/ BL          0x0000000000003078          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000307c*/ B           AoIoctl_x3240
    AoIoctl_x3080:
    /*00003080*/ MOVW        R3, #22537                  @ 0x5809 
    /*00003084*/ CMP         R6, R3 
    /*00003088*/ BEQ         AoIoctl_x33a8
    /*0000308c*/ BLS         AoIoctl_x314c
    /*00003090*/ MOV         R3, #22528                  @ 0x5800 
    /*00003094*/ MOVT        R3, #16388                  @ 0x4004 
    /*00003098*/ CMP         R6, R3 
    /*0000309c*/ BEQ         AoIoctl_x328c
    /*000030a0*/ BLS         AoIoctl_x31b0
    /*000030a4*/ MOVW        R3, #22546                  @ 0x5812 
    /*000030a8*/ MOVT        R3, #16388                  @ 0x4004 
    /*000030ac*/ CMP         R6, R3 
    /*000030b0*/ BEQ         AoIoctl_x33f8
    /*000030b4*/ MOVW        R3, #22548                  @ 0x5814 
    /*000030b8*/ MOVT        R3, #16388                  @ 0x4004 
    /*000030bc*/ CMP         R6, R3 
    /*000030c0*/ BEQ         AoIoctl_x32f0
    /*000030c4*/ MOVW        R3, #22544                  @ 0x5810 
    /*000030c8*/ MOVT        R3, #16388                  @ 0x4004 
    /*000030cc*/ CMP         R6, R3 
    /*000030d0*/ BNE         AoIoctl_x3058
    /*000030d4*/ CMP         R8, #0 
    /*000030d8*/ BEQ         AoIoctl_x3414
    /*000030dc*/ LDR         R1, [R8] 
    /*000030e0*/ MOV         R0, R9 
    /*000030e4*/ BL          0x00000000000030E4          @ AoSetTrackMode [R_ARM_CALL: .text+0x2108] 
    /*000030e8*/ MOV         R4, R0 
    /*000030ec*/ B           AoIoctl_x3240
    AoIoctl_x30f0:
    /*000030f0*/ MOVW        R3, #22530                  @ 0x5802 
    /*000030f4*/ MOVT        R3, #32808                  @ 0x8028 
    /*000030f8*/ CMP         R6, R3 
    /*000030fc*/ BEQ         AoIoctl_x3278
    /*00003100*/ BLS         AoIoctl_x31d8
    /*00003104*/ MOVW        R3, #22551                  @ 0x5817 
    /*00003108*/ MOVT        R3, #49168                  @ 0xc010 
    /*0000310c*/ CMP         R6, R3 
    /*00003110*/ BEQ         AoIoctl_x33e4
    /*00003114*/ MOVW        R3, #22533                  @ 0x5805 
    /*00003118*/ MOVT        R3, #49288                  @ 0xc088 
    /*0000311c*/ CMP         R6, R3 
    /*00003120*/ BEQ         AoIoctl_x3320
    /*00003124*/ MOVW        R3, #22554                  @ 0x581a 
    /*00003128*/ MOVT        R3, #33288                  @ 0x8208 
    /*0000312c*/ CMP         R6, R3 
    /*00003130*/ BNE         AoIoctl_x3058
    /*00003134*/ MOV         R2, R8 
    /*00003138*/ MOV         R1, R5 
    /*0000313c*/ MOV         R0, R9 
    /*00003140*/ BL          0x0000000000003140          @ AoGetSaveFile [R_ARM_CALL: .text+0x2d5c] 
    /*00003144*/ MOV         R4, R0 
    /*00003148*/ B           AoIoctl_x3240
    AoIoctl_x314c:
    /*0000314c*/ MOVW        R3, #22534                  @ 0x5806 
    /*00003150*/ CMP         R6, R3 
    /*00003154*/ BEQ         AoIoctl_x337c
    /*00003158*/ BHI         AoIoctl_x3184
    /*0000315c*/ MOVW        R3, #22531                  @ 0x5803 
    /*00003160*/ CMP         R6, R3 
    /*00003164*/ BEQ         AoIoctl_x3254
    /*00003168*/ MOVW        R3, #22532                  @ 0x5804 
    /*0000316c*/ CMP         R6, R3 
    /*00003170*/ BNE         AoIoctl_x3058
    /*00003174*/ MOV         R0, R9 
    /*00003178*/ BL          0x0000000000003178          @ AoDisableDev [R_ARM_CALL: .text+0x45ec] 
    /*0000317c*/ MOV         R4, R0 
    /*00003180*/ B           AoIoctl_x3240
    AoIoctl_x3184:
    /*00003184*/ MOVW        R3, #22535                  @ 0x5807 
    /*00003188*/ CMP         R6, R3 
    /*0000318c*/ BEQ         AoIoctl_x3264
    /*00003190*/ MOVW        R3, #22536                  @ 0x5808 
    /*00003194*/ CMP         R6, R3 
    /*00003198*/ BNE         AoIoctl_x3058
    /*0000319c*/ MOV         R1, R5 
    /*000031a0*/ MOV         R0, R9 
    /*000031a4*/ BL          0x00000000000031A4          @ AOPauseChn [R_ARM_CALL: .text+0xed8] 
    /*000031a8*/ MOV         R4, R0 
    /*000031ac*/ B           AoIoctl_x3240
    AoIoctl_x31b0:
    /*000031b0*/ MOVW        R3, #22540                  @ 0x580c 
    /*000031b4*/ CMP         R6, R3 
    /*000031b8*/ BEQ         AoIoctl_x32dc
    /*000031bc*/ MOVW        R3, #22552                  @ 0x5818 
    /*000031c0*/ CMP         R6, R3 
    /*000031c4*/ BNE         AoIoctl_x3058
    /*000031c8*/ MOV         R0, R9 
    /*000031cc*/ BL          0x00000000000031CC          @ AoClrDevAttr [R_ARM_CALL: .text+0x4b80] 
    /*000031d0*/ MOV         R4, R0 
    /*000031d4*/ B           AoIoctl_x3240
    AoIoctl_x31d8:
    /*000031d8*/ MOVW        R3, #22555                  @ 0x581b 
    /*000031dc*/ MOVT        R3, #32772                  @ 0x8004 
    /*000031e0*/ CMP         R6, R3 
    /*000031e4*/ BEQ         AoIoctl_x32b0
    /*000031e8*/ MOVW        R3, #22541                  @ 0x580d 
    /*000031ec*/ MOVT        R3, #32780                  @ 0x800c 
    /*000031f0*/ CMP         R6, R3 
    /*000031f4*/ BNE         AoIoctl_x3058
    /*000031f8*/ MOV         R2, R8 
    /*000031fc*/ MOV         R1, R5 
    /*00003200*/ MOV         R0, R9 
    /*00003204*/ BL          0x0000000000003204          @ AOQueryChnState [R_ARM_CALL: .text+0x171c] 
    /*00003208*/ MOV         R4, R0 
    /*0000320c*/ B           AoIoctl_x3240
    AoIoctl_x3210:
    /*00003210*/ MOVW        R3, #22550                  @ 0x5816 
    /*00003214*/ MOVT        R3, #16400                  @ 0x4010 
    /*00003218*/ CMP         R6, R3 
    /*0000321c*/ BEQ         AoIoctl_x32c8
    /*00003220*/ MOVW        R3, #22529                  @ 0x5801 
    /*00003224*/ MOVT        R3, #16424                  @ 0x4028 
    /*00003228*/ CMP         R6, R3 
    /*0000322c*/ BNE         AoIoctl_x3058
    /*00003230*/ MOV         R1, R8 
    /*00003234*/ MOV         R0, R9 
    /*00003238*/ BL          0x0000000000003238          @ AoSetDevAttr [R_ARM_CALL: .text+0x40d0] 
    /*0000323c*/ MOV         R4, R0 
    AoIoctl_x3240:
    /*00003240*/ LDR         R0, [PC, #472]              @ 0x0000000000003420 
    /*00003244*/ BL          0x0000000000003244          @ osal_atomic_dec_return [R_ARM_CALL: +0x0] 
    /*00003248*/ MOV         R0, R4 
    AoIoctl_x324c:
    /*0000324c*/ ADD         SP, SP, #12 
    /*00003250*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AoIoctl_x3254:
    /*00003254*/ MOV         R0, R9 
    /*00003258*/ BL          0x0000000000003258          @ AoEnableDev [R_ARM_CALL: .text+0x4384] 
    /*0000325c*/ MOV         R4, R0 
    /*00003260*/ B           AoIoctl_x3240
    AoIoctl_x3264:
    /*00003264*/ MOV         R1, R5 
    /*00003268*/ MOV         R0, R9 
    /*0000326c*/ BL          0x000000000000326C          @ AODisableChn [R_ARM_CALL: .text+0xc48] 
    /*00003270*/ MOV         R4, R0 
    /*00003274*/ B           AoIoctl_x3240
    AoIoctl_x3278:
    /*00003278*/ MOV         R1, R8 
    /*0000327c*/ MOV         R0, R9 
    /*00003280*/ BL          0x0000000000003280          @ AoGetDevAttr [R_ARM_CALL: .text+0x4240] 
    /*00003284*/ MOV         R4, R0 
    /*00003288*/ B           AoIoctl_x3240
    AoIoctl_x328c:
    /*0000328c*/ LDR         R3, [R8] 
    /*00003290*/ STR         R3, [R7] 
    /*00003294*/ B           AoIoctl_x3240
    AoIoctl_x3298:
    /*00003298*/ MOV         R2, R8 
    /*0000329c*/ MOV         R1, R5 
    /*000032a0*/ MOV         R0, R9 
    /*000032a4*/ BL          0x00000000000032A4          @ AoSetSaveFile [R_ARM_CALL: .text+0x296c] 
    /*000032a8*/ MOV         R4, R0 
    /*000032ac*/ B           AoIoctl_x3240
    AoIoctl_x32b0:
    /*000032b0*/ MOV         R2, R8 
    /*000032b4*/ MOV         R1, R5 
    /*000032b8*/ MOV         R0, R9 
    /*000032bc*/ BL          0x00000000000032BC          @ AOGetChnState [R_ARM_CALL: .text+0x1640] 
    /*000032c0*/ MOV         R4, R0 
    /*000032c4*/ B           AoIoctl_x3240
    AoIoctl_x32c8:
    /*000032c8*/ MOV         R1, R8 
    /*000032cc*/ MOV         R0, R9 
    /*000032d0*/ BL          0x00000000000032D0          @ AoSetMute [R_ARM_CALL: .text+0x2834] 
    /*000032d4*/ MOV         R4, R0 
    /*000032d8*/ B           AoIoctl_x3240
    AoIoctl_x32dc:
    /*000032dc*/ MOV         R1, R5 
    /*000032e0*/ MOV         R0, R9 
    /*000032e4*/ BL          0x00000000000032E4          @ AOClearChnBuf [R_ARM_CALL: .text+0x13ac] 
    /*000032e8*/ MOV         R4, R0 
    /*000032ec*/ B           AoIoctl_x3240
    AoIoctl_x32f0:
    /*000032f0*/ CMP         R8, #0 
    /*000032f4*/ BEQ         AoIoctl_x3414
    /*000032f8*/ LDR         R1, [R8] 
    /*000032fc*/ MOV         R0, R9 
    /*00003300*/ BL          0x0000000000003300          @ AoSetVolume [R_ARM_CALL: .text+0x2364] 
    /*00003304*/ MOV         R4, R0 
    /*00003308*/ B           AoIoctl_x3240
    AoIoctl_x330c:
    /*0000330c*/ MOV         R1, R8 
    /*00003310*/ MOV         R0, R9 
    /*00003314*/ BL          0x0000000000003314          @ AoGetClkDir [R_ARM_CALL: .text+0x26f4] 
    /*00003318*/ MOV         R4, R0 
    /*0000331c*/ B           AoIoctl_x3240
    AoIoctl_x3320:
    /*00003320*/ MOV         R2, R8 
    /*00003324*/ MOV         R1, R5 
    /*00003328*/ MOV         R0, R9 
    /*0000332c*/ BL          0x000000000000332C          @ AOPutFrame [R_ARM_CALL: .text+0x1928] 
    /*00003330*/ MOV         R4, R0 
    /*00003334*/ B           AoIoctl_x3240
    AoIoctl_x3338:
    /*00003338*/ MOVW        R3, #1211                   @ 0x4bb 
    /*0000333c*/ MOVW        R2, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb94] 
    /*00003340*/ STR         R3, [SP] 
    /*00003344*/ MOVT        R2, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb94] 
    /*00003348*/ LDR         R3, [PC, #212]              @ 0x0000000000003424 
    /*0000334c*/ MOV         R1, #22 
    /*00003350*/ MOV         R0, #4 
    /*00003354*/ BL          0x0000000000003354          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003358*/ MOVW        R0, #32784                  @ 0x8010 
    /*0000335c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00003360*/ B           AoIoctl_x324c
    AoIoctl_x3364:
    /*00003364*/ MOV         R2, R8 
    /*00003368*/ MOV         R1, R5 
    /*0000336c*/ MOV         R0, R9 
    /*00003370*/ BL          0x0000000000003370          @ AoSetChnResmpInfo [R_ARM_CALL: .text+0x1f2c] 
    /*00003374*/ MOV         R4, R0 
    /*00003378*/ B           AoIoctl_x3240
    AoIoctl_x337c:
    /*0000337c*/ MOV         R1, R5 
    /*00003380*/ MOV         R0, R9 
    /*00003384*/ BL          0x0000000000003384          @ AOEnableChn [R_ARM_CALL: .text+0x908] 
    /*00003388*/ MOV         R4, R0 
    /*0000338c*/ B           AoIoctl_x3240
    AoIoctl_x3390:
    /*00003390*/ MOV         R2, R8 
    /*00003394*/ MOV         R1, R5 
    /*00003398*/ MOV         R0, R9 
    /*0000339c*/ BL          0x000000000000339C          @ AOSetChnVqeInfo [R_ARM_CALL: .text+0x2020] 
    /*000033a0*/ MOV         R4, R0 
    /*000033a4*/ B           AoIoctl_x3240
    AoIoctl_x33a8:
    /*000033a8*/ MOV         R1, R5 
    /*000033ac*/ MOV         R0, R9 
    /*000033b0*/ BL          0x00000000000033B0          @ AOResumeChn [R_ARM_CALL: .text+0x1144] 
    /*000033b4*/ MOV         R4, R0 
    /*000033b8*/ B           AoIoctl_x3240
    AoIoctl_x33bc:
    /*000033bc*/ MOV         R1, R8 
    /*000033c0*/ MOV         R0, R9 
    /*000033c4*/ BL          0x00000000000033C4          @ AoGetVolume [R_ARM_CALL: .text+0x248c] 
    /*000033c8*/ MOV         R4, R0 
    /*000033cc*/ B           AoIoctl_x3240
    AoIoctl_x33d0:
    /*000033d0*/ MOV         R1, R8 
    /*000033d4*/ MOV         R0, R9 
    /*000033d8*/ BL          0x00000000000033D8          @ AoGetTrackMode [R_ARM_CALL: .text+0x2230] 
    /*000033dc*/ MOV         R4, R0 
    /*000033e0*/ B           AoIoctl_x3240
    AoIoctl_x33e4:
    /*000033e4*/ MOV         R1, R8 
    /*000033e8*/ MOV         R0, R9 
    /*000033ec*/ BL          0x00000000000033EC          @ AoGetMute [R_ARM_CALL: .text+0x2e68] 
    /*000033f0*/ MOV         R4, R0 
    /*000033f4*/ B           AoIoctl_x3240
    AoIoctl_x33f8:
    /*000033f8*/ CMP         R8, #0 
    /*000033fc*/ BEQ         AoIoctl_x3414
    /*00003400*/ LDR         R1, [R8] 
    /*00003404*/ MOV         R0, R9 
    /*00003408*/ BL          0x0000000000003408          @ AoSetClkDir [R_ARM_CALL: .text+0x25c0] 
    /*0000340c*/ MOV         R4, R0 
    /*00003410*/ B           AoIoctl_x3240
    AoIoctl_x3414:
    /*00003414*/ MOVW        R0, #32774                  @ 0x8006 
    /*00003418*/ MOVT        R0, #40982                  @ 0xa016 
    /*0000341c*/ B           AoIoctl_x324c
    /*00003420*/ ANDEQ       R0, R0, R4 
    /*00003424*/ ANDEQ       R0, R0, R4, ASR #2 
FUNC_END AoIoctl


FUNC_BEGIN AO_ModInit
    /*00003428*/ PUSH        {R4, R5, R6, R7, LR} 
    /*0000342c*/ SUB         SP, SP, #12 
    /*00003430*/ BL          0x0000000000003430          @ AO_DRV_CheckAioExportFunc [R_ARM_CALL: .text+0x786c] 
    /*00003434*/ SUBS        R4, R0, #0 
    /*00003438*/ BEQ         AO_ModInit_x3448
    AO_ModInit_x343c:
    /*0000343c*/ MOV         R0, R4 
    /*00003440*/ ADD         SP, SP, #12 
    /*00003444*/ POP         {R4, R5, R6, R7, PC} 
    AO_ModInit_x3448:
    /*00003448*/ BL          0x0000000000003448          @ AO_DRV_OpenClock [R_ARM_CALL: .text+0x78bc] 
    /*0000344c*/ MOV         R1, R4 
    /*00003450*/ MOVW        R0, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbfc] 
    /*00003454*/ MOVT        R0, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbfc] 
    /*00003458*/ BL          0x0000000000003458          @ osal_create_proc_entry [R_ARM_CALL: +0x0] 
    /*0000345c*/ SUBS        R2, R0, #0 
    /*00003460*/ BEQ         AO_ModInit_x3540
    /*00003464*/ MOVW        R3, #0                      @ AOProcShow [R_ARM_MOVW_ABS_NC: .text+0x4c8c] 
    /*00003468*/ MOVW        R0, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbfc] 
    /*0000346c*/ MOVT        R3, #0                      @ AOProcShow [R_ARM_MOVT_ABS: .text+0x4c8c] 
    /*00003470*/ MOVT        R0, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbfc] 
    /*00003474*/ STR         R3, [R2, #60]               @ 0x3c 
    /*00003478*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000347c*/ BL          0x000000000000347C          @ osal_createdev [R_ARM_CALL: +0x0] 
    /*00003480*/ LDR         R6, [PC, #224]              @ 0x0000000000003568 
    /*00003484*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003488*/ MOV         R7, #6 
    /*0000348c*/ ADD         R2, R6, #32 
    /*00003490*/ STR         R6, [R0, #56]               @ 0x38 
    /*00003494*/ STR         R2, [R0, #60]               @ 0x3c 
    /*00003498*/ STR         R7, [R0, #52]               @ 0x34 
    /*0000349c*/ STR         R0, [R5, #8] 
    /*000034a0*/ BL          0x00000000000034A0          @ osal_registerdevice [R_ARM_CALL: +0x0] 
    /*000034a4*/ SUBS        R4, R0, #0 
    /*000034a8*/ BNE         AO_ModInit_x34e0
    /*000034ac*/ ADD         R0, R6, #112                @ 0x70 
    /*000034b0*/ BL          0x00000000000034B0          @ CMPI_RegisterModule [R_ARM_CALL: +0x0] 
    /*000034b4*/ CMP         R0, #0 
    /*000034b8*/ BNE         AO_ModInit_x3518
    /*000034bc*/ BL          0x00000000000034BC          @ AO_DRV_CloseClock [R_ARM_CALL: .text+0x78c4] 
    /*000034c0*/ ADD         R0, R5, #4 
    /*000034c4*/ BL          0x00000000000034C4          @ osal_atomic_init [R_ARM_CALL: +0x0] 
    /*000034c8*/ ADD         R0, R5, #4 
    /*000034cc*/ MOV         R1, R4 
    /*000034d0*/ BL          0x00000000000034D0          @ osal_atomic_set [R_ARM_CALL: +0x0] 
    /*000034d4*/ MOV         R0, R4 
    /*000034d8*/ ADD         SP, SP, #12 
    /*000034dc*/ POP         {R4, R5, R6, R7, PC} 
    AO_ModInit_x34e0:
    /*000034e0*/ MOV         R0, R7 
    /*000034e4*/ MOVW        R3, #1669                   @ 0x685 
    /*000034e8*/ MOV         R1, #22 
    /*000034ec*/ STR         R3, [SP] 
    /*000034f0*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc34] 
    /*000034f4*/ LDR         R3, [PC, #112]              @ 0x000000000000356c 
    /*000034f8*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc34] 
    /*000034fc*/ MVN         R4, #0 
    /*00003500*/ BL          0x0000000000003500          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003504*/ MOVW        R0, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbfc] 
    /*00003508*/ MOV         R1, #0 
    /*0000350c*/ MOVT        R0, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbfc] 
    /*00003510*/ BL          0x0000000000003510          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*00003514*/ B           AO_ModInit_x343c
    AO_ModInit_x3518:
    /*00003518*/ MOV         R1, R4 
    /*0000351c*/ MOVW        R0, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbfc] 
    /*00003520*/ MOVT        R0, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbfc] 
    /*00003524*/ MVN         R4, #0 
    /*00003528*/ BL          0x0000000000003528          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*0000352c*/ LDR         R0, [R5, #8] 
    /*00003530*/ BL          0x0000000000003530          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*00003534*/ LDR         R0, [R5, #8] 
    /*00003538*/ BL          0x0000000000003538          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*0000353c*/ B           AO_ModInit_x343c
    AO_ModInit_x3540:
    /*00003540*/ MOVW        R3, #1656                   @ 0x678 
    /*00003544*/ MOVW        R2, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc00] 
    /*00003548*/ STR         R3, [SP] 
    /*0000354c*/ MOVT        R2, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc00] 
    /*00003550*/ LDR         R3, [PC, #20]               @ 0x000000000000356c 
    /*00003554*/ MOV         R1, #22 
    /*00003558*/ MOV         R0, #6 
    /*0000355c*/ MVN         R4, #0 
    /*00003560*/ BL          0x0000000000003560          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003564*/ B           AO_ModInit_x343c
    /*00003568*/ ANDEQ       R0, R0, R4 
    /*0000356c*/ ANDEQ       R0, R0, R0, ASR R1 
FUNC_END AO_ModInit


FUNC_BEGIN AO_ModExit
    /*00003570*/ PUSH        {R4, LR} 
    /*00003574*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003578*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000357c*/ ADD         R0, R4, #4 
    /*00003580*/ BL          0x0000000000003580          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*00003584*/ MOV         R0, #22 
    /*00003588*/ BL          0x0000000000003588          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    /*0000358c*/ LDR         R0, [R4, #8] 
    /*00003590*/ BL          0x0000000000003590          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*00003594*/ LDR         R0, [R4, #8] 
    /*00003598*/ BL          0x0000000000003598          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*0000359c*/ MOVW        R0, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbfc] 
    /*000035a0*/ MOV         R1, #0 
    /*000035a4*/ MOVT        R0, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbfc] 
    /*000035a8*/ POP         {R4, LR} 
    AO_ModExit_x35ac:
    /*000035ac*/ B           AO_ModExit_x35ac            @ osal_remove_proc_entry [R_ARM_JUMP24: +0x0] 
FUNC_END AO_ModExit


FUNC_BEGIN AODmaCallback
    /*000035b0*/ MOV         R3, #136                    @ 0x88 
    /*000035b4*/ MUL         R3, R3, R0 
    /*000035b8*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000035bc*/ SUB         SP, SP, #52                 @ 0x34 
    /*000035c0*/ MOVW        R7, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*000035c4*/ MOVT        R7, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*000035c8*/ MOV         R9, R0 
    /*000035cc*/ ADD         R1, SP, #40                 @ 0x28 
    /*000035d0*/ MOV         R2, R3 
    /*000035d4*/ STR         R3, [SP, #32] 
    /*000035d8*/ ADD         R3, R3, #128                @ 0x80 
    /*000035dc*/ ADD         R4, R7, R2 
    /*000035e0*/ ADD         R3, R7, R3 
    /*000035e4*/ STR         R3, [SP, #28] 
    /*000035e8*/ MOV         R0, R3 
    /*000035ec*/ BL          0x00000000000035EC          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000035f0*/ LDR         R3, [R4, #12] 
    /*000035f4*/ CMP         R3, #0 
    /*000035f8*/ BEQ         AODmaCallback_x37a0
    /*000035fc*/ MOVW        R0, #0                      @ g_stAoBufLock [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003600*/ ADD         R1, SP, #44                 @ 0x2c 
    /*00003604*/ MOVT        R0, #0                      @ g_stAoBufLock [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003608*/ BL          0x0000000000003608          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000360c*/ LDR         R3, [R4, #12] 
    /*00003610*/ CMP         R3, #1 
    /*00003614*/ BNE         AODmaCallback_x3790
    /*00003618*/ ADD         SL, R9, R9, LSL #1 
    /*0000361c*/ MOVW        R2, #2892                   @ 0xb4c 
    /*00003620*/ ADD         R1, SL, #2 
    /*00003624*/ MLA         R2, R2, R1, R7 
    /*00003628*/ LDR         IP, [R2, #1496]             @ 0x5d8 
    /*0000362c*/ LDR         R3, [R2, #1500]             @ 0x5dc 
    /*00003630*/ CMP         IP, R3 
    /*00003634*/ BEQ         AODmaCallback_x38c0
    AODmaCallback_x3638:
    /*00003638*/ MOVW        R2, #2892                   @ 0xb4c 
    /*0000363c*/ ADD         R0, R3, #1 
    /*00003640*/ MLA         R2, R2, R1, R7 
    /*00003644*/ ADD         R3, R3, #2 
    /*00003648*/ MOVW        LR, #723                    @ 0x2d3 
    /*0000364c*/ MLA         R3, LR, R1, R3 
    /*00003650*/ LDR         LR, [PC, #668]              @ 0x00000000000038f4 
    /*00003654*/ LDR         R4, [R2, #1480]             @ 0x5c8 
    /*00003658*/ STR         R0, [R2, #1500]             @ 0x5dc 
    /*0000365c*/ CMP         R0, R4 
    /*00003660*/ ADD         R3, LR, R3, LSL #2 
    /*00003664*/ STR         R3, [SP, #24] 
    /*00003668*/ MOVEQ       R0, #0 
    /*0000366c*/ STREQ       R0, [R2, #1500]             @ 0x5dc 
    /*00003670*/ CMP         IP, R0 
    /*00003674*/ MOVWEQ      R2, #2892                   @ 0xb4c 
    /*00003678*/ MOVEQ       R3, #0 
    /*0000367c*/ MLAEQ       R1, R2, R1, R7 
    /*00003680*/ STREQ       R3, [R1, #1508]             @ 0x5e4 
    AODmaCallback_x3684:
    /*00003684*/ MOV         R8, #136                    @ 0x88 
    /*00003688*/ MLA         R8, R8, R9, R7 
    /*0000368c*/ LDR         R3, [R8, #44]               @ 0x2c 
    /*00003690*/ CMP         R3, #0 
    /*00003694*/ BEQ         AODmaCallback_x3788
    /*00003698*/ MOVW        R3, #723                    @ 0x2d3 
    /*0000369c*/ STR         R3, [SP, #20] 
    /*000036a0*/ MUL         R6, R3, SL 
    /*000036a4*/ LDR         R3, [PC, #584]              @ 0x00000000000038f4 
    /*000036a8*/ MOVW        R4, #2892                   @ 0xb4c 
    /*000036ac*/ MOV         R5, #0 
    /*000036b0*/ STR         R7, [SP, #36]               @ 0x24 
    /*000036b4*/ MLA         R4, R4, SL, R3 
    /*000036b8*/ B           AODmaCallback_x36ec
    AODmaCallback_x36bc:
    /*000036bc*/ CMP         R3, #2 
    /*000036c0*/ BEQ         AODmaCallback_x37b4
    /*000036c4*/ CMP         R3, #3 
    /*000036c8*/ BEQ         AODmaCallback_x3898
    AODmaCallback_x36cc:
    /*000036cc*/ LDR         R3, [R8, #44]               @ 0x2c 
    /*000036d0*/ ADD         R5, R5, #1 
    /*000036d4*/ LDR         R2, [SP, #20] 
    /*000036d8*/ ADD         R4, R4, #2880               @ 0xb40 
    /*000036dc*/ CMP         R5, R3 
    /*000036e0*/ ADD         R4, R4, #12 
    /*000036e4*/ ADD         R6, R6, R2 
    /*000036e8*/ BCS         AODmaCallback_x3788
    AODmaCallback_x36ec:
    /*000036ec*/ LDR         R3, [R4, #1240]             @ 0x4d8 
    /*000036f0*/ ADD         R2, R5, SL 
    /*000036f4*/ CMP         R3, #1 
    /*000036f8*/ BNE         AODmaCallback_x36bc
    /*000036fc*/ LDR         R3, [R4, #1228]             @ 0x4cc 
    /*00003700*/ LDR         R1, [R4, #1224]             @ 0x4c8 
    /*00003704*/ CMP         R1, R3 
    /*00003708*/ BEQ         AODmaCallback_x3828
    AODmaCallback_x370c:
    /*0000370c*/ LDR         R0, [SP, #24] 
    /*00003710*/ ADD         R1, R6, #2 
    /*00003714*/ ADD         R1, R1, R3 
    /*00003718*/ CMP         R0, #0 
    /*0000371c*/ LDR         R0, [PC, #464]              @ 0x00000000000038f4 
    /*00003720*/ ADD         R1, R0, R1, LSL #2 
    /*00003724*/ BEQ         AODmaCallback_x3734
    /*00003728*/ LDR         LR, [R4, #1212]             @ 0x4bc 
    /*0000372c*/ CMP         LR, #0 
    /*00003730*/ BNE         AODmaCallback_x37d4
    AODmaCallback_x3734:
    /*00003734*/ MOV         R2, R5 
    /*00003738*/ MOV         R0, R9 
    /*0000373c*/ BL          0x000000000000373C          @ AO_DRV_PutChnData [R_ARM_CALL: .text+0x6d3c] 
    /*00003740*/ LDR         R3, [R4, #1228]             @ 0x4cc 
    /*00003744*/ LDR         R2, [R4, #1208]             @ 0x4b8 
    /*00003748*/ ADD         R3, R3, #1 
    /*0000374c*/ STR         R3, [R4, #1228]             @ 0x4cc 
    /*00003750*/ CMP         R3, R2 
    /*00003754*/ BEQ         AODmaCallback_x38b4
    AODmaCallback_x3758:
    /*00003758*/ LDR         R2, [R4, #1224]             @ 0x4c8 
    /*0000375c*/ CMP         R2, R3 
    /*00003760*/ LDR         R2, [SP, #36]               @ 0x24 
    /*00003764*/ MOVEQ       R3, #0 
    /*00003768*/ STREQ       R3, [R4, #1236]             @ 0x4d4 
    /*0000376c*/ LDR         R3, [SP, #32] 
    /*00003770*/ ADD         R0, R3, #120                @ 0x78 
    /*00003774*/ MOV         R3, #0 
    /*00003778*/ ADD         R0, R2, R0 
    /*0000377c*/ STR         R3, [R4, #1252]             @ 0x4e4 
    /*00003780*/ BL          0x0000000000003780          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00003784*/ B           AODmaCallback_x36cc
    AODmaCallback_x3788:
    /*00003788*/ MOV         R0, R9 
    /*0000378c*/ BL          0x000000000000378C          @ AO_DRV_FlushData [R_ARM_CALL: .text+0x6d18] 
    AODmaCallback_x3790:
    /*00003790*/ MOVW        R0, #0                      @ g_stAoBufLock [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003794*/ ADD         R1, SP, #44                 @ 0x2c 
    /*00003798*/ MOVT        R0, #0                      @ g_stAoBufLock [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000379c*/ BL          0x000000000000379C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    AODmaCallback_x37a0:
    /*000037a0*/ ADD         R1, SP, #40                 @ 0x28 
    /*000037a4*/ LDR         R0, [SP, #28] 
    /*000037a8*/ BL          0x00000000000037A8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000037ac*/ ADD         SP, SP, #52                 @ 0x34 
    /*000037b0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AODmaCallback_x37b4:
    /*000037b4*/ LDR         R3, [SP, #24] 
    /*000037b8*/ CMP         R3, #0 
    /*000037bc*/ BEQ         AODmaCallback_x38d4
    /*000037c0*/ MOV         R2, R5 
    /*000037c4*/ MOV         R1, R3 
    /*000037c8*/ MOV         R0, R9 
    /*000037cc*/ BL          0x00000000000037CC          @ AO_DRV_PutChnData [R_ARM_CALL: .text+0x6d3c] 
    /*000037d0*/ B           AODmaCallback_x36cc
    AODmaCallback_x37d4:
    /*000037d4*/ LDR         R0, [SP, #20] 
    /*000037d8*/ MOVW        R7, #32767                  @ 0x7fff 
    /*000037dc*/ MLA         R2, R0, R2, R3 
    /*000037e0*/ LDR         R3, [SP, #24] 
    /*000037e4*/ MOV         R0, #0 
    /*000037e8*/ LDR         IP, [R3] 
    /*000037ec*/ LDR         R3, [SP, #36]               @ 0x24 
    /*000037f0*/ SUB         IP, IP, #2 
    /*000037f4*/ ADD         R2, R3, R2, LSL #2 
    /*000037f8*/ LDR         R2, [R2, #280]              @ 0x118 
    AODmaCallback_x37fc:
    /*000037fc*/ LDRSH       FP, [IP, #2]! 
    /*00003800*/ ADD         R0, R0, #1 
    /*00003804*/ LDRSH       R3, [R2] 
    /*00003808*/ ADD         R3, R3, FP 
    /*0000380c*/ ASR         FP, R3, #31 
    /*00003810*/ CMP         FP, R3, ASR #15 
    /*00003814*/ EORNE       R3, FP, R7 
    /*00003818*/ CMP         LR, R0 
    /*0000381c*/ STRH        R3, [R2], #2 
    /*00003820*/ BNE         AODmaCallback_x37fc
    /*00003824*/ B           AODmaCallback_x3734
    AODmaCallback_x3828:
    /*00003828*/ LDR         R1, [R4, #1236]             @ 0x4d4 
    /*0000382c*/ CMP         R1, #0 
    /*00003830*/ BNE         AODmaCallback_x370c
    /*00003834*/ LDR         R1, [SP, #24] 
    /*00003838*/ LDR         R3, [R4, #1252]             @ 0x4e4 
    /*0000383c*/ CMP         R1, #0 
    /*00003840*/ ADD         R3, R3, #1 
    /*00003844*/ STR         R3, [R4, #1252]             @ 0x4e4 
    /*00003848*/ BEQ         AODmaCallback_x38e4
    /*0000384c*/ MOV         R2, R5 
    /*00003850*/ MOV         R0, R9 
    /*00003854*/ BL          0x0000000000003854          @ AO_DRV_PutChnData [R_ARM_CALL: .text+0x6d3c] 
    AODmaCallback_x3858:
    /*00003858*/ LDR         R2, [R4, #1252]             @ 0x4e4 
    /*0000385c*/ MOV         R1, #237                    @ 0xed 
    /*00003860*/ STR         R5, [SP, #8] 
    /*00003864*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x15c] 
    /*00003868*/ STR         R1, [SP] 
    /*0000386c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x15c] 
    /*00003870*/ STR         R2, [SP, #4] 
    /*00003874*/ MOV         R1, #22 
    /*00003878*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc64] 
    /*0000387c*/ MOV         R0, #4 
    /*00003880*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc64] 
    /*00003884*/ BL          0x0000000000003884          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003888*/ LDR         R3, [R4, #1248]             @ 0x4e0 
    /*0000388c*/ ADD         R3, R3, #1 
    /*00003890*/ STR         R3, [R4, #1248]             @ 0x4e0 
    /*00003894*/ B           AODmaCallback_x36cc
    AODmaCallback_x3898:
    /*00003898*/ MOV         R1, R5 
    /*0000389c*/ MOV         R0, R9 
    /*000038a0*/ BL          0x00000000000038A0          @ AO_DRV_PutZeroData [R_ARM_CALL: .text+0x6000] 
    /*000038a4*/ MOV         R3, #0 
    /*000038a8*/ STR         R3, [R4, #1228]             @ 0x4cc 
    /*000038ac*/ STR         R3, [R4, #1224]             @ 0x4c8 
    /*000038b0*/ B           AODmaCallback_x36cc
    AODmaCallback_x38b4:
    /*000038b4*/ MOV         R3, #0 
    /*000038b8*/ STR         R3, [R4, #1228]             @ 0x4cc 
    /*000038bc*/ B           AODmaCallback_x3758
    AODmaCallback_x38c0:
    /*000038c0*/ LDR         R2, [R2, #1508]             @ 0x5e4 
    /*000038c4*/ CMP         R2, #0 
    /*000038c8*/ STREQ       R2, [SP, #24] 
    /*000038cc*/ BEQ         AODmaCallback_x3684
    /*000038d0*/ B           AODmaCallback_x3638
    AODmaCallback_x38d4:
    /*000038d4*/ MOV         R1, R5 
    /*000038d8*/ MOV         R0, R9 
    /*000038dc*/ BL          0x00000000000038DC          @ AO_DRV_PutZeroData [R_ARM_CALL: .text+0x6000] 
    /*000038e0*/ B           AODmaCallback_x36cc
    AODmaCallback_x38e4:
    /*000038e4*/ MOV         R1, R5 
    /*000038e8*/ MOV         R0, R9 
    /*000038ec*/ BL          0x00000000000038EC          @ AO_DRV_PutLastData [R_ARM_CALL: .text+0x64e8] 
    /*000038f0*/ B           AODmaCallback_x3858
    /*000038f4*/ ANDEQ       R0, R0, R0, LSR #2 
FUNC_END AODmaCallback


FUNC_BEGIN AOBufInit
    /*000038f8*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000038fc*/ SUB         SP, SP, #76                 @ 0x4c 
    /*00003900*/ LDR         R2, [R1, #4] 
    /*00003904*/ MOV         R4, R1 
    /*00003908*/ ADD         R1, SP, #36                 @ 0x24 
    /*0000390c*/ MOV         R3, #0 
    /*00003910*/ CMP         R2, #2 
    /*00003914*/ STR         R3, [SP, #60]               @ 0x3c 
    /*00003918*/ STR         R3, [SP, #56]               @ 0x38 
    /*0000391c*/ MOVNE       R5, #2 
    /*00003920*/ MOVEQ       R5, #4 
    /*00003924*/ STR         R3, [SP, #64]               @ 0x40 
    /*00003928*/ STR         R3, [SP, #68]               @ 0x44 
    /*0000392c*/ STR         R3, [SP, #40]               @ 0x28 
    /*00003930*/ LDR         R8, [R4, #28] 
    /*00003934*/ STR         R0, [SP, #16] 
    /*00003938*/ BL          0x0000000000003938          @ AO_DRV_DmaBufInfoInit [R_ARM_CALL: .text+0x7344] 
    /*0000393c*/ SUBS        R3, R0, #0 
    /*00003940*/ STR         R3, [SP, #12] 
    /*00003944*/ BNE         AOBufInit_x3bd8
    /*00003948*/ LDR         R1, [R4, #24] 
    /*0000394c*/ MOV         R0, #2 
    /*00003950*/ LDR         R3, [R4, #20] 
    /*00003954*/ MOV         R9, #22 
    /*00003958*/ LDR         R2, [R4, #12] 
    /*0000395c*/ STR         R9, [SP, #44]               @ 0x2c 
    /*00003960*/ MUL         R3, R3, R1 
    /*00003964*/ CMP         R2, #1 
    /*00003968*/ ADDEQ       R7, R8, #2 
    /*0000396c*/ MUL         R3, R5, R3 
    /*00003970*/ STR         R3, [SP, #8] 
    /*00003974*/ LDRNE       R3, [SP, #8] 
    /*00003978*/ LDREQ       R3, [SP, #36]               @ 0x24 
    /*0000397c*/ LDREQ       R2, [SP, #8] 
    /*00003980*/ MLANE       R7, R8, R3, R3 
    /*00003984*/ LDRNE       R3, [SP, #36]               @ 0x24 
    /*00003988*/ MLAEQ       R7, R2, R7, R3 
    /*0000398c*/ ADDNE       R7, R7, R3 
    /*00003990*/ LDR         R3, [SP, #16] 
    /*00003994*/ STR         R3, [SP, #48]               @ 0x30 
    /*00003998*/ MOV         R3, #0 
    /*0000399c*/ STR         R3, [SP, #52]               @ 0x34 
    /*000039a0*/ BL          0x00000000000039A0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000039a4*/ ADD         R1, SP, #40                 @ 0x28 
    /*000039a8*/ LDR         R3, [R0, #60]               @ 0x3c 
    /*000039ac*/ ADD         R0, SP, #44                 @ 0x2c 
    /*000039b0*/ BLX         R3 
    /*000039b4*/ SUBS        SL, R0, #0 
    /*000039b8*/ BNE         AOBufInit_x3c20
    /*000039bc*/ MOVW        IP, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*000039c0*/ LDR         R3, [SP, #16] 
    /*000039c4*/ MOV         R1, #16 
    /*000039c8*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd24] 
    /*000039cc*/ ADD         R0, SP, #56                 @ 0x38 
    /*000039d0*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd24] 
    /*000039d4*/ MOV         FP, IP 
    /*000039d8*/ BL          0x00000000000039D8          @ osal_snprintf [R_ARM_CALL: +0x0] 
    /*000039dc*/ LDR         R3, [SP, #16] 
    /*000039e0*/ MOV         R6, #136                    @ 0x88 
    /*000039e4*/ MOV         IP, FP 
    /*000039e8*/ MOVT        IP, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*000039ec*/ LDR         R0, [SP, #40]               @ 0x28 
    /*000039f0*/ ADD         R1, SP, #56                 @ 0x38 
    /*000039f4*/ MUL         R6, R6, R3 
    /*000039f8*/ STR         R7, [SP] 
    /*000039fc*/ STR         IP, [SP, #20] 
    /*00003a00*/ ADD         R3, R6, #64                 @ 0x40 
    /*00003a04*/ ADD         R2, R6, #56                 @ 0x38 
    /*00003a08*/ ADD         R3, IP, R3 
    /*00003a0c*/ ADD         R2, IP, R2 
    /*00003a10*/ BL          0x0000000000003A10          @ CMPI_MmzMallocNocache [R_ARM_CALL: +0x0] 
    /*00003a14*/ SUBS        R3, R0, #0 
    /*00003a18*/ STR         R3, [SP, #12] 
    /*00003a1c*/ BNE         AOBufInit_x3c4c
    /*00003a20*/ LDR         R3, [SP, #20] 
    /*00003a24*/ MOV         R2, R7 
    /*00003a28*/ LDR         R1, [SP, #12] 
    /*00003a2c*/ ADD         R6, R3, R6 
    /*00003a30*/ LDR         R0, [R6, #64]               @ 0x40 
    /*00003a34*/ BL          0x0000000000003A34          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00003a38*/ LDR         R1, [R6, #64]               @ 0x40 
    /*00003a3c*/ LDRD        R2, [R6, #56]               @ 0x38 
    /*00003a40*/ LDR         R0, [SP, #16] 
    /*00003a44*/ STR         R1, [SP] 
    /*00003a48*/ BL          0x0000000000003A48          @ AO_DRV_SetDmaBufAddr [R_ARM_CALL: .text+0x749c] 
    /*00003a4c*/ LDR         R6, [R6, #64]               @ 0x40 
    /*00003a50*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00003a54*/ CMP         R8, #0 
    /*00003a58*/ ADD         R6, R6, R3 
    /*00003a5c*/ BEQ         AOBufInit_x3c04
    /*00003a60*/ LDR         R2, [SP, #16] 
    /*00003a64*/ LDR         R1, [SP, #20] 
    /*00003a68*/ LDR         SL, [SP, #12] 
    /*00003a6c*/ LSL         R3, R2, #1 
    /*00003a70*/ ADD         R1, R1, #272                @ 0x110 
    /*00003a74*/ ADD         R9, R3, R2 
    /*00003a78*/ STR         R3, [SP, #28] 
    /*00003a7c*/ MOVW        R3, #2892                   @ 0xb4c 
    /*00003a80*/ MOV         R7, SL 
    /*00003a84*/ MLA         R9, R3, R9, R1 
    /*00003a88*/ MOV         FP, SL 
    /*00003a8c*/ STR         R1, [SP, #24] 
    /*00003a90*/ ADD         R9, R9, #8 
    AOBufInit_x3a94:
    /*00003a94*/ MOV         R2, #1232                   @ 0x4d0 
    /*00003a98*/ MOV         R1, #0 
    /*00003a9c*/ MOV         R0, R9 
    /*00003aa0*/ BL          0x0000000000003AA0          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00003aa4*/ LDR         IP, [R4, #20] 
    /*00003aa8*/ CMP         IP, #0 
    /*00003aac*/ STR         IP, [R9, #1200]             @ 0x4b0 
    /*00003ab0*/ SUBNE       R0, R9, #4 
    /*00003ab4*/ LDR         R2, [R4, #24] 
    /*00003ab8*/ MUL         R2, R5, R2 
    /*00003abc*/ STR         R2, [R9, #1204]             @ 0x4b4 
    /*00003ac0*/ LDR         R2, [R4, #20] 
    /*00003ac4*/ STR         SL, [R9, #1216]             @ 0x4c0 
    /*00003ac8*/ STR         SL, [R9, #1220]             @ 0x4c4 
    /*00003acc*/ LSR         R1, R2, #1 
    /*00003ad0*/ STR         SL, [R9, #1228]             @ 0x4cc 
    /*00003ad4*/ LSR         R2, R2, #2 
    /*00003ad8*/ STR         R1, [R9, #1208]             @ 0x4b8 
    /*00003adc*/ STR         R2, [R9, #1212]             @ 0x4bc 
    /*00003ae0*/ MOVNE       R1, #0 
    /*00003ae4*/ BEQ         AOBufInit_x3b08
    AOBufInit_x3ae8:
    /*00003ae8*/ LDR         R3, [R4, #24] 
    /*00003aec*/ MUL         R3, R3, R5 
    /*00003af0*/ MLA         R3, R1, R3, R7 
    /*00003af4*/ ADD         R1, R1, #1 
    /*00003af8*/ CMP         R1, IP 
    /*00003afc*/ ADD         R3, R6, R3 
    /*00003b00*/ STR         R3, [R0, #4]! 
    /*00003b04*/ BNE         AOBufInit_x3ae8
    AOBufInit_x3b08:
    /*00003b08*/ ADD         FP, FP, #1 
    /*00003b0c*/ LDR         R3, [SP, #8] 
    /*00003b10*/ CMP         R8, FP 
    /*00003b14*/ ADD         R9, R9, #2880               @ 0xb40 
    /*00003b18*/ ADD         R9, R9, #12 
    /*00003b1c*/ ADD         R7, R7, R3 
    /*00003b20*/ BNE         AOBufInit_x3a94
    AOBufInit_x3b24:
    /*00003b24*/ LDR         R2, [SP, #16] 
    /*00003b28*/ MOVW        R9, #2892                   @ 0xb4c 
    /*00003b2c*/ LDR         R3, [SP, #28] 
    /*00003b30*/ LDR         R1, [SP, #8] 
    /*00003b34*/ ADD         R3, R3, R2 
    /*00003b38*/ MOV         R2, #1232                   @ 0x4d0 
    /*00003b3c*/ ADD         R3, R3, #2 
    /*00003b40*/ MUL         R8, R1, R8 
    /*00003b44*/ MOV         R1, #0 
    /*00003b48*/ MUL         R9, R9, R3 
    /*00003b4c*/ LDR         R3, [SP, #24] 
    /*00003b50*/ ADD         R7, R9, #8 
    /*00003b54*/ ADD         R7, R3, R7 
    /*00003b58*/ MOV         R0, R7 
    /*00003b5c*/ BL          0x0000000000003B5C          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00003b60*/ LDR         R3, [SP, #20] 
    /*00003b64*/ LDR         R1, [R4, #20] 
    /*00003b68*/ MOV         R2, #0 
    /*00003b6c*/ ADD         R9, R3, R9 
    /*00003b70*/ CMP         R1, R2 
    /*00003b74*/ STR         R1, [R9, #1480]             @ 0x5c8 
    /*00003b78*/ SUBNE       R7, R7, #4 
    /*00003b7c*/ LDR         R3, [R4, #24] 
    /*00003b80*/ MUL         R3, R5, R3 
    /*00003b84*/ STR         R3, [R9, #1484]             @ 0x5cc 
    /*00003b88*/ LDR         R3, [R4, #20] 
    /*00003b8c*/ STR         R2, [R9, #1496]             @ 0x5d8 
    /*00003b90*/ STR         R2, [R9, #1500]             @ 0x5dc 
    /*00003b94*/ LSR         R0, R3, #1 
    /*00003b98*/ STR         R2, [R9, #1508]             @ 0x5e4 
    /*00003b9c*/ LSR         R3, R3, #2 
    /*00003ba0*/ STR         R0, [R9, #1488]             @ 0x5d0 
    /*00003ba4*/ STR         R3, [R9, #1492]             @ 0x5d4 
    /*00003ba8*/ BEQ         AOBufInit_x3bcc
    AOBufInit_x3bac:
    /*00003bac*/ LDR         R3, [R4, #24] 
    /*00003bb0*/ MUL         R3, R3, R5 
    /*00003bb4*/ MLA         R3, R2, R3, R8 
    /*00003bb8*/ ADD         R2, R2, #1 
    /*00003bbc*/ CMP         R2, R1 
    /*00003bc0*/ ADD         R3, R6, R3 
    /*00003bc4*/ STR         R3, [R7, #4]! 
    /*00003bc8*/ BNE         AOBufInit_x3bac
    AOBufInit_x3bcc:
    /*00003bcc*/ LDR         R0, [SP, #12] 
    /*00003bd0*/ ADD         SP, SP, #76                 @ 0x4c 
    /*00003bd4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOBufInit_x3bd8:
    /*00003bd8*/ MOV         R3, #62                     @ 0x3e 
    /*00003bdc*/ MOV         R1, #22 
    /*00003be0*/ STR         R3, [SP] 
    /*00003be4*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcb8] 
    /*00003be8*/ LDR         R3, [PC, #148]              @ 0x0000000000003c84 
    /*00003bec*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcb8] 
    /*00003bf0*/ MOV         R0, #4 
    /*00003bf4*/ BL          0x0000000000003BF4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003bf8*/ LDR         R0, [SP, #12] 
    /*00003bfc*/ ADD         SP, SP, #76                 @ 0x4c 
    /*00003c00*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOBufInit_x3c04:
    /*00003c04*/ LDR         R3, [SP, #16] 
    /*00003c08*/ LSL         R3, R3, #1 
    /*00003c0c*/ STR         R3, [SP, #28] 
    /*00003c10*/ LDR         R3, [SP, #20] 
    /*00003c14*/ ADD         R3, R3, #272                @ 0x110 
    /*00003c18*/ STR         R3, [SP, #24] 
    /*00003c1c*/ B           AOBufInit_x3b24
    AOBufInit_x3c20:
    /*00003c20*/ MOV         R3, #82                     @ 0x52 
    /*00003c24*/ MOV         R1, R9 
    /*00003c28*/ STR         R3, [SP] 
    /*00003c2c*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcf4] 
    /*00003c30*/ LDR         R3, [PC, #76]               @ 0x0000000000003c84 
    /*00003c34*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcf4] 
    /*00003c38*/ MOV         R0, #4 
    /*00003c3c*/ MVN         IP, #0 
    /*00003c40*/ STR         IP, [SP, #12] 
    /*00003c44*/ BL          0x0000000000003C44          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003c48*/ B           AOBufInit_x3bcc
    AOBufInit_x3c4c:
    /*00003c4c*/ MOVW        IP, #32780                  @ 0x800c 
    /*00003c50*/ MOV         R3, #91                     @ 0x5b 
    /*00003c54*/ MOV         R4, IP 
    /*00003c58*/ STR         R3, [SP] 
    /*00003c5c*/ MOV         R1, R9 
    /*00003c60*/ MOV         R0, SL 
    /*00003c64*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd34] 
    /*00003c68*/ LDR         R3, [PC, #20]               @ 0x0000000000003c84 
    /*00003c6c*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd34] 
    /*00003c70*/ BL          0x0000000000003C70          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003c74*/ MOV         IP, R4 
    /*00003c78*/ MOVT        IP, #40982                  @ 0xa016 
    /*00003c7c*/ STR         IP, [SP, #12] 
    /*00003c80*/ B           AOBufInit_x3bcc
    /*00003c84*/ ANDEQ       R0, R0, IP, ROR #2 
FUNC_END AOBufInit


FUNC_BEGIN AOBufExit
    /*00003c88*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00003c8c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00003c90*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00003c94*/ MOV         R4, #136                    @ 0x88 
    /*00003c98*/ MLA         R4, R4, R0, R3 
    /*00003c9c*/ MOV         R5, R0 
    /*00003ca0*/ MOV         R6, #0 
    /*00003ca4*/ MOV         R7, #0 
    /*00003ca8*/ LDRD        R0, [R4, #56]               @ 0x38 
    /*00003cac*/ LDR         R2, [R4, #64]               @ 0x40 
    /*00003cb0*/ BL          0x0000000000003CB0          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*00003cb4*/ MOV         R0, R5 
    /*00003cb8*/ MOV         R3, #0 
    /*00003cbc*/ STRD        R6, [R4, #56]               @ 0x38 
    /*00003cc0*/ STR         R3, [R4, #64]               @ 0x40 
    /*00003cc4*/ POP         {R4, R5, R6, R7, R8, LR} 
    AOBufExit_x3cc8:
    /*00003cc8*/ B           AOBufExit_x3cc8             @ AO_DRV_ClrDmaBufAddr [R_ARM_JUMP24: .text+0x74c4] 
FUNC_END AOBufExit


FUNC_BEGIN AoSaveDmaRdPtr
    /*00003ccc*/ LDR         R3, [PC, #8]                @ 0x0000000000003cdc 
    /*00003cd0*/ MOV         R1, #184                    @ 0xb8 
    /*00003cd4*/ MLA         R1, R1, R0, R3 
    AoSaveDmaRdPtr_x3cd8:
    /*00003cd8*/ B           AoSaveDmaRdPtr_x3cd8        @ AO_DRV_GetDmaRdpt [R_ARM_JUMP24: .text+0x8934] 
    /*00003cdc*/ ANDEQ       R0, R0, R0, LSR #32         @ g_stAoDrvDev [R_ARM_ABS32: .bss+0x44e8] 
FUNC_END AoSaveDmaRdPtr


FUNC_BEGIN AoSetDmaRdOffsetInitFlag
    /*00003ce0*/ ADD         R0, R0, R0, LSL #1 
    /*00003ce4*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00003ce8*/ ADD         R1, R0, R1 
    /*00003cec*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00003cf0*/ MOVW        R0, #2892                   @ 0xb4c 
    /*00003cf4*/ MOV         R2, #0 
    /*00003cf8*/ MLA         R1, R0, R1, R3 
    /*00003cfc*/ STR         R2, [R1, #1532]             @ 0x5fc 
    /*00003d00*/ BX          LR 
FUNC_END AoSetDmaRdOffsetInitFlag


FUNC_BEGIN AoGetData
    /*00003d04*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00003d08*/ SUBS        R9, R2, #0 
    /*00003d0c*/ SUB         SP, SP, #44                 @ 0x2c 
    /*00003d10*/ MOV         R5, R0 
    /*00003d14*/ STR         R1, [SP, #24] 
    /*00003d18*/ BEQ         AoGetData_x401c
    AoGetData_x3d1c:
    /*00003d1c*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00003d20*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00003d24*/ MOV         R3, #136                    @ 0x88 
    /*00003d28*/ LDR         IP, [R9, #44]               @ 0x2c 
    /*00003d2c*/ MLA         R3, R3, R5, R4 
    /*00003d30*/ LDR         R0, [R3, #40]               @ 0x28 
    /*00003d34*/ LDR         R2, [R3, #20] 
    /*00003d38*/ LSL         R1, R0, R2 
    /*00003d3c*/ CMP         IP, R1 
    /*00003d40*/ BNE         AoGetData_x403c
    /*00003d44*/ LDR         R1, [R9] 
    /*00003d48*/ CMP         R2, R1 
    /*00003d4c*/ BNE         AoGetData_x407c
    /*00003d50*/ LDR         R1, [R9, #4] 
    /*00003d54*/ LDR         R2, [R3, #28] 
    /*00003d58*/ CMP         R1, R2 
    /*00003d5c*/ BNE         AoGetData_x40b0
    /*00003d60*/ LDR         R2, [R3, #12] 
    /*00003d64*/ ADD         R7, R5, R5, LSL #1 
    /*00003d68*/ LDR         R1, [SP, #24] 
    /*00003d6c*/ CMP         R2, #1 
    /*00003d70*/ ADD         R7, R7, R1 
    /*00003d74*/ BNE         AoGetData_x3fe0
    /*00003d78*/ MOVW        R2, #2892                   @ 0xb4c 
    /*00003d7c*/ MLA         R2, R2, R7, R4 
    /*00003d80*/ LDR         R2, [R2, #1512]             @ 0x5e8 
    /*00003d84*/ CMP         R2, #1 
    /*00003d88*/ BNE         AoGetData_x3fe0
    /*00003d8c*/ MOVW        R2, #16960                  @ 0x4240 
    /*00003d90*/ MOVT        R2, #15 
    /*00003d94*/ LDR         R1, [R3, #16] 
    /*00003d98*/ MOVW        R6, #0                      @ g_stAoDrvDev [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00003d9c*/ MUL         R0, R2, R0 
    /*00003da0*/ BL          0x0000000000003DA0          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00003da4*/ MOV         R2, #184                    @ 0xb8 
    /*00003da8*/ MOV         R1, #136                    @ 0x88 
    /*00003dac*/ MOVT        R6, #0                      @ g_stAoDrvDev [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00003db0*/ MUL         R2, R2, R5 
    /*00003db4*/ MOVW        R3, #10000                  @ 0x2710 
    /*00003db8*/ MLA         R1, R1, R5, R4 
    /*00003dbc*/ ADD         R8, R6, R2 
    /*00003dc0*/ LDR         SL, [R8, #116]              @ 0x74 
    /*00003dc4*/ STR         R2, [SP, #28] 
    /*00003dc8*/ CMP         R0, R3 
    /*00003dcc*/ MOV         FP, R0 
    /*00003dd0*/ LSRLS       R3, R0, #1 
    /*00003dd4*/ MOV         R0, SL 
    /*00003dd8*/ MOVHI       R3, #8000                   @ 0x1f40 
    /*00003ddc*/ STR         R3, [SP, #32] 
    /*00003de0*/ LDR         R3, [R1, #44]               @ 0x2c 
    /*00003de4*/ MOV         R1, R3 
    /*00003de8*/ STR         R3, [SP, #36]               @ 0x24 
    /*00003dec*/ BL          0x0000000000003DEC          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00003df0*/ LDR         R3, [SP, #32] 
    /*00003df4*/ MOV         R1, FP 
    /*00003df8*/ MUL         R0, R3, R0 
    /*00003dfc*/ BL          0x0000000000003DFC          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00003e00*/ MOV         R3, #184                    @ 0xb8 
    /*00003e04*/ MLA         R3, R3, R5, R6 
    /*00003e08*/ LDR         R1, [R8, #32] 
    /*00003e0c*/ LDR         R2, [SP, #28] 
    /*00003e10*/ ADD         R0, R0, SL 
    /*00003e14*/ CMP         R0, R1 
    /*00003e18*/ SUBLS       R8, R1, R0 
    /*00003e1c*/ LDRHI       R8, [R6, R2] 
    /*00003e20*/ LDR         R2, [R3, #24] 
    /*00003e24*/ MOVW        R3, #2892                   @ 0xb4c 
    /*00003e28*/ MLA         R3, R3, R7, R4 
    /*00003e2c*/ ADDHI       R8, R1, R8 
    /*00003e30*/ SUBHI       R8, R8, R0 
    /*00003e34*/ CMP         R1, R2 
    /*00003e38*/ BIC         R8, R8, #31 
    /*00003e3c*/ LDR         R3, [R3, #1528]             @ 0x5f8 
    /*00003e40*/ BCS         AoGetData_x3ecc
    /*00003e44*/ CMP         R1, R3 
    /*00003e48*/ MOVCS       R0, #0 
    /*00003e4c*/ MOVCC       R0, #1 
    /*00003e50*/ CMP         R2, R3 
    /*00003e54*/ MOVLS       R0, #0 
    /*00003e58*/ CMP         R0, #0 
    /*00003e5c*/ BEQ         AoGetData_x3ecc
    AoGetData_x3e60:
    /*00003e60*/ MOVW        R3, #2892                   @ 0xb4c 
    /*00003e64*/ MOV         R2, #1 
    /*00003e68*/ MLA         R3, R3, R7, R4 
    /*00003e6c*/ STR         R8, [R3, #1528]             @ 0x5f8 
    /*00003e70*/ STR         R2, [R3, #1532]             @ 0x5fc 
    AoGetData_x3e74:
    /*00003e74*/ MOV         R3, R8 
    /*00003e78*/ MOV         R2, R9 
    /*00003e7c*/ LDR         R1, [SP, #24] 
    /*00003e80*/ MOV         R0, R5 
    /*00003e84*/ BL          0x0000000000003E84          @ AO_DRV_GetAECData [R_ARM_CALL: .text+0x8344] 
    /*00003e88*/ SUBS        R8, R0, #0 
    /*00003e8c*/ BNE         AoGetData_x3fb4
    /*00003e90*/ MOV         R2, #184                    @ 0xb8 
    /*00003e94*/ MOVW        R3, #2892                   @ 0xb4c 
    /*00003e98*/ MLA         R4, R3, R7, R4 
    /*00003e9c*/ MUL         R5, R2, R5 
    /*00003ea0*/ ADD         R3, R6, R5 
    /*00003ea4*/ LDR         R1, [R3, #116]              @ 0x74 
    /*00003ea8*/ LDR         R3, [R4, #1528]             @ 0x5f8 
    /*00003eac*/ LDR         R2, [R6, R5] 
    /*00003eb0*/ ADD         R3, R3, R1 
    /*00003eb4*/ CMP         R3, R2 
    /*00003eb8*/ SUBCS       R3, R3, R2 
    /*00003ebc*/ STR         R3, [R4, #1528]             @ 0x5f8 
    AoGetData_x3ec0:
    /*00003ec0*/ MOV         R0, R8 
    /*00003ec4*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00003ec8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AoGetData_x3ecc:
    /*00003ecc*/ CMP         R1, R2 
    /*00003ed0*/ BHI         AoGetData_x3f54
    /*00003ed4*/ CMP         R8, R3 
    /*00003ed8*/ BLS         AoGetData_x3f7c
    AoGetData_x3edc:
    /*00003edc*/ SUB         R2, R8, R3 
    /*00003ee0*/ CMP         R2, SL, LSL #1 
    /*00003ee4*/ BHI         AoGetData_x3f9c
    AoGetData_x3ee8:
    /*00003ee8*/ STR         R3, [SP, #32] 
    /*00003eec*/ MOV         R1, SL 
    /*00003ef0*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00003ef4*/ STR         R2, [SP, #28] 
    /*00003ef8*/ MUL         R0, R3, FP 
    /*00003efc*/ MUL         R0, R2, R0 
    /*00003f00*/ BL          0x0000000000003F00          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00003f04*/ LDR         R2, [SP, #28] 
    /*00003f08*/ MOV         R1, SL 
    /*00003f0c*/ CMP         R0, #8000                   @ 0x1f40 
    /*00003f10*/ MOV         R0, R2 
    /*00003f14*/ MOVLS       SL, #0 
    /*00003f18*/ MOVHI       SL, #1 
    /*00003f1c*/ BL          0x0000000000003F1C          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00003f20*/ LDR         R3, [SP, #32] 
    /*00003f24*/ CMP         R0, #2 
    /*00003f28*/ MOVLS       R0, SL 
    /*00003f2c*/ ORRHI       R0, SL, #1 
    /*00003f30*/ CMP         R0, #0 
    /*00003f34*/ BNE         AoGetData_x3e60
    /*00003f38*/ MOVW        R2, #2892                   @ 0xb4c 
    /*00003f3c*/ MLA         R2, R2, R7, R4 
    /*00003f40*/ LDR         R2, [R2, #1532]             @ 0x5fc 
    /*00003f44*/ CMP         R2, #0 
    /*00003f48*/ BEQ         AoGetData_x3e60
    /*00003f4c*/ MOV         R8, R3 
    /*00003f50*/ B           AoGetData_x3e74
    AoGetData_x3f54:
    /*00003f54*/ CMP         R2, R3 
    /*00003f58*/ MOVLS       R2, #0 
    /*00003f5c*/ MOVHI       R2, #1 
    /*00003f60*/ CMP         R1, R3 
    /*00003f64*/ MOVCS       R1, R2 
    /*00003f68*/ ORRCC       R1, R2, #1 
    /*00003f6c*/ CMP         R1, #0 
    /*00003f70*/ BNE         AoGetData_x3e60
    /*00003f74*/ CMP         R8, R3 
    /*00003f78*/ BHI         AoGetData_x3edc
    AoGetData_x3f7c:
    /*00003f7c*/ SUB         R2, R3, R8 
    /*00003f80*/ CMP         R2, SL, LSL #1 
    /*00003f84*/ MOVHI       R2, #184                    @ 0xb8 
    /*00003f88*/ MULHI       R2, R2, R5 
    /*00003f8c*/ LDRHI       R2, [R6, R2] 
    /*00003f90*/ ADDHI       R2, R8, R2 
    /*00003f94*/ SUBHI       R2, R2, R3 
    /*00003f98*/ B           AoGetData_x3ee8
    AoGetData_x3f9c:
    /*00003f9c*/ MOV         R2, #184                    @ 0xb8 
    /*00003fa0*/ MUL         R2, R2, R5 
    /*00003fa4*/ LDR         R2, [R6, R2] 
    /*00003fa8*/ SUB         R2, R2, R8 
    /*00003fac*/ ADD         R2, R2, R3 
    /*00003fb0*/ B           AoGetData_x3ee8
    AoGetData_x3fb4:
    /*00003fb4*/ MOVW        R3, #405                    @ 0x195 
    /*00003fb8*/ MOV         R1, #22 
    /*00003fbc*/ STR         R3, [SP] 
    /*00003fc0*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf14] 
    /*00003fc4*/ LDR         R3, [PC, #256]              @ 0x00000000000040cc 
    /*00003fc8*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf14] 
    /*00003fcc*/ MOV         R0, #3 
    /*00003fd0*/ BL          0x0000000000003FD0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003fd4*/ MOV         R0, R8 
    /*00003fd8*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00003fdc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AoGetData_x3fe0:
    /*00003fe0*/ MOV         R3, #336                    @ 0x150 
    /*00003fe4*/ MOV         R1, #22 
    /*00003fe8*/ STR         R3, [SP] 
    /*00003fec*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xedc] 
    /*00003ff0*/ LDR         R3, [PC, #212]              @ 0x00000000000040cc 
    /*00003ff4*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0xedc] 
    /*00003ff8*/ MOV         R0, #3 
    /*00003ffc*/ MOVW        R8, #32773                  @ 0x8005 
    /*00004000*/ BL          0x0000000000004000          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004004*/ MOVT        R8, #40982                  @ 0xa016 
    /*00004008*/ MOV         R3, #0 
    /*0000400c*/ STR         R3, [R9, #44]               @ 0x2c 
    /*00004010*/ MOV         R0, R8 
    /*00004014*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00004018*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AoGetData_x401c:
    /*0000401c*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd84] 
    /*00004020*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd94] 
    /*00004024*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd84] 
    /*00004028*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd94] 
    /*0000402c*/ MOVW        R2, #303                    @ 0x12f 
    /*00004030*/ LDR         R1, [PC, #148]              @ 0x00000000000040cc 
    /*00004034*/ BL          0x0000000000004034          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004038*/ B           AoGetData_x3d1c
    AoGetData_x403c:
    /*0000403c*/ STR         R1, [SP, #12] 
    /*00004040*/ MOVW        R3, #309                    @ 0x135 
    /*00004044*/ LDR         R1, [SP, #24] 
    /*00004048*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdd4] 
    /*0000404c*/ STR         R3, [SP] 
    /*00004050*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdd4] 
    /*00004054*/ STR         IP, [SP, #16] 
    /*00004058*/ MOV         R0, #3 
    /*0000405c*/ STR         R1, [SP, #8] 
    /*00004060*/ MOV         R1, #22 
    /*00004064*/ STR         R5, [SP, #4] 
    /*00004068*/ MOVW        R8, #32771                  @ 0x8003 
    /*0000406c*/ LDR         R3, [PC, #88]               @ 0x00000000000040cc 
    /*00004070*/ MOVT        R8, #40982                  @ 0xa016 
    /*00004074*/ BL          0x0000000000004074          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004078*/ B           AoGetData_x3ec0
    AoGetData_x407c:
    /*0000407c*/ STR         R2, [SP, #4] 
    /*00004080*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe30] 
    /*00004084*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe30] 
    /*00004088*/ MOVW        R3, #317                    @ 0x13d 
    /*0000408c*/ STR         R1, [SP, #8] 
    /*00004090*/ STR         R3, [SP] 
    AoGetData_x4094:
    /*00004094*/ LDR         R3, [PC, #48]               @ 0x00000000000040cc 
    /*00004098*/ MOV         R1, #22 
    /*0000409c*/ MOV         R0, #3 
    /*000040a0*/ MOVW        R8, #32771                  @ 0x8003 
    /*000040a4*/ BL          0x00000000000040A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000040a8*/ MOVT        R8, #40982                  @ 0xa016 
    /*000040ac*/ B           AoGetData_x3ec0
    AoGetData_x40b0:
    /*000040b0*/ MOV         R3, #324                    @ 0x144 
    /*000040b4*/ STR         R2, [SP, #4] 
    /*000040b8*/ STR         R1, [SP, #8] 
    /*000040bc*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe84] 
    /*000040c0*/ STR         R3, [SP] 
    /*000040c4*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe84] 
    /*000040c8*/ B           AoGetData_x4094
    /*000040cc*/ ANDEQ       R0, R0, R8, ROR R1 
FUNC_END AoGetData


FUNC_BEGIN AoSetDevAttr
    /*000040d0*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*000040d4*/ MOV         R8, R0 
    /*000040d8*/ SUB         SP, SP, #12 
    /*000040dc*/ MOV         R0, #20 
    /*000040e0*/ MOV         R9, R1 
    /*000040e4*/ BL          0x00000000000040E4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000040e8*/ CMP         R0, #0 
    /*000040ec*/ BEQ         AoSetDevAttr_x4200
    /*000040f0*/ CMP         R8, #1 
    /*000040f4*/ BHI         AoSetDevAttr_x4190
    /*000040f8*/ CMP         R9, #0 
    /*000040fc*/ BEQ         AoSetDevAttr_x420c
    /*00004100*/ MOV         R4, #136                    @ 0x88 
    /*00004104*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00004108*/ MUL         R4, R4, R8 
    /*0000410c*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00004110*/ ADD         R6, R4, #124                @ 0x7c 
    /*00004114*/ ADD         R6, R5, R6 
    /*00004118*/ MOV         R0, R6 
    /*0000411c*/ BL          0x000000000000411C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00004120*/ CMP         R0, #0 
    /*00004124*/ BNE         AoSetDevAttr_x41c0
    /*00004128*/ ADD         R7, R5, R4 
    /*0000412c*/ LDR         R3, [R7, #12] 
    /*00004130*/ CMP         R3, #0 
    /*00004134*/ BNE         AoSetDevAttr_x41cc
    /*00004138*/ MOV         R0, R8 
    /*0000413c*/ MOV         R1, R9 
    /*00004140*/ BL          0x0000000000004140          @ AO_DRV_SetDevAttr [R_ARM_CALL: .text+0x74f8] 
    /*00004144*/ SUBS        R8, R0, #0 
    /*00004148*/ BNE         AoSetDevAttr_x417c
    /*0000414c*/ ADD         R0, R4, #16 
    /*00004150*/ MOV         R1, R9 
    /*00004154*/ ADD         R0, R5, R0 
    /*00004158*/ MOV         R2, #40                     @ 0x28 
    /*0000415c*/ BL          0x000000000000415C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004160*/ MOV         R3, #1 
    /*00004164*/ MOV         R0, R6 
    /*00004168*/ STR         R3, [R7, #8] 
    /*0000416c*/ BL          0x000000000000416C          @ osal_up [R_ARM_CALL: +0x0] 
    AoSetDevAttr_x4170:
    /*00004170*/ MOV         R0, R8 
    /*00004174*/ ADD         SP, SP, #12 
    /*00004178*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AoSetDevAttr_x417c:
    /*0000417c*/ MOV         R0, R6 
    /*00004180*/ BL          0x0000000000004180          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004184*/ MOV         R0, R8 
    /*00004188*/ ADD         SP, SP, #12 
    /*0000418c*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AoSetDevAttr_x4190:
    /*00004190*/ STR         R8, [SP, #4] 
    /*00004194*/ MOVW        R3, #430                    @ 0x1ae 
    /*00004198*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf4c] 
    /*0000419c*/ STR         R3, [SP] 
    /*000041a0*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf4c] 
    /*000041a4*/ LDR         R3, [PC, #144]              @ 0x000000000000423c 
    /*000041a8*/ MOV         R1, #22 
    /*000041ac*/ MOV         R0, #3 
    /*000041b0*/ MOVW        R8, #32769                  @ 0x8001 
    /*000041b4*/ BL          0x00000000000041B4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000041b8*/ MOVT        R8, #40982                  @ 0xa016 
    /*000041bc*/ B           AoSetDevAttr_x4170
    AoSetDevAttr_x41c0:
    /*000041c0*/ MOV         R8, #65024                  @ 0xfe00 
    /*000041c4*/ MOVT        R8, #65535                  @ 0xffff 
    /*000041c8*/ B           AoSetDevAttr_x4170
    AoSetDevAttr_x41cc:
    /*000041cc*/ MOV         R0, R6 
    /*000041d0*/ MOVW        R8, #32777                  @ 0x8009 
    /*000041d4*/ BL          0x00000000000041D4          @ osal_up [R_ARM_CALL: +0x0] 
    /*000041d8*/ MOVW        R3, #450                    @ 0x1c2 
    /*000041dc*/ MOVW        R2, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfd0] 
    /*000041e0*/ STR         R3, [SP] 
    /*000041e4*/ MOVT        R2, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfd0] 
    /*000041e8*/ LDR         R3, [PC, #76]               @ 0x000000000000423c 
    /*000041ec*/ MOV         R1, #22 
    /*000041f0*/ MOV         R0, #3 
    /*000041f4*/ BL          0x00000000000041F4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000041f8*/ MOVT        R8, #40982                  @ 0xa016 
    /*000041fc*/ B           AoSetDevAttr_x4170
    AoSetDevAttr_x4200:
    /*00004200*/ MOVW        R8, #32784                  @ 0x8010 
    /*00004204*/ MOVT        R8, #40982                  @ 0xa016 
    /*00004208*/ B           AoSetDevAttr_x4170
    AoSetDevAttr_x420c:
    /*0000420c*/ STR         R8, [SP, #4] 
    /*00004210*/ MOVW        R3, #437                    @ 0x1b5 
    /*00004214*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf80] 
    /*00004218*/ STR         R3, [SP] 
    /*0000421c*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf80] 
    /*00004220*/ LDR         R3, [PC, #20]               @ 0x000000000000423c 
    /*00004224*/ MOV         R1, #22 
    /*00004228*/ MOV         R0, #3 
    /*0000422c*/ MOVW        R8, #32774                  @ 0x8006 
    /*00004230*/ BL          0x0000000000004230          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004234*/ MOVT        R8, #40982                  @ 0xa016 
    /*00004238*/ B           AoSetDevAttr_x4170
    /*0000423c*/ ANDEQ       R0, R0, R4, LSL #3 
FUNC_END AoSetDevAttr


FUNC_BEGIN AoGetDevAttr
    /*00004240*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00004244*/ MOV         R9, R0 
    /*00004248*/ SUB         SP, SP, #12 
    /*0000424c*/ MOV         R0, #20 
    /*00004250*/ MOV         R8, R1 
    /*00004254*/ BL          0x0000000000004254          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00004258*/ CMP         R0, #0 
    /*0000425c*/ BEQ         AoGetDevAttr_x4344
    /*00004260*/ CMP         R9, #1 
    /*00004264*/ BHI         AoGetDevAttr_x4308
    /*00004268*/ CMP         R8, #0 
    /*0000426c*/ BEQ         AoGetDevAttr_x4350
    /*00004270*/ MOV         R4, #136                    @ 0x88 
    /*00004274*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00004278*/ MUL         R4, R4, R9 
    /*0000427c*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00004280*/ ADD         R6, R4, #124                @ 0x7c 
    /*00004284*/ ADD         R6, R5, R6 
    /*00004288*/ MOV         R0, R6 
    /*0000428c*/ BL          0x000000000000428C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00004290*/ SUBS        R7, R0, #0 
    /*00004294*/ BNE         AoGetDevAttr_x4338
    /*00004298*/ ADD         R3, R5, R4 
    /*0000429c*/ LDR         R3, [R3, #8] 
    /*000042a0*/ CMP         R3, #1 
    /*000042a4*/ BNE         AoGetDevAttr_x42d0
    /*000042a8*/ ADD         R4, R4, #16 
    /*000042ac*/ MOV         R2, #40                     @ 0x28 
    /*000042b0*/ ADD         R1, R5, R4 
    /*000042b4*/ MOV         R0, R8 
    /*000042b8*/ BL          0x00000000000042B8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000042bc*/ MOV         R0, R6 
    /*000042c0*/ BL          0x00000000000042C0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000042c4*/ MOV         R0, R7 
    AoGetDevAttr_x42c8:
    /*000042c8*/ ADD         SP, SP, #12 
    /*000042cc*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AoGetDevAttr_x42d0:
    /*000042d0*/ MOV         R0, R6 
    /*000042d4*/ BL          0x00000000000042D4          @ osal_up [R_ARM_CALL: +0x0] 
    /*000042d8*/ MOVW        R3, #494                    @ 0x1ee 
    /*000042dc*/ STR         R9, [SP, #4] 
    /*000042e0*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x105c] 
    /*000042e4*/ STR         R3, [SP] 
    /*000042e8*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x105c] 
    /*000042ec*/ LDR         R3, [PC, #140]              @ 0x0000000000004380 
    /*000042f0*/ MOV         R1, #22 
    /*000042f4*/ MOV         R0, #3 
    /*000042f8*/ BL          0x00000000000042F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000042fc*/ MOVW        R0, #32775                  @ 0x8007 
    /*00004300*/ MOVT        R0, #40982                  @ 0xa016 
    /*00004304*/ B           AoGetDevAttr_x42c8
    AoGetDevAttr_x4308:
    /*00004308*/ MOVW        R3, #475                    @ 0x1db 
    /*0000430c*/ STR         R9, [SP, #4] 
    /*00004310*/ STR         R3, [SP] 
    /*00004314*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf4c] 
    /*00004318*/ LDR         R3, [PC, #96]               @ 0x0000000000004380 
    /*0000431c*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf4c] 
    /*00004320*/ MOV         R1, #22 
    /*00004324*/ MOV         R0, #3 
    /*00004328*/ BL          0x0000000000004328          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000432c*/ MOVW        R0, #32769                  @ 0x8001 
    /*00004330*/ MOVT        R0, #40982                  @ 0xa016 
    /*00004334*/ B           AoGetDevAttr_x42c8
    AoGetDevAttr_x4338:
    /*00004338*/ MOV         R0, #65024                  @ 0xfe00 
    /*0000433c*/ MOVT        R0, #65535                  @ 0xffff 
    /*00004340*/ B           AoGetDevAttr_x42c8
    AoGetDevAttr_x4344:
    /*00004344*/ MOVW        R0, #32784                  @ 0x8010 
    /*00004348*/ MOVT        R0, #40982                  @ 0xa016 
    /*0000434c*/ B           AoGetDevAttr_x42c8
    AoGetDevAttr_x4350:
    /*00004350*/ MOVW        R3, #482                    @ 0x1e2 
    /*00004354*/ STR         R9, [SP, #4] 
    /*00004358*/ STR         R3, [SP] 
    /*0000435c*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1014] 
    /*00004360*/ LDR         R3, [PC, #24]               @ 0x0000000000004380 
    /*00004364*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1014] 
    /*00004368*/ MOV         R1, #22 
    /*0000436c*/ MOV         R0, #3 
    /*00004370*/ BL          0x0000000000004370          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004374*/ MOVW        R0, #32774                  @ 0x8006 
    /*00004378*/ MOVT        R0, #40982                  @ 0xa016 
    /*0000437c*/ B           AoGetDevAttr_x42c8
    /*00004380*/ MULEQ       R0, R4, R1 
FUNC_END AoGetDevAttr


FUNC_BEGIN AoEnableDev
    /*00004384*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00004388*/ MOV         R8, R0 
    /*0000438c*/ SUB         SP, SP, #28 
    /*00004390*/ MOV         R0, #20 
    /*00004394*/ BL          0x0000000000004394          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00004398*/ CMP         R0, #0 
    /*0000439c*/ BEQ         AoEnableDev_x45dc
    /*000043a0*/ CMP         R8, #1 
    /*000043a4*/ BHI         AoEnableDev_x4538
    /*000043a8*/ MOV         R9, #136                    @ 0x88 
    /*000043ac*/ MOVW        R7, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*000043b0*/ MUL         R9, R9, R8 
    /*000043b4*/ MOVT        R7, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*000043b8*/ ADD         R4, R7, R9 
    /*000043bc*/ LDR         R3, [R4, #4] 
    /*000043c0*/ CMP         R3, #0 
    /*000043c4*/ BEQ         AoEnableDev_x4568
    /*000043c8*/ ADD         SL, R9, #124                @ 0x7c 
    /*000043cc*/ ADD         R3, R7, SL 
    /*000043d0*/ STR         R3, [SP, #8] 
    /*000043d4*/ MOV         R0, R3 
    /*000043d8*/ BL          0x00000000000043D8          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000043dc*/ SUBS        R5, R0, #0 
    /*000043e0*/ BNE         AoEnableDev_x4598
    /*000043e4*/ LDR         R3, [R4, #12] 
    /*000043e8*/ CMP         R3, #1 
    /*000043ec*/ BEQ         AoEnableDev_x4508
    /*000043f0*/ LDR         R3, [R4, #8] 
    /*000043f4*/ CMP         R3, #1 
    /*000043f8*/ BNE         AoEnableDev_x45a4
    /*000043fc*/ ADD         R6, R8, R8, LSL #1 
    /*00004400*/ MOVW        R4, #2892                   @ 0xb4c 
    /*00004404*/ MOVW        SL, #30465                  @ 0x7701 
    /*00004408*/ ADD         FP, R6, #3 
    /*0000440c*/ MLA         R4, R4, R6, R7 
    /*00004410*/ MOVT        SL, #1 
    /*00004414*/ ADD         R9, R9, #16 
    /*00004418*/ ADD         R3, R7, R9 
    /*0000441c*/ STR         R3, [SP, #12] 
    /*00004420*/ ADD         R4, R4, #276                @ 0x114 
    AoEnableDev_x4424:
    /*00004424*/ MOVW        R2, #2888                   @ 0xb48 
    /*00004428*/ MOV         R0, R4 
    /*0000442c*/ MOV         R1, #0 
    /*00004430*/ MOV         R9, #1 
    /*00004434*/ BL          0x0000000000004434          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00004438*/ MOVW        R3, #2892                   @ 0xb4c 
    /*0000443c*/ MLA         R2, R3, R6, R7 
    /*00004440*/ ADD         R6, R6, R9 
    /*00004444*/ CMP         R6, FP 
    /*00004448*/ STR         R5, [R4, #1236]             @ 0x4d4 
    /*0000444c*/ STR         R9, [R2, #1516]             @ 0x5ec 
    /*00004450*/ STR         R5, [R4, #1264]             @ 0x4f0 
    /*00004454*/ STR         R5, [R4, #1272]             @ 0x4f8 
    /*00004458*/ STR         R5, [R4, #1300]             @ 0x514 
    /*0000445c*/ STR         SL, [R4, #1292]             @ 0x50c 
    /*00004460*/ STR         R5, [R4, #2352]             @ 0x930 
    /*00004464*/ STR         R5, [R4, #2356]             @ 0x934 
    /*00004468*/ STR         SL, [R4, #2360]             @ 0x938 
    /*0000446c*/ STR         SL, [R4, #2364]             @ 0x93c 
    /*00004470*/ STR         R5, [R4, #1252]             @ 0x4e4 
    /*00004474*/ STR         R5, [R4, #1256]             @ 0x4e8 
    /*00004478*/ ADD         R4, R4, R3 
    /*0000447c*/ BNE         AoEnableDev_x4424
    /*00004480*/ LDR         R1, [SP, #12] 
    /*00004484*/ MOV         R0, R8 
    /*00004488*/ BL          0x0000000000004488          @ AOBufInit [R_ARM_CALL: .text+0x38f8] 
    /*0000448c*/ SUBS        R5, R0, #0 
    /*00004490*/ BNE         AoEnableDev_x4508
    /*00004494*/ MOV         R0, R8 
    /*00004498*/ BL          0x0000000000004498          @ AO_DRV_EnableDev [R_ARM_CALL: .text+0x765c] 
    /*0000449c*/ SUBS        R5, R0, #0 
    /*000044a0*/ BNE         AoEnableDev_x451c
    /*000044a4*/ ADD         R1, SP, #20 
    /*000044a8*/ MOVW        R0, #0                      @ g_stAoBufLock [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000044ac*/ MOVT        R0, #0                      @ g_stAoBufLock [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000044b0*/ BL          0x00000000000044B0          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000044b4*/ MOV         R3, #136                    @ 0x88 
    /*000044b8*/ MLA         R7, R3, R8, R7 
    /*000044bc*/ ADD         R1, SP, #20 
    /*000044c0*/ MOVW        R0, #0                      @ g_stAoBufLock [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000044c4*/ MOVT        R0, #0                      @ g_stAoBufLock [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000044c8*/ STR         R9, [R7, #12] 
    /*000044cc*/ BL          0x00000000000044CC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000044d0*/ LDR         R0, [SP, #8] 
    /*000044d4*/ BL          0x00000000000044D4          @ osal_up [R_ARM_CALL: +0x0] 
    /*000044d8*/ MOV         R3, #592                    @ 0x250 
    /*000044dc*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1100] 
    /*000044e0*/ STR         R3, [SP] 
    /*000044e4*/ STR         R8, [SP, #4] 
    /*000044e8*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1100] 
    /*000044ec*/ LDR         R3, [PC, #244]              @ 0x00000000000045e8 
    /*000044f0*/ MOV         R1, #22 
    /*000044f4*/ MOV         R0, #6 
    /*000044f8*/ BL          0x00000000000044F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    AoEnableDev_x44fc:
    /*000044fc*/ MOV         R0, R5 
    /*00004500*/ ADD         SP, SP, #28 
    /*00004504*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AoEnableDev_x4508:
    /*00004508*/ LDR         R0, [SP, #8] 
    /*0000450c*/ BL          0x000000000000450C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004510*/ MOV         R0, R5 
    /*00004514*/ ADD         SP, SP, #28 
    /*00004518*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AoEnableDev_x451c:
    /*0000451c*/ LDR         R0, [SP, #8] 
    /*00004520*/ BL          0x0000000000004520          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004524*/ MOV         R0, R8 
    /*00004528*/ BL          0x0000000000004528          @ AOBufExit [R_ARM_CALL: .text+0x3c88] 
    /*0000452c*/ MOV         R0, R5 
    /*00004530*/ ADD         SP, SP, #28 
    /*00004534*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AoEnableDev_x4538:
    /*00004538*/ MOV         R3, #516                    @ 0x204 
    /*0000453c*/ STR         R8, [SP, #4] 
    /*00004540*/ STR         R3, [SP] 
    /*00004544*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf4c] 
    /*00004548*/ LDR         R3, [PC, #152]              @ 0x00000000000045e8 
    /*0000454c*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf4c] 
    /*00004550*/ MOV         R1, #22 
    /*00004554*/ MOV         R0, #3 
    /*00004558*/ MOVW        R5, #32769                  @ 0x8001 
    /*0000455c*/ BL          0x000000000000455C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004560*/ MOVT        R5, #40982                  @ 0xa016 
    /*00004564*/ B           AoEnableDev_x44fc
    AoEnableDev_x4568:
    /*00004568*/ MOVW        R3, #522                    @ 0x20a 
    /*0000456c*/ STR         R8, [SP, #4] 
    /*00004570*/ STR         R3, [SP] 
    /*00004574*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1094] 
    /*00004578*/ LDR         R3, [PC, #104]              @ 0x00000000000045e8 
    /*0000457c*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1094] 
    /*00004580*/ MOV         R1, #22 
    /*00004584*/ MOV         R0, #3 
    /*00004588*/ MOVW        R5, #32777                  @ 0x8009 
    /*0000458c*/ BL          0x000000000000458C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004590*/ MOVT        R5, #40982                  @ 0xa016 
    /*00004594*/ B           AoEnableDev_x44fc
    AoEnableDev_x4598:
    /*00004598*/ MOV         R5, #65024                  @ 0xfe00 
    /*0000459c*/ MOVT        R5, #65535                  @ 0xffff 
    /*000045a0*/ B           AoEnableDev_x44fc
    AoEnableDev_x45a4:
    /*000045a4*/ LDR         R0, [SP, #8] 
    /*000045a8*/ MOVW        R5, #32775                  @ 0x8007 
    /*000045ac*/ BL          0x00000000000045AC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000045b0*/ MOVW        R3, #539                    @ 0x21b 
    /*000045b4*/ STR         R8, [SP, #4] 
    /*000045b8*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10cc] 
    /*000045bc*/ STR         R3, [SP] 
    /*000045c0*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10cc] 
    /*000045c4*/ LDR         R3, [PC, #28]               @ 0x00000000000045e8 
    /*000045c8*/ MOV         R1, #22 
    /*000045cc*/ MOV         R0, #3 
    /*000045d0*/ MOVT        R5, #40982                  @ 0xa016 
    /*000045d4*/ BL          0x00000000000045D4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000045d8*/ B           AoEnableDev_x44fc
    AoEnableDev_x45dc:
    /*000045dc*/ MOVW        R5, #32784                  @ 0x8010 
    /*000045e0*/ MOVT        R5, #40982                  @ 0xa016 
    /*000045e4*/ B           AoEnableDev_x44fc
    /*000045e8*/ ANDEQ       R0, R0, R4, LSR #3 
FUNC_END AoEnableDev


FUNC_BEGIN AoDisableDev
    /*000045ec*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*000045f0*/ MOV         R5, R0 
    /*000045f4*/ SUB         SP, SP, #20 
    /*000045f8*/ MOV         R0, #20 
    /*000045fc*/ BL          0x00000000000045FC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00004600*/ CMP         R0, #0 
    /*00004604*/ BEQ         AoDisableDev_x48ac
    /*00004608*/ CMP         R5, #1 
    /*0000460c*/ BHI         AoDisableDev_x4844
    /*00004610*/ MOV         R6, #136                    @ 0x88 
    /*00004614*/ MOVW        R8, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00004618*/ MUL         R6, R6, R5 
    /*0000461c*/ MOVT        R8, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00004620*/ ADD         R4, R8, R6 
    /*00004624*/ LDR         R3, [R4, #4] 
    /*00004628*/ CMP         R3, #0 
    /*0000462c*/ BEQ         AoDisableDev_x4874
    /*00004630*/ ADD         R7, R6, #124                @ 0x7c 
    /*00004634*/ ADD         R7, R8, R7 
    /*00004638*/ MOV         R0, R7 
    /*0000463c*/ BL          0x000000000000463C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00004640*/ CMP         R0, #0 
    /*00004644*/ BNE         AoDisableDev_x48a0
    /*00004648*/ LDR         R3, [R4, #12] 
    /*0000464c*/ CMP         R3, #0 
    /*00004650*/ BEQ         AoDisableDev_x482c
    /*00004654*/ LDR         R0, [R4, #44]               @ 0x2c 
    /*00004658*/ CMP         R0, #0 
    /*0000465c*/ BEQ         AoDisableDev_x46bc
    /*00004660*/ ADD         R4, R5, R5, LSL #1 
    /*00004664*/ MOVW        R1, #2892                   @ 0xb4c 
    /*00004668*/ MUL         R2, R1, R4 
    /*0000466c*/ ADD         R3, R8, R2 
    /*00004670*/ LDR         R3, [R3, #1512]             @ 0x5e8 
    /*00004674*/ SUB         R3, R3, #1 
    /*00004678*/ CMP         R3, #1 
    /*0000467c*/ BLS         AoDisableDev_x47d0
    /*00004680*/ ADD         R2, R8, R2 
    /*00004684*/ ADD         R0, R0, R4 
    /*00004688*/ ADD         R2, R2, #4352               @ 0x1100 
    /*0000468c*/ ADD         R4, R4, #1 
    /*00004690*/ ADD         R2, R2, #52                 @ 0x34 
    /*00004694*/ MOV         IP, R1 
    /*00004698*/ B           AoDisableDev_x46b0
    AoDisableDev_x469c:
    /*0000469c*/ LDR         R3, [R2], IP 
    /*000046a0*/ SUB         R3, R3, #1 
    /*000046a4*/ CMP         R3, #1 
    /*000046a8*/ BLS         AoDisableDev_x47d0
    /*000046ac*/ MOV         R4, R1 
    AoDisableDev_x46b0:
    /*000046b0*/ CMP         R4, R0 
    /*000046b4*/ ADD         R1, R4, #1 
    /*000046b8*/ BNE         AoDisableDev_x469c
    AoDisableDev_x46bc:
    /*000046bc*/ MOV         R9, #136                    @ 0x88 
    /*000046c0*/ ADD         R1, SP, #12 
    /*000046c4*/ MLA         R9, R9, R5, R8 
    /*000046c8*/ MOVW        R0, #0                      @ g_stAoBufLock [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000046cc*/ MOVT        R0, #0                      @ g_stAoBufLock [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000046d0*/ MOV         R4, #0 
    /*000046d4*/ BL          0x00000000000046D4          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000046d8*/ ADD         R1, SP, #12 
    /*000046dc*/ MOVW        R0, #0                      @ g_stAoBufLock [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000046e0*/ MOVT        R0, #0                      @ g_stAoBufLock [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000046e4*/ STR         R4, [R9, #12] 
    /*000046e8*/ BL          0x00000000000046E8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000046ec*/ MOV         R0, R5 
    /*000046f0*/ BL          0x00000000000046F0          @ AO_DRV_DisableDev [R_ARM_CALL: .text+0x7730] 
    /*000046f4*/ ADD         R1, SP, #12 
    /*000046f8*/ MOVW        R0, #0                      @ g_stAoBufLock [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000046fc*/ MOVT        R0, #0                      @ g_stAoBufLock [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00004700*/ BL          0x0000000000004700          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00004704*/ LDR         LR, [R9, #44]               @ 0x2c 
    /*00004708*/ CMP         LR, R4 
    /*0000470c*/ BEQ         AoDisableDev_x4750
    /*00004710*/ LDR         IP, [PC, #416]              @ 0x00000000000048b8 
    /*00004714*/ ADD         R2, R5, R5, LSL #1 
    /*00004718*/ MOVW        R3, #2892                   @ 0xb4c 
    /*0000471c*/ ADD         LR, R2, LR 
    /*00004720*/ MOV         R0, R3 
    /*00004724*/ MOV         R1, R4 
    /*00004728*/ MLA         R3, R3, R2, IP 
    /*0000472c*/ MOVW        R4, #1516                   @ 0x5ec 
    AoDisableDev_x4730:
    /*00004730*/ MLA         IP, R0, R2, R8 
    /*00004734*/ ADD         R2, R2, #1 
    /*00004738*/ CMP         R2, LR 
    /*0000473c*/ STR         R1, [IP, R4] 
    /*00004740*/ STR         R1, [R3, #1240]             @ 0x4d8 
    /*00004744*/ STR         R1, [R3, #1248]             @ 0x4e0 
    /*00004748*/ ADD         R3, R3, R0 
    /*0000474c*/ BNE         AoDisableDev_x4730
    AoDisableDev_x4750:
    /*00004750*/ ADD         R1, SP, #12 
    /*00004754*/ MOVW        R0, #0                      @ g_stAoBufLock [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00004758*/ MOVT        R0, #0                      @ g_stAoBufLock [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000475c*/ BL          0x000000000000475C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00004760*/ ADD         R0, R6, #120                @ 0x78 
    /*00004764*/ ADD         R0, R8, R0 
    /*00004768*/ BL          0x0000000000004768          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*0000476c*/ MOV         R0, R5 
    /*00004770*/ BL          0x0000000000004770          @ AOBufExit [R_ARM_CALL: .text+0x3c88] 
    /*00004774*/ MOV         R3, #136                    @ 0x88 
    /*00004778*/ MLA         R8, R3, R5, R8 
    /*0000477c*/ LDR         R3, [R8, #8] 
    /*00004780*/ CMP         R3, #0 
    /*00004784*/ BEQ         AoDisableDev_x4794
    /*00004788*/ LDR         R3, [R8, #12] 
    /*0000478c*/ CMP         R3, #0 
    /*00004790*/ BEQ         AoDisableDev_x4810
    AoDisableDev_x4794:
    /*00004794*/ MOV         R0, R7 
    /*00004798*/ MOV         R6, #0 
    /*0000479c*/ BL          0x000000000000479C          @ osal_up [R_ARM_CALL: +0x0] 
    /*000047a0*/ MOVW        R3, #687                    @ 0x2af 
    /*000047a4*/ MOVW        R2, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11c8] 
    /*000047a8*/ STR         R3, [SP] 
    /*000047ac*/ STR         R5, [SP, #4] 
    /*000047b0*/ MOVT        R2, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11c8] 
    /*000047b4*/ LDR         R3, [PC, #256]              @ 0x00000000000048bc 
    /*000047b8*/ MOV         R1, #22 
    /*000047bc*/ MOV         R0, #6 
    /*000047c0*/ BL          0x00000000000047C0          @ HI_LOG [R_ARM_CALL: +0x0] 
    AoDisableDev_x47c4:
    /*000047c4*/ MOV         R0, R6 
    /*000047c8*/ ADD         SP, SP, #20 
    /*000047cc*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AoDisableDev_x47d0:
    /*000047d0*/ MOV         R0, R7 
    /*000047d4*/ MOVW        R6, #32777                  @ 0x8009 
    /*000047d8*/ BL          0x00000000000047D8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000047dc*/ STR         R4, [SP, #4] 
    /*000047e0*/ MOVW        R3, #639                    @ 0x27f 
    /*000047e4*/ MOV         R1, #22 
    /*000047e8*/ STR         R3, [SP] 
    /*000047ec*/ MOVW        R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1164] 
    /*000047f0*/ LDR         R3, [PC, #196]              @ 0x00000000000048bc 
    /*000047f4*/ MOVT        R2, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1164] 
    /*000047f8*/ MOV         R0, #3 
    /*000047fc*/ MOVT        R6, #40982                  @ 0xa016 
    /*00004800*/ BL          0x0000000000004800          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004804*/ MOV         R0, R6 
    /*00004808*/ ADD         SP, SP, #20 
    /*0000480c*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AoDisableDev_x4810:
    /*00004810*/ MOV         R0, R5 
    /*00004814*/ BL          0x0000000000004814          @ AO_DRV_ClrDevAttr [R_ARM_CALL: .text+0x75fc] 
    /*00004818*/ SUBS        R6, R0, #0 
    /*0000481c*/ BEQ         AoDisableDev_x4794
    /*00004820*/ MOV         R0, R7 
    /*00004824*/ BL          0x0000000000004824          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004828*/ B           AoDisableDev_x47c4
    AoDisableDev_x482c:
    /*0000482c*/ MOV         R0, R7 
    /*00004830*/ MOV         R6, R3 
    /*00004834*/ BL          0x0000000000004834          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004838*/ MOV         R0, R6 
    /*0000483c*/ ADD         SP, SP, #20 
    /*00004840*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AoDisableDev_x4844:
    /*00004844*/ MOVW        R3, #607                    @ 0x25f 
    /*00004848*/ STR         R5, [SP, #4] 
    /*0000484c*/ STR         R3, [SP] 
    /*00004850*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf4c] 
    /*00004854*/ LDR         R3, [PC, #96]               @ 0x00000000000048bc 
    /*00004858*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf4c] 
    /*0000485c*/ MOV         R1, #22 
    /*00004860*/ MOV         R0, #3 
    /*00004864*/ MOVW        R6, #32769                  @ 0x8001 
    /*00004868*/ BL          0x0000000000004868          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000486c*/ MOVT        R6, #40982                  @ 0xa016 
    /*00004870*/ B           AoDisableDev_x47c4
    AoDisableDev_x4874:
    /*00004874*/ MOVW        R3, #614                    @ 0x266 
    /*00004878*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1130] 
    /*0000487c*/ STR         R3, [SP] 
    /*00004880*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1130] 
    /*00004884*/ LDR         R3, [PC, #48]               @ 0x00000000000048bc 
    /*00004888*/ MOV         R1, #22 
    /*0000488c*/ MOV         R0, #6 
    /*00004890*/ MOVW        R6, #32777                  @ 0x8009 
    /*00004894*/ BL          0x0000000000004894          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004898*/ MOVT        R6, #40982                  @ 0xa016 
    /*0000489c*/ B           AoDisableDev_x47c4
    AoDisableDev_x48a0:
    /*000048a0*/ MOV         R6, #65024                  @ 0xfe00 
    /*000048a4*/ MOVT        R6, #65535                  @ 0xffff 
    /*000048a8*/ B           AoDisableDev_x47c4
    AoDisableDev_x48ac:
    /*000048ac*/ MOVW        R6, #32784                  @ 0x8010 
    /*000048b0*/ MOVT        R6, #40982                  @ 0xa016 
    /*000048b4*/ B           AoDisableDev_x47c4
    /*000048b8*/ ANDEQ       R0, R0, R0, LSR #2 
    /*000048bc*/ @           <UNDEFINED> instruction: 0x000001b0 
FUNC_END AoDisableDev


FUNC_BEGIN AO_DEV_Init
    /*000048c0*/ CMP         R0, #1 
    /*000048c4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*000048c8*/ MOV         R4, R0 
    /*000048cc*/ SUB         SP, SP, #8 
    /*000048d0*/ BHI         AO_DEV_Init_x49b0
    /*000048d4*/ MOV         R2, #136                    @ 0x88 
    /*000048d8*/ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*000048dc*/ MUL         R7, R2, R0 
    /*000048e0*/ MOVT        R6, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*000048e4*/ MOV         R1, #0 
    /*000048e8*/ ADD         R5, R6, R7 
    /*000048ec*/ MOV         R0, R5 
    /*000048f0*/ BL          0x00000000000048F0          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000048f4*/ MOV         R3, #0 
    /*000048f8*/ MOV         R0, R4 
    /*000048fc*/ STR         R3, [R5, #12] 
    /*00004900*/ BL          0x0000000000004900          @ AO_DRV_DevInit [R_ARM_CALL: .text+0x77c8] 
    /*00004904*/ SUBS        R8, R0, #0 
    /*00004908*/ BEQ         AO_DEV_Init_x4918
    /*0000490c*/ MOV         R0, R8 
    /*00004910*/ ADD         SP, SP, #8 
    /*00004914*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AO_DEV_Init_x4918:
    /*00004918*/ MOVW        R0, #0                      @ AODmaCallback [R_ARM_MOVW_ABS_NC: .text+0x35b0] 
    /*0000491c*/ MOVT        R0, #0                      @ AODmaCallback [R_ARM_MOVT_ABS: .text+0x35b0] 
    /*00004920*/ BL          0x0000000000004920          @ AO_DRV_SetDmaISR [R_ARM_CALL: .text+0x77b8] 
    /*00004924*/ ADD         R0, R7, #120                @ 0x78 
    /*00004928*/ ADD         R0, R6, R0 
    /*0000492c*/ ADD         R5, R4, R4, LSL #1 
    /*00004930*/ BL          0x0000000000004930          @ osal_wait_init [R_ARM_CALL: +0x0] 
    /*00004934*/ ADD         R0, R7, #124                @ 0x7c 
    /*00004938*/ ADD         R0, R6, R0 
    /*0000493c*/ MOV         R1, #1 
    /*00004940*/ BL          0x0000000000004940          @ osal_sema_init [R_ARM_CALL: +0x0] 
    /*00004944*/ ADD         R3, R6, #272                @ 0x110 
    /*00004948*/ MOVW        SL, #2892                   @ 0xb4c 
    /*0000494c*/ MOV         R9, R8 
    /*00004950*/ MLA         R5, SL, R5, R3 
    AO_DEV_Init_x4954:
    /*00004954*/ MOVW        R2, #2892                   @ 0xb4c 
    /*00004958*/ MOV         R1, #0 
    /*0000495c*/ MOV         R0, R5 
    /*00004960*/ BL          0x0000000000004960          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00004964*/ ADD         R9, R9, #1 
    /*00004968*/ MOV         R0, R5 
    /*0000496c*/ MOV         R1, #1 
    /*00004970*/ BL          0x0000000000004970          @ osal_sema_init [R_ARM_CALL: +0x0] 
    /*00004974*/ CMP         R9, #3 
    /*00004978*/ ADD         R5, R5, SL 
    /*0000497c*/ BNE         AO_DEV_Init_x4954
    /*00004980*/ ADD         R0, R7, #128                @ 0x80 
    /*00004984*/ ADD         R0, R6, R0 
    /*00004988*/ BL          0x0000000000004988          @ osal_spin_lock_init [R_ARM_CALL: +0x0] 
    /*0000498c*/ MOV         R3, #136                    @ 0x88 
    /*00004990*/ MLA         R4, R3, R4, R6 
    /*00004994*/ MOV         R2, #1 
    /*00004998*/ MOV         R3, #0 
    /*0000499c*/ MOV         R0, R8 
    /*000049a0*/ STR         R2, [R4, #4] 
    /*000049a4*/ STR         R3, [R4, #8] 
    /*000049a8*/ ADD         SP, SP, #8 
    /*000049ac*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AO_DEV_Init_x49b0:
    /*000049b0*/ STR         R0, [SP, #4] 
    /*000049b4*/ MOVW        R3, #698                    @ 0x2ba 
    /*000049b8*/ MOV         R1, #22 
    /*000049bc*/ STR         R3, [SP] 
    /*000049c0*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf4c] 
    /*000049c4*/ LDR         R3, [PC, #28]               @ 0x00000000000049e8 
    /*000049c8*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf4c] 
    /*000049cc*/ MOV         R0, #3 
    /*000049d0*/ BL          0x00000000000049D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000049d4*/ MOVW        R8, #32769                  @ 0x8001 
    /*000049d8*/ MOVT        R8, #40982                  @ 0xa016 
    /*000049dc*/ MOV         R0, R8 
    /*000049e0*/ ADD         SP, SP, #8 
    /*000049e4*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    /*000049e8*/ ANDEQ       R0, R0, R0, ASR #3 
FUNC_END AO_DEV_Init


FUNC_BEGIN AO_DEV_Exit
    /*000049ec*/ CMP         R0, #1 
    /*000049f0*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*000049f4*/ MOV         R7, R0 
    /*000049f8*/ SUB         SP, SP, #8 
    /*000049fc*/ BHI         AO_DEV_Exit_x4b10
    /*00004a00*/ MOV         R5, #136                    @ 0x88 
    /*00004a04*/ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00004a08*/ MUL         R5, R5, R0 
    /*00004a0c*/ MOVT        R6, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00004a10*/ ADD         R9, R6, R5 
    /*00004a14*/ LDR         R8, [R9, #4] 
    /*00004a18*/ CMP         R8, #0 
    /*00004a1c*/ BNE         AO_DEV_Exit_x4a2c
    AO_DEV_Exit_x4a20:
    /*00004a20*/ MOV         R0, R8 
    /*00004a24*/ ADD         SP, SP, #8 
    /*00004a28*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AO_DEV_Exit_x4a2c:
    /*00004a2c*/ BL          0x0000000000004A2C          @ AO_DRV_DevDeInit [R_ARM_CALL: .text+0x784c] 
    /*00004a30*/ SUBS        R8, R0, #0 
    /*00004a34*/ BNE         AO_DEV_Exit_x4a20
    /*00004a38*/ LDR         R3, [R9, #44]               @ 0x2c 
    /*00004a3c*/ CMP         R3, #0 
    /*00004a40*/ BEQ         AO_DEV_Exit_x4a84
    /*00004a44*/ ADD         R3, R6, #272                @ 0x110 
    /*00004a48*/ ADD         R4, R7, R7, LSL #1 
    /*00004a4c*/ MOVW        R2, #2892                   @ 0xb4c 
    /*00004a50*/ ADD         R5, R5, #120                @ 0x78 
    /*00004a54*/ MLA         R4, R2, R4, R3 
    /*00004a58*/ ADD         R5, R6, R5 
    /*00004a5c*/ MOV         SL, #3 
    AO_DEV_Exit_x4a60:
    /*00004a60*/ STR         SL, [R4, #1240]             @ 0x4d8 
    /*00004a64*/ MOV         R0, R5 
    /*00004a68*/ BL          0x0000000000004A68          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00004a6c*/ LDR         R3, [R9, #44]               @ 0x2c 
    /*00004a70*/ ADD         R8, R8, #1 
    /*00004a74*/ ADD         R4, R4, #2880               @ 0xb40 
    /*00004a78*/ CMP         R3, R8 
    /*00004a7c*/ ADD         R4, R4, #12 
    /*00004a80*/ BHI         AO_DEV_Exit_x4a60
    AO_DEV_Exit_x4a84:
    /*00004a84*/ MOV         R0, R7 
    /*00004a88*/ BL          0x0000000000004A88          @ AoDisableDev [R_ARM_CALL: .text+0x45ec] 
    /*00004a8c*/ SUBS        R8, R0, #0 
    /*00004a90*/ BNE         AO_DEV_Exit_x4a20
    /*00004a94*/ ADD         R4, R7, R7, LSL #1 
    /*00004a98*/ MOVW        R3, #2892                   @ 0xb4c 
    /*00004a9c*/ MOV         R9, R3 
    /*00004aa0*/ LDR         R5, [PC, #152]              @ 0x0000000000004b40 
    /*00004aa4*/ MUL         R4, R3, R4 
    /*00004aa8*/ MOV         R3, #136                    @ 0x88 
    /*00004aac*/ MLA         R3, R3, R7, R6 
    /*00004ab0*/ ADD         R2, R4, #8640               @ 0x21c0 
    /*00004ab4*/ ADD         R4, R5, R4 
    /*00004ab8*/ ADD         R2, R2, #36                 @ 0x24 
    /*00004abc*/ ADD         R5, R5, R2 
    /*00004ac0*/ STR         R8, [R3, #4] 
    AO_DEV_Exit_x4ac4:
    /*00004ac4*/ MOV         R0, R4 
    /*00004ac8*/ ADD         R4, R4, R9 
    /*00004acc*/ BL          0x0000000000004ACC          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*00004ad0*/ CMP         R5, R4 
    /*00004ad4*/ BNE         AO_DEV_Exit_x4ac4
    /*00004ad8*/ MOV         R0, #136                    @ 0x88 
    /*00004adc*/ MUL         R7, R0, R7 
    /*00004ae0*/ ADD         R0, R7, #120                @ 0x78 
    /*00004ae4*/ ADD         R0, R6, R0 
    /*00004ae8*/ BL          0x0000000000004AE8          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00004aec*/ ADD         R0, R7, #124                @ 0x7c 
    /*00004af0*/ ADD         R0, R6, R0 
    /*00004af4*/ ADD         R7, R7, #128                @ 0x80 
    /*00004af8*/ BL          0x0000000000004AF8          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*00004afc*/ ADD         R0, R6, R7 
    /*00004b00*/ BL          0x0000000000004B00          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00004b04*/ MOV         R0, R8 
    /*00004b08*/ ADD         SP, SP, #8 
    /*00004b0c*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AO_DEV_Exit_x4b10:
    /*00004b10*/ STR         R0, [SP, #4] 
    /*00004b14*/ MOVW        R3, #738                    @ 0x2e2 
    /*00004b18*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf4c] 
    /*00004b1c*/ STR         R3, [SP] 
    /*00004b20*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf4c] 
    /*00004b24*/ LDR         R3, [PC, #24]               @ 0x0000000000004b44 
    /*00004b28*/ MOV         R1, #22 
    /*00004b2c*/ MOV         R0, #3 
    /*00004b30*/ MOVW        R8, #32769                  @ 0x8001 
    /*00004b34*/ BL          0x0000000000004B34          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004b38*/ MOVT        R8, #40982                  @ 0xa016 
    /*00004b3c*/ B           AO_DEV_Exit_x4a20
    /*00004b40*/ ANDEQ       R0, R0, R0, LSR #2 
    /*00004b44*/ ANDEQ       R0, R0, IP, ASR #3 
FUNC_END AO_DEV_Exit


FUNC_BEGIN AO_GetChnCtx
    /*00004b48*/ CMP         R0, #5 
    /*00004b4c*/ LDRLE       R3, [PC, #12]               @ 0x0000000000004b60 
    /*00004b50*/ MOVWLE      R2, #2892                   @ 0xb4c 
    /*00004b54*/ MLALE       R0, R2, R0, R3 
    /*00004b58*/ MOVGT       R0, #0 
    /*00004b5c*/ BX          LR 
    /*00004b60*/ ANDEQ       R0, R0, R0, LSR #2 
FUNC_END AO_GetChnCtx


FUNC_BEGIN AO_GetDevCtx
    /*00004b64*/ CMP         R0, #1 
    /*00004b68*/ MOVWLS      R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00004b6c*/ MOVTLS      R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00004b70*/ MOVLS       R2, #136                    @ 0x88 
    /*00004b74*/ MLALS       R0, R2, R0, R3 
    /*00004b78*/ MOVHI       R0, #0 
    /*00004b7c*/ BX          LR 
FUNC_END AO_GetDevCtx


FUNC_BEGIN AoClrDevAttr
    /*00004b80*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00004b84*/ MOV         R7, R0 
    /*00004b88*/ SUB         SP, SP, #12 
    /*00004b8c*/ MOV         R0, #20 
    /*00004b90*/ BL          0x0000000000004B90          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00004b94*/ CMP         R0, #0 
    /*00004b98*/ BEQ         AoClrDevAttr_x4c7c
    /*00004b9c*/ CMP         R7, #1 
    /*00004ba0*/ BHI         AoClrDevAttr_x4c00
    /*00004ba4*/ MOV         R5, #136                    @ 0x88 
    /*00004ba8*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00004bac*/ MUL         R5, R5, R7 
    /*00004bb0*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00004bb4*/ ADD         R6, R5, #124                @ 0x7c 
    /*00004bb8*/ ADD         R6, R4, R6 
    /*00004bbc*/ MOV         R0, R6 
    /*00004bc0*/ BL          0x0000000000004BC0          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00004bc4*/ CMP         R0, #0 
    /*00004bc8*/ BNE         AoClrDevAttr_x4c38
    /*00004bcc*/ ADD         R4, R4, R5 
    /*00004bd0*/ LDR         R3, [R4, #12] 
    /*00004bd4*/ CMP         R3, #0 
    /*00004bd8*/ BNE         AoClrDevAttr_x4c44
    /*00004bdc*/ MOV         R0, R7 
    /*00004be0*/ BL          0x0000000000004BE0          @ AO_DRV_ClrDevAttr [R_ARM_CALL: .text+0x75fc] 
    /*00004be4*/ SUBS        R5, R0, #0 
    /*00004be8*/ MOV         R0, R6 
    /*00004bec*/ STREQ       R5, [R4, #8] 
    /*00004bf0*/ BL          0x0000000000004BF0          @ osal_up [R_ARM_CALL: +0x0] 
    AoClrDevAttr_x4bf4:
    /*00004bf4*/ MOV         R0, R5 
    /*00004bf8*/ ADD         SP, SP, #12 
    /*00004bfc*/ POP         {R4, R5, R6, R7, PC} 
    AoClrDevAttr_x4c00:
    /*00004c00*/ STR         R7, [SP, #4] 
    /*00004c04*/ MOVW        R3, #837                    @ 0x345 
    /*00004c08*/ MOV         R1, #22 
    /*00004c0c*/ STR         R3, [SP] 
    /*00004c10*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf4c] 
    /*00004c14*/ LDR         R3, [PC, #108]              @ 0x0000000000004c88 
    /*00004c18*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf4c] 
    /*00004c1c*/ MOV         R0, #3 
    /*00004c20*/ BL          0x0000000000004C20          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004c24*/ MOVW        R5, #32769                  @ 0x8001 
    /*00004c28*/ MOVT        R5, #40982                  @ 0xa016 
    /*00004c2c*/ MOV         R0, R5 
    /*00004c30*/ ADD         SP, SP, #12 
    /*00004c34*/ POP         {R4, R5, R6, R7, PC} 
    AoClrDevAttr_x4c38:
    /*00004c38*/ MOV         R5, #65024                  @ 0xfe00 
    /*00004c3c*/ MOVT        R5, #65535                  @ 0xffff 
    /*00004c40*/ B           AoClrDevAttr_x4bf4
    AoClrDevAttr_x4c44:
    /*00004c44*/ MOV         R0, R6 
    /*00004c48*/ MOVW        R5, #32777                  @ 0x8009 
    /*00004c4c*/ BL          0x0000000000004C4C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004c50*/ MOVW        R3, #851                    @ 0x353 
    /*00004c54*/ STR         R7, [SP, #4] 
    /*00004c58*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11fc] 
    /*00004c5c*/ STR         R3, [SP] 
    /*00004c60*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11fc] 
    /*00004c64*/ LDR         R3, [PC, #28]               @ 0x0000000000004c88 
    /*00004c68*/ MOV         R1, #22 
    /*00004c6c*/ MOV         R0, #3 
    /*00004c70*/ MOVT        R5, #40982                  @ 0xa016 
    /*00004c74*/ BL          0x0000000000004C74          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004c78*/ B           AoClrDevAttr_x4bf4
    AoClrDevAttr_x4c7c:
    /*00004c7c*/ MOVW        R5, #32784                  @ 0x8010 
    /*00004c80*/ MOVT        R5, #40982                  @ 0xa016 
    /*00004c84*/ B           AoClrDevAttr_x4bf4
    /*00004c88*/ LDRDEQ      R0, [R0], -R8 
FUNC_END AoClrDevAttr


FUNC_BEGIN AOProcShow
    /*00004c8c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00004c90*/ MOVW        R1, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c4] 
    /*00004c94*/ SUB         SP, SP, #116                @ 0x74 
    /*00004c98*/ MOV         R9, R0 
    /*00004c9c*/ MOVT        R1, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c4] 
    /*00004ca0*/ MOVW        FP, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13c8] 
    /*00004ca4*/ BL          0x0000000000004CA4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00004ca8*/ MOV         R0, R9 
    /*00004cac*/ BL          0x0000000000004CAC          @ AO_DRV_ProcShow [R_ARM_CALL: .text+0x7ddc] 
    /*00004cb0*/ MOV         R0, R9 
    /*00004cb4*/ MOVW        R1, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1324] 
    /*00004cb8*/ MOVT        R1, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1324] 
    /*00004cbc*/ BL          0x0000000000004CBC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00004cc0*/ MOVW        IP, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00004cc4*/ MOVT        FP, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13c8] 
    /*00004cc8*/ MOV         R4, IP 
    /*00004ccc*/ STR         FP, [SP, #28] 
    /*00004cd0*/ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13b4] 
    /*00004cd4*/ MOVW        R3, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13c0] 
    /*00004cd8*/ MOVT        R2, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13b4] 
    /*00004cdc*/ MOVT        R3, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13c0] 
    /*00004ce0*/ STR         R2, [SP, #36]               @ 0x24 
    /*00004ce4*/ STR         R3, [SP, #32] 
    /*00004ce8*/ MOVW        R2, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13d0] 
    /*00004cec*/ MOVW        R3, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13d8] 
    /*00004cf0*/ MOVT        R2, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13d0] 
    /*00004cf4*/ MOVT        R3, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13d8] 
    /*00004cf8*/ STR         R2, [SP, #24] 
    /*00004cfc*/ STR         R3, [SP, #20] 
    /*00004d00*/ MOVW        R2, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13e4] 
    /*00004d04*/ MOVW        R3, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13f0] 
    /*00004d08*/ MOVT        R2, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13e4] 
    /*00004d0c*/ MOVT        R3, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13f0] 
    /*00004d10*/ STR         R2, [SP, #16] 
    /*00004d14*/ STR         R3, [SP, #12] 
    /*00004d18*/ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13f8] 
    /*00004d1c*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1400] 
    /*00004d20*/ MOVT        R2, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13f8] 
    /*00004d24*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1400] 
    /*00004d28*/ STR         R2, [SP, #8] 
    /*00004d2c*/ STR         R3, [SP, #4] 
    /*00004d30*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1408] 
    /*00004d34*/ MOV         R0, R9 
    /*00004d38*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1408] 
    /*00004d3c*/ MOVW        R3, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1378] 
    /*00004d40*/ STR         R2, [SP, #72]               @ 0x48 
    /*00004d44*/ STR         R2, [SP] 
    /*00004d48*/ MOVT        R3, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1378] 
    /*00004d4c*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1380] 
    /*00004d50*/ MOVW        R1, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1388] 
    /*00004d54*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1380] 
    /*00004d58*/ MOVT        R1, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1388] 
    /*00004d5c*/ BL          0x0000000000004D5C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00004d60*/ MOV         IP, R4 
    /*00004d64*/ MOVT        IP, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00004d68*/ MOV         R7, #0 
    /*00004d6c*/ ADD         R3, IP, #272                @ 0x110 
    /*00004d70*/ STR         FP, [SP, #80]               @ 0x50 
    /*00004d74*/ MOV         FP, R7 
    /*00004d78*/ MOV         R7, IP 
    /*00004d7c*/ MOV         R8, R3 
    /*00004d80*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x123c] 
    /*00004d84*/ MOVW        R1, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1244] 
    /*00004d88*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x123c] 
    /*00004d8c*/ MOVT        R1, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1244] 
    /*00004d90*/ STR         IP, [SP, #92]               @ 0x5c 
    /*00004d94*/ STR         R3, [SP, #68]               @ 0x44 
    /*00004d98*/ STR         R2, [SP, #60]               @ 0x3c 
    /*00004d9c*/ STR         R1, [SP, #64]               @ 0x40 
    /*00004da0*/ STR         R9, [SP, #48]               @ 0x30 
    AOProcShow_x4da4:
    /*00004da4*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00004da8*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00004dac*/ MOV         R3, #136                    @ 0x88 
    /*00004db0*/ MLA         R3, R3, FP, R2 
    /*00004db4*/ LDR         R3, [R3, #12] 
    /*00004db8*/ CMP         R3, #0 
    /*00004dbc*/ BEQ         AOProcShow_x4f68
    /*00004dc0*/ LDR         R2, [R7, #44]               @ 0x2c 
    /*00004dc4*/ CMP         R2, #0 
    /*00004dc8*/ BEQ         AOProcShow_x4f68
    /*00004dcc*/ MOVW        R9, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1410] 
    /*00004dd0*/ MOV         R4, R8 
    /*00004dd4*/ MOVT        R9, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1410] 
    /*00004dd8*/ MOV         R5, #0 
    /*00004ddc*/ STR         R8, [SP, #84]               @ 0x54 
    /*00004de0*/ STR         R9, [SP, #76]               @ 0x4c 
    /*00004de4*/ B           AOProcShow_x4dfc
    AOProcShow_x4de8:
    /*00004de8*/ ADD         R5, R5, #1 
    /*00004dec*/ ADD         R4, R4, #2880               @ 0xb40 
    /*00004df0*/ CMP         R2, R5 
    /*00004df4*/ ADD         R4, R4, #12 
    /*00004df8*/ BLS         AOProcShow_x4f64
    AOProcShow_x4dfc:
    /*00004dfc*/ LDR         R3, [R4, #1240]             @ 0x4d8 
    /*00004e00*/ SUB         R1, R3, #1 
    /*00004e04*/ CMP         R1, #1 
    /*00004e08*/ BHI         AOProcShow_x4de8
    /*00004e0c*/ LDR         R2, [R4, #8] 
    /*00004e10*/ CMP         R3, #1 
    /*00004e14*/ LDR         R1, [SP, #64]               @ 0x40 
    /*00004e18*/ LDR         SL, [SP, #60]               @ 0x3c 
    /*00004e1c*/ LDRH        IP, [R2] 
    /*00004e20*/ LDRH        LR, [R2, #2] 
    /*00004e24*/ MOVEQ       SL, R1 
    /*00004e28*/ ADD         IP, IP, #8 
    /*00004e2c*/ LDR         R3, [R4, #2356]             @ 0x934 
    /*00004e30*/ ADD         LR, LR, #8 
    /*00004e34*/ LDR         R1, [R4, #1228]             @ 0x4cc 
    /*00004e38*/ UXTH        IP, IP 
    /*00004e3c*/ LDR         R6, [R4, #1224]             @ 0x4c8 
    /*00004e40*/ CMP         IP, #16 
    /*00004e44*/ UXTH        LR, LR 
    /*00004e48*/ STR         R1, [SP, #52]               @ 0x34 
    /*00004e4c*/ MOVLS       IP, #0 
    /*00004e50*/ MOVHI       IP, #1 
    /*00004e54*/ CMP         LR, #16 
    /*00004e58*/ LDR         R8, [R4, #1248]             @ 0x4e0 
    /*00004e5c*/ MOVLS       LR, #0 
    /*00004e60*/ MOVHI       LR, #1 
    /*00004e64*/ CMP         R3, #1 
    /*00004e68*/ BEQ         AOProcShow_x5480
    /*00004e6c*/ CMP         R3, #0 
    /*00004e70*/ MOVW        R0, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1250] 
    /*00004e74*/ MOVT        R0, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1250] 
    /*00004e78*/ MOVNE       R0, #0 
    AOProcShow_x4e7c:
    /*00004e7c*/ LDR         R3, [R4, #2364]             @ 0x93c 
    /*00004e80*/ LDR         R2, [R4, #2360]             @ 0x938 
    /*00004e84*/ CMP         R3, #8000                   @ 0x1f40 
    /*00004e88*/ STR         R2, [SP, #56]               @ 0x38 
    /*00004e8c*/ BEQ         AOProcShow_x5550
    /*00004e90*/ MOVW        R2, #11025                  @ 0x2b11 
    /*00004e94*/ CMP         R3, R2 
    /*00004e98*/ BEQ         AOProcShow_x5600
    /*00004e9c*/ MOVW        R2, #12000                  @ 0x2ee0 
    /*00004ea0*/ CMP         R3, R2 
    /*00004ea4*/ BEQ         AOProcShow_x5630
    /*00004ea8*/ CMP         R3, #16000                  @ 0x3e80 
    /*00004eac*/ BEQ         AOProcShow_x5cac
    /*00004eb0*/ MOVW        R2, #22050                  @ 0x5622 
    /*00004eb4*/ CMP         R3, R2 
    /*00004eb8*/ BEQ         AOProcShow_x5cd0
    /*00004ebc*/ MOVW        R2, #24000                  @ 0x5dc0 
    /*00004ec0*/ CMP         R3, R2 
    /*00004ec4*/ BEQ         AOProcShow_x5c04
    /*00004ec8*/ CMP         R3, #32000                  @ 0x7d00 
    /*00004ecc*/ BEQ         AOProcShow_x5d00
    /*00004ed0*/ MOVW        R2, #44100                  @ 0xac44 
    /*00004ed4*/ CMP         R3, R2 
    /*00004ed8*/ BEQ         AOProcShow_x5d40
    /*00004edc*/ MOVW        R2, #48000                  @ 0xbb80 
    /*00004ee0*/ CMP         R3, R2 
    /*00004ee4*/ BEQ         AOProcShow_x5d70
    /*00004ee8*/ CMP         R3, #64000                  @ 0xfa00 
    /*00004eec*/ BEQ         AOProcShow_x5d88
    /*00004ef0*/ MOV         R9, #30464                  @ 0x7700 
    /*00004ef4*/ MOVT        R9, #1 
    /*00004ef8*/ CMP         R3, R9 
    /*00004efc*/ LDR         R3, [R4, #2368]             @ 0x940 
    /*00004f00*/ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x129c] 
    /*00004f04*/ MOVT        R2, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x129c] 
    /*00004f08*/ MOVNE       R2, #0 
    /*00004f0c*/ CMP         R3, #8000                   @ 0x1f40 
    /*00004f10*/ BNE         AOProcShow_x5564
    AOProcShow_x4f14:
    /*00004f14*/ MOVW        R3, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1254] 
    /*00004f18*/ MOVT        R3, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1254] 
    AOProcShow_x4f1c:
    /*00004f1c*/ LDR         R1, [SP, #56]               @ 0x38 
    /*00004f20*/ STR         IP, [SP, #16] 
    /*00004f24*/ LDR         IP, [SP, #52]               @ 0x34 
    /*00004f28*/ STR         R3, [SP, #36]               @ 0x24 
    /*00004f2c*/ MOV         R3, R5 
    /*00004f30*/ STR         R2, [SP, #32] 
    /*00004f34*/ MOV         R2, FP 
    /*00004f38*/ STR         R1, [SP, #28] 
    /*00004f3c*/ STR         R0, [SP, #24] 
    /*00004f40*/ LDR         R1, [SP, #76]               @ 0x4c 
    /*00004f44*/ LDR         R0, [SP, #48]               @ 0x30 
    /*00004f48*/ STR         LR, [SP, #20] 
    /*00004f4c*/ STR         R8, [SP, #12] 
    /*00004f50*/ STR         R6, [SP, #8] 
    /*00004f54*/ STM         SP, {SL, IP} 
    /*00004f58*/ BL          0x0000000000004F58          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00004f5c*/ LDR         R2, [R7, #44]               @ 0x2c 
    /*00004f60*/ B           AOProcShow_x4de8
    AOProcShow_x4f64:
    /*00004f64*/ LDR         R8, [SP, #84]               @ 0x54 
    AOProcShow_x4f68:
    /*00004f68*/ ADD         FP, FP, #1 
    /*00004f6c*/ ADD         R8, R8, #8640               @ 0x21c0 
    /*00004f70*/ CMP         FP, #2 
    /*00004f74*/ ADD         R8, R8, #36                 @ 0x24 
    /*00004f78*/ ADD         R7, R7, #136                @ 0x88 
    /*00004f7c*/ BNE         AOProcShow_x4da4
    /*00004f80*/ LDR         R9, [SP, #48]               @ 0x30 
    /*00004f84*/ MOVW        R3, #0                      @ .LC61 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14c0] 
    /*00004f88*/ MOV         R4, R3 
    /*00004f8c*/ MOVW        R1, #0                      @ .LC58 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x143c] 
    /*00004f90*/ MOVT        R1, #0                      @ .LC58 [R_ARM_MOVT_ABS: .rodata.str1.4+0x143c] 
    /*00004f94*/ LDR         FP, [SP, #80]               @ 0x50 
    /*00004f98*/ MOV         R0, R9 
    /*00004f9c*/ MOV         R8, #0 
    /*00004fa0*/ BL          0x0000000000004FA0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00004fa4*/ MOV         R3, R4 
    /*00004fa8*/ MOVT        R3, #0                      @ .LC61 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14c0] 
    /*00004fac*/ LDR         R1, [SP, #72]               @ 0x48 
    /*00004fb0*/ STR         R3, [SP, #96]               @ 0x60 
    /*00004fb4*/ MOV         R0, R9 
    /*00004fb8*/ MOV         R2, R3 
    /*00004fbc*/ STR         R2, [SP, #32] 
    /*00004fc0*/ MOVW        R2, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14c8] 
    /*00004fc4*/ MOVT        R2, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14c8] 
    /*00004fc8*/ STR         R2, [SP, #100]              @ 0x64 
    /*00004fcc*/ MOVW        R3, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14b8] 
    /*00004fd0*/ STR         R2, [SP, #28] 
    /*00004fd4*/ MOVW        R2, #0                      @ .LC64 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14d4] 
    /*00004fd8*/ MOVT        R2, #0                      @ .LC64 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14d4] 
    /*00004fdc*/ STR         R1, [SP] 
    /*00004fe0*/ MOVT        R3, #0                      @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14b8] 
    /*00004fe4*/ STR         R2, [SP, #108]              @ 0x6c 
    /*00004fe8*/ STR         R3, [SP, #36]               @ 0x24 
    /*00004fec*/ MOVW        R1, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1490] 
    /*00004ff0*/ STR         FP, [SP, #16] 
    /*00004ff4*/ MOVW        R3, #0                      @ .LC63 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14cc] 
    /*00004ff8*/ MOVT        R1, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1490] 
    /*00004ffc*/ MOVT        R3, #0                      @ .LC63 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14cc] 
    /*00005000*/ STR         R3, [SP, #104]              @ 0x68 
    /*00005004*/ MOV         R7, R8 
    /*00005008*/ STR         R3, [SP, #24] 
    /*0000500c*/ MOV         R3, R2 
    /*00005010*/ MOVW        R2, #0                      @ .LC65 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14dc] 
    /*00005014*/ STR         R3, [SP, #20] 
    /*00005018*/ MOVT        R2, #0                      @ .LC65 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14dc] 
    /*0000501c*/ MOVW        R3, #0                      @ .LC66 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14e4] 
    /*00005020*/ STR         R2, [SP, #12] 
    /*00005024*/ MOVT        R3, #0                      @ .LC66 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14e4] 
    /*00005028*/ MOVW        R2, #0                      @ .LC67 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14ec] 
    /*0000502c*/ STR         R3, [SP, #8] 
    /*00005030*/ MOVT        R2, #0                      @ .LC67 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14ec] 
    /*00005034*/ MOVW        R3, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1378] 
    /*00005038*/ STR         R2, [SP, #4] 
    /*0000503c*/ MOVT        R3, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1378] 
    /*00005040*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1380] 
    /*00005044*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1380] 
    /*00005048*/ BL          0x0000000000005048          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000504c*/ MOVW        R3, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1244] 
    /*00005050*/ MOVT        R3, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1244] 
    /*00005054*/ STR         R3, [SP, #76]               @ 0x4c 
    /*00005058*/ LDR         R3, [SP, #68]               @ 0x44 
    /*0000505c*/ MOVW        R2, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14f4] 
    /*00005060*/ STR         R8, [SP, #64]               @ 0x40 
    /*00005064*/ MOVT        R2, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14f4] 
    /*00005068*/ STR         R8, [SP, #60]               @ 0x3c 
    /*0000506c*/ MOV         R6, R3 
    /*00005070*/ STR         R2, [SP, #80]               @ 0x50 
    /*00005074*/ STR         R8, [SP, #48]               @ 0x30 
    /*00005078*/ STR         R8, [SP, #52]               @ 0x34 
    /*0000507c*/ STR         R8, [SP, #56]               @ 0x38 
    AOProcShow_x5080:
    /*00005080*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00005084*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00005088*/ MOV         R3, #136                    @ 0x88 
    /*0000508c*/ MLA         R8, R3, R7, R2 
    /*00005090*/ LDR         R3, [R8, #12] 
    /*00005094*/ CMP         R3, #0 
    /*00005098*/ BEQ         AOProcShow_x52a4
    /*0000509c*/ LDR         R2, [R8, #44]               @ 0x2c 
    /*000050a0*/ CMP         R2, #0 
    /*000050a4*/ BEQ         AOProcShow_x52a4
    /*000050a8*/ MOVW        R3, #0                      @ .LC69 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1500] 
    /*000050ac*/ MOV         R4, R6 
    /*000050b0*/ MOVT        R3, #0                      @ .LC69 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1500] 
    /*000050b4*/ STR         R6, [SP, #88]               @ 0x58 
    /*000050b8*/ STR         R3, [SP, #84]               @ 0x54 
    /*000050bc*/ MOV         R5, #0 
    /*000050c0*/ MOV         R6, R8 
    /*000050c4*/ B           AOProcShow_x50dc
    AOProcShow_x50c8:
    /*000050c8*/ ADD         R5, R5, #1 
    /*000050cc*/ ADD         R4, R4, #2880               @ 0xb40 
    /*000050d0*/ CMP         R2, R5 
    /*000050d4*/ ADD         R4, R4, #12 
    /*000050d8*/ BLS         AOProcShow_x52a0
    AOProcShow_x50dc:
    /*000050dc*/ LDR         R3, [R4, #1240]             @ 0x4d8 
    /*000050e0*/ CMP         R3, #1 
    /*000050e4*/ BNE         AOProcShow_x50c8
    /*000050e8*/ LDR         R1, [R4, #1268]             @ 0x4f4 
    /*000050ec*/ MOV         R3, R5 
    /*000050f0*/ LDR         LR, [SP, #52]               @ 0x34 
    /*000050f4*/ MOV         R2, R7 
    /*000050f8*/ LDR         R0, [R4, #1276]             @ 0x4fc 
    /*000050fc*/ CMP         R1, #0 
    /*00005100*/ LDR         R1, [SP, #76]               @ 0x4c 
    /*00005104*/ LDR         IP, [R4, #1280]             @ 0x500 
    /*00005108*/ MOVNE       LR, #1 
    /*0000510c*/ CMP         R0, #0 
    /*00005110*/ STR         LR, [SP, #52]               @ 0x34 
    /*00005114*/ LDR         LR, [SP, #48]               @ 0x30 
    /*00005118*/ STR         R1, [SP] 
    /*0000511c*/ MOVNE       LR, #1 
    /*00005120*/ LDR         R0, [R4, #1284]             @ 0x504 
    /*00005124*/ CMP         IP, #0 
    /*00005128*/ STR         LR, [SP, #48]               @ 0x30 
    /*0000512c*/ LDR         LR, [SP, #60]               @ 0x3c 
    /*00005130*/ MOVNE       LR, #1 
    /*00005134*/ LDR         IP, [R4, #1272]             @ 0x4f8 
    /*00005138*/ CMP         R0, #0 
    /*0000513c*/ LDR         R1, [SP, #80]               @ 0x50 
    /*00005140*/ MOV         R0, R9 
    /*00005144*/ STR         LR, [SP, #60]               @ 0x3c 
    /*00005148*/ LDR         LR, [SP, #56]               @ 0x38 
    /*0000514c*/ MOVNE       LR, #1 
    /*00005150*/ CMP         IP, #0 
    /*00005154*/ LDR         IP, [SP, #64]               @ 0x40 
    /*00005158*/ STR         LR, [SP, #56]               @ 0x38 
    /*0000515c*/ MOVNE       IP, #1 
    /*00005160*/ STR         IP, [SP, #64]               @ 0x40 
    /*00005164*/ BL          0x0000000000005164          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005168*/ LDR         R3, [R4, #1268]             @ 0x4f4 
    /*0000516c*/ CMP         R3, #1 
    /*00005170*/ BEQ         AOProcShow_x5474
    /*00005174*/ CMP         R3, #0 
    /*00005178*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1250] 
    /*0000517c*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1250] 
    /*00005180*/ MOVNE       R2, #0 
    AOProcShow_x5184:
    /*00005184*/ LDR         R3, [R4, #1308]             @ 0x51c 
    /*00005188*/ CMP         R3, #0 
    /*0000518c*/ MOVWEQ      R3, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12ac] 
    /*00005190*/ MOVTEQ      R3, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12ac] 
    /*00005194*/ BEQ         AOProcShow_x51b0
    /*00005198*/ CMP         R3, #1 
    /*0000519c*/ BEQ         AOProcShow_x560c
    /*000051a0*/ CMP         R3, #2 
    /*000051a4*/ MOVW        R3, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12bc] 
    /*000051a8*/ MOVT        R3, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12bc] 
    /*000051ac*/ MOVNE       R3, #0 
    AOProcShow_x51b0:
    /*000051b0*/ LDR         R1, [R4, #1296]             @ 0x510 
    /*000051b4*/ CMP         R1, #8000                   @ 0x1f40 
    /*000051b8*/ BEQ         AOProcShow_x548c
    /*000051bc*/ MOVW        R0, #11025                  @ 0x2b11 
    /*000051c0*/ CMP         R1, R0 
    /*000051c4*/ BEQ         AOProcShow_x55e8
    /*000051c8*/ MOVW        R0, #12000                  @ 0x2ee0 
    /*000051cc*/ CMP         R1, R0 
    /*000051d0*/ BEQ         AOProcShow_x5618
    /*000051d4*/ CMP         R1, #16000                  @ 0x3e80 
    /*000051d8*/ BEQ         AOProcShow_x5ca0
    /*000051dc*/ MOVW        R0, #22050                  @ 0x5622 
    /*000051e0*/ CMP         R1, R0 
    /*000051e4*/ BEQ         AOProcShow_x5cc4
    /*000051e8*/ MOVW        R0, #24000                  @ 0x5dc0 
    /*000051ec*/ CMP         R1, R0 
    /*000051f0*/ BEQ         AOProcShow_x5bf8
    /*000051f4*/ CMP         R1, #32000                  @ 0x7d00 
    /*000051f8*/ BEQ         AOProcShow_x5cf4
    /*000051fc*/ MOVW        R0, #44100                  @ 0xac44 
    /*00005200*/ CMP         R1, R0 
    /*00005204*/ BEQ         AOProcShow_x5d34
    /*00005208*/ MOVW        R0, #48000                  @ 0xbb80 
    /*0000520c*/ CMP         R1, R0 
    /*00005210*/ BEQ         AOProcShow_x5d58
    /*00005214*/ CMP         R1, #64000                  @ 0xfa00 
    /*00005218*/ BEQ         AOProcShow_x5d7c
    /*0000521c*/ MOV         R0, #30464                  @ 0x7700 
    /*00005220*/ MOVT        R0, #1 
    /*00005224*/ CMP         R1, R0 
    /*00005228*/ LDR         R1, [R4, #1276]             @ 0x4fc 
    /*0000522c*/ MOVW        LR, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x129c] 
    /*00005230*/ MOVT        LR, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x129c] 
    /*00005234*/ MOVNE       LR, #0 
    /*00005238*/ CMP         R1, #1 
    /*0000523c*/ LDR         R8, [R4, #1304]             @ 0x518 
    /*00005240*/ BNE         AOProcShow_x54a4
    AOProcShow_x5244:
    /*00005244*/ LDR         R1, [R4, #1280]             @ 0x500 
    /*00005248*/ MOVW        IP, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x124c] 
    /*0000524c*/ MOVT        IP, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x124c] 
    /*00005250*/ CMP         R1, #1 
    /*00005254*/ BNE         AOProcShow_x54c0
    AOProcShow_x5258:
    /*00005258*/ LDR         R1, [R4, #1284]             @ 0x504 
    /*0000525c*/ MOVW        R0, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x124c] 
    /*00005260*/ MOVT        R0, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x124c] 
    /*00005264*/ CMP         R1, #1 
    /*00005268*/ BNE         AOProcShow_x54dc
    AOProcShow_x526c:
    /*0000526c*/ LDR         SL, [R4, #1272]             @ 0x4f8 
    /*00005270*/ MOVW        R1, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x124c] 
    /*00005274*/ MOVT        R1, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x124c] 
    /*00005278*/ CMP         SL, #1 
    /*0000527c*/ BNE         AOProcShow_x54f8
    AOProcShow_x5280:
    /*00005280*/ LDR         FP, [R4, #2372]             @ 0x944 
    /*00005284*/ MOVW        SL, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x124c] 
    /*00005288*/ MOVT        SL, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x124c] 
    /*0000528c*/ CMP         FP, #1 
    /*00005290*/ BNE         AOProcShow_x5514
    AOProcShow_x5294:
    /*00005294*/ MOVW        FP, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x124c] 
    /*00005298*/ MOVT        FP, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x124c] 
    /*0000529c*/ B           AOProcShow_x5524
    AOProcShow_x52a0:
    /*000052a0*/ LDR         R6, [SP, #88]               @ 0x58 
    AOProcShow_x52a4:
    /*000052a4*/ ADD         R7, R7, #1 
    /*000052a8*/ ADD         R6, R6, #8640               @ 0x21c0 
    /*000052ac*/ CMP         R7, #2 
    /*000052b0*/ ADD         R6, R6, #36                 @ 0x24 
    /*000052b4*/ BNE         AOProcShow_x5080
    /*000052b8*/ LDR         R3, [SP, #52]               @ 0x34 
    /*000052bc*/ LDR         R8, [SP, #56]               @ 0x38 
    /*000052c0*/ CMP         R3, #0 
    /*000052c4*/ BEQ         AOProcShow_x52f4
    /*000052c8*/ LDR         R3, [SP, #48]               @ 0x30 
    /*000052cc*/ CMP         R3, #0 
    /*000052d0*/ BNE         AOProcShow_x563c
    /*000052d4*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*000052d8*/ CMP         R3, #0 
    /*000052dc*/ BNE         AOProcShow_x57c4
    AOProcShow_x52e0:
    /*000052e0*/ CMP         R8, #0 
    /*000052e4*/ BNE         AOProcShow_x59e4
    AOProcShow_x52e8:
    /*000052e8*/ LDR         R3, [SP, #64]               @ 0x40 
    /*000052ec*/ CMP         R3, #0 
    /*000052f0*/ BNE         AOProcShow_x5300
    AOProcShow_x52f4:
    /*000052f4*/ MOV         R0, #0 
    /*000052f8*/ ADD         SP, SP, #116                @ 0x74 
    /*000052fc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOProcShow_x5300:
    /*00005300*/ LDR         R3, [SP, #48]               @ 0x30 
    /*00005304*/ MOVW        R1, #0                      @ .LC70 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1520] 
    /*00005308*/ MOV         R0, R9 
    /*0000530c*/ MOVT        R1, #0                      @ .LC70 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1520] 
    /*00005310*/ ADD         R2, R3, #1 
    /*00005314*/ MOVW        SL, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1244] 
    /*00005318*/ BL          0x0000000000005318          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000531c*/ LDR         R3, [SP, #96]               @ 0x60 
    /*00005320*/ MOVW        R2, #0                      @ .LC100 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1714] 
    /*00005324*/ MOVW        R1, #0                      @ .LC99 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1700] 
    /*00005328*/ MOVT        R2, #0                      @ .LC100 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1714] 
    /*0000532c*/ MOV         R0, R9 
    /*00005330*/ STR         R3, [SP, #4] 
    /*00005334*/ MOVT        R1, #0                      @ .LC99 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1700] 
    /*00005338*/ LDR         R3, [SP, #72]               @ 0x48 
    /*0000533c*/ MOVT        SL, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1244] 
    /*00005340*/ STR         R2, [SP, #12] 
    /*00005344*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1380] 
    /*00005348*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1380] 
    /*0000534c*/ MOVW        FP, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14f4] 
    /*00005350*/ STR         R3, [SP] 
    /*00005354*/ MOVW        R3, #0                      @ .LC75 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15b4] 
    /*00005358*/ MOVT        R3, #0                      @ .LC75 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15b4] 
    /*0000535c*/ STR         R3, [SP, #8] 
    /*00005360*/ MOVW        R3, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1378] 
    /*00005364*/ MOVT        R3, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1378] 
    /*00005368*/ BL          0x0000000000005368          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000536c*/ MOVT        FP, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14f4] 
    /*00005370*/ STR         SL, [SP, #48]               @ 0x30 
    /*00005374*/ MOVW        R7, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1250] 
    /*00005378*/ STR         FP, [SP, #52]               @ 0x34 
    /*0000537c*/ MOVW        R8, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x124c] 
    /*00005380*/ LDR         SL, [SP, #68]               @ 0x44 
    /*00005384*/ MOVT        R7, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1250] 
    /*00005388*/ LDR         FP, [SP, #92]               @ 0x5c 
    /*0000538c*/ MOVT        R8, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x124c] 
    /*00005390*/ MOV         R6, #0 
    AOProcShow_x5394:
    /*00005394*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00005398*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*0000539c*/ MOV         R3, #136                    @ 0x88 
    /*000053a0*/ MLA         R3, R3, R6, R2 
    /*000053a4*/ LDR         R3, [R3, #12] 
    /*000053a8*/ CMP         R3, #0 
    /*000053ac*/ BEQ         AOProcShow_x5450
    /*000053b0*/ LDR         R2, [FP, #44]               @ 0x2c 
    /*000053b4*/ CMP         R2, #0 
    /*000053b8*/ BEQ         AOProcShow_x5450
    /*000053bc*/ MOV         R4, SL 
    /*000053c0*/ MOV         R5, #0 
    /*000053c4*/ B           AOProcShow_x53dc
    AOProcShow_x53c8:
    /*000053c8*/ ADD         R5, R5, #1 
    /*000053cc*/ ADD         R4, R4, #2880               @ 0xb40 
    /*000053d0*/ CMP         R2, R5 
    /*000053d4*/ ADD         R4, R4, #12 
    /*000053d8*/ BLS         AOProcShow_x5450
    AOProcShow_x53dc:
    /*000053dc*/ LDR         R3, [R4, #1240]             @ 0x4d8 
    /*000053e0*/ CMP         R3, #1 
    /*000053e4*/ BNE         AOProcShow_x53c8
    /*000053e8*/ LDR         R3, [SP, #48]               @ 0x30 
    /*000053ec*/ MOV         R2, R6 
    /*000053f0*/ LDR         R1, [SP, #52]               @ 0x34 
    /*000053f4*/ MOV         R0, R9 
    /*000053f8*/ STR         R3, [SP] 
    /*000053fc*/ MOV         R3, R5 
    /*00005400*/ BL          0x0000000000005400          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005404*/ LDR         R3, [R4, #1272]             @ 0x4f8 
    /*00005408*/ CMP         R3, #1 
    /*0000540c*/ MOVEQ       R2, R8 
    /*00005410*/ BEQ         AOProcShow_x5420
    /*00005414*/ CMP         R3, #0 
    /*00005418*/ MOVEQ       R2, R7 
    /*0000541c*/ MOVNE       R2, #0 
    AOProcShow_x5420:
    /*00005420*/ LDR         R3, [R4, #1312]             @ 0x520 
    /*00005424*/ MOVW        R1, #0                      @ .LC101 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x171c] 
    /*00005428*/ LDR         IP, [R4, #1316]             @ 0x524 
    /*0000542c*/ MOVT        R1, #0                      @ .LC101 [R_ARM_MOVT_ABS: .rodata.str1.4+0x171c] 
    /*00005430*/ CMP         R3, #1 
    /*00005434*/ MOV         R0, R9 
    /*00005438*/ STR         IP, [SP] 
    /*0000543c*/ MOVNE       R3, R7 
    /*00005440*/ MOVEQ       R3, R8 
    /*00005444*/ BL          0x0000000000005444          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005448*/ LDR         R2, [FP, #44]               @ 0x2c 
    /*0000544c*/ B           AOProcShow_x53c8
    AOProcShow_x5450:
    /*00005450*/ ADD         R6, R6, #1 
    /*00005454*/ ADD         SL, SL, #8640               @ 0x21c0 
    /*00005458*/ CMP         R6, #2 
    /*0000545c*/ ADD         SL, SL, #36                 @ 0x24 
    /*00005460*/ ADD         FP, FP, #136                @ 0x88 
    /*00005464*/ BNE         AOProcShow_x5394
    /*00005468*/ MOV         R0, #0 
    /*0000546c*/ ADD         SP, SP, #116                @ 0x74 
    /*00005470*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AOProcShow_x5474:
    /*00005474*/ MOVW        R2, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x124c] 
    /*00005478*/ MOVT        R2, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x124c] 
    /*0000547c*/ B           AOProcShow_x5184
    AOProcShow_x5480:
    /*00005480*/ MOVW        R0, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x124c] 
    /*00005484*/ MOVT        R0, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x124c] 
    /*00005488*/ B           AOProcShow_x4e7c
    AOProcShow_x548c:
    /*0000548c*/ MOVW        LR, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1254] 
    /*00005490*/ MOVT        LR, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1254] 
    AOProcShow_x5494:
    /*00005494*/ LDR         R1, [R4, #1276]             @ 0x4fc 
    /*00005498*/ LDR         R8, [R4, #1304]             @ 0x518 
    /*0000549c*/ CMP         R1, #1 
    /*000054a0*/ BEQ         AOProcShow_x5244
    AOProcShow_x54a4:
    /*000054a4*/ CMP         R1, #0 
    /*000054a8*/ LDR         R1, [R4, #1280]             @ 0x500 
    /*000054ac*/ MOVW        IP, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1250] 
    /*000054b0*/ MOVT        IP, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1250] 
    /*000054b4*/ MOVNE       IP, #0 
    /*000054b8*/ CMP         R1, #1 
    /*000054bc*/ BEQ         AOProcShow_x5258
    AOProcShow_x54c0:
    /*000054c0*/ CMP         R1, #0 
    /*000054c4*/ LDR         R1, [R4, #1284]             @ 0x504 
    /*000054c8*/ MOVW        R0, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1250] 
    /*000054cc*/ MOVT        R0, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1250] 
    /*000054d0*/ MOVNE       R0, #0 
    /*000054d4*/ CMP         R1, #1 
    /*000054d8*/ BEQ         AOProcShow_x526c
    AOProcShow_x54dc:
    /*000054dc*/ CMP         R1, #0 
    /*000054e0*/ LDR         SL, [R4, #1272]             @ 0x4f8 
    /*000054e4*/ MOVW        R1, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1250] 
    /*000054e8*/ MOVT        R1, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1250] 
    /*000054ec*/ MOVNE       R1, #0 
    /*000054f0*/ CMP         SL, #1 
    /*000054f4*/ BEQ         AOProcShow_x5280
    AOProcShow_x54f8:
    /*000054f8*/ CMP         SL, #0 
    /*000054fc*/ LDR         FP, [R4, #2372]             @ 0x944 
    /*00005500*/ MOVW        SL, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1250] 
    /*00005504*/ MOVT        SL, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1250] 
    /*00005508*/ MOVNE       SL, #0 
    /*0000550c*/ CMP         FP, #1 
    /*00005510*/ BEQ         AOProcShow_x5294
    AOProcShow_x5514:
    /*00005514*/ CMP         FP, #0 
    /*00005518*/ MOVW        FP, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1250] 
    /*0000551c*/ MOVT        FP, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1250] 
    /*00005520*/ MOVNE       FP, #0 
    AOProcShow_x5524:
    /*00005524*/ STR         R1, [SP, #16] 
    /*00005528*/ STR         R0, [SP, #12] 
    /*0000552c*/ MOV         R0, R9 
    /*00005530*/ STR         FP, [SP, #24] 
    /*00005534*/ STR         SL, [SP, #20] 
    /*00005538*/ LDR         R1, [SP, #84]               @ 0x54 
    /*0000553c*/ STMIB       SP, {R8, IP} 
    /*00005540*/ STR         LR, [SP] 
    /*00005544*/ BL          0x0000000000005544          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005548*/ LDR         R2, [R6, #44]               @ 0x2c 
    /*0000554c*/ B           AOProcShow_x50c8
    AOProcShow_x5550:
    /*00005550*/ MOVW        R2, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1254] 
    /*00005554*/ MOVT        R2, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1254] 
    AOProcShow_x5558:
    /*00005558*/ LDR         R3, [R4, #2368]             @ 0x940 
    /*0000555c*/ CMP         R3, #8000                   @ 0x1f40 
    /*00005560*/ BEQ         AOProcShow_x4f14
    AOProcShow_x5564:
    /*00005564*/ MOVW        R9, #11025                  @ 0x2b11 
    /*00005568*/ CMP         R3, R9 
    /*0000556c*/ BEQ         AOProcShow_x55f4
    /*00005570*/ MOVW        R9, #12000                  @ 0x2ee0 
    /*00005574*/ CMP         R3, R9 
    /*00005578*/ BEQ         AOProcShow_x5624
    /*0000557c*/ CMP         R3, #16000                  @ 0x3e80 
    /*00005580*/ BEQ         AOProcShow_x5cb8
    /*00005584*/ MOVW        R9, #22050                  @ 0x5622 
    /*00005588*/ CMP         R3, R9 
    /*0000558c*/ BEQ         AOProcShow_x5cdc
    /*00005590*/ MOVW        R9, #24000                  @ 0x5dc0 
    /*00005594*/ CMP         R3, R9 
    /*00005598*/ BEQ         AOProcShow_x5ce8
    /*0000559c*/ CMP         R3, #32000                  @ 0x7d00 
    /*000055a0*/ BEQ         AOProcShow_x5d0c
    /*000055a4*/ MOVW        R9, #44100                  @ 0xac44 
    /*000055a8*/ CMP         R3, R9 
    /*000055ac*/ BEQ         AOProcShow_x5d4c
    /*000055b0*/ MOVW        R9, #48000                  @ 0xbb80 
    /*000055b4*/ CMP         R3, R9 
    /*000055b8*/ BEQ         AOProcShow_x5d64
    /*000055bc*/ CMP         R3, #64000                  @ 0xfa00 
    /*000055c0*/ BEQ         AOProcShow_x5d94
    /*000055c4*/ MOV         R9, #30464                  @ 0x7700 
    /*000055c8*/ MOVT        R9, #1 
    /*000055cc*/ MOV         R1, R9 
    /*000055d0*/ CMP         R3, R1 
    /*000055d4*/ MOVW        R9, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x129c] 
    /*000055d8*/ MOVT        R9, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x129c] 
    /*000055dc*/ MOVNE       R9, #0 
    /*000055e0*/ MOV         R3, R9 
    /*000055e4*/ B           AOProcShow_x4f1c
    AOProcShow_x55e8:
    /*000055e8*/ MOVW        LR, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x125c] 
    /*000055ec*/ MOVT        LR, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x125c] 
    /*000055f0*/ B           AOProcShow_x5494
    AOProcShow_x55f4:
    /*000055f4*/ MOVW        R3, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x125c] 
    /*000055f8*/ MOVT        R3, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x125c] 
    /*000055fc*/ B           AOProcShow_x4f1c
    AOProcShow_x5600:
    /*00005600*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x125c] 
    /*00005604*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x125c] 
    /*00005608*/ B           AOProcShow_x5558
    AOProcShow_x560c:
    /*0000560c*/ MOVW        R3, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12b4] 
    /*00005610*/ MOVT        R3, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12b4] 
    /*00005614*/ B           AOProcShow_x51b0
    AOProcShow_x5618:
    /*00005618*/ MOVW        LR, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1264] 
    /*0000561c*/ MOVT        LR, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1264] 
    /*00005620*/ B           AOProcShow_x5494
    AOProcShow_x5624:
    /*00005624*/ MOVW        R3, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1264] 
    /*00005628*/ MOVT        R3, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1264] 
    /*0000562c*/ B           AOProcShow_x4f1c
    AOProcShow_x5630:
    /*00005630*/ MOVW        R2, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1264] 
    /*00005634*/ MOVT        R2, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1264] 
    /*00005638*/ B           AOProcShow_x5558
    AOProcShow_x563c:
    /*0000563c*/ MOV         R2, #1 
    /*00005640*/ MOVW        R1, #0                      @ .LC70 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1520] 
    /*00005644*/ MOV         R0, R9 
    /*00005648*/ MOVT        R1, #0                      @ .LC70 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1520] 
    /*0000564c*/ BL          0x000000000000564C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005650*/ LDR         R3, [SP, #108]              @ 0x6c 
    /*00005654*/ MOVW        R2, #0                      @ .LC72 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1590] 
    /*00005658*/ MOVW        R1, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1574] 
    /*0000565c*/ MOVT        R2, #0                      @ .LC72 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1590] 
    /*00005660*/ MOV         R0, R9 
    /*00005664*/ STR         R3, [SP, #4] 
    /*00005668*/ MOVT        R1, #0                      @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1574] 
    /*0000566c*/ LDR         R3, [SP, #72]               @ 0x48 
    /*00005670*/ MOVW        SL, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00005674*/ STR         R2, [SP, #20] 
    /*00005678*/ MOVW        R2, #0                      @ .LC74 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*0000567c*/ MOVT        R2, #0                      @ .LC74 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00005680*/ STR         R2, [SP, #12] 
    /*00005684*/ STR         R3, [SP] 
    /*00005688*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1380] 
    /*0000568c*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x159c] 
    /*00005690*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1380] 
    /*00005694*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x159c] 
    /*00005698*/ STR         R3, [SP, #16] 
    /*0000569c*/ MOVW        R3, #0                      @ .LC75 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15b4] 
    /*000056a0*/ MOVT        R3, #0                      @ .LC75 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15b4] 
    /*000056a4*/ STR         R3, [SP, #8] 
    /*000056a8*/ MOVW        R3, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1378] 
    /*000056ac*/ MOVT        R3, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1378] 
    /*000056b0*/ MOVW        FP, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1250] 
    /*000056b4*/ BL          0x00000000000056B4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000056b8*/ LDR         R7, [SP, #68]               @ 0x44 
    /*000056bc*/ MOVT        SL, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*000056c0*/ MOVT        FP, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1250] 
    /*000056c4*/ MOV         R6, #0 
    /*000056c8*/ MOVW        R3, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1244] 
    /*000056cc*/ MOVT        R3, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1244] 
    /*000056d0*/ STR         R3, [SP, #52]               @ 0x34 
    /*000056d4*/ MOVW        R3, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14f4] 
    /*000056d8*/ MOVT        R3, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14f4] 
    /*000056dc*/ STR         R3, [SP, #56]               @ 0x38 
    /*000056e0*/ MOVW        R3, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x124c] 
    /*000056e4*/ MOVT        R3, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x124c] 
    /*000056e8*/ STR         R3, [SP, #48]               @ 0x30 
    AOProcShow_x56ec:
    /*000056ec*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*000056f0*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*000056f4*/ MOV         R3, #136                    @ 0x88 
    /*000056f8*/ MLA         R3, R3, R6, R2 
    /*000056fc*/ LDR         R3, [R3, #12] 
    /*00005700*/ CMP         R3, #0 
    /*00005704*/ BEQ         AOProcShow_x5c70
    /*00005708*/ LDR         R2, [SL, #44]               @ 0x2c 
    /*0000570c*/ CMP         R2, #0 
    /*00005710*/ BEQ         AOProcShow_x5c70
    /*00005714*/ MOV         R4, R7 
    /*00005718*/ MOV         R5, #0 
    /*0000571c*/ B           AOProcShow_x5734
    AOProcShow_x5720:
    /*00005720*/ ADD         R5, R5, #1 
    /*00005724*/ ADD         R4, R4, #2880               @ 0xb40 
    /*00005728*/ CMP         R2, R5 
    /*0000572c*/ ADD         R4, R4, #12 
    /*00005730*/ BLS         AOProcShow_x5c70
    AOProcShow_x5734:
    /*00005734*/ LDR         R3, [R4, #1240]             @ 0x4d8 
    /*00005738*/ CMP         R3, #1 
    /*0000573c*/ BNE         AOProcShow_x5720
    /*00005740*/ LDR         R3, [SP, #52]               @ 0x34 
    /*00005744*/ MOV         R2, R6 
    /*00005748*/ LDR         R1, [SP, #56]               @ 0x38 
    /*0000574c*/ MOV         R0, R9 
    /*00005750*/ STR         R3, [SP] 
    /*00005754*/ MOV         R3, R5 
    /*00005758*/ BL          0x0000000000005758          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000575c*/ LDR         R3, [R4, #1276]             @ 0x4fc 
    /*00005760*/ CMP         R3, #1 
    /*00005764*/ BEQ         AOProcShow_x5d18
    /*00005768*/ CMP         R3, #0 
    /*0000576c*/ MOVEQ       R2, FP 
    /*00005770*/ MOVNE       R2, #0 
    AOProcShow_x5774:
    /*00005774*/ MOVW        R0, #1326                   @ 0x52e 
    /*00005778*/ ADD         R3, R4, #1328               @ 0x530 
    /*0000577c*/ LDRSH       R0, [R4, R0] 
    /*00005780*/ MOVW        R1, #1324                   @ 0x52c 
    /*00005784*/ LDRSH       IP, [R4, R1] 
    /*00005788*/ MOVW        R1, #0                      @ .LC76 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15bc] 
    /*0000578c*/ LDRSB       LR, [R3] 
    /*00005790*/ MOVT        R1, #0                      @ .LC76 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15bc] 
    /*00005794*/ STR         R0, [SP, #4] 
    /*00005798*/ MOV         R0, R9 
    /*0000579c*/ LDR         R3, [R4, #1320]             @ 0x528 
    /*000057a0*/ STR         IP, [SP] 
    /*000057a4*/ CMP         R3, #1 
    /*000057a8*/ LDR         IP, [SP, #48]               @ 0x30 
    /*000057ac*/ STR         LR, [SP, #8] 
    /*000057b0*/ MOVNE       IP, FP 
    /*000057b4*/ MOV         R3, IP 
    /*000057b8*/ BL          0x00000000000057B8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000057bc*/ LDR         R2, [SL, #44]               @ 0x2c 
    /*000057c0*/ B           AOProcShow_x5720
    AOProcShow_x57c4:
    /*000057c4*/ LDR         R3, [SP, #48]               @ 0x30 
    /*000057c8*/ MOVW        R1, #0                      @ .LC70 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1520] 
    /*000057cc*/ MOV         R0, R9 
    /*000057d0*/ MOVT        R1, #0                      @ .LC70 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1520] 
    /*000057d4*/ ADD         R3, R3, #1 
    /*000057d8*/ STR         R3, [SP, #48]               @ 0x30 
    /*000057dc*/ MOVW        R7, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1250] 
    /*000057e0*/ MOVT        R7, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1250] 
    /*000057e4*/ MOV         R2, R3 
    /*000057e8*/ MOVW        FP, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*000057ec*/ BL          0x00000000000057EC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000057f0*/ LDR         R3, [SP, #104]              @ 0x68 
    /*000057f4*/ MOVW        R2, #0                      @ .LC78 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1600] 
    /*000057f8*/ MOVW        R1, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15d0] 
    /*000057fc*/ MOVT        R2, #0                      @ .LC78 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1600] 
    /*00005800*/ MOVT        R1, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15d0] 
    /*00005804*/ STR         R3, [SP, #4] 
    /*00005808*/ MOV         R0, R9 
    /*0000580c*/ LDR         R3, [SP, #72]               @ 0x48 
    /*00005810*/ MOVT        FP, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00005814*/ STR         R2, [SP, #40]               @ 0x28 
    /*00005818*/ MOVW        R2, #0                      @ .LC80 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1614] 
    /*0000581c*/ MOVT        R2, #0                      @ .LC80 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1614] 
    /*00005820*/ STR         R2, [SP, #32] 
    /*00005824*/ STR         R3, [SP] 
    /*00005828*/ MOVW        R2, #0                      @ .LC82 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x162c] 
    /*0000582c*/ MOVW        R3, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1608] 
    /*00005830*/ MOVT        R2, #0                      @ .LC82 [R_ARM_MOVT_ABS: .rodata.str1.4+0x162c] 
    /*00005834*/ MOVT        R3, #0                      @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1608] 
    /*00005838*/ STR         R2, [SP, #24] 
    /*0000583c*/ STR         R3, [SP, #36]               @ 0x24 
    /*00005840*/ MOVW        R2, #0                      @ .LC84 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1640] 
    /*00005844*/ MOVW        R3, #0                      @ .LC81 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1620] 
    /*00005848*/ MOVT        R2, #0                      @ .LC84 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1640] 
    /*0000584c*/ MOVT        R3, #0                      @ .LC81 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1620] 
    /*00005850*/ STR         R2, [SP, #16] 
    /*00005854*/ STR         R3, [SP, #28] 
    /*00005858*/ MOVW        R2, #0                      @ .LC75 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15b4] 
    /*0000585c*/ MOVW        R3, #0                      @ .LC83 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1634] 
    /*00005860*/ MOVT        R2, #0                      @ .LC75 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15b4] 
    /*00005864*/ MOVT        R3, #0                      @ .LC83 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1634] 
    /*00005868*/ STR         R2, [SP, #8] 
    /*0000586c*/ STR         R3, [SP, #20] 
    /*00005870*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1380] 
    /*00005874*/ MOVW        R3, #0                      @ .LC85 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x164c] 
    /*00005878*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1380] 
    /*0000587c*/ MOVT        R3, #0                      @ .LC85 [R_ARM_MOVT_ABS: .rodata.str1.4+0x164c] 
    /*00005880*/ STR         R3, [SP, #12] 
    /*00005884*/ MOVW        R3, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1378] 
    /*00005888*/ MOVT        R3, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1378] 
    /*0000588c*/ BL          0x000000000000588C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005890*/ LDR         SL, [SP, #68]               @ 0x44 
    /*00005894*/ STR         R8, [SP, #80]               @ 0x50 
    /*00005898*/ MOV         R6, #0 
    /*0000589c*/ MOV         R8, R7 
    /*000058a0*/ MOV         R7, R9 
    /*000058a4*/ MOVW        R3, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1244] 
    /*000058a8*/ MOVW        R2, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14f4] 
    /*000058ac*/ MOVT        R3, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1244] 
    /*000058b0*/ MOVT        R2, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14f4] 
    /*000058b4*/ STR         R3, [SP, #60]               @ 0x3c 
    /*000058b8*/ MOVW        R3, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x124c] 
    /*000058bc*/ STR         R2, [SP, #76]               @ 0x4c 
    /*000058c0*/ MOVT        R3, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x124c] 
    /*000058c4*/ STR         R3, [SP, #56]               @ 0x38 
    AOProcShow_x58c8:
    /*000058c8*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*000058cc*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*000058d0*/ MOV         R3, #136                    @ 0x88 
    /*000058d4*/ MLA         R3, R3, R6, R2 
    /*000058d8*/ LDR         R3, [R3, #12] 
    /*000058dc*/ CMP         R3, #0 
    /*000058e0*/ BEQ         AOProcShow_x5c44
    /*000058e4*/ LDR         R2, [FP, #44]               @ 0x2c 
    /*000058e8*/ CMP         R2, #0 
    /*000058ec*/ BEQ         AOProcShow_x5c44
    /*000058f0*/ MOV         R4, SL 
    /*000058f4*/ MOV         R5, #0 
    /*000058f8*/ STR         R6, [SP, #52]               @ 0x34 
    /*000058fc*/ STR         SL, [SP, #84]               @ 0x54 
    /*00005900*/ B           AOProcShow_x5918
    AOProcShow_x5904:
    /*00005904*/ ADD         R5, R5, #1 
    /*00005908*/ ADD         R4, R4, #2880               @ 0xb40 
    /*0000590c*/ CMP         R2, R5 
    /*00005910*/ ADD         R4, R4, #12 
    /*00005914*/ BLS         AOProcShow_x5c3c
    AOProcShow_x5918:
    /*00005918*/ LDR         R3, [R4, #1240]             @ 0x4d8 
    /*0000591c*/ CMP         R3, #1 
    /*00005920*/ BNE         AOProcShow_x5904
    /*00005924*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*00005928*/ MOV         R0, R7 
    /*0000592c*/ LDR         R2, [SP, #52]               @ 0x34 
    /*00005930*/ LDR         R1, [SP, #76]               @ 0x4c 
    /*00005934*/ STR         R3, [SP] 
    /*00005938*/ MOV         R3, R5 
    /*0000593c*/ BL          0x000000000000593C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005940*/ LDR         R3, [R4, #1280]             @ 0x500 
    /*00005944*/ CMP         R3, #1 
    /*00005948*/ BEQ         AOProcShow_x5d20
    /*0000594c*/ CMP         R3, #0 
    /*00005950*/ MOVEQ       R2, R8 
    /*00005954*/ MOVNE       R2, #0 
    AOProcShow_x5958:
    /*00005958*/ ADD         R1, R4, #1344               @ 0x540 
    /*0000595c*/ MOVW        R3, #1340                   @ 0x53c 
    /*00005960*/ LDRSB       R6, [R4, R3] 
    /*00005964*/ MOVW        IP, #1345                   @ 0x541 
    /*00005968*/ LDRSB       SL, [R1] 
    /*0000596c*/ MOVW        R0, #1346                   @ 0x542 
    /*00005970*/ MOVW        R1, #1341                   @ 0x53d 
    /*00005974*/ MOVW        R3, #1342                   @ 0x53e 
    /*00005978*/ LDRSB       R9, [R4, IP] 
    /*0000597c*/ MOVW        IP, #1348                   @ 0x544 
    /*00005980*/ LDRSB       LR, [R4, R0] 
    /*00005984*/ MOVW        R0, #1343                   @ 0x53f 
    /*00005988*/ LDRSB       R1, [R4, R1] 
    /*0000598c*/ LDRSB       R3, [R4, R3] 
    /*00005990*/ LDRSH       IP, [R4, IP] 
    /*00005994*/ LDRSB       R0, [R4, R0] 
    /*00005998*/ STR         R1, [SP, #16] 
    /*0000599c*/ MOVW        R1, #0                      @ .LC86 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1658] 
    /*000059a0*/ STR         SL, [SP, #8] 
    /*000059a4*/ MOVT        R1, #0                      @ .LC86 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1658] 
    /*000059a8*/ STR         R9, [SP, #28] 
    /*000059ac*/ STR         R6, [SP, #24] 
    /*000059b0*/ STR         LR, [SP, #20] 
    /*000059b4*/ STR         R3, [SP, #12] 
    /*000059b8*/ LDR         R3, [R4, #1336]             @ 0x538 
    /*000059bc*/ STR         IP, [SP] 
    /*000059c0*/ CMP         R3, #1 
    /*000059c4*/ LDR         IP, [SP, #56]               @ 0x38 
    /*000059c8*/ STR         R0, [SP, #4] 
    /*000059cc*/ MOV         R0, R7 
    /*000059d0*/ MOVNE       IP, R8 
    /*000059d4*/ MOV         R3, IP 
    /*000059d8*/ BL          0x00000000000059D8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000059dc*/ LDR         R2, [FP, #44]               @ 0x2c 
    /*000059e0*/ B           AOProcShow_x5904
    AOProcShow_x59e4:
    /*000059e4*/ LDR         R3, [SP, #48]               @ 0x30 
    /*000059e8*/ MOVW        R1, #0                      @ .LC70 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1520] 
    /*000059ec*/ MOV         R0, R9 
    /*000059f0*/ MOVT        R1, #0                      @ .LC70 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1520] 
    /*000059f4*/ ADD         R3, R3, #1 
    /*000059f8*/ STR         R3, [SP, #48]               @ 0x30 
    /*000059fc*/ MOVW        SL, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00005a00*/ MOVT        SL, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00005a04*/ MOV         R2, R3 
    /*00005a08*/ MOV         R6, #0 
    /*00005a0c*/ BL          0x0000000000005A0C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005a10*/ LDR         R3, [SP, #100]              @ 0x64 
    /*00005a14*/ MOVW        R2, #0                      @ .LC88 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ac] 
    /*00005a18*/ MOVW        R1, #0                      @ .LC87 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1680] 
    /*00005a1c*/ MOVT        R2, #0                      @ .LC88 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ac] 
    /*00005a20*/ MOV         R0, R9 
    /*00005a24*/ STR         R3, [SP, #4] 
    /*00005a28*/ MOVT        R1, #0                      @ .LC87 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1680] 
    /*00005a2c*/ LDR         R3, [SP, #72]               @ 0x48 
    /*00005a30*/ MOVW        FP, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1244] 
    /*00005a34*/ STR         R2, [SP, #44]               @ 0x2c 
    /*00005a38*/ MOVW        R2, #0                      @ .LC90 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16b4] 
    /*00005a3c*/ MOVT        R2, #0                      @ .LC90 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16b4] 
    /*00005a40*/ STR         R2, [SP, #36]               @ 0x24 
    /*00005a44*/ STR         R3, [SP] 
    /*00005a48*/ MOVW        R2, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c4] 
    /*00005a4c*/ MOVW        R3, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16b0] 
    /*00005a50*/ MOVT        R2, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c4] 
    /*00005a54*/ MOVT        R3, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16b0] 
    /*00005a58*/ STR         R2, [SP, #28] 
    /*00005a5c*/ STR         R3, [SP, #40]               @ 0x28 
    /*00005a60*/ MOVW        R2, #0                      @ .LC94 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16cc] 
    /*00005a64*/ MOVW        R3, #0                      @ .LC91 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16bc] 
    /*00005a68*/ MOVT        R2, #0                      @ .LC94 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16cc] 
    /*00005a6c*/ MOVT        R3, #0                      @ .LC91 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16bc] 
    /*00005a70*/ STR         R2, [SP, #20] 
    /*00005a74*/ STR         R3, [SP, #32] 
    /*00005a78*/ MOVW        R2, #0                      @ .LC96 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16d4] 
    /*00005a7c*/ MOVW        R3, #0                      @ .LC93 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c8] 
    /*00005a80*/ MOVT        R2, #0                      @ .LC96 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16d4] 
    /*00005a84*/ MOVT        R3, #0                      @ .LC93 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c8] 
    /*00005a88*/ STR         R2, [SP, #12] 
    /*00005a8c*/ STR         R3, [SP, #24] 
    /*00005a90*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1380] 
    /*00005a94*/ MOVW        R3, #0                      @ .LC95 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16d0] 
    /*00005a98*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1380] 
    /*00005a9c*/ MOVT        R3, #0                      @ .LC95 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16d0] 
    /*00005aa0*/ STR         R3, [SP, #16] 
    /*00005aa4*/ MOVW        R3, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16d8] 
    /*00005aa8*/ MOVT        R3, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16d8] 
    /*00005aac*/ STR         R3, [SP, #8] 
    /*00005ab0*/ MOVW        R3, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1378] 
    /*00005ab4*/ MOVT        R3, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1378] 
    /*00005ab8*/ MOVT        FP, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1244] 
    /*00005abc*/ BL          0x0000000000005ABC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005ac0*/ LDR         R7, [SP, #68]               @ 0x44 
    /*00005ac4*/ MOVW        R3, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14f4] 
    /*00005ac8*/ STR         FP, [SP, #56]               @ 0x38 
    /*00005acc*/ MOVT        R3, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14f4] 
    /*00005ad0*/ STR         R3, [SP, #52]               @ 0x34 
    AOProcShow_x5ad4:
    /*00005ad4*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x10] 
    /*00005ad8*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x10] 
    /*00005adc*/ MOV         R3, #136                    @ 0x88 
    /*00005ae0*/ MLA         R3, R3, R6, R2 
    /*00005ae4*/ LDR         R3, [R3, #12] 
    /*00005ae8*/ CMP         R3, #0 
    /*00005aec*/ BEQ         AOProcShow_x5c14
    /*00005af0*/ LDR         R2, [SL, #44]               @ 0x2c 
    /*00005af4*/ CMP         R2, #0 
    /*00005af8*/ BEQ         AOProcShow_x5c14
    /*00005afc*/ MOVW        FP, #0                      @ .LC98 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16dc] 
    /*00005b00*/ MOV         R4, R7 
    /*00005b04*/ MOVT        FP, #0                      @ .LC98 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16dc] 
    /*00005b08*/ MOV         R5, #0 
    /*00005b0c*/ STR         R7, [SP, #76]               @ 0x4c 
    /*00005b10*/ STR         FP, [SP, #60]               @ 0x3c 
    /*00005b14*/ B           AOProcShow_x5b2c
    AOProcShow_x5b18:
    /*00005b18*/ ADD         R5, R5, #1 
    /*00005b1c*/ ADD         R4, R4, #2880               @ 0xb40 
    /*00005b20*/ CMP         R2, R5 
    /*00005b24*/ ADD         R4, R4, #12 
    /*00005b28*/ BLS         AOProcShow_x5c10
    AOProcShow_x5b2c:
    /*00005b2c*/ LDR         R3, [R4, #1240]             @ 0x4d8 
    /*00005b30*/ CMP         R3, #1 
    /*00005b34*/ BNE         AOProcShow_x5b18
    /*00005b38*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00005b3c*/ MOV         R2, R6 
    /*00005b40*/ LDR         R1, [SP, #52]               @ 0x34 
    /*00005b44*/ MOV         R0, R9 
    /*00005b48*/ STR         R3, [SP] 
    /*00005b4c*/ MOV         R3, R5 
    /*00005b50*/ BL          0x0000000000005B50          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005b54*/ LDR         R3, [R4, #1284]             @ 0x504 
    /*00005b58*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1250] 
    /*00005b5c*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1250] 
    /*00005b60*/ CMP         R3, #1 
    /*00005b64*/ BEQ         AOProcShow_x5d28
    /*00005b68*/ CMP         R3, #0 
    /*00005b6c*/ MOVNE       R2, #0 
    AOProcShow_x5b70:
    /*00005b70*/ ADD         R0, R4, #1360               @ 0x550 
    /*00005b74*/ MOVW        R1, #1365                   @ 0x555 
    /*00005b78*/ LDRSB       IP, [R4, R1] 
    /*00005b7c*/ MOVW        R7, #1364                   @ 0x554 
    /*00005b80*/ LDRSB       R0, [R0] 
    /*00005b84*/ MOVW        R1, #1362                   @ 0x552 
    /*00005b88*/ LDRSB       FP, [R4, R7] 
    /*00005b8c*/ MOVW        LR, #1363                   @ 0x553 
    /*00005b90*/ LDRSB       R7, [R4, R1] 
    /*00005b94*/ MOVW        R1, #1361                   @ 0x551 
    /*00005b98*/ LDRSB       R8, [R4, LR] 
    /*00005b9c*/ MOVW        R3, #1356                   @ 0x54c 
    /*00005ba0*/ LDRSB       LR, [R4, R1] 
    /*00005ba4*/ MOVW        R1, #1359                   @ 0x54f 
    /*00005ba8*/ LDRSB       R1, [R4, R1] 
    /*00005bac*/ LDRSB       R3, [R4, R3] 
    /*00005bb0*/ STR         R0, [SP, #12] 
    /*00005bb4*/ MOVW        R0, #1358                   @ 0x54e 
    /*00005bb8*/ LDRSB       R0, [R4, R0] 
    /*00005bbc*/ STR         IP, [SP, #32] 
    /*00005bc0*/ MOVW        IP, #1357                   @ 0x54d 
    /*00005bc4*/ LDRSB       IP, [R4, IP] 
    /*00005bc8*/ STR         R1, [SP, #8] 
    /*00005bcc*/ STR         R0, [SP, #4] 
    /*00005bd0*/ MOV         R0, R9 
    /*00005bd4*/ STR         FP, [SP, #28] 
    /*00005bd8*/ STR         R8, [SP, #24] 
    /*00005bdc*/ STR         R7, [SP, #20] 
    /*00005be0*/ STR         LR, [SP, #16] 
    /*00005be4*/ LDR         R1, [SP, #60]               @ 0x3c 
    /*00005be8*/ STR         IP, [SP] 
    /*00005bec*/ BL          0x0000000000005BEC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005bf0*/ LDR         R2, [SL, #44]               @ 0x2c 
    /*00005bf4*/ B           AOProcShow_x5b18
    AOProcShow_x5bf8:
    /*00005bf8*/ MOVW        LR, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x127c] 
    /*00005bfc*/ MOVT        LR, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x127c] 
    /*00005c00*/ B           AOProcShow_x5494
    AOProcShow_x5c04:
    /*00005c04*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x127c] 
    /*00005c08*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x127c] 
    /*00005c0c*/ B           AOProcShow_x5558
    AOProcShow_x5c10:
    /*00005c10*/ LDR         R7, [SP, #76]               @ 0x4c 
    AOProcShow_x5c14:
    /*00005c14*/ ADD         R6, R6, #1 
    /*00005c18*/ ADD         R7, R7, #8640               @ 0x21c0 
    /*00005c1c*/ CMP         R6, #2 
    /*00005c20*/ ADD         R7, R7, #36                 @ 0x24 
    /*00005c24*/ ADD         SL, SL, #136                @ 0x88 
    /*00005c28*/ BNE         AOProcShow_x5ad4
    /*00005c2c*/ LDR         R3, [SP, #64]               @ 0x40 
    /*00005c30*/ CMP         R3, #0 
    /*00005c34*/ BEQ         AOProcShow_x52f4
    /*00005c38*/ B           AOProcShow_x5300
    AOProcShow_x5c3c:
    /*00005c3c*/ LDR         R6, [SP, #52]               @ 0x34 
    /*00005c40*/ LDR         SL, [SP, #84]               @ 0x54 
    AOProcShow_x5c44:
    /*00005c44*/ ADD         R6, R6, #1 
    /*00005c48*/ ADD         SL, SL, #8640               @ 0x21c0 
    /*00005c4c*/ CMP         R6, #2 
    /*00005c50*/ ADD         SL, SL, #36                 @ 0x24 
    /*00005c54*/ ADD         FP, FP, #136                @ 0x88 
    /*00005c58*/ BNE         AOProcShow_x58c8
    /*00005c5c*/ LDR         R8, [SP, #80]               @ 0x50 
    /*00005c60*/ MOV         R9, R7 
    /*00005c64*/ CMP         R8, #0 
    /*00005c68*/ BEQ         AOProcShow_x52e8
    /*00005c6c*/ B           AOProcShow_x59e4
    AOProcShow_x5c70:
    /*00005c70*/ ADD         R6, R6, #1 
    /*00005c74*/ ADD         R7, R7, #8640               @ 0x21c0 
    /*00005c78*/ CMP         R6, #2 
    /*00005c7c*/ ADD         R7, R7, #36                 @ 0x24 
    /*00005c80*/ ADD         SL, SL, #136                @ 0x88 
    /*00005c84*/ BNE         AOProcShow_x56ec
    /*00005c88*/ MOV         R3, #1 
    /*00005c8c*/ STR         R3, [SP, #48]               @ 0x30 
    /*00005c90*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*00005c94*/ CMP         R3, #0 
    /*00005c98*/ BEQ         AOProcShow_x52e0
    /*00005c9c*/ B           AOProcShow_x57c4
    AOProcShow_x5ca0:
    /*00005ca0*/ MOVW        LR, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x126c] 
    /*00005ca4*/ MOVT        LR, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x126c] 
    /*00005ca8*/ B           AOProcShow_x5494
    AOProcShow_x5cac:
    /*00005cac*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x126c] 
    /*00005cb0*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x126c] 
    /*00005cb4*/ B           AOProcShow_x5558
    AOProcShow_x5cb8:
    /*00005cb8*/ MOVW        R3, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x126c] 
    /*00005cbc*/ MOVT        R3, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x126c] 
    /*00005cc0*/ B           AOProcShow_x4f1c
    AOProcShow_x5cc4:
    /*00005cc4*/ MOVW        LR, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1274] 
    /*00005cc8*/ MOVT        LR, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1274] 
    /*00005ccc*/ B           AOProcShow_x5494
    AOProcShow_x5cd0:
    /*00005cd0*/ MOVW        R2, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1274] 
    /*00005cd4*/ MOVT        R2, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1274] 
    /*00005cd8*/ B           AOProcShow_x5558
    AOProcShow_x5cdc:
    /*00005cdc*/ MOVW        R3, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1274] 
    /*00005ce0*/ MOVT        R3, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1274] 
    /*00005ce4*/ B           AOProcShow_x4f1c
    AOProcShow_x5ce8:
    /*00005ce8*/ MOVW        R3, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x127c] 
    /*00005cec*/ MOVT        R3, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x127c] 
    /*00005cf0*/ B           AOProcShow_x4f1c
    AOProcShow_x5cf4:
    /*00005cf4*/ MOVW        LR, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1284] 
    /*00005cf8*/ MOVT        LR, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1284] 
    /*00005cfc*/ B           AOProcShow_x5494
    AOProcShow_x5d00:
    /*00005d00*/ MOVW        R2, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1284] 
    /*00005d04*/ MOVT        R2, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1284] 
    /*00005d08*/ B           AOProcShow_x5558
    AOProcShow_x5d0c:
    /*00005d0c*/ MOVW        R3, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1284] 
    /*00005d10*/ MOVT        R3, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1284] 
    /*00005d14*/ B           AOProcShow_x4f1c
    AOProcShow_x5d18:
    /*00005d18*/ LDR         R2, [SP, #48]               @ 0x30 
    /*00005d1c*/ B           AOProcShow_x5774
    AOProcShow_x5d20:
    /*00005d20*/ LDR         R2, [SP, #56]               @ 0x38 
    /*00005d24*/ B           AOProcShow_x5958
    AOProcShow_x5d28:
    /*00005d28*/ MOVW        R2, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x124c] 
    /*00005d2c*/ MOVT        R2, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x124c] 
    /*00005d30*/ B           AOProcShow_x5b70
    AOProcShow_x5d34:
    /*00005d34*/ MOVW        LR, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x128c] 
    /*00005d38*/ MOVT        LR, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x128c] 
    /*00005d3c*/ B           AOProcShow_x5494
    AOProcShow_x5d40:
    /*00005d40*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x128c] 
    /*00005d44*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x128c] 
    /*00005d48*/ B           AOProcShow_x5558
    AOProcShow_x5d4c:
    /*00005d4c*/ MOVW        R3, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x128c] 
    /*00005d50*/ MOVT        R3, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x128c] 
    /*00005d54*/ B           AOProcShow_x4f1c
    AOProcShow_x5d58:
    /*00005d58*/ MOVW        LR, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1294] 
    /*00005d5c*/ MOVT        LR, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1294] 
    /*00005d60*/ B           AOProcShow_x5494
    AOProcShow_x5d64:
    /*00005d64*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1294] 
    /*00005d68*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1294] 
    /*00005d6c*/ B           AOProcShow_x4f1c
    AOProcShow_x5d70:
    /*00005d70*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1294] 
    /*00005d74*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1294] 
    /*00005d78*/ B           AOProcShow_x5558
    AOProcShow_x5d7c:
    /*00005d7c*/ MOVW        LR, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12a4] 
    /*00005d80*/ MOVT        LR, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12a4] 
    /*00005d84*/ B           AOProcShow_x5494
    AOProcShow_x5d88:
    /*00005d88*/ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12a4] 
    /*00005d8c*/ MOVT        R2, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12a4] 
    /*00005d90*/ B           AOProcShow_x5558
    AOProcShow_x5d94:
    /*00005d94*/ MOVW        R3, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12a4] 
    /*00005d98*/ MOVT        R3, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12a4] 
    /*00005d9c*/ B           AOProcShow_x4f1c
FUNC_END AOProcShow


FUNC_BEGIN AO_DRV_ISR
    /*00005da0*/ PUSH        {R4, R5, R6, LR} 
    /*00005da4*/ MOV         R4, R0 
    /*00005da8*/ SUB         SP, SP, #40                 @ 0x28 
    /*00005dac*/ MOV         R0, #20 
    /*00005db0*/ BL          0x0000000000005DB0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00005db4*/ SUBS        R6, R0, #0 
    /*00005db8*/ BEQ         AO_DRV_ISR_x5fd8
    /*00005dbc*/ LDR         R3, [R6, #120]              @ 0x78 
    /*00005dc0*/ ADD         R1, SP, #20 
    /*00005dc4*/ MOV         R0, R4 
    /*00005dc8*/ BLX         R3 
    /*00005dcc*/ LDR         R3, [SP, #20] 
    /*00005dd0*/ TST         R3, #24 
    /*00005dd4*/ BNE         AO_DRV_ISR_x5f40
    AO_DRV_ISR_x5dd8:
    /*00005dd8*/ LDR         R3, [R6, #124]              @ 0x7c 
    /*00005ddc*/ MOV         R0, R4 
    /*00005de0*/ BLX         R3 
    /*00005de4*/ LDR         R3, [SP, #20] 
    /*00005de8*/ TST         R3, #3 
    /*00005dec*/ BEQ         AO_DRV_ISR_x5f38
    /*00005df0*/ TST         R3, #1 
    /*00005df4*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00005df8*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00005dfc*/ BEQ         AO_DRV_ISR_x5f90
    /*00005e00*/ MOV         R3, #184                    @ 0xb8 
    /*00005e04*/ MLA         R3, R3, R4, R5 
    /*00005e08*/ LDR         R2, [R3, #132]              @ 0x84 
    /*00005e0c*/ ADD         R2, R2, #1 
    /*00005e10*/ STR         R2, [R3, #132]              @ 0x84 
    AO_DRV_ISR_x5e14:
    /*00005e14*/ MOV         R3, #184                    @ 0xb8 
    /*00005e18*/ MOV         R0, #2 
    /*00005e1c*/ MUL         R3, R3, R4 
    /*00005e20*/ ADD         R2, R5, R3 
    /*00005e24*/ LDR         R3, [R5, R3] 
    /*00005e28*/ LDR         R1, [R2, #24] 
    /*00005e2c*/ CMP         R1, R3 
    /*00005e30*/ MOVEQ       R3, #0 
    /*00005e34*/ STREQ       R3, [R2, #24] 
    /*00005e38*/ BL          0x0000000000005E38          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00005e3c*/ LDR         R3, [R0] 
    /*00005e40*/ BLX         R3 
    /*00005e44*/ MOV         R3, #184                    @ 0xb8 
    /*00005e48*/ MLA         R3, R3, R4, R5 
    /*00005e4c*/ LDR         IP, [R3, #132]              @ 0x84 
    /*00005e50*/ LDR         R2, [R3, #80]               @ 0x50 
    /*00005e54*/ CMP         IP, #1 
    /*00005e58*/ STR         R0, [R3, #80]               @ 0x50 
    /*00005e5c*/ SUB         R0, R0, R2 
    /*00005e60*/ STR         R1, [R3, #84]               @ 0x54 
    /*00005e64*/ STR         R0, [R3, #88]               @ 0x58 
    /*00005e68*/ BEQ         AO_DRV_ISR_x5e78
    /*00005e6c*/ LDR         R2, [R3, #92]               @ 0x5c 
    /*00005e70*/ CMP         R0, R2 
    /*00005e74*/ STRHI       R0, [R3, #92]               @ 0x5c 
    AO_DRV_ISR_x5e78:
    /*00005e78*/ ADD         R0, SP, #24 
    /*00005e7c*/ BL          0x0000000000005E7C          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00005e80*/ LDR         R3, [R5, #368]              @ 0x170 
    /*00005e84*/ MOV         R0, R4 
    /*00005e88*/ BLX         R3 
    /*00005e8c*/ ADD         R0, SP, #32 
    /*00005e90*/ BL          0x0000000000005E90          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00005e94*/ LDR         IP, [SP, #28] 
    /*00005e98*/ LDR         R3, [SP, #32] 
    /*00005e9c*/ MOV         R2, #184                    @ 0xb8 
    /*00005ea0*/ LDR         LR, [SP, #24] 
    /*00005ea4*/ MOVW        R0, #16960                  @ 0x4240 
    /*00005ea8*/ LDR         R1, [SP, #36]               @ 0x24 
    /*00005eac*/ MOVT        R0, #15 
    /*00005eb0*/ MLA         R2, R2, R4, R5 
    /*00005eb4*/ SUB         R1, R1, IP 
    /*00005eb8*/ SUB         R3, R3, LR 
    /*00005ebc*/ MLA         R3, R0, R3, R1 
    /*00005ec0*/ LDR         IP, [R2, #100]              @ 0x64 
    /*00005ec4*/ CMP         R3, IP 
    /*00005ec8*/ STR         R3, [R2, #96]               @ 0x60 
    /*00005ecc*/ STRHI       R3, [R2, #100]              @ 0x64 
    /*00005ed0*/ MOV         R3, #184                    @ 0xb8 
    /*00005ed4*/ MUL         R3, R3, R4 
    /*00005ed8*/ ADD         R0, R5, R3 
    /*00005edc*/ LDR         R3, [R5, R3] 
    /*00005ee0*/ LDR         R2, [R0, #24] 
    /*00005ee4*/ LDR         R1, [R0, #116]              @ 0x74 
    /*00005ee8*/ SUBS        IP, R3, R2 
    /*00005eec*/ BEQ         AO_DRV_ISR_x5f00
    /*00005ef0*/ CMP         IP, R1 
    /*00005ef4*/ SUBCC       R1, R1, IP 
    /*00005ef8*/ STRCC       R1, [R0, #24] 
    /*00005efc*/ BCC         AO_DRV_ISR_x5f10
    AO_DRV_ISR_x5f00:
    /*00005f00*/ MOV         R0, #184                    @ 0xb8 
    /*00005f04*/ ADD         R1, R2, R1 
    /*00005f08*/ MLA         R2, R0, R4, R5 
    /*00005f0c*/ STR         R1, [R2, #24] 
    AO_DRV_ISR_x5f10:
    /*00005f10*/ CMP         R3, R1 
    /*00005f14*/ MOV         R0, R4 
    /*00005f18*/ BICNE       R1, R1, #15 
    /*00005f1c*/ MOVEQ       R3, #184                    @ 0xb8 
    /*00005f20*/ MOVEQ       R2, #0 
    /*00005f24*/ MLAEQ       R5, R3, R4, R5 
    /*00005f28*/ MOVEQ       R1, R2 
    /*00005f2c*/ STREQ       R2, [R5, #24] 
    /*00005f30*/ LDR         R3, [R6, #92]               @ 0x5c 
    /*00005f34*/ BLX         R3 
    AO_DRV_ISR_x5f38:
    /*00005f38*/ ADD         SP, SP, #40                 @ 0x28 
    /*00005f3c*/ POP         {R4, R5, R6, PC} 
    AO_DRV_ISR_x5f40:
    /*00005f40*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00005f44*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00005f48*/ MOV         IP, #184                    @ 0xb8 
    /*00005f4c*/ STR         R3, [SP, #8] 
    /*00005f50*/ MLA         IP, IP, R4, R2 
    /*00005f54*/ STR         R4, [SP, #4] 
    /*00005f58*/ MOVW        R2, #889                    @ 0x379 
    /*00005f5c*/ STR         R2, [SP] 
    /*00005f60*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x1e8] 
    /*00005f64*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1760] 
    /*00005f68*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x1e8] 
    /*00005f6c*/ MOV         R1, #22 
    /*00005f70*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1760] 
    /*00005f74*/ MOV         R0, #4 
    /*00005f78*/ LDR         LR, [IP, #136]              @ 0x88 
    /*00005f7c*/ ADD         LR, LR, #1 
    /*00005f80*/ STR         LR, [SP, #12] 
    /*00005f84*/ STR         LR, [IP, #136]              @ 0x88 
    /*00005f88*/ BL          0x0000000000005F88          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005f8c*/ B           AO_DRV_ISR_x5dd8
    AO_DRV_ISR_x5f90:
    /*00005f90*/ MOV         IP, #184                    @ 0xb8 
    /*00005f94*/ STR         R3, [SP, #8] 
    /*00005f98*/ MLA         IP, IP, R4, R5 
    /*00005f9c*/ STR         R4, [SP, #4] 
    /*00005fa0*/ MOV         R2, #976                    @ 0x3d0 
    /*00005fa4*/ STR         R2, [SP] 
    /*00005fa8*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x1e8] 
    /*00005fac*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17b0] 
    /*00005fb0*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x1e8] 
    /*00005fb4*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17b0] 
    /*00005fb8*/ MOV         R1, #22 
    /*00005fbc*/ MOV         R0, #3 
    /*00005fc0*/ LDR         LR, [IP, #140]              @ 0x8c 
    /*00005fc4*/ ADD         LR, LR, #1 
    /*00005fc8*/ STR         LR, [SP, #12] 
    /*00005fcc*/ STR         LR, [IP, #140]              @ 0x8c 
    /*00005fd0*/ BL          0x0000000000005FD0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005fd4*/ B           AO_DRV_ISR_x5e14
    AO_DRV_ISR_x5fd8:
    /*00005fd8*/ MOVW        R2, #879                    @ 0x36f 
    /*00005fdc*/ MOV         R1, #22 
    /*00005fe0*/ STR         R2, [SP] 
    /*00005fe4*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x1e8] 
    /*00005fe8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1728] 
    /*00005fec*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x1e8] 
    /*00005ff0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1728] 
    /*00005ff4*/ BL          0x0000000000005FF4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005ff8*/ ADD         SP, SP, #40                 @ 0x28 
    /*00005ffc*/ POP         {R4, R5, R6, PC} 
FUNC_END AO_DRV_ISR


FUNC_BEGIN AO_DRV_PutZeroData
    /*00006000*/ MOV         IP, #184                    @ 0xb8 
    /*00006004*/ MUL         IP, IP, R0 
    /*00006008*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000600c*/ MOVW        R7, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00006010*/ MOVT        R7, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00006014*/ SUB         SP, SP, #20 
    /*00006018*/ ADD         R3, R7, IP 
    /*0000601c*/ LDR         R2, [R3, #16] 
    /*00006020*/ LDR         LR, [R3, #24] 
    /*00006024*/ LDR         R4, [R3, #40]               @ 0x28 
    /*00006028*/ ADDS        R9, R2, LR 
    /*0000602c*/ LDR         R5, [R3, #60]               @ 0x3c 
    /*00006030*/ LDR         R6, [R3, #108]              @ 0x6c 
    /*00006034*/ BEQ         AO_DRV_PutZeroData_x64b8
    /*00006038*/ MOV         R8, R0 
    /*0000603c*/ LDR         R0, [R7, IP] 
    /*00006040*/ ADD         R2, R2, R0 
    /*00006044*/ CMP         R9, R2 
    /*00006048*/ BHI         AO_DRV_PutZeroData_x6474
    /*0000604c*/ MOV         SL, R1 
    /*00006050*/ LDR         R1, [R3, #64]               @ 0x40 
    /*00006054*/ MOV         R3, #8 
    /*00006058*/ SUB         R0, R0, LR 
    /*0000605c*/ LSL         R3, R3, R4 
    /*00006060*/ LSL         R0, R0, #3 
    /*00006064*/ MUL         R1, R1, R3 
    /*00006068*/ BL          0x0000000000006068          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000606c*/ MOV         R1, #46                     @ 0x2e 
    /*00006070*/ MLA         R1, R1, R8, SL 
    /*00006074*/ ADD         R1, R1, #30 
    /*00006078*/ LDR         R3, [R7, R1, LSL #2] 
    /*0000607c*/ STR         R3, [SP, #12] 
    /*00006080*/ CMP         R5, R0 
    /*00006084*/ MOVCC       R0, R5 
    /*00006088*/ CMP         R4, #1 
    /*0000608c*/ BEQ         AO_DRV_PutZeroData_x6300
    /*00006090*/ BCC         AO_DRV_PutZeroData_x61d4
    /*00006094*/ CMP         R4, #2 
    /*00006098*/ BNE         AO_DRV_PutZeroData_x642c
    /*0000609c*/ LDR         R3, [SP, #12] 
    /*000060a0*/ ANDS        R1, R0, #3 
    /*000060a4*/ LSL         R4, R3, #2 
    /*000060a8*/ ADD         R9, R9, R4 
    /*000060ac*/ BEQ         AO_DRV_PutZeroData_x6464
    /*000060b0*/ MOV         R2, #1 
    /*000060b4*/ MOV         R3, #0 
    /*000060b8*/ LSL         LR, R2, R6 
    /*000060bc*/ MOV         IP, R3 
    /*000060c0*/ MOV         R2, R9 
    AO_DRV_PutZeroData_x60c4:
    /*000060c4*/ ADD         R3, R3, #1 
    /*000060c8*/ STR         IP, [R2], LR 
    /*000060cc*/ CMP         R1, R3 
    /*000060d0*/ BNE         AO_DRV_PutZeroData_x60c4
    AO_DRV_PutZeroData_x60d4:
    /*000060d4*/ CMP         R0, R3 
    /*000060d8*/ BLS         AO_DRV_PutZeroData_x6128
    /*000060dc*/ ADD         FP, R3, #1 
    /*000060e0*/ ADD         SL, R3, #2 
    /*000060e4*/ ADD         LR, R3, #3 
    /*000060e8*/ MOV         R1, #4 
    /*000060ec*/ ADD         FP, R9, FP, LSL R6 
    /*000060f0*/ ADD         SL, R9, SL, LSL R6 
    /*000060f4*/ ADD         LR, R9, LR, LSL R6 
    /*000060f8*/ MOV         R2, #0 
    /*000060fc*/ ADD         R9, R9, R3, LSL R6 
    /*00006100*/ LSL         R1, R1, R6 
    /*00006104*/ MOV         IP, R2 
    AO_DRV_PutZeroData_x6108:
    /*00006108*/ ADD         R3, R3, #4 
    /*0000610c*/ STR         IP, [R9, R2] 
    /*00006110*/ CMP         R0, R3 
    /*00006114*/ STR         IP, [FP, R2] 
    /*00006118*/ STR         IP, [SL, R2] 
    /*0000611c*/ STR         IP, [LR, R2] 
    /*00006120*/ ADD         R2, R2, R1 
    /*00006124*/ BHI         AO_DRV_PutZeroData_x6108
    AO_DRV_PutZeroData_x6128:
    /*00006128*/ CMP         R5, R0 
    /*0000612c*/ BLS         AO_DRV_PutZeroData_x61c8
    /*00006130*/ MOV         R3, #184                    @ 0xb8 
    /*00006134*/ SUB         R1, R5, R0 
    /*00006138*/ MLA         R7, R3, R8, R7 
    /*0000613c*/ ANDS        R1, R1, #3 
    /*00006140*/ LDR         R3, [R7, #16] 
    /*00006144*/ ADD         R4, R3, R4 
    /*00006148*/ BEQ         AO_DRV_PutZeroData_x64a8
    /*0000614c*/ MOV         R2, #1 
    /*00006150*/ MOV         R3, #0 
    /*00006154*/ LSL         LR, R2, R6 
    /*00006158*/ MOV         IP, R3 
    /*0000615c*/ MOV         R2, R4 
    AO_DRV_PutZeroData_x6160:
    /*00006160*/ ADD         R3, R3, #1 
    /*00006164*/ STR         IP, [R2], LR 
    /*00006168*/ CMP         R1, R3 
    /*0000616c*/ BNE         AO_DRV_PutZeroData_x6160
    /*00006170*/ ADD         R0, R0, R3 
    /*00006174*/ CMP         R5, R0 
    /*00006178*/ BLS         AO_DRV_PutZeroData_x61c8
    AO_DRV_PutZeroData_x617c:
    /*0000617c*/ ADD         LR, R3, #1 
    /*00006180*/ ADD         IP, R3, #2 
    /*00006184*/ ADD         R1, R3, #3 
    /*00006188*/ MOV         R2, #4 
    /*0000618c*/ ADD         LR, R4, LR, LSL R6 
    /*00006190*/ ADD         IP, R4, IP, LSL R6 
    /*00006194*/ ADD         R1, R4, R1, LSL R6 
    /*00006198*/ ADD         R4, R4, R3, LSL R6 
    /*0000619c*/ MOV         R3, #0 
    /*000061a0*/ LSL         R6, R2, R6 
    /*000061a4*/ MOV         R2, R3 
    AO_DRV_PutZeroData_x61a8:
    /*000061a8*/ ADD         R0, R0, #4 
    /*000061ac*/ STR         R2, [R4, R3] 
    /*000061b0*/ CMP         R5, R0 
    /*000061b4*/ STR         R2, [LR, R3] 
    /*000061b8*/ STR         R2, [IP, R3] 
    /*000061bc*/ STR         R2, [R1, R3] 
    /*000061c0*/ ADD         R3, R3, R6 
    /*000061c4*/ BHI         AO_DRV_PutZeroData_x61a8
    AO_DRV_PutZeroData_x61c8:
    /*000061c8*/ MOV         R0, #0 
    AO_DRV_PutZeroData_x61cc:
    /*000061cc*/ ADD         SP, SP, #20 
    /*000061d0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AO_DRV_PutZeroData_x61d4:
    /*000061d4*/ LDR         R3, [SP, #12] 
    /*000061d8*/ ANDS        LR, R0, #3 
    /*000061dc*/ ADD         R1, R9, R3 
    /*000061e0*/ BEQ         AO_DRV_PutZeroData_x646c
    /*000061e4*/ MOV         R3, #1 
    /*000061e8*/ MOV         R2, R1 
    /*000061ec*/ LSL         R9, R3, R6 
    /*000061f0*/ MOV         IP, #0 
    /*000061f4*/ MOV         R3, R4 
    AO_DRV_PutZeroData_x61f8:
    /*000061f8*/ ADD         R3, R3, #1 
    /*000061fc*/ STRB        IP, [R2], R9 
    /*00006200*/ CMP         LR, R3 
    /*00006204*/ BNE         AO_DRV_PutZeroData_x61f8
    AO_DRV_PutZeroData_x6208:
    /*00006208*/ CMP         R3, R0 
    /*0000620c*/ BCS         AO_DRV_PutZeroData_x625c
    /*00006210*/ ADD         SL, R3, #1 
    /*00006214*/ ADD         R9, R3, #2 
    /*00006218*/ ADD         LR, R3, #3 
    /*0000621c*/ MOV         FP, #4 
    /*00006220*/ ADD         SL, R1, SL, LSL R6 
    /*00006224*/ ADD         R9, R1, R9, LSL R6 
    /*00006228*/ ADD         LR, R1, LR, LSL R6 
    /*0000622c*/ MOV         R2, #0 
    /*00006230*/ ADD         R1, R1, R3, LSL R6 
    /*00006234*/ LSL         FP, FP, R6 
    /*00006238*/ MOV         IP, R2 
    AO_DRV_PutZeroData_x623c:
    /*0000623c*/ ADD         R3, R3, #4 
    /*00006240*/ STRB        IP, [R1, R2] 
    /*00006244*/ CMP         R0, R3 
    /*00006248*/ STRB        IP, [SL, R2] 
    /*0000624c*/ STRB        IP, [R9, R2] 
    /*00006250*/ STRB        IP, [LR, R2] 
    /*00006254*/ ADD         R2, R2, FP 
    /*00006258*/ BHI         AO_DRV_PutZeroData_x623c
    AO_DRV_PutZeroData_x625c:
    /*0000625c*/ CMP         R5, R0 
    /*00006260*/ BLS         AO_DRV_PutZeroData_x61c8
    /*00006264*/ MOV         R3, #184                    @ 0xb8 
    /*00006268*/ SUB         R1, R5, R0 
    /*0000626c*/ MLA         R7, R3, R8, R7 
    /*00006270*/ ANDS        R1, R1, #3 
    /*00006274*/ LDR         R3, [SP, #12] 
    /*00006278*/ LDR         R2, [R7, #16] 
    /*0000627c*/ ADD         R2, R2, R3 
    /*00006280*/ BEQ         AO_DRV_PutZeroData_x64a0
    /*00006284*/ MOV         IP, #1 
    /*00006288*/ MOV         R3, R2 
    /*0000628c*/ LSL         LR, IP, R6 
    /*00006290*/ MOV         IP, #0 
    AO_DRV_PutZeroData_x6294:
    /*00006294*/ ADD         R4, R4, #1 
    /*00006298*/ STRB        IP, [R3], LR 
    /*0000629c*/ CMP         R1, R4 
    /*000062a0*/ BNE         AO_DRV_PutZeroData_x6294
    /*000062a4*/ ADD         R0, R0, R4 
    /*000062a8*/ CMP         R5, R0 
    /*000062ac*/ BLS         AO_DRV_PutZeroData_x61c8
    AO_DRV_PutZeroData_x62b0:
    /*000062b0*/ ADD         LR, R4, #1 
    /*000062b4*/ ADD         IP, R4, #2 
    /*000062b8*/ ADD         R1, R4, #3 
    /*000062bc*/ MOV         R3, #4 
    /*000062c0*/ ADD         LR, R2, LR, LSL R6 
    /*000062c4*/ ADD         IP, R2, IP, LSL R6 
    /*000062c8*/ ADD         R1, R2, R1, LSL R6 
    /*000062cc*/ ADD         R4, R2, R4, LSL R6 
    /*000062d0*/ LSL         R6, R3, R6 
    /*000062d4*/ MOV         R3, #0 
    /*000062d8*/ MOV         R2, R3 
    AO_DRV_PutZeroData_x62dc:
    /*000062dc*/ ADD         R0, R0, #4 
    /*000062e0*/ STRB        R2, [R4, R3] 
    /*000062e4*/ CMP         R5, R0 
    /*000062e8*/ STRB        R2, [LR, R3] 
    /*000062ec*/ STRB        R2, [IP, R3] 
    /*000062f0*/ STRB        R2, [R1, R3] 
    /*000062f4*/ ADD         R3, R3, R6 
    /*000062f8*/ BHI         AO_DRV_PutZeroData_x62dc
    /*000062fc*/ B           AO_DRV_PutZeroData_x61c8
    AO_DRV_PutZeroData_x6300:
    /*00006300*/ LDR         R3, [SP, #12] 
    /*00006304*/ ANDS        R1, R0, #3 
    /*00006308*/ LSL         SL, R3, #1 
    /*0000630c*/ ADD         R9, R9, SL 
    /*00006310*/ BEQ         AO_DRV_PutZeroData_x645c
    /*00006314*/ MOV         R3, #0 
    /*00006318*/ LSL         R4, R4, R6 
    /*0000631c*/ MOV         IP, R3 
    /*00006320*/ MOV         R2, R9 
    AO_DRV_PutZeroData_x6324:
    /*00006324*/ ADD         R3, R3, #1 
    /*00006328*/ STRH        IP, [R2], R4 
    /*0000632c*/ CMP         R1, R3 
    /*00006330*/ BNE         AO_DRV_PutZeroData_x6324
    AO_DRV_PutZeroData_x6334:
    /*00006334*/ CMP         R3, R0 
    /*00006338*/ BCS         AO_DRV_PutZeroData_x6388
    /*0000633c*/ ADD         R1, R3, #1 
    /*00006340*/ ADD         FP, R3, #2 
    /*00006344*/ ADD         R4, R3, #3 
    /*00006348*/ MOV         LR, #4 
    /*0000634c*/ ADD         R1, R9, R1, LSL R6 
    /*00006350*/ ADD         FP, R9, FP, LSL R6 
    /*00006354*/ ADD         R4, R9, R4, LSL R6 
    /*00006358*/ MOV         R2, #0 
    /*0000635c*/ ADD         R9, R9, R3, LSL R6 
    /*00006360*/ LSL         LR, LR, R6 
    /*00006364*/ MOV         IP, R2 
    AO_DRV_PutZeroData_x6368:
    /*00006368*/ ADD         R3, R3, #4 
    /*0000636c*/ STRH        IP, [R9, R2] 
    /*00006370*/ CMP         R0, R3 
    /*00006374*/ STRH        IP, [R1, R2] 
    /*00006378*/ STRH        IP, [FP, R2] 
    /*0000637c*/ STRH        IP, [R4, R2] 
    /*00006380*/ ADD         R2, R2, LR 
    /*00006384*/ BHI         AO_DRV_PutZeroData_x6368
    AO_DRV_PutZeroData_x6388:
    /*00006388*/ CMP         R5, R0 
    /*0000638c*/ BLS         AO_DRV_PutZeroData_x61c8
    /*00006390*/ MOV         R3, #184                    @ 0xb8 
    /*00006394*/ SUB         R1, R5, R0 
    /*00006398*/ MLA         R7, R3, R8, R7 
    /*0000639c*/ ANDS        R1, R1, #3 
    /*000063a0*/ LDR         R3, [R7, #16] 
    /*000063a4*/ ADD         SL, R3, SL 
    /*000063a8*/ BEQ         AO_DRV_PutZeroData_x64b0
    /*000063ac*/ MOV         R2, #1 
    /*000063b0*/ MOV         R3, #0 
    /*000063b4*/ LSL         LR, R2, R6 
    /*000063b8*/ MOV         IP, R3 
    /*000063bc*/ MOV         R2, SL 
    AO_DRV_PutZeroData_x63c0:
    /*000063c0*/ ADD         R3, R3, #1 
    /*000063c4*/ STRH        IP, [R2], LR 
    /*000063c8*/ CMP         R1, R3 
    /*000063cc*/ BNE         AO_DRV_PutZeroData_x63c0
    /*000063d0*/ ADD         R0, R0, R3 
    /*000063d4*/ CMP         R5, R0 
    /*000063d8*/ BLS         AO_DRV_PutZeroData_x61c8
    AO_DRV_PutZeroData_x63dc:
    /*000063dc*/ ADD         LR, R3, #1 
    /*000063e0*/ ADD         IP, R3, #2 
    /*000063e4*/ ADD         R1, R3, #3 
    /*000063e8*/ MOV         R2, #4 
    /*000063ec*/ ADD         LR, SL, LR, LSL R6 
    /*000063f0*/ ADD         IP, SL, IP, LSL R6 
    /*000063f4*/ ADD         R1, SL, R1, LSL R6 
    /*000063f8*/ ADD         SL, SL, R3, LSL R6 
    /*000063fc*/ MOV         R3, #0 
    /*00006400*/ LSL         R6, R2, R6 
    /*00006404*/ MOV         R2, R3 
    AO_DRV_PutZeroData_x6408:
    /*00006408*/ ADD         R0, R0, #4 
    /*0000640c*/ STRH        R2, [SL, R3] 
    /*00006410*/ CMP         R5, R0 
    /*00006414*/ STRH        R2, [LR, R3] 
    /*00006418*/ STRH        R2, [IP, R3] 
    /*0000641c*/ STRH        R2, [R1, R3] 
    /*00006420*/ ADD         R3, R3, R6 
    /*00006424*/ BHI         AO_DRV_PutZeroData_x6408
    /*00006428*/ B           AO_DRV_PutZeroData_x61c8
    AO_DRV_PutZeroData_x642c:
    /*0000642c*/ MOV         R3, #187                    @ 0xbb 
    /*00006430*/ MOV         R1, #22 
    /*00006434*/ STR         R3, [SP] 
    /*00006438*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1870] 
    /*0000643c*/ LDR         R3, [PC, #160]              @ 0x00000000000064e4 
    /*00006440*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1870] 
    /*00006444*/ MOV         R0, #6 
    /*00006448*/ BL          0x0000000000006448          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000644c*/ MOVW        R0, #32776                  @ 0x8008 
    /*00006450*/ MOVT        R0, #40982                  @ 0xa016 
    /*00006454*/ ADD         SP, SP, #20 
    /*00006458*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AO_DRV_PutZeroData_x645c:
    /*0000645c*/ MOV         R3, R1 
    /*00006460*/ B           AO_DRV_PutZeroData_x6334
    AO_DRV_PutZeroData_x6464:
    /*00006464*/ MOV         R3, R1 
    /*00006468*/ B           AO_DRV_PutZeroData_x60d4
    AO_DRV_PutZeroData_x646c:
    /*0000646c*/ MOV         R3, LR 
    /*00006470*/ B           AO_DRV_PutZeroData_x6208
    AO_DRV_PutZeroData_x6474:
    /*00006474*/ MOV         R3, #60                     @ 0x3c 
    /*00006478*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x183c] 
    /*0000647c*/ STR         R3, [SP] 
    /*00006480*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x183c] 
    /*00006484*/ LDR         R3, [PC, #88]               @ 0x00000000000064e4 
    /*00006488*/ MOV         R1, #22 
    /*0000648c*/ MOV         R0, #3 
    /*00006490*/ BL          0x0000000000006490          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006494*/ MOVW        R0, #32771                  @ 0x8003 
    /*00006498*/ MOVT        R0, #40982                  @ 0xa016 
    /*0000649c*/ B           AO_DRV_PutZeroData_x61cc
    AO_DRV_PutZeroData_x64a0:
    /*000064a0*/ MOV         R4, R1 
    /*000064a4*/ B           AO_DRV_PutZeroData_x62b0
    AO_DRV_PutZeroData_x64a8:
    /*000064a8*/ MOV         R3, R1 
    /*000064ac*/ B           AO_DRV_PutZeroData_x617c
    AO_DRV_PutZeroData_x64b0:
    /*000064b0*/ MOV         R3, R1 
    /*000064b4*/ B           AO_DRV_PutZeroData_x63dc
    AO_DRV_PutZeroData_x64b8:
    /*000064b8*/ MOV         R3, #55                     @ 0x37 
    /*000064bc*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1800] 
    /*000064c0*/ STR         R3, [SP] 
    /*000064c4*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1800] 
    /*000064c8*/ LDR         R3, [PC, #20]               @ 0x00000000000064e4 
    /*000064cc*/ MOV         R1, #22 
    /*000064d0*/ MOV         R0, #3 
    /*000064d4*/ BL          0x00000000000064D4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000064d8*/ MOVW        R0, #32773                  @ 0x8005 
    /*000064dc*/ MOVT        R0, #40982                  @ 0xa016 
    /*000064e0*/ B           AO_DRV_PutZeroData_x61cc
    /*000064e4*/ STRDEQ      R0, [R0], -R4 
FUNC_END AO_DRV_PutZeroData


FUNC_BEGIN AO_DRV_PutLastData
    /*000064e8*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000064ec*/ SUB         SP, SP, #28 
    /*000064f0*/ MOV         R3, R0 
    /*000064f4*/ STR         R0, [SP, #12] 
    /*000064f8*/ MOV         R0, #184                    @ 0xb8 
    /*000064fc*/ MUL         R0, R0, R3 
    /*00006500*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00006504*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00006508*/ STR         R3, [SP, #8] 
    /*0000650c*/ ADD         R2, R3, R0 
    /*00006510*/ LDR         R3, [R2, #16] 
    /*00006514*/ LDR         R9, [R2, #24] 
    /*00006518*/ LDR         R8, [R2, #40]               @ 0x28 
    /*0000651c*/ ADDS        R5, R3, R9 
    /*00006520*/ LDR         R4, [R2, #60]               @ 0x3c 
    /*00006524*/ LDR         R7, [R2, #108]              @ 0x6c 
    /*00006528*/ BEQ         AO_DRV_PutLastData_x6ce8
    /*0000652c*/ MOV         FP, R1 
    /*00006530*/ LDR         R1, [SP, #8] 
    /*00006534*/ LDR         SL, [R1, R0] 
    /*00006538*/ ADD         R1, R3, SL 
    /*0000653c*/ CMP         R5, R1 
    /*00006540*/ BHI         AO_DRV_PutLastData_x6c8c
    /*00006544*/ LDR         R6, [R2, #64]               @ 0x40 
    /*00006548*/ MOV         R1, #8 
    /*0000654c*/ LSL         R1, R1, R8 
    /*00006550*/ SUB         R0, SL, R9 
    /*00006554*/ STR         R3, [SP, #20] 
    /*00006558*/ MUL         R6, R6, R1 
    /*0000655c*/ LSL         R0, R0, #3 
    /*00006560*/ STR         R2, [SP, #16] 
    /*00006564*/ MOV         R1, R6 
    /*00006568*/ BL          0x0000000000006568          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000656c*/ LDR         R2, [SP, #16] 
    /*00006570*/ LDR         R3, [SP, #20] 
    /*00006574*/ LDR         R1, [R2, #116]              @ 0x74 
    /*00006578*/ STR         R3, [SP, #16] 
    /*0000657c*/ CMP         R4, R0 
    /*00006580*/ MOVCS       R2, R0 
    /*00006584*/ MOVCC       R2, R4 
    /*00006588*/ CMP         R9, R1 
    /*0000658c*/ STR         R2, [SP, #20] 
    /*00006590*/ ADDCC       R9, R9, SL 
    /*00006594*/ SUB         R9, R9, R1 
    /*00006598*/ MOV         R1, R6 
    /*0000659c*/ SUB         R0, SL, R9 
    /*000065a0*/ ADD         R9, R3, R9 
    /*000065a4*/ LSL         R0, R0, #3 
    /*000065a8*/ BL          0x00000000000065A8          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000065ac*/ LDR         R3, [SP, #16] 
    /*000065b0*/ LDR         R2, [SP, #20] 
    /*000065b4*/ CMP         R4, R0 
    /*000065b8*/ BCC         AO_DRV_PutLastData_x6820
    /*000065bc*/ CMP         R4, R2 
    /*000065c0*/ BHI         AO_DRV_PutLastData_x682c
    /*000065c4*/ CMP         R4, R0 
    /*000065c8*/ BLS         AO_DRV_PutLastData_x6864
    /*000065cc*/ MOV         R1, R0 
    AO_DRV_PutLastData_x65d0:
    /*000065d0*/ LDR         LR, [SP, #12] 
    /*000065d4*/ MOV         IP, #46                     @ 0x2e 
    /*000065d8*/ CMP         R8, #1 
    /*000065dc*/ MLA         FP, IP, LR, FP 
    /*000065e0*/ LDR         IP, [SP, #8] 
    /*000065e4*/ ADD         FP, FP, #30 
    /*000065e8*/ LDR         IP, [IP, FP, LSL #2] 
    /*000065ec*/ BEQ         AO_DRV_PutLastData_x6b14
    /*000065f0*/ BCC         AO_DRV_PutLastData_x689c
    /*000065f4*/ CMP         R8, #2 
    /*000065f8*/ BNE         AO_DRV_PutLastData_x6834
    /*000065fc*/ CMP         R1, R4 
    /*00006600*/ LSL         FP, IP, #2 
    /*00006604*/ ADD         SL, R5, FP 
    /*00006608*/ MOVCS       R3, #0 
    /*0000660c*/ MOVCC       R3, #1 
    /*00006610*/ CMP         R1, R2 
    /*00006614*/ MOVNE       R2, #0 
    /*00006618*/ ANDEQ       R2, R3, #1 
    /*0000661c*/ CMP         R1, R0 
    /*00006620*/ MOVNE       R3, #0 
    /*00006624*/ ANDEQ       R3, R3, #1 
    /*00006628*/ CMP         R2, #0 
    /*0000662c*/ STR         R3, [SP, #20] 
    /*00006630*/ ADD         R3, R9, FP 
    /*00006634*/ AND         R0, R1, #3 
    /*00006638*/ STR         R3, [SP, #16] 
    /*0000663c*/ MOV         R2, R3 
    /*00006640*/ BEQ         AO_DRV_PutLastData_x6760
    /*00006644*/ SUB         R3, R4, #1 
    /*00006648*/ CMP         R0, #0 
    /*0000664c*/ LSL         R3, R3, R7 
    /*00006650*/ LDR         R3, [R2, R3] 
    /*00006654*/ BEQ         AO_DRV_PutLastData_x6cd8
    /*00006658*/ MOV         R2, #1 
    /*0000665c*/ MOV         IP, SL 
    /*00006660*/ LSL         LR, R2, R7 
    /*00006664*/ MOV         R2, #0 
    AO_DRV_PutLastData_x6668:
    /*00006668*/ ADD         R2, R2, #1 
    /*0000666c*/ STR         R3, [IP], LR 
    /*00006670*/ CMP         R0, R2 
    /*00006674*/ BNE         AO_DRV_PutLastData_x6668
    AO_DRV_PutLastData_x6678:
    /*00006678*/ CMP         R1, R2 
    /*0000667c*/ BLS         AO_DRV_PutLastData_x66c8
    /*00006680*/ ADD         R6, R2, #1 
    /*00006684*/ ADD         LR, R2, #2 
    /*00006688*/ ADD         IP, R2, #3 
    /*0000668c*/ MOV         R8, #4 
    /*00006690*/ ADD         R6, SL, R6, LSL R7 
    /*00006694*/ ADD         LR, SL, LR, LSL R7 
    /*00006698*/ ADD         IP, SL, IP, LSL R7 
    /*0000669c*/ ADD         R5, SL, R2, LSL R7 
    /*000066a0*/ LSL         R8, R8, R7 
    /*000066a4*/ MOV         R0, #0 
    AO_DRV_PutLastData_x66a8:
    /*000066a8*/ ADD         R2, R2, #4 
    /*000066ac*/ STR         R3, [R5, R0] 
    /*000066b0*/ CMP         R1, R2 
    /*000066b4*/ STR         R3, [R6, R0] 
    /*000066b8*/ STR         R3, [LR, R0] 
    /*000066bc*/ STR         R3, [IP, R0] 
    /*000066c0*/ ADD         R0, R0, R8 
    /*000066c4*/ BHI         AO_DRV_PutLastData_x66a8
    AO_DRV_PutLastData_x66c8:
    /*000066c8*/ LDR         IP, [SP, #12] 
    /*000066cc*/ MOV         R2, #184                    @ 0xb8 
    /*000066d0*/ LDR         LR, [SP, #8] 
    /*000066d4*/ SUB         R0, R4, R1 
    /*000066d8*/ ANDS        R0, R0, #3 
    /*000066dc*/ MLA         R2, R2, IP, LR 
    /*000066e0*/ LDR         R5, [R2, #16] 
    /*000066e4*/ MOVNE       R2, #1 
    /*000066e8*/ LSLNE       LR, R2, R7 
    /*000066ec*/ MOVNE       R2, #0 
    /*000066f0*/ ADD         SL, R5, FP 
    /*000066f4*/ MOVNE       IP, SL 
    /*000066f8*/ BEQ         AO_DRV_PutLastData_x670c
    AO_DRV_PutLastData_x66fc:
    /*000066fc*/ ADD         R2, R2, #1 
    /*00006700*/ STR         R3, [IP], LR 
    /*00006704*/ CMP         R0, R2 
    /*00006708*/ BNE         AO_DRV_PutLastData_x66fc
    AO_DRV_PutLastData_x670c:
    /*0000670c*/ ADD         IP, R1, R0 
    /*00006710*/ CMP         R4, IP 
    /*00006714*/ BLS         AO_DRV_PutLastData_x6760
    /*00006718*/ ADD         R6, R0, #1 
    /*0000671c*/ ADD         R5, R0, #2 
    /*00006720*/ ADD         LR, R0, #3 
    /*00006724*/ MOV         R8, #4 
    /*00006728*/ ADD         R6, SL, R6, LSL R7 
    /*0000672c*/ ADD         R5, SL, R5, LSL R7 
    /*00006730*/ ADD         LR, SL, LR, LSL R7 
    /*00006734*/ LSL         R8, R8, R7 
    /*00006738*/ ADD         R9, SL, R0, LSL R7 
    /*0000673c*/ MOV         R2, #0 
    AO_DRV_PutLastData_x6740:
    /*00006740*/ ADD         IP, IP, #4 
    /*00006744*/ STR         R3, [R9, R2] 
    /*00006748*/ CMP         R4, IP 
    /*0000674c*/ STR         R3, [R6, R2] 
    /*00006750*/ STR         R3, [R5, R2] 
    /*00006754*/ STR         R3, [LR, R2] 
    /*00006758*/ ADD         R2, R2, R8 
    /*0000675c*/ BHI         AO_DRV_PutLastData_x6740
    AO_DRV_PutLastData_x6760:
    /*00006760*/ LDR         R3, [SP, #20] 
    /*00006764*/ CMP         R3, #0 
    /*00006768*/ BEQ         AO_DRV_PutLastData_x6890
    /*0000676c*/ CMP         R4, R1 
    /*00006770*/ SUBNE       R3, R4, #1 
    /*00006774*/ SUBEQ       R1, R4, #1 
    /*00006778*/ LDRNE       IP, [SP, #12] 
    /*0000677c*/ MOVNE       R2, #184                    @ 0xb8 
    /*00006780*/ LDRNE       LR, [SP, #8] 
    /*00006784*/ SUBNE       R1, R3, R1 
    /*00006788*/ LSL         R1, R1, R7 
    /*0000678c*/ MLANE       R2, R2, IP, LR 
    /*00006790*/ LDRNE       R6, [R2, #16] 
    /*00006794*/ ADDNE       R3, R6, FP 
    /*00006798*/ STRNE       R3, [SP, #16] 
    /*0000679c*/ LDR         R3, [SP, #16] 
    /*000067a0*/ CMP         R0, #0 
    /*000067a4*/ LDR         R1, [R3, R1] 
    /*000067a8*/ BEQ         AO_DRV_PutLastData_x6cb8
    /*000067ac*/ MOV         R3, #1 
    /*000067b0*/ MOV         R2, SL 
    /*000067b4*/ LSL         IP, R3, R7 
    /*000067b8*/ MOV         R3, #0 
    AO_DRV_PutLastData_x67bc:
    /*000067bc*/ ADD         R3, R3, #1 
    /*000067c0*/ STR         R1, [R2], IP 
    /*000067c4*/ CMP         R0, R3 
    /*000067c8*/ BNE         AO_DRV_PutLastData_x67bc
    AO_DRV_PutLastData_x67cc:
    /*000067cc*/ CMP         R4, R3 
    /*000067d0*/ BLS         AO_DRV_PutLastData_x6890
    /*000067d4*/ ADD         LR, R3, #1 
    /*000067d8*/ ADD         IP, R3, #2 
    /*000067dc*/ ADD         R0, R3, #3 
    /*000067e0*/ MOV         R2, #4 
    /*000067e4*/ ADD         LR, SL, LR, LSL R7 
    /*000067e8*/ ADD         IP, SL, IP, LSL R7 
    /*000067ec*/ ADD         R0, SL, R0, LSL R7 
    /*000067f0*/ ADD         R5, SL, R3, LSL R7 
    /*000067f4*/ LSL         R7, R2, R7 
    /*000067f8*/ MOV         R2, #0 
    AO_DRV_PutLastData_x67fc:
    /*000067fc*/ ADD         R3, R3, #4 
    /*00006800*/ STR         R1, [R5, R2] 
    /*00006804*/ CMP         R4, R3 
    /*00006808*/ STR         R1, [LR, R2] 
    /*0000680c*/ STR         R1, [IP, R2] 
    /*00006810*/ STR         R1, [R0, R2] 
    /*00006814*/ ADD         R2, R2, R7 
    /*00006818*/ BHI         AO_DRV_PutLastData_x67fc
    /*0000681c*/ B           AO_DRV_PutLastData_x6890
    AO_DRV_PutLastData_x6820:
    /*00006820*/ CMP         R4, R2 
    /*00006824*/ BLS         AO_DRV_PutLastData_x6864
    /*00006828*/ MOV         R0, R4 
    AO_DRV_PutLastData_x682c:
    /*0000682c*/ MOV         R1, R2 
    /*00006830*/ B           AO_DRV_PutLastData_x65d0
    AO_DRV_PutLastData_x6834:
    /*00006834*/ MOVW        R3, #490                    @ 0x1ea 
    /*00006838*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1870] 
    /*0000683c*/ STR         R3, [SP] 
    /*00006840*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1870] 
    /*00006844*/ LDR         R3, [PC, #1224]             @ 0x0000000000006d14 
    /*00006848*/ MOV         R1, #22 
    /*0000684c*/ MOV         R0, #6 
    /*00006850*/ BL          0x0000000000006850          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006854*/ MOVW        R0, #32776                  @ 0x8008 
    /*00006858*/ MOVT        R0, #40982                  @ 0xa016 
    AO_DRV_PutLastData_x685c:
    /*0000685c*/ ADD         SP, SP, #28 
    /*00006860*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AO_DRV_PutLastData_x6864:
    /*00006864*/ LDR         R2, [SP, #12] 
    /*00006868*/ MOV         R3, #46                     @ 0x2e 
    /*0000686c*/ CMP         R8, #1 
    /*00006870*/ MLA         FP, R3, R2, FP 
    /*00006874*/ LDR         R3, [SP, #8] 
    /*00006878*/ ADD         FP, FP, #30 
    /*0000687c*/ LDR         R3, [R3, FP, LSL #2] 
    /*00006880*/ BEQ         AO_DRV_PutLastData_x6a7c
    /*00006884*/ BCC         AO_DRV_PutLastData_x6c78
    /*00006888*/ CMP         R8, #2 
    /*0000688c*/ BNE         AO_DRV_PutLastData_x6834
    AO_DRV_PutLastData_x6890:
    /*00006890*/ MOV         R0, #0 
    /*00006894*/ ADD         SP, SP, #28 
    /*00006898*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AO_DRV_PutLastData_x689c:
    /*0000689c*/ CMP         R1, R4 
    /*000068a0*/ ADD         R5, R5, IP 
    /*000068a4*/ ADD         R6, R9, IP 
    /*000068a8*/ MOVCS       R0, #0 
    /*000068ac*/ MOVCC       R0, #1 
    /*000068b0*/ CMP         R1, R2 
    /*000068b4*/ MOVNE       R0, #0 
    /*000068b8*/ CMP         R0, #0 
    /*000068bc*/ AND         R0, R1, #3 
    /*000068c0*/ BEQ         AO_DRV_PutLastData_x69e0
    /*000068c4*/ SUB         R3, R4, #1 
    /*000068c8*/ CMP         R0, #0 
    /*000068cc*/ LSL         R3, R3, R7 
    /*000068d0*/ LDRB        R3, [R6, R3] 
    /*000068d4*/ BEQ         AO_DRV_PutLastData_x6cd0
    /*000068d8*/ MOV         R2, #1 
    /*000068dc*/ MOV         LR, R5 
    /*000068e0*/ LSL         R6, R2, R7 
    /*000068e4*/ MOV         R2, R8 
    AO_DRV_PutLastData_x68e8:
    /*000068e8*/ ADD         R2, R2, #1 
    /*000068ec*/ STRB        R3, [LR], R6 
    /*000068f0*/ CMP         R0, R2 
    /*000068f4*/ BNE         AO_DRV_PutLastData_x68e8
    AO_DRV_PutLastData_x68f8:
    /*000068f8*/ CMP         R1, R2 
    /*000068fc*/ BLS         AO_DRV_PutLastData_x6948
    /*00006900*/ ADD         R9, R2, #1 
    /*00006904*/ ADD         R6, R2, #2 
    /*00006908*/ ADD         LR, R2, #3 
    /*0000690c*/ MOV         SL, #4 
    /*00006910*/ ADD         R9, R5, R9, LSL R7 
    /*00006914*/ ADD         R6, R5, R6, LSL R7 
    /*00006918*/ ADD         LR, R5, LR, LSL R7 
    /*0000691c*/ LSL         SL, SL, R7 
    /*00006920*/ ADD         R5, R5, R2, LSL R7 
    /*00006924*/ MOV         R0, #0 
    AO_DRV_PutLastData_x6928:
    /*00006928*/ ADD         R2, R2, #4 
    /*0000692c*/ STRB        R3, [R5, R0] 
    /*00006930*/ CMP         R1, R2 
    /*00006934*/ STRB        R3, [R9, R0] 
    /*00006938*/ STRB        R3, [R6, R0] 
    /*0000693c*/ STRB        R3, [LR, R0] 
    /*00006940*/ ADD         R0, R0, SL 
    /*00006944*/ BHI         AO_DRV_PutLastData_x6928
    AO_DRV_PutLastData_x6948:
    /*00006948*/ LDR         LR, [SP, #8] 
    /*0000694c*/ MOV         FP, #184                    @ 0xb8 
    /*00006950*/ LDR         R2, [SP, #12] 
    /*00006954*/ SUB         R0, R4, R1 
    /*00006958*/ ANDS        R0, R0, #3 
    /*0000695c*/ MLA         FP, FP, R2, LR 
    /*00006960*/ MOVNE       LR, #1 
    /*00006964*/ LSLNE       LR, LR, R7 
    /*00006968*/ LDR         R2, [FP, #16] 
    /*0000696c*/ ADD         IP, R2, IP 
    /*00006970*/ MOVNE       R2, IP 
    /*00006974*/ BEQ         AO_DRV_PutLastData_x6988
    AO_DRV_PutLastData_x6978:
    /*00006978*/ ADD         R8, R8, #1 
    /*0000697c*/ STRB        R3, [R2], LR 
    /*00006980*/ CMP         R0, R8 
    /*00006984*/ BNE         AO_DRV_PutLastData_x6978
    AO_DRV_PutLastData_x6988:
    /*00006988*/ ADD         R1, R1, R0 
    /*0000698c*/ CMP         R4, R1 
    /*00006990*/ BLS         AO_DRV_PutLastData_x6890
    /*00006994*/ ADD         R6, R0, #1 
    /*00006998*/ ADD         R5, R0, #2 
    /*0000699c*/ ADD         LR, R0, #3 
    /*000069a0*/ MOV         R2, #4 
    /*000069a4*/ ADD         R6, IP, R6, LSL R7 
    /*000069a8*/ ADD         R5, IP, R5, LSL R7 
    /*000069ac*/ ADD         LR, IP, LR, LSL R7 
    /*000069b0*/ ADD         R0, IP, R0, LSL R7 
    /*000069b4*/ LSL         R7, R2, R7 
    /*000069b8*/ MOV         R2, #0 
    AO_DRV_PutLastData_x69bc:
    /*000069bc*/ ADD         R1, R1, #4 
    /*000069c0*/ STRB        R3, [R0, R2] 
    /*000069c4*/ CMP         R4, R1 
    /*000069c8*/ STRB        R3, [R6, R2] 
    /*000069cc*/ STRB        R3, [R5, R2] 
    /*000069d0*/ STRB        R3, [LR, R2] 
    /*000069d4*/ ADD         R2, R2, R7 
    /*000069d8*/ BHI         AO_DRV_PutLastData_x69bc
    /*000069dc*/ B           AO_DRV_PutLastData_x6890
    AO_DRV_PutLastData_x69e0:
    /*000069e0*/ CMP         R1, R4 
    /*000069e4*/ SUBNE       R2, R4, #1 
    /*000069e8*/ ADDNE       R6, R3, IP 
    /*000069ec*/ MOVNE       R3, R2 
    /*000069f0*/ SUBNE       R1, R3, R1 
    /*000069f4*/ BEQ         AO_DRV_PutLastData_x6c84
    AO_DRV_PutLastData_x69f8:
    /*000069f8*/ LSL         R1, R1, R7 
    /*000069fc*/ CMP         R0, #0 
    /*00006a00*/ LDRB        R1, [R6, R1] 
    /*00006a04*/ BEQ         AO_DRV_PutLastData_x6ce0
    /*00006a08*/ MOV         R3, #1 
    /*00006a0c*/ MOV         R2, R5 
    /*00006a10*/ LSL         IP, R3, R7 
    /*00006a14*/ MOV         R3, #0 
    AO_DRV_PutLastData_x6a18:
    /*00006a18*/ ADD         R3, R3, #1 
    /*00006a1c*/ STRB        R1, [R2], IP 
    /*00006a20*/ CMP         R3, R0 
    /*00006a24*/ BNE         AO_DRV_PutLastData_x6a18
    AO_DRV_PutLastData_x6a28:
    /*00006a28*/ CMP         R4, R3 
    /*00006a2c*/ BLS         AO_DRV_PutLastData_x6890
    /*00006a30*/ ADD         LR, R3, #1 
    /*00006a34*/ ADD         IP, R3, #2 
    /*00006a38*/ ADD         R0, R3, #3 
    /*00006a3c*/ MOV         R2, #4 
    /*00006a40*/ ADD         LR, R5, LR, LSL R7 
    /*00006a44*/ ADD         IP, R5, IP, LSL R7 
    /*00006a48*/ ADD         R0, R5, R0, LSL R7 
    /*00006a4c*/ ADD         R5, R5, R3, LSL R7 
    /*00006a50*/ LSL         R7, R2, R7 
    /*00006a54*/ MOV         R2, #0 
    AO_DRV_PutLastData_x6a58:
    /*00006a58*/ ADD         R3, R3, #4 
    /*00006a5c*/ STRB        R1, [R5, R2] 
    /*00006a60*/ CMP         R4, R3 
    /*00006a64*/ STRB        R1, [LR, R2] 
    /*00006a68*/ STRB        R1, [IP, R2] 
    /*00006a6c*/ STRB        R1, [R0, R2] 
    /*00006a70*/ ADD         R2, R2, R7 
    /*00006a74*/ BHI         AO_DRV_PutLastData_x6a58
    /*00006a78*/ B           AO_DRV_PutLastData_x6890
    AO_DRV_PutLastData_x6a7c:
    /*00006a7c*/ LSL         R3, R3, #1 
    /*00006a80*/ AND         R0, R4, #3 
    /*00006a84*/ ADD         R5, R5, R3 
    /*00006a88*/ ADD         R6, R9, R3 
    AO_DRV_PutLastData_x6a8c:
    /*00006a8c*/ SUB         R1, R4, #1 
    AO_DRV_PutLastData_x6a90:
    /*00006a90*/ LSL         R1, R1, R7 
    /*00006a94*/ CMP         R0, #0 
    /*00006a98*/ LDRH        R1, [R6, R1] 
    /*00006a9c*/ BEQ         AO_DRV_PutLastData_x6cc8
    /*00006aa0*/ MOV         R3, #1 
    /*00006aa4*/ MOV         R2, R5 
    /*00006aa8*/ LSL         IP, R3, R7 
    /*00006aac*/ MOV         R3, #0 
    AO_DRV_PutLastData_x6ab0:
    /*00006ab0*/ ADD         R3, R3, #1 
    /*00006ab4*/ STRH        R1, [R2], IP 
    /*00006ab8*/ CMP         R3, R0 
    /*00006abc*/ BNE         AO_DRV_PutLastData_x6ab0
    AO_DRV_PutLastData_x6ac0:
    /*00006ac0*/ CMP         R4, R3 
    /*00006ac4*/ BLS         AO_DRV_PutLastData_x6890
    /*00006ac8*/ ADD         LR, R3, #1 
    /*00006acc*/ ADD         IP, R3, #2 
    /*00006ad0*/ ADD         R0, R3, #3 
    /*00006ad4*/ MOV         R2, #4 
    /*00006ad8*/ ADD         LR, R5, LR, LSL R7 
    /*00006adc*/ ADD         IP, R5, IP, LSL R7 
    /*00006ae0*/ ADD         R0, R5, R0, LSL R7 
    /*00006ae4*/ ADD         R5, R5, R3, LSL R7 
    /*00006ae8*/ LSL         R7, R2, R7 
    /*00006aec*/ MOV         R2, #0 
    AO_DRV_PutLastData_x6af0:
    /*00006af0*/ ADD         R3, R3, #4 
    /*00006af4*/ STRH        R1, [R5, R2] 
    /*00006af8*/ CMP         R4, R3 
    /*00006afc*/ STRH        R1, [LR, R2] 
    /*00006b00*/ STRH        R1, [IP, R2] 
    /*00006b04*/ STRH        R1, [R0, R2] 
    /*00006b08*/ ADD         R2, R2, R7 
    /*00006b0c*/ BHI         AO_DRV_PutLastData_x6af0
    /*00006b10*/ B           AO_DRV_PutLastData_x6890
    AO_DRV_PutLastData_x6b14:
    /*00006b14*/ CMP         R1, R4 
    /*00006b18*/ LSL         IP, IP, #1 
    /*00006b1c*/ ADD         R5, R5, IP 
    /*00006b20*/ ADD         R6, R9, IP 
    /*00006b24*/ MOVCS       R0, #0 
    /*00006b28*/ MOVCC       R0, #1 
    /*00006b2c*/ CMP         R1, R2 
    /*00006b30*/ MOVNE       R0, #0 
    /*00006b34*/ CMP         R0, #0 
    /*00006b38*/ AND         R0, R1, #3 
    /*00006b3c*/ BEQ         AO_DRV_PutLastData_x6c5c
    /*00006b40*/ SUB         R3, R4, #1 
    /*00006b44*/ CMP         R0, #0 
    /*00006b48*/ LSL         R3, R3, R7 
    /*00006b4c*/ LDRH        R3, [R6, R3] 
    /*00006b50*/ BEQ         AO_DRV_PutLastData_x6cc0
    /*00006b54*/ LSL         R8, R8, R7 
    /*00006b58*/ MOV         LR, R5 
    /*00006b5c*/ MOV         R2, #0 
    AO_DRV_PutLastData_x6b60:
    /*00006b60*/ ADD         R2, R2, #1 
    /*00006b64*/ STRH        R3, [LR], R8 
    /*00006b68*/ CMP         R0, R2 
    /*00006b6c*/ BNE         AO_DRV_PutLastData_x6b60
    AO_DRV_PutLastData_x6b70:
    /*00006b70*/ CMP         R1, R2 
    /*00006b74*/ BLS         AO_DRV_PutLastData_x6bc0
    /*00006b78*/ ADD         R8, R2, #1 
    /*00006b7c*/ ADD         R6, R2, #2 
    /*00006b80*/ ADD         LR, R2, #3 
    /*00006b84*/ MOV         R9, #4 
    /*00006b88*/ ADD         R8, R5, R8, LSL R7 
    /*00006b8c*/ ADD         R6, R5, R6, LSL R7 
    /*00006b90*/ ADD         LR, R5, LR, LSL R7 
    /*00006b94*/ LSL         R9, R9, R7 
    /*00006b98*/ ADD         R5, R5, R2, LSL R7 
    /*00006b9c*/ MOV         R0, #0 
    AO_DRV_PutLastData_x6ba0:
    /*00006ba0*/ ADD         R2, R2, #4 
    /*00006ba4*/ STRH        R3, [R5, R0] 
    /*00006ba8*/ CMP         R1, R2 
    /*00006bac*/ STRH        R3, [R8, R0] 
    /*00006bb0*/ STRH        R3, [R6, R0] 
    /*00006bb4*/ STRH        R3, [LR, R0] 
    /*00006bb8*/ ADD         R0, R0, R9 
    /*00006bbc*/ BHI         AO_DRV_PutLastData_x6ba0
    AO_DRV_PutLastData_x6bc0:
    /*00006bc0*/ LDR         R2, [SP, #12] 
    /*00006bc4*/ MOV         FP, #184                    @ 0xb8 
    /*00006bc8*/ LDR         R0, [SP, #8] 
    /*00006bcc*/ SUB         LR, R4, R1 
    /*00006bd0*/ ANDS        LR, LR, #3 
    /*00006bd4*/ MLA         FP, FP, R2, R0 
    /*00006bd8*/ LDR         R2, [FP, #16] 
    /*00006bdc*/ ADD         IP, R2, IP 
    /*00006be0*/ MOVNE       R2, #1 
    /*00006be4*/ LSLNE       R5, R2, R7 
    /*00006be8*/ MOVNE       R2, #0 
    /*00006bec*/ MOVNE       R0, IP 
    /*00006bf0*/ BEQ         AO_DRV_PutLastData_x6c04
    AO_DRV_PutLastData_x6bf4:
    /*00006bf4*/ ADD         R2, R2, #1 
    /*00006bf8*/ STRH        R3, [R0], R5 
    /*00006bfc*/ CMP         LR, R2 
    /*00006c00*/ BNE         AO_DRV_PutLastData_x6bf4
    AO_DRV_PutLastData_x6c04:
    /*00006c04*/ ADD         R1, R1, LR 
    /*00006c08*/ CMP         R4, R1 
    /*00006c0c*/ BLS         AO_DRV_PutLastData_x6890
    /*00006c10*/ ADD         R6, LR, #1 
    /*00006c14*/ ADD         R5, LR, #2 
    /*00006c18*/ ADD         R0, LR, #3 
    /*00006c1c*/ MOV         R2, #4 
    /*00006c20*/ ADD         R6, IP, R6, LSL R7 
    /*00006c24*/ ADD         R5, IP, R5, LSL R7 
    /*00006c28*/ ADD         R0, IP, R0, LSL R7 
    /*00006c2c*/ ADD         LR, IP, LR, LSL R7 
    /*00006c30*/ LSL         R7, R2, R7 
    /*00006c34*/ MOV         R2, #0 
    AO_DRV_PutLastData_x6c38:
    /*00006c38*/ ADD         R1, R1, #4 
    /*00006c3c*/ STRH        R3, [LR, R2] 
    /*00006c40*/ CMP         R4, R1 
    /*00006c44*/ STRH        R3, [R6, R2] 
    /*00006c48*/ STRH        R3, [R5, R2] 
    /*00006c4c*/ STRH        R3, [R0, R2] 
    /*00006c50*/ ADD         R2, R2, R7 
    /*00006c54*/ BHI         AO_DRV_PutLastData_x6c38
    /*00006c58*/ B           AO_DRV_PutLastData_x6890
    AO_DRV_PutLastData_x6c5c:
    /*00006c5c*/ CMP         R1, R4 
    /*00006c60*/ SUBNE       R2, R4, #1 
    /*00006c64*/ ADDNE       R6, R3, IP 
    /*00006c68*/ MOVNE       R3, R2 
    /*00006c6c*/ SUBNE       R1, R3, R1 
    /*00006c70*/ BNE         AO_DRV_PutLastData_x6a90
    /*00006c74*/ B           AO_DRV_PutLastData_x6a8c
    AO_DRV_PutLastData_x6c78:
    /*00006c78*/ ADD         R5, R5, R3 
    /*00006c7c*/ ADD         R6, R9, R3 
    /*00006c80*/ AND         R0, R4, #3 
    AO_DRV_PutLastData_x6c84:
    /*00006c84*/ SUB         R1, R4, #1 
    /*00006c88*/ B           AO_DRV_PutLastData_x69f8
    AO_DRV_PutLastData_x6c8c:
    /*00006c8c*/ MOV         R3, #224                    @ 0xe0 
    /*00006c90*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x183c] 
    /*00006c94*/ STR         R3, [SP] 
    /*00006c98*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x183c] 
    /*00006c9c*/ LDR         R3, [PC, #112]              @ 0x0000000000006d14 
    /*00006ca0*/ MOV         R1, #22 
    /*00006ca4*/ MOV         R0, #3 
    /*00006ca8*/ BL          0x0000000000006CA8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006cac*/ MOVW        R0, #32771                  @ 0x8003 
    /*00006cb0*/ MOVT        R0, #40982                  @ 0xa016 
    /*00006cb4*/ B           AO_DRV_PutLastData_x685c
    AO_DRV_PutLastData_x6cb8:
    /*00006cb8*/ MOV         R3, R0 
    /*00006cbc*/ B           AO_DRV_PutLastData_x67cc
    AO_DRV_PutLastData_x6cc0:
    /*00006cc0*/ MOV         R2, R0 
    /*00006cc4*/ B           AO_DRV_PutLastData_x6b70
    AO_DRV_PutLastData_x6cc8:
    /*00006cc8*/ MOV         R3, R0 
    /*00006ccc*/ B           AO_DRV_PutLastData_x6ac0
    AO_DRV_PutLastData_x6cd0:
    /*00006cd0*/ MOV         R2, R0 
    /*00006cd4*/ B           AO_DRV_PutLastData_x68f8
    AO_DRV_PutLastData_x6cd8:
    /*00006cd8*/ MOV         R2, R0 
    /*00006cdc*/ B           AO_DRV_PutLastData_x6678
    AO_DRV_PutLastData_x6ce0:
    /*00006ce0*/ MOV         R3, R0 
    /*00006ce4*/ B           AO_DRV_PutLastData_x6a28
    AO_DRV_PutLastData_x6ce8:
    /*00006ce8*/ MOV         R3, #219                    @ 0xdb 
    /*00006cec*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1800] 
    /*00006cf0*/ STR         R3, [SP] 
    /*00006cf4*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1800] 
    /*00006cf8*/ LDR         R3, [PC, #20]               @ 0x0000000000006d14 
    /*00006cfc*/ MOV         R1, #22 
    /*00006d00*/ MOV         R0, #3 
    /*00006d04*/ BL          0x0000000000006D04          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006d08*/ MOVW        R0, #32773                  @ 0x8005 
    /*00006d0c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00006d10*/ B           AO_DRV_PutLastData_x685c
    /*00006d14*/ ANDEQ       R0, R0, R8, LSL #4 
FUNC_END AO_DRV_PutLastData


FUNC_BEGIN AO_DRV_FlushData
    /*00006d18*/ MOV         R3, #184                    @ 0xb8 
    /*00006d1c*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00006d20*/ MUL         R0, R3, R0 
    /*00006d24*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00006d28*/ ADD         R3, R2, R0 
    /*00006d2c*/ LDR         R2, [R2, R0] 
    /*00006d30*/ LDR         R1, [R3, #8] 
    /*00006d34*/ LDR         R0, [R3, #16] 
    AO_DRV_FlushData_x6d38:
    /*00006d38*/ B           AO_DRV_FlushData_x6d38      @ hil_mmb_flush_dcache_byaddr [R_ARM_JUMP24: +0x0] 
FUNC_END AO_DRV_FlushData


FUNC_BEGIN AO_DRV_PutChnData
    /*00006d3c*/ MOV         IP, #184                    @ 0xb8 
    /*00006d40*/ MUL         IP, IP, R0 
    /*00006d44*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00006d48*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00006d4c*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00006d50*/ MOV         R4, R1 
    /*00006d54*/ ADD         R3, R6, IP 
    /*00006d58*/ MOV         R8, R2 
    /*00006d5c*/ SUB         SP, SP, #20 
    /*00006d60*/ LDR         R1, [R3, #16] 
    /*00006d64*/ LDR         R2, [R3, #24] 
    /*00006d68*/ STR         R0, [SP, #8] 
    /*00006d6c*/ ADDS        R9, R1, R2 
    /*00006d70*/ LDR         R5, [R3, #60]               @ 0x3c 
    /*00006d74*/ LDR         R7, [R3, #108]              @ 0x6c 
    /*00006d78*/ BEQ         AO_DRV_PutChnData_x7314
    /*00006d7c*/ LDR         R0, [R6, IP] 
    /*00006d80*/ ADD         R1, R1, R0 
    /*00006d84*/ CMP         R9, R1 
    /*00006d88*/ BHI         AO_DRV_PutChnData_x72e8
    /*00006d8c*/ LDR         SL, [R3, #40]               @ 0x28 
    /*00006d90*/ MOV         IP, #8 
    /*00006d94*/ LDR         R1, [R3, #64]               @ 0x40 
    /*00006d98*/ SUB         R0, R0, R2 
    /*00006d9c*/ LSL         IP, IP, SL 
    /*00006da0*/ LSL         R0, R0, #3 
    /*00006da4*/ MUL         R1, R1, IP 
    /*00006da8*/ BL          0x0000000000006DA8          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00006dac*/ LDR         R3, [SP, #8] 
    /*00006db0*/ MOV         R2, #46                     @ 0x2e 
    /*00006db4*/ MLA         R8, R2, R3, R8 
    /*00006db8*/ ADD         R8, R8, #30 
    /*00006dbc*/ LDR         R8, [R6, R8, LSL #2] 
    /*00006dc0*/ CMP         R5, R0 
    /*00006dc4*/ MOVCC       R0, R5 
    /*00006dc8*/ CMP         SL, #1 
    /*00006dcc*/ BEQ         AO_DRV_PutChnData_x710c
    /*00006dd0*/ BCC         AO_DRV_PutChnData_x6f8c
    /*00006dd4*/ CMP         SL, #2 
    /*00006dd8*/ BNE         AO_DRV_PutChnData_x72b0
    /*00006ddc*/ ANDS        LR, R0, #3 
    /*00006de0*/ LSL         R3, R8, #2 
    /*00006de4*/ ADD         R1, R9, R3 
    /*00006de8*/ STR         R3, [SP, #12] 
    /*00006dec*/ BEQ         AO_DRV_PutChnData_x6e1c
    /*00006df0*/ MOV         R9, #1 
    /*00006df4*/ LSL         R8, LR, #2 
    /*00006df8*/ LSL         R9, R9, R7 
    /*00006dfc*/ MOV         R2, R1 
    /*00006e00*/ MOV         R3, #0 
    AO_DRV_PutChnData_x6e04:
    /*00006e04*/ LDR         IP, [R4] 
    /*00006e08*/ LDR         IP, [IP, R3] 
    /*00006e0c*/ ADD         R3, R3, #4 
    /*00006e10*/ CMP         R8, R3 
    /*00006e14*/ STR         IP, [R2], R9 
    /*00006e18*/ BNE         AO_DRV_PutChnData_x6e04
    AO_DRV_PutChnData_x6e1c:
    /*00006e1c*/ CMP         R0, LR 
    /*00006e20*/ BLS         AO_DRV_PutChnData_x6ea0
    /*00006e24*/ ADD         SL, LR, #1 
    /*00006e28*/ ADD         R9, LR, #2 
    /*00006e2c*/ ADD         R8, LR, #3 
    /*00006e30*/ MOV         FP, #4 
    /*00006e34*/ ADD         SL, R1, SL, LSL R7 
    /*00006e38*/ ADD         R9, R1, R9, LSL R7 
    /*00006e3c*/ ADD         R8, R1, R8, LSL R7 
    /*00006e40*/ LSL         FP, FP, R7 
    /*00006e44*/ ADD         R1, R1, LR, LSL R7 
    /*00006e48*/ LSL         R2, LR, #2 
    /*00006e4c*/ MOV         R3, #0 
    AO_DRV_PutChnData_x6e50:
    /*00006e50*/ LDR         IP, [R4] 
    /*00006e54*/ ADD         LR, LR, #4 
    /*00006e58*/ CMP         R0, LR 
    /*00006e5c*/ LDR         IP, [IP, R2] 
    /*00006e60*/ STR         IP, [R1, R3] 
    /*00006e64*/ LDR         IP, [R4] 
    /*00006e68*/ ADD         IP, IP, R2 
    /*00006e6c*/ LDR         IP, [IP, #4] 
    /*00006e70*/ STR         IP, [SL, R3] 
    /*00006e74*/ LDR         IP, [R4] 
    /*00006e78*/ ADD         IP, IP, R2 
    /*00006e7c*/ LDR         IP, [IP, #8] 
    /*00006e80*/ STR         IP, [R9, R3] 
    /*00006e84*/ LDR         IP, [R4] 
    /*00006e88*/ ADD         IP, IP, R2 
    /*00006e8c*/ ADD         R2, R2, #16 
    /*00006e90*/ LDR         IP, [IP, #12] 
    /*00006e94*/ STR         IP, [R8, R3] 
    /*00006e98*/ ADD         R3, R3, FP 
    /*00006e9c*/ BHI         AO_DRV_PutChnData_x6e50
    AO_DRV_PutChnData_x6ea0:
    /*00006ea0*/ CMP         R5, R0 
    /*00006ea4*/ BLS         AO_DRV_PutChnData_x6f80
    /*00006ea8*/ LDR         R2, [SP, #8] 
    /*00006eac*/ MOV         R3, #184                    @ 0xb8 
    /*00006eb0*/ SUB         R8, R5, R0 
    /*00006eb4*/ ANDS        R9, R8, #3 
    /*00006eb8*/ MLA         R6, R3, R2, R6 
    /*00006ebc*/ LDR         R3, [SP, #12] 
    /*00006ec0*/ LDR         R8, [R6, #16] 
    /*00006ec4*/ ADD         R8, R8, R3 
    /*00006ec8*/ LSLEQ       R3, R0, #2 
    /*00006ecc*/ BEQ         AO_DRV_PutChnData_x6f08
    /*00006ed0*/ LSL         R2, R0, #2 
    /*00006ed4*/ ADD         R0, R0, R9 
    /*00006ed8*/ MOV         IP, #1 
    /*00006edc*/ MOV         R1, R8 
    /*00006ee0*/ LSL         R3, R0, #2 
    /*00006ee4*/ LSL         LR, IP, R7 
    AO_DRV_PutChnData_x6ee8:
    /*00006ee8*/ LDR         IP, [R4] 
    /*00006eec*/ LDR         IP, [IP, R2] 
    /*00006ef0*/ ADD         R2, R2, #4 
    /*00006ef4*/ CMP         R3, R2 
    /*00006ef8*/ STR         IP, [R1], LR 
    /*00006efc*/ BNE         AO_DRV_PutChnData_x6ee8
    /*00006f00*/ CMP         R5, R0 
    /*00006f04*/ BLS         AO_DRV_PutChnData_x6f80
    AO_DRV_PutChnData_x6f08:
    /*00006f08*/ ADD         R6, R9, #1 
    /*00006f0c*/ ADD         LR, R9, #2 
    /*00006f10*/ ADD         IP, R9, #3 
    /*00006f14*/ MOV         R2, #4 
    /*00006f18*/ ADD         R6, R8, R6, LSL R7 
    /*00006f1c*/ ADD         LR, R8, LR, LSL R7 
    /*00006f20*/ ADD         IP, R8, IP, LSL R7 
    /*00006f24*/ ADD         R8, R8, R9, LSL R7 
    /*00006f28*/ LSL         R7, R2, R7 
    /*00006f2c*/ MOV         R2, #0 
    AO_DRV_PutChnData_x6f30:
    /*00006f30*/ LDR         R1, [R4] 
    /*00006f34*/ ADD         R0, R0, #4 
    /*00006f38*/ CMP         R5, R0 
    /*00006f3c*/ LDR         R1, [R1, R3] 
    /*00006f40*/ STR         R1, [R8, R2] 
    /*00006f44*/ LDR         R1, [R4] 
    /*00006f48*/ ADD         R1, R1, R3 
    /*00006f4c*/ LDR         R1, [R1, #4] 
    /*00006f50*/ STR         R1, [R6, R2] 
    /*00006f54*/ LDR         R1, [R4] 
    /*00006f58*/ ADD         R1, R1, R3 
    /*00006f5c*/ LDR         R1, [R1, #8] 
    /*00006f60*/ STR         R1, [LR, R2] 
    /*00006f64*/ LDR         R1, [R4] 
    /*00006f68*/ ADD         R1, R1, R3 
    /*00006f6c*/ ADD         R3, R3, #16 
    /*00006f70*/ LDR         R1, [R1, #12] 
    /*00006f74*/ STR         R1, [IP, R2] 
    /*00006f78*/ ADD         R2, R2, R7 
    /*00006f7c*/ BHI         AO_DRV_PutChnData_x6f30
    AO_DRV_PutChnData_x6f80:
    /*00006f80*/ MOV         R0, #0 
    AO_DRV_PutChnData_x6f84:
    /*00006f84*/ ADD         SP, SP, #20 
    /*00006f88*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AO_DRV_PutChnData_x6f8c:
    /*00006f8c*/ ANDS        SL, R0, #3 
    /*00006f90*/ ADD         IP, R9, R8 
    /*00006f94*/ BEQ         AO_DRV_PutChnData_x72e0
    /*00006f98*/ MOV         R3, #1 
    /*00006f9c*/ MOV         R2, IP 
    /*00006fa0*/ LSL         R9, R3, R7 
    /*00006fa4*/ MOV         R3, #0 
    AO_DRV_PutChnData_x6fa8:
    /*00006fa8*/ LDR         R1, [R4] 
    /*00006fac*/ LDRB        R1, [R1, R3] 
    /*00006fb0*/ ADD         R3, R3, #1 
    /*00006fb4*/ CMP         SL, R3 
    /*00006fb8*/ STRB        R1, [R2], R9 
    /*00006fbc*/ BNE         AO_DRV_PutChnData_x6fa8
    AO_DRV_PutChnData_x6fc0:
    /*00006fc0*/ CMP         R3, R0 
    /*00006fc4*/ BCS         AO_DRV_PutChnData_x703c
    /*00006fc8*/ ADD         SL, R3, #1 
    /*00006fcc*/ ADD         R9, R3, #2 
    /*00006fd0*/ ADD         LR, R3, #3 
    /*00006fd4*/ MOV         FP, #4 
    /*00006fd8*/ ADD         SL, IP, SL, LSL R7 
    /*00006fdc*/ ADD         R9, IP, R9, LSL R7 
    /*00006fe0*/ ADD         LR, IP, LR, LSL R7 
    /*00006fe4*/ LSL         FP, FP, R7 
    /*00006fe8*/ ADD         IP, IP, R3, LSL R7 
    /*00006fec*/ MOV         R2, #0 
    AO_DRV_PutChnData_x6ff0:
    /*00006ff0*/ LDR         R1, [R4] 
    /*00006ff4*/ LDRB        R1, [R1, R3] 
    /*00006ff8*/ STRB        R1, [IP, R2] 
    /*00006ffc*/ LDR         R1, [R4] 
    /*00007000*/ ADD         R1, R1, R3 
    /*00007004*/ LDRB        R1, [R1, #1] 
    /*00007008*/ STRB        R1, [SL, R2] 
    /*0000700c*/ LDR         R1, [R4] 
    /*00007010*/ ADD         R1, R1, R3 
    /*00007014*/ LDRB        R1, [R1, #2] 
    /*00007018*/ STRB        R1, [R9, R2] 
    /*0000701c*/ LDR         R1, [R4] 
    /*00007020*/ ADD         R1, R1, R3 
    /*00007024*/ ADD         R3, R3, #4 
    /*00007028*/ CMP         R0, R3 
    /*0000702c*/ LDRB        R1, [R1, #3] 
    /*00007030*/ STRB        R1, [LR, R2] 
    /*00007034*/ ADD         R2, R2, FP 
    /*00007038*/ BHI         AO_DRV_PutChnData_x6ff0
    AO_DRV_PutChnData_x703c:
    /*0000703c*/ CMP         R5, R0 
    /*00007040*/ BLS         AO_DRV_PutChnData_x6f80
    /*00007044*/ LDR         R2, [SP, #8] 
    /*00007048*/ MOV         R3, #184                    @ 0xb8 
    /*0000704c*/ SUB         R9, R5, R0 
    /*00007050*/ ANDS        R9, R9, #3 
    /*00007054*/ MLA         R6, R3, R2, R6 
    /*00007058*/ LDR         R3, [R6, #16] 
    /*0000705c*/ ADD         R8, R3, R8 
    /*00007060*/ BEQ         AO_DRV_PutChnData_x7094
    /*00007064*/ MOV         R3, #1 
    /*00007068*/ ADD         R1, R0, R9 
    /*0000706c*/ LSL         IP, R3, R7 
    /*00007070*/ MOV         R3, R8 
    AO_DRV_PutChnData_x7074:
    /*00007074*/ LDR         R2, [R4] 
    /*00007078*/ LDRB        R2, [R2, R0] 
    /*0000707c*/ ADD         R0, R0, #1 
    /*00007080*/ CMP         R1, R0 
    /*00007084*/ STRB        R2, [R3], IP 
    /*00007088*/ BNE         AO_DRV_PutChnData_x7074
    /*0000708c*/ CMP         R5, R0 
    /*00007090*/ BLS         AO_DRV_PutChnData_x6f80
    AO_DRV_PutChnData_x7094:
    /*00007094*/ ADD         LR, R9, #1 
    /*00007098*/ ADD         IP, R9, #2 
    /*0000709c*/ ADD         R1, R9, #3 
    /*000070a0*/ MOV         R3, #4 
    /*000070a4*/ ADD         LR, R8, LR, LSL R7 
    /*000070a8*/ ADD         IP, R8, IP, LSL R7 
    /*000070ac*/ ADD         R1, R8, R1, LSL R7 
    /*000070b0*/ ADD         R8, R8, R9, LSL R7 
    /*000070b4*/ LSL         R7, R3, R7 
    /*000070b8*/ MOV         R3, #0 
    AO_DRV_PutChnData_x70bc:
    /*000070bc*/ LDR         R2, [R4] 
    /*000070c0*/ LDRB        R2, [R2, R0] 
    /*000070c4*/ STRB        R2, [R8, R3] 
    /*000070c8*/ LDR         R2, [R4] 
    /*000070cc*/ ADD         R2, R2, R0 
    /*000070d0*/ LDRB        R2, [R2, #1] 
    /*000070d4*/ STRB        R2, [LR, R3] 
    /*000070d8*/ LDR         R2, [R4] 
    /*000070dc*/ ADD         R2, R2, R0 
    /*000070e0*/ LDRB        R2, [R2, #2] 
    /*000070e4*/ STRB        R2, [IP, R3] 
    /*000070e8*/ LDR         R2, [R4] 
    /*000070ec*/ ADD         R2, R2, R0 
    /*000070f0*/ ADD         R0, R0, #4 
    /*000070f4*/ CMP         R5, R0 
    /*000070f8*/ LDRB        R2, [R2, #3] 
    /*000070fc*/ STRB        R2, [R1, R3] 
    /*00007100*/ ADD         R3, R3, R7 
    /*00007104*/ BHI         AO_DRV_PutChnData_x70bc
    /*00007108*/ B           AO_DRV_PutChnData_x6f80
    AO_DRV_PutChnData_x710c:
    /*0000710c*/ ANDS        LR, R0, #3 
    /*00007110*/ LSL         R3, R8, #1 
    /*00007114*/ ADD         R1, R9, R3 
    /*00007118*/ STR         R3, [SP, #12] 
    /*0000711c*/ BEQ         AO_DRV_PutChnData_x7148
    /*00007120*/ LSL         SL, SL, R7 
    /*00007124*/ LSL         R8, LR, #1 
    /*00007128*/ MOV         R2, R1 
    /*0000712c*/ MOV         R3, #0 
    AO_DRV_PutChnData_x7130:
    /*00007130*/ LDR         IP, [R4] 
    /*00007134*/ LDRH        IP, [IP, R3] 
    /*00007138*/ ADD         R3, R3, #2 
    /*0000713c*/ CMP         R8, R3 
    /*00007140*/ STRH        IP, [R2], SL 
    /*00007144*/ BNE         AO_DRV_PutChnData_x7130
    AO_DRV_PutChnData_x7148:
    /*00007148*/ CMP         LR, R0 
    /*0000714c*/ BCS         AO_DRV_PutChnData_x71cc
    /*00007150*/ ADD         SL, LR, #1 
    /*00007154*/ ADD         R9, LR, #2 
    /*00007158*/ ADD         R8, LR, #3 
    /*0000715c*/ MOV         FP, #4 
    /*00007160*/ ADD         SL, R1, SL, LSL R7 
    /*00007164*/ ADD         R9, R1, R9, LSL R7 
    /*00007168*/ ADD         R8, R1, R8, LSL R7 
    /*0000716c*/ LSL         FP, FP, R7 
    /*00007170*/ ADD         R1, R1, LR, LSL R7 
    /*00007174*/ LSL         R2, LR, #1 
    /*00007178*/ MOV         R3, #0 
    AO_DRV_PutChnData_x717c:
    /*0000717c*/ LDR         IP, [R4] 
    /*00007180*/ ADD         LR, LR, #4 
    /*00007184*/ CMP         R0, LR 
    /*00007188*/ LDRH        IP, [IP, R2] 
    /*0000718c*/ STRH        IP, [R1, R3] 
    /*00007190*/ LDR         IP, [R4] 
    /*00007194*/ ADD         IP, IP, R2 
    /*00007198*/ LDRH        IP, [IP, #2] 
    /*0000719c*/ STRH        IP, [SL, R3] 
    /*000071a0*/ LDR         IP, [R4] 
    /*000071a4*/ ADD         IP, IP, R2 
    /*000071a8*/ LDRH        IP, [IP, #4] 
    /*000071ac*/ STRH        IP, [R9, R3] 
    /*000071b0*/ LDR         IP, [R4] 
    /*000071b4*/ ADD         IP, IP, R2 
    /*000071b8*/ ADD         R2, R2, #8 
    /*000071bc*/ LDRH        IP, [IP, #6] 
    /*000071c0*/ STRH        IP, [R8, R3] 
    /*000071c4*/ ADD         R3, R3, FP 
    /*000071c8*/ BHI         AO_DRV_PutChnData_x717c
    AO_DRV_PutChnData_x71cc:
    /*000071cc*/ CMP         R5, R0 
    /*000071d0*/ BLS         AO_DRV_PutChnData_x6f80
    /*000071d4*/ LDR         R2, [SP, #8] 
    /*000071d8*/ MOV         R3, #184                    @ 0xb8 
    /*000071dc*/ SUB         R8, R5, R0 
    /*000071e0*/ ANDS        R9, R8, #3 
    /*000071e4*/ MLA         R6, R3, R2, R6 
    /*000071e8*/ LDR         R3, [SP, #12] 
    /*000071ec*/ LDR         R8, [R6, #16] 
    /*000071f0*/ ADD         R8, R8, R3 
    /*000071f4*/ LSLEQ       R3, R0, #1 
    /*000071f8*/ BEQ         AO_DRV_PutChnData_x7234
    /*000071fc*/ LSL         R2, R0, #1 
    /*00007200*/ ADD         R0, R0, R9 
    /*00007204*/ MOV         IP, #1 
    /*00007208*/ MOV         R1, R8 
    /*0000720c*/ LSL         R3, R0, IP 
    /*00007210*/ LSL         LR, IP, R7 
    AO_DRV_PutChnData_x7214:
    /*00007214*/ LDR         IP, [R4] 
    /*00007218*/ LDRH        IP, [IP, R2] 
    /*0000721c*/ ADD         R2, R2, #2 
    /*00007220*/ CMP         R3, R2 
    /*00007224*/ STRH        IP, [R1], LR 
    /*00007228*/ BNE         AO_DRV_PutChnData_x7214
    /*0000722c*/ CMP         R5, R0 
    /*00007230*/ BLS         AO_DRV_PutChnData_x6f80
    AO_DRV_PutChnData_x7234:
    /*00007234*/ ADD         R6, R9, #1 
    /*00007238*/ ADD         LR, R9, #2 
    /*0000723c*/ ADD         IP, R9, #3 
    /*00007240*/ MOV         R2, #4 
    /*00007244*/ ADD         R6, R8, R6, LSL R7 
    /*00007248*/ ADD         LR, R8, LR, LSL R7 
    /*0000724c*/ ADD         IP, R8, IP, LSL R7 
    /*00007250*/ ADD         R8, R8, R9, LSL R7 
    /*00007254*/ LSL         R7, R2, R7 
    /*00007258*/ MOV         R2, #0 
    AO_DRV_PutChnData_x725c:
    /*0000725c*/ LDR         R1, [R4] 
    /*00007260*/ ADD         R0, R0, #4 
    /*00007264*/ CMP         R5, R0 
    /*00007268*/ LDRH        R1, [R1, R3] 
    /*0000726c*/ STRH        R1, [R8, R2] 
    /*00007270*/ LDR         R1, [R4] 
    /*00007274*/ ADD         R1, R1, R3 
    /*00007278*/ LDRH        R1, [R1, #2] 
    /*0000727c*/ STRH        R1, [R6, R2] 
    /*00007280*/ LDR         R1, [R4] 
    /*00007284*/ ADD         R1, R1, R3 
    /*00007288*/ LDRH        R1, [R1, #4] 
    /*0000728c*/ STRH        R1, [LR, R2] 
    /*00007290*/ LDR         R1, [R4] 
    /*00007294*/ ADD         R1, R1, R3 
    /*00007298*/ ADD         R3, R3, #8 
    /*0000729c*/ LDRH        R1, [R1, #6] 
    /*000072a0*/ STRH        R1, [IP, R2] 
    /*000072a4*/ ADD         R2, R2, R7 
    /*000072a8*/ BHI         AO_DRV_PutChnData_x725c
    /*000072ac*/ B           AO_DRV_PutChnData_x6f80
    AO_DRV_PutChnData_x72b0:
    /*000072b0*/ MOVW        R3, #662                    @ 0x296 
    /*000072b4*/ MOV         R1, #22 
    /*000072b8*/ STR         R3, [SP] 
    /*000072bc*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1870] 
    /*000072c0*/ LDR         R3, [PC, #120]              @ 0x0000000000007340 
    /*000072c4*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1870] 
    /*000072c8*/ MOV         R0, #6 
    /*000072cc*/ BL          0x00000000000072CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000072d0*/ MOVW        R0, #32776                  @ 0x8008 
    /*000072d4*/ MOVT        R0, #40982                  @ 0xa016 
    /*000072d8*/ ADD         SP, SP, #20 
    /*000072dc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AO_DRV_PutChnData_x72e0:
    /*000072e0*/ MOV         R3, SL 
    /*000072e4*/ B           AO_DRV_PutChnData_x6fc0
    AO_DRV_PutChnData_x72e8:
    /*000072e8*/ MOVW        R3, #530                    @ 0x212 
    /*000072ec*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x183c] 
    /*000072f0*/ STR         R3, [SP] 
    /*000072f4*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x183c] 
    /*000072f8*/ LDR         R3, [PC, #64]               @ 0x0000000000007340 
    /*000072fc*/ MOV         R1, #22 
    /*00007300*/ MOV         R0, #3 
    /*00007304*/ BL          0x0000000000007304          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007308*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000730c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00007310*/ B           AO_DRV_PutChnData_x6f84
    AO_DRV_PutChnData_x7314:
    /*00007314*/ MOVW        R3, #525                    @ 0x20d 
    /*00007318*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1800] 
    /*0000731c*/ STR         R3, [SP] 
    /*00007320*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1800] 
    /*00007324*/ LDR         R3, [PC, #20]               @ 0x0000000000007340 
    /*00007328*/ MOV         R1, #22 
    /*0000732c*/ MOV         R0, #3 
    /*00007330*/ BL          0x0000000000007330          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007334*/ MOVW        R0, #32773                  @ 0x8005 
    /*00007338*/ MOVT        R0, #40982                  @ 0xa016 
    /*0000733c*/ B           AO_DRV_PutChnData_x6f84
    /*00007340*/ ANDEQ       R0, R0, IP, LSL R2 
FUNC_END AO_DRV_PutChnData


FUNC_BEGIN AO_DRV_DmaBufInfoInit
    /*00007344*/ MOVW        IP, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00007348*/ MOVT        IP, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*0000734c*/ MOV         R2, #184                    @ 0xb8 
    /*00007350*/ MOV         R3, #8 
    /*00007354*/ MLA         R2, R2, R0, IP 
    /*00007358*/ PUSH        {R4, R5, LR} 
    /*0000735c*/ SUB         SP, SP, #12 
    /*00007360*/ LDR         R4, [R2, #40]               @ 0x28 
    /*00007364*/ LDR         LR, [R2, #64]               @ 0x40 
    /*00007368*/ LSL         R3, R3, R4 
    /*0000736c*/ MUL         R3, LR, R3 
    /*00007370*/ CMP         R3, #0 
    /*00007374*/ ADD         R4, R3, #7 
    /*00007378*/ MOVLT       R3, R4 
    /*0000737c*/ CMP         LR, #0 
    /*00007380*/ ASR         R3, R3, #3 
    /*00007384*/ BEQ         AO_DRV_DmaBufInfoInit_x73ac
    /*00007388*/ CMP         LR, #1 
    /*0000738c*/ MOV         R4, #0 
    /*00007390*/ STR         R4, [R2, #120]              @ 0x78 
    /*00007394*/ BLS         AO_DRV_DmaBufInfoInit_x73ac
    /*00007398*/ CMP         LR, #2 
    /*0000739c*/ MOV         LR, #1 
    /*000073a0*/ STR         LR, [R2, #124]              @ 0x7c 
    /*000073a4*/ MOVNE       LR, #2 
    /*000073a8*/ STRNE       LR, [R2, #128]              @ 0x80 
    AO_DRV_DmaBufInfoInit_x73ac:
    /*000073ac*/ MOV         R2, #184                    @ 0xb8 
    /*000073b0*/ CMP         R3, #64                     @ 0x40 
    /*000073b4*/ MLA         R2, R2, R0, IP 
    /*000073b8*/ STR         R3, [R2, #104]              @ 0x68 
    /*000073bc*/ BEQ         AO_DRV_DmaBufInfoInit_x741c
    /*000073c0*/ BIC         R2, R3, #16 
    /*000073c4*/ CMP         R2, #32 
    /*000073c8*/ BEQ         AO_DRV_DmaBufInfoInit_x7470
    /*000073cc*/ CMP         R3, #16 
    /*000073d0*/ BEQ         AO_DRV_DmaBufInfoInit_x7480
    /*000073d4*/ CMP         R3, #8 
    /*000073d8*/ BEQ         AO_DRV_DmaBufInfoInit_x7478
    /*000073dc*/ CMP         R3, #4 
    /*000073e0*/ BEQ         AO_DRV_DmaBufInfoInit_x7488
    /*000073e4*/ CMP         R3, #2 
    /*000073e8*/ BEQ         AO_DRV_DmaBufInfoInit_x7490
    /*000073ec*/ STR         R3, [SP, #4] 
    /*000073f0*/ MOV         R1, #22 
    /*000073f4*/ MOVW        R3, #695                    @ 0x2b7 
    /*000073f8*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x189c] 
    /*000073fc*/ STR         R3, [SP] 
    /*00007400*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x189c] 
    /*00007404*/ LDR         R3, [PC, #140]              @ 0x0000000000007498 
    /*00007408*/ MOV         R0, #3 
    /*0000740c*/ BL          0x000000000000740C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007410*/ MVN         R0, #0 
    /*00007414*/ ADD         SP, SP, #12 
    /*00007418*/ POP         {R4, R5, PC} 
    AO_DRV_DmaBufInfoInit_x741c:
    /*0000741c*/ MOV         R5, #6 
    AO_DRV_DmaBufInfoInit_x7420:
    /*00007420*/ MOV         R2, #184                    @ 0xb8 
    /*00007424*/ MOV         R4, #0 
    /*00007428*/ MUL         R0, R2, R0 
    /*0000742c*/ ADD         LR, IP, R0 
    /*00007430*/ LDR         R2, [LR, #60]               @ 0x3c 
    /*00007434*/ STR         R5, [LR, #108]              @ 0x6c 
    /*00007438*/ STR         R4, [LR, #20] 
    /*0000743c*/ MUL         R3, R2, R3 
    /*00007440*/ LSL         R2, R3, #1 
    /*00007444*/ STR         R3, [LR, #116]              @ 0x74 
    /*00007448*/ ADD         R3, R2, R3 
    /*0000744c*/ STR         R2, [LR, #24] 
    /*00007450*/ TST         R3, #127                    @ 0x7f 
    /*00007454*/ BICNE       R3, R3, #127                @ 0x7f 
    /*00007458*/ ADDNE       R3, R3, #128                @ 0x80 
    /*0000745c*/ STR         R3, [IP, R0] 
    /*00007460*/ MOV         R0, #0 
    /*00007464*/ STR         R3, [R1] 
    /*00007468*/ ADD         SP, SP, #12 
    /*0000746c*/ POP         {R4, R5, PC} 
    AO_DRV_DmaBufInfoInit_x7470:
    /*00007470*/ MOV         R5, #5 
    /*00007474*/ B           AO_DRV_DmaBufInfoInit_x7420
    AO_DRV_DmaBufInfoInit_x7478:
    /*00007478*/ MOV         R5, #3 
    /*0000747c*/ B           AO_DRV_DmaBufInfoInit_x7420
    AO_DRV_DmaBufInfoInit_x7480:
    /*00007480*/ MOV         R5, #4 
    /*00007484*/ B           AO_DRV_DmaBufInfoInit_x7420
    AO_DRV_DmaBufInfoInit_x7488:
    /*00007488*/ MOV         R5, #2 
    /*0000748c*/ B           AO_DRV_DmaBufInfoInit_x7420
    AO_DRV_DmaBufInfoInit_x7490:
    /*00007490*/ MOV         R5, #1 
    /*00007494*/ B           AO_DRV_DmaBufInfoInit_x7420
    /*00007498*/ ANDEQ       R0, R0, R0, LSR R2 
FUNC_END AO_DRV_DmaBufInfoInit


FUNC_BEGIN AO_DRV_SetDmaBufAddr
    /*0000749c*/ MOVW        IP, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*000074a0*/ MOVT        IP, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*000074a4*/ MOV         R1, #184                    @ 0xb8 
    /*000074a8*/ MLA         R1, R1, R0, IP 
    /*000074ac*/ MOV         R0, #0 
    /*000074b0*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000074b4*/ LDR         LR, [SP, #4] 
    /*000074b8*/ STRD        R2, [R1, #8] 
    /*000074bc*/ STR         LR, [R1, #16] 
    /*000074c0*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END AO_DRV_SetDmaBufAddr


FUNC_BEGIN AO_DRV_ClrDmaBufAddr
    /*000074c4*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*000074c8*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*000074cc*/ MOV         R3, #184                    @ 0xb8 
    /*000074d0*/ MOV         R1, #0 
    /*000074d4*/ MLA         R3, R3, R0, R2 
    /*000074d8*/ MOV         R0, R1 
    /*000074dc*/ PUSH        {R4, R5} 
    /*000074e0*/ MOV         R4, #0 
    /*000074e4*/ MOV         R5, #0 
    /*000074e8*/ STR         R1, [R3, #16] 
    /*000074ec*/ STRD        R4, [R3, #8] 
    /*000074f0*/ POP         {R4, R5} 
    /*000074f4*/ BX          LR 
FUNC_END AO_DRV_ClrDmaBufAddr


FUNC_BEGIN AO_DRV_SetDevAttr
    /*000074f8*/ PUSH        {R4, R5, R6, LR} 
    /*000074fc*/ SUB         SP, SP, #32 
    /*00007500*/ MOV         R5, R0 
    /*00007504*/ MOV         R0, #2 
    /*00007508*/ MOV         R4, #20 
    /*0000750c*/ MOV         R2, #1 
    /*00007510*/ MOV         R3, #0 
    /*00007514*/ MOV         R6, R1 
    /*00007518*/ STR         R2, [SP, #12] 
    /*0000751c*/ STR         R3, [SP, #16] 
    /*00007520*/ STR         R5, [SP, #24] 
    /*00007524*/ STR         R4, [SP, #20] 
    /*00007528*/ BL          0x0000000000007528          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000752c*/ ADD         R2, SP, #12 
    /*00007530*/ MOV         R1, #32 
    /*00007534*/ LDR         R3, [R0, #20] 
    /*00007538*/ ADD         R0, SP, R4 
    /*0000753c*/ BLX         R3 
    /*00007540*/ MOV         R0, #2 
    /*00007544*/ BL          0x0000000000007544          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007548*/ ADD         R2, SP, #16 
    /*0000754c*/ MOV         R1, #31 
    /*00007550*/ LDR         R3, [R0, #20] 
    /*00007554*/ ADD         R0, SP, R4 
    /*00007558*/ BLX         R3 
    /*0000755c*/ MOV         R0, R4 
    /*00007560*/ BL          0x0000000000007560          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007564*/ SUBS        R3, R0, #0 
    /*00007568*/ BEQ         AO_DRV_SetDevAttr_x75d0
    /*0000756c*/ LDR         R3, [R3, #80]               @ 0x50 
    /*00007570*/ MOV         R1, R6 
    /*00007574*/ MOV         R0, R5 
    /*00007578*/ BLX         R3 
    /*0000757c*/ SUBS        R4, R0, #0 
    /*00007580*/ BNE         AO_DRV_SetDevAttr_x75c4
    /*00007584*/ MOV         R3, #184                    @ 0xb8 
    /*00007588*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*0000758c*/ MUL         R5, R3, R5 
    /*00007590*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00007594*/ MOV         R1, R6 
    /*00007598*/ MOV         R2, #40                     @ 0x28 
    /*0000759c*/ ADD         R3, R0, R5 
    /*000075a0*/ ADD         R5, R5, #36                 @ 0x24 
    /*000075a4*/ ADD         R0, R0, R5 
    /*000075a8*/ STR         R4, [R3, #144]              @ 0x90 
    /*000075ac*/ STR         R4, [R3, #152]              @ 0x98 
    /*000075b0*/ STR         R4, [R3, #156]              @ 0x9c 
    /*000075b4*/ STR         R4, [R3, #160]              @ 0xa0 
    /*000075b8*/ STR         R4, [R3, #164]              @ 0xa4 
    /*000075bc*/ STR         R4, [R3, #168]              @ 0xa8 
    /*000075c0*/ BL          0x00000000000075C0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    AO_DRV_SetDevAttr_x75c4:
    /*000075c4*/ MOV         R0, R4 
    /*000075c8*/ ADD         SP, SP, #32 
    /*000075cc*/ POP         {R4, R5, R6, PC} 
    AO_DRV_SetDevAttr_x75d0:
    /*000075d0*/ MOVW        R3, #763                    @ 0x2fb 
    /*000075d4*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18f0] 
    /*000075d8*/ STR         R3, [SP] 
    /*000075dc*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18f0] 
    /*000075e0*/ LDR         R3, [PC, #16]               @ 0x00000000000075f8 
    /*000075e4*/ MOV         R1, #22 
    /*000075e8*/ MOVW        R4, #32784                  @ 0x8010 
    /*000075ec*/ BL          0x00000000000075EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000075f0*/ MOVT        R4, #40982                  @ 0xa016 
    /*000075f4*/ B           AO_DRV_SetDevAttr_x75c4
    /*000075f8*/ ANDEQ       R0, R0, R8, ASR #4 
FUNC_END AO_DRV_SetDevAttr


FUNC_BEGIN AO_DRV_ClrDevAttr
    /*000075fc*/ PUSH        {R4, LR} 
    /*00007600*/ MOV         R4, R0 
    /*00007604*/ SUB         SP, SP, #8 
    /*00007608*/ MOV         R0, #20 
    /*0000760c*/ BL          0x000000000000760C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007610*/ SUBS        R3, R0, #0 
    /*00007614*/ BEQ         AO_DRV_ClrDevAttr_x762c
    /*00007618*/ LDR         R3, [R3, #132]              @ 0x84 
    /*0000761c*/ MOV         R0, R4 
    /*00007620*/ ADD         SP, SP, #8 
    /*00007624*/ POP         {R4, LR} 
    /*00007628*/ BX          R3 
    AO_DRV_ClrDevAttr_x762c:
    /*0000762c*/ MOVW        R3, #794                    @ 0x31a 
    /*00007630*/ MOV         R1, #22 
    /*00007634*/ STR         R3, [SP] 
    /*00007638*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1938] 
    /*0000763c*/ LDR         R3, [PC, #20]               @ 0x0000000000007658 
    /*00007640*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1938] 
    /*00007644*/ BL          0x0000000000007644          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007648*/ MOVW        R0, #32784                  @ 0x8010 
    /*0000764c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00007650*/ ADD         SP, SP, #8 
    /*00007654*/ POP         {R4, PC} 
    /*00007658*/ ANDEQ       R0, R0, IP, ASR R2 
FUNC_END AO_DRV_ClrDevAttr


FUNC_BEGIN AO_DRV_EnableDev
    /*0000765c*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00007660*/ MOV         R4, R0 
    /*00007664*/ SUB         SP, SP, #8 
    /*00007668*/ MOV         R0, #20 
    /*0000766c*/ BL          0x000000000000766C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007670*/ SUBS        R5, R0, #0 
    /*00007674*/ BEQ         AO_DRV_EnableDev_x7700
    /*00007678*/ MOV         R7, #184                    @ 0xb8 
    /*0000767c*/ MOVW        R8, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00007680*/ MUL         R7, R7, R4 
    /*00007684*/ MOVT        R8, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00007688*/ LDR         R1, [R5, #108]              @ 0x6c 
    /*0000768c*/ MOV         R0, R4 
    /*00007690*/ ADD         R6, R8, R7 
    /*00007694*/ LDRD        R2, [R6, #8] 
    /*00007698*/ BLX         R1 
    /*0000769c*/ LDR         R1, [R8, R7] 
    /*000076a0*/ LDR         R3, [R5, #112]              @ 0x70 
    /*000076a4*/ MOV         R0, R4 
    /*000076a8*/ BLX         R3 
    /*000076ac*/ LDR         R1, [R6, #24] 
    /*000076b0*/ LDR         R3, [R5, #92]               @ 0x5c 
    /*000076b4*/ MOV         R0, R4 
    /*000076b8*/ BIC         R1, R1, #15 
    /*000076bc*/ BLX         R3 
    /*000076c0*/ LDR         R3, [R5, #100]              @ 0x64 
    /*000076c4*/ MOV         R1, #0 
    /*000076c8*/ MOV         R0, R4 
    /*000076cc*/ BLX         R3 
    /*000076d0*/ LDR         R1, [R6, #116]              @ 0x74 
    /*000076d4*/ LDR         R3, [R5, #116]              @ 0x74 
    /*000076d8*/ MOV         R0, R4 
    /*000076dc*/ BLX         R3 
    /*000076e0*/ LDR         R3, [R5, #84]               @ 0x54 
    /*000076e4*/ MOV         R0, R4 
    /*000076e8*/ BLX         R3 
    /*000076ec*/ MOV         R0, #0 
    /*000076f0*/ MOV         R3, #1 
    /*000076f4*/ STR         R3, [R6, #76]               @ 0x4c 
    AO_DRV_EnableDev_x76f8:
    /*000076f8*/ ADD         SP, SP, #8 
    /*000076fc*/ POP         {R4, R5, R6, R7, R8, PC} 
    AO_DRV_EnableDev_x7700:
    /*00007700*/ MOV         R3, #816                    @ 0x330 
    /*00007704*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x197c] 
    /*00007708*/ STR         R3, [SP] 
    /*0000770c*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x197c] 
    /*00007710*/ LDR         R3, [PC, #20]               @ 0x000000000000772c 
    /*00007714*/ MOV         R1, #22 
    /*00007718*/ MOV         R0, #3 
    /*0000771c*/ BL          0x000000000000771C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007720*/ MOVW        R0, #32784                  @ 0x8010 
    /*00007724*/ MOVT        R0, #40982                  @ 0xa016 
    /*00007728*/ B           AO_DRV_EnableDev_x76f8
    /*0000772c*/ ANDEQ       R0, R0, R0, ROR R2 
FUNC_END AO_DRV_EnableDev


FUNC_BEGIN AO_DRV_DisableDev
    /*00007730*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00007734*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00007738*/ MOV         R3, #184                    @ 0xb8 
    /*0000773c*/ MLA         R3, R3, R0, R2 
    /*00007740*/ MOV         R2, #0 
    /*00007744*/ PUSH        {R4, LR} 
    /*00007748*/ MOV         R4, R0 
    /*0000774c*/ SUB         SP, SP, #8 
    /*00007750*/ MOV         R0, #20 
    /*00007754*/ STR         R2, [R3, #76]               @ 0x4c 
    /*00007758*/ STR         R2, [R3, #132]              @ 0x84 
    /*0000775c*/ STR         R2, [R3, #136]              @ 0x88 
    /*00007760*/ STR         R2, [R3, #140]              @ 0x8c 
    /*00007764*/ STR         R2, [R3, #88]               @ 0x58 
    /*00007768*/ STR         R2, [R3, #92]               @ 0x5c 
    /*0000776c*/ BL          0x000000000000776C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007770*/ CMP         R0, #0 
    /*00007774*/ BEQ         AO_DRV_DisableDev_x778c
    /*00007778*/ LDR         R3, [R0, #88]               @ 0x58 
    /*0000777c*/ MOV         R0, R4 
    /*00007780*/ ADD         SP, SP, #8 
    /*00007784*/ POP         {R4, LR} 
    /*00007788*/ BX          R3 
    AO_DRV_DisableDev_x778c:
    /*0000778c*/ MOVW        R3, #851                    @ 0x353 
    /*00007790*/ MOV         R1, #22 
    /*00007794*/ STR         R3, [SP] 
    /*00007798*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19b4] 
    /*0000779c*/ LDR         R3, [PC, #16]               @ 0x00000000000077b4 
    /*000077a0*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19b4] 
    /*000077a4*/ MOV         R0, #3 
    /*000077a8*/ BL          0x00000000000077A8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000077ac*/ ADD         SP, SP, #8 
    /*000077b0*/ POP         {R4, PC} 
    /*000077b4*/ ANDEQ       R0, R0, R4, LSL #5 
FUNC_END AO_DRV_DisableDev


FUNC_BEGIN AO_DRV_SetDmaISR
    /*000077b8*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*000077bc*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*000077c0*/ STR         R0, [R3, #368]              @ 0x170 
    /*000077c4*/ BX          LR 
FUNC_END AO_DRV_SetDmaISR


FUNC_BEGIN AO_DRV_DevInit
    /*000077c8*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*000077cc*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*000077d0*/ MOV         R2, #184                    @ 0xb8 
    /*000077d4*/ MOV         R1, #0 
    /*000077d8*/ PUSH        {R4, R5, LR} 
    /*000077dc*/ SUB         SP, SP, #12 
    /*000077e0*/ MLA         R4, R2, R0, R3 
    /*000077e4*/ MOV         R5, R1 
    /*000077e8*/ MOV         R0, R4 
    /*000077ec*/ BL          0x00000000000077EC          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000077f0*/ MOV         R0, #20 
    /*000077f4*/ STR         R5, [R4, #76]               @ 0x4c 
    /*000077f8*/ BL          0x00000000000077F8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000077fc*/ SUBS        R3, R0, #0 
    /*00007800*/ BEQ         AO_DRV_DevInit_x7820
    /*00007804*/ LDR         R3, [R3, #128]              @ 0x80 
    /*00007808*/ MOVW        R0, #0                      @ AO_DRV_ISR [R_ARM_MOVW_ABS_NC: .text+0x5da0] 
    /*0000780c*/ MOVT        R0, #0                      @ AO_DRV_ISR [R_ARM_MOVT_ABS: .text+0x5da0] 
    /*00007810*/ BLX         R3 
    /*00007814*/ MOV         R0, R5 
    AO_DRV_DevInit_x7818:
    /*00007818*/ ADD         SP, SP, #12 
    /*0000781c*/ POP         {R4, R5, PC} 
    AO_DRV_DevInit_x7820:
    /*00007820*/ MOVW        R3, #1041                   @ 0x411 
    /*00007824*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1728] 
    /*00007828*/ STR         R3, [SP] 
    /*0000782c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1728] 
    /*00007830*/ LDR         R3, [PC, #16]               @ 0x0000000000007848 
    /*00007834*/ MOV         R1, #22 
    /*00007838*/ BL          0x0000000000007838          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000783c*/ MOVW        R0, #32784                  @ 0x8010 
    /*00007840*/ MOVT        R0, #40981                  @ 0xa015 
    /*00007844*/ B           AO_DRV_DevInit_x7818
    /*00007848*/ MULEQ       R0, R8, R2 
FUNC_END AO_DRV_DevInit


FUNC_BEGIN AO_DRV_DevDeInit
    /*0000784c*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00007850*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00007854*/ MOV         R3, #184                    @ 0xb8 
    /*00007858*/ MOV         R1, #0 
    /*0000785c*/ MLA         R3, R3, R0, R2 
    /*00007860*/ MOV         R0, R1 
    /*00007864*/ STR         R1, [R3, #76]               @ 0x4c 
    /*00007868*/ BX          LR 
FUNC_END AO_DRV_DevDeInit


FUNC_BEGIN AO_DRV_CheckAioExportFunc
    /*0000786c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00007870*/ MOV         R0, #20 
    /*00007874*/ SUB         SP, SP, #12 
    /*00007878*/ BL          0x0000000000007878          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000787c*/ CMP         R0, #0 
    /*00007880*/ MOVNE       R0, #0 
    /*00007884*/ BEQ         AO_DRV_CheckAioExportFunc_x7890
    AO_DRV_CheckAioExportFunc_x7888:
    /*00007888*/ ADD         SP, SP, #12 
    /*0000788c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    AO_DRV_CheckAioExportFunc_x7890:
    /*00007890*/ MOVW        R3, #1064                   @ 0x428 
    /*00007894*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1728] 
    /*00007898*/ STR         R3, [SP] 
    /*0000789c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1728] 
    /*000078a0*/ LDR         R3, [PC, #16]               @ 0x00000000000078b8 
    /*000078a4*/ MOV         R1, #22 
    /*000078a8*/ BL          0x00000000000078A8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000078ac*/ MOVW        R0, #32784                  @ 0x8010 
    /*000078b0*/ MOVT        R0, #40982                  @ 0xa016 
    /*000078b4*/ B           AO_DRV_CheckAioExportFunc_x7888
    /*000078b8*/ ANDEQ       R0, R0, R8, LSR #5 
FUNC_END AO_DRV_CheckAioExportFunc


FUNC_BEGIN AO_DRV_OpenClock
    /*000078bc*/ MOV         R0, #0 
    /*000078c0*/ BX          LR 
FUNC_END AO_DRV_OpenClock


FUNC_BEGIN AO_DRV_CloseClock
    /*000078c4*/ MOV         R0, #0 
    /*000078c8*/ BX          LR 
FUNC_END AO_DRV_CloseClock


FUNC_BEGIN AO_DRV_SetTrackMode
    /*000078cc*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*000078d0*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*000078d4*/ PUSH        {R4, R5, R6, LR} 
    /*000078d8*/ MOV         R4, #184                    @ 0xb8 
    /*000078dc*/ MLA         R4, R4, R0, R3 
    /*000078e0*/ SUB         SP, SP, #8 
    /*000078e4*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*000078e8*/ CMP         R3, #1 
    /*000078ec*/ BHI         AO_DRV_SetTrackMode_x7928
    /*000078f0*/ MOV         R5, R0 
    /*000078f4*/ MOV         R0, #20 
    /*000078f8*/ MOV         R6, R1 
    /*000078fc*/ BL          0x00000000000078FC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007900*/ SUBS        R3, R0, #0 
    /*00007904*/ BEQ         AO_DRV_SetTrackMode_x7958
    /*00007908*/ LDR         R3, [R3, #136]              @ 0x88 
    /*0000790c*/ MOV         R0, R5 
    /*00007910*/ MOV         R1, R6 
    /*00007914*/ BLX         R3 
    /*00007918*/ CMP         R0, #0 
    /*0000791c*/ STREQ       R6, [R4, #144]              @ 0x90 
    AO_DRV_SetTrackMode_x7920:
    /*00007920*/ ADD         SP, SP, #8 
    /*00007924*/ POP         {R4, R5, R6, PC} 
    AO_DRV_SetTrackMode_x7928:
    /*00007928*/ MOVW        R3, #1090                   @ 0x442 
    /*0000792c*/ MOV         R1, #22 
    /*00007930*/ STR         R3, [SP] 
    /*00007934*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19e8] 
    /*00007938*/ LDR         R3, [PC, #64]               @ 0x0000000000007980 
    /*0000793c*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19e8] 
    /*00007940*/ MOV         R0, #3 
    /*00007944*/ BL          0x0000000000007944          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007948*/ MOVW        R0, #32776                  @ 0x8008 
    /*0000794c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00007950*/ ADD         SP, SP, #8 
    /*00007954*/ POP         {R4, R5, R6, PC} 
    AO_DRV_SetTrackMode_x7958:
    /*00007958*/ MOVW        R3, #1097                   @ 0x449 
    /*0000795c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1728] 
    /*00007960*/ STR         R3, [SP] 
    /*00007964*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1728] 
    /*00007968*/ LDR         R3, [PC, #16]               @ 0x0000000000007980 
    /*0000796c*/ MOV         R1, #22 
    /*00007970*/ BL          0x0000000000007970          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007974*/ MOVW        R0, #32784                  @ 0x8010 
    /*00007978*/ MOVT        R0, #40982                  @ 0xa016 
    /*0000797c*/ B           AO_DRV_SetTrackMode_x7920
    /*00007980*/ ANDEQ       R0, R0, R4, ASR #5 
FUNC_END AO_DRV_SetTrackMode


FUNC_BEGIN AO_DRV_GetTrackMode
    /*00007984*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00007988*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*0000798c*/ PUSH        {R4, R5, LR} 
    /*00007990*/ MOV         R4, #184                    @ 0xb8 
    /*00007994*/ MLA         R4, R4, R0, R3 
    /*00007998*/ SUB         SP, SP, #12 
    /*0000799c*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*000079a0*/ CMP         R3, #1 
    /*000079a4*/ BHI         AO_DRV_GetTrackMode_x79d0
    /*000079a8*/ MOV         R0, #20 
    /*000079ac*/ MOV         R5, R1 
    /*000079b0*/ BL          0x00000000000079B0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000079b4*/ CMP         R0, #0 
    /*000079b8*/ LDRNE       R3, [R4, #144]              @ 0x90 
    /*000079bc*/ MOVNE       R0, #0 
    /*000079c0*/ STRNE       R3, [R5] 
    /*000079c4*/ BEQ         AO_DRV_GetTrackMode_x7a00
    AO_DRV_GetTrackMode_x79c8:
    /*000079c8*/ ADD         SP, SP, #12 
    /*000079cc*/ POP         {R4, R5, PC} 
    AO_DRV_GetTrackMode_x79d0:
    /*000079d0*/ MOV         R3, #1120                   @ 0x460 
    /*000079d4*/ MOV         R1, #22 
    /*000079d8*/ STR         R3, [SP] 
    /*000079dc*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19e8] 
    /*000079e0*/ LDR         R3, [PC, #64]               @ 0x0000000000007a28 
    /*000079e4*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19e8] 
    /*000079e8*/ MOV         R0, #3 
    /*000079ec*/ BL          0x00000000000079EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000079f0*/ MOVW        R0, #32776                  @ 0x8008 
    /*000079f4*/ MOVT        R0, #40982                  @ 0xa016 
    /*000079f8*/ ADD         SP, SP, #12 
    /*000079fc*/ POP         {R4, R5, PC} 
    AO_DRV_GetTrackMode_x7a00:
    /*00007a00*/ MOVW        R3, #1126                   @ 0x466 
    /*00007a04*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1728] 
    /*00007a08*/ STR         R3, [SP] 
    /*00007a0c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1728] 
    /*00007a10*/ LDR         R3, [PC, #16]               @ 0x0000000000007a28 
    /*00007a14*/ MOV         R1, #22 
    /*00007a18*/ BL          0x0000000000007A18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007a1c*/ MOVW        R0, #32784                  @ 0x8010 
    /*00007a20*/ MOVT        R0, #40982                  @ 0xa016 
    /*00007a24*/ B           AO_DRV_GetTrackMode_x79c8
    /*00007a28*/ LDRDEQ      R0, [R0], -R8 
FUNC_END AO_DRV_GetTrackMode


FUNC_BEGIN AO_DRV_SetMute
    /*00007a2c*/ PUSH        {R4, R5, R6, LR} 
    /*00007a30*/ MOV         R4, R0 
    /*00007a34*/ SUB         SP, SP, #8 
    /*00007a38*/ MOV         R0, #20 
    /*00007a3c*/ MOV         R6, R1 
    /*00007a40*/ MOV         R5, R2 
    /*00007a44*/ BL          0x0000000000007A44          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007a48*/ SUBS        R3, R0, #0 
    /*00007a4c*/ BEQ         AO_DRV_SetMute_x7aa8
    /*00007a50*/ LDR         R3, [R3, #152]              @ 0x98 
    /*00007a54*/ MOV         R2, R5 
    /*00007a58*/ MOV         R1, R6 
    /*00007a5c*/ MOV         R0, R4 
    /*00007a60*/ BLX         R3 
    /*00007a64*/ CMP         R0, #0 
    /*00007a68*/ BNE         AO_DRV_SetMute_x7aa0
    /*00007a6c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00007a70*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00007a74*/ MOV         R2, #184                    @ 0xb8 
    /*00007a78*/ CMP         R5, #0 
    /*00007a7c*/ MLA         R3, R2, R4, R3 
    /*00007a80*/ STR         R6, [R3, #152]              @ 0x98 
    /*00007a84*/ BEQ         AO_DRV_SetMute_x7aa0
    /*00007a88*/ LDR         R2, [R5] 
    /*00007a8c*/ STR         R2, [R3, #156]              @ 0x9c 
    /*00007a90*/ LDR         R2, [R5, #4] 
    /*00007a94*/ STR         R2, [R3, #160]              @ 0xa0 
    /*00007a98*/ LDR         R2, [R5, #8] 
    /*00007a9c*/ STR         R2, [R3, #164]              @ 0xa4 
    AO_DRV_SetMute_x7aa0:
    /*00007aa0*/ ADD         SP, SP, #8 
    /*00007aa4*/ POP         {R4, R5, R6, PC} 
    AO_DRV_SetMute_x7aa8:
    /*00007aa8*/ MOVW        R3, #1144                   @ 0x478 
    /*00007aac*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1728] 
    /*00007ab0*/ STR         R3, [SP] 
    /*00007ab4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1728] 
    /*00007ab8*/ LDR         R3, [PC, #16]               @ 0x0000000000007ad0 
    /*00007abc*/ MOV         R1, #22 
    /*00007ac0*/ BL          0x0000000000007AC0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007ac4*/ MOVW        R0, #32784                  @ 0x8010 
    /*00007ac8*/ MOVT        R0, #40982                  @ 0xa016 
    /*00007acc*/ B           AO_DRV_SetMute_x7aa0
    /*00007ad0*/ ANDEQ       R0, R0, IP, ROR #5 
FUNC_END AO_DRV_SetMute


FUNC_BEGIN AO_DRV_GetMute
    /*00007ad4*/ PUSH        {R4, R5, R6, LR} 
    /*00007ad8*/ MOV         R4, R0 
    /*00007adc*/ SUB         SP, SP, #8 
    /*00007ae0*/ MOV         R0, #20 
    /*00007ae4*/ MOV         R6, R1 
    /*00007ae8*/ MOV         R5, R2 
    /*00007aec*/ BL          0x0000000000007AEC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007af0*/ CMP         R0, #0 
    /*00007af4*/ BEQ         AO_DRV_GetMute_x7b60
    /*00007af8*/ CMP         R6, #0 
    /*00007afc*/ BEQ         AO_DRV_GetMute_x7b18
    /*00007b00*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00007b04*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00007b08*/ MOV         R3, #184                    @ 0xb8 
    /*00007b0c*/ MLA         R3, R3, R4, R2 
    /*00007b10*/ LDR         R3, [R3, #152]              @ 0x98 
    /*00007b14*/ STR         R3, [R6] 
    AO_DRV_GetMute_x7b18:
    /*00007b18*/ CMP         R5, #0 
    /*00007b1c*/ BEQ         AO_DRV_GetMute_x7b54
    /*00007b20*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00007b24*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00007b28*/ MOV         R2, #184                    @ 0xb8 
    /*00007b2c*/ MOV         R0, #0 
    /*00007b30*/ MLA         R3, R2, R4, R3 
    /*00007b34*/ LDR         R2, [R3, #156]              @ 0x9c 
    /*00007b38*/ STR         R2, [R5] 
    /*00007b3c*/ LDR         R2, [R3, #160]              @ 0xa0 
    /*00007b40*/ STR         R2, [R5, #4] 
    /*00007b44*/ LDR         R3, [R3, #164]              @ 0xa4 
    /*00007b48*/ STR         R3, [R5, #8] 
    AO_DRV_GetMute_x7b4c:
    /*00007b4c*/ ADD         SP, SP, #8 
    /*00007b50*/ POP         {R4, R5, R6, PC} 
    AO_DRV_GetMute_x7b54:
    /*00007b54*/ MOV         R0, R5 
    /*00007b58*/ ADD         SP, SP, #8 
    /*00007b5c*/ POP         {R4, R5, R6, PC} 
    AO_DRV_GetMute_x7b60:
    /*00007b60*/ MOVW        R3, #1175                   @ 0x497 
    /*00007b64*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1728] 
    /*00007b68*/ STR         R3, [SP] 
    /*00007b6c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1728] 
    /*00007b70*/ LDR         R3, [PC, #16]               @ 0x0000000000007b88 
    /*00007b74*/ MOV         R1, #22 
    /*00007b78*/ BL          0x0000000000007B78          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007b7c*/ MOVW        R0, #32784                  @ 0x8010 
    /*00007b80*/ MOVT        R0, #40982                  @ 0xa016 
    /*00007b84*/ B           AO_DRV_GetMute_x7b4c
    /*00007b88*/ STRDEQ      R0, [R0], -IP 
FUNC_END AO_DRV_GetMute


FUNC_BEGIN AO_DRV_SetVolume
    /*00007b8c*/ PUSH        {R4, R5, LR} 
    /*00007b90*/ MOV         R4, R0 
    /*00007b94*/ SUB         SP, SP, #12 
    /*00007b98*/ MOV         R0, #20 
    /*00007b9c*/ MOV         R5, R1 
    /*00007ba0*/ BL          0x0000000000007BA0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007ba4*/ SUBS        R3, R0, #0 
    /*00007ba8*/ BEQ         AO_DRV_SetVolume_x7bdc
    /*00007bac*/ LDR         R3, [R3, #160]              @ 0xa0 
    /*00007bb0*/ MOV         R0, R4 
    /*00007bb4*/ MOV         R1, R5 
    /*00007bb8*/ BLX         R3 
    /*00007bbc*/ CMP         R0, #0 
    /*00007bc0*/ MOVWEQ      R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00007bc4*/ MOVTEQ      R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00007bc8*/ MOVEQ       R2, #184                    @ 0xb8 
    /*00007bcc*/ MLAEQ       R4, R2, R4, R3 
    /*00007bd0*/ STREQ       R5, [R4, #168]              @ 0xa8 
    AO_DRV_SetVolume_x7bd4:
    /*00007bd4*/ ADD         SP, SP, #12 
    /*00007bd8*/ POP         {R4, R5, PC} 
    AO_DRV_SetVolume_x7bdc:
    /*00007bdc*/ MOVW        R3, #1203                   @ 0x4b3 
    /*00007be0*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1728] 
    /*00007be4*/ STR         R3, [SP] 
    /*00007be8*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1728] 
    /*00007bec*/ LDR         R3, [PC, #16]               @ 0x0000000000007c04 
    /*00007bf0*/ MOV         R1, #22 
    /*00007bf4*/ BL          0x0000000000007BF4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007bf8*/ MOVW        R0, #32784                  @ 0x8010 
    /*00007bfc*/ MOVT        R0, #40982                  @ 0xa016 
    /*00007c00*/ B           AO_DRV_SetVolume_x7bd4
    /*00007c04*/ ANDEQ       R0, R0, IP, LSL #6 
FUNC_END AO_DRV_SetVolume


FUNC_BEGIN AO_DRV_GetVolume
    /*00007c08*/ PUSH        {R4, R5, LR} 
    /*00007c0c*/ MOV         R4, R0 
    /*00007c10*/ SUB         SP, SP, #12 
    /*00007c14*/ MOV         R0, #20 
    /*00007c18*/ MOV         R5, R1 
    /*00007c1c*/ BL          0x0000000000007C1C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007c20*/ CMP         R0, #0 
    /*00007c24*/ BEQ         AO_DRV_GetVolume_x7c4c
    /*00007c28*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00007c2c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00007c30*/ MOV         R2, #184                    @ 0xb8 
    /*00007c34*/ MOV         R0, #0 
    /*00007c38*/ MLA         R4, R2, R4, R3 
    /*00007c3c*/ LDR         R3, [R4, #168]              @ 0xa8 
    /*00007c40*/ STR         R3, [R5] 
    AO_DRV_GetVolume_x7c44:
    /*00007c44*/ ADD         SP, SP, #12 
    /*00007c48*/ POP         {R4, R5, PC} 
    AO_DRV_GetVolume_x7c4c:
    /*00007c4c*/ MOVW        R3, #1225                   @ 0x4c9 
    /*00007c50*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1728] 
    /*00007c54*/ STR         R3, [SP] 
    /*00007c58*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1728] 
    /*00007c5c*/ LDR         R3, [PC, #16]               @ 0x0000000000007c74 
    /*00007c60*/ MOV         R1, #22 
    /*00007c64*/ BL          0x0000000000007C64          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007c68*/ MOVW        R0, #32784                  @ 0x8010 
    /*00007c6c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00007c70*/ B           AO_DRV_GetVolume_x7c44
    /*00007c74*/ ANDEQ       R0, R0, R0, LSR #6 
FUNC_END AO_DRV_GetVolume


FUNC_BEGIN AO_DRV_SetClkDir
    /*00007c78*/ PUSH        {R4, R5, LR} 
    /*00007c7c*/ MOV         R4, R0 
    /*00007c80*/ SUB         SP, SP, #12 
    /*00007c84*/ MOV         R0, #20 
    /*00007c88*/ MOV         R5, R1 
    /*00007c8c*/ BL          0x0000000000007C8C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007c90*/ SUBS        R3, R0, #0 
    /*00007c94*/ BEQ         AO_DRV_SetClkDir_x7cc8
    /*00007c98*/ LDR         R3, [R3, #144]              @ 0x90 
    /*00007c9c*/ MOV         R0, R4 
    /*00007ca0*/ MOV         R1, R5 
    /*00007ca4*/ BLX         R3 
    /*00007ca8*/ CMP         R0, #0 
    /*00007cac*/ MOVWEQ      R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00007cb0*/ MOVTEQ      R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00007cb4*/ MOVEQ       R2, #184                    @ 0xb8 
    /*00007cb8*/ MLAEQ       R4, R2, R4, R3 
    /*00007cbc*/ STREQ       R5, [R4, #148]              @ 0x94 
    AO_DRV_SetClkDir_x7cc0:
    /*00007cc0*/ ADD         SP, SP, #12 
    /*00007cc4*/ POP         {R4, R5, PC} 
    AO_DRV_SetClkDir_x7cc8:
    /*00007cc8*/ MOVW        R3, #1246                   @ 0x4de 
    /*00007ccc*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1728] 
    /*00007cd0*/ STR         R3, [SP] 
    /*00007cd4*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1728] 
    /*00007cd8*/ LDR         R3, [PC, #16]               @ 0x0000000000007cf0 
    /*00007cdc*/ MOV         R1, #21 
    /*00007ce0*/ BL          0x0000000000007CE0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007ce4*/ MOVW        R0, #32784                  @ 0x8010 
    /*00007ce8*/ MOVT        R0, #40981                  @ 0xa015 
    /*00007cec*/ B           AO_DRV_SetClkDir_x7cc0
    /*00007cf0*/ ANDEQ       R0, R0, R4, LSR R3 
FUNC_END AO_DRV_SetClkDir


FUNC_BEGIN AO_DRV_GetClkDir
    /*00007cf4*/ PUSH        {R4, R5, LR} 
    /*00007cf8*/ MOV         R4, R0 
    /*00007cfc*/ SUB         SP, SP, #12 
    /*00007d00*/ MOV         R0, #20 
    /*00007d04*/ MOV         R5, R1 
    /*00007d08*/ BL          0x0000000000007D08          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007d0c*/ SUBS        R3, R0, #0 
    /*00007d10*/ BEQ         AO_DRV_GetClkDir_x7d2c
    /*00007d14*/ LDR         R3, [R3, #148]              @ 0x94 
    /*00007d18*/ MOV         R1, R5 
    /*00007d1c*/ MOV         R0, R4 
    /*00007d20*/ ADD         SP, SP, #12 
    /*00007d24*/ POP         {R4, R5, LR} 
    /*00007d28*/ BX          R3 
    AO_DRV_GetClkDir_x7d2c:
    /*00007d2c*/ MOVW        R3, #1265                   @ 0x4f1 
    /*00007d30*/ MOV         R1, #21 
    /*00007d34*/ STR         R3, [SP] 
    /*00007d38*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1728] 
    /*00007d3c*/ LDR         R3, [PC, #20]               @ 0x0000000000007d58 
    /*00007d40*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1728] 
    /*00007d44*/ BL          0x0000000000007D44          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007d48*/ MOVW        R0, #32784                  @ 0x8010 
    /*00007d4c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00007d50*/ ADD         SP, SP, #12 
    /*00007d54*/ POP         {R4, R5, PC} 
    /*00007d58*/ ANDEQ       R0, R0, R8, ASR #6 
FUNC_END AO_DRV_GetClkDir


FUNC_BEGIN AO_DRV_Restore
    /*00007d5c*/ PUSH        {R4, LR} 
    /*00007d60*/ MOV         R0, #20 
    /*00007d64*/ SUB         SP, SP, #8 
    /*00007d68*/ BL          0x0000000000007D68          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007d6c*/ SUBS        R3, R0, #0 
    /*00007d70*/ BEQ         AO_DRV_Restore_x7db0
    /*00007d74*/ LDR         R3, [R3, #168]              @ 0xa8 
    /*00007d78*/ BLX         R3 
    /*00007d7c*/ SUBS        R4, R0, #0 
    /*00007d80*/ BEQ         AO_DRV_Restore_x7da4
    /*00007d84*/ MOVW        R3, #1286                   @ 0x506 
    /*00007d88*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a28] 
    /*00007d8c*/ STR         R3, [SP] 
    /*00007d90*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a28] 
    /*00007d94*/ LDR         R3, [PC, #60]               @ 0x0000000000007dd8 
    /*00007d98*/ MOV         R1, #22 
    /*00007d9c*/ MOV         R0, #0 
    /*00007da0*/ BL          0x0000000000007DA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    AO_DRV_Restore_x7da4:
    /*00007da4*/ MOV         R0, R4 
    /*00007da8*/ ADD         SP, SP, #8 
    /*00007dac*/ POP         {R4, PC} 
    AO_DRV_Restore_x7db0:
    /*00007db0*/ MOVW        R3, #1279                   @ 0x4ff 
    /*00007db4*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1728] 
    /*00007db8*/ STR         R3, [SP] 
    /*00007dbc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1728] 
    /*00007dc0*/ LDR         R3, [PC, #16]               @ 0x0000000000007dd8 
    /*00007dc4*/ MOV         R1, #22 
    /*00007dc8*/ MOVW        R4, #32784                  @ 0x8010 
    /*00007dcc*/ BL          0x0000000000007DCC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007dd0*/ MOVT        R4, #40982                  @ 0xa016 
    /*00007dd4*/ B           AO_DRV_Restore_x7da4
    /*00007dd8*/ ANDEQ       R0, R0, IP, ASR R3 
FUNC_END AO_DRV_Restore


FUNC_BEGIN AO_DRV_ProcShow
    /*00007ddc*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00007de0*/ SUB         SP, SP, #36                 @ 0x24 
    /*00007de4*/ MOV         R6, R0 
    /*00007de8*/ MOVW        R1, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b10] 
    /*00007dec*/ MOVT        R1, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b10] 
    /*00007df0*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00007df4*/ BL          0x0000000000007DF4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007df8*/ MOVW        R2, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b94] 
    /*00007dfc*/ MOVW        R3, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b9c] 
    /*00007e00*/ MOVT        R2, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b94] 
    /*00007e04*/ MOVT        R3, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b9c] 
    /*00007e08*/ STR         R2, [SP, #28] 
    /*00007e0c*/ STR         R3, [SP, #24] 
    /*00007e10*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ba4] 
    /*00007e14*/ MOVW        R3, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bac] 
    /*00007e18*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ba4] 
    /*00007e1c*/ MOVT        R3, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bac] 
    /*00007e20*/ STR         R2, [SP, #20] 
    /*00007e24*/ STR         R3, [SP, #16] 
    /*00007e28*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bb8] 
    /*00007e2c*/ MOVW        R3, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bc0] 
    /*00007e30*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bb8] 
    /*00007e34*/ MOVT        R3, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bc0] 
    /*00007e38*/ STR         R2, [SP, #12] 
    /*00007e3c*/ STR         R3, [SP, #8] 
    /*00007e40*/ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bc8] 
    /*00007e44*/ MOVW        R3, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bd0] 
    /*00007e48*/ MOVT        R2, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bc8] 
    /*00007e4c*/ MOVT        R3, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bd0] 
    /*00007e50*/ STR         R2, [SP, #4] 
    /*00007e54*/ STR         R3, [SP] 
    /*00007e58*/ MOV         R0, R6 
    /*00007e5c*/ MOVW        R3, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b64] 
    /*00007e60*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b6c] 
    /*00007e64*/ MOVT        R3, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b64] 
    /*00007e68*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b6c] 
    /*00007e6c*/ MOVW        R1, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b74] 
    /*00007e70*/ MOVT        R1, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b74] 
    /*00007e74*/ BL          0x0000000000007E74          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007e78*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00007e7c*/ MOVW        R8, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bd8] 
    /*00007e80*/ MOVW        SL, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1af8] 
    /*00007e84*/ MOVW        R9, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b00] 
    /*00007e88*/ MOVT        R8, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bd8] 
    /*00007e8c*/ MOVT        SL, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1af8] 
    /*00007e90*/ MOVT        R9, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b00] 
    /*00007e94*/ MOV         R4, R5 
    /*00007e98*/ MOV         R7, #0 
    AO_DRV_ProcShow_x7e9c:
    /*00007e9c*/ LDR         R3, [R4, #76]               @ 0x4c 
    /*00007ea0*/ CMP         R3, #0 
    /*00007ea4*/ BEQ         AO_DRV_ProcShow_x7ffc
    /*00007ea8*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00007eac*/ CMP         R3, #0 
    /*00007eb0*/ MOVWEQ      R3, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a58] 
    /*00007eb4*/ MOVTEQ      R3, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a58] 
    /*00007eb8*/ BEQ         AO_DRV_ProcShow_x7eec
    /*00007ebc*/ CMP         R3, #1 
    /*00007ec0*/ BEQ         AO_DRV_ProcShow_x8288
    /*00007ec4*/ CMP         R3, #2 
    /*00007ec8*/ BEQ         AO_DRV_ProcShow_x82e4
    /*00007ecc*/ CMP         R3, #3 
    /*00007ed0*/ BEQ         AO_DRV_ProcShow_x82b4
    /*00007ed4*/ CMP         R3, #4 
    /*00007ed8*/ BEQ         AO_DRV_ProcShow_x8314
    /*00007edc*/ CMP         R3, #5 
    /*00007ee0*/ MOVW        R3, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a80] 
    /*00007ee4*/ MOVT        R3, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a80] 
    /*00007ee8*/ MOVNE       R3, #0 
    AO_DRV_ProcShow_x7eec:
    /*00007eec*/ LDR         R2, [R4, #36]               @ 0x24 
    /*00007ef0*/ CMP         R2, #8000                   @ 0x1f40 
    /*00007ef4*/ BEQ         AO_DRV_ProcShow_x829c
    AO_DRV_ProcShow_x7ef8:
    /*00007ef8*/ MOVW        R1, #11025                  @ 0x2b11 
    /*00007efc*/ CMP         R2, R1 
    /*00007f00*/ BEQ         AO_DRV_ProcShow_x82c0
    /*00007f04*/ MOVW        R1, #12000                  @ 0x2ee0 
    /*00007f08*/ CMP         R2, R1 
    /*00007f0c*/ BEQ         AO_DRV_ProcShow_x82d8
    /*00007f10*/ CMP         R2, #16000                  @ 0x3e80 
    /*00007f14*/ BEQ         AO_DRV_ProcShow_x82a8
    /*00007f18*/ MOVW        R1, #22050                  @ 0x5622 
    /*00007f1c*/ CMP         R2, R1 
    /*00007f20*/ BEQ         AO_DRV_ProcShow_x82f0
    /*00007f24*/ MOVW        R1, #24000                  @ 0x5dc0 
    /*00007f28*/ CMP         R2, R1 
    /*00007f2c*/ BEQ         AO_DRV_ProcShow_x82fc
    /*00007f30*/ CMP         R2, #32000                  @ 0x7d00 
    /*00007f34*/ BEQ         AO_DRV_ProcShow_x8308
    /*00007f38*/ MOVW        R1, #44100                  @ 0xac44 
    /*00007f3c*/ CMP         R2, R1 
    /*00007f40*/ BEQ         AO_DRV_ProcShow_x8320
    /*00007f44*/ MOVW        R1, #48000                  @ 0xbb80 
    /*00007f48*/ CMP         R2, R1 
    /*00007f4c*/ BEQ         AO_DRV_ProcShow_x832c
    /*00007f50*/ CMP         R2, #64000                  @ 0xfa00 
    /*00007f54*/ BEQ         AO_DRV_ProcShow_x8338
    /*00007f58*/ MOV         R1, #30464                  @ 0x7700 
    /*00007f5c*/ MOVT        R1, #1 
    /*00007f60*/ CMP         R2, R1 
    /*00007f64*/ MOVW        R0, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ad8] 
    /*00007f68*/ MOVT        R0, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ad8] 
    /*00007f6c*/ MOVNE       R0, #0 
    AO_DRV_ProcShow_x7f70:
    /*00007f70*/ LDR         R2, [R4, #40]               @ 0x28 
    /*00007f74*/ CMP         R2, #0 
    /*00007f78*/ MOVWEQ      R1, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ae0] 
    /*00007f7c*/ MOVTEQ      R1, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ae0] 
    /*00007f80*/ BEQ         AO_DRV_ProcShow_x7f9c
    /*00007f84*/ CMP         R2, #1 
    /*00007f88*/ BEQ         AO_DRV_ProcShow_x82cc
    /*00007f8c*/ CMP         R2, #2 
    /*00007f90*/ MOVW        R1, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1af0] 
    /*00007f94*/ MOVT        R1, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1af0] 
    /*00007f98*/ MOVNE       R1, #0 
    AO_DRV_ProcShow_x7f9c:
    /*00007f9c*/ LDR         IP, [R4, #48]               @ 0x30 
    /*00007fa0*/ LDR         R2, [R4, #64]               @ 0x40 
    /*00007fa4*/ CMP         IP, #0 
    /*00007fa8*/ LDR         LR, [R4, #68]               @ 0x44 
    /*00007fac*/ MOVEQ       FP, SL 
    /*00007fb0*/ BEQ         AO_DRV_ProcShow_x7fc0
    /*00007fb4*/ CMP         IP, #1 
    /*00007fb8*/ MOVEQ       FP, R9 
    /*00007fbc*/ MOVNE       FP, #0 
    AO_DRV_ProcShow_x7fc0:
    /*00007fc0*/ LDR         IP, [R4, #56]               @ 0x38 
    /*00007fc4*/ STR         FP, [SP, #16] 
    /*00007fc8*/ LDR         FP, [R4, #60]               @ 0x3c 
    /*00007fcc*/ STR         R2, [SP, #8] 
    /*00007fd0*/ MOV         R2, R7 
    /*00007fd4*/ STR         R1, [SP, #4] 
    /*00007fd8*/ MOV         R1, R8 
    /*00007fdc*/ STR         R0, [SP] 
    /*00007fe0*/ MOV         R0, R6 
    /*00007fe4*/ STR         LR, [SP, #12] 
    /*00007fe8*/ STR         IP, [SP, #28] 
    /*00007fec*/ LDR         IP, [R4, #52]               @ 0x34 
    /*00007ff0*/ STR         FP, [SP, #20] 
    /*00007ff4*/ STR         IP, [SP, #24] 
    /*00007ff8*/ BL          0x0000000000007FF8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    AO_DRV_ProcShow_x7ffc:
    /*00007ffc*/ ADD         R7, R7, #1 
    /*00008000*/ ADD         R4, R4, #184                @ 0xb8 
    /*00008004*/ CMP         R7, #2 
    /*00008008*/ BNE         AO_DRV_ProcShow_x7e9c
    /*0000800c*/ MOVW        R1, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bf8] 
    /*00008010*/ MOV         R0, R6 
    /*00008014*/ MOVT        R1, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bf8] 
    /*00008018*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*0000801c*/ BL          0x000000000000801C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00008020*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c74] 
    /*00008024*/ MOVW        R3, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c7c] 
    /*00008028*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c74] 
    /*0000802c*/ MOVT        R3, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c7c] 
    /*00008030*/ STR         R2, [SP, #20] 
    /*00008034*/ STR         R3, [SP, #16] 
    /*00008038*/ MOVW        R2, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c84] 
    /*0000803c*/ MOVW        R3, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c90] 
    /*00008040*/ MOVT        R2, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c84] 
    /*00008044*/ MOVT        R3, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c90] 
    /*00008048*/ STR         R2, [SP, #12] 
    /*0000804c*/ STR         R3, [SP, #8] 
    /*00008050*/ MOVW        R2, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c98] 
    /*00008054*/ MOVW        R3, #0                      @ .LC58 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ca0] 
    /*00008058*/ MOVT        R2, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c98] 
    /*0000805c*/ MOVT        R3, #0                      @ .LC58 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ca0] 
    /*00008060*/ STR         R2, [SP, #4] 
    /*00008064*/ STR         R3, [SP] 
    /*00008068*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b6c] 
    /*0000806c*/ MOVW        R3, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c4c] 
    /*00008070*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b6c] 
    /*00008074*/ MOVT        R3, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c4c] 
    /*00008078*/ MOVW        R1, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c54] 
    /*0000807c*/ MOV         R0, R6 
    /*00008080*/ MOVT        R1, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c54] 
    /*00008084*/ BL          0x0000000000008084          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00008088*/ MOVW        R8, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ca8] 
    /*0000808c*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00008090*/ MOVT        R8, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ca8] 
    /*00008094*/ MOV         R7, #0 
    AO_DRV_ProcShow_x8098:
    /*00008098*/ LDR         R3, [R4, #76]               @ 0x4c 
    /*0000809c*/ CMP         R3, #0 
    /*000080a0*/ BNE         AO_DRV_ProcShow_x8244
    AO_DRV_ProcShow_x80a4:
    /*000080a4*/ ADD         R7, R7, #1 
    /*000080a8*/ ADD         R4, R4, #184                @ 0xb8 
    /*000080ac*/ CMP         R7, #2 
    /*000080b0*/ BNE         AO_DRV_ProcShow_x8098
    /*000080b4*/ MOVW        R1, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1cc8] 
    /*000080b8*/ MOV         R0, R6 
    /*000080bc*/ MOVT        R1, #0                      @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1cc8] 
    /*000080c0*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*000080c4*/ BL          0x00000000000080C4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000080c8*/ MOVW        R2, #0                      @ .LC63 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d40] 
    /*000080cc*/ MOVW        R3, #0                      @ .LC64 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d48] 
    /*000080d0*/ MOVT        R2, #0                      @ .LC63 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d40] 
    /*000080d4*/ MOVT        R3, #0                      @ .LC64 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d48] 
    /*000080d8*/ STR         R2, [SP, #12] 
    /*000080dc*/ STR         R3, [SP, #8] 
    /*000080e0*/ MOVW        R2, #0                      @ .LC65 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d50] 
    /*000080e4*/ MOVW        R3, #0                      @ .LC66 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d58] 
    /*000080e8*/ MOVT        R2, #0                      @ .LC65 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d50] 
    /*000080ec*/ MOVT        R3, #0                      @ .LC66 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d58] 
    /*000080f0*/ STR         R2, [SP, #4] 
    /*000080f4*/ STR         R3, [SP] 
    /*000080f8*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b6c] 
    /*000080fc*/ MOVW        R3, #0                      @ .LC61 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d1c] 
    /*00008100*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b6c] 
    /*00008104*/ MOVT        R3, #0                      @ .LC61 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d1c] 
    /*00008108*/ MOVW        R1, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d28] 
    /*0000810c*/ MOV         R0, R6 
    /*00008110*/ MOVT        R1, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d28] 
    /*00008114*/ BL          0x0000000000008114          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00008118*/ MOVW        SL, #0                      @ .LC67 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d60] 
    /*0000811c*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00008120*/ MOVT        SL, #0                      @ .LC67 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d60] 
    /*00008124*/ MOV         R7, #0 
    AO_DRV_ProcShow_x8128:
    /*00008128*/ LDR         R3, [R4, #76]               @ 0x4c 
    /*0000812c*/ CMP         R3, #0 
    /*00008130*/ BNE         AO_DRV_ProcShow_x820c
    AO_DRV_ProcShow_x8134:
    /*00008134*/ ADD         R7, R7, #1 
    /*00008138*/ ADD         R4, R4, #184                @ 0xb8 
    /*0000813c*/ CMP         R7, #2 
    /*00008140*/ BNE         AO_DRV_ProcShow_x8128
    /*00008144*/ MOVW        R1, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d7c] 
    /*00008148*/ MOV         R0, R6 
    /*0000814c*/ MOVT        R1, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d7c] 
    /*00008150*/ MOVW        R7, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1df8] 
    /*00008154*/ BL          0x0000000000008154          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00008158*/ MOVW        R2, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1de8] 
    /*0000815c*/ MOVW        R3, #0                      @ .LC72 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1df0] 
    /*00008160*/ MOVT        R2, #0                      @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1de8] 
    /*00008164*/ MOVT        R3, #0                      @ .LC72 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1df0] 
    /*00008168*/ STR         R2, [SP, #4] 
    /*0000816c*/ STR         R3, [SP] 
    /*00008170*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b6c] 
    /*00008174*/ MOVW        R3, #0                      @ .LC69 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1dd0] 
    /*00008178*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b6c] 
    /*0000817c*/ MOVT        R3, #0                      @ .LC69 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1dd0] 
    /*00008180*/ MOVW        R1, #0                      @ .LC70 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1dd8] 
    /*00008184*/ MOV         R0, R6 
    /*00008188*/ MOVT        R1, #0                      @ .LC70 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1dd8] 
    /*0000818c*/ BL          0x000000000000818C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00008190*/ MOVW        R9, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b08] 
    /*00008194*/ MOVW        R8, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b0c] 
    /*00008198*/ MOVT        R7, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1df8] 
    /*0000819c*/ MOVT        R9, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b08] 
    /*000081a0*/ MOVT        R8, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b0c] 
    /*000081a4*/ MOV         R4, #0 
    AO_DRV_ProcShow_x81a8:
    /*000081a8*/ LDR         R3, [R5, #76]               @ 0x4c 
    /*000081ac*/ CMP         R3, #0 
    /*000081b0*/ BEQ         AO_DRV_ProcShow_x81f0
    /*000081b4*/ LDR         R2, [R5, #152]              @ 0x98 
    /*000081b8*/ LDR         R3, [R5, #144]              @ 0x90 
    /*000081bc*/ CMP         R2, #1 
    /*000081c0*/ MOVEQ       R2, R9 
    /*000081c4*/ BEQ         AO_DRV_ProcShow_x81d4
    /*000081c8*/ CMP         R2, #0 
    /*000081cc*/ MOVEQ       R2, R8 
    /*000081d0*/ MOVNE       R2, #0 
    AO_DRV_ProcShow_x81d4:
    /*000081d4*/ LDR         IP, [R5, #168]              @ 0xa8 
    /*000081d8*/ MOV         R1, R7 
    /*000081dc*/ STR         R2, [SP] 
    /*000081e0*/ MOV         R0, R6 
    /*000081e4*/ MOV         R2, R4 
    /*000081e8*/ STR         IP, [SP, #4] 
    /*000081ec*/ BL          0x00000000000081EC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    AO_DRV_ProcShow_x81f0:
    /*000081f0*/ ADD         R4, R4, #1 
    /*000081f4*/ ADD         R5, R5, #184                @ 0xb8 
    /*000081f8*/ CMP         R4, #2 
    /*000081fc*/ BNE         AO_DRV_ProcShow_x81a8
    /*00008200*/ MOV         R0, #0 
    /*00008204*/ ADD         SP, SP, #36                 @ 0x24 
    /*00008208*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AO_DRV_ProcShow_x820c:
    /*0000820c*/ LDR         R8, [R4, #24] 
    /*00008210*/ MOV         R2, R7 
    /*00008214*/ LDR         LR, [R4, #20] 
    /*00008218*/ MOV         R1, SL 
    /*0000821c*/ LDR         R3, [R4, #100]              @ 0x64 
    /*00008220*/ MOV         R0, R6 
    /*00008224*/ LDR         IP, [R4] 
    /*00008228*/ STR         R8, [SP, #16] 
    /*0000822c*/ STR         LR, [SP, #12] 
    /*00008230*/ LDRD        R8, [R4, #8] 
    /*00008234*/ STR         IP, [SP, #8] 
    /*00008238*/ STRD        R8, [SP] 
    /*0000823c*/ BL          0x000000000000823C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00008240*/ B           AO_DRV_ProcShow_x8134
    AO_DRV_ProcShow_x8244:
    /*00008244*/ LDR         IP, [R4, #116]              @ 0x74 
    /*00008248*/ MOV         R2, R7 
    /*0000824c*/ LDR         R9, [R4, #96]               @ 0x60 
    /*00008250*/ MOV         R1, R8 
    /*00008254*/ LDR         LR, [R4, #92]               @ 0x5c 
    /*00008258*/ MOV         R0, R6 
    /*0000825c*/ LDR         R3, [R4, #132]              @ 0x84 
    /*00008260*/ STR         R9, [SP, #20] 
    /*00008264*/ STR         IP, [SP, #16] 
    /*00008268*/ LDR         IP, [R4, #88]               @ 0x58 
    /*0000826c*/ STR         LR, [SP, #12] 
    /*00008270*/ LDR         LR, [R4, #140]              @ 0x8c 
    /*00008274*/ STR         IP, [SP, #8] 
    /*00008278*/ LDR         IP, [R4, #136]              @ 0x88 
    /*0000827c*/ STM         SP, {IP, LR} 
    /*00008280*/ BL          0x0000000000008280          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00008284*/ B           AO_DRV_ProcShow_x80a4
    AO_DRV_ProcShow_x8288:
    /*00008288*/ LDR         R2, [R4, #36]               @ 0x24 
    /*0000828c*/ MOVW        R3, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a60] 
    /*00008290*/ MOVT        R3, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a60] 
    /*00008294*/ CMP         R2, #8000                   @ 0x1f40 
    /*00008298*/ BNE         AO_DRV_ProcShow_x7ef8
    AO_DRV_ProcShow_x829c:
    /*0000829c*/ MOVW        R0, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a88] 
    /*000082a0*/ MOVT        R0, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a88] 
    /*000082a4*/ B           AO_DRV_ProcShow_x7f70
    AO_DRV_ProcShow_x82a8:
    /*000082a8*/ MOVW        R0, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1aa0] 
    /*000082ac*/ MOVT        R0, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1aa0] 
    /*000082b0*/ B           AO_DRV_ProcShow_x7f70
    AO_DRV_ProcShow_x82b4:
    /*000082b4*/ MOVW        R3, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a70] 
    /*000082b8*/ MOVT        R3, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a70] 
    /*000082bc*/ B           AO_DRV_ProcShow_x7eec
    AO_DRV_ProcShow_x82c0:
    /*000082c0*/ MOVW        R0, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a90] 
    /*000082c4*/ MOVT        R0, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a90] 
    /*000082c8*/ B           AO_DRV_ProcShow_x7f70
    AO_DRV_ProcShow_x82cc:
    /*000082cc*/ MOVW        R1, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ae8] 
    /*000082d0*/ MOVT        R1, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ae8] 
    /*000082d4*/ B           AO_DRV_ProcShow_x7f9c
    AO_DRV_ProcShow_x82d8:
    /*000082d8*/ MOVW        R0, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a98] 
    /*000082dc*/ MOVT        R0, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a98] 
    /*000082e0*/ B           AO_DRV_ProcShow_x7f70
    AO_DRV_ProcShow_x82e4:
    /*000082e4*/ MOVW        R3, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a68] 
    /*000082e8*/ MOVT        R3, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a68] 
    /*000082ec*/ B           AO_DRV_ProcShow_x7eec
    AO_DRV_ProcShow_x82f0:
    /*000082f0*/ MOVW        R0, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1aa8] 
    /*000082f4*/ MOVT        R0, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1aa8] 
    /*000082f8*/ B           AO_DRV_ProcShow_x7f70
    AO_DRV_ProcShow_x82fc:
    /*000082fc*/ MOVW        R0, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ab0] 
    /*00008300*/ MOVT        R0, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ab0] 
    /*00008304*/ B           AO_DRV_ProcShow_x7f70
    AO_DRV_ProcShow_x8308:
    /*00008308*/ MOVW        R0, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ab8] 
    /*0000830c*/ MOVT        R0, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ab8] 
    /*00008310*/ B           AO_DRV_ProcShow_x7f70
    AO_DRV_ProcShow_x8314:
    /*00008314*/ MOVW        R3, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a78] 
    /*00008318*/ MOVT        R3, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a78] 
    /*0000831c*/ B           AO_DRV_ProcShow_x7eec
    AO_DRV_ProcShow_x8320:
    /*00008320*/ MOVW        R0, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ac0] 
    /*00008324*/ MOVT        R0, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ac0] 
    /*00008328*/ B           AO_DRV_ProcShow_x7f70
    AO_DRV_ProcShow_x832c:
    /*0000832c*/ MOVW        R0, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ac8] 
    /*00008330*/ MOVT        R0, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ac8] 
    /*00008334*/ B           AO_DRV_ProcShow_x7f70
    AO_DRV_ProcShow_x8338:
    /*00008338*/ MOVW        R0, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ad0] 
    /*0000833c*/ MOVT        R0, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ad0] 
    /*00008340*/ B           AO_DRV_ProcShow_x7f70
FUNC_END AO_DRV_ProcShow


FUNC_BEGIN AO_DRV_GetAECData
    /*00008344*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00008348*/ SUBS        SL, R2, #0 
    /*0000834c*/ SUB         SP, SP, #20 
    /*00008350*/ MOV         R8, R0 
    /*00008354*/ MOV         FP, R1 
    /*00008358*/ MOV         R6, R3 
    /*0000835c*/ BEQ         AO_DRV_GetAECData_x88a0
    AO_DRV_GetAECData_x8360:
    /*00008360*/ MOV         R3, #184                    @ 0xb8 
    /*00008364*/ MOVW        R7, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x44e8] 
    /*00008368*/ MUL         R3, R3, R8 
    /*0000836c*/ MOVT        R7, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x44e8] 
    /*00008370*/ MOV         R5, R7 
    /*00008374*/ LDR         R0, [R5, R3]! 
    /*00008378*/ CMP         R0, R6 
    /*0000837c*/ LDR         R3, [R5, #16] 
    /*00008380*/ MOVEQ       R6, #0 
    /*00008384*/ CMP         R3, #0 
    /*00008388*/ STR         R3, [SP, #12] 
    /*0000838c*/ BEQ         AO_DRV_GetAECData_x8904
    /*00008390*/ LDR         R3, [SP, #12] 
    /*00008394*/ ADD         R4, R3, R6 
    /*00008398*/ ADD         R3, R3, R0 
    /*0000839c*/ CMP         R4, R3 
    /*000083a0*/ BHI         AO_DRV_GetAECData_x88d8
    /*000083a4*/ LDR         R9, [R5, #40]               @ 0x28 
    /*000083a8*/ CMP         R9, #0 
    /*000083ac*/ BNE         AO_DRV_GetAECData_x854c
    /*000083b0*/ LDR         R9, [R5, #52]               @ 0x34 
    /*000083b4*/ CMP         R9, #1 
    /*000083b8*/ BEQ         AO_DRV_GetAECData_x86e8
    /*000083bc*/ LDR         R1, [R5, #64]               @ 0x40 
    /*000083c0*/ SUB         R0, R0, R6 
    /*000083c4*/ LSL         R0, R0, #3 
    /*000083c8*/ LSL         R1, R1, #3 
    /*000083cc*/ BL          0x00000000000083CC          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000083d0*/ MOV         R3, #46                     @ 0x2e 
    /*000083d4*/ LDR         IP, [R5, #60]               @ 0x3c 
    /*000083d8*/ MLA         R8, R3, R8, FP 
    /*000083dc*/ LDR         R5, [R5, #108]              @ 0x6c 
    /*000083e0*/ LDR         R3, [SL, #8] 
    /*000083e4*/ ADD         R8, R8, #30 
    /*000083e8*/ LDR         LR, [R7, R8, LSL #2] 
    /*000083ec*/ CMP         IP, R0 
    /*000083f0*/ MOVCC       R0, IP 
    /*000083f4*/ CMP         R9, #0 
    /*000083f8*/ BNE         AO_DRV_GetAECData_x886c
    /*000083fc*/ ANDS        R6, R0, #3 
    /*00008400*/ ADD         R4, R4, LR 
    /*00008404*/ BEQ         AO_DRV_GetAECData_x842c
    /*00008408*/ MOV         R2, #1 
    /*0000840c*/ ADD         R8, R3, R6 
    /*00008410*/ LSL         R9, R2, R5 
    /*00008414*/ MOV         R1, R4 
    /*00008418*/ MOV         R2, R3 
    AO_DRV_GetAECData_x841c:
    /*0000841c*/ LDRB        R7, [R1], R9 
    /*00008420*/ STRB        R7, [R2], #1 
    /*00008424*/ CMP         R8, R2 
    /*00008428*/ BNE         AO_DRV_GetAECData_x841c
    AO_DRV_GetAECData_x842c:
    /*0000842c*/ CMP         R6, R0 
    /*00008430*/ BCS         AO_DRV_GetAECData_x8494
    /*00008434*/ ADD         SL, R6, #1 
    /*00008438*/ ADD         R9, R6, #2 
    /*0000843c*/ ADD         R8, R6, #3 
    /*00008440*/ MOV         FP, #4 
    /*00008444*/ ADD         SL, R4, SL, LSL R5 
    /*00008448*/ ADD         R9, R4, R9, LSL R5 
    /*0000844c*/ ADD         R8, R4, R8, LSL R5 
    /*00008450*/ LSL         FP, FP, R5 
    /*00008454*/ ADD         R4, R4, R6, LSL R5 
    /*00008458*/ ADD         R1, R3, R6 
    /*0000845c*/ MOV         R2, #0 
    AO_DRV_GetAECData_x8460:
    /*00008460*/ LDRB        R7, [R4, R2] 
    /*00008464*/ ADD         R6, R6, #4 
    /*00008468*/ CMP         R6, R0 
    /*0000846c*/ ADD         R1, R1, #4 
    /*00008470*/ STRB        R7, [R1, #-4] 
    /*00008474*/ LDRB        R7, [SL, R2] 
    /*00008478*/ STRB        R7, [R1, #-3] 
    /*0000847c*/ LDRB        R7, [R9, R2] 
    /*00008480*/ STRB        R7, [R1, #-2] 
    /*00008484*/ LDRB        R7, [R8, R2] 
    /*00008488*/ ADD         R2, R2, FP 
    /*0000848c*/ STRB        R7, [R1, #-1] 
    /*00008490*/ BCC         AO_DRV_GetAECData_x8460
    AO_DRV_GetAECData_x8494:
    /*00008494*/ CMP         IP, R0 
    /*00008498*/ BEQ         AO_DRV_GetAECData_x8540
    /*0000849c*/ SUB         R8, IP, R0 
    /*000084a0*/ LDR         R2, [SP, #12] 
    /*000084a4*/ ANDS        R8, R8, #3 
    /*000084a8*/ ADD         LR, R2, LR 
    /*000084ac*/ BEQ         AO_DRV_GetAECData_x88d0
    /*000084b0*/ ADD         R1, R8, R0 
    /*000084b4*/ MOV         R2, #1 
    /*000084b8*/ ADD         R0, R3, R0 
    /*000084bc*/ ADD         R6, R3, R1 
    /*000084c0*/ LSL         R7, R2, R5 
    /*000084c4*/ MOV         R2, LR 
    AO_DRV_GetAECData_x84c8:
    /*000084c8*/ LDRB        R4, [R2], R7 
    /*000084cc*/ STRB        R4, [R0], #1 
    /*000084d0*/ CMP         R6, R0 
    /*000084d4*/ BNE         AO_DRV_GetAECData_x84c8
    AO_DRV_GetAECData_x84d8:
    /*000084d8*/ CMP         R1, IP 
    /*000084dc*/ BCS         AO_DRV_GetAECData_x8540
    /*000084e0*/ ADD         R7, R8, #1 
    /*000084e4*/ ADD         R6, R8, #2 
    /*000084e8*/ ADD         R4, R8, #3 
    /*000084ec*/ MOV         R2, #4 
    /*000084f0*/ ADD         R3, R3, R1 
    /*000084f4*/ ADD         R7, LR, R7, LSL R5 
    /*000084f8*/ ADD         R6, LR, R6, LSL R5 
    /*000084fc*/ ADD         R4, LR, R4, LSL R5 
    /*00008500*/ ADD         LR, LR, R8, LSL R5 
    /*00008504*/ LSL         R5, R2, R5 
    /*00008508*/ MOV         R2, #0 
    AO_DRV_GetAECData_x850c:
    /*0000850c*/ LDRB        R0, [LR, R2] 
    /*00008510*/ ADD         R1, R1, #4 
    /*00008514*/ CMP         R1, IP 
    /*00008518*/ ADD         R3, R3, #4 
    /*0000851c*/ STRB        R0, [R3, #-4] 
    /*00008520*/ LDRB        R0, [R7, R2] 
    /*00008524*/ STRB        R0, [R3, #-3] 
    /*00008528*/ LDRB        R0, [R6, R2] 
    /*0000852c*/ STRB        R0, [R3, #-2] 
    /*00008530*/ LDRB        R0, [R4, R2] 
    /*00008534*/ ADD         R2, R2, R5 
    /*00008538*/ STRB        R0, [R3, #-1] 
    /*0000853c*/ BCC         AO_DRV_GetAECData_x850c
    AO_DRV_GetAECData_x8540:
    /*00008540*/ MOV         R0, #0 
    AO_DRV_GetAECData_x8544:
    /*00008544*/ ADD         SP, SP, #20 
    /*00008548*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AO_DRV_GetAECData_x854c:
    /*0000854c*/ LDR         R1, [R5, #64]               @ 0x40 
    /*00008550*/ MOV         R3, #8 
    /*00008554*/ LSL         R3, R3, R9 
    /*00008558*/ SUB         R0, R0, R6 
    /*0000855c*/ MUL         R1, R1, R3 
    /*00008560*/ LSL         R0, R0, #3 
    /*00008564*/ BL          0x0000000000008564          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00008568*/ MOV         R3, #46                     @ 0x2e 
    /*0000856c*/ LDR         IP, [R5, #60]               @ 0x3c 
    /*00008570*/ MLA         R8, R3, R8, FP 
    /*00008574*/ LDR         R5, [R5, #108]              @ 0x6c 
    /*00008578*/ LDR         R3, [SL, #8] 
    /*0000857c*/ ADD         R8, R8, #30 
    /*00008580*/ LDR         LR, [R7, R8, LSL #2] 
    /*00008584*/ CMP         IP, R0 
    /*00008588*/ MOVCC       R0, IP 
    /*0000858c*/ CMP         R9, #1 
    /*00008590*/ BEQ         AO_DRV_GetAECData_x8720
    /*00008594*/ CMP         R9, #2 
    /*00008598*/ BNE         AO_DRV_GetAECData_x8874
    /*0000859c*/ ANDS        R6, R0, #3 
    /*000085a0*/ LSL         LR, LR, #2 
    /*000085a4*/ ADD         R4, R4, LR 
    /*000085a8*/ BEQ         AO_DRV_GetAECData_x85d0
    /*000085ac*/ MOV         R2, #1 
    /*000085b0*/ ADD         R8, R3, R6, LSL #2 
    /*000085b4*/ LSL         R9, R2, R5 
    /*000085b8*/ MOV         R1, R4 
    /*000085bc*/ MOV         R2, R3 
    AO_DRV_GetAECData_x85c0:
    /*000085c0*/ LDR         R7, [R1], R9 
    /*000085c4*/ STR         R7, [R2], #4 
    /*000085c8*/ CMP         R2, R8 
    /*000085cc*/ BNE         AO_DRV_GetAECData_x85c0
    AO_DRV_GetAECData_x85d0:
    /*000085d0*/ CMP         R0, R6 
    /*000085d4*/ BLS         AO_DRV_GetAECData_x8638
    /*000085d8*/ ADD         R9, R6, #1 
    /*000085dc*/ ADD         R8, R6, #2 
    /*000085e0*/ ADD         R7, R6, #3 
    /*000085e4*/ MOV         SL, #4 
    /*000085e8*/ ADD         R9, R4, R9, LSL R5 
    /*000085ec*/ ADD         R8, R4, R8, LSL R5 
    /*000085f0*/ ADD         R7, R4, R7, LSL R5 
    /*000085f4*/ LSL         SL, SL, R5 
    /*000085f8*/ ADD         R4, R4, R6, LSL R5 
    /*000085fc*/ ADD         R1, R3, R6, LSL #2 
    /*00008600*/ MOV         R2, #0 
    AO_DRV_GetAECData_x8604:
    /*00008604*/ LDR         FP, [R4, R2] 
    /*00008608*/ ADD         R6, R6, #4 
    /*0000860c*/ CMP         R0, R6 
    /*00008610*/ ADD         R1, R1, #16 
    /*00008614*/ STR         FP, [R1, #-16] 
    /*00008618*/ LDR         FP, [R9, R2] 
    /*0000861c*/ STR         FP, [R1, #-12] 
    /*00008620*/ LDR         FP, [R8, R2] 
    /*00008624*/ STR         FP, [R1, #-8] 
    /*00008628*/ LDR         FP, [R7, R2] 
    /*0000862c*/ ADD         R2, R2, SL 
    /*00008630*/ STR         FP, [R1, #-4] 
    /*00008634*/ BHI         AO_DRV_GetAECData_x8604
    AO_DRV_GetAECData_x8638:
    /*00008638*/ CMP         R0, IP 
    /*0000863c*/ BEQ         AO_DRV_GetAECData_x8540
    /*00008640*/ SUB         R8, IP, R0 
    /*00008644*/ LDR         R2, [SP, #12] 
    /*00008648*/ ANDS        R8, R8, #3 
    /*0000864c*/ ADD         LR, R2, LR 
    /*00008650*/ BEQ         AO_DRV_GetAECData_x88c0
    /*00008654*/ ADD         R1, R0, R8 
    /*00008658*/ MOV         R2, #1 
    /*0000865c*/ ADD         R0, R3, R0, LSL #2 
    /*00008660*/ LSL         R7, R2, R5 
    /*00008664*/ ADD         R6, R3, R1, LSL #2 
    /*00008668*/ MOV         R2, LR 
    AO_DRV_GetAECData_x866c:
    /*0000866c*/ LDR         R4, [R2], R7 
    /*00008670*/ STR         R4, [R0], #4 
    /*00008674*/ CMP         R6, R0 
    /*00008678*/ BNE         AO_DRV_GetAECData_x866c
    AO_DRV_GetAECData_x867c:
    /*0000867c*/ CMP         IP, R1 
    /*00008680*/ BLS         AO_DRV_GetAECData_x8540
    /*00008684*/ ADD         R7, R8, #1 
    /*00008688*/ ADD         R6, R8, #2 
    /*0000868c*/ ADD         R4, R8, #3 
    /*00008690*/ MOV         R2, #4 
    /*00008694*/ ADD         R3, R3, R1, LSL #2 
    /*00008698*/ ADD         R7, LR, R7, LSL R5 
    /*0000869c*/ ADD         R6, LR, R6, LSL R5 
    /*000086a0*/ ADD         R4, LR, R4, LSL R5 
    /*000086a4*/ ADD         LR, LR, R8, LSL R5 
    /*000086a8*/ LSL         R5, R2, R5 
    /*000086ac*/ MOV         R2, #0 
    AO_DRV_GetAECData_x86b0:
    /*000086b0*/ LDR         R0, [LR, R2] 
    /*000086b4*/ ADD         R1, R1, #4 
    /*000086b8*/ CMP         IP, R1 
    /*000086bc*/ ADD         R3, R3, #16 
    /*000086c0*/ STR         R0, [R3, #-16] 
    /*000086c4*/ LDR         R0, [R7, R2] 
    /*000086c8*/ STR         R0, [R3, #-12] 
    /*000086cc*/ LDR         R0, [R6, R2] 
    /*000086d0*/ STR         R0, [R3, #-8] 
    /*000086d4*/ LDR         R0, [R4, R2] 
    /*000086d8*/ ADD         R2, R2, R5 
    /*000086dc*/ STR         R0, [R3, #-4] 
    /*000086e0*/ BHI         AO_DRV_GetAECData_x86b0
    /*000086e4*/ B           AO_DRV_GetAECData_x8540
    AO_DRV_GetAECData_x86e8:
    /*000086e8*/ LDR         R1, [R5, #64]               @ 0x40 
    /*000086ec*/ SUB         R0, R0, R6 
    /*000086f0*/ LSL         R0, R0, #3 
    /*000086f4*/ LSL         R1, R1, #4 
    /*000086f8*/ BL          0x00000000000086F8          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000086fc*/ MOV         R3, #46                     @ 0x2e 
    /*00008700*/ MLA         R8, R3, R8, FP 
    /*00008704*/ LDR         IP, [R5, #60]               @ 0x3c 
    /*00008708*/ LDR         R3, [SL, #8] 
    /*0000870c*/ LDR         R5, [R5, #108]              @ 0x6c 
    /*00008710*/ ADD         R8, R8, #30 
    /*00008714*/ LDR         LR, [R7, R8, LSL #2] 
    /*00008718*/ CMP         IP, R0 
    /*0000871c*/ MOVCC       R0, IP 
    AO_DRV_GetAECData_x8720:
    /*00008720*/ ANDS        R6, R0, #3 
    /*00008724*/ LSL         FP, LR, #1 
    /*00008728*/ ADD         R4, R4, FP 
    /*0000872c*/ BEQ         AO_DRV_GetAECData_x8754
    /*00008730*/ MOV         R2, #1 
    /*00008734*/ MOV         R1, R4 
    /*00008738*/ ADD         R7, R3, R6, LSL R2 
    /*0000873c*/ LSL         R8, R2, R5 
    /*00008740*/ MOV         R2, R3 
    AO_DRV_GetAECData_x8744:
    /*00008744*/ LDRH        LR, [R1], R8 
    /*00008748*/ STRH        LR, [R2], #2 
    /*0000874c*/ CMP         R7, R2 
    /*00008750*/ BNE         AO_DRV_GetAECData_x8744
    AO_DRV_GetAECData_x8754:
    /*00008754*/ CMP         R6, R0 
    /*00008758*/ BCS         AO_DRV_GetAECData_x87bc
    /*0000875c*/ ADD         R9, R6, #1 
    /*00008760*/ ADD         R8, R6, #2 
    /*00008764*/ ADD         R7, R6, #3 
    /*00008768*/ MOV         SL, #4 
    /*0000876c*/ ADD         R9, R4, R9, LSL R5 
    /*00008770*/ ADD         R8, R4, R8, LSL R5 
    /*00008774*/ ADD         R7, R4, R7, LSL R5 
    /*00008778*/ LSL         SL, SL, R5 
    /*0000877c*/ ADD         R4, R4, R6, LSL R5 
    /*00008780*/ ADD         R1, R3, R6, LSL #1 
    /*00008784*/ MOV         R2, #0 
    AO_DRV_GetAECData_x8788:
    /*00008788*/ LDRH        LR, [R4, R2] 
    /*0000878c*/ ADD         R6, R6, #4 
    /*00008790*/ CMP         R0, R6 
    /*00008794*/ ADD         R1, R1, #8 
    /*00008798*/ STRH        LR, [R1, #-8] 
    /*0000879c*/ LDRH        LR, [R9, R2] 
    /*000087a0*/ STRH        LR, [R1, #-6] 
    /*000087a4*/ LDRH        LR, [R8, R2] 
    /*000087a8*/ STRH        LR, [R1, #-4] 
    /*000087ac*/ LDRH        LR, [R7, R2] 
    /*000087b0*/ ADD         R2, R2, SL 
    /*000087b4*/ STRH        LR, [R1, #-2] 
    /*000087b8*/ BHI         AO_DRV_GetAECData_x8788
    AO_DRV_GetAECData_x87bc:
    /*000087bc*/ CMP         R0, IP 
    /*000087c0*/ BEQ         AO_DRV_GetAECData_x8540
    /*000087c4*/ LDR         R1, [SP, #12] 
    /*000087c8*/ SUB         R2, IP, R0 
    /*000087cc*/ ADD         LR, R1, FP 
    /*000087d0*/ ANDS        FP, R2, #3 
    /*000087d4*/ BEQ         AO_DRV_GetAECData_x88c8
    /*000087d8*/ ADD         R1, R0, FP 
    /*000087dc*/ MOV         R2, #1 
    /*000087e0*/ LSL         R7, R2, R5 
    /*000087e4*/ ADD         R0, R3, R0, LSL R2 
    /*000087e8*/ ADD         R6, R3, R1, LSL R2 
    /*000087ec*/ MOV         R2, LR 
    AO_DRV_GetAECData_x87f0:
    /*000087f0*/ LDRH        R4, [R2], R7 
    /*000087f4*/ STRH        R4, [R0], #2 
    /*000087f8*/ CMP         R6, R0 
    /*000087fc*/ BNE         AO_DRV_GetAECData_x87f0
    AO_DRV_GetAECData_x8800:
    /*00008800*/ CMP         R1, IP 
    /*00008804*/ BCS         AO_DRV_GetAECData_x8540
    /*00008808*/ ADD         R7, FP, #1 
    /*0000880c*/ ADD         R6, FP, #2 
    /*00008810*/ ADD         R4, FP, #3 
    /*00008814*/ MOV         R2, #4 
    /*00008818*/ ADD         R3, R3, R1, LSL #1 
    /*0000881c*/ ADD         R7, LR, R7, LSL R5 
    /*00008820*/ ADD         R6, LR, R6, LSL R5 
    /*00008824*/ ADD         R4, LR, R4, LSL R5 
    /*00008828*/ ADD         LR, LR, FP, LSL R5 
    /*0000882c*/ LSL         R5, R2, R5 
    /*00008830*/ MOV         R2, #0 
    AO_DRV_GetAECData_x8834:
    /*00008834*/ LDRH        R0, [LR, R2] 
    /*00008838*/ ADD         R1, R1, #4 
    /*0000883c*/ CMP         R1, IP 
    /*00008840*/ ADD         R3, R3, #8 
    /*00008844*/ STRH        R0, [R3, #-8] 
    /*00008848*/ LDRH        R0, [R7, R2] 
    /*0000884c*/ STRH        R0, [R3, #-6] 
    /*00008850*/ LDRH        R0, [R6, R2] 
    /*00008854*/ STRH        R0, [R3, #-4] 
    /*00008858*/ LDRH        R0, [R4, R2] 
    /*0000885c*/ ADD         R2, R2, R5 
    /*00008860*/ STRH        R0, [R3, #-2] 
    /*00008864*/ BCC         AO_DRV_GetAECData_x8834
    /*00008868*/ B           AO_DRV_GetAECData_x8540
    AO_DRV_GetAECData_x886c:
    /*0000886c*/ CMP         R9, #1 
    /*00008870*/ BEQ         AO_DRV_GetAECData_x8720
    AO_DRV_GetAECData_x8874:
    /*00008874*/ MOVW        R3, #1577                   @ 0x629 
    /*00008878*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1870] 
    /*0000887c*/ STR         R3, [SP] 
    /*00008880*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1870] 
    /*00008884*/ LDR         R3, [PC, #164]              @ 0x0000000000008930 
    /*00008888*/ MOV         R1, #22 
    /*0000888c*/ MOV         R0, #6 
    /*00008890*/ BL          0x0000000000008890          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008894*/ MOVW        R0, #32776                  @ 0x8008 
    /*00008898*/ MOVT        R0, #40982                  @ 0xa016 
    /*0000889c*/ B           AO_DRV_GetAECData_x8544
    AO_DRV_GetAECData_x88a0:
    /*000088a0*/ MOVW        R3, #0                      @ .LC74 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e08] 
    /*000088a4*/ MOVW        R0, #0                      @ .LC75 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e18] 
    /*000088a8*/ MOVT        R3, #0                      @ .LC74 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e08] 
    /*000088ac*/ MOVT        R0, #0                      @ .LC75 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e18] 
    /*000088b0*/ MOV         R2, #1408                   @ 0x580 
    /*000088b4*/ LDR         R1, [PC, #116]              @ 0x0000000000008930 
    /*000088b8*/ BL          0x00000000000088B8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000088bc*/ B           AO_DRV_GetAECData_x8360
    AO_DRV_GetAECData_x88c0:
    /*000088c0*/ MOV         R1, R0 
    /*000088c4*/ B           AO_DRV_GetAECData_x867c
    AO_DRV_GetAECData_x88c8:
    /*000088c8*/ MOV         R1, R0 
    /*000088cc*/ B           AO_DRV_GetAECData_x8800
    AO_DRV_GetAECData_x88d0:
    /*000088d0*/ MOV         R1, R0 
    /*000088d4*/ B           AO_DRV_GetAECData_x84d8
    AO_DRV_GetAECData_x88d8:
    /*000088d8*/ MOV         R3, #1424                   @ 0x590 
    /*000088dc*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x183c] 
    /*000088e0*/ STR         R3, [SP] 
    /*000088e4*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x183c] 
    /*000088e8*/ LDR         R3, [PC, #64]               @ 0x0000000000008930 
    /*000088ec*/ MOV         R1, #22 
    /*000088f0*/ MOV         R0, #3 
    /*000088f4*/ BL          0x00000000000088F4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000088f8*/ MOVW        R0, #32771                  @ 0x8003 
    /*000088fc*/ MOVT        R0, #40982                  @ 0xa016 
    /*00008900*/ B           AO_DRV_GetAECData_x8544
    AO_DRV_GetAECData_x8904:
    /*00008904*/ MOVW        R3, #1417                   @ 0x589 
    /*00008908*/ MOVW        R2, #0                      @ .LC76 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e58] 
    /*0000890c*/ STR         R3, [SP] 
    /*00008910*/ MOVT        R2, #0                      @ .LC76 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e58] 
    /*00008914*/ LDR         R3, [PC, #20]               @ 0x0000000000008930 
    /*00008918*/ MOV         R1, #22 
    /*0000891c*/ MOV         R0, #3 
    /*00008920*/ BL          0x0000000000008920          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008924*/ MOVW        R0, #32773                  @ 0x8005 
    /*00008928*/ MOVT        R0, #40982                  @ 0xa016 
    /*0000892c*/ B           AO_DRV_GetAECData_x8544
    /*00008930*/ ANDEQ       R0, R0, IP, ROR #6 
FUNC_END AO_DRV_GetAECData


FUNC_BEGIN AO_DRV_GetDmaRdpt
    /*00008934*/ PUSH        {R4, R5, LR} 
    /*00008938*/ MOV         R5, R0 
    /*0000893c*/ SUB         SP, SP, #12 
    /*00008940*/ MOV         R0, #20 
    /*00008944*/ MOV         R4, R1 
    /*00008948*/ BL          0x0000000000008948          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000894c*/ SUBS        R3, R0, #0 
    /*00008950*/ BEQ         AO_DRV_GetDmaRdpt_x896c
    /*00008954*/ LDR         R3, [R3, #104]              @ 0x68 
    /*00008958*/ MOV         R0, R5 
    /*0000895c*/ BLX         R3 
    /*00008960*/ STR         R0, [R4] 
    /*00008964*/ ADD         SP, SP, #12 
    /*00008968*/ POP         {R4, R5, PC} 
    AO_DRV_GetDmaRdpt_x896c:
    /*0000896c*/ MOVW        R3, #1595                   @ 0x63b 
    /*00008970*/ MOV         R1, #22 
    /*00008974*/ STR         R3, [SP] 
    /*00008978*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1728] 
    /*0000897c*/ LDR         R3, [PC, #12]               @ 0x0000000000008990 
    /*00008980*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1728] 
    /*00008984*/ BL          0x0000000000008984          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008988*/ ADD         SP, SP, #12 
    /*0000898c*/ POP         {R4, R5, PC} 
    /*00008990*/ ANDEQ       R0, R0, R0, LSL #7 
FUNC_END AO_DRV_GetDmaRdpt


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .init.text
@ Size: 0x10
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN ao_mod_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ BL          0x0000000000000004          @ AO_ModInit [R_ARM_CALL: .text+0x3428] 
    /*00000008*/ MOV         R0, #0 
    /*0000000c*/ POP         {R4, PC} 
FUNC_END ao_mod_init




@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .exit.text
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN ao_mod_exit
    ao_mod_exit_x0:
    /*00000000*/ B           ao_mod_exit_x0              @ AO_ModExit [R_ARM_JUMP24: .text+0x3570] 
FUNC_END ao_mod_exit




