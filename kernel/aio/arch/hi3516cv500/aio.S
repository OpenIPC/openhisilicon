@
@ -- FILES --
@

@ aiao_hal.c
@ aio_init.c
@ hi3516cv500_aio.mod.c


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text
@ Size: 0x3a78
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN AIAO_Exit
    /*00000000*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000004*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000008*/ LDR         R3, [R1] 
    /*0000000c*/ CMP         R3, #2 
    /*00000010*/ BXEQ        LR 
    /*00000014*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000018*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000001c*/ MOV         R2, #0 
    /*00000020*/ MOV         R0, #2 
    /*00000024*/ STR         R2, [R3, #4] 
    /*00000028*/ STR         R2, [R3, #8] 
    /*0000002c*/ STR         R2, [R3] 
    /*00000030*/ STR         R2, [R3, #96]               @ 0x60 
    /*00000034*/ STR         R2, [R3, #100]              @ 0x64 
    /*00000038*/ STR         R2, [R3, #92]               @ 0x5c 
    /*0000003c*/ STR         R0, [R1] 
    /*00000040*/ BX          LR 
FUNC_END AIAO_Exit


FUNC_BEGIN AIAO_Notify
    /*00000044*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000048*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*0000004c*/ MOV         R2, #1 
    /*00000050*/ STR         R2, [R3] 
    /*00000054*/ BX          LR 
FUNC_END AIAO_Notify


FUNC_BEGIN AIAO_GetVerMagic
    /*00000058*/ MOVW        R0, #61479                  @ 0xf027 
    /*0000005c*/ MOVT        R0, #307                    @ 0x133 
    /*00000060*/ BX          LR 
FUNC_END AIAO_GetVerMagic


FUNC_BEGIN AIP_HAL_SetAiISR
    /*00000064*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000068*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000006c*/ STR         R0, [R3, #184]              @ 0xb8 
    /*00000070*/ BX          LR 
FUNC_END AIP_HAL_SetAiISR


FUNC_BEGIN AOP_HAL_SetAoISR
    /*00000074*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000078*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000007c*/ STR         R0, [R3, #188]              @ 0xbc 
    /*00000080*/ BX          LR 
FUNC_END AOP_HAL_SetAoISR


FUNC_BEGIN AioInterruptRoute
    /*00000084*/ PUSH        {R4, R5, R6, LR} 
    /*00000088*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000008c*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000090*/ LDR         R3, [R5, #192]              @ 0xc0 
    /*00000094*/ LDR         R4, [R3, #4] 
    /*00000098*/ TST         R4, #1 
    /*0000009c*/ BNE         AioInterruptRoute_x104
    AioInterruptRoute_xa0:
    /*000000a0*/ TST         R4, #2 
    /*000000a4*/ BNE         AioInterruptRoute_xf4
    AioInterruptRoute_xa8:
    /*000000a8*/ TST         R4, #65536                  @ 0x10000 
    /*000000ac*/ BNE         AioInterruptRoute_xe4
    AioInterruptRoute_xb0:
    /*000000b0*/ TST         R4, #131072                 @ 0x20000 
    /*000000b4*/ BNE         AioInterruptRoute_xd4
    AioInterruptRoute_xb8:
    /*000000b8*/ TST         R4, #262144                 @ 0x40000 
    /*000000bc*/ BEQ         AioInterruptRoute_xcc
    /*000000c0*/ LDR         R3, [R5, #188]              @ 0xbc 
    /*000000c4*/ MOV         R0, #2 
    /*000000c8*/ BLX         R3 
    AioInterruptRoute_xcc:
    /*000000cc*/ MOV         R0, #1 
    /*000000d0*/ POP         {R4, R5, R6, PC} 
    AioInterruptRoute_xd4:
    /*000000d4*/ LDR         R3, [R5, #188]              @ 0xbc 
    /*000000d8*/ MOV         R0, #1 
    /*000000dc*/ BLX         R3 
    /*000000e0*/ B           AioInterruptRoute_xb8
    AioInterruptRoute_xe4:
    /*000000e4*/ LDR         R3, [R5, #188]              @ 0xbc 
    /*000000e8*/ MOV         R0, #0 
    /*000000ec*/ BLX         R3 
    /*000000f0*/ B           AioInterruptRoute_xb0
    AioInterruptRoute_xf4:
    /*000000f4*/ LDR         R3, [R5, #184]              @ 0xb8 
    /*000000f8*/ MOV         R0, #1 
    /*000000fc*/ BLX         R3 
    /*00000100*/ B           AioInterruptRoute_xa8
    AioInterruptRoute_x104:
    /*00000104*/ LDR         R3, [R5, #184]              @ 0xb8 
    /*00000108*/ MOV         R0, #0 
    /*0000010c*/ BLX         R3 
    /*00000110*/ B           AioInterruptRoute_xa0
FUNC_END AioInterruptRoute


FUNC_BEGIN aiao_poll
    /*00000114*/ MOV         R0, #0 
    /*00000118*/ BX          LR 
FUNC_END aiao_poll


FUNC_BEGIN AiaoProcShow
    /*0000011c*/ MOV         R0, #0 
    /*00000120*/ BX          LR 
FUNC_END AiaoProcShow


FUNC_BEGIN AIAOGetBclkFsclkDivCfg
    /*00000124*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000128*/ MOV         R7, R2 
    /*0000012c*/ LDR         R4, [R0, #28] 
    /*00000130*/ SUB         SP, SP, #8 
    /*00000134*/ MOV         R8, R3 
    /*00000138*/ CMP         R4, #1 
    /*0000013c*/ BEQ         AIAOGetBclkFsclkDivCfg_x2d0
    /*00000140*/ LDR         R2, [R0, #4] 
    /*00000144*/ CMP         R2, #1 
    /*00000148*/ BEQ         AIAOGetBclkFsclkDivCfg_x2e8
    AIAOGetBclkFsclkDivCfg_x14c:
    /*0000014c*/ MOV         R3, #8 
    /*00000150*/ LSL         R3, R3, R2 
    /*00000154*/ MUL         R4, R4, R3 
    AIAOGetBclkFsclkDivCfg_x158:
    /*00000158*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000015c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000160*/ LDR         R2, [R3, #4] 
    /*00000164*/ CMP         R2, R1 
    /*00000168*/ BEQ         AIAOGetBclkFsclkDivCfg_x2c4
    /*0000016c*/ LDR         R2, [R3, #8] 
    /*00000170*/ CMP         R1, R2 
    /*00000174*/ BEQ         AIAOGetBclkFsclkDivCfg_x2fc
    /*00000178*/ LDR         R3, [R3, #12] 
    /*0000017c*/ CMP         R1, R3 
    /*00000180*/ MOVEQ       R6, #17408                  @ 0x4400 
    /*00000184*/ MOVTEQ      R6, #172                    @ 0xac 
    /*00000188*/ BNE         AIAOGetBclkFsclkDivCfg_x484
    AIAOGetBclkFsclkDivCfg_x18c:
    /*0000018c*/ LDR         R5, [R0] 
    /*00000190*/ MOV         R0, R6 
    /*00000194*/ MUL         R5, R5, R4 
    /*00000198*/ MOV         R1, R5 
    /*0000019c*/ BL          0x000000000000019C          @ __aeabi_uidivmod [R_ARM_CALL: +0x0] 
    /*000001a0*/ CMP         R1, #0 
    /*000001a4*/ BNE         AIAOGetBclkFsclkDivCfg_x4b0
    /*000001a8*/ MOV         R1, R5 
    /*000001ac*/ MOV         R0, R6 
    /*000001b0*/ BL          0x00000000000001B0          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000001b4*/ SUB         R0, R0, #1 
    /*000001b8*/ CMP         R0, #63                     @ 0x3f 
    /*000001bc*/ LDRLS       PC, [PC, R0, LSL #2] 
    /*000001c0*/ B           AIAOGetBclkFsclkDivCfg_x3c8
    /*000001c4*/ ANDEQ       R0, R0, R4, LSR #7 
    /*000001c8*/ @           <UNDEFINED> instruction: 0x000003b0 
    /*000001cc*/ @           <UNDEFINED> instruction: 0x000003bc 
    /*000001d0*/ ANDEQ       R0, R0, R4, ASR #6 
    /*000001d4*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000001d8*/ ANDEQ       R0, R0, R0, ASR R3 
    /*000001dc*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000001e0*/ ANDEQ       R0, R0, IP, ASR R3 
    /*000001e4*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000001e8*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000001ec*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000001f0*/ ANDEQ       R0, R0, R8, ROR #6 
    /*000001f4*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000001f8*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000001fc*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000200*/ ANDEQ       R0, R0, R4, ROR R3 
    /*00000204*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000208*/ ANDEQ       R0, R0, R8, ASR #7 
    /*0000020c*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000210*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000214*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000218*/ ANDEQ       R0, R0, R8, ASR #7 
    /*0000021c*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000220*/ ANDEQ       R0, R0, R0, LSL #7 
    /*00000224*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000228*/ ANDEQ       R0, R0, R8, ASR #7 
    /*0000022c*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000230*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000234*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000238*/ ANDEQ       R0, R0, R8, ASR #7 
    /*0000023c*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000240*/ ANDEQ       R0, R0, IP, LSL #7 
    /*00000244*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000248*/ ANDEQ       R0, R0, R8, ASR #7 
    /*0000024c*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000250*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000254*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000258*/ ANDEQ       R0, R0, R8, ASR #7 
    /*0000025c*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000260*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000264*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000268*/ ANDEQ       R0, R0, R8, ASR #7 
    /*0000026c*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000270*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000274*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000278*/ ANDEQ       R0, R0, R8, ASR #7 
    /*0000027c*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000280*/ MULEQ       R0, R8, R3 
    /*00000284*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000288*/ ANDEQ       R0, R0, R8, ASR #7 
    /*0000028c*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000290*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000294*/ ANDEQ       R0, R0, R8, ASR #7 
    /*00000298*/ ANDEQ       R0, R0, R8, ASR #7 
    /*0000029c*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000002a0*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000002a4*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000002a8*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000002ac*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000002b0*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000002b4*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000002b8*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000002bc*/ ANDEQ       R0, R0, R8, ASR #7 
    /*000002c0*/ ANDEQ       R0, R0, R4, LSL #6 
    AIAOGetBclkFsclkDivCfg_x2c4:
    /*000002c4*/ MOV         R6, #32768                  @ 0x8000 
    /*000002c8*/ MOVT        R6, #187                    @ 0xbb 
    /*000002cc*/ B           AIAOGetBclkFsclkDivCfg_x18c
    AIAOGetBclkFsclkDivCfg_x2d0:
    /*000002d0*/ LDR         R3, [R0, #4] 
    /*000002d4*/ CMP         R3, #1 
    /*000002d8*/ BEQ         AIAOGetBclkFsclkDivCfg_x470
    AIAOGetBclkFsclkDivCfg_x2dc:
    /*000002dc*/ MOV         R4, #16 
    /*000002e0*/ LSL         R4, R4, R3 
    /*000002e4*/ B           AIAOGetBclkFsclkDivCfg_x158
    AIAOGetBclkFsclkDivCfg_x2e8:
    /*000002e8*/ LDR         R3, [R0, #16] 
    /*000002ec*/ CMP         R3, #2 
    /*000002f0*/ LSLEQ       R4, R4, #5 
    /*000002f4*/ BNE         AIAOGetBclkFsclkDivCfg_x14c
    /*000002f8*/ B           AIAOGetBclkFsclkDivCfg_x158
    AIAOGetBclkFsclkDivCfg_x2fc:
    /*000002fc*/ MOV         R6, #8192000                @ 0x7d0000 
    /*00000300*/ B           AIAOGetBclkFsclkDivCfg_x18c
    /*00000304*/ MOV         R3, #11 
    /*00000308*/ STR         R3, [R7] 
    AIAOGetBclkFsclkDivCfg_x30c:
    /*0000030c*/ CMP         R4, #64                     @ 0x40 
    /*00000310*/ BEQ         AIAOGetBclkFsclkDivCfg_x418
    /*00000314*/ BHI         AIAOGetBclkFsclkDivCfg_x3f4
    /*00000318*/ CMP         R4, #16 
    /*0000031c*/ MOVEQ       R0, #0 
    /*00000320*/ STREQ       R0, [R8] 
    /*00000324*/ BEQ         AIAOGetBclkFsclkDivCfg_x33c
    /*00000328*/ CMP         R4, #32 
    /*0000032c*/ MOVEQ       R3, #1 
    /*00000330*/ MOVEQ       R0, #0 
    /*00000334*/ STREQ       R3, [R8] 
    /*00000338*/ BNE         AIAOGetBclkFsclkDivCfg_x42c
    AIAOGetBclkFsclkDivCfg_x33c:
    /*0000033c*/ ADD         SP, SP, #8 
    /*00000340*/ POP         {R4, R5, R6, R7, R8, PC} 
    /*00000344*/ MOV         R3, #3 
    /*00000348*/ STR         R3, [R7] 
    /*0000034c*/ B           AIAOGetBclkFsclkDivCfg_x30c
    /*00000350*/ MOV         R3, #4 
    /*00000354*/ STR         R3, [R7] 
    /*00000358*/ B           AIAOGetBclkFsclkDivCfg_x30c
    /*0000035c*/ MOV         R3, #5 
    /*00000360*/ STR         R3, [R7] 
    /*00000364*/ B           AIAOGetBclkFsclkDivCfg_x30c
    /*00000368*/ MOV         R3, #6 
    /*0000036c*/ STR         R3, [R7] 
    /*00000370*/ B           AIAOGetBclkFsclkDivCfg_x30c
    /*00000374*/ MOV         R3, #7 
    /*00000378*/ STR         R3, [R7] 
    /*0000037c*/ B           AIAOGetBclkFsclkDivCfg_x30c
    /*00000380*/ MOV         R3, #8 
    /*00000384*/ STR         R3, [R7] 
    /*00000388*/ B           AIAOGetBclkFsclkDivCfg_x30c
    /*0000038c*/ MOV         R3, #9 
    /*00000390*/ STR         R3, [R7] 
    /*00000394*/ B           AIAOGetBclkFsclkDivCfg_x30c
    /*00000398*/ MOV         R3, #10 
    /*0000039c*/ STR         R3, [R7] 
    /*000003a0*/ B           AIAOGetBclkFsclkDivCfg_x30c
    /*000003a4*/ MOV         R3, #0 
    /*000003a8*/ STR         R3, [R7] 
    /*000003ac*/ B           AIAOGetBclkFsclkDivCfg_x30c
    /*000003b0*/ MOV         R3, #2 
    /*000003b4*/ STR         R3, [R7] 
    /*000003b8*/ B           AIAOGetBclkFsclkDivCfg_x30c
    /*000003bc*/ MOV         R3, #1 
    /*000003c0*/ STR         R3, [R7] 
    /*000003c4*/ B           AIAOGetBclkFsclkDivCfg_x30c
    AIAOGetBclkFsclkDivCfg_x3c8:
    /*000003c8*/ MOVW        R2, #462                    @ 0x1ce 
    /*000003cc*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000003d0*/ STR         R2, [SP] 
    /*000003d4*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*000003d8*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x74] 
    /*000003dc*/ MOV         R1, #20 
    /*000003e0*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x74] 
    /*000003e4*/ MOV         R0, #3 
    /*000003e8*/ BL          0x00000000000003E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000003ec*/ MVN         R0, #0 
    /*000003f0*/ B           AIAOGetBclkFsclkDivCfg_x33c
    AIAOGetBclkFsclkDivCfg_x3f4:
    /*000003f4*/ CMP         R4, #128                    @ 0x80 
    /*000003f8*/ BEQ         AIAOGetBclkFsclkDivCfg_x45c
    /*000003fc*/ CMP         R4, #256                    @ 0x100 
    /*00000400*/ BNE         AIAOGetBclkFsclkDivCfg_x42c
    /*00000404*/ MOV         R3, #5 
    /*00000408*/ MOV         R0, #0 
    /*0000040c*/ STR         R3, [R8] 
    /*00000410*/ ADD         SP, SP, #8 
    /*00000414*/ POP         {R4, R5, R6, R7, R8, PC} 
    AIAOGetBclkFsclkDivCfg_x418:
    /*00000418*/ MOV         R3, #3 
    /*0000041c*/ MOV         R0, #0 
    /*00000420*/ STR         R3, [R8] 
    /*00000424*/ ADD         SP, SP, #8 
    /*00000428*/ POP         {R4, R5, R6, R7, R8, PC} 
    AIAOGetBclkFsclkDivCfg_x42c:
    /*0000042c*/ MOVW        R2, #495                    @ 0x1ef 
    /*00000430*/ MOV         R1, #20 
    /*00000434*/ STR         R2, [SP] 
    /*00000438*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*0000043c*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb8] 
    /*00000440*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*00000444*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb8] 
    /*00000448*/ MOV         R0, #3 
    /*0000044c*/ BL          0x000000000000044C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000450*/ MVN         R0, #0 
    /*00000454*/ ADD         SP, SP, #8 
    /*00000458*/ POP         {R4, R5, R6, R7, R8, PC} 
    AIAOGetBclkFsclkDivCfg_x45c:
    /*0000045c*/ MOV         R3, #4 
    /*00000460*/ MOV         R0, #0 
    /*00000464*/ STR         R3, [R8] 
    /*00000468*/ ADD         SP, SP, #8 
    /*0000046c*/ POP         {R4, R5, R6, R7, R8, PC} 
    AIAOGetBclkFsclkDivCfg_x470:
    /*00000470*/ LDR         R2, [R0, #16] 
    /*00000474*/ CMP         R2, #2 
    /*00000478*/ MOVEQ       R4, #64                     @ 0x40 
    /*0000047c*/ BNE         AIAOGetBclkFsclkDivCfg_x2dc
    /*00000480*/ B           AIAOGetBclkFsclkDivCfg_x158
    AIAOGetBclkFsclkDivCfg_x484:
    /*00000484*/ MOVW        R2, #410                    @ 0x19a 
    /*00000488*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*0000048c*/ STR         R2, [SP] 
    /*00000490*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*00000494*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*00000498*/ MOV         R1, #20 
    /*0000049c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*000004a0*/ MOV         R0, #3 
    /*000004a4*/ BL          0x00000000000004A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000004a8*/ MVN         R0, #0 
    /*000004ac*/ B           AIAOGetBclkFsclkDivCfg_x33c
    AIAOGetBclkFsclkDivCfg_x4b0:
    /*000004b0*/ MOVW        R2, #418                    @ 0x1a2 
    /*000004b4*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000004b8*/ STR         R2, [SP] 
    /*000004bc*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*000004c0*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x34] 
    /*000004c4*/ MOV         R1, #20 
    /*000004c8*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x34] 
    /*000004cc*/ MOV         R0, #3 
    /*000004d0*/ BL          0x00000000000004D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000004d4*/ MVN         R0, #0 
    /*000004d8*/ B           AIAOGetBclkFsclkDivCfg_x33c
FUNC_END AIAOGetBclkFsclkDivCfg


FUNC_BEGIN AIP_HAL_SetBufferAddr
    /*000004dc*/ PUSH        {R4, R5, R6, LR} 
    /*000004e0*/ SUBS        R4, R0, #0 
    /*000004e4*/ SUB         SP, SP, #8 
    /*000004e8*/ MOV         R6, R2 
    /*000004ec*/ MOV         R5, R3 
    /*000004f0*/ BEQ         AIP_HAL_SetBufferAddr_x534
    /*000004f4*/ MOVW        R3, #539                    @ 0x21b 
    /*000004f8*/ MOV         R1, #20 
    /*000004fc*/ STR         R3, [SP] 
    /*00000500*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00000504*/ LDR         R3, [PC, #88]               @ 0x0000000000000564 
    /*00000508*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*0000050c*/ STR         R4, [SP, #4] 
    /*00000510*/ MOV         R0, #3 
    /*00000514*/ BL          0x0000000000000514          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000518*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*0000051c*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000520*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00000524*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000528*/ MOV         R2, #540                    @ 0x21c 
    /*0000052c*/ LDR         R1, [PC, #48]               @ 0x0000000000000564 
    /*00000530*/ BL          0x0000000000000530          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_SetBufferAddr_x534:
    /*00000534*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000538*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000053c*/ LSL         R3, R4, #8 
    /*00000540*/ LDR         R0, [R2, #192]              @ 0xc0 
    /*00000544*/ ADD         R1, R3, #4224               @ 0x1080 
    /*00000548*/ ADD         R3, R3, #4160               @ 0x1040 
    /*0000054c*/ ADD         R3, R3, #60                 @ 0x3c 
    /*00000550*/ STR         R6, [R0, R1] 
    /*00000554*/ LDR         R2, [R2, #192]              @ 0xc0 
    /*00000558*/ STR         R5, [R2, R3] 
    /*0000055c*/ ADD         SP, SP, #8 
    /*00000560*/ POP         {R4, R5, R6, PC} 
    /*00000564*/ ANDEQ       R0, R0, R8, LSL R0 
FUNC_END AIP_HAL_SetBufferAddr


FUNC_BEGIN AIP_HAL_SetBufferSize
    /*00000568*/ PUSH        {R4, R5, LR} 
    /*0000056c*/ SUBS        R4, R0, #0 
    /*00000570*/ SUB         SP, SP, #12 
    /*00000574*/ MOV         R5, R1 
    /*00000578*/ BEQ         AIP_HAL_SetBufferSize_x5bc
    /*0000057c*/ MOV         R3, #556                    @ 0x22c 
    /*00000580*/ MOV         R1, #20 
    /*00000584*/ STR         R3, [SP] 
    /*00000588*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*0000058c*/ LDR         R3, [PC, #80]               @ 0x00000000000005e4 
    /*00000590*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00000594*/ STR         R4, [SP, #4] 
    /*00000598*/ MOV         R0, #3 
    /*0000059c*/ BL          0x000000000000059C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000005a0*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*000005a4*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*000005a8*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*000005ac*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*000005b0*/ MOVW        R2, #557                    @ 0x22d 
    /*000005b4*/ LDR         R1, [PC, #40]               @ 0x00000000000005e4 
    /*000005b8*/ BL          0x00000000000005B8          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_SetBufferSize_x5bc:
    /*000005bc*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000005c0*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000005c4*/ MOVW        R0, #4228                   @ 0x1084 
    /*000005c8*/ ADD         R0, R0, R4, LSL #8 
    /*000005cc*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*000005d0*/ LDR         R3, [R2, R0] 
    /*000005d4*/ BFI         R3, R5, #0, #24 
    /*000005d8*/ STR         R3, [R2, R0] 
    /*000005dc*/ ADD         SP, SP, #12 
    /*000005e0*/ POP         {R4, R5, PC} 
    /*000005e4*/ ANDEQ       R0, R0, R0, LSR R0 
FUNC_END AIP_HAL_SetBufferSize


FUNC_BEGIN AIP_HAL_SetBuffWptr
    /*000005e8*/ PUSH        {R4, R5, LR} 
    /*000005ec*/ SUBS        R4, R0, #0 
    /*000005f0*/ SUB         SP, SP, #12 
    /*000005f4*/ MOV         R5, R1 
    /*000005f8*/ BEQ         AIP_HAL_SetBuffWptr_x63c
    /*000005fc*/ MOVW        R3, #573                    @ 0x23d 
    /*00000600*/ MOV         R1, #20 
    /*00000604*/ STR         R3, [SP] 
    /*00000608*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*0000060c*/ LDR         R3, [PC, #80]               @ 0x0000000000000664 
    /*00000610*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00000614*/ STR         R4, [SP, #4] 
    /*00000618*/ MOV         R0, #3 
    /*0000061c*/ BL          0x000000000000061C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000620*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000624*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000628*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*0000062c*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000630*/ MOVW        R2, #574                    @ 0x23e 
    /*00000634*/ LDR         R1, [PC, #40]               @ 0x0000000000000664 
    /*00000638*/ BL          0x0000000000000638          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_SetBuffWptr_x63c:
    /*0000063c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000640*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000644*/ MOVW        R0, #4232                   @ 0x1088 
    /*00000648*/ ADD         R0, R0, R4, LSL #8 
    /*0000064c*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00000650*/ LDR         R3, [R2, R0] 
    /*00000654*/ BFI         R3, R5, #0, #24 
    /*00000658*/ STR         R3, [R2, R0] 
    /*0000065c*/ ADD         SP, SP, #12 
    /*00000660*/ POP         {R4, R5, PC} 
    /*00000664*/ ANDEQ       R0, R0, R8, ASR #32 
FUNC_END AIP_HAL_SetBuffWptr


FUNC_BEGIN AIP_HAL_GetBuffWptr
    /*00000668*/ PUSH        {R4, LR} 
    /*0000066c*/ SUBS        R4, R0, #0 
    /*00000670*/ SUB         SP, SP, #8 
    /*00000674*/ BEQ         AIP_HAL_GetBuffWptr_x6b8
    /*00000678*/ MOVW        R3, #593                    @ 0x251 
    /*0000067c*/ MOV         R1, #20 
    /*00000680*/ STR         R3, [SP] 
    /*00000684*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00000688*/ LDR         R3, [PC, #76]               @ 0x00000000000006dc 
    /*0000068c*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00000690*/ STR         R4, [SP, #4] 
    /*00000694*/ MOV         R0, #3 
    /*00000698*/ BL          0x0000000000000698          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000069c*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*000006a0*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*000006a4*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*000006a8*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*000006ac*/ MOVW        R2, #594                    @ 0x252 
    /*000006b0*/ LDR         R1, [PC, #36]               @ 0x00000000000006dc 
    /*000006b4*/ BL          0x00000000000006B4          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_GetBuffWptr_x6b8:
    /*000006b8*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000006bc*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000006c0*/ MOVW        R0, #4232                   @ 0x1088 
    /*000006c4*/ ADD         R0, R0, R4, LSL #8 
    /*000006c8*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*000006cc*/ LDR         R0, [R3, R0] 
    /*000006d0*/ BIC         R0, R0, #-16777216          @ 0xff000000 
    /*000006d4*/ ADD         SP, SP, #8 
    /*000006d8*/ POP         {R4, PC} 
    /*000006dc*/ ANDEQ       R0, R0, IP, ASR R0 
FUNC_END AIP_HAL_GetBuffWptr


FUNC_BEGIN AIP_HAL_SetBuffRptr
    /*000006e0*/ PUSH        {R4, R5, LR} 
    /*000006e4*/ SUBS        R4, R0, #0 
    /*000006e8*/ SUB         SP, SP, #12 
    /*000006ec*/ MOV         R5, R1 
    /*000006f0*/ BEQ         AIP_HAL_SetBuffRptr_x734
    /*000006f4*/ MOVW        R3, #610                    @ 0x262 
    /*000006f8*/ MOV         R1, #20 
    /*000006fc*/ STR         R3, [SP] 
    /*00000700*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00000704*/ LDR         R3, [PC, #80]               @ 0x000000000000075c 
    /*00000708*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*0000070c*/ STR         R4, [SP, #4] 
    /*00000710*/ MOV         R0, #3 
    /*00000714*/ BL          0x0000000000000714          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000718*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*0000071c*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000720*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00000724*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000728*/ MOVW        R2, #611                    @ 0x263 
    /*0000072c*/ LDR         R1, [PC, #40]               @ 0x000000000000075c 
    /*00000730*/ BL          0x0000000000000730          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_SetBuffRptr_x734:
    /*00000734*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000738*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000073c*/ MOVW        R0, #4236                   @ 0x108c 
    /*00000740*/ ADD         R0, R0, R4, LSL #8 
    /*00000744*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00000748*/ LDR         R3, [R2, R0] 
    /*0000074c*/ BFI         R3, R5, #0, #24 
    /*00000750*/ STR         R3, [R2, R0] 
    /*00000754*/ ADD         SP, SP, #12 
    /*00000758*/ POP         {R4, R5, PC} 
    /*0000075c*/ ANDEQ       R0, R0, R0, ROR R0 
FUNC_END AIP_HAL_SetBuffRptr


FUNC_BEGIN AIP_HAL_GetBuffRptr
    /*00000760*/ PUSH        {R4, LR} 
    /*00000764*/ SUBS        R4, R0, #0 
    /*00000768*/ SUB         SP, SP, #8 
    /*0000076c*/ BEQ         AIP_HAL_GetBuffRptr_x7b0
    /*00000770*/ MOVW        R3, #629                    @ 0x275 
    /*00000774*/ MOV         R1, #20 
    /*00000778*/ STR         R3, [SP] 
    /*0000077c*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00000780*/ LDR         R3, [PC, #76]               @ 0x00000000000007d4 
    /*00000784*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00000788*/ STR         R4, [SP, #4] 
    /*0000078c*/ MOV         R0, #3 
    /*00000790*/ BL          0x0000000000000790          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000794*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000798*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*0000079c*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*000007a0*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*000007a4*/ MOVW        R2, #630                    @ 0x276 
    /*000007a8*/ LDR         R1, [PC, #36]               @ 0x00000000000007d4 
    /*000007ac*/ BL          0x00000000000007AC          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_GetBuffRptr_x7b0:
    /*000007b0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000007b4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000007b8*/ MOVW        R0, #4236                   @ 0x108c 
    /*000007bc*/ ADD         R0, R0, R4, LSL #8 
    /*000007c0*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*000007c4*/ LDR         R0, [R3, R0] 
    /*000007c8*/ BIC         R0, R0, #-16777216          @ 0xff000000 
    /*000007cc*/ ADD         SP, SP, #8 
    /*000007d0*/ POP         {R4, PC} 
    /*000007d4*/ ANDEQ       R0, R0, R4, LSL #1 
FUNC_END AIP_HAL_GetBuffRptr


FUNC_BEGIN AIP_HAL_SetTransSize
    /*000007d8*/ PUSH        {R4, R5, LR} 
    /*000007dc*/ SUBS        R4, R0, #0 
    /*000007e0*/ SUB         SP, SP, #12 
    /*000007e4*/ MOV         R5, R1 
    /*000007e8*/ BEQ         AIP_HAL_SetTransSize_x82c
    /*000007ec*/ MOVW        R3, #646                    @ 0x286 
    /*000007f0*/ MOV         R1, #20 
    /*000007f4*/ STR         R3, [SP] 
    /*000007f8*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*000007fc*/ LDR         R3, [PC, #80]               @ 0x0000000000000854 
    /*00000800*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00000804*/ STR         R4, [SP, #4] 
    /*00000808*/ MOV         R0, #3 
    /*0000080c*/ BL          0x000000000000080C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000810*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000814*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000818*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*0000081c*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000820*/ MOVW        R2, #647                    @ 0x287 
    /*00000824*/ LDR         R1, [PC, #40]               @ 0x0000000000000854 
    /*00000828*/ BL          0x0000000000000828          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_SetTransSize_x82c:
    /*0000082c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000830*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000834*/ MOVW        R0, #4244                   @ 0x1094 
    /*00000838*/ ADD         R0, R0, R4, LSL #8 
    /*0000083c*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00000840*/ LDR         R3, [R2, R0] 
    /*00000844*/ BFI         R3, R5, #0, #24 
    /*00000848*/ STR         R3, [R2, R0] 
    /*0000084c*/ ADD         SP, SP, #12 
    /*00000850*/ POP         {R4, R5, PC} 
    /*00000854*/ MULEQ       R0, R8, R0 
FUNC_END AIP_HAL_SetTransSize


FUNC_BEGIN AIP_HAL_GetTrackMode
    /*00000858*/ PUSH        {R4, R5, LR} 
    /*0000085c*/ SUBS        R4, R0, #0 
    /*00000860*/ SUB         SP, SP, #12 
    /*00000864*/ MOV         R5, R1 
    /*00000868*/ BEQ         AIP_HAL_GetTrackMode_x8ac
    /*0000086c*/ MOVW        R3, #830                    @ 0x33e 
    /*00000870*/ MOV         R1, #20 
    /*00000874*/ STR         R3, [SP] 
    /*00000878*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*0000087c*/ LDR         R3, [PC, #80]               @ 0x00000000000008d4 
    /*00000880*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00000884*/ STR         R4, [SP, #4] 
    /*00000888*/ MOV         R0, #3 
    /*0000088c*/ BL          0x000000000000088C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000890*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000894*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000898*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*0000089c*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*000008a0*/ MOVW        R2, #831                    @ 0x33f 
    /*000008a4*/ LDR         R1, [PC, #40]               @ 0x00000000000008d4 
    /*000008a8*/ BL          0x00000000000008A8          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_GetTrackMode_x8ac:
    /*000008ac*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000008b0*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000008b4*/ ADD         R4, R4, #16 
    /*000008b8*/ MOV         R0, #0 
    /*000008bc*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*000008c0*/ LDR         R3, [R3, R4, LSL #8] 
    /*000008c4*/ UBFX        R3, R3, #16, #3 
    /*000008c8*/ STR         R3, [R5] 
    /*000008cc*/ ADD         SP, SP, #12 
    /*000008d0*/ POP         {R4, R5, PC} 
    /*000008d4*/ STRHEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
FUNC_END AIP_HAL_GetTrackMode


FUNC_BEGIN AIP_HAL_GetClkDir
    /*000008d8*/ PUSH        {R4, R5, LR} 
    /*000008dc*/ SUBS        R4, R0, #0 
    /*000008e0*/ SUB         SP, SP, #12 
    /*000008e4*/ MOV         R5, R1 
    /*000008e8*/ BEQ         AIP_HAL_GetClkDir_x92c
    /*000008ec*/ MOVW        R3, #899                    @ 0x383 
    /*000008f0*/ MOV         R1, #20 
    /*000008f4*/ STR         R3, [SP] 
    /*000008f8*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*000008fc*/ LDR         R3, [PC, #84]               @ 0x0000000000000958 
    /*00000900*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00000904*/ STR         R4, [SP, #4] 
    /*00000908*/ MOV         R0, #3 
    /*0000090c*/ BL          0x000000000000090C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000910*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000914*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000918*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*0000091c*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000920*/ MOV         R2, #900                    @ 0x384 
    /*00000924*/ LDR         R1, [PC, #44]               @ 0x0000000000000958 
    /*00000928*/ BL          0x0000000000000928          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_GetClkDir_x92c:
    /*0000092c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000930*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000934*/ LSL         R4, R4, #3 
    /*00000938*/ MOV         R0, #0 
    /*0000093c*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00000940*/ ADD         R4, R4, #260                @ 0x104 
    /*00000944*/ LDR         R3, [R3, R4] 
    /*00000948*/ UBFX        R3, R3, #13, #1 
    /*0000094c*/ STR         R3, [R5] 
    /*00000950*/ ADD         SP, SP, #12 
    /*00000954*/ POP         {R4, R5, PC} 
    /*00000958*/ ANDEQ       R0, R0, R8, ASR #1 
FUNC_END AIP_HAL_GetClkDir


FUNC_BEGIN AOP_HAL_GetTrackMode
    /*0000095c*/ CMP         R0, #1 
    /*00000960*/ PUSH        {R4, R5, LR} 
    /*00000964*/ MOV         R4, R0 
    /*00000968*/ SUB         SP, SP, #12 
    /*0000096c*/ MOV         R5, R1 
    /*00000970*/ BLS         AOP_HAL_GetTrackMode_x9b4
    /*00000974*/ STR         R0, [SP, #4] 
    /*00000978*/ MOV         R3, #1808                   @ 0x710 
    /*0000097c*/ MOV         R1, #20 
    /*00000980*/ STR         R3, [SP] 
    /*00000984*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00000988*/ LDR         R3, [PC, #76]               @ 0x00000000000009dc 
    /*0000098c*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00000990*/ MOV         R0, #3 
    /*00000994*/ BL          0x0000000000000994          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000998*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*0000099c*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*000009a0*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*000009a4*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*000009a8*/ MOVW        R2, #1809                   @ 0x711 
    /*000009ac*/ LDR         R1, [PC, #40]               @ 0x00000000000009dc 
    /*000009b0*/ BL          0x00000000000009B0          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_GetTrackMode_x9b4:
    /*000009b4*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000009b8*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000009bc*/ ADD         R4, R4, #32 
    /*000009c0*/ MOV         R0, #0 
    /*000009c4*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*000009c8*/ LDR         R3, [R3, R4, LSL #8] 
    /*000009cc*/ UBFX        R3, R3, #16, #3 
    /*000009d0*/ STR         R3, [R5] 
    /*000009d4*/ ADD         SP, SP, #12 
    /*000009d8*/ POP         {R4, R5, PC} 
    /*000009dc*/ LDRDEQ      R0, [R0], -IP 
FUNC_END AOP_HAL_GetTrackMode


FUNC_BEGIN AOP_HAL_GetClkDir
    /*000009e0*/ CMP         R0, #1 
    /*000009e4*/ PUSH        {R4, R5, LR} 
    /*000009e8*/ MOV         R4, R0 
    /*000009ec*/ SUB         SP, SP, #12 
    /*000009f0*/ MOV         R5, R1 
    /*000009f4*/ BLS         AOP_HAL_GetClkDir_xa38
    /*000009f8*/ STR         R0, [SP, #4] 
    /*000009fc*/ MOVW        R3, #1877                   @ 0x755 
    /*00000a00*/ MOV         R1, #20 
    /*00000a04*/ STR         R3, [SP] 
    /*00000a08*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00000a0c*/ LDR         R3, [PC, #80]               @ 0x0000000000000a64 
    /*00000a10*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00000a14*/ MOV         R0, #3 
    /*00000a18*/ BL          0x0000000000000A18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000a1c*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000a20*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000a24*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00000a28*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000a2c*/ MOVW        R2, #1878                   @ 0x756 
    /*00000a30*/ LDR         R1, [PC, #44]               @ 0x0000000000000a64 
    /*00000a34*/ BL          0x0000000000000A34          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_GetClkDir_xa38:
    /*00000a38*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000a3c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000a40*/ LSL         R4, R4, #3 
    /*00000a44*/ MOV         R0, #0 
    /*00000a48*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00000a4c*/ ADD         R4, R4, #324                @ 0x144 
    /*00000a50*/ LDR         R3, [R3, R4] 
    /*00000a54*/ UBFX        R3, R3, #13, #1 
    /*00000a58*/ STR         R3, [R5] 
    /*00000a5c*/ ADD         SP, SP, #12 
    /*00000a60*/ POP         {R4, R5, PC} 
    /*00000a64*/ STRDEQ      R0, [R0], -R4 
FUNC_END AOP_HAL_GetClkDir


FUNC_BEGIN AOP_HAL_GetVolume
    /*00000a68*/ CMP         R0, #1 
    /*00000a6c*/ PUSH        {R4, R5, LR} 
    /*00000a70*/ MOV         R4, R0 
    /*00000a74*/ SUB         SP, SP, #12 
    /*00000a78*/ MOV         R5, R1 
    /*00000a7c*/ BLS         AOP_HAL_GetVolume_xac0
    /*00000a80*/ STR         R0, [SP, #4] 
    /*00000a84*/ MOVW        R3, #1922                   @ 0x782 
    /*00000a88*/ MOV         R1, #20 
    /*00000a8c*/ STR         R3, [SP] 
    /*00000a90*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00000a94*/ LDR         R3, [PC, #84]               @ 0x0000000000000af0 
    /*00000a98*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00000a9c*/ MOV         R0, #3 
    /*00000aa0*/ BL          0x0000000000000AA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000aa4*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000aa8*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000aac*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00000ab0*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000ab4*/ MOVW        R2, #1923                   @ 0x783 
    /*00000ab8*/ LDR         R1, [PC, #48]               @ 0x0000000000000af0 
    /*00000abc*/ BL          0x0000000000000ABC          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_GetVolume_xac0:
    /*00000ac0*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000ac4*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000ac8*/ MOVW        R3, #8196                   @ 0x2004 
    /*00000acc*/ ADD         R3, R3, R4, LSL #8 
    /*00000ad0*/ LDR         R2, [R2, #192]              @ 0xc0 
    /*00000ad4*/ MOV         R0, #0 
    /*00000ad8*/ LDR         R3, [R2, R3] 
    /*00000adc*/ UBFX        R3, R3, #8, #7 
    /*00000ae0*/ SUB         R3, R3, #121                @ 0x79 
    /*00000ae4*/ STR         R3, [R5] 
    /*00000ae8*/ ADD         SP, SP, #12 
    /*00000aec*/ POP         {R4, R5, PC} 
    /*00000af0*/ ANDEQ       R0, R0, R8, LSL #2 
FUNC_END AOP_HAL_GetVolume


FUNC_BEGIN AOP_HAL_SetMute
    /*00000af4*/ CMP         R0, #1 
    /*00000af8*/ PUSH        {R4, R5, R6, LR} 
    /*00000afc*/ MOV         R5, R0 
    /*00000b00*/ SUB         SP, SP, #8 
    /*00000b04*/ MOV         R6, R1 
    /*00000b08*/ MOV         R4, R2 
    /*00000b0c*/ BLS         AOP_HAL_SetMute_xb50
    /*00000b10*/ STR         R0, [SP, #4] 
    /*00000b14*/ MOVW        R3, #1940                   @ 0x794 
    /*00000b18*/ MOV         R1, #20 
    /*00000b1c*/ STR         R3, [SP] 
    /*00000b20*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00000b24*/ LDR         R3, [PC, #252]              @ 0x0000000000000c28 
    /*00000b28*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00000b2c*/ MOV         R0, #3 
    /*00000b30*/ BL          0x0000000000000B30          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000b34*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000b38*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000b3c*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00000b40*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000b44*/ MOVW        R2, #1941                   @ 0x795 
    /*00000b48*/ LDR         R1, [PC, #216]              @ 0x0000000000000c28 
    /*00000b4c*/ BL          0x0000000000000B4C          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_SetMute_xb50:
    /*00000b50*/ CMP         R6, #1 
    /*00000b54*/ STRHI       R5, [SP, #4] 
    /*00000b58*/ MOVWHI      R3, #1946                   @ 0x79a 
    /*00000b5c*/ BHI         AOP_HAL_SetMute_xbfc
    /*00000b60*/ CMP         R4, #0 
    /*00000b64*/ BEQ         AOP_HAL_SetMute_xbd0
    /*00000b68*/ LDR         R2, [R4] 
    /*00000b6c*/ CMP         R2, #1 
    /*00000b70*/ STRHI       R5, [SP, #4] 
    /*00000b74*/ MOVWHI      R3, #1954                   @ 0x7a2 
    /*00000b78*/ BHI         AOP_HAL_SetMute_xbfc
    /*00000b7c*/ LDR         R1, [R4, #4] 
    /*00000b80*/ CMP         R1, #7 
    /*00000b84*/ BHI         AOP_HAL_SetMute_xbf4
    /*00000b88*/ LDR         IP, [R4, #8] 
    /*00000b8c*/ CMP         IP, #7 
    /*00000b90*/ BHI         AOP_HAL_SetMute_xbf4
    /*00000b94*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000b98*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000b9c*/ MOVW        R0, #8196                   @ 0x2004 
    /*00000ba0*/ ADD         R0, R0, R5, LSL #8 
    /*00000ba4*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00000ba8*/ ADD         LR, R3, R0 
    /*00000bac*/ LDR         R3, [R3, R0] 
    /*00000bb0*/ BFI         R3, R6, #0, #1 
    /*00000bb4*/ BFI         R3, R2, #1, #1 
    /*00000bb8*/ BFI         R3, R1, #16, #4 
    /*00000bbc*/ BFI         R3, IP, #20, #4 
    AOP_HAL_SetMute_xbc0:
    /*00000bc0*/ MOV         R0, #0 
    /*00000bc4*/ STR         R3, [LR] 
    /*00000bc8*/ ADD         SP, SP, #8 
    /*00000bcc*/ POP         {R4, R5, R6, PC} 
    AOP_HAL_SetMute_xbd0:
    /*00000bd0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000bd4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000bd8*/ MOVW        R0, #8196                   @ 0x2004 
    /*00000bdc*/ ADD         R0, R0, R5, LSL #8 
    /*00000be0*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00000be4*/ ADD         LR, R3, R0 
    /*00000be8*/ LDR         R3, [R3, R0] 
    /*00000bec*/ BFI         R3, R6, #0, #1 
    /*00000bf0*/ B           AOP_HAL_SetMute_xbc0
    AOP_HAL_SetMute_xbf4:
    /*00000bf4*/ MOVW        R3, #1960                   @ 0x7a8 
    /*00000bf8*/ STR         R5, [SP, #4] 
    AOP_HAL_SetMute_xbfc:
    /*00000bfc*/ STR         R3, [SP] 
    /*00000c00*/ MOV         R1, #20 
    /*00000c04*/ LDR         R3, [PC, #28]               @ 0x0000000000000c28 
    /*00000c08*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a0] 
    /*00000c0c*/ MOV         R0, #3 
    /*00000c10*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a0] 
    /*00000c14*/ BL          0x0000000000000C14          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000c18*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000c1c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00000c20*/ ADD         SP, SP, #8 
    /*00000c24*/ POP         {R4, R5, R6, PC} 
    /*00000c28*/ ANDEQ       R0, R0, IP, LSL R1 
FUNC_END AOP_HAL_SetMute


FUNC_BEGIN AOP_HAL_GetMute
    /*00000c2c*/ CMP         R0, #1 
    /*00000c30*/ PUSH        {R4, R5, R6, LR} 
    /*00000c34*/ MOV         R5, R0 
    /*00000c38*/ SUB         SP, SP, #8 
    /*00000c3c*/ MOV         R6, R1 
    /*00000c40*/ MOV         R4, R2 
    /*00000c44*/ BLS         AOP_HAL_GetMute_xc88
    /*00000c48*/ STR         R0, [SP, #4] 
    /*00000c4c*/ MOVW        R3, #1987                   @ 0x7c3 
    /*00000c50*/ MOV         R1, #20 
    /*00000c54*/ STR         R3, [SP] 
    /*00000c58*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00000c5c*/ LDR         R3, [PC, #104]              @ 0x0000000000000ccc 
    /*00000c60*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00000c64*/ MOV         R0, #3 
    /*00000c68*/ BL          0x0000000000000C68          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000c6c*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000c70*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000c74*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00000c78*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000c7c*/ MOVW        R2, #1988                   @ 0x7c4 
    /*00000c80*/ LDR         R1, [PC, #68]               @ 0x0000000000000ccc 
    /*00000c84*/ BL          0x0000000000000C84          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_GetMute_xc88:
    /*00000c88*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000c8c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000c90*/ MOVW        R0, #8196                   @ 0x2004 
    /*00000c94*/ ADD         R0, R0, R5, LSL #8 
    /*00000c98*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00000c9c*/ CMP         R4, #0 
    /*00000ca0*/ LDR         R3, [R3, R0] 
    /*00000ca4*/ MOV         R0, #0 
    /*00000ca8*/ AND         R2, R3, #1 
    /*00000cac*/ UBFXNE      R1, R3, #1, #1 
    /*00000cb0*/ STR         R2, [R6] 
    /*00000cb4*/ UBFXNE      R2, R3, #16, #4 
    /*00000cb8*/ STRNE       R1, [R4] 
    /*00000cbc*/ UBFXNE      R3, R3, #20, #4 
    /*00000cc0*/ STMIBNE     R4, {R2, R3} 
    /*00000cc4*/ ADD         SP, SP, #8 
    /*00000cc8*/ POP         {R4, R5, R6, PC} 
    /*00000ccc*/ ANDEQ       R0, R0, IP, LSR #2 
FUNC_END AOP_HAL_GetMute


FUNC_BEGIN AOP_HAL_GetBuffWptr
    /*00000cd0*/ CMP         R0, #1 
    /*00000cd4*/ PUSH        {R4, LR} 
    /*00000cd8*/ MOV         R4, R0 
    /*00000cdc*/ SUB         SP, SP, #8 
    /*00000ce0*/ BLS         AOP_HAL_GetBuffWptr_xd24
    /*00000ce4*/ STR         R0, [SP, #4] 
    /*00000ce8*/ MOVW        R3, #1544                   @ 0x608 
    /*00000cec*/ MOV         R1, #20 
    /*00000cf0*/ STR         R3, [SP] 
    /*00000cf4*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00000cf8*/ LDR         R3, [PC, #72]               @ 0x0000000000000d48 
    /*00000cfc*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00000d00*/ MOV         R0, #3 
    /*00000d04*/ BL          0x0000000000000D04          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000d08*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000d0c*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000d10*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00000d14*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000d18*/ MOVW        R2, #1545                   @ 0x609 
    /*00000d1c*/ LDR         R1, [PC, #36]               @ 0x0000000000000d48 
    /*00000d20*/ BL          0x0000000000000D20          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_GetBuffWptr_xd24:
    /*00000d24*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000d28*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000d2c*/ MOVW        R0, #8328                   @ 0x2088 
    /*00000d30*/ ADD         R0, R0, R4, LSL #8 
    /*00000d34*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00000d38*/ LDR         R0, [R3, R0] 
    /*00000d3c*/ BIC         R0, R0, #-16777216          @ 0xff000000 
    /*00000d40*/ ADD         SP, SP, #8 
    /*00000d44*/ POP         {R4, PC} 
    /*00000d48*/ ANDEQ       R0, R0, IP, LSR R1 
FUNC_END AOP_HAL_GetBuffWptr


FUNC_BEGIN AOP_HAL_GetBuffRptr
    /*00000d4c*/ CMP         R0, #1 
    /*00000d50*/ PUSH        {R4, LR} 
    /*00000d54*/ MOV         R4, R0 
    /*00000d58*/ SUB         SP, SP, #8 
    /*00000d5c*/ BLS         AOP_HAL_GetBuffRptr_xda0
    /*00000d60*/ STR         R0, [SP, #4] 
    /*00000d64*/ MOVW        R3, #1563                   @ 0x61b 
    /*00000d68*/ MOV         R1, #20 
    /*00000d6c*/ STR         R3, [SP] 
    /*00000d70*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00000d74*/ LDR         R3, [PC, #72]               @ 0x0000000000000dc4 
    /*00000d78*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00000d7c*/ MOV         R0, #3 
    /*00000d80*/ BL          0x0000000000000D80          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000d84*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000d88*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000d8c*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00000d90*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000d94*/ MOVW        R2, #1564                   @ 0x61c 
    /*00000d98*/ LDR         R1, [PC, #36]               @ 0x0000000000000dc4 
    /*00000d9c*/ BL          0x0000000000000D9C          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_GetBuffRptr_xda0:
    /*00000da0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000da4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000da8*/ MOVW        R0, #8332                   @ 0x208c 
    /*00000dac*/ ADD         R0, R0, R4, LSL #8 
    /*00000db0*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00000db4*/ LDR         R0, [R3, R0] 
    /*00000db8*/ BIC         R0, R0, #-16777216          @ 0xff000000 
    /*00000dbc*/ ADD         SP, SP, #8 
    /*00000dc0*/ POP         {R4, PC} 
    /*00000dc4*/ ANDEQ       R0, R0, R0, ASR R1 
FUNC_END AOP_HAL_GetBuffRptr


FUNC_BEGIN AOP_HAL_SetBufferAddr
    /*00000dc8*/ CMP         R0, #1 
    /*00000dcc*/ PUSH        {R4, R5, R6, LR} 
    /*00000dd0*/ MOV         R4, R0 
    /*00000dd4*/ SUB         SP, SP, #8 
    /*00000dd8*/ MOV         R6, R2 
    /*00000ddc*/ MOV         R5, R3 
    /*00000de0*/ BLS         AOP_HAL_SetBufferAddr_xe24
    /*00000de4*/ STR         R0, [SP, #4] 
    /*00000de8*/ MOVW        R3, #1508                   @ 0x5e4 
    /*00000dec*/ MOV         R1, #20 
    /*00000df0*/ STR         R3, [SP] 
    /*00000df4*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00000df8*/ LDR         R3, [PC, #84]               @ 0x0000000000000e54 
    /*00000dfc*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00000e00*/ MOV         R0, #3 
    /*00000e04*/ BL          0x0000000000000E04          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000e08*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000e0c*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000e10*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00000e14*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000e18*/ MOVW        R2, #1509                   @ 0x5e5 
    /*00000e1c*/ LDR         R1, [PC, #48]               @ 0x0000000000000e54 
    /*00000e20*/ BL          0x0000000000000E20          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_SetBufferAddr_xe24:
    /*00000e24*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000e28*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000e2c*/ LSL         R0, R4, #8 
    /*00000e30*/ LDR         R1, [R3, #192]              @ 0xc0 
    /*00000e34*/ ADD         R2, R0, #8320               @ 0x2080 
    /*00000e38*/ ADD         R0, R0, #8256               @ 0x2040 
    /*00000e3c*/ ADD         R0, R0, #60                 @ 0x3c 
    /*00000e40*/ STR         R6, [R1, R2] 
    /*00000e44*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00000e48*/ STR         R5, [R3, R0] 
    /*00000e4c*/ ADD         SP, SP, #8 
    /*00000e50*/ POP         {R4, R5, R6, PC} 
    /*00000e54*/ ANDEQ       R0, R0, R4, ROR #2 
FUNC_END AOP_HAL_SetBufferAddr


FUNC_BEGIN AOP_HAL_SetBufferSize
    /*00000e58*/ CMP         R0, #1 
    /*00000e5c*/ PUSH        {R4, R5, LR} 
    /*00000e60*/ MOV         R4, R0 
    /*00000e64*/ SUB         SP, SP, #12 
    /*00000e68*/ MOV         R5, R1 
    /*00000e6c*/ BLS         AOP_HAL_SetBufferSize_xeb0
    /*00000e70*/ STR         R0, [SP, #4] 
    /*00000e74*/ MOVW        R3, #1525                   @ 0x5f5 
    /*00000e78*/ MOV         R1, #20 
    /*00000e7c*/ STR         R3, [SP] 
    /*00000e80*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00000e84*/ LDR         R3, [PC, #76]               @ 0x0000000000000ed8 
    /*00000e88*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00000e8c*/ MOV         R0, #3 
    /*00000e90*/ BL          0x0000000000000E90          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000e94*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000e98*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000e9c*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00000ea0*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000ea4*/ MOVW        R2, #1526                   @ 0x5f6 
    /*00000ea8*/ LDR         R1, [PC, #40]               @ 0x0000000000000ed8 
    /*00000eac*/ BL          0x0000000000000EAC          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_SetBufferSize_xeb0:
    /*00000eb0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000eb4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000eb8*/ MOVW        R0, #8324                   @ 0x2084 
    /*00000ebc*/ ADD         R0, R0, R4, LSL #8 
    /*00000ec0*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00000ec4*/ LDR         R3, [R2, R0] 
    /*00000ec8*/ BFI         R3, R5, #0, #24 
    /*00000ecc*/ STR         R3, [R2, R0] 
    /*00000ed0*/ ADD         SP, SP, #12 
    /*00000ed4*/ POP         {R4, R5, PC} 
    /*00000ed8*/ ANDEQ       R0, R0, IP, ROR R1 
FUNC_END AOP_HAL_SetBufferSize


FUNC_BEGIN AOP_HAL_SetBuffWptr
    /*00000edc*/ CMP         R0, #1 
    /*00000ee0*/ PUSH        {R4, R5, LR} 
    /*00000ee4*/ MOV         R4, R0 
    /*00000ee8*/ SUB         SP, SP, #12 
    /*00000eec*/ MOV         R5, R1 
    /*00000ef0*/ BLS         AOP_HAL_SetBuffWptr_xf34
    /*00000ef4*/ STR         R0, [SP, #4] 
    /*00000ef8*/ MOVW        R3, #1580                   @ 0x62c 
    /*00000efc*/ MOV         R1, #20 
    /*00000f00*/ STR         R3, [SP] 
    /*00000f04*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00000f08*/ LDR         R3, [PC, #76]               @ 0x0000000000000f5c 
    /*00000f0c*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00000f10*/ MOV         R0, #3 
    /*00000f14*/ BL          0x0000000000000F14          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000f18*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000f1c*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000f20*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00000f24*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000f28*/ MOVW        R2, #1581                   @ 0x62d 
    /*00000f2c*/ LDR         R1, [PC, #40]               @ 0x0000000000000f5c 
    /*00000f30*/ BL          0x0000000000000F30          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_SetBuffWptr_xf34:
    /*00000f34*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000f38*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000f3c*/ MOVW        R0, #8328                   @ 0x2088 
    /*00000f40*/ ADD         R0, R0, R4, LSL #8 
    /*00000f44*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00000f48*/ LDR         R3, [R2, R0] 
    /*00000f4c*/ BFI         R3, R5, #0, #24 
    /*00000f50*/ STR         R3, [R2, R0] 
    /*00000f54*/ ADD         SP, SP, #12 
    /*00000f58*/ POP         {R4, R5, PC} 
    /*00000f5c*/ MULEQ       R0, R4, R1 
FUNC_END AOP_HAL_SetBuffWptr


FUNC_BEGIN AOP_HAL_SetBuffRptr
    /*00000f60*/ CMP         R0, #1 
    /*00000f64*/ PUSH        {R4, R5, LR} 
    /*00000f68*/ MOV         R4, R0 
    /*00000f6c*/ SUB         SP, SP, #12 
    /*00000f70*/ MOV         R5, R1 
    /*00000f74*/ BLS         AOP_HAL_SetBuffRptr_xfb8
    /*00000f78*/ STR         R0, [SP, #4] 
    /*00000f7c*/ MOVW        R3, #1597                   @ 0x63d 
    /*00000f80*/ MOV         R1, #20 
    /*00000f84*/ STR         R3, [SP] 
    /*00000f88*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00000f8c*/ LDR         R3, [PC, #76]               @ 0x0000000000000fe0 
    /*00000f90*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00000f94*/ MOV         R0, #3 
    /*00000f98*/ BL          0x0000000000000F98          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000f9c*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00000fa0*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00000fa4*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00000fa8*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00000fac*/ MOVW        R2, #1598                   @ 0x63e 
    /*00000fb0*/ LDR         R1, [PC, #40]               @ 0x0000000000000fe0 
    /*00000fb4*/ BL          0x0000000000000FB4          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_SetBuffRptr_xfb8:
    /*00000fb8*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000fbc*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000fc0*/ MOVW        R0, #8332                   @ 0x208c 
    /*00000fc4*/ ADD         R0, R0, R4, LSL #8 
    /*00000fc8*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00000fcc*/ LDR         R3, [R2, R0] 
    /*00000fd0*/ BFI         R3, R5, #0, #24 
    /*00000fd4*/ STR         R3, [R2, R0] 
    /*00000fd8*/ ADD         SP, SP, #12 
    /*00000fdc*/ POP         {R4, R5, PC} 
    /*00000fe0*/ ANDEQ       R0, R0, R8, LSR #3 
FUNC_END AOP_HAL_SetBuffRptr


FUNC_BEGIN AOP_HAL_SetTransSize
    /*00000fe4*/ CMP         R0, #1 
    /*00000fe8*/ PUSH        {R4, R5, LR} 
    /*00000fec*/ MOV         R4, R0 
    /*00000ff0*/ SUB         SP, SP, #12 
    /*00000ff4*/ MOV         R5, R1 
    /*00000ff8*/ BLS         AOP_HAL_SetTransSize_x103c
    /*00000ffc*/ STR         R0, [SP, #4] 
    /*00001000*/ MOVW        R3, #1615                   @ 0x64f 
    /*00001004*/ MOV         R1, #20 
    /*00001008*/ STR         R3, [SP] 
    /*0000100c*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00001010*/ LDR         R3, [PC, #76]               @ 0x0000000000001064 
    /*00001014*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00001018*/ MOV         R0, #3 
    /*0000101c*/ BL          0x000000000000101C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001020*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00001024*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00001028*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*0000102c*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00001030*/ MOV         R2, #1616                   @ 0x650 
    /*00001034*/ LDR         R1, [PC, #40]               @ 0x0000000000001064 
    /*00001038*/ BL          0x0000000000001038          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_SetTransSize_x103c:
    /*0000103c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001040*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001044*/ MOVW        R0, #8340                   @ 0x2094 
    /*00001048*/ ADD         R0, R0, R4, LSL #8 
    /*0000104c*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00001050*/ LDR         R3, [R2, R0] 
    /*00001054*/ BFI         R3, R5, #0, #24 
    /*00001058*/ STR         R3, [R2, R0] 
    /*0000105c*/ ADD         SP, SP, #12 
    /*00001060*/ POP         {R4, R5, PC} 
    /*00001064*/ @           <UNDEFINED> instruction: 0x000001bc 
FUNC_END AOP_HAL_SetTransSize


FUNC_BEGIN AIP_HAL_SetClkDir
    /*00001068*/ PUSH        {R4, R5, R6, LR} 
    /*0000106c*/ SUBS        R6, R0, #0 
    /*00001070*/ SUB         SP, SP, #16 
    /*00001074*/ MOV         R4, R1 
    /*00001078*/ BNE         AIP_HAL_SetClkDir_x10f8
    /*0000107c*/ CMP         R4, #1 
    /*00001080*/ BHI         AIP_HAL_SetClkDir_x1140
    AIP_HAL_SetClkDir_x1084:
    /*00001084*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001088*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000108c*/ ADD         R0, R5, #196                @ 0xc4 
    /*00001090*/ BL          0x0000000000001090          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001094*/ CMP         R0, #0 
    /*00001098*/ BNE         AIP_HAL_SetClkDir_x11a8
    /*0000109c*/ MOV         R3, #92                     @ 0x5c 
    /*000010a0*/ MLA         R3, R3, R6, R5 
    /*000010a4*/ LDR         R2, [R3, #4] 
    /*000010a8*/ CMP         R2, #0 
    /*000010ac*/ BEQ         AIP_HAL_SetClkDir_x11b4
    /*000010b0*/ LDR         R2, [R3, #44]               @ 0x2c 
    /*000010b4*/ LSL         R6, R6, #3 
    /*000010b8*/ LDR         R0, [R5, #192]              @ 0xc0 
    /*000010bc*/ ADD         IP, R6, #260                @ 0x104 
    /*000010c0*/ CMP         R2, #1 
    /*000010c4*/ ADD         R2, R0, IP 
    /*000010c8*/ LDR         R1, [R0, IP] 
    /*000010cc*/ BEQ         AIP_HAL_SetClkDir_x1178
    AIP_HAL_SetClkDir_x10d0:
    /*000010d0*/ AND         R4, R4, #1 
    /*000010d4*/ MOV         R3, R2 
    AIP_HAL_SetClkDir_x10d8:
    /*000010d8*/ BFI         R1, R4, #12, #1 
    /*000010dc*/ LDR         R0, [PC, #264]              @ 0x00000000000011ec 
    /*000010e0*/ BFI         R1, R4, #13, #1 
    /*000010e4*/ STR         R1, [R3] 
    /*000010e8*/ BL          0x00000000000010E8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000010ec*/ MOV         R0, #0 
    AIP_HAL_SetClkDir_x10f0:
    /*000010f0*/ ADD         SP, SP, #16 
    /*000010f4*/ POP         {R4, R5, R6, PC} 
    AIP_HAL_SetClkDir_x10f8:
    /*000010f8*/ MOVW        R3, #849                    @ 0x351 
    /*000010fc*/ MOV         R1, #20 
    /*00001100*/ STR         R3, [SP] 
    /*00001104*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00001108*/ LDR         R3, [PC, #224]              @ 0x00000000000011f0 
    /*0000110c*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00001110*/ STR         R6, [SP, #4] 
    /*00001114*/ MOV         R0, #3 
    /*00001118*/ BL          0x0000000000001118          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000111c*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00001120*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00001124*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00001128*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*0000112c*/ MOVW        R2, #850                    @ 0x352 
    /*00001130*/ LDR         R1, [PC, #184]              @ 0x00000000000011f0 
    /*00001134*/ BL          0x0000000000001134          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001138*/ CMP         R4, #1 
    /*0000113c*/ BLS         AIP_HAL_SetClkDir_x1084
    AIP_HAL_SetClkDir_x1140:
    /*00001140*/ STR         R4, [SP, #8] 
    /*00001144*/ MOVW        R3, #855                    @ 0x357 
    /*00001148*/ STR         R6, [SP, #4] 
    /*0000114c*/ MOV         R1, #20 
    /*00001150*/ STR         R3, [SP] 
    /*00001154*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1dc] 
    /*00001158*/ LDR         R3, [PC, #144]              @ 0x00000000000011f0 
    /*0000115c*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1dc] 
    /*00001160*/ MOV         R0, #3 
    /*00001164*/ BL          0x0000000000001164          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001168*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000116c*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001170*/ ADD         SP, SP, #16 
    /*00001174*/ POP         {R4, R5, R6, PC} 
    AIP_HAL_SetClkDir_x1178:
    /*00001178*/ LDR         R3, [R3, #8] 
    /*0000117c*/ CMP         R3, #1 
    /*00001180*/ BNE         AIP_HAL_SetClkDir_x10d0
    /*00001184*/ ADD         R6, R6, #324                @ 0x144 
    /*00001188*/ AND         R4, R4, #1 
    /*0000118c*/ LDR         R3, [R0, R6] 
    /*00001190*/ BFI         R3, R4, #12, #1 
    /*00001194*/ BFI         R3, R4, #13, #1 
    /*00001198*/ STR         R3, [R0, R6] 
    /*0000119c*/ LDR         R3, [R5, #192]              @ 0xc0 
    /*000011a0*/ ADD         R3, R3, IP 
    /*000011a4*/ B           AIP_HAL_SetClkDir_x10d8
    AIP_HAL_SetClkDir_x11a8:
    /*000011a8*/ MOV         R0, #65024                  @ 0xfe00 
    /*000011ac*/ MOVT        R0, #65535                  @ 0xffff 
    /*000011b0*/ B           AIP_HAL_SetClkDir_x10f0
    AIP_HAL_SetClkDir_x11b4:
    /*000011b4*/ MOVW        R3, #866                    @ 0x362 
    /*000011b8*/ MOV         R1, #20 
    /*000011bc*/ STR         R3, [SP] 
    /*000011c0*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x218] 
    /*000011c4*/ LDR         R3, [PC, #36]               @ 0x00000000000011f0 
    /*000011c8*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x218] 
    /*000011cc*/ STR         R6, [SP, #4] 
    /*000011d0*/ MOV         R0, #3 
    /*000011d4*/ BL          0x00000000000011D4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000011d8*/ ADD         R0, R5, #196                @ 0xc4 
    /*000011dc*/ BL          0x00000000000011DC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000011e0*/ MOVW        R0, #32777                  @ 0x8009 
    /*000011e4*/ MOVT        R0, #40981                  @ 0xa015 
    /*000011e8*/ B           AIP_HAL_SetClkDir_x10f0
    /*000011ec*/ ANDEQ       R0, R0, R4, ASR #1 
    /*000011f0*/ LDRDEQ      R0, [R0], -R4 
FUNC_END AIP_HAL_SetClkDir


FUNC_BEGIN AIP_HAL_ClrAiAttr
    /*000011f4*/ PUSH        {R4, R5, R6, LR} 
    /*000011f8*/ SUBS        R6, R0, #0 
    /*000011fc*/ SUB         SP, SP, #8 
    /*00001200*/ BNE         AIP_HAL_ClrAiAttr_x123c
    AIP_HAL_ClrAiAttr_x1204:
    /*00001204*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001208*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000120c*/ ADD         R0, R4, #196                @ 0xc4 
    /*00001210*/ BL          0x0000000000001210          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001214*/ SUBS        R5, R0, #0 
    /*00001218*/ BNE         AIP_HAL_ClrAiAttr_x1280
    /*0000121c*/ MOV         R3, #92                     @ 0x5c 
    /*00001220*/ ADD         R0, R4, #196                @ 0xc4 
    /*00001224*/ MLA         R4, R3, R6, R4 
    /*00001228*/ STR         R5, [R4, #4] 
    /*0000122c*/ BL          0x000000000000122C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001230*/ MOV         R0, R5 
    AIP_HAL_ClrAiAttr_x1234:
    /*00001234*/ ADD         SP, SP, #8 
    /*00001238*/ POP         {R4, R5, R6, PC} 
    AIP_HAL_ClrAiAttr_x123c:
    /*0000123c*/ STR         R6, [SP, #4] 
    /*00001240*/ MOVW        R3, #1356                   @ 0x54c 
    /*00001244*/ MOV         R1, #20 
    /*00001248*/ STR         R3, [SP] 
    /*0000124c*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00001250*/ LDR         R3, [PC, #52]               @ 0x000000000000128c 
    /*00001254*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00001258*/ MOV         R0, #3 
    /*0000125c*/ BL          0x000000000000125C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001260*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00001264*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00001268*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*0000126c*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00001270*/ MOVW        R2, #1357                   @ 0x54d 
    /*00001274*/ LDR         R1, [PC, #16]               @ 0x000000000000128c 
    /*00001278*/ BL          0x0000000000001278          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000127c*/ B           AIP_HAL_ClrAiAttr_x1204
    AIP_HAL_ClrAiAttr_x1280:
    /*00001280*/ MOV         R0, #65024                  @ 0xfe00 
    /*00001284*/ MOVT        R0, #65535                  @ 0xffff 
    /*00001288*/ B           AIP_HAL_ClrAiAttr_x1234
    /*0000128c*/ ANDEQ       R0, R0, R8, ROR #3 
FUNC_END AIP_HAL_ClrAiAttr


FUNC_BEGIN AOP_HAL_SetClkDir
    /*00001290*/ CMP         R0, #1 
    /*00001294*/ PUSH        {R4, R5, R6, LR} 
    /*00001298*/ MOV         R4, R0 
    /*0000129c*/ SUB         SP, SP, #16 
    /*000012a0*/ MOV         R5, R1 
    /*000012a4*/ BLS         AOP_HAL_SetClkDir_x12e8
    /*000012a8*/ STR         R0, [SP, #4] 
    /*000012ac*/ MOVW        R3, #1827                   @ 0x723 
    /*000012b0*/ MOV         R1, #20 
    /*000012b4*/ STR         R3, [SP] 
    /*000012b8*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*000012bc*/ LDR         R3, [PC, #332]              @ 0x0000000000001410 
    /*000012c0*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*000012c4*/ MOV         R0, #3 
    /*000012c8*/ BL          0x00000000000012C8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000012cc*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*000012d0*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*000012d4*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*000012d8*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*000012dc*/ MOVW        R2, #1828                   @ 0x724 
    /*000012e0*/ LDR         R1, [PC, #296]              @ 0x0000000000001410 
    /*000012e4*/ BL          0x00000000000012E4          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_SetClkDir_x12e8:
    /*000012e8*/ CMP         R5, #1 
    /*000012ec*/ BHI         AOP_HAL_SetClkDir_x1394
    /*000012f0*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000012f4*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000012f8*/ ADD         R0, R6, #196                @ 0xc4 
    /*000012fc*/ BL          0x00000000000012FC          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001300*/ CMP         R0, #0 
    /*00001304*/ BNE         AOP_HAL_SetClkDir_x13cc
    /*00001308*/ MOV         R3, #92                     @ 0x5c 
    /*0000130c*/ MLA         R3, R3, R4, R6 
    /*00001310*/ LDR         R2, [R3, #8] 
    /*00001314*/ CMP         R2, #0 
    /*00001318*/ BEQ         AOP_HAL_SetClkDir_x13d8
    /*0000131c*/ LDR         R2, [R3, #84]               @ 0x54 
    /*00001320*/ LSL         R4, R4, #3 
    /*00001324*/ LDR         R0, [R6, #192]              @ 0xc0 
    /*00001328*/ ADD         IP, R4, #324                @ 0x144 
    /*0000132c*/ CMP         R2, #1 
    /*00001330*/ ADD         R2, R0, IP 
    /*00001334*/ LDR         R1, [R0, IP] 
    /*00001338*/ BEQ         AOP_HAL_SetClkDir_x1364
    AOP_HAL_SetClkDir_x133c:
    /*0000133c*/ AND         R5, R5, #1 
    /*00001340*/ MOV         R3, R2 
    AOP_HAL_SetClkDir_x1344:
    /*00001344*/ BFI         R1, R5, #12, #1 
    /*00001348*/ LDR         R0, [PC, #196]              @ 0x0000000000001414 
    /*0000134c*/ BFI         R1, R5, #13, #1 
    /*00001350*/ STR         R1, [R3] 
    /*00001354*/ BL          0x0000000000001354          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001358*/ MOV         R0, #0 
    AOP_HAL_SetClkDir_x135c:
    /*0000135c*/ ADD         SP, SP, #16 
    /*00001360*/ POP         {R4, R5, R6, PC} 
    AOP_HAL_SetClkDir_x1364:
    /*00001364*/ LDR         R3, [R3, #4] 
    /*00001368*/ CMP         R3, #1 
    /*0000136c*/ BNE         AOP_HAL_SetClkDir_x133c
    /*00001370*/ ADD         R4, R4, #260                @ 0x104 
    /*00001374*/ AND         R5, R5, #1 
    /*00001378*/ LDR         R3, [R0, R4] 
    /*0000137c*/ BFI         R3, R5, #12, #1 
    /*00001380*/ BFI         R3, R5, #13, #1 
    /*00001384*/ STR         R3, [R0, R4] 
    /*00001388*/ LDR         R3, [R6, #192]              @ 0xc0 
    /*0000138c*/ ADD         R3, R3, IP 
    /*00001390*/ B           AOP_HAL_SetClkDir_x1344
    AOP_HAL_SetClkDir_x1394:
    /*00001394*/ STR         R5, [SP, #8] 
    /*00001398*/ MOVW        R3, #1833                   @ 0x729 
    /*0000139c*/ STR         R4, [SP, #4] 
    /*000013a0*/ MOV         R1, #20 
    /*000013a4*/ STR         R3, [SP] 
    /*000013a8*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1dc] 
    /*000013ac*/ LDR         R3, [PC, #92]               @ 0x0000000000001410 
    /*000013b0*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1dc] 
    /*000013b4*/ MOV         R0, #3 
    /*000013b8*/ BL          0x00000000000013B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000013bc*/ MOVW        R0, #32771                  @ 0x8003 
    /*000013c0*/ MOVT        R0, #40982                  @ 0xa016 
    /*000013c4*/ ADD         SP, SP, #16 
    /*000013c8*/ POP         {R4, R5, R6, PC} 
    AOP_HAL_SetClkDir_x13cc:
    /*000013cc*/ MOV         R0, #65024                  @ 0xfe00 
    /*000013d0*/ MOVT        R0, #65535                  @ 0xffff 
    /*000013d4*/ B           AOP_HAL_SetClkDir_x135c
    AOP_HAL_SetClkDir_x13d8:
    /*000013d8*/ MOVW        R3, #1844                   @ 0x734 
    /*000013dc*/ MOV         R1, #20 
    /*000013e0*/ STR         R3, [SP] 
    /*000013e4*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x264] 
    /*000013e8*/ LDR         R3, [PC, #32]               @ 0x0000000000001410 
    /*000013ec*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x264] 
    /*000013f0*/ STR         R4, [SP, #4] 
    /*000013f4*/ MOV         R0, #3 
    /*000013f8*/ BL          0x00000000000013F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000013fc*/ ADD         R0, R6, #196                @ 0xc4 
    /*00001400*/ BL          0x0000000000001400          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001404*/ MOVW        R0, #32777                  @ 0x8009 
    /*00001408*/ MOVT        R0, #40982                  @ 0xa016 
    /*0000140c*/ B           AOP_HAL_SetClkDir_x135c
    /*00001410*/ STRDEQ      R0, [R0], -IP 
    /*00001414*/ ANDEQ       R0, R0, R4, ASR #1 
FUNC_END AOP_HAL_SetClkDir


FUNC_BEGIN AOP_HAL_ClrAoAttr
    /*00001418*/ CMP         R0, #1 
    /*0000141c*/ PUSH        {R4, R5, R6, LR} 
    /*00001420*/ MOV         R5, R0 
    /*00001424*/ SUB         SP, SP, #8 
    /*00001428*/ BLS         AOP_HAL_ClrAoAttr_x146c
    /*0000142c*/ STR         R0, [SP, #4] 
    /*00001430*/ MOVW        R3, #2433                   @ 0x981 
    /*00001434*/ MOV         R1, #20 
    /*00001438*/ STR         R3, [SP] 
    /*0000143c*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00001440*/ LDR         R3, [PC, #104]              @ 0x00000000000014b0 
    /*00001444*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00001448*/ MOV         R0, #3 
    /*0000144c*/ BL          0x000000000000144C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001450*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00001454*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00001458*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*0000145c*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00001460*/ MOVW        R2, #2434                   @ 0x982 
    /*00001464*/ LDR         R1, [PC, #68]               @ 0x00000000000014b0 
    /*00001468*/ BL          0x0000000000001468          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_ClrAoAttr_x146c:
    /*0000146c*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001470*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001474*/ ADD         R0, R4, #196                @ 0xc4 
    /*00001478*/ BL          0x0000000000001478          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*0000147c*/ SUBS        R6, R0, #0 
    /*00001480*/ BNE         AOP_HAL_ClrAoAttr_x14a4
    /*00001484*/ MOV         R3, #92                     @ 0x5c 
    /*00001488*/ ADD         R0, R4, #196                @ 0xc4 
    /*0000148c*/ MLA         R4, R3, R5, R4 
    /*00001490*/ STR         R6, [R4, #8] 
    /*00001494*/ BL          0x0000000000001494          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001498*/ MOV         R0, R6 
    AOP_HAL_ClrAoAttr_x149c:
    /*0000149c*/ ADD         SP, SP, #8 
    /*000014a0*/ POP         {R4, R5, R6, PC} 
    AOP_HAL_ClrAoAttr_x14a4:
    /*000014a4*/ MOV         R0, #65024                  @ 0xfe00 
    /*000014a8*/ MOVT        R0, #65535                  @ 0xffff 
    /*000014ac*/ B           AOP_HAL_ClrAoAttr_x149c
    /*000014b0*/ ANDEQ       R0, R0, R0, LSL R2 
FUNC_END AOP_HAL_ClrAoAttr


FUNC_BEGIN AIP_HAL_GetAcodecGain
    /*000014b4*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000014b8*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000014bc*/ PUSH        {R4, R5, R6, LR} 
    /*000014c0*/ MOV         R6, R0 
    /*000014c4*/ LDR         R3, [R3, #200]              @ 0xc8 
    /*000014c8*/ MOV         R0, #100                    @ 0x64 
    /*000014cc*/ LDR         R5, [R3, #32] 
    /*000014d0*/ LDR         R4, [R3, #24] 
    /*000014d4*/ BL          0x00000000000014D4          @ osal_udelay [R_ARM_CALL: +0x0] 
    /*000014d8*/ UBFX        R5, R5, #16, #5 
    /*000014dc*/ MOV         R0, #0 
    /*000014e0*/ ANDS        R3, R4, #4194304            @ 0x400000 
    /*000014e4*/ MOVNE       R3, #20 
    /*000014e8*/ ADD         R3, R3, R5, LSL #1 
    /*000014ec*/ STR         R3, [R6] 
    /*000014f0*/ POP         {R4, R5, R6, PC} 
FUNC_END AIP_HAL_GetAcodecGain


FUNC_BEGIN AIAO_QueryState
    /*000014f4*/ PUSH        {R4, LR} 
    /*000014f8*/ MOV         R4, R0 
    /*000014fc*/ LDR         R0, [PC, #20]               @ 0x0000000000001518 
    /*00001500*/ BL          0x0000000000001500          @ osal_atomic_read [R_ARM_CALL: +0x0] 
    /*00001504*/ CMP         R0, #0 
    /*00001508*/ MOVEQ       R3, #17 
    /*0000150c*/ MOVNE       R3, #34                     @ 0x22 
    /*00001510*/ STR         R3, [R4] 
    /*00001514*/ POP         {R4, PC} 
    /*00001518*/ ANDEQ       R0, R0, IP, ASR #1 
FUNC_END AIAO_QueryState


FUNC_BEGIN AiaoSetMask.part.0
    /*0000151c*/ LDR         R3, [R0, #36]               @ 0x24 
    /*00001520*/ CMP         R3, #0 
    /*00001524*/ BEQ         AiaoSetMask.part.0_x1544
    /*00001528*/ CMP         R3, #2 
    /*0000152c*/ MOVWEQ      R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001530*/ MOVTEQ      R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001534*/ MOVEQ       R2, #30 
    /*00001538*/ LDREQ       R3, [R3, #200]              @ 0xc8 
    /*0000153c*/ STREQ       R2, [R3, #72]               @ 0x48 
    /*00001540*/ BX          LR 
    AiaoSetMask.part.0_x1544:
    /*00001544*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001548*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000154c*/ MOV         R2, #29 
    /*00001550*/ LDR         R3, [R3, #200]              @ 0xc8 
    /*00001554*/ STR         R2, [R3, #72]               @ 0x48 
    /*00001558*/ BX          LR 
FUNC_END AiaoSetMask.part.0


FUNC_BEGIN AIO_HAL_SetRestore
    /*0000155c*/ PUSH        {R4, LR} 
    /*00001560*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001564*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001568*/ MOV         R0, #0 
    /*0000156c*/ LDR         R3, [R4, #200]              @ 0xc8 
    /*00001570*/ CMP         R3, #0 
    /*00001574*/ BEQ         AIO_HAL_SetRestore_x157c
    /*00001578*/ POP         {R4, PC} 
    AIO_HAL_SetRestore_x157c:
    /*0000157c*/ MOV         R1, #32768                  @ 0x8000 
    /*00001580*/ MOVT        R0, #4412                   @ 0x113c 
    /*00001584*/ BL          0x0000000000001584          @ osal_ioremap [R_ARM_CALL: +0x0] 
    /*00001588*/ CLZ         R3, R0 
    /*0000158c*/ STR         R0, [R4, #200]              @ 0xc8 
    /*00001590*/ LSR         R3, R3, #5 
    /*00001594*/ RSB         R0, R3, #0 
    /*00001598*/ POP         {R4, PC} 
FUNC_END AIO_HAL_SetRestore


FUNC_BEGIN AIP_HAL_SetAcodecGain
    /*0000159c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000015a0*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000015a4*/ CMP         R0, #19 
    /*000015a8*/ LDR         IP, [R3, #208]              @ 0xd0 
    /*000015ac*/ ASRLS       R3, R0, #1 
    /*000015b0*/ LDR         R2, [IP, #12] 
    /*000015b4*/ BIC         R2, R2, #520093696          @ 0x1f000000 
    /*000015b8*/ BIC         R2, R2, #2031616            @ 0x1f0000 
    /*000015bc*/ BLS         AIP_HAL_SetAcodecGain_x15dc
    /*000015c0*/ SUB         R3, R0, #20 
    /*000015c4*/ CMP         R3, #30 
    /*000015c8*/ BHI         AIP_HAL_SetAcodecGain_x15f4
    /*000015cc*/ LDR         R1, [IP, #4] 
    /*000015d0*/ ASR         R3, R3, #1 
    /*000015d4*/ ORR         R1, R1, #12582912           @ 0xc00000 
    /*000015d8*/ STR         R1, [IP, #4] 
    AIP_HAL_SetAcodecGain_x15dc:
    /*000015dc*/ LSL         R1, R3, #24 
    /*000015e0*/ MOV         R0, #0 
    /*000015e4*/ ORR         R3, R1, R3, LSL #16 
    /*000015e8*/ ORR         R2, R3, R2 
    /*000015ec*/ STR         R2, [IP, #12] 
    /*000015f0*/ BX          LR 
    AIP_HAL_SetAcodecGain_x15f4:
    /*000015f4*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000015f8*/ SUB         SP, SP, #12 
    /*000015fc*/ MOVW        R3, #2668                   @ 0xa6c 
    /*00001600*/ MOV         R1, #20 
    /*00001604*/ STR         R0, [SP, #4] 
    /*00001608*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b0] 
    /*0000160c*/ STR         R3, [SP] 
    /*00001610*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b0] 
    /*00001614*/ LDR         R3, [PC, #16]               @ 0x000000000000162c 
    /*00001618*/ MOV         R0, #3 
    /*0000161c*/ BL          0x000000000000161C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001620*/ MVN         R0, #0 
    /*00001624*/ ADD         SP, SP, #12 
    /*00001628*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*0000162c*/ ANDEQ       R0, R0, R4, LSR #4 
FUNC_END AIP_HAL_SetAcodecGain


FUNC_BEGIN AIP_HAL_SetTrackMode
    /*00001630*/ PUSH        {R4, R5, LR} 
    /*00001634*/ SUBS        R4, R0, #0 
    /*00001638*/ SUB         SP, SP, #20 
    /*0000163c*/ MOV         R5, R1 
    /*00001640*/ BNE         AIP_HAL_SetTrackMode_x1674
    AIP_HAL_SetTrackMode_x1644:
    /*00001644*/ CMP         R5, #7 
    /*00001648*/ BHI         AIP_HAL_SetTrackMode_x16b8
    /*0000164c*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001650*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001654*/ ADD         R3, R4, #16 
    /*00001658*/ MOV         R0, #0 
    /*0000165c*/ LDR         R1, [R2, #192]              @ 0xc0 
    /*00001660*/ LDR         R2, [R1, R3, LSL #8] 
    /*00001664*/ BFI         R2, R5, #16, #3 
    /*00001668*/ STR         R2, [R1, R3, LSL #8] 
    AIP_HAL_SetTrackMode_x166c:
    /*0000166c*/ ADD         SP, SP, #20 
    /*00001670*/ POP         {R4, R5, PC} 
    AIP_HAL_SetTrackMode_x1674:
    /*00001674*/ STR         R4, [SP, #4] 
    /*00001678*/ MOVW        R3, #805                    @ 0x325 
    /*0000167c*/ MOV         R1, #20 
    /*00001680*/ STR         R3, [SP] 
    /*00001684*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00001688*/ LDR         R3, [PC, #92]               @ 0x00000000000016ec 
    /*0000168c*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00001690*/ MOV         R0, #3 
    /*00001694*/ BL          0x0000000000001694          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001698*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*0000169c*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*000016a0*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*000016a4*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*000016a8*/ MOVW        R2, #806                    @ 0x326 
    /*000016ac*/ LDR         R1, [PC, #56]               @ 0x00000000000016ec 
    /*000016b0*/ BL          0x00000000000016B0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000016b4*/ B           AIP_HAL_SetTrackMode_x1644
    AIP_HAL_SetTrackMode_x16b8:
    /*000016b8*/ MOVW        R3, #811                    @ 0x32b 
    /*000016bc*/ STR         R5, [SP, #8] 
    /*000016c0*/ STR         R3, [SP] 
    /*000016c4*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2f8] 
    /*000016c8*/ STR         R4, [SP, #4] 
    /*000016cc*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2f8] 
    /*000016d0*/ LDR         R3, [PC, #20]               @ 0x00000000000016ec 
    /*000016d4*/ MOV         R1, #20 
    /*000016d8*/ MOV         R0, #3 
    /*000016dc*/ BL          0x00000000000016DC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000016e0*/ MOVW        R0, #32771                  @ 0x8003 
    /*000016e4*/ MOVT        R0, #40981                  @ 0xa015 
    /*000016e8*/ B           AIP_HAL_SetTrackMode_x166c
    /*000016ec*/ ANDEQ       R0, R0, IP, LSR R2 
FUNC_END AIP_HAL_SetTrackMode


FUNC_BEGIN AOP_HAL_SetTrackMode
    /*000016f0*/ CMP         R0, #1 
    /*000016f4*/ PUSH        {R4, R5, LR} 
    /*000016f8*/ MOV         R4, R0 
    /*000016fc*/ SUB         SP, SP, #20 
    /*00001700*/ MOV         R5, R1 
    /*00001704*/ BLS         AOP_HAL_SetTrackMode_x1748
    /*00001708*/ STR         R0, [SP, #4] 
    /*0000170c*/ MOVW        R3, #1779                   @ 0x6f3 
    /*00001710*/ MOV         R1, #20 
    /*00001714*/ STR         R3, [SP] 
    /*00001718*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*0000171c*/ LDR         R3, [PC, #136]              @ 0x00000000000017ac 
    /*00001720*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00001724*/ MOV         R0, #3 
    /*00001728*/ BL          0x0000000000001728          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000172c*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00001730*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00001734*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00001738*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*0000173c*/ MOVW        R2, #1780                   @ 0x6f4 
    /*00001740*/ LDR         R1, [PC, #100]              @ 0x00000000000017ac 
    /*00001744*/ BL          0x0000000000001744          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_SetTrackMode_x1748:
    /*00001748*/ CMP         R5, #7 
    /*0000174c*/ BHI         AOP_HAL_SetTrackMode_x1778
    /*00001750*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001754*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001758*/ ADD         R4, R4, #32 
    /*0000175c*/ MOV         R0, #0 
    /*00001760*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00001764*/ LDR         R3, [R2, R4, LSL #8] 
    /*00001768*/ BFI         R3, R5, #16, #3 
    /*0000176c*/ STR         R3, [R2, R4, LSL #8] 
    AOP_HAL_SetTrackMode_x1770:
    /*00001770*/ ADD         SP, SP, #20 
    /*00001774*/ POP         {R4, R5, PC} 
    AOP_HAL_SetTrackMode_x1778:
    /*00001778*/ MOVW        R3, #1785                   @ 0x6f9 
    /*0000177c*/ STR         R5, [SP, #8] 
    /*00001780*/ STR         R3, [SP] 
    /*00001784*/ MOVW        R2, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x330] 
    /*00001788*/ STR         R4, [SP, #4] 
    /*0000178c*/ MOVT        R2, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x330] 
    /*00001790*/ LDR         R3, [PC, #20]               @ 0x00000000000017ac 
    /*00001794*/ MOV         R1, #20 
    /*00001798*/ MOV         R0, #3 
    /*0000179c*/ BL          0x000000000000179C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000017a0*/ MOVW        R0, #32771                  @ 0x8003 
    /*000017a4*/ MOVT        R0, #40982                  @ 0xa016 
    /*000017a8*/ B           AOP_HAL_SetTrackMode_x1770
    /*000017ac*/ ANDEQ       R0, R0, R4, ASR R2 
FUNC_END AOP_HAL_SetTrackMode


FUNC_BEGIN AOP_HAL_SetVolume
    /*000017b0*/ CMP         R0, #1 
    /*000017b4*/ PUSH        {R4, R5, LR} 
    /*000017b8*/ MOV         R4, R0 
    /*000017bc*/ SUB         SP, SP, #20 
    /*000017c0*/ MOV         R5, R1 
    /*000017c4*/ BLS         AOP_HAL_SetVolume_x1808
    /*000017c8*/ STR         R0, [SP, #4] 
    /*000017cc*/ MOVW        R3, #1896                   @ 0x768 
    /*000017d0*/ MOV         R1, #20 
    /*000017d4*/ STR         R3, [SP] 
    /*000017d8*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*000017dc*/ LDR         R3, [PC, #144]              @ 0x0000000000001874 
    /*000017e0*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*000017e4*/ MOV         R0, #3 
    /*000017e8*/ BL          0x00000000000017E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000017ec*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*000017f0*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*000017f4*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*000017f8*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*000017fc*/ MOVW        R2, #1897                   @ 0x769 
    /*00001800*/ LDR         R1, [PC, #108]              @ 0x0000000000001874 
    /*00001804*/ BL          0x0000000000001804          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_SetVolume_x1808:
    /*00001808*/ ADD         R1, R5, #121                @ 0x79 
    /*0000180c*/ CMP         R1, #127                    @ 0x7f 
    /*00001810*/ BHI         AOP_HAL_SetVolume_x1840
    /*00001814*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001818*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000181c*/ MOVW        R3, #8196                   @ 0x2004 
    /*00001820*/ ADD         R3, R3, R4, LSL #8 
    /*00001824*/ LDR         IP, [R2, #192]              @ 0xc0 
    /*00001828*/ MOV         R0, #0 
    /*0000182c*/ LDR         R2, [IP, R3] 
    /*00001830*/ BFI         R2, R1, #8, #7 
    /*00001834*/ STR         R2, [IP, R3] 
    AOP_HAL_SetVolume_x1838:
    /*00001838*/ ADD         SP, SP, #20 
    /*0000183c*/ POP         {R4, R5, PC} 
    AOP_HAL_SetVolume_x1840:
    /*00001840*/ MOVW        R3, #1903                   @ 0x76f 
    /*00001844*/ STR         R5, [SP, #8] 
    /*00001848*/ STR         R3, [SP] 
    /*0000184c*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x368] 
    /*00001850*/ STR         R4, [SP, #4] 
    /*00001854*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x368] 
    /*00001858*/ LDR         R3, [PC, #20]               @ 0x0000000000001874 
    /*0000185c*/ MOV         R1, #20 
    /*00001860*/ MOV         R0, #3 
    /*00001864*/ BL          0x0000000000001864          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001868*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000186c*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001870*/ B           AOP_HAL_SetVolume_x1838
    /*00001874*/ ANDEQ       R0, R0, IP, ROR #4 
FUNC_END AOP_HAL_SetVolume


FUNC_BEGIN aiao_ioctl
    /*00001878*/ MOV         R3, #21248                  @ 0x5300 
    /*0000187c*/ MOVT        R3, #16388                  @ 0x4004 
    /*00001880*/ CMP         R0, R3 
    /*00001884*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00001888*/ SUB         SP, SP, #20 
    /*0000188c*/ BEQ         aiao_ioctl_x18f0
    /*00001890*/ MOVW        R3, #21249                  @ 0x5301 
    /*00001894*/ MOVT        R3, #32772                  @ 0x8004 
    /*00001898*/ CMP         R0, R3 
    /*0000189c*/ BNE         aiao_ioctl_x18b8
    /*000018a0*/ CMP         R1, #0 
    /*000018a4*/ BEQ         aiao_ioctl_x1920
    /*000018a8*/ MOV         R0, #0 
    /*000018ac*/ STR         R0, [R1] 
    /*000018b0*/ ADD         SP, SP, #20 
    /*000018b4*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    aiao_ioctl_x18b8:
    /*000018b8*/ STR         R0, [SP, #4] 
    /*000018bc*/ MOVW        R3, #2837                   @ 0xb15 
    /*000018c0*/ UXTB        R0, R0 
    /*000018c4*/ STR         R3, [SP] 
    /*000018c8*/ STR         R0, [SP, #8] 
    /*000018cc*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3d8] 
    /*000018d0*/ LDR         R3, [PC, #80]               @ 0x0000000000001928 
    /*000018d4*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3d8] 
    /*000018d8*/ MOV         R1, #20 
    /*000018dc*/ MOV         R0, #3 
    /*000018e0*/ BL          0x00000000000018E0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000018e4*/ MVN         R0, #0 
    aiao_ioctl_x18e8:
    /*000018e8*/ ADD         SP, SP, #20 
    /*000018ec*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    aiao_ioctl_x18f0:
    /*000018f0*/ MOV         R3, #236                    @ 0xec 
    /*000018f4*/ MOV         R1, #20 
    /*000018f8*/ STR         R3, [SP] 
    /*000018fc*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x39c] 
    /*00001900*/ LDR         R3, [PC, #36]               @ 0x000000000000192c 
    /*00001904*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x39c] 
    /*00001908*/ MOV         R0, #3 
    /*0000190c*/ BL          0x000000000000190C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001910*/ MOVW        R0, #32776                  @ 0x8008 
    /*00001914*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001918*/ ADD         SP, SP, #20 
    /*0000191c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    aiao_ioctl_x1920:
    /*00001920*/ MVN         R0, #0 
    /*00001924*/ B           aiao_ioctl_x18e8
    /*00001928*/ MULEQ       R0, R8, R2 
    /*0000192c*/ ANDEQ       R0, R0, R0, LSL #5 
FUNC_END aiao_ioctl


FUNC_BEGIN aiao_freeze
    /*00001930*/ MOV         R0, #0 
    /*00001934*/ BX          LR 
FUNC_END aiao_freeze


FUNC_BEGIN AIAOGetMclkCfg.constprop.15
    /*00001938*/ MOV         R3, #0 
    /*0000193c*/ STR         R3, [R2] 
    /*00001940*/ LDR         R3, [R0] 
    /*00001944*/ MOVW        R2, #24000                  @ 0x5dc0 
    /*00001948*/ CMP         R3, R2 
    /*0000194c*/ BEQ         AIAOGetMclkCfg.constprop.15_x1a10
    /*00001950*/ BHI         AIAOGetMclkCfg.constprop.15_x19b0
    /*00001954*/ MOVW        R2, #12000                  @ 0x2ee0 
    /*00001958*/ CMP         R3, R2 
    /*0000195c*/ BEQ         AIAOGetMclkCfg.constprop.15_x1a10
    /*00001960*/ BHI         AIAOGetMclkCfg.constprop.15_x19d8
    /*00001964*/ CMP         R3, #8000                   @ 0x1f40 
    /*00001968*/ BEQ         AIAOGetMclkCfg.constprop.15_x19e0
    /*0000196c*/ MOVW        R2, #11025                  @ 0x2b11 
    /*00001970*/ CMP         R3, R2 
    /*00001974*/ BEQ         AIAOGetMclkCfg.constprop.15_x1a34
    AIAOGetMclkCfg.constprop.15_x1978:
    /*00001978*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000197c*/ SUB         SP, SP, #12 
    /*00001980*/ MOVW        R3, #329                    @ 0x149 
    /*00001984*/ MOV         R1, #20 
    /*00001988*/ STR         R3, [SP] 
    /*0000198c*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x410] 
    /*00001990*/ LDR         R3, [PC, #180]              @ 0x0000000000001a4c 
    /*00001994*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x410] 
    /*00001998*/ MOV         R0, #3 
    /*0000199c*/ BL          0x000000000000199C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000019a0*/ MOVW        R0, #32776                  @ 0x8008 
    /*000019a4*/ MOVT        R0, #40982                  @ 0xa016 
    /*000019a8*/ ADD         SP, SP, #12 
    /*000019ac*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    AIAOGetMclkCfg.constprop.15_x19b0:
    /*000019b0*/ MOVW        R2, #48000                  @ 0xbb80 
    /*000019b4*/ CMP         R3, R2 
    /*000019b8*/ BEQ         AIAOGetMclkCfg.constprop.15_x1a10
    /*000019bc*/ BHI         AIAOGetMclkCfg.constprop.15_x19f8
    /*000019c0*/ CMP         R3, #32000                  @ 0x7d00 
    /*000019c4*/ BEQ         AIAOGetMclkCfg.constprop.15_x19e0
    /*000019c8*/ MOVW        R2, #44100                  @ 0xac44 
    /*000019cc*/ CMP         R3, R2 
    /*000019d0*/ BEQ         AIAOGetMclkCfg.constprop.15_x1a34
    /*000019d4*/ B           AIAOGetMclkCfg.constprop.15_x1978
    AIAOGetMclkCfg.constprop.15_x19d8:
    /*000019d8*/ CMP         R3, #16000                  @ 0x3e80 
    /*000019dc*/ BNE         AIAOGetMclkCfg.constprop.15_x1a28
    AIAOGetMclkCfg.constprop.15_x19e0:
    /*000019e0*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000019e4*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000019e8*/ MOV         R0, #0 
    /*000019ec*/ LDR         R3, [R3, #8] 
    /*000019f0*/ STR         R3, [R1] 
    /*000019f4*/ BX          LR 
    AIAOGetMclkCfg.constprop.15_x19f8:
    /*000019f8*/ CMP         R3, #64000                  @ 0xfa00 
    /*000019fc*/ BEQ         AIAOGetMclkCfg.constprop.15_x19e0
    /*00001a00*/ MOV         R2, #30464                  @ 0x7700 
    /*00001a04*/ MOVT        R2, #1 
    /*00001a08*/ CMP         R3, R2 
    /*00001a0c*/ BNE         AIAOGetMclkCfg.constprop.15_x1978
    AIAOGetMclkCfg.constprop.15_x1a10:
    /*00001a10*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00001a14*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00001a18*/ MOV         R0, #0 
    /*00001a1c*/ LDR         R3, [R3, #4] 
    /*00001a20*/ STR         R3, [R1] 
    /*00001a24*/ BX          LR 
    AIAOGetMclkCfg.constprop.15_x1a28:
    /*00001a28*/ MOVW        R2, #22050                  @ 0x5622 
    /*00001a2c*/ CMP         R3, R2 
    /*00001a30*/ BNE         AIAOGetMclkCfg.constprop.15_x1978
    AIAOGetMclkCfg.constprop.15_x1a34:
    /*00001a34*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00001a38*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00001a3c*/ MOV         R0, #0 
    /*00001a40*/ LDR         R3, [R3, #12] 
    /*00001a44*/ STR         R3, [R1] 
    /*00001a48*/ BX          LR 
    /*00001a4c*/ ANDEQ       R0, R0, R4, LSR #5 
FUNC_END AIAOGetMclkCfg.constprop.15


FUNC_BEGIN aiao_restore
    /*00001a50*/ MOV         R0, #0 
    /*00001a54*/ BX          LR 
FUNC_END aiao_restore


FUNC_BEGIN aiao_open
    /*00001a58*/ MOV         R0, #0 
    /*00001a5c*/ BX          LR 
FUNC_END aiao_open


FUNC_BEGIN aiao_close
    /*00001a60*/ MOV         R0, #0 
    /*00001a64*/ BX          LR 
FUNC_END aiao_close


FUNC_BEGIN AIAO_HAL_ReadReg
    /*00001a68*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001a6c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001a70*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00001a74*/ LDR         R0, [R3, R0] 
    /*00001a78*/ BX          LR 
FUNC_END AIAO_HAL_ReadReg


FUNC_BEGIN AIAO_HAL_WriteReg
    /*00001a7c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001a80*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001a84*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00001a88*/ STR         R1, [R3, R0] 
    /*00001a8c*/ BX          LR 
FUNC_END AIAO_HAL_WriteReg


FUNC_BEGIN AIAO_HAL_SysInit
    /*00001a90*/ PUSH        {R4, LR} 
    /*00001a94*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001a98*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001a9c*/ LDR         R3, [R4, #192]              @ 0xc0 
    /*00001aa0*/ CMP         R3, #0 
    /*00001aa4*/ BEQ         AIAO_HAL_SysInit_x1ac4
    AIAO_HAL_SysInit_x1aa8:
    /*00001aa8*/ LDR         R3, [R4, #200]              @ 0xc8 
    /*00001aac*/ CMP         R3, #0 
    /*00001ab0*/ BEQ         AIAO_HAL_SysInit_x1ae8
    AIAO_HAL_SysInit_x1ab4:
    /*00001ab4*/ ADD         R3, R3, #20 
    /*00001ab8*/ MOV         R0, #0 
    /*00001abc*/ STR         R3, [R4, #208]              @ 0xd0 
    /*00001ac0*/ POP         {R4, PC} 
    AIAO_HAL_SysInit_x1ac4:
    /*00001ac4*/ MOV         R0, #0 
    /*00001ac8*/ MOV         R1, #65536                  @ 0x10000 
    /*00001acc*/ MOVT        R0, #4411                   @ 0x113b 
    /*00001ad0*/ BL          0x0000000000001AD0          @ osal_ioremap [R_ARM_CALL: +0x0] 
    /*00001ad4*/ CMP         R0, #0 
    /*00001ad8*/ STR         R0, [R4, #192]              @ 0xc0 
    /*00001adc*/ BNE         AIAO_HAL_SysInit_x1aa8
    AIAO_HAL_SysInit_x1ae0:
    /*00001ae0*/ MVN         R0, #0 
    /*00001ae4*/ POP         {R4, PC} 
    AIAO_HAL_SysInit_x1ae8:
    /*00001ae8*/ MOV         R0, #0 
    /*00001aec*/ MOV         R1, #32768                  @ 0x8000 
    /*00001af0*/ MOVT        R0, #4412                   @ 0x113c 
    /*00001af4*/ BL          0x0000000000001AF4          @ osal_ioremap [R_ARM_CALL: +0x0] 
    /*00001af8*/ CMP         R0, #0 
    /*00001afc*/ MOV         R3, R0 
    /*00001b00*/ STR         R0, [R4, #200]              @ 0xc8 
    /*00001b04*/ BNE         AIAO_HAL_SysInit_x1ab4
    /*00001b08*/ B           AIAO_HAL_SysInit_x1ae0
FUNC_END AIAO_HAL_SysInit


FUNC_BEGIN AIAO_Init
    /*00001b0c*/ PUSH        {R4, R5, LR} 
    /*00001b10*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00001b14*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00001b18*/ SUB         SP, SP, #12 
    /*00001b1c*/ LDR         R4, [R5] 
    /*00001b20*/ CMP         R4, #0 
    /*00001b24*/ BEQ         AIAO_Init_x1b6c
    /*00001b28*/ CMP         R4, #1 
    /*00001b2c*/ BEQ         AIAO_Init_x1b98
    /*00001b30*/ BL          0x0000000000001B30          @ AIAO_HAL_SysInit [R_ARM_CALL: .text+0x1a90] 
    /*00001b34*/ MOV         R2, #0 
    /*00001b38*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001b3c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001b40*/ MOV         R0, R2 
    /*00001b44*/ MOV         R1, #1 
    /*00001b48*/ STR         R2, [R5] 
    /*00001b4c*/ STR         R2, [R3, #4] 
    /*00001b50*/ STR         R2, [R3, #8] 
    /*00001b54*/ STR         R2, [R3, #96]               @ 0x60 
    /*00001b58*/ STR         R2, [R3, #100]              @ 0x64 
    /*00001b5c*/ STR         R1, [R3] 
    /*00001b60*/ STR         R1, [R3, #92]               @ 0x5c 
    AIAO_Init_x1b64:
    /*00001b64*/ ADD         SP, SP, #12 
    /*00001b68*/ POP         {R4, R5, PC} 
    AIAO_Init_x1b6c:
    /*00001b6c*/ MOVW        R3, #2564                   @ 0xa04 
    /*00001b70*/ MOV         R1, #20 
    /*00001b74*/ STR         R3, [SP] 
    /*00001b78*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44c] 
    /*00001b7c*/ LDR         R3, [PC, #64]               @ 0x0000000000001bc4 
    /*00001b80*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44c] 
    /*00001b84*/ MOV         R0, #6 
    /*00001b88*/ BL          0x0000000000001B88          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001b8c*/ MOV         R0, R4 
    /*00001b90*/ ADD         SP, SP, #12 
    /*00001b94*/ POP         {R4, R5, PC} 
    AIAO_Init_x1b98:
    /*00001b98*/ MOVW        R3, #2569                   @ 0xa09 
    /*00001b9c*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x47c] 
    /*00001ba0*/ STR         R3, [SP] 
    /*00001ba4*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x47c] 
    /*00001ba8*/ LDR         R3, [PC, #20]               @ 0x0000000000001bc4 
    /*00001bac*/ MOV         R1, #20 
    /*00001bb0*/ MOV         R0, #3 
    /*00001bb4*/ BL          0x0000000000001BB4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001bb8*/ MOVW        R0, #32786                  @ 0x8012 
    /*00001bbc*/ MOVT        R0, #40981                  @ 0xa015 
    /*00001bc0*/ B           AIAO_Init_x1b64
    /*00001bc4*/ @           <UNDEFINED> instruction: 0x000002b4 
FUNC_END AIAO_Init


FUNC_BEGIN AIAO_HAL_GetIntStatus
    /*00001bc8*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001bcc*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001bd0*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00001bd4*/ LDR         R0, [R3, #4] 
    /*00001bd8*/ BX          LR 
FUNC_END AIAO_HAL_GetIntStatus


FUNC_BEGIN AIAO_HAL_SysDeInit
    /*00001bdc*/ BX          LR 
FUNC_END AIAO_HAL_SysDeInit


FUNC_BEGIN AIAO_HAL_SetClkSel
    /*00001be0*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00001be4*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*00001be8*/ MOVW        R2, #12016                  @ 0x2ef0 
    /*00001bec*/ MOVW        R1, #30291                  @ 0x7653 
    /*00001bf0*/ MOVT        R2, #21 
    /*00001bf4*/ MOVT        R1, #19 
    /*00001bf8*/ STR         R2, [R3, #4] 
    /*00001bfc*/ MOV         R0, #0 
    /*00001c00*/ MOVW        R2, #8011                   @ 0x1f4b 
    /*00001c04*/ STR         R1, [R3, #12] 
    /*00001c08*/ MOVT        R2, #14 
    /*00001c0c*/ STR         R2, [R3, #8] 
    /*00001c10*/ BX          LR 
FUNC_END AIAO_HAL_SetClkSel


FUNC_BEGIN AIAO_HAL_GetClkSel
    /*00001c14*/ MOV         R2, R0 
    /*00001c18*/ MOV         R3, #0 
    /*00001c1c*/ MOV         R0, R3 
    /*00001c20*/ STR         R3, [R2] 
    /*00001c24*/ BX          LR 
FUNC_END AIAO_HAL_GetClkSel


FUNC_BEGIN AIAO_HAL_SetModParam
    /*00001c28*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00001c2c*/ SUB         SP, SP, #12 
    /*00001c30*/ MOV         R3, #236                    @ 0xec 
    /*00001c34*/ MOV         R1, #20 
    /*00001c38*/ STR         R3, [SP] 
    /*00001c3c*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x39c] 
    /*00001c40*/ LDR         R3, [PC, #24]               @ 0x0000000000001c60 
    /*00001c44*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x39c] 
    /*00001c48*/ MOV         R0, #3 
    /*00001c4c*/ BL          0x0000000000001C4C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001c50*/ MOVW        R0, #32776                  @ 0x8008 
    /*00001c54*/ MOVT        R0, #40982                  @ 0xa016 
    /*00001c58*/ ADD         SP, SP, #12 
    /*00001c5c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00001c60*/ ANDEQ       R0, R0, R0, LSL #5 
FUNC_END AIAO_HAL_SetModParam


FUNC_BEGIN AIAO_HAL_GetModParam
    /*00001c64*/ SUBS        R3, R0, #0 
    /*00001c68*/ MOVNE       R0, #0 
    /*00001c6c*/ STRNE       R0, [R3] 
    /*00001c70*/ MVNEQ       R0, #0 
    /*00001c74*/ BX          LR 
FUNC_END AIAO_HAL_GetModParam


FUNC_BEGIN AIAO_HAL_CheckAiAoClk
    /*00001c78*/ PUSH        {R4, LR} 
    /*00001c7c*/ MOV         IP, #8 
    /*00001c80*/ LDR         R2, [R0, #28] 
    /*00001c84*/ SUB         SP, SP, #8 
    /*00001c88*/ LDR         R3, [R1, #28] 
    /*00001c8c*/ CMP         R2, #1 
    /*00001c90*/ LDR         LR, [R0, #4] 
    /*00001c94*/ LDR         R4, [R1, #4] 
    /*00001c98*/ MOVEQ       R2, #2 
    /*00001c9c*/ CMP         R3, #1 
    /*00001ca0*/ LSL         LR, IP, LR 
    /*00001ca4*/ LSL         IP, IP, R4 
    /*00001ca8*/ MOVEQ       R3, #2 
    /*00001cac*/ MUL         R2, R2, LR 
    /*00001cb0*/ MUL         R3, R3, IP 
    /*00001cb4*/ CMP         R2, R3 
    /*00001cb8*/ MOVWNE      R3, #277                    @ 0x115 
    /*00001cbc*/ MOVWNE      R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4a8] 
    /*00001cc0*/ STRNE       R3, [SP] 
    /*00001cc4*/ MOVTNE      R2, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4a8] 
    /*00001cc8*/ BNE         AIAO_HAL_CheckAiAoClk_x1cf8
    /*00001ccc*/ LDR         R2, [R0] 
    /*00001cd0*/ LDR         R3, [R1] 
    /*00001cd4*/ CMP         R2, R3 
    /*00001cd8*/ MOVEQ       R0, #0 
    /*00001cdc*/ BNE         AIAO_HAL_CheckAiAoClk_x1ce8
    AIAO_HAL_CheckAiAoClk_x1ce0:
    /*00001ce0*/ ADD         SP, SP, #8 
    /*00001ce4*/ POP         {R4, PC} 
    AIAO_HAL_CheckAiAoClk_x1ce8:
    /*00001ce8*/ MOVW        R2, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x508] 
    /*00001cec*/ MOVT        R2, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x508] 
    /*00001cf0*/ MOVW        R3, #283                    @ 0x11b 
    /*00001cf4*/ STR         R3, [SP] 
    AIAO_HAL_CheckAiAoClk_x1cf8:
    /*00001cf8*/ LDR         R3, [PC, #16]               @ 0x0000000000001d10 
    /*00001cfc*/ MOV         R1, #20 
    /*00001d00*/ MOV         R0, #3 
    /*00001d04*/ BL          0x0000000000001D04          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001d08*/ MVN         R0, #0 
    /*00001d0c*/ B           AIAO_HAL_CheckAiAoClk_x1ce0
    /*00001d10*/ ANDEQ       R0, R0, R0, ASR #5 
FUNC_END AIAO_HAL_CheckAiAoClk


FUNC_BEGIN AIP_HAL_SetDevAttr
    /*00001d14*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00001d18*/ SUBS        R5, R0, #0 
    /*00001d1c*/ SUB         SP, SP, #28 
    /*00001d20*/ MOV         R4, R1 
    /*00001d24*/ BNE         AIP_HAL_SetDevAttr_x202c
    /*00001d28*/ LDR         R2, [R1, #8] 
    /*00001d2c*/ SUB         R3, R2, #4 
    /*00001d30*/ CMP         R2, #0 
    /*00001d34*/ CMPNE       R3, #1 
    /*00001d38*/ BLS         AIP_HAL_SetDevAttr_x2080
    AIP_HAL_SetDevAttr_x1d3c:
    /*00001d3c*/ LDR         R3, [R4, #36]               @ 0x24 
    /*00001d40*/ BICS        R3, R3, #2 
    /*00001d44*/ BNE         AIP_HAL_SetDevAttr_x22c8
    /*00001d48*/ LDR         R1, [R4, #28] 
    /*00001d4c*/ CMP         R1, #16 
    /*00001d50*/ BHI         AIP_HAL_SetDevAttr_x2214
    /*00001d54*/ MOVW        R3, #278                    @ 0x116 
    /*00001d58*/ MOVT        R3, #1 
    /*00001d5c*/ MVN         R3, R3, LSR R1 
    /*00001d60*/ TST         R3, #1 
    /*00001d64*/ BNE         AIP_HAL_SetDevAttr_x2214
    /*00001d68*/ CMP         R1, #2 
    /*00001d6c*/ STRHI       R1, [SP, #4] 
    /*00001d70*/ MOVWHI      R3, #1057                   @ 0x421 
    /*00001d74*/ BHI         AIP_HAL_SetDevAttr_x221c
    /*00001d78*/ CMP         R1, #1 
    /*00001d7c*/ BEQ         AIP_HAL_SetDevAttr_x2158
    AIP_HAL_SetDevAttr_x1d80:
    /*00001d80*/ LDR         R3, [R4, #4] 
    /*00001d84*/ SUB         R1, R3, #1 
    /*00001d88*/ CMP         R1, #1 
    /*00001d8c*/ BHI         AIP_HAL_SetDevAttr_x2274
    /*00001d90*/ CMP         R2, #5 
    /*00001d94*/ BHI         AIP_HAL_SetDevAttr_x22dc
    /*00001d98*/ LDR         R3, [R4, #12] 
    /*00001d9c*/ CMP         R3, #1 
    /*00001da0*/ BHI         AIP_HAL_SetDevAttr_x228c
    /*00001da4*/ LDR         R3, [R4, #20] 
    /*00001da8*/ MOVW        R2, #298                    @ 0x12a 
    /*00001dac*/ SUB         R1, R3, #2 
    /*00001db0*/ CMP         R1, R2 
    /*00001db4*/ BHI         AIP_HAL_SetDevAttr_x22f4
    /*00001db8*/ LDR         R3, [R4, #24] 
    /*00001dbc*/ SUB         R2, R3, #80                 @ 0x50 
    /*00001dc0*/ CMP         R2, #1968                   @ 0x7b0 
    /*00001dc4*/ BHI         AIP_HAL_SetDevAttr_x230c
    /*00001dc8*/ LDR         R3, [R4, #16] 
    /*00001dcc*/ CMP         R3, #2 
    /*00001dd0*/ MOVWHI      R3, #1117                   @ 0x45d 
    /*00001dd4*/ MOVWHI      R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7a4] 
    /*00001dd8*/ STRHI       R3, [SP] 
    /*00001ddc*/ MOVTHI      R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7a4] 
    /*00001de0*/ BHI         AIP_HAL_SetDevAttr_x2174
    /*00001de4*/ LDR         R3, [R4, #32] 
    /*00001de8*/ CMP         R3, #1 
    /*00001dec*/ BHI         AIP_HAL_SetDevAttr_x22a4
    /*00001df0*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001df4*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001df8*/ ADD         R0, R5, #196                @ 0xc4 
    /*00001dfc*/ BL          0x0000000000001DFC          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001e00*/ CMP         R0, #0 
    /*00001e04*/ BNE         AIP_HAL_SetDevAttr_x22bc
    /*00001e08*/ LDR         R3, [R4, #32] 
    /*00001e0c*/ CMP         R3, #1 
    /*00001e10*/ BEQ         AIP_HAL_SetDevAttr_x21c4
    AIP_HAL_SetDevAttr_x1e14:
    /*00001e14*/ MOV         R0, R4 
    /*00001e18*/ BL          0x000000000000151C 
    /*00001e1c*/ LDR         IP, [R5, #192]              @ 0xc0 
    /*00001e20*/ MOV         R0, R4 
    /*00001e24*/ MOV         R3, #0 
    /*00001e28*/ LDR         R6, [IP, #260]              @ 0x104 
    /*00001e2c*/ ADD         R2, SP, #20 
    /*00001e30*/ ADD         R1, SP, #8 
    /*00001e34*/ STR         R3, [SP, #8] 
    /*00001e38*/ STR         R3, [SP, #12] 
    /*00001e3c*/ STR         R3, [SP, #16] 
    /*00001e40*/ BL          0x0000000000001938 
    /*00001e44*/ MOV         R7, R0 
    /*00001e48*/ ADD         R3, SP, #12 
    /*00001e4c*/ MOV         R0, R4 
    /*00001e50*/ ADD         R2, SP, #16 
    /*00001e54*/ LDR         R1, [SP, #8] 
    /*00001e58*/ BL          0x0000000000000124 
    /*00001e5c*/ ORRS        R7, R7, R0 
    /*00001e60*/ BNE         AIP_HAL_SetDevAttr_x21e4
    /*00001e64*/ LDR         R1, [SP, #8] 
    /*00001e68*/ MOV         R2, #0 
    /*00001e6c*/ LDR         R3, [R5, #192]              @ 0xc0 
    /*00001e70*/ BFI         R2, R1, #0, #27 
    /*00001e74*/ LDR         R1, [SP, #16] 
    /*00001e78*/ BFI         R2, R7, #27, #5 
    /*00001e7c*/ LDR         R0, [SP, #12] 
    /*00001e80*/ BFI         R6, R1, #0, #4 
    /*00001e84*/ STR         R2, [R3, #256]              @ 0x100 
    /*00001e88*/ LDR         R1, [R4, #8] 
    /*00001e8c*/ BFI         R6, R0, #4, #3 
    /*00001e90*/ SUB         IP, R1, #4 
    /*00001e94*/ CMP         R1, #0 
    /*00001e98*/ CMPNE       IP, #1 
    /*00001e9c*/ BFILS       R6, R7, #10, #1 
    /*00001ea0*/ BFILS       R6, R7, #11, #1 
    /*00001ea4*/ BLS         AIP_HAL_SetDevAttr_x1eb8
    /*00001ea8*/ SUB         R0, R1, #1 
    /*00001eac*/ CMP         R0, #2 
    /*00001eb0*/ BHI         AIP_HAL_SetDevAttr_x224c
    /*00001eb4*/ ORR         R6, R6, #3072               @ 0xc00 
    AIP_HAL_SetDevAttr_x1eb8:
    /*00001eb8*/ ORR         R6, R6, #256                @ 0x100 
    /*00001ebc*/ LDR         R1, [R5, #8] 
    /*00001ec0*/ BFC         R6, #9, #1 
    /*00001ec4*/ ADD         R0, R3, #4096               @ 0x1000 
    /*00001ec8*/ BFC         R6, #12, #1 
    /*00001ecc*/ CMP         R1, #0 
    /*00001ed0*/ BFC         R6, #13, #1 
    /*00001ed4*/ STR         R6, [R3, #260]              @ 0x104 
    /*00001ed8*/ STREQ       R2, [R3, #320]              @ 0x140 
    /*00001edc*/ MOV         R1, #8 
    /*00001ee0*/ STREQ       R6, [R3, #324]              @ 0x144 
    /*00001ee4*/ LDR         R2, [R3, #40]               @ 0x28 
    /*00001ee8*/ BFI         R2, R1, #0, #4 
    /*00001eec*/ STR         R2, [R3, #40]               @ 0x28 
    /*00001ef0*/ LDR         R3, [R0] 
    /*00001ef4*/ LDR         R1, [R4, #8] 
    /*00001ef8*/ BFC         R3, #6, #1 
    /*00001efc*/ LDR         IP, [R0, #4] 
    /*00001f00*/ CMP         R1, #1 
    /*00001f04*/ BFC         R3, #19, #1 
    /*00001f08*/ BFCLS       R3, #0, #2 
    /*00001f0c*/ BFCLS       R3, #8, #8 
    /*00001f10*/ BLS         AIP_HAL_SetDevAttr_x1f38
    /*00001f14*/ SUB         R2, R1, #2 
    /*00001f18*/ BICS        R2, R2, #2 
    /*00001f1c*/ BEQ         AIP_HAL_SetDevAttr_x21b4
    /*00001f20*/ SUB         R2, R1, #3 
    /*00001f24*/ BICS        R2, R2, #2 
    /*00001f28*/ BNE         AIP_HAL_SetDevAttr_x21a0
    /*00001f2c*/ MOV         R0, #1 
    /*00001f30*/ BFI         R3, R0, #0, #2 
    /*00001f34*/ BFI         R3, R2, #8, #8 
    AIP_HAL_SetDevAttr_x1f38:
    /*00001f38*/ LDR         R2, [R4, #28] 
    /*00001f3c*/ CMP         R2, #1 
    /*00001f40*/ BFCEQ       R3, #7, #1 
    /*00001f44*/ BFCEQ       R3, #4, #2 
    /*00001f48*/ BEQ         AIP_HAL_SetDevAttr_x1f88
    /*00001f4c*/ CMP         R2, #2 
    /*00001f50*/ BEQ         AIP_HAL_SetDevAttr_x21f8
    /*00001f54*/ CMP         R2, #4 
    /*00001f58*/ ORREQ       R3, R3, #128                @ 0x80 
    /*00001f5c*/ MOVEQ       R2, #1 
    /*00001f60*/ BFIEQ       R3, R2, #4, #2 
    /*00001f64*/ BEQ         AIP_HAL_SetDevAttr_x1f88
    /*00001f68*/ CMP         R2, #8 
    /*00001f6c*/ ORREQ       R3, R3, #128                @ 0x80 
    /*00001f70*/ MOVEQ       R2, #2 
    /*00001f74*/ BFIEQ       R3, R2, #4, #2 
    /*00001f78*/ BEQ         AIP_HAL_SetDevAttr_x1f88
    /*00001f7c*/ CMP         R2, #16 
    /*00001f80*/ BNE         AIP_HAL_SetDevAttr_x21a0
    /*00001f84*/ ORR         R3, R3, #176                @ 0xb0 
    AIP_HAL_SetDevAttr_x1f88:
    /*00001f88*/ LDR         R2, [R4, #4] 
    /*00001f8c*/ CMP         R2, #0 
    /*00001f90*/ BNE         AIP_HAL_SetDevAttr_x2190
    AIP_HAL_SetDevAttr_x1f94:
    /*00001f94*/ BFC         IP, #0, #1 
    /*00001f98*/ BFI         R3, R2, #2, #2 
    /*00001f9c*/ BFC         IP, #1, #1 
    /*00001fa0*/ MOV         R2, #8 
    /*00001fa4*/ BFC         IP, #2, #1 
    /*00001fa8*/ BFI         R3, R2, #20, #4 
    /*00001fac*/ BFC         IP, #3, #1 
    /*00001fb0*/ LDR         R0, [R5, #192]              @ 0xc0 
    /*00001fb4*/ BFC         IP, #4, #4 
    /*00001fb8*/ BFC         R3, #24, #2 
    /*00001fbc*/ BFC         IP, #8, #7 
    /*00001fc0*/ MOV         R6, #1 
    /*00001fc4*/ BFC         IP, #15, #1 
    /*00001fc8*/ BFI         R3, R6, #26, #2 
    /*00001fcc*/ BFC         IP, #16, #4 
    /*00001fd0*/ ADD         R0, R0, #4096               @ 0x1000 
    /*00001fd4*/ BFC         IP, #20, #4 
    /*00001fd8*/ MOV         R2, #2 
    /*00001fdc*/ BFC         IP, #24, #3 
    /*00001fe0*/ BFI         R3, R2, #28, #2 
    /*00001fe4*/ BFC         IP, #27, #1 
    /*00001fe8*/ ORR         R3, R3, #-1073741824        @ 0xc0000000 
    /*00001fec*/ BFC         IP, #28, #1 
    /*00001ff0*/ BFC         R3, #16, #3 
    /*00001ff4*/ BFC         IP, #29, #1 
    /*00001ff8*/ STR         R3, [R0] 
    /*00001ffc*/ BFC         IP, #30, #2 
    /*00002000*/ MOV         R1, R4 
    /*00002004*/ STR         IP, [R0, #4] 
    /*00002008*/ MOV         R2, #40                     @ 0x28 
    /*0000200c*/ LDR         R0, [PC, #784]              @ 0x0000000000002324 
    /*00002010*/ BL          0x0000000000002010          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002014*/ LDR         R0, [PC, #780]              @ 0x0000000000002328 
    /*00002018*/ STR         R6, [R5, #4] 
    /*0000201c*/ BL          0x000000000000201C          @ osal_up [R_ARM_CALL: +0x0] 
    AIP_HAL_SetDevAttr_x2020:
    /*00002020*/ MOV         R0, R7 
    /*00002024*/ ADD         SP, SP, #28 
    /*00002028*/ POP         {R4, R5, R6, R7, PC} 
    AIP_HAL_SetDevAttr_x202c:
    /*0000202c*/ MOVW        R3, #1148                   @ 0x47c 
    /*00002030*/ MOV         R1, #20 
    /*00002034*/ STR         R3, [SP] 
    /*00002038*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*0000203c*/ LDR         R3, [PC, #744]              @ 0x000000000000232c 
    /*00002040*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00002044*/ STR         R5, [SP, #4] 
    /*00002048*/ MOV         R0, #3 
    /*0000204c*/ BL          0x000000000000204C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002050*/ MOVW        R2, #1149                   @ 0x47d 
    /*00002054*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00002058*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*0000205c*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00002060*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00002064*/ LDR         R1, [PC, #704]              @ 0x000000000000232c 
    /*00002068*/ BL          0x0000000000002068          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000206c*/ LDR         R2, [R4, #8] 
    /*00002070*/ SUB         R3, R2, #4 
    /*00002074*/ CMP         R2, #0 
    /*00002078*/ CMPNE       R3, #1 
    /*0000207c*/ BHI         AIP_HAL_SetDevAttr_x2138
    AIP_HAL_SetDevAttr_x2080:
    /*00002080*/ LDR         R1, [R4] 
    /*00002084*/ MOVW        R3, #12000                  @ 0x2ee0 
    /*00002088*/ MOVW        LR, #11025                  @ 0x2b11 
    /*0000208c*/ MOVW        IP, #22050                  @ 0x5622 
    /*00002090*/ CMP         R1, #8000                   @ 0x1f40 
    /*00002094*/ CMPNE       R1, R3 
    /*00002098*/ MOVW        R0, #24000                  @ 0x5dc0 
    /*0000209c*/ MOVNE       R3, #1 
    /*000020a0*/ MOVEQ       R3, #0 
    /*000020a4*/ CMP         R1, LR 
    /*000020a8*/ MOVEQ       R3, #0 
    /*000020ac*/ ANDNE       R3, R3, #1 
    /*000020b0*/ MOVW        LR, #44100                  @ 0xac44 
    /*000020b4*/ CMP         R1, #16000                  @ 0x3e80 
    /*000020b8*/ MOVEQ       R3, #0 
    /*000020bc*/ ANDNE       R3, R3, #1 
    /*000020c0*/ CMP         R1, IP 
    /*000020c4*/ MOVEQ       R3, #0 
    /*000020c8*/ ANDNE       R3, R3, #1 
    /*000020cc*/ MOVW        IP, #48000                  @ 0xbb80 
    /*000020d0*/ CMP         R1, R0 
    /*000020d4*/ MOVEQ       R3, #0 
    /*000020d8*/ ANDNE       R3, R3, #1 
    /*000020dc*/ MOV         R0, #30464                  @ 0x7700 
    /*000020e0*/ CMP         R1, #32000                  @ 0x7d00 
    /*000020e4*/ MOVEQ       R3, #0 
    /*000020e8*/ ANDNE       R3, R3, #1 
    /*000020ec*/ MOVT        R0, #1 
    /*000020f0*/ CMP         R1, LR 
    /*000020f4*/ MOVEQ       R3, #0 
    /*000020f8*/ ANDNE       R3, R3, #1 
    /*000020fc*/ CMP         R1, IP 
    /*00002100*/ MOVEQ       R3, #0 
    /*00002104*/ ANDNE       R3, R3, #1 
    /*00002108*/ CMP         R1, #64000                  @ 0xfa00 
    /*0000210c*/ MOVEQ       R3, #0 
    /*00002110*/ ANDNE       R3, R3, #1 
    /*00002114*/ CMP         R1, R0 
    /*00002118*/ MOVEQ       R3, #0 
    /*0000211c*/ ANDNE       R3, R3, #1 
    /*00002120*/ CMP         R3, #0 
    /*00002124*/ MOVWNE      R3, #1028                   @ 0x404 
    /*00002128*/ MOVWNE      R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x55c] 
    /*0000212c*/ STRNE       R3, [SP] 
    /*00002130*/ MOVTNE      R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x55c] 
    /*00002134*/ BNE         AIP_HAL_SetDevAttr_x2174
    AIP_HAL_SetDevAttr_x2138:
    /*00002138*/ CMP         R5, #0 
    /*0000213c*/ BEQ         AIP_HAL_SetDevAttr_x1d3c
    /*00002140*/ MOVW        R3, #1044                   @ 0x414 
    /*00002144*/ MOVW        R2, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5f8] 
    /*00002148*/ STR         R5, [SP, #4] 
    /*0000214c*/ MOVT        R2, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5f8] 
    /*00002150*/ STR         R3, [SP] 
    /*00002154*/ B           AIP_HAL_SetDevAttr_x2228
    AIP_HAL_SetDevAttr_x2158:
    /*00002158*/ LDR         R3, [R4, #12] 
    /*0000215c*/ CMP         R3, #1 
    /*00002160*/ MOVWEQ      R3, #1063                   @ 0x427 
    /*00002164*/ MOVWEQ      R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*00002168*/ STREQ       R3, [SP] 
    /*0000216c*/ MOVTEQ      R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*00002170*/ BNE         AIP_HAL_SetDevAttr_x1d80
    AIP_HAL_SetDevAttr_x2174:
    /*00002174*/ LDR         R3, [PC, #436]              @ 0x0000000000002330 
    /*00002178*/ MOV         R1, #20 
    /*0000217c*/ MOV         R0, #3 
    /*00002180*/ MOVW        R7, #32771                  @ 0x8003 
    /*00002184*/ BL          0x0000000000002184          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002188*/ MOVT        R7, #40981                  @ 0xa015 
    /*0000218c*/ B           AIP_HAL_SetDevAttr_x2020
    AIP_HAL_SetDevAttr_x2190:
    /*00002190*/ CMP         R2, #1 
    /*00002194*/ BEQ         AIP_HAL_SetDevAttr_x1f94
    /*00002198*/ CMP         R2, #2 
    /*0000219c*/ BEQ         AIP_HAL_SetDevAttr_x1f94
    AIP_HAL_SetDevAttr_x21a0:
    /*000021a0*/ LDR         R0, [PC, #384]              @ 0x0000000000002328 
    /*000021a4*/ MOVW        R7, #32776                  @ 0x8008 
    /*000021a8*/ BL          0x00000000000021A8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000021ac*/ MOVT        R7, #40981                  @ 0xa015 
    /*000021b0*/ B           AIP_HAL_SetDevAttr_x2020
    AIP_HAL_SetDevAttr_x21b4:
    /*000021b4*/ MOV         R2, #1 
    /*000021b8*/ BFI         R3, R2, #0, #2 
    /*000021bc*/ BFI         R3, R2, #8, #8 
    /*000021c0*/ B           AIP_HAL_SetDevAttr_x1f38
    AIP_HAL_SetDevAttr_x21c4:
    /*000021c4*/ LDR         R3, [R5, #8] 
    /*000021c8*/ CMP         R3, #1 
    /*000021cc*/ BNE         AIP_HAL_SetDevAttr_x1e14
    /*000021d0*/ MOV         R1, R4 
    /*000021d4*/ LDR         R0, [PC, #344]              @ 0x0000000000002334 
    /*000021d8*/ BL          0x00000000000021D8          @ AIAO_HAL_CheckAiAoClk [R_ARM_CALL: .text+0x1c78] 
    /*000021dc*/ CMP         R0, #0 
    /*000021e0*/ BEQ         AIP_HAL_SetDevAttr_x1e14
    AIP_HAL_SetDevAttr_x21e4:
    /*000021e4*/ LDR         R0, [PC, #316]              @ 0x0000000000002328 
    /*000021e8*/ MOVW        R7, #32771                  @ 0x8003 
    /*000021ec*/ BL          0x00000000000021EC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000021f0*/ MOVT        R7, #40981                  @ 0xa015 
    /*000021f4*/ B           AIP_HAL_SetDevAttr_x2020
    AIP_HAL_SetDevAttr_x21f8:
    /*000021f8*/ CMP         R1, #1 
    /*000021fc*/ BFCLS       R3, #7, #1 
    /*00002200*/ ORRHI       R3, R3, #128                @ 0x80 
    /*00002204*/ MOVLS       R2, #1 
    /*00002208*/ BFCHI       R3, #4, #2 
    /*0000220c*/ BFILS       R3, R2, #4, #2 
    /*00002210*/ B           AIP_HAL_SetDevAttr_x1f88
    AIP_HAL_SetDevAttr_x2214:
    /*00002214*/ MOVW        R3, #1051                   @ 0x41b 
    /*00002218*/ STR         R1, [SP, #4] 
    AIP_HAL_SetDevAttr_x221c:
    /*0000221c*/ MOVW        R2, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x628] 
    /*00002220*/ MOVT        R2, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x628] 
    /*00002224*/ STR         R3, [SP] 
    AIP_HAL_SetDevAttr_x2228:
    /*00002228*/ LDR         R3, [PC, #256]              @ 0x0000000000002330 
    /*0000222c*/ MOV         R1, #20 
    /*00002230*/ MOV         R0, #3 
    /*00002234*/ MOVW        R7, #32771                  @ 0x8003 
    /*00002238*/ BL          0x0000000000002238          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000223c*/ MOVT        R7, #40981                  @ 0xa015 
    /*00002240*/ MOV         R0, R7 
    /*00002244*/ ADD         SP, SP, #28 
    /*00002248*/ POP         {R4, R5, R6, R7, PC} 
    AIP_HAL_SetDevAttr_x224c:
    /*0000224c*/ MOVW        R3, #950                    @ 0x3b6 
    /*00002250*/ STR         R1, [SP, #4] 
    /*00002254*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x808] 
    /*00002258*/ STR         R3, [SP] 
    /*0000225c*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x808] 
    /*00002260*/ LDR         R3, [PC, #208]              @ 0x0000000000002338 
    /*00002264*/ MOV         R1, #20 
    /*00002268*/ MOV         R0, #3 
    /*0000226c*/ BL          0x000000000000226C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002270*/ B           AIP_HAL_SetDevAttr_x21e4
    AIP_HAL_SetDevAttr_x2274:
    /*00002274*/ STR         R3, [SP, #4] 
    /*00002278*/ MOVW        R2, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x69c] 
    /*0000227c*/ MOVW        R3, #1070                   @ 0x42e 
    /*00002280*/ MOVT        R2, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x69c] 
    /*00002284*/ STR         R3, [SP] 
    /*00002288*/ B           AIP_HAL_SetDevAttr_x2228
    AIP_HAL_SetDevAttr_x228c:
    /*0000228c*/ STR         R3, [SP, #4] 
    /*00002290*/ MOVW        R2, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x704] 
    /*00002294*/ MOVW        R3, #1102                   @ 0x44e 
    /*00002298*/ MOVT        R2, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x704] 
    /*0000229c*/ STR         R3, [SP] 
    /*000022a0*/ B           AIP_HAL_SetDevAttr_x2228
    AIP_HAL_SetDevAttr_x22a4:
    /*000022a4*/ STR         R3, [SP, #4] 
    /*000022a8*/ MOVW        R2, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7d4] 
    /*000022ac*/ MOVW        R3, #1122                   @ 0x462 
    /*000022b0*/ MOVT        R2, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7d4] 
    /*000022b4*/ STR         R3, [SP] 
    /*000022b8*/ B           AIP_HAL_SetDevAttr_x2228
    AIP_HAL_SetDevAttr_x22bc:
    /*000022bc*/ MOV         R7, #65024                  @ 0xfe00 
    /*000022c0*/ MOVT        R7, #65535                  @ 0xffff 
    /*000022c4*/ B           AIP_HAL_SetDevAttr_x2020
    AIP_HAL_SetDevAttr_x22c8:
    /*000022c8*/ MOVW        R3, #1039                   @ 0x40f 
    /*000022cc*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x590] 
    /*000022d0*/ STR         R3, [SP] 
    /*000022d4*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x590] 
    /*000022d8*/ B           AIP_HAL_SetDevAttr_x2174
    AIP_HAL_SetDevAttr_x22dc:
    /*000022dc*/ STR         R2, [SP, #4] 
    /*000022e0*/ MOVW        R3, #1097                   @ 0x449 
    /*000022e4*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6d0] 
    /*000022e8*/ STR         R3, [SP] 
    /*000022ec*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6d0] 
    /*000022f0*/ B           AIP_HAL_SetDevAttr_x2228
    AIP_HAL_SetDevAttr_x22f4:
    /*000022f4*/ STR         R3, [SP, #4] 
    /*000022f8*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x738] 
    /*000022fc*/ MOVW        R3, #1107                   @ 0x453 
    /*00002300*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x738] 
    /*00002304*/ STR         R3, [SP] 
    /*00002308*/ B           AIP_HAL_SetDevAttr_x2228
    AIP_HAL_SetDevAttr_x230c:
    /*0000230c*/ STR         R3, [SP, #4] 
    /*00002310*/ MOVW        R2, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x76c] 
    /*00002314*/ MOVW        R3, #1112                   @ 0x458 
    /*00002318*/ MOVT        R2, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x76c] 
    /*0000231c*/ STR         R3, [SP] 
    /*00002320*/ B           AIP_HAL_SetDevAttr_x2228
    /*00002324*/ ANDEQ       R0, R0, IP 
    /*00002328*/ ANDEQ       R0, R0, R4, ASR #1 
    /*0000232c*/ LDRDEQ      R0, [R0], -R8 
    /*00002330*/ ANDEQ       R0, R0, IP, ROR #5 
    /*00002334*/ ANDEQ       R0, R0, R4, LSR R0 
    /*00002338*/ STRDEQ      R0, [R0], -IP 
FUNC_END AIP_HAL_SetDevAttr


FUNC_BEGIN AOP_HAL_SetDevAttr
    /*0000233c*/ CMP         R0, #1 
    /*00002340*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00002344*/ MOV         R5, R0 
    /*00002348*/ SUB         SP, SP, #24 
    /*0000234c*/ MOV         R4, R1 
    /*00002350*/ BLS         AOP_HAL_SetDevAttr_x2394
    /*00002354*/ STR         R0, [SP, #4] 
    /*00002358*/ MOVW        R3, #2249                   @ 0x8c9 
    /*0000235c*/ MOV         R1, #20 
    /*00002360*/ STR         R3, [SP] 
    /*00002364*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00002368*/ LDR         R3, [PC, #1788]             @ 0x0000000000002a6c 
    /*0000236c*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00002370*/ MOV         R0, #3 
    /*00002374*/ BL          0x0000000000002374          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002378*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*0000237c*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00002380*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00002384*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00002388*/ MOVW        R2, #2250                   @ 0x8ca 
    /*0000238c*/ LDR         R1, [PC, #1752]             @ 0x0000000000002a6c 
    /*00002390*/ BL          0x0000000000002390          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_SetDevAttr_x2394:
    /*00002394*/ LDR         R2, [R4, #8] 
    /*00002398*/ SUB         R3, R2, #4 
    /*0000239c*/ CMP         R2, #0 
    /*000023a0*/ CMPNE       R3, #1 
    /*000023a4*/ BLS         AOP_HAL_SetDevAttr_x2690
    AOP_HAL_SetDevAttr_x23a8:
    /*000023a8*/ MOVW        R3, #0                      @ reg_otp_base_va [R_ARM_MOVW_ABS_NC: +0x0] 
    /*000023ac*/ MOVT        R3, #0                      @ reg_otp_base_va [R_ARM_MOVT_ABS: +0x0] 
    /*000023b0*/ LDR         R3, [R3] 
    /*000023b4*/ LDRB        R3, [R3, #40]               @ 0x28 
    /*000023b8*/ CMP         R3, #0 
    /*000023bc*/ BNE         AOP_HAL_SetDevAttr_x2764
    AOP_HAL_SetDevAttr_x23c0:
    /*000023c0*/ CMP         R5, #0 
    /*000023c4*/ BEQ         AOP_HAL_SetDevAttr_x27b0
    /*000023c8*/ CMP         R5, #1 
    /*000023cc*/ BNE         AOP_HAL_SetDevAttr_x279c
    /*000023d0*/ LDR         R3, [R4, #36]               @ 0x24 
    /*000023d4*/ CMP         R3, #1 
    /*000023d8*/ BNE         AOP_HAL_SetDevAttr_x29d4
    AOP_HAL_SetDevAttr_x23dc:
    /*000023dc*/ LDR         R3, [R4, #28] 
    /*000023e0*/ CMP         R3, #3 
    /*000023e4*/ STRHI       R3, [SP, #4] 
    /*000023e8*/ MOVHI       R3, #2144                   @ 0x860 
    /*000023ec*/ BHI         AOP_HAL_SetDevAttr_x2984
    /*000023f0*/ SUB         R1, R3, #1 
    /*000023f4*/ CMP         R1, #1 
    /*000023f8*/ BHI         AOP_HAL_SetDevAttr_x297c
    /*000023fc*/ CMP         R3, #1 
    /*00002400*/ BEQ         AOP_HAL_SetDevAttr_x27dc
    AOP_HAL_SetDevAttr_x2404:
    /*00002404*/ LDR         R1, [R4, #4] 
    /*00002408*/ SUB         R0, R1, #1 
    /*0000240c*/ CMP         R0, #1 
    /*00002410*/ BHI         AOP_HAL_SetDevAttr_x29a4
    /*00002414*/ CMP         R2, #5 
    /*00002418*/ SUB         R2, R2, #2 
    /*0000241c*/ MOVHI       R3, #2192                   @ 0x890 
    /*00002420*/ MOVWHI      R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x928] 
    /*00002424*/ STRHI       R3, [SP] 
    /*00002428*/ MOVTHI      R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x928] 
    /*0000242c*/ BHI         AOP_HAL_SetDevAttr_x2748
    /*00002430*/ LDR         R1, [R4, #12] 
    /*00002434*/ CMP         R1, #1 
    /*00002438*/ MOVWHI      R3, #2197                   @ 0x895 
    /*0000243c*/ MOVWHI      R2, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x958] 
    /*00002440*/ STRHI       R3, [SP] 
    /*00002444*/ MOVTHI      R2, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x958] 
    /*00002448*/ BHI         AOP_HAL_SetDevAttr_x2748
    /*0000244c*/ LDR         R1, [R4, #20] 
    /*00002450*/ MOVW        R0, #298                    @ 0x12a 
    /*00002454*/ SUB         R1, R1, #2 
    /*00002458*/ CMP         R1, R0 
    /*0000245c*/ MOVWHI      R3, #2202                   @ 0x89a 
    /*00002460*/ MOVWHI      R2, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x988] 
    /*00002464*/ STRHI       R3, [SP] 
    /*00002468*/ MOVTHI      R2, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x988] 
    /*0000246c*/ BHI         AOP_HAL_SetDevAttr_x2748
    /*00002470*/ LDR         R1, [R4, #24] 
    /*00002474*/ SUB         R0, R1, #80                 @ 0x50 
    /*00002478*/ CMP         R0, #4016                   @ 0xfb0 
    /*0000247c*/ BHI         AOP_HAL_SetDevAttr_x29fc
    /*00002480*/ LDR         R1, [R4, #16] 
    /*00002484*/ CMP         R1, #2 
    /*00002488*/ MOVWHI      R3, #2212                   @ 0x8a4 
    /*0000248c*/ MOVWHI      R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7a4] 
    /*00002490*/ STRHI       R3, [SP] 
    /*00002494*/ MOVTHI      R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7a4] 
    /*00002498*/ BHI         AOP_HAL_SetDevAttr_x2748
    /*0000249c*/ LDR         R1, [R4, #32] 
    /*000024a0*/ CMP         R1, #1 
    /*000024a4*/ MOVWHI      R3, #2217                   @ 0x8a9 
    /*000024a8*/ MOVWHI      R2, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9b8] 
    /*000024ac*/ STRHI       R3, [SP] 
    /*000024b0*/ MOVTHI      R2, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9b8] 
    /*000024b4*/ BHI         AOP_HAL_SetDevAttr_x2748
    /*000024b8*/ CMP         R3, #2 
    /*000024bc*/ BNE         AOP_HAL_SetDevAttr_x24c8
    /*000024c0*/ CMP         R2, #3 
    /*000024c4*/ BLS         AOP_HAL_SetDevAttr_x2a14
    AOP_HAL_SetDevAttr_x24c8:
    /*000024c8*/ MOVW        R7, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000024cc*/ MOVT        R7, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000024d0*/ ADD         R0, R7, #196                @ 0xc4 
    /*000024d4*/ BL          0x00000000000024D4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000024d8*/ CMP         R0, #0 
    /*000024dc*/ BNE         AOP_HAL_SetDevAttr_x29bc
    /*000024e0*/ CMP         R5, #0 
    /*000024e4*/ BEQ         AOP_HAL_SetDevAttr_x2894
    AOP_HAL_SetDevAttr_x24e8:
    /*000024e8*/ LDR         IP, [R7, #192]              @ 0xc0 
    /*000024ec*/ LSL         R9, R5, #3 
    /*000024f0*/ ADD         SL, R9, #324                @ 0x144 
    /*000024f4*/ ADD         R2, SP, #20 
    /*000024f8*/ ADD         R1, SP, #8 
    /*000024fc*/ MOV         R0, R4 
    /*00002500*/ MOV         R3, #0 
    /*00002504*/ STR         R3, [SP, #8] 
    /*00002508*/ STR         R3, [SP, #12] 
    /*0000250c*/ STR         R3, [SP, #16] 
    /*00002510*/ LDR         R8, [IP, SL] 
    /*00002514*/ BL          0x0000000000001938 
    /*00002518*/ MOV         R6, R0 
    /*0000251c*/ ADD         R3, SP, #12 
    /*00002520*/ MOV         R0, R4 
    /*00002524*/ ADD         R2, SP, #16 
    /*00002528*/ LDR         R1, [SP, #8] 
    /*0000252c*/ BL          0x0000000000000124 
    /*00002530*/ ORRS        R6, R6, R0 
    /*00002534*/ BNE         AOP_HAL_SetDevAttr_x29c8
    /*00002538*/ LDR         R1, [SP, #8] 
    /*0000253c*/ ADD         R9, R9, #320                @ 0x140 
    /*00002540*/ LDR         R2, [R7, #192]              @ 0xc0 
    /*00002544*/ MOV         R3, #0 
    /*00002548*/ BFI         R3, R1, #0, #27 
    /*0000254c*/ BFI         R3, R6, #27, #5 
    /*00002550*/ STR         R3, [R2, R9] 
    /*00002554*/ LDR         R1, [SP, #16] 
    /*00002558*/ LDR         R3, [R4, #8] 
    /*0000255c*/ LDR         R2, [SP, #12] 
    /*00002560*/ BFI         R8, R1, #0, #4 
    /*00002564*/ SUB         R0, R3, #4 
    /*00002568*/ CMP         R3, #0 
    /*0000256c*/ CMPNE       R0, #1 
    /*00002570*/ BFI         R8, R2, #4, #3 
    /*00002574*/ BFILS       R8, R6, #10, #1 
    /*00002578*/ BFILS       R8, R6, #11, #1 
    /*0000257c*/ BLS         AOP_HAL_SetDevAttr_x2590
    /*00002580*/ SUB         R2, R3, #1 
    /*00002584*/ CMP         R2, #2 
    /*00002588*/ BHI         AOP_HAL_SetDevAttr_x294c
    /*0000258c*/ ORR         R8, R8, #3072               @ 0xc00 
    AOP_HAL_SetDevAttr_x2590:
    /*00002590*/ LDR         R3, [R7, #192]              @ 0xc0 
    /*00002594*/ ORR         R8, R8, #256                @ 0x100 
    /*00002598*/ BFC         R8, #9, #1 
    /*0000259c*/ CMP         R5, #1 
    /*000025a0*/ BFC         R8, #12, #1 
    /*000025a4*/ BFC         R8, #13, #1 
    /*000025a8*/ STR         R8, [R3, SL] 
    /*000025ac*/ LDR         R2, [R7, #192]              @ 0xc0 
    /*000025b0*/ LDR         R3, [R2, #44]               @ 0x2c 
    /*000025b4*/ BEQ         AOP_HAL_SetDevAttr_x2938
    /*000025b8*/ BCC         AOP_HAL_SetDevAttr_x27d0
    /*000025bc*/ CMP         R5, #2 
    /*000025c0*/ BNE         AOP_HAL_SetDevAttr_x28f8
    /*000025c4*/ MOV         R1, #10 
    /*000025c8*/ BFI         R3, R1, #8, #4 
    AOP_HAL_SetDevAttr_x25cc:
    /*000025cc*/ ADD         R0, R5, #32 
    /*000025d0*/ STR         R3, [R2, #44]               @ 0x2c 
    /*000025d4*/ LDR         R1, [R4, #8] 
    /*000025d8*/ LDR         R3, [R2, R0, LSL #8] 
    /*000025dc*/ LSL         IP, R0, #8 
    /*000025e0*/ CMP         R1, #1 
    /*000025e4*/ ADD         IP, IP, #4 
    /*000025e8*/ ORR         R3, R3, #64                 @ 0x40 
    /*000025ec*/ BFC         R3, #7, #1 
    /*000025f0*/ LDR         R2, [R2, IP] 
    /*000025f4*/ BFC         R3, #19, #1 
    /*000025f8*/ BFCLS       R3, #0, #2 
    /*000025fc*/ BFCLS       R3, #8, #8 
    /*00002600*/ BLS         AOP_HAL_SetDevAttr_x261c
    /*00002604*/ SUB         LR, R1, #2 
    /*00002608*/ BICS        LR, LR, #2 
    /*0000260c*/ BNE         AOP_HAL_SetDevAttr_x28dc
    /*00002610*/ MOV         R1, #1 
    /*00002614*/ BFI         R3, R1, #0, #2 
    /*00002618*/ BFI         R3, R1, #8, #8 
    AOP_HAL_SetDevAttr_x261c:
    /*0000261c*/ LDR         R1, [R4, #28] 
    /*00002620*/ CMP         R1, #1 
    /*00002624*/ BFCEQ       R3, #4, #2 
    /*00002628*/ BEQ         AOP_HAL_SetDevAttr_x2648
    /*0000262c*/ CMP         R1, #2 
    /*00002630*/ MOVEQ       R1, #1 
    /*00002634*/ BFIEQ       R3, R1, #4, #2 
    /*00002638*/ BEQ         AOP_HAL_SetDevAttr_x2648
    /*0000263c*/ CMP         R1, #8 
    /*00002640*/ BNE         AOP_HAL_SetDevAttr_x29e8
    /*00002644*/ ORR         R3, R3, #48                 @ 0x30 
    AOP_HAL_SetDevAttr_x2648:
    /*00002648*/ LDR         R1, [R4, #4] 
    /*0000264c*/ CMP         R1, #0 
    /*00002650*/ BEQ         AOP_HAL_SetDevAttr_x2664
    /*00002654*/ CMP         R1, #1 
    /*00002658*/ BEQ         AOP_HAL_SetDevAttr_x2664
    /*0000265c*/ CMP         R1, #2 
    /*00002660*/ BNE         AOP_HAL_SetDevAttr_x29e8
    AOP_HAL_SetDevAttr_x2664:
    /*00002664*/ CMP         R5, #1 
    /*00002668*/ BFI         R3, R1, #2, #2 
    /*0000266c*/ BEQ         AOP_HAL_SetDevAttr_x27fc
    /*00002670*/ BCC         AOP_HAL_SetDevAttr_x2944
    /*00002674*/ CMP         R5, #2 
    /*00002678*/ BEQ         AOP_HAL_SetDevAttr_x27fc
    /*0000267c*/ LDR         R0, [PC, #1004]             @ 0x0000000000002a70 
    /*00002680*/ MOVW        R6, #32769                  @ 0x8001 
    /*00002684*/ BL          0x0000000000002684          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002688*/ MOVT        R6, #40982                  @ 0xa016 
    /*0000268c*/ B           AOP_HAL_SetDevAttr_x27a4
    AOP_HAL_SetDevAttr_x2690:
    /*00002690*/ LDR         R1, [R4] 
    /*00002694*/ MOVW        R3, #12000                  @ 0x2ee0 
    /*00002698*/ MOVW        LR, #11025                  @ 0x2b11 
    /*0000269c*/ MOVW        IP, #22050                  @ 0x5622 
    /*000026a0*/ CMP         R1, #8000                   @ 0x1f40 
    /*000026a4*/ CMPNE       R1, R3 
    /*000026a8*/ MOVW        R0, #24000                  @ 0x5dc0 
    /*000026ac*/ MOVNE       R3, #1 
    /*000026b0*/ MOVEQ       R3, #0 
    /*000026b4*/ CMP         R1, LR 
    /*000026b8*/ MOVEQ       R3, #0 
    /*000026bc*/ ANDNE       R3, R3, #1 
    /*000026c0*/ MOVW        LR, #44100                  @ 0xac44 
    /*000026c4*/ CMP         R1, #16000                  @ 0x3e80 
    /*000026c8*/ MOVEQ       R3, #0 
    /*000026cc*/ ANDNE       R3, R3, #1 
    /*000026d0*/ CMP         R1, IP 
    /*000026d4*/ MOVEQ       R3, #0 
    /*000026d8*/ ANDNE       R3, R3, #1 
    /*000026dc*/ MOVW        IP, #48000                  @ 0xbb80 
    /*000026e0*/ CMP         R1, R0 
    /*000026e4*/ MOVEQ       R3, #0 
    /*000026e8*/ ANDNE       R3, R3, #1 
    /*000026ec*/ MOV         R0, #30464                  @ 0x7700 
    /*000026f0*/ CMP         R1, #32000                  @ 0x7d00 
    /*000026f4*/ MOVEQ       R3, #0 
    /*000026f8*/ ANDNE       R3, R3, #1 
    /*000026fc*/ MOVT        R0, #1 
    /*00002700*/ CMP         R1, LR 
    /*00002704*/ MOVEQ       R3, #0 
    /*00002708*/ ANDNE       R3, R3, #1 
    /*0000270c*/ CMP         R1, IP 
    /*00002710*/ MOVEQ       R3, #0 
    /*00002714*/ ANDNE       R3, R3, #1 
    /*00002718*/ CMP         R1, #64000                  @ 0xfa00 
    /*0000271c*/ MOVEQ       R3, #0 
    /*00002720*/ ANDNE       R3, R3, #1 
    /*00002724*/ CMP         R1, R0 
    /*00002728*/ MOVEQ       R3, #0 
    /*0000272c*/ ANDNE       R3, R3, #1 
    /*00002730*/ CMP         R3, #0 
    /*00002734*/ MOVWNE      R3, #2109                   @ 0x83d 
    /*00002738*/ MOVWNE      R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x55c] 
    /*0000273c*/ STRNE       R3, [SP] 
    /*00002740*/ MOVTNE      R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x55c] 
    /*00002744*/ BEQ         AOP_HAL_SetDevAttr_x23a8
    AOP_HAL_SetDevAttr_x2748:
    /*00002748*/ LDR         R3, [PC, #804]              @ 0x0000000000002a74 
    /*0000274c*/ MOV         R1, #20 
    /*00002750*/ MOV         R0, #3 
    /*00002754*/ MOVW        R6, #32771                  @ 0x8003 
    /*00002758*/ BL          0x0000000000002758          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000275c*/ MOVT        R6, #40982                  @ 0xa016 
    /*00002760*/ B           AOP_HAL_SetDevAttr_x27a4
    AOP_HAL_SetDevAttr_x2764:
    /*00002764*/ SUB         R3, R3, #5 
    /*00002768*/ CMP         R3, #7 
    /*0000276c*/ BHI         AOP_HAL_SetDevAttr_x2780
    /*00002770*/ MOV         R1, #147                    @ 0x93 
    /*00002774*/ MVN         R3, R1, LSR R3 
    /*00002778*/ TST         R3, #1 
    /*0000277c*/ BEQ         AOP_HAL_SetDevAttr_x23c0
    AOP_HAL_SetDevAttr_x2780:
    /*00002780*/ LDR         R3, [R4, #36]               @ 0x24 
    /*00002784*/ CMP         R3, #1 
    /*00002788*/ BEQ         AOP_HAL_SetDevAttr_x2a40
    /*0000278c*/ CMP         R5, #0 
    /*00002790*/ BEQ         AOP_HAL_SetDevAttr_x27b4
    /*00002794*/ CMP         R5, #1 
    /*00002798*/ BEQ         AOP_HAL_SetDevAttr_x29d4
    AOP_HAL_SetDevAttr_x279c:
    /*0000279c*/ MOVW        R6, #32771                  @ 0x8003 
    /*000027a0*/ MOVT        R6, #40982                  @ 0xa016 
    AOP_HAL_SetDevAttr_x27a4:
    /*000027a4*/ MOV         R0, R6 
    /*000027a8*/ ADD         SP, SP, #24 
    /*000027ac*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AOP_HAL_SetDevAttr_x27b0:
    /*000027b0*/ LDR         R3, [R4, #36]               @ 0x24 
    AOP_HAL_SetDevAttr_x27b4:
    /*000027b4*/ BICS        R3, R3, #2 
    /*000027b8*/ BEQ         AOP_HAL_SetDevAttr_x23dc
    /*000027bc*/ MOVW        R3, #2127                   @ 0x84f 
    /*000027c0*/ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x870] 
    /*000027c4*/ STR         R3, [SP] 
    /*000027c8*/ MOVT        R2, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x870] 
    /*000027cc*/ B           AOP_HAL_SetDevAttr_x2748
    AOP_HAL_SetDevAttr_x27d0:
    /*000027d0*/ MOV         R1, #8 
    /*000027d4*/ BFI         R3, R1, #0, #4 
    /*000027d8*/ B           AOP_HAL_SetDevAttr_x25cc
    AOP_HAL_SetDevAttr_x27dc:
    /*000027dc*/ LDR         R1, [R4, #12] 
    /*000027e0*/ CMP         R1, #1 
    /*000027e4*/ MOVWEQ      R3, #2157                   @ 0x86d 
    /*000027e8*/ MOVWEQ      R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x65c] 
    /*000027ec*/ STREQ       R3, [SP] 
    /*000027f0*/ MOVTEQ      R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x65c] 
    /*000027f4*/ BNE         AOP_HAL_SetDevAttr_x2404
    /*000027f8*/ B           AOP_HAL_SetDevAttr_x2748
    AOP_HAL_SetDevAttr_x27fc:
    /*000027fc*/ BFI         R3, R5, #20, #4 
    AOP_HAL_SetDevAttr_x2800:
    /*00002800*/ BFC         R2, #0, #1 
    /*00002804*/ BFC         R3, #24, #2 
    /*00002808*/ BFC         R2, #1, #1 
    /*0000280c*/ MOV         R8, #1 
    /*00002810*/ BFC         R2, #2, #6 
    /*00002814*/ MOV         R1, #121                    @ 0x79 
    /*00002818*/ BFI         R3, R8, #26, #2 
    /*0000281c*/ BFI         R2, R1, #8, #7 
    /*00002820*/ MOV         LR, #2 
    /*00002824*/ LDR         R1, [R7, #192]              @ 0xc0 
    /*00002828*/ BFI         R3, LR, #28, #2 
    /*0000282c*/ BFC         R2, #15, #1 
    /*00002830*/ MOV         LR, #92                     @ 0x5c 
    /*00002834*/ BFC         R2, #16, #4 
    /*00002838*/ MUL         R5, LR, R5 
    /*0000283c*/ BFC         R2, #20, #4 
    /*00002840*/ ORR         R3, R3, #-1073741824        @ 0xc0000000 
    /*00002844*/ BFC         R2, #24, #3 
    /*00002848*/ BFC         R3, #16, #3 
    /*0000284c*/ STR         R3, [R1, R0, LSL #8] 
    /*00002850*/ LDR         R3, [R7, #192]              @ 0xc0 
    /*00002854*/ BFC         R2, #27, #1 
    /*00002858*/ ADD         R0, R5, #52                 @ 0x34 
    /*0000285c*/ BFC         R2, #28, #1 
    /*00002860*/ ADD         R5, R7, R5 
    /*00002864*/ MOV         R1, R4 
    /*00002868*/ BFC         R2, #30, #2 
    /*0000286c*/ ADD         R0, R7, R0 
    /*00002870*/ STR         R2, [R3, IP] 
    /*00002874*/ MOV         R2, #40                     @ 0x28 
    /*00002878*/ BL          0x0000000000002878          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000287c*/ LDR         R0, [PC, #492]              @ 0x0000000000002a70 
    /*00002880*/ STR         R8, [R5, #8] 
    /*00002884*/ BL          0x0000000000002884          @ osal_up [R_ARM_CALL: +0x0] 
    /*00002888*/ MOV         R0, R6 
    /*0000288c*/ ADD         SP, SP, #24 
    /*00002890*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AOP_HAL_SetDevAttr_x2894:
    /*00002894*/ MOV         R0, R4 
    /*00002898*/ BL          0x000000000000151C 
    /*0000289c*/ LDR         R3, [R4, #32] 
    /*000028a0*/ CMP         R3, #1 
    /*000028a4*/ BNE         AOP_HAL_SetDevAttr_x24e8
    /*000028a8*/ LDR         R3, [R7, #4] 
    /*000028ac*/ CMP         R3, #1 
    /*000028b0*/ BNE         AOP_HAL_SetDevAttr_x24e8
    /*000028b4*/ MOV         R1, R4 
    /*000028b8*/ ADD         R0, R7, #12 
    /*000028bc*/ BL          0x00000000000028BC          @ AIAO_HAL_CheckAiAoClk [R_ARM_CALL: .text+0x1c78] 
    /*000028c0*/ CMP         R0, #0 
    /*000028c4*/ BEQ         AOP_HAL_SetDevAttr_x24e8
    /*000028c8*/ ADD         R0, R7, #196                @ 0xc4 
    /*000028cc*/ MOVW        R6, #32771                  @ 0x8003 
    /*000028d0*/ BL          0x00000000000028D0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000028d4*/ MOVT        R6, #40982                  @ 0xa016 
    /*000028d8*/ B           AOP_HAL_SetDevAttr_x27a4
    AOP_HAL_SetDevAttr_x28dc:
    /*000028dc*/ SUB         R1, R1, #3 
    /*000028e0*/ BICS        R1, R1, #2 
    /*000028e4*/ BNE         AOP_HAL_SetDevAttr_x29e8
    /*000028e8*/ MOV         LR, #1 
    /*000028ec*/ BFI         R3, LR, #0, #2 
    /*000028f0*/ BFI         R3, R1, #8, #8 
    /*000028f4*/ B           AOP_HAL_SetDevAttr_x261c
    AOP_HAL_SetDevAttr_x28f8:
    /*000028f8*/ MOVW        R3, #2082                   @ 0x822 
    /*000028fc*/ STR         R5, [SP, #4] 
    /*00002900*/ STR         R3, [SP] 
    /*00002904*/ MOVW        R2, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa38] 
    /*00002908*/ LDR         R3, [PC, #360]              @ 0x0000000000002a78 
    /*0000290c*/ MOVT        R2, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa38] 
    /*00002910*/ MOV         R1, #20 
    /*00002914*/ MOV         R0, #3 
    /*00002918*/ BL          0x0000000000002918          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000291c*/ MOVW        R6, #32769                  @ 0x8001 
    /*00002920*/ MOVT        R6, #40982                  @ 0xa016 
    AOP_HAL_SetDevAttr_x2924:
    /*00002924*/ LDR         R0, [PC, #324]              @ 0x0000000000002a70 
    /*00002928*/ BL          0x0000000000002928          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000292c*/ MOV         R0, R6 
    /*00002930*/ ADD         SP, SP, #24 
    /*00002934*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    AOP_HAL_SetDevAttr_x2938:
    /*00002938*/ MOV         R1, #9 
    /*0000293c*/ BFI         R3, R1, #4, #4 
    /*00002940*/ B           AOP_HAL_SetDevAttr_x25cc
    AOP_HAL_SetDevAttr_x2944:
    /*00002944*/ BFC         R3, #20, #4 
    /*00002948*/ B           AOP_HAL_SetDevAttr_x2800
    AOP_HAL_SetDevAttr_x294c:
    /*0000294c*/ STR         R3, [SP, #4] 
    /*00002950*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x808] 
    /*00002954*/ MOVW        R3, #2045                   @ 0x7fd 
    /*00002958*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x808] 
    /*0000295c*/ STR         R3, [SP] 
    /*00002960*/ MOV         R1, #20 
    /*00002964*/ LDR         R3, [PC, #268]              @ 0x0000000000002a78 
    /*00002968*/ MOV         R0, #3 
    /*0000296c*/ MOVW        R6, #32771                  @ 0x8003 
    /*00002970*/ BL          0x0000000000002970          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002974*/ MOVT        R6, #40982                  @ 0xa016 
    /*00002978*/ B           AOP_HAL_SetDevAttr_x2924
    AOP_HAL_SetDevAttr_x297c:
    /*0000297c*/ STR         R3, [SP, #4] 
    /*00002980*/ MOVW        R3, #2151                   @ 0x867 
    AOP_HAL_SetDevAttr_x2984:
    /*00002984*/ MOVW        R2, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x628] 
    /*00002988*/ MOVT        R2, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x628] 
    /*0000298c*/ STR         R3, [SP] 
    AOP_HAL_SetDevAttr_x2990:
    /*00002990*/ LDR         R3, [PC, #220]              @ 0x0000000000002a74 
    /*00002994*/ MOV         R1, #20 
    /*00002998*/ MOV         R0, #3 
    /*0000299c*/ BL          0x000000000000299C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000029a0*/ B           AOP_HAL_SetDevAttr_x279c
    AOP_HAL_SetDevAttr_x29a4:
    /*000029a4*/ MOVW        R3, #2164                   @ 0x874 
    /*000029a8*/ MOVW        R2, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x69c] 
    /*000029ac*/ STR         R1, [SP, #4] 
    /*000029b0*/ MOVT        R2, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x69c] 
    /*000029b4*/ STR         R3, [SP] 
    /*000029b8*/ B           AOP_HAL_SetDevAttr_x2990
    AOP_HAL_SetDevAttr_x29bc:
    /*000029bc*/ MOV         R6, #65024                  @ 0xfe00 
    /*000029c0*/ MOVT        R6, #65535                  @ 0xffff 
    /*000029c4*/ B           AOP_HAL_SetDevAttr_x27a4
    AOP_HAL_SetDevAttr_x29c8:
    /*000029c8*/ MOVW        R6, #32771                  @ 0x8003 
    /*000029cc*/ MOVT        R6, #40982                  @ 0xa016 
    /*000029d0*/ B           AOP_HAL_SetDevAttr_x2924
    AOP_HAL_SetDevAttr_x29d4:
    /*000029d4*/ MOVW        R3, #2134                   @ 0x856 
    /*000029d8*/ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8d8] 
    /*000029dc*/ STR         R3, [SP] 
    /*000029e0*/ MOVT        R2, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8d8] 
    /*000029e4*/ B           AOP_HAL_SetDevAttr_x2748
    AOP_HAL_SetDevAttr_x29e8:
    /*000029e8*/ LDR         R0, [PC, #128]              @ 0x0000000000002a70 
    /*000029ec*/ MOVW        R6, #32776                  @ 0x8008 
    /*000029f0*/ BL          0x00000000000029F0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000029f4*/ MOVT        R6, #40982                  @ 0xa016 
    /*000029f8*/ B           AOP_HAL_SetDevAttr_x27a4
    AOP_HAL_SetDevAttr_x29fc:
    /*000029fc*/ MOVW        R3, #2207                   @ 0x89f 
    /*00002a00*/ MOVW        R2, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x76c] 
    /*00002a04*/ STR         R1, [SP, #4] 
    /*00002a08*/ MOVT        R2, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x76c] 
    /*00002a0c*/ STR         R3, [SP] 
    /*00002a10*/ B           AOP_HAL_SetDevAttr_x2990
    AOP_HAL_SetDevAttr_x2a14:
    /*00002a14*/ MOV         R3, #2224                   @ 0x8b0 
    /*00002a18*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9e8] 
    /*00002a1c*/ STR         R3, [SP] 
    /*00002a20*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9e8] 
    /*00002a24*/ LDR         R3, [PC, #72]               @ 0x0000000000002a74 
    /*00002a28*/ MOV         R1, #20 
    /*00002a2c*/ MOV         R0, #3 
    /*00002a30*/ MOVW        R6, #32776                  @ 0x8008 
    /*00002a34*/ BL          0x0000000000002A34          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002a38*/ MOVT        R6, #40982                  @ 0xa016 
    /*00002a3c*/ B           AOP_HAL_SetDevAttr_x27a4
    AOP_HAL_SetDevAttr_x2a40:
    /*00002a40*/ MOVW        R3, #2117                   @ 0x845 
    /*00002a44*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x83c] 
    /*00002a48*/ STR         R3, [SP] 
    /*00002a4c*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x83c] 
    /*00002a50*/ LDR         R3, [PC, #28]               @ 0x0000000000002a74 
    /*00002a54*/ MOV         R1, #22 
    /*00002a58*/ MOV         R0, #3 
    /*00002a5c*/ MOVW        R6, #32771                  @ 0x8003 
    /*00002a60*/ BL          0x0000000000002A60          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002a64*/ MOVT        R6, #40982                  @ 0xa016 
    /*00002a68*/ B           AOP_HAL_SetDevAttr_x27a4
    /*00002a6c*/ ANDEQ       R0, R0, IP, LSL #6 
    /*00002a70*/ ANDEQ       R0, R0, R4, ASR #1 
    /*00002a74*/ ANDEQ       R0, R0, R0, LSR #6 
    /*00002a78*/ ANDEQ       R0, R0, R0, LSR R3 
FUNC_END AOP_HAL_SetDevAttr


FUNC_BEGIN AIP_HAL_IntEn
    /*00002a7c*/ PUSH        {R4, R5, LR} 
    /*00002a80*/ SUBS        R5, R0, #0 
    /*00002a84*/ SUB         SP, SP, #12 
    /*00002a88*/ BNE         AIP_HAL_IntEn_x2ab0
    /*00002a8c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002a90*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002a94*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00002a98*/ LDR         R3, [R2] 
    /*00002a9c*/ BFI         R3, R1, #0, #1 
    /*00002aa0*/ MOV         R1, R3 
    /*00002aa4*/ STR         R1, [R2] 
    /*00002aa8*/ ADD         SP, SP, #12 
    /*00002aac*/ POP         {R4, R5, PC} 
    AIP_HAL_IntEn_x2ab0:
    /*00002ab0*/ MOV         R4, R1 
    /*00002ab4*/ STR         R5, [SP, #4] 
    /*00002ab8*/ MOVW        R3, #509                    @ 0x1fd 
    /*00002abc*/ MOV         R1, #20 
    /*00002ac0*/ STR         R3, [SP] 
    /*00002ac4*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00002ac8*/ LDR         R3, [PC, #80]               @ 0x0000000000002b20 
    /*00002acc*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00002ad0*/ MOV         R0, #3 
    /*00002ad4*/ BL          0x0000000000002AD4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002ad8*/ MOVW        R2, #510                    @ 0x1fe 
    /*00002adc*/ LDR         R1, [PC, #60]               @ 0x0000000000002b20 
    /*00002ae0*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00002ae4*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00002ae8*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00002aec*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00002af0*/ BL          0x0000000000002AF0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002af4*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002af8*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002afc*/ CMP         R5, #1 
    /*00002b00*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00002b04*/ LDR         R1, [R2] 
    /*00002b08*/ MOV         R3, R1 
    /*00002b0c*/ BFIEQ       R3, R4, #1, #1 
    /*00002b10*/ MOVEQ       R1, R3 
    /*00002b14*/ STR         R1, [R2] 
    /*00002b18*/ ADD         SP, SP, #12 
    /*00002b1c*/ POP         {R4, R5, PC} 
    /*00002b20*/ ANDEQ       R0, R0, R0, ASR #6 
FUNC_END AIP_HAL_IntEn


FUNC_BEGIN AIP_HAL_SetChildIntMask
    /*00002b24*/ PUSH        {R4, LR} 
    /*00002b28*/ SUBS        R4, R0, #0 
    /*00002b2c*/ SUB         SP, SP, #8 
    /*00002b30*/ BEQ         AIP_HAL_SetChildIntMask_x2b74
    /*00002b34*/ MOVW        R3, #663                    @ 0x297 
    /*00002b38*/ MOV         R1, #20 
    /*00002b3c*/ STR         R3, [SP] 
    /*00002b40*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00002b44*/ LDR         R3, [PC, #100]              @ 0x0000000000002bb0 
    /*00002b48*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00002b4c*/ STR         R4, [SP, #4] 
    /*00002b50*/ MOV         R0, #3 
    /*00002b54*/ BL          0x0000000000002B54          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002b58*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00002b5c*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00002b60*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00002b64*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00002b68*/ MOV         R2, #664                    @ 0x298 
    /*00002b6c*/ LDR         R1, [PC, #60]               @ 0x0000000000002bb0 
    /*00002b70*/ BL          0x0000000000002B70          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_SetChildIntMask_x2b74:
    /*00002b74*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002b78*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002b7c*/ MOVW        R0, #4256                   @ 0x10a0 
    /*00002b80*/ ADD         R0, R0, R4, LSL #8 
    /*00002b84*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00002b88*/ LDR         R3, [R2, R0] 
    /*00002b8c*/ ORR         R3, R3, #3 
    /*00002b90*/ BFC         R3, #2, #1 
    /*00002b94*/ ORR         R3, R3, #8 
    /*00002b98*/ BFC         R3, #4, #1 
    /*00002b9c*/ BFC         R3, #5, #1 
    /*00002ba0*/ BFC         R3, #6, #26 
    /*00002ba4*/ STR         R3, [R2, R0] 
    /*00002ba8*/ ADD         SP, SP, #8 
    /*00002bac*/ POP         {R4, PC} 
    /*00002bb0*/ ANDEQ       R0, R0, R0, ASR R3 
FUNC_END AIP_HAL_SetChildIntMask


FUNC_BEGIN AIP_HAL_GetChildIntAllStatus
    /*00002bb4*/ PUSH        {R4, LR} 
    /*00002bb8*/ SUBS        R4, R0, #0 
    /*00002bbc*/ SUB         SP, SP, #8 
    /*00002bc0*/ BEQ         AIP_HAL_GetChildIntAllStatus_x2c04
    /*00002bc4*/ MOVW        R3, #683                    @ 0x2ab 
    /*00002bc8*/ MOV         R1, #20 
    /*00002bcc*/ STR         R3, [SP] 
    /*00002bd0*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00002bd4*/ LDR         R3, [PC, #72]               @ 0x0000000000002c24 
    /*00002bd8*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00002bdc*/ STR         R4, [SP, #4] 
    /*00002be0*/ MOV         R0, #3 
    /*00002be4*/ BL          0x0000000000002BE4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002be8*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00002bec*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00002bf0*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00002bf4*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00002bf8*/ MOV         R2, #684                    @ 0x2ac 
    /*00002bfc*/ LDR         R1, [PC, #32]               @ 0x0000000000002c24 
    /*00002c00*/ BL          0x0000000000002C00          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_GetChildIntAllStatus_x2c04:
    /*00002c04*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002c08*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002c0c*/ MOVW        R0, #4264                   @ 0x10a8 
    /*00002c10*/ ADD         R0, R0, R4, LSL #8 
    /*00002c14*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00002c18*/ LDR         R0, [R3, R0] 
    /*00002c1c*/ ADD         SP, SP, #8 
    /*00002c20*/ POP         {R4, PC} 
    /*00002c24*/ ANDEQ       R0, R0, R8, ROR #6 
FUNC_END AIP_HAL_GetChildIntAllStatus


FUNC_BEGIN AIP_HAL_GetIntStatus
    /*00002c28*/ PUSH        {R4, R5, R6, LR} 
    /*00002c2c*/ MOV         R4, R1 
    /*00002c30*/ MOV         R5, R0 
    /*00002c34*/ BL          0x0000000000002C34          @ AIP_HAL_GetChildIntAllStatus [R_ARM_CALL: .text+0x2bb4] 
    /*00002c38*/ MOV         R2, R0 
    /*00002c3c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002c40*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002c44*/ STR         R2, [R4] 
    /*00002c48*/ ADD         R3, R0, R5, LSL #2 
    /*00002c4c*/ MOV         R0, #0 
    /*00002c50*/ STR         R2, [R3, #212]              @ 0xd4 
    /*00002c54*/ POP         {R4, R5, R6, PC} 
FUNC_END AIP_HAL_GetIntStatus


FUNC_BEGIN AIP_HAL_ClrChildIntAllStatus
    /*00002c58*/ PUSH        {R4, LR} 
    /*00002c5c*/ SUBS        R4, R0, #0 
    /*00002c60*/ SUB         SP, SP, #8 
    /*00002c64*/ BEQ         AIP_HAL_ClrChildIntAllStatus_x2ca8
    /*00002c68*/ MOV         R3, #696                    @ 0x2b8 
    /*00002c6c*/ MOV         R1, #20 
    /*00002c70*/ STR         R3, [SP] 
    /*00002c74*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00002c78*/ LDR         R3, [PC, #76]               @ 0x0000000000002ccc 
    /*00002c7c*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00002c80*/ STR         R4, [SP, #4] 
    /*00002c84*/ MOV         R0, #3 
    /*00002c88*/ BL          0x0000000000002C88          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002c8c*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00002c90*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00002c94*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00002c98*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00002c9c*/ MOVW        R2, #697                    @ 0x2b9 
    /*00002ca0*/ LDR         R1, [PC, #36]               @ 0x0000000000002ccc 
    /*00002ca4*/ BL          0x0000000000002CA4          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_ClrChildIntAllStatus_x2ca8:
    /*00002ca8*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002cac*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002cb0*/ MOVW        R0, #4268                   @ 0x10ac 
    /*00002cb4*/ ADD         R0, R0, R4, LSL #8 
    /*00002cb8*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00002cbc*/ MOV         R2, #63                     @ 0x3f 
    /*00002cc0*/ STR         R2, [R3, R0] 
    /*00002cc4*/ ADD         SP, SP, #8 
    /*00002cc8*/ POP         {R4, PC} 
    /*00002ccc*/ ANDEQ       R0, R0, R8, LSL #7 
FUNC_END AIP_HAL_ClrChildIntAllStatus


FUNC_BEGIN AIP_HAL_SetChildIntClr
    /*00002cd0*/ PUSH        {R4, LR} 
    /*00002cd4*/ SUBS        R4, R0, #0 
    /*00002cd8*/ SUB         SP, SP, #8 
    /*00002cdc*/ BNE         AIP_HAL_SetChildIntClr_x2d3c
    AIP_HAL_SetChildIntClr_x2ce0:
    /*00002ce0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002ce4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002ce8*/ LSL         R2, R4, #8 
    /*00002cec*/ ADD         R4, R3, R4, LSL #2 
    /*00002cf0*/ LDR         R0, [R3, #192]              @ 0xc0 
    /*00002cf4*/ ADD         R3, R2, #4224               @ 0x1080 
    /*00002cf8*/ ADD         R3, R3, #40                 @ 0x28 
    /*00002cfc*/ LDR         R1, [R4, #212]              @ 0xd4 
    /*00002d00*/ ADD         R2, R2, #4224               @ 0x1080 
    /*00002d04*/ LDR         R3, [R0, R3] 
    /*00002d08*/ TST         R1, #8 
    /*00002d0c*/ ADD         R2, R2, #44                 @ 0x2c 
    /*00002d10*/ BFC         R3, #3, #1 
    /*00002d14*/ BFC         R3, #1, #1 
    /*00002d18*/ BFC         R3, #0, #1 
    /*00002d1c*/ ORRNE       R3, R3, #8 
    /*00002d20*/ TST         R1, #2 
    /*00002d24*/ ORRNE       R3, R3, #2 
    /*00002d28*/ TST         R1, #1 
    /*00002d2c*/ ORRNE       R3, R3, #1 
    /*00002d30*/ STR         R3, [R0, R2] 
    /*00002d34*/ ADD         SP, SP, #8 
    /*00002d38*/ POP         {R4, PC} 
    AIP_HAL_SetChildIntClr_x2d3c:
    /*00002d3c*/ STR         R4, [SP, #4] 
    /*00002d40*/ MOVW        R3, #719                    @ 0x2cf 
    /*00002d44*/ MOV         R1, #20 
    /*00002d48*/ STR         R3, [SP] 
    /*00002d4c*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00002d50*/ LDR         R3, [PC, #40]               @ 0x0000000000002d80 
    /*00002d54*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00002d58*/ MOV         R0, #3 
    /*00002d5c*/ BL          0x0000000000002D5C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002d60*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00002d64*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00002d68*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00002d6c*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00002d70*/ MOV         R2, #720                    @ 0x2d0 
    /*00002d74*/ LDR         R1, [PC, #4]                @ 0x0000000000002d80 
    /*00002d78*/ BL          0x0000000000002D78          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002d7c*/ B           AIP_HAL_SetChildIntClr_x2ce0
    /*00002d80*/ ANDEQ       R0, R0, R8, LSR #7 
FUNC_END AIP_HAL_SetChildIntClr


FUNC_BEGIN AIP_HAL_ClrIntStatus
    /*00002d84*/ PUSH        {R4, LR} 
    /*00002d88*/ BL          0x0000000000002D88          @ AIP_HAL_SetChildIntClr [R_ARM_CALL: .text+0x2cd0] 
    /*00002d8c*/ MOV         R0, #0 
    /*00002d90*/ POP         {R4, PC} 
FUNC_END AIP_HAL_ClrIntStatus


FUNC_BEGIN AIP_HAL_SetRxStart
    /*00002d94*/ PUSH        {R4, R5, LR} 
    /*00002d98*/ SUBS        R4, R0, #0 
    /*00002d9c*/ SUB         SP, SP, #12 
    /*00002da0*/ MOV         R5, R1 
    /*00002da4*/ BEQ         AIP_HAL_SetRxStart_x2de8
    /*00002da8*/ MOVW        R3, #759                    @ 0x2f7 
    /*00002dac*/ MOV         R1, #20 
    /*00002db0*/ STR         R3, [SP] 
    /*00002db4*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00002db8*/ LDR         R3, [PC, #80]               @ 0x0000000000002e10 
    /*00002dbc*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00002dc0*/ STR         R4, [SP, #4] 
    /*00002dc4*/ MOV         R0, #3 
    /*00002dc8*/ BL          0x0000000000002DC8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002dcc*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00002dd0*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00002dd4*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00002dd8*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00002ddc*/ MOV         R2, #760                    @ 0x2f8 
    /*00002de0*/ LDR         R1, [PC, #40]               @ 0x0000000000002e10 
    /*00002de4*/ BL          0x0000000000002DE4          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_SetRxStart_x2de8:
    /*00002de8*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002dec*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002df0*/ MOVW        R0, #4100                   @ 0x1004 
    /*00002df4*/ ADD         R0, R0, R4, LSL #8 
    /*00002df8*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00002dfc*/ LDR         R3, [R2, R0] 
    /*00002e00*/ BFI         R3, R5, #28, #1 
    /*00002e04*/ STR         R3, [R2, R0] 
    /*00002e08*/ ADD         SP, SP, #12 
    /*00002e0c*/ POP         {R4, R5, PC} 
    /*00002e10*/ ANDEQ       R0, R0, R0, ASR #7 
FUNC_END AIP_HAL_SetRxStart


FUNC_BEGIN AIP_HAL_DevEnable
    /*00002e14*/ PUSH        {R4, R5, R6, LR} 
    /*00002e18*/ MOV         R4, R0 
    /*00002e1c*/ BL          0x0000000000002E1C          @ AIP_HAL_SetChildIntMask [R_ARM_CALL: .text+0x2b24] 
    /*00002e20*/ MOV         R0, R4 
    /*00002e24*/ MOV         R1, #1 
    /*00002e28*/ BL          0x0000000000002E28          @ AIP_HAL_IntEn [R_ARM_CALL: .text+0x2a7c] 
    /*00002e2c*/ LSL         R0, R4, #2 
    /*00002e30*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002e34*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002e38*/ ADD         R2, R3, R0 
    /*00002e3c*/ LSL         R1, R4, #3 
    /*00002e40*/ LDR         LR, [R3, #192]              @ 0xc0 
    /*00002e44*/ ADD         IP, R1, #260                @ 0x104 
    /*00002e48*/ LDR         R5, [R2, #220]              @ 0xdc 
    /*00002e4c*/ LDR         R2, [LR, IP] 
    /*00002e50*/ CMP         R5, #0 
    /*00002e54*/ ADDEQ       R1, R1, #324                @ 0x144 
    /*00002e58*/ ORR         R2, R2, #49408              @ 0xc100 
    /*00002e5c*/ STR         R2, [LR, IP] 
    /*00002e60*/ LDREQ       IP, [R3, #192]              @ 0xc0 
    /*00002e64*/ ADD         R3, R3, R0 
    /*00002e68*/ MOV         R0, R4 
    /*00002e6c*/ LDREQ       R2, [IP, R1] 
    /*00002e70*/ ORREQ       R2, R2, #49408              @ 0xc100 
    /*00002e74*/ STREQ       R2, [IP, R1] 
    /*00002e78*/ MOV         R2, #1 
    /*00002e7c*/ MOV         R1, R2 
    /*00002e80*/ STR         R2, [R3, #228]              @ 0xe4 
    /*00002e84*/ BL          0x0000000000002E84          @ AIP_HAL_SetRxStart [R_ARM_CALL: .text+0x2d94] 
    /*00002e88*/ MOV         R0, #0 
    /*00002e8c*/ POP         {R4, R5, R6, PC} 
FUNC_END AIP_HAL_DevEnable


FUNC_BEGIN AIP_HAL_GetDisDone
    /*00002e90*/ PUSH        {R4, LR} 
    /*00002e94*/ SUBS        R4, R0, #0 
    /*00002e98*/ SUB         SP, SP, #8 
    /*00002e9c*/ BEQ         AIP_HAL_GetDisDone_x2ee0
    /*00002ea0*/ MOVW        R3, #778                    @ 0x30a 
    /*00002ea4*/ MOV         R1, #20 
    /*00002ea8*/ STR         R3, [SP] 
    /*00002eac*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfc] 
    /*00002eb0*/ LDR         R3, [PC, #76]               @ 0x0000000000002f04 
    /*00002eb4*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfc] 
    /*00002eb8*/ STR         R4, [SP, #4] 
    /*00002ebc*/ MOV         R0, #3 
    /*00002ec0*/ BL          0x0000000000002EC0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002ec4*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00002ec8*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00002ecc*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00002ed0*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00002ed4*/ MOVW        R2, #779                    @ 0x30b 
    /*00002ed8*/ LDR         R1, [PC, #36]               @ 0x0000000000002f04 
    /*00002edc*/ BL          0x0000000000002EDC          @ osal_panic [R_ARM_CALL: +0x0] 
    AIP_HAL_GetDisDone_x2ee0:
    /*00002ee0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002ee4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002ee8*/ MOVW        R0, #4100                   @ 0x1004 
    /*00002eec*/ ADD         R0, R0, R4, LSL #8 
    /*00002ef0*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00002ef4*/ LDR         R0, [R3, R0] 
    /*00002ef8*/ UBFX        R0, R0, #29, #1 
    /*00002efc*/ ADD         SP, SP, #8 
    /*00002f00*/ POP         {R4, PC} 
    /*00002f04*/ LDRDEQ      R0, [R0], -R4 
FUNC_END AIP_HAL_GetDisDone


FUNC_BEGIN AIP_HAL_DevDisable
    /*00002f08*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00002f0c*/ MOV         R1, #0 
    /*00002f10*/ MOV         R5, R0 
    /*00002f14*/ SUB         SP, SP, #12 
    /*00002f18*/ BL          0x0000000000002F18          @ AIP_HAL_SetRxStart [R_ARM_CALL: .text+0x2d94] 
    /*00002f1c*/ MOV         R0, R5 
    /*00002f20*/ MOV         R1, #0 
    /*00002f24*/ MOV         R4, #102                    @ 0x66 
    /*00002f28*/ BL          0x0000000000002F28          @ AIP_HAL_IntEn [R_ARM_CALL: .text+0x2a7c] 
    /*00002f2c*/ MOV         R0, R5 
    /*00002f30*/ BL          0x0000000000002F30          @ AIP_HAL_ClrChildIntAllStatus [R_ARM_CALL: .text+0x2c58] 
    /*00002f34*/ B           AIP_HAL_DevDisable_x2f44
    AIP_HAL_DevDisable_x2f38:
    /*00002f38*/ SUBS        R4, R4, #1 
    /*00002f3c*/ BEQ         AIP_HAL_DevDisable_x2fc4
    /*00002f40*/ BL          0x0000000000002F40          @ osal_msleep [R_ARM_CALL: +0x0] 
    AIP_HAL_DevDisable_x2f44:
    /*00002f44*/ MOV         R0, R5 
    /*00002f48*/ BL          0x0000000000002F48          @ AIP_HAL_GetDisDone [R_ARM_CALL: .text+0x2e90] 
    /*00002f4c*/ CMP         R0, #0 
    /*00002f50*/ MOV         R0, #10 
    /*00002f54*/ BEQ         AIP_HAL_DevDisable_x2f38
    /*00002f58*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002f5c*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    AIP_HAL_DevDisable_x2f60:
    /*00002f60*/ LSL         R1, R5, #2 
    /*00002f64*/ ADD         R3, R6, R1 
    /*00002f68*/ LDR         R2, [R3, #220]              @ 0xdc 
    /*00002f6c*/ CMP         R2, #0 
    /*00002f70*/ BNE         AIP_HAL_DevDisable_x2fb0
    /*00002f74*/ LDR         IP, [R6, #192]              @ 0xc0 
    /*00002f78*/ LSL         R5, R5, #3 
    /*00002f7c*/ ADD         R0, R5, #260                @ 0x104 
    /*00002f80*/ ADD         R5, R5, #324                @ 0x144 
    /*00002f84*/ LDR         R3, [IP, R0] 
    /*00002f88*/ BFI         R3, R2, #14, #1 
    /*00002f8c*/ BFI         R3, R2, #15, #1 
    /*00002f90*/ ORR         R3, R3, #512                @ 0x200 
    /*00002f94*/ STR         R3, [IP, R0] 
    /*00002f98*/ LDR         R0, [R6, #192]              @ 0xc0 
    /*00002f9c*/ LDR         R3, [R0, R5] 
    /*00002fa0*/ BFI         R3, R2, #14, #1 
    /*00002fa4*/ BFI         R3, R2, #15, #1 
    /*00002fa8*/ ORR         R3, R3, #512                @ 0x200 
    /*00002fac*/ STR         R3, [R0, R5] 
    AIP_HAL_DevDisable_x2fb0:
    /*00002fb0*/ ADD         R6, R6, R1 
    /*00002fb4*/ MOV         R3, #0 
    /*00002fb8*/ STR         R3, [R6, #228]              @ 0xe4 
    /*00002fbc*/ ADD         SP, SP, #12 
    /*00002fc0*/ POP         {R4, R5, R6, R7, PC} 
    AIP_HAL_DevDisable_x2fc4:
    /*00002fc4*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002fc8*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002fcc*/ LSL         R7, R5, #3 
    /*00002fd0*/ MOV         R0, #10 
    /*00002fd4*/ LDR         R2, [R6, #192]              @ 0xc0 
    /*00002fd8*/ ADD         R7, R7, #260                @ 0x104 
    /*00002fdc*/ LDR         R3, [R2, R7] 
    /*00002fe0*/ ORR         R3, R3, #512                @ 0x200 
    /*00002fe4*/ BFI         R3, R4, #8, #1 
    /*00002fe8*/ STR         R3, [R2, R7] 
    /*00002fec*/ BL          0x0000000000002FEC          @ osal_msleep [R_ARM_CALL: +0x0] 
    /*00002ff0*/ LDR         R0, [R6, #192]              @ 0xc0 
    /*00002ff4*/ MOVW        IP, #1444                   @ 0x5a4 
    /*00002ff8*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa68] 
    /*00002ffc*/ MOV         R1, #20 
    /*00003000*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa68] 
    /*00003004*/ LDR         R3, [R0, R7] 
    /*00003008*/ ORR         R3, R3, #256                @ 0x100 
    /*0000300c*/ BFI         R3, R4, #9, #1 
    /*00003010*/ STR         R3, [R0, R7] 
    /*00003014*/ LDR         R3, [PC, #12]               @ 0x0000000000003028 
    /*00003018*/ MOV         R0, #3 
    /*0000301c*/ STR         IP, [SP] 
    /*00003020*/ BL          0x0000000000003020          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003024*/ B           AIP_HAL_DevDisable_x2f60
    /*00003028*/ ANDEQ       R0, R0, R8, ROR #7 
FUNC_END AIP_HAL_DevDisable


FUNC_BEGIN AOP_HAL_IntEn
    /*0000302c*/ CMP         R0, #1 
    /*00003030*/ PUSH        {R4, R5, LR} 
    /*00003034*/ MOV         R4, R0 
    /*00003038*/ SUB         SP, SP, #12 
    /*0000303c*/ MOV         R5, R1 
    /*00003040*/ BLS         AOP_HAL_IntEn_x3084
    /*00003044*/ STR         R0, [SP, #4] 
    /*00003048*/ MOVW        R3, #1477                   @ 0x5c5 
    /*0000304c*/ MOV         R1, #20 
    /*00003050*/ STR         R3, [SP] 
    /*00003054*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00003058*/ LDR         R3, [PC, #132]              @ 0x00000000000030e4 
    /*0000305c*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00003060*/ MOV         R0, #3 
    /*00003064*/ BL          0x0000000000003064          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003068*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*0000306c*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00003070*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00003074*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00003078*/ MOVW        R2, #1478                   @ 0x5c6 
    /*0000307c*/ LDR         R1, [PC, #96]               @ 0x00000000000030e4 
    /*00003080*/ BL          0x0000000000003080          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_IntEn_x3084:
    /*00003084*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003088*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000308c*/ CMP         R4, #1 
    /*00003090*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00003094*/ LDR         R3, [R2] 
    /*00003098*/ MOV         R1, R3 
    /*0000309c*/ BEQ         AOP_HAL_IntEn_x30bc
    /*000030a0*/ BCC         AOP_HAL_IntEn_x30d0
    /*000030a4*/ CMP         R4, #2 
    /*000030a8*/ BFIEQ       R1, R5, #18, #1 
    /*000030ac*/ MOVEQ       R3, R1 
    /*000030b0*/ STR         R3, [R2] 
    /*000030b4*/ ADD         SP, SP, #12 
    /*000030b8*/ POP         {R4, R5, PC} 
    AOP_HAL_IntEn_x30bc:
    /*000030bc*/ BFI         R1, R5, #17, #1 
    /*000030c0*/ MOV         R3, R1 
    /*000030c4*/ STR         R3, [R2] 
    /*000030c8*/ ADD         SP, SP, #12 
    /*000030cc*/ POP         {R4, R5, PC} 
    AOP_HAL_IntEn_x30d0:
    /*000030d0*/ BFI         R1, R5, #16, #1 
    /*000030d4*/ MOV         R3, R1 
    /*000030d8*/ STR         R3, [R2] 
    /*000030dc*/ ADD         SP, SP, #12 
    /*000030e0*/ POP         {R4, R5, PC} 
    /*000030e4*/ STRDEQ      R0, [R0], -IP 
FUNC_END AOP_HAL_IntEn


FUNC_BEGIN AOP_HAL_SetTxStart
    /*000030e8*/ CMP         R0, #1 
    /*000030ec*/ PUSH        {R4, R5, LR} 
    /*000030f0*/ MOV         R4, R0 
    /*000030f4*/ SUB         SP, SP, #12 
    /*000030f8*/ MOV         R5, R1 
    /*000030fc*/ BLS         AOP_HAL_SetTxStart_x3140
    /*00003100*/ STR         R0, [SP, #4] 
    /*00003104*/ MOV         R3, #1632                   @ 0x660 
    /*00003108*/ MOV         R1, #20 
    /*0000310c*/ STR         R3, [SP] 
    /*00003110*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00003114*/ LDR         R3, [PC, #76]               @ 0x0000000000003168 
    /*00003118*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*0000311c*/ MOV         R0, #3 
    /*00003120*/ BL          0x0000000000003120          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003124*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00003128*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*0000312c*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00003130*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00003134*/ MOVW        R2, #1633                   @ 0x661 
    /*00003138*/ LDR         R1, [PC, #40]               @ 0x0000000000003168 
    /*0000313c*/ BL          0x000000000000313C          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_SetTxStart_x3140:
    /*00003140*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003144*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003148*/ MOVW        R0, #8196                   @ 0x2004 
    /*0000314c*/ ADD         R0, R0, R4, LSL #8 
    /*00003150*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00003154*/ LDR         R3, [R2, R0] 
    /*00003158*/ BFI         R3, R5, #28, #1 
    /*0000315c*/ STR         R3, [R2, R0] 
    /*00003160*/ ADD         SP, SP, #12 
    /*00003164*/ POP         {R4, R5, PC} 
    /*00003168*/ ANDEQ       R0, R0, IP, LSL #8 
FUNC_END AOP_HAL_SetTxStart


FUNC_BEGIN AOP_HAL_GetDisDone
    /*0000316c*/ CMP         R0, #1 
    /*00003170*/ PUSH        {R4, LR} 
    /*00003174*/ MOV         R4, R0 
    /*00003178*/ SUB         SP, SP, #8 
    /*0000317c*/ BLS         AOP_HAL_GetDisDone_x31c0
    /*00003180*/ STR         R0, [SP, #4] 
    /*00003184*/ MOVW        R3, #1651                   @ 0x673 
    /*00003188*/ MOV         R1, #20 
    /*0000318c*/ STR         R3, [SP] 
    /*00003190*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00003194*/ LDR         R3, [PC, #72]               @ 0x00000000000031e4 
    /*00003198*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*0000319c*/ MOV         R0, #3 
    /*000031a0*/ BL          0x00000000000031A0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000031a4*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*000031a8*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*000031ac*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*000031b0*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*000031b4*/ MOVW        R2, #1652                   @ 0x674 
    /*000031b8*/ LDR         R1, [PC, #36]               @ 0x00000000000031e4 
    /*000031bc*/ BL          0x00000000000031BC          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_GetDisDone_x31c0:
    /*000031c0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000031c4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000031c8*/ MOVW        R0, #8196                   @ 0x2004 
    /*000031cc*/ ADD         R0, R0, R4, LSL #8 
    /*000031d0*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*000031d4*/ LDR         R0, [R3, R0] 
    /*000031d8*/ UBFX        R0, R0, #29, #1 
    /*000031dc*/ ADD         SP, SP, #8 
    /*000031e0*/ POP         {R4, PC} 
    /*000031e4*/ ANDEQ       R0, R0, R0, LSR #8 
FUNC_END AOP_HAL_GetDisDone


FUNC_BEGIN AOP_HAL_SetChildIntMask
    /*000031e8*/ CMP         R0, #1 
    /*000031ec*/ PUSH        {R4, LR} 
    /*000031f0*/ MOV         R4, R0 
    /*000031f4*/ SUB         SP, SP, #8 
    /*000031f8*/ BLS         AOP_HAL_SetChildIntMask_x323c
    /*000031fc*/ STR         R0, [SP, #4] 
    /*00003200*/ MOVW        R3, #1668                   @ 0x684 
    /*00003204*/ MOV         R1, #20 
    /*00003208*/ STR         R3, [SP] 
    /*0000320c*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00003210*/ LDR         R3, [PC, #100]              @ 0x000000000000327c 
    /*00003214*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00003218*/ MOV         R0, #3 
    /*0000321c*/ BL          0x000000000000321C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003220*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00003224*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00003228*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*0000322c*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00003230*/ MOVW        R2, #1669                   @ 0x685 
    /*00003234*/ LDR         R1, [PC, #64]               @ 0x000000000000327c 
    /*00003238*/ BL          0x0000000000003238          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_SetChildIntMask_x323c:
    /*0000323c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003240*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003244*/ MOVW        R0, #8352                   @ 0x20a0 
    /*00003248*/ ADD         R0, R0, R4, LSL #8 
    /*0000324c*/ LDR         R2, [R3, #192]              @ 0xc0 
    /*00003250*/ LDR         R3, [R2, R0] 
    /*00003254*/ ORR         R3, R3, #3 
    /*00003258*/ BFC         R3, #2, #1 
    /*0000325c*/ ORR         R3, R3, #24 
    /*00003260*/ BFC         R3, #5, #1 
    /*00003264*/ BFC         R3, #6, #1 
    /*00003268*/ BFC         R3, #7, #1 
    /*0000326c*/ BFC         R3, #8, #24 
    /*00003270*/ STR         R3, [R2, R0] 
    /*00003274*/ ADD         SP, SP, #8 
    /*00003278*/ POP         {R4, PC} 
    /*0000327c*/ ANDEQ       R0, R0, R4, LSR R4 
FUNC_END AOP_HAL_SetChildIntMask


FUNC_BEGIN AOP_HAL_DevEnable
    /*00003280*/ PUSH        {R4, R5, R6, LR} 
    /*00003284*/ MOV         R5, R0 
    /*00003288*/ BL          0x0000000000003288          @ AOP_HAL_SetChildIntMask [R_ARM_CALL: .text+0x31e8] 
    /*0000328c*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003290*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003294*/ MOV         R0, R5 
    /*00003298*/ MOV         R1, #1 
    /*0000329c*/ BL          0x000000000000329C          @ AOP_HAL_IntEn [R_ARM_CALL: .text+0x302c] 
    /*000032a0*/ LDR         IP, [R4, #192]              @ 0xc0 
    /*000032a4*/ LSL         R3, R5, #3 
    /*000032a8*/ MOV         R0, R5 
    /*000032ac*/ ADD         R3, R3, #324                @ 0x144 
    /*000032b0*/ MOV         R1, #1 
    /*000032b4*/ ADD         R5, R4, R5, LSL #2 
    /*000032b8*/ LDR         R2, [IP, R3] 
    /*000032bc*/ ORR         R2, R2, #49408              @ 0xc100 
    /*000032c0*/ STR         R2, [IP, R3] 
    /*000032c4*/ BL          0x00000000000032C4          @ AOP_HAL_SetTxStart [R_ARM_CALL: .text+0x30e8] 
    /*000032c8*/ LDR         R2, [R4, #208]              @ 0xd0 
    /*000032cc*/ MOV         R1, #1 
    /*000032d0*/ MOV         R0, #0 
    /*000032d4*/ STR         R1, [R5, #220]              @ 0xdc 
    /*000032d8*/ LDR         R3, [R2] 
    /*000032dc*/ BIC         R3, R3, #16384              @ 0x4000 
    /*000032e0*/ STR         R3, [R2] 
    /*000032e4*/ LDR         R2, [R4, #208]              @ 0xd0 
    /*000032e8*/ LDR         R3, [R2] 
    /*000032ec*/ BIC         R3, R3, #8192               @ 0x2000 
    /*000032f0*/ STR         R3, [R2] 
    /*000032f4*/ POP         {R4, R5, R6, PC} 
FUNC_END AOP_HAL_DevEnable


FUNC_BEGIN AOP_HAL_SetChildIntClr
    /*000032f8*/ CMP         R0, #1 
    /*000032fc*/ PUSH        {R4, LR} 
    /*00003300*/ MOV         R4, R0 
    /*00003304*/ SUB         SP, SP, #8 
    /*00003308*/ BLS         AOP_HAL_SetChildIntClr_x334c
    /*0000330c*/ STR         R0, [SP, #4] 
    /*00003310*/ MOVW        R3, #1693                   @ 0x69d 
    /*00003314*/ MOV         R1, #20 
    /*00003318*/ STR         R3, [SP] 
    /*0000331c*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00003320*/ LDR         R3, [PC, #132]              @ 0x00000000000033ac 
    /*00003324*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*00003328*/ MOV         R0, #3 
    /*0000332c*/ BL          0x000000000000332C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003330*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00003334*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00003338*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*0000333c*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00003340*/ MOVW        R2, #1694                   @ 0x69e 
    /*00003344*/ LDR         R1, [PC, #96]               @ 0x00000000000033ac 
    /*00003348*/ BL          0x0000000000003348          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_SetChildIntClr_x334c:
    /*0000334c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003350*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003354*/ LSL         R2, R4, #8 
    /*00003358*/ ADD         R4, R3, R4, LSL #2 
    /*0000335c*/ LDR         R0, [R3, #192]              @ 0xc0 
    /*00003360*/ ADD         R3, R2, #8320               @ 0x2080 
    /*00003364*/ ADD         R3, R3, #40                 @ 0x28 
    /*00003368*/ LDR         R1, [R4, #232]              @ 0xe8 
    /*0000336c*/ ADD         R2, R2, #8320               @ 0x2080 
    /*00003370*/ LDR         R3, [R0, R3] 
    /*00003374*/ TST         R1, #24 
    /*00003378*/ ADD         R2, R2, #44                 @ 0x2c 
    /*0000337c*/ BFC         R3, #3, #1 
    /*00003380*/ BFC         R3, #4, #1 
    /*00003384*/ BFC         R3, #1, #1 
    /*00003388*/ BFC         R3, #0, #1 
    /*0000338c*/ ORRNE       R3, R3, #24 
    /*00003390*/ TST         R1, #2 
    /*00003394*/ ORRNE       R3, R3, #2 
    /*00003398*/ TST         R1, #1 
    /*0000339c*/ ORRNE       R3, R3, #1 
    /*000033a0*/ STR         R3, [R0, R2] 
    /*000033a4*/ ADD         SP, SP, #8 
    /*000033a8*/ POP         {R4, PC} 
    /*000033ac*/ ANDEQ       R0, R0, IP, ASR #8 
FUNC_END AOP_HAL_SetChildIntClr


FUNC_BEGIN AOP_HAL_ClrIntStatus
    /*000033b0*/ CMP         R0, #1 
    /*000033b4*/ PUSH        {R4, LR} 
    /*000033b8*/ MOV         R4, R0 
    /*000033bc*/ SUB         SP, SP, #8 
    /*000033c0*/ BLS         AOP_HAL_ClrIntStatus_x3404
    /*000033c4*/ STR         R0, [SP, #4] 
    /*000033c8*/ MOVW        R3, #1764                   @ 0x6e4 
    /*000033cc*/ MOV         R1, #20 
    /*000033d0*/ STR         R3, [SP] 
    /*000033d4*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*000033d8*/ LDR         R3, [PC, #56]               @ 0x0000000000003418 
    /*000033dc*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*000033e0*/ MOV         R0, #3 
    /*000033e4*/ BL          0x00000000000033E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000033e8*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*000033ec*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*000033f0*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*000033f4*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*000033f8*/ MOVW        R2, #1765                   @ 0x6e5 
    /*000033fc*/ LDR         R1, [PC, #20]               @ 0x0000000000003418 
    /*00003400*/ BL          0x0000000000003400          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_ClrIntStatus_x3404:
    /*00003404*/ MOV         R0, R4 
    /*00003408*/ BL          0x0000000000003408          @ AOP_HAL_SetChildIntClr [R_ARM_CALL: .text+0x32f8] 
    /*0000340c*/ MOV         R0, #0 
    /*00003410*/ ADD         SP, SP, #8 
    /*00003414*/ POP         {R4, PC} 
    /*00003418*/ ANDEQ       R0, R0, R4, ROR #8 
FUNC_END AOP_HAL_ClrIntStatus


FUNC_BEGIN AOP_HAL_GetChildIntAllStatus
    /*0000341c*/ CMP         R0, #1 
    /*00003420*/ PUSH        {R4, LR} 
    /*00003424*/ MOV         R4, R0 
    /*00003428*/ SUB         SP, SP, #8 
    /*0000342c*/ BLS         AOP_HAL_GetChildIntAllStatus_x3470
    /*00003430*/ STR         R0, [SP, #4] 
    /*00003434*/ MOVW        R3, #1726                   @ 0x6be 
    /*00003438*/ MOV         R1, #20 
    /*0000343c*/ STR         R3, [SP] 
    /*00003440*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*00003444*/ LDR         R3, [PC, #68]               @ 0x0000000000003490 
    /*00003448*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*0000344c*/ MOV         R0, #3 
    /*00003450*/ BL          0x0000000000003450          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003454*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00003458*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*0000345c*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00003460*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*00003464*/ MOVW        R2, #1727                   @ 0x6bf 
    /*00003468*/ LDR         R1, [PC, #32]               @ 0x0000000000003490 
    /*0000346c*/ BL          0x000000000000346C          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_GetChildIntAllStatus_x3470:
    /*00003470*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003474*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003478*/ MOVW        R0, #8360                   @ 0x20a8 
    /*0000347c*/ ADD         R0, R0, R4, LSL #8 
    /*00003480*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*00003484*/ LDR         R0, [R3, R0] 
    /*00003488*/ ADD         SP, SP, #8 
    /*0000348c*/ POP         {R4, PC} 
    /*00003490*/ ANDEQ       R0, R0, IP, ROR R4 
FUNC_END AOP_HAL_GetChildIntAllStatus


FUNC_BEGIN AOP_HAL_GetIntStatus
    /*00003494*/ PUSH        {R4, R5, R6, LR} 
    /*00003498*/ MOV         R4, R1 
    /*0000349c*/ MOV         R5, R0 
    /*000034a0*/ BL          0x00000000000034A0          @ AOP_HAL_GetChildIntAllStatus [R_ARM_CALL: .text+0x341c] 
    /*000034a4*/ MOV         R2, R0 
    /*000034a8*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000034ac*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000034b0*/ STR         R2, [R4] 
    /*000034b4*/ ADD         R3, R0, R5, LSL #2 
    /*000034b8*/ MOV         R0, #0 
    /*000034bc*/ STR         R2, [R3, #232]              @ 0xe8 
    /*000034c0*/ POP         {R4, R5, R6, PC} 
FUNC_END AOP_HAL_GetIntStatus


FUNC_BEGIN AOP_HAL_ClrChildIntAllStatus
    /*000034c4*/ CMP         R0, #1 
    /*000034c8*/ PUSH        {R4, LR} 
    /*000034cc*/ MOV         R4, R0 
    /*000034d0*/ SUB         SP, SP, #8 
    /*000034d4*/ BLS         AOP_HAL_ClrChildIntAllStatus_x3518
    /*000034d8*/ STR         R0, [SP, #4] 
    /*000034dc*/ MOVW        R3, #1739                   @ 0x6cb 
    /*000034e0*/ MOV         R1, #20 
    /*000034e4*/ STR         R3, [SP] 
    /*000034e8*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170] 
    /*000034ec*/ LDR         R3, [PC, #72]               @ 0x000000000000353c 
    /*000034f0*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170] 
    /*000034f4*/ MOV         R0, #3 
    /*000034f8*/ BL          0x00000000000034F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000034fc*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c] 
    /*00003500*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130] 
    /*00003504*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c] 
    /*00003508*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130] 
    /*0000350c*/ MOVW        R2, #1740                   @ 0x6cc 
    /*00003510*/ LDR         R1, [PC, #36]               @ 0x000000000000353c 
    /*00003514*/ BL          0x0000000000003514          @ osal_panic [R_ARM_CALL: +0x0] 
    AOP_HAL_ClrChildIntAllStatus_x3518:
    /*00003518*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000351c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003520*/ MOVW        R0, #8364                   @ 0x20ac 
    /*00003524*/ ADD         R0, R0, R4, LSL #8 
    /*00003528*/ LDR         R3, [R3, #192]              @ 0xc0 
    /*0000352c*/ MOV         R2, #255                    @ 0xff 
    /*00003530*/ STR         R2, [R3, R0] 
    /*00003534*/ ADD         SP, SP, #8 
    /*00003538*/ POP         {R4, PC} 
    /*0000353c*/ MULEQ       R0, IP, R4 
FUNC_END AOP_HAL_ClrChildIntAllStatus


FUNC_BEGIN AOP_HAL_DevDisable
    /*00003540*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00003544*/ MOV         R1, #0 
    /*00003548*/ MOV         R5, R0 
    /*0000354c*/ SUB         SP, SP, #12 
    /*00003550*/ BL          0x0000000000003550          @ AOP_HAL_SetTxStart [R_ARM_CALL: .text+0x30e8] 
    /*00003554*/ MOV         R0, R5 
    /*00003558*/ MOV         R1, #0 
    /*0000355c*/ MOV         R4, #102                    @ 0x66 
    /*00003560*/ BL          0x0000000000003560          @ AOP_HAL_IntEn [R_ARM_CALL: .text+0x302c] 
    /*00003564*/ MOV         R0, R5 
    /*00003568*/ BL          0x0000000000003568          @ AOP_HAL_ClrChildIntAllStatus [R_ARM_CALL: .text+0x34c4] 
    /*0000356c*/ B           AOP_HAL_DevDisable_x357c
    AOP_HAL_DevDisable_x3570:
    /*00003570*/ SUBS        R4, R4, #1 
    /*00003574*/ BEQ         AOP_HAL_DevDisable_x362c
    /*00003578*/ BL          0x0000000000003578          @ osal_msleep [R_ARM_CALL: +0x0] 
    AOP_HAL_DevDisable_x357c:
    /*0000357c*/ MOV         R0, R5 
    /*00003580*/ BL          0x0000000000003580          @ AOP_HAL_GetDisDone [R_ARM_CALL: .text+0x316c] 
    /*00003584*/ CMP         R0, #0 
    /*00003588*/ MOV         R0, #10 
    /*0000358c*/ BEQ         AOP_HAL_DevDisable_x3570
    /*00003590*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003594*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    AOP_HAL_DevDisable_x3598:
    /*00003598*/ LDR         R0, [R6, #208]              @ 0xd0 
    /*0000359c*/ LSL         R1, R5, #2 
    /*000035a0*/ ADD         IP, R6, R1 
    /*000035a4*/ LDR         R3, [R0] 
    /*000035a8*/ EOR         R2, R3, #8192               @ 0x2000 
    /*000035ac*/ AND         R2, R2, #8192               @ 0x2000 
    /*000035b0*/ EOR         R3, R3, R2 
    /*000035b4*/ STR         R3, [R0] 
    /*000035b8*/ LDR         R0, [R6, #208]              @ 0xd0 
    /*000035bc*/ LDR         R3, [R0] 
    /*000035c0*/ EOR         R2, R3, #16384              @ 0x4000 
    /*000035c4*/ AND         R2, R2, #16384              @ 0x4000 
    /*000035c8*/ EOR         R3, R3, R2 
    /*000035cc*/ STR         R3, [R0] 
    /*000035d0*/ LDR         R2, [IP, #228]              @ 0xe4 
    /*000035d4*/ CMP         R2, #0 
    /*000035d8*/ BNE         AOP_HAL_DevDisable_x3618
    /*000035dc*/ LDR         IP, [R6, #192]              @ 0xc0 
    /*000035e0*/ LSL         R5, R5, #3 
    /*000035e4*/ ADD         R0, R5, #260                @ 0x104 
    /*000035e8*/ ADD         R5, R5, #324                @ 0x144 
    /*000035ec*/ LDR         R3, [IP, R0] 
    /*000035f0*/ BFI         R3, R2, #14, #1 
    /*000035f4*/ BFI         R3, R2, #15, #1 
    /*000035f8*/ ORR         R3, R3, #512                @ 0x200 
    /*000035fc*/ STR         R3, [IP, R0] 
    /*00003600*/ LDR         R0, [R6, #192]              @ 0xc0 
    /*00003604*/ LDR         R3, [R0, R5] 
    /*00003608*/ BFI         R3, R2, #14, #1 
    /*0000360c*/ BFI         R3, R2, #15, #1 
    /*00003610*/ ORR         R3, R3, #512                @ 0x200 
    /*00003614*/ STR         R3, [R0, R5] 
    AOP_HAL_DevDisable_x3618:
    /*00003618*/ ADD         R6, R6, R1 
    /*0000361c*/ MOV         R3, #0 
    /*00003620*/ STR         R3, [R6, #220]              @ 0xdc 
    /*00003624*/ ADD         SP, SP, #12 
    /*00003628*/ POP         {R4, R5, R6, R7, PC} 
    AOP_HAL_DevDisable_x362c:
    /*0000362c*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003630*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003634*/ LSL         R7, R5, #3 
    /*00003638*/ MOV         R0, #10 
    /*0000363c*/ LDR         R2, [R6, #192]              @ 0xc0 
    /*00003640*/ ADD         R7, R7, #324                @ 0x144 
    /*00003644*/ LDR         R3, [R2, R7] 
    /*00003648*/ ORR         R3, R3, #512                @ 0x200 
    /*0000364c*/ BFI         R3, R4, #8, #1 
    /*00003650*/ STR         R3, [R2, R7] 
    /*00003654*/ BL          0x0000000000003654          @ osal_msleep [R_ARM_CALL: +0x0] 
    /*00003658*/ LDR         R0, [R6, #192]              @ 0xc0 
    /*0000365c*/ MOVW        IP, #2517                   @ 0x9d5 
    /*00003660*/ MOVW        R2, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xad0] 
    /*00003664*/ MOV         R1, #20 
    /*00003668*/ MOVT        R2, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0xad0] 
    /*0000366c*/ LDR         R3, [R0, R7] 
    /*00003670*/ ORR         R3, R3, #256                @ 0x100 
    /*00003674*/ BFI         R3, R4, #9, #1 
    /*00003678*/ STR         R3, [R0, R7] 
    /*0000367c*/ LDR         R3, [PC, #12]               @ 0x0000000000003690 
    /*00003680*/ MOV         R0, #3 
    /*00003684*/ STR         IP, [SP] 
    /*00003688*/ BL          0x0000000000003688          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000368c*/ B           AOP_HAL_DevDisable_x3598
    /*00003690*/ @           <UNDEFINED> instruction: 0x000004bc 
FUNC_END AOP_HAL_DevDisable


FUNC_BEGIN AIAO_ModInit
    /*00003694*/ PUSH        {R4, R5, R6, LR} 
    /*00003698*/ MOVW        R0, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb38] 
    /*0000369c*/ SUB         SP, SP, #32 
    /*000036a0*/ MOV         R1, #0 
    /*000036a4*/ MOVT        R0, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb38] 
    /*000036a8*/ BL          0x00000000000036A8          @ osal_create_proc_entry [R_ARM_CALL: +0x0] 
    /*000036ac*/ SUBS        R2, R0, #0 
    /*000036b0*/ BEQ         AIAO_ModInit_x38a0
    /*000036b4*/ MOVW        R3, #0                      @ AiaoProcShow [R_ARM_MOVW_ABS_NC: .text+0x11c] 
    /*000036b8*/ MOVW        R0, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb38] 
    /*000036bc*/ MOVT        R3, #0                      @ AiaoProcShow [R_ARM_MOVT_ABS: .text+0x11c] 
    /*000036c0*/ MOVT        R0, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb38] 
    /*000036c4*/ STR         R3, [R2, #60]               @ 0x3c 
    /*000036c8*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000036cc*/ BL          0x00000000000036CC          @ osal_createdev [R_ARM_CALL: +0x0] 
    /*000036d0*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000036d4*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000036d8*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000036dc*/ ADD         R1, R4, #16 
    /*000036e0*/ MOV         R2, #11 
    /*000036e4*/ STR         R1, [R0, #56]               @ 0x38 
    /*000036e8*/ STR         R2, [R0, #52]               @ 0x34 
    /*000036ec*/ ADD         R2, R4, #48                 @ 0x30 
    /*000036f0*/ STR         R2, [R0, #60]               @ 0x3c 
    /*000036f4*/ STR         R0, [R5, #240]              @ 0xf0 
    /*000036f8*/ BL          0x00000000000036F8          @ osal_registerdevice [R_ARM_CALL: +0x0] 
    /*000036fc*/ SUBS        R6, R0, #0 
    /*00003700*/ BNE         AIAO_ModInit_x3830
    /*00003704*/ ADD         R0, R4, #128                @ 0x80 
    /*00003708*/ BL          0x0000000000003708          @ CMPI_RegisterModule [R_ARM_CALL: +0x0] 
    /*0000370c*/ CMP         R0, #0 
    /*00003710*/ BNE         AIAO_ModInit_x3878
    /*00003714*/ LDR         R2, [R5, #192]              @ 0xc0 
    /*00003718*/ MOV         R1, #20 
    /*0000371c*/ MOV         R3, #1 
    /*00003720*/ STR         R0, [SP, #24] 
    /*00003724*/ CMP         R2, #0 
    /*00003728*/ STR         R0, [SP, #12] 
    /*0000372c*/ STR         R1, [SP, #20] 
    /*00003730*/ STR         R3, [SP, #16] 
    /*00003734*/ BEQ         AIAO_ModInit_x3808
    AIAO_ModInit_x3738:
    /*00003738*/ LDR         R3, [R2] 
    /*0000373c*/ MOV         R0, #2 
    /*00003740*/ MOVW        LR, #12016                  @ 0x2ef0 
    /*00003744*/ MOVW        IP, #30291                  @ 0x7653 
    /*00003748*/ BFC         R3, #16, #1 
    /*0000374c*/ STR         R3, [R2] 
    /*00003750*/ LDR         R2, [R5, #192]              @ 0xc0 
    /*00003754*/ MOVT        LR, #21 
    /*00003758*/ MOVT        IP, #19 
    /*0000375c*/ STR         LR, [R4, #4] 
    /*00003760*/ STR         IP, [R4, #12] 
    /*00003764*/ MOVW        R1, #8011                   @ 0x1f4b 
    /*00003768*/ LDR         R3, [R2, #52]               @ 0x34 
    /*0000376c*/ MOVT        R1, #14 
    /*00003770*/ STR         R1, [R4, #8] 
    /*00003774*/ ORR         R3, R3, #7 
    /*00003778*/ STR         R3, [R2, #52]               @ 0x34 
    /*0000377c*/ BL          0x000000000000377C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003780*/ MOV         R1, #34                     @ 0x22 
    /*00003784*/ LDR         R3, [R0, #20] 
    /*00003788*/ ADD         R2, SP, #16 
    /*0000378c*/ ADD         R0, SP, #20 
    /*00003790*/ BLX         R3 
    /*00003794*/ MOV         R0, #2 
    /*00003798*/ BL          0x0000000000003798          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000379c*/ MOV         R1, #33                     @ 0x21 
    /*000037a0*/ LDR         R3, [R0, #20] 
    /*000037a4*/ ADD         R2, SP, #12 
    /*000037a8*/ ADD         R0, SP, #20 
    /*000037ac*/ BLX         R3 
    /*000037b0*/ MOVW        IP, #0                      @ AioInterruptRoute [R_ARM_MOVW_ABS_NC: .text+0x84] 
    /*000037b4*/ MOVT        IP, #0                      @ AioInterruptRoute [R_ARM_MOVT_ABS: .text+0x84] 
    /*000037b8*/ LDR         R0, [R4, #192]              @ 0xc0 
    /*000037bc*/ MOVW        R3, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb78] 
    /*000037c0*/ MOV         R2, #0 
    /*000037c4*/ MOV         R1, IP 
    /*000037c8*/ MOVT        R3, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb78] 
    /*000037cc*/ STR         IP, [SP] 
    /*000037d0*/ BL          0x00000000000037D0          @ osal_request_irq [R_ARM_CALL: +0x0] 
    /*000037d4*/ SUBS        R6, R0, #0 
    /*000037d8*/ BNE         AIAO_ModInit_x3848
    /*000037dc*/ MOV         R1, #1 
    /*000037e0*/ LDR         R0, [PC, #224]              @ 0x00000000000038c8 
    /*000037e4*/ BL          0x00000000000037E4          @ osal_sema_init [R_ARM_CALL: +0x0] 
    /*000037e8*/ LDR         R0, [PC, #220]              @ 0x00000000000038cc 
    /*000037ec*/ BL          0x00000000000037EC          @ osal_atomic_init [R_ARM_CALL: +0x0] 
    /*000037f0*/ MOV         R1, R6 
    /*000037f4*/ LDR         R0, [PC, #208]              @ 0x00000000000038cc 
    /*000037f8*/ BL          0x00000000000037F8          @ osal_atomic_set [R_ARM_CALL: +0x0] 
    AIAO_ModInit_x37fc:
    /*000037fc*/ MOV         R0, R6 
    /*00003800*/ ADD         SP, SP, #32 
    /*00003804*/ POP         {R4, R5, R6, PC} 
    AIAO_ModInit_x3808:
    /*00003808*/ MOV         R0, #0 
    /*0000380c*/ MOV         R1, #65536                  @ 0x10000 
    /*00003810*/ MOVT        R0, #4411                   @ 0x113b 
    /*00003814*/ BL          0x0000000000003814          @ osal_ioremap [R_ARM_CALL: +0x0] 
    /*00003818*/ CMP         R0, #0 
    /*0000381c*/ MOV         R2, R0 
    /*00003820*/ STR         R0, [R5, #192]              @ 0xc0 
    /*00003824*/ BNE         AIAO_ModInit_x3738
    /*00003828*/ MVN         R6, #0 
    /*0000382c*/ B           AIAO_ModInit_x37fc
    AIAO_ModInit_x3830:
    /*00003830*/ MOVW        R0, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb70] 
    /*00003834*/ MOV         R1, #0 
    /*00003838*/ MOVT        R0, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb70] 
    /*0000383c*/ MVN         R6, #0 
    /*00003840*/ BL          0x0000000000003840          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*00003844*/ B           AIAO_ModInit_x37fc
    AIAO_ModInit_x3848:
    /*00003848*/ LDR         R1, [R4, #192]              @ 0xc0 
    /*0000384c*/ MOVW        R3, #2997                   @ 0xbb5 
    /*00003850*/ MOVW        R2, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb88] 
    /*00003854*/ STR         R3, [SP] 
    /*00003858*/ MOVT        R2, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb88] 
    /*0000385c*/ LDR         R3, [PC, #108]              @ 0x00000000000038d0 
    /*00003860*/ STR         R1, [SP, #4] 
    /*00003864*/ MOV         R0, #3 
    /*00003868*/ MOV         R1, #20 
    /*0000386c*/ MVN         R6, #0 
    /*00003870*/ BL          0x0000000000003870          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003874*/ B           AIAO_ModInit_x37fc
    AIAO_ModInit_x3878:
    /*00003878*/ MOV         R1, R6 
    /*0000387c*/ MOVW        R0, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb38] 
    /*00003880*/ MOVT        R0, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb38] 
    /*00003884*/ MVN         R6, #0 
    /*00003888*/ BL          0x0000000000003888          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*0000388c*/ LDR         R0, [R5, #240]              @ 0xf0 
    /*00003890*/ BL          0x0000000000003890          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*00003894*/ LDR         R0, [R5, #240]              @ 0xf0 
    /*00003898*/ BL          0x0000000000003898          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*0000389c*/ B           AIAO_ModInit_x37fc
    AIAO_ModInit_x38a0:
    /*000038a0*/ MOVW        R3, #2933                   @ 0xb75 
    /*000038a4*/ MOVW        R2, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb3c] 
    /*000038a8*/ STR         R3, [SP] 
    /*000038ac*/ MOVT        R2, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb3c] 
    /*000038b0*/ LDR         R3, [PC, #24]               @ 0x00000000000038d0 
    /*000038b4*/ MOV         R1, #22 
    /*000038b8*/ MOV         R0, #6 
    /*000038bc*/ MVN         R6, #0 
    /*000038c0*/ BL          0x00000000000038C0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000038c4*/ B           AIAO_ModInit_x37fc
    /*000038c8*/ ANDEQ       R0, R0, R4, ASR #1 
    /*000038cc*/ ANDEQ       R0, R0, IP, ASR #1 
    /*000038d0*/ LDRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
FUNC_END AIAO_ModInit


FUNC_BEGIN AIAO_ModExit
    /*000038d4*/ PUSH        {R4, R5, R6, LR} 
    /*000038d8*/ SUB         SP, SP, #24 
    /*000038dc*/ MOV         R0, #2 
    /*000038e0*/ MOV         R5, #0 
    /*000038e4*/ MOV         R3, #1 
    /*000038e8*/ MOV         R6, #20 
    /*000038ec*/ STMIB       SP, {R3, R5} 
    /*000038f0*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000038f4*/ STR         R5, [SP, #16] 
    /*000038f8*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000038fc*/ STR         R6, [SP, #12] 
    /*00003900*/ BL          0x0000000000003900          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003904*/ MOV         R1, #33                     @ 0x21 
    /*00003908*/ LDR         R3, [R0, #20] 
    /*0000390c*/ ADD         R2, SP, #4 
    /*00003910*/ ADD         R0, SP, #12 
    /*00003914*/ BLX         R3 
    /*00003918*/ MOV         R0, #2 
    /*0000391c*/ BL          0x000000000000391C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003920*/ MOV         R1, #34                     @ 0x22 
    /*00003924*/ ADD         R2, SP, #8 
    /*00003928*/ LDR         R3, [R0, #20] 
    /*0000392c*/ ADD         R0, SP, #12 
    /*00003930*/ BLX         R3 
    /*00003934*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00003938*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*0000393c*/ MOVW        R1, #0                      @ AioInterruptRoute [R_ARM_MOVW_ABS_NC: .text+0x84] 
    /*00003940*/ MOVT        R1, #0                      @ AioInterruptRoute [R_ARM_MOVT_ABS: .text+0x84] 
    /*00003944*/ LDR         R0, [R3, #192]              @ 0xc0 
    /*00003948*/ BL          0x0000000000003948          @ osal_free_irq [R_ARM_CALL: +0x0] 
    /*0000394c*/ ADD         R0, R4, #204                @ 0xcc 
    /*00003950*/ BL          0x0000000000003950          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*00003954*/ MOV         R0, R6 
    /*00003958*/ BL          0x0000000000003958          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    /*0000395c*/ LDR         R0, [R4, #240]              @ 0xf0 
    /*00003960*/ BL          0x0000000000003960          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*00003964*/ LDR         R0, [R4, #240]              @ 0xf0 
    /*00003968*/ BL          0x0000000000003968          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*0000396c*/ MOV         R1, R5 
    /*00003970*/ MOVW        R0, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb38] 
    /*00003974*/ MOVT        R0, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb38] 
    /*00003978*/ BL          0x0000000000003978          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*0000397c*/ ADD         SP, SP, #24 
    /*00003980*/ POP         {R4, R5, R6, PC} 
FUNC_END AIAO_ModExit


FUNC_BEGIN hi35xx_aiao_remove
    /*00003984*/ PUSH        {R4, LR} 
    /*00003988*/ BL          0x0000000000003988          @ AIAO_ModExit [R_ARM_CALL: .text+0x38d4] 
    /*0000398c*/ MOVW        R1, #0                      @ pAioBase [R_ARM_MOVW_ABS_NC: .bss+0xc0] 
    /*00003990*/ MOVW        R3, #0                      @ pAcodec [R_ARM_MOVW_ABS_NC: .bss+0xf8] 
    /*00003994*/ MOVT        R1, #0                      @ pAioBase [R_ARM_MOVT_ABS: .bss+0xc0] 
    /*00003998*/ MOVT        R3, #0                      @ pAcodec [R_ARM_MOVT_ABS: .bss+0xf8] 
    /*0000399c*/ MOV         R2, #0 
    /*000039a0*/ MOV         R0, R2 
    /*000039a4*/ STR         R2, [R1] 
    /*000039a8*/ STR         R2, [R3] 
    /*000039ac*/ POP         {R4, PC} 
FUNC_END hi35xx_aiao_remove


FUNC_BEGIN hi35xx_aiao_probe
    /*000039b0*/ PUSH        {R4, R5, R6, LR} 
    /*000039b4*/ MOV         R1, #512                    @ 0x200 
    /*000039b8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbc0] 
    /*000039bc*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbc0] 
    /*000039c0*/ MOV         R4, R0 
    /*000039c4*/ BL          0x00000000000039C4          @ osal_platform_get_resource_byname [R_ARM_CALL: +0x0] 
    /*000039c8*/ ADD         R5, R4, #16 
    /*000039cc*/ MOV         R1, R0 
    /*000039d0*/ MOV         R0, R5 
    /*000039d4*/ BL          0x00000000000039D4          @ devm_ioremap_resource [R_ARM_CALL: +0x0] 
    /*000039d8*/ MOVW        R3, #0                      @ pAioBase [R_ARM_MOVW_ABS_NC: .bss+0xc0] 
    /*000039dc*/ MOVT        R3, #0                      @ pAioBase [R_ARM_MOVT_ABS: .bss+0xc0] 
    /*000039e0*/ CMN         R0, #4096                   @ 0x1000 
    /*000039e4*/ STR         R0, [R3] 
    /*000039e8*/ POPHI       {R4, R5, R6, PC} 
    /*000039ec*/ MOV         R1, #512                    @ 0x200 
    /*000039f0*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbc8] 
    /*000039f4*/ MOV         R0, R4 
    /*000039f8*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbc8] 
    /*000039fc*/ BL          0x00000000000039FC          @ osal_platform_get_resource_byname [R_ARM_CALL: +0x0] 
    /*00003a00*/ MOV         R1, R0 
    /*00003a04*/ MOV         R0, R5 
    /*00003a08*/ BL          0x0000000000003A08          @ devm_ioremap_resource [R_ARM_CALL: +0x0] 
    /*00003a0c*/ MOVW        R2, #0                      @ pAcodec [R_ARM_MOVW_ABS_NC: .bss+0xf8] 
    /*00003a10*/ MOVT        R2, #0                      @ pAcodec [R_ARM_MOVT_ABS: .bss+0xf8] 
    /*00003a14*/ CMN         R0, #4096                   @ 0x1000 
    /*00003a18*/ MOV         R3, R0 
    /*00003a1c*/ STR         R0, [R2] 
    /*00003a20*/ BHI         hi35xx_aiao_probe_x3a74
    /*00003a24*/ MOVW        R2, #0                      @ reg_acodec_base_va [R_ARM_MOVW_ABS_NC: .bss+0xc8] 
    /*00003a28*/ MOVT        R2, #0                      @ reg_acodec_base_va [R_ARM_MOVT_ABS: .bss+0xc8] 
    /*00003a2c*/ MOV         R0, R4 
    /*00003a30*/ MOVW        R1, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbd0] 
    /*00003a34*/ MOVT        R1, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbd0] 
    /*00003a38*/ STR         R3, [R2] 
    /*00003a3c*/ BL          0x0000000000003A3C          @ osal_platform_get_irq_byname [R_ARM_CALL: +0x0] 
    /*00003a40*/ MOVW        R3, #0                      @ aiao_irq [R_ARM_MOVW_ABS_NC: .data+0xc0] 
    /*00003a44*/ MOVT        R3, #0                      @ aiao_irq [R_ARM_MOVT_ABS: .data+0xc0] 
    /*00003a48*/ CMP         R0, #0 
    /*00003a4c*/ STR         R0, [R3] 
    /*00003a50*/ BEQ         hi35xx_aiao_probe_x3a60
    hi35xx_aiao_probe_x3a54:
    /*00003a54*/ BL          0x0000000000003A54          @ AIAO_ModInit [R_ARM_CALL: .text+0x3694] 
    /*00003a58*/ MOV         R0, #0 
    /*00003a5c*/ POP         {R4, R5, R6, PC} 
    hi35xx_aiao_probe_x3a60:
    /*00003a60*/ MOV         R0, R5 
    /*00003a64*/ MOVW        R1, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbd4] 
    /*00003a68*/ MOVT        R1, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbd4] 
    /*00003a6c*/ BL          0x0000000000003A6C          @ dev_err [R_ARM_CALL: +0x0] 
    /*00003a70*/ B           hi35xx_aiao_probe_x3a54
    hi35xx_aiao_probe_x3a74:
    /*00003a74*/ POP         {R4, R5, R6, PC} 
FUNC_END hi35xx_aiao_probe


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .init.text
@ Size: 0xc
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN hi35xx_aiao_driver_init
    /*00000000*/ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x170] 
    /*00000004*/ MOVT        R0, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x170] 
    hi35xx_aiao_driver_init_x8:
    /*00000008*/ B           hi35xx_aiao_driver_init_x8  @ osal_platform_driver_register [R_ARM_JUMP24: +0x0] 
FUNC_END hi35xx_aiao_driver_init




@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .exit.text
@ Size: 0xc
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN hi35xx_aiao_driver_exit
    /*00000000*/ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x170] 
    /*00000004*/ MOVT        R0, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x170] 
    hi35xx_aiao_driver_exit_x8:
    /*00000008*/ B           hi35xx_aiao_driver_exit_x8  @ osal_platform_driver_unregister [R_ARM_JUMP24: +0x0] 
FUNC_END hi35xx_aiao_driver_exit




