@
@ -- FILES --
@

@ ai.c
@ ai_trans.c
@ audio_fb.c
@ ai_drv.c
@ ai_init.c
@ hi3516cv500_ai.mod.c


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text
@ Size: 0x9d28
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN ai_restore
    /*00000000*/ MOV         R0, #0 
    /*00000004*/ BX          LR 
FUNC_END ai_restore


FUNC_BEGIN AI_GetVerMagic
    /*00000008*/ MOVW        R0, #61479                  @ 0xf027 
    /*0000000c*/ MOVT        R0, #307                    @ 0x133 
    /*00000010*/ BX          LR 
FUNC_END AI_GetVerMagic


FUNC_BEGIN AiVbPhy2Vir
    /*00000014*/ PUSH        {R4, R6, R7, LR} 
    /*00000018*/ MOV         R6, R0 
    /*0000001c*/ MOV         R0, #1 
    /*00000020*/ MOV         R7, R1 
    /*00000024*/ BL          0x0000000000000024          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000028*/ LDR         R4, [R0, #48]               @ 0x30 
    /*0000002c*/ MOV         R0, #1 
    /*00000030*/ BL          0x0000000000000030          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000034*/ MOV         R1, R7 
    /*00000038*/ LDR         R3, [R0, #36]               @ 0x24 
    /*0000003c*/ MOV         R0, R6 
    /*00000040*/ BLX         R3 
    /*00000044*/ BLX         R4 
    /*00000048*/ POP         {R4, R6, R7, PC} 
FUNC_END AiVbPhy2Vir


FUNC_BEGIN AI_Notify
    /*0000004c*/ PUSH        {R4, LR} 
    /*00000050*/ MOVW        R4, #0                      @ g_stAiChn [R_ARM_MOVW_ABS_NC: .bss+0x90] 
    /*00000054*/ MOVT        R4, #0                      @ g_stAiChn [R_ARM_MOVT_ABS: .bss+0x90] 
    /*00000058*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000005c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000060*/ MOV         R2, #1 
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ STR         R2, [R3] 
    /*0000006c*/ BL          0x000000000000006C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000070*/ CMP         R0, #0 
    /*00000074*/ BNE         AI_Notify_x90
    /*00000078*/ LDR         R3, [R4, #208]              @ 0xd0 
    /*0000007c*/ CMP         R3, #1 
    /*00000080*/ BEQ         AI_Notify_xcc
    AI_Notify_x84:
    /*00000084*/ MOVW        R0, #0                      @ g_stAiChn [R_ARM_MOVW_ABS_NC: .bss+0x90] 
    /*00000088*/ MOVT        R0, #0                      @ g_stAiChn [R_ARM_MOVT_ABS: .bss+0x90] 
    /*0000008c*/ BL          0x000000000000008C          @ osal_up [R_ARM_CALL: +0x0] 
    AI_Notify_x90:
    /*00000090*/ LDR         R0, [PC, #72]               @ 0x00000000000000e0 
    /*00000094*/ BL          0x0000000000000094          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000098*/ CMP         R0, #0 
    /*0000009c*/ POPNE       {R4, PC} 
    /*000000a0*/ LDR         R3, [R4, #1328]             @ 0x530 
    /*000000a4*/ CMP         R3, #1 
    /*000000a8*/ BEQ         AI_Notify_xb8
    AI_Notify_xac:
    /*000000ac*/ LDR         R0, [PC, #44]               @ 0x00000000000000e0 
    /*000000b0*/ POP         {R4, LR} 
    AI_Notify_xb4:
    /*000000b4*/ B           AI_Notify_xb4               @ osal_up [R_ARM_JUMP24: +0x0] 
    AI_Notify_xb8:
    /*000000b8*/ LDR         R0, [PC, #36]               @ 0x00000000000000e4 
    /*000000bc*/ BL          0x00000000000000BC          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*000000c0*/ LDR         R0, [PC, #32]               @ 0x00000000000000e8 
    /*000000c4*/ BL          0x00000000000000C4          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*000000c8*/ B           AI_Notify_xac
    AI_Notify_xcc:
    /*000000cc*/ ADD         R0, R4, #4 
    /*000000d0*/ BL          0x00000000000000D0          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*000000d4*/ ADD         R0, R4, #8 
    /*000000d8*/ BL          0x00000000000000D8          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*000000dc*/ B           AI_Notify_x84
    /*000000e0*/ ANDEQ       R0, R0, R0, ROR #8          @ g_stAiChn [R_ARM_ABS32: .bss+0x90] 
    /*000000e4*/ ANDEQ       R0, R0, R4, ROR #8          @ g_stAiChn [R_ARM_ABS32: .bss+0x90] 
    /*000000e8*/ ANDEQ       R0, R0, R8, ROR #8          @ g_stAiChn [R_ARM_ABS32: .bss+0x90] 
FUNC_END AI_Notify


FUNC_BEGIN AI_Init
    /*000000ec*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*000000f0*/ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000000f4*/ MOVT        R6, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000000f8*/ MOV         R5, #0 
    /*000000fc*/ SUB         SP, SP, #28 
    /*00000100*/ LDR         R4, [R6] 
    /*00000104*/ STR         R5, [SP, #12] 
    /*00000108*/ CMP         R4, R5 
    /*0000010c*/ STR         R5, [SP, #8] 
    /*00000110*/ STR         R5, [SP, #16] 
    /*00000114*/ STR         R5, [SP, #20] 
    /*00000118*/ BEQ         AI_Init_x1f8
    /*0000011c*/ CMP         R4, #1 
    /*00000120*/ BEQ         AI_Init_x250
    /*00000124*/ MOV         R0, R5 
    /*00000128*/ BL          0x0000000000000128          @ AI_DEV_Init [R_ARM_CALL: .text+0x5450] 
    /*0000012c*/ SUBS        R4, R0, #0 
    /*00000130*/ BNE         AI_Init_x228
    /*00000134*/ LDR         R9, [PC, #324]              @ 0x0000000000000280 
    /*00000138*/ MOVW        R8, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30] 
    /*0000013c*/ MOVW        R7, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000140*/ MOVT        R8, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30] 
    /*00000144*/ MOVT        R7, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    AI_Init_x148:
    /*00000148*/ MOV         R0, R4 
    /*0000014c*/ BL          0x000000000000014C          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*00000150*/ SUBS        R5, R0, #0 
    /*00000154*/ BEQ         AI_Init_x1bc
    AI_Init_x158:
    /*00000158*/ ADD         R4, R4, #1 
    /*0000015c*/ MOV         R3, #0 
    /*00000160*/ CMP         R4, #2 
    /*00000164*/ STR         R3, [R5, #212]              @ 0xd4 
    /*00000168*/ MVN         R3, #0 
    /*0000016c*/ STR         R3, [R5, #216]              @ 0xd8 
    /*00000170*/ STR         R3, [R5, #220]              @ 0xdc 
    /*00000174*/ BNE         AI_Init_x148
    AI_Init_x178:
    /*00000178*/ MOV         R3, #2 
    /*0000017c*/ MOV         R2, #21 
    /*00000180*/ MOV         R0, R3 
    /*00000184*/ STR         R3, [SP, #16] 
    /*00000188*/ STR         R2, [SP, #8] 
    /*0000018c*/ MOV         R3, #1 
    /*00000190*/ STR         R3, [SP, #12] 
    /*00000194*/ BL          0x0000000000000194          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000198*/ LDR         R3, [R0, #24] 
    /*0000019c*/ ADD         R0, SP, #8 
    /*000001a0*/ BLX         R3 
    /*000001a4*/ CMP         R0, #0 
    /*000001a8*/ BNE         AI_Init_x1d4
    AI_Init_x1ac:
    /*000001ac*/ MOV         R0, #0 
    /*000001b0*/ STR         R0, [R6] 
    AI_Init_x1b4:
    /*000001b4*/ ADD         SP, SP, #28 
    /*000001b8*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AI_Init_x1bc:
    /*000001bc*/ MOV         R3, R8 
    /*000001c0*/ MOVW        R2, #1716                   @ 0x6b4 
    /*000001c4*/ MOV         R1, R9 
    /*000001c8*/ MOV         R0, R7 
    /*000001cc*/ BL          0x00000000000001CC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000001d0*/ B           AI_Init_x158
    AI_Init_x1d4:
    /*000001d4*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xec] 
    /*000001d8*/ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000001dc*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xec] 
    /*000001e0*/ MOVT        R1, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*000001e4*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000001e8*/ MOVW        R2, #1765                   @ 0x6e5 
    /*000001ec*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000001f0*/ BL          0x00000000000001F0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000001f4*/ B           AI_Init_x1ac
    AI_Init_x1f8:
    /*000001f8*/ MOVW        R2, #1746                   @ 0x6d2 
    /*000001fc*/ MOV         R1, #21 
    /*00000200*/ STR         R2, [SP] 
    /*00000204*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000208*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*0000020c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*00000210*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*00000214*/ MOV         R0, #6 
    /*00000218*/ BL          0x0000000000000218          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000021c*/ MOV         R0, R4 
    /*00000220*/ ADD         SP, SP, #28 
    /*00000224*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AI_Init_x228:
    /*00000228*/ MOVW        R3, #1708                   @ 0x6ac 
    /*0000022c*/ STR         R5, [SP, #4] 
    /*00000230*/ STR         R3, [SP] 
    /*00000234*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x84] 
    /*00000238*/ LDR         R3, [PC, #64]               @ 0x0000000000000280 
    /*0000023c*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x84] 
    /*00000240*/ MOV         R1, #21 
    /*00000244*/ MOV         R0, #4 
    /*00000248*/ BL          0x0000000000000248          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000024c*/ B           AI_Init_x178
    AI_Init_x250:
    /*00000250*/ MOVW        R2, #1751                   @ 0x6d7 
    /*00000254*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000258*/ STR         R2, [SP] 
    /*0000025c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*00000260*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc0] 
    /*00000264*/ MOV         R1, #21 
    /*00000268*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc0] 
    /*0000026c*/ MOV         R0, #3 
    /*00000270*/ BL          0x0000000000000270          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000274*/ MOVW        R0, #32786                  @ 0x8012 
    /*00000278*/ MOVT        R0, #40981                  @ 0xa015 
    /*0000027c*/ B           AI_Init_x1b4
    /*00000280*/ ANDEQ       R0, R0, R8 
FUNC_END AI_Init


FUNC_BEGIN AI_QueryState
    /*00000284*/ PUSH        {R4, LR} 
    /*00000288*/ MOV         R4, R0 
    /*0000028c*/ MOVW        R0, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000290*/ MOVT        R0, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000294*/ BL          0x0000000000000294          @ osal_atomic_read [R_ARM_CALL: +0x0] 
    /*00000298*/ CMP         R0, #0 
    /*0000029c*/ MOVEQ       R3, #17 
    /*000002a0*/ MOVNE       R3, #34                     @ 0x22 
    /*000002a4*/ STR         R3, [R4] 
    /*000002a8*/ POP         {R4, PC} 
FUNC_END AI_QueryState


FUNC_BEGIN ai_poll
    /*000002ac*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000002b0*/ MOV         R7, R0 
    /*000002b4*/ LDR         R4, [R1] 
    /*000002b8*/ SUB         SP, SP, #12 
    /*000002bc*/ MOV         R0, R4 
    /*000002c0*/ ADD         R4, R4, R4, LSR #31 
    /*000002c4*/ BL          0x00000000000002C4          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*000002c8*/ MOV         R6, R0 
    /*000002cc*/ ASR         R0, R4, #1 
    /*000002d0*/ BL          0x00000000000002D0          @ AI_GetDevCtx [R_ARM_CALL: .text+0x56c4] 
    /*000002d4*/ CMP         R0, #0 
    /*000002d8*/ CMPNE       R6, #0 
    /*000002dc*/ MOVEQ       R5, #1 
    /*000002e0*/ MOVNE       R5, #0 
    /*000002e4*/ MOVEQ       R5, #0 
    /*000002e8*/ BEQ         ai_poll_x2f8
    /*000002ec*/ LDR         R3, [R0, #12] 
    /*000002f0*/ CMP         R3, #1 
    /*000002f4*/ BEQ         ai_poll_x304
    ai_poll_x2f8:
    /*000002f8*/ MOV         R0, R5 
    /*000002fc*/ ADD         SP, SP, #12 
    /*00000300*/ POP         {R4, R5, R6, R7, PC} 
    ai_poll_x304:
    /*00000304*/ LDR         R3, [R6, #208]              @ 0xd0 
    /*00000308*/ CMP         R3, #1 
    /*0000030c*/ BNE         ai_poll_x2f8
    /*00000310*/ LDR         R5, [R6, #148]              @ 0x94 
    /*00000314*/ CMP         R5, #0 
    /*00000318*/ BEQ         ai_poll_x2f8
    /*0000031c*/ ADD         R4, R0, #128                @ 0x80 
    /*00000320*/ ADD         R1, R6, #8 
    /*00000324*/ MOV         R0, R7 
    /*00000328*/ BL          0x0000000000000328          @ osal_poll_wait [R_ARM_CALL: +0x0] 
    /*0000032c*/ ADD         R1, SP, #4 
    /*00000330*/ MOV         R0, R4 
    /*00000334*/ BL          0x0000000000000334          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000338*/ ADD         R0, R6, #180                @ 0xb4 
    /*0000033c*/ BL          0x000000000000033C          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*00000340*/ ADD         R1, SP, #4 
    /*00000344*/ CMP         R0, #0 
    /*00000348*/ MOV         R0, R4 
    /*0000034c*/ MOVNE       R5, #65                     @ 0x41 
    /*00000350*/ MOVEQ       R5, #0 
    /*00000354*/ BL          0x0000000000000354          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000358*/ B           ai_poll_x2f8
FUNC_END ai_poll


FUNC_BEGIN AiGetFrame_AieWaitConditionCallBack
    /*0000035c*/ PUSH        {R4, LR} 
    /*00000360*/ SUBS        R4, R0, #0 
    /*00000364*/ BEQ         AiGetFrame_AieWaitConditionCallBack_x3b0
    /*00000368*/ ADD         R0, R4, #180                @ 0xb4 
    /*0000036c*/ BL          0x000000000000036C          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*00000370*/ LDR         R3, [R4, #208]              @ 0xd0 
    /*00000374*/ CMP         R0, #0 
    /*00000378*/ BNE         AiGetFrame_AieWaitConditionCallBack_x390
    /*0000037c*/ SUB         R3, R3, #1 
    /*00000380*/ CMP         R3, #1 
    /*00000384*/ BLS         AiGetFrame_AieWaitConditionCallBack_x398
    AiGetFrame_AieWaitConditionCallBack_x388:
    /*00000388*/ MOV         R0, #1 
    /*0000038c*/ POP         {R4, PC} 
    AiGetFrame_AieWaitConditionCallBack_x390:
    /*00000390*/ CMP         R3, #2 
    /*00000394*/ BNE         AiGetFrame_AieWaitConditionCallBack_x388
    AiGetFrame_AieWaitConditionCallBack_x398:
    /*00000398*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000039c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000003a0*/ LDR         R4, [R3] 
    /*000003a4*/ ADDS        R0, R4, #0 
    /*000003a8*/ MOVNE       R0, #1 
    /*000003ac*/ POP         {R4, PC} 
    AiGetFrame_AieWaitConditionCallBack_x3b0:
    /*000003b0*/ MOV         R0, R4 
    /*000003b4*/ POP         {R4, PC} 
FUNC_END AiGetFrame_AieWaitConditionCallBack


FUNC_BEGIN AI_Exit
    /*000003b8*/ PUSH        {R4, LR} 
    /*000003bc*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000003c0*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000003c4*/ LDR         R3, [R4] 
    /*000003c8*/ CMP         R3, #2 
    /*000003cc*/ POPEQ       {R4, PC} 
    /*000003d0*/ MOV         R0, #2 
    /*000003d4*/ BL          0x00000000000003D4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000003d8*/ LDR         R3, [R0, #28] 
    /*000003dc*/ MOV         R0, #21 
    /*000003e0*/ BLX         R3 
    /*000003e4*/ MOV         R0, #0 
    /*000003e8*/ BL          0x00000000000003E8          @ AI_DEV_Exit [R_ARM_CALL: .text+0x554c] 
    /*000003ec*/ MOV         R3, #2 
    /*000003f0*/ STR         R3, [R4] 
    /*000003f4*/ POP         {R4, PC} 
FUNC_END AI_Exit


FUNC_BEGIN ai_close
    /*000003f8*/ MOV         R0, #0 
    /*000003fc*/ BX          LR 
FUNC_END ai_close


FUNC_BEGIN ai_freeze
    /*00000400*/ MOV         R0, #0 
    /*00000404*/ BX          LR 
FUNC_END ai_freeze


FUNC_BEGIN ai_open
    /*00000408*/ MOV         R0, #0 
    /*0000040c*/ BX          LR 
FUNC_END ai_open


FUNC_BEGIN AiGetFrame_UserWaitConditionCallBack
    /*00000410*/ PUSH        {R4, LR} 
    /*00000414*/ SUBS        R4, R0, #0 
    /*00000418*/ BEQ         AiGetFrame_UserWaitConditionCallBack_x464
    /*0000041c*/ ADD         R0, R4, #152                @ 0x98 
    /*00000420*/ BL          0x0000000000000420          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*00000424*/ LDR         R3, [R4, #208]              @ 0xd0 
    /*00000428*/ CMP         R0, #0 
    /*0000042c*/ BNE         AiGetFrame_UserWaitConditionCallBack_x444
    /*00000430*/ SUB         R3, R3, #1 
    /*00000434*/ CMP         R3, #1 
    /*00000438*/ BLS         AiGetFrame_UserWaitConditionCallBack_x44c
    AiGetFrame_UserWaitConditionCallBack_x43c:
    /*0000043c*/ MOV         R0, #1 
    /*00000440*/ POP         {R4, PC} 
    AiGetFrame_UserWaitConditionCallBack_x444:
    /*00000444*/ CMP         R3, #2 
    /*00000448*/ BNE         AiGetFrame_UserWaitConditionCallBack_x43c
    AiGetFrame_UserWaitConditionCallBack_x44c:
    /*0000044c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000450*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000454*/ LDR         R4, [R3] 
    /*00000458*/ ADDS        R0, R4, #0 
    /*0000045c*/ MOVNE       R0, #1 
    /*00000460*/ POP         {R4, PC} 
    AiGetFrame_UserWaitConditionCallBack_x464:
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ POP         {R4, PC} 
FUNC_END AiGetFrame_UserWaitConditionCallBack


FUNC_BEGIN AIEnableChn
    /*0000046c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000470*/ MOV         R4, R0 
    /*00000474*/ SUB         SP, SP, #36                 @ 0x24 
    /*00000478*/ MOV         R0, #20 
    /*0000047c*/ MOV         R5, R1 
    /*00000480*/ BL          0x0000000000000480          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000484*/ CMP         R0, #0 
    /*00000488*/ BEQ         AIEnableChn_x6fc
    /*0000048c*/ CMP         R4, #0 
    /*00000490*/ BNE         AIEnableChn_x68c
    /*00000494*/ CMP         R5, #1 
    /*00000498*/ BHI         AIEnableChn_x5f4
    /*0000049c*/ MOV         R0, R4 
    /*000004a0*/ BL          0x00000000000004A0          @ AI_GetDevCtx [R_ARM_CALL: .text+0x56c4] 
    /*000004a4*/ SUBS        R8, R0, #0 
    /*000004a8*/ BEQ         AIEnableChn_x628
    AIEnableChn_x4ac:
    /*000004ac*/ ADD         R3, R8, #124                @ 0x7c 
    /*000004b0*/ STR         R3, [SP, #16] 
    /*000004b4*/ MOV         R0, R3 
    /*000004b8*/ BL          0x00000000000004B8          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000004bc*/ SUBS        R7, R0, #0 
    /*000004c0*/ BNE         AIEnableChn_x680
    /*000004c4*/ LDR         R1, [R8, #12] 
    /*000004c8*/ CMP         R1, #1 
    /*000004cc*/ BNE         AIEnableChn_x648
    /*000004d0*/ LDR         R3, [R8, #28] 
    /*000004d4*/ LDR         R4, [R8, #44]               @ 0x2c 
    /*000004d8*/ CMP         R3, #1 
    /*000004dc*/ MOV         R0, R4 
    /*000004e0*/ MOVEQ       R1, #2 
    /*000004e4*/ MOV         SL, R1 
    /*000004e8*/ BL          0x00000000000004E8          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000004ec*/ CMP         R5, R0 
    /*000004f0*/ BCS         AIEnableChn_x6bc
    /*000004f4*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*000004f8*/ MOVW        FP, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000004fc*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*00000500*/ MOVT        FP, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000504*/ STR         R3, [SP, #20] 
    /*00000508*/ LDR         R9, [PC, #504]              @ 0x0000000000000708 
    /*0000050c*/ B           AIEnableChn_x594
    AIEnableChn_x510:
    /*00000510*/ MOV         R0, R4 
    /*00000514*/ BL          0x0000000000000514          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000518*/ ADD         R1, SP, #28 
    /*0000051c*/ SUBS        R6, R0, #0 
    /*00000520*/ LDR         R0, [PC, #484]              @ 0x000000000000070c 
    /*00000524*/ BNE         AIEnableChn_x5dc
    /*00000528*/ BL          0x0000000000000528          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000052c*/ ADD         R1, SP, #28 
    /*00000530*/ MOV         R3, #1 
    /*00000534*/ LDR         R0, [PC, #464]              @ 0x000000000000070c 
    /*00000538*/ STR         R3, [R4, #208]              @ 0xd0 
    /*0000053c*/ BL          0x000000000000053C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000540*/ STR         R6, [R4, #1104]             @ 0x450 
    /*00000544*/ STR         R6, [R4, #1108]             @ 0x454 
    /*00000548*/ MOV         R0, R4 
    /*0000054c*/ LDR         R3, [R8, #36]               @ 0x24 
    /*00000550*/ CMP         R3, #30 
    /*00000554*/ MOVCS       R3, #30 
    /*00000558*/ STR         R3, [R4, #148]              @ 0x94 
    /*0000055c*/ BL          0x000000000000055C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000560*/ MOV         R3, #125                    @ 0x7d 
    /*00000564*/ STR         R5, [SP, #8] 
    /*00000568*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x230] 
    /*0000056c*/ STR         R3, [SP] 
    /*00000570*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x230] 
    /*00000574*/ STR         R6, [SP, #4] 
    /*00000578*/ MOV         R3, R9 
    /*0000057c*/ MOV         R1, #21 
    /*00000580*/ MOV         R0, #6 
    /*00000584*/ BL          0x0000000000000584          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000588*/ CMP         SL, R7 
    /*0000058c*/ BEQ         AIEnableChn_x5c8
    /*00000590*/ LDR         R4, [R8, #44]               @ 0x2c 
    AIEnableChn_x594:
    /*00000594*/ LSR         R4, R4, #1 
    /*00000598*/ MLA         R5, R7, R4, R5 
    /*0000059c*/ ADD         R7, R7, #1 
    /*000005a0*/ MOV         R0, R5 
    /*000005a4*/ BL          0x00000000000005A4          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*000005a8*/ SUBS        R4, R0, #0 
    /*000005ac*/ BNE         AIEnableChn_x510
    /*000005b0*/ LDR         R3, [SP, #20] 
    /*000005b4*/ MOV         R2, #106                    @ 0x6a 
    /*000005b8*/ MOV         R1, R9 
    /*000005bc*/ MOV         R0, FP 
    /*000005c0*/ BL          0x00000000000005C0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000005c4*/ B           AIEnableChn_x510
    AIEnableChn_x5c8:
    /*000005c8*/ LDR         R0, [SP, #16] 
    /*000005cc*/ BL          0x00000000000005CC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000005d0*/ MOV         R0, R6 
    AIEnableChn_x5d4:
    /*000005d4*/ ADD         SP, SP, #36                 @ 0x24 
    /*000005d8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIEnableChn_x5dc:
    /*000005dc*/ LDR         R0, [SP, #16] 
    /*000005e0*/ BL          0x00000000000005E0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000005e4*/ MOV         R0, #65024                  @ 0xfe00 
    /*000005e8*/ MOVT        R0, #65535                  @ 0xffff 
    /*000005ec*/ ADD         SP, SP, #36                 @ 0x24 
    /*000005f0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIEnableChn_x5f4:
    /*000005f4*/ STR         R5, [SP, #4] 
    /*000005f8*/ MOV         R3, #74                     @ 0x4a 
    /*000005fc*/ MOV         R1, #20 
    /*00000600*/ STR         R3, [SP] 
    /*00000604*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00000608*/ LDR         R3, [PC, #248]              @ 0x0000000000000708 
    /*0000060c*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*00000610*/ MOV         R0, #3 
    /*00000614*/ BL          0x0000000000000614          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000618*/ MOVW        R0, #32770                  @ 0x8002 
    /*0000061c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000620*/ ADD         SP, SP, #36                 @ 0x24 
    /*00000624*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIEnableChn_x628:
    /*00000628*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*0000062c*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000630*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*00000634*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000638*/ MOV         R2, #76                     @ 0x4c 
    /*0000063c*/ LDR         R1, [PC, #196]              @ 0x0000000000000708 
    /*00000640*/ BL          0x0000000000000640          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000644*/ B           AIEnableChn_x4ac
    AIEnableChn_x648:
    /*00000648*/ LDR         R0, [SP, #16] 
    /*0000064c*/ BL          0x000000000000064C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000650*/ MOV         R3, #86                     @ 0x56 
    /*00000654*/ STR         R7, [SP, #4] 
    /*00000658*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x180] 
    /*0000065c*/ STR         R3, [SP] 
    /*00000660*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x180] 
    /*00000664*/ LDR         R3, [PC, #156]              @ 0x0000000000000708 
    /*00000668*/ MOV         R1, #21 
    /*0000066c*/ MOV         R0, #3 
    /*00000670*/ BL          0x0000000000000670          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000674*/ MOVW        R0, #32773                  @ 0x8005 
    /*00000678*/ MOVT        R0, #40981                  @ 0xa015 
    /*0000067c*/ B           AIEnableChn_x5d4
    AIEnableChn_x680:
    /*00000680*/ MOV         R0, #65024                  @ 0xfe00 
    /*00000684*/ MOVT        R0, #65535                  @ 0xffff 
    /*00000688*/ B           AIEnableChn_x5d4
    AIEnableChn_x68c:
    /*0000068c*/ MOV         R3, #73                     @ 0x49 
    /*00000690*/ STR         R4, [SP, #4] 
    /*00000694*/ STR         R3, [SP] 
    /*00000698*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*0000069c*/ LDR         R3, [PC, #100]              @ 0x0000000000000708 
    /*000006a0*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*000006a4*/ MOV         R1, #21 
    /*000006a8*/ MOV         R0, #3 
    /*000006ac*/ BL          0x00000000000006AC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000006b0*/ MOVW        R0, #32769                  @ 0x8001 
    /*000006b4*/ MOVT        R0, #40981                  @ 0xa015 
    /*000006b8*/ B           AIEnableChn_x5d4
    AIEnableChn_x6bc:
    /*000006bc*/ LDR         R0, [SP, #16] 
    /*000006c0*/ BL          0x00000000000006C0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000006c4*/ MOV         R2, #0 
    /*000006c8*/ MOV         R3, #96                     @ 0x60 
    /*000006cc*/ STR         R2, [SP, #8] 
    /*000006d0*/ STR         R3, [SP] 
    /*000006d4*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*000006d8*/ STR         R5, [SP, #4] 
    /*000006dc*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*000006e0*/ LDR         R3, [PC, #32]               @ 0x0000000000000708 
    /*000006e4*/ MOV         R1, #21 
    /*000006e8*/ MOV         R0, #3 
    /*000006ec*/ BL          0x00000000000006EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000006f0*/ MOVW        R0, #32770                  @ 0x8002 
    /*000006f4*/ MOVT        R0, #40981                  @ 0xa015 
    /*000006f8*/ B           AIEnableChn_x5d4
    AIEnableChn_x6fc:
    /*000006fc*/ MOVW        R0, #32784                  @ 0x8010 
    /*00000700*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000704*/ B           AIEnableChn_x5d4
    /*00000708*/ ANDEQ       R0, R0, R0, LSL R0 
    /*0000070c*/ ANDEQ       R0, R0, R0, LSL #1          @ g_stAiDev [R_ARM_ABS32: .bss+0x8] 
FUNC_END AIEnableChn


FUNC_BEGIN AIDisableChn
    /*00000710*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000714*/ MOV         R4, R0 
    /*00000718*/ SUB         SP, SP, #36                 @ 0x24 
    /*0000071c*/ MOV         R0, #20 
    /*00000720*/ MOV         R9, R1 
    /*00000724*/ BL          0x0000000000000724          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000728*/ CMP         R0, #0 
    /*0000072c*/ BEQ         AIDisableChn_xa14
    /*00000730*/ CMP         R4, #0 
    /*00000734*/ BNE         AIDisableChn_x9e4
    /*00000738*/ CMP         R9, #1 
    /*0000073c*/ BHI         AIDisableChn_x984
    /*00000740*/ MOV         R0, R4 
    /*00000744*/ BL          0x0000000000000744          @ AI_GetDevCtx [R_ARM_CALL: .text+0x56c4] 
    /*00000748*/ SUBS        SL, R0, #0 
    /*0000074c*/ BEQ         AIDisableChn_x9b8
    AIDisableChn_x750:
    /*00000750*/ ADD         R3, SL, #124                @ 0x7c 
    /*00000754*/ STR         R3, [SP, #12] 
    /*00000758*/ MOV         R0, R3 
    /*0000075c*/ MOV         R4, R3 
    /*00000760*/ BL          0x0000000000000760          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000764*/ SUBS        R8, R0, #0 
    /*00000768*/ BNE         AIDisableChn_x9d8
    /*0000076c*/ LDR         R3, [SL, #12] 
    /*00000770*/ CMP         R3, #1 
    /*00000774*/ BNE         AIDisableChn_x8e4
    /*00000778*/ LDR         R3, [SL, #28] 
    /*0000077c*/ LDR         R4, [SL, #44]               @ 0x2c 
    /*00000780*/ CMP         R3, #1 
    /*00000784*/ MOVNE       R3, #1 
    /*00000788*/ MOVEQ       R3, #2 
    /*0000078c*/ MOV         R0, R4 
    /*00000790*/ MOV         R1, R3 
    /*00000794*/ STR         R3, [SP, #8] 
    /*00000798*/ BL          0x0000000000000798          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000079c*/ CMP         R9, R0 
    /*000007a0*/ BGE         AIDisableChn_x934
    /*000007a4*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*000007a8*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*000007ac*/ STR         R3, [SP, #16] 
    /*000007b0*/ MOV         R3, R4 
    /*000007b4*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000007b8*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000007bc*/ STR         R2, [SP, #20] 
    AIDisableChn_x7c0:
    /*000007c0*/ LSR         R3, R3, #1 
    /*000007c4*/ MLA         R9, R8, R3, R9 
    /*000007c8*/ MOV         R0, R9 
    /*000007cc*/ BL          0x00000000000007CC          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*000007d0*/ SUBS        R7, R0, #0 
    /*000007d4*/ BEQ         AIDisableChn_x8f8
    /*000007d8*/ MOV         R0, R7 
    /*000007dc*/ BL          0x00000000000007DC          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000007e0*/ CMP         R0, #0 
    /*000007e4*/ BNE         AIDisableChn_x91c
    AIDisableChn_x7e8:
    /*000007e8*/ LDR         R4, [R7, #208]              @ 0xd0 
    /*000007ec*/ CMP         R4, #0 
    /*000007f0*/ CMPNE       R4, #3 
    /*000007f4*/ MOVEQ       R4, #1 
    /*000007f8*/ MOVNE       R4, #0 
    /*000007fc*/ BEQ         AIDisableChn_x95c
    /*00000800*/ ADD         R5, R7, #152                @ 0x98 
    /*00000804*/ LDR         R0, [PC, #532]              @ 0x0000000000000a20 
    /*00000808*/ ADD         R1, SP, #28 
    /*0000080c*/ BL          0x000000000000080C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000810*/ MOV         R0, R5 
    /*00000814*/ BL          0x0000000000000814          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*00000818*/ SUBS        FP, R0, #0 
    /*0000081c*/ BEQ         AIDisableChn_x850
    AIDisableChn_x820:
    /*00000820*/ MOV         R0, R5 
    /*00000824*/ ADD         R4, R4, #1 
    /*00000828*/ BL          0x0000000000000828          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x805c] 
    /*0000082c*/ MOV         R1, #14 
    /*00000830*/ MOV         R6, R0 
    /*00000834*/ ADD         R0, R0, #8 
    /*00000838*/ BL          0x0000000000000838          @ AISubVb [R_ARM_CALL: .text+0x3a50] 
    /*0000083c*/ MOV         R1, R6 
    /*00000840*/ MOV         R0, R5 
    /*00000844*/ BL          0x0000000000000844          @ AUDIO_FB_PutFree [R_ARM_CALL: .text+0x80bc] 
    /*00000848*/ CMP         FP, R4 
    /*0000084c*/ BNE         AIDisableChn_x820
    AIDisableChn_x850:
    /*00000850*/ ADD         R5, R7, #180                @ 0xb4 
    /*00000854*/ MOV         R0, R5 
    /*00000858*/ BL          0x0000000000000858          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*0000085c*/ SUBS        FP, R0, #0 
    /*00000860*/ MOVNE       R4, #0 
    /*00000864*/ BEQ         AIDisableChn_x898
    AIDisableChn_x868:
    /*00000868*/ MOV         R0, R5 
    /*0000086c*/ ADD         R4, R4, #1 
    /*00000870*/ BL          0x0000000000000870          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x805c] 
    /*00000874*/ MOV         R1, #14 
    /*00000878*/ MOV         R6, R0 
    /*0000087c*/ ADD         R0, R0, #8 
    /*00000880*/ BL          0x0000000000000880          @ AISubVb [R_ARM_CALL: .text+0x3a50] 
    /*00000884*/ MOV         R1, R6 
    /*00000888*/ MOV         R0, R5 
    /*0000088c*/ BL          0x000000000000088C          @ AUDIO_FB_PutFree [R_ARM_CALL: .text+0x80bc] 
    /*00000890*/ CMP         FP, R4 
    /*00000894*/ BNE         AIDisableChn_x868
    AIDisableChn_x898:
    /*00000898*/ MOV         R3, #3 
    /*0000089c*/ ADD         R1, SP, #28 
    /*000008a0*/ STR         R3, [R7, #208]              @ 0xd0 
    /*000008a4*/ MOV         R4, #0 
    /*000008a8*/ LDR         R0, [PC, #368]              @ 0x0000000000000a20 
    /*000008ac*/ ADD         R8, R8, #1 
    /*000008b0*/ BL          0x00000000000008B0          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000008b4*/ ADD         R0, R7, #4 
    /*000008b8*/ STR         R4, [R7, #580]              @ 0x244 
    /*000008bc*/ BL          0x00000000000008BC          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*000008c0*/ ADD         R0, R7, #8 
    /*000008c4*/ BL          0x00000000000008C4          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*000008c8*/ MOV         R0, R7 
    /*000008cc*/ BL          0x00000000000008CC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000008d0*/ LDR         R3, [SP, #8] 
    /*000008d4*/ CMP         R3, R8 
    /*000008d8*/ BEQ         AIDisableChn_x948
    /*000008dc*/ LDR         R3, [SL, #44]               @ 0x2c 
    /*000008e0*/ B           AIDisableChn_x7c0
    AIDisableChn_x8e4:
    /*000008e4*/ MOV         R0, R4 
    /*000008e8*/ BL          0x00000000000008E8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000008ec*/ MOV         R0, R8 
    AIDisableChn_x8f0:
    /*000008f0*/ ADD         SP, SP, #36                 @ 0x24 
    /*000008f4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIDisableChn_x8f8:
    /*000008f8*/ LDR         R3, [SP, #16] 
    /*000008fc*/ MOV         R2, #173                    @ 0xad 
    /*00000900*/ LDR         R1, [PC, #284]              @ 0x0000000000000a24 
    /*00000904*/ LDR         R0, [SP, #20] 
    /*00000908*/ BL          0x0000000000000908          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000090c*/ MOV         R0, R7 
    /*00000910*/ BL          0x0000000000000910          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000914*/ CMP         R0, #0 
    /*00000918*/ BEQ         AIDisableChn_x7e8
    AIDisableChn_x91c:
    /*0000091c*/ LDR         R0, [SP, #12] 
    /*00000920*/ BL          0x0000000000000920          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000924*/ MOV         R0, #65024                  @ 0xfe00 
    /*00000928*/ MOVT        R0, #65535                  @ 0xffff 
    /*0000092c*/ ADD         SP, SP, #36                 @ 0x24 
    /*00000930*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIDisableChn_x934:
    /*00000934*/ LDR         R0, [SP, #12] 
    /*00000938*/ BL          0x0000000000000938          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000093c*/ MOV         R0, R8 
    /*00000940*/ ADD         SP, SP, #36                 @ 0x24 
    /*00000944*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIDisableChn_x948:
    /*00000948*/ LDR         R0, [SP, #12] 
    /*0000094c*/ BL          0x000000000000094C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000950*/ MOV         R0, R4 
    /*00000954*/ ADD         SP, SP, #36                 @ 0x24 
    /*00000958*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIDisableChn_x95c:
    /*0000095c*/ MOV         R3, #3 
    /*00000960*/ MOV         R5, R0 
    /*00000964*/ STR         R3, [R7, #208]              @ 0xd0 
    /*00000968*/ MOV         R0, R7 
    /*0000096c*/ BL          0x000000000000096C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000970*/ LDR         R0, [SP, #12] 
    /*00000974*/ BL          0x0000000000000974          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000978*/ MOV         R0, R5 
    /*0000097c*/ ADD         SP, SP, #36                 @ 0x24 
    /*00000980*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIDisableChn_x984:
    /*00000984*/ STR         R9, [SP, #4] 
    /*00000988*/ MOV         R3, #144                    @ 0x90 
    /*0000098c*/ MOV         R1, #20 
    /*00000990*/ STR         R3, [SP] 
    /*00000994*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00000998*/ LDR         R3, [PC, #132]              @ 0x0000000000000a24 
    /*0000099c*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*000009a0*/ MOV         R0, #3 
    /*000009a4*/ BL          0x00000000000009A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000009a8*/ MOVW        R0, #32770                  @ 0x8002 
    /*000009ac*/ MOVT        R0, #40981                  @ 0xa015 
    /*000009b0*/ ADD         SP, SP, #36                 @ 0x24 
    /*000009b4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIDisableChn_x9b8:
    /*000009b8*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*000009bc*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000009c0*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*000009c4*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000009c8*/ MOV         R2, #146                    @ 0x92 
    /*000009cc*/ LDR         R1, [PC, #80]               @ 0x0000000000000a24 
    /*000009d0*/ BL          0x00000000000009D0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000009d4*/ B           AIDisableChn_x750
    AIDisableChn_x9d8:
    /*000009d8*/ MOV         R0, #65024                  @ 0xfe00 
    /*000009dc*/ MOVT        R0, #65535                  @ 0xffff 
    /*000009e0*/ B           AIDisableChn_x8f0
    AIDisableChn_x9e4:
    /*000009e4*/ MOV         R3, #143                    @ 0x8f 
    /*000009e8*/ STR         R4, [SP, #4] 
    /*000009ec*/ STR         R3, [SP] 
    /*000009f0*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*000009f4*/ LDR         R3, [PC, #40]               @ 0x0000000000000a24 
    /*000009f8*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*000009fc*/ MOV         R1, #21 
    /*00000a00*/ MOV         R0, #3 
    /*00000a04*/ BL          0x0000000000000A04          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000a08*/ MOVW        R0, #32769                  @ 0x8001 
    /*00000a0c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000a10*/ B           AIDisableChn_x8f0
    AIDisableChn_xa14:
    /*00000a14*/ MOVW        R0, #32784                  @ 0x8010 
    /*00000a18*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000a1c*/ B           AIDisableChn_x8f0
    /*00000a20*/ ANDEQ       R0, R0, R0, LSL #1          @ g_stAiDev [R_ARM_ABS32: .bss+0x8] 
    /*00000a24*/ ANDEQ       R0, R0, IP, LSL R0 
FUNC_END AIDisableChn


FUNC_BEGIN AIEnableVqe
    /*00000a28*/ CMP         R0, #0 
    /*00000a2c*/ ADD         R3, R1, R0, LSL #1 
    /*00000a30*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000a34*/ SUB         SP, SP, #24 
    /*00000a38*/ BNE         AIEnableVqe_xc70
    /*00000a3c*/ CMP         R1, #1 
    /*00000a40*/ MOV         R7, R1 
    /*00000a44*/ BHI         AIEnableVqe_xbb4
    /*00000a48*/ CMP         R2, #0 
    /*00000a4c*/ MOV         R4, R2 
    /*00000a50*/ BEQ         AIEnableVqe_xca0
    /*00000a54*/ MOV         R0, R3 
    /*00000a58*/ BL          0x0000000000000A58          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*00000a5c*/ SUBS        R5, R0, #0 
    /*00000a60*/ BEQ         AIEnableVqe_xb70
    AIEnableVqe_xa64:
    /*00000a64*/ LDR         R3, [R4] 
    /*00000a68*/ CMP         R3, #1 
    /*00000a6c*/ BHI         AIEnableVqe_xbe4
    /*00000a70*/ LDR         R3, [R4, #4] 
    /*00000a74*/ CMP         R3, #2 
    /*00000a78*/ BHI         AIEnableVqe_xc14
    /*00000a7c*/ MOV         R0, R5 
    /*00000a80*/ BL          0x0000000000000A80          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000a84*/ SUBS        R6, R0, #0 
    /*00000a88*/ BNE         AIEnableVqe_xc64
    /*00000a8c*/ LDR         R8, [R5, #208]              @ 0xd0 
    /*00000a90*/ CMP         R8, #1 
    /*00000a94*/ BNE         AIEnableVqe_xc44
    /*00000a98*/ LDR         R3, [R5, #216]              @ 0xd8 
    /*00000a9c*/ CMN         R3, #1 
    /*00000aa0*/ BEQ         AIEnableVqe_xb14
    /*00000aa4*/ LDR         R2, [R4] 
    /*00000aa8*/ MOV         R0, R5 
    /*00000aac*/ CMP         R3, R2 
    /*00000ab0*/ BNE         AIEnableVqe_xad4
    /*00000ab4*/ LDR         R3, [R4, #4] 
    /*00000ab8*/ LDR         R2, [R5, #220]              @ 0xdc 
    /*00000abc*/ CMP         R2, R3 
    /*00000ac0*/ BNE         AIEnableVqe_xad4
    AIEnableVqe_xac4:
    /*00000ac4*/ BL          0x0000000000000AC4          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000ac8*/ MOV         R0, #0 
    AIEnableVqe_xacc:
    /*00000acc*/ ADD         SP, SP, #24 
    /*00000ad0*/ POP         {R4, R5, R6, R7, R8, PC} 
    AIEnableVqe_xad4:
    /*00000ad4*/ BL          0x0000000000000AD4          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000ad8*/ MOV         R2, #0 
    /*00000adc*/ STR         R2, [SP, #4] 
    /*00000ae0*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x304] 
    /*00000ae4*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x304] 
    /*00000ae8*/ MOVW        R3, #265                    @ 0x109 
    /*00000aec*/ STR         R7, [SP, #8] 
    /*00000af0*/ STR         R3, [SP] 
    AIEnableVqe_xaf4:
    /*00000af4*/ LDR         R3, [PC, #432]              @ 0x0000000000000cac 
    /*00000af8*/ MOV         R1, #21 
    /*00000afc*/ MOV         R0, #3 
    /*00000b00*/ BL          0x0000000000000B00          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000b04*/ MOVW        R0, #32777                  @ 0x8009 
    /*00000b08*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000b0c*/ ADD         SP, SP, #24 
    /*00000b10*/ POP         {R4, R5, R6, R7, R8, PC} 
    AIEnableVqe_xb14:
    /*00000b14*/ LDR         R0, [PC, #404]              @ 0x0000000000000cb0 
    /*00000b18*/ ADD         R1, SP, #20 
    /*00000b1c*/ BL          0x0000000000000B1C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000b20*/ LDR         R6, [R4] 
    /*00000b24*/ CMN         R6, #1 
    /*00000b28*/ STR         R6, [R5, #216]              @ 0xd8 
    /*00000b2c*/ LDR         R3, [R4, #4] 
    /*00000b30*/ STR         R8, [R5, #236]              @ 0xec 
    /*00000b34*/ STR         R3, [R5, #220]              @ 0xdc 
    /*00000b38*/ BEQ         AIEnableVqe_xb5c
    /*00000b3c*/ MOV         R0, #22 
    /*00000b40*/ BL          0x0000000000000B40          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000b44*/ CMP         R0, #0 
    /*00000b48*/ BEQ         AIEnableVqe_xb90
    /*00000b4c*/ LDR         R3, [R0, #36]               @ 0x24 
    /*00000b50*/ MOV         R0, R6 
    /*00000b54*/ LDR         R1, [R5, #220]              @ 0xdc 
    /*00000b58*/ BLX         R3 
    AIEnableVqe_xb5c:
    /*00000b5c*/ LDR         R0, [PC, #332]              @ 0x0000000000000cb0 
    /*00000b60*/ ADD         R1, SP, #20 
    /*00000b64*/ BL          0x0000000000000B64          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000b68*/ MOV         R0, R5 
    /*00000b6c*/ B           AIEnableVqe_xac4
    AIEnableVqe_xb70:
    /*00000b70*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*00000b74*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000b78*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*00000b7c*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000b80*/ MOV         R2, #237                    @ 0xed 
    /*00000b84*/ LDR         R1, [PC, #288]              @ 0x0000000000000cac 
    /*00000b88*/ BL          0x0000000000000B88          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000b8c*/ B           AIEnableVqe_xa64
    AIEnableVqe_xb90:
    /*00000b90*/ MOVW        R3, #285                    @ 0x11d 
    /*00000b94*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x370] 
    /*00000b98*/ STR         R3, [SP] 
    /*00000b9c*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x370] 
    /*00000ba0*/ LDR         R3, [PC, #260]              @ 0x0000000000000cac 
    /*00000ba4*/ MOV         R1, #21 
    /*00000ba8*/ MOV         R0, #4 
    /*00000bac*/ BL          0x0000000000000BAC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000bb0*/ B           AIEnableVqe_xb5c
    AIEnableVqe_xbb4:
    /*00000bb4*/ STR         R1, [SP, #4] 
    /*00000bb8*/ MOV         R3, #233                    @ 0xe9 
    /*00000bbc*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00000bc0*/ STR         R3, [SP] 
    /*00000bc4*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*00000bc8*/ LDR         R3, [PC, #220]              @ 0x0000000000000cac 
    /*00000bcc*/ MOV         R1, #20 
    /*00000bd0*/ MOV         R0, #3 
    /*00000bd4*/ BL          0x0000000000000BD4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000bd8*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000bdc*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000be0*/ B           AIEnableVqe_xacc
    AIEnableVqe_xbe4:
    /*00000be4*/ STR         R3, [SP, #4] 
    /*00000be8*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x264] 
    /*00000bec*/ MOV         R3, #239                    @ 0xef 
    /*00000bf0*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x264] 
    /*00000bf4*/ STR         R3, [SP] 
    /*00000bf8*/ MOV         R1, #22 
    /*00000bfc*/ LDR         R3, [PC, #168]              @ 0x0000000000000cac 
    /*00000c00*/ MOV         R0, #3 
    /*00000c04*/ BL          0x0000000000000C04          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000c08*/ MOVW        R0, #32769                  @ 0x8001 
    /*00000c0c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000c10*/ B           AIEnableVqe_xacc
    AIEnableVqe_xc14:
    /*00000c14*/ STR         R3, [SP, #4] 
    /*00000c18*/ MOVW        R2, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x298] 
    /*00000c1c*/ MOV         R3, #240                    @ 0xf0 
    /*00000c20*/ MOVT        R2, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x298] 
    /*00000c24*/ STR         R3, [SP] 
    /*00000c28*/ MOV         R1, #20 
    /*00000c2c*/ LDR         R3, [PC, #120]              @ 0x0000000000000cac 
    /*00000c30*/ MOV         R0, #3 
    /*00000c34*/ BL          0x0000000000000C34          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000c38*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000c3c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000c40*/ B           AIEnableVqe_xacc
    AIEnableVqe_xc44:
    /*00000c44*/ MOV         R0, R5 
    /*00000c48*/ BL          0x0000000000000C48          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000c4c*/ MOV         R3, #249                    @ 0xf9 
    /*00000c50*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2cc] 
    /*00000c54*/ STMIB       SP, {R6, R7} 
    /*00000c58*/ STR         R3, [SP] 
    /*00000c5c*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2cc] 
    /*00000c60*/ B           AIEnableVqe_xaf4
    AIEnableVqe_xc64:
    /*00000c64*/ MOV         R0, #65024                  @ 0xfe00 
    /*00000c68*/ MOVT        R0, #65535                  @ 0xffff 
    /*00000c6c*/ B           AIEnableVqe_xacc
    AIEnableVqe_xc70:
    /*00000c70*/ STR         R0, [SP, #4] 
    /*00000c74*/ MOV         R3, #232                    @ 0xe8 
    /*00000c78*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00000c7c*/ STR         R3, [SP] 
    /*00000c80*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*00000c84*/ LDR         R3, [PC, #32]               @ 0x0000000000000cac 
    /*00000c88*/ MOV         R1, #21 
    /*00000c8c*/ MOV         R0, #3 
    /*00000c90*/ BL          0x0000000000000C90          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000c94*/ MOVW        R0, #32769                  @ 0x8001 
    /*00000c98*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000c9c*/ B           AIEnableVqe_xacc
    AIEnableVqe_xca0:
    /*00000ca0*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000ca4*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000ca8*/ B           AIEnableVqe_xacc
    /*00000cac*/ ANDEQ       R0, R0, IP, LSR #32 
    /*00000cb0*/ ANDEQ       R0, R0, R0, LSL #1          @ g_stAiDev [R_ARM_ABS32: .bss+0x8] 
FUNC_END AIEnableVqe


FUNC_BEGIN AIDisableVqe
    /*00000cb4*/ CMP         R0, #0 
    /*00000cb8*/ ADD         R3, R1, R0, LSL #1 
    /*00000cbc*/ PUSH        {R4, R5, LR} 
    /*00000cc0*/ SUB         SP, SP, #20 
    /*00000cc4*/ BNE         AIDisableVqe_xd68
    /*00000cc8*/ CMP         R1, #1 
    /*00000ccc*/ BHI         AIDisableVqe_xd18
    /*00000cd0*/ MOV         R0, R3 
    /*00000cd4*/ BL          0x0000000000000CD4          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*00000cd8*/ SUBS        R5, R0, #0 
    /*00000cdc*/ BEQ         AIDisableVqe_xd48
    AIDisableVqe_xce0:
    /*00000ce0*/ LDR         R0, [PC, #176]              @ 0x0000000000000d98 
    /*00000ce4*/ ADD         R1, SP, #12 
    /*00000ce8*/ BL          0x0000000000000CE8          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000cec*/ MOV         R4, #0 
    /*00000cf0*/ MVN         R3, #0 
    /*00000cf4*/ LDR         R0, [PC, #156]              @ 0x0000000000000d98 
    /*00000cf8*/ ADD         R1, SP, #12 
    /*00000cfc*/ STR         R4, [R5, #236]              @ 0xec 
    /*00000d00*/ STR         R3, [R5, #216]              @ 0xd8 
    /*00000d04*/ STR         R3, [R5, #220]              @ 0xdc 
    /*00000d08*/ BL          0x0000000000000D08          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000d0c*/ MOV         R0, R4 
    AIDisableVqe_xd10:
    /*00000d10*/ ADD         SP, SP, #20 
    /*00000d14*/ POP         {R4, R5, PC} 
    AIDisableVqe_xd18:
    /*00000d18*/ STR         R1, [SP, #4] 
    /*00000d1c*/ MOVW        R3, #306                    @ 0x132 
    /*00000d20*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00000d24*/ STR         R3, [SP] 
    /*00000d28*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*00000d2c*/ LDR         R3, [PC, #104]              @ 0x0000000000000d9c 
    /*00000d30*/ MOV         R1, #20 
    /*00000d34*/ MOV         R0, #3 
    /*00000d38*/ BL          0x0000000000000D38          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000d3c*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000d40*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000d44*/ B           AIDisableVqe_xd10
    AIDisableVqe_xd48:
    /*00000d48*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*00000d4c*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000d50*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*00000d54*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000d58*/ MOVW        R2, #309                    @ 0x135 
    /*00000d5c*/ LDR         R1, [PC, #56]               @ 0x0000000000000d9c 
    /*00000d60*/ BL          0x0000000000000D60          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000d64*/ B           AIDisableVqe_xce0
    AIDisableVqe_xd68:
    /*00000d68*/ STR         R0, [SP, #4] 
    /*00000d6c*/ MOVW        R3, #305                    @ 0x131 
    /*00000d70*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00000d74*/ STR         R3, [SP] 
    /*00000d78*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*00000d7c*/ LDR         R3, [PC, #24]               @ 0x0000000000000d9c 
    /*00000d80*/ MOV         R1, #21 
    /*00000d84*/ MOV         R0, #3 
    /*00000d88*/ BL          0x0000000000000D88          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000d8c*/ MOVW        R0, #32769                  @ 0x8001 
    /*00000d90*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000d94*/ B           AIDisableVqe_xd10
    /*00000d98*/ ANDEQ       R0, R0, R0, LSL #1          @ g_stAiDev [R_ARM_ABS32: .bss+0x8] 
    /*00000d9c*/ ANDEQ       R0, R0, R8, LSR R0 
FUNC_END AIDisableVqe


FUNC_BEGIN AIGetPoolId
    /*00000da0*/ SUBS        R3, R0, #0 
    /*00000da4*/ PUSH        {R4, R5, LR} 
    /*00000da8*/ SUB         SP, SP, #12 
    /*00000dac*/ BNE         AIGetPoolId_xe08
    /*00000db0*/ CMP         R1, #0 
    /*00000db4*/ MOV         R4, R1 
    /*00000db8*/ BEQ         AIGetPoolId_xdfc
    /*00000dbc*/ BL          0x0000000000000DBC          @ AI_GetDevCtx [R_ARM_CALL: .text+0x56c4] 
    /*00000dc0*/ SUBS        R5, R0, #0 
    /*00000dc4*/ BEQ         AIGetPoolId_xddc
    AIGetPoolId_xdc8:
    /*00000dc8*/ LDR         R3, [R5] 
    /*00000dcc*/ MOV         R0, #0 
    /*00000dd0*/ STR         R3, [R4] 
    AIGetPoolId_xdd4:
    /*00000dd4*/ ADD         SP, SP, #12 
    /*00000dd8*/ POP         {R4, R5, PC} 
    AIGetPoolId_xddc:
    /*00000ddc*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*00000de0*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000de4*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*00000de8*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000dec*/ MOV         R2, #328                    @ 0x148 
    /*00000df0*/ LDR         R1, [PC, #64]               @ 0x0000000000000e38 
    /*00000df4*/ BL          0x0000000000000DF4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000df8*/ B           AIGetPoolId_xdc8
    AIGetPoolId_xdfc:
    /*00000dfc*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000e00*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000e04*/ B           AIGetPoolId_xdd4
    AIGetPoolId_xe08:
    /*00000e08*/ STR         R3, [SP, #4] 
    /*00000e0c*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00000e10*/ MOV         R3, #324                    @ 0x144 
    /*00000e14*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*00000e18*/ STR         R3, [SP] 
    /*00000e1c*/ MOV         R1, #21 
    /*00000e20*/ LDR         R3, [PC, #16]               @ 0x0000000000000e38 
    /*00000e24*/ MOV         R0, #3 
    /*00000e28*/ BL          0x0000000000000E28          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000e2c*/ MOVW        R0, #32769                  @ 0x8001 
    /*00000e30*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000e34*/ B           AIGetPoolId_xdd4
    /*00000e38*/ ANDEQ       R0, R0, R8, ASR #32 
FUNC_END AIGetPoolId


FUNC_BEGIN AIGetChnParam
    /*00000e3c*/ SUBS        R3, R0, #0 
    /*00000e40*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00000e44*/ SUB         SP, SP, #20 
    /*00000e48*/ BNE         AIGetChnParam_xfcc
    /*00000e4c*/ CMP         R1, #1 
    /*00000e50*/ MOV         R6, R1 
    /*00000e54*/ BHI         AIGetChnParam_xf1c
    /*00000e58*/ CMP         R2, #0 
    /*00000e5c*/ MOV         R7, R2 
    /*00000e60*/ BEQ         AIGetChnParam_xffc
    /*00000e64*/ BL          0x0000000000000E64          @ AI_GetDevCtx [R_ARM_CALL: .text+0x56c4] 
    /*00000e68*/ SUBS        R4, R0, #0 
    /*00000e6c*/ BEQ         AIGetChnParam_xedc
    AIGetChnParam_xe70:
    /*00000e70*/ MOV         R0, R6 
    /*00000e74*/ BL          0x0000000000000E74          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*00000e78*/ SUBS        R9, R0, #0 
    /*00000e7c*/ BEQ         AIGetChnParam_xefc
    AIGetChnParam_xe80:
    /*00000e80*/ ADD         R8, R4, #124                @ 0x7c 
    /*00000e84*/ MOV         R0, R8 
    /*00000e88*/ BL          0x0000000000000E88          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000e8c*/ SUBS        R5, R0, #0 
    /*00000e90*/ BNE         AIGetChnParam_xf84
    /*00000e94*/ LDR         R3, [R4, #12] 
    /*00000e98*/ CMP         R3, #1 
    /*00000e9c*/ BNE         AIGetChnParam_xf4c
    /*00000ea0*/ LDR         R3, [R4, #28] 
    /*00000ea4*/ LDR         R0, [R4, #44]               @ 0x2c 
    /*00000ea8*/ CMP         R3, #1 
    /*00000eac*/ MOVEQ       R1, #2 
    /*00000eb0*/ MOVNE       R1, #1 
    /*00000eb4*/ BL          0x0000000000000EB4          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00000eb8*/ CMP         R6, R0 
    /*00000ebc*/ BCS         AIGetChnParam_xf90
    /*00000ec0*/ LDR         R3, [R9, #148]              @ 0x94 
    /*00000ec4*/ MOV         R0, R8 
    /*00000ec8*/ STR         R3, [R7] 
    /*00000ecc*/ BL          0x0000000000000ECC          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000ed0*/ MOV         R0, R5 
    AIGetChnParam_xed4:
    /*00000ed4*/ ADD         SP, SP, #20 
    /*00000ed8*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AIGetChnParam_xedc:
    /*00000edc*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*00000ee0*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000ee4*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*00000ee8*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000eec*/ MOVW        R2, #347                    @ 0x15b 
    /*00000ef0*/ LDR         R1, [PC, #272]              @ 0x0000000000001008 
    /*00000ef4*/ BL          0x0000000000000EF4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000ef8*/ B           AIGetChnParam_xe70
    AIGetChnParam_xefc:
    /*00000efc*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*00000f00*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000f04*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*00000f08*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000f0c*/ MOVW        R2, #351                    @ 0x15f 
    /*00000f10*/ LDR         R1, [PC, #240]              @ 0x0000000000001008 
    /*00000f14*/ BL          0x0000000000000F14          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000f18*/ B           AIGetChnParam_xe80
    AIGetChnParam_xf1c:
    /*00000f1c*/ STR         R1, [SP, #4] 
    /*00000f20*/ MOVW        R3, #343                    @ 0x157 
    /*00000f24*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00000f28*/ STR         R3, [SP] 
    /*00000f2c*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*00000f30*/ LDR         R3, [PC, #208]              @ 0x0000000000001008 
    /*00000f34*/ MOV         R1, #20 
    /*00000f38*/ MOV         R0, #3 
    /*00000f3c*/ BL          0x0000000000000F3C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000f40*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000f44*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000f48*/ B           AIGetChnParam_xed4
    AIGetChnParam_xf4c:
    /*00000f4c*/ MOV         R0, R8 
    /*00000f50*/ BL          0x0000000000000F50          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000f54*/ MOV         R3, #360                    @ 0x168 
    /*00000f58*/ STR         R5, [SP, #4] 
    /*00000f5c*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x180] 
    /*00000f60*/ STR         R3, [SP] 
    /*00000f64*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x180] 
    /*00000f68*/ LDR         R3, [PC, #152]              @ 0x0000000000001008 
    /*00000f6c*/ MOV         R1, #21 
    /*00000f70*/ MOV         R0, #3 
    /*00000f74*/ BL          0x0000000000000F74          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000f78*/ MOVW        R0, #32773                  @ 0x8005 
    /*00000f7c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000f80*/ B           AIGetChnParam_xed4
    AIGetChnParam_xf84:
    /*00000f84*/ MOV         R0, #65024                  @ 0xfe00 
    /*00000f88*/ MOVT        R0, #65535                  @ 0xffff 
    /*00000f8c*/ B           AIGetChnParam_xed4
    AIGetChnParam_xf90:
    /*00000f90*/ MOV         R0, R8 
    /*00000f94*/ BL          0x0000000000000F94          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000f98*/ MOVW        R3, #369                    @ 0x171 
    /*00000f9c*/ STR         R5, [SP, #8] 
    /*00000fa0*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*00000fa4*/ STR         R3, [SP] 
    /*00000fa8*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*00000fac*/ STR         R6, [SP, #4] 
    /*00000fb0*/ MOV         R1, #21 
    /*00000fb4*/ LDR         R3, [PC, #76]               @ 0x0000000000001008 
    /*00000fb8*/ MOV         R0, #3 
    /*00000fbc*/ BL          0x0000000000000FBC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000fc0*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000fc4*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000fc8*/ B           AIGetChnParam_xed4
    AIGetChnParam_xfcc:
    /*00000fcc*/ STR         R3, [SP, #4] 
    /*00000fd0*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00000fd4*/ MOVW        R3, #342                    @ 0x156 
    /*00000fd8*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*00000fdc*/ STR         R3, [SP] 
    /*00000fe0*/ MOV         R1, #21 
    /*00000fe4*/ LDR         R3, [PC, #28]               @ 0x0000000000001008 
    /*00000fe8*/ MOV         R0, #3 
    /*00000fec*/ BL          0x0000000000000FEC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000ff0*/ MOVW        R0, #32769                  @ 0x8001 
    /*00000ff4*/ MOVT        R0, #40981                  @ 0xa015 
    /*00000ff8*/ B           AIGetChnParam_xed4
    AIGetChnParam_xffc:
    /*00000ffc*/ MOVW        R0, #32774                  @ 0x8006 
    /*00001000*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001004*/ B           AIGetChnParam_xed4
    /*00001008*/ ANDEQ       R0, R0, R4, ASR R0 
FUNC_END AIGetChnParam


FUNC_BEGIN AISetChnParam
    /*0000100c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00001010*/ MOV         R4, R0 
    /*00001014*/ SUB         SP, SP, #28 
    /*00001018*/ MOV         R0, #20 
    /*0000101c*/ MOV         R6, R1 
    /*00001020*/ MOV         R8, R2 
    /*00001024*/ BL          0x0000000000001024          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00001028*/ CMP         R0, #0 
    /*0000102c*/ BEQ         AISetChnParam_x12c4
    /*00001030*/ CMP         R4, #0 
    /*00001034*/ BNE         AISetChnParam_x1248
    /*00001038*/ CMP         R6, #1 
    /*0000103c*/ BHI         AISetChnParam_x11d4
    /*00001040*/ CMP         R8, #0 
    /*00001044*/ BEQ         AISetChnParam_x1278
    /*00001048*/ MOV         R0, R4 
    /*0000104c*/ BL          0x000000000000104C          @ AI_GetDevCtx [R_ARM_CALL: .text+0x56c4] 
    /*00001050*/ SUBS        R7, R0, #0 
    /*00001054*/ BEQ         AISetChnParam_x11b4
    AISetChnParam_x1058:
    /*00001058*/ ADD         R3, R7, #124                @ 0x7c 
    /*0000105c*/ STR         R3, [SP, #16] 
    /*00001060*/ MOV         R0, R3 
    /*00001064*/ BL          0x0000000000001064          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001068*/ SUBS        R4, R0, #0 
    /*0000106c*/ BNE         AISetChnParam_x123c
    /*00001070*/ LDR         R1, [R7, #12] 
    /*00001074*/ CMP         R1, #1 
    /*00001078*/ BNE         AISetChnParam_x1204
    /*0000107c*/ LDR         R3, [R7, #28] 
    /*00001080*/ LDR         R5, [R7, #44]               @ 0x2c 
    /*00001084*/ CMP         R3, #1 
    /*00001088*/ MOV         R0, R5 
    /*0000108c*/ MOVEQ       R1, #2 
    /*00001090*/ MOV         R9, R1 
    /*00001094*/ BL          0x0000000000001094          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00001098*/ CMP         R6, R0 
    /*0000109c*/ BCS         AISetChnParam_x1284
    /*000010a0*/ MOVW        SL, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*000010a4*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000010a8*/ MOVT        SL, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*000010ac*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000010b0*/ STR         R3, [SP, #20] 
    /*000010b4*/ B           AISetChnParam_x110c
    AISetChnParam_x10b8:
    /*000010b8*/ MOV         R0, R5 
    /*000010bc*/ BL          0x00000000000010BC          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000010c0*/ CMP         R0, #0 
    /*000010c4*/ MOV         R0, R5 
    /*000010c8*/ BNE         AISetChnParam_x119c
    /*000010cc*/ LDR         R3, [R8] 
    /*000010d0*/ LDR         R1, [R7, #36]               @ 0x24 
    /*000010d4*/ CMP         R3, #30 
    /*000010d8*/ BHI         AISetChnParam_x1140
    /*000010dc*/ CMP         R3, #4 
    /*000010e0*/ MOVHI       FP, #0 
    /*000010e4*/ MOVLS       FP, #1 
    /*000010e8*/ CMP         R3, R1 
    /*000010ec*/ ORRHI       FP, FP, #1 
    /*000010f0*/ CMP         FP, #0 
    /*000010f4*/ BNE         AISetChnParam_x1140
    /*000010f8*/ STR         R3, [R5, #148]              @ 0x94 
    /*000010fc*/ BL          0x00000000000010FC          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001100*/ CMP         R9, R4 
    /*00001104*/ BEQ         AISetChnParam_x1188
    /*00001108*/ LDR         R5, [R7, #44]               @ 0x2c 
    AISetChnParam_x110c:
    /*0000110c*/ LSR         R5, R5, #1 
    /*00001110*/ MLA         R6, R4, R5, R6 
    /*00001114*/ ADD         R4, R4, #1 
    /*00001118*/ MOV         R0, R6 
    /*0000111c*/ BL          0x000000000000111C          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*00001120*/ SUBS        R5, R0, #0 
    /*00001124*/ BNE         AISetChnParam_x10b8
    /*00001128*/ MOV         R3, SL 
    /*0000112c*/ MOV         R2, #420                    @ 0x1a4 
    /*00001130*/ LDR         R1, [PC, #408]              @ 0x00000000000012d0 
    /*00001134*/ LDR         R0, [SP, #20] 
    /*00001138*/ BL          0x0000000000001138          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000113c*/ B           AISetChnParam_x10b8
    AISetChnParam_x1140:
    /*00001140*/ STR         R1, [SP, #8] 
    /*00001144*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3a8] 
    /*00001148*/ MOVW        R1, #431                    @ 0x1af 
    /*0000114c*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3a8] 
    /*00001150*/ STR         R3, [SP, #4] 
    /*00001154*/ MOV         R0, #3 
    /*00001158*/ LDR         R3, [PC, #368]              @ 0x00000000000012d0 
    /*0000115c*/ STR         R1, [SP] 
    /*00001160*/ MOV         R1, #21 
    /*00001164*/ BL          0x0000000000001164          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001168*/ MOV         R0, R5 
    /*0000116c*/ BL          0x000000000000116C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001170*/ LDR         R0, [SP, #16] 
    /*00001174*/ BL          0x0000000000001174          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001178*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000117c*/ MOVT        R0, #40981                  @ 0xa015 
    AISetChnParam_x1180:
    /*00001180*/ ADD         SP, SP, #28 
    /*00001184*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AISetChnParam_x1188:
    /*00001188*/ LDR         R0, [SP, #16] 
    /*0000118c*/ BL          0x000000000000118C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001190*/ MOV         R0, FP 
    /*00001194*/ ADD         SP, SP, #28 
    /*00001198*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AISetChnParam_x119c:
    /*0000119c*/ LDR         R0, [SP, #16] 
    /*000011a0*/ BL          0x00000000000011A0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000011a4*/ MOV         R0, #65024                  @ 0xfe00 
    /*000011a8*/ MOVT        R0, #65535                  @ 0xffff 
    /*000011ac*/ ADD         SP, SP, #28 
    /*000011b0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AISetChnParam_x11b4:
    /*000011b4*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*000011b8*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000011bc*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*000011c0*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000011c4*/ MOVW        R2, #391                    @ 0x187 
    /*000011c8*/ LDR         R1, [PC, #256]              @ 0x00000000000012d0 
    /*000011cc*/ BL          0x00000000000011CC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000011d0*/ B           AISetChnParam_x1058
    AISetChnParam_x11d4:
    /*000011d4*/ MOV         R3, #388                    @ 0x184 
    /*000011d8*/ STR         R6, [SP, #4] 
    /*000011dc*/ STR         R3, [SP] 
    /*000011e0*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*000011e4*/ LDR         R3, [PC, #228]              @ 0x00000000000012d0 
    /*000011e8*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*000011ec*/ MOV         R1, #20 
    /*000011f0*/ MOV         R0, #3 
    /*000011f4*/ BL          0x00000000000011F4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000011f8*/ MOVW        R0, #32770                  @ 0x8002 
    /*000011fc*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001200*/ B           AISetChnParam_x1180
    AISetChnParam_x1204:
    /*00001204*/ LDR         R0, [SP, #16] 
    /*00001208*/ BL          0x0000000000001208          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000120c*/ MOVW        R3, #401                    @ 0x191 
    /*00001210*/ STR         R4, [SP, #4] 
    /*00001214*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x180] 
    /*00001218*/ STR         R3, [SP] 
    /*0000121c*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x180] 
    /*00001220*/ LDR         R3, [PC, #168]              @ 0x00000000000012d0 
    /*00001224*/ MOV         R1, #21 
    /*00001228*/ MOV         R0, #3 
    /*0000122c*/ BL          0x000000000000122C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001230*/ MOVW        R0, #32773                  @ 0x8005 
    /*00001234*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001238*/ B           AISetChnParam_x1180
    AISetChnParam_x123c:
    /*0000123c*/ MOV         R0, #65024                  @ 0xfe00 
    /*00001240*/ MOVT        R0, #65535                  @ 0xffff 
    /*00001244*/ B           AISetChnParam_x1180
    AISetChnParam_x1248:
    /*00001248*/ MOVW        R3, #387                    @ 0x183 
    /*0000124c*/ STR         R4, [SP, #4] 
    /*00001250*/ STR         R3, [SP] 
    /*00001254*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00001258*/ LDR         R3, [PC, #112]              @ 0x00000000000012d0 
    /*0000125c*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*00001260*/ MOV         R1, #21 
    /*00001264*/ MOV         R0, #3 
    /*00001268*/ BL          0x0000000000001268          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000126c*/ MOVW        R0, #32769                  @ 0x8001 
    /*00001270*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001274*/ B           AISetChnParam_x1180
    AISetChnParam_x1278:
    /*00001278*/ MOVW        R0, #32774                  @ 0x8006 
    /*0000127c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001280*/ B           AISetChnParam_x1180
    AISetChnParam_x1284:
    /*00001284*/ LDR         R0, [SP, #16] 
    /*00001288*/ BL          0x0000000000001288          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000128c*/ MOV         R2, #0 
    /*00001290*/ MOVW        R3, #411                    @ 0x19b 
    /*00001294*/ STR         R2, [SP, #8] 
    /*00001298*/ STR         R3, [SP] 
    /*0000129c*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*000012a0*/ STR         R6, [SP, #4] 
    /*000012a4*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*000012a8*/ LDR         R3, [PC, #32]               @ 0x00000000000012d0 
    /*000012ac*/ MOV         R1, #21 
    /*000012b0*/ MOV         R0, #3 
    /*000012b4*/ BL          0x00000000000012B4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000012b8*/ MOVW        R0, #32770                  @ 0x8002 
    /*000012bc*/ MOVT        R0, #40981                  @ 0xa015 
    /*000012c0*/ B           AISetChnParam_x1180
    AISetChnParam_x12c4:
    /*000012c4*/ MOVW        R0, #32784                  @ 0x8010 
    /*000012c8*/ MOVT        R0, #40981                  @ 0xa015 
    /*000012cc*/ B           AISetChnParam_x1180
    /*000012d0*/ ANDEQ       R0, R0, R4, RRX 
FUNC_END AISetChnParam


FUNC_BEGIN AIGetFrame
    /*000012d4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000012d8*/ SUB         SP, SP, #36                 @ 0x24 
    /*000012dc*/ MOV         R4, R0 
    /*000012e0*/ MOV         R0, #20 
    /*000012e4*/ STR         R1, [SP, #16] 
    /*000012e8*/ MOV         R9, R2 
    /*000012ec*/ STR         R3, [SP, #20] 
    /*000012f0*/ BL          0x00000000000012F0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000012f4*/ CMP         R0, #0 
    /*000012f8*/ BEQ         AIGetFrame_x181c
    /*000012fc*/ CMP         R4, #0 
    /*00001300*/ BNE         AIGetFrame_x17e0
    /*00001304*/ LDR         R3, [SP, #16] 
    /*00001308*/ CMP         R3, #1 
    /*0000130c*/ BHI         AIGetFrame_x16a8
    /*00001310*/ CMP         R9, #0 
    /*00001314*/ BEQ         AIGetFrame_x1810
    /*00001318*/ MOVW        R4, #0                      @ g_stAiDev [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*0000131c*/ MOVT        R4, #0                      @ g_stAiDev [R_ARM_MOVT_ABS: .bss+0x8] 
    /*00001320*/ CMP         R4, #0 
    /*00001324*/ BEQ         AIGetFrame_x1688
    AIGetFrame_x1328:
    /*00001328*/ LDR         R0, [SP, #16] 
    /*0000132c*/ BL          0x000000000000132C          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*00001330*/ SUBS        R5, R0, #0 
    /*00001334*/ BEQ         AIGetFrame_x1668
    AIGetFrame_x1338:
    /*00001338*/ LDR         R3, [R9, #128]              @ 0x80 
    /*0000133c*/ CMN         R3, #1 
    /*00001340*/ BLT         AIGetFrame_x1774
    /*00001344*/ LDR         R0, [PC, #1308]             @ 0x0000000000001868 
    /*00001348*/ BL          0x0000000000001348          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*0000134c*/ SUBS        R6, R0, #0 
    /*00001350*/ BNE         AIGetFrame_x1534
    /*00001354*/ LDR         R3, [R4, #12] 
    /*00001358*/ CMP         R3, #1 
    /*0000135c*/ BNE         AIGetFrame_x1714
    /*00001360*/ LDR         R3, [R4, #28] 
    /*00001364*/ LDR         R0, [R4, #44]               @ 0x2c 
    /*00001368*/ CMP         R3, #1 
    /*0000136c*/ MOVNE       R1, #1 
    /*00001370*/ MOVEQ       R1, #2 
    /*00001374*/ BL          0x0000000000001374          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00001378*/ LDR         R3, [SP, #16] 
    /*0000137c*/ CMP         R3, R0 
    /*00001380*/ BCS         AIGetFrame_x1734
    /*00001384*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*00001388*/ SUB         R3, R3, #1 
    /*0000138c*/ CMP         R3, #1 
    /*00001390*/ BHI         AIGetFrame_x16d8
    /*00001394*/ LDR         R3, [R5, #148]              @ 0x94 
    /*00001398*/ CMP         R3, #0 
    /*0000139c*/ BEQ         AIGetFrame_x17a4
    /*000013a0*/ LDR         R3, [SP, #20] 
    /*000013a4*/ MOVW        SL, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000013a8*/ LDR         R8, [PC, #1208]             @ 0x0000000000001868 
    /*000013ac*/ MOVT        SL, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000013b0*/ CMP         R3, #0 
    /*000013b4*/ ADDEQ       FP, R5, #152                @ 0x98 
    /*000013b8*/ ADDEQ       R7, R5, #4 
    /*000013bc*/ MOVWEQ      R6, #0                      @ AiGetFrame_UserWaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x410] 
    /*000013c0*/ MOVWNE      R6, #0                      @ AiGetFrame_AieWaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x35c] 
    /*000013c4*/ MOVTEQ      R6, #0                      @ AiGetFrame_UserWaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x410] 
    /*000013c8*/ ADDNE       FP, R5, #180                @ 0xb4 
    /*000013cc*/ MOVTNE      R6, #0                      @ AiGetFrame_AieWaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x35c] 
    /*000013d0*/ ADDNE       R7, R5, #8 
    AIGetFrame_x13d4:
    /*000013d4*/ MOV         R0, FP 
    /*000013d8*/ BL          0x00000000000013D8          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*000013dc*/ CMP         R0, #0 
    /*000013e0*/ BNE         AIGetFrame_x1544
    /*000013e4*/ MOV         R0, R8 
    /*000013e8*/ BL          0x00000000000013E8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000013ec*/ LDR         R4, [R9, #128]              @ 0x80 
    /*000013f0*/ CMN         R4, #1 
    /*000013f4*/ BEQ         AIGetFrame_x14dc
    /*000013f8*/ CMP         R4, #0 
    /*000013fc*/ BEQ         AIGetFrame_x15e0
    /*00001400*/ MOV         R0, R5 
    /*00001404*/ BLX         R6 
    /*00001408*/ CMP         R0, #0 
    /*0000140c*/ BEQ         AIGetFrame_x1434
    /*00001410*/ LDR         R3, [R9, #128]              @ 0x80 
    /*00001414*/ CMP         R3, #0 
    /*00001418*/ MOVEQ       R4, #1 
    /*0000141c*/ B           AIGetFrame_x1434
    AIGetFrame_x1420:
    /*00001420*/ BL          0x0000000000001420          @ osal_wait_timeout_interruptible [R_ARM_CALL: +0x0] 
    /*00001424*/ ADD         R3, R0, #512                @ 0x200 
    /*00001428*/ MOV         R4, R0 
    /*0000142c*/ BICS        R3, R3, #512                @ 0x200 
    /*00001430*/ BEQ         AIGetFrame_x1460
    AIGetFrame_x1434:
    /*00001434*/ MOV         R0, R5 
    /*00001438*/ BLX         R6 
    /*0000143c*/ MOV         R3, R4 
    /*00001440*/ MOV         R2, R5 
    /*00001444*/ MOV         R1, R6 
    /*00001448*/ CMP         R0, #0 
    /*0000144c*/ MOV         R0, R7 
    /*00001450*/ BEQ         AIGetFrame_x1420
    /*00001454*/ MOV         R0, R4 
    /*00001458*/ BL          0x0000000000001458          @ osal_msecs_to_jiffies [R_ARM_CALL: +0x0] 
    /*0000145c*/ MOV         R4, R0 
    AIGetFrame_x1460:
    /*00001460*/ CMP         R4, #0 
    /*00001464*/ BLT         AIGetFrame_x15ec
    /*00001468*/ BEQ         AIGetFrame_x1630
    AIGetFrame_x146c:
    /*0000146c*/ MOV         R0, R8 
    /*00001470*/ BL          0x0000000000001470          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001474*/ CMP         R0, #0 
    /*00001478*/ BNE         AIGetFrame_x1534
    /*0000147c*/ LDR         R3, [SL] 
    /*00001480*/ CMP         R3, #0 
    /*00001484*/ BNE         AIGetFrame_x15c8
    /*00001488*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*0000148c*/ SUB         R3, R3, #1 
    /*00001490*/ CMP         R3, #1 
    /*00001494*/ BLS         AIGetFrame_x13d4
    /*00001498*/ MOV         R1, #21 
    /*0000149c*/ MOV         R3, #836                    @ 0x344 
    /*000014a0*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x54c] 
    /*000014a4*/ STR         R3, [SP] 
    /*000014a8*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x54c] 
    /*000014ac*/ LDR         R3, [PC, #952]              @ 0x000000000000186c 
    /*000014b0*/ MOV         R0, #4 
    /*000014b4*/ BL          0x00000000000014B4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000014b8*/ LDR         R0, [PC, #936]              @ 0x0000000000001868 
    /*000014bc*/ BL          0x00000000000014BC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000014c0*/ MOVW        R0, #32773                  @ 0x8005 
    /*000014c4*/ MOVT        R0, #40981                  @ 0xa015 
    /*000014c8*/ ADD         SP, SP, #36                 @ 0x24 
    /*000014cc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIGetFrame_x14d0:
    /*000014d0*/ BL          0x00000000000014D0          @ osal_wait_timeout_interruptible [R_ARM_CALL: +0x0] 
    /*000014d4*/ CMP         R0, #0 
    /*000014d8*/ BLT         AIGetFrame_x1500
    AIGetFrame_x14dc:
    /*000014dc*/ MOV         R0, R5 
    /*000014e0*/ BLX         R6 
    /*000014e4*/ MOV         R3, #100                    @ 0x64 
    /*000014e8*/ MOV         R2, R5 
    /*000014ec*/ MOV         R1, R6 
    /*000014f0*/ SUBS        R4, R0, #0 
    /*000014f4*/ MOV         R0, R7 
    /*000014f8*/ BEQ         AIGetFrame_x14d0
    /*000014fc*/ B           AIGetFrame_x146c
    AIGetFrame_x1500:
    /*00001500*/ LDR         R1, [R5, #208]              @ 0xd0 
    /*00001504*/ MOVW        R3, #794                    @ 0x31a 
    /*00001508*/ LDR         R2, [SP, #16] 
    /*0000150c*/ MOV         R0, #4 
    /*00001510*/ STR         R3, [SP] 
    /*00001514*/ STR         R1, [SP, #12] 
    /*00001518*/ MOV         R1, #21 
    /*0000151c*/ STR         R2, [SP, #8] 
    /*00001520*/ MOVW        R2, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4d4] 
    /*00001524*/ STR         R4, [SP, #4] 
    /*00001528*/ MOVT        R2, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4d4] 
    /*0000152c*/ LDR         R3, [PC, #824]              @ 0x000000000000186c 
    /*00001530*/ BL          0x0000000000001530          @ HI_LOG [R_ARM_CALL: +0x0] 
    AIGetFrame_x1534:
    /*00001534*/ MOV         R0, #65024                  @ 0xfe00 
    /*00001538*/ MOVT        R0, #65535                  @ 0xffff 
    AIGetFrame_x153c:
    /*0000153c*/ ADD         SP, SP, #36                 @ 0x24 
    /*00001540*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIGetFrame_x1544:
    /*00001544*/ LDR         R0, [PC, #804]              @ 0x0000000000001870 
    /*00001548*/ ADD         R1, SP, #28 
    /*0000154c*/ BL          0x000000000000154C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001550*/ MOV         R0, FP 
    /*00001554*/ BL          0x0000000000001554          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x805c] 
    /*00001558*/ SUBS        R4, R0, #0 
    /*0000155c*/ BEQ         AIGetFrame_x1828
    /*00001560*/ MOV         R2, #128                    @ 0x80 
    /*00001564*/ ADD         R1, R4, #8 
    /*00001568*/ MOV         R0, R9 
    /*0000156c*/ BL          0x000000000000156C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00001570*/ MOV         R1, #12 
    /*00001574*/ MOV         R0, R9 
    /*00001578*/ BL          0x0000000000001578          @ AIAddVb [R_ARM_CALL: .text+0x3958] 
    /*0000157c*/ MOV         R0, R9 
    /*00001580*/ MOV         R1, #14 
    /*00001584*/ BL          0x0000000000001584          @ AISubVb [R_ARM_CALL: .text+0x3a50] 
    /*00001588*/ MOV         R1, R4 
    /*0000158c*/ MOV         R0, FP 
    /*00001590*/ BL          0x0000000000001590          @ AUDIO_FB_PutFree [R_ARM_CALL: .text+0x80bc] 
    /*00001594*/ ADD         R1, SP, #28 
    /*00001598*/ LDR         R0, [PC, #720]              @ 0x0000000000001870 
    /*0000159c*/ BL          0x000000000000159C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000015a0*/ LDR         R3, [SP, #20] 
    /*000015a4*/ LDR         R0, [PC, #700]              @ 0x0000000000001868 
    /*000015a8*/ CMP         R3, #0 
    /*000015ac*/ LDREQ       R3, [R5, #1104]             @ 0x450 
    /*000015b0*/ ADDEQ       R3, R3, #1 
    /*000015b4*/ STREQ       R3, [R5, #1104]             @ 0x450 
    /*000015b8*/ BL          0x00000000000015B8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000015bc*/ MOV         R0, #0 
    /*000015c0*/ ADD         SP, SP, #36                 @ 0x24 
    /*000015c4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIGetFrame_x15c8:
    /*000015c8*/ LDR         R0, [PC, #664]              @ 0x0000000000001868 
    /*000015cc*/ BL          0x00000000000015CC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000015d0*/ MOVW        R0, #32784                  @ 0x8010 
    /*000015d4*/ MOVT        R0, #40981                  @ 0xa015 
    /*000015d8*/ ADD         SP, SP, #36                 @ 0x24 
    /*000015dc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIGetFrame_x15e0:
    /*000015e0*/ MOVW        R0, #32782                  @ 0x800e 
    /*000015e4*/ MOVT        R0, #40981                  @ 0xa015 
    /*000015e8*/ B           AIGetFrame_x153c
    AIGetFrame_x15ec:
    /*000015ec*/ LDR         R1, [R5, #208]              @ 0xd0 
    /*000015f0*/ MOV         R2, #0 
    /*000015f4*/ LDR         R3, [SP, #16] 
    /*000015f8*/ MOV         R0, #4 
    /*000015fc*/ STR         R2, [SP, #4] 
    /*00001600*/ MOVW        R2, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4d4] 
    /*00001604*/ STR         R1, [SP, #12] 
    /*00001608*/ MOVT        R2, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4d4] 
    /*0000160c*/ STR         R3, [SP, #8] 
    /*00001610*/ MOV         R1, #21 
    /*00001614*/ MOV         R3, #812                    @ 0x32c 
    /*00001618*/ STR         R3, [SP] 
    /*0000161c*/ LDR         R3, [PC, #584]              @ 0x000000000000186c 
    /*00001620*/ BL          0x0000000000001620          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001624*/ MOV         R0, #65024                  @ 0xfe00 
    /*00001628*/ MOVT        R0, #65535                  @ 0xffff 
    /*0000162c*/ B           AIGetFrame_x153c
    AIGetFrame_x1630:
    /*00001630*/ LDR         R3, [SP, #16] 
    /*00001634*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x504] 
    /*00001638*/ STR         R4, [SP, #4] 
    /*0000163c*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x504] 
    /*00001640*/ MOV         R1, #21 
    /*00001644*/ MOV         R0, #3 
    /*00001648*/ STR         R3, [SP, #8] 
    /*0000164c*/ MOVW        R3, #817                    @ 0x331 
    /*00001650*/ STR         R3, [SP] 
    /*00001654*/ LDR         R3, [PC, #528]              @ 0x000000000000186c 
    /*00001658*/ BL          0x0000000000001658          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000165c*/ MOVW        R0, #32782                  @ 0x800e 
    /*00001660*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001664*/ B           AIGetFrame_x153c
    AIGetFrame_x1668:
    /*00001668*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*0000166c*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001670*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*00001674*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00001678*/ MOVW        R2, #722                    @ 0x2d2 
    /*0000167c*/ LDR         R1, [PC, #488]              @ 0x000000000000186c 
    /*00001680*/ BL          0x0000000000001680          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001684*/ B           AIGetFrame_x1338
    AIGetFrame_x1688:
    /*00001688*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*0000168c*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001690*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*00001694*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00001698*/ MOVW        R2, #719                    @ 0x2cf 
    /*0000169c*/ LDR         R1, [PC, #456]              @ 0x000000000000186c 
    /*000016a0*/ BL          0x00000000000016A0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000016a4*/ B           AIGetFrame_x1328
    AIGetFrame_x16a8:
    /*000016a8*/ STR         R3, [SP, #4] 
    /*000016ac*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*000016b0*/ MOVW        R3, #715                    @ 0x2cb 
    /*000016b4*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*000016b8*/ STR         R3, [SP] 
    /*000016bc*/ MOV         R1, #20 
    /*000016c0*/ LDR         R3, [PC, #420]              @ 0x000000000000186c 
    /*000016c4*/ MOV         R0, #3 
    /*000016c8*/ BL          0x00000000000016C8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000016cc*/ MOVW        R0, #32770                  @ 0x8002 
    /*000016d0*/ MOVT        R0, #40981                  @ 0xa015 
    /*000016d4*/ B           AIGetFrame_x153c
    AIGetFrame_x16d8:
    /*000016d8*/ LDR         R0, [PC, #392]              @ 0x0000000000001868 
    /*000016dc*/ BL          0x00000000000016DC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000016e0*/ LDR         R3, [SP, #16] 
    /*000016e4*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x43c] 
    /*000016e8*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x43c] 
    /*000016ec*/ STR         R3, [SP, #4] 
    /*000016f0*/ MOV         R3, #756                    @ 0x2f4 
    /*000016f4*/ STR         R3, [SP] 
    AIGetFrame_x16f8:
    /*000016f8*/ LDR         R3, [PC, #364]              @ 0x000000000000186c 
    /*000016fc*/ MOV         R1, #21 
    /*00001700*/ MOV         R0, #3 
    /*00001704*/ BL          0x0000000000001704          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001708*/ MOVW        R0, #32773                  @ 0x8005 
    /*0000170c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001710*/ B           AIGetFrame_x153c
    AIGetFrame_x1714:
    /*00001714*/ LDR         R0, [PC, #332]              @ 0x0000000000001868 
    /*00001718*/ BL          0x0000000000001718          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000171c*/ MOVW        R3, #738                    @ 0x2e2 
    /*00001720*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x180] 
    /*00001724*/ STR         R6, [SP, #4] 
    /*00001728*/ STR         R3, [SP] 
    /*0000172c*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x180] 
    /*00001730*/ B           AIGetFrame_x16f8
    AIGetFrame_x1734:
    /*00001734*/ LDR         R0, [PC, #300]              @ 0x0000000000001868 
    /*00001738*/ BL          0x0000000000001738          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000173c*/ LDR         R3, [SP, #16] 
    /*00001740*/ STR         R6, [SP, #8] 
    /*00001744*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*00001748*/ MOV         R1, #21 
    /*0000174c*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*00001750*/ STR         R3, [SP, #4] 
    /*00001754*/ MOV         R0, #3 
    /*00001758*/ MOVW        R3, #747                    @ 0x2eb 
    /*0000175c*/ STR         R3, [SP] 
    /*00001760*/ LDR         R3, [PC, #260]              @ 0x000000000000186c 
    /*00001764*/ BL          0x0000000000001764          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001768*/ MOVW        R0, #32770                  @ 0x8002 
    /*0000176c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001770*/ B           AIGetFrame_x153c
    AIGetFrame_x1774:
    /*00001774*/ STR         R3, [SP, #4] 
    /*00001778*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x404] 
    /*0000177c*/ MOVW        R3, #726                    @ 0x2d6 
    /*00001780*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x404] 
    /*00001784*/ STR         R3, [SP] 
    /*00001788*/ MOV         R1, #21 
    /*0000178c*/ LDR         R3, [PC, #216]              @ 0x000000000000186c 
    /*00001790*/ MOV         R0, #3 
    /*00001794*/ BL          0x0000000000001794          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001798*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000179c*/ MOVT        R0, #40981                  @ 0xa015 
    /*000017a0*/ B           AIGetFrame_x153c
    AIGetFrame_x17a4:
    /*000017a4*/ LDR         R0, [PC, #188]              @ 0x0000000000001868 
    /*000017a8*/ BL          0x00000000000017A8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000017ac*/ LDR         R3, [SP, #16] 
    /*000017b0*/ MOVW        R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x480] 
    /*000017b4*/ MOV         R1, #21 
    /*000017b8*/ MOVT        R2, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x480] 
    /*000017bc*/ MOV         R0, #3 
    /*000017c0*/ STR         R3, [SP, #4] 
    /*000017c4*/ MOVW        R3, #762                    @ 0x2fa 
    /*000017c8*/ STR         R3, [SP] 
    /*000017cc*/ LDR         R3, [PC, #152]              @ 0x000000000000186c 
    /*000017d0*/ BL          0x00000000000017D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000017d4*/ MOVW        R0, #32776                  @ 0x8008 
    /*000017d8*/ MOVT        R0, #40981                  @ 0xa015 
    /*000017dc*/ B           AIGetFrame_x153c
    AIGetFrame_x17e0:
    /*000017e0*/ MOVW        R3, #714                    @ 0x2ca 
    /*000017e4*/ STR         R4, [SP, #4] 
    /*000017e8*/ STR         R3, [SP] 
    /*000017ec*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*000017f0*/ LDR         R3, [PC, #116]              @ 0x000000000000186c 
    /*000017f4*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*000017f8*/ MOV         R1, #21 
    /*000017fc*/ MOV         R0, #3 
    /*00001800*/ BL          0x0000000000001800          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001804*/ MOVW        R0, #32769                  @ 0x8001 
    /*00001808*/ MOVT        R0, #40981                  @ 0xa015 
    /*0000180c*/ B           AIGetFrame_x153c
    AIGetFrame_x1810:
    /*00001810*/ MOVW        R0, #32774                  @ 0x8006 
    /*00001814*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001818*/ B           AIGetFrame_x153c
    AIGetFrame_x181c:
    /*0000181c*/ MOVW        R0, #32784                  @ 0x8010 
    /*00001820*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001824*/ B           AIGetFrame_x153c
    AIGetFrame_x1828:
    /*00001828*/ MOVW        R3, #846                    @ 0x34e 
    /*0000182c*/ MOV         R1, #21 
    /*00001830*/ STR         R3, [SP] 
    /*00001834*/ MOVW        R2, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x57c] 
    /*00001838*/ LDR         R3, [PC, #44]               @ 0x000000000000186c 
    /*0000183c*/ MOVT        R2, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x57c] 
    /*00001840*/ MOV         R0, #3 
    /*00001844*/ BL          0x0000000000001844          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001848*/ ADD         R1, SP, #28 
    /*0000184c*/ LDR         R0, [PC, #28]               @ 0x0000000000001870 
    /*00001850*/ BL          0x0000000000001850          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001854*/ LDR         R0, [PC, #12]               @ 0x0000000000001868 
    /*00001858*/ BL          0x0000000000001858          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000185c*/ MOVW        R0, #32782                  @ 0x800e 
    /*00001860*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001864*/ B           AIGetFrame_x153c
    /*00001868*/ ANDEQ       R0, R0, IP, ROR R0          @ g_stAiDev [R_ARM_ABS32: .bss+0x8] 
    /*0000186c*/ ANDEQ       R0, R0, R4, ROR R0 
    /*00001870*/ ANDEQ       R0, R0, R0, LSL #1          @ g_stAiDev [R_ARM_ABS32: .bss+0x8] 
FUNC_END AIGetFrame


FUNC_BEGIN AIReleaseFrame
    /*00001874*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00001878*/ MOV         R4, R0 
    /*0000187c*/ SUB         SP, SP, #28 
    /*00001880*/ MOV         R0, #20 
    /*00001884*/ MOV         SL, R1 
    /*00001888*/ MOV         R7, R2 
    /*0000188c*/ BL          0x000000000000188C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00001890*/ CMP         R0, #0 
    /*00001894*/ BEQ         AIReleaseFrame_x1b44
    /*00001898*/ CMP         R4, #0 
    /*0000189c*/ BNE         AIReleaseFrame_x1ad4
    /*000018a0*/ CMP         SL, #1 
    /*000018a4*/ BHI         AIReleaseFrame_x1a60
    /*000018a8*/ CMP         R7, #0 
    /*000018ac*/ BEQ         AIReleaseFrame_x1b50
    /*000018b0*/ MOVW        R9, #0                      @ g_stAiDev [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*000018b4*/ MOVT        R9, #0                      @ g_stAiDev [R_ARM_MOVT_ABS: .bss+0x8] 
    /*000018b8*/ CMP         R9, #0 
    /*000018bc*/ BEQ         AIReleaseFrame_x1a20
    AIReleaseFrame_x18c0:
    /*000018c0*/ MOV         R0, SL 
    /*000018c4*/ BL          0x00000000000018C4          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*000018c8*/ SUBS        FP, R0, #0 
    /*000018cc*/ BEQ         AIReleaseFrame_x1a40
    AIReleaseFrame_x18d0:
    /*000018d0*/ LDR         R0, [PC, #664]              @ 0x0000000000001b70 
    /*000018d4*/ BL          0x00000000000018D4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000018d8*/ SUBS        R4, R0, #0 
    /*000018dc*/ BNE         AIReleaseFrame_x1ac8
    /*000018e0*/ LDR         R1, [R9, #12] 
    /*000018e4*/ CMP         R1, #1 
    /*000018e8*/ BNE         AIReleaseFrame_x1a90
    /*000018ec*/ LDR         R3, [R7, #4] 
    /*000018f0*/ LDR         R0, [R9, #44]               @ 0x2c 
    /*000018f4*/ CMP         R3, #1 
    /*000018f8*/ MOVEQ       R1, #2 
    /*000018fc*/ MOV         R8, R1 
    /*00001900*/ BL          0x0000000000001900          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00001904*/ CMP         SL, R0 
    /*00001908*/ ADDCC       R6, R7, #48                 @ 0x30 
    /*0000190c*/ ADDCC       R5, R7, #16 
    /*00001910*/ BCC         AIReleaseFrame_x1924
    /*00001914*/ B           AIReleaseFrame_x1b04
    AIReleaseFrame_x1918:
    /*00001918*/ ADD         R4, R4, #1 
    /*0000191c*/ CMP         R8, R4 
    /*00001920*/ BEQ         AIReleaseFrame_x198c
    AIReleaseFrame_x1924:
    /*00001924*/ MOV         R0, #1 
    /*00001928*/ BL          0x0000000000001928          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000192c*/ LDRD        R2, [R5], #8 
    /*00001930*/ LDR         R1, [R0, #84]               @ 0x54 
    /*00001934*/ LDR         R0, [R6], #4 
    /*00001938*/ BLX         R1 
    /*0000193c*/ CMP         R0, #1 
    /*00001940*/ BNE         AIReleaseFrame_x19cc
    /*00001944*/ LDR         R3, [R7, #112]              @ 0x70 
    /*00001948*/ CMP         R3, #1 
    /*0000194c*/ BNE         AIReleaseFrame_x1918
    /*00001950*/ BL          0x0000000000001950          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00001954*/ LDRD        R2, [R5, #48]               @ 0x30 
    /*00001958*/ LDR         R1, [R0, #84]               @ 0x54 
    /*0000195c*/ LDR         R0, [R6, #52]               @ 0x34 
    /*00001960*/ BLX         R1 
    /*00001964*/ CMP         R0, #1 
    /*00001968*/ BEQ         AIReleaseFrame_x1918
    /*0000196c*/ ADD         R4, R7, R4, LSL #3 
    /*00001970*/ LDR         R0, [PC, #504]              @ 0x0000000000001b70 
    /*00001974*/ BL          0x0000000000001974          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001978*/ MOVW        R5, #32771                  @ 0x8003 
    /*0000197c*/ MOV         R1, #0 
    /*00001980*/ MOVW        R3, #930                    @ 0x3a2 
    /*00001984*/ LDRD        R6, [R4, #72]               @ 0x48 
    /*00001988*/ B           AIReleaseFrame_x19e8
    AIReleaseFrame_x198c:
    /*0000198c*/ MOV         R1, #12 
    /*00001990*/ MOV         R0, R7 
    /*00001994*/ BL          0x0000000000001994          @ AISubVb [R_ARM_CALL: .text+0x3a50] 
    /*00001998*/ SUBS        R5, R0, #0 
    /*0000199c*/ BNE         AIReleaseFrame_x1b5c
    /*000019a0*/ LDR         R3, [R7, #48]               @ 0x30 
    /*000019a4*/ LDR         R2, [R9] 
    /*000019a8*/ LDR         R0, [PC, #448]              @ 0x0000000000001b70 
    /*000019ac*/ CMP         R2, R3 
    /*000019b0*/ LDREQ       R3, [FP, #1108]             @ 0x454 
    /*000019b4*/ ADDEQ       R3, R3, #1 
    /*000019b8*/ STREQ       R3, [FP, #1108]             @ 0x454 
    /*000019bc*/ BL          0x00000000000019BC          @ osal_up [R_ARM_CALL: +0x0] 
    AIReleaseFrame_x19c0:
    /*000019c0*/ MOV         R0, R5 
    /*000019c4*/ ADD         SP, SP, #28 
    /*000019c8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIReleaseFrame_x19cc:
    /*000019cc*/ ADD         R4, R7, R4, LSL #3 
    /*000019d0*/ LDR         R0, [PC, #408]              @ 0x0000000000001b70 
    /*000019d4*/ BL          0x00000000000019D4          @ osal_up [R_ARM_CALL: +0x0] 
    /*000019d8*/ MOVW        R5, #32771                  @ 0x8003 
    /*000019dc*/ LDRD        R6, [R4, #16] 
    /*000019e0*/ MOV         R1, #0 
    /*000019e4*/ MOVW        R3, #919                    @ 0x397 
    AIReleaseFrame_x19e8:
    /*000019e8*/ STR         R1, [SP, #4] 
    /*000019ec*/ MOVW        R2, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5a8] 
    /*000019f0*/ STR         R3, [SP] 
    /*000019f4*/ MOVT        R2, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5a8] 
    /*000019f8*/ STR         SL, [SP, #8] 
    /*000019fc*/ MOV         R1, #21 
    /*00001a00*/ STRD        R6, [SP, #16] 
    /*00001a04*/ MOV         R0, #3 
    /*00001a08*/ LDR         R3, [PC, #356]              @ 0x0000000000001b74 
    /*00001a0c*/ MOVT        R5, #40981                  @ 0xa015 
    /*00001a10*/ BL          0x0000000000001A10          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001a14*/ MOV         R0, R5 
    /*00001a18*/ ADD         SP, SP, #28 
    /*00001a1c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIReleaseFrame_x1a20:
    /*00001a20*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*00001a24*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001a28*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*00001a2c*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00001a30*/ MOV         R2, #884                    @ 0x374 
    /*00001a34*/ LDR         R1, [PC, #312]              @ 0x0000000000001b74 
    /*00001a38*/ BL          0x0000000000001A38          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001a3c*/ B           AIReleaseFrame_x18c0
    AIReleaseFrame_x1a40:
    /*00001a40*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*00001a44*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001a48*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*00001a4c*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00001a50*/ MOVW        R2, #887                    @ 0x377 
    /*00001a54*/ LDR         R1, [PC, #280]              @ 0x0000000000001b74 
    /*00001a58*/ BL          0x0000000000001A58          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001a5c*/ B           AIReleaseFrame_x18d0
    AIReleaseFrame_x1a60:
    /*00001a60*/ MOV         R3, #880                    @ 0x370 
    /*00001a64*/ STR         SL, [SP, #4] 
    /*00001a68*/ STR         R3, [SP] 
    /*00001a6c*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00001a70*/ LDR         R3, [PC, #252]              @ 0x0000000000001b74 
    /*00001a74*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*00001a78*/ MOV         R1, #20 
    /*00001a7c*/ MOV         R0, #3 
    /*00001a80*/ MOVW        R5, #32770                  @ 0x8002 
    /*00001a84*/ BL          0x0000000000001A84          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001a88*/ MOVT        R5, #40981                  @ 0xa015 
    /*00001a8c*/ B           AIReleaseFrame_x19c0
    AIReleaseFrame_x1a90:
    /*00001a90*/ LDR         R0, [PC, #216]              @ 0x0000000000001b70 
    /*00001a94*/ MOVW        R5, #32773                  @ 0x8005 
    /*00001a98*/ BL          0x0000000000001A98          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001a9c*/ MOVW        R3, #898                    @ 0x382 
    /*00001aa0*/ STR         R4, [SP, #4] 
    /*00001aa4*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x180] 
    /*00001aa8*/ STR         R3, [SP] 
    /*00001aac*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x180] 
    /*00001ab0*/ LDR         R3, [PC, #188]              @ 0x0000000000001b74 
    /*00001ab4*/ MOV         R1, #21 
    /*00001ab8*/ MOV         R0, #3 
    /*00001abc*/ MOVT        R5, #40981                  @ 0xa015 
    /*00001ac0*/ BL          0x0000000000001AC0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001ac4*/ B           AIReleaseFrame_x19c0
    AIReleaseFrame_x1ac8:
    /*00001ac8*/ MOV         R5, #65024                  @ 0xfe00 
    /*00001acc*/ MOVT        R5, #65535                  @ 0xffff 
    /*00001ad0*/ B           AIReleaseFrame_x19c0
    AIReleaseFrame_x1ad4:
    /*00001ad4*/ MOVW        R3, #879                    @ 0x36f 
    /*00001ad8*/ STR         R4, [SP, #4] 
    /*00001adc*/ STR         R3, [SP] 
    /*00001ae0*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00001ae4*/ LDR         R3, [PC, #136]              @ 0x0000000000001b74 
    /*00001ae8*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*00001aec*/ MOV         R1, #21 
    /*00001af0*/ MOV         R0, #3 
    /*00001af4*/ MOVW        R5, #32769                  @ 0x8001 
    /*00001af8*/ BL          0x0000000000001AF8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001afc*/ MOVT        R5, #40981                  @ 0xa015 
    /*00001b00*/ B           AIReleaseFrame_x19c0
    AIReleaseFrame_x1b04:
    /*00001b04*/ LDR         R0, [PC, #100]              @ 0x0000000000001b70 
    /*00001b08*/ MOVW        R5, #32770                  @ 0x8002 
    /*00001b0c*/ BL          0x0000000000001B0C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001b10*/ MOV         R2, #0 
    /*00001b14*/ MOVW        R3, #907                    @ 0x38b 
    /*00001b18*/ STR         R2, [SP, #8] 
    /*00001b1c*/ STR         R3, [SP] 
    /*00001b20*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*00001b24*/ STR         SL, [SP, #4] 
    /*00001b28*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*00001b2c*/ LDR         R3, [PC, #64]               @ 0x0000000000001b74 
    /*00001b30*/ MOV         R1, #21 
    /*00001b34*/ MOV         R0, #3 
    /*00001b38*/ MOVT        R5, #40981                  @ 0xa015 
    /*00001b3c*/ BL          0x0000000000001B3C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001b40*/ B           AIReleaseFrame_x19c0
    AIReleaseFrame_x1b44:
    /*00001b44*/ MOVW        R5, #32784                  @ 0x8010 
    /*00001b48*/ MOVT        R5, #40981                  @ 0xa015 
    /*00001b4c*/ B           AIReleaseFrame_x19c0
    AIReleaseFrame_x1b50:
    /*00001b50*/ MOVW        R5, #32774                  @ 0x8006 
    /*00001b54*/ MOVT        R5, #40981                  @ 0xa015 
    /*00001b58*/ B           AIReleaseFrame_x19c0
    AIReleaseFrame_x1b5c:
    /*00001b5c*/ LDR         R0, [PC, #12]               @ 0x0000000000001b70 
    /*00001b60*/ MOVW        R5, #32771                  @ 0x8003 
    /*00001b64*/ BL          0x0000000000001B64          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001b68*/ MOVT        R5, #40981                  @ 0xa015 
    /*00001b6c*/ B           AIReleaseFrame_x19c0
    /*00001b70*/ ANDEQ       R0, R0, IP, ROR R0          @ g_stAiDev [R_ARM_ABS32: .bss+0x8] 
    /*00001b74*/ ANDEQ       R0, R0, R0, LSL #1 
FUNC_END AIReleaseFrame


FUNC_BEGIN AISetChnResmpInfo
    /*00001b78*/ CMP         R0, #0 
    /*00001b7c*/ PUSH        {R4, R5, LR} 
    /*00001b80*/ SUB         SP, SP, #12 
    /*00001b84*/ BNE         AISetChnResmpInfo_x1c28
    /*00001b88*/ CMP         R1, #1 
    /*00001b8c*/ BHI         AISetChnResmpInfo_x1bf8
    /*00001b90*/ CMP         R2, #0 
    /*00001b94*/ MOV         R4, R2 
    /*00001b98*/ BEQ         AISetChnResmpInfo_x1c58
    /*00001b9c*/ MOV         R0, R1 
    /*00001ba0*/ BL          0x0000000000001BA0          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*00001ba4*/ SUBS        R5, R0, #0 
    /*00001ba8*/ BEQ         AISetChnResmpInfo_x1bd8
    AISetChnResmpInfo_x1bac:
    /*00001bac*/ LDR         R3, [R4] 
    /*00001bb0*/ MOV         R0, #0 
    /*00001bb4*/ STR         R3, [R5, #564]              @ 0x234 
    /*00001bb8*/ LDR         R3, [R4, #4] 
    /*00001bbc*/ STR         R3, [R5, #568]              @ 0x238 
    /*00001bc0*/ LDR         R3, [R4, #8] 
    /*00001bc4*/ STR         R3, [R5, #572]              @ 0x23c 
    /*00001bc8*/ LDR         R3, [R4, #12] 
    /*00001bcc*/ STR         R3, [R5, #576]              @ 0x240 
    AISetChnResmpInfo_x1bd0:
    /*00001bd0*/ ADD         SP, SP, #12 
    /*00001bd4*/ POP         {R4, R5, PC} 
    AISetChnResmpInfo_x1bd8:
    /*00001bd8*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*00001bdc*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001be0*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*00001be4*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00001be8*/ MOVW        R2, #963                    @ 0x3c3 
    /*00001bec*/ LDR         R1, [PC, #112]              @ 0x0000000000001c64 
    /*00001bf0*/ BL          0x0000000000001BF0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001bf4*/ B           AISetChnResmpInfo_x1bac
    AISetChnResmpInfo_x1bf8:
    /*00001bf8*/ STR         R1, [SP, #4] 
    /*00001bfc*/ MOVW        R3, #958                    @ 0x3be 
    /*00001c00*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00001c04*/ STR         R3, [SP] 
    /*00001c08*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*00001c0c*/ LDR         R3, [PC, #80]               @ 0x0000000000001c64 
    /*00001c10*/ MOV         R1, #20 
    /*00001c14*/ MOV         R0, #3 
    /*00001c18*/ BL          0x0000000000001C18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001c1c*/ MOVW        R0, #32770                  @ 0x8002 
    /*00001c20*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001c24*/ B           AISetChnResmpInfo_x1bd0
    AISetChnResmpInfo_x1c28:
    /*00001c28*/ STR         R0, [SP, #4] 
    /*00001c2c*/ MOVW        R3, #957                    @ 0x3bd 
    /*00001c30*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00001c34*/ STR         R3, [SP] 
    /*00001c38*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*00001c3c*/ LDR         R3, [PC, #32]               @ 0x0000000000001c64 
    /*00001c40*/ MOV         R1, #21 
    /*00001c44*/ MOV         R0, #3 
    /*00001c48*/ BL          0x0000000000001C48          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001c4c*/ MOVW        R0, #32769                  @ 0x8001 
    /*00001c50*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001c54*/ B           AISetChnResmpInfo_x1bd0
    AISetChnResmpInfo_x1c58:
    /*00001c58*/ MOVW        R0, #32774                  @ 0x8006 
    /*00001c5c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001c60*/ B           AISetChnResmpInfo_x1bd0
    /*00001c64*/ MULEQ       R0, R0, R0 
FUNC_END AISetChnResmpInfo


FUNC_BEGIN AISetChnVqeInfo
    /*00001c68*/ CMP         R0, #0 
    /*00001c6c*/ PUSH        {R4, R5, LR} 
    /*00001c70*/ SUB         SP, SP, #12 
    /*00001c74*/ BNE         AISetChnVqeInfo_x1d08
    /*00001c78*/ CMP         R1, #1 
    /*00001c7c*/ BHI         AISetChnVqeInfo_x1cd8
    /*00001c80*/ CMP         R2, #0 
    /*00001c84*/ MOV         R4, R2 
    /*00001c88*/ BEQ         AISetChnVqeInfo_x1d38
    /*00001c8c*/ MOV         R0, R1 
    /*00001c90*/ BL          0x0000000000001C90          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*00001c94*/ SUBS        R5, R0, #0 
    /*00001c98*/ BEQ         AISetChnVqeInfo_x1cb8
    AISetChnVqeInfo_x1c9c:
    /*00001c9c*/ MOV         R1, R4 
    /*00001ca0*/ ADD         R0, R5, #240                @ 0xf0 
    /*00001ca4*/ MOV         R2, #320                    @ 0x140 
    /*00001ca8*/ BL          0x0000000000001CA8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00001cac*/ MOV         R0, #0 
    AISetChnVqeInfo_x1cb0:
    /*00001cb0*/ ADD         SP, SP, #12 
    /*00001cb4*/ POP         {R4, R5, PC} 
    AISetChnVqeInfo_x1cb8:
    /*00001cb8*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*00001cbc*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001cc0*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*00001cc4*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00001cc8*/ MOVW        R2, #985                    @ 0x3d9 
    /*00001ccc*/ LDR         R1, [PC, #112]              @ 0x0000000000001d44 
    /*00001cd0*/ BL          0x0000000000001CD0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001cd4*/ B           AISetChnVqeInfo_x1c9c
    AISetChnVqeInfo_x1cd8:
    /*00001cd8*/ STR         R1, [SP, #4] 
    /*00001cdc*/ MOV         R3, #980                    @ 0x3d4 
    /*00001ce0*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00001ce4*/ STR         R3, [SP] 
    /*00001ce8*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*00001cec*/ LDR         R3, [PC, #80]               @ 0x0000000000001d44 
    /*00001cf0*/ MOV         R1, #20 
    /*00001cf4*/ MOV         R0, #3 
    /*00001cf8*/ BL          0x0000000000001CF8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001cfc*/ MOVW        R0, #32770                  @ 0x8002 
    /*00001d00*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001d04*/ B           AISetChnVqeInfo_x1cb0
    AISetChnVqeInfo_x1d08:
    /*00001d08*/ STR         R0, [SP, #4] 
    /*00001d0c*/ MOVW        R3, #979                    @ 0x3d3 
    /*00001d10*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00001d14*/ STR         R3, [SP] 
    /*00001d18*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*00001d1c*/ LDR         R3, [PC, #32]               @ 0x0000000000001d44 
    /*00001d20*/ MOV         R1, #21 
    /*00001d24*/ MOV         R0, #3 
    /*00001d28*/ BL          0x0000000000001D28          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001d2c*/ MOVW        R0, #32769                  @ 0x8001 
    /*00001d30*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001d34*/ B           AISetChnVqeInfo_x1cb0
    AISetChnVqeInfo_x1d38:
    /*00001d38*/ MOVW        R0, #32774                  @ 0x8006 
    /*00001d3c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001d40*/ B           AISetChnVqeInfo_x1cb0
    /*00001d44*/ ANDEQ       R0, R0, R4, LSR #1 
FUNC_END AISetChnVqeInfo


FUNC_BEGIN AISetChnVqeVolInfo
    /*00001d48*/ CMP         R0, #0 
    /*00001d4c*/ PUSH        {R4, R5, LR} 
    /*00001d50*/ SUB         SP, SP, #12 
    /*00001d54*/ BNE         AISetChnVqeVolInfo_x1de0
    /*00001d58*/ CMP         R1, #1 
    /*00001d5c*/ BHI         AISetChnVqeVolInfo_x1db0
    /*00001d60*/ CMP         R2, #0 
    /*00001d64*/ MOV         R4, R2 
    /*00001d68*/ BEQ         AISetChnVqeVolInfo_x1e10
    /*00001d6c*/ MOV         R0, R1 
    /*00001d70*/ BL          0x0000000000001D70          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*00001d74*/ SUBS        R5, R0, #0 
    /*00001d78*/ BEQ         AISetChnVqeVolInfo_x1d90
    AISetChnVqeVolInfo_x1d7c:
    /*00001d7c*/ LDR         R3, [R4] 
    /*00001d80*/ MOV         R0, #0 
    /*00001d84*/ STR         R3, [R5, #560]              @ 0x230 
    AISetChnVqeVolInfo_x1d88:
    /*00001d88*/ ADD         SP, SP, #12 
    /*00001d8c*/ POP         {R4, R5, PC} 
    AISetChnVqeVolInfo_x1d90:
    /*00001d90*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*00001d94*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001d98*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*00001d9c*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00001da0*/ MOVW        R2, #1003                   @ 0x3eb 
    /*00001da4*/ LDR         R1, [PC, #112]              @ 0x0000000000001e1c 
    /*00001da8*/ BL          0x0000000000001DA8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001dac*/ B           AISetChnVqeVolInfo_x1d7c
    AISetChnVqeVolInfo_x1db0:
    /*00001db0*/ STR         R1, [SP, #4] 
    /*00001db4*/ MOVW        R3, #998                    @ 0x3e6 
    /*00001db8*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00001dbc*/ STR         R3, [SP] 
    /*00001dc0*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*00001dc4*/ LDR         R3, [PC, #80]               @ 0x0000000000001e1c 
    /*00001dc8*/ MOV         R1, #20 
    /*00001dcc*/ MOV         R0, #3 
    /*00001dd0*/ BL          0x0000000000001DD0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001dd4*/ MOVW        R0, #32770                  @ 0x8002 
    /*00001dd8*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001ddc*/ B           AISetChnVqeVolInfo_x1d88
    AISetChnVqeVolInfo_x1de0:
    /*00001de0*/ STR         R0, [SP, #4] 
    /*00001de4*/ MOVW        R3, #997                    @ 0x3e5 
    /*00001de8*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00001dec*/ STR         R3, [SP] 
    /*00001df0*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*00001df4*/ LDR         R3, [PC, #32]               @ 0x0000000000001e1c 
    /*00001df8*/ MOV         R1, #21 
    /*00001dfc*/ MOV         R0, #3 
    /*00001e00*/ BL          0x0000000000001E00          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001e04*/ MOVW        R0, #32769                  @ 0x8001 
    /*00001e08*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001e0c*/ B           AISetChnVqeVolInfo_x1d88
    AISetChnVqeVolInfo_x1e10:
    /*00001e10*/ MOVW        R0, #32774                  @ 0x8006 
    /*00001e14*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001e18*/ B           AISetChnVqeVolInfo_x1d88
    /*00001e1c*/ STRHEQ      R0, [R0], -R4 
FUNC_END AISetChnVqeVolInfo


FUNC_BEGIN AiSetTrackMode
    /*00001e20*/ PUSH        {R4, R5, R6, LR} 
    /*00001e24*/ MOV         R4, R0 
    /*00001e28*/ SUB         SP, SP, #8 
    /*00001e2c*/ MOV         R0, #20 
    /*00001e30*/ MOV         R5, R1 
    /*00001e34*/ BL          0x0000000000001E34          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00001e38*/ CMP         R0, #0 
    /*00001e3c*/ BEQ         AiSetTrackMode_x1f28
    /*00001e40*/ CMP         R4, #0 
    /*00001e44*/ BNE         AiSetTrackMode_x1ef8
    /*00001e48*/ MOVW        R4, #0                      @ g_stAiDev [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*00001e4c*/ MOVT        R4, #0                      @ g_stAiDev [R_ARM_MOVT_ABS: .bss+0x8] 
    /*00001e50*/ CMP         R4, #0 
    /*00001e54*/ BEQ         AiSetTrackMode_x1ecc
    AiSetTrackMode_x1e58:
    /*00001e58*/ LDR         R0, [PC, #212]              @ 0x0000000000001f34 
    /*00001e5c*/ BL          0x0000000000001E5C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001e60*/ SUBS        R6, R0, #0 
    /*00001e64*/ BNE         AiSetTrackMode_x1eec
    /*00001e68*/ LDR         R3, [R4, #12] 
    /*00001e6c*/ CMP         R3, #1 
    /*00001e70*/ BNE         AiSetTrackMode_x1e94
    /*00001e74*/ MOV         R1, R5 
    /*00001e78*/ BL          0x0000000000001E78          @ AI_DRV_SetTrackMode [R_ARM_CALL: .text+0x9454] 
    /*00001e7c*/ MOV         R4, R0 
    /*00001e80*/ LDR         R0, [PC, #172]              @ 0x0000000000001f34 
    /*00001e84*/ BL          0x0000000000001E84          @ osal_up [R_ARM_CALL: +0x0] 
    AiSetTrackMode_x1e88:
    /*00001e88*/ MOV         R0, R4 
    /*00001e8c*/ ADD         SP, SP, #8 
    /*00001e90*/ POP         {R4, R5, R6, PC} 
    AiSetTrackMode_x1e94:
    /*00001e94*/ LDR         R0, [PC, #152]              @ 0x0000000000001f34 
    /*00001e98*/ MOVW        R4, #32773                  @ 0x8005 
    /*00001e9c*/ BL          0x0000000000001E9C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001ea0*/ MOVW        R3, #1030                   @ 0x406 
    /*00001ea4*/ STR         R6, [SP, #4] 
    /*00001ea8*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x180] 
    /*00001eac*/ STR         R3, [SP] 
    /*00001eb0*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x180] 
    /*00001eb4*/ LDR         R3, [PC, #124]              @ 0x0000000000001f38 
    /*00001eb8*/ MOV         R1, #21 
    /*00001ebc*/ MOV         R0, #3 
    /*00001ec0*/ MOVT        R4, #40981                  @ 0xa015 
    /*00001ec4*/ BL          0x0000000000001EC4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001ec8*/ B           AiSetTrackMode_x1e88
    AiSetTrackMode_x1ecc:
    /*00001ecc*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*00001ed0*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001ed4*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*00001ed8*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00001edc*/ MOVW        R2, #1019                   @ 0x3fb 
    /*00001ee0*/ LDR         R1, [PC, #80]               @ 0x0000000000001f38 
    /*00001ee4*/ BL          0x0000000000001EE4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001ee8*/ B           AiSetTrackMode_x1e58
    AiSetTrackMode_x1eec:
    /*00001eec*/ MOV         R4, #65024                  @ 0xfe00 
    /*00001ef0*/ MOVT        R4, #65535                  @ 0xffff 
    /*00001ef4*/ B           AiSetTrackMode_x1e88
    AiSetTrackMode_x1ef8:
    /*00001ef8*/ STR         R4, [SP, #4] 
    /*00001efc*/ MOV         R3, #1016                   @ 0x3f8 
    /*00001f00*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00001f04*/ STR         R3, [SP] 
    /*00001f08*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*00001f0c*/ LDR         R3, [PC, #36]               @ 0x0000000000001f38 
    /*00001f10*/ MOV         R1, #21 
    /*00001f14*/ MOV         R0, #3 
    /*00001f18*/ MOVW        R4, #32769                  @ 0x8001 
    /*00001f1c*/ BL          0x0000000000001F1C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001f20*/ MOVT        R4, #40981                  @ 0xa015 
    /*00001f24*/ B           AiSetTrackMode_x1e88
    AiSetTrackMode_x1f28:
    /*00001f28*/ MOVW        R4, #32784                  @ 0x8010 
    /*00001f2c*/ MOVT        R4, #40981                  @ 0xa015 
    /*00001f30*/ B           AiSetTrackMode_x1e88
    /*00001f34*/ ANDEQ       R0, R0, IP, ROR R0          @ g_stAiDev [R_ARM_ABS32: .bss+0x8] 
    /*00001f38*/ ANDEQ       R0, R0, R8, ASR #1 
FUNC_END AiSetTrackMode


FUNC_BEGIN AiGetTrackMode
    /*00001f3c*/ PUSH        {R4, R5, R6, LR} 
    /*00001f40*/ MOV         R4, R0 
    /*00001f44*/ SUB         SP, SP, #8 
    /*00001f48*/ MOV         R0, #20 
    /*00001f4c*/ MOV         R5, R1 
    /*00001f50*/ BL          0x0000000000001F50          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00001f54*/ CMP         R0, #0 
    /*00001f58*/ BEQ         AiGetTrackMode_x2058
    /*00001f5c*/ CMP         R4, #0 
    /*00001f60*/ BNE         AiGetTrackMode_x2028
    /*00001f64*/ MOVW        R4, #0                      @ g_stAiDev [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*00001f68*/ MOVT        R4, #0                      @ g_stAiDev [R_ARM_MOVT_ABS: .bss+0x8] 
    /*00001f6c*/ CMP         R4, #0 
    /*00001f70*/ BEQ         AiGetTrackMode_x1ff0
    AiGetTrackMode_x1f74:
    /*00001f74*/ CMP         R5, #0 
    /*00001f78*/ BEQ         AiGetTrackMode_x201c
    /*00001f7c*/ LDR         R0, [PC, #224]              @ 0x0000000000002064 
    /*00001f80*/ BL          0x0000000000001F80          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001f84*/ SUBS        R6, R0, #0 
    /*00001f88*/ BNE         AiGetTrackMode_x2010
    /*00001f8c*/ LDR         R3, [R4, #12] 
    /*00001f90*/ CMP         R3, #1 
    /*00001f94*/ BNE         AiGetTrackMode_x1fb8
    /*00001f98*/ MOV         R1, R5 
    /*00001f9c*/ BL          0x0000000000001F9C          @ AI_DRV_GetTrackMode [R_ARM_CALL: .text+0x950c] 
    /*00001fa0*/ MOV         R4, R0 
    /*00001fa4*/ LDR         R0, [PC, #184]              @ 0x0000000000002064 
    /*00001fa8*/ BL          0x0000000000001FA8          @ osal_up [R_ARM_CALL: +0x0] 
    AiGetTrackMode_x1fac:
    /*00001fac*/ MOV         R0, R4 
    /*00001fb0*/ ADD         SP, SP, #8 
    /*00001fb4*/ POP         {R4, R5, R6, PC} 
    AiGetTrackMode_x1fb8:
    /*00001fb8*/ LDR         R0, [PC, #164]              @ 0x0000000000002064 
    /*00001fbc*/ MOVW        R4, #32773                  @ 0x8005 
    /*00001fc0*/ BL          0x0000000000001FC0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001fc4*/ MOVW        R3, #1062                   @ 0x426 
    /*00001fc8*/ STR         R6, [SP, #4] 
    /*00001fcc*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x180] 
    /*00001fd0*/ STR         R3, [SP] 
    /*00001fd4*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x180] 
    /*00001fd8*/ LDR         R3, [PC, #136]              @ 0x0000000000002068 
    /*00001fdc*/ MOV         R1, #21 
    /*00001fe0*/ MOV         R0, #3 
    /*00001fe4*/ MOVT        R4, #40981                  @ 0xa015 
    /*00001fe8*/ BL          0x0000000000001FE8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001fec*/ B           AiGetTrackMode_x1fac
    AiGetTrackMode_x1ff0:
    /*00001ff0*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*00001ff4*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001ff8*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*00001ffc*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00002000*/ MOVW        R2, #1049                   @ 0x419 
    /*00002004*/ LDR         R1, [PC, #92]               @ 0x0000000000002068 
    /*00002008*/ BL          0x0000000000002008          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000200c*/ B           AiGetTrackMode_x1f74
    AiGetTrackMode_x2010:
    /*00002010*/ MOV         R4, #65024                  @ 0xfe00 
    /*00002014*/ MOVT        R4, #65535                  @ 0xffff 
    /*00002018*/ B           AiGetTrackMode_x1fac
    AiGetTrackMode_x201c:
    /*0000201c*/ MOVW        R4, #32774                  @ 0x8006 
    /*00002020*/ MOVT        R4, #40981                  @ 0xa015 
    /*00002024*/ B           AiGetTrackMode_x1fac
    AiGetTrackMode_x2028:
    /*00002028*/ STR         R4, [SP, #4] 
    /*0000202c*/ MOVW        R3, #1046                   @ 0x416 
    /*00002030*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00002034*/ STR         R3, [SP] 
    /*00002038*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*0000203c*/ LDR         R3, [PC, #36]               @ 0x0000000000002068 
    /*00002040*/ MOV         R1, #21 
    /*00002044*/ MOV         R0, #3 
    /*00002048*/ MOVW        R4, #32769                  @ 0x8001 
    /*0000204c*/ BL          0x000000000000204C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002050*/ MOVT        R4, #40981                  @ 0xa015 
    /*00002054*/ B           AiGetTrackMode_x1fac
    AiGetTrackMode_x2058:
    /*00002058*/ MOVW        R4, #32784                  @ 0x8010 
    /*0000205c*/ MOVT        R4, #40981                  @ 0xa015 
    /*00002060*/ B           AiGetTrackMode_x1fac
    /*00002064*/ ANDEQ       R0, R0, IP, ROR R0          @ g_stAiDev [R_ARM_ABS32: .bss+0x8] 
    /*00002068*/ LDRDEQ      R0, [R0], -R8 
FUNC_END AiGetTrackMode


FUNC_BEGIN AiSetClkDir
    /*0000206c*/ PUSH        {R4, R5, LR} 
    /*00002070*/ MOV         R4, R0 
    /*00002074*/ SUB         SP, SP, #20 
    /*00002078*/ MOV         R0, #20 
    /*0000207c*/ MOV         R5, R1 
    /*00002080*/ BL          0x0000000000002080          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002084*/ CMP         R0, #0 
    /*00002088*/ BEQ         AiSetClkDir_x2194
    /*0000208c*/ CMP         R4, #0 
    /*00002090*/ BNE         AiSetClkDir_x2164
    /*00002094*/ MOVW        R4, #0                      @ g_stAiDev [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*00002098*/ MOVT        R4, #0                      @ g_stAiDev [R_ARM_MOVT_ABS: .bss+0x8] 
    /*0000209c*/ CMP         R4, #0 
    /*000020a0*/ BEQ         AiSetClkDir_x20ec
    AiSetClkDir_x20a4:
    /*000020a4*/ LDR         R0, [PC, #244]              @ 0x00000000000021a0 
    /*000020a8*/ BL          0x00000000000020A8          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000020ac*/ CMP         R0, #0 
    /*000020b0*/ BNE         AiSetClkDir_x2158
    /*000020b4*/ LDR         R3, [R4, #8] 
    /*000020b8*/ CMP         R3, #0 
    /*000020bc*/ BEQ         AiSetClkDir_x210c
    /*000020c0*/ LDR         R3, [R4, #12] 
    /*000020c4*/ CMP         R3, #1 
    /*000020c8*/ BEQ         AiSetClkDir_x210c
    /*000020cc*/ MOV         R1, R5 
    /*000020d0*/ BL          0x00000000000020D0          @ AI_DRV_SetClkDir [R_ARM_CALL: .text+0x95b4] 
    /*000020d4*/ MOV         R5, R0 
    /*000020d8*/ LDR         R0, [PC, #192]              @ 0x00000000000021a0 
    /*000020dc*/ BL          0x00000000000020DC          @ osal_up [R_ARM_CALL: +0x0] 
    AiSetClkDir_x20e0:
    /*000020e0*/ MOV         R0, R5 
    /*000020e4*/ ADD         SP, SP, #20 
    /*000020e8*/ POP         {R4, R5, PC} 
    AiSetClkDir_x20ec:
    /*000020ec*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*000020f0*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000020f4*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*000020f8*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000020fc*/ MOVW        R2, #1081                   @ 0x439 
    /*00002100*/ LDR         R1, [PC, #156]              @ 0x00000000000021a4 
    /*00002104*/ BL          0x0000000000002104          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002108*/ B           AiSetClkDir_x20a4
    AiSetClkDir_x210c:
    /*0000210c*/ LDR         R0, [PC, #140]              @ 0x00000000000021a0 
    /*00002110*/ MOVW        R5, #32777                  @ 0x8009 
    /*00002114*/ BL          0x0000000000002114          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002118*/ LDR         R2, [R4, #8] 
    /*0000211c*/ LDR         R1, [R4, #12] 
    /*00002120*/ MOV         R0, #0 
    /*00002124*/ MOVW        R3, #1092                   @ 0x444 
    /*00002128*/ STR         R0, [SP, #12] 
    /*0000212c*/ STR         R2, [SP, #4] 
    /*00002130*/ MOV         R0, #3 
    /*00002134*/ STR         R1, [SP, #8] 
    /*00002138*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5f0] 
    /*0000213c*/ STR         R3, [SP] 
    /*00002140*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5f0] 
    /*00002144*/ MOV         R1, #21 
    /*00002148*/ LDR         R3, [PC, #84]               @ 0x00000000000021a4 
    /*0000214c*/ BL          0x000000000000214C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002150*/ MOVT        R5, #40981                  @ 0xa015 
    /*00002154*/ B           AiSetClkDir_x20e0
    AiSetClkDir_x2158:
    /*00002158*/ MOV         R5, #65024                  @ 0xfe00 
    /*0000215c*/ MOVT        R5, #65535                  @ 0xffff 
    /*00002160*/ B           AiSetClkDir_x20e0
    AiSetClkDir_x2164:
    /*00002164*/ MOVW        R3, #1078                   @ 0x436 
    /*00002168*/ STR         R4, [SP, #4] 
    /*0000216c*/ STR         R3, [SP] 
    /*00002170*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00002174*/ LDR         R3, [PC, #40]               @ 0x00000000000021a4 
    /*00002178*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*0000217c*/ MOV         R1, #21 
    /*00002180*/ MOV         R0, #3 
    /*00002184*/ MOVW        R5, #32769                  @ 0x8001 
    /*00002188*/ BL          0x0000000000002188          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000218c*/ MOVT        R5, #40981                  @ 0xa015 
    /*00002190*/ B           AiSetClkDir_x20e0
    AiSetClkDir_x2194:
    /*00002194*/ MOVW        R5, #32784                  @ 0x8010 
    /*00002198*/ MOVT        R5, #40981                  @ 0xa015 
    /*0000219c*/ B           AiSetClkDir_x20e0
    /*000021a0*/ ANDEQ       R0, R0, IP, ROR R0          @ g_stAiDev [R_ARM_ABS32: .bss+0x8] 
    /*000021a4*/ ANDEQ       R0, R0, R8, ROR #1 
FUNC_END AiSetClkDir


FUNC_BEGIN AIEnableAecRefFrame
    /*000021a8*/ CMP         R0, #0 
    /*000021ac*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000021b0*/ MOV         R4, R2 
    /*000021b4*/ SUB         SP, SP, #24 
    /*000021b8*/ ADD         R2, R1, R0, LSL #1 
    /*000021bc*/ BNE         AIEnableAecRefFrame_x23d4
    /*000021c0*/ CMP         R1, #1 
    /*000021c4*/ MOV         R7, R1 
    /*000021c8*/ BHI         AIEnableAecRefFrame_x2318
    /*000021cc*/ CMP         R4, #0 
    /*000021d0*/ BEQ         AIEnableAecRefFrame_x2404
    /*000021d4*/ LDR         R3, [R4] 
    /*000021d8*/ CMP         R3, #1 
    /*000021dc*/ BHI         AIEnableAecRefFrame_x2348
    /*000021e0*/ LDR         R3, [R4, #4] 
    /*000021e4*/ CMP         R3, #2 
    /*000021e8*/ BHI         AIEnableAecRefFrame_x2378
    /*000021ec*/ MOV         R0, R2 
    /*000021f0*/ BL          0x00000000000021F0          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*000021f4*/ SUBS        R5, R0, #0 
    /*000021f8*/ BEQ         AIEnableAecRefFrame_x22f8
    AIEnableAecRefFrame_x21fc:
    /*000021fc*/ MOV         R0, #0 
    /*00002200*/ BL          0x0000000000002200          @ AI_GetDevCtx [R_ARM_CALL: .text+0x56c4] 
    /*00002204*/ CMP         R0, #0 
    /*00002208*/ BEQ         AIEnableAecRefFrame_x22d8
    AIEnableAecRefFrame_x220c:
    /*0000220c*/ MOV         R0, R5 
    /*00002210*/ BL          0x0000000000002210          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00002214*/ SUBS        R6, R0, #0 
    /*00002218*/ BNE         AIEnableAecRefFrame_x23c8
    /*0000221c*/ LDR         R8, [R5, #208]              @ 0xd0 
    /*00002220*/ CMP         R8, #1 
    /*00002224*/ BNE         AIEnableAecRefFrame_x23a8
    /*00002228*/ LDR         R3, [R5, #216]              @ 0xd8 
    /*0000222c*/ CMN         R3, #1 
    /*00002230*/ BEQ         AIEnableAecRefFrame_x22a4
    /*00002234*/ LDR         R2, [R4] 
    /*00002238*/ MOV         R0, R5 
    /*0000223c*/ CMP         R3, R2 
    /*00002240*/ BNE         AIEnableAecRefFrame_x2264
    /*00002244*/ LDR         R3, [R4, #4] 
    /*00002248*/ LDR         R2, [R5, #220]              @ 0xdc 
    /*0000224c*/ CMP         R2, R3 
    /*00002250*/ BNE         AIEnableAecRefFrame_x2264
    AIEnableAecRefFrame_x2254:
    /*00002254*/ BL          0x0000000000002254          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002258*/ MOV         R0, R6 
    AIEnableAecRefFrame_x225c:
    /*0000225c*/ ADD         SP, SP, #24 
    /*00002260*/ POP         {R4, R5, R6, R7, R8, PC} 
    AIEnableAecRefFrame_x2264:
    /*00002264*/ BL          0x0000000000002264          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002268*/ MOV         R2, #0 
    /*0000226c*/ STR         R2, [SP, #4] 
    /*00002270*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x304] 
    /*00002274*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x304] 
    /*00002278*/ MOVW        R3, #1149                   @ 0x47d 
    /*0000227c*/ STR         R7, [SP, #8] 
    /*00002280*/ STR         R3, [SP] 
    AIEnableAecRefFrame_x2284:
    /*00002284*/ LDR         R3, [PC, #388]              @ 0x0000000000002410 
    /*00002288*/ MOV         R1, #21 
    /*0000228c*/ MOV         R0, #3 
    /*00002290*/ BL          0x0000000000002290          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002294*/ MOVW        R0, #32777                  @ 0x8009 
    /*00002298*/ MOVT        R0, #40981                  @ 0xa015 
    /*0000229c*/ ADD         SP, SP, #24 
    /*000022a0*/ POP         {R4, R5, R6, R7, R8, PC} 
    AIEnableAecRefFrame_x22a4:
    /*000022a4*/ LDR         R0, [PC, #360]              @ 0x0000000000002414 
    /*000022a8*/ ADD         R1, SP, #20 
    /*000022ac*/ BL          0x00000000000022AC          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000022b0*/ LDR         R3, [R4] 
    /*000022b4*/ LDR         R0, [PC, #344]              @ 0x0000000000002414 
    /*000022b8*/ ADD         R1, SP, #20 
    /*000022bc*/ STR         R3, [R5, #216]              @ 0xd8 
    /*000022c0*/ LDR         R3, [R4, #4] 
    /*000022c4*/ STR         R8, [R5, #1100]             @ 0x44c 
    /*000022c8*/ STR         R3, [R5, #220]              @ 0xdc 
    /*000022cc*/ BL          0x00000000000022CC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000022d0*/ MOV         R0, R5 
    /*000022d4*/ B           AIEnableAecRefFrame_x2254
    AIEnableAecRefFrame_x22d8:
    /*000022d8*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*000022dc*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000022e0*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*000022e4*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000022e8*/ MOVW        R2, #1117                   @ 0x45d 
    /*000022ec*/ LDR         R1, [PC, #284]              @ 0x0000000000002410 
    /*000022f0*/ BL          0x00000000000022F0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000022f4*/ B           AIEnableAecRefFrame_x220c
    AIEnableAecRefFrame_x22f8:
    /*000022f8*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*000022fc*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002300*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*00002304*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00002308*/ MOVW        R2, #1115                   @ 0x45b 
    /*0000230c*/ LDR         R1, [PC, #252]              @ 0x0000000000002410 
    /*00002310*/ BL          0x0000000000002310          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002314*/ B           AIEnableAecRefFrame_x21fc
    AIEnableAecRefFrame_x2318:
    /*00002318*/ STR         R1, [SP, #4] 
    /*0000231c*/ MOVW        R3, #1110                   @ 0x456 
    /*00002320*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00002324*/ STR         R3, [SP] 
    /*00002328*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*0000232c*/ LDR         R3, [PC, #220]              @ 0x0000000000002410 
    /*00002330*/ MOV         R1, #20 
    /*00002334*/ MOV         R0, #3 
    /*00002338*/ BL          0x0000000000002338          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000233c*/ MOVW        R0, #32770                  @ 0x8002 
    /*00002340*/ MOVT        R0, #40981                  @ 0xa015 
    /*00002344*/ B           AIEnableAecRefFrame_x225c
    AIEnableAecRefFrame_x2348:
    /*00002348*/ STR         R3, [SP, #4] 
    /*0000234c*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x264] 
    /*00002350*/ MOVW        R3, #1112                   @ 0x458 
    /*00002354*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x264] 
    /*00002358*/ STR         R3, [SP] 
    /*0000235c*/ MOV         R1, #22 
    /*00002360*/ LDR         R3, [PC, #168]              @ 0x0000000000002410 
    /*00002364*/ MOV         R0, #3 
    /*00002368*/ BL          0x0000000000002368          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000236c*/ MOVW        R0, #32769                  @ 0x8001 
    /*00002370*/ MOVT        R0, #40982                  @ 0xa016 
    /*00002374*/ B           AIEnableAecRefFrame_x225c
    AIEnableAecRefFrame_x2378:
    /*00002378*/ STR         R3, [SP, #4] 
    /*0000237c*/ MOVW        R2, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x298] 
    /*00002380*/ MOVW        R3, #1113                   @ 0x459 
    /*00002384*/ MOVT        R2, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x298] 
    /*00002388*/ STR         R3, [SP] 
    /*0000238c*/ MOV         R1, #20 
    /*00002390*/ LDR         R3, [PC, #120]              @ 0x0000000000002410 
    /*00002394*/ MOV         R0, #3 
    /*00002398*/ BL          0x0000000000002398          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000239c*/ MOVW        R0, #32770                  @ 0x8002 
    /*000023a0*/ MOVT        R0, #40982                  @ 0xa016 
    /*000023a4*/ B           AIEnableAecRefFrame_x225c
    AIEnableAecRefFrame_x23a8:
    /*000023a8*/ MOV         R0, R5 
    /*000023ac*/ BL          0x00000000000023AC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000023b0*/ MOVW        R3, #1133                   @ 0x46d 
    /*000023b4*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2cc] 
    /*000023b8*/ STMIB       SP, {R6, R7} 
    /*000023bc*/ STR         R3, [SP] 
    /*000023c0*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2cc] 
    /*000023c4*/ B           AIEnableAecRefFrame_x2284
    AIEnableAecRefFrame_x23c8:
    /*000023c8*/ MOV         R0, #65024                  @ 0xfe00 
    /*000023cc*/ MOVT        R0, #65535                  @ 0xffff 
    /*000023d0*/ B           AIEnableAecRefFrame_x225c
    AIEnableAecRefFrame_x23d4:
    /*000023d4*/ STR         R0, [SP, #4] 
    /*000023d8*/ MOVW        R3, #1109                   @ 0x455 
    /*000023dc*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*000023e0*/ STR         R3, [SP] 
    /*000023e4*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*000023e8*/ LDR         R3, [PC, #32]               @ 0x0000000000002410 
    /*000023ec*/ MOV         R1, #21 
    /*000023f0*/ MOV         R0, #3 
    /*000023f4*/ BL          0x00000000000023F4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000023f8*/ MOVW        R0, #32769                  @ 0x8001 
    /*000023fc*/ MOVT        R0, #40981                  @ 0xa015 
    /*00002400*/ B           AIEnableAecRefFrame_x225c
    AIEnableAecRefFrame_x2404:
    /*00002404*/ MOVW        R0, #32774                  @ 0x8006 
    /*00002408*/ MOVT        R0, #40981                  @ 0xa015 
    /*0000240c*/ B           AIEnableAecRefFrame_x225c
    /*00002410*/ STRDEQ      R0, [R0], -R4 
    /*00002414*/ ANDEQ       R0, R0, R0, LSL #1          @ g_stAiDev [R_ARM_ABS32: .bss+0x8] 
FUNC_END AIEnableAecRefFrame


FUNC_BEGIN AIDisableAecRefFrame
    /*00002418*/ CMP         R0, #0 
    /*0000241c*/ ADD         R3, R1, R0, LSL #1 
    /*00002420*/ PUSH        {R4, R5, LR} 
    /*00002424*/ SUB         SP, SP, #20 
    /*00002428*/ BNE         AIDisableAecRefFrame_x24cc
    /*0000242c*/ CMP         R1, #1 
    /*00002430*/ BHI         AIDisableAecRefFrame_x247c
    /*00002434*/ MOV         R0, R3 
    /*00002438*/ BL          0x0000000000002438          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*0000243c*/ SUBS        R5, R0, #0 
    /*00002440*/ BEQ         AIDisableAecRefFrame_x24ac
    AIDisableAecRefFrame_x2444:
    /*00002444*/ LDR         R0, [PC, #176]              @ 0x00000000000024fc 
    /*00002448*/ ADD         R1, SP, #12 
    /*0000244c*/ BL          0x000000000000244C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00002450*/ MOV         R4, #0 
    /*00002454*/ MVN         R3, #0 
    /*00002458*/ LDR         R0, [PC, #156]              @ 0x00000000000024fc 
    /*0000245c*/ ADD         R1, SP, #12 
    /*00002460*/ STR         R4, [R5, #1100]             @ 0x44c 
    /*00002464*/ STR         R3, [R5, #216]              @ 0xd8 
    /*00002468*/ STR         R3, [R5, #220]              @ 0xdc 
    /*0000246c*/ BL          0x000000000000246C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002470*/ MOV         R0, R4 
    AIDisableAecRefFrame_x2474:
    /*00002474*/ ADD         SP, SP, #20 
    /*00002478*/ POP         {R4, R5, PC} 
    AIDisableAecRefFrame_x247c:
    /*0000247c*/ STR         R1, [SP, #4] 
    /*00002480*/ MOVW        R3, #1173                   @ 0x495 
    /*00002484*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00002488*/ STR         R3, [SP] 
    /*0000248c*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*00002490*/ LDR         R3, [PC, #104]              @ 0x0000000000002500 
    /*00002494*/ MOV         R1, #20 
    /*00002498*/ MOV         R0, #3 
    /*0000249c*/ BL          0x000000000000249C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000024a0*/ MOVW        R0, #32770                  @ 0x8002 
    /*000024a4*/ MOVT        R0, #40981                  @ 0xa015 
    /*000024a8*/ B           AIDisableAecRefFrame_x2474
    AIDisableAecRefFrame_x24ac:
    /*000024ac*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*000024b0*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000024b4*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*000024b8*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000024bc*/ MOVW        R2, #1176                   @ 0x498 
    /*000024c0*/ LDR         R1, [PC, #56]               @ 0x0000000000002500 
    /*000024c4*/ BL          0x00000000000024C4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000024c8*/ B           AIDisableAecRefFrame_x2444
    AIDisableAecRefFrame_x24cc:
    /*000024cc*/ STR         R0, [SP, #4] 
    /*000024d0*/ MOVW        R3, #1172                   @ 0x494 
    /*000024d4*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*000024d8*/ STR         R3, [SP] 
    /*000024dc*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*000024e0*/ LDR         R3, [PC, #24]               @ 0x0000000000002500 
    /*000024e4*/ MOV         R1, #21 
    /*000024e8*/ MOV         R0, #3 
    /*000024ec*/ BL          0x00000000000024EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000024f0*/ MOVW        R0, #32769                  @ 0x8001 
    /*000024f4*/ MOVT        R0, #40981                  @ 0xa015 
    /*000024f8*/ B           AIDisableAecRefFrame_x2474
    /*000024fc*/ ANDEQ       R0, R0, R0, LSL #1          @ g_stAiDev [R_ARM_ABS32: .bss+0x8] 
    /*00002500*/ ANDEQ       R0, R0, R8, LSL #2 
FUNC_END AIDisableAecRefFrame


FUNC_BEGIN AISetAcodecGain
    AISetAcodecGain_x2504:
    /*00002504*/ B           AISetAcodecGain_x2504       @ AI_DRV_SetAcodecGain [R_ARM_JUMP24: .text+0x9698] 
FUNC_END AISetAcodecGain


FUNC_BEGIN AIGetAcodecGain
    /*00002508*/ CMP         R0, #0 
    /*0000250c*/ BEQ         AIGetAcodecGain_x2514
    AIGetAcodecGain_x2510:
    /*00002510*/ B           AIGetAcodecGain_x2510       @ AI_DRV_GetAcodecGain [R_ARM_JUMP24: .text+0x972c] 
    AIGetAcodecGain_x2514:
    /*00002514*/ MOVW        R0, #32774                  @ 0x8006 
    /*00002518*/ MOVT        R0, #40981                  @ 0xa015 
    /*0000251c*/ BX          LR 
FUNC_END AIGetAcodecGain


FUNC_BEGIN AiGetClkDir
    /*00002520*/ PUSH        {R4, R5, LR} 
    /*00002524*/ MOV         R4, R0 
    /*00002528*/ SUB         SP, SP, #12 
    /*0000252c*/ MOV         R0, #20 
    /*00002530*/ MOV         R5, R1 
    /*00002534*/ BL          0x0000000000002534          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002538*/ CMP         R0, #0 
    /*0000253c*/ BEQ         AiGetClkDir_x263c
    /*00002540*/ CMP         R4, #0 
    /*00002544*/ BNE         AiGetClkDir_x260c
    /*00002548*/ CMP         R5, #0 
    /*0000254c*/ BEQ         AiGetClkDir_x2648
    /*00002550*/ MOVW        R4, #0                      @ g_stAiDev [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*00002554*/ MOVT        R4, #0                      @ g_stAiDev [R_ARM_MOVT_ABS: .bss+0x8] 
    /*00002558*/ CMP         R4, #0 
    /*0000255c*/ BEQ         AiGetClkDir_x25a8
    AiGetClkDir_x2560:
    /*00002560*/ LDR         R0, [PC, #236]              @ 0x0000000000002654 
    /*00002564*/ BL          0x0000000000002564          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00002568*/ CMP         R0, #0 
    /*0000256c*/ BNE         AiGetClkDir_x2600
    /*00002570*/ LDR         R3, [R4, #8] 
    /*00002574*/ CMP         R3, #0 
    /*00002578*/ BEQ         AiGetClkDir_x25c8
    /*0000257c*/ LDR         R3, [R4, #12] 
    /*00002580*/ CMP         R3, #1 
    /*00002584*/ BEQ         AiGetClkDir_x25c8
    /*00002588*/ MOV         R1, R5 
    /*0000258c*/ BL          0x000000000000258C          @ AI_DRV_GetClkDir [R_ARM_CALL: .text+0x9630] 
    /*00002590*/ MOV         R4, R0 
    /*00002594*/ LDR         R0, [PC, #184]              @ 0x0000000000002654 
    /*00002598*/ BL          0x0000000000002598          @ osal_up [R_ARM_CALL: +0x0] 
    AiGetClkDir_x259c:
    /*0000259c*/ MOV         R0, R4 
    /*000025a0*/ ADD         SP, SP, #12 
    /*000025a4*/ POP         {R4, R5, PC} 
    AiGetClkDir_x25a8:
    /*000025a8*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*000025ac*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000025b0*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*000025b4*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000025b8*/ MOVW        R2, #1209                   @ 0x4b9 
    /*000025bc*/ LDR         R1, [PC, #148]              @ 0x0000000000002658 
    /*000025c0*/ BL          0x00000000000025C0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000025c4*/ B           AiGetClkDir_x2560
    AiGetClkDir_x25c8:
    /*000025c8*/ LDR         R0, [PC, #132]              @ 0x0000000000002654 
    /*000025cc*/ MOVW        R4, #32777                  @ 0x8009 
    /*000025d0*/ BL          0x00000000000025D0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000025d4*/ MOVW        R2, #1220                   @ 0x4c4 
    /*000025d8*/ MOV         R3, #0 
    /*000025dc*/ MOV         R1, #21 
    /*000025e0*/ STM         SP, {R2, R3} 
    /*000025e4*/ MOV         R0, #3 
    /*000025e8*/ MOVW        R2, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x648] 
    /*000025ec*/ LDR         R3, [PC, #100]              @ 0x0000000000002658 
    /*000025f0*/ MOVT        R2, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x648] 
    /*000025f4*/ MOVT        R4, #40981                  @ 0xa015 
    /*000025f8*/ BL          0x00000000000025F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000025fc*/ B           AiGetClkDir_x259c
    AiGetClkDir_x2600:
    /*00002600*/ MOV         R4, #65024                  @ 0xfe00 
    /*00002604*/ MOVT        R4, #65535                  @ 0xffff 
    /*00002608*/ B           AiGetClkDir_x259c
    AiGetClkDir_x260c:
    /*0000260c*/ STR         R4, [SP, #4] 
    /*00002610*/ MOVW        R3, #1205                   @ 0x4b5 
    /*00002614*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00002618*/ STR         R3, [SP] 
    /*0000261c*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*00002620*/ LDR         R3, [PC, #48]               @ 0x0000000000002658 
    /*00002624*/ MOV         R1, #21 
    /*00002628*/ MOV         R0, #3 
    /*0000262c*/ MOVW        R4, #32769                  @ 0x8001 
    /*00002630*/ BL          0x0000000000002630          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002634*/ MOVT        R4, #40981                  @ 0xa015 
    /*00002638*/ B           AiGetClkDir_x259c
    AiGetClkDir_x263c:
    /*0000263c*/ MOVW        R4, #32784                  @ 0x8010 
    /*00002640*/ MOVT        R4, #40981                  @ 0xa015 
    /*00002644*/ B           AiGetClkDir_x259c
    AiGetClkDir_x2648:
    /*00002648*/ MOVW        R4, #32774                  @ 0x8006 
    /*0000264c*/ MOVT        R4, #40981                  @ 0xa015 
    /*00002650*/ B           AiGetClkDir_x259c
    /*00002654*/ ANDEQ       R0, R0, IP, ROR R0          @ g_stAiDev [R_ARM_ABS32: .bss+0x8] 
    /*00002658*/ ANDEQ       R0, R0, R0, LSR #2 
FUNC_END AiGetClkDir


FUNC_BEGIN AiSetSaveFile
    /*0000265c*/ CMP         R0, #0 
    /*00002660*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00002664*/ SUB         SP, SP, #32 
    /*00002668*/ BNE         AiSetSaveFile_x29a4
    /*0000266c*/ CMP         R1, #1 
    /*00002670*/ MOV         R7, R1 
    /*00002674*/ BHI         AiSetSaveFile_x284c
    /*00002678*/ CMP         R2, #0 
    /*0000267c*/ MOV         R4, R2 
    /*00002680*/ BEQ         AiSetSaveFile_x2a58
    /*00002684*/ MOVW        R3, #0                      @ g_stAiDev [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*00002688*/ MOVT        R3, #0                      @ g_stAiDev [R_ARM_MOVT_ABS: .bss+0x8] 
    /*0000268c*/ CMP         R3, #0 
    /*00002690*/ BEQ         AiSetSaveFile_x280c
    AiSetSaveFile_x2694:
    /*00002694*/ MOV         R0, R7 
    /*00002698*/ BL          0x0000000000002698          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*0000269c*/ SUBS        R5, R0, #0 
    /*000026a0*/ BEQ         AiSetSaveFile_x282c
    AiSetSaveFile_x26a4:
    /*000026a4*/ MOV         R0, R5 
    /*000026a8*/ BL          0x00000000000026A8          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000026ac*/ CMP         R0, #0 
    /*000026b0*/ BNE         AiSetSaveFile_x28f4
    /*000026b4*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*000026b8*/ CMP         R3, #1 
    /*000026bc*/ BNE         AiSetSaveFile_x28bc
    /*000026c0*/ LDR         R3, [R4] 
    /*000026c4*/ CMP         R3, #1 
    /*000026c8*/ BHI         AiSetSaveFile_x287c
    /*000026cc*/ LDR         R6, [R5, #580]              @ 0x244 
    /*000026d0*/ CMP         R6, #0 
    /*000026d4*/ MOVEQ       R6, #0 
    /*000026d8*/ ANDNE       R6, R3, #1 
    /*000026dc*/ CMP         R6, #0 
    /*000026e0*/ BNE         AiSetSaveFile_x2a64
    /*000026e4*/ LDRB        R3, [R4, #4] 
    /*000026e8*/ CMP         R3, #0 
    /*000026ec*/ BEQ         AiSetSaveFile_x2900
    /*000026f0*/ ADD         R8, R4, #4 
    /*000026f4*/ MOV         R1, #256                    @ 0x100 
    /*000026f8*/ MOV         R0, R8 
    /*000026fc*/ BL          0x00000000000026FC          @ osal_strnlen [R_ARM_CALL: +0x0] 
    /*00002700*/ CMP         R0, #255                    @ 0xff 
    /*00002704*/ BGT         AiSetSaveFile_x29d4
    /*00002708*/ LDRB        R3, [R4, #260]              @ 0x104 
    /*0000270c*/ CMP         R3, #0 
    /*00002710*/ BEQ         AiSetSaveFile_x2938
    /*00002714*/ ADD         R8, R4, #260                @ 0x104 
    /*00002718*/ MOV         R1, #256                    @ 0x100 
    /*0000271c*/ MOV         R0, R8 
    /*00002720*/ BL          0x0000000000002720          @ osal_strnlen [R_ARM_CALL: +0x0] 
    /*00002724*/ CMP         R0, #255                    @ 0xff 
    /*00002728*/ BGT         AiSetSaveFile_x2a24
    /*0000272c*/ LDR         R3, [R4, #516]              @ 0x204 
    /*00002730*/ SUB         R3, R3, #1 
    /*00002734*/ CMP         R3, #10240                  @ 0x2800 
    /*00002738*/ BCS         AiSetSaveFile_x2958
    /*0000273c*/ MOV         R0, #772                    @ 0x304 
    /*00002740*/ STR         R6, [SP, #24] 
    /*00002744*/ MOV         R3, #21 
    /*00002748*/ STR         R7, [SP, #28] 
    /*0000274c*/ STR         R3, [SP, #20] 
    /*00002750*/ BL          0x0000000000002750          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*00002754*/ SUBS        R6, R0, #0 
    /*00002758*/ BEQ         AiSetSaveFile_x2a9c
    /*0000275c*/ MOV         R2, #772                    @ 0x304 
    /*00002760*/ MOV         R1, #0 
    /*00002764*/ MOV         R0, R6 
    /*00002768*/ BL          0x0000000000002768          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000276c*/ MOV         R0, #2 
    /*00002770*/ BL          0x0000000000002770          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002774*/ MOV         R1, R6 
    /*00002778*/ LDR         R3, [R0, #48]               @ 0x30 
    /*0000277c*/ ADD         R0, SP, #20 
    /*00002780*/ BLX         R3 
    /*00002784*/ CMP         R0, #0 
    /*00002788*/ BEQ         AiSetSaveFile_x27c4
    AiSetSaveFile_x278c:
    /*0000278c*/ LDR         R3, [R5, #240]              @ 0xf0 
    /*00002790*/ CMP         R3, #0 
    /*00002794*/ BEQ         AiSetSaveFile_x2980
    /*00002798*/ MOV         R1, R4 
    /*0000279c*/ MOV         R2, #520                    @ 0x208 
    /*000027a0*/ ADD         R0, R5, #580                @ 0x244 
    /*000027a4*/ BL          0x00000000000027A4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000027a8*/ MOV         R0, R5 
    /*000027ac*/ BL          0x00000000000027AC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000027b0*/ MOV         R0, R6 
    /*000027b4*/ BL          0x00000000000027B4          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*000027b8*/ MOV         R0, #0 
    AiSetSaveFile_x27bc:
    /*000027bc*/ ADD         SP, SP, #32 
    /*000027c0*/ POP         {R4, R5, R6, R7, R8, PC} 
    AiSetSaveFile_x27c4:
    /*000027c4*/ LDR         R3, [R6] 
    /*000027c8*/ CMP         R3, #0 
    /*000027cc*/ BEQ         AiSetSaveFile_x278c
    /*000027d0*/ MOV         R0, R5 
    /*000027d4*/ BL          0x00000000000027D4          @ osal_up [R_ARM_CALL: +0x0] 
    /*000027d8*/ MOV         R0, R6 
    /*000027dc*/ BL          0x00000000000027DC          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*000027e0*/ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x924] 
    /*000027e4*/ MOVT        R2, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x924] 
    /*000027e8*/ MOVW        R3, #1337                   @ 0x539 
    /*000027ec*/ STR         R3, [SP] 
    AiSetSaveFile_x27f0:
    /*000027f0*/ LDR         R3, [PC, #732]              @ 0x0000000000002ad4 
    /*000027f4*/ MOV         R1, #21 
    /*000027f8*/ MOV         R0, #3 
    /*000027fc*/ BL          0x00000000000027FC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002800*/ MOVW        R0, #32777                  @ 0x8009 
    /*00002804*/ MOVT        R0, #40981                  @ 0xa015 
    /*00002808*/ B           AiSetSaveFile_x27bc
    AiSetSaveFile_x280c:
    /*0000280c*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c] 
    /*00002810*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002814*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c] 
    /*00002818*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*0000281c*/ MOV         R2, #1248                   @ 0x4e0 
    /*00002820*/ LDR         R1, [PC, #684]              @ 0x0000000000002ad4 
    /*00002824*/ BL          0x0000000000002824          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002828*/ B           AiSetSaveFile_x2694
    AiSetSaveFile_x282c:
    /*0000282c*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*00002830*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002834*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*00002838*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*0000283c*/ MOVW        R2, #1252                   @ 0x4e4 
    /*00002840*/ LDR         R1, [PC, #652]              @ 0x0000000000002ad4 
    /*00002844*/ BL          0x0000000000002844          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002848*/ B           AiSetSaveFile_x26a4
    AiSetSaveFile_x284c:
    /*0000284c*/ STR         R1, [SP, #4] 
    /*00002850*/ MOVW        R3, #1244                   @ 0x4dc 
    /*00002854*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00002858*/ STR         R3, [SP] 
    /*0000285c*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*00002860*/ LDR         R3, [PC, #620]              @ 0x0000000000002ad4 
    /*00002864*/ MOV         R1, #20 
    /*00002868*/ MOV         R0, #3 
    /*0000286c*/ BL          0x000000000000286C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002870*/ MOVW        R0, #32770                  @ 0x8002 
    /*00002874*/ MOVT        R0, #40981                  @ 0xa015 
    /*00002878*/ B           AiSetSaveFile_x27bc
    AiSetSaveFile_x287c:
    /*0000287c*/ MOV         R0, R5 
    /*00002880*/ BL          0x0000000000002880          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002884*/ LDR         R3, [R4] 
    /*00002888*/ MOVW        R2, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6c0] 
    /*0000288c*/ MOVT        R2, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6c0] 
    /*00002890*/ MOVW        R1, #1270                   @ 0x4f6 
    /*00002894*/ STR         R7, [SP, #4] 
    /*00002898*/ STR         R1, [SP] 
    /*0000289c*/ STR         R3, [SP, #8] 
    AiSetSaveFile_x28a0:
    /*000028a0*/ LDR         R3, [PC, #556]              @ 0x0000000000002ad4 
    /*000028a4*/ MOV         R1, #21 
    /*000028a8*/ MOV         R0, #3 
    /*000028ac*/ BL          0x00000000000028AC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000028b0*/ MOVW        R0, #32771                  @ 0x8003 
    /*000028b4*/ MOVT        R0, #40981                  @ 0xa015 
    /*000028b8*/ B           AiSetSaveFile_x27bc
    AiSetSaveFile_x28bc:
    /*000028bc*/ MOV         R0, R5 
    /*000028c0*/ BL          0x00000000000028C0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000028c4*/ MOVW        R3, #1263                   @ 0x4ef 
    /*000028c8*/ STR         R7, [SP, #4] 
    /*000028cc*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x68c] 
    /*000028d0*/ STR         R3, [SP] 
    /*000028d4*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x68c] 
    /*000028d8*/ LDR         R3, [PC, #500]              @ 0x0000000000002ad4 
    /*000028dc*/ MOV         R1, #21 
    /*000028e0*/ MOV         R0, #3 
    /*000028e4*/ BL          0x00000000000028E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000028e8*/ MOVW        R0, #32773                  @ 0x8005 
    /*000028ec*/ MOVT        R0, #40981                  @ 0xa015 
    /*000028f0*/ B           AiSetSaveFile_x27bc
    AiSetSaveFile_x28f4:
    /*000028f4*/ MOV         R0, #65024                  @ 0xfe00 
    /*000028f8*/ MOVT        R0, #65535                  @ 0xffff 
    /*000028fc*/ B           AiSetSaveFile_x27bc
    AiSetSaveFile_x2900:
    /*00002900*/ MOV         R0, R5 
    /*00002904*/ BL          0x0000000000002904          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002908*/ MOVW        R2, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x758] 
    /*0000290c*/ MOVT        R2, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x758] 
    /*00002910*/ MOVW        R3, #1284                   @ 0x504 
    /*00002914*/ STR         R7, [SP, #4] 
    /*00002918*/ STR         R3, [SP] 
    AiSetSaveFile_x291c:
    /*0000291c*/ LDR         R3, [PC, #432]              @ 0x0000000000002ad4 
    /*00002920*/ MOV         R1, #21 
    /*00002924*/ MOV         R0, #3 
    /*00002928*/ BL          0x0000000000002928          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000292c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00002930*/ MOVT        R0, #40981                  @ 0xa015 
    /*00002934*/ B           AiSetSaveFile_x27bc
    AiSetSaveFile_x2938:
    /*00002938*/ MOV         R0, R5 
    /*0000293c*/ BL          0x000000000000293C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002940*/ MOVW        R3, #1299                   @ 0x513 
    /*00002944*/ MOVW        R2, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7f0] 
    /*00002948*/ STR         R7, [SP, #4] 
    /*0000294c*/ STR         R3, [SP] 
    /*00002950*/ MOVT        R2, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7f0] 
    /*00002954*/ B           AiSetSaveFile_x291c
    AiSetSaveFile_x2958:
    /*00002958*/ MOV         R0, R5 
    /*0000295c*/ BL          0x000000000000295C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002960*/ LDR         R3, [R4, #516]              @ 0x204 
    /*00002964*/ MOVW        R1, #1314                   @ 0x522 
    /*00002968*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x888] 
    /*0000296c*/ STR         R7, [SP, #4] 
    /*00002970*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x888] 
    /*00002974*/ STR         R1, [SP] 
    /*00002978*/ STR         R3, [SP, #8] 
    /*0000297c*/ B           AiSetSaveFile_x28a0
    AiSetSaveFile_x2980:
    /*00002980*/ MOV         R0, R5 
    /*00002984*/ BL          0x0000000000002984          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002988*/ MOV         R0, R6 
    /*0000298c*/ BL          0x000000000000298C          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*00002990*/ MOVW        R3, #1347                   @ 0x543 
    /*00002994*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x964] 
    /*00002998*/ STR         R3, [SP] 
    /*0000299c*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x964] 
    /*000029a0*/ B           AiSetSaveFile_x27f0
    AiSetSaveFile_x29a4:
    /*000029a4*/ STR         R0, [SP, #4] 
    /*000029a8*/ MOVW        R3, #1243                   @ 0x4db 
    /*000029ac*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*000029b0*/ STR         R3, [SP] 
    /*000029b4*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*000029b8*/ LDR         R3, [PC, #276]              @ 0x0000000000002ad4 
    /*000029bc*/ MOV         R1, #21 
    /*000029c0*/ MOV         R0, #3 
    /*000029c4*/ BL          0x00000000000029C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000029c8*/ MOVW        R0, #32769                  @ 0x8001 
    /*000029cc*/ MOVT        R0, #40981                  @ 0xa015 
    /*000029d0*/ B           AiSetSaveFile_x27bc
    AiSetSaveFile_x29d4:
    /*000029d4*/ MOV         R0, R5 
    /*000029d8*/ BL          0x00000000000029D8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000029dc*/ MOV         R0, R8 
    /*000029e0*/ MOV         R1, #256                    @ 0x100 
    /*000029e4*/ BL          0x00000000000029E4          @ osal_strnlen [R_ARM_CALL: +0x0] 
    /*000029e8*/ MOV         R2, #256                    @ 0x100 
    /*000029ec*/ STR         R2, [SP, #12] 
    /*000029f0*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x79c] 
    /*000029f4*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x79c] 
    /*000029f8*/ MOVW        R3, #1292                   @ 0x50c 
    /*000029fc*/ STR         R7, [SP, #4] 
    /*00002a00*/ STR         R3, [SP] 
    AiSetSaveFile_x2a04:
    /*00002a04*/ STR         R0, [SP, #8] 
    /*00002a08*/ MOV         R1, #21 
    /*00002a0c*/ LDR         R3, [PC, #192]              @ 0x0000000000002ad4 
    /*00002a10*/ MOV         R0, #3 
    /*00002a14*/ BL          0x0000000000002A14          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002a18*/ MOVW        R0, #32771                  @ 0x8003 
    /*00002a1c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00002a20*/ B           AiSetSaveFile_x27bc
    AiSetSaveFile_x2a24:
    /*00002a24*/ MOV         R0, R5 
    /*00002a28*/ BL          0x0000000000002A28          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002a2c*/ MOV         R0, R8 
    /*00002a30*/ MOV         R1, #256                    @ 0x100 
    /*00002a34*/ BL          0x0000000000002A34          @ osal_strnlen [R_ARM_CALL: +0x0] 
    /*00002a38*/ MOV         R2, #256                    @ 0x100 
    /*00002a3c*/ MOVW        R3, #1307                   @ 0x51b 
    /*00002a40*/ STR         R2, [SP, #12] 
    /*00002a44*/ STR         R7, [SP, #4] 
    /*00002a48*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x834] 
    /*00002a4c*/ STR         R3, [SP] 
    /*00002a50*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x834] 
    /*00002a54*/ B           AiSetSaveFile_x2a04
    AiSetSaveFile_x2a58:
    /*00002a58*/ MOVW        R0, #32774                  @ 0x8006 
    /*00002a5c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00002a60*/ B           AiSetSaveFile_x27bc
    AiSetSaveFile_x2a64:
    /*00002a64*/ MOV         R0, R5 
    /*00002a68*/ BL          0x0000000000002A68          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002a6c*/ MOVW        R3, #1277                   @ 0x4fd 
    /*00002a70*/ STR         R7, [SP, #4] 
    /*00002a74*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x714] 
    /*00002a78*/ STR         R3, [SP] 
    /*00002a7c*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x714] 
    /*00002a80*/ LDR         R3, [PC, #76]               @ 0x0000000000002ad4 
    /*00002a84*/ MOV         R1, #21 
    /*00002a88*/ MOV         R0, #3 
    /*00002a8c*/ BL          0x0000000000002A8C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002a90*/ MOVW        R0, #32777                  @ 0x8009 
    /*00002a94*/ MOVT        R0, #40981                  @ 0xa015 
    /*00002a98*/ B           AiSetSaveFile_x27bc
    AiSetSaveFile_x2a9c:
    /*00002a9c*/ MOV         R0, R5 
    /*00002aa0*/ BL          0x0000000000002AA0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002aa4*/ MOVW        R3, #1326                   @ 0x52e 
    /*00002aa8*/ STR         R7, [SP, #4] 
    /*00002aac*/ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8e8] 
    /*00002ab0*/ STR         R3, [SP] 
    /*00002ab4*/ MOVT        R2, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8e8] 
    /*00002ab8*/ LDR         R3, [PC, #20]               @ 0x0000000000002ad4 
    /*00002abc*/ MOV         R1, #21 
    /*00002ac0*/ MOV         R0, #3 
    /*00002ac4*/ BL          0x0000000000002AC4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002ac8*/ MOVW        R0, #32780                  @ 0x800c 
    /*00002acc*/ MOVT        R0, #40981                  @ 0xa015 
    /*00002ad0*/ B           AiSetSaveFile_x27bc
    /*00002ad4*/ ANDEQ       R0, R0, IP, LSR #2 
FUNC_END AiSetSaveFile


FUNC_BEGIN AiGetSaveFile
    /*00002ad8*/ CMP         R0, #0 
    /*00002adc*/ PUSH        {R4, R5, R6, LR} 
    /*00002ae0*/ SUB         SP, SP, #8 
    /*00002ae4*/ BNE         AiGetSaveFile_x2b9c
    /*00002ae8*/ CMP         R1, #1 
    /*00002aec*/ BHI         AiGetSaveFile_x2b6c
    /*00002af0*/ CMP         R2, #0 
    /*00002af4*/ MOV         R6, R2 
    /*00002af8*/ BEQ         AiGetSaveFile_x2bcc
    /*00002afc*/ MOV         R0, R1 
    /*00002b00*/ BL          0x0000000000002B00          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*00002b04*/ SUBS        R5, R0, #0 
    /*00002b08*/ BEQ         AiGetSaveFile_x2b4c
    AiGetSaveFile_x2b0c:
    /*00002b0c*/ MOV         R0, R5 
    /*00002b10*/ BL          0x0000000000002B10          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00002b14*/ SUBS        R4, R0, #0 
    /*00002b18*/ BNE         AiGetSaveFile_x2b40
    /*00002b1c*/ MOV         R2, #520                    @ 0x208 
    /*00002b20*/ ADD         R1, R5, #580                @ 0x244 
    /*00002b24*/ MOV         R0, R6 
    /*00002b28*/ BL          0x0000000000002B28          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002b2c*/ MOV         R0, R5 
    /*00002b30*/ BL          0x0000000000002B30          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002b34*/ MOV         R0, R4 
    AiGetSaveFile_x2b38:
    /*00002b38*/ ADD         SP, SP, #8 
    /*00002b3c*/ POP         {R4, R5, R6, PC} 
    AiGetSaveFile_x2b40:
    /*00002b40*/ MOV         R0, #65024                  @ 0xfe00 
    /*00002b44*/ MOVT        R0, #65535                  @ 0xffff 
    /*00002b48*/ B           AiGetSaveFile_x2b38
    AiGetSaveFile_x2b4c:
    /*00002b4c*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21c] 
    /*00002b50*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002b54*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21c] 
    /*00002b58*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00002b5c*/ MOVW        R2, #1371                   @ 0x55b 
    /*00002b60*/ LDR         R1, [PC, #112]              @ 0x0000000000002bd8 
    /*00002b64*/ BL          0x0000000000002B64          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002b68*/ B           AiGetSaveFile_x2b0c
    AiGetSaveFile_x2b6c:
    /*00002b6c*/ STR         R1, [SP, #4] 
    /*00002b70*/ MOVW        R3, #1366                   @ 0x556 
    /*00002b74*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x138] 
    /*00002b78*/ STR         R3, [SP] 
    /*00002b7c*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x138] 
    /*00002b80*/ LDR         R3, [PC, #80]               @ 0x0000000000002bd8 
    /*00002b84*/ MOV         R1, #20 
    /*00002b88*/ MOV         R0, #3 
    /*00002b8c*/ BL          0x0000000000002B8C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002b90*/ MOVW        R0, #32770                  @ 0x8002 
    /*00002b94*/ MOVT        R0, #40981                  @ 0xa015 
    /*00002b98*/ B           AiGetSaveFile_x2b38
    AiGetSaveFile_x2b9c:
    /*00002b9c*/ STR         R0, [SP, #4] 
    /*00002ba0*/ MOVW        R3, #1365                   @ 0x555 
    /*00002ba4*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x104] 
    /*00002ba8*/ STR         R3, [SP] 
    /*00002bac*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x104] 
    /*00002bb0*/ LDR         R3, [PC, #32]               @ 0x0000000000002bd8 
    /*00002bb4*/ MOV         R1, #21 
    /*00002bb8*/ MOV         R0, #3 
    /*00002bbc*/ BL          0x0000000000002BBC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002bc0*/ MOVW        R0, #32769                  @ 0x8001 
    /*00002bc4*/ MOVT        R0, #40981                  @ 0xa015 
    /*00002bc8*/ B           AiGetSaveFile_x2b38
    AiGetSaveFile_x2bcc:
    /*00002bcc*/ MOVW        R0, #32774                  @ 0x8006 
    /*00002bd0*/ MOVT        R0, #40981                  @ 0xa015 
    /*00002bd4*/ B           AiGetSaveFile_x2b38
    /*00002bd8*/ ANDEQ       R0, R0, IP, LSR R1 
FUNC_END AiGetSaveFile


FUNC_BEGIN AiIoctl
    /*00002bdc*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00002be0*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00002be4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00002be8*/ SUB         SP, SP, #44                 @ 0x2c 
    /*00002bec*/ LDR         R4, [R2] 
    /*00002bf0*/ LDR         R8, [R3] 
    /*00002bf4*/ CMP         R4, #0 
    /*00002bf8*/ AND         R5, R4, #1 
    /*00002bfc*/ RSBLT       R5, R5, #0 
    /*00002c00*/ CMP         R8, #0 
    /*00002c04*/ ADD         R4, R4, R4, LSR #31 
    /*00002c08*/ BNE         AiIoctl_x2da4
    /*00002c0c*/ MOV         R6, R0 
    /*00002c10*/ MOVW        R0, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002c14*/ MOVT        R0, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002c18*/ MOV         R9, R2 
    /*00002c1c*/ MOV         R7, R1 
    /*00002c20*/ BL          0x0000000000002C20          @ osal_atomic_inc_return [R_ARM_CALL: +0x0] 
    /*00002c24*/ MOVW        R2, #23068                  @ 0x5a1c 
    /*00002c28*/ MOVT        R2, #16392                  @ 0x4008 
    /*00002c2c*/ CMP         R6, R2 
    /*00002c30*/ ASR         R4, R4, #1 
    /*00002c34*/ BEQ         AiIoctl_x32bc
    /*00002c38*/ BLS         AiIoctl_x2cac
    /*00002c3c*/ MOVW        R3, #23062                  @ 0x5a16 
    /*00002c40*/ MOVT        R3, #32772                  @ 0x8004 
    /*00002c44*/ CMP         R6, R3 
    /*00002c48*/ BEQ         AiIoctl_x3294
    /*00002c4c*/ BHI         AiIoctl_x2d10
    /*00002c50*/ MOVW        R3, #23059                  @ 0x5a13 
    /*00002c54*/ MOVT        R3, #16704                  @ 0x4140 
    /*00002c58*/ CMP         R6, R3 
    /*00002c5c*/ BEQ         AiIoctl_x30c8
    /*00002c60*/ BHI         AiIoctl_x3034
    /*00002c64*/ MOVW        R3, #23041                  @ 0x5a01 
    /*00002c68*/ MOVT        R3, #16424                  @ 0x4028 
    /*00002c6c*/ CMP         R6, R3 
    /*00002c70*/ BEQ         AiIoctl_x31f4
    /*00002c74*/ MOVW        R3, #23049                  @ 0x5a09 
    /*00002c78*/ MOVT        R3, #16512                  @ 0x4080 
    /*00002c7c*/ CMP         R6, R3 
    /*00002c80*/ BEQ         AiIoctl_x30f8
    /*00002c84*/ MOVW        R3, #23058                  @ 0x5a12 
    /*00002c88*/ MOVT        R3, #16400                  @ 0x4010 
    /*00002c8c*/ CMP         R6, R3 
    /*00002c90*/ BNE         AiIoctl_x3064
    /*00002c94*/ MOV         R2, R7 
    /*00002c98*/ MOV         R1, R5 
    /*00002c9c*/ MOV         R0, R4 
    /*00002ca0*/ BL          0x0000000000002CA0          @ AISetChnResmpInfo [R_ARM_CALL: .text+0x1b78] 
    /*00002ca4*/ MOV         R6, R0 
    /*00002ca8*/ B           AiIoctl_x30b0
    AiIoctl_x2cac:
    /*00002cac*/ MOVW        R3, #23069                  @ 0x5a1d 
    /*00002cb0*/ CMP         R6, R3 
    /*00002cb4*/ BEQ         AiIoctl_x32a8
    /*00002cb8*/ BLS         AiIoctl_x2d68
    /*00002cbc*/ MOVW        R3, #23061                  @ 0x5a15 
    /*00002cc0*/ MOVT        R3, #16388                  @ 0x4004 
    /*00002cc4*/ CMP         R6, R3 
    /*00002cc8*/ BEQ         AiIoctl_x3110
    /*00002ccc*/ BHI         AiIoctl_x2fe8
    /*00002cd0*/ MOVW        R3, #23047                  @ 0x5a07 
    /*00002cd4*/ MOVT        R3, #16388                  @ 0x4004 
    /*00002cd8*/ CMP         R6, R3 
    /*00002cdc*/ BEQ         AiIoctl_x3208
    /*00002ce0*/ MOVW        R3, #23060                  @ 0x5a14 
    /*00002ce4*/ MOVT        R3, #16388                  @ 0x4004 
    /*00002ce8*/ CMP         R6, R3 
    /*00002cec*/ BEQ         AiIoctl_x3168
    /*00002cf0*/ MOV         R3, #23040                  @ 0x5a00 
    /*00002cf4*/ MOVT        R3, #16388                  @ 0x4004 
    /*00002cf8*/ CMP         R6, R3 
    /*00002cfc*/ BNE         AiIoctl_x3064
    /*00002d00*/ LDR         R3, [R7] 
    /*00002d04*/ MOV         R6, #0 
    /*00002d08*/ STR         R3, [R9] 
    /*00002d0c*/ B           AiIoctl_x30b0
    AiIoctl_x2d10:
    /*00002d10*/ MOVW        R3, #23067                  @ 0x5a1b 
    /*00002d14*/ MOVT        R3, #33288                  @ 0x8208 
    /*00002d18*/ CMP         R6, R3 
    /*00002d1c*/ BEQ         AiIoctl_x30e0
    /*00002d20*/ BHI         AiIoctl_x2db4
    /*00002d24*/ MOVW        R3, #23071                  @ 0x5a1f 
    /*00002d28*/ MOVT        R3, #32772                  @ 0x8004 
    /*00002d2c*/ CMP         R6, R3 
    /*00002d30*/ BEQ         AiIoctl_x327c
    /*00002d34*/ MOVW        R3, #23042                  @ 0x5a02 
    /*00002d38*/ MOVT        R3, #32808                  @ 0x8028 
    /*00002d3c*/ CMP         R6, R3 
    /*00002d40*/ BEQ         AiIoctl_x3154
    /*00002d44*/ MOVW        R3, #23064                  @ 0x5a18 
    /*00002d48*/ MOVT        R3, #32772                  @ 0x8004 
    /*00002d4c*/ CMP         R6, R3 
    /*00002d50*/ BNE         AiIoctl_x3064
    /*00002d54*/ MOV         R1, R7 
    /*00002d58*/ MOV         R0, R4 
    /*00002d5c*/ BL          0x0000000000002D5C          @ AiGetClkDir [R_ARM_CALL: .text+0x2520] 
    /*00002d60*/ MOV         R6, R0 
    /*00002d64*/ B           AiIoctl_x30b0
    AiIoctl_x2d68:
    /*00002d68*/ MOVW        R3, #23051                  @ 0x5a0b 
    /*00002d6c*/ CMP         R6, R3 
    /*00002d70*/ BEQ         AiIoctl_x3268
    /*00002d74*/ BLS         AiIoctl_x308c
    /*00002d78*/ MOVW        R3, #23053                  @ 0x5a0d 
    /*00002d7c*/ CMP         R6, R3 
    /*00002d80*/ BEQ         AiIoctl_x31c8
    /*00002d84*/ BCC         AiIoctl_x31b4
    /*00002d88*/ MOVW        R3, #23065                  @ 0x5a19 
    /*00002d8c*/ CMP         R6, R3 
    /*00002d90*/ BNE         AiIoctl_x3064
    /*00002d94*/ MOV         R0, R4 
    /*00002d98*/ BL          0x0000000000002D98          @ AiClrDevAttr [R_ARM_CALL: .text+0x5710] 
    /*00002d9c*/ MOV         R6, R0 
    /*00002da0*/ B           AiIoctl_x30b0
    AiIoctl_x2da4:
    /*00002da4*/ MOVW        R0, #32784                  @ 0x8010 
    /*00002da8*/ MOVT        R0, #40981                  @ 0xa015 
    AiIoctl_x2dac:
    /*00002dac*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00002db0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AiIoctl_x2db4:
    /*00002db4*/ MOVW        R3, #23048                  @ 0x5a08 
    /*00002db8*/ MOVT        R3, #49288                  @ 0xc088 
    /*00002dbc*/ CMP         R6, R3 
    /*00002dc0*/ BEQ         AiIoctl_x324c
    /*00002dc4*/ MOVW        R3, #23073                  @ 0x5a21 
    /*00002dc8*/ MOVT        R3, #49288                  @ 0xc088 
    /*00002dcc*/ CMP         R6, R3 
    /*00002dd0*/ BEQ         AiIoctl_x3198
    /*00002dd4*/ MOVW        R3, #23072                  @ 0x5a20 
    /*00002dd8*/ MOVT        R3, #49280                  @ 0xc080 
    /*00002ddc*/ CMP         R6, R3 
    /*00002de0*/ BNE         AiIoctl_x3064
    /*00002de4*/ CMP         R7, #0 
    /*00002de8*/ BEQ         AiIoctl_x32d4
    /*00002dec*/ ADD         R8, R5, R4, LSL #1 
    /*00002df0*/ MOV         R0, R8 
    /*00002df4*/ BL          0x0000000000002DF4          @ AI_GetChnCtx [R_ARM_CALL: .text+0x56a8] 
    /*00002df8*/ CMP         R0, #0 
    /*00002dfc*/ BEQ         AiIoctl_x3420
    /*00002e00*/ LDR         R3, [R0, #208]              @ 0xd0 
    /*00002e04*/ CMP         R3, #1 
    /*00002e08*/ BNE         AiIoctl_x3420
    /*00002e0c*/ LDR         R6, [R7, #44]               @ 0x2c 
    /*00002e10*/ CMP         R6, #8192                   @ 0x2000 
    /*00002e14*/ BHI         AiIoctl_x3590
    /*00002e18*/ LDR         R9, [R7, #16] 
    /*00002e1c*/ MOV         R1, R6 
    /*00002e20*/ MOV         R0, R9 
    /*00002e24*/ BL          0x0000000000002E24          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00002e28*/ CMP         R0, #0 
    /*00002e2c*/ BNE         AiIoctl_x32e0
    AiIoctl_x2e30:
    /*00002e30*/ MOV         R3, #136                    @ 0x88 
    /*00002e34*/ MOVW        R6, #0                      @ g_stAiDev [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*00002e38*/ MUL         R3, R3, R4 
    /*00002e3c*/ MOVT        R6, #0                      @ g_stAiDev [R_ARM_MOVT_ABS: .bss+0x8] 
    /*00002e40*/ LDR         R2, [R7, #48]               @ 0x30 
    /*00002e44*/ LDR         R3, [R6, R3] 
    /*00002e48*/ CMP         R2, R3 
    /*00002e4c*/ BNE         AiIoctl_x34ec
    /*00002e50*/ MOV         R0, #1 
    /*00002e54*/ BL          0x0000000000002E54          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002e58*/ LDRD        R2, [R7, #16] 
    /*00002e5c*/ LDR         R1, [R0, #84]               @ 0x54 
    /*00002e60*/ LDR         R0, [R7, #48]               @ 0x30 
    /*00002e64*/ BLX         R1 
    /*00002e68*/ CMP         R0, #1 
    /*00002e6c*/ BNE         AiIoctl_x35c0
    /*00002e70*/ LDR         R3, [R7, #4] 
    /*00002e74*/ CMP         R3, #1 
    /*00002e78*/ BEQ         AiIoctl_x3470
    AiIoctl_x2e7c:
    /*00002e7c*/ LDR         R3, [R7, #112]              @ 0x70 
    /*00002e80*/ CMP         R3, #1 
    /*00002e84*/ BEQ         AiIoctl_x3328
    AiIoctl_x2e88:
    /*00002e88*/ MOV         R3, #136                    @ 0x88 
    /*00002e8c*/ MOV         R9, #1120                   @ 0x460 
    /*00002e90*/ MLA         R6, R3, R4, R6 
    /*00002e94*/ MOVW        SL, #0                      @ g_stAiChn [R_ARM_MOVW_ABS_NC: .bss+0x90] 
    /*00002e98*/ MUL         R9, R9, R8 
    /*00002e9c*/ MOVT        SL, #0                      @ g_stAiChn [R_ARM_MOVT_ABS: .bss+0x90] 
    /*00002ea0*/ ADD         R2, R6, #128                @ 0x80 
    /*00002ea4*/ ADD         R1, SP, #36                 @ 0x24 
    /*00002ea8*/ STR         R2, [SP, #20] 
    /*00002eac*/ ADD         R8, R9, #180                @ 0xb4 
    /*00002eb0*/ ADD         R3, SL, R9 
    /*00002eb4*/ ADD         R8, SL, R8 
    /*00002eb8*/ MOV         R0, R2 
    /*00002ebc*/ LDR         R6, [R3, #148]              @ 0x94 
    /*00002ec0*/ BL          0x0000000000002EC0          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00002ec4*/ MOV         R0, R8 
    /*00002ec8*/ BL          0x0000000000002EC8          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*00002ecc*/ CMP         R6, R0 
    /*00002ed0*/ BHI         AiIoctl_x2f34
    /*00002ed4*/ RSB         R6, R6, #2 
    /*00002ed8*/ ADD         R6, R6, R0 
    /*00002edc*/ CMP         R6, #0 
    /*00002ee0*/ SUB         R6, R6, #1 
    /*00002ee4*/ BEQ         AiIoctl_x2f34
    /*00002ee8*/ MOVW        R3, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcd8] 
    /*00002eec*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002ef0*/ MOVT        R3, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcd8] 
    /*00002ef4*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00002ef8*/ STR         R3, [SP, #24] 
    /*00002efc*/ STR         R2, [SP, #28] 
    AiIoctl_x2f00:
    /*00002f00*/ MOV         R0, R8 
    /*00002f04*/ SUB         R6, R6, #1 
    /*00002f08*/ BL          0x0000000000002F08          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x805c] 
    /*00002f0c*/ SUBS        FP, R0, #0 
    /*00002f10*/ BEQ         AiIoctl_x342c
    AiIoctl_x2f14:
    /*00002f14*/ MOV         R1, #14 
    /*00002f18*/ ADD         R0, FP, #8 
    /*00002f1c*/ BL          0x0000000000002F1C          @ AISubVb [R_ARM_CALL: .text+0x3a50] 
    /*00002f20*/ MOV         R1, FP 
    /*00002f24*/ MOV         R0, R8 
    /*00002f28*/ BL          0x0000000000002F28          @ AUDIO_FB_PutFree [R_ARM_CALL: .text+0x80bc] 
    /*00002f2c*/ CMN         R6, #1 
    /*00002f30*/ BNE         AiIoctl_x2f00
    AiIoctl_x2f34:
    /*00002f34*/ MOV         R0, R8 
    /*00002f38*/ BL          0x0000000000002F38          @ AUDIO_FB_GetFree [R_ARM_CALL: .text+0x7ffc] 
    /*00002f3c*/ SUBS        FP, R0, #0 
    /*00002f40*/ BEQ         AiIoctl_x35e8
    AiIoctl_x2f44:
    /*00002f44*/ LDRD        R0, [R7, #16] 
    /*00002f48*/ BL          0x0000000000000014 
    /*00002f4c*/ LDR         R3, [R7, #4] 
    /*00002f50*/ CMP         R3, #1 
    /*00002f54*/ STR         R0, [R7, #8] 
    /*00002f58*/ BEQ         AiIoctl_x3554
    AiIoctl_x2f5c:
    /*00002f5c*/ LDR         R3, [R7, #112]              @ 0x70 
    /*00002f60*/ CMP         R3, #1 
    /*00002f64*/ BEQ         AiIoctl_x352c
    AiIoctl_x2f68:
    /*00002f68*/ MOV         R2, #128                    @ 0x80 
    /*00002f6c*/ MOV         R1, R7 
    /*00002f70*/ ADD         R0, FP, #8 
    /*00002f74*/ MOV         R6, #0 
    /*00002f78*/ BL          0x0000000000002F78          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002f7c*/ MOV         R1, FP 
    /*00002f80*/ MOV         R0, R8 
    /*00002f84*/ BL          0x0000000000002F84          @ AUDIO_FB_PutBusy [R_ARM_CALL: .text+0x810c] 
    /*00002f88*/ MOV         R1, #14 
    /*00002f8c*/ MOV         R0, R7 
    /*00002f90*/ BL          0x0000000000002F90          @ AIAddVb [R_ARM_CALL: .text+0x3958] 
    /*00002f94*/ MOV         R1, #12 
    /*00002f98*/ MOV         R0, R7 
    /*00002f9c*/ BL          0x0000000000002F9C          @ AISubVb [R_ARM_CALL: .text+0x3a50] 
    /*00002fa0*/ MOV         R0, #2 
    /*00002fa4*/ BL          0x0000000000002FA4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002fa8*/ MOV         IP, #5 
    /*00002fac*/ STR         R7, [SP, #4] 
    /*00002fb0*/ MOV         R2, R5 
    /*00002fb4*/ STR         IP, [SP] 
    /*00002fb8*/ MOV         R1, R4 
    /*00002fbc*/ MOV         R3, R6 
    /*00002fc0*/ LDR         R4, [R0, #40]               @ 0x28 
    /*00002fc4*/ MOV         R0, #21 
    /*00002fc8*/ BLX         R4 
    /*00002fcc*/ LDR         R0, [SP, #20] 
    /*00002fd0*/ ADD         R1, SP, #36                 @ 0x24 
    /*00002fd4*/ BL          0x0000000000002FD4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002fd8*/ ADD         R0, R9, #8 
    /*00002fdc*/ ADD         R0, SL, R0 
    /*00002fe0*/ BL          0x0000000000002FE0          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00002fe4*/ B           AiIoctl_x30b0
    AiIoctl_x2fe8:
    /*00002fe8*/ MOVW        R3, #23070                  @ 0x5a1e 
    /*00002fec*/ MOVT        R3, #16388                  @ 0x4004 
    /*00002ff0*/ CMP         R6, R3 
    /*00002ff4*/ BEQ         AiIoctl_x31dc
    /*00002ff8*/ MOVW        R3, #23050                  @ 0x5a0a 
    /*00002ffc*/ MOVT        R3, #16392                  @ 0x4008 
    /*00003000*/ CMP         R6, R3 
    /*00003004*/ BEQ         AiIoctl_x3180
    /*00003008*/ MOVW        R3, #23063                  @ 0x5a17 
    /*0000300c*/ MOVT        R3, #16388                  @ 0x4004 
    /*00003010*/ CMP         R6, R3 
    /*00003014*/ BNE         AiIoctl_x3064
    /*00003018*/ CMP         R7, #0 
    /*0000301c*/ BEQ         AiIoctl_x3520
    /*00003020*/ LDR         R1, [R7] 
    /*00003024*/ MOV         R0, R4 
    /*00003028*/ BL          0x0000000000003028          @ AiSetClkDir [R_ARM_CALL: .text+0x206c] 
    /*0000302c*/ MOV         R6, R0 
    /*00003030*/ B           AiIoctl_x30b0
    AiIoctl_x3034:
    /*00003034*/ MOVW        R3, #23045                  @ 0x5a05 
    /*00003038*/ MOVT        R3, #32772                  @ 0x8004 
    /*0000303c*/ CMP         R6, R3 
    /*00003040*/ BEQ         AiIoctl_x3238
    /*00003044*/ MOVW        R3, #23046                  @ 0x5a06 
    /*00003048*/ MOVT        R3, #32772                  @ 0x8004 
    /*0000304c*/ CMP         R6, R3 
    /*00003050*/ BEQ         AiIoctl_x313c
    /*00003054*/ MOVW        R3, #23066                  @ 0x5a1a 
    /*00003058*/ MOVT        R3, #16904                  @ 0x4208 
    /*0000305c*/ CMP         R6, R3 
    /*00003060*/ BEQ         AiIoctl_x3220
    AiIoctl_x3064:
    /*00003064*/ MOVW        R3, #1586                   @ 0x632 
    /*00003068*/ MOVW        R2, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xce8] 
    /*0000306c*/ STR         R3, [SP] 
    /*00003070*/ MOVT        R2, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0xce8] 
    /*00003074*/ LDR         R3, [PC, #1612]             @ 0x00000000000036c8 
    /*00003078*/ MOV         R1, #21 
    /*0000307c*/ MOV         R0, #6 
    /*00003080*/ MVN         R6, #0 
    /*00003084*/ BL          0x0000000000003084          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003088*/ B           AiIoctl_x30b0
    AiIoctl_x308c:
    /*0000308c*/ MOVW        R3, #23043                  @ 0x5a03 
    /*00003090*/ CMP         R6, R3 
    /*00003094*/ BEQ         AiIoctl_x312c
    /*00003098*/ MOVW        R3, #23044                  @ 0x5a04 
    /*0000309c*/ CMP         R6, R3 
    /*000030a0*/ BNE         AiIoctl_x3064
    /*000030a4*/ MOV         R0, R4 
    /*000030a8*/ BL          0x00000000000030A8          @ AiDisableDev [R_ARM_CALL: .text+0x508c] 
    /*000030ac*/ MOV         R6, R0 
    AiIoctl_x30b0:
    /*000030b0*/ MOVW        R0, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000030b4*/ MOVT        R0, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000030b8*/ BL          0x00000000000030B8          @ osal_atomic_dec_return [R_ARM_CALL: +0x0] 
    /*000030bc*/ MOV         R0, R6 
    /*000030c0*/ ADD         SP, SP, #44                 @ 0x2c 
    /*000030c4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AiIoctl_x30c8:
    /*000030c8*/ MOV         R2, R7 
    /*000030cc*/ MOV         R1, R5 
    /*000030d0*/ MOV         R0, R4 
    /*000030d4*/ BL          0x00000000000030D4          @ AISetChnVqeInfo [R_ARM_CALL: .text+0x1c68] 
    /*000030d8*/ MOV         R6, R0 
    /*000030dc*/ B           AiIoctl_x30b0
    AiIoctl_x30e0:
    /*000030e0*/ MOV         R2, R7 
    /*000030e4*/ MOV         R1, R5 
    /*000030e8*/ MOV         R0, R4 
    /*000030ec*/ BL          0x00000000000030EC          @ AiGetSaveFile [R_ARM_CALL: .text+0x2ad8] 
    /*000030f0*/ MOV         R6, R0 
    /*000030f4*/ B           AiIoctl_x30b0
    AiIoctl_x30f8:
    /*000030f8*/ MOV         R2, R7 
    /*000030fc*/ MOV         R1, R5 
    /*00003100*/ MOV         R0, R4 
    /*00003104*/ BL          0x0000000000003104          @ AIReleaseFrame [R_ARM_CALL: .text+0x1874] 
    /*00003108*/ MOV         R6, R0 
    /*0000310c*/ B           AiIoctl_x30b0
    AiIoctl_x3110:
    /*00003110*/ CMP         R7, #0 
    /*00003114*/ BEQ         AiIoctl_x3520
    /*00003118*/ LDR         R1, [R7] 
    /*0000311c*/ MOV         R0, R4 
    /*00003120*/ BL          0x0000000000003120          @ AiSetTrackMode [R_ARM_CALL: .text+0x1e20] 
    /*00003124*/ MOV         R6, R0 
    /*00003128*/ B           AiIoctl_x30b0
    AiIoctl_x312c:
    /*0000312c*/ MOV         R0, R4 
    /*00003130*/ BL          0x0000000000003130          @ AiEnableDev [R_ARM_CALL: .text+0x4c18] 
    /*00003134*/ MOV         R6, R0 
    /*00003138*/ B           AiIoctl_x30b0
    AiIoctl_x313c:
    /*0000313c*/ MOV         R2, R7 
    /*00003140*/ MOV         R1, R5 
    /*00003144*/ MOV         R0, R4 
    /*00003148*/ BL          0x0000000000003148          @ AIGetChnParam [R_ARM_CALL: .text+0xe3c] 
    /*0000314c*/ MOV         R6, R0 
    /*00003150*/ B           AiIoctl_x30b0
    AiIoctl_x3154:
    /*00003154*/ MOV         R1, R7 
    /*00003158*/ MOV         R0, R4 
    /*0000315c*/ BL          0x000000000000315C          @ AiGetDevAttr [R_ARM_CALL: .text+0x4acc] 
    /*00003160*/ MOV         R6, R0 
    /*00003164*/ B           AiIoctl_x30b0
    AiIoctl_x3168:
    /*00003168*/ MOV         R2, R7 
    /*0000316c*/ MOV         R1, R5 
    /*00003170*/ MOV         R0, R4 
    /*00003174*/ BL          0x0000000000003174          @ AISetChnVqeVolInfo [R_ARM_CALL: .text+0x1d48] 
    /*00003178*/ MOV         R6, R0 
    /*0000317c*/ B           AiIoctl_x30b0
    AiIoctl_x3180:
    /*00003180*/ MOV         R2, R7 
    /*00003184*/ MOV         R1, R5 
    /*00003188*/ MOV         R0, R4 
    /*0000318c*/ BL          0x000000000000318C          @ AIEnableVqe [R_ARM_CALL: .text+0xa28] 
    /*00003190*/ MOV         R6, R0 
    /*00003194*/ B           AiIoctl_x30b0
    AiIoctl_x3198:
    /*00003198*/ MOV         R2, R7 
    /*0000319c*/ MOV         R1, R5 
    /*000031a0*/ MOV         R0, R4 
    /*000031a4*/ MOV         R3, #1 
    /*000031a8*/ BL          0x00000000000031A8          @ AIGetFrame [R_ARM_CALL: .text+0x12d4] 
    /*000031ac*/ MOV         R6, R0 
    /*000031b0*/ B           AiIoctl_x30b0
    AiIoctl_x31b4:
    /*000031b4*/ MOV         R1, R5 
    /*000031b8*/ MOV         R0, R4 
    /*000031bc*/ BL          0x00000000000031BC          @ AIEnableChn [R_ARM_CALL: .text+0x46c] 
    /*000031c0*/ MOV         R6, R0 
    /*000031c4*/ B           AiIoctl_x30b0
    AiIoctl_x31c8:
    /*000031c8*/ MOV         R1, R5 
    /*000031cc*/ MOV         R0, R4 
    /*000031d0*/ BL          0x00000000000031D0          @ AIDisableChn [R_ARM_CALL: .text+0x710] 
    /*000031d4*/ MOV         R6, R0 
    /*000031d8*/ B           AiIoctl_x30b0
    AiIoctl_x31dc:
    /*000031dc*/ CMP         R7, #0 
    /*000031e0*/ BEQ         AiIoctl_x3520
    /*000031e4*/ LDR         R0, [R7] 
    /*000031e8*/ BL          0x00000000000031E8          @ AI_DRV_SetAcodecGain [R_ARM_CALL: .text+0x9698] 
    /*000031ec*/ MOV         R6, R0 
    /*000031f0*/ B           AiIoctl_x30b0
    AiIoctl_x31f4:
    /*000031f4*/ MOV         R1, R7 
    /*000031f8*/ MOV         R0, R4 
    /*000031fc*/ BL          0x00000000000031FC          @ AiSetDevAttr [R_ARM_CALL: .text+0x4968] 
    /*00003200*/ MOV         R6, R0 
    /*00003204*/ B           AiIoctl_x30b0
    AiIoctl_x3208:
    /*00003208*/ MOV         R2, R7 
    /*0000320c*/ MOV         R1, R5 
    /*00003210*/ MOV         R0, R4 
    /*00003214*/ BL          0x0000000000003214          @ AISetChnParam [R_ARM_CALL: .text+0x100c] 
    /*00003218*/ MOV         R6, R0 
    /*0000321c*/ B           AiIoctl_x30b0
    AiIoctl_x3220:
    /*00003220*/ MOV         R2, R7 
    /*00003224*/ MOV         R1, R5 
    /*00003228*/ MOV         R0, R4 
    /*0000322c*/ BL          0x000000000000322C          @ AiSetSaveFile [R_ARM_CALL: .text+0x265c] 
    /*00003230*/ MOV         R6, R0 
    /*00003234*/ B           AiIoctl_x30b0
    AiIoctl_x3238:
    /*00003238*/ MOV         R1, R7 
    /*0000323c*/ MOV         R0, R4 
    /*00003240*/ BL          0x0000000000003240          @ AIGetPoolId [R_ARM_CALL: .text+0xda0] 
    /*00003244*/ MOV         R6, R0 
    /*00003248*/ B           AiIoctl_x30b0
    AiIoctl_x324c:
    /*0000324c*/ MOV         R3, R8 
    /*00003250*/ MOV         R2, R7 
    /*00003254*/ MOV         R1, R5 
    /*00003258*/ MOV         R0, R4 
    /*0000325c*/ BL          0x000000000000325C          @ AIGetFrame [R_ARM_CALL: .text+0x12d4] 
    /*00003260*/ MOV         R6, R0 
    /*00003264*/ B           AiIoctl_x30b0
    AiIoctl_x3268:
    /*00003268*/ MOV         R1, R5 
    /*0000326c*/ MOV         R0, R4 
    /*00003270*/ BL          0x0000000000003270          @ AIDisableVqe [R_ARM_CALL: .text+0xcb4] 
    /*00003274*/ MOV         R6, R0 
    /*00003278*/ B           AiIoctl_x30b0
    AiIoctl_x327c:
    /*0000327c*/ CMP         R7, #0 
    /*00003280*/ BEQ         AiIoctl_x32d4
    /*00003284*/ MOV         R0, R7 
    /*00003288*/ BL          0x0000000000003288          @ AI_DRV_GetAcodecGain [R_ARM_CALL: .text+0x972c] 
    /*0000328c*/ MOV         R6, R0 
    /*00003290*/ B           AiIoctl_x30b0
    AiIoctl_x3294:
    /*00003294*/ MOV         R1, R7 
    /*00003298*/ MOV         R0, R4 
    /*0000329c*/ BL          0x000000000000329C          @ AiGetTrackMode [R_ARM_CALL: .text+0x1f3c] 
    /*000032a0*/ MOV         R6, R0 
    /*000032a4*/ B           AiIoctl_x30b0
    AiIoctl_x32a8:
    /*000032a8*/ MOV         R1, R5 
    /*000032ac*/ MOV         R0, R4 
    /*000032b0*/ BL          0x00000000000032B0          @ AIDisableAecRefFrame [R_ARM_CALL: .text+0x2418] 
    /*000032b4*/ MOV         R6, R0 
    /*000032b8*/ B           AiIoctl_x30b0
    AiIoctl_x32bc:
    /*000032bc*/ MOV         R2, R7 
    /*000032c0*/ MOV         R1, R5 
    /*000032c4*/ MOV         R0, R4 
    /*000032c8*/ BL          0x00000000000032C8          @ AIEnableAecRefFrame [R_ARM_CALL: .text+0x21a8] 
    /*000032cc*/ MOV         R6, R0 
    /*000032d0*/ B           AiIoctl_x30b0
    AiIoctl_x32d4:
    /*000032d4*/ MOVW        R6, #32774                  @ 0x8006 
    /*000032d8*/ MOVT        R6, #40981                  @ 0xa015 
    /*000032dc*/ B           AiIoctl_x30b0
    AiIoctl_x32e0:
    /*000032e0*/ MOV         R1, R6 
    /*000032e4*/ MOV         R0, R9 
    /*000032e8*/ BL          0x00000000000032E8          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*000032ec*/ CMP         R0, #0 
    /*000032f0*/ BEQ         AiIoctl_x2e30
    /*000032f4*/ MOVW        R2, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9ec] 
    /*000032f8*/ MOVT        R2, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9ec] 
    /*000032fc*/ LDRD        R0, [R7, #16] 
    /*00003300*/ MOVW        R3, #513                    @ 0x201 
    /*00003304*/ STR         R3, [SP] 
    AiIoctl_x3308:
    /*00003308*/ STRD        R0, [SP, #8] 
    /*0000330c*/ MOV         R1, #21 
    /*00003310*/ LDR         R3, [PC, #948]              @ 0x00000000000036cc 
    /*00003314*/ MOV         R0, #3 
    /*00003318*/ MOVW        R6, #32771                  @ 0x8003 
    /*0000331c*/ BL          0x000000000000331C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003320*/ MOVT        R6, #40981                  @ 0xa015 
    /*00003324*/ B           AiIoctl_x30b0
    AiIoctl_x3328:
    /*00003328*/ LDR         R9, [R7, #100]              @ 0x64 
    /*0000332c*/ CMP         R9, #8192                   @ 0x2000 
    /*00003330*/ BHI         AiIoctl_x3650
    /*00003334*/ LDR         SL, [R7, #72]               @ 0x48 
    /*00003338*/ MOV         R1, R9 
    /*0000333c*/ MOV         R0, SL 
    /*00003340*/ BL          0x0000000000003340          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00003344*/ CMP         R0, #0 
    /*00003348*/ BNE         AiIoctl_x3444
    AiIoctl_x334c:
    /*0000334c*/ MOV         R3, #136                    @ 0x88 
    /*00003350*/ LDR         R2, [R7, #104]              @ 0x68 
    /*00003354*/ MUL         R3, R3, R4 
    /*00003358*/ LDR         R3, [R6, R3] 
    /*0000335c*/ CMP         R2, R3 
    /*00003360*/ BNE         AiIoctl_x3608
    /*00003364*/ MOV         R0, #1 
    /*00003368*/ BL          0x0000000000003368          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000336c*/ LDRD        R2, [R7, #72]               @ 0x48 
    /*00003370*/ LDR         R1, [R0, #84]               @ 0x54 
    /*00003374*/ LDR         R0, [R7, #104]              @ 0x68 
    /*00003378*/ BLX         R1 
    /*0000337c*/ CMP         R0, #1 
    /*00003380*/ BNE         AiIoctl_x3684
    /*00003384*/ LDR         R3, [R7, #4] 
    /*00003388*/ CMP         R3, #1 
    /*0000338c*/ BNE         AiIoctl_x2e88
    /*00003390*/ LDR         SL, [R7, #100]              @ 0x64 
    /*00003394*/ LDR         R9, [R7, #80]               @ 0x50 
    /*00003398*/ MOV         R1, SL 
    /*0000339c*/ MOV         R0, R9 
    /*000033a0*/ BL          0x00000000000033A0          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*000033a4*/ CMP         R0, #0 
    /*000033a8*/ BNE         AiIoctl_x3624
    AiIoctl_x33ac:
    /*000033ac*/ MOV         R3, #136                    @ 0x88 
    /*000033b0*/ LDR         R2, [R7, #108]              @ 0x6c 
    /*000033b4*/ MUL         R3, R3, R4 
    /*000033b8*/ LDR         R3, [R6, R3] 
    /*000033bc*/ CMP         R2, R3 
    /*000033c0*/ BNE         AiIoctl_x36ac
    /*000033c4*/ MOV         R0, #1 
    /*000033c8*/ BL          0x00000000000033C8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000033cc*/ LDRD        R2, [R7, #80]               @ 0x50 
    /*000033d0*/ LDR         R1, [R0, #84]               @ 0x54 
    /*000033d4*/ LDR         R0, [R7, #108]              @ 0x6c 
    /*000033d8*/ BLX         R1 
    /*000033dc*/ CMP         R0, #1 
    /*000033e0*/ BEQ         AiIoctl_x2e88
    /*000033e4*/ LDRD        R4, [R7, #80]               @ 0x50 
    /*000033e8*/ MOVW        R2, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc34] 
    /*000033ec*/ LDR         R3, [PC, #728]              @ 0x00000000000036cc 
    /*000033f0*/ MOVT        R2, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc34] 
    /*000033f4*/ MOV         LR, #596                    @ 0x254 
    /*000033f8*/ MOV         R1, #21 
    /*000033fc*/ STRD        R4, [SP, #8] 
    /*00003400*/ MOV         R0, #3 
    /*00003404*/ LDR         IP, [R7, #108]              @ 0x6c 
    AiIoctl_x3408:
    /*00003408*/ STR         LR, [SP] 
    /*0000340c*/ MOVW        R6, #32771                  @ 0x8003 
    /*00003410*/ STR         IP, [SP, #4] 
    /*00003414*/ MOVT        R6, #40981                  @ 0xa015 
    /*00003418*/ BL          0x0000000000003418          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000341c*/ B           AiIoctl_x30b0
    AiIoctl_x3420:
    /*00003420*/ MOVW        R6, #32770                  @ 0x8002 
    /*00003424*/ MOVT        R6, #40981                  @ 0xa015 
    /*00003428*/ B           AiIoctl_x30b0
    AiIoctl_x342c:
    /*0000342c*/ LDR         R3, [SP, #24] 
    /*00003430*/ MOV         R2, #656                    @ 0x290 
    /*00003434*/ LDR         R1, [PC, #660]              @ 0x00000000000036d0 
    /*00003438*/ LDR         R0, [SP, #28] 
    /*0000343c*/ BL          0x000000000000343C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003440*/ B           AiIoctl_x2f14
    AiIoctl_x3444:
    /*00003444*/ MOV         R1, R9 
    /*00003448*/ MOV         R0, SL 
    /*0000344c*/ BL          0x000000000000344C          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*00003450*/ CMP         R0, #0 
    /*00003454*/ BEQ         AiIoctl_x334c
    /*00003458*/ LDRD        R0, [R7, #72]               @ 0x48 
    /*0000345c*/ MOVW        R3, #562                    @ 0x232 
    /*00003460*/ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xba0] 
    /*00003464*/ STR         R3, [SP] 
    /*00003468*/ MOVT        R2, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0xba0] 
    /*0000346c*/ B           AiIoctl_x3308
    AiIoctl_x3470:
    /*00003470*/ LDR         SL, [R7, #44]               @ 0x2c 
    /*00003474*/ LDR         R9, [R7, #24] 
    /*00003478*/ MOV         R1, SL 
    /*0000347c*/ MOV         R0, R9 
    /*00003480*/ BL          0x0000000000003480          @ hil_is_phys_in_mmz [R_ARM_CALL: +0x0] 
    /*00003484*/ CMP         R0, #0 
    /*00003488*/ BNE         AiIoctl_x3564
    AiIoctl_x348c:
    /*0000348c*/ MOV         R3, #136                    @ 0x88 
    /*00003490*/ LDR         R2, [R7, #52]               @ 0x34 
    /*00003494*/ MUL         R3, R3, R4 
    /*00003498*/ LDR         R3, [R6, R3] 
    /*0000349c*/ CMP         R2, R3 
    /*000034a0*/ BNE         AiIoctl_x3668
    /*000034a4*/ MOV         R0, #1 
    /*000034a8*/ BL          0x00000000000034A8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000034ac*/ LDRD        R2, [R7, #24] 
    /*000034b0*/ LDR         R1, [R0, #84]               @ 0x54 
    /*000034b4*/ LDR         R0, [R7, #52]               @ 0x34 
    /*000034b8*/ BLX         R1 
    /*000034bc*/ CMP         R0, #1 
    /*000034c0*/ BEQ         AiIoctl_x2e7c
    /*000034c4*/ LDRD        R4, [R7, #24] 
    /*000034c8*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa78] 
    /*000034cc*/ MOVW        LR, #547                    @ 0x223 
    /*000034d0*/ LDR         R3, [PC, #500]              @ 0x00000000000036cc 
    /*000034d4*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa78] 
    /*000034d8*/ MOV         R1, #21 
    /*000034dc*/ STRD        R4, [SP, #8] 
    /*000034e0*/ MOV         R0, #3 
    /*000034e4*/ LDR         IP, [R7, #52]               @ 0x34 
    /*000034e8*/ B           AiIoctl_x3408
    AiIoctl_x34ec:
    /*000034ec*/ STR         R2, [SP, #4] 
    /*000034f0*/ MOVW        R2, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa30] 
    /*000034f4*/ MOVT        R2, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa30] 
    /*000034f8*/ STR         R3, [SP, #8] 
    /*000034fc*/ MOVW        R3, #519                    @ 0x207 
    /*00003500*/ STR         R3, [SP] 
    AiIoctl_x3504:
    /*00003504*/ LDR         R3, [PC, #448]              @ 0x00000000000036cc 
    /*00003508*/ MOV         R1, #21 
    /*0000350c*/ MOV         R0, #3 
    /*00003510*/ MOVW        R6, #32771                  @ 0x8003 
    /*00003514*/ BL          0x0000000000003514          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003518*/ MOVT        R6, #40981                  @ 0xa015 
    /*0000351c*/ B           AiIoctl_x30b0
    AiIoctl_x3520:
    /*00003520*/ MOVW        R0, #32774                  @ 0x8006 
    /*00003524*/ MOVT        R0, #40981                  @ 0xa015 
    /*00003528*/ B           AiIoctl_x2dac
    AiIoctl_x352c:
    /*0000352c*/ LDRD        R0, [R7, #72]               @ 0x48 
    /*00003530*/ BL          0x0000000000000014 
    /*00003534*/ LDR         R3, [R7, #4] 
    /*00003538*/ CMP         R3, #1 
    /*0000353c*/ STR         R0, [R7, #64]               @ 0x40 
    /*00003540*/ BNE         AiIoctl_x2f68
    /*00003544*/ LDRD        R0, [R7, #80]               @ 0x50 
    /*00003548*/ BL          0x0000000000000014 
    /*0000354c*/ STR         R0, [R7, #68]               @ 0x44 
    /*00003550*/ B           AiIoctl_x2f68
    AiIoctl_x3554:
    /*00003554*/ LDRD        R0, [R7, #24] 
    /*00003558*/ BL          0x0000000000000014 
    /*0000355c*/ STR         R0, [R7, #12] 
    /*00003560*/ B           AiIoctl_x2f5c
    AiIoctl_x3564:
    /*00003564*/ MOV         R1, SL 
    /*00003568*/ MOV         R0, R9 
    /*0000356c*/ BL          0x000000000000356C          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*00003570*/ CMP         R0, #0 
    /*00003574*/ BEQ         AiIoctl_x348c
    /*00003578*/ LDRD        R0, [R7, #24] 
    /*0000357c*/ MOVW        R3, #534                    @ 0x216 
    /*00003580*/ MOVW        R2, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xacc] 
    /*00003584*/ STR         R3, [SP] 
    /*00003588*/ MOVT        R2, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0xacc] 
    /*0000358c*/ B           AiIoctl_x3308
    AiIoctl_x3590:
    /*00003590*/ MOVW        R2, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9a8] 
    /*00003594*/ MOVT        R2, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9a8] 
    /*00003598*/ MOVW        R3, #507                    @ 0x1fb 
    /*0000359c*/ STR         R6, [SP, #4] 
    /*000035a0*/ STR         R3, [SP] 
    AiIoctl_x35a4:
    /*000035a4*/ LDR         R3, [PC, #288]              @ 0x00000000000036cc 
    /*000035a8*/ MOV         R1, #21 
    /*000035ac*/ MOV         R0, #3 
    /*000035b0*/ MOVW        R6, #32771                  @ 0x8003 
    /*000035b4*/ BL          0x00000000000035B4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000035b8*/ MOVT        R6, #40981                  @ 0xa015 
    /*000035bc*/ B           AiIoctl_x30b0
    AiIoctl_x35c0:
    /*000035c0*/ LDRD        R4, [R7, #16] 
    /*000035c4*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa78] 
    /*000035c8*/ MOVW        LR, #526                    @ 0x20e 
    /*000035cc*/ LDR         R3, [PC, #248]              @ 0x00000000000036cc 
    /*000035d0*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa78] 
    /*000035d4*/ MOV         R1, #21 
    /*000035d8*/ STRD        R4, [SP, #8] 
    /*000035dc*/ MOV         R0, #3 
    /*000035e0*/ LDR         IP, [R7, #48]               @ 0x30 
    /*000035e4*/ B           AiIoctl_x3408
    AiIoctl_x35e8:
    /*000035e8*/ MOVW        R3, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcd8] 
    /*000035ec*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000035f0*/ MOVT        R3, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcd8] 
    /*000035f4*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000035f8*/ MOVW        R2, #665                    @ 0x299 
    /*000035fc*/ LDR         R1, [PC, #204]              @ 0x00000000000036d0 
    /*00003600*/ BL          0x0000000000003600          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003604*/ B           AiIoctl_x2f44
    AiIoctl_x3608:
    /*00003608*/ STR         R3, [SP, #8] 
    /*0000360c*/ MOV         R3, #568                    @ 0x238 
    /*00003610*/ STR         R2, [SP, #4] 
    /*00003614*/ MOVW        R2, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbe8] 
    /*00003618*/ STR         R3, [SP] 
    /*0000361c*/ MOVT        R2, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbe8] 
    /*00003620*/ B           AiIoctl_x3504
    AiIoctl_x3624:
    /*00003624*/ MOV         R1, SL 
    /*00003628*/ MOV         R0, R9 
    /*0000362c*/ BL          0x000000000000362C          @ hil_map_mmz_check_phys [R_ARM_CALL: +0x0] 
    /*00003630*/ CMP         R0, #0 
    /*00003634*/ BEQ         AiIoctl_x33ac
    /*00003638*/ LDRD        R0, [R7, #80]               @ 0x50 
    /*0000363c*/ MOVW        R3, #583                    @ 0x247 
    /*00003640*/ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xba0] 
    /*00003644*/ STR         R3, [SP] 
    /*00003648*/ MOVT        R2, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0xba0] 
    /*0000364c*/ B           AiIoctl_x3308
    AiIoctl_x3650:
    /*00003650*/ MOV         R3, #556                    @ 0x22c 
    /*00003654*/ MOVW        R2, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb58] 
    /*00003658*/ STR         R9, [SP, #4] 
    /*0000365c*/ MOVT        R2, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb58] 
    /*00003660*/ STR         R3, [SP] 
    /*00003664*/ B           AiIoctl_x35a4
    AiIoctl_x3668:
    /*00003668*/ STR         R3, [SP, #8] 
    /*0000366c*/ MOV         R3, #540                    @ 0x21c 
    /*00003670*/ STR         R2, [SP, #4] 
    /*00003674*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb10] 
    /*00003678*/ STR         R3, [SP] 
    /*0000367c*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb10] 
    /*00003680*/ B           AiIoctl_x3504
    AiIoctl_x3684:
    /*00003684*/ LDRD        R4, [R7, #72]               @ 0x48 
    /*00003688*/ MOVW        R2, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc34] 
    /*0000368c*/ MOVW        LR, #575                    @ 0x23f 
    /*00003690*/ LDR         R3, [PC, #52]               @ 0x00000000000036cc 
    /*00003694*/ MOVT        R2, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc34] 
    /*00003698*/ MOV         R1, #21 
    /*0000369c*/ STRD        R4, [SP, #8] 
    /*000036a0*/ MOV         R0, #3 
    /*000036a4*/ LDR         IP, [R7, #104]              @ 0x68 
    /*000036a8*/ B           AiIoctl_x3408
    AiIoctl_x36ac:
    /*000036ac*/ STR         R3, [SP, #8] 
    /*000036b0*/ MOVW        R3, #589                    @ 0x24d 
    /*000036b4*/ STR         R2, [SP, #4] 
    /*000036b8*/ MOVW        R2, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc8c] 
    /*000036bc*/ STR         R3, [SP] 
    /*000036c0*/ MOVT        R2, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc8c] 
    /*000036c4*/ B           AiIoctl_x3504
    /*000036c8*/ ANDEQ       R0, R0, IP, ROR #2 
    /*000036cc*/ ANDEQ       R0, R0, IP, ASR #2 
    /*000036d0*/ ANDEQ       R0, R0, IP, ASR R1 
FUNC_END AiIoctl


FUNC_BEGIN AI_ModInit
    /*000036d4*/ PUSH        {R4, R5, R6, LR} 
    /*000036d8*/ SUB         SP, SP, #8 
    /*000036dc*/ BL          0x00000000000036DC          @ AI_DRV_CheckAioExportFunc [R_ARM_CALL: .text+0x93f4] 
    /*000036e0*/ SUBS        R4, R0, #0 
    /*000036e4*/ BEQ         AI_ModInit_x36f4
    AI_ModInit_x36e8:
    /*000036e8*/ MOV         R0, R4 
    /*000036ec*/ ADD         SP, SP, #8 
    /*000036f0*/ POP         {R4, R5, R6, PC} 
    AI_ModInit_x36f4:
    /*000036f4*/ BL          0x00000000000036F4          @ AI_DRV_OpenClock [R_ARM_CALL: .text+0x9444] 
    /*000036f8*/ MOV         R1, R4 
    /*000036fc*/ MOVW        R0, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd14] 
    /*00003700*/ MOVT        R0, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd14] 
    /*00003704*/ BL          0x0000000000003704          @ osal_create_proc_entry [R_ARM_CALL: +0x0] 
    /*00003708*/ SUBS        R2, R0, #0 
    /*0000370c*/ BEQ         AI_ModInit_x37cc
    /*00003710*/ MOVW        R3, #0                      @ AIProcShow [R_ARM_MOVW_ABS_NC: .text+0x57fc] 
    /*00003714*/ MOVW        R0, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd14] 
    /*00003718*/ MOVT        R3, #0                      @ AIProcShow [R_ARM_MOVT_ABS: .text+0x57fc] 
    /*0000371c*/ MOVT        R0, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd14] 
    /*00003720*/ STR         R3, [R2, #60]               @ 0x3c 
    /*00003724*/ MOVW        R5, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003728*/ BL          0x0000000000003728          @ osal_createdev [R_ARM_CALL: +0x0] 
    /*0000372c*/ LDR         R6, [PC, #192]              @ 0x00000000000037f4 
    /*00003730*/ MOVT        R5, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003734*/ ADD         R2, R6, #32 
    /*00003738*/ STR         R6, [R0, #56]               @ 0x38 
    /*0000373c*/ STR         R2, [R0, #60]               @ 0x3c 
    /*00003740*/ MOV         R2, #5 
    /*00003744*/ STR         R2, [R0, #52]               @ 0x34 
    /*00003748*/ STR         R0, [R5, #4] 
    /*0000374c*/ BL          0x000000000000374C          @ osal_registerdevice [R_ARM_CALL: +0x0] 
    /*00003750*/ SUBS        R4, R0, #0 
    /*00003754*/ BNE         AI_ModInit_x378c
    /*00003758*/ ADD         R0, R6, #112                @ 0x70 
    /*0000375c*/ BL          0x000000000000375C          @ CMPI_RegisterModule [R_ARM_CALL: +0x0] 
    /*00003760*/ CMP         R0, #0 
    /*00003764*/ BNE         AI_ModInit_x37a4
    /*00003768*/ BL          0x0000000000003768          @ AI_DRV_CloseClock [R_ARM_CALL: .text+0x944c] 
    /*0000376c*/ MOV         R0, R5 
    /*00003770*/ BL          0x0000000000003770          @ osal_atomic_init [R_ARM_CALL: +0x0] 
    /*00003774*/ MOV         R0, R5 
    /*00003778*/ MOV         R1, R4 
    /*0000377c*/ BL          0x000000000000377C          @ osal_atomic_set [R_ARM_CALL: +0x0] 
    /*00003780*/ MOV         R0, R4 
    /*00003784*/ ADD         SP, SP, #8 
    /*00003788*/ POP         {R4, R5, R6, PC} 
    AI_ModInit_x378c:
    /*0000378c*/ MOVW        R0, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd14] 
    /*00003790*/ MOV         R1, #0 
    /*00003794*/ MOVT        R0, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd14] 
    /*00003798*/ MVN         R4, #0 
    /*0000379c*/ BL          0x000000000000379C          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*000037a0*/ B           AI_ModInit_x36e8
    AI_ModInit_x37a4:
    /*000037a4*/ MOV         R1, R4 
    /*000037a8*/ MOVW        R0, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd14] 
    /*000037ac*/ MOVT        R0, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd14] 
    /*000037b0*/ MVN         R4, #0 
    /*000037b4*/ BL          0x00000000000037B4          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*000037b8*/ LDR         R0, [R5, #4] 
    /*000037bc*/ BL          0x00000000000037BC          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*000037c0*/ LDR         R0, [R5, #4] 
    /*000037c4*/ BL          0x00000000000037C4          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*000037c8*/ B           AI_ModInit_x36e8
    AI_ModInit_x37cc:
    /*000037cc*/ MOVW        R3, #1878                   @ 0x756 
    /*000037d0*/ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd18] 
    /*000037d4*/ STR         R3, [SP] 
    /*000037d8*/ MOVT        R2, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd18] 
    /*000037dc*/ LDR         R3, [PC, #20]               @ 0x00000000000037f8 
    /*000037e0*/ MOV         R1, #22 
    /*000037e4*/ MOV         R0, #6 
    /*000037e8*/ MVN         R4, #0 
    /*000037ec*/ BL          0x00000000000037EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000037f0*/ B           AI_ModInit_x36e8
    /*000037f4*/ ANDEQ       R0, R0, R4 
    /*000037f8*/ ANDEQ       R0, R0, R8, ROR R1 
FUNC_END AI_ModInit


FUNC_BEGIN AI_ModExit
    /*000037fc*/ PUSH        {R4, LR} 
    /*00003800*/ MOVW        R4, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003804*/ MOVT        R4, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003808*/ MOV         R0, R4 
    /*0000380c*/ BL          0x000000000000380C          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*00003810*/ MOV         R0, #21 
    /*00003814*/ BL          0x0000000000003814          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    /*00003818*/ LDR         R0, [R4, #4] 
    /*0000381c*/ BL          0x000000000000381C          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*00003820*/ LDR         R0, [R4, #4] 
    /*00003824*/ BL          0x0000000000003824          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*00003828*/ MOVW        R0, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd14] 
    /*0000382c*/ MOV         R1, #0 
    /*00003830*/ MOVT        R0, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd14] 
    /*00003834*/ POP         {R4, LR} 
    AI_ModExit_x3838:
    /*00003838*/ B           AI_ModExit_x3838            @ osal_remove_proc_entry [R_ARM_JUMP24: +0x0] 
FUNC_END AI_ModExit


FUNC_BEGIN AIGetFrmVbBlk
    /*0000383c*/ CMP         R1, #0 
    /*00003840*/ CMPNE       R0, #0 
    /*00003844*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00003848*/ MOVEQ       R7, #1 
    /*0000384c*/ MOVNE       R7, #0 
    /*00003850*/ BEQ         AIGetFrmVbBlk_x3900
    /*00003854*/ MOV         R5, R0 
    /*00003858*/ MOV         R0, #1 
    /*0000385c*/ MOV         R4, R1 
    /*00003860*/ BL          0x0000000000003860          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003864*/ MOV         R1, #14 
    /*00003868*/ LDR         R3, [R0, #8] 
    /*0000386c*/ LDR         R0, [R5] 
    /*00003870*/ BLX         R3 
    /*00003874*/ CMN         R0, #1 
    /*00003878*/ MOV         R6, R0 
    /*0000387c*/ BEQ         AIGetFrmVbBlk_x38f8
    /*00003880*/ LDR         R3, [R5, #20] 
    /*00003884*/ MOV         R0, #1 
    /*00003888*/ STR         R3, [R4] 
    /*0000388c*/ LDR         R3, [R5, #28] 
    /*00003890*/ STR         R3, [R4, #4] 
    /*00003894*/ BL          0x0000000000003894          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003898*/ LDR         R3, [R0, #48]               @ 0x30 
    /*0000389c*/ MOV         R0, R6 
    /*000038a0*/ BLX         R3 
    /*000038a4*/ STR         R0, [R4, #8] 
    /*000038a8*/ MOV         R0, #1 
    /*000038ac*/ BL          0x00000000000038AC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000038b0*/ LDR         R3, [R0, #52]               @ 0x34 
    /*000038b4*/ MOV         R0, R6 
    /*000038b8*/ BLX         R3 
    /*000038bc*/ STRD        R0, [R4, #16] 
    /*000038c0*/ MOV         R0, #1 
    /*000038c4*/ BL          0x00000000000038C4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000038c8*/ LDR         R3, [R0, #40]               @ 0x28 
    /*000038cc*/ MOV         R0, R6 
    /*000038d0*/ BLX         R3 
    /*000038d4*/ MOV         R6, #0 
    /*000038d8*/ STR         R7, [R4, #12] 
    /*000038dc*/ MOV         R3, R7 
    /*000038e0*/ STR         R7, [R4, #52]               @ 0x34 
    /*000038e4*/ MOV         R7, #0 
    /*000038e8*/ STRD        R6, [R4, #24] 
    /*000038ec*/ STR         R0, [R4, #48]               @ 0x30 
    AIGetFrmVbBlk_x38f0:
    /*000038f0*/ MOV         R0, R3 
    /*000038f4*/ POP         {R4, R5, R6, R7, R8, PC} 
    AIGetFrmVbBlk_x38f8:
    /*000038f8*/ MOV         R3, R0 
    /*000038fc*/ B           AIGetFrmVbBlk_x38f0
    AIGetFrmVbBlk_x3900:
    /*00003900*/ MOVW        R3, #32774                  @ 0x8006 
    /*00003904*/ MOVT        R3, #40981                  @ 0xa015 
    /*00003908*/ B           AIGetFrmVbBlk_x38f0
FUNC_END AIGetFrmVbBlk


FUNC_BEGIN AIPutFrmVbBlk
    /*0000390c*/ PUSH        {R4, LR} 
    /*00003910*/ SUBS        R4, R0, #0 
    /*00003914*/ SUB         SP, SP, #8 
    /*00003918*/ BEQ         AIPutFrmVbBlk_x394c
    /*0000391c*/ MOV         R0, #1 
    /*00003920*/ BL          0x0000000000003920          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003924*/ LDR         IP, [R4, #48]               @ 0x30 
    /*00003928*/ LDRD        R2, [R4, #16] 
    /*0000392c*/ MOV         R1, #14 
    /*00003930*/ STR         R1, [SP] 
    /*00003934*/ LDR         R1, [R0, #64]               @ 0x40 
    /*00003938*/ MOV         R0, IP 
    /*0000393c*/ BLX         R1 
    /*00003940*/ MOV         R0, #0 
    AIPutFrmVbBlk_x3944:
    /*00003944*/ ADD         SP, SP, #8 
    /*00003948*/ POP         {R4, PC} 
    AIPutFrmVbBlk_x394c:
    /*0000394c*/ MOVW        R0, #32774                  @ 0x8006 
    /*00003950*/ MOVT        R0, #40981                  @ 0xa015 
    /*00003954*/ B           AIPutFrmVbBlk_x3944
FUNC_END AIPutFrmVbBlk


FUNC_BEGIN AIAddVb
    /*00003958*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000395c*/ SUBS        R4, R0, #0 
    /*00003960*/ SUB         SP, SP, #12 
    /*00003964*/ BEQ         AIAddVb_x3a44
    /*00003968*/ LDR         R3, [R4, #4] 
    /*0000396c*/ MOVW        FP, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd4c] 
    /*00003970*/ MOVW        SL, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x184] 
    /*00003974*/ MOVW        R9, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd64] 
    /*00003978*/ CMP         R3, #1 
    /*0000397c*/ MOVT        FP, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd4c] 
    /*00003980*/ MOVT        SL, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x184] 
    /*00003984*/ MOVT        R9, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd64] 
    /*00003988*/ MOVNE       R8, #52                     @ 0x34 
    /*0000398c*/ MOVEQ       R8, #56                     @ 0x38 
    /*00003990*/ ADD         R5, R4, #48                 @ 0x30 
    /*00003994*/ ADD         R8, R4, R8 
    /*00003998*/ ADD         R7, R4, #16 
    /*0000399c*/ MOV         R6, R1 
    AIAddVb_x39a0:
    /*000039a0*/ MOV         R0, #1 
    /*000039a4*/ BL          0x00000000000039A4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000039a8*/ LDR         IP, [R5], #4 
    /*000039ac*/ LDRD        R2, [R7], #8 
    /*000039b0*/ STR         R6, [SP] 
    /*000039b4*/ LDR         R1, [R0, #60]               @ 0x3c 
    /*000039b8*/ MOV         R0, IP 
    /*000039bc*/ BLX         R1 
    /*000039c0*/ CMP         R0, #0 
    /*000039c4*/ BNE         AIAddVb_x39e8
    /*000039c8*/ LDR         R0, [R4, #112]              @ 0x70 
    /*000039cc*/ CMP         R0, #1 
    /*000039d0*/ BEQ         AIAddVb_x3a08
    AIAddVb_x39d4:
    /*000039d4*/ CMP         R5, R8 
    /*000039d8*/ BNE         AIAddVb_x39a0
    /*000039dc*/ MOV         R0, #0 
    AIAddVb_x39e0:
    /*000039e0*/ ADD         SP, SP, #12 
    /*000039e4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIAddVb_x39e8:
    /*000039e8*/ MOV         R0, R9 
    /*000039ec*/ MOV         R3, FP 
    /*000039f0*/ MOV         R2, #103                    @ 0x67 
    /*000039f4*/ MOV         R1, SL 
    /*000039f8*/ BL          0x00000000000039F8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000039fc*/ LDR         R0, [R4, #112]              @ 0x70 
    /*00003a00*/ CMP         R0, #1 
    /*00003a04*/ BNE         AIAddVb_x39d4
    AIAddVb_x3a08:
    /*00003a08*/ BL          0x0000000000003A08          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003a0c*/ LDR         IP, [R5, #52]               @ 0x34 
    /*00003a10*/ LDRD        R2, [R7, #48]               @ 0x30 
    /*00003a14*/ STR         R6, [SP] 
    /*00003a18*/ LDR         R1, [R0, #60]               @ 0x3c 
    /*00003a1c*/ MOV         R0, IP 
    /*00003a20*/ BLX         R1 
    /*00003a24*/ CMP         R0, #0 
    /*00003a28*/ BEQ         AIAddVb_x39d4
    /*00003a2c*/ MOV         R3, FP 
    /*00003a30*/ MOV         R2, #108                    @ 0x6c 
    /*00003a34*/ MOV         R1, SL 
    /*00003a38*/ MOV         R0, R9 
    /*00003a3c*/ BL          0x0000000000003A3C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003a40*/ B           AIAddVb_x39d4
    AIAddVb_x3a44:
    /*00003a44*/ MOVW        R0, #32774                  @ 0x8006 
    /*00003a48*/ MOVT        R0, #40981                  @ 0xa015 
    /*00003a4c*/ B           AIAddVb_x39e0
FUNC_END AIAddVb


FUNC_BEGIN AISubVb
    /*00003a50*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00003a54*/ SUBS        R9, R0, #0 
    /*00003a58*/ SUB         SP, SP, #44                 @ 0x2c 
    /*00003a5c*/ MOV         R5, #0 
    /*00003a60*/ STR         R5, [SP, #32] 
    /*00003a64*/ STR         R5, [SP, #36]               @ 0x24 
    /*00003a68*/ BEQ         AISubVb_x3c5c
    /*00003a6c*/ LDR         R3, [R9, #4] 
    /*00003a70*/ MOV         R6, R1 
    /*00003a74*/ ADD         R4, R9, #48                 @ 0x30 
    /*00003a78*/ ADD         R8, R9, #16 
    /*00003a7c*/ CMP         R3, #1 
    /*00003a80*/ MOVEQ       SL, #2 
    /*00003a84*/ MOVNE       SL, #1 
    /*00003a88*/ B           AISubVb_x3a98
    AISubVb_x3a8c:
    /*00003a8c*/ ADD         R5, R5, #1 
    /*00003a90*/ CMP         SL, R5 
    /*00003a94*/ BEQ         AISubVb_x3ba4
    AISubVb_x3a98:
    /*00003a98*/ MOV         R0, #1 
    /*00003a9c*/ MOV         R7, R4 
    /*00003aa0*/ BL          0x0000000000003AA0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003aa4*/ ADD         R2, SP, #32 
    /*00003aa8*/ MOV         R1, R6 
    /*00003aac*/ ADD         R4, R4, #4 
    /*00003ab0*/ LDR         R3, [R0, #96]               @ 0x60 
    /*00003ab4*/ LDR         R0, [R4, #-4] 
    /*00003ab8*/ BLX         R3 
    /*00003abc*/ CMP         R0, #0 
    /*00003ac0*/ MOV         R0, #1 
    /*00003ac4*/ BNE         AISubVb_x3bb0
    /*00003ac8*/ LDR         R3, [SP, #32] 
    /*00003acc*/ CMP         R3, #0 
    /*00003ad0*/ BEQ         AISubVb_x3be0
    /*00003ad4*/ BL          0x0000000000003AD4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003ad8*/ LDR         IP, [R7] 
    /*00003adc*/ LDRD        R2, [R8], #8 
    /*00003ae0*/ STR         R6, [SP] 
    /*00003ae4*/ LDR         R1, [R0, #64]               @ 0x40 
    /*00003ae8*/ MOV         R0, IP 
    /*00003aec*/ BLX         R1 
    /*00003af0*/ SUBS        FP, R0, #0 
    /*00003af4*/ BNE         AISubVb_x3bf4
    /*00003af8*/ LDR         R7, [R9, #112]              @ 0x70 
    /*00003afc*/ CMP         R7, #1 
    /*00003b00*/ BNE         AISubVb_x3a8c
    /*00003b04*/ MOV         R0, R7 
    /*00003b08*/ BL          0x0000000000003B08          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003b0c*/ ADD         R2, SP, #36                 @ 0x24 
    /*00003b10*/ MOV         R1, R6 
    /*00003b14*/ LDR         R3, [R0, #96]               @ 0x60 
    /*00003b18*/ LDR         R0, [R4, #52]               @ 0x34 
    /*00003b1c*/ BLX         R3 
    /*00003b20*/ CMP         R0, #0 
    /*00003b24*/ BNE         AISubVb_x3c48
    /*00003b28*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00003b2c*/ CMP         R3, #0 
    /*00003b30*/ BEQ         AISubVb_x3a8c
    /*00003b34*/ MOV         R0, R7 
    /*00003b38*/ BL          0x0000000000003B38          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003b3c*/ LDR         IP, [R4, #52]               @ 0x34 
    /*00003b40*/ LDRD        R2, [R8, #48]               @ 0x30 
    /*00003b44*/ STR         R6, [SP] 
    /*00003b48*/ LDR         R1, [R0, #64]               @ 0x40 
    /*00003b4c*/ MOV         R0, IP 
    /*00003b50*/ BLX         R1 
    /*00003b54*/ SUBS        IP, R0, #0 
    /*00003b58*/ BEQ         AISubVb_x3a8c
    /*00003b5c*/ ADD         R3, R9, R5, LSL #3 
    /*00003b60*/ STR         R6, [SP, #24] 
    /*00003b64*/ ADD         R4, R5, #26 
    /*00003b68*/ MOV         LR, #159                    @ 0x9f 
    /*00003b6c*/ LDRD        R0, [R3, #72]               @ 0x48 
    /*00003b70*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe90] 
    /*00003b74*/ LDR         R3, [PC, #236]              @ 0x0000000000003c68 
    /*00003b78*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe90] 
    /*00003b7c*/ MVN         FP, #0 
    /*00003b80*/ STRD        R0, [SP, #16] 
    /*00003b84*/ MOV         R1, #21 
    /*00003b88*/ LDR         R4, [R9, R4, LSL #2] 
    /*00003b8c*/ MOV         R0, #3 
    /*00003b90*/ STR         R5, [SP, #8] 
    /*00003b94*/ STR         IP, [SP, #4] 
    /*00003b98*/ STR         R4, [SP, #12] 
    /*00003b9c*/ STR         LR, [SP] 
    /*00003ba0*/ BL          0x0000000000003BA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    AISubVb_x3ba4:
    /*00003ba4*/ MOV         R0, FP 
    /*00003ba8*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00003bac*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AISubVb_x3bb0:
    /*00003bb0*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xda4] 
    /*00003bb4*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0xda4] 
    /*00003bb8*/ MOV         R3, #129                    @ 0x81 
    /*00003bbc*/ STR         R3, [SP] 
    AISubVb_x3bc0:
    /*00003bc0*/ LDR         R3, [PC, #160]              @ 0x0000000000003c68 
    /*00003bc4*/ MOV         R1, #21 
    /*00003bc8*/ MOV         R0, #3 
    /*00003bcc*/ MVN         FP, #0 
    /*00003bd0*/ BL          0x0000000000003BD0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003bd4*/ MOV         R0, FP 
    /*00003bd8*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00003bdc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AISubVb_x3be0:
    /*00003be0*/ MOV         R3, #168                    @ 0xa8 
    /*00003be4*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xef8] 
    /*00003be8*/ STR         R3, [SP] 
    /*00003bec*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0xef8] 
    /*00003bf0*/ B           AISubVb_x3bc0
    AISubVb_x3bf4:
    /*00003bf4*/ ADD         R3, R9, R5, LSL #3 
    /*00003bf8*/ STR         R5, [SP, #28] 
    /*00003bfc*/ STR         R6, [SP, #24] 
    /*00003c00*/ ADD         IP, R5, #12 
    /*00003c04*/ LDRD        R4, [R3, #16] 
    /*00003c08*/ MOV         LR, #139                    @ 0x8b 
    /*00003c0c*/ LDR         R3, [PC, #84]               @ 0x0000000000003c68 
    /*00003c10*/ MOV         R1, #21 
    /*00003c14*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xde8] 
    /*00003c18*/ MOV         R0, #3 
    /*00003c1c*/ STRD        R4, [SP, #16] 
    /*00003c20*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xde8] 
    /*00003c24*/ LDR         IP, [R9, IP, LSL #2] 
    /*00003c28*/ MVN         FP, #0 
    /*00003c2c*/ STR         R9, [SP, #4] 
    /*00003c30*/ STR         LR, [SP] 
    /*00003c34*/ STR         IP, [SP, #8] 
    /*00003c38*/ BL          0x0000000000003C38          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003c3c*/ MOV         R0, FP 
    /*00003c40*/ ADD         SP, SP, #44                 @ 0x2c 
    /*00003c44*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AISubVb_x3c48:
    /*00003c48*/ MOV         R3, #148                    @ 0x94 
    /*00003c4c*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe48] 
    /*00003c50*/ STR         R3, [SP] 
    /*00003c54*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe48] 
    /*00003c58*/ B           AISubVb_x3bc0
    AISubVb_x3c5c:
    /*00003c5c*/ MOVW        FP, #32774                  @ 0x8006 
    /*00003c60*/ MOVT        FP, #40981                  @ 0xa015 
    /*00003c64*/ B           AISubVb_x3ba4
    /*00003c68*/ ANDEQ       R0, R0, IP, LSL #3 
FUNC_END AISubVb


FUNC_BEGIN AISendChnData
    /*00003c6c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00003c70*/ MOVW        SL, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*00003c74*/ MOVT        SL, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*00003c78*/ MOV         R5, #136                    @ 0x88 
    /*00003c7c*/ SUB         SP, SP, #164                @ 0xa4 
    /*00003c80*/ MOV         FP, R0 
    /*00003c84*/ MLA         R9, R5, R0, SL 
    /*00003c88*/ LDR         R3, [R9, #28] 
    /*00003c8c*/ LDR         R6, [R9, #44]               @ 0x2c 
    /*00003c90*/ CMP         R3, #1 
    /*00003c94*/ MOVNE       R5, #1 
    /*00003c98*/ MOVEQ       R5, #2 
    /*00003c9c*/ MOV         R0, R6 
    /*00003ca0*/ MOV         R1, R5 
    /*00003ca4*/ BL          0x0000000000003CA4          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00003ca8*/ CMP         R0, #0 
    /*00003cac*/ BEQ         AISendChnData_x3cfc
    /*00003cb0*/ LSL         R3, FP, #1 
    /*00003cb4*/ MOV         R4, #1120                   @ 0x460 
    /*00003cb8*/ MOV         R7, #0 
    /*00003cbc*/ STR         R3, [SP, #12] 
    /*00003cc0*/ MLA         R4, R4, R3, SL 
    /*00003cc4*/ STMIB       SP, {SL, FP} 
    /*00003cc8*/ ADD         R4, R4, #140                @ 0x8c 
    AISendChnData_x3ccc:
    /*00003ccc*/ LDR         R3, [R4, #204]              @ 0xcc 
    /*00003cd0*/ CMP         R3, #1 
    /*00003cd4*/ BEQ         AISendChnData_x3d54
    AISendChnData_x3cd8:
    /*00003cd8*/ LDR         R6, [R9, #44]               @ 0x2c 
    AISendChnData_x3cdc:
    /*00003cdc*/ MOV         R1, R5 
    /*00003ce0*/ MOV         R0, R6 
    /*00003ce4*/ BL          0x0000000000003CE4          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00003ce8*/ ADD         R7, R7, #1 
    /*00003cec*/ ADD         R4, R4, #1120               @ 0x460 
    /*00003cf0*/ CMP         R0, R7 
    /*00003cf4*/ BHI         AISendChnData_x3ccc
    /*00003cf8*/ LDMIB       SP, {SL, FP} 
    AISendChnData_x3cfc:
    /*00003cfc*/ CMP         R6, #0 
    /*00003d00*/ BEQ         AISendChnData_x3d48
    /*00003d04*/ LDR         R3, [PC, #824]              @ 0x0000000000004044 
    /*00003d08*/ LSL         R4, FP, #1 
    /*00003d0c*/ MOV         R2, #1120                   @ 0x460 
    /*00003d10*/ MOV         R6, #136                    @ 0x88 
    /*00003d14*/ MLA         R6, R6, FP, SL 
    /*00003d18*/ MOV         R5, #0 
    /*00003d1c*/ MLA         R4, R2, R4, R3 
    /*00003d20*/ MOV         R7, R5 
    /*00003d24*/ ADD         R4, R4, #16 
    AISendChnData_x3d28:
    /*00003d28*/ LDR         R3, [R4, #128]              @ 0x80 
    /*00003d2c*/ ADD         R5, R5, #1 
    /*00003d30*/ CMP         R3, #1 
    /*00003d34*/ BEQ         AISendChnData_x3e3c
    AISendChnData_x3d38:
    /*00003d38*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*00003d3c*/ ADD         R4, R4, #1120               @ 0x460 
    /*00003d40*/ CMP         R3, R5 
    /*00003d44*/ BHI         AISendChnData_x3d28
    AISendChnData_x3d48:
    /*00003d48*/ MOV         R0, #0 
    /*00003d4c*/ ADD         SP, SP, #164                @ 0xa4 
    /*00003d50*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AISendChnData_x3d54:
    /*00003d54*/ LDR         R3, [R4, #140]              @ 0x8c 
    /*00003d58*/ CMP         R3, #0 
    /*00003d5c*/ BEQ         AISendChnData_x3cd8
    /*00003d60*/ LDR         R3, [SP, #12] 
    /*00003d64*/ MOV         R2, #128                    @ 0x80 
    /*00003d68*/ ADD         R1, R4, #12 
    /*00003d6c*/ ADD         R0, SP, #32 
    /*00003d70*/ ADD         R8, R7, R3 
    /*00003d74*/ BL          0x0000000000003D74          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00003d78*/ LDR         R3, [R9, #28] 
    /*00003d7c*/ CMP         R3, #1 
    /*00003d80*/ BNE         AISendChnData_x3dc4
    /*00003d84*/ LDR         R6, [R9, #44]               @ 0x2c 
    /*00003d88*/ MOV         R3, #1120                   @ 0x460 
    /*00003d8c*/ LDR         R2, [SP, #4] 
    /*00003d90*/ ADD         R1, R8, R6, LSR #1 
    /*00003d94*/ MLA         R1, R3, R1, R2 
    /*00003d98*/ LDR         R3, [R1, #280]              @ 0x118 
    /*00003d9c*/ CMP         R3, #0 
    /*00003da0*/ BEQ         AISendChnData_x3cdc
    /*00003da4*/ LDR         IP, [R1, #200]              @ 0xc8 
    /*00003da8*/ MOV         R0, #0 
    /*00003dac*/ LDRD        R2, [R1, #168]              @ 0xa8 
    /*00003db0*/ LDR         R1, [R1, #160]              @ 0xa0 
    /*00003db4*/ STR         IP, [SP, #84]               @ 0x54 
    /*00003db8*/ STRD        R2, [SP, #56]               @ 0x38 
    /*00003dbc*/ STR         R1, [SP, #44]               @ 0x2c 
    /*00003dc0*/ STR         R0, [SP, #144]              @ 0x90 
    AISendChnData_x3dc4:
    /*00003dc4*/ ADD         R3, R4, #148                @ 0x94 
    /*00003dc8*/ LDR         R6, [R4, #144]              @ 0x90 
    /*00003dcc*/ STR         R3, [SP, #16] 
    /*00003dd0*/ ADD         SL, R4, #176                @ 0xb0 
    /*00003dd4*/ MOV         R0, R3 
    /*00003dd8*/ MOV         FP, R3 
    /*00003ddc*/ BL          0x0000000000003DDC          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*00003de0*/ MOV         R8, R0 
    /*00003de4*/ MOV         R0, SL 
    /*00003de8*/ BL          0x0000000000003DE8          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*00003dec*/ ADD         R0, R8, R0 
    /*00003df0*/ CMP         R6, R0 
    /*00003df4*/ BLS         AISendChnData_x3e50
    /*00003df8*/ MOV         R0, FP 
    /*00003dfc*/ BL          0x0000000000003DFC          @ AUDIO_FB_GetFree [R_ARM_CALL: .text+0x7ffc] 
    /*00003e00*/ SUBS        R6, R0, #0 
    /*00003e04*/ BEQ         AISendChnData_x4024
    AISendChnData_x3e08:
    /*00003e08*/ MOV         R2, #128                    @ 0x80 
    /*00003e0c*/ ADD         R1, SP, #32 
    /*00003e10*/ ADD         R0, R6, #8 
    /*00003e14*/ BL          0x0000000000003E14          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00003e18*/ MOV         R1, R6 
    /*00003e1c*/ LDR         R0, [SP, #16] 
    /*00003e20*/ BL          0x0000000000003E20          @ AUDIO_FB_PutBusy [R_ARM_CALL: .text+0x810c] 
    /*00003e24*/ ADD         R0, SP, #32 
    /*00003e28*/ MOV         R1, #14 
    /*00003e2c*/ BL          0x0000000000003E2C          @ AIAddVb [R_ARM_CALL: .text+0x3958] 
    /*00003e30*/ MOV         R0, R4 
    /*00003e34*/ BL          0x0000000000003E34          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00003e38*/ B           AISendChnData_x3cd8
    AISendChnData_x3e3c:
    /*00003e3c*/ MOV         R1, #14 
    /*00003e40*/ MOV         R0, R4 
    /*00003e44*/ STR         R7, [R4, #4] 
    /*00003e48*/ BL          0x0000000000003E48          @ AISubVb [R_ARM_CALL: .text+0x3a50] 
    /*00003e4c*/ B           AISendChnData_x3d38
    AISendChnData_x3e50:
    /*00003e50*/ LDR         R8, [SP, #16] 
    /*00003e54*/ MOV         R0, R8 
    /*00003e58*/ BL          0x0000000000003E58          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*00003e5c*/ CMP         R6, R0 
    /*00003e60*/ BLS         AISendChnData_x3f08
    /*00003e64*/ MOV         R0, R8 
    /*00003e68*/ RSB         R6, R6, #1 
    /*00003e6c*/ BL          0x0000000000003E6C          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*00003e70*/ MOV         R8, R0 
    /*00003e74*/ MOV         R0, SL 
    /*00003e78*/ BL          0x0000000000003E78          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*00003e7c*/ ADD         R6, R6, R8 
    /*00003e80*/ ADDS        R6, R6, R0 
    /*00003e84*/ BEQ         AISendChnData_x3ed8
    /*00003e88*/ MOV         R8, #0 
    /*00003e8c*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf58] 
    /*00003e90*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd64] 
    /*00003e94*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf58] 
    /*00003e98*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd64] 
    /*00003e9c*/ STR         R3, [SP, #20] 
    /*00003ea0*/ STR         R2, [SP, #24] 
    AISendChnData_x3ea4:
    /*00003ea4*/ MOV         R0, SL 
    /*00003ea8*/ ADD         R8, R8, #1 
    /*00003eac*/ BL          0x0000000000003EAC          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x805c] 
    /*00003eb0*/ SUBS        FP, R0, #0 
    /*00003eb4*/ BEQ         AISendChnData_x400c
    AISendChnData_x3eb8:
    /*00003eb8*/ MOV         R1, #14 
    /*00003ebc*/ ADD         R0, FP, #8 
    /*00003ec0*/ BL          0x0000000000003EC0          @ AISubVb [R_ARM_CALL: .text+0x3a50] 
    /*00003ec4*/ MOV         R1, FP 
    /*00003ec8*/ MOV         R0, SL 
    /*00003ecc*/ BL          0x0000000000003ECC          @ AUDIO_FB_PutFree [R_ARM_CALL: .text+0x80bc] 
    /*00003ed0*/ CMP         R6, R8 
    /*00003ed4*/ BNE         AISendChnData_x3ea4
    AISendChnData_x3ed8:
    /*00003ed8*/ LDR         R0, [SP, #16] 
    /*00003edc*/ BL          0x0000000000003EDC          @ AUDIO_FB_GetFree [R_ARM_CALL: .text+0x7ffc] 
    /*00003ee0*/ SUBS        R6, R0, #0 
    /*00003ee4*/ BNE         AISendChnData_x3e08
    /*00003ee8*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf58] 
    /*00003eec*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd64] 
    /*00003ef0*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf58] 
    /*00003ef4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd64] 
    /*00003ef8*/ MOV         R2, #254                    @ 0xfe 
    /*00003efc*/ LDR         R1, [PC, #324]              @ 0x0000000000004048 
    /*00003f00*/ BL          0x0000000000003F00          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003f04*/ B           AISendChnData_x3e08
    AISendChnData_x3f08:
    /*00003f08*/ LDR         R0, [SP, #16] 
    /*00003f0c*/ RSB         R6, R6, #1 
    /*00003f10*/ BL          0x0000000000003F10          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*00003f14*/ ADDS        R6, R6, R0 
    /*00003f18*/ BEQ         AISendChnData_x3f78
    /*00003f1c*/ LDR         FP, [SP, #16] 
    /*00003f20*/ MOV         R8, #0 
    /*00003f24*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf58] 
    /*00003f28*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd64] 
    /*00003f2c*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf58] 
    /*00003f30*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd64] 
    /*00003f34*/ STR         R3, [SP, #24] 
    /*00003f38*/ STR         R2, [SP, #28] 
    /*00003f3c*/ STR         SL, [SP, #20] 
    AISendChnData_x3f40:
    /*00003f40*/ MOV         R0, FP 
    /*00003f44*/ ADD         R8, R8, #1 
    /*00003f48*/ BL          0x0000000000003F48          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x805c] 
    /*00003f4c*/ SUBS        SL, R0, #0 
    /*00003f50*/ BEQ         AISendChnData_x3fdc
    AISendChnData_x3f54:
    /*00003f54*/ MOV         R1, #14 
    /*00003f58*/ ADD         R0, SL, #8 
    /*00003f5c*/ BL          0x0000000000003F5C          @ AISubVb [R_ARM_CALL: .text+0x3a50] 
    /*00003f60*/ MOV         R1, SL 
    /*00003f64*/ MOV         R0, FP 
    /*00003f68*/ BL          0x0000000000003F68          @ AUDIO_FB_PutFree [R_ARM_CALL: .text+0x80bc] 
    /*00003f6c*/ CMP         R8, R6 
    /*00003f70*/ BNE         AISendChnData_x3f40
    /*00003f74*/ LDR         SL, [SP, #20] 
    AISendChnData_x3f78:
    /*00003f78*/ MOV         R0, SL 
    /*00003f7c*/ BL          0x0000000000003F7C          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*00003f80*/ SUBS        FP, R0, #0 
    /*00003f84*/ BEQ         AISendChnData_x3ed8
    /*00003f88*/ MOV         R6, #0 
    /*00003f8c*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf58] 
    /*00003f90*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd64] 
    /*00003f94*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf58] 
    /*00003f98*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd64] 
    /*00003f9c*/ STR         R3, [SP, #20] 
    /*00003fa0*/ STR         R2, [SP, #24] 
    AISendChnData_x3fa4:
    /*00003fa4*/ MOV         R0, SL 
    /*00003fa8*/ ADD         R6, R6, #1 
    /*00003fac*/ BL          0x0000000000003FAC          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x805c] 
    /*00003fb0*/ SUBS        R8, R0, #0 
    /*00003fb4*/ BEQ         AISendChnData_x3ff4
    AISendChnData_x3fb8:
    /*00003fb8*/ MOV         R1, #14 
    /*00003fbc*/ ADD         R0, R8, #8 
    /*00003fc0*/ BL          0x0000000000003FC0          @ AISubVb [R_ARM_CALL: .text+0x3a50] 
    /*00003fc4*/ MOV         R1, R8 
    /*00003fc8*/ MOV         R0, SL 
    /*00003fcc*/ BL          0x0000000000003FCC          @ AUDIO_FB_PutFree [R_ARM_CALL: .text+0x80bc] 
    /*00003fd0*/ CMP         R6, FP 
    /*00003fd4*/ BNE         AISendChnData_x3fa4
    /*00003fd8*/ B           AISendChnData_x3ed8
    AISendChnData_x3fdc:
    /*00003fdc*/ LDR         R3, [SP, #24] 
    /*00003fe0*/ MOV         R2, #235                    @ 0xeb 
    /*00003fe4*/ LDR         R1, [PC, #92]               @ 0x0000000000004048 
    /*00003fe8*/ LDR         R0, [SP, #28] 
    /*00003fec*/ BL          0x0000000000003FEC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003ff0*/ B           AISendChnData_x3f54
    AISendChnData_x3ff4:
    /*00003ff4*/ LDR         R3, [SP, #20] 
    /*00003ff8*/ MOV         R2, #246                    @ 0xf6 
    /*00003ffc*/ LDR         R1, [PC, #68]               @ 0x0000000000004048 
    /*00004000*/ LDR         R0, [SP, #24] 
    /*00004004*/ BL          0x0000000000004004          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004008*/ B           AISendChnData_x3fb8
    AISendChnData_x400c:
    /*0000400c*/ LDR         R3, [SP, #20] 
    /*00004010*/ MOV         R2, #222                    @ 0xde 
    /*00004014*/ LDR         R1, [PC, #44]               @ 0x0000000000004048 
    /*00004018*/ LDR         R0, [SP, #24] 
    /*0000401c*/ BL          0x000000000000401C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004020*/ B           AISendChnData_x3eb8
    AISendChnData_x4024:
    /*00004024*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf58] 
    /*00004028*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd64] 
    /*0000402c*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf58] 
    /*00004030*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd64] 
    /*00004034*/ MOV         R2, #206                    @ 0xce 
    /*00004038*/ LDR         R1, [PC, #8]                @ 0x0000000000004048 
    /*0000403c*/ BL          0x000000000000403C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004040*/ B           AISendChnData_x3e08
    /*00004044*/ MULEQ       R0, R0, R0 
    /*00004048*/ MULEQ       R0, R4, R1 
FUNC_END AISendChnData


FUNC_BEGIN AIDmaCallback
    /*0000404c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00004050*/ MOV         R6, #136                    @ 0x88 
    /*00004054*/ MUL         R6, R6, R0 
    /*00004058*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*0000405c*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*00004060*/ SUB         SP, SP, #36                 @ 0x24 
    /*00004064*/ MOV         R7, R0 
    /*00004068*/ ADD         R1, SP, #28 
    /*0000406c*/ ADD         R3, R6, #128                @ 0x80 
    /*00004070*/ ADD         SL, R5, R6 
    /*00004074*/ ADD         R0, R5, R3 
    /*00004078*/ STR         R0, [SP, #16] 
    /*0000407c*/ BL          0x000000000000407C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00004080*/ LDR         R3, [SL, #12] 
    /*00004084*/ CMP         R3, #0 
    /*00004088*/ BEQ         AIDmaCallback_x4214
    /*0000408c*/ LDR         R3, [SL, #44]               @ 0x2c 
    /*00004090*/ CMP         R3, #0 
    /*00004094*/ BEQ         AIDmaCallback_x4228
    /*00004098*/ LSL         R3, R7, #1 
    /*0000409c*/ MOV         R4, #1120                   @ 0x460 
    /*000040a0*/ LDR         R2, [PC, #1212]             @ 0x0000000000004564 
    /*000040a4*/ MOV         R8, #0 
    /*000040a8*/ MUL         R3, R4, R3 
    /*000040ac*/ MOV         FP, SL 
    /*000040b0*/ ADD         R9, R2, R3 
    /*000040b4*/ STR         R3, [SP, #8] 
    AIDmaCallback_x40b8:
    /*000040b8*/ LDR         R4, [R9, #216]              @ 0xd8 
    /*000040bc*/ MOV         R0, #22 
    /*000040c0*/ CMN         R4, #1 
    /*000040c4*/ BEQ         AIDmaCallback_x40e0
    /*000040c8*/ BL          0x00000000000040C8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000040cc*/ SUBS        R3, R0, #0 
    /*000040d0*/ MOV         R0, R4 
    /*000040d4*/ BEQ         AIDmaCallback_x43bc
    /*000040d8*/ LDR         R3, [R3, #32] 
    /*000040dc*/ BLX         R3 
    AIDmaCallback_x40e0:
    /*000040e0*/ LDR         R3, [FP, #44]               @ 0x2c 
    /*000040e4*/ ADD         R8, R8, #1 
    /*000040e8*/ ADD         R9, R9, #1120               @ 0x460 
    /*000040ec*/ CMP         R3, R8 
    /*000040f0*/ BHI         AIDmaCallback_x40b8
    /*000040f4*/ CMP         R3, #0 
    /*000040f8*/ BEQ         AIDmaCallback_x4228
    /*000040fc*/ MOV         R8, #136                    @ 0x88 
    /*00004100*/ LDR         R3, [SP, #8] 
    /*00004104*/ MLA         R8, R8, R7, R5 
    /*00004108*/ ADD         R4, R3, #16 
    /*0000410c*/ LDR         R3, [PC, #1104]             @ 0x0000000000004564 
    /*00004110*/ MOV         R9, #0 
    /*00004114*/ ADD         FP, R3, R4 
    /*00004118*/ MOV         R4, SL 
    AIDmaCallback_x411c:
    /*0000411c*/ LDR         R3, [FP, #224]              @ 0xe0 
    /*00004120*/ MOV         R2, #1 
    /*00004124*/ CMP         R3, #0 
    /*00004128*/ BNE         AIDmaCallback_x413c
    /*0000412c*/ LDR         R3, [FP, #548]              @ 0x224 
    /*00004130*/ CMP         R3, #0 
    /*00004134*/ STREQ       R3, [FP, #120]              @ 0x78 
    /*00004138*/ BEQ         AIDmaCallback_x4140
    AIDmaCallback_x413c:
    /*0000413c*/ STR         R2, [FP, #120]              @ 0x78 
    AIDmaCallback_x4140:
    /*00004140*/ LDR         R2, [FP, #192]              @ 0xc0 
    /*00004144*/ MOV         R3, #0 
    /*00004148*/ STR         R3, [FP, #128]              @ 0x80 
    /*0000414c*/ CMP         R2, #1 
    /*00004150*/ STR         R3, [R4, #84]               @ 0x54 
    /*00004154*/ STR         R3, [FP, #112]              @ 0x70 
    /*00004158*/ STR         R3, [FP, #116]              @ 0x74 
    /*0000415c*/ STR         R3, [FP, #44]               @ 0x2c 
    /*00004160*/ STR         R3, [FP, #100]              @ 0x64 
    /*00004164*/ BEQ         AIDmaCallback_x4290
    AIDmaCallback_x4168:
    /*00004168*/ LDR         R2, [R8, #44]               @ 0x2c 
    /*0000416c*/ ADD         R9, R9, #1 
    /*00004170*/ ADD         R4, R4, #16 
    /*00004174*/ ADD         FP, FP, #1120               @ 0x460 
    /*00004178*/ CMP         R2, R9 
    /*0000417c*/ BHI         AIDmaCallback_x411c
    AIDmaCallback_x4180:
    /*00004180*/ ADD         R1, R6, #72                 @ 0x48 
    /*00004184*/ MOV         R0, R7 
    /*00004188*/ ADD         R1, R5, R1 
    /*0000418c*/ BL          0x000000000000418C          @ AI_DRV_GetChnData [R_ARM_CALL: .text+0x8810] 
    /*00004190*/ SUBS        R6, R0, #0 
    /*00004194*/ BEQ         AIDmaCallback_x4230
    /*00004198*/ MOV         R3, #136                    @ 0x88 
    /*0000419c*/ MLA         R5, R3, R7, R5 
    /*000041a0*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*000041a4*/ CMP         R3, #0 
    /*000041a8*/ BEQ         AIDmaCallback_x41e8
    /*000041ac*/ LDR         R3, [PC, #944]              @ 0x0000000000004564 
    /*000041b0*/ LSL         R4, R7, #1 
    /*000041b4*/ MOV         R2, #1120                   @ 0x460 
    /*000041b8*/ MOV         R6, #0 
    /*000041bc*/ MOV         R8, R6 
    /*000041c0*/ MLA         R4, R2, R4, R3 
    /*000041c4*/ ADD         R4, R4, #16 
    AIDmaCallback_x41c8:
    /*000041c8*/ LDR         R3, [R4, #128]              @ 0x80 
    /*000041cc*/ ADD         R6, R6, #1 
    /*000041d0*/ CMP         R3, #1 
    /*000041d4*/ BEQ         AIDmaCallback_x42c8
    AIDmaCallback_x41d8:
    /*000041d8*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*000041dc*/ ADD         R4, R4, #1120               @ 0x460 
    /*000041e0*/ CMP         R3, R6 
    /*000041e4*/ BHI         AIDmaCallback_x41c8
    AIDmaCallback_x41e8:
    /*000041e8*/ MOVW        R3, #438                    @ 0x1b6 
    /*000041ec*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfe4] 
    /*000041f0*/ STR         R3, [SP] 
    /*000041f4*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfe4] 
    /*000041f8*/ STR         R7, [SP, #4] 
    /*000041fc*/ MOV         R1, #21 
    /*00004200*/ LDR         R3, [PC, #864]              @ 0x0000000000004568 
    /*00004204*/ MOV         R0, #3 
    /*00004208*/ BL          0x0000000000004208          @ HI_LOG [R_ARM_CALL: +0x0] 
    AIDmaCallback_x420c:
    /*0000420c*/ MOV         R0, R7 
    /*00004210*/ BL          0x0000000000003C6C 
    AIDmaCallback_x4214:
    /*00004214*/ ADD         R1, SP, #28 
    /*00004218*/ LDR         R0, [SP, #16] 
    /*0000421c*/ BL          0x000000000000421C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00004220*/ ADD         SP, SP, #36                 @ 0x24 
    /*00004224*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIDmaCallback_x4228:
    /*00004228*/ MOV         R2, #0 
    /*0000422c*/ B           AIDmaCallback_x4180
    AIDmaCallback_x4230:
    /*00004230*/ MOV         R0, #2 
    /*00004234*/ BL          0x0000000000004234          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00004238*/ LDR         R3, [R0] 
    /*0000423c*/ BLX         R3 
    /*00004240*/ MOV         R3, #136                    @ 0x88 
    /*00004244*/ MLA         R5, R3, R7, R5 
    /*00004248*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*0000424c*/ CMP         R3, #0 
    /*00004250*/ STRD        R0, [SP, #8] 
    /*00004254*/ BEQ         AIDmaCallback_x420c
    /*00004258*/ LDR         R3, [PC, #772]              @ 0x0000000000004564 
    /*0000425c*/ LSL         R4, R7, #1 
    /*00004260*/ MOV         R2, #1120                   @ 0x460 
    /*00004264*/ MLA         R4, R2, R4, R3 
    /*00004268*/ ADD         R4, R4, #72                 @ 0x48 
    AIDmaCallback_x426c:
    /*0000426c*/ LDR         R3, [R4, #72]               @ 0x48 
    /*00004270*/ CMP         R3, #1 
    /*00004274*/ BEQ         AIDmaCallback_x42e4
    AIDmaCallback_x4278:
    /*00004278*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*0000427c*/ ADD         R6, R6, #1 
    /*00004280*/ ADD         R4, R4, #1120               @ 0x460 
    /*00004284*/ CMP         R3, R6 
    /*00004288*/ BHI         AIDmaCallback_x426c
    /*0000428c*/ B           AIDmaCallback_x420c
    AIDmaCallback_x4290:
    /*00004290*/ MOV         R1, FP 
    /*00004294*/ MOV         R0, SL 
    /*00004298*/ STR         R2, [SP, #8] 
    /*0000429c*/ BL          0x000000000000383C 
    /*000042a0*/ LDR         R2, [SP, #8] 
    /*000042a4*/ CMP         R0, #0 
    /*000042a8*/ BNE         AIDmaCallback_x4428
    /*000042ac*/ LDRD        R0, [FP, #16] 
    /*000042b0*/ LDR         R3, [FP, #8] 
    /*000042b4*/ STR         R2, [FP, #128]              @ 0x80 
    /*000042b8*/ STR         R2, [R4, #84]               @ 0x54 
    /*000042bc*/ STRD        R0, [R4, #72]               @ 0x48 
    /*000042c0*/ STR         R3, [R4, #80]               @ 0x50 
    /*000042c4*/ B           AIDmaCallback_x4168
    AIDmaCallback_x42c8:
    /*000042c8*/ MOV         R0, R4 
    /*000042cc*/ BL          0x000000000000390C 
    /*000042d0*/ LDR         R3, [R4, #212]              @ 0xd4 
    /*000042d4*/ STR         R8, [R4, #128]              @ 0x80 
    /*000042d8*/ ADD         R3, R3, #1 
    /*000042dc*/ STR         R3, [R4, #212]              @ 0xd4 
    /*000042e0*/ B           AIDmaCallback_x41d8
    AIDmaCallback_x42e4:
    /*000042e4*/ LDR         R1, [R5, #32] 
    /*000042e8*/ LDR         R2, [R4, #-16] 
    /*000042ec*/ CMP         R1, #0 
    /*000042f0*/ LDRD        R0, [SP, #8] 
    /*000042f4*/ ADD         R2, R2, #1 
    /*000042f8*/ STR         R2, [R4, #-16] 
    /*000042fc*/ LDR         R2, [R5, #20] 
    /*00004300*/ STRD        R0, [R4, #-24]              @ 0xffffffe8 
    /*00004304*/ BNE         AIDmaCallback_x43e4
    AIDmaCallback_x4308:
    /*00004308*/ LDR         R3, [R5, #40]               @ 0x28 
    /*0000430c*/ STR         R2, [R4, #-56]              @ 0xffffffc8 
    /*00004310*/ LSL         R2, R3, R2 
    /*00004314*/ STR         R2, [R4, #-12] 
    AIDmaCallback_x4318:
    /*00004318*/ LDR         R8, [R4, #144]              @ 0x90 
    /*0000431c*/ CMN         R8, #1 
    /*00004320*/ BEQ         AIDmaCallback_x4278
    /*00004324*/ MOV         R0, #22 
    /*00004328*/ LDR         FP, [R4, #148]              @ 0x94 
    /*0000432c*/ BL          0x000000000000432C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00004330*/ SUBS        R9, R0, #0 
    /*00004334*/ BEQ         AIDmaCallback_x4400
    /*00004338*/ LDR         R3, [R9, #24] 
    /*0000433c*/ MOV         R0, R8 
    /*00004340*/ BLX         R3 
    /*00004344*/ CMP         R0, #0 
    /*00004348*/ BEQ         AIDmaCallback_x4278
    /*0000434c*/ LDR         R3, [R0, #12] 
    /*00004350*/ CMP         R3, #1 
    /*00004354*/ BNE         AIDmaCallback_x4278
    /*00004358*/ LDR         R3, [R4, #164]              @ 0xa4 
    /*0000435c*/ CMP         R3, #1 
    /*00004360*/ BEQ         AIDmaCallback_x44b0
    AIDmaCallback_x4364:
    /*00004364*/ LDR         R3, [R4, #1028]             @ 0x404 
    /*00004368*/ CMP         R3, #1 
    /*0000436c*/ BNE         AIDmaCallback_x4278
    AIDmaCallback_x4370:
    /*00004370*/ MOV         R1, R4 
    /*00004374*/ MOV         R0, SL 
    /*00004378*/ BL          0x000000000000383C 
    /*0000437c*/ SUBS        R3, R0, #0 
    /*00004380*/ STR         R3, [SP, #20] 
    /*00004384*/ BEQ         AIDmaCallback_x445c
    /*00004388*/ LDR         IP, [R4, #152]              @ 0x98 
    /*0000438c*/ MOV         R3, #496                    @ 0x1f0 
    /*00004390*/ STR         R6, [SP, #4] 
    /*00004394*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a8] 
    /*00004398*/ STR         R3, [SP] 
    /*0000439c*/ ADD         IP, IP, #1 
    /*000043a0*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a8] 
    /*000043a4*/ LDR         R3, [PC, #444]              @ 0x0000000000004568 
    /*000043a8*/ MOV         R1, #21 
    /*000043ac*/ STR         IP, [R4, #152]              @ 0x98 
    /*000043b0*/ MOV         R0, #3 
    /*000043b4*/ BL          0x00000000000043B4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000043b8*/ B           AIDmaCallback_x4278
    AIDmaCallback_x43bc:
    /*000043bc*/ MOV         R3, #360                    @ 0x168 
    /*000043c0*/ STR         R8, [SP, #4] 
    /*000043c4*/ STR         R3, [SP] 
    /*000043c8*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf68] 
    /*000043cc*/ LDR         R3, [PC, #404]              @ 0x0000000000004568 
    /*000043d0*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf68] 
    /*000043d4*/ MOV         R1, #21 
    /*000043d8*/ MOV         R0, #4 
    /*000043dc*/ BL          0x00000000000043DC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000043e0*/ B           AIDmaCallback_x40e0
    AIDmaCallback_x43e4:
    /*000043e4*/ CMP         R2, #0 
    /*000043e8*/ BNE         AIDmaCallback_x4308
    /*000043ec*/ LDR         R2, [R5, #40]               @ 0x28 
    /*000043f0*/ STR         R3, [R4, #-56]              @ 0xffffffc8 
    /*000043f4*/ LSL         R3, R2, #1 
    /*000043f8*/ STR         R3, [R4, #-12] 
    /*000043fc*/ B           AIDmaCallback_x4318
    AIDmaCallback_x4400:
    /*00004400*/ MOVW        R3, #482                    @ 0x1e2 
    /*00004404*/ STR         R6, [SP, #4] 
    /*00004408*/ STR         R3, [SP] 
    /*0000440c*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf68] 
    /*00004410*/ LDR         R3, [PC, #336]              @ 0x0000000000004568 
    /*00004414*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf68] 
    /*00004418*/ MOV         R1, #21 
    /*0000441c*/ MOV         R0, #4 
    /*00004420*/ BL          0x0000000000004420          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004424*/ B           AIDmaCallback_x4278
    AIDmaCallback_x4428:
    /*00004428*/ LDR         IP, [FP, #212]              @ 0xd4 
    /*0000442c*/ MOVW        R3, #405                    @ 0x195 
    /*00004430*/ STR         R9, [SP, #4] 
    /*00004434*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfac] 
    /*00004438*/ STR         R3, [SP] 
    /*0000443c*/ ADD         IP, IP, #1 
    /*00004440*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfac] 
    /*00004444*/ LDR         R3, [PC, #284]              @ 0x0000000000004568 
    /*00004448*/ MOV         R1, #21 
    /*0000444c*/ STR         IP, [FP, #212]              @ 0xd4 
    /*00004450*/ MOV         R0, #4 
    /*00004454*/ BL          0x0000000000004454          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004458*/ B           AIDmaCallback_x4168
    AIDmaCallback_x445c:
    /*0000445c*/ LDR         R2, [R4, #-56]              @ 0xffffffc8 
    /*00004460*/ MOV         R1, FP 
    /*00004464*/ LDR         R3, [R4, #-52]              @ 0xffffffcc 
    /*00004468*/ MOV         R0, R8 
    /*0000446c*/ LDR         IP, [R4, #-12] 
    /*00004470*/ STR         R2, [R4] 
    /*00004474*/ MOV         R2, R4 
    /*00004478*/ STR         R3, [R4, #4] 
    /*0000447c*/ STR         IP, [R4, #44]               @ 0x2c 
    /*00004480*/ LDR         R3, [R9, #28] 
    /*00004484*/ BLX         R3 
    /*00004488*/ CMP         R0, #0 
    /*0000448c*/ BNE         AIDmaCallback_x44f0
    /*00004490*/ LDRD        R2, [R4, #-24]              @ 0xffffffe8 
    /*00004494*/ MOV         R1, #1 
    /*00004498*/ LDR         R0, [R4, #-16] 
    /*0000449c*/ STR         R1, [R4, #56]               @ 0x38 
    /*000044a0*/ STRD        R2, [R4, #32] 
    /*000044a4*/ STR         R0, [R4, #40]               @ 0x28 
    /*000044a8*/ STR         R1, [R4, #60]               @ 0x3c 
    /*000044ac*/ B           AIDmaCallback_x4278
    AIDmaCallback_x44b0:
    /*000044b0*/ LDR         R3, [R0, #20] 
    /*000044b4*/ CMP         R3, #1 
    /*000044b8*/ BHI         AIDmaCallback_x453c
    /*000044bc*/ LDR         R3, [R0, #44]               @ 0x2c 
    /*000044c0*/ CMP         R3, FP 
    /*000044c4*/ BHI         AIDmaCallback_x4370
    /*000044c8*/ MOV         R3, #49                     @ 0x31 
    /*000044cc*/ STR         FP, [SP, #4] 
    /*000044d0*/ STR         R3, [SP] 
    /*000044d4*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x106c] 
    /*000044d8*/ LDR         R3, [PC, #140]              @ 0x000000000000456c 
    /*000044dc*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x106c] 
    /*000044e0*/ MOV         R1, #21 
    /*000044e4*/ MOV         R0, #4 
    /*000044e8*/ BL          0x00000000000044E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000044ec*/ B           AIDmaCallback_x4364
    AIDmaCallback_x44f0:
    /*000044f0*/ MOV         R0, R4 
    /*000044f4*/ BL          0x000000000000390C 
    /*000044f8*/ LDR         R0, [SP, #20] 
    /*000044fc*/ LDR         IP, [R4, #152]              @ 0x98 
    /*00004500*/ MOVW        R3, #517                    @ 0x205 
    /*00004504*/ STR         R6, [SP, #4] 
    /*00004508*/ MOVW        R2, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10e4] 
    /*0000450c*/ STR         R3, [SP] 
    /*00004510*/ ADD         IP, IP, #1 
    /*00004514*/ STR         R0, [R4, #44]               @ 0x2c 
    /*00004518*/ MOVT        R2, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10e4] 
    /*0000451c*/ STR         R0, [R4, #56]               @ 0x38 
    /*00004520*/ MOV         R1, #21 
    /*00004524*/ STR         R0, [R4, #60]               @ 0x3c 
    /*00004528*/ MOV         R0, #3 
    /*0000452c*/ LDR         R3, [PC, #52]               @ 0x0000000000004568 
    /*00004530*/ STR         IP, [R4, #152]              @ 0x98 
    /*00004534*/ BL          0x0000000000004534          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004538*/ B           AIDmaCallback_x4278
    AIDmaCallback_x453c:
    /*0000453c*/ STR         R3, [SP, #4] 
    /*00004540*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1038] 
    /*00004544*/ MOV         R3, #43                     @ 0x2b 
    /*00004548*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1038] 
    /*0000454c*/ STR         R3, [SP] 
    /*00004550*/ MOV         R1, #21 
    /*00004554*/ LDR         R3, [PC, #16]               @ 0x000000000000456c 
    /*00004558*/ MOV         R0, #4 
    /*0000455c*/ BL          0x000000000000455C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004560*/ B           AIDmaCallback_x4364
    /*00004564*/ MULEQ       R0, R0, R0 
    /*00004568*/ ANDEQ       R0, R0, R4, LSR #3 
    /*0000456c*/ @           <UNDEFINED> instruction: 0x000001b4 
FUNC_END AIDmaCallback


FUNC_BEGIN AIDmaBufInit
    /*00004570*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00004574*/ SUB         SP, SP, #52                 @ 0x34 
    /*00004578*/ ADD         R1, SP, #12 
    /*0000457c*/ MOV         R3, #0 
    /*00004580*/ MOV         R7, R0 
    /*00004584*/ STR         R3, [SP, #36]               @ 0x24 
    /*00004588*/ STR         R3, [SP, #32] 
    /*0000458c*/ STR         R3, [SP, #40]               @ 0x28 
    /*00004590*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00004594*/ STR         R3, [SP, #16] 
    /*00004598*/ BL          0x0000000000004598          @ AI_DRV_DmaBufInfoInit [R_ARM_CALL: .text+0x83d0] 
    /*0000459c*/ SUBS        R5, R0, #0 
    /*000045a0*/ BNE         AIDmaBufInit_x465c
    /*000045a4*/ MOV         R0, #2 
    /*000045a8*/ STR         R7, [SP, #24] 
    /*000045ac*/ STR         R5, [SP, #28] 
    /*000045b0*/ MOV         R8, #21 
    /*000045b4*/ STR         R8, [SP, #20] 
    /*000045b8*/ BL          0x00000000000045B8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000045bc*/ ADD         R1, SP, #16 
    /*000045c0*/ LDR         R3, [R0, #60]               @ 0x3c 
    /*000045c4*/ ADD         R0, SP, #20 
    /*000045c8*/ BLX         R3 
    /*000045cc*/ SUBS        R9, R0, #0 
    /*000045d0*/ BNE         AIDmaBufInit_x4688
    /*000045d4*/ MOV         R6, #136                    @ 0x88 
    /*000045d8*/ MOV         R3, R7 
    /*000045dc*/ MUL         R6, R6, R7 
    /*000045e0*/ MOV         R1, #16 
    /*000045e4*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x118c] 
    /*000045e8*/ ADD         R0, SP, #32 
    /*000045ec*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x118c] 
    /*000045f0*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*000045f4*/ BL          0x00000000000045F4          @ osal_snprintf [R_ARM_CALL: +0x0] 
    /*000045f8*/ LDR         IP, [SP, #12] 
    /*000045fc*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*00004600*/ ADD         R3, R6, #64                 @ 0x40 
    /*00004604*/ ADD         R2, R6, #56                 @ 0x38 
    /*00004608*/ LDR         R0, [SP, #16] 
    /*0000460c*/ ADD         R1, SP, #32 
    /*00004610*/ ADD         R3, R4, R3 
    /*00004614*/ ADD         R2, R4, R2 
    /*00004618*/ STR         IP, [SP] 
    /*0000461c*/ BL          0x000000000000461C          @ CMPI_MmzMallocNocache [R_ARM_CALL: +0x0] 
    /*00004620*/ SUBS        R5, R0, #0 
    /*00004624*/ BNE         AIDmaBufInit_x46b0
    /*00004628*/ ADD         R4, R4, R6 
    /*0000462c*/ LDR         R2, [SP, #12] 
    /*00004630*/ MOV         R1, R5 
    /*00004634*/ LDR         R0, [R4, #64]               @ 0x40 
    /*00004638*/ BL          0x0000000000004638          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000463c*/ LDR         R1, [R4, #64]               @ 0x40 
    /*00004640*/ LDRD        R2, [R4, #56]               @ 0x38 
    /*00004644*/ MOV         R0, R7 
    /*00004648*/ STR         R1, [SP] 
    /*0000464c*/ BL          0x000000000000464C          @ AI_DRV_SetDmaBufAddr [R_ARM_CALL: .text+0x8544] 
    AIDmaBufInit_x4650:
    /*00004650*/ MOV         R0, R5 
    /*00004654*/ ADD         SP, SP, #52                 @ 0x34 
    /*00004658*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AIDmaBufInit_x465c:
    /*0000465c*/ MOVW        R3, #541                    @ 0x21d 
    /*00004660*/ MOV         R1, #21 
    /*00004664*/ STR         R3, [SP] 
    /*00004668*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1120] 
    /*0000466c*/ LDR         R3, [PC, #104]              @ 0x00000000000046dc 
    /*00004670*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1120] 
    /*00004674*/ MOV         R0, #4 
    /*00004678*/ BL          0x0000000000004678          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000467c*/ MOV         R0, R5 
    /*00004680*/ ADD         SP, SP, #52                 @ 0x34 
    /*00004684*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AIDmaBufInit_x4688:
    /*00004688*/ MOVW        R3, #551                    @ 0x227 
    /*0000468c*/ MOV         R1, R8 
    /*00004690*/ STR         R3, [SP] 
    /*00004694*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x115c] 
    /*00004698*/ LDR         R3, [PC, #60]               @ 0x00000000000046dc 
    /*0000469c*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x115c] 
    /*000046a0*/ MOV         R0, #4 
    /*000046a4*/ MVN         R5, #0 
    /*000046a8*/ BL          0x00000000000046A8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000046ac*/ B           AIDmaBufInit_x4650
    AIDmaBufInit_x46b0:
    /*000046b0*/ MOVW        R3, #559                    @ 0x22f 
    /*000046b4*/ MOV         R1, R8 
    /*000046b8*/ STR         R3, [SP] 
    /*000046bc*/ MOV         R0, R9 
    /*000046c0*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x119c] 
    /*000046c4*/ LDR         R3, [PC, #16]               @ 0x00000000000046dc 
    /*000046c8*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x119c] 
    /*000046cc*/ MOVW        R5, #32780                  @ 0x800c 
    /*000046d0*/ BL          0x00000000000046D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000046d4*/ MOVT        R5, #40981                  @ 0xa015 
    /*000046d8*/ B           AIDmaBufInit_x4650
    /*000046dc*/ ANDEQ       R0, R0, R0, ASR #3 
FUNC_END AIDmaBufInit


FUNC_BEGIN AIDmaBufExit
    /*000046e0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*000046e4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*000046e8*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000046ec*/ MOV         R4, #136                    @ 0x88 
    /*000046f0*/ MLA         R4, R4, R0, R3 
    /*000046f4*/ MOV         R5, R0 
    /*000046f8*/ MOV         R6, #0 
    /*000046fc*/ MOV         R7, #0 
    /*00004700*/ LDRD        R0, [R4, #56]               @ 0x38 
    /*00004704*/ LDR         R2, [R4, #64]               @ 0x40 
    /*00004708*/ BL          0x0000000000004708          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*0000470c*/ MOV         R0, R5 
    /*00004710*/ MOV         R3, #0 
    /*00004714*/ STRD        R6, [R4, #56]               @ 0x38 
    /*00004718*/ STR         R3, [R4, #64]               @ 0x40 
    /*0000471c*/ POP         {R4, R5, R6, R7, R8, LR} 
    AIDmaBufExit_x4720:
    /*00004720*/ B           AIDmaBufExit_x4720          @ AI_DRV_ClrDmaBufAddr [R_ARM_JUMP24: .text+0x856c] 
FUNC_END AIDmaBufExit


FUNC_BEGIN AIVbInit
    /*00004724*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00004728*/ SUB         SP, SP, #48                 @ 0x30 
    /*0000472c*/ LDR         R2, [R1, #4] 
    /*00004730*/ MOV         R3, #0 
    /*00004734*/ MOV         R4, R1 
    /*00004738*/ MOV         R6, R0 
    /*0000473c*/ CMP         R2, #2 
    /*00004740*/ STR         R3, [SP, #36]               @ 0x24 
    /*00004744*/ STR         R3, [SP, #16] 
    /*00004748*/ STR         R3, [SP, #32] 
    /*0000474c*/ STR         R3, [SP, #40]               @ 0x28 
    /*00004750*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00004754*/ BEQ         AIVbInit_x476c
    /*00004758*/ LDR         R1, [R1] 
    /*0000475c*/ MOVW        R3, #48000                  @ 0xbb80 
    /*00004760*/ CMP         R1, R3 
    /*00004764*/ CMPLS       R2, #1 
    /*00004768*/ BEQ         AIVbInit_x4828
    AIVbInit_x476c:
    /*0000476c*/ LDR         R5, [R4, #24] 
    /*00004770*/ LSL         R5, R5, #2 
    AIVbInit_x4774:
    /*00004774*/ LDR         R1, [R4, #28] 
    /*00004778*/ MOV         R0, #2 
    /*0000477c*/ LDR         R4, [R4, #20] 
    /*00004780*/ MOV         R3, #0 
    /*00004784*/ MOV         R8, #21 
    /*00004788*/ STR         R3, [SP, #28] 
    /*0000478c*/ LSL         R1, R1, #1 
    /*00004790*/ STR         R6, [SP, #24] 
    /*00004794*/ STR         R8, [SP, #20] 
    /*00004798*/ MLA         R4, R4, R1, R1 
    /*0000479c*/ BL          0x000000000000479C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000047a0*/ ADD         R1, SP, #16 
    /*000047a4*/ LDR         R3, [R0, #60]               @ 0x3c 
    /*000047a8*/ ADD         R0, SP, #20 
    /*000047ac*/ BLX         R3 
    /*000047b0*/ SUBS        R9, R0, #0 
    /*000047b4*/ BNE         AIVbInit_x4848
    /*000047b8*/ MOV         R3, R6 
    /*000047bc*/ MOV         R1, #16 
    /*000047c0*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11cc] 
    /*000047c4*/ ADD         R0, SP, #32 
    /*000047c8*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11cc] 
    /*000047cc*/ MOVW        R7, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*000047d0*/ BL          0x00000000000047D0          @ osal_snprintf [R_ARM_CALL: +0x0] 
    /*000047d4*/ MOV         R0, #1 
    /*000047d8*/ BL          0x00000000000047D8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000047dc*/ MOV         R3, #136                    @ 0x88 
    /*000047e0*/ MUL         R6, R3, R6 
    /*000047e4*/ LDR         R3, [SP, #16] 
    /*000047e8*/ MOVT        R7, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*000047ec*/ ADD         R2, SP, #32 
    /*000047f0*/ MOV         R1, #1 
    /*000047f4*/ STR         R2, [SP, #4] 
    /*000047f8*/ STR         R3, [SP] 
    /*000047fc*/ MOV         R2, R5 
    /*00004800*/ STR         R1, [SP, #8] 
    /*00004804*/ MOV         R3, R9 
    /*00004808*/ MOV         R1, R4 
    /*0000480c*/ LDR         SL, [R0] 
    /*00004810*/ ADD         R0, R7, R6 
    /*00004814*/ BLX         SL 
    /*00004818*/ CMP         R0, #0 
    /*0000481c*/ BNE         AIVbInit_x4870
    AIVbInit_x4820:
    /*00004820*/ ADD         SP, SP, #48                 @ 0x30 
    /*00004824*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AIVbInit_x4828:
    /*00004828*/ LDR         R0, [R4, #24] 
    /*0000482c*/ MUL         R0, R3, R0 
    /*00004830*/ BL          0x0000000000004830          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00004834*/ ADD         R0, R0, #1 
    /*00004838*/ CMP         R0, #2048                   @ 0x800 
    /*0000483c*/ LSLLS       R5, R0, #1 
    /*00004840*/ MOVHI       R5, #4096                   @ 0x1000 
    /*00004844*/ B           AIVbInit_x4774
    AIVbInit_x4848:
    /*00004848*/ MOVW        R3, #633                    @ 0x279 
    /*0000484c*/ MOV         R1, R8 
    /*00004850*/ STR         R3, [SP] 
    /*00004854*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x115c] 
    /*00004858*/ LDR         R3, [PC, #80]               @ 0x00000000000048b0 
    /*0000485c*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x115c] 
    /*00004860*/ MOV         R0, #4 
    /*00004864*/ BL          0x0000000000004864          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004868*/ MVN         R0, #0 
    /*0000486c*/ B           AIVbInit_x4820
    AIVbInit_x4870:
    /*00004870*/ STR         R0, [SP, #12] 
    /*00004874*/ MOVW        R3, #642                    @ 0x282 
    /*00004878*/ MOV         R1, R8 
    /*0000487c*/ STR         R3, [SP] 
    /*00004880*/ STR         R4, [SP, #8] 
    /*00004884*/ MOV         R0, R9 
    /*00004888*/ LDR         R3, [PC, #32]               @ 0x00000000000048b0 
    /*0000488c*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11dc] 
    /*00004890*/ STR         R5, [SP, #4] 
    /*00004894*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11dc] 
    /*00004898*/ BL          0x0000000000004898          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000489c*/ MOVW        R0, #32780                  @ 0x800c 
    /*000048a0*/ MVN         R3, #0 
    /*000048a4*/ MOVT        R0, #40981                  @ 0xa015 
    /*000048a8*/ STR         R3, [R7, R6] 
    /*000048ac*/ B           AIVbInit_x4820
    /*000048b0*/ LDRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
FUNC_END AIVbInit


FUNC_BEGIN AIVbFree
    /*000048b4*/ PUSH        {R4, R5, R6, LR} 
    /*000048b8*/ MOV         R4, #136                    @ 0x88 
    /*000048bc*/ MUL         R4, R4, R0 
    /*000048c0*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*000048c4*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*000048c8*/ SUB         SP, SP, #8 
    /*000048cc*/ LDR         R3, [R5, R4] 
    /*000048d0*/ CMN         R3, #1 
    /*000048d4*/ BEQ         AIVbFree_x4938
    /*000048d8*/ MOV         R0, #1 
    /*000048dc*/ BL          0x00000000000048DC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000048e0*/ LDR         R3, [R0, #4] 
    /*000048e4*/ LDR         R0, [R5, R4] 
    /*000048e8*/ BLX         R3 
    /*000048ec*/ SUBS        R6, R0, #0 
    /*000048f0*/ MVNEQ       R3, #0 
    /*000048f4*/ STREQ       R3, [R5, R4] 
    /*000048f8*/ BNE         AIVbFree_x4908
    AIVbFree_x48fc:
    /*000048fc*/ MOV         R0, R6 
    /*00004900*/ ADD         SP, SP, #8 
    /*00004904*/ POP         {R4, R5, R6, PC} 
    AIVbFree_x4908:
    /*00004908*/ STR         R6, [SP, #4] 
    /*0000490c*/ MOV         R3, #668                    @ 0x29c 
    /*00004910*/ MOV         R1, #21 
    /*00004914*/ STR         R3, [SP] 
    /*00004918*/ MOVW        R2, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1274] 
    /*0000491c*/ LDR         R3, [PC, #64]               @ 0x0000000000004964 
    /*00004920*/ MOVT        R2, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1274] 
    /*00004924*/ MOV         R0, #3 
    /*00004928*/ BL          0x0000000000004928          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000492c*/ MOV         R0, R6 
    /*00004930*/ ADD         SP, SP, #8 
    /*00004934*/ POP         {R4, R5, R6, PC} 
    AIVbFree_x4938:
    /*00004938*/ MOVW        R3, #661                    @ 0x295 
    /*0000493c*/ MOVW        R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1238] 
    /*00004940*/ STR         R3, [SP] 
    /*00004944*/ MOVT        R2, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1238] 
    /*00004948*/ LDR         R3, [PC, #20]               @ 0x0000000000004964 
    /*0000494c*/ MOV         R1, #21 
    /*00004950*/ MOV         R0, #3 
    /*00004954*/ MOVW        R6, #32781                  @ 0x800d 
    /*00004958*/ BL          0x0000000000004958          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000495c*/ MOVT        R6, #40981                  @ 0xa015 
    /*00004960*/ B           AIVbFree_x48fc
    /*00004964*/ LDRDEQ      R0, [R0], -IP 
FUNC_END AIVbFree


FUNC_BEGIN AiSetDevAttr
    /*00004968*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000496c*/ MOV         R4, R0 
    /*00004970*/ SUB         SP, SP, #8 
    /*00004974*/ MOV         R0, #20 
    /*00004978*/ MOV         R5, R1 
    /*0000497c*/ BL          0x000000000000497C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00004980*/ CMP         R0, #0 
    /*00004984*/ BEQ         AiSetDevAttr_x4abc
    /*00004988*/ CMP         R4, #0 
    /*0000498c*/ BNE         AiSetDevAttr_x4a68
    /*00004990*/ CMP         R5, #0 
    /*00004994*/ BEQ         AiSetDevAttr_x4a5c
    /*00004998*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*0000499c*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*000049a0*/ ADD         R0, R4, #124                @ 0x7c 
    /*000049a4*/ BL          0x00000000000049A4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000049a8*/ SUBS        R8, R0, #0 
    /*000049ac*/ BNE         AiSetDevAttr_x4a18
    /*000049b0*/ LDR         R7, [R4, #4] 
    /*000049b4*/ CMP         R7, #0 
    /*000049b8*/ BEQ         AiSetDevAttr_x4a98
    /*000049bc*/ LDR         R3, [R4, #12] 
    /*000049c0*/ CMP         R3, #0 
    /*000049c4*/ BNE         AiSetDevAttr_x4a24
    /*000049c8*/ MOV         R1, R5 
    /*000049cc*/ BL          0x00000000000049CC          @ AI_DRV_SetDevAttr [R_ARM_CALL: .text+0x85b0] 
    /*000049d0*/ SUBS        R6, R0, #0 
    /*000049d4*/ BNE         AiSetDevAttr_x4a04
    /*000049d8*/ MOV         R1, R5 
    /*000049dc*/ MOV         R2, #40                     @ 0x28 
    /*000049e0*/ ADD         R0, R4, #16 
    /*000049e4*/ BL          0x00000000000049E4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000049e8*/ MOV         R3, #1 
    /*000049ec*/ ADD         R0, R4, #124                @ 0x7c 
    /*000049f0*/ STR         R3, [R4, #8] 
    /*000049f4*/ BL          0x00000000000049F4          @ osal_up [R_ARM_CALL: +0x0] 
    AiSetDevAttr_x49f8:
    /*000049f8*/ MOV         R0, R6 
    /*000049fc*/ ADD         SP, SP, #8 
    /*00004a00*/ POP         {R4, R5, R6, R7, R8, PC} 
    AiSetDevAttr_x4a04:
    /*00004a04*/ ADD         R0, R4, #124                @ 0x7c 
    /*00004a08*/ BL          0x0000000000004A08          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004a0c*/ MOV         R0, R6 
    /*00004a10*/ ADD         SP, SP, #8 
    /*00004a14*/ POP         {R4, R5, R6, R7, R8, PC} 
    AiSetDevAttr_x4a18:
    /*00004a18*/ MOV         R6, #65024                  @ 0xfe00 
    /*00004a1c*/ MOVT        R6, #65535                  @ 0xffff 
    /*00004a20*/ B           AiSetDevAttr_x49f8
    AiSetDevAttr_x4a24:
    /*00004a24*/ ADD         R0, R4, #124                @ 0x7c 
    /*00004a28*/ MOVW        R6, #32777                  @ 0x8009 
    /*00004a2c*/ BL          0x0000000000004A2C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004a30*/ MOVW        R2, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1320] 
    /*00004a34*/ MOVT        R2, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1320] 
    /*00004a38*/ MOVW        R3, #702                    @ 0x2be 
    /*00004a3c*/ STR         R8, [SP, #4] 
    /*00004a40*/ STR         R3, [SP] 
    AiSetDevAttr_x4a44:
    /*00004a44*/ LDR         R3, [PC, #124]              @ 0x0000000000004ac8 
    /*00004a48*/ MOV         R1, #21 
    /*00004a4c*/ MOV         R0, #3 
    /*00004a50*/ MOVT        R6, #40981                  @ 0xa015 
    /*00004a54*/ BL          0x0000000000004A54          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004a58*/ B           AiSetDevAttr_x49f8
    AiSetDevAttr_x4a5c:
    /*00004a5c*/ MOVW        R6, #32774                  @ 0x8006 
    /*00004a60*/ MOVT        R6, #40981                  @ 0xa015 
    /*00004a64*/ B           AiSetDevAttr_x49f8
    AiSetDevAttr_x4a68:
    /*00004a68*/ MOVW        R3, #683                    @ 0x2ab 
    /*00004a6c*/ STR         R4, [SP, #4] 
    /*00004a70*/ STR         R3, [SP] 
    /*00004a74*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12b8] 
    /*00004a78*/ LDR         R3, [PC, #72]               @ 0x0000000000004ac8 
    /*00004a7c*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12b8] 
    /*00004a80*/ MOV         R1, #21 
    /*00004a84*/ MOV         R0, #3 
    /*00004a88*/ MOVW        R6, #32769                  @ 0x8001 
    /*00004a8c*/ BL          0x0000000000004A8C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004a90*/ MOVT        R6, #40981                  @ 0xa015 
    /*00004a94*/ B           AiSetDevAttr_x49f8
    AiSetDevAttr_x4a98:
    /*00004a98*/ ADD         R0, R4, #124                @ 0x7c 
    /*00004a9c*/ MOVW        R6, #32777                  @ 0x8009 
    /*00004aa0*/ BL          0x0000000000004AA0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004aa4*/ MOVW        R3, #694                    @ 0x2b6 
    /*00004aa8*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12ec] 
    /*00004aac*/ STR         R7, [SP, #4] 
    /*00004ab0*/ STR         R3, [SP] 
    /*00004ab4*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12ec] 
    /*00004ab8*/ B           AiSetDevAttr_x4a44
    AiSetDevAttr_x4abc:
    /*00004abc*/ MOVW        R6, #32784                  @ 0x8010 
    /*00004ac0*/ MOVT        R6, #40981                  @ 0xa015 
    /*00004ac4*/ B           AiSetDevAttr_x49f8
    /*00004ac8*/ ANDEQ       R0, R0, R8, ROR #3 
FUNC_END AiSetDevAttr


FUNC_BEGIN AiGetDevAttr
    /*00004acc*/ PUSH        {R4, R5, R6, LR} 
    /*00004ad0*/ MOV         R4, R0 
    /*00004ad4*/ SUB         SP, SP, #8 
    /*00004ad8*/ MOV         R0, #20 
    /*00004adc*/ MOV         R6, R1 
    /*00004ae0*/ BL          0x0000000000004AE0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00004ae4*/ CMP         R0, #0 
    /*00004ae8*/ BEQ         AiGetDevAttr_x4c08
    /*00004aec*/ CMP         R4, #0 
    /*00004af0*/ BNE         AiGetDevAttr_x4ba0
    /*00004af4*/ CMP         R6, #0 
    /*00004af8*/ BEQ         AiGetDevAttr_x4b94
    /*00004afc*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*00004b00*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*00004b04*/ ADD         R0, R4, #124                @ 0x7c 
    /*00004b08*/ BL          0x0000000000004B08          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00004b0c*/ SUBS        R5, R0, #0 
    /*00004b10*/ BNE         AiGetDevAttr_x4b88
    /*00004b14*/ LDR         R3, [R4, #4] 
    /*00004b18*/ CMP         R3, #0 
    /*00004b1c*/ BEQ         AiGetDevAttr_x4bd0
    /*00004b20*/ LDR         R3, [R4, #8] 
    /*00004b24*/ CMP         R3, #1 
    /*00004b28*/ BNE         AiGetDevAttr_x4b50
    /*00004b2c*/ MOV         R2, #40                     @ 0x28 
    /*00004b30*/ ADD         R1, R4, #16 
    /*00004b34*/ MOV         R0, R6 
    /*00004b38*/ BL          0x0000000000004B38          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004b3c*/ ADD         R0, R4, #124                @ 0x7c 
    /*00004b40*/ BL          0x0000000000004B40          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004b44*/ MOV         R0, R5 
    AiGetDevAttr_x4b48:
    /*00004b48*/ ADD         SP, SP, #8 
    /*00004b4c*/ POP         {R4, R5, R6, PC} 
    AiGetDevAttr_x4b50:
    /*00004b50*/ MOVW        R3, #742                    @ 0x2e6 
    /*00004b54*/ MOV         R1, #21 
    /*00004b58*/ STR         R3, [SP] 
    /*00004b5c*/ MOVW        R2, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x136c] 
    /*00004b60*/ LDR         R3, [PC, #172]              @ 0x0000000000004c14 
    /*00004b64*/ MOVT        R2, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x136c] 
    /*00004b68*/ STR         R5, [SP, #4] 
    /*00004b6c*/ MOV         R0, #3 
    /*00004b70*/ BL          0x0000000000004B70          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004b74*/ ADD         R0, R4, #124                @ 0x7c 
    /*00004b78*/ BL          0x0000000000004B78          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004b7c*/ MOVW        R0, #32775                  @ 0x8007 
    /*00004b80*/ MOVT        R0, #40981                  @ 0xa015 
    /*00004b84*/ B           AiGetDevAttr_x4b48
    AiGetDevAttr_x4b88:
    /*00004b88*/ MOV         R0, #65024                  @ 0xfe00 
    /*00004b8c*/ MOVT        R0, #65535                  @ 0xffff 
    /*00004b90*/ B           AiGetDevAttr_x4b48
    AiGetDevAttr_x4b94:
    /*00004b94*/ MOVW        R0, #32774                  @ 0x8006 
    /*00004b98*/ MOVT        R0, #40981                  @ 0xa015 
    /*00004b9c*/ B           AiGetDevAttr_x4b48
    AiGetDevAttr_x4ba0:
    /*00004ba0*/ MOVW        R3, #725                    @ 0x2d5 
    /*00004ba4*/ STR         R4, [SP, #4] 
    /*00004ba8*/ STR         R3, [SP] 
    /*00004bac*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12b8] 
    /*00004bb0*/ LDR         R3, [PC, #92]               @ 0x0000000000004c14 
    /*00004bb4*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12b8] 
    /*00004bb8*/ MOV         R1, #21 
    /*00004bbc*/ MOV         R0, #3 
    /*00004bc0*/ BL          0x0000000000004BC0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004bc4*/ MOVW        R0, #32769                  @ 0x8001 
    /*00004bc8*/ MOVT        R0, #40981                  @ 0xa015 
    /*00004bcc*/ B           AiGetDevAttr_x4b48
    AiGetDevAttr_x4bd0:
    /*00004bd0*/ STR         R3, [SP, #4] 
    /*00004bd4*/ MOV         R1, #21 
    /*00004bd8*/ MOVW        R3, #735                    @ 0x2df 
    /*00004bdc*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12ec] 
    /*00004be0*/ STR         R3, [SP] 
    /*00004be4*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12ec] 
    /*00004be8*/ LDR         R3, [PC, #36]               @ 0x0000000000004c14 
    /*00004bec*/ MOV         R0, #3 
    /*00004bf0*/ BL          0x0000000000004BF0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004bf4*/ ADD         R0, R4, #124                @ 0x7c 
    /*00004bf8*/ BL          0x0000000000004BF8          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004bfc*/ MOVW        R0, #32777                  @ 0x8009 
    /*00004c00*/ MOVT        R0, #40981                  @ 0xa015 
    /*00004c04*/ B           AiGetDevAttr_x4b48
    AiGetDevAttr_x4c08:
    /*00004c08*/ MOVW        R0, #32784                  @ 0x8010 
    /*00004c0c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00004c10*/ B           AiGetDevAttr_x4b48
    /*00004c14*/ STRDEQ      R0, [R0], -R8 
FUNC_END AiGetDevAttr


FUNC_BEGIN AiEnableDev
    /*00004c18*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00004c1c*/ MOV         R4, R0 
    /*00004c20*/ SUB         SP, SP, #16 
    /*00004c24*/ MOV         R0, #20 
    /*00004c28*/ BL          0x0000000000004C28          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00004c2c*/ CMP         R0, #0 
    /*00004c30*/ BEQ         AiEnableDev_x506c
    /*00004c34*/ CMP         R4, #0 
    /*00004c38*/ BNE         AiEnableDev_x4f78
    /*00004c3c*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*00004c40*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*00004c44*/ ADD         R0, R6, #124                @ 0x7c 
    /*00004c48*/ BL          0x0000000000004C48          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00004c4c*/ SUBS        R7, R0, #0 
    /*00004c50*/ BNE         AiEnableDev_x4fa8
    /*00004c54*/ LDR         R3, [R6, #4] 
    /*00004c58*/ CMP         R3, #0 
    /*00004c5c*/ BEQ         AiEnableDev_x4fec
    /*00004c60*/ LDR         R3, [R6, #12] 
    /*00004c64*/ CMP         R3, #1 
    /*00004c68*/ BEQ         AiEnableDev_x4f28
    /*00004c6c*/ LDR         R3, [R6, #8] 
    /*00004c70*/ CMP         R3, #1 
    /*00004c74*/ ADDEQ       R4, R6, #148                @ 0x94 
    /*00004c78*/ MOVWEQ      R8, #30465                  @ 0x7701 
    /*00004c7c*/ MOVEQ       R5, R7 
    /*00004c80*/ MOVTEQ      R8, #1 
    /*00004c84*/ BNE         AiEnableDev_x4fb4
    AiEnableDev_x4c88:
    /*00004c88*/ MOVW        R2, #1108                   @ 0x454 
    /*00004c8c*/ MOV         R1, #0 
    /*00004c90*/ MOV         R0, R4 
    /*00004c94*/ BL          0x0000000000004C94          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00004c98*/ MOV         R3, #1120                   @ 0x460 
    /*00004c9c*/ MLA         R3, R3, R7, R6 
    /*00004ca0*/ ADD         R7, R7, #1 
    /*00004ca4*/ ADD         R0, R4, #404                @ 0x194 
    /*00004ca8*/ ADD         R1, R4, #364                @ 0x16c 
    /*00004cac*/ CMP         R7, #2 
    /*00004cb0*/ STR         R5, [R4, #196]              @ 0xc4 
    /*00004cb4*/ MVN         R2, #0 
    /*00004cb8*/ MOV         LR, #1 
    /*00004cbc*/ STR         R2, [R4, #204]              @ 0xcc 
    /*00004cc0*/ MOV         IP, #2 
    /*00004cc4*/ STR         R2, [R4, #208]              @ 0xd0 
    /*00004cc8*/ MVN         R2, #49                     @ 0x31 
    /*00004ccc*/ STR         LR, [R3, #348]              @ 0x15c 
    /*00004cd0*/ ADD         R4, R4, #1120               @ 0x460 
    /*00004cd4*/ STR         R5, [R4, #-892]             @ 0xfffffc84 
    /*00004cd8*/ MVN         R3, #39                     @ 0x27 
    /*00004cdc*/ STR         R5, [R4, #-880]             @ 0xfffffc90 
    /*00004ce0*/ STR         R5, [R4, #-884]             @ 0xfffffc8c 
    /*00004ce4*/ STR         R5, [R4, #-872]             @ 0xfffffc98 
    /*00004ce8*/ STR         R5, [R4, #-876]             @ 0xfffffc94 
    /*00004cec*/ STR         R5, [R4, #-888]             @ 0xfffffc88 
    /*00004cf0*/ STR         R5, [R4, #-868]             @ 0xfffffc9c 
    /*00004cf4*/ STR         R5, [R4, #-824]             @ 0xfffffcc8 
    /*00004cf8*/ STR         R8, [R4, #-848]             @ 0xfffffcb0 
    /*00004cfc*/ STR         R5, [R4, #-812]             @ 0xfffffcd4 
    /*00004d00*/ STR         R5, [R4, #-728]             @ 0xfffffd28 
    /*00004d04*/ STRB        IP, [R4, #-808]             @ 0xfffffcd8 
    /*00004d08*/ STRH        R5, [R0] 
    /*00004d0c*/ STRB        R5, [R4, #-721]             @ 0xfffffd2f 
    /*00004d10*/ STRB        R5, [R4, #-720]             @ 0xfffffd30 
    /*00004d14*/ STRB        R5, [R4, #-722]             @ 0xfffffd2e 
    /*00004d18*/ STRB        R5, [R4, #-718]             @ 0xfffffd32 
    /*00004d1c*/ STRB        R5, [R4, #-719]             @ 0xfffffd31 
    /*00004d20*/ STR         R5, [R4, #-760]             @ 0xfffffd08 
    /*00004d24*/ STRB        R2, [R4, #-723]             @ 0xfffffd2d 
    /*00004d28*/ STRB        R3, [R4, #-724]             @ 0xfffffd2c 
    /*00004d2c*/ STRH        R5, [R1] 
    /*00004d30*/ STR         R5, [R4, #-572]             @ 0xfffffdc4 
    /*00004d34*/ STR         R5, [R4, #-568]             @ 0xfffffdc8 
    /*00004d38*/ STR         R5, [R4, #-564]             @ 0xfffffdcc 
    /*00004d3c*/ STR         R8, [R4, #-560]             @ 0xfffffdd0 
    /*00004d40*/ STR         R8, [R4, #-556]             @ 0xfffffdd4 
    /*00004d44*/ BNE         AiEnableDev_x4c88
    /*00004d48*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*00004d4c*/ CMP         R3, #0 
    /*00004d50*/ ADDNE       R7, R6, #288                @ 0x120 
    /*00004d54*/ MOVNE       R4, #0 
    /*00004d58*/ MOVNE       R5, R7 
    /*00004d5c*/ BNE         AiEnableDev_x4d74
    /*00004d60*/ B           AiEnableDev_x4e60
    AiEnableDev_x4d64:
    /*00004d64*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*00004d68*/ ADD         R4, R4, #1 
    /*00004d6c*/ CMP         R3, R4 
    /*00004d70*/ BLS         AiEnableDev_x4dcc
    AiEnableDev_x4d74:
    /*00004d74*/ MOV         R1, R5 
    /*00004d78*/ MOV         R0, #30 
    /*00004d7c*/ BL          0x0000000000004D7C          @ AUDIO_FB_Init [R_ARM_CALL: .text+0x7f30] 
    /*00004d80*/ ADD         R5, R5, #1120               @ 0x460 
    /*00004d84*/ CMP         R0, #0 
    /*00004d88*/ BEQ         AiEnableDev_x4d64
    /*00004d8c*/ CMP         R4, #0 
    /*00004d90*/ MOVNE       R5, #0 
    /*00004d94*/ BEQ         AiEnableDev_x4db0
    AiEnableDev_x4d98:
    /*00004d98*/ MOV         R0, R7 
    /*00004d9c*/ ADD         R5, R5, #1 
    /*00004da0*/ BL          0x0000000000004DA0          @ AUDIO_FB_Release [R_ARM_CALL: .text+0x7fc8] 
    /*00004da4*/ CMP         R5, R4 
    /*00004da8*/ ADD         R7, R7, #1120               @ 0x460 
    /*00004dac*/ BNE         AiEnableDev_x4d98
    AiEnableDev_x4db0:
    /*00004db0*/ LDR         R0, [PC, #704]              @ 0x0000000000005078 
    /*00004db4*/ MOVW        R7, #32780                  @ 0x800c 
    /*00004db8*/ BL          0x0000000000004DB8          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004dbc*/ MOVT        R7, #40981                  @ 0xa015 
    AiEnableDev_x4dc0:
    /*00004dc0*/ MOV         R0, R7 
    /*00004dc4*/ ADD         SP, SP, #16 
    /*00004dc8*/ POP         {R4, R5, R6, R7, R8, PC} 
    AiEnableDev_x4dcc:
    /*00004dcc*/ CMP         R3, #0 
    /*00004dd0*/ BEQ         AiEnableDev_x4e60
    /*00004dd4*/ ADD         R8, R6, #316                @ 0x13c 
    /*00004dd8*/ MOV         R4, R0 
    /*00004ddc*/ MOV         R5, R8 
    /*00004de0*/ B           AiEnableDev_x4df4
    AiEnableDev_x4de4:
    /*00004de4*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*00004de8*/ ADD         R4, R4, #1 
    /*00004dec*/ CMP         R3, R4 
    /*00004df0*/ BLS         AiEnableDev_x4e60
    AiEnableDev_x4df4:
    /*00004df4*/ MOV         R1, R5 
    /*00004df8*/ MOV         R0, #30 
    /*00004dfc*/ BL          0x0000000000004DFC          @ AUDIO_FB_Init [R_ARM_CALL: .text+0x7f30] 
    /*00004e00*/ ADD         R5, R5, #1120               @ 0x460 
    /*00004e04*/ CMP         R0, #0 
    /*00004e08*/ BEQ         AiEnableDev_x4de4
    /*00004e0c*/ CMP         R4, #0 
    /*00004e10*/ MOVNE       R5, #0 
    /*00004e14*/ BEQ         AiEnableDev_x4e30
    AiEnableDev_x4e18:
    /*00004e18*/ MOV         R0, R8 
    /*00004e1c*/ ADD         R5, R5, #1 
    /*00004e20*/ BL          0x0000000000004E20          @ AUDIO_FB_Release [R_ARM_CALL: .text+0x7fc8] 
    /*00004e24*/ CMP         R5, R4 
    /*00004e28*/ ADD         R8, R8, #1120               @ 0x460 
    /*00004e2c*/ BNE         AiEnableDev_x4e18
    AiEnableDev_x4e30:
    /*00004e30*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*00004e34*/ CMP         R3, #0 
    /*00004e38*/ MOVNE       R4, #0 
    /*00004e3c*/ BEQ         AiEnableDev_x4db0
    AiEnableDev_x4e40:
    /*00004e40*/ MOV         R0, R7 
    /*00004e44*/ ADD         R4, R4, #1 
    /*00004e48*/ BL          0x0000000000004E48          @ AUDIO_FB_Release [R_ARM_CALL: .text+0x7fc8] 
    /*00004e4c*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*00004e50*/ ADD         R7, R7, #1120               @ 0x460 
    /*00004e54*/ CMP         R3, R4 
    /*00004e58*/ BHI         AiEnableDev_x4e40
    /*00004e5c*/ B           AiEnableDev_x4db0
    AiEnableDev_x4e60:
    /*00004e60*/ LDR         R1, [PC, #532]              @ 0x000000000000507c 
    /*00004e64*/ MOV         R0, #0 
    /*00004e68*/ BL          0x0000000000004E68          @ AIDmaBufInit [R_ARM_CALL: .text+0x4570] 
    /*00004e6c*/ SUBS        R7, R0, #0 
    /*00004e70*/ BEQ         AiEnableDev_x4ec0
    /*00004e74*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*00004e78*/ CMP         R3, #0 
    /*00004e7c*/ BEQ         AiEnableDev_x4eac
    /*00004e80*/ LDR         R4, [PC, #504]              @ 0x0000000000005080 
    /*00004e84*/ MOV         R5, #0 
    AiEnableDev_x4e88:
    /*00004e88*/ MOV         R0, R4 
    /*00004e8c*/ ADD         R5, R5, #1 
    /*00004e90*/ BL          0x0000000000004E90          @ AUDIO_FB_Release [R_ARM_CALL: .text+0x7fc8] 
    /*00004e94*/ ADD         R0, R4, #28 
    /*00004e98*/ BL          0x0000000000004E98          @ AUDIO_FB_Release [R_ARM_CALL: .text+0x7fc8] 
    /*00004e9c*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*00004ea0*/ ADD         R4, R4, #1120               @ 0x460 
    /*00004ea4*/ CMP         R3, R5 
    /*00004ea8*/ BHI         AiEnableDev_x4e88
    AiEnableDev_x4eac:
    /*00004eac*/ LDR         R0, [PC, #452]              @ 0x0000000000005078 
    /*00004eb0*/ BL          0x0000000000004EB0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004eb4*/ MOV         R0, R7 
    /*00004eb8*/ ADD         SP, SP, #16 
    /*00004ebc*/ POP         {R4, R5, R6, R7, R8, PC} 
    AiEnableDev_x4ec0:
    /*00004ec0*/ LDR         R1, [PC, #436]              @ 0x000000000000507c 
    /*00004ec4*/ BL          0x0000000000004EC4          @ AIVbInit [R_ARM_CALL: .text+0x4724] 
    /*00004ec8*/ SUBS        R7, R0, #0 
    /*00004ecc*/ BNE         AiEnableDev_x4f34
    /*00004ed0*/ BL          0x0000000000004ED0          @ AI_DRV_EnableDev [R_ARM_CALL: .text+0x86c0] 
    /*00004ed4*/ SUBS        R7, R0, #0 
    /*00004ed8*/ BEQ         AiEnableDev_x5024
    /*00004edc*/ MOV         R0, #0 
    /*00004ee0*/ BL          0x0000000000004EE0          @ AIVbFree [R_ARM_CALL: .text+0x48b4] 
    /*00004ee4*/ MOV         R0, #0 
    /*00004ee8*/ BL          0x0000000000004EE8          @ AIDmaBufExit [R_ARM_CALL: .text+0x46e0] 
    /*00004eec*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*00004ef0*/ CMP         R3, #0 
    /*00004ef4*/ BEQ         AiEnableDev_x4eac
    /*00004ef8*/ LDR         R4, [PC, #384]              @ 0x0000000000005080 
    /*00004efc*/ MOV         R5, #0 
    AiEnableDev_x4f00:
    /*00004f00*/ MOV         R0, R4 
    /*00004f04*/ ADD         R5, R5, #1 
    /*00004f08*/ BL          0x0000000000004F08          @ AUDIO_FB_Release [R_ARM_CALL: .text+0x7fc8] 
    /*00004f0c*/ ADD         R0, R4, #28 
    /*00004f10*/ BL          0x0000000000004F10          @ AUDIO_FB_Release [R_ARM_CALL: .text+0x7fc8] 
    /*00004f14*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*00004f18*/ ADD         R4, R4, #1120               @ 0x460 
    /*00004f1c*/ CMP         R3, R5 
    /*00004f20*/ BHI         AiEnableDev_x4f00
    /*00004f24*/ B           AiEnableDev_x4eac
    AiEnableDev_x4f28:
    /*00004f28*/ ADD         R0, R6, #124                @ 0x7c 
    /*00004f2c*/ BL          0x0000000000004F2C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004f30*/ B           AiEnableDev_x4dc0
    AiEnableDev_x4f34:
    /*00004f34*/ MOV         R0, #0 
    /*00004f38*/ BL          0x0000000000004F38          @ AIDmaBufExit [R_ARM_CALL: .text+0x46e0] 
    /*00004f3c*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*00004f40*/ CMP         R3, #0 
    /*00004f44*/ BEQ         AiEnableDev_x4eac
    /*00004f48*/ LDR         R4, [PC, #304]              @ 0x0000000000005080 
    /*00004f4c*/ MOV         R5, #0 
    AiEnableDev_x4f50:
    /*00004f50*/ MOV         R0, R4 
    /*00004f54*/ ADD         R5, R5, #1 
    /*00004f58*/ BL          0x0000000000004F58          @ AUDIO_FB_Release [R_ARM_CALL: .text+0x7fc8] 
    /*00004f5c*/ ADD         R0, R4, #28 
    /*00004f60*/ BL          0x0000000000004F60          @ AUDIO_FB_Release [R_ARM_CALL: .text+0x7fc8] 
    /*00004f64*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*00004f68*/ ADD         R4, R4, #1120               @ 0x460 
    /*00004f6c*/ CMP         R3, R5 
    /*00004f70*/ BHI         AiEnableDev_x4f50
    /*00004f74*/ B           AiEnableDev_x4eac
    AiEnableDev_x4f78:
    /*00004f78*/ MOVW        R3, #767                    @ 0x2ff 
    /*00004f7c*/ STR         R4, [SP, #4] 
    /*00004f80*/ STR         R3, [SP] 
    /*00004f84*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12b8] 
    /*00004f88*/ LDR         R3, [PC, #244]              @ 0x0000000000005084 
    /*00004f8c*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12b8] 
    /*00004f90*/ MOV         R1, #21 
    /*00004f94*/ MOV         R0, #3 
    /*00004f98*/ MOVW        R7, #32769                  @ 0x8001 
    /*00004f9c*/ BL          0x0000000000004F9C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004fa0*/ MOVT        R7, #40981                  @ 0xa015 
    /*00004fa4*/ B           AiEnableDev_x4dc0
    AiEnableDev_x4fa8:
    /*00004fa8*/ MOV         R7, #65024                  @ 0xfe00 
    /*00004fac*/ MOVT        R7, #65535                  @ 0xffff 
    /*00004fb0*/ B           AiEnableDev_x4dc0
    AiEnableDev_x4fb4:
    /*00004fb4*/ STR         R7, [SP, #4] 
    /*00004fb8*/ MOV         R3, #792                    @ 0x318 
    /*00004fbc*/ MOV         R1, #21 
    /*00004fc0*/ STR         R3, [SP] 
    /*00004fc4*/ MOVW        R2, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13dc] 
    /*00004fc8*/ LDR         R3, [PC, #180]              @ 0x0000000000005084 
    /*00004fcc*/ MOVT        R2, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13dc] 
    /*00004fd0*/ MOV         R0, #3 
    /*00004fd4*/ BL          0x0000000000004FD4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004fd8*/ ADD         R0, R6, #124                @ 0x7c 
    /*00004fdc*/ MOVW        R7, #32775                  @ 0x8007 
    /*00004fe0*/ BL          0x0000000000004FE0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004fe4*/ MOVT        R7, #40981                  @ 0xa015 
    /*00004fe8*/ B           AiEnableDev_x4dc0
    AiEnableDev_x4fec:
    /*00004fec*/ STR         R3, [SP, #4] 
    /*00004ff0*/ MOV         R1, #21 
    /*00004ff4*/ MOVW        R3, #779                    @ 0x30b 
    /*00004ff8*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13a4] 
    /*00004ffc*/ STR         R3, [SP] 
    /*00005000*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13a4] 
    /*00005004*/ LDR         R3, [PC, #120]              @ 0x0000000000005084 
    /*00005008*/ MOV         R0, #3 
    /*0000500c*/ BL          0x000000000000500C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005010*/ ADD         R0, R6, #124                @ 0x7c 
    /*00005014*/ MOVW        R7, #32777                  @ 0x8009 
    /*00005018*/ BL          0x0000000000005018          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000501c*/ MOVT        R7, #40981                  @ 0xa015 
    /*00005020*/ B           AiEnableDev_x4dc0
    AiEnableDev_x5024:
    /*00005024*/ ADD         R1, SP, #12 
    /*00005028*/ LDR         R0, [PC, #88]               @ 0x0000000000005088 
    /*0000502c*/ BL          0x000000000000502C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00005030*/ ADD         R1, SP, #12 
    /*00005034*/ LDR         R0, [PC, #76]               @ 0x0000000000005088 
    /*00005038*/ MOV         R3, #1 
    /*0000503c*/ STR         R3, [R6, #12] 
    /*00005040*/ BL          0x0000000000005040          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00005044*/ MOVW        R3, #939                    @ 0x3ab 
    /*00005048*/ STR         R7, [SP, #4] 
    /*0000504c*/ STR         R3, [SP] 
    /*00005050*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1410] 
    /*00005054*/ LDR         R3, [PC, #40]               @ 0x0000000000005084 
    /*00005058*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1410] 
    /*0000505c*/ MOV         R1, #21 
    /*00005060*/ MOV         R0, #6 
    /*00005064*/ BL          0x0000000000005064          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005068*/ B           AiEnableDev_x4eac
    AiEnableDev_x506c:
    /*0000506c*/ MOVW        R7, #32784                  @ 0x8010 
    /*00005070*/ MOVT        R7, #40981                  @ 0xa015 
    /*00005074*/ B           AiEnableDev_x4dc0
    /*00005078*/ ANDEQ       R0, R0, R4, LSL #1 
    /*0000507c*/ ANDEQ       R0, R0, R8, LSL R0 
    /*00005080*/ ANDEQ       R0, R0, R8, LSR #2 
    /*00005084*/ ANDEQ       R0, R0, R8, LSL #4 
    /*00005088*/ ANDEQ       R0, R0, R8, LSL #1 
FUNC_END AiEnableDev


FUNC_BEGIN AiDisableDev
    /*0000508c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00005090*/ SUB         SP, SP, #24 
    /*00005094*/ MOV         R4, R0 
    /*00005098*/ MOV         R0, #20 
    /*0000509c*/ MOV         R3, #0 
    /*000050a0*/ STR         R3, [SP, #16] 
    /*000050a4*/ BL          0x00000000000050A4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000050a8*/ CMP         R0, #0 
    /*000050ac*/ BEQ         AiDisableDev_x5434
    /*000050b0*/ CMP         R4, #0 
    /*000050b4*/ BNE         AiDisableDev_x538c
    /*000050b8*/ MOVW        R7, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*000050bc*/ MOVT        R7, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*000050c0*/ ADD         R0, R7, #124                @ 0x7c 
    /*000050c4*/ BL          0x00000000000050C4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000050c8*/ CMP         R0, #0 
    /*000050cc*/ BNE         AiDisableDev_x53bc
    /*000050d0*/ LDR         R3, [R7, #4] 
    /*000050d4*/ CMP         R3, #0 
    /*000050d8*/ BEQ         AiDisableDev_x5330
    /*000050dc*/ LDR         R3, [R7, #12] 
    /*000050e0*/ CMP         R3, #0 
    /*000050e4*/ BEQ         AiDisableDev_x5330
    /*000050e8*/ LDR         R4, [R7, #44]               @ 0x2c 
    /*000050ec*/ CMP         R4, #0 
    /*000050f0*/ BEQ         AiDisableDev_x5120
    /*000050f4*/ LDR         R3, [R7, #344]              @ 0x158 
    /*000050f8*/ SUB         R3, R3, #1 
    /*000050fc*/ CMP         R3, #1 
    /*00005100*/ BLS         AiDisableDev_x53e0
    /*00005104*/ CMP         R4, #1 
    /*00005108*/ BLS         AiDisableDev_x511c
    /*0000510c*/ LDR         R3, [R7, #1464]             @ 0x5b8 
    /*00005110*/ SUB         R3, R3, #1 
    /*00005114*/ CMP         R3, #1 
    /*00005118*/ BLS         AiDisableDev_x542c
    AiDisableDev_x511c:
    /*0000511c*/ MOV         R4, R0 
    AiDisableDev_x5120:
    /*00005120*/ MOV         R0, #1 
    /*00005124*/ BL          0x0000000000005124          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00005128*/ ADD         R2, SP, #16 
    /*0000512c*/ MOV         R1, R4 
    /*00005130*/ LDR         R3, [R0, #96]               @ 0x60 
    /*00005134*/ LDR         R0, [R7] 
    /*00005138*/ BLX         R3 
    /*0000513c*/ SUBS        R5, R0, #0 
    /*00005140*/ BNE         AiDisableDev_x5154
    /*00005144*/ LDR         R3, [SP, #16] 
    /*00005148*/ CMP         R3, #0 
    /*0000514c*/ CMPNE       R4, #14 
    /*00005150*/ BNE         AiDisableDev_x5348
    AiDisableDev_x5154:
    /*00005154*/ ADD         R4, R4, #1 
    /*00005158*/ CMP         R4, #23 
    /*0000515c*/ BNE         AiDisableDev_x5120
    /*00005160*/ MOV         R0, #0 
    /*00005164*/ BL          0x0000000000005164          @ AI_DRV_DisableDev [R_ARM_CALL: .text+0x878c] 
    /*00005168*/ MOV         R0, #0 
    /*0000516c*/ BL          0x000000000000516C          @ AIDmaBufExit [R_ARM_CALL: .text+0x46e0] 
    /*00005170*/ ADD         R1, SP, #20 
    /*00005174*/ LDR         R0, [PC, #708]              @ 0x0000000000005440 
    /*00005178*/ BL          0x0000000000005178          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000517c*/ LDR         R3, [R7, #44]               @ 0x2c 
    /*00005180*/ CMP         R3, #0 
    /*00005184*/ LDRNE       R8, [PC, #696]              @ 0x0000000000005444 
    /*00005188*/ MOVNE       R9, #0 
    /*0000518c*/ BEQ         AiDisableDev_x5278
    AiDisableDev_x5190:
    /*00005190*/ ADD         R6, R8, #148                @ 0x94 
    /*00005194*/ MOV         R0, R8 
    /*00005198*/ BL          0x0000000000005198          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*0000519c*/ MOV         R0, R6 
    /*000051a0*/ BL          0x00000000000051A0          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*000051a4*/ SUBS        SL, R0, #0 
    /*000051a8*/ MOVNE       R4, #0 
    /*000051ac*/ BEQ         AiDisableDev_x51e0
    AiDisableDev_x51b0:
    /*000051b0*/ MOV         R0, R6 
    /*000051b4*/ ADD         R4, R4, #1 
    /*000051b8*/ BL          0x00000000000051B8          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x805c] 
    /*000051bc*/ MOV         R1, #14 
    /*000051c0*/ MOV         R5, R0 
    /*000051c4*/ ADD         R0, R0, #8 
    /*000051c8*/ BL          0x00000000000051C8          @ AISubVb [R_ARM_CALL: .text+0x3a50] 
    /*000051cc*/ MOV         R1, R5 
    /*000051d0*/ MOV         R0, R6 
    /*000051d4*/ BL          0x00000000000051D4          @ AUDIO_FB_PutFree [R_ARM_CALL: .text+0x80bc] 
    /*000051d8*/ CMP         SL, R4 
    /*000051dc*/ BNE         AiDisableDev_x51b0
    AiDisableDev_x51e0:
    /*000051e0*/ ADD         R6, R8, #176                @ 0xb0 
    /*000051e4*/ ADD         R0, R8, #4 
    /*000051e8*/ BL          0x00000000000051E8          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*000051ec*/ MOV         R0, R6 
    /*000051f0*/ BL          0x00000000000051F0          @ AUDIO_FB_QueryBusyNum [R_ARM_CALL: .text+0x816c] 
    /*000051f4*/ SUBS        SL, R0, #0 
    /*000051f8*/ MOVNE       R4, #0 
    /*000051fc*/ BEQ         AiDisableDev_x5230
    AiDisableDev_x5200:
    /*00005200*/ MOV         R0, R6 
    /*00005204*/ ADD         R4, R4, #1 
    /*00005208*/ BL          0x0000000000005208          @ AUDIO_FB_GetBusy [R_ARM_CALL: .text+0x805c] 
    /*0000520c*/ MOV         R1, #14 
    /*00005210*/ MOV         R5, R0 
    /*00005214*/ ADD         R0, R0, #8 
    /*00005218*/ BL          0x0000000000005218          @ AISubVb [R_ARM_CALL: .text+0x3a50] 
    /*0000521c*/ MOV         R1, R5 
    /*00005220*/ MOV         R0, R6 
    /*00005224*/ BL          0x0000000000005224          @ AUDIO_FB_PutFree [R_ARM_CALL: .text+0x80bc] 
    /*00005228*/ CMP         SL, R4 
    /*0000522c*/ BNE         AiDisableDev_x5200
    AiDisableDev_x5230:
    /*00005230*/ LDR         R3, [R7, #44]               @ 0x2c 
    /*00005234*/ ADD         R9, R9, #1 
    /*00005238*/ ADD         R8, R8, #1120               @ 0x460 
    /*0000523c*/ CMP         R3, R9 
    /*00005240*/ BHI         AiDisableDev_x5190
    /*00005244*/ CMP         R3, #0 
    /*00005248*/ BEQ         AiDisableDev_x5278
    /*0000524c*/ ADD         R4, R7, #288                @ 0x120 
    /*00005250*/ MOV         R5, #0 
    AiDisableDev_x5254:
    /*00005254*/ MOV         R0, R4 
    /*00005258*/ ADD         R5, R5, #1 
    /*0000525c*/ BL          0x000000000000525C          @ AUDIO_FB_Release [R_ARM_CALL: .text+0x7fc8] 
    /*00005260*/ ADD         R0, R4, #28 
    /*00005264*/ BL          0x0000000000005264          @ AUDIO_FB_Release [R_ARM_CALL: .text+0x7fc8] 
    /*00005268*/ LDR         R3, [R7, #44]               @ 0x2c 
    /*0000526c*/ ADD         R4, R4, #1120               @ 0x460 
    /*00005270*/ CMP         R3, R5 
    /*00005274*/ BHI         AiDisableDev_x5254
    AiDisableDev_x5278:
    /*00005278*/ MOV         R4, #0 
    /*0000527c*/ ADD         R1, SP, #20 
    /*00005280*/ LDR         R0, [PC, #440]              @ 0x0000000000005440 
    /*00005284*/ STR         R4, [R7, #12] 
    /*00005288*/ BL          0x0000000000005288          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000528c*/ MOV         R0, R4 
    /*00005290*/ BL          0x0000000000005290          @ AIVbFree [R_ARM_CALL: .text+0x48b4] 
    /*00005294*/ LDR         R3, [R7, #44]               @ 0x2c 
    /*00005298*/ CMP         R3, R4 
    /*0000529c*/ BEQ         AiDisableDev_x52dc
    /*000052a0*/ CMP         R3, #1 
    /*000052a4*/ STR         R4, [R7, #348]              @ 0x15c 
    /*000052a8*/ MVN         R3, #0 
    /*000052ac*/ STR         R4, [R7, #344]              @ 0x158 
    /*000052b0*/ STR         R4, [R7, #364]              @ 0x16c 
    /*000052b4*/ STR         R4, [R7, #360]              @ 0x168 
    /*000052b8*/ STR         R4, [R7, #368]              @ 0x170 
    /*000052bc*/ STR         R3, [R7, #352]              @ 0x160 
    /*000052c0*/ BLS         AiDisableDev_x52dc
    /*000052c4*/ STR         R4, [R7, #1468]             @ 0x5bc 
    /*000052c8*/ STR         R4, [R7, #1464]             @ 0x5b8 
    /*000052cc*/ STR         R3, [R7, #1472]             @ 0x5c0 
    /*000052d0*/ STR         R4, [R7, #1484]             @ 0x5cc 
    /*000052d4*/ STR         R4, [R7, #1480]             @ 0x5c8 
    /*000052d8*/ STR         R4, [R7, #1488]             @ 0x5d0 
    AiDisableDev_x52dc:
    /*000052dc*/ LDR         R3, [R7, #8] 
    /*000052e0*/ CMP         R3, #0 
    /*000052e4*/ BEQ         AiDisableDev_x52f4
    /*000052e8*/ LDR         R0, [R7, #12] 
    /*000052ec*/ CMP         R0, #0 
    /*000052f0*/ BEQ         AiDisableDev_x53c8
    AiDisableDev_x52f4:
    /*000052f4*/ LDR         R0, [PC, #332]              @ 0x0000000000005448 
    /*000052f8*/ MOV         R6, #0 
    /*000052fc*/ BL          0x00000000000052FC          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005300*/ STR         R6, [SP, #4] 
    /*00005304*/ MOV         R3, #1088                   @ 0x440 
    /*00005308*/ MOV         R1, #21 
    /*0000530c*/ STR         R3, [SP] 
    /*00005310*/ MOVW        R2, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14f4] 
    /*00005314*/ LDR         R3, [PC, #304]              @ 0x000000000000544c 
    /*00005318*/ MOVT        R2, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14f4] 
    /*0000531c*/ MOV         R0, #6 
    /*00005320*/ BL          0x0000000000005320          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005324*/ MOV         R0, R6 
    /*00005328*/ ADD         SP, SP, #24 
    /*0000532c*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AiDisableDev_x5330:
    /*00005330*/ LDR         R0, [PC, #272]              @ 0x0000000000005448 
    /*00005334*/ MOV         R6, #0 
    /*00005338*/ BL          0x0000000000005338          @ osal_up [R_ARM_CALL: +0x0] 
    AiDisableDev_x533c:
    /*0000533c*/ MOV         R0, R6 
    /*00005340*/ ADD         SP, SP, #24 
    /*00005344*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AiDisableDev_x5348:
    /*00005348*/ LDR         R0, [PC, #248]              @ 0x0000000000005448 
    /*0000534c*/ MOVW        R6, #32786                  @ 0x8012 
    /*00005350*/ BL          0x0000000000005350          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005354*/ LDR         R1, [SP, #16] 
    /*00005358*/ MOV         R3, #996                    @ 0x3e4 
    /*0000535c*/ STR         R5, [SP, #8] 
    /*00005360*/ STR         R3, [SP] 
    /*00005364*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14b0] 
    /*00005368*/ STR         R1, [SP, #12] 
    /*0000536c*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14b0] 
    /*00005370*/ STR         R4, [SP, #4] 
    /*00005374*/ MOV         R1, #21 
    /*00005378*/ LDR         R3, [PC, #204]              @ 0x000000000000544c 
    /*0000537c*/ MOV         R0, #3 
    /*00005380*/ BL          0x0000000000005380          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005384*/ MOVT        R6, #40961                  @ 0xa001 
    /*00005388*/ B           AiDisableDev_x533c
    AiDisableDev_x538c:
    /*0000538c*/ MOVW        R3, #955                    @ 0x3bb 
    /*00005390*/ STR         R4, [SP, #4] 
    /*00005394*/ STR         R3, [SP] 
    /*00005398*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12b8] 
    /*0000539c*/ LDR         R3, [PC, #168]              @ 0x000000000000544c 
    /*000053a0*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12b8] 
    /*000053a4*/ MOV         R1, #21 
    /*000053a8*/ MOV         R0, #3 
    /*000053ac*/ MOVW        R6, #32769                  @ 0x8001 
    /*000053b0*/ BL          0x00000000000053B0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000053b4*/ MOVT        R6, #40981                  @ 0xa015 
    /*000053b8*/ B           AiDisableDev_x533c
    AiDisableDev_x53bc:
    /*000053bc*/ MOV         R6, #65024                  @ 0xfe00 
    /*000053c0*/ MOVT        R6, #65535                  @ 0xffff 
    /*000053c4*/ B           AiDisableDev_x533c
    AiDisableDev_x53c8:
    /*000053c8*/ BL          0x00000000000053C8          @ AI_DRV_ClrDevAttr [R_ARM_CALL: .text+0x8660] 
    /*000053cc*/ SUBS        R6, R0, #0 
    /*000053d0*/ BEQ         AiDisableDev_x52f4
    /*000053d4*/ LDR         R0, [PC, #108]              @ 0x0000000000005448 
    /*000053d8*/ BL          0x00000000000053D8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000053dc*/ B           AiDisableDev_x533c
    AiDisableDev_x53e0:
    /*000053e0*/ MOV         R4, R0 
    AiDisableDev_x53e4:
    /*000053e4*/ LDR         R0, [PC, #92]               @ 0x0000000000005448 
    /*000053e8*/ MOVW        R6, #32777                  @ 0x8009 
    /*000053ec*/ BL          0x00000000000053EC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000053f0*/ STR         R4, [SP, #8] 
    /*000053f4*/ MOV         R2, #0 
    /*000053f8*/ MOVW        R3, #981                    @ 0x3d5 
    /*000053fc*/ STR         R2, [SP, #4] 
    /*00005400*/ MOV         R1, #21 
    /*00005404*/ STR         R3, [SP] 
    /*00005408*/ MOVW        R2, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1444] 
    /*0000540c*/ LDR         R3, [PC, #56]               @ 0x000000000000544c 
    /*00005410*/ MOVT        R2, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1444] 
    /*00005414*/ MOV         R0, #3 
    /*00005418*/ MOVT        R6, #40981                  @ 0xa015 
    /*0000541c*/ BL          0x000000000000541C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005420*/ MOV         R0, R6 
    /*00005424*/ ADD         SP, SP, #24 
    /*00005428*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AiDisableDev_x542c:
    /*0000542c*/ MOV         R4, #1 
    /*00005430*/ B           AiDisableDev_x53e4
    AiDisableDev_x5434:
    /*00005434*/ MOVW        R6, #32784                  @ 0x8010 
    /*00005438*/ MOVT        R6, #40981                  @ 0xa015 
    /*0000543c*/ B           AiDisableDev_x533c
    /*00005440*/ ANDEQ       R0, R0, R8, LSL #1 
    /*00005444*/ MULEQ       R0, R4, R0 
    /*00005448*/ ANDEQ       R0, R0, R4, LSL #1 
    /*0000544c*/ ANDEQ       R0, R0, R4, LSL R2 
FUNC_END AiDisableDev


FUNC_BEGIN AI_DEV_Init
    /*00005450*/ MOV         R2, #136                    @ 0x88 
    /*00005454*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00005458*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*0000545c*/ MUL         R7, R2, R0 
    /*00005460*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*00005464*/ ADD         R6, R4, R7 
    /*00005468*/ LDR         R3, [R6, #4] 
    /*0000546c*/ CMP         R3, #1 
    /*00005470*/ BEQ         AI_DEV_Init_x5540
    /*00005474*/ LDR         R3, [R6, #12] 
    /*00005478*/ CMP         R3, #1 
    /*0000547c*/ BEQ         AI_DEV_Init_x5540
    /*00005480*/ MOV         R5, R0 
    /*00005484*/ MOV         R1, #0 
    /*00005488*/ MOV         R0, R6 
    /*0000548c*/ BL          0x000000000000548C          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00005490*/ MOV         R3, #0 
    /*00005494*/ MOV         R0, R5 
    /*00005498*/ STR         R3, [R6, #12] 
    /*0000549c*/ BL          0x000000000000549C          @ AI_DRV_DevInit [R_ARM_CALL: .text+0x9350] 
    /*000054a0*/ SUBS        R8, R0, #0 
    /*000054a4*/ BNE         AI_DEV_Init_x5544
    /*000054a8*/ MOVW        R0, #0                      @ AIDmaCallback [R_ARM_MOVW_ABS_NC: .text+0x404c] 
    /*000054ac*/ MOVT        R0, #0                      @ AIDmaCallback [R_ARM_MOVT_ABS: .text+0x404c] 
    /*000054b0*/ BL          0x00000000000054B0          @ AI_DRV_SetDmaISR [R_ARM_CALL: .text+0x85a0] 
    /*000054b4*/ LSL         R6, R5, #1 
    /*000054b8*/ ADD         R3, R4, #136                @ 0x88 
    /*000054bc*/ MOV         R2, #1120                   @ 0x460 
    /*000054c0*/ MOV         R9, R8 
    /*000054c4*/ MLA         R6, R2, R6, R3 
    AI_DEV_Init_x54c8:
    /*000054c8*/ MOV         R2, #1120                   @ 0x460 
    /*000054cc*/ MOV         R1, #0 
    /*000054d0*/ MOV         R0, R6 
    /*000054d4*/ BL          0x00000000000054D4          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000054d8*/ MOV         R1, #1 
    /*000054dc*/ MOV         R0, R6 
    /*000054e0*/ ADD         R9, R9, R1 
    /*000054e4*/ BL          0x00000000000054E4          @ osal_sema_init [R_ARM_CALL: +0x0] 
    /*000054e8*/ ADD         R0, R6, #4 
    /*000054ec*/ BL          0x00000000000054EC          @ osal_wait_init [R_ARM_CALL: +0x0] 
    /*000054f0*/ ADD         R0, R6, #8 
    /*000054f4*/ BL          0x00000000000054F4          @ osal_wait_init [R_ARM_CALL: +0x0] 
    /*000054f8*/ CMP         R9, #2 
    /*000054fc*/ ADD         R6, R6, #1120               @ 0x460 
    /*00005500*/ BNE         AI_DEV_Init_x54c8
    /*00005504*/ ADD         R0, R7, #124                @ 0x7c 
    /*00005508*/ MOV         R1, #1 
    /*0000550c*/ ADD         R0, R4, R0 
    /*00005510*/ BL          0x0000000000005510          @ osal_sema_init [R_ARM_CALL: +0x0] 
    /*00005514*/ ADD         R0, R7, #128                @ 0x80 
    /*00005518*/ ADD         R0, R4, R0 
    /*0000551c*/ BL          0x000000000000551C          @ osal_spin_lock_init [R_ARM_CALL: +0x0] 
    /*00005520*/ MOV         R0, #136                    @ 0x88 
    /*00005524*/ MLA         R4, R0, R5, R4 
    /*00005528*/ MOV         R2, #1 
    /*0000552c*/ MOV         R3, #0 
    /*00005530*/ MOV         R0, R8 
    /*00005534*/ STR         R2, [R4, #4] 
    /*00005538*/ STR         R3, [R4, #8] 
    /*0000553c*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AI_DEV_Init_x5540:
    /*00005540*/ MOV         R8, #0 
    AI_DEV_Init_x5544:
    /*00005544*/ MOV         R0, R8 
    /*00005548*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
FUNC_END AI_DEV_Init


FUNC_BEGIN AI_DEV_Exit
    /*0000554c*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00005550*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*00005554*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*00005558*/ MOV         R6, #136                    @ 0x88 
    /*0000555c*/ SUB         SP, SP, #20 
    /*00005560*/ MLA         R6, R6, R0, R5 
    /*00005564*/ LDR         R4, [R6, #4] 
    /*00005568*/ CMP         R4, #0 
    /*0000556c*/ BNE         AI_DEV_Exit_x557c
    AI_DEV_Exit_x5570:
    /*00005570*/ MOV         R0, R4 
    /*00005574*/ ADD         SP, SP, #20 
    /*00005578*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AI_DEV_Exit_x557c:
    /*0000557c*/ MOV         R7, R0 
    /*00005580*/ BL          0x0000000000005580          @ AI_DRV_DevDeInit [R_ARM_CALL: .text+0x93d4] 
    /*00005584*/ SUBS        R4, R0, #0 
    /*00005588*/ BNE         AI_DEV_Exit_x5570
    /*0000558c*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*00005590*/ CMP         R3, #0 
    /*00005594*/ BEQ         AI_DEV_Exit_x55cc
    /*00005598*/ LSL         R8, R7, #1 
    /*0000559c*/ MOV         R3, #1120                   @ 0x460 
    /*000055a0*/ MVN         R9, #0 
    /*000055a4*/ MLA         R8, R3, R8, R5 
    /*000055a8*/ ADD         R8, R8, #140                @ 0x8c 
    AI_DEV_Exit_x55ac:
    /*000055ac*/ LDR         R3, [R8, #204]              @ 0xcc 
    /*000055b0*/ ADD         R4, R4, #1 
    /*000055b4*/ CMP         R3, #0 
    /*000055b8*/ BNE         AI_DEV_Exit_x5654
    AI_DEV_Exit_x55bc:
    /*000055bc*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*000055c0*/ ADD         R8, R8, #1120               @ 0x460 
    /*000055c4*/ CMP         R3, R4 
    /*000055c8*/ BHI         AI_DEV_Exit_x55ac
    AI_DEV_Exit_x55cc:
    /*000055cc*/ MOV         R0, R7 
    /*000055d0*/ BL          0x00000000000055D0          @ AiDisableDev [R_ARM_CALL: .text+0x508c] 
    /*000055d4*/ SUBS        R4, R0, #0 
    /*000055d8*/ BNE         AI_DEV_Exit_x5674
    /*000055dc*/ LDR         R3, [PC, #188]              @ 0x00000000000056a0 
    /*000055e0*/ LSL         R6, R7, #1 
    /*000055e4*/ MOV         R2, #1120                   @ 0x460 
    /*000055e8*/ MOV         R8, R4 
    /*000055ec*/ MLA         R6, R2, R6, R3 
    /*000055f0*/ ADD         R6, R6, #4 
    AI_DEV_Exit_x55f4:
    /*000055f4*/ MOV         R0, R6 
    /*000055f8*/ ADD         R8, R8, #1 
    /*000055fc*/ BL          0x00000000000055FC          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00005600*/ ADD         R0, R6, #4 
    /*00005604*/ BL          0x0000000000005604          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00005608*/ SUB         R0, R6, #4 
    /*0000560c*/ BL          0x000000000000560C          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*00005610*/ CMP         R8, #2 
    /*00005614*/ ADD         R6, R6, #1120               @ 0x460 
    /*00005618*/ BNE         AI_DEV_Exit_x55f4
    /*0000561c*/ MOV         R3, #136                    @ 0x88 
    /*00005620*/ MUL         R7, R3, R7 
    /*00005624*/ ADD         R0, R7, #128                @ 0x80 
    /*00005628*/ ADD         R0, R5, R0 
    /*0000562c*/ BL          0x000000000000562C          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00005630*/ ADD         R0, R7, #124                @ 0x7c 
    /*00005634*/ ADD         R0, R5, R0 
    /*00005638*/ ADD         R5, R5, R7 
    /*0000563c*/ BL          0x000000000000563C          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*00005640*/ MOV         R3, #0 
    /*00005644*/ MOV         R0, R4 
    /*00005648*/ STR         R3, [R5, #4] 
    /*0000564c*/ ADD         SP, SP, #20 
    /*00005650*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    AI_DEV_Exit_x5654:
    /*00005654*/ MOV         R0, R8 
    /*00005658*/ MOV         R3, #3 
    /*0000565c*/ STR         R9, [R8, #212]              @ 0xd4 
    /*00005660*/ STR         R3, [R8, #204]              @ 0xcc 
    /*00005664*/ BL          0x0000000000005664          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00005668*/ ADD         R0, R8, #4 
    /*0000566c*/ BL          0x000000000000566C          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00005670*/ B           AI_DEV_Exit_x55bc
    AI_DEV_Exit_x5674:
    /*00005674*/ MOVW        R3, #1167                   @ 0x48f 
    /*00005678*/ STR         R4, [SP, #8] 
    /*0000567c*/ STR         R3, [SP] 
    /*00005680*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1528] 
    /*00005684*/ STR         R7, [SP, #4] 
    /*00005688*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1528] 
    /*0000568c*/ LDR         R3, [PC, #16]               @ 0x00000000000056a4 
    /*00005690*/ MOV         R1, #21 
    /*00005694*/ MOV         R0, #3 
    /*00005698*/ BL          0x0000000000005698          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000569c*/ B           AI_DEV_Exit_x5570
    /*000056a0*/ MULEQ       R0, R0, R0 
    /*000056a4*/ ANDEQ       R0, R0, R4, LSR #4 
FUNC_END AI_DEV_Exit


FUNC_BEGIN AI_GetChnCtx
    /*000056a8*/ CMP         R0, #1 
    /*000056ac*/ LDRLS       R3, [PC, #12]               @ 0x00000000000056c0 
    /*000056b0*/ MOVLS       R2, #1120                   @ 0x460 
    /*000056b4*/ MLALS       R0, R2, R0, R3 
    /*000056b8*/ MOVHI       R0, #0 
    /*000056bc*/ BX          LR 
    /*000056c0*/ MULEQ       R0, R0, R0 
FUNC_END AI_GetChnCtx


FUNC_BEGIN AI_GetDevCtx
    /*000056c4*/ CMP         R0, #0 
    /*000056c8*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*000056cc*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*000056d0*/ MOVNE       R0, #0 
    /*000056d4*/ BX          LR 
FUNC_END AI_GetDevCtx


FUNC_BEGIN AI_DRV_IsVqeEnable
    /*000056d8*/ CLZ         R3, R1 
    /*000056dc*/ LSR         R3, R3, #5 
    /*000056e0*/ CMP         R0, #1 
    /*000056e4*/ ORRHI       R3, R3, #1 
    /*000056e8*/ CMP         R3, #0 
    /*000056ec*/ MOVWEQ      R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*000056f0*/ MOVTEQ      R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*000056f4*/ MOVEQ       IP, #1120                   @ 0x460 
    /*000056f8*/ MVNNE       R3, #0 
    /*000056fc*/ MLAEQ       R0, IP, R0, R2 
    /*00005700*/ LDREQ       R2, [R0, #376]              @ 0x178 
    /*00005704*/ MOV         R0, R3 
    /*00005708*/ STREQ       R2, [R1] 
    /*0000570c*/ BX          LR 
FUNC_END AI_DRV_IsVqeEnable


FUNC_BEGIN AiClrDevAttr
    /*00005710*/ PUSH        {R4, R5, LR} 
    /*00005714*/ MOV         R4, R0 
    /*00005718*/ SUB         SP, SP, #12 
    /*0000571c*/ MOV         R0, #20 
    /*00005720*/ BL          0x0000000000005720          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00005724*/ CMP         R0, #0 
    /*00005728*/ BEQ         AiClrDevAttr_x57ec
    /*0000572c*/ CMP         R4, #0 
    /*00005730*/ BNE         AiClrDevAttr_x5778
    /*00005734*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*00005738*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*0000573c*/ ADD         R0, R4, #124                @ 0x7c 
    /*00005740*/ BL          0x0000000000005740          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00005744*/ SUBS        R2, R0, #0 
    /*00005748*/ BNE         AiClrDevAttr_x57a8
    /*0000574c*/ LDR         R3, [R4, #12] 
    /*00005750*/ CMP         R3, #1 
    /*00005754*/ BEQ         AiClrDevAttr_x57b4
    /*00005758*/ BL          0x0000000000005758          @ AI_DRV_ClrDevAttr [R_ARM_CALL: .text+0x8660] 
    /*0000575c*/ SUBS        R5, R0, #0 
    /*00005760*/ ADD         R0, R4, #124                @ 0x7c 
    /*00005764*/ STREQ       R5, [R4, #8] 
    /*00005768*/ BL          0x0000000000005768          @ osal_up [R_ARM_CALL: +0x0] 
    AiClrDevAttr_x576c:
    /*0000576c*/ MOV         R0, R5 
    /*00005770*/ ADD         SP, SP, #12 
    /*00005774*/ POP         {R4, R5, PC} 
    AiClrDevAttr_x5778:
    /*00005778*/ MOVW        R3, #1246                   @ 0x4de 
    /*0000577c*/ STR         R4, [SP, #4] 
    /*00005780*/ STR         R3, [SP] 
    /*00005784*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12b8] 
    /*00005788*/ LDR         R3, [PC, #104]              @ 0x00000000000057f8 
    /*0000578c*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12b8] 
    /*00005790*/ MOV         R1, #21 
    /*00005794*/ MOV         R0, #3 
    /*00005798*/ MOVW        R5, #32769                  @ 0x8001 
    /*0000579c*/ BL          0x000000000000579C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000057a0*/ MOVT        R5, #40981                  @ 0xa015 
    /*000057a4*/ B           AiClrDevAttr_x576c
    AiClrDevAttr_x57a8:
    /*000057a8*/ MOV         R5, #65024                  @ 0xfe00 
    /*000057ac*/ MOVT        R5, #65535                  @ 0xffff 
    /*000057b0*/ B           AiClrDevAttr_x576c
    AiClrDevAttr_x57b4:
    /*000057b4*/ STR         R2, [SP, #4] 
    /*000057b8*/ MOVW        R3, #1258                   @ 0x4ea 
    /*000057bc*/ MOV         R1, #21 
    /*000057c0*/ STR         R3, [SP] 
    /*000057c4*/ MOVW        R2, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1568] 
    /*000057c8*/ LDR         R3, [PC, #40]               @ 0x00000000000057f8 
    /*000057cc*/ MOVT        R2, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1568] 
    /*000057d0*/ MOV         R0, #3 
    /*000057d4*/ BL          0x00000000000057D4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000057d8*/ ADD         R0, R4, #124                @ 0x7c 
    /*000057dc*/ MOVW        R5, #32777                  @ 0x8009 
    /*000057e0*/ BL          0x00000000000057E0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000057e4*/ MOVT        R5, #40981                  @ 0xa015 
    /*000057e8*/ B           AiClrDevAttr_x576c
    AiClrDevAttr_x57ec:
    /*000057ec*/ MOVW        R5, #32784                  @ 0x8010 
    /*000057f0*/ MOVT        R5, #40981                  @ 0xa015 
    /*000057f4*/ B           AiClrDevAttr_x576c
    /*000057f8*/ ANDEQ       R0, R0, R0, LSR R2 
FUNC_END AiClrDevAttr


FUNC_BEGIN AIProcShow
    /*000057fc*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00005800*/ MOVW        R1, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1630] 
    /*00005804*/ SUB         SP, SP, #148                @ 0x94 
    /*00005808*/ MOV         FP, R0 
    /*0000580c*/ MOVT        R1, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1630] 
    /*00005810*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x8] 
    /*00005814*/ BL          0x0000000000005814          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005818*/ MOV         R0, FP 
    /*0000581c*/ BL          0x000000000000581C          @ AI_DRV_ProcShow [R_ARM_CALL: .text+0x9840] 
    /*00005820*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x8] 
    /*00005824*/ MOV         R0, FP 
    /*00005828*/ MOVW        R1, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1690] 
    /*0000582c*/ MOVT        R1, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1690] 
    /*00005830*/ MOVW        R9, #0                      @ .LC65 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1764] 
    /*00005834*/ BL          0x0000000000005834          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005838*/ MOVT        R9, #0                      @ .LC65 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1764] 
    /*0000583c*/ MOVW        R3, #0                      @ .LC58 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x171c] 
    /*00005840*/ MOVW        R2, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1724] 
    /*00005844*/ MOVT        R3, #0                      @ .LC58 [R_ARM_MOVT_ABS: .rodata.str1.4+0x171c] 
    /*00005848*/ MOVT        R2, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1724] 
    /*0000584c*/ STR         R3, [SP, #28] 
    /*00005850*/ MOV         R0, FP 
    /*00005854*/ STR         R2, [SP, #24] 
    /*00005858*/ MOVW        R3, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x172c] 
    /*0000585c*/ MOVW        R2, #0                      @ .LC61 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1738] 
    /*00005860*/ MOVT        R3, #0                      @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0x172c] 
    /*00005864*/ MOVT        R2, #0                      @ .LC61 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1738] 
    /*00005868*/ STR         R3, [SP, #20] 
    /*0000586c*/ STR         R2, [SP, #16] 
    /*00005870*/ MOVW        R3, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1744] 
    /*00005874*/ MOVW        R2, #0                      @ .LC63 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1750] 
    /*00005878*/ MOVT        R3, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1744] 
    /*0000587c*/ MOVT        R2, #0                      @ .LC63 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1750] 
    /*00005880*/ STR         R3, [SP, #12] 
    /*00005884*/ STR         R2, [SP, #8] 
    /*00005888*/ MOVW        R3, #0                      @ .LC64 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x175c] 
    /*0000588c*/ STR         R9, [SP] 
    /*00005890*/ MOVT        R3, #0                      @ .LC64 [R_ARM_MOVT_ABS: .rodata.str1.4+0x175c] 
    /*00005894*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*00005898*/ STR         R3, [SP, #4] 
    /*0000589c*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*000058a0*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*000058a4*/ MOVW        R1, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16f4] 
    /*000058a8*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*000058ac*/ MOVT        R1, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16f4] 
    /*000058b0*/ BL          0x00000000000058B0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000058b4*/ LDR         R3, [R4, #12] 
    /*000058b8*/ CMP         R3, #0 
    /*000058bc*/ BEQ         AIProcShow_x590c
    /*000058c0*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*000058c4*/ CMP         R3, #0 
    /*000058c8*/ BEQ         AIProcShow_x590c
    /*000058cc*/ MOVW        SL, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*000058d0*/ MOVW        R8, #0                      @ .LC72 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1800] 
    /*000058d4*/ MOVW        R7, #0                      @ .LC76 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1840] 
    /*000058d8*/ MOVT        SL, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*000058dc*/ MOVT        R8, #0                      @ .LC72 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1800] 
    /*000058e0*/ ADD         R5, R4, #136                @ 0x88 
    /*000058e4*/ MOVT        R7, #0                      @ .LC76 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1840] 
    /*000058e8*/ MOV         R6, #0 
    AIProcShow_x58ec:
    /*000058ec*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*000058f0*/ CMP         R3, #1 
    /*000058f4*/ BEQ         AIProcShow_x5f14
    AIProcShow_x58f8:
    /*000058f8*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*000058fc*/ ADD         R6, R6, #1 
    /*00005900*/ ADD         R5, R5, #1120               @ 0x460 
    /*00005904*/ CMP         R3, R6 
    /*00005908*/ BHI         AIProcShow_x58ec
    AIProcShow_x590c:
    /*0000590c*/ MOVW        R1, #0                      @ .LC66 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x176c] 
    /*00005910*/ MOV         R0, FP 
    /*00005914*/ MOVT        R1, #0                      @ .LC66 [R_ARM_MOVT_ABS: .rodata.str1.4+0x176c] 
    /*00005918*/ MOVW        R5, #0                      @ .LC70 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17f0] 
    /*0000591c*/ BL          0x000000000000591C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005920*/ MOVT        R5, #0                      @ .LC70 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17f0] 
    /*00005924*/ STR         R9, [SP] 
    /*00005928*/ MOVW        R2, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17dc] 
    /*0000592c*/ MOVW        R3, #0                      @ .LC69 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17e8] 
    /*00005930*/ MOVT        R2, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17dc] 
    /*00005934*/ MOVT        R3, #0                      @ .LC69 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17e8] 
    /*00005938*/ STR         R2, [SP, #16] 
    /*0000593c*/ STR         R3, [SP, #12] 
    /*00005940*/ MOVW        R2, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17f8] 
    /*00005944*/ STR         R5, [SP, #8] 
    /*00005948*/ MOVT        R2, #0                      @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17f8] 
    /*0000594c*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*00005950*/ STR         R2, [SP, #4] 
    /*00005954*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*00005958*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*0000595c*/ MOVW        R1, #0                      @ .LC67 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17c0] 
    /*00005960*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*00005964*/ MOVT        R1, #0                      @ .LC67 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17c0] 
    /*00005968*/ MOV         R0, FP 
    /*0000596c*/ BL          0x000000000000596C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005970*/ LDR         R3, [R4, #12] 
    /*00005974*/ CMP         R3, #0 
    /*00005978*/ BEQ         AIProcShow_x59cc
    /*0000597c*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00005980*/ CMP         R3, #0 
    /*00005984*/ BEQ         AIProcShow_x59cc
    /*00005988*/ LDR         R6, [PC, #3504]             @ 0x0000000000006740 
    /*0000598c*/ MOVW        SL, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00005990*/ MOVW        R8, #0                      @ .LC90 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1924] 
    /*00005994*/ MOVT        SL, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00005998*/ MOVT        R8, #0                      @ .LC90 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1924] 
    /*0000599c*/ MOV         R7, #0 
    /*000059a0*/ MOVW        R3, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*000059a4*/ MOVT        R3, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*000059a8*/ STR         R3, [SP, #56]               @ 0x38 
    AIProcShow_x59ac:
    /*000059ac*/ LDR         R3, [R6, #208]              @ 0xd0 
    /*000059b0*/ CMP         R3, #1 
    /*000059b4*/ BEQ         AIProcShow_x5e54
    AIProcShow_x59b8:
    /*000059b8*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*000059bc*/ ADD         R7, R7, #1 
    /*000059c0*/ ADD         R6, R6, #1120               @ 0x460 
    /*000059c4*/ CMP         R3, R7 
    /*000059c8*/ BHI         AIProcShow_x59ac
    AIProcShow_x59cc:
    /*000059cc*/ MOVW        R3, #0                      @ .LC80 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18d4] 
    /*000059d0*/ MOVW        R1, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x184c] 
    /*000059d4*/ MOV         R6, R3 
    /*000059d8*/ MOVT        R1, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x184c] 
    /*000059dc*/ MOV         R0, FP 
    /*000059e0*/ MOVW        R8, #0                      @ .LC82 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18e4] 
    /*000059e4*/ BL          0x00000000000059E4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000059e8*/ MOV         R3, R6 
    /*000059ec*/ MOVT        R3, #0                      @ .LC80 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18d4] 
    /*000059f0*/ STR         R5, [SP, #16] 
    /*000059f4*/ STR         R3, [SP, #60]               @ 0x3c 
    /*000059f8*/ MOVT        R8, #0                      @ .LC82 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18e4] 
    /*000059fc*/ MOV         R2, R3 
    /*00005a00*/ STR         R2, [SP, #40]               @ 0x28 
    /*00005a04*/ MOVW        R2, #0                      @ .LC81 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18dc] 
    /*00005a08*/ MOVT        R2, #0                      @ .LC81 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18dc] 
    /*00005a0c*/ STR         R2, [SP, #100]              @ 0x64 
    /*00005a10*/ MOVW        R3, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18cc] 
    /*00005a14*/ STR         R2, [SP, #36]               @ 0x24 
    /*00005a18*/ MOVW        R2, #0                      @ .LC85 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18f8] 
    /*00005a1c*/ MOVT        R2, #0                      @ .LC85 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18f8] 
    /*00005a20*/ MOVT        R3, #0                      @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18cc] 
    /*00005a24*/ STR         R2, [SP, #112]              @ 0x70 
    /*00005a28*/ MOVW        R7, #0                      @ .LC83 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18ec] 
    /*00005a2c*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00005a30*/ MOVT        R7, #0                      @ .LC83 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18ec] 
    /*00005a34*/ STR         R9, [SP] 
    /*00005a38*/ MOVW        R3, #0                      @ .LC84 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18f4] 
    /*00005a3c*/ STR         R8, [SP, #32] 
    /*00005a40*/ MOVT        R3, #0                      @ .LC84 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18f4] 
    /*00005a44*/ STR         R7, [SP, #28] 
    /*00005a48*/ MOVW        R1, #0                      @ .LC78 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18a0] 
    /*00005a4c*/ STR         R3, [SP, #108]              @ 0x6c 
    /*00005a50*/ MOVT        R1, #0                      @ .LC78 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18a0] 
    /*00005a54*/ STR         R3, [SP, #24] 
    /*00005a58*/ MOV         R0, FP 
    /*00005a5c*/ MOV         R3, R2 
    /*00005a60*/ MOVW        R2, #0                      @ .LC86 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1900] 
    /*00005a64*/ STR         R3, [SP, #20] 
    /*00005a68*/ MOVT        R2, #0                      @ .LC86 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1900] 
    /*00005a6c*/ MOVW        R3, #0                      @ .LC87 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1908] 
    /*00005a70*/ STR         R2, [SP, #12] 
    /*00005a74*/ MOVT        R3, #0                      @ .LC87 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1908] 
    /*00005a78*/ MOVW        R2, #0                      @ .LC88 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1910] 
    /*00005a7c*/ STR         R3, [SP, #8] 
    /*00005a80*/ MOVT        R2, #0                      @ .LC88 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1910] 
    /*00005a84*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*00005a88*/ STR         R2, [SP, #4] 
    /*00005a8c*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*00005a90*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*00005a94*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*00005a98*/ BL          0x0000000000005A98          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005a9c*/ LDR         R3, [R4, #12] 
    /*00005aa0*/ CMP         R3, #0 
    /*00005aa4*/ BEQ         AIProcShow_x5ca8
    /*00005aa8*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00005aac*/ CMP         R3, #0 
    /*00005ab0*/ BEQ         AIProcShow_x5ca8
    /*00005ab4*/ LDR         R3, [PC, #3204]             @ 0x0000000000006740 
    /*00005ab8*/ MOVW        R2, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00005abc*/ MOVW        R1, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*00005ac0*/ MOVT        R2, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00005ac4*/ MOVT        R1, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*00005ac8*/ STR         R2, [SP, #128]              @ 0x80 
    /*00005acc*/ MOV         R5, R3 
    /*00005ad0*/ STR         R3, [SP, #96]               @ 0x60 
    /*00005ad4*/ MOV         R3, #0 
    /*00005ad8*/ MOVW        R2, #0                      @ .LC91 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1934] 
    /*00005adc*/ MOV         R6, R3 
    /*00005ae0*/ MOVT        R2, #0                      @ .LC91 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1934] 
    /*00005ae4*/ STR         R3, [SP, #92]               @ 0x5c 
    /*00005ae8*/ STR         R1, [SP, #132]              @ 0x84 
    /*00005aec*/ STR         R2, [SP, #136]              @ 0x88 
    /*00005af0*/ STR         R3, [SP, #88]               @ 0x58 
    /*00005af4*/ STR         R3, [SP, #84]               @ 0x54 
    /*00005af8*/ STR         R3, [SP, #80]               @ 0x50 
    /*00005afc*/ STR         R3, [SP, #76]               @ 0x4c 
    /*00005b00*/ STR         R3, [SP, #72]               @ 0x48 
    /*00005b04*/ STR         R3, [SP, #68]               @ 0x44 
    /*00005b08*/ STR         R3, [SP, #64]               @ 0x40 
    /*00005b0c*/ STR         R3, [SP, #56]               @ 0x38 
    /*00005b10*/ STR         R3, [SP, #104]              @ 0x68 
    /*00005b14*/ STR         R8, [SP, #116]              @ 0x74 
    /*00005b18*/ STR         R7, [SP, #120]              @ 0x78 
    /*00005b1c*/ STR         R9, [SP, #124]              @ 0x7c 
    AIProcShow_x5b20:
    /*00005b20*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*00005b24*/ CMP         R3, #1 
    /*00005b28*/ BEQ         AIProcShow_x5cb4
    AIProcShow_x5b2c:
    /*00005b2c*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00005b30*/ ADD         R6, R6, #1 
    /*00005b34*/ ADD         R5, R5, #1120               @ 0x460 
    /*00005b38*/ CMP         R3, R6 
    /*00005b3c*/ BHI         AIProcShow_x5b20
    /*00005b40*/ LDR         R3, [SP, #104]              @ 0x68 
    /*00005b44*/ LDR         R8, [SP, #116]              @ 0x74 
    /*00005b48*/ CMP         R3, #0 
    /*00005b4c*/ LDR         R7, [SP, #120]              @ 0x78 
    /*00005b50*/ LDR         R9, [SP, #124]              @ 0x7c 
    /*00005b54*/ BEQ         AIProcShow_x5ca8
    /*00005b58*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00005b5c*/ CMP         R3, #0 
    /*00005b60*/ LDREQ       SL, [SP, #56]               @ 0x38 
    /*00005b64*/ BNE         AIProcShow_x6f1c
    AIProcShow_x5b68:
    /*00005b68*/ LDR         R3, [SP, #64]               @ 0x40 
    /*00005b6c*/ CMP         R3, #0 
    /*00005b70*/ BNE         AIProcShow_x6dec
    AIProcShow_x5b74:
    /*00005b74*/ LDR         R3, [SP, #72]               @ 0x48 
    /*00005b78*/ CMP         R3, #0 
    /*00005b7c*/ BNE         AIProcShow_x6ca8
    AIProcShow_x5b80:
    /*00005b80*/ LDR         R3, [SP, #76]               @ 0x4c 
    /*00005b84*/ CMP         R3, #0 
    /*00005b88*/ BNE         AIProcShow_x6bec
    AIProcShow_x5b8c:
    /*00005b8c*/ LDR         R3, [SP, #80]               @ 0x50 
    /*00005b90*/ CMP         R3, #0 
    /*00005b94*/ BNE         AIProcShow_x6b1c
    AIProcShow_x5b98:
    /*00005b98*/ LDR         R3, [SP, #68]               @ 0x44 
    /*00005b9c*/ CMP         R3, #0 
    /*00005ba0*/ BNE         AIProcShow_x6744
    AIProcShow_x5ba4:
    /*00005ba4*/ LDR         R3, [SP, #88]               @ 0x58 
    /*00005ba8*/ CMP         R3, #0 
    /*00005bac*/ BNE         AIProcShow_x64b8
    AIProcShow_x5bb0:
    /*00005bb0*/ LDR         R3, [SP, #92]               @ 0x5c 
    /*00005bb4*/ CMP         R3, #0 
    /*00005bb8*/ BNE         AIProcShow_x5fdc
    AIProcShow_x5bbc:
    /*00005bbc*/ LDR         R3, [SP, #84]               @ 0x54 
    /*00005bc0*/ CMP         R3, #0 
    /*00005bc4*/ BEQ         AIProcShow_x5ca8
    /*00005bc8*/ ADD         R2, SL, #1 
    /*00005bcc*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00005bd0*/ MOV         R0, FP 
    /*00005bd4*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*00005bd8*/ BL          0x0000000000005BD8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005bdc*/ LDR         R3, [SP, #100]              @ 0x64 
    /*00005be0*/ STR         R9, [SP] 
    /*00005be4*/ MOVW        R2, #0                      @ .LC225 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2064] 
    /*00005be8*/ MOVW        R1, #0                      @ .LC224 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2040] 
    /*00005bec*/ MOVT        R2, #0                      @ .LC225 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2064] 
    /*00005bf0*/ STR         R3, [SP, #4] 
    /*00005bf4*/ MOV         R0, FP 
    /*00005bf8*/ STR         R2, [SP, #28] 
    /*00005bfc*/ MOVW        R3, #0                      @ .LC226 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2070] 
    /*00005c00*/ MOVW        R2, #0                      @ .LC227 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2078] 
    /*00005c04*/ MOVT        R3, #0                      @ .LC226 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2070] 
    /*00005c08*/ MOVT        R2, #0                      @ .LC227 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2078] 
    /*00005c0c*/ STR         R3, [SP, #24] 
    /*00005c10*/ STR         R2, [SP, #20] 
    /*00005c14*/ MOVW        R3, #0                      @ .LC228 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2084] 
    /*00005c18*/ MOVW        R2, #0                      @ .LC105 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a6c] 
    /*00005c1c*/ MOVT        R3, #0                      @ .LC228 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2084] 
    /*00005c20*/ MOVT        R2, #0                      @ .LC105 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a6c] 
    /*00005c24*/ STR         R3, [SP, #16] 
    /*00005c28*/ STR         R2, [SP, #12] 
    /*00005c2c*/ MOVW        R3, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19ec] 
    /*00005c30*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*00005c34*/ MOVT        R3, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19ec] 
    /*00005c38*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*00005c3c*/ STR         R3, [SP, #8] 
    /*00005c40*/ MOVT        R1, #0                      @ .LC224 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2040] 
    /*00005c44*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*00005c48*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*00005c4c*/ BL          0x0000000000005C4C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005c50*/ LDR         R3, [R4, #12] 
    /*00005c54*/ CMP         R3, #0 
    /*00005c58*/ BEQ         AIProcShow_x5ca8
    /*00005c5c*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00005c60*/ CMP         R3, #0 
    /*00005c64*/ BEQ         AIProcShow_x5ca8
    /*00005c68*/ LDR         R9, [SP, #96]               @ 0x60 
    /*00005c6c*/ MOVW        R8, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00005c70*/ MOVW        R7, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*00005c74*/ MOVW        R6, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00005c78*/ MOVT        R8, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00005c7c*/ MOVT        R7, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*00005c80*/ MOVT        R6, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00005c84*/ MOV         R5, #0 
    AIProcShow_x5c88:
    /*00005c88*/ LDR         R3, [R9, #208]              @ 0xd0 
    /*00005c8c*/ CMP         R3, #1 
    /*00005c90*/ BEQ         AIProcShow_x7010
    AIProcShow_x5c94:
    /*00005c94*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00005c98*/ ADD         R5, R5, #1 
    /*00005c9c*/ ADD         R9, R9, #1120               @ 0x460 
    /*00005ca0*/ CMP         R5, R3 
    /*00005ca4*/ BCC         AIProcShow_x5c88
    AIProcShow_x5ca8:
    /*00005ca8*/ MOV         R0, #0 
    /*00005cac*/ ADD         SP, SP, #148                @ 0x94 
    /*00005cb0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AIProcShow_x5cb4:
    /*00005cb4*/ LDR         R1, [R5, #240]              @ 0xf0 
    /*00005cb8*/ MOV         R2, #0 
    /*00005cbc*/ LDR         LR, [SP, #104]              @ 0x68 
    /*00005cc0*/ MOV         R3, R6 
    /*00005cc4*/ CMP         R1, R2 
    /*00005cc8*/ LDR         IP, [R5, #252]              @ 0xfc 
    /*00005ccc*/ LDR         R0, [R5, #260]              @ 0x104 
    /*00005cd0*/ MOVNE       LR, #1 
    /*00005cd4*/ STR         LR, [SP, #104]              @ 0x68 
    /*00005cd8*/ LDR         LR, [SP, #56]               @ 0x38 
    /*00005cdc*/ CMP         IP, R2 
    /*00005ce0*/ LDR         R1, [SP, #128]              @ 0x80 
    /*00005ce4*/ MOVNE       LR, #1 
    /*00005ce8*/ CMP         R0, R2 
    /*00005cec*/ STR         LR, [SP, #56]               @ 0x38 
    /*00005cf0*/ LDR         LR, [SP, #64]               @ 0x40 
    /*00005cf4*/ MOVNE       LR, #1 
    /*00005cf8*/ STR         R1, [SP] 
    /*00005cfc*/ LDR         IP, [R5, #248]              @ 0xf8 
    /*00005d00*/ STR         LR, [SP, #64]               @ 0x40 
    /*00005d04*/ LDR         LR, [SP, #68]               @ 0x44 
    /*00005d08*/ CMP         IP, R2 
    /*00005d0c*/ LDR         R0, [R5, #244]              @ 0xf4 
    /*00005d10*/ MOVNE       LR, #1 
    /*00005d14*/ LDR         IP, [R5, #264]              @ 0x108 
    /*00005d18*/ STR         LR, [SP, #68]               @ 0x44 
    /*00005d1c*/ CMP         R0, R2 
    /*00005d20*/ LDR         LR, [SP, #76]               @ 0x4c 
    /*00005d24*/ MOVNE       LR, #1 
    /*00005d28*/ LDR         R0, [R5, #256]              @ 0x100 
    /*00005d2c*/ CMP         IP, R2 
    /*00005d30*/ LDR         IP, [R5, #268]              @ 0x10c 
    /*00005d34*/ STR         LR, [SP, #76]               @ 0x4c 
    /*00005d38*/ LDR         LR, [SP, #72]               @ 0x48 
    /*00005d3c*/ MOVNE       LR, #1 
    /*00005d40*/ CMP         R0, R2 
    /*00005d44*/ LDR         R0, [R5, #272]              @ 0x110 
    /*00005d48*/ LDR         R1, [SP, #132]              @ 0x84 
    /*00005d4c*/ STR         LR, [SP, #72]               @ 0x48 
    /*00005d50*/ LDR         LR, [SP, #80]               @ 0x50 
    /*00005d54*/ MOVNE       LR, #1 
    /*00005d58*/ CMP         IP, R2 
    /*00005d5c*/ LDR         IP, [R5, #276]              @ 0x114 
    /*00005d60*/ STR         LR, [SP, #80]               @ 0x50 
    /*00005d64*/ LDR         LR, [SP, #84]               @ 0x54 
    /*00005d68*/ MOVNE       LR, #1 
    /*00005d6c*/ CMP         R0, R2 
    /*00005d70*/ MOV         R0, FP 
    /*00005d74*/ STR         LR, [SP, #84]               @ 0x54 
    /*00005d78*/ LDR         LR, [SP, #88]               @ 0x58 
    /*00005d7c*/ MOVNE       LR, #1 
    /*00005d80*/ CMP         IP, R2 
    /*00005d84*/ LDR         IP, [SP, #92]               @ 0x5c 
    /*00005d88*/ STR         LR, [SP, #88]               @ 0x58 
    /*00005d8c*/ MOVNE       IP, #1 
    /*00005d90*/ STR         IP, [SP, #92]               @ 0x5c 
    /*00005d94*/ BL          0x0000000000005D94          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005d98*/ LDR         R3, [R5, #240]              @ 0xf0 
    /*00005d9c*/ CMP         R3, #1 
    /*00005da0*/ BEQ         AIProcShow_x7004
    /*00005da4*/ CMP         R3, #0 
    /*00005da8*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00005dac*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00005db0*/ MOVNE       R9, #0 
    /*00005db4*/ MOVEQ       R9, R2 
    AIProcShow_x5db8:
    /*00005db8*/ LDR         R3, [R5, #308]              @ 0x134 
    /*00005dbc*/ CMP         R3, #0 
    /*00005dc0*/ MOVWEQ      R3, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1608] 
    /*00005dc4*/ MOVTEQ      R3, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1608] 
    /*00005dc8*/ BNE         AIProcShow_x5fc0
    AIProcShow_x5dcc:
    /*00005dcc*/ LDR         R2, [R5, #284]              @ 0x11c 
    /*00005dd0*/ CMP         R2, #8000                   @ 0x1f40 
    /*00005dd4*/ BEQ         AIProcShow_x712c
    /*00005dd8*/ MOVW        R1, #11025                  @ 0x2b11 
    /*00005ddc*/ CMP         R2, R1 
    /*00005de0*/ BEQ         AIProcShow_x72a8
    /*00005de4*/ MOVW        R1, #12000                  @ 0x2ee0 
    /*00005de8*/ CMP         R2, R1 
    /*00005dec*/ BEQ         AIProcShow_x7df0
    /*00005df0*/ CMP         R2, #16000                  @ 0x3e80 
    /*00005df4*/ BEQ         AIProcShow_x7e20
    /*00005df8*/ MOVW        R1, #22050                  @ 0x5622 
    /*00005dfc*/ CMP         R2, R1 
    /*00005e00*/ BEQ         AIProcShow_x7e50
    /*00005e04*/ MOVW        R1, #24000                  @ 0x5dc0 
    /*00005e08*/ CMP         R2, R1 
    /*00005e0c*/ BEQ         AIProcShow_x7e68
    /*00005e10*/ CMP         R2, #32000                  @ 0x7d00 
    /*00005e14*/ BEQ         AIProcShow_x7e98
    /*00005e18*/ MOVW        R1, #44100                  @ 0xac44 
    /*00005e1c*/ CMP         R2, R1 
    /*00005e20*/ BEQ         AIProcShow_x72d8
    /*00005e24*/ MOVW        R1, #48000                  @ 0xbb80 
    /*00005e28*/ CMP         R2, R1 
    /*00005e2c*/ BEQ         AIProcShow_x7f00
    /*00005e30*/ CMP         R2, #64000                  @ 0xfa00 
    /*00005e34*/ BEQ         AIProcShow_x7f0c
    /*00005e38*/ MOV         R1, #30464                  @ 0x7700 
    /*00005e3c*/ MOVT        R1, #1 
    /*00005e40*/ CMP         R2, R1 
    /*00005e44*/ MOVW        R8, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15f8] 
    /*00005e48*/ MOVT        R8, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15f8] 
    /*00005e4c*/ MOVNE       R8, #0 
    /*00005e50*/ B           AIProcShow_x7134
    AIProcShow_x5e54:
    /*00005e54*/ MOV         R3, R7 
    /*00005e58*/ STR         SL, [SP] 
    /*00005e5c*/ MOV         R2, #0 
    /*00005e60*/ LDR         R1, [SP, #56]               @ 0x38 
    /*00005e64*/ MOV         R0, FP 
    /*00005e68*/ BL          0x0000000000005E68          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005e6c*/ LDR         R3, [R6, #564]              @ 0x234 
    /*00005e70*/ CMP         R3, #1 
    /*00005e74*/ BEQ         AIProcShow_x6ff8
    /*00005e78*/ CMP         R3, #0 
    /*00005e7c*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00005e80*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00005e84*/ MOVNE       R2, #0 
    AIProcShow_x5e88:
    /*00005e88*/ LDR         R1, [R6, #572]              @ 0x23c 
    /*00005e8c*/ LDR         R3, [R6, #568]              @ 0x238 
    /*00005e90*/ CMP         R1, #8000                   @ 0x1f40 
    /*00005e94*/ BEQ         AIProcShow_x7098
    /*00005e98*/ MOVW        R0, #11025                  @ 0x2b11 
    /*00005e9c*/ CMP         R1, R0 
    /*00005ea0*/ BEQ         AIProcShow_x72b4
    /*00005ea4*/ MOVW        R0, #12000                  @ 0x2ee0 
    /*00005ea8*/ CMP         R1, R0 
    /*00005eac*/ BEQ         AIProcShow_x7dfc
    /*00005eb0*/ CMP         R1, #16000                  @ 0x3e80 
    /*00005eb4*/ BEQ         AIProcShow_x7e14
    /*00005eb8*/ MOVW        R0, #22050                  @ 0x5622 
    /*00005ebc*/ CMP         R1, R0 
    /*00005ec0*/ BEQ         AIProcShow_x7e44
    /*00005ec4*/ MOVW        R0, #24000                  @ 0x5dc0 
    /*00005ec8*/ CMP         R1, R0 
    /*00005ecc*/ BEQ         AIProcShow_x7e74
    /*00005ed0*/ CMP         R1, #32000                  @ 0x7d00 
    /*00005ed4*/ BEQ         AIProcShow_x7e80
    /*00005ed8*/ MOVW        R0, #44100                  @ 0xac44 
    /*00005edc*/ CMP         R1, R0 
    /*00005ee0*/ BEQ         AIProcShow_x72cc
    /*00005ee4*/ MOVW        R0, #48000                  @ 0xbb80 
    /*00005ee8*/ CMP         R1, R0 
    /*00005eec*/ BEQ         AIProcShow_x7ef4
    /*00005ef0*/ CMP         R1, #64000                  @ 0xfa00 
    /*00005ef4*/ BEQ         AIProcShow_x7f24
    /*00005ef8*/ MOV         IP, #30464                  @ 0x7700 
    /*00005efc*/ MOVT        IP, #1 
    /*00005f00*/ CMP         R1, IP 
    /*00005f04*/ MOVW        R0, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15f8] 
    /*00005f08*/ MOVT        R0, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15f8] 
    /*00005f0c*/ MOVNE       R0, #0 
    /*00005f10*/ B           AIProcShow_x70a0
    AIProcShow_x5f14:
    /*00005f14*/ LDR         R1, [R5, #148]              @ 0x94 
    /*00005f18*/ MOV         R3, R6 
    /*00005f1c*/ LDR         R0, [R5, #232]              @ 0xe8 
    /*00005f20*/ MOV         R2, #0 
    /*00005f24*/ LDR         IP, [R5, #228]              @ 0xe4 
    /*00005f28*/ STR         R1, [SP, #12] 
    /*00005f2c*/ MOV         R1, R8 
    /*00005f30*/ STR         R0, [SP, #8] 
    /*00005f34*/ MOV         R0, FP 
    /*00005f38*/ STR         SL, [SP] 
    /*00005f3c*/ STR         IP, [SP, #4] 
    /*00005f40*/ BL          0x0000000000005F40          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005f44*/ LDR         R3, [R5, #24] 
    /*00005f48*/ MOVW        R1, #0                      @ .LC74 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1820] 
    /*00005f4c*/ MOV         R0, FP 
    /*00005f50*/ CMP         R3, #0 
    /*00005f54*/ MOVT        R1, #0                      @ .LC74 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1820] 
    /*00005f58*/ BEQ         AIProcShow_x7dd0
    /*00005f5c*/ LDRH        R2, [R3] 
    /*00005f60*/ STR         R3, [SP, #56]               @ 0x38 
    /*00005f64*/ ADD         R2, R2, #8 
    /*00005f68*/ UXTH        R2, R2 
    /*00005f6c*/ CMP         R2, #16 
    /*00005f70*/ MOVLS       R2, #0 
    /*00005f74*/ MOVHI       R2, #1 
    /*00005f78*/ BL          0x0000000000005F78          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005f7c*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00005f80*/ MOVW        R1, #0                      @ .LC74 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1820] 
    /*00005f84*/ MOV         R0, FP 
    /*00005f88*/ MOVT        R1, #0                      @ .LC74 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1820] 
    /*00005f8c*/ LDRH        R2, [R3, #2] 
    /*00005f90*/ ADD         R2, R2, #8 
    /*00005f94*/ UXTH        R2, R2 
    /*00005f98*/ CMP         R2, #16 
    /*00005f9c*/ MOVLS       R2, #0 
    /*00005fa0*/ MOVHI       R2, #1 
    /*00005fa4*/ BL          0x0000000000005FA4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    AIProcShow_x5fa8:
    /*00005fa8*/ LDR         R3, [R5, #1108]             @ 0x454 
    /*00005fac*/ MOV         R1, R7 
    /*00005fb0*/ LDR         R2, [R5, #1104]             @ 0x450 
    /*00005fb4*/ MOV         R0, FP 
    /*00005fb8*/ BL          0x0000000000005FB8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005fbc*/ B           AIProcShow_x58f8
    AIProcShow_x5fc0:
    /*00005fc0*/ CMP         R3, #1 
    /*00005fc4*/ BEQ         AIProcShow_x7de4
    /*00005fc8*/ CMP         R3, #2 
    /*00005fcc*/ MOVW        R3, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1618] 
    /*00005fd0*/ MOVT        R3, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1618] 
    /*00005fd4*/ MOVNE       R3, #0 
    /*00005fd8*/ B           AIProcShow_x5dcc
    AIProcShow_x5fdc:
    /*00005fdc*/ ADD         R2, SL, #1 
    /*00005fe0*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00005fe4*/ MOV         R0, FP 
    /*00005fe8*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*00005fec*/ BL          0x0000000000005FEC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00005ff0*/ MOVW        R3, #0                      @ .LC182 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ef4] 
    /*00005ff4*/ MOVT        R3, #0                      @ .LC182 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ef4] 
    /*00005ff8*/ STR         R3, [SP, #48]               @ 0x30 
    /*00005ffc*/ MOVW        R3, #0                      @ .LC184 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f04] 
    /*00006000*/ MOVT        R3, #0                      @ .LC184 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f04] 
    /*00006004*/ STR         R3, [SP, #40]               @ 0x28 
    /*00006008*/ MOVW        R3, #0                      @ .LC186 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f14] 
    /*0000600c*/ MOVT        R3, #0                      @ .LC186 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f14] 
    /*00006010*/ STR         R3, [SP, #32] 
    /*00006014*/ MOVW        R3, #0                      @ .LC188 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f24] 
    /*00006018*/ MOVT        R3, #0                      @ .LC188 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f24] 
    /*0000601c*/ STR         R3, [SP, #24] 
    /*00006020*/ MOVW        R3, #0                      @ .LC190 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f34] 
    /*00006024*/ MOVT        R3, #0                      @ .LC190 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f34] 
    /*00006028*/ STR         R3, [SP, #16] 
    /*0000602c*/ MOVW        R3, #0                      @ .LC192 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f44] 
    /*00006030*/ MOVT        R3, #0                      @ .LC192 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f44] 
    /*00006034*/ MOVW        R8, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f3c] 
    /*00006038*/ STR         R3, [SP, #56]               @ 0x38 
    /*0000603c*/ STR         R9, [SP] 
    /*00006040*/ MOVW        R2, #0                      @ .LC181 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eec] 
    /*00006044*/ MOVT        R8, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f3c] 
    /*00006048*/ MOVT        R2, #0                      @ .LC181 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eec] 
    /*0000604c*/ MOVW        R5, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19ec] 
    /*00006050*/ STR         R2, [SP, #52]               @ 0x34 
    /*00006054*/ STR         R8, [SP, #12] 
    /*00006058*/ MOVW        R2, #0                      @ .LC183 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1efc] 
    /*0000605c*/ MOVT        R5, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19ec] 
    /*00006060*/ MOVT        R2, #0                      @ .LC183 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1efc] 
    /*00006064*/ STR         R5, [SP, #8] 
    /*00006068*/ MOVW        R1, #0                      @ .LC180 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eb8] 
    /*0000606c*/ STR         R2, [SP, #44]               @ 0x2c 
    /*00006070*/ MOVT        R1, #0                      @ .LC180 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eb8] 
    /*00006074*/ MOVW        R2, #0                      @ .LC185 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f0c] 
    /*00006078*/ MOV         R0, FP 
    /*0000607c*/ MOVT        R2, #0                      @ .LC185 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f0c] 
    /*00006080*/ STR         R2, [SP, #36]               @ 0x24 
    /*00006084*/ MOVW        R2, #0                      @ .LC187 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f1c] 
    /*00006088*/ MOVT        R2, #0                      @ .LC187 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f1c] 
    /*0000608c*/ STR         R2, [SP, #28] 
    /*00006090*/ MOVW        R2, #0                      @ .LC189 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f2c] 
    /*00006094*/ MOVT        R2, #0                      @ .LC189 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f2c] 
    /*00006098*/ STR         R2, [SP, #20] 
    /*0000609c*/ MOV         R2, R3 
    /*000060a0*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*000060a4*/ STR         R2, [SP, #4] 
    /*000060a8*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*000060ac*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*000060b0*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*000060b4*/ BL          0x00000000000060B4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000060b8*/ LDR         R3, [R4, #12] 
    /*000060bc*/ CMP         R3, #0 
    /*000060c0*/ BEQ         AIProcShow_x611c
    /*000060c4*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*000060c8*/ CMP         R3, #0 
    /*000060cc*/ BEQ         AIProcShow_x611c
    /*000060d0*/ LDR         R6, [SP, #96]               @ 0x60 
    /*000060d4*/ MOV         R7, #0 
    /*000060d8*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*000060dc*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*000060e0*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*000060e4*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*000060e8*/ STR         R3, [SP, #64]               @ 0x40 
    /*000060ec*/ MOVW        R3, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*000060f0*/ STR         R2, [SP, #68]               @ 0x44 
    /*000060f4*/ MOVT        R3, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*000060f8*/ STR         R3, [SP, #60]               @ 0x3c 
    AIProcShow_x60fc:
    /*000060fc*/ LDR         R3, [R6, #208]              @ 0xd0 
    /*00006100*/ CMP         R3, #1 
    /*00006104*/ BEQ         AIProcShow_x7ac4
    AIProcShow_x6108:
    /*00006108*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*0000610c*/ ADD         R7, R7, #1 
    /*00006110*/ ADD         R6, R6, #1120               @ 0x460 
    /*00006114*/ CMP         R7, R3 
    /*00006118*/ BCC         AIProcShow_x60fc
    AIProcShow_x611c:
    /*0000611c*/ ADD         R2, SL, #2 
    /*00006120*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00006124*/ MOV         R0, FP 
    /*00006128*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*0000612c*/ BL          0x000000000000612C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006130*/ LDR         R2, [SP, #56]               @ 0x38 
    /*00006134*/ STR         R8, [SP, #12] 
    /*00006138*/ MOVW        R3, #0                      @ .LC193 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f4c] 
    /*0000613c*/ STR         R5, [SP, #8] 
    /*00006140*/ MOVT        R3, #0                      @ .LC193 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f4c] 
    /*00006144*/ STR         R2, [SP, #4] 
    /*00006148*/ MOVW        R1, #0                      @ .LC180 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eb8] 
    /*0000614c*/ STR         R3, [SP, #52]               @ 0x34 
    /*00006150*/ MOVW        R2, #0                      @ .LC194 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f54] 
    /*00006154*/ STR         R9, [SP] 
    /*00006158*/ MOVT        R2, #0                      @ .LC194 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f54] 
    /*0000615c*/ MOVW        R3, #0                      @ .LC195 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f5c] 
    /*00006160*/ STR         R2, [SP, #48]               @ 0x30 
    /*00006164*/ MOVT        R3, #0                      @ .LC195 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f5c] 
    /*00006168*/ MOVW        R2, #0                      @ .LC196 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f64] 
    /*0000616c*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00006170*/ MOVT        R2, #0                      @ .LC196 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f64] 
    /*00006174*/ MOVW        R3, #0                      @ .LC197 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f6c] 
    /*00006178*/ STR         R2, [SP, #40]               @ 0x28 
    /*0000617c*/ MOVT        R3, #0                      @ .LC197 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f6c] 
    /*00006180*/ MOVW        R2, #0                      @ .LC198 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f74] 
    /*00006184*/ STR         R3, [SP, #36]               @ 0x24 
    /*00006188*/ MOVT        R2, #0                      @ .LC198 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f74] 
    /*0000618c*/ MOVW        R3, #0                      @ .LC199 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f7c] 
    /*00006190*/ STR         R2, [SP, #32] 
    /*00006194*/ MOVT        R3, #0                      @ .LC199 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f7c] 
    /*00006198*/ MOVW        R2, #0                      @ .LC200 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f84] 
    /*0000619c*/ STR         R3, [SP, #28] 
    /*000061a0*/ MOVT        R2, #0                      @ .LC200 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f84] 
    /*000061a4*/ MOVW        R3, #0                      @ .LC201 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f8c] 
    /*000061a8*/ STR         R2, [SP, #24] 
    /*000061ac*/ MOVT        R3, #0                      @ .LC201 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f8c] 
    /*000061b0*/ MOVW        R2, #0                      @ .LC202 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f94] 
    /*000061b4*/ STR         R3, [SP, #20] 
    /*000061b8*/ MOVT        R2, #0                      @ .LC202 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f94] 
    /*000061bc*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*000061c0*/ STR         R2, [SP, #16] 
    /*000061c4*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*000061c8*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*000061cc*/ MOVT        R1, #0                      @ .LC180 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eb8] 
    /*000061d0*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*000061d4*/ MOV         R0, FP 
    /*000061d8*/ BL          0x00000000000061D8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000061dc*/ LDR         R3, [R4, #12] 
    /*000061e0*/ CMP         R3, #0 
    /*000061e4*/ BEQ         AIProcShow_x6248
    /*000061e8*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*000061ec*/ CMP         R3, #0 
    /*000061f0*/ BEQ         AIProcShow_x6248
    /*000061f4*/ LDR         R6, [SP, #96]               @ 0x60 
    /*000061f8*/ MOV         R7, #0 
    /*000061fc*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00006200*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*00006204*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00006208*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*0000620c*/ STR         R3, [SP, #68]               @ 0x44 
    /*00006210*/ MOVW        R3, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00006214*/ STR         R2, [SP, #72]               @ 0x48 
    /*00006218*/ MOVT        R3, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*0000621c*/ STR         R8, [SP, #64]               @ 0x40 
    /*00006220*/ STR         R3, [SP, #60]               @ 0x3c 
    AIProcShow_x6224:
    /*00006224*/ LDR         R3, [R6, #208]              @ 0xd0 
    /*00006228*/ CMP         R3, #1 
    /*0000622c*/ BEQ         AIProcShow_x79e0
    AIProcShow_x6230:
    /*00006230*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006234*/ ADD         R7, R7, #1 
    /*00006238*/ ADD         R6, R6, #1120               @ 0x460 
    /*0000623c*/ CMP         R7, R3 
    /*00006240*/ BCC         AIProcShow_x6224
    /*00006244*/ LDR         R8, [SP, #64]               @ 0x40 
    AIProcShow_x6248:
    /*00006248*/ ADD         R2, SL, #3 
    /*0000624c*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00006250*/ MOV         R0, FP 
    /*00006254*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*00006258*/ BL          0x0000000000006258          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000625c*/ LDR         R2, [SP, #56]               @ 0x38 
    /*00006260*/ STR         R8, [SP, #12] 
    /*00006264*/ MOVW        R3, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fc8] 
    /*00006268*/ STR         R5, [SP, #8] 
    /*0000626c*/ MOVT        R3, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fc8] 
    /*00006270*/ STR         R2, [SP, #4] 
    /*00006274*/ MOVW        R1, #0                      @ .LC180 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eb8] 
    /*00006278*/ STR         R3, [SP, #52]               @ 0x34 
    /*0000627c*/ MOVW        R2, #0                      @ .LC205 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fd0] 
    /*00006280*/ STR         R9, [SP] 
    /*00006284*/ MOVT        R2, #0                      @ .LC205 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fd0] 
    /*00006288*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fd8] 
    /*0000628c*/ STR         R2, [SP, #48]               @ 0x30 
    /*00006290*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fd8] 
    /*00006294*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fe0] 
    /*00006298*/ STR         R3, [SP, #44]               @ 0x2c 
    /*0000629c*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fe0] 
    /*000062a0*/ MOVW        R3, #0                      @ .LC208 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fe8] 
    /*000062a4*/ STR         R2, [SP, #40]               @ 0x28 
    /*000062a8*/ MOVT        R3, #0                      @ .LC208 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fe8] 
    /*000062ac*/ MOVW        R2, #0                      @ .LC209 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ff0] 
    /*000062b0*/ STR         R3, [SP, #36]               @ 0x24 
    /*000062b4*/ MOVT        R2, #0                      @ .LC209 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ff0] 
    /*000062b8*/ MOVW        R3, #0                      @ .LC210 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ff8] 
    /*000062bc*/ STR         R2, [SP, #32] 
    /*000062c0*/ MOVT        R3, #0                      @ .LC210 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ff8] 
    /*000062c4*/ MOVW        R2, #0                      @ .LC211 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2000] 
    /*000062c8*/ STR         R3, [SP, #28] 
    /*000062cc*/ MOVT        R2, #0                      @ .LC211 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2000] 
    /*000062d0*/ MOVW        R3, #0                      @ .LC212 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2008] 
    /*000062d4*/ STR         R2, [SP, #24] 
    /*000062d8*/ MOVT        R3, #0                      @ .LC212 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2008] 
    /*000062dc*/ MOVW        R2, #0                      @ .LC213 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2010] 
    /*000062e0*/ STR         R3, [SP, #20] 
    /*000062e4*/ MOVT        R2, #0                      @ .LC213 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2010] 
    /*000062e8*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*000062ec*/ STR         R2, [SP, #16] 
    /*000062f0*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*000062f4*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*000062f8*/ MOVT        R1, #0                      @ .LC180 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eb8] 
    /*000062fc*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*00006300*/ MOV         R0, FP 
    /*00006304*/ BL          0x0000000000006304          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006308*/ LDR         R3, [R4, #12] 
    /*0000630c*/ CMP         R3, #0 
    /*00006310*/ BEQ         AIProcShow_x6384
    /*00006314*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006318*/ CMP         R3, #0 
    /*0000631c*/ BEQ         AIProcShow_x6384
    /*00006320*/ LDR         R6, [SP, #96]               @ 0x60 
    /*00006324*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00006328*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*0000632c*/ STR         R3, [SP, #64]               @ 0x40 
    /*00006330*/ MOVW        R3, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00006334*/ MOVT        R3, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00006338*/ STR         R3, [SP, #60]               @ 0x3c 
    /*0000633c*/ MOV         R3, R5 
    /*00006340*/ MOV         R7, #0 
    /*00006344*/ MOV         R5, SL 
    /*00006348*/ MOV         SL, R3 
    /*0000634c*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*00006350*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*00006354*/ STR         R2, [SP, #68]               @ 0x44 
    AIProcShow_x6358:
    /*00006358*/ LDR         R3, [R6, #208]              @ 0xd0 
    /*0000635c*/ CMP         R3, #1 
    /*00006360*/ BEQ         AIProcShow_x7ce0
    AIProcShow_x6364:
    /*00006364*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006368*/ ADD         R7, R7, #1 
    /*0000636c*/ ADD         R6, R6, #1120               @ 0x460 
    /*00006370*/ CMP         R7, R3 
    /*00006374*/ BCC         AIProcShow_x6358
    /*00006378*/ MOV         R3, SL 
    /*0000637c*/ MOV         SL, R5 
    /*00006380*/ MOV         R5, R3 
    AIProcShow_x6384:
    /*00006384*/ ADD         SL, SL, #4 
    /*00006388*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*0000638c*/ MOV         R0, FP 
    /*00006390*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*00006394*/ MOV         R2, SL 
    /*00006398*/ BL          0x0000000000006398          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000639c*/ LDR         R2, [SP, #56]               @ 0x38 
    /*000063a0*/ STR         R8, [SP, #12] 
    /*000063a4*/ MOVW        R3, #0                      @ .LC214 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2018] 
    /*000063a8*/ STR         R5, [SP, #8] 
    /*000063ac*/ MOVT        R3, #0                      @ .LC214 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2018] 
    /*000063b0*/ STR         R2, [SP, #4] 
    /*000063b4*/ MOVW        R1, #0                      @ .LC180 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eb8] 
    /*000063b8*/ STR         R3, [SP, #52]               @ 0x34 
    /*000063bc*/ MOVW        R2, #0                      @ .LC215 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x201c] 
    /*000063c0*/ STR         R9, [SP] 
    /*000063c4*/ MOVT        R2, #0                      @ .LC215 [R_ARM_MOVT_ABS: .rodata.str1.4+0x201c] 
    /*000063c8*/ MOVW        R3, #0                      @ .LC216 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2020] 
    /*000063cc*/ STR         R2, [SP, #48]               @ 0x30 
    /*000063d0*/ MOVT        R3, #0                      @ .LC216 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2020] 
    /*000063d4*/ MOVW        R2, #0                      @ .LC217 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2024] 
    /*000063d8*/ STR         R3, [SP, #44]               @ 0x2c 
    /*000063dc*/ MOVT        R2, #0                      @ .LC217 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2024] 
    /*000063e0*/ MOVW        R3, #0                      @ .LC218 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2028] 
    /*000063e4*/ STR         R2, [SP, #40]               @ 0x28 
    /*000063e8*/ MOVT        R3, #0                      @ .LC218 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2028] 
    /*000063ec*/ MOVW        R2, #0                      @ .LC219 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x202c] 
    /*000063f0*/ STR         R3, [SP, #36]               @ 0x24 
    /*000063f4*/ MOVT        R2, #0                      @ .LC219 [R_ARM_MOVT_ABS: .rodata.str1.4+0x202c] 
    /*000063f8*/ MOVW        R3, #0                      @ .LC220 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2030] 
    /*000063fc*/ STR         R2, [SP, #32] 
    /*00006400*/ MOVT        R3, #0                      @ .LC220 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2030] 
    /*00006404*/ MOVW        R2, #0                      @ .LC221 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2034] 
    /*00006408*/ STR         R3, [SP, #28] 
    /*0000640c*/ MOVT        R2, #0                      @ .LC221 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2034] 
    /*00006410*/ MOVW        R3, #0                      @ .LC222 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2038] 
    /*00006414*/ STR         R2, [SP, #24] 
    /*00006418*/ MOVT        R3, #0                      @ .LC222 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2038] 
    /*0000641c*/ MOVW        R2, #0                      @ .LC223 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x203c] 
    /*00006420*/ STR         R3, [SP, #20] 
    /*00006424*/ MOVT        R2, #0                      @ .LC223 [R_ARM_MOVT_ABS: .rodata.str1.4+0x203c] 
    /*00006428*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*0000642c*/ STR         R2, [SP, #16] 
    /*00006430*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*00006434*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*00006438*/ MOVT        R1, #0                      @ .LC180 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eb8] 
    /*0000643c*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*00006440*/ MOV         R0, FP 
    /*00006444*/ BL          0x0000000000006444          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006448*/ LDR         R3, [R4, #12] 
    /*0000644c*/ CMP         R3, #0 
    /*00006450*/ BEQ         AIProcShow_x5bbc
    /*00006454*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006458*/ CMP         R3, #0 
    /*0000645c*/ BEQ         AIProcShow_x5bbc
    /*00006460*/ MOVW        R7, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00006464*/ MOVT        R7, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00006468*/ LDR         R5, [SP, #96]               @ 0x60 
    /*0000646c*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00006470*/ MOVW        R8, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*00006474*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00006478*/ MOVT        R8, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*0000647c*/ STR         R3, [SP, #56]               @ 0x38 
    /*00006480*/ MOV         R3, R7 
    /*00006484*/ MOV         R6, #0 
    /*00006488*/ MOV         R7, SL 
    /*0000648c*/ MOV         SL, R3 
    AIProcShow_x6490:
    /*00006490*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*00006494*/ CMP         R3, #1 
    /*00006498*/ BEQ         AIProcShow_x7b80
    AIProcShow_x649c:
    /*0000649c*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*000064a0*/ ADD         R6, R6, #1 
    /*000064a4*/ ADD         R5, R5, #1120               @ 0x460 
    /*000064a8*/ CMP         R6, R3 
    /*000064ac*/ BCC         AIProcShow_x6490
    /*000064b0*/ MOV         SL, R7 
    /*000064b4*/ B           AIProcShow_x5bbc
    AIProcShow_x64b8:
    /*000064b8*/ ADD         R2, SL, #1 
    /*000064bc*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*000064c0*/ MOV         R0, FP 
    /*000064c4*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*000064c8*/ BL          0x00000000000064C8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000064cc*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*000064d0*/ MOVW        R5, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19ec] 
    /*000064d4*/ MOVW        R2, #0                      @ .LC165 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e10] 
    /*000064d8*/ STR         R9, [SP] 
    /*000064dc*/ MOVT        R2, #0                      @ .LC165 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e10] 
    /*000064e0*/ STR         R3, [SP, #4] 
    /*000064e4*/ MOVT        R5, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19ec] 
    /*000064e8*/ STR         R2, [SP, #16] 
    /*000064ec*/ MOVW        R3, #0                      @ .LC166 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e18] 
    /*000064f0*/ STR         R5, [SP, #8] 
    /*000064f4*/ MOVT        R3, #0                      @ .LC166 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e18] 
    /*000064f8*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*000064fc*/ STR         R3, [SP, #12] 
    /*00006500*/ MOVW        R1, #0                      @ .LC164 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1df8] 
    /*00006504*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*00006508*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*0000650c*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*00006510*/ MOVT        R1, #0                      @ .LC164 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1df8] 
    /*00006514*/ MOV         R0, FP 
    /*00006518*/ BL          0x0000000000006518          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000651c*/ LDR         R3, [R4, #12] 
    /*00006520*/ CMP         R3, #0 
    /*00006524*/ BEQ         AIProcShow_x657c
    /*00006528*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*0000652c*/ CMP         R3, #0 
    /*00006530*/ BEQ         AIProcShow_x657c
    /*00006534*/ LDR         R6, [SP, #96]               @ 0x60 
    /*00006538*/ MOVW        R8, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*0000653c*/ MOVT        R8, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00006540*/ MOV         R7, #0 
    /*00006544*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00006548*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*0000654c*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00006550*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*00006554*/ STR         R3, [SP, #56]               @ 0x38 
    /*00006558*/ STR         R2, [SP, #64]               @ 0x40 
    AIProcShow_x655c:
    /*0000655c*/ LDR         R3, [R6, #208]              @ 0xd0 
    /*00006560*/ CMP         R3, #1 
    /*00006564*/ BEQ         AIProcShow_x7c64
    AIProcShow_x6568:
    /*00006568*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*0000656c*/ ADD         R7, R7, #1 
    /*00006570*/ ADD         R6, R6, #1120               @ 0x460 
    /*00006574*/ CMP         R7, R3 
    /*00006578*/ BCC         AIProcShow_x655c
    AIProcShow_x657c:
    /*0000657c*/ ADD         R2, SL, #2 
    /*00006580*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00006584*/ MOV         R0, FP 
    /*00006588*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*0000658c*/ BL          0x000000000000658C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006590*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*00006594*/ STR         R5, [SP, #8] 
    /*00006598*/ MOVW        R2, #0                      @ .LC168 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e40] 
    /*0000659c*/ STR         R9, [SP] 
    /*000065a0*/ MOVT        R2, #0                      @ .LC168 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e40] 
    /*000065a4*/ STR         R3, [SP, #4] 
    /*000065a8*/ MOVW        R1, #0                      @ .LC167 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e20] 
    /*000065ac*/ STR         R2, [SP, #28] 
    /*000065b0*/ MOVW        R3, #0                      @ .LC169 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e48] 
    /*000065b4*/ MOVW        R2, #0                      @ .LC170 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e50] 
    /*000065b8*/ MOVT        R3, #0                      @ .LC169 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e48] 
    /*000065bc*/ MOVT        R2, #0                      @ .LC170 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e50] 
    /*000065c0*/ STR         R3, [SP, #24] 
    /*000065c4*/ STR         R2, [SP, #20] 
    /*000065c8*/ MOVW        R3, #0                      @ .LC171 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e58] 
    /*000065cc*/ MOVW        R2, #0                      @ .LC172 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e60] 
    /*000065d0*/ MOVT        R3, #0                      @ .LC171 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e58] 
    /*000065d4*/ MOVT        R2, #0                      @ .LC172 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e60] 
    /*000065d8*/ STR         R3, [SP, #16] 
    /*000065dc*/ STR         R2, [SP, #12] 
    /*000065e0*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*000065e4*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*000065e8*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*000065ec*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*000065f0*/ MOVT        R1, #0                      @ .LC167 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e20] 
    /*000065f4*/ MOV         R0, FP 
    /*000065f8*/ BL          0x00000000000065F8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000065fc*/ LDR         R3, [R4, #12] 
    /*00006600*/ CMP         R3, #0 
    /*00006604*/ BEQ         AIProcShow_x665c
    /*00006608*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*0000660c*/ CMP         R3, #0 
    /*00006610*/ BEQ         AIProcShow_x665c
    /*00006614*/ LDR         R6, [SP, #96]               @ 0x60 
    /*00006618*/ MOVW        R8, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*0000661c*/ MOVT        R8, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00006620*/ MOV         R7, #0 
    /*00006624*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00006628*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*0000662c*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00006630*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*00006634*/ STR         R3, [SP, #56]               @ 0x38 
    /*00006638*/ STR         R2, [SP, #64]               @ 0x40 
    AIProcShow_x663c:
    /*0000663c*/ LDR         R3, [R6, #208]              @ 0xd0 
    /*00006640*/ CMP         R3, #1 
    /*00006644*/ BEQ         AIProcShow_x7940
    AIProcShow_x6648:
    /*00006648*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*0000664c*/ ADD         R7, R7, #1 
    /*00006650*/ ADD         R6, R6, #1120               @ 0x460 
    /*00006654*/ CMP         R7, R3 
    /*00006658*/ BCC         AIProcShow_x663c
    AIProcShow_x665c:
    /*0000665c*/ ADD         SL, SL, #3 
    /*00006660*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00006664*/ MOV         R0, FP 
    /*00006668*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*0000666c*/ MOV         R2, SL 
    /*00006670*/ BL          0x0000000000006670          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006674*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*00006678*/ STR         R5, [SP, #8] 
    /*0000667c*/ MOVW        R2, #0                      @ .LC174 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e78] 
    /*00006680*/ STR         R9, [SP] 
    /*00006684*/ MOVT        R2, #0                      @ .LC174 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e78] 
    /*00006688*/ STR         R3, [SP, #4] 
    /*0000668c*/ MOVW        R1, #0                      @ .LC167 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e20] 
    /*00006690*/ STR         R2, [SP, #28] 
    /*00006694*/ MOVW        R3, #0                      @ .LC175 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e80] 
    /*00006698*/ MOVW        R2, #0                      @ .LC176 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e88] 
    /*0000669c*/ MOVT        R3, #0                      @ .LC175 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e80] 
    /*000066a0*/ MOVT        R2, #0                      @ .LC176 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e88] 
    /*000066a4*/ STR         R3, [SP, #24] 
    /*000066a8*/ STR         R2, [SP, #20] 
    /*000066ac*/ MOVW        R3, #0                      @ .LC177 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e90] 
    /*000066b0*/ MOVW        R2, #0                      @ .LC178 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e98] 
    /*000066b4*/ MOVT        R3, #0                      @ .LC177 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e90] 
    /*000066b8*/ MOVT        R2, #0                      @ .LC178 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e98] 
    /*000066bc*/ STR         R3, [SP, #16] 
    /*000066c0*/ STR         R2, [SP, #12] 
    /*000066c4*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*000066c8*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*000066cc*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*000066d0*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*000066d4*/ MOVT        R1, #0                      @ .LC167 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e20] 
    /*000066d8*/ MOV         R0, FP 
    /*000066dc*/ BL          0x00000000000066DC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000066e0*/ LDR         R3, [R4, #12] 
    /*000066e4*/ CMP         R3, #0 
    /*000066e8*/ BEQ         AIProcShow_x5bb0
    /*000066ec*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*000066f0*/ CMP         R3, #0 
    /*000066f4*/ BEQ         AIProcShow_x5bb0
    /*000066f8*/ LDR         R5, [SP, #96]               @ 0x60 
    /*000066fc*/ MOVW        R8, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*00006700*/ MOVW        R7, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00006704*/ MOVT        R8, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*00006708*/ MOVT        R7, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*0000670c*/ MOV         R6, #0 
    /*00006710*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00006714*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00006718*/ STR         R3, [SP, #56]               @ 0x38 
    AIProcShow_x671c:
    /*0000671c*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*00006720*/ CMP         R3, #1 
    /*00006724*/ BEQ         AIProcShow_x7754
    AIProcShow_x6728:
    /*00006728*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*0000672c*/ ADD         R6, R6, #1 
    /*00006730*/ ADD         R5, R5, #1120               @ 0x460 
    /*00006734*/ CMP         R6, R3 
    /*00006738*/ BCC         AIProcShow_x671c
    /*0000673c*/ B           AIProcShow_x5bb0
    /*00006740*/ MULEQ       R0, R0, R0 
    AIProcShow_x6744:
    /*00006744*/ ADD         R2, SL, #1 
    /*00006748*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*0000674c*/ MOV         R0, FP 
    /*00006750*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*00006754*/ BL          0x0000000000006754          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006758*/ MOVW        R5, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19ec] 
    /*0000675c*/ STR         R9, [SP] 
    /*00006760*/ MOVW        R2, #0                      @ .LC131 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1be0] 
    /*00006764*/ MOVW        R3, #0                      @ .LC132 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1be8] 
    /*00006768*/ MOVT        R2, #0                      @ .LC131 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1be0] 
    /*0000676c*/ MOVT        R3, #0                      @ .LC132 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1be8] 
    /*00006770*/ STR         R2, [SP, #32] 
    /*00006774*/ STR         R3, [SP, #28] 
    /*00006778*/ MOVW        R2, #0                      @ .LC133 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bf0] 
    /*0000677c*/ MOVW        R3, #0                      @ .LC134 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c00] 
    /*00006780*/ MOVT        R2, #0                      @ .LC133 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bf0] 
    /*00006784*/ MOVT        R3, #0                      @ .LC134 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c00] 
    /*00006788*/ STR         R2, [SP, #24] 
    /*0000678c*/ STR         R3, [SP, #20] 
    /*00006790*/ MOVW        R2, #0                      @ .LC135 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c0c] 
    /*00006794*/ MOVW        R3, #0                      @ .LC136 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c18] 
    /*00006798*/ MOVT        R2, #0                      @ .LC135 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c0c] 
    /*0000679c*/ MOVT        R3, #0                      @ .LC136 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c18] 
    /*000067a0*/ STR         R2, [SP, #16] 
    /*000067a4*/ STR         R3, [SP, #12] 
    /*000067a8*/ MOVT        R5, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19ec] 
    /*000067ac*/ MOVW        R6, #0                      @ .LC137 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c20] 
    /*000067b0*/ STR         R5, [SP, #8] 
    /*000067b4*/ MOVT        R6, #0                      @ .LC137 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c20] 
    /*000067b8*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*000067bc*/ STR         R6, [SP, #4] 
    /*000067c0*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*000067c4*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*000067c8*/ MOVW        R1, #0                      @ .LC130 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bb8] 
    /*000067cc*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*000067d0*/ MOVT        R1, #0                      @ .LC130 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bb8] 
    /*000067d4*/ MOV         R0, FP 
    /*000067d8*/ BL          0x00000000000067D8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000067dc*/ LDR         R3, [R4, #12] 
    /*000067e0*/ CMP         R3, #0 
    /*000067e4*/ BEQ         AIProcShow_x6840
    /*000067e8*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*000067ec*/ CMP         R3, #0 
    /*000067f0*/ BEQ         AIProcShow_x6840
    /*000067f4*/ LDR         R7, [SP, #96]               @ 0x60 
    /*000067f8*/ MOV         R8, #0 
    /*000067fc*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00006800*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*00006804*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00006808*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*0000680c*/ STR         R3, [SP, #64]               @ 0x40 
    /*00006810*/ MOVW        R3, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00006814*/ STR         R2, [SP, #68]               @ 0x44 
    /*00006818*/ MOVT        R3, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*0000681c*/ STR         R3, [SP, #56]               @ 0x38 
    AIProcShow_x6820:
    /*00006820*/ LDR         R3, [R7, #208]              @ 0xd0 
    /*00006824*/ CMP         R3, #1 
    /*00006828*/ BEQ         AIProcShow_x7690
    AIProcShow_x682c:
    /*0000682c*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006830*/ ADD         R8, R8, #1 
    /*00006834*/ ADD         R7, R7, #1120               @ 0x460 
    /*00006838*/ CMP         R8, R3 
    /*0000683c*/ BCC         AIProcShow_x6820
    AIProcShow_x6840:
    /*00006840*/ ADD         R2, SL, #2 
    /*00006844*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00006848*/ MOV         R0, FP 
    /*0000684c*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*00006850*/ BL          0x0000000000006850          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006854*/ STR         R5, [SP, #8] 
    /*00006858*/ STR         R6, [SP, #4] 
    /*0000685c*/ MOVW        R3, #0                      @ .LC139 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c54] 
    /*00006860*/ STR         R9, [SP] 
    /*00006864*/ MOVT        R3, #0                      @ .LC139 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c54] 
    /*00006868*/ MOVW        R2, #0                      @ .LC140 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c60] 
    /*0000686c*/ STR         R3, [SP, #32] 
    /*00006870*/ MOVT        R2, #0                      @ .LC140 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c60] 
    /*00006874*/ MOVW        R3, #0                      @ .LC141 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c6c] 
    /*00006878*/ STR         R2, [SP, #28] 
    /*0000687c*/ MOVT        R3, #0                      @ .LC141 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c6c] 
    /*00006880*/ MOVW        R2, #0                      @ .LC142 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c78] 
    /*00006884*/ STR         R3, [SP, #24] 
    /*00006888*/ MOVT        R2, #0                      @ .LC142 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c78] 
    /*0000688c*/ MOVW        R3, #0                      @ .LC143 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c84] 
    /*00006890*/ STR         R2, [SP, #20] 
    /*00006894*/ MOVT        R3, #0                      @ .LC143 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c84] 
    /*00006898*/ MOVW        R2, #0                      @ .LC144 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c94] 
    /*0000689c*/ STR         R3, [SP, #16] 
    /*000068a0*/ MOVT        R2, #0                      @ .LC144 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c94] 
    /*000068a4*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*000068a8*/ STR         R2, [SP, #12] 
    /*000068ac*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*000068b0*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*000068b4*/ MOVW        R1, #0                      @ .LC138 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c28] 
    /*000068b8*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*000068bc*/ MOVT        R1, #0                      @ .LC138 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c28] 
    /*000068c0*/ MOV         R0, FP 
    /*000068c4*/ BL          0x00000000000068C4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000068c8*/ LDR         R3, [R4, #12] 
    /*000068cc*/ CMP         R3, #0 
    /*000068d0*/ BEQ         AIProcShow_x692c
    /*000068d4*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*000068d8*/ CMP         R3, #0 
    /*000068dc*/ BEQ         AIProcShow_x692c
    /*000068e0*/ LDR         R7, [SP, #96]               @ 0x60 
    /*000068e4*/ MOV         R8, #0 
    /*000068e8*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*000068ec*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*000068f0*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*000068f4*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*000068f8*/ STR         R3, [SP, #64]               @ 0x40 
    /*000068fc*/ MOVW        R3, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00006900*/ STR         R2, [SP, #68]               @ 0x44 
    /*00006904*/ MOVT        R3, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00006908*/ STR         R3, [SP, #56]               @ 0x38 
    AIProcShow_x690c:
    /*0000690c*/ LDR         R3, [R7, #208]              @ 0xd0 
    /*00006910*/ CMP         R3, #1 
    /*00006914*/ BEQ         AIProcShow_x75e4
    AIProcShow_x6918:
    /*00006918*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*0000691c*/ ADD         R8, R8, #1 
    /*00006920*/ ADD         R7, R7, #1120               @ 0x460 
    /*00006924*/ CMP         R8, R3 
    /*00006928*/ BCC         AIProcShow_x690c
    AIProcShow_x692c:
    /*0000692c*/ ADD         R2, SL, #3 
    /*00006930*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00006934*/ MOV         R0, FP 
    /*00006938*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*0000693c*/ BL          0x000000000000693C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006940*/ STR         R5, [SP, #8] 
    /*00006944*/ STR         R6, [SP, #4] 
    /*00006948*/ MOVW        R3, #0                      @ .LC147 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1cf0] 
    /*0000694c*/ STR         R9, [SP] 
    /*00006950*/ MOVT        R3, #0                      @ .LC147 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1cf0] 
    /*00006954*/ MOVW        R2, #0                      @ .LC148 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1cfc] 
    /*00006958*/ STR         R3, [SP, #32] 
    /*0000695c*/ MOVT        R2, #0                      @ .LC148 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1cfc] 
    /*00006960*/ MOVW        R3, #0                      @ .LC149 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d08] 
    /*00006964*/ STR         R2, [SP, #28] 
    /*00006968*/ MOVT        R3, #0                      @ .LC149 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d08] 
    /*0000696c*/ MOVW        R2, #0                      @ .LC150 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d14] 
    /*00006970*/ STR         R3, [SP, #24] 
    /*00006974*/ MOVT        R2, #0                      @ .LC150 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d14] 
    /*00006978*/ MOVW        R3, #0                      @ .LC151 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d20] 
    /*0000697c*/ STR         R2, [SP, #20] 
    /*00006980*/ MOVT        R3, #0                      @ .LC151 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d20] 
    /*00006984*/ MOVW        R2, #0                      @ .LC152 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d2c] 
    /*00006988*/ STR         R3, [SP, #16] 
    /*0000698c*/ MOVT        R2, #0                      @ .LC152 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d2c] 
    /*00006990*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*00006994*/ STR         R2, [SP, #12] 
    /*00006998*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*0000699c*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*000069a0*/ MOVW        R1, #0                      @ .LC146 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1cc4] 
    /*000069a4*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*000069a8*/ MOVT        R1, #0                      @ .LC146 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1cc4] 
    /*000069ac*/ MOV         R0, FP 
    /*000069b0*/ BL          0x00000000000069B0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000069b4*/ LDR         R3, [R4, #12] 
    /*000069b8*/ CMP         R3, #0 
    /*000069bc*/ BEQ         AIProcShow_x6a18
    /*000069c0*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*000069c4*/ CMP         R3, #0 
    /*000069c8*/ BEQ         AIProcShow_x6a18
    /*000069cc*/ LDR         R7, [SP, #96]               @ 0x60 
    /*000069d0*/ MOV         R8, #0 
    /*000069d4*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*000069d8*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*000069dc*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*000069e0*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*000069e4*/ STR         R3, [SP, #64]               @ 0x40 
    /*000069e8*/ MOVW        R3, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*000069ec*/ STR         R2, [SP, #68]               @ 0x44 
    /*000069f0*/ MOVT        R3, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*000069f4*/ STR         R3, [SP, #56]               @ 0x38 
    AIProcShow_x69f8:
    /*000069f8*/ LDR         R3, [R7, #208]              @ 0xd0 
    /*000069fc*/ CMP         R3, #1 
    /*00006a00*/ BEQ         AIProcShow_x7538
    AIProcShow_x6a04:
    /*00006a04*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006a08*/ ADD         R8, R8, #1 
    /*00006a0c*/ ADD         R7, R7, #1120               @ 0x460 
    /*00006a10*/ CMP         R8, R3 
    /*00006a14*/ BCC         AIProcShow_x69f8
    AIProcShow_x6a18:
    /*00006a18*/ ADD         SL, SL, #4 
    /*00006a1c*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00006a20*/ MOV         R0, FP 
    /*00006a24*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*00006a28*/ MOV         R2, SL 
    /*00006a2c*/ BL          0x0000000000006A2C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006a30*/ STR         R5, [SP, #8] 
    /*00006a34*/ STR         R6, [SP, #4] 
    /*00006a38*/ MOVW        R2, #0                      @ .LC155 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d80] 
    /*00006a3c*/ STR         R9, [SP] 
    /*00006a40*/ MOVT        R2, #0                      @ .LC155 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d80] 
    /*00006a44*/ MOVW        R3, #0                      @ .LC156 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d88] 
    /*00006a48*/ STR         R2, [SP, #36]               @ 0x24 
    /*00006a4c*/ MOVT        R3, #0                      @ .LC156 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d88] 
    /*00006a50*/ MOVW        R2, #0                      @ .LC157 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d90] 
    /*00006a54*/ STR         R3, [SP, #32] 
    /*00006a58*/ MOVT        R2, #0                      @ .LC157 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d90] 
    /*00006a5c*/ MOVW        R3, #0                      @ .LC158 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d98] 
    /*00006a60*/ STR         R2, [SP, #28] 
    /*00006a64*/ MOVT        R3, #0                      @ .LC158 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d98] 
    /*00006a68*/ MOVW        R2, #0                      @ .LC159 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1da0] 
    /*00006a6c*/ STR         R3, [SP, #24] 
    /*00006a70*/ MOVT        R2, #0                      @ .LC159 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1da0] 
    /*00006a74*/ MOVW        R3, #0                      @ .LC160 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1da8] 
    /*00006a78*/ STR         R2, [SP, #20] 
    /*00006a7c*/ MOVT        R3, #0                      @ .LC160 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1da8] 
    /*00006a80*/ MOVW        R2, #0                      @ .LC161 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1db0] 
    /*00006a84*/ STR         R3, [SP, #16] 
    /*00006a88*/ MOVT        R2, #0                      @ .LC161 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1db0] 
    /*00006a8c*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*00006a90*/ STR         R2, [SP, #12] 
    /*00006a94*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*00006a98*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*00006a9c*/ MOVW        R1, #0                      @ .LC154 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d58] 
    /*00006aa0*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*00006aa4*/ MOVT        R1, #0                      @ .LC154 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d58] 
    /*00006aa8*/ MOV         R0, FP 
    /*00006aac*/ BL          0x0000000000006AAC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006ab0*/ LDR         R3, [R4, #12] 
    /*00006ab4*/ CMP         R3, #0 
    /*00006ab8*/ BEQ         AIProcShow_x5ba4
    /*00006abc*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006ac0*/ CMP         R3, #0 
    /*00006ac4*/ BEQ         AIProcShow_x5ba4
    /*00006ac8*/ MOVW        R7, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00006acc*/ MOVT        R7, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00006ad0*/ MOVW        R8, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*00006ad4*/ LDR         R5, [SP, #96]               @ 0x60 
    /*00006ad8*/ MOVT        R8, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*00006adc*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00006ae0*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00006ae4*/ STR         R3, [SP, #56]               @ 0x38 
    /*00006ae8*/ MOV         R3, R7 
    /*00006aec*/ MOV         R6, #0 
    /*00006af0*/ MOV         R7, R8 
    /*00006af4*/ MOV         R8, R3 
    AIProcShow_x6af8:
    /*00006af8*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*00006afc*/ CMP         R3, #1 
    /*00006b00*/ BEQ         AIProcShow_x7480
    AIProcShow_x6b04:
    /*00006b04*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006b08*/ ADD         R6, R6, #1 
    /*00006b0c*/ ADD         R5, R5, #1120               @ 0x460 
    /*00006b10*/ CMP         R6, R3 
    /*00006b14*/ BCC         AIProcShow_x6af8
    /*00006b18*/ B           AIProcShow_x5ba4
    AIProcShow_x6b1c:
    /*00006b1c*/ ADD         SL, SL, #1 
    /*00006b20*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00006b24*/ MOV         R0, FP 
    /*00006b28*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*00006b2c*/ MOV         R2, SL 
    /*00006b30*/ BL          0x0000000000006B30          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006b34*/ STR         R8, [SP, #4] 
    /*00006b38*/ STR         R9, [SP] 
    /*00006b3c*/ MOVW        R2, #0                      @ .LC126 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b84] 
    /*00006b40*/ MOVW        R3, #0                      @ .LC127 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b8c] 
    /*00006b44*/ MOVT        R2, #0                      @ .LC126 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b84] 
    /*00006b48*/ MOVT        R3, #0                      @ .LC127 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b8c] 
    /*00006b4c*/ STR         R2, [SP, #20] 
    /*00006b50*/ STR         R3, [SP, #16] 
    /*00006b54*/ MOVW        R2, #0                      @ .LC128 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b98] 
    /*00006b58*/ MOVW        R3, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19ec] 
    /*00006b5c*/ MOVT        R2, #0                      @ .LC128 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b98] 
    /*00006b60*/ MOVT        R3, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19ec] 
    /*00006b64*/ STR         R2, [SP, #12] 
    /*00006b68*/ STR         R3, [SP, #8] 
    /*00006b6c*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*00006b70*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*00006b74*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*00006b78*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*00006b7c*/ MOVW        R1, #0                      @ .LC125 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b68] 
    /*00006b80*/ MOV         R0, FP 
    /*00006b84*/ MOVT        R1, #0                      @ .LC125 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b68] 
    /*00006b88*/ BL          0x0000000000006B88          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006b8c*/ LDR         R3, [R4, #12] 
    /*00006b90*/ CMP         R3, #0 
    /*00006b94*/ BEQ         AIProcShow_x5b98
    /*00006b98*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006b9c*/ CMP         R3, #0 
    /*00006ba0*/ BEQ         AIProcShow_x5b98
    /*00006ba4*/ LDR         R5, [SP, #96]               @ 0x60 
    /*00006ba8*/ MOVW        R8, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00006bac*/ MOVW        R7, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00006bb0*/ MOVT        R8, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00006bb4*/ MOVT        R7, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00006bb8*/ MOV         R6, #0 
    /*00006bbc*/ MOVW        R3, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*00006bc0*/ MOVT        R3, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*00006bc4*/ STR         R3, [SP, #56]               @ 0x38 
    AIProcShow_x6bc8:
    /*00006bc8*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*00006bcc*/ CMP         R3, #1 
    /*00006bd0*/ BEQ         AIProcShow_x740c
    AIProcShow_x6bd4:
    /*00006bd4*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006bd8*/ ADD         R6, R6, #1 
    /*00006bdc*/ ADD         R5, R5, #1120               @ 0x460 
    /*00006be0*/ CMP         R6, R3 
    /*00006be4*/ BCC         AIProcShow_x6bc8
    /*00006be8*/ B           AIProcShow_x5b98
    AIProcShow_x6bec:
    /*00006bec*/ ADD         SL, SL, #1 
    /*00006bf0*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00006bf4*/ MOV         R0, FP 
    /*00006bf8*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*00006bfc*/ MOV         R2, SL 
    /*00006c00*/ BL          0x0000000000006C00          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006c04*/ STR         R7, [SP, #4] 
    /*00006c08*/ STR         R9, [SP] 
    /*00006c0c*/ MOVW        R2, #0                      @ .LC123 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b54] 
    /*00006c10*/ MOVW        R3, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19ec] 
    /*00006c14*/ MOVT        R2, #0                      @ .LC123 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b54] 
    /*00006c18*/ MOVT        R3, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19ec] 
    /*00006c1c*/ STR         R2, [SP, #12] 
    /*00006c20*/ STR         R3, [SP, #8] 
    /*00006c24*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*00006c28*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*00006c2c*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*00006c30*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*00006c34*/ MOVW        R1, #0                      @ .LC122 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b40] 
    /*00006c38*/ MOV         R0, FP 
    /*00006c3c*/ MOVT        R1, #0                      @ .LC122 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b40] 
    /*00006c40*/ BL          0x0000000000006C40          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006c44*/ LDR         R3, [R4, #12] 
    /*00006c48*/ CMP         R3, #0 
    /*00006c4c*/ BEQ         AIProcShow_x5b8c
    /*00006c50*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006c54*/ CMP         R3, #0 
    /*00006c58*/ BEQ         AIProcShow_x5b8c
    /*00006c5c*/ LDR         R5, [SP, #96]               @ 0x60 
    /*00006c60*/ MOVW        R7, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00006c64*/ MOVT        R7, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00006c68*/ MOV         R6, #0 
    /*00006c6c*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00006c70*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*00006c74*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00006c78*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*00006c7c*/ STR         R3, [SP, #56]               @ 0x38 
    /*00006c80*/ STR         R2, [SP, #64]               @ 0x40 
    AIProcShow_x6c84:
    /*00006c84*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*00006c88*/ CMP         R3, #1 
    /*00006c8c*/ BEQ         AIProcShow_x73a0
    AIProcShow_x6c90:
    /*00006c90*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006c94*/ ADD         R6, R6, #1 
    /*00006c98*/ ADD         R5, R5, #1120               @ 0x460 
    /*00006c9c*/ CMP         R6, R3 
    /*00006ca0*/ BCC         AIProcShow_x6c84
    /*00006ca4*/ B           AIProcShow_x5b8c
    AIProcShow_x6ca8:
    /*00006ca8*/ ADD         SL, SL, #1 
    /*00006cac*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00006cb0*/ MOV         R0, FP 
    /*00006cb4*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*00006cb8*/ MOV         R2, SL 
    /*00006cbc*/ BL          0x0000000000006CBC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006cc0*/ LDR         R3, [SP, #108]              @ 0x6c 
    /*00006cc4*/ STR         R9, [SP] 
    /*00006cc8*/ MOVW        R2, #0                      @ .LC111 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1aec] 
    /*00006ccc*/ MOVW        R1, #0                      @ .LC110 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ac0] 
    /*00006cd0*/ MOVT        R2, #0                      @ .LC111 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1aec] 
    /*00006cd4*/ STR         R3, [SP, #4] 
    /*00006cd8*/ MOV         R0, FP 
    /*00006cdc*/ STR         R2, [SP, #44]               @ 0x2c 
    /*00006ce0*/ MOVW        R3, #0                      @ .LC112 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1af0] 
    /*00006ce4*/ MOVW        R2, #0                      @ .LC113 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1af4] 
    /*00006ce8*/ MOVT        R3, #0                      @ .LC112 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1af0] 
    /*00006cec*/ MOVT        R2, #0                      @ .LC113 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1af4] 
    /*00006cf0*/ STR         R3, [SP, #40]               @ 0x28 
    /*00006cf4*/ STR         R2, [SP, #36]               @ 0x24 
    /*00006cf8*/ MOVW        R3, #0                      @ .LC114 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1afc] 
    /*00006cfc*/ MOVW        R2, #0                      @ .LC115 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b04] 
    /*00006d00*/ MOVT        R3, #0                      @ .LC114 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1afc] 
    /*00006d04*/ MOVT        R2, #0                      @ .LC115 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b04] 
    /*00006d08*/ STR         R3, [SP, #32] 
    /*00006d0c*/ STR         R2, [SP, #28] 
    /*00006d10*/ MOVW        R3, #0                      @ .LC116 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b08] 
    /*00006d14*/ MOVW        R2, #0                      @ .LC117 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b0c] 
    /*00006d18*/ MOVT        R3, #0                      @ .LC116 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b08] 
    /*00006d1c*/ MOVT        R2, #0                      @ .LC117 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b0c] 
    /*00006d20*/ STR         R3, [SP, #24] 
    /*00006d24*/ STR         R2, [SP, #20] 
    /*00006d28*/ MOVW        R3, #0                      @ .LC118 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b10] 
    /*00006d2c*/ MOVW        R2, #0                      @ .LC119 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b14] 
    /*00006d30*/ MOVT        R3, #0                      @ .LC118 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b10] 
    /*00006d34*/ MOVT        R2, #0                      @ .LC119 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b14] 
    /*00006d38*/ STR         R3, [SP, #16] 
    /*00006d3c*/ STR         R2, [SP, #12] 
    /*00006d40*/ MOVW        R3, #0                      @ .LC120 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b18] 
    /*00006d44*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*00006d48*/ MOVT        R3, #0                      @ .LC120 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b18] 
    /*00006d4c*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*00006d50*/ STR         R3, [SP, #8] 
    /*00006d54*/ MOVT        R1, #0                      @ .LC110 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ac0] 
    /*00006d58*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*00006d5c*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*00006d60*/ BL          0x0000000000006D60          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006d64*/ LDR         R3, [R4, #12] 
    /*00006d68*/ CMP         R3, #0 
    /*00006d6c*/ BEQ         AIProcShow_x5b80
    /*00006d70*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006d74*/ CMP         R3, #0 
    /*00006d78*/ BEQ         AIProcShow_x5b80
    /*00006d7c*/ LDR         R5, [SP, #96]               @ 0x60 
    /*00006d80*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00006d84*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00006d88*/ STR         R3, [SP, #64]               @ 0x40 
    /*00006d8c*/ MOVW        R3, #0                      @ .LC121 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b1c] 
    /*00006d90*/ MOVT        R3, #0                      @ .LC121 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b1c] 
    /*00006d94*/ STR         R3, [SP, #104]              @ 0x68 
    /*00006d98*/ MOV         R3, R8 
    /*00006d9c*/ MOV         R6, #0 
    /*00006da0*/ MOV         R8, SL 
    /*00006da4*/ MOV         SL, R3 
    /*00006da8*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*00006dac*/ STR         R7, [SP, #56]               @ 0x38 
    /*00006db0*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*00006db4*/ STR         R2, [SP, #72]               @ 0x48 
    AIProcShow_x6db8:
    /*00006db8*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*00006dbc*/ CMP         R3, #1 
    /*00006dc0*/ BEQ         AIProcShow_x72e4
    AIProcShow_x6dc4:
    /*00006dc4*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006dc8*/ ADD         R6, R6, #1 
    /*00006dcc*/ ADD         R5, R5, #1120               @ 0x460 
    /*00006dd0*/ CMP         R6, R3 
    /*00006dd4*/ BCC         AIProcShow_x6db8
    /*00006dd8*/ MOV         R3, SL 
    /*00006ddc*/ LDR         R7, [SP, #56]               @ 0x38 
    /*00006de0*/ MOV         SL, R8 
    /*00006de4*/ MOV         R8, R3 
    /*00006de8*/ B           AIProcShow_x5b80
    AIProcShow_x6dec:
    /*00006dec*/ ADD         SL, SL, #1 
    /*00006df0*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00006df4*/ MOV         R0, FP 
    /*00006df8*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*00006dfc*/ MOV         R2, SL 
    /*00006e00*/ BL          0x0000000000006E00          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006e04*/ LDR         R3, [SP, #112]              @ 0x70 
    /*00006e08*/ STR         R9, [SP] 
    /*00006e0c*/ MOVW        R2, #0                      @ .LC101 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a40] 
    /*00006e10*/ MOVW        R1, #0                      @ .LC100 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a10] 
    /*00006e14*/ MOVT        R2, #0                      @ .LC101 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a40] 
    /*00006e18*/ STR         R3, [SP, #4] 
    /*00006e1c*/ MOVT        R1, #0                      @ .LC100 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a10] 
    /*00006e20*/ STR         R2, [SP, #40]               @ 0x28 
    /*00006e24*/ MOVW        R3, #0                      @ .LC102 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a48] 
    /*00006e28*/ MOVW        R2, #0                      @ .LC103 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a54] 
    /*00006e2c*/ MOVT        R3, #0                      @ .LC102 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a48] 
    /*00006e30*/ MOVT        R2, #0                      @ .LC103 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a54] 
    /*00006e34*/ STR         R3, [SP, #36]               @ 0x24 
    /*00006e38*/ STR         R2, [SP, #32] 
    /*00006e3c*/ MOVW        R3, #0                      @ .LC104 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a60] 
    /*00006e40*/ MOVW        R2, #0                      @ .LC105 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a6c] 
    /*00006e44*/ MOVT        R3, #0                      @ .LC104 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a60] 
    /*00006e48*/ MOVT        R2, #0                      @ .LC105 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a6c] 
    /*00006e4c*/ STR         R3, [SP, #28] 
    /*00006e50*/ STR         R2, [SP, #24] 
    /*00006e54*/ MOVW        R3, #0                      @ .LC106 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a74] 
    /*00006e58*/ MOVW        R2, #0                      @ .LC107 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a80] 
    /*00006e5c*/ MOVT        R3, #0                      @ .LC106 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a74] 
    /*00006e60*/ MOVT        R2, #0                      @ .LC107 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a80] 
    /*00006e64*/ STR         R3, [SP, #20] 
    /*00006e68*/ STR         R2, [SP, #16] 
    /*00006e6c*/ MOVW        R3, #0                      @ .LC108 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a8c] 
    /*00006e70*/ MOVW        R2, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19ec] 
    /*00006e74*/ MOVT        R3, #0                      @ .LC108 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a8c] 
    /*00006e78*/ MOVT        R2, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19ec] 
    /*00006e7c*/ STR         R3, [SP, #12] 
    /*00006e80*/ STR         R2, [SP, #8] 
    /*00006e84*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*00006e88*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*00006e8c*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*00006e90*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*00006e94*/ MOV         R0, FP 
    /*00006e98*/ BL          0x0000000000006E98          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006e9c*/ LDR         R3, [R4, #12] 
    /*00006ea0*/ CMP         R3, #0 
    /*00006ea4*/ BEQ         AIProcShow_x5b74
    /*00006ea8*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006eac*/ CMP         R3, #0 
    /*00006eb0*/ BEQ         AIProcShow_x5b74
    /*00006eb4*/ LDR         R5, [SP, #96]               @ 0x60 
    /*00006eb8*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00006ebc*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00006ec0*/ STR         R3, [SP, #64]               @ 0x40 
    /*00006ec4*/ MOVW        R3, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00006ec8*/ MOVT        R3, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00006ecc*/ STR         R3, [SP, #56]               @ 0x38 
    /*00006ed0*/ MOV         R3, R7 
    /*00006ed4*/ MOV         R6, #0 
    /*00006ed8*/ MOV         R7, SL 
    /*00006edc*/ MOV         SL, R3 
    /*00006ee0*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*00006ee4*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*00006ee8*/ STR         R2, [SP, #104]              @ 0x68 
    AIProcShow_x6eec:
    /*00006eec*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*00006ef0*/ CMP         R3, #1 
    /*00006ef4*/ BEQ         AIProcShow_x787c
    AIProcShow_x6ef8:
    /*00006ef8*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006efc*/ ADD         R6, R6, #1 
    /*00006f00*/ ADD         R5, R5, #1120               @ 0x460 
    /*00006f04*/ CMP         R6, R3 
    /*00006f08*/ BCC         AIProcShow_x6eec
    /*00006f0c*/ MOV         R3, SL 
    /*00006f10*/ MOV         SL, R7 
    /*00006f14*/ MOV         R7, R3 
    /*00006f18*/ B           AIProcShow_x5b74
    AIProcShow_x6f1c:
    /*00006f1c*/ MOV         R2, #1 
    /*00006f20*/ MOVW        R1, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1958] 
    /*00006f24*/ MOV         R0, FP 
    /*00006f28*/ MOVT        R1, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1958] 
    /*00006f2c*/ BL          0x0000000000006F2C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006f30*/ STR         R9, [SP] 
    /*00006f34*/ MOVW        R2, #0                      @ .LC94 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19c8] 
    /*00006f38*/ MOVW        R3, #0                      @ .LC95 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19d4] 
    /*00006f3c*/ MOVT        R2, #0                      @ .LC94 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19c8] 
    /*00006f40*/ MOVT        R3, #0                      @ .LC95 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19d4] 
    /*00006f44*/ STR         R2, [SP, #20] 
    /*00006f48*/ MOVW        R1, #0                      @ .LC93 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19ac] 
    /*00006f4c*/ STR         R3, [SP, #16] 
    /*00006f50*/ MOVW        R2, #0                      @ .LC96 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19e0] 
    /*00006f54*/ MOVW        R3, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19ec] 
    /*00006f58*/ MOVT        R2, #0                      @ .LC96 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19e0] 
    /*00006f5c*/ MOVT        R3, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19ec] 
    /*00006f60*/ STR         R2, [SP, #12] 
    /*00006f64*/ STR         R3, [SP, #8] 
    /*00006f68*/ MOVW        R2, #0                      @ .LC98 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19f4] 
    /*00006f6c*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e4] 
    /*00006f70*/ MOVT        R2, #0                      @ .LC98 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19f4] 
    /*00006f74*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e4] 
    /*00006f78*/ STR         R2, [SP, #4] 
    /*00006f7c*/ MOVT        R1, #0                      @ .LC93 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19ac] 
    /*00006f80*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16ec] 
    /*00006f84*/ MOV         R0, FP 
    /*00006f88*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16ec] 
    /*00006f8c*/ BL          0x0000000000006F8C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006f90*/ LDR         R3, [R4, #12] 
    /*00006f94*/ CMP         R3, #0 
    /*00006f98*/ BEQ         AIProcShow_x6ff0
    /*00006f9c*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006fa0*/ CMP         R3, #0 
    /*00006fa4*/ BEQ         AIProcShow_x6ff0
    /*00006fa8*/ LDR         R5, [SP, #96]               @ 0x60 
    /*00006fac*/ MOVW        SL, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00006fb0*/ MOVT        SL, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00006fb4*/ MOV         R6, #0 
    /*00006fb8*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00006fbc*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1918] 
    /*00006fc0*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00006fc4*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1918] 
    /*00006fc8*/ STR         R3, [SP, #56]               @ 0x38 
    /*00006fcc*/ STR         R2, [SP, #104]              @ 0x68 
    AIProcShow_x6fd0:
    /*00006fd0*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*00006fd4*/ CMP         R3, #1 
    /*00006fd8*/ BEQ         AIProcShow_x77f4
    AIProcShow_x6fdc:
    /*00006fdc*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00006fe0*/ ADD         R6, R6, #1 
    /*00006fe4*/ ADD         R5, R5, #1120               @ 0x460 
    /*00006fe8*/ CMP         R6, R3 
    /*00006fec*/ BCC         AIProcShow_x6fd0
    AIProcShow_x6ff0:
    /*00006ff0*/ MOV         SL, #1 
    /*00006ff4*/ B           AIProcShow_x5b68
    AIProcShow_x6ff8:
    /*00006ff8*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00006ffc*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00007000*/ B           AIProcShow_x5e88
    AIProcShow_x7004:
    /*00007004*/ MOVW        R9, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00007008*/ MOVT        R9, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*0000700c*/ B           AIProcShow_x5db8
    AIProcShow_x7010:
    /*00007010*/ MOV         R3, R5 
    /*00007014*/ MOV         R2, #0 
    /*00007018*/ STR         R8, [SP] 
    /*0000701c*/ MOV         R1, R7 
    /*00007020*/ MOV         R0, FP 
    /*00007024*/ BL          0x0000000000007024          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007028*/ LDR         R3, [R9, #268]              @ 0x10c 
    /*0000702c*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007030*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007034*/ CMP         R3, #1 
    /*00007038*/ MOVEQ       R2, R6 
    /*0000703c*/ BEQ         AIProcShow_x7048
    /*00007040*/ CMP         R3, #0 
    /*00007044*/ MOVNE       R2, #0 
    AIProcShow_x7048:
    /*00007048*/ LDR         IP, [R9, #460]              @ 0x1cc 
    /*0000704c*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007050*/ LDR         R0, [R9, #440]              @ 0x1b8 
    /*00007054*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007058*/ LDR         R1, [R9, #452]              @ 0x1c4 
    /*0000705c*/ LDR         LR, [R9, #456]              @ 0x1c8 
    /*00007060*/ CMP         R0, #1 
    /*00007064*/ LDR         R0, [R9, #444]              @ 0x1bc 
    /*00007068*/ STR         IP, [SP, #16] 
    /*0000706c*/ MOVEQ       R3, R6 
    /*00007070*/ LDR         IP, [R9, #448]              @ 0x1c0 
    /*00007074*/ STR         R1, [SP, #12] 
    /*00007078*/ MOVW        R1, #0                      @ .LC229 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x208c] 
    /*0000707c*/ STR         R0, [SP, #4] 
    /*00007080*/ MOVT        R1, #0                      @ .LC229 [R_ARM_MOVT_ABS: .rodata.str1.4+0x208c] 
    /*00007084*/ STR         LR, [SP, #8] 
    /*00007088*/ MOV         R0, FP 
    /*0000708c*/ STR         IP, [SP] 
    /*00007090*/ BL          0x0000000000007090          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007094*/ B           AIProcShow_x5c94
    AIProcShow_x7098:
    /*00007098*/ MOVW        R0, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15b0] 
    /*0000709c*/ MOVT        R0, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15b0] 
    AIProcShow_x70a0:
    /*000070a0*/ LDR         R1, [R6, #576]              @ 0x240 
    /*000070a4*/ CMP         R1, #8000                   @ 0x1f40 
    /*000070a8*/ BEQ         AIProcShow_x728c
    /*000070ac*/ MOVW        IP, #11025                  @ 0x2b11 
    /*000070b0*/ CMP         R1, IP 
    /*000070b4*/ BEQ         AIProcShow_x7dc4
    /*000070b8*/ MOVW        IP, #12000                  @ 0x2ee0 
    /*000070bc*/ CMP         R1, IP 
    /*000070c0*/ BEQ         AIProcShow_x7e08
    /*000070c4*/ CMP         R1, #16000                  @ 0x3e80 
    /*000070c8*/ BEQ         AIProcShow_x7e2c
    /*000070cc*/ MOVW        IP, #22050                  @ 0x5622 
    /*000070d0*/ CMP         R1, IP 
    /*000070d4*/ BEQ         AIProcShow_x7e38
    /*000070d8*/ MOVW        IP, #24000                  @ 0x5dc0 
    /*000070dc*/ CMP         R1, IP 
    /*000070e0*/ BEQ         AIProcShow_x7e5c
    /*000070e4*/ CMP         R1, #32000                  @ 0x7d00 
    /*000070e8*/ BEQ         AIProcShow_x7e8c
    /*000070ec*/ MOVW        IP, #44100                  @ 0xac44 
    /*000070f0*/ CMP         R1, IP 
    /*000070f4*/ BEQ         AIProcShow_x72c0
    /*000070f8*/ MOVW        IP, #48000                  @ 0xbb80 
    /*000070fc*/ CMP         R1, IP 
    /*00007100*/ BEQ         AIProcShow_x7ee8
    /*00007104*/ CMP         R1, #64000                  @ 0xfa00 
    /*00007108*/ BEQ         AIProcShow_x7f18
    /*0000710c*/ MOV         LR, #30464                  @ 0x7700 
    /*00007110*/ MOVT        LR, #1 
    /*00007114*/ CMP         R1, LR 
    /*00007118*/ MOVW        IP, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15f8] 
    /*0000711c*/ MOVT        IP, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15f8] 
    /*00007120*/ MOVNE       R1, #0 
    /*00007124*/ MOVEQ       R1, IP 
    /*00007128*/ B           AIProcShow_x7294
    AIProcShow_x712c:
    /*0000712c*/ MOVW        R8, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15b0] 
    /*00007130*/ MOVT        R8, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15b0] 
    AIProcShow_x7134:
    /*00007134*/ LDR         R2, [R5, #260]              @ 0x104 
    /*00007138*/ LDR         R1, [R5, #292]              @ 0x124 
    /*0000713c*/ CMP         R2, #1 
    /*00007140*/ STR         R1, [SP, #140]              @ 0x8c 
    /*00007144*/ BEQ         AIProcShow_x7238
    /*00007148*/ CMP         R2, #0 
    /*0000714c*/ MOVW        IP, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007150*/ MOVT        IP, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007154*/ MOVNE       IP, #0 
    AIProcShow_x7158:
    /*00007158*/ LDR         R2, [R5, #264]              @ 0x108 
    /*0000715c*/ CMP         R2, #1 
    /*00007160*/ BEQ         AIProcShow_x7280
    /*00007164*/ CMP         R2, #0 
    /*00007168*/ MOVW        LR, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*0000716c*/ MOVT        LR, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007170*/ MOVNE       LR, #0 
    AIProcShow_x7174:
    /*00007174*/ LDR         R2, [R5, #244]              @ 0xf4 
    /*00007178*/ CMP         R2, #1 
    /*0000717c*/ BEQ         AIProcShow_x7274
    /*00007180*/ CMP         R2, #0 
    /*00007184*/ MOVW        R7, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007188*/ MOVT        R7, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*0000718c*/ MOVNE       R7, #0 
    AIProcShow_x7190:
    /*00007190*/ LDR         R2, [R5, #256]              @ 0x100 
    /*00007194*/ CMP         R2, #1 
    /*00007198*/ BEQ         AIProcShow_x7268
    /*0000719c*/ CMP         R2, #0 
    /*000071a0*/ MOVW        R0, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*000071a4*/ MOVT        R0, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*000071a8*/ MOVNE       R0, #0 
    AIProcShow_x71ac:
    /*000071ac*/ LDR         R2, [R5, #268]              @ 0x10c 
    /*000071b0*/ CMP         R2, #1 
    /*000071b4*/ BEQ         AIProcShow_x725c
    /*000071b8*/ CMP         R2, #0 
    /*000071bc*/ MOVW        R1, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*000071c0*/ MOVT        R1, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*000071c4*/ MOVNE       R1, #0 
    AIProcShow_x71c8:
    /*000071c8*/ LDR         R2, [R5, #272]              @ 0x110 
    /*000071cc*/ CMP         R2, #1 
    /*000071d0*/ BEQ         AIProcShow_x7250
    /*000071d4*/ CMP         R2, #0 
    /*000071d8*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*000071dc*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*000071e0*/ MOVNE       R2, #0 
    AIProcShow_x71e4:
    /*000071e4*/ LDR         SL, [R5, #580]              @ 0x244 
    /*000071e8*/ CMP         SL, #1 
    /*000071ec*/ BEQ         AIProcShow_x7244
    /*000071f0*/ CMP         SL, #0 
    /*000071f4*/ MOVW        SL, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*000071f8*/ MOVT        SL, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*000071fc*/ MOVNE       SL, #0 
    AIProcShow_x7200:
    /*00007200*/ STR         IP, [SP, #8] 
    /*00007204*/ LDR         IP, [SP, #140]              @ 0x8c 
    /*00007208*/ STR         R2, [SP, #28] 
    /*0000720c*/ MOV         R2, R9 
    /*00007210*/ STR         R1, [SP, #24] 
    /*00007214*/ STR         R0, [SP, #20] 
    /*00007218*/ MOV         R0, FP 
    /*0000721c*/ STR         SL, [SP, #32] 
    /*00007220*/ LDR         R1, [SP, #136]              @ 0x88 
    /*00007224*/ STR         R7, [SP, #16] 
    /*00007228*/ STR         LR, [SP, #12] 
    /*0000722c*/ STM         SP, {R8, IP} 
    /*00007230*/ BL          0x0000000000007230          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007234*/ B           AIProcShow_x5b2c
    AIProcShow_x7238:
    /*00007238*/ MOVW        IP, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*0000723c*/ MOVT        IP, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00007240*/ B           AIProcShow_x7158
    AIProcShow_x7244:
    /*00007244*/ MOVW        SL, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00007248*/ MOVT        SL, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*0000724c*/ B           AIProcShow_x7200
    AIProcShow_x7250:
    /*00007250*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00007254*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00007258*/ B           AIProcShow_x71e4
    AIProcShow_x725c:
    /*0000725c*/ MOVW        R1, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00007260*/ MOVT        R1, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00007264*/ B           AIProcShow_x71c8
    AIProcShow_x7268:
    /*00007268*/ MOVW        R0, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*0000726c*/ MOVT        R0, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00007270*/ B           AIProcShow_x71ac
    AIProcShow_x7274:
    /*00007274*/ MOVW        R7, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00007278*/ MOVT        R7, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*0000727c*/ B           AIProcShow_x7190
    AIProcShow_x7280:
    /*00007280*/ MOVW        LR, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00007284*/ MOVT        LR, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00007288*/ B           AIProcShow_x7174
    AIProcShow_x728c:
    /*0000728c*/ MOVW        R1, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15b0] 
    /*00007290*/ MOVT        R1, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15b0] 
    AIProcShow_x7294:
    /*00007294*/ STM         SP, {R0, R1} 
    /*00007298*/ MOV         R1, R8 
    /*0000729c*/ MOV         R0, FP 
    /*000072a0*/ BL          0x00000000000072A0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000072a4*/ B           AIProcShow_x59b8
    AIProcShow_x72a8:
    /*000072a8*/ MOVW        R8, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15b8] 
    /*000072ac*/ MOVT        R8, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15b8] 
    /*000072b0*/ B           AIProcShow_x7134
    AIProcShow_x72b4:
    /*000072b4*/ MOVW        R0, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15b8] 
    /*000072b8*/ MOVT        R0, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15b8] 
    /*000072bc*/ B           AIProcShow_x70a0
    AIProcShow_x72c0:
    /*000072c0*/ MOVW        R1, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15e8] 
    /*000072c4*/ MOVT        R1, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15e8] 
    /*000072c8*/ B           AIProcShow_x7294
    AIProcShow_x72cc:
    /*000072cc*/ MOVW        R0, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15e8] 
    /*000072d0*/ MOVT        R0, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15e8] 
    /*000072d4*/ B           AIProcShow_x70a0
    AIProcShow_x72d8:
    /*000072d8*/ MOVW        R8, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15e8] 
    /*000072dc*/ MOVT        R8, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15e8] 
    /*000072e0*/ B           AIProcShow_x7134
    AIProcShow_x72e4:
    /*000072e4*/ LDR         R3, [SP, #64]               @ 0x40 
    /*000072e8*/ MOV         R2, #0 
    /*000072ec*/ LDR         R1, [SP, #72]               @ 0x48 
    /*000072f0*/ MOV         R0, FP 
    /*000072f4*/ STR         R3, [SP] 
    /*000072f8*/ MOV         R3, R6 
    /*000072fc*/ BL          0x00000000000072FC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007300*/ LDR         R3, [R5, #264]              @ 0x108 
    /*00007304*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007308*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*0000730c*/ CMP         R3, #1 
    /*00007310*/ BEQ         AIProcShow_x7eb4
    /*00007314*/ CMP         R3, #0 
    /*00007318*/ MOVNE       R2, #0 
    AIProcShow_x731c:
    /*0000731c*/ MOVW        R7, #433                    @ 0x1b1 
    /*00007320*/ ADD         R1, R5, #428                @ 0x1ac 
    /*00007324*/ LDRSB       R7, [R5, R7] 
    /*00007328*/ ADD         R3, R5, #424                @ 0x1a8 
    /*0000732c*/ ADD         R0, R5, #432                @ 0x1b0 
    /*00007330*/ MOVW        LR, #431                    @ 0x1af 
    /*00007334*/ LDRSB       IP, [R1] 
    /*00007338*/ MOVW        R1, #430                    @ 0x1ae 
    /*0000733c*/ LDRSB       R3, [R3] 
    /*00007340*/ LDRSB       R0, [R0] 
    /*00007344*/ STR         R7, [SP, #32] 
    /*00007348*/ LDRSB       LR, [R5, LR] 
    /*0000734c*/ STR         LR, [SP, #24] 
    /*00007350*/ LDRSB       R7, [R5, R1] 
    /*00007354*/ MOVW        R1, #429                    @ 0x1ad 
    /*00007358*/ LDRSB       LR, [R5, R1] 
    /*0000735c*/ MOVW        R1, #427                    @ 0x1ab 
    /*00007360*/ LDRSB       R1, [R5, R1] 
    /*00007364*/ STR         R0, [SP, #28] 
    /*00007368*/ MOVW        R0, #426                    @ 0x1aa 
    /*0000736c*/ LDRSB       R0, [R5, R0] 
    /*00007370*/ STR         IP, [SP, #12] 
    /*00007374*/ MOVW        IP, #425                    @ 0x1a9 
    /*00007378*/ LDRSB       IP, [R5, IP] 
    /*0000737c*/ STR         R1, [SP, #8] 
    /*00007380*/ STR         R0, [SP, #4] 
    /*00007384*/ MOV         R0, FP 
    /*00007388*/ STR         R7, [SP, #20] 
    /*0000738c*/ STR         LR, [SP, #16] 
    /*00007390*/ LDR         R1, [SP, #104]              @ 0x68 
    /*00007394*/ STR         IP, [SP] 
    /*00007398*/ BL          0x0000000000007398          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000739c*/ B           AIProcShow_x6dc4
    AIProcShow_x73a0:
    /*000073a0*/ LDR         R3, [SP, #56]               @ 0x38 
    /*000073a4*/ MOV         R2, #0 
    /*000073a8*/ LDR         R1, [SP, #64]               @ 0x40 
    /*000073ac*/ MOV         R0, FP 
    /*000073b0*/ STR         R3, [SP] 
    /*000073b4*/ MOV         R3, R6 
    /*000073b8*/ BL          0x00000000000073B8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000073bc*/ LDR         R3, [R5, #244]              @ 0xf4 
    /*000073c0*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*000073c4*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*000073c8*/ CMP         R3, #1 
    /*000073cc*/ MOVEQ       R2, R7 
    /*000073d0*/ BEQ         AIProcShow_x73dc
    /*000073d4*/ CMP         R3, #0 
    /*000073d8*/ MOVNE       R2, #0 
    AIProcShow_x73dc:
    /*000073dc*/ LDR         IP, [R5, #312]              @ 0x138 
    /*000073e0*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*000073e4*/ LDR         R0, [R5, #316]              @ 0x13c 
    /*000073e8*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*000073ec*/ CMP         IP, #1 
    /*000073f0*/ MOVW        R1, #0                      @ .LC124 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b5c] 
    /*000073f4*/ MOVT        R1, #0                      @ .LC124 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b5c] 
    /*000073f8*/ STR         R0, [SP] 
    /*000073fc*/ MOVEQ       R3, R7 
    /*00007400*/ MOV         R0, FP 
    /*00007404*/ BL          0x0000000000007404          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007408*/ B           AIProcShow_x6c90
    AIProcShow_x740c:
    /*0000740c*/ MOV         R3, R6 
    /*00007410*/ MOV         R2, #0 
    /*00007414*/ STR         R8, [SP] 
    /*00007418*/ MOV         R0, FP 
    /*0000741c*/ LDR         R1, [SP, #56]               @ 0x38 
    /*00007420*/ BL          0x0000000000007420          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007424*/ LDR         R3, [R5, #256]              @ 0x100 
    /*00007428*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*0000742c*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007430*/ CMP         R3, #1 
    /*00007434*/ MOVEQ       R2, R7 
    /*00007438*/ BEQ         AIProcShow_x7444
    /*0000743c*/ CMP         R3, #0 
    /*00007440*/ MOVNE       R2, #0 
    AIProcShow_x7444:
    /*00007444*/ LDR         LR, [R5, #388]              @ 0x184 
    /*00007448*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*0000744c*/ LDR         R1, [R5, #400]              @ 0x190 
    /*00007450*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007454*/ LDR         R0, [R5, #396]              @ 0x18c 
    /*00007458*/ CMP         LR, #1 
    /*0000745c*/ LDR         IP, [R5, #392]              @ 0x188 
    /*00007460*/ MOVEQ       R3, R7 
    /*00007464*/ STR         R0, [SP] 
    /*00007468*/ MOV         R0, FP 
    /*0000746c*/ STMIB       SP, {R1, IP} 
    /*00007470*/ MOVW        R1, #0                      @ .LC129 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ba4] 
    /*00007474*/ MOVT        R1, #0                      @ .LC129 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ba4] 
    /*00007478*/ BL          0x0000000000007478          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000747c*/ B           AIProcShow_x6bd4
    AIProcShow_x7480:
    /*00007480*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00007484*/ MOV         R2, #0 
    /*00007488*/ MOV         R1, R7 
    /*0000748c*/ MOV         R0, FP 
    /*00007490*/ STR         R3, [SP] 
    /*00007494*/ MOV         R3, R6 
    /*00007498*/ BL          0x0000000000007498          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000749c*/ LDR         R3, [R5, #248]              @ 0xf8 
    /*000074a0*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*000074a4*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*000074a8*/ CMP         R3, #1 
    /*000074ac*/ MOVEQ       R2, R8 
    /*000074b0*/ BEQ         AIProcShow_x74bc
    /*000074b4*/ CMP         R3, #0 
    /*000074b8*/ MOVNE       R2, #0 
    AIProcShow_x74bc:
    /*000074bc*/ ADD         R1, R5, #360                @ 0x168 
    /*000074c0*/ ADD         R0, R5, #356                @ 0x164 
    /*000074c4*/ ADD         R3, R5, #352                @ 0x160 
    /*000074c8*/ MOVW        IP, #362                    @ 0x16a 
    /*000074cc*/ LDRSH       R1, [R1] 
    /*000074d0*/ STR         R1, [SP, #20] 
    /*000074d4*/ MOVW        R1, #358                    @ 0x166 
    /*000074d8*/ LDRSH       R0, [R0] 
    /*000074dc*/ STR         R0, [SP, #12] 
    /*000074e0*/ MOVW        R0, #354                    @ 0x162 
    /*000074e4*/ LDRSH       LR, [R5, IP] 
    /*000074e8*/ LDRSH       R1, [R5, R1] 
    /*000074ec*/ LDRSH       IP, [R5, R0] 
    /*000074f0*/ MOVW        R0, #350                    @ 0x15e 
    /*000074f4*/ LDRSH       R3, [R3] 
    /*000074f8*/ LDRSH       R0, [R5, R0] 
    /*000074fc*/ STR         R1, [SP, #16] 
    /*00007500*/ MOVW        R1, #0                      @ .LC163 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1dd8] 
    /*00007504*/ STR         R3, [SP, #4] 
    /*00007508*/ MOVT        R1, #0                      @ .LC163 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1dd8] 
    /*0000750c*/ STR         LR, [SP, #24] 
    /*00007510*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007514*/ STR         IP, [SP, #8] 
    /*00007518*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*0000751c*/ LDR         IP, [R5, #320]              @ 0x140 
    /*00007520*/ STR         R0, [SP] 
    /*00007524*/ MOV         R0, FP 
    /*00007528*/ CMP         IP, #1 
    /*0000752c*/ MOVEQ       R3, R8 
    /*00007530*/ BL          0x0000000000007530          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007534*/ B           AIProcShow_x6b04
    AIProcShow_x7538:
    /*00007538*/ LDR         R3, [SP, #64]               @ 0x40 
    /*0000753c*/ MOV         R2, #0 
    /*00007540*/ LDR         R1, [SP, #68]               @ 0x44 
    /*00007544*/ MOV         R0, FP 
    /*00007548*/ STR         R3, [SP] 
    /*0000754c*/ MOV         R3, R8 
    /*00007550*/ BL          0x0000000000007550          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007554*/ LDR         R3, [R7, #248]              @ 0xf8 
    /*00007558*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*0000755c*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007560*/ CMP         R3, #1 
    /*00007564*/ BEQ         AIProcShow_x7ec0
    /*00007568*/ CMP         R3, #0 
    /*0000756c*/ MOVNE       R2, #0 
    AIProcShow_x7570:
    /*00007570*/ ADD         IP, R7, #348                @ 0x15c 
    /*00007574*/ ADD         R0, R7, #340                @ 0x154 
    /*00007578*/ ADD         R3, R7, #344                @ 0x158 
    /*0000757c*/ MOVW        R1, #346                    @ 0x15a 
    /*00007580*/ LDRSH       IP, [IP] 
    /*00007584*/ STR         IP, [SP, #20] 
    /*00007588*/ MOVW        IP, #342                    @ 0x156 
    /*0000758c*/ LDRSH       LR, [R0] 
    /*00007590*/ MOVW        R0, #338                    @ 0x152 
    /*00007594*/ LDRSH       R1, [R7, R1] 
    /*00007598*/ LDRSH       IP, [R7, IP] 
    /*0000759c*/ LDRSH       R3, [R3] 
    /*000075a0*/ LDRSH       R0, [R7, R0] 
    /*000075a4*/ STR         LR, [SP, #4] 
    /*000075a8*/ STR         R3, [SP, #12] 
    /*000075ac*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*000075b0*/ STR         R1, [SP, #16] 
    /*000075b4*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*000075b8*/ STR         IP, [SP, #8] 
    /*000075bc*/ MOVW        R1, #0                      @ .LC162 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1db8] 
    /*000075c0*/ LDR         IP, [R7, #320]              @ 0x140 
    /*000075c4*/ MOVT        R1, #0                      @ .LC162 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1db8] 
    /*000075c8*/ LDR         LR, [SP, #56]               @ 0x38 
    /*000075cc*/ CMP         IP, #1 
    /*000075d0*/ STR         R0, [SP] 
    /*000075d4*/ MOV         R0, FP 
    /*000075d8*/ MOVEQ       R3, LR 
    /*000075dc*/ BL          0x00000000000075DC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000075e0*/ B           AIProcShow_x6a04
    AIProcShow_x75e4:
    /*000075e4*/ LDR         R3, [SP, #64]               @ 0x40 
    /*000075e8*/ MOV         R2, #0 
    /*000075ec*/ LDR         R1, [SP, #68]               @ 0x44 
    /*000075f0*/ MOV         R0, FP 
    /*000075f4*/ STR         R3, [SP] 
    /*000075f8*/ MOV         R3, R8 
    /*000075fc*/ BL          0x00000000000075FC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007600*/ LDR         R3, [R7, #248]              @ 0xf8 
    /*00007604*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007608*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*0000760c*/ CMP         R3, #1 
    /*00007610*/ BEQ         AIProcShow_x7ec8
    /*00007614*/ CMP         R3, #0 
    /*00007618*/ MOVNE       R2, #0 
    AIProcShow_x761c:
    /*0000761c*/ ADD         IP, R7, #336                @ 0x150 
    /*00007620*/ ADD         R0, R7, #364                @ 0x16c 
    /*00007624*/ ADD         R3, R7, #332                @ 0x14c 
    /*00007628*/ MOVW        R1, #334                    @ 0x14e 
    /*0000762c*/ LDRSH       IP, [IP] 
    /*00007630*/ STR         IP, [SP, #20] 
    /*00007634*/ MOVW        IP, #330                    @ 0x14a 
    /*00007638*/ LDRSH       LR, [R0] 
    /*0000763c*/ MOVW        R0, #366                    @ 0x16e 
    /*00007640*/ LDRSH       R1, [R7, R1] 
    /*00007644*/ LDRSH       IP, [R7, IP] 
    /*00007648*/ LDRSH       R3, [R3] 
    /*0000764c*/ LDRSH       R0, [R7, R0] 
    /*00007650*/ STR         LR, [SP] 
    /*00007654*/ STR         R3, [SP, #12] 
    /*00007658*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*0000765c*/ STR         R1, [SP, #16] 
    /*00007660*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007664*/ STR         IP, [SP, #8] 
    /*00007668*/ MOVW        R1, #0                      @ .LC153 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d38] 
    /*0000766c*/ LDR         IP, [R7, #320]              @ 0x140 
    /*00007670*/ MOVT        R1, #0                      @ .LC153 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d38] 
    /*00007674*/ LDR         LR, [SP, #56]               @ 0x38 
    /*00007678*/ CMP         IP, #1 
    /*0000767c*/ STR         R0, [SP, #4] 
    /*00007680*/ MOV         R0, FP 
    /*00007684*/ MOVEQ       R3, LR 
    /*00007688*/ BL          0x0000000000007688          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000768c*/ B           AIProcShow_x6918
    AIProcShow_x7690:
    /*00007690*/ LDR         R3, [SP, #64]               @ 0x40 
    /*00007694*/ MOV         R2, #0 
    /*00007698*/ LDR         R1, [SP, #68]               @ 0x44 
    /*0000769c*/ MOV         R0, FP 
    /*000076a0*/ STR         R3, [SP] 
    /*000076a4*/ MOV         R3, R8 
    /*000076a8*/ BL          0x00000000000076A8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000076ac*/ LDR         R3, [R7, #248]              @ 0xf8 
    /*000076b0*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*000076b4*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*000076b8*/ CMP         R3, #1 
    /*000076bc*/ BEQ         AIProcShow_x7ed0
    /*000076c0*/ CMP         R3, #0 
    /*000076c4*/ MOVNE       R2, #0 
    AIProcShow_x76c8:
    /*000076c8*/ ADD         R1, R7, #328                @ 0x148 
    /*000076cc*/ MOVW        R3, #326                    @ 0x146 
    /*000076d0*/ LDRSB       IP, [R7, R3] 
    /*000076d4*/ MOVW        R3, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1620] 
    /*000076d8*/ LDRSH       LR, [R1] 
    /*000076dc*/ MOVW        R1, #325                    @ 0x145 
    /*000076e0*/ LDRSB       R1, [R7, R1] 
    /*000076e4*/ MOVT        R3, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1620] 
    /*000076e8*/ STR         R1, [SP, #8] 
    /*000076ec*/ LDRB        R1, [R7, #324]              @ 0x144 
    /*000076f0*/ STR         LR, [SP, #4] 
    /*000076f4*/ MOV         R0, R1 
    /*000076f8*/ MOVW        R1, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1628] 
    /*000076fc*/ CMP         R0, #1 
    /*00007700*/ MOVT        R1, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1628] 
    /*00007704*/ MOVNE       R3, R1 
    /*00007708*/ STR         R3, [SP] 
    /*0000770c*/ LDR         R1, [R7, #220]              @ 0xdc 
    /*00007710*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007714*/ STR         IP, [SP, #12] 
    /*00007718*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*0000771c*/ LDR         LR, [R7, #224]              @ 0xe0 
    /*00007720*/ LDR         R0, [R7, #320]              @ 0x140 
    /*00007724*/ LDR         IP, [R7, #216]              @ 0xd8 
    /*00007728*/ STR         LR, [SP, #24] 
    /*0000772c*/ CMP         R0, #1 
    /*00007730*/ LDR         LR, [SP, #56]               @ 0x38 
    /*00007734*/ MOV         R0, FP 
    /*00007738*/ STR         R1, [SP, #20] 
    /*0000773c*/ MOVW        R1, #0                      @ .LC145 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ca0] 
    /*00007740*/ STR         IP, [SP, #16] 
    /*00007744*/ MOVT        R1, #0                      @ .LC145 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ca0] 
    /*00007748*/ MOVEQ       R3, LR 
    /*0000774c*/ BL          0x000000000000774C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007750*/ B           AIProcShow_x682c
    AIProcShow_x7754:
    /*00007754*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00007758*/ MOV         R2, #0 
    /*0000775c*/ MOV         R1, R8 
    /*00007760*/ MOV         R0, FP 
    /*00007764*/ STR         R3, [SP] 
    /*00007768*/ MOV         R3, R6 
    /*0000776c*/ BL          0x000000000000776C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007770*/ LDR         R3, [R5, #272]              @ 0x110 
    /*00007774*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007778*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*0000777c*/ CMP         R3, #1 
    /*00007780*/ MOVEQ       R2, R7 
    /*00007784*/ BEQ         AIProcShow_x7790
    /*00007788*/ CMP         R3, #0 
    /*0000778c*/ MOVNE       R2, #0 
    AIProcShow_x7790:
    /*00007790*/ ADD         R3, R5, #488                @ 0x1e8 
    /*00007794*/ ADD         LR, R5, #484                @ 0x1e4 
    /*00007798*/ MOVW        R1, #490                    @ 0x1ea 
    /*0000779c*/ MOVW        IP, #486                    @ 0x1e6 
    /*000077a0*/ LDRSH       R1, [R5, R1] 
    /*000077a4*/ MOVW        R0, #482                    @ 0x1e2 
    /*000077a8*/ LDRSH       IP, [R5, IP] 
    /*000077ac*/ LDRSH       R3, [R3] 
    /*000077b0*/ LDRSH       LR, [LR] 
    /*000077b4*/ LDRSH       R0, [R5, R0] 
    /*000077b8*/ STR         R3, [SP, #12] 
    /*000077bc*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*000077c0*/ STR         R1, [SP, #16] 
    /*000077c4*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*000077c8*/ STR         LR, [SP, #4] 
    /*000077cc*/ MOVW        R1, #0                      @ .LC179 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ea0] 
    /*000077d0*/ STR         IP, [SP, #8] 
    /*000077d4*/ MOVT        R1, #0                      @ .LC179 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ea0] 
    /*000077d8*/ LDR         IP, [R5, #464]              @ 0x1d0 
    /*000077dc*/ STR         R0, [SP] 
    /*000077e0*/ MOV         R0, FP 
    /*000077e4*/ CMP         IP, #1 
    /*000077e8*/ MOVEQ       R3, R7 
    /*000077ec*/ BL          0x00000000000077EC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000077f0*/ B           AIProcShow_x6728
    AIProcShow_x77f4:
    /*000077f4*/ LDR         R3, [SP, #56]               @ 0x38 
    /*000077f8*/ MOV         R2, #0 
    /*000077fc*/ LDR         R1, [SP, #104]              @ 0x68 
    /*00007800*/ MOV         R0, FP 
    /*00007804*/ STR         R3, [SP] 
    /*00007808*/ MOV         R3, R6 
    /*0000780c*/ BL          0x000000000000780C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007810*/ LDR         R3, [R5, #252]              @ 0xfc 
    /*00007814*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007818*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*0000781c*/ CMP         R3, #1 
    /*00007820*/ MOVEQ       R2, SL 
    /*00007824*/ BEQ         AIProcShow_x7830
    /*00007828*/ CMP         R3, #0 
    /*0000782c*/ MOVNE       R2, #0 
    AIProcShow_x7830:
    /*00007830*/ ADD         R3, R5, #380                @ 0x17c 
    /*00007834*/ ADD         R1, R5, #376                @ 0x178 
    /*00007838*/ MOVW        R0, #378                    @ 0x17a 
    /*0000783c*/ LDRSB       IP, [R3] 
    /*00007840*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007844*/ LDRSH       LR, [R1] 
    /*00007848*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*0000784c*/ LDRSH       R0, [R5, R0] 
    /*00007850*/ MOVW        R1, #0                      @ .LC99 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19fc] 
    /*00007854*/ STR         IP, [SP, #8] 
    /*00007858*/ MOVT        R1, #0                      @ .LC99 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19fc] 
    /*0000785c*/ STR         LR, [SP] 
    /*00007860*/ LDR         IP, [R5, #372]              @ 0x174 
    /*00007864*/ STR         R0, [SP, #4] 
    /*00007868*/ MOV         R0, FP 
    /*0000786c*/ CMP         IP, #1 
    /*00007870*/ MOVEQ       R3, SL 
    /*00007874*/ BL          0x0000000000007874          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007878*/ B           AIProcShow_x6fdc
    AIProcShow_x787c:
    /*0000787c*/ LDR         R3, [SP, #64]               @ 0x40 
    /*00007880*/ MOV         R2, #0 
    /*00007884*/ LDR         R1, [SP, #104]              @ 0x68 
    /*00007888*/ MOV         R0, FP 
    /*0000788c*/ STR         R3, [SP] 
    /*00007890*/ MOV         R3, R6 
    /*00007894*/ BL          0x0000000000007894          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007898*/ LDR         R3, [R5, #260]              @ 0x104 
    /*0000789c*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*000078a0*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*000078a4*/ CMP         R3, #1 
    /*000078a8*/ BEQ         AIProcShow_x7ee0
    /*000078ac*/ CMP         R3, #0 
    /*000078b0*/ MOVNE       R2, #0 
    AIProcShow_x78b4:
    /*000078b4*/ ADD         R0, R5, #408                @ 0x198 
    /*000078b8*/ ADD         IP, R5, #412                @ 0x19c 
    /*000078bc*/ ADD         R1, R5, #416                @ 0x1a0 
    /*000078c0*/ MOVW        R3, #413                    @ 0x19d 
    /*000078c4*/ LDRSB       R0, [R0] 
    /*000078c8*/ STR         R0, [SP, #24] 
    /*000078cc*/ MOVW        R0, #414                    @ 0x19e 
    /*000078d0*/ LDRSB       IP, [IP] 
    /*000078d4*/ STR         IP, [SP, #8] 
    /*000078d8*/ LDRSH       R1, [R1] 
    /*000078dc*/ STR         R1, [SP] 
    /*000078e0*/ MOVW        R1, #409                    @ 0x199 
    /*000078e4*/ LDRSB       LR, [R5, R0] 
    /*000078e8*/ MOVW        R0, #410                    @ 0x19a 
    /*000078ec*/ LDRSB       IP, [R5, R0] 
    /*000078f0*/ MOVW        R0, #411                    @ 0x19b 
    /*000078f4*/ LDRSB       R3, [R5, R3] 
    /*000078f8*/ LDRSB       R1, [R5, R1] 
    /*000078fc*/ LDRSB       R0, [R5, R0] 
    /*00007900*/ STR         R3, [SP, #28] 
    /*00007904*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007908*/ STR         LR, [SP, #20] 
    /*0000790c*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007910*/ STR         R1, [SP, #16] 
    /*00007914*/ MOVW        R1, #0                      @ .LC109 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a98] 
    /*00007918*/ STR         IP, [SP, #12] 
    /*0000791c*/ MOVT        R1, #0                      @ .LC109 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a98] 
    /*00007920*/ LDR         IP, [R5, #404]              @ 0x194 
    /*00007924*/ LDR         LR, [SP, #56]               @ 0x38 
    /*00007928*/ CMP         IP, #1 
    /*0000792c*/ STR         R0, [SP, #4] 
    /*00007930*/ MOV         R0, FP 
    /*00007934*/ MOVEQ       R3, LR 
    /*00007938*/ BL          0x0000000000007938          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000793c*/ B           AIProcShow_x6ef8
    AIProcShow_x7940:
    /*00007940*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00007944*/ MOV         R2, #0 
    /*00007948*/ LDR         R1, [SP, #64]               @ 0x40 
    /*0000794c*/ MOV         R0, FP 
    /*00007950*/ STR         R3, [SP] 
    /*00007954*/ MOV         R3, R7 
    /*00007958*/ BL          0x0000000000007958          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000795c*/ LDR         R3, [R6, #272]              @ 0x110 
    /*00007960*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007964*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007968*/ CMP         R3, #1 
    /*0000796c*/ MOVEQ       R2, R8 
    /*00007970*/ BEQ         AIProcShow_x797c
    /*00007974*/ CMP         R3, #0 
    /*00007978*/ MOVNE       R2, #0 
    AIProcShow_x797c:
    /*0000797c*/ ADD         R3, R6, #480                @ 0x1e0 
    /*00007980*/ ADD         LR, R6, #476                @ 0x1dc 
    /*00007984*/ ADD         R1, R6, #472                @ 0x1d8 
    /*00007988*/ MOVW        IP, #478                    @ 0x1de 
    /*0000798c*/ LDRSH       IP, [R6, IP] 
    /*00007990*/ MOVW        R0, #474                    @ 0x1da 
    /*00007994*/ LDRSH       R3, [R3] 
    /*00007998*/ LDRSH       LR, [LR] 
    /*0000799c*/ LDRSH       R1, [R1] 
    /*000079a0*/ LDRSH       R0, [R6, R0] 
    /*000079a4*/ STR         R3, [SP, #16] 
    /*000079a8*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*000079ac*/ STR         R1, [SP] 
    /*000079b0*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*000079b4*/ STR         LR, [SP, #8] 
    /*000079b8*/ MOVW        R1, #0                      @ .LC179 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ea0] 
    /*000079bc*/ STR         IP, [SP, #12] 
    /*000079c0*/ MOVT        R1, #0                      @ .LC179 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ea0] 
    /*000079c4*/ LDR         IP, [R6, #464]              @ 0x1d0 
    /*000079c8*/ STR         R0, [SP, #4] 
    /*000079cc*/ MOV         R0, FP 
    /*000079d0*/ CMP         IP, #1 
    /*000079d4*/ MOVEQ       R3, R8 
    /*000079d8*/ BL          0x00000000000079D8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000079dc*/ B           AIProcShow_x6648
    AIProcShow_x79e0:
    /*000079e0*/ LDR         R3, [SP, #68]               @ 0x44 
    /*000079e4*/ MOV         R2, #0 
    /*000079e8*/ LDR         R1, [SP, #72]               @ 0x48 
    /*000079ec*/ MOV         R0, FP 
    /*000079f0*/ STR         R3, [SP] 
    /*000079f4*/ MOV         R3, R7 
    /*000079f8*/ BL          0x00000000000079F8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000079fc*/ LDR         R3, [R6, #276]              @ 0x114 
    /*00007a00*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007a04*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007a08*/ CMP         R3, #1 
    /*00007a0c*/ BEQ         AIProcShow_x7ed8
    /*00007a10*/ CMP         R3, #0 
    /*00007a14*/ MOVNE       R2, #0 
    AIProcShow_x7a18:
    /*00007a18*/ ADD         R1, R6, #516                @ 0x204 
    /*00007a1c*/ MOVW        R8, #514                    @ 0x202 
    /*00007a20*/ LDRSB       R8, [R6, R8] 
    /*00007a24*/ ADD         R0, R6, #512                @ 0x200 
    /*00007a28*/ LDRSB       R1, [R1] 
    /*00007a2c*/ MOVW        R3, #515                    @ 0x203 
    /*00007a30*/ LDRSB       R0, [R0] 
    /*00007a34*/ MOVW        LR, #513                    @ 0x201 
    /*00007a38*/ LDRSB       IP, [R6, R3] 
    /*00007a3c*/ MOVW        R3, #511                    @ 0x1ff 
    /*00007a40*/ STR         R8, [SP, #20] 
    /*00007a44*/ LDRSB       R8, [R6, LR] 
    /*00007a48*/ MOVW        LR, #510                    @ 0x1fe 
    /*00007a4c*/ LDRSB       R3, [R6, R3] 
    /*00007a50*/ LDRSB       LR, [R6, LR] 
    /*00007a54*/ STR         R1, [SP, #28] 
    /*00007a58*/ MOVW        R1, #519                    @ 0x207 
    /*00007a5c*/ LDRSB       R1, [R6, R1] 
    /*00007a60*/ STR         R0, [SP, #12] 
    /*00007a64*/ MOVW        R0, #518                    @ 0x206 
    /*00007a68*/ LDRSB       R0, [R6, R0] 
    /*00007a6c*/ STR         IP, [SP, #24] 
    /*00007a70*/ MOVW        IP, #517                    @ 0x205 
    /*00007a74*/ LDRSB       IP, [R6, IP] 
    /*00007a78*/ STR         R3, [SP, #8] 
    /*00007a7c*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007a80*/ STR         R8, [SP, #16] 
    /*00007a84*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007a88*/ STR         LR, [SP, #4] 
    /*00007a8c*/ LDR         LR, [R6, #496]              @ 0x1f0 
    /*00007a90*/ STR         R1, [SP, #40]               @ 0x28 
    /*00007a94*/ MOVW        R1, #0                      @ .LC203 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f9c] 
    /*00007a98*/ STR         R0, [SP, #36]               @ 0x24 
    /*00007a9c*/ MOVT        R1, #0                      @ .LC203 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f9c] 
    /*00007aa0*/ STR         LR, [SP] 
    /*00007aa4*/ MOV         R0, FP 
    /*00007aa8*/ LDR         LR, [R6, #492]              @ 0x1ec 
    /*00007aac*/ STR         IP, [SP, #32] 
    /*00007ab0*/ LDR         IP, [SP, #60]               @ 0x3c 
    /*00007ab4*/ CMP         LR, #1 
    /*00007ab8*/ MOVEQ       R3, IP 
    /*00007abc*/ BL          0x0000000000007ABC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007ac0*/ B           AIProcShow_x6230
    AIProcShow_x7ac4:
    /*00007ac4*/ LDR         R3, [SP, #64]               @ 0x40 
    /*00007ac8*/ MOV         R2, #0 
    /*00007acc*/ LDR         R1, [SP, #68]               @ 0x44 
    /*00007ad0*/ MOV         R0, FP 
    /*00007ad4*/ STR         R3, [SP] 
    /*00007ad8*/ MOV         R3, R7 
    /*00007adc*/ BL          0x0000000000007ADC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007ae0*/ LDR         R3, [R6, #276]              @ 0x114 
    /*00007ae4*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007ae8*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007aec*/ CMP         R3, #1 
    /*00007af0*/ BEQ         AIProcShow_x7ea4
    /*00007af4*/ CMP         R3, #0 
    /*00007af8*/ MOVNE       R2, #0 
    AIProcShow_x7afc:
    /*00007afc*/ LDRB        R1, [R6, #505]              @ 0x1f9 
    /*00007b00*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007b04*/ LDRB        LR, [R6, #506]              @ 0x1fa 
    /*00007b08*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007b0c*/ STR         R1, [SP, #24] 
    /*00007b10*/ LDR         R0, [R6, #492]              @ 0x1ec 
    /*00007b14*/ LDRB        IP, [R6, #504]              @ 0x1f8 
    /*00007b18*/ LDR         R1, [SP, #60]               @ 0x3c 
    /*00007b1c*/ CMP         R0, #1 
    /*00007b20*/ LDRB        R0, [R6, #503]              @ 0x1f7 
    /*00007b24*/ STR         LR, [SP, #28] 
    /*00007b28*/ MOVEQ       R3, R1 
    /*00007b2c*/ LDRB        LR, [R6, #502]              @ 0x1f6 
    /*00007b30*/ LDRB        R1, [R6, #501]              @ 0x1f5 
    /*00007b34*/ STR         IP, [SP, #20] 
    /*00007b38*/ LDRB        IP, [R6, #500]              @ 0x1f4 
    /*00007b3c*/ STR         R0, [SP, #16] 
    /*00007b40*/ LDR         R0, [R6, #496]              @ 0x1f0 
    /*00007b44*/ STR         R1, [SP, #8] 
    /*00007b48*/ MOVW        R1, #0                      @ .LC203 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f9c] 
    /*00007b4c*/ STR         LR, [SP, #12] 
    /*00007b50*/ MOVT        R1, #0                      @ .LC203 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f9c] 
    /*00007b54*/ STR         IP, [SP, #4] 
    /*00007b58*/ LDRB        IP, [R6, #509]              @ 0x1fd 
    /*00007b5c*/ STR         R0, [SP] 
    /*00007b60*/ MOV         R0, FP 
    /*00007b64*/ LDRB        LR, [R6, #508]              @ 0x1fc 
    /*00007b68*/ STR         IP, [SP, #40]               @ 0x28 
    /*00007b6c*/ LDRB        IP, [R6, #507]              @ 0x1fb 
    /*00007b70*/ STR         LR, [SP, #36]               @ 0x24 
    /*00007b74*/ STR         IP, [SP, #32] 
    /*00007b78*/ BL          0x0000000000007B78          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007b7c*/ B           AIProcShow_x6108
    AIProcShow_x7b80:
    /*00007b80*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00007b84*/ MOV         R2, #0 
    /*00007b88*/ MOV         R1, R8 
    /*00007b8c*/ MOV         R0, FP 
    /*00007b90*/ STR         R3, [SP] 
    /*00007b94*/ MOV         R3, R6 
    /*00007b98*/ BL          0x0000000000007B98          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007b9c*/ LDR         R3, [R5, #276]              @ 0x114 
    /*00007ba0*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007ba4*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007ba8*/ CMP         R3, #1 
    /*00007bac*/ MOVEQ       R2, SL 
    /*00007bb0*/ BEQ         AIProcShow_x7bbc
    /*00007bb4*/ CMP         R3, #0 
    /*00007bb8*/ MOVNE       R2, #0 
    AIProcShow_x7bbc:
    /*00007bbc*/ ADD         R1, R5, #552                @ 0x228 
    /*00007bc0*/ ADD         LR, R5, #540                @ 0x21c 
    /*00007bc4*/ ADD         R3, R5, #544                @ 0x220 
    /*00007bc8*/ ADD         R0, R5, #548                @ 0x224 
    /*00007bcc*/ LDRH        R1, [R1] 
    /*00007bd0*/ STR         R1, [SP, #28] 
    /*00007bd4*/ ADD         R1, R5, #556                @ 0x22c 
    /*00007bd8*/ LDRH        LR, [LR] 
    /*00007bdc*/ LDRH        IP, [R3] 
    /*00007be0*/ MOVW        R3, #550                    @ 0x226 
    /*00007be4*/ LDRH        R0, [R0] 
    /*00007be8*/ STR         LR, [SP, #4] 
    /*00007bec*/ MOVW        LR, #542                    @ 0x21e 
    /*00007bf0*/ LDRH        R3, [R5, R3] 
    /*00007bf4*/ STR         R3, [SP, #24] 
    /*00007bf8*/ MOVW        R3, #546                    @ 0x222 
    /*00007bfc*/ LDRH        R3, [R5, R3] 
    /*00007c00*/ LDRH        LR, [R5, LR] 
    /*00007c04*/ LDRH        R1, [R1] 
    /*00007c08*/ STR         R0, [SP, #20] 
    /*00007c0c*/ MOVW        R0, #558                    @ 0x22e 
    /*00007c10*/ LDRH        R0, [R5, R0] 
    /*00007c14*/ STR         IP, [SP, #12] 
    /*00007c18*/ MOVW        IP, #554                    @ 0x22a 
    /*00007c1c*/ LDRH        IP, [R5, IP] 
    /*00007c20*/ STR         R3, [SP, #16] 
    /*00007c24*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007c28*/ STR         LR, [SP, #8] 
    /*00007c2c*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007c30*/ LDR         LR, [R5, #496]              @ 0x1f0 
    /*00007c34*/ STR         R1, [SP, #36]               @ 0x24 
    /*00007c38*/ MOVW        R1, #0                      @ .LC203 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f9c] 
    /*00007c3c*/ STR         R0, [SP, #40]               @ 0x28 
    /*00007c40*/ MOVT        R1, #0                      @ .LC203 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f9c] 
    /*00007c44*/ STR         LR, [SP] 
    /*00007c48*/ MOV         R0, FP 
    /*00007c4c*/ LDR         LR, [R5, #492]              @ 0x1ec 
    /*00007c50*/ STR         IP, [SP, #32] 
    /*00007c54*/ CMP         LR, #1 
    /*00007c58*/ MOVEQ       R3, SL 
    /*00007c5c*/ BL          0x0000000000007C5C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007c60*/ B           AIProcShow_x649c
    AIProcShow_x7c64:
    /*00007c64*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00007c68*/ MOV         R2, #0 
    /*00007c6c*/ LDR         R1, [SP, #64]               @ 0x40 
    /*00007c70*/ MOV         R0, FP 
    /*00007c74*/ STR         R3, [SP] 
    /*00007c78*/ MOV         R3, R7 
    /*00007c7c*/ BL          0x0000000000007C7C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007c80*/ LDR         R3, [R6, #272]              @ 0x110 
    /*00007c84*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007c88*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007c8c*/ CMP         R3, #1 
    /*00007c90*/ MOVEQ       R2, R8 
    /*00007c94*/ BEQ         AIProcShow_x7ca0
    /*00007c98*/ CMP         R3, #0 
    /*00007c9c*/ MOVNE       R2, #0 
    AIProcShow_x7ca0:
    /*00007ca0*/ ADD         R0, R6, #468                @ 0x1d4 
    /*00007ca4*/ MOVW        R3, #470                    @ 0x1d6 
    /*00007ca8*/ LDRSH       R1, [R6, R3] 
    /*00007cac*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007cb0*/ LDRSH       IP, [R0] 
    /*00007cb4*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007cb8*/ LDR         R0, [R6, #464]              @ 0x1d0 
    /*00007cbc*/ STR         R1, [SP, #4] 
    /*00007cc0*/ MOVW        R1, #0                      @ .LC173 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e68] 
    /*00007cc4*/ CMP         R0, #1 
    /*00007cc8*/ STR         IP, [SP] 
    /*00007ccc*/ MOVT        R1, #0                      @ .LC173 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e68] 
    /*00007cd0*/ MOV         R0, FP 
    /*00007cd4*/ MOVEQ       R3, R8 
    /*00007cd8*/ BL          0x0000000000007CD8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007cdc*/ B           AIProcShow_x6568
    AIProcShow_x7ce0:
    /*00007ce0*/ LDR         R3, [SP, #64]               @ 0x40 
    /*00007ce4*/ MOV         R2, #0 
    /*00007ce8*/ LDR         R1, [SP, #68]               @ 0x44 
    /*00007cec*/ MOV         R0, FP 
    /*00007cf0*/ STR         R3, [SP] 
    /*00007cf4*/ MOV         R3, R7 
    /*00007cf8*/ BL          0x0000000000007CF8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007cfc*/ LDR         R3, [R6, #276]              @ 0x114 
    /*00007d00*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007d04*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007d08*/ CMP         R3, #1 
    /*00007d0c*/ BEQ         AIProcShow_x7eac
    /*00007d10*/ CMP         R3, #0 
    /*00007d14*/ MOVNE       R2, #0 
    AIProcShow_x7d18:
    /*00007d18*/ ADD         R1, R6, #532                @ 0x214 
    /*00007d1c*/ ADD         LR, R6, #520                @ 0x208 
    /*00007d20*/ ADD         R3, R6, #524                @ 0x20c 
    /*00007d24*/ ADD         R0, R6, #528                @ 0x210 
    /*00007d28*/ LDRH        R1, [R1] 
    /*00007d2c*/ STR         R1, [SP, #28] 
    /*00007d30*/ ADD         R1, R6, #536                @ 0x218 
    /*00007d34*/ LDRH        LR, [LR] 
    /*00007d38*/ LDRH        IP, [R3] 
    /*00007d3c*/ MOVW        R3, #530                    @ 0x212 
    /*00007d40*/ LDRH        R0, [R0] 
    /*00007d44*/ STR         LR, [SP, #4] 
    /*00007d48*/ MOVW        LR, #522                    @ 0x20a 
    /*00007d4c*/ LDRH        R3, [R6, R3] 
    /*00007d50*/ STR         R3, [SP, #24] 
    /*00007d54*/ MOVW        R3, #526                    @ 0x20e 
    /*00007d58*/ LDRH        R3, [R6, R3] 
    /*00007d5c*/ LDRH        LR, [R6, LR] 
    /*00007d60*/ LDRH        R1, [R1] 
    /*00007d64*/ STR         R0, [SP, #20] 
    /*00007d68*/ MOVW        R0, #538                    @ 0x21a 
    /*00007d6c*/ LDRH        R0, [R6, R0] 
    /*00007d70*/ STR         IP, [SP, #12] 
    /*00007d74*/ MOVW        IP, #534                    @ 0x216 
    /*00007d78*/ LDRH        IP, [R6, IP] 
    /*00007d7c*/ STR         R3, [SP, #16] 
    /*00007d80*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15ac] 
    /*00007d84*/ STR         LR, [SP, #8] 
    /*00007d88*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15ac] 
    /*00007d8c*/ LDR         LR, [R6, #496]              @ 0x1f0 
    /*00007d90*/ STR         R1, [SP, #36]               @ 0x24 
    /*00007d94*/ MOVW        R1, #0                      @ .LC203 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f9c] 
    /*00007d98*/ STR         R0, [SP, #40]               @ 0x28 
    /*00007d9c*/ MOVT        R1, #0                      @ .LC203 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f9c] 
    /*00007da0*/ STR         LR, [SP] 
    /*00007da4*/ MOV         R0, FP 
    /*00007da8*/ LDR         LR, [R6, #492]              @ 0x1ec 
    /*00007dac*/ STR         IP, [SP, #32] 
    /*00007db0*/ LDR         IP, [SP, #60]               @ 0x3c 
    /*00007db4*/ CMP         LR, #1 
    /*00007db8*/ MOVEQ       R3, IP 
    /*00007dbc*/ BL          0x0000000000007DBC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007dc0*/ B           AIProcShow_x6364
    AIProcShow_x7dc4:
    /*00007dc4*/ MOVW        R1, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15b8] 
    /*00007dc8*/ MOVT        R1, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15b8] 
    /*00007dcc*/ B           AIProcShow_x7294
    AIProcShow_x7dd0:
    /*00007dd0*/ MOVW        R1, #0                      @ .LC75 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1828] 
    /*00007dd4*/ MOV         R0, FP 
    /*00007dd8*/ MOVT        R1, #0                      @ .LC75 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1828] 
    /*00007ddc*/ BL          0x0000000000007DDC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00007de0*/ B           AIProcShow_x5fa8
    AIProcShow_x7de4:
    /*00007de4*/ MOVW        R3, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1610] 
    /*00007de8*/ MOVT        R3, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1610] 
    /*00007dec*/ B           AIProcShow_x5dcc
    AIProcShow_x7df0:
    /*00007df0*/ MOVW        R8, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15c0] 
    /*00007df4*/ MOVT        R8, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15c0] 
    /*00007df8*/ B           AIProcShow_x7134
    AIProcShow_x7dfc:
    /*00007dfc*/ MOVW        R0, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15c0] 
    /*00007e00*/ MOVT        R0, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15c0] 
    /*00007e04*/ B           AIProcShow_x70a0
    AIProcShow_x7e08:
    /*00007e08*/ MOVW        R1, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15c0] 
    /*00007e0c*/ MOVT        R1, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15c0] 
    /*00007e10*/ B           AIProcShow_x7294
    AIProcShow_x7e14:
    /*00007e14*/ MOVW        R0, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15c8] 
    /*00007e18*/ MOVT        R0, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15c8] 
    /*00007e1c*/ B           AIProcShow_x70a0
    AIProcShow_x7e20:
    /*00007e20*/ MOVW        R8, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15c8] 
    /*00007e24*/ MOVT        R8, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15c8] 
    /*00007e28*/ B           AIProcShow_x7134
    AIProcShow_x7e2c:
    /*00007e2c*/ MOVW        R1, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15c8] 
    /*00007e30*/ MOVT        R1, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15c8] 
    /*00007e34*/ B           AIProcShow_x7294
    AIProcShow_x7e38:
    /*00007e38*/ MOVW        R1, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15d0] 
    /*00007e3c*/ MOVT        R1, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15d0] 
    /*00007e40*/ B           AIProcShow_x7294
    AIProcShow_x7e44:
    /*00007e44*/ MOVW        R0, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15d0] 
    /*00007e48*/ MOVT        R0, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15d0] 
    /*00007e4c*/ B           AIProcShow_x70a0
    AIProcShow_x7e50:
    /*00007e50*/ MOVW        R8, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15d0] 
    /*00007e54*/ MOVT        R8, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15d0] 
    /*00007e58*/ B           AIProcShow_x7134
    AIProcShow_x7e5c:
    /*00007e5c*/ MOVW        R1, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15d8] 
    /*00007e60*/ MOVT        R1, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15d8] 
    /*00007e64*/ B           AIProcShow_x7294
    AIProcShow_x7e68:
    /*00007e68*/ MOVW        R8, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15d8] 
    /*00007e6c*/ MOVT        R8, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15d8] 
    /*00007e70*/ B           AIProcShow_x7134
    AIProcShow_x7e74:
    /*00007e74*/ MOVW        R0, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15d8] 
    /*00007e78*/ MOVT        R0, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15d8] 
    /*00007e7c*/ B           AIProcShow_x70a0
    AIProcShow_x7e80:
    /*00007e80*/ MOVW        R0, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15e0] 
    /*00007e84*/ MOVT        R0, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15e0] 
    /*00007e88*/ B           AIProcShow_x70a0
    AIProcShow_x7e8c:
    /*00007e8c*/ MOVW        R1, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15e0] 
    /*00007e90*/ MOVT        R1, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15e0] 
    /*00007e94*/ B           AIProcShow_x7294
    AIProcShow_x7e98:
    /*00007e98*/ MOVW        R8, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15e0] 
    /*00007e9c*/ MOVT        R8, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15e0] 
    /*00007ea0*/ B           AIProcShow_x7134
    AIProcShow_x7ea4:
    /*00007ea4*/ LDR         R2, [SP, #60]               @ 0x3c 
    /*00007ea8*/ B           AIProcShow_x7afc
    AIProcShow_x7eac:
    /*00007eac*/ LDR         R2, [SP, #60]               @ 0x3c 
    /*00007eb0*/ B           AIProcShow_x7d18
    AIProcShow_x7eb4:
    /*00007eb4*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*00007eb8*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*00007ebc*/ B           AIProcShow_x731c
    AIProcShow_x7ec0:
    /*00007ec0*/ LDR         R2, [SP, #56]               @ 0x38 
    /*00007ec4*/ B           AIProcShow_x7570
    AIProcShow_x7ec8:
    /*00007ec8*/ LDR         R2, [SP, #56]               @ 0x38 
    /*00007ecc*/ B           AIProcShow_x761c
    AIProcShow_x7ed0:
    /*00007ed0*/ LDR         R2, [SP, #56]               @ 0x38 
    /*00007ed4*/ B           AIProcShow_x76c8
    AIProcShow_x7ed8:
    /*00007ed8*/ LDR         R2, [SP, #60]               @ 0x3c 
    /*00007edc*/ B           AIProcShow_x7a18
    AIProcShow_x7ee0:
    /*00007ee0*/ LDR         R2, [SP, #56]               @ 0x38 
    /*00007ee4*/ B           AIProcShow_x78b4
    AIProcShow_x7ee8:
    /*00007ee8*/ MOVW        R1, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15f0] 
    /*00007eec*/ MOVT        R1, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15f0] 
    /*00007ef0*/ B           AIProcShow_x7294
    AIProcShow_x7ef4:
    /*00007ef4*/ MOVW        R0, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15f0] 
    /*00007ef8*/ MOVT        R0, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15f0] 
    /*00007efc*/ B           AIProcShow_x70a0
    AIProcShow_x7f00:
    /*00007f00*/ MOVW        R8, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15f0] 
    /*00007f04*/ MOVT        R8, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15f0] 
    /*00007f08*/ B           AIProcShow_x7134
    AIProcShow_x7f0c:
    /*00007f0c*/ MOVW        R8, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1600] 
    /*00007f10*/ MOVT        R8, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1600] 
    /*00007f14*/ B           AIProcShow_x7134
    AIProcShow_x7f18:
    /*00007f18*/ MOVW        R1, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1600] 
    /*00007f1c*/ MOVT        R1, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1600] 
    /*00007f20*/ B           AIProcShow_x7294
    AIProcShow_x7f24:
    /*00007f24*/ MOVW        R0, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1600] 
    /*00007f28*/ MOVT        R0, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1600] 
    /*00007f2c*/ B           AIProcShow_x70a0
FUNC_END AIProcShow


FUNC_BEGIN AUDIO_FB_Init
    /*00007f30*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00007f34*/ SUBS        R5, R1, #0 
    /*00007f38*/ BEQ         AUDIO_FB_Init_x7fc0
    /*00007f3c*/ MOV         R8, #136                    @ 0x88 
    /*00007f40*/ ADD         R3, R5, #12 
    /*00007f44*/ MUL         R8, R8, R0 
    /*00007f48*/ ADD         R7, R5, #4 
    /*00007f4c*/ MOV         R6, R0 
    /*00007f50*/ STR         R7, [R5, #4] 
    /*00007f54*/ STR         R7, [R5, #8] 
    /*00007f58*/ STR         R3, [R5, #12] 
    /*00007f5c*/ STR         R3, [R5, #16] 
    /*00007f60*/ MOV         R0, R8 
    /*00007f64*/ BL          0x0000000000007F64          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*00007f68*/ SUBS        R4, R0, #0 
    /*00007f6c*/ BEQ         AUDIO_FB_Init_x7fc0
    /*00007f70*/ MOV         R2, R8 
    /*00007f74*/ MOV         R1, #0 
    /*00007f78*/ BL          0x0000000000007F78          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00007f7c*/ CMP         R6, #0 
    /*00007f80*/ STR         R4, [R5] 
    /*00007f84*/ MOVNE       R3, #0 
    /*00007f88*/ BEQ         AUDIO_FB_Init_x7fb0
    AUDIO_FB_Init_x7f8c:
    /*00007f8c*/ LDR         R2, [R5, #8] 
    /*00007f90*/ ADD         R3, R3, #1 
    /*00007f94*/ CMP         R3, R6 
    /*00007f98*/ STR         R4, [R5, #8] 
    /*00007f9c*/ STR         R7, [R4] 
    /*00007fa0*/ STR         R2, [R4, #4] 
    /*00007fa4*/ STR         R4, [R2] 
    /*00007fa8*/ ADD         R4, R4, #136                @ 0x88 
    /*00007fac*/ BNE         AUDIO_FB_Init_x7f8c
    AUDIO_FB_Init_x7fb0:
    /*00007fb0*/ MOV         R0, #0 
    /*00007fb4*/ STR         R0, [R5, #24] 
    /*00007fb8*/ STR         R6, [R5, #20] 
    /*00007fbc*/ POP         {R4, R5, R6, R7, R8, PC} 
    AUDIO_FB_Init_x7fc0:
    /*00007fc0*/ MVN         R0, #0 
    /*00007fc4*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END AUDIO_FB_Init


FUNC_BEGIN AUDIO_FB_Release
    /*00007fc8*/ PUSH        {R4, LR} 
    /*00007fcc*/ SUBS        R4, R0, #0 
    /*00007fd0*/ BEQ         AUDIO_FB_Release_x7ff4
    /*00007fd4*/ LDR         R0, [R4] 
    /*00007fd8*/ CMP         R0, #0 
    /*00007fdc*/ POPEQ       {R4, PC} 
    /*00007fe0*/ BL          0x0000000000007FE0          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*00007fe4*/ MOV         R3, #0 
    /*00007fe8*/ MOV         R0, R3 
    /*00007fec*/ STR         R3, [R4] 
    /*00007ff0*/ POP         {R4, PC} 
    AUDIO_FB_Release_x7ff4:
    /*00007ff4*/ MVN         R0, #0 
    /*00007ff8*/ POP         {R4, PC} 
FUNC_END AUDIO_FB_Release


FUNC_BEGIN AUDIO_FB_GetFree
    /*00007ffc*/ SUBS        R2, R0, #0 
    /*00008000*/ BEQ         AUDIO_FB_GetFree_x8054
    /*00008004*/ MOV         R1, R2 
    /*00008008*/ LDR         R3, [R1, #4]! 
    /*0000800c*/ CMP         R3, R1 
    /*00008010*/ BEQ         AUDIO_FB_GetFree_x8054
    /*00008014*/ PUSH        {R4, LR} 
    /*00008018*/ MOV         R0, R3 
    /*0000801c*/ LDR         LR, [R3, #4] 
    /*00008020*/ MOV         IP, #256                    @ 0x100 
    /*00008024*/ LDR         R4, [R3] 
    /*00008028*/ MOVT        IP, #16 
    /*0000802c*/ MOV         R1, #512                    @ 0x200 
    /*00008030*/ MOVT        R1, #32 
    /*00008034*/ STR         LR, [R4, #4] 
    /*00008038*/ STR         R4, [LR] 
    /*0000803c*/ STR         IP, [R3] 
    /*00008040*/ STR         R1, [R3, #4] 
    /*00008044*/ LDR         R3, [R2, #20] 
    /*00008048*/ SUB         R3, R3, #1 
    /*0000804c*/ STR         R3, [R2, #20] 
    /*00008050*/ POP         {R4, PC} 
    AUDIO_FB_GetFree_x8054:
    /*00008054*/ MOV         R0, #0 
    /*00008058*/ BX          LR 
FUNC_END AUDIO_FB_GetFree


FUNC_BEGIN AUDIO_FB_GetBusy
    /*0000805c*/ SUBS        R2, R0, #0 
    /*00008060*/ BEQ         AUDIO_FB_GetBusy_x80b4
    /*00008064*/ MOV         R1, R2 
    /*00008068*/ LDR         R3, [R1, #12]! 
    /*0000806c*/ CMP         R3, R1 
    /*00008070*/ BEQ         AUDIO_FB_GetBusy_x80b4
    /*00008074*/ PUSH        {R4, LR} 
    /*00008078*/ MOV         R0, R3 
    /*0000807c*/ LDR         LR, [R3, #4] 
    /*00008080*/ MOV         IP, #256                    @ 0x100 
    /*00008084*/ LDR         R4, [R3] 
    /*00008088*/ MOVT        IP, #16 
    /*0000808c*/ MOV         R1, #512                    @ 0x200 
    /*00008090*/ MOVT        R1, #32 
    /*00008094*/ STR         LR, [R4, #4] 
    /*00008098*/ STR         R4, [LR] 
    /*0000809c*/ STR         IP, [R3] 
    /*000080a0*/ STR         R1, [R3, #4] 
    /*000080a4*/ LDR         R3, [R2, #24] 
    /*000080a8*/ SUB         R3, R3, #1 
    /*000080ac*/ STR         R3, [R2, #24] 
    /*000080b0*/ POP         {R4, PC} 
    AUDIO_FB_GetBusy_x80b4:
    /*000080b4*/ MOV         R0, #0 
    /*000080b8*/ BX          LR 
FUNC_END AUDIO_FB_GetBusy


FUNC_BEGIN AUDIO_FB_PutFree
    /*000080bc*/ CMP         R1, #0 
    /*000080c0*/ CMPNE       R0, #0 
    /*000080c4*/ MOVEQ       R2, #1 
    /*000080c8*/ MOVNE       R2, #0 
    /*000080cc*/ BEQ         AUDIO_FB_PutFree_x8104
    /*000080d0*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000080d4*/ MOV         R3, R0 
    /*000080d8*/ LDR         IP, [R0, #8] 
    /*000080dc*/ ADD         LR, R0, #4 
    /*000080e0*/ STR         R1, [R0, #8] 
    /*000080e4*/ MOV         R0, R2 
    /*000080e8*/ STR         LR, [R1] 
    /*000080ec*/ STR         IP, [R1, #4] 
    /*000080f0*/ STR         R1, [IP] 
    /*000080f4*/ LDR         R2, [R3, #20] 
    /*000080f8*/ ADD         R2, R2, #1 
    /*000080fc*/ STR         R2, [R3, #20] 
    /*00008100*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    AUDIO_FB_PutFree_x8104:
    /*00008104*/ MVN         R0, #0 
    /*00008108*/ BX          LR 
FUNC_END AUDIO_FB_PutFree


FUNC_BEGIN AUDIO_FB_PutBusy
    /*0000810c*/ CMP         R1, #0 
    /*00008110*/ CMPNE       R0, #0 
    /*00008114*/ MOVEQ       R2, #1 
    /*00008118*/ MOVNE       R2, #0 
    /*0000811c*/ BEQ         AUDIO_FB_PutBusy_x8154
    /*00008120*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00008124*/ MOV         R3, R0 
    /*00008128*/ LDR         IP, [R0, #16] 
    /*0000812c*/ ADD         LR, R0, #12 
    /*00008130*/ STR         R1, [R0, #16] 
    /*00008134*/ MOV         R0, R2 
    /*00008138*/ STR         LR, [R1] 
    /*0000813c*/ STR         IP, [R1, #4] 
    /*00008140*/ STR         R1, [IP] 
    /*00008144*/ LDR         R2, [R3, #24] 
    /*00008148*/ ADD         R2, R2, #1 
    /*0000814c*/ STR         R2, [R3, #24] 
    /*00008150*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    AUDIO_FB_PutBusy_x8154:
    /*00008154*/ MVN         R0, #0 
    /*00008158*/ BX          LR 
FUNC_END AUDIO_FB_PutBusy


FUNC_BEGIN AUDIO_FB_QueryFreeNum
    /*0000815c*/ CMP         R0, #0 
    /*00008160*/ LDRNE       R0, [R0, #20] 
    /*00008164*/ MVNEQ       R0, #0 
    /*00008168*/ BX          LR 
FUNC_END AUDIO_FB_QueryFreeNum


FUNC_BEGIN AUDIO_FB_QueryBusyNum
    /*0000816c*/ CMP         R0, #0 
    /*00008170*/ LDRNE       R0, [R0, #24] 
    /*00008174*/ MVNEQ       R0, #0 
    /*00008178*/ BX          LR 
FUNC_END AUDIO_FB_QueryBusyNum


FUNC_BEGIN AI_DRV_ISR
    /*0000817c*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00008180*/ MOV         R4, R0 
    /*00008184*/ SUB         SP, SP, #40                 @ 0x28 
    /*00008188*/ MOV         R0, #20 
    /*0000818c*/ BL          0x000000000000818C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00008190*/ SUBS        R5, R0, #0 
    /*00008194*/ BEQ         AI_DRV_ISR_x83a8
    /*00008198*/ LDR         R3, [R5, #40]               @ 0x28 
    /*0000819c*/ ADD         R1, SP, #20 
    /*000081a0*/ MOV         R0, R4 
    /*000081a4*/ BLX         R3 
    /*000081a8*/ LDR         R3, [SP, #20] 
    /*000081ac*/ TST         R3, #8 
    /*000081b0*/ BNE         AI_DRV_ISR_x81dc
    /*000081b4*/ TST         R3, #2 
    /*000081b8*/ BNE         AI_DRV_ISR_x8234
    AI_DRV_ISR_x81bc:
    /*000081bc*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*000081c0*/ MOV         R0, R4 
    /*000081c4*/ BLX         R3 
    /*000081c8*/ LDR         R3, [SP, #20] 
    /*000081cc*/ TST         R3, #1 
    /*000081d0*/ BNE         AI_DRV_ISR_x82e0
    /*000081d4*/ ADD         SP, SP, #40                 @ 0x28 
    /*000081d8*/ POP         {R4, R5, R6, R7, R8, PC} 
    AI_DRV_ISR_x81dc:
    /*000081dc*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*000081e0*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*000081e4*/ MOV         IP, #184                    @ 0xb8 
    /*000081e8*/ STR         R3, [SP, #8] 
    /*000081ec*/ MLA         IP, IP, R4, R2 
    /*000081f0*/ STR         R4, [SP, #4] 
    /*000081f4*/ MOVW        R2, #557                    @ 0x22d 
    /*000081f8*/ STR         R2, [SP] 
    /*000081fc*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x240] 
    /*00008200*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x20f0] 
    /*00008204*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x240] 
    /*00008208*/ MOV         R1, #21 
    /*0000820c*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x20f0] 
    /*00008210*/ MOV         R0, #3 
    /*00008214*/ LDR         LR, [IP, #136]              @ 0x88 
    /*00008218*/ ADD         LR, LR, #1 
    /*0000821c*/ STR         LR, [SP, #12] 
    /*00008220*/ STR         LR, [IP, #136]              @ 0x88 
    /*00008224*/ BL          0x0000000000008224          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008228*/ LDR         R3, [SP, #20] 
    /*0000822c*/ TST         R3, #2 
    /*00008230*/ BEQ         AI_DRV_ISR_x81bc
    AI_DRV_ISR_x8234:
    /*00008234*/ MOV         R7, #184                    @ 0xb8 
    /*00008238*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*0000823c*/ MUL         R7, R7, R4 
    /*00008240*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*00008244*/ MOV         R0, R4 
    /*00008248*/ ADD         R8, R6, R7 
    /*0000824c*/ LDR         R3, [R8, #140]              @ 0x8c 
    /*00008250*/ ADD         R3, R3, #1 
    /*00008254*/ STR         R3, [R8, #140]              @ 0x8c 
    /*00008258*/ LDR         R3, [R5, #16] 
    /*0000825c*/ BLX         R3 
    /*00008260*/ LDR         R3, [R6, R7] 
    /*00008264*/ LDR         R1, [R8, #116]              @ 0x74 
    /*00008268*/ SUBS        R2, R3, R0 
    /*0000826c*/ BNE         AI_DRV_ISR_x8398
    AI_DRV_ISR_x8270:
    /*00008270*/ ADD         R1, R0, R1 
    AI_DRV_ISR_x8274:
    /*00008274*/ CMP         R1, R3 
    /*00008278*/ MOV         R0, R4 
    /*0000827c*/ BICNE       R1, R1, #15 
    /*00008280*/ LDR         R3, [R5, #12] 
    /*00008284*/ MOVEQ       R1, #0 
    /*00008288*/ BLX         R3 
    /*0000828c*/ LDR         R3, [R5, #16] 
    /*00008290*/ MOV         R0, R4 
    /*00008294*/ BLX         R3 
    /*00008298*/ MOV         R3, #184                    @ 0xb8 
    /*0000829c*/ MLA         R6, R3, R4, R6 
    /*000082a0*/ LDR         R2, [SP, #20] 
    /*000082a4*/ STR         R4, [SP, #4] 
    /*000082a8*/ MOVW        R1, #585                    @ 0x249 
    /*000082ac*/ STR         R1, [SP] 
    /*000082b0*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x240] 
    /*000082b4*/ STR         R2, [SP, #8] 
    /*000082b8*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x240] 
    /*000082bc*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x213c] 
    /*000082c0*/ MOV         R1, #21 
    /*000082c4*/ LDR         IP, [R6, #140]              @ 0x8c 
    /*000082c8*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x213c] 
    /*000082cc*/ STR         IP, [SP, #12] 
    /*000082d0*/ STR         R0, [R6, #20] 
    /*000082d4*/ MOV         R0, #3 
    /*000082d8*/ BL          0x00000000000082D8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000082dc*/ B           AI_DRV_ISR_x81bc
    AI_DRV_ISR_x82e0:
    /*000082e0*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*000082e4*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*000082e8*/ MOV         R5, #184                    @ 0xb8 
    /*000082ec*/ MOV         R0, #2 
    /*000082f0*/ MLA         R5, R5, R4, R6 
    /*000082f4*/ LDR         R3, [R5, #132]              @ 0x84 
    /*000082f8*/ ADD         R3, R3, #1 
    /*000082fc*/ STR         R3, [R5, #132]              @ 0x84 
    /*00008300*/ BL          0x0000000000008300          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00008304*/ LDR         R3, [R0] 
    /*00008308*/ BLX         R3 
    /*0000830c*/ LDR         R2, [R5, #132]              @ 0x84 
    /*00008310*/ LDR         R3, [R5, #80]               @ 0x50 
    /*00008314*/ CMP         R2, #1 
    /*00008318*/ STR         R0, [R5, #80]               @ 0x50 
    /*0000831c*/ SUB         R0, R0, R3 
    /*00008320*/ STR         R1, [R5, #84]               @ 0x54 
    /*00008324*/ STR         R0, [R5, #88]               @ 0x58 
    /*00008328*/ BEQ         AI_DRV_ISR_x8338
    /*0000832c*/ LDR         R3, [R5, #92]               @ 0x5c 
    /*00008330*/ CMP         R0, R3 
    /*00008334*/ STRHI       R0, [R5, #92]               @ 0x5c 
    AI_DRV_ISR_x8338:
    /*00008338*/ ADD         R0, SP, #24 
    /*0000833c*/ BL          0x000000000000833C          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00008340*/ LDR         R3, [R6, #184]              @ 0xb8 
    /*00008344*/ MOV         R0, R4 
    /*00008348*/ BLX         R3 
    /*0000834c*/ ADD         R0, SP, #32 
    /*00008350*/ BL          0x0000000000008350          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00008354*/ MOV         R3, #184                    @ 0xb8 
    /*00008358*/ MLA         R4, R3, R4, R6 
    /*0000835c*/ LDR         R0, [SP, #28] 
    /*00008360*/ LDR         R3, [SP, #32] 
    /*00008364*/ MOVW        R1, #16960                  @ 0x4240 
    /*00008368*/ LDR         IP, [SP, #24] 
    /*0000836c*/ MOVT        R1, #15 
    /*00008370*/ LDR         R2, [SP, #36]               @ 0x24 
    /*00008374*/ SUB         R3, R3, IP 
    /*00008378*/ SUB         R2, R2, R0 
    /*0000837c*/ LDR         R0, [R4, #100]              @ 0x64 
    /*00008380*/ MLA         R3, R1, R3, R2 
    /*00008384*/ CMP         R3, R0 
    /*00008388*/ STR         R3, [R4, #96]               @ 0x60 
    /*0000838c*/ STRHI       R3, [R4, #100]              @ 0x64 
    /*00008390*/ ADD         SP, SP, #40                 @ 0x28 
    /*00008394*/ POP         {R4, R5, R6, R7, R8, PC} 
    AI_DRV_ISR_x8398:
    /*00008398*/ CMP         R2, R1 
    /*0000839c*/ SUBCC       R1, R1, R2 
    /*000083a0*/ BCC         AI_DRV_ISR_x8274
    /*000083a4*/ B           AI_DRV_ISR_x8270
    AI_DRV_ISR_x83a8:
    /*000083a8*/ MOV         R2, #548                    @ 0x224 
    /*000083ac*/ MOV         R1, #21 
    /*000083b0*/ STR         R2, [SP] 
    /*000083b4*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x240] 
    /*000083b8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x20a8] 
    /*000083bc*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x240] 
    /*000083c0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x20a8] 
    /*000083c4*/ BL          0x00000000000083C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000083c8*/ ADD         SP, SP, #40                 @ 0x28 
    /*000083cc*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END AI_DRV_ISR


FUNC_BEGIN AI_DRV_DmaBufInfoInit
    /*000083d0*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*000083d4*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*000083d8*/ MOV         IP, #184                    @ 0xb8 
    /*000083dc*/ MLA         IP, IP, R0, R2 
    /*000083e0*/ PUSH        {R4, R5, LR} 
    /*000083e4*/ SUB         SP, SP, #12 
    /*000083e8*/ LDR         R4, [IP, #40]               @ 0x28 
    /*000083ec*/ LDR         LR, [IP, #64]               @ 0x40 
    /*000083f0*/ CMP         R4, #0 
    /*000083f4*/ BNE         AI_DRV_DmaBufInfoInit_x8408
    /*000083f8*/ LDR         R3, [IP, #52]               @ 0x34 
    /*000083fc*/ CMP         R3, #1 
    /*00008400*/ MOVEQ       R3, #16 
    /*00008404*/ BEQ         AI_DRV_DmaBufInfoInit_x8410
    AI_DRV_DmaBufInfoInit_x8408:
    /*00008408*/ MOV         R3, #8 
    /*0000840c*/ LSL         R3, R3, R4 
    AI_DRV_DmaBufInfoInit_x8410:
    /*00008410*/ MUL         R3, LR, R3 
    /*00008414*/ CMP         R3, #0 
    /*00008418*/ ADD         IP, R3, #7 
    /*0000841c*/ MOVLT       R3, IP 
    /*00008420*/ CMP         LR, #0 
    /*00008424*/ ASR         R3, R3, #3 
    /*00008428*/ BEQ         AI_DRV_DmaBufInfoInit_x8458
    /*0000842c*/ MOV         IP, #184                    @ 0xb8 
    /*00008430*/ CMP         LR, #1 
    /*00008434*/ MLA         IP, IP, R0, R2 
    /*00008438*/ MOV         R4, #0 
    /*0000843c*/ STR         R4, [IP, #120]              @ 0x78 
    /*00008440*/ BLS         AI_DRV_DmaBufInfoInit_x8458
    /*00008444*/ CMP         LR, #2 
    /*00008448*/ MOV         LR, #1 
    /*0000844c*/ STR         LR, [IP, #124]              @ 0x7c 
    /*00008450*/ MOVNE       LR, #2 
    /*00008454*/ STRNE       LR, [IP, #128]              @ 0x80 
    AI_DRV_DmaBufInfoInit_x8458:
    /*00008458*/ MOV         IP, #184                    @ 0xb8 
    /*0000845c*/ CMP         R3, #64                     @ 0x40 
    /*00008460*/ MLA         IP, IP, R0, R2 
    /*00008464*/ STR         R3, [IP, #104]              @ 0x68 
    /*00008468*/ BEQ         AI_DRV_DmaBufInfoInit_x84c8
    /*0000846c*/ BIC         IP, R3, #16 
    /*00008470*/ CMP         IP, #32 
    /*00008474*/ BEQ         AI_DRV_DmaBufInfoInit_x8518
    /*00008478*/ CMP         R3, #16 
    /*0000847c*/ BEQ         AI_DRV_DmaBufInfoInit_x8528
    /*00008480*/ CMP         R3, #8 
    /*00008484*/ BEQ         AI_DRV_DmaBufInfoInit_x8520
    /*00008488*/ CMP         R3, #4 
    /*0000848c*/ BEQ         AI_DRV_DmaBufInfoInit_x8530
    /*00008490*/ CMP         R3, #2 
    /*00008494*/ BEQ         AI_DRV_DmaBufInfoInit_x8538
    /*00008498*/ STR         R3, [SP, #4] 
    /*0000849c*/ MOV         R1, #21 
    /*000084a0*/ MOV         R3, #74                     @ 0x4a 
    /*000084a4*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2188] 
    /*000084a8*/ STR         R3, [SP] 
    /*000084ac*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2188] 
    /*000084b0*/ LDR         R3, [PC, #136]              @ 0x0000000000008540 
    /*000084b4*/ MOV         R0, #3 
    /*000084b8*/ BL          0x00000000000084B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000084bc*/ MVN         R0, #0 
    /*000084c0*/ ADD         SP, SP, #12 
    /*000084c4*/ POP         {R4, R5, PC} 
    AI_DRV_DmaBufInfoInit_x84c8:
    /*000084c8*/ MOV         R5, #6 
    AI_DRV_DmaBufInfoInit_x84cc:
    /*000084cc*/ MOV         IP, #184                    @ 0xb8 
    /*000084d0*/ MOV         R4, #0 
    /*000084d4*/ MUL         R0, IP, R0 
    /*000084d8*/ ADD         LR, R2, R0 
    /*000084dc*/ LDR         IP, [LR, #60]               @ 0x3c 
    /*000084e0*/ STR         R5, [LR, #108]              @ 0x6c 
    /*000084e4*/ STR         R4, [LR, #20] 
    /*000084e8*/ MUL         R3, IP, R3 
    /*000084ec*/ STR         R4, [LR, #24] 
    /*000084f0*/ ADD         IP, R3, R3, LSL #1 
    /*000084f4*/ STR         R3, [LR, #116]              @ 0x74 
    /*000084f8*/ TST         IP, #127                    @ 0x7f 
    /*000084fc*/ BICNE       IP, IP, #127                @ 0x7f 
    /*00008500*/ ADDNE       IP, IP, #128                @ 0x80 
    /*00008504*/ STR         IP, [R2, R0] 
    /*00008508*/ MOV         R0, #0 
    /*0000850c*/ STR         IP, [R1] 
    /*00008510*/ ADD         SP, SP, #12 
    /*00008514*/ POP         {R4, R5, PC} 
    AI_DRV_DmaBufInfoInit_x8518:
    /*00008518*/ MOV         R5, #5 
    /*0000851c*/ B           AI_DRV_DmaBufInfoInit_x84cc
    AI_DRV_DmaBufInfoInit_x8520:
    /*00008520*/ MOV         R5, #3 
    /*00008524*/ B           AI_DRV_DmaBufInfoInit_x84cc
    AI_DRV_DmaBufInfoInit_x8528:
    /*00008528*/ MOV         R5, #4 
    /*0000852c*/ B           AI_DRV_DmaBufInfoInit_x84cc
    AI_DRV_DmaBufInfoInit_x8530:
    /*00008530*/ MOV         R5, #2 
    /*00008534*/ B           AI_DRV_DmaBufInfoInit_x84cc
    AI_DRV_DmaBufInfoInit_x8538:
    /*00008538*/ MOV         R5, #1 
    /*0000853c*/ B           AI_DRV_DmaBufInfoInit_x84cc
    /*00008540*/ ANDEQ       R0, R0, IP, ASR #4 
FUNC_END AI_DRV_DmaBufInfoInit


FUNC_BEGIN AI_DRV_SetDmaBufAddr
    /*00008544*/ MOVW        IP, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*00008548*/ MOVT        IP, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*0000854c*/ MOV         R1, #184                    @ 0xb8 
    /*00008550*/ MLA         R1, R1, R0, IP 
    /*00008554*/ MOV         R0, #0 
    /*00008558*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000855c*/ LDR         LR, [SP, #4] 
    /*00008560*/ STRD        R2, [R1, #8] 
    /*00008564*/ STR         LR, [R1, #16] 
    /*00008568*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END AI_DRV_SetDmaBufAddr


FUNC_BEGIN AI_DRV_ClrDmaBufAddr
    /*0000856c*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*00008570*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*00008574*/ MOV         R3, #184                    @ 0xb8 
    /*00008578*/ MOV         R1, #0 
    /*0000857c*/ MLA         R3, R3, R0, R2 
    /*00008580*/ MOV         R0, R1 
    /*00008584*/ PUSH        {R4, R5} 
    /*00008588*/ MOV         R4, #0 
    /*0000858c*/ MOV         R5, #0 
    /*00008590*/ STR         R1, [R3, #16] 
    /*00008594*/ STRD        R4, [R3, #8] 
    /*00008598*/ POP         {R4, R5} 
    /*0000859c*/ BX          LR 
FUNC_END AI_DRV_ClrDmaBufAddr


FUNC_BEGIN AI_DRV_SetDmaISR
    /*000085a0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*000085a4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*000085a8*/ STR         R0, [R3, #184]              @ 0xb8 
    /*000085ac*/ BX          LR 
FUNC_END AI_DRV_SetDmaISR


FUNC_BEGIN AI_DRV_SetDevAttr
    /*000085b0*/ PUSH        {R4, R5, R6, LR} 
    /*000085b4*/ MOV         R6, R0 
    /*000085b8*/ SUB         SP, SP, #8 
    /*000085bc*/ MOV         R0, #20 
    /*000085c0*/ MOV         R5, R1 
    /*000085c4*/ BL          0x00000000000085C4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000085c8*/ SUBS        R3, R0, #0 
    /*000085cc*/ BEQ         AI_DRV_SetDevAttr_x8634
    /*000085d0*/ LDR         R3, [R3] 
    /*000085d4*/ MOV         R1, R5 
    /*000085d8*/ MOV         R0, R6 
    /*000085dc*/ BLX         R3 
    /*000085e0*/ SUBS        R4, R0, #0 
    /*000085e4*/ BNE         AI_DRV_SetDevAttr_x8628
    /*000085e8*/ MOV         IP, #184                    @ 0xb8 
    /*000085ec*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*000085f0*/ MUL         IP, IP, R6 
    /*000085f4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*000085f8*/ MOV         R1, R5 
    /*000085fc*/ MOV         R2, #40                     @ 0x28 
    /*00008600*/ ADD         R3, R0, IP 
    /*00008604*/ ADD         IP, IP, #36                 @ 0x24 
    /*00008608*/ ADD         R0, R0, IP 
    /*0000860c*/ STR         R4, [R3, #144]              @ 0x90 
    /*00008610*/ STR         R4, [R3, #152]              @ 0x98 
    /*00008614*/ STR         R4, [R3, #156]              @ 0x9c 
    /*00008618*/ STR         R4, [R3, #160]              @ 0xa0 
    /*0000861c*/ STR         R4, [R3, #164]              @ 0xa4 
    /*00008620*/ STR         R4, [R3, #168]              @ 0xa8 
    /*00008624*/ BL          0x0000000000008624          @ osal_memcpy [R_ARM_CALL: +0x0] 
    AI_DRV_SetDevAttr_x8628:
    /*00008628*/ MOV         R0, R4 
    /*0000862c*/ ADD         SP, SP, #8 
    /*00008630*/ POP         {R4, R5, R6, PC} 
    AI_DRV_SetDevAttr_x8634:
    /*00008634*/ MOV         R3, #148                    @ 0x94 
    /*00008638*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21dc] 
    /*0000863c*/ STR         R3, [SP] 
    /*00008640*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21dc] 
    /*00008644*/ LDR         R3, [PC, #16]               @ 0x000000000000865c 
    /*00008648*/ MOV         R1, #21 
    /*0000864c*/ MOVW        R4, #32784                  @ 0x8010 
    /*00008650*/ BL          0x0000000000008650          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008654*/ MOVT        R4, #40981                  @ 0xa015 
    /*00008658*/ B           AI_DRV_SetDevAttr_x8628
    /*0000865c*/ ANDEQ       R0, R0, R4, ROR #4 
FUNC_END AI_DRV_SetDevAttr


FUNC_BEGIN AI_DRV_ClrDevAttr
    /*00008660*/ PUSH        {R4, LR} 
    /*00008664*/ MOV         R4, R0 
    /*00008668*/ SUB         SP, SP, #8 
    /*0000866c*/ MOV         R0, #20 
    /*00008670*/ BL          0x0000000000008670          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00008674*/ SUBS        R3, R0, #0 
    /*00008678*/ BEQ         AI_DRV_ClrDevAttr_x8690
    /*0000867c*/ LDR         R3, [R3, #52]               @ 0x34 
    /*00008680*/ MOV         R0, R4 
    /*00008684*/ ADD         SP, SP, #8 
    /*00008688*/ POP         {R4, LR} 
    /*0000868c*/ BX          R3 
    AI_DRV_ClrDevAttr_x8690:
    /*00008690*/ MOV         R3, #177                    @ 0xb1 
    /*00008694*/ MOV         R1, #21 
    /*00008698*/ STR         R3, [SP] 
    /*0000869c*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2224] 
    /*000086a0*/ LDR         R3, [PC, #20]               @ 0x00000000000086bc 
    /*000086a4*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2224] 
    /*000086a8*/ BL          0x00000000000086A8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000086ac*/ MOVW        R0, #32784                  @ 0x8010 
    /*000086b0*/ MOVT        R0, #40981                  @ 0xa015 
    /*000086b4*/ ADD         SP, SP, #8 
    /*000086b8*/ POP         {R4, PC} 
    /*000086bc*/ ANDEQ       R0, R0, R8, ROR R2 
FUNC_END AI_DRV_ClrDevAttr


FUNC_BEGIN AI_DRV_EnableDev
    /*000086c0*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000086c4*/ MOV         R4, R0 
    /*000086c8*/ SUB         SP, SP, #8 
    /*000086cc*/ MOV         R0, #20 
    /*000086d0*/ BL          0x00000000000086D0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000086d4*/ SUBS        R5, R0, #0 
    /*000086d8*/ BEQ         AI_DRV_EnableDev_x8760
    /*000086dc*/ MOV         R6, #184                    @ 0xb8 
    /*000086e0*/ MOVW        R7, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*000086e4*/ MUL         R6, R6, R4 
    /*000086e8*/ MOVT        R7, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*000086ec*/ LDR         R1, [R5, #28] 
    /*000086f0*/ MOV         R0, R4 
    /*000086f4*/ ADD         R8, R7, R6 
    /*000086f8*/ LDRD        R2, [R8, #8] 
    /*000086fc*/ BLX         R1 
    /*00008700*/ LDR         R1, [R7, R6] 
    /*00008704*/ LDR         R3, [R5, #32] 
    /*00008708*/ MOV         R0, R4 
    /*0000870c*/ BLX         R3 
    /*00008710*/ LDR         R3, [R5, #12] 
    /*00008714*/ MOV         R1, #0 
    /*00008718*/ MOV         R0, R4 
    /*0000871c*/ BLX         R3 
    /*00008720*/ LDR         R3, [R5, #20] 
    /*00008724*/ MOV         R1, #0 
    /*00008728*/ MOV         R0, R4 
    /*0000872c*/ BLX         R3 
    /*00008730*/ LDR         R1, [R8, #116]              @ 0x74 
    /*00008734*/ LDR         R3, [R5, #36]               @ 0x24 
    /*00008738*/ MOV         R0, R4 
    /*0000873c*/ BLX         R3 
    /*00008740*/ LDR         R3, [R5, #4] 
    /*00008744*/ MOV         R0, R4 
    /*00008748*/ BLX         R3 
    /*0000874c*/ MOV         R0, #0 
    /*00008750*/ MOV         R3, #1 
    /*00008754*/ STR         R3, [R8, #76]               @ 0x4c 
    AI_DRV_EnableDev_x8758:
    /*00008758*/ ADD         SP, SP, #8 
    /*0000875c*/ POP         {R4, R5, R6, R7, R8, PC} 
    AI_DRV_EnableDev_x8760:
    /*00008760*/ MOV         R3, #200                    @ 0xc8 
    /*00008764*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2268] 
    /*00008768*/ STR         R3, [SP] 
    /*0000876c*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2268] 
    /*00008770*/ LDR         R3, [PC, #16]               @ 0x0000000000008788 
    /*00008774*/ MOV         R1, #21 
    /*00008778*/ BL          0x0000000000008778          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000877c*/ MOVW        R0, #32784                  @ 0x8010 
    /*00008780*/ MOVT        R0, #40981                  @ 0xa015 
    /*00008784*/ B           AI_DRV_EnableDev_x8758
    /*00008788*/ ANDEQ       R0, R0, IP, LSL #5 
FUNC_END AI_DRV_EnableDev


FUNC_BEGIN AI_DRV_DisableDev
    /*0000878c*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*00008790*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*00008794*/ MOV         R3, #184                    @ 0xb8 
    /*00008798*/ MLA         R3, R3, R0, R2 
    /*0000879c*/ MOV         R2, #0 
    /*000087a0*/ PUSH        {R4, LR} 
    /*000087a4*/ MOV         R4, R0 
    /*000087a8*/ SUB         SP, SP, #8 
    /*000087ac*/ MOV         R0, #20 
    /*000087b0*/ STR         R2, [R3, #76]               @ 0x4c 
    /*000087b4*/ STR         R2, [R3, #132]              @ 0x84 
    /*000087b8*/ STR         R2, [R3, #136]              @ 0x88 
    /*000087bc*/ STR         R2, [R3, #140]              @ 0x8c 
    /*000087c0*/ STR         R2, [R3, #88]               @ 0x58 
    /*000087c4*/ STR         R2, [R3, #92]               @ 0x5c 
    /*000087c8*/ BL          0x00000000000087C8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000087cc*/ SUBS        R3, R0, #0 
    /*000087d0*/ BEQ         AI_DRV_DisableDev_x87e8
    /*000087d4*/ LDR         R3, [R3, #8] 
    /*000087d8*/ MOV         R0, R4 
    /*000087dc*/ ADD         SP, SP, #8 
    /*000087e0*/ POP         {R4, LR} 
    /*000087e4*/ BX          R3 
    AI_DRV_DisableDev_x87e8:
    /*000087e8*/ MOV         R3, #234                    @ 0xea 
    /*000087ec*/ MOV         R1, #21 
    /*000087f0*/ STR         R3, [SP] 
    /*000087f4*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x22b4] 
    /*000087f8*/ LDR         R3, [PC, #12]               @ 0x000000000000880c 
    /*000087fc*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x22b4] 
    /*00008800*/ BL          0x0000000000008800          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008804*/ ADD         SP, SP, #8 
    /*00008808*/ POP         {R4, PC} 
    /*0000880c*/ ANDEQ       R0, R0, R0, LSR #5 
FUNC_END AI_DRV_DisableDev


FUNC_BEGIN AI_DRV_GetChnData
    /*00008810*/ MOV         R3, #184                    @ 0xb8 
    /*00008814*/ MUL         R3, R3, R0 
    /*00008818*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000881c*/ SUB         SP, SP, #164                @ 0xa4 
    /*00008820*/ MOV         R4, R1 
    /*00008824*/ STR         R0, [SP, #120]              @ 0x78 
    /*00008828*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*0000882c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*00008830*/ STR         R2, [SP, #44]               @ 0x2c 
    /*00008834*/ ADD         R1, R0, R3 
    /*00008838*/ STR         R0, [SP, #96]               @ 0x60 
    /*0000883c*/ LDR         R2, [R0, R3] 
    /*00008840*/ LDR         R3, [R1, #20] 
    /*00008844*/ LDR         R0, [SP, #96]               @ 0x60 
    /*00008848*/ CMP         R3, R2 
    /*0000884c*/ MOVEQ       R3, #0 
    /*00008850*/ STREQ       R3, [R1, #20] 
    /*00008854*/ LDR         R1, [SP, #120]              @ 0x78 
    /*00008858*/ MOV         R3, #184                    @ 0xb8 
    /*0000885c*/ MLA         R3, R3, R1, R0 
    /*00008860*/ LDR         R1, [R3, #16] 
    /*00008864*/ CMP         R1, #0 
    /*00008868*/ STR         R1, [SP, #56]               @ 0x38 
    /*0000886c*/ BEQ         AI_DRV_GetChnData_x92f8
    /*00008870*/ LDR         IP, [SP, #56]               @ 0x38 
    /*00008874*/ LDR         R0, [R3, #20] 
    /*00008878*/ ADD         R1, IP, R2 
    /*0000887c*/ ADD         IP, IP, R0 
    /*00008880*/ STR         IP, [SP, #52]               @ 0x34 
    /*00008884*/ CMP         IP, R1 
    /*00008888*/ BHI         AI_DRV_GetChnData_x92cc
    /*0000888c*/ LDR         R1, [R3, #40]               @ 0x28 
    /*00008890*/ CMP         R1, #0 
    /*00008894*/ BEQ         AI_DRV_GetChnData_x928c
    AI_DRV_GetChnData_x8898:
    /*00008898*/ MOV         R3, #8 
    /*0000889c*/ LSL         R3, R3, R1 
    AI_DRV_GetChnData_x88a0:
    /*000088a0*/ LDR         R1, [SP, #120]              @ 0x78 
    /*000088a4*/ MOV         R6, #184                    @ 0xb8 
    /*000088a8*/ SUB         R2, R2, R0 
    /*000088ac*/ STR         R2, [SP, #156]              @ 0x9c 
    /*000088b0*/ MUL         R6, R6, R1 
    /*000088b4*/ LSL         R0, R2, #3 
    /*000088b8*/ LDR         R2, [SP, #96]               @ 0x60 
    /*000088bc*/ ADD         R8, R2, R6 
    /*000088c0*/ LDR         R1, [R8, #64]               @ 0x40 
    /*000088c4*/ MUL         R1, R1, R3 
    /*000088c8*/ BL          0x00000000000088C8          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000088cc*/ LDR         R3, [R8, #60]               @ 0x3c 
    /*000088d0*/ MOV         R2, R3 
    /*000088d4*/ STR         R3, [SP, #12] 
    /*000088d8*/ LDR         R3, [R8, #108]              @ 0x6c 
    /*000088dc*/ CMP         R0, R2 
    /*000088e0*/ MOVCC       R2, R0 
    /*000088e4*/ STR         R2, [SP, #8] 
    /*000088e8*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*000088ec*/ CMP         R2, #0 
    /*000088f0*/ BEQ         AI_DRV_GetChnData_x8bd4
    /*000088f4*/ LDR         LR, [SP, #8] 
    /*000088f8*/ ADD         R6, R6, #120                @ 0x78 
    /*000088fc*/ LDR         R1, [SP, #12] 
    /*00008900*/ STR         R4, [SP, #16] 
    /*00008904*/ SUB         R2, R1, LR 
    /*00008908*/ AND         R1, LR, #3 
    /*0000890c*/ AND         R2, R2, #3 
    /*00008910*/ STR         R2, [SP, #88]               @ 0x58 
    /*00008914*/ MOV         R0, R1 
    /*00008918*/ STR         R1, [SP, #32] 
    /*0000891c*/ MOV         R9, R2 
    /*00008920*/ ADD         R2, R1, #3 
    /*00008924*/ ADD         R5, LR, R9 
    /*00008928*/ ADD         R1, R9, #3 
    /*0000892c*/ ADD         R7, R9, #1 
    /*00008930*/ LSL         R2, R2, R3 
    /*00008934*/ MOV         SL, R5 
    /*00008938*/ STR         R2, [SP, #140]              @ 0x8c 
    /*0000893c*/ STR         R5, [SP, #80]               @ 0x50 
    /*00008940*/ MOV         R5, R0 
    /*00008944*/ LDR         R2, [SP, #96]               @ 0x60 
    /*00008948*/ LSL         FP, R1, R3 
    /*0000894c*/ ADD         LR, R9, #2 
    /*00008950*/ LSL         R1, R5, R3 
    /*00008954*/ LSL         R9, R9, R3 
    /*00008958*/ STR         R1, [SP, #64]               @ 0x40 
    /*0000895c*/ ADD         R6, R2, R6 
    /*00008960*/ MOV         R1, #1 
    /*00008964*/ ADD         IP, R0, #1 
    /*00008968*/ STR         R9, [SP, #104]              @ 0x68 
    /*0000896c*/ ADD         R0, R0, #2 
    /*00008970*/ SUB         R9, FP, R9 
    /*00008974*/ STR         R6, [SP, #20] 
    /*00008978*/ STR         R9, [SP, #132]              @ 0x84 
    /*0000897c*/ LSL         R9, SL, #2 
    /*00008980*/ LDR         R6, [SP, #140]              @ 0x8c 
    /*00008984*/ STR         R9, [SP, #116]              @ 0x74 
    /*00008988*/ LSL         R9, SL, R1 
    /*0000898c*/ STR         R9, [SP, #136]              @ 0x88 
    /*00008990*/ LSL         R9, R7, R3 
    /*00008994*/ LDR         R7, [SP, #64]               @ 0x40 
    /*00008998*/ STR         R9, [SP, #108]              @ 0x6c 
    /*0000899c*/ LSL         R9, LR, R3 
    /*000089a0*/ SUB         R2, R6, R7 
    /*000089a4*/ STR         R2, [SP, #100]              @ 0x64 
    /*000089a8*/ LSL         R2, IP, R3 
    /*000089ac*/ STR         R2, [SP, #68]               @ 0x44 
    /*000089b0*/ LSL         R2, R0, R3 
    /*000089b4*/ STR         R2, [SP, #72]               @ 0x48 
    /*000089b8*/ LSL         R2, R5, #2 
    /*000089bc*/ STR         R2, [SP, #84]               @ 0x54 
    /*000089c0*/ LSL         R2, R5, R1 
    /*000089c4*/ STR         R2, [SP, #124]              @ 0x7c 
    /*000089c8*/ MOV         R2, #4 
    /*000089cc*/ LSL         R5, R1, R3 
    /*000089d0*/ LSL         SL, R2, R3 
    /*000089d4*/ LDR         LR, [SP, #8] 
    /*000089d8*/ STR         FP, [SP, #152]              @ 0x98 
    /*000089dc*/ MOVW        FP, #0                      @ g_bSoftDifferential [R_ARM_MOVW_ABS_NC: +0x0] 
    /*000089e0*/ MOVT        FP, #0                      @ g_bSoftDifferential [R_ARM_MOVT_ABS: +0x0] 
    /*000089e4*/ MOV         R4, SL 
    /*000089e8*/ MOV         SL, R5 
    /*000089ec*/ LSL         R3, LR, #2 
    /*000089f0*/ STR         R9, [SP, #112]              @ 0x70 
    /*000089f4*/ STR         R3, [SP, #144]              @ 0x90 
    /*000089f8*/ LSL         R3, LR, R1 
    /*000089fc*/ STR         R8, [SP, #60]               @ 0x3c 
    /*00008a00*/ STR         R3, [SP, #148]              @ 0x94 
    /*00008a04*/ MOV         R3, #0 
    /*00008a08*/ STR         R3, [SP, #24] 
    AI_DRV_GetChnData_x8a0c:
    /*00008a0c*/ LDR         R2, [SP, #16] 
    /*00008a10*/ LDR         R3, [R2, #12] 
    /*00008a14*/ CMP         R3, #0 
    /*00008a18*/ BEQ         AI_DRV_GetChnData_x8ba4
    /*00008a1c*/ LDR         R1, [SP, #60]               @ 0x3c 
    /*00008a20*/ LDR         R0, [SP, #20] 
    /*00008a24*/ LDR         R2, [R2, #8] 
    /*00008a28*/ LDR         R3, [R1, #40]               @ 0x28 
    /*00008a2c*/ LDR         R0, [R0] 
    /*00008a30*/ CMP         R3, #0 
    /*00008a34*/ STR         R2, [SP, #28] 
    /*00008a38*/ STR         R0, [SP, #40]               @ 0x28 
    /*00008a3c*/ BEQ         AI_DRV_GetChnData_x8c74
    /*00008a40*/ CMP         R3, #1 
    /*00008a44*/ BEQ         AI_DRV_GetChnData_x8f90
    /*00008a48*/ CMP         R3, #2 
    /*00008a4c*/ BNE         AI_DRV_GetChnData_x8f60
    /*00008a50*/ LDR         R2, [SP, #40]               @ 0x28 
    /*00008a54*/ LDR         R3, [FP] 
    /*00008a58*/ LDR         R0, [SP, #52]               @ 0x34 
    /*00008a5c*/ LSL         R2, R2, #2 
    /*00008a60*/ CMP         R3, #1 
    /*00008a64*/ ADD         R1, R2, #4 
    /*00008a68*/ STR         R2, [SP, #76]               @ 0x4c 
    /*00008a6c*/ STR         R1, [SP, #128]              @ 0x80 
    /*00008a70*/ ADD         R7, R0, R2 
    /*00008a74*/ ADD         R9, R0, R1 
    /*00008a78*/ BEQ         AI_DRV_GetChnData_x9254
    AI_DRV_GetChnData_x8a7c:
    /*00008a7c*/ LDR         R3, [SP, #32] 
    /*00008a80*/ CMP         R3, #0 
    /*00008a84*/ BEQ         AI_DRV_GetChnData_x8acc
    /*00008a88*/ LDR         R3, [SP, #28] 
    /*00008a8c*/ MOV         R0, R7 
    /*00008a90*/ LDR         R2, [SP, #84]               @ 0x54 
    /*00008a94*/ MOV         IP, R9 
    /*00008a98*/ ADD         LR, R3, R2 
    AI_DRV_GetChnData_x8a9c:
    /*00008a9c*/ LDR         R2, [R0] 
    /*00008aa0*/ ADD         R3, R3, #4 
    /*00008aa4*/ ADD         R0, R0, SL 
    /*00008aa8*/ STR         R2, [R3, #-4] 
    /*00008aac*/ LDR         R1, [FP] 
    /*00008ab0*/ CMP         R1, #1 
    /*00008ab4*/ LDREQ       R1, [IP] 
    /*00008ab8*/ ADD         IP, IP, SL 
    /*00008abc*/ SUBEQ       R2, R2, R1 
    /*00008ac0*/ STREQ       R2, [R3, #-4] 
    /*00008ac4*/ CMP         R3, LR 
    /*00008ac8*/ BNE         AI_DRV_GetChnData_x8a9c
    AI_DRV_GetChnData_x8acc:
    /*00008acc*/ LDR         R3, [SP, #8] 
    /*00008ad0*/ LDR         R2, [SP, #32] 
    /*00008ad4*/ CMP         R3, R2 
    /*00008ad8*/ BLS         AI_DRV_GetChnData_x8df4
    /*00008adc*/ LDR         R2, [SP, #84]               @ 0x54 
    /*00008ae0*/ LDR         R3, [SP, #28] 
    /*00008ae4*/ STR         SL, [SP, #92]               @ 0x5c 
    /*00008ae8*/ ADD         R3, R3, R2 
    /*00008aec*/ LDR         R2, [SP, #100]              @ 0x64 
    /*00008af0*/ LDR         R8, [SP, #32] 
    /*00008af4*/ ADD         R1, R7, R2 
    /*00008af8*/ ADD         R2, R9, R2 
    /*00008afc*/ STR         R2, [SP, #48]               @ 0x30 
    /*00008b00*/ ADD         R2, SP, #64                 @ 0x40 
    /*00008b04*/ STR         R9, [SP, #36]               @ 0x24 
    /*00008b08*/ MOV         SL, R1 
    /*00008b0c*/ LDM         R2, {R2, R5, R6} 
    /*00008b10*/ B           AI_DRV_GetChnData_x8b30
    AI_DRV_GetChnData_x8b14:
    /*00008b14*/ LDR         R1, [SP, #8] 
    /*00008b18*/ ADD         R3, R3, #16 
    /*00008b1c*/ ADD         R2, R2, R4 
    /*00008b20*/ ADD         R5, R5, R4 
    /*00008b24*/ CMP         R1, R8 
    /*00008b28*/ ADD         R6, R6, R4 
    /*00008b2c*/ BLS         AI_DRV_GetChnData_x8df0
    AI_DRV_GetChnData_x8b30:
    /*00008b30*/ LDR         LR, [R7, R2] 
    /*00008b34*/ ADD         R8, R8, #4 
    /*00008b38*/ STR         LR, [R3] 
    /*00008b3c*/ LDR         IP, [R7, R5] 
    /*00008b40*/ STR         IP, [R3, #4] 
    /*00008b44*/ LDR         R0, [R7, R6] 
    /*00008b48*/ STR         R0, [R3, #8] 
    /*00008b4c*/ LDR         R1, [SL, R2] 
    /*00008b50*/ STR         R1, [R3, #12] 
    /*00008b54*/ LDR         R9, [FP] 
    /*00008b58*/ CMP         R9, #1 
    /*00008b5c*/ BNE         AI_DRV_GetChnData_x8b14
    /*00008b60*/ LDR         R9, [SP, #36]               @ 0x24 
    /*00008b64*/ LDR         R9, [R9, R2] 
    /*00008b68*/ SUB         LR, LR, R9 
    /*00008b6c*/ LDR         R9, [SP, #36]               @ 0x24 
    /*00008b70*/ STR         LR, [R3] 
    /*00008b74*/ LDR         LR, [R9, R5] 
    /*00008b78*/ SUB         IP, IP, LR 
    /*00008b7c*/ STR         IP, [R3, #4] 
    /*00008b80*/ LDR         IP, [R9, R6] 
    /*00008b84*/ SUB         R0, R0, IP 
    /*00008b88*/ STR         R0, [R3, #8] 
    /*00008b8c*/ LDR         R0, [SP, #48]               @ 0x30 
    /*00008b90*/ LDR         R0, [R0, R2] 
    /*00008b94*/ SUB         R1, R1, R0 
    /*00008b98*/ STR         R1, [R3, #12] 
    /*00008b9c*/ B           AI_DRV_GetChnData_x8b14
    AI_DRV_GetChnData_x8ba0:
    /*00008ba0*/ LDR         SL, [SP, #40]               @ 0x28 
    AI_DRV_GetChnData_x8ba4:
    /*00008ba4*/ LDR         R2, [SP, #16] 
    /*00008ba8*/ LDR         R3, [SP, #24] 
    /*00008bac*/ ADD         R2, R2, #16 
    /*00008bb0*/ STR         R2, [SP, #16] 
    /*00008bb4*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*00008bb8*/ ADD         R3, R3, #1 
    /*00008bbc*/ STR         R3, [SP, #24] 
    /*00008bc0*/ CMP         R2, R3 
    /*00008bc4*/ LDR         R3, [SP, #20] 
    /*00008bc8*/ ADD         R3, R3, #4 
    /*00008bcc*/ STR         R3, [SP, #20] 
    /*00008bd0*/ BNE         AI_DRV_GetChnData_x8a0c
    AI_DRV_GetChnData_x8bd4:
    /*00008bd4*/ LDR         R3, [SP, #156]              @ 0x9c 
    /*00008bd8*/ CMP         R3, #0 
    /*00008bdc*/ BNE         AI_DRV_GetChnData_x92a0
    /*00008be0*/ LDR         R2, [SP, #120]              @ 0x78 
    /*00008be4*/ MOV         R3, #184                    @ 0xb8 
    /*00008be8*/ LDR         R1, [SP, #96]               @ 0x60 
    /*00008bec*/ MLA         R3, R3, R2, R1 
    /*00008bf0*/ LDR         R3, [R3, #116]              @ 0x74 
    AI_DRV_GetChnData_x8bf4:
    /*00008bf4*/ LDR         R1, [SP, #120]              @ 0x78 
    /*00008bf8*/ MOV         R2, #184                    @ 0xb8 
    /*00008bfc*/ LDR         R0, [SP, #96]               @ 0x60 
    /*00008c00*/ MLA         R2, R2, R1, R0 
    /*00008c04*/ LDR         R1, [R2, #20] 
    /*00008c08*/ ADD         R3, R3, R1 
    /*00008c0c*/ STR         R3, [R2, #20] 
    AI_DRV_GetChnData_x8c10:
    /*00008c10*/ LDR         R1, [SP, #120]              @ 0x78 
    /*00008c14*/ MOV         R2, #184                    @ 0xb8 
    /*00008c18*/ LDR         R0, [SP, #96]               @ 0x60 
    /*00008c1c*/ MUL         R2, R2, R1 
    /*00008c20*/ ADD         R1, R0, R2 
    /*00008c24*/ LDR         R2, [R0, R2] 
    /*00008c28*/ MOV         R0, #20 
    /*00008c2c*/ CMP         R2, R3 
    /*00008c30*/ MOVEQ       R3, #0 
    /*00008c34*/ STREQ       R3, [R1, #20] 
    /*00008c38*/ BL          0x0000000000008C38          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00008c3c*/ SUBS        R2, R0, #0 
    /*00008c40*/ BEQ         AI_DRV_GetChnData_x9324
    /*00008c44*/ LDR         R1, [SP, #120]              @ 0x78 
    /*00008c48*/ MOV         R3, #184                    @ 0xb8 
    /*00008c4c*/ LDR         R0, [SP, #96]               @ 0x60 
    /*00008c50*/ LDR         R2, [R2, #12] 
    /*00008c54*/ MLA         R3, R3, R1, R0 
    /*00008c58*/ MOV         R0, R1 
    /*00008c5c*/ LDR         R1, [R3, #20] 
    /*00008c60*/ BIC         R1, R1, #15 
    /*00008c64*/ BLX         R2 
    /*00008c68*/ MOV         R0, #0 
    AI_DRV_GetChnData_x8c6c:
    /*00008c6c*/ ADD         SP, SP, #164                @ 0xa4 
    /*00008c70*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AI_DRV_GetChnData_x8c74:
    /*00008c74*/ LDR         R3, [R1, #52]               @ 0x34 
    /*00008c78*/ CMP         R3, #0 
    /*00008c7c*/ BNE         AI_DRV_GetChnData_x8f58
    /*00008c80*/ LDR         R1, [SP, #32] 
    /*00008c84*/ LDR         R3, [SP, #52]               @ 0x34 
    /*00008c88*/ CMP         R1, #0 
    /*00008c8c*/ ADD         IP, R3, R0 
    /*00008c90*/ BEQ         AI_DRV_GetChnData_x8cb0
    /*00008c94*/ MOV         R3, R2 
    /*00008c98*/ ADD         R0, R2, R1 
    /*00008c9c*/ MOV         R2, IP 
    AI_DRV_GetChnData_x8ca0:
    /*00008ca0*/ LDRB        R1, [R2], SL 
    /*00008ca4*/ STRB        R1, [R3], #1 
    /*00008ca8*/ CMP         R0, R3 
    /*00008cac*/ BNE         AI_DRV_GetChnData_x8ca0
    AI_DRV_GetChnData_x8cb0:
    /*00008cb0*/ LDR         R3, [SP, #8] 
    /*00008cb4*/ LDR         R2, [SP, #32] 
    /*00008cb8*/ CMP         R3, R2 
    /*00008cbc*/ BLS         AI_DRV_GetChnData_x8d2c
    /*00008cc0*/ LDR         R1, [SP, #64]               @ 0x40 
    /*00008cc4*/ LDR         R3, [SP, #32] 
    /*00008cc8*/ ADD         R5, IP, R1 
    /*00008ccc*/ LDR         R1, [SP, #68]               @ 0x44 
    /*00008cd0*/ LDR         R2, [SP, #28] 
    /*00008cd4*/ ADD         R6, IP, R1 
    /*00008cd8*/ LDR         R1, [SP, #72]               @ 0x48 
    /*00008cdc*/ LDR         R7, [SP, #8] 
    /*00008ce0*/ ADD         R2, R2, R3 
    /*00008ce4*/ ADD         LR, IP, R1 
    /*00008ce8*/ LDR         R1, [SP, #140]              @ 0x8c 
    /*00008cec*/ ADD         IP, IP, R1 
    /*00008cf0*/ MOV         R1, R3 
    /*00008cf4*/ MOV         R3, #0 
    AI_DRV_GetChnData_x8cf8:
    /*00008cf8*/ LDRB        R0, [R5, R3] 
    /*00008cfc*/ ADD         R1, R1, #4 
    /*00008d00*/ CMP         R7, R1 
    /*00008d04*/ ADD         R2, R2, #4 
    /*00008d08*/ STRB        R0, [R2, #-4] 
    /*00008d0c*/ LDRB        R0, [R6, R3] 
    /*00008d10*/ STRB        R0, [R2, #-3] 
    /*00008d14*/ LDRB        R0, [LR, R3] 
    /*00008d18*/ STRB        R0, [R2, #-2] 
    /*00008d1c*/ LDRB        R0, [IP, R3] 
    /*00008d20*/ ADD         R3, R3, R4 
    /*00008d24*/ STRB        R0, [R2, #-1] 
    /*00008d28*/ BHI         AI_DRV_GetChnData_x8cf8
    AI_DRV_GetChnData_x8d2c:
    /*00008d2c*/ LDR         R3, [SP, #8] 
    /*00008d30*/ LDR         R2, [SP, #12] 
    /*00008d34*/ CMP         R3, R2 
    /*00008d38*/ BEQ         AI_DRV_GetChnData_x8ba4
    /*00008d3c*/ LDR         R3, [SP, #88]               @ 0x58 
    /*00008d40*/ LDR         R2, [SP, #40]               @ 0x28 
    /*00008d44*/ CMP         R3, #0 
    /*00008d48*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00008d4c*/ ADD         IP, R3, R2 
    /*00008d50*/ BEQ         AI_DRV_GetChnData_x8d7c
    /*00008d54*/ LDR         R2, [SP, #28] 
    /*00008d58*/ LDR         R1, [SP, #8] 
    /*00008d5c*/ ADD         R3, R2, R1 
    /*00008d60*/ LDR         R1, [SP, #80]               @ 0x50 
    /*00008d64*/ ADD         R0, R2, R1 
    /*00008d68*/ MOV         R2, IP 
    AI_DRV_GetChnData_x8d6c:
    /*00008d6c*/ LDRB        R1, [R2], SL 
    /*00008d70*/ STRB        R1, [R3], #1 
    /*00008d74*/ CMP         R0, R3 
    /*00008d78*/ BNE         AI_DRV_GetChnData_x8d6c
    AI_DRV_GetChnData_x8d7c:
    /*00008d7c*/ LDR         R7, [SP, #12] 
    /*00008d80*/ LDR         R1, [SP, #80]               @ 0x50 
    /*00008d84*/ CMP         R7, R1 
    /*00008d88*/ BLS         AI_DRV_GetChnData_x8ba4
    /*00008d8c*/ LDR         R2, [SP, #28] 
    /*00008d90*/ ADD         R3, R2, R1 
    /*00008d94*/ LDR         R2, [SP, #104]              @ 0x68 
    /*00008d98*/ ADD         R5, IP, R2 
    /*00008d9c*/ LDR         R2, [SP, #108]              @ 0x6c 
    /*00008da0*/ ADD         R6, IP, R2 
    /*00008da4*/ LDR         R2, [SP, #112]              @ 0x70 
    /*00008da8*/ ADD         LR, IP, R2 
    /*00008dac*/ LDR         R2, [SP, #152]              @ 0x98 
    /*00008db0*/ ADD         IP, IP, R2 
    /*00008db4*/ MOV         R2, #0 
    AI_DRV_GetChnData_x8db8:
    /*00008db8*/ LDRB        R0, [R5, R2] 
    /*00008dbc*/ ADD         R1, R1, #4 
    /*00008dc0*/ CMP         R7, R1 
    /*00008dc4*/ ADD         R3, R3, #4 
    /*00008dc8*/ STRB        R0, [R3, #-4] 
    /*00008dcc*/ LDRB        R0, [R6, R2] 
    /*00008dd0*/ STRB        R0, [R3, #-3] 
    /*00008dd4*/ LDRB        R0, [LR, R2] 
    /*00008dd8*/ STRB        R0, [R3, #-2] 
    /*00008ddc*/ LDRB        R0, [IP, R2] 
    /*00008de0*/ ADD         R2, R2, R4 
    /*00008de4*/ STRB        R0, [R3, #-1] 
    /*00008de8*/ BHI         AI_DRV_GetChnData_x8db8
    /*00008dec*/ B           AI_DRV_GetChnData_x8ba4
    AI_DRV_GetChnData_x8df0:
    /*00008df0*/ LDR         SL, [SP, #92]               @ 0x5c 
    AI_DRV_GetChnData_x8df4:
    /*00008df4*/ LDR         R3, [SP, #8] 
    /*00008df8*/ LDR         R2, [SP, #12] 
    /*00008dfc*/ CMP         R3, R2 
    /*00008e00*/ BEQ         AI_DRV_GetChnData_x8ba4
    /*00008e04*/ LDR         R3, [SP, #40]               @ 0x28 
    /*00008e08*/ LDR         R2, [SP, #76]               @ 0x4c 
    /*00008e0c*/ TST         R3, #1 
    /*00008e10*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00008e14*/ ADD         R7, R3, R2 
    /*00008e18*/ LDRNE       R3, [SP, #76]               @ 0x4c 
    /*00008e1c*/ LDREQ       R2, [SP, #128]              @ 0x80 
    /*00008e20*/ SUBNE       R9, R3, #4 
    /*00008e24*/ LDRNE       R3, [SP, #56]               @ 0x38 
    /*00008e28*/ ADDEQ       R9, R3, R2 
    /*00008e2c*/ ADDNE       R9, R3, R9 
    /*00008e30*/ LDR         R3, [SP, #88]               @ 0x58 
    /*00008e34*/ CMP         R3, #0 
    /*00008e38*/ BEQ         AI_DRV_GetChnData_x8e88
    /*00008e3c*/ LDR         R3, [SP, #28] 
    /*00008e40*/ MOV         R0, R7 
    /*00008e44*/ LDR         R2, [SP, #144]              @ 0x90 
    /*00008e48*/ MOV         IP, R9 
    /*00008e4c*/ LDR         R1, [SP, #116]              @ 0x74 
    /*00008e50*/ ADD         R2, R3, R2 
    /*00008e54*/ ADD         LR, R3, R1 
    AI_DRV_GetChnData_x8e58:
    /*00008e58*/ LDR         R3, [R0] 
    /*00008e5c*/ ADD         R2, R2, #4 
    /*00008e60*/ ADD         R0, R0, SL 
    /*00008e64*/ STR         R3, [R2, #-4] 
    /*00008e68*/ LDR         R1, [FP] 
    /*00008e6c*/ CMP         R1, #1 
    /*00008e70*/ LDREQ       R1, [IP] 
    /*00008e74*/ ADD         IP, IP, SL 
    /*00008e78*/ SUBEQ       R3, R3, R1 
    /*00008e7c*/ STREQ       R3, [R2, #-4] 
    /*00008e80*/ CMP         R2, LR 
    /*00008e84*/ BNE         AI_DRV_GetChnData_x8e58
    AI_DRV_GetChnData_x8e88:
    /*00008e88*/ LDR         R2, [SP, #12] 
    /*00008e8c*/ LDR         R8, [SP, #80]               @ 0x50 
    /*00008e90*/ CMP         R2, R8 
    /*00008e94*/ BLS         AI_DRV_GetChnData_x8ba4
    /*00008e98*/ LDR         R3, [SP, #28] 
    /*00008e9c*/ LDR         R2, [SP, #116]              @ 0x74 
    /*00008ea0*/ STR         SL, [SP, #40]               @ 0x28 
    /*00008ea4*/ ADD         R3, R3, R2 
    /*00008ea8*/ LDR         R2, [SP, #132]              @ 0x84 
    /*00008eac*/ STR         R9, [SP, #28] 
    /*00008eb0*/ ADD         R1, R7, R2 
    /*00008eb4*/ ADD         R2, R9, R2 
    /*00008eb8*/ STR         R2, [SP, #36]               @ 0x24 
    /*00008ebc*/ ADD         R2, SP, #104                @ 0x68 
    /*00008ec0*/ MOV         SL, R1 
    /*00008ec4*/ LDM         R2, {R2, R5, R6} 
    /*00008ec8*/ B           AI_DRV_GetChnData_x8ee8
    AI_DRV_GetChnData_x8ecc:
    /*00008ecc*/ LDR         R1, [SP, #12] 
    /*00008ed0*/ ADD         R3, R3, #16 
    /*00008ed4*/ ADD         R2, R2, R4 
    /*00008ed8*/ ADD         R5, R5, R4 
    /*00008edc*/ CMP         R1, R8 
    /*00008ee0*/ ADD         R6, R6, R4 
    /*00008ee4*/ BLS         AI_DRV_GetChnData_x8ba0
    AI_DRV_GetChnData_x8ee8:
    /*00008ee8*/ LDR         LR, [R7, R2] 
    /*00008eec*/ ADD         R8, R8, #4 
    /*00008ef0*/ STR         LR, [R3] 
    /*00008ef4*/ LDR         IP, [R7, R5] 
    /*00008ef8*/ STR         IP, [R3, #4] 
    /*00008efc*/ LDR         R0, [R7, R6] 
    /*00008f00*/ STR         R0, [R3, #8] 
    /*00008f04*/ LDR         R1, [SL, R2] 
    /*00008f08*/ STR         R1, [R3, #12] 
    /*00008f0c*/ LDR         R9, [FP] 
    /*00008f10*/ CMP         R9, #1 
    /*00008f14*/ BNE         AI_DRV_GetChnData_x8ecc
    /*00008f18*/ LDR         R9, [SP, #28] 
    /*00008f1c*/ LDR         R9, [R9, R2] 
    /*00008f20*/ SUB         LR, LR, R9 
    /*00008f24*/ LDR         R9, [SP, #28] 
    /*00008f28*/ STR         LR, [R3] 
    /*00008f2c*/ LDR         LR, [R9, R5] 
    /*00008f30*/ SUB         IP, IP, LR 
    /*00008f34*/ STR         IP, [R3, #4] 
    /*00008f38*/ LDR         IP, [R9, R6] 
    /*00008f3c*/ SUB         R0, R0, IP 
    /*00008f40*/ STR         R0, [R3, #8] 
    /*00008f44*/ LDR         R0, [SP, #36]               @ 0x24 
    /*00008f48*/ LDR         R0, [R0, R2] 
    /*00008f4c*/ SUB         R1, R1, R0 
    /*00008f50*/ STR         R1, [R3, #12] 
    /*00008f54*/ B           AI_DRV_GetChnData_x8ecc
    AI_DRV_GetChnData_x8f58:
    /*00008f58*/ CMP         R3, #1 
    /*00008f5c*/ BEQ         AI_DRV_GetChnData_x8f90
    AI_DRV_GetChnData_x8f60:
    /*00008f60*/ MOVW        R3, #503                    @ 0x1f7 
    /*00008f64*/ MOV         R1, #21 
    /*00008f68*/ STR         R3, [SP] 
    /*00008f6c*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2368] 
    /*00008f70*/ LDR         R3, [PC, #980]              @ 0x000000000000934c 
    /*00008f74*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2368] 
    /*00008f78*/ MOV         R0, #6 
    /*00008f7c*/ BL          0x0000000000008F7C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008f80*/ MOVW        R0, #32776                  @ 0x8008 
    /*00008f84*/ MOVT        R0, #40981                  @ 0xa015 
    /*00008f88*/ ADD         SP, SP, #164                @ 0xa4 
    /*00008f8c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    AI_DRV_GetChnData_x8f90:
    /*00008f90*/ LDR         R2, [SP, #40]               @ 0x28 
    /*00008f94*/ LDR         R3, [FP] 
    /*00008f98*/ LDR         R0, [SP, #52]               @ 0x34 
    /*00008f9c*/ LSL         R2, R2, #1 
    /*00008fa0*/ CMP         R3, #1 
    /*00008fa4*/ ADD         R1, R2, #2 
    /*00008fa8*/ STR         R2, [SP, #76]               @ 0x4c 
    /*00008fac*/ STR         R1, [SP, #128]              @ 0x80 
    /*00008fb0*/ ADD         R7, R0, R2 
    /*00008fb4*/ ADD         R9, R0, R1 
    /*00008fb8*/ BEQ         AI_DRV_GetChnData_x9270
    AI_DRV_GetChnData_x8fbc:
    /*00008fbc*/ LDR         R3, [SP, #32] 
    /*00008fc0*/ CMP         R3, #0 
    /*00008fc4*/ BEQ         AI_DRV_GetChnData_x9010
    /*00008fc8*/ LDR         R3, [SP, #28] 
    /*00008fcc*/ MOV         R0, R7 
    /*00008fd0*/ LDR         R2, [SP, #124]              @ 0x7c 
    /*00008fd4*/ MOV         IP, R9 
    /*00008fd8*/ ADD         LR, R3, R2 
    /*00008fdc*/ MOV         R2, R3 
    AI_DRV_GetChnData_x8fe0:
    /*00008fe0*/ LDRH        R3, [R0] 
    /*00008fe4*/ ADD         R2, R2, #2 
    /*00008fe8*/ ADD         R0, R0, SL 
    /*00008fec*/ STRH        R3, [R2, #-2] 
    /*00008ff0*/ LDR         R1, [FP] 
    /*00008ff4*/ CMP         R1, #1 
    /*00008ff8*/ LDRHEQ      R1, [IP] 
    /*00008ffc*/ ADD         IP, IP, SL 
    /*00009000*/ SUBEQ       R3, R3, R1 
    /*00009004*/ STRHEQ      R3, [R2, #-2] 
    /*00009008*/ CMP         LR, R2 
    /*0000900c*/ BNE         AI_DRV_GetChnData_x8fe0
    AI_DRV_GetChnData_x9010:
    /*00009010*/ LDR         R3, [SP, #8] 
    /*00009014*/ LDR         R2, [SP, #32] 
    /*00009018*/ CMP         R3, R2 
    /*0000901c*/ BLS         AI_DRV_GetChnData_x90e8
    /*00009020*/ LDR         R2, [SP, #124]              @ 0x7c 
    /*00009024*/ LDR         R3, [SP, #28] 
    /*00009028*/ STR         SL, [SP, #92]               @ 0x5c 
    /*0000902c*/ ADD         R3, R3, R2 
    /*00009030*/ LDR         R2, [SP, #100]              @ 0x64 
    /*00009034*/ LDR         R8, [SP, #32] 
    /*00009038*/ ADD         R1, R7, R2 
    /*0000903c*/ ADD         R2, R9, R2 
    /*00009040*/ STR         R2, [SP, #48]               @ 0x30 
    /*00009044*/ ADD         R2, SP, #64                 @ 0x40 
    /*00009048*/ STR         R9, [SP, #36]               @ 0x24 
    /*0000904c*/ MOV         SL, R1 
    /*00009050*/ LDM         R2, {R2, R5, R6} 
    /*00009054*/ B           AI_DRV_GetChnData_x9074
    AI_DRV_GetChnData_x9058:
    /*00009058*/ LDR         R1, [SP, #8] 
    /*0000905c*/ ADD         R3, R3, #8 
    /*00009060*/ ADD         R2, R2, R4 
    /*00009064*/ ADD         R5, R5, R4 
    /*00009068*/ CMP         R1, R8 
    /*0000906c*/ ADD         R6, R6, R4 
    /*00009070*/ BLS         AI_DRV_GetChnData_x90e4
    AI_DRV_GetChnData_x9074:
    /*00009074*/ LDRH        LR, [R7, R2] 
    /*00009078*/ ADD         R8, R8, #4 
    /*0000907c*/ STRH        LR, [R3] 
    /*00009080*/ LDRH        IP, [R7, R5] 
    /*00009084*/ STRH        IP, [R3, #2] 
    /*00009088*/ LDRH        R0, [R7, R6] 
    /*0000908c*/ STRH        R0, [R3, #4] 
    /*00009090*/ LDRH        R1, [SL, R2] 
    /*00009094*/ STRH        R1, [R3, #6] 
    /*00009098*/ LDR         R9, [FP] 
    /*0000909c*/ CMP         R9, #1 
    /*000090a0*/ BNE         AI_DRV_GetChnData_x9058
    /*000090a4*/ LDR         R9, [SP, #36]               @ 0x24 
    /*000090a8*/ LDRH        R9, [R9, R2] 
    /*000090ac*/ SUB         LR, LR, R9 
    /*000090b0*/ LDR         R9, [SP, #36]               @ 0x24 
    /*000090b4*/ STRH        LR, [R3] 
    /*000090b8*/ LDRH        LR, [R9, R5] 
    /*000090bc*/ SUB         IP, IP, LR 
    /*000090c0*/ STRH        IP, [R3, #2] 
    /*000090c4*/ LDRH        IP, [R9, R6] 
    /*000090c8*/ SUB         R0, R0, IP 
    /*000090cc*/ STRH        R0, [R3, #4] 
    /*000090d0*/ LDR         R0, [SP, #48]               @ 0x30 
    /*000090d4*/ LDRH        R0, [R0, R2] 
    /*000090d8*/ SUB         R1, R1, R0 
    /*000090dc*/ STRH        R1, [R3, #6] 
    /*000090e0*/ B           AI_DRV_GetChnData_x9058
    AI_DRV_GetChnData_x90e4:
    /*000090e4*/ LDR         SL, [SP, #92]               @ 0x5c 
    AI_DRV_GetChnData_x90e8:
    /*000090e8*/ LDR         R3, [SP, #8] 
    /*000090ec*/ LDR         R2, [SP, #12] 
    /*000090f0*/ CMP         R3, R2 
    /*000090f4*/ BEQ         AI_DRV_GetChnData_x8ba4
    /*000090f8*/ LDR         R3, [SP, #40]               @ 0x28 
    /*000090fc*/ LDR         R2, [SP, #76]               @ 0x4c 
    /*00009100*/ TST         R3, #1 
    /*00009104*/ LDR         R3, [SP, #56]               @ 0x38 
    /*00009108*/ ADD         R7, R3, R2 
    /*0000910c*/ LDRNE       R3, [SP, #76]               @ 0x4c 
    /*00009110*/ LDREQ       R2, [SP, #128]              @ 0x80 
    /*00009114*/ LDRNE       R2, [SP, #56]               @ 0x38 
    /*00009118*/ SUBNE       R3, R3, #2 
    /*0000911c*/ ADD         R9, R2, R3 
    /*00009120*/ LDR         R3, [SP, #88]               @ 0x58 
    /*00009124*/ CMP         R3, #0 
    /*00009128*/ BEQ         AI_DRV_GetChnData_x9184
    /*0000912c*/ LDR         R3, [SP, #28] 
    /*00009130*/ MOV         R0, R7 
    /*00009134*/ LDR         R2, [SP, #148]              @ 0x94 
    /*00009138*/ LDR         R1, [SP, #136]              @ 0x88 
    /*0000913c*/ ADD         R2, R3, R2 
    /*00009140*/ ADD         IP, R3, R1 
    /*00009144*/ MOV         R1, R9 
    AI_DRV_GetChnData_x9148:
    /*00009148*/ LDRH        LR, [R1] 
    /*0000914c*/ ADD         R2, R2, #2 
    /*00009150*/ LDRH        R3, [R0] 
    /*00009154*/ ADD         R0, R0, SL 
    /*00009158*/ SUB         R3, R3, LR 
    /*0000915c*/ UXTH        R3, R3 
    /*00009160*/ STRH        R3, [R2, #-2] 
    /*00009164*/ LDR         LR, [FP] 
    /*00009168*/ CMP         LR, #1 
    /*0000916c*/ LDRHEQ      LR, [R1] 
    /*00009170*/ ADD         R1, R1, SL 
    /*00009174*/ SUBEQ       R3, R3, LR 
    /*00009178*/ STRHEQ      R3, [R2, #-2] 
    /*0000917c*/ CMP         IP, R2 
    /*00009180*/ BNE         AI_DRV_GetChnData_x9148
    AI_DRV_GetChnData_x9184:
    /*00009184*/ LDR         R2, [SP, #12] 
    /*00009188*/ LDR         R8, [SP, #80]               @ 0x50 
    /*0000918c*/ CMP         R2, R8 
    /*00009190*/ BLS         AI_DRV_GetChnData_x8ba4
    /*00009194*/ LDR         R3, [SP, #28] 
    /*00009198*/ LDR         R2, [SP, #136]              @ 0x88 
    /*0000919c*/ STR         SL, [SP, #40]               @ 0x28 
    /*000091a0*/ ADD         R3, R3, R2 
    /*000091a4*/ LDR         R2, [SP, #132]              @ 0x84 
    /*000091a8*/ STR         R9, [SP, #28] 
    /*000091ac*/ ADD         R1, R7, R2 
    /*000091b0*/ ADD         R2, R9, R2 
    /*000091b4*/ STR         R2, [SP, #36]               @ 0x24 
    /*000091b8*/ ADD         R2, SP, #104                @ 0x68 
    /*000091bc*/ MOV         SL, R1 
    /*000091c0*/ LDM         R2, {R2, R5, R6} 
    /*000091c4*/ B           AI_DRV_GetChnData_x91e4
    AI_DRV_GetChnData_x91c8:
    /*000091c8*/ LDR         R1, [SP, #12] 
    /*000091cc*/ ADD         R3, R3, #8 
    /*000091d0*/ ADD         R2, R2, R4 
    /*000091d4*/ ADD         R5, R5, R4 
    /*000091d8*/ CMP         R1, R8 
    /*000091dc*/ ADD         R6, R6, R4 
    /*000091e0*/ BLS         AI_DRV_GetChnData_x8ba0
    AI_DRV_GetChnData_x91e4:
    /*000091e4*/ LDRH        LR, [R7, R2] 
    /*000091e8*/ ADD         R8, R8, #4 
    /*000091ec*/ STRH        LR, [R3] 
    /*000091f0*/ LDRH        IP, [R7, R5] 
    /*000091f4*/ STRH        IP, [R3, #2] 
    /*000091f8*/ LDRH        R0, [R7, R6] 
    /*000091fc*/ STRH        R0, [R3, #4] 
    /*00009200*/ LDRH        R1, [SL, R2] 
    /*00009204*/ STRH        R1, [R3, #6] 
    /*00009208*/ LDR         R9, [FP] 
    /*0000920c*/ CMP         R9, #1 
    /*00009210*/ BNE         AI_DRV_GetChnData_x91c8
    /*00009214*/ LDR         R9, [SP, #28] 
    /*00009218*/ LDRH        R9, [R9, R2] 
    /*0000921c*/ SUB         LR, LR, R9 
    /*00009220*/ LDR         R9, [SP, #28] 
    /*00009224*/ STRH        LR, [R3] 
    /*00009228*/ LDRH        LR, [R9, R5] 
    /*0000922c*/ SUB         IP, IP, LR 
    /*00009230*/ STRH        IP, [R3, #2] 
    /*00009234*/ LDRH        IP, [R9, R6] 
    /*00009238*/ SUB         R0, R0, IP 
    /*0000923c*/ STRH        R0, [R3, #4] 
    /*00009240*/ LDR         R0, [SP, #36]               @ 0x24 
    /*00009244*/ LDRH        R0, [R0, R2] 
    /*00009248*/ SUB         R1, R1, R0 
    /*0000924c*/ STRH        R1, [R3, #6] 
    /*00009250*/ B           AI_DRV_GetChnData_x91c8
    AI_DRV_GetChnData_x9254:
    /*00009254*/ LDR         R3, [SP, #40]               @ 0x28 
    /*00009258*/ TST         R3, #1 
    /*0000925c*/ MOVNE       R3, R2 
    /*00009260*/ SUBNE       R9, R3, #4 
    /*00009264*/ MOVNE       R3, R0 
    /*00009268*/ ADDNE       R9, R3, R9 
    /*0000926c*/ B           AI_DRV_GetChnData_x8a7c
    AI_DRV_GetChnData_x9270:
    /*00009270*/ LDR         R3, [SP, #40]               @ 0x28 
    /*00009274*/ TST         R3, #1 
    /*00009278*/ MOVNE       R3, R2 
    /*0000927c*/ SUBNE       R3, R3, #2 
    /*00009280*/ MOVNE       R2, R0 
    /*00009284*/ ADDNE       R9, R2, R3 
    /*00009288*/ B           AI_DRV_GetChnData_x8fbc
    AI_DRV_GetChnData_x928c:
    /*0000928c*/ LDR         R3, [R3, #52]               @ 0x34 
    /*00009290*/ CMP         R3, #1 
    /*00009294*/ MOVEQ       R3, #16 
    /*00009298*/ BNE         AI_DRV_GetChnData_x8898
    /*0000929c*/ B           AI_DRV_GetChnData_x88a0
    AI_DRV_GetChnData_x92a0:
    /*000092a0*/ LDR         R3, [SP, #120]              @ 0x78 
    /*000092a4*/ MOV         R2, #184                    @ 0xb8 
    /*000092a8*/ LDR         R1, [SP, #96]               @ 0x60 
    /*000092ac*/ MLA         R2, R2, R3, R1 
    /*000092b0*/ LDR         R1, [SP, #156]              @ 0x9c 
    /*000092b4*/ LDR         R3, [R2, #116]              @ 0x74 
    /*000092b8*/ CMP         R1, R3 
    /*000092bc*/ SUBCC       R3, R3, R1 
    /*000092c0*/ STRCC       R3, [R2, #20] 
    /*000092c4*/ BCC         AI_DRV_GetChnData_x8c10
    /*000092c8*/ B           AI_DRV_GetChnData_x8bf4
    AI_DRV_GetChnData_x92cc:
    /*000092cc*/ MOV         R3, #272                    @ 0x110 
    /*000092d0*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2334] 
    /*000092d4*/ STR         R3, [SP] 
    /*000092d8*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2334] 
    /*000092dc*/ LDR         R3, [PC, #104]              @ 0x000000000000934c 
    /*000092e0*/ MOV         R1, #21 
    /*000092e4*/ MOV         R0, #3 
    /*000092e8*/ BL          0x00000000000092E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000092ec*/ MOVW        R0, #32771                  @ 0x8003 
    /*000092f0*/ MOVT        R0, #40981                  @ 0xa015 
    /*000092f4*/ B           AI_DRV_GetChnData_x8c6c
    AI_DRV_GetChnData_x92f8:
    /*000092f8*/ MOVW        R3, #265                    @ 0x109 
    /*000092fc*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2300] 
    /*00009300*/ STR         R3, [SP] 
    /*00009304*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2300] 
    /*00009308*/ LDR         R3, [PC, #60]               @ 0x000000000000934c 
    /*0000930c*/ MOV         R1, #21 
    /*00009310*/ MOV         R0, #3 
    /*00009314*/ BL          0x0000000000009314          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00009318*/ MOVW        R0, #32773                  @ 0x8005 
    /*0000931c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00009320*/ B           AI_DRV_GetChnData_x8c6c
    AI_DRV_GetChnData_x9324:
    /*00009324*/ MOVW        R3, #525                    @ 0x20d 
    /*00009328*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2394] 
    /*0000932c*/ STR         R3, [SP] 
    /*00009330*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2394] 
    /*00009334*/ LDR         R3, [PC, #16]               @ 0x000000000000934c 
    /*00009338*/ MOV         R1, #21 
    /*0000933c*/ BL          0x000000000000933C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00009340*/ MOVW        R0, #32784                  @ 0x8010 
    /*00009344*/ MOVT        R0, #40981                  @ 0xa015 
    /*00009348*/ B           AI_DRV_GetChnData_x8c6c
    /*0000934c*/ @           <UNDEFINED> instruction: 0x000002b4 
FUNC_END AI_DRV_GetChnData


FUNC_BEGIN AI_DRV_DevInit
    /*00009350*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*00009354*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*00009358*/ MOV         R2, #184                    @ 0xb8 
    /*0000935c*/ MOV         R1, #0 
    /*00009360*/ PUSH        {R4, R5, LR} 
    /*00009364*/ SUB         SP, SP, #12 
    /*00009368*/ MLA         R4, R2, R0, R3 
    /*0000936c*/ MOV         R5, R1 
    /*00009370*/ MOV         R0, R4 
    /*00009374*/ BL          0x0000000000009374          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00009378*/ MOV         R0, #20 
    /*0000937c*/ STR         R5, [R4, #76]               @ 0x4c 
    /*00009380*/ BL          0x0000000000009380          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00009384*/ SUBS        R3, R0, #0 
    /*00009388*/ BEQ         AI_DRV_DevInit_x93a8
    /*0000938c*/ LDR         R3, [R3, #48]               @ 0x30 
    /*00009390*/ MOVW        R0, #0                      @ AI_DRV_ISR [R_ARM_MOVW_ABS_NC: .text+0x817c] 
    /*00009394*/ MOVT        R0, #0                      @ AI_DRV_ISR [R_ARM_MOVT_ABS: .text+0x817c] 
    /*00009398*/ BLX         R3 
    /*0000939c*/ MOV         R0, R5 
    AI_DRV_DevInit_x93a0:
    /*000093a0*/ ADD         SP, SP, #12 
    /*000093a4*/ POP         {R4, R5, PC} 
    AI_DRV_DevInit_x93a8:
    /*000093a8*/ MOV         R3, #636                    @ 0x27c 
    /*000093ac*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23dc] 
    /*000093b0*/ STR         R3, [SP] 
    /*000093b4*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23dc] 
    /*000093b8*/ LDR         R3, [PC, #16]               @ 0x00000000000093d0 
    /*000093bc*/ MOV         R1, #21 
    /*000093c0*/ BL          0x00000000000093C0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000093c4*/ MOVW        R0, #32784                  @ 0x8010 
    /*000093c8*/ MOVT        R0, #40981                  @ 0xa015 
    /*000093cc*/ B           AI_DRV_DevInit_x93a0
    /*000093d0*/ ANDEQ       R0, R0, R8, ASR #5 
FUNC_END AI_DRV_DevInit


FUNC_BEGIN AI_DRV_DevDeInit
    /*000093d4*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*000093d8*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*000093dc*/ MOV         R3, #184                    @ 0xb8 
    /*000093e0*/ MOV         R1, #0 
    /*000093e4*/ MLA         R3, R3, R0, R2 
    /*000093e8*/ MOV         R0, R1 
    /*000093ec*/ STR         R1, [R3, #76]               @ 0x4c 
    /*000093f0*/ BX          LR 
FUNC_END AI_DRV_DevDeInit


FUNC_BEGIN AI_DRV_CheckAioExportFunc
    /*000093f4*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000093f8*/ MOV         R0, #20 
    /*000093fc*/ SUB         SP, SP, #12 
    /*00009400*/ BL          0x0000000000009400          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00009404*/ CMP         R0, #0 
    /*00009408*/ MOVNE       R0, #0 
    /*0000940c*/ BEQ         AI_DRV_CheckAioExportFunc_x9418
    AI_DRV_CheckAioExportFunc_x9410:
    /*00009410*/ ADD         SP, SP, #12 
    /*00009414*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    AI_DRV_CheckAioExportFunc_x9418:
    /*00009418*/ MOVW        R3, #659                    @ 0x293 
    /*0000941c*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23dc] 
    /*00009420*/ STR         R3, [SP] 
    /*00009424*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23dc] 
    /*00009428*/ LDR         R3, [PC, #16]               @ 0x0000000000009440 
    /*0000942c*/ MOV         R1, #21 
    /*00009430*/ BL          0x0000000000009430          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00009434*/ MOVW        R0, #32784                  @ 0x8010 
    /*00009438*/ MOVT        R0, #40981                  @ 0xa015 
    /*0000943c*/ B           AI_DRV_CheckAioExportFunc_x9410
    /*00009440*/ LDRDEQ      R0, [R0], -R8 
FUNC_END AI_DRV_CheckAioExportFunc


FUNC_BEGIN AI_DRV_OpenClock
    /*00009444*/ MOV         R0, #0 
    /*00009448*/ BX          LR 
FUNC_END AI_DRV_OpenClock


FUNC_BEGIN AI_DRV_CloseClock
    /*0000944c*/ MOV         R0, #0 
    /*00009450*/ BX          LR 
FUNC_END AI_DRV_CloseClock


FUNC_BEGIN AI_DRV_SetTrackMode
    /*00009454*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*00009458*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*0000945c*/ PUSH        {R4, R5, R6, LR} 
    /*00009460*/ MOV         R4, #184                    @ 0xb8 
    /*00009464*/ MLA         R4, R4, R0, R3 
    /*00009468*/ SUB         SP, SP, #8 
    /*0000946c*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00009470*/ CMP         R3, #1 
    /*00009474*/ BHI         AI_DRV_SetTrackMode_x94b0
    /*00009478*/ MOV         R5, R0 
    /*0000947c*/ MOV         R0, #20 
    /*00009480*/ MOV         R6, R1 
    /*00009484*/ BL          0x0000000000009484          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00009488*/ SUBS        R3, R0, #0 
    /*0000948c*/ BEQ         AI_DRV_SetTrackMode_x94e0
    /*00009490*/ LDR         R3, [R3, #56]               @ 0x38 
    /*00009494*/ MOV         R0, R5 
    /*00009498*/ MOV         R1, R6 
    /*0000949c*/ BLX         R3 
    /*000094a0*/ CMP         R0, #0 
    /*000094a4*/ STREQ       R6, [R4, #144]              @ 0x90 
    AI_DRV_SetTrackMode_x94a8:
    /*000094a8*/ ADD         SP, SP, #8 
    /*000094ac*/ POP         {R4, R5, R6, PC} 
    AI_DRV_SetTrackMode_x94b0:
    /*000094b0*/ MOVW        R3, #685                    @ 0x2ad 
    /*000094b4*/ MOV         R1, #21 
    /*000094b8*/ STR         R3, [SP] 
    /*000094bc*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2414] 
    /*000094c0*/ LDR         R3, [PC, #64]               @ 0x0000000000009508 
    /*000094c4*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2414] 
    /*000094c8*/ MOV         R0, #3 
    /*000094cc*/ BL          0x00000000000094CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000094d0*/ MOVW        R0, #32776                  @ 0x8008 
    /*000094d4*/ MOVT        R0, #40981                  @ 0xa015 
    /*000094d8*/ ADD         SP, SP, #8 
    /*000094dc*/ POP         {R4, R5, R6, PC} 
    AI_DRV_SetTrackMode_x94e0:
    /*000094e0*/ MOV         R3, #692                    @ 0x2b4 
    /*000094e4*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23dc] 
    /*000094e8*/ STR         R3, [SP] 
    /*000094ec*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23dc] 
    /*000094f0*/ LDR         R3, [PC, #16]               @ 0x0000000000009508 
    /*000094f4*/ MOV         R1, #21 
    /*000094f8*/ BL          0x00000000000094F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000094fc*/ MOVW        R0, #32784                  @ 0x8010 
    /*00009500*/ MOVT        R0, #40981                  @ 0xa015 
    /*00009504*/ B           AI_DRV_SetTrackMode_x94a8
    /*00009508*/ STRDEQ      R0, [R0], -R4 
FUNC_END AI_DRV_SetTrackMode


FUNC_BEGIN AI_DRV_GetTrackMode
    /*0000950c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*00009510*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*00009514*/ PUSH        {R4, R5, LR} 
    /*00009518*/ MOV         R4, #184                    @ 0xb8 
    /*0000951c*/ MLA         R4, R4, R0, R3 
    /*00009520*/ SUB         SP, SP, #12 
    /*00009524*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*00009528*/ CMP         R3, #1 
    /*0000952c*/ BHI         AI_DRV_GetTrackMode_x9558
    /*00009530*/ MOV         R0, #20 
    /*00009534*/ MOV         R5, R1 
    /*00009538*/ BL          0x0000000000009538          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000953c*/ CMP         R0, #0 
    /*00009540*/ LDRNE       R3, [R4, #144]              @ 0x90 
    /*00009544*/ MOVNE       R0, #0 
    /*00009548*/ STRNE       R3, [R5] 
    /*0000954c*/ BEQ         AI_DRV_GetTrackMode_x9588
    AI_DRV_GetTrackMode_x9550:
    /*00009550*/ ADD         SP, SP, #12 
    /*00009554*/ POP         {R4, R5, PC} 
    AI_DRV_GetTrackMode_x9558:
    /*00009558*/ MOVW        R3, #715                    @ 0x2cb 
    /*0000955c*/ MOV         R1, #21 
    /*00009560*/ STR         R3, [SP] 
    /*00009564*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2414] 
    /*00009568*/ LDR         R3, [PC, #64]               @ 0x00000000000095b0 
    /*0000956c*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2414] 
    /*00009570*/ MOV         R0, #3 
    /*00009574*/ BL          0x0000000000009574          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00009578*/ MOVW        R0, #32776                  @ 0x8008 
    /*0000957c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00009580*/ ADD         SP, SP, #12 
    /*00009584*/ POP         {R4, R5, PC} 
    AI_DRV_GetTrackMode_x9588:
    /*00009588*/ MOVW        R3, #721                    @ 0x2d1 
    /*0000958c*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23dc] 
    /*00009590*/ STR         R3, [SP] 
    /*00009594*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23dc] 
    /*00009598*/ LDR         R3, [PC, #16]               @ 0x00000000000095b0 
    /*0000959c*/ MOV         R1, #21 
    /*000095a0*/ BL          0x00000000000095A0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000095a4*/ MOVW        R0, #32784                  @ 0x8010 
    /*000095a8*/ MOVT        R0, #40981                  @ 0xa015 
    /*000095ac*/ B           AI_DRV_GetTrackMode_x9550
    /*000095b0*/ ANDEQ       R0, R0, R8, LSL #6 
FUNC_END AI_DRV_GetTrackMode


FUNC_BEGIN AI_DRV_SetClkDir
    /*000095b4*/ PUSH        {R4, R5, LR} 
    /*000095b8*/ MOV         R4, R0 
    /*000095bc*/ SUB         SP, SP, #12 
    /*000095c0*/ MOV         R0, #20 
    /*000095c4*/ MOV         R5, R1 
    /*000095c8*/ BL          0x00000000000095C8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000095cc*/ SUBS        R3, R0, #0 
    /*000095d0*/ BEQ         AI_DRV_SetClkDir_x9604
    /*000095d4*/ LDR         R3, [R3, #64]               @ 0x40 
    /*000095d8*/ MOV         R0, R4 
    /*000095dc*/ MOV         R1, R5 
    /*000095e0*/ BLX         R3 
    /*000095e4*/ CMP         R0, #0 
    /*000095e8*/ MOVWEQ      R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*000095ec*/ MOVTEQ      R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*000095f0*/ MOVEQ       R2, #184                    @ 0xb8 
    /*000095f4*/ MLAEQ       R4, R2, R4, R3 
    /*000095f8*/ STREQ       R5, [R4, #148]              @ 0x94 
    AI_DRV_SetClkDir_x95fc:
    /*000095fc*/ ADD         SP, SP, #12 
    /*00009600*/ POP         {R4, R5, PC} 
    AI_DRV_SetClkDir_x9604:
    /*00009604*/ MOVW        R3, #742                    @ 0x2e6 
    /*00009608*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23dc] 
    /*0000960c*/ STR         R3, [SP] 
    /*00009610*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23dc] 
    /*00009614*/ LDR         R3, [PC, #16]               @ 0x000000000000962c 
    /*00009618*/ MOV         R1, #21 
    /*0000961c*/ BL          0x000000000000961C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00009620*/ MOVW        R0, #32784                  @ 0x8010 
    /*00009624*/ MOVT        R0, #40981                  @ 0xa015 
    /*00009628*/ B           AI_DRV_SetClkDir_x95fc
    /*0000962c*/ ANDEQ       R0, R0, IP, LSL R3 
FUNC_END AI_DRV_SetClkDir


FUNC_BEGIN AI_DRV_GetClkDir
    /*00009630*/ PUSH        {R4, R5, LR} 
    /*00009634*/ MOV         R4, R0 
    /*00009638*/ SUB         SP, SP, #12 
    /*0000963c*/ MOV         R0, #20 
    /*00009640*/ MOV         R5, R1 
    /*00009644*/ BL          0x0000000000009644          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00009648*/ SUBS        R3, R0, #0 
    /*0000964c*/ BEQ         AI_DRV_GetClkDir_x9668
    /*00009650*/ LDR         R3, [R3, #68]               @ 0x44 
    /*00009654*/ MOV         R1, R5 
    /*00009658*/ MOV         R0, R4 
    /*0000965c*/ ADD         SP, SP, #12 
    /*00009660*/ POP         {R4, R5, LR} 
    /*00009664*/ BX          R3 
    AI_DRV_GetClkDir_x9668:
    /*00009668*/ MOVW        R3, #761                    @ 0x2f9 
    /*0000966c*/ MOV         R1, #21 
    /*00009670*/ STR         R3, [SP] 
    /*00009674*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23dc] 
    /*00009678*/ LDR         R3, [PC, #20]               @ 0x0000000000009694 
    /*0000967c*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23dc] 
    /*00009680*/ BL          0x0000000000009680          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00009684*/ MOVW        R0, #32784                  @ 0x8010 
    /*00009688*/ MOVT        R0, #40981                  @ 0xa015 
    /*0000968c*/ ADD         SP, SP, #12 
    /*00009690*/ POP         {R4, R5, PC} 
    /*00009694*/ ANDEQ       R0, R0, R0, LSR R3 
FUNC_END AI_DRV_GetClkDir


FUNC_BEGIN AI_DRV_SetAcodecGain
    /*00009698*/ PUSH        {R4, LR} 
    /*0000969c*/ MOV         R4, R0 
    /*000096a0*/ SUB         SP, SP, #8 
    /*000096a4*/ MOV         R0, #20 
    /*000096a8*/ BL          0x00000000000096A8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000096ac*/ SUBS        R3, R0, #0 
    /*000096b0*/ BEQ         AI_DRV_SetAcodecGain_x9700
    /*000096b4*/ LDR         R3, [R3, #72]               @ 0x48 
    /*000096b8*/ CMP         R3, #0 
    /*000096bc*/ BEQ         AI_DRV_SetAcodecGain_x96d0
    /*000096c0*/ MOV         R0, R4 
    /*000096c4*/ ADD         SP, SP, #8 
    /*000096c8*/ POP         {R4, LR} 
    /*000096cc*/ BX          R3 
    AI_DRV_SetAcodecGain_x96d0:
    /*000096d0*/ MOV         R3, #780                    @ 0x30c 
    /*000096d4*/ MOVW        R2, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2454] 
    /*000096d8*/ STR         R3, [SP] 
    /*000096dc*/ MOVT        R2, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2454] 
    /*000096e0*/ LDR         R3, [PC, #64]               @ 0x0000000000009728 
    /*000096e4*/ MOV         R1, #21 
    /*000096e8*/ MOV         R0, #3 
    /*000096ec*/ BL          0x00000000000096EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000096f0*/ MOVW        R0, #32776                  @ 0x8008 
    /*000096f4*/ MOVT        R0, #40981                  @ 0xa015 
    AI_DRV_SetAcodecGain_x96f8:
    /*000096f8*/ ADD         SP, SP, #8 
    /*000096fc*/ POP         {R4, PC} 
    AI_DRV_SetAcodecGain_x9700:
    /*00009700*/ MOVW        R3, #774                    @ 0x306 
    /*00009704*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23dc] 
    /*00009708*/ STR         R3, [SP] 
    /*0000970c*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23dc] 
    /*00009710*/ LDR         R3, [PC, #16]               @ 0x0000000000009728 
    /*00009714*/ MOV         R1, #21 
    /*00009718*/ BL          0x0000000000009718          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000971c*/ MOVW        R0, #32784                  @ 0x8010 
    /*00009720*/ MOVT        R0, #40981                  @ 0xa015 
    /*00009724*/ B           AI_DRV_SetAcodecGain_x96f8
    /*00009728*/ ANDEQ       R0, R0, R4, ASR #6 
FUNC_END AI_DRV_SetAcodecGain


FUNC_BEGIN AI_DRV_GetAcodecGain
    /*0000972c*/ PUSH        {R4, LR} 
    /*00009730*/ MOV         R4, R0 
    /*00009734*/ SUB         SP, SP, #8 
    /*00009738*/ MOV         R0, #20 
    /*0000973c*/ BL          0x000000000000973C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00009740*/ SUBS        R3, R0, #0 
    /*00009744*/ BEQ         AI_DRV_GetAcodecGain_x9794
    /*00009748*/ LDR         R3, [R3, #76]               @ 0x4c 
    /*0000974c*/ CMP         R3, #0 
    /*00009750*/ BEQ         AI_DRV_GetAcodecGain_x9764
    /*00009754*/ MOV         R0, R4 
    /*00009758*/ ADD         SP, SP, #8 
    /*0000975c*/ POP         {R4, LR} 
    /*00009760*/ BX          R3 
    AI_DRV_GetAcodecGain_x9764:
    /*00009764*/ MOV         R3, #800                    @ 0x320 
    /*00009768*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2494] 
    /*0000976c*/ STR         R3, [SP] 
    /*00009770*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2494] 
    /*00009774*/ LDR         R3, [PC, #64]               @ 0x00000000000097bc 
    /*00009778*/ MOV         R1, #21 
    /*0000977c*/ MOV         R0, #3 
    /*00009780*/ BL          0x0000000000009780          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00009784*/ MOVW        R0, #32776                  @ 0x8008 
    /*00009788*/ MOVT        R0, #40981                  @ 0xa015 
    AI_DRV_GetAcodecGain_x978c:
    /*0000978c*/ ADD         SP, SP, #8 
    /*00009790*/ POP         {R4, PC} 
    AI_DRV_GetAcodecGain_x9794:
    /*00009794*/ MOVW        R3, #794                    @ 0x31a 
    /*00009798*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23dc] 
    /*0000979c*/ STR         R3, [SP] 
    /*000097a0*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23dc] 
    /*000097a4*/ LDR         R3, [PC, #16]               @ 0x00000000000097bc 
    /*000097a8*/ MOV         R1, #21 
    /*000097ac*/ BL          0x00000000000097AC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000097b0*/ MOVW        R0, #32784                  @ 0x8010 
    /*000097b4*/ MOVT        R0, #40981                  @ 0xa015 
    /*000097b8*/ B           AI_DRV_GetAcodecGain_x978c
    /*000097bc*/ ANDEQ       R0, R0, IP, ASR R3 
FUNC_END AI_DRV_GetAcodecGain


FUNC_BEGIN AI_DRV_Restore
    /*000097c0*/ PUSH        {R4, LR} 
    /*000097c4*/ MOV         R0, #20 
    /*000097c8*/ SUB         SP, SP, #8 
    /*000097cc*/ BL          0x00000000000097CC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000097d0*/ SUBS        R3, R0, #0 
    /*000097d4*/ BEQ         AI_DRV_Restore_x9814
    /*000097d8*/ LDR         R3, [R3, #168]              @ 0xa8 
    /*000097dc*/ BLX         R3 
    /*000097e0*/ SUBS        R4, R0, #0 
    /*000097e4*/ BEQ         AI_DRV_Restore_x9808
    /*000097e8*/ MOVW        R3, #823                    @ 0x337 
    /*000097ec*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24d4] 
    /*000097f0*/ STR         R3, [SP] 
    /*000097f4*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24d4] 
    /*000097f8*/ LDR         R3, [PC, #60]               @ 0x000000000000983c 
    /*000097fc*/ MOV         R1, #21 
    /*00009800*/ MOV         R0, #0 
    /*00009804*/ BL          0x0000000000009804          @ HI_LOG [R_ARM_CALL: +0x0] 
    AI_DRV_Restore_x9808:
    /*00009808*/ MOV         R0, R4 
    /*0000980c*/ ADD         SP, SP, #8 
    /*00009810*/ POP         {R4, PC} 
    AI_DRV_Restore_x9814:
    /*00009814*/ MOV         R3, #816                    @ 0x330 
    /*00009818*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23dc] 
    /*0000981c*/ STR         R3, [SP] 
    /*00009820*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23dc] 
    /*00009824*/ LDR         R3, [PC, #16]               @ 0x000000000000983c 
    /*00009828*/ MOV         R1, #21 
    /*0000982c*/ MOVW        R4, #32784                  @ 0x8010 
    /*00009830*/ BL          0x0000000000009830          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00009834*/ MOVT        R4, #40981                  @ 0xa015 
    /*00009838*/ B           AI_DRV_Restore_x9808
    /*0000983c*/ ANDEQ       R0, R0, R4, ROR R3 
FUNC_END AI_DRV_Restore


FUNC_BEGIN AI_DRV_ProcShow
    /*00009840*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00009844*/ SUB         SP, SP, #32 
    /*00009848*/ MOV         R5, R0 
    /*0000984c*/ MOVW        R1, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x25bc] 
    /*00009850*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x950] 
    /*00009854*/ MOVT        R1, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x25bc] 
    /*00009858*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x950] 
    /*0000985c*/ BL          0x000000000000985C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009860*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2640] 
    /*00009864*/ MOVW        R3, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2648] 
    /*00009868*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2640] 
    /*0000986c*/ MOVT        R3, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2648] 
    /*00009870*/ STR         R2, [SP, #28] 
    /*00009874*/ MOV         R0, R5 
    /*00009878*/ STR         R3, [SP, #24] 
    /*0000987c*/ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2650] 
    /*00009880*/ MOVW        R3, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2658] 
    /*00009884*/ MOVT        R2, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2650] 
    /*00009888*/ MOVT        R3, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2658] 
    /*0000988c*/ STR         R2, [SP, #20] 
    /*00009890*/ STR         R3, [SP, #16] 
    /*00009894*/ MOVW        R2, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2664] 
    /*00009898*/ MOVW        R3, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x266c] 
    /*0000989c*/ MOVT        R2, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2664] 
    /*000098a0*/ MOVT        R3, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0x266c] 
    /*000098a4*/ STR         R2, [SP, #12] 
    /*000098a8*/ STR         R3, [SP, #8] 
    /*000098ac*/ MOVW        R2, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2674] 
    /*000098b0*/ MOVW        R3, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x267c] 
    /*000098b4*/ MOVT        R2, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2674] 
    /*000098b8*/ MOVT        R3, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0x267c] 
    /*000098bc*/ STR         R2, [SP, #4] 
    /*000098c0*/ STR         R3, [SP] 
    /*000098c4*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2618] 
    /*000098c8*/ MOVW        R3, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2610] 
    /*000098cc*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2618] 
    /*000098d0*/ MOVT        R3, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2610] 
    /*000098d4*/ MOVW        R1, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2620] 
    /*000098d8*/ MOVT        R1, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2620] 
    /*000098dc*/ BL          0x00000000000098DC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000098e0*/ LDR         R3, [R4, #76]               @ 0x4c 
    /*000098e4*/ CMP         R3, #0 
    /*000098e8*/ BEQ         AI_DRV_ProcShow_x9a20
    /*000098ec*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*000098f0*/ CMP         R3, #0 
    /*000098f4*/ MOVWEQ      R3, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2504] 
    /*000098f8*/ MOVTEQ      R3, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2504] 
    /*000098fc*/ BNE         AI_DRV_ProcShow_x9bb0
    AI_DRV_ProcShow_x9900:
    /*00009900*/ LDR         R2, [R4, #36]               @ 0x24 
    /*00009904*/ CMP         R2, #8000                   @ 0x1f40 
    /*00009908*/ BEQ         AI_DRV_ProcShow_x9c74
    /*0000990c*/ MOVW        R1, #11025                  @ 0x2b11 
    /*00009910*/ CMP         R2, R1 
    /*00009914*/ BEQ         AI_DRV_ProcShow_x9c8c
    /*00009918*/ MOVW        R1, #12000                  @ 0x2ee0 
    /*0000991c*/ CMP         R2, R1 
    /*00009920*/ BEQ         AI_DRV_ProcShow_x9ca4
    /*00009924*/ CMP         R2, #16000                  @ 0x3e80 
    /*00009928*/ BEQ         AI_DRV_ProcShow_x9cb0
    /*0000992c*/ MOVW        R1, #22050                  @ 0x5622 
    /*00009930*/ CMP         R2, R1 
    /*00009934*/ BEQ         AI_DRV_ProcShow_x9cc8
    /*00009938*/ MOVW        R1, #24000                  @ 0x5dc0 
    /*0000993c*/ CMP         R2, R1 
    /*00009940*/ BEQ         AI_DRV_ProcShow_x9ce0
    /*00009944*/ CMP         R2, #32000                  @ 0x7d00 
    /*00009948*/ BEQ         AI_DRV_ProcShow_x9cec
    /*0000994c*/ MOVW        R1, #44100                  @ 0xac44 
    /*00009950*/ CMP         R2, R1 
    /*00009954*/ BEQ         AI_DRV_ProcShow_x9d04
    /*00009958*/ MOVW        R1, #48000                  @ 0xbb80 
    /*0000995c*/ CMP         R2, R1 
    /*00009960*/ BEQ         AI_DRV_ProcShow_x9d10
    /*00009964*/ CMP         R2, #64000                  @ 0xfa00 
    /*00009968*/ BEQ         AI_DRV_ProcShow_x9d1c
    /*0000996c*/ MOV         R0, #30464                  @ 0x7700 
    /*00009970*/ MOVT        R0, #1 
    /*00009974*/ CMP         R2, R0 
    /*00009978*/ MOVW        R1, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2584] 
    /*0000997c*/ MOVT        R1, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2584] 
    /*00009980*/ MOVNE       R2, #0 
    /*00009984*/ MOVEQ       R2, R1 
    AI_DRV_ProcShow_x9988:
    /*00009988*/ LDR         R1, [R4, #40]               @ 0x28 
    /*0000998c*/ CMP         R1, #0 
    /*00009990*/ MOVWEQ      R0, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x258c] 
    /*00009994*/ MOVTEQ      R0, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x258c] 
    /*00009998*/ BEQ         AI_DRV_ProcShow_x99b4
    /*0000999c*/ CMP         R1, #1 
    /*000099a0*/ BEQ         AI_DRV_ProcShow_x9c98
    /*000099a4*/ CMP         R1, #2 
    /*000099a8*/ MOVW        R0, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x259c] 
    /*000099ac*/ MOVT        R0, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x259c] 
    /*000099b0*/ MOVNE       R0, #0 
    AI_DRV_ProcShow_x99b4:
    /*000099b4*/ LDR         R1, [R4, #48]               @ 0x30 
    /*000099b8*/ LDR         R7, [R4, #64]               @ 0x40 
    /*000099bc*/ CMP         R1, #0 
    /*000099c0*/ LDR         R8, [R4, #68]               @ 0x44 
    /*000099c4*/ MOVWEQ      R1, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x25a4] 
    /*000099c8*/ MOVTEQ      R1, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x25a4] 
    /*000099cc*/ BEQ         AI_DRV_ProcShow_x99e0
    /*000099d0*/ CMP         R1, #1 
    /*000099d4*/ MOVW        R1, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x25ac] 
    /*000099d8*/ MOVT        R1, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x25ac] 
    /*000099dc*/ MOVNE       R1, #0 
    AI_DRV_ProcShow_x99e0:
    /*000099e0*/ LDR         R6, [R4, #56]               @ 0x38 
    /*000099e4*/ LDR         LR, [R4, #52]               @ 0x34 
    /*000099e8*/ LDR         IP, [R4, #60]               @ 0x3c 
    /*000099ec*/ STR         R1, [SP, #16] 
    /*000099f0*/ MOVW        R1, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2684] 
    /*000099f4*/ STR         R0, [SP, #4] 
    /*000099f8*/ MOVT        R1, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2684] 
    /*000099fc*/ STR         R2, [SP] 
    /*00009a00*/ MOV         R0, R5 
    /*00009a04*/ STR         R8, [SP, #12] 
    /*00009a08*/ MOV         R2, #0 
    /*00009a0c*/ STR         R7, [SP, #8] 
    /*00009a10*/ STR         R6, [SP, #28] 
    /*00009a14*/ STR         LR, [SP, #24] 
    /*00009a18*/ STR         IP, [SP, #20] 
    /*00009a1c*/ BL          0x0000000000009A1C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    AI_DRV_ProcShow_x9a20:
    /*00009a20*/ MOVW        R1, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x26a4] 
    /*00009a24*/ MOV         R0, R5 
    /*00009a28*/ MOVT        R1, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x26a4] 
    /*00009a2c*/ BL          0x0000000000009A2C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009a30*/ MOVW        R2, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2720] 
    /*00009a34*/ MOVW        R3, #0                      @ .LC58 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2728] 
    /*00009a38*/ MOVT        R2, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2720] 
    /*00009a3c*/ MOVT        R3, #0                      @ .LC58 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2728] 
    /*00009a40*/ STR         R2, [SP, #20] 
    /*00009a44*/ STR         R3, [SP, #16] 
    /*00009a48*/ MOVW        R2, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2730] 
    /*00009a4c*/ MOVW        R3, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x273c] 
    /*00009a50*/ MOVT        R2, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2730] 
    /*00009a54*/ MOVT        R3, #0                      @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0x273c] 
    /*00009a58*/ STR         R2, [SP, #12] 
    /*00009a5c*/ STR         R3, [SP, #8] 
    /*00009a60*/ MOVW        R2, #0                      @ .LC61 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2744] 
    /*00009a64*/ MOVW        R3, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x274c] 
    /*00009a68*/ MOVT        R2, #0                      @ .LC61 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2744] 
    /*00009a6c*/ MOVT        R3, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0x274c] 
    /*00009a70*/ STR         R2, [SP, #4] 
    /*00009a74*/ STR         R3, [SP] 
    /*00009a78*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2618] 
    /*00009a7c*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x26f8] 
    /*00009a80*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2618] 
    /*00009a84*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x26f8] 
    /*00009a88*/ MOVW        R1, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2700] 
    /*00009a8c*/ MOV         R0, R5 
    /*00009a90*/ MOVT        R1, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2700] 
    /*00009a94*/ BL          0x0000000000009A94          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009a98*/ LDR         R3, [R4, #76]               @ 0x4c 
    /*00009a9c*/ CMP         R3, #0 
    /*00009aa0*/ BNE         AI_DRV_ProcShow_x9c20
    AI_DRV_ProcShow_x9aa4:
    /*00009aa4*/ MOVW        R1, #0                      @ .LC64 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2774] 
    /*00009aa8*/ MOV         R0, R5 
    /*00009aac*/ MOVT        R1, #0                      @ .LC64 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2774] 
    /*00009ab0*/ BL          0x0000000000009AB0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009ab4*/ MOVW        R2, #0                      @ .LC67 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x27ec] 
    /*00009ab8*/ MOVW        R3, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x27f4] 
    /*00009abc*/ MOVT        R2, #0                      @ .LC67 [R_ARM_MOVT_ABS: .rodata.str1.4+0x27ec] 
    /*00009ac0*/ MOVT        R3, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0x27f4] 
    /*00009ac4*/ STR         R2, [SP, #12] 
    /*00009ac8*/ STR         R3, [SP, #8] 
    /*00009acc*/ MOVW        R2, #0                      @ .LC69 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x27fc] 
    /*00009ad0*/ MOVW        R3, #0                      @ .LC70 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2804] 
    /*00009ad4*/ MOVT        R2, #0                      @ .LC69 [R_ARM_MOVT_ABS: .rodata.str1.4+0x27fc] 
    /*00009ad8*/ MOVT        R3, #0                      @ .LC70 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2804] 
    /*00009adc*/ STR         R2, [SP, #4] 
    /*00009ae0*/ STR         R3, [SP] 
    /*00009ae4*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2618] 
    /*00009ae8*/ MOVW        R3, #0                      @ .LC65 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x27c8] 
    /*00009aec*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2618] 
    /*00009af0*/ MOVT        R3, #0                      @ .LC65 [R_ARM_MOVT_ABS: .rodata.str1.4+0x27c8] 
    /*00009af4*/ MOVW        R1, #0                      @ .LC66 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x27d4] 
    /*00009af8*/ MOV         R0, R5 
    /*00009afc*/ MOVT        R1, #0                      @ .LC66 [R_ARM_MOVT_ABS: .rodata.str1.4+0x27d4] 
    /*00009b00*/ BL          0x0000000000009B00          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009b04*/ LDR         R3, [R4, #76]               @ 0x4c 
    /*00009b08*/ CMP         R3, #0 
    /*00009b0c*/ BNE         AI_DRV_ProcShow_x9be4
    AI_DRV_ProcShow_x9b10:
    /*00009b10*/ MOVW        R1, #0                      @ .LC72 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2828] 
    /*00009b14*/ MOV         R0, R5 
    /*00009b18*/ MOVT        R1, #0                      @ .LC72 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2828] 
    /*00009b1c*/ BL          0x0000000000009B1C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009b20*/ MOVW        R2, #0                      @ .LC75 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2894] 
    /*00009b24*/ MOVW        R3, #0                      @ .LC76 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x289c] 
    /*00009b28*/ MOVT        R2, #0                      @ .LC75 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2894] 
    /*00009b2c*/ MOVT        R3, #0                      @ .LC76 [R_ARM_MOVT_ABS: .rodata.str1.4+0x289c] 
    /*00009b30*/ STR         R2, [SP, #4] 
    /*00009b34*/ STR         R3, [SP] 
    /*00009b38*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2618] 
    /*00009b3c*/ MOVW        R3, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x287c] 
    /*00009b40*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2618] 
    /*00009b44*/ MOVT        R3, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x287c] 
    /*00009b48*/ MOVW        R1, #0                      @ .LC74 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2884] 
    /*00009b4c*/ MOV         R0, R5 
    /*00009b50*/ MOVT        R1, #0                      @ .LC74 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2884] 
    /*00009b54*/ BL          0x0000000000009B54          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009b58*/ LDR         R3, [R4, #76]               @ 0x4c 
    /*00009b5c*/ CMP         R3, #0 
    /*00009b60*/ BEQ         AI_DRV_ProcShow_x9ba4
    /*00009b64*/ LDR         R2, [R4, #152]              @ 0x98 
    /*00009b68*/ LDR         R3, [R4, #144]              @ 0x90 
    /*00009b6c*/ CMP         R2, #1 
    /*00009b70*/ BEQ         AI_DRV_ProcShow_x9c68
    /*00009b74*/ CMP         R2, #0 
    /*00009b78*/ MOVW        R2, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x25b8] 
    /*00009b7c*/ MOVT        R2, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x25b8] 
    /*00009b80*/ MOVNE       R2, #0 
    AI_DRV_ProcShow_x9b84:
    /*00009b84*/ LDR         IP, [R4, #168]              @ 0xa8 
    /*00009b88*/ MOVW        R1, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28a4] 
    /*00009b8c*/ STR         R2, [SP] 
    /*00009b90*/ MOV         R0, R5 
    /*00009b94*/ MOV         R2, #0 
    /*00009b98*/ MOVT        R1, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28a4] 
    /*00009b9c*/ STR         IP, [SP, #4] 
    /*00009ba0*/ BL          0x0000000000009BA0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    AI_DRV_ProcShow_x9ba4:
    /*00009ba4*/ MOV         R0, #0 
    /*00009ba8*/ ADD         SP, SP, #32 
    /*00009bac*/ POP         {R4, R5, R6, R7, R8, PC} 
    AI_DRV_ProcShow_x9bb0:
    /*00009bb0*/ CMP         R3, #1 
    /*00009bb4*/ BEQ         AI_DRV_ProcShow_x9c80
    /*00009bb8*/ CMP         R3, #2 
    /*00009bbc*/ BEQ         AI_DRV_ProcShow_x9cbc
    /*00009bc0*/ CMP         R3, #3 
    /*00009bc4*/ BEQ         AI_DRV_ProcShow_x9cd4
    /*00009bc8*/ CMP         R3, #4 
    /*00009bcc*/ BEQ         AI_DRV_ProcShow_x9cf8
    /*00009bd0*/ CMP         R3, #5 
    /*00009bd4*/ MOVW        R3, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x252c] 
    /*00009bd8*/ MOVT        R3, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x252c] 
    /*00009bdc*/ MOVNE       R3, #0 
    /*00009be0*/ B           AI_DRV_ProcShow_x9900
    AI_DRV_ProcShow_x9be4:
    /*00009be4*/ LDR         R0, [R4, #24] 
    /*00009be8*/ MOVW        R1, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x280c] 
    /*00009bec*/ LDR         LR, [R4, #20] 
    /*00009bf0*/ MOVT        R1, #0                      @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0x280c] 
    /*00009bf4*/ LDR         IP, [R4] 
    /*00009bf8*/ MOV         R2, #0 
    /*00009bfc*/ LDRD        R6, [R4, #8] 
    /*00009c00*/ STR         R0, [SP, #16] 
    /*00009c04*/ MOV         R0, R5 
    /*00009c08*/ LDR         R3, [R4, #100]              @ 0x64 
    /*00009c0c*/ STR         LR, [SP, #12] 
    /*00009c10*/ STR         IP, [SP, #8] 
    /*00009c14*/ STRD        R6, [SP] 
    /*00009c18*/ BL          0x0000000000009C18          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009c1c*/ B           AI_DRV_ProcShow_x9b10
    AI_DRV_ProcShow_x9c20:
    /*00009c20*/ LDR         LR, [R4, #96]               @ 0x60 
    /*00009c24*/ MOVW        R1, #0                      @ .LC63 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2754] 
    /*00009c28*/ LDR         IP, [R4, #116]              @ 0x74 
    /*00009c2c*/ MOVT        R1, #0                      @ .LC63 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2754] 
    /*00009c30*/ LDR         R0, [R4, #88]               @ 0x58 
    /*00009c34*/ MOV         R2, #0 
    /*00009c38*/ LDR         R6, [R4, #92]               @ 0x5c 
    /*00009c3c*/ STR         LR, [SP, #20] 
    /*00009c40*/ STR         IP, [SP, #16] 
    /*00009c44*/ LDR         LR, [R4, #140]              @ 0x8c 
    /*00009c48*/ LDR         IP, [R4, #136]              @ 0x88 
    /*00009c4c*/ STR         R0, [SP, #8] 
    /*00009c50*/ MOV         R0, R5 
    /*00009c54*/ LDR         R3, [R4, #132]              @ 0x84 
    /*00009c58*/ STR         R6, [SP, #12] 
    /*00009c5c*/ STM         SP, {IP, LR} 
    /*00009c60*/ BL          0x0000000000009C60          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009c64*/ B           AI_DRV_ProcShow_x9aa4
    AI_DRV_ProcShow_x9c68:
    /*00009c68*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x25b4] 
    /*00009c6c*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x25b4] 
    /*00009c70*/ B           AI_DRV_ProcShow_x9b84
    AI_DRV_ProcShow_x9c74:
    /*00009c74*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2534] 
    /*00009c78*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2534] 
    /*00009c7c*/ B           AI_DRV_ProcShow_x9988
    AI_DRV_ProcShow_x9c80:
    /*00009c80*/ MOVW        R3, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x250c] 
    /*00009c84*/ MOVT        R3, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x250c] 
    /*00009c88*/ B           AI_DRV_ProcShow_x9900
    AI_DRV_ProcShow_x9c8c:
    /*00009c8c*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x253c] 
    /*00009c90*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x253c] 
    /*00009c94*/ B           AI_DRV_ProcShow_x9988
    AI_DRV_ProcShow_x9c98:
    /*00009c98*/ MOVW        R0, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2594] 
    /*00009c9c*/ MOVT        R0, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2594] 
    /*00009ca0*/ B           AI_DRV_ProcShow_x99b4
    AI_DRV_ProcShow_x9ca4:
    /*00009ca4*/ MOVW        R2, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2544] 
    /*00009ca8*/ MOVT        R2, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2544] 
    /*00009cac*/ B           AI_DRV_ProcShow_x9988
    AI_DRV_ProcShow_x9cb0:
    /*00009cb0*/ MOVW        R2, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x254c] 
    /*00009cb4*/ MOVT        R2, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x254c] 
    /*00009cb8*/ B           AI_DRV_ProcShow_x9988
    AI_DRV_ProcShow_x9cbc:
    /*00009cbc*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2514] 
    /*00009cc0*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2514] 
    /*00009cc4*/ B           AI_DRV_ProcShow_x9900
    AI_DRV_ProcShow_x9cc8:
    /*00009cc8*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2554] 
    /*00009ccc*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2554] 
    /*00009cd0*/ B           AI_DRV_ProcShow_x9988
    AI_DRV_ProcShow_x9cd4:
    /*00009cd4*/ MOVW        R3, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x251c] 
    /*00009cd8*/ MOVT        R3, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x251c] 
    /*00009cdc*/ B           AI_DRV_ProcShow_x9900
    AI_DRV_ProcShow_x9ce0:
    /*00009ce0*/ MOVW        R2, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x255c] 
    /*00009ce4*/ MOVT        R2, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x255c] 
    /*00009ce8*/ B           AI_DRV_ProcShow_x9988
    AI_DRV_ProcShow_x9cec:
    /*00009cec*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2564] 
    /*00009cf0*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2564] 
    /*00009cf4*/ B           AI_DRV_ProcShow_x9988
    AI_DRV_ProcShow_x9cf8:
    /*00009cf8*/ MOVW        R3, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2524] 
    /*00009cfc*/ MOVT        R3, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2524] 
    /*00009d00*/ B           AI_DRV_ProcShow_x9900
    AI_DRV_ProcShow_x9d04:
    /*00009d04*/ MOVW        R2, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x256c] 
    /*00009d08*/ MOVT        R2, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x256c] 
    /*00009d0c*/ B           AI_DRV_ProcShow_x9988
    AI_DRV_ProcShow_x9d10:
    /*00009d10*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2574] 
    /*00009d14*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2574] 
    /*00009d18*/ B           AI_DRV_ProcShow_x9988
    AI_DRV_ProcShow_x9d1c:
    /*00009d1c*/ MOVW        R2, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x257c] 
    /*00009d20*/ MOVT        R2, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x257c] 
    /*00009d24*/ B           AI_DRV_ProcShow_x9988
FUNC_END AI_DRV_ProcShow


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .init.text
@ Size: 0x10
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN ai_mod_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ BL          0x0000000000000004          @ AI_ModInit [R_ARM_CALL: .text+0x36d4] 
    /*00000008*/ MOV         R0, #0 
    /*0000000c*/ POP         {R4, PC} 
FUNC_END ai_mod_init




@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .exit.text
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN ai_mod_exit
    ai_mod_exit_x0:
    /*00000000*/ B           ai_mod_exit_x0              @ AI_ModExit [R_ARM_JUMP24: .text+0x37fc] 
FUNC_END ai_mod_exit




