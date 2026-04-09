@
@ -- FILES --
@

@ jpege_ext.c
@ jpege_ctrl.c
@ jpege_proc.c
@ string.c
@ jpege_drv.c
@ jpege_init.c
@ hi3516cv500_jpege.mod.c


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text
@ Size: 0x7488
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN JPEGE_SysResetChn
    /*00000000*/ MOV         R0, #0 
    /*00000004*/ BX          LR 
FUNC_END JPEGE_SysResetChn


FUNC_BEGIN JPEGE_SetUniqueID
    /*00000008*/ CMP         R0, #15 
    /*0000000c*/ MOV         R2, R0 
    /*00000010*/ MOVWLS      R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000014*/ MOVTLS      R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000018*/ MOVLS       R0, #0 
    /*0000001c*/ MOVWHI      R0, #32770                  @ 0x8002 
    /*00000020*/ STRLS       R1, [R3, R2, LSL #2] 
    /*00000024*/ MOVTHI      R0, #40971                  @ 0xa00b 
    /*00000028*/ BX          LR 
FUNC_END JPEGE_SetUniqueID


FUNC_BEGIN JPEGE_GetUniqueID
    /*0000002c*/ CMP         R0, #15 
    /*00000030*/ MOV         R2, R0 
    /*00000034*/ BHI         JPEGE_GetUniqueID_x58
    /*00000038*/ CMP         R1, #0 
    /*0000003c*/ BEQ         JPEGE_GetUniqueID_x64
    /*00000040*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000044*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000048*/ MOV         R0, #0 
    /*0000004c*/ LDR         R3, [R3, R2, LSL #2] 
    /*00000050*/ STR         R3, [R1] 
    /*00000054*/ BX          LR 
    JPEGE_GetUniqueID_x58:
    /*00000058*/ MOVW        R0, #32770                  @ 0x8002 
    /*0000005c*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000060*/ BX          LR 
    JPEGE_GetUniqueID_x64:
    /*00000064*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000068*/ MOVT        R0, #40971                  @ 0xa00b 
    /*0000006c*/ BX          LR 
FUNC_END JPEGE_GetUniqueID


FUNC_BEGIN JPEGE_GetClearStreamBufFlag
    /*00000070*/ SUBS        R2, R0, #0 
    /*00000074*/ MOVWNE      R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000078*/ MOVTNE      R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x0] 
    /*0000007c*/ MOVNE       R0, #0 
    /*00000080*/ MOVWEQ      R0, #32774                  @ 0x8006 
    /*00000084*/ LDRNE       R3, [R3] 
    /*00000088*/ MOVTEQ      R0, #40971                  @ 0xa00b 
    /*0000008c*/ STRNE       R3, [R2] 
    /*00000090*/ BX          LR 
FUNC_END JPEGE_GetClearStreamBufFlag


FUNC_BEGIN JPEGE_GetModPara
    /*00000094*/ SUBS        R2, R0, #0 
    /*00000098*/ BEQ         JPEGE_GetModPara_xd0
    /*0000009c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000000a0*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000000a4*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000000a8*/ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000000ac*/ LDR         LR, [R3, #68]               @ 0x44 
    /*000000b0*/ MOVT        R1, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x0] 
    /*000000b4*/ LDR         IP, [R3, #64]               @ 0x40 
    /*000000b8*/ MOV         R0, #0 
    /*000000bc*/ STR         LR, [R2] 
    /*000000c0*/ LDR         R3, [R1] 
    /*000000c4*/ STR         IP, [R2, #4] 
    /*000000c8*/ STR         R3, [R2, #8] 
    /*000000cc*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    JPEGE_GetModPara_xd0:
    /*000000d0*/ MOVW        R0, #32774                  @ 0x8006 
    /*000000d4*/ MOVT        R0, #40971                  @ 0xa00b 
    /*000000d8*/ BX          LR 
FUNC_END JPEGE_GetModPara


FUNC_BEGIN JPEGE_GetVerMagic
    /*000000dc*/ MOVW        R0, #61479                  @ 0xf027 
    /*000000e0*/ MOVT        R0, #307                    @ 0x133 
    /*000000e4*/ BX          LR 
FUNC_END JPEGE_GetVerMagic


FUNC_BEGIN JPEGE_SetModPara
    /*000000e8*/ SUBS        R3, R0, #0 
    /*000000ec*/ BEQ         JPEGE_SetModPara_x1e4
    /*000000f0*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000000f4*/ SUB         SP, SP, #12 
    /*000000f8*/ LDR         R0, [R3, #4] 
    /*000000fc*/ CMP         R0, #1 
    /*00000100*/ BHI         JPEGE_SetModPara_x148
    /*00000104*/ LDR         R2, [R3] 
    /*00000108*/ CMP         R2, #1 
    /*0000010c*/ BHI         JPEGE_SetModPara_x17c
    /*00000110*/ LDR         IP, [R3, #8] 
    /*00000114*/ CMP         IP, #1 
    /*00000118*/ BHI         JPEGE_SetModPara_x1b0
    /*0000011c*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000120*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000124*/ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000128*/ MOVT        R1, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x0] 
    /*0000012c*/ STR         R0, [R2, #64]               @ 0x40 
    /*00000130*/ MOV         R0, #0 
    /*00000134*/ LDR         R3, [R3] 
    /*00000138*/ STR         IP, [R1] 
    /*0000013c*/ STR         R3, [R2, #68]               @ 0x44 
    JPEGE_SetModPara_x140:
    /*00000140*/ ADD         SP, SP, #12 
    /*00000144*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    JPEGE_SetModPara_x148:
    /*00000148*/ STR         R0, [SP, #4] 
    /*0000014c*/ MOVW        R2, #3263                   @ 0xcbf 
    /*00000150*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000154*/ STR         R2, [SP] 
    /*00000158*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*0000015c*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*00000160*/ MOV         R1, #8 
    /*00000164*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*00000168*/ MOV         R0, #3 
    /*0000016c*/ BL          0x000000000000016C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000170*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000174*/ MOVT        R0, #40968                  @ 0xa008 
    /*00000178*/ B           JPEGE_SetModPara_x140
    JPEGE_SetModPara_x17c:
    /*0000017c*/ STR         R2, [SP, #4] 
    /*00000180*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000184*/ MOVW        R2, #3268                   @ 0xcc4 
    /*00000188*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*0000018c*/ STR         R2, [SP] 
    /*00000190*/ MOV         R1, #8 
    /*00000194*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x54] 
    /*00000198*/ MOV         R0, #3 
    /*0000019c*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x54] 
    /*000001a0*/ BL          0x00000000000001A0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000001a4*/ MOVW        R0, #32771                  @ 0x8003 
    /*000001a8*/ MOVT        R0, #40968                  @ 0xa008 
    /*000001ac*/ B           JPEGE_SetModPara_x140
    JPEGE_SetModPara_x1b0:
    /*000001b0*/ MOVW        R2, #3274                   @ 0xcca 
    /*000001b4*/ STR         IP, [SP, #4] 
    /*000001b8*/ STR         R2, [SP] 
    /*000001bc*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000001c0*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa4] 
    /*000001c4*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*000001c8*/ MOV         R1, #8 
    /*000001cc*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa4] 
    /*000001d0*/ MOV         R0, #3 
    /*000001d4*/ BL          0x00000000000001D4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000001d8*/ MOVW        R0, #32771                  @ 0x8003 
    /*000001dc*/ MOVT        R0, #40968                  @ 0xa008 
    /*000001e0*/ B           JPEGE_SetModPara_x140
    JPEGE_SetModPara_x1e4:
    /*000001e4*/ MOVW        R0, #32774                  @ 0x8006 
    /*000001e8*/ MOVT        R0, #40971                  @ 0xa00b 
    /*000001ec*/ BX          LR 
FUNC_END JPEGE_SetModPara


FUNC_BEGIN JPEGE_GetParam
    /*000001f0*/ CMP         R0, #15 
    /*000001f4*/ BHI         JPEGE_GetParam_x280
    /*000001f8*/ CMP         R1, #0 
    /*000001fc*/ BEQ         JPEGE_GetParam_x28c
    /*00000200*/ PUSH        {R4, LR} 
    /*00000204*/ MOV         R4, R1 
    /*00000208*/ BL          0x0000000000000208          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*0000020c*/ CMP         R0, #0 
    /*00000210*/ BEQ         JPEGE_GetParam_x298
    /*00000214*/ ADD         R3, R0, #4096               @ 0x1000 
    /*00000218*/ ADD         IP, R0, #6976               @ 0x1b40 
    /*0000021c*/ ADD         LR, R0, #7040               @ 0x1b80 
    /*00000220*/ ADD         IP, IP, #3 
    /*00000224*/ LDR         R2, [R3, #2872]             @ 0xb38 
    /*00000228*/ ADD         LR, LR, #3 
    /*0000022c*/ MOV         R0, R4 
    /*00000230*/ STR         R2, [R4] 
    /*00000234*/ LDR         R3, [R3, #3068]             @ 0xbfc 
    /*00000238*/ STR         R3, [R4, #196]              @ 0xc4 
    JPEGE_GetParam_x23c:
    /*0000023c*/ SUB         R3, IP, #8 
    /*00000240*/ MOV         R2, R0 
    JPEGE_GetParam_x244:
    /*00000244*/ LDRB        R1, [R3, #1]! 
    /*00000248*/ ADD         R2, R2, #8 
    /*0000024c*/ CMP         R3, IP 
    /*00000250*/ STRB        R1, [R2, #-4] 
    /*00000254*/ LDRB        R1, [R3, #64]               @ 0x40 
    /*00000258*/ STRB        R1, [R2, #60]               @ 0x3c 
    /*0000025c*/ LDRB        R1, [R3, #128]              @ 0x80 
    /*00000260*/ STRB        R1, [R2, #124]              @ 0x7c 
    /*00000264*/ BNE         JPEGE_GetParam_x244
    /*00000268*/ ADD         IP, R3, #8 
    /*0000026c*/ ADD         R0, R0, #1 
    /*00000270*/ CMP         IP, LR 
    /*00000274*/ BNE         JPEGE_GetParam_x23c
    /*00000278*/ MOV         R0, #0 
    /*0000027c*/ POP         {R4, PC} 
    JPEGE_GetParam_x280:
    /*00000280*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000284*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000288*/ BX          LR 
    JPEGE_GetParam_x28c:
    /*0000028c*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000290*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000294*/ BX          LR 
    JPEGE_GetParam_x298:
    /*00000298*/ MOVW        R0, #32773                  @ 0x8005 
    /*0000029c*/ MOVT        R0, #40971                  @ 0xa00b 
    /*000002a0*/ POP         {R4, PC} 
FUNC_END JPEGE_GetParam


FUNC_BEGIN JPEGE_GetChnParam
    /*000002a4*/ CMP         R0, #15 
    /*000002a8*/ BHI         JPEGE_GetChnParam_x2e4
    /*000002ac*/ CMP         R1, #0 
    /*000002b0*/ BEQ         JPEGE_GetChnParam_x2f0
    /*000002b4*/ PUSH        {R4, LR} 
    /*000002b8*/ MOV         R4, R1 
    /*000002bc*/ BL          0x00000000000002BC          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*000002c0*/ CMP         R0, #0 
    /*000002c4*/ BEQ         JPEGE_GetChnParam_x2fc
    /*000002c8*/ LDR         R2, [R0, #3976]             @ 0xf88 
    /*000002cc*/ ADD         R3, R0, #4096               @ 0x1000 
    /*000002d0*/ MOV         R0, #0 
    /*000002d4*/ STR         R2, [R4, #8] 
    /*000002d8*/ LDR         R3, [R3, #3760]             @ 0xeb0 
    /*000002dc*/ STR         R3, [R4] 
    /*000002e0*/ POP         {R4, PC} 
    JPEGE_GetChnParam_x2e4:
    /*000002e4*/ MOVW        R0, #32770                  @ 0x8002 
    /*000002e8*/ MOVT        R0, #40971                  @ 0xa00b 
    /*000002ec*/ BX          LR 
    JPEGE_GetChnParam_x2f0:
    /*000002f0*/ MOVW        R0, #32774                  @ 0x8006 
    /*000002f4*/ MOVT        R0, #40971                  @ 0xa00b 
    /*000002f8*/ BX          LR 
    JPEGE_GetChnParam_x2fc:
    /*000002fc*/ MOVW        R0, #32773                  @ 0x8005 
    /*00000300*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000304*/ POP         {R4, PC} 
FUNC_END JPEGE_GetChnParam


FUNC_BEGIN JPEGE_ResetChn
    /*00000308*/ CMP         R0, #15 
    /*0000030c*/ BHI         JPEGE_ResetChn_x404
    /*00000310*/ PUSH        {R4, R5, R6, LR} 
    /*00000314*/ BL          0x0000000000000314          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00000318*/ SUBS        R4, R0, #0 
    /*0000031c*/ BEQ         JPEGE_ResetChn_x410
    /*00000320*/ LDR         R2, [R4, #452]              @ 0x1c4 
    /*00000324*/ MOV         R3, #0 
    /*00000328*/ MOVW        R1, #0                      @ VencBufferCache [R_ARM_MOVW_ABS_NC: +0x0] 
    /*0000032c*/ MOVT        R1, #0                      @ VencBufferCache [R_ARM_MOVT_ABS: +0x0] 
    /*00000330*/ LDR         R0, [R2, #160]              @ 0xa0 
    /*00000334*/ STR         R3, [R0] 
    /*00000338*/ LDR         R0, [R2, #164]              @ 0xa4 
    /*0000033c*/ STR         R3, [R0] 
    /*00000340*/ LDR         R0, [R2, #168]              @ 0xa8 
    /*00000344*/ STR         R3, [R0] 
    /*00000348*/ LDR         R2, [R2, #172]              @ 0xac 
    /*0000034c*/ STR         R3, [R2] 
    /*00000350*/ LDR         R3, [R1] 
    /*00000354*/ CMP         R3, #1 
    /*00000358*/ BEQ         JPEGE_ResetChn_x3ec
    JPEGE_ResetChn_x35c:
    /*0000035c*/ LDR         R2, [R4, #424]              @ 0x1a8 
    /*00000360*/ MOV         R3, #0 
    /*00000364*/ STR         R3, [R4, #3980]             @ 0xf8c 
    /*00000368*/ ADD         R5, R4, #4096               @ 0x1000 
    /*0000036c*/ STR         R3, [R2] 
    /*00000370*/ LDR         R2, [R4, #428]              @ 0x1ac 
    /*00000374*/ STR         R3, [R2] 
    /*00000378*/ LDR         R2, [R4, #432]              @ 0x1b0 
    /*0000037c*/ STR         R3, [R2] 
    /*00000380*/ LDR         R2, [R4, #436]              @ 0x1b4 
    /*00000384*/ STR         R3, [R2] 
    /*00000388*/ LDR         R2, [R4, #16] 
    /*0000038c*/ CMP         R2, R3 
    /*00000390*/ BEQ         JPEGE_ResetChn_x3b0
    /*00000394*/ ADD         R0, R4, #3920               @ 0xf50 
    /*00000398*/ MOV         R1, R3 
    /*0000039c*/ ADD         R0, R0, #4 
    /*000003a0*/ STR         R3, [R5, #3704]             @ 0xe78 
    /*000003a4*/ MOV         R2, #48                     @ 0x30 
    /*000003a8*/ STR         R3, [R5, #3700]             @ 0xe74 
    /*000003ac*/ BL          0x00000000000003AC          @ osal_memset [R_ARM_CALL: +0x0] 
    JPEGE_ResetChn_x3b0:
    /*000003b0*/ MOV         R3, #0 
    /*000003b4*/ MOV         R2, #1 
    /*000003b8*/ STR         R3, [R5, #3780]             @ 0xec4 
    /*000003bc*/ MOV         R0, R3 
    /*000003c0*/ STR         R2, [R5, #3824]             @ 0xef0 
    /*000003c4*/ STR         R3, [R5, #3784]             @ 0xec8 
    /*000003c8*/ STR         R3, [R5, #3788]             @ 0xecc 
    /*000003cc*/ STR         R3, [R5, #3792]             @ 0xed0 
    /*000003d0*/ STR         R3, [R5, #3796]             @ 0xed4 
    /*000003d4*/ STR         R3, [R5, #3772]             @ 0xebc 
    /*000003d8*/ STR         R3, [R5, #3800]             @ 0xed8 
    /*000003dc*/ LDR         R3, [R4, #3972]             @ 0xf84 
    /*000003e0*/ ADD         R3, R3, R2 
    /*000003e4*/ STR         R3, [R4, #3972]             @ 0xf84 
    /*000003e8*/ POP         {R4, R5, R6, PC} 
    JPEGE_ResetChn_x3ec:
    /*000003ec*/ LDR         R3, [R4, #452]              @ 0x1c4 
    /*000003f0*/ LDR         R1, [R4, #8] 
    /*000003f4*/ LDR         R2, [R3, #8] 
    /*000003f8*/ LDR         R0, [R3, #176]              @ 0xb0 
    /*000003fc*/ BL          0x00000000000003FC          @ hil_mmb_invalid_cache_byaddr [R_ARM_CALL: +0x0] 
    /*00000400*/ B           JPEGE_ResetChn_x35c
    JPEGE_ResetChn_x404:
    /*00000404*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000408*/ MOVT        R0, #40971                  @ 0xa00b 
    /*0000040c*/ BX          LR 
    JPEGE_ResetChn_x410:
    /*00000410*/ MOVW        R0, #32773                  @ 0x8005 
    /*00000414*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000418*/ POP         {R4, R5, R6, PC} 
FUNC_END JPEGE_ResetChn


FUNC_BEGIN JPEGE_GetChnAttr
    /*0000041c*/ CMP         R0, #15 
    /*00000420*/ BHI         JPEGE_GetChnAttr_x458
    /*00000424*/ CMP         R1, #0 
    /*00000428*/ BEQ         JPEGE_GetChnAttr_x464
    /*0000042c*/ PUSH        {R4, LR} 
    /*00000430*/ MOV         R4, R1 
    /*00000434*/ BL          0x0000000000000434          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00000438*/ CMP         R0, #0 
    /*0000043c*/ BEQ         JPEGE_GetChnAttr_x470
    /*00000440*/ ADD         R1, R0, #16 
    /*00000444*/ MOV         R2, #248                    @ 0xf8 
    /*00000448*/ MOV         R0, R4 
    /*0000044c*/ BL          0x000000000000044C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000450*/ MOV         R0, #0 
    /*00000454*/ POP         {R4, PC} 
    JPEGE_GetChnAttr_x458:
    /*00000458*/ MOVW        R0, #32770                  @ 0x8002 
    /*0000045c*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000460*/ BX          LR 
    JPEGE_GetChnAttr_x464:
    /*00000464*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000468*/ MOVT        R0, #40971                  @ 0xa00b 
    /*0000046c*/ BX          LR 
    JPEGE_GetChnAttr_x470:
    /*00000470*/ MOVW        R0, #32773                  @ 0x8005 
    /*00000474*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000478*/ POP         {R4, PC} 
FUNC_END JPEGE_GetChnAttr


FUNC_BEGIN JPEGE_GetStreamInfo
    /*0000047c*/ PUSH        {R4, R5, R6, LR} 
    /*00000480*/ SUBS        R5, R1, #0 
    /*00000484*/ BEQ         JPEGE_GetStreamInfo_x4dc
    /*00000488*/ CMP         R0, #15 
    /*0000048c*/ BHI         JPEGE_GetStreamInfo_x4c4
    /*00000490*/ BL          0x0000000000000490          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00000494*/ SUBS        R4, R0, #0 
    /*00000498*/ BEQ         JPEGE_GetStreamInfo_x4d0
    /*0000049c*/ ADD         R1, R4, #7744               @ 0x1e40 
    /*000004a0*/ MOV         R0, R5 
    /*000004a4*/ ADD         R1, R1, #60                 @ 0x3c 
    /*000004a8*/ MOV         R2, #12 
    /*000004ac*/ ADD         R4, R4, #4096               @ 0x1000 
    /*000004b0*/ BL          0x00000000000004B0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000004b4*/ MOV         R3, #0 
    /*000004b8*/ MOV         R0, R3 
    /*000004bc*/ STR         R3, [R4, #3708]             @ 0xe7c 
    /*000004c0*/ POP         {R4, R5, R6, PC} 
    JPEGE_GetStreamInfo_x4c4:
    /*000004c4*/ MOVW        R0, #32770                  @ 0x8002 
    /*000004c8*/ MOVT        R0, #40971                  @ 0xa00b 
    /*000004cc*/ POP         {R4, R5, R6, PC} 
    JPEGE_GetStreamInfo_x4d0:
    /*000004d0*/ MOVW        R0, #32773                  @ 0x8005 
    /*000004d4*/ MOVT        R0, #40971                  @ 0xa00b 
    /*000004d8*/ POP         {R4, R5, R6, PC} 
    JPEGE_GetStreamInfo_x4dc:
    /*000004dc*/ MOVW        R0, #32774                  @ 0x8006 
    /*000004e0*/ MOVT        R0, #40971                  @ 0xa00b 
    /*000004e4*/ POP         {R4, R5, R6, PC} 
FUNC_END JPEGE_GetStreamInfo


FUNC_BEGIN JPEGE_GetStreamAdvanceInfo
    /*000004e8*/ PUSH        {R4, LR} 
    /*000004ec*/ SUBS        R4, R1, #0 
    /*000004f0*/ BEQ         JPEGE_GetStreamAdvanceInfo_x53c
    /*000004f4*/ CMP         R0, #15 
    /*000004f8*/ BHI         JPEGE_GetStreamAdvanceInfo_x524
    /*000004fc*/ BL          0x00000000000004FC          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00000500*/ CMP         R0, #0 
    /*00000504*/ BEQ         JPEGE_GetStreamAdvanceInfo_x530
    /*00000508*/ ADD         R1, R0, #7808               @ 0x1e80 
    /*0000050c*/ MOV         R2, #0 
    /*00000510*/ MOV         R0, R4 
    /*00000514*/ ADD         R1, R1, #8 
    /*00000518*/ BL          0x0000000000000518          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000051c*/ MOV         R0, #0 
    /*00000520*/ POP         {R4, PC} 
    JPEGE_GetStreamAdvanceInfo_x524:
    /*00000524*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000528*/ MOVT        R0, #40971                  @ 0xa00b 
    /*0000052c*/ POP         {R4, PC} 
    JPEGE_GetStreamAdvanceInfo_x530:
    /*00000530*/ MOVW        R0, #32773                  @ 0x8005 
    /*00000534*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000538*/ POP         {R4, PC} 
    JPEGE_GetStreamAdvanceInfo_x53c:
    /*0000053c*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000540*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000544*/ POP         {R4, PC} 
FUNC_END JPEGE_GetStreamAdvanceInfo


FUNC_BEGIN MJPEGE_GetParam
    /*00000548*/ CMP         R0, #15 
    /*0000054c*/ BHI         MJPEGE_GetParam_x584
    /*00000550*/ CMP         R1, #0 
    /*00000554*/ BEQ         MJPEGE_GetParam_x590
    /*00000558*/ PUSH        {R4, LR} 
    /*0000055c*/ MOV         R4, R1 
    /*00000560*/ BL          0x0000000000000560          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00000564*/ CMP         R0, #0 
    /*00000568*/ BEQ         MJPEGE_GetParam_x59c
    /*0000056c*/ ADD         R1, R0, #7168               @ 0x1c00 
    /*00000570*/ MOV         R2, #196                    @ 0xc4 
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ BL          0x0000000000000578          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000057c*/ MOV         R0, #0 
    /*00000580*/ POP         {R4, PC} 
    MJPEGE_GetParam_x584:
    /*00000584*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000588*/ MOVT        R0, #40971                  @ 0xa00b 
    /*0000058c*/ BX          LR 
    MJPEGE_GetParam_x590:
    /*00000590*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000594*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000598*/ BX          LR 
    MJPEGE_GetParam_x59c:
    /*0000059c*/ MOVW        R0, #32773                  @ 0x8005 
    /*000005a0*/ MOVT        R0, #40971                  @ 0xa00b 
    /*000005a4*/ POP         {R4, PC} 
FUNC_END MJPEGE_GetParam


FUNC_BEGIN JPEGE_DestroyChn
    /*000005a8*/ CMP         R0, #15 
    /*000005ac*/ BHI         JPEGE_DestroyChn_x624
    /*000005b0*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000005b4*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000005b8*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000005bc*/ MOV         R4, R0 
    /*000005c0*/ ADD         R0, R5, #72                 @ 0x48 
    /*000005c4*/ BL          0x00000000000005C4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000005c8*/ SUBS        R7, R0, #0 
    /*000005cc*/ BNE         JPEGE_DestroyChn_x630
    /*000005d0*/ MOV         R0, R4 
    /*000005d4*/ BL          0x00000000000005D4          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*000005d8*/ SUBS        R6, R0, #0 
    /*000005dc*/ BEQ         JPEGE_DestroyChn_x63c
    /*000005e0*/ ADD         R0, R6, #6912               @ 0x1b00 
    /*000005e4*/ ADD         R4, R5, R4, LSL #2 
    /*000005e8*/ ADD         R0, R0, #52                 @ 0x34 
    /*000005ec*/ BL          0x00000000000005EC          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*000005f0*/ ADD         R0, R6, #3984               @ 0xf90 
    /*000005f4*/ ADD         R0, R0, #4 
    /*000005f8*/ BL          0x00000000000005F8          @ JPEGE_DRV_Destroy [R_ARM_CALL: .text+0x6b44] 
    /*000005fc*/ LDRD        R0, [R6, #8] 
    /*00000600*/ LDR         R2, [R6] 
    /*00000604*/ BL          0x0000000000000604          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*00000608*/ MOV         R0, R6 
    /*0000060c*/ BL          0x000000000000060C          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*00000610*/ ADD         R0, R5, #72                 @ 0x48 
    /*00000614*/ STR         R7, [R4, #76]               @ 0x4c 
    /*00000618*/ BL          0x0000000000000618          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000061c*/ MOV         R0, R7 
    /*00000620*/ POP         {R4, R5, R6, R7, R8, PC} 
    JPEGE_DestroyChn_x624:
    /*00000624*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000628*/ MOVT        R0, #40971                  @ 0xa00b 
    /*0000062c*/ BX          LR 
    JPEGE_DestroyChn_x630:
    /*00000630*/ MOV         R0, #65024                  @ 0xfe00 
    /*00000634*/ MOVT        R0, #65535                  @ 0xffff 
    /*00000638*/ POP         {R4, R5, R6, R7, R8, PC} 
    JPEGE_DestroyChn_x63c:
    /*0000063c*/ MOVW        R0, #32773                  @ 0x8005 
    /*00000640*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000644*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END JPEGE_DestroyChn


FUNC_BEGIN JPEGE_Exit
    /*00000648*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*0000064c*/ MOVW        R8, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf8] 
    /*00000650*/ LDR         R4, [PC, #100]              @ 0x00000000000006bc 
    /*00000654*/ MOVW        R7, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00000658*/ LDR         R9, [PC, #96]               @ 0x00000000000006c0 
    /*0000065c*/ MOVT        R8, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf8] 
    /*00000660*/ ADD         R6, R4, #64                 @ 0x40 
    /*00000664*/ MOVT        R7, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00000668*/ MOV         R5, #0 
    /*0000066c*/ B           JPEGE_Exit_x678
    JPEGE_Exit_x670:
    /*00000670*/ CMP         R4, R6 
    /*00000674*/ BEQ         JPEGE_Exit_x6b4
    JPEGE_Exit_x678:
    /*00000678*/ LDR         R3, [R4, #4]! 
    /*0000067c*/ MOV         R0, R5 
    /*00000680*/ ADD         R5, R5, #1 
    /*00000684*/ CMP         R3, #0 
    /*00000688*/ BEQ         JPEGE_Exit_x670
    /*0000068c*/ BL          0x000000000000068C          @ JPEGE_DestroyChn [R_ARM_CALL: .text+0x5a8] 
    /*00000690*/ CMP         R0, #0 
    /*00000694*/ BEQ         JPEGE_Exit_x670
    /*00000698*/ MOV         R3, R8 
    /*0000069c*/ MOVW        R2, #749                    @ 0x2ed 
    /*000006a0*/ MOV         R1, R9 
    /*000006a4*/ MOV         R0, R7 
    /*000006a8*/ BL          0x00000000000006A8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000006ac*/ CMP         R4, R6 
    /*000006b0*/ BNE         JPEGE_Exit_x678
    JPEGE_Exit_x6b4:
    /*000006b4*/ POP         {R4, R5, R6, R7, R8, R9, SL, LR} 
    JPEGE_Exit_x6b8:
    /*000006b8*/ B           JPEGE_Exit_x6b8             @ JPEGE_DRV_Exit [R_ARM_JUMP24: .text+0x69b0] 
    /*000006bc*/ ANDEQ       R0, R0, R8, ASR #32 
    /*000006c0*/ ANDEQ       R0, R0, R4, LSL R0 
FUNC_END JPEGE_Exit


FUNC_BEGIN JPEGE_PutBitStream
    /*000006c4*/ CMP         R0, #15 
    /*000006c8*/ BHI         JPEGE_PutBitStream_x874
    /*000006cc*/ CMP         R1, #0 
    /*000006d0*/ BEQ         JPEGE_PutBitStream_x8c0
    /*000006d4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*000006d8*/ SUB         SP, SP, #16 
    /*000006dc*/ MOV         R4, R1 
    /*000006e0*/ BL          0x00000000000006E0          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*000006e4*/ SUBS        R6, R0, #0 
    /*000006e8*/ BEQ         JPEGE_PutBitStream_x8cc
    /*000006ec*/ ADD         R7, R6, #6912               @ 0x1b00 
    /*000006f0*/ ADD         R1, SP, #12 
    /*000006f4*/ ADD         R7, R7, #52                 @ 0x34 
    /*000006f8*/ MOV         R0, R7 
    /*000006fc*/ BL          0x00000000000006FC          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000700*/ LDR         R8, [R6, #452]              @ 0x1c4 
    /*00000704*/ LDR         R0, [R4, #8] 
    /*00000708*/ LDR         R1, [R8, #164]              @ 0xa4 
    /*0000070c*/ LDR         R2, [R8, #176]              @ 0xb0 
    /*00000710*/ LDR         R3, [R1] 
    /*00000714*/ ADD         R2, R2, R3 
    /*00000718*/ CMP         R0, R2 
    /*0000071c*/ BNE         JPEGE_PutBitStream_x820
    /*00000720*/ LDR         R2, [R4, #28] 
    /*00000724*/ CMP         R2, #9 
    /*00000728*/ MOVW        R2, #0                      @ VencBufferCache [R_ARM_MOVW_ABS_NC: +0x0] 
    /*0000072c*/ ADDEQ       R9, R6, #4096               @ 0x1000 
    /*00000730*/ MOVT        R2, #0                      @ VencBufferCache [R_ARM_MOVT_ABS: +0x0] 
    /*00000734*/ LDRNE       R5, [R4, #12] 
    /*00000738*/ ADDNE       R9, R6, #4096               @ 0x1000 
    /*0000073c*/ LDREQ       R5, [R9, #3828]             @ 0xef4 
    /*00000740*/ LDR         R2, [R2] 
    /*00000744*/ ADDNE       R5, R5, #63                 @ 0x3f 
    /*00000748*/ RSBEQ       R5, R5, #65536              @ 0x10000 
    /*0000074c*/ BICNE       R5, R5, #63                 @ 0x3f 
    /*00000750*/ ADDEQ       R5, R5, #128                @ 0x80 
    /*00000754*/ CMP         R2, #1 
    /*00000758*/ BEQ         JPEGE_PutBitStream_x7d8
    JPEGE_PutBitStream_x75c:
    /*0000075c*/ LDR         R2, [R8, #160]              @ 0xa0 
    /*00000760*/ TST         R5, #3 
    /*00000764*/ LDR         R2, [R2] 
    /*00000768*/ BNE         JPEGE_PutBitStream_x880
    /*0000076c*/ CMP         R2, R3 
    /*00000770*/ ADD         R3, R5, R3 
    /*00000774*/ BCC         JPEGE_PutBitStream_x7c4
    JPEGE_PutBitStream_x778:
    /*00000778*/ CMP         R2, R3 
    /*0000077c*/ BCC         JPEGE_PutBitStream_x898
    JPEGE_PutBitStream_x780:
    /*00000780*/ STR         R3, [R1] 
    /*00000784*/ MOV         R0, R7 
    /*00000788*/ LDR         R3, [R4, #28] 
    /*0000078c*/ ADD         R1, SP, #12 
    /*00000790*/ CMP         R3, #10 
    /*00000794*/ STREQ       R5, [R9, #3828]             @ 0xef4 
    /*00000798*/ MOVNE       R3, #0 
    /*0000079c*/ STRNE       R3, [R9, #3828]             @ 0xef4 
    /*000007a0*/ LDR         R3, [R4, #24] 
    /*000007a4*/ CMP         R3, #0 
    /*000007a8*/ LDRNE       R3, [R6, #3980]             @ 0xf8c 
    /*000007ac*/ SUBNE       R3, R3, #1 
    /*000007b0*/ STRNE       R3, [R6, #3980]             @ 0xf8c 
    /*000007b4*/ BL          0x00000000000007B4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000007b8*/ MOV         R0, #0 
    JPEGE_PutBitStream_x7bc:
    /*000007bc*/ ADD         SP, SP, #16 
    /*000007c0*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    JPEGE_PutBitStream_x7c4:
    /*000007c4*/ LDR         R0, [R8, #8] 
    /*000007c8*/ CMP         R3, R0 
    /*000007cc*/ BCC         JPEGE_PutBitStream_x780
    /*000007d0*/ SUB         R3, R3, R0 
    /*000007d4*/ B           JPEGE_PutBitStream_x778
    JPEGE_PutBitStream_x7d8:
    /*000007d8*/ LDR         R3, [R8, #64]               @ 0x40 
    /*000007dc*/ LDR         R2, [R8, #8] 
    /*000007e0*/ ADD         IP, R5, R3 
    /*000007e4*/ LDR         R1, [R6, #8] 
    /*000007e8*/ CMP         IP, R2 
    /*000007ec*/ ADD         R1, R3, R1 
    /*000007f0*/ BLS         JPEGE_PutBitStream_x860
    /*000007f4*/ SUB         R2, R2, R3 
    /*000007f8*/ SUB         SL, R5, R2 
    /*000007fc*/ BL          0x00000000000007FC          @ hil_mmb_invalid_cache_byaddr [R_ARM_CALL: +0x0] 
    /*00000800*/ LDR         R3, [R6, #452]              @ 0x1c4 
    /*00000804*/ LDR         R1, [R6, #8] 
    /*00000808*/ MOV         R2, SL 
    /*0000080c*/ LDR         R0, [R3, #176]              @ 0xb0 
    /*00000810*/ BL          0x0000000000000810          @ hil_mmb_invalid_cache_byaddr [R_ARM_CALL: +0x0] 
    /*00000814*/ LDR         R1, [R8, #164]              @ 0xa4 
    /*00000818*/ LDR         R3, [R1] 
    /*0000081c*/ B           JPEGE_PutBitStream_x75c
    JPEGE_PutBitStream_x820:
    /*00000820*/ LDR         R3, [PC, #176]              @ 0x00000000000008d8 
    /*00000824*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x150] 
    /*00000828*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x150] 
    /*0000082c*/ MOVW        R1, #1933                   @ 0x78d 
    /*00000830*/ MOV         R0, #3 
    /*00000834*/ STR         R1, [SP] 
    /*00000838*/ MOV         R1, #11 
    /*0000083c*/ STR         R3, [SP, #4] 
    JPEGE_PutBitStream_x840:
    /*00000840*/ BL          0x0000000000000840          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000844*/ MOV         R0, R7 
    /*00000848*/ ADD         R1, SP, #12 
    /*0000084c*/ BL          0x000000000000084C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000850*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000854*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000858*/ ADD         SP, SP, #16 
    /*0000085c*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    JPEGE_PutBitStream_x860:
    /*00000860*/ MOV         R2, R5 
    /*00000864*/ BL          0x0000000000000864          @ hil_mmb_invalid_cache_byaddr [R_ARM_CALL: +0x0] 
    /*00000868*/ LDR         R1, [R8, #164]              @ 0xa4 
    /*0000086c*/ LDR         R3, [R1] 
    /*00000870*/ B           JPEGE_PutBitStream_x75c
    JPEGE_PutBitStream_x874:
    /*00000874*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000878*/ MOVT        R0, #40971                  @ 0xa00b 
    /*0000087c*/ BX          LR 
    JPEGE_PutBitStream_x880:
    /*00000880*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x194] 
    /*00000884*/ LDR         R3, [PC, #80]               @ 0x00000000000008dc 
    /*00000888*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x194] 
    /*0000088c*/ MOV         R1, #18 
    /*00000890*/ MOV         R0, #3 
    /*00000894*/ BL          0x0000000000000894          @ HI_LOG [R_ARM_CALL: +0x0] 
    JPEGE_PutBitStream_x898:
    /*00000898*/ LDR         R3, [PC, #56]               @ 0x00000000000008d8 
    /*0000089c*/ MOV         R2, #0 
    /*000008a0*/ MOVW        R1, #1959                   @ 0x7a7 
    /*000008a4*/ STR         R2, [R9, #3828]             @ 0xef4 
    /*000008a8*/ MOV         R0, R2 
    /*000008ac*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c4] 
    /*000008b0*/ STM         SP, {R1, R3} 
    /*000008b4*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c4] 
    /*000008b8*/ MOV         R1, #11 
    /*000008bc*/ B           JPEGE_PutBitStream_x840
    JPEGE_PutBitStream_x8c0:
    /*000008c0*/ MOVW        R0, #32774                  @ 0x8006 
    /*000008c4*/ MOVT        R0, #40971                  @ 0xa00b 
    /*000008c8*/ BX          LR 
    JPEGE_PutBitStream_x8cc:
    /*000008cc*/ MOVW        R0, #32773                  @ 0x8005 
    /*000008d0*/ MOVT        R0, #40971                  @ 0xa00b 
    /*000008d4*/ B           JPEGE_PutBitStream_x7bc
    /*000008d8*/ ANDEQ       R0, R0, R0, LSR #32 
    /*000008dc*/ ANDEQ       R0, R0, R4, LSR R0 
FUNC_END JPEGE_PutBitStream


FUNC_BEGIN JPEGE_GetJpegStreamBufStat
    /*000008e0*/ CMP         R0, #15 
    /*000008e4*/ BHI         JPEGE_GetJpegStreamBufStat_xaf8
    /*000008e8*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000008ec*/ SUB         SP, SP, #24 
    /*000008f0*/ MOV         R7, R0 
    /*000008f4*/ MOV         R5, R1 
    /*000008f8*/ BL          0x00000000000008F8          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*000008fc*/ MOVW        R8, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000900*/ MOVT        R8, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000904*/ ADD         R3, R8, R7, LSL #2 
    /*00000908*/ LDR         R4, [R3, #76]               @ 0x4c 
    /*0000090c*/ CMP         R4, #0 
    /*00000910*/ MOV         R6, R0 
    /*00000914*/ BEQ         JPEGE_GetJpegStreamBufStat_xb04
    /*00000918*/ ADD         R4, R0, #6912               @ 0x1b00 
    /*0000091c*/ ADD         R1, SP, #20 
    /*00000920*/ ADD         R4, R4, #52                 @ 0x34 
    /*00000924*/ MOV         R0, R4 
    /*00000928*/ BL          0x0000000000000928          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000092c*/ CMP         R5, #1 
    /*00000930*/ LDR         R5, [R6, #452]              @ 0x1c4 
    /*00000934*/ BEQ         JPEGE_GetJpegStreamBufStat_xa98
    JPEGE_GetJpegStreamBufStat_x938:
    /*00000938*/ LDR         R1, [R6, #3980]             @ 0xf8c 
    /*0000093c*/ LDR         R2, [R6, #3976]             @ 0xf88 
    /*00000940*/ CMP         R1, R2 
    /*00000944*/ BCS         JPEGE_GetJpegStreamBufStat_xa4c
    /*00000948*/ LDR         R3, [R8, #64]               @ 0x40 
    /*0000094c*/ CMP         R3, #0 
    /*00000950*/ BEQ         JPEGE_GetJpegStreamBufStat_x9b4
    /*00000954*/ LDR         R3, [R6, #452]              @ 0x1c4 
    /*00000958*/ LDR         R3, [R3, #8] 
    /*0000095c*/ LSR         R3, R3, #1 
    JPEGE_GetJpegStreamBufStat_x960:
    /*00000960*/ LDR         R2, [R5, #164]              @ 0xa4 
    /*00000964*/ LDR         R0, [R5, #168]              @ 0xa8 
    /*00000968*/ LDR         R1, [R5, #12] 
    /*0000096c*/ LDR         R2, [R2] 
    /*00000970*/ LDR         R0, [R0] 
    /*00000974*/ SUB         R3, R3, R1 
    /*00000978*/ CMP         R2, R0 
    /*0000097c*/ SUBLS       R2, R2, R1 
    /*00000980*/ SUBHI       R2, R2, R0 
    /*00000984*/ LDRLS       IP, [R5, #8] 
    /*00000988*/ SUBHI       R2, R2, R1 
    /*0000098c*/ ADDLS       R2, R2, IP 
    /*00000990*/ SUBLS       R2, R2, R0 
    /*00000994*/ CMP         R3, R2 
    /*00000998*/ BHI         JPEGE_GetJpegStreamBufStat_xa10
    /*0000099c*/ MOV         R0, R4 
    /*000009a0*/ ADD         R1, SP, #20 
    /*000009a4*/ BL          0x00000000000009A4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000009a8*/ MOV         R0, #1 
    JPEGE_GetJpegStreamBufStat_x9ac:
    /*000009ac*/ ADD         SP, SP, #24 
    /*000009b0*/ POP         {R4, R5, R6, R7, R8, PC} 
    JPEGE_GetJpegStreamBufStat_x9b4:
    /*000009b4*/ LDR         R2, [R6, #36]               @ 0x24 
    /*000009b8*/ LDR         R3, [R6, #40]               @ 0x28 
    /*000009bc*/ LDR         R1, [R6, #424]              @ 0x1a8 
    /*000009c0*/ ADD         R2, R2, #15 
    /*000009c4*/ LDR         R0, [R6, #436]              @ 0x1b4 
    /*000009c8*/ ADD         R3, R3, #15 
    /*000009cc*/ BIC         R2, R2, #15 
    /*000009d0*/ BIC         R3, R3, #15 
    /*000009d4*/ LDR         R1, [R1] 
    /*000009d8*/ LDR         R0, [R0] 
    /*000009dc*/ MUL         R3, R3, R2 
    /*000009e0*/ CMP         R1, R0 
    /*000009e4*/ LDRHI       R2, [R6, #272]              @ 0x110 
    /*000009e8*/ ADDHI       R0, R0, R2 
    /*000009ec*/ MOVW        R2, #1028                   @ 0x404 
    /*000009f0*/ SUB         R1, R0, R1 
    /*000009f4*/ LSR         R3, R3, #1 
    /*000009f8*/ CMP         R1, R2 
    /*000009fc*/ ADDLS       R3, R3, R1 
    /*00000a00*/ ADDHI       R3, R3, R2 
    /*00000a04*/ CMP         R3, #4096                   @ 0x1000 
    /*00000a08*/ MOVCC       R3, #4096                   @ 0x1000 
    /*00000a0c*/ B           JPEGE_GetJpegStreamBufStat_x960
    JPEGE_GetJpegStreamBufStat_xa10:
    /*00000a10*/ LDR         R3, [PC, #280]              @ 0x0000000000000b30 
    /*00000a14*/ MOVW        R1, #3059                   @ 0xbf3 
    /*00000a18*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2e4] 
    /*00000a1c*/ STR         R1, [SP] 
    /*00000a20*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2e4] 
    /*00000a24*/ MOV         R1, #11 
    /*00000a28*/ STR         R3, [SP, #4] 
    /*00000a2c*/ MOV         R0, #7 
    /*00000a30*/ BL          0x0000000000000A30          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000a34*/ MOV         R0, R4 
    /*00000a38*/ ADD         R1, SP, #20 
    /*00000a3c*/ BL          0x0000000000000A3C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000a40*/ MOV         R0, #0 
    /*00000a44*/ ADD         SP, SP, #24 
    /*00000a48*/ POP         {R4, R5, R6, R7, R8, PC} 
    JPEGE_GetJpegStreamBufStat_xa4c:
    /*00000a4c*/ LDR         R3, [PC, #220]              @ 0x0000000000000b30 
    /*00000a50*/ MOVW        R0, #3017                   @ 0xbc9 
    /*00000a54*/ STR         R2, [SP, #12] 
    /*00000a58*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x27c] 
    /*00000a5c*/ STR         R1, [SP, #8] 
    /*00000a60*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x27c] 
    /*00000a64*/ STM         SP, {R0, R3} 
    /*00000a68*/ MOV         R1, #11 
    /*00000a6c*/ MOV         R0, #4 
    /*00000a70*/ BL          0x0000000000000A70          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000a74*/ LDR         R3, [R6, #3940]             @ 0xf64 
    /*00000a78*/ MOV         R0, R4 
    /*00000a7c*/ ADD         R1, SP, #20 
    /*00000a80*/ ADD         R3, R3, #1 
    /*00000a84*/ STR         R3, [R6, #3940]             @ 0xf64 
    /*00000a88*/ BL          0x0000000000000A88          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000a8c*/ MOV         R0, #0 
    /*00000a90*/ ADD         SP, SP, #24 
    /*00000a94*/ POP         {R4, R5, R6, R7, R8, PC} 
    JPEGE_GetJpegStreamBufStat_xa98:
    /*00000a98*/ MOVW        R3, #3002                   @ 0xbba 
    /*00000a9c*/ STR         R7, [SP, #4] 
    /*00000aa0*/ STR         R3, [SP] 
    /*00000aa4*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23c] 
    /*00000aa8*/ LDR         R3, [PC, #128]              @ 0x0000000000000b30 
    /*00000aac*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23c] 
    /*00000ab0*/ MOV         R1, #11 
    /*00000ab4*/ MOV         R0, #3 
    /*00000ab8*/ BL          0x0000000000000AB8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000abc*/ ADD         R3, R6, #4096               @ 0x1000 
    /*00000ac0*/ LDR         R2, [R3, #3764]             @ 0xeb4 
    /*00000ac4*/ CMP         R2, #0 
    /*00000ac8*/ BEQ         JPEGE_GetJpegStreamBufStat_xae0
    /*00000acc*/ LDRB        R2, [R3, #3804]             @ 0xedc 
    /*00000ad0*/ CMP         R2, #1 
    /*00000ad4*/ LDREQ       R3, [R3, #3816]             @ 0xee8 
    /*00000ad8*/ STREQ       R3, [R5, #128]              @ 0x80 
    /*00000adc*/ BEQ         JPEGE_GetJpegStreamBufStat_x938
    JPEGE_GetJpegStreamBufStat_xae0:
    /*00000ae0*/ LDR         R3, [R6, #452]              @ 0x1c4 
    /*00000ae4*/ LDR         R2, [R3, #172]              @ 0xac 
    /*00000ae8*/ LDR         R3, [R3, #168]              @ 0xa8 
    /*00000aec*/ LDR         R2, [R2] 
    /*00000af0*/ STR         R2, [R3] 
    /*00000af4*/ B           JPEGE_GetJpegStreamBufStat_x938
    JPEGE_GetJpegStreamBufStat_xaf8:
    /*00000af8*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000afc*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000b00*/ BX          LR 
    JPEGE_GetJpegStreamBufStat_xb04:
    /*00000b04*/ MOVW        R3, #2989                   @ 0xbad 
    /*00000b08*/ STR         R7, [SP, #4] 
    /*00000b0c*/ STR         R3, [SP] 
    /*00000b10*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x204] 
    /*00000b14*/ LDR         R3, [PC, #20]               @ 0x0000000000000b30 
    /*00000b18*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x204] 
    /*00000b1c*/ MOV         R1, #11 
    /*00000b20*/ MOV         R0, #3 
    /*00000b24*/ BL          0x0000000000000B24          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000b28*/ MOV         R0, R4 
    /*00000b2c*/ B           JPEGE_GetJpegStreamBufStat_x9ac
    /*00000b30*/ ANDEQ       R0, R0, R8, ASR #32 
FUNC_END JPEGE_GetJpegStreamBufStat


FUNC_BEGIN JPEGE_ConfigRegs
    /*00000b34*/ CMP         R0, #15 
    /*00000b38*/ BHI         JPEGE_ConfigRegs_xbb4
    /*00000b3c*/ PUSH        {R4, R5, R6, LR} 
    /*00000b40*/ SUB         SP, SP, #8 
    /*00000b44*/ MOV         R5, R2 
    /*00000b48*/ BL          0x0000000000000B48          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00000b4c*/ SUBS        R6, R0, #0 
    /*00000b50*/ BEQ         JPEGE_ConfigRegs_xbc0
    /*00000b54*/ ADD         R4, R6, #6912               @ 0x1b00 
    /*00000b58*/ ADD         R1, SP, #4 
    /*00000b5c*/ ADD         R4, R4, #52                 @ 0x34 
    /*00000b60*/ MOV         R0, R4 
    /*00000b64*/ BL          0x0000000000000B64          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000b68*/ MOV         R1, R5 
    /*00000b6c*/ ADD         R0, R6, #4032               @ 0xfc0 
    /*00000b70*/ BL          0x0000000000000B70          @ JPEGE_DRV_WriteRegs [R_ARM_CALL: .text+0x71e8] 
    /*00000b74*/ CMP         R0, #0 
    /*00000b78*/ BNE         JPEGE_ConfigRegs_xb94
    JPEGE_ConfigRegs_xb7c:
    /*00000b7c*/ MOV         R0, R4 
    /*00000b80*/ ADD         R1, SP, #4 
    /*00000b84*/ BL          0x0000000000000B84          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000b88*/ MOV         R0, #0 
    JPEGE_ConfigRegs_xb8c:
    /*00000b8c*/ ADD         SP, SP, #8 
    /*00000b90*/ POP         {R4, R5, R6, PC} 
    JPEGE_ConfigRegs_xb94:
    /*00000b94*/ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x31c] 
    /*00000b98*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00000b9c*/ MOVT        R3, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x31c] 
    /*00000ba0*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00000ba4*/ MOVW        R2, #2609                   @ 0xa31 
    /*00000ba8*/ LDR         R1, [PC, #28]               @ 0x0000000000000bcc 
    /*00000bac*/ BL          0x0000000000000BAC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000bb0*/ B           JPEGE_ConfigRegs_xb7c
    JPEGE_ConfigRegs_xbb4:
    /*00000bb4*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000bb8*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000bbc*/ BX          LR 
    JPEGE_ConfigRegs_xbc0:
    /*00000bc0*/ MOVW        R0, #32773                  @ 0x8005 
    /*00000bc4*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000bc8*/ B           JPEGE_ConfigRegs_xb8c
    /*00000bcc*/ ANDEQ       R0, R0, R4, RRX 
FUNC_END JPEGE_ConfigRegs


FUNC_BEGIN JPEGE_Init
    /*00000bd0*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000bd4*/ MOV         R0, #0 
    /*00000bd8*/ SUB         SP, SP, #12 
    /*00000bdc*/ BL          0x0000000000000BDC          @ JPEGE_DRV_Init [R_ARM_CALL: .text+0x6960] 
    /*00000be0*/ CMP         R0, #0 
    /*00000be4*/ BNE         JPEGE_Init_xbf0
    JPEGE_Init_xbe8:
    /*00000be8*/ ADD         SP, SP, #12 
    /*00000bec*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    JPEGE_Init_xbf0:
    /*00000bf0*/ LDR         R3, [PC, #36]               @ 0x0000000000000c1c 
    /*00000bf4*/ MOV         R1, #254                    @ 0xfe 
    /*00000bf8*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x334] 
    /*00000bfc*/ STR         R1, [SP] 
    /*00000c00*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x334] 
    /*00000c04*/ MOV         R1, #11 
    /*00000c08*/ STR         R3, [SP, #4] 
    /*00000c0c*/ MOV         R0, #3 
    /*00000c10*/ BL          0x0000000000000C10          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000c14*/ MVN         R0, #0 
    /*00000c18*/ B           JPEGE_Init_xbe8
    /*00000c1c*/ ANDEQ       R0, R0, R8, ROR R0 
FUNC_END JPEGE_Init


FUNC_BEGIN JPEGE_ReadRegs
    /*00000c20*/ CMP         R0, #15 
    /*00000c24*/ BHI         JPEGE_ReadRegs_xcc4
    /*00000c28*/ PUSH        {R4, R5, R6, LR} 
    /*00000c2c*/ SUB         SP, SP, #32 
    /*00000c30*/ MOV         R5, R0 
    /*00000c34*/ MOV         R4, R2 
    /*00000c38*/ BL          0x0000000000000C38          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00000c3c*/ SUBS        R6, R0, #0 
    /*00000c40*/ BEQ         JPEGE_ReadRegs_xcf0
    /*00000c44*/ CMP         R4, #0 
    /*00000c48*/ BEQ         JPEGE_ReadRegs_xcd0
    JPEGE_ReadRegs_xc4c:
    /*00000c4c*/ ADD         R0, R6, #4000               @ 0xfa0 
    /*00000c50*/ MOV         R1, R4 
    /*00000c54*/ ADD         R0, R0, #4 
    /*00000c58*/ BL          0x0000000000000C58          @ JPEGE_DRV_ReadRegs [R_ARM_CALL: .text+0x6be8] 
    /*00000c5c*/ LDR         R3, [R6, #16] 
    /*00000c60*/ CMP         R3, #1 
    /*00000c64*/ BEQ         JPEGE_ReadRegs_xc74
    JPEGE_ReadRegs_xc68:
    /*00000c68*/ MOV         R0, #0 
    JPEGE_ReadRegs_xc6c:
    /*00000c6c*/ ADD         SP, SP, #32 
    /*00000c70*/ POP         {R4, R5, R6, PC} 
    JPEGE_ReadRegs_xc74:
    /*00000c74*/ MOV         R0, #19 
    /*00000c78*/ BL          0x0000000000000C78          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000c7c*/ CMP         R0, #0 
    /*00000c80*/ BEQ         JPEGE_ReadRegs_xcfc
    /*00000c84*/ LDR         R6, [R0, #24] 
    /*00000c88*/ MOV         R1, R4 
    /*00000c8c*/ MOV         R0, R5 
    /*00000c90*/ MOV         R3, #0 
    /*00000c94*/ MOV         R2, SP 
    /*00000c98*/ BLX         R6 
    /*00000c9c*/ CMP         R0, #0 
    /*00000ca0*/ BEQ         JPEGE_ReadRegs_xc68
    /*00000ca4*/ MOVW        R3, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf8] 
    /*00000ca8*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00000cac*/ MOVT        R3, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf8] 
    /*00000cb0*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00000cb4*/ MOVW        R2, #2655                   @ 0xa5f 
    /*00000cb8*/ LDR         R1, [PC, #72]               @ 0x0000000000000d08 
    /*00000cbc*/ BL          0x0000000000000CBC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000cc0*/ B           JPEGE_ReadRegs_xc68
    JPEGE_ReadRegs_xcc4:
    /*00000cc4*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000cc8*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000ccc*/ BX          LR 
    JPEGE_ReadRegs_xcd0:
    /*00000cd0*/ MOVW        R3, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x36c] 
    /*00000cd4*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00000cd8*/ MOVT        R3, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x36c] 
    /*00000cdc*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00000ce0*/ MOVW        R2, #2645                   @ 0xa55 
    /*00000ce4*/ LDR         R1, [PC, #28]               @ 0x0000000000000d08 
    /*00000ce8*/ BL          0x0000000000000CE8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000cec*/ B           JPEGE_ReadRegs_xc4c
    JPEGE_ReadRegs_xcf0:
    /*00000cf0*/ MOVW        R0, #32773                  @ 0x8005 
    /*00000cf4*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000cf8*/ B           JPEGE_ReadRegs_xc6c
    JPEGE_ReadRegs_xcfc:
    /*00000cfc*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000d00*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000d04*/ B           JPEGE_ReadRegs_xc6c
    /*00000d08*/ ANDEQ       R0, R0, R4, LSL #1 
FUNC_END JPEGE_ReadRegs


FUNC_BEGIN JPEGE_InsertUserData
    /*00000d0c*/ CMP         R0, #15 
    /*00000d10*/ BHI         JPEGE_InsertUserData_xf44
    /*00000d14*/ CMP         R1, #0 
    /*00000d18*/ BEQ         JPEGE_InsertUserData_xfb0
    /*00000d1c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000d20*/ SUB         SP, SP, #92                 @ 0x5c 
    /*00000d24*/ MOV         R7, R0 
    /*00000d28*/ MOV         R6, R2 
    /*00000d2c*/ MOV         R5, R1 
    /*00000d30*/ BL          0x0000000000000D30          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00000d34*/ SUBS        R4, R0, #0 
    /*00000d38*/ BEQ         JPEGE_InsertUserData_x1050
    /*00000d3c*/ SUB         R3, R6, #1 
    /*00000d40*/ CMP         R3, #1024                   @ 0x400 
    /*00000d44*/ BCS         JPEGE_InsertUserData_xfbc
    /*00000d48*/ LDR         R2, [R4, #428]              @ 0x1ac 
    /*00000d4c*/ ADD         R3, R6, #63                 @ 0x3f 
    /*00000d50*/ LDR         R1, [R4, #432]              @ 0x1b0 
    /*00000d54*/ BIC         R3, R3, #63                 @ 0x3f 
    /*00000d58*/ LDR         R0, [R4, #448]              @ 0x1c0 
    /*00000d5c*/ LDR         R2, [R2] 
    /*00000d60*/ LDR         R1, [R1] 
    /*00000d64*/ CMP         R2, R1 
    /*00000d68*/ SUBHI       R2, R2, R1 
    /*00000d6c*/ LDRLS       LR, [R4, #276]              @ 0x114 
    /*00000d70*/ LDRLS       IP, [R4, #272]              @ 0x110 
    /*00000d74*/ SUBLS       R2, R2, LR 
    /*00000d78*/ LDRHI       IP, [R4, #276]              @ 0x114 
    /*00000d7c*/ ADDLS       R2, R2, IP 
    /*00000d80*/ SUBLS       R2, R2, R1 
    /*00000d84*/ SUBHI       R2, R2, IP 
    /*00000d88*/ CMP         R0, #0 
    /*00000d8c*/ BEQ         JPEGE_InsertUserData_x105c
    /*00000d90*/ ADD         R1, R3, #64                 @ 0x40 
    /*00000d94*/ CMP         R1, R2 
    /*00000d98*/ BHI         JPEGE_InsertUserData_xf50
    /*00000d9c*/ ADD         R8, R0, #64                 @ 0x40 
    /*00000da0*/ MOV         R2, #64                     @ 0x40 
    /*00000da4*/ STR         R1, [SP, #24] 
    /*00000da8*/ ADD         R1, SP, #24 
    /*00000dac*/ SUB         R3, R3, R6 
    /*00000db0*/ MOV         IP, #6 
    /*00000db4*/ STR         R3, [SP, #28] 
    /*00000db8*/ MOV         SL, #0 
    /*00000dbc*/ MOV         R3, #0 
    /*00000dc0*/ STRB        IP, [SP, #32] 
    /*00000dc4*/ STRB        R3, [SP, #35]               @ 0x23 
    /*00000dc8*/ MOV         IP, #1 
    /*00000dcc*/ STR         R3, [SP, #56]               @ 0x38 
    /*00000dd0*/ MOV         FP, #0 
    /*00000dd4*/ STRB        IP, [SP, #33]               @ 0x21 
    /*00000dd8*/ STR         R7, [SP, #36]               @ 0x24 
    /*00000ddc*/ STRD        SL, [SP, #40]               @ 0x28 
    /*00000de0*/ BL          0x0000000000000DE0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000de4*/ MOV         R1, R5 
    /*00000de8*/ MOV         R0, R8 
    /*00000dec*/ MOV         R2, R6 
    /*00000df0*/ BL          0x0000000000000DF0          @ osal_copy_from_user [R_ARM_CALL: +0x0] 
    /*00000df4*/ SUBS        R1, R0, #0 
    /*00000df8*/ BNE         JPEGE_InsertUserData_xf80
    /*00000dfc*/ ADD         R5, R4, #6912               @ 0x1b00 
    /*00000e00*/ ADD         R0, R8, R6 
    /*00000e04*/ ADD         R5, R5, #52                 @ 0x34 
    /*00000e08*/ LDR         R2, [SP, #28] 
    /*00000e0c*/ BL          0x0000000000000E0C          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00000e10*/ ADD         R1, SP, #20 
    /*00000e14*/ MOV         R0, R5 
    /*00000e18*/ LDR         R8, [R4, #448]              @ 0x1c0 
    /*00000e1c*/ BL          0x0000000000000E1C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000e20*/ LDR         R6, [SP, #24] 
    /*00000e24*/ LDR         R3, [R4, #428]              @ 0x1ac 
    /*00000e28*/ LDR         SL, [R4, #432]              @ 0x1b0 
    /*00000e2c*/ ANDS        R9, R6, #3 
    /*00000e30*/ LDR         R3, [R3] 
    /*00000e34*/ LDR         R1, [SL] 
    /*00000e38*/ BNE         JPEGE_InsertUserData_xff0
    /*00000e3c*/ CMP         R3, R1 
    /*00000e40*/ SUBHI       R3, R3, R1 
    /*00000e44*/ LDRLS       R0, [R4, #276]              @ 0x114 
    /*00000e48*/ LDRLS       R2, [R4, #272]              @ 0x110 
    /*00000e4c*/ SUBLS       R3, R3, R0 
    /*00000e50*/ LDRHI       R2, [R4, #276]              @ 0x114 
    /*00000e54*/ ADDLS       R3, R3, R2 
    /*00000e58*/ SUBLS       R3, R3, R1 
    /*00000e5c*/ SUBHI       R3, R3, R2 
    /*00000e60*/ CMP         R6, R3 
    /*00000e64*/ BHI         JPEGE_InsertUserData_x1038
    /*00000e68*/ LDR         R3, [R4, #272]              @ 0x110 
    /*00000e6c*/ ADD         IP, R6, R1 
    /*00000e70*/ CMP         IP, R3 
    /*00000e74*/ BCS         JPEGE_InsertUserData_xf10
    /*00000e78*/ LSR         R0, R6, #3 
    /*00000e7c*/ LDR         R3, [R4, #440]              @ 0x1b8 
    /*00000e80*/ CMP         R0, #0 
    /*00000e84*/ SUB         R0, R0, #1 
    /*00000e88*/ ADD         R1, R3, R1 
    /*00000e8c*/ ADD         LR, R1, R6 
    /*00000e90*/ BEQ         JPEGE_InsertUserData_xee4
    /*00000e94*/ MVN         R3, #0 
    JPEGE_InsertUserData_xe98:
    /*00000e98*/ SUB         R1, R1, #8 
    /*00000e9c*/ MOV         R2, R0 
    /*00000ea0*/ MOV         SL, R8 
    JPEGE_InsertUserData_xea4:
    /*00000ea4*/ SUB         R2, R2, #1 
    /*00000ea8*/ LDRD        R6, [SL], #8 
    /*00000eac*/ CMN         R2, #1 
    /*00000eb0*/ STRD        R6, [R1, #8]! 
    /*00000eb4*/ BNE         JPEGE_InsertUserData_xea4
    /*00000eb8*/ ADD         R2, R0, #1 
    /*00000ebc*/ ADD         R2, R8, R2, LSL #3 
    JPEGE_InsertUserData_xec0:
    /*00000ec0*/ CMP         R9, #0 
    /*00000ec4*/ BEQ         JPEGE_InsertUserData_xee0
    /*00000ec8*/ SUB         LR, LR, #8 
    JPEGE_InsertUserData_xecc:
    /*00000ecc*/ SUB         R3, R3, #1 
    /*00000ed0*/ LDRD        R0, [R2], #8 
    /*00000ed4*/ CMN         R3, #1 
    /*00000ed8*/ STRD        R0, [LR, #8]! 
    /*00000edc*/ BNE         JPEGE_InsertUserData_xecc
    JPEGE_InsertUserData_xee0:
    /*00000ee0*/ LDR         SL, [R4, #432]              @ 0x1b0 
    JPEGE_InsertUserData_xee4:
    /*00000ee4*/ STR         IP, [SL] 
    /*00000ee8*/ MOV         R0, R5 
    /*00000eec*/ LDR         R2, [R4, #432]              @ 0x1b0 
    /*00000ef0*/ ADD         R1, SP, #20 
    /*00000ef4*/ LDR         R3, [R4, #436]              @ 0x1b4 
    /*00000ef8*/ LDR         R2, [R2] 
    /*00000efc*/ STR         R2, [R3] 
    /*00000f00*/ BL          0x0000000000000F00          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000f04*/ MOV         R0, #0 
    JPEGE_InsertUserData_xf08:
    /*00000f08*/ ADD         SP, SP, #92                 @ 0x5c 
    /*00000f0c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JPEGE_InsertUserData_xf10:
    /*00000f10*/ SUB         R0, R3, R1 
    /*00000f14*/ LDR         LR, [R4, #440]              @ 0x1b8 
    /*00000f18*/ SUB         IP, R6, R0 
    /*00000f1c*/ LSR         R0, R0, #3 
    /*00000f20*/ ADD         R1, LR, R1 
    /*00000f24*/ CMP         R0, #0 
    /*00000f28*/ LSR         R9, IP, #3 
    /*00000f2c*/ SUB         R0, R0, #1 
    /*00000f30*/ SUBEQ       R3, R9, #1 
    /*00000f34*/ MOVEQ       R2, R8 
    /*00000f38*/ BEQ         JPEGE_InsertUserData_xec0
    /*00000f3c*/ SUB         R3, R9, #1 
    /*00000f40*/ B           JPEGE_InsertUserData_xe98
    JPEGE_InsertUserData_xf44:
    /*00000f44*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000f48*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000f4c*/ BX          LR 
    JPEGE_InsertUserData_xf50:
    /*00000f50*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x410] 
    /*00000f54*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x410] 
    /*00000f58*/ MOVW        R3, #1411                   @ 0x583 
    /*00000f5c*/ STR         R7, [SP, #4] 
    /*00000f60*/ STR         R3, [SP] 
    JPEGE_InsertUserData_xf64:
    /*00000f64*/ LDR         R3, [PC, #264]              @ 0x0000000000001074 
    /*00000f68*/ MOV         R1, #11 
    /*00000f6c*/ MOV         R0, #3 
    /*00000f70*/ BL          0x0000000000000F70          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000f74*/ MOVW        R0, #32783                  @ 0x800f 
    /*00000f78*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000f7c*/ B           JPEGE_InsertUserData_xf08
    JPEGE_InsertUserData_xf80:
    /*00000f80*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x450] 
    /*00000f84*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x450] 
    /*00000f88*/ MOV         R3, #1440                   @ 0x5a0 
    /*00000f8c*/ STR         R7, [SP, #4] 
    /*00000f90*/ STR         R3, [SP] 
    JPEGE_InsertUserData_xf94:
    /*00000f94*/ LDR         R3, [PC, #216]              @ 0x0000000000001074 
    /*00000f98*/ MOV         R1, #11 
    /*00000f9c*/ MOV         R0, #3 
    /*00000fa0*/ BL          0x0000000000000FA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000fa4*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000fa8*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000fac*/ B           JPEGE_InsertUserData_xf08
    JPEGE_InsertUserData_xfb0:
    /*00000fb0*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000fb4*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000fb8*/ BX          LR 
    JPEGE_InsertUserData_xfbc:
    /*00000fbc*/ LDR         R3, [PC, #176]              @ 0x0000000000001074 
    /*00000fc0*/ MOVW        R1, #1389                   @ 0x56d 
    /*00000fc4*/ STR         R6, [SP, #8] 
    /*00000fc8*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x384] 
    /*00000fcc*/ STR         R1, [SP] 
    /*00000fd0*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x384] 
    /*00000fd4*/ STR         R3, [SP, #4] 
    /*00000fd8*/ MOV         R1, #11 
    /*00000fdc*/ MOV         R0, #4 
    /*00000fe0*/ BL          0x0000000000000FE0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000fe4*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000fe8*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00000fec*/ B           JPEGE_InsertUserData_xf08
    JPEGE_InsertUserData_xff0:
    /*00000ff0*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x48c] 
    /*00000ff4*/ MOV         R1, #18 
    /*00000ff8*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x48c] 
    /*00000ffc*/ MOV         R0, #3 
    /*00001000*/ BL          0x0000000000001000          @ HI_LOG [R_ARM_CALL: +0x0] 
    JPEGE_InsertUserData_x1004:
    /*00001004*/ LDR         R2, [R4, #436]              @ 0x1b4 
    /*00001008*/ ADD         R1, SP, #20 
    /*0000100c*/ LDR         R3, [R4, #432]              @ 0x1b0 
    /*00001010*/ MOV         R0, R5 
    /*00001014*/ LDR         R2, [R2] 
    /*00001018*/ STR         R2, [R3] 
    /*0000101c*/ BL          0x000000000000101C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001020*/ MOVW        R3, #1457                   @ 0x5b1 
    /*00001024*/ MOVW        R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4f0] 
    /*00001028*/ STR         R7, [SP, #4] 
    /*0000102c*/ STR         R3, [SP] 
    /*00001030*/ MOVT        R2, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4f0] 
    /*00001034*/ B           JPEGE_InsertUserData_xf94
    JPEGE_InsertUserData_x1038:
    /*00001038*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4b4] 
    /*0000103c*/ MOV         R1, #18 
    /*00001040*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4b4] 
    /*00001044*/ MOV         R0, #4 
    /*00001048*/ BL          0x0000000000001048          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000104c*/ B           JPEGE_InsertUserData_x1004
    JPEGE_InsertUserData_x1050:
    /*00001050*/ MOVW        R0, #32773                  @ 0x8005 
    /*00001054*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00001058*/ B           JPEGE_InsertUserData_xf08
    JPEGE_InsertUserData_x105c:
    /*0000105c*/ MOVW        R3, #1404                   @ 0x57c 
    /*00001060*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3cc] 
    /*00001064*/ STR         R7, [SP, #4] 
    /*00001068*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3cc] 
    /*0000106c*/ STR         R3, [SP] 
    /*00001070*/ B           JPEGE_InsertUserData_xf64
    /*00001074*/ MULEQ       R0, R4, R0 
FUNC_END JPEGE_InsertUserData


FUNC_BEGIN JpegeDefaultUserParam
    /*00001078*/ PUSH        {R4, R5, R6, LR} 
    /*0000107c*/ SUBS        R4, R0, #0 
    /*00001080*/ BEQ         JpegeDefaultUserParam_x1120
    JpegeDefaultUserParam_x1084:
    /*00001084*/ LDR         R5, [R4, #16] 
    /*00001088*/ LDR         R2, [PC, #176]              @ 0x0000000000001140 
    /*0000108c*/ CMP         R5, #0 
    /*00001090*/ BEQ         JpegeDefaultUserParam_x10d8
    /*00001094*/ ADD         R3, R4, #7104               @ 0x1bc0 
    /*00001098*/ ADD         IP, R2, #64                 @ 0x40 
    /*0000109c*/ ADD         R4, R4, #4096               @ 0x1000 
    /*000010a0*/ ADD         R3, R3, #63                 @ 0x3f 
    /*000010a4*/ ADD         LR, R2, #63                 @ 0x3f 
    /*000010a8*/ MOV         R1, #17 
    /*000010ac*/ MOV         R0, #16 
    /*000010b0*/ MOV         R5, #0 
    /*000010b4*/ STR         R5, [R4, #3264]             @ 0xcc0 
    JpegeDefaultUserParam_x10b8:
    /*000010b8*/ CMP         R2, LR 
    /*000010bc*/ STRB        R0, [R3, #1]! 
    /*000010c0*/ STRB        R1, [R3, #64]               @ 0x40 
    /*000010c4*/ STRB        R1, [R3, #128]              @ 0x80 
    /*000010c8*/ POPEQ       {R4, R5, R6, PC} 
    /*000010cc*/ LDRB        R0, [R2, #1]! 
    /*000010d0*/ LDRB        R1, [IP, #1]! 
    /*000010d4*/ B           JpegeDefaultUserParam_x10b8
    JpegeDefaultUserParam_x10d8:
    /*000010d8*/ ADD         R3, R4, #6912               @ 0x1b00 
    /*000010dc*/ ADD         IP, R2, #64                 @ 0x40 
    /*000010e0*/ ADD         R4, R4, #4096               @ 0x1000 
    /*000010e4*/ ADD         R3, R3, #59                 @ 0x3b 
    /*000010e8*/ ADD         LR, R2, #63                 @ 0x3f 
    /*000010ec*/ MOV         R1, #17 
    /*000010f0*/ MOV         R0, #16 
    /*000010f4*/ STR         R5, [R4, #3068]             @ 0xbfc 
    /*000010f8*/ MOV         R5, #90                     @ 0x5a 
    /*000010fc*/ STR         R5, [R4, #2872]             @ 0xb38 
    JpegeDefaultUserParam_x1100:
    /*00001100*/ CMP         R2, LR 
    /*00001104*/ STRB        R0, [R3, #1]! 
    /*00001108*/ STRB        R1, [R3, #64]               @ 0x40 
    /*0000110c*/ STRB        R1, [R3, #128]              @ 0x80 
    /*00001110*/ POPEQ       {R4, R5, R6, PC} 
    /*00001114*/ LDRB        R0, [R2, #1]! 
    /*00001118*/ LDRB        R1, [IP, #1]! 
    /*0000111c*/ B           JpegeDefaultUserParam_x1100
    JpegeDefaultUserParam_x1120:
    /*00001120*/ MOVW        R3, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x52c] 
    /*00001124*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00001128*/ MOVT        R3, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x52c] 
    /*0000112c*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00001130*/ MOVW        R2, #271                    @ 0x10f 
    /*00001134*/ LDR         R1, [PC, #8]                @ 0x0000000000001144 
    /*00001138*/ BL          0x0000000000001138          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000113c*/ B           JpegeDefaultUserParam_x1084
    /*00001140*/ ANDEQ       R0, R0, R4, ASR #1 
    /*00001144*/ ANDEQ       R0, R0, IP, LSR #1 
FUNC_END JpegeDefaultUserParam


FUNC_BEGIN JpegeValidUserParam
    /*00001148*/ PUSH        {R4, LR} 
    /*0000114c*/ SUBS        R4, R0, #0 
    /*00001150*/ BEQ         JpegeValidUserParam_x11e4
    JpegeValidUserParam_x1154:
    /*00001154*/ LDR         R3, [R4, #16] 
    /*00001158*/ ADD         R1, R4, #4096               @ 0x1000 
    /*0000115c*/ CMP         R3, #0 
    /*00001160*/ BEQ         JpegeValidUserParam_x11a0
    /*00001164*/ ADD         R2, R4, #7104               @ 0x1bc0 
    /*00001168*/ LDR         R1, [R1, #3264]             @ 0xcc0 
    /*0000116c*/ ADD         R2, R2, #63                 @ 0x3f 
    /*00001170*/ ADD         R3, R4, #71                 @ 0x47 
    /*00001174*/ ADD         R0, R4, #135                @ 0x87 
    /*00001178*/ STR         R1, [R4, #64]               @ 0x40 
    JpegeValidUserParam_x117c:
    /*0000117c*/ LDRB        R1, [R2, #1]! 
    /*00001180*/ STRB        R1, [R3, #1]! 
    /*00001184*/ CMP         R3, R0 
    /*00001188*/ LDRB        R1, [R2, #64]               @ 0x40 
    /*0000118c*/ STRB        R1, [R3, #64]               @ 0x40 
    /*00001190*/ LDRB        R1, [R2, #128]              @ 0x80 
    /*00001194*/ STRB        R1, [R3, #128]              @ 0x80 
    /*00001198*/ BNE         JpegeValidUserParam_x117c
    /*0000119c*/ POP         {R4, PC} 
    JpegeValidUserParam_x11a0:
    /*000011a0*/ LDR         IP, [R1, #3068]             @ 0xbfc 
    /*000011a4*/ ADD         R2, R4, #6912               @ 0x1b00 
    /*000011a8*/ ADD         R2, R2, #59                 @ 0x3b 
    /*000011ac*/ ADD         R3, R4, #71                 @ 0x47 
    /*000011b0*/ ADD         R0, R4, #135                @ 0x87 
    /*000011b4*/ STR         IP, [R4, #64]               @ 0x40 
    /*000011b8*/ LDR         R1, [R1, #2872]             @ 0xb38 
    /*000011bc*/ STR         R1, [R4, #68]               @ 0x44 
    JpegeValidUserParam_x11c0:
    /*000011c0*/ LDRB        R1, [R2, #1]! 
    /*000011c4*/ STRB        R1, [R3, #1]! 
    /*000011c8*/ CMP         R3, R0 
    /*000011cc*/ LDRB        R1, [R2, #64]               @ 0x40 
    /*000011d0*/ STRB        R1, [R3, #64]               @ 0x40 
    /*000011d4*/ LDRB        R1, [R2, #128]              @ 0x80 
    /*000011d8*/ STRB        R1, [R3, #128]              @ 0x80 
    /*000011dc*/ BNE         JpegeValidUserParam_x11c0
    /*000011e0*/ POP         {R4, PC} 
    JpegeValidUserParam_x11e4:
    /*000011e4*/ MOVW        R3, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x52c] 
    /*000011e8*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*000011ec*/ MOVT        R3, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x52c] 
    /*000011f0*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*000011f4*/ MOVW        R2, #317                    @ 0x13d 
    /*000011f8*/ LDR         R1, [PC, #4]                @ 0x0000000000001204 
    /*000011fc*/ BL          0x00000000000011FC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001200*/ B           JpegeValidUserParam_x1154
    /*00001204*/ ANDEQ       R0, R0, R4, ASR #2 
FUNC_END JpegeValidUserParam


FUNC_BEGIN JPEGE_CreateChn
    /*00001208*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000120c*/ CMP         R0, #15 
    /*00001210*/ SUB         SP, SP, #108                @ 0x6c 
    /*00001214*/ MOV         R3, #0 
    /*00001218*/ STR         R3, [SP, #80]               @ 0x50 
    /*0000121c*/ STR         R3, [SP, #48]               @ 0x30 
    /*00001220*/ STR         R3, [SP, #76]               @ 0x4c 
    /*00001224*/ STR         R3, [SP, #84]               @ 0x54 
    /*00001228*/ STR         R3, [SP, #52]               @ 0x34 
    /*0000122c*/ STR         R3, [SP, #88]               @ 0x58 
    /*00001230*/ STR         R3, [SP, #92]               @ 0x5c 
    /*00001234*/ STR         R3, [SP, #96]               @ 0x60 
    /*00001238*/ STR         R3, [SP, #100]              @ 0x64 
    /*0000123c*/ BHI         JPEGE_CreateChn_x1664
    /*00001240*/ CMP         R2, R3 
    /*00001244*/ CMPNE       R1, R3 
    /*00001248*/ BEQ         JPEGE_CreateChn_x174c
    /*0000124c*/ MOV         R5, R0 
    /*00001250*/ MOV         R9, R2 
    /*00001254*/ MOV         R6, R1 
    /*00001258*/ BL          0x0000000000001258          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*0000125c*/ CMP         R0, #0 
    /*00001260*/ BNE         JPEGE_CreateChn_x1618
    /*00001264*/ MOV         R0, R6 
    /*00001268*/ BL          0x0000000000001268          @ JpegeCheckChnAttr [R_ARM_CALL: .text+0x5e18] 
    /*0000126c*/ CMP         R0, #0 
    /*00001270*/ BNE         JPEGE_CreateChn_x1758
    /*00001274*/ MOVW        R7, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001278*/ MOVT        R7, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000127c*/ LDR         R2, [R6, #12] 
    /*00001280*/ LDR         R1, [R7, #64]               @ 0x40 
    /*00001284*/ LDR         R3, [R6, #16] 
    /*00001288*/ CMP         R1, #0 
    /*0000128c*/ LDR         R4, [R6, #8] 
    /*00001290*/ ADDEQ       R1, R2, #15 
    /*00001294*/ ADDEQ       R2, R3, #15 
    /*00001298*/ BICEQ       R2, R2, #15 
    /*0000129c*/ BICEQ       R3, R1, #15 
    /*000012a0*/ MOVNE       R2, #32768                  @ 0x8000 
    /*000012a4*/ MULEQ       R2, R2, R3 
    /*000012a8*/ BICEQ       R2, R2, #-2147483648        @ 0x80000000 
    /*000012ac*/ CMP         R4, R2 
    /*000012b0*/ BCC         JPEGE_CreateChn_x1788
    /*000012b4*/ ANDS        R3, R4, #63                 @ 0x3f 
    /*000012b8*/ BNE         JPEGE_CreateChn_x1688
    /*000012bc*/ MOV         R0, #2 
    /*000012c0*/ STR         R3, [SP, #80]               @ 0x50 
    /*000012c4*/ STR         R5, [SP, #84]               @ 0x54 
    /*000012c8*/ MOV         R3, #8 
    /*000012cc*/ STR         R3, [SP, #76]               @ 0x4c 
    /*000012d0*/ BL          0x00000000000012D0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000012d4*/ ADD         R1, SP, #52                 @ 0x34 
    /*000012d8*/ ADD         R4, R4, #4080               @ 0xff0 
    /*000012dc*/ ADD         R4, R4, #15 
    /*000012e0*/ BIC         R4, R4, #4080               @ 0xff0 
    /*000012e4*/ BIC         R4, R4, #15 
    /*000012e8*/ ADD         R8, R4, #5696               @ 0x1640 
    /*000012ec*/ ADD         R8, R8, #56                 @ 0x38 
    /*000012f0*/ LDR         R3, [R0, #60]               @ 0x3c 
    /*000012f4*/ ADD         R0, SP, #76                 @ 0x4c 
    /*000012f8*/ BLX         R3 
    /*000012fc*/ CMP         R0, #0 
    /*00001300*/ BNE         JPEGE_CreateChn_x16bc
    /*00001304*/ MOV         R3, R5 
    /*00001308*/ MOV         R1, #16 
    /*0000130c*/ MOVW        R2, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x670] 
    /*00001310*/ ADD         R0, SP, #88                 @ 0x58 
    /*00001314*/ MOVT        R2, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x670] 
    /*00001318*/ BL          0x0000000000001318          @ osal_snprintf [R_ARM_CALL: +0x0] 
    /*0000131c*/ LDR         R0, [SP, #52]               @ 0x34 
    /*00001320*/ STR         R8, [SP] 
    /*00001324*/ ADD         R1, SP, #88                 @ 0x58 
    /*00001328*/ ADD         R3, SP, #48                 @ 0x30 
    /*0000132c*/ ADD         R2, SP, #56                 @ 0x38 
    /*00001330*/ BL          0x0000000000001330          @ CMPI_MmzMallocNocache [R_ARM_CALL: +0x0] 
    /*00001334*/ CMP         R0, #0 
    /*00001338*/ BNE         JPEGE_CreateChn_x17bc
    /*0000133c*/ LDR         IP, [SP, #48]               @ 0x30 
    /*00001340*/ MOV         R1, #0 
    /*00001344*/ LDRD        R2, [SP, #56]               @ 0x38 
    /*00001348*/ LDRD        SL, [SP, #56]               @ 0x38 
    /*0000134c*/ MOV         R0, IP 
    /*00001350*/ STRD        R0, [SP, #24] 
    /*00001354*/ ADDS        R0, IP, #63                 @ 0x3f 
    /*00001358*/ ADDS        R2, R2, #63                 @ 0x3f 
    /*0000135c*/ BIC         R1, R0, #63                 @ 0x3f 
    /*00001360*/ ADC         R3, R3, #0 
    /*00001364*/ BIC         R0, R2, #63                 @ 0x3f 
    /*00001368*/ STR         R1, [SP, #40]               @ 0x28 
    /*0000136c*/ MOV         LR, R1 
    /*00001370*/ MOV         R1, R3 
    /*00001374*/ MOV         R3, #0 
    /*00001378*/ STRD        R0, [SP, #16] 
    /*0000137c*/ MOV         R1, R3 
    /*00001380*/ LDRD        R2, [SP, #16] 
    /*00001384*/ MOV         R0, LR 
    /*00001388*/ STRD        R0, [SP, #32] 
    /*0000138c*/ SUBS        SL, R2, SL 
    /*00001390*/ SBC         FP, R3, FP 
    /*00001394*/ MOV         R2, SL 
    /*00001398*/ MOV         R3, FP 
    /*0000139c*/ LDRD        SL, [SP, #24] 
    /*000013a0*/ SUBS        SL, LR, SL 
    /*000013a4*/ SBC         FP, R1, FP 
    /*000013a8*/ CMP         R3, FP 
    /*000013ac*/ CMPEQ       R2, SL 
    /*000013b0*/ BEQ         JPEGE_CreateChn_x13d4
    /*000013b4*/ MOVW        R3, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6c4] 
    /*000013b8*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*000013bc*/ MOVT        R3, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6c4] 
    /*000013c0*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*000013c4*/ MOVW        R2, #561                    @ 0x231 
    /*000013c8*/ LDR         R1, [PC, #1120]             @ 0x0000000000001830 
    /*000013cc*/ BL          0x00000000000013CC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000013d0*/ LDR         IP, [SP, #48]               @ 0x30 
    JPEGE_CreateChn_x13d4:
    /*000013d4*/ LDR         R3, [SP, #40]               @ 0x28 
    /*000013d8*/ ADD         SL, R3, R4 
    /*000013dc*/ ADD         R3, SL, #5632               @ 0x1600 
    /*000013e0*/ ADD         R2, SL, #4544               @ 0x11c0 
    /*000013e4*/ ADD         R3, R3, #56                 @ 0x38 
    /*000013e8*/ ADD         R2, R2, #56                 @ 0x38 
    /*000013ec*/ SUB         IP, R3, IP 
    /*000013f0*/ STR         R2, [SP, #44]               @ 0x2c 
    /*000013f4*/ CMP         R8, IP 
    /*000013f8*/ ADD         R3, SL, #184                @ 0xb8 
    /*000013fc*/ STR         R3, [SP, #24] 
    /*00001400*/ BCC         JPEGE_CreateChn_x16ec
    /*00001404*/ MOVW        R0, #7928                   @ 0x1ef8 
    /*00001408*/ BL          0x0000000000001408          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*0000140c*/ SUBS        FP, R0, #0 
    /*00001410*/ BEQ         JPEGE_CreateChn_x17f0
    /*00001414*/ MOVW        R2, #7928                   @ 0x1ef8 
    /*00001418*/ MOV         R1, #0 
    /*0000141c*/ BL          0x000000000000141C          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00001420*/ LDR         R3, [SP, #48]               @ 0x30 
    /*00001424*/ CMP         R4, #63                     @ 0x3f 
    /*00001428*/ STR         SL, [FP, #452]              @ 0x1c4 
    /*0000142c*/ ADD         LR, SL, #32 
    /*00001430*/ ADD         IP, SL, #64                 @ 0x40 
    /*00001434*/ STR         R3, [FP] 
    /*00001438*/ ADD         R2, SL, #128                @ 0x80 
    /*0000143c*/ LDRD        R0, [SP, #56]               @ 0x38 
    /*00001440*/ ADD         R3, SL, #96                 @ 0x60 
    /*00001444*/ STRD        R0, [FP, #8] 
    /*00001448*/ BLS         JPEGE_CreateChn_x1670
    /*0000144c*/ LDR         R8, [SP, #40]               @ 0x28 
    /*00001450*/ MOV         R0, #0 
    /*00001454*/ MOV         R1, #0 
    /*00001458*/ STR         R8, [SL, #176]              @ 0xb0 
    /*0000145c*/ STRD        R0, [R8, R4] 
    /*00001460*/ MOV         R1, #64                     @ 0x40 
    /*00001464*/ STR         LR, [SL, #160]              @ 0xa0 
    /*00001468*/ STR         IP, [SL, #164]              @ 0xa4 
    /*0000146c*/ STR         R2, [SL, #168]              @ 0xa8 
    /*00001470*/ STR         R3, [SL, #172]              @ 0xac 
    /*00001474*/ STR         R4, [SL, #8] 
    /*00001478*/ STR         R1, [SL, #12] 
    /*0000147c*/ STR         R0, [SL, #32] 
    /*00001480*/ STR         R0, [SL, #64]               @ 0x40 
    /*00001484*/ STR         R0, [SL, #128]              @ 0x80 
    /*00001488*/ STR         R0, [SL, #96]               @ 0x60 
    JPEGE_CreateChn_x148c:
    /*0000148c*/ LDR         R2, [FP, #452]              @ 0x1c4 
    /*00001490*/ MOVW        R3, #0                      @ VencBufferCache [R_ARM_MOVW_ABS_NC: +0x0] 
    /*00001494*/ LDRD        R0, [SP, #16] 
    /*00001498*/ MOVT        R3, #0                      @ VencBufferCache [R_ARM_MOVT_ABS: +0x0] 
    /*0000149c*/ STRD        R0, [R2] 
    /*000014a0*/ LDR         R3, [R3] 
    /*000014a4*/ CMP         R3, #1 
    /*000014a8*/ BEQ         JPEGE_CreateChn_x1650
    JPEGE_CreateChn_x14ac:
    /*000014ac*/ LDR         R3, [SP, #24] 
    /*000014b0*/ ADD         R2, FP, #272                @ 0x110 
    /*000014b4*/ MOV         R0, #0 
    /*000014b8*/ MOV         R1, #0 
    /*000014bc*/ MOV         R8, #0 
    /*000014c0*/ ADD         SL, FP, #4096               @ 0x1000 
    /*000014c4*/ STR         R3, [FP, #440]              @ 0x1b8 
    /*000014c8*/ ADD         R3, FP, #296                @ 0x128 
    /*000014cc*/ STRD        R0, [R2, #-8] 
    /*000014d0*/ ADD         R2, FP, #328                @ 0x148 
    /*000014d4*/ LDR         R1, [SP, #44]               @ 0x2c 
    /*000014d8*/ ADD         R0, FP, #392                @ 0x188 
    /*000014dc*/ STR         R3, [FP, #424]              @ 0x1a8 
    /*000014e0*/ MOV         R3, #64                     @ 0x40 
    /*000014e4*/ STR         R2, [FP, #428]              @ 0x1ac 
    /*000014e8*/ MOV         R2, #4416                   @ 0x1140 
    /*000014ec*/ STR         R1, [FP, #448]              @ 0x1c0 
    /*000014f0*/ ADD         R1, FP, #360                @ 0x168 
    /*000014f4*/ STR         R0, [FP, #432]              @ 0x1b0 
    /*000014f8*/ ADD         R0, FP, #16 
    /*000014fc*/ STR         R1, [FP, #436]              @ 0x1b4 
    /*00001500*/ MOV         R1, R6 
    /*00001504*/ STR         R2, [FP, #272]              @ 0x110 
    /*00001508*/ MOV         R2, #248                    @ 0xf8 
    /*0000150c*/ STR         R3, [FP, #276]              @ 0x114 
    /*00001510*/ STR         R8, [FP, #296]              @ 0x128 
    /*00001514*/ STR         R8, [FP, #328]              @ 0x148 
    /*00001518*/ STR         R8, [FP, #392]              @ 0x188 
    /*0000151c*/ STR         R8, [FP, #360]              @ 0x168 
    /*00001520*/ BL          0x0000000000001520          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00001524*/ LDR         R3, [FP, #16] 
    /*00001528*/ MOV         R0, FP 
    /*0000152c*/ CMP         R3, R8 
    /*00001530*/ MOV         R3, #1 
    /*00001534*/ STRNE       R8, [SL, #3700]             @ 0xe74 
    /*00001538*/ STRNE       R8, [SL, #3704]             @ 0xe78 
    /*0000153c*/ MOV         R8, #0 
    /*00001540*/ STR         R3, [SL, #3824]             @ 0xef0 
    /*00001544*/ STR         R3, [SP, #40]               @ 0x28 
    /*00001548*/ BL          0x0000000000001548          @ JpegeDefaultUserParam [R_ARM_CALL: .text+0x1078] 
    /*0000154c*/ ADD         R0, FP, #6912               @ 0x1b00 
    /*00001550*/ ADD         R0, R0, #52                 @ 0x34 
    /*00001554*/ BL          0x0000000000001554          @ osal_spin_lock_init [R_ARM_CALL: +0x0] 
    /*00001558*/ MOV         R0, FP 
    /*0000155c*/ BL          0x000000000000155C          @ JpegeValidUserParam [R_ARM_CALL: .text+0x1148] 
    /*00001560*/ MOV         R1, FP 
    /*00001564*/ MOV         R0, R5 
    /*00001568*/ BL          0x0000000000001568          @ JpegeInitPicPar [R_ARM_CALL: .text+0x47a0] 
    /*0000156c*/ MOV         R2, #200                    @ 0xc8 
    /*00001570*/ STR         R2, [FP, #3976]             @ 0xf88 
    /*00001574*/ MOVW        IP, #1002                   @ 0x3ea 
    /*00001578*/ LDRD        R2, [SP, #32] 
    /*0000157c*/ ADD         R0, FP, #3920               @ 0xf50 
    /*00001580*/ STR         R8, [FP, #3980]             @ 0xf8c 
    /*00001584*/ MOV         R1, R8 
    /*00001588*/ STR         R4, [R9, #16] 
    /*0000158c*/ ADD         R0, R0, #4 
    /*00001590*/ STRD        R2, [R9, #8] 
    /*00001594*/ LDRD        R2, [SP, #16] 
    /*00001598*/ STRD        R2, [R9] 
    /*0000159c*/ MOV         R2, #48                     @ 0x30 
    /*000015a0*/ LDR         LR, [R6] 
    /*000015a4*/ LDR         R3, [SP, #40]               @ 0x28 
    /*000015a8*/ CMP         LR, R8 
    /*000015ac*/ STR         R3, [R9, #24] 
    /*000015b0*/ MOVEQ       R3, #26 
    /*000015b4*/ MOVNE       R3, IP 
    /*000015b8*/ STR         R3, [R9, #20] 
    /*000015bc*/ BL          0x00000000000015BC          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000015c0*/ STR         R8, [FP, #3972]             @ 0xf84 
    /*000015c4*/ LDR         R0, [PC, #616]              @ 0x0000000000001834 
    /*000015c8*/ STR         R8, [SL, #3768]             @ 0xeb8 
    /*000015cc*/ STR         R5, [FP, #3984]             @ 0xf90 
    /*000015d0*/ STR         R8, [SL, #3828]             @ 0xef4 
    /*000015d4*/ BL          0x00000000000015D4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000015d8*/ SUBS        R4, R0, #0 
    /*000015dc*/ BNE         JPEGE_CreateChn_x172c
    /*000015e0*/ ADD         R3, R7, R5, LSL #2 
    /*000015e4*/ LDR         R0, [PC, #584]              @ 0x0000000000001834 
    /*000015e8*/ STR         FP, [R3, #76]               @ 0x4c 
    /*000015ec*/ BL          0x00000000000015EC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000015f0*/ ADD         R0, FP, #3984               @ 0xf90 
    /*000015f4*/ ADD         R1, SP, #68                 @ 0x44 
    /*000015f8*/ ADD         R0, R0, #4 
    /*000015fc*/ BL          0x00000000000015FC          @ JPEGE_DRV_Create [R_ARM_CALL: .text+0x6b24] 
    /*00001600*/ LDR         R3, [R7, R5, LSL #2] 
    /*00001604*/ ADD         R3, R3, #1 
    /*00001608*/ STR         R3, [R7, R5, LSL #2] 
    JPEGE_CreateChn_x160c:
    /*0000160c*/ MOV         R0, R4 
    /*00001610*/ ADD         SP, SP, #108                @ 0x6c 
    /*00001614*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JPEGE_CreateChn_x1618:
    /*00001618*/ LDR         R3, [PC, #528]              @ 0x0000000000001830 
    /*0000161c*/ MOVW        R1, #481                    @ 0x1e1 
    /*00001620*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001624*/ STR         R1, [SP] 
    /*00001628*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*0000162c*/ MOV         R1, #11 
    /*00001630*/ STR         R3, [SP, #4] 
    /*00001634*/ MOV         R0, #3 
    /*00001638*/ BL          0x0000000000001638          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000163c*/ MOVW        R4, #32772                  @ 0x8004 
    /*00001640*/ MOVT        R4, #40971                  @ 0xa00b 
    /*00001644*/ MOV         R0, R4 
    /*00001648*/ ADD         SP, SP, #108                @ 0x6c 
    /*0000164c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JPEGE_CreateChn_x1650:
    /*00001650*/ LDR         R0, [SP, #40]               @ 0x28 
    /*00001654*/ MOV         R2, R4 
    /*00001658*/ LDR         R1, [FP, #8] 
    /*0000165c*/ BL          0x000000000000165C          @ hil_mmb_invalid_cache_byaddr [R_ARM_CALL: +0x0] 
    /*00001660*/ B           JPEGE_CreateChn_x14ac
    JPEGE_CreateChn_x1664:
    /*00001664*/ MOVW        R4, #32770                  @ 0x8002 
    /*00001668*/ MOVT        R4, #40971                  @ 0xa00b 
    /*0000166c*/ B           JPEGE_CreateChn_x160c
    JPEGE_CreateChn_x1670:
    /*00001670*/ MOVW        R2, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x758] 
    /*00001674*/ MOV         R1, #18 
    /*00001678*/ MOVT        R2, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x758] 
    /*0000167c*/ MOV         R0, #3 
    /*00001680*/ BL          0x0000000000001680          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001684*/ B           JPEGE_CreateChn_x148c
    JPEGE_CreateChn_x1688:
    /*00001688*/ LDR         R3, [PC, #416]              @ 0x0000000000001830 
    /*0000168c*/ MOV         R1, #516                    @ 0x204 
    /*00001690*/ STR         R4, [SP, #8] 
    /*00001694*/ MOVW        R2, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5fc] 
    /*00001698*/ STR         R1, [SP] 
    /*0000169c*/ MOVT        R2, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5fc] 
    /*000016a0*/ STR         R3, [SP, #4] 
    /*000016a4*/ MOV         R1, #11 
    /*000016a8*/ MOV         R0, #3 
    /*000016ac*/ MOVW        R4, #32771                  @ 0x8003 
    /*000016b0*/ BL          0x00000000000016B0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000016b4*/ MOVT        R4, #40971                  @ 0xa00b 
    /*000016b8*/ B           JPEGE_CreateChn_x160c
    JPEGE_CreateChn_x16bc:
    /*000016bc*/ MOVW        R3, #543                    @ 0x21f 
    /*000016c0*/ STR         R5, [SP, #4] 
    /*000016c4*/ STR         R3, [SP] 
    /*000016c8*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x638] 
    /*000016cc*/ LDR         R3, [PC, #348]              @ 0x0000000000001830 
    /*000016d0*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x638] 
    /*000016d4*/ MOV         R1, #11 
    /*000016d8*/ MOV         R0, #3 
    /*000016dc*/ MOVW        R4, #32784                  @ 0x8010 
    /*000016e0*/ BL          0x00000000000016E0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000016e4*/ MOVT        R4, #40971                  @ 0xa00b 
    /*000016e8*/ B           JPEGE_CreateChn_x160c
    JPEGE_CreateChn_x16ec:
    /*000016ec*/ STR         R8, [SP, #8] 
    /*000016f0*/ MOVW        R3, #573                    @ 0x23d 
    /*000016f4*/ STR         IP, [SP, #4] 
    /*000016f8*/ MOV         R1, #11 
    /*000016fc*/ STR         R3, [SP] 
    /*00001700*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x720] 
    /*00001704*/ LDR         R3, [PC, #292]              @ 0x0000000000001830 
    /*00001708*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x720] 
    /*0000170c*/ MOV         R0, #3 
    /*00001710*/ MOVW        R4, #32780                  @ 0x800c 
    /*00001714*/ BL          0x0000000000001714          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001718*/ LDR         R2, [SP, #48]               @ 0x30 
    /*0000171c*/ LDRD        R0, [SP, #56]               @ 0x38 
    /*00001720*/ MOVT        R4, #40971                  @ 0xa00b 
    /*00001724*/ BL          0x0000000000001724          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*00001728*/ B           JPEGE_CreateChn_x160c
    JPEGE_CreateChn_x172c:
    /*0000172c*/ LDRD        R0, [FP, #8] 
    /*00001730*/ MOV         R4, #65024                  @ 0xfe00 
    /*00001734*/ LDR         R2, [FP] 
    /*00001738*/ MOVT        R4, #65535                  @ 0xffff 
    /*0000173c*/ BL          0x000000000000173C          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*00001740*/ MOV         R0, FP 
    /*00001744*/ BL          0x0000000000001744          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*00001748*/ B           JPEGE_CreateChn_x160c
    JPEGE_CreateChn_x174c:
    /*0000174c*/ MOVW        R4, #32774                  @ 0x8006 
    /*00001750*/ MOVT        R4, #40971                  @ 0xa00b 
    /*00001754*/ B           JPEGE_CreateChn_x160c
    JPEGE_CreateChn_x1758:
    /*00001758*/ LDR         R3, [PC, #208]              @ 0x0000000000001830 
    /*0000175c*/ MOVW        R1, #489                    @ 0x1e9 
    /*00001760*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x584] 
    /*00001764*/ STR         R1, [SP] 
    /*00001768*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x584] 
    /*0000176c*/ MOV         R1, #11 
    /*00001770*/ STR         R3, [SP, #4] 
    /*00001774*/ MOV         R0, #3 
    /*00001778*/ MOVW        R4, #32771                  @ 0x8003 
    /*0000177c*/ BL          0x000000000000177C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001780*/ MOVT        R4, #40971                  @ 0xa00b 
    /*00001784*/ B           JPEGE_CreateChn_x160c
    JPEGE_CreateChn_x1788:
    /*00001788*/ LDR         R3, [PC, #160]              @ 0x0000000000001830 
    /*0000178c*/ MOV         R1, #508                    @ 0x1fc 
    /*00001790*/ STR         R2, [SP, #12] 
    /*00001794*/ MOV         R0, #3 
    /*00001798*/ STR         R4, [SP, #8] 
    /*0000179c*/ MOVW        R2, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5b8] 
    /*000017a0*/ STM         SP, {R1, R3} 
    /*000017a4*/ MOVT        R2, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5b8] 
    /*000017a8*/ MOV         R1, #11 
    /*000017ac*/ MOVW        R4, #32771                  @ 0x8003 
    /*000017b0*/ BL          0x00000000000017B0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000017b4*/ MOVT        R4, #40971                  @ 0xa00b 
    /*000017b8*/ B           JPEGE_CreateChn_x160c
    JPEGE_CreateChn_x17bc:
    /*000017bc*/ LDR         R3, [PC, #108]              @ 0x0000000000001830 
    /*000017c0*/ MOVW        R1, #553                    @ 0x229 
    /*000017c4*/ STR         R8, [SP, #8] 
    /*000017c8*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x67c] 
    /*000017cc*/ STR         R1, [SP] 
    /*000017d0*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x67c] 
    /*000017d4*/ STR         R3, [SP, #4] 
    /*000017d8*/ MOV         R1, #11 
    /*000017dc*/ MOV         R0, #3 
    /*000017e0*/ MOVW        R4, #32780                  @ 0x800c 
    /*000017e4*/ BL          0x00000000000017E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000017e8*/ MOVT        R4, #40971                  @ 0xa00b 
    /*000017ec*/ B           JPEGE_CreateChn_x160c
    JPEGE_CreateChn_x17f0:
    /*000017f0*/ LDR         R3, [PC, #56]               @ 0x0000000000001830 
    /*000017f4*/ MOVW        R1, #587                    @ 0x24b 
    /*000017f8*/ STR         R8, [SP, #8] 
    /*000017fc*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x67c] 
    /*00001800*/ STR         R1, [SP] 
    /*00001804*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x67c] 
    /*00001808*/ STR         R3, [SP, #4] 
    /*0000180c*/ MOV         R1, #11 
    /*00001810*/ MOV         R0, #3 
    /*00001814*/ MOVW        R4, #32780                  @ 0x800c 
    /*00001818*/ BL          0x0000000000001818          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000181c*/ LDR         R2, [SP, #48]               @ 0x30 
    /*00001820*/ LDRD        R0, [SP, #56]               @ 0x38 
    /*00001824*/ MOVT        R4, #40971                  @ 0xa00b 
    /*00001828*/ BL          0x0000000000001828          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*0000182c*/ B           JPEGE_CreateChn_x160c
    /*00001830*/ ANDEQ       R0, R0, R8, ASR R1 
    /*00001834*/ ANDEQ       R0, R0, R8, ASR #32 
FUNC_END JPEGE_CreateChn


FUNC_BEGIN JPEGE_ParamInit
    /*00001838*/ PUSH        {R4, R5, R6, LR} 
    /*0000183c*/ MOV         R6, R0 
    /*00001840*/ BL          0x0000000000001840          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00001844*/ MOV         R4, #0 
    /*00001848*/ MOV         R5, R0 
    /*0000184c*/ BL          0x000000000000184C          @ JpegeDefaultUserParam [R_ARM_CALL: .text+0x1078] 
    /*00001850*/ MOV         R0, R5 
    /*00001854*/ BL          0x0000000000001854          @ JpegeValidUserParam [R_ARM_CALL: .text+0x1148] 
    /*00001858*/ MOV         R1, R5 
    /*0000185c*/ MOV         R0, R6 
    /*00001860*/ BL          0x0000000000001860          @ JpegeInitPicPar [R_ARM_CALL: .text+0x47a0] 
    /*00001864*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00001868*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x0] 
    /*0000186c*/ ADD         R0, R5, #3920               @ 0xf50 
    /*00001870*/ MOV         R2, #200                    @ 0xc8 
    /*00001874*/ MOV         R1, R4 
    /*00001878*/ LDR         R3, [R3] 
    /*0000187c*/ ADD         R0, R0, #4 
    /*00001880*/ STR         R2, [R5, #3976]             @ 0xf88 
    /*00001884*/ MOV         R2, #48                     @ 0x30 
    /*00001888*/ CMP         R3, #1 
    /*0000188c*/ MOVEQ       R3, #0 
    /*00001890*/ STREQ       R3, [R5, #3980]             @ 0xf8c 
    /*00001894*/ ADD         R3, R5, #4096               @ 0x1000 
    /*00001898*/ STR         R4, [R3, #3760]             @ 0xeb0 
    /*0000189c*/ STR         R4, [R3, #3768]             @ 0xeb8 
    /*000018a0*/ STR         R4, [R3, #3780]             @ 0xec4 
    /*000018a4*/ STR         R4, [R3, #3784]             @ 0xec8 
    /*000018a8*/ STR         R4, [R3, #3788]             @ 0xecc 
    /*000018ac*/ STR         R4, [R3, #3792]             @ 0xed0 
    /*000018b0*/ STR         R4, [R3, #3796]             @ 0xed4 
    /*000018b4*/ STR         R4, [R3, #3772]             @ 0xebc 
    /*000018b8*/ STR         R4, [R3, #3800]             @ 0xed8 
    /*000018bc*/ BL          0x00000000000018BC          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000018c0*/ STR         R4, [R5, #3972]             @ 0xf84 
    /*000018c4*/ MOV         R0, R4 
    /*000018c8*/ STR         R6, [R5, #3984]             @ 0xf90 
    /*000018cc*/ POP         {R4, R5, R6, PC} 
FUNC_END JPEGE_ParamInit


FUNC_BEGIN JPEGE_SetChnAttr
    /*000018d0*/ CMP         R0, #15 
    /*000018d4*/ BHI         JPEGE_SetChnAttr_x19f4
    /*000018d8*/ CMP         R1, #0 
    /*000018dc*/ BEQ         JPEGE_SetChnAttr_x1a00
    /*000018e0*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*000018e4*/ SUB         SP, SP, #28 
    /*000018e8*/ MOV         R6, R0 
    /*000018ec*/ MOV         R4, R1 
    /*000018f0*/ BL          0x00000000000018F0          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*000018f4*/ SUBS        R5, R0, #0 
    /*000018f8*/ BEQ         JPEGE_SetChnAttr_x1a0c
    /*000018fc*/ ADD         R7, R5, #6912               @ 0x1b00 
    /*00001900*/ ADD         R1, SP, #20 
    /*00001904*/ ADD         R7, R7, #52                 @ 0x34 
    /*00001908*/ ADD         R8, R5, #4096               @ 0x1000 
    /*0000190c*/ ADD         R9, R5, #16 
    /*00001910*/ MOV         R0, R7 
    /*00001914*/ BL          0x0000000000001914          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001918*/ LDR         R1, [R4, #20] 
    /*0000191c*/ LDR         R0, [R4, #24] 
    /*00001920*/ ADD         R2, R1, #15 
    /*00001924*/ LDR         IP, [R8, #3068]             @ 0xbfc 
    /*00001928*/ ADD         R3, R0, #15 
    /*0000192c*/ LSR         R2, R2, #4 
    /*00001930*/ LSR         R3, R3, #4 
    /*00001934*/ MUL         R3, R3, R2 
    /*00001938*/ CMP         IP, R3, LSL #1 
    /*0000193c*/ BHI         JPEGE_SetChnAttr_x1994
    /*00001940*/ LDR         R3, [R5, #36]               @ 0x24 
    /*00001944*/ CMP         R3, R1 
    /*00001948*/ BEQ         JPEGE_SetChnAttr_x19d0
    JPEGE_SetChnAttr_x194c:
    /*0000194c*/ LDR         R0, [R8, #2864]             @ 0xb30 
    /*00001950*/ CMP         R0, #0 
    /*00001954*/ BEQ         JPEGE_SetChnAttr_x1964
    /*00001958*/ MOVW        R2, #2328                   @ 0x918 
    /*0000195c*/ MOV         R1, #0 
    /*00001960*/ BL          0x0000000000001960          @ osal_memset [R_ARM_CALL: +0x0] 
    JPEGE_SetChnAttr_x1964:
    /*00001964*/ MOV         R1, R4 
    /*00001968*/ MOV         R0, R9 
    /*0000196c*/ MOV         R2, #248                    @ 0xf8 
    /*00001970*/ BL          0x0000000000001970          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00001974*/ MOV         R0, R6 
    /*00001978*/ BL          0x0000000000001978          @ JPEGE_ParamInit [R_ARM_CALL: .text+0x1838] 
    JPEGE_SetChnAttr_x197c:
    /*0000197c*/ MOV         R0, R7 
    /*00001980*/ ADD         R1, SP, #20 
    /*00001984*/ BL          0x0000000000001984          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001988*/ MOV         R0, #0 
    JPEGE_SetChnAttr_x198c:
    /*0000198c*/ ADD         SP, SP, #28 
    /*00001990*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    JPEGE_SetChnAttr_x1994:
    /*00001994*/ MOV         R3, #840                    @ 0x348 
    /*00001998*/ STR         R0, [SP, #8] 
    /*0000199c*/ STR         R1, [SP, #4] 
    /*000019a0*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x778] 
    /*000019a4*/ STR         R3, [SP] 
    /*000019a8*/ MOV         R1, #11 
    /*000019ac*/ LDR         R3, [PC, #100]              @ 0x0000000000001a18 
    /*000019b0*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x778] 
    /*000019b4*/ STR         IP, [SP, #12] 
    /*000019b8*/ MOV         R0, #3 
    /*000019bc*/ BL          0x00000000000019BC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000019c0*/ LDR         R1, [R4, #20] 
    /*000019c4*/ LDR         R3, [R5, #36]               @ 0x24 
    /*000019c8*/ CMP         R3, R1 
    /*000019cc*/ BNE         JPEGE_SetChnAttr_x194c
    JPEGE_SetChnAttr_x19d0:
    /*000019d0*/ LDR         R3, [R5, #40]               @ 0x28 
    /*000019d4*/ LDR         R2, [R4, #24] 
    /*000019d8*/ CMP         R2, R3 
    /*000019dc*/ BNE         JPEGE_SetChnAttr_x194c
    /*000019e0*/ MOV         R1, R4 
    /*000019e4*/ MOV         R0, R9 
    /*000019e8*/ MOV         R2, #248                    @ 0xf8 
    /*000019ec*/ BL          0x00000000000019EC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000019f0*/ B           JPEGE_SetChnAttr_x197c
    JPEGE_SetChnAttr_x19f4:
    /*000019f4*/ MOVW        R0, #32770                  @ 0x8002 
    /*000019f8*/ MOVT        R0, #40971                  @ 0xa00b 
    /*000019fc*/ BX          LR 
    JPEGE_SetChnAttr_x1a00:
    /*00001a00*/ MOVW        R0, #32774                  @ 0x8006 
    /*00001a04*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00001a08*/ BX          LR 
    JPEGE_SetChnAttr_x1a0c:
    /*00001a0c*/ MOVW        R0, #32773                  @ 0x8005 
    /*00001a10*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00001a14*/ B           JPEGE_SetChnAttr_x198c
    /*00001a18*/ ANDEQ       R0, R0, R8, ROR #2 
FUNC_END JPEGE_SetChnAttr


FUNC_BEGIN JPEGE_FillStreamBuf
    /*00001a1c*/ LDR         IP, [R2] 
    /*00001a20*/ PUSH        {R4, R5, R6, LR} 
    /*00001a24*/ CMP         IP, R1 
    /*00001a28*/ LDR         R4, [SP, #16] 
    /*00001a2c*/ LDRCS       LR, [R0, #8] 
    /*00001a30*/ SUBCS       IP, IP, LR 
    /*00001a34*/ STRCS       IP, [R2] 
    /*00001a38*/ CMP         R3, #0 
    /*00001a3c*/ MOV         IP, R3 
    /*00001a40*/ BLT         JPEGE_FillStreamBuf_x1a68
    /*00001a44*/ LDR         LR, [R2] 
    /*00001a48*/ SUB         R5, R1, LR 
    /*00001a4c*/ CMP         R3, R5 
    /*00001a50*/ BGT         JPEGE_FillStreamBuf_x1a60
    /*00001a54*/ B           JPEGE_FillStreamBuf_x1a70
    JPEGE_FillStreamBuf_x1a58:
    /*00001a58*/ CMP         IP, R5 
    /*00001a5c*/ BLE         JPEGE_FillStreamBuf_x1a70
    JPEGE_FillStreamBuf_x1a60:
    /*00001a60*/ SUBS        IP, IP, #1 
    /*00001a64*/ BCS         JPEGE_FillStreamBuf_x1a58
    JPEGE_FillStreamBuf_x1a68:
    /*00001a68*/ MOV         R0, #0 
    /*00001a6c*/ POP         {R4, R5, R6, PC} 
    JPEGE_FillStreamBuf_x1a70:
    /*00001a70*/ CMP         IP, #0 
    /*00001a74*/ BLE         JPEGE_FillStreamBuf_x1a98
    /*00001a78*/ ADD         R6, R4, IP 
    JPEGE_FillStreamBuf_x1a7c:
    /*00001a7c*/ LDRB        R5, [R4], #1 
    /*00001a80*/ CMP         R6, R4 
    /*00001a84*/ STRB        R5, [LR] 
    /*00001a88*/ LDR         LR, [R2] 
    /*00001a8c*/ ADD         LR, LR, #1 
    /*00001a90*/ STR         LR, [R2] 
    /*00001a94*/ BNE         JPEGE_FillStreamBuf_x1a7c
    JPEGE_FillStreamBuf_x1a98:
    /*00001a98*/ CMP         R3, IP 
    /*00001a9c*/ BLS         JPEGE_FillStreamBuf_x1a68
    /*00001aa0*/ LDR         R0, [R0, #8] 
    /*00001aa4*/ SUBS        R3, R3, IP 
    /*00001aa8*/ SUB         R1, R1, R0 
    /*00001aac*/ STR         R1, [R2] 
    /*00001ab0*/ BEQ         JPEGE_FillStreamBuf_x1a68
    /*00001ab4*/ ADD         R3, R4, R3 
    JPEGE_FillStreamBuf_x1ab8:
    /*00001ab8*/ LDRB        R0, [R4], #1 
    /*00001abc*/ CMP         R3, R4 
    /*00001ac0*/ STRB        R0, [R1] 
    /*00001ac4*/ LDR         R1, [R2] 
    /*00001ac8*/ ADD         R1, R1, #1 
    /*00001acc*/ STR         R1, [R2] 
    /*00001ad0*/ BNE         JPEGE_FillStreamBuf_x1ab8
    /*00001ad4*/ MOV         R0, #0 
    /*00001ad8*/ POP         {R4, R5, R6, PC} 
FUNC_END JPEGE_FillStreamBuf


FUNC_BEGIN JPEGE_ProcStreamBufForDcf
    /*00001adc*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00001ae0*/ SUB         SP, SP, #68                 @ 0x44 
    /*00001ae4*/ STR         R1, [SP, #32] 
    /*00001ae8*/ BL          0x0000000000001AE8          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00001aec*/ ADD         R3, R0, #4096               @ 0x1000 
    /*00001af0*/ STR         R3, [SP, #28] 
    /*00001af4*/ STR         R0, [SP, #36]               @ 0x24 
    /*00001af8*/ MOV         R2, R3 
    /*00001afc*/ LDRB        R3, [R3, #3804]             @ 0xedc 
    /*00001b00*/ CMP         R3, #0 
    /*00001b04*/ BNE         JPEGE_ProcStreamBufForDcf_x1d18
    /*00001b08*/ LDR         R2, [R2, #2824]             @ 0xb08 
    /*00001b0c*/ LDR         FP, [R0, #452]              @ 0x1c4 
    /*00001b10*/ CMP         R2, #3 
    /*00001b14*/ LDR         R2, [SP, #32] 
    /*00001b18*/ MOVNE       R3, #1 
    /*00001b1c*/ STR         R3, [R2] 
    /*00001b20*/ LDR         R3, [SP, #28] 
    /*00001b24*/ LDR         R3, [R3, #3764]             @ 0xeb4 
    /*00001b28*/ CMP         R3, #0 
    /*00001b2c*/ BEQ         JPEGE_ProcStreamBufForDcf_x1d0c
    /*00001b30*/ LDR         R3, [FP, #160]              @ 0xa0 
    /*00001b34*/ LDR         R2, [FP, #176]              @ 0xb0 
    /*00001b38*/ LDR         R5, [FP, #8] 
    /*00001b3c*/ LDR         R3, [R3] 
    /*00001b40*/ STR         R2, [SP, #40]               @ 0x28 
    /*00001b44*/ ADD         R5, R2, R5 
    /*00001b48*/ ADD         SL, R2, R3 
    /*00001b4c*/ TST         SL, #63                     @ 0x3f 
    /*00001b50*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00001b54*/ BNE         JPEGE_ProcStreamBufForDcf_x1f1c
    JPEGE_ProcStreamBufForDcf_x1b58:
    /*00001b58*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*00001b5c*/ LDR         R3, [SP, #40]               @ 0x28 
    /*00001b60*/ LDRB        R1, [SL, #11] 
    /*00001b64*/ LDR         R3, [R3, R2] 
    /*00001b68*/ LDR         R2, [SL, #4] 
    /*00001b6c*/ SUB         R3, R3, R2 
    /*00001b70*/ ADD         R3, SL, R3 
    /*00001b74*/ STR         R3, [SP, #52]               @ 0x34 
    /*00001b78*/ CMP         R5, R3 
    /*00001b7c*/ LDRLS       R2, [FP, #8] 
    /*00001b80*/ SUBLS       R3, R3, R2 
    /*00001b84*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*00001b88*/ STRLS       R3, [SP, #52]               @ 0x34 
    /*00001b8c*/ LDR         R3, [SP, #40]               @ 0x28 
    /*00001b90*/ LDR         R2, [R3, R2] 
    /*00001b94*/ ADD         R4, SL, R2 
    /*00001b98*/ CMP         R5, R4 
    /*00001b9c*/ BLS         JPEGE_ProcStreamBufForDcf_x1d98
    JPEGE_ProcStreamBufForDcf_x1ba0:
    /*00001ba0*/ CMP         R1, #0 
    /*00001ba4*/ BNE         JPEGE_ProcStreamBufForDcf_x1da4
    /*00001ba8*/ ADD         R3, SP, #52                 @ 0x34 
    /*00001bac*/ STR         R3, [SP, #20] 
    /*00001bb0*/ ADD         R3, SP, #56                 @ 0x38 
    /*00001bb4*/ STR         R3, [SP, #24] 
    JPEGE_ProcStreamBufForDcf_x1bb8:
    /*00001bb8*/ LDR         R3, [R4] 
    /*00001bbc*/ MVN         LR, #0 
    /*00001bc0*/ LDR         R6, [R4, #4] 
    /*00001bc4*/ MOV         R0, FP 
    /*00001bc8*/ ADD         R8, R4, R3 
    /*00001bcc*/ LDRB        R9, [R4, #11] 
    /*00001bd0*/ CMP         R5, R8 
    /*00001bd4*/ SUB         R6, R3, R6 
    /*00001bd8*/ ADD         R6, R4, R6 
    /*00001bdc*/ ADD         R3, R3, R2 
    /*00001be0*/ LDRLS       R1, [FP, #8] 
    /*00001be4*/ LDR         R2, [SP, #20] 
    /*00001be8*/ SUBLS       R8, R8, R1 
    /*00001bec*/ LDR         R1, [R4, #32] 
    /*00001bf0*/ CMP         R5, R6 
    /*00001bf4*/ STR         R1, [SP, #12] 
    /*00001bf8*/ LDRB        R1, [R4, #8] 
    /*00001bfc*/ STR         R1, [SP, #16] 
    /*00001c00*/ LDRLS       R1, [FP, #8] 
    /*00001c04*/ STR         R3, [SL] 
    /*00001c08*/ LDR         R7, [SP, #52]               @ 0x34 
    /*00001c0c*/ SUBLS       R6, R6, R1 
    /*00001c10*/ CMP         R9, #1 
    /*00001c14*/ LDR         R3, [R4, #4] 
    /*00001c18*/ ADDEQ       R4, R4, #44                 @ 0x2c 
    /*00001c1c*/ ADDNE       R4, R4, #64                 @ 0x40 
    /*00001c20*/ CMP         R4, R7 
    /*00001c24*/ MOV         R1, R5 
    /*00001c28*/ STR         R3, [SL, #4] 
    /*00001c2c*/ SUBCS       R7, R4, R7 
    /*00001c30*/ LDRCC       R3, [FP, #8] 
    /*00001c34*/ STRB        LR, [SP, #56]               @ 0x38 
    /*00001c38*/ ADDCC       R3, R4, R3 
    /*00001c3c*/ SUBCC       R7, R3, R7 
    /*00001c40*/ LDR         R3, [SP, #24] 
    /*00001c44*/ SUB         IP, R7, #2 
    /*00001c48*/ STRB        IP, [SP, #59]               @ 0x3b 
    /*00001c4c*/ LSR         LR, IP, #8 
    /*00001c50*/ STR         LR, [SP, #8] 
    /*00001c54*/ LDRB        IP, [SP, #8] 
    /*00001c58*/ MVN         LR, #18 
    /*00001c5c*/ STR         R3, [SP] 
    /*00001c60*/ MOV         R3, #4 
    /*00001c64*/ STRB        LR, [SP, #57]               @ 0x39 
    /*00001c68*/ STRB        IP, [SP, #58]               @ 0x3a 
    /*00001c6c*/ BL          0x0000000000001C6C          @ JPEGE_FillStreamBuf [R_ARM_CALL: .text+0x1a1c] 
    /*00001c70*/ LDR         R3, [SP, #52]               @ 0x34 
    /*00001c74*/ CMP         R4, R3 
    /*00001c78*/ BCC         JPEGE_ProcStreamBufForDcf_x1d40
    /*00001c7c*/ MOVHI       R2, #0 
    /*00001c80*/ BLS         JPEGE_ProcStreamBufForDcf_x1c9c
    JPEGE_ProcStreamBufForDcf_x1c84:
    /*00001c84*/ STRB        R2, [R3] 
    /*00001c88*/ LDR         R3, [SP, #52]               @ 0x34 
    /*00001c8c*/ ADD         R3, R3, #1 
    /*00001c90*/ STR         R3, [SP, #52]               @ 0x34 
    /*00001c94*/ CMP         R4, R3 
    /*00001c98*/ BHI         JPEGE_ProcStreamBufForDcf_x1c84
    JPEGE_ProcStreamBufForDcf_x1c9c:
    /*00001c9c*/ CMP         R9, #1 
    /*00001ca0*/ STR         R6, [SP, #52]               @ 0x34 
    /*00001ca4*/ BEQ         JPEGE_ProcStreamBufForDcf_x1cd0
    /*00001ca8*/ LDR         R3, [SP, #12] 
    /*00001cac*/ LDR         R2, [SP, #16] 
    /*00001cb0*/ CMP         R3, #0 
    /*00001cb4*/ CMPEQ       R2, #8 
    /*00001cb8*/ BEQ         JPEGE_ProcStreamBufForDcf_x1cfc
    /*00001cbc*/ CMP         R9, #0 
    /*00001cc0*/ BNE         JPEGE_ProcStreamBufForDcf_x1da4
    /*00001cc4*/ MOV         R4, R8 
    /*00001cc8*/ LDR         R2, [SL] 
    /*00001ccc*/ B           JPEGE_ProcStreamBufForDcf_x1bb8
    JPEGE_ProcStreamBufForDcf_x1cd0:
    /*00001cd0*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00001cd4*/ MOV         R2, #20 
    /*00001cd8*/ MOV         R0, R4 
    /*00001cdc*/ ADD         R1, R3, #3872               @ 0xf20 
    /*00001ce0*/ ADD         R1, R1, #14 
    /*00001ce4*/ BL          0x0000000000001CE4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00001ce8*/ LDR         R3, [SP, #12] 
    /*00001cec*/ LDR         R2, [SP, #16] 
    /*00001cf0*/ CMP         R3, #0 
    /*00001cf4*/ CMPEQ       R2, #8 
    /*00001cf8*/ BNE         JPEGE_ProcStreamBufForDcf_x1da4
    JPEGE_ProcStreamBufForDcf_x1cfc:
    /*00001cfc*/ LDR         R2, [SP, #28] 
    /*00001d00*/ LDR         R3, [R2, #3776]             @ 0xec0 
    /*00001d04*/ ADD         R7, R3, R7 
    /*00001d08*/ STR         R7, [R2, #3776]             @ 0xec0 
    JPEGE_ProcStreamBufForDcf_x1d0c:
    /*00001d0c*/ MOV         R0, #0 
    /*00001d10*/ ADD         SP, SP, #68                 @ 0x44 
    /*00001d14*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JPEGE_ProcStreamBufForDcf_x1d18:
    /*00001d18*/ LDR         R3, [SP, #28] 
    /*00001d1c*/ MOV         R0, #0 
    /*00001d20*/ LDR         R2, [SP, #32] 
    /*00001d24*/ LDR         R3, [R3, #2824]             @ 0xb08 
    /*00001d28*/ CMP         R3, #3 
    /*00001d2c*/ MOVEQ       R3, #1 
    /*00001d30*/ MOVNE       R3, #0 
    /*00001d34*/ STR         R3, [R2] 
    /*00001d38*/ ADD         SP, SP, #68                 @ 0x44 
    /*00001d3c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JPEGE_ProcStreamBufForDcf_x1d40:
    /*00001d40*/ CMP         R5, R3 
    /*00001d44*/ MOVHI       R2, #0 
    /*00001d48*/ BLS         JPEGE_ProcStreamBufForDcf_x1d64
    JPEGE_ProcStreamBufForDcf_x1d4c:
    /*00001d4c*/ STRB        R2, [R3] 
    /*00001d50*/ LDR         R3, [SP, #52]               @ 0x34 
    /*00001d54*/ ADD         R3, R3, #1 
    /*00001d58*/ STR         R3, [SP, #52]               @ 0x34 
    /*00001d5c*/ CMP         R5, R3 
    /*00001d60*/ BHI         JPEGE_ProcStreamBufForDcf_x1d4c
    JPEGE_ProcStreamBufForDcf_x1d64:
    /*00001d64*/ LDR         R3, [FP, #8] 
    /*00001d68*/ SUB         R3, R5, R3 
    /*00001d6c*/ STR         R3, [SP, #52]               @ 0x34 
    /*00001d70*/ CMP         R4, R3 
    /*00001d74*/ MOVHI       R2, #0 
    /*00001d78*/ BLS         JPEGE_ProcStreamBufForDcf_x1c9c
    JPEGE_ProcStreamBufForDcf_x1d7c:
    /*00001d7c*/ STRB        R2, [R3] 
    /*00001d80*/ LDR         R3, [SP, #52]               @ 0x34 
    /*00001d84*/ ADD         R3, R3, #1 
    /*00001d88*/ STR         R3, [SP, #52]               @ 0x34 
    /*00001d8c*/ CMP         R4, R3 
    /*00001d90*/ BHI         JPEGE_ProcStreamBufForDcf_x1d7c
    /*00001d94*/ B           JPEGE_ProcStreamBufForDcf_x1c9c
    JPEGE_ProcStreamBufForDcf_x1d98:
    /*00001d98*/ LDR         R3, [FP, #8] 
    /*00001d9c*/ SUB         R4, R4, R3 
    /*00001da0*/ B           JPEGE_ProcStreamBufForDcf_x1ba0
    JPEGE_ProcStreamBufForDcf_x1da4:
    /*00001da4*/ LDR         R3, [SP, #32] 
    /*00001da8*/ LDR         R3, [R3] 
    /*00001dac*/ STRB        R3, [SL, #11] 
    /*00001db0*/ LDR         R3, [SP, #28] 
    /*00001db4*/ LDR         R3, [R3, #2824]             @ 0xb08 
    /*00001db8*/ CMP         R3, #3 
    /*00001dbc*/ BNE         JPEGE_ProcStreamBufForDcf_x1d0c
    /*00001dc0*/ LDR         R0, [SP, #40]               @ 0x28 
    /*00001dc4*/ ADD         R2, SL, #64                 @ 0x40 
    /*00001dc8*/ LDR         IP, [SP, #44]               @ 0x2c 
    /*00001dcc*/ CMP         R5, R2 
    /*00001dd0*/ LDR         R2, [SP, #28] 
    /*00001dd4*/ LDR         R1, [SL, #4] 
    /*00001dd8*/ LDR         R3, [R0, IP] 
    /*00001ddc*/ SUB         R3, R3, #64                 @ 0x40 
    /*00001de0*/ SUB         R3, R3, R1 
    /*00001de4*/ STR         R3, [R2, #3772]             @ 0xebc 
    /*00001de8*/ BHI         JPEGE_ProcStreamBufForDcf_x1f3c
    /*00001dec*/ LDR         R0, [SP, #40]               @ 0x28 
    /*00001df0*/ LDR         IP, [SP, #44]               @ 0x2c 
    /*00001df4*/ LDR         R1, [SL, #4] 
    /*00001df8*/ LDR         R2, [FP, #176]              @ 0xb0 
    /*00001dfc*/ LDR         R3, [R0, IP] 
    /*00001e00*/ SUB         R3, R3, #68                 @ 0x44 
    /*00001e04*/ SUB         R3, R3, R1 
    /*00001e08*/ LSR         R3, R3, #8 
    /*00001e0c*/ STRB        R3, [R2, #4] 
    /*00001e10*/ LDR         R2, [FP, #176]              @ 0xb0 
    /*00001e14*/ LDR         R3, [R0, IP] 
    /*00001e18*/ LDR         R1, [SL, #4] 
    /*00001e1c*/ SUB         R3, R3, #68                 @ 0x44 
    /*00001e20*/ SUB         R3, R3, R1 
    /*00001e24*/ STRB        R3, [R2, #5] 
    /*00001e28*/ LDR         R4, [R0, IP] 
    /*00001e2c*/ LDR         R2, [SL, #4] 
    JPEGE_ProcStreamBufForDcf_x1e30:
    /*00001e30*/ ADD         R3, SL, #812                @ 0x32c 
    /*00001e34*/ SUB         R4, R4, #1600               @ 0x640 
    /*00001e38*/ ADD         R3, R3, #2 
    /*00001e3c*/ SUB         R4, R4, R2 
    /*00001e40*/ CMP         R5, R3 
    /*00001e44*/ STR         R3, [SP, #52]               @ 0x34 
    /*00001e48*/ MOV         R0, FP 
    /*00001e4c*/ LSR         IP, R4, #24 
    /*00001e50*/ LDRLS       R2, [FP, #8] 
    /*00001e54*/ ADD         SL, SL, #1040               @ 0x410 
    /*00001e58*/ STRB        R4, [SP, #59]               @ 0x3b 
    /*00001e5c*/ ADD         SL, SL, #2 
    /*00001e60*/ SUBLS       R3, R3, R2 
    /*00001e64*/ STRLS       R3, [SP, #52]               @ 0x34 
    /*00001e68*/ ADD         R3, SP, #52                 @ 0x34 
    /*00001e6c*/ STR         R3, [SP, #20] 
    /*00001e70*/ STRB        IP, [SP, #56]               @ 0x38 
    /*00001e74*/ LSR         IP, R4, #16 
    /*00001e78*/ MOV         R2, R3 
    /*00001e7c*/ ADD         R3, SP, #56                 @ 0x38 
    /*00001e80*/ STR         R3, [SP, #24] 
    /*00001e84*/ MOV         R1, R3 
    /*00001e88*/ MOV         R3, #4 
    /*00001e8c*/ STR         R1, [SP] 
    /*00001e90*/ MOV         R1, R5 
    /*00001e94*/ STRB        IP, [SP, #57]               @ 0x39 
    /*00001e98*/ LSR         IP, R4, #8 
    /*00001e9c*/ STRB        IP, [SP, #58]               @ 0x3a 
    /*00001ea0*/ BL          0x0000000000001EA0          @ JPEGE_FillStreamBuf [R_ARM_CALL: .text+0x1a1c] 
    /*00001ea4*/ LSL         R2, R4, #13 
    /*00001ea8*/ MOVW        R3, #33205                  @ 0x81b5 
    /*00001eac*/ MOVT        R3, #6990                   @ 0x1b4e 
    /*00001eb0*/ CMP         R5, SL 
    /*00001eb4*/ STR         SL, [SP, #52]               @ 0x34 
    /*00001eb8*/ MOV         R1, R5 
    /*00001ebc*/ UMULL       R2, R3, R2, R3 
    /*00001ec0*/ MOV         R0, FP 
    /*00001ec4*/ LDR         R2, [SP, #20] 
    /*00001ec8*/ MOV         LR, #0 
    /*00001ecc*/ MOV         R4, #4 
    /*00001ed0*/ STRB        LR, [SP, #56]               @ 0x38 
    /*00001ed4*/ LSR         IP, R3, #11 
    /*00001ed8*/ LDRLS       R3, [FP, #8] 
    /*00001edc*/ STRB        IP, [SP, #59]               @ 0x3b 
    /*00001ee0*/ SUBLS       SL, SL, R3 
    /*00001ee4*/ LDR         R3, [SP, #24] 
    /*00001ee8*/ LSR         R5, IP, #16 
    /*00001eec*/ STRLS       SL, [SP, #52]               @ 0x34 
    /*00001ef0*/ STRB        LR, [SP, #60]               @ 0x3c 
    /*00001ef4*/ STR         R3, [SP] 
    /*00001ef8*/ MOV         R3, #8 
    /*00001efc*/ STRB        LR, [SP, #61]               @ 0x3d 
    /*00001f00*/ LSR         IP, IP, R3 
    /*00001f04*/ STRB        LR, [SP, #63]               @ 0x3f 
    /*00001f08*/ STRB        R5, [SP, #57]               @ 0x39 
    /*00001f0c*/ STRB        IP, [SP, #58]               @ 0x3a 
    /*00001f10*/ STRB        R4, [SP, #62]               @ 0x3e 
    /*00001f14*/ BL          0x0000000000001F14          @ JPEGE_FillStreamBuf [R_ARM_CALL: .text+0x1a1c] 
    /*00001f18*/ B           JPEGE_ProcStreamBufForDcf_x1d0c
    JPEGE_ProcStreamBufForDcf_x1f1c:
    /*00001f1c*/ MOVW        R3, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7e8] 
    /*00001f20*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00001f24*/ MOVT        R3, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7e8] 
    /*00001f28*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00001f2c*/ MOVW        R2, #1013                   @ 0x3f5 
    /*00001f30*/ LDR         R1, [PC, #36]               @ 0x0000000000001f5c 
    /*00001f34*/ BL          0x0000000000001F34          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001f38*/ B           JPEGE_ProcStreamBufForDcf_x1b58
    JPEGE_ProcStreamBufForDcf_x1f3c:
    /*00001f3c*/ LDR         R4, [R0, IP] 
    /*00001f40*/ LDR         R2, [SL, #4] 
    /*00001f44*/ SUB         R3, R4, #68                 @ 0x44 
    /*00001f48*/ SUB         R3, R3, R2 
    /*00001f4c*/ STRB        R3, [SL, #69]               @ 0x45 
    /*00001f50*/ LSR         R3, R3, #8 
    /*00001f54*/ STRB        R3, [SL, #68]               @ 0x44 
    /*00001f58*/ B           JPEGE_ProcStreamBufForDcf_x1e30
    /*00001f5c*/ ANDEQ       R0, R0, IP, ROR R1 
FUNC_END JPEGE_ProcStreamBufForDcf


FUNC_BEGIN JPEGE_ProcStreamBufForMpf
    /*00001f60*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00001f64*/ SUB         SP, SP, #24 
    /*00001f68*/ BL          0x0000000000001F68          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00001f6c*/ ADD         R1, SP, #8 
    /*00001f70*/ ADD         R9, R0, #6912               @ 0x1b00 
    /*00001f74*/ ADD         R8, R0, #4096               @ 0x1000 
    /*00001f78*/ ADD         R9, R9, #52                 @ 0x34 
    /*00001f7c*/ MOV         R4, R0 
    /*00001f80*/ MOV         R0, R9 
    /*00001f84*/ BL          0x0000000000001F84          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001f88*/ LDRB        R3, [R8, #3804]             @ 0xedc 
    /*00001f8c*/ LDR         R7, [R4, #452]              @ 0x1c4 
    /*00001f90*/ CMP         R3, #1 
    /*00001f94*/ BEQ         JPEGE_ProcStreamBufForMpf_x20ec
    JPEGE_ProcStreamBufForMpf_x1f98:
    /*00001f98*/ LDR         R3, [R7, #160]              @ 0xa0 
    /*00001f9c*/ LDR         R0, [R8, #3800]             @ 0xed8 
    /*00001fa0*/ LDR         R6, [R7, #176]              @ 0xb0 
    /*00001fa4*/ LDR         R3, [R3] 
    /*00001fa8*/ CMP         R0, #3 
    /*00001fac*/ ADD         R3, R6, R3 
    /*00001fb0*/ MOVNE       SL, #0 
    /*00001fb4*/ STREQ       R0, [R3, #36]               @ 0x24 
    /*00001fb8*/ MOVEQ       SL, R3 
    /*00001fbc*/ LDREQ       R6, [R7, #176]              @ 0xb0 
    /*00001fc0*/ LDREQ       R0, [R8, #3800]             @ 0xed8 
    JPEGE_ProcStreamBufForMpf_x1fc4:
    /*00001fc4*/ LDR         R2, [R3] 
    /*00001fc8*/ LDR         IP, [R7, #8] 
    /*00001fcc*/ ADD         R4, R3, R2 
    /*00001fd0*/ LDR         R1, [R3, #4] 
    /*00001fd4*/ ADD         R6, R6, IP 
    /*00001fd8*/ SUB         R5, R2, #64                 @ 0x40 
    /*00001fdc*/ CMP         R6, R4 
    /*00001fe0*/ SUB         R5, R5, R1 
    /*00001fe4*/ SUBLS       R4, R4, IP 
    /*00001fe8*/ CMP         R0, #3 
    /*00001fec*/ LDRB        R1, [R3, #11] 
    /*00001ff0*/ BEQ         JPEGE_ProcStreamBufForMpf_x20b8
    JPEGE_ProcStreamBufForMpf_x1ff4:
    /*00001ff4*/ CMP         R1, #0 
    /*00001ff8*/ BEQ         JPEGE_ProcStreamBufForMpf_x2004
    /*00001ffc*/ B           JPEGE_ProcStreamBufForMpf_x2308
    JPEGE_ProcStreamBufForMpf_x2000:
    /*00002000*/ MOV         R4, R2 
    JPEGE_ProcStreamBufForMpf_x2004:
    /*00002004*/ LDR         R3, [R4] 
    /*00002008*/ LDR         R1, [R4, #4] 
    /*0000200c*/ ADD         R2, R4, R3 
    /*00002010*/ SUB         R3, R3, #64                 @ 0x40 
    /*00002014*/ CMP         R6, R2 
    /*00002018*/ SUB         R3, R3, R1 
    /*0000201c*/ ADD         R5, R5, R3 
    /*00002020*/ LDRB        R3, [R4, #11] 
    /*00002024*/ LDRLS       R1, [R7, #8] 
    /*00002028*/ SUBLS       R2, R2, R1 
    /*0000202c*/ CMP         R3, #0 
    /*00002030*/ BEQ         JPEGE_ProcStreamBufForMpf_x2000
    JPEGE_ProcStreamBufForMpf_x2034:
    /*00002034*/ CMP         R0, #0 
    /*00002038*/ BNE         JPEGE_ProcStreamBufForMpf_x2080
    /*0000203c*/ LDR         R3, [R8, #3784]             @ 0xec8 
    /*00002040*/ CMP         R3, #0 
    /*00002044*/ BNE         JPEGE_ProcStreamBufForMpf_x2124
    JPEGE_ProcStreamBufForMpf_x2048:
    /*00002048*/ LDR         R3, [R8, #3796]             @ 0xed4 
    /*0000204c*/ CMP         R3, #0 
    /*00002050*/ MOVNE       R3, #0 
    /*00002054*/ MOVEQ       R3, #1 
    /*00002058*/ STRB        R3, [R4, #11] 
    /*0000205c*/ MOV         R3, #0 
    /*00002060*/ STR         R3, [R4, #36]               @ 0x24 
    /*00002064*/ STR         R5, [R8, #3784]             @ 0xec8 
    JPEGE_ProcStreamBufForMpf_x2068:
    /*00002068*/ MOV         R0, R9 
    /*0000206c*/ ADD         R1, SP, #8 
    /*00002070*/ BL          0x0000000000002070          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002074*/ MOV         R0, #0 
    /*00002078*/ ADD         SP, SP, #24 
    /*0000207c*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    JPEGE_ProcStreamBufForMpf_x2080:
    /*00002080*/ CMP         R0, #3 
    /*00002084*/ BEQ         JPEGE_ProcStreamBufForMpf_x2144
    /*00002088*/ CMP         R0, #1 
    /*0000208c*/ BEQ         JPEGE_ProcStreamBufForMpf_x22c0
    /*00002090*/ CMP         R0, #2 
    /*00002094*/ BEQ         JPEGE_ProcStreamBufForMpf_x2280
    /*00002098*/ MOVW        R3, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8d0] 
    /*0000209c*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*000020a0*/ MOVT        R3, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8d0] 
    /*000020a4*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*000020a8*/ MOV         R2, #1344                   @ 0x540 
    /*000020ac*/ LDR         R1, [PC, #740]              @ 0x0000000000002398 
    /*000020b0*/ BL          0x00000000000020B0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000020b4*/ B           JPEGE_ProcStreamBufForMpf_x2068
    JPEGE_ProcStreamBufForMpf_x20b8:
    /*000020b8*/ LDR         LR, [R4, #4] 
    /*000020bc*/ MOV         R0, #128                    @ 0x80 
    /*000020c0*/ MOVT        R0, #1 
    /*000020c4*/ LDR         IP, [R4] 
    /*000020c8*/ SUB         R2, R0, R2 
    /*000020cc*/ ADD         R2, R2, LR 
    /*000020d0*/ SUB         R2, R2, IP 
    /*000020d4*/ STR         R2, [R4, #4] 
    /*000020d8*/ LDR         R2, [R3] 
    /*000020dc*/ SUB         R2, R0, R2 
    /*000020e0*/ STR         R2, [R4] 
    /*000020e4*/ LDR         R0, [R8, #3800]             @ 0xed8 
    /*000020e8*/ B           JPEGE_ProcStreamBufForMpf_x1ff4
    JPEGE_ProcStreamBufForMpf_x20ec:
    /*000020ec*/ LDR         R3, [R8, #3764]             @ 0xeb4 
    /*000020f0*/ CMP         R3, #1 
    /*000020f4*/ BNE         JPEGE_ProcStreamBufForMpf_x1f98
    /*000020f8*/ LDR         R0, [R8, #3800]             @ 0xed8 
    /*000020fc*/ CMP         R0, #0 
    /*00002100*/ BEQ         JPEGE_ProcStreamBufForMpf_x2310
    /*00002104*/ CMP         R0, #3 
    /*00002108*/ BEQ         JPEGE_ProcStreamBufForMpf_x237c
    /*0000210c*/ LDR         R3, [R7, #160]              @ 0xa0 
    /*00002110*/ MOV         SL, #0 
    /*00002114*/ LDR         R6, [R7, #176]              @ 0xb0 
    /*00002118*/ LDR         R3, [R3] 
    /*0000211c*/ ADD         R3, R6, R3 
    /*00002120*/ B           JPEGE_ProcStreamBufForMpf_x1fc4
    JPEGE_ProcStreamBufForMpf_x2124:
    /*00002124*/ MOVW        R3, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x820] 
    /*00002128*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*0000212c*/ MOVT        R3, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x820] 
    /*00002130*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00002134*/ MOV         R2, #1248                   @ 0x4e0 
    /*00002138*/ LDR         R1, [PC, #600]              @ 0x0000000000002398 
    /*0000213c*/ BL          0x000000000000213C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002140*/ B           JPEGE_ProcStreamBufForMpf_x2048
    JPEGE_ProcStreamBufForMpf_x2144:
    /*00002144*/ LDR         R3, [R8, #3772]             @ 0xebc 
    /*00002148*/ CMP         R3, #0 
    /*0000214c*/ BNE         JPEGE_ProcStreamBufForMpf_x231c
    JPEGE_ProcStreamBufForMpf_x2150:
    /*00002150*/ ADD         R3, SL, #64                 @ 0x40 
    /*00002154*/ MOV         R2, #0 
    /*00002158*/ CMP         R6, R3 
    /*0000215c*/ STRB        R2, [R4, #11] 
    /*00002160*/ MOV         R3, #3 
    /*00002164*/ STR         R3, [R4, #36]               @ 0x24 
    /*00002168*/ STR         R5, [R8, #3772]             @ 0xebc 
    /*0000216c*/ SUBLS       R5, R5, #4 
    /*00002170*/ LDRLS       R3, [R7, #176]              @ 0xb0 
    /*00002174*/ SUBHI       R5, R5, #4 
    /*00002178*/ LSRLS       R2, R5, #8 
    /*0000217c*/ STRBHI      R5, [SL, #69]               @ 0x45 
    /*00002180*/ LSRHI       R5, R5, #8 
    /*00002184*/ STRBHI      R5, [SL, #68]               @ 0x44 
    /*00002188*/ STRBLS      R2, [R3, #4] 
    /*0000218c*/ ADD         R2, SL, #812                @ 0x32c 
    /*00002190*/ LDRLS       R3, [R7, #176]              @ 0xb0 
    /*00002194*/ ADD         R2, R2, #2 
    /*00002198*/ ADD         SL, SL, #1040               @ 0x410 
    /*0000219c*/ STRBLS      R5, [R3, #5] 
    /*000021a0*/ CMP         R6, R2 
    /*000021a4*/ LDR         R3, [SL, #-1040]            @ 0xfffffbf0 
    /*000021a8*/ ADD         R5, SP, #16 
    /*000021ac*/ LDR         R1, [SL, #-1036]            @ 0xfffffbf4 
    /*000021b0*/ ADD         SL, SL, #2 
    /*000021b4*/ SUB         R3, R3, #1664               @ 0x680 
    /*000021b8*/ SUB         R3, R3, R1 
    /*000021bc*/ LDM         R4, {R0, R1} 
    /*000021c0*/ STR         R2, [SP, #12] 
    /*000021c4*/ ADD         R4, R3, R0 
    /*000021c8*/ LDRLS       R3, [R7, #8] 
    /*000021cc*/ SUB         R4, R4, R1 
    /*000021d0*/ STR         R5, [SP] 
    /*000021d4*/ SUBLS       R2, R2, R3 
    /*000021d8*/ MOV         R1, R6 
    /*000021dc*/ STRLS       R2, [SP, #12] 
    /*000021e0*/ MOV         R3, #4 
    /*000021e4*/ ADD         R2, SP, #12 
    /*000021e8*/ MOV         R0, R7 
    /*000021ec*/ LSR         IP, R4, #24 
    /*000021f0*/ STRB        R4, [SP, #19] 
    /*000021f4*/ STRB        IP, [SP, #16] 
    /*000021f8*/ LSR         IP, R4, #16 
    /*000021fc*/ STRB        IP, [SP, #17] 
    /*00002200*/ LSR         IP, R4, #8 
    /*00002204*/ STRB        IP, [SP, #18] 
    /*00002208*/ BL          0x0000000000002208          @ JPEGE_FillStreamBuf [R_ARM_CALL: .text+0x1a1c] 
    /*0000220c*/ LSL         R2, R4, #13 
    /*00002210*/ MOVW        R3, #33205                  @ 0x81b5 
    /*00002214*/ MOVT        R3, #6990                   @ 0x1b4e 
    /*00002218*/ CMP         R6, SL 
    /*0000221c*/ STR         SL, [SP, #12] 
    /*00002220*/ MOV         R1, R6 
    /*00002224*/ UMULL       R2, R3, R2, R3 
    /*00002228*/ MOV         R0, R7 
    /*0000222c*/ ADD         R2, SP, #12 
    /*00002230*/ MOV         LR, #0 
    /*00002234*/ STRB        LR, [SP, #16] 
    /*00002238*/ LSR         IP, R3, #11 
    /*0000223c*/ LDRLS       R3, [R7, #8] 
    /*00002240*/ STR         R5, [SP] 
    /*00002244*/ SUBLS       SL, SL, R3 
    /*00002248*/ MOV         R3, #8 
    /*0000224c*/ STRB        IP, [SP, #19] 
    /*00002250*/ STRB        LR, [SP, #20] 
    /*00002254*/ STRB        LR, [SP, #21] 
    /*00002258*/ STRB        LR, [SP, #23] 
    /*0000225c*/ LSR         LR, IP, #16 
    /*00002260*/ STRLS       SL, [SP, #12] 
    /*00002264*/ LSR         IP, IP, R3 
    /*00002268*/ STRB        LR, [SP, #17] 
    /*0000226c*/ STRB        IP, [SP, #18] 
    /*00002270*/ MOV         IP, #4 
    /*00002274*/ STRB        IP, [SP, #22] 
    /*00002278*/ BL          0x0000000000002278          @ JPEGE_FillStreamBuf [R_ARM_CALL: .text+0x1a1c] 
    /*0000227c*/ B           JPEGE_ProcStreamBufForMpf_x2068
    JPEGE_ProcStreamBufForMpf_x2280:
    /*00002280*/ LDR         R3, [R8, #3792]             @ 0xed0 
    /*00002284*/ CMP         R3, #0 
    /*00002288*/ BNE         JPEGE_ProcStreamBufForMpf_x233c
    JPEGE_ProcStreamBufForMpf_x228c:
    /*0000228c*/ MOV         R3, #2 
    /*00002290*/ STR         R3, [R4, #36]               @ 0x24 
    /*00002294*/ LDRB        R1, [R8, #3804]             @ 0xedc 
    /*00002298*/ LDR         R3, [R8, #3784]             @ 0xec8 
    /*0000229c*/ LDR         R2, [R8, #3788]             @ 0xecc 
    /*000022a0*/ CMP         R1, #1 
    /*000022a4*/ SUB         R3, R5, R3 
    /*000022a8*/ SUB         R3, R3, R2 
    /*000022ac*/ STR         R3, [R8, #3792]             @ 0xed0 
    /*000022b0*/ LDREQ       R2, [R8, #3772]             @ 0xebc 
    /*000022b4*/ SUBEQ       R3, R3, R2 
    /*000022b8*/ STREQ       R3, [R8, #3792]             @ 0xed0 
    /*000022bc*/ B           JPEGE_ProcStreamBufForMpf_x2068
    JPEGE_ProcStreamBufForMpf_x22c0:
    /*000022c0*/ LDR         R3, [R8, #3788]             @ 0xecc 
    /*000022c4*/ CMP         R3, #0 
    /*000022c8*/ BNE         JPEGE_ProcStreamBufForMpf_x235c
    JPEGE_ProcStreamBufForMpf_x22cc:
    /*000022cc*/ LDR         R3, [R8, #3796]             @ 0xed4 
    /*000022d0*/ CMP         R3, #1 
    /*000022d4*/ MOVHI       R3, #0 
    /*000022d8*/ STRBHI      R3, [R4, #11] 
    /*000022dc*/ MOV         R3, #1 
    /*000022e0*/ STR         R3, [R4, #36]               @ 0x24 
    /*000022e4*/ LDRB        R2, [R8, #3804]             @ 0xedc 
    /*000022e8*/ LDR         R3, [R8, #3784]             @ 0xec8 
    /*000022ec*/ CMP         R2, #1 
    /*000022f0*/ SUB         R5, R5, R3 
    /*000022f4*/ STR         R5, [R8, #3788]             @ 0xecc 
    /*000022f8*/ LDREQ       R3, [R8, #3772]             @ 0xebc 
    /*000022fc*/ SUBEQ       R5, R5, R3 
    /*00002300*/ STREQ       R5, [R8, #3788]             @ 0xecc 
    /*00002304*/ B           JPEGE_ProcStreamBufForMpf_x2068
    JPEGE_ProcStreamBufForMpf_x2308:
    /*00002308*/ MOV         R4, R3 
    /*0000230c*/ B           JPEGE_ProcStreamBufForMpf_x2034
    JPEGE_ProcStreamBufForMpf_x2310:
    /*00002310*/ LDR         R3, [R8, #3808]             @ 0xee0 
    /*00002314*/ STR         R3, [R7, #32] 
    /*00002318*/ B           JPEGE_ProcStreamBufForMpf_x1f98
    JPEGE_ProcStreamBufForMpf_x231c:
    /*0000231c*/ MOVW        R3, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x84c] 
    /*00002320*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00002324*/ MOVT        R3, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x84c] 
    /*00002328*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*0000232c*/ MOVW        R2, #1262                   @ 0x4ee 
    /*00002330*/ LDR         R1, [PC, #96]               @ 0x0000000000002398 
    /*00002334*/ BL          0x0000000000002334          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002338*/ B           JPEGE_ProcStreamBufForMpf_x2150
    JPEGE_ProcStreamBufForMpf_x233c:
    /*0000233c*/ MOVW        R3, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8a4] 
    /*00002340*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00002344*/ MOVT        R3, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8a4] 
    /*00002348*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*0000234c*/ MOVW        R2, #1334                   @ 0x536 
    /*00002350*/ LDR         R1, [PC, #64]               @ 0x0000000000002398 
    /*00002354*/ BL          0x0000000000002354          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002358*/ B           JPEGE_ProcStreamBufForMpf_x228c
    JPEGE_ProcStreamBufForMpf_x235c:
    /*0000235c*/ MOVW        R3, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x878] 
    /*00002360*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00002364*/ MOVT        R3, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x878] 
    /*00002368*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*0000236c*/ MOVW        R2, #1320                   @ 0x528 
    /*00002370*/ LDR         R1, [PC, #32]               @ 0x0000000000002398 
    /*00002374*/ BL          0x0000000000002374          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002378*/ B           JPEGE_ProcStreamBufForMpf_x22cc
    JPEGE_ProcStreamBufForMpf_x237c:
    /*0000237c*/ LDR         R3, [R8, #3816]             @ 0xee8 
    /*00002380*/ STR         R3, [R7, #32] 
    /*00002384*/ LDR         R3, [R8, #3820]             @ 0xeec 
    /*00002388*/ STR         R3, [R7, #128]              @ 0x80 
    /*0000238c*/ LDR         R3, [R8, #3820]             @ 0xeec 
    /*00002390*/ STR         R3, [R7, #96]               @ 0x60 
    /*00002394*/ B           JPEGE_ProcStreamBufForMpf_x1f98
    /*00002398*/ MULEQ       R0, R8, R1 
FUNC_END JPEGE_ProcStreamBufForMpf


FUNC_BEGIN Jpege_DealMpfStream
    /*0000239c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*000023a0*/ SUB         SP, SP, #48                 @ 0x30 
    /*000023a4*/ ADD         R4, R0, #4096               @ 0x1000 
    /*000023a8*/ MOV         R5, R0 
    /*000023ac*/ MOV         R2, #48                     @ 0x30 
    /*000023b0*/ MOV         R9, R1 
    /*000023b4*/ MOV         R0, SP 
    /*000023b8*/ MOV         R1, #0 
    /*000023bc*/ LDR         R7, [R4, #3772]             @ 0xebc 
    /*000023c0*/ LDR         SL, [R4, #3776]             @ 0xec0 
    /*000023c4*/ BL          0x00000000000023C4          @ memset [R_ARM_CALL: +0x0] 
    /*000023c8*/ MOV         R2, #16 
    /*000023cc*/ LDR         R1, [PC, #448]              @ 0x0000000000002594 
    /*000023d0*/ MOV         R0, SP 
    /*000023d4*/ LDR         R6, [R5, #452]              @ 0x1c4 
    /*000023d8*/ BL          0x00000000000023D8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000023dc*/ LDRB        R3, [R4, #3804]             @ 0xedc 
    /*000023e0*/ LDR         R1, [R4, #3796]             @ 0xed4 
    /*000023e4*/ CMP         R3, #0 
    /*000023e8*/ LDR         R3, [R4, #3784]             @ 0xec8 
    /*000023ec*/ LDRNE       R2, [R4, #3772]             @ 0xebc 
    /*000023f0*/ ADDNE       R3, R3, R2 
    /*000023f4*/ CMP         R1, #2 
    /*000023f8*/ STRB        R3, [SP, #7] 
    /*000023fc*/ LSR         R2, R3, #24 
    /*00002400*/ STRB        R2, [SP, #4] 
    /*00002404*/ LSR         R2, R3, #16 
    /*00002408*/ LSR         R3, R3, #8 
    /*0000240c*/ STRB        R3, [SP, #6] 
    /*00002410*/ MOVEQ       R3, #3 
    /*00002414*/ LDRBNE      R3, [SP, #15] 
    /*00002418*/ STRB        R2, [SP, #5] 
    /*0000241c*/ MOV         R2, #16 
    /*00002420*/ LDR         R1, [PC, #368]              @ 0x0000000000002598 
    /*00002424*/ ADD         R0, SP, R2 
    /*00002428*/ STRB        R3, [SP, #15] 
    /*0000242c*/ BL          0x000000000000242C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002430*/ LDRB        R2, [R4, #3804]             @ 0xedc 
    /*00002434*/ LDR         R3, [R4, #3788]             @ 0xecc 
    /*00002438*/ CMP         R2, #0 
    /*0000243c*/ LDR         R5, [R4, #3784]             @ 0xec8 
    /*00002440*/ LSR         R1, R3, #24 
    /*00002444*/ STRB        R1, [SP, #20] 
    /*00002448*/ LSR         R1, R3, #16 
    /*0000244c*/ STRB        R1, [SP, #21] 
    /*00002450*/ LDREQ       R1, [R4, #3772]             @ 0xebc 
    /*00002454*/ SUBNE       R5, R5, #8 
    /*00002458*/ STRB        R3, [SP, #23] 
    /*0000245c*/ LSR         R3, R3, #8 
    /*00002460*/ STRB        R3, [SP, #22] 
    /*00002464*/ SUBEQ       R5, R5, R1 
    /*00002468*/ LDREQ       R3, [R4, #3776]             @ 0xec0 
    /*0000246c*/ LDRNE       R3, [R4, #3776]             @ 0xec0 
    /*00002470*/ LDR         R1, [R4, #3796]             @ 0xed4 
    /*00002474*/ SUBEQ       R5, R5, R3 
    /*00002478*/ SUBEQ       R5, R5, #8 
    /*0000247c*/ SUBNE       R5, R5, R3 
    /*00002480*/ CMP         R1, #2 
    /*00002484*/ STRB        R5, [SP, #27] 
    /*00002488*/ LSR         R3, R5, #24 
    /*0000248c*/ STRB        R3, [SP, #24] 
    /*00002490*/ MOVNE       R8, #32 
    /*00002494*/ LSR         R3, R5, #16 
    /*00002498*/ STRB        R3, [SP, #25] 
    /*0000249c*/ LSR         R3, R5, #8 
    /*000024a0*/ STRB        R3, [SP, #26] 
    /*000024a4*/ BEQ         Jpege_DealMpfStream_x2534
    Jpege_DealMpfStream_x24a8:
    /*000024a8*/ CMP         R2, #1 
    /*000024ac*/ ADDNE       R7, R7, SL 
    /*000024b0*/ LDREQ       R2, [R4, #3776]             @ 0xec0 
    /*000024b4*/ MOVWEQ      R3, #274                    @ 0x112 
    /*000024b8*/ LDREQ       R0, [R9] 
    /*000024bc*/ MOVTEQ      R3, #1 
    /*000024c0*/ ADDEQ       R3, R2, R3 
    /*000024c4*/ LDRNE       R0, [R9] 
    /*000024c8*/ ADDEQ       R0, R0, R3 
    /*000024cc*/ LDR         R2, [R6, #8] 
    /*000024d0*/ LDR         R3, [R6, #176]              @ 0xb0 
    /*000024d4*/ ADDNE       R7, R7, #146                @ 0x92 
    /*000024d8*/ ADDNE       R0, R0, R7 
    /*000024dc*/ ADD         R2, R3, R2 
    /*000024e0*/ CMP         R0, R2 
    /*000024e4*/ SUBCS       R0, R0, R2 
    /*000024e8*/ ADDCS       R0, R3, R0 
    /*000024ec*/ ADD         R3, R0, R8 
    /*000024f0*/ CMP         R2, R3 
    /*000024f4*/ BLS         Jpege_DealMpfStream_x250c
    /*000024f8*/ MOV         R2, R8 
    /*000024fc*/ MOV         R1, SP 
    /*00002500*/ BL          0x0000000000002500          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002504*/ ADD         SP, SP, #48                 @ 0x30 
    /*00002508*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    Jpege_DealMpfStream_x250c:
    /*0000250c*/ SUB         R4, R2, R0 
    /*00002510*/ MOV         R1, SP 
    /*00002514*/ MOV         R2, R4 
    /*00002518*/ BL          0x0000000000002518          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000251c*/ SUB         R2, R8, R4 
    /*00002520*/ ADD         R1, SP, R4 
    /*00002524*/ LDR         R0, [R6, #176]              @ 0xb0 
    /*00002528*/ BL          0x0000000000002528          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000252c*/ ADD         SP, SP, #48                 @ 0x30 
    /*00002530*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    Jpege_DealMpfStream_x2534:
    /*00002534*/ MOV         R2, #16 
    /*00002538*/ LDR         R1, [PC, #88]               @ 0x0000000000002598 
    /*0000253c*/ ADD         R0, SP, #32 
    /*00002540*/ MOV         R8, #48                     @ 0x30 
    /*00002544*/ BL          0x0000000000002544          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002548*/ LDR         R2, [R4, #3788]             @ 0xecc 
    /*0000254c*/ LDR         R3, [R4, #3792]             @ 0xed0 
    /*00002550*/ ADD         R5, R5, R2 
    /*00002554*/ LDRB        R2, [R4, #3804]             @ 0xedc 
    /*00002558*/ STRB        R5, [SP, #43]               @ 0x2b 
    /*0000255c*/ LSR         R1, R5, #24 
    /*00002560*/ STRB        R3, [SP, #39]               @ 0x27 
    /*00002564*/ STRB        R1, [SP, #40]               @ 0x28 
    /*00002568*/ LSR         R1, R5, #16 
    /*0000256c*/ LSR         R5, R5, #8 
    /*00002570*/ STRB        R1, [SP, #41]               @ 0x29 
    /*00002574*/ STRB        R5, [SP, #42]               @ 0x2a 
    /*00002578*/ LSR         R1, R3, #24 
    /*0000257c*/ STRB        R1, [SP, #36]               @ 0x24 
    /*00002580*/ LSR         R1, R3, #16 
    /*00002584*/ LSR         R3, R3, #8 
    /*00002588*/ STRB        R1, [SP, #37]               @ 0x25 
    /*0000258c*/ STRB        R3, [SP, #38]               @ 0x26 
    /*00002590*/ B           Jpege_DealMpfStream_x24a8
    /*00002594*/ @           <UNDEFINED> instruction: 0x000001b4 
    /*00002598*/ ANDEQ       R0, R0, R4, ASR #3 
FUNC_END Jpege_DealMpfStream


FUNC_BEGIN JPEGE_GetBitStream
    /*0000259c*/ CMP         R0, #15 
    /*000025a0*/ BHI         JPEGE_GetBitStream_x2a44
    /*000025a4*/ CMP         R1, #0 
    /*000025a8*/ BEQ         JPEGE_GetBitStream_x2c68
    /*000025ac*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000025b0*/ SUB         SP, SP, #188                @ 0xbc 
    /*000025b4*/ MOV         R7, R0 
    /*000025b8*/ MOV         R4, R1 
    /*000025bc*/ BL          0x00000000000025BC          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*000025c0*/ SUBS        R8, R0, #0 
    /*000025c4*/ BEQ         JPEGE_GetBitStream_x2c74
    /*000025c8*/ ADD         R6, R8, #6912               @ 0x1b00 
    /*000025cc*/ ADD         R1, SP, #36                 @ 0x24 
    /*000025d0*/ ADD         R6, R6, #52                 @ 0x34 
    /*000025d4*/ MOV         R0, R6 
    /*000025d8*/ BL          0x00000000000025D8          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000025dc*/ LDR         R5, [R8, #452]              @ 0x1c4 
    /*000025e0*/ LDR         R2, [R5, #160]              @ 0xa0 
    /*000025e4*/ LDR         R1, [R5, #172]              @ 0xac 
    /*000025e8*/ LDR         R9, [R2] 
    /*000025ec*/ LDR         R3, [R1] 
    /*000025f0*/ CMP         R3, R9 
    /*000025f4*/ LDRCC       R0, [R5, #8] 
    /*000025f8*/ ADDCC       R3, R3, R0 
    /*000025fc*/ SUB         R3, R3, R9 
    /*00002600*/ CMP         R3, #0 
    /*00002604*/ BEQ         JPEGE_GetBitStream_x2c80
    /*00002608*/ LDR         R3, [R5, #176]              @ 0xb0 
    /*0000260c*/ ADD         R9, R3, R9 
    /*00002610*/ LDR         R0, [R9, #12] 
    /*00002614*/ CMP         R7, R0 
    /*00002618*/ BNE         JPEGE_GetBitStream_x2a50
    /*0000261c*/ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002620*/ MOVT        R0, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002624*/ LDR         R0, [R0, #68]               @ 0x44 
    /*00002628*/ CMP         R0, #1 
    /*0000262c*/ ADDNE       R7, R8, #4096               @ 0x1000 
    /*00002630*/ BEQ         JPEGE_GetBitStream_x27c8
    JPEGE_GetBitStream_x2634:
    /*00002634*/ TST         R9, #63                     @ 0x3f 
    /*00002638*/ STR         R9, [SP, #40]               @ 0x28 
    /*0000263c*/ BNE         JPEGE_GetBitStream_x2900
    JPEGE_GetBitStream_x2640:
    /*00002640*/ LDR         R3, [R8, #16] 
    /*00002644*/ CMP         R3, #0 
    /*00002648*/ BNE         JPEGE_GetBitStream_x2664
    /*0000264c*/ LDR         R3, [R7, #3796]             @ 0xed4 
    /*00002650*/ CMP         R3, #0 
    /*00002654*/ BEQ         JPEGE_GetBitStream_x2664
    /*00002658*/ LDR         R3, [R7, #3824]             @ 0xef0 
    /*0000265c*/ CMP         R3, #0 
    /*00002660*/ BNE         JPEGE_GetBitStream_x29cc
    JPEGE_GetBitStream_x2664:
    /*00002664*/ LDR         R0, [R5, #160]              @ 0xa0 
    /*00002668*/ LDR         R3, [R5, #172]              @ 0xac 
    /*0000266c*/ LDR         R2, [SP, #40]               @ 0x28 
    /*00002670*/ LDR         SL, [R0] 
    /*00002674*/ LDR         R3, [R3] 
    /*00002678*/ LDR         IP, [R2] 
    /*0000267c*/ CMP         SL, R3 
    /*00002680*/ ADD         R2, IP, SL 
    /*00002684*/ BLS         JPEGE_GetBitStream_x279c
    /*00002688*/ LDR         R1, [R5, #8] 
    /*0000268c*/ CMP         R2, R1 
    /*00002690*/ BCC         JPEGE_GetBitStream_x27b0
    /*00002694*/ SUB         R2, R2, R1 
    /*00002698*/ CMP         R3, R2 
    /*0000269c*/ BCC         JPEGE_GetBitStream_x2920
    JPEGE_GetBitStream_x26a0:
    /*000026a0*/ LDR         R3, [R5, #176]              @ 0xb0 
    /*000026a4*/ SUB         R1, R1, SL 
    /*000026a8*/ SUB         R2, IP, R1 
    /*000026ac*/ STR         R1, [SP, #80]               @ 0x50 
    /*000026b0*/ ADD         SL, R3, SL 
    /*000026b4*/ STR         R2, [SP, #84]               @ 0x54 
    JPEGE_GetBitStream_x26b8:
    /*000026b8*/ LDRD        R8, [R5] 
    /*000026bc*/ ORRS        R1, R8, R9 
    /*000026c0*/ BEQ         JPEGE_GetBitStream_x26d4
    /*000026c4*/ SUBS        R8, R8, R3 
    /*000026c8*/ SBC         R9, R9, #0 
    /*000026cc*/ ADDS        R8, R8, SL 
    /*000026d0*/ ADC         R9, R9, #0 
    JPEGE_GetBitStream_x26d4:
    /*000026d4*/ STR         R2, [R0] 
    JPEGE_GetBitStream_x26d8:
    /*000026d8*/ LDR         FP, [SP, #84]               @ 0x54 
    /*000026dc*/ LDR         R5, [SP, #80]               @ 0x50 
    /*000026e0*/ ADD         R5, FP, R5 
    /*000026e4*/ CMP         R5, #64                     @ 0x40 
    /*000026e8*/ BLS         JPEGE_GetBitStream_x2990
    JPEGE_GetBitStream_x26ec:
    /*000026ec*/ LDR         R1, [SP, #40]               @ 0x28 
    /*000026f0*/ LDRB        R0, [R1, #11] 
    /*000026f4*/ TST         R0, #254                    @ 0xfe 
    /*000026f8*/ BNE         JPEGE_GetBitStream_x2968
    JPEGE_GetBitStream_x26fc:
    /*000026fc*/ LDRB        R3, [R1, #8] 
    /*00002700*/ LDRB        R2, [R7, #3804]             @ 0xedc 
    /*00002704*/ SUB         IP, R3, #6 
    /*00002708*/ TST         IP, #253                    @ 0xfd 
    /*0000270c*/ MOVNE       R3, #5 
    /*00002710*/ CMP         R2, #1 
    /*00002714*/ BEQ         JPEGE_GetBitStream_x29b0
    JPEGE_GetBitStream_x2718:
    /*00002718*/ STR         R0, [R4, #24] 
    /*0000271c*/ CMP         R0, #0 
    /*00002720*/ STR         R3, [R4, #28] 
    /*00002724*/ MOV         IP, #0 
    /*00002728*/ LDRD        R2, [R1, #16] 
    /*0000272c*/ STR         SL, [R4, #8] 
    /*00002730*/ STRD        R8, [R4] 
    /*00002734*/ STRD        R2, [R4, #16] 
    /*00002738*/ MOVNE       R3, #1 
    /*0000273c*/ STR         IP, [R4, #32] 
    /*00002740*/ MOV         R2, #0 
    /*00002744*/ STRNE       R3, [R7, #3824]             @ 0xef0 
    /*00002748*/ STREQ       R0, [R7, #3824]             @ 0xef0 
    /*0000274c*/ LDR         R3, [R1, #4] 
    /*00002750*/ SUB         R5, R5, R3 
    /*00002754*/ STR         R5, [R4, #12] 
    /*00002758*/ LDR         R3, [R7, #3708]             @ 0xe7c 
    /*0000275c*/ LDR         R0, [R1] 
    /*00002760*/ SUB         R3, R3, #64                 @ 0x40 
    /*00002764*/ LDR         R1, [R1, #4] 
    /*00002768*/ ADD         R3, R3, R0 
    /*0000276c*/ SUB         R3, R3, R1 
    /*00002770*/ STR         R3, [R7, #3708]             @ 0xe7c 
    /*00002774*/ LDR         R3, [R4, #12] 
    /*00002778*/ STR         R2, [R4, #36]               @ 0x24 
    /*0000277c*/ CMP         R3, #64                     @ 0x40 
    /*00002780*/ BLS         JPEGE_GetBitStream_x2948
    JPEGE_GetBitStream_x2784:
    /*00002784*/ MOV         R0, R6 
    /*00002788*/ ADD         R1, SP, #36                 @ 0x24 
    /*0000278c*/ BL          0x000000000000278C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002790*/ MOV         R0, #0 
    JPEGE_GetBitStream_x2794:
    /*00002794*/ ADD         SP, SP, #188                @ 0xbc 
    /*00002798*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JPEGE_GetBitStream_x279c:
    /*0000279c*/ CMP         R3, R2 
    /*000027a0*/ BCC         JPEGE_GetBitStream_x2920
    /*000027a4*/ LDR         R1, [R5, #8] 
    /*000027a8*/ CMP         R2, R1 
    /*000027ac*/ BCS         JPEGE_GetBitStream_x26a0
    JPEGE_GetBitStream_x27b0:
    /*000027b0*/ LDR         R3, [R5, #176]              @ 0xb0 
    /*000027b4*/ MOV         R1, #0 
    /*000027b8*/ STR         IP, [SP, #80]               @ 0x50 
    /*000027bc*/ ADD         SL, R3, SL 
    /*000027c0*/ STR         R1, [SP, #84]               @ 0x54 
    /*000027c4*/ B           JPEGE_GetBitStream_x26b8
    JPEGE_GetBitStream_x27c8:
    /*000027c8*/ LDRB        R0, [R9, #8] 
    /*000027cc*/ ADD         R7, R8, #4096               @ 0x1000 
    /*000027d0*/ CMP         R0, #8 
    /*000027d4*/ BNE         JPEGE_GetBitStream_x2634
    /*000027d8*/ LDRB        R0, [R7, #3804]             @ 0xedc 
    /*000027dc*/ CMP         R0, #0 
    /*000027e0*/ BNE         JPEGE_GetBitStream_x2634
    /*000027e4*/ MOV         IP, R0 
    /*000027e8*/ ADD         SL, SP, #56                 @ 0x38 
    /*000027ec*/ STR         R6, [SP, #24] 
    /*000027f0*/ MOV         FP, IP 
    /*000027f4*/ STR         R4, [SP, #28] 
    /*000027f8*/ MOV         R6, IP 
    /*000027fc*/ MOV         R4, IP 
    /*00002800*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf8] 
    /*00002804*/ MOVW        LR, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00002808*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf8] 
    /*0000280c*/ MOVT        LR, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00002810*/ STR         R0, [SP, #12] 
    /*00002814*/ STR         LR, [SP, #16] 
    /*00002818*/ STR         IP, [SP, #8] 
    /*0000281c*/ STR         R8, [SP, #20] 
    JPEGE_GetBitStream_x2820:
    /*00002820*/ ADD         R0, SP, #40                 @ 0x28 
    /*00002824*/ STR         R9, [R0, FP, LSL #2] 
    /*00002828*/ LDR         R8, [R1] 
    /*0000282c*/ LDR         R0, [R2] 
    /*00002830*/ LDR         LR, [R9] 
    /*00002834*/ CMP         R0, R8 
    /*00002838*/ ADD         R1, LR, R0 
    /*0000283c*/ BHI         JPEGE_GetBitStream_x29dc
    /*00002840*/ CMP         R8, R1 
    /*00002844*/ BCC         JPEGE_GetBitStream_x29f4
    /*00002848*/ LDR         IP, [R5, #8] 
    /*0000284c*/ CMP         R1, IP 
    /*00002850*/ BCC         JPEGE_GetBitStream_x2a24
    JPEGE_GetBitStream_x2854:
    /*00002854*/ SUB         IP, IP, R0 
    /*00002858*/ ADD         R0, R3, R0 
    /*0000285c*/ SUB         R1, LR, IP 
    /*00002860*/ STR         IP, [SL, #24] 
    /*00002864*/ STR         R1, [SL, #28] 
    /*00002868*/ STM         SL, {R0, R3} 
    JPEGE_GetBitStream_x286c:
    /*0000286c*/ LDRD        R8, [R5] 
    /*00002870*/ ORRS        R0, R8, R9 
    /*00002874*/ BEQ         JPEGE_GetBitStream_x288c
    /*00002878*/ LDR         R0, [SL] 
    /*0000287c*/ SUBS        R8, R8, R3 
    /*00002880*/ SBC         R9, R9, #0 
    /*00002884*/ ADDS        R8, R8, R0 
    /*00002888*/ ADC         R9, R9, #0 
    JPEGE_GetBitStream_x288c:
    /*0000288c*/ STRD        R8, [SL, #8] 
    /*00002890*/ STR         R1, [R2] 
    JPEGE_GetBitStream_x2894:
    /*00002894*/ ADD         R3, SP, #40                 @ 0x28 
    /*00002898*/ LDR         R2, [SL, #28] 
    /*0000289c*/ LDR         R1, [R3, FP, LSL #2] 
    /*000028a0*/ CMP         R2, #0 
    /*000028a4*/ LDRB        R3, [R1, #8] 
    /*000028a8*/ BEQ         JPEGE_GetBitStream_x2a0c
    /*000028ac*/ LDR         R0, [SL, #24] 
    /*000028b0*/ ADD         R6, R6, R2 
    /*000028b4*/ MOV         R2, #1 
    /*000028b8*/ STR         R2, [SP, #8] 
    /*000028bc*/ ADD         R4, R4, R0 
    JPEGE_GetBitStream_x28c0:
    /*000028c0*/ ADD         R0, FP, #1 
    /*000028c4*/ ADD         SL, SL, #32 
    /*000028c8*/ CMP         R0, #3 
    /*000028cc*/ MOVGT       R2, #0 
    /*000028d0*/ MOVLE       R2, #1 
    /*000028d4*/ CMP         R3, #1 
    /*000028d8*/ MOVEQ       R2, #0 
    /*000028dc*/ CMP         R2, #0 
    /*000028e0*/ BEQ         JPEGE_GetBitStream_x2ac4
    /*000028e4*/ LDR         R2, [R5, #160]              @ 0xa0 
    /*000028e8*/ MOV         FP, R0 
    /*000028ec*/ LDR         R3, [R5, #176]              @ 0xb0 
    /*000028f0*/ LDR         R1, [R5, #172]              @ 0xac 
    /*000028f4*/ LDR         R9, [R2] 
    /*000028f8*/ ADD         R9, R3, R9 
    /*000028fc*/ B           JPEGE_GetBitStream_x2820
    JPEGE_GetBitStream_x2900:
    /*00002900*/ MOVW        R3, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9b4] 
    /*00002904*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00002908*/ MOVT        R3, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9b4] 
    /*0000290c*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00002910*/ MOVW        R2, #1778                   @ 0x6f2 
    /*00002914*/ LDR         R1, [PC, #1096]             @ 0x0000000000002d64 
    /*00002918*/ BL          0x0000000000002918          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000291c*/ B           JPEGE_GetBitStream_x2640
    JPEGE_GetBitStream_x2920:
    /*00002920*/ MOVW        R3, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf8] 
    /*00002924*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00002928*/ MOVT        R3, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf8] 
    /*0000292c*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00002930*/ MOVW        R2, #1796                   @ 0x704 
    /*00002934*/ LDR         R1, [PC, #1064]             @ 0x0000000000002d64 
    /*00002938*/ BL          0x0000000000002938          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000293c*/ LDR         SL, [SP, #56]               @ 0x38 
    /*00002940*/ LDRD        R8, [SP, #64]               @ 0x40 
    /*00002944*/ B           JPEGE_GetBitStream_x26d8
    JPEGE_GetBitStream_x2948:
    /*00002948*/ MOVW        R3, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa48] 
    /*0000294c*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00002950*/ MOVT        R3, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa48] 
    /*00002954*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00002958*/ MOV         R2, #1856                   @ 0x740 
    /*0000295c*/ LDR         R1, [PC, #1024]             @ 0x0000000000002d64 
    /*00002960*/ BL          0x0000000000002960          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002964*/ B           JPEGE_GetBitStream_x2784
    JPEGE_GetBitStream_x2968:
    /*00002968*/ LDR         R1, [PC, #1012]             @ 0x0000000000002d64 
    /*0000296c*/ MOVW        R3, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa24] 
    /*00002970*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00002974*/ MOVT        R3, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa24] 
    /*00002978*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*0000297c*/ MOVW        R2, #1801                   @ 0x709 
    /*00002980*/ BL          0x0000000000002980          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002984*/ LDR         R1, [SP, #40]               @ 0x28 
    /*00002988*/ LDRB        R0, [R1, #11] 
    /*0000298c*/ B           JPEGE_GetBitStream_x26fc
    JPEGE_GetBitStream_x2990:
    /*00002990*/ MOVW        R3, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9e8] 
    /*00002994*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00002998*/ MOVT        R3, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9e8] 
    /*0000299c*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*000029a0*/ MOVW        R2, #1798                   @ 0x706 
    /*000029a4*/ LDR         R1, [PC, #952]              @ 0x0000000000002d64 
    /*000029a8*/ BL          0x00000000000029A8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000029ac*/ B           JPEGE_GetBitStream_x26ec
    JPEGE_GetBitStream_x29b0:
    /*000029b0*/ LDR         R2, [R1, #36]               @ 0x24 
    /*000029b4*/ CMP         R2, #3 
    /*000029b8*/ BNE         JPEGE_GetBitStream_x2718
    /*000029bc*/ CMP         R3, #8 
    /*000029c0*/ MOVEQ       R3, #10 
    /*000029c4*/ MOVNE       R3, #9 
    /*000029c8*/ B           JPEGE_GetBitStream_x2718
    JPEGE_GetBitStream_x29cc:
    /*000029cc*/ MOV         R0, R8 
    /*000029d0*/ ADD         R1, SP, #40                 @ 0x28 
    /*000029d4*/ BL          0x00000000000029D4          @ Jpege_DealMpfStream [R_ARM_CALL: .text+0x239c] 
    /*000029d8*/ B           JPEGE_GetBitStream_x2664
    JPEGE_GetBitStream_x29dc:
    /*000029dc*/ LDR         IP, [R5, #8] 
    /*000029e0*/ CMP         R1, IP 
    /*000029e4*/ BCC         JPEGE_GetBitStream_x2a24
    /*000029e8*/ SUB         R1, R1, IP 
    /*000029ec*/ CMP         R8, R1 
    /*000029f0*/ BCS         JPEGE_GetBitStream_x2854
    JPEGE_GetBitStream_x29f4:
    /*000029f4*/ LDR         R3, [SP, #12] 
    /*000029f8*/ MOVW        R2, #1656                   @ 0x678 
    /*000029fc*/ LDR         R1, [PC, #864]              @ 0x0000000000002d64 
    /*00002a00*/ LDR         R0, [SP, #16] 
    /*00002a04*/ BL          0x0000000000002A04          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002a08*/ B           JPEGE_GetBitStream_x2894
    JPEGE_GetBitStream_x2a0c:
    /*00002a0c*/ LDR         R2, [SP, #8] 
    /*00002a10*/ CMP         R2, #0 
    /*00002a14*/ LDR         R2, [R1] 
    /*00002a18*/ ADDEQ       R4, R4, R2 
    /*00002a1c*/ ADDNE       R6, R6, R2 
    /*00002a20*/ B           JPEGE_GetBitStream_x28c0
    JPEGE_GetBitStream_x2a24:
    /*00002a24*/ ADD         R0, R3, R0 
    /*00002a28*/ MOV         IP, #0 
    /*00002a2c*/ STR         R0, [SL] 
    /*00002a30*/ ADD         R0, R0, LR 
    /*00002a34*/ STR         LR, [SL, #24] 
    /*00002a38*/ STR         R0, [SL, #4] 
    /*00002a3c*/ STR         IP, [SL, #28] 
    /*00002a40*/ B           JPEGE_GetBitStream_x286c
    JPEGE_GetBitStream_x2a44:
    /*00002a44*/ MOVW        R0, #32770                  @ 0x8002 
    /*00002a48*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00002a4c*/ BX          LR 
    JPEGE_GetBitStream_x2a50:
    /*00002a50*/ MOV         R3, #1632                   @ 0x660 
    /*00002a54*/ MOV         R1, #11 
    /*00002a58*/ STR         R3, [SP] 
    /*00002a5c*/ MOVW        R2, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8d4] 
    /*00002a60*/ LDR         R3, [PC, #764]              @ 0x0000000000002d64 
    /*00002a64*/ MOVT        R2, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8d4] 
    /*00002a68*/ MOV         R0, #0 
    /*00002a6c*/ BL          0x0000000000002A6C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002a70*/ MOVW        R0, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x90c] 
    /*00002a74*/ LDR         R2, [R9, #12] 
    /*00002a78*/ MOVT        R0, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x90c] 
    /*00002a7c*/ MOV         R1, R7 
    /*00002a80*/ BL          0x0000000000002A80          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00002a84*/ LDR         R3, [R9, #12] 
    /*00002a88*/ CMP         R7, R3 
    /*00002a8c*/ BEQ         JPEGE_GetBitStream_x2aac
    /*00002a90*/ MOVW        R3, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x958] 
    /*00002a94*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00002a98*/ MOVT        R3, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x958] 
    /*00002a9c*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00002aa0*/ MOVW        R2, #1635                   @ 0x663 
    /*00002aa4*/ LDR         R1, [PC, #696]              @ 0x0000000000002d64 
    /*00002aa8*/ BL          0x0000000000002AA8          @ osal_panic [R_ARM_CALL: +0x0] 
    JPEGE_GetBitStream_x2aac:
    /*00002aac*/ MOV         R0, R6 
    /*00002ab0*/ ADD         R1, SP, #36                 @ 0x24 
    /*00002ab4*/ BL          0x0000000000002AB4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002ab8*/ MOVW        R0, #32771                  @ 0x8003 
    /*00002abc*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00002ac0*/ B           JPEGE_GetBitStream_x2794
    JPEGE_GetBitStream_x2ac4:
    /*00002ac4*/ CMP         R0, #2 
    /*00002ac8*/ MOV         R9, R4 
    /*00002acc*/ MOV         SL, R6 
    /*00002ad0*/ LDR         R8, [SP, #20] 
    /*00002ad4*/ LDR         R6, [SP, #24] 
    /*00002ad8*/ LDR         R4, [SP, #28] 
    /*00002adc*/ BEQ         JPEGE_GetBitStream_x2b0c
    /*00002ae0*/ LDR         R1, [PC, #636]              @ 0x0000000000002d64 
    /*00002ae4*/ MOVW        R3, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x984] 
    /*00002ae8*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00002aec*/ MOVT        R3, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x984] 
    /*00002af0*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00002af4*/ MOVW        R2, #1678                   @ 0x68e 
    /*00002af8*/ BL          0x0000000000002AF8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002afc*/ ADD         R3, SP, #184                @ 0xb8 
    /*00002b00*/ ADD         R3, R3, FP, LSL #2 
    /*00002b04*/ LDR         R1, [R3, #-144]             @ 0xffffff70 
    /*00002b08*/ LDRB        R3, [R1, #8] 
    JPEGE_GetBitStream_x2b0c:
    /*00002b0c*/ SUB         R0, R3, #6 
    /*00002b10*/ LDRB        R2, [R1, #11] 
    /*00002b14*/ TST         R0, #253                    @ 0xfd 
    /*00002b18*/ LDR         R0, [SP, #40]               @ 0x28 
    /*00002b1c*/ MOVNE       R3, #5 
    /*00002b20*/ STR         R3, [R4, #28] 
    /*00002b24*/ LDR         R3, [SP, #8] 
    /*00002b28*/ STR         R2, [R4, #24] 
    /*00002b2c*/ CMP         R3, #1 
    /*00002b30*/ LDRD        R2, [R1, #16] 
    /*00002b34*/ STRD        R2, [R4, #16] 
    /*00002b38*/ LDR         R3, [R0, #4] 
    /*00002b3c*/ ADD         R3, R3, #64                 @ 0x40 
    /*00002b40*/ STR         R3, [R4, #32] 
    /*00002b44*/ BEQ         JPEGE_GetBitStream_x2c98
    /*00002b48*/ LDRD        R2, [SP, #64]               @ 0x40 
    /*00002b4c*/ LDR         IP, [SP, #56]               @ 0x38 
    /*00002b50*/ STRD        R2, [R4] 
    /*00002b54*/ STR         IP, [R4, #8] 
    /*00002b58*/ LDR         R3, [R1, #4] 
    /*00002b5c*/ SUB         R3, R9, R3 
    /*00002b60*/ STR         R3, [R4, #12] 
    JPEGE_GetBitStream_x2b64:
    /*00002b64*/ LDR         R2, [R0, #4] 
    /*00002b68*/ ADD         R3, SP, #184                @ 0xb8 
    /*00002b6c*/ LDR         R0, [R0] 
    /*00002b70*/ ADD         FP, R3, FP, LSL #5 
    /*00002b74*/ LDR         R9, [R7, #3708]             @ 0xe7c 
    /*00002b78*/ SUB         R0, R0, R2 
    /*00002b7c*/ LDR         SL, [R1] 
    /*00002b80*/ LDR         LR, [R7, #3764]             @ 0xeb4 
    /*00002b84*/ SUB         R3, R0, #128                @ 0x80 
    /*00002b88*/ LDR         R1, [R1, #4] 
    /*00002b8c*/ ADD         R3, R3, R9 
    /*00002b90*/ LDR         IP, [FP, #-128]             @ 0xffffff80 
    /*00002b94*/ ADD         R3, R3, SL 
    /*00002b98*/ SUB         R9, R0, #64                 @ 0x40 
    /*00002b9c*/ CMP         LR, #0 
    /*00002ba0*/ SUB         R3, R3, R1 
    /*00002ba4*/ RSB         R1, R9, #64                 @ 0x40 
    /*00002ba8*/ LDR         FP, [R5, #176]              @ 0xb0 
    /*00002bac*/ LDR         SL, [R5, #8] 
    /*00002bb0*/ STR         R3, [R7, #3708]             @ 0xe7c 
    /*00002bb4*/ ADD         R7, IP, R1 
    /*00002bb8*/ BNE         JPEGE_GetBitStream_x2c14
    /*00002bbc*/ LDR         R3, [R5, #176]              @ 0xb0 
    /*00002bc0*/ CMP         R7, R3 
    /*00002bc4*/ BCS         JPEGE_GetBitStream_x2cbc
    /*00002bc8*/ SUB         SL, R3, R7 
    /*00002bcc*/ LDR         R0, [R5, #8] 
    /*00002bd0*/ ADD         R1, R8, #520                @ 0x208 
    /*00002bd4*/ MOV         R2, SL 
    /*00002bd8*/ ADD         R0, R7, R0 
    /*00002bdc*/ BL          0x0000000000002BDC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002be0*/ ADD         R1, SL, #520                @ 0x208 
    /*00002be4*/ LDR         R0, [R5, #176]              @ 0xb0 
    /*00002be8*/ ADD         R1, R8, R1 
    /*00002bec*/ SUB         R2, R9, SL 
    /*00002bf0*/ BL          0x0000000000002BF0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    JPEGE_GetBitStream_x2bf4:
    /*00002bf4*/ MOV         R3, #8 
    /*00002bf8*/ MOV         R2, #1 
    /*00002bfc*/ STR         R3, [R4, #40]               @ 0x28 
    /*00002c00*/ MOV         R3, #0 
    /*00002c04*/ STR         R9, [R4, #48]               @ 0x30 
    /*00002c08*/ STR         R2, [R4, #36]               @ 0x24 
    /*00002c0c*/ STR         R3, [R4, #44]               @ 0x2c 
    /*00002c10*/ B           JPEGE_GetBitStream_x2784
    JPEGE_GetBitStream_x2c14:
    /*00002c14*/ RSB         R8, R0, #64                 @ 0x40 
    /*00002c18*/ SUB         R8, R8, R2 
    /*00002c1c*/ ADD         R8, IP, R8 
    /*00002c20*/ CMP         R7, R8 
    /*00002c24*/ BLS         JPEGE_GetBitStream_x2d44
    JPEGE_GetBitStream_x2c28:
    /*00002c28*/ LDR         R1, [R5, #176]              @ 0xb0 
    /*00002c2c*/ CMP         R7, R1 
    /*00002c30*/ BCC         JPEGE_GetBitStream_x2ce4
    /*00002c34*/ CMP         R8, R1 
    /*00002c38*/ BCS         JPEGE_GetBitStream_x2cd0
    /*00002c3c*/ SUB         R8, R1, R8 
    /*00002c40*/ SUB         R2, R9, R8 
    /*00002c44*/ ADD         R0, R7, R8 
    /*00002c48*/ BL          0x0000000000002C48          @ osal_memmove [R_ARM_CALL: +0x0] 
    /*00002c4c*/ LDR         R1, [R5, #176]              @ 0xb0 
    /*00002c50*/ LDR         R3, [R5, #8] 
    /*00002c54*/ MOV         R2, R8 
    /*00002c58*/ MOV         R0, R7 
    /*00002c5c*/ ADD         R1, R1, R3 
    /*00002c60*/ BL          0x0000000000002C60          @ osal_memmove [R_ARM_CALL: +0x0] 
    /*00002c64*/ B           JPEGE_GetBitStream_x2bf4
    JPEGE_GetBitStream_x2c68:
    /*00002c68*/ MOVW        R0, #32774                  @ 0x8006 
    /*00002c6c*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00002c70*/ BX          LR 
    JPEGE_GetBitStream_x2c74:
    /*00002c74*/ MOVW        R0, #32773                  @ 0x8005 
    /*00002c78*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00002c7c*/ B           JPEGE_GetBitStream_x2794
    JPEGE_GetBitStream_x2c80:
    /*00002c80*/ MOV         R0, R6 
    /*00002c84*/ ADD         R1, SP, #36                 @ 0x24 
    /*00002c88*/ BL          0x0000000000002C88          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002c8c*/ MOVW        R0, #32782                  @ 0x800e 
    /*00002c90*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00002c94*/ B           JPEGE_GetBitStream_x2794
    JPEGE_GetBitStream_x2c98:
    /*00002c98*/ LDRD        R2, [SP, #64]               @ 0x40 
    /*00002c9c*/ ADD         IP, SL, R9 
    /*00002ca0*/ LDR         LR, [SP, #56]               @ 0x38 
    /*00002ca4*/ STRD        R2, [R4] 
    /*00002ca8*/ STR         LR, [R4, #8] 
    /*00002cac*/ LDR         R3, [R1, #4] 
    /*00002cb0*/ SUB         R3, IP, R3 
    /*00002cb4*/ STR         R3, [R4, #12] 
    /*00002cb8*/ B           JPEGE_GetBitStream_x2b64
    JPEGE_GetBitStream_x2cbc:
    /*00002cbc*/ ADD         R1, R8, #520                @ 0x208 
    /*00002cc0*/ MOV         R0, R7 
    /*00002cc4*/ MOV         R2, R9 
    /*00002cc8*/ BL          0x0000000000002CC8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002ccc*/ B           JPEGE_GetBitStream_x2bf4
    JPEGE_GetBitStream_x2cd0:
    /*00002cd0*/ MOV         R1, R8 
    /*00002cd4*/ MOV         R0, R7 
    /*00002cd8*/ MOV         R2, R9 
    /*00002cdc*/ BL          0x0000000000002CDC          @ osal_memmove [R_ARM_CALL: +0x0] 
    /*00002ce0*/ B           JPEGE_GetBitStream_x2bf4
    JPEGE_GetBitStream_x2ce4:
    /*00002ce4*/ LDR         R3, [SP, #40]               @ 0x28 
    /*00002ce8*/ SUB         R2, R1, R7 
    /*00002cec*/ STR         R2, [SP, #8] 
    /*00002cf0*/ SUB         R2, R9, R2 
    /*00002cf4*/ LDR         R3, [R3, #4] 
    /*00002cf8*/ ADD         R0, R3, #64                 @ 0x40 
    /*00002cfc*/ STR         R0, [SP, #12] 
    /*00002d00*/ SUB         R2, R2, R0 
    /*00002d04*/ ADD         R0, R1, R0 
    /*00002d08*/ STR         R3, [SP, #16] 
    /*00002d0c*/ BL          0x0000000000002D0C          @ osal_memmove [R_ARM_CALL: +0x0] 
    /*00002d10*/ LDR         R3, [SP, #16] 
    /*00002d14*/ LDR         R2, [SP, #12] 
    /*00002d18*/ SUB         R3, SL, R3 
    /*00002d1c*/ LDR         R0, [R5, #176]              @ 0xb0 
    /*00002d20*/ SUB         R1, R3, #64                 @ 0x40 
    /*00002d24*/ ADD         R1, FP, R1 
    /*00002d28*/ BL          0x0000000000002D28          @ osal_memmove [R_ARM_CALL: +0x0] 
    /*00002d2c*/ LDR         R0, [R5, #8] 
    /*00002d30*/ LDR         R2, [SP, #8] 
    /*00002d34*/ ADD         R1, R8, R0 
    /*00002d38*/ ADD         R0, R7, R0 
    /*00002d3c*/ BL          0x0000000000002D3C          @ osal_memmove [R_ARM_CALL: +0x0] 
    /*00002d40*/ B           JPEGE_GetBitStream_x2bf4
    JPEGE_GetBitStream_x2d44:
    /*00002d44*/ MOVW        R3, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x994] 
    /*00002d48*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00002d4c*/ MOVT        R3, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x994] 
    /*00002d50*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00002d54*/ MOVW        R2, #1737                   @ 0x6c9 
    /*00002d58*/ LDR         R1, [PC, #4]                @ 0x0000000000002d64 
    /*00002d5c*/ BL          0x0000000000002D5C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002d60*/ B           JPEGE_GetBitStream_x2c28
    /*00002d64*/ LDRDEQ      R0, [R0], -R4 
FUNC_END JPEGE_GetBitStream


FUNC_BEGIN JPEGE_UpdateUniqueID
    /*00002d68*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00002d6c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002d70*/ LDR         LR, [R0, #3984]             @ 0xf90 
    /*00002d74*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002d78*/ ADD         IP, R0, #4096               @ 0x1000 
    /*00002d7c*/ MOV         R0, #0 
    /*00002d80*/ LDR         R2, [R3, LR, LSL #2] 
    /*00002d84*/ ADD         R1, R1, R2 
    /*00002d88*/ STR         R1, [IP, #3712]             @ 0xe80 
    /*00002d8c*/ STR         R1, [R3, LR, LSL #2] 
    /*00002d90*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END JPEGE_UpdateUniqueID


FUNC_BEGIN JpegeCheckRollbackStreamBuffer
    /*00002d94*/ ADD         R0, R0, #4096               @ 0x1000 
    /*00002d98*/ LDRB        R3, [R0, #3804]             @ 0xedc 
    /*00002d9c*/ CMP         R3, #0 
    /*00002da0*/ BNE         JpegeCheckRollbackStreamBuffer_x2dc0
    /*00002da4*/ LDR         R3, [R0, #3764]             @ 0xeb4 
    /*00002da8*/ CMP         R3, #1 
    /*00002dac*/ BEQ         JpegeCheckRollbackStreamBuffer_x2e00
    JpegeCheckRollbackStreamBuffer_x2db0:
    /*00002db0*/ LDR         R0, [R0, #3800]             @ 0xed8 
    /*00002db4*/ ADDS        R0, R0, #0 
    /*00002db8*/ MOVNE       R0, #1 
    /*00002dbc*/ BX          LR 
    JpegeCheckRollbackStreamBuffer_x2dc0:
    /*00002dc0*/ CMP         R3, #1 
    /*00002dc4*/ BEQ         JpegeCheckRollbackStreamBuffer_x2db0
    /*00002dc8*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00002dcc*/ SUB         SP, SP, #12 
    /*00002dd0*/ MOV         R1, #11 
    /*00002dd4*/ MOVW        R2, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa6c] 
    /*00002dd8*/ STR         R3, [SP, #4] 
    /*00002ddc*/ MOVT        R2, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa6c] 
    /*00002de0*/ MOVW        R3, #2050                   @ 0x802 
    /*00002de4*/ MOV         R0, #0 
    /*00002de8*/ STR         R3, [SP] 
    /*00002dec*/ LDR         R3, [PC, #28]               @ 0x0000000000002e10 
    /*00002df0*/ BL          0x0000000000002DF0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002df4*/ MOV         R0, #0 
    /*00002df8*/ ADD         SP, SP, #12 
    /*00002dfc*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    JpegeCheckRollbackStreamBuffer_x2e00:
    /*00002e00*/ LDR         R0, [R0, #3800]             @ 0xed8 
    /*00002e04*/ SUBS        R0, R0, #3 
    /*00002e08*/ MOVNE       R0, #1 
    /*00002e0c*/ BX          LR 
    /*00002e10*/ ANDEQ       R0, R0, R8, ROR #3 
FUNC_END JpegeCheckRollbackStreamBuffer


FUNC_BEGIN JpegeCheckEncodeFinish
    /*00002e14*/ ADD         R0, R0, #4096               @ 0x1000 
    /*00002e18*/ LDRB        R3, [R0, #3804]             @ 0xedc 
    /*00002e1c*/ CMP         R3, #0 
    /*00002e20*/ BNE         JpegeCheckEncodeFinish_x2e44
    /*00002e24*/ LDR         R2, [R0, #3796]             @ 0xed4 
    /*00002e28*/ CMP         R2, #1 
    /*00002e2c*/ BLS         JpegeCheckEncodeFinish_x2ea0
    JpegeCheckEncodeFinish_x2e30:
    /*00002e30*/ LDR         R0, [R0, #3800]             @ 0xed8 
    /*00002e34*/ SUB         R0, R0, #2 
    /*00002e38*/ CLZ         R0, R0 
    /*00002e3c*/ LSR         R0, R0, #5 
    /*00002e40*/ BX          LR 
    JpegeCheckEncodeFinish_x2e44:
    /*00002e44*/ LDR         R3, [R0, #3764]             @ 0xeb4 
    /*00002e48*/ CMP         R3, #1 
    /*00002e4c*/ LDR         R3, [R0, #3796]             @ 0xed4 
    /*00002e50*/ BEQ         JpegeCheckEncodeFinish_x2e78
    /*00002e54*/ CMP         R3, #1 
    /*00002e58*/ BHI         JpegeCheckEncodeFinish_x2e30
    /*00002e5c*/ BEQ         JpegeCheckEncodeFinish_x2ea4
    /*00002e60*/ CMP         R3, #0 
    /*00002e64*/ BNE         JpegeCheckEncodeFinish_x2ec0
    JpegeCheckEncodeFinish_x2e68:
    /*00002e68*/ LDR         R0, [R0, #3800]             @ 0xed8 
    /*00002e6c*/ CLZ         R0, R0 
    /*00002e70*/ LSR         R0, R0, #5 
    /*00002e74*/ BX          LR 
    JpegeCheckEncodeFinish_x2e78:
    /*00002e78*/ CMP         R3, #1 
    /*00002e7c*/ BHI         JpegeCheckEncodeFinish_x2e30
    /*00002e80*/ BEQ         JpegeCheckEncodeFinish_x2ea4
    /*00002e84*/ CMP         R3, #0 
    /*00002e88*/ BNE         JpegeCheckEncodeFinish_x2ec0
    /*00002e8c*/ LDR         R0, [R0, #3800]             @ 0xed8 
    /*00002e90*/ SUB         R0, R0, #3 
    /*00002e94*/ CLZ         R0, R0 
    /*00002e98*/ LSR         R0, R0, #5 
    /*00002e9c*/ BX          LR 
    JpegeCheckEncodeFinish_x2ea0:
    /*00002ea0*/ BNE         JpegeCheckEncodeFinish_x2eb8
    JpegeCheckEncodeFinish_x2ea4:
    /*00002ea4*/ LDR         R0, [R0, #3800]             @ 0xed8 
    /*00002ea8*/ SUB         R0, R0, #1 
    /*00002eac*/ CLZ         R0, R0 
    /*00002eb0*/ LSR         R0, R0, #5 
    /*00002eb4*/ BX          LR 
    JpegeCheckEncodeFinish_x2eb8:
    /*00002eb8*/ CMP         R2, #0 
    /*00002ebc*/ BEQ         JpegeCheckEncodeFinish_x2e68
    JpegeCheckEncodeFinish_x2ec0:
    /*00002ec0*/ MOV         R0, #0 
    /*00002ec4*/ BX          LR 
FUNC_END JpegeCheckEncodeFinish


FUNC_BEGIN JPEGE_StartOneFrame
    /*00002ec8*/ CMP         R0, #15 
    /*00002ecc*/ BHI         JPEGE_StartOneFrame_x3460
    /*00002ed0*/ CMP         R1, #0 
    /*00002ed4*/ BEQ         JPEGE_StartOneFrame_x3478
    /*00002ed8*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00002edc*/ SUB         SP, SP, #20 
    /*00002ee0*/ MOV         R5, R1 
    /*00002ee4*/ BL          0x0000000000002EE4          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00002ee8*/ SUBS        R4, R0, #0 
    /*00002eec*/ BEQ         JPEGE_StartOneFrame_x346c
    /*00002ef0*/ LDR         R3, [R5, #8] 
    /*00002ef4*/ ADD         R6, R4, #4096               @ 0x1000 
    /*00002ef8*/ MOV         R2, #0 
    /*00002efc*/ ADD         R8, R4, #6912               @ 0x1b00 
    /*00002f00*/ STR         R2, [R5] 
    /*00002f04*/ ADD         R0, R4, #7808               @ 0x1e80 
    /*00002f08*/ STR         R3, [R6, #3764]             @ 0xeb4 
    /*00002f0c*/ ADD         R8, R8, #52                 @ 0x34 
    /*00002f10*/ LDR         R1, [R5, #12] 
    /*00002f14*/ MOV         R2, #40                     @ 0x28 
    /*00002f18*/ ADD         R0, R0, #8 
    /*00002f1c*/ ADD         R1, R1, #400                @ 0x190 
    /*00002f20*/ BL          0x0000000000002F20          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002f24*/ MOV         R0, R8 
    /*00002f28*/ ADD         R1, SP, #12 
    /*00002f2c*/ BL          0x0000000000002F2C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00002f30*/ LDR         R3, [R4, #3924]             @ 0xf54 
    /*00002f34*/ ADD         R3, R3, #1 
    /*00002f38*/ STR         R3, [R4, #3924]             @ 0xf54 
    /*00002f3c*/ LDR         R3, [R5, #36]               @ 0x24 
    /*00002f40*/ STR         R3, [R6, #3800]             @ 0xed8 
    /*00002f44*/ LDR         R2, [R5, #36]               @ 0x24 
    /*00002f48*/ STR         R2, [R6, #2824]             @ 0xb08 
    /*00002f4c*/ LDRB        R2, [R5, #40]               @ 0x28 
    /*00002f50*/ CMP         R2, #0 
    /*00002f54*/ STRB        R2, [R6, #3804]             @ 0xedc 
    /*00002f58*/ LDR         R2, [R5, #12] 
    /*00002f5c*/ LDR         R2, [R2, #384]              @ 0x180 
    /*00002f60*/ STR         R2, [R6, #3796]             @ 0xed4 
    /*00002f64*/ BEQ         JPEGE_StartOneFrame_x31dc
    /*00002f68*/ CMP         R3, #0 
    /*00002f6c*/ BNE         JPEGE_StartOneFrame_x2f84
    JPEGE_StartOneFrame_x2f70:
    /*00002f70*/ STR         R3, [R6, #3772]             @ 0xebc 
    /*00002f74*/ STR         R3, [R6, #3780]             @ 0xec4 
    /*00002f78*/ STR         R3, [R6, #3784]             @ 0xec8 
    /*00002f7c*/ STR         R3, [R6, #3788]             @ 0xecc 
    /*00002f80*/ STR         R3, [R6, #3792]             @ 0xed0 
    JPEGE_StartOneFrame_x2f84:
    /*00002f84*/ LDR         R3, [R5, #12] 
    /*00002f88*/ LDR         R7, [R4, #452]              @ 0x1c4 
    /*00002f8c*/ LDR         R3, [R3, #12] 
    /*00002f90*/ CMP         R3, #26 
    /*00002f94*/ MOVEQ       R3, #0 
    /*00002f98*/ STREQ       R3, [R4, #3968]             @ 0xf80 
    /*00002f9c*/ BEQ         JPEGE_StartOneFrame_x2fdc
    /*00002fa0*/ CMP         R3, #25 
    /*00002fa4*/ MOVEQ       R3, #1 
    /*00002fa8*/ STREQ       R3, [R4, #3968]             @ 0xf80 
    /*00002fac*/ BEQ         JPEGE_StartOneFrame_x2fdc
    /*00002fb0*/ CMP         R3, #39                     @ 0x27 
    /*00002fb4*/ MOVEQ       R3, #2 
    /*00002fb8*/ STREQ       R3, [R4, #3968]             @ 0xf80 
    /*00002fbc*/ BEQ         JPEGE_StartOneFrame_x2fdc
    /*00002fc0*/ MOVW        R3, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8d0] 
    /*00002fc4*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00002fc8*/ MOVT        R3, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8d0] 
    /*00002fcc*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00002fd0*/ MOVW        R2, #2181                   @ 0x885 
    /*00002fd4*/ LDR         R1, [PC, #1316]             @ 0x0000000000003500 
    /*00002fd8*/ BL          0x0000000000002FD8          @ osal_panic [R_ARM_CALL: +0x0] 
    JPEGE_StartOneFrame_x2fdc:
    /*00002fdc*/ ADD         R1, SP, #12 
    /*00002fe0*/ MOV         R0, R8 
    /*00002fe4*/ BL          0x0000000000002FE4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002fe8*/ LDR         R3, [R5, #12] 
    /*00002fec*/ LDR         R1, [R4, #36]               @ 0x24 
    /*00002ff0*/ LDR         R2, [R3] 
    /*00002ff4*/ CMP         R1, R2 
    /*00002ff8*/ BEQ         JPEGE_StartOneFrame_x3360
    /*00002ffc*/ LDR         R3, [R6, #3800]             @ 0xed8 
    /*00003000*/ CMP         R3, #0 
    /*00003004*/ BEQ         JPEGE_StartOneFrame_x337c
    JPEGE_StartOneFrame_x3008:
    /*00003008*/ ADD         R1, SP, #12 
    /*0000300c*/ MOV         R0, R8 
    /*00003010*/ BL          0x0000000000003010          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00003014*/ MOV         R0, R4 
    /*00003018*/ BL          0x0000000000003018          @ JpegeValidUserParam [R_ARM_CALL: .text+0x1148] 
    /*0000301c*/ LDR         R3, [R5, #12] 
    /*00003020*/ MOV         R1, R5 
    /*00003024*/ MOV         R0, R4 
    /*00003028*/ LDR         R2, [R3, #336]              @ 0x150 
    /*0000302c*/ CMP         R2, #1 
    /*00003030*/ LDREQ       R3, [R3, #380]              @ 0x17c 
    /*00003034*/ MOVNE       R3, #0 
    /*00003038*/ ADDEQ       R3, R3, #8 
    /*0000303c*/ STR         R3, [R6, #2864]             @ 0xb30 
    /*00003040*/ LDR         R3, [R4, #16] 
    /*00003044*/ CMP         R3, #0 
    /*00003048*/ BEQ         JPEGE_StartOneFrame_x32e4
    /*0000304c*/ BL          0x000000000000304C          @ JpegePicStartCtrl [R_ARM_CALL: .text+0x5ae0] 
    /*00003050*/ CMP         R0, #0 
    /*00003054*/ BNE         JPEGE_StartOneFrame_x34d4
    JPEGE_StartOneFrame_x3058:
    /*00003058*/ LDRB        R3, [R6, #3804]             @ 0xedc 
    /*0000305c*/ CMP         R3, #1 
    /*00003060*/ BEQ         JPEGE_StartOneFrame_x33f8
    JPEGE_StartOneFrame_x3064:
    /*00003064*/ LDR         R2, [R7, #168]              @ 0xa8 
    /*00003068*/ ADD         LR, R4, #456                @ 0x1c8 
    /*0000306c*/ LDR         R3, [R7, #164]              @ 0xa4 
    /*00003070*/ LDR         IP, [R2] 
    /*00003074*/ LDR         R3, [R3] 
    /*00003078*/ CMP         R3, IP 
    /*0000307c*/ SUBHI       R3, R3, IP 
    /*00003080*/ LDRLS       R1, [R7, #12] 
    /*00003084*/ LDRLS       R2, [R7, #8] 
    /*00003088*/ SUBLS       R3, R3, R1 
    /*0000308c*/ LDRHI       R2, [R7, #12] 
    /*00003090*/ ADDLS       R3, R3, R2 
    /*00003094*/ SUBLS       R3, R3, IP 
    /*00003098*/ SUBHI       R3, R3, R2 
    /*0000309c*/ CMP         R3, #63                     @ 0x3f 
    /*000030a0*/ BLS         JPEGE_StartOneFrame_x3484
    /*000030a4*/ LDR         R3, [R7, #8] 
    /*000030a8*/ ADD         R9, IP, #64                 @ 0x40 
    /*000030ac*/ CMP         R9, R3 
    /*000030b0*/ BCC         JPEGE_StartOneFrame_x333c
    /*000030b4*/ SUB         FP, R3, IP 
    /*000030b8*/ LDR         R3, [R7, #176]              @ 0xb0 
    /*000030bc*/ RSB         R9, FP, #64                 @ 0x40 
    /*000030c0*/ LSR         FP, FP, #3 
    /*000030c4*/ ADD         IP, R3, IP 
    /*000030c8*/ CMP         FP, #0 
    /*000030cc*/ STR         R3, [SP, #4] 
    /*000030d0*/ LSR         R3, R9, #3 
    /*000030d4*/ SUB         FP, FP, #1 
    /*000030d8*/ STR         R3, [SP] 
    /*000030dc*/ SUB         R3, R3, #1 
    /*000030e0*/ BEQ         JPEGE_StartOneFrame_x310c
    JPEGE_StartOneFrame_x30e4:
    /*000030e4*/ SUB         IP, IP, #8 
    /*000030e8*/ MOV         R2, FP 
    /*000030ec*/ MOV         SL, LR 
    JPEGE_StartOneFrame_x30f0:
    /*000030f0*/ SUB         R2, R2, #1 
    /*000030f4*/ LDRD        R0, [SL], #8 
    /*000030f8*/ CMN         R2, #1 
    /*000030fc*/ STRD        R0, [IP, #8]! 
    /*00003100*/ BNE         JPEGE_StartOneFrame_x30f0
    /*00003104*/ ADD         FP, FP, #1 
    /*00003108*/ ADD         LR, LR, FP, LSL #3 
    JPEGE_StartOneFrame_x310c:
    /*0000310c*/ LDR         R2, [SP] 
    /*00003110*/ CMP         R2, #0 
    /*00003114*/ LDRNE       R2, [SP, #4] 
    /*00003118*/ SUBNE       R2, R2, #8 
    /*0000311c*/ BEQ         JPEGE_StartOneFrame_x3134
    JPEGE_StartOneFrame_x3120:
    /*00003120*/ SUB         R3, R3, #1 
    /*00003124*/ LDRD        R0, [LR], #8 
    /*00003128*/ CMN         R3, #1 
    /*0000312c*/ STRD        R0, [R2, #8]! 
    /*00003130*/ BNE         JPEGE_StartOneFrame_x3120
    JPEGE_StartOneFrame_x3134:
    /*00003134*/ LDR         R3, [R7, #168]              @ 0xa8 
    /*00003138*/ STR         R9, [R3] 
    JPEGE_StartOneFrame_x313c:
    /*0000313c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003140*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003144*/ LDR         R3, [R3, #68]               @ 0x44 
    /*00003148*/ CMP         R3, #0 
    /*0000314c*/ BEQ         JPEGE_StartOneFrame_x31fc
    /*00003150*/ LDR         R3, [R5, #8] 
    /*00003154*/ CMP         R3, #0 
    /*00003158*/ BNE         JPEGE_StartOneFrame_x31fc
    /*0000315c*/ LDR         R2, [R7, #164]              @ 0xa4 
    /*00003160*/ LDR         R1, [R7, #168]              @ 0xa8 
    /*00003164*/ LDR         R3, [R4, #456]              @ 0x1c8 
    /*00003168*/ LDR         R2, [R2] 
    /*0000316c*/ LDR         R1, [R1] 
    /*00003170*/ SUB         R9, R3, #64                 @ 0x40 
    /*00003174*/ LDR         R4, [R7, #128]              @ 0x80 
    /*00003178*/ CMP         R2, R1 
    /*0000317c*/ SUBHI       R2, R2, R1 
    /*00003180*/ LDRLS       R0, [R7, #12] 
    /*00003184*/ LDRLS       R3, [R7, #8] 
    /*00003188*/ SUBLS       R2, R2, R0 
    /*0000318c*/ LDRHI       R3, [R7, #12] 
    /*00003190*/ ADDLS       R2, R2, R3 
    /*00003194*/ SUBLS       R2, R2, R1 
    /*00003198*/ SUBHI       R2, R2, R3 
    /*0000319c*/ CMP         R9, R2 
    /*000031a0*/ BCS         JPEGE_StartOneFrame_x33d8
    JPEGE_StartOneFrame_x31a4:
    /*000031a4*/ LDR         R2, [R7, #8] 
    /*000031a8*/ ADD         R3, R4, R9 
    /*000031ac*/ CMP         R3, R2 
    /*000031b0*/ SUBCS       R3, R3, R2 
    /*000031b4*/ STR         R3, [R7, #128]              @ 0x80 
    JPEGE_StartOneFrame_x31b8:
    /*000031b8*/ STR         R3, [R6, #3700]             @ 0xe74 
    /*000031bc*/ MOV         R0, R8 
    /*000031c0*/ MOV         R3, #1 
    /*000031c4*/ ADD         R1, SP, #12 
    /*000031c8*/ STR         R3, [R5] 
    /*000031cc*/ BL          0x00000000000031CC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000031d0*/ MOV         R0, #0 
    JPEGE_StartOneFrame_x31d4:
    /*000031d4*/ ADD         SP, SP, #20 
    /*000031d8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JPEGE_StartOneFrame_x31dc:
    /*000031dc*/ CMP         R3, #3 
    /*000031e0*/ BEQ         JPEGE_StartOneFrame_x31f4
    /*000031e4*/ LDR         R2, [R6, #3764]             @ 0xeb4 
    /*000031e8*/ CMP         R2, #0 
    /*000031ec*/ CMPEQ       R3, #0 
    /*000031f0*/ BNE         JPEGE_StartOneFrame_x2f84
    JPEGE_StartOneFrame_x31f4:
    /*000031f4*/ MOV         R3, #0 
    /*000031f8*/ B           JPEGE_StartOneFrame_x2f70
    JPEGE_StartOneFrame_x31fc:
    /*000031fc*/ LDR         R3, [R4, #456]              @ 0x1c8 
    /*00003200*/ ADD         R4, R4, #520                @ 0x208 
    /*00003204*/ LDR         R2, [R7, #164]              @ 0xa4 
    /*00003208*/ SUB         R3, R3, #64                 @ 0x40 
    /*0000320c*/ LDR         R1, [R7, #168]              @ 0xa8 
    /*00003210*/ ANDS        FP, R3, #3 
    /*00003214*/ LDR         R2, [R2] 
    /*00003218*/ LDR         IP, [R1] 
    /*0000321c*/ BNE         JPEGE_StartOneFrame_x34b8
    /*00003220*/ CMP         R2, IP 
    /*00003224*/ SUBHI       R2, R2, IP 
    /*00003228*/ LDRLS       LR, [R7, #12] 
    /*0000322c*/ LDRLS       R0, [R7, #8] 
    /*00003230*/ SUBLS       R2, R2, LR 
    /*00003234*/ LDRHI       R0, [R7, #12] 
    /*00003238*/ ADDLS       R2, R2, R0 
    /*0000323c*/ SUBLS       R2, R2, IP 
    /*00003240*/ SUBHI       R2, R2, R0 
    /*00003244*/ CMP         R3, R2 
    /*00003248*/ BHI         JPEGE_StartOneFrame_x349c
    /*0000324c*/ LDR         R2, [R7, #8] 
    /*00003250*/ ADD         R9, R3, IP 
    /*00003254*/ CMP         R9, R2 
    /*00003258*/ BCC         JPEGE_StartOneFrame_x33b0
    /*0000325c*/ SUB         SL, R2, IP 
    /*00003260*/ LDR         R2, [R7, #176]              @ 0xb0 
    /*00003264*/ SUB         R9, R3, SL 
    /*00003268*/ LSR         SL, SL, #3 
    /*0000326c*/ ADD         IP, R2, IP 
    /*00003270*/ CMP         SL, #0 
    /*00003274*/ LSR         FP, R9, #3 
    /*00003278*/ STR         R2, [SP] 
    /*0000327c*/ SUB         R1, FP, #1 
    /*00003280*/ SUB         SL, SL, #1 
    /*00003284*/ BEQ         JPEGE_StartOneFrame_x32b0
    JPEGE_StartOneFrame_x3288:
    /*00003288*/ SUB         IP, IP, #8 
    /*0000328c*/ MOV         R0, SL 
    /*00003290*/ MOV         LR, R4 
    JPEGE_StartOneFrame_x3294:
    /*00003294*/ SUB         R0, R0, #1 
    /*00003298*/ LDRD        R2, [LR], #8 
    /*0000329c*/ CMN         R0, #1 
    /*000032a0*/ STRD        R2, [IP, #8]! 
    /*000032a4*/ BNE         JPEGE_StartOneFrame_x3294
    /*000032a8*/ ADD         SL, SL, #1 
    /*000032ac*/ ADD         R4, R4, SL, LSL #3 
    JPEGE_StartOneFrame_x32b0:
    /*000032b0*/ CMP         FP, #0 
    /*000032b4*/ BEQ         JPEGE_StartOneFrame_x32d4
    /*000032b8*/ LDR         R3, [SP] 
    /*000032bc*/ SUB         R0, R3, #8 
    JPEGE_StartOneFrame_x32c0:
    /*000032c0*/ SUB         R1, R1, #1 
    /*000032c4*/ LDRD        R2, [R4], #8 
    /*000032c8*/ CMN         R1, #1 
    /*000032cc*/ STRD        R2, [R0, #8]! 
    /*000032d0*/ BNE         JPEGE_StartOneFrame_x32c0
    JPEGE_StartOneFrame_x32d4:
    /*000032d4*/ LDR         R1, [R7, #168]              @ 0xa8 
    JPEGE_StartOneFrame_x32d8:
    /*000032d8*/ STR         R9, [R1] 
    /*000032dc*/ LDR         R3, [R7, #128]              @ 0x80 
    /*000032e0*/ B           JPEGE_StartOneFrame_x31b8
    JPEGE_StartOneFrame_x32e4:
    /*000032e4*/ BL          0x00000000000032E4          @ JpegePicStartCtrlSnap [R_ARM_CALL: .text+0x5ce4] 
    /*000032e8*/ CMP         R0, #0 
    /*000032ec*/ BEQ         JPEGE_StartOneFrame_x3058
    /*000032f0*/ LDR         R3, [R4, #3940]             @ 0xf64 
    /*000032f4*/ MOV         R0, R4 
    /*000032f8*/ LDR         R2, [R4, #3952]             @ 0xf70 
    /*000032fc*/ ADD         R3, R3, #1 
    /*00003300*/ STR         R3, [R4, #3940]             @ 0xf64 
    /*00003304*/ ADD         R2, R2, #1 
    /*00003308*/ STR         R2, [R4, #3952]             @ 0xf70 
    /*0000330c*/ BL          0x000000000000330C          @ JpegeCheckEncodeFinish [R_ARM_CALL: .text+0x2e14] 
    /*00003310*/ ADD         R1, SP, #12 
    /*00003314*/ CMP         R0, #0 
    /*00003318*/ MOV         R0, R8 
    /*0000331c*/ LDREQ       R3, [R4, #452]              @ 0x1c4 
    /*00003320*/ LDREQ       R2, [R3, #172]              @ 0xac 
    /*00003324*/ LDREQ       R3, [R3, #168]              @ 0xa8 
    /*00003328*/ LDREQ       R2, [R2] 
    /*0000332c*/ STREQ       R2, [R3] 
    /*00003330*/ BL          0x0000000000003330          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00003334*/ MVN         R0, #0 
    /*00003338*/ B           JPEGE_StartOneFrame_x31d4
    JPEGE_StartOneFrame_x333c:
    /*0000333c*/ LDR         R2, [R7, #176]              @ 0xb0 
    /*00003340*/ MOV         R1, #0 
    /*00003344*/ MVN         R3, #0 
    /*00003348*/ STR         R1, [SP] 
    /*0000334c*/ ADD         IP, R2, IP 
    /*00003350*/ MOV         FP, #7 
    /*00003354*/ ADD         R2, IP, #64                 @ 0x40 
    /*00003358*/ STR         R2, [SP, #4] 
    /*0000335c*/ B           JPEGE_StartOneFrame_x30e4
    JPEGE_StartOneFrame_x3360:
    /*00003360*/ LDR         R3, [R3, #4] 
    /*00003364*/ LDR         R2, [R4, #40]               @ 0x28 
    /*00003368*/ CMP         R2, R3 
    /*0000336c*/ BEQ         JPEGE_StartOneFrame_x3008
    /*00003370*/ LDR         R3, [R6, #3800]             @ 0xed8 
    /*00003374*/ CMP         R3, #0 
    /*00003378*/ BNE         JPEGE_StartOneFrame_x3008
    JPEGE_StartOneFrame_x337c:
    /*0000337c*/ MOV         R0, R4 
    /*00003380*/ BL          0x0000000000003380          @ JpegeCheckRollbackStreamBuffer [R_ARM_CALL: .text+0x2d94] 
    /*00003384*/ CMP         R0, #0 
    /*00003388*/ MVNEQ       R0, #0 
    /*0000338c*/ BEQ         JPEGE_StartOneFrame_x31d4
    /*00003390*/ LDR         R3, [R4, #452]              @ 0x1c4 
    /*00003394*/ MVN         R0, #0 
    /*00003398*/ LDR         R2, [R3, #172]              @ 0xac 
    /*0000339c*/ LDR         R3, [R3, #168]              @ 0xa8 
    /*000033a0*/ LDR         R2, [R2] 
    /*000033a4*/ STR         R2, [R3] 
    /*000033a8*/ ADD         SP, SP, #20 
    /*000033ac*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JPEGE_StartOneFrame_x33b0:
    /*000033b0*/ LSR         SL, R3, #3 
    /*000033b4*/ LDR         R2, [R7, #176]              @ 0xb0 
    /*000033b8*/ CMP         SL, #0 
    /*000033bc*/ SUB         SL, SL, #1 
    /*000033c0*/ ADD         IP, R2, IP 
    /*000033c4*/ ADD         R3, IP, R3 
    /*000033c8*/ STR         R3, [SP] 
    /*000033cc*/ BEQ         JPEGE_StartOneFrame_x32d8
    /*000033d0*/ MVN         R1, #0 
    /*000033d4*/ B           JPEGE_StartOneFrame_x3288
    JPEGE_StartOneFrame_x33d8:
    /*000033d8*/ MOVW        R3, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xaa0] 
    /*000033dc*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*000033e0*/ MOVT        R3, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0xaa0] 
    /*000033e4*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*000033e8*/ MOVW        R2, #2290                   @ 0x8f2 
    /*000033ec*/ LDR         R1, [PC, #268]              @ 0x0000000000003500 
    /*000033f0*/ BL          0x00000000000033F0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000033f4*/ B           JPEGE_StartOneFrame_x31a4
    JPEGE_StartOneFrame_x33f8:
    /*000033f8*/ LDR         R3, [R5, #8] 
    /*000033fc*/ CMP         R3, #1 
    /*00003400*/ BNE         JPEGE_StartOneFrame_x3064
    /*00003404*/ LDR         R3, [R6, #3800]             @ 0xed8 
    /*00003408*/ CMP         R3, #0 
    /*0000340c*/ BEQ         JPEGE_StartOneFrame_x3430
    /*00003410*/ CMP         R3, #3 
    /*00003414*/ LDREQ       R3, [R6, #3816]             @ 0xee8 
    /*00003418*/ STREQ       R3, [R7, #128]              @ 0x80 
    /*0000341c*/ BEQ         JPEGE_StartOneFrame_x3064
    /*00003420*/ CMP         R3, #1 
    /*00003424*/ LDREQ       R3, [R6, #3820]             @ 0xeec 
    /*00003428*/ STREQ       R3, [R7, #128]              @ 0x80 
    /*0000342c*/ B           JPEGE_StartOneFrame_x3064
    JPEGE_StartOneFrame_x3430:
    /*00003430*/ LDR         R3, [R7, #128]              @ 0x80 
    /*00003434*/ STR         R3, [R6, #3816]             @ 0xee8 
    /*00003438*/ LDR         R3, [R7, #128]              @ 0x80 
    /*0000343c*/ LDR         R2, [R7, #8] 
    /*00003440*/ ADD         R3, R3, #65536              @ 0x10000 
    /*00003444*/ ADD         R3, R3, #128                @ 0x80 
    /*00003448*/ STR         R3, [R7, #128]              @ 0x80 
    /*0000344c*/ CMP         R3, R2 
    /*00003450*/ SUBCS       R3, R3, R2 
    /*00003454*/ STRCS       R3, [R7, #128]              @ 0x80 
    /*00003458*/ STR         R3, [R6, #3808]             @ 0xee0 
    /*0000345c*/ B           JPEGE_StartOneFrame_x3064
    JPEGE_StartOneFrame_x3460:
    /*00003460*/ MOVW        R0, #32770                  @ 0x8002 
    /*00003464*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00003468*/ BX          LR 
    JPEGE_StartOneFrame_x346c:
    /*0000346c*/ MOVW        R0, #32773                  @ 0x8005 
    /*00003470*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00003474*/ B           JPEGE_StartOneFrame_x31d4
    JPEGE_StartOneFrame_x3478:
    /*00003478*/ MOVW        R0, #32774                  @ 0x8006 
    /*0000347c*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00003480*/ BX          LR 
    JPEGE_StartOneFrame_x3484:
    /*00003484*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4b4] 
    /*00003488*/ MOV         R1, #18 
    /*0000348c*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4b4] 
    /*00003490*/ MOV         R0, #4 
    /*00003494*/ BL          0x0000000000003494          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003498*/ B           JPEGE_StartOneFrame_x313c
    JPEGE_StartOneFrame_x349c:
    /*0000349c*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4b4] 
    /*000034a0*/ MOV         R1, #18 
    /*000034a4*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4b4] 
    /*000034a8*/ MOV         R0, #4 
    /*000034ac*/ BL          0x00000000000034AC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000034b0*/ LDR         R3, [R7, #128]              @ 0x80 
    /*000034b4*/ B           JPEGE_StartOneFrame_x31b8
    JPEGE_StartOneFrame_x34b8:
    /*000034b8*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x48c] 
    /*000034bc*/ MOV         R1, #18 
    /*000034c0*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x48c] 
    /*000034c4*/ MOV         R0, #3 
    /*000034c8*/ BL          0x00000000000034C8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000034cc*/ LDR         R3, [R7, #128]              @ 0x80 
    /*000034d0*/ B           JPEGE_StartOneFrame_x31b8
    JPEGE_StartOneFrame_x34d4:
    /*000034d4*/ LDR         R2, [R4, #3952]             @ 0xf70 
    /*000034d8*/ MOV         R0, R8 
    /*000034dc*/ LDR         R3, [R4, #3940]             @ 0xf64 
    /*000034e0*/ ADD         R1, SP, #12 
    /*000034e4*/ ADD         R2, R2, #1 
    /*000034e8*/ STR         R2, [R4, #3952]             @ 0xf70 
    /*000034ec*/ ADD         R3, R3, #1 
    /*000034f0*/ STR         R3, [R4, #3940]             @ 0xf64 
    /*000034f4*/ BL          0x00000000000034F4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000034f8*/ MVN         R0, #0 
    /*000034fc*/ B           JPEGE_StartOneFrame_x31d4
    /*00003500*/ ANDEQ       R0, R0, R8, LSL #4 
FUNC_END JPEGE_StartOneFrame


FUNC_BEGIN JPEGE_IntProcess
    /*00003504*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00003508*/ CMP         R0, #15 
    /*0000350c*/ SUB         SP, SP, #56                 @ 0x38 
    /*00003510*/ MOV         R7, #2 
    /*00003514*/ MOV         R3, #3 
    /*00003518*/ STR         R7, [SP, #8] 
    /*0000351c*/ STR         R3, [SP, #12] 
    /*00003520*/ BHI         JPEGE_IntProcess_x3a40
    /*00003524*/ CMP         R2, #0 
    /*00003528*/ BEQ         JPEGE_IntProcess_x3a4c
    /*0000352c*/ MOV         R6, R0 
    /*00003530*/ MOV         R5, R2 
    /*00003534*/ BL          0x0000000000003534          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00003538*/ SUBS        R4, R0, #0 
    /*0000353c*/ BEQ         JPEGE_IntProcess_x3a58
    /*00003540*/ MOV         R0, #19 
    /*00003544*/ BL          0x0000000000003544          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003548*/ LDR         R9, [R5, #16] 
    /*0000354c*/ MOV         R8, R0 
    /*00003550*/ MOV         R0, R6 
    /*00003554*/ BL          0x0000000000003554          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00003558*/ LDR         R3, [R9, #4] 
    /*0000355c*/ CMP         R3, #1 
    /*00003560*/ BEQ         JPEGE_IntProcess_x3804
    /*00003564*/ CMP         R3, #0 
    /*00003568*/ BNE         JPEGE_IntProcess_x365c
    /*0000356c*/ LDR         R3, [R4, #4004]             @ 0xfa4 
    /*00003570*/ LDR         R7, [R4, #4008]             @ 0xfa8 
    /*00003574*/ CMP         R3, #1 
    /*00003578*/ BEQ         JPEGE_IntProcess_x3954
    JPEGE_IntProcess_x357c:
    /*0000357c*/ CMP         R7, #1 
    /*00003580*/ MOVNE       R7, #2 
    /*00003584*/ MOVEQ       R7, #1 
    JPEGE_IntProcess_x3588:
    /*00003588*/ ADD         R0, R0, #4096               @ 0x1000 
    /*0000358c*/ LDR         R3, [R0, #3720]             @ 0xe88 
    /*00003590*/ CMP         R3, #0 
    /*00003594*/ BEQ         JPEGE_IntProcess_x35b4
    /*00003598*/ LDR         R3, [R0, #3740]             @ 0xe9c 
    /*0000359c*/ CMP         R3, #0 
    /*000035a0*/ BEQ         JPEGE_IntProcess_x3a78
    /*000035a4*/ LDRB        R3, [R3] 
    /*000035a8*/ CMP         R3, #1 
    /*000035ac*/ MOVNE       R7, #1 
    /*000035b0*/ BEQ         JPEGE_IntProcess_x39b0
    JPEGE_IntProcess_x35b4:
    /*000035b4*/ LDR         R3, [R4, #4016]             @ 0xfb0 
    /*000035b8*/ CMP         R3, #1 
    /*000035bc*/ BEQ         JPEGE_IntProcess_x3960
    /*000035c0*/ LDR         R3, [R4, #16] 
    /*000035c4*/ STR         R7, [SP, #8] 
    /*000035c8*/ CMP         R3, #0 
    /*000035cc*/ BNE         JPEGE_IntProcess_x3790
    /*000035d0*/ CMP         R7, #0 
    /*000035d4*/ BNE         JPEGE_IntProcess_x3a98
    /*000035d8*/ ADD         R1, SP, #12 
    /*000035dc*/ MOV         R0, R4 
    /*000035e0*/ STR         R7, [SP, #12] 
    /*000035e4*/ BL          0x00000000000035E4          @ JPEGE_DRV_RingPostProcess [R_ARM_CALL: .text+0x7470] 
    /*000035e8*/ LDR         R7, [SP, #12] 
    JPEGE_IntProcess_x35ec:
    /*000035ec*/ CMP         R7, #1 
    /*000035f0*/ BNE         JPEGE_IntProcess_x3700
    JPEGE_IntProcess_x35f4:
    /*000035f4*/ ADD         R8, R4, #6912               @ 0x1b00 
    /*000035f8*/ ADD         R1, SP, #16 
    /*000035fc*/ ADD         R8, R8, #52                 @ 0x34 
    /*00003600*/ MOV         R0, R8 
    /*00003604*/ BL          0x0000000000003604          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00003608*/ STR         R7, [R5] 
    /*0000360c*/ MOV         R3, #0 
    /*00003610*/ STR         R3, [R5, #4] 
    /*00003614*/ LDR         R3, [R4, #452]              @ 0x1c4 
    /*00003618*/ MOV         R0, R8 
    /*0000361c*/ ADD         R1, SP, #16 
    /*00003620*/ LDR         R2, [R3, #172]              @ 0xac 
    /*00003624*/ LDR         R3, [R3, #168]              @ 0xa8 
    /*00003628*/ LDR         R2, [R2] 
    /*0000362c*/ STR         R2, [R3] 
    /*00003630*/ LDR         R3, [R4, #328]              @ 0x148 
    /*00003634*/ STR         R3, [R4, #296]              @ 0x128 
    /*00003638*/ STR         R7, [R5, #20] 
    /*0000363c*/ LDR         R2, [R4, #3948]             @ 0xf6c 
    /*00003640*/ LDR         R3, [R4, #3928]             @ 0xf58 
    /*00003644*/ ADD         R2, R2, #1 
    /*00003648*/ STR         R2, [R4, #3948]             @ 0xf6c 
    /*0000364c*/ ADD         R3, R3, #1 
    /*00003650*/ STR         R3, [R4, #3928]             @ 0xf58 
    /*00003654*/ BL          0x0000000000003654          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00003658*/ B           JPEGE_IntProcess_x3760
    JPEGE_IntProcess_x365c:
    /*0000365c*/ MOVW        R3, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8d0] 
    /*00003660*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00003664*/ MOVT        R3, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8d0] 
    /*00003668*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*0000366c*/ MOV         R2, #218                    @ 0xda 
    /*00003670*/ LDR         R1, [PC, #1068]             @ 0x0000000000003aa4 
    /*00003674*/ BL          0x0000000000003674          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003678*/ LDR         R3, [R4, #16] 
    /*0000367c*/ STR         R7, [SP, #8] 
    /*00003680*/ CMP         R3, #0 
    /*00003684*/ BEQ         JPEGE_IntProcess_x3814
    JPEGE_IntProcess_x3688:
    /*00003688*/ ADD         R7, R4, #6912               @ 0x1b00 
    /*0000368c*/ ADD         R1, SP, #16 
    /*00003690*/ ADD         R7, R7, #52                 @ 0x34 
    /*00003694*/ MOV         R0, R7 
    /*00003698*/ BL          0x0000000000003698          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000369c*/ ADD         R2, R4, #4096               @ 0x1000 
    /*000036a0*/ MOV         R3, #0 
    /*000036a4*/ ADD         R1, SP, #8 
    /*000036a8*/ STR         R3, [R2, #3704]             @ 0xe78 
    /*000036ac*/ MOV         R0, R4 
    /*000036b0*/ STR         R3, [R2, #3708]             @ 0xe7c 
    /*000036b4*/ STR         R3, [R2, #3716]             @ 0xe84 
    /*000036b8*/ STR         R3, [SP, #28] 
    /*000036bc*/ STR         R3, [SP, #32] 
    /*000036c0*/ BL          0x00000000000036C0          @ JPEGE_DRV_ConvertState [R_ARM_CALL: .text+0x7474] 
    /*000036c4*/ ADD         R1, SP, #16 
    /*000036c8*/ STRB        R0, [SP, #20] 
    /*000036cc*/ MOV         R0, R7 
    /*000036d0*/ BL          0x00000000000036D0          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    JPEGE_IntProcess_x36d4:
    /*000036d4*/ LDR         R3, [R8, #28] 
    /*000036d8*/ ADD         R2, SP, #36                 @ 0x24 
    /*000036dc*/ ADD         R1, SP, #20 
    /*000036e0*/ MOV         R0, R6 
    /*000036e4*/ BLX         R3 
    /*000036e8*/ CMP         R0, #0 
    /*000036ec*/ BNE         JPEGE_IntProcess_x3934
    JPEGE_IntProcess_x36f0:
    /*000036f0*/ LDR         R7, [SP, #36]               @ 0x24 
    /*000036f4*/ CMP         R7, #1 
    /*000036f8*/ STR         R7, [SP, #12] 
    /*000036fc*/ BEQ         JPEGE_IntProcess_x35f4
    JPEGE_IntProcess_x3700:
    /*00003700*/ BCC         JPEGE_IntProcess_x3860
    /*00003704*/ CMP         R7, #2 
    /*00003708*/ BNE         JPEGE_IntProcess_x3830
    /*0000370c*/ ADD         R8, R4, #6912               @ 0x1b00 
    /*00003710*/ ADD         R1, SP, #16 
    /*00003714*/ ADD         R8, R8, #52                 @ 0x34 
    /*00003718*/ MOV         R0, R8 
    /*0000371c*/ BL          0x000000000000371C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00003720*/ LDR         R3, [R4, #452]              @ 0x1c4 
    /*00003724*/ MOV         R2, #0 
    /*00003728*/ MOV         R0, R8 
    /*0000372c*/ ADD         R1, SP, #16 
    /*00003730*/ LDR         IP, [R3, #172]              @ 0xac 
    /*00003734*/ LDR         R3, [R3, #168]              @ 0xa8 
    /*00003738*/ LDR         IP, [IP] 
    /*0000373c*/ STR         IP, [R3] 
    /*00003740*/ LDR         R3, [R4, #328]              @ 0x148 
    /*00003744*/ STR         R3, [R4, #296]              @ 0x128 
    /*00003748*/ STR         R2, [R5] 
    /*0000374c*/ LDR         R3, [R4, #3932]             @ 0xf5c 
    /*00003750*/ ADD         R3, R3, #1 
    /*00003754*/ STR         R3, [R4, #3932]             @ 0xf5c 
    /*00003758*/ STR         R7, [R5, #20] 
    /*0000375c*/ BL          0x000000000000375C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    JPEGE_IntProcess_x3760:
    /*00003760*/ LDR         R4, [SP, #12] 
    /*00003764*/ CMP         R4, #0 
    /*00003768*/ MOVNE       R0, #0 
    /*0000376c*/ BNE         JPEGE_IntProcess_x3788
    /*00003770*/ MOV         R0, #8 
    /*00003774*/ BL          0x0000000000003774          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003778*/ LDR         R3, [R0] 
    /*0000377c*/ MOV         R0, R6 
    /*00003780*/ BLX         R3 
    /*00003784*/ MOV         R0, R4 
    JPEGE_IntProcess_x3788:
    /*00003788*/ ADD         SP, SP, #56                 @ 0x38 
    /*0000378c*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    JPEGE_IntProcess_x3790:
    /*00003790*/ CMP         R7, #0 
    /*00003794*/ BNE         JPEGE_IntProcess_x3688
    /*00003798*/ ADD         SL, R4, #6912               @ 0x1b00 
    /*0000379c*/ ADD         R7, SP, #16 
    /*000037a0*/ ADD         SL, SL, #52                 @ 0x34 
    /*000037a4*/ ADD         R9, R4, #4096               @ 0x1000 
    /*000037a8*/ MOV         R1, R7 
    /*000037ac*/ MOV         R0, SL 
    /*000037b0*/ BL          0x00000000000037B0          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000037b4*/ LDR         R1, [R4, #452]              @ 0x1c4 
    /*000037b8*/ LDR         R3, [R9, #3700]             @ 0xe74 
    /*000037bc*/ MOV         R0, R4 
    /*000037c0*/ LDR         R2, [R1, #128]              @ 0x80 
    /*000037c4*/ CMP         R2, R3 
    /*000037c8*/ LDRCC       R1, [R1, #8] 
    /*000037cc*/ ADDCC       R2, R2, R1 
    /*000037d0*/ ADD         R1, SP, #8 
    /*000037d4*/ SUB         R3, R2, R3 
    /*000037d8*/ LSL         R3, R3, #3 
    /*000037dc*/ STR         R3, [R9, #3704]             @ 0xe78 
    /*000037e0*/ STR         R3, [SP, #28] 
    /*000037e4*/ MOV         R3, #0 
    /*000037e8*/ STR         R3, [SP, #32] 
    /*000037ec*/ BL          0x00000000000037EC          @ JPEGE_DRV_ConvertState [R_ARM_CALL: .text+0x7474] 
    /*000037f0*/ MOV         R1, R7 
    /*000037f4*/ STRB        R0, [SP, #20] 
    /*000037f8*/ MOV         R0, SL 
    /*000037fc*/ BL          0x00000000000037FC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00003800*/ B           JPEGE_IntProcess_x36d4
    JPEGE_IntProcess_x3804:
    /*00003804*/ LDR         R3, [R4, #16] 
    /*00003808*/ STR         R7, [SP, #8] 
    /*0000380c*/ CMP         R3, #0 
    /*00003810*/ BNE         JPEGE_IntProcess_x3688
    JPEGE_IntProcess_x3814:
    /*00003814*/ ADD         R1, SP, #12 
    /*00003818*/ MOV         R0, R4 
    /*0000381c*/ MOV         R3, #2 
    /*00003820*/ STR         R3, [SP, #12] 
    /*00003824*/ BL          0x0000000000003824          @ JPEGE_DRV_RingPostProcess [R_ARM_CALL: .text+0x7470] 
    /*00003828*/ LDR         R7, [SP, #12] 
    /*0000382c*/ B           JPEGE_IntProcess_x35ec
    JPEGE_IntProcess_x3830:
    /*00003830*/ MOV         R1, R7 
    /*00003834*/ MOVW        R0, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb14] 
    /*00003838*/ MOVT        R0, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb14] 
    /*0000383c*/ BL          0x000000000000383C          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00003840*/ MOVW        R3, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8d0] 
    /*00003844*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00003848*/ MOVT        R3, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8d0] 
    /*0000384c*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00003850*/ MOVW        R2, #2566                   @ 0xa06 
    /*00003854*/ LDR         R1, [PC, #588]              @ 0x0000000000003aa8 
    /*00003858*/ BL          0x0000000000003858          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000385c*/ B           JPEGE_IntProcess_x3760
    JPEGE_IntProcess_x3860:
    /*00003860*/ ADD         R8, R4, #6912               @ 0x1b00 
    /*00003864*/ ADD         R7, SP, #16 
    /*00003868*/ ADD         R8, R8, #52                 @ 0x34 
    /*0000386c*/ MOV         R1, R7 
    /*00003870*/ MOV         R0, R8 
    /*00003874*/ BL          0x0000000000003874          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00003878*/ MOVW        R3, #0                      @ VencBufferCache [R_ARM_MOVW_ABS_NC: +0x0] 
    /*0000387c*/ MOVT        R3, #0                      @ VencBufferCache [R_ARM_MOVT_ABS: +0x0] 
    /*00003880*/ LDR         R3, [R3] 
    /*00003884*/ CMP         R3, #1 
    /*00003888*/ BEQ         JPEGE_IntProcess_x39d0
    JPEGE_IntProcess_x388c:
    /*0000388c*/ ADD         R9, R4, #4096               @ 0x1000 
    /*00003890*/ LDRB        R3, [R9, #3804]             @ 0xedc 
    /*00003894*/ CMP         R3, #1 
    /*00003898*/ BEQ         JPEGE_IntProcess_x38d8
    JPEGE_IntProcess_x389c:
    /*0000389c*/ MOV         R0, R4 
    /*000038a0*/ BL          0x00000000000038A0          @ JpegeCheckEncodeFinish [R_ARM_CALL: .text+0x2e14] 
    /*000038a4*/ CMP         R0, #1 
    /*000038a8*/ BEQ         JPEGE_IntProcess_x38f0
    /*000038ac*/ MOV         R3, #0 
    /*000038b0*/ STR         R3, [R5] 
    /*000038b4*/ LDRB        R3, [R9, #3804]             @ 0xedc 
    /*000038b8*/ CMP         R3, #1 
    /*000038bc*/ BEQ         JPEGE_IntProcess_x3a30
    JPEGE_IntProcess_x38c0:
    /*000038c0*/ MOV         R3, #0 
    /*000038c4*/ MOV         R1, R7 
    /*000038c8*/ MOV         R0, R8 
    /*000038cc*/ STR         R3, [R5, #20] 
    /*000038d0*/ BL          0x00000000000038D0          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000038d4*/ B           JPEGE_IntProcess_x3760
    JPEGE_IntProcess_x38d8:
    /*000038d8*/ LDR         R3, [R9, #3800]             @ 0xed8 
    /*000038dc*/ CMP         R3, #0 
    /*000038e0*/ LDREQ       R3, [R4, #452]              @ 0x1c4 
    /*000038e4*/ LDREQ       R3, [R3, #128]              @ 0x80 
    /*000038e8*/ STREQ       R3, [R9, #3820]             @ 0xeec 
    /*000038ec*/ B           JPEGE_IntProcess_x389c
    JPEGE_IntProcess_x38f0:
    /*000038f0*/ LDR         R3, [R4, #68]               @ 0x44 
    /*000038f4*/ STR         R3, [R9, #3716]             @ 0xe84 
    /*000038f8*/ STR         R0, [R5] 
    /*000038fc*/ LDR         R1, [R4, #452]              @ 0x1c4 
    /*00003900*/ LDR         R2, [R4, #3980]             @ 0xf8c 
    /*00003904*/ LDR         R3, [R4, #3944]             @ 0xf68 
    /*00003908*/ ADD         R2, R2, #1 
    /*0000390c*/ STR         R2, [R4, #3980]             @ 0xf8c 
    /*00003910*/ ADD         R3, R3, #1 
    /*00003914*/ STR         R3, [R4, #3944]             @ 0xf68 
    /*00003918*/ LDR         R2, [R1, #168]              @ 0xa8 
    /*0000391c*/ LDR         R3, [R1, #172]              @ 0xac 
    /*00003920*/ LDR         R2, [R2] 
    /*00003924*/ STR         R2, [R3] 
    /*00003928*/ LDR         R3, [R4, #296]              @ 0x128 
    /*0000392c*/ STR         R3, [R4, #328]              @ 0x148 
    /*00003930*/ B           JPEGE_IntProcess_x38c0
    JPEGE_IntProcess_x3934:
    /*00003934*/ MOVW        R3, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf8] 
    /*00003938*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*0000393c*/ MOVT        R3, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf8] 
    /*00003940*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00003944*/ MOVW        R2, #2488                   @ 0x9b8 
    /*00003948*/ LDR         R1, [PC, #344]              @ 0x0000000000003aa8 
    /*0000394c*/ BL          0x000000000000394C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003950*/ B           JPEGE_IntProcess_x36f0
    JPEGE_IntProcess_x3954:
    /*00003954*/ CMP         R7, #0 
    /*00003958*/ BEQ         JPEGE_IntProcess_x3588
    /*0000395c*/ B           JPEGE_IntProcess_x357c
    JPEGE_IntProcess_x3960:
    /*00003960*/ LDR         R2, [R4, #16] 
    /*00003964*/ STR         R3, [SP, #8] 
    /*00003968*/ CMP         R2, #0 
    /*0000396c*/ BNE         JPEGE_IntProcess_x3688
    JPEGE_IntProcess_x3970:
    /*00003970*/ MOVW        R3, #2454                   @ 0x996 
    /*00003974*/ MOV         R1, #11 
    /*00003978*/ STR         R3, [SP] 
    /*0000397c*/ MOVW        R2, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xac0] 
    /*00003980*/ LDR         R3, [PC, #288]              @ 0x0000000000003aa8 
    /*00003984*/ MOVT        R2, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xac0] 
    /*00003988*/ STR         R6, [SP, #4] 
    /*0000398c*/ MOV         R0, #3 
    /*00003990*/ BL          0x0000000000003990          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003994*/ ADD         R1, SP, #12 
    /*00003998*/ MOV         R0, R4 
    /*0000399c*/ MOV         R3, #1 
    /*000039a0*/ STR         R3, [SP, #12] 
    /*000039a4*/ BL          0x00000000000039A4          @ JPEGE_DRV_RingPostProcess [R_ARM_CALL: .text+0x7470] 
    /*000039a8*/ LDR         R7, [SP, #12] 
    /*000039ac*/ B           JPEGE_IntProcess_x35ec
    JPEGE_IntProcess_x39b0:
    /*000039b0*/ LDR         R2, [R4, #4020]             @ 0xfb4 
    /*000039b4*/ CMP         R2, #0 
    /*000039b8*/ MOVNE       R7, R3 
    /*000039bc*/ BNE         JPEGE_IntProcess_x35b4
    /*000039c0*/ LDR         R3, [R4, #4024]             @ 0xfb8 
    /*000039c4*/ CMP         R3, #0 
    /*000039c8*/ MOVNE       R7, #1 
    /*000039cc*/ B           JPEGE_IntProcess_x35b4
    JPEGE_IntProcess_x39d0:
    /*000039d0*/ LDR         IP, [R4, #452]              @ 0x1c4 
    /*000039d4*/ LDRD        R0, [R4, #8] 
    /*000039d8*/ LDR         LR, [IP, #168]              @ 0xa8 
    /*000039dc*/ LDR         R2, [IP, #172]              @ 0xac 
    /*000039e0*/ LDR         R3, [IP, #176]              @ 0xb0 
    /*000039e4*/ LDR         R9, [LR] 
    /*000039e8*/ LDR         R2, [R2] 
    /*000039ec*/ ADD         SL, R3, R9 
    /*000039f0*/ ADD         LR, R3, R2 
    /*000039f4*/ ADDS        R0, R0, R2 
    /*000039f8*/ CMP         LR, SL 
    /*000039fc*/ BCC         JPEGE_IntProcess_x3a64
    /*00003a00*/ LDR         R2, [IP, #8] 
    /*00003a04*/ MOV         R1, R0 
    /*00003a08*/ MOV         R0, LR 
    /*00003a0c*/ ADD         R2, R3, R2 
    /*00003a10*/ SUB         R2, R2, LR 
    /*00003a14*/ BL          0x0000000000003A14          @ hil_mmb_invalid_cache_byaddr [R_ARM_CALL: +0x0] 
    /*00003a18*/ LDR         R3, [R4, #452]              @ 0x1c4 
    /*00003a1c*/ MOV         R2, R9 
    /*00003a20*/ LDR         R1, [R4, #8] 
    /*00003a24*/ LDR         R0, [R3, #176]              @ 0xb0 
    /*00003a28*/ BL          0x0000000000003A28          @ hil_mmb_invalid_cache_byaddr [R_ARM_CALL: +0x0] 
    /*00003a2c*/ B           JPEGE_IntProcess_x388c
    JPEGE_IntProcess_x3a30:
    /*00003a30*/ LDR         R2, [R9, #3800]             @ 0xed8 
    /*00003a34*/ CMP         R2, #0 
    /*00003a38*/ STREQ       R3, [R5] 
    /*00003a3c*/ B           JPEGE_IntProcess_x38c0
    JPEGE_IntProcess_x3a40:
    /*00003a40*/ MOVW        R0, #32770                  @ 0x8002 
    /*00003a44*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00003a48*/ B           JPEGE_IntProcess_x3788
    JPEGE_IntProcess_x3a4c:
    /*00003a4c*/ MOVW        R0, #32774                  @ 0x8006 
    /*00003a50*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00003a54*/ B           JPEGE_IntProcess_x3788
    JPEGE_IntProcess_x3a58:
    /*00003a58*/ MOVW        R0, #32773                  @ 0x8005 
    /*00003a5c*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00003a60*/ B           JPEGE_IntProcess_x3788
    JPEGE_IntProcess_x3a64:
    /*00003a64*/ MOV         R1, R0 
    /*00003a68*/ SUB         R2, R9, R2 
    /*00003a6c*/ MOV         R0, LR 
    /*00003a70*/ BL          0x0000000000003A70          @ hil_mmb_invalid_cache_byaddr [R_ARM_CALL: +0x0] 
    /*00003a74*/ B           JPEGE_IntProcess_x388c
    JPEGE_IntProcess_x3a78:
    /*00003a78*/ MOVW        R3, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8d0] 
    /*00003a7c*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x110] 
    /*00003a80*/ MOVT        R3, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8d0] 
    /*00003a84*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x110] 
    /*00003a88*/ MOV         R2, #199                    @ 0xc7 
    /*00003a8c*/ LDR         R1, [PC, #16]               @ 0x0000000000003aa4 
    /*00003a90*/ BL          0x0000000000003A90          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003a94*/ B           JPEGE_IntProcess_x35b4
    JPEGE_IntProcess_x3a98:
    /*00003a98*/ CMP         R7, #2 
    /*00003a9c*/ BNE         JPEGE_IntProcess_x3970
    /*00003aa0*/ B           JPEGE_IntProcess_x3814
    /*00003aa4*/ ANDEQ       R0, R0, IP, LSL R2 
    /*00003aa8*/ ANDEQ       R0, R0, IP, LSR #4 
FUNC_END JPEGE_IntProcess


FUNC_BEGIN JpegeGetBitsOfLastFrm
    /*00003aac*/ LDR         R2, [R0, #452]              @ 0x1c4 
    /*00003ab0*/ ADD         R3, R0, #4096               @ 0x1000 
    /*00003ab4*/ LDR         R0, [R3, #3700]             @ 0xe74 
    /*00003ab8*/ LDR         R3, [R2, #128]              @ 0x80 
    /*00003abc*/ CMP         R3, R0 
    /*00003ac0*/ LDRCC       R2, [R2, #8] 
    /*00003ac4*/ ADDCC       R3, R3, R2 
    /*00003ac8*/ SUB         R0, R3, R0 
    /*00003acc*/ LSL         R0, R0, #3 
    /*00003ad0*/ BX          LR 
FUNC_END JpegeGetBitsOfLastFrm


FUNC_BEGIN JpegeCheckParam
    /*00003ad4*/ CMP         R0, #0 
    /*00003ad8*/ BEQ         JpegeCheckParam_x3bfc
    /*00003adc*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00003ae0*/ SUB         SP, SP, #20 
    /*00003ae4*/ LDR         R3, [R0] 
    /*00003ae8*/ SUB         R2, R3, #1 
    /*00003aec*/ CMP         R2, #98                     @ 0x62 
    /*00003af0*/ BHI         JpegeCheckParam_x3b7c
    /*00003af4*/ LDR         R3, [R0, #196]              @ 0xc4 
    /*00003af8*/ CMP         R3, #65536                  @ 0x10000 
    /*00003afc*/ BCS         JpegeCheckParam_x3be0
    /*00003b00*/ CMP         R3, R1 
    /*00003b04*/ ADDLS       R3, R0, #3 
    /*00003b08*/ ADDLS       R0, R0, #67                 @ 0x43 
    /*00003b0c*/ BLS         JpegeCheckParam_x3b34
    /*00003b10*/ B           JpegeCheckParam_x3bbc
    JpegeCheckParam_x3b14:
    /*00003b14*/ LDRB        R2, [R3, #64]               @ 0x40 
    /*00003b18*/ CMP         R2, #0 
    /*00003b1c*/ BEQ         JpegeCheckParam_x3b40
    /*00003b20*/ LDRB        R2, [R3, #128]              @ 0x80 
    /*00003b24*/ CMP         R2, #0 
    /*00003b28*/ BEQ         JpegeCheckParam_x3b40
    /*00003b2c*/ CMP         R3, R0 
    /*00003b30*/ BEQ         JpegeCheckParam_x3b70
    JpegeCheckParam_x3b34:
    /*00003b34*/ LDRB        R2, [R3, #1]! 
    /*00003b38*/ CMP         R2, #0 
    /*00003b3c*/ BNE         JpegeCheckParam_x3b14
    JpegeCheckParam_x3b40:
    /*00003b40*/ MOVW        R3, #2718                   @ 0xa9e 
    /*00003b44*/ MOVW        R2, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xba8] 
    /*00003b48*/ STR         R3, [SP] 
    /*00003b4c*/ MOVT        R2, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0xba8] 
    /*00003b50*/ LDR         R3, [PC, #176]              @ 0x0000000000003c08 
    /*00003b54*/ MOV         R1, #11 
    /*00003b58*/ MOV         R0, #3 
    /*00003b5c*/ BL          0x0000000000003B5C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003b60*/ MOVW        R0, #32771                  @ 0x8003 
    /*00003b64*/ MOVT        R0, #40971                  @ 0xa00b 
    JpegeCheckParam_x3b68:
    /*00003b68*/ ADD         SP, SP, #20 
    /*00003b6c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    JpegeCheckParam_x3b70:
    /*00003b70*/ MOV         R0, #0 
    /*00003b74*/ ADD         SP, SP, #20 
    /*00003b78*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    JpegeCheckParam_x3b7c:
    /*00003b7c*/ MOV         R2, #99                     @ 0x63 
    /*00003b80*/ STR         R2, [SP, #12] 
    /*00003b84*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb24] 
    /*00003b88*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb24] 
    /*00003b8c*/ STR         R3, [SP, #4] 
    /*00003b90*/ MOV         R3, #1 
    /*00003b94*/ STR         R3, [SP, #8] 
    /*00003b98*/ MOV         R3, #2688                   @ 0xa80 
    /*00003b9c*/ STR         R3, [SP] 
    JpegeCheckParam_x3ba0:
    /*00003ba0*/ LDR         R3, [PC, #96]               @ 0x0000000000003c08 
    /*00003ba4*/ MOV         R1, #11 
    /*00003ba8*/ MOV         R0, #3 
    /*00003bac*/ BL          0x0000000000003BAC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003bb0*/ MOVW        R0, #32771                  @ 0x8003 
    /*00003bb4*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00003bb8*/ B           JpegeCheckParam_x3b68
    JpegeCheckParam_x3bbc:
    /*00003bbc*/ STR         R3, [SP, #4] 
    /*00003bc0*/ MOVW        R3, #2706                   @ 0xa92 
    /*00003bc4*/ MOV         R2, #0 
    /*00003bc8*/ STR         R1, [SP, #12] 
    /*00003bcc*/ STR         R2, [SP, #8] 
    JpegeCheckParam_x3bd0:
    /*00003bd0*/ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb64] 
    /*00003bd4*/ STR         R3, [SP] 
    /*00003bd8*/ MOVT        R2, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb64] 
    /*00003bdc*/ B           JpegeCheckParam_x3ba0
    JpegeCheckParam_x3be0:
    /*00003be0*/ STR         R3, [SP, #4] 
    /*00003be4*/ MOVW        R2, #65535                  @ 0xffff 
    /*00003be8*/ MOV         R3, #0 
    /*00003bec*/ STR         R2, [SP, #12] 
    /*00003bf0*/ STR         R3, [SP, #8] 
    /*00003bf4*/ MOVW        R3, #2697                   @ 0xa89 
    /*00003bf8*/ B           JpegeCheckParam_x3bd0
    JpegeCheckParam_x3bfc:
    /*00003bfc*/ MOVW        R0, #32774                  @ 0x8006 
    /*00003c00*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00003c04*/ BX          LR 
    /*00003c08*/ ANDEQ       R0, R0, R0, ASR #4 
FUNC_END JpegeCheckParam


FUNC_BEGIN JPEGE_SetParam
    /*00003c0c*/ CMP         R0, #15 
    /*00003c10*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00003c14*/ SUB         SP, SP, #20 
    /*00003c18*/ BHI         JPEGE_SetParam_x3d1c
    /*00003c1c*/ CMP         R1, #0 
    /*00003c20*/ BEQ         JPEGE_SetParam_x3d34
    /*00003c24*/ MOV         R4, R0 
    /*00003c28*/ MOV         R7, R1 
    /*00003c2c*/ BL          0x0000000000003C2C          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00003c30*/ SUBS        R8, R0, #0 
    /*00003c34*/ BEQ         JPEGE_SetParam_x3d28
    /*00003c38*/ ADD         R5, R8, #6912               @ 0x1b00 
    /*00003c3c*/ ADD         R1, SP, #12 
    /*00003c40*/ ADD         R5, R5, #52                 @ 0x34 
    /*00003c44*/ MOV         R0, R5 
    /*00003c48*/ BL          0x0000000000003C48          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00003c4c*/ LDR         R3, [R8, #36]               @ 0x24 
    /*00003c50*/ LDR         R1, [R8, #40]               @ 0x28 
    /*00003c54*/ MOV         R0, R7 
    /*00003c58*/ ADD         R3, R3, #15 
    /*00003c5c*/ ADD         R1, R1, #15 
    /*00003c60*/ LSR         R3, R3, #4 
    /*00003c64*/ LSR         R1, R1, #4 
    /*00003c68*/ MUL         R1, R1, R3 
    /*00003c6c*/ LSL         R1, R1, #1 
    /*00003c70*/ BL          0x0000000000003C70          @ JpegeCheckParam [R_ARM_CALL: .text+0x3ad4] 
    /*00003c74*/ SUBS        R6, R0, #0 
    /*00003c78*/ BNE         JPEGE_SetParam_x3d40
    /*00003c7c*/ LDR         R2, [R7, #196]              @ 0xc4 
    /*00003c80*/ ADD         R3, R8, #4096               @ 0x1000 
    /*00003c84*/ ADD         LR, R8, #6912               @ 0x1b00 
    /*00003c88*/ ADD         R9, R7, #8 
    /*00003c8c*/ ADD         LR, LR, #59                 @ 0x3b 
    /*00003c90*/ MOV         R0, R7 
    /*00003c94*/ STR         R2, [R3, #3068]             @ 0xbfc 
    /*00003c98*/ LDR         R2, [R7] 
    /*00003c9c*/ STR         R2, [R3, #2872]             @ 0xb38 
    JPEGE_SetParam_x3ca0:
    /*00003ca0*/ ADD         R1, R0, #64                 @ 0x40 
    /*00003ca4*/ MOV         R2, LR 
    /*00003ca8*/ MOV         R3, R0 
    JPEGE_SetParam_x3cac:
    /*00003cac*/ LDRB        IP, [R3, #4] 
    /*00003cb0*/ ADD         R3, R3, #8 
    /*00003cb4*/ STRB        IP, [R2, #1]! 
    /*00003cb8*/ LDRB        IP, [R3, #60]               @ 0x3c 
    /*00003cbc*/ STRB        IP, [R2, #64]               @ 0x40 
    /*00003cc0*/ LDRB        IP, [R3, #124]              @ 0x7c 
    /*00003cc4*/ CMP         R3, R1 
    /*00003cc8*/ STRB        IP, [R2, #128]              @ 0x80 
    /*00003ccc*/ BNE         JPEGE_SetParam_x3cac
    /*00003cd0*/ ADD         R0, R0, #1 
    /*00003cd4*/ ADD         LR, LR, #8 
    /*00003cd8*/ CMP         R0, R9 
    /*00003cdc*/ BNE         JPEGE_SetParam_x3ca0
    /*00003ce0*/ LDR         R2, [R7] 
    /*00003ce4*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003ce8*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003cec*/ ADD         R1, SP, #12 
    /*00003cf0*/ MOV         R0, R5 
    /*00003cf4*/ STR         R2, [R8, #68]               @ 0x44 
    /*00003cf8*/ LDR         IP, [R7, #196]              @ 0xc4 
    /*00003cfc*/ LDR         R2, [R3, R4, LSL #2] 
    /*00003d00*/ STR         IP, [R8, #64]               @ 0x40 
    /*00003d04*/ ADD         R2, R2, #1 
    /*00003d08*/ STR         R2, [R3, R4, LSL #2] 
    /*00003d0c*/ BL          0x0000000000003D0C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    JPEGE_SetParam_x3d10:
    /*00003d10*/ MOV         R0, R6 
    /*00003d14*/ ADD         SP, SP, #20 
    /*00003d18*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    JPEGE_SetParam_x3d1c:
    /*00003d1c*/ MOVW        R6, #32770                  @ 0x8002 
    /*00003d20*/ MOVT        R6, #40971                  @ 0xa00b 
    /*00003d24*/ B           JPEGE_SetParam_x3d10
    JPEGE_SetParam_x3d28:
    /*00003d28*/ MOVW        R6, #32773                  @ 0x8005 
    /*00003d2c*/ MOVT        R6, #40971                  @ 0xa00b 
    /*00003d30*/ B           JPEGE_SetParam_x3d10
    JPEGE_SetParam_x3d34:
    /*00003d34*/ MOVW        R6, #32774                  @ 0x8006 
    /*00003d38*/ MOVT        R6, #40971                  @ 0xa00b 
    /*00003d3c*/ B           JPEGE_SetParam_x3d10
    JPEGE_SetParam_x3d40:
    /*00003d40*/ MOVW        R3, #2766                   @ 0xace 
    /*00003d44*/ MOV         R1, #11 
    /*00003d48*/ STR         R3, [SP] 
    /*00003d4c*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbe4] 
    /*00003d50*/ LDR         R3, [PC, #32]               @ 0x0000000000003d78 
    /*00003d54*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbe4] 
    /*00003d58*/ MOV         R0, #3 
    /*00003d5c*/ MOVW        R6, #32771                  @ 0x8003 
    /*00003d60*/ BL          0x0000000000003D60          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003d64*/ ADD         R1, SP, #12 
    /*00003d68*/ MOV         R0, R5 
    /*00003d6c*/ MOVT        R6, #40971                  @ 0xa00b 
    /*00003d70*/ BL          0x0000000000003D70          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00003d74*/ B           JPEGE_SetParam_x3d10
    /*00003d78*/ ANDEQ       R0, R0, R0, ASR R2 
FUNC_END JPEGE_SetParam


FUNC_BEGIN MjpegeCheckParam
    /*00003d7c*/ CMP         R0, #0 
    /*00003d80*/ BEQ         MjpegeCheckParam_x3e40
    /*00003d84*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00003d88*/ SUB         SP, SP, #20 
    /*00003d8c*/ LDR         R3, [R0, #192]              @ 0xc0 
    /*00003d90*/ CMP         R3, R1 
    /*00003d94*/ SUBLS       R3, R0, #1 
    /*00003d98*/ ADDLS       R0, R0, #63                 @ 0x3f 
    /*00003d9c*/ BLS         MjpegeCheckParam_x3dc4
    /*00003da0*/ B           MjpegeCheckParam_x3e08
    MjpegeCheckParam_x3da4:
    /*00003da4*/ LDRB        R2, [R3, #64]               @ 0x40 
    /*00003da8*/ CMP         R2, #0 
    /*00003dac*/ BEQ         MjpegeCheckParam_x3dd0
    /*00003db0*/ LDRB        R2, [R3, #128]              @ 0x80 
    /*00003db4*/ CMP         R2, #0 
    /*00003db8*/ BEQ         MjpegeCheckParam_x3dd0
    /*00003dbc*/ CMP         R3, R0 
    /*00003dc0*/ BEQ         MjpegeCheckParam_x3dfc
    MjpegeCheckParam_x3dc4:
    /*00003dc4*/ LDRB        R2, [R3, #1]! 
    /*00003dc8*/ CMP         R2, #0 
    /*00003dcc*/ BNE         MjpegeCheckParam_x3da4
    MjpegeCheckParam_x3dd0:
    /*00003dd0*/ MOVW        R3, #2875                   @ 0xb3b 
    /*00003dd4*/ MOVW        R2, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xba8] 
    /*00003dd8*/ STR         R3, [SP] 
    /*00003ddc*/ MOVT        R2, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0xba8] 
    /*00003de0*/ LDR         R3, [PC, #100]              @ 0x0000000000003e4c 
    /*00003de4*/ MOV         R1, #11 
    /*00003de8*/ MOV         R0, #3 
    /*00003dec*/ BL          0x0000000000003DEC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003df0*/ MVN         R0, #0 
    MjpegeCheckParam_x3df4:
    /*00003df4*/ ADD         SP, SP, #20 
    /*00003df8*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    MjpegeCheckParam_x3dfc:
    /*00003dfc*/ MOV         R0, #0 
    /*00003e00*/ ADD         SP, SP, #20 
    /*00003e04*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    MjpegeCheckParam_x3e08:
    /*00003e08*/ STR         R1, [SP, #12] 
    /*00003e0c*/ MOV         R2, #0 
    /*00003e10*/ STR         R3, [SP, #4] 
    /*00003e14*/ MOV         R1, #11 
    /*00003e18*/ MOVW        R3, #2863                   @ 0xb2f 
    /*00003e1c*/ STR         R2, [SP, #8] 
    /*00003e20*/ STR         R3, [SP] 
    /*00003e24*/ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb64] 
    /*00003e28*/ LDR         R3, [PC, #28]               @ 0x0000000000003e4c 
    /*00003e2c*/ MOVT        R2, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb64] 
    /*00003e30*/ MOV         R0, #3 
    /*00003e34*/ BL          0x0000000000003E34          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003e38*/ MVN         R0, #0 
    /*00003e3c*/ B           MjpegeCheckParam_x3df4
    MjpegeCheckParam_x3e40:
    /*00003e40*/ MOVW        R0, #32774                  @ 0x8006 
    /*00003e44*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00003e48*/ BX          LR 
    /*00003e4c*/ ANDEQ       R0, R0, R0, ROR #4 
FUNC_END MjpegeCheckParam


FUNC_BEGIN MJPEGE_SetParam
    /*00003e50*/ CMP         R0, #15 
    /*00003e54*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00003e58*/ SUB         SP, SP, #16 
    /*00003e5c*/ BHI         MJPEGE_SetParam_x3f04
    /*00003e60*/ CMP         R1, #0 
    /*00003e64*/ BEQ         MJPEGE_SetParam_x3f48
    /*00003e68*/ MOV         R5, R0 
    /*00003e6c*/ MOV         R4, R1 
    /*00003e70*/ BL          0x0000000000003E70          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00003e74*/ SUBS        R7, R0, #0 
    /*00003e78*/ BEQ         MJPEGE_SetParam_x3f54
    /*00003e7c*/ ADD         R6, R7, #6912               @ 0x1b00 
    /*00003e80*/ ADD         R1, SP, #12 
    /*00003e84*/ ADD         R6, R6, #52                 @ 0x34 
    /*00003e88*/ MOV         R0, R6 
    /*00003e8c*/ BL          0x0000000000003E8C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00003e90*/ LDR         R1, [R7, #36]               @ 0x24 
    /*00003e94*/ LDR         R3, [R7, #40]               @ 0x28 
    /*00003e98*/ MOV         R0, R4 
    /*00003e9c*/ ADD         R1, R1, #15 
    /*00003ea0*/ ADD         R3, R3, #15 
    /*00003ea4*/ LSR         R1, R1, #4 
    /*00003ea8*/ LSR         R3, R3, #4 
    /*00003eac*/ MUL         R1, R3, R1 
    /*00003eb0*/ LSL         R1, R1, #1 
    /*00003eb4*/ BL          0x0000000000003EB4          @ MjpegeCheckParam [R_ARM_CALL: .text+0x3d7c] 
    /*00003eb8*/ SUBS        R8, R0, #0 
    /*00003ebc*/ BNE         MJPEGE_SetParam_x3f10
    /*00003ec0*/ MOV         R2, #196                    @ 0xc4 
    /*00003ec4*/ MOV         R1, R4 
    /*00003ec8*/ ADD         R0, R7, #7168               @ 0x1c00 
    /*00003ecc*/ BL          0x0000000000003ECC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00003ed0*/ LDR         IP, [R4, #192]              @ 0xc0 
    /*00003ed4*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003ed8*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003edc*/ ADD         R1, SP, #12 
    /*00003ee0*/ MOV         R0, R6 
    /*00003ee4*/ LDR         R2, [R3, R5, LSL #2] 
    /*00003ee8*/ STR         IP, [R7, #64]               @ 0x40 
    /*00003eec*/ ADD         R2, R2, #1 
    /*00003ef0*/ STR         R2, [R3, R5, LSL #2] 
    /*00003ef4*/ BL          0x0000000000003EF4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    MJPEGE_SetParam_x3ef8:
    /*00003ef8*/ MOV         R0, R8 
    /*00003efc*/ ADD         SP, SP, #16 
    /*00003f00*/ POP         {R4, R5, R6, R7, R8, PC} 
    MJPEGE_SetParam_x3f04:
    /*00003f04*/ MOVW        R8, #32770                  @ 0x8002 
    /*00003f08*/ MOVT        R8, #40971                  @ 0xa00b 
    /*00003f0c*/ B           MJPEGE_SetParam_x3ef8
    MJPEGE_SetParam_x3f10:
    /*00003f10*/ MOVW        R3, #2922                   @ 0xb6a 
    /*00003f14*/ MOV         R1, #11 
    /*00003f18*/ STR         R3, [SP] 
    /*00003f1c*/ MOVW        R2, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc18] 
    /*00003f20*/ LDR         R3, [PC, #56]               @ 0x0000000000003f60 
    /*00003f24*/ MOVT        R2, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc18] 
    /*00003f28*/ MOV         R0, #3 
    /*00003f2c*/ MOVW        R8, #32771                  @ 0x8003 
    /*00003f30*/ BL          0x0000000000003F30          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003f34*/ ADD         R1, SP, #12 
    /*00003f38*/ MOV         R0, R6 
    /*00003f3c*/ MOVT        R8, #40971                  @ 0xa00b 
    /*00003f40*/ BL          0x0000000000003F40          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00003f44*/ B           MJPEGE_SetParam_x3ef8
    MJPEGE_SetParam_x3f48:
    /*00003f48*/ MOVW        R8, #32774                  @ 0x8006 
    /*00003f4c*/ MOVT        R8, #40971                  @ 0xa00b 
    /*00003f50*/ B           MJPEGE_SetParam_x3ef8
    MJPEGE_SetParam_x3f54:
    /*00003f54*/ MOVW        R8, #32773                  @ 0x8005 
    /*00003f58*/ MOVT        R8, #40971                  @ 0xa00b 
    /*00003f5c*/ B           MJPEGE_SetParam_x3ef8
    /*00003f60*/ ANDEQ       R0, R0, R4, ROR R2 
FUNC_END MJPEGE_SetParam


FUNC_BEGIN JPEGE_SetMaxStreamCnt
    /*00003f64*/ PUSH        {R4, R5, R6, LR} 
    /*00003f68*/ SUB         SP, SP, #8 
    /*00003f6c*/ MOV         R6, R1 
    /*00003f70*/ BL          0x0000000000003F70          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00003f74*/ LDR         R3, [R0, #3976]             @ 0xf88 
    /*00003f78*/ CMP         R3, R6 
    /*00003f7c*/ BEQ         JPEGE_SetMaxStreamCnt_x3fa8
    /*00003f80*/ ADD         R5, R0, #6912               @ 0x1b00 
    /*00003f84*/ MOV         R4, R0 
    /*00003f88*/ ADD         R5, R5, #52                 @ 0x34 
    /*00003f8c*/ ADD         R1, SP, #4 
    /*00003f90*/ MOV         R0, R5 
    /*00003f94*/ BL          0x0000000000003F94          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00003f98*/ MOV         R0, R5 
    /*00003f9c*/ ADD         R1, SP, #4 
    /*00003fa0*/ STR         R6, [R4, #3976]             @ 0xf88 
    /*00003fa4*/ BL          0x0000000000003FA4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    JPEGE_SetMaxStreamCnt_x3fa8:
    /*00003fa8*/ MOV         R0, #0 
    /*00003fac*/ ADD         SP, SP, #8 
    /*00003fb0*/ POP         {R4, R5, R6, PC} 
FUNC_END JPEGE_SetMaxStreamCnt


FUNC_BEGIN JPEGE_SetColor2Grey
    /*00003fb4*/ PUSH        {R4, R5, R6, LR} 
    /*00003fb8*/ SUB         SP, SP, #16 
    /*00003fbc*/ MOV         R5, R1 
    /*00003fc0*/ MOV         R4, R0 
    /*00003fc4*/ BL          0x0000000000003FC4          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*00003fc8*/ ADD         R6, R0, #4096               @ 0x1000 
    /*00003fcc*/ LDR         R3, [R6, #3760]             @ 0xeb0 
    /*00003fd0*/ CMP         R3, R5 
    /*00003fd4*/ MOVEQ       R0, #0 
    /*00003fd8*/ BEQ         JPEGE_SetColor2Grey_x400c
    /*00003fdc*/ CMP         R5, #1 
    /*00003fe0*/ BHI         JPEGE_SetColor2Grey_x4014
    /*00003fe4*/ ADD         R4, R0, #6912               @ 0x1b00 
    /*00003fe8*/ ADD         R1, SP, #12 
    /*00003fec*/ ADD         R4, R4, #52                 @ 0x34 
    /*00003ff0*/ MOV         R0, R4 
    /*00003ff4*/ BL          0x0000000000003FF4          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00003ff8*/ MOV         R0, R4 
    /*00003ffc*/ ADD         R1, SP, #12 
    /*00004000*/ STR         R5, [R6, #3760]             @ 0xeb0 
    /*00004004*/ BL          0x0000000000004004          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00004008*/ MOV         R0, #0 
    JPEGE_SetColor2Grey_x400c:
    /*0000400c*/ ADD         SP, SP, #16 
    /*00004010*/ POP         {R4, R5, R6, PC} 
    JPEGE_SetColor2Grey_x4014:
    /*00004014*/ MOVW        R3, #3139                   @ 0xc43 
    /*00004018*/ STR         R4, [SP, #4] 
    /*0000401c*/ STR         R3, [SP] 
    /*00004020*/ MOVW        R2, #0                      @ .LC58 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc4c] 
    /*00004024*/ LDR         R3, [PC, #24]               @ 0x0000000000004044 
    /*00004028*/ MOVT        R2, #0                      @ .LC58 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc4c] 
    /*0000402c*/ MOV         R1, #11 
    /*00004030*/ MOV         R0, #3 
    /*00004034*/ BL          0x0000000000004034          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004038*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000403c*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00004040*/ B           JPEGE_SetColor2Grey_x400c
    /*00004044*/ ANDEQ       R0, R0, R4, LSL #5 
FUNC_END JPEGE_SetColor2Grey


FUNC_BEGIN JPEGE_SetChnParam
    /*00004048*/ CMP         R0, #15 
    /*0000404c*/ PUSH        {R4, R5, R6, LR} 
    /*00004050*/ SUB         SP, SP, #8 
    /*00004054*/ BHI         JPEGE_SetChnParam_x40f4
    /*00004058*/ CMP         R1, #0 
    /*0000405c*/ BEQ         JPEGE_SetChnParam_x4100
    /*00004060*/ MOV         R4, R0 
    /*00004064*/ MOV         R5, R1 
    /*00004068*/ BL          0x0000000000004068          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*0000406c*/ CMP         R0, #0 
    /*00004070*/ BEQ         JPEGE_SetChnParam_x410c
    /*00004074*/ LDR         R1, [R5, #8] 
    /*00004078*/ MOV         R0, R4 
    /*0000407c*/ BL          0x000000000000407C          @ JPEGE_SetMaxStreamCnt [R_ARM_CALL: .text+0x3f64] 
    /*00004080*/ SUBS        R6, R0, #0 
    /*00004084*/ STRNE       R4, [SP, #4] 
    /*00004088*/ MOVWNE      R3, #3182                   @ 0xc6e 
    /*0000408c*/ BNE         JPEGE_SetChnParam_x40cc
    /*00004090*/ LDR         R1, [R5] 
    /*00004094*/ MOV         R0, R4 
    /*00004098*/ BL          0x0000000000004098          @ JPEGE_SetColor2Grey [R_ARM_CALL: .text+0x3fb4] 
    /*0000409c*/ SUBS        R6, R0, #0 
    /*000040a0*/ BNE         JPEGE_SetChnParam_x40c4
    /*000040a4*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000040a8*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000040ac*/ LDR         R2, [R3, R4, LSL #2] 
    /*000040b0*/ ADD         R2, R2, #1 
    /*000040b4*/ STR         R2, [R3, R4, LSL #2] 
    JPEGE_SetChnParam_x40b8:
    /*000040b8*/ MOV         R0, R6 
    /*000040bc*/ ADD         SP, SP, #8 
    /*000040c0*/ POP         {R4, R5, R6, PC} 
    JPEGE_SetChnParam_x40c4:
    /*000040c4*/ MOVW        R3, #3189                   @ 0xc75 
    /*000040c8*/ STR         R4, [SP, #4] 
    JPEGE_SetChnParam_x40cc:
    /*000040cc*/ STR         R3, [SP] 
    /*000040d0*/ MOV         R1, #11 
    /*000040d4*/ LDR         R3, [PC, #60]               @ 0x0000000000004118 
    /*000040d8*/ MOVW        R2, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc8c] 
    /*000040dc*/ MOV         R0, #3 
    /*000040e0*/ MOVT        R2, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc8c] 
    /*000040e4*/ BL          0x00000000000040E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000040e8*/ MOV         R0, R6 
    /*000040ec*/ ADD         SP, SP, #8 
    /*000040f0*/ POP         {R4, R5, R6, PC} 
    JPEGE_SetChnParam_x40f4:
    /*000040f4*/ MOVW        R6, #32770                  @ 0x8002 
    /*000040f8*/ MOVT        R6, #40971                  @ 0xa00b 
    /*000040fc*/ B           JPEGE_SetChnParam_x40b8
    JPEGE_SetChnParam_x4100:
    /*00004100*/ MOVW        R6, #32774                  @ 0x8006 
    /*00004104*/ MOVT        R6, #40971                  @ 0xa00b 
    /*00004108*/ B           JPEGE_SetChnParam_x40b8
    JPEGE_SetChnParam_x410c:
    /*0000410c*/ MOVW        R6, #32773                  @ 0x8005 
    /*00004110*/ MOVT        R6, #40971                  @ 0xa00b 
    /*00004114*/ B           JPEGE_SetChnParam_x40b8
    /*00004118*/ MULEQ       R0, R8, R2 
FUNC_END JPEGE_SetChnParam


FUNC_BEGIN JpegeNotifyFrameOverToVenc
    /*0000411c*/ PUSH        {R4, LR} 
    /*00004120*/ MOV         R4, R0 
    /*00004124*/ MOV         R0, #8 
    /*00004128*/ BL          0x0000000000004128          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000412c*/ LDR         R3, [R0] 
    /*00004130*/ MOV         R0, R4 
    /*00004134*/ BLX         R3 
    /*00004138*/ MOV         R0, #0 
    /*0000413c*/ POP         {R4, PC} 
FUNC_END JpegeNotifyFrameOverToVenc


FUNC_BEGIN JPEGE_ModInit
    /*00004140*/ PUSH        {R4, R5, LR} 
    /*00004144*/ MOVW        R0, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcc4] 
    /*00004148*/ SUB         SP, SP, #12 
    /*0000414c*/ MOV         R1, #0 
    /*00004150*/ MOVT        R0, #0                      @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcc4] 
    /*00004154*/ BL          0x0000000000004154          @ osal_create_proc_entry [R_ARM_CALL: +0x0] 
    /*00004158*/ CMP         R0, #0 
    /*0000415c*/ BEQ         JPEGE_ModInit_x41e8
    /*00004160*/ MOVW        R3, #0                      @ JpegeProcShow [R_ARM_MOVW_ABS_NC: .text+0x617c] 
    /*00004164*/ MOV         R5, #0 
    /*00004168*/ MOVT        R3, #0                      @ JpegeProcShow [R_ARM_MOVT_ABS: .text+0x617c] 
    /*0000416c*/ STR         R5, [R0, #64]               @ 0x40 
    /*00004170*/ STR         R3, [R0, #60]               @ 0x3c 
    /*00004174*/ LDR         R0, [PC, #148]              @ 0x0000000000004210 
    /*00004178*/ BL          0x0000000000004178          @ CMPI_RegisterModule [R_ARM_CALL: +0x0] 
    /*0000417c*/ SUBS        R4, R0, #0 
    /*00004180*/ BNE         JPEGE_ModInit_x41b0
    /*00004184*/ MOV         R1, #1 
    /*00004188*/ LDR         R0, [PC, #132]              @ 0x0000000000004214 
    /*0000418c*/ BL          0x000000000000418C          @ osal_sema_init [R_ARM_CALL: +0x0] 
    /*00004190*/ MOVW        R1, #0                      @ .LC63 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd30] 
    /*00004194*/ MOVW        R0, #0                      @ .LC64 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd3c] 
    /*00004198*/ MOVT        R1, #0                      @ .LC63 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd30] 
    /*0000419c*/ MOVT        R0, #0                      @ .LC64 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd3c] 
    /*000041a0*/ BL          0x00000000000041A0          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000041a4*/ MOV         R0, R4 
    JPEGE_ModInit_x41a8:
    /*000041a8*/ ADD         SP, SP, #12 
    /*000041ac*/ POP         {R4, R5, PC} 
    JPEGE_ModInit_x41b0:
    /*000041b0*/ MOVW        R3, #3461                   @ 0xd85 
    /*000041b4*/ MOV         R1, #11 
    /*000041b8*/ STR         R3, [SP] 
    /*000041bc*/ MOVW        R2, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd00] 
    /*000041c0*/ LDR         R3, [PC, #80]               @ 0x0000000000004218 
    /*000041c4*/ MOVT        R2, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd00] 
    /*000041c8*/ MOV         R0, #3 
    /*000041cc*/ BL          0x00000000000041CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000041d0*/ MOV         R1, R5 
    /*000041d4*/ MOVW        R0, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcc4] 
    /*000041d8*/ MOVT        R0, #0                      @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcc4] 
    /*000041dc*/ BL          0x00000000000041DC          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*000041e0*/ MVN         R0, #0 
    /*000041e4*/ B           JPEGE_ModInit_x41a8
    JPEGE_ModInit_x41e8:
    /*000041e8*/ MOVW        R3, #3450                   @ 0xd7a 
    /*000041ec*/ MOVW        R2, #0                      @ .LC61 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xccc] 
    /*000041f0*/ STR         R3, [SP] 
    /*000041f4*/ MOVT        R2, #0                      @ .LC61 [R_ARM_MOVT_ABS: .rodata.str1.4+0xccc] 
    /*000041f8*/ LDR         R3, [PC, #24]               @ 0x0000000000004218 
    /*000041fc*/ MOV         R1, #11 
    /*00004200*/ MOV         R0, #3 
    /*00004204*/ BL          0x0000000000004204          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004208*/ MVN         R0, #0 
    /*0000420c*/ B           JPEGE_ModInit_x41a8
    /*00004210*/ ANDEQ       R0, R0, R4 
    /*00004214*/ ANDEQ       R0, R0, R8, ASR #32 
    /*00004218*/ ANDEQ       R0, R0, IP, LSR #5 
FUNC_END JPEGE_ModInit


FUNC_BEGIN JPEGE_ModExit
    /*0000421c*/ PUSH        {R4, LR} 
    /*00004220*/ MOV         R0, #11 
    /*00004224*/ BL          0x0000000000004224          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    /*00004228*/ MOV         R1, #0 
    /*0000422c*/ MOVW        R0, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcc4] 
    /*00004230*/ MOVT        R0, #0                      @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcc4] 
    /*00004234*/ BL          0x0000000000004234          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*00004238*/ LDR         R0, [PC, #24]               @ 0x0000000000004258 
    /*0000423c*/ BL          0x000000000000423C          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*00004240*/ MOVW        R1, #0                      @ .LC63 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd30] 
    /*00004244*/ MOVW        R0, #0                      @ .LC65 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd58] 
    /*00004248*/ MOVT        R1, #0                      @ .LC63 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd30] 
    /*0000424c*/ MOVT        R0, #0                      @ .LC65 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd58] 
    /*00004250*/ POP         {R4, LR} 
    JPEGE_ModExit_x4254:
    /*00004254*/ B           JPEGE_ModExit_x4254         @ osal_printk [R_ARM_JUMP24: +0x0] 
    /*00004258*/ ANDEQ       R0, R0, R8, ASR #32 
FUNC_END JPEGE_ModExit


FUNC_BEGIN JpegeUpdateVpuConfig
    /*0000425c*/ MOVW        R3, #0                      @ g_pstJpegeChnCtx [R_ARM_MOVW_ABS_NC: .bss+0x4c] 
    /*00004260*/ MOVT        R3, #0                      @ g_pstJpegeChnCtx [R_ARM_MOVT_ABS: .bss+0x4c] 
    /*00004264*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00004268*/ SUB         SP, SP, #36                 @ 0x24 
    /*0000426c*/ LDR         R4, [R3, R0, LSL #2] 
    /*00004270*/ MOV         R8, R0 
    /*00004274*/ LDR         R6, [R1, #12] 
    /*00004278*/ ADD         R5, R4, #4096               @ 0x1000 
    /*0000427c*/ STR         R1, [SP, #4] 
    /*00004280*/ LDR         R3, [R4, #3040]             @ 0xbe0 
    /*00004284*/ ADD         R7, R4, #4032               @ 0xfc0 
    /*00004288*/ LDR         SL, [R4, #3044]             @ 0xbe4 
    /*0000428c*/ ADD         R3, R3, #15 
    /*00004290*/ LDR         R2, [R4, #3916]             @ 0xf4c 
    /*00004294*/ ADD         SL, SL, #15 
    /*00004298*/ LDR         FP, [R4, #64]               @ 0x40 
    /*0000429c*/ LSR         R3, R3, #4 
    /*000042a0*/ CMP         R2, #0 
    /*000042a4*/ LSR         SL, SL, #4 
    /*000042a8*/ STR         R0, [R4, #4032]             @ 0xfc0 
    /*000042ac*/ STR         R2, [R5, #2824]             @ 0xb08 
    /*000042b0*/ MOVNE       FP, #0 
    /*000042b4*/ MUL         SL, SL, R3 
    /*000042b8*/ LDR         R2, [R6, #12] 
    /*000042bc*/ CMP         R2, #25 
    /*000042c0*/ MOVEQ       R3, #1 
    /*000042c4*/ STREQ       R3, [R4, #4036]             @ 0xfc4 
    /*000042c8*/ LSLEQ       SL, SL, #1 
    /*000042cc*/ BEQ         JpegeUpdateVpuConfig_x4310
    /*000042d0*/ CMP         R2, #26 
    /*000042d4*/ MOVEQ       R3, #0 
    /*000042d8*/ STREQ       R3, [R4, #4036]             @ 0xfc4 
    /*000042dc*/ BEQ         JpegeUpdateVpuConfig_x4310
    /*000042e0*/ CMP         R2, #39                     @ 0x27 
    /*000042e4*/ MOVEQ       R3, #3 
    /*000042e8*/ STREQ       R3, [R4, #4036]             @ 0xfc4 
    /*000042ec*/ BEQ         JpegeUpdateVpuConfig_x4310
    /*000042f0*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd78] 
    /*000042f4*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x2bc] 
    /*000042f8*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*000042fc*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd78] 
    /*00004300*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x2bc] 
    /*00004304*/ MOVW        R2, #1156                   @ 0x484 
    /*00004308*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*0000430c*/ BL          0x000000000000430C          @ osal_panic [R_ARM_CALL: +0x0] 
    JpegeUpdateVpuConfig_x4310:
    /*00004310*/ CMP         FP, #0 
    /*00004314*/ BEQ         JpegeUpdateVpuConfig_x4578
    /*00004318*/ CMP         FP, SL 
    /*0000431c*/ STR         FP, [R4, #3048]             @ 0xbe8 
    /*00004320*/ SUBCC       FP, FP, #1 
    /*00004324*/ STRCC       FP, [R5, #92]               @ 0x5c 
    /*00004328*/ MOVCC       R3, #1 
    /*0000432c*/ STRCC       R3, [R5, #88]               @ 0x58 
    /*00004330*/ BCS         JpegeUpdateVpuConfig_x457c
    JpegeUpdateVpuConfig_x4334:
    /*00004334*/ LDR         IP, [R4, #452]              @ 0x1c4 
    /*00004338*/ MOV         R3, #0 
    /*0000433c*/ LDRD        SL, [R4, #8] 
    /*00004340*/ ADD         LR, IP, #128                @ 0x80 
    /*00004344*/ LDR         R2, [R4] 
    /*00004348*/ ADD         R1, IP, #64                 @ 0x40 
    /*0000434c*/ MOV         R9, LR 
    /*00004350*/ LDR         LR, [IP, #176]              @ 0xb0 
    /*00004354*/ STRD        SL, [SP, #8] 
    /*00004358*/ SUBS        SL, R1, R2 
    /*0000435c*/ RSC         FP, R3, #0 
    /*00004360*/ SUBS        R0, LR, R2 
    /*00004364*/ STRD        SL, [SP, #24] 
    /*00004368*/ RSC         R1, R3, #0 
    /*0000436c*/ SUBS        SL, R9, R2 
    /*00004370*/ RSC         FP, R3, #0 
    /*00004374*/ STRD        SL, [SP, #16] 
    /*00004378*/ LDRD        SL, [SP, #8] 
    /*0000437c*/ ADDS        R2, SL, R0 
    /*00004380*/ ADC         R3, FP, R1 
    /*00004384*/ LDRD        R0, [SP, #24] 
    /*00004388*/ STRD        R2, [R7, #88]               @ 0x58 
    /*0000438c*/ AND         R2, R2, #63                 @ 0x3f 
    /*00004390*/ MOV         R3, #0 
    /*00004394*/ STRD        R2, [SP, #8] 
    /*00004398*/ ADDS        R2, SL, R0 
    /*0000439c*/ LDR         IP, [IP, #8] 
    /*000043a0*/ ADC         R3, FP, R1 
    /*000043a4*/ MOV         R0, R2 
    /*000043a8*/ MOV         R1, R3 
    /*000043ac*/ LDRD        R2, [SP, #16] 
    /*000043b0*/ STR         IP, [R5, #32] 
    /*000043b4*/ ADDS        R2, R2, SL 
    /*000043b8*/ STRD        R0, [R7, #104]              @ 0x68 
    /*000043bc*/ ADC         R3, R3, FP 
    /*000043c0*/ LDRD        SL, [SP, #8] 
    /*000043c4*/ STRD        R2, [R7, #112]              @ 0x70 
    /*000043c8*/ ORRS        LR, SL, FP 
    /*000043cc*/ BNE         JpegeUpdateVpuConfig_x46b8
    JpegeUpdateVpuConfig_x43d0:
    /*000043d0*/ TST         IP, #63                     @ 0x3f 
    /*000043d4*/ BNE         JpegeUpdateVpuConfig_x4694
    JpegeUpdateVpuConfig_x43d8:
    /*000043d8*/ LDRD        R2, [R7, #104]              @ 0x68 
    /*000043dc*/ MOV         R1, #0 
    /*000043e0*/ AND         R0, R2, #63                 @ 0x3f 
    /*000043e4*/ ORRS        R3, R0, R1 
    /*000043e8*/ BNE         JpegeUpdateVpuConfig_x4670
    JpegeUpdateVpuConfig_x43ec:
    /*000043ec*/ LDRD        R2, [R7, #112]              @ 0x70 
    /*000043f0*/ MOV         R1, #0 
    /*000043f4*/ AND         R0, R2, #63                 @ 0x3f 
    /*000043f8*/ ORRS        R3, R0, R1 
    /*000043fc*/ BNE         JpegeUpdateVpuConfig_x4620
    /*00004400*/ LDR         R3, [R5, #3720]             @ 0xe88 
    /*00004404*/ CMP         R3, #1 
    /*00004408*/ BEQ         JpegeUpdateVpuConfig_x464c
    JpegeUpdateVpuConfig_x440c:
    /*0000440c*/ MOV         R3, #0 
    /*00004410*/ MOV         R2, #0 
    /*00004414*/ STR         R3, [R5, #60]               @ 0x3c 
    /*00004418*/ STR         R3, [R5, #64]               @ 0x40 
    /*0000441c*/ MOV         R3, #0 
    /*00004420*/ STRD        R2, [R7, #136]              @ 0x88 
    JpegeUpdateVpuConfig_x4424:
    /*00004424*/ LDR         R3, [R6] 
    /*00004428*/ MOV         R1, #0 
    /*0000442c*/ SUB         R3, R3, #1 
    /*00004430*/ STR         R3, [R4, #4040]             @ 0xfc8 
    /*00004434*/ LDR         R3, [R6, #4] 
    /*00004438*/ SUB         R3, R3, #1 
    /*0000443c*/ STR         R3, [R4, #4044]             @ 0xfcc 
    /*00004440*/ LDRD        R2, [R6, #72]               @ 0x48 
    /*00004444*/ STRD        R2, [R7, #16] 
    /*00004448*/ LDRD        R2, [R6, #80]               @ 0x50 
    /*0000444c*/ STRD        R2, [R7, #24] 
    /*00004450*/ LDRD        R2, [R6, #120]              @ 0x78 
    /*00004454*/ AND         R0, R2, #15 
    /*00004458*/ STRD        R2, [R7, #32] 
    /*0000445c*/ ORRS        R3, R0, R1 
    /*00004460*/ LDRD        SL, [R6, #128]              @ 0x80 
    /*00004464*/ STRD        SL, [R7, #40]               @ 0x28 
    /*00004468*/ LDRD        R2, [R6, #168]              @ 0xa8 
    /*0000446c*/ STRD        R2, [R7, #48]               @ 0x30 
    /*00004470*/ LDRD        R2, [R6, #176]              @ 0xb0 
    /*00004474*/ STRD        R2, [R7, #56]               @ 0x38 
    /*00004478*/ LDR         R3, [R6, #44]               @ 0x2c 
    /*0000447c*/ STR         R3, [R5] 
    /*00004480*/ LDR         R3, [R6, #48]               @ 0x30 
    /*00004484*/ STR         R3, [R5, #4] 
    /*00004488*/ LDR         R3, [R6, #56]               @ 0x38 
    /*0000448c*/ STR         R3, [R5, #8] 
    /*00004490*/ LDR         R3, [R6, #60]               @ 0x3c 
    /*00004494*/ STR         R3, [R5, #12] 
    /*00004498*/ LDR         R3, [R6, #32] 
    /*0000449c*/ STR         R3, [R5, #16] 
    /*000044a0*/ BNE         JpegeUpdateVpuConfig_x45f8
    JpegeUpdateVpuConfig_x44a4:
    /*000044a4*/ AND         R0, SL, #15 
    /*000044a8*/ MOV         R1, #0 
    /*000044ac*/ ORRS        R3, R0, R1 
    /*000044b0*/ BNE         JpegeUpdateVpuConfig_x45d4
    JpegeUpdateVpuConfig_x44b4:
    /*000044b4*/ LDR         R3, [R5] 
    /*000044b8*/ TST         R3, #15 
    /*000044bc*/ BNE         JpegeUpdateVpuConfig_x45b0
    JpegeUpdateVpuConfig_x44c0:
    /*000044c0*/ LDR         R3, [R5, #4] 
    /*000044c4*/ TST         R3, #15 
    /*000044c8*/ BNE         JpegeUpdateVpuConfig_x458c
    JpegeUpdateVpuConfig_x44cc:
    /*000044cc*/ LDR         R3, [SP, #4] 
    /*000044d0*/ MOV         R1, R7 
    /*000044d4*/ MOV         R0, R8 
    /*000044d8*/ LDR         R2, [R3, #12] 
    /*000044dc*/ BL          0x00000000000044DC          @ JPEGE_DRV_GetCmpSegCfg [R_ARM_CALL: .text+0x69b4] 
    /*000044e0*/ LDR         R0, [R6, #240]              @ 0xf0 
    /*000044e4*/ ADD         R1, R4, #3104               @ 0xc20 
    /*000044e8*/ ADD         R2, R4, #3040               @ 0xbe0 
    /*000044ec*/ ADD         R1, R1, #12 
    /*000044f0*/ ADD         R3, R4, #4160               @ 0x1040 
    /*000044f4*/ STR         R0, [R5, #80]               @ 0x50 
    /*000044f8*/ ADD         R0, R4, #3168               @ 0xc60 
    /*000044fc*/ LDR         IP, [R6, #244]              @ 0xf4 
    /*00004500*/ ADD         R2, R2, #12 
    /*00004504*/ ADD         R3, R3, #28 
    /*00004508*/ ADD         R0, R0, #12 
    /*0000450c*/ MOV         LR, R1 
    /*00004510*/ STR         IP, [R5, #84]               @ 0x54 
    JpegeUpdateVpuConfig_x4514:
    /*00004514*/ LDR         IP, [R2, #4]! 
    /*00004518*/ CMP         LR, R2 
    /*0000451c*/ STR         IP, [R3, #4]! 
    /*00004520*/ LDR         IP, [R1, #4]! 
    /*00004524*/ STR         IP, [R3, #64]               @ 0x40 
    /*00004528*/ LDR         IP, [R0, #4]! 
    /*0000452c*/ STR         IP, [R3, #128]              @ 0x80 
    /*00004530*/ BNE         JpegeUpdateVpuConfig_x4514
    /*00004534*/ ADD         R3, R4, #3040               @ 0xbe0 
    /*00004538*/ ADD         R4, R4, #3104               @ 0xc20 
    /*0000453c*/ ADD         R3, R3, #15 
    /*00004540*/ ADD         R4, R4, #15 
    JpegeUpdateVpuConfig_x4544:
    /*00004544*/ LDRB        R0, [R3, #1]! 
    /*00004548*/ CMP         R4, R3 
    /*0000454c*/ LDRB        R1, [R3, #64]               @ 0x40 
    /*00004550*/ LDRB        R2, [R3, #128]              @ 0x80 
    /*00004554*/ STRB        R0, [R3, #1328]             @ 0x530 
    /*00004558*/ STRB        R1, [R3, #1392]             @ 0x570 
    /*0000455c*/ STRB        R2, [R3, #1456]             @ 0x5b0 
    /*00004560*/ BNE         JpegeUpdateVpuConfig_x4544
    /*00004564*/ LDR         R3, [R5, #3760]             @ 0xeb0 
    /*00004568*/ MOV         R0, #0 
    /*0000456c*/ STR         R3, [R5, #2816]             @ 0xb00 
    /*00004570*/ ADD         SP, SP, #36                 @ 0x24 
    /*00004574*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JpegeUpdateVpuConfig_x4578:
    /*00004578*/ STR         SL, [R4, #3048]             @ 0xbe8 
    JpegeUpdateVpuConfig_x457c:
    /*0000457c*/ MOV         R3, #0 
    /*00004580*/ STR         R3, [R5, #88]               @ 0x58 
    /*00004584*/ STR         R3, [R5, #92]               @ 0x5c 
    /*00004588*/ B           JpegeUpdateVpuConfig_x4334
    JpegeUpdateVpuConfig_x458c:
    /*0000458c*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfa0] 
    /*00004590*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x2bc] 
    /*00004594*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfa0] 
    /*00004598*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x2bc] 
    /*0000459c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*000045a0*/ MOVW        R2, #1231                   @ 0x4cf 
    /*000045a4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*000045a8*/ BL          0x00000000000045A8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000045ac*/ B           JpegeUpdateVpuConfig_x44cc
    JpegeUpdateVpuConfig_x45b0:
    /*000045b0*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf60] 
    /*000045b4*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x2bc] 
    /*000045b8*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf60] 
    /*000045bc*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x2bc] 
    /*000045c0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*000045c4*/ MOVW        R2, #1230                   @ 0x4ce 
    /*000045c8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*000045cc*/ BL          0x00000000000045CC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000045d0*/ B           JpegeUpdateVpuConfig_x44c0
    JpegeUpdateVpuConfig_x45d4:
    /*000045d4*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf1c] 
    /*000045d8*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x2bc] 
    /*000045dc*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf1c] 
    /*000045e0*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x2bc] 
    /*000045e4*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*000045e8*/ MOVW        R2, #1229                   @ 0x4cd 
    /*000045ec*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*000045f0*/ BL          0x00000000000045F0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000045f4*/ B           JpegeUpdateVpuConfig_x44b4
    JpegeUpdateVpuConfig_x45f8:
    /*000045f8*/ MOVW        R3, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xed8] 
    /*000045fc*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x2bc] 
    /*00004600*/ MOVT        R3, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0xed8] 
    /*00004604*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x2bc] 
    /*00004608*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*0000460c*/ MOVW        R2, #1228                   @ 0x4cc 
    /*00004610*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00004614*/ BL          0x0000000000004614          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004618*/ LDRD        SL, [R7, #40]               @ 0x28 
    /*0000461c*/ B           JpegeUpdateVpuConfig_x44a4
    JpegeUpdateVpuConfig_x4620:
    /*00004620*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe90] 
    /*00004624*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x2bc] 
    /*00004628*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe90] 
    /*0000462c*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x2bc] 
    /*00004630*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00004634*/ MOVW        R2, #1193                   @ 0x4a9 
    /*00004638*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*0000463c*/ BL          0x000000000000463C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004640*/ LDR         R3, [R5, #3720]             @ 0xe88 
    /*00004644*/ CMP         R3, #1 
    /*00004648*/ BNE         JpegeUpdateVpuConfig_x440c
    JpegeUpdateVpuConfig_x464c:
    /*0000464c*/ MOVW        R2, #7824                   @ 0x1e90 
    /*00004650*/ STR         R3, [R5, #60]               @ 0x3c 
    /*00004654*/ MOV         R3, #50                     @ 0x32 
    /*00004658*/ STR         R3, [R5, #64]               @ 0x40 
    /*0000465c*/ LDRD        R2, [R2, R4] 
    /*00004660*/ MOV         R1, #0 
    /*00004664*/ MOV         R0, R2 
    /*00004668*/ STRD        R0, [R7, #136]              @ 0x88 
    /*0000466c*/ B           JpegeUpdateVpuConfig_x4424
    JpegeUpdateVpuConfig_x4670:
    /*00004670*/ MOVW        R3, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe48] 
    /*00004674*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x2bc] 
    /*00004678*/ MOVT        R3, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe48] 
    /*0000467c*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x2bc] 
    /*00004680*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00004684*/ MOVW        R2, #1192                   @ 0x4a8 
    /*00004688*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*0000468c*/ BL          0x000000000000468C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004690*/ B           JpegeUpdateVpuConfig_x43ec
    JpegeUpdateVpuConfig_x4694:
    /*00004694*/ MOVW        R3, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe00] 
    /*00004698*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x2bc] 
    /*0000469c*/ MOVT        R3, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe00] 
    /*000046a0*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x2bc] 
    /*000046a4*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*000046a8*/ MOVW        R2, #1191                   @ 0x4a7 
    /*000046ac*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*000046b0*/ BL          0x00000000000046B0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000046b4*/ B           JpegeUpdateVpuConfig_x43d8
    JpegeUpdateVpuConfig_x46b8:
    /*000046b8*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdbc] 
    /*000046bc*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x2bc] 
    /*000046c0*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdbc] 
    /*000046c4*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x2bc] 
    /*000046c8*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*000046cc*/ MOVW        R2, #1190                   @ 0x4a6 
    /*000046d0*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*000046d4*/ BL          0x00000000000046D4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000046d8*/ LDR         IP, [R5, #32] 
    /*000046dc*/ B           JpegeUpdateVpuConfig_x43d0
FUNC_END JpegeUpdateVpuConfig


FUNC_BEGIN jpege_math_log2.part.0
    /*000046e0*/ CMP         R0, #65536                  @ 0x10000 
    /*000046e4*/ LSRCS       R3, R0, #16 
    /*000046e8*/ MOVCC       R3, R0 
    /*000046ec*/ PUSH        {R4, R5, LR} 
    /*000046f0*/ MOVCS       R5, #16 
    /*000046f4*/ MOVCC       R5, #0 
    /*000046f8*/ CMP         R3, #255                    @ 0xff 
    /*000046fc*/ ADDHI       R4, R5, #8 
    /*00004700*/ LSRHI       R3, R3, #8 
    /*00004704*/ UXTBHI      R5, R4 
    /*00004708*/ CMP         R3, #15 
    /*0000470c*/ ADDHI       R4, R5, #4 
    /*00004710*/ LSRHI       R3, R3, #4 
    /*00004714*/ UXTBHI      R5, R4 
    /*00004718*/ CMP         R3, #3 
    /*0000471c*/ ADDHI       R4, R5, #2 
    /*00004720*/ LSRHI       R3, R3, #2 
    /*00004724*/ UXTBHI      R5, R4 
    /*00004728*/ CMP         R3, #1 
    /*0000472c*/ ADDHI       R4, R5, #1 
    /*00004730*/ UXTBHI      R5, R4 
    /*00004734*/ CMP         R5, #15 
    /*00004738*/ RSBLS       R3, R5, #15 
    /*0000473c*/ SUBHI       R3, R5, #15 
    /*00004740*/ LSLLS       R3, R0, R3 
    /*00004744*/ LSRHI       R3, R0, R3 
    /*00004748*/ CMP         R1, #0 
    /*0000474c*/ BEQ         jpege_math_log2.part.0_x4788
    /*00004750*/ MOV         LR, #0 
    /*00004754*/ MOV         R4, LR 
    jpege_math_log2.part.0_x4758:
    /*00004758*/ MUL         IP, R3, R3 
    /*0000475c*/ LSL         R4, R4, #1 
    /*00004760*/ ADD         LR, LR, #1 
    /*00004764*/ CMP         IP, #0 
    /*00004768*/ ADDLT       R4, R4, #1 
    /*0000476c*/ LSR         R3, IP, #15 
    /*00004770*/ LSRLT       R3, IP, #16 
    /*00004774*/ CMP         LR, R1 
    /*00004778*/ BNE         jpege_math_log2.part.0_x4758
    jpege_math_log2.part.0_x477c:
    /*0000477c*/ ADD         R4, R4, R5, LSL R1 
    /*00004780*/ LSL         R0, R4, R2 
    /*00004784*/ POP         {R4, R5, PC} 
    jpege_math_log2.part.0_x4788:
    /*00004788*/ MOV         R4, R1 
    /*0000478c*/ B           jpege_math_log2.part.0_x477c
FUNC_END jpege_math_log2.part.0


FUNC_BEGIN JpegeGetChnCtxPtr
    /*00004790*/ MOVW        R3, #0                      @ g_pstJpegeChnCtx [R_ARM_MOVW_ABS_NC: .bss+0x4c] 
    /*00004794*/ MOVT        R3, #0                      @ g_pstJpegeChnCtx [R_ARM_MOVT_ABS: .bss+0x4c] 
    /*00004798*/ LDR         R0, [R3, R0, LSL #2] 
    /*0000479c*/ BX          LR 
FUNC_END JpegeGetChnCtxPtr


FUNC_BEGIN JpegeInitPicPar
    /*000047a0*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000047a4*/ SUB         SP, SP, #220                @ 0xdc 
    /*000047a8*/ MOV         R5, R1 
    /*000047ac*/ LDR         R4, [R1, #64]               @ 0x40 
    /*000047b0*/ ADD         R2, R1, #3248               @ 0xcb0 
    /*000047b4*/ LDR         R3, [R5, #36]               @ 0x24 
    /*000047b8*/ STR         R1, [SP, #8] 
    /*000047bc*/ MOV         LR, #0 
    /*000047c0*/ LDR         R1, [R1, #40]               @ 0x28 
    /*000047c4*/ MOV         R6, R2 
    /*000047c8*/ STR         R2, [SP, #12] 
    /*000047cc*/ MOV         IP, #8 
    /*000047d0*/ STR         R0, [R5, #468]              @ 0x1d4 
    /*000047d4*/ MOVW        R2, #658                    @ 0x292 
    /*000047d8*/ STR         R1, [R5, #3044]             @ 0xbe4 
    /*000047dc*/ MOV         R0, R6 
    /*000047e0*/ STR         R1, [R5, #3028]             @ 0xbd4 
    /*000047e4*/ STR         R3, [R5, #3040]             @ 0xbe0 
    /*000047e8*/ STR         R3, [R5, #3024]             @ 0xbd0 
    /*000047ec*/ STR         R4, [R5, #3048]             @ 0xbe8 
    /*000047f0*/ STR         LR, [R5, #3036]             @ 0xbdc 
    /*000047f4*/ STRB        IP, [R5, #464]              @ 0x1d0 
    /*000047f8*/ LDR         R1, [PC, #860]              @ 0x0000000000004b5c 
    /*000047fc*/ BL          0x00000000000047FC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004800*/ LDRH        R2, [R6] 
    /*00004804*/ MOVW        R3, #56319                  @ 0xdbff 
    /*00004808*/ CMP         R2, R3 
    /*0000480c*/ BEQ         JpegeInitPicPar_x482c
    /*00004810*/ MOVW        R3, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfe0] 
    /*00004814*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00004818*/ MOVT        R3, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfe0] 
    /*0000481c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00004820*/ MOV         R2, #136                    @ 0x88 
    /*00004824*/ LDR         R1, [PC, #820]              @ 0x0000000000004b60 
    /*00004828*/ BL          0x0000000000004828          @ osal_panic [R_ARM_CALL: +0x0] 
    JpegeInitPicPar_x482c:
    /*0000482c*/ LDR         R3, [SP, #8] 
    /*00004830*/ ADD         R3, R3, #3440               @ 0xd70 
    /*00004834*/ MOV         R2, R3 
    /*00004838*/ ADD         R2, R2, #7 
    /*0000483c*/ STR         R2, [SP, #16] 
    /*00004840*/ MOVW        R3, #49407                  @ 0xc0ff 
    /*00004844*/ LDRH        R2, [R2] 
    /*00004848*/ CMP         R2, R3 
    /*0000484c*/ BEQ         JpegeInitPicPar_x486c
    /*00004850*/ MOVW        R3, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xffc] 
    /*00004854*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00004858*/ MOVT        R3, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0xffc] 
    /*0000485c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00004860*/ MOV         R2, #137                    @ 0x89 
    /*00004864*/ LDR         R1, [PC, #756]              @ 0x0000000000004b60 
    /*00004868*/ BL          0x0000000000004868          @ osal_panic [R_ARM_CALL: +0x0] 
    JpegeInitPicPar_x486c:
    /*0000486c*/ LDR         R3, [SP, #8] 
    /*00004870*/ MOVW        R2, #50431                  @ 0xc4ff 
    /*00004874*/ ADD         R3, R3, #3456               @ 0xd80 
    /*00004878*/ ADD         R3, R3, #10 
    /*0000487c*/ LDRH        R3, [R3] 
    /*00004880*/ CMP         R3, R2 
    /*00004884*/ BEQ         JpegeInitPicPar_x48a4
    /*00004888*/ MOVW        R3, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1018] 
    /*0000488c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00004890*/ MOVT        R3, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1018] 
    /*00004894*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00004898*/ MOV         R2, #138                    @ 0x8a 
    /*0000489c*/ LDR         R1, [PC, #700]              @ 0x0000000000004b60 
    /*000048a0*/ BL          0x00000000000048A0          @ osal_panic [R_ARM_CALL: +0x0] 
    JpegeInitPicPar_x48a4:
    /*000048a4*/ LDR         R3, [SP, #8] 
    /*000048a8*/ ADD         R3, R3, #3872               @ 0xf20 
    /*000048ac*/ MOV         R2, R3 
    /*000048b0*/ ADD         R2, R2, #14 
    /*000048b4*/ STR         R2, [SP, #20] 
    /*000048b8*/ MOVW        R3, #56831                  @ 0xddff 
    /*000048bc*/ LDRH        R2, [R2] 
    /*000048c0*/ CMP         R2, R3 
    /*000048c4*/ BEQ         JpegeInitPicPar_x48e4
    /*000048c8*/ MOVW        R3, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1034] 
    /*000048cc*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*000048d0*/ MOVT        R3, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1034] 
    /*000048d4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*000048d8*/ MOV         R2, #139                    @ 0x8b 
    /*000048dc*/ LDR         R1, [PC, #636]              @ 0x0000000000004b60 
    /*000048e0*/ BL          0x00000000000048E0          @ osal_panic [R_ARM_CALL: +0x0] 
    JpegeInitPicPar_x48e4:
    /*000048e4*/ LDR         R3, [SP, #8] 
    /*000048e8*/ MOVW        R2, #56063                  @ 0xdaff 
    /*000048ec*/ ADD         R3, R3, #3888               @ 0xf30 
    /*000048f0*/ ADD         R3, R3, #4 
    /*000048f4*/ LDRH        R3, [R3] 
    /*000048f8*/ CMP         R3, R2 
    /*000048fc*/ BEQ         JpegeInitPicPar_x491c
    /*00004900*/ MOVW        R3, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1050] 
    /*00004904*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00004908*/ MOVT        R3, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1050] 
    /*0000490c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00004910*/ MOV         R2, #140                    @ 0x8c 
    /*00004914*/ LDR         R1, [PC, #580]              @ 0x0000000000004b60 
    /*00004918*/ BL          0x0000000000004918          @ osal_panic [R_ARM_CALL: +0x0] 
    JpegeInitPicPar_x491c:
    /*0000491c*/ LDR         R3, [SP, #8] 
    /*00004920*/ LDR         R1, [R3, #68]               @ 0x44 
    /*00004924*/ CMP         R1, #0 
    /*00004928*/ BLE         JpegeInitPicPar_x4b38
    /*0000492c*/ CMP         R1, #99                     @ 0x63 
    /*00004930*/ MOVGT       R0, #2 
    /*00004934*/ BLE         JpegeInitPicPar_x4b48
    JpegeInitPicPar_x4938:
    /*00004938*/ LDR         R3, [SP, #8] 
    /*0000493c*/ MOVW        R2, #34079                  @ 0x851f 
    /*00004940*/ MOVT        R2, #20971                  @ 0x51eb 
    /*00004944*/ ADD         LR, R3, #3104               @ 0xc20 
    /*00004948*/ ADD         IP, R3, #3168               @ 0xc60 
    /*0000494c*/ ADD         LR, LR, #15 
    /*00004950*/ ADD         IP, IP, #15 
    /*00004954*/ ADD         R1, R3, #71                 @ 0x47 
    /*00004958*/ ADD         R9, R3, #199                @ 0xc7 
    /*0000495c*/ ADD         R6, R3, #3040               @ 0xbe0 
    /*00004960*/ ADD         R8, R3, #135                @ 0x87 
    /*00004964*/ ADD         R6, R6, #15 
    /*00004968*/ STM         SP, {R6, R8} 
    JpegeInitPicPar_x496c:
    /*0000496c*/ LDRB        FP, [R1, #1]! 
    /*00004970*/ LDRB        SL, [R8, #1]! 
    /*00004974*/ LDRB        R3, [R9, #1]! 
    /*00004978*/ MUL         FP, R0, FP 
    /*0000497c*/ MUL         SL, R0, SL 
    /*00004980*/ ADD         FP, FP, #50                 @ 0x32 
    /*00004984*/ MUL         R3, R0, R3 
    /*00004988*/ ADD         SL, SL, #50                 @ 0x32 
    /*0000498c*/ SMULL       R4, R5, FP, R2 
    /*00004990*/ ADD         R3, R3, #50                 @ 0x32 
    /*00004994*/ SMULL       R6, R7, SL, R2 
    /*00004998*/ ASR         FP, FP, #31 
    /*0000499c*/ ASR         SL, SL, #31 
    /*000049a0*/ LDR         R6, [SP] 
    /*000049a4*/ RSB         FP, FP, R5, ASR #5 
    /*000049a8*/ CMP         FP, #255                    @ 0xff 
    /*000049ac*/ SMULL       R4, R5, R3, R2 
    /*000049b0*/ MOVGE       FP, #255                    @ 0xff 
    /*000049b4*/ CMP         FP, #1 
    /*000049b8*/ RSB         SL, SL, R7, ASR #5 
    /*000049bc*/ ASR         R3, R3, #31 
    /*000049c0*/ LDR         R4, [SP, #4] 
    /*000049c4*/ MOVLT       FP, #1 
    /*000049c8*/ CMP         SL, #255                    @ 0xff 
    /*000049cc*/ STRB        FP, [R6, #1]! 
    /*000049d0*/ MOVGE       SL, #255                    @ 0xff 
    /*000049d4*/ CMP         SL, #1 
    /*000049d8*/ RSB         R3, R3, R5, ASR #5 
    /*000049dc*/ STR         R6, [SP] 
    /*000049e0*/ MOVLT       SL, #1 
    /*000049e4*/ CMP         R3, #255                    @ 0xff 
    /*000049e8*/ STRB        SL, [LR, #1]! 
    /*000049ec*/ MOVGE       R3, #255                    @ 0xff 
    /*000049f0*/ CMP         R3, #1 
    /*000049f4*/ MOVLT       R3, #1 
    /*000049f8*/ CMP         R4, R1 
    /*000049fc*/ STRB        R3, [IP, #1]! 
    /*00004a00*/ BNE         JpegeInitPicPar_x496c
    /*00004a04*/ LDR         R5, [PC, #344]              @ 0x0000000000004b64 
    /*00004a08*/ LDR         R3, [SP, #8] 
    /*00004a0c*/ SUB         R0, R5, #63                 @ 0x3f 
    /*00004a10*/ ADD         R2, R3, #3120               @ 0xc30 
    /*00004a14*/ MOV         R6, R5 
    /*00004a18*/ MOV         R3, #63                     @ 0x3f 
    /*00004a1c*/ B           JpegeInitPicPar_x4a24
    JpegeInitPicPar_x4a20:
    /*00004a20*/ LDRB        R3, [R6, #-1]! 
    JpegeInitPicPar_x4a24:
    /*00004a24*/ LDRB        R1, [R2, #-1]! 
    /*00004a28*/ ADD         IP, SP, #216                @ 0xd8 
    /*00004a2c*/ ADD         R3, IP, R3 
    /*00004a30*/ CMP         R0, R6 
    /*00004a34*/ STRB        R1, [R3, #-192]             @ 0xffffff40 
    /*00004a38*/ BNE         JpegeInitPicPar_x4a20
    /*00004a3c*/ LDR         R3, [SP, #12] 
    /*00004a40*/ MOV         R2, #64                     @ 0x40 
    /*00004a44*/ ADD         R1, SP, #24 
    /*00004a48*/ MOV         R4, R5 
    /*00004a4c*/ ADD         R0, R3, #5 
    /*00004a50*/ BL          0x0000000000004A50          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004a54*/ LDR         R3, [SP, #8] 
    /*00004a58*/ ADD         R2, R3, #3184               @ 0xc70 
    /*00004a5c*/ MOV         R3, #63                     @ 0x3f 
    /*00004a60*/ B           JpegeInitPicPar_x4a68
    JpegeInitPicPar_x4a64:
    /*00004a64*/ LDRB        R3, [R4, #-1]! 
    JpegeInitPicPar_x4a68:
    /*00004a68*/ LDRB        R1, [R2, #-1]! 
    /*00004a6c*/ ADD         R0, SP, #216                @ 0xd8 
    /*00004a70*/ ADD         R3, R0, R3 
    /*00004a74*/ CMP         R6, R4 
    /*00004a78*/ STRB        R1, [R3, #-128]             @ 0xffffff80 
    /*00004a7c*/ BNE         JpegeInitPicPar_x4a64
    /*00004a80*/ LDR         R6, [SP, #12] 
    /*00004a84*/ MOV         R2, #64                     @ 0x40 
    /*00004a88*/ ADD         R1, SP, #88                 @ 0x58 
    /*00004a8c*/ ADD         R0, R6, #70                 @ 0x46 
    /*00004a90*/ BL          0x0000000000004A90          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004a94*/ MOV         R2, R6 
    /*00004a98*/ MOV         R3, #63                     @ 0x3f 
    /*00004a9c*/ B           JpegeInitPicPar_x4aa4
    JpegeInitPicPar_x4aa0:
    /*00004aa0*/ LDRB        R3, [R5, #-1]! 
    JpegeInitPicPar_x4aa4:
    /*00004aa4*/ LDRB        R1, [R2, #-1]! 
    /*00004aa8*/ ADD         R0, SP, #216                @ 0xd8 
    /*00004aac*/ ADD         R3, R0, R3 
    /*00004ab0*/ CMP         R4, R5 
    /*00004ab4*/ STRB        R1, [R3, #-64]              @ 0xffffffc0 
    /*00004ab8*/ BNE         JpegeInitPicPar_x4aa0
    /*00004abc*/ LDR         R3, [SP, #12] 
    /*00004ac0*/ ADD         R1, SP, #152                @ 0x98 
    /*00004ac4*/ MOV         R2, #64                     @ 0x40 
    /*00004ac8*/ ADD         R0, R3, #135                @ 0x87 
    /*00004acc*/ BL          0x0000000000004ACC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004ad0*/ LDR         R4, [SP, #8] 
    /*00004ad4*/ LDR         R2, [SP, #16] 
    /*00004ad8*/ ADD         R1, SP, #152                @ 0x98 
    /*00004adc*/ LDR         IP, [R4, #3044]             @ 0xbe4 
    /*00004ae0*/ ADD         R0, R2, #5 
    /*00004ae4*/ LDR         R3, [R4, #3040]             @ 0xbe0 
    /*00004ae8*/ MOV         R2, #4 
    /*00004aec*/ STRB        IP, [SP, #153]              @ 0x99 
    /*00004af0*/ LSR         IP, IP, #8 
    /*00004af4*/ STRB        R3, [SP, #155]              @ 0x9b 
    /*00004af8*/ LSR         R3, R3, #8 
    /*00004afc*/ STRB        IP, [SP, #152]              @ 0x98 
    /*00004b00*/ STRB        R3, [SP, #154]              @ 0x9a 
    /*00004b04*/ BL          0x0000000000004B04          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004b08*/ LDR         R3, [R4, #64]               @ 0x40 
    /*00004b0c*/ ADD         R1, SP, #152                @ 0x98 
    /*00004b10*/ LDR         R2, [SP, #20] 
    /*00004b14*/ STR         R3, [R4, #3048]             @ 0xbe8 
    /*00004b18*/ ADD         R0, R2, #4 
    /*00004b1c*/ MOV         R2, #2 
    /*00004b20*/ LSR         IP, R3, #8 
    /*00004b24*/ STRB        R3, [SP, #153]              @ 0x99 
    /*00004b28*/ STRB        IP, [SP, #152]              @ 0x98 
    /*00004b2c*/ BL          0x0000000000004B2C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004b30*/ ADD         SP, SP, #220                @ 0xdc 
    /*00004b34*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JpegeInitPicPar_x4b38:
    /*00004b38*/ MOV         R1, #1 
    JpegeInitPicPar_x4b3c:
    /*00004b3c*/ MOVW        R0, #5000                   @ 0x1388 
    /*00004b40*/ BL          0x0000000000004B40          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*00004b44*/ B           JpegeInitPicPar_x4938
    JpegeInitPicPar_x4b48:
    /*00004b48*/ CMP         R1, #49                     @ 0x31 
    /*00004b4c*/ RSBGT       R1, R1, #100                @ 0x64 
    /*00004b50*/ LSLGT       R0, R1, #1 
    /*00004b54*/ BLE         JpegeInitPicPar_x4b3c
    /*00004b58*/ B           JpegeInitPicPar_x4938
    /*00004b5c*/ LDRDEQ      R0, [R0], -R4 
    /*00004b60*/ ANDEQ       R0, R0, R8, ROR #10 
    /*00004b64*/ @           <UNDEFINED> instruction: 0x000005b7 
FUNC_END JpegeInitPicPar


FUNC_BEGIN JpegeUpdatePicPar
    /*00004b68*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00004b6c*/ ADD         R3, R1, #224                @ 0xe0 
    /*00004b70*/ SUB         SP, SP, #220                @ 0xdc 
    /*00004b74*/ MOV         IP, R1 
    /*00004b78*/ MOV         R4, R0 
    /*00004b7c*/ MOVW        R2, #658                    @ 0x292 
    /*00004b80*/ STR         R1, [SP, #8] 
    /*00004b84*/ MOV         R1, R3 
    /*00004b88*/ STR         R3, [SP, #12] 
    /*00004b8c*/ LDR         R3, [IP, #12] 
    /*00004b90*/ STR         R0, [SP, #16] 
    /*00004b94*/ MOV         R0, R1 
    /*00004b98*/ LDR         R1, [PC, #668]              @ 0x0000000000004e3c 
    /*00004b9c*/ MOV         LR, R3 
    /*00004ba0*/ STR         R3, [SP, #20] 
    /*00004ba4*/ LDR         R3, [R3] 
    /*00004ba8*/ STR         R3, [IP, #16] 
    /*00004bac*/ LDR         R3, [LR, #4] 
    /*00004bb0*/ STR         R3, [IP, #20] 
    /*00004bb4*/ BL          0x0000000000004BB4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004bb8*/ LDR         R1, [R4, #52]               @ 0x34 
    /*00004bbc*/ CMP         R1, #0 
    /*00004bc0*/ BLE         JpegeUpdatePicPar_x4e18
    /*00004bc4*/ CMP         R1, #99                     @ 0x63 
    /*00004bc8*/ MOVGT       R0, #2 
    /*00004bcc*/ BLE         JpegeUpdatePicPar_x4e28
    JpegeUpdatePicPar_x4bd0:
    /*00004bd0*/ LDR         R3, [SP, #16] 
    /*00004bd4*/ MOVW        R2, #34079                  @ 0x851f 
    /*00004bd8*/ MOVT        R2, #20971                  @ 0x51eb 
    /*00004bdc*/ ADD         IP, R3, #119                @ 0x77 
    /*00004be0*/ ADD         R1, R3, #55                 @ 0x37 
    /*00004be4*/ ADD         R9, R3, #183                @ 0xb7 
    /*00004be8*/ LDR         R3, [SP, #8] 
    /*00004bec*/ ADD         R8, R3, #95                 @ 0x5f 
    /*00004bf0*/ ADD         LR, R3, #159                @ 0x9f 
    /*00004bf4*/ ADD         R6, R3, #31 
    /*00004bf8*/ STM         SP, {R6, IP} 
    JpegeUpdatePicPar_x4bfc:
    /*00004bfc*/ LDRB        FP, [R1, #1]! 
    /*00004c00*/ LDRB        SL, [IP, #1]! 
    /*00004c04*/ LDRB        R3, [R9, #1]! 
    /*00004c08*/ MUL         FP, R0, FP 
    /*00004c0c*/ MUL         SL, R0, SL 
    /*00004c10*/ ADD         FP, FP, #50                 @ 0x32 
    /*00004c14*/ MUL         R3, R0, R3 
    /*00004c18*/ ADD         SL, SL, #50                 @ 0x32 
    /*00004c1c*/ SMULL       R4, R5, FP, R2 
    /*00004c20*/ ADD         R3, R3, #50                 @ 0x32 
    /*00004c24*/ SMULL       R6, R7, SL, R2 
    /*00004c28*/ ASR         FP, FP, #31 
    /*00004c2c*/ ASR         SL, SL, #31 
    /*00004c30*/ LDR         R6, [SP] 
    /*00004c34*/ RSB         FP, FP, R5, ASR #5 
    /*00004c38*/ CMP         FP, #255                    @ 0xff 
    /*00004c3c*/ SMULL       R4, R5, R3, R2 
    /*00004c40*/ MOVGE       FP, #255                    @ 0xff 
    /*00004c44*/ CMP         FP, #1 
    /*00004c48*/ RSB         SL, SL, R7, ASR #5 
    /*00004c4c*/ ASR         R3, R3, #31 
    /*00004c50*/ LDR         R4, [SP, #4] 
    /*00004c54*/ MOVLT       FP, #1 
    /*00004c58*/ CMP         SL, #255                    @ 0xff 
    /*00004c5c*/ STRB        FP, [R6, #1]! 
    /*00004c60*/ MOVGE       SL, #255                    @ 0xff 
    /*00004c64*/ CMP         SL, #1 
    /*00004c68*/ RSB         R3, R3, R5, ASR #5 
    /*00004c6c*/ STR         R6, [SP] 
    /*00004c70*/ MOVLT       SL, #1 
    /*00004c74*/ CMP         R3, #255                    @ 0xff 
    /*00004c78*/ STRB        SL, [R8, #1]! 
    /*00004c7c*/ MOVGE       R3, #255                    @ 0xff 
    /*00004c80*/ CMP         R3, #1 
    /*00004c84*/ MOVLT       R3, #1 
    /*00004c88*/ CMP         R4, R1 
    /*00004c8c*/ STRB        R3, [LR, #1]! 
    /*00004c90*/ BNE         JpegeUpdatePicPar_x4bfc
    /*00004c94*/ LDR         R5, [PC, #420]              @ 0x0000000000004e40 
    /*00004c98*/ LDR         R3, [SP, #8] 
    /*00004c9c*/ SUB         R0, R5, #63                 @ 0x3f 
    /*00004ca0*/ ADD         R2, R3, #96                 @ 0x60 
    /*00004ca4*/ MOV         R6, R5 
    /*00004ca8*/ MOV         R3, #63                     @ 0x3f 
    /*00004cac*/ B           JpegeUpdatePicPar_x4cb4
    JpegeUpdatePicPar_x4cb0:
    /*00004cb0*/ LDRB        R3, [R6, #-1]! 
    JpegeUpdatePicPar_x4cb4:
    /*00004cb4*/ LDRB        R1, [R2, #-1]! 
    /*00004cb8*/ ADD         IP, SP, #216                @ 0xd8 
    /*00004cbc*/ ADD         R3, IP, R3 
    /*00004cc0*/ CMP         R0, R6 
    /*00004cc4*/ STRB        R1, [R3, #-192]             @ 0xffffff40 
    /*00004cc8*/ BNE         JpegeUpdatePicPar_x4cb0
    /*00004ccc*/ LDR         R7, [SP, #8] 
    /*00004cd0*/ MOV         R2, #64                     @ 0x40 
    /*00004cd4*/ ADD         R1, SP, #24 
    /*00004cd8*/ MOV         R4, R5 
    /*00004cdc*/ ADD         R0, R7, #229                @ 0xe5 
    /*00004ce0*/ BL          0x0000000000004CE0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004ce4*/ ADD         R2, R7, #160                @ 0xa0 
    /*00004ce8*/ MOV         R3, #63                     @ 0x3f 
    /*00004cec*/ B           JpegeUpdatePicPar_x4cf4
    JpegeUpdatePicPar_x4cf0:
    /*00004cf0*/ LDRB        R3, [R4, #-1]! 
    JpegeUpdatePicPar_x4cf4:
    /*00004cf4*/ LDRB        R1, [R2, #-1]! 
    /*00004cf8*/ ADD         R0, SP, #216                @ 0xd8 
    /*00004cfc*/ ADD         R3, R0, R3 
    /*00004d00*/ CMP         R6, R4 
    /*00004d04*/ STRB        R1, [R3, #-128]             @ 0xffffff80 
    /*00004d08*/ BNE         JpegeUpdatePicPar_x4cf0
    /*00004d0c*/ LDR         R6, [SP, #12] 
    /*00004d10*/ MOV         R2, #64                     @ 0x40 
    /*00004d14*/ ADD         R1, SP, #88                 @ 0x58 
    /*00004d18*/ ADD         R0, R6, #70                 @ 0x46 
    /*00004d1c*/ BL          0x0000000000004D1C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004d20*/ MOV         R2, R6 
    /*00004d24*/ MOV         R3, #63                     @ 0x3f 
    /*00004d28*/ B           JpegeUpdatePicPar_x4d30
    JpegeUpdatePicPar_x4d2c:
    /*00004d2c*/ LDRB        R3, [R5, #-1]! 
    JpegeUpdatePicPar_x4d30:
    /*00004d30*/ LDRB        R1, [R2, #-1]! 
    /*00004d34*/ ADD         R0, SP, #216                @ 0xd8 
    /*00004d38*/ ADD         R3, R0, R3 
    /*00004d3c*/ CMP         R4, R5 
    /*00004d40*/ STRB        R1, [R3, #-64]              @ 0xffffffc0 
    /*00004d44*/ BNE         JpegeUpdatePicPar_x4d2c
    /*00004d48*/ LDR         R3, [SP, #12] 
    /*00004d4c*/ MOV         R2, #64                     @ 0x40 
    /*00004d50*/ ADD         R1, SP, #152                @ 0x98 
    /*00004d54*/ ADD         R0, R3, #135                @ 0x87 
    /*00004d58*/ BL          0x0000000000004D58          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004d5c*/ LDR         R0, [SP, #8] 
    /*00004d60*/ ADD         R1, SP, #152                @ 0x98 
    /*00004d64*/ MOV         R2, #4 
    /*00004d68*/ ADD         R0, R0, #428                @ 0x1ac 
    /*00004d6c*/ LDR         IP, [R0, #-408]             @ 0xfffffe68 
    /*00004d70*/ LDR         R3, [R0, #-412]             @ 0xfffffe64 
    /*00004d74*/ STRB        IP, [SP, #153]              @ 0x99 
    /*00004d78*/ LSR         IP, IP, #8 
    /*00004d7c*/ STRB        R3, [SP, #155]              @ 0x9b 
    /*00004d80*/ LSR         R3, R3, #8 
    /*00004d84*/ STRB        IP, [SP, #152]              @ 0x98 
    /*00004d88*/ STRB        R3, [SP, #154]              @ 0x9a 
    /*00004d8c*/ BL          0x0000000000004D8C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004d90*/ LDR         R3, [SP, #20] 
    /*00004d94*/ LDR         R3, [R3, #12] 
    /*00004d98*/ CMP         R3, #25 
    /*00004d9c*/ BEQ         JpegeUpdatePicPar_x4dfc
    JpegeUpdatePicPar_x4da0:
    /*00004da0*/ LDR         R3, [SP, #8] 
    /*00004da4*/ ADD         R1, SP, #152                @ 0x98 
    /*00004da8*/ LDR         R3, [R3, #892]              @ 0x37c 
    /*00004dac*/ CMP         R3, #0 
    /*00004db0*/ LDREQ       R3, [SP, #16] 
    /*00004db4*/ MOVNE       R3, #0 
    /*00004db8*/ LDREQ       R2, [SP, #8] 
    /*00004dbc*/ MOVNE       IP, R3 
    /*00004dc0*/ LDRNE       R2, [SP, #8] 
    /*00004dc4*/ LDREQ       R3, [R3, #48]               @ 0x30 
    /*00004dc8*/ STRNE       R3, [R2, #24] 
    /*00004dcc*/ UBFXEQ      IP, R3, #8, #8 
    /*00004dd0*/ STREQ       R3, [R2, #24] 
    /*00004dd4*/ UXTBEQ      R3, R3 
    /*00004dd8*/ LDR         R2, [SP, #8] 
    /*00004ddc*/ STRB        IP, [SP, #152]              @ 0x98 
    /*00004de0*/ ADD         R0, R2, #864                @ 0x360 
    /*00004de4*/ MOV         R2, #2 
    /*00004de8*/ ADD         R0, R0, #2 
    /*00004dec*/ STRB        R3, [SP, #153]              @ 0x99 
    /*00004df0*/ BL          0x0000000000004DF0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004df4*/ ADD         SP, SP, #220                @ 0xdc 
    /*00004df8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JpegeUpdatePicPar_x4dfc:
    /*00004dfc*/ LDR         R1, [SP, #8] 
    /*00004e00*/ MOV         R2, #1 
    /*00004e04*/ LDRB        R3, [R1, #434]              @ 0x1b2 
    /*00004e08*/ BFI         R3, R2, #0, #4 
    /*00004e0c*/ MOV         R2, R1 
    /*00004e10*/ STRB        R3, [R1, #434]              @ 0x1b2 
    /*00004e14*/ B           JpegeUpdatePicPar_x4da0
    JpegeUpdatePicPar_x4e18:
    /*00004e18*/ MOV         R1, #1 
    JpegeUpdatePicPar_x4e1c:
    /*00004e1c*/ MOVW        R0, #5000                   @ 0x1388 
    /*00004e20*/ BL          0x0000000000004E20          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*00004e24*/ B           JpegeUpdatePicPar_x4bd0
    JpegeUpdatePicPar_x4e28:
    /*00004e28*/ CMP         R1, #49                     @ 0x31 
    /*00004e2c*/ RSBGT       R1, R1, #100                @ 0x64 
    /*00004e30*/ LSLGT       R0, R1, #1 
    /*00004e34*/ BLE         JpegeUpdatePicPar_x4e1c
    /*00004e38*/ B           JpegeUpdatePicPar_x4bd0
    /*00004e3c*/ LDRDEQ      R0, [R0], -R4 
    /*00004e40*/ @           <UNDEFINED> instruction: 0x000005b7 
FUNC_END JpegeUpdatePicPar


FUNC_BEGIN jpege_math_log2
    /*00004e44*/ CMP         R0, #0 
    /*00004e48*/ BXEQ        LR 
    /*00004e4c*/ B           0x00000000000046E0 
FUNC_END jpege_math_log2


FUNC_BEGIN JpegeUpdatePicParamPack
    /*00004e50*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00004e54*/ SUBS        R6, R0, #0 
    /*00004e58*/ SUB         SP, SP, #20 
    /*00004e5c*/ MOV         R8, R1 
    /*00004e60*/ MOV         R4, R2 
    /*00004e64*/ MOV         SL, R3 
    /*00004e68*/ BEQ         JpegeUpdatePicParamPack_x5938
    JpegeUpdatePicParamPack_x4e6c:
    /*00004e6c*/ CMP         R4, #0 
    /*00004e70*/ BEQ         JpegeUpdatePicParamPack_x5918
    JpegeUpdatePicParamPack_x4e74:
    /*00004e74*/ ADD         R7, R4, #64                 @ 0x40 
    /*00004e78*/ MOVW        R2, #2500                   @ 0x9c4 
    /*00004e7c*/ MOV         R1, #0 
    /*00004e80*/ MOV         R0, R7 
    /*00004e84*/ BL          0x0000000000004E84          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00004e88*/ LDR         R3, [R6, #892]              @ 0x37c 
    /*00004e8c*/ CMP         R3, #3 
    /*00004e90*/ BEQ         JpegeUpdatePicParamPack_x5194
    JpegeUpdatePicParamPack_x4e94:
    /*00004e94*/ LDR         R3, [R6, #8] 
    /*00004e98*/ CMP         R3, #0 
    /*00004e9c*/ BEQ         JpegeUpdatePicParamPack_x4eb0
    /*00004ea0*/ LDR         R3, [R6, #892]              @ 0x37c 
    /*00004ea4*/ CMP         R3, #0 
    /*00004ea8*/ MOVEQ       R9, R7 
    /*00004eac*/ BEQ         JpegeUpdatePicParamPack_x4ec4
    JpegeUpdatePicParamPack_x4eb0:
    /*00004eb0*/ MOV         R2, #2 
    /*00004eb4*/ LDR         R1, [PC, #2916]             @ 0x0000000000005a20 
    /*00004eb8*/ MOV         R0, R7 
    /*00004ebc*/ ADD         R9, R4, #66                 @ 0x42 
    /*00004ec0*/ BL          0x0000000000004EC0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    JpegeUpdatePicParamPack_x4ec4:
    /*00004ec4*/ MOV         R2, #18 
    /*00004ec8*/ LDR         R1, [PC, #2900]             @ 0x0000000000005a24 
    /*00004ecc*/ MOV         R0, R9 
    /*00004ed0*/ ADD         R5, R9, R2 
    /*00004ed4*/ BL          0x0000000000004ED4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004ed8*/ LDR         R3, [R6, #892]              @ 0x37c 
    /*00004edc*/ CMP         R3, #0 
    /*00004ee0*/ BEQ         JpegeUpdatePicParamPack_x4f98
    JpegeUpdatePicParamPack_x4ee4:
    /*00004ee4*/ LDR         R2, [R8, #160]              @ 0xa0 
    /*00004ee8*/ LDR         R3, [R8, #172]              @ 0xac 
    /*00004eec*/ LDR         IP, [R2] 
    /*00004ef0*/ LDR         R3, [R3] 
    /*00004ef4*/ CMP         IP, R3 
    /*00004ef8*/ LDRHI       R2, [R8, #8] 
    /*00004efc*/ ADDHI       R3, R3, R2 
    /*00004f00*/ SUB         R3, R3, IP 
    /*00004f04*/ CMP         R3, #0 
    /*00004f08*/ BNE         JpegeUpdatePicParamPack_x5094
    JpegeUpdatePicParamPack_x4f0c:
    /*00004f0c*/ LDR         R3, [R6, #892]              @ 0x37c 
    /*00004f10*/ CMP         R3, #3 
    /*00004f14*/ BEQ         JpegeUpdatePicParamPack_x506c
    JpegeUpdatePicParamPack_x4f18:
    /*00004f18*/ MOV         R0, R5 
    /*00004f1c*/ MOVW        R2, #658                    @ 0x292 
    /*00004f20*/ ADD         R1, R6, #224                @ 0xe0 
    /*00004f24*/ ADD         R5, R5, #656                @ 0x290 
    /*00004f28*/ BL          0x0000000000004F28          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004f2c*/ ADD         R5, R5, #2 
    JpegeUpdatePicParamPack_x4f30:
    /*00004f30*/ SUB         R5, R5, R7 
    /*00004f34*/ MOV         R0, #0 
    /*00004f38*/ ADD         R3, R5, #127                @ 0x7f 
    /*00004f3c*/ ADD         R5, R5, #64                 @ 0x40 
    /*00004f40*/ BIC         R3, R3, #63                 @ 0x3f 
    /*00004f44*/ STRB        R0, [R4, #11] 
    /*00004f48*/ STR         R3, [R4] 
    /*00004f4c*/ SUB         R5, R3, R5 
    /*00004f50*/ STR         R5, [R4, #4] 
    /*00004f54*/ MOV         R1, #1 
    /*00004f58*/ LDR         R3, [R6, #12] 
    /*00004f5c*/ MOV         R2, #8 
    /*00004f60*/ LDR         R3, [R3, #8] 
    /*00004f64*/ STRB        R1, [R4, #9] 
    /*00004f68*/ STRB        R2, [R4, #8] 
    /*00004f6c*/ STRB        R3, [R4, #10] 
    /*00004f70*/ LDR         R3, [R6, #12] 
    /*00004f74*/ LDRD        R2, [R3, #240]              @ 0xf0 
    /*00004f78*/ STRD        R2, [R4, #16] 
    /*00004f7c*/ LDR         R3, [R6, #892]              @ 0x37c 
    /*00004f80*/ SUB         R3, R3, #3 
    /*00004f84*/ CLZ         R3, R3 
    /*00004f88*/ LSR         R3, R3, #5 
    /*00004f8c*/ STR         R3, [R4, #32] 
    /*00004f90*/ ADD         SP, SP, #20 
    /*00004f94*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JpegeUpdatePicParamPack_x4f98:
    /*00004f98*/ LDR         R3, [SL] 
    /*00004f9c*/ CMP         R3, #0 
    /*00004fa0*/ BEQ         JpegeUpdatePicParamPack_x4ee4
    /*00004fa4*/ MOV         R2, #16 
    /*00004fa8*/ LDR         R1, [PC, #2680]             @ 0x0000000000005a28 
    /*00004fac*/ MOV         R0, R5 
    /*00004fb0*/ BL          0x0000000000004FB0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004fb4*/ LDR         R3, [SL] 
    /*00004fb8*/ MOV         R2, #66                     @ 0x42 
    /*00004fbc*/ LDR         R1, [PC, #2664]             @ 0x0000000000005a2c 
    /*00004fc0*/ CMP         R3, #2 
    /*00004fc4*/ ADD         R0, R9, #34                 @ 0x22 
    /*00004fc8*/ LDRBNE      R3, [R9, #21] 
    /*00004fcc*/ MOVEQ       R3, #227                    @ 0xe3 
    /*00004fd0*/ STRB        R3, [R9, #21] 
    /*00004fd4*/ BL          0x0000000000004FD4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004fd8*/ LDR         R3, [SL] 
    /*00004fdc*/ CMP         R3, #2 
    /*00004fe0*/ BEQ         JpegeUpdatePicParamPack_x5958
    JpegeUpdatePicParamPack_x4fe4:
    /*00004fe4*/ MOV         R2, #16 
    /*00004fe8*/ LDR         R1, [PC, #2624]             @ 0x0000000000005a30 
    /*00004fec*/ ADD         R0, R9, #100                @ 0x64 
    /*00004ff0*/ ADD         FP, R9, #132                @ 0x84 
    /*00004ff4*/ BL          0x0000000000004FF4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004ff8*/ LDR         R3, [SL] 
    /*00004ffc*/ MOV         R2, #16 
    /*00005000*/ LDR         R1, [PC, #2604]             @ 0x0000000000005a34 
    /*00005004*/ CMP         R3, #2 
    /*00005008*/ ADD         R0, R9, #116                @ 0x74 
    /*0000500c*/ MOVEQ       R3, #3 
    /*00005010*/ STRBEQ      R3, [R9, #115]              @ 0x73 
    /*00005014*/ BL          0x0000000000005014          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005018*/ LDR         R3, [SL] 
    /*0000501c*/ CMP         R3, #2 
    /*00005020*/ BEQ         JpegeUpdatePicParamPack_x5974
    JpegeUpdatePicParamPack_x5024:
    /*00005024*/ MOV         R2, #33                     @ 0x21 
    /*00005028*/ LDR         R1, [PC, #2568]             @ 0x0000000000005a38 
    /*0000502c*/ MOV         R0, FP 
    /*00005030*/ ADD         R5, FP, #66                 @ 0x42 
    /*00005034*/ BL          0x0000000000005034          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005038*/ MOV         R2, #33                     @ 0x21 
    /*0000503c*/ LDR         R1, [PC, #2548]             @ 0x0000000000005a38 
    /*00005040*/ ADD         R0, FP, R2 
    /*00005044*/ BL          0x0000000000005044          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005048*/ LDR         R3, [SL] 
    /*0000504c*/ CMP         R3, #2 
    /*00005050*/ BNE         JpegeUpdatePicParamPack_x4ee4
    /*00005054*/ MOV         R0, R5 
    /*00005058*/ MOV         R2, #33                     @ 0x21 
    /*0000505c*/ LDR         R1, [PC, #2516]             @ 0x0000000000005a38 
    /*00005060*/ ADD         R5, FP, #99                 @ 0x63 
    /*00005064*/ BL          0x0000000000005064          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005068*/ B           JpegeUpdatePicParamPack_x4ee4
    JpegeUpdatePicParamPack_x506c:
    /*0000506c*/ LDRB        R3, [R6, #896]              @ 0x380 
    /*00005070*/ CMP         R3, #0 
    /*00005074*/ BNE         JpegeUpdatePicParamPack_x4f18
    /*00005078*/ MOV         R0, R5 
    /*0000507c*/ MOVW        R2, #638                    @ 0x27e 
    /*00005080*/ ADD         R1, R6, #224                @ 0xe0 
    /*00005084*/ ADD         R5, R5, #636                @ 0x27c 
    /*00005088*/ BL          0x0000000000005088          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000508c*/ ADD         R5, R5, #2 
    /*00005090*/ B           JpegeUpdatePicParamPack_x4f30
    JpegeUpdatePicParamPack_x5094:
    /*00005094*/ LDR         R3, [R8, #176]              @ 0xb0 
    /*00005098*/ MOV         R2, #4 
    /*0000509c*/ ADD         R1, SP, #12 
    /*000050a0*/ MOV         R0, R5 
    /*000050a4*/ ADD         SL, R3, IP 
    /*000050a8*/ MVN         LR, #4096                   @ 0x1000 
    /*000050ac*/ LDR         R3, [R3, IP] 
    /*000050b0*/ ADD         R9, R5, R2 
    /*000050b4*/ LDR         IP, [SL, #4] 
    /*000050b8*/ STRH        LR, [SP, #12] 
    /*000050bc*/ SUB         R3, R3, IP 
    /*000050c0*/ SUB         R3, R3, #62                 @ 0x3e 
    /*000050c4*/ REV16       R3, R3 
    /*000050c8*/ STRH        R3, [SP, #14] 
    /*000050cc*/ BL          0x00000000000050CC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000050d0*/ LDR         R1, [R8, #160]              @ 0xa0 
    /*000050d4*/ LDR         R3, [R8, #172]              @ 0xac 
    /*000050d8*/ LDR         FP, [R8, #176]              @ 0xb0 
    /*000050dc*/ LDR         R2, [R1] 
    /*000050e0*/ LDR         IP, [R3] 
    /*000050e4*/ ADD         SL, FP, R2 
    /*000050e8*/ LDR         R3, [FP, R2] 
    /*000050ec*/ CMP         R2, IP 
    /*000050f0*/ ADD         R0, R3, R2 
    /*000050f4*/ BLS         JpegeUpdatePicParamPack_x5158
    /*000050f8*/ LDR         R8, [R8, #8] 
    /*000050fc*/ CMP         R0, R8 
    /*00005100*/ BCC         JpegeUpdatePicParamPack_x516c
    /*00005104*/ SUB         R0, R0, R8 
    /*00005108*/ CMP         IP, R0 
    /*0000510c*/ BCC         JpegeUpdatePicParamPack_x58c0
    JpegeUpdatePicParamPack_x5110:
    /*00005110*/ SUB         R8, R8, R2 
    /*00005114*/ MOV         R0, R9 
    /*00005118*/ SUB         R2, R8, #64                 @ 0x40 
    /*0000511c*/ SUB         R8, R3, R8 
    /*00005120*/ STR         R8, [R1] 
    /*00005124*/ ADD         R1, SL, #64                 @ 0x40 
    /*00005128*/ ADD         R5, R9, R2 
    /*0000512c*/ BL          0x000000000000512C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005130*/ CMP         R8, #0 
    /*00005134*/ BEQ         JpegeUpdatePicParamPack_x514c
    /*00005138*/ MOV         R0, R5 
    /*0000513c*/ MOV         R1, FP 
    /*00005140*/ MOV         R2, R8 
    /*00005144*/ ADD         R5, R5, R8 
    /*00005148*/ BL          0x0000000000005148          @ osal_memcpy [R_ARM_CALL: +0x0] 
    JpegeUpdatePicParamPack_x514c:
    /*0000514c*/ LDR         R3, [SL, #4] 
    /*00005150*/ SUB         R5, R5, R3 
    /*00005154*/ B           JpegeUpdatePicParamPack_x4f0c
    JpegeUpdatePicParamPack_x5158:
    /*00005158*/ CMP         IP, R0 
    /*0000515c*/ BCC         JpegeUpdatePicParamPack_x58c0
    /*00005160*/ LDR         R8, [R8, #8] 
    /*00005164*/ CMP         R0, R8 
    /*00005168*/ BCS         JpegeUpdatePicParamPack_x5110
    JpegeUpdatePicParamPack_x516c:
    /*0000516c*/ SUB         R5, R3, #64                 @ 0x40 
    /*00005170*/ STR         R0, [R1] 
    /*00005174*/ ADD         R1, SL, #64                 @ 0x40 
    /*00005178*/ MOV         R0, R9 
    /*0000517c*/ MOV         R2, R5 
    /*00005180*/ BL          0x0000000000005180          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005184*/ LDR         R3, [SL, #4] 
    /*00005188*/ SUB         R5, R5, R3 
    /*0000518c*/ ADD         R5, R9, R5 
    /*00005190*/ B           JpegeUpdatePicParamPack_x4f0c
    JpegeUpdatePicParamPack_x5194:
    /*00005194*/ MOV         R0, #1 
    /*00005198*/ BL          0x0000000000005198          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000519c*/ LDR         R3, [R0, #104]              @ 0x68 
    /*000051a0*/ MOV         R0, #1 
    /*000051a4*/ BLX         R3 
    /*000051a8*/ CMP         R0, #0 
    /*000051ac*/ BEQ         JpegeUpdatePicParamPack_x4e94
    /*000051b0*/ MOV         R0, #2 
    /*000051b4*/ BL          0x00000000000051B4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000051b8*/ LDR         R3, [R0, #112]              @ 0x70 
    /*000051bc*/ BLX         R3 
    /*000051c0*/ MOV         R8, R0 
    /*000051c4*/ MOV         R0, #1 
    /*000051c8*/ BL          0x00000000000051C8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000051cc*/ LDR         R2, [R6, #12] 
    /*000051d0*/ LDR         R3, [R0, #36]               @ 0x24 
    /*000051d4*/ LDRD        R0, [R2, #120]              @ 0x78 
    /*000051d8*/ BLX         R3 
    /*000051dc*/ CMN         R0, #1 
    /*000051e0*/ MOV         R5, R0 
    /*000051e4*/ BEQ         JpegeUpdatePicParamPack_x5a00
    JpegeUpdatePicParamPack_x51e8:
    /*000051e8*/ MOV         R0, #1 
    /*000051ec*/ BL          0x00000000000051EC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000051f0*/ LDR         R3, [R0, #100]              @ 0x64 
    /*000051f4*/ MOV         R0, R5 
    /*000051f8*/ BLX         R3 
    /*000051fc*/ LDR         R3, [R6, #12] 
    /*00005200*/ LDR         R3, [R3, #304]              @ 0x130 
    /*00005204*/ LDR         R9, [R0, #40]               @ 0x28 
    /*00005208*/ CMP         R9, R3 
    /*0000520c*/ BEQ         JpegeUpdatePicParamPack_x5230
    /*00005210*/ MOVW        R3, #394                    @ 0x18a 
    /*00005214*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10b8] 
    /*00005218*/ STR         R3, [SP] 
    /*0000521c*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10b8] 
    /*00005220*/ LDR         R3, [PC, #2068]             @ 0x0000000000005a3c 
    /*00005224*/ MOV         R1, #11 
    /*00005228*/ MOV         R0, #4 
    /*0000522c*/ BL          0x000000000000522C          @ HI_LOG [R_ARM_CALL: +0x0] 
    JpegeUpdatePicParamPack_x5230:
    /*00005230*/ MOV         R2, #2 
    /*00005234*/ LDR         R1, [PC, #2020]             @ 0x0000000000005a20 
    /*00005238*/ MOV         R0, R7 
    /*0000523c*/ MOV         R5, #0 
    /*00005240*/ BL          0x0000000000005240          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005244*/ MOV         R2, #18 
    /*00005248*/ LDR         R1, [PC, #2032]             @ 0x0000000000005a40 
    /*0000524c*/ ADD         R0, R4, #66                 @ 0x42 
    /*00005250*/ BL          0x0000000000005250          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005254*/ MOV         R3, #11 
    /*00005258*/ MOV         R2, #132                    @ 0x84 
    /*0000525c*/ STRB        R3, [R4, #85]               @ 0x55 
    /*00005260*/ LDR         R1, [PC, #2012]             @ 0x0000000000005a44 
    /*00005264*/ ADD         R0, R4, #86                 @ 0x56 
    /*00005268*/ STRB        R5, [R4, #84]               @ 0x54 
    /*0000526c*/ MOV         SL, #2 
    /*00005270*/ BL          0x0000000000005270          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005274*/ MOV         R3, #34                     @ 0x22 
    /*00005278*/ MVN         R1, #87                     @ 0x57 
    /*0000527c*/ MOV         R2, #408                    @ 0x198 
    /*00005280*/ STRB        R1, [R4, #221]              @ 0xdd 
    /*00005284*/ ADD         R0, R4, #224                @ 0xe0 
    /*00005288*/ STRB        R3, [R4, #223]              @ 0xdf 
    /*0000528c*/ LDR         R1, [PC, #1972]             @ 0x0000000000005a48 
    /*00005290*/ STRB        R5, [R4, #218]              @ 0xda 
    /*00005294*/ STRB        R5, [R4, #219]              @ 0xdb 
    /*00005298*/ STRB        R5, [R4, #222]              @ 0xde 
    /*0000529c*/ STRB        SL, [R4, #220]              @ 0xdc 
    /*000052a0*/ BL          0x00000000000052A0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000052a4*/ ADD         R0, R4, #636                @ 0x27c 
    /*000052a8*/ MOV         R3, #7 
    /*000052ac*/ MOV         R2, #84                     @ 0x54 
    /*000052b0*/ STRB        R3, [R4, #637]              @ 0x27d 
    /*000052b4*/ ADD         R0, R0, SL 
    /*000052b8*/ LDR         R1, [PC, #1932]             @ 0x0000000000005a4c 
    /*000052bc*/ STRB        R5, [R4, #636]              @ 0x27c 
    /*000052c0*/ BL          0x00000000000052C0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000052c4*/ MOV         R2, #24 
    /*000052c8*/ LDR         R1, [PC, #1920]             @ 0x0000000000005a50 
    /*000052cc*/ ADD         R0, R4, #728                @ 0x2d8 
    /*000052d0*/ STRB        R5, [R4, #726]              @ 0x2d6 
    /*000052d4*/ STRB        SL, [R4, #727]              @ 0x2d7 
    /*000052d8*/ BL          0x00000000000052D8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000052dc*/ ADD         R0, R4, #756                @ 0x2f4 
    /*000052e0*/ MOV         R3, #5 
    /*000052e4*/ MOV         R2, #60                     @ 0x3c 
    /*000052e8*/ STRB        R3, [R4, #757]              @ 0x2f5 
    /*000052ec*/ ADD         R0, R0, SL 
    /*000052f0*/ LDR         R1, [PC, #1884]             @ 0x0000000000005a54 
    /*000052f4*/ STRB        R5, [R4, #756]              @ 0x2f4 
    /*000052f8*/ BL          0x00000000000052F8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000052fc*/ ADD         R0, R4, #820                @ 0x334 
    /*00005300*/ MOV         R3, #120                    @ 0x78 
    /*00005304*/ MVN         R2, #95                     @ 0x5f 
    /*00005308*/ STRB        R3, [R4, #779]              @ 0x30b 
    /*0000530c*/ ADD         R1, R9, #28 
    /*00005310*/ STRB        R2, [R4, #767]              @ 0x2ff 
    /*00005314*/ ADD         R0, R0, SL 
    /*00005318*/ MOV         R2, #32 
    /*0000531c*/ STRB        R5, [R4, #766]              @ 0x2fe 
    /*00005320*/ STRB        R5, [R4, #778]              @ 0x30a 
    /*00005324*/ BL          0x0000000000005324          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005328*/ ADD         R0, R4, #852                @ 0x354 
    /*0000532c*/ MOV         R2, #32 
    /*00005330*/ ADD         R1, R9, #60                 @ 0x3c 
    /*00005334*/ ADD         R0, R0, SL 
    /*00005338*/ BL          0x0000000000005338          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000533c*/ ADD         R0, R4, #884                @ 0x374 
    /*00005340*/ MOV         R2, #32 
    /*00005344*/ ADD         R1, R9, #92                 @ 0x5c 
    /*00005348*/ ADD         R0, R0, SL 
    /*0000534c*/ BL          0x000000000000534C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005350*/ ADD         R0, R4, #932                @ 0x3a4 
    /*00005354*/ MOV         R2, #32 
    /*00005358*/ ADD         R1, R9, #124                @ 0x7c 
    /*0000535c*/ ADD         R0, R0, SL 
    /*00005360*/ BL          0x0000000000005360          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005364*/ MOV         R2, #72                     @ 0x48 
    /*00005368*/ MOV         R3, #1 
    /*0000536c*/ STRB        R2, [R4, #921]              @ 0x399 
    /*00005370*/ STRB        R2, [R4, #929]              @ 0x3a1 
    /*00005374*/ STRB        R3, [R4, #925]              @ 0x39d 
    /*00005378*/ STRB        R3, [R4, #933]              @ 0x3a5 
    /*0000537c*/ LDR         R3, [R9, #180]              @ 0xb4 
    /*00005380*/ STRB        R3, [R4, #993]              @ 0x3e1 
    /*00005384*/ LSR         R1, R3, #24 
    /*00005388*/ LSR         R2, R3, #16 
    /*0000538c*/ STRB        R1, [R4, #988]              @ 0x3dc 
    /*00005390*/ STRB        R2, [R4, #989]              @ 0x3dd 
    /*00005394*/ LSR         R3, R3, #8 
    /*00005398*/ STRB        R3, [R4, #992]              @ 0x3e0 
    /*0000539c*/ LDR         R3, [R9, #192]              @ 0xc0 
    /*000053a0*/ STRB        R3, [R4, #1001]             @ 0x3e9 
    /*000053a4*/ LSR         R1, R3, #24 
    /*000053a8*/ LSR         R2, R3, #16 
    /*000053ac*/ STRB        R1, [R4, #996]              @ 0x3e4 
    /*000053b0*/ STRB        R2, [R4, #997]              @ 0x3e5 
    /*000053b4*/ LSR         R3, R3, #8 
    /*000053b8*/ STRB        R3, [R4, #1000]             @ 0x3e8 
    /*000053bc*/ LDR         R2, [R9, #184]              @ 0xb8 
    /*000053c0*/ LSR         R1, R2, #16 
    /*000053c4*/ SXTH        R3, R2 
    /*000053c8*/ STRB        R2, [R4, #1073]             @ 0x431 
    /*000053cc*/ SXTH        R2, R1 
    /*000053d0*/ STRB        R1, [R4, #1069]             @ 0x42d 
    /*000053d4*/ LSR         R1, R3, #24 
    /*000053d8*/ STRB        R1, [R4, #1070]             @ 0x42e 
    /*000053dc*/ LSR         R0, R2, #24 
    /*000053e0*/ LSR         R1, R2, #16 
    /*000053e4*/ STRB        R0, [R4, #1066]             @ 0x42a 
    /*000053e8*/ LSR         R2, R2, #8 
    /*000053ec*/ STRB        R1, [R4, #1067]             @ 0x42b 
    /*000053f0*/ LSR         R1, R3, #16 
    /*000053f4*/ STRB        R2, [R4, #1068]             @ 0x42c 
    /*000053f8*/ LSR         R3, R3, #8 
    /*000053fc*/ STRB        R1, [R4, #1071]             @ 0x42f 
    /*00005400*/ STRB        R3, [R4, #1072]             @ 0x430 
    /*00005404*/ LDR         R3, [R9, #196]              @ 0xc4 
    /*00005408*/ LSR         R1, R3, #24 
    /*0000540c*/ STRB        R3, [R4, #1081]             @ 0x439 
    /*00005410*/ LSR         R2, R3, #16 
    /*00005414*/ STRB        R1, [R4, #1076]             @ 0x434 
    /*00005418*/ STRB        R2, [R4, #1077]             @ 0x435 
    /*0000541c*/ LSR         R3, R3, #8 
    /*00005420*/ STRB        R3, [R4, #1080]             @ 0x438 
    /*00005424*/ LDR         R0, [R9, #192]              @ 0xc0 
    /*00005428*/ UXTH        R1, R0 
    /*0000542c*/ CMP         R1, R5 
    /*00005430*/ BEQ         JpegeUpdatePicParamPack_x5490
    /*00005434*/ LSR         R0, R0, #16 
    /*00005438*/ LSL         R0, R0, #10 
    /*0000543c*/ BL          0x000000000000543C          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00005440*/ CMP         R0, R5 
    /*00005444*/ BNE         JpegeUpdatePicParamPack_x598c
    JpegeUpdatePicParamPack_x5448:
    /*00005448*/ MOVW        R3, #587                    @ 0x24b 
    /*0000544c*/ MOV         R1, #11 
    /*00005450*/ STR         R3, [SP] 
    /*00005454*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10f8] 
    /*00005458*/ LDR         R3, [PC, #1500]             @ 0x0000000000005a3c 
    /*0000545c*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10f8] 
    /*00005460*/ MOV         R0, #4 
    /*00005464*/ BL          0x0000000000005464          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005468*/ MOV         R3, #0 
    /*0000546c*/ MOV         R1, R3 
    /*00005470*/ MOV         R0, R3 
    /*00005474*/ MOV         R2, R3 
    JpegeUpdatePicParamPack_x5478:
    /*00005478*/ STRB        R2, [R4, #1058]             @ 0x422 
    /*0000547c*/ MOV         R2, #1 
    /*00005480*/ STRB        R0, [R4, #1059]             @ 0x423 
    /*00005484*/ STRB        R1, [R4, #1060]             @ 0x424 
    /*00005488*/ STRB        R3, [R4, #1061]             @ 0x425 
    /*0000548c*/ STRB        R2, [R4, #1064]             @ 0x428 
    JpegeUpdatePicParamPack_x5490:
    /*00005490*/ LDR         R3, [R9, #180]              @ 0xb4 
    /*00005494*/ LSRS        R1, R3, #16 
    /*00005498*/ BEQ         JpegeUpdatePicParamPack_x54fc
    /*0000549c*/ MOV         R0, #64512                  @ 0xfc00 
    /*000054a0*/ MOVT        R0, #1023                   @ 0x3ff 
    /*000054a4*/ AND         R0, R0, R3, LSL #10 
    /*000054a8*/ BL          0x00000000000054A8          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000054ac*/ CMP         R0, #0 
    /*000054b0*/ BNE         JpegeUpdatePicParamPack_x59c8
    JpegeUpdatePicParamPack_x54b4:
    /*000054b4*/ MOVW        R3, #614                    @ 0x266 
    /*000054b8*/ MOV         R1, #11 
    /*000054bc*/ STR         R3, [SP] 
    /*000054c0*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10f8] 
    /*000054c4*/ LDR         R3, [PC, #1392]             @ 0x0000000000005a3c 
    /*000054c8*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10f8] 
    /*000054cc*/ MOV         R0, #4 
    /*000054d0*/ BL          0x00000000000054D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000054d4*/ MOV         R3, #0 
    /*000054d8*/ MOV         R1, R3 
    /*000054dc*/ MOV         R0, R3 
    /*000054e0*/ MOV         R2, R3 
    JpegeUpdatePicParamPack_x54e4:
    /*000054e4*/ STRB        R2, [R4, #1050]             @ 0x41a 
    /*000054e8*/ MOV         R2, #1 
    /*000054ec*/ STRB        R0, [R4, #1051]             @ 0x41b 
    /*000054f0*/ STRB        R1, [R4, #1052]             @ 0x41c 
    /*000054f4*/ STRB        R3, [R4, #1053]             @ 0x41d 
    /*000054f8*/ STRB        R2, [R4, #1056]             @ 0x420 
    JpegeUpdatePicParamPack_x54fc:
    /*000054fc*/ LDR         R3, [R9, #176]              @ 0xb0 
    /*00005500*/ ADD         R0, R4, #964                @ 0x3c4 
    /*00005504*/ MOV         R2, #20 
    /*00005508*/ ADD         R0, R0, #2 
    /*0000550c*/ UBFX        R1, R3, #8, #8 
    /*00005510*/ STRB        R1, [R4, #268]              @ 0x10c 
    /*00005514*/ STRB        R3, [R4, #269]              @ 0x10d 
    /*00005518*/ MOV         R1, R9 
    /*0000551c*/ LDR         R3, [R9, #160]              @ 0xa0 
    /*00005520*/ ADD         R5, R4, #1600               @ 0x640 
    /*00005524*/ ADD         R5, R5, #2 
    /*00005528*/ STRB        R3, [R4, #1089]             @ 0x441 
    /*0000552c*/ LSR         IP, R3, #24 
    /*00005530*/ STRB        IP, [R4, #1084]             @ 0x43c 
    /*00005534*/ LSR         IP, R3, #16 
    /*00005538*/ LSR         R3, R3, #8 
    /*0000553c*/ STRB        IP, [R4, #1085]             @ 0x43d 
    /*00005540*/ STRB        R3, [R4, #1088]             @ 0x440 
    /*00005544*/ BL          0x0000000000005544          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005548*/ ADD         R0, R4, #1000               @ 0x3e8 
    /*0000554c*/ MOV         R1, R9 
    /*00005550*/ MOV         R2, #20 
    /*00005554*/ ADD         R0, R0, #2 
    /*00005558*/ BL          0x0000000000005558          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000555c*/ ADD         R0, R4, #1020               @ 0x3fc 
    /*00005560*/ MOV         R1, R9 
    /*00005564*/ MOV         R2, #20 
    /*00005568*/ ADD         R0, R0, #2 
    /*0000556c*/ BL          0x000000000000556C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005570*/ LDRB        R1, [R6, #3] 
    /*00005574*/ MOV         R3, #0 
    /*00005578*/ MOV         R2, #2 
    /*0000557c*/ ADD         R0, R4, #1600               @ 0x640 
    /*00005580*/ STRB        R1, [R4, #448]              @ 0x1c0 
    /*00005584*/ LDRH        R1, [R6, #2] 
    /*00005588*/ STRB        R1, [R4, #449]              @ 0x1c1 
    /*0000558c*/ LDR         R1, [R6] 
    /*00005590*/ LSR         R1, R1, #8 
    /*00005594*/ STRB        R1, [R4, #450]              @ 0x1c2 
    /*00005598*/ LDR         R1, [R6] 
    /*0000559c*/ STRB        R1, [R4, #451]              @ 0x1c3 
    /*000055a0*/ LDRB        R1, [R6, #7] 
    /*000055a4*/ STRB        R1, [R4, #460]              @ 0x1cc 
    /*000055a8*/ LDRH        R1, [R6, #6] 
    /*000055ac*/ STRB        R1, [R4, #461]              @ 0x1cd 
    /*000055b0*/ LDR         R1, [R6, #4] 
    /*000055b4*/ LSR         R1, R1, #8 
    /*000055b8*/ STRB        R1, [R4, #462]              @ 0x1ce 
    /*000055bc*/ LDR         R1, [R6, #4] 
    /*000055c0*/ STRB        R1, [R4, #463]              @ 0x1cf 
    /*000055c4*/ LDRB        R1, [R9, #172]              @ 0xac 
    /*000055c8*/ STRB        R3, [R4, #388]              @ 0x184 
    /*000055cc*/ STRB        R1, [R4, #389]              @ 0x185 
    /*000055d0*/ LDRB        R1, [R9, #20] 
    /*000055d4*/ STRB        R3, [R4, #412]              @ 0x19c 
    /*000055d8*/ STRB        R1, [R4, #413]              @ 0x19d 
    /*000055dc*/ LDRB        R1, [R9, #156]              @ 0x9c 
    /*000055e0*/ STRB        R3, [R4, #400]              @ 0x190 
    /*000055e4*/ STRB        R1, [R4, #401]              @ 0x191 
    /*000055e8*/ LDRB        R1, [R9, #188]              @ 0xbc 
    /*000055ec*/ STRB        R3, [R4, #256]              @ 0x100 
    /*000055f0*/ STRB        R1, [R4, #257]              @ 0x101 
    /*000055f4*/ LDRB        R1, [R9, #165]              @ 0xa5 
    /*000055f8*/ STRB        R3, [R4, #520]              @ 0x208 
    /*000055fc*/ STRB        R1, [R4, #521]              @ 0x209 
    /*00005600*/ LDRB        R1, [R9, #200]              @ 0xc8 
    /*00005604*/ STRB        R3, [R4, #532]              @ 0x214 
    /*00005608*/ STRB        R1, [R4, #533]              @ 0x215 
    /*0000560c*/ LDRB        R1, [R9, #201]              @ 0xc9 
    /*00005610*/ STRB        R3, [R4, #544]              @ 0x220 
    /*00005614*/ STRB        R1, [R4, #545]              @ 0x221 
    /*00005618*/ LDR         R1, [R9, #24] 
    /*0000561c*/ STRB        R1, [R4, #1097]             @ 0x449 
    /*00005620*/ LSR         IP, R1, #24 
    /*00005624*/ STRB        IP, [R4, #1092]             @ 0x444 
    /*00005628*/ LSR         IP, R1, #16 
    /*0000562c*/ LSR         R1, R1, #8 
    /*00005630*/ STRB        IP, [R4, #1093]             @ 0x445 
    /*00005634*/ STRB        R1, [R4, #1096]             @ 0x448 
    /*00005638*/ LDRB        R1, [R8, #7] 
    /*0000563c*/ STRB        R1, [R4, #1098]             @ 0x44a 
    /*00005640*/ LDRH        R1, [R8, #6] 
    /*00005644*/ STRB        R1, [R4, #1099]             @ 0x44b 
    /*00005648*/ LDR         R1, [R8, #4] 
    /*0000564c*/ LSR         R1, R1, #8 
    /*00005650*/ STRB        R1, [R4, #1100]             @ 0x44c 
    /*00005654*/ LDR         R1, [R8, #4] 
    /*00005658*/ STRB        R1, [R4, #1101]             @ 0x44d 
    /*0000565c*/ LDRB        R1, [R8, #11] 
    /*00005660*/ STRB        R1, [R4, #1102]             @ 0x44e 
    /*00005664*/ LDRH        R1, [R8, #10] 
    /*00005668*/ STRB        R1, [R4, #1103]             @ 0x44f 
    /*0000566c*/ LDR         R1, [R8, #8] 
    /*00005670*/ LSR         R1, R1, #8 
    /*00005674*/ STRB        R1, [R4, #1104]             @ 0x450 
    /*00005678*/ LDR         R1, [R8, #8] 
    /*0000567c*/ STRB        R1, [R4, #1105]             @ 0x451 
    /*00005680*/ LDRB        R1, [R8, #15] 
    /*00005684*/ STRB        R1, [R4, #1106]             @ 0x452 
    /*00005688*/ LDRH        R1, [R8, #14] 
    /*0000568c*/ STRB        R1, [R4, #1107]             @ 0x453 
    /*00005690*/ LDR         R1, [R8, #12] 
    /*00005694*/ LSR         R1, R1, #8 
    /*00005698*/ STRB        R1, [R4, #1108]             @ 0x454 
    /*0000569c*/ LDR         R1, [R8, #12] 
    /*000056a0*/ STRB        R1, [R4, #1109]             @ 0x455 
    /*000056a4*/ LDRB        R1, [R8, #19] 
    /*000056a8*/ STRB        R1, [R4, #1110]             @ 0x456 
    /*000056ac*/ LDRH        R1, [R8, #18] 
    /*000056b0*/ STRB        R1, [R4, #1111]             @ 0x457 
    /*000056b4*/ LDR         R1, [R8, #16] 
    /*000056b8*/ LSR         R1, R1, #8 
    /*000056bc*/ STRB        R1, [R4, #1112]             @ 0x458 
    /*000056c0*/ LDR         R1, [R8, #16] 
    /*000056c4*/ STRB        R1, [R4, #1113]             @ 0x459 
    /*000056c8*/ LDRB        R1, [R8, #23] 
    /*000056cc*/ STRB        R1, [R4, #1114]             @ 0x45a 
    /*000056d0*/ LDRH        R1, [R8, #22] 
    /*000056d4*/ STRB        R1, [R4, #1115]             @ 0x45b 
    /*000056d8*/ LDR         R1, [R8, #20] 
    /*000056dc*/ LSR         R1, R1, #8 
    /*000056e0*/ STRB        R1, [R4, #1116]             @ 0x45c 
    /*000056e4*/ LDR         R1, [R8, #20] 
    /*000056e8*/ STRB        R1, [R4, #1117]             @ 0x45d 
    /*000056ec*/ LDRB        R1, [R8, #27] 
    /*000056f0*/ STRB        R1, [R4, #1118]             @ 0x45e 
    /*000056f4*/ LDRH        R1, [R8, #26] 
    /*000056f8*/ STRB        R1, [R4, #1119]             @ 0x45f 
    /*000056fc*/ LDR         R1, [R8, #24] 
    /*00005700*/ LSR         R1, R1, #8 
    /*00005704*/ STRB        R1, [R4, #1120]             @ 0x460 
    /*00005708*/ LDR         R1, [R8, #24] 
    /*0000570c*/ STRB        R1, [R4, #1121]             @ 0x461 
    /*00005710*/ LDRB        R1, [R8, #35]               @ 0x23 
    /*00005714*/ STRB        R1, [R4, #1122]             @ 0x462 
    /*00005718*/ LDRH        R1, [R8, #34]               @ 0x22 
    /*0000571c*/ STRB        R1, [R4, #1123]             @ 0x463 
    /*00005720*/ LDR         R1, [R8, #32] 
    /*00005724*/ LSR         R1, R1, #8 
    /*00005728*/ STRB        R1, [R4, #1124]             @ 0x464 
    /*0000572c*/ LDR         R1, [R8, #32] 
    /*00005730*/ STRB        R1, [R4, #1125]             @ 0x465 
    /*00005734*/ LDRB        R1, [R8, #39]               @ 0x27 
    /*00005738*/ STRB        R1, [R4, #1126]             @ 0x466 
    /*0000573c*/ LDRH        R1, [R8, #38]               @ 0x26 
    /*00005740*/ STRB        R1, [R4, #1127]             @ 0x467 
    /*00005744*/ LDR         R1, [R8, #36]               @ 0x24 
    /*00005748*/ LSR         R1, R1, #8 
    /*0000574c*/ STRB        R1, [R4, #1128]             @ 0x468 
    /*00005750*/ LDR         R1, [R8, #36]               @ 0x24 
    /*00005754*/ STRB        R1, [R4, #1129]             @ 0x469 
    /*00005758*/ LDRB        R1, [R8, #43]               @ 0x2b 
    /*0000575c*/ STRB        R1, [R4, #1130]             @ 0x46a 
    /*00005760*/ LDRH        R1, [R8, #42]               @ 0x2a 
    /*00005764*/ STRB        R1, [R4, #1131]             @ 0x46b 
    /*00005768*/ LDR         R1, [R8, #40]               @ 0x28 
    /*0000576c*/ LSR         R1, R1, #8 
    /*00005770*/ STRB        R1, [R4, #1132]             @ 0x46c 
    /*00005774*/ LDR         R1, [R8, #40]               @ 0x28 
    /*00005778*/ STRB        R1, [R4, #1133]             @ 0x46d 
    /*0000577c*/ LDRB        R1, [R8, #47]               @ 0x2f 
    /*00005780*/ STRB        R1, [R4, #1134]             @ 0x46e 
    /*00005784*/ LDRH        R1, [R8, #46]               @ 0x2e 
    /*00005788*/ STRB        R1, [R4, #1135]             @ 0x46f 
    /*0000578c*/ LDR         R1, [R8, #44]               @ 0x2c 
    /*00005790*/ LSR         R1, R1, #8 
    /*00005794*/ STRB        R1, [R4, #1136]             @ 0x470 
    /*00005798*/ LDR         R1, [R8, #44]               @ 0x2c 
    /*0000579c*/ STRB        R1, [R4, #1137]             @ 0x471 
    /*000057a0*/ LDRB        R1, [R8, #51]               @ 0x33 
    /*000057a4*/ STRB        R1, [R4, #1138]             @ 0x472 
    /*000057a8*/ LDRH        R1, [R8, #50]               @ 0x32 
    /*000057ac*/ STRB        R1, [R4, #1139]             @ 0x473 
    /*000057b0*/ LDR         R1, [R8, #48]               @ 0x30 
    /*000057b4*/ LSR         R1, R1, #8 
    /*000057b8*/ STRB        R1, [R4, #1140]             @ 0x474 
    /*000057bc*/ LDR         R1, [R8, #48]               @ 0x30 
    /*000057c0*/ STRB        R1, [R4, #1141]             @ 0x475 
    /*000057c4*/ LDRB        R1, [R8, #55]               @ 0x37 
    /*000057c8*/ STRB        R1, [R4, #1142]             @ 0x476 
    /*000057cc*/ LDRH        R1, [R8, #54]               @ 0x36 
    /*000057d0*/ STRB        R1, [R4, #1143]             @ 0x477 
    /*000057d4*/ LDR         R1, [R8, #52]               @ 0x34 
    /*000057d8*/ LSR         R1, R1, #8 
    /*000057dc*/ STRB        R1, [R4, #1144]             @ 0x478 
    /*000057e0*/ LDR         R1, [R8, #52]               @ 0x34 
    /*000057e4*/ STRB        R1, [R4, #1145]             @ 0x479 
    /*000057e8*/ LDRB        R1, [R8, #63]               @ 0x3f 
    /*000057ec*/ STRB        R1, [R4, #1146]             @ 0x47a 
    /*000057f0*/ LDRH        R1, [R8, #62]               @ 0x3e 
    /*000057f4*/ STRB        R1, [R4, #1147]             @ 0x47b 
    /*000057f8*/ LDR         R1, [R8, #60]               @ 0x3c 
    /*000057fc*/ LSR         R1, R1, #8 
    /*00005800*/ STRB        R1, [R4, #1148]             @ 0x47c 
    /*00005804*/ LDR         R1, [R8, #60]               @ 0x3c 
    /*00005808*/ STRB        R1, [R4, #1149]             @ 0x47d 
    /*0000580c*/ LDRB        R1, [R8, #67]               @ 0x43 
    /*00005810*/ STRB        R1, [R4, #1150]             @ 0x47e 
    /*00005814*/ LDRH        R1, [R8, #66]               @ 0x42 
    /*00005818*/ STRB        R1, [R4, #1151]             @ 0x47f 
    /*0000581c*/ LDR         R1, [R8, #64]               @ 0x40 
    /*00005820*/ LSR         R1, R1, #8 
    /*00005824*/ STRB        R1, [R4, #1152]             @ 0x480 
    /*00005828*/ LDR         R1, [R8, #64]               @ 0x40 
    /*0000582c*/ STRB        R1, [R4, #1153]             @ 0x481 
    /*00005830*/ LDRB        R1, [R8] 
    /*00005834*/ STRB        R3, [R4, #659]              @ 0x293 
    /*00005838*/ STRB        R3, [R4, #660]              @ 0x294 
    /*0000583c*/ STRB        R1, [R4, #658]              @ 0x292 
    /*00005840*/ STRB        R3, [R4, #661]              @ 0x295 
    /*00005844*/ LDRB        R1, [R8, #28] 
    /*00005848*/ STRB        R3, [R4, #683]              @ 0x2ab 
    /*0000584c*/ STRB        R1, [R4, #682]              @ 0x2aa 
    /*00005850*/ STRB        R3, [R4, #684]              @ 0x2ac 
    /*00005854*/ STRB        R3, [R4, #685]              @ 0x2ad 
    /*00005858*/ LDRB        R1, [R8, #56]               @ 0x38 
    /*0000585c*/ STRB        R3, [R4, #707]              @ 0x2c3 
    /*00005860*/ STRB        R3, [R4, #708]              @ 0x2c4 
    /*00005864*/ STRB        R1, [R4, #706]              @ 0x2c2 
    /*00005868*/ STRB        R3, [R4, #709]              @ 0x2c5 
    /*0000586c*/ LDRB        R1, [R9, #166]              @ 0xa6 
    /*00005870*/ STRB        R3, [R4, #568]              @ 0x238 
    /*00005874*/ STRB        R1, [R4, #569]              @ 0x239 
    /*00005878*/ LDRB        R1, [R9, #167]              @ 0xa7 
    /*0000587c*/ STRB        R3, [R4, #580]              @ 0x244 
    /*00005880*/ STRB        R1, [R4, #581]              @ 0x245 
    /*00005884*/ LDRB        R1, [R9, #168]              @ 0xa8 
    /*00005888*/ STRB        R3, [R4, #592]              @ 0x250 
    /*0000588c*/ STRB        R1, [R4, #593]              @ 0x251 
    /*00005890*/ LDRB        R1, [R9, #169]              @ 0xa9 
    /*00005894*/ STRB        R3, [R4, #604]              @ 0x25c 
    /*00005898*/ STRB        R1, [R4, #605]              @ 0x25d 
    /*0000589c*/ LDRB        R1, [R9, #170]              @ 0xaa 
    /*000058a0*/ STRB        R3, [R4, #616]              @ 0x268 
    /*000058a4*/ STRB        R1, [R4, #617]              @ 0x269 
    /*000058a8*/ LDRB        IP, [R9, #171]              @ 0xab 
    /*000058ac*/ LDR         R1, [PC, #364]              @ 0x0000000000005a20 
    /*000058b0*/ STRB        R3, [R4, #628]              @ 0x274 
    /*000058b4*/ STRB        IP, [R4, #629]              @ 0x275 
    /*000058b8*/ BL          0x00000000000058B8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000058bc*/ B           JpegeUpdatePicParamPack_x4f0c
    JpegeUpdatePicParamPack_x58c0:
    /*000058c0*/ MOVW        R2, #1052                   @ 0x41c 
    /*000058c4*/ LDR         R1, [PC, #368]              @ 0x0000000000005a3c 
    /*000058c8*/ MOVW        R3, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x113c] 
    /*000058cc*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*000058d0*/ MOVT        R3, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x113c] 
    /*000058d4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*000058d8*/ BL          0x00000000000058D8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000058dc*/ MOV         R0, R9 
    /*000058e0*/ MVN         R2, #63                     @ 0x3f 
    /*000058e4*/ MOV         R1, #64                     @ 0x40 
    /*000058e8*/ BL          0x00000000000058E8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000058ec*/ LDR         IP, [SL, #4] 
    /*000058f0*/ SUB         R5, R5, #60                 @ 0x3c 
    /*000058f4*/ MOVW        R3, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x113c] 
    /*000058f8*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*000058fc*/ MOVT        R3, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x113c] 
    /*00005900*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00005904*/ MOVW        R2, #1067                   @ 0x42b 
    /*00005908*/ LDR         R1, [PC, #300]              @ 0x0000000000005a3c 
    /*0000590c*/ SUB         R5, R5, IP 
    /*00005910*/ BL          0x0000000000005910          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005914*/ B           JpegeUpdatePicParamPack_x4f0c
    JpegeUpdatePicParamPack_x5918:
    /*00005918*/ MOVW        R3, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1084] 
    /*0000591c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00005920*/ MOVT        R3, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1084] 
    /*00005924*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00005928*/ MOVW        R2, #369                    @ 0x171 
    /*0000592c*/ LDR         R1, [PC, #264]              @ 0x0000000000005a3c 
    /*00005930*/ BL          0x0000000000005930          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005934*/ B           JpegeUpdatePicParamPack_x4e74
    JpegeUpdatePicParamPack_x5938:
    /*00005938*/ MOVW        R3, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1070] 
    /*0000593c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00005940*/ MOVT        R3, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1070] 
    /*00005944*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00005948*/ MOV         R2, #368                    @ 0x170 
    /*0000594c*/ LDR         R1, [PC, #232]              @ 0x0000000000005a3c 
    /*00005950*/ BL          0x0000000000005950          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005954*/ B           JpegeUpdatePicParamPack_x4e6c
    JpegeUpdatePicParamPack_x5958:
    /*00005958*/ MOV         R3, #3 
    /*0000595c*/ STRB        R3, [R9, #59]               @ 0x3b 
    /*00005960*/ MOV         R3, #48                     @ 0x30 
    /*00005964*/ STRB        R3, [R9, #67]               @ 0x43 
    /*00005968*/ MOV         R3, #122                    @ 0x7a 
    /*0000596c*/ STRB        R3, [R9, #83]               @ 0x53 
    /*00005970*/ B           JpegeUpdatePicParamPack_x4fe4
    JpegeUpdatePicParamPack_x5974:
    /*00005974*/ MOV         R0, FP 
    /*00005978*/ MOV         R2, #16 
    /*0000597c*/ LDR         R1, [PC, #176]              @ 0x0000000000005a34 
    /*00005980*/ ADD         FP, R9, #148                @ 0x94 
    /*00005984*/ BL          0x0000000000005984          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005988*/ B           JpegeUpdatePicParamPack_x5024
    JpegeUpdatePicParamPack_x598c:
    /*0000598c*/ MOV         R2, R5 
    /*00005990*/ MOV         R1, #8 
    /*00005994*/ BL          0x00000000000046E0 
    /*00005998*/ LSR         R3, R0, #8 
    /*0000599c*/ CMP         R3, #9 
    /*000059a0*/ BLS         JpegeUpdatePicParamPack_x5448
    /*000059a4*/ SUB         R2, R3, #10 
    /*000059a8*/ UXTB        R3, R0 
    /*000059ac*/ ADD         R3, R3, R2, LSL #8 
    /*000059b0*/ LSL         R3, R3, #1 
    /*000059b4*/ UBFX        R0, R3, #16, #8 
    /*000059b8*/ UBFX        R1, R3, #8, #8 
    /*000059bc*/ LSR         R2, R3, #24 
    /*000059c0*/ UXTB        R3, R3 
    /*000059c4*/ B           JpegeUpdatePicParamPack_x5478
    JpegeUpdatePicParamPack_x59c8:
    /*000059c8*/ MOV         R2, #0 
    /*000059cc*/ MOV         R1, #8 
    /*000059d0*/ BL          0x00000000000046E0 
    /*000059d4*/ LSR         R3, R0, #8 
    /*000059d8*/ CMP         R3, #9 
    /*000059dc*/ BLS         JpegeUpdatePicParamPack_x54b4
    /*000059e0*/ SUB         R2, R3, #10 
    /*000059e4*/ UXTB        R3, R0 
    /*000059e8*/ ADD         R3, R3, R2, LSL #8 
    /*000059ec*/ UBFX        R0, R3, #16, #8 
    /*000059f0*/ UBFX        R1, R3, #8, #8 
    /*000059f4*/ LSR         R2, R3, #24 
    /*000059f8*/ UXTB        R3, R3 
    /*000059fc*/ B           JpegeUpdatePicParamPack_x54e4
    JpegeUpdatePicParamPack_x5a00:
    /*00005a00*/ MOVW        R3, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x109c] 
    /*00005a04*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00005a08*/ MOVT        R3, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x109c] 
    /*00005a0c*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00005a10*/ MOVW        R2, #387                    @ 0x183 
    /*00005a14*/ LDR         R1, [PC, #32]               @ 0x0000000000005a3c 
    /*00005a18*/ BL          0x0000000000005A18          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005a1c*/ B           JpegeUpdatePicParamPack_x51e8
    /*00005a20*/ LDRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
    /*00005a24*/ ANDEQ       R0, R0, IP, LSR #17 
    /*00005a28*/ ANDEQ       R0, R0, R0, ASR #17 
    /*00005a2c*/ LDRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
    /*00005a30*/ ANDEQ       R0, R0, R4, LSL R9 
    /*00005a34*/ ANDEQ       R0, R0, R4, LSR #18 
    /*00005a38*/ ANDEQ       R0, R0, R4, LSR R9 
    /*00005a3c*/ @           <UNDEFINED> instruction: 0x000005b8 
    /*00005a40*/ LDRDEQ      R0, [R0], -R4 
    /*00005a44*/ ANDEQ       R0, R0, R8, ROR #11 
    /*00005a48*/ ANDEQ       R0, R0, IP, ROR #12 
    /*00005a4c*/ ANDEQ       R0, R0, R4, LSL #16 
    /*00005a50*/ ANDEQ       R0, R0, R8, ASR R8 
    /*00005a54*/ ANDEQ       R0, R0, R0, ROR R8 
FUNC_END JpegeUpdatePicParamPack


FUNC_BEGIN JpegeUpdateOsdConfig
    /*00005a58*/ PUSH        {R4, LR} 
    /*00005a5c*/ ADD         R4, R0, #4096               @ 0x1000 
    /*00005a60*/ LDR         R3, [R4, #2864]             @ 0xb30 
    /*00005a64*/ CMP         R3, #0 
    /*00005a68*/ STR         R3, [R4, #480]              @ 0x1e0 
    /*00005a6c*/ BEQ         JpegeUpdateOsdConfig_x5a90
    /*00005a70*/ LDR         R3, [R4, #3800]             @ 0xed8 
    /*00005a74*/ CMP         R3, #3 
    /*00005a78*/ BEQ         JpegeUpdateOsdConfig_x5ab4
    JpegeUpdateOsdConfig_x5a7c:
    /*00005a7c*/ LDR         R3, [R4, #3768]             @ 0xeb8 
    /*00005a80*/ CMP         R3, #0 
    /*00005a84*/ BEQ         JpegeUpdateOsdConfig_x5a98
    JpegeUpdateOsdConfig_x5a88:
    /*00005a88*/ MOV         R3, #1 
    /*00005a8c*/ STR         R3, [R4, #3768]             @ 0xeb8 
    JpegeUpdateOsdConfig_x5a90:
    /*00005a90*/ MOV         R0, #0 
    /*00005a94*/ POP         {R4, PC} 
    JpegeUpdateOsdConfig_x5a98:
    /*00005a98*/ LDR         R3, [R4, #480]              @ 0x1e0 
    /*00005a9c*/ LDR         R2, [R3] 
    /*00005aa0*/ CMP         R2, #0 
    /*00005aa4*/ STREQ       R2, [R3, #4] 
    /*00005aa8*/ BNE         JpegeUpdateOsdConfig_x5a88
    /*00005aac*/ MOV         R0, #0 
    /*00005ab0*/ POP         {R4, PC} 
    JpegeUpdateOsdConfig_x5ab4:
    /*00005ab4*/ MOV         R0, #1 
    /*00005ab8*/ BL          0x0000000000005AB8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00005abc*/ LDR         R3, [R0, #104]              @ 0x68 
    /*00005ac0*/ MOV         R0, #1 
    /*00005ac4*/ BLX         R3 
    /*00005ac8*/ CMP         R0, #0 
    /*00005acc*/ BEQ         JpegeUpdateOsdConfig_x5a7c
    /*00005ad0*/ LDR         R3, [R4, #480]              @ 0x1e0 
    /*00005ad4*/ MOV         R2, #0 
    /*00005ad8*/ STR         R2, [R3, #16] 
    /*00005adc*/ B           JpegeUpdateOsdConfig_x5a90
FUNC_END JpegeUpdateOsdConfig


FUNC_BEGIN JpegePicStartCtrl
    /*00005ae0*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00005ae4*/ SUB         SP, SP, #100                @ 0x64 
    /*00005ae8*/ MOV         R4, R0 
    /*00005aec*/ MOV         R0, #19 
    /*00005af0*/ MOV         R6, #0 
    /*00005af4*/ MOV         R5, R1 
    /*00005af8*/ STR         R6, [SP, #8] 
    /*00005afc*/ BL          0x0000000000005AFC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00005b00*/ CMP         R0, R6 
    /*00005b04*/ BEQ         JpegePicStartCtrl_x5c54
    /*00005b08*/ MOV         R1, R5 
    /*00005b0c*/ MOV         R0, R4 
    /*00005b10*/ BL          0x0000000000005B10          @ JpegeUpdateOsdConfig [R_ARM_CALL: .text+0x5a58] 
    /*00005b14*/ MOV         R2, #48                     @ 0x30 
    /*00005b18*/ MOV         R1, R6 
    /*00005b1c*/ ADD         R0, SP, #48                 @ 0x30 
    /*00005b20*/ BL          0x0000000000005B20          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00005b24*/ LDR         R3, [R5, #12] 
    /*00005b28*/ MOV         R0, #19 
    /*00005b2c*/ STR         R6, [SP, #52]               @ 0x34 
    /*00005b30*/ STR         R3, [SP, #48]               @ 0x30 
    /*00005b34*/ BL          0x0000000000005B34          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00005b38*/ ADD         R1, SP, #48                 @ 0x30 
    /*00005b3c*/ ADD         R2, SP, #12 
    /*00005b40*/ LDR         R3, [R0, #12] 
    /*00005b44*/ LDR         R0, [R4, #3984]             @ 0xf90 
    /*00005b48*/ BLX         R3 
    /*00005b4c*/ LDR         R3, [SP, #12] 
    /*00005b50*/ CMP         R3, #2 
    /*00005b54*/ BEQ         JpegePicStartCtrl_x5cb8
    /*00005b58*/ MOV         R0, #19 
    /*00005b5c*/ STR         R3, [SP, #52]               @ 0x34 
    /*00005b60*/ BL          0x0000000000005B60          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00005b64*/ ADD         R1, SP, #48                 @ 0x30 
    /*00005b68*/ ADD         R2, SP, #24 
    /*00005b6c*/ LDR         R3, [R0, #16] 
    /*00005b70*/ LDR         R0, [R4, #3984]             @ 0xf90 
    /*00005b74*/ BLX         R3 
    /*00005b78*/ LDR         R1, [SP, #24] 
    /*00005b7c*/ MOV         R6, R0 
    /*00005b80*/ MOV         R0, R4 
    /*00005b84*/ BL          0x0000000000005B84          @ JPEGE_UpdateUniqueID [R_ARM_CALL: .text+0x2d68] 
    /*00005b88*/ CMP         R6, #0 
    /*00005b8c*/ BNE         JpegePicStartCtrl_x5c60
    /*00005b90*/ LDR         R3, [SP, #32] 
    /*00005b94*/ ADD         R7, R4, #3024               @ 0xbd0 
    /*00005b98*/ LDR         R2, [R5, #12] 
    /*00005b9c*/ SUB         R1, R3, #1 
    /*00005ba0*/ CMP         R1, #98                     @ 0x62 
    /*00005ba4*/ STR         R3, [R4, #68]               @ 0x44 
    /*00005ba8*/ STR         R2, [R4, #3036]             @ 0xbdc 
    /*00005bac*/ BHI         JpegePicStartCtrl_x5c34
    JpegePicStartCtrl_x5bb0:
    /*00005bb0*/ MOV         R1, R7 
    /*00005bb4*/ ADD         R0, R4, #16 
    /*00005bb8*/ BL          0x0000000000005BB8          @ JpegeUpdatePicPar [R_ARM_CALL: .text+0x4b68] 
    /*00005bbc*/ MOV         R0, R7 
    /*00005bc0*/ ADD         R3, SP, #8 
    /*00005bc4*/ ADD         R2, R4, #456                @ 0x1c8 
    /*00005bc8*/ ADD         R1, R4, #264                @ 0x108 
    /*00005bcc*/ BL          0x0000000000005BCC          @ JpegeUpdatePicParamPack [R_ARM_CALL: .text+0x4e50] 
    /*00005bd0*/ CMP         R0, #0 
    /*00005bd4*/ BNE         JpegePicStartCtrl_x5c14
    JpegePicStartCtrl_x5bd8:
    /*00005bd8*/ MOV         R1, R5 
    /*00005bdc*/ LDR         R0, [R4, #3984]             @ 0xf90 
    /*00005be0*/ BL          0x000000000000425C 
    /*00005be4*/ CMP         R0, #0 
    /*00005be8*/ BEQ         JpegePicStartCtrl_x5c0c
    /*00005bec*/ MOVW        R3, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x113c] 
    /*00005bf0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00005bf4*/ MOVT        R3, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x113c] 
    /*00005bf8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00005bfc*/ MOVW        R2, #1468                   @ 0x5bc 
    /*00005c00*/ LDR         R1, [PC, #216]              @ 0x0000000000005ce0 
    /*00005c04*/ BL          0x0000000000005C04          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005c08*/ MOV         R0, #0 
    JpegePicStartCtrl_x5c0c:
    /*00005c0c*/ ADD         SP, SP, #100                @ 0x64 
    /*00005c10*/ POP         {R4, R5, R6, R7, PC} 
    JpegePicStartCtrl_x5c14:
    /*00005c14*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd78] 
    /*00005c18*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00005c1c*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd78] 
    /*00005c20*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00005c24*/ MOVW        R2, #1464                   @ 0x5b8 
    /*00005c28*/ LDR         R1, [PC, #176]              @ 0x0000000000005ce0 
    /*00005c2c*/ BL          0x0000000000005C2C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005c30*/ B           JpegePicStartCtrl_x5bd8
    JpegePicStartCtrl_x5c34:
    /*00005c34*/ MOVW        R3, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1198] 
    /*00005c38*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00005c3c*/ MOVT        R3, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1198] 
    /*00005c40*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00005c44*/ MOVW        R2, #1444                   @ 0x5a4 
    /*00005c48*/ LDR         R1, [PC, #144]              @ 0x0000000000005ce0 
    /*00005c4c*/ BL          0x0000000000005C4C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005c50*/ B           JpegePicStartCtrl_x5bb0
    JpegePicStartCtrl_x5c54:
    /*00005c54*/ MOVW        R0, #32774                  @ 0x8006 
    /*00005c58*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00005c5c*/ B           JpegePicStartCtrl_x5c0c
    JpegePicStartCtrl_x5c60:
    /*00005c60*/ MOVW        R3, #1450                   @ 0x5aa 
    /*00005c64*/ MOV         R1, #11 
    /*00005c68*/ STR         R3, [SP] 
    /*00005c6c*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1208] 
    /*00005c70*/ LDR         R3, [PC, #104]              @ 0x0000000000005ce0 
    /*00005c74*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1208] 
    /*00005c78*/ STR         R6, [SP, #4] 
    /*00005c7c*/ MOV         R0, #3 
    /*00005c80*/ BL          0x0000000000005C80          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005c84*/ MOV         R1, R6 
    /*00005c88*/ MOVW        R0, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1248] 
    /*00005c8c*/ MOVT        R0, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1248] 
    /*00005c90*/ BL          0x0000000000005C90          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00005c94*/ MOVW        R3, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd78] 
    /*00005c98*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00005c9c*/ MOVT        R3, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd78] 
    /*00005ca0*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00005ca4*/ MOVW        R2, #1452                   @ 0x5ac 
    /*00005ca8*/ LDR         R1, [PC, #48]               @ 0x0000000000005ce0 
    /*00005cac*/ BL          0x0000000000005CAC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005cb0*/ MVN         R0, #0 
    /*00005cb4*/ B           JpegePicStartCtrl_x5c0c
    JpegePicStartCtrl_x5cb8:
    /*00005cb8*/ MOVW        R3, #1431                   @ 0x597 
    /*00005cbc*/ MOVW        R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1154] 
    /*00005cc0*/ STR         R3, [SP] 
    /*00005cc4*/ MOVT        R2, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1154] 
    /*00005cc8*/ LDR         R3, [PC, #16]               @ 0x0000000000005ce0 
    /*00005ccc*/ MOV         R1, #11 
    /*00005cd0*/ MOV         R0, #5 
    /*00005cd4*/ BL          0x0000000000005CD4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005cd8*/ MVN         R0, #0 
    /*00005cdc*/ B           JpegePicStartCtrl_x5c0c
    /*00005ce0*/ ANDEQ       R0, R0, R8, ASR R9 
FUNC_END JpegePicStartCtrl


FUNC_BEGIN JpegePicStartCtrlSnap
    /*00005ce4*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00005ce8*/ CMP         R0, #0 
    /*00005cec*/ CMPNE       R1, #0 
    /*00005cf0*/ SUB         SP, SP, #8 
    /*00005cf4*/ MOV         R2, #0 
    /*00005cf8*/ STR         R2, [SP, #4] 
    /*00005cfc*/ BEQ         JpegePicStartCtrlSnap_x5e08
    /*00005d00*/ LDR         R2, [R1, #12] 
    /*00005d04*/ MOV         R6, R1 
    /*00005d08*/ CMP         R2, #0 
    /*00005d0c*/ BEQ         JpegePicStartCtrlSnap_x5e08
    /*00005d10*/ ADD         R7, R0, #4096               @ 0x1000 
    /*00005d14*/ STR         R2, [R0, #3036]             @ 0xbdc 
    /*00005d18*/ ADD         R8, R0, #3024               @ 0xbd0 
    /*00005d1c*/ ADD         R5, SP, #8 
    /*00005d20*/ LDR         R3, [R7, #3764]             @ 0xeb4 
    /*00005d24*/ MOV         R4, R0 
    /*00005d28*/ STR         R3, [R0, #3032]             @ 0xbd8 
    /*00005d2c*/ LDR         R2, [R1, #12] 
    /*00005d30*/ LDR         R3, [R7, #3800]             @ 0xed8 
    /*00005d34*/ LDR         R2, [R2, #384]              @ 0x180 
    /*00005d38*/ STR         R3, [R0, #3916]             @ 0xf4c 
    /*00005d3c*/ LDRB        R3, [R7, #3804]             @ 0xedc 
    /*00005d40*/ STR         R2, [R5, #-4]! 
    /*00005d44*/ STRB        R3, [R0, #3920]             @ 0xf50 
    /*00005d48*/ BL          0x0000000000005D48          @ JpegeUpdateOsdConfig [R_ARM_CALL: .text+0x5a58] 
    /*00005d4c*/ MOV         R1, R8 
    /*00005d50*/ ADD         R0, R4, #16 
    /*00005d54*/ BL          0x0000000000005D54          @ JpegeUpdatePicPar [R_ARM_CALL: .text+0x4b68] 
    /*00005d58*/ MOV         R3, R5 
    /*00005d5c*/ MOV         R0, R8 
    /*00005d60*/ ADD         R2, R4, #456                @ 0x1c8 
    /*00005d64*/ ADD         R1, R4, #264                @ 0x108 
    /*00005d68*/ BL          0x0000000000005D68          @ JpegeUpdatePicParamPack [R_ARM_CALL: .text+0x4e50] 
    /*00005d6c*/ CMP         R0, #0 
    /*00005d70*/ BNE         JpegePicStartCtrlSnap_x5dc8
    JpegePicStartCtrlSnap_x5d74:
    /*00005d74*/ LDR         R3, [R7, #3800]             @ 0xed8 
    /*00005d78*/ CMP         R3, #0 
    /*00005d7c*/ BEQ         JpegePicStartCtrlSnap_x5db0
    JpegePicStartCtrlSnap_x5d80:
    /*00005d80*/ STR         R3, [R4, #492]              @ 0x1ec 
    /*00005d84*/ MOV         R1, R6 
    /*00005d88*/ LDR         R0, [R4, #3984]             @ 0xf90 
    /*00005d8c*/ BL          0x000000000000425C 
    /*00005d90*/ CMP         R0, #0 
    /*00005d94*/ BNE         JpegePicStartCtrlSnap_x5de8
    JpegePicStartCtrlSnap_x5d98:
    /*00005d98*/ MOV         R0, R4 
    /*00005d9c*/ MOV         R1, #0 
    /*00005da0*/ BL          0x0000000000005DA0          @ JPEGE_UpdateUniqueID [R_ARM_CALL: .text+0x2d68] 
    /*00005da4*/ MOV         R0, #0 
    JpegePicStartCtrlSnap_x5da8:
    /*00005da8*/ ADD         SP, SP, #8 
    /*00005dac*/ POP         {R4, R5, R6, R7, R8, PC} 
    JpegePicStartCtrlSnap_x5db0:
    /*00005db0*/ LDR         R2, [R4, #3032]             @ 0xbd8 
    /*00005db4*/ CMP         R2, #0 
    /*00005db8*/ MOVNE       R2, #18 
    /*00005dbc*/ MOVEQ       R2, #20 
    /*00005dc0*/ STR         R2, [R7, #3776]             @ 0xec0 
    /*00005dc4*/ B           JpegePicStartCtrlSnap_x5d80
    JpegePicStartCtrlSnap_x5dc8:
    /*00005dc8*/ MOVW        R3, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x113c] 
    /*00005dcc*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00005dd0*/ MOVT        R3, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x113c] 
    /*00005dd4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00005dd8*/ MOVW        R2, #1516                   @ 0x5ec 
    /*00005ddc*/ LDR         R1, [PC, #48]               @ 0x0000000000005e14 
    /*00005de0*/ BL          0x0000000000005DE0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005de4*/ B           JpegePicStartCtrlSnap_x5d74
    JpegePicStartCtrlSnap_x5de8:
    /*00005de8*/ MOVW        R3, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x113c] 
    /*00005dec*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd7c] 
    /*00005df0*/ MOVT        R3, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x113c] 
    /*00005df4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd7c] 
    /*00005df8*/ MOVW        R2, #1529                   @ 0x5f9 
    /*00005dfc*/ LDR         R1, [PC, #16]               @ 0x0000000000005e14 
    /*00005e00*/ BL          0x0000000000005E00          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005e04*/ B           JpegePicStartCtrlSnap_x5d98
    JpegePicStartCtrlSnap_x5e08:
    /*00005e08*/ MOVW        R0, #32774                  @ 0x8006 
    /*00005e0c*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00005e10*/ B           JpegePicStartCtrlSnap_x5da8
    /*00005e14*/ ANDEQ       R0, R0, IP, ROR #18 
FUNC_END JpegePicStartCtrlSnap


FUNC_BEGIN JpegeCheckChnAttr
    /*00005e18*/ PUSH        {R4, LR} 
    /*00005e1c*/ SUB         SP, SP, #16 
    /*00005e20*/ LDR         IP, [R0, #16] 
    /*00005e24*/ LDR         R2, [R0, #12] 
    /*00005e28*/ LDR         R1, [R0, #24] 
    /*00005e2c*/ LDR         R3, [R0, #20] 
    /*00005e30*/ MUL         R2, R2, IP 
    /*00005e34*/ MUL         R3, R3, R1 
    /*00005e38*/ CMP         R2, R3 
    /*00005e3c*/ BCC         JpegeCheckChnAttr_x5e90
    /*00005e40*/ LDR         R3, [R0, #32] 
    /*00005e44*/ MOV         R4, R0 
    /*00005e48*/ CMP         R3, #1 
    /*00005e4c*/ BHI         JpegeCheckChnAttr_x5e64
    /*00005e50*/ BL          0x0000000000005E50          @ JPEGE_DRV_CheckDcf [R_ARM_CALL: .text+0x73e0] 
    /*00005e54*/ CMP         R0, #0 
    /*00005e58*/ BNE         JpegeCheckChnAttr_x5ec0
    JpegeCheckChnAttr_x5e5c:
    /*00005e5c*/ ADD         SP, SP, #16 
    /*00005e60*/ POP         {R4, PC} 
    JpegeCheckChnAttr_x5e64:
    /*00005e64*/ STR         R3, [SP, #4] 
    /*00005e68*/ MOVW        R2, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12f0] 
    /*00005e6c*/ MOVW        R3, #1557                   @ 0x615 
    /*00005e70*/ MOVT        R2, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12f0] 
    /*00005e74*/ STR         R3, [SP] 
    /*00005e78*/ MOV         R1, #11 
    /*00005e7c*/ LDR         R3, [PC, #108]              @ 0x0000000000005ef0 
    /*00005e80*/ MOV         R0, #3 
    /*00005e84*/ BL          0x0000000000005E84          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005e88*/ MVN         R0, #0 
    /*00005e8c*/ B           JpegeCheckChnAttr_x5e5c
    JpegeCheckChnAttr_x5e90:
    /*00005e90*/ STR         R2, [SP, #8] 
    /*00005e94*/ MOVW        R1, #1548                   @ 0x60c 
    /*00005e98*/ STR         R3, [SP, #4] 
    /*00005e9c*/ MOVW        R2, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x126c] 
    /*00005ea0*/ STR         R1, [SP] 
    /*00005ea4*/ MOVT        R2, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x126c] 
    /*00005ea8*/ LDR         R3, [PC, #64]               @ 0x0000000000005ef0 
    /*00005eac*/ MOV         R1, #11 
    /*00005eb0*/ MOV         R0, #3 
    /*00005eb4*/ BL          0x0000000000005EB4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005eb8*/ MVN         R0, #0 
    /*00005ebc*/ B           JpegeCheckChnAttr_x5e5c
    JpegeCheckChnAttr_x5ec0:
    /*00005ec0*/ LDR         R1, [R4, #32] 
    /*00005ec4*/ MOVW        R3, #1566                   @ 0x61e 
    /*00005ec8*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1340] 
    /*00005ecc*/ STR         R3, [SP] 
    /*00005ed0*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1340] 
    /*00005ed4*/ LDR         R3, [PC, #20]               @ 0x0000000000005ef0 
    /*00005ed8*/ STR         R1, [SP, #4] 
    /*00005edc*/ MOV         R0, #3 
    /*00005ee0*/ MOV         R1, #11 
    /*00005ee4*/ BL          0x0000000000005EE4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005ee8*/ MVN         R0, #0 
    /*00005eec*/ B           JpegeCheckChnAttr_x5e5c
    /*00005ef0*/ ANDEQ       R0, R0, R4, LSL #19 
FUNC_END JpegeCheckChnAttr


FUNC_BEGIN Jpege_CheckAttr
    /*00005ef4*/ MOVW        R3, #0                      @ g_pstJpegeChnCtx [R_ARM_MOVW_ABS_NC: .bss+0x4c] 
    /*00005ef8*/ MOVT        R3, #0                      @ g_pstJpegeChnCtx [R_ARM_MOVT_ABS: .bss+0x4c] 
    /*00005efc*/ LDR         R3, [R3, R0, LSL #2] 
    /*00005f00*/ CMP         R3, #0 
    /*00005f04*/ BEQ         Jpege_CheckAttr_x6038
    /*00005f08*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00005f0c*/ MOV         R0, R1 
    /*00005f10*/ LDR         R2, [R3, #16] 
    /*00005f14*/ SUB         SP, SP, #12 
    /*00005f18*/ LDR         R1, [R1] 
    /*00005f1c*/ CMP         R1, R2 
    /*00005f20*/ BNE         Jpege_CheckAttr_x5fd4
    /*00005f24*/ LDR         R1, [R0, #8] 
    /*00005f28*/ LDR         R2, [R3, #24] 
    /*00005f2c*/ CMP         R1, R2 
    /*00005f30*/ BNE         Jpege_CheckAttr_x5fd4
    /*00005f34*/ LDR         R1, [R0, #12] 
    /*00005f38*/ LDR         R2, [R3, #28] 
    /*00005f3c*/ CMP         R1, R2 
    /*00005f40*/ BNE         Jpege_CheckAttr_x5fd4
    /*00005f44*/ LDR         R1, [R0, #16] 
    /*00005f48*/ LDR         R2, [R3, #32] 
    /*00005f4c*/ CMP         R1, R2 
    /*00005f50*/ BNE         Jpege_CheckAttr_x5fd4
    /*00005f54*/ LDR         R1, [R0, #28] 
    /*00005f58*/ LDR         R2, [R3, #44]               @ 0x2c 
    /*00005f5c*/ CMP         R1, R2 
    /*00005f60*/ BNE         Jpege_CheckAttr_x5fd4
    /*00005f64*/ LDR         R1, [R0, #32] 
    /*00005f68*/ LDR         R2, [R3, #48]               @ 0x30 
    /*00005f6c*/ CMP         R1, R2 
    /*00005f70*/ BNE         Jpege_CheckAttr_x5fd4
    /*00005f74*/ LDR         R1, [R0, #20] 
    /*00005f78*/ LDR         R2, [R3, #36]               @ 0x24 
    /*00005f7c*/ CMP         R1, R2 
    /*00005f80*/ BEQ         Jpege_CheckAttr_x5f98
    Jpege_CheckAttr_x5f84:
    /*00005f84*/ BL          0x0000000000005F84          @ JpegeCheckChnAttr [R_ARM_CALL: .text+0x5e18] 
    /*00005f88*/ CMP         R0, #0 
    /*00005f8c*/ BNE         Jpege_CheckAttr_x6008
    Jpege_CheckAttr_x5f90:
    /*00005f90*/ ADD         SP, SP, #12 
    /*00005f94*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    Jpege_CheckAttr_x5f98:
    /*00005f98*/ LDR         R3, [R3, #40]               @ 0x28 
    /*00005f9c*/ LDR         R2, [R0, #24] 
    /*00005fa0*/ CMP         R2, R3 
    /*00005fa4*/ BNE         Jpege_CheckAttr_x5f84
    /*00005fa8*/ LDR         R3, [PC, #148]              @ 0x0000000000006044 
    /*00005fac*/ MOVW        R1, #1605                   @ 0x645 
    /*00005fb0*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13dc] 
    /*00005fb4*/ STR         R1, [SP] 
    /*00005fb8*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13dc] 
    /*00005fbc*/ MOV         R1, #11 
    /*00005fc0*/ STR         R3, [SP, #4] 
    /*00005fc4*/ MOV         R0, #4 
    /*00005fc8*/ BL          0x0000000000005FC8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005fcc*/ MOV         R0, #0 
    /*00005fd0*/ B           Jpege_CheckAttr_x5f90
    Jpege_CheckAttr_x5fd4:
    /*00005fd4*/ LDR         R3, [PC, #104]              @ 0x0000000000006044 
    /*00005fd8*/ MOVW        R1, #1597                   @ 0x63d 
    /*00005fdc*/ MOVW        R2, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x139c] 
    /*00005fe0*/ STR         R1, [SP] 
    /*00005fe4*/ MOVT        R2, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x139c] 
    /*00005fe8*/ MOV         R1, #11 
    /*00005fec*/ STR         R3, [SP, #4] 
    /*00005ff0*/ MOV         R0, #3 
    /*00005ff4*/ BL          0x0000000000005FF4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005ff8*/ MOVW        R0, #32777                  @ 0x8009 
    /*00005ffc*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00006000*/ ADD         SP, SP, #12 
    /*00006004*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    Jpege_CheckAttr_x6008:
    /*00006008*/ LDR         R3, [PC, #52]               @ 0x0000000000006044 
    /*0000600c*/ MOVW        R1, #1615                   @ 0x64f 
    /*00006010*/ MOVW        R2, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x141c] 
    /*00006014*/ STR         R1, [SP] 
    /*00006018*/ MOVT        R2, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x141c] 
    /*0000601c*/ MOV         R1, #11 
    /*00006020*/ STR         R3, [SP, #4] 
    /*00006024*/ MOV         R0, #3 
    /*00006028*/ BL          0x0000000000006028          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000602c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00006030*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00006034*/ B           Jpege_CheckAttr_x5f90
    Jpege_CheckAttr_x6038:
    /*00006038*/ MOVW        R0, #32773                  @ 0x8005 
    /*0000603c*/ MOVT        R0, #40971                  @ 0xa00b 
    /*00006040*/ BX          LR 
    /*00006044*/ MULEQ       R0, R8, R9 
FUNC_END Jpege_CheckAttr


FUNC_BEGIN Jpge_init_quant_table
    /*00006048*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000604c*/ SUB         R5, R0, #4 
    /*00006050*/ SUB         R4, R1, #1 
    /*00006054*/ ADD         R6, R1, #63                 @ 0x3f 
    /*00006058*/ MOV         R7, R2 
    Jpge_init_quant_table_x605c:
    /*0000605c*/ LDRB        R1, [R4, #1]! 
    /*00006060*/ MOV         R0, #8388608                @ 0x800000 
    /*00006064*/ MUL         R1, R7, R1 
    /*00006068*/ BL          0x0000000000006068          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000606c*/ CMP         R4, R6 
    /*00006070*/ STR         R0, [R5, #4]! 
    /*00006074*/ BNE         Jpge_init_quant_table_x605c
    /*00006078*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END Jpge_init_quant_table


FUNC_BEGIN JpegeProcJpegRunInfo
    /*0000607c*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00006080*/ SUB         SP, SP, #24 
    /*00006084*/ MOV         R7, R0 
    /*00006088*/ MOVW        R1, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1450] 
    /*0000608c*/ MOVT        R1, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1450] 
    /*00006090*/ LDR         R4, [PC, #224]              @ 0x0000000000006178 
    /*00006094*/ BL          0x0000000000006094          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006098*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14d8] 
    /*0000609c*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14e8] 
    /*000060a0*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14d8] 
    /*000060a4*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14e8] 
    /*000060a8*/ STR         R2, [SP, #20] 
    /*000060ac*/ STR         R3, [SP, #16] 
    /*000060b0*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14f0] 
    /*000060b4*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14fc] 
    /*000060b8*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14f0] 
    /*000060bc*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14fc] 
    /*000060c0*/ STR         R2, [SP, #12] 
    /*000060c4*/ STR         R3, [SP, #8] 
    /*000060c8*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1508] 
    /*000060cc*/ MOVW        R3, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1510] 
    /*000060d0*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1508] 
    /*000060d4*/ MOVT        R3, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1510] 
    /*000060d8*/ STR         R2, [SP, #4] 
    /*000060dc*/ STR         R3, [SP] 
    /*000060e0*/ MOV         R0, R7 
    /*000060e4*/ MOVW        R3, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14a0] 
    /*000060e8*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14a8] 
    /*000060ec*/ MOVT        R3, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14a0] 
    /*000060f0*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14a8] 
    /*000060f4*/ MOVW        R1, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14ac] 
    /*000060f8*/ MOVT        R1, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14ac] 
    /*000060fc*/ BL          0x00000000000060FC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006100*/ MOVW        R6, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x151c] 
    /*00006104*/ ADD         R8, R4, #64                 @ 0x40 
    /*00006108*/ MOVT        R6, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x151c] 
    /*0000610c*/ MOV         R5, #0 
    JpegeProcJpegRunInfo_x6110:
    /*00006110*/ LDR         R3, [R4, #4]! 
    /*00006114*/ MOV         R2, R5 
    /*00006118*/ MOV         R1, R6 
    /*0000611c*/ MOV         R0, R7 
    /*00006120*/ CMP         R3, #0 
    /*00006124*/ ADD         R5, R5, #1 
    /*00006128*/ ADD         IP, R3, #4096               @ 0x1000 
    /*0000612c*/ BEQ         JpegeProcJpegRunInfo_x6168
    /*00006130*/ LDR         LR, [IP, #3800]             @ 0xed8 
    /*00006134*/ LDR         R3, [IP, #3772]             @ 0xebc 
    /*00006138*/ STR         LR, [SP, #20] 
    /*0000613c*/ LDR         LR, [IP, #3796]             @ 0xed4 
    /*00006140*/ STR         LR, [SP, #16] 
    /*00006144*/ LDR         LR, [IP, #3792]             @ 0xed0 
    /*00006148*/ STR         LR, [SP, #12] 
    /*0000614c*/ LDR         LR, [IP, #3788]             @ 0xecc 
    /*00006150*/ STR         LR, [SP, #8] 
    /*00006154*/ LDR         LR, [IP, #3784]             @ 0xec8 
    /*00006158*/ STR         LR, [SP, #4] 
    /*0000615c*/ LDR         IP, [IP, #3780]             @ 0xec4 
    /*00006160*/ STR         IP, [SP] 
    /*00006164*/ BL          0x0000000000006164          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    JpegeProcJpegRunInfo_x6168:
    /*00006168*/ CMP         R4, R8 
    /*0000616c*/ BNE         JpegeProcJpegRunInfo_x6110
    /*00006170*/ ADD         SP, SP, #24 
    /*00006174*/ POP         {R4, R5, R6, R7, R8, PC} 
    /*00006178*/ @           <UNDEFINED> instruction: 0xfffffffc @ g_pstJpegeChnCtx [R_ARM_ABS32: .bss+0x4c] 
FUNC_END JpegeProcJpegRunInfo


FUNC_BEGIN JpegeProcShow
    /*0000617c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00006180*/ MOV         R4, R0 
    /*00006184*/ SUB         SP, SP, #52                 @ 0x34 
    /*00006188*/ MOVW        R0, #0                      @ g_stJpegeSem [R_ARM_MOVW_ABS_NC: .bss+0x48] 
    /*0000618c*/ MOVT        R0, #0                      @ g_stJpegeSem [R_ARM_MOVT_ABS: .bss+0x48] 
    /*00006190*/ BL          0x0000000000006190          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00006194*/ SUBS        R7, R0, #0 
    /*00006198*/ BNE         JpegeProcShow_x66c8
    /*0000619c*/ MOVW        R1, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1548] 
    /*000061a0*/ MOV         R0, R4 
    /*000061a4*/ MOVT        R1, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1548] 
    /*000061a8*/ LDR         R6, [PC, #1316]             @ 0x00000000000066d4 
    /*000061ac*/ BL          0x00000000000061AC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000061b0*/ MOVW        R1, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a8] 
    /*000061b4*/ MOV         R0, R4 
    /*000061b8*/ MOVT        R1, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a8] 
    /*000061bc*/ BL          0x00000000000061BC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000061c0*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1628] 
    /*000061c4*/ MOVW        R3, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15fc] 
    /*000061c8*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1628] 
    /*000061cc*/ MOVT        R3, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15fc] 
    /*000061d0*/ STR         R2, [SP] 
    /*000061d4*/ MOVW        R1, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1618] 
    /*000061d8*/ MOVW        R2, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1610] 
    /*000061dc*/ MOVT        R1, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1618] 
    /*000061e0*/ MOVT        R2, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1610] 
    /*000061e4*/ MOV         R0, R4 
    /*000061e8*/ MOVW        R9, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x9a8] 
    /*000061ec*/ BL          0x00000000000061EC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000061f0*/ MOVW        R0, #0                      @ JpegClearStreamBuf [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*000061f4*/ MOVT        R0, #0                      @ JpegClearStreamBuf [R_ARM_MOVT_ABS: .data+0x0] 
    /*000061f8*/ MOVW        R3, #0                      @ JpegeMiniBufMode [R_ARM_MOVW_ABS_NC: .bss+0x40] 
    /*000061fc*/ MOVW        R2, #0                      @ JpegeOneStreamBuffer [R_ARM_MOVW_ABS_NC: .bss+0x44] 
    /*00006200*/ MOVT        R3, #0                      @ JpegeMiniBufMode [R_ARM_MOVT_ABS: .bss+0x40] 
    /*00006204*/ LDR         IP, [R0] 
    /*00006208*/ MOVT        R2, #0                      @ JpegeOneStreamBuffer [R_ARM_MOVT_ABS: .bss+0x44] 
    /*0000620c*/ LDR         R3, [R3] 
    /*00006210*/ MOVW        R1, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x163c] 
    /*00006214*/ LDR         R2, [R2] 
    /*00006218*/ MOVT        R1, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x163c] 
    /*0000621c*/ STR         IP, [SP] 
    /*00006220*/ MOV         R0, R4 
    /*00006224*/ BL          0x0000000000006224          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006228*/ MOVW        R1, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x164c] 
    /*0000622c*/ MOV         R0, R4 
    /*00006230*/ MOVT        R1, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x164c] 
    /*00006234*/ BL          0x0000000000006234          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006238*/ MOVW        R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16d8] 
    /*0000623c*/ MOVW        R3, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e0] 
    /*00006240*/ MOVT        R2, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16d8] 
    /*00006244*/ MOVT        R3, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e0] 
    /*00006248*/ STR         R2, [SP, #40]               @ 0x28 
    /*0000624c*/ STR         R3, [SP, #36]               @ 0x24 
    /*00006250*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16e8] 
    /*00006254*/ MOVW        R3, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16f0] 
    /*00006258*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16e8] 
    /*0000625c*/ MOVT        R3, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16f0] 
    /*00006260*/ STR         R2, [SP, #32] 
    /*00006264*/ STR         R3, [SP, #28] 
    /*00006268*/ MOVW        R2, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16f4] 
    /*0000626c*/ MOVW        R3, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16fc] 
    /*00006270*/ MOVT        R2, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16f4] 
    /*00006274*/ MOVT        R3, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16fc] 
    /*00006278*/ STR         R2, [SP, #24] 
    /*0000627c*/ STR         R3, [SP, #20] 
    /*00006280*/ MOVW        R2, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1704] 
    /*00006284*/ MOVW        R3, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170c] 
    /*00006288*/ MOVT        R2, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1704] 
    /*0000628c*/ MOVT        R3, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170c] 
    /*00006290*/ STR         R2, [SP, #16] 
    /*00006294*/ STR         R3, [SP, #12] 
    /*00006298*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1714] 
    /*0000629c*/ MOVW        R3, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1720] 
    /*000062a0*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1714] 
    /*000062a4*/ MOVT        R3, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1720] 
    /*000062a8*/ STR         R2, [SP, #8] 
    /*000062ac*/ STR         R3, [SP, #4] 
    /*000062b0*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x172c] 
    /*000062b4*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16a0] 
    /*000062b8*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x172c] 
    /*000062bc*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16a0] 
    /*000062c0*/ STR         R2, [SP] 
    /*000062c4*/ MOVW        R1, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16a8] 
    /*000062c8*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14a8] 
    /*000062cc*/ MOVT        R1, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16a8] 
    /*000062d0*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14a8] 
    /*000062d4*/ MOV         R0, R4 
    /*000062d8*/ MOVW        SL, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1734] 
    /*000062dc*/ BL          0x00000000000062DC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000062e0*/ MOVT        R9, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x9a8] 
    /*000062e4*/ MOVT        SL, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1734] 
    /*000062e8*/ MOV         R5, R6 
    /*000062ec*/ MOV         R8, R7 
    JpegeProcShow_x62f0:
    /*000062f0*/ LDR         IP, [R5, #4]! 
    /*000062f4*/ MOV         R2, R8 
    /*000062f8*/ MOV         R1, SL 
    /*000062fc*/ MOV         R0, R4 
    /*00006300*/ CMP         IP, #0 
    /*00006304*/ ADD         R8, R8, #1 
    /*00006308*/ ADD         FP, IP, #4096               @ 0x1000 
    /*0000630c*/ BEQ         JpegeProcShow_x6388
    /*00006310*/ LDR         LR, [IP, #48]               @ 0x30 
    /*00006314*/ LDR         R3, [IP, #16] 
    /*00006318*/ ADD         LR, R9, LR, LSL #1 
    /*0000631c*/ STR         LR, [SP, #40]               @ 0x28 
    /*00006320*/ LDR         LR, [FP, #3760]             @ 0xeb0 
    /*00006324*/ ADD         R3, R9, R3, LSL #1 
    /*00006328*/ ADD         LR, R9, LR, LSL #1 
    /*0000632c*/ STR         LR, [SP, #36]               @ 0x24 
    /*00006330*/ LDR         LR, [IP, #68]               @ 0x44 
    /*00006334*/ STR         LR, [SP, #32] 
    /*00006338*/ LDR         LR, [IP, #64]               @ 0x40 
    /*0000633c*/ STR         LR, [SP, #28] 
    /*00006340*/ LDR         LR, [IP, #44]               @ 0x2c 
    /*00006344*/ ADD         LR, R9, LR, LSL #1 
    /*00006348*/ STR         LR, [SP, #24] 
    /*0000634c*/ LDR         LR, [IP, #24] 
    /*00006350*/ STR         LR, [SP, #20] 
    /*00006354*/ LDR         LR, [IP, #40]               @ 0x28 
    /*00006358*/ STR         LR, [SP, #16] 
    /*0000635c*/ LDR         LR, [IP, #36]               @ 0x24 
    /*00006360*/ STR         LR, [SP, #12] 
    /*00006364*/ LDR         LR, [IP, #32] 
    /*00006368*/ STR         LR, [SP, #8] 
    /*0000636c*/ LDR         LR, [IP, #28] 
    /*00006370*/ STR         LR, [SP, #4] 
    /*00006374*/ LDR         IP, [IP, #3968]             @ 0xf80 
    /*00006378*/ LDR         LR, [PC, #856]              @ 0x00000000000066d8 
    /*0000637c*/ ADD         IP, LR, IP, LSL #3 
    /*00006380*/ STR         IP, [SP] 
    /*00006384*/ BL          0x0000000000006384          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    JpegeProcShow_x6388:
    /*00006388*/ LDR         R3, [PC, #844]              @ 0x00000000000066dc 
    /*0000638c*/ CMP         R5, R3 
    /*00006390*/ BNE         JpegeProcShow_x62f0
    /*00006394*/ MOVW        R1, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1764] 
    /*00006398*/ MOV         R0, R4 
    /*0000639c*/ MOVT        R1, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1764] 
    /*000063a0*/ SUB         R8, R3, #64                 @ 0x40 
    /*000063a4*/ BL          0x00000000000063A4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000063a8*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17d8] 
    /*000063ac*/ MOVW        R3, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17e0] 
    /*000063b0*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17d8] 
    /*000063b4*/ MOVT        R3, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17e0] 
    /*000063b8*/ STR         R2, [SP, #8] 
    /*000063bc*/ STR         R3, [SP, #4] 
    /*000063c0*/ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17e8] 
    /*000063c4*/ MOVW        R3, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17b8] 
    /*000063c8*/ MOVT        R2, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17e8] 
    /*000063cc*/ MOVT        R3, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17b8] 
    /*000063d0*/ STR         R2, [SP] 
    /*000063d4*/ MOVW        R1, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17c0] 
    /*000063d8*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14a8] 
    /*000063dc*/ MOVT        R1, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17c0] 
    /*000063e0*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14a8] 
    /*000063e4*/ MOV         R0, R4 
    /*000063e8*/ MOVW        SL, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17f0] 
    /*000063ec*/ BL          0x00000000000063EC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000063f0*/ MOVT        SL, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17f0] 
    /*000063f4*/ MOV         R9, #0 
    JpegeProcShow_x63f8:
    /*000063f8*/ LDR         IP, [R8, #4]! 
    /*000063fc*/ CMP         IP, #0 
    /*00006400*/ BEQ         JpegeProcShow_x6460
    /*00006404*/ LDR         R1, [IP, #452]              @ 0x1c4 
    /*00006408*/ LDR         FP, [IP, #3976]             @ 0xf88 
    /*0000640c*/ LDR         R2, [R1, #164]              @ 0xa4 
    /*00006410*/ LDR         R0, [R1, #168]              @ 0xa8 
    /*00006414*/ LDR         R3, [R1, #8] 
    /*00006418*/ LDR         R2, [R2] 
    /*0000641c*/ LDR         R0, [R0] 
    /*00006420*/ CMP         R2, R0 
    /*00006424*/ SUB         LR, R2, R0 
    /*00006428*/ LDRLS       R1, [R1, #12] 
    /*0000642c*/ LDRHI       R2, [R1, #12] 
    /*00006430*/ STR         FP, [SP, #8] 
    /*00006434*/ SUBLS       R2, R2, R1 
    /*00006438*/ LDR         IP, [IP, #3980]             @ 0xf8c 
    /*0000643c*/ ADDLS       R2, R2, R3 
    /*00006440*/ SUBLS       LR, R2, R0 
    /*00006444*/ SUBHI       LR, LR, R2 
    /*00006448*/ MOV         R1, SL 
    /*0000644c*/ MOV         R2, R9 
    /*00006450*/ MOV         R0, R4 
    /*00006454*/ STR         LR, [SP] 
    /*00006458*/ STR         IP, [SP, #4] 
    /*0000645c*/ BL          0x000000000000645C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    JpegeProcShow_x6460:
    /*00006460*/ CMP         R5, R8 
    /*00006464*/ ADD         R9, R9, #1 
    /*00006468*/ BNE         JpegeProcShow_x63f8
    /*0000646c*/ MOVW        R1, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1808] 
    /*00006470*/ MOV         R0, R4 
    /*00006474*/ MOVT        R1, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1808] 
    /*00006478*/ LDR         R5, [PC, #596]              @ 0x00000000000066d4 
    /*0000647c*/ BL          0x000000000000647C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006480*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1894] 
    /*00006484*/ MOVW        R3, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x189c] 
    /*00006488*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1894] 
    /*0000648c*/ MOVT        R3, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x189c] 
    /*00006490*/ STR         R2, [SP, #24] 
    /*00006494*/ STR         R3, [SP, #20] 
    /*00006498*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18a8] 
    /*0000649c*/ MOVW        R3, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18b0] 
    /*000064a0*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18a8] 
    /*000064a4*/ MOVT        R3, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18b0] 
    /*000064a8*/ STR         R2, [SP, #16] 
    /*000064ac*/ STR         R3, [SP, #12] 
    /*000064b0*/ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18bc] 
    /*000064b4*/ MOVW        R3, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18c4] 
    /*000064b8*/ MOVT        R2, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18bc] 
    /*000064bc*/ MOVT        R3, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18c4] 
    /*000064c0*/ STR         R2, [SP, #8] 
    /*000064c4*/ STR         R3, [SP, #4] 
    /*000064c8*/ MOVW        R2, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18d0] 
    /*000064cc*/ MOVW        R3, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x185c] 
    /*000064d0*/ MOVT        R2, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18d0] 
    /*000064d4*/ MOVT        R3, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x185c] 
    /*000064d8*/ STR         R2, [SP] 
    /*000064dc*/ MOVW        R1, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1864] 
    /*000064e0*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14a8] 
    /*000064e4*/ MOVT        R1, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1864] 
    /*000064e8*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14a8] 
    /*000064ec*/ MOV         R0, R4 
    /*000064f0*/ MOVW        R9, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18dc] 
    /*000064f4*/ BL          0x00000000000064F4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000064f8*/ MOVT        R9, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18dc] 
    JpegeProcShow_x64fc:
    /*000064fc*/ LDR         IP, [R5, #4]! 
    /*00006500*/ MOV         R2, R7 
    /*00006504*/ MOV         R1, R9 
    /*00006508*/ MOV         R0, R4 
    /*0000650c*/ CMP         IP, #0 
    /*00006510*/ ADD         R7, R7, #1 
    /*00006514*/ BEQ         JpegeProcShow_x6558
    /*00006518*/ LDR         LR, [IP, #3980]             @ 0xf8c 
    /*0000651c*/ LDR         R3, [IP, #3924]             @ 0xf54 
    /*00006520*/ STR         LR, [SP, #24] 
    /*00006524*/ LDR         LR, [IP, #3932]             @ 0xf5c 
    /*00006528*/ STR         LR, [SP, #20] 
    /*0000652c*/ LDR         LR, [IP, #3952]             @ 0xf70 
    /*00006530*/ STR         LR, [SP, #16] 
    /*00006534*/ LDR         LR, [IP, #3936]             @ 0xf60 
    /*00006538*/ STR         LR, [SP, #12] 
    /*0000653c*/ LDR         LR, [IP, #3928]             @ 0xf58 
    /*00006540*/ STR         LR, [SP, #8] 
    /*00006544*/ LDR         LR, [IP, #3940]             @ 0xf64 
    /*00006548*/ STR         LR, [SP, #4] 
    /*0000654c*/ LDR         IP, [IP, #3944]             @ 0xf68 
    /*00006550*/ STR         IP, [SP] 
    /*00006554*/ BL          0x0000000000006554          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    JpegeProcShow_x6558:
    /*00006558*/ CMP         R8, R5 
    /*0000655c*/ BNE         JpegeProcShow_x64fc
    /*00006560*/ MOVW        R1, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x190c] 
    /*00006564*/ MOV         R0, R4 
    /*00006568*/ MOVT        R1, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0x190c] 
    /*0000656c*/ MOVW        R5, #0                      @ .LC61 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19ec] 
    /*00006570*/ BL          0x0000000000006570          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00006574*/ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1998] 
    /*00006578*/ MOVW        R3, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19a4] 
    /*0000657c*/ MOVT        R2, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1998] 
    /*00006580*/ MOVT        R3, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19a4] 
    /*00006584*/ STR         R2, [SP, #24] 
    /*00006588*/ STR         R3, [SP, #20] 
    /*0000658c*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19b0] 
    /*00006590*/ MOVW        R3, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19bc] 
    /*00006594*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19b0] 
    /*00006598*/ MOVT        R3, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19bc] 
    /*0000659c*/ STR         R2, [SP, #16] 
    /*000065a0*/ STR         R3, [SP, #12] 
    /*000065a4*/ MOVW        R2, #0                      @ .LC58 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19c8] 
    /*000065a8*/ MOVW        R3, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19d4] 
    /*000065ac*/ MOVT        R2, #0                      @ .LC58 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19c8] 
    /*000065b0*/ MOVT        R3, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19d4] 
    /*000065b4*/ STR         R2, [SP, #8] 
    /*000065b8*/ STR         R3, [SP, #4] 
    /*000065bc*/ MOVW        R2, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19e0] 
    /*000065c0*/ MOVW        R3, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1960] 
    /*000065c4*/ MOVT        R2, #0                      @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19e0] 
    /*000065c8*/ MOVW        R1, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1968] 
    /*000065cc*/ STR         R2, [SP] 
    /*000065d0*/ MOVT        R3, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1960] 
    /*000065d4*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14a8] 
    /*000065d8*/ MOVT        R1, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1968] 
    /*000065dc*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14a8] 
    /*000065e0*/ MOV         R0, R4 
    /*000065e4*/ MOVT        R5, #0                      @ .LC61 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19ec] 
    /*000065e8*/ BL          0x00000000000065E8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    JpegeProcShow_x65ec:
    /*000065ec*/ LDR         R2, [R6, #4]! 
    /*000065f0*/ CMP         R2, #0 
    /*000065f4*/ BEQ         JpegeProcShow_x6690
    /*000065f8*/ LDR         IP, [R2, #452]              @ 0x1c4 
    /*000065fc*/ LDR         R2, [R2, #3984]             @ 0xf90 
    /*00006600*/ LDR         R1, [IP, #160]              @ 0xa0 
    /*00006604*/ LDR         R3, [IP, #172]              @ 0xac 
    /*00006608*/ LDR         LR, [IP, #8] 
    /*0000660c*/ LDR         R1, [R1] 
    /*00006610*/ LDR         R3, [R3] 
    /*00006614*/ CMP         R1, R3 
    /*00006618*/ SUB         R7, R3, R1 
    /*0000661c*/ ADDHI       R3, R3, LR 
    /*00006620*/ SUBHI       R7, R3, R1 
    /*00006624*/ LDR         R3, [IP, #164]              @ 0xa4 
    /*00006628*/ LDR         R1, [IP, #168]              @ 0xa8 
    /*0000662c*/ LDR         R3, [R3] 
    /*00006630*/ LDR         R1, [R1] 
    /*00006634*/ CMP         R3, R1 
    /*00006638*/ SUB         R9, R3, R1 
    /*0000663c*/ LDRLS       R0, [IP, #12] 
    /*00006640*/ LDRHI       R3, [IP, #12] 
    /*00006644*/ SUBLS       R3, R3, R0 
    /*00006648*/ MOV         R0, R4 
    /*0000664c*/ SUBLS       R3, R3, R1 
    /*00006650*/ SUBHI       R9, R9, R3 
    /*00006654*/ ADDLS       R9, R3, LR 
    /*00006658*/ LDR         R3, [IP, #176]              @ 0xb0 
    /*0000665c*/ STR         R9, [SP, #24] 
    /*00006660*/ MOV         R1, R5 
    /*00006664*/ STR         R7, [SP, #20] 
    /*00006668*/ STR         LR, [SP, #16] 
    /*0000666c*/ LDR         LR, [IP, #128]              @ 0x80 
    /*00006670*/ STR         LR, [SP, #12] 
    /*00006674*/ LDR         LR, [IP, #96]               @ 0x60 
    /*00006678*/ STR         LR, [SP, #8] 
    /*0000667c*/ LDR         LR, [IP, #32] 
    /*00006680*/ STR         LR, [SP, #4] 
    /*00006684*/ LDR         IP, [IP, #64]               @ 0x40 
    /*00006688*/ STR         IP, [SP] 
    /*0000668c*/ BL          0x000000000000668C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    JpegeProcShow_x6690:
    /*00006690*/ CMP         R6, R8 
    /*00006694*/ BNE         JpegeProcShow_x65ec
    /*00006698*/ MOVW        R1, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a1c] 
    /*0000669c*/ MOV         R0, R4 
    /*000066a0*/ MOVT        R1, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a1c] 
    /*000066a4*/ BL          0x00000000000066A4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000066a8*/ MOV         R0, R4 
    /*000066ac*/ BL          0x00000000000066AC          @ JpegeProcJpegRunInfo [R_ARM_CALL: .text+0x607c] 
    /*000066b0*/ MOVW        R0, #0                      @ g_stJpegeSem [R_ARM_MOVW_ABS_NC: .bss+0x48] 
    /*000066b4*/ MOVT        R0, #0                      @ g_stJpegeSem [R_ARM_MOVT_ABS: .bss+0x48] 
    /*000066b8*/ BL          0x00000000000066B8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000066bc*/ MOV         R0, #0 
    JpegeProcShow_x66c0:
    /*000066c0*/ ADD         SP, SP, #52                 @ 0x34 
    /*000066c4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JpegeProcShow_x66c8:
    /*000066c8*/ MOV         R0, #65024                  @ 0xfe00 
    /*000066cc*/ MOVT        R0, #65535                  @ 0xffff 
    /*000066d0*/ B           JpegeProcShow_x66c0
    /*000066d4*/ @           <UNDEFINED> instruction: 0xfffffffc @ g_pstJpegeChnCtx [R_ARM_ABS32: .bss+0x4c] 
    /*000066d8*/ ANDEQ       R0, R0, IP, LSR #19 
    /*000066dc*/ ANDEQ       R0, R0, IP, LSR R0          @ g_pstJpegeChnCtx [R_ARM_ABS32: .bss+0x4c] 
FUNC_END JpegeProcShow


FUNC_BEGIN STR_SameString
    /*000066e0*/ PUSH        {R4, LR} 
    /*000066e4*/ BL          0x00000000000066E4          @ osal_strcmp [R_ARM_CALL: +0x0] 
    /*000066e8*/ CLZ         R0, R0 
    /*000066ec*/ LSR         R0, R0, #5 
    /*000066f0*/ POP         {R4, PC} 
FUNC_END STR_SameString


FUNC_BEGIN STR_SkipSeperator
    /*000066f4*/ LDRB        R3, [R0] 
    /*000066f8*/ CMP         R3, #10 
    /*000066fc*/ CMPNE       R3, #0 
    /*00006700*/ MOVEQ       IP, #1 
    /*00006704*/ MOVNE       IP, #0 
    /*00006708*/ BEQ         STR_SkipSeperator_x6778
    /*0000670c*/ CMP         R3, #32 
    /*00006710*/ CMPNE       R3, #44                     @ 0x2c 
    /*00006714*/ MOVNE       R2, #1 
    /*00006718*/ MOVEQ       R2, #0 
    /*0000671c*/ CMP         R3, #9 
    /*00006720*/ MOVEQ       R3, #0 
    /*00006724*/ ANDNE       R3, R2, #1 
    /*00006728*/ CMP         R3, #0 
    /*0000672c*/ BNE         STR_SkipSeperator_x6770
    /*00006730*/ ADD         R1, R0, #1 
    /*00006734*/ B           STR_SkipSeperator_x675c
    STR_SkipSeperator_x6738:
    /*00006738*/ CMP         R3, #32 
    /*0000673c*/ CMPNE       R3, #44                     @ 0x2c 
    /*00006740*/ MOVNE       R2, #1 
    /*00006744*/ MOVEQ       R2, #0 
    /*00006748*/ CMP         R3, #9 
    /*0000674c*/ MOVEQ       R3, #0 
    /*00006750*/ ANDNE       R3, R2, #1 
    /*00006754*/ CMP         R3, #0 
    /*00006758*/ BNE         STR_SkipSeperator_x6770
    STR_SkipSeperator_x675c:
    /*0000675c*/ SUB         IP, R1, R0 
    /*00006760*/ LDRB        R3, [R1], #1 
    /*00006764*/ CMP         R3, #10 
    /*00006768*/ CMPNE       R3, #0 
    /*0000676c*/ BNE         STR_SkipSeperator_x6738
    STR_SkipSeperator_x6770:
    /*00006770*/ MOV         R0, IP 
    /*00006774*/ BX          LR 
    STR_SkipSeperator_x6778:
    /*00006778*/ MOV         IP, #0 
    /*0000677c*/ B           STR_SkipSeperator_x6770
FUNC_END STR_SkipSeperator


FUNC_BEGIN STR_ToNextLine
    /*00006780*/ LDRB        R3, [R0] 
    /*00006784*/ MOV         R1, R0 
    /*00006788*/ CMP         R3, #0 
    /*0000678c*/ CMPNE       R3, #10 
    /*00006790*/ BEQ         STR_ToNextLine_x67b0
    /*00006794*/ ADD         R3, R0, #1 
    STR_ToNextLine_x6798:
    /*00006798*/ SUB         R0, R3, R1 
    /*0000679c*/ LDRB        R2, [R3], #1 
    /*000067a0*/ CMP         R2, #0 
    /*000067a4*/ CMPNE       R2, #10 
    /*000067a8*/ BNE         STR_ToNextLine_x6798
    /*000067ac*/ BX          LR 
    STR_ToNextLine_x67b0:
    /*000067b0*/ MOV         R0, #0 
    /*000067b4*/ BX          LR 
FUNC_END STR_ToNextLine


FUNC_BEGIN STR_ToNextSeperator
    /*000067b8*/ LDRB        R3, [R0] 
    /*000067bc*/ ANDS        IP, R3, #223                @ 0xdf 
    /*000067c0*/ BEQ         STR_ToNextSeperator_x67fc
    /*000067c4*/ SUB         R2, R3, #9 
    /*000067c8*/ CMP         R3, #44                     @ 0x2c 
    /*000067cc*/ CMPNE       R2, #1 
    /*000067d0*/ BLS         STR_ToNextSeperator_x6804
    /*000067d4*/ ADD         R2, R0, #1 
    /*000067d8*/ B           STR_ToNextSeperator_x67e8
    STR_ToNextSeperator_x67dc:
    /*000067dc*/ CMP         R3, #44                     @ 0x2c 
    /*000067e0*/ CMPNE       R1, #1 
    /*000067e4*/ BLS         STR_ToNextSeperator_x67fc
    STR_ToNextSeperator_x67e8:
    /*000067e8*/ SUB         IP, R2, R0 
    /*000067ec*/ LDRB        R3, [R2], #1 
    /*000067f0*/ TST         R3, #223                    @ 0xdf 
    /*000067f4*/ SUB         R1, R3, #9 
    /*000067f8*/ BNE         STR_ToNextSeperator_x67dc
    STR_ToNextSeperator_x67fc:
    /*000067fc*/ MOV         R0, IP 
    /*00006800*/ BX          LR 
    STR_ToNextSeperator_x6804:
    /*00006804*/ MOV         IP, #0 
    /*00006808*/ B           STR_ToNextSeperator_x67fc
FUNC_END STR_ToNextSeperator


FUNC_BEGIN STR_GetOneString
    /*0000680c*/ LDRB        R3, [R0] 
    /*00006810*/ CMP         R3, #44                     @ 0x2c 
    /*00006814*/ BEQ         STR_GetOneString_x688c
    /*00006818*/ TST         R3, #223                    @ 0xdf 
    /*0000681c*/ SUB         R2, R3, #9 
    /*00006820*/ MOVEQ       IP, #1 
    /*00006824*/ MOVNE       IP, #0 
    /*00006828*/ CMP         R2, #1 
    /*0000682c*/ MOVHI       R2, IP 
    /*00006830*/ ORRLS       R2, IP, #1 
    /*00006834*/ CMP         R2, #0 
    /*00006838*/ BNE         STR_GetOneString_x688c
    /*0000683c*/ PUSH        {R4, LR} 
    /*00006840*/ MOV         IP, R1 
    /*00006844*/ MOV         LR, R0 
    /*00006848*/ B           STR_GetOneString_x6868
    STR_GetOneString_x684c:
    /*0000684c*/ TST         R3, #223                    @ 0xdf 
    /*00006850*/ MOVEQ       R2, #1 
    /*00006854*/ MOVNE       R2, #0 
    /*00006858*/ CMP         R4, #1 
    /*0000685c*/ ORRLS       R2, R2, #1 
    /*00006860*/ CMP         R2, #0 
    /*00006864*/ BNE         STR_GetOneString_x6880
    STR_GetOneString_x6868:
    /*00006868*/ STRB        R3, [IP], #1 
    /*0000686c*/ SUB         R0, IP, R1 
    /*00006870*/ LDRB        R3, [LR, #1]! 
    /*00006874*/ CMP         R3, #44                     @ 0x2c 
    /*00006878*/ SUB         R4, R3, #9 
    /*0000687c*/ BNE         STR_GetOneString_x684c
    STR_GetOneString_x6880:
    /*00006880*/ MOV         R3, #0 
    /*00006884*/ STRB        R3, [IP] 
    /*00006888*/ POP         {R4, PC} 
    STR_GetOneString_x688c:
    /*0000688c*/ MOV         R3, #0 
    /*00006890*/ MOV         R0, #0 
    /*00006894*/ STRB        R3, [R1] 
    /*00006898*/ BX          LR 
FUNC_END STR_GetOneString


FUNC_BEGIN STR_GetOneInteger
    /*0000689c*/ PUSH        {R4, R5, R6, LR} 
    /*000068a0*/ MOV         R5, R1 
    /*000068a4*/ LDRB        R3, [R0] 
    /*000068a8*/ SUB         SP, SP, #16 
    /*000068ac*/ CMP         R3, #44                     @ 0x2c 
    /*000068b0*/ BEQ         STR_GetOneInteger_x6944
    /*000068b4*/ TST         R3, #223                    @ 0xdf 
    /*000068b8*/ SUB         R4, R3, #9 
    /*000068bc*/ MOV         R6, SP 
    /*000068c0*/ MOVEQ       R2, #1 
    /*000068c4*/ MOVNE       R2, #0 
    /*000068c8*/ CMP         R4, #1 
    /*000068cc*/ MOVHI       R4, R2 
    /*000068d0*/ ORRLS       R4, R2, #1 
    /*000068d4*/ CMP         R4, #0 
    /*000068d8*/ BNE         STR_GetOneInteger_x6954
    /*000068dc*/ MOV         R1, R0 
    /*000068e0*/ MOV         IP, SP 
    /*000068e4*/ B           STR_GetOneInteger_x6904
    STR_GetOneInteger_x68e8:
    /*000068e8*/ TST         R3, #223                    @ 0xdf 
    /*000068ec*/ MOVEQ       R2, #1 
    /*000068f0*/ MOVNE       R2, #0 
    /*000068f4*/ CMP         LR, #1 
    /*000068f8*/ ORRLS       R2, R2, #1 
    /*000068fc*/ CMP         R2, #0 
    /*00006900*/ BNE         STR_GetOneInteger_x691c
    STR_GetOneInteger_x6904:
    /*00006904*/ STRB        R3, [IP], #1 
    /*00006908*/ ADD         R4, R4, #1 
    /*0000690c*/ LDRB        R3, [R1, #1]! 
    /*00006910*/ CMP         R3, #44                     @ 0x2c 
    /*00006914*/ SUB         LR, R3, #9 
    /*00006918*/ BNE         STR_GetOneInteger_x68e8
    STR_GetOneInteger_x691c:
    /*0000691c*/ MOV         R3, #0 
    /*00006920*/ MOV         R0, R6 
    /*00006924*/ MOV         R1, R3 
    /*00006928*/ MOV         R2, #10 
    /*0000692c*/ STRB        R3, [IP] 
    /*00006930*/ BL          0x0000000000006930          @ osal_strtol [R_ARM_CALL: +0x0] 
    /*00006934*/ STR         R0, [R5] 
    /*00006938*/ MOV         R0, R4 
    /*0000693c*/ ADD         SP, SP, #16 
    /*00006940*/ POP         {R4, R5, R6, PC} 
    STR_GetOneInteger_x6944:
    /*00006944*/ MOV         R6, SP 
    /*00006948*/ MOV         R4, #0 
    /*0000694c*/ MOV         IP, SP 
    /*00006950*/ B           STR_GetOneInteger_x691c
    STR_GetOneInteger_x6954:
    /*00006954*/ MOV         R4, #0 
    /*00006958*/ MOV         IP, SP 
    /*0000695c*/ B           STR_GetOneInteger_x691c
FUNC_END STR_GetOneInteger


FUNC_BEGIN JPEGE_DRV_Init
    /*00006960*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00006964*/ MOV         R0, #25 
    /*00006968*/ SUB         SP, SP, #12 
    /*0000696c*/ BL          0x000000000000696C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00006970*/ CMP         R0, #0 
    /*00006974*/ MOVNE       R0, #0 
    /*00006978*/ BEQ         JPEGE_DRV_Init_x6984
    JPEGE_DRV_Init_x697c:
    /*0000697c*/ ADD         SP, SP, #12 
    /*00006980*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    JPEGE_DRV_Init_x6984:
    /*00006984*/ MOVW        R2, #533                    @ 0x215 
    /*00006988*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x9c4] 
    /*0000698c*/ STR         R2, [SP] 
    /*00006990*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x9c4] 
    /*00006994*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a20] 
    /*00006998*/ MOV         R1, #4 
    /*0000699c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a20] 
    /*000069a0*/ MOV         R0, #3 
    /*000069a4*/ BL          0x00000000000069A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000069a8*/ MVN         R0, #0 
    /*000069ac*/ B           JPEGE_DRV_Init_x697c
FUNC_END JPEGE_DRV_Init


FUNC_BEGIN JPEGE_DRV_Exit
    /*000069b0*/ BX          LR 
FUNC_END JPEGE_DRV_Exit


FUNC_BEGIN JPEGE_DRV_GetCmpSegCfg
    /*000069b4*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000069b8*/ MOV         R4, R2 
    /*000069bc*/ SUB         SP, SP, #52                 @ 0x34 
    /*000069c0*/ MOV         R5, R1 
    /*000069c4*/ BL          0x00000000000069C4          @ JpegeGetChnCtxPtr [R_ARM_CALL: .text+0x4790] 
    /*000069c8*/ LDR         R3, [R4, #20] 
    /*000069cc*/ CMP         R3, #1 
    /*000069d0*/ BEQ         JPEGE_DRV_GetCmpSegCfg_x69f4
    /*000069d4*/ MOV         R3, #0 
    /*000069d8*/ STR         R3, [R5, #2896]             @ 0xb50 
    /*000069dc*/ STR         R3, [R5, #2900]             @ 0xb54 
    /*000069e0*/ STR         R3, [R5, #2904]             @ 0xb58 
    /*000069e4*/ LDR         R3, [R4, #20] 
    /*000069e8*/ STR         R3, [R5, #2892]             @ 0xb4c 
    /*000069ec*/ ADD         SP, SP, #52                 @ 0x34 
    /*000069f0*/ POP         {R4, R5, R6, R7, PC} 
    JPEGE_DRV_GetCmpSegCfg_x69f4:
    /*000069f4*/ LDR         R2, [R4, #24] 
    /*000069f8*/ MOV         R7, #0 
    /*000069fc*/ MOV         R6, R0 
    /*00006a00*/ STR         R7, [SP, #36]               @ 0x24 
    /*00006a04*/ SUB         R1, R2, #1 
    /*00006a08*/ STR         R7, [SP, #40]               @ 0x28 
    /*00006a0c*/ CMP         R1, #1 
    /*00006a10*/ STRLS       R3, [SP, #20] 
    /*00006a14*/ BLS         JPEGE_DRV_GetCmpSegCfg_x6a24
    /*00006a18*/ CMP         R2, #0 
    /*00006a1c*/ STREQ       R2, [SP, #20] 
    /*00006a20*/ BNE         JPEGE_DRV_GetCmpSegCfg_x6af0
    JPEGE_DRV_GetCmpSegCfg_x6a24:
    /*00006a24*/ LDM         R4, {R1, R2} 
    /*00006a28*/ MOV         R0, #2 
    /*00006a2c*/ LDR         LR, [R4, #332]              @ 0x14c 
    /*00006a30*/ MOV         R7, #0 
    /*00006a34*/ LDR         IP, [R4, #12] 
    /*00006a38*/ STR         R3, [SP, #16] 
    /*00006a3c*/ STR         LR, [SP, #8] 
    /*00006a40*/ STR         IP, [SP, #12] 
    /*00006a44*/ STR         R1, [SP, #28] 
    /*00006a48*/ STR         R2, [SP, #32] 
    /*00006a4c*/ STR         R7, [SP, #44]               @ 0x2c 
    /*00006a50*/ BL          0x0000000000006A50          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00006a54*/ ADD         R2, R6, #7552               @ 0x1d80 
    /*00006a58*/ ADD         R1, R6, #7360               @ 0x1cc0 
    /*00006a5c*/ ADD         R2, R2, #28 
    /*00006a60*/ ADD         R1, R1, #4 
    /*00006a64*/ LDR         R3, [R0, #128]              @ 0x80 
    /*00006a68*/ ADD         R0, SP, #8 
    /*00006a6c*/ BLX         R3 
    /*00006a70*/ LDR         R3, [R4, #20] 
    /*00006a74*/ STR         R3, [R5, #2892]             @ 0xb4c 
    /*00006a78*/ LDR         R3, [R4, #32] 
    /*00006a7c*/ CMP         R3, #16 
    /*00006a80*/ STREQ       R7, [R5, #2904]             @ 0xb58 
    /*00006a84*/ BEQ         JPEGE_DRV_GetCmpSegCfg_x6adc
    /*00006a88*/ CMP         R3, #32 
    /*00006a8c*/ MOVEQ       R3, #1 
    /*00006a90*/ STREQ       R3, [R5, #2904]             @ 0xb58 
    /*00006a94*/ BEQ         JPEGE_DRV_GetCmpSegCfg_x6adc
    /*00006a98*/ CMP         R3, #48                     @ 0x30 
    /*00006a9c*/ MOVEQ       R3, #2 
    /*00006aa0*/ STREQ       R3, [R5, #2904]             @ 0xb58 
    /*00006aa4*/ BEQ         JPEGE_DRV_GetCmpSegCfg_x6adc
    /*00006aa8*/ CMP         R3, #64                     @ 0x40 
    /*00006aac*/ BEQ         JPEGE_DRV_GetCmpSegCfg_x6ad4
    /*00006ab0*/ STR         R3, [SP, #4] 
    /*00006ab4*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1aac] 
    /*00006ab8*/ MOVW        R3, #614                    @ 0x266 
    /*00006abc*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1aac] 
    /*00006ac0*/ STR         R3, [SP] 
    /*00006ac4*/ MOV         R1, #11 
    /*00006ac8*/ LDR         R3, [PC, #80]               @ 0x0000000000006b20 
    /*00006acc*/ MOV         R0, #3 
    /*00006ad0*/ BL          0x0000000000006AD0          @ HI_LOG [R_ARM_CALL: +0x0] 
    JPEGE_DRV_GetCmpSegCfg_x6ad4:
    /*00006ad4*/ MOV         R3, #3 
    /*00006ad8*/ STR         R3, [R5, #2904]             @ 0xb58 
    JPEGE_DRV_GetCmpSegCfg_x6adc:
    /*00006adc*/ ADD         R6, R6, #4096               @ 0x1000 
    /*00006ae0*/ LDR         R3, [R6, #3276]             @ 0xccc 
    /*00006ae4*/ STR         R3, [R5, #2896]             @ 0xb50 
    /*00006ae8*/ ADD         SP, SP, #52                 @ 0x34 
    /*00006aec*/ POP         {R4, R5, R6, R7, PC} 
    JPEGE_DRV_GetCmpSegCfg_x6af0:
    /*00006af0*/ STR         R2, [SP, #4] 
    /*00006af4*/ MOV         R3, #584                    @ 0x248 
    /*00006af8*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a6c] 
    /*00006afc*/ STR         R3, [SP] 
    /*00006b00*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a6c] 
    /*00006b04*/ LDR         R3, [PC, #20]               @ 0x0000000000006b20 
    /*00006b08*/ MOV         R1, #11 
    /*00006b0c*/ MOV         R0, #3 
    /*00006b10*/ BL          0x0000000000006B10          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006b14*/ LDR         R3, [R4, #20] 
    /*00006b18*/ STR         R7, [SP, #20] 
    /*00006b1c*/ B           JPEGE_DRV_GetCmpSegCfg_x6a24
    /*00006b20*/ LDRDEQ      R0, [R0], -R4 
FUNC_END JPEGE_DRV_GetCmpSegCfg


FUNC_BEGIN JPEGE_DRV_Create
    /*00006b24*/ PUSH        {R4, LR} 
    /*00006b28*/ MOV         R2, #8 
    /*00006b2c*/ MOV         R4, R0 
    /*00006b30*/ BL          0x0000000000006B30          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00006b34*/ MOV         R3, #0 
    /*00006b38*/ STR         R3, [R4, #12] 
    /*00006b3c*/ MOV         R0, R3 
    /*00006b40*/ POP         {R4, PC} 
FUNC_END JPEGE_DRV_Create


FUNC_BEGIN JPEGE_DRV_Destroy
    /*00006b44*/ BX          LR 
FUNC_END JPEGE_DRV_Destroy


FUNC_BEGIN JPEGE_DRV_SetQt
    /*00006b48*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00006b4c*/ SUB         SP, SP, #260                @ 0x104 
    /*00006b50*/ SUB         R6, SP, #4 
    /*00006b54*/ MOV         R7, R0 
    /*00006b58*/ MOV         R5, R1 
    /*00006b5c*/ MOV         R0, SP 
    /*00006b60*/ ADD         R1, R7, #352                @ 0x160 
    /*00006b64*/ MOV         R2, #1 
    /*00006b68*/ BL          0x0000000000006B68          @ Jpge_init_quant_table [R_ARM_CALL: .text+0x6048] 
    /*00006b6c*/ MOV         R4, #0 
    /*00006b70*/ MOV         R3, R6 
    JPEGE_DRV_SetQt_x6b74:
    /*00006b74*/ ADD         R2, R4, #512                @ 0x200 
    /*00006b78*/ LDR         R1, [R3, #4]! 
    /*00006b7c*/ ADD         R4, R4, #1 
    /*00006b80*/ CMP         R4, #64                     @ 0x40 
    /*00006b84*/ STR         R1, [R5, R2, LSL #2] 
    /*00006b88*/ BNE         JPEGE_DRV_SetQt_x6b74
    /*00006b8c*/ MOV         R2, #1 
    /*00006b90*/ ADD         R1, R7, #416                @ 0x1a0 
    /*00006b94*/ MOV         R0, SP 
    /*00006b98*/ BL          0x0000000000006B98          @ Jpge_init_quant_table [R_ARM_CALL: .text+0x6048] 
    /*00006b9c*/ MOV         R3, R6 
    JPEGE_DRV_SetQt_x6ba0:
    /*00006ba0*/ ADD         R2, R4, #512                @ 0x200 
    /*00006ba4*/ LDR         R1, [R3, #4]! 
    /*00006ba8*/ ADD         R4, R4, #1 
    /*00006bac*/ CMP         R4, #128                    @ 0x80 
    /*00006bb0*/ STR         R1, [R5, R2, LSL #2] 
    /*00006bb4*/ BNE         JPEGE_DRV_SetQt_x6ba0
    /*00006bb8*/ ADD         R1, R7, #480                @ 0x1e0 
    /*00006bbc*/ MOV         R0, SP 
    /*00006bc0*/ MOV         R2, #1 
    /*00006bc4*/ BL          0x0000000000006BC4          @ Jpge_init_quant_table [R_ARM_CALL: .text+0x6048] 
    JPEGE_DRV_SetQt_x6bc8:
    /*00006bc8*/ ADD         R3, R4, #512                @ 0x200 
    /*00006bcc*/ LDR         R2, [R6, #4]! 
    /*00006bd0*/ ADD         R4, R4, #1 
    /*00006bd4*/ CMP         R4, #192                    @ 0xc0 
    /*00006bd8*/ STR         R2, [R5, R3, LSL #2] 
    /*00006bdc*/ BNE         JPEGE_DRV_SetQt_x6bc8
    /*00006be0*/ ADD         SP, SP, #260                @ 0x104 
    /*00006be4*/ POP         {R4, R5, R6, R7, PC} 
FUNC_END JPEGE_DRV_SetQt


FUNC_BEGIN JPEGE_DRV_ReadRegs
    /*00006be8*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00006bec*/ MOV         R2, R0 
    /*00006bf0*/ LDR         IP, [R1] 
    /*00006bf4*/ MOV         R0, #0 
    /*00006bf8*/ LDR         LR, [R2, #12] 
    /*00006bfc*/ AND         IP, IP, #1 
    /*00006c00*/ STR         IP, [R2] 
    /*00006c04*/ LDR         IP, [R1] 
    /*00006c08*/ UBFX        IP, IP, #1, #1 
    /*00006c0c*/ STR         IP, [R2, #4] 
    /*00006c10*/ LDR         R3, [R1, #8] 
    /*00006c14*/ UBFX        R3, R3, #3, #1 
    /*00006c18*/ ORR         R3, R3, LR 
    /*00006c1c*/ STR         R3, [R2, #12] 
    /*00006c20*/ LDR         R1, [R1, #8] 
    /*00006c24*/ UBFX        R1, R1, #2, #1 
    /*00006c28*/ ORR         R3, R1, R3 
    /*00006c2c*/ STR         R3, [R2, #12] 
    /*00006c30*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END JPEGE_DRV_ReadRegs


FUNC_BEGIN JPEGE_DRV_GetOsdInfoByAttachDest
    /*00006c34*/ LDR         IP, [R1, #16] 
    /*00006c38*/ CMP         IP, #0 
    /*00006c3c*/ BXEQ        LR 
    /*00006c40*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00006c44*/ SUB         SP, SP, #20 
    /*00006c48*/ MOV         R7, #0 
    /*00006c4c*/ MOVW        R3, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ae0] 
    /*00006c50*/ MOV         R6, R2 
    /*00006c54*/ MOVT        R3, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ae0] 
    /*00006c58*/ MOV         R8, R0 
    /*00006c5c*/ MOV         R9, R1 
    /*00006c60*/ ADD         R4, R1, #80                 @ 0x50 
    /*00006c64*/ STR         R3, [SP, #12] 
    /*00006c68*/ MOV         R5, R7 
    /*00006c6c*/ B           JPEGE_DRV_GetOsdInfoByAttachDest_x6c80
    JPEGE_DRV_GetOsdInfoByAttachDest_x6c70:
    /*00006c70*/ ADD         R5, R5, #1 
    /*00006c74*/ ADD         R4, R4, #96                 @ 0x60 
    /*00006c78*/ CMP         IP, R5 
    /*00006c7c*/ BLS         JPEGE_DRV_GetOsdInfoByAttachDest_x6d88
    JPEGE_DRV_GetOsdInfoByAttachDest_x6c80:
    /*00006c80*/ LDR         R3, [R4, #20] 
    /*00006c84*/ CMP         R3, R8 
    /*00006c88*/ BNE         JPEGE_DRV_GetOsdInfoByAttachDest_x6c70
    /*00006c8c*/ MOV         LR, #96                     @ 0x60 
    /*00006c90*/ ADD         SL, R7, #1 
    /*00006c94*/ MUL         R0, LR, R7 
    /*00006c98*/ CMP         R7, #7 
    /*00006c9c*/ MOV         R2, #20 
    /*00006ca0*/ MOV         R1, R4 
    /*00006ca4*/ MLA         LR, LR, SL, R6 
    /*00006ca8*/ ADD         IP, R6, R0 
    /*00006cac*/ ADD         R0, R0, #80                 @ 0x50 
    /*00006cb0*/ MOVW        FP, #977                    @ 0x3d1 
    /*00006cb4*/ ADD         R0, R6, R0 
    /*00006cb8*/ LDR         R3, [PC, #244]              @ 0x0000000000006db4 
    /*00006cbc*/ BGT         JPEGE_DRV_GetOsdInfoByAttachDest_x6d90
    /*00006cc0*/ LDR         R3, [R4, #36]               @ 0x24 
    /*00006cc4*/ MOV         R7, SL 
    /*00006cc8*/ ADD         R5, R5, #1 
    /*00006ccc*/ ADD         R4, R4, #96                 @ 0x60 
    /*00006cd0*/ STR         R3, [IP, #116]              @ 0x74 
    /*00006cd4*/ LDR         R3, [R4, #-152]             @ 0xffffff68 
    /*00006cd8*/ STR         R3, [IP, #24] 
    /*00006cdc*/ LDR         R3, [R4, #-148]             @ 0xffffff6c 
    /*00006ce0*/ STR         R3, [IP, #28] 
    /*00006ce4*/ LDR         R3, [R4, #-144]             @ 0xffffff70 
    /*00006ce8*/ STR         R3, [IP, #32] 
    /*00006cec*/ LDR         R3, [R4, #-140]             @ 0xffffff74 
    /*00006cf0*/ STR         R3, [IP, #36]               @ 0x24 
    /*00006cf4*/ LDR         R3, [R4, #-124]             @ 0xffffff84 
    /*00006cf8*/ STR         R3, [IP, #52]               @ 0x34 
    /*00006cfc*/ LDR         R3, [R4, #-128]             @ 0xffffff80 
    /*00006d00*/ STR         R3, [IP, #48]               @ 0x30 
    /*00006d04*/ LDR         R3, [R4, #-132]             @ 0xffffff7c 
    /*00006d08*/ STR         R3, [IP, #44]               @ 0x2c 
    /*00006d0c*/ LDR         R3, [R4, #-136]             @ 0xffffff78 
    /*00006d10*/ STR         R3, [IP, #40]               @ 0x28 
    /*00006d14*/ LDRD        SL, [R4, #-120]             @ 0xffffff88 
    /*00006d18*/ STRD        SL, [IP, #56]               @ 0x38 
    /*00006d1c*/ LDR         R3, [R4, #-112]             @ 0xffffff90 
    /*00006d20*/ STR         R3, [IP, #64]               @ 0x40 
    /*00006d24*/ LDR         R3, [R4, #-108]             @ 0xffffff94 
    /*00006d28*/ STR         R3, [IP, #68]               @ 0x44 
    /*00006d2c*/ LDR         R3, [R4, #-104]             @ 0xffffff98 
    /*00006d30*/ STR         R3, [IP, #72]               @ 0x48 
    /*00006d34*/ LDR         R3, [R4, #-100]             @ 0xffffff9c 
    /*00006d38*/ STR         R3, [IP, #76]               @ 0x4c 
    /*00006d3c*/ LDR         R3, [R4, #-76]              @ 0xffffffb4 
    /*00006d40*/ STR         R3, [LR, #4] 
    /*00006d44*/ LDR         R3, [R4, #-64]              @ 0xffffffc0 
    /*00006d48*/ STR         R3, [IP, #112]              @ 0x70 
    /*00006d4c*/ LDR         R3, [R4, #-68]              @ 0xffffffbc 
    /*00006d50*/ STR         R3, [IP, #108]              @ 0x6c 
    /*00006d54*/ LDRH        R3, [R4, #-72]              @ 0xffffffb8 
    /*00006d58*/ STRH        R3, [IP, #104]              @ 0x68 
    /*00006d5c*/ LDRH        R3, [R4, #-70]              @ 0xffffffba 
    /*00006d60*/ STRH        R3, [IP, #106]              @ 0x6a 
    /*00006d64*/ BL          0x0000000000006D64          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00006d68*/ LDR         R3, [R9, #4] 
    /*00006d6c*/ STR         R3, [R6, #4] 
    /*00006d70*/ LDR         R3, [R9] 
    /*00006d74*/ STR         R7, [R6, #16] 
    /*00006d78*/ STR         R3, [R6] 
    /*00006d7c*/ LDR         IP, [R9, #16] 
    /*00006d80*/ CMP         IP, R5 
    /*00006d84*/ BHI         JPEGE_DRV_GetOsdInfoByAttachDest_x6c80
    JPEGE_DRV_GetOsdInfoByAttachDest_x6d88:
    /*00006d88*/ ADD         SP, SP, #20 
    /*00006d8c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    JPEGE_DRV_GetOsdInfoByAttachDest_x6d90:
    /*00006d90*/ MOV         R2, #8 
    /*00006d94*/ STR         FP, [SP] 
    /*00006d98*/ STR         R2, [SP, #4] 
    /*00006d9c*/ MOV         R1, #8 
    /*00006da0*/ LDR         R2, [SP, #12] 
    /*00006da4*/ MOV         R0, #3 
    /*00006da8*/ BL          0x0000000000006DA8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006dac*/ LDR         IP, [R9, #16] 
    /*00006db0*/ B           JPEGE_DRV_GetOsdInfoByAttachDest_x6c70
    /*00006db4*/ ANDEQ       R0, R0, IP, ROR #19 
FUNC_END JPEGE_DRV_GetOsdInfoByAttachDest


FUNC_BEGIN JPEGE_DRV_GetOsdInfo
    /*00006db8*/ SUBS        IP, R0, #0 
    /*00006dbc*/ MOV         R3, #0 
    /*00006dc0*/ MOV         R0, R2 
    /*00006dc4*/ STR         R3, [R1, #16] 
    /*00006dc8*/ STR         R3, [R1, #4] 
    /*00006dcc*/ STR         R3, [R1] 
    /*00006dd0*/ BXEQ        LR 
    /*00006dd4*/ CMP         R2, #1 
    /*00006dd8*/ BEQ         JPEGE_DRV_GetOsdInfo_x6de4
    /*00006ddc*/ CMP         R2, #2 
    /*00006de0*/ BNE         JPEGE_DRV_GetOsdInfo_x6df0
    JPEGE_DRV_GetOsdInfo_x6de4:
    /*00006de4*/ MOV         R2, R1 
    /*00006de8*/ MOV         R1, IP 
    JPEGE_DRV_GetOsdInfo_x6dec:
    /*00006dec*/ B           JPEGE_DRV_GetOsdInfo_x6dec  @ JPEGE_DRV_GetOsdInfoByAttachDest [R_ARM_JUMP24: .text+0x6c34] 
    JPEGE_DRV_GetOsdInfo_x6df0:
    /*00006df0*/ MOV         R2, R1 
    /*00006df4*/ MOV         R0, R3 
    /*00006df8*/ MOV         R1, IP 
    JPEGE_DRV_GetOsdInfo_x6dfc:
    /*00006dfc*/ B           JPEGE_DRV_GetOsdInfo_x6dfc  @ JPEGE_DRV_GetOsdInfoByAttachDest [R_ARM_JUMP24: .text+0x6c34] 
FUNC_END JPEGE_DRV_GetOsdInfo


FUNC_BEGIN JPEGE_DRV_SetOsd
    /*00006e00*/ CMP         R1, #0 
    /*00006e04*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00006e08*/ MOV         R4, R0 
    /*00006e0c*/ SUB         SP, SP, #84                 @ 0x54 
    /*00006e10*/ LDR         R6, [R0, #48]               @ 0x30 
    /*00006e14*/ BEQ         JPEGE_DRV_SetOsd_x71d4
    /*00006e18*/ MOV         R5, R3 
    /*00006e1c*/ LDR         R3, [R3, #16] 
    /*00006e20*/ CMP         R3, #0 
    /*00006e24*/ BEQ         JPEGE_DRV_SetOsd_x71d4
    /*00006e28*/ CMP         R2, #1 
    /*00006e2c*/ BEQ         JPEGE_DRV_SetOsd_x71b0
    JPEGE_DRV_SetOsd_x6e30:
    /*00006e30*/ LDR         R3, [R5, #4] 
    /*00006e34*/ CMP         R3, #0 
    /*00006e38*/ BEQ         JPEGE_DRV_SetOsd_x71d4
    /*00006e3c*/ ORR         R6, R6, #4 
    /*00006e40*/ STR         R6, [R4, #48]               @ 0x30 
    /*00006e44*/ LDR         IP, [R5, #16] 
    /*00006e48*/ MOV         R3, #0 
    /*00006e4c*/ ADD         R1, SP, #12 
    /*00006e50*/ ADD         R2, SP, #44                 @ 0x2c 
    /*00006e54*/ MOV         R7, R3 
    JPEGE_DRV_SetOsd_x6e58:
    /*00006e58*/ CMP         IP, R3 
    /*00006e5c*/ ADD         R3, R3, #1 
    /*00006e60*/ STR         R7, [R2, #4]! 
    /*00006e64*/ MOVLS       R0, #0 
    /*00006e68*/ MOVHI       R0, #1 
    /*00006e6c*/ CMP         R3, #8 
    /*00006e70*/ STR         R0, [R1, #4]! 
    /*00006e74*/ BNE         JPEGE_DRV_SetOsd_x6e58
    /*00006e78*/ LDR         R3, [R5, #116]              @ 0x74 
    /*00006e7c*/ LDR         R6, [R4, #256]              @ 0x100 
    /*00006e80*/ CMP         R3, #8 
    /*00006e84*/ BEQ         JPEGE_DRV_SetOsd_x6ed8
    /*00006e88*/ CMP         R3, #9 
    /*00006e8c*/ MOVEQ       R3, #1 
    /*00006e90*/ BFIEQ       R6, R3, #0, #2 
    /*00006e94*/ BEQ         JPEGE_DRV_SetOsd_x6edc
    /*00006e98*/ CMP         R3, #11 
    /*00006e9c*/ MOVEQ       R3, #2 
    /*00006ea0*/ BFIEQ       R6, R3, #0, #2 
    /*00006ea4*/ BEQ         JPEGE_DRV_SetOsd_x6edc
    /*00006ea8*/ CMP         R3, #12 
    /*00006eac*/ ORREQ       R6, R6, #3 
    /*00006eb0*/ BEQ         JPEGE_DRV_SetOsd_x6edc
    /*00006eb4*/ STR         R3, [SP, #4] 
    /*00006eb8*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b34] 
    /*00006ebc*/ MOVW        R3, #1074                   @ 0x432 
    /*00006ec0*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b34] 
    /*00006ec4*/ STR         R3, [SP] 
    /*00006ec8*/ MOV         R1, #11 
    /*00006ecc*/ LDR         R3, [PC, #784]              @ 0x00000000000071e4 
    /*00006ed0*/ MOV         R0, #3 
    /*00006ed4*/ BL          0x0000000000006ED4          @ HI_LOG [R_ARM_CALL: +0x0] 
    JPEGE_DRV_SetOsd_x6ed8:
    /*00006ed8*/ BFI         R6, R7, #0, #2 
    JPEGE_DRV_SetOsd_x6edc:
    /*00006edc*/ LDR         R2, [SP, #16] 
    /*00006ee0*/ MOV         R1, R5 
    /*00006ee4*/ LDR         R3, [SP, #20] 
    /*00006ee8*/ MOV         R0, R5 
    /*00006eec*/ BFI         R6, R2, #24, #1 
    /*00006ef0*/ LDR         R2, [SP, #24] 
    /*00006ef4*/ BFI         R6, R3, #25, #1 
    /*00006ef8*/ LDR         R3, [SP, #28] 
    /*00006efc*/ BFI         R6, R2, #26, #1 
    /*00006f00*/ LDR         R2, [SP, #32] 
    /*00006f04*/ BFI         R6, R3, #27, #1 
    /*00006f08*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00006f0c*/ BFI         R6, R2, #28, #1 
    /*00006f10*/ LDR         R2, [SP, #40]               @ 0x28 
    /*00006f14*/ BFI         R6, R3, #29, #1 
    /*00006f18*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*00006f1c*/ BFI         R6, R2, #30, #1 
    /*00006f20*/ LDR         R2, [SP, #48]               @ 0x30 
    /*00006f24*/ BFI         R6, R3, #31, #1 
    /*00006f28*/ LDR         R3, [SP, #52]               @ 0x34 
    /*00006f2c*/ BFI         R6, R2, #16, #1 
    /*00006f30*/ LDR         R2, [SP, #56]               @ 0x38 
    /*00006f34*/ BFI         R6, R3, #17, #1 
    /*00006f38*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*00006f3c*/ BFI         R6, R2, #18, #1 
    /*00006f40*/ LDR         R2, [SP, #64]               @ 0x40 
    /*00006f44*/ BFI         R6, R3, #19, #1 
    /*00006f48*/ LDR         R3, [SP, #68]               @ 0x44 
    /*00006f4c*/ BFI         R6, R2, #20, #1 
    /*00006f50*/ LDR         R2, [SP, #72]               @ 0x48 
    /*00006f54*/ BFI         R6, R3, #21, #1 
    /*00006f58*/ LDR         R3, [SP, #76]               @ 0x4c 
    /*00006f5c*/ BFI         R6, R2, #22, #1 
    /*00006f60*/ MOV         R2, #0 
    /*00006f64*/ BFI         R6, R3, #23, #1 
    /*00006f68*/ STR         R6, [R4, #256]              @ 0x100 
    /*00006f6c*/ LDR         R3, [R4, #260]              @ 0x104 
    /*00006f70*/ LDR         LR, [R5, #52]               @ 0x34 
    /*00006f74*/ LDR         IP, [R5, #48]               @ 0x30 
    /*00006f78*/ BFI         R3, LR, #0, #8 
    /*00006f7c*/ LDR         LR, [R5, #148]              @ 0x94 
    /*00006f80*/ BFI         R3, IP, #8, #8 
    /*00006f84*/ LDR         IP, [R5, #144]              @ 0x90 
    /*00006f88*/ BFI         R3, LR, #16, #8 
    /*00006f8c*/ BFI         R3, IP, #24, #8 
    /*00006f90*/ STR         R3, [R4, #260]              @ 0x104 
    /*00006f94*/ LDR         R3, [R4, #264]              @ 0x108 
    /*00006f98*/ LDR         LR, [R5, #244]              @ 0xf4 
    /*00006f9c*/ LDR         IP, [R5, #240]              @ 0xf0 
    /*00006fa0*/ BFI         R3, LR, #0, #8 
    /*00006fa4*/ LDR         LR, [R5, #340]              @ 0x154 
    /*00006fa8*/ BFI         R3, IP, #8, #8 
    /*00006fac*/ LDR         IP, [R5, #336]              @ 0x150 
    /*00006fb0*/ BFI         R3, LR, #16, #8 
    /*00006fb4*/ BFI         R3, IP, #24, #8 
    /*00006fb8*/ STR         R3, [R4, #264]              @ 0x108 
    /*00006fbc*/ LDR         R3, [R4, #268]              @ 0x10c 
    /*00006fc0*/ LDR         LR, [R5, #436]              @ 0x1b4 
    /*00006fc4*/ LDR         IP, [R5, #432]              @ 0x1b0 
    /*00006fc8*/ BFI         R3, LR, #0, #8 
    /*00006fcc*/ LDR         LR, [R5, #532]              @ 0x214 
    /*00006fd0*/ BFI         R3, IP, #8, #8 
    /*00006fd4*/ LDR         IP, [R5, #528]              @ 0x210 
    /*00006fd8*/ BFI         R3, LR, #16, #8 
    /*00006fdc*/ BFI         R3, IP, #24, #8 
    /*00006fe0*/ STR         R3, [R4, #268]              @ 0x10c 
    /*00006fe4*/ LDR         R3, [R4, #272]              @ 0x110 
    /*00006fe8*/ LDR         LR, [R5, #628]              @ 0x274 
    /*00006fec*/ LDR         IP, [R5, #624]              @ 0x270 
    /*00006ff0*/ BFI         R3, LR, #0, #8 
    /*00006ff4*/ LDR         LR, [R5, #724]              @ 0x2d4 
    /*00006ff8*/ BFI         R3, IP, #8, #8 
    /*00006ffc*/ LDR         IP, [R5, #720]              @ 0x2d0 
    /*00007000*/ BFI         R3, LR, #16, #8 
    /*00007004*/ BFI         R3, IP, #24, #8 
    /*00007008*/ STR         R3, [R4, #272]              @ 0x110 
    /*0000700c*/ LDR         R3, [R4, #276]              @ 0x114 
    /*00007010*/ LDR         LR, [R5, #44]               @ 0x2c 
    /*00007014*/ LDR         IP, [R5, #140]              @ 0x8c 
    /*00007018*/ BFI         R3, LR, #0, #8 
    /*0000701c*/ LDR         LR, [R5, #236]              @ 0xec 
    /*00007020*/ BFI         R3, IP, #8, #8 
    /*00007024*/ LDR         IP, [R5, #332]              @ 0x14c 
    /*00007028*/ BFI         R3, LR, #16, #8 
    /*0000702c*/ BFI         R3, IP, #24, #8 
    /*00007030*/ STR         R3, [R4, #276]              @ 0x114 
    /*00007034*/ LDR         LR, [R5, #428]              @ 0x1ac 
    /*00007038*/ LDR         R3, [R4, #280]              @ 0x118 
    /*0000703c*/ LDR         IP, [R5, #524]              @ 0x20c 
    /*00007040*/ BFI         R3, LR, #0, #8 
    /*00007044*/ LDR         LR, [R5, #620]              @ 0x26c 
    /*00007048*/ BFI         R3, IP, #8, #8 
    /*0000704c*/ LDR         IP, [R5, #716]              @ 0x2cc 
    /*00007050*/ BFI         R3, LR, #16, #8 
    /*00007054*/ BFI         R3, IP, #24, #8 
    /*00007058*/ STR         R3, [R4, #280]              @ 0x118 
    JPEGE_DRV_SetOsd_x705c:
    /*0000705c*/ ADD         IP, R4, R2, LSL #2 
    /*00007060*/ LDR         R6, [R0, #24] 
    /*00007064*/ ADD         R2, R2, #1 
    /*00007068*/ LDR         LR, [R0, #28] 
    /*0000706c*/ LDR         R3, [IP, #284]              @ 0x11c 
    /*00007070*/ CMP         R2, #8 
    /*00007074*/ ADD         R0, R0, #96                 @ 0x60 
    /*00007078*/ BFI         R3, R6, #0, #16 
    /*0000707c*/ BFI         R3, LR, #16, #16 
    /*00007080*/ STR         R3, [IP, #284]              @ 0x11c 
    /*00007084*/ BNE         JPEGE_DRV_SetOsd_x705c
    /*00007088*/ MOV         R0, R5 
    /*0000708c*/ MOV         R2, #0 
    JPEGE_DRV_SetOsd_x7090:
    /*00007090*/ ADD         R6, R4, R2, LSL #2 
    /*00007094*/ LDR         LR, [R0, #32] 
    /*00007098*/ LDR         IP, [R0, #36]               @ 0x24 
    /*0000709c*/ ADD         R2, R2, #1 
    /*000070a0*/ LDR         R3, [R6, #316]              @ 0x13c 
    /*000070a4*/ SUB         LR, LR, #1 
    /*000070a8*/ CMP         R2, #8 
    /*000070ac*/ SUB         IP, IP, #1 
    /*000070b0*/ BFI         R3, LR, #0, #16 
    /*000070b4*/ ADD         R0, R0, #96                 @ 0x60 
    /*000070b8*/ BFI         R3, IP, #16, #16 
    /*000070bc*/ STR         R3, [R6, #316]              @ 0x13c 
    /*000070c0*/ BNE         JPEGE_DRV_SetOsd_x7090
    /*000070c4*/ LDR         R3, [R5, #56]               @ 0x38 
    /*000070c8*/ MOV         R0, R5 
    /*000070cc*/ MOV         R2, #0 
    /*000070d0*/ STR         R3, [R4, #768]              @ 0x300 
    /*000070d4*/ LDR         R3, [R5, #152]              @ 0x98 
    /*000070d8*/ STR         R3, [R4, #772]              @ 0x304 
    /*000070dc*/ LDR         R3, [R5, #248]              @ 0xf8 
    /*000070e0*/ STR         R3, [R4, #776]              @ 0x308 
    /*000070e4*/ LDR         R3, [R5, #344]              @ 0x158 
    /*000070e8*/ STR         R3, [R4, #780]              @ 0x30c 
    /*000070ec*/ LDR         R3, [R5, #440]              @ 0x1b8 
    /*000070f0*/ STR         R3, [R4, #784]              @ 0x310 
    /*000070f4*/ LDR         R3, [R5, #536]              @ 0x218 
    /*000070f8*/ STR         R3, [R4, #788]              @ 0x314 
    /*000070fc*/ LDR         R3, [R5, #632]              @ 0x278 
    /*00007100*/ STR         R3, [R4, #792]              @ 0x318 
    /*00007104*/ LDR         R3, [R5, #728]              @ 0x2d8 
    /*00007108*/ STR         R3, [R4, #796]              @ 0x31c 
    JPEGE_DRV_SetOsd_x710c:
    /*0000710c*/ ADD         IP, R4, R2, LSL #2 
    /*00007110*/ ADD         R2, R2, #1 
    /*00007114*/ LDR         LR, [R0, #64]               @ 0x40 
    /*00007118*/ CMP         R2, #8 
    /*0000711c*/ LDR         R3, [IP, #364]              @ 0x16c 
    /*00007120*/ ADD         R0, R0, #96                 @ 0x60 
    /*00007124*/ BFI         R3, LR, #0, #16 
    /*00007128*/ STR         R3, [IP, #364]              @ 0x16c 
    /*0000712c*/ BNE         JPEGE_DRV_SetOsd_x710c
    /*00007130*/ LDR         IP, [R5, #40]               @ 0x28 
    /*00007134*/ MOV         R2, #0 
    /*00007138*/ LDR         R3, [R4, #348]              @ 0x15c 
    /*0000713c*/ LDR         R0, [R5, #136]              @ 0x88 
    /*00007140*/ BFI         R3, IP, #0, #3 
    /*00007144*/ LDR         IP, [R5, #232]              @ 0xe8 
    /*00007148*/ BFI         R3, R0, #4, #3 
    /*0000714c*/ LDR         R0, [R5, #328]              @ 0x148 
    /*00007150*/ BFI         R3, IP, #8, #3 
    /*00007154*/ LDR         IP, [R5, #424]              @ 0x1a8 
    /*00007158*/ BFI         R3, R0, #12, #3 
    /*0000715c*/ LDR         R0, [R5, #520]              @ 0x208 
    /*00007160*/ BFI         R3, IP, #16, #3 
    /*00007164*/ LDR         IP, [R5, #616]              @ 0x268 
    /*00007168*/ BFI         R3, R0, #20, #3 
    /*0000716c*/ LDR         R0, [R5, #712]              @ 0x2c8 
    /*00007170*/ BFI         R3, IP, #24, #3 
    /*00007174*/ BFI         R3, R0, #28, #3 
    /*00007178*/ STR         R3, [R4, #348]              @ 0x15c 
    JPEGE_DRV_SetOsd_x717c:
    /*0000717c*/ ADD         R0, R4, R2, LSL #2 
    /*00007180*/ LDRH        LR, [R1, #104]              @ 0x68 
    /*00007184*/ ADD         R2, R2, #1 
    /*00007188*/ LDRH        IP, [R1, #106]              @ 0x6a 
    /*0000718c*/ LDR         R3, [R0, #396]              @ 0x18c 
    /*00007190*/ CMP         R2, #8 
    /*00007194*/ ADD         R1, R1, #96                 @ 0x60 
    /*00007198*/ BFI         R3, LR, #0, #15 
    /*0000719c*/ BFI         R3, IP, #16, #15 
    /*000071a0*/ STR         R3, [R0, #396]              @ 0x18c 
    /*000071a4*/ BNE         JPEGE_DRV_SetOsd_x717c
    /*000071a8*/ ADD         SP, SP, #84                 @ 0x54 
    /*000071ac*/ POP         {R4, R5, R6, R7, PC} 
    JPEGE_DRV_SetOsd_x71b0:
    /*000071b0*/ MOV         R0, R2 
    /*000071b4*/ STR         R2, [SP, #12] 
    /*000071b8*/ BL          0x00000000000071B8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000071bc*/ LDR         R2, [SP, #12] 
    /*000071c0*/ LDR         R3, [R0, #104]              @ 0x68 
    /*000071c4*/ MOV         R0, R2 
    /*000071c8*/ BLX         R3 
    /*000071cc*/ CMP         R0, #0 
    /*000071d0*/ BEQ         JPEGE_DRV_SetOsd_x6e30
    JPEGE_DRV_SetOsd_x71d4:
    /*000071d4*/ BFC         R6, #2, #1 
    /*000071d8*/ STR         R6, [R4, #48]               @ 0x30 
    /*000071dc*/ ADD         SP, SP, #84                 @ 0x54 
    /*000071e0*/ POP         {R4, R5, R6, R7, PC} 
    /*000071e4*/ ANDEQ       R0, R0, R0, LSL SL 
FUNC_END JPEGE_DRV_SetOsd


FUNC_BEGIN JPEGE_DRV_WriteRegs
    /*000071e8*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000071ec*/ MOV         R3, #3 
    /*000071f0*/ BFC         R3, #6, #1 
    /*000071f4*/ STR         R3, [R1, #4] 
    /*000071f8*/ LDR         R2, [R0, #2892]             @ 0xb4c 
    /*000071fc*/ MOV         R5, R1 
    /*00007200*/ LDR         IP, [R0] 
    /*00007204*/ MOV         R4, R0 
    /*00007208*/ CMP         R2, #0 
    /*0000720c*/ MOV         R1, #63                     @ 0x3f 
    /*00007210*/ ADD         R7, R4, #552                @ 0x228 
    /*00007214*/ MOVEQ       R3, #0 
    /*00007218*/ BFIEQ       R3, R2, #0, #2 
    /*0000721c*/ LDR         R2, [R0, #2880]             @ 0xb40 
    /*00007220*/ MOVNE       R3, #3 
    /*00007224*/ BFC         R3, #2, #1 
    /*00007228*/ CMP         R2, #1 
    /*0000722c*/ LDR         R2, [R0, #4] 
    /*00007230*/ ORREQ       R3, R3, #524288             @ 0x80000 
    /*00007234*/ BFCNE       R3, #19, #1 
    /*00007238*/ CMP         R2, #3 
    /*0000723c*/ MOV         R0, #1 
    /*00007240*/ BFCEQ       R3, #3, #2 
    /*00007244*/ BFINE       R3, R2, #3, #2 
    /*00007248*/ ORREQ       R3, R3, #524288             @ 0x80000 
    /*0000724c*/ MOV         R2, #0 
    /*00007250*/ BFI         R3, IP, #8, #8 
    /*00007254*/ MOV         R6, R2 
    /*00007258*/ BFC         R3, #16, #1 
    /*0000725c*/ MOV         IP, R2 
    /*00007260*/ BFC         R3, #18, #1 
    /*00007264*/ BFC         IP, #0, #1 
    /*00007268*/ BFI         R3, R0, #20, #8 
    /*0000726c*/ STR         R3, [R5, #48]               @ 0x30 
    /*00007270*/ LDR         LR, [R4, #8] 
    /*00007274*/ MOV         R0, R2 
    /*00007278*/ LDR         R3, [R4, #12] 
    /*0000727c*/ BFI         R2, LR, #0, #16 
    /*00007280*/ BFI         R2, R3, #16, #16 
    /*00007284*/ STR         R2, [R5, #52]               @ 0x34 
    /*00007288*/ STR         R1, [R5, #56]               @ 0x38 
    /*0000728c*/ MOV         R2, R6 
    /*00007290*/ STR         IP, [R5, #100]              @ 0x64 
    /*00007294*/ MOV         R1, R6 
    /*00007298*/ STR         R6, [R5, #816]              @ 0x330 
    /*0000729c*/ BFI         R1, R6, #0, #10 
    /*000072a0*/ STR         R6, [R5, #820]              @ 0x334 
    /*000072a4*/ BFI         R2, R6, #0, #10 
    /*000072a8*/ LDR         R3, [R4, #152]              @ 0x98 
    /*000072ac*/ BFI         R1, R6, #16, #10 
    /*000072b0*/ LDR         IP, [R4, #156]              @ 0x9c 
    /*000072b4*/ BFI         R2, R6, #16, #10 
    /*000072b8*/ BFI         R0, R3, #0, #1 
    /*000072bc*/ MOV         R3, R6 
    /*000072c0*/ BFI         R0, IP, #4, #20 
    /*000072c4*/ STR         R0, [R5, #64]               @ 0x40 
    /*000072c8*/ STR         R1, [R5, #68]               @ 0x44 
    /*000072cc*/ MOVW        IP, #65535                  @ 0xffff 
    /*000072d0*/ STR         R2, [R5, #72]               @ 0x48 
    /*000072d4*/ MOVT        IP, #15 
    /*000072d8*/ LDR         R0, [R4, #2896]             @ 0xb50 
    /*000072dc*/ MOVW        R2, #9372                   @ 0x249c 
    /*000072e0*/ LDR         R1, [R4, #2904]             @ 0xb58 
    /*000072e4*/ MOVT        R2, #23211                  @ 0x5aab 
    /*000072e8*/ BFI         R3, R0, #1, #1 
    /*000072ec*/ MOVW        R0, #41485                  @ 0xa20d 
    /*000072f0*/ BFI         R3, R1, #16, #2 
    /*000072f4*/ MOVT        R0, #8866                   @ 0x22a2 
    /*000072f8*/ STR         R3, [R5, #84]               @ 0x54 
    /*000072fc*/ MOV         R1, R6 
    /*00007300*/ STR         IP, [R5, #76]               @ 0x4c 
    /*00007304*/ MOV         R3, R6 
    /*00007308*/ STR         R2, [R5, #88]               @ 0x58 
    /*0000730c*/ MOV         R2, R6 
    /*00007310*/ STR         R0, [R5, #92]               @ 0x5c 
    /*00007314*/ LDR         IP, [R4, #64]               @ 0x40 
    /*00007318*/ LDR         R0, [R4, #68]               @ 0x44 
    /*0000731c*/ BFI         R1, IP, #0, #16 
    /*00007320*/ BFI         R1, R0, #16, #16 
    /*00007324*/ STR         R1, [R5, #356]              @ 0x164 
    /*00007328*/ LDR         R0, [R4, #80]               @ 0x50 
    /*0000732c*/ MOV         R1, R7 
    /*00007330*/ BFI         R3, R0, #0, #2 
    /*00007334*/ STR         R3, [R5, #360]              @ 0x168 
    /*00007338*/ LDR         R0, [R4, #128]              @ 0x80 
    /*0000733c*/ LDR         R3, [R4, #124]              @ 0x7c 
    /*00007340*/ BFI         R2, R0, #0, #8 
    /*00007344*/ BFI         R2, R3, #8, #1 
    /*00007348*/ STR         R2, [R5, #36]               @ 0x24 
    /*0000734c*/ LDR         R3, [R4, #136]              @ 0x88 
    /*00007350*/ STR         R3, [R5, #540]              @ 0x21c 
    /*00007354*/ LDR         R3, [R4, #32] 
    /*00007358*/ STR         R3, [R5, #512]              @ 0x200 
    /*0000735c*/ LDR         R3, [R4, #40]               @ 0x28 
    /*00007360*/ STR         R3, [R5, #516]              @ 0x204 
    /*00007364*/ LDR         R3, [R4, #16] 
    /*00007368*/ STR         R3, [R5, #520]              @ 0x208 
    /*0000736c*/ LDR         R3, [R4, #24] 
    /*00007370*/ STR         R3, [R5, #524]              @ 0x20c 
    /*00007374*/ LDR         R3, [R4, #96]               @ 0x60 
    /*00007378*/ STR         R3, [R5, #96]               @ 0x60 
    /*0000737c*/ LDR         R3, [R4, #88]               @ 0x58 
    /*00007380*/ STR         R3, [R5, #528]              @ 0x210 
    /*00007384*/ LDR         R3, [R4, #104]              @ 0x68 
    /*00007388*/ STR         R3, [R5, #532]              @ 0x214 
    /*0000738c*/ LDR         R3, [R4, #112]              @ 0x70 
    /*00007390*/ STR         R3, [R5, #536]              @ 0x218 
    /*00007394*/ LDR         R3, [R4, #144]              @ 0x90 
    /*00007398*/ STR         R3, [R5, #800]              @ 0x320 
    /*0000739c*/ LDR         R3, [R4, #148]              @ 0x94 
    /*000073a0*/ STR         R3, [R5, #804]              @ 0x324 
    /*000073a4*/ STR         R6, [R5, #808]              @ 0x328 
    /*000073a8*/ STR         R6, [R5, #812]              @ 0x32c 
    /*000073ac*/ LDR         R2, [R4, #2888]             @ 0xb48 
    /*000073b0*/ LDR         R0, [R4, #544]              @ 0x220 
    /*000073b4*/ BL          0x00000000000073B4          @ JPEGE_DRV_GetOsdInfo [R_ARM_CALL: .text+0x6db8] 
    /*000073b8*/ MOV         R3, R7 
    /*000073bc*/ LDR         R2, [R4, #2884]             @ 0xb44 
    /*000073c0*/ MOV         R0, R5 
    /*000073c4*/ LDR         R1, [R4, #544]              @ 0x220 
    /*000073c8*/ BL          0x00000000000073C8          @ JPEGE_DRV_SetOsd [R_ARM_CALL: .text+0x6e00] 
    /*000073cc*/ MOV         R0, R4 
    /*000073d0*/ MOV         R1, R5 
    /*000073d4*/ BL          0x00000000000073D4          @ JPEGE_DRV_SetQt [R_ARM_CALL: .text+0x6b48] 
    /*000073d8*/ MOV         R0, R6 
    /*000073dc*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END JPEGE_DRV_WriteRegs


FUNC_BEGIN JPEGE_DRV_CheckDcf
    /*000073e0*/ PUSH        {R4, LR} 
    /*000073e4*/ SUBS        R4, R0, #0 
    /*000073e8*/ SUB         SP, SP, #8 
    /*000073ec*/ BEQ         JPEGE_DRV_CheckDcf_x744c
    JPEGE_DRV_CheckDcf_x73f0:
    /*000073f0*/ LDR         R4, [R4, #32] 
    /*000073f4*/ CMP         R4, #1 
    /*000073f8*/ BEQ         JPEGE_DRV_CheckDcf_x7408
    JPEGE_DRV_CheckDcf_x73fc:
    /*000073fc*/ MOV         R0, #0 
    JPEGE_DRV_CheckDcf_x7400:
    /*00007400*/ ADD         SP, SP, #8 
    /*00007404*/ POP         {R4, PC} 
    JPEGE_DRV_CheckDcf_x7408:
    /*00007408*/ MOV         R0, R4 
    /*0000740c*/ BL          0x000000000000740C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00007410*/ LDR         R3, [R0, #104]              @ 0x68 
    /*00007414*/ MOV         R0, R4 
    /*00007418*/ BLX         R3 
    /*0000741c*/ CMP         R0, #0 
    /*00007420*/ BNE         JPEGE_DRV_CheckDcf_x73fc
    /*00007424*/ MOVW        R3, #1276                   @ 0x4fc 
    /*00007428*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bc4] 
    /*0000742c*/ STR         R3, [SP] 
    /*00007430*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bc4] 
    /*00007434*/ LDR         R3, [PC, #48]               @ 0x000000000000746c 
    /*00007438*/ MOV         R1, #11 
    /*0000743c*/ MOV         R0, #3 
    /*00007440*/ BL          0x0000000000007440          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007444*/ MVN         R0, #0 
    /*00007448*/ B           JPEGE_DRV_CheckDcf_x7400
    JPEGE_DRV_CheckDcf_x744c:
    /*0000744c*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b70] 
    /*00007450*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b84] 
    /*00007454*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b70] 
    /*00007458*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b84] 
    /*0000745c*/ MOVW        R2, #1270                   @ 0x4f6 
    /*00007460*/ LDR         R1, [PC, #4]                @ 0x000000000000746c 
    /*00007464*/ BL          0x0000000000007464          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00007468*/ B           JPEGE_DRV_CheckDcf_x73f0
    /*0000746c*/ ANDEQ       R0, R0, R4, LSR #20 
FUNC_END JPEGE_DRV_CheckDcf


FUNC_BEGIN JPEGE_DRV_RingPostProcess
    /*00007470*/ BX          LR 
FUNC_END JPEGE_DRV_RingPostProcess


FUNC_BEGIN JPEGE_DRV_ConvertState
    /*00007474*/ LDR         R3, [R1] 
    /*00007478*/ CMP         R3, #0 
    /*0000747c*/ MOVEQ       R0, #255                    @ 0xff 
    /*00007480*/ MOVNE       R0, #254                    @ 0xfe 
    /*00007484*/ BX          LR 
FUNC_END JPEGE_DRV_ConvertState


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .init.text
@ Size: 0x10
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN jpege_mod_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ BL          0x0000000000000004          @ JPEGE_ModInit [R_ARM_CALL: .text+0x4140] 
    /*00000008*/ MOV         R0, #0 
    /*0000000c*/ POP         {R4, PC} 
FUNC_END jpege_mod_init




@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .exit.text
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN jpege_mod_exit
    jpege_mod_exit_x0:
    /*00000000*/ B           jpege_mod_exit_x0           @ JPEGE_ModExit [R_ARM_JUMP24: .text+0x421c] 
FUNC_END jpege_mod_exit




