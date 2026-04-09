@
@ -- FILES --
@

@ region.c
@ region_ext.c
@ rgn_init.c
@ hi3516cv500_rgn.mod.c


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text
@ Size: 0xbcf8
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN RgnMod2String
    /*00000000*/ SUB         R0, R0, #6 
    /*00000004*/ CMP         R0, #20 
    /*00000008*/ LDRLS       PC, [PC, R0, LSL #2] 
    /*0000000c*/ B           RgnMod2String_xac
    /*00000010*/ ANDEQ       R0, R0, R0, LSR #1 
    /*00000014*/ ANDEQ       R0, R0, R4, RRX 
    /*00000018*/ ANDEQ       R0, R0, R0, ROR R0 
    /*0000001c*/ ANDEQ       R0, R0, IP, LSR #1 
    /*00000020*/ ANDEQ       R0, R0, IP, LSR #1 
    /*00000024*/ ANDEQ       R0, R0, IP, LSR #1 
    /*00000028*/ ANDEQ       R0, R0, IP, LSR #1 
    /*0000002c*/ ANDEQ       R0, R0, IP, LSR #1 
    /*00000030*/ ANDEQ       R0, R0, IP, LSR #1 
    /*00000034*/ ANDEQ       R0, R0, IP, ROR R0 
    /*00000038*/ ANDEQ       R0, R0, R8, LSL #1 
    /*0000003c*/ ANDEQ       R0, R0, IP, LSR #1 
    /*00000040*/ ANDEQ       R0, R0, IP, LSR #1 
    /*00000044*/ ANDEQ       R0, R0, IP, LSR #1 
    /*00000048*/ ANDEQ       R0, R0, IP, LSR #1 
    /*0000004c*/ ANDEQ       R0, R0, IP, LSR #1 
    /*00000050*/ ANDEQ       R0, R0, IP, LSR #1 
    /*00000054*/ ANDEQ       R0, R0, IP, LSR #1 
    /*00000058*/ ANDEQ       R0, R0, IP, LSR #1 
    /*0000005c*/ ANDEQ       R0, R0, IP, LSR #1 
    /*00000060*/ MULEQ       R0, R4, R0 
    /*00000064*/ MOVW        R0, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*00000068*/ MOVT        R0, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*0000006c*/ BX          LR 
    /*00000070*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10] 
    /*00000074*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10] 
    /*00000078*/ BX          LR 
    /*0000007c*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8] 
    /*00000080*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8] 
    /*00000084*/ BX          LR 
    /*00000088*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc] 
    /*0000008c*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc] 
    /*00000090*/ BX          LR 
    /*00000094*/ MOVW        R0, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18] 
    /*00000098*/ MOVT        R0, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18] 
    /*0000009c*/ BX          LR 
    /*000000a0*/ MOVW        R0, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x20] 
    /*000000a4*/ MOVT        R0, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x20] 
    /*000000a8*/ BX          LR 
    RgnMod2String_xac:
    /*000000ac*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24] 
    /*000000b0*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24] 
    /*000000b4*/ BX          LR 
FUNC_END RgnMod2String


FUNC_BEGIN RgnGetNextUpdateBuf
    /*000000b8*/ PUSH        {R4, LR} 
    /*000000bc*/ SUBS        R4, R0, #0 
    /*000000c0*/ BEQ         RgnGetNextUpdateBuf_x110
    RgnGetNextUpdateBuf_xc4:
    /*000000c4*/ LDR         R1, [R4, #116]              @ 0x74 
    /*000000c8*/ CMP         R1, #0 
    /*000000cc*/ BEQ         RgnGetNextUpdateBuf_xfc
    /*000000d0*/ ADD         R3, R4, #184                @ 0xb8 
    /*000000d4*/ MOV         R0, #0 
    RgnGetNextUpdateBuf_xd8:
    /*000000d8*/ LDR         R2, [R3], #4 
    /*000000dc*/ CMP         R2, #0 
    /*000000e0*/ BNE         RgnGetNextUpdateBuf_xf0
    /*000000e4*/ LDR         R2, [R4, #216]              @ 0xd8 
    /*000000e8*/ CMP         R2, R0 
    /*000000ec*/ BNE         RgnGetNextUpdateBuf_x104
    RgnGetNextUpdateBuf_xf0:
    /*000000f0*/ ADD         R0, R0, #1 
    /*000000f4*/ CMP         R0, R1 
    /*000000f8*/ BNE         RgnGetNextUpdateBuf_xd8
    RgnGetNextUpdateBuf_xfc:
    /*000000fc*/ MVN         R0, #0 
    /*00000100*/ POP         {R4, PC} 
    RgnGetNextUpdateBuf_x104:
    /*00000104*/ CMP         R1, R0 
    /*00000108*/ MVNLS       R0, #0 
    /*0000010c*/ POP         {R4, PC} 
    RgnGetNextUpdateBuf_x110:
    /*00000110*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c] 
    /*00000114*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*00000118*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c] 
    /*0000011c*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*00000120*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000124*/ MOVW        R2, #1782                   @ 0x6f6 
    /*00000128*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*0000012c*/ BL          0x000000000000012C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000130*/ B           RgnGetNextUpdateBuf_xc4
FUNC_END RgnGetNextUpdateBuf


FUNC_BEGIN Rgn_WaitConditionCallBackEx
    /*00000134*/ PUSH        {R4, R5, R6, LR} 
    /*00000138*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000013c*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000140*/ SUB         SP, SP, #8 
    /*00000144*/ MOV         R6, R0 
    /*00000148*/ MOV         R0, R4 
    /*0000014c*/ BL          0x000000000000014C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000150*/ SUBS        R5, R0, #0 
    /*00000154*/ BNE         Rgn_WaitConditionCallBackEx_x1cc
    /*00000158*/ ADD         R0, R4, #4 
    /*0000015c*/ ADD         R1, SP, #4 
    /*00000160*/ BL          0x0000000000000160          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000164*/ LDR         R3, [R6] 
    /*00000168*/ ADD         R4, R4, R3, LSL #2 
    /*0000016c*/ LDR         R2, [R4, #8] 
    /*00000170*/ CMP         R2, #0 
    /*00000174*/ BEQ         Rgn_WaitConditionCallBackEx_x18c
    /*00000178*/ MOVW        R3, #0                      @ s_u32RgnState [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000017c*/ MOVT        R3, #0                      @ s_u32RgnState [R_ARM_MOVT_ABS: .data+0x0] 
    /*00000180*/ LDR         R3, [R3] 
    /*00000184*/ CMP         R3, #0 
    /*00000188*/ BEQ         Rgn_WaitConditionCallBackEx_x1b4
    Rgn_WaitConditionCallBackEx_x18c:
    /*0000018c*/ MOV         R5, #1 
    Rgn_WaitConditionCallBackEx_x190:
    /*00000190*/ LDR         R0, [PC, #64]               @ 0x00000000000001d8 
    /*00000194*/ ADD         R1, SP, #4 
    /*00000198*/ BL          0x0000000000000198          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000019c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000001a0*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000001a4*/ BL          0x00000000000001A4          @ osal_up [R_ARM_CALL: +0x0] 
    Rgn_WaitConditionCallBackEx_x1a8:
    /*000001a8*/ MOV         R0, R5 
    /*000001ac*/ ADD         SP, SP, #8 
    /*000001b0*/ POP         {R4, R5, R6, PC} 
    Rgn_WaitConditionCallBackEx_x1b4:
    /*000001b4*/ LDR         R3, [R2, #180]              @ 0xb4 
    /*000001b8*/ CMP         R3, #0 
    /*000001bc*/ LDREQ       R5, [R2, #228]              @ 0xe4 
    /*000001c0*/ CLZEQ       R5, R5 
    /*000001c4*/ LSREQ       R5, R5, #5 
    /*000001c8*/ B           Rgn_WaitConditionCallBackEx_x190
    Rgn_WaitConditionCallBackEx_x1cc:
    /*000001cc*/ MOV         R5, #65024                  @ 0xfe00 
    /*000001d0*/ MOVT        R5, #65535                  @ 0xffff 
    /*000001d4*/ B           Rgn_WaitConditionCallBackEx_x1a8
    /*000001d8*/ ANDEQ       R0, R0, R4 
FUNC_END Rgn_WaitConditionCallBackEx


FUNC_BEGIN Rgn_WaitConditionCallBack
    /*000001dc*/ PUSH        {R4, R5, LR} 
    /*000001e0*/ SUBS        R5, R0, #0 
    /*000001e4*/ SUB         SP, SP, #12 
    /*000001e8*/ BEQ         Rgn_WaitConditionCallBack_x25c
    /*000001ec*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000001f0*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ BL          0x00000000000001F8          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000001fc*/ CMP         R0, #0 
    /*00000200*/ MOVNE       R4, #0 
    /*00000204*/ BNE         Rgn_WaitConditionCallBack_x238
    /*00000208*/ LDR         R3, [R5] 
    /*0000020c*/ ADD         R4, R4, R3, LSL #2 
    /*00000210*/ LDR         R0, [R4, #8] 
    /*00000214*/ CMP         R0, #0 
    /*00000218*/ BEQ         Rgn_WaitConditionCallBack_x228
    /*0000021c*/ BL          0x00000000000000B8 
    /*00000220*/ CMN         R0, #1 
    /*00000224*/ BEQ         Rgn_WaitConditionCallBack_x244
    Rgn_WaitConditionCallBack_x228:
    /*00000228*/ MOV         R4, #1 
    Rgn_WaitConditionCallBack_x22c:
    /*0000022c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000230*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000234*/ BL          0x0000000000000234          @ osal_up [R_ARM_CALL: +0x0] 
    Rgn_WaitConditionCallBack_x238:
    /*00000238*/ MOV         R0, R4 
    /*0000023c*/ ADD         SP, SP, #12 
    /*00000240*/ POP         {R4, R5, PC} 
    Rgn_WaitConditionCallBack_x244:
    /*00000244*/ MOVW        R3, #0                      @ s_u32RgnState [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000248*/ MOVT        R3, #0                      @ s_u32RgnState [R_ARM_MOVT_ABS: .data+0x0] 
    /*0000024c*/ LDR         R4, [R3] 
    /*00000250*/ ADDS        R4, R4, #0 
    /*00000254*/ MOVNE       R4, #1 
    /*00000258*/ B           Rgn_WaitConditionCallBack_x22c
    Rgn_WaitConditionCallBack_x25c:
    /*0000025c*/ MOV         R1, #3 
    /*00000260*/ MOVW        R3, #3306                   @ 0xcea 
    /*00000264*/ MOV         R0, R1 
    /*00000268*/ STR         R3, [SP] 
    /*0000026c*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x84] 
    /*00000270*/ LDR         R3, [PC, #20]               @ 0x000000000000028c 
    /*00000274*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x84] 
    /*00000278*/ MOV         R4, R5 
    /*0000027c*/ BL          0x000000000000027C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000280*/ MOV         R0, R4 
    /*00000284*/ ADD         SP, SP, #12 
    /*00000288*/ POP         {R4, R5, PC} 
    /*0000028c*/ ANDEQ       R0, R0, R4, LSL R0 
FUNC_END Rgn_WaitConditionCallBack


FUNC_BEGIN RGNCHECK_NULL.part.2
    /*00000290*/ MOV         R1, #3 
    /*00000294*/ LDR         R3, [PC, #28]               @ 0x00000000000002b8 
    /*00000298*/ PUSH        {R4, LR} 
    /*0000029c*/ MOV         R0, R1 
    /*000002a0*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb0] 
    /*000002a4*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb0] 
    /*000002a8*/ BL          0x00000000000002A8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000002ac*/ MOVW        R0, #32774                  @ 0x8006 
    /*000002b0*/ MOVT        R0, #40963                  @ 0xa003 
    /*000002b4*/ POP         {R4, PC} 
    /*000002b8*/ ANDEQ       R0, R0, R0, LSR R0 
FUNC_END RGNCHECK_NULL.part.2


FUNC_BEGIN RGNCHECK_RGN_TYPE.part.3
    /*000002bc*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000002c0*/ SUB         SP, SP, #12 
    /*000002c4*/ MOV         R1, #3 
    /*000002c8*/ MOV         R3, #88                     @ 0x58 
    /*000002cc*/ STR         R0, [SP, #4] 
    /*000002d0*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd0] 
    /*000002d4*/ STR         R3, [SP] 
    /*000002d8*/ MOV         R0, R1 
    /*000002dc*/ LDR         R3, [PC, #20]               @ 0x00000000000002f8 
    /*000002e0*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd0] 
    /*000002e4*/ BL          0x00000000000002E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000002e8*/ MOVW        R0, #32771                  @ 0x8003 
    /*000002ec*/ MOVT        R0, #40963                  @ 0xa003 
    /*000002f0*/ ADD         SP, SP, #12 
    /*000002f4*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*000002f8*/ ANDEQ       R0, R0, R0, ASR #32 
FUNC_END RGNCHECK_RGN_TYPE.part.3


FUNC_BEGIN RGNCHECK_HANDLE.part.4
    /*000002fc*/ MOV         R1, #3 
    /*00000300*/ MOV         R3, R0 
    /*00000304*/ PUSH        {R4, LR} 
    /*00000308*/ MOV         R0, R1 
    /*0000030c*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x100] 
    /*00000310*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x100] 
    /*00000314*/ BL          0x0000000000000314          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000318*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000031c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00000320*/ POP         {R4, PC} 
FUNC_END RGNCHECK_HANDLE.part.4


FUNC_BEGIN RgnCheckAttachDestByChn.part.16
    /*00000324*/ LDR         R3, [R0, #152]              @ 0x98 
    /*00000328*/ CMP         R3, #2 
    /*0000032c*/ BHI         RgnCheckAttachDestByChn.part.16_x338
    /*00000330*/ MOV         R0, #0 
    /*00000334*/ BX          LR 
    RgnCheckAttachDestByChn.part.16_x338:
    /*00000338*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000033c*/ SUB         SP, SP, #20 
    /*00000340*/ MOV         R1, #3 
    /*00000344*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x114] 
    /*00000348*/ STR         R3, [SP, #4] 
    /*0000034c*/ MOV         R0, R1 
    /*00000350*/ STR         R1, [SP, #8] 
    /*00000354*/ MOVW        R3, #2809                   @ 0xaf9 
    /*00000358*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x114] 
    /*0000035c*/ STR         R3, [SP] 
    /*00000360*/ LDR         R3, [PC, #16]               @ 0x0000000000000378 
    /*00000364*/ BL          0x0000000000000364          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000368*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000036c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00000370*/ ADD         SP, SP, #20 
    /*00000374*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00000378*/ ANDEQ       R0, R0, R4, ASR R0 
FUNC_END RgnCheckAttachDestByChn.part.16


FUNC_BEGIN RGNCHECK_NULL
    /*0000037c*/ CMP         R0, #0 
    /*00000380*/ BEQ         RGNCHECK_NULL_x38c
    /*00000384*/ MOV         R0, #0 
    /*00000388*/ BX          LR 
    RGNCHECK_NULL_x38c:
    /*0000038c*/ B           0x0000000000000290 
FUNC_END RGNCHECK_NULL


FUNC_BEGIN RGNCHECK_RGN_TYPE
    /*00000390*/ CMP         R0, #4 
    /*00000394*/ BHI         RGNCHECK_RGN_TYPE_x3a0
    /*00000398*/ MOV         R0, #0 
    /*0000039c*/ BX          LR 
    RGNCHECK_RGN_TYPE_x3a0:
    /*000003a0*/ B           0x00000000000002BC 
FUNC_END RGNCHECK_RGN_TYPE


FUNC_BEGIN RGNCHECK_HANDLE
    /*000003a4*/ CMP         R0, #127                    @ 0x7f 
    /*000003a8*/ BHI         RGNCHECK_HANDLE_x3b4
    /*000003ac*/ MOV         R0, #0 
    /*000003b0*/ BX          LR 
    RGNCHECK_HANDLE_x3b4:
    /*000003b4*/ B           0x00000000000002FC 
FUNC_END RGNCHECK_HANDLE


FUNC_BEGIN RGNCHECK_HANDLENUM
    /*000003b8*/ CMP         R0, #23 
    /*000003bc*/ PUSH        {R4, LR} 
    /*000003c0*/ BHI         RGNCHECK_HANDLENUM_x3d4
    /*000003c4*/ CMP         R0, #1 
    /*000003c8*/ BLS         RGNCHECK_HANDLENUM_x3f8
    /*000003cc*/ MOV         R0, #0 
    /*000003d0*/ POP         {R4, PC} 
    RGNCHECK_HANDLENUM_x3d4:
    /*000003d4*/ MOV         R1, #3 
    /*000003d8*/ MOV         R3, R0 
    /*000003dc*/ MOVW        R2, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x164] 
    /*000003e0*/ MOV         R0, R1 
    /*000003e4*/ MOVT        R2, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x164] 
    /*000003e8*/ BL          0x00000000000003E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000003ec*/ MOVW        R0, #32771                  @ 0x8003 
    /*000003f0*/ MOVT        R0, #40963                  @ 0xa003 
    /*000003f4*/ POP         {R4, PC} 
    RGNCHECK_HANDLENUM_x3f8:
    /*000003f8*/ MOV         R1, #3 
    /*000003fc*/ MOV         R3, R0 
    /*00000400*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x180] 
    /*00000404*/ MOV         R0, R1 
    /*00000408*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x180] 
    /*0000040c*/ BL          0x000000000000040C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000410*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000414*/ MOVT        R0, #40963                  @ 0xa003 
    /*00000418*/ POP         {R4, PC} 
FUNC_END RGNCHECK_HANDLENUM


FUNC_BEGIN RgnCfgCapa
    /*0000041c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000420*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000424*/ ADD         R1, R3, #524                @ 0x20c 
    /*00000428*/ ADD         R2, R3, #536                @ 0x218 
    /*0000042c*/ ADD         IP, R3, #548                @ 0x224 
    /*00000430*/ ADD         R0, R3, #560                @ 0x230 
    /*00000434*/ STR         R1, [R3, #524]              @ 0x20c 
    /*00000438*/ STR         R1, [R3, #528]              @ 0x210 
    /*0000043c*/ ADD         R1, R3, #572                @ 0x23c 
    /*00000440*/ STR         R2, [R3, #536]              @ 0x218 
    /*00000444*/ STR         R2, [R3, #540]              @ 0x21c 
    /*00000448*/ MOV         R2, #0 
    /*0000044c*/ STR         IP, [R3, #548]              @ 0x224 
    /*00000450*/ STR         IP, [R3, #552]              @ 0x228 
    /*00000454*/ STR         R0, [R3, #560]              @ 0x230 
    /*00000458*/ STR         R0, [R3, #564]              @ 0x234 
    /*0000045c*/ STR         R1, [R3, #572]              @ 0x23c 
    /*00000460*/ STR         R1, [R3, #576]              @ 0x240 
    /*00000464*/ STR         R2, [R3, #520]              @ 0x208 
    /*00000468*/ STR         R2, [R3, #532]              @ 0x214 
    /*0000046c*/ STR         R2, [R3, #544]              @ 0x220 
    /*00000470*/ STR         R2, [R3, #556]              @ 0x22c 
    /*00000474*/ STR         R2, [R3, #568]              @ 0x238 
    /*00000478*/ BX          LR 
FUNC_END RgnCfgCapa


FUNC_BEGIN RgnAttrToComm
    /*0000047c*/ PUSH        {R4, R5, LR} 
    /*00000480*/ SUBS        R5, R0, #0 
    /*00000484*/ SUB         SP, SP, #12 
    /*00000488*/ MOV         R4, R1 
    /*0000048c*/ BEQ         RgnAttrToComm_x560
    RgnAttrToComm_x490:
    /*00000490*/ CMP         R4, #0 
    /*00000494*/ BEQ         RgnAttrToComm_x540
    RgnAttrToComm_x498:
    /*00000498*/ LDR         R3, [R5] 
    /*0000049c*/ CMP         R3, #4 
    /*000004a0*/ LDRLS       PC, [PC, R3, LSL #2] 
    /*000004a4*/ B           RgnAttrToComm_x50c
    /*000004a8*/ @           <UNDEFINED> instruction: 0x000004bc 
    /*000004ac*/ STRDEQ      R0, [R0], -IP 
    /*000004b0*/ STRDEQ      R0, [R0], -IP 
    /*000004b4*/ @           <UNDEFINED> instruction: 0x000004bc 
    /*000004b8*/ STRDEQ      R0, [R0], -IP 
    /*000004bc*/ MOV         R2, #8 
    /*000004c0*/ ADD         R1, R5, #12 
    /*000004c4*/ ADD         R0, R4, #16 
    /*000004c8*/ BL          0x00000000000004C8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000004cc*/ LDR         R3, [R5, #4] 
    /*000004d0*/ MOV         R0, #0 
    /*000004d4*/ STR         R3, [R4, #112]              @ 0x70 
    /*000004d8*/ LDR         R3, [R5, #8] 
    /*000004dc*/ STR         R3, [R4, #68]               @ 0x44 
    /*000004e0*/ LDR         R3, [R5, #20] 
    /*000004e4*/ SUB         R2, R3, #1 
    /*000004e8*/ CMP         R2, #5 
    /*000004ec*/ MOVHI       R3, #2 
    /*000004f0*/ STR         R3, [R4, #108]              @ 0x6c 
    /*000004f4*/ ADD         SP, SP, #12 
    /*000004f8*/ POP         {R4, R5, PC} 
    /*000004fc*/ MOV         R0, #0 
    /*00000500*/ STR         R0, [R4, #108]              @ 0x6c 
    /*00000504*/ ADD         SP, SP, #12 
    /*00000508*/ POP         {R4, R5, PC} 
    RgnAttrToComm_x50c:
    /*0000050c*/ MOV         R1, #3 
    /*00000510*/ STR         R3, [SP, #4] 
    /*00000514*/ MOV         R0, R1 
    /*00000518*/ MOV         R3, #175                    @ 0xaf 
    /*0000051c*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c8] 
    /*00000520*/ STR         R3, [SP] 
    /*00000524*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c8] 
    /*00000528*/ LDR         R3, [PC, #80]               @ 0x0000000000000580 
    /*0000052c*/ BL          0x000000000000052C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000530*/ MOVW        R0, #32776                  @ 0x8008 
    /*00000534*/ MOVT        R0, #40963                  @ 0xa003 
    /*00000538*/ ADD         SP, SP, #12 
    /*0000053c*/ POP         {R4, R5, PC} 
    RgnAttrToComm_x540:
    /*00000540*/ MOVW        R3, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*00000544*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000548*/ MOVT        R3, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*0000054c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000550*/ MOV         R2, #138                    @ 0x8a 
    /*00000554*/ LDR         R1, [PC, #36]               @ 0x0000000000000580 
    /*00000558*/ BL          0x0000000000000558          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000055c*/ B           RgnAttrToComm_x498
    RgnAttrToComm_x560:
    /*00000560*/ MOVW        R3, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19c] 
    /*00000564*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000568*/ MOVT        R3, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19c] 
    /*0000056c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000570*/ MOV         R2, #137                    @ 0x89 
    /*00000574*/ LDR         R1, [PC, #4]                @ 0x0000000000000580 
    /*00000578*/ BL          0x0000000000000578          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000057c*/ B           RgnAttrToComm_x490
    /*00000580*/ ANDEQ       R0, R0, IP, RRX 
FUNC_END RgnAttrToComm


FUNC_BEGIN RgnCommToAttr
    /*00000584*/ CMP         R0, #4 
    /*00000588*/ PUSH        {R4, R5, R6, LR} 
    /*0000058c*/ MOV         R4, R0 
    /*00000590*/ SUB         SP, SP, #8 
    /*00000594*/ MOV         R6, R1 
    /*00000598*/ MOV         R5, R2 
    /*0000059c*/ BHI         RgnCommToAttr_x614
    RgnCommToAttr_x5a0:
    /*000005a0*/ CMP         R5, #0 
    /*000005a4*/ BEQ         RgnCommToAttr_x668
    RgnCommToAttr_x5a8:
    /*000005a8*/ CMP         R6, #0 
    /*000005ac*/ BEQ         RgnCommToAttr_x688
    RgnCommToAttr_x5b0:
    /*000005b0*/ STR         R4, [R5] 
    /*000005b4*/ CMP         R4, #4 
    /*000005b8*/ LDRLS       PC, [PC, R4, LSL #2] 
    /*000005bc*/ B           RgnCommToAttr_x634
    /*000005c0*/ LDRDEQ      R0, [R0], -R4 
    /*000005c4*/ ANDEQ       R0, R0, R8, LSL #12 
    /*000005c8*/ ANDEQ       R0, R0, R8, LSL #12 
    /*000005cc*/ LDRDEQ      R0, [R0], -R4 
    /*000005d0*/ ANDEQ       R0, R0, R8, LSL #12 
    /*000005d4*/ MOV         R2, #8 
    /*000005d8*/ ADD         R1, R6, #16 
    /*000005dc*/ ADD         R0, R5, #12 
    /*000005e0*/ BL          0x00000000000005E0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000005e4*/ LDR         R3, [R6, #112]              @ 0x70 
    /*000005e8*/ MOV         R0, #0 
    /*000005ec*/ STR         R3, [R5, #4] 
    /*000005f0*/ LDR         R3, [R6, #68]               @ 0x44 
    /*000005f4*/ STR         R3, [R5, #8] 
    /*000005f8*/ LDR         R3, [R6, #108]              @ 0x6c 
    /*000005fc*/ STR         R3, [R5, #20] 
    /*00000600*/ ADD         SP, SP, #8 
    /*00000604*/ POP         {R4, R5, R6, PC} 
    /*00000608*/ MOV         R0, #0 
    /*0000060c*/ ADD         SP, SP, #8 
    /*00000610*/ POP         {R4, R5, R6, PC} 
    RgnCommToAttr_x614:
    /*00000614*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fc] 
    /*00000618*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*0000061c*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fc] 
    /*00000620*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000624*/ MOV         R2, #188                    @ 0xbc 
    /*00000628*/ LDR         R1, [PC, #120]              @ 0x00000000000006a8 
    /*0000062c*/ BL          0x000000000000062C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000630*/ B           RgnCommToAttr_x5a0
    RgnCommToAttr_x634:
    /*00000634*/ MOV         R1, #3 
    /*00000638*/ STR         R4, [SP, #4] 
    /*0000063c*/ MOV         R0, R1 
    /*00000640*/ MOV         R3, #239                    @ 0xef 
    /*00000644*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c8] 
    /*00000648*/ STR         R3, [SP] 
    /*0000064c*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c8] 
    /*00000650*/ LDR         R3, [PC, #80]               @ 0x00000000000006a8 
    /*00000654*/ BL          0x0000000000000654          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000658*/ MOVW        R0, #32776                  @ 0x8008 
    /*0000065c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00000660*/ ADD         SP, SP, #8 
    /*00000664*/ POP         {R4, R5, R6, PC} 
    RgnCommToAttr_x668:
    /*00000668*/ MOVW        R3, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19c] 
    /*0000066c*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000670*/ MOVT        R3, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19c] 
    /*00000674*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000678*/ MOV         R2, #189                    @ 0xbd 
    /*0000067c*/ LDR         R1, [PC, #36]               @ 0x00000000000006a8 
    /*00000680*/ BL          0x0000000000000680          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000684*/ B           RgnCommToAttr_x5a8
    RgnCommToAttr_x688:
    /*00000688*/ MOVW        R3, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b4] 
    /*0000068c*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000690*/ MOVT        R3, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b4] 
    /*00000694*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000698*/ MOV         R2, #190                    @ 0xbe 
    /*0000069c*/ LDR         R1, [PC, #4]                @ 0x00000000000006a8 
    /*000006a0*/ BL          0x00000000000006A0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000006a4*/ B           RgnCommToAttr_x5b0
    /*000006a8*/ ANDEQ       R0, R0, IP, ROR R0 
FUNC_END RgnCommToAttr


FUNC_BEGIN RgnDisplayAttrToChnAttr
    /*000006ac*/ PUSH        {R4, R5, LR} 
    /*000006b0*/ SUBS        R4, R0, #0 
    /*000006b4*/ SUB         SP, SP, #12 
    /*000006b8*/ MOV         R5, R1 
    /*000006bc*/ BEQ         RgnDisplayAttrToChnAttr_x8ec
    RgnDisplayAttrToChnAttr_x6c0:
    /*000006c0*/ CMP         R5, #0 
    /*000006c4*/ BEQ         RgnDisplayAttrToChnAttr_x90c
    RgnDisplayAttrToChnAttr_x6c8:
    /*000006c8*/ LDR         R3, [R4, #4] 
    /*000006cc*/ CMP         R3, #4 
    /*000006d0*/ BHI         RgnDisplayAttrToChnAttr_x700
    RgnDisplayAttrToChnAttr_x6d4:
    /*000006d4*/ LDR         R3, [R4] 
    /*000006d8*/ STR         R3, [R5] 
    /*000006dc*/ LDR         R3, [R4, #4] 
    /*000006e0*/ CMP         R3, #4 
    /*000006e4*/ LDRLS       PC, [PC, R3, LSL #2] 
    /*000006e8*/ B           RgnDisplayAttrToChnAttr_x89c
    /*000006ec*/ ANDEQ       R0, R0, IP, ASR R7 
    /*000006f0*/ @           <UNDEFINED> instruction: 0x000007b8 
    /*000006f4*/ ANDEQ       R0, R0, IP, LSL #16 
    /*000006f8*/ ANDEQ       R0, R0, R0, ASR R8 
    /*000006fc*/ ANDEQ       R0, R0, R0, LSR #14 
    RgnDisplayAttrToChnAttr_x700:
    /*00000700*/ MOVW        R3, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x250] 
    /*00000704*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000708*/ MOVT        R3, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x250] 
    /*0000070c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000710*/ MOV         R2, #254                    @ 0xfe 
    /*00000714*/ LDR         R1, [PC, #564]              @ 0x0000000000000950 
    /*00000718*/ BL          0x0000000000000718          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000071c*/ B           RgnDisplayAttrToChnAttr_x6d4
    /*00000720*/ LDR         R3, [R4, #8] 
    /*00000724*/ MOV         R0, #0 
    /*00000728*/ STR         R3, [R5, #8] 
    /*0000072c*/ LDR         R3, [R4, #12] 
    /*00000730*/ STR         R3, [R5, #12] 
    /*00000734*/ LDR         R3, [R4, #16] 
    /*00000738*/ STR         R3, [R5, #16] 
    /*0000073c*/ LDR         R3, [R4, #20] 
    /*00000740*/ STR         R3, [R5, #20] 
    /*00000744*/ LDR         R3, [R4, #28] 
    /*00000748*/ STR         R3, [R5, #64]               @ 0x40 
    /*0000074c*/ LDR         R3, [R4, #24] 
    /*00000750*/ STR         R3, [R5, #156]              @ 0x9c 
    RgnDisplayAttrToChnAttr_x754:
    /*00000754*/ ADD         SP, SP, #12 
    /*00000758*/ POP         {R4, R5, PC} 
    /*0000075c*/ MOV         R2, #8 
    /*00000760*/ ADD         R1, R4, R2 
    /*00000764*/ ADD         R0, R5, R2 
    /*00000768*/ BL          0x0000000000000768          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000076c*/ LDR         R3, [R4, #16] 
    /*00000770*/ ADD         R1, R4, #28 
    /*00000774*/ MOV         R2, #12 
    /*00000778*/ ADD         R0, R5, #120                @ 0x78 
    /*0000077c*/ STR         R3, [R5, #76]               @ 0x4c 
    /*00000780*/ LDR         R3, [R4, #20] 
    /*00000784*/ STR         R3, [R5, #80]               @ 0x50 
    /*00000788*/ LDR         R3, [R4, #24] 
    /*0000078c*/ STR         R3, [R5, #64]               @ 0x40 
    /*00000790*/ BL          0x0000000000000790          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000794*/ LDR         R3, [R4, #60]               @ 0x3c 
    /*00000798*/ ADD         R1, R4, #64                 @ 0x40 
    /*0000079c*/ ADD         R0, R5, #164                @ 0xa4 
    /*000007a0*/ MOV         R2, #4 
    /*000007a4*/ STR         R3, [R5, #152]              @ 0x98 
    /*000007a8*/ BL          0x00000000000007A8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000007ac*/ MOV         R0, #0 
    /*000007b0*/ ADD         SP, SP, #12 
    /*000007b4*/ POP         {R4, R5, PC} 
    /*000007b8*/ LDR         R3, [R4, #8] 
    /*000007bc*/ CMP         R3, #0 
    /*000007c0*/ STR         R3, [R5, #4] 
    /*000007c4*/ LDR         R2, [R4, #52]               @ 0x34 
    /*000007c8*/ STR         R2, [R5, #68]               @ 0x44 
    /*000007cc*/ LDR         R2, [R4, #56]               @ 0x38 
    /*000007d0*/ STR         R2, [R5, #64]               @ 0x40 
    /*000007d4*/ BNE         RgnDisplayAttrToChnAttr_x8d0
    /*000007d8*/ LDR         R3, [R4, #12] 
    /*000007dc*/ STR         R3, [R5, #8] 
    /*000007e0*/ LDR         R3, [R4, #16] 
    /*000007e4*/ STR         R3, [R5, #12] 
    /*000007e8*/ LDR         R3, [R4, #20] 
    /*000007ec*/ STR         R3, [R5, #16] 
    /*000007f0*/ LDR         R3, [R4, #24] 
    /*000007f4*/ STR         R3, [R5, #20] 
    RgnDisplayAttrToChnAttr_x7f8:
    /*000007f8*/ LDR         R3, [R4, #60]               @ 0x3c 
    /*000007fc*/ MOV         R0, #0 
    /*00000800*/ STR         R3, [R5, #160]              @ 0xa0 
    /*00000804*/ ADD         SP, SP, #12 
    /*00000808*/ POP         {R4, R5, PC} 
    /*0000080c*/ LDR         R0, [R4, #8] 
    /*00000810*/ CMP         R0, #0 
    /*00000814*/ STR         R0, [R5, #4] 
    /*00000818*/ LDR         R3, [R4, #52]               @ 0x34 
    /*0000081c*/ STR         R3, [R5, #68]               @ 0x44 
    /*00000820*/ LDR         R3, [R4, #56]               @ 0x38 
    /*00000824*/ STR         R3, [R5, #64]               @ 0x40 
    /*00000828*/ BEQ         RgnDisplayAttrToChnAttr_x92c
    /*0000082c*/ CMP         R0, #1 
    /*00000830*/ MOVNE       R0, #0 
    /*00000834*/ BNE         RgnDisplayAttrToChnAttr_x754
    /*00000838*/ ADD         R1, R4, #12 
    /*0000083c*/ ADD         R0, R5, #24 
    /*00000840*/ MOV         R2, #40                     @ 0x28 
    /*00000844*/ BL          0x0000000000000844          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000848*/ MOV         R0, #0 
    /*0000084c*/ B           RgnDisplayAttrToChnAttr_x754
    /*00000850*/ MOV         R2, #8 
    /*00000854*/ ADD         R1, R4, R2 
    /*00000858*/ ADD         R0, R5, R2 
    /*0000085c*/ BL          0x000000000000085C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000860*/ LDR         R3, [R4, #16] 
    /*00000864*/ ADD         R1, R4, #28 
    /*00000868*/ MOV         R2, #4 
    /*0000086c*/ ADD         R0, R5, #164                @ 0xa4 
    /*00000870*/ STR         R3, [R5, #76]               @ 0x4c 
    /*00000874*/ LDR         R3, [R4, #20] 
    /*00000878*/ STR         R3, [R5, #80]               @ 0x50 
    /*0000087c*/ LDR         R3, [R4, #24] 
    /*00000880*/ STR         R3, [R5, #64]               @ 0x40 
    /*00000884*/ BL          0x0000000000000884          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000888*/ MOV         R3, #255                    @ 0xff 
    /*0000088c*/ MOV         R0, #0 
    /*00000890*/ STR         R3, [R5, #72]               @ 0x48 
    /*00000894*/ ADD         SP, SP, #12 
    /*00000898*/ POP         {R4, R5, PC} 
    RgnDisplayAttrToChnAttr_x89c:
    /*0000089c*/ MOV         R1, #3 
    /*000008a0*/ STR         R3, [SP, #4] 
    /*000008a4*/ MOV         R0, R1 
    /*000008a8*/ MOV         R3, #348                    @ 0x15c 
    /*000008ac*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c8] 
    /*000008b0*/ STR         R3, [SP] 
    /*000008b4*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c8] 
    /*000008b8*/ LDR         R3, [PC, #144]              @ 0x0000000000000950 
    /*000008bc*/ BL          0x00000000000008BC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000008c0*/ MOVW        R0, #32776                  @ 0x8008 
    /*000008c4*/ MOVT        R0, #40963                  @ 0xa003 
    /*000008c8*/ ADD         SP, SP, #12 
    /*000008cc*/ POP         {R4, R5, PC} 
    RgnDisplayAttrToChnAttr_x8d0:
    /*000008d0*/ CMP         R3, #1 
    /*000008d4*/ BNE         RgnDisplayAttrToChnAttr_x7f8
    /*000008d8*/ MOV         R2, #40                     @ 0x28 
    /*000008dc*/ ADD         R1, R4, #12 
    /*000008e0*/ ADD         R0, R5, #24 
    /*000008e4*/ BL          0x00000000000008E4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000008e8*/ B           RgnDisplayAttrToChnAttr_x7f8
    RgnDisplayAttrToChnAttr_x8ec:
    /*000008ec*/ MOVW        R3, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x220] 
    /*000008f0*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000008f4*/ MOVT        R3, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x220] 
    /*000008f8*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000008fc*/ MOV         R2, #252                    @ 0xfc 
    /*00000900*/ LDR         R1, [PC, #72]               @ 0x0000000000000950 
    /*00000904*/ BL          0x0000000000000904          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000908*/ B           RgnDisplayAttrToChnAttr_x6c0
    RgnDisplayAttrToChnAttr_x90c:
    /*0000090c*/ MOVW        R3, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x234] 
    /*00000910*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000914*/ MOVT        R3, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x234] 
    /*00000918*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*0000091c*/ MOV         R2, #253                    @ 0xfd 
    /*00000920*/ LDR         R1, [PC, #40]               @ 0x0000000000000950 
    /*00000924*/ BL          0x0000000000000924          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000928*/ B           RgnDisplayAttrToChnAttr_x6c8
    RgnDisplayAttrToChnAttr_x92c:
    /*0000092c*/ LDR         R3, [R4, #12] 
    /*00000930*/ STR         R3, [R5, #8] 
    /*00000934*/ LDR         R3, [R4, #16] 
    /*00000938*/ STR         R3, [R5, #12] 
    /*0000093c*/ LDR         R3, [R4, #20] 
    /*00000940*/ STR         R3, [R5, #16] 
    /*00000944*/ LDR         R3, [R4, #24] 
    /*00000948*/ STR         R3, [R5, #20] 
    /*0000094c*/ B           RgnDisplayAttrToChnAttr_x754
    /*00000950*/ ANDEQ       R0, R0, IP, LSL #1 
FUNC_END RgnDisplayAttrToChnAttr


FUNC_BEGIN RgnChnAttrToDisplayAttr
    /*00000954*/ PUSH        {R4, R5, R6, LR} 
    /*00000958*/ SUBS        R5, R2, #0 
    /*0000095c*/ SUB         SP, SP, #8 
    /*00000960*/ MOV         R6, R0 
    /*00000964*/ MOV         R4, R1 
    /*00000968*/ BEQ         RgnChnAttrToDisplayAttr_xb5c
    RgnChnAttrToDisplayAttr_x96c:
    /*0000096c*/ CMP         R4, #0 
    /*00000970*/ BEQ         RgnChnAttrToDisplayAttr_xb7c
    RgnChnAttrToDisplayAttr_x974:
    /*00000974*/ CMP         R6, #4 
    /*00000978*/ BHI         RgnChnAttrToDisplayAttr_x9a8
    RgnChnAttrToDisplayAttr_x97c:
    /*0000097c*/ STR         R6, [R5, #4] 
    /*00000980*/ LDR         R3, [R4] 
    /*00000984*/ STR         R3, [R5] 
    /*00000988*/ CMP         R6, #4 
    /*0000098c*/ LDRLS       PC, [PC, R6, LSL #2] 
    /*00000990*/ B           RgnChnAttrToDisplayAttr_xb28
    /*00000994*/ ANDEQ       R0, R0, R4, LSL #20 
    /*00000998*/ ANDEQ       R0, R0, R0, ROR #20 
    /*0000099c*/ ANDEQ       R0, R0, R0, LSR #21 
    /*000009a0*/ ANDEQ       R0, R0, IP, ROR #21 
    /*000009a4*/ ANDEQ       R0, R0, R8, ASR #19 
    RgnChnAttrToDisplayAttr_x9a8:
    /*000009a8*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fc] 
    /*000009ac*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000009b0*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fc] 
    /*000009b4*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000009b8*/ MOV         R2, #364                    @ 0x16c 
    /*000009bc*/ LDR         R1, [PC, #496]              @ 0x0000000000000bb4 
    /*000009c0*/ BL          0x00000000000009C0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000009c4*/ B           RgnChnAttrToDisplayAttr_x97c
    /*000009c8*/ LDR         R3, [R4, #8] 
    /*000009cc*/ MOV         R0, #0 
    /*000009d0*/ STR         R3, [R5, #8] 
    /*000009d4*/ LDR         R3, [R4, #12] 
    /*000009d8*/ STR         R3, [R5, #12] 
    /*000009dc*/ LDR         R3, [R4, #16] 
    /*000009e0*/ STR         R3, [R5, #16] 
    /*000009e4*/ LDR         R3, [R4, #20] 
    /*000009e8*/ STR         R3, [R5, #20] 
    /*000009ec*/ LDR         R3, [R4, #64]               @ 0x40 
    /*000009f0*/ STR         R3, [R5, #28] 
    /*000009f4*/ LDR         R3, [R4, #156]              @ 0x9c 
    /*000009f8*/ STR         R3, [R5, #24] 
    RgnChnAttrToDisplayAttr_x9fc:
    /*000009fc*/ ADD         SP, SP, #8 
    /*00000a00*/ POP         {R4, R5, R6, PC} 
    /*00000a04*/ MOV         R2, #8 
    /*00000a08*/ ADD         R1, R4, R2 
    /*00000a0c*/ ADD         R0, R5, R2 
    /*00000a10*/ BL          0x0000000000000A10          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000a14*/ LDR         R3, [R4, #76]               @ 0x4c 
    /*00000a18*/ ADD         R1, R4, #120                @ 0x78 
    /*00000a1c*/ MOV         R2, #12 
    /*00000a20*/ ADD         R0, R5, #28 
    /*00000a24*/ STR         R3, [R5, #16] 
    /*00000a28*/ LDR         R3, [R4, #80]               @ 0x50 
    /*00000a2c*/ STR         R3, [R5, #20] 
    /*00000a30*/ LDR         R3, [R4, #64]               @ 0x40 
    /*00000a34*/ STR         R3, [R5, #24] 
    /*00000a38*/ BL          0x0000000000000A38          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000a3c*/ LDR         R3, [R4, #152]              @ 0x98 
    /*00000a40*/ ADD         R1, R4, #164                @ 0xa4 
    /*00000a44*/ ADD         R0, R5, #64                 @ 0x40 
    /*00000a48*/ MOV         R2, #4 
    /*00000a4c*/ STR         R3, [R5, #60]               @ 0x3c 
    /*00000a50*/ BL          0x0000000000000A50          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000a54*/ MOV         R0, #0 
    /*00000a58*/ ADD         SP, SP, #8 
    /*00000a5c*/ POP         {R4, R5, R6, PC} 
    /*00000a60*/ LDR         R3, [R4, #4] 
    /*00000a64*/ STR         R3, [R5, #8] 
    /*00000a68*/ LDR         R3, [R4, #68]               @ 0x44 
    /*00000a6c*/ STR         R3, [R5, #52]               @ 0x34 
    /*00000a70*/ LDR         R3, [R4, #64]               @ 0x40 
    /*00000a74*/ STR         R3, [R5, #56]               @ 0x38 
    /*00000a78*/ LDR         R3, [R4, #160]              @ 0xa0 
    /*00000a7c*/ STR         R3, [R5, #60]               @ 0x3c 
    /*00000a80*/ LDR         R0, [R4, #4] 
    /*00000a84*/ CMP         R0, #0 
    /*00000a88*/ BEQ         RgnChnAttrToDisplayAttr_xac4
    RgnChnAttrToDisplayAttr_xa8c:
    /*00000a8c*/ CMP         R0, #1 
    /*00000a90*/ BEQ         RgnChnAttrToDisplayAttr_xb9c
    RgnChnAttrToDisplayAttr_xa94:
    /*00000a94*/ MOV         R0, #0 
    /*00000a98*/ ADD         SP, SP, #8 
    /*00000a9c*/ POP         {R4, R5, R6, PC} 
    /*00000aa0*/ LDR         R3, [R4, #4] 
    /*00000aa4*/ STR         R3, [R5, #8] 
    /*00000aa8*/ LDR         R3, [R4, #68]               @ 0x44 
    /*00000aac*/ STR         R3, [R5, #52]               @ 0x34 
    /*00000ab0*/ LDR         R3, [R4, #64]               @ 0x40 
    /*00000ab4*/ STR         R3, [R5, #56]               @ 0x38 
    /*00000ab8*/ LDR         R0, [R4, #4] 
    /*00000abc*/ CMP         R0, #0 
    /*00000ac0*/ BNE         RgnChnAttrToDisplayAttr_xa8c
    RgnChnAttrToDisplayAttr_xac4:
    /*00000ac4*/ LDR         R3, [R4, #8] 
    /*00000ac8*/ STR         R3, [R5, #12] 
    /*00000acc*/ LDR         R3, [R4, #12] 
    /*00000ad0*/ STR         R3, [R5, #16] 
    /*00000ad4*/ LDR         R3, [R4, #16] 
    /*00000ad8*/ STR         R3, [R5, #20] 
    /*00000adc*/ LDR         R3, [R4, #20] 
    /*00000ae0*/ STR         R3, [R5, #24] 
    /*00000ae4*/ ADD         SP, SP, #8 
    /*00000ae8*/ POP         {R4, R5, R6, PC} 
    /*00000aec*/ MOV         R2, #8 
    /*00000af0*/ ADD         R1, R4, R2 
    /*00000af4*/ ADD         R0, R5, R2 
    /*00000af8*/ BL          0x0000000000000AF8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000afc*/ LDR         R3, [R4, #76]               @ 0x4c 
    /*00000b00*/ ADD         R1, R4, #164                @ 0xa4 
    /*00000b04*/ ADD         R0, R5, #28 
    /*00000b08*/ MOV         R2, #4 
    /*00000b0c*/ STR         R3, [R5, #16] 
    /*00000b10*/ LDR         R3, [R4, #80]               @ 0x50 
    /*00000b14*/ STR         R3, [R5, #20] 
    /*00000b18*/ LDR         R3, [R4, #64]               @ 0x40 
    /*00000b1c*/ STR         R3, [R5, #24] 
    /*00000b20*/ BL          0x0000000000000B20          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000b24*/ B           RgnChnAttrToDisplayAttr_xa94
    RgnChnAttrToDisplayAttr_xb28:
    /*00000b28*/ MOV         R1, #3 
    /*00000b2c*/ STR         R6, [SP, #4] 
    /*00000b30*/ MOV         R0, R1 
    /*00000b34*/ MOVW        R3, #453                    @ 0x1c5 
    /*00000b38*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c8] 
    /*00000b3c*/ STR         R3, [SP] 
    /*00000b40*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c8] 
    /*00000b44*/ LDR         R3, [PC, #104]              @ 0x0000000000000bb4 
    /*00000b48*/ BL          0x0000000000000B48          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000b4c*/ MOVW        R0, #32776                  @ 0x8008 
    /*00000b50*/ MOVT        R0, #40963                  @ 0xa003 
    /*00000b54*/ ADD         SP, SP, #8 
    /*00000b58*/ POP         {R4, R5, R6, PC} 
    RgnChnAttrToDisplayAttr_xb5c:
    /*00000b5c*/ MOVW        R3, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x220] 
    /*00000b60*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000b64*/ MOVT        R3, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x220] 
    /*00000b68*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000b6c*/ MOVW        R2, #362                    @ 0x16a 
    /*00000b70*/ LDR         R1, [PC, #60]               @ 0x0000000000000bb4 
    /*00000b74*/ BL          0x0000000000000B74          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000b78*/ B           RgnChnAttrToDisplayAttr_x96c
    RgnChnAttrToDisplayAttr_xb7c:
    /*00000b7c*/ MOVW        R3, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x234] 
    /*00000b80*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000b84*/ MOVT        R3, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x234] 
    /*00000b88*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000b8c*/ MOVW        R2, #363                    @ 0x16b 
    /*00000b90*/ LDR         R1, [PC, #28]               @ 0x0000000000000bb4 
    /*00000b94*/ BL          0x0000000000000B94          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000b98*/ B           RgnChnAttrToDisplayAttr_x974
    RgnChnAttrToDisplayAttr_xb9c:
    /*00000b9c*/ ADD         R1, R4, #24 
    /*00000ba0*/ ADD         R0, R5, #12 
    /*00000ba4*/ MOV         R2, #40                     @ 0x28 
    /*00000ba8*/ BL          0x0000000000000BA8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00000bac*/ MOV         R0, #0 
    /*00000bb0*/ B           RgnChnAttrToDisplayAttr_x9fc
    /*00000bb4*/ ANDEQ       R0, R0, R4, LSR #1 
FUNC_END RgnChnAttrToDisplayAttr


FUNC_BEGIN RgnIsBmpCoverRgn
    /*00000bb8*/ CMP         R3, R1 
    /*00000bbc*/ CMPGE       R2, R0 
    /*00000bc0*/ MOVLT       R0, #1 
    /*00000bc4*/ MOVGE       R0, #0 
    /*00000bc8*/ EOR         R0, R0, #1 
    /*00000bcc*/ BX          LR 
FUNC_END RgnIsBmpCoverRgn


FUNC_BEGIN RgnMemSet
    /*00000bd0*/ CMP         R2, #2 
    /*00000bd4*/ BEQ         RgnMemSet_xc38
    /*00000bd8*/ CMP         R2, #4 
    /*00000bdc*/ BEQ         RgnMemSet_xc5c
    /*00000be0*/ CMP         R2, #1 
    /*00000be4*/ BEQ         RgnMemSet_xc04
    /*00000be8*/ MOVW        R3, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x284] 
    /*00000bec*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000bf0*/ MOVT        R3, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x284] 
    /*00000bf4*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000bf8*/ MOV         R2, #508                    @ 0x1fc 
    /*00000bfc*/ LDR         R1, [PC, #120]              @ 0x0000000000000c7c 
    RgnMemSet_xc00:
    /*00000c00*/ B           RgnMemSet_xc00              @ osal_panic [R_ARM_JUMP24: +0x0] 
    RgnMemSet_xc04:
    /*00000c04*/ ADD         R3, R3, #3 
    /*00000c08*/ LSL         R2, R1, #4 
    /*00000c0c*/ ADD         R2, R2, R1, LSL #2 
    /*00000c10*/ LSRS        R3, R3, #2 
    /*00000c14*/ ADD         R2, R2, R1 
    /*00000c18*/ ADD         R1, R2, R1, LSL #6 
    /*00000c1c*/ BXEQ        LR 
    /*00000c20*/ UXTB        R1, R1 
    /*00000c24*/ ADD         R3, R0, R3 
    RgnMemSet_xc28:
    /*00000c28*/ STRB        R1, [R0], #1 
    /*00000c2c*/ CMP         R3, R0 
    /*00000c30*/ BNE         RgnMemSet_xc28
    /*00000c34*/ BX          LR 
    RgnMemSet_xc38:
    /*00000c38*/ CMP         R3, #0 
    /*00000c3c*/ BXEQ        LR 
    /*00000c40*/ UXTH        R1, R1 
    /*00000c44*/ MOV         R2, #0 
    RgnMemSet_xc48:
    /*00000c48*/ ADD         R2, R2, #1 
    /*00000c4c*/ STRH        R1, [R0], #2 
    /*00000c50*/ CMP         R3, R2 
    /*00000c54*/ BNE         RgnMemSet_xc48
    /*00000c58*/ BX          LR 
    RgnMemSet_xc5c:
    /*00000c5c*/ CMP         R3, #0 
    /*00000c60*/ BXEQ        LR 
    /*00000c64*/ MOV         R2, #0 
    RgnMemSet_xc68:
    /*00000c68*/ ADD         R2, R2, #1 
    /*00000c6c*/ STR         R1, [R0], #4 
    /*00000c70*/ CMP         R3, R2 
    /*00000c74*/ BNE         RgnMemSet_xc68
    /*00000c78*/ BX          LR 
    /*00000c7c*/ STRHEQ      R0, [R0], -IP 
FUNC_END RgnMemSet


FUNC_BEGIN RGN_LockInit
    /*00000c80*/ PUSH        {R4, LR} 
    /*00000c84*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000c88*/ MOV         R1, #1 
    /*00000c8c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000c90*/ BL          0x0000000000000C90          @ osal_sema_init [R_ARM_CALL: +0x0] 
    /*00000c94*/ SUBS        R4, R0, #0 
    /*00000c98*/ BNE         RGN_LockInit_xcdc
    /*00000c9c*/ LDR         R0, [PC, #84]               @ 0x0000000000000cf8 
    /*00000ca0*/ BL          0x0000000000000CA0          @ osal_spin_lock_init [R_ARM_CALL: +0x0] 
    /*00000ca4*/ SUBS        R4, R0, #0 
    /*00000ca8*/ BNE         RGN_LockInit_xcb4
    /*00000cac*/ MOV         R0, R4 
    /*00000cb0*/ POP         {R4, PC} 
    RGN_LockInit_xcb4:
    /*00000cb4*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000cb8*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000cbc*/ BL          0x0000000000000CBC          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*00000cc0*/ MOV         R1, #3 
    /*00000cc4*/ MOVW        R2, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a4] 
    /*00000cc8*/ MOV         R0, #1 
    /*00000ccc*/ MOVT        R2, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a4] 
    /*00000cd0*/ BL          0x0000000000000CD0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000cd4*/ MOV         R0, R4 
    /*00000cd8*/ POP         {R4, PC} 
    RGN_LockInit_xcdc:
    /*00000cdc*/ MOV         R1, #3 
    /*00000ce0*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x288] 
    /*00000ce4*/ MOV         R0, #1 
    /*00000ce8*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x288] 
    /*00000cec*/ BL          0x0000000000000CEC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000cf0*/ MOV         R0, R4 
    /*00000cf4*/ POP         {R4, PC} 
    /*00000cf8*/ ANDEQ       R0, R0, R4 
FUNC_END RGN_LockInit


FUNC_BEGIN RGN_LockExit
    /*00000cfc*/ PUSH        {R4, LR} 
    /*00000d00*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000d04*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000d08*/ MOV         R0, R4 
    /*00000d0c*/ BL          0x0000000000000D0C          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*00000d10*/ ADD         R0, R4, #4 
    /*00000d14*/ BL          0x0000000000000D14          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00000d18*/ MOV         R0, #0 
    /*00000d1c*/ POP         {R4, PC} 
FUNC_END RGN_LockExit


FUNC_BEGIN RgnGetpstModInfo
    /*00000d20*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000d24*/ SUBS        R8, R1, #0 
    /*00000d28*/ MOV         R7, R0 
    /*00000d2c*/ BEQ         RgnGetpstModInfo_xe84
    RgnGetpstModInfo_xd30:
    /*00000d30*/ CMP         R7, #4 
    /*00000d34*/ BHI         RgnGetpstModInfo_xe14
    /*00000d38*/ LDR         R5, [R8] 
    /*00000d3c*/ CMP         R5, #59                     @ 0x3b 
    /*00000d40*/ BHI         RgnGetpstModInfo_xe3c
    RgnGetpstModInfo_xd44:
    /*00000d44*/ MOV         R2, #20 
    /*00000d48*/ MOV         R3, #1200                   @ 0x4b0 
    /*00000d4c*/ MUL         R2, R2, R5 
    /*00000d50*/ LDR         R1, [R8, #4] 
    /*00000d54*/ MLA         R3, R3, R7, R2 
    /*00000d58*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000d5c*/ CMP         R1, #0 
    /*00000d60*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000d64*/ ADD         R6, R4, #580                @ 0x244 
    /*00000d68*/ ADD         R6, R6, R3 
    /*00000d6c*/ BLT         RgnGetpstModInfo_xdf4
    /*00000d70*/ ADD         R3, R4, R3 
    /*00000d74*/ LDR         R3, [R3, #580]              @ 0x244 
    /*00000d78*/ CMP         R1, R3 
    /*00000d7c*/ BCS         RgnGetpstModInfo_xdf4
    RgnGetpstModInfo_xd80:
    /*00000d80*/ LDR         R1, [R8, #8] 
    /*00000d84*/ CMP         R1, #0 
    /*00000d88*/ BLT         RgnGetpstModInfo_xdd4
    /*00000d8c*/ MOV         R2, #1200                   @ 0x4b0 
    /*00000d90*/ MOV         R3, #20 
    /*00000d94*/ MUL         R2, R2, R7 
    /*00000d98*/ MLA         R3, R3, R5, R2 
    /*00000d9c*/ ADD         R3, R4, R3 
    /*00000da0*/ LDR         R3, [R3, #584]              @ 0x248 
    /*00000da4*/ CMP         R1, R3 
    /*00000da8*/ BCS         RgnGetpstModInfo_xdd4
    RgnGetpstModInfo_xdac:
    /*00000dac*/ MOV         R3, #1200                   @ 0x4b0 
    /*00000db0*/ MOV         R2, #20 
    /*00000db4*/ MUL         R7, R3, R7 
    /*00000db8*/ MLA         R5, R2, R5, R7 
    /*00000dbc*/ ADD         R4, R4, R5 
    /*00000dc0*/ LDR         R3, [R4, #592]              @ 0x250 
    /*00000dc4*/ CMP         R3, #0 
    /*00000dc8*/ BEQ         RgnGetpstModInfo_xe60
    /*00000dcc*/ MOV         R0, R6 
    /*00000dd0*/ POP         {R4, R5, R6, R7, R8, PC} 
    RgnGetpstModInfo_xdd4:
    /*00000dd4*/ MOVW        R3, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x350] 
    /*00000dd8*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000ddc*/ MOVT        R3, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x350] 
    /*00000de0*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000de4*/ MOVW        R2, #553                    @ 0x229 
    /*00000de8*/ LDR         R1, [PC, #180]              @ 0x0000000000000ea4 
    /*00000dec*/ BL          0x0000000000000DEC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000df0*/ B           RgnGetpstModInfo_xdac
    RgnGetpstModInfo_xdf4:
    /*00000df4*/ MOVW        R3, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30c] 
    /*00000df8*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000dfc*/ MOVT        R3, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30c] 
    /*00000e00*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000e04*/ MOV         R2, #552                    @ 0x228 
    /*00000e08*/ LDR         R1, [PC, #148]              @ 0x0000000000000ea4 
    /*00000e0c*/ BL          0x0000000000000E0C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000e10*/ B           RgnGetpstModInfo_xd80
    RgnGetpstModInfo_xe14:
    /*00000e14*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fc] 
    /*00000e18*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000e1c*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fc] 
    /*00000e20*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000e24*/ MOV         R2, #548                    @ 0x224 
    /*00000e28*/ LDR         R1, [PC, #116]              @ 0x0000000000000ea4 
    /*00000e2c*/ BL          0x0000000000000E2C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000e30*/ LDR         R5, [R8] 
    /*00000e34*/ CMP         R5, #59                     @ 0x3b 
    /*00000e38*/ BLS         RgnGetpstModInfo_xd44
    RgnGetpstModInfo_xe3c:
    /*00000e3c*/ MOVW        R3, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2d8] 
    /*00000e40*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000e44*/ MOVT        R3, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2d8] 
    /*00000e48*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000e4c*/ MOVW        R2, #549                    @ 0x225 
    /*00000e50*/ LDR         R1, [PC, #76]               @ 0x0000000000000ea4 
    /*00000e54*/ BL          0x0000000000000E54          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000e58*/ LDR         R5, [R8] 
    /*00000e5c*/ B           RgnGetpstModInfo_xd44
    RgnGetpstModInfo_xe60:
    /*00000e60*/ MOVW        R2, #555                    @ 0x22b 
    /*00000e64*/ LDR         R1, [PC, #56]               @ 0x0000000000000ea4 
    /*00000e68*/ MOVW        R3, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x394] 
    /*00000e6c*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000e70*/ MOVT        R3, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x394] 
    /*00000e74*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000e78*/ BL          0x0000000000000E78          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000e7c*/ MOV         R0, R6 
    /*00000e80*/ POP         {R4, R5, R6, R7, R8, PC} 
    RgnGetpstModInfo_xe84:
    /*00000e84*/ MOVW        R3, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c4] 
    /*00000e88*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000e8c*/ MOVT        R3, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c4] 
    /*00000e90*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000e94*/ MOVW        R2, #547                    @ 0x223 
    /*00000e98*/ LDR         R1, [PC, #4]                @ 0x0000000000000ea4 
    /*00000e9c*/ BL          0x0000000000000E9C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000ea0*/ B           RgnGetpstModInfo_xd30
    /*00000ea4*/ ANDEQ       R0, R0, R8, ASR #1 
FUNC_END RgnGetpstModInfo


FUNC_BEGIN RgnGetIndex
    /*00000ea8*/ PUSH        {R4, LR} 
    /*00000eac*/ MOV         R4, R1 
    /*00000eb0*/ BL          0x0000000000000EB0          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*00000eb4*/ LDMIB       R4, {R2, R3} 
    /*00000eb8*/ LDR         R0, [R0, #4] 
    /*00000ebc*/ MLA         R0, R0, R2, R3 
    /*00000ec0*/ POP         {R4, PC} 
FUNC_END RgnGetIndex


FUNC_BEGIN RgnAllocChnInfo
    /*00000ec4*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00000ec8*/ SUBS        R4, R1, #0 
    /*00000ecc*/ SUB         SP, SP, #12 
    /*00000ed0*/ MOV         R5, R0 
    /*00000ed4*/ BEQ         RgnAllocChnInfo_xfb4
    RgnAllocChnInfo_xed8:
    /*00000ed8*/ CMP         R5, #4 
    /*00000edc*/ BHI         RgnAllocChnInfo_xf38
    /*00000ee0*/ LDR         R3, [R4] 
    /*00000ee4*/ CMP         R3, #59                     @ 0x3b 
    /*00000ee8*/ BHI         RgnAllocChnInfo_xf60
    RgnAllocChnInfo_xeec:
    /*00000eec*/ MOV         R1, R4 
    /*00000ef0*/ MOV         R0, R5 
    /*00000ef4*/ BL          0x0000000000000EF4          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*00000ef8*/ SUBS        R7, R0, #0 
    /*00000efc*/ BEQ         RgnAllocChnInfo_xfd4
    /*00000f00*/ MOV         R0, R5 
    /*00000f04*/ MOV         R1, R4 
    /*00000f08*/ BL          0x0000000000000F08          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*00000f0c*/ LDR         R5, [R4, #4] 
    /*00000f10*/ LDR         R2, [R4, #8] 
    /*00000f14*/ LDR         R3, [R7, #16] 
    /*00000f18*/ LDR         R1, [R0, #4] 
    /*00000f1c*/ MLA         R5, R1, R5, R2 
    /*00000f20*/ LDR         R6, [R3, R5, LSL #2] 
    /*00000f24*/ CMP         R6, #0 
    /*00000f28*/ BEQ         RgnAllocChnInfo_xf80
    RgnAllocChnInfo_xf2c:
    /*00000f2c*/ MOV         R0, R6 
    /*00000f30*/ ADD         SP, SP, #12 
    /*00000f34*/ POP         {R4, R5, R6, R7, PC} 
    RgnAllocChnInfo_xf38:
    /*00000f38*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fc] 
    /*00000f3c*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000f40*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fc] 
    /*00000f44*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000f48*/ MOVW        R2, #579                    @ 0x243 
    /*00000f4c*/ LDR         R1, [PC, #220]              @ 0x0000000000001030 
    /*00000f50*/ BL          0x0000000000000F50          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000f54*/ LDR         R3, [R4] 
    /*00000f58*/ CMP         R3, #59                     @ 0x3b 
    /*00000f5c*/ BLS         RgnAllocChnInfo_xeec
    RgnAllocChnInfo_xf60:
    /*00000f60*/ MOVW        R3, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2d8] 
    /*00000f64*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000f68*/ MOVT        R3, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2d8] 
    /*00000f6c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000f70*/ MOV         R2, #580                    @ 0x244 
    /*00000f74*/ LDR         R1, [PC, #180]              @ 0x0000000000001030 
    /*00000f78*/ BL          0x0000000000000F78          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000f7c*/ B           RgnAllocChnInfo_xeec
    RgnAllocChnInfo_xf80:
    /*00000f80*/ MOV         R1, R6 
    /*00000f84*/ MOV         R0, #200                    @ 0xc8 
    /*00000f88*/ BL          0x0000000000000F88          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*00000f8c*/ SUBS        R6, R0, #0 
    /*00000f90*/ BEQ         RgnAllocChnInfo_x1004
    /*00000f94*/ MOV         R2, #200                    @ 0xc8 
    /*00000f98*/ MOV         R1, #0 
    /*00000f9c*/ BL          0x0000000000000F9C          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00000fa0*/ LDR         R3, [R7, #16] 
    /*00000fa4*/ STR         R6, [R3, R5, LSL #2] 
    /*00000fa8*/ LDR         R3, [R7, #16] 
    /*00000fac*/ LDR         R6, [R3, R5, LSL #2] 
    /*00000fb0*/ B           RgnAllocChnInfo_xf2c
    RgnAllocChnInfo_xfb4:
    /*00000fb4*/ MOVW        R3, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c4] 
    /*00000fb8*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00000fbc*/ MOVT        R3, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c4] 
    /*00000fc0*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00000fc4*/ MOVW        R2, #578                    @ 0x242 
    /*00000fc8*/ LDR         R1, [PC, #96]               @ 0x0000000000001030 
    /*00000fcc*/ BL          0x0000000000000FCC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000fd0*/ B           RgnAllocChnInfo_xed8
    RgnAllocChnInfo_xfd4:
    /*00000fd4*/ LDR         R3, [R4] 
    /*00000fd8*/ MOV         R1, #3 
    /*00000fdc*/ MOVW        IP, #585                    @ 0x249 
    /*00000fe0*/ MOV         R0, R1 
    /*00000fe4*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b8] 
    /*00000fe8*/ STR         IP, [SP] 
    /*00000fec*/ STR         R3, [SP, #4] 
    /*00000ff0*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b8] 
    /*00000ff4*/ LDR         R3, [PC, #52]               @ 0x0000000000001030 
    /*00000ff8*/ MOV         R6, R7 
    /*00000ffc*/ BL          0x0000000000000FFC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001000*/ B           RgnAllocChnInfo_xf2c
    RgnAllocChnInfo_x1004:
    /*00001004*/ LDR         R3, [R4] 
    /*00001008*/ MOV         R1, #3 
    /*0000100c*/ MOVW        IP, #598                    @ 0x256 
    /*00001010*/ MOV         R0, R1 
    /*00001014*/ MOVW        R2, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3ec] 
    /*00001018*/ STR         IP, [SP] 
    /*0000101c*/ STR         R3, [SP, #4] 
    /*00001020*/ MOVT        R2, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3ec] 
    /*00001024*/ LDR         R3, [PC, #4]                @ 0x0000000000001030 
    /*00001028*/ BL          0x0000000000001028          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000102c*/ B           RgnAllocChnInfo_xf2c
    /*00001030*/ LDRDEQ      R0, [R0], -IP 
FUNC_END RgnAllocChnInfo


FUNC_BEGIN RgnGetChnGetInfo
    /*00001034*/ PUSH        {R4, R5, R6, LR} 
    /*00001038*/ MOV         R4, R1 
    /*0000103c*/ MOV         R6, R0 
    /*00001040*/ BL          0x0000000000001040          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*00001044*/ MOV         R1, R4 
    /*00001048*/ MOV         R5, R0 
    /*0000104c*/ MOV         R0, R6 
    /*00001050*/ BL          0x0000000000001050          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*00001054*/ LDR         R3, [R4, #4] 
    /*00001058*/ LDR         R1, [R4, #8] 
    /*0000105c*/ LDR         R2, [R5, #16] 
    /*00001060*/ LDR         R0, [R0, #4] 
    /*00001064*/ MLA         R3, R0, R3, R1 
    /*00001068*/ LDR         R0, [R2, R3, LSL #2] 
    /*0000106c*/ POP         {R4, R5, R6, PC} 
FUNC_END RgnGetChnGetInfo


FUNC_BEGIN RgnAllocChnManage
    /*00001070*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00001074*/ SUBS        R4, R1, #0 
    /*00001078*/ SUB         SP, SP, #12 
    /*0000107c*/ MOV         R5, R0 
    /*00001080*/ BEQ         RgnAllocChnManage_x117c
    RgnAllocChnManage_x1084:
    /*00001084*/ CMP         R5, #4 
    /*00001088*/ BHI         RgnAllocChnManage_x10e4
    /*0000108c*/ LDR         R3, [R4] 
    /*00001090*/ CMP         R3, #59                     @ 0x3b 
    /*00001094*/ BHI         RgnAllocChnManage_x110c
    RgnAllocChnManage_x1098:
    /*00001098*/ MOV         R1, R4 
    /*0000109c*/ MOV         R0, R5 
    /*000010a0*/ BL          0x00000000000010A0          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*000010a4*/ SUBS        R7, R0, #0 
    /*000010a8*/ BEQ         RgnAllocChnManage_x119c
    /*000010ac*/ MOV         R1, R4 
    /*000010b0*/ MOV         R0, R5 
    /*000010b4*/ BL          0x00000000000010B4          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*000010b8*/ LDR         R3, [R4, #4] 
    /*000010bc*/ LDR         R1, [R4, #8] 
    /*000010c0*/ LDR         R2, [R7, #12] 
    /*000010c4*/ LDR         R0, [R0, #4] 
    /*000010c8*/ MLA         R3, R0, R3, R1 
    /*000010cc*/ LDR         R6, [R2, R3, LSL #2] 
    /*000010d0*/ CMP         R6, #0 
    /*000010d4*/ BEQ         RgnAllocChnManage_x112c
    RgnAllocChnManage_x10d8:
    /*000010d8*/ MOV         R0, R6 
    /*000010dc*/ ADD         SP, SP, #12 
    /*000010e0*/ POP         {R4, R5, R6, R7, PC} 
    RgnAllocChnManage_x10e4:
    /*000010e4*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fc] 
    /*000010e8*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000010ec*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fc] 
    /*000010f0*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000010f4*/ MOVW        R2, #629                    @ 0x275 
    /*000010f8*/ LDR         R1, [PC, #248]              @ 0x00000000000011f8 
    /*000010fc*/ BL          0x00000000000010FC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001100*/ LDR         R3, [R4] 
    /*00001104*/ CMP         R3, #59                     @ 0x3b 
    /*00001108*/ BLS         RgnAllocChnManage_x1098
    RgnAllocChnManage_x110c:
    /*0000110c*/ MOVW        R3, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2d8] 
    /*00001110*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001114*/ MOVT        R3, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2d8] 
    /*00001118*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*0000111c*/ MOVW        R2, #630                    @ 0x276 
    /*00001120*/ LDR         R1, [PC, #208]              @ 0x00000000000011f8 
    /*00001124*/ BL          0x0000000000001124          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001128*/ B           RgnAllocChnManage_x1098
    RgnAllocChnManage_x112c:
    /*0000112c*/ MOV         R0, R5 
    /*00001130*/ MOV         R1, R4 
    /*00001134*/ BL          0x0000000000001134          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*00001138*/ LDR         R5, [R4, #4] 
    /*0000113c*/ LDR         R3, [R4, #8] 
    /*00001140*/ MOV         R1, R6 
    /*00001144*/ LDR         R2, [R0, #4] 
    /*00001148*/ MOVW        R0, #4440                   @ 0x1158 
    /*0000114c*/ MLA         R5, R2, R5, R3 
    /*00001150*/ BL          0x0000000000001150          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*00001154*/ SUBS        R6, R0, #0 
    /*00001158*/ BEQ         RgnAllocChnManage_x11cc
    /*0000115c*/ MOVW        R2, #4440                   @ 0x1158 
    /*00001160*/ MOV         R1, #0 
    /*00001164*/ BL          0x0000000000001164          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00001168*/ LDR         R3, [R7, #12] 
    /*0000116c*/ STR         R6, [R3, R5, LSL #2] 
    /*00001170*/ LDR         R3, [R7, #12] 
    /*00001174*/ LDR         R6, [R3, R5, LSL #2] 
    /*00001178*/ B           RgnAllocChnManage_x10d8
    RgnAllocChnManage_x117c:
    /*0000117c*/ MOVW        R3, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c4] 
    /*00001180*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001184*/ MOVT        R3, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c4] 
    /*00001188*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*0000118c*/ MOV         R2, #628                    @ 0x274 
    /*00001190*/ LDR         R1, [PC, #96]               @ 0x00000000000011f8 
    /*00001194*/ BL          0x0000000000001194          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001198*/ B           RgnAllocChnManage_x1084
    RgnAllocChnManage_x119c:
    /*0000119c*/ LDR         R3, [R4] 
    /*000011a0*/ MOV         R1, #3 
    /*000011a4*/ MOVW        IP, #635                    @ 0x27b 
    /*000011a8*/ MOV         R0, R1 
    /*000011ac*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b8] 
    /*000011b0*/ STR         IP, [SP] 
    /*000011b4*/ STR         R3, [SP, #4] 
    /*000011b8*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b8] 
    /*000011bc*/ LDR         R3, [PC, #52]               @ 0x00000000000011f8 
    /*000011c0*/ MOV         R6, R7 
    /*000011c4*/ BL          0x00000000000011C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000011c8*/ B           RgnAllocChnManage_x10d8
    RgnAllocChnManage_x11cc:
    /*000011cc*/ LDR         R3, [R4] 
    /*000011d0*/ MOV         R1, #3 
    /*000011d4*/ MOV         IP, #652                    @ 0x28c 
    /*000011d8*/ MOV         R0, R1 
    /*000011dc*/ MOVW        R2, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3ec] 
    /*000011e0*/ STR         IP, [SP] 
    /*000011e4*/ STR         R3, [SP, #4] 
    /*000011e8*/ MOVT        R2, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3ec] 
    /*000011ec*/ LDR         R3, [PC, #4]                @ 0x00000000000011f8 
    /*000011f0*/ BL          0x00000000000011F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000011f4*/ B           RgnAllocChnManage_x10d8
    /*000011f8*/ ANDEQ       R0, R0, IP, ROR #1 
FUNC_END RgnAllocChnManage


FUNC_BEGIN RgnGetChnManage
    /*000011fc*/ PUSH        {R4, R5, R6, LR} 
    /*00001200*/ MOV         R4, R1 
    /*00001204*/ MOV         R6, R0 
    /*00001208*/ BL          0x0000000000001208          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*0000120c*/ MOV         R1, R4 
    /*00001210*/ MOV         R5, R0 
    /*00001214*/ MOV         R0, R6 
    /*00001218*/ BL          0x0000000000001218          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*0000121c*/ LDR         R3, [R4, #4] 
    /*00001220*/ LDR         R1, [R4, #8] 
    /*00001224*/ LDR         R2, [R5, #12] 
    /*00001228*/ LDR         R0, [R0, #4] 
    /*0000122c*/ MLA         R3, R0, R3, R1 
    /*00001230*/ LDR         R0, [R2, R3, LSL #2] 
    /*00001234*/ POP         {R4, R5, R6, PC} 
FUNC_END RgnGetChnManage


FUNC_BEGIN RgnGetChnRgnAttr
    /*00001238*/ PUSH        {R4, LR} 
    /*0000123c*/ MOV         R4, R2 
    /*00001240*/ BL          0x0000000000001240          @ RgnGetChnManage [R_ARM_CALL: .text+0x11fc] 
    /*00001244*/ CMP         R0, #0 
    /*00001248*/ POPEQ       {R4, PC} 
    /*0000124c*/ LDR         LR, [R0] 
    /*00001250*/ CMP         LR, #0 
    /*00001254*/ BEQ         RgnGetChnRgnAttr_x1288
    /*00001258*/ LDR         R3, [R0, #24] 
    /*0000125c*/ CMP         R3, R4 
    /*00001260*/ ADDNE       R1, R0, #24 
    /*00001264*/ MOVNE       R3, #0 
    /*00001268*/ BNE         RgnGetChnRgnAttr_x127c
    /*0000126c*/ B           RgnGetChnRgnAttr_x12a4
    RgnGetChnRgnAttr_x1270:
    /*00001270*/ LDR         IP, [R1, #4]! 
    /*00001274*/ CMP         IP, R4 
    /*00001278*/ BEQ         RgnGetChnRgnAttr_x1290
    RgnGetChnRgnAttr_x127c:
    /*0000127c*/ ADD         R3, R3, #1 
    /*00001280*/ CMP         R3, LR 
    /*00001284*/ BNE         RgnGetChnRgnAttr_x1270
    RgnGetChnRgnAttr_x1288:
    /*00001288*/ MOV         R0, #0 
    /*0000128c*/ POP         {R4, PC} 
    RgnGetChnRgnAttr_x1290:
    /*00001290*/ MOV         LR, #168                    @ 0xa8 
    /*00001294*/ MUL         R3, LR, R3 
    /*00001298*/ ADD         LR, R3, #120                @ 0x78 
    RgnGetChnRgnAttr_x129c:
    /*0000129c*/ ADD         R0, R0, LR 
    /*000012a0*/ POP         {R4, PC} 
    RgnGetChnRgnAttr_x12a4:
    /*000012a4*/ MOV         LR, #120                    @ 0x78 
    /*000012a8*/ B           RgnGetChnRgnAttr_x129c
FUNC_END RgnGetChnRgnAttr


FUNC_BEGIN RgnCheckLastInChnRgnAttrByLayer
    /*000012ac*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000012b0*/ SUB         SP, SP, #12 
    /*000012b4*/ MOV         R4, R0 
    /*000012b8*/ MOV         R7, R1 
    /*000012bc*/ LDR         R0, [PC, #228]              @ 0x00000000000013a8 
    /*000012c0*/ ADD         R1, SP, #4 
    /*000012c4*/ MOV         R5, R2 
    /*000012c8*/ MOV         R6, R3 
    /*000012cc*/ BL          0x00000000000012CC          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000012d0*/ MOV         R1, R7 
    /*000012d4*/ MOV         R0, R4 
    /*000012d8*/ BL          0x00000000000012D8          @ RgnGetChnManage [R_ARM_CALL: .text+0x11fc] 
    /*000012dc*/ CMP         R0, #0 
    /*000012e0*/ BEQ         RgnCheckLastInChnRgnAttrByLayer_x1320
    /*000012e4*/ LDR         R4, [R0] 
    /*000012e8*/ CMP         R4, #0 
    /*000012ec*/ BEQ         RgnCheckLastInChnRgnAttrByLayer_x1320
    /*000012f0*/ LDR         R3, [R0, #24] 
    /*000012f4*/ CMP         R5, R3 
    /*000012f8*/ BEQ         RgnCheckLastInChnRgnAttrByLayer_x1338
    /*000012fc*/ ADD         IP, R0, #24 
    /*00001300*/ MOV         R1, #0 
    /*00001304*/ B           RgnCheckLastInChnRgnAttrByLayer_x1314
    RgnCheckLastInChnRgnAttrByLayer_x1308:
    /*00001308*/ LDR         LR, [IP, #4]! 
    /*0000130c*/ CMP         LR, R5 
    /*00001310*/ BEQ         RgnCheckLastInChnRgnAttrByLayer_x133c
    RgnCheckLastInChnRgnAttrByLayer_x1314:
    /*00001314*/ ADD         R1, R1, #1 
    /*00001318*/ CMP         R1, R4 
    /*0000131c*/ BNE         RgnCheckLastInChnRgnAttrByLayer_x1308
    RgnCheckLastInChnRgnAttrByLayer_x1320:
    /*00001320*/ ADD         R1, SP, #4 
    /*00001324*/ LDR         R0, [PC, #124]              @ 0x00000000000013a8 
    /*00001328*/ BL          0x0000000000001328          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000132c*/ MOV         R0, #0 
    /*00001330*/ ADD         SP, SP, #12 
    /*00001334*/ POP         {R4, R5, R6, R7, PC} 
    RgnCheckLastInChnRgnAttrByLayer_x1338:
    /*00001338*/ MOV         R1, #0 
    RgnCheckLastInChnRgnAttrByLayer_x133c:
    /*0000133c*/ MOV         R3, #168                    @ 0xa8 
    /*00001340*/ MLA         R2, R3, R1, R0 
    /*00001344*/ LDR         IP, [R2, #184]              @ 0xb8 
    /*00001348*/ CMP         IP, R6 
    /*0000134c*/ BNE         RgnCheckLastInChnRgnAttrByLayer_x1320
    /*00001350*/ ADD         R1, R1, #1 
    /*00001354*/ CMP         R4, R1 
    /*00001358*/ BLS         RgnCheckLastInChnRgnAttrByLayer_x1390
    /*0000135c*/ MLA         R0, R3, R1, R0 
    /*00001360*/ LDR         R3, [R0, #184]              @ 0xb8 
    /*00001364*/ CMP         R3, R6 
    /*00001368*/ MOVNE       R3, R2 
    /*0000136c*/ BNE         RgnCheckLastInChnRgnAttrByLayer_x1380
    /*00001370*/ B           RgnCheckLastInChnRgnAttrByLayer_x1320
    RgnCheckLastInChnRgnAttrByLayer_x1374:
    /*00001374*/ LDR         R2, [R3, #352]              @ 0x160 
    /*00001378*/ CMP         IP, R2 
    /*0000137c*/ BEQ         RgnCheckLastInChnRgnAttrByLayer_x1320
    RgnCheckLastInChnRgnAttrByLayer_x1380:
    /*00001380*/ ADD         R1, R1, #1 
    /*00001384*/ ADD         R3, R3, #168                @ 0xa8 
    /*00001388*/ CMP         R4, R1 
    /*0000138c*/ BHI         RgnCheckLastInChnRgnAttrByLayer_x1374
    RgnCheckLastInChnRgnAttrByLayer_x1390:
    /*00001390*/ ADD         R1, SP, #4 
    /*00001394*/ LDR         R0, [PC, #12]               @ 0x00000000000013a8 
    /*00001398*/ BL          0x0000000000001398          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000139c*/ MOV         R0, #1 
    /*000013a0*/ ADD         SP, SP, #12 
    /*000013a4*/ POP         {R4, R5, R6, R7, PC} 
    /*000013a8*/ ANDEQ       R0, R0, R4 
FUNC_END RgnCheckLastInChnRgnAttrByLayer


FUNC_BEGIN RgnCheckMppChn
    /*000013ac*/ PUSH        {R4, R5, LR} 
    /*000013b0*/ SUBS        R4, R1, #0 
    /*000013b4*/ SUB         SP, SP, #20 
    /*000013b8*/ MOV         R5, R0 
    /*000013bc*/ BEQ         RgnCheckMppChn_x1458
    RgnCheckMppChn_x13c0:
    /*000013c0*/ CMP         R5, #4 
    /*000013c4*/ BHI         RgnCheckMppChn_x1438
    RgnCheckMppChn_x13c8:
    /*000013c8*/ LDR         R0, [R4] 
    /*000013cc*/ CMP         R0, #59                     @ 0x3b 
    /*000013d0*/ BHI         RgnCheckMppChn_x14f0
    /*000013d4*/ MOV         R1, #1200                   @ 0x4b0 
    /*000013d8*/ MOV         R2, #20 
    /*000013dc*/ MUL         R1, R1, R5 
    /*000013e0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000013e4*/ MLA         R2, R2, R0, R1 
    /*000013e8*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000013ec*/ ADD         R3, R3, R2 
    /*000013f0*/ LDR         R2, [R3, #592]              @ 0x250 
    /*000013f4*/ CMP         R2, #0 
    /*000013f8*/ BEQ         RgnCheckMppChn_x1520
    /*000013fc*/ LDR         R2, [R4, #4] 
    /*00001400*/ LDR         R0, [R3, #580]              @ 0x244 
    /*00001404*/ CMP         R2, #0 
    /*00001408*/ BLT         RgnCheckMppChn_x1478
    /*0000140c*/ CMP         R2, R0 
    /*00001410*/ BCS         RgnCheckMppChn_x1478
    /*00001414*/ LDR         R2, [R4, #8] 
    /*00001418*/ LDR         R3, [R3, #584]              @ 0x248 
    /*0000141c*/ CMP         R2, #0 
    /*00001420*/ BLT         RgnCheckMppChn_x14b4
    /*00001424*/ CMP         R2, R3 
    /*00001428*/ MOVCC       R0, #0 
    /*0000142c*/ BCS         RgnCheckMppChn_x14b4
    RgnCheckMppChn_x1430:
    /*00001430*/ ADD         SP, SP, #20 
    /*00001434*/ POP         {R4, R5, PC} 
    RgnCheckMppChn_x1438:
    /*00001438*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fc] 
    /*0000143c*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001440*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fc] 
    /*00001444*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00001448*/ MOVW        R2, #763                    @ 0x2fb 
    /*0000144c*/ LDR         R1, [PC, #256]              @ 0x0000000000001554 
    /*00001450*/ BL          0x0000000000001450          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001454*/ B           RgnCheckMppChn_x13c8
    RgnCheckMppChn_x1458:
    /*00001458*/ MOVW        R3, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c4] 
    /*0000145c*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001460*/ MOVT        R3, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c4] 
    /*00001464*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00001468*/ MOVW        R2, #762                    @ 0x2fa 
    /*0000146c*/ LDR         R1, [PC, #224]              @ 0x0000000000001554 
    /*00001470*/ BL          0x0000000000001470          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001474*/ B           RgnCheckMppChn_x13c0
    RgnCheckMppChn_x1478:
    /*00001478*/ MOV         R1, #3 
    /*0000147c*/ STR         R0, [SP, #12] 
    /*00001480*/ STR         R2, [SP, #4] 
    /*00001484*/ MOVW        R3, #782                    @ 0x30e 
    /*00001488*/ MOV         R2, #0 
    /*0000148c*/ STR         R3, [SP] 
    /*00001490*/ STR         R2, [SP, #8] 
    /*00001494*/ MOV         R0, R1 
    /*00001498*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x480] 
    /*0000149c*/ LDR         R3, [PC, #176]              @ 0x0000000000001554 
    /*000014a0*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x480] 
    /*000014a4*/ BL          0x00000000000014A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000014a8*/ MOVW        R0, #32769                  @ 0x8001 
    /*000014ac*/ MOVT        R0, #40963                  @ 0xa003 
    /*000014b0*/ B           RgnCheckMppChn_x1430
    RgnCheckMppChn_x14b4:
    /*000014b4*/ MOV         R1, #3 
    /*000014b8*/ STR         R3, [SP, #12] 
    /*000014bc*/ STR         R2, [SP, #4] 
    /*000014c0*/ MOV         R3, #788                    @ 0x314 
    /*000014c4*/ MOV         R2, #0 
    /*000014c8*/ STR         R3, [SP] 
    /*000014cc*/ STR         R2, [SP, #8] 
    /*000014d0*/ MOV         R0, R1 
    /*000014d4*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4c0] 
    /*000014d8*/ LDR         R3, [PC, #116]              @ 0x0000000000001554 
    /*000014dc*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4c0] 
    /*000014e0*/ BL          0x00000000000014E0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000014e4*/ MOVW        R0, #32770                  @ 0x8002 
    /*000014e8*/ MOVT        R0, #40963                  @ 0xa003 
    /*000014ec*/ B           RgnCheckMppChn_x1430
    RgnCheckMppChn_x14f0:
    /*000014f0*/ MOV         R1, #3 
    /*000014f4*/ STR         R0, [SP, #4] 
    /*000014f8*/ MOVW        R3, #767                    @ 0x2ff 
    /*000014fc*/ MOV         R0, R1 
    /*00001500*/ STR         R3, [SP] 
    /*00001504*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x41c] 
    /*00001508*/ LDR         R3, [PC, #68]               @ 0x0000000000001554 
    /*0000150c*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x41c] 
    /*00001510*/ BL          0x0000000000001510          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001514*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001518*/ MOVT        R0, #40963                  @ 0xa003 
    /*0000151c*/ B           RgnCheckMppChn_x1430
    RgnCheckMppChn_x1520:
    /*00001520*/ MOV         R1, #3 
    /*00001524*/ STR         R0, [SP, #8] 
    /*00001528*/ MOV         R3, #776                    @ 0x308 
    /*0000152c*/ STR         R5, [SP, #4] 
    /*00001530*/ STR         R3, [SP] 
    /*00001534*/ MOV         R0, R1 
    /*00001538*/ MOVW        R2, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x444] 
    /*0000153c*/ LDR         R3, [PC, #16]               @ 0x0000000000001554 
    /*00001540*/ MOVT        R2, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x444] 
    /*00001544*/ BL          0x0000000000001544          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001548*/ MOVW        R0, #32776                  @ 0x8008 
    /*0000154c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001550*/ B           RgnCheckMppChn_x1430
    /*00001554*/ ANDEQ       R0, R0, R0, LSL #2 
FUNC_END RgnCheckMppChn


FUNC_BEGIN RgnCheckLayer
    /*00001558*/ LDR         R2, [R0, #8] 
    /*0000155c*/ LDR         R0, [R0, #12] 
    /*00001560*/ CMP         R2, R1 
    /*00001564*/ BHI         RgnCheckLayer_x1578
    /*00001568*/ CMP         R1, R0 
    /*0000156c*/ BHI         RgnCheckLayer_x1578
    /*00001570*/ MOV         R0, #0 
    /*00001574*/ BX          LR 
    RgnCheckLayer_x1578:
    /*00001578*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000157c*/ SUB         SP, SP, #20 
    /*00001580*/ MOV         R3, R1 
    /*00001584*/ MOV         R1, #3 
    /*00001588*/ STR         R0, [SP, #12] 
    /*0000158c*/ MOV         R0, R1 
    /*00001590*/ STR         R2, [SP, #8] 
    /*00001594*/ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x500] 
    /*00001598*/ STR         R3, [SP, #4] 
    /*0000159c*/ MOVT        R2, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x500] 
    /*000015a0*/ MOV         R3, #800                    @ 0x320 
    /*000015a4*/ STR         R3, [SP] 
    /*000015a8*/ LDR         R3, [PC, #16]               @ 0x00000000000015c0 
    /*000015ac*/ BL          0x00000000000015AC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000015b0*/ MOVW        R0, #32771                  @ 0x8003 
    /*000015b4*/ MOVT        R0, #40963                  @ 0xa003 
    /*000015b8*/ ADD         SP, SP, #20 
    /*000015bc*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*000015c0*/ ANDEQ       R0, R0, R0, LSL R1 
FUNC_END RgnCheckLayer


FUNC_BEGIN RgnCheckPoint
    /*000015c4*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000015c8*/ MOV         R4, R0 
    /*000015cc*/ LDR         R3, [R0, #8] 
    /*000015d0*/ SUB         SP, SP, #36                 @ 0x24 
    /*000015d4*/ LDR         R5, [R1] 
    /*000015d8*/ LDR         R2, [R0, #12] 
    /*000015dc*/ CMP         R5, R3 
    /*000015e0*/ LDR         IP, [R0, #16] 
    /*000015e4*/ LDR         R6, [R1, #4] 
    /*000015e8*/ LDR         R0, [R0, #20] 
    /*000015ec*/ BLT         RgnCheckPoint_x1640
    /*000015f0*/ CMP         R6, R2 
    /*000015f4*/ BLT         RgnCheckPoint_x1640
    /*000015f8*/ CMP         R5, IP 
    /*000015fc*/ BGT         RgnCheckPoint_x1640
    /*00001600*/ CMP         R6, R0 
    /*00001604*/ BGT         RgnCheckPoint_x1640
    /*00001608*/ LDR         R7, [R4, #24] 
    /*0000160c*/ MOV         R0, R5 
    /*00001610*/ MOV         R1, R7 
    /*00001614*/ BL          0x0000000000001614          @ __aeabi_uidivmod [R_ARM_CALL: +0x0] 
    /*00001618*/ LDR         R4, [R4, #28] 
    /*0000161c*/ CMP         R1, #0 
    /*00001620*/ BNE         RgnCheckPoint_x1688
    /*00001624*/ MOV         R0, R6 
    /*00001628*/ MOV         R1, R4 
    /*0000162c*/ BL          0x000000000000162C          @ __aeabi_uidivmod [R_ARM_CALL: +0x0] 
    /*00001630*/ SUBS        R0, R1, #0 
    /*00001634*/ BNE         RgnCheckPoint_x1688
    RgnCheckPoint_x1638:
    /*00001638*/ ADD         SP, SP, #36                 @ 0x24 
    /*0000163c*/ POP         {R4, R5, R6, R7, PC} 
    RgnCheckPoint_x1640:
    /*00001640*/ MOV         R1, #3 
    /*00001644*/ STR         R0, [SP, #24] 
    /*00001648*/ STR         R2, [SP, #20] 
    /*0000164c*/ MOV         R0, R1 
    /*00001650*/ STR         IP, [SP, #16] 
    /*00001654*/ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x544] 
    /*00001658*/ MOVW        IP, #815                    @ 0x32f 
    /*0000165c*/ STR         R3, [SP, #12] 
    /*00001660*/ STR         R6, [SP, #8] 
    /*00001664*/ MOVT        R2, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x544] 
    /*00001668*/ STR         R5, [SP, #4] 
    /*0000166c*/ STR         IP, [SP] 
    /*00001670*/ LDR         R3, [PC, #76]               @ 0x00000000000016c4 
    /*00001674*/ BL          0x0000000000001674          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001678*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000167c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001680*/ ADD         SP, SP, #36                 @ 0x24 
    /*00001684*/ POP         {R4, R5, R6, R7, PC} 
    RgnCheckPoint_x1688:
    /*00001688*/ MOV         R1, #3 
    /*0000168c*/ MOVW        R3, #823                    @ 0x337 
    /*00001690*/ STR         R4, [SP, #16] 
    /*00001694*/ MOV         R0, R1 
    /*00001698*/ STR         R3, [SP] 
    /*0000169c*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x598] 
    /*000016a0*/ STR         R7, [SP, #12] 
    /*000016a4*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x598] 
    /*000016a8*/ STR         R6, [SP, #8] 
    /*000016ac*/ STR         R5, [SP, #4] 
    /*000016b0*/ LDR         R3, [PC, #12]               @ 0x00000000000016c4 
    /*000016b4*/ BL          0x00000000000016B4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000016b8*/ MOVW        R0, #32771                  @ 0x8003 
    /*000016bc*/ MOVT        R0, #40963                  @ 0xa003 
    /*000016c0*/ B           RgnCheckPoint_x1638
    /*000016c4*/ ANDEQ       R0, R0, R0, LSR #2 
FUNC_END RgnCheckPoint


FUNC_BEGIN RgnCheckCoordinatePoint
    /*000016c8*/ LDR         R3, [R0] 
    /*000016cc*/ LDR         R2, [R0, #4] 
    /*000016d0*/ CMP         R3, #0 
    /*000016d4*/ BLT         RgnCheckCoordinatePoint_x16f4
    /*000016d8*/ CMP         R2, #1000                   @ 0x3e8 
    /*000016dc*/ MOVCC       R0, #0 
    /*000016e0*/ MOVCS       R0, #1 
    /*000016e4*/ CMP         R3, #1000                   @ 0x3e8 
    /*000016e8*/ ORRGE       R0, R0, #1 
    /*000016ec*/ CMP         R0, #0 
    /*000016f0*/ BXEQ        LR 
    RgnCheckCoordinatePoint_x16f4:
    /*000016f4*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000016f8*/ SUB         SP, SP, #20 
    /*000016fc*/ MOV         R1, #3 
    /*00001700*/ STR         R2, [SP, #8] 
    /*00001704*/ MOV         R0, R1 
    /*00001708*/ STR         R3, [SP, #4] 
    /*0000170c*/ MOVW        R2, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5ec] 
    /*00001710*/ MOVW        R3, #838                    @ 0x346 
    /*00001714*/ MOVT        R2, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5ec] 
    /*00001718*/ STR         R3, [SP] 
    /*0000171c*/ LDR         R3, [PC, #16]               @ 0x0000000000001734 
    /*00001720*/ BL          0x0000000000001720          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001724*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001728*/ MOVT        R0, #40963                  @ 0xa003 
    /*0000172c*/ ADD         SP, SP, #20 
    /*00001730*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00001734*/ ANDEQ       R0, R0, R0, LSR R1 
FUNC_END RgnCheckCoordinatePoint


FUNC_BEGIN RgnCheckQuadCoverAttr
    /*00001738*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000173c*/ SUB         SP, SP, #52                 @ 0x34 
    /*00001740*/ LDR         R3, [R1] 
    /*00001744*/ STR         R1, [SP, #44]               @ 0x2c 
    /*00001748*/ CMP         R3, #1 
    /*0000174c*/ BHI         RgnCheckQuadCoverAttr_x1914
    /*00001750*/ CMP         R3, #0 
    /*00001754*/ MOV         R6, R0 
    /*00001758*/ BEQ         RgnCheckQuadCoverAttr_x1800
    RgnCheckQuadCoverAttr_x175c:
    /*0000175c*/ LDR         R3, [R6, #20] 
    /*00001760*/ MOV         R7, #0 
    /*00001764*/ LDR         R9, [R6, #16] 
    /*00001768*/ LDR         R4, [SP, #44]               @ 0x2c 
    /*0000176c*/ STR         R3, [SP, #32] 
    /*00001770*/ LDR         R3, [R6, #24] 
    /*00001774*/ STR         R3, [SP, #36]               @ 0x24 
    /*00001778*/ LDR         R3, [R6, #28] 
    /*0000177c*/ STR         R3, [SP, #40]               @ 0x28 
    RgnCheckQuadCoverAttr_x1780:
    /*00001780*/ LDR         R5, [R4, #8] 
    /*00001784*/ CMP         R5, R9 
    /*00001788*/ MOV         R0, R5 
    /*0000178c*/ BLT         RgnCheckQuadCoverAttr_x1858
    /*00001790*/ LDR         FP, [R4, #12] 
    /*00001794*/ ADD         R4, R4, #8 
    /*00001798*/ LDR         R3, [SP, #32] 
    /*0000179c*/ CMP         FP, R3 
    /*000017a0*/ BLT         RgnCheckQuadCoverAttr_x1864
    /*000017a4*/ LDR         R3, [SP, #36]               @ 0x24 
    /*000017a8*/ CMP         R5, R3 
    /*000017ac*/ BGT         RgnCheckQuadCoverAttr_x1864
    /*000017b0*/ LDR         R3, [SP, #40]               @ 0x28 
    /*000017b4*/ CMP         FP, R3 
    /*000017b8*/ BGT         RgnCheckQuadCoverAttr_x1864
    /*000017bc*/ LDR         SL, [R6, #32] 
    /*000017c0*/ MOV         R1, SL 
    /*000017c4*/ BL          0x00000000000017C4          @ __aeabi_uidivmod [R_ARM_CALL: +0x0] 
    /*000017c8*/ MOV         R0, FP 
    /*000017cc*/ CMP         R1, #0 
    /*000017d0*/ BNE         RgnCheckQuadCoverAttr_x18bc
    /*000017d4*/ LDR         R8, [R6, #36]               @ 0x24 
    /*000017d8*/ MOV         R1, R8 
    /*000017dc*/ BL          0x00000000000017DC          @ __aeabi_uidivmod [R_ARM_CALL: +0x0] 
    /*000017e0*/ CMP         R1, #0 
    /*000017e4*/ BNE         RgnCheckQuadCoverAttr_x1904
    /*000017e8*/ ADD         R7, R7, #1 
    /*000017ec*/ CMP         R7, #4 
    /*000017f0*/ BNE         RgnCheckQuadCoverAttr_x1780
    /*000017f4*/ MOV         R0, R1 
    RgnCheckQuadCoverAttr_x17f8:
    /*000017f8*/ ADD         SP, SP, #52                 @ 0x34 
    /*000017fc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnCheckQuadCoverAttr_x1800:
    /*00001800*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*00001804*/ LDR         R2, [R0, #8] 
    /*00001808*/ LDR         R0, [R0, #12] 
    /*0000180c*/ LDR         R3, [R3, #4] 
    /*00001810*/ CMP         R3, R2 
    /*00001814*/ BCC         RgnCheckQuadCoverAttr_x1820
    /*00001818*/ CMP         R3, R0 
    /*0000181c*/ BLS         RgnCheckQuadCoverAttr_x175c
    RgnCheckQuadCoverAttr_x1820:
    /*00001820*/ MOV         R1, #3 
    /*00001824*/ STR         R0, [SP, #12] 
    /*00001828*/ STR         R2, [SP, #8] 
    /*0000182c*/ MOV         R0, R1 
    /*00001830*/ STR         R3, [SP, #4] 
    /*00001834*/ MOVW        R2, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x670] 
    /*00001838*/ MOVW        R3, #858                    @ 0x35a 
    /*0000183c*/ MOVT        R2, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x670] 
    /*00001840*/ STR         R3, [SP] 
    /*00001844*/ LDR         R3, [PC, #248]              @ 0x0000000000001944 
    /*00001848*/ BL          0x0000000000001848          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000184c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001850*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001854*/ B           RgnCheckQuadCoverAttr_x17f8
    RgnCheckQuadCoverAttr_x1858:
    /*00001858*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*0000185c*/ ADD         R3, R3, R7, LSL #3 
    /*00001860*/ LDR         FP, [R3, #12] 
    RgnCheckQuadCoverAttr_x1864:
    /*00001864*/ LDR         R3, [SP, #40]               @ 0x28 
    /*00001868*/ MOV         R1, #3 
    /*0000186c*/ MOVW        IP, #871                    @ 0x367 
    /*00001870*/ MOV         R0, R1 
    /*00001874*/ STR         R9, [SP, #16] 
    /*00001878*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6b8] 
    /*0000187c*/ STR         R3, [SP, #28] 
    /*00001880*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6b8] 
    /*00001884*/ LDR         R3, [SP, #32] 
    /*00001888*/ STR         FP, [SP, #12] 
    /*0000188c*/ STR         R5, [SP, #8] 
    /*00001890*/ STR         R3, [SP, #24] 
    /*00001894*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00001898*/ STR         R7, [SP, #4] 
    /*0000189c*/ STR         IP, [SP] 
    /*000018a0*/ STR         R3, [SP, #20] 
    /*000018a4*/ LDR         R3, [PC, #152]              @ 0x0000000000001944 
    /*000018a8*/ BL          0x00000000000018A8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000018ac*/ MOVW        R0, #32771                  @ 0x8003 
    /*000018b0*/ MOVT        R0, #40963                  @ 0xa003 
    /*000018b4*/ ADD         SP, SP, #52                 @ 0x34 
    /*000018b8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnCheckQuadCoverAttr_x18bc:
    /*000018bc*/ MOV         R8, SL 
    /*000018c0*/ LDR         SL, [R6, #36]               @ 0x24 
    RgnCheckQuadCoverAttr_x18c4:
    /*000018c4*/ MOV         R1, #3 
    /*000018c8*/ MOVW        R3, #878                    @ 0x36e 
    /*000018cc*/ STR         SL, [SP, #20] 
    /*000018d0*/ MOV         R0, R1 
    /*000018d4*/ STR         R3, [SP] 
    /*000018d8*/ MOVW        R2, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x710] 
    /*000018dc*/ STR         R8, [SP, #16] 
    /*000018e0*/ MOVT        R2, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0x710] 
    /*000018e4*/ STR         FP, [SP, #12] 
    /*000018e8*/ STR         R5, [SP, #8] 
    /*000018ec*/ STR         R7, [SP, #4] 
    /*000018f0*/ LDR         R3, [PC, #76]               @ 0x0000000000001944 
    /*000018f4*/ BL          0x00000000000018F4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000018f8*/ MOVW        R0, #32771                  @ 0x8003 
    /*000018fc*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001900*/ B           RgnCheckQuadCoverAttr_x17f8
    RgnCheckQuadCoverAttr_x1904:
    /*00001904*/ MOV         R3, R8 
    /*00001908*/ MOV         R8, SL 
    /*0000190c*/ MOV         SL, R3 
    /*00001910*/ B           RgnCheckQuadCoverAttr_x18c4
    RgnCheckQuadCoverAttr_x1914:
    /*00001914*/ MOV         R1, #3 
    /*00001918*/ STR         R3, [SP, #4] 
    /*0000191c*/ MOV         R0, R1 
    /*00001920*/ MOVW        R3, #850                    @ 0x352 
    /*00001924*/ MOVW        R2, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x640] 
    /*00001928*/ STR         R3, [SP] 
    /*0000192c*/ MOVT        R2, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x640] 
    /*00001930*/ LDR         R3, [PC, #12]               @ 0x0000000000001944 
    /*00001934*/ BL          0x0000000000001934          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001938*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000193c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001940*/ B           RgnCheckQuadCoverAttr_x17f8
    /*00001944*/ ANDEQ       R0, R0, R8, ASR #2 
FUNC_END RgnCheckQuadCoverAttr


FUNC_BEGIN RgnCheckQuadCoverCoordinateAttr
    /*00001948*/ PUSH        {R4, LR} 
    /*0000194c*/ SUB         SP, SP, #16 
    /*00001950*/ LDR         R3, [R1] 
    /*00001954*/ CMP         R3, #1 
    /*00001958*/ BHI         RgnCheckQuadCoverCoordinateAttr_x1a4c
    /*0000195c*/ CMP         R3, #0 
    /*00001960*/ BEQ         RgnCheckQuadCoverCoordinateAttr_x19b4
    RgnCheckQuadCoverCoordinateAttr_x1964:
    /*00001964*/ MOV         R3, R1 
    /*00001968*/ MOV         R2, #0 
    /*0000196c*/ MOVW        LR, #999                    @ 0x3e7 
    RgnCheckQuadCoverCoordinateAttr_x1970:
    /*00001970*/ LDR         IP, [R3, #8] 
    /*00001974*/ CMP         IP, #0 
    /*00001978*/ BLT         RgnCheckQuadCoverCoordinateAttr_x1a08
    /*0000197c*/ CMP         IP, LR 
    /*00001980*/ LDR         R4, [R3, #12] 
    /*00001984*/ ADD         R3, R3, #8 
    /*00001988*/ MOVLE       R0, #0 
    /*0000198c*/ MOVGT       R0, #1 
    /*00001990*/ CMP         R4, LR 
    /*00001994*/ ORRHI       R0, R0, #1 
    /*00001998*/ CMP         R0, #0 
    /*0000199c*/ BNE         RgnCheckQuadCoverCoordinateAttr_x1a10
    /*000019a0*/ ADD         R2, R2, #1 
    /*000019a4*/ CMP         R2, #4 
    /*000019a8*/ BNE         RgnCheckQuadCoverCoordinateAttr_x1970
    RgnCheckQuadCoverCoordinateAttr_x19ac:
    /*000019ac*/ ADD         SP, SP, #16 
    /*000019b0*/ POP         {R4, PC} 
    RgnCheckQuadCoverCoordinateAttr_x19b4:
    /*000019b4*/ LDR         R2, [R0, #8] 
    /*000019b8*/ LDR         R3, [R1, #4] 
    /*000019bc*/ LDR         R0, [R0, #12] 
    /*000019c0*/ CMP         R3, R2 
    /*000019c4*/ BCC         RgnCheckQuadCoverCoordinateAttr_x19d0
    /*000019c8*/ CMP         R3, R0 
    /*000019cc*/ BLS         RgnCheckQuadCoverCoordinateAttr_x1964
    RgnCheckQuadCoverCoordinateAttr_x19d0:
    /*000019d0*/ MOV         R1, #3 
    /*000019d4*/ STR         R0, [SP, #12] 
    /*000019d8*/ STR         R2, [SP, #8] 
    /*000019dc*/ MOV         R0, R1 
    /*000019e0*/ STR         R3, [SP, #4] 
    /*000019e4*/ MOVW        R2, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x670] 
    /*000019e8*/ MOV         R3, #900                    @ 0x384 
    /*000019ec*/ MOVT        R2, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0x670] 
    /*000019f0*/ STR         R3, [SP] 
    /*000019f4*/ LDR         R3, [PC, #128]              @ 0x0000000000001a7c 
    /*000019f8*/ BL          0x00000000000019F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000019fc*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001a00*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001a04*/ B           RgnCheckQuadCoverCoordinateAttr_x19ac
    RgnCheckQuadCoverCoordinateAttr_x1a08:
    /*00001a08*/ ADD         R1, R1, R2, LSL #3 
    /*00001a0c*/ LDR         R4, [R1, #12] 
    RgnCheckQuadCoverCoordinateAttr_x1a10:
    /*00001a10*/ MOV         R1, #3 
    /*00001a14*/ STR         R2, [SP, #4] 
    /*00001a18*/ STR         R4, [SP, #12] 
    /*00001a1c*/ MOV         R0, R1 
    /*00001a20*/ STR         IP, [SP, #8] 
    /*00001a24*/ MOV         R3, #912                    @ 0x390 
    /*00001a28*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x778] 
    /*00001a2c*/ STR         R3, [SP] 
    /*00001a30*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0x778] 
    /*00001a34*/ LDR         R3, [PC, #64]               @ 0x0000000000001a7c 
    /*00001a38*/ BL          0x0000000000001A38          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001a3c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001a40*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001a44*/ ADD         SP, SP, #16 
    /*00001a48*/ POP         {R4, PC} 
    RgnCheckQuadCoverCoordinateAttr_x1a4c:
    /*00001a4c*/ MOV         R1, #3 
    /*00001a50*/ STR         R3, [SP, #4] 
    /*00001a54*/ MOV         R0, R1 
    /*00001a58*/ MOV         R3, #892                    @ 0x37c 
    /*00001a5c*/ MOVW        R2, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x640] 
    /*00001a60*/ STR         R3, [SP] 
    /*00001a64*/ MOVT        R2, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x640] 
    /*00001a68*/ LDR         R3, [PC, #12]               @ 0x0000000000001a7c 
    /*00001a6c*/ BL          0x0000000000001A6C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001a70*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001a74*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001a78*/ B           RgnCheckQuadCoverCoordinateAttr_x19ac
    /*00001a7c*/ ANDEQ       R0, R0, R0, ROR #2 
FUNC_END RgnCheckQuadCoverCoordinateAttr


FUNC_BEGIN RgnCheckSize
    /*00001a80*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00001a84*/ MOV         R4, R0 
    /*00001a88*/ LDR         R3, [R0, #8] 
    /*00001a8c*/ SUB         SP, SP, #32 
    /*00001a90*/ LDR         R5, [R1] 
    /*00001a94*/ LDR         R2, [R0, #12] 
    /*00001a98*/ CMP         R5, R3 
    /*00001a9c*/ LDR         IP, [R0, #16] 
    /*00001aa0*/ LDR         R6, [R1, #4] 
    /*00001aa4*/ LDR         R0, [R0, #20] 
    /*00001aa8*/ BCC         RgnCheckSize_x1b0c
    /*00001aac*/ CMP         R6, R2 
    /*00001ab0*/ BCC         RgnCheckSize_x1b0c
    /*00001ab4*/ CMP         R5, IP 
    /*00001ab8*/ BHI         RgnCheckSize_x1b0c
    /*00001abc*/ CMP         R6, R0 
    /*00001ac0*/ BHI         RgnCheckSize_x1b0c
    /*00001ac4*/ LDR         R7, [R4, #24] 
    /*00001ac8*/ MOV         R0, R5 
    /*00001acc*/ MOV         R1, R7 
    /*00001ad0*/ BL          0x0000000000001AD0          @ __aeabi_uidivmod [R_ARM_CALL: +0x0] 
    /*00001ad4*/ LDR         R8, [R4, #28] 
    /*00001ad8*/ CMP         R1, #0 
    /*00001adc*/ BNE         RgnCheckSize_x1b54
    /*00001ae0*/ MOV         R0, R6 
    /*00001ae4*/ MOV         R1, R8 
    /*00001ae8*/ BL          0x0000000000001AE8          @ __aeabi_uidivmod [R_ARM_CALL: +0x0] 
    /*00001aec*/ SUBS        R0, R1, #0 
    /*00001af0*/ BNE         RgnCheckSize_x1b54
    /*00001af4*/ MUL         R3, R6, R5 
    /*00001af8*/ LDR         R2, [R4, #32] 
    /*00001afc*/ CMP         R3, R2 
    /*00001b00*/ BHI         RgnCheckSize_x1b90
    RgnCheckSize_x1b04:
    /*00001b04*/ ADD         SP, SP, #32 
    /*00001b08*/ POP         {R4, R5, R6, R7, R8, PC} 
    RgnCheckSize_x1b0c:
    /*00001b0c*/ MOV         R1, #3 
    /*00001b10*/ STR         R0, [SP, #24] 
    /*00001b14*/ STR         R2, [SP, #20] 
    /*00001b18*/ MOV         R0, R1 
    /*00001b1c*/ STR         IP, [SP, #16] 
    /*00001b20*/ MOVW        R2, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7d0] 
    /*00001b24*/ MOVW        IP, #929                    @ 0x3a1 
    /*00001b28*/ STR         R3, [SP, #12] 
    /*00001b2c*/ STR         R6, [SP, #8] 
    /*00001b30*/ MOVT        R2, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7d0] 
    /*00001b34*/ STR         R5, [SP, #4] 
    /*00001b38*/ STR         IP, [SP] 
    /*00001b3c*/ LDR         R3, [PC, #132]              @ 0x0000000000001bc8 
    /*00001b40*/ BL          0x0000000000001B40          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001b44*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001b48*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001b4c*/ ADD         SP, SP, #32 
    /*00001b50*/ POP         {R4, R5, R6, R7, R8, PC} 
    RgnCheckSize_x1b54:
    /*00001b54*/ MOV         R1, #3 
    /*00001b58*/ MOVW        R3, #938                    @ 0x3aa 
    /*00001b5c*/ STR         R8, [SP, #16] 
    /*00001b60*/ MOV         R0, R1 
    /*00001b64*/ STR         R3, [SP] 
    /*00001b68*/ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x824] 
    /*00001b6c*/ STR         R7, [SP, #12] 
    /*00001b70*/ MOVT        R2, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0x824] 
    /*00001b74*/ STR         R6, [SP, #8] 
    /*00001b78*/ STR         R5, [SP, #4] 
    /*00001b7c*/ LDR         R3, [PC, #68]               @ 0x0000000000001bc8 
    /*00001b80*/ BL          0x0000000000001B80          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001b84*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001b88*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001b8c*/ B           RgnCheckSize_x1b04
    RgnCheckSize_x1b90:
    /*00001b90*/ MOV         R1, #3 
    /*00001b94*/ STR         R2, [SP, #12] 
    /*00001b98*/ MOVW        R3, #946                    @ 0x3b2 
    /*00001b9c*/ STR         R6, [SP, #8] 
    /*00001ba0*/ STR         R3, [SP] 
    /*00001ba4*/ MOV         R0, R1 
    /*00001ba8*/ STR         R5, [SP, #4] 
    /*00001bac*/ MOVW        R2, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x878] 
    /*00001bb0*/ LDR         R3, [PC, #16]               @ 0x0000000000001bc8 
    /*00001bb4*/ MOVT        R2, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0x878] 
    /*00001bb8*/ BL          0x0000000000001BB8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001bbc*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001bc0*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001bc4*/ B           RgnCheckSize_x1b04
    /*00001bc8*/ ANDEQ       R0, R0, R0, LSL #3 
FUNC_END RgnCheckSize


FUNC_BEGIN RgnCheckCoordinateSize
    /*00001bcc*/ LDR         R3, [R1] 
    /*00001bd0*/ LDR         R2, [R1, #4] 
    /*00001bd4*/ CMP         R3, #0 
    /*00001bd8*/ BEQ         RgnCheckCoordinateSize_x1bf4
    /*00001bdc*/ SUB         R0, R2, #1 
    /*00001be0*/ CMP         R3, #1000                   @ 0x3e8 
    /*00001be4*/ CMPLS       R0, #1000                   @ 0x3e8 
    /*00001be8*/ MOVCS       R0, #1 
    /*00001bec*/ MOVCC       R0, #0 
    /*00001bf0*/ BXCC        LR 
    RgnCheckCoordinateSize_x1bf4:
    /*00001bf4*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00001bf8*/ SUB         SP, SP, #20 
    /*00001bfc*/ MOV         R1, #3 
    /*00001c00*/ STR         R2, [SP, #8] 
    /*00001c04*/ MOV         R0, R1 
    /*00001c08*/ STR         R3, [SP, #4] 
    /*00001c0c*/ MOVW        R2, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8b8] 
    /*00001c10*/ MOV         R3, #960                    @ 0x3c0 
    /*00001c14*/ MOVT        R2, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8b8] 
    /*00001c18*/ STR         R3, [SP] 
    /*00001c1c*/ LDR         R3, [PC, #16]               @ 0x0000000000001c34 
    /*00001c20*/ BL          0x0000000000001C20          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001c24*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001c28*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001c2c*/ ADD         SP, SP, #20 
    /*00001c30*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00001c34*/ MULEQ       R0, R0, R1 
FUNC_END RgnCheckCoordinateSize


FUNC_BEGIN RgnCheckPixl
    /*00001c38*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00001c3c*/ MOV         IP, R1 
    /*00001c40*/ LDR         R3, [R0, #8] 
    /*00001c44*/ SUB         SP, SP, #12 
    /*00001c48*/ CMP         R3, #4 
    /*00001c4c*/ BHI         RgnCheckPixl_x1cc8
    /*00001c50*/ CMP         R3, #0 
    /*00001c54*/ BEQ         RgnCheckPixl_x1c88
    /*00001c58*/ LDR         R2, [R0, #12] 
    /*00001c5c*/ CMP         R1, R2 
    /*00001c60*/ BEQ         RgnCheckPixl_x1cbc
    /*00001c64*/ ADD         R2, R3, #3 
    /*00001c68*/ ADD         R3, R0, #16 
    /*00001c6c*/ ADD         R0, R0, R2, LSL #2 
    /*00001c70*/ B           RgnCheckPixl_x1c80
    RgnCheckPixl_x1c74:
    /*00001c74*/ LDR         R2, [R3], #4 
    /*00001c78*/ CMP         IP, R2 
    /*00001c7c*/ BEQ         RgnCheckPixl_x1cbc
    RgnCheckPixl_x1c80:
    /*00001c80*/ CMP         R3, R0 
    /*00001c84*/ BNE         RgnCheckPixl_x1c74
    RgnCheckPixl_x1c88:
    /*00001c88*/ MOV         R1, #3 
    /*00001c8c*/ MOV         R3, #984                    @ 0x3d8 
    /*00001c90*/ STR         IP, [SP, #4] 
    /*00001c94*/ MOV         R0, R1 
    /*00001c98*/ STR         R3, [SP] 
    /*00001c9c*/ MOVW        R2, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x948] 
    /*00001ca0*/ LDR         R3, [PC, #80]               @ 0x0000000000001cf8 
    /*00001ca4*/ MOVT        R2, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0x948] 
    /*00001ca8*/ BL          0x0000000000001CA8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001cac*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001cb0*/ MOVT        R0, #40963                  @ 0xa003 
    RgnCheckPixl_x1cb4:
    /*00001cb4*/ ADD         SP, SP, #12 
    /*00001cb8*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    RgnCheckPixl_x1cbc:
    /*00001cbc*/ MOV         R0, #0 
    /*00001cc0*/ ADD         SP, SP, #12 
    /*00001cc4*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    RgnCheckPixl_x1cc8:
    /*00001cc8*/ MOV         R1, #3 
    /*00001ccc*/ MOV         R3, #972                    @ 0x3cc 
    /*00001cd0*/ STR         IP, [SP, #4] 
    /*00001cd4*/ MOV         R0, R1 
    /*00001cd8*/ STR         R3, [SP] 
    /*00001cdc*/ MOVW        R2, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x910] 
    /*00001ce0*/ LDR         R3, [PC, #16]               @ 0x0000000000001cf8 
    /*00001ce4*/ MOVT        R2, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0x910] 
    /*00001ce8*/ BL          0x0000000000001CE8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001cec*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001cf0*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001cf4*/ B           RgnCheckPixl_x1cb4
    /*00001cf8*/ ANDEQ       R0, R0, R8, LSR #3 
FUNC_END RgnCheckPixl


FUNC_BEGIN RgnCheckAlpha
    /*00001cfc*/ LDR         R2, [R0, #12] 
    /*00001d00*/ LDR         R0, [R0, #8] 
    /*00001d04*/ CMP         R2, R1 
    /*00001d08*/ BHI         RgnCheckAlpha_x1d1c
    /*00001d0c*/ CMP         R1, R0 
    /*00001d10*/ BHI         RgnCheckAlpha_x1d1c
    /*00001d14*/ MOV         R0, #0 
    /*00001d18*/ BX          LR 
    RgnCheckAlpha_x1d1c:
    /*00001d1c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00001d20*/ SUB         SP, SP, #20 
    /*00001d24*/ MOV         R3, R1 
    /*00001d28*/ MOV         R1, #3 
    /*00001d2c*/ STR         R0, [SP, #12] 
    /*00001d30*/ MOV         R0, R1 
    /*00001d34*/ STR         R2, [SP, #8] 
    /*00001d38*/ MOVW        R2, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x978] 
    /*00001d3c*/ STR         R3, [SP, #4] 
    /*00001d40*/ MOVT        R2, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0x978] 
    /*00001d44*/ MOVW        R3, #993                    @ 0x3e1 
    /*00001d48*/ STR         R3, [SP] 
    /*00001d4c*/ LDR         R3, [PC, #16]               @ 0x0000000000001d64 
    /*00001d50*/ BL          0x0000000000001D50          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001d54*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001d58*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001d5c*/ ADD         SP, SP, #20 
    /*00001d60*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00001d64*/ @           <UNDEFINED> instruction: 0x000001b8 
FUNC_END RgnCheckAlpha


FUNC_BEGIN RgnCheckQp
    /*00001d68*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00001d6c*/ SUB         SP, SP, #28 
    /*00001d70*/ LDR         R3, [R1] 
    /*00001d74*/ CMP         R3, #1 
    /*00001d78*/ BHI         RgnCheckQp_x1e54
    /*00001d7c*/ CMP         R3, #0 
    /*00001d80*/ LDR         R3, [R1, #4] 
    /*00001d84*/ BNE         RgnCheckQp_x1dc4
    /*00001d88*/ LDR         R2, [R0, #20] 
    /*00001d8c*/ LDR         IP, [R0, #24] 
    /*00001d90*/ CMP         R3, R2 
    /*00001d94*/ BLT         RgnCheckQp_x1e84
    /*00001d98*/ CMP         R3, IP 
    /*00001d9c*/ BGT         RgnCheckQp_x1e84
    RgnCheckQp_x1da0:
    /*00001da0*/ LDR         R3, [R0, #8] 
    /*00001da4*/ CMP         R3, #1 
    /*00001da8*/ LDR         R3, [R1, #8] 
    /*00001dac*/ BEQ         RgnCheckQp_x1e1c
    /*00001db0*/ CMP         R3, #1 
    /*00001db4*/ BEQ         RgnCheckQp_x1ea4
    RgnCheckQp_x1db8:
    /*00001db8*/ MOV         R0, #0 
    RgnCheckQp_x1dbc:
    /*00001dbc*/ ADD         SP, SP, #28 
    /*00001dc0*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    RgnCheckQp_x1dc4:
    /*00001dc4*/ LDR         R2, [R0, #12] 
    /*00001dc8*/ LDR         IP, [R0, #16] 
    /*00001dcc*/ CMP         R3, R2 
    /*00001dd0*/ BLT         RgnCheckQp_x1ddc
    /*00001dd4*/ CMP         R3, IP 
    /*00001dd8*/ BLE         RgnCheckQp_x1da0
    RgnCheckQp_x1ddc:
    /*00001ddc*/ MOV         R1, #3 
    /*00001de0*/ STR         R2, [SP, #12] 
    /*00001de4*/ STR         R3, [SP, #8] 
    /*00001de8*/ MOV         R0, R1 
    /*00001dec*/ MOV         R2, #1 
    /*00001df0*/ MOVW        R3, #1015                   @ 0x3f7 
    /*00001df4*/ STR         IP, [SP, #16] 
    RgnCheckQp_x1df8:
    /*00001df8*/ STR         R2, [SP, #4] 
    /*00001dfc*/ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9e8] 
    /*00001e00*/ STR         R3, [SP] 
    /*00001e04*/ MOVT        R2, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9e8] 
    /*00001e08*/ LDR         R3, [PC, #196]              @ 0x0000000000001ed4 
    /*00001e0c*/ BL          0x0000000000001E0C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001e10*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001e14*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001e18*/ B           RgnCheckQp_x1dbc
    RgnCheckQp_x1e1c:
    /*00001e1c*/ CMP         R3, #1 
    /*00001e20*/ BLS         RgnCheckQp_x1db8
    /*00001e24*/ MOV         R1, #3 
    /*00001e28*/ STR         R3, [SP, #4] 
    /*00001e2c*/ MOV         R0, R1 
    /*00001e30*/ MOVW        R3, #1034                   @ 0x40a 
    /*00001e34*/ MOVW        R2, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa30] 
    /*00001e38*/ STR         R3, [SP] 
    /*00001e3c*/ MOVT        R2, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa30] 
    /*00001e40*/ LDR         R3, [PC, #140]              @ 0x0000000000001ed4 
    /*00001e44*/ BL          0x0000000000001E44          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001e48*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001e4c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001e50*/ B           RgnCheckQp_x1dbc
    RgnCheckQp_x1e54:
    /*00001e54*/ MOV         R1, #3 
    /*00001e58*/ STR         R3, [SP, #4] 
    /*00001e5c*/ MOV         R0, R1 
    /*00001e60*/ MOVW        R3, #1005                   @ 0x3ed 
    /*00001e64*/ MOVW        R2, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9bc] 
    /*00001e68*/ STR         R3, [SP] 
    /*00001e6c*/ MOVT        R2, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9bc] 
    /*00001e70*/ LDR         R3, [PC, #92]               @ 0x0000000000001ed4 
    /*00001e74*/ BL          0x0000000000001E74          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001e78*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001e7c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001e80*/ B           RgnCheckQp_x1dbc
    RgnCheckQp_x1e84:
    /*00001e84*/ MOV         R1, #3 
    /*00001e88*/ STR         R2, [SP, #12] 
    /*00001e8c*/ STR         R3, [SP, #8] 
    /*00001e90*/ MOV         R0, R1 
    /*00001e94*/ STR         IP, [SP, #16] 
    /*00001e98*/ MOV         R2, #0 
    /*00001e9c*/ MOVW        R3, #1025                   @ 0x401 
    /*00001ea0*/ B           RgnCheckQp_x1df8
    RgnCheckQp_x1ea4:
    /*00001ea4*/ MOV         R1, #3 
    /*00001ea8*/ STR         R3, [SP, #4] 
    /*00001eac*/ MOV         R0, R1 
    /*00001eb0*/ MOVW        R3, #1042                   @ 0x412 
    /*00001eb4*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa60] 
    /*00001eb8*/ STR         R3, [SP] 
    /*00001ebc*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa60] 
    /*00001ec0*/ LDR         R3, [PC, #12]               @ 0x0000000000001ed4 
    /*00001ec4*/ BL          0x0000000000001EC4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001ec8*/ MOVW        R0, #32771                  @ 0x8003 
    /*00001ecc*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001ed0*/ B           RgnCheckQp_x1dbc
    /*00001ed4*/ ANDEQ       R0, R0, R8, ASR #3 
FUNC_END RgnCheckQp


FUNC_BEGIN RgnCheckPixlFmtUnique
    /*00001ed8*/ PUSH        {R4, R5, R6, LR} 
    /*00001edc*/ SUB         SP, SP, #24 
    /*00001ee0*/ MOV         R5, R2 
    /*00001ee4*/ MOV         R6, R1 
    /*00001ee8*/ BL          0x0000000000001EE8          @ RgnGetChnManage [R_ARM_CALL: .text+0x11fc] 
    /*00001eec*/ SUBS        R4, R0, #0 
    /*00001ef0*/ BEQ         RgnCheckPixlFmtUnique_x1f8c
    RgnCheckPixlFmtUnique_x1ef4:
    /*00001ef4*/ LDR         R0, [R4] 
    /*00001ef8*/ CMP         R0, #0 
    /*00001efc*/ BEQ         RgnCheckPixlFmtUnique_x1f38
    /*00001f00*/ LDR         R3, [R4, #232]              @ 0xe8 
    /*00001f04*/ CMP         R5, R3 
    /*00001f08*/ MOVEQ       R3, R4 
    /*00001f0c*/ MOVEQ       R2, #0 
    /*00001f10*/ BEQ         RgnCheckPixlFmtUnique_x1f24
    /*00001f14*/ B           RgnCheckPixlFmtUnique_x1f40
    RgnCheckPixlFmtUnique_x1f18:
    /*00001f18*/ LDR         R1, [R3, #232]              @ 0xe8 
    /*00001f1c*/ CMP         R1, R5 
    /*00001f20*/ BNE         RgnCheckPixlFmtUnique_x1f40
    RgnCheckPixlFmtUnique_x1f24:
    /*00001f24*/ ADD         R2, R2, #1 
    /*00001f28*/ ADD         R3, R3, #168                @ 0xa8 
    /*00001f2c*/ CMP         R2, R0 
    /*00001f30*/ BNE         RgnCheckPixlFmtUnique_x1f18
    /*00001f34*/ MOV         R0, #0 
    RgnCheckPixlFmtUnique_x1f38:
    /*00001f38*/ ADD         SP, SP, #24 
    /*00001f3c*/ POP         {R4, R5, R6, PC} 
    RgnCheckPixlFmtUnique_x1f40:
    /*00001f40*/ LDR         R4, [R6, #8] 
    /*00001f44*/ MOV         R1, #3 
    /*00001f48*/ LDR         LR, [R6, #4] 
    /*00001f4c*/ MOV         R0, R1 
    /*00001f50*/ LDR         IP, [R6] 
    /*00001f54*/ MOVW        R3, #1122                   @ 0x462 
    /*00001f58*/ STR         R5, [SP, #4] 
    /*00001f5c*/ MOVW        R2, #0                      @ .LC58 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xab4] 
    /*00001f60*/ STR         R3, [SP] 
    /*00001f64*/ MOVT        R2, #0                      @ .LC58 [R_ARM_MOVT_ABS: .rodata.str1.4+0xab4] 
    /*00001f68*/ STR         R4, [SP, #16] 
    /*00001f6c*/ STR         LR, [SP, #12] 
    /*00001f70*/ STR         IP, [SP, #8] 
    /*00001f74*/ LDR         R3, [PC, #48]               @ 0x0000000000001fac 
    /*00001f78*/ BL          0x0000000000001F78          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001f7c*/ MOVW        R0, #32777                  @ 0x8009 
    /*00001f80*/ MOVT        R0, #40963                  @ 0xa003 
    /*00001f84*/ ADD         SP, SP, #24 
    /*00001f88*/ POP         {R4, R5, R6, PC} 
    RgnCheckPixlFmtUnique_x1f8c:
    /*00001f8c*/ MOVW        R3, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa9c] 
    /*00001f90*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00001f94*/ MOVT        R3, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa9c] 
    /*00001f98*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00001f9c*/ MOVW        R2, #1116                   @ 0x45c 
    /*00001fa0*/ LDR         R1, [PC, #4]                @ 0x0000000000001fac 
    /*00001fa4*/ BL          0x0000000000001FA4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001fa8*/ B           RgnCheckPixlFmtUnique_x1ef4
    /*00001fac*/ LDRDEQ      R0, [R0], -R4 
FUNC_END RgnCheckPixlFmtUnique


FUNC_BEGIN RgnCheckMscBlkSz
    /*00001fb0*/ LDR         R2, [R0, #12] 
    /*00001fb4*/ LDR         R0, [R0, #8] 
    /*00001fb8*/ CMP         R2, R1 
    /*00001fbc*/ BHI         RgnCheckMscBlkSz_x1fd0
    /*00001fc0*/ CMP         R1, R0 
    /*00001fc4*/ BHI         RgnCheckMscBlkSz_x1fd0
    /*00001fc8*/ MOV         R0, #0 
    /*00001fcc*/ BX          LR 
    RgnCheckMscBlkSz_x1fd0:
    /*00001fd0*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00001fd4*/ SUB         SP, SP, #20 
    /*00001fd8*/ MOV         R3, R1 
    /*00001fdc*/ MOV         R1, #3 
    /*00001fe0*/ STR         R0, [SP, #12] 
    /*00001fe4*/ MOV         R0, R1 
    /*00001fe8*/ STR         R2, [SP, #8] 
    /*00001fec*/ MOVW        R2, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb08] 
    /*00001ff0*/ STR         R3, [SP, #4] 
    /*00001ff4*/ MOVT        R2, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb08] 
    /*00001ff8*/ MOV         R3, #1136                   @ 0x470 
    /*00001ffc*/ STR         R3, [SP] 
    /*00002000*/ LDR         R3, [PC, #16]               @ 0x0000000000002018 
    /*00002004*/ BL          0x0000000000002004          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002008*/ MOVW        R0, #32771                  @ 0x8003 
    /*0000200c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002010*/ ADD         SP, SP, #20 
    /*00002014*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00002018*/ ANDEQ       R0, R0, IP, ROR #3 
FUNC_END RgnCheckMscBlkSz


FUNC_BEGIN RgnCheckStatic
    /*0000201c*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00002020*/ SUBS        R8, R1, #0 
    /*00002024*/ SUB         SP, SP, #8 
    /*00002028*/ MOV         R6, R0 
    /*0000202c*/ MOV         R7, R2 
    /*00002030*/ MOV         R4, R3 
    /*00002034*/ LDR         R5, [SP, #32] 
    /*00002038*/ BEQ         RgnCheckStatic_x249c
    RgnCheckStatic_x203c:
    /*0000203c*/ CMP         R7, #0 
    /*00002040*/ BEQ         RgnCheckStatic_x245c
    RgnCheckStatic_x2044:
    /*00002044*/ CMP         R4, #0 
    /*00002048*/ BEQ         RgnCheckStatic_x247c
    RgnCheckStatic_x204c:
    /*0000204c*/ CMP         R6, #4 
    /*00002050*/ BHI         RgnCheckStatic_x20f4
    RgnCheckStatic_x2054:
    /*00002054*/ LDR         R3, [R4] 
    /*00002058*/ CMP         R5, R3 
    /*0000205c*/ BEQ         RgnCheckStatic_x2414
    RgnCheckStatic_x2060:
    /*00002060*/ SUB         R6, R6, #1 
    /*00002064*/ CMP         R6, #1 
    /*00002068*/ BLS         RgnCheckStatic_x230c
    RgnCheckStatic_x206c:
    /*0000206c*/ LDR         R3, [R4, #16] 
    /*00002070*/ CMP         R5, R3 
    /*00002074*/ BEQ         RgnCheckStatic_x2374
    RgnCheckStatic_x2078:
    /*00002078*/ LDR         R3, [R4, #48]               @ 0x30 
    /*0000207c*/ CMP         R5, R3 
    /*00002080*/ BEQ         RgnCheckStatic_x23c4
    RgnCheckStatic_x2084:
    /*00002084*/ LDR         R3, [R4, #128]              @ 0x80 
    /*00002088*/ CMP         R5, R3 
    /*0000208c*/ BEQ         RgnCheckStatic_x210c
    RgnCheckStatic_x2090:
    /*00002090*/ LDR         R3, [R4, #160]              @ 0xa0 
    /*00002094*/ CMP         R5, R3 
    /*00002098*/ BEQ         RgnCheckStatic_x2154
    RgnCheckStatic_x209c:
    /*0000209c*/ LDR         R3, [R4, #180]              @ 0xb4 
    /*000020a0*/ CMP         R5, R3 
    /*000020a4*/ BEQ         RgnCheckStatic_x219c
    RgnCheckStatic_x20a8:
    /*000020a8*/ LDR         R3, [R4, #200]              @ 0xc8 
    /*000020ac*/ CMP         R5, R3 
    /*000020b0*/ BEQ         RgnCheckStatic_x21e4
    RgnCheckStatic_x20b4:
    /*000020b4*/ LDR         R3, [R4, #220]              @ 0xdc 
    /*000020b8*/ CMP         R5, R3 
    /*000020bc*/ BEQ         RgnCheckStatic_x222c
    RgnCheckStatic_x20c0:
    /*000020c0*/ LDR         R3, [R4, #248]              @ 0xf8 
    /*000020c4*/ CMP         R5, R3 
    /*000020c8*/ BEQ         RgnCheckStatic_x2274
    RgnCheckStatic_x20cc:
    /*000020cc*/ LDR         R3, [R4, #288]              @ 0x120 
    /*000020d0*/ CMP         R5, R3 
    /*000020d4*/ BEQ         RgnCheckStatic_x22c4
    RgnCheckStatic_x20d8:
    /*000020d8*/ LDR         R2, [R8, #108]              @ 0x6c 
    /*000020dc*/ LDR         R3, [R7, #108]              @ 0x6c 
    /*000020e0*/ CMP         R2, R3 
    /*000020e4*/ MOVEQ       R0, #0 
    /*000020e8*/ BNE         RgnCheckStatic_x24bc
    RgnCheckStatic_x20ec:
    /*000020ec*/ ADD         SP, SP, #8 
    /*000020f0*/ POP         {R4, R5, R6, R7, R8, PC} 
    RgnCheckStatic_x20f4:
    /*000020f4*/ MOV         R0, R6 
    /*000020f8*/ BL          0x00000000000002BC 
    /*000020fc*/ CMP         R0, #0 
    /*00002100*/ BEQ         RgnCheckStatic_x2054
    /*00002104*/ ADD         SP, SP, #8 
    /*00002108*/ POP         {R4, R5, R6, R7, R8, PC} 
    RgnCheckStatic_x210c:
    /*0000210c*/ LDR         R3, [R4, #132]              @ 0x84 
    /*00002110*/ CMP         R3, #0 
    /*00002114*/ BNE         RgnCheckStatic_x2090
    /*00002118*/ LDR         R2, [R8, #112]              @ 0x70 
    /*0000211c*/ LDR         R3, [R7, #112]              @ 0x70 
    /*00002120*/ CMP         R2, R3 
    /*00002124*/ BEQ         RgnCheckStatic_x2090
    /*00002128*/ MOV         R1, #3 
    /*0000212c*/ MOV         R3, #1216                   @ 0x4c0 
    /*00002130*/ MOV         R0, R1 
    /*00002134*/ STR         R3, [SP] 
    /*00002138*/ MOVW        R2, #0                      @ .LC67 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xca4] 
    /*0000213c*/ LDR         R3, [PC, #932]              @ 0x00000000000024e8 
    /*00002140*/ MOVT        R2, #0                      @ .LC67 [R_ARM_MOVT_ABS: .rodata.str1.4+0xca4] 
    /*00002144*/ BL          0x0000000000002144          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002148*/ MOVW        R0, #32777                  @ 0x8009 
    /*0000214c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002150*/ B           RgnCheckStatic_x20ec
    RgnCheckStatic_x2154:
    /*00002154*/ LDR         R3, [R4, #164]              @ 0xa4 
    /*00002158*/ CMP         R3, #0 
    /*0000215c*/ BNE         RgnCheckStatic_x209c
    /*00002160*/ LDR         R2, [R8, #76]               @ 0x4c 
    /*00002164*/ LDR         R3, [R7, #76]               @ 0x4c 
    /*00002168*/ CMP         R2, R3 
    /*0000216c*/ BEQ         RgnCheckStatic_x209c
    /*00002170*/ MOV         R1, #3 
    /*00002174*/ MOVW        R3, #1230                   @ 0x4ce 
    /*00002178*/ MOV         R0, R1 
    /*0000217c*/ STR         R3, [SP] 
    /*00002180*/ MOVW        R2, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xce4] 
    /*00002184*/ LDR         R3, [PC, #860]              @ 0x00000000000024e8 
    /*00002188*/ MOVT        R2, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0xce4] 
    /*0000218c*/ BL          0x000000000000218C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002190*/ MOVW        R0, #32777                  @ 0x8009 
    /*00002194*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002198*/ B           RgnCheckStatic_x20ec
    RgnCheckStatic_x219c:
    /*0000219c*/ LDR         R3, [R4, #184]              @ 0xb8 
    /*000021a0*/ CMP         R3, #0 
    /*000021a4*/ BNE         RgnCheckStatic_x20a8
    /*000021a8*/ LDR         R2, [R8, #80]               @ 0x50 
    /*000021ac*/ LDR         R3, [R7, #80]               @ 0x50 
    /*000021b0*/ CMP         R2, R3 
    /*000021b4*/ BEQ         RgnCheckStatic_x20a8
    /*000021b8*/ MOV         R1, #3 
    /*000021bc*/ MOVW        R3, #1243                   @ 0x4db 
    /*000021c0*/ MOV         R0, R1 
    /*000021c4*/ STR         R3, [SP] 
    /*000021c8*/ MOVW        R2, #0                      @ .LC69 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd24] 
    /*000021cc*/ LDR         R3, [PC, #788]              @ 0x00000000000024e8 
    /*000021d0*/ MOVT        R2, #0                      @ .LC69 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd24] 
    /*000021d4*/ BL          0x00000000000021D4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000021d8*/ MOVW        R0, #32777                  @ 0x8009 
    /*000021dc*/ MOVT        R0, #40963                  @ 0xa003 
    /*000021e0*/ B           RgnCheckStatic_x20ec
    RgnCheckStatic_x21e4:
    /*000021e4*/ LDR         R3, [R4, #204]              @ 0xcc 
    /*000021e8*/ CMP         R3, #0 
    /*000021ec*/ BNE         RgnCheckStatic_x20b4
    /*000021f0*/ LDR         R2, [R8, #72]               @ 0x48 
    /*000021f4*/ LDR         R3, [R7, #72]               @ 0x48 
    /*000021f8*/ CMP         R2, R3 
    /*000021fc*/ BEQ         RgnCheckStatic_x20b4
    /*00002200*/ MOV         R1, #3 
    /*00002204*/ MOVW        R3, #1256                   @ 0x4e8 
    /*00002208*/ MOV         R0, R1 
    /*0000220c*/ STR         R3, [SP] 
    /*00002210*/ MOVW        R2, #0                      @ .LC70 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd64] 
    /*00002214*/ LDR         R3, [PC, #716]              @ 0x00000000000024e8 
    /*00002218*/ MOVT        R2, #0                      @ .LC70 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd64] 
    /*0000221c*/ BL          0x000000000000221C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002220*/ MOVW        R0, #32777                  @ 0x8009 
    /*00002224*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002228*/ B           RgnCheckStatic_x20ec
    RgnCheckStatic_x222c:
    /*0000222c*/ LDR         R3, [R4, #224]              @ 0xe0 
    /*00002230*/ CMP         R3, #0 
    /*00002234*/ BNE         RgnCheckStatic_x20c0
    /*00002238*/ LDR         R2, [R8, #68]               @ 0x44 
    /*0000223c*/ LDR         R3, [R7, #68]               @ 0x44 
    /*00002240*/ CMP         R2, R3 
    /*00002244*/ BEQ         RgnCheckStatic_x20c0
    /*00002248*/ MOV         R1, #3 
    /*0000224c*/ MOVW        R3, #1269                   @ 0x4f5 
    /*00002250*/ MOV         R0, R1 
    /*00002254*/ STR         R3, [SP] 
    /*00002258*/ MOVW        R2, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xda8] 
    /*0000225c*/ LDR         R3, [PC, #644]              @ 0x00000000000024e8 
    /*00002260*/ MOVT        R2, #0                      @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0xda8] 
    /*00002264*/ BL          0x0000000000002264          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002268*/ MOVW        R0, #32777                  @ 0x8009 
    /*0000226c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002270*/ B           RgnCheckStatic_x20ec
    RgnCheckStatic_x2274:
    /*00002274*/ LDR         R3, [R4, #252]              @ 0xfc 
    /*00002278*/ CMP         R3, #0 
    /*0000227c*/ BNE         RgnCheckStatic_x20cc
    /*00002280*/ MOV         R2, #12 
    /*00002284*/ ADD         R1, R7, #120                @ 0x78 
    /*00002288*/ ADD         R0, R8, #120                @ 0x78 
    /*0000228c*/ BL          0x000000000000228C          @ osal_memcmp [R_ARM_CALL: +0x0] 
    /*00002290*/ CMP         R0, #0 
    /*00002294*/ BEQ         RgnCheckStatic_x20cc
    /*00002298*/ MOV         R1, #3 
    /*0000229c*/ MOVW        R3, #1282                   @ 0x502 
    /*000022a0*/ MOV         R0, R1 
    /*000022a4*/ STR         R3, [SP] 
    /*000022a8*/ MOVW        R2, #0                      @ .LC72 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xde8] 
    /*000022ac*/ LDR         R3, [PC, #564]              @ 0x00000000000024e8 
    /*000022b0*/ MOVT        R2, #0                      @ .LC72 [R_ARM_MOVT_ABS: .rodata.str1.4+0xde8] 
    /*000022b4*/ BL          0x00000000000022B4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000022b8*/ MOVW        R0, #32777                  @ 0x8009 
    /*000022bc*/ MOVT        R0, #40963                  @ 0xa003 
    /*000022c0*/ B           RgnCheckStatic_x20ec
    RgnCheckStatic_x22c4:
    /*000022c4*/ LDR         R3, [R4, #292]              @ 0x124 
    /*000022c8*/ CMP         R3, #0 
    /*000022cc*/ BNE         RgnCheckStatic_x20d8
    /*000022d0*/ LDR         R2, [R8, #156]              @ 0x9c 
    /*000022d4*/ LDR         R3, [R7, #156]              @ 0x9c 
    /*000022d8*/ CMP         R2, R3 
    /*000022dc*/ BEQ         RgnCheckStatic_x20d8
    /*000022e0*/ MOV         R1, #3 
    /*000022e4*/ MOVW        R3, #1308                   @ 0x51c 
    /*000022e8*/ MOV         R0, R1 
    /*000022ec*/ STR         R3, [SP] 
    /*000022f0*/ MOVW        R2, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe20] 
    /*000022f4*/ LDR         R3, [PC, #492]              @ 0x00000000000024e8 
    /*000022f8*/ MOVT        R2, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe20] 
    /*000022fc*/ BL          0x00000000000022FC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002300*/ MOVW        R0, #32777                  @ 0x8009 
    /*00002304*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002308*/ B           RgnCheckStatic_x20ec
    RgnCheckStatic_x230c:
    /*0000230c*/ LDR         R3, [R7, #4] 
    /*00002310*/ CMP         R3, #1 
    /*00002314*/ BNE         RgnCheckStatic_x206c
    /*00002318*/ LDR         R3, [R4, #84]               @ 0x54 
    /*0000231c*/ CMP         R5, R3 
    /*00002320*/ BNE         RgnCheckStatic_x2084
    /*00002324*/ LDR         R3, [R4, #88]               @ 0x58 
    /*00002328*/ CMP         R3, #0 
    /*0000232c*/ BNE         RgnCheckStatic_x2084
    /*00002330*/ MOV         R2, #40                     @ 0x28 
    /*00002334*/ ADD         R1, R7, #24 
    /*00002338*/ ADD         R0, R8, #24 
    /*0000233c*/ BL          0x000000000000233C          @ osal_memcmp [R_ARM_CALL: +0x0] 
    /*00002340*/ CMP         R0, #0 
    /*00002344*/ BEQ         RgnCheckStatic_x2084
    /*00002348*/ MOV         R1, #3 
    /*0000234c*/ MOVW        R3, #1174                   @ 0x496 
    /*00002350*/ MOV         R0, R1 
    /*00002354*/ STR         R3, [SP] 
    /*00002358*/ MOVW        R2, #0                      @ .LC64 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbdc] 
    /*0000235c*/ LDR         R3, [PC, #388]              @ 0x00000000000024e8 
    /*00002360*/ MOVT        R2, #0                      @ .LC64 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbdc] 
    /*00002364*/ BL          0x0000000000002364          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002368*/ MOVW        R0, #32777                  @ 0x8009 
    /*0000236c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002370*/ B           RgnCheckStatic_x20ec
    RgnCheckStatic_x2374:
    /*00002374*/ LDR         R3, [R4, #20] 
    /*00002378*/ CMP         R3, #0 
    /*0000237c*/ BNE         RgnCheckStatic_x2078
    /*00002380*/ MOV         R2, #8 
    /*00002384*/ ADD         R1, R7, R2 
    /*00002388*/ ADD         R0, R8, R2 
    /*0000238c*/ BL          0x000000000000238C          @ osal_memcmp [R_ARM_CALL: +0x0] 
    /*00002390*/ CMP         R0, #0 
    /*00002394*/ BEQ         RgnCheckStatic_x2078
    /*00002398*/ MOV         R1, #3 
    /*0000239c*/ MOVW        R3, #1189                   @ 0x4a5 
    /*000023a0*/ MOV         R0, R1 
    /*000023a4*/ STR         R3, [SP] 
    /*000023a8*/ MOVW        R2, #0                      @ .LC65 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc2c] 
    /*000023ac*/ LDR         R3, [PC, #308]              @ 0x00000000000024e8 
    /*000023b0*/ MOVT        R2, #0                      @ .LC65 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc2c] 
    /*000023b4*/ BL          0x00000000000023B4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000023b8*/ MOVW        R0, #32777                  @ 0x8009 
    /*000023bc*/ MOVT        R0, #40963                  @ 0xa003 
    /*000023c0*/ B           RgnCheckStatic_x20ec
    RgnCheckStatic_x23c4:
    /*000023c4*/ LDR         R3, [R4, #52]               @ 0x34 
    /*000023c8*/ CMP         R3, #0 
    /*000023cc*/ BNE         RgnCheckStatic_x2084
    /*000023d0*/ MOV         R2, #8 
    /*000023d4*/ ADD         R1, R7, #16 
    /*000023d8*/ ADD         R0, R8, #16 
    /*000023dc*/ BL          0x00000000000023DC          @ osal_memcmp [R_ARM_CALL: +0x0] 
    /*000023e0*/ CMP         R0, #0 
    /*000023e4*/ BEQ         RgnCheckStatic_x2084
    /*000023e8*/ MOV         R1, #3 
    /*000023ec*/ MOVW        R3, #1202                   @ 0x4b2 
    /*000023f0*/ MOV         R0, R1 
    /*000023f4*/ STR         R3, [SP] 
    /*000023f8*/ MOVW        R2, #0                      @ .LC66 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc68] 
    /*000023fc*/ LDR         R3, [PC, #228]              @ 0x00000000000024e8 
    /*00002400*/ MOVT        R2, #0                      @ .LC66 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc68] 
    /*00002404*/ BL          0x0000000000002404          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002408*/ MOVW        R0, #32777                  @ 0x8009 
    /*0000240c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002410*/ B           RgnCheckStatic_x20ec
    RgnCheckStatic_x2414:
    /*00002414*/ LDR         R3, [R4, #4] 
    /*00002418*/ CMP         R3, #0 
    /*0000241c*/ BNE         RgnCheckStatic_x2060
    /*00002420*/ LDR         R2, [R8, #64]               @ 0x40 
    /*00002424*/ LDR         R3, [R7, #64]               @ 0x40 
    /*00002428*/ CMP         R2, R3 
    /*0000242c*/ BEQ         RgnCheckStatic_x2060
    /*00002430*/ MOV         R1, #3 
    /*00002434*/ MOVW        R3, #1159                   @ 0x487 
    /*00002438*/ MOV         R0, R1 
    /*0000243c*/ STR         R3, [SP] 
    /*00002440*/ MOVW        R2, #0                      @ .LC63 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xba0] 
    /*00002444*/ LDR         R3, [PC, #156]              @ 0x00000000000024e8 
    /*00002448*/ MOVT        R2, #0                      @ .LC63 [R_ARM_MOVT_ABS: .rodata.str1.4+0xba0] 
    /*0000244c*/ BL          0x000000000000244C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002450*/ MOVW        R0, #32777                  @ 0x8009 
    /*00002454*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002458*/ B           RgnCheckStatic_x20ec
    RgnCheckStatic_x245c:
    /*0000245c*/ MOVW        R3, #0                      @ .LC61 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb6c] 
    /*00002460*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002464*/ MOVT        R3, #0                      @ .LC61 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb6c] 
    /*00002468*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*0000246c*/ MOVW        R2, #1148                   @ 0x47c 
    /*00002470*/ LDR         R1, [PC, #112]              @ 0x00000000000024e8 
    /*00002474*/ BL          0x0000000000002474          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002478*/ B           RgnCheckStatic_x2044
    RgnCheckStatic_x247c:
    /*0000247c*/ MOVW        R3, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb88] 
    /*00002480*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002484*/ MOVT        R3, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb88] 
    /*00002488*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*0000248c*/ MOVW        R2, #1149                   @ 0x47d 
    /*00002490*/ LDR         R1, [PC, #80]               @ 0x00000000000024e8 
    /*00002494*/ BL          0x0000000000002494          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002498*/ B           RgnCheckStatic_x204c
    RgnCheckStatic_x249c:
    /*0000249c*/ MOVW        R3, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb50] 
    /*000024a0*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000024a4*/ MOVT        R3, #0                      @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb50] 
    /*000024a8*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000024ac*/ MOVW        R2, #1147                   @ 0x47b 
    /*000024b0*/ LDR         R1, [PC, #48]               @ 0x00000000000024e8 
    /*000024b4*/ BL          0x00000000000024B4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000024b8*/ B           RgnCheckStatic_x203c
    RgnCheckStatic_x24bc:
    /*000024bc*/ MOV         R1, #3 
    /*000024c0*/ MOVW        R3, #1316                   @ 0x524 
    /*000024c4*/ MOV         R0, R1 
    /*000024c8*/ STR         R3, [SP] 
    /*000024cc*/ MOVW        R2, #0                      @ .LC74 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe68] 
    /*000024d0*/ LDR         R3, [PC, #16]               @ 0x00000000000024e8 
    /*000024d4*/ MOVT        R2, #0                      @ .LC74 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe68] 
    /*000024d8*/ BL          0x00000000000024D8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000024dc*/ MOVW        R0, #32777                  @ 0x8009 
    /*000024e0*/ MOVT        R0, #40963                  @ 0xa003 
    /*000024e4*/ B           RgnCheckStatic_x20ec
    /*000024e8*/ ANDEQ       R0, R0, R0, LSL #4 
FUNC_END RgnCheckStatic


FUNC_BEGIN RgnCheckBaseAttr
    /*000024ec*/ PUSH        {R4, R5, LR} 
    /*000024f0*/ SUBS        R5, R1, #0 
    /*000024f4*/ SUB         SP, SP, #20 
    /*000024f8*/ MOV         R4, R0 
    /*000024fc*/ BEQ         RgnCheckBaseAttr_x2610
    RgnCheckBaseAttr_x2500:
    /*00002500*/ CMP         R4, #4 
    /*00002504*/ BHI         RgnCheckBaseAttr_x2594
    RgnCheckBaseAttr_x2508:
    /*00002508*/ CMP         R4, #3 
    /*0000250c*/ CMPNE       R4, #0 
    /*00002510*/ MOVEQ       R0, #1 
    /*00002514*/ MOVNE       R0, #0 
    /*00002518*/ BNE         RgnCheckBaseAttr_x258c
    /*0000251c*/ LDR         R3, [R5, #16] 
    /*00002520*/ CMP         R4, #0 
    /*00002524*/ MOVW        R1, #8190                   @ 0x1ffe 
    /*00002528*/ SUB         R2, R3, #2 
    /*0000252c*/ BNE         RgnCheckBaseAttr_x25b4
    /*00002530*/ CMP         R2, R1 
    /*00002534*/ LDR         R2, [R5, #20] 
    /*00002538*/ BHI         RgnCheckBaseAttr_x2660
    /*0000253c*/ TST         R3, #1 
    /*00002540*/ BNE         RgnCheckBaseAttr_x2660
    /*00002544*/ SUB         R0, R2, #2 
    /*00002548*/ CMP         R0, R1 
    /*0000254c*/ BHI         RgnCheckBaseAttr_x2660
    /*00002550*/ TST         R2, #1 
    /*00002554*/ BNE         RgnCheckBaseAttr_x2660
    RgnCheckBaseAttr_x2558:
    /*00002558*/ LDR         R3, [R5, #112]              @ 0x70 
    /*0000255c*/ SUB         R0, R3, #8 
    /*00002560*/ SUB         R2, R3, #11 
    /*00002564*/ CMP         R0, #1 
    /*00002568*/ CMPHI       R2, #1 
    /*0000256c*/ MOVHI       R0, #1 
    /*00002570*/ MOVLS       R0, #0 
    /*00002574*/ BHI         RgnCheckBaseAttr_x2678
    /*00002578*/ CMP         R3, #12 
    /*0000257c*/ BNE         RgnCheckBaseAttr_x258c
    /*00002580*/ LDR         R2, [R5, #68]               @ 0x44 
    /*00002584*/ CMP         R2, #3 
    /*00002588*/ BHI         RgnCheckBaseAttr_x2630
    RgnCheckBaseAttr_x258c:
    /*0000258c*/ ADD         SP, SP, #20 
    /*00002590*/ POP         {R4, R5, PC} 
    RgnCheckBaseAttr_x2594:
    /*00002594*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fc] 
    /*00002598*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*0000259c*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fc] 
    /*000025a0*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000025a4*/ MOVW        R2, #1330                   @ 0x532 
    /*000025a8*/ LDR         R1, [PC, #248]              @ 0x00000000000026a8 
    /*000025ac*/ BL          0x00000000000025AC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000025b0*/ B           RgnCheckBaseAttr_x2508
    RgnCheckBaseAttr_x25b4:
    /*000025b4*/ CMP         R2, R1 
    /*000025b8*/ LDR         R2, [R5, #20] 
    /*000025bc*/ BHI         RgnCheckBaseAttr_x25dc
    /*000025c0*/ TST         R3, #1 
    /*000025c4*/ BNE         RgnCheckBaseAttr_x25dc
    /*000025c8*/ SUB         R0, R2, #2 
    /*000025cc*/ CMP         R0, R1 
    /*000025d0*/ BHI         RgnCheckBaseAttr_x25dc
    /*000025d4*/ TST         R2, #1 
    /*000025d8*/ BEQ         RgnCheckBaseAttr_x2558
    RgnCheckBaseAttr_x25dc:
    /*000025dc*/ MOV         R1, #3 
    /*000025e0*/ STR         R3, [SP, #4] 
    /*000025e4*/ MOV         R0, R1 
    /*000025e8*/ MOVW        R3, #1359                   @ 0x54f 
    /*000025ec*/ STR         R2, [SP, #8] 
    RgnCheckBaseAttr_x25f0:
    /*000025f0*/ STR         R3, [SP] 
    /*000025f4*/ MOVW        R2, #0                      @ .LC76 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xed4] 
    /*000025f8*/ LDR         R3, [PC, #168]              @ 0x00000000000026a8 
    /*000025fc*/ MOVT        R2, #0                      @ .LC76 [R_ARM_MOVT_ABS: .rodata.str1.4+0xed4] 
    /*00002600*/ BL          0x0000000000002600          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002604*/ MOVW        R0, #32771                  @ 0x8003 
    /*00002608*/ MOVT        R0, #40963                  @ 0xa003 
    /*0000260c*/ B           RgnCheckBaseAttr_x258c
    RgnCheckBaseAttr_x2610:
    /*00002610*/ MOVW        R3, #0                      @ .LC75 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xebc] 
    /*00002614*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002618*/ MOVT        R3, #0                      @ .LC75 [R_ARM_MOVT_ABS: .rodata.str1.4+0xebc] 
    /*0000261c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00002620*/ MOVW        R2, #1329                   @ 0x531 
    /*00002624*/ LDR         R1, [PC, #124]              @ 0x00000000000026a8 
    /*00002628*/ BL          0x0000000000002628          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000262c*/ B           RgnCheckBaseAttr_x2500
    RgnCheckBaseAttr_x2630:
    /*00002630*/ MOV         R1, #3 
    /*00002634*/ STMIB       SP, {R2, R3} 
    /*00002638*/ MOV         R0, R1 
    /*0000263c*/ MOVW        R3, #1379                   @ 0x563 
    /*00002640*/ MOVW        R2, #0                      @ .LC78 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf3c] 
    /*00002644*/ STR         R3, [SP] 
    /*00002648*/ MOVT        R2, #0                      @ .LC78 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf3c] 
    /*0000264c*/ LDR         R3, [PC, #84]               @ 0x00000000000026a8 
    /*00002650*/ BL          0x0000000000002650          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002654*/ MOVW        R0, #32771                  @ 0x8003 
    /*00002658*/ MOVT        R0, #40963                  @ 0xa003 
    /*0000265c*/ B           RgnCheckBaseAttr_x258c
    RgnCheckBaseAttr_x2660:
    /*00002660*/ MOV         R1, #3 
    /*00002664*/ STR         R3, [SP, #4] 
    /*00002668*/ STR         R2, [SP, #8] 
    /*0000266c*/ MOV         R0, R1 
    /*00002670*/ MOV         R3, #1344                   @ 0x540 
    /*00002674*/ B           RgnCheckBaseAttr_x25f0
    RgnCheckBaseAttr_x2678:
    /*00002678*/ MOV         R1, #3 
    /*0000267c*/ STR         R3, [SP, #4] 
    /*00002680*/ MOV         R0, R1 
    /*00002684*/ MOVW        R3, #1371                   @ 0x55b 
    /*00002688*/ MOVW        R2, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf08] 
    /*0000268c*/ STR         R3, [SP] 
    /*00002690*/ MOVT        R2, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf08] 
    /*00002694*/ LDR         R3, [PC, #12]               @ 0x00000000000026a8 
    /*00002698*/ BL          0x0000000000002698          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000269c*/ MOVW        R0, #32771                  @ 0x8003 
    /*000026a0*/ MOVT        R0, #40963                  @ 0xa003 
    /*000026a4*/ B           RgnCheckBaseAttr_x258c
    /*000026a8*/ ANDEQ       R0, R0, R0, LSL R2 
FUNC_END RgnCheckBaseAttr


FUNC_BEGIN RgnCheckPointAndSize
    /*000026ac*/ CMP         R0, #1 
    /*000026b0*/ PUSH        {R4, LR} 
    /*000026b4*/ MOV         R4, R1 
    /*000026b8*/ SUB         SP, SP, #8 
    /*000026bc*/ BEQ         RgnCheckPointAndSize_x26ec
    /*000026c0*/ CMP         R0, #2 
    /*000026c4*/ BEQ         RgnCheckPointAndSize_x26f8
    RgnCheckPointAndSize_x26c8:
    /*000026c8*/ LDR         R1, [R2, #16] 
    /*000026cc*/ CMP         R3, R1 
    /*000026d0*/ BEQ         RgnCheckPointAndSize_x2738
    RgnCheckPointAndSize_x26d4:
    /*000026d4*/ LDR         R1, [R2, #48]               @ 0x30 
    /*000026d8*/ CMP         R3, R1 
    /*000026dc*/ BEQ         RgnCheckPointAndSize_x2724
    RgnCheckPointAndSize_x26e0:
    /*000026e0*/ MOV         R0, #0 
    RgnCheckPointAndSize_x26e4:
    /*000026e4*/ ADD         SP, SP, #8 
    /*000026e8*/ POP         {R4, PC} 
    RgnCheckPointAndSize_x26ec:
    /*000026ec*/ LDR         R1, [R1, #160]              @ 0xa0 
    /*000026f0*/ CMP         R1, #1 
    /*000026f4*/ BEQ         RgnCheckPointAndSize_x275c
    RgnCheckPointAndSize_x26f8:
    /*000026f8*/ LDR         R1, [R4, #4] 
    /*000026fc*/ CMP         R1, #1 
    /*00002700*/ BNE         RgnCheckPointAndSize_x26c8
    /*00002704*/ LDR         R1, [R2, #84]               @ 0x54 
    /*00002708*/ CMP         R3, R1 
    /*0000270c*/ BNE         RgnCheckPointAndSize_x26e0
    /*00002710*/ ADD         R1, R4, #24 
    /*00002714*/ ADD         R0, R2, #84                 @ 0x54 
    /*00002718*/ ADD         SP, SP, #8 
    /*0000271c*/ POP         {R4, LR} 
    RgnCheckPointAndSize_x2720:
    /*00002720*/ B           RgnCheckPointAndSize_x2720  @ RgnCheckQuadCoverAttr [R_ARM_JUMP24: .text+0x1738] 
    RgnCheckPointAndSize_x2724:
    /*00002724*/ ADD         R1, R4, #16 
    /*00002728*/ ADD         R0, R2, #48                 @ 0x30 
    /*0000272c*/ ADD         SP, SP, #8 
    /*00002730*/ POP         {R4, LR} 
    RgnCheckPointAndSize_x2734:
    /*00002734*/ B           RgnCheckPointAndSize_x2734  @ RgnCheckSize [R_ARM_JUMP24: .text+0x1a80] 
    RgnCheckPointAndSize_x2738:
    /*00002738*/ ADD         R0, R2, #16 
    /*0000273c*/ ADD         R1, R4, #8 
    /*00002740*/ STR         R3, [SP, #4] 
    /*00002744*/ STR         R2, [SP] 
    /*00002748*/ BL          0x0000000000002748          @ RgnCheckPoint [R_ARM_CALL: .text+0x15c4] 
    /*0000274c*/ LDM         SP, {R2, R3} 
    /*00002750*/ CMP         R0, #0 
    /*00002754*/ BEQ         RgnCheckPointAndSize_x26d4
    /*00002758*/ B           RgnCheckPointAndSize_x26e4
    RgnCheckPointAndSize_x275c:
    /*0000275c*/ LDR         R1, [R4, #4] 
    /*00002760*/ CMP         R1, #1 
    /*00002764*/ BEQ         RgnCheckPointAndSize_x27b0
    /*00002768*/ LDR         R1, [R2, #16] 
    /*0000276c*/ CMP         R3, R1 
    /*00002770*/ BEQ         RgnCheckPointAndSize_x2794
    RgnCheckPointAndSize_x2774:
    /*00002774*/ LDR         R1, [R2, #48]               @ 0x30 
    /*00002778*/ CMP         R3, R1 
    /*0000277c*/ BNE         RgnCheckPointAndSize_x26e0
    /*00002780*/ ADD         R1, R4, #16 
    /*00002784*/ ADD         R0, R2, #48                 @ 0x30 
    /*00002788*/ ADD         SP, SP, #8 
    /*0000278c*/ POP         {R4, LR} 
    RgnCheckPointAndSize_x2790:
    /*00002790*/ B           RgnCheckPointAndSize_x2790  @ RgnCheckCoordinateSize [R_ARM_JUMP24: .text+0x1bcc] 
    RgnCheckPointAndSize_x2794:
    /*00002794*/ ADD         R0, R4, #8 
    /*00002798*/ STM         SP, {R2, R3} 
    /*0000279c*/ BL          0x000000000000279C          @ RgnCheckCoordinatePoint [R_ARM_CALL: .text+0x16c8] 
    /*000027a0*/ LDM         SP, {R2, R3} 
    /*000027a4*/ CMP         R0, #0 
    /*000027a8*/ BEQ         RgnCheckPointAndSize_x2774
    /*000027ac*/ B           RgnCheckPointAndSize_x26e4
    RgnCheckPointAndSize_x27b0:
    /*000027b0*/ LDR         R1, [R2, #84]               @ 0x54 
    /*000027b4*/ CMP         R1, R3 
    /*000027b8*/ BNE         RgnCheckPointAndSize_x26e0
    /*000027bc*/ ADD         R1, R4, #24 
    /*000027c0*/ ADD         R0, R2, #84                 @ 0x54 
    /*000027c4*/ ADD         SP, SP, #8 
    /*000027c8*/ POP         {R4, LR} 
    RgnCheckPointAndSize_x27cc:
    /*000027cc*/ B           RgnCheckPointAndSize_x27cc  @ RgnCheckQuadCoverCoordinateAttr [R_ARM_JUMP24: .text+0x1948] 
FUNC_END RgnCheckPointAndSize


FUNC_BEGIN RgnCheckAttr
    /*000027d0*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*000027d4*/ SUBS        R5, R1, #0 
    /*000027d8*/ SUB         SP, SP, #16 
    /*000027dc*/ MOV         R6, R0 
    /*000027e0*/ MOV         R4, R2 
    /*000027e4*/ MOV         R7, R3 
    /*000027e8*/ BEQ         RgnCheckAttr_x2a30
    RgnCheckAttr_x27ec:
    /*000027ec*/ CMP         R6, #4 
    /*000027f0*/ BHI         RgnCheckAttr_x2900
    RgnCheckAttr_x27f4:
    /*000027f4*/ CMP         R4, #0 
    /*000027f8*/ BEQ         RgnCheckAttr_x2a10
    RgnCheckAttr_x27fc:
    /*000027fc*/ LDR         R3, [R4, #316]              @ 0x13c 
    /*00002800*/ CMP         R3, #0 
    /*00002804*/ BEQ         RgnCheckAttr_x2ae0
    /*00002808*/ LDR         R3, [R5] 
    /*0000280c*/ CMP         R3, #1 
    /*00002810*/ BHI         RgnCheckAttr_x2a50
    /*00002814*/ LDR         R3, [R5, #160]              @ 0xa0 
    /*00002818*/ CMP         R3, #1 
    /*0000281c*/ BHI         RgnCheckAttr_x2a80
    /*00002820*/ LDR         R3, [R4] 
    /*00002824*/ CMP         R3, R7 
    /*00002828*/ BEQ         RgnCheckAttr_x29f8
    RgnCheckAttr_x282c:
    /*0000282c*/ SUB         R3, R6, #1 
    /*00002830*/ CMP         R3, #1 
    /*00002834*/ BLS         RgnCheckAttr_x28a8
    RgnCheckAttr_x2838:
    /*00002838*/ MOV         R0, R6 
    /*0000283c*/ MOV         R3, R7 
    /*00002840*/ MOV         R2, R4 
    /*00002844*/ MOV         R1, R5 
    /*00002848*/ BL          0x0000000000002848          @ RgnCheckPointAndSize [R_ARM_CALL: .text+0x26ac] 
    /*0000284c*/ SUBS        R8, R0, #0 
    /*00002850*/ BNE         RgnCheckAttr_x289c
    /*00002854*/ LDR         R3, [R4, #124]              @ 0x7c 
    /*00002858*/ CMP         R3, #1 
    /*0000285c*/ BEQ         RgnCheckAttr_x2940
    RgnCheckAttr_x2860:
    /*00002860*/ LDR         R3, [R4, #156]              @ 0x9c 
    /*00002864*/ CMP         R3, #1 
    /*00002868*/ BEQ         RgnCheckAttr_x2968
    RgnCheckAttr_x286c:
    /*0000286c*/ LDR         R3, [R4, #176]              @ 0xb0 
    /*00002870*/ CMP         R3, #1 
    /*00002874*/ BEQ         RgnCheckAttr_x298c
    RgnCheckAttr_x2878:
    /*00002878*/ LDR         R3, [R4, #196]              @ 0xc4 
    /*0000287c*/ CMP         R3, #1 
    /*00002880*/ BEQ         RgnCheckAttr_x29b0
    RgnCheckAttr_x2884:
    /*00002884*/ LDR         R3, [R4, #244]              @ 0xf4 
    /*00002888*/ CMP         R3, #1 
    /*0000288c*/ BEQ         RgnCheckAttr_x29d4
    RgnCheckAttr_x2890:
    /*00002890*/ LDR         R3, [R4, #284]              @ 0x11c 
    /*00002894*/ CMP         R3, #1 
    /*00002898*/ BEQ         RgnCheckAttr_x2920
    RgnCheckAttr_x289c:
    /*0000289c*/ MOV         R0, R8 
    /*000028a0*/ ADD         SP, SP, #16 
    /*000028a4*/ POP         {R4, R5, R6, R7, R8, PC} 
    RgnCheckAttr_x28a8:
    /*000028a8*/ LDR         R3, [R5, #4] 
    /*000028ac*/ CMP         R3, #1 
    /*000028b0*/ BHI         RgnCheckAttr_x2ab0
    /*000028b4*/ LDR         R3, [R5, #68]               @ 0x44 
    /*000028b8*/ TST         R3, #-16777216              @ 0xff000000 
    /*000028bc*/ BEQ         RgnCheckAttr_x2838
    /*000028c0*/ MOV         R1, #3 
    /*000028c4*/ STR         R3, [SP, #4] 
    /*000028c8*/ MVN         R2, #-16777216              @ 0xff000000 
    /*000028cc*/ MOV         R3, #0 
    /*000028d0*/ STR         R2, [SP, #12] 
    /*000028d4*/ MOV         R0, R1 
    /*000028d8*/ STR         R3, [SP, #8] 
    /*000028dc*/ MOVW        R2, #0                      @ .LC84 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1090] 
    /*000028e0*/ MOVW        R3, #1522                   @ 0x5f2 
    /*000028e4*/ MOVT        R2, #0                      @ .LC84 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1090] 
    /*000028e8*/ STR         R3, [SP] 
    /*000028ec*/ MOVW        R8, #32771                  @ 0x8003 
    /*000028f0*/ LDR         R3, [PC, #532]              @ 0x0000000000002b0c 
    /*000028f4*/ MOVT        R8, #40963                  @ 0xa003 
    /*000028f8*/ BL          0x00000000000028F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000028fc*/ B           RgnCheckAttr_x289c
    RgnCheckAttr_x2900:
    /*00002900*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fc] 
    /*00002904*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002908*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fc] 
    /*0000290c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00002910*/ MOVW        R2, #1479                   @ 0x5c7 
    /*00002914*/ LDR         R1, [PC, #496]              @ 0x0000000000002b0c 
    /*00002918*/ BL          0x0000000000002918          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000291c*/ B           RgnCheckAttr_x27f4
    RgnCheckAttr_x2920:
    /*00002920*/ LDR         R3, [R4, #288]              @ 0x120 
    /*00002924*/ CMP         R7, R3 
    /*00002928*/ BNE         RgnCheckAttr_x289c
    /*0000292c*/ LDR         R1, [R5, #156]              @ 0x9c 
    /*00002930*/ ADD         R0, R4, #288                @ 0x120 
    /*00002934*/ ADD         SP, SP, #16 
    /*00002938*/ POP         {R4, R5, R6, R7, R8, LR} 
    RgnCheckAttr_x293c:
    /*0000293c*/ B           RgnCheckAttr_x293c          @ RgnCheckMscBlkSz [R_ARM_JUMP24: .text+0x1fb0] 
    RgnCheckAttr_x2940:
    /*00002940*/ LDR         R3, [R4, #128]              @ 0x80 
    /*00002944*/ CMP         R7, R3 
    /*00002948*/ BNE         RgnCheckAttr_x2860
    /*0000294c*/ LDR         R1, [R5, #112]              @ 0x70 
    /*00002950*/ ADD         R0, R4, #128                @ 0x80 
    /*00002954*/ BL          0x0000000000002954          @ RgnCheckPixl [R_ARM_CALL: .text+0x1c38] 
    /*00002958*/ CMP         R0, #0 
    /*0000295c*/ BEQ         RgnCheckAttr_x2860
    RgnCheckAttr_x2960:
    /*00002960*/ MOV         R8, R0 
    /*00002964*/ B           RgnCheckAttr_x289c
    RgnCheckAttr_x2968:
    /*00002968*/ LDR         R3, [R4, #160]              @ 0xa0 
    /*0000296c*/ CMP         R7, R3 
    /*00002970*/ BNE         RgnCheckAttr_x286c
    /*00002974*/ LDR         R1, [R5, #76]               @ 0x4c 
    /*00002978*/ ADD         R0, R4, #160                @ 0xa0 
    /*0000297c*/ BL          0x000000000000297C          @ RgnCheckAlpha [R_ARM_CALL: .text+0x1cfc] 
    /*00002980*/ CMP         R0, #0 
    /*00002984*/ BEQ         RgnCheckAttr_x286c
    /*00002988*/ B           RgnCheckAttr_x2960
    RgnCheckAttr_x298c:
    /*0000298c*/ LDR         R3, [R4, #180]              @ 0xb4 
    /*00002990*/ CMP         R7, R3 
    /*00002994*/ BNE         RgnCheckAttr_x2878
    /*00002998*/ LDR         R1, [R5, #80]               @ 0x50 
    /*0000299c*/ ADD         R0, R4, #180                @ 0xb4 
    /*000029a0*/ BL          0x00000000000029A0          @ RgnCheckAlpha [R_ARM_CALL: .text+0x1cfc] 
    /*000029a4*/ CMP         R0, #0 
    /*000029a8*/ BEQ         RgnCheckAttr_x2878
    /*000029ac*/ B           RgnCheckAttr_x2960
    RgnCheckAttr_x29b0:
    /*000029b0*/ LDR         R3, [R4, #200]              @ 0xc8 
    /*000029b4*/ CMP         R7, R3 
    /*000029b8*/ BNE         RgnCheckAttr_x2884
    /*000029bc*/ LDR         R1, [R5, #72]               @ 0x48 
    /*000029c0*/ ADD         R0, R4, #200                @ 0xc8 
    /*000029c4*/ BL          0x00000000000029C4          @ RgnCheckAlpha [R_ARM_CALL: .text+0x1cfc] 
    /*000029c8*/ CMP         R0, #0 
    /*000029cc*/ BEQ         RgnCheckAttr_x2884
    /*000029d0*/ B           RgnCheckAttr_x2960
    RgnCheckAttr_x29d4:
    /*000029d4*/ LDR         R3, [R4, #248]              @ 0xf8 
    /*000029d8*/ CMP         R7, R3 
    /*000029dc*/ BNE         RgnCheckAttr_x2890
    /*000029e0*/ ADD         R1, R5, #120                @ 0x78 
    /*000029e4*/ ADD         R0, R4, #248                @ 0xf8 
    /*000029e8*/ BL          0x00000000000029E8          @ RgnCheckQp [R_ARM_CALL: .text+0x1d68] 
    /*000029ec*/ CMP         R0, #0 
    /*000029f0*/ BEQ         RgnCheckAttr_x2890
    /*000029f4*/ B           RgnCheckAttr_x2960
    RgnCheckAttr_x29f8:
    /*000029f8*/ LDR         R1, [R5, #64]               @ 0x40 
    /*000029fc*/ MOV         R0, R4 
    /*00002a00*/ BL          0x0000000000002A00          @ RgnCheckLayer [R_ARM_CALL: .text+0x1558] 
    /*00002a04*/ SUBS        R8, R0, #0 
    /*00002a08*/ BEQ         RgnCheckAttr_x282c
    /*00002a0c*/ B           RgnCheckAttr_x289c
    RgnCheckAttr_x2a10:
    /*00002a10*/ MOVW        R3, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf8c] 
    /*00002a14*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002a18*/ MOVT        R3, #0                      @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf8c] 
    /*00002a1c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00002a20*/ MOVW        R2, #1480                   @ 0x5c8 
    /*00002a24*/ LDR         R1, [PC, #224]              @ 0x0000000000002b0c 
    /*00002a28*/ BL          0x0000000000002A28          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002a2c*/ B           RgnCheckAttr_x27fc
    RgnCheckAttr_x2a30:
    /*00002a30*/ MOVW        R3, #0                      @ .LC75 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xebc] 
    /*00002a34*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002a38*/ MOVT        R3, #0                      @ .LC75 [R_ARM_MOVT_ABS: .rodata.str1.4+0xebc] 
    /*00002a3c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00002a40*/ MOVW        R2, #1478                   @ 0x5c6 
    /*00002a44*/ LDR         R1, [PC, #192]              @ 0x0000000000002b0c 
    /*00002a48*/ BL          0x0000000000002A48          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002a4c*/ B           RgnCheckAttr_x27ec
    RgnCheckAttr_x2a50:
    /*00002a50*/ MOV         R1, #3 
    /*00002a54*/ STR         R3, [SP, #4] 
    /*00002a58*/ MOV         R0, R1 
    /*00002a5c*/ MOVW        R3, #1491                   @ 0x5d3 
    /*00002a60*/ MOVW        R2, #0                      @ .LC81 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfd8] 
    /*00002a64*/ STR         R3, [SP] 
    /*00002a68*/ MOVT        R2, #0                      @ .LC81 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfd8] 
    /*00002a6c*/ LDR         R3, [PC, #152]              @ 0x0000000000002b0c 
    /*00002a70*/ MOVW        R8, #32771                  @ 0x8003 
    /*00002a74*/ BL          0x0000000000002A74          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002a78*/ MOVT        R8, #40963                  @ 0xa003 
    /*00002a7c*/ B           RgnCheckAttr_x289c
    RgnCheckAttr_x2a80:
    /*00002a80*/ MOV         R1, #3 
    /*00002a84*/ STR         R3, [SP, #4] 
    /*00002a88*/ MOV         R0, R1 
    /*00002a8c*/ MOVW        R3, #1498                   @ 0x5da 
    /*00002a90*/ MOVW        R2, #0                      @ .LC82 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x100c] 
    /*00002a94*/ STR         R3, [SP] 
    /*00002a98*/ MOVT        R2, #0                      @ .LC82 [R_ARM_MOVT_ABS: .rodata.str1.4+0x100c] 
    /*00002a9c*/ LDR         R3, [PC, #104]              @ 0x0000000000002b0c 
    /*00002aa0*/ MOVW        R8, #32771                  @ 0x8003 
    /*00002aa4*/ BL          0x0000000000002AA4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002aa8*/ MOVT        R8, #40963                  @ 0xa003 
    /*00002aac*/ B           RgnCheckAttr_x289c
    RgnCheckAttr_x2ab0:
    /*00002ab0*/ MOV         R1, #3 
    /*00002ab4*/ STR         R3, [SP, #4] 
    /*00002ab8*/ MOV         R0, R1 
    /*00002abc*/ MOVW        R3, #1516                   @ 0x5ec 
    /*00002ac0*/ MOVW        R2, #0                      @ .LC83 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x105c] 
    /*00002ac4*/ STR         R3, [SP] 
    /*00002ac8*/ MOVT        R2, #0                      @ .LC83 [R_ARM_MOVT_ABS: .rodata.str1.4+0x105c] 
    /*00002acc*/ LDR         R3, [PC, #56]               @ 0x0000000000002b0c 
    /*00002ad0*/ MOVW        R8, #32771                  @ 0x8003 
    /*00002ad4*/ BL          0x0000000000002AD4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002ad8*/ MOVT        R8, #40963                  @ 0xa003 
    /*00002adc*/ B           RgnCheckAttr_x289c
    RgnCheckAttr_x2ae0:
    /*00002ae0*/ MOV         R1, #3 
    /*00002ae4*/ MOVW        R3, #1484                   @ 0x5cc 
    /*00002ae8*/ MOV         R0, R1 
    /*00002aec*/ STR         R3, [SP] 
    /*00002af0*/ MOVW        R2, #0                      @ .LC80 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfa4] 
    /*00002af4*/ LDR         R3, [PC, #16]               @ 0x0000000000002b0c 
    /*00002af8*/ MOVT        R2, #0                      @ .LC80 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfa4] 
    /*00002afc*/ MOVW        R8, #32776                  @ 0x8008 
    /*00002b00*/ BL          0x0000000000002B00          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002b04*/ MOVT        R8, #40963                  @ 0xa003 
    /*00002b08*/ B           RgnCheckAttr_x289c
    /*00002b0c*/ ANDEQ       R0, R0, R4, LSR #4 
FUNC_END RgnCheckAttr


FUNC_BEGIN RgnCheckBatch
    /*00002b10*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002b14*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002b18*/ ADD         R3, R3, R0, LSL #2 
    /*00002b1c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00002b20*/ SUB         SP, SP, #12 
    /*00002b24*/ LDR         R3, [R3, #8] 
    /*00002b28*/ CMP         R3, #0 
    /*00002b2c*/ BEQ         RgnCheckBatch_x2bf0
    /*00002b30*/ LDR         R2, [R3] 
    /*00002b34*/ CMP         R2, #0 
    /*00002b38*/ BEQ         RgnCheckBatch_x2b90
    /*00002b3c*/ LDR         R2, [R3, #208]              @ 0xd0 
    /*00002b40*/ CMP         R2, #0 
    /*00002b44*/ BNE         RgnCheckBatch_x2b5c
    /*00002b48*/ LDR         R0, [R3, #176]              @ 0xb0 
    /*00002b4c*/ CMP         R0, #0 
    /*00002b50*/ BNE         RgnCheckBatch_x2bc0
    RgnCheckBatch_x2b54:
    /*00002b54*/ ADD         SP, SP, #12 
    /*00002b58*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    RgnCheckBatch_x2b5c:
    /*00002b5c*/ MOV         R1, #3 
    /*00002b60*/ STR         R0, [SP, #4] 
    /*00002b64*/ MOVW        R3, #1656                   @ 0x678 
    /*00002b68*/ MOV         R0, R1 
    /*00002b6c*/ STR         R3, [SP] 
    /*00002b70*/ MOVW        R2, #0                      @ .LC88 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1198] 
    /*00002b74*/ LDR         R3, [PC, #164]              @ 0x0000000000002c20 
    /*00002b78*/ MOVT        R2, #0                      @ .LC88 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1198] 
    /*00002b7c*/ BL          0x0000000000002B7C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002b80*/ MOVW        R0, #32777                  @ 0x8009 
    /*00002b84*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002b88*/ ADD         SP, SP, #12 
    /*00002b8c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    RgnCheckBatch_x2b90:
    /*00002b90*/ MOV         R1, #3 
    /*00002b94*/ STR         R0, [SP, #4] 
    /*00002b98*/ MOVW        R3, #1650                   @ 0x672 
    /*00002b9c*/ MOV         R0, R1 
    /*00002ba0*/ STR         R3, [SP] 
    /*00002ba4*/ MOVW        R2, #0                      @ .LC86 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1124] 
    /*00002ba8*/ LDR         R3, [PC, #112]              @ 0x0000000000002c20 
    /*00002bac*/ MOVT        R2, #0                      @ .LC86 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1124] 
    /*00002bb0*/ BL          0x0000000000002BB0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002bb4*/ MOVW        R0, #32773                  @ 0x8005 
    /*00002bb8*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002bbc*/ B           RgnCheckBatch_x2b54
    RgnCheckBatch_x2bc0:
    /*00002bc0*/ MOV         R1, #3 
    /*00002bc4*/ STR         R0, [SP, #4] 
    /*00002bc8*/ MOVW        R3, #1662                   @ 0x67e 
    /*00002bcc*/ MOV         R0, R1 
    /*00002bd0*/ STR         R3, [SP] 
    /*00002bd4*/ MOVW        R2, #0                      @ .LC87 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1160] 
    /*00002bd8*/ LDR         R3, [PC, #64]               @ 0x0000000000002c20 
    /*00002bdc*/ MOVT        R2, #0                      @ .LC87 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1160] 
    /*00002be0*/ BL          0x0000000000002BE0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002be4*/ MOVW        R0, #32777                  @ 0x8009 
    /*00002be8*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002bec*/ B           RgnCheckBatch_x2b54
    RgnCheckBatch_x2bf0:
    /*00002bf0*/ MOV         R1, #3 
    /*00002bf4*/ STR         R0, [SP, #4] 
    /*00002bf8*/ MOVW        R3, #1644                   @ 0x66c 
    /*00002bfc*/ MOV         R0, R1 
    /*00002c00*/ STR         R3, [SP] 
    /*00002c04*/ MOVW        R2, #0                      @ .LC85 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10f0] 
    /*00002c08*/ LDR         R3, [PC, #16]               @ 0x0000000000002c20 
    /*00002c0c*/ MOVT        R2, #0                      @ .LC85 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10f0] 
    /*00002c10*/ BL          0x0000000000002C10          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002c14*/ MOVW        R0, #32772                  @ 0x8004 
    /*00002c18*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002c1c*/ B           RgnCheckBatch_x2b54
    /*00002c20*/ ANDEQ       R0, R0, R4, LSR R2 
FUNC_END RgnCheckBatch


FUNC_BEGIN RgnAbnormalVerifyAfterWait
    /*00002c24*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00002c28*/ LSL         R2, R0, #2 
    /*00002c2c*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00002c30*/ ADD         R3, R3, R2 
    /*00002c34*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00002c38*/ SUB         SP, SP, #12 
    /*00002c3c*/ LDR         R3, [R3, #-1604]            @ 0xfffff9bc 
    /*00002c40*/ CMP         R3, R1 
    /*00002c44*/ BNE         RgnAbnormalVerifyAfterWait_x2c80
    /*00002c48*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002c4c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002c50*/ ADD         R3, R3, R2 
    /*00002c54*/ LDR         R3, [R3, #8] 
    /*00002c58*/ CMP         R3, #0 
    /*00002c5c*/ BEQ         RgnAbnormalVerifyAfterWait_x2ce0
    /*00002c60*/ MOVW        R3, #0                      @ s_u32RgnState [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00002c64*/ MOVT        R3, #0                      @ s_u32RgnState [R_ARM_MOVT_ABS: .data+0x0] 
    /*00002c68*/ LDR         R3, [R3] 
    /*00002c6c*/ CMP         R3, #0 
    /*00002c70*/ MOVEQ       R0, R3 
    /*00002c74*/ BNE         RgnAbnormalVerifyAfterWait_x2cb0
    RgnAbnormalVerifyAfterWait_x2c78:
    /*00002c78*/ ADD         SP, SP, #12 
    /*00002c7c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    RgnAbnormalVerifyAfterWait_x2c80:
    /*00002c80*/ MOV         R1, #3 
    /*00002c84*/ STR         R0, [SP, #4] 
    /*00002c88*/ MOVW        R3, #1674                   @ 0x68a 
    /*00002c8c*/ MOV         R0, R1 
    /*00002c90*/ STR         R3, [SP] 
    /*00002c94*/ MOVW        R2, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11d4] 
    /*00002c98*/ LDR         R3, [PC, #112]              @ 0x0000000000002d10 
    /*00002c9c*/ MOVT        R2, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11d4] 
    /*00002ca0*/ BL          0x0000000000002CA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002ca4*/ MOVW        R0, #32784                  @ 0x8010 
    /*00002ca8*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002cac*/ B           RgnAbnormalVerifyAfterWait_x2c78
    RgnAbnormalVerifyAfterWait_x2cb0:
    /*00002cb0*/ MOV         R1, #3 
    /*00002cb4*/ STR         R0, [SP, #4] 
    /*00002cb8*/ MOVW        R3, #1684                   @ 0x694 
    /*00002cbc*/ MOV         R0, R1 
    /*00002cc0*/ STR         R3, [SP] 
    /*00002cc4*/ MOVW        R2, #0                      @ .LC91 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1240] 
    /*00002cc8*/ LDR         R3, [PC, #64]               @ 0x0000000000002d10 
    /*00002ccc*/ MOVT        R2, #0                      @ .LC91 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1240] 
    /*00002cd0*/ BL          0x0000000000002CD0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002cd4*/ MOVW        R0, #32784                  @ 0x8010 
    /*00002cd8*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002cdc*/ B           RgnAbnormalVerifyAfterWait_x2c78
    RgnAbnormalVerifyAfterWait_x2ce0:
    /*00002ce0*/ MOV         R1, #3 
    /*00002ce4*/ STR         R0, [SP, #4] 
    /*00002ce8*/ MOVW        R3, #1679                   @ 0x68f 
    /*00002cec*/ MOV         R0, R1 
    /*00002cf0*/ STR         R3, [SP] 
    /*00002cf4*/ MOVW        R2, #0                      @ .LC90 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x120c] 
    /*00002cf8*/ LDR         R3, [PC, #16]               @ 0x0000000000002d10 
    /*00002cfc*/ MOVT        R2, #0                      @ .LC90 [R_ARM_MOVT_ABS: .rodata.str1.4+0x120c] 
    /*00002d00*/ BL          0x0000000000002D00          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002d04*/ MOVW        R0, #32773                  @ 0x8005 
    /*00002d08*/ MOVT        R0, #40963                  @ 0xa003 
    /*00002d0c*/ B           RgnAbnormalVerifyAfterWait_x2c78
    /*00002d10*/ ANDEQ       R0, R0, R4, ASR #4 
FUNC_END RgnAbnormalVerifyAfterWait


FUNC_BEGIN RgnAllocBmpMem
    /*00002d14*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00002d18*/ SUBS        R4, R0, #0 
    /*00002d1c*/ SUB         SP, SP, #64                 @ 0x40 
    /*00002d20*/ MOV         R3, #0 
    /*00002d24*/ MOV         R6, R1 
    /*00002d28*/ STR         R3, [SP, #48]               @ 0x30 
    /*00002d2c*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00002d30*/ STR         R3, [SP, #52]               @ 0x34 
    /*00002d34*/ STR         R3, [SP, #56]               @ 0x38 
    /*00002d38*/ STR         R3, [SP, #60]               @ 0x3c 
    /*00002d3c*/ BEQ         RgnAllocBmpMem_x2fdc
    RgnAllocBmpMem_x2d40:
    /*00002d40*/ LDR         R5, [R4, #176]              @ 0xb0 
    /*00002d44*/ CMP         R5, #4 
    /*00002d48*/ BHI         RgnAllocBmpMem_x2f44
    RgnAllocBmpMem_x2d4c:
    /*00002d4c*/ CMP         R5, #0 
    /*00002d50*/ CMPNE       R5, #3 
    /*00002d54*/ MOVEQ       R5, #1 
    /*00002d58*/ MOVNE       R5, #0 
    /*00002d5c*/ BNE         RgnAllocBmpMem_x2f14
    /*00002d60*/ LDR         R3, [R4, #120]              @ 0x78 
    /*00002d64*/ SUB         R2, R3, #8 
    /*00002d68*/ CMP         R2, #1 
    /*00002d6c*/ BLS         RgnAllocBmpMem_x2f64
    /*00002d70*/ CMP         R3, #11 
    /*00002d74*/ BEQ         RgnAllocBmpMem_x2f6c
    /*00002d78*/ CMP         R3, #12 
    /*00002d7c*/ BEQ         RgnAllocBmpMem_x2fa0
    /*00002d80*/ MOVW        R3, #0                      @ .LC96 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1318] 
    /*00002d84*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002d88*/ MOVT        R3, #0                      @ .LC96 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1318] 
    /*00002d8c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00002d90*/ MOVW        R2, #1711                   @ 0x6af 
    /*00002d94*/ LDR         R1, [PC, #704]              @ 0x000000000000305c 
    /*00002d98*/ BL          0x0000000000002D98          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002d9c*/ MOV         SL, #0 
    RgnAllocBmpMem_x2da0:
    /*00002da0*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00002da4*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00002da8*/ LDR         R0, [R4, #24] 
    /*00002dac*/ LDR         R5, [R3, #-1092]            @ 0xfffffbbc 
    /*00002db0*/ SUB         R3, R5, #1 
    /*00002db4*/ MOV         R1, R5 
    /*00002db8*/ MLA         R0, R0, SL, R3 
    /*00002dbc*/ BL          0x0000000000002DBC          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00002dc0*/ MUL         R5, R5, R0 
    /*00002dc4*/ CMP         R5, #0 
    /*00002dc8*/ BEQ         RgnAllocBmpMem_x2ffc
    /*00002dcc*/ LDR         R3, [R4, #120]              @ 0x78 
    /*00002dd0*/ CMP         R3, #12 
    /*00002dd4*/ BEQ         RgnAllocBmpMem_x2fd0
    RgnAllocBmpMem_x2dd8:
    /*00002dd8*/ LDR         R7, [R4, #28] 
    /*00002ddc*/ MOV         R3, R6 
    /*00002de0*/ MOV         R2, #1 
    /*00002de4*/ MOV         R6, #0 
    /*00002de8*/ STR         R2, [R4, #232]              @ 0xe8 
    /*00002dec*/ MOV         R1, #20 
    /*00002df0*/ MUL         R7, R7, R5 
    /*00002df4*/ STR         R5, [R4, #268]              @ 0x10c 
    /*00002df8*/ STR         SL, [R4, #264]              @ 0x108 
    /*00002dfc*/ MOVW        R2, #0                      @ .LC94 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12d8] 
    /*00002e00*/ STR         R6, [R4, #216]              @ 0xd8 
    /*00002e04*/ MOVT        R2, #0                      @ .LC94 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12d8] 
    /*00002e08*/ STR         R6, [R4, #212]              @ 0xd4 
    /*00002e0c*/ ADD         R0, SP, #44                 @ 0x2c 
    /*00002e10*/ ADD         R8, R4, #248                @ 0xf8 
    /*00002e14*/ STR         R7, [R4, #240]              @ 0xf0 
    /*00002e18*/ BL          0x0000000000002E18          @ osal_snprintf [R_ARM_CALL: +0x0] 
    /*00002e1c*/ LDR         R3, [R4, #116]              @ 0x74 
    /*00002e20*/ MOV         R0, #2 
    /*00002e24*/ STR         R6, [SP, #28] 
    /*00002e28*/ MUL         R7, R3, R7 
    /*00002e2c*/ BL          0x0000000000002E2C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002e30*/ CMP         R0, R6 
    /*00002e34*/ BEQ         RgnAllocBmpMem_x3030
    /*00002e38*/ MOV         R0, #2 
    /*00002e3c*/ BL          0x0000000000002E3C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002e40*/ LDR         R3, [R0, #60]               @ 0x3c 
    /*00002e44*/ CMP         R3, R6 
    /*00002e48*/ BEQ         RgnAllocBmpMem_x3030
    /*00002e4c*/ MOV         R0, #2 
    /*00002e50*/ MOV         R3, #3 
    /*00002e54*/ STR         R6, [SP, #40]               @ 0x28 
    /*00002e58*/ STR         R6, [SP, #36]               @ 0x24 
    /*00002e5c*/ STR         R3, [SP, #32] 
    /*00002e60*/ BL          0x0000000000002E60          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00002e64*/ ADD         R1, SP, #24 
    /*00002e68*/ LDR         R3, [R0, #60]               @ 0x3c 
    /*00002e6c*/ ADD         R0, SP, #32 
    /*00002e70*/ BLX         R3 
    /*00002e74*/ STR         R7, [SP] 
    /*00002e78*/ MOV         R2, R8 
    /*00002e7c*/ ADD         R3, SP, #28 
    /*00002e80*/ LDR         R0, [SP, #24] 
    /*00002e84*/ ADD         R1, SP, #44                 @ 0x2c 
    /*00002e88*/ BL          0x0000000000002E88          @ CMPI_MmzMallocNocache [R_ARM_CALL: +0x0] 
    /*00002e8c*/ LDR         R6, [SP, #28] 
    /*00002e90*/ ADD         R3, R4, #256                @ 0x100 
    /*00002e94*/ MOV         R7, #0 
    /*00002e98*/ STRD        R6, [R3] 
    /*00002e9c*/ SUBS        R8, R0, #0 
    /*00002ea0*/ BNE         RgnAllocBmpMem_x3030
    /*00002ea4*/ LDR         R2, [R4, #212]              @ 0xd4 
    /*00002ea8*/ LDR         R3, [R4, #240]              @ 0xf0 
    /*00002eac*/ LDR         IP, [R4, #28] 
    /*00002eb0*/ UMULL       R2, R3, R2, R3 
    /*00002eb4*/ ADDS        R0, R6, R2 
    /*00002eb8*/ ADC         R1, R7, R3 
    /*00002ebc*/ LDRD        R6, [R4, #248]              @ 0xf8 
    /*00002ec0*/ STRD        R0, [SP, #8] 
    /*00002ec4*/ ADDS        R6, R6, R2 
    /*00002ec8*/ ADC         R7, R7, R3 
    /*00002ecc*/ CMP         IP, #0 
    /*00002ed0*/ STRD        R6, [SP, #16] 
    /*00002ed4*/ BEQ         RgnAllocBmpMem_x2f24
    /*00002ed8*/ MOV         R9, R0 
    /*00002edc*/ MOV         R6, R0 
    /*00002ee0*/ MOV         R7, R1 
    RgnAllocBmpMem_x2ee4:
    /*00002ee4*/ LDR         R3, [R4, #24] 
    /*00002ee8*/ MOV         R0, R9 
    /*00002eec*/ MOV         R2, SL 
    /*00002ef0*/ LDR         R1, [R4, #76]               @ 0x4c 
    /*00002ef4*/ BL          0x0000000000002EF4          @ RgnMemSet [R_ARM_CALL: .text+0xbd0] 
    /*00002ef8*/ LDR         R3, [R4, #28] 
    /*00002efc*/ ADD         R8, R8, #1 
    /*00002f00*/ ADD         R9, R9, R5 
    /*00002f04*/ CMP         R3, R8 
    /*00002f08*/ BHI         RgnAllocBmpMem_x2ee4
    /*00002f0c*/ STRD        R6, [SP, #8] 
    /*00002f10*/ B           RgnAllocBmpMem_x2f24
    RgnAllocBmpMem_x2f14:
    /*00002f14*/ MOV         R2, #0 
    /*00002f18*/ MOV         R3, #0 
    /*00002f1c*/ STRD        R2, [SP, #8] 
    /*00002f20*/ STRD        R2, [SP, #16] 
    RgnAllocBmpMem_x2f24:
    /*00002f24*/ LDRD        R2, [SP, #16] 
    /*00002f28*/ MOV         R0, #0 
    /*00002f2c*/ STR         R5, [R4, #112]              @ 0x70 
    /*00002f30*/ STRD        R2, [R4, #96]               @ 0x60 
    /*00002f34*/ LDRD        R2, [SP, #8] 
    /*00002f38*/ STRD        R2, [R4, #104]              @ 0x68 
    RgnAllocBmpMem_x2f3c:
    /*00002f3c*/ ADD         SP, SP, #64                 @ 0x40 
    /*00002f40*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    RgnAllocBmpMem_x2f44:
    /*00002f44*/ MOVW        R3, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1278] 
    /*00002f48*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002f4c*/ MOVT        R3, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1278] 
    /*00002f50*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00002f54*/ MOVW        R2, #1706                   @ 0x6aa 
    /*00002f58*/ LDR         R1, [PC, #252]              @ 0x000000000000305c 
    /*00002f5c*/ BL          0x0000000000002F5C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002f60*/ B           RgnAllocBmpMem_x2d4c
    RgnAllocBmpMem_x2f64:
    /*00002f64*/ MOV         SL, #2 
    /*00002f68*/ B           RgnAllocBmpMem_x2da0
    RgnAllocBmpMem_x2f6c:
    /*00002f6c*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00002f70*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00002f74*/ LDR         R0, [R4, #24] 
    /*00002f78*/ LDR         R5, [R3, #-1092]            @ 0xfffffbbc 
    /*00002f7c*/ SUB         R3, R5, #1 
    /*00002f80*/ MOV         R1, R5 
    /*00002f84*/ ADD         R0, R3, R0, LSL #2 
    /*00002f88*/ BL          0x0000000000002F88          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00002f8c*/ MUL         R5, R5, R0 
    /*00002f90*/ CMP         R5, #0 
    /*00002f94*/ BEQ         RgnAllocBmpMem_x2ffc
    /*00002f98*/ MOV         SL, #4 
    /*00002f9c*/ B           RgnAllocBmpMem_x2dd8
    RgnAllocBmpMem_x2fa0:
    /*00002fa0*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00002fa4*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00002fa8*/ LDR         R2, [R4, #24] 
    /*00002fac*/ LDR         R5, [R3, #-1092]            @ 0xfffffbbc 
    /*00002fb0*/ SUB         R0, R5, #1 
    /*00002fb4*/ MOV         R1, R5 
    /*00002fb8*/ ADD         R0, R0, R2 
    /*00002fbc*/ BL          0x0000000000002FBC          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00002fc0*/ MUL         R5, R5, R0 
    /*00002fc4*/ CMP         R5, #0 
    /*00002fc8*/ BEQ         RgnAllocBmpMem_x2ffc
    /*00002fcc*/ MOV         SL, #1 
    RgnAllocBmpMem_x2fd0:
    /*00002fd0*/ ADD         R5, R5, #3 
    /*00002fd4*/ LSR         R5, R5, #2 
    /*00002fd8*/ B           RgnAllocBmpMem_x2dd8
    RgnAllocBmpMem_x2fdc:
    /*00002fdc*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c] 
    /*00002fe0*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00002fe4*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c] 
    /*00002fe8*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00002fec*/ MOVW        R2, #1704                   @ 0x6a8 
    /*00002ff0*/ LDR         R1, [PC, #100]              @ 0x000000000000305c 
    /*00002ff4*/ BL          0x0000000000002FF4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002ff8*/ B           RgnAllocBmpMem_x2d40
    RgnAllocBmpMem_x2ffc:
    /*00002ffc*/ MOV         R1, #3 
    /*00003000*/ MOV         R3, #0 
    /*00003004*/ MOV         R0, R1 
    /*00003008*/ STR         R3, [SP, #4] 
    /*0000300c*/ MOVW        R2, #0                      @ .LC93 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12a0] 
    /*00003010*/ MOVW        R3, #1716                   @ 0x6b4 
    /*00003014*/ MOVT        R2, #0                      @ .LC93 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12a0] 
    /*00003018*/ STR         R3, [SP] 
    /*0000301c*/ LDR         R3, [PC, #56]               @ 0x000000000000305c 
    /*00003020*/ BL          0x0000000000003020          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003024*/ MOVW        R0, #32777                  @ 0x8009 
    /*00003028*/ MOVT        R0, #40963                  @ 0xa003 
    /*0000302c*/ B           RgnAllocBmpMem_x2f3c
    RgnAllocBmpMem_x3030:
    /*00003030*/ MOV         R1, #3 
    /*00003034*/ MOVW        R3, #1741                   @ 0x6cd 
    /*00003038*/ MOV         R0, R1 
    /*0000303c*/ STR         R3, [SP] 
    /*00003040*/ MOVW        R2, #0                      @ .LC95 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12e8] 
    /*00003044*/ LDR         R3, [PC, #16]               @ 0x000000000000305c 
    /*00003048*/ MOVT        R2, #0                      @ .LC95 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12e8] 
    /*0000304c*/ BL          0x000000000000304C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003050*/ MOVW        R0, #32780                  @ 0x800c 
    /*00003054*/ MOVT        R0, #40963                  @ 0xa003 
    /*00003058*/ B           RgnAllocBmpMem_x2f3c
    /*0000305c*/ ANDEQ       R0, R0, R0, ROR #4 
FUNC_END RgnAllocBmpMem


FUNC_BEGIN RgnFreeBmpMem
    /*00003060*/ PUSH        {R4, R5, R6, LR} 
    /*00003064*/ SUBS        R4, R0, #0 
    /*00003068*/ BEQ         RgnFreeBmpMem_x30a0
    RgnFreeBmpMem_x306c:
    /*0000306c*/ LDRD        R0, [R4, #248]              @ 0xf8 
    /*00003070*/ ORRS        R3, R0, R1 
    /*00003074*/ POPEQ       {R4, R5, R6, PC} 
    /*00003078*/ ADD         R5, R4, #256                @ 0x100 
    /*0000307c*/ LDR         R2, [R4, #256]              @ 0x100 
    /*00003080*/ BL          0x0000000000003080          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*00003084*/ MOV         R2, #0 
    /*00003088*/ MOV         R3, #0 
    /*0000308c*/ MOV         R1, #0 
    /*00003090*/ STRD        R2, [R4, #248]              @ 0xf8 
    /*00003094*/ STRD        R2, [R5] 
    /*00003098*/ STR         R1, [R4, #268]              @ 0x10c 
    /*0000309c*/ POP         {R4, R5, R6, PC} 
    RgnFreeBmpMem_x30a0:
    /*000030a0*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c] 
    /*000030a4*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000030a8*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c] 
    /*000030ac*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000030b0*/ MOVW        R2, #1765                   @ 0x6e5 
    /*000030b4*/ LDR         R1, [PC, #4]                @ 0x00000000000030c0 
    /*000030b8*/ BL          0x00000000000030B8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000030bc*/ B           RgnFreeBmpMem_x306c
    /*000030c0*/ ANDEQ       R0, R0, R0, ROR R2 
FUNC_END RgnFreeBmpMem


FUNC_BEGIN RgnReallocBmpMem
    /*000030c4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000030c8*/ SUBS        R7, R0, #0 
    /*000030cc*/ SUB         SP, SP, #60                 @ 0x3c 
    /*000030d0*/ MOV         R3, #0 
    /*000030d4*/ MOV         R4, R1 
    /*000030d8*/ MOV         R0, #0 
    /*000030dc*/ MOV         R1, #0 
    /*000030e0*/ MOV         R6, R2 
    /*000030e4*/ STR         R3, [SP, #40]               @ 0x28 
    /*000030e8*/ STR         R3, [SP, #36]               @ 0x24 
    /*000030ec*/ STR         R3, [SP, #44]               @ 0x2c 
    /*000030f0*/ STR         R3, [SP, #48]               @ 0x30 
    /*000030f4*/ STR         R3, [SP, #52]               @ 0x34 
    /*000030f8*/ STRD        R0, [SP, #16] 
    /*000030fc*/ BEQ         RgnReallocBmpMem_x330c
    RgnReallocBmpMem_x3100:
    /*00003100*/ LDR         R8, [R7, #176]              @ 0xb0 
    /*00003104*/ CMP         R8, #4 
    /*00003108*/ BHI         RgnReallocBmpMem_x32ec
    RgnReallocBmpMem_x310c:
    /*0000310c*/ CMP         R8, #0 
    /*00003110*/ CMPNE       R8, #3 
    /*00003114*/ MOVEQ       R8, #1 
    /*00003118*/ MOVNE       R8, #0 
    /*0000311c*/ BEQ         RgnReallocBmpMem_x3150
    /*00003120*/ LDRD        R2, [SP, #16] 
    /*00003124*/ MOV         SL, R8 
    /*00003128*/ MOV         R9, R8 
    RgnReallocBmpMem_x312c:
    /*0000312c*/ STRD        R2, [R7, #96]               @ 0x60 
    /*00003130*/ MOV         R0, #0 
    /*00003134*/ STR         R8, [R7, #104]              @ 0x68 
    /*00003138*/ STR         SL, [R7, #108]              @ 0x6c 
    /*0000313c*/ STR         R9, [R7, #112]              @ 0x70 
    /*00003140*/ LDR         R3, [R4, #108]              @ 0x6c 
    /*00003144*/ STR         R3, [R7, #116]              @ 0x74 
    RgnReallocBmpMem_x3148:
    /*00003148*/ ADD         SP, SP, #60                 @ 0x3c 
    /*0000314c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnReallocBmpMem_x3150:
    /*00003150*/ LDR         R3, [R4, #112]              @ 0x70 
    /*00003154*/ SUB         R2, R3, #8 
    /*00003158*/ CMP         R2, #1 
    /*0000315c*/ MOVLS       FP, #2 
    /*00003160*/ BLS         RgnReallocBmpMem_x319c
    /*00003164*/ CMP         R3, #11 
    /*00003168*/ MOVEQ       FP, #4 
    /*0000316c*/ BEQ         RgnReallocBmpMem_x319c
    /*00003170*/ CMP         R3, #12 
    /*00003174*/ MOVEQ       FP, #1 
    /*00003178*/ BEQ         RgnReallocBmpMem_x319c
    /*0000317c*/ MOVW        R3, #0                      @ .LC96 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1318] 
    /*00003180*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00003184*/ MOVT        R3, #0                      @ .LC96 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1318] 
    /*00003188*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*0000318c*/ MOV         R2, #1824                   @ 0x720 
    /*00003190*/ LDR         R1, [PC, #496]              @ 0x0000000000003388 
    /*00003194*/ BL          0x0000000000003194          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003198*/ MOV         FP, #0 
    RgnReallocBmpMem_x319c:
    /*0000319c*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*000031a0*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*000031a4*/ LDR         R0, [R4, #16] 
    /*000031a8*/ LDR         R9, [R3, #-1092]            @ 0xfffffbbc 
    /*000031ac*/ SUB         R3, R9, #1 
    /*000031b0*/ MOV         R1, R9 
    /*000031b4*/ MLA         R0, R0, FP, R3 
    /*000031b8*/ BL          0x00000000000031B8          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000031bc*/ MUL         R9, R9, R0 
    /*000031c0*/ CMP         R9, #0 
    /*000031c4*/ BEQ         RgnReallocBmpMem_x332c
    /*000031c8*/ LDR         R3, [R7, #120]              @ 0x78 
    /*000031cc*/ MOV         R1, #20 
    /*000031d0*/ LDR         R5, [R4, #20] 
    /*000031d4*/ MOVW        R2, #0                      @ .LC94 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12d8] 
    /*000031d8*/ CMP         R3, #12 
    /*000031dc*/ MOVT        R2, #0                      @ .LC94 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12d8] 
    /*000031e0*/ LSREQ       R9, R9, #2 
    /*000031e4*/ MOV         R3, R6 
    /*000031e8*/ ADD         R0, SP, #36                 @ 0x24 
    /*000031ec*/ MOV         SL, #0 
    /*000031f0*/ MUL         R5, R5, R9 
    /*000031f4*/ BL          0x00000000000031F4          @ osal_snprintf [R_ARM_CALL: +0x0] 
    /*000031f8*/ LDR         R3, [R4, #108]              @ 0x6c 
    /*000031fc*/ MOV         R0, #2 
    /*00003200*/ STR         SL, [SP, #12] 
    /*00003204*/ MUL         R6, R3, R5 
    /*00003208*/ BL          0x0000000000003208          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000320c*/ CMP         R0, SL 
    /*00003210*/ BEQ         RgnReallocBmpMem_x335c
    /*00003214*/ MOV         R0, #2 
    /*00003218*/ BL          0x0000000000003218          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000321c*/ LDR         R3, [R0, #60]               @ 0x3c 
    /*00003220*/ CMP         R3, SL 
    /*00003224*/ BEQ         RgnReallocBmpMem_x335c
    /*00003228*/ MOV         R0, #2 
    /*0000322c*/ MOV         R3, #3 
    /*00003230*/ STR         SL, [SP, #32] 
    /*00003234*/ STR         R3, [SP, #24] 
    /*00003238*/ STR         SL, [SP, #28] 
    /*0000323c*/ BL          0x000000000000323C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00003240*/ ADD         R1, SP, #8 
    /*00003244*/ LDR         R3, [R0, #60]               @ 0x3c 
    /*00003248*/ ADD         R0, SP, #24 
    /*0000324c*/ BLX         R3 
    /*00003250*/ LDR         R0, [SP, #8] 
    /*00003254*/ STR         R6, [SP] 
    /*00003258*/ ADD         R1, SP, #36                 @ 0x24 
    /*0000325c*/ ADD         R3, SP, #12 
    /*00003260*/ ADD         R2, SP, #16 
    /*00003264*/ BL          0x0000000000003264          @ CMPI_MmzMallocNocache [R_ARM_CALL: +0x0] 
    /*00003268*/ LDR         R8, [SP, #12] 
    /*0000326c*/ CMP         R0, SL 
    /*00003270*/ BNE         RgnReallocBmpMem_x335c
    /*00003274*/ MOV         R0, R7 
    /*00003278*/ BL          0x0000000000003278          @ RgnFreeBmpMem [R_ARM_CALL: .text+0x3060] 
    /*0000327c*/ LDRD        R2, [SP, #16] 
    /*00003280*/ MOV         R1, #1 
    /*00003284*/ STR         R9, [R7, #268]              @ 0x10c 
    /*00003288*/ STR         R5, [R7, #240]              @ 0xf0 
    /*0000328c*/ STR         FP, [R7, #264]              @ 0x108 
    /*00003290*/ STR         R8, [R7, #256]              @ 0x100 
    /*00003294*/ STR         SL, [R7, #260]              @ 0x104 
    /*00003298*/ STR         SL, [R7, #216]              @ 0xd8 
    /*0000329c*/ STR         SL, [R7, #212]              @ 0xd4 
    /*000032a0*/ STRD        R2, [R7, #248]              @ 0xf8 
    /*000032a4*/ STR         R1, [R7, #232]              @ 0xe8 
    /*000032a8*/ LDR         R1, [R4, #20] 
    /*000032ac*/ CMP         R1, #0 
    /*000032b0*/ BEQ         RgnReallocBmpMem_x312c
    /*000032b4*/ MOV         R6, R8 
    /*000032b8*/ MOV         R5, SL 
    RgnReallocBmpMem_x32bc:
    /*000032bc*/ LDR         R3, [R4, #16] 
    /*000032c0*/ MOV         R0, R6 
    /*000032c4*/ MOV         R2, FP 
    /*000032c8*/ LDR         R1, [R4, #68]               @ 0x44 
    /*000032cc*/ BL          0x00000000000032CC          @ RgnMemSet [R_ARM_CALL: .text+0xbd0] 
    /*000032d0*/ LDR         R3, [R4, #20] 
    /*000032d4*/ ADD         R5, R5, #1 
    /*000032d8*/ ADD         R6, R6, R9 
    /*000032dc*/ CMP         R3, R5 
    /*000032e0*/ BHI         RgnReallocBmpMem_x32bc
    /*000032e4*/ LDRD        R2, [SP, #16] 
    /*000032e8*/ B           RgnReallocBmpMem_x312c
    RgnReallocBmpMem_x32ec:
    /*000032ec*/ MOVW        R3, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1278] 
    /*000032f0*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000032f4*/ MOVT        R3, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1278] 
    /*000032f8*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000032fc*/ MOVW        R2, #1819                   @ 0x71b 
    /*00003300*/ LDR         R1, [PC, #128]              @ 0x0000000000003388 
    /*00003304*/ BL          0x0000000000003304          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003308*/ B           RgnReallocBmpMem_x310c
    RgnReallocBmpMem_x330c:
    /*0000330c*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c] 
    /*00003310*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00003314*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c] 
    /*00003318*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*0000331c*/ MOVW        R2, #1817                   @ 0x719 
    /*00003320*/ LDR         R1, [PC, #96]               @ 0x0000000000003388 
    /*00003324*/ BL          0x0000000000003324          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003328*/ B           RgnReallocBmpMem_x3100
    RgnReallocBmpMem_x332c:
    /*0000332c*/ MOV         R1, #3 
    /*00003330*/ MOVW        R3, #1829                   @ 0x725 
    /*00003334*/ STR         R9, [SP, #4] 
    /*00003338*/ MOV         R0, R1 
    /*0000333c*/ STR         R3, [SP] 
    /*00003340*/ MOVW        R2, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x132c] 
    /*00003344*/ LDR         R3, [PC, #60]               @ 0x0000000000003388 
    /*00003348*/ MOVT        R2, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0x132c] 
    /*0000334c*/ BL          0x000000000000334C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003350*/ MOVW        R0, #32777                  @ 0x8009 
    /*00003354*/ MOVT        R0, #40963                  @ 0xa003 
    /*00003358*/ B           RgnReallocBmpMem_x3148
    RgnReallocBmpMem_x335c:
    /*0000335c*/ MOV         R1, #3 
    /*00003360*/ MOVW        R3, #1843                   @ 0x733 
    /*00003364*/ MOV         R0, R1 
    /*00003368*/ STR         R3, [SP] 
    /*0000336c*/ MOVW        R2, #0                      @ .LC95 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12e8] 
    /*00003370*/ LDR         R3, [PC, #16]               @ 0x0000000000003388 
    /*00003374*/ MOVT        R2, #0                      @ .LC95 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12e8] 
    /*00003378*/ BL          0x0000000000003378          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000337c*/ MOVW        R0, #32780                  @ 0x800c 
    /*00003380*/ MOVT        R0, #40963                  @ 0xa003 
    /*00003384*/ B           RgnReallocBmpMem_x3148
    /*00003388*/ ANDEQ       R0, R0, R0, LSL #5 
FUNC_END RgnReallocBmpMem


FUNC_BEGIN RgnFreeChnMng
    /*0000338c*/ PUSH        {R4, LR} 
    /*00003390*/ SUBS        R4, R0, #0 
    /*00003394*/ BEQ         RgnFreeChnMng_x33b4
    RgnFreeChnMng_x3398:
    /*00003398*/ LDR         R0, [R4] 
    /*0000339c*/ CMP         R0, #0 
    /*000033a0*/ POPEQ       {R4, PC} 
    /*000033a4*/ BL          0x00000000000033A4          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*000033a8*/ MOV         R3, #0 
    /*000033ac*/ STR         R3, [R4] 
    /*000033b0*/ POP         {R4, PC} 
    RgnFreeChnMng_x33b4:
    /*000033b4*/ MOVW        R3, #0                      @ .LC98 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1364] 
    /*000033b8*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000033bc*/ MOVT        R3, #0                      @ .LC98 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1364] 
    /*000033c0*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000033c4*/ MOVW        R2, #1899                   @ 0x76b 
    /*000033c8*/ LDR         R1, [PC, #4]                @ 0x00000000000033d4 
    /*000033cc*/ BL          0x00000000000033CC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000033d0*/ B           RgnFreeChnMng_x3398
    /*000033d4*/ MULEQ       R0, R4, R2 
FUNC_END RgnFreeChnMng


FUNC_BEGIN RgnFreeGetInfo
    /*000033d8*/ PUSH        {R4, LR} 
    /*000033dc*/ SUBS        R4, R0, #0 
    /*000033e0*/ BEQ         RgnFreeGetInfo_x3400
    RgnFreeGetInfo_x33e4:
    /*000033e4*/ LDR         R0, [R4] 
    /*000033e8*/ CMP         R0, #0 
    /*000033ec*/ POPEQ       {R4, PC} 
    /*000033f0*/ BL          0x00000000000033F0          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*000033f4*/ MOV         R3, #0 
    /*000033f8*/ STR         R3, [R4] 
    /*000033fc*/ POP         {R4, PC} 
    RgnFreeGetInfo_x3400:
    /*00003400*/ MOVW        R3, #0                      @ .LC99 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x137c] 
    /*00003404*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00003408*/ MOVT        R3, #0                      @ .LC99 [R_ARM_MOVT_ABS: .rodata.str1.4+0x137c] 
    /*0000340c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00003410*/ MOVW        R2, #1910                   @ 0x776 
    /*00003414*/ LDR         R1, [PC, #4]                @ 0x0000000000003420 
    /*00003418*/ BL          0x0000000000003418          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000341c*/ B           RgnFreeGetInfo_x33e4
    /*00003420*/ ANDEQ       R0, R0, R4, LSR #5 
FUNC_END RgnFreeGetInfo


FUNC_BEGIN RgnMallocModInfo
    /*00003424*/ PUSH        {R4, R5, LR} 
    /*00003428*/ SUBS        R4, R0, #0 
    /*0000342c*/ SUB         SP, SP, #12 
    /*00003430*/ BEQ         RgnMallocModInfo_x3560
    RgnMallocModInfo_x3434:
    /*00003434*/ LDR         R3, [R4, #12] 
    /*00003438*/ CMP         R3, #0 
    /*0000343c*/ BEQ         RgnMallocModInfo_x345c
    /*00003440*/ MOVW        R3, #0                      @ .LC101 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13ac] 
    /*00003444*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00003448*/ MOVT        R3, #0                      @ .LC101 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13ac] 
    /*0000344c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00003450*/ MOVW        R2, #1924                   @ 0x784 
    /*00003454*/ LDR         R1, [PC, #340]              @ 0x00000000000035b0 
    /*00003458*/ BL          0x0000000000003458          @ osal_panic [R_ARM_CALL: +0x0] 
    RgnMallocModInfo_x345c:
    /*0000345c*/ LDR         R3, [R4] 
    /*00003460*/ LDR         R2, [R4, #4] 
    /*00003464*/ MUL         R2, R2, R3 
    /*00003468*/ LSLS        R5, R2, #2 
    /*0000346c*/ BEQ         RgnMallocModInfo_x3580
    /*00003470*/ MOV         R1, #0 
    /*00003474*/ MOV         R0, R5 
    /*00003478*/ BL          0x0000000000003478          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*0000347c*/ CMP         R0, #0 
    /*00003480*/ STR         R0, [R4, #12] 
    /*00003484*/ BEQ         RgnMallocModInfo_x3530
    /*00003488*/ MOV         R2, R5 
    /*0000348c*/ MOV         R1, #0 
    /*00003490*/ BL          0x0000000000003490          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00003494*/ LDM         R4, {R2, R5} 
    /*00003498*/ MOV         R1, #0 
    /*0000349c*/ MUL         R5, R5, R2 
    /*000034a0*/ LSL         R5, R5, #2 
    /*000034a4*/ MOV         R0, R5 
    /*000034a8*/ BL          0x00000000000034A8          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*000034ac*/ CMP         R0, #0 
    /*000034b0*/ STR         R0, [R4, #16] 
    /*000034b4*/ BEQ         RgnMallocModInfo_x34d0
    /*000034b8*/ MOV         R2, R5 
    /*000034bc*/ MOV         R1, #0 
    /*000034c0*/ BL          0x00000000000034C0          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000034c4*/ MOV         R0, #0 
    RgnMallocModInfo_x34c8:
    /*000034c8*/ ADD         SP, SP, #12 
    /*000034cc*/ POP         {R4, R5, PC} 
    RgnMallocModInfo_x34d0:
    /*000034d0*/ MOV         R1, #3 
    /*000034d4*/ MOVW        R3, #1946                   @ 0x79a 
    /*000034d8*/ MOVW        R2, #0                      @ .LC104 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1438] 
    /*000034dc*/ STR         R3, [SP] 
    /*000034e0*/ MOV         R0, R1 
    /*000034e4*/ LDR         R3, [PC, #196]              @ 0x00000000000035b0 
    /*000034e8*/ MOVT        R2, #0                      @ .LC104 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1438] 
    /*000034ec*/ BL          0x00000000000034EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    RgnMallocModInfo_x34f0:
    /*000034f0*/ LDR         R0, [R4, #12] 
    /*000034f4*/ CMP         R0, #0 
    /*000034f8*/ BEQ         RgnMallocModInfo_x3508
    /*000034fc*/ BL          0x00000000000034FC          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00003500*/ MOV         R3, #0 
    /*00003504*/ STR         R3, [R4, #12] 
    RgnMallocModInfo_x3508:
    /*00003508*/ LDR         R0, [R4, #16] 
    /*0000350c*/ CMP         R0, #0 
    /*00003510*/ BEQ         RgnMallocModInfo_x3554
    /*00003514*/ BL          0x0000000000003514          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00003518*/ MOV         R3, #0 
    /*0000351c*/ MOVW        R0, #32780                  @ 0x800c 
    /*00003520*/ STR         R3, [R4, #16] 
    /*00003524*/ MOVT        R0, #40963                  @ 0xa003 
    /*00003528*/ ADD         SP, SP, #12 
    /*0000352c*/ POP         {R4, R5, PC} 
    RgnMallocModInfo_x3530:
    /*00003530*/ MOV         R1, #3 
    /*00003534*/ MOV         R3, #1936                   @ 0x790 
    /*00003538*/ MOV         R0, R1 
    /*0000353c*/ STR         R3, [SP] 
    /*00003540*/ MOVW        R2, #0                      @ .LC103 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x140c] 
    /*00003544*/ LDR         R3, [PC, #100]              @ 0x00000000000035b0 
    /*00003548*/ MOVT        R2, #0                      @ .LC103 [R_ARM_MOVT_ABS: .rodata.str1.4+0x140c] 
    /*0000354c*/ BL          0x000000000000354C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003550*/ B           RgnMallocModInfo_x34f0
    RgnMallocModInfo_x3554:
    /*00003554*/ MOVW        R0, #32780                  @ 0x800c 
    /*00003558*/ MOVT        R0, #40963                  @ 0xa003 
    /*0000355c*/ B           RgnMallocModInfo_x34c8
    RgnMallocModInfo_x3560:
    /*00003560*/ MOVW        R3, #0                      @ .LC100 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1394] 
    /*00003564*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00003568*/ MOVT        R3, #0                      @ .LC100 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1394] 
    /*0000356c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00003570*/ MOVW        R2, #1923                   @ 0x783 
    /*00003574*/ LDR         R1, [PC, #52]               @ 0x00000000000035b0 
    /*00003578*/ BL          0x0000000000003578          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000357c*/ B           RgnMallocModInfo_x3434
    RgnMallocModInfo_x3580:
    /*00003580*/ MOV         R1, #3 
    /*00003584*/ MOVW        R3, #1930                   @ 0x78a 
    /*00003588*/ STR         R5, [SP, #4] 
    /*0000358c*/ MOV         R0, R1 
    /*00003590*/ STR         R3, [SP] 
    /*00003594*/ MOVW        R2, #0                      @ .LC102 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13d0] 
    /*00003598*/ LDR         R3, [PC, #16]               @ 0x00000000000035b0 
    /*0000359c*/ MOVT        R2, #0                      @ .LC102 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13d0] 
    /*000035a0*/ BL          0x00000000000035A0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000035a4*/ MOVW        R0, #32780                  @ 0x800c 
    /*000035a8*/ MOVT        R0, #40963                  @ 0xa003 
    /*000035ac*/ B           RgnMallocModInfo_x34c8
    /*000035b0*/ @           <UNDEFINED> instruction: 0x000002b4 
FUNC_END RgnMallocModInfo


FUNC_BEGIN RgnFreeModInfo
    /*000035b4*/ PUSH        {R4, R5, R6, LR} 
    /*000035b8*/ SUBS        R5, R0, #0 
    /*000035bc*/ BEQ         RgnFreeModInfo_x3644
    RgnFreeModInfo_x35c0:
    /*000035c0*/ LDR         R0, [R5, #12] 
    /*000035c4*/ LDM         R5, {R3, R6} 
    /*000035c8*/ CMP         R0, #0 
    /*000035cc*/ MUL         R6, R6, R3 
    /*000035d0*/ BEQ         RgnFreeModInfo_x3604
    /*000035d4*/ CMP         R6, #0 
    /*000035d8*/ BEQ         RgnFreeModInfo_x35f8
    /*000035dc*/ MOV         R4, #0 
    RgnFreeModInfo_x35e0:
    /*000035e0*/ ADD         R0, R0, R4, LSL #2 
    /*000035e4*/ ADD         R4, R4, #1 
    /*000035e8*/ BL          0x00000000000035E8          @ RgnFreeChnMng [R_ARM_CALL: .text+0x338c] 
    /*000035ec*/ CMP         R4, R6 
    /*000035f0*/ LDR         R0, [R5, #12] 
    /*000035f4*/ BNE         RgnFreeModInfo_x35e0
    RgnFreeModInfo_x35f8:
    /*000035f8*/ BL          0x00000000000035F8          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*000035fc*/ MOV         R3, #0 
    /*00003600*/ STR         R3, [R5, #12] 
    RgnFreeModInfo_x3604:
    /*00003604*/ LDR         R0, [R5, #16] 
    /*00003608*/ CMP         R0, #0 
    /*0000360c*/ POPEQ       {R4, R5, R6, PC} 
    /*00003610*/ CMP         R6, #0 
    /*00003614*/ BEQ         RgnFreeModInfo_x3634
    /*00003618*/ MOV         R4, #0 
    RgnFreeModInfo_x361c:
    /*0000361c*/ ADD         R0, R0, R4, LSL #2 
    /*00003620*/ ADD         R4, R4, #1 
    /*00003624*/ BL          0x0000000000003624          @ RgnFreeGetInfo [R_ARM_CALL: .text+0x33d8] 
    /*00003628*/ CMP         R4, R6 
    /*0000362c*/ LDR         R0, [R5, #16] 
    /*00003630*/ BNE         RgnFreeModInfo_x361c
    RgnFreeModInfo_x3634:
    /*00003634*/ BL          0x0000000000003634          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00003638*/ MOV         R3, #0 
    /*0000363c*/ STR         R3, [R5, #16] 
    /*00003640*/ POP         {R4, R5, R6, PC} 
    RgnFreeModInfo_x3644:
    /*00003644*/ MOVW        R3, #0                      @ .LC100 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1394] 
    /*00003648*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*0000364c*/ MOVT        R3, #0                      @ .LC100 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1394] 
    /*00003650*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00003654*/ MOVW        R2, #1970                   @ 0x7b2 
    /*00003658*/ LDR         R1, [PC, #4]                @ 0x0000000000003664 
    /*0000365c*/ BL          0x000000000000365C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003660*/ B           RgnFreeModInfo_x35c0
    /*00003664*/ ANDEQ       R0, R0, R8, ASR #5 
FUNC_END RgnFreeModInfo


FUNC_BEGIN RgnGetCapacity
    /*00003668*/ CMP         R0, #4 
    /*0000366c*/ PUSH        {R4, R5, R6, LR} 
    /*00003670*/ MOV         R5, R0 
    /*00003674*/ MOV         R4, R1 
    /*00003678*/ BHI         RgnGetCapacity_x36e4
    RgnGetCapacity_x367c:
    /*0000367c*/ MOV         R0, #12 
    /*00003680*/ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003684*/ MUL         R0, R0, R5 
    /*00003688*/ MOVT        R1, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000368c*/ ADD         R3, R1, R0 
    /*00003690*/ ADD         R1, R3, #524                @ 0x20c 
    /*00003694*/ LDR         R3, [R3, #524]              @ 0x20c 
    /*00003698*/ CMP         R3, R1 
    /*0000369c*/ LDR         R0, [R3] 
    /*000036a0*/ BEQ         RgnGetCapacity_x36d0
    /*000036a4*/ LDR         R2, [R3, #-332]             @ 0xfffffeb4 
    /*000036a8*/ CMP         R4, R2 
    /*000036ac*/ BNE         RgnGetCapacity_x36c4
    /*000036b0*/ B           RgnGetCapacity_x36d8
    RgnGetCapacity_x36b4:
    /*000036b4*/ LDR         R2, [R0, #-332]             @ 0xfffffeb4 
    /*000036b8*/ CMP         R2, R4 
    /*000036bc*/ BEQ         RgnGetCapacity_x36dc
    /*000036c0*/ MOV         R0, R3 
    RgnGetCapacity_x36c4:
    /*000036c4*/ CMP         R1, R0 
    /*000036c8*/ LDR         R3, [R0] 
    /*000036cc*/ BNE         RgnGetCapacity_x36b4
    RgnGetCapacity_x36d0:
    /*000036d0*/ MOV         R0, #0 
    /*000036d4*/ POP         {R4, R5, R6, PC} 
    RgnGetCapacity_x36d8:
    /*000036d8*/ MOV         R0, R3 
    RgnGetCapacity_x36dc:
    /*000036dc*/ SUB         R0, R0, #328                @ 0x148 
    /*000036e0*/ POP         {R4, R5, R6, PC} 
    RgnGetCapacity_x36e4:
    /*000036e4*/ MOVW        R3, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fc] 
    /*000036e8*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000036ec*/ MOVT        R3, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fc] 
    /*000036f0*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000036f4*/ MOVW        R2, #2006                   @ 0x7d6 
    /*000036f8*/ LDR         R1, [PC, #4]                @ 0x0000000000003704 
    /*000036fc*/ BL          0x00000000000036FC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003700*/ B           RgnGetCapacity_x367c
    /*00003704*/ LDRDEQ      R0, [R0], -R8 
FUNC_END RgnGetCapacity


FUNC_BEGIN RgnAddCapacityInfo
    /*00003708*/ PUSH        {R4, R5, R6, R7, LR} 
    /*0000370c*/ MOV         R6, R0 
    /*00003710*/ SUB         SP, SP, #12 
    /*00003714*/ MOV         R5, R1 
    /*00003718*/ MOV         R0, #340                    @ 0x154 
    /*0000371c*/ MOV         R1, #0 
    /*00003720*/ MOV         R7, R2 
    /*00003724*/ BL          0x0000000000003724          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*00003728*/ SUBS        R4, R0, #0 
    /*0000372c*/ BEQ         RgnAddCapacityInfo_x37bc
    /*00003730*/ MOV         R0, R4 
    /*00003734*/ MOV         R1, R7 
    /*00003738*/ STR         R6, [R0], #4 
    /*0000373c*/ MOV         R2, #328                    @ 0x148 
    /*00003740*/ BL          0x0000000000003740          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00003744*/ LDR         R3, [R4, #308]              @ 0x134 
    /*00003748*/ CMP         R3, #0 
    /*0000374c*/ BNE         RgnAddCapacityInfo_x3798
    RgnAddCapacityInfo_x3750:
    /*00003750*/ MOV         R1, #12 
    /*00003754*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003758*/ MUL         R1, R1, R5 
    /*0000375c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003760*/ ADD         IP, R4, #332                @ 0x14c 
    /*00003764*/ MOV         R0, #0 
    /*00003768*/ ADD         R2, R3, R1 
    /*0000376c*/ ADD         R3, R2, #524                @ 0x20c 
    /*00003770*/ LDR         LR, [R2, #524]              @ 0x20c 
    /*00003774*/ STR         IP, [LR, #4] 
    /*00003778*/ LDR         R1, [R2, #520]              @ 0x208 
    /*0000377c*/ STR         R3, [R4, #336]              @ 0x150 
    /*00003780*/ STR         LR, [R4, #332]              @ 0x14c 
    /*00003784*/ ADD         R3, R1, #1 
    /*00003788*/ STR         IP, [R2, #524]              @ 0x20c 
    /*0000378c*/ STR         R3, [R2, #520]              @ 0x208 
    RgnAddCapacityInfo_x3790:
    /*00003790*/ ADD         SP, SP, #12 
    /*00003794*/ POP         {R4, R5, R6, R7, PC} 
    RgnAddCapacityInfo_x3798:
    /*00003798*/ MOVW        R2, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*0000379c*/ MOVT        R2, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*000037a0*/ LDR         R3, [R4, #316]              @ 0x13c 
    /*000037a4*/ LDR         R1, [R2, #-1092]            @ 0xfffffbbc 
    /*000037a8*/ CMP         R3, R1 
    /*000037ac*/ MOVCC       R3, R1 
    /*000037b0*/ STR         R3, [R2, #-1092]            @ 0xfffffbbc 
    /*000037b4*/ STR         R3, [R4, #316]              @ 0x13c 
    /*000037b8*/ B           RgnAddCapacityInfo_x3750
    RgnAddCapacityInfo_x37bc:
    /*000037bc*/ MOV         R1, #3 
    /*000037c0*/ MOVW        R3, #2026                   @ 0x7ea 
    /*000037c4*/ MOV         R0, R1 
    /*000037c8*/ STR         R3, [SP] 
    /*000037cc*/ MOVW        R2, #0                      @ .LC105 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1464] 
    /*000037d0*/ LDR         R3, [PC, #16]               @ 0x00000000000037e8 
    /*000037d4*/ MOVT        R2, #0                      @ .LC105 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1464] 
    /*000037d8*/ BL          0x00000000000037D8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000037dc*/ MOVW        R0, #32780                  @ 0x800c 
    /*000037e0*/ MOVT        R0, #40963                  @ 0xa003 
    /*000037e4*/ B           RgnAddCapacityInfo_x3790
    /*000037e8*/ ANDEQ       R0, R0, R8, ROR #5 
FUNC_END RgnAddCapacityInfo


FUNC_BEGIN RgnDelCapacityInfo
    /*000037ec*/ PUSH        {R4, R5, LR} 
    /*000037f0*/ MOV         R5, R0 
    /*000037f4*/ SUB         SP, SP, #20 
    /*000037f8*/ MOV         R0, R1 
    /*000037fc*/ MOV         R4, R1 
    /*00003800*/ MOV         R1, R5 
    /*00003804*/ BL          0x0000000000003804          @ RgnGetCapacity [R_ARM_CALL: .text+0x3668] 
    /*00003808*/ SUBS        R3, R0, #0 
    /*0000380c*/ BEQ         RgnDelCapacityInfo_x3890
    /*00003810*/ SUBS        R0, R3, #4 
    /*00003814*/ BEQ         RgnDelCapacityInfo_x3868
    /*00003818*/ LDR         IP, [R3, #332]              @ 0x14c 
    /*0000381c*/ MOV         R1, #256                    @ 0x100 
    /*00003820*/ LDR         LR, [R3, #328]              @ 0x148 
    /*00003824*/ MOVT        R1, #16 
    /*00003828*/ MOV         R2, #512                    @ 0x200 
    /*0000382c*/ MOVT        R2, #32 
    /*00003830*/ STR         IP, [LR, #4] 
    /*00003834*/ STR         LR, [IP] 
    /*00003838*/ STR         R2, [R3, #332]              @ 0x14c 
    /*0000383c*/ STR         R1, [R3, #328]              @ 0x148 
    /*00003840*/ BL          0x0000000000003840          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00003844*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003848*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000384c*/ MOV         R2, #12 
    /*00003850*/ MLA         R4, R2, R4, R3 
    /*00003854*/ LDR         R3, [R4, #520]              @ 0x208 
    /*00003858*/ SUB         R3, R3, #1 
    /*0000385c*/ STR         R3, [R4, #520]              @ 0x208 
    RgnDelCapacityInfo_x3860:
    /*00003860*/ ADD         SP, SP, #20 
    /*00003864*/ POP         {R4, R5, PC} 
    RgnDelCapacityInfo_x3868:
    /*00003868*/ MOV         R1, #3 
    /*0000386c*/ MOVW        R3, #2065                   @ 0x811 
    /*00003870*/ STMIB       SP, {R4, R5} 
    /*00003874*/ MOV         R0, R1 
    /*00003878*/ STR         R3, [SP] 
    /*0000387c*/ MOVW        R2, #0                      @ .LC107 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14e0] 
    /*00003880*/ LDR         R3, [PC, #48]               @ 0x00000000000038b8 
    /*00003884*/ MOVT        R2, #0                      @ .LC107 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14e0] 
    /*00003888*/ BL          0x0000000000003888          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000388c*/ B           RgnDelCapacityInfo_x3860
    RgnDelCapacityInfo_x3890:
    /*00003890*/ MOV         R1, #3 
    /*00003894*/ MOVW        R3, #2057                   @ 0x809 
    /*00003898*/ STMIB       SP, {R4, R5} 
    /*0000389c*/ MOV         R0, R1 
    /*000038a0*/ STR         R3, [SP] 
    /*000038a4*/ MOVW        R2, #0                      @ .LC106 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14a0] 
    /*000038a8*/ LDR         R3, [PC, #8]                @ 0x00000000000038b8 
    /*000038ac*/ MOVT        R2, #0                      @ .LC106 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14a0] 
    /*000038b0*/ BL          0x00000000000038B0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000038b4*/ B           RgnDelCapacityInfo_x3860
    /*000038b8*/ STRDEQ      R0, [R0], -IP 
FUNC_END RgnDelCapacityInfo


FUNC_BEGIN RgnIsPointInRect
    /*000038bc*/ PUSH        {R4, LR} 
    /*000038c0*/ LDR         R3, [R2, #8] 
    /*000038c4*/ LDR         IP, [R2] 
    /*000038c8*/ SUB         R3, R3, #1 
    /*000038cc*/ LDR         LR, [R2, #4] 
    /*000038d0*/ ADD         R3, R3, IP 
    /*000038d4*/ LDR         R2, [R2, #12] 
    /*000038d8*/ CMP         R3, R0 
    /*000038dc*/ MOVLT       R4, #0 
    /*000038e0*/ MOVGE       R4, #1 
    /*000038e4*/ CMP         IP, R0 
    /*000038e8*/ MOVGT       R4, #0 
    /*000038ec*/ CMP         R4, #0 
    /*000038f0*/ BEQ         RgnIsPointInRect_x3914
    /*000038f4*/ SUB         R0, R2, #1 
    /*000038f8*/ ADD         R0, R0, LR 
    /*000038fc*/ CMP         R0, R1 
    /*00003900*/ MOVLT       R0, #0 
    /*00003904*/ MOVGE       R0, #1 
    /*00003908*/ CMP         LR, R1 
    /*0000390c*/ MOVGT       R0, #0 
    /*00003910*/ POP         {R4, PC} 
    RgnIsPointInRect_x3914:
    /*00003914*/ MOV         R0, R4 
    /*00003918*/ POP         {R4, PC} 
FUNC_END RgnIsPointInRect


FUNC_BEGIN RgnIsRgnCascade
    /*0000391c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00003920*/ SUB         SP, SP, #12 
    /*00003924*/ LDR         LR, [R1] 
    /*00003928*/ LDR         R2, [R1, #8] 
    /*0000392c*/ LDR         IP, [R0] 
    /*00003930*/ ADD         R2, LR, R2 
    /*00003934*/ LDR         R4, [R1, #4] 
    /*00003938*/ SUB         R5, R2, #1 
    /*0000393c*/ LDR         R1, [R1, #12] 
    /*00003940*/ CMP         IP, R5 
    /*00003944*/ LDR         R3, [R0, #4] 
    /*00003948*/ ADD         R1, R4, R1 
    /*0000394c*/ LDR         SL, [R0, #12] 
    /*00003950*/ MOVGT       R8, #0 
    /*00003954*/ MOVLE       R8, #1 
    /*00003958*/ CMP         IP, LR 
    /*0000395c*/ MOVLT       R6, #0 
    /*00003960*/ ANDGE       R6, R8, #1 
    /*00003964*/ SUB         R9, R1, #1 
    /*00003968*/ CMP         R6, #0 
    /*0000396c*/ LDR         R6, [R0, #8] 
    /*00003970*/ BEQ         RgnIsRgnCascade_x399c
    /*00003974*/ CMP         R3, R4 
    /*00003978*/ MOVLT       R0, #0 
    /*0000397c*/ MOVGE       R0, #1 
    /*00003980*/ CMP         R3, R9 
    /*00003984*/ MOVGT       R0, #0 
    /*00003988*/ CMP         R0, #0 
    /*0000398c*/ BEQ         RgnIsRgnCascade_x3a64
    RgnIsRgnCascade_x3990:
    /*00003990*/ MOV         R0, #1 
    RgnIsRgnCascade_x3994:
    /*00003994*/ ADD         SP, SP, #12 
    /*00003998*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnIsRgnCascade_x399c:
    /*0000399c*/ ADD         R0, IP, R6 
    /*000039a0*/ SUB         R6, R0, #1 
    /*000039a4*/ CMP         LR, R6 
    /*000039a8*/ MOVGT       R7, #0 
    /*000039ac*/ MOVLE       R7, #1 
    /*000039b0*/ CMP         R6, R5 
    /*000039b4*/ MOVGT       FP, #0 
    /*000039b8*/ ANDLE       FP, R7, #1 
    /*000039bc*/ CMP         FP, #0 
    /*000039c0*/ BNE         RgnIsRgnCascade_x3ac0
    /*000039c4*/ ADD         SL, R3, SL 
    /*000039c8*/ STR         SL, [SP, #4] 
    /*000039cc*/ SUB         SL, SL, #1 
    RgnIsRgnCascade_x39d0:
    /*000039d0*/ CMP         IP, LR 
    /*000039d4*/ MOVGT       R7, #0 
    /*000039d8*/ ANDLE       R7, R7, #1 
    /*000039dc*/ CMP         R7, #0 
    /*000039e0*/ BEQ         RgnIsRgnCascade_x3b1c
    RgnIsRgnCascade_x39e4:
    /*000039e4*/ CMP         R3, R4 
    /*000039e8*/ CMPLE       R4, SL 
    /*000039ec*/ BLE         RgnIsRgnCascade_x3990
    /*000039f0*/ CMP         R3, R9 
    /*000039f4*/ CMPLE       R9, SL 
    /*000039f8*/ BLE         RgnIsRgnCascade_x3990
    RgnIsRgnCascade_x39fc:
    /*000039fc*/ CMP         R3, R4 
    /*00003a00*/ LDR         R6, [SP, #4] 
    /*00003a04*/ MOVGE       R5, #0 
    /*00003a08*/ MOVLT       R5, #1 
    /*00003a0c*/ CMP         IP, LR 
    /*00003a10*/ MOVLE       R5, #0 
    /*00003a14*/ CMP         R2, R0 
    /*00003a18*/ MOVLS       R5, #0 
    /*00003a1c*/ ANDHI       R5, R5, #1 
    /*00003a20*/ CMP         R1, R6 
    /*00003a24*/ MOVCS       R5, #0 
    /*00003a28*/ ANDCC       R5, R5, #1 
    /*00003a2c*/ CMP         R5, #0 
    /*00003a30*/ BNE         RgnIsRgnCascade_x3990
    /*00003a34*/ CMP         R3, R4 
    /*00003a38*/ MOVLE       R3, #0 
    /*00003a3c*/ MOVGT       R3, #1 
    /*00003a40*/ CMP         IP, LR 
    /*00003a44*/ MOVGE       R3, #0 
    /*00003a48*/ CMP         R2, R0 
    /*00003a4c*/ MOVCS       R0, #0 
    /*00003a50*/ ANDCC       R0, R3, #1 
    /*00003a54*/ CMP         R1, R6 
    /*00003a58*/ MOVLS       R0, #0 
    /*00003a5c*/ ANDHI       R0, R0, #1 
    /*00003a60*/ B           RgnIsRgnCascade_x3994
    RgnIsRgnCascade_x3a64:
    /*00003a64*/ ADD         R0, IP, R6 
    /*00003a68*/ SUB         R6, R0, #1 
    /*00003a6c*/ CMP         LR, R6 
    /*00003a70*/ MOVGT       FP, #0 
    /*00003a74*/ MOVLE       FP, #1 
    /*00003a78*/ CMP         R5, R6 
    /*00003a7c*/ MOVLT       R7, #0 
    /*00003a80*/ ANDGE       R7, FP, #1 
    /*00003a84*/ CMP         R7, #0 
    /*00003a88*/ BNE         RgnIsRgnCascade_x3b0c
    /*00003a8c*/ ADD         R7, R3, SL 
    /*00003a90*/ STR         R7, [SP, #4] 
    /*00003a94*/ MOV         SL, R7 
    /*00003a98*/ SUB         SL, SL, #1 
    /*00003a9c*/ MOV         R7, FP 
    RgnIsRgnCascade_x3aa0:
    /*00003aa0*/ CMP         R4, SL 
    /*00003aa4*/ MOVGT       FP, #0 
    /*00003aa8*/ MOVLE       FP, #1 
    /*00003aac*/ CMP         R9, SL 
    /*00003ab0*/ MOVLT       FP, #0 
    /*00003ab4*/ CMP         FP, #0 
    /*00003ab8*/ BNE         RgnIsRgnCascade_x3990
    /*00003abc*/ B           RgnIsRgnCascade_x39d0
    RgnIsRgnCascade_x3ac0:
    /*00003ac0*/ CMP         R3, R4 
    /*00003ac4*/ MOVLT       R7, #0 
    /*00003ac8*/ MOVGE       R7, #1 
    /*00003acc*/ CMP         R3, R9 
    /*00003ad0*/ MOVGT       R7, #0 
    /*00003ad4*/ CMP         R7, #0 
    /*00003ad8*/ BNE         RgnIsRgnCascade_x3990
    /*00003adc*/ ADD         R7, R3, SL 
    /*00003ae0*/ STR         R7, [SP, #4] 
    /*00003ae4*/ SUB         SL, R7, #1 
    /*00003ae8*/ CMP         R4, SL 
    /*00003aec*/ MOVGT       R7, #0 
    /*00003af0*/ MOVLE       R7, #1 
    /*00003af4*/ CMP         R9, SL 
    /*00003af8*/ MOVLT       R7, #0 
    /*00003afc*/ CMP         R7, #0 
    /*00003b00*/ MOVEQ       R7, FP 
    /*00003b04*/ BEQ         RgnIsRgnCascade_x39d0
    /*00003b08*/ B           RgnIsRgnCascade_x3990
    RgnIsRgnCascade_x3b0c:
    /*00003b0c*/ ADD         SL, R3, SL 
    /*00003b10*/ STR         SL, [SP, #4] 
    /*00003b14*/ SUB         SL, SL, #1 
    /*00003b18*/ B           RgnIsRgnCascade_x3aa0
    RgnIsRgnCascade_x3b1c:
    /*00003b1c*/ CMP         R5, R6 
    /*00003b20*/ MOVGT       R5, #0 
    /*00003b24*/ ANDLE       R5, R8, #1 
    /*00003b28*/ CMP         R5, #0 
    /*00003b2c*/ BNE         RgnIsRgnCascade_x39e4
    /*00003b30*/ B           RgnIsRgnCascade_x39fc
FUNC_END RgnIsRgnCascade


FUNC_BEGIN RgnIsQuadRangleRgnCascade
    /*00003b34*/ MOV         R0, #0 
    /*00003b38*/ BX          LR 
FUNC_END RgnIsQuadRangleRgnCascade


FUNC_BEGIN RgnCheckChnRgnsOverlap
    /*00003b3c*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00003b40*/ SUB         SP, SP, #28 
    /*00003b44*/ MOV         R8, R2 
    /*00003b48*/ MOV         R9, R3 
    /*00003b4c*/ BL          0x0000000000003B4C          @ RgnGetChnManage [R_ARM_CALL: .text+0x11fc] 
    /*00003b50*/ SUBS        R4, R0, #0 
    /*00003b54*/ BEQ         RgnCheckChnRgnsOverlap_x3bf8
    RgnCheckChnRgnsOverlap_x3b58:
    /*00003b58*/ LDR         R7, [R4] 
    /*00003b5c*/ CMP         R7, #0 
    /*00003b60*/ ADDNE       R6, R4, #20 
    /*00003b64*/ MOVNE       R5, #0 
    /*00003b68*/ BEQ         RgnCheckChnRgnsOverlap_x3bbc
    RgnCheckChnRgnsOverlap_x3b6c:
    /*00003b6c*/ LDR         R3, [R6, #4]! 
    /*00003b70*/ MOV         R1, R9 
    /*00003b74*/ ADD         R0, SP, #8 
    /*00003b78*/ ADD         R5, R5, #1 
    /*00003b7c*/ CMP         R3, R8 
    /*00003b80*/ BEQ         RgnCheckChnRgnsOverlap_x3bb0
    /*00003b84*/ LDR         LR, [R4, #128]              @ 0x80 
    /*00003b88*/ LDR         IP, [R4, #132]              @ 0x84 
    /*00003b8c*/ LDR         R2, [R4, #136]              @ 0x88 
    /*00003b90*/ LDR         R3, [R4, #140]              @ 0x8c 
    /*00003b94*/ STR         LR, [SP, #8] 
    /*00003b98*/ STR         IP, [SP, #12] 
    /*00003b9c*/ STR         R2, [SP, #16] 
    /*00003ba0*/ STR         R3, [SP, #20] 
    /*00003ba4*/ BL          0x0000000000003BA4          @ RgnIsRgnCascade [R_ARM_CALL: .text+0x391c] 
    /*00003ba8*/ CMP         R0, #0 
    /*00003bac*/ BNE         RgnCheckChnRgnsOverlap_x3bc8
    RgnCheckChnRgnsOverlap_x3bb0:
    /*00003bb0*/ CMP         R5, R7 
    /*00003bb4*/ ADD         R4, R4, #168                @ 0xa8 
    /*00003bb8*/ BNE         RgnCheckChnRgnsOverlap_x3b6c
    RgnCheckChnRgnsOverlap_x3bbc:
    /*00003bbc*/ MOV         R0, #0 
    /*00003bc0*/ ADD         SP, SP, #28 
    /*00003bc4*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    RgnCheckChnRgnsOverlap_x3bc8:
    /*00003bc8*/ MOV         R1, #3 
    /*00003bcc*/ MOVW        R3, #2169                   @ 0x879 
    /*00003bd0*/ MOV         R0, R1 
    /*00003bd4*/ STR         R3, [SP] 
    /*00003bd8*/ MOVW        R2, #0                      @ .LC108 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1530] 
    /*00003bdc*/ LDR         R3, [PC, #52]               @ 0x0000000000003c18 
    /*00003be0*/ MOVT        R2, #0                      @ .LC108 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1530] 
    /*00003be4*/ BL          0x0000000000003BE4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003be8*/ MOVW        R0, #32771                  @ 0x8003 
    /*00003bec*/ MOVT        R0, #40963                  @ 0xa003 
    /*00003bf0*/ ADD         SP, SP, #28 
    /*00003bf4*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    RgnCheckChnRgnsOverlap_x3bf8:
    /*00003bf8*/ MOVW        R3, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa9c] 
    /*00003bfc*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00003c00*/ MOVT        R3, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa9c] 
    /*00003c04*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00003c08*/ MOVW        R2, #2151                   @ 0x867 
    /*00003c0c*/ LDR         R1, [PC, #4]                @ 0x0000000000003c18 
    /*00003c10*/ BL          0x0000000000003C10          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003c14*/ B           RgnCheckChnRgnsOverlap_x3b58
    /*00003c18*/ ANDEQ       R0, R0, R0, LSL R3 
FUNC_END RgnCheckChnRgnsOverlap


FUNC_BEGIN RgnCheckChnQuadRangleRgnsOverlap
    /*00003c1c*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00003c20*/ SUB         SP, SP, #40                 @ 0x28 
    /*00003c24*/ MOV         R7, R2 
    /*00003c28*/ BL          0x0000000000003C28          @ RgnGetChnManage [R_ARM_CALL: .text+0x11fc] 
    /*00003c2c*/ SUBS        R8, R0, #0 
    /*00003c30*/ BEQ         RgnCheckChnQuadRangleRgnsOverlap_x3c88
    RgnCheckChnQuadRangleRgnsOverlap_x3c34:
    /*00003c34*/ LDR         IP, [R8] 
    /*00003c38*/ CMP         IP, #0 
    /*00003c3c*/ BEQ         RgnCheckChnQuadRangleRgnsOverlap_x3c7c
    /*00003c40*/ ADD         R6, R8, #20 
    /*00003c44*/ ADD         R5, R8, #144                @ 0x90 
    /*00003c48*/ MOV         R4, #0 
    RgnCheckChnQuadRangleRgnsOverlap_x3c4c:
    /*00003c4c*/ LDR         R3, [R6, #4]! 
    /*00003c50*/ MOV         R1, R5 
    /*00003c54*/ MOV         R2, #40                     @ 0x28 
    /*00003c58*/ MOV         R0, SP 
    /*00003c5c*/ CMP         R3, R7 
    /*00003c60*/ ADD         R4, R4, #1 
    /*00003c64*/ ADD         R5, R5, #168                @ 0xa8 
    /*00003c68*/ BEQ         RgnCheckChnQuadRangleRgnsOverlap_x3c74
    /*00003c6c*/ BL          0x0000000000003C6C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00003c70*/ LDR         IP, [R8] 
    RgnCheckChnQuadRangleRgnsOverlap_x3c74:
    /*00003c74*/ CMP         IP, R4 
    /*00003c78*/ BHI         RgnCheckChnQuadRangleRgnsOverlap_x3c4c
    RgnCheckChnQuadRangleRgnsOverlap_x3c7c:
    /*00003c7c*/ MOV         R0, #0 
    /*00003c80*/ ADD         SP, SP, #40                 @ 0x28 
    /*00003c84*/ POP         {R4, R5, R6, R7, R8, PC} 
    RgnCheckChnQuadRangleRgnsOverlap_x3c88:
    /*00003c88*/ MOVW        R3, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa9c] 
    /*00003c8c*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00003c90*/ MOVT        R3, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa9c] 
    /*00003c94*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00003c98*/ MOVW        R2, #2189                   @ 0x88d 
    /*00003c9c*/ LDR         R1, [PC, #4]                @ 0x0000000000003ca8 
    /*00003ca0*/ BL          0x0000000000003CA0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003ca4*/ B           RgnCheckChnQuadRangleRgnsOverlap_x3c34
    /*00003ca8*/ ANDEQ       R0, R0, R8, LSR #6 
FUNC_END RgnCheckChnQuadRangleRgnsOverlap


FUNC_BEGIN RgnCmpLayer
    /*00003cac*/ CMP         R0, #0 
    /*00003cb0*/ LDR         R3, [R2] 
    /*00003cb4*/ LDR         R0, [R1] 
    /*00003cb8*/ BNE         RgnCmpLayer_x3ccc
    /*00003cbc*/ CMP         R0, R3 
    /*00003cc0*/ MOVHI       R0, #0 
    /*00003cc4*/ MOVLS       R0, #1 
    /*00003cc8*/ BX          LR 
    RgnCmpLayer_x3ccc:
    /*00003ccc*/ CMP         R0, R3 
    /*00003cd0*/ MOVLS       R0, #0 
    /*00003cd4*/ MOVHI       R0, #1 
    /*00003cd8*/ BX          LR 
FUNC_END RgnCmpLayer


FUNC_BEGIN RgnCmpPostion
    /*00003cdc*/ LDR         R3, [R2, #4] 
    /*00003ce0*/ LDR         IP, [R1, #4] 
    /*00003ce4*/ CMP         IP, R3 
    /*00003ce8*/ MOVGT       R3, #1 
    /*00003cec*/ BGT         RgnCmpPostion_x3cf8
    /*00003cf0*/ MOVNE       R3, #0 
    /*00003cf4*/ BEQ         RgnCmpPostion_x3d08
    RgnCmpPostion_x3cf8:
    /*00003cf8*/ CMP         R0, #0 
    /*00003cfc*/ EOREQ       R3, R3, #1 
    /*00003d00*/ MOV         R0, R3 
    /*00003d04*/ BX          LR 
    RgnCmpPostion_x3d08:
    /*00003d08*/ LDR         R3, [R1] 
    /*00003d0c*/ LDR         R2, [R2] 
    /*00003d10*/ CMP         R3, R2 
    /*00003d14*/ MOVLE       R3, #0 
    /*00003d18*/ MOVGT       R3, #1 
    /*00003d1c*/ B           RgnCmpPostion_x3cf8
FUNC_END RgnCmpPostion


FUNC_BEGIN RgnCmpHandleValue
    /*00003d20*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00003d24*/ SUBS        R6, R0, #0 
    /*00003d28*/ MOV         R5, R1 
    /*00003d2c*/ MOV         R4, R2 
    /*00003d30*/ MOV         R7, R3 
    /*00003d34*/ BEQ         RgnCmpHandleValue_x3dc8
    RgnCmpHandleValue_x3d38:
    /*00003d38*/ CMP         R5, #0 
    /*00003d3c*/ BEQ         RgnCmpHandleValue_x3de8
    RgnCmpHandleValue_x3d40:
    /*00003d40*/ CMP         R4, #0 
    /*00003d44*/ BEQ         RgnCmpHandleValue_x3d78
    /*00003d48*/ CMP         R4, #1 
    /*00003d4c*/ BNE         RgnCmpHandleValue_x3d70
    /*00003d50*/ LDR         R2, [R6, #12] 
    /*00003d54*/ LDR         R3, [R5, #12] 
    /*00003d58*/ CMP         R2, R3 
    /*00003d5c*/ MOVGT       R0, R4 
    /*00003d60*/ BLE         RgnCmpHandleValue_x3da8
    RgnCmpHandleValue_x3d64:
    /*00003d64*/ CMP         R7, #0 
    /*00003d68*/ EOREQ       R0, R0, #1 
    /*00003d6c*/ POP         {R4, R5, R6, R7, R8, PC} 
    RgnCmpHandleValue_x3d70:
    /*00003d70*/ MOV         R0, #0 
    /*00003d74*/ POP         {R4, R5, R6, R7, R8, PC} 
    RgnCmpHandleValue_x3d78:
    /*00003d78*/ CMP         R7, #0 
    /*00003d7c*/ LDR         R4, [R6, #64]               @ 0x40 
    /*00003d80*/ LDR         R0, [R5, #64]               @ 0x40 
    /*00003d84*/ BNE         RgnCmpHandleValue_x3d98
    /*00003d88*/ CMP         R0, R4 
    /*00003d8c*/ MOVCC       R0, #0 
    /*00003d90*/ MOVCS       R0, #1 
    /*00003d94*/ POP         {R4, R5, R6, R7, R8, PC} 
    RgnCmpHandleValue_x3d98:
    /*00003d98*/ CMP         R0, R4 
    /*00003d9c*/ MOVCS       R0, #0 
    /*00003da0*/ MOVCC       R0, #1 
    /*00003da4*/ POP         {R4, R5, R6, R7, R8, PC} 
    RgnCmpHandleValue_x3da8:
    /*00003da8*/ MOVNE       R0, #0 
    /*00003dac*/ BNE         RgnCmpHandleValue_x3d64
    /*00003db0*/ LDR         R0, [R6, #8] 
    /*00003db4*/ LDR         R4, [R5, #8] 
    /*00003db8*/ CMP         R0, R4 
    /*00003dbc*/ MOVLE       R0, #0 
    /*00003dc0*/ MOVGT       R0, #1 
    /*00003dc4*/ B           RgnCmpHandleValue_x3d64
    RgnCmpHandleValue_x3dc8:
    /*00003dc8*/ MOVW        R3, #0                      @ .LC109 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1560] 
    /*00003dcc*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00003dd0*/ MOVT        R3, #0                      @ .LC109 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1560] 
    /*00003dd4*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00003dd8*/ MOVW        R2, #2261                   @ 0x8d5 
    /*00003ddc*/ LDR         R1, [PC, #36]               @ 0x0000000000003e08 
    /*00003de0*/ BL          0x0000000000003DE0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003de4*/ B           RgnCmpHandleValue_x3d38
    RgnCmpHandleValue_x3de8:
    /*00003de8*/ MOVW        R3, #0                      @ .LC110 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1574] 
    /*00003dec*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00003df0*/ MOVT        R3, #0                      @ .LC110 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1574] 
    /*00003df4*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00003df8*/ MOVW        R2, #2262                   @ 0x8d6 
    /*00003dfc*/ LDR         R1, [PC, #4]                @ 0x0000000000003e08 
    /*00003e00*/ BL          0x0000000000003E00          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003e04*/ B           RgnCmpHandleValue_x3d40
    /*00003e08*/ ANDEQ       R0, R0, IP, ASR #6 
FUNC_END RgnCmpHandleValue


FUNC_BEGIN RgnChnRgnsSort
    /*00003e0c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00003e10*/ SUB         SP, SP, #172                @ 0xac 
    /*00003e14*/ MOV         R9, R2 
    /*00003e18*/ BL          0x0000000000003E18          @ RgnGetChnManage [R_ARM_CALL: .text+0x11fc] 
    /*00003e1c*/ SUBS        R7, R0, #0 
    /*00003e20*/ BEQ         RgnChnRgnsSort_x3f08
    RgnChnRgnsSort_x3e24:
    /*00003e24*/ LDR         R3, [R9, #232]              @ 0xe8 
    /*00003e28*/ CMP         R3, #0 
    /*00003e2c*/ BEQ         RgnChnRgnsSort_x3ef0
    /*00003e30*/ LDR         R4, [R7] 
    /*00003e34*/ CMP         R4, #1 
    /*00003e38*/ BLS         RgnChnRgnsSort_x3ef0
    /*00003e3c*/ SUB         FP, R4, #1 
    /*00003e40*/ MOV         R2, #168                    @ 0xa8 
    /*00003e44*/ ADD         R3, R4, #5 
    /*00003e48*/ MOV         R0, SP 
    /*00003e4c*/ MUL         SL, R2, FP 
    /*00003e50*/ LDR         R8, [R7, R3, LSL #2] 
    /*00003e54*/ ADD         R6, R7, #120                @ 0x78 
    /*00003e58*/ MOV         R5, #0 
    /*00003e5c*/ ADD         SL, SL, #120                @ 0x78 
    /*00003e60*/ ADD         R1, R7, SL 
    /*00003e64*/ BL          0x0000000000003E64          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00003e68*/ B           RgnChnRgnsSort_x3e7c
    RgnChnRgnsSort_x3e6c:
    /*00003e6c*/ ADD         R5, R5, #1 
    /*00003e70*/ ADD         R6, R6, #168                @ 0xa8 
    /*00003e74*/ CMP         R5, FP 
    /*00003e78*/ BEQ         RgnChnRgnsSort_x3ef8
    RgnChnRgnsSort_x3e7c:
    /*00003e7c*/ LDR         R3, [R9, #240]              @ 0xf0 
    /*00003e80*/ MOV         R1, SP 
    /*00003e84*/ LDR         R2, [R9, #236]              @ 0xec 
    /*00003e88*/ MOV         R0, R6 
    /*00003e8c*/ BL          0x0000000000003E8C          @ RgnCmpHandleValue [R_ARM_CALL: .text+0x3d20] 
    /*00003e90*/ CMP         R0, #1 
    /*00003e94*/ BNE         RgnChnRgnsSort_x3e6c
    RgnChnRgnsSort_x3e98:
    /*00003e98*/ SUB         R9, R4, #2 
    /*00003e9c*/ CMP         R9, R5 
    /*00003ea0*/ ADDGE       R4, R7, R4, LSL #2 
    /*00003ea4*/ ADDGE       R0, R7, SL 
    /*00003ea8*/ ADDGE       R4, R4, #20 
    /*00003eac*/ BLT         RgnChnRgnsSort_x3ed8
    RgnChnRgnsSort_x3eb0:
    /*00003eb0*/ LDR         R3, [R4, #-4]! 
    /*00003eb4*/ SUB         SL, R0, #168                @ 0xa8 
    /*00003eb8*/ SUB         R9, R9, #1 
    /*00003ebc*/ MOV         R2, #168                    @ 0xa8 
    /*00003ec0*/ MOV         R1, SL 
    /*00003ec4*/ STR         R3, [R4, #4] 
    /*00003ec8*/ BL          0x0000000000003EC8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00003ecc*/ CMP         R9, R5 
    /*00003ed0*/ MOV         R0, SL 
    /*00003ed4*/ BGE         RgnChnRgnsSort_x3eb0
    RgnChnRgnsSort_x3ed8:
    /*00003ed8*/ ADD         R5, R5, #6 
    /*00003edc*/ MOV         R1, SP 
    /*00003ee0*/ MOV         R0, R6 
    /*00003ee4*/ MOV         R2, #168                    @ 0xa8 
    /*00003ee8*/ STR         R8, [R7, R5, LSL #2] 
    /*00003eec*/ BL          0x0000000000003EEC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    RgnChnRgnsSort_x3ef0:
    /*00003ef0*/ ADD         SP, SP, #172                @ 0xac 
    /*00003ef4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnChnRgnsSort_x3ef8:
    /*00003ef8*/ MOV         R6, #168                    @ 0xa8 
    /*00003efc*/ MLA         R6, R6, R5, R7 
    /*00003f00*/ ADD         R6, R6, #120                @ 0x78 
    /*00003f04*/ B           RgnChnRgnsSort_x3e98
    RgnChnRgnsSort_x3f08:
    /*00003f08*/ MOVW        R3, #0                      @ .LC111 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1588] 
    /*00003f0c*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00003f10*/ MOVT        R3, #0                      @ .LC111 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1588] 
    /*00003f14*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00003f18*/ MOVW        R2, #2297                   @ 0x8f9 
    /*00003f1c*/ LDR         R1, [PC, #4]                @ 0x0000000000003f28 
    /*00003f20*/ BL          0x0000000000003F20          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00003f24*/ B           RgnChnRgnsSort_x3e24
    /*00003f28*/ ANDEQ       R0, R0, R0, ROR #6 
FUNC_END RgnChnRgnsSort


FUNC_BEGIN RgnAddRgnFrmChn
    /*00003f2c*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00003f30*/ SUBS        R6, R3, #0 
    /*00003f34*/ MOV         R4, R0 
    /*00003f38*/ MOV         R5, R1 
    /*00003f3c*/ MOV         R7, R2 
    /*00003f40*/ BEQ         RgnAddRgnFrmChn_x4014
    RgnAddRgnFrmChn_x3f44:
    /*00003f44*/ MOV         R1, R5 
    /*00003f48*/ MOV         R0, R4 
    /*00003f4c*/ BL          0x0000000000003F4C          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*00003f50*/ MOV         R1, R5 
    /*00003f54*/ MOV         R8, R0 
    /*00003f58*/ MOV         R0, R4 
    /*00003f5c*/ BL          0x0000000000003F5C          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*00003f60*/ LDR         R3, [R5, #4] 
    /*00003f64*/ LDR         R1, [R5, #8] 
    /*00003f68*/ LDR         R2, [R8, #12] 
    /*00003f6c*/ LDR         R0, [R0, #4] 
    /*00003f70*/ MLA         R3, R0, R3, R1 
    /*00003f74*/ LDR         R4, [R2, R3, LSL #2] 
    /*00003f78*/ CMP         R4, #0 
    /*00003f7c*/ BEQ         RgnAddRgnFrmChn_x4034
    RgnAddRgnFrmChn_x3f80:
    /*00003f80*/ LDR         R3, [R4] 
    /*00003f84*/ MOV         R0, #168                    @ 0xa8 
    /*00003f88*/ MOV         R2, R0 
    /*00003f8c*/ MOV         R1, R6 
    /*00003f90*/ MLA         R0, R0, R3, R4 
    /*00003f94*/ ADD         R3, R3, #6 
    /*00003f98*/ STR         R7, [R4, R3, LSL #2] 
    /*00003f9c*/ ADD         R0, R0, #120                @ 0x78 
    /*00003fa0*/ BL          0x0000000000003FA0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00003fa4*/ LDR         R3, [R5] 
    /*00003fa8*/ CMP         R3, #8 
    /*00003fac*/ BEQ         RgnAddRgnFrmChn_x3fcc
    RgnAddRgnFrmChn_x3fb0:
    /*00003fb0*/ LDR         R3, [R4] 
    /*00003fb4*/ MOV         R2, #1 
    /*00003fb8*/ MOV         R0, #0 
    /*00003fbc*/ STR         R2, [R4, #20] 
    /*00003fc0*/ ADD         R3, R3, R2 
    /*00003fc4*/ STR         R3, [R4] 
    /*00003fc8*/ POP         {R4, R5, R6, R7, R8, PC} 
    RgnAddRgnFrmChn_x3fcc:
    /*00003fcc*/ LDR         R3, [R6, #152]              @ 0x98 
    /*00003fd0*/ CMP         R3, #0 
    /*00003fd4*/ BEQ         RgnAddRgnFrmChn_x3ff0
    /*00003fd8*/ CMP         R3, #1 
    /*00003fdc*/ BNE         RgnAddRgnFrmChn_x4000
    /*00003fe0*/ LDR         R3, [R4, #12] 
    /*00003fe4*/ ADD         R3, R3, #1 
    /*00003fe8*/ STR         R3, [R4, #12] 
    /*00003fec*/ B           RgnAddRgnFrmChn_x3fb0
    RgnAddRgnFrmChn_x3ff0:
    /*00003ff0*/ LDR         R3, [R4, #8] 
    /*00003ff4*/ ADD         R3, R3, #1 
    /*00003ff8*/ STR         R3, [R4, #8] 
    /*00003ffc*/ B           RgnAddRgnFrmChn_x3fb0
    RgnAddRgnFrmChn_x4000:
    /*00004000*/ CMP         R3, #2 
    /*00004004*/ LDREQ       R3, [R4, #16] 
    /*00004008*/ ADDEQ       R3, R3, #1 
    /*0000400c*/ STREQ       R3, [R4, #16] 
    /*00004010*/ B           RgnAddRgnFrmChn_x3fb0
    RgnAddRgnFrmChn_x4014:
    /*00004014*/ MOVW        R3, #0                      @ .LC112 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a0] 
    /*00004018*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*0000401c*/ MOVT        R3, #0                      @ .LC112 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a0] 
    /*00004020*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00004024*/ MOVW        R2, #2342                   @ 0x926 
    /*00004028*/ LDR         R1, [PC, #36]               @ 0x0000000000004054 
    /*0000402c*/ BL          0x000000000000402C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004030*/ B           RgnAddRgnFrmChn_x3f44
    RgnAddRgnFrmChn_x4034:
    /*00004034*/ MOVW        R3, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa9c] 
    /*00004038*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*0000403c*/ MOVT        R3, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa9c] 
    /*00004040*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00004044*/ MOVW        R2, #2349                   @ 0x92d 
    /*00004048*/ LDR         R1, [PC, #4]                @ 0x0000000000004054 
    /*0000404c*/ BL          0x000000000000404C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004050*/ B           RgnAddRgnFrmChn_x3f80
    /*00004054*/ ANDEQ       R0, R0, R0, ROR R3 
FUNC_END RgnAddRgnFrmChn


FUNC_BEGIN RgnDelRgnFrmChn
    /*00004058*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000405c*/ SUB         SP, SP, #12 
    /*00004060*/ MOV         R9, R1 
    /*00004064*/ MOV         R5, R2 
    /*00004068*/ MOV         FP, R3 
    /*0000406c*/ MOV         R4, R0 
    /*00004070*/ BL          0x0000000000004070          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*00004074*/ MOV         R1, R9 
    /*00004078*/ MOV         SL, R0 
    /*0000407c*/ MOV         R0, R4 
    /*00004080*/ BL          0x0000000000004080          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*00004084*/ LDR         R8, [R9, #4] 
    /*00004088*/ LDR         R2, [R9, #8] 
    /*0000408c*/ LDR         R3, [SL, #12] 
    /*00004090*/ LDR         R1, [R0, #4] 
    /*00004094*/ MLA         R8, R1, R8, R2 
    /*00004098*/ LDR         R7, [R3, R8, LSL #2] 
    /*0000409c*/ LSL         R3, R8, #2 
    /*000040a0*/ STR         R3, [SP, #4] 
    /*000040a4*/ CMP         R7, #0 
    /*000040a8*/ BEQ         RgnDelRgnFrmChn_x41f8
    RgnDelRgnFrmChn_x40ac:
    /*000040ac*/ LDR         R6, [R7] 
    /*000040b0*/ CMP         R6, #0 
    /*000040b4*/ BEQ         RgnDelRgnFrmChn_x40ec
    /*000040b8*/ LDR         R3, [R7, #24] 
    /*000040bc*/ CMP         R5, R3 
    /*000040c0*/ ADDNE       R3, R7, #24 
    /*000040c4*/ MOVNE       R4, #0 
    /*000040c8*/ BNE         RgnDelRgnFrmChn_x40dc
    /*000040cc*/ B           RgnDelRgnFrmChn_x4148
    RgnDelRgnFrmChn_x40d0:
    /*000040d0*/ LDR         R2, [R3, #4]! 
    /*000040d4*/ CMP         R2, R5 
    /*000040d8*/ BEQ         RgnDelRgnFrmChn_x4150
    RgnDelRgnFrmChn_x40dc:
    /*000040dc*/ ADD         R4, R4, #1 
    /*000040e0*/ CMP         R4, R6 
    /*000040e4*/ MOV         R1, R4 
    /*000040e8*/ BNE         RgnDelRgnFrmChn_x40d0
    RgnDelRgnFrmChn_x40ec:
    /*000040ec*/ MOVW        R3, #0                      @ .LC113 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15b8] 
    /*000040f0*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000040f4*/ MOVT        R3, #0                      @ .LC113 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15b8] 
    /*000040f8*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000040fc*/ MOVW        R2, #2407                   @ 0x967 
    /*00004100*/ LDR         R1, [PC, #292]              @ 0x000000000000422c 
    /*00004104*/ BL          0x0000000000004104          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004108*/ LDR         R6, [R7] 
    /*0000410c*/ SUB         R6, R6, #1 
    RgnDelRgnFrmChn_x4110:
    /*00004110*/ LDR         R3, [R9] 
    /*00004114*/ CMP         R3, #8 
    /*00004118*/ BEQ         RgnDelRgnFrmChn_x41ac
    RgnDelRgnFrmChn_x411c:
    /*0000411c*/ CMP         R6, #0 
    /*00004120*/ STR         R6, [R7] 
    /*00004124*/ BNE         RgnDelRgnFrmChn_x413c
    /*00004128*/ LDR         R3, [R7, #4] 
    /*0000412c*/ CMP         R3, #0 
    /*00004130*/ MOVNE       R3, #2 
    /*00004134*/ STRNE       R3, [R7, #20] 
    /*00004138*/ BEQ         RgnDelRgnFrmChn_x41dc
    RgnDelRgnFrmChn_x413c:
    /*0000413c*/ MOV         R0, #0 
    /*00004140*/ ADD         SP, SP, #12 
    /*00004144*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnDelRgnFrmChn_x4148:
    /*00004148*/ MOV         R1, #0 
    /*0000414c*/ MOV         R4, R1 
    RgnDelRgnFrmChn_x4150:
    /*00004150*/ SUB         R6, R6, #1 
    /*00004154*/ CMP         R6, R1 
    /*00004158*/ BLS         RgnDelRgnFrmChn_x4110
    /*0000415c*/ MOV         R5, #168                    @ 0xa8 
    /*00004160*/ ADD         R3, R4, #6 
    /*00004164*/ MLA         R5, R5, R4, R7 
    /*00004168*/ ADD         R8, R7, R3, LSL #2 
    /*0000416c*/ ADD         R5, R5, #120                @ 0x78 
    RgnDelRgnFrmChn_x4170:
    /*00004170*/ LDR         IP, [R8, #4] 
    /*00004174*/ MOV         R0, R5 
    /*00004178*/ ADD         R5, R5, #168                @ 0xa8 
    /*0000417c*/ ADD         R4, R4, #1 
    /*00004180*/ MOV         R2, #168                    @ 0xa8 
    /*00004184*/ MOV         R1, R5 
    /*00004188*/ STR         IP, [R8], #4 
    /*0000418c*/ BL          0x000000000000418C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00004190*/ CMP         R4, R6 
    /*00004194*/ BCC         RgnDelRgnFrmChn_x4170
    /*00004198*/ LDR         R3, [R9] 
    /*0000419c*/ LDR         R6, [R7] 
    /*000041a0*/ CMP         R3, #8 
    /*000041a4*/ SUB         R6, R6, #1 
    /*000041a8*/ BNE         RgnDelRgnFrmChn_x411c
    RgnDelRgnFrmChn_x41ac:
    /*000041ac*/ CMP         FP, #0 
    /*000041b0*/ BEQ         RgnDelRgnFrmChn_x41cc
    /*000041b4*/ CMP         FP, #1 
    /*000041b8*/ BNE         RgnDelRgnFrmChn_x4218
    /*000041bc*/ LDR         R3, [R7, #12] 
    /*000041c0*/ SUB         R3, R3, #1 
    /*000041c4*/ STR         R3, [R7, #12] 
    /*000041c8*/ B           RgnDelRgnFrmChn_x411c
    RgnDelRgnFrmChn_x41cc:
    /*000041cc*/ LDR         R3, [R7, #8] 
    /*000041d0*/ SUB         R3, R3, #1 
    /*000041d4*/ STR         R3, [R7, #8] 
    /*000041d8*/ B           RgnDelRgnFrmChn_x411c
    RgnDelRgnFrmChn_x41dc:
    /*000041dc*/ LDR         R0, [SL, #12] 
    /*000041e0*/ LDR         R3, [SP, #4] 
    /*000041e4*/ ADD         R0, R0, R3 
    /*000041e8*/ BL          0x00000000000041E8          @ RgnFreeChnMng [R_ARM_CALL: .text+0x338c] 
    /*000041ec*/ MOV         R0, #0 
    /*000041f0*/ ADD         SP, SP, #12 
    /*000041f4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnDelRgnFrmChn_x41f8:
    /*000041f8*/ MOVW        R3, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa9c] 
    /*000041fc*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00004200*/ MOVT        R3, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa9c] 
    /*00004204*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00004208*/ MOVW        R2, #2395                   @ 0x95b 
    /*0000420c*/ LDR         R1, [PC, #24]               @ 0x000000000000422c 
    /*00004210*/ BL          0x0000000000004210          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004214*/ B           RgnDelRgnFrmChn_x40ac
    RgnDelRgnFrmChn_x4218:
    /*00004218*/ CMP         FP, #2 
    /*0000421c*/ LDREQ       R3, [R7, #16] 
    /*00004220*/ SUBEQ       R3, R3, #1 
    /*00004224*/ STREQ       R3, [R7, #16] 
    /*00004228*/ B           RgnDelRgnFrmChn_x411c
    /*0000422c*/ ANDEQ       R0, R0, R0, LSL #7 
FUNC_END RgnDelRgnFrmChn


FUNC_BEGIN RgnUpChnAttr
    /*00004230*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00004234*/ SUBS        R5, R0, #0 
    /*00004238*/ MOV         R6, R1 
    /*0000423c*/ MOV         R7, R2 
    /*00004240*/ MOV         R8, R3 
    /*00004244*/ LDR         R4, [SP, #24] 
    /*00004248*/ BEQ         RgnUpChnAttr_x43e4
    RgnUpChnAttr_x424c:
    /*0000424c*/ CMP         R6, #0 
    /*00004250*/ BEQ         RgnUpChnAttr_x4404
    RgnUpChnAttr_x4254:
    /*00004254*/ CMP         R7, #0 
    /*00004258*/ BEQ         RgnUpChnAttr_x4424
    RgnUpChnAttr_x425c:
    /*0000425c*/ LDR         R3, [R5] 
    /*00004260*/ SUB         R8, R8, #1 
    /*00004264*/ CMP         R3, R4 
    /*00004268*/ LDREQ       R3, [R6, #64]               @ 0x40 
    /*0000426c*/ STREQ       R3, [R7, #64]               @ 0x40 
    /*00004270*/ CMP         R8, #1 
    /*00004274*/ BLS         RgnUpChnAttr_x433c
    RgnUpChnAttr_x4278:
    /*00004278*/ LDR         R3, [R5, #16] 
    /*0000427c*/ CMP         R4, R3 
    /*00004280*/ BEQ         RgnUpChnAttr_x43d0
    RgnUpChnAttr_x4284:
    /*00004284*/ LDR         R3, [R5, #48]               @ 0x30 
    RgnUpChnAttr_x4288:
    /*00004288*/ CMP         R4, R3 
    /*0000428c*/ BEQ         RgnUpChnAttr_x43bc
    RgnUpChnAttr_x4290:
    /*00004290*/ LDR         R3, [R5, #128]              @ 0x80 
    /*00004294*/ CMP         R4, R3 
    /*00004298*/ LDREQ       R3, [R6, #112]              @ 0x70 
    /*0000429c*/ STREQ       R3, [R7, #112]              @ 0x70 
    /*000042a0*/ LDR         R3, [R5, #160]              @ 0xa0 
    /*000042a4*/ CMP         R4, R3 
    /*000042a8*/ LDREQ       R3, [R6, #76]               @ 0x4c 
    /*000042ac*/ STREQ       R3, [R7, #76]               @ 0x4c 
    /*000042b0*/ LDR         R3, [R5, #180]              @ 0xb4 
    /*000042b4*/ CMP         R4, R3 
    /*000042b8*/ LDREQ       R3, [R6, #80]               @ 0x50 
    /*000042bc*/ STREQ       R3, [R7, #80]               @ 0x50 
    /*000042c0*/ LDR         R3, [R5, #200]              @ 0xc8 
    /*000042c4*/ CMP         R4, R3 
    /*000042c8*/ LDREQ       R3, [R6, #72]               @ 0x48 
    /*000042cc*/ STREQ       R3, [R7, #72]               @ 0x48 
    /*000042d0*/ LDR         R3, [R5, #220]              @ 0xdc 
    /*000042d4*/ CMP         R4, R3 
    /*000042d8*/ LDREQ       R3, [R6, #68]               @ 0x44 
    /*000042dc*/ STREQ       R3, [R7, #68]               @ 0x44 
    /*000042e0*/ LDR         R3, [R5, #248]              @ 0xf8 
    /*000042e4*/ CMP         R4, R3 
    /*000042e8*/ BEQ         RgnUpChnAttr_x43a8
    RgnUpChnAttr_x42ec:
    /*000042ec*/ LDR         R3, [R5, #288]              @ 0x120 
    /*000042f0*/ CMP         R4, R3 
    /*000042f4*/ LDREQ       R3, [R6, #156]              @ 0x9c 
    /*000042f8*/ STREQ       R3, [R7, #156]              @ 0x9c 
    /*000042fc*/ CMP         R4, #0 
    /*00004300*/ BEQ         RgnUpChnAttr_x4330
    /*00004304*/ LDRD        R2, [R6, #88]               @ 0x58 
    /*00004308*/ STRD        R2, [R7, #88]               @ 0x58 
    /*0000430c*/ LDRD        R2, [R6, #96]               @ 0x60 
    /*00004310*/ STRD        R2, [R7, #96]               @ 0x60 
    /*00004314*/ LDR         R3, [R6, #104]              @ 0x68 
    /*00004318*/ STR         R3, [R7, #104]              @ 0x68 
    RgnUpChnAttr_x431c:
    /*0000431c*/ ADD         R1, R6, #164                @ 0xa4 
    /*00004320*/ ADD         R0, R7, #164                @ 0xa4 
    /*00004324*/ MOV         R2, #4 
    /*00004328*/ POP         {R4, R5, R6, R7, R8, LR} 
    RgnUpChnAttr_x432c:
    /*0000432c*/ B           RgnUpChnAttr_x432c          @ osal_memcpy [R_ARM_JUMP24: +0x0] 
    RgnUpChnAttr_x4330:
    /*00004330*/ LDR         R3, [R6] 
    /*00004334*/ STR         R3, [R7] 
    /*00004338*/ B           RgnUpChnAttr_x431c
    RgnUpChnAttr_x433c:
    /*0000433c*/ LDR         R2, [R6, #4] 
    /*00004340*/ CMP         R2, #1 
    /*00004344*/ BEQ         RgnUpChnAttr_x4380
    /*00004348*/ CMP         R2, #0 
    /*0000434c*/ BNE         RgnUpChnAttr_x4278
    /*00004350*/ LDR         R3, [R5, #16] 
    /*00004354*/ CMP         R4, R3 
    /*00004358*/ BEQ         RgnUpChnAttr_x4368
    /*0000435c*/ LDR         R3, [R5, #48]               @ 0x30 
    /*00004360*/ CMP         R4, R3 
    /*00004364*/ BNE         RgnUpChnAttr_x4288
    RgnUpChnAttr_x4368:
    /*00004368*/ STR         R2, [R7, #4] 
    /*0000436c*/ LDR         R3, [R6, #4] 
    /*00004370*/ CMP         R3, #1 
    /*00004374*/ BNE         RgnUpChnAttr_x4278
    /*00004378*/ LDR         R3, [R5, #84]               @ 0x54 
    /*0000437c*/ B           RgnUpChnAttr_x438c
    RgnUpChnAttr_x4380:
    /*00004380*/ LDR         R3, [R5, #84]               @ 0x54 
    /*00004384*/ CMP         R4, R3 
    /*00004388*/ BEQ         RgnUpChnAttr_x4368
    RgnUpChnAttr_x438c:
    /*0000438c*/ CMP         R4, R3 
    /*00004390*/ BNE         RgnUpChnAttr_x4290
    /*00004394*/ MOV         R2, #40                     @ 0x28 
    /*00004398*/ ADD         R1, R6, #24 
    /*0000439c*/ ADD         R0, R7, #24 
    /*000043a0*/ BL          0x00000000000043A0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000043a4*/ B           RgnUpChnAttr_x4290
    RgnUpChnAttr_x43a8:
    /*000043a8*/ MOV         R2, #12 
    /*000043ac*/ ADD         R1, R6, #120                @ 0x78 
    /*000043b0*/ ADD         R0, R7, #120                @ 0x78 
    /*000043b4*/ BL          0x00000000000043B4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000043b8*/ B           RgnUpChnAttr_x42ec
    RgnUpChnAttr_x43bc:
    /*000043bc*/ MOV         R2, #8 
    /*000043c0*/ ADD         R1, R6, #16 
    /*000043c4*/ ADD         R0, R7, #16 
    /*000043c8*/ BL          0x00000000000043C8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000043cc*/ B           RgnUpChnAttr_x4290
    RgnUpChnAttr_x43d0:
    /*000043d0*/ MOV         R2, #8 
    /*000043d4*/ ADD         R1, R6, R2 
    /*000043d8*/ ADD         R0, R7, R2 
    /*000043dc*/ BL          0x00000000000043DC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000043e0*/ B           RgnUpChnAttr_x4284
    RgnUpChnAttr_x43e4:
    /*000043e4*/ MOVW        R3, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb88] 
    /*000043e8*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000043ec*/ MOVT        R3, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb88] 
    /*000043f0*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000043f4*/ MOVW        R2, #2456                   @ 0x998 
    /*000043f8*/ LDR         R1, [PC, #68]               @ 0x0000000000004444 
    /*000043fc*/ BL          0x00000000000043FC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004400*/ B           RgnUpChnAttr_x424c
    RgnUpChnAttr_x4404:
    /*00004404*/ MOVW        R3, #0                      @ .LC109 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1560] 
    /*00004408*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*0000440c*/ MOVT        R3, #0                      @ .LC109 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1560] 
    /*00004410*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00004414*/ MOVW        R2, #2457                   @ 0x999 
    /*00004418*/ LDR         R1, [PC, #36]               @ 0x0000000000004444 
    /*0000441c*/ BL          0x000000000000441C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004420*/ B           RgnUpChnAttr_x4254
    RgnUpChnAttr_x4424:
    /*00004424*/ MOVW        R3, #0                      @ .LC114 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15c8] 
    /*00004428*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*0000442c*/ MOVT        R3, #0                      @ .LC114 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15c8] 
    /*00004430*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00004434*/ MOVW        R2, #2458                   @ 0x99a 
    /*00004438*/ LDR         R1, [PC, #4]                @ 0x0000000000004444 
    /*0000443c*/ BL          0x000000000000443C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004440*/ B           RgnUpChnAttr_x425c
    /*00004444*/ MULEQ       R0, R0, R3 
FUNC_END RgnUpChnAttr


FUNC_BEGIN RgnUpdBgColor
    /*00004448*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000444c*/ SUBS        R4, R0, #0 
    /*00004450*/ SUB         SP, SP, #12 
    /*00004454*/ MOV         R5, R1 
    /*00004458*/ BEQ         RgnUpdBgColor_x4518
    RgnUpdBgColor_x445c:
    /*0000445c*/ LDR         R3, [R4, #116]              @ 0x74 
    /*00004460*/ CMP         R3, #1 
    /*00004464*/ BLS         RgnUpdBgColor_x4500
    /*00004468*/ MOV         R0, R4 
    /*0000446c*/ BL          0x00000000000000B8 
    /*00004470*/ CMP         R0, #0 
    /*00004474*/ BLT         RgnUpdBgColor_x44f8
    /*00004478*/ ASR         R1, R0, #31 
    /*0000447c*/ STR         R0, [R4, #212]              @ 0xd4 
    RgnUpdBgColor_x4480:
    /*00004480*/ LDR         LR, [R4, #240]              @ 0xf0 
    /*00004484*/ ADD         R3, R4, #256                @ 0x100 
    /*00004488*/ LDRD        SL, [R4, #248]              @ 0xf8 
    /*0000448c*/ LDRD        R8, [R3] 
    /*00004490*/ UMULL       R6, R7, LR, R0 
    /*00004494*/ LDR         IP, [R4, #28] 
    /*00004498*/ ADDS        SL, SL, R6 
    /*0000449c*/ MLA         R7, LR, R1, R7 
    /*000044a0*/ ADC         FP, FP, R7 
    /*000044a4*/ ADDS        R6, R6, R8 
    /*000044a8*/ ADC         R7, R7, R9 
    /*000044ac*/ CMP         IP, #0 
    /*000044b0*/ MOVNE       R9, R6 
    /*000044b4*/ MOVNE       R8, #0 
    /*000044b8*/ BEQ         RgnUpdBgColor_x44e4
    RgnUpdBgColor_x44bc:
    /*000044bc*/ LDR         R0, [R4, #268]              @ 0x10c 
    /*000044c0*/ MOV         R1, R5 
    /*000044c4*/ LDR         R3, [R4, #24] 
    /*000044c8*/ LDR         R2, [R4, #264]              @ 0x108 
    /*000044cc*/ MLA         R0, R0, R8, R9 
    /*000044d0*/ BL          0x00000000000044D0          @ RgnMemSet [R_ARM_CALL: .text+0xbd0] 
    /*000044d4*/ LDR         R3, [R4, #28] 
    /*000044d8*/ ADD         R8, R8, #1 
    /*000044dc*/ CMP         R3, R8 
    /*000044e0*/ BHI         RgnUpdBgColor_x44bc
    RgnUpdBgColor_x44e4:
    /*000044e4*/ MOV         R0, #0 
    /*000044e8*/ MOV         R3, #1 
    /*000044ec*/ STRD        SL, [R4, #96]               @ 0x60 
    /*000044f0*/ STRD        R6, [R4, #104]              @ 0x68 
    /*000044f4*/ STR         R3, [R4, #232]              @ 0xe8 
    RgnUpdBgColor_x44f8:
    /*000044f8*/ ADD         SP, SP, #12 
    /*000044fc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnUpdBgColor_x4500:
    /*00004500*/ BNE         RgnUpdBgColor_x4538
    /*00004504*/ MOV         R3, #0 
    /*00004508*/ MOV         R0, #0 
    /*0000450c*/ MOV         R1, #0 
    /*00004510*/ STR         R3, [R4, #212]              @ 0xd4 
    /*00004514*/ B           RgnUpdBgColor_x4480
    RgnUpdBgColor_x4518:
    /*00004518*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c] 
    /*0000451c*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00004520*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c] 
    /*00004524*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00004528*/ MOVW        R2, #2568                   @ 0xa08 
    /*0000452c*/ LDR         R1, [PC, #56]               @ 0x000000000000456c 
    /*00004530*/ BL          0x0000000000004530          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004534*/ B           RgnUpdBgColor_x445c
    RgnUpdBgColor_x4538:
    /*00004538*/ MOV         R1, #3 
    /*0000453c*/ MOV         R3, #0 
    /*00004540*/ MOV         R0, R1 
    /*00004544*/ STR         R3, [SP, #4] 
    /*00004548*/ MOVW        R2, #0                      @ .LC115 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15dc] 
    /*0000454c*/ MOVW        R3, #2587                   @ 0xa1b 
    /*00004550*/ MOVT        R2, #0                      @ .LC115 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15dc] 
    /*00004554*/ STR         R3, [SP] 
    /*00004558*/ LDR         R3, [PC, #12]               @ 0x000000000000456c 
    /*0000455c*/ BL          0x000000000000455C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004560*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004564*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004568*/ B           RgnUpdBgColor_x44f8
    /*0000456c*/ ANDEQ       R0, R0, R0, LSR #7 
FUNC_END RgnUpdBgColor


FUNC_BEGIN RgnCheckSetAttr
    /*00004570*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00004574*/ SUB         SP, SP, #252                @ 0xfc 
    /*00004578*/ MOV         R8, R1 
    /*0000457c*/ MOV         R4, R0 
    /*00004580*/ MOV         R2, #240                    @ 0xf0 
    /*00004584*/ MOV         R1, #0 
    /*00004588*/ ADD         R0, SP, #8 
    /*0000458c*/ BL          0x000000000000458C          @ memset [R_ARM_CALL: +0x0] 
    /*00004590*/ CMP         R8, #0 
    /*00004594*/ BEQ         RgnCheckSetAttr_x4674
    RgnCheckSetAttr_x4598:
    /*00004598*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000459c*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000045a0*/ ADD         R6, R6, R4, LSL #2 
    /*000045a4*/ LDR         R2, [R6, #8] 
    /*000045a8*/ ADD         SL, R2, #8 
    /*000045ac*/ MOV         R3, R2 
    /*000045b0*/ LDR         R7, [R2, #176]              @ 0xb0 
    /*000045b4*/ LDR         R4, [R3, #308]!             @ 0x134 
    /*000045b8*/ CMP         R4, R3 
    /*000045bc*/ LDR         R5, [R4] 
    /*000045c0*/ MOVNE       R9, #1 
    /*000045c4*/ BNE         RgnCheckSetAttr_x45e4
    /*000045c8*/ B           RgnCheckSetAttr_x4664
    RgnCheckSetAttr_x45cc:
    /*000045cc*/ LDR         R3, [R6, #8] 
    /*000045d0*/ MOV         R4, R5 
    /*000045d4*/ LDR         R5, [R5] 
    /*000045d8*/ ADD         R3, R3, #308                @ 0x134 
    /*000045dc*/ CMP         R4, R3 
    /*000045e0*/ BEQ         RgnCheckSetAttr_x4664
    RgnCheckSetAttr_x45e4:
    /*000045e4*/ LDR         R1, [R4, #8] 
    /*000045e8*/ ADD         R3, SP, #248                @ 0xf8 
    /*000045ec*/ ADD         R3, R3, R1, LSL #2 
    /*000045f0*/ LDR         R3, [R3, #-240]             @ 0xffffff10 
    /*000045f4*/ CMP         R3, #0 
    /*000045f8*/ BNE         RgnCheckSetAttr_x45cc
    /*000045fc*/ MOV         R0, R7 
    /*00004600*/ BL          0x0000000000004600          @ RgnGetCapacity [R_ARM_CALL: .text+0x3668] 
    /*00004604*/ MOV         R2, R8 
    /*00004608*/ MOV         R1, SL 
    /*0000460c*/ SUBS        FP, R0, #0 
    /*00004610*/ MOV         R0, R7 
    /*00004614*/ MOV         R3, FP 
    /*00004618*/ BEQ         RgnCheckSetAttr_x45cc
    /*0000461c*/ LDR         IP, [R4, #8] 
    /*00004620*/ ADD         LR, SP, #248                @ 0xf8 
    /*00004624*/ STR         R9, [SP] 
    /*00004628*/ ADD         IP, LR, IP, LSL #2 
    /*0000462c*/ STR         R9, [IP, #-240]             @ 0xffffff10 
    /*00004630*/ BL          0x0000000000004630          @ RgnCheckStatic [R_ARM_CALL: .text+0x201c] 
    /*00004634*/ MOV         R3, #1 
    /*00004638*/ MOV         R2, FP 
    /*0000463c*/ MOV         R1, R8 
    /*00004640*/ SUBS        IP, R0, #0 
    /*00004644*/ MOV         R0, R7 
    /*00004648*/ BNE         RgnCheckSetAttr_x4658
    /*0000464c*/ BL          0x000000000000464C          @ RgnCheckAttr [R_ARM_CALL: .text+0x27d0] 
    /*00004650*/ SUBS        IP, R0, #0 
    /*00004654*/ BEQ         RgnCheckSetAttr_x45cc
    RgnCheckSetAttr_x4658:
    /*00004658*/ MOV         R0, IP 
    /*0000465c*/ ADD         SP, SP, #252                @ 0xfc 
    /*00004660*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnCheckSetAttr_x4664:
    /*00004664*/ MOV         IP, #0 
    /*00004668*/ MOV         R0, IP 
    /*0000466c*/ ADD         SP, SP, #252                @ 0xfc 
    /*00004670*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnCheckSetAttr_x4674:
    /*00004674*/ MOVW        R3, #0                      @ .LC61 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb6c] 
    /*00004678*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*0000467c*/ MOVT        R3, #0                      @ .LC61 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb6c] 
    /*00004680*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00004684*/ MOVW        R2, #2625                   @ 0xa41 
    /*00004688*/ LDR         R1, [PC, #4]                @ 0x0000000000004694 
    /*0000468c*/ BL          0x000000000000468C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004690*/ B           RgnCheckSetAttr_x4598
    /*00004694*/ @           <UNDEFINED> instruction: 0x000003b0 
FUNC_END RgnCheckSetAttr


FUNC_BEGIN RgnCheckAttachFiled
    /*00004698*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000469c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000046a0*/ ADD         R3, R3, R0, LSL #2 
    /*000046a4*/ PUSH        {R4, R5, LR} 
    /*000046a8*/ MOV         R5, R2 
    /*000046ac*/ LDR         R3, [R3, #8] 
    /*000046b0*/ MOV         R2, R0 
    /*000046b4*/ SUB         SP, SP, #20 
    /*000046b8*/ LDR         R4, [R3, #176]              @ 0xb0 
    /*000046bc*/ MOV         R0, R4 
    /*000046c0*/ BL          0x00000000000046C0          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*000046c4*/ ADDS        R3, R0, #0 
    /*000046c8*/ MOVNE       R3, #1 
    /*000046cc*/ CMP         R4, #0 
    /*000046d0*/ MOVNE       R3, #0 
    /*000046d4*/ CMP         R3, #0 
    /*000046d8*/ BEQ         RgnCheckAttachFiled_x46ec
    /*000046dc*/ LDR         R2, [R5, #152]              @ 0x98 
    /*000046e0*/ LDR         R3, [R0, #152]              @ 0x98 
    /*000046e4*/ CMP         R2, R3 
    /*000046e8*/ BNE         RgnCheckAttachFiled_x46f8
    RgnCheckAttachFiled_x46ec:
    /*000046ec*/ MOV         R0, #0 
    RgnCheckAttachFiled_x46f0:
    /*000046f0*/ ADD         SP, SP, #20 
    /*000046f4*/ POP         {R4, R5, PC} 
    RgnCheckAttachFiled_x46f8:
    /*000046f8*/ MOV         R1, #3 
    /*000046fc*/ STR         R2, [SP, #8] 
    /*00004700*/ STR         R3, [SP, #4] 
    /*00004704*/ MOV         R0, R1 
    /*00004708*/ MOVW        R3, #2674                   @ 0xa72 
    /*0000470c*/ MOVW        R2, #0                      @ .LC116 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1614] 
    /*00004710*/ STR         R3, [SP] 
    /*00004714*/ MOVT        R2, #0                      @ .LC116 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1614] 
    /*00004718*/ LDR         R3, [PC, #12]               @ 0x000000000000472c 
    /*0000471c*/ BL          0x000000000000471C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004720*/ MOVW        R0, #32777                  @ 0x8009 
    /*00004724*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004728*/ B           RgnCheckAttachFiled_x46f0
    /*0000472c*/ ANDEQ       R0, R0, R0, ASR #7 
FUNC_END RgnCheckAttachFiled


FUNC_BEGIN RgnCheckSetDisplayAttr
    /*00004730*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00004734*/ MOV         R8, R0 
    /*00004738*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000473c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00004740*/ ADD         R0, R0, R8, LSL #2 
    /*00004744*/ MOV         R5, R2 
    /*00004748*/ SUB         SP, SP, #28 
    /*0000474c*/ MOV         R2, R3 
    /*00004750*/ LDR         R0, [R0, #8] 
    /*00004754*/ MOV         R7, R3 
    /*00004758*/ MOV         R9, R1 
    /*0000475c*/ MOV         R3, #0 
    /*00004760*/ MOV         R1, R5 
    /*00004764*/ LDR         R4, [R0, #176]              @ 0xb0 
    /*00004768*/ MOV         R0, R4 
    /*0000476c*/ BL          0x000000000000476C          @ RgnCheckAttr [R_ARM_CALL: .text+0x27d0] 
    /*00004770*/ SUBS        R6, R0, #0 
    /*00004774*/ BEQ         RgnCheckSetDisplayAttr_x4784
    RgnCheckSetDisplayAttr_x4778:
    /*00004778*/ MOV         R0, R6 
    /*0000477c*/ ADD         SP, SP, #28 
    /*00004780*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    RgnCheckSetDisplayAttr_x4784:
    /*00004784*/ MOV         R1, R9 
    /*00004788*/ MOV         R2, R8 
    /*0000478c*/ MOV         R0, R4 
    /*00004790*/ BL          0x0000000000004790          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*00004794*/ SUBS        R1, R0, #0 
    /*00004798*/ BEQ         RgnCheckSetDisplayAttr_x4778
    /*0000479c*/ STR         R6, [SP] 
    /*000047a0*/ MOV         R3, R7 
    /*000047a4*/ MOV         R2, R5 
    /*000047a8*/ MOV         R0, R4 
    /*000047ac*/ BL          0x00000000000047AC          @ RgnCheckStatic [R_ARM_CALL: .text+0x201c] 
    /*000047b0*/ CMP         R0, #0 
    /*000047b4*/ BNE         RgnCheckSetDisplayAttr_x482c
    /*000047b8*/ LDR         R3, [R7, #308]              @ 0x134 
    /*000047bc*/ CMP         R3, #0 
    /*000047c0*/ BNE         RgnCheckSetDisplayAttr_x480c
    /*000047c4*/ SUB         R3, R4, #1 
    /*000047c8*/ CMP         R3, #1 
    /*000047cc*/ BLS         RgnCheckSetDisplayAttr_x483c
    RgnCheckSetDisplayAttr_x47d0:
    /*000047d0*/ LDR         IP, [R5, #12] 
    /*000047d4*/ MOV         R2, R8 
    /*000047d8*/ LDR         R0, [R5, #8] 
    /*000047dc*/ ADD         R3, SP, #8 
    /*000047e0*/ LDR         LR, [R5, #16] 
    /*000047e4*/ MOV         R1, R9 
    /*000047e8*/ STR         IP, [SP, #12] 
    /*000047ec*/ LDR         IP, [R5, #20] 
    /*000047f0*/ STR         R0, [SP, #8] 
    /*000047f4*/ MOV         R0, R4 
    /*000047f8*/ STR         LR, [SP, #16] 
    /*000047fc*/ STR         IP, [SP, #20] 
    /*00004800*/ BL          0x0000000000004800          @ RgnCheckChnRgnsOverlap [R_ARM_CALL: .text+0x3b3c] 
    /*00004804*/ CMP         R0, #0 
    /*00004808*/ BNE         RgnCheckSetDisplayAttr_x482c
    RgnCheckSetDisplayAttr_x480c:
    /*0000480c*/ CMP         R4, #0 
    /*00004810*/ BNE         RgnCheckSetDisplayAttr_x4778
    /*00004814*/ LDR         R2, [R5, #112]              @ 0x70 
    /*00004818*/ MOV         R1, R9 
    /*0000481c*/ MOV         R0, R4 
    /*00004820*/ BL          0x0000000000004820          @ RgnCheckPixlFmtUnique [R_ARM_CALL: .text+0x1ed8] 
    /*00004824*/ MOV         R6, R0 
    /*00004828*/ B           RgnCheckSetDisplayAttr_x4778
    RgnCheckSetDisplayAttr_x482c:
    /*0000482c*/ MOV         R6, R0 
    /*00004830*/ MOV         R0, R6 
    /*00004834*/ ADD         SP, SP, #28 
    /*00004838*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    RgnCheckSetDisplayAttr_x483c:
    /*0000483c*/ LDR         R3, [R5, #4] 
    /*00004840*/ CMP         R3, #1 
    /*00004844*/ BNE         RgnCheckSetDisplayAttr_x47d0
    /*00004848*/ ADD         R3, R5, #24 
    /*0000484c*/ MOV         R2, R8 
    /*00004850*/ MOV         R1, R9 
    /*00004854*/ MOV         R0, R4 
    /*00004858*/ BL          0x0000000000004858          @ RgnCheckChnQuadRangleRgnsOverlap [R_ARM_CALL: .text+0x3c1c] 
    /*0000485c*/ MOV         R6, R0 
    /*00004860*/ B           RgnCheckSetDisplayAttr_x4778
FUNC_END RgnCheckSetDisplayAttr


FUNC_BEGIN RgnInitCtrl
    /*00004864*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00004868*/ SUBS        R8, R0, #0 
    /*0000486c*/ MOV         R4, R1 
    /*00004870*/ BEQ         RgnInitCtrl_x4960
    RgnInitCtrl_x4874:
    /*00004874*/ CMP         R4, #0 
    /*00004878*/ BEQ         RgnInitCtrl_x4980
    RgnInitCtrl_x487c:
    /*0000487c*/ MOV         R1, #0 
    /*00004880*/ MOV         R2, #576                    @ 0x240 
    /*00004884*/ MOV         R0, R4 
    /*00004888*/ MOV         R5, R1 
    /*0000488c*/ BL          0x000000000000488C          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00004890*/ MOV         R3, #1 
    /*00004894*/ STR         R3, [R4] 
    /*00004898*/ ADD         R3, R4, #256                @ 0x100 
    /*0000489c*/ LDR         R2, [R8] 
    /*000048a0*/ MOV         R6, #0 
    /*000048a4*/ MOV         R7, #0 
    /*000048a8*/ STR         R5, [R4, #184]              @ 0xb8 
    /*000048ac*/ STR         R5, [R4, #188]              @ 0xbc 
    /*000048b0*/ ADD         R0, R4, #300                @ 0x12c 
    /*000048b4*/ STR         R2, [R4, #176]              @ 0xb0 
    /*000048b8*/ STR         R5, [R4, #192]              @ 0xc0 
    /*000048bc*/ STR         R5, [R4, #196]              @ 0xc4 
    /*000048c0*/ STR         R5, [R4, #200]              @ 0xc8 
    /*000048c4*/ STR         R5, [R4, #204]              @ 0xcc 
    /*000048c8*/ STR         R5, [R4, #180]              @ 0xb4 
    /*000048cc*/ STR         R5, [R4, #208]              @ 0xd0 
    /*000048d0*/ STR         R5, [R4, #212]              @ 0xd4 
    /*000048d4*/ STR         R5, [R4, #216]              @ 0xd8 
    /*000048d8*/ STR         R5, [R4, #220]              @ 0xdc 
    /*000048dc*/ STR         R5, [R4, #228]              @ 0xe4 
    /*000048e0*/ STR         R5, [R4, #224]              @ 0xe0 
    /*000048e4*/ STR         R5, [R4, #232]              @ 0xe8 
    /*000048e8*/ STR         R5, [R4, #236]              @ 0xec 
    /*000048ec*/ STR         R5, [R4, #240]              @ 0xf0 
    /*000048f0*/ STRD        R6, [R4, #248]              @ 0xf8 
    /*000048f4*/ STRD        R6, [R3] 
    /*000048f8*/ STR         R5, [R4, #264]              @ 0x108 
    /*000048fc*/ STR         R5, [R4, #272]              @ 0x110 
    /*00004900*/ STR         R5, [R4, #276]              @ 0x114 
    /*00004904*/ STR         R5, [R4, #280]              @ 0x118 
    /*00004908*/ STR         R5, [R4, #284]              @ 0x11c 
    /*0000490c*/ STR         R5, [R4, #288]              @ 0x120 
    /*00004910*/ STR         R5, [R4, #292]              @ 0x124 
    /*00004914*/ STR         R5, [R4, #296]              @ 0x128 
    /*00004918*/ BL          0x0000000000004918          @ osal_wait_init [R_ARM_CALL: +0x0] 
    /*0000491c*/ ADD         R0, R4, #304                @ 0x130 
    /*00004920*/ BL          0x0000000000004920          @ osal_wait_init [R_ARM_CALL: +0x0] 
    /*00004924*/ ADD         R3, R4, #308                @ 0x134 
    /*00004928*/ STR         R5, [R4, #8] 
    /*0000492c*/ MOV         R0, R8 
    /*00004930*/ STRD        R6, [R4, #96]               @ 0x60 
    /*00004934*/ MOV         R2, #4 
    /*00004938*/ STRD        R6, [R4, #104]              @ 0x68 
    /*0000493c*/ ADD         R1, R4, #8 
    /*00004940*/ STR         R5, [R4, #112]              @ 0x70 
    /*00004944*/ STR         R5, [R4, #160]              @ 0xa0 
    /*00004948*/ STR         R5, [R4, #316]              @ 0x13c 
    /*0000494c*/ STR         R3, [R4, #308]              @ 0x134 
    /*00004950*/ STR         R3, [R4, #312]              @ 0x138 
    /*00004954*/ STR         R2, [R4, #124]              @ 0x7c 
    /*00004958*/ POP         {R4, R5, R6, R7, R8, LR} 
    RgnInitCtrl_x495c:
    /*0000495c*/ B           RgnInitCtrl_x495c           @ RgnAttrToComm [R_ARM_JUMP24: .text+0x47c] 
    RgnInitCtrl_x4960:
    /*00004960*/ MOVW        R3, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19c] 
    /*00004964*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00004968*/ MOVT        R3, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19c] 
    /*0000496c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00004970*/ MOVW        R2, #2751                   @ 0xabf 
    /*00004974*/ LDR         R1, [PC, #36]               @ 0x00000000000049a0 
    /*00004978*/ BL          0x0000000000004978          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000497c*/ B           RgnInitCtrl_x4874
    RgnInitCtrl_x4980:
    /*00004980*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c] 
    /*00004984*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00004988*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c] 
    /*0000498c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00004990*/ MOV         R2, #2752                   @ 0xac0 
    /*00004994*/ LDR         R1, [PC, #4]                @ 0x00000000000049a0 
    /*00004998*/ BL          0x0000000000004998          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000499c*/ B           RgnInitCtrl_x487c
    /*000049a0*/ LDRDEQ      R0, [R0], -R4 
FUNC_END RgnInitCtrl


FUNC_BEGIN RgnCheckAttachDestByChn
    /*000049a4*/ LDR         R3, [R0] 
    /*000049a8*/ CMP         R3, #8 
    /*000049ac*/ BEQ         RgnCheckAttachDestByChn_x49b8
    /*000049b0*/ MOV         R0, #0 
    /*000049b4*/ BX          LR 
    RgnCheckAttachDestByChn_x49b8:
    /*000049b8*/ MOV         R0, R1 
    /*000049bc*/ B           0x0000000000000324 
FUNC_END RgnCheckAttachDestByChn


FUNC_BEGIN RgnCheckColorLUT
    /*000049c0*/ PUSH        {R4, R5, LR} 
    /*000049c4*/ SUBS        R5, R0, #0 
    /*000049c8*/ SUB         SP, SP, #20 
    /*000049cc*/ MOV         R4, R1 
    /*000049d0*/ BEQ         RgnCheckColorLUT_x4a4c
    RgnCheckColorLUT_x49d4:
    /*000049d4*/ LDR         R3, [R5, #112]              @ 0x70 
    /*000049d8*/ CMP         R3, #12 
    /*000049dc*/ BEQ         RgnCheckColorLUT_x49ec
    RgnCheckColorLUT_x49e0:
    /*000049e0*/ MOV         R0, #0 
    RgnCheckColorLUT_x49e4:
    /*000049e4*/ ADD         SP, SP, #20 
    /*000049e8*/ POP         {R4, R5, PC} 
    RgnCheckColorLUT_x49ec:
    /*000049ec*/ CMP         R4, #3 
    /*000049f0*/ CMPNE       R4, #0 
    /*000049f4*/ MOVEQ       R0, #1 
    /*000049f8*/ MOVNE       R0, #0 
    /*000049fc*/ BNE         RgnCheckColorLUT_x49e4
    /*00004a00*/ LDRH        R3, [R5, #164]              @ 0xa4 
    /*00004a04*/ LDRH        R2, [R5, #166]              @ 0xa6 
    /*00004a08*/ TST         R3, #32768                  @ 0x8000 
    /*00004a0c*/ BNE         RgnCheckColorLUT_x4a18
    /*00004a10*/ TST         R2, #32768                  @ 0x8000 
    /*00004a14*/ BEQ         RgnCheckColorLUT_x49e0
    RgnCheckColorLUT_x4a18:
    /*00004a18*/ MOV         R1, #3 
    /*00004a1c*/ STR         R2, [SP, #8] 
    /*00004a20*/ STR         R3, [SP, #4] 
    /*00004a24*/ MOV         R0, R1 
    /*00004a28*/ MOVW        R3, #2824                   @ 0xb08 
    /*00004a2c*/ MOVW        R2, #0                      @ .LC117 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1660] 
    /*00004a30*/ STR         R3, [SP] 
    /*00004a34*/ MOVT        R2, #0                      @ .LC117 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1660] 
    /*00004a38*/ LDR         R3, [PC, #44]               @ 0x0000000000004a6c 
    /*00004a3c*/ BL          0x0000000000004A3C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004a40*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004a44*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004a48*/ B           RgnCheckColorLUT_x49e4
    RgnCheckColorLUT_x4a4c:
    /*00004a4c*/ MOVW        R3, #0                      @ .LC112 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a0] 
    /*00004a50*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00004a54*/ MOVT        R3, #0                      @ .LC112 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a0] 
    /*00004a58*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00004a5c*/ MOVW        R2, #2819                   @ 0xb03 
    /*00004a60*/ LDR         R1, [PC, #4]                @ 0x0000000000004a6c 
    /*00004a64*/ BL          0x0000000000004A64          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004a68*/ B           RgnCheckColorLUT_x49d4
    /*00004a6c*/ ANDEQ       R0, R0, R0, ROR #7 
FUNC_END RgnCheckColorLUT


FUNC_BEGIN RgnCheckCapacityByModId
    /*00004a70*/ CMP         R0, #4 
    /*00004a74*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00004a78*/ MOV         R4, R0 
    /*00004a7c*/ SUB         SP, SP, #20 
    /*00004a80*/ MOV         R5, R1 
    /*00004a84*/ MOV         R6, R2 
    /*00004a88*/ BHI         RgnCheckCapacityByModId_x4b0c
    RgnCheckCapacityByModId_x4a8c:
    /*00004a8c*/ CMP         R5, #0 
    /*00004a90*/ BEQ         RgnCheckCapacityByModId_x4b58
    RgnCheckCapacityByModId_x4a94:
    /*00004a94*/ LDR         R3, [R5] 
    /*00004a98*/ CMP         R3, #59                     @ 0x3b 
    /*00004a9c*/ BHI         RgnCheckCapacityByModId_x4b68
    /*00004aa0*/ MOV         R1, R3 
    /*00004aa4*/ MOV         R0, R4 
    /*00004aa8*/ BL          0x0000000000004AA8          @ RgnGetCapacity [R_ARM_CALL: .text+0x3668] 
    /*00004aac*/ SUBS        R7, R0, #0 
    /*00004ab0*/ BEQ         RgnCheckCapacityByModId_x4b20
    /*00004ab4*/ LDR         R3, [R7, #320]              @ 0x140 
    /*00004ab8*/ CMP         R3, #0 
    /*00004abc*/ BEQ         RgnCheckCapacityByModId_x4ad4
    /*00004ac0*/ MOV         R1, R5 
    /*00004ac4*/ MOV         R0, R4 
    /*00004ac8*/ BLX         R3 
    /*00004acc*/ CMP         R0, #0 
    /*00004ad0*/ BEQ         RgnCheckCapacityByModId_x4b98
    RgnCheckCapacityByModId_x4ad4:
    /*00004ad4*/ CMP         R6, #0 
    /*00004ad8*/ BEQ         RgnCheckCapacityByModId_x4b00
    /*00004adc*/ LDR         R3, [R7, #324]              @ 0x144 
    /*00004ae0*/ CMP         R3, #0 
    /*00004ae4*/ BEQ         RgnCheckCapacityByModId_x4b00
    /*00004ae8*/ MOV         R2, R6 
    /*00004aec*/ MOV         R1, R5 
    /*00004af0*/ MOV         R0, R4 
    /*00004af4*/ BLX         R3 
    /*00004af8*/ CMP         R0, #0 
    /*00004afc*/ BEQ         RgnCheckCapacityByModId_x4bbc
    RgnCheckCapacityByModId_x4b00:
    /*00004b00*/ MOV         R0, #0 
    RgnCheckCapacityByModId_x4b04:
    /*00004b04*/ ADD         SP, SP, #20 
    /*00004b08*/ POP         {R4, R5, R6, R7, PC} 
    RgnCheckCapacityByModId_x4b0c:
    /*00004b0c*/ BL          0x00000000000002BC 
    /*00004b10*/ CMP         R0, #0 
    /*00004b14*/ BEQ         RgnCheckCapacityByModId_x4a8c
    /*00004b18*/ ADD         SP, SP, #20 
    /*00004b1c*/ POP         {R4, R5, R6, R7, PC} 
    RgnCheckCapacityByModId_x4b20:
    /*00004b20*/ LDR         IP, [R5] 
    /*00004b24*/ MOV         R1, #3 
    /*00004b28*/ MOVW        R2, #0                      @ .LC118 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16c4] 
    /*00004b2c*/ MOVT        R2, #0                      @ .LC118 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16c4] 
    /*00004b30*/ MOV         R0, R1 
    /*00004b34*/ MOVW        R3, #2850                   @ 0xb22 
    /*00004b38*/ STR         R4, [SP, #8] 
    /*00004b3c*/ STR         IP, [SP, #4] 
    RgnCheckCapacityByModId_x4b40:
    /*00004b40*/ STR         R3, [SP] 
    /*00004b44*/ LDR         R3, [PC, #160]              @ 0x0000000000004bec 
    /*00004b48*/ BL          0x0000000000004B48          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004b4c*/ MOVW        R0, #32776                  @ 0x8008 
    /*00004b50*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004b54*/ B           RgnCheckCapacityByModId_x4b04
    RgnCheckCapacityByModId_x4b58:
    /*00004b58*/ BL          0x0000000000000290 
    /*00004b5c*/ CMP         R0, #0 
    /*00004b60*/ BNE         RgnCheckCapacityByModId_x4b04
    /*00004b64*/ B           RgnCheckCapacityByModId_x4a94
    RgnCheckCapacityByModId_x4b68:
    /*00004b68*/ MOV         R1, #3 
    /*00004b6c*/ STR         R3, [SP, #4] 
    /*00004b70*/ MOV         R0, R1 
    /*00004b74*/ MOVW        R3, #2843                   @ 0xb1b 
    /*00004b78*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x41c] 
    /*00004b7c*/ STR         R3, [SP] 
    /*00004b80*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x41c] 
    /*00004b84*/ LDR         R3, [PC, #96]               @ 0x0000000000004bec 
    /*00004b88*/ BL          0x0000000000004B88          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004b8c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004b90*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004b94*/ B           RgnCheckCapacityByModId_x4b04
    RgnCheckCapacityByModId_x4b98:
    /*00004b98*/ LDR         IP, [R5, #8] 
    /*00004b9c*/ MOV         R1, #3 
    /*00004ba0*/ MOVW        R2, #0                      @ .LC119 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1700] 
    /*00004ba4*/ STR         R4, [SP, #8] 
    /*00004ba8*/ MOV         R0, R1 
    /*00004bac*/ MOVW        R3, #2861                   @ 0xb2d 
    /*00004bb0*/ STR         IP, [SP, #4] 
    /*00004bb4*/ MOVT        R2, #0                      @ .LC119 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1700] 
    /*00004bb8*/ B           RgnCheckCapacityByModId_x4b40
    RgnCheckCapacityByModId_x4bbc:
    /*00004bbc*/ MOV         R1, #3 
    /*00004bc0*/ MOVW        R3, #2873                   @ 0xb39 
    /*00004bc4*/ STR         R4, [SP, #4] 
    /*00004bc8*/ MOV         R0, R1 
    /*00004bcc*/ STR         R3, [SP] 
    /*00004bd0*/ MOVW        R2, #0                      @ .LC120 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1754] 
    /*00004bd4*/ LDR         R3, [PC, #16]               @ 0x0000000000004bec 
    /*00004bd8*/ MOVT        R2, #0                      @ .LC120 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1754] 
    /*00004bdc*/ BL          0x0000000000004BDC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004be0*/ MOVW        R0, #32771                  @ 0x8003 
    /*00004be4*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004be8*/ B           RgnCheckCapacityByModId_x4b04
    /*00004bec*/ STRDEQ      R0, [R0], -R4 
FUNC_END RgnCheckCapacityByModId


FUNC_BEGIN RgnCheckCapacityByRgnChnAttr
    /*00004bf0*/ PUSH        {R4, R5, R6, LR} 
    /*00004bf4*/ MOV         R5, R1 
    /*00004bf8*/ LDR         R4, [R1, #4] 
    /*00004bfc*/ SUB         SP, SP, #8 
    /*00004c00*/ MOV         R6, R0 
    /*00004c04*/ CMP         R4, #4 
    /*00004c08*/ BHI         RgnCheckCapacityByRgnChnAttr_x4cd8
    RgnCheckCapacityByRgnChnAttr_x4c0c:
    /*00004c0c*/ CMP         R4, #1 
    /*00004c10*/ BEQ         RgnCheckCapacityByRgnChnAttr_x4c74
    /*00004c14*/ CMP         R4, #2 
    /*00004c18*/ BNE         RgnCheckCapacityByRgnChnAttr_x4ccc
    /*00004c1c*/ LDR         R0, [R5, #8] 
    /*00004c20*/ CMP         R0, #0 
    /*00004c24*/ BEQ         RgnCheckCapacityByRgnChnAttr_x4d28
    /*00004c28*/ CMP         R0, #1 
    /*00004c2c*/ BNE         RgnCheckCapacityByRgnChnAttr_x4dc8
    /*00004c30*/ LDR         R0, [R5, #12] 
    /*00004c34*/ LDR         R3, [R6, #280]              @ 0x118 
    /*00004c38*/ CMP         R0, #0 
    /*00004c3c*/ BEQ         RgnCheckCapacityByRgnChnAttr_x4d60
    /*00004c40*/ TST         R3, #1 
    /*00004c44*/ BNE         RgnCheckCapacityByRgnChnAttr_x4ccc
    /*00004c48*/ MOV         R1, #3 
    /*00004c4c*/ MOVW        R3, #2914                   @ 0xb62 
    /*00004c50*/ MOV         R0, R1 
    /*00004c54*/ STR         R3, [SP] 
    /*00004c58*/ MOVW        R2, #0                      @ .LC122 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17d0] 
    /*00004c5c*/ LDR         R3, [PC, #452]              @ 0x0000000000004e28 
    /*00004c60*/ MOVT        R2, #0                      @ .LC122 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17d0] 
    /*00004c64*/ BL          0x0000000000004C64          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004c68*/ MOVW        R0, #32776                  @ 0x8008 
    /*00004c6c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004c70*/ B           RgnCheckCapacityByRgnChnAttr_x4cd0
    RgnCheckCapacityByRgnChnAttr_x4c74:
    /*00004c74*/ LDR         R0, [R5, #8] 
    /*00004c78*/ CMP         R0, #0 
    /*00004c7c*/ BEQ         RgnCheckCapacityByRgnChnAttr_x4cf0
    /*00004c80*/ CMP         R0, #1 
    /*00004c84*/ BNE         RgnCheckCapacityByRgnChnAttr_x4df8
    /*00004c88*/ LDR         R0, [R5, #12] 
    /*00004c8c*/ LDR         R3, [R6, #280]              @ 0x118 
    /*00004c90*/ CMP         R0, #0 
    /*00004c94*/ BEQ         RgnCheckCapacityByRgnChnAttr_x4d94
    /*00004c98*/ TST         R3, #1 
    /*00004c9c*/ BNE         RgnCheckCapacityByRgnChnAttr_x4ccc
    /*00004ca0*/ MOV         R1, #3 
    /*00004ca4*/ MOVW        R3, #2963                   @ 0xb93 
    /*00004ca8*/ MOV         R0, R1 
    /*00004cac*/ STR         R3, [SP] 
    /*00004cb0*/ MOVW        R2, #0                      @ .LC126 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18d4] 
    /*00004cb4*/ LDR         R3, [PC, #364]              @ 0x0000000000004e28 
    /*00004cb8*/ MOVT        R2, #0                      @ .LC126 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18d4] 
    /*00004cbc*/ BL          0x0000000000004CBC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004cc0*/ MOVW        R0, #32776                  @ 0x8008 
    /*00004cc4*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004cc8*/ B           RgnCheckCapacityByRgnChnAttr_x4cd0
    RgnCheckCapacityByRgnChnAttr_x4ccc:
    /*00004ccc*/ MOV         R0, #0 
    RgnCheckCapacityByRgnChnAttr_x4cd0:
    /*00004cd0*/ ADD         SP, SP, #8 
    /*00004cd4*/ POP         {R4, R5, R6, PC} 
    RgnCheckCapacityByRgnChnAttr_x4cd8:
    /*00004cd8*/ MOV         R0, R4 
    /*00004cdc*/ BL          0x00000000000002BC 
    /*00004ce0*/ CMP         R0, #0 
    /*00004ce4*/ BEQ         RgnCheckCapacityByRgnChnAttr_x4c0c
    /*00004ce8*/ ADD         SP, SP, #8 
    /*00004cec*/ POP         {R4, R5, R6, PC} 
    RgnCheckCapacityByRgnChnAttr_x4cf0:
    /*00004cf0*/ LDR         R3, [R6, #276]              @ 0x114 
    /*00004cf4*/ CMP         R3, #0 
    /*00004cf8*/ BNE         RgnCheckCapacityByRgnChnAttr_x4cd0
    /*00004cfc*/ MOV         R1, #3 
    /*00004d00*/ MOVW        R3, #2949                   @ 0xb85 
    /*00004d04*/ MOV         R0, R1 
    /*00004d08*/ STR         R3, [SP] 
    /*00004d0c*/ MOVW        R2, #0                      @ .LC125 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18a0] 
    /*00004d10*/ LDR         R3, [PC, #272]              @ 0x0000000000004e28 
    /*00004d14*/ MOVT        R2, #0                      @ .LC125 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18a0] 
    /*00004d18*/ BL          0x0000000000004D18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004d1c*/ MOVW        R0, #32776                  @ 0x8008 
    /*00004d20*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004d24*/ B           RgnCheckCapacityByRgnChnAttr_x4cd0
    RgnCheckCapacityByRgnChnAttr_x4d28:
    /*00004d28*/ LDR         R3, [R6, #276]              @ 0x114 
    /*00004d2c*/ CMP         R3, #0 
    /*00004d30*/ BNE         RgnCheckCapacityByRgnChnAttr_x4cd0
    /*00004d34*/ MOV         R1, #3 
    /*00004d38*/ MOVW        R3, #2900                   @ 0xb54 
    /*00004d3c*/ MOV         R0, R1 
    /*00004d40*/ STR         R3, [SP] 
    /*00004d44*/ MOVW        R2, #0                      @ .LC121 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1798] 
    /*00004d48*/ LDR         R3, [PC, #216]              @ 0x0000000000004e28 
    /*00004d4c*/ MOVT        R2, #0                      @ .LC121 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1798] 
    /*00004d50*/ BL          0x0000000000004D50          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004d54*/ MOVW        R0, #32776                  @ 0x8008 
    /*00004d58*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004d5c*/ B           RgnCheckCapacityByRgnChnAttr_x4cd0
    RgnCheckCapacityByRgnChnAttr_x4d60:
    /*00004d60*/ TST         R3, #2 
    /*00004d64*/ BNE         RgnCheckCapacityByRgnChnAttr_x4cd0
    /*00004d68*/ MOV         R1, #3 
    /*00004d6c*/ MOVW        R3, #2926                   @ 0xb6e 
    /*00004d70*/ MOV         R0, R1 
    /*00004d74*/ STR         R3, [SP] 
    /*00004d78*/ MOVW        R2, #0                      @ .LC123 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1818] 
    /*00004d7c*/ LDR         R3, [PC, #164]              @ 0x0000000000004e28 
    /*00004d80*/ MOVT        R2, #0                      @ .LC123 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1818] 
    /*00004d84*/ BL          0x0000000000004D84          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004d88*/ MOVW        R0, #32776                  @ 0x8008 
    /*00004d8c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004d90*/ B           RgnCheckCapacityByRgnChnAttr_x4cd0
    RgnCheckCapacityByRgnChnAttr_x4d94:
    /*00004d94*/ TST         R3, #2 
    /*00004d98*/ BNE         RgnCheckCapacityByRgnChnAttr_x4cd0
    /*00004d9c*/ MOV         R1, #3 
    /*00004da0*/ MOVW        R3, #2975                   @ 0xb9f 
    /*00004da4*/ MOV         R0, R1 
    /*00004da8*/ STR         R3, [SP] 
    /*00004dac*/ MOVW        R2, #0                      @ .LC127 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x191c] 
    /*00004db0*/ LDR         R3, [PC, #112]              @ 0x0000000000004e28 
    /*00004db4*/ MOVT        R2, #0                      @ .LC127 [R_ARM_MOVT_ABS: .rodata.str1.4+0x191c] 
    /*00004db8*/ BL          0x0000000000004DB8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004dbc*/ MOVW        R0, #32776                  @ 0x8008 
    /*00004dc0*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004dc4*/ B           RgnCheckCapacityByRgnChnAttr_x4cd0
    RgnCheckCapacityByRgnChnAttr_x4dc8:
    /*00004dc8*/ MOV         R1, #3 
    /*00004dcc*/ STR         R0, [SP, #4] 
    /*00004dd0*/ MOVW        R3, #2933                   @ 0xb75 
    /*00004dd4*/ MOV         R0, R1 
    /*00004dd8*/ STR         R3, [SP] 
    /*00004ddc*/ MOVW        R2, #0                      @ .LC124 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1864] 
    /*00004de0*/ LDR         R3, [PC, #64]               @ 0x0000000000004e28 
    /*00004de4*/ MOVT        R2, #0                      @ .LC124 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1864] 
    /*00004de8*/ BL          0x0000000000004DE8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004dec*/ MOVW        R0, #32776                  @ 0x8008 
    /*00004df0*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004df4*/ B           RgnCheckCapacityByRgnChnAttr_x4cd0
    RgnCheckCapacityByRgnChnAttr_x4df8:
    /*00004df8*/ MOV         R1, #3 
    /*00004dfc*/ STR         R0, [SP, #4] 
    /*00004e00*/ MOVW        R3, #2982                   @ 0xba6 
    /*00004e04*/ MOV         R0, R1 
    /*00004e08*/ STR         R3, [SP] 
    /*00004e0c*/ MOVW        R2, #0                      @ .LC128 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1964] 
    /*00004e10*/ LDR         R3, [PC, #16]               @ 0x0000000000004e28 
    /*00004e14*/ MOVT        R2, #0                      @ .LC128 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1964] 
    /*00004e18*/ BL          0x0000000000004E18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004e1c*/ MOVW        R0, #32776                  @ 0x8008 
    /*00004e20*/ MOVT        R0, #40963                  @ 0xa003 
    /*00004e24*/ B           RgnCheckCapacityByRgnChnAttr_x4cd0
    /*00004e28*/ ANDEQ       R0, R0, IP, LSL #8 
FUNC_END RgnCheckCapacityByRgnChnAttr


FUNC_BEGIN RgnCreate
    /*00004e2c*/ CMP         R0, #127                    @ 0x7f 
    /*00004e30*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00004e34*/ MOV         R5, R0 
    /*00004e38*/ SUB         SP, SP, #16 
    /*00004e3c*/ MOV         R4, R1 
    /*00004e40*/ BHI         RgnCreate_x4f3c
    RgnCreate_x4e44:
    /*00004e44*/ CMP         R4, #0 
    /*00004e48*/ BEQ         RgnCreate_x5050
    RgnCreate_x4e4c:
    /*00004e4c*/ LDR         R0, [R4] 
    /*00004e50*/ CMP         R0, #4 
    /*00004e54*/ BHI         RgnCreate_x4f54
    RgnCreate_x4e58:
    /*00004e58*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00004e5c*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00004e60*/ MOV         R0, R6 
    /*00004e64*/ BL          0x0000000000004E64          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00004e68*/ CMP         R0, #0 
    /*00004e6c*/ BNE         RgnCreate_x5060
    /*00004e70*/ LSL         R8, R5, #2 
    /*00004e74*/ LDR         R9, [R4] 
    /*00004e78*/ ADD         R3, R6, R8 
    /*00004e7c*/ LDR         R3, [R3, #8] 
    /*00004e80*/ CMP         R3, #0 
    /*00004e84*/ BNE         RgnCreate_x5018
    /*00004e88*/ CMP         R9, #0 
    /*00004e8c*/ BNE         RgnCreate_x4f6c
    /*00004e90*/ LDR         R3, [R4, #20] 
    /*00004e94*/ SUB         R2, R3, #1 
    /*00004e98*/ CMP         R2, #5 
    /*00004e9c*/ BHI         RgnCreate_x506c
    RgnCreate_x4ea0:
    /*00004ea0*/ MOV         R1, #0 
    /*00004ea4*/ MOV         R0, #576                    @ 0x240 
    /*00004ea8*/ BL          0x0000000000004EA8          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*00004eac*/ ADD         SL, R6, R8 
    /*00004eb0*/ CMP         R0, #0 
    /*00004eb4*/ MOV         R1, R0 
    /*00004eb8*/ STR         R0, [SL, #8] 
    /*00004ebc*/ BEQ         RgnCreate_x5098
    /*00004ec0*/ MOV         R0, R4 
    /*00004ec4*/ BL          0x0000000000004EC4          @ RgnInitCtrl [R_ARM_CALL: .text+0x4864] 
    /*00004ec8*/ SUBS        R7, R0, #0 
    /*00004ecc*/ BEQ         RgnCreate_x4fc4
    /*00004ed0*/ MOV         R1, #3 
    /*00004ed4*/ MOVW        R3, #3046                   @ 0xbe6 
    /*00004ed8*/ MOVW        R2, #0                      @ .LC132 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a44] 
    /*00004edc*/ STR         R3, [SP] 
    /*00004ee0*/ MOV         R0, R1 
    /*00004ee4*/ LDR         R3, [PC, #484]              @ 0x00000000000050d0 
    /*00004ee8*/ MOVT        R2, #0                      @ .LC132 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a44] 
    /*00004eec*/ BL          0x0000000000004EEC          @ HI_LOG [R_ARM_CALL: +0x0] 
    RgnCreate_x4ef0:
    /*00004ef0*/ ADD         R6, R6, R8 
    /*00004ef4*/ LDR         R0, [R6, #8] 
    /*00004ef8*/ CMP         R0, #0 
    /*00004efc*/ BEQ         RgnCreate_x4f24
    /*00004f00*/ ADD         R0, R0, #304                @ 0x130 
    /*00004f04*/ BL          0x0000000000004F04          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00004f08*/ LDR         R0, [R6, #8] 
    /*00004f0c*/ ADD         R0, R0, #300                @ 0x12c 
    /*00004f10*/ BL          0x0000000000004F10          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00004f14*/ LDR         R0, [R6, #8] 
    /*00004f18*/ BL          0x0000000000004F18          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00004f1c*/ MOV         R3, #0 
    /*00004f20*/ STR         R3, [R6, #8] 
    RgnCreate_x4f24:
    /*00004f24*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00004f28*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00004f2c*/ BL          0x0000000000004F2C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004f30*/ MOV         R0, R7 
    /*00004f34*/ ADD         SP, SP, #16 
    /*00004f38*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    RgnCreate_x4f3c:
    /*00004f3c*/ BL          0x00000000000002FC 
    /*00004f40*/ SUBS        R7, R0, #0 
    /*00004f44*/ BEQ         RgnCreate_x4e44
    RgnCreate_x4f48:
    /*00004f48*/ MOV         R0, R7 
    /*00004f4c*/ ADD         SP, SP, #16 
    /*00004f50*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    RgnCreate_x4f54:
    /*00004f54*/ BL          0x00000000000002BC 
    /*00004f58*/ SUBS        R7, R0, #0 
    /*00004f5c*/ BEQ         RgnCreate_x4e58
    /*00004f60*/ MOV         R0, R7 
    /*00004f64*/ ADD         SP, SP, #16 
    /*00004f68*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    RgnCreate_x4f6c:
    /*00004f6c*/ CMP         R9, #3 
    /*00004f70*/ BNE         RgnCreate_x4ea0
    /*00004f74*/ LDR         R3, [R4, #20] 
    /*00004f78*/ SUB         R2, R3, #1 
    /*00004f7c*/ CMP         R2, #5 
    /*00004f80*/ BLS         RgnCreate_x4ea0
    /*00004f84*/ STR         R3, [SP, #4] 
    /*00004f88*/ MOVW        R3, #3028                   @ 0xbd4 
    /*00004f8c*/ STR         R3, [SP] 
    /*00004f90*/ MOV         R2, #6 
    /*00004f94*/ LDR         R3, [PC, #308]              @ 0x00000000000050d0 
    /*00004f98*/ MOV         R1, R9 
    /*00004f9c*/ STR         R2, [SP, #8] 
    /*00004fa0*/ MOVW        R2, #0                      @ .LC130 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19cc] 
    /*00004fa4*/ MOVT        R2, #0                      @ .LC130 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19cc] 
    /*00004fa8*/ MOV         R0, R9 
    RgnCreate_x4fac:
    /*00004fac*/ BL          0x0000000000004FAC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004fb0*/ MOV         R0, R6 
    /*00004fb4*/ MOVW        R7, #32771                  @ 0x8003 
    /*00004fb8*/ BL          0x0000000000004FB8          @ osal_up [R_ARM_CALL: +0x0] 
    /*00004fbc*/ MOVT        R7, #40963                  @ 0xa003 
    /*00004fc0*/ B           RgnCreate_x4f48
    RgnCreate_x4fc4:
    /*00004fc4*/ LDR         R1, [SL, #8] 
    /*00004fc8*/ MOV         R0, R9 
    /*00004fcc*/ ADD         R1, R1, #8 
    /*00004fd0*/ BL          0x0000000000004FD0          @ RgnCheckBaseAttr [R_ARM_CALL: .text+0x24ec] 
    /*00004fd4*/ SUBS        R7, R0, #0 
    /*00004fd8*/ BNE         RgnCreate_x4ef0
    /*00004fdc*/ MOV         R1, R5 
    /*00004fe0*/ LDR         R0, [SL, #8] 
    /*00004fe4*/ BL          0x0000000000004FE4          @ RgnAllocBmpMem [R_ARM_CALL: .text+0x2d14] 
    /*00004fe8*/ SUBS        R7, R0, #0 
    /*00004fec*/ BNE         RgnCreate_x4ef0
    /*00004ff0*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00004ff4*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00004ff8*/ ADD         R3, R3, R8 
    /*00004ffc*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005000*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005004*/ LDR         R2, [R3, #-1604]            @ 0xfffff9bc 
    /*00005008*/ ADD         R2, R2, #1 
    /*0000500c*/ STR         R2, [R3, #-1604]            @ 0xfffff9bc 
    /*00005010*/ BL          0x0000000000005010          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005014*/ B           RgnCreate_x4f48
    RgnCreate_x5018:
    /*00005018*/ MOV         R1, #3 
    /*0000501c*/ MOVW        R3, #3014                   @ 0xbc6 
    /*00005020*/ MOV         R0, R1 
    /*00005024*/ STR         R3, [SP] 
    /*00005028*/ STR         R5, [SP, #4] 
    /*0000502c*/ MOVW        R2, #0                      @ .LC129 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x199c] 
    /*00005030*/ LDR         R3, [PC, #152]              @ 0x00000000000050d0 
    /*00005034*/ MOVT        R2, #0                      @ .LC129 [R_ARM_MOVT_ABS: .rodata.str1.4+0x199c] 
    /*00005038*/ BL          0x0000000000005038          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000503c*/ MOV         R0, R6 
    /*00005040*/ MOVW        R7, #32772                  @ 0x8004 
    /*00005044*/ BL          0x0000000000005044          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005048*/ MOVT        R7, #40963                  @ 0xa003 
    /*0000504c*/ B           RgnCreate_x4f48
    RgnCreate_x5050:
    /*00005050*/ BL          0x0000000000000290 
    /*00005054*/ SUBS        R7, R0, #0 
    /*00005058*/ BNE         RgnCreate_x4f48
    /*0000505c*/ B           RgnCreate_x4e4c
    RgnCreate_x5060:
    /*00005060*/ MOV         R7, #65024                  @ 0xfe00 
    /*00005064*/ MOVT        R7, #65535                  @ 0xffff 
    /*00005068*/ B           RgnCreate_x4f48
    RgnCreate_x506c:
    /*0000506c*/ MOV         R1, #3 
    /*00005070*/ MOV         R2, #6 
    /*00005074*/ STR         R3, [SP, #4] 
    /*00005078*/ MOV         R0, R1 
    /*0000507c*/ MOVW        R3, #3021                   @ 0xbcd 
    /*00005080*/ STR         R2, [SP, #8] 
    /*00005084*/ STR         R3, [SP] 
    /*00005088*/ MOVW        R2, #0                      @ .LC130 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19cc] 
    /*0000508c*/ LDR         R3, [PC, #60]               @ 0x00000000000050d0 
    /*00005090*/ MOVT        R2, #0                      @ .LC130 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19cc] 
    /*00005094*/ B           RgnCreate_x4fac
    RgnCreate_x5098:
    /*00005098*/ MOV         R1, #3 
    /*0000509c*/ MOVW        R3, #3037                   @ 0xbdd 
    /*000050a0*/ MOV         R0, R1 
    /*000050a4*/ STR         R3, [SP] 
    /*000050a8*/ MOVW        R2, #0                      @ .LC131 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a18] 
    /*000050ac*/ LDR         R3, [PC, #28]               @ 0x00000000000050d0 
    /*000050b0*/ MOVT        R2, #0                      @ .LC131 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a18] 
    /*000050b4*/ MOVW        R7, #32780                  @ 0x800c 
    /*000050b8*/ BL          0x00000000000050B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000050bc*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000050c0*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000050c4*/ MOVT        R7, #40963                  @ 0xa003 
    /*000050c8*/ BL          0x00000000000050C8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000050cc*/ B           RgnCreate_x4f48
    /*000050d0*/ ANDEQ       R0, R0, IP, LSR #8 
FUNC_END RgnCreate


FUNC_BEGIN RgnDestroy
    /*000050d4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000050d8*/ CMP         R0, #127                    @ 0x7f 
    /*000050dc*/ SUB         SP, SP, #36                 @ 0x24 
    /*000050e0*/ STR         R0, [SP, #20] 
    /*000050e4*/ BHI         RgnDestroy_x52dc
    RgnDestroy_x50e8:
    /*000050e8*/ MOVW        R7, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000050ec*/ MOVT        R7, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000050f0*/ MOV         R0, R7 
    /*000050f4*/ BL          0x00000000000050F4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000050f8*/ SUBS        SL, R0, #0 
    /*000050fc*/ BNE         RgnDestroy_x5448
    /*00005100*/ LDR         R3, [SP, #20] 
    /*00005104*/ ADD         R2, R7, R3, LSL #2 
    /*00005108*/ LDR         FP, [R2, #8] 
    /*0000510c*/ CMP         FP, #0 
    /*00005110*/ BEQ         RgnDestroy_x5454
    /*00005114*/ LDR         R2, [FP, #208]              @ 0xd0 
    /*00005118*/ CMP         R2, #0 
    /*0000511c*/ BNE         RgnDestroy_x530c
    /*00005120*/ LDR         R8, [FP, #176]              @ 0xb0 
    /*00005124*/ CMP         R8, #4 
    /*00005128*/ BHI         RgnDestroy_x534c
    RgnDestroy_x512c:
    /*0000512c*/ LDR         R3, [FP, #228]              @ 0xe4 
    /*00005130*/ CMP         R3, #0 
    /*00005134*/ BNE         RgnDestroy_x5520
    /*00005138*/ LDR         R4, [FP, #224]              @ 0xe0 
    /*0000513c*/ CMP         R4, #0 
    /*00005140*/ BNE         RgnDestroy_x54ac
    /*00005144*/ LDR         R0, [PC, #1012]             @ 0x0000000000005540 
    /*00005148*/ ADD         R1, SP, #28 
    /*0000514c*/ BL          0x000000000000514C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00005150*/ STR         R4, [FP] 
    /*00005154*/ ADD         R1, SP, #28 
    /*00005158*/ LDR         R0, [PC, #992]              @ 0x0000000000005540 
    /*0000515c*/ BL          0x000000000000515C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00005160*/ LDR         R3, [FP, #180]              @ 0xb4 
    /*00005164*/ CMP         R3, #1 
    /*00005168*/ BEQ         RgnDestroy_x536c
    RgnDestroy_x516c:
    /*0000516c*/ ADD         R1, SP, #28 
    /*00005170*/ LDR         R0, [PC, #968]              @ 0x0000000000005540 
    /*00005174*/ BL          0x0000000000005174          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00005178*/ MOV         R3, FP 
    /*0000517c*/ LDR         R4, [R3, #308]!             @ 0x134 
    /*00005180*/ CMP         R4, R3 
    /*00005184*/ LDR         R5, [R4] 
    /*00005188*/ BEQ         RgnDestroy_x526c
    /*0000518c*/ MOVW        R2, #0                      @ .LC137 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b68] 
    /*00005190*/ MOVW        R1, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00005194*/ MOVT        R2, #0                      @ .LC137 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b68] 
    /*00005198*/ MOVT        R1, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*0000519c*/ STR         FP, [SP, #12] 
    /*000051a0*/ MOV         FP, R3 
    /*000051a4*/ STR         R2, [SP, #16] 
    /*000051a8*/ STR         R1, [SP, #8] 
    /*000051ac*/ B           RgnDestroy_x51f8
    RgnDestroy_x51b0:
    /*000051b0*/ MOV         R3, #1 
    /*000051b4*/ STR         R3, [R9, #4] 
    /*000051b8*/ LDR         R0, [R4] 
    /*000051bc*/ MOV         R2, #256                    @ 0x100 
    /*000051c0*/ LDR         R1, [R4, #4] 
    /*000051c4*/ MOVT        R2, #16 
    /*000051c8*/ MOV         R3, #512                    @ 0x200 
    /*000051cc*/ MOVT        R3, #32 
    /*000051d0*/ STR         R1, [R0, #4] 
    /*000051d4*/ STR         R0, [R1] 
    /*000051d8*/ MOV         R0, R4 
    /*000051dc*/ STM         R4, {R2, R3} 
    /*000051e0*/ BL          0x00000000000051E0          @ osal_kfree [R_ARM_CALL: +0x0] 
    RgnDestroy_x51e4:
    /*000051e4*/ CMP         R5, FP 
    /*000051e8*/ LDR         R3, [R5] 
    /*000051ec*/ MOV         R4, R5 
    /*000051f0*/ BEQ         RgnDestroy_x5268
    /*000051f4*/ MOV         R5, R3 
    RgnDestroy_x51f8:
    /*000051f8*/ ADD         R6, R4, #8 
    /*000051fc*/ MOV         R0, R8 
    /*00005200*/ MOV         R1, R6 
    /*00005204*/ BL          0x0000000000005204          @ RgnGetChnGetInfo [R_ARM_CALL: .text+0x1034] 
    /*00005208*/ SUBS        R9, R0, #0 
    /*0000520c*/ BEQ         RgnDestroy_x52f4
    RgnDestroy_x5210:
    /*00005210*/ LDR         R2, [SP, #20] 
    /*00005214*/ MOV         R1, R6 
    /*00005218*/ ADD         R3, R7, R2, LSL #2 
    /*0000521c*/ LDR         R3, [R3, #8] 
    /*00005220*/ LDR         R0, [R3, #176]              @ 0xb0 
    /*00005224*/ BL          0x0000000000005224          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*00005228*/ MOV         R1, R6 
    /*0000522c*/ SUBS        R3, R0, #0 
    /*00005230*/ MOV         R0, R8 
    /*00005234*/ BEQ         RgnDestroy_x51e4
    /*00005238*/ LDR         R3, [R3, #152]              @ 0x98 
    /*0000523c*/ LDR         R2, [SP, #20] 
    /*00005240*/ BL          0x0000000000005240          @ RgnDelRgnFrmChn [R_ARM_CALL: .text+0x4058] 
    /*00005244*/ SUBS        SL, R0, #0 
    /*00005248*/ BEQ         RgnDestroy_x51b0
    /*0000524c*/ MOVW        R3, #0                      @ .LC138 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b84] 
    /*00005250*/ MOVW        R2, #3215                   @ 0xc8f 
    /*00005254*/ MOVT        R3, #0                      @ .LC138 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b84] 
    /*00005258*/ LDR         R1, [PC, #740]              @ 0x0000000000005544 
    /*0000525c*/ LDR         R0, [SP, #8] 
    /*00005260*/ BL          0x0000000000005260          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005264*/ B           RgnDestroy_x51b0
    RgnDestroy_x5268:
    /*00005268*/ LDR         FP, [SP, #12] 
    RgnDestroy_x526c:
    /*0000526c*/ LDR         R3, [SP, #20] 
    /*00005270*/ LDRD        R4, [FP, #248]              @ 0xf8 
    /*00005274*/ LDR         R8, [FP, #256]              @ 0x100 
    /*00005278*/ ADD         R3, R7, R3, LSL #2 
    /*0000527c*/ LDR         R6, [R3, #8] 
    /*00005280*/ CMP         R6, #0 
    /*00005284*/ BEQ         RgnDestroy_x548c
    RgnDestroy_x5288:
    /*00005288*/ ADD         R0, R6, #304                @ 0x130 
    /*0000528c*/ BL          0x000000000000528C          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00005290*/ ADD         R0, R6, #300                @ 0x12c 
    /*00005294*/ BL          0x0000000000005294          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00005298*/ LDR         R3, [SP, #20] 
    /*0000529c*/ ADD         R1, SP, #28 
    /*000052a0*/ LDR         R0, [PC, #664]              @ 0x0000000000005540 
    /*000052a4*/ ADD         R7, R7, R3, LSL #2 
    /*000052a8*/ MOV         R3, #0 
    /*000052ac*/ STR         R3, [R7, #8] 
    /*000052b0*/ BL          0x00000000000052B0          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000052b4*/ MOV         R0, R6 
    /*000052b8*/ BL          0x00000000000052B8          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*000052bc*/ ORRS        R3, R4, R5 
    /*000052c0*/ BNE         RgnDestroy_x5434
    RgnDestroy_x52c4:
    /*000052c4*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000052c8*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000052cc*/ BL          0x00000000000052CC          @ osal_up [R_ARM_CALL: +0x0] 
    RgnDestroy_x52d0:
    /*000052d0*/ MOV         R0, SL 
    /*000052d4*/ ADD         SP, SP, #36                 @ 0x24 
    /*000052d8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnDestroy_x52dc:
    /*000052dc*/ BL          0x00000000000002FC 
    /*000052e0*/ SUBS        SL, R0, #0 
    /*000052e4*/ BEQ         RgnDestroy_x50e8
    /*000052e8*/ MOV         R0, SL 
    /*000052ec*/ ADD         SP, SP, #36                 @ 0x24 
    /*000052f0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnDestroy_x52f4:
    /*000052f4*/ LDR         R3, [SP, #16] 
    /*000052f8*/ MOVW        R2, #3206                   @ 0xc86 
    /*000052fc*/ LDR         R1, [PC, #576]              @ 0x0000000000005544 
    /*00005300*/ LDR         R0, [SP, #8] 
    /*00005304*/ BL          0x0000000000005304          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005308*/ B           RgnDestroy_x5210
    RgnDestroy_x530c:
    /*0000530c*/ MOV         R1, #3 
    /*00005310*/ STR         R3, [SP, #4] 
    /*00005314*/ MOV         R0, R1 
    /*00005318*/ MOVW        R3, #3141                   @ 0xc45 
    /*0000531c*/ MOVW        R2, #0                      @ .LC140 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1bb4] 
    /*00005320*/ STR         R3, [SP] 
    /*00005324*/ MOVT        R2, #0                      @ .LC140 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1bb4] 
    /*00005328*/ LDR         R3, [PC, #532]              @ 0x0000000000005544 
    /*0000532c*/ BL          0x000000000000532C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005330*/ MOV         R0, R7 
    /*00005334*/ BL          0x0000000000005334          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005338*/ MOVW        SL, #32777                  @ 0x8009 
    /*0000533c*/ MOVT        SL, #40963                  @ 0xa003 
    /*00005340*/ MOV         R0, SL 
    /*00005344*/ ADD         SP, SP, #36                 @ 0x24 
    /*00005348*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnDestroy_x534c:
    /*0000534c*/ MOVW        R3, #0                      @ .LC133 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a74] 
    /*00005350*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00005354*/ MOVT        R3, #0                      @ .LC133 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a74] 
    /*00005358*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*0000535c*/ MOVW        R2, #3148                   @ 0xc4c 
    /*00005360*/ LDR         R1, [PC, #476]              @ 0x0000000000005544 
    /*00005364*/ BL          0x0000000000005364          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005368*/ B           RgnDestroy_x512c
    RgnDestroy_x536c:
    /*0000536c*/ ADD         R4, SP, #32 
    /*00005370*/ MOVW        R9, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00005374*/ MOVT        R9, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00005378*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000537c*/ LDR         R3, [R4, #-12]! 
    /*00005380*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005384*/ MOVW        R5, #0                      @ Rgn_WaitConditionCallBackEx [R_ARM_MOVW_ABS_NC: .text+0x134] 
    /*00005388*/ ADD         R6, FP, #300                @ 0x12c 
    /*0000538c*/ MOVT        R5, #0                      @ Rgn_WaitConditionCallBackEx [R_ARM_MOVT_ABS: .text+0x134] 
    /*00005390*/ ADD         R3, R9, R3, LSL #2 
    /*00005394*/ LDR         R3, [R3, #-1604]            @ 0xfffff9bc 
    /*00005398*/ STR         R3, [SP, #8] 
    /*0000539c*/ BL          0x000000000000539C          @ osal_up [R_ARM_CALL: +0x0] 
    /*000053a0*/ B           RgnDestroy_x53b0
    RgnDestroy_x53a4:
    /*000053a4*/ BL          0x00000000000053A4          @ osal_wait_uninterruptible [R_ARM_CALL: +0x0] 
    /*000053a8*/ CMP         R0, #0 
    /*000053ac*/ BLT         RgnDestroy_x5448
    RgnDestroy_x53b0:
    /*000053b0*/ MOV         R0, R4 
    /*000053b4*/ BL          0x00000000000053B4          @ Rgn_WaitConditionCallBackEx [R_ARM_CALL: .text+0x134] 
    /*000053b8*/ MOV         R2, R4 
    /*000053bc*/ MOV         R1, R5 
    /*000053c0*/ CMP         R0, #0 
    /*000053c4*/ MOV         R0, R6 
    /*000053c8*/ BEQ         RgnDestroy_x53a4
    /*000053cc*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000053d0*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000053d4*/ BL          0x00000000000053D4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000053d8*/ CMP         R0, #0 
    /*000053dc*/ BNE         RgnDestroy_x5448
    /*000053e0*/ LDR         R0, [SP, #20] 
    /*000053e4*/ LDR         R1, [SP, #8] 
    /*000053e8*/ LSL         R3, R0, #2 
    /*000053ec*/ ADD         R9, R9, R3 
    /*000053f0*/ LDR         R2, [R9, #-1604]            @ 0xfffff9bc 
    /*000053f4*/ CMP         R1, R2 
    /*000053f8*/ BNE         RgnDestroy_x54e4
    /*000053fc*/ MOVW        R2, #0                      @ s_u32RgnState [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00005400*/ MOVT        R2, #0                      @ s_u32RgnState [R_ARM_MOVT_ABS: .data+0x0] 
    /*00005404*/ LDR         R2, [R2] 
    /*00005408*/ CMP         R2, #0 
    /*0000540c*/ BNE         RgnDestroy_x54e4
    /*00005410*/ ADD         R3, R7, R3 
    /*00005414*/ LDR         R3, [R3, #8] 
    /*00005418*/ CMP         R3, #0 
    /*0000541c*/ BNE         RgnDestroy_x516c
    /*00005420*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005424*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005428*/ MOV         SL, R3 
    /*0000542c*/ BL          0x000000000000542C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005430*/ B           RgnDestroy_x52d0
    RgnDestroy_x5434:
    /*00005434*/ MOV         R2, R8 
    /*00005438*/ MOV         R0, R4 
    /*0000543c*/ MOV         R1, R5 
    /*00005440*/ BL          0x0000000000005440          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*00005444*/ B           RgnDestroy_x52c4
    RgnDestroy_x5448:
    /*00005448*/ MOV         SL, #65024                  @ 0xfe00 
    /*0000544c*/ MOVT        SL, #65535                  @ 0xffff 
    /*00005450*/ B           RgnDestroy_x52d0
    RgnDestroy_x5454:
    /*00005454*/ MOV         R1, #3 
    /*00005458*/ STR         R3, [SP, #4] 
    /*0000545c*/ MOV         R0, R1 
    /*00005460*/ MOVW        R3, #3135                   @ 0xc3f 
    /*00005464*/ MOVW        R2, #0                      @ .LC90 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x120c] 
    /*00005468*/ STR         R3, [SP] 
    /*0000546c*/ MOVT        R2, #0                      @ .LC90 [R_ARM_MOVT_ABS: .rodata.str1.4+0x120c] 
    /*00005470*/ LDR         R3, [PC, #204]              @ 0x0000000000005544 
    /*00005474*/ BL          0x0000000000005474          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005478*/ MOV         R0, R7 
    /*0000547c*/ MOVW        SL, #32773                  @ 0x8005 
    /*00005480*/ BL          0x0000000000005480          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005484*/ MOVT        SL, #40963                  @ 0xa003 
    /*00005488*/ B           RgnDestroy_x52d0
    RgnDestroy_x548c:
    /*0000548c*/ MOVW        R3, #0                      @ .LC139 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b9c] 
    /*00005490*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00005494*/ MOVT        R3, #0                      @ .LC139 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b9c] 
    /*00005498*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*0000549c*/ MOVW        R2, #3233                   @ 0xca1 
    /*000054a0*/ LDR         R1, [PC, #156]              @ 0x0000000000005544 
    /*000054a4*/ BL          0x00000000000054A4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000054a8*/ B           RgnDestroy_x5288
    RgnDestroy_x54ac:
    /*000054ac*/ MOVW        R3, #3161                   @ 0xc59 
    /*000054b0*/ STR         R3, [SP] 
    /*000054b4*/ LDR         R3, [PC, #136]              @ 0x0000000000005544 
    /*000054b8*/ MOV         R1, #3 
    /*000054bc*/ MOVW        R2, #0                      @ .LC135 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1aec] 
    /*000054c0*/ MOVT        R2, #0                      @ .LC135 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1aec] 
    /*000054c4*/ MOV         R0, R1 
    RgnDestroy_x54c8:
    /*000054c8*/ BL          0x00000000000054C8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000054cc*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000054d0*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000054d4*/ MOVW        SL, #32777                  @ 0x8009 
    /*000054d8*/ MOVT        SL, #40963                  @ 0xa003 
    /*000054dc*/ BL          0x00000000000054DC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000054e0*/ B           RgnDestroy_x52d0
    RgnDestroy_x54e4:
    /*000054e4*/ MOV         R1, #3 
    /*000054e8*/ STR         R0, [SP, #4] 
    /*000054ec*/ MOVW        R3, #3185                   @ 0xc71 
    /*000054f0*/ MOV         R0, R1 
    /*000054f4*/ STR         R3, [SP] 
    /*000054f8*/ MOVW        R2, #0                      @ .LC136 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b2c] 
    /*000054fc*/ LDR         R3, [PC, #64]               @ 0x0000000000005544 
    /*00005500*/ MOVT        R2, #0                      @ .LC136 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b2c] 
    /*00005504*/ BL          0x0000000000005504          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005508*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000550c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005510*/ MOVW        SL, #32784                  @ 0x8010 
    /*00005514*/ BL          0x0000000000005514          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005518*/ MOVT        SL, #40963                  @ 0xa003 
    /*0000551c*/ B           RgnDestroy_x52d0
    RgnDestroy_x5520:
    /*00005520*/ MOV         R1, #3 
    /*00005524*/ MOVW        R3, #3154                   @ 0xc52 
    /*00005528*/ MOVW        R2, #0                      @ .LC134 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a9c] 
    /*0000552c*/ STR         R3, [SP] 
    /*00005530*/ MOV         R0, R1 
    /*00005534*/ LDR         R3, [PC, #8]                @ 0x0000000000005544 
    /*00005538*/ MOVT        R2, #0                      @ .LC134 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a9c] 
    /*0000553c*/ B           RgnDestroy_x54c8
    /*00005540*/ ANDEQ       R0, R0, R4 
    /*00005544*/ ANDEQ       R0, R0, R8, LSR R4 
FUNC_END RgnDestroy


FUNC_BEGIN RgnGetAttr
    /*00005548*/ CMP         R0, #127                    @ 0x7f 
    /*0000554c*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00005550*/ MOV         R5, R0 
    /*00005554*/ SUB         SP, SP, #20 
    /*00005558*/ MOV         R6, R1 
    /*0000555c*/ BHI         RgnGetAttr_x55d8
    RgnGetAttr_x5560:
    /*00005560*/ CMP         R6, #0 
    /*00005564*/ BEQ         RgnGetAttr_x5648
    RgnGetAttr_x5568:
    /*00005568*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000556c*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005570*/ MOV         R0, R4 
    /*00005574*/ BL          0x0000000000005574          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00005578*/ CMP         R0, #0 
    /*0000557c*/ BNE         RgnGetAttr_x5658
    /*00005580*/ ADD         R3, R4, R5, LSL #2 
    /*00005584*/ LDR         R7, [R3, #8] 
    /*00005588*/ CMP         R7, #0 
    /*0000558c*/ BEQ         RgnGetAttr_x5610
    /*00005590*/ LDR         R4, [R7, #176]              @ 0xb0 
    /*00005594*/ CMP         R4, #4 
    /*00005598*/ BHI         RgnGetAttr_x55f0
    RgnGetAttr_x559c:
    /*0000559c*/ SUB         R3, R4, #1 
    /*000055a0*/ CMP         R4, #4 
    /*000055a4*/ CMPNE       R3, #1 
    /*000055a8*/ BLS         RgnGetAttr_x5664
    /*000055ac*/ MOV         R0, R4 
    /*000055b0*/ MOV         R2, R6 
    /*000055b4*/ ADD         R1, R7, #8 
    /*000055b8*/ BL          0x00000000000055B8          @ RgnCommToAttr [R_ARM_CALL: .text+0x584] 
    /*000055bc*/ SUBS        R4, R0, #0 
    /*000055c0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000055c4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000055c8*/ BL          0x00000000000055C8          @ osal_up [R_ARM_CALL: +0x0] 
    RgnGetAttr_x55cc:
    /*000055cc*/ MOV         R0, R4 
    /*000055d0*/ ADD         SP, SP, #20 
    /*000055d4*/ POP         {R4, R5, R6, R7, PC} 
    RgnGetAttr_x55d8:
    /*000055d8*/ BL          0x00000000000002FC 
    /*000055dc*/ SUBS        R4, R0, #0 
    /*000055e0*/ BEQ         RgnGetAttr_x5560
    /*000055e4*/ MOV         R0, R4 
    /*000055e8*/ ADD         SP, SP, #20 
    /*000055ec*/ POP         {R4, R5, R6, R7, PC} 
    RgnGetAttr_x55f0:
    /*000055f0*/ MOVW        R3, #0                      @ .LC133 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a74] 
    /*000055f4*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000055f8*/ MOVT        R3, #0                      @ .LC133 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a74] 
    /*000055fc*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00005600*/ MOVW        R2, #3276                   @ 0xccc 
    /*00005604*/ LDR         R1, [PC, #152]              @ 0x00000000000056a4 
    /*00005608*/ BL          0x0000000000005608          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000560c*/ B           RgnGetAttr_x559c
    RgnGetAttr_x5610:
    /*00005610*/ MOV         R1, #3 
    /*00005614*/ MOVW        R3, #3267                   @ 0xcc3 
    /*00005618*/ MOV         R0, R1 
    /*0000561c*/ STR         R3, [SP] 
    /*00005620*/ STR         R5, [SP, #4] 
    /*00005624*/ MOVW        R2, #0                      @ .LC90 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x120c] 
    /*00005628*/ LDR         R3, [PC, #116]              @ 0x00000000000056a4 
    /*0000562c*/ MOVT        R2, #0                      @ .LC90 [R_ARM_MOVT_ABS: .rodata.str1.4+0x120c] 
    /*00005630*/ BL          0x0000000000005630          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005634*/ MOV         R0, R4 
    /*00005638*/ BL          0x0000000000005638          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000563c*/ MOVW        R4, #32773                  @ 0x8005 
    /*00005640*/ MOVT        R4, #40963                  @ 0xa003 
    /*00005644*/ B           RgnGetAttr_x55cc
    RgnGetAttr_x5648:
    /*00005648*/ BL          0x0000000000000290 
    /*0000564c*/ SUBS        R4, R0, #0 
    /*00005650*/ BNE         RgnGetAttr_x55cc
    /*00005654*/ B           RgnGetAttr_x5568
    RgnGetAttr_x5658:
    /*00005658*/ MOV         R4, #65024                  @ 0xfe00 
    /*0000565c*/ MOVT        R4, #65535                  @ 0xffff 
    /*00005660*/ B           RgnGetAttr_x55cc
    RgnGetAttr_x5664:
    /*00005664*/ MOV         R1, #3 
    /*00005668*/ STR         R4, [SP, #8] 
    /*0000566c*/ STR         R5, [SP, #4] 
    /*00005670*/ MOV         R0, R1 
    /*00005674*/ MOVW        R3, #3281                   @ 0xcd1 
    /*00005678*/ MOVW        R2, #0                      @ .LC141 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c04] 
    /*0000567c*/ STR         R3, [SP] 
    /*00005680*/ MOVT        R2, #0                      @ .LC141 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c04] 
    /*00005684*/ LDR         R3, [PC, #24]               @ 0x00000000000056a4 
    /*00005688*/ MOVW        R4, #32776                  @ 0x8008 
    /*0000568c*/ BL          0x000000000000568C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005690*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005694*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005698*/ MOVT        R4, #40963                  @ 0xa003 
    /*0000569c*/ BL          0x000000000000569C          @ osal_up [R_ARM_CALL: +0x0] 
    /*000056a0*/ B           RgnGetAttr_x55cc
    /*000056a4*/ ANDEQ       R0, R0, R4, ASR #8 
FUNC_END RgnGetAttr


FUNC_BEGIN RgnSetAttr
    /*000056a8*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000056ac*/ CMP         R0, #127                    @ 0x7f 
    /*000056b0*/ SUB         SP, SP, #372                @ 0x174 
    /*000056b4*/ MOV         R8, R1 
    /*000056b8*/ MOV         R7, R2 
    /*000056bc*/ STR         R0, [SP, #28] 
    /*000056c0*/ BHI         RgnSetAttr_x5754
    RgnSetAttr_x56c4:
    /*000056c4*/ CMP         R8, #0 
    /*000056c8*/ BEQ         RgnSetAttr_x5a00
    RgnSetAttr_x56cc:
    /*000056cc*/ LDR         R4, [R8] 
    /*000056d0*/ CMP         R4, #4 
    /*000056d4*/ BHI         RgnSetAttr_x576c
    RgnSetAttr_x56d8:
    /*000056d8*/ MOVW        R9, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000056dc*/ MOVT        R9, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000056e0*/ MOV         R0, R9 
    /*000056e4*/ BL          0x00000000000056E4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000056e8*/ CMP         R0, #0 
    /*000056ec*/ BNE         RgnSetAttr_x5ad4
    /*000056f0*/ LDR         R3, [SP, #28] 
    /*000056f4*/ ADD         R2, R9, R3, LSL #2 
    /*000056f8*/ LDR         R5, [R2, #8] 
    /*000056fc*/ CMP         R5, #0 
    /*00005700*/ BEQ         RgnSetAttr_x5f4c
    /*00005704*/ LDR         R2, [R5] 
    /*00005708*/ CMP         R2, #0 
    /*0000570c*/ BEQ         RgnSetAttr_x5da4
    /*00005710*/ LDR         R2, [R5, #208]              @ 0xd0 
    /*00005714*/ CMP         R2, #0 
    /*00005718*/ BEQ         RgnSetAttr_x5784
    /*0000571c*/ MOV         R1, #3 
    /*00005720*/ STR         R3, [SP, #4] 
    /*00005724*/ MOV         R0, R1 
    /*00005728*/ MOVW        R3, #3381                   @ 0xd35 
    /*0000572c*/ MOVW        R2, #0                      @ .LC148 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1dec] 
    /*00005730*/ STR         R3, [SP] 
    /*00005734*/ MOVT        R2, #0                      @ .LC148 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1dec] 
    /*00005738*/ LDR         R3, [PC, #2232]             @ 0x0000000000005ff8 
    /*0000573c*/ BL          0x000000000000573C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005740*/ MOV         R0, R9 
    /*00005744*/ MOVW        R4, #32777                  @ 0x8009 
    /*00005748*/ BL          0x0000000000005748          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000574c*/ MOVT        R4, #40963                  @ 0xa003 
    /*00005750*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5754:
    /*00005754*/ BL          0x00000000000002FC 
    /*00005758*/ SUBS        R4, R0, #0 
    /*0000575c*/ BEQ         RgnSetAttr_x56c4
    RgnSetAttr_x5760:
    /*00005760*/ MOV         R0, R4 
    /*00005764*/ ADD         SP, SP, #372                @ 0x174 
    /*00005768*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnSetAttr_x576c:
    /*0000576c*/ MOV         R0, R4 
    /*00005770*/ BL          0x00000000000002BC 
    /*00005774*/ SUBS        R4, R0, #0 
    /*00005778*/ BNE         RgnSetAttr_x5760
    /*0000577c*/ LDR         R4, [R8] 
    /*00005780*/ B           RgnSetAttr_x56d8
    RgnSetAttr_x5784:
    /*00005784*/ SUB         R2, R4, #1 
    /*00005788*/ CMP         R4, #4 
    /*0000578c*/ CMPNE       R2, #1 
    /*00005790*/ BLS         RgnSetAttr_x5ddc
    /*00005794*/ LDR         R6, [R5, #176]              @ 0xb0 
    /*00005798*/ CMP         R6, R4 
    /*0000579c*/ BNE         RgnSetAttr_x5b14
    /*000057a0*/ LDR         R1, [R8] 
    /*000057a4*/ CMP         R1, #0 
    /*000057a8*/ BNE         RgnSetAttr_x59ac
    /*000057ac*/ LDR         R2, [R8, #20] 
    /*000057b0*/ SUB         R1, R2, #1 
    /*000057b4*/ CMP         R1, #5 
    /*000057b8*/ BHI         RgnSetAttr_x5e58
    RgnSetAttr_x57bc:
    /*000057bc*/ LDR         R2, [R5, #228]              @ 0xe4 
    /*000057c0*/ CMP         R2, #1 
    /*000057c4*/ BEQ         RgnSetAttr_x5e14
    /*000057c8*/ ADD         SL, R5, #8 
    /*000057cc*/ MOV         R2, #168                    @ 0xa8 
    /*000057d0*/ ADD         R0, SP, #32 
    /*000057d4*/ MOV         R1, SL 
    /*000057d8*/ BL          0x00000000000057D8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000057dc*/ ADD         R1, SP, #32 
    /*000057e0*/ MOV         R0, R8 
    /*000057e4*/ BL          0x00000000000057E4          @ RgnAttrToComm [R_ARM_CALL: .text+0x47c] 
    /*000057e8*/ SUBS        R4, R0, #0 
    /*000057ec*/ BNE         RgnSetAttr_x5994
    /*000057f0*/ ADD         R1, SP, #32 
    /*000057f4*/ MOV         R0, R6 
    /*000057f8*/ BL          0x00000000000057F8          @ RgnCheckBaseAttr [R_ARM_CALL: .text+0x24ec] 
    /*000057fc*/ SUBS        R4, R0, #0 
    /*00005800*/ BNE         RgnSetAttr_x5994
    /*00005804*/ LDR         R0, [SP, #28] 
    /*00005808*/ LDR         IP, [SP, #144]              @ 0x90 
    /*0000580c*/ LSL         R2, R0, #2 
    /*00005810*/ ADD         R3, R9, R2 
    /*00005814*/ LDR         R3, [R3, #8] 
    /*00005818*/ LDR         R1, [R3, #120]              @ 0x78 
    /*0000581c*/ CMP         IP, R1 
    /*00005820*/ BEQ         RgnSetAttr_x5a90
    RgnSetAttr_x5824:
    /*00005824*/ LDR         R1, [R3, #308]              @ 0x134 
    /*00005828*/ ADD         R3, R3, #308                @ 0x134 
    /*0000582c*/ CMP         R1, R3 
    /*00005830*/ BEQ         RgnSetAttr_x5a10
    /*00005834*/ ADD         R1, SP, #32 
    /*00005838*/ BL          0x0000000000005838          @ RgnCheckSetAttr [R_ARM_CALL: .text+0x4570] 
    /*0000583c*/ SUBS        R4, R0, #0 
    /*00005840*/ BNE         RgnSetAttr_x5994
    /*00005844*/ LDR         R1, [SP, #100]              @ 0x64 
    /*00005848*/ LDR         R3, [R5, #76]               @ 0x4c 
    /*0000584c*/ CMP         R3, R1 
    /*00005850*/ BEQ         RgnSetAttr_x5894
    /*00005854*/ LDR         R3, [R5, #116]              @ 0x74 
    /*00005858*/ CMP         R3, #1 
    /*0000585c*/ BLS         RgnSetAttr_x5874
    /*00005860*/ MOV         R0, R5 
    /*00005864*/ BL          0x00000000000000B8 
    /*00005868*/ CMN         R0, #1 
    /*0000586c*/ BEQ         RgnSetAttr_x5b94
    RgnSetAttr_x5870:
    /*00005870*/ LDR         R1, [SP, #100]              @ 0x64 
    RgnSetAttr_x5874:
    /*00005874*/ MOV         R0, R5 
    /*00005878*/ BL          0x0000000000005878          @ RgnUpdBgColor [R_ARM_CALL: .text+0x4448] 
    /*0000587c*/ SUBS        R3, R0, #0 
    /*00005880*/ BNE         RgnSetAttr_x5b80
    /*00005884*/ LDRD        R2, [R5, #96]               @ 0x60 
    /*00005888*/ LDRD        R0, [R5, #104]              @ 0x68 
    /*0000588c*/ STRD        R2, [SP, #120]              @ 0x78 
    /*00005890*/ STRD        R0, [SP, #128]              @ 0x80 
    RgnSetAttr_x5894:
    /*00005894*/ ADD         R1, SP, #200                @ 0xc8 
    /*00005898*/ LDR         R0, [PC, #1884]             @ 0x0000000000005ffc 
    /*0000589c*/ BL          0x000000000000589C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000058a0*/ MOV         R3, R5 
    /*000058a4*/ LDR         R9, [R3, #308]!             @ 0x134 
    /*000058a8*/ CMP         R3, R9 
    /*000058ac*/ MOV         R1, R3 
    /*000058b0*/ LDR         R7, [R9] 
    /*000058b4*/ BEQ         RgnSetAttr_x5970
    /*000058b8*/ MOVW        R3, #0                      @ .LC137 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b68] 
    /*000058bc*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000058c0*/ MOVT        R3, #0                      @ .LC137 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b68] 
    /*000058c4*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000058c8*/ MOV         R8, R9 
    /*000058cc*/ STR         R3, [SP, #24] 
    /*000058d0*/ STR         R2, [SP, #16] 
    /*000058d4*/ MOV         R9, R1 
    /*000058d8*/ STR         R5, [SP, #20] 
    /*000058dc*/ B           RgnSetAttr_x58e4
    RgnSetAttr_x58e0:
    /*000058e0*/ MOV         R7, R3 
    RgnSetAttr_x58e4:
    /*000058e4*/ ADD         R5, R8, #8 
    /*000058e8*/ LDR         R2, [SP, #28] 
    /*000058ec*/ MOV         R0, R6 
    /*000058f0*/ MOV         R1, R5 
    /*000058f4*/ BL          0x00000000000058F4          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*000058f8*/ SUBS        FP, R0, #0 
    /*000058fc*/ MOV         R0, R6 
    /*00005900*/ BEQ         RgnSetAttr_x595c
    /*00005904*/ LDR         R1, [R8, #8] 
    /*00005908*/ BL          0x0000000000005908          @ RgnGetCapacity [R_ARM_CALL: .text+0x3668] 
    /*0000590c*/ SUBS        R8, R0, #0 
    /*00005910*/ BEQ         RgnSetAttr_x5af8
    RgnSetAttr_x5914:
    /*00005914*/ MOV         R3, R6 
    /*00005918*/ MOV         R2, FP 
    /*0000591c*/ MOV         R1, #1 
    /*00005920*/ MOV         R0, R8 
    /*00005924*/ STR         R1, [SP] 
    /*00005928*/ ADD         R1, SP, #32 
    /*0000592c*/ BL          0x000000000000592C          @ RgnUpChnAttr [R_ARM_CALL: .text+0x4230] 
    /*00005930*/ MOV         R1, R5 
    /*00005934*/ MOV         R2, R8 
    /*00005938*/ MOV         R0, R6 
    /*0000593c*/ BL          0x000000000000593C          @ RgnChnRgnsSort [R_ARM_CALL: .text+0x3e0c] 
    /*00005940*/ MOV         R1, R5 
    /*00005944*/ MOV         R0, R6 
    /*00005948*/ BL          0x0000000000005948          @ RgnGetChnGetInfo [R_ARM_CALL: .text+0x1034] 
    /*0000594c*/ SUBS        R5, R0, #0 
    /*00005950*/ BEQ         RgnSetAttr_x5ae0
    RgnSetAttr_x5954:
    /*00005954*/ MOV         R3, #1 
    /*00005958*/ STR         R3, [R5, #4] 
    RgnSetAttr_x595c:
    /*0000595c*/ CMP         R9, R7 
    /*00005960*/ LDR         R3, [R7] 
    /*00005964*/ MOV         R8, R7 
    /*00005968*/ BNE         RgnSetAttr_x58e0
    /*0000596c*/ LDR         R5, [SP, #20] 
    RgnSetAttr_x5970:
    /*00005970*/ LDR         R3, [R5, #212]              @ 0xd4 
    /*00005974*/ ADD         R1, SP, #200                @ 0xc8 
    /*00005978*/ LDR         R0, [PC, #1660]             @ 0x0000000000005ffc 
    /*0000597c*/ STR         R3, [R5, #216]              @ 0xd8 
    /*00005980*/ BL          0x0000000000005980          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00005984*/ ADD         R1, SP, #32 
    /*00005988*/ MOV         R0, SL 
    /*0000598c*/ MOV         R2, #168                    @ 0xa8 
    /*00005990*/ BL          0x0000000000005990          @ osal_memcpy [R_ARM_CALL: +0x0] 
    RgnSetAttr_x5994:
    /*00005994*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005998*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000599c*/ BL          0x000000000000599C          @ osal_up [R_ARM_CALL: +0x0] 
    /*000059a0*/ MOV         R0, R4 
    /*000059a4*/ ADD         SP, SP, #372                @ 0x174 
    /*000059a8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnSetAttr_x59ac:
    /*000059ac*/ CMP         R1, #3 
    /*000059b0*/ BNE         RgnSetAttr_x57bc
    /*000059b4*/ LDR         R2, [R8, #20] 
    /*000059b8*/ SUB         R0, R2, #1 
    /*000059bc*/ CMP         R0, #5 
    /*000059c0*/ BLS         RgnSetAttr_x57bc
    /*000059c4*/ MOVW        R3, #3410                   @ 0xd52 
    /*000059c8*/ STR         R3, [SP] 
    /*000059cc*/ LDR         R3, [PC, #1572]             @ 0x0000000000005ff8 
    /*000059d0*/ MOV         R0, R1 
    /*000059d4*/ STR         R2, [SP, #4] 
    /*000059d8*/ MOV         R2, #6 
    /*000059dc*/ STR         R2, [SP, #8] 
    /*000059e0*/ MOVW        R2, #0                      @ .LC130 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19cc] 
    /*000059e4*/ MOVT        R2, #0                      @ .LC130 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19cc] 
    RgnSetAttr_x59e8:
    /*000059e8*/ BL          0x00000000000059E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000059ec*/ MOV         R0, R9 
    /*000059f0*/ MOVW        R4, #32771                  @ 0x8003 
    /*000059f4*/ BL          0x00000000000059F4          @ osal_up [R_ARM_CALL: +0x0] 
    /*000059f8*/ MOVT        R4, #40963                  @ 0xa003 
    /*000059fc*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5a00:
    /*00005a00*/ BL          0x0000000000000290 
    /*00005a04*/ SUBS        R4, R0, #0 
    /*00005a08*/ BNE         RgnSetAttr_x5760
    /*00005a0c*/ B           RgnSetAttr_x56cc
    RgnSetAttr_x5a10:
    /*00005a10*/ LDR         R3, [R5, #180]              @ 0xb4 
    /*00005a14*/ CMP         R3, #1 
    /*00005a18*/ BEQ         RgnSetAttr_x5cb8
    RgnSetAttr_x5a1c:
    /*00005a1c*/ MOV         R2, #168                    @ 0xa8 
    /*00005a20*/ MOV         R1, SL 
    /*00005a24*/ ADD         R0, SP, #200                @ 0xc8 
    /*00005a28*/ BL          0x0000000000005A28          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005a2c*/ MOV         R2, #168                    @ 0xa8 
    /*00005a30*/ ADD         R1, SP, #32 
    /*00005a34*/ MOV         R0, SL 
    /*00005a38*/ BL          0x0000000000005A38          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005a3c*/ LDR         R2, [SP, #28] 
    /*00005a40*/ ADD         R1, SP, #32 
    /*00005a44*/ ADD         R9, R9, R2, LSL #2 
    /*00005a48*/ LDR         R0, [R9, #8] 
    /*00005a4c*/ BL          0x0000000000005A4C          @ RgnReallocBmpMem [R_ARM_CALL: .text+0x30c4] 
    /*00005a50*/ SUBS        R4, R0, #0 
    /*00005a54*/ BEQ         RgnSetAttr_x5b4c
    /*00005a58*/ ADD         R1, SP, #200                @ 0xc8 
    /*00005a5c*/ MOV         R0, SL 
    /*00005a60*/ MOV         R2, #168                    @ 0xa8 
    /*00005a64*/ BL          0x0000000000005A64          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00005a68*/ LDR         R3, [SP, #28] 
    /*00005a6c*/ MOV         R1, #3 
    /*00005a70*/ MOVW        R2, #3490                   @ 0xda2 
    /*00005a74*/ MOV         R0, R1 
    /*00005a78*/ STM         SP, {R2, R3} 
    /*00005a7c*/ MOVW        R2, #0                      @ .LC145 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d28] 
    /*00005a80*/ LDR         R3, [PC, #1392]             @ 0x0000000000005ff8 
    /*00005a84*/ MOVT        R2, #0                      @ .LC145 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d28] 
    /*00005a88*/ BL          0x0000000000005A88          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005a8c*/ B           RgnSetAttr_x5994
    RgnSetAttr_x5a90:
    /*00005a90*/ LDR         IP, [SP, #48]               @ 0x30 
    /*00005a94*/ LDR         R1, [R3, #24] 
    /*00005a98*/ CMP         IP, R1 
    /*00005a9c*/ BNE         RgnSetAttr_x5824
    /*00005aa0*/ LDR         IP, [SP, #52]               @ 0x34 
    /*00005aa4*/ LDR         R1, [R3, #28] 
    /*00005aa8*/ CMP         IP, R1 
    /*00005aac*/ BNE         RgnSetAttr_x5824
    /*00005ab0*/ LDR         IP, [SP, #100]              @ 0x64 
    /*00005ab4*/ LDR         R1, [R3, #76]               @ 0x4c 
    /*00005ab8*/ CMP         IP, R1 
    /*00005abc*/ BNE         RgnSetAttr_x5824
    /*00005ac0*/ LDR         IP, [SP, #140]              @ 0x8c 
    /*00005ac4*/ LDR         R1, [R3, #116]              @ 0x74 
    /*00005ac8*/ CMP         IP, R1 
    /*00005acc*/ BNE         RgnSetAttr_x5824
    /*00005ad0*/ B           RgnSetAttr_x5994
    RgnSetAttr_x5ad4:
    /*00005ad4*/ MOV         R4, #65024                  @ 0xfe00 
    /*00005ad8*/ MOVT        R4, #65535                  @ 0xffff 
    /*00005adc*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5ae0:
    /*00005ae0*/ LDR         R3, [SP, #24] 
    /*00005ae4*/ MOVW        R2, #3608                   @ 0xe18 
    /*00005ae8*/ LDR         R1, [PC, #1288]             @ 0x0000000000005ff8 
    /*00005aec*/ LDR         R0, [SP, #16] 
    /*00005af0*/ BL          0x0000000000005AF0          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005af4*/ B           RgnSetAttr_x5954
    RgnSetAttr_x5af8:
    /*00005af8*/ MOVW        R3, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb88] 
    /*00005afc*/ MOVW        R2, #3602                   @ 0xe12 
    /*00005b00*/ MOVT        R3, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb88] 
    /*00005b04*/ LDR         R1, [PC, #1260]             @ 0x0000000000005ff8 
    /*00005b08*/ LDR         R0, [SP, #16] 
    /*00005b0c*/ BL          0x0000000000005B0C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00005b10*/ B           RgnSetAttr_x5914
    RgnSetAttr_x5b14:
    /*00005b14*/ MOV         R1, #3 
    /*00005b18*/ STR         R3, [SP, #4] 
    /*00005b1c*/ MOV         R0, R1 
    /*00005b20*/ MOVW        R3, #3396                   @ 0xd44 
    /*00005b24*/ MOVW        R2, #0                      @ .LC142 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c48] 
    /*00005b28*/ STR         R3, [SP] 
    /*00005b2c*/ MOVT        R2, #0                      @ .LC142 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c48] 
    /*00005b30*/ LDR         R3, [PC, #1216]             @ 0x0000000000005ff8 
    /*00005b34*/ BL          0x0000000000005B34          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005b38*/ MOV         R0, R9 
    /*00005b3c*/ MOVW        R4, #32771                  @ 0x8003 
    /*00005b40*/ BL          0x0000000000005B40          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005b44*/ MOVT        R4, #40963                  @ 0xa003 
    /*00005b48*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5b4c:
    /*00005b4c*/ LDR         R2, [SP, #28] 
    /*00005b50*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00005b54*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00005b58*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005b5c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005b60*/ MOV         R4, #16384                  @ 0x4000 
    /*00005b64*/ ADD         R3, R3, R2, LSL #2 
    /*00005b68*/ MOVT        R4, #40963                  @ 0xa003 
    /*00005b6c*/ LDR         R2, [R3, #-1604]            @ 0xfffff9bc 
    /*00005b70*/ ADD         R2, R2, #1 
    /*00005b74*/ STR         R2, [R3, #-1604]            @ 0xfffff9bc 
    /*00005b78*/ BL          0x0000000000005B78          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005b7c*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5b80:
    /*00005b80*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005b84*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005b88*/ MOV         R4, R3 
    /*00005b8c*/ BL          0x0000000000005B8C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005b90*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5b94:
    /*00005b94*/ CMP         R7, #0 
    /*00005b98*/ MOV         R3, #1 
    /*00005b9c*/ STR         R3, [R5, #224]              @ 0xe0 
    /*00005ba0*/ BEQ         RgnSetAttr_x5fdc
    /*00005ba4*/ ADD         R8, SP, #368                @ 0x170 
    /*00005ba8*/ CMN         R7, #1 
    /*00005bac*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00005bb0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005bb4*/ LDR         R2, [R8, #-340]!            @ 0xfffffeac 
    /*00005bb8*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00005bbc*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005bc0*/ BEQ         RgnSetAttr_x5e84
    /*00005bc4*/ ADD         R3, R3, R2, LSL #2 
    /*00005bc8*/ MOVW        R9, #0                      @ Rgn_WaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x1dc] 
    /*00005bcc*/ ADD         FP, R5, #304                @ 0x130 
    /*00005bd0*/ MOVT        R9, #0                      @ Rgn_WaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x1dc] 
    /*00005bd4*/ LDR         R3, [R3, #-1604]            @ 0xfffff9bc 
    /*00005bd8*/ STR         R3, [SP, #16] 
    /*00005bdc*/ BL          0x0000000000005BDC          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005be0*/ MOV         R0, R8 
    /*00005be4*/ BL          0x0000000000005BE4          @ Rgn_WaitConditionCallBack [R_ARM_CALL: .text+0x1dc] 
    /*00005be8*/ B           RgnSetAttr_x5c00
    RgnSetAttr_x5bec:
    /*00005bec*/ BL          0x0000000000005BEC          @ osal_wait_timeout_uninterruptible [R_ARM_CALL: +0x0] 
    /*00005bf0*/ ADD         R3, R0, #512                @ 0x200 
    /*00005bf4*/ MOV         R7, R0 
    /*00005bf8*/ BICS        R3, R3, #512                @ 0x200 
    /*00005bfc*/ BEQ         RgnSetAttr_x5c2c
    RgnSetAttr_x5c00:
    /*00005c00*/ MOV         R0, R8 
    /*00005c04*/ BL          0x0000000000005C04          @ Rgn_WaitConditionCallBack [R_ARM_CALL: .text+0x1dc] 
    /*00005c08*/ MOV         R3, R7 
    /*00005c0c*/ MOV         R2, R8 
    /*00005c10*/ MOV         R1, R9 
    /*00005c14*/ CMP         R0, #0 
    /*00005c18*/ MOV         R0, FP 
    /*00005c1c*/ BEQ         RgnSetAttr_x5bec
    /*00005c20*/ MOV         R0, R7 
    /*00005c24*/ BL          0x0000000000005C24          @ osal_msecs_to_jiffies [R_ARM_CALL: +0x0] 
    /*00005c28*/ MOV         R7, R0 
    RgnSetAttr_x5c2c:
    /*00005c2c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005c30*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005c34*/ BL          0x0000000000005C34          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00005c38*/ CMP         R0, #0 
    /*00005c3c*/ BNE         RgnSetAttr_x5ad4
    /*00005c40*/ LDR         R1, [SP, #16] 
    /*00005c44*/ LDR         R0, [SP, #28] 
    /*00005c48*/ BL          0x0000000000005C48          @ RgnAbnormalVerifyAfterWait [R_ARM_CALL: .text+0x2c24] 
    /*00005c4c*/ SUBS        R3, R0, #0 
    /*00005c50*/ BNE         RgnSetAttr_x5b80
    /*00005c54*/ CMP         R7, #0 
    /*00005c58*/ BEQ         RgnSetAttr_x5f98
    /*00005c5c*/ STRLT       R3, [R5, #224]              @ 0xe0 
    /*00005c60*/ BLT         RgnSetAttr_x5f2c
    RgnSetAttr_x5c64:
    /*00005c64*/ LDR         R3, [R5] 
    /*00005c68*/ MOV         R2, #0 
    /*00005c6c*/ STR         R2, [R5, #224]              @ 0xe0 
    /*00005c70*/ CMP         R3, R2 
    /*00005c74*/ BNE         RgnSetAttr_x5870
    /*00005c78*/ LDR         R3, [SP, #28] 
    /*00005c7c*/ MOV         R1, #3 
    /*00005c80*/ MOVW        IP, #3575                   @ 0xdf7 
    /*00005c84*/ MOV         R0, R1 
    /*00005c88*/ STR         IP, [SP] 
    /*00005c8c*/ MOVW        R2, #0                      @ .LC144 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1cec] 
    /*00005c90*/ STR         R3, [SP, #4] 
    /*00005c94*/ MOVT        R2, #0                      @ .LC144 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1cec] 
    /*00005c98*/ LDR         R3, [PC, #856]              @ 0x0000000000005ff8 
    /*00005c9c*/ MOVW        R4, #32773                  @ 0x8005 
    /*00005ca0*/ BL          0x0000000000005CA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005ca4*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005ca8*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005cac*/ MOVT        R4, #40963                  @ 0xa003 
    /*00005cb0*/ BL          0x0000000000005CB0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005cb4*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5cb8:
    /*00005cb8*/ MOVW        R1, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00005cbc*/ MOVT        R1, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00005cc0*/ ADD         R2, R1, R2 
    /*00005cc4*/ STR         R3, [R5, #224]              @ 0xe0 
    /*00005cc8*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005ccc*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005cd0*/ LDR         FP, [R2, #-1604]            @ 0xfffff9bc 
    /*00005cd4*/ MOVW        R4, #0                      @ Rgn_WaitConditionCallBackEx [R_ARM_MOVW_ABS_NC: .text+0x134] 
    /*00005cd8*/ BL          0x0000000000005CD8          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005cdc*/ ADD         R8, SP, #28 
    /*00005ce0*/ MOVT        R4, #0                      @ Rgn_WaitConditionCallBackEx [R_ARM_MOVT_ABS: .text+0x134] 
    /*00005ce4*/ ADD         R6, R5, #300                @ 0x12c 
    /*00005ce8*/ B           RgnSetAttr_x5cf8
    RgnSetAttr_x5cec:
    /*00005cec*/ BL          0x0000000000005CEC          @ osal_wait_uninterruptible [R_ARM_CALL: +0x0] 
    /*00005cf0*/ CMP         R0, #0 
    /*00005cf4*/ BLT         RgnSetAttr_x5e50
    RgnSetAttr_x5cf8:
    /*00005cf8*/ MOV         R0, R8 
    /*00005cfc*/ BL          0x0000000000005CFC          @ Rgn_WaitConditionCallBackEx [R_ARM_CALL: .text+0x134] 
    /*00005d00*/ MOV         R2, R8 
    /*00005d04*/ MOV         R1, R4 
    /*00005d08*/ CMP         R0, #0 
    /*00005d0c*/ MOV         R0, R6 
    /*00005d10*/ BEQ         RgnSetAttr_x5cec
    /*00005d14*/ MOV         R7, #0 
    RgnSetAttr_x5d18:
    /*00005d18*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005d1c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005d20*/ BL          0x0000000000005D20          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00005d24*/ SUBS        R6, R0, #0 
    /*00005d28*/ BNE         RgnSetAttr_x5ad4
    /*00005d2c*/ MOV         R1, FP 
    /*00005d30*/ LDR         R0, [SP, #28] 
    /*00005d34*/ BL          0x0000000000005D34          @ RgnAbnormalVerifyAfterWait [R_ARM_CALL: .text+0x2c24] 
    /*00005d38*/ SUBS        R4, R0, #0 
    /*00005d3c*/ BNE         RgnSetAttr_x5f6c
    /*00005d40*/ CMP         R7, #0 
    /*00005d44*/ STRNE       R4, [R5, #224]              @ 0xe0 
    /*00005d48*/ BNE         RgnSetAttr_x5f2c
    /*00005d4c*/ LDR         R3, [SP, #28] 
    /*00005d50*/ STR         R4, [R5, #224]              @ 0xe0 
    /*00005d54*/ ADD         R2, R9, R3, LSL #2 
    /*00005d58*/ LDR         R2, [R2, #8] 
    /*00005d5c*/ LDR         R2, [R2] 
    /*00005d60*/ CMP         R2, #0 
    /*00005d64*/ BNE         RgnSetAttr_x5a1c
    /*00005d68*/ MOV         R1, #3 
    /*00005d6c*/ STR         R3, [SP, #4] 
    /*00005d70*/ MOV         R0, R1 
    /*00005d74*/ MOVW        R3, #3476                   @ 0xd94 
    /*00005d78*/ MOVW        R2, #0                      @ .LC144 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1cec] 
    /*00005d7c*/ STR         R3, [SP] 
    /*00005d80*/ MOVT        R2, #0                      @ .LC144 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1cec] 
    /*00005d84*/ LDR         R3, [PC, #620]              @ 0x0000000000005ff8 
    /*00005d88*/ BL          0x0000000000005D88          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005d8c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005d90*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005d94*/ MOVW        R4, #32773                  @ 0x8005 
    /*00005d98*/ BL          0x0000000000005D98          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005d9c*/ MOVT        R4, #40963                  @ 0xa003 
    /*00005da0*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5da4:
    /*00005da4*/ MOV         R1, #3 
    /*00005da8*/ MOVW        R2, #0                      @ .LC86 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1124] 
    /*00005dac*/ MOVT        R2, #0                      @ .LC86 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1124] 
    /*00005db0*/ MOV         R0, R1 
    /*00005db4*/ STR         R3, [SP, #4] 
    /*00005db8*/ MOVW        R3, #3375                   @ 0xd2f 
    /*00005dbc*/ STR         R3, [SP] 
    RgnSetAttr_x5dc0:
    /*00005dc0*/ LDR         R3, [PC, #560]              @ 0x0000000000005ff8 
    /*00005dc4*/ MOVW        R4, #32773                  @ 0x8005 
    /*00005dc8*/ BL          0x0000000000005DC8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005dcc*/ MOV         R0, R9 
    /*00005dd0*/ BL          0x0000000000005DD0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005dd4*/ MOVT        R4, #40963                  @ 0xa003 
    /*00005dd8*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5ddc:
    /*00005ddc*/ MOV         R1, #3 
    /*00005de0*/ STMIB       SP, {R3, R4} 
    /*00005de4*/ MOV         R0, R1 
    /*00005de8*/ MOVW        R3, #3388                   @ 0xd3c 
    /*00005dec*/ MOVW        R2, #0                      @ .LC141 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c04] 
    /*00005df0*/ STR         R3, [SP] 
    /*00005df4*/ MOVT        R2, #0                      @ .LC141 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c04] 
    /*00005df8*/ LDR         R3, [PC, #504]              @ 0x0000000000005ff8 
    /*00005dfc*/ BL          0x0000000000005DFC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005e00*/ MOV         R0, R9 
    /*00005e04*/ MOVW        R4, #32776                  @ 0x8008 
    /*00005e08*/ BL          0x0000000000005E08          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005e0c*/ MOVT        R4, #40963                  @ 0xa003 
    /*00005e10*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5e14:
    /*00005e14*/ MOV         R1, #3 
    /*00005e18*/ STR         R3, [SP, #4] 
    /*00005e1c*/ MOV         R0, R1 
    /*00005e20*/ MOVW        R3, #3418                   @ 0xd5a 
    /*00005e24*/ MOVW        R2, #0                      @ .LC143 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c80] 
    /*00005e28*/ STR         R3, [SP] 
    /*00005e2c*/ MOVT        R2, #0                      @ .LC143 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c80] 
    /*00005e30*/ LDR         R3, [PC, #448]              @ 0x0000000000005ff8 
    /*00005e34*/ BL          0x0000000000005E34          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005e38*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005e3c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005e40*/ MOVW        R4, #32777                  @ 0x8009 
    /*00005e44*/ BL          0x0000000000005E44          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005e48*/ MOVT        R4, #40963                  @ 0xa003 
    /*00005e4c*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5e50:
    /*00005e50*/ MOV         R7, R0 
    /*00005e54*/ B           RgnSetAttr_x5d18
    RgnSetAttr_x5e58:
    /*00005e58*/ MOV         R1, #3 
    /*00005e5c*/ MOVW        R3, #3403                   @ 0xd4b 
    /*00005e60*/ STR         R2, [SP, #4] 
    /*00005e64*/ MOV         R0, R1 
    /*00005e68*/ MOV         R2, #6 
    /*00005e6c*/ STR         R3, [SP] 
    /*00005e70*/ STR         R2, [SP, #8] 
    /*00005e74*/ MOVW        R2, #0                      @ .LC130 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19cc] 
    /*00005e78*/ LDR         R3, [PC, #376]              @ 0x0000000000005ff8 
    /*00005e7c*/ MOVT        R2, #0                      @ .LC130 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19cc] 
    /*00005e80*/ B           RgnSetAttr_x59e8
    RgnSetAttr_x5e84:
    /*00005e84*/ ADD         R3, R3, R2, LSL #2 
    /*00005e88*/ MOVW        R7, #0                      @ Rgn_WaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x1dc] 
    /*00005e8c*/ ADD         R9, R5, #304                @ 0x130 
    /*00005e90*/ MOVT        R7, #0                      @ Rgn_WaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x1dc] 
    /*00005e94*/ LDR         R3, [R3, #-1604]            @ 0xfffff9bc 
    /*00005e98*/ STR         R3, [SP, #16] 
    /*00005e9c*/ BL          0x0000000000005E9C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005ea0*/ B           RgnSetAttr_x5eb0
    RgnSetAttr_x5ea4:
    /*00005ea4*/ BL          0x0000000000005EA4          @ osal_wait_uninterruptible [R_ARM_CALL: +0x0] 
    /*00005ea8*/ CMP         R0, #0 
    /*00005eac*/ BLT         RgnSetAttr_x5f44
    RgnSetAttr_x5eb0:
    /*00005eb0*/ MOV         R0, R8 
    /*00005eb4*/ BL          0x0000000000005EB4          @ Rgn_WaitConditionCallBack [R_ARM_CALL: .text+0x1dc] 
    /*00005eb8*/ MOV         R2, R8 
    /*00005ebc*/ MOV         R1, R7 
    /*00005ec0*/ CMP         R0, #0 
    /*00005ec4*/ MOV         R0, R9 
    /*00005ec8*/ BEQ         RgnSetAttr_x5ea4
    /*00005ecc*/ MOV         FP, #0 
    RgnSetAttr_x5ed0:
    /*00005ed0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005ed4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005ed8*/ BL          0x0000000000005ED8          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00005edc*/ SUBS        R7, R0, #0 
    /*00005ee0*/ BNE         RgnSetAttr_x5ad4
    /*00005ee4*/ LDR         R1, [SP, #16] 
    /*00005ee8*/ LDR         R0, [SP, #28] 
    /*00005eec*/ BL          0x0000000000005EEC          @ RgnAbnormalVerifyAfterWait [R_ARM_CALL: .text+0x2c24] 
    /*00005ef0*/ SUBS        R2, R0, #0 
    /*00005ef4*/ BNE         RgnSetAttr_x5f80
    /*00005ef8*/ CMP         FP, #0 
    /*00005efc*/ BEQ         RgnSetAttr_x5c64
    /*00005f00*/ LDR         R3, [SP, #28] 
    /*00005f04*/ MOVW        R1, #3540                   @ 0xdd4 
    /*00005f08*/ STR         R2, [R5, #224]              @ 0xe0 
    /*00005f0c*/ MOV         R0, #4 
    /*00005f10*/ MOVW        R2, #0                      @ .LC146 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d60] 
    /*00005f14*/ STR         R1, [SP] 
    /*00005f18*/ STR         R3, [SP, #4] 
    /*00005f1c*/ MOVT        R2, #0                      @ .LC146 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d60] 
    /*00005f20*/ MOV         R1, #3 
    /*00005f24*/ LDR         R3, [PC, #204]              @ 0x0000000000005ff8 
    /*00005f28*/ BL          0x0000000000005F28          @ HI_LOG [R_ARM_CALL: +0x0] 
    RgnSetAttr_x5f2c:
    /*00005f2c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005f30*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005f34*/ MOV         R4, #65024                  @ 0xfe00 
    /*00005f38*/ BL          0x0000000000005F38          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005f3c*/ MOVT        R4, #65535                  @ 0xffff 
    /*00005f40*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5f44:
    /*00005f44*/ MOV         FP, R0 
    /*00005f48*/ B           RgnSetAttr_x5ed0
    RgnSetAttr_x5f4c:
    /*00005f4c*/ MOV         R1, #3 
    /*00005f50*/ STR         R3, [SP, #4] 
    /*00005f54*/ MOVW        R2, #0                      @ .LC90 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x120c] 
    /*00005f58*/ MOVW        R3, #3368                   @ 0xd28 
    /*00005f5c*/ MOV         R0, R1 
    /*00005f60*/ STR         R3, [SP] 
    /*00005f64*/ MOVT        R2, #0                      @ .LC90 [R_ARM_MOVT_ABS: .rodata.str1.4+0x120c] 
    /*00005f68*/ B           RgnSetAttr_x5dc0
    RgnSetAttr_x5f6c:
    /*00005f6c*/ STR         R6, [R5, #224]              @ 0xe0 
    /*00005f70*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005f74*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005f78*/ BL          0x0000000000005F78          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005f7c*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5f80:
    /*00005f80*/ STR         R7, [R5, #224]              @ 0xe0 
    /*00005f84*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005f88*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005f8c*/ MOV         R4, R2 
    /*00005f90*/ BL          0x0000000000005F90          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005f94*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5f98:
    /*00005f98*/ LDR         IP, [SP, #28] 
    /*00005f9c*/ MOV         R1, #3 
    /*00005fa0*/ STR         R7, [R5, #224]              @ 0xe0 
    /*00005fa4*/ MOV         R0, R1 
    /*00005fa8*/ MOVW        R3, #3561                   @ 0xde9 
    /*00005fac*/ MOVW        R2, #0                      @ .LC147 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1da8] 
    /*00005fb0*/ STR         R3, [SP] 
    /*00005fb4*/ MOVT        R2, #0                      @ .LC147 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1da8] 
    /*00005fb8*/ STR         IP, [SP, #4] 
    /*00005fbc*/ MOVW        R4, #32783                  @ 0x800f 
    /*00005fc0*/ LDR         R3, [PC, #48]               @ 0x0000000000005ff8 
    /*00005fc4*/ MOVT        R4, #40963                  @ 0xa003 
    /*00005fc8*/ BL          0x0000000000005FC8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005fcc*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005fd0*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005fd4*/ BL          0x0000000000005FD4          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005fd8*/ B           RgnSetAttr_x5760
    RgnSetAttr_x5fdc:
    /*00005fdc*/ STR         R4, [R5, #224]              @ 0xe0 
    /*00005fe0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00005fe4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00005fe8*/ MOVW        R4, #32783                  @ 0x800f 
    /*00005fec*/ BL          0x0000000000005FEC          @ osal_up [R_ARM_CALL: +0x0] 
    /*00005ff0*/ MOVT        R4, #40963                  @ 0xa003 
    /*00005ff4*/ B           RgnSetAttr_x5760
    /*00005ff8*/ ANDEQ       R0, R0, R0, ASR R4 
    /*00005ffc*/ ANDEQ       R0, R0, R4 
FUNC_END RgnSetAttr


FUNC_BEGIN RgnSetBitMap
    /*00006000*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00006004*/ CMP         R0, #127                    @ 0x7f 
    /*00006008*/ SUB         SP, SP, #76                 @ 0x4c 
    /*0000600c*/ MOV         R6, R1 
    /*00006010*/ MOV         R8, R2 
    /*00006014*/ STR         R0, [SP, #60]               @ 0x3c 
    /*00006018*/ BHI         RgnSetBitMap_x6284
    RgnSetBitMap_x601c:
    /*0000601c*/ CMP         R6, #0 
    /*00006020*/ BEQ         RgnSetBitMap_x64e0
    RgnSetBitMap_x6024:
    /*00006024*/ LDR         R3, [R6, #12] 
    /*00006028*/ CMP         R3, #0 
    /*0000602c*/ BEQ         RgnSetBitMap_x654c
    RgnSetBitMap_x6030:
    /*00006030*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006034*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006038*/ STR         R0, [SP, #20] 
    /*0000603c*/ BL          0x000000000000603C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00006040*/ SUBS        R5, R0, #0 
    /*00006044*/ BNE         RgnSetBitMap_x63d0
    /*00006048*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*0000604c*/ LDR         R2, [SP, #20] 
    /*00006050*/ ADD         R2, R2, R3, LSL #2 
    /*00006054*/ LDR         R4, [R2, #8] 
    /*00006058*/ CMP         R4, #0 
    /*0000605c*/ BEQ         RgnSetBitMap_x679c
    /*00006060*/ LDR         R2, [R4] 
    /*00006064*/ CMP         R2, #0 
    /*00006068*/ BEQ         RgnSetBitMap_x66f0
    /*0000606c*/ LDR         R3, [R4, #176]              @ 0xb0 
    /*00006070*/ CMP         R3, #4 
    /*00006074*/ STR         R3, [SP, #36]               @ 0x24 
    /*00006078*/ BHI         RgnSetBitMap_x64f0
    RgnSetBitMap_x607c:
    /*0000607c*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00006080*/ CMP         R3, #3 
    /*00006084*/ CMPNE       R3, #0 
    /*00006088*/ MOVNE       R9, #1 
    /*0000608c*/ MOVEQ       R9, #0 
    /*00006090*/ BNE         RgnSetBitMap_x6728
    /*00006094*/ LDR         R3, [R4, #228]              @ 0xe4 
    /*00006098*/ CMP         R3, #1 
    /*0000609c*/ BEQ         RgnSetBitMap_x6754
    /*000060a0*/ LDMIB       R6, {R2, R3} 
    /*000060a4*/ CMP         R3, #0 
    /*000060a8*/ BEQ         RgnSetBitMap_x6620
    /*000060ac*/ CMP         R2, #0 
    /*000060b0*/ BEQ         RgnSetBitMap_x6620
    /*000060b4*/ LDR         R2, [R6] 
    /*000060b8*/ LDR         R3, [R4, #120]              @ 0x78 
    /*000060bc*/ CMP         R2, R3 
    /*000060c0*/ BNE         RgnSetBitMap_x665c
    /*000060c4*/ LDR         R3, [R4, #116]              @ 0x74 
    /*000060c8*/ CMP         R3, #1 
    /*000060cc*/ BLS         RgnSetBitMap_x629c
    /*000060d0*/ MOV         R0, R4 
    /*000060d4*/ BL          0x00000000000000B8 
    /*000060d8*/ CMN         R0, #1 
    /*000060dc*/ MOVNE       R7, R9 
    /*000060e0*/ BEQ         RgnSetBitMap_x62b4
    RgnSetBitMap_x60e4:
    /*000060e4*/ MOV         R0, R4 
    /*000060e8*/ BL          0x00000000000000B8 
    /*000060ec*/ MOV         LR, #0 
    /*000060f0*/ MOV         R1, R0 
    /*000060f4*/ STR         R0, [R4, #212]              @ 0xd4 
    RgnSetBitMap_x60f8:
    /*000060f8*/ LDR         IP, [R4, #240]              @ 0xf0 
    /*000060fc*/ ADD         R3, R4, #256                @ 0x100 
    /*00006100*/ LDRD        SL, [R4, #248]              @ 0xf8 
    /*00006104*/ LDRD        R2, [R3] 
    /*00006108*/ UMULL       R0, R1, IP, R1 
    /*0000610c*/ STRD        SL, [SP, #48]               @ 0x30 
    /*00006110*/ LDR         R8, [R4, #28] 
    /*00006114*/ STRD        R0, [SP, #24] 
    /*00006118*/ LDR         R9, [SP, #28] 
    /*0000611c*/ LDR         R1, [R4, #24] 
    /*00006120*/ LDR         R0, [R6, #8] 
    /*00006124*/ MLA         IP, IP, LR, R9 
    /*00006128*/ STR         R1, [SP, #32] 
    /*0000612c*/ LDR         R1, [R6, #4] 
    /*00006130*/ STR         IP, [SP, #28] 
    /*00006134*/ LDRD        SL, [SP, #24] 
    /*00006138*/ ADDS        SL, SL, R2 
    /*0000613c*/ ADC         FP, FP, R3 
    /*00006140*/ LDR         R3, [SP, #32] 
    /*00006144*/ STRD        SL, [SP, #40]               @ 0x28 
    /*00006148*/ CMP         R8, R0 
    /*0000614c*/ CMPLE       R3, R1 
    /*00006150*/ BLE         RgnSetBitMap_x61a4
    /*00006154*/ CMP         R8, #0 
    /*00006158*/ BEQ         RgnSetBitMap_x63e0
    /*0000615c*/ LDR         FP, [SP, #32] 
    /*00006160*/ MOV         R9, #0 
    /*00006164*/ LDR         SL, [SP, #40]               @ 0x28 
    /*00006168*/ MOV         R3, FP 
    /*0000616c*/ B           RgnSetBitMap_x6174
    RgnSetBitMap_x6170:
    /*00006170*/ LDR         R3, [R4, #24] 
    RgnSetBitMap_x6174:
    /*00006174*/ LDR         R0, [R4, #112]              @ 0x70 
    /*00006178*/ LDR         R2, [R4, #264]              @ 0x108 
    /*0000617c*/ LDR         R1, [R4, #76]               @ 0x4c 
    /*00006180*/ MLA         R0, R0, R9, SL 
    /*00006184*/ BL          0x0000000000006184          @ RgnMemSet [R_ARM_CALL: .text+0xbd0] 
    /*00006188*/ LDR         R3, [R4, #28] 
    /*0000618c*/ ADD         R9, R9, #1 
    /*00006190*/ CMP         R3, R9 
    /*00006194*/ BHI         RgnSetBitMap_x6170
    /*00006198*/ LDR         R0, [R6, #8] 
    /*0000619c*/ LDR         R1, [R6, #4] 
    /*000061a0*/ STR         FP, [SP, #32] 
    RgnSetBitMap_x61a4:
    /*000061a4*/ LDR         R3, [SP, #32] 
    /*000061a8*/ CMP         R8, R0 
    /*000061ac*/ MOVCS       R8, R0 
    /*000061b0*/ CMP         R3, R1 
    /*000061b4*/ MOVCS       R3, R1 
    /*000061b8*/ CMP         R8, #0 
    /*000061bc*/ MOV         R2, R3 
    /*000061c0*/ BEQ         RgnSetBitMap_x63e0
    /*000061c4*/ ADD         R3, R3, #3 
    /*000061c8*/ STR         R7, [SP, #56]               @ 0x38 
    /*000061cc*/ LDR         R9, [SP, #40]               @ 0x28 
    /*000061d0*/ MOV         R7, R2 
    /*000061d4*/ LSR         R3, R3, #2 
    /*000061d8*/ STR         R3, [SP, #32] 
    /*000061dc*/ B           RgnSetBitMap_x6204
    RgnSetBitMap_x61e0:
    /*000061e0*/ CMP         IP, LR 
    /*000061e4*/ BHI         RgnSetBitMap_x6248
    /*000061e8*/ MOV         R1, FP 
    /*000061ec*/ BL          0x00000000000061EC          @ osal_copy_from_user [R_ARM_CALL: +0x0] 
    /*000061f0*/ CMP         R0, #0 
    /*000061f4*/ BNE         RgnSetBitMap_x6510
    /*000061f8*/ CMP         R5, R8 
    /*000061fc*/ BEQ         RgnSetBitMap_x63dc
    /*00006200*/ LDR         R1, [R6, #4] 
    RgnSetBitMap_x6204:
    /*00006204*/ LDR         R3, [R4, #120]              @ 0x78 
    /*00006208*/ LDR         LR, [R4, #112]              @ 0x70 
    /*0000620c*/ CMP         R3, #12 
    /*00006210*/ LDR         R3, [R4, #264]              @ 0x108 
    /*00006214*/ ADDEQ       R1, R1, #3 
    /*00006218*/ LDR         FP, [R6, #12] 
    /*0000621c*/ LDREQ       R2, [SP, #32] 
    /*00006220*/ MLA         SL, R5, LR, R9 
    /*00006224*/ LSREQ       R1, R1, #2 
    /*00006228*/ MULNE       IP, R3, R7 
    /*0000622c*/ MULEQ       IP, R2, R3 
    /*00006230*/ MUL         R3, R5, R3 
    /*00006234*/ MOV         R0, SL 
    /*00006238*/ SUBS        R2, IP, #0 
    /*0000623c*/ ADD         R5, R5, #1 
    /*00006240*/ MLA         FP, R1, R3, FP 
    /*00006244*/ BNE         RgnSetBitMap_x61e0
    RgnSetBitMap_x6248:
    /*00006248*/ MOV         R1, #3 
    /*0000624c*/ STR         IP, [SP, #4] 
    /*00006250*/ MOV         R0, R1 
    /*00006254*/ MOVW        R3, #3877                   @ 0xf25 
    /*00006258*/ MOVW        R2, #0                      @ .LC155 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fe4] 
    /*0000625c*/ STR         R3, [SP] 
    /*00006260*/ MOVT        R2, #0                      @ .LC155 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fe4] 
    /*00006264*/ LDR         R3, [PC, #1436]             @ 0x0000000000006808 
    /*00006268*/ BL          0x0000000000006268          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000626c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006270*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006274*/ MOVW        R7, #32785                  @ 0x8011 
    /*00006278*/ BL          0x0000000000006278          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000627c*/ MOVT        R7, #40963                  @ 0xa003 
    /*00006280*/ B           RgnSetBitMap_x6290
    RgnSetBitMap_x6284:
    /*00006284*/ BL          0x00000000000002FC 
    /*00006288*/ SUBS        R7, R0, #0 
    /*0000628c*/ BEQ         RgnSetBitMap_x601c
    RgnSetBitMap_x6290:
    /*00006290*/ MOV         R0, R7 
    /*00006294*/ ADD         SP, SP, #76                 @ 0x4c 
    /*00006298*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnSetBitMap_x629c:
    /*0000629c*/ BNE         RgnSetBitMap_x66b4
    /*000062a0*/ MOV         R7, R9 
    /*000062a4*/ STR         R9, [R4, #212]              @ 0xd4 
    /*000062a8*/ MOV         R1, R9 
    /*000062ac*/ MOV         LR, R9 
    /*000062b0*/ B           RgnSetBitMap_x60f8
    RgnSetBitMap_x62b4:
    /*000062b4*/ CMP         R8, #0 
    /*000062b8*/ MOV         R3, #1 
    /*000062bc*/ STR         R3, [R4, #224]              @ 0xe0 
    /*000062c0*/ BEQ         RgnSetBitMap_x6780
    /*000062c4*/ ADD         R9, SP, #72                 @ 0x48 
    /*000062c8*/ CMN         R8, #1 
    /*000062cc*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*000062d0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000062d4*/ LDR         R2, [R9, #-12]! 
    /*000062d8*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*000062dc*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000062e0*/ BEQ         RgnSetBitMap_x6564
    /*000062e4*/ ADD         R3, R3, R2, LSL #2 
    /*000062e8*/ MOVW        R7, #0                      @ Rgn_WaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x1dc] 
    /*000062ec*/ ADD         FP, R4, #304                @ 0x130 
    /*000062f0*/ MOVT        R7, #0                      @ Rgn_WaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x1dc] 
    /*000062f4*/ LDR         SL, [R3, #-1604]            @ 0xfffff9bc 
    /*000062f8*/ BL          0x00000000000062F8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000062fc*/ MOV         R0, R9 
    /*00006300*/ BL          0x0000000000006300          @ Rgn_WaitConditionCallBack [R_ARM_CALL: .text+0x1dc] 
    /*00006304*/ B           RgnSetBitMap_x631c
    RgnSetBitMap_x6308:
    /*00006308*/ BL          0x0000000000006308          @ osal_wait_timeout_uninterruptible [R_ARM_CALL: +0x0] 
    /*0000630c*/ ADD         R3, R0, #512                @ 0x200 
    /*00006310*/ MOV         R8, R0 
    /*00006314*/ BICS        R3, R3, #512                @ 0x200 
    /*00006318*/ BEQ         RgnSetBitMap_x655c
    RgnSetBitMap_x631c:
    /*0000631c*/ MOV         R0, R9 
    /*00006320*/ BL          0x0000000000006320          @ Rgn_WaitConditionCallBack [R_ARM_CALL: .text+0x1dc] 
    /*00006324*/ MOV         R3, R8 
    /*00006328*/ MOV         R2, R9 
    /*0000632c*/ MOV         R1, R7 
    /*00006330*/ CMP         R0, #0 
    /*00006334*/ MOV         R0, FP 
    /*00006338*/ BEQ         RgnSetBitMap_x6308
    /*0000633c*/ MOV         R0, R8 
    /*00006340*/ BL          0x0000000000006340          @ osal_msecs_to_jiffies [R_ARM_CALL: +0x0] 
    /*00006344*/ MOV         R7, R0 
    RgnSetBitMap_x6348:
    /*00006348*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000634c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006350*/ BL          0x0000000000006350          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00006354*/ CMP         R0, #0 
    /*00006358*/ BNE         RgnSetBitMap_x63d0
    /*0000635c*/ MOV         R1, SL 
    /*00006360*/ LDR         R0, [SP, #60]               @ 0x3c 
    /*00006364*/ BL          0x0000000000006364          @ RgnAbnormalVerifyAfterWait [R_ARM_CALL: .text+0x2c24] 
    /*00006368*/ SUBS        R3, R0, #0 
    /*0000636c*/ BNE         RgnSetBitMap_x6648
    /*00006370*/ CMP         R7, #0 
    /*00006374*/ BEQ         RgnSetBitMap_x67bc
    /*00006378*/ BLT         RgnSetBitMap_x6800
    RgnSetBitMap_x637c:
    /*0000637c*/ LDR         R3, [R4] 
    /*00006380*/ MOV         R2, #0 
    /*00006384*/ STR         R2, [R4, #224]              @ 0xe0 
    /*00006388*/ CMP         R3, R2 
    /*0000638c*/ BNE         RgnSetBitMap_x60e4
    /*00006390*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*00006394*/ MOV         R1, #3 
    /*00006398*/ MOVW        R2, #0                      @ .LC144 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1cec] 
    /*0000639c*/ MOVT        R2, #0                      @ .LC144 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1cec] 
    /*000063a0*/ MOV         R0, R1 
    /*000063a4*/ MOVW        R7, #32773                  @ 0x8005 
    /*000063a8*/ STR         R3, [SP, #4] 
    /*000063ac*/ MOVW        IP, #3809                   @ 0xee1 
    /*000063b0*/ LDR         R3, [PC, #1104]             @ 0x0000000000006808 
    /*000063b4*/ STR         IP, [SP] 
    RgnSetBitMap_x63b8:
    /*000063b8*/ BL          0x00000000000063B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000063bc*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000063c0*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000063c4*/ MOVT        R7, #40963                  @ 0xa003 
    /*000063c8*/ BL          0x00000000000063C8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000063cc*/ B           RgnSetBitMap_x6290
    RgnSetBitMap_x63d0:
    /*000063d0*/ MOV         R7, #65024                  @ 0xfe00 
    /*000063d4*/ MOVT        R7, #65535                  @ 0xffff 
    /*000063d8*/ B           RgnSetBitMap_x6290
    RgnSetBitMap_x63dc:
    /*000063dc*/ LDR         R7, [SP, #56]               @ 0x38 
    RgnSetBitMap_x63e0:
    /*000063e0*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*000063e4*/ MOV         R2, #1 
    /*000063e8*/ STR         R2, [R4, #232]              @ 0xe8 
    /*000063ec*/ LDR         R2, [SP, #20] 
    /*000063f0*/ ADD         R3, R2, R3, LSL #2 
    /*000063f4*/ LDR         R3, [R3, #8] 
    /*000063f8*/ LDR         R3, [R3, #208]              @ 0xd0 
    /*000063fc*/ CMP         R3, #0 
    /*00006400*/ BNE         RgnSetBitMap_x64d0
    /*00006404*/ LDRD        R0, [SP, #24] 
    /*00006408*/ MOV         R6, R4 
    /*0000640c*/ LDRD        R2, [SP, #48]               @ 0x30 
    /*00006410*/ ADDS        R2, R2, R0 
    /*00006414*/ ADC         R3, R3, R1 
    /*00006418*/ LDRD        R0, [SP, #40]               @ 0x28 
    /*0000641c*/ STRD        R2, [R4, #96]               @ 0x60 
    /*00006420*/ STRD        R0, [R4, #104]              @ 0x68 
    /*00006424*/ ADD         R1, SP, #68                 @ 0x44 
    /*00006428*/ LDR         R0, [PC, #988]              @ 0x000000000000680c 
    /*0000642c*/ BL          0x000000000000642C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00006430*/ LDR         R8, [R6, #308]!             @ 0x134 
    /*00006434*/ CMP         R6, R8 
    /*00006438*/ LDR         R5, [R8] 
    /*0000643c*/ BEQ         RgnSetBitMap_x64bc
    /*00006440*/ MOVW        SL, #0                      @ .LC137 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b68] 
    /*00006444*/ MOVW        R9, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00006448*/ MOVT        SL, #0                      @ .LC137 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b68] 
    /*0000644c*/ MOVT        R9, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00006450*/ STR         SL, [SP, #20] 
    /*00006454*/ LDR         FP, [PC, #940]              @ 0x0000000000006808 
    /*00006458*/ LDR         SL, [SP, #36]               @ 0x24 
    /*0000645c*/ B           RgnSetBitMap_x6464
    RgnSetBitMap_x6460:
    /*00006460*/ MOV         R5, R3 
    RgnSetBitMap_x6464:
    /*00006464*/ ADD         R8, R8, #8 
    /*00006468*/ LDR         R2, [SP, #60]               @ 0x3c 
    /*0000646c*/ MOV         R0, SL 
    /*00006470*/ MOV         R1, R8 
    /*00006474*/ BL          0x0000000000006474          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*00006478*/ MOV         R1, R8 
    /*0000647c*/ SUBS        IP, R0, #0 
    /*00006480*/ MOV         R0, SL 
    /*00006484*/ BEQ         RgnSetBitMap_x64ac
    /*00006488*/ LDRD        R2, [R4, #96]               @ 0x60 
    /*0000648c*/ STRD        R2, [IP, #88]               @ 0x58 
    /*00006490*/ LDRD        R2, [R4, #104]              @ 0x68 
    /*00006494*/ STRD        R2, [IP, #96]               @ 0x60 
    /*00006498*/ BL          0x0000000000006498          @ RgnGetChnGetInfo [R_ARM_CALL: .text+0x1034] 
    /*0000649c*/ SUBS        R8, R0, #0 
    /*000064a0*/ BEQ         RgnSetBitMap_x669c
    RgnSetBitMap_x64a4:
    /*000064a4*/ MOV         R3, #1 
    /*000064a8*/ STR         R3, [R8, #4] 
    RgnSetBitMap_x64ac:
    /*000064ac*/ CMP         R6, R5 
    /*000064b0*/ LDR         R3, [R5] 
    /*000064b4*/ MOV         R8, R5 
    /*000064b8*/ BNE         RgnSetBitMap_x6460
    RgnSetBitMap_x64bc:
    /*000064bc*/ LDR         R3, [R4, #212]              @ 0xd4 
    /*000064c0*/ ADD         R1, SP, #68                 @ 0x44 
    /*000064c4*/ LDR         R0, [PC, #832]              @ 0x000000000000680c 
    /*000064c8*/ STR         R3, [R4, #216]              @ 0xd8 
    /*000064cc*/ BL          0x00000000000064CC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    RgnSetBitMap_x64d0:
    /*000064d0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000064d4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000064d8*/ BL          0x00000000000064D8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000064dc*/ B           RgnSetBitMap_x6290
    RgnSetBitMap_x64e0:
    /*000064e0*/ BL          0x0000000000000290 
    /*000064e4*/ SUBS        R7, R0, #0 
    /*000064e8*/ BNE         RgnSetBitMap_x6290
    /*000064ec*/ B           RgnSetBitMap_x6024
    RgnSetBitMap_x64f0:
    /*000064f0*/ MOVW        R3, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1278] 
    /*000064f4*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000064f8*/ MOVT        R3, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1278] 
    /*000064fc*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00006500*/ MOVW        R2, #3711                   @ 0xe7f 
    /*00006504*/ LDR         R1, [PC, #764]              @ 0x0000000000006808 
    /*00006508*/ BL          0x0000000000006508          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000650c*/ B           RgnSetBitMap_x607c
    RgnSetBitMap_x6510:
    /*00006510*/ MOVW        R3, #3870                   @ 0xf1e 
    /*00006514*/ STR         R3, [SP] 
    /*00006518*/ LDR         R3, [PC, #744]              @ 0x0000000000006808 
    /*0000651c*/ MOV         R1, #3 
    /*00006520*/ MOVW        R2, #0                      @ .LC154 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f88] 
    /*00006524*/ MOVT        R2, #0                      @ .LC154 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f88] 
    /*00006528*/ MOV         R0, R1 
    /*0000652c*/ MOVW        R7, #32785                  @ 0x8011 
    /*00006530*/ STMIB       SP, {SL, FP} 
    RgnSetBitMap_x6534:
    /*00006534*/ BL          0x0000000000006534          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006538*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000653c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006540*/ MOVT        R7, #40963                  @ 0xa003 
    /*00006544*/ BL          0x0000000000006544          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006548*/ B           RgnSetBitMap_x6290
    RgnSetBitMap_x654c:
    /*0000654c*/ BL          0x0000000000000290 
    /*00006550*/ SUBS        R7, R0, #0 
    /*00006554*/ BNE         RgnSetBitMap_x6290
    /*00006558*/ B           RgnSetBitMap_x6030
    RgnSetBitMap_x655c:
    /*0000655c*/ MOV         R7, R0 
    /*00006560*/ B           RgnSetBitMap_x6348
    RgnSetBitMap_x6564:
    /*00006564*/ ADD         R3, R3, R2, LSL #2 
    /*00006568*/ MOVW        R8, #0                      @ Rgn_WaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x1dc] 
    /*0000656c*/ ADD         SL, R4, #304                @ 0x130 
    /*00006570*/ MOVT        R8, #0                      @ Rgn_WaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x1dc] 
    /*00006574*/ LDR         FP, [R3, #-1604]            @ 0xfffff9bc 
    /*00006578*/ BL          0x0000000000006578          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000657c*/ B           RgnSetBitMap_x658c
    RgnSetBitMap_x6580:
    /*00006580*/ BL          0x0000000000006580          @ osal_wait_uninterruptible [R_ARM_CALL: +0x0] 
    /*00006584*/ CMP         R0, #0 
    /*00006588*/ BLT         RgnSetBitMap_x6694
    RgnSetBitMap_x658c:
    /*0000658c*/ MOV         R0, R9 
    /*00006590*/ BL          0x0000000000006590          @ Rgn_WaitConditionCallBack [R_ARM_CALL: .text+0x1dc] 
    /*00006594*/ MOV         R2, R9 
    /*00006598*/ MOV         R1, R8 
    /*0000659c*/ CMP         R0, #0 
    /*000065a0*/ MOV         R0, SL 
    /*000065a4*/ BEQ         RgnSetBitMap_x6580
    /*000065a8*/ MOV         R7, #0 
    RgnSetBitMap_x65ac:
    /*000065ac*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000065b0*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000065b4*/ BL          0x00000000000065B4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000065b8*/ CMP         R0, #0 
    /*000065bc*/ BNE         RgnSetBitMap_x63d0
    /*000065c0*/ MOV         R1, FP 
    /*000065c4*/ LDR         R0, [SP, #60]               @ 0x3c 
    /*000065c8*/ BL          0x00000000000065C8          @ RgnAbnormalVerifyAfterWait [R_ARM_CALL: .text+0x2c24] 
    /*000065cc*/ SUBS        R3, R0, #0 
    /*000065d0*/ BNE         RgnSetBitMap_x6648
    /*000065d4*/ CMP         R7, #0 
    /*000065d8*/ BEQ         RgnSetBitMap_x637c
    /*000065dc*/ LDR         R1, [SP, #60]               @ 0x3c 
    /*000065e0*/ MOVW        R0, #3773                   @ 0xebd 
    /*000065e4*/ STR         R3, [R4, #224]              @ 0xe0 
    /*000065e8*/ MOVW        R2, #0                      @ .LC146 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d60] 
    /*000065ec*/ STR         R0, [SP] 
    /*000065f0*/ MOVT        R2, #0                      @ .LC146 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d60] 
    /*000065f4*/ STR         R1, [SP, #4] 
    /*000065f8*/ MOV         R0, #4 
    /*000065fc*/ MOV         R1, #3 
    /*00006600*/ LDR         R3, [PC, #512]              @ 0x0000000000006808 
    /*00006604*/ BL          0x0000000000006604          @ HI_LOG [R_ARM_CALL: +0x0] 
    RgnSetBitMap_x6608:
    /*00006608*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000660c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006610*/ MOV         R7, #65024                  @ 0xfe00 
    /*00006614*/ BL          0x0000000000006614          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006618*/ MOVT        R7, #65535                  @ 0xffff 
    /*0000661c*/ B           RgnSetBitMap_x6290
    RgnSetBitMap_x6620:
    /*00006620*/ MOV         R1, #3 
    /*00006624*/ STMIB       SP, {R2, R3} 
    /*00006628*/ MOV         R0, R1 
    /*0000662c*/ MOVW        R3, #3731                   @ 0xe93 
    /*00006630*/ MOVW        R2, #0                      @ .LC152 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f24] 
    /*00006634*/ STR         R3, [SP] 
    /*00006638*/ MOVT        R2, #0                      @ .LC152 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f24] 
    /*0000663c*/ LDR         R3, [PC, #452]              @ 0x0000000000006808 
    /*00006640*/ MOVW        R7, #32771                  @ 0x8003 
    /*00006644*/ B           RgnSetBitMap_x6534
    RgnSetBitMap_x6648:
    /*00006648*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000664c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006650*/ MOV         R7, R3 
    /*00006654*/ BL          0x0000000000006654          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006658*/ B           RgnSetBitMap_x6290
    RgnSetBitMap_x665c:
    /*0000665c*/ MOV         R1, #3 
    /*00006660*/ MOVW        R3, #3739                   @ 0xe9b 
    /*00006664*/ MOV         R0, R1 
    /*00006668*/ STR         R3, [SP] 
    /*0000666c*/ MOVW        R2, #0                      @ .LC153 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f58] 
    /*00006670*/ LDR         R3, [PC, #400]              @ 0x0000000000006808 
    /*00006674*/ MOVT        R2, #0                      @ .LC153 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f58] 
    /*00006678*/ MOVW        R7, #32771                  @ 0x8003 
    /*0000667c*/ BL          0x000000000000667C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006680*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006684*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006688*/ MOVT        R7, #40963                  @ 0xa003 
    /*0000668c*/ BL          0x000000000000668C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006690*/ B           RgnSetBitMap_x6290
    RgnSetBitMap_x6694:
    /*00006694*/ MOV         R7, R0 
    /*00006698*/ B           RgnSetBitMap_x65ac
    RgnSetBitMap_x669c:
    /*0000669c*/ LDR         R3, [SP, #20] 
    /*000066a0*/ MOVW        R2, #3906                   @ 0xf42 
    /*000066a4*/ MOV         R1, FP 
    /*000066a8*/ MOV         R0, R9 
    /*000066ac*/ BL          0x00000000000066AC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000066b0*/ B           RgnSetBitMap_x64a4
    RgnSetBitMap_x66b4:
    /*000066b4*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000066b8*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000066bc*/ BL          0x00000000000066BC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000066c0*/ LDR         R3, [R4, #116]              @ 0x74 
    /*000066c4*/ MOV         R1, #3 
    /*000066c8*/ MOV         R2, #3824                   @ 0xef0 
    /*000066cc*/ MOV         R0, R1 
    /*000066d0*/ MOVW        R7, #32771                  @ 0x8003 
    /*000066d4*/ STM         SP, {R2, R3} 
    /*000066d8*/ MOVW        R2, #0                      @ .LC115 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15dc] 
    /*000066dc*/ LDR         R3, [PC, #292]              @ 0x0000000000006808 
    /*000066e0*/ MOVT        R2, #0                      @ .LC115 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15dc] 
    /*000066e4*/ BL          0x00000000000066E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000066e8*/ MOVT        R7, #40963                  @ 0xa003 
    /*000066ec*/ B           RgnSetBitMap_x6290
    RgnSetBitMap_x66f0:
    /*000066f0*/ MOV         R1, #3 
    /*000066f4*/ MOVW        R2, #0                      @ .LC86 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1124] 
    /*000066f8*/ MOVT        R2, #0                      @ .LC86 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1124] 
    /*000066fc*/ MOV         R0, R1 
    /*00006700*/ STR         R3, [SP, #4] 
    /*00006704*/ MOVW        R3, #3705                   @ 0xe79 
    /*00006708*/ STR         R3, [SP] 
    RgnSetBitMap_x670c:
    /*0000670c*/ LDR         R3, [PC, #244]              @ 0x0000000000006808 
    /*00006710*/ MOVW        R7, #32773                  @ 0x8005 
    /*00006714*/ BL          0x0000000000006714          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006718*/ LDR         R0, [SP, #20] 
    /*0000671c*/ BL          0x000000000000671C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006720*/ MOVT        R7, #40963                  @ 0xa003 
    /*00006724*/ B           RgnSetBitMap_x6290
    RgnSetBitMap_x6728:
    /*00006728*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*0000672c*/ MOV         R1, #3 
    /*00006730*/ MOVW        IP, #3715                   @ 0xe83 
    /*00006734*/ MOVW        R2, #0                      @ .LC150 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e74] 
    /*00006738*/ MOV         R0, R1 
    /*0000673c*/ STR         IP, [SP] 
    /*00006740*/ STR         R3, [SP, #4] 
    /*00006744*/ MOVT        R2, #0                      @ .LC150 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e74] 
    /*00006748*/ LDR         R3, [PC, #184]              @ 0x0000000000006808 
    /*0000674c*/ MOVW        R7, #32776                  @ 0x8008 
    /*00006750*/ B           RgnSetBitMap_x63b8
    RgnSetBitMap_x6754:
    /*00006754*/ LDR         R3, [SP, #60]               @ 0x3c 
    /*00006758*/ MOV         R1, #3 
    /*0000675c*/ MOVW        IP, #3723                   @ 0xe8b 
    /*00006760*/ MOVW        R2, #0                      @ .LC151 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eb0] 
    /*00006764*/ MOV         R0, R1 
    /*00006768*/ STR         IP, [SP] 
    /*0000676c*/ STR         R3, [SP, #4] 
    /*00006770*/ MOVT        R2, #0                      @ .LC151 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eb0] 
    /*00006774*/ LDR         R3, [PC, #140]              @ 0x0000000000006808 
    /*00006778*/ MOVW        R7, #32777                  @ 0x8009 
    /*0000677c*/ B           RgnSetBitMap_x63b8
    RgnSetBitMap_x6780:
    /*00006780*/ STR         R8, [R4, #224]              @ 0xe0 
    /*00006784*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006788*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000678c*/ MOVW        R7, #32783                  @ 0x800f 
    /*00006790*/ BL          0x0000000000006790          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006794*/ MOVT        R7, #40963                  @ 0xa003 
    /*00006798*/ B           RgnSetBitMap_x6290
    RgnSetBitMap_x679c:
    /*0000679c*/ MOV         R1, #3 
    /*000067a0*/ STR         R3, [SP, #4] 
    /*000067a4*/ MOVW        R2, #0                      @ .LC149 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e44] 
    /*000067a8*/ MOVW        R3, #3698                   @ 0xe72 
    /*000067ac*/ MOV         R0, R1 
    /*000067b0*/ STR         R3, [SP] 
    /*000067b4*/ MOVT        R2, #0                      @ .LC149 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e44] 
    /*000067b8*/ B           RgnSetBitMap_x670c
    RgnSetBitMap_x67bc:
    /*000067bc*/ LDR         IP, [SP, #60]               @ 0x3c 
    /*000067c0*/ MOV         R1, #3 
    /*000067c4*/ STR         R7, [R4, #224]              @ 0xe0 
    /*000067c8*/ MOV         R0, R1 
    /*000067cc*/ MOVW        R3, #3794                   @ 0xed2 
    /*000067d0*/ MOVW        R2, #0                      @ .LC147 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1da8] 
    /*000067d4*/ STR         R3, [SP] 
    /*000067d8*/ MOVT        R2, #0                      @ .LC147 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1da8] 
    /*000067dc*/ STR         IP, [SP, #4] 
    /*000067e0*/ MOVW        R7, #32783                  @ 0x800f 
    /*000067e4*/ LDR         R3, [PC, #28]               @ 0x0000000000006808 
    /*000067e8*/ MOVT        R7, #40963                  @ 0xa003 
    /*000067ec*/ BL          0x00000000000067EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000067f0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000067f4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000067f8*/ BL          0x00000000000067F8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000067fc*/ B           RgnSetBitMap_x6290
    RgnSetBitMap_x6800:
    /*00006800*/ STR         R3, [R4, #224]              @ 0xe0 
    /*00006804*/ B           RgnSetBitMap_x6608
    /*00006808*/ ANDEQ       R0, R0, IP, ASR R4 
    /*0000680c*/ ANDEQ       R0, R0, R4 
FUNC_END RgnSetBitMap


FUNC_BEGIN RgnVgsQuickCopy
    /*00006810*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00006814*/ SUBS        R4, R1, #0 
    /*00006818*/ SUB         SP, SP, #100                @ 0x64 
    /*0000681c*/ MOV         R6, R0 
    /*00006820*/ BEQ         RgnVgsQuickCopy_x6a7c
    RgnVgsQuickCopy_x6824:
    /*00006824*/ LDRD        R8, [R4, #248]              @ 0xf8 
    /*00006828*/ ADD         R7, SP, #32 
    /*0000682c*/ LDR         R3, [R4, #272]              @ 0x110 
    /*00006830*/ MOV         R2, #64                     @ 0x40 
    /*00006834*/ MOV         R1, #0 
    /*00006838*/ MOV         R0, R7 
    /*0000683c*/ ADD         R3, R3, #1 
    /*00006840*/ STR         R3, [R4, #272]              @ 0x110 
    /*00006844*/ STRD        R8, [SP, #16] 
    /*00006848*/ MOV         R5, #1 
    /*0000684c*/ LDR         FP, [R4, #212]              @ 0xd4 
    /*00006850*/ LDR         R9, [R4, #240]              @ 0xf0 
    /*00006854*/ LDR         R8, [R4, #216]              @ 0xd8 
    /*00006858*/ BL          0x0000000000006858          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000685c*/ MOV         R0, #45                     @ 0x2d 
    /*00006860*/ STR         R5, [SP, #84]               @ 0x54 
    /*00006864*/ BL          0x0000000000006864          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00006868*/ MOV         R3, #0 
    /*0000686c*/ STR         R7, [SP, #4] 
    /*00006870*/ MOV         R1, R5 
    /*00006874*/ STR         R3, [SP] 
    /*00006878*/ MOV         R2, #3 
    /*0000687c*/ LDR         R5, [R0] 
    /*00006880*/ ADD         R0, SP, #28 
    /*00006884*/ BLX         R5 
    /*00006888*/ SUBS        R5, R0, #0 
    /*0000688c*/ BNE         RgnVgsQuickCopy_x6a1c
    /*00006890*/ MOV         R0, #45                     @ 0x2d 
    /*00006894*/ BL          0x0000000000006894          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00006898*/ LDR         R3, [R0, #64]               @ 0x40 
    /*0000689c*/ BLX         R3 
    /*000068a0*/ SUBS        R5, R0, #0 
    /*000068a4*/ BEQ         RgnVgsQuickCopy_x6a9c
    /*000068a8*/ LDR         R3, [R4, #120]              @ 0x78 
    /*000068ac*/ LDR         SL, [R4, #24] 
    /*000068b0*/ CMP         R3, #11 
    /*000068b4*/ LSLEQ       SL, SL, #2 
    /*000068b8*/ BEQ         RgnVgsQuickCopy_x68d4
    /*000068bc*/ CMP         R3, #12 
    /*000068c0*/ ADDEQ       SL, SL, #3 
    /*000068c4*/ LSLNE       SL, SL, #1 
    /*000068c8*/ LSREQ       SL, SL, #2 
    /*000068cc*/ ADDEQ       SL, SL, #1 
    /*000068d0*/ BICEQ       SL, SL, #1 
    RgnVgsQuickCopy_x68d4:
    /*000068d4*/ MOV         R2, #744                    @ 0x2e8 
    /*000068d8*/ MOV         R1, #0 
    /*000068dc*/ MOV         R0, R5 
    /*000068e0*/ MOV         R7, #3 
    /*000068e4*/ BL          0x00000000000068E4          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000068e8*/ STR         SL, [R5] 
    /*000068ec*/ LDR         R3, [R4, #28] 
    /*000068f0*/ ADD         IP, R5, #464                @ 0x1d0 
    /*000068f4*/ MUL         R1, R8, R9 
    /*000068f8*/ MOV         LR, #0 
    /*000068fc*/ STR         R3, [R5, #4] 
    /*00006900*/ MOV         R0, #45                     @ 0x2d 
    /*00006904*/ LDRD        R2, [SP, #16] 
    /*00006908*/ UMLAL       R2, R3, R9, FP 
    /*0000690c*/ LDRD        R8, [SP, #16] 
    /*00006910*/ ADDS        R8, R8, R1 
    /*00006914*/ LDR         R1, [R4, #120]              @ 0x78 
    /*00006918*/ ADC         R9, R9, #0 
    /*0000691c*/ STRD        R8, [R5, #120]              @ 0x78 
    /*00006920*/ STR         R1, [R5, #12] 
    /*00006924*/ LDR         R1, [R4, #268]              @ 0x10c 
    /*00006928*/ STR         SL, [R5, #336]              @ 0x150 
    /*0000692c*/ STR         R1, [R5, #44]               @ 0x2c 
    /*00006930*/ LDR         R1, [R4, #28] 
    /*00006934*/ STR         R1, [R5, #340]              @ 0x154 
    /*00006938*/ LDR         R1, [R4, #120]              @ 0x78 
    /*0000693c*/ STR         R1, [R5, #348]              @ 0x15c 
    /*00006940*/ STRD        R2, [IP, #-8] 
    /*00006944*/ LDR         R3, [R4, #268]              @ 0x10c 
    /*00006948*/ STR         LR, [R5, #728]              @ 0x2d8 
    /*0000694c*/ STR         LR, [R5, #732]              @ 0x2dc 
    /*00006950*/ STR         R3, [R5, #380]              @ 0x17c 
    /*00006954*/ STR         R7, [R5, #724]              @ 0x2d4 
    /*00006958*/ BL          0x0000000000006958          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000695c*/ MOV         R1, R5 
    /*00006960*/ LDR         R3, [R0, #40]               @ 0x28 
    /*00006964*/ LDR         R0, [SP, #28] 
    /*00006968*/ BLX         R3 
    /*0000696c*/ SUBS        R5, R0, #0 
    /*00006970*/ BNE         RgnVgsQuickCopy_x69c0
    /*00006974*/ LDR         R2, [R4, #284]              @ 0x11c 
    /*00006978*/ MOV         R0, #45                     @ 0x2d 
    /*0000697c*/ LDR         R3, [R4, #276]              @ 0x114 
    /*00006980*/ ADD         R2, R2, #1 
    /*00006984*/ STR         R2, [R4, #284]              @ 0x11c 
    /*00006988*/ ADD         R3, R3, #1 
    /*0000698c*/ STR         R3, [R4, #276]              @ 0x114 
    /*00006990*/ BL          0x0000000000006990          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00006994*/ LDR         R3, [R0, #52]               @ 0x34 
    /*00006998*/ LDR         R0, [SP, #28] 
    /*0000699c*/ BLX         R3 
    /*000069a0*/ SUBS        R5, R0, #0 
    /*000069a4*/ BNE         RgnVgsQuickCopy_x6a58
    /*000069a8*/ LDR         R3, [R4, #292]              @ 0x124 
    /*000069ac*/ ADD         R3, R3, #1 
    /*000069b0*/ STR         R3, [R4, #292]              @ 0x124 
    RgnVgsQuickCopy_x69b4:
    /*000069b4*/ MOV         R0, R5 
    /*000069b8*/ ADD         SP, SP, #100                @ 0x64 
    /*000069bc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnVgsQuickCopy_x69c0:
    /*000069c0*/ LDR         LR, [SP, #28] 
    /*000069c4*/ MOVW        R2, #0                      @ .LC158 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x209c] 
    /*000069c8*/ LDR         R3, [PC, #268]              @ 0x0000000000006adc 
    /*000069cc*/ MOVT        R2, #0                      @ .LC158 [R_ARM_MOVT_ABS: .rodata.str1.4+0x209c] 
    /*000069d0*/ LDR         R1, [R4, #288]              @ 0x120 
    /*000069d4*/ MOVW        IP, #4004                   @ 0xfa4 
    /*000069d8*/ ADD         R1, R1, #1 
    /*000069dc*/ STR         R1, [R4, #288]              @ 0x120 
    RgnVgsQuickCopy_x69e0:
    /*000069e0*/ MOV         R1, R7 
    /*000069e4*/ STR         LR, [SP, #8] 
    /*000069e8*/ STR         IP, [SP] 
    /*000069ec*/ MOV         R0, R7 
    /*000069f0*/ STR         R5, [SP, #12] 
    /*000069f4*/ STR         R6, [SP, #4] 
    /*000069f8*/ BL          0x00000000000069F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000069fc*/ MOV         R0, #45                     @ 0x2d 
    /*00006a00*/ BL          0x0000000000006A00          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00006a04*/ LDR         R3, [R0, #4] 
    /*00006a08*/ LDR         R0, [SP, #28] 
    /*00006a0c*/ BLX         R3 
    /*00006a10*/ MOV         R0, R5 
    /*00006a14*/ ADD         SP, SP, #100                @ 0x64 
    /*00006a18*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnVgsQuickCopy_x6a1c:
    /*00006a1c*/ LDR         R3, [R4, #280]              @ 0x118 
    /*00006a20*/ MOV         R1, #3 
    /*00006a24*/ MOVW        IP, #3954                   @ 0xf72 
    /*00006a28*/ MOV         R0, R1 
    /*00006a2c*/ ADD         R3, R3, #1 
    /*00006a30*/ MOVW        R2, #0                      @ .LC156 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x201c] 
    /*00006a34*/ STR         R3, [R4, #280]              @ 0x118 
    /*00006a38*/ MOVT        R2, #0                      @ .LC156 [R_ARM_MOVT_ABS: .rodata.str1.4+0x201c] 
    /*00006a3c*/ STR         R6, [SP, #4] 
    /*00006a40*/ STR         IP, [SP] 
    /*00006a44*/ LDR         R3, [PC, #144]              @ 0x0000000000006adc 
    /*00006a48*/ BL          0x0000000000006A48          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006a4c*/ MOV         R0, R5 
    /*00006a50*/ ADD         SP, SP, #100                @ 0x64 
    /*00006a54*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnVgsQuickCopy_x6a58:
    /*00006a58*/ LDR         R1, [R4, #296]              @ 0x128 
    /*00006a5c*/ MOVW        R2, #0                      @ .LC159 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x20f4] 
    /*00006a60*/ LDR         LR, [SP, #28] 
    /*00006a64*/ MOVW        IP, #4018                   @ 0xfb2 
    /*00006a68*/ ADD         R1, R1, #1 
    /*00006a6c*/ MOVT        R2, #0                      @ .LC159 [R_ARM_MOVT_ABS: .rodata.str1.4+0x20f4] 
    /*00006a70*/ LDR         R3, [PC, #100]              @ 0x0000000000006adc 
    /*00006a74*/ STR         R1, [R4, #296]              @ 0x128 
    /*00006a78*/ B           RgnVgsQuickCopy_x69e0
    RgnVgsQuickCopy_x6a7c:
    /*00006a7c*/ MOVW        R3, #0                      @ .LC139 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b9c] 
    /*00006a80*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00006a84*/ MOVT        R3, #0                      @ .LC139 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b9c] 
    /*00006a88*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00006a8c*/ MOVW        R2, #3933                   @ 0xf5d 
    /*00006a90*/ LDR         R1, [PC, #68]               @ 0x0000000000006adc 
    /*00006a94*/ BL          0x0000000000006A94          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00006a98*/ B           RgnVgsQuickCopy_x6824
    RgnVgsQuickCopy_x6a9c:
    /*00006a9c*/ MOV         R0, #45                     @ 0x2d 
    /*00006aa0*/ MVN         R5, #0 
    /*00006aa4*/ BL          0x0000000000006AA4          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00006aa8*/ LDR         R3, [R0, #4] 
    /*00006aac*/ LDR         R0, [SP, #28] 
    /*00006ab0*/ BLX         R3 
    /*00006ab4*/ MOV         R1, #3 
    /*00006ab8*/ MOVW        R3, #3963                   @ 0xf7b 
    /*00006abc*/ STR         R6, [SP, #4] 
    /*00006ac0*/ STR         R3, [SP] 
    /*00006ac4*/ MOV         R0, R1 
    /*00006ac8*/ MOVW        R2, #0                      @ .LC157 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2060] 
    /*00006acc*/ LDR         R3, [PC, #8]                @ 0x0000000000006adc 
    /*00006ad0*/ MOVT        R2, #0                      @ .LC157 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2060] 
    /*00006ad4*/ BL          0x0000000000006AD4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006ad8*/ B           RgnVgsQuickCopy_x69b4
    /*00006adc*/ ANDEQ       R0, R0, IP, ROR #8 
FUNC_END RgnVgsQuickCopy


FUNC_BEGIN RgnIsHandleInBatch
    /*00006ae0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006ae4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006ae8*/ ADD         R0, R3, R0, LSL #2 
    /*00006aec*/ LDR         R3, [R0, #8] 
    /*00006af0*/ LDR         R0, [R3, #208]              @ 0xd0 
    /*00006af4*/ ADDS        R0, R0, #0 
    /*00006af8*/ MOVNE       R0, #1 
    /*00006afc*/ BX          LR 
FUNC_END RgnIsHandleInBatch


FUNC_BEGIN RgnGetCanvasInfo
    /*00006b00*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00006b04*/ CMP         R0, #127                    @ 0x7f 
    /*00006b08*/ SUB         SP, SP, #36                 @ 0x24 
    /*00006b0c*/ MOV         R5, R1 
    /*00006b10*/ MOV         R7, R2 
    /*00006b14*/ MOV         R6, R3 
    /*00006b18*/ STR         R0, [SP, #20] 
    /*00006b1c*/ LDR         R8, [SP, #72]               @ 0x48 
    /*00006b20*/ BHI         RgnGetCanvasInfo_x6cc0
    RgnGetCanvasInfo_x6b24:
    /*00006b24*/ MOVW        FP, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006b28*/ MOVT        FP, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006b2c*/ MOV         R0, FP 
    /*00006b30*/ BL          0x0000000000006B30          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00006b34*/ SUBS        SL, R0, #0 
    /*00006b38*/ BNE         RgnGetCanvasInfo_x6f58
    /*00006b3c*/ LDR         R2, [SP, #20] 
    /*00006b40*/ ADD         R3, FP, R2, LSL #2 
    /*00006b44*/ LDR         R4, [R3, #8] 
    /*00006b48*/ CMP         R4, #0 
    /*00006b4c*/ BEQ         RgnGetCanvasInfo_x6e64
    /*00006b50*/ LDR         R3, [R4] 
    /*00006b54*/ CMP         R3, #0 
    /*00006b58*/ BEQ         RgnGetCanvasInfo_x6fa4
    /*00006b5c*/ LDR         R3, [R4, #176]              @ 0xb0 
    /*00006b60*/ CMP         R3, #4 
    /*00006b64*/ BHI         RgnGetCanvasInfo_x6fc4
    /*00006b68*/ CMP         R3, #3 
    /*00006b6c*/ CMPNE       R3, #0 
    /*00006b70*/ MOVNE       R0, #1 
    /*00006b74*/ MOVEQ       R0, #0 
    /*00006b78*/ BNE         RgnGetCanvasInfo_x6ffc
    /*00006b7c*/ LDRD        R2, [R4, #248]              @ 0xf8 
    /*00006b80*/ MOV         R1, #1 
    /*00006b84*/ STR         R1, [R4, #228]              @ 0xe4 
    /*00006b88*/ ORRS        R3, R2, R3 
    /*00006b8c*/ BEQ         RgnGetCanvasInfo_x7030
    /*00006b90*/ LDR         R3, [R4, #116]              @ 0x74 
    /*00006b94*/ CMP         R3, #1 
    /*00006b98*/ BLS         RgnGetCanvasInfo_x6cd8
    /*00006b9c*/ MOV         R0, R4 
    /*00006ba0*/ BL          0x00000000000000B8 
    /*00006ba4*/ CMN         R0, #1 
    /*00006ba8*/ BNE         RgnGetCanvasInfo_x6e40
    /*00006bac*/ CMP         R8, #0 
    /*00006bb0*/ BEQ         RgnGetCanvasInfo_x707c
    /*00006bb4*/ ADD         R9, SP, #32 
    /*00006bb8*/ CMN         R8, #1 
    /*00006bbc*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00006bc0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006bc4*/ LDR         R2, [R9, #-12]! 
    /*00006bc8*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00006bcc*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006bd0*/ BEQ         RgnGetCanvasInfo_x6e9c
    /*00006bd4*/ ADD         R3, R3, R2, LSL #2 
    /*00006bd8*/ MOVW        SL, #0                      @ Rgn_WaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x1dc] 
    /*00006bdc*/ ADD         FP, R4, #304                @ 0x130 
    /*00006be0*/ MOVT        SL, #0                      @ Rgn_WaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x1dc] 
    /*00006be4*/ LDR         R3, [R3, #-1604]            @ 0xfffff9bc 
    /*00006be8*/ STR         R3, [SP, #16] 
    /*00006bec*/ BL          0x0000000000006BEC          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006bf0*/ MOV         R0, R9 
    /*00006bf4*/ BL          0x0000000000006BF4          @ Rgn_WaitConditionCallBack [R_ARM_CALL: .text+0x1dc] 
    /*00006bf8*/ B           RgnGetCanvasInfo_x6c10
    RgnGetCanvasInfo_x6bfc:
    /*00006bfc*/ BL          0x0000000000006BFC          @ osal_wait_timeout_uninterruptible [R_ARM_CALL: +0x0] 
    /*00006c00*/ ADD         R3, R0, #512                @ 0x200 
    /*00006c04*/ MOV         R8, R0 
    /*00006c08*/ BICS        R3, R3, #512                @ 0x200 
    /*00006c0c*/ BEQ         RgnGetCanvasInfo_x6c3c
    RgnGetCanvasInfo_x6c10:
    /*00006c10*/ MOV         R0, R9 
    /*00006c14*/ BL          0x0000000000006C14          @ Rgn_WaitConditionCallBack [R_ARM_CALL: .text+0x1dc] 
    /*00006c18*/ MOV         R3, R8 
    /*00006c1c*/ MOV         R2, R9 
    /*00006c20*/ MOV         R1, SL 
    /*00006c24*/ CMP         R0, #0 
    /*00006c28*/ MOV         R0, FP 
    /*00006c2c*/ BEQ         RgnGetCanvasInfo_x6bfc
    /*00006c30*/ MOV         R0, R8 
    /*00006c34*/ BL          0x0000000000006C34          @ osal_msecs_to_jiffies [R_ARM_CALL: +0x0] 
    /*00006c38*/ MOV         R8, R0 
    RgnGetCanvasInfo_x6c3c:
    /*00006c3c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006c40*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006c44*/ BL          0x0000000000006C44          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00006c48*/ SUBS        SL, R0, #0 
    /*00006c4c*/ BNE         RgnGetCanvasInfo_x6f58
    /*00006c50*/ LDR         R1, [SP, #16] 
    /*00006c54*/ LDR         R0, [SP, #20] 
    /*00006c58*/ BL          0x0000000000006C58          @ RgnAbnormalVerifyAfterWait [R_ARM_CALL: .text+0x2c24] 
    /*00006c5c*/ SUBS        R9, R0, #0 
    /*00006c60*/ BNE         RgnGetCanvasInfo_x6e50
    /*00006c64*/ CMP         R8, #0 
    /*00006c68*/ BEQ         RgnGetCanvasInfo_x7094
    /*00006c6c*/ BLT         RgnGetCanvasInfo_x70c0
    RgnGetCanvasInfo_x6c70:
    /*00006c70*/ LDR         R3, [R4] 
    /*00006c74*/ CMP         R3, #0 
    /*00006c78*/ BNE         RgnGetCanvasInfo_x6e40
    /*00006c7c*/ LDR         IP, [SP, #20] 
    /*00006c80*/ MOV         R1, #3 
    /*00006c84*/ MOVW        R2, #0                      @ .LC86 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1124] 
    /*00006c88*/ MOVT        R2, #0                      @ .LC86 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1124] 
    /*00006c8c*/ MOV         R0, R1 
    /*00006c90*/ MOVW        R9, #32773                  @ 0x8005 
    /*00006c94*/ STR         R3, [R4, #228]              @ 0xe4 
    /*00006c98*/ MOVW        R3, #4317                   @ 0x10dd 
    /*00006c9c*/ STR         IP, [SP, #4] 
    /*00006ca0*/ STR         R3, [SP] 
    RgnGetCanvasInfo_x6ca4:
    /*00006ca4*/ LDR         R3, [PC, #1052]             @ 0x00000000000070c8 
    /*00006ca8*/ MOVT        R9, #40963                  @ 0xa003 
    /*00006cac*/ BL          0x0000000000006CAC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006cb0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006cb4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006cb8*/ BL          0x0000000000006CB8          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006cbc*/ B           RgnGetCanvasInfo_x6ccc
    RgnGetCanvasInfo_x6cc0:
    /*00006cc0*/ BL          0x00000000000002FC 
    /*00006cc4*/ SUBS        R9, R0, #0 
    /*00006cc8*/ BEQ         RgnGetCanvasInfo_x6b24
    RgnGetCanvasInfo_x6ccc:
    /*00006ccc*/ MOV         R0, R9 
    /*00006cd0*/ ADD         SP, SP, #36                 @ 0x24 
    /*00006cd4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnGetCanvasInfo_x6cd8:
    /*00006cd8*/ BNE         RgnGetCanvasInfo_x6f6c
    /*00006cdc*/ STR         R0, [R4, #212]              @ 0xd4 
    RgnGetCanvasInfo_x6ce0:
    /*00006ce0*/ LDR         R0, [PC, #996]              @ 0x00000000000070cc 
    /*00006ce4*/ ADD         R1, SP, #28 
    /*00006ce8*/ BL          0x0000000000006CE8          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00006cec*/ MOV         R3, #1 
    /*00006cf0*/ ADD         R1, SP, #28 
    /*00006cf4*/ STR         R3, [R4, #220]              @ 0xdc 
    /*00006cf8*/ LDR         R0, [PC, #972]              @ 0x00000000000070cc 
    /*00006cfc*/ BL          0x0000000000006CFC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00006d00*/ LDR         R3, [R4, #116]              @ 0x74 
    /*00006d04*/ CMP         R3, #1 
    /*00006d08*/ BLS         RgnGetCanvasInfo_x6d68
    /*00006d0c*/ ADD         R3, R4, #256                @ 0x100 
    /*00006d10*/ LDR         R2, [R4, #240]              @ 0xf0 
    /*00006d14*/ LDR         R1, [R4, #212]              @ 0xd4 
    /*00006d18*/ LDRD        R8, [R3] 
    /*00006d1c*/ LDR         IP, [R4, #28] 
    /*00006d20*/ LDR         R3, [R4, #216]              @ 0xd8 
    /*00006d24*/ MOV         SL, R8 
    /*00006d28*/ MOV         FP, R9 
    /*00006d2c*/ UMLAL       SL, FP, R2, R1 
    /*00006d30*/ CMP         IP, #31 
    /*00006d34*/ UMLAL       R8, R9, R3, R2 
    /*00006d38*/ MOV         R0, SL 
    /*00006d3c*/ BLS         RgnGetCanvasInfo_x6d60
    /*00006d40*/ LDR         R3, [R4, #24] 
    /*00006d44*/ CMP         R3, #31 
    /*00006d48*/ BLS         RgnGetCanvasInfo_x6d60
    /*00006d4c*/ CMP         IP, #120                    @ 0x78 
    /*00006d50*/ CMPLS       R3, #120                    @ 0x78 
    /*00006d54*/ MOVLS       SL, #1 
    /*00006d58*/ MOVHI       SL, #0 
    /*00006d5c*/ BHI         RgnGetCanvasInfo_x6df0
    RgnGetCanvasInfo_x6d60:
    /*00006d60*/ MOV         R1, R8 
    /*00006d64*/ BL          0x0000000000006D64          @ osal_memcpy [R_ARM_CALL: +0x0] 
    RgnGetCanvasInfo_x6d68:
    /*00006d68*/ LDR         IP, [R4, #212]              @ 0xd4 
    /*00006d6c*/ ADD         R1, SP, #28 
    /*00006d70*/ LDR         R0, [R4, #240]              @ 0xf0 
    /*00006d74*/ MOV         R8, #0 
    /*00006d78*/ LDRD        R2, [R4, #248]              @ 0xf8 
    /*00006d7c*/ MOV         R9, R8 
    /*00006d80*/ UMLAL       R2, R3, R0, IP 
    /*00006d84*/ LDR         R0, [PC, #832]              @ 0x00000000000070cc 
    /*00006d88*/ STRD        R2, [R5] 
    /*00006d8c*/ LDR         R3, [R4, #268]              @ 0x10c 
    /*00006d90*/ STR         R3, [R5, #24] 
    /*00006d94*/ LDR         R3, [R4, #120]              @ 0x78 
    /*00006d98*/ STR         R3, [R5, #28] 
    /*00006d9c*/ LDR         R3, [R4, #24] 
    /*00006da0*/ STR         R3, [R5, #16] 
    /*00006da4*/ LDR         R3, [R4, #28] 
    /*00006da8*/ STR         R3, [R5, #20] 
    /*00006dac*/ LDR         R3, [R4, #240]              @ 0xf0 
    /*00006db0*/ STR         R3, [R7] 
    /*00006db4*/ LDR         R3, [R4, #212]              @ 0xd4 
    /*00006db8*/ STR         R3, [R6] 
    /*00006dbc*/ BL          0x0000000000006DBC          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00006dc0*/ MOV         R3, #1 
    /*00006dc4*/ ADD         R1, SP, #28 
    /*00006dc8*/ STR         R3, [R4, #232]              @ 0xe8 
    /*00006dcc*/ STR         R8, [R4, #236]              @ 0xec 
    /*00006dd0*/ LDR         R0, [PC, #756]              @ 0x00000000000070cc 
    /*00006dd4*/ BL          0x0000000000006DD4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00006dd8*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006ddc*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006de0*/ BL          0x0000000000006DE0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006de4*/ MOV         R0, R9 
    /*00006de8*/ ADD         SP, SP, #36                 @ 0x24 
    /*00006dec*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnGetCanvasInfo_x6df0:
    /*00006df0*/ MOV         R1, R4 
    /*00006df4*/ LDR         R0, [SP, #20] 
    /*00006df8*/ BL          0x0000000000006DF8          @ RgnVgsQuickCopy [R_ARM_CALL: .text+0x6810] 
    /*00006dfc*/ SUBS        R9, R0, #0 
    /*00006e00*/ BEQ         RgnGetCanvasInfo_x6d68
    /*00006e04*/ LDR         IP, [SP, #20] 
    /*00006e08*/ MOV         R1, #3 
    /*00006e0c*/ STR         SL, [R4, #228]              @ 0xe4 
    /*00006e10*/ MOV         R0, R1 
    /*00006e14*/ MOVW        R3, #4364                   @ 0x110c 
    /*00006e18*/ MOVW        R2, #0                      @ .LC164 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x222c] 
    /*00006e1c*/ STR         R3, [SP] 
    /*00006e20*/ MOVT        R2, #0                      @ .LC164 [R_ARM_MOVT_ABS: .rodata.str1.4+0x222c] 
    /*00006e24*/ STR         IP, [SP, #4] 
    /*00006e28*/ LDR         R3, [PC, #664]              @ 0x00000000000070c8 
    /*00006e2c*/ BL          0x0000000000006E2C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006e30*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006e34*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006e38*/ BL          0x0000000000006E38          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006e3c*/ B           RgnGetCanvasInfo_x6ccc
    RgnGetCanvasInfo_x6e40:
    /*00006e40*/ MOV         R0, R4 
    /*00006e44*/ BL          0x00000000000000B8 
    /*00006e48*/ STR         R0, [R4, #212]              @ 0xd4 
    /*00006e4c*/ B           RgnGetCanvasInfo_x6ce0
    RgnGetCanvasInfo_x6e50:
    /*00006e50*/ STR         SL, [R4, #228]              @ 0xe4 
    /*00006e54*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006e58*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006e5c*/ BL          0x0000000000006E5C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006e60*/ B           RgnGetCanvasInfo_x6ccc
    RgnGetCanvasInfo_x6e64:
    /*00006e64*/ MOV         R1, #3 
    /*00006e68*/ STR         R2, [SP, #4] 
    /*00006e6c*/ MOVW        R2, #0                      @ .LC160 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2144] 
    /*00006e70*/ MOVT        R2, #0                      @ .LC160 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2144] 
    /*00006e74*/ MOV         R0, R1 
    /*00006e78*/ MOVW        R3, #4205                   @ 0x106d 
    /*00006e7c*/ STR         R3, [SP] 
    RgnGetCanvasInfo_x6e80:
    /*00006e80*/ LDR         R3, [PC, #576]              @ 0x00000000000070c8 
    /*00006e84*/ MOVW        R9, #32773                  @ 0x8005 
    /*00006e88*/ BL          0x0000000000006E88          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006e8c*/ MOV         R0, FP 
    /*00006e90*/ BL          0x0000000000006E90          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006e94*/ MOVT        R9, #40963                  @ 0xa003 
    /*00006e98*/ B           RgnGetCanvasInfo_x6ccc
    RgnGetCanvasInfo_x6e9c:
    /*00006e9c*/ ADD         R3, R3, R2, LSL #2 
    /*00006ea0*/ MOVW        R8, #0                      @ Rgn_WaitConditionCallBack [R_ARM_MOVW_ABS_NC: .text+0x1dc] 
    /*00006ea4*/ ADD         FP, R4, #304                @ 0x130 
    /*00006ea8*/ MOVT        R8, #0                      @ Rgn_WaitConditionCallBack [R_ARM_MOVT_ABS: .text+0x1dc] 
    /*00006eac*/ LDR         R3, [R3, #-1604]            @ 0xfffff9bc 
    /*00006eb0*/ STR         R3, [SP, #16] 
    /*00006eb4*/ BL          0x0000000000006EB4          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006eb8*/ B           RgnGetCanvasInfo_x6ec8
    RgnGetCanvasInfo_x6ebc:
    /*00006ebc*/ BL          0x0000000000006EBC          @ osal_wait_uninterruptible [R_ARM_CALL: +0x0] 
    /*00006ec0*/ CMP         R0, #0 
    /*00006ec4*/ BLT         RgnGetCanvasInfo_x6f64
    RgnGetCanvasInfo_x6ec8:
    /*00006ec8*/ MOV         R0, R9 
    /*00006ecc*/ BL          0x0000000000006ECC          @ Rgn_WaitConditionCallBack [R_ARM_CALL: .text+0x1dc] 
    /*00006ed0*/ MOV         R2, R9 
    /*00006ed4*/ MOV         R1, R8 
    /*00006ed8*/ CMP         R0, #0 
    /*00006edc*/ MOV         R0, FP 
    /*00006ee0*/ BEQ         RgnGetCanvasInfo_x6ebc
    RgnGetCanvasInfo_x6ee4:
    /*00006ee4*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006ee8*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006eec*/ BL          0x0000000000006EEC          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00006ef0*/ SUBS        R8, R0, #0 
    /*00006ef4*/ BNE         RgnGetCanvasInfo_x6f58
    /*00006ef8*/ LDR         R1, [SP, #16] 
    /*00006efc*/ LDR         R0, [SP, #20] 
    /*00006f00*/ BL          0x0000000000006F00          @ RgnAbnormalVerifyAfterWait [R_ARM_CALL: .text+0x2c24] 
    /*00006f04*/ SUBS        R9, R0, #0 
    /*00006f08*/ BNE         RgnGetCanvasInfo_x7068
    /*00006f0c*/ CMP         SL, #0 
    /*00006f10*/ BEQ         RgnGetCanvasInfo_x6c70
    /*00006f14*/ LDR         R3, [SP, #20] 
    /*00006f18*/ MOVW        R1, #4277                   @ 0x10b5 
    /*00006f1c*/ STR         R9, [R4, #228]              @ 0xe4 
    /*00006f20*/ MOVW        R2, #0                      @ .LC146 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d60] 
    /*00006f24*/ STR         R1, [SP] 
    /*00006f28*/ MOVT        R2, #0                      @ .LC146 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d60] 
    /*00006f2c*/ STR         R3, [SP, #4] 
    /*00006f30*/ MOV         R1, #3 
    /*00006f34*/ LDR         R3, [PC, #396]              @ 0x00000000000070c8 
    /*00006f38*/ MOV         R0, #4 
    /*00006f3c*/ BL          0x0000000000006F3C          @ HI_LOG [R_ARM_CALL: +0x0] 
    RgnGetCanvasInfo_x6f40:
    /*00006f40*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00006f44*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00006f48*/ MOV         R9, #65024                  @ 0xfe00 
    /*00006f4c*/ BL          0x0000000000006F4C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006f50*/ MOVT        R9, #65535                  @ 0xffff 
    /*00006f54*/ B           RgnGetCanvasInfo_x6ccc
    RgnGetCanvasInfo_x6f58:
    /*00006f58*/ MOV         R9, #65024                  @ 0xfe00 
    /*00006f5c*/ MOVT        R9, #65535                  @ 0xffff 
    /*00006f60*/ B           RgnGetCanvasInfo_x6ccc
    RgnGetCanvasInfo_x6f64:
    /*00006f64*/ MOV         SL, R0 
    /*00006f68*/ B           RgnGetCanvasInfo_x6ee4
    RgnGetCanvasInfo_x6f6c:
    /*00006f6c*/ MOV         R0, FP 
    /*00006f70*/ MOVW        R9, #32771                  @ 0x8003 
    /*00006f74*/ BL          0x0000000000006F74          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006f78*/ LDR         R3, [R4, #116]              @ 0x74 
    /*00006f7c*/ MOV         R1, #3 
    /*00006f80*/ MOVW        R2, #4332                   @ 0x10ec 
    /*00006f84*/ MOV         R0, R1 
    /*00006f88*/ MOVT        R9, #40963                  @ 0xa003 
    /*00006f8c*/ STM         SP, {R2, R3} 
    /*00006f90*/ MOVW        R2, #0                      @ .LC115 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15dc] 
    /*00006f94*/ LDR         R3, [PC, #300]              @ 0x00000000000070c8 
    /*00006f98*/ MOVT        R2, #0                      @ .LC115 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15dc] 
    /*00006f9c*/ BL          0x0000000000006F9C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006fa0*/ B           RgnGetCanvasInfo_x6ccc
    RgnGetCanvasInfo_x6fa4:
    /*00006fa4*/ MOV         R1, #3 
    /*00006fa8*/ STR         R2, [SP, #4] 
    /*00006fac*/ MOVW        R3, #4212                   @ 0x1074 
    /*00006fb0*/ MOVW        R2, #0                      @ .LC86 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1124] 
    /*00006fb4*/ MOV         R0, R1 
    /*00006fb8*/ STR         R3, [SP] 
    /*00006fbc*/ MOVT        R2, #0                      @ .LC86 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1124] 
    /*00006fc0*/ B           RgnGetCanvasInfo_x6e80
    RgnGetCanvasInfo_x6fc4:
    /*00006fc4*/ MOV         R1, #3 
    /*00006fc8*/ STMIB       SP, {R2, R3} 
    /*00006fcc*/ MOV         R0, R1 
    /*00006fd0*/ MOVW        R3, #4222                   @ 0x107e 
    /*00006fd4*/ MOVW        R2, #0                      @ .LC161 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2178] 
    /*00006fd8*/ STR         R3, [SP] 
    /*00006fdc*/ MOVT        R2, #0                      @ .LC161 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2178] 
    /*00006fe0*/ LDR         R3, [PC, #224]              @ 0x00000000000070c8 
    /*00006fe4*/ BL          0x0000000000006FE4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006fe8*/ MOV         R0, FP 
    /*00006fec*/ MOVW        R9, #32776                  @ 0x8008 
    /*00006ff0*/ BL          0x0000000000006FF0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00006ff4*/ MOVT        R9, #40963                  @ 0xa003 
    /*00006ff8*/ B           RgnGetCanvasInfo_x6ccc
    RgnGetCanvasInfo_x6ffc:
    /*00006ffc*/ MOV         R1, #3 
    /*00007000*/ MOVW        R3, #4231                   @ 0x1087 
    /*00007004*/ MOV         R0, R1 
    /*00007008*/ STR         R3, [SP] 
    /*0000700c*/ MOVW        R2, #0                      @ .LC162 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21bc] 
    /*00007010*/ LDR         R3, [PC, #176]              @ 0x00000000000070c8 
    /*00007014*/ MOVT        R2, #0                      @ .LC162 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21bc] 
    /*00007018*/ MOVW        R9, #32776                  @ 0x8008 
    /*0000701c*/ BL          0x000000000000701C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007020*/ MOV         R0, FP 
    /*00007024*/ BL          0x0000000000007024          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007028*/ MOVT        R9, #40963                  @ 0xa003 
    /*0000702c*/ B           RgnGetCanvasInfo_x6ccc
    RgnGetCanvasInfo_x7030:
    /*00007030*/ MOV         R1, #3 
    /*00007034*/ STR         R0, [R4, #228]              @ 0xe4 
    /*00007038*/ MOVW        R3, #4243                   @ 0x1093 
    /*0000703c*/ MOV         R0, R1 
    /*00007040*/ STR         R3, [SP] 
    /*00007044*/ MOVW        R2, #0                      @ .LC163 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21f0] 
    /*00007048*/ LDR         R3, [PC, #120]              @ 0x00000000000070c8 
    /*0000704c*/ MOVT        R2, #0                      @ .LC163 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21f0] 
    /*00007050*/ BL          0x0000000000007050          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007054*/ MOV         R0, FP 
    /*00007058*/ MOVW        R9, #32777                  @ 0x8009 
    /*0000705c*/ BL          0x000000000000705C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007060*/ MOVT        R9, #40963                  @ 0xa003 
    /*00007064*/ B           RgnGetCanvasInfo_x6ccc
    RgnGetCanvasInfo_x7068:
    /*00007068*/ STR         R8, [R4, #228]              @ 0xe4 
    /*0000706c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00007070*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00007074*/ BL          0x0000000000007074          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007078*/ B           RgnGetCanvasInfo_x6ccc
    RgnGetCanvasInfo_x707c:
    /*0000707c*/ STR         R8, [R4, #228]              @ 0xe4 
    /*00007080*/ MOV         R0, FP 
    /*00007084*/ MOVW        R9, #32783                  @ 0x800f 
    /*00007088*/ BL          0x0000000000007088          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000708c*/ MOVT        R9, #40963                  @ 0xa003 
    /*00007090*/ B           RgnGetCanvasInfo_x6ccc
    RgnGetCanvasInfo_x7094:
    /*00007094*/ LDR         IP, [SP, #20] 
    /*00007098*/ MOV         R1, #3 
    /*0000709c*/ STR         R9, [R4, #228]              @ 0xe4 
    /*000070a0*/ MOVW        R3, #4301                   @ 0x10cd 
    /*000070a4*/ MOVW        R2, #0                      @ .LC147 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1da8] 
    /*000070a8*/ MOV         R0, R1 
    /*000070ac*/ STR         IP, [SP, #4] 
    /*000070b0*/ MOVT        R2, #0                      @ .LC147 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1da8] 
    /*000070b4*/ STR         R3, [SP] 
    /*000070b8*/ MOVW        R9, #32783                  @ 0x800f 
    /*000070bc*/ B           RgnGetCanvasInfo_x6ca4
    RgnGetCanvasInfo_x70c0:
    /*000070c0*/ STR         R9, [R4, #228]              @ 0xe4 
    /*000070c4*/ B           RgnGetCanvasInfo_x6f40
    /*000070c8*/ ANDEQ       R0, R0, IP, ROR R4 
    /*000070cc*/ ANDEQ       R0, R0, R4 
FUNC_END RgnGetCanvasInfo


FUNC_BEGIN RgnPutCanvasInfo
    /*000070d0*/ CMP         R0, #127                    @ 0x7f 
    /*000070d4*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000070d8*/ MOV         R6, R0 
    /*000070dc*/ SUB         SP, SP, #20 
    /*000070e0*/ BHI         RgnPutCanvasInfo_x7144
    RgnPutCanvasInfo_x70e4:
    /*000070e4*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000070e8*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000070ec*/ MOV         R0, R4 
    /*000070f0*/ BL          0x00000000000070F0          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000070f4*/ SUBS        R5, R0, #0 
    /*000070f8*/ BNE         RgnPutCanvasInfo_x7190
    /*000070fc*/ ADD         R3, R4, R6, LSL #2 
    /*00007100*/ LDR         R7, [R3, #8] 
    /*00007104*/ CMP         R7, #0 
    /*00007108*/ BEQ         RgnPutCanvasInfo_x7158
    /*0000710c*/ ADD         R0, R4, #4 
    /*00007110*/ ADD         R1, SP, #12 
    /*00007114*/ BL          0x0000000000007114          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00007118*/ ADD         R1, SP, #12 
    /*0000711c*/ ADD         R0, R4, #4 
    /*00007120*/ STR         R5, [R7, #232]              @ 0xe8 
    /*00007124*/ STR         R5, [R7, #220]              @ 0xdc 
    /*00007128*/ BL          0x0000000000007128          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000712c*/ MOV         R0, R4 
    /*00007130*/ STR         R5, [R7, #228]              @ 0xe4 
    /*00007134*/ BL          0x0000000000007134          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007138*/ MOV         R0, R5 
    RgnPutCanvasInfo_x713c:
    /*0000713c*/ ADD         SP, SP, #20 
    /*00007140*/ POP         {R4, R5, R6, R7, PC} 
    RgnPutCanvasInfo_x7144:
    /*00007144*/ BL          0x00000000000002FC 
    /*00007148*/ CMP         R0, #0 
    /*0000714c*/ BEQ         RgnPutCanvasInfo_x70e4
    /*00007150*/ ADD         SP, SP, #20 
    /*00007154*/ POP         {R4, R5, R6, R7, PC} 
    RgnPutCanvasInfo_x7158:
    /*00007158*/ MOV         R1, #3 
    /*0000715c*/ MOVW        R3, #4408                   @ 0x1138 
    /*00007160*/ MOV         R0, R1 
    /*00007164*/ STR         R3, [SP] 
    /*00007168*/ STR         R6, [SP, #4] 
    /*0000716c*/ MOVW        R2, #0                      @ .LC160 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2144] 
    /*00007170*/ LDR         R3, [PC, #36]               @ 0x000000000000719c 
    /*00007174*/ MOVT        R2, #0                      @ .LC160 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2144] 
    /*00007178*/ BL          0x0000000000007178          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000717c*/ MOV         R0, R4 
    /*00007180*/ BL          0x0000000000007180          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007184*/ MOVW        R0, #32773                  @ 0x8005 
    /*00007188*/ MOVT        R0, #40963                  @ 0xa003 
    /*0000718c*/ B           RgnPutCanvasInfo_x713c
    RgnPutCanvasInfo_x7190:
    /*00007190*/ MOV         R0, #65024                  @ 0xfe00 
    /*00007194*/ MOVT        R0, #65535                  @ 0xffff 
    /*00007198*/ B           RgnPutCanvasInfo_x713c
    /*0000719c*/ MULEQ       R0, R0, R4 
FUNC_END RgnPutCanvasInfo


FUNC_BEGIN RgnUpdateCanvas
    /*000071a0*/ CMP         R0, #127                    @ 0x7f 
    /*000071a4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000071a8*/ MOV         R7, R0 
    /*000071ac*/ SUB         SP, SP, #20 
    /*000071b0*/ BHI         RgnUpdateCanvas_x7328
    RgnUpdateCanvas_x71b4:
    /*000071b4*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000071b8*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000071bc*/ MOV         R0, R4 
    /*000071c0*/ BL          0x00000000000071C0          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000071c4*/ CMP         R0, #0 
    /*000071c8*/ BNE         RgnUpdateCanvas_x7420
    /*000071cc*/ ADD         R3, R4, R7, LSL #2 
    /*000071d0*/ LDR         R5, [R3, #8] 
    /*000071d4*/ CMP         R5, #0 
    /*000071d8*/ BEQ         RgnUpdateCanvas_x73b0
    /*000071dc*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*000071e0*/ CMP         R3, #0 
    /*000071e4*/ BNE         RgnUpdateCanvas_x733c
    /*000071e8*/ LDR         R6, [R5, #176]              @ 0xb0 
    /*000071ec*/ CMP         R6, #4 
    /*000071f0*/ BHI         RgnUpdateCanvas_x7390
    RgnUpdateCanvas_x71f4:
    /*000071f4*/ CMP         R6, #3 
    /*000071f8*/ CMPNE       R6, #0 
    /*000071fc*/ BNE         RgnUpdateCanvas_x7464
    /*00007200*/ LDR         R3, [R5, #228]              @ 0xe4 
    /*00007204*/ CMP         R3, #0 
    /*00007208*/ BEQ         RgnUpdateCanvas_x74a0
    /*0000720c*/ LDR         R3, [R5, #232]              @ 0xe8 
    /*00007210*/ CMP         R3, #1 
    /*00007214*/ BNE         RgnUpdateCanvas_x73e8
    /*00007218*/ LDR         R3, [R5, #220]              @ 0xdc 
    /*0000721c*/ CMP         R3, #1 
    /*00007220*/ BNE         RgnUpdateCanvas_x73e8
    /*00007224*/ LDRD        R2, [R5, #248]              @ 0xf8 
    /*00007228*/ ORRS        R1, R2, R3 
    /*0000722c*/ BEQ         RgnUpdateCanvas_x742c
    /*00007230*/ LDR         R0, [R5, #240]              @ 0xf0 
    /*00007234*/ ADD         R1, R5, #256                @ 0x100 
    /*00007238*/ LDR         R8, [R5, #212]              @ 0xd4 
    /*0000723c*/ UMULL       R8, R9, R8, R0 
    /*00007240*/ LDR         R0, [PC, #660]              @ 0x00000000000074dc 
    /*00007244*/ ADDS        R2, R2, R8 
    /*00007248*/ ADC         R3, R3, R9 
    /*0000724c*/ STRD        R2, [R5, #96]               @ 0x60 
    /*00007250*/ LDRD        R2, [R1] 
    /*00007254*/ ADD         R1, SP, #12 
    /*00007258*/ ADDS        R8, R8, R2 
    /*0000725c*/ ADC         R9, R9, R3 
    /*00007260*/ STRD        R8, [R5, #104]              @ 0x68 
    /*00007264*/ MOV         R8, R5 
    /*00007268*/ BL          0x0000000000007268          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000726c*/ LDR         R9, [R8, #308]!             @ 0x134 
    /*00007270*/ CMP         R9, R8 
    /*00007274*/ LDR         R4, [R9] 
    /*00007278*/ MOVWNE      FP, #0                      @ .LC167 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x22b4] 
    /*0000727c*/ MOVWNE      SL, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00007280*/ MOVTNE      FP, #0                      @ .LC167 [R_ARM_MOVT_ABS: .rodata.str1.4+0x22b4] 
    /*00007284*/ MOVTNE      SL, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00007288*/ BNE         RgnUpdateCanvas_x7294
    /*0000728c*/ B           RgnUpdateCanvas_x72ec
    RgnUpdateCanvas_x7290:
    /*00007290*/ MOV         R4, R3 
    RgnUpdateCanvas_x7294:
    /*00007294*/ ADD         R9, R9, #8 
    /*00007298*/ MOV         R2, R7 
    /*0000729c*/ MOV         R0, R6 
    /*000072a0*/ MOV         R1, R9 
    /*000072a4*/ BL          0x00000000000072A4          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*000072a8*/ MOV         R1, R9 
    /*000072ac*/ SUBS        IP, R0, #0 
    /*000072b0*/ MOV         R0, R6 
    /*000072b4*/ BEQ         RgnUpdateCanvas_x72dc
    /*000072b8*/ LDRD        R2, [R5, #96]               @ 0x60 
    /*000072bc*/ STRD        R2, [IP, #88]               @ 0x58 
    /*000072c0*/ LDRD        R2, [R5, #104]              @ 0x68 
    /*000072c4*/ STRD        R2, [IP, #96]               @ 0x60 
    /*000072c8*/ BL          0x00000000000072C8          @ RgnGetChnGetInfo [R_ARM_CALL: .text+0x1034] 
    /*000072cc*/ SUBS        R9, R0, #0 
    /*000072d0*/ BEQ         RgnUpdateCanvas_x7378
    RgnUpdateCanvas_x72d4:
    /*000072d4*/ MOV         R3, #1 
    /*000072d8*/ STR         R3, [R9, #4] 
    RgnUpdateCanvas_x72dc:
    /*000072dc*/ CMP         R4, R8 
    /*000072e0*/ LDR         R3, [R4] 
    /*000072e4*/ MOV         R9, R4 
    /*000072e8*/ BNE         RgnUpdateCanvas_x7290
    RgnUpdateCanvas_x72ec:
    /*000072ec*/ LDR         R3, [R5, #212]              @ 0xd4 
    /*000072f0*/ MOV         R4, #0 
    /*000072f4*/ ADD         R1, SP, #12 
    /*000072f8*/ LDR         R0, [PC, #476]              @ 0x00000000000074dc 
    /*000072fc*/ STR         R4, [R5, #232]              @ 0xe8 
    /*00007300*/ STR         R3, [R5, #216]              @ 0xd8 
    /*00007304*/ STR         R4, [R5, #220]              @ 0xdc 
    /*00007308*/ BL          0x0000000000007308          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000730c*/ STR         R4, [R5, #228]              @ 0xe4 
    /*00007310*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00007314*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00007318*/ BL          0x0000000000007318          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000731c*/ MOV         R0, R4 
    RgnUpdateCanvas_x7320:
    /*00007320*/ ADD         SP, SP, #20 
    /*00007324*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnUpdateCanvas_x7328:
    /*00007328*/ BL          0x00000000000002FC 
    /*0000732c*/ CMP         R0, #0 
    /*00007330*/ BEQ         RgnUpdateCanvas_x71b4
    /*00007334*/ ADD         SP, SP, #20 
    /*00007338*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnUpdateCanvas_x733c:
    /*0000733c*/ MOV         R1, #3 
    /*00007340*/ STR         R7, [SP, #4] 
    /*00007344*/ MOVW        R3, #4454                   @ 0x1166 
    /*00007348*/ MOVW        R2, #0                      @ .LC168 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x22d0] 
    /*0000734c*/ STR         R3, [SP] 
    /*00007350*/ MOVT        R2, #0                      @ .LC168 [R_ARM_MOVT_ABS: .rodata.str1.4+0x22d0] 
    /*00007354*/ LDR         R3, [PC, #388]              @ 0x00000000000074e0 
    /*00007358*/ MOV         R0, #4 
    /*0000735c*/ BL          0x000000000000735C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007360*/ MOV         R0, R4 
    /*00007364*/ BL          0x0000000000007364          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007368*/ MOVW        R0, #32777                  @ 0x8009 
    /*0000736c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00007370*/ ADD         SP, SP, #20 
    /*00007374*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnUpdateCanvas_x7378:
    /*00007378*/ MOV         R3, FP 
    /*0000737c*/ MOVW        R2, #4517                   @ 0x11a5 
    /*00007380*/ LDR         R1, [PC, #344]              @ 0x00000000000074e0 
    /*00007384*/ MOV         R0, SL 
    /*00007388*/ BL          0x0000000000007388          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000738c*/ B           RgnUpdateCanvas_x72d4
    RgnUpdateCanvas_x7390:
    /*00007390*/ MOVW        R3, #0                      @ .LC165 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x226c] 
    /*00007394*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00007398*/ MOVT        R3, #0                      @ .LC165 [R_ARM_MOVT_ABS: .rodata.str1.4+0x226c] 
    /*0000739c*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000073a0*/ MOVW        R2, #4460                   @ 0x116c 
    /*000073a4*/ LDR         R1, [PC, #308]              @ 0x00000000000074e0 
    /*000073a8*/ BL          0x00000000000073A8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000073ac*/ B           RgnUpdateCanvas_x71f4
    RgnUpdateCanvas_x73b0:
    /*000073b0*/ MOV         R1, #3 
    /*000073b4*/ MOVW        R3, #4447                   @ 0x115f 
    /*000073b8*/ MOV         R0, R1 
    /*000073bc*/ STR         R3, [SP] 
    /*000073c0*/ STR         R7, [SP, #4] 
    /*000073c4*/ MOVW        R2, #0                      @ .LC160 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2144] 
    /*000073c8*/ LDR         R3, [PC, #272]              @ 0x00000000000074e0 
    /*000073cc*/ MOVT        R2, #0                      @ .LC160 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2144] 
    /*000073d0*/ BL          0x00000000000073D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000073d4*/ MOV         R0, R4 
    /*000073d8*/ BL          0x00000000000073D8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000073dc*/ MOVW        R0, #32773                  @ 0x8005 
    /*000073e0*/ MOVT        R0, #40963                  @ 0xa003 
    /*000073e4*/ B           RgnUpdateCanvas_x7320
    RgnUpdateCanvas_x73e8:
    /*000073e8*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000073ec*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000073f0*/ BL          0x00000000000073F0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000073f4*/ MOV         R1, #3 
    /*000073f8*/ MOVW        R3, #4486                   @ 0x1186 
    /*000073fc*/ MOV         R0, R1 
    /*00007400*/ STR         R3, [SP] 
    /*00007404*/ MOVW        R2, #0                      @ .LC166 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2280] 
    /*00007408*/ LDR         R3, [PC, #208]              @ 0x00000000000074e0 
    /*0000740c*/ MOVT        R2, #0                      @ .LC166 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2280] 
    /*00007410*/ BL          0x0000000000007410          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007414*/ MOVW        R0, #32777                  @ 0x8009 
    /*00007418*/ MOVT        R0, #40963                  @ 0xa003 
    /*0000741c*/ B           RgnUpdateCanvas_x7320
    RgnUpdateCanvas_x7420:
    /*00007420*/ MOV         R0, #65024                  @ 0xfe00 
    /*00007424*/ MOVT        R0, #65535                  @ 0xffff 
    /*00007428*/ B           RgnUpdateCanvas_x7320
    RgnUpdateCanvas_x742c:
    /*0000742c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00007430*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00007434*/ BL          0x0000000000007434          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007438*/ MOV         R1, #3 
    /*0000743c*/ MOVW        R3, #4494                   @ 0x118e 
    /*00007440*/ MOV         R0, R1 
    /*00007444*/ STR         R3, [SP] 
    /*00007448*/ MOVW        R2, #0                      @ .LC163 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21f0] 
    /*0000744c*/ LDR         R3, [PC, #140]              @ 0x00000000000074e0 
    /*00007450*/ MOVT        R2, #0                      @ .LC163 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21f0] 
    /*00007454*/ BL          0x0000000000007454          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007458*/ MOVW        R0, #32777                  @ 0x8009 
    /*0000745c*/ MOVT        R0, #40963                  @ 0xa003 
    /*00007460*/ B           RgnUpdateCanvas_x7320
    RgnUpdateCanvas_x7464:
    /*00007464*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00007468*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000746c*/ BL          0x000000000000746C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007470*/ MOV         R1, #3 
    /*00007474*/ MOVW        R3, #4470                   @ 0x1176 
    /*00007478*/ STR         R7, [SP, #4] 
    /*0000747c*/ STR         R3, [SP] 
    /*00007480*/ MOV         R0, R1 
    /*00007484*/ MOVW        R2, #0                      @ .LC150 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e74] 
    /*00007488*/ LDR         R3, [PC, #80]               @ 0x00000000000074e0 
    /*0000748c*/ MOVT        R2, #0                      @ .LC150 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e74] 
    /*00007490*/ BL          0x0000000000007490          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007494*/ MOVW        R0, #32776                  @ 0x8008 
    /*00007498*/ MOVT        R0, #40963                  @ 0xa003 
    /*0000749c*/ B           RgnUpdateCanvas_x7320
    RgnUpdateCanvas_x74a0:
    /*000074a0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000074a4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000074a8*/ BL          0x00000000000074A8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000074ac*/ MOV         R1, #3 
    /*000074b0*/ MOVW        R3, #4478                   @ 0x117e 
    /*000074b4*/ STR         R7, [SP, #4] 
    /*000074b8*/ STR         R3, [SP] 
    /*000074bc*/ MOV         R0, R1 
    /*000074c0*/ MOVW        R2, #0                      @ .LC151 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1eb0] 
    /*000074c4*/ LDR         R3, [PC, #20]               @ 0x00000000000074e0 
    /*000074c8*/ MOVT        R2, #0                      @ .LC151 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1eb0] 
    /*000074cc*/ BL          0x00000000000074CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000074d0*/ MOVW        R0, #32777                  @ 0x8009 
    /*000074d4*/ MOVT        R0, #40963                  @ 0xa003 
    /*000074d8*/ B           RgnUpdateCanvas_x7320
    /*000074dc*/ ANDEQ       R0, R0, R4 
    /*000074e0*/ ANDEQ       R0, R0, R4, LSR #9 
FUNC_END RgnUpdateCanvas


FUNC_BEGIN RgnCheckRgnMngNum
    /*000074e4*/ LDR         R2, [R2] 
    /*000074e8*/ CMP         R2, #8 
    /*000074ec*/ BEQ         RgnCheckRgnMngNum_x7508
    RgnCheckRgnMngNum_x74f0:
    /*000074f0*/ LDR         R0, [R0] 
    /*000074f4*/ LDR         R3, [R1, #316]              @ 0x13c 
    /*000074f8*/ CMP         R0, R3 
    /*000074fc*/ MOVCC       R0, #0 
    /*00007500*/ MOVCS       R0, #1 
    /*00007504*/ BX          LR 
    RgnCheckRgnMngNum_x7508:
    /*00007508*/ CMP         R3, #0 
    /*0000750c*/ BEQ         RgnCheckRgnMngNum_x7538
    /*00007510*/ CMP         R3, #1 
    /*00007514*/ BEQ         RgnCheckRgnMngNum_x7550
    /*00007518*/ CMP         R3, #2 
    /*0000751c*/ BNE         RgnCheckRgnMngNum_x74f0
    /*00007520*/ LDR         R0, [R0, #16] 
    /*00007524*/ LDR         R3, [R1, #316]              @ 0x13c 
    /*00007528*/ CMP         R0, R3 
    /*0000752c*/ MOVCC       R0, #0 
    /*00007530*/ MOVCS       R0, #1 
    /*00007534*/ BX          LR 
    RgnCheckRgnMngNum_x7538:
    /*00007538*/ LDR         R0, [R0, #8] 
    /*0000753c*/ LDR         R3, [R1, #316]              @ 0x13c 
    /*00007540*/ CMP         R0, R3 
    /*00007544*/ MOVCC       R0, #0 
    /*00007548*/ MOVCS       R0, #1 
    /*0000754c*/ BX          LR 
    RgnCheckRgnMngNum_x7550:
    /*00007550*/ LDR         R0, [R0, #12] 
    /*00007554*/ LDR         R3, [R1, #316]              @ 0x13c 
    /*00007558*/ CMP         R0, R3 
    /*0000755c*/ MOVCC       R0, #0 
    /*00007560*/ MOVCS       R0, #1 
    /*00007564*/ BX          LR 
FUNC_END RgnCheckRgnMngNum


FUNC_BEGIN RgnAttachToChn
    /*00007568*/ CMP         R0, #127                    @ 0x7f 
    /*0000756c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00007570*/ MOV         R6, R0 
    /*00007574*/ SUB         SP, SP, #204                @ 0xcc 
    /*00007578*/ MOV         R4, R1 
    /*0000757c*/ MOV         R5, R2 
    /*00007580*/ BHI         RgnAttachToChn_x7698
    RgnAttachToChn_x7584:
    /*00007584*/ CMP         R4, #0 
    /*00007588*/ BEQ         RgnAttachToChn_x76c4
    RgnAttachToChn_x758c:
    /*0000758c*/ CMP         R5, #0 
    /*00007590*/ BEQ         RgnAttachToChn_x76d4
    RgnAttachToChn_x7594:
    /*00007594*/ LDR         R0, [R5, #4] 
    /*00007598*/ CMP         R0, #4 
    /*0000759c*/ BHI         RgnAttachToChn_x76b0
    RgnAttachToChn_x75a0:
    /*000075a0*/ MOV         R2, R5 
    /*000075a4*/ MOV         R1, R4 
    /*000075a8*/ BL          0x00000000000075A8          @ RgnCheckCapacityByModId [R_ARM_CALL: .text+0x4a70] 
    /*000075ac*/ SUBS        R7, R0, #0 
    /*000075b0*/ BNE         RgnAttachToChn_x76a4
    /*000075b4*/ MOVW        R8, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000075b8*/ MOVT        R8, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000075bc*/ MOV         R0, R8 
    /*000075c0*/ BL          0x00000000000075C0          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000075c4*/ CMP         R0, #0 
    /*000075c8*/ BNE         RgnAttachToChn_x7998
    /*000075cc*/ LSL         R3, R6, #2 
    /*000075d0*/ STR         R3, [SP, #16] 
    /*000075d4*/ ADD         R3, R8, R3 
    /*000075d8*/ LDR         R3, [R3, #8] 
    /*000075dc*/ CMP         R3, #0 
    /*000075e0*/ BEQ         RgnAttachToChn_x7a28
    /*000075e4*/ LDR         SL, [R3, #176]              @ 0xb0 
    /*000075e8*/ LDR         R3, [R5, #4] 
    /*000075ec*/ CMP         SL, R3 
    /*000075f0*/ BNE         RgnAttachToChn_x7934
    /*000075f4*/ MOV         R1, R4 
    /*000075f8*/ MOV         R0, SL 
    /*000075fc*/ BL          0x00000000000075FC          @ RgnCheckMppChn [R_ARM_CALL: .text+0x13ac] 
    /*00007600*/ SUBS        R7, R0, #0 
    /*00007604*/ BNE         RgnAttachToChn_x76e4
    /*00007608*/ LDR         R1, [R4] 
    /*0000760c*/ MOV         R0, SL 
    /*00007610*/ BL          0x0000000000007610          @ RgnGetCapacity [R_ARM_CALL: .text+0x3668] 
    /*00007614*/ SUBS        R3, R0, #0 
    /*00007618*/ STR         R3, [SP, #20] 
    /*0000761c*/ BEQ         RgnAttachToChn_x79a4
    RgnAttachToChn_x7620:
    /*00007620*/ MOV         R1, R5 
    /*00007624*/ LDR         R0, [SP, #20] 
    /*00007628*/ BL          0x0000000000007628          @ RgnCheckCapacityByRgnChnAttr [R_ARM_CALL: .text+0x4bf0] 
    /*0000762c*/ SUBS        R7, R0, #0 
    /*00007630*/ BNE         RgnAttachToChn_x78f8
    /*00007634*/ LDR         R3, [SP, #16] 
    /*00007638*/ ADD         R9, R8, R3 
    /*0000763c*/ LDR         R3, [R9, #8] 
    /*00007640*/ LDR         R1, [R3, #308]!             @ 0x134 
    /*00007644*/ CMP         R1, R3 
    /*00007648*/ LDR         FP, [R1] 
    /*0000764c*/ BNE         RgnAttachToChn_x766c
    /*00007650*/ B           RgnAttachToChn_x7724
    RgnAttachToChn_x7654:
    /*00007654*/ LDR         R3, [R9, #8] 
    /*00007658*/ LDR         R2, [FP] 
    /*0000765c*/ ADD         R3, R3, #308                @ 0x134 
    /*00007660*/ CMP         FP, R3 
    /*00007664*/ MOV         FP, R2 
    /*00007668*/ BEQ         RgnAttachToChn_x7724
    RgnAttachToChn_x766c:
    /*0000766c*/ ADD         R0, R1, #8 
    /*00007670*/ MOV         R2, #12 
    /*00007674*/ MOV         R1, R4 
    /*00007678*/ BL          0x0000000000007678          @ osal_memcmp [R_ARM_CALL: +0x0] 
    /*0000767c*/ MOV         R1, FP 
    /*00007680*/ CMP         R0, #0 
    /*00007684*/ BNE         RgnAttachToChn_x7654
    RgnAttachToChn_x7688:
    /*00007688*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000768c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00007690*/ BL          0x0000000000007690          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007694*/ B           RgnAttachToChn_x76a4
    RgnAttachToChn_x7698:
    /*00007698*/ BL          0x00000000000002FC 
    /*0000769c*/ SUBS        R7, R0, #0 
    /*000076a0*/ BEQ         RgnAttachToChn_x7584
    RgnAttachToChn_x76a4:
    /*000076a4*/ MOV         R0, R7 
    /*000076a8*/ ADD         SP, SP, #204                @ 0xcc 
    /*000076ac*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnAttachToChn_x76b0:
    /*000076b0*/ BL          0x00000000000002BC 
    /*000076b4*/ SUBS        R7, R0, #0 
    /*000076b8*/ BNE         RgnAttachToChn_x76a4
    /*000076bc*/ LDR         R0, [R5, #4] 
    /*000076c0*/ B           RgnAttachToChn_x75a0
    RgnAttachToChn_x76c4:
    /*000076c4*/ BL          0x0000000000000290 
    /*000076c8*/ SUBS        R7, R0, #0 
    /*000076cc*/ BNE         RgnAttachToChn_x76a4
    /*000076d0*/ B           RgnAttachToChn_x758c
    RgnAttachToChn_x76d4:
    /*000076d4*/ BL          0x0000000000000290 
    /*000076d8*/ SUBS        R7, R0, #0 
    /*000076dc*/ BNE         RgnAttachToChn_x76a4
    /*000076e0*/ B           RgnAttachToChn_x7594
    RgnAttachToChn_x76e4:
    /*000076e4*/ LDR         R2, [R4, #8] 
    /*000076e8*/ MOV         R1, #3 
    /*000076ec*/ LDR         LR, [R4, #4] 
    /*000076f0*/ MOV         R0, R1 
    /*000076f4*/ LDR         IP, [R4] 
    /*000076f8*/ MOVW        R3, #4615                   @ 0x1207 
    /*000076fc*/ STR         R2, [SP, #12] 
    /*00007700*/ MOVW        R2, #0                      @ .LC170 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2354] 
    /*00007704*/ STR         LR, [SP, #8] 
    /*00007708*/ MOVT        R2, #0                      @ .LC170 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2354] 
    /*0000770c*/ STM         SP, {R3, IP} 
    /*00007710*/ LDR         R3, [PC, #1164]             @ 0x0000000000007ba4 
    /*00007714*/ BL          0x0000000000007714          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007718*/ MOV         R0, R8 
    /*0000771c*/ BL          0x000000000000771C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007720*/ B           RgnAttachToChn_x76a4
    RgnAttachToChn_x7724:
    /*00007724*/ LDR         R0, [PC, #1148]             @ 0x0000000000007ba8 
    /*00007728*/ ADD         R1, SP, #28 
    /*0000772c*/ BL          0x000000000000772C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00007730*/ MOV         R1, R4 
    /*00007734*/ MOV         R0, SL 
    /*00007738*/ BL          0x0000000000007738          @ RgnGetChnManage [R_ARM_CALL: .text+0x11fc] 
    /*0000773c*/ CMP         R0, #0 
    /*00007740*/ BEQ         RgnAttachToChn_x7770
    /*00007744*/ LDR         R3, [R4] 
    /*00007748*/ CMP         R3, #8 
    /*0000774c*/ BEQ         RgnAttachToChn_x796c
    RgnAttachToChn_x7750:
    /*00007750*/ LDR         R3, [R0] 
    RgnAttachToChn_x7754:
    /*00007754*/ LDR         R2, [SP, #20] 
    /*00007758*/ LDR         R2, [R2, #316]              @ 0x13c 
    /*0000775c*/ CMP         R3, R2 
    /*00007760*/ MOVCC       R3, #0 
    /*00007764*/ MOVCS       R3, #1 
    /*00007768*/ CMP         R3, #0 
    /*0000776c*/ BNE         RgnAttachToChn_x7a60
    RgnAttachToChn_x7770:
    /*00007770*/ ADD         R1, SP, #28 
    /*00007774*/ LDR         R0, [PC, #1068]             @ 0x0000000000007ba8 
    /*00007778*/ BL          0x0000000000007778          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000777c*/ LDR         R3, [SP, #16] 
    /*00007780*/ MOV         R2, #168                    @ 0xa8 
    /*00007784*/ ADD         R0, SP, #32 
    /*00007788*/ ADD         R3, R8, R3 
    /*0000778c*/ LDR         R1, [R3, #8] 
    /*00007790*/ ADD         R1, R1, #8 
    /*00007794*/ BL          0x0000000000007794          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00007798*/ ADD         R1, SP, #32 
    /*0000779c*/ MOV         R0, R5 
    /*000077a0*/ BL          0x00000000000077A0          @ RgnDisplayAttrToChnAttr [R_ARM_CALL: .text+0x6ac] 
    /*000077a4*/ SUBS        R3, R0, #0 
    /*000077a8*/ BNE         RgnAttachToChn_x78e4
    /*000077ac*/ LDR         R3, [R4] 
    /*000077b0*/ CMP         R3, #8 
    /*000077b4*/ BEQ         RgnAttachToChn_x78d4
    RgnAttachToChn_x77b8:
    /*000077b8*/ MOV         R1, SL 
    /*000077bc*/ ADD         R0, SP, #32 
    /*000077c0*/ BL          0x00000000000077C0          @ RgnCheckColorLUT [R_ARM_CALL: .text+0x49c0] 
    /*000077c4*/ SUBS        R3, R0, #0 
    /*000077c8*/ BNE         RgnAttachToChn_x78e4
    /*000077cc*/ MOV         R3, #1 
    /*000077d0*/ LDR         R2, [SP, #20] 
    /*000077d4*/ ADD         R1, SP, #32 
    /*000077d8*/ MOV         R0, SL 
    /*000077dc*/ BL          0x00000000000077DC          @ RgnCheckAttr [R_ARM_CALL: .text+0x27d0] 
    /*000077e0*/ SUBS        R3, R0, #0 
    /*000077e4*/ BNE         RgnAttachToChn_x79c4
    /*000077e8*/ ADD         R1, SP, #28 
    /*000077ec*/ LDR         R0, [PC, #948]              @ 0x0000000000007ba8 
    /*000077f0*/ BL          0x00000000000077F0          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000077f4*/ MOV         R1, R4 
    /*000077f8*/ LDR         R3, [SP, #20] 
    /*000077fc*/ ADD         R2, SP, #32 
    /*00007800*/ MOV         R0, R6 
    /*00007804*/ BL          0x0000000000007804          @ RgnCheckSetDisplayAttr [R_ARM_CALL: .text+0x4730] 
    /*00007808*/ ADD         R1, SP, #28 
    /*0000780c*/ SUBS        R5, R0, #0 
    /*00007810*/ LDR         R0, [PC, #912]              @ 0x0000000000007ba8 
    /*00007814*/ BNE         RgnAttachToChn_x7a10
    /*00007818*/ BL          0x0000000000007818          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000781c*/ MOV         R1, R5 
    /*00007820*/ MOV         R0, #20 
    /*00007824*/ BL          0x0000000000007824          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*00007828*/ SUBS        R5, R0, #0 
    /*0000782c*/ BEQ         RgnAttachToChn_x7ab8
    /*00007830*/ MOV         R2, #12 
    /*00007834*/ MOV         R1, R4 
    /*00007838*/ ADD         R0, R5, #8 
    /*0000783c*/ BL          0x000000000000783C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00007840*/ LDR         R3, [SP, #16] 
    /*00007844*/ MOV         R1, R4 
    /*00007848*/ MOV         R0, SL 
    /*0000784c*/ ADD         R8, R8, R3 
    /*00007850*/ LDR         R3, [R8, #8] 
    /*00007854*/ ADD         IP, R3, #308                @ 0x134 
    /*00007858*/ LDR         R2, [R3, #312]              @ 0x138 
    /*0000785c*/ STR         R5, [R3, #312]              @ 0x138 
    /*00007860*/ STR         IP, [R5] 
    /*00007864*/ STR         R2, [R5, #4] 
    /*00007868*/ STR         R5, [R2] 
    /*0000786c*/ BL          0x000000000000786C          @ RgnAllocChnInfo [R_ARM_CALL: .text+0xec4] 
    /*00007870*/ SUBS        R8, R0, #0 
    /*00007874*/ BEQ         RgnAttachToChn_x7b5c
    /*00007878*/ MOV         R1, R4 
    /*0000787c*/ MOV         R0, SL 
    /*00007880*/ BL          0x0000000000007880          @ RgnAllocChnManage [R_ARM_CALL: .text+0x1070] 
    /*00007884*/ CMP         R0, #0 
    /*00007888*/ BEQ         RgnAttachToChn_x7af4
    /*0000788c*/ ADD         R1, SP, #28 
    /*00007890*/ LDR         R0, [PC, #784]              @ 0x0000000000007ba8 
    /*00007894*/ BL          0x0000000000007894          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00007898*/ ADD         R3, SP, #32 
    /*0000789c*/ MOV         R2, R6 
    /*000078a0*/ MOV         R1, R4 
    /*000078a4*/ MOV         R0, SL 
    /*000078a8*/ BL          0x00000000000078A8          @ RgnAddRgnFrmChn [R_ARM_CALL: .text+0x3f2c] 
    /*000078ac*/ MOV         R1, R4 
    /*000078b0*/ MOV         R0, SL 
    /*000078b4*/ LDR         R2, [SP, #20] 
    /*000078b8*/ BL          0x00000000000078B8          @ RgnChnRgnsSort [R_ARM_CALL: .text+0x3e0c] 
    /*000078bc*/ MOV         R3, #1 
    /*000078c0*/ ADD         R1, SP, #28 
    /*000078c4*/ LDR         R0, [PC, #732]              @ 0x0000000000007ba8 
    /*000078c8*/ STR         R3, [R8, #4] 
    /*000078cc*/ BL          0x00000000000078CC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000078d0*/ B           RgnAttachToChn_x7688
    RgnAttachToChn_x78d4:
    /*000078d4*/ ADD         R0, SP, #32 
    /*000078d8*/ BL          0x0000000000000324 
    /*000078dc*/ SUBS        R3, R0, #0 
    /*000078e0*/ BEQ         RgnAttachToChn_x77b8
    RgnAttachToChn_x78e4:
    /*000078e4*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000078e8*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000078ec*/ MOV         R7, R3 
    /*000078f0*/ BL          0x00000000000078F0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000078f4*/ B           RgnAttachToChn_x76a4
    RgnAttachToChn_x78f8:
    /*000078f8*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000078fc*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00007900*/ BL          0x0000000000007900          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007904*/ LDR         R2, [R4] 
    /*00007908*/ MOV         R1, #3 
    /*0000790c*/ MOVW        R3, #4628                   @ 0x1214 
    /*00007910*/ STR         SL, [SP, #4] 
    /*00007914*/ MOV         R0, R1 
    /*00007918*/ STR         R2, [SP, #8] 
    /*0000791c*/ MOVW        R2, #0                      @ .LC171 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2390] 
    /*00007920*/ STR         R3, [SP] 
    /*00007924*/ MOVT        R2, #0                      @ .LC171 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2390] 
    /*00007928*/ LDR         R3, [PC, #628]              @ 0x0000000000007ba4 
    /*0000792c*/ BL          0x000000000000792C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007930*/ B           RgnAttachToChn_x76a4
    RgnAttachToChn_x7934:
    /*00007934*/ MOV         R1, #3 
    /*00007938*/ STMIB       SP, {R3, SL} 
    /*0000793c*/ MOV         R0, R1 
    /*00007940*/ MOVW        R3, #4605                   @ 0x11fd 
    /*00007944*/ MOVW        R2, #0                      @ .LC169 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2304] 
    /*00007948*/ STR         R3, [SP] 
    /*0000794c*/ MOVT        R2, #0                      @ .LC169 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2304] 
    /*00007950*/ LDR         R3, [PC, #588]              @ 0x0000000000007ba4 
    /*00007954*/ BL          0x0000000000007954          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007958*/ MOV         R0, R8 
    /*0000795c*/ MOVW        R7, #32771                  @ 0x8003 
    /*00007960*/ BL          0x0000000000007960          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007964*/ MOVT        R7, #40963                  @ 0xa003 
    /*00007968*/ B           RgnAttachToChn_x76a4
    RgnAttachToChn_x796c:
    /*0000796c*/ LDR         R3, [R5, #60]               @ 0x3c 
    /*00007970*/ CMP         R3, #0 
    /*00007974*/ LDREQ       R3, [R0, #8] 
    /*00007978*/ BEQ         RgnAttachToChn_x7754
    /*0000797c*/ CMP         R3, #1 
    /*00007980*/ LDREQ       R3, [R0, #12] 
    /*00007984*/ BEQ         RgnAttachToChn_x7754
    /*00007988*/ CMP         R3, #2 
    /*0000798c*/ LDREQ       R3, [R0, #16] 
    /*00007990*/ BNE         RgnAttachToChn_x7750
    /*00007994*/ B           RgnAttachToChn_x7754
    RgnAttachToChn_x7998:
    /*00007998*/ MOV         R7, #65024                  @ 0xfe00 
    /*0000799c*/ MOVT        R7, #65535                  @ 0xffff 
    /*000079a0*/ B           RgnAttachToChn_x76a4
    RgnAttachToChn_x79a4:
    /*000079a4*/ MOVW        R3, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb88] 
    /*000079a8*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000079ac*/ MOVT        R3, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb88] 
    /*000079b0*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000079b4*/ MOVW        R2, #4622                   @ 0x120e 
    /*000079b8*/ LDR         R1, [PC, #484]              @ 0x0000000000007ba4 
    /*000079bc*/ BL          0x00000000000079BC          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000079c0*/ B           RgnAttachToChn_x7620
    RgnAttachToChn_x79c4:
    /*000079c4*/ LDR         R2, [R4, #8] 
    /*000079c8*/ MOV         R1, #3 
    /*000079cc*/ LDR         R5, [R4, #4] 
    /*000079d0*/ MOVW        IP, #4694                   @ 0x1256 
    /*000079d4*/ LDR         LR, [R4] 
    /*000079d8*/ MOV         R0, R1 
    /*000079dc*/ STR         R2, [SP, #12] 
    /*000079e0*/ MOV         R7, R3 
    /*000079e4*/ STR         R5, [SP, #8] 
    /*000079e8*/ MOVW        R2, #0                      @ .LC173 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2428] 
    /*000079ec*/ STR         LR, [SP, #4] 
    /*000079f0*/ MOVT        R2, #0                      @ .LC173 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2428] 
    /*000079f4*/ STR         IP, [SP] 
    /*000079f8*/ LDR         R3, [PC, #420]              @ 0x0000000000007ba4 
    /*000079fc*/ BL          0x00000000000079FC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007a00*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00007a04*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00007a08*/ BL          0x0000000000007A08          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007a0c*/ B           RgnAttachToChn_x76a4
    RgnAttachToChn_x7a10:
    /*00007a10*/ BL          0x0000000000007A10          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00007a14*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00007a18*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00007a1c*/ MOV         R7, R5 
    /*00007a20*/ BL          0x0000000000007A20          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007a24*/ B           RgnAttachToChn_x76a4
    RgnAttachToChn_x7a28:
    /*00007a28*/ MOV         R1, #3 
    /*00007a2c*/ MOVW        R3, #4595                   @ 0x11f3 
    /*00007a30*/ MOV         R0, R1 
    /*00007a34*/ STR         R3, [SP] 
    /*00007a38*/ STR         R6, [SP, #4] 
    /*00007a3c*/ MOVW        R2, #0                      @ .LC160 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2144] 
    /*00007a40*/ LDR         R3, [PC, #348]              @ 0x0000000000007ba4 
    /*00007a44*/ MOVT        R2, #0                      @ .LC160 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2144] 
    /*00007a48*/ BL          0x0000000000007A48          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007a4c*/ MOV         R0, R8 
    /*00007a50*/ MOVW        R7, #32773                  @ 0x8005 
    /*00007a54*/ BL          0x0000000000007A54          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007a58*/ MOVT        R7, #40963                  @ 0xa003 
    /*00007a5c*/ B           RgnAttachToChn_x76a4
    RgnAttachToChn_x7a60:
    /*00007a60*/ ADD         R1, SP, #28 
    /*00007a64*/ LDR         R0, [PC, #316]              @ 0x0000000000007ba8 
    /*00007a68*/ BL          0x0000000000007A68          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00007a6c*/ LDMIB       R4, {R2, R3} 
    /*00007a70*/ LDR         IP, [R4] 
    /*00007a74*/ MOV         R1, #3 
    /*00007a78*/ MOV         R0, R1 
    /*00007a7c*/ MOVW        R7, #32777                  @ 0x8009 
    /*00007a80*/ STR         R3, [SP, #12] 
    /*00007a84*/ MOVW        R3, #4656                   @ 0x1230 
    /*00007a88*/ STR         R2, [SP, #8] 
    /*00007a8c*/ MOVW        R2, #0                      @ .LC172 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x23d8] 
    /*00007a90*/ STR         R3, [SP] 
    /*00007a94*/ MOVT        R2, #0                      @ .LC172 [R_ARM_MOVT_ABS: .rodata.str1.4+0x23d8] 
    /*00007a98*/ STR         IP, [SP, #4] 
    /*00007a9c*/ MOVT        R7, #40963                  @ 0xa003 
    /*00007aa0*/ LDR         R3, [PC, #252]              @ 0x0000000000007ba4 
    /*00007aa4*/ BL          0x0000000000007AA4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007aa8*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00007aac*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00007ab0*/ BL          0x0000000000007AB0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007ab4*/ B           RgnAttachToChn_x76a4
    RgnAttachToChn_x7ab8:
    /*00007ab8*/ MOV         R1, #3 
    /*00007abc*/ MOVW        R3, #4714                   @ 0x126a 
    /*00007ac0*/ MOV         R0, R1 
    /*00007ac4*/ STR         R3, [SP] 
    /*00007ac8*/ STR         R6, [SP, #4] 
    /*00007acc*/ MOVW        R2, #0                      @ .LC174 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x246c] 
    /*00007ad0*/ LDR         R3, [PC, #204]              @ 0x0000000000007ba4 
    /*00007ad4*/ MOVT        R2, #0                      @ .LC174 [R_ARM_MOVT_ABS: .rodata.str1.4+0x246c] 
    /*00007ad8*/ BL          0x0000000000007AD8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007adc*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00007ae0*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00007ae4*/ MOVW        R7, #32780                  @ 0x800c 
    /*00007ae8*/ BL          0x0000000000007AE8          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007aec*/ MOVT        R7, #40963                  @ 0xa003 
    /*00007af0*/ B           RgnAttachToChn_x76a4
    RgnAttachToChn_x7af4:
    /*00007af4*/ LDR         R1, [R5, #4] 
    /*00007af8*/ MOV         R0, R5 
    /*00007afc*/ LDR         IP, [R5] 
    /*00007b00*/ MOV         R2, #256                    @ 0x100 
    /*00007b04*/ MOV         R3, #512                    @ 0x200 
    /*00007b08*/ MOVT        R2, #16 
    /*00007b0c*/ MOVT        R3, #32 
    /*00007b10*/ MOVW        R7, #32780                  @ 0x800c 
    /*00007b14*/ STR         R1, [IP, #4] 
    /*00007b18*/ MOVT        R7, #40963                  @ 0xa003 
    /*00007b1c*/ STR         IP, [R1] 
    /*00007b20*/ STR         R3, [R5, #4] 
    /*00007b24*/ STR         R2, [R5] 
    /*00007b28*/ BL          0x0000000000007B28          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00007b2c*/ MOV         R0, R8 
    /*00007b30*/ BL          0x0000000000007B30          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00007b34*/ MOV         R1, #3 
    /*00007b38*/ MOV         R0, R1 
    /*00007b3c*/ MOVW        R3, #4743                   @ 0x1287 
    /*00007b40*/ STR         R6, [SP, #4] 
    RgnAttachToChn_x7b44:
    /*00007b44*/ STR         R3, [SP] 
    /*00007b48*/ MOVW        R2, #0                      @ .LC175 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24a8] 
    /*00007b4c*/ LDR         R3, [PC, #80]               @ 0x0000000000007ba4 
    /*00007b50*/ MOVT        R2, #0                      @ .LC175 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24a8] 
    /*00007b54*/ BL          0x0000000000007B54          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007b58*/ B           RgnAttachToChn_x7688
    RgnAttachToChn_x7b5c:
    /*00007b5c*/ LDR         R1, [R5, #4] 
    /*00007b60*/ MOV         R0, R5 
    /*00007b64*/ LDR         IP, [R5] 
    /*00007b68*/ MOV         R2, #256                    @ 0x100 
    /*00007b6c*/ MOV         R3, #512                    @ 0x200 
    /*00007b70*/ MOVT        R2, #16 
    /*00007b74*/ MOVT        R3, #32 
    /*00007b78*/ MOVW        R7, #32780                  @ 0x800c 
    /*00007b7c*/ STR         R1, [IP, #4] 
    /*00007b80*/ MOVT        R7, #40963                  @ 0xa003 
    /*00007b84*/ STR         IP, [R1] 
    /*00007b88*/ STM         R5, {R2, R3} 
    /*00007b8c*/ BL          0x0000000000007B8C          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00007b90*/ MOV         R1, #3 
    /*00007b94*/ STR         R6, [SP, #4] 
    /*00007b98*/ MOV         R0, R1 
    /*00007b9c*/ MOVW        R3, #4731                   @ 0x127b 
    /*00007ba0*/ B           RgnAttachToChn_x7b44
    /*00007ba4*/ @           <UNDEFINED> instruction: 0x000004b4 
    /*00007ba8*/ ANDEQ       R0, R0, R4 
FUNC_END RgnAttachToChn


FUNC_BEGIN RgnDetachFrmChn
    /*00007bac*/ CMP         R0, #127                    @ 0x7f 
    /*00007bb0*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00007bb4*/ MOV         R7, R0 
    /*00007bb8*/ SUB         SP, SP, #36                 @ 0x24 
    /*00007bbc*/ MOV         R5, R1 
    /*00007bc0*/ BHI         RgnDetachFrmChn_x7db8
    RgnDetachFrmChn_x7bc4:
    /*00007bc4*/ CMP         R5, #0 
    /*00007bc8*/ BEQ         RgnDetachFrmChn_x7e70
    RgnDetachFrmChn_x7bcc:
    /*00007bcc*/ MOVW        R6, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00007bd0*/ MOVT        R6, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00007bd4*/ MOV         R0, R6 
    /*00007bd8*/ BL          0x0000000000007BD8          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00007bdc*/ CMP         R0, #0 
    /*00007be0*/ BNE         RgnDetachFrmChn_x7e80
    /*00007be4*/ LSL         R3, R7, #2 
    /*00007be8*/ STR         R3, [SP, #20] 
    /*00007bec*/ ADD         SL, R6, R3 
    /*00007bf0*/ LDR         R3, [SL, #8] 
    /*00007bf4*/ CMP         R3, #0 
    /*00007bf8*/ BEQ         RgnDetachFrmChn_x7e38
    /*00007bfc*/ LDR         R2, [R3, #208]              @ 0xd0 
    /*00007c00*/ CMP         R2, #0 
    /*00007c04*/ BEQ         RgnDetachFrmChn_x7c48
    /*00007c08*/ MOV         R1, #3 
    /*00007c0c*/ STR         R7, [SP, #4] 
    /*00007c10*/ MOV         R0, R1 
    /*00007c14*/ MOVW        R3, #4833                   @ 0x12e1 
    /*00007c18*/ MOVW        R2, #0                      @ .LC148 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1dec] 
    /*00007c1c*/ STR         R3, [SP] 
    /*00007c20*/ MOVT        R2, #0                      @ .LC148 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1dec] 
    /*00007c24*/ LDR         R3, [PC, #668]              @ 0x0000000000007ec8 
    /*00007c28*/ BL          0x0000000000007C28          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007c2c*/ MOV         R0, R6 
    /*00007c30*/ BL          0x0000000000007C30          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007c34*/ MOVW        R9, #32777                  @ 0x8009 
    /*00007c38*/ MOVT        R9, #40963                  @ 0xa003 
    /*00007c3c*/ MOV         R0, R9 
    /*00007c40*/ ADD         SP, SP, #36                 @ 0x24 
    /*00007c44*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnDetachFrmChn_x7c48:
    /*00007c48*/ LDR         R3, [R3, #176]              @ 0xb0 
    /*00007c4c*/ MOV         R1, R5 
    /*00007c50*/ MOV         R0, R3 
    /*00007c54*/ STR         R3, [SP, #16] 
    /*00007c58*/ BL          0x0000000000007C58          @ RgnCheckCapacityByModId [R_ARM_CALL: .text+0x4a70] 
    /*00007c5c*/ SUBS        R9, R0, #0 
    /*00007c60*/ BNE         RgnDetachFrmChn_x7e04
    /*00007c64*/ MOV         R1, R5 
    /*00007c68*/ LDR         R0, [SP, #16] 
    /*00007c6c*/ BL          0x0000000000007C6C          @ RgnCheckMppChn [R_ARM_CALL: .text+0x13ac] 
    /*00007c70*/ SUBS        R9, R0, #0 
    /*00007c74*/ BNE         RgnDetachFrmChn_x7dd0
    /*00007c78*/ LDR         R3, [SL, #8] 
    /*00007c7c*/ LDR         R4, [R3, #308]!             @ 0x134 
    /*00007c80*/ CMP         R4, R3 
    /*00007c84*/ LDR         FP, [R4] 
    /*00007c88*/ BNE         RgnDetachFrmChn_x7ca8
    /*00007c8c*/ B           RgnDetachFrmChn_x7d74
    RgnDetachFrmChn_x7c90:
    /*00007c90*/ LDR         R3, [SL, #8] 
    /*00007c94*/ MOV         R4, FP 
    /*00007c98*/ ADD         R3, R3, #308                @ 0x134 
    /*00007c9c*/ CMP         FP, R3 
    /*00007ca0*/ LDR         FP, [FP] 
    /*00007ca4*/ BEQ         RgnDetachFrmChn_x7d74
    RgnDetachFrmChn_x7ca8:
    /*00007ca8*/ ADD         R8, R4, #8 
    /*00007cac*/ MOV         R2, #12 
    /*00007cb0*/ MOV         R1, R5 
    /*00007cb4*/ MOV         R0, R8 
    /*00007cb8*/ BL          0x0000000000007CB8          @ osal_memcmp [R_ARM_CALL: +0x0] 
    /*00007cbc*/ CMP         R0, #0 
    /*00007cc0*/ BNE         RgnDetachFrmChn_x7c90
    /*00007cc4*/ MOV         R1, R5 
    /*00007cc8*/ LDR         R0, [SP, #16] 
    /*00007ccc*/ BL          0x0000000000007CCC          @ RgnGetChnGetInfo [R_ARM_CALL: .text+0x1034] 
    /*00007cd0*/ SUBS        SL, R0, #0 
    /*00007cd4*/ BEQ         RgnDetachFrmChn_x7e18
    RgnDetachFrmChn_x7cd8:
    /*00007cd8*/ LDR         R0, [PC, #492]              @ 0x0000000000007ecc 
    /*00007cdc*/ ADD         R1, SP, #28 
    /*00007ce0*/ BL          0x0000000000007CE0          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00007ce4*/ LDR         R3, [SP, #20] 
    /*00007ce8*/ MOV         R2, R7 
    /*00007cec*/ MOV         R1, R5 
    /*00007cf0*/ ADD         R3, R6, R3 
    /*00007cf4*/ LDR         R3, [R3, #8] 
    /*00007cf8*/ LDR         R0, [R3, #176]              @ 0xb0 
    /*00007cfc*/ BL          0x0000000000007CFC          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*00007d00*/ CMP         R0, #0 
    /*00007d04*/ BEQ         RgnDetachFrmChn_x7eac
    /*00007d08*/ LDR         R3, [R0, #152]              @ 0x98 
    /*00007d0c*/ MOV         R2, R7 
    /*00007d10*/ MOV         R1, R5 
    /*00007d14*/ LDR         R0, [SP, #16] 
    /*00007d18*/ BL          0x0000000000007D18          @ RgnDelRgnFrmChn [R_ARM_CALL: .text+0x4058] 
    /*00007d1c*/ MOV         R3, #1 
    /*00007d20*/ ADD         R1, SP, #28 
    /*00007d24*/ LDR         R0, [PC, #416]              @ 0x0000000000007ecc 
    /*00007d28*/ STR         R3, [SL, #4] 
    /*00007d2c*/ BL          0x0000000000007D2C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00007d30*/ MOV         R0, R8 
    /*00007d34*/ MOV         R2, #12 
    /*00007d38*/ MOV         R1, R5 
    /*00007d3c*/ BL          0x0000000000007D3C          @ osal_memcmp [R_ARM_CALL: +0x0] 
    /*00007d40*/ CMP         R0, #0 
    /*00007d44*/ BNE         RgnDetachFrmChn_x7e8c
    RgnDetachFrmChn_x7d48:
    /*00007d48*/ LDR         R0, [R4] 
    /*00007d4c*/ MOV         R2, #256                    @ 0x100 
    /*00007d50*/ LDR         R1, [R4, #4] 
    /*00007d54*/ MOVT        R2, #16 
    /*00007d58*/ MOV         R3, #512                    @ 0x200 
    /*00007d5c*/ MOVT        R3, #32 
    /*00007d60*/ STR         R1, [R0, #4] 
    /*00007d64*/ STR         R0, [R1] 
    /*00007d68*/ MOV         R0, R4 
    /*00007d6c*/ STM         R4, {R2, R3} 
    /*00007d70*/ BL          0x0000000000007D70          @ osal_kfree [R_ARM_CALL: +0x0] 
    RgnDetachFrmChn_x7d74:
    /*00007d74*/ LDR         R3, [R5] 
    /*00007d78*/ LDR         R2, [SP, #16] 
    /*00007d7c*/ CMP         R3, #8 
    /*00007d80*/ CMPEQ       R2, #0 
    /*00007d84*/ BNE         RgnDetachFrmChn_x7da0
    /*00007d88*/ LDR         R3, [R6, #748]              @ 0x2ec 
    /*00007d8c*/ CMP         R3, #0 
    /*00007d90*/ BEQ         RgnDetachFrmChn_x7da0
    /*00007d94*/ MOV         R2, #1 
    /*00007d98*/ LDMIB       R5, {R0, R1} 
    /*00007d9c*/ BLX         R3 
    RgnDetachFrmChn_x7da0:
    /*00007da0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00007da4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00007da8*/ BL          0x0000000000007DA8          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007dac*/ MOV         R0, R9 
    /*00007db0*/ ADD         SP, SP, #36                 @ 0x24 
    /*00007db4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnDetachFrmChn_x7db8:
    /*00007db8*/ BL          0x00000000000002FC 
    /*00007dbc*/ SUBS        R9, R0, #0 
    /*00007dc0*/ BEQ         RgnDetachFrmChn_x7bc4
    RgnDetachFrmChn_x7dc4:
    /*00007dc4*/ MOV         R0, R9 
    /*00007dc8*/ ADD         SP, SP, #36                 @ 0x24 
    /*00007dcc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnDetachFrmChn_x7dd0:
    /*00007dd0*/ LDR         R2, [R5, #8] 
    /*00007dd4*/ MOV         R1, #3 
    /*00007dd8*/ LDR         LR, [R5, #4] 
    /*00007ddc*/ MOVW        R3, #4852                   @ 0x12f4 
    /*00007de0*/ LDR         IP, [R5] 
    /*00007de4*/ MOV         R0, R1 
    /*00007de8*/ STR         R2, [SP, #12] 
    /*00007dec*/ MOVW        R2, #0                      @ .LC170 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2354] 
    /*00007df0*/ STR         LR, [SP, #8] 
    /*00007df4*/ MOVT        R2, #0                      @ .LC170 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2354] 
    /*00007df8*/ STM         SP, {R3, IP} 
    /*00007dfc*/ LDR         R3, [PC, #196]              @ 0x0000000000007ec8 
    /*00007e00*/ BL          0x0000000000007E00          @ HI_LOG [R_ARM_CALL: +0x0] 
    RgnDetachFrmChn_x7e04:
    /*00007e04*/ MOV         R0, R6 
    /*00007e08*/ BL          0x0000000000007E08          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007e0c*/ MOV         R0, R9 
    /*00007e10*/ ADD         SP, SP, #36                 @ 0x24 
    /*00007e14*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnDetachFrmChn_x7e18:
    /*00007e18*/ MOVW        R3, #0                      @ .LC137 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b68] 
    /*00007e1c*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00007e20*/ MOVT        R3, #0                      @ .LC137 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b68] 
    /*00007e24*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00007e28*/ MOVW        R2, #4872                   @ 0x1308 
    /*00007e2c*/ LDR         R1, [PC, #148]              @ 0x0000000000007ec8 
    /*00007e30*/ BL          0x0000000000007E30          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00007e34*/ B           RgnDetachFrmChn_x7cd8
    RgnDetachFrmChn_x7e38:
    /*00007e38*/ MOV         R1, #3 
    /*00007e3c*/ MOVW        R3, #4827                   @ 0x12db 
    /*00007e40*/ MOV         R0, R1 
    /*00007e44*/ STR         R3, [SP] 
    /*00007e48*/ STR         R7, [SP, #4] 
    /*00007e4c*/ MOVW        R2, #0                      @ .LC176 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24e0] 
    /*00007e50*/ LDR         R3, [PC, #112]              @ 0x0000000000007ec8 
    /*00007e54*/ MOVT        R2, #0                      @ .LC176 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24e0] 
    /*00007e58*/ BL          0x0000000000007E58          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00007e5c*/ MOV         R0, R6 
    /*00007e60*/ MOVW        R9, #32773                  @ 0x8005 
    /*00007e64*/ BL          0x0000000000007E64          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007e68*/ MOVT        R9, #40963                  @ 0xa003 
    /*00007e6c*/ B           RgnDetachFrmChn_x7dc4
    RgnDetachFrmChn_x7e70:
    /*00007e70*/ BL          0x0000000000000290 
    /*00007e74*/ SUBS        R9, R0, #0 
    /*00007e78*/ BNE         RgnDetachFrmChn_x7dc4
    /*00007e7c*/ B           RgnDetachFrmChn_x7bcc
    RgnDetachFrmChn_x7e80:
    /*00007e80*/ MOV         R9, #65024                  @ 0xfe00 
    /*00007e84*/ MOVT        R9, #65535                  @ 0xffff 
    /*00007e88*/ B           RgnDetachFrmChn_x7dc4
    RgnDetachFrmChn_x7e8c:
    /*00007e8c*/ MOVW        R3, #0                      @ .LC177 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2518] 
    /*00007e90*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00007e94*/ MOVT        R3, #0                      @ .LC177 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2518] 
    /*00007e98*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00007e9c*/ MOVW        R2, #4890                   @ 0x131a 
    /*00007ea0*/ LDR         R1, [PC, #32]               @ 0x0000000000007ec8 
    /*00007ea4*/ BL          0x0000000000007EA4          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00007ea8*/ B           RgnDetachFrmChn_x7d48
    RgnDetachFrmChn_x7eac:
    /*00007eac*/ ADD         R1, SP, #28 
    /*00007eb0*/ LDR         R0, [PC, #20]               @ 0x0000000000007ecc 
    /*00007eb4*/ BL          0x0000000000007EB4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00007eb8*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00007ebc*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00007ec0*/ BL          0x0000000000007EC0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007ec4*/ B           RgnDetachFrmChn_x7dc4
    /*00007ec8*/ ANDEQ       R0, R0, R4, ASR #9 
    /*00007ecc*/ ANDEQ       R0, R0, R4 
FUNC_END RgnDetachFrmChn


FUNC_BEGIN RgnGetDisplayAttr
    /*00007ed0*/ CMP         R0, #127                    @ 0x7f 
    /*00007ed4*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00007ed8*/ MOV         R5, R0 
    /*00007edc*/ SUB         SP, SP, #36                 @ 0x24 
    /*00007ee0*/ MOV         R6, R1 
    /*00007ee4*/ MOV         R8, R2 
    /*00007ee8*/ BHI         RgnGetDisplayAttr_x7f9c
    RgnGetDisplayAttr_x7eec:
    /*00007eec*/ CMP         R6, #0 
    /*00007ef0*/ BEQ         RgnGetDisplayAttr_x8044
    RgnGetDisplayAttr_x7ef4:
    /*00007ef4*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00007ef8*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00007efc*/ MOV         R0, R4 
    /*00007f00*/ BL          0x0000000000007F00          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00007f04*/ SUBS        R2, R0, #0 
    /*00007f08*/ BNE         RgnGetDisplayAttr_x8054
    /*00007f0c*/ ADD         R3, R4, R5, LSL #2 
    /*00007f10*/ LDR         R3, [R3, #8] 
    /*00007f14*/ CMP         R3, #0 
    /*00007f18*/ BEQ         RgnGetDisplayAttr_x800c
    /*00007f1c*/ LDR         R9, [R3, #176]              @ 0xb0 
    /*00007f20*/ MOV         R1, R6 
    /*00007f24*/ MOV         R0, R9 
    /*00007f28*/ BL          0x0000000000007F28          @ RgnCheckCapacityByModId [R_ARM_CALL: .text+0x4a70] 
    /*00007f2c*/ SUBS        R7, R0, #0 
    /*00007f30*/ BNE         RgnGetDisplayAttr_x8004
    /*00007f34*/ MOV         R1, R6 
    /*00007f38*/ MOV         R0, R9 
    /*00007f3c*/ BL          0x0000000000007F3C          @ RgnCheckMppChn [R_ARM_CALL: .text+0x13ac] 
    /*00007f40*/ SUBS        R7, R0, #0 
    /*00007f44*/ BNE         RgnGetDisplayAttr_x8004
    /*00007f48*/ ADD         R0, R4, #4 
    /*00007f4c*/ ADD         R1, SP, #28 
    /*00007f50*/ BL          0x0000000000007F50          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00007f54*/ MOV         R1, R6 
    /*00007f58*/ MOV         R2, R5 
    /*00007f5c*/ MOV         R0, R9 
    /*00007f60*/ BL          0x0000000000007F60          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*00007f64*/ SUBS        R1, R0, #0 
    /*00007f68*/ BEQ         RgnGetDisplayAttr_x7fb4
    /*00007f6c*/ MOV         R2, R8 
    /*00007f70*/ MOV         R0, R9 
    /*00007f74*/ BL          0x0000000000007F74          @ RgnChnAttrToDisplayAttr [R_ARM_CALL: .text+0x954] 
    /*00007f78*/ ADD         R1, SP, #28 
    /*00007f7c*/ MOV         R7, R0 
    /*00007f80*/ ADD         R0, R4, #4 
    /*00007f84*/ BL          0x0000000000007F84          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00007f88*/ MOV         R0, R4 
    RgnGetDisplayAttr_x7f8c:
    /*00007f8c*/ BL          0x0000000000007F8C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00007f90*/ MOV         R0, R7 
    /*00007f94*/ ADD         SP, SP, #36                 @ 0x24 
    /*00007f98*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    RgnGetDisplayAttr_x7f9c:
    /*00007f9c*/ BL          0x00000000000002FC 
    /*00007fa0*/ SUBS        R7, R0, #0 
    /*00007fa4*/ BEQ         RgnGetDisplayAttr_x7eec
    RgnGetDisplayAttr_x7fa8:
    /*00007fa8*/ MOV         R0, R7 
    /*00007fac*/ ADD         SP, SP, #36                 @ 0x24 
    /*00007fb0*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    RgnGetDisplayAttr_x7fb4:
    /*00007fb4*/ ADD         R1, SP, #28 
    /*00007fb8*/ ADD         R0, R4, #4 
    /*00007fbc*/ BL          0x0000000000007FBC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00007fc0*/ LDR         R2, [R6, #8] 
    /*00007fc4*/ LDR         LR, [R6, #4] 
    /*00007fc8*/ MOV         R1, #3 
    /*00007fcc*/ LDR         IP, [R6] 
    /*00007fd0*/ MOVW        R3, #4960                   @ 0x1360 
    /*00007fd4*/ STR         R2, [SP, #16] 
    /*00007fd8*/ MOV         R0, R1 
    /*00007fdc*/ MOVW        R2, #0                      @ .LC178 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2528] 
    /*00007fe0*/ STR         R3, [SP] 
    /*00007fe4*/ STR         R5, [SP, #4] 
    /*00007fe8*/ MOVT        R2, #0                      @ .LC178 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2528] 
    /*00007fec*/ STR         LR, [SP, #12] 
    /*00007ff0*/ MOVW        R7, #32777                  @ 0x8009 
    /*00007ff4*/ STR         IP, [SP, #8] 
    /*00007ff8*/ MOVT        R7, #40963                  @ 0xa003 
    /*00007ffc*/ LDR         R3, [PC, #92]               @ 0x0000000000008060 
    /*00008000*/ BL          0x0000000000008000          @ HI_LOG [R_ARM_CALL: +0x0] 
    RgnGetDisplayAttr_x8004:
    /*00008004*/ MOV         R0, R4 
    /*00008008*/ B           RgnGetDisplayAttr_x7f8c
    RgnGetDisplayAttr_x800c:
    /*0000800c*/ MOV         R1, #3 
    /*00008010*/ MOVW        R3, #4929                   @ 0x1341 
    /*00008014*/ MOV         R0, R1 
    /*00008018*/ STR         R3, [SP] 
    /*0000801c*/ STR         R5, [SP, #4] 
    /*00008020*/ MOVW        R2, #0                      @ .LC160 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2144] 
    /*00008024*/ LDR         R3, [PC, #52]               @ 0x0000000000008060 
    /*00008028*/ MOVT        R2, #0                      @ .LC160 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2144] 
    /*0000802c*/ BL          0x000000000000802C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008030*/ MOV         R0, R4 
    /*00008034*/ MOVW        R7, #32773                  @ 0x8005 
    /*00008038*/ BL          0x0000000000008038          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000803c*/ MOVT        R7, #40963                  @ 0xa003 
    /*00008040*/ B           RgnGetDisplayAttr_x7fa8
    RgnGetDisplayAttr_x8044:
    /*00008044*/ BL          0x0000000000000290 
    /*00008048*/ SUBS        R7, R0, #0 
    /*0000804c*/ BNE         RgnGetDisplayAttr_x7fa8
    /*00008050*/ B           RgnGetDisplayAttr_x7ef4
    RgnGetDisplayAttr_x8054:
    /*00008054*/ MOV         R7, #65024                  @ 0xfe00 
    /*00008058*/ MOVT        R7, #65535                  @ 0xffff 
    /*0000805c*/ B           RgnGetDisplayAttr_x7fa8
    /*00008060*/ LDRDEQ      R0, [R0], -R4 
FUNC_END RgnGetDisplayAttr


FUNC_BEGIN RgnSetDisplayAttr
    /*00008064*/ CMP         R0, #127                    @ 0x7f 
    /*00008068*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000806c*/ MOV         R5, R0 
    /*00008070*/ SUB         SP, SP, #196                @ 0xc4 
    /*00008074*/ MOV         R6, R1 
    /*00008078*/ MOV         R4, R2 
    /*0000807c*/ BHI         RgnSetDisplayAttr_x8290
    RgnSetDisplayAttr_x8080:
    /*00008080*/ CMP         R6, #0 
    /*00008084*/ BEQ         RgnSetDisplayAttr_x82c0
    RgnSetDisplayAttr_x8088:
    /*00008088*/ CMP         R4, #0 
    /*0000808c*/ BEQ         RgnSetDisplayAttr_x82d0
    RgnSetDisplayAttr_x8090:
    /*00008090*/ LDR         R8, [R4, #4] 
    /*00008094*/ CMP         R8, #4 
    /*00008098*/ BHI         RgnSetDisplayAttr_x82a8
    RgnSetDisplayAttr_x809c:
    /*0000809c*/ MOV         R2, R4 
    /*000080a0*/ MOV         R1, R6 
    /*000080a4*/ MOV         R0, R8 
    /*000080a8*/ BL          0x00000000000080A8          @ RgnCheckCapacityByModId [R_ARM_CALL: .text+0x4a70] 
    /*000080ac*/ SUBS        R7, R0, #0 
    /*000080b0*/ BNE         RgnSetDisplayAttr_x829c
    /*000080b4*/ MOV         R1, R6 
    /*000080b8*/ MOV         R0, R8 
    /*000080bc*/ BL          0x00000000000080BC          @ RgnCheckMppChn [R_ARM_CALL: .text+0x13ac] 
    /*000080c0*/ SUBS        R7, R0, #0 
    /*000080c4*/ BNE         RgnSetDisplayAttr_x829c
    /*000080c8*/ MOVW        R9, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000080cc*/ MOVT        R9, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000080d0*/ MOV         R0, R9 
    /*000080d4*/ BL          0x00000000000080D4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000080d8*/ CMP         R0, #0 
    /*000080dc*/ BNE         RgnSetDisplayAttr_x836c
    /*000080e0*/ LSL         SL, R5, #2 
    /*000080e4*/ ADD         FP, R9, SL 
    /*000080e8*/ LDR         R3, [FP, #8] 
    /*000080ec*/ CMP         R3, #0 
    /*000080f0*/ BEQ         RgnSetDisplayAttr_x8410
    /*000080f4*/ LDR         R2, [R3, #176]              @ 0xb0 
    /*000080f8*/ CMP         R8, R2 
    /*000080fc*/ BNE         RgnSetDisplayAttr_x8378
    /*00008100*/ LDR         R2, [R3, #308]!             @ 0x134 
    /*00008104*/ CMP         R2, R3 
    /*00008108*/ LDR         R7, [R2] 
    /*0000810c*/ BNE         RgnSetDisplayAttr_x812c
    /*00008110*/ B           RgnSetDisplayAttr_x82e0
    RgnSetDisplayAttr_x8114:
    /*00008114*/ LDR         R3, [FP, #8] 
    /*00008118*/ LDR         R1, [R7] 
    /*0000811c*/ ADD         R3, R3, #308                @ 0x134 
    /*00008120*/ CMP         R7, R3 
    /*00008124*/ MOV         R7, R1 
    /*00008128*/ BEQ         RgnSetDisplayAttr_x82e0
    RgnSetDisplayAttr_x812c:
    /*0000812c*/ ADD         R0, R2, #8 
    /*00008130*/ MOV         R1, R6 
    /*00008134*/ MOV         R2, #12 
    /*00008138*/ BL          0x0000000000008138          @ osal_memcmp [R_ARM_CALL: +0x0] 
    /*0000813c*/ MOV         R2, R7 
    /*00008140*/ CMP         R0, #0 
    /*00008144*/ BNE         RgnSetDisplayAttr_x8114
    /*00008148*/ ADD         R3, R9, SL 
    /*0000814c*/ MOV         R2, #168                    @ 0xa8 
    /*00008150*/ ADD         R0, SP, #24 
    /*00008154*/ LDR         R1, [R3, #8] 
    /*00008158*/ ADD         R1, R1, #8 
    /*0000815c*/ BL          0x000000000000815C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00008160*/ ADD         R1, SP, #24 
    /*00008164*/ MOV         R0, R4 
    /*00008168*/ BL          0x0000000000008168          @ RgnDisplayAttrToChnAttr [R_ARM_CALL: .text+0x6ac] 
    /*0000816c*/ SUBS        R7, R0, #0 
    /*00008170*/ BNE         RgnSetDisplayAttr_x8280
    /*00008174*/ LDR         R2, [R6] 
    /*00008178*/ LDR         R3, [R4] 
    /*0000817c*/ CMP         R2, #8 
    /*00008180*/ STR         R3, [SP, #24] 
    /*00008184*/ BEQ         RgnSetDisplayAttr_x8358
    RgnSetDisplayAttr_x8188:
    /*00008188*/ MOV         R1, R8 
    /*0000818c*/ ADD         R0, SP, #24 
    /*00008190*/ BL          0x0000000000008190          @ RgnCheckColorLUT [R_ARM_CALL: .text+0x49c0] 
    /*00008194*/ SUBS        R7, R0, #0 
    /*00008198*/ BNE         RgnSetDisplayAttr_x8280
    /*0000819c*/ LDR         R1, [R6] 
    /*000081a0*/ MOV         R0, R8 
    /*000081a4*/ BL          0x00000000000081A4          @ RgnGetCapacity [R_ARM_CALL: .text+0x3668] 
    /*000081a8*/ SUBS        FP, R0, #0 
    /*000081ac*/ BEQ         RgnSetDisplayAttr_x83b0
    RgnSetDisplayAttr_x81b0:
    /*000081b0*/ MOV         R1, R4 
    /*000081b4*/ MOV         R0, FP 
    /*000081b8*/ BL          0x00000000000081B8          @ RgnCheckCapacityByRgnChnAttr [R_ARM_CALL: .text+0x4bf0] 
    /*000081bc*/ SUBS        R7, R0, #0 
    /*000081c0*/ BNE         RgnSetDisplayAttr_x831c
    /*000081c4*/ LDR         R0, [PC, #636]              @ 0x0000000000008448 
    /*000081c8*/ ADD         R1, SP, #20 
    /*000081cc*/ BL          0x00000000000081CC          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000081d0*/ ADD         R2, SP, #24 
    /*000081d4*/ MOV         R1, R6 
    /*000081d8*/ MOV         R0, R5 
    /*000081dc*/ BL          0x00000000000081DC          @ RgnCheckAttachFiled [R_ARM_CALL: .text+0x4698] 
    /*000081e0*/ SUBS        R7, R0, #0 
    /*000081e4*/ BNE         RgnSetDisplayAttr_x8274
    /*000081e8*/ MOV         R3, FP 
    /*000081ec*/ ADD         R2, SP, #24 
    /*000081f0*/ MOV         R1, R6 
    /*000081f4*/ MOV         R0, R5 
    /*000081f8*/ BL          0x00000000000081F8          @ RgnCheckSetDisplayAttr [R_ARM_CALL: .text+0x4730] 
    /*000081fc*/ SUBS        R7, R0, #0 
    /*00008200*/ BNE         RgnSetDisplayAttr_x8274
    /*00008204*/ MOV         R2, R5 
    /*00008208*/ MOV         R1, R6 
    /*0000820c*/ MOV         R0, R8 
    /*00008210*/ BL          0x0000000000008210          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*00008214*/ SUBS        R5, R0, #0 
    /*00008218*/ BEQ         RgnSetDisplayAttr_x83f0
    /*0000821c*/ MOV         R1, R6 
    /*00008220*/ MOV         R0, R8 
    /*00008224*/ BL          0x0000000000008224          @ RgnGetChnGetInfo [R_ARM_CALL: .text+0x1034] 
    /*00008228*/ SUBS        R4, R0, #0 
    /*0000822c*/ BEQ         RgnSetDisplayAttr_x83d0
    RgnSetDisplayAttr_x8230:
    /*00008230*/ MOV         R3, R8 
    /*00008234*/ MOV         R2, R5 
    /*00008238*/ ADD         R1, SP, #24 
    /*0000823c*/ MOV         R0, #0 
    /*00008240*/ ADD         R9, R9, SL 
    /*00008244*/ STR         R0, [SP] 
    /*00008248*/ MOV         R0, FP 
    /*0000824c*/ BL          0x000000000000824C          @ RgnUpChnAttr [R_ARM_CALL: .text+0x4230] 
    /*00008250*/ MOV         R2, FP 
    /*00008254*/ MOV         R1, R6 
    /*00008258*/ MOV         R0, R8 
    /*0000825c*/ BL          0x000000000000825C          @ RgnChnRgnsSort [R_ARM_CALL: .text+0x3e0c] 
    /*00008260*/ LDR         R3, [R9, #8] 
    /*00008264*/ LDR         R3, [R3, #208]              @ 0xd0 
    /*00008268*/ CMP         R3, #0 
    /*0000826c*/ MOVEQ       R3, #1 
    /*00008270*/ STREQ       R3, [R4, #4] 
    RgnSetDisplayAttr_x8274:
    /*00008274*/ ADD         R1, SP, #20 
    /*00008278*/ LDR         R0, [PC, #456]              @ 0x0000000000008448 
    /*0000827c*/ BL          0x000000000000827C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    RgnSetDisplayAttr_x8280:
    /*00008280*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00008284*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00008288*/ BL          0x0000000000008288          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000828c*/ B           RgnSetDisplayAttr_x829c
    RgnSetDisplayAttr_x8290:
    /*00008290*/ BL          0x00000000000002FC 
    /*00008294*/ SUBS        R7, R0, #0 
    /*00008298*/ BEQ         RgnSetDisplayAttr_x8080
    RgnSetDisplayAttr_x829c:
    /*0000829c*/ MOV         R0, R7 
    /*000082a0*/ ADD         SP, SP, #196                @ 0xc4 
    /*000082a4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnSetDisplayAttr_x82a8:
    /*000082a8*/ MOV         R0, R8 
    /*000082ac*/ BL          0x00000000000002BC 
    /*000082b0*/ SUBS        R7, R0, #0 
    /*000082b4*/ BNE         RgnSetDisplayAttr_x829c
    /*000082b8*/ LDR         R8, [R4, #4] 
    /*000082bc*/ B           RgnSetDisplayAttr_x809c
    RgnSetDisplayAttr_x82c0:
    /*000082c0*/ BL          0x0000000000000290 
    /*000082c4*/ SUBS        R7, R0, #0 
    /*000082c8*/ BNE         RgnSetDisplayAttr_x829c
    /*000082cc*/ B           RgnSetDisplayAttr_x8088
    RgnSetDisplayAttr_x82d0:
    /*000082d0*/ BL          0x0000000000000290 
    /*000082d4*/ SUBS        R7, R0, #0 
    /*000082d8*/ BNE         RgnSetDisplayAttr_x829c
    /*000082dc*/ B           RgnSetDisplayAttr_x8090
    RgnSetDisplayAttr_x82e0:
    /*000082e0*/ MOV         R1, #3 
    /*000082e4*/ MOVW        R3, #5050                   @ 0x13ba 
    /*000082e8*/ MOV         R0, R1 
    /*000082ec*/ STR         R3, [SP] 
    /*000082f0*/ STR         R5, [SP, #4] 
    /*000082f4*/ MOVW        R2, #0                      @ .LC179 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2580] 
    /*000082f8*/ LDR         R3, [PC, #332]              @ 0x000000000000844c 
    /*000082fc*/ MOVT        R2, #0                      @ .LC179 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2580] 
    /*00008300*/ BL          0x0000000000008300          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008304*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00008308*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000830c*/ MOVW        R7, #32777                  @ 0x8009 
    /*00008310*/ BL          0x0000000000008310          @ osal_up [R_ARM_CALL: +0x0] 
    /*00008314*/ MOVT        R7, #40963                  @ 0xa003 
    /*00008318*/ B           RgnSetDisplayAttr_x829c
    RgnSetDisplayAttr_x831c:
    /*0000831c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00008320*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00008324*/ BL          0x0000000000008324          @ osal_up [R_ARM_CALL: +0x0] 
    /*00008328*/ LDR         R2, [R6] 
    /*0000832c*/ MOV         R1, #3 
    /*00008330*/ MOVW        R3, #5091                   @ 0x13e3 
    /*00008334*/ STR         R8, [SP, #4] 
    /*00008338*/ MOV         R0, R1 
    /*0000833c*/ STR         R2, [SP, #8] 
    /*00008340*/ MOVW        R2, #0                      @ .LC171 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2390] 
    /*00008344*/ STR         R3, [SP] 
    /*00008348*/ MOVT        R2, #0                      @ .LC171 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2390] 
    /*0000834c*/ LDR         R3, [PC, #248]              @ 0x000000000000844c 
    /*00008350*/ BL          0x0000000000008350          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008354*/ B           RgnSetDisplayAttr_x829c
    RgnSetDisplayAttr_x8358:
    /*00008358*/ ADD         R0, SP, #24 
    /*0000835c*/ BL          0x0000000000000324 
    /*00008360*/ SUBS        R7, R0, #0 
    /*00008364*/ BNE         RgnSetDisplayAttr_x8280
    /*00008368*/ B           RgnSetDisplayAttr_x8188
    RgnSetDisplayAttr_x836c:
    /*0000836c*/ MOV         R7, #65024                  @ 0xfe00 
    /*00008370*/ MOVT        R7, #65535                  @ 0xffff 
    /*00008374*/ B           RgnSetDisplayAttr_x829c
    RgnSetDisplayAttr_x8378:
    /*00008378*/ MOV         R1, #3 
    /*0000837c*/ MOVW        R3, #5030                   @ 0x13a6 
    /*00008380*/ MOV         R0, R1 
    /*00008384*/ STR         R3, [SP] 
    /*00008388*/ STR         R5, [SP, #4] 
    /*0000838c*/ MOVW        R2, #0                      @ .LC142 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c48] 
    /*00008390*/ LDR         R3, [PC, #180]              @ 0x000000000000844c 
    /*00008394*/ MOVT        R2, #0                      @ .LC142 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c48] 
    /*00008398*/ BL          0x0000000000008398          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000839c*/ MOV         R0, R9 
    /*000083a0*/ MOVW        R7, #32777                  @ 0x8009 
    /*000083a4*/ BL          0x00000000000083A4          @ osal_up [R_ARM_CALL: +0x0] 
    /*000083a8*/ MOVT        R7, #40963                  @ 0xa003 
    /*000083ac*/ B           RgnSetDisplayAttr_x829c
    RgnSetDisplayAttr_x83b0:
    /*000083b0*/ MOVW        R3, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb88] 
    /*000083b4*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000083b8*/ MOVT        R3, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb88] 
    /*000083bc*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000083c0*/ MOVW        R2, #5085                   @ 0x13dd 
    /*000083c4*/ LDR         R1, [PC, #128]              @ 0x000000000000844c 
    /*000083c8*/ BL          0x00000000000083C8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000083cc*/ B           RgnSetDisplayAttr_x81b0
    RgnSetDisplayAttr_x83d0:
    /*000083d0*/ MOVW        R3, #0                      @ .LC137 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b68] 
    /*000083d4*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000083d8*/ MOVT        R3, #0                      @ .LC137 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b68] 
    /*000083dc*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000083e0*/ MOVW        R2, #5121                   @ 0x1401 
    /*000083e4*/ LDR         R1, [PC, #96]               @ 0x000000000000844c 
    /*000083e8*/ BL          0x00000000000083E8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000083ec*/ B           RgnSetDisplayAttr_x8230
    RgnSetDisplayAttr_x83f0:
    /*000083f0*/ ADD         R1, SP, #20 
    /*000083f4*/ LDR         R0, [PC, #76]               @ 0x0000000000008448 
    /*000083f8*/ BL          0x00000000000083F8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000083fc*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00008400*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00008404*/ MVN         R7, #0 
    /*00008408*/ BL          0x0000000000008408          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000840c*/ B           RgnSetDisplayAttr_x829c
    RgnSetDisplayAttr_x8410:
    /*00008410*/ MOV         R1, #3 
    /*00008414*/ MOVW        R3, #5023                   @ 0x139f 
    /*00008418*/ MOV         R0, R1 
    /*0000841c*/ STR         R3, [SP] 
    /*00008420*/ STR         R5, [SP, #4] 
    /*00008424*/ MOVW        R2, #0                      @ .LC90 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x120c] 
    /*00008428*/ LDR         R3, [PC, #28]               @ 0x000000000000844c 
    /*0000842c*/ MOVT        R2, #0                      @ .LC90 [R_ARM_MOVT_ABS: .rodata.str1.4+0x120c] 
    /*00008430*/ BL          0x0000000000008430          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008434*/ MOV         R0, R9 
    /*00008438*/ MOVW        R7, #32773                  @ 0x8005 
    /*0000843c*/ BL          0x000000000000843C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00008440*/ MOVT        R7, #40963                  @ 0xa003 
    /*00008444*/ B           RgnSetDisplayAttr_x829c
    /*00008448*/ ANDEQ       R0, R0, R4 
    /*0000844c*/ ANDEQ       R0, R0, R8, ROR #9 
FUNC_END RgnSetDisplayAttr


FUNC_BEGIN RgnInit
    /*00008450*/ MOV         R0, #0 
    /*00008454*/ BX          LR 
FUNC_END RgnInit


FUNC_BEGIN RgnExit
    /*00008458*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*0000845c*/ MOV         R5, #512                    @ 0x200 
    /*00008460*/ LDR         R4, [PC, #340]              @ 0x00000000000085bc 
    /*00008464*/ MOV         R6, #256                    @ 0x100 
    /*00008468*/ MOVT        R6, #16 
    /*0000846c*/ MOV         R8, #0 
    /*00008470*/ ADD         R7, R4, R5 
    /*00008474*/ MOVT        R5, #32 
    RgnExit_x8478:
    /*00008478*/ LDR         R0, [R4] 
    /*0000847c*/ CMP         R0, #0 
    /*00008480*/ BEQ         RgnExit_x84fc
    /*00008484*/ BL          0x0000000000008484          @ RgnFreeBmpMem [R_ARM_CALL: .text+0x3060] 
    /*00008488*/ LDR         R0, [R4] 
    /*0000848c*/ MOV         R2, R0 
    /*00008490*/ LDR         R3, [R2, #308]!             @ 0x134 
    /*00008494*/ CMP         R3, R2 
    /*00008498*/ LDR         R9, [R3] 
    /*0000849c*/ BEQ         RgnExit_x84dc
    /*000084a0*/ MOV         R2, R9 
    RgnExit_x84a4:
    /*000084a4*/ LDR         R1, [R3, #4] 
    /*000084a8*/ MOV         R0, R3 
    /*000084ac*/ STR         R1, [R2, #4] 
    /*000084b0*/ STR         R2, [R1] 
    /*000084b4*/ STR         R6, [R3] 
    /*000084b8*/ STR         R5, [R3, #4] 
    /*000084bc*/ BL          0x00000000000084BC          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*000084c0*/ LDR         R0, [R4] 
    /*000084c4*/ LDR         R2, [R9] 
    /*000084c8*/ MOV         R3, R9 
    /*000084cc*/ ADD         R1, R0, #308                @ 0x134 
    /*000084d0*/ CMP         R9, R1 
    /*000084d4*/ MOV         R9, R2 
    /*000084d8*/ BNE         RgnExit_x84a4
    RgnExit_x84dc:
    /*000084dc*/ ADD         R0, R0, #304                @ 0x130 
    /*000084e0*/ BL          0x00000000000084E0          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*000084e4*/ LDR         R0, [R4] 
    /*000084e8*/ ADD         R0, R0, #300                @ 0x12c 
    /*000084ec*/ BL          0x00000000000084EC          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*000084f0*/ LDR         R0, [R4] 
    /*000084f4*/ BL          0x00000000000084F4          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*000084f8*/ STR         R8, [R4] 
    RgnExit_x84fc:
    /*000084fc*/ ADD         R4, R4, #4 
    /*00008500*/ CMP         R4, R7 
    /*00008504*/ BNE         RgnExit_x8478
    /*00008508*/ LDR         R4, [PC, #176]              @ 0x00000000000085c0 
    /*0000850c*/ LDR         R6, [PC, #176]              @ 0x00000000000085c4 
    RgnExit_x8510:
    /*00008510*/ ADD         R5, R4, #1200               @ 0x4b0 
    RgnExit_x8514:
    /*00008514*/ MOV         R0, R4 
    /*00008518*/ ADD         R4, R4, #20 
    /*0000851c*/ BL          0x000000000000851C          @ RgnFreeModInfo [R_ARM_CALL: .text+0x35b4] 
    /*00008520*/ CMP         R4, R5 
    /*00008524*/ BNE         RgnExit_x8514
    /*00008528*/ CMP         R6, R4 
    /*0000852c*/ BNE         RgnExit_x8510
    /*00008530*/ LDR         R4, [PC, #144]              @ 0x00000000000085c8 
    /*00008534*/ MOV         R6, #256                    @ 0x100 
    /*00008538*/ MOV         R5, #512                    @ 0x200 
    /*0000853c*/ MOVT        R6, #16 
    /*00008540*/ ADD         R7, R4, #32 
    /*00008544*/ MOVT        R5, #32 
    /*00008548*/ MOV         R8, #0 
    RgnExit_x854c:
    /*0000854c*/ LDR         R0, [R4] 
    /*00008550*/ CMP         R0, #0 
    /*00008554*/ BEQ         RgnExit_x85a8
    /*00008558*/ LDR         R3, [R0] 
    /*0000855c*/ CMP         R0, R3 
    /*00008560*/ LDR         R9, [R3] 
    /*00008564*/ BEQ         RgnExit_x85a0
    /*00008568*/ MOV         R2, R9 
    RgnExit_x856c:
    /*0000856c*/ LDR         R1, [R3, #4] 
    /*00008570*/ SUB         R0, R3, #4 
    /*00008574*/ STR         R1, [R2, #4] 
    /*00008578*/ STR         R2, [R1] 
    /*0000857c*/ STR         R6, [R3] 
    /*00008580*/ STR         R5, [R3, #4] 
    /*00008584*/ BL          0x0000000000008584          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00008588*/ LDR         R0, [R4] 
    /*0000858c*/ LDR         R2, [R9] 
    /*00008590*/ MOV         R3, R9 
    /*00008594*/ CMP         R0, R9 
    /*00008598*/ MOV         R9, R2 
    /*0000859c*/ BNE         RgnExit_x856c
    RgnExit_x85a0:
    /*000085a0*/ BL          0x00000000000085A0          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*000085a4*/ STR         R8, [R4] 
    RgnExit_x85a8:
    /*000085a8*/ ADD         R4, R4, #4 
    /*000085ac*/ CMP         R7, R4 
    /*000085b0*/ BNE         RgnExit_x854c
    /*000085b4*/ MOV         R0, #0 
    /*000085b8*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    /*000085bc*/ ANDEQ       R0, R0, R8 
    /*000085c0*/ ANDEQ       R0, R0, R4, ASR #4 
    /*000085c4*/ @           <UNDEFINED> instruction: 0x000019b4 
    /*000085c8*/ @           <UNDEFINED> instruction: 0x00001bb8 
FUNC_END RgnExit


FUNC_BEGIN RgnNotify
    /*000085cc*/ PUSH        {R4, R5, R6, LR} 
    /*000085d0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000085d4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000085d8*/ BL          0x00000000000085D8          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000085dc*/ CMP         R0, #0 
    /*000085e0*/ BNE         RgnNotify_x8624
    /*000085e4*/ LDR         R5, [PC, #68]               @ 0x0000000000008630 
    /*000085e8*/ ADD         R6, R5, #512                @ 0x200 
    RgnNotify_x85ec:
    /*000085ec*/ LDR         R4, [R5, #4]! 
    /*000085f0*/ CMP         R4, #0 
    /*000085f4*/ ADD         R0, R4, #304                @ 0x130 
    /*000085f8*/ BEQ         RgnNotify_x8608
    /*000085fc*/ BL          0x00000000000085FC          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00008600*/ ADD         R0, R4, #300                @ 0x12c 
    /*00008604*/ BL          0x0000000000008604          @ osal_wakeup [R_ARM_CALL: +0x0] 
    RgnNotify_x8608:
    /*00008608*/ CMP         R5, R6 
    /*0000860c*/ BNE         RgnNotify_x85ec
    /*00008610*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00008614*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00008618*/ BL          0x0000000000008618          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000861c*/ MOV         R0, #0 
    /*00008620*/ POP         {R4, R5, R6, PC} 
    RgnNotify_x8624:
    /*00008624*/ MOV         R0, #65024                  @ 0xfe00 
    /*00008628*/ MOVT        R0, #65535                  @ 0xffff 
    /*0000862c*/ POP         {R4, R5, R6, PC} 
    /*00008630*/ ANDEQ       R0, R0, R4 
FUNC_END RgnNotify


FUNC_BEGIN RGN_RegisterMod
    /*00008634*/ CMP         R0, #4 
    /*00008638*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000863c*/ MOV         R5, R0 
    /*00008640*/ SUB         SP, SP, #8 
    /*00008644*/ MOV         R6, R1 
    /*00008648*/ MOV         R4, R2 
    /*0000864c*/ BHI         RGN_RegisterMod_x86f4
    RGN_RegisterMod_x8650:
    /*00008650*/ CMP         R6, #0 
    /*00008654*/ BEQ         RGN_RegisterMod_x8780
    RGN_RegisterMod_x8658:
    /*00008658*/ CMP         R4, #0 
    /*0000865c*/ BEQ         RGN_RegisterMod_x8790
    RGN_RegisterMod_x8660:
    /*00008660*/ LDR         R3, [R4] 
    /*00008664*/ CMP         R3, #59                     @ 0x3b 
    /*00008668*/ BHI         RGN_RegisterMod_x8750
    /*0000866c*/ MOV         R1, #20 
    /*00008670*/ MOV         R2, #1200                   @ 0x4b0 
    /*00008674*/ MUL         R1, R1, R3 
    /*00008678*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000867c*/ MLA         R2, R2, R5, R1 
    /*00008680*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00008684*/ LDR         R1, [R4, #4] 
    /*00008688*/ ADD         R8, R3, #580                @ 0x244 
    /*0000868c*/ ADD         R3, R3, R2 
    /*00008690*/ ADD         R8, R8, R2 
    /*00008694*/ STR         R1, [R3, #580]              @ 0x244 
    /*00008698*/ MOV         R0, R8 
    /*0000869c*/ LDR         R2, [R4, #8] 
    /*000086a0*/ STR         R2, [R3, #584]              @ 0x248 
    /*000086a4*/ LDR         R2, [R4, #12] 
    /*000086a8*/ STR         R2, [R3, #588]              @ 0x24c 
    /*000086ac*/ BL          0x00000000000086AC          @ RgnMallocModInfo [R_ARM_CALL: .text+0x3424] 
    /*000086b0*/ SUBS        R7, R0, #0 
    /*000086b4*/ BEQ         RGN_RegisterMod_x870c
    /*000086b8*/ LDR         R3, [R4] 
    /*000086bc*/ MOV         R1, #3 
    /*000086c0*/ MOVW        IP, #5280                   @ 0x14a0 
    /*000086c4*/ MOVW        R2, #0                      @ .LC180 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x25c0] 
    /*000086c8*/ MOV         R0, R1 
    /*000086cc*/ STR         IP, [SP] 
    /*000086d0*/ STR         R3, [SP, #4] 
    /*000086d4*/ MOVT        R2, #0                      @ .LC180 [R_ARM_MOVT_ABS: .rodata.str1.4+0x25c0] 
    /*000086d8*/ LDR         R3, [PC, #192]              @ 0x00000000000087a0 
    /*000086dc*/ BL          0x00000000000086DC          @ HI_LOG [R_ARM_CALL: +0x0] 
    RGN_RegisterMod_x86e0:
    /*000086e0*/ MOV         R0, R8 
    /*000086e4*/ BL          0x00000000000086E4          @ RgnFreeModInfo [R_ARM_CALL: .text+0x35b4] 
    /*000086e8*/ MOV         R0, R7 
    /*000086ec*/ ADD         SP, SP, #8 
    /*000086f0*/ POP         {R4, R5, R6, R7, R8, PC} 
    RGN_RegisterMod_x86f4:
    /*000086f4*/ BL          0x00000000000002BC 
    /*000086f8*/ SUBS        R7, R0, #0 
    /*000086fc*/ BEQ         RGN_RegisterMod_x8650
    RGN_RegisterMod_x8700:
    /*00008700*/ MOV         R0, R7 
    /*00008704*/ ADD         SP, SP, #8 
    /*00008708*/ POP         {R4, R5, R6, R7, R8, PC} 
    RGN_RegisterMod_x870c:
    /*0000870c*/ MOV         R2, R6 
    /*00008710*/ MOV         R1, R5 
    /*00008714*/ LDR         R0, [R4] 
    /*00008718*/ BL          0x0000000000008718          @ RgnAddCapacityInfo [R_ARM_CALL: .text+0x3708] 
    /*0000871c*/ SUBS        R7, R0, #0 
    /*00008720*/ BEQ         RGN_RegisterMod_x8700
    /*00008724*/ LDR         R3, [R4] 
    /*00008728*/ MOV         R1, #3 
    /*0000872c*/ MOVW        IP, #5287                   @ 0x14a7 
    /*00008730*/ MOV         R0, R1 
    /*00008734*/ MOVW        R2, #0                      @ .LC181 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x25f8] 
    /*00008738*/ STR         IP, [SP] 
    /*0000873c*/ STR         R3, [SP, #4] 
    /*00008740*/ MOVT        R2, #0                      @ .LC181 [R_ARM_MOVT_ABS: .rodata.str1.4+0x25f8] 
    /*00008744*/ LDR         R3, [PC, #84]               @ 0x00000000000087a0 
    /*00008748*/ BL          0x0000000000008748          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000874c*/ B           RGN_RegisterMod_x86e0
    RGN_RegisterMod_x8750:
    /*00008750*/ MOV         R1, #3 
    /*00008754*/ STR         R3, [SP, #4] 
    /*00008758*/ MOV         R0, R1 
    /*0000875c*/ MOVW        R3, #5268                   @ 0x1494 
    /*00008760*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x41c] 
    /*00008764*/ STR         R3, [SP] 
    /*00008768*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x41c] 
    /*0000876c*/ LDR         R3, [PC, #44]               @ 0x00000000000087a0 
    /*00008770*/ MOVW        R7, #32771                  @ 0x8003 
    /*00008774*/ BL          0x0000000000008774          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008778*/ MOVT        R7, #40963                  @ 0xa003 
    /*0000877c*/ B           RGN_RegisterMod_x8700
    RGN_RegisterMod_x8780:
    /*00008780*/ BL          0x0000000000000290 
    /*00008784*/ SUBS        R7, R0, #0 
    /*00008788*/ BNE         RGN_RegisterMod_x8700
    /*0000878c*/ B           RGN_RegisterMod_x8658
    RGN_RegisterMod_x8790:
    /*00008790*/ BL          0x0000000000000290 
    /*00008794*/ SUBS        R7, R0, #0 
    /*00008798*/ BNE         RGN_RegisterMod_x8700
    /*0000879c*/ B           RGN_RegisterMod_x8660
    /*000087a0*/ STRDEQ      R0, [R0], -IP 
FUNC_END RGN_RegisterMod


FUNC_BEGIN RGN_UnRegisterMod
    /*000087a4*/ CMP         R0, #4 
    /*000087a8*/ PUSH        {R4, R5, LR} 
    /*000087ac*/ MOV         R4, R0 
    /*000087b0*/ SUB         SP, SP, #12 
    /*000087b4*/ MOV         R5, R1 
    /*000087b8*/ BHI         RGN_UnRegisterMod_x87f8
    RGN_UnRegisterMod_x87bc:
    /*000087bc*/ CMP         R5, #59                     @ 0x3b 
    /*000087c0*/ BHI         RGN_UnRegisterMod_x880c
    /*000087c4*/ MOV         R2, #1200                   @ 0x4b0 
    /*000087c8*/ MOV         R3, #20 
    /*000087cc*/ MUL         R2, R2, R4 
    /*000087d0*/ LDR         R0, [PC, #100]              @ 0x000000000000883c 
    /*000087d4*/ MLA         R3, R3, R5, R2 
    /*000087d8*/ ADD         R0, R0, R3 
    /*000087dc*/ BL          0x00000000000087DC          @ RgnFreeModInfo [R_ARM_CALL: .text+0x35b4] 
    /*000087e0*/ MOV         R0, R5 
    /*000087e4*/ MOV         R1, R4 
    /*000087e8*/ BL          0x00000000000087E8          @ RgnDelCapacityInfo [R_ARM_CALL: .text+0x37ec] 
    /*000087ec*/ MOV         R0, #0 
    RGN_UnRegisterMod_x87f0:
    /*000087f0*/ ADD         SP, SP, #12 
    /*000087f4*/ POP         {R4, R5, PC} 
    RGN_UnRegisterMod_x87f8:
    /*000087f8*/ BL          0x00000000000002BC 
    /*000087fc*/ CMP         R0, #0 
    /*00008800*/ BEQ         RGN_UnRegisterMod_x87bc
    /*00008804*/ ADD         SP, SP, #12 
    /*00008808*/ POP         {R4, R5, PC} 
    RGN_UnRegisterMod_x880c:
    /*0000880c*/ MOV         R1, #3 
    /*00008810*/ MOVW        R3, #5308                   @ 0x14bc 
    /*00008814*/ STR         R5, [SP, #4] 
    /*00008818*/ MOV         R0, R1 
    /*0000881c*/ STR         R3, [SP] 
    /*00008820*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x41c] 
    /*00008824*/ LDR         R3, [PC, #20]               @ 0x0000000000008840 
    /*00008828*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x41c] 
    /*0000882c*/ BL          0x000000000000882C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008830*/ MOVW        R0, #32771                  @ 0x8003 
    /*00008834*/ MOVT        R0, #40963                  @ 0xa003 
    /*00008838*/ B           RGN_UnRegisterMod_x87f0
    /*0000883c*/ ANDEQ       R0, R0, R4, ASR #4 
    /*00008840*/ ANDEQ       R0, R0, IP, LSL #10 
FUNC_END RGN_UnRegisterMod


FUNC_BEGIN RGN_GetRegion
    /*00008844*/ CMP         R0, #4 
    /*00008848*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000884c*/ MOV         R4, R0 
    /*00008850*/ SUB         SP, SP, #228                @ 0xe4 
    /*00008854*/ MOV         R7, R1 
    /*00008858*/ MOV         R6, R2 
    /*0000885c*/ BHI         RGN_GetRegion_x8894
    RGN_GetRegion_x8860:
    /*00008860*/ CMP         R7, #0 
    /*00008864*/ BEQ         RGN_GetRegion_x8acc
    RGN_GetRegion_x8868:
    /*00008868*/ CMP         R6, #0 
    /*0000886c*/ BEQ         RGN_GetRegion_x8aec
    RGN_GetRegion_x8870:
    /*00008870*/ MOV         R2, #0 
    /*00008874*/ MOV         R1, R7 
    /*00008878*/ MOV         R0, R4 
    /*0000887c*/ BL          0x000000000000887C          @ RgnCheckCapacityByModId [R_ARM_CALL: .text+0x4a70] 
    /*00008880*/ SUBS        R5, R0, #0 
    /*00008884*/ BEQ         RGN_GetRegion_x88ac
    RGN_GetRegion_x8888:
    /*00008888*/ MOV         R0, R5 
    /*0000888c*/ ADD         SP, SP, #228                @ 0xe4 
    /*00008890*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RGN_GetRegion_x8894:
    /*00008894*/ BL          0x00000000000002BC 
    /*00008898*/ SUBS        R5, R0, #0 
    /*0000889c*/ BEQ         RGN_GetRegion_x8860
    /*000088a0*/ MOV         R0, R5 
    /*000088a4*/ ADD         SP, SP, #228                @ 0xe4 
    /*000088a8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RGN_GetRegion_x88ac:
    /*000088ac*/ LDR         R0, [PC, #684]              @ 0x0000000000008b60 
    /*000088b0*/ ADD         R1, SP, #20 
    /*000088b4*/ BL          0x00000000000088B4          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000088b8*/ MOV         R1, R7 
    /*000088bc*/ MOV         R0, R4 
    /*000088c0*/ BL          0x00000000000088C0          @ RgnCheckMppChn [R_ARM_CALL: .text+0x13ac] 
    /*000088c4*/ SUBS        R5, R0, #0 
    /*000088c8*/ BNE         RGN_GetRegion_x8aa8
    /*000088cc*/ MOV         R1, R7 
    /*000088d0*/ MOV         R0, R4 
    /*000088d4*/ BL          0x00000000000088D4          @ RgnGetChnGetInfo [R_ARM_CALL: .text+0x1034] 
    /*000088d8*/ SUBS        R3, R0, #0 
    /*000088dc*/ STR         R3, [SP] 
    /*000088e0*/ BEQ         RGN_GetRegion_x8b0c
    /*000088e4*/ LDR         R2, [SP] 
    /*000088e8*/ MOV         R1, R7 
    /*000088ec*/ MOV         R0, R4 
    /*000088f0*/ LDR         R3, [R2, #4] 
    /*000088f4*/ STR         R3, [R6, #4] 
    /*000088f8*/ STR         R5, [R2, #4] 
    /*000088fc*/ BL          0x00000000000088FC          @ RgnGetChnManage [R_ARM_CALL: .text+0x11fc] 
    /*00008900*/ SUBS        SL, R0, #0 
    /*00008904*/ BEQ         RGN_GetRegion_x8b24
    /*00008908*/ LDR         R1, [SP] 
    /*0000890c*/ MOV         R2, #200                    @ 0xc8 
    /*00008910*/ LDR         R3, [R1] 
    /*00008914*/ CMP         R3, #0 
    /*00008918*/ BEQ         RGN_GetRegion_x8afc
    /*0000891c*/ ADD         R0, SP, #24 
    /*00008920*/ BL          0x0000000000008920          @ osal_memcpy [R_ARM_CALL: +0x0] 
    RGN_GetRegion_x8924:
    /*00008924*/ LDR         LR, [SL] 
    /*00008928*/ CMP         LR, #0 
    /*0000892c*/ BEQ         RGN_GetRegion_x8b2c
    /*00008930*/ LDR         R9, [SP] 
    /*00008934*/ MOV         R7, #0 
    /*00008938*/ MOVW        FP, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000893c*/ ADD         IP, SL, #20 
    /*00008940*/ MOVT        FP, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00008944*/ ADD         R1, SL, #120                @ 0x78 
    /*00008948*/ MOV         R2, R7 
    /*0000894c*/ STR         R4, [SP, #8] 
    /*00008950*/ STR         R6, [SP, #12] 
    /*00008954*/ STR         R5, [SP, #4] 
    RGN_GetRegion_x8958:
    /*00008958*/ LDR         R0, [IP, #4]! 
    /*0000895c*/ ADD         R2, R2, #1 
    /*00008960*/ ADD         R3, FP, R0, LSL #2 
    /*00008964*/ LDR         R3, [R3, #8] 
    /*00008968*/ CMP         R3, #0 
    /*0000896c*/ BEQ         RGN_GetRegion_x89dc
    /*00008970*/ LDR         R4, [R3] 
    /*00008974*/ CMP         R4, #0 
    /*00008978*/ BEQ         RGN_GetRegion_x89dc
    /*0000897c*/ MOV         R4, #12 
    /*00008980*/ LDR         R6, [R1] 
    /*00008984*/ MLA         R4, R4, R7, SL 
    /*00008988*/ CMP         R6, #0 
    /*0000898c*/ ADD         R5, R9, R7, LSL #2 
    /*00008990*/ ADD         R8, R4, #4160               @ 0x1040 
    /*00008994*/ BEQ         RGN_GetRegion_x89dc
    /*00008998*/ MOVW        LR, #4156                   @ 0x103c 
    /*0000899c*/ ADD         R7, R7, #1 
    /*000089a0*/ STR         R0, [R4, LR] 
    /*000089a4*/ MOVW        LR, #4152                   @ 0x1038 
    /*000089a8*/ LDR         R0, [R3, #216]              @ 0xd8 
    /*000089ac*/ STR         R0, [R4, LR] 
    /*000089b0*/ STR         R1, [R8] 
    /*000089b4*/ LDR         LR, [R3, #216]              @ 0xd8 
    /*000089b8*/ ADD         R4, R3, LR, LSL #2 
    /*000089bc*/ LDR         R0, [R4, #184]              @ 0xb8 
    /*000089c0*/ ADD         R0, R0, #1 
    /*000089c4*/ STR         R0, [R4, #184]              @ 0xb8 
    /*000089c8*/ MOV         R0, #1 
    /*000089cc*/ STR         R0, [R3, #180]              @ 0xb4 
    /*000089d0*/ STR         LR, [R5, #8] 
    /*000089d4*/ STR         R3, [R5, #104]              @ 0x68 
    /*000089d8*/ LDR         LR, [SL] 
    RGN_GetRegion_x89dc:
    /*000089dc*/ CMP         LR, R2 
    /*000089e0*/ ADD         R1, R1, #168                @ 0xa8 
    /*000089e4*/ BHI         RGN_GetRegion_x8958
    /*000089e8*/ LDR         R4, [SP, #8] 
    /*000089ec*/ ADD         R3, SL, #4096               @ 0x1000 
    /*000089f0*/ LDR         R6, [SP, #12] 
    /*000089f4*/ ADD         R3, R3, #56                 @ 0x38 
    /*000089f8*/ LDR         R2, [SP] 
    /*000089fc*/ CMP         R4, #0 
    /*00008a00*/ LDR         R5, [SP, #4] 
    /*00008a04*/ STR         R7, [R2] 
    /*00008a08*/ STR         R7, [R6] 
    /*00008a0c*/ STR         R3, [R6, #8] 
    /*00008a10*/ BEQ         RGN_GetRegion_x8a98
    /*00008a14*/ LDR         R1, [SP, #24] 
    /*00008a18*/ CMP         R1, #0 
    /*00008a1c*/ BEQ         RGN_GetRegion_x8a98
    RGN_GetRegion_x8a20:
    /*00008a20*/ MOV         R8, #0 
    /*00008a24*/ ADD         R9, SP, #124                @ 0x7c 
    /*00008a28*/ MOV         R6, R8 
    RGN_GetRegion_x8a2c:
    /*00008a2c*/ LDR         R4, [R9, #4]! 
    /*00008a30*/ CMP         R4, #0 
    /*00008a34*/ BEQ         RGN_GetRegion_x8a8c
    /*00008a38*/ LDR         R3, [R9, #-96]              @ 0xffffffa0 
    /*00008a3c*/ ADD         R3, R4, R3, LSL #2 
    /*00008a40*/ LDR         R2, [R3, #184]              @ 0xb8 
    /*00008a44*/ CMP         R2, #0 
    /*00008a48*/ BNE         RGN_GetRegion_x8ab8
    RGN_GetRegion_x8a4c:
    /*00008a4c*/ ADD         R0, R4, #304                @ 0x130 
    /*00008a50*/ BL          0x0000000000008A50          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00008a54*/ LDR         R2, [R4, #116]              @ 0x74 
    /*00008a58*/ CMP         R2, #0 
    /*00008a5c*/ BEQ         RGN_GetRegion_x8adc
    /*00008a60*/ ADD         R2, R2, #46                 @ 0x2e 
    /*00008a64*/ ADD         R3, R4, #184                @ 0xb8 
    /*00008a68*/ MOV         R1, #0 
    /*00008a6c*/ ADD         R2, R4, R2, LSL #2 
    RGN_GetRegion_x8a70:
    /*00008a70*/ LDR         R0, [R3], #4 
    /*00008a74*/ CMP         R3, R2 
    /*00008a78*/ ADD         R1, R1, R0 
    /*00008a7c*/ BNE         RGN_GetRegion_x8a70
    /*00008a80*/ CMP         R1, #0 
    /*00008a84*/ BEQ         RGN_GetRegion_x8adc
    RGN_GetRegion_x8a88:
    /*00008a88*/ LDR         R1, [SP, #24] 
    RGN_GetRegion_x8a8c:
    /*00008a8c*/ ADD         R8, R8, #1 
    /*00008a90*/ CMP         R1, R8 
    /*00008a94*/ BHI         RGN_GetRegion_x8a2c
    RGN_GetRegion_x8a98:
    /*00008a98*/ CMP         R7, #0 
    /*00008a9c*/ LDRNE       R3, [SL, #4] 
    /*00008aa0*/ ADDNE       R3, R3, #1 
    /*00008aa4*/ STRNE       R3, [SL, #4] 
    RGN_GetRegion_x8aa8:
    /*00008aa8*/ ADD         R1, SP, #20 
    /*00008aac*/ LDR         R0, [PC, #172]              @ 0x0000000000008b60 
    /*00008ab0*/ BL          0x0000000000008AB0          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00008ab4*/ B           RGN_GetRegion_x8888
    RGN_GetRegion_x8ab8:
    /*00008ab8*/ SUB         R2, R2, #1 
    /*00008abc*/ STR         R2, [R3, #184]              @ 0xb8 
    /*00008ac0*/ CMP         R2, #0 
    /*00008ac4*/ BNE         RGN_GetRegion_x8a8c
    /*00008ac8*/ B           RGN_GetRegion_x8a4c
    RGN_GetRegion_x8acc:
    /*00008acc*/ BL          0x0000000000000290 
    /*00008ad0*/ SUBS        R5, R0, #0 
    /*00008ad4*/ BNE         RGN_GetRegion_x8888
    /*00008ad8*/ B           RGN_GetRegion_x8868
    RGN_GetRegion_x8adc:
    /*00008adc*/ STR         R6, [R4, #180]              @ 0xb4 
    /*00008ae0*/ ADD         R0, R4, #300                @ 0x12c 
    /*00008ae4*/ BL          0x0000000000008AE4          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00008ae8*/ B           RGN_GetRegion_x8a88
    RGN_GetRegion_x8aec:
    /*00008aec*/ BL          0x0000000000000290 
    /*00008af0*/ SUBS        R5, R0, #0 
    /*00008af4*/ BNE         RGN_GetRegion_x8888
    /*00008af8*/ B           RGN_GetRegion_x8870
    RGN_GetRegion_x8afc:
    /*00008afc*/ MOV         R1, R5 
    /*00008b00*/ ADD         R0, SP, #24 
    /*00008b04*/ BL          0x0000000000008B04          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00008b08*/ B           RGN_GetRegion_x8924
    RGN_GetRegion_x8b0c:
    /*00008b0c*/ STR         R5, [R6, #4] 
    /*00008b10*/ ADD         R1, SP, #20 
    /*00008b14*/ STR         R5, [R6] 
    /*00008b18*/ LDR         R0, [PC, #64]               @ 0x0000000000008b60 
    /*00008b1c*/ BL          0x0000000000008B1C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00008b20*/ B           RGN_GetRegion_x8888
    RGN_GetRegion_x8b24:
    /*00008b24*/ STR         R5, [R6] 
    /*00008b28*/ B           RGN_GetRegion_x8aa8
    RGN_GetRegion_x8b2c:
    /*00008b2c*/ LDR         R2, [SP] 
    /*00008b30*/ CMP         R4, #0 
    /*00008b34*/ ADD         R3, SL, #4096               @ 0x1000 
    /*00008b38*/ ADD         R3, R3, #56                 @ 0x38 
    /*00008b3c*/ STR         LR, [R2] 
    /*00008b40*/ STR         LR, [R6] 
    /*00008b44*/ STR         R3, [R6, #8] 
    /*00008b48*/ BEQ         RGN_GetRegion_x8aa8
    /*00008b4c*/ LDR         R1, [SP, #24] 
    /*00008b50*/ CMP         R1, #0 
    /*00008b54*/ MOVNE       R7, LR 
    /*00008b58*/ BNE         RGN_GetRegion_x8a20
    /*00008b5c*/ B           RGN_GetRegion_x8aa8
    /*00008b60*/ ANDEQ       R0, R0, R4 
FUNC_END RGN_GetRegion


FUNC_BEGIN RGN_PutRegion
    /*00008b64*/ CMP         R0, #4 
    /*00008b68*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00008b6c*/ MOV         R5, R0 
    /*00008b70*/ SUB         SP, SP, #28 
    /*00008b74*/ MOV         R8, R1 
    /*00008b78*/ MOV         FP, R2 
    /*00008b7c*/ BHI         RGN_PutRegion_x8bac
    RGN_PutRegion_x8b80:
    /*00008b80*/ CMP         R8, #0 
    /*00008b84*/ BEQ         RGN_PutRegion_x8df4
    RGN_PutRegion_x8b88:
    /*00008b88*/ MOV         R2, #0 
    /*00008b8c*/ MOV         R1, R8 
    /*00008b90*/ MOV         R0, R5 
    /*00008b94*/ BL          0x0000000000008B94          @ RgnCheckCapacityByModId [R_ARM_CALL: .text+0x4a70] 
    /*00008b98*/ SUBS        SL, R0, #0 
    /*00008b9c*/ BEQ         RGN_PutRegion_x8bc4
    RGN_PutRegion_x8ba0:
    /*00008ba0*/ MOV         R0, SL 
    /*00008ba4*/ ADD         SP, SP, #28 
    /*00008ba8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RGN_PutRegion_x8bac:
    /*00008bac*/ BL          0x00000000000002BC 
    /*00008bb0*/ SUBS        SL, R0, #0 
    /*00008bb4*/ BEQ         RGN_PutRegion_x8b80
    /*00008bb8*/ MOV         R0, SL 
    /*00008bbc*/ ADD         SP, SP, #28 
    /*00008bc0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RGN_PutRegion_x8bc4:
    /*00008bc4*/ LDR         R0, [PC, #624]              @ 0x0000000000008e3c 
    /*00008bc8*/ ADD         R1, SP, #20 
    /*00008bcc*/ BL          0x0000000000008BCC          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00008bd0*/ MOV         R1, R8 
    /*00008bd4*/ MOV         R0, R5 
    /*00008bd8*/ BL          0x0000000000008BD8          @ RgnCheckMppChn [R_ARM_CALL: .text+0x13ac] 
    /*00008bdc*/ SUBS        SL, R0, #0 
    /*00008be0*/ BNE         RGN_PutRegion_x8d04
    /*00008be4*/ CMP         R5, #0 
    /*00008be8*/ BNE         RGN_PutRegion_x8e04
    /*00008bec*/ CMP         FP, #0 
    /*00008bf0*/ LDRNE       R3, [FP] 
    /*00008bf4*/ MOVNE       R9, R5 
    /*00008bf8*/ STRNE       R3, [SP] 
    /*00008bfc*/ BEQ         RGN_PutRegion_x8e28
    RGN_PutRegion_x8c00:
    /*00008c00*/ LDR         R3, [SP] 
    /*00008c04*/ CMP         R3, #0 
    /*00008c08*/ BEQ         RGN_PutRegion_x8ce0
    /*00008c0c*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*00008c10*/ STR         SL, [SP, #8] 
    /*00008c14*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*00008c18*/ LDR         SL, [SP] 
    /*00008c1c*/ ADD         R9, R9, #8 
    /*00008c20*/ STR         R8, [SP, #12] 
    /*00008c24*/ MOV         R7, #0 
    /*00008c28*/ MOV         R8, R2 
    /*00008c2c*/ MOVW        R3, #0                      @ .LC182 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x262c] 
    /*00008c30*/ MOVT        R3, #0                      @ .LC182 [R_ARM_MOVT_ABS: .rodata.str1.4+0x262c] 
    /*00008c34*/ STR         R3, [SP, #4] 
    RGN_PutRegion_x8c38:
    /*00008c38*/ CMP         R5, #0 
    /*00008c3c*/ BEQ         RGN_PutRegion_x8d14
    /*00008c40*/ LDR         R4, [R9, #96]               @ 0x60 
    /*00008c44*/ LDR         R6, [R9] 
    /*00008c48*/ CMP         R4, #0 
    /*00008c4c*/ BEQ         RGN_PutRegion_x8cc8
    /*00008c50*/ CMP         R5, #3 
    /*00008c54*/ BEQ         RGN_PutRegion_x8d3c
    /*00008c58*/ LDR         R3, [R4, #116]              @ 0x74 
    /*00008c5c*/ CMP         R6, R3 
    /*00008c60*/ BEQ         RGN_PutRegion_x8c7c
    /*00008c64*/ MOVW        R3, #0                      @ .LC183 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x265c] 
    /*00008c68*/ MOVW        R2, #5551                   @ 0x15af 
    /*00008c6c*/ MOVT        R3, #0                      @ .LC183 [R_ARM_MOVT_ABS: .rodata.str1.4+0x265c] 
    /*00008c70*/ LDR         R1, [PC, #456]              @ 0x0000000000008e40 
    /*00008c74*/ MOV         R0, R8 
    /*00008c78*/ BL          0x0000000000008C78          @ osal_panic [R_ARM_CALL: +0x0] 
    RGN_PutRegion_x8c7c:
    /*00008c7c*/ ADD         R6, R4, R6, LSL #2 
    /*00008c80*/ LDR         R3, [R6, #184]              @ 0xb8 
    /*00008c84*/ CMP         R3, #0 
    /*00008c88*/ BNE         RGN_PutRegion_x8d60
    RGN_PutRegion_x8c8c:
    /*00008c8c*/ ADD         R0, R4, #304                @ 0x130 
    /*00008c90*/ BL          0x0000000000008C90          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00008c94*/ LDR         R2, [R4, #116]              @ 0x74 
    /*00008c98*/ CMP         R2, #0 
    /*00008c9c*/ BEQ         RGN_PutRegion_x8d74
    /*00008ca0*/ ADD         R2, R2, #46                 @ 0x2e 
    /*00008ca4*/ ADD         R3, R4, #184                @ 0xb8 
    /*00008ca8*/ MOV         R1, #0 
    /*00008cac*/ ADD         R2, R4, R2, LSL #2 
    RGN_PutRegion_x8cb0:
    /*00008cb0*/ LDR         R0, [R3], #4 
    /*00008cb4*/ CMP         R2, R3 
    /*00008cb8*/ ADD         R1, R1, R0 
    /*00008cbc*/ BNE         RGN_PutRegion_x8cb0
    /*00008cc0*/ CMP         R1, #0 
    /*00008cc4*/ BEQ         RGN_PutRegion_x8d74
    RGN_PutRegion_x8cc8:
    /*00008cc8*/ ADD         R7, R7, #1 
    /*00008ccc*/ ADD         R9, R9, #4 
    /*00008cd0*/ CMP         SL, R7 
    /*00008cd4*/ BNE         RGN_PutRegion_x8c38
    /*00008cd8*/ LDR         SL, [SP, #8] 
    /*00008cdc*/ LDR         R8, [SP, #12] 
    RGN_PutRegion_x8ce0:
    /*00008ce0*/ MOV         R1, R8 
    /*00008ce4*/ MOV         R0, R5 
    /*00008ce8*/ BL          0x0000000000008CE8          @ RgnGetChnManage [R_ARM_CALL: .text+0x11fc] 
    /*00008cec*/ CMP         R0, #0 
    /*00008cf0*/ BEQ         RGN_PutRegion_x8d04
    /*00008cf4*/ LDR         R3, [R0] 
    /*00008cf8*/ LDR         R2, [SP] 
    /*00008cfc*/ CMP         R2, R3 
    /*00008d00*/ BEQ         RGN_PutRegion_x8d88
    RGN_PutRegion_x8d04:
    /*00008d04*/ ADD         R1, SP, #20 
    /*00008d08*/ LDR         R0, [PC, #300]              @ 0x0000000000008e3c 
    /*00008d0c*/ BL          0x0000000000008D0C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00008d10*/ B           RGN_PutRegion_x8ba0
    RGN_PutRegion_x8d14:
    /*00008d14*/ LDR         R2, [FP, #4] 
    /*00008d18*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00008d1c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00008d20*/ ADD         R1, R2, R7, LSL #3 
    /*00008d24*/ LDR         R6, [R2, R7, LSL #3] 
    /*00008d28*/ LDR         R2, [R1, #4] 
    /*00008d2c*/ ADD         R3, R3, R2, LSL #2 
    /*00008d30*/ LDR         R4, [R3, #8] 
    /*00008d34*/ CMP         R4, #0 
    /*00008d38*/ BEQ         RGN_PutRegion_x8cc8
    RGN_PutRegion_x8d3c:
    /*00008d3c*/ LDR         R3, [R4, #116]              @ 0x74 
    /*00008d40*/ CMP         R3, R6 
    /*00008d44*/ BHI         RGN_PutRegion_x8c7c
    /*00008d48*/ LDR         R3, [SP, #4] 
    /*00008d4c*/ MOVW        R2, #5547                   @ 0x15ab 
    /*00008d50*/ LDR         R1, [PC, #232]              @ 0x0000000000008e40 
    /*00008d54*/ MOV         R0, R8 
    /*00008d58*/ BL          0x0000000000008D58          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00008d5c*/ B           RGN_PutRegion_x8c7c
    RGN_PutRegion_x8d60:
    /*00008d60*/ SUB         R3, R3, #1 
    /*00008d64*/ STR         R3, [R6, #184]              @ 0xb8 
    /*00008d68*/ CMP         R3, #0 
    /*00008d6c*/ BNE         RGN_PutRegion_x8cc8
    /*00008d70*/ B           RGN_PutRegion_x8c8c
    RGN_PutRegion_x8d74:
    /*00008d74*/ MOV         R3, #0 
    /*00008d78*/ ADD         R0, R4, #300                @ 0x12c 
    /*00008d7c*/ STR         R3, [R4, #180]              @ 0xb4 
    /*00008d80*/ BL          0x0000000000008D80          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00008d84*/ B           RGN_PutRegion_x8cc8
    RGN_PutRegion_x8d88:
    /*00008d88*/ LDR         R3, [R0, #4] 
    /*00008d8c*/ SUB         R3, R3, #1 
    /*00008d90*/ STR         R3, [R0, #4] 
    /*00008d94*/ CMP         R3, #0 
    /*00008d98*/ BNE         RGN_PutRegion_x8d04
    /*00008d9c*/ LDR         R3, [R0, #20] 
    /*00008da0*/ CMP         R3, #2 
    /*00008da4*/ BNE         RGN_PutRegion_x8d04
    /*00008da8*/ MOV         R1, R8 
    /*00008dac*/ MOV         R0, R5 
    /*00008db0*/ LDR         R4, [R8] 
    /*00008db4*/ BL          0x0000000000008DB4          @ RgnGetpstModInfo [R_ARM_CALL: .text+0xd20] 
    /*00008db8*/ MOV         R3, #1200                   @ 0x4b0 
    /*00008dbc*/ LDR         R2, [R8, #4] 
    /*00008dc0*/ MUL         R3, R3, R5 
    /*00008dc4*/ LDR         R1, [R8, #8] 
    /*00008dc8*/ MOV         IP, #20 
    /*00008dcc*/ MLA         R4, IP, R4, R3 
    /*00008dd0*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00008dd4*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00008dd8*/ ADD         R3, R3, R4 
    /*00008ddc*/ LDR         R3, [R3, #592]              @ 0x250 
    /*00008de0*/ LDR         R0, [R0, #4] 
    /*00008de4*/ MLA         R0, R0, R2, R1 
    /*00008de8*/ ADD         R0, R3, R0, LSL #2 
    /*00008dec*/ BL          0x0000000000008DEC          @ RgnFreeChnMng [R_ARM_CALL: .text+0x338c] 
    /*00008df0*/ B           RGN_PutRegion_x8d04
    RGN_PutRegion_x8df4:
    /*00008df4*/ BL          0x0000000000000290 
    /*00008df8*/ SUBS        SL, R0, #0 
    /*00008dfc*/ BNE         RGN_PutRegion_x8ba0
    /*00008e00*/ B           RGN_PutRegion_x8b88
    RGN_PutRegion_x8e04:
    /*00008e04*/ MOV         R1, R8 
    /*00008e08*/ MOV         R0, R5 
    /*00008e0c*/ BL          0x0000000000008E0C          @ RgnGetChnGetInfo [R_ARM_CALL: .text+0x1034] 
    /*00008e10*/ SUBS        R9, R0, #0 
    /*00008e14*/ LDRNE       R3, [R9] 
    /*00008e18*/ STRNE       SL, [R9] 
    /*00008e1c*/ STRNE       R3, [SP] 
    /*00008e20*/ BNE         RGN_PutRegion_x8c00
    /*00008e24*/ B           RGN_PutRegion_x8d04
    RGN_PutRegion_x8e28:
    /*00008e28*/ ADD         R1, SP, #20 
    /*00008e2c*/ LDR         R0, [PC, #8]                @ 0x0000000000008e3c 
    /*00008e30*/ BL          0x0000000000008E30          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00008e34*/ MVN         SL, #0 
    /*00008e38*/ B           RGN_PutRegion_x8ba0
    /*00008e3c*/ ANDEQ       R0, R0, R4 
    /*00008e40*/ ANDEQ       R0, R0, R0, LSR #10 
FUNC_END RGN_PutRegion


FUNC_BEGIN RGN_SetModifyFalse
    /*00008e44*/ CMP         R0, #4 
    /*00008e48*/ PUSH        {R4, R5, R6, LR} 
    /*00008e4c*/ MOV         R4, R0 
    /*00008e50*/ SUB         SP, SP, #8 
    /*00008e54*/ MOV         R5, R1 
    /*00008e58*/ BHI         RGN_SetModifyFalse_x8e88
    RGN_SetModifyFalse_x8e5c:
    /*00008e5c*/ CMP         R5, #0 
    /*00008e60*/ BEQ         RGN_SetModifyFalse_x8ee8
    RGN_SetModifyFalse_x8e64:
    /*00008e64*/ MOV         R2, #0 
    /*00008e68*/ MOV         R1, R5 
    /*00008e6c*/ MOV         R0, R4 
    /*00008e70*/ BL          0x0000000000008E70          @ RgnCheckCapacityByModId [R_ARM_CALL: .text+0x4a70] 
    /*00008e74*/ SUBS        R6, R0, #0 
    /*00008e78*/ BEQ         RGN_SetModifyFalse_x8ea0
    RGN_SetModifyFalse_x8e7c:
    /*00008e7c*/ MOV         R0, R6 
    /*00008e80*/ ADD         SP, SP, #8 
    /*00008e84*/ POP         {R4, R5, R6, PC} 
    RGN_SetModifyFalse_x8e88:
    /*00008e88*/ BL          0x00000000000002BC 
    /*00008e8c*/ SUBS        R6, R0, #0 
    /*00008e90*/ BEQ         RGN_SetModifyFalse_x8e5c
    /*00008e94*/ MOV         R0, R6 
    /*00008e98*/ ADD         SP, SP, #8 
    /*00008e9c*/ POP         {R4, R5, R6, PC} 
    RGN_SetModifyFalse_x8ea0:
    /*00008ea0*/ MOV         R1, R5 
    /*00008ea4*/ MOV         R0, R4 
    /*00008ea8*/ BL          0x0000000000008EA8          @ RgnCheckMppChn [R_ARM_CALL: .text+0x13ac] 
    /*00008eac*/ SUBS        R6, R0, #0 
    /*00008eb0*/ BNE         RGN_SetModifyFalse_x8e7c
    /*00008eb4*/ MOV         R0, R4 
    /*00008eb8*/ MOV         R1, R5 
    /*00008ebc*/ BL          0x0000000000008EBC          @ RgnGetChnGetInfo [R_ARM_CALL: .text+0x1034] 
    /*00008ec0*/ SUBS        R4, R0, #0 
    /*00008ec4*/ BEQ         RGN_SetModifyFalse_x8e7c
    /*00008ec8*/ ADD         R1, SP, #4 
    /*00008ecc*/ LDR         R0, [PC, #36]               @ 0x0000000000008ef8 
    /*00008ed0*/ BL          0x0000000000008ED0          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00008ed4*/ ADD         R1, SP, #4 
    /*00008ed8*/ STR         R6, [R4, #4] 
    /*00008edc*/ LDR         R0, [PC, #20]               @ 0x0000000000008ef8 
    /*00008ee0*/ BL          0x0000000000008EE0          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00008ee4*/ B           RGN_SetModifyFalse_x8e7c
    RGN_SetModifyFalse_x8ee8:
    /*00008ee8*/ BL          0x0000000000000290 
    /*00008eec*/ SUBS        R6, R0, #0 
    /*00008ef0*/ BNE         RGN_SetModifyFalse_x8e7c
    /*00008ef4*/ B           RGN_SetModifyFalse_x8e64
    /*00008ef8*/ ANDEQ       R0, R0, R4 
FUNC_END RGN_SetModifyFalse


FUNC_BEGIN RgnGetBatchRgnGroup
    /*00008efc*/ LDR         R3, [PC, #32]               @ 0x0000000000008f24 
    /*00008f00*/ MOV         R0, #0 
    RgnGetBatchRgnGroup_x8f04:
    /*00008f04*/ LDR         R2, [R3, #4]! 
    /*00008f08*/ CMP         R2, #0 
    /*00008f0c*/ BXEQ        LR 
    /*00008f10*/ ADD         R0, R0, #1 
    /*00008f14*/ CMP         R0, #8 
    /*00008f18*/ BNE         RgnGetBatchRgnGroup_x8f04
    /*00008f1c*/ MVN         R0, #0 
    /*00008f20*/ BX          LR 
    /*00008f24*/ @           <UNDEFINED> instruction: 0x00001bb4 
FUNC_END RgnGetBatchRgnGroup


FUNC_BEGIN RgnBatchBegin
    /*00008f28*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00008f2c*/ SUBS        R3, R0, #0 
    /*00008f30*/ SUB         SP, SP, #20 
    /*00008f34*/ MOV         SL, R1 
    /*00008f38*/ MOV         R8, R2 
    /*00008f3c*/ STR         R3, [SP, #8] 
    /*00008f40*/ BEQ         RgnBatchBegin_x9000
    RgnBatchBegin_x8f44:
    /*00008f44*/ MOV         R0, SL 
    /*00008f48*/ BL          0x0000000000008F48          @ RGNCHECK_HANDLENUM [R_ARM_CALL: .text+0x3b8] 
    /*00008f4c*/ SUBS        R4, R0, #0 
    /*00008f50*/ BNE         RgnBatchBegin_x8fcc
    /*00008f54*/ CMP         SL, #0 
    /*00008f58*/ SUBNE       R6, R8, #4 
    /*00008f5c*/ MOVNE       R5, R4 
    /*00008f60*/ BNE         RgnBatchBegin_x8fec
    RgnBatchBegin_x8f64:
    /*00008f64*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00008f68*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00008f6c*/ BL          0x0000000000008F6C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00008f70*/ SUBS        R6, R0, #0 
    /*00008f74*/ LDREQ       R2, [PC, #688]              @ 0x000000000000922c 
    /*00008f78*/ MOVEQ       R5, R6 
    /*00008f7c*/ BNE         RgnBatchBegin_x91d0
    RgnBatchBegin_x8f80:
    /*00008f80*/ LDR         R3, [R2, #4]! 
    /*00008f84*/ CMP         R3, #0 
    /*00008f88*/ BEQ         RgnBatchBegin_x9018
    /*00008f8c*/ ADD         R5, R5, #1 
    /*00008f90*/ CMP         R5, #8 
    /*00008f94*/ BNE         RgnBatchBegin_x8f80
    /*00008f98*/ MOVW        R3, #5677                   @ 0x162d 
    /*00008f9c*/ STR         R3, [SP] 
    /*00008fa0*/ LDR         R3, [PC, #648]              @ 0x0000000000009230 
    /*00008fa4*/ MOV         R1, #3 
    /*00008fa8*/ MOVW        R2, #0                      @ .LC186 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x26f0] 
    /*00008fac*/ MOVT        R2, #0                      @ .LC186 [R_ARM_MOVT_ABS: .rodata.str1.4+0x26f0] 
    /*00008fb0*/ MOV         R0, R1 
    /*00008fb4*/ MOVW        R4, #32777                  @ 0x8009 
    RgnBatchBegin_x8fb8:
    /*00008fb8*/ BL          0x0000000000008FB8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00008fbc*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00008fc0*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00008fc4*/ MOVT        R4, #40963                  @ 0xa003 
    /*00008fc8*/ BL          0x0000000000008FC8          @ osal_up [R_ARM_CALL: +0x0] 
    RgnBatchBegin_x8fcc:
    /*00008fcc*/ MOV         R0, R4 
    /*00008fd0*/ ADD         SP, SP, #20 
    /*00008fd4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnBatchBegin_x8fd8:
    /*00008fd8*/ BL          0x00000000000002FC 
    /*00008fdc*/ CMP         R0, #0 
    /*00008fe0*/ BNE         RgnBatchBegin_x9010
    RgnBatchBegin_x8fe4:
    /*00008fe4*/ CMP         R5, SL 
    /*00008fe8*/ BEQ         RgnBatchBegin_x8f64
    RgnBatchBegin_x8fec:
    /*00008fec*/ LDR         R0, [R6, #4]! 
    /*00008ff0*/ ADD         R5, R5, #1 
    /*00008ff4*/ CMP         R0, #127                    @ 0x7f 
    /*00008ff8*/ BLS         RgnBatchBegin_x8fe4
    /*00008ffc*/ B           RgnBatchBegin_x8fd8
    RgnBatchBegin_x9000:
    /*00009000*/ BL          0x0000000000000290 
    /*00009004*/ SUBS        R4, R0, #0 
    /*00009008*/ BNE         RgnBatchBegin_x8fcc
    /*0000900c*/ B           RgnBatchBegin_x8f44
    RgnBatchBegin_x9010:
    /*00009010*/ MOV         R4, R0 
    /*00009014*/ B           RgnBatchBegin_x8fcc
    RgnBatchBegin_x9018:
    /*00009018*/ CMP         SL, #0 
    /*0000901c*/ SUBNE       R7, R8, #4 
    /*00009020*/ MOVNE       R9, R3 
    /*00009024*/ MOVNE       FP, R7 
    /*00009028*/ BEQ         RgnBatchBegin_x91dc
    RgnBatchBegin_x902c:
    /*0000902c*/ LDR         R0, [FP, #4]! 
    /*00009030*/ ADD         R9, R9, #1 
    /*00009034*/ BL          0x0000000000009034          @ RgnCheckBatch [R_ARM_CALL: .text+0x2b10] 
    /*00009038*/ CMP         R0, #0 
    /*0000903c*/ BNE         RgnBatchBegin_x919c
    /*00009040*/ CMP         R9, SL 
    /*00009044*/ BNE         RgnBatchBegin_x902c
    /*00009048*/ MOV         R1, R0 
    /*0000904c*/ MOV         R0, #12 
    /*00009050*/ BL          0x0000000000009050          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*00009054*/ MOVW        SL, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00009058*/ LSL         FP, R5, #2 
    /*0000905c*/ MOVT        SL, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00009060*/ ADD         R2, SL, FP 
    /*00009064*/ CMP         R0, #0 
    /*00009068*/ STR         R0, [R2, #-1088]            @ 0xfffffbc0 
    /*0000906c*/ BEQ         RgnBatchBegin_x9208
    /*00009070*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00009074*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00009078*/ STR         R4, [SP, #12] 
    /*0000907c*/ MOV         R4, R6 
    /*00009080*/ STR         R0, [R0] 
    /*00009084*/ MOV         R6, R7 
    /*00009088*/ STR         R0, [R0, #4] 
    /*0000908c*/ MOV         R7, R3 
    /*00009090*/ B           RgnBatchBegin_x90cc
    RgnBatchBegin_x9094:
    /*00009094*/ LDR         LR, [R6, #4] 
    /*00009098*/ ADD         R4, R4, #1 
    /*0000909c*/ CMP         R9, R4 
    /*000090a0*/ STR         LR, [R3], #4 
    /*000090a4*/ LDR         IP, [IP, #-1088]            @ 0xfffffbc0 
    /*000090a8*/ LDR         LR, [IP, #4] 
    /*000090ac*/ STR         R3, [IP, #4] 
    /*000090b0*/ STMIB       R0, {IP, LR} 
    /*000090b4*/ STR         R3, [LR] 
    /*000090b8*/ LDR         R3, [R6, #4]! 
    /*000090bc*/ ADD         R3, R7, R3, LSL #2 
    /*000090c0*/ LDR         R3, [R3, #8] 
    /*000090c4*/ STR         R1, [R3, #208]              @ 0xd0 
    /*000090c8*/ BEQ         RgnBatchBegin_x91b4
    RgnBatchBegin_x90cc:
    /*000090cc*/ MOV         R1, #0 
    /*000090d0*/ MOV         R0, #12 
    /*000090d4*/ BL          0x00000000000090D4          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*000090d8*/ ADD         IP, SL, FP 
    /*000090dc*/ MOV         R1, #1 
    /*000090e0*/ CMP         R0, #0 
    /*000090e4*/ MOV         R3, R0 
    /*000090e8*/ BNE         RgnBatchBegin_x9094
    /*000090ec*/ LDR         R3, [R8, R4, LSL #2] 
    /*000090f0*/ MOV         R1, #3 
    /*000090f4*/ ADD         R4, SL, FP 
    /*000090f8*/ MOVW        IP, #5711                   @ 0x164f 
    /*000090fc*/ MOV         R0, R1 
    /*00009100*/ MOVW        R2, #0                      @ .LC185 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x26b8] 
    /*00009104*/ STR         R3, [SP, #4] 
    /*00009108*/ MOVT        R2, #0                      @ .LC185 [R_ARM_MOVT_ABS: .rodata.str1.4+0x26b8] 
    /*0000910c*/ LDR         R3, [PC, #284]              @ 0x0000000000009230 
    /*00009110*/ STR         IP, [SP] 
    /*00009114*/ BL          0x0000000000009114          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00009118*/ LDR         R0, [R4, #-1088]            @ 0xfffffbc0 
    /*0000911c*/ LDR         R3, [R0] 
    /*00009120*/ CMP         R3, R0 
    /*00009124*/ LDR         R7, [R3] 
    /*00009128*/ BEQ         RgnBatchBegin_x9174
    /*0000912c*/ MOV         R6, #256                    @ 0x100 
    /*00009130*/ MOV         R5, #512                    @ 0x200 
    /*00009134*/ MOVT        R6, #16 
    /*00009138*/ MOVT        R5, #32 
    /*0000913c*/ MOV         R2, R7 
    RgnBatchBegin_x9140:
    /*00009140*/ LDR         R1, [R3, #4] 
    /*00009144*/ SUB         R0, R3, #4 
    /*00009148*/ STR         R1, [R2, #4] 
    /*0000914c*/ STR         R2, [R1] 
    /*00009150*/ STR         R6, [R3] 
    /*00009154*/ STR         R5, [R3, #4] 
    /*00009158*/ BL          0x0000000000009158          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000915c*/ LDR         R0, [R4, #-1088]            @ 0xfffffbc0 
    /*00009160*/ LDR         R2, [R7] 
    /*00009164*/ MOV         R3, R7 
    /*00009168*/ CMP         R0, R7 
    /*0000916c*/ MOV         R7, R2 
    /*00009170*/ BNE         RgnBatchBegin_x9140
    RgnBatchBegin_x9174:
    /*00009174*/ ADD         SL, SL, FP 
    /*00009178*/ BL          0x0000000000009178          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000917c*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00009180*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00009184*/ MOV         R3, #0 
    /*00009188*/ MOVW        R4, #32780                  @ 0x800c 
    /*0000918c*/ STR         R3, [SL, #-1088]            @ 0xfffffbc0 
    /*00009190*/ MOVT        R4, #40963                  @ 0xa003 
    /*00009194*/ BL          0x0000000000009194          @ osal_up [R_ARM_CALL: +0x0] 
    /*00009198*/ B           RgnBatchBegin_x8fcc
    RgnBatchBegin_x919c:
    /*0000919c*/ MOV         R1, R0 
    /*000091a0*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000091a4*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000091a8*/ MOV         R4, R1 
    /*000091ac*/ BL          0x00000000000091AC          @ osal_up [R_ARM_CALL: +0x0] 
    /*000091b0*/ B           RgnBatchBegin_x8fcc
    RgnBatchBegin_x91b4:
    /*000091b4*/ LDR         R4, [SP, #12] 
    RgnBatchBegin_x91b8:
    /*000091b8*/ LDR         R3, [SP, #8] 
    /*000091bc*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000091c0*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000091c4*/ STR         R5, [R3] 
    /*000091c8*/ BL          0x00000000000091C8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000091cc*/ B           RgnBatchBegin_x8fcc
    RgnBatchBegin_x91d0:
    /*000091d0*/ MOV         R4, #65024                  @ 0xfe00 
    /*000091d4*/ MOVT        R4, #65535                  @ 0xffff 
    /*000091d8*/ B           RgnBatchBegin_x8fcc
    RgnBatchBegin_x91dc:
    /*000091dc*/ MOV         R1, SL 
    /*000091e0*/ MOV         R0, #12 
    /*000091e4*/ BL          0x00000000000091E4          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*000091e8*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*000091ec*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*000091f0*/ ADD         R3, R3, R5, LSL #2 
    /*000091f4*/ CMP         R0, #0 
    /*000091f8*/ STR         R0, [R3, #-1088]            @ 0xfffffbc0 
    /*000091fc*/ STRNE       R0, [R0] 
    /*00009200*/ STRNE       R0, [R0, #4] 
    /*00009204*/ BNE         RgnBatchBegin_x91b8
    RgnBatchBegin_x9208:
    /*00009208*/ MOV         R1, #3 
    /*0000920c*/ MOVW        R3, #5697                   @ 0x1641 
    /*00009210*/ MOVW        R2, #0                      @ .LC184 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x268c] 
    /*00009214*/ STR         R3, [SP] 
    /*00009218*/ MOV         R0, R1 
    /*0000921c*/ LDR         R3, [PC, #12]               @ 0x0000000000009230 
    /*00009220*/ MOVT        R2, #0                      @ .LC184 [R_ARM_MOVT_ABS: .rodata.str1.4+0x268c] 
    /*00009224*/ MOVW        R4, #32780                  @ 0x800c 
    /*00009228*/ B           RgnBatchBegin_x8fb8
    /*0000922c*/ @           <UNDEFINED> instruction: 0x00001bb4 
    /*00009230*/ ANDEQ       R0, R0, R0, LSR R5 
FUNC_END RgnBatchBegin


FUNC_BEGIN RgnBatchEnd
    /*00009234*/ CMP         R0, #7 
    /*00009238*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000923c*/ MOV         R4, R0 
    /*00009240*/ SUB         SP, SP, #44                 @ 0x2c 
    /*00009244*/ BHI         RgnBatchEnd_x94ac
    /*00009248*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000924c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00009250*/ STR         R3, [SP, #12] 
    /*00009254*/ MOV         R0, R3 
    /*00009258*/ BL          0x0000000000009258          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*0000925c*/ CMP         R0, #0 
    /*00009260*/ BNE         RgnBatchEnd_x94dc
    /*00009264*/ LSL         R2, R4, #2 
    /*00009268*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*0000926c*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*00009270*/ STR         R3, [SP, #16] 
    /*00009274*/ ADD         R5, R3, R2 
    /*00009278*/ STR         R2, [SP, #20] 
    /*0000927c*/ LDR         R3, [R5, #-1088]            @ 0xfffffbc0 
    /*00009280*/ CMP         R3, #0 
    /*00009284*/ BEQ         RgnBatchEnd_x94e8
    /*00009288*/ LDR         R4, [SP, #12] 
    /*0000928c*/ ADD         R1, SP, #36                 @ 0x24 
    /*00009290*/ ADD         R0, R4, #4 
    /*00009294*/ BL          0x0000000000009294          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00009298*/ LDR         R0, [R5, #-1088]            @ 0xfffffbc0 
    /*0000929c*/ LDR         SL, [R0] 
    /*000092a0*/ CMP         SL, R0 
    /*000092a4*/ LDR         R9, [SL] 
    /*000092a8*/ BEQ         RgnBatchEnd_x9424
    /*000092ac*/ LDR         R8, [SL, #-4] 
    /*000092b0*/ SUB         FP, SL, #4 
    /*000092b4*/ ADD         R3, R4, R8, LSL #2 
    /*000092b8*/ LDR         R4, [R3, #8] 
    /*000092bc*/ CMP         R4, #0 
    /*000092c0*/ BEQ         RgnBatchEnd_x9460
    /*000092c4*/ MOVW        R3, #0                      @ .LC190 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x27e8] 
    /*000092c8*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x44] 
    /*000092cc*/ MOVT        R3, #0                      @ .LC190 [R_ARM_MOVT_ABS: .rodata.str1.4+0x27e8] 
    /*000092d0*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x44] 
    /*000092d4*/ STR         R3, [SP, #24] 
    /*000092d8*/ STR         R2, [SP, #28] 
    RgnBatchEnd_x92dc:
    /*000092dc*/ LDR         IP, [R4, #212]              @ 0xd4 
    /*000092e0*/ ADD         LR, R4, #256                @ 0x100 
    /*000092e4*/ LDR         R2, [R4, #240]              @ 0xf0 
    /*000092e8*/ MOV         R6, R4 
    /*000092ec*/ LDRD        R0, [R4, #248]              @ 0xf8 
    /*000092f0*/ UMULL       R2, R3, IP, R2 
    /*000092f4*/ ADDS        R0, R0, R2 
    /*000092f8*/ ADC         R1, R1, R3 
    /*000092fc*/ STRD        R0, [R4, #96]               @ 0x60 
    /*00009300*/ LDRD        R0, [LR] 
    /*00009304*/ ADDS        R2, R2, R0 
    /*00009308*/ ADC         R3, R3, R1 
    /*0000930c*/ STRD        R2, [R4, #104]              @ 0x68 
    /*00009310*/ LDR         R7, [R6, #308]!             @ 0x134 
    /*00009314*/ CMP         R7, R6 
    /*00009318*/ LDR         R5, [R7] 
    /*0000931c*/ BNE         RgnBatchEnd_x9328
    /*00009320*/ B           RgnBatchEnd_x9384
    RgnBatchEnd_x9324:
    /*00009324*/ MOV         R5, R3 
    RgnBatchEnd_x9328:
    /*00009328*/ ADD         R7, R7, #8 
    /*0000932c*/ MOV         R2, R8 
    /*00009330*/ LDR         R0, [R4, #176]              @ 0xb0 
    /*00009334*/ MOV         R1, R7 
    /*00009338*/ BL          0x0000000000009338          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*0000933c*/ MOV         R1, R7 
    /*00009340*/ CMP         R0, #0 
    /*00009344*/ BEQ         RgnBatchEnd_x9370
    /*00009348*/ LDRD        R2, [R4, #96]               @ 0x60 
    /*0000934c*/ STRD        R2, [R0, #88]               @ 0x58 
    /*00009350*/ LDRD        R2, [R4, #104]              @ 0x68 
    /*00009354*/ STRD        R2, [R0, #96]               @ 0x60 
    /*00009358*/ LDR         R0, [R4, #176]              @ 0xb0 
    /*0000935c*/ BL          0x000000000000935C          @ RgnGetChnGetInfo [R_ARM_CALL: .text+0x1034] 
    /*00009360*/ SUBS        R7, R0, #0 
    /*00009364*/ BEQ         RgnBatchEnd_x940c
    RgnBatchEnd_x9368:
    /*00009368*/ MOV         R3, #1 
    /*0000936c*/ STR         R3, [R7, #4] 
    RgnBatchEnd_x9370:
    /*00009370*/ CMP         R6, R5 
    /*00009374*/ LDR         R3, [R5] 
    /*00009378*/ MOV         R7, R5 
    /*0000937c*/ BNE         RgnBatchEnd_x9324
    /*00009380*/ LDR         IP, [R4, #212]              @ 0xd4 
    RgnBatchEnd_x9384:
    /*00009384*/ STR         IP, [R4, #216]              @ 0xd8 
    /*00009388*/ MOV         R3, #0 
    /*0000938c*/ STR         R3, [R4, #232]              @ 0xe8 
    /*00009390*/ MOV         R0, FP 
    /*00009394*/ STR         R3, [R4, #220]              @ 0xdc 
    /*00009398*/ MOV         R2, #256                    @ 0x100 
    /*0000939c*/ STR         R3, [R4, #228]              @ 0xe4 
    /*000093a0*/ MOVT        R2, #16 
    /*000093a4*/ STR         R3, [R4, #208]              @ 0xd0 
    /*000093a8*/ MOV         R3, #512                    @ 0x200 
    /*000093ac*/ LDR         R1, [SL, #4] 
    /*000093b0*/ MOVT        R3, #32 
    /*000093b4*/ LDR         IP, [SL] 
    /*000093b8*/ STR         R1, [IP, #4] 
    /*000093bc*/ STR         IP, [R1] 
    /*000093c0*/ STM         SL, {R2, R3} 
    /*000093c4*/ BL          0x00000000000093C4          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*000093c8*/ LDR         R2, [SP, #20] 
    /*000093cc*/ LDR         R3, [SP, #16] 
    /*000093d0*/ ADD         R3, R3, R2 
    /*000093d4*/ LDR         R2, [R9] 
    /*000093d8*/ LDR         R0, [R3, #-1088]            @ 0xfffffbc0 
    /*000093dc*/ CMP         R0, R9 
    /*000093e0*/ BEQ         RgnBatchEnd_x9424
    /*000093e4*/ LDR         R8, [R9, #-4] 
    /*000093e8*/ SUB         FP, R9, #4 
    /*000093ec*/ LDR         R3, [SP, #12] 
    /*000093f0*/ MOV         SL, R9 
    /*000093f4*/ ADD         R3, R3, R8, LSL #2 
    /*000093f8*/ LDR         R4, [R3, #8] 
    /*000093fc*/ CMP         R4, #0 
    /*00009400*/ BEQ         RgnBatchEnd_x9460
    /*00009404*/ MOV         R9, R2 
    /*00009408*/ B           RgnBatchEnd_x92dc
    RgnBatchEnd_x940c:
    /*0000940c*/ LDR         R3, [SP, #24] 
    /*00009410*/ MOVW        R2, #5800                   @ 0x16a8 
    /*00009414*/ LDR         R1, [PC, #260]              @ 0x0000000000009520 
    /*00009418*/ LDR         R0, [SP, #28] 
    /*0000941c*/ BL          0x000000000000941C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00009420*/ B           RgnBatchEnd_x9368
    RgnBatchEnd_x9424:
    /*00009424*/ BL          0x0000000000009424          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00009428*/ LDR         R2, [SP, #20] 
    /*0000942c*/ LDR         R3, [SP, #16] 
    /*00009430*/ ADD         R1, SP, #36                 @ 0x24 
    /*00009434*/ LDR         R0, [PC, #232]              @ 0x0000000000009524 
    /*00009438*/ MOV         R4, #0 
    /*0000943c*/ ADD         R3, R3, R2 
    /*00009440*/ STR         R4, [R3, #-1088]            @ 0xfffffbc0 
    /*00009444*/ BL          0x0000000000009444          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00009448*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000944c*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00009450*/ BL          0x0000000000009450          @ osal_up [R_ARM_CALL: +0x0] 
    /*00009454*/ MOV         R0, R4 
    RgnBatchEnd_x9458:
    /*00009458*/ ADD         SP, SP, #44                 @ 0x2c 
    /*0000945c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnBatchEnd_x9460:
    /*00009460*/ MOV         R1, #3 
    /*00009464*/ STR         R8, [SP, #4] 
    /*00009468*/ MOV         R0, R1 
    /*0000946c*/ MOVW        R3, #5777                   @ 0x1691 
    /*00009470*/ MOVW        R2, #0                      @ .LC189 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x27b0] 
    /*00009474*/ STR         R3, [SP] 
    /*00009478*/ MOVT        R2, #0                      @ .LC189 [R_ARM_MOVT_ABS: .rodata.str1.4+0x27b0] 
    /*0000947c*/ LDR         R3, [PC, #156]              @ 0x0000000000009520 
    /*00009480*/ BL          0x0000000000009480          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00009484*/ ADD         R1, SP, #36                 @ 0x24 
    /*00009488*/ LDR         R0, [PC, #148]              @ 0x0000000000009524 
    /*0000948c*/ BL          0x000000000000948C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00009490*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00009494*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00009498*/ BL          0x0000000000009498          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000949c*/ MOVW        R0, #32773                  @ 0x8005 
    /*000094a0*/ MOVT        R0, #40963                  @ 0xa003 
    /*000094a4*/ ADD         SP, SP, #44                 @ 0x2c 
    /*000094a8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RgnBatchEnd_x94ac:
    /*000094ac*/ MOV         R1, #3 
    /*000094b0*/ STR         R0, [SP, #4] 
    /*000094b4*/ MOVW        R3, #5750                   @ 0x1676 
    /*000094b8*/ MOV         R0, R1 
    /*000094bc*/ STR         R3, [SP] 
    /*000094c0*/ MOVW        R2, #0                      @ .LC187 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2734] 
    /*000094c4*/ LDR         R3, [PC, #84]               @ 0x0000000000009520 
    /*000094c8*/ MOVT        R2, #0                      @ .LC187 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2734] 
    /*000094cc*/ BL          0x00000000000094CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000094d0*/ MOVW        R0, #32777                  @ 0x8009 
    /*000094d4*/ MOVT        R0, #40963                  @ 0xa003 
    /*000094d8*/ B           RgnBatchEnd_x9458
    RgnBatchEnd_x94dc:
    /*000094dc*/ MOV         R0, #65024                  @ 0xfe00 
    /*000094e0*/ MOVT        R0, #65535                  @ 0xffff 
    /*000094e4*/ B           RgnBatchEnd_x9458
    RgnBatchEnd_x94e8:
    /*000094e8*/ MOV         R1, #3 
    /*000094ec*/ MOVW        R3, #5762                   @ 0x1682 
    /*000094f0*/ MOV         R0, R1 
    /*000094f4*/ STR         R3, [SP] 
    /*000094f8*/ STR         R4, [SP, #4] 
    /*000094fc*/ MOVW        R2, #0                      @ .LC188 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2770] 
    /*00009500*/ LDR         R3, [PC, #24]               @ 0x0000000000009520 
    /*00009504*/ MOVT        R2, #0                      @ .LC188 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2770] 
    /*00009508*/ BL          0x0000000000009508          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000950c*/ LDR         R0, [SP, #12] 
    /*00009510*/ BL          0x0000000000009510          @ osal_up [R_ARM_CALL: +0x0] 
    /*00009514*/ MOVW        R0, #32777                  @ 0x8009 
    /*00009518*/ MOVT        R0, #40963                  @ 0xa003 
    /*0000951c*/ B           RgnBatchEnd_x9458
    /*00009520*/ ANDEQ       R0, R0, R0, ASR #10 
    /*00009524*/ ANDEQ       R0, R0, R4 
FUNC_END RgnBatchEnd


FUNC_BEGIN RGN_Proc
    /*00009528*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000952c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00009530*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00009534*/ SUB         SP, SP, #204                @ 0xcc 
    /*00009538*/ MOV         R4, R3 
    /*0000953c*/ STR         R0, [SP, #84]               @ 0x54 
    /*00009540*/ MOV         R0, R4 
    /*00009544*/ STR         R3, [SP, #128]              @ 0x80 
    /*00009548*/ MOV         R3, #0 
    /*0000954c*/ STR         R3, [SP, #180]              @ 0xb4 
    /*00009550*/ STR         R3, [SP, #184]              @ 0xb8 
    /*00009554*/ STR         R3, [SP, #188]              @ 0xbc 
    /*00009558*/ STR         R3, [SP, #192]              @ 0xc0 
    /*0000955c*/ STR         R3, [SP, #196]              @ 0xc4 
    /*00009560*/ BL          0x0000000000009560          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00009564*/ CMP         R0, #0 
    /*00009568*/ BNE         RGN_Proc_x9604
    /*0000956c*/ ADD         R3, R4, #4 
    /*00009570*/ ADD         R2, R4, #516                @ 0x204 
    /*00009574*/ MOV         R1, #1 
    /*00009578*/ STR         R3, [SP, #168]              @ 0xa8 
    /*0000957c*/ MOV         R0, R3 
    RGN_Proc_x9580:
    /*00009580*/ LDR         R3, [R0, #4]! 
    /*00009584*/ CMP         R3, #0 
    /*00009588*/ BEQ         RGN_Proc_x95a0
    /*0000958c*/ LDR         R3, [R3, #176]              @ 0xb0 
    /*00009590*/ ADD         IP, SP, #200                @ 0xc8 
    /*00009594*/ CMP         R3, #4 
    /*00009598*/ ADD         R3, IP, R3, LSL #2 
    /*0000959c*/ STRLS       R1, [R3, #-20]              @ 0xffffffec 
    RGN_Proc_x95a0:
    /*000095a0*/ CMP         R0, R2 
    /*000095a4*/ BNE         RGN_Proc_x9580
    /*000095a8*/ LDR         R3, [SP, #180]              @ 0xb4 
    /*000095ac*/ STR         R0, [SP, #136]              @ 0x88 
    /*000095b0*/ CMP         R3, #0 
    /*000095b4*/ BNE         RGN_Proc_xa0d8
    /*000095b8*/ LDR         R3, [SP, #184]              @ 0xb8 
    /*000095bc*/ CMP         R3, #0 
    /*000095c0*/ BNE         RGN_Proc_xa64c
    RGN_Proc_x95c4:
    /*000095c4*/ LDR         R3, [SP, #188]              @ 0xbc 
    /*000095c8*/ CMP         R3, #0 
    /*000095cc*/ BNE         RGN_Proc_xad88
    RGN_Proc_x95d0:
    /*000095d0*/ LDR         R3, [SP, #192]              @ 0xc0 
    /*000095d4*/ CMP         R3, #0 
    /*000095d8*/ BNE         RGN_Proc_x9ae4
    RGN_Proc_x95dc:
    /*000095dc*/ LDR         R3, [SP, #196]              @ 0xc4 
    /*000095e0*/ CMP         R3, #0 
    /*000095e4*/ BNE         RGN_Proc_x960c
    RGN_Proc_x95e8:
    /*000095e8*/ LDR         R0, [SP, #84]               @ 0x54 
    /*000095ec*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*000095f0*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*000095f4*/ BL          0x00000000000095F4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000095f8*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000095fc*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00009600*/ BL          0x0000000000009600          @ osal_up [R_ARM_CALL: +0x0] 
    RGN_Proc_x9604:
    /*00009604*/ ADD         SP, SP, #204                @ 0xcc 
    /*00009608*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    RGN_Proc_x960c:
    /*0000960c*/ MOV         R2, #64                     @ 0x40 
    /*00009610*/ MOV         R1, #0 
    /*00009614*/ LDR         R0, [PC, #3588]             @ 0x000000000000a420 
    /*00009618*/ MOV         R7, #0 
    /*0000961c*/ BL          0x000000000000961C          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00009620*/ LDR         R4, [SP, #84]               @ 0x54 
    /*00009624*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*00009628*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*0000962c*/ LDR         FP, [PC, #3572]             @ 0x000000000000a428 
    /*00009630*/ MOV         R0, R4 
    /*00009634*/ BL          0x0000000000009634          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009638*/ MOV         R0, R4 
    /*0000963c*/ MOVW        R1, #0                      @ .LC289 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x31f8] 
    /*00009640*/ MOVT        R1, #0                      @ .LC289 [R_ARM_MOVT_ABS: .rodata.str1.4+0x31f8] 
    /*00009644*/ BL          0x0000000000009644          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009648*/ MOVW        IP, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*0000964c*/ MOV         R0, R4 
    /*00009650*/ MOVW        R2, #0                      @ .LC216 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2914] 
    /*00009654*/ MOV         R4, IP 
    /*00009658*/ MOVT        R2, #0                      @ .LC216 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2914] 
    /*0000965c*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*00009660*/ STR         R2, [SP] 
    /*00009664*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*00009668*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*0000966c*/ MOVW        R1, #0                      @ .LC253 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c38] 
    /*00009670*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*00009674*/ MOVT        R1, #0                      @ .LC253 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c38] 
    /*00009678*/ BL          0x0000000000009678          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000967c*/ MOV         IP, R4 
    /*00009680*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*00009684*/ MOVT        IP, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*00009688*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*0000968c*/ STR         IP, [SP, #96]               @ 0x60 
    /*00009690*/ STR         R3, [SP, #92]               @ 0x5c 
    /*00009694*/ B           RGN_Proc_x96a8
    RGN_Proc_x9698:
    /*00009698*/ LDR         R3, [SP, #136]              @ 0x88 
    /*0000969c*/ ADD         R7, R7, #1 
    /*000096a0*/ CMP         FP, R3 
    /*000096a4*/ BEQ         RGN_Proc_x979c
    RGN_Proc_x96a8:
    /*000096a8*/ LDR         R6, [FP, #4]! 
    /*000096ac*/ CMP         R6, #0 
    /*000096b0*/ BEQ         RGN_Proc_x9698
    /*000096b4*/ LDR         R3, [R6, #176]              @ 0xb0 
    /*000096b8*/ CMP         R3, #4 
    /*000096bc*/ BNE         RGN_Proc_x9698
    /*000096c0*/ LDR         R1, [R6, #180]              @ 0xb4 
    /*000096c4*/ MOVW        R0, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*000096c8*/ MOVT        R0, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*000096cc*/ MOVW        R2, #0                      @ .LC192 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2804] 
    /*000096d0*/ CMP         R1, #0 
    /*000096d4*/ MOVT        R2, #0                      @ .LC192 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2804] 
    /*000096d8*/ MOVW        R1, #0                      @ .LC254 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c44] 
    /*000096dc*/ MOVT        R1, #0                      @ .LC254 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c44] 
    /*000096e0*/ MOVNE       R2, R0 
    /*000096e4*/ LDR         R0, [SP, #84]               @ 0x54 
    /*000096e8*/ STR         R2, [SP] 
    /*000096ec*/ MOV         R2, R7 
    /*000096f0*/ BL          0x00000000000096F0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000096f4*/ LDR         R5, [R6, #308]              @ 0x134 
    /*000096f8*/ ADD         SL, R6, #308                @ 0x134 
    /*000096fc*/ CMP         SL, R5 
    /*00009700*/ BEQ         RGN_Proc_x9698
    /*00009704*/ LDR         R4, [R5] 
    /*00009708*/ ADD         R9, SP, #176                @ 0xb0 
    /*0000970c*/ STR         FP, [SP, #88]               @ 0x58 
    /*00009710*/ LDR         R8, [PC, #3344]             @ 0x000000000000a428 
    /*00009714*/ MOV         FP, R4 
    /*00009718*/ LDR         R4, [SP, #92]               @ 0x5c 
    /*0000971c*/ B           RGN_Proc_x9724
    RGN_Proc_x9720:
    /*00009720*/ MOV         FP, R2 
    RGN_Proc_x9724:
    /*00009724*/ MOV         R1, R9 
    /*00009728*/ MOV         R0, R8 
    /*0000972c*/ BL          0x000000000000972C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00009730*/ MOV         R2, R7 
    /*00009734*/ ADD         R1, R5, #8 
    /*00009738*/ LDR         R0, [R6, #176]              @ 0xb0 
    /*0000973c*/ BL          0x000000000000973C          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*00009740*/ CMP         R0, #0 
    /*00009744*/ BEQ         RGN_Proc_x976c
    /*00009748*/ LDR         R2, [R5, #12] 
    /*0000974c*/ CMP         R2, #15 
    /*00009750*/ ADD         R2, R4, R2, LSL #2 
    /*00009754*/ BHI         RGN_Proc_x976c
    /*00009758*/ LDR         R1, [R0, #64]               @ 0x40 
    /*0000975c*/ LDR         R0, [R2, #-1056]            @ 0xfffffbe0 
    /*00009760*/ CMP         R1, R0 
    /*00009764*/ MOVCC       R1, R0 
    /*00009768*/ STR         R1, [R2, #-1056]            @ 0xfffffbe0 
    RGN_Proc_x976c:
    /*0000976c*/ MOV         R1, R9 
    /*00009770*/ MOV         R0, R8 
    /*00009774*/ BL          0x0000000000009774          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00009778*/ CMP         SL, FP 
    /*0000977c*/ LDR         R2, [FP] 
    /*00009780*/ MOV         R5, FP 
    /*00009784*/ BNE         RGN_Proc_x9720
    /*00009788*/ LDR         FP, [SP, #88]               @ 0x58 
    /*0000978c*/ ADD         R7, R7, #1 
    /*00009790*/ LDR         R3, [SP, #136]              @ 0x88 
    /*00009794*/ CMP         FP, R3 
    /*00009798*/ BNE         RGN_Proc_x96a8
    RGN_Proc_x979c:
    /*0000979c*/ LDR         R4, [SP, #84]               @ 0x54 
    /*000097a0*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*000097a4*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*000097a8*/ MOV         R9, #0 
    /*000097ac*/ MOV         R8, R9 
    /*000097b0*/ MOV         R0, R4 
    /*000097b4*/ BL          0x00000000000097B4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000097b8*/ MOV         R0, R4 
    /*000097bc*/ MOVW        R1, #0                      @ .LC290 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3254] 
    /*000097c0*/ MOVT        R1, #0                      @ .LC290 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3254] 
    /*000097c4*/ BL          0x00000000000097C4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000097c8*/ LDR         R3, [SP, #96]               @ 0x60 
    /*000097cc*/ MOVW        IP, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .bss+0x1ff8] 
    /*000097d0*/ MOV         R0, R4 
    /*000097d4*/ MOVW        R2, #0                      @ .LC292 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x32dc] 
    /*000097d8*/ MOV         R4, IP 
    /*000097dc*/ STR         R3, [SP, #20] 
    /*000097e0*/ MOVT        R2, #0                      @ .LC292 [R_ARM_MOVT_ABS: .rodata.str1.4+0x32dc] 
    /*000097e4*/ MOVW        R3, #0                      @ .LC241 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b68] 
    /*000097e8*/ STR         R2, [SP, #36]               @ 0x24 
    /*000097ec*/ MOVT        R3, #0                      @ .LC241 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b68] 
    /*000097f0*/ MOVW        R2, #0                      @ .LC213 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2904] 
    /*000097f4*/ STR         R3, [SP, #40]               @ 0x28 
    /*000097f8*/ MOVT        R2, #0                      @ .LC213 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2904] 
    /*000097fc*/ MOVW        R3, #0                      @ .LC259 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2cec] 
    /*00009800*/ STR         R2, [SP, #28] 
    /*00009804*/ MOVT        R3, #0                      @ .LC259 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2cec] 
    /*00009808*/ MOVW        R2, #0                      @ .LC244 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b80] 
    /*0000980c*/ STR         R3, [SP, #32] 
    /*00009810*/ MOVT        R2, #0                      @ .LC244 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b80] 
    /*00009814*/ MOVW        R3, #0                      @ .LC214 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2908] 
    /*00009818*/ STR         R2, [SP, #16] 
    /*0000981c*/ MOVT        R3, #0                      @ .LC214 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2908] 
    /*00009820*/ MOVW        R2, #0                      @ .LC248 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b9c] 
    /*00009824*/ STR         R3, [SP, #24] 
    /*00009828*/ MOVT        R2, #0                      @ .LC248 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b9c] 
    /*0000982c*/ MOVW        R3, #0                      @ .LC260 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2cf4] 
    /*00009830*/ STR         R2, [SP, #8] 
    /*00009834*/ MOVT        R3, #0                      @ .LC260 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2cf4] 
    /*00009838*/ MOVW        R2, #0                      @ .LC250 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ba4] 
    /*0000983c*/ STR         R3, [SP, #12] 
    /*00009840*/ MOVT        R2, #0                      @ .LC250 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ba4] 
    /*00009844*/ MOVW        R3, #0                      @ .LC249 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ba0] 
    /*00009848*/ STR         R2, [SP] 
    /*0000984c*/ MOVT        R3, #0                      @ .LC249 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ba0] 
    /*00009850*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*00009854*/ STR         R3, [SP, #4] 
    /*00009858*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*0000985c*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*00009860*/ MOVW        R1, #0                      @ .LC291 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x32b0] 
    /*00009864*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*00009868*/ MOVT        R1, #0                      @ .LC291 [R_ARM_MOVT_ABS: .rodata.str1.4+0x32b0] 
    /*0000986c*/ BL          0x000000000000986C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009870*/ MOV         IP, R4 
    /*00009874*/ MOVW        R3, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*00009878*/ MOVT        IP, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .bss+0x1ff8] 
    /*0000987c*/ MOVW        R2, #0                      @ .LC192 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2804] 
    /*00009880*/ MOVT        R3, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*00009884*/ MOVT        R2, #0                      @ .LC192 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2804] 
    /*00009888*/ STR         IP, [SP, #116]              @ 0x74 
    /*0000988c*/ STR         R3, [SP, #120]              @ 0x78 
    /*00009890*/ STR         R2, [SP, #124]              @ 0x7c 
    /*00009894*/ STR         FP, [SP, #108]              @ 0x6c 
    /*00009898*/ LDR         R4, [SP, #168]              @ 0xa8 
    /*0000989c*/ B           RGN_Proc_x98b0
    RGN_Proc_x98a0:
    /*000098a0*/ LDR         R3, [SP, #108]              @ 0x6c 
    /*000098a4*/ ADD         R8, R8, #1 
    /*000098a8*/ CMP         R4, R3 
    /*000098ac*/ BEQ         RGN_Proc_x95e8
    RGN_Proc_x98b0:
    /*000098b0*/ LDR         FP, [R4, #4]! 
    /*000098b4*/ CMP         FP, #0 
    /*000098b8*/ BEQ         RGN_Proc_x98a0
    /*000098bc*/ LDR         R3, [FP, #176]              @ 0xb0 
    /*000098c0*/ CMP         R3, #4 
    /*000098c4*/ BNE         RGN_Proc_x98a0
    /*000098c8*/ LDR         R6, [FP, #308]              @ 0x134 
    /*000098cc*/ ADD         R5, FP, #308                @ 0x134 
    /*000098d0*/ CMP         R5, R6 
    /*000098d4*/ BEQ         RGN_Proc_x98a0
    /*000098d8*/ LDR         R9, [R6] 
    /*000098dc*/ STR         R4, [SP, #128]              @ 0x80 
    /*000098e0*/ B           RGN_Proc_x98f8
    RGN_Proc_x98e4:
    /*000098e4*/ CMP         R5, R9 
    /*000098e8*/ LDR         R3, [R9] 
    /*000098ec*/ MOV         R6, R9 
    /*000098f0*/ BEQ         RGN_Proc_x9a7c
    RGN_Proc_x98f4:
    /*000098f4*/ MOV         R9, R3 
    RGN_Proc_x98f8:
    /*000098f8*/ LDR         R3, [R6, #12] 
    /*000098fc*/ CMP         R3, #15 
    /*00009900*/ BHI         RGN_Proc_x98e4
    /*00009904*/ LDR         R2, [SP, #116]              @ 0x74 
    /*00009908*/ ADD         SL, R6, #8 
    /*0000990c*/ LDR         R0, [FP, #176]              @ 0xb0 
    /*00009910*/ ADD         R7, SP, #176                @ 0xb0 
    /*00009914*/ ADD         R3, R2, R3, LSL #2 
    /*00009918*/ MOV         R1, SL 
    /*0000991c*/ MOV         R2, R8 
    /*00009920*/ LDR         R3, [R3, #-1056]            @ 0xfffffbe0 
    /*00009924*/ BL          0x0000000000009924          @ RgnCheckLastInChnRgnAttrByLayer [R_ARM_CALL: .text+0x12ac] 
    /*00009928*/ MOV         R1, R7 
    /*0000992c*/ STR         R0, [SP, #112]              @ 0x70 
    /*00009930*/ LDR         R0, [PC, #2800]             @ 0x000000000000a428 
    /*00009934*/ BL          0x0000000000009934          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00009938*/ MOV         R2, R8 
    /*0000993c*/ MOV         R1, SL 
    /*00009940*/ LDR         R0, [FP, #176]              @ 0xb0 
    /*00009944*/ BL          0x0000000000009944          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*00009948*/ SUBS        R3, R0, #0 
    /*0000994c*/ STR         R3, [SP, #88]               @ 0x58 
    /*00009950*/ BEQ         RGN_Proc_x9a60
    /*00009954*/ LDR         R0, [R6, #8] 
    /*00009958*/ BL          0x0000000000000000 
    /*0000995c*/ LDR         LR, [SP, #88]               @ 0x58 
    /*00009960*/ LDR         R2, [SP, #120]              @ 0x78 
    /*00009964*/ LDR         IP, [SP, #124]              @ 0x7c 
    /*00009968*/ LDR         R1, [LR, #116]              @ 0x74 
    /*0000996c*/ LDR         R3, [FP, #176]              @ 0xb0 
    /*00009970*/ SUB         R1, R1, #1 
    /*00009974*/ MOV         R4, R0 
    /*00009978*/ LDR         R0, [LR] 
    /*0000997c*/ CMP         R0, #0 
    /*00009980*/ MOV         R0, LR 
    /*00009984*/ MOVEQ       R2, IP 
    /*00009988*/ STR         R2, [SP, #104]              @ 0x68 
    /*0000998c*/ LDR         R2, [R6, #12] 
    /*00009990*/ CMP         R1, #3 
    /*00009994*/ LDR         IP, [LR, #12] 
    /*00009998*/ STR         R2, [SP, #92]               @ 0x5c 
    /*0000999c*/ LDR         R2, [R6, #16] 
    /*000099a0*/ LDR         R6, [R0, #20] 
    /*000099a4*/ MOVWLS      R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000099a8*/ MOVTLS      R0, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*000099ac*/ MOVWHI      R0, #0                      @ .LC193 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2808] 
    /*000099b0*/ STR         R2, [SP, #96]               @ 0x60 
    /*000099b4*/ ADDLS       R1, R0, R1, LSL #2 
    /*000099b8*/ LDR         R2, [LR, #8] 
    /*000099bc*/ MOVTHI      R0, #0                      @ .LC193 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2808] 
    /*000099c0*/ LDRLS       R0, [R1, #1368]             @ 0x558 
    /*000099c4*/ LDR         LR, [LR, #16] 
    /*000099c8*/ STR         R2, [SP, #100]              @ 0x64 
    /*000099cc*/ LDR         R2, [SP, #88]               @ 0x58 
    /*000099d0*/ LDR         R1, [R2, #156]              @ 0x9c 
    /*000099d4*/ CMP         R1, #3 
    /*000099d8*/ LDRLS       PC, [PC, R1, LSL #2] 
    /*000099dc*/ B           RGN_Proc_xb618
    /*000099e0*/ ANDEQ       R9, R0, R4, LSR #21 
    /*000099e4*/ ANDEQ       R9, R0, R4, LSL #21 
    /*000099e8*/ ANDEQ       R9, R0, R4, ASR #21 
    /*000099ec*/ STRDEQ      R9, [R0], -R0 
    /*000099f0*/ LDR         R2, [SP, #112]              @ 0x70 
    /*000099f4*/ MOVW        SL, #0                      @ .LC202 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2848] 
    /*000099f8*/ MOVT        SL, #0                      @ .LC202 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2848] 
    /*000099fc*/ MOVW        R1, #0                      @ .LC203 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2854] 
    /*00009a00*/ CMP         R2, #0 
    /*00009a04*/ MOVT        R1, #0                      @ .LC203 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2854] 
    /*00009a08*/ MOVNE       R1, SL 
    RGN_Proc_x9a0c:
    /*00009a0c*/ LDR         R2, [SP, #88]               @ 0x58 
    /*00009a10*/ LDR         R2, [R2, #64]               @ 0x40 
    /*00009a14*/ STR         IP, [SP, #20] 
    /*00009a18*/ LDR         IP, [SP, #100]              @ 0x64 
    /*00009a1c*/ STR         R1, [SP, #36]               @ 0x24 
    /*00009a20*/ MOVW        R1, #0                      @ .LC293 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x32e4] 
    /*00009a24*/ STR         R0, [SP, #32] 
    /*00009a28*/ MOVT        R1, #0                      @ .LC293 [R_ARM_MOVT_ABS: .rodata.str1.4+0x32e4] 
    /*00009a2c*/ STR         IP, [SP, #16] 
    /*00009a30*/ LDR         IP, [SP, #104]              @ 0x68 
    /*00009a34*/ STR         R2, [SP, #40]               @ 0x28 
    /*00009a38*/ MOV         R2, R8 
    /*00009a3c*/ STR         R6, [SP, #28] 
    /*00009a40*/ STR         IP, [SP, #12] 
    /*00009a44*/ LDR         IP, [SP, #96]               @ 0x60 
    /*00009a48*/ LDR         R0, [SP, #84]               @ 0x54 
    /*00009a4c*/ STR         LR, [SP, #24] 
    /*00009a50*/ STR         IP, [SP, #8] 
    /*00009a54*/ LDR         IP, [SP, #92]               @ 0x5c 
    /*00009a58*/ STM         SP, {R4, IP} 
    /*00009a5c*/ BL          0x0000000000009A5C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    RGN_Proc_x9a60:
    /*00009a60*/ MOV         R1, R7 
    /*00009a64*/ LDR         R0, [PC, #2492]             @ 0x000000000000a428 
    /*00009a68*/ BL          0x0000000000009A68          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00009a6c*/ CMP         R5, R9 
    /*00009a70*/ LDR         R3, [R9] 
    /*00009a74*/ MOV         R6, R9 
    /*00009a78*/ BNE         RGN_Proc_x98f4
    RGN_Proc_x9a7c:
    /*00009a7c*/ LDR         R4, [SP, #128]              @ 0x80 
    /*00009a80*/ B           RGN_Proc_x98a0
    /*00009a84*/ LDR         R2, [SP, #112]              @ 0x70 
    /*00009a88*/ MOVW        SL, #0                      @ .LC198 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2828] 
    /*00009a8c*/ MOVT        SL, #0                      @ .LC198 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2828] 
    /*00009a90*/ MOVW        R1, #0                      @ .LC199 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2834] 
    /*00009a94*/ CMP         R2, #0 
    /*00009a98*/ MOVT        R1, #0                      @ .LC199 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2834] 
    /*00009a9c*/ MOVNE       R1, SL 
    /*00009aa0*/ B           RGN_Proc_x9a0c
    /*00009aa4*/ LDR         R2, [SP, #112]              @ 0x70 
    /*00009aa8*/ MOVW        SL, #0                      @ .LC196 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2818] 
    /*00009aac*/ MOVT        SL, #0                      @ .LC196 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2818] 
    /*00009ab0*/ MOVW        R1, #0                      @ .LC197 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2824] 
    /*00009ab4*/ CMP         R2, #0 
    /*00009ab8*/ MOVT        R1, #0                      @ .LC197 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2824] 
    /*00009abc*/ MOVNE       R1, SL 
    /*00009ac0*/ B           RGN_Proc_x9a0c
    /*00009ac4*/ LDR         R2, [SP, #112]              @ 0x70 
    /*00009ac8*/ MOVW        SL, #0                      @ .LC200 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2838] 
    /*00009acc*/ MOVT        SL, #0                      @ .LC200 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2838] 
    /*00009ad0*/ MOVW        R1, #0                      @ .LC201 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2844] 
    /*00009ad4*/ CMP         R2, #0 
    /*00009ad8*/ MOVT        R1, #0                      @ .LC201 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2844] 
    /*00009adc*/ MOVNE       R1, SL 
    /*00009ae0*/ B           RGN_Proc_x9a0c
    RGN_Proc_x9ae4:
    /*00009ae4*/ LDR         R9, [SP, #84]               @ 0x54 
    /*00009ae8*/ MOVW        R3, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*00009aec*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*00009af0*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*00009af4*/ MOV         R4, R3 
    /*00009af8*/ MOVW        R6, #0                      @ .LC192 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2804] 
    /*00009afc*/ MOV         R0, R9 
    /*00009b00*/ MOVW        R5, #0                      @ .LC217 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x291c] 
    /*00009b04*/ BL          0x0000000000009B04          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009b08*/ MOVW        R1, #0                      @ .LC283 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3030] 
    /*00009b0c*/ MOV         R0, R9 
    /*00009b10*/ MOVT        R1, #0                      @ .LC283 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3030] 
    /*00009b14*/ BL          0x0000000000009B14          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009b18*/ MOVW        R2, #0                      @ .LC209 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28e8] 
    /*00009b1c*/ MOVW        R3, #0                      @ .LC210 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28f0] 
    /*00009b20*/ MOVT        R2, #0                      @ .LC209 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28e8] 
    /*00009b24*/ MOVT        R3, #0                      @ .LC210 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28f0] 
    /*00009b28*/ STR         R2, [SP, #28] 
    /*00009b2c*/ STR         R3, [SP, #24] 
    /*00009b30*/ MOVW        R2, #0                      @ .LC211 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28f8] 
    /*00009b34*/ MOVW        R3, #0                      @ .LC212 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28fc] 
    /*00009b38*/ MOVT        R2, #0                      @ .LC211 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28f8] 
    /*00009b3c*/ MOVT        R3, #0                      @ .LC212 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28fc] 
    /*00009b40*/ STR         R2, [SP, #20] 
    /*00009b44*/ STR         R3, [SP, #16] 
    /*00009b48*/ MOVW        R2, #0                      @ .LC213 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2904] 
    /*00009b4c*/ MOVW        R3, #0                      @ .LC214 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2908] 
    /*00009b50*/ MOVT        R2, #0                      @ .LC213 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2904] 
    /*00009b54*/ MOVT        R3, #0                      @ .LC214 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2908] 
    /*00009b58*/ STR         R2, [SP, #12] 
    /*00009b5c*/ STR         R3, [SP, #8] 
    /*00009b60*/ MOVW        R2, #0                      @ .LC215 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x290c] 
    /*00009b64*/ MOVW        R3, #0                      @ .LC216 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2914] 
    /*00009b68*/ MOVT        R2, #0                      @ .LC215 [R_ARM_MOVT_ABS: .rodata.str1.4+0x290c] 
    /*00009b6c*/ MOVT        R3, #0                      @ .LC216 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2914] 
    /*00009b70*/ STR         R2, [SP, #4] 
    /*00009b74*/ STR         R3, [SP] 
    /*00009b78*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*00009b7c*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*00009b80*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*00009b84*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*00009b88*/ MOVW        R1, #0                      @ .LC208 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c4] 
    /*00009b8c*/ MOV         R0, R9 
    /*00009b90*/ MOVT        R1, #0                      @ .LC208 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c4] 
    /*00009b94*/ BL          0x0000000000009B94          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009b98*/ MOVT        R6, #0                      @ .LC192 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2804] 
    /*00009b9c*/ MOV         R3, R4 
    /*00009ba0*/ MOVT        R5, #0                      @ .LC217 [R_ARM_MOVT_ABS: .rodata.str1.4+0x291c] 
    /*00009ba4*/ MOVT        R3, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*00009ba8*/ LDR         FP, [PC, #2168]             @ 0x000000000000a428 
    /*00009bac*/ STR         R3, [SP, #96]               @ 0x60 
    /*00009bb0*/ MOV         R4, #0 
    /*00009bb4*/ STR         R3, [SP, #88]               @ 0x58 
    /*00009bb8*/ LDR         R8, [SP, #136]              @ 0x88 
    /*00009bbc*/ STR         R6, [SP, #92]               @ 0x5c 
    /*00009bc0*/ B           RGN_Proc_x9bd0
    RGN_Proc_x9bc4:
    /*00009bc4*/ CMP         R8, FP 
    /*00009bc8*/ ADD         R4, R4, #1 
    /*00009bcc*/ BEQ         RGN_Proc_x9c58
    RGN_Proc_x9bd0:
    /*00009bd0*/ LDR         IP, [FP, #4]! 
    /*00009bd4*/ CMP         IP, #0 
    /*00009bd8*/ BEQ         RGN_Proc_x9bc4
    /*00009bdc*/ LDR         R3, [IP, #176]              @ 0xb0 
    /*00009be0*/ CMP         R3, #3 
    /*00009be4*/ BNE         RGN_Proc_x9bc4
    /*00009be8*/ LDR         R1, [IP, #112]              @ 0x70 
    /*00009bec*/ MOV         R2, R4 
    /*00009bf0*/ LDR         LR, [IP, #180]              @ 0xb4 
    /*00009bf4*/ ADD         R4, R4, #1 
    /*00009bf8*/ LDR         R0, [SP, #92]               @ 0x5c 
    /*00009bfc*/ STR         R1, [SP, #40]               @ 0x28 
    /*00009c00*/ CMP         LR, #0 
    /*00009c04*/ LDRD        R6, [IP, #104]              @ 0x68 
    /*00009c08*/ LDR         R1, [SP, #88]               @ 0x58 
    /*00009c0c*/ MOVEQ       R1, R0 
    /*00009c10*/ MOV         R0, R9 
    /*00009c14*/ STRD        R6, [SP, #32] 
    /*00009c18*/ LDRD        R6, [IP, #96]               @ 0x60 
    /*00009c1c*/ MOV         LR, R1 
    /*00009c20*/ MOV         R1, R5 
    /*00009c24*/ STRD        R6, [SP, #24] 
    /*00009c28*/ LDR         SL, [IP, #76]               @ 0x4c 
    /*00009c2c*/ STR         SL, [SP, #16] 
    /*00009c30*/ LDR         SL, [IP, #28] 
    /*00009c34*/ STR         SL, [SP, #12] 
    /*00009c38*/ LDR         SL, [IP, #24] 
    /*00009c3c*/ STR         SL, [SP, #8] 
    /*00009c40*/ LDR         IP, [IP, #120]              @ 0x78 
    /*00009c44*/ STR         LR, [SP] 
    /*00009c48*/ STR         IP, [SP, #4] 
    /*00009c4c*/ BL          0x0000000000009C4C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009c50*/ CMP         R8, FP 
    /*00009c54*/ BNE         RGN_Proc_x9bd0
    RGN_Proc_x9c58:
    /*00009c58*/ LDR         R4, [SP, #84]               @ 0x54 
    /*00009c5c*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*00009c60*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*00009c64*/ MOVW        R5, #0                      @ .LC221 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29b4] 
    /*00009c68*/ MOVT        R5, #0                      @ .LC221 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29b4] 
    /*00009c6c*/ MOV         R0, R4 
    /*00009c70*/ BL          0x0000000000009C70          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009c74*/ MOV         R0, R4 
    /*00009c78*/ MOVW        R1, #0                      @ .LC284 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x308c] 
    /*00009c7c*/ MOVT        R1, #0                      @ .LC284 [R_ARM_MOVT_ABS: .rodata.str1.4+0x308c] 
    /*00009c80*/ BL          0x0000000000009C80          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009c84*/ MOV         R0, R4 
    /*00009c88*/ MOVW        R2, #0                      @ .LC220 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29ac] 
    /*00009c8c*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*00009c90*/ MOVT        R2, #0                      @ .LC220 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29ac] 
    /*00009c94*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*00009c98*/ STR         R2, [SP] 
    /*00009c9c*/ MOVW        R1, #0                      @ .LC219 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29a0] 
    /*00009ca0*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*00009ca4*/ MOVT        R1, #0                      @ .LC219 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29a0] 
    /*00009ca8*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*00009cac*/ MOV         R4, #0 
    /*00009cb0*/ BL          0x0000000000009CB0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    RGN_Proc_x9cb4:
    /*00009cb4*/ MOV         R2, R4 
    /*00009cb8*/ MOV         R1, R5 
    /*00009cbc*/ ADD         R4, R4, #1 
    /*00009cc0*/ LDR         R0, [SP, #84]               @ 0x54 
    /*00009cc4*/ BL          0x0000000000009CC4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009cc8*/ CMP         R4, #5 
    /*00009ccc*/ BNE         RGN_Proc_x9cb4
    /*00009cd0*/ LDR         R9, [SP, #84]               @ 0x54 
    /*00009cd4*/ MOV         R2, R4 
    /*00009cd8*/ MOVW        R1, #0                      @ .LC222 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c0] 
    /*00009cdc*/ MOVT        R1, #0                      @ .LC222 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c0] 
    /*00009ce0*/ MOVW        SL, #0                      @ .LC223 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29cc] 
    /*00009ce4*/ MOVW        R8, #0                      @ .LC224 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29d8] 
    /*00009ce8*/ MOV         R0, R9 
    /*00009cec*/ MOVT        SL, #0                      @ .LC223 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29cc] 
    /*00009cf0*/ BL          0x0000000000009CF0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009cf4*/ MOVT        R8, #0                      @ .LC224 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29d8] 
    /*00009cf8*/ MOVW        R3, #0                      @ .LC225 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29dc] 
    /*00009cfc*/ LDR         R5, [PC, #1828]             @ 0x000000000000a428 
    /*00009d00*/ MOVT        R3, #0                      @ .LC225 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29dc] 
    /*00009d04*/ MOV         R6, #0 
    /*00009d08*/ STR         R3, [SP, #88]               @ 0x58 
    /*00009d0c*/ STR         SL, [SP, #92]               @ 0x5c 
    /*00009d10*/ B           RGN_Proc_x9d20
    RGN_Proc_x9d14:
    /*00009d14*/ CMP         R5, FP 
    /*00009d18*/ ADD         R6, R6, #1 
    /*00009d1c*/ BEQ         RGN_Proc_x9d8c
    RGN_Proc_x9d20:
    /*00009d20*/ LDR         R4, [R5, #4]! 
    /*00009d24*/ CMP         R4, #0 
    /*00009d28*/ BEQ         RGN_Proc_x9d14
    /*00009d2c*/ LDR         R3, [R4, #176]              @ 0xb0 
    /*00009d30*/ CMP         R3, #3 
    /*00009d34*/ BNE         RGN_Proc_x9d14
    /*00009d38*/ LDR         IP, [R4, #116]              @ 0x74 
    /*00009d3c*/ MOV         R2, R6 
    /*00009d40*/ LDR         R1, [SP, #92]               @ 0x5c 
    /*00009d44*/ MOV         R0, R9 
    /*00009d48*/ ADD         R7, R4, #184                @ 0xb8 
    /*00009d4c*/ ADD         SL, R4, #204                @ 0xcc 
    /*00009d50*/ STR         IP, [SP] 
    /*00009d54*/ BL          0x0000000000009D54          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    RGN_Proc_x9d58:
    /*00009d58*/ LDR         R2, [R7], #4 
    /*00009d5c*/ MOV         R1, R8 
    /*00009d60*/ MOV         R0, R9 
    /*00009d64*/ BL          0x0000000000009D64          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009d68*/ CMP         R7, SL 
    /*00009d6c*/ BNE         RGN_Proc_x9d58
    /*00009d70*/ LDR         R2, [R4, #204]              @ 0xcc 
    /*00009d74*/ MOV         R0, R9 
    /*00009d78*/ LDR         R1, [SP, #88]               @ 0x58 
    /*00009d7c*/ ADD         R6, R6, #1 
    /*00009d80*/ BL          0x0000000000009D80          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009d84*/ CMP         R5, FP 
    /*00009d88*/ BNE         RGN_Proc_x9d20
    RGN_Proc_x9d8c:
    /*00009d8c*/ LDR         R7, [SP, #84]               @ 0x54 
    /*00009d90*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*00009d94*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*00009d98*/ MOVW        R6, #0                      @ .LC235 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2aa0] 
    /*00009d9c*/ LDR         R4, [PC, #1668]             @ 0x000000000000a428 
    /*00009da0*/ MOVT        R6, #0                      @ .LC235 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2aa0] 
    /*00009da4*/ MOV         R0, R7 
    /*00009da8*/ MOV         R5, #0 
    /*00009dac*/ BL          0x0000000000009DAC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009db0*/ MOVW        R1, #0                      @ .LC285 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x30ec] 
    /*00009db4*/ MOV         R0, R7 
    /*00009db8*/ MOVT        R1, #0                      @ .LC285 [R_ARM_MOVT_ABS: .rodata.str1.4+0x30ec] 
    /*00009dbc*/ BL          0x0000000000009DBC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009dc0*/ MOVW        R2, #0                      @ .LC229 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a6c] 
    /*00009dc4*/ MOVW        R3, #0                      @ .LC230 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a74] 
    /*00009dc8*/ MOVT        R2, #0                      @ .LC229 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a6c] 
    /*00009dcc*/ MOVT        R3, #0                      @ .LC230 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a74] 
    /*00009dd0*/ STR         R2, [SP, #20] 
    /*00009dd4*/ STR         R3, [SP, #16] 
    /*00009dd8*/ MOVW        R2, #0                      @ .LC231 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a7c] 
    /*00009ddc*/ MOVW        R3, #0                      @ .LC232 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a88] 
    /*00009de0*/ MOVT        R2, #0                      @ .LC231 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a7c] 
    /*00009de4*/ MOVT        R3, #0                      @ .LC232 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a88] 
    /*00009de8*/ STR         R2, [SP, #12] 
    /*00009dec*/ STR         R3, [SP, #8] 
    /*00009df0*/ MOVW        R2, #0                      @ .LC233 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a90] 
    /*00009df4*/ MOVW        R3, #0                      @ .LC234 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a98] 
    /*00009df8*/ MOVT        R2, #0                      @ .LC233 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a90] 
    /*00009dfc*/ MOVT        R3, #0                      @ .LC234 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a98] 
    /*00009e00*/ STR         R2, [SP, #4] 
    /*00009e04*/ STR         R3, [SP] 
    /*00009e08*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*00009e0c*/ MOVW        R3, #0                      @ .LC227 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a40] 
    /*00009e10*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*00009e14*/ MOVT        R3, #0                      @ .LC227 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a40] 
    /*00009e18*/ MOVW        R1, #0                      @ .LC228 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a48] 
    /*00009e1c*/ MOV         R0, R7 
    /*00009e20*/ MOVT        R1, #0                      @ .LC228 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a48] 
    /*00009e24*/ BL          0x0000000000009E24          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009e28*/ B           RGN_Proc_x9e38
    RGN_Proc_x9e2c:
    /*00009e2c*/ CMP         R4, FP 
    /*00009e30*/ ADD         R5, R5, #1 
    /*00009e34*/ BEQ         RGN_Proc_x9ea0
    RGN_Proc_x9e38:
    /*00009e38*/ LDR         IP, [R4, #4]! 
    /*00009e3c*/ CMP         IP, #0 
    /*00009e40*/ BEQ         RGN_Proc_x9e2c
    /*00009e44*/ LDR         R3, [IP, #176]              @ 0xb0 
    /*00009e48*/ CMP         R3, #3 
    /*00009e4c*/ BNE         RGN_Proc_x9e2c
    /*00009e50*/ LDR         R0, [IP, #296]              @ 0x128 
    /*00009e54*/ MOV         R2, R5 
    /*00009e58*/ LDR         R3, [IP, #272]              @ 0x110 
    /*00009e5c*/ MOV         R1, R6 
    /*00009e60*/ ADD         R5, R5, #1 
    /*00009e64*/ STR         R0, [SP, #20] 
    /*00009e68*/ MOV         R0, R7 
    /*00009e6c*/ LDR         LR, [IP, #292]              @ 0x124 
    /*00009e70*/ STR         LR, [SP, #16] 
    /*00009e74*/ LDR         LR, [IP, #288]              @ 0x120 
    /*00009e78*/ STR         LR, [SP, #12] 
    /*00009e7c*/ LDR         LR, [IP, #284]              @ 0x11c 
    /*00009e80*/ STR         LR, [SP, #8] 
    /*00009e84*/ LDR         LR, [IP, #280]              @ 0x118 
    /*00009e88*/ STR         LR, [SP, #4] 
    /*00009e8c*/ LDR         IP, [IP, #276]              @ 0x114 
    /*00009e90*/ STR         IP, [SP] 
    /*00009e94*/ BL          0x0000000000009E94          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009e98*/ CMP         R4, FP 
    /*00009e9c*/ BNE         RGN_Proc_x9e38
    RGN_Proc_x9ea0:
    /*00009ea0*/ LDR         R4, [SP, #84]               @ 0x54 
    /*00009ea4*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*00009ea8*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*00009eac*/ LDR         SL, [PC, #1396]             @ 0x000000000000a428 
    /*00009eb0*/ MOV         R9, #0 
    /*00009eb4*/ MOV         R0, R4 
    /*00009eb8*/ BL          0x0000000000009EB8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009ebc*/ MOVW        R1, #0                      @ .LC286 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x314c] 
    /*00009ec0*/ MOV         R0, R4 
    /*00009ec4*/ MOVT        R1, #0                      @ .LC286 [R_ARM_MOVT_ABS: .rodata.str1.4+0x314c] 
    /*00009ec8*/ BL          0x0000000000009EC8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009ecc*/ LDR         R3, [SP, #96]               @ 0x60 
    /*00009ed0*/ MOVW        R2, #0                      @ .LC241 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b68] 
    /*00009ed4*/ MOVW        R1, #0                      @ .LC287 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x31a8] 
    /*00009ed8*/ MOVT        R2, #0                      @ .LC241 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b68] 
    /*00009edc*/ MOVT        R1, #0                      @ .LC287 [R_ARM_MOVT_ABS: .rodata.str1.4+0x31a8] 
    /*00009ee0*/ STR         R3, [SP, #24] 
    /*00009ee4*/ MOV         R0, R4 
    /*00009ee8*/ STR         R2, [SP, #36]               @ 0x24 
    /*00009eec*/ MOVW        R3, #0                      @ .LC242 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b70] 
    /*00009ef0*/ MOVW        R2, #0                      @ .LC243 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b78] 
    /*00009ef4*/ MOVT        R3, #0                      @ .LC242 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b70] 
    /*00009ef8*/ MOVT        R2, #0                      @ .LC243 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b78] 
    /*00009efc*/ STR         R3, [SP, #32] 
    /*00009f00*/ STR         R2, [SP, #28] 
    /*00009f04*/ MOVW        R3, #0                      @ .LC244 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b80] 
    /*00009f08*/ MOVW        R2, #0                      @ .LC245 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b84] 
    /*00009f0c*/ MOVT        R3, #0                      @ .LC244 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b80] 
    /*00009f10*/ MOVT        R2, #0                      @ .LC245 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b84] 
    /*00009f14*/ STR         R3, [SP, #20] 
    /*00009f18*/ STR         R2, [SP, #16] 
    /*00009f1c*/ MOVW        R3, #0                      @ .LC259 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2cec] 
    /*00009f20*/ MOVW        R2, #0                      @ .LC248 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b9c] 
    /*00009f24*/ MOVT        R3, #0                      @ .LC259 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2cec] 
    /*00009f28*/ MOVT        R2, #0                      @ .LC248 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b9c] 
    /*00009f2c*/ STR         R3, [SP, #12] 
    /*00009f30*/ STR         R2, [SP, #8] 
    /*00009f34*/ MOVW        R3, #0                      @ .LC249 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ba0] 
    /*00009f38*/ MOVW        R2, #0                      @ .LC250 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ba4] 
    /*00009f3c*/ MOVT        R3, #0                      @ .LC249 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ba0] 
    /*00009f40*/ MOVT        R2, #0                      @ .LC250 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ba4] 
    /*00009f44*/ STR         R3, [SP, #4] 
    /*00009f48*/ STR         R2, [SP] 
    /*00009f4c*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*00009f50*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*00009f54*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*00009f58*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*00009f5c*/ BL          0x0000000000009F5C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00009f60*/ MOVW        R3, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*00009f64*/ MOVW        R2, #0                      @ .LC192 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2804] 
    /*00009f68*/ MOVT        R3, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*00009f6c*/ MOVT        R2, #0                      @ .LC192 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2804] 
    /*00009f70*/ STR         R3, [SP, #104]              @ 0x68 
    /*00009f74*/ STR         R2, [SP, #108]              @ 0x6c 
    /*00009f78*/ MOVW        R3, #0                      @ .LC288 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x31d0] 
    /*00009f7c*/ STR         SL, [SP, #96]               @ 0x60 
    /*00009f80*/ MOVT        R3, #0                      @ .LC288 [R_ARM_MOVT_ABS: .rodata.str1.4+0x31d0] 
    /*00009f84*/ STR         FP, [SP, #116]              @ 0x74 
    /*00009f88*/ STR         R3, [SP, #112]              @ 0x70 
    /*00009f8c*/ B           RGN_Proc_x9fa4
    RGN_Proc_x9f90:
    /*00009f90*/ LDR         R3, [SP, #96]               @ 0x60 
    /*00009f94*/ ADD         R9, R9, #1 
    /*00009f98*/ LDR         R2, [SP, #116]              @ 0x74 
    /*00009f9c*/ CMP         R3, R2 
    /*00009fa0*/ BEQ         RGN_Proc_x95dc
    RGN_Proc_x9fa4:
    /*00009fa4*/ LDR         R3, [SP, #96]               @ 0x60 
    /*00009fa8*/ LDR         R7, [R3, #4]! 
    /*00009fac*/ CMP         R7, #0 
    /*00009fb0*/ STR         R3, [SP, #96]               @ 0x60 
    /*00009fb4*/ BEQ         RGN_Proc_x9f90
    /*00009fb8*/ LDR         R3, [R7, #176]              @ 0xb0 
    /*00009fbc*/ CMP         R3, #3 
    /*00009fc0*/ BNE         RGN_Proc_x9f90
    /*00009fc4*/ LDR         R5, [R7, #308]              @ 0x134 
    /*00009fc8*/ ADD         R3, R7, #308                @ 0x134 
    /*00009fcc*/ STR         R3, [SP, #100]              @ 0x64 
    /*00009fd0*/ CMP         R3, R5 
    /*00009fd4*/ BEQ         RGN_Proc_x9f90
    /*00009fd8*/ LDR         R6, [R5] 
    /*00009fdc*/ ADD         R8, SP, #176                @ 0xb0 
    /*00009fe0*/ B           RGN_Proc_x9fe8
    RGN_Proc_x9fe4:
    /*00009fe4*/ MOV         R6, R3 
    RGN_Proc_x9fe8:
    /*00009fe8*/ MOV         R1, R8 
    /*00009fec*/ MOV         R0, SL 
    /*00009ff0*/ BL          0x0000000000009FF0          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00009ff4*/ MOV         R2, R9 
    /*00009ff8*/ ADD         R1, R5, #8 
    /*00009ffc*/ LDR         R0, [R7, #176]              @ 0xb0 
    /*0000a000*/ BL          0x000000000000A000          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*0000a004*/ SUBS        R4, R0, #0 
    /*0000a008*/ BEQ         RGN_Proc_xa0b4
    /*0000a00c*/ LDR         R0, [R5, #8] 
    /*0000a010*/ BL          0x0000000000000000 
    /*0000a014*/ LDR         R1, [R4, #116]              @ 0x74 
    /*0000a018*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*0000a01c*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*0000a020*/ SUB         R1, R1, #1 
    /*0000a024*/ LDR         LR, [R4] 
    /*0000a028*/ CMP         R1, #3 
    /*0000a02c*/ MOVW        IP, #0                      @ .LC193 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2808] 
    /*0000a030*/ ADD         R2, R2, R1, LSL #2 
    /*0000a034*/ LDR         R1, [R4, #64]               @ 0x40 
    /*0000a038*/ MOVT        IP, #0                      @ .LC193 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2808] 
    /*0000a03c*/ LDR         FP, [R5, #12] 
    /*0000a040*/ LDRLS       IP, [R2, #1368]             @ 0x558 
    /*0000a044*/ CMP         LR, #0 
    /*0000a048*/ LDR         R3, [R7, #176]              @ 0xb0 
    /*0000a04c*/ MOV         R2, R9 
    /*0000a050*/ LDR         R5, [R5, #16] 
    /*0000a054*/ STR         R1, [SP, #36]               @ 0x24 
    /*0000a058*/ LDR         R1, [SP, #104]              @ 0x68 
    /*0000a05c*/ STR         R0, [SP, #88]               @ 0x58 
    /*0000a060*/ LDR         R0, [SP, #108]              @ 0x6c 
    /*0000a064*/ MOVEQ       R1, R0 
    /*0000a068*/ LDR         R0, [R4, #80]               @ 0x50 
    /*0000a06c*/ STR         R1, [SP, #92]               @ 0x5c 
    /*0000a070*/ LDR         R1, [SP, #112]              @ 0x70 
    /*0000a074*/ STR         R0, [SP, #32] 
    /*0000a078*/ LDR         LR, [R4, #76]               @ 0x4c 
    /*0000a07c*/ LDR         R0, [SP, #84]               @ 0x54 
    /*0000a080*/ STR         LR, [SP, #28] 
    /*0000a084*/ LDR         LR, [R4, #12] 
    /*0000a088*/ STR         LR, [SP, #24] 
    /*0000a08c*/ LDR         R4, [R4, #8] 
    /*0000a090*/ STR         IP, [SP, #12] 
    /*0000a094*/ LDR         IP, [SP, #88]               @ 0x58 
    /*0000a098*/ LDR         LR, [SP, #92]               @ 0x5c 
    /*0000a09c*/ STR         R4, [SP, #20] 
    /*0000a0a0*/ STR         IP, [SP] 
    /*0000a0a4*/ STR         LR, [SP, #16] 
    /*0000a0a8*/ STR         R5, [SP, #8] 
    /*0000a0ac*/ STR         FP, [SP, #4] 
    /*0000a0b0*/ BL          0x000000000000A0B0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    RGN_Proc_xa0b4:
    /*0000a0b4*/ MOV         R1, R8 
    /*0000a0b8*/ MOV         R0, SL 
    /*0000a0bc*/ BL          0x000000000000A0BC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000a0c0*/ LDR         R3, [SP, #100]              @ 0x64 
    /*0000a0c4*/ MOV         R5, R6 
    /*0000a0c8*/ CMP         R3, R6 
    /*0000a0cc*/ LDR         R3, [R6] 
    /*0000a0d0*/ BNE         RGN_Proc_x9fe4
    /*0000a0d4*/ B           RGN_Proc_x9f90
    RGN_Proc_xa0d8:
    /*0000a0d8*/ LDR         SL, [SP, #84]               @ 0x54 
    /*0000a0dc*/ MOVW        R3, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*0000a0e0*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*0000a0e4*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*0000a0e8*/ MOV         R4, R3 
    /*0000a0ec*/ MOVW        R5, #0                      @ .LC217 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x291c] 
    /*0000a0f0*/ MOV         R0, SL 
    /*0000a0f4*/ MOVT        R5, #0                      @ .LC217 [R_ARM_MOVT_ABS: .rodata.str1.4+0x291c] 
    /*0000a0f8*/ BL          0x000000000000A0F8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a0fc*/ MOVW        R1, #0                      @ .LC205 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x285c] 
    /*0000a100*/ MOV         R0, SL 
    /*0000a104*/ MOVT        R1, #0                      @ .LC205 [R_ARM_MOVT_ABS: .rodata.str1.4+0x285c] 
    /*0000a108*/ BL          0x000000000000A108          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a10c*/ MOVW        R2, #0                      @ .LC209 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28e8] 
    /*0000a110*/ MOVW        R3, #0                      @ .LC210 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28f0] 
    /*0000a114*/ MOVT        R2, #0                      @ .LC209 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28e8] 
    /*0000a118*/ MOVT        R3, #0                      @ .LC210 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28f0] 
    /*0000a11c*/ STR         R2, [SP, #28] 
    /*0000a120*/ STR         R3, [SP, #24] 
    /*0000a124*/ MOVW        R2, #0                      @ .LC211 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28f8] 
    /*0000a128*/ MOVW        R3, #0                      @ .LC212 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28fc] 
    /*0000a12c*/ MOVT        R2, #0                      @ .LC211 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28f8] 
    /*0000a130*/ MOVT        R3, #0                      @ .LC212 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28fc] 
    /*0000a134*/ STR         R2, [SP, #20] 
    /*0000a138*/ STR         R3, [SP, #16] 
    /*0000a13c*/ MOVW        R2, #0                      @ .LC213 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2904] 
    /*0000a140*/ MOVW        R3, #0                      @ .LC214 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2908] 
    /*0000a144*/ MOVT        R2, #0                      @ .LC213 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2904] 
    /*0000a148*/ MOVT        R3, #0                      @ .LC214 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2908] 
    /*0000a14c*/ STR         R2, [SP, #12] 
    /*0000a150*/ STR         R3, [SP, #8] 
    /*0000a154*/ MOVW        R2, #0                      @ .LC215 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x290c] 
    /*0000a158*/ MOVW        R3, #0                      @ .LC216 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2914] 
    /*0000a15c*/ MOVT        R2, #0                      @ .LC215 [R_ARM_MOVT_ABS: .rodata.str1.4+0x290c] 
    /*0000a160*/ MOVT        R3, #0                      @ .LC216 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2914] 
    /*0000a164*/ STR         R2, [SP, #4] 
    /*0000a168*/ STR         R3, [SP] 
    /*0000a16c*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*0000a170*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*0000a174*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*0000a178*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*0000a17c*/ MOVW        R1, #0                      @ .LC208 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c4] 
    /*0000a180*/ MOV         R0, SL 
    /*0000a184*/ MOVT        R1, #0                      @ .LC208 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c4] 
    /*0000a188*/ BL          0x000000000000A188          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a18c*/ MOV         R3, R4 
    /*0000a190*/ MOVT        R3, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*0000a194*/ STR         R3, [SP, #96]               @ 0x60 
    /*0000a198*/ STR         R3, [SP, #88]               @ 0x58 
    /*0000a19c*/ MOV         R4, #0 
    /*0000a1a0*/ LDR         R3, [PC, #640]              @ 0x000000000000a428 
    /*0000a1a4*/ MOVW        R6, #0                      @ .LC192 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2804] 
    /*0000a1a8*/ LDR         R9, [SP, #136]              @ 0x88 
    /*0000a1ac*/ MOVT        R6, #0                      @ .LC192 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2804] 
    /*0000a1b0*/ STR         R6, [SP, #92]               @ 0x5c 
    /*0000a1b4*/ MOV         R8, R3 
    RGN_Proc_xa1b8:
    /*0000a1b8*/ LDR         IP, [R8, #4]! 
    /*0000a1bc*/ MOV         R2, R4 
    /*0000a1c0*/ MOV         R1, R5 
    /*0000a1c4*/ MOV         R0, SL 
    /*0000a1c8*/ CMP         IP, #0 
    /*0000a1cc*/ ADD         R4, R4, #1 
    /*0000a1d0*/ BEQ         RGN_Proc_xa238
    /*0000a1d4*/ LDR         R3, [IP, #176]              @ 0xb0 
    /*0000a1d8*/ CMP         R3, #0 
    /*0000a1dc*/ BNE         RGN_Proc_xa238
    /*0000a1e0*/ LDR         LR, [IP, #180]              @ 0xb4 
    /*0000a1e4*/ LDR         R7, [SP, #92]               @ 0x5c 
    /*0000a1e8*/ LDR         FP, [IP, #112]              @ 0x70 
    /*0000a1ec*/ CMP         LR, #0 
    /*0000a1f0*/ LDR         R6, [SP, #88]               @ 0x58 
    /*0000a1f4*/ MOVEQ       R6, R7 
    /*0000a1f8*/ STR         FP, [SP, #40]               @ 0x28 
    /*0000a1fc*/ MOV         LR, R6 
    /*0000a200*/ LDRD        R6, [IP, #104]              @ 0x68 
    /*0000a204*/ STRD        R6, [SP, #32] 
    /*0000a208*/ LDRD        R6, [IP, #96]               @ 0x60 
    /*0000a20c*/ STRD        R6, [SP, #24] 
    /*0000a210*/ LDR         FP, [IP, #76]               @ 0x4c 
    /*0000a214*/ STR         FP, [SP, #16] 
    /*0000a218*/ LDR         FP, [IP, #28] 
    /*0000a21c*/ STR         FP, [SP, #12] 
    /*0000a220*/ LDR         FP, [IP, #24] 
    /*0000a224*/ STR         FP, [SP, #8] 
    /*0000a228*/ LDR         IP, [IP, #120]              @ 0x78 
    /*0000a22c*/ STR         LR, [SP] 
    /*0000a230*/ STR         IP, [SP, #4] 
    /*0000a234*/ BL          0x000000000000A234          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    RGN_Proc_xa238:
    /*0000a238*/ CMP         R9, R8 
    /*0000a23c*/ BNE         RGN_Proc_xa1b8
    /*0000a240*/ LDR         R5, [SP, #84]               @ 0x54 
    /*0000a244*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*0000a248*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*0000a24c*/ STR         R8, [SP, #140]              @ 0x8c 
    /*0000a250*/ MOVW        R4, #0                      @ .LC221 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29b4] 
    /*0000a254*/ MOVT        R4, #0                      @ .LC221 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29b4] 
    /*0000a258*/ MOV         R0, R5 
    /*0000a25c*/ BL          0x000000000000A25C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a260*/ MOV         R0, R5 
    /*0000a264*/ MOVW        R1, #0                      @ .LC218 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2944] 
    /*0000a268*/ MOVT        R1, #0                      @ .LC218 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2944] 
    /*0000a26c*/ BL          0x000000000000A26C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a270*/ MOV         R0, R5 
    /*0000a274*/ MOVW        R2, #0                      @ .LC220 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29ac] 
    /*0000a278*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*0000a27c*/ MOVT        R2, #0                      @ .LC220 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29ac] 
    /*0000a280*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*0000a284*/ STR         R2, [SP] 
    /*0000a288*/ MOVW        R1, #0                      @ .LC219 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29a0] 
    /*0000a28c*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*0000a290*/ MOVT        R1, #0                      @ .LC219 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29a0] 
    /*0000a294*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*0000a298*/ MOV         R5, #0 
    /*0000a29c*/ BL          0x000000000000A29C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    RGN_Proc_xa2a0:
    /*0000a2a0*/ MOV         R2, R5 
    /*0000a2a4*/ MOV         R1, R4 
    /*0000a2a8*/ ADD         R5, R5, #1 
    /*0000a2ac*/ LDR         R0, [SP, #84]               @ 0x54 
    /*0000a2b0*/ BL          0x000000000000A2B0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a2b4*/ CMP         R5, #5 
    /*0000a2b8*/ BNE         RGN_Proc_xa2a0
    /*0000a2bc*/ LDR         R8, [SP, #84]               @ 0x54 
    /*0000a2c0*/ MOV         R2, R5 
    /*0000a2c4*/ MOVW        R1, #0                      @ .LC222 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c0] 
    /*0000a2c8*/ MOVT        R1, #0                      @ .LC222 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c0] 
    /*0000a2cc*/ MOVW        SL, #0                      @ .LC223 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29cc] 
    /*0000a2d0*/ MOVW        R9, #0                      @ .LC224 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29d8] 
    /*0000a2d4*/ MOV         R0, R8 
    /*0000a2d8*/ MOVT        SL, #0                      @ .LC223 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29cc] 
    /*0000a2dc*/ BL          0x000000000000A2DC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a2e0*/ MOVW        FP, #0                      @ .LC225 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29dc] 
    /*0000a2e4*/ MOVT        R9, #0                      @ .LC224 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29d8] 
    /*0000a2e8*/ MOVT        FP, #0                      @ .LC225 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29dc] 
    /*0000a2ec*/ LDR         R6, [PC, #308]              @ 0x000000000000a428 
    /*0000a2f0*/ MOV         R7, #0 
    /*0000a2f4*/ STR         FP, [SP, #92]               @ 0x5c 
    /*0000a2f8*/ STR         SL, [SP, #88]               @ 0x58 
    /*0000a2fc*/ LDR         FP, [SP, #140]              @ 0x8c 
    /*0000a300*/ B           RGN_Proc_xa310
    RGN_Proc_xa304:
    /*0000a304*/ CMP         FP, R6 
    /*0000a308*/ ADD         R7, R7, #1 
    /*0000a30c*/ BEQ         RGN_Proc_xa37c
    RGN_Proc_xa310:
    /*0000a310*/ LDR         R5, [R6, #4]! 
    /*0000a314*/ CMP         R5, #0 
    /*0000a318*/ BEQ         RGN_Proc_xa304
    /*0000a31c*/ LDR         R3, [R5, #176]              @ 0xb0 
    /*0000a320*/ CMP         R3, #0 
    /*0000a324*/ BNE         RGN_Proc_xa304
    /*0000a328*/ LDR         IP, [R5, #116]              @ 0x74 
    /*0000a32c*/ MOV         R2, R7 
    /*0000a330*/ LDR         R1, [SP, #88]               @ 0x58 
    /*0000a334*/ MOV         R0, R8 
    /*0000a338*/ ADD         R4, R5, #184                @ 0xb8 
    /*0000a33c*/ ADD         SL, R5, #204                @ 0xcc 
    /*0000a340*/ STR         IP, [SP] 
    /*0000a344*/ BL          0x000000000000A344          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    RGN_Proc_xa348:
    /*0000a348*/ LDR         R2, [R4], #4 
    /*0000a34c*/ MOV         R1, R9 
    /*0000a350*/ MOV         R0, R8 
    /*0000a354*/ BL          0x000000000000A354          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a358*/ CMP         SL, R4 
    /*0000a35c*/ BNE         RGN_Proc_xa348
    /*0000a360*/ LDR         R2, [R5, #204]              @ 0xcc 
    /*0000a364*/ MOV         R0, R8 
    /*0000a368*/ LDR         R1, [SP, #92]               @ 0x5c 
    /*0000a36c*/ ADD         R7, R7, #1 
    /*0000a370*/ BL          0x000000000000A370          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a374*/ CMP         FP, R6 
    /*0000a378*/ BNE         RGN_Proc_xa310
    RGN_Proc_xa37c:
    /*0000a37c*/ LDR         R8, [SP, #84]               @ 0x54 
    /*0000a380*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*0000a384*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*0000a388*/ MOVW        R6, #0                      @ .LC235 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2aa0] 
    /*0000a38c*/ LDR         R4, [PC, #148]              @ 0x000000000000a428 
    /*0000a390*/ MOVT        R6, #0                      @ .LC235 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2aa0] 
    /*0000a394*/ MOV         R0, R8 
    /*0000a398*/ MOV         R5, #0 
    /*0000a39c*/ BL          0x000000000000A39C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a3a0*/ MOVW        R1, #0                      @ .LC226 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29e4] 
    /*0000a3a4*/ MOV         R0, R8 
    /*0000a3a8*/ MOVT        R1, #0                      @ .LC226 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29e4] 
    /*0000a3ac*/ BL          0x000000000000A3AC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a3b0*/ MOVW        R2, #0                      @ .LC229 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a6c] 
    /*0000a3b4*/ MOVW        R3, #0                      @ .LC230 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a74] 
    /*0000a3b8*/ MOVT        R2, #0                      @ .LC229 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a6c] 
    /*0000a3bc*/ MOVT        R3, #0                      @ .LC230 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a74] 
    /*0000a3c0*/ STR         R2, [SP, #20] 
    /*0000a3c4*/ STR         R3, [SP, #16] 
    /*0000a3c8*/ MOVW        R2, #0                      @ .LC231 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a7c] 
    /*0000a3cc*/ MOVW        R3, #0                      @ .LC232 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a88] 
    /*0000a3d0*/ MOVT        R2, #0                      @ .LC231 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a7c] 
    /*0000a3d4*/ MOVT        R3, #0                      @ .LC232 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a88] 
    /*0000a3d8*/ STR         R2, [SP, #12] 
    /*0000a3dc*/ STR         R3, [SP, #8] 
    /*0000a3e0*/ MOVW        R2, #0                      @ .LC233 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a90] 
    /*0000a3e4*/ MOVW        R3, #0                      @ .LC234 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a98] 
    /*0000a3e8*/ MOVT        R2, #0                      @ .LC233 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a90] 
    /*0000a3ec*/ MOVT        R3, #0                      @ .LC234 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a98] 
    /*0000a3f0*/ STR         R2, [SP, #4] 
    /*0000a3f4*/ STR         R3, [SP] 
    /*0000a3f8*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*0000a3fc*/ MOVW        R3, #0                      @ .LC227 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a40] 
    /*0000a400*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*0000a404*/ MOVT        R3, #0                      @ .LC227 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a40] 
    /*0000a408*/ MOVW        R1, #0                      @ .LC228 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a48] 
    /*0000a40c*/ MOV         R0, R8 
    /*0000a410*/ MOVT        R1, #0                      @ .LC228 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a48] 
    /*0000a414*/ BL          0x000000000000A414          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a418*/ LDR         R7, [SP, #140]              @ 0x8c 
    /*0000a41c*/ B           RGN_Proc_xa438
    /*0000a420*/ LDRDEQ      R1, [R0], -R8 
    /*0000a424*/ ANDEQ       R0, R0, IP, LSL #4 
    /*0000a428*/ ANDEQ       R0, R0, R4 
    RGN_Proc_xa42c:
    /*0000a42c*/ CMP         R4, R7 
    /*0000a430*/ ADD         R5, R5, #1 
    /*0000a434*/ BEQ         RGN_Proc_xa4a0
    RGN_Proc_xa438:
    /*0000a438*/ LDR         IP, [R4, #4]! 
    /*0000a43c*/ CMP         IP, #0 
    /*0000a440*/ BEQ         RGN_Proc_xa42c
    /*0000a444*/ LDR         R3, [IP, #176]              @ 0xb0 
    /*0000a448*/ CMP         R3, #0 
    /*0000a44c*/ BNE         RGN_Proc_xa42c
    /*0000a450*/ LDR         R0, [IP, #296]              @ 0x128 
    /*0000a454*/ MOV         R2, R5 
    /*0000a458*/ LDR         R3, [IP, #272]              @ 0x110 
    /*0000a45c*/ MOV         R1, R6 
    /*0000a460*/ ADD         R5, R5, #1 
    /*0000a464*/ STR         R0, [SP, #20] 
    /*0000a468*/ MOV         R0, R8 
    /*0000a46c*/ LDR         LR, [IP, #292]              @ 0x124 
    /*0000a470*/ STR         LR, [SP, #16] 
    /*0000a474*/ LDR         LR, [IP, #288]              @ 0x120 
    /*0000a478*/ STR         LR, [SP, #12] 
    /*0000a47c*/ LDR         LR, [IP, #284]              @ 0x11c 
    /*0000a480*/ STR         LR, [SP, #8] 
    /*0000a484*/ LDR         LR, [IP, #280]              @ 0x118 
    /*0000a488*/ STR         LR, [SP, #4] 
    /*0000a48c*/ LDR         IP, [IP, #276]              @ 0x114 
    /*0000a490*/ STR         IP, [SP] 
    /*0000a494*/ BL          0x000000000000A494          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a498*/ CMP         R4, R7 
    /*0000a49c*/ BNE         RGN_Proc_xa438
    RGN_Proc_xa4a0:
    /*0000a4a0*/ LDR         R4, [SP, #84]               @ 0x54 
    /*0000a4a4*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*0000a4a8*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*0000a4ac*/ MOVW        SL, #0                      @ .LC192 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2804] 
    /*0000a4b0*/ MOVT        SL, #0                      @ .LC192 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2804] 
    /*0000a4b4*/ MOV         R0, R4 
    /*0000a4b8*/ BL          0x000000000000A4B8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a4bc*/ MOVW        R1, #0                      @ .LC236 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ac4] 
    /*0000a4c0*/ MOV         R0, R4 
    /*0000a4c4*/ MOVT        R1, #0                      @ .LC236 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ac4] 
    /*0000a4c8*/ BL          0x000000000000A4C8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a4cc*/ LDR         R3, [SP, #96]               @ 0x60 
    /*0000a4d0*/ MOVW        R2, #0                      @ .LC238 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b54] 
    /*0000a4d4*/ MOVW        R1, #0                      @ .LC237 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b20] 
    /*0000a4d8*/ MOVT        R2, #0                      @ .LC238 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b54] 
    /*0000a4dc*/ MOVT        R1, #0                      @ .LC237 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b20] 
    /*0000a4e0*/ STR         R3, [SP, #28] 
    /*0000a4e4*/ MOV         R0, R4 
    /*0000a4e8*/ STR         R2, [SP, #52]               @ 0x34 
    /*0000a4ec*/ MOVW        R3, #0                      @ .LC239 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b5c] 
    /*0000a4f0*/ MOVW        R2, #0                      @ .LC240 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b60] 
    /*0000a4f4*/ MOVT        R3, #0                      @ .LC239 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b5c] 
    /*0000a4f8*/ MOVT        R2, #0                      @ .LC240 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b60] 
    /*0000a4fc*/ STR         R3, [SP, #48]               @ 0x30 
    /*0000a500*/ STR         R2, [SP, #44]               @ 0x2c 
    /*0000a504*/ MOVW        R3, #0                      @ .LC241 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b68] 
    /*0000a508*/ MOVW        R2, #0                      @ .LC242 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b70] 
    /*0000a50c*/ MOVT        R3, #0                      @ .LC241 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b68] 
    /*0000a510*/ MOVT        R2, #0                      @ .LC242 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b70] 
    /*0000a514*/ STR         R3, [SP, #40]               @ 0x28 
    /*0000a518*/ STR         R2, [SP, #36]               @ 0x24 
    /*0000a51c*/ MOVW        R3, #0                      @ .LC243 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b78] 
    /*0000a520*/ MOVW        R2, #0                      @ .LC244 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b80] 
    /*0000a524*/ MOVT        R3, #0                      @ .LC243 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b78] 
    /*0000a528*/ MOVT        R2, #0                      @ .LC244 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b80] 
    /*0000a52c*/ STR         R3, [SP, #32] 
    /*0000a530*/ STR         R2, [SP, #24] 
    /*0000a534*/ MOVW        R3, #0                      @ .LC245 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b84] 
    /*0000a538*/ MOVW        R2, #0                      @ .LC246 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b88] 
    /*0000a53c*/ MOVT        R3, #0                      @ .LC245 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b84] 
    /*0000a540*/ MOVT        R2, #0                      @ .LC246 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b88] 
    /*0000a544*/ STR         R3, [SP, #20] 
    /*0000a548*/ STR         R2, [SP, #16] 
    /*0000a54c*/ MOVW        R3, #0                      @ .LC247 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b94] 
    /*0000a550*/ MOVW        R2, #0                      @ .LC248 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b9c] 
    /*0000a554*/ MOVT        R3, #0                      @ .LC247 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b94] 
    /*0000a558*/ MOVT        R2, #0                      @ .LC248 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b9c] 
    /*0000a55c*/ STR         R3, [SP, #12] 
    /*0000a560*/ STR         R2, [SP, #8] 
    /*0000a564*/ MOVW        R3, #0                      @ .LC249 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ba0] 
    /*0000a568*/ MOVW        R2, #0                      @ .LC250 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ba4] 
    /*0000a56c*/ MOVT        R3, #0                      @ .LC249 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ba0] 
    /*0000a570*/ MOVT        R2, #0                      @ .LC250 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ba4] 
    /*0000a574*/ STR         R3, [SP, #4] 
    /*0000a578*/ STR         R2, [SP] 
    /*0000a57c*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*0000a580*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*0000a584*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*0000a588*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*0000a58c*/ MOV         R8, SL 
    /*0000a590*/ BL          0x000000000000A590          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a594*/ LDR         R3, [PC, #-372]             @ 0x000000000000a428 
    /*0000a598*/ STR         R3, [SP, #132]              @ 0x84 
    /*0000a59c*/ MOV         R3, #0 
    /*0000a5a0*/ STR         R3, [SP, #88]               @ 0x58 
    RGN_Proc_xa5a4:
    /*0000a5a4*/ LDR         R3, [SP, #132]              @ 0x84 
    /*0000a5a8*/ LDR         R6, [R3, #4]! 
    /*0000a5ac*/ CMP         R6, #0 
    /*0000a5b0*/ STR         R3, [SP, #132]              @ 0x84 
    /*0000a5b4*/ BEQ         RGN_Proc_xb5ec
    /*0000a5b8*/ LDR         R3, [R6, #176]              @ 0xb0 
    /*0000a5bc*/ CMP         R3, #0 
    /*0000a5c0*/ BNE         RGN_Proc_xb5ec
    /*0000a5c4*/ LDR         R5, [R6, #308]              @ 0x134 
    /*0000a5c8*/ ADD         R3, R6, #308                @ 0x134 
    /*0000a5cc*/ STR         R3, [SP, #124]              @ 0x7c 
    /*0000a5d0*/ CMP         R3, R5 
    /*0000a5d4*/ BEQ         RGN_Proc_xb5ec
    /*0000a5d8*/ LDR         R9, [R5] 
    /*0000a5dc*/ LDR         SL, [PC, #-448]             @ 0x000000000000a424 
    /*0000a5e0*/ MOV         FP, R9 
    /*0000a5e4*/ MOV         R9, R6 
    /*0000a5e8*/ MOV         R6, R8 
    RGN_Proc_xa5ec:
    /*0000a5ec*/ LDR         R3, [SP, #128]              @ 0x80 
    /*0000a5f0*/ LDR         R1, [R5, #8] 
    /*0000a5f4*/ LDR         R3, [R3, #524]              @ 0x20c 
    /*0000a5f8*/ CMP         R3, SL 
    /*0000a5fc*/ LDR         R4, [R3] 
    /*0000a600*/ BEQ         RGN_Proc_xa630
    /*0000a604*/ LDR         R2, [R3, #-332]             @ 0xfffffeb4 
    /*0000a608*/ CMP         R2, R1 
    /*0000a60c*/ BNE         RGN_Proc_xa624
    /*0000a610*/ B           RGN_Proc_xb460
    RGN_Proc_xa614:
    /*0000a614*/ LDR         R2, [R4, #-332]             @ 0xfffffeb4 
    /*0000a618*/ CMP         R1, R2 
    /*0000a61c*/ BEQ         RGN_Proc_xb464
    /*0000a620*/ MOV         R4, R3 
    RGN_Proc_xa624:
    /*0000a624*/ CMP         R4, SL 
    /*0000a628*/ LDR         R3, [R4] 
    /*0000a62c*/ BNE         RGN_Proc_xa614
    RGN_Proc_xa630:
    /*0000a630*/ LDR         R3, [SP, #124]              @ 0x7c 
    /*0000a634*/ MOV         R5, FP 
    /*0000a638*/ CMP         R3, FP 
    /*0000a63c*/ LDR         R3, [FP] 
    /*0000a640*/ BEQ         RGN_Proc_xb5e8
    /*0000a644*/ MOV         FP, R3 
    /*0000a648*/ B           RGN_Proc_xa5ec
    RGN_Proc_xa64c:
    /*0000a64c*/ LDR         SL, [SP, #84]               @ 0x54 
    /*0000a650*/ MOVW        R3, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*0000a654*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*0000a658*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*0000a65c*/ MOV         R4, R3 
    /*0000a660*/ MOVW        R6, #0                      @ .LC192 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2804] 
    /*0000a664*/ MOV         R0, SL 
    /*0000a668*/ MOVW        R5, #0                      @ .LC254 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c44] 
    /*0000a66c*/ BL          0x000000000000A66C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a670*/ MOVW        R1, #0                      @ .LC252 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2bdc] 
    /*0000a674*/ MOV         R0, SL 
    /*0000a678*/ MOVT        R1, #0                      @ .LC252 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2bdc] 
    /*0000a67c*/ BL          0x000000000000A67C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a680*/ MOV         IP, R4 
    /*0000a684*/ MOVT        IP, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*0000a688*/ MOVW        R2, #0                      @ .LC216 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2914] 
    /*0000a68c*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*0000a690*/ MOVT        R2, #0                      @ .LC216 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2914] 
    /*0000a694*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*0000a698*/ STR         R2, [SP] 
    /*0000a69c*/ MOVW        R1, #0                      @ .LC253 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c38] 
    /*0000a6a0*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*0000a6a4*/ MOVT        R1, #0                      @ .LC253 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c38] 
    /*0000a6a8*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*0000a6ac*/ MOV         R0, SL 
    /*0000a6b0*/ MOV         R7, IP 
    /*0000a6b4*/ STR         IP, [SP, #96]               @ 0x60 
    /*0000a6b8*/ MOVT        R6, #0                      @ .LC192 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2804] 
    /*0000a6bc*/ BL          0x000000000000A6BC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a6c0*/ MOVT        R5, #0                      @ .LC254 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c44] 
    /*0000a6c4*/ LDR         R8, [PC, #-676]             @ 0x000000000000a428 
    /*0000a6c8*/ MOV         R4, #0 
    /*0000a6cc*/ LDR         R9, [SP, #136]              @ 0x88 
    /*0000a6d0*/ B           RGN_Proc_xa6e0
    RGN_Proc_xa6d4:
    /*0000a6d4*/ CMP         R8, R9 
    /*0000a6d8*/ ADD         R4, R4, #1 
    /*0000a6dc*/ BEQ         RGN_Proc_xa728
    RGN_Proc_xa6e0:
    /*0000a6e0*/ LDR         R2, [R8, #4]! 
    /*0000a6e4*/ CMP         R2, #0 
    /*0000a6e8*/ BEQ         RGN_Proc_xa6d4
    /*0000a6ec*/ LDR         R3, [R2, #176]              @ 0xb0 
    /*0000a6f0*/ CMP         R3, #1 
    /*0000a6f4*/ BNE         RGN_Proc_xa6d4
    /*0000a6f8*/ LDR         R0, [R2, #180]              @ 0xb4 
    /*0000a6fc*/ MOV         R1, R5 
    /*0000a700*/ MOV         R2, R4 
    /*0000a704*/ ADD         R4, R4, #1 
    /*0000a708*/ CMP         R0, #0 
    /*0000a70c*/ MOVNE       R0, R7 
    /*0000a710*/ MOVEQ       R0, R6 
    /*0000a714*/ STR         R0, [SP] 
    /*0000a718*/ MOV         R0, SL 
    /*0000a71c*/ BL          0x000000000000A71C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a720*/ CMP         R8, R9 
    /*0000a724*/ BNE         RGN_Proc_xa6e0
    RGN_Proc_xa728:
    /*0000a728*/ LDR         R5, [SP, #84]               @ 0x54 
    /*0000a72c*/ MOVW        R3, #0                      @ .LC257 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2cd8] 
    /*0000a730*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*0000a734*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*0000a738*/ MOV         R4, R3 
    /*0000a73c*/ LDR         FP, [PC, #-796]             @ 0x000000000000a428 
    /*0000a740*/ MOV         R0, R5 
    /*0000a744*/ MOV         SL, #0 
    /*0000a748*/ BL          0x000000000000A748          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a74c*/ MOVW        R1, #0                      @ .LC255 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c50] 
    /*0000a750*/ MOV         R0, R5 
    /*0000a754*/ MOVT        R1, #0                      @ .LC255 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c50] 
    /*0000a758*/ BL          0x000000000000A758          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a75c*/ LDR         R2, [SP, #96]               @ 0x60 
    /*0000a760*/ MOV         R3, R4 
    /*0000a764*/ MOVT        R3, #0                      @ .LC257 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2cd8] 
    /*0000a768*/ STR         R3, [SP, #172]              @ 0xac 
    /*0000a76c*/ MOVW        R1, #0                      @ .LC256 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2cac] 
    /*0000a770*/ STR         R3, [SP, #44]               @ 0x2c 
    /*0000a774*/ MOVW        R3, #0                      @ .LC258 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ce4] 
    /*0000a778*/ MOVT        R3, #0                      @ .LC258 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ce4] 
    /*0000a77c*/ STR         R3, [SP, #156]              @ 0x9c 
    /*0000a780*/ STR         R3, [SP, #36]               @ 0x24 
    /*0000a784*/ MOVW        R3, #0                      @ .LC213 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2904] 
    /*0000a788*/ MOVT        R3, #0                      @ .LC213 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2904] 
    /*0000a78c*/ STR         R3, [SP, #28] 
    /*0000a790*/ MOVW        R3, #0                      @ .LC244 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b80] 
    /*0000a794*/ MOVT        R3, #0                      @ .LC244 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b80] 
    /*0000a798*/ STR         R3, [SP, #16] 
    /*0000a79c*/ MOVW        R3, #0                      @ .LC248 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b9c] 
    /*0000a7a0*/ MOVT        R3, #0                      @ .LC248 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b9c] 
    /*0000a7a4*/ STR         R3, [SP, #144]              @ 0x90 
    /*0000a7a8*/ STR         R3, [SP, #8] 
    /*0000a7ac*/ MOVW        R3, #0                      @ .LC250 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ba4] 
    /*0000a7b0*/ MOVT        R3, #0                      @ .LC250 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ba4] 
    /*0000a7b4*/ STR         R2, [SP, #20] 
    /*0000a7b8*/ STR         R3, [SP, #152]              @ 0x98 
    /*0000a7bc*/ MOVW        R2, #0                      @ .LC241 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b68] 
    /*0000a7c0*/ MOVT        R1, #0                      @ .LC256 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2cac] 
    /*0000a7c4*/ MOVT        R2, #0                      @ .LC241 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b68] 
    /*0000a7c8*/ MOV         R0, R5 
    /*0000a7cc*/ STR         R2, [SP, #140]              @ 0x8c 
    /*0000a7d0*/ STR         R2, [SP, #40]               @ 0x28 
    /*0000a7d4*/ MOVW        R2, #0                      @ .LC259 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2cec] 
    /*0000a7d8*/ MOVT        R2, #0                      @ .LC259 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2cec] 
    /*0000a7dc*/ STR         R2, [SP, #160]              @ 0xa0 
    /*0000a7e0*/ STR         R2, [SP, #32] 
    /*0000a7e4*/ MOVW        R2, #0                      @ .LC214 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2908] 
    /*0000a7e8*/ MOVT        R2, #0                      @ .LC214 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2908] 
    /*0000a7ec*/ STR         R2, [SP, #24] 
    /*0000a7f0*/ MOVW        R2, #0                      @ .LC260 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2cf4] 
    /*0000a7f4*/ MOVT        R2, #0                      @ .LC260 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2cf4] 
    /*0000a7f8*/ STR         R2, [SP, #164]              @ 0xa4 
    /*0000a7fc*/ MOV         R6, FP 
    /*0000a800*/ STR         R2, [SP, #12] 
    /*0000a804*/ MOVW        R2, #0                      @ .LC249 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ba0] 
    /*0000a808*/ MOVT        R2, #0                      @ .LC249 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ba0] 
    /*0000a80c*/ STR         R2, [SP, #148]              @ 0x94 
    /*0000a810*/ STR         R2, [SP, #4] 
    /*0000a814*/ MOV         R2, R3 
    /*0000a818*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*0000a81c*/ STR         R2, [SP] 
    /*0000a820*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*0000a824*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*0000a828*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*0000a82c*/ MOV         R7, SL 
    /*0000a830*/ BL          0x000000000000A830          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000a834*/ MOVW        R3, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*0000a838*/ MOVW        R2, #0                      @ .LC192 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2804] 
    /*0000a83c*/ MOVT        R3, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*0000a840*/ MOVT        R2, #0                      @ .LC192 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2804] 
    /*0000a844*/ STR         R3, [SP, #120]              @ 0x78 
    /*0000a848*/ STR         R2, [SP, #124]              @ 0x7c 
    /*0000a84c*/ MOVW        R3, #0                      @ .LC194 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x280c] 
    /*0000a850*/ STR         R8, [SP, #116]              @ 0x74 
    /*0000a854*/ MOVT        R3, #0                      @ .LC194 [R_ARM_MOVT_ABS: .rodata.str1.4+0x280c] 
    /*0000a858*/ STR         R3, [SP, #128]              @ 0x80 
    /*0000a85c*/ B           RGN_Proc_xa870
    RGN_Proc_xa860:
    /*0000a860*/ LDR         R3, [SP, #116]              @ 0x74 
    /*0000a864*/ ADD         R7, R7, #1 
    /*0000a868*/ CMP         R3, R6 
    /*0000a86c*/ BEQ         RGN_Proc_xaa28
    RGN_Proc_xa870:
    /*0000a870*/ LDR         SL, [R6, #4]! 
    /*0000a874*/ CMP         SL, #0 
    /*0000a878*/ BEQ         RGN_Proc_xa860
    /*0000a87c*/ LDR         R3, [SL, #176]              @ 0xb0 
    /*0000a880*/ CMP         R3, #1 
    /*0000a884*/ BNE         RGN_Proc_xa860
    /*0000a888*/ LDR         R5, [SL, #308]              @ 0x134 
    /*0000a88c*/ ADD         R3, SL, #308                @ 0x134 
    /*0000a890*/ STR         R3, [SP, #92]               @ 0x5c 
    /*0000a894*/ CMP         R3, R5 
    /*0000a898*/ BEQ         RGN_Proc_xa860
    /*0000a89c*/ ADD         R8, SP, #176                @ 0xb0 
    /*0000a8a0*/ STR         R6, [SP, #132]              @ 0x84 
    /*0000a8a4*/ LDR         R9, [R5] 
    /*0000a8a8*/ MOV         R6, R7 
    /*0000a8ac*/ MOV         R4, SL 
    /*0000a8b0*/ MOV         R7, R8 
    /*0000a8b4*/ B           RGN_Proc_xa8bc
    RGN_Proc_xa8b8:
    /*0000a8b8*/ MOV         R9, R3 
    RGN_Proc_xa8bc:
    /*0000a8bc*/ MOV         R1, R7 
    /*0000a8c0*/ MOV         R0, FP 
    /*0000a8c4*/ BL          0x000000000000A8C4          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000a8c8*/ MOV         R2, R6 
    /*0000a8cc*/ ADD         R1, R5, #8 
    /*0000a8d0*/ LDR         R0, [R4, #176]              @ 0xb0 
    /*0000a8d4*/ BL          0x000000000000A8D4          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*0000a8d8*/ SUBS        R3, R0, #0 
    /*0000a8dc*/ STR         R3, [SP, #88]               @ 0x58 
    /*0000a8e0*/ BEQ         RGN_Proc_xa9f0
    /*0000a8e4*/ LDR         R8, [SP, #88]               @ 0x58 
    /*0000a8e8*/ LDR         R3, [R8, #4] 
    /*0000a8ec*/ CMP         R3, #0 
    /*0000a8f0*/ BNE         RGN_Proc_xa9f0
    /*0000a8f4*/ LDR         R0, [R5, #8] 
    /*0000a8f8*/ BL          0x0000000000000000 
    /*0000a8fc*/ LDR         IP, [R8, #116]              @ 0x74 
    /*0000a900*/ STR         R8, [SP, #88]               @ 0x58 
    /*0000a904*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*0000a908*/ SUB         R3, IP, #1 
    /*0000a90c*/ LDR         IP, [SP, #124]              @ 0x7c 
    /*0000a910*/ LDR         SL, [R5, #16] 
    /*0000a914*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*0000a918*/ LDR         R2, [SP, #120]              @ 0x78 
    /*0000a91c*/ MOVW        LR, #0                      @ .LC193 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2808] 
    /*0000a920*/ MOVT        LR, #0                      @ .LC193 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2808] 
    /*0000a924*/ STR         R0, [SP, #96]               @ 0x60 
    /*0000a928*/ LDR         R0, [R8] 
    /*0000a92c*/ LDR         R8, [R5, #12] 
    /*0000a930*/ CMP         R0, #0 
    /*0000a934*/ LDR         R5, [SP, #88]               @ 0x58 
    /*0000a938*/ MOVEQ       R2, IP 
    /*0000a93c*/ ADD         IP, R1, R3, LSL #2 
    /*0000a940*/ MOV         R0, R2 
    /*0000a944*/ LDR         R2, [R5, #12] 
    /*0000a948*/ CMP         R3, #3 
    /*0000a94c*/ LDR         R1, [R5, #8] 
    /*0000a950*/ LDR         R3, [R4, #176]              @ 0xb0 
    /*0000a954*/ LDRLS       LR, [IP, #1368]             @ 0x558 
    /*0000a958*/ STR         R2, [SP, #100]              @ 0x64 
    /*0000a95c*/ LDR         IP, [SP, #88]               @ 0x58 
    /*0000a960*/ LDR         R2, [R5, #16] 
    /*0000a964*/ LDR         R5, [R5, #20] 
    /*0000a968*/ STR         R2, [SP, #104]              @ 0x68 
    /*0000a96c*/ LDR         R2, [IP, #68]               @ 0x44 
    /*0000a970*/ STR         R2, [SP, #108]              @ 0x6c 
    /*0000a974*/ LDR         R2, [IP, #160]              @ 0xa0 
    /*0000a978*/ MOVW        IP, #0                      @ .LC195 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2814] 
    /*0000a97c*/ MOVT        IP, #0                      @ .LC195 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2814] 
    /*0000a980*/ STR         R2, [SP, #112]              @ 0x70 
    /*0000a984*/ LDR         R2, [SP, #88]               @ 0x58 
    /*0000a988*/ LDR         R2, [R2, #64]               @ 0x40 
    /*0000a98c*/ STR         R1, [SP, #16] 
    /*0000a990*/ MOVW        R1, #0                      @ .LC261 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2cfc] 
    /*0000a994*/ STR         R0, [SP, #12] 
    /*0000a998*/ MOVT        R1, #0                      @ .LC261 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2cfc] 
    /*0000a99c*/ STR         R2, [SP, #40]               @ 0x28 
    /*0000a9a0*/ MOV         R2, R6 
    /*0000a9a4*/ STR         R8, [SP, #4] 
    /*0000a9a8*/ STR         SL, [SP, #8] 
    /*0000a9ac*/ LDR         R0, [SP, #84]               @ 0x54 
    /*0000a9b0*/ LDR         SL, [SP, #96]               @ 0x60 
    /*0000a9b4*/ LDR         R8, [SP, #112]              @ 0x70 
    /*0000a9b8*/ STR         LR, [SP, #32] 
    /*0000a9bc*/ STR         SL, [SP] 
    /*0000a9c0*/ CMP         R8, #1 
    /*0000a9c4*/ LDR         SL, [SP, #128]              @ 0x80 
    /*0000a9c8*/ STR         R5, [SP, #28] 
    /*0000a9cc*/ MOVEQ       IP, SL 
    /*0000a9d0*/ STR         IP, [SP, #44]               @ 0x2c 
    /*0000a9d4*/ LDR         IP, [SP, #108]              @ 0x6c 
    /*0000a9d8*/ STR         IP, [SP, #36]               @ 0x24 
    /*0000a9dc*/ LDR         IP, [SP, #104]              @ 0x68 
    /*0000a9e0*/ STR         IP, [SP, #24] 
    /*0000a9e4*/ LDR         IP, [SP, #100]              @ 0x64 
    /*0000a9e8*/ STR         IP, [SP, #20] 
    /*0000a9ec*/ BL          0x000000000000A9EC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    RGN_Proc_xa9f0:
    /*0000a9f0*/ MOV         R1, R7 
    /*0000a9f4*/ MOV         R0, FP 
    /*0000a9f8*/ BL          0x000000000000A9F8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000a9fc*/ LDR         R3, [SP, #92]               @ 0x5c 
    /*0000aa00*/ MOV         R5, R9 
    /*0000aa04*/ CMP         R3, R9 
    /*0000aa08*/ LDR         R3, [R9] 
    /*0000aa0c*/ BNE         RGN_Proc_xa8b8
    /*0000aa10*/ MOV         R7, R6 
    /*0000aa14*/ LDR         R3, [SP, #116]              @ 0x74 
    /*0000aa18*/ LDR         R6, [SP, #132]              @ 0x84 
    /*0000aa1c*/ ADD         R7, R7, #1 
    /*0000aa20*/ CMP         R3, R6 
    /*0000aa24*/ BNE         RGN_Proc_xa870
    RGN_Proc_xaa28:
    /*0000aa28*/ LDR         R4, [SP, #84]               @ 0x54 
    /*0000aa2c*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*0000aa30*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*0000aa34*/ LDR         R8, [SP, #116]              @ 0x74 
    /*0000aa38*/ LDR         SL, [PC, #-1560]            @ 0x000000000000a428 
    /*0000aa3c*/ MOV         FP, #0 
    /*0000aa40*/ MOV         R0, R4 
    /*0000aa44*/ MOV         R7, FP 
    /*0000aa48*/ BL          0x000000000000AA48          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000aa4c*/ MOVW        R1, #0                      @ .LC262 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2d28] 
    /*0000aa50*/ MOV         R0, R4 
    /*0000aa54*/ MOVT        R1, #0                      @ .LC262 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2d28] 
    /*0000aa58*/ BL          0x000000000000AA58          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000aa5c*/ LDR         R3, [SP, #164]              @ 0xa4 
    /*0000aa60*/ LDR         IP, [SP, #172]              @ 0xac 
    /*0000aa64*/ MOVW        R2, #0                      @ .LC266 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2dd8] 
    /*0000aa68*/ MOVW        R1, #0                      @ .LC263 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2d84] 
    /*0000aa6c*/ MOVT        R2, #0                      @ .LC266 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2dd8] 
    /*0000aa70*/ STR         R3, [SP, #12] 
    /*0000aa74*/ MOVT        R1, #0                      @ .LC263 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2d84] 
    /*0000aa78*/ LDR         R3, [SP, #144]              @ 0x90 
    /*0000aa7c*/ MOV         R0, R4 
    /*0000aa80*/ STR         IP, [SP, #72]               @ 0x48 
    /*0000aa84*/ MOVW        LR, #0                      @ .LC264 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2dc8] 
    /*0000aa88*/ LDR         IP, [SP, #140]              @ 0x8c 
    /*0000aa8c*/ MOVT        LR, #0                      @ .LC264 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2dc8] 
    /*0000aa90*/ STR         R3, [SP, #8] 
    /*0000aa94*/ MOV         R6, SL 
    /*0000aa98*/ LDR         R3, [SP, #148]              @ 0x94 
    /*0000aa9c*/ STR         IP, [SP, #68]               @ 0x44 
    /*0000aaa0*/ LDR         IP, [SP, #156]              @ 0x9c 
    /*0000aaa4*/ STR         R3, [SP, #4] 
    /*0000aaa8*/ LDR         R3, [SP, #152]              @ 0x98 
    /*0000aaac*/ STR         IP, [SP, #64]               @ 0x40 
    /*0000aab0*/ LDR         IP, [SP, #160]              @ 0xa0 
    /*0000aab4*/ STR         R3, [SP] 
    /*0000aab8*/ MOVW        R3, #0                      @ .LC267 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2de0] 
    /*0000aabc*/ STR         R2, [SP, #48]               @ 0x30 
    /*0000aac0*/ MOVT        R3, #0                      @ .LC267 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2de0] 
    /*0000aac4*/ MOVW        R2, #0                      @ .LC268 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2de4] 
    /*0000aac8*/ STR         R3, [SP, #44]               @ 0x2c 
    /*0000aacc*/ MOVT        R2, #0                      @ .LC268 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2de4] 
    /*0000aad0*/ MOVW        R3, #0                      @ .LC269 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2de8] 
    /*0000aad4*/ STR         R2, [SP, #40]               @ 0x28 
    /*0000aad8*/ MOVT        R3, #0                      @ .LC269 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2de8] 
    /*0000aadc*/ MOVW        R2, #0                      @ .LC270 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2dec] 
    /*0000aae0*/ STR         R3, [SP, #36]               @ 0x24 
    /*0000aae4*/ MOVT        R2, #0                      @ .LC270 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2dec] 
    /*0000aae8*/ MOVW        R3, #0                      @ .LC271 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2df0] 
    /*0000aaec*/ STR         R2, [SP, #32] 
    /*0000aaf0*/ MOVT        R3, #0                      @ .LC271 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2df0] 
    /*0000aaf4*/ MOVW        R2, #0                      @ .LC272 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2df4] 
    /*0000aaf8*/ STR         R3, [SP, #28] 
    /*0000aafc*/ MOVT        R2, #0                      @ .LC272 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2df4] 
    /*0000ab00*/ MOVW        R3, #0                      @ .LC273 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2df8] 
    /*0000ab04*/ STR         R2, [SP, #24] 
    /*0000ab08*/ MOVT        R3, #0                      @ .LC273 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2df8] 
    /*0000ab0c*/ MOVW        R2, #0                      @ .LC274 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2dfc] 
    /*0000ab10*/ STR         R3, [SP, #20] 
    /*0000ab14*/ MOVT        R2, #0                      @ .LC274 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2dfc] 
    /*0000ab18*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*0000ab1c*/ STR         R2, [SP, #16] 
    /*0000ab20*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*0000ab24*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*0000ab28*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*0000ab2c*/ STR         IP, [SP, #60]               @ 0x3c 
    /*0000ab30*/ MOVW        IP, #0                      @ .LC265 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2dd0] 
    /*0000ab34*/ STR         LR, [SP, #56]               @ 0x38 
    /*0000ab38*/ MOVT        IP, #0                      @ .LC265 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2dd0] 
    /*0000ab3c*/ STR         IP, [SP, #52]               @ 0x34 
    /*0000ab40*/ BL          0x000000000000AB40          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000ab44*/ MOVW        R3, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*0000ab48*/ MOVW        R2, #0                      @ .LC192 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2804] 
    /*0000ab4c*/ MOVT        R3, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*0000ab50*/ MOVT        R2, #0                      @ .LC192 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2804] 
    /*0000ab54*/ STR         R3, [SP, #148]              @ 0x94 
    /*0000ab58*/ MOVW        R3, #0                      @ .LC194 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x280c] 
    /*0000ab5c*/ STR         R2, [SP, #152]              @ 0x98 
    /*0000ab60*/ MOVT        R3, #0                      @ .LC194 [R_ARM_MOVT_ABS: .rodata.str1.4+0x280c] 
    /*0000ab64*/ STR         R8, [SP, #92]               @ 0x5c 
    /*0000ab68*/ STR         R3, [SP, #156]              @ 0x9c 
    /*0000ab6c*/ B           RGN_Proc_xab80
    RGN_Proc_xab70:
    /*0000ab70*/ LDR         R3, [SP, #92]               @ 0x5c 
    /*0000ab74*/ ADD         R7, R7, #1 
    /*0000ab78*/ CMP         R3, R6 
    /*0000ab7c*/ BEQ         RGN_Proc_x95c4
    RGN_Proc_xab80:
    /*0000ab80*/ LDR         R5, [R6, #4]! 
    /*0000ab84*/ CMP         R5, #0 
    /*0000ab88*/ BEQ         RGN_Proc_xab70
    /*0000ab8c*/ LDR         R3, [R5, #176]              @ 0xb0 
    /*0000ab90*/ CMP         R3, #1 
    /*0000ab94*/ BNE         RGN_Proc_xab70
    /*0000ab98*/ LDR         R4, [R5, #308]              @ 0x134 
    /*0000ab9c*/ ADD         FP, R5, #308                @ 0x134 
    /*0000aba0*/ CMP         R4, FP 
    /*0000aba4*/ BEQ         RGN_Proc_xab70
    /*0000aba8*/ STR         R6, [SP, #160]              @ 0xa0 
    /*0000abac*/ ADD         R9, SP, #176                @ 0xb0 
    /*0000abb0*/ LDR         R8, [R4] 
    /*0000abb4*/ MOV         R6, FP 
    /*0000abb8*/ B           RGN_Proc_xabdc
    RGN_Proc_xabbc:
    /*0000abbc*/ MOV         R1, R9 
    /*0000abc0*/ MOV         R0, SL 
    /*0000abc4*/ BL          0x000000000000ABC4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000abc8*/ CMP         R6, R8 
    /*0000abcc*/ LDR         R3, [R8] 
    /*0000abd0*/ MOV         R4, R8 
    /*0000abd4*/ BEQ         RGN_Proc_xb458
    /*0000abd8*/ MOV         R8, R3 
    RGN_Proc_xabdc:
    /*0000abdc*/ MOV         R1, R9 
    /*0000abe0*/ MOV         R0, SL 
    /*0000abe4*/ BL          0x000000000000ABE4          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000abe8*/ MOV         R2, R7 
    /*0000abec*/ ADD         R1, R4, #8 
    /*0000abf0*/ LDR         R0, [R5, #176]              @ 0xb0 
    /*0000abf4*/ BL          0x000000000000ABF4          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*0000abf8*/ SUBS        R3, R0, #0 
    /*0000abfc*/ STR         R3, [SP, #88]               @ 0x58 
    /*0000ac00*/ BEQ         RGN_Proc_xabbc
    /*0000ac04*/ LDR         R3, [SP, #88]               @ 0x58 
    /*0000ac08*/ LDR         R3, [R3, #4] 
    /*0000ac0c*/ CMP         R3, #1 
    /*0000ac10*/ BNE         RGN_Proc_xabbc
    /*0000ac14*/ LDR         R0, [R4, #8] 
    /*0000ac18*/ BL          0x0000000000000000 
    /*0000ac1c*/ LDR         R2, [SP, #88]               @ 0x58 
    /*0000ac20*/ LDR         LR, [SP, #152]              @ 0x98 
    /*0000ac24*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*0000ac28*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*0000ac2c*/ LDR         IP, [R2, #24] 
    /*0000ac30*/ LDR         R3, [R2, #116]              @ 0x74 
    /*0000ac34*/ SUB         R3, R3, #1 
    /*0000ac38*/ ADD         R1, R1, R3, LSL #2 
    /*0000ac3c*/ STR         R0, [SP, #104]              @ 0x68 
    /*0000ac40*/ LDR         R0, [R2] 
    /*0000ac44*/ LDR         R2, [SP, #148]              @ 0x94 
    /*0000ac48*/ CMP         R0, #0 
    /*0000ac4c*/ MOVW        R0, #0                      @ .LC193 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2808] 
    /*0000ac50*/ MOVT        R0, #0                      @ .LC193 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2808] 
    /*0000ac54*/ MOV         FP, R2 
    /*0000ac58*/ MOVEQ       FP, LR 
    /*0000ac5c*/ CMP         IP, #0 
    /*0000ac60*/ LDR         IP, [SP, #88]               @ 0x58 
    /*0000ac64*/ STR         FP, [SP, #96]               @ 0x60 
    /*0000ac68*/ MOVEQ       R2, LR 
    /*0000ac6c*/ STR         R2, [SP, #100]              @ 0x64 
    /*0000ac70*/ LDR         R2, [R4, #12] 
    /*0000ac74*/ CMP         R3, #3 
    /*0000ac78*/ LDR         FP, [IP, #56]               @ 0x38 
    /*0000ac7c*/ LDR         R3, [R5, #176]              @ 0xb0 
    /*0000ac80*/ STR         R2, [SP, #108]              @ 0x6c 
    /*0000ac84*/ LDR         R2, [R4, #16] 
    /*0000ac88*/ STR         R2, [SP, #112]              @ 0x70 
    /*0000ac8c*/ LDR         R2, [IP, #32] 
    /*0000ac90*/ STR         R2, [SP, #116]              @ 0x74 
    /*0000ac94*/ LDR         R2, [IP, #36]               @ 0x24 
    /*0000ac98*/ STR         R2, [SP, #120]              @ 0x78 
    /*0000ac9c*/ LDR         R2, [IP, #40]               @ 0x28 
    /*0000aca0*/ STR         R2, [SP, #124]              @ 0x7c 
    /*0000aca4*/ LDR         R2, [IP, #44]               @ 0x2c 
    /*0000aca8*/ STR         R2, [SP, #128]              @ 0x80 
    /*0000acac*/ LDR         R2, [IP, #48]               @ 0x30 
    /*0000acb0*/ STR         R2, [SP, #132]              @ 0x84 
    /*0000acb4*/ LDR         R2, [IP, #52]               @ 0x34 
    /*0000acb8*/ STR         R2, [SP, #140]              @ 0x8c 
    /*0000acbc*/ LDR         R2, [IP, #60]               @ 0x3c 
    /*0000acc0*/ STR         R2, [SP, #144]              @ 0x90 
    /*0000acc4*/ LDR         LR, [IP, #28] 
    /*0000acc8*/ LDR         IP, [SP, #88]               @ 0x58 
    /*0000accc*/ LDRLS       R0, [R1, #1368]             @ 0x558 
    /*0000acd0*/ MOVW        R1, #0                      @ .LC195 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2814] 
    /*0000acd4*/ MOVT        R1, #0                      @ .LC195 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2814] 
    /*0000acd8*/ LDR         R2, [IP, #160]              @ 0xa0 
    /*0000acdc*/ LDR         R4, [IP, #68]               @ 0x44 
    /*0000ace0*/ LDR         IP, [IP, #64]               @ 0x40 
    /*0000ace4*/ CMP         R2, #1 
    /*0000ace8*/ STR         R0, [SP, #60]               @ 0x3c 
    /*0000acec*/ MOV         R2, R7 
    /*0000acf0*/ LDR         R0, [SP, #100]              @ 0x64 
    /*0000acf4*/ STR         IP, [SP, #68]               @ 0x44 
    /*0000acf8*/ LDR         IP, [SP, #156]              @ 0x9c 
    /*0000acfc*/ STR         R4, [SP, #64]               @ 0x40 
    /*0000ad00*/ MOV         R4, #1 
    /*0000ad04*/ STR         R0, [SP, #48]               @ 0x30 
    /*0000ad08*/ MOVEQ       R1, IP 
    /*0000ad0c*/ LDR         IP, [SP, #144]              @ 0x90 
    /*0000ad10*/ STR         R1, [SP, #72]               @ 0x48 
    /*0000ad14*/ MOVW        R1, #0                      @ .LC275 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2e00] 
    /*0000ad18*/ STR         LR, [SP, #52]               @ 0x34 
    /*0000ad1c*/ MOVT        R1, #0                      @ .LC275 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2e00] 
    /*0000ad20*/ STR         IP, [SP, #44]               @ 0x2c 
    /*0000ad24*/ LDR         IP, [SP, #140]              @ 0x8c 
    /*0000ad28*/ LDR         R0, [SP, #84]               @ 0x54 
    /*0000ad2c*/ STR         FP, [SP, #40]               @ 0x28 
    /*0000ad30*/ STR         IP, [SP, #36]               @ 0x24 
    /*0000ad34*/ LDR         IP, [SP, #132]              @ 0x84 
    /*0000ad38*/ STR         R4, [SP, #56]               @ 0x38 
    /*0000ad3c*/ STR         IP, [SP, #32] 
    /*0000ad40*/ LDR         IP, [SP, #128]              @ 0x80 
    /*0000ad44*/ STR         IP, [SP, #28] 
    /*0000ad48*/ LDR         IP, [SP, #124]              @ 0x7c 
    /*0000ad4c*/ STR         IP, [SP, #24] 
    /*0000ad50*/ LDR         IP, [SP, #120]              @ 0x78 
    /*0000ad54*/ STR         IP, [SP, #20] 
    /*0000ad58*/ LDR         IP, [SP, #116]              @ 0x74 
    /*0000ad5c*/ STR         IP, [SP, #16] 
    /*0000ad60*/ LDR         IP, [SP, #96]               @ 0x60 
    /*0000ad64*/ STR         IP, [SP, #12] 
    /*0000ad68*/ LDR         IP, [SP, #112]              @ 0x70 
    /*0000ad6c*/ STR         IP, [SP, #8] 
    /*0000ad70*/ LDR         IP, [SP, #108]              @ 0x6c 
    /*0000ad74*/ STR         IP, [SP, #4] 
    /*0000ad78*/ LDR         IP, [SP, #104]              @ 0x68 
    /*0000ad7c*/ STR         IP, [SP] 
    /*0000ad80*/ BL          0x000000000000AD80          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000ad84*/ B           RGN_Proc_xabbc
    RGN_Proc_xad88:
    /*0000ad88*/ LDR         R9, [SP, #84]               @ 0x54 
    /*0000ad8c*/ MOVW        R3, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*0000ad90*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*0000ad94*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*0000ad98*/ MOV         R4, R3 
    /*0000ad9c*/ MOVW        R6, #0                      @ .LC192 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2804] 
    /*0000ada0*/ MOV         R0, R9 
    /*0000ada4*/ MOVW        R5, #0                      @ .LC254 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c44] 
    /*0000ada8*/ BL          0x000000000000ADA8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000adac*/ MOVW        R1, #0                      @ .LC276 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2e44] 
    /*0000adb0*/ MOV         R0, R9 
    /*0000adb4*/ MOVT        R1, #0                      @ .LC276 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2e44] 
    /*0000adb8*/ BL          0x000000000000ADB8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000adbc*/ MOV         IP, R4 
    /*0000adc0*/ MOVT        IP, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*0000adc4*/ MOVW        R2, #0                      @ .LC216 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2914] 
    /*0000adc8*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*0000adcc*/ MOVT        R2, #0                      @ .LC216 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2914] 
    /*0000add0*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*0000add4*/ STR         R2, [SP] 
    /*0000add8*/ MOVW        R1, #0                      @ .LC253 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2c38] 
    /*0000addc*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*0000ade0*/ MOVT        R1, #0                      @ .LC253 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c38] 
    /*0000ade4*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*0000ade8*/ MOV         R0, R9 
    /*0000adec*/ MOV         R7, IP 
    /*0000adf0*/ STR         IP, [SP, #96]               @ 0x60 
    /*0000adf4*/ MOVT        R6, #0                      @ .LC192 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2804] 
    /*0000adf8*/ BL          0x000000000000ADF8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000adfc*/ MOVT        R5, #0                      @ .LC254 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2c44] 
    /*0000ae00*/ LDR         FP, [PC, #-2528]            @ 0x000000000000a428 
    /*0000ae04*/ MOV         R4, #0 
    /*0000ae08*/ LDR         R8, [SP, #136]              @ 0x88 
    /*0000ae0c*/ B           RGN_Proc_xae1c
    RGN_Proc_xae10:
    /*0000ae10*/ CMP         R8, FP 
    /*0000ae14*/ ADD         R4, R4, #1 
    /*0000ae18*/ BEQ         RGN_Proc_xae64
    RGN_Proc_xae1c:
    /*0000ae1c*/ LDR         R2, [FP, #4]! 
    /*0000ae20*/ CMP         R2, #0 
    /*0000ae24*/ BEQ         RGN_Proc_xae10
    /*0000ae28*/ LDR         R3, [R2, #176]              @ 0xb0 
    /*0000ae2c*/ CMP         R3, #2 
    /*0000ae30*/ BNE         RGN_Proc_xae10
    /*0000ae34*/ LDR         R0, [R2, #180]              @ 0xb4 
    /*0000ae38*/ MOV         R1, R5 
    /*0000ae3c*/ MOV         R2, R4 
    /*0000ae40*/ ADD         R4, R4, #1 
    /*0000ae44*/ CMP         R0, #0 
    /*0000ae48*/ MOVNE       R0, R7 
    /*0000ae4c*/ MOVEQ       R0, R6 
    /*0000ae50*/ STR         R0, [SP] 
    /*0000ae54*/ MOV         R0, R9 
    /*0000ae58*/ BL          0x000000000000AE58          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000ae5c*/ CMP         R8, FP 
    /*0000ae60*/ BNE         RGN_Proc_xae1c
    RGN_Proc_xae64:
    /*0000ae64*/ LDR         R5, [SP, #84]               @ 0x54 
    /*0000ae68*/ MOVW        R3, #0                      @ .LC241 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b68] 
    /*0000ae6c*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*0000ae70*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*0000ae74*/ MOV         R4, R3 
    /*0000ae78*/ LDR         R9, [PC, #-2648]            @ 0x000000000000a428 
    /*0000ae7c*/ MOV         R0, R5 
    /*0000ae80*/ MOV         SL, #0 
    /*0000ae84*/ BL          0x000000000000AE84          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000ae88*/ MOVW        R1, #0                      @ .LC277 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ea0] 
    /*0000ae8c*/ MOV         R0, R5 
    /*0000ae90*/ MOVT        R1, #0                      @ .LC277 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ea0] 
    /*0000ae94*/ BL          0x000000000000AE94          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000ae98*/ LDR         R2, [SP, #96]               @ 0x60 
    /*0000ae9c*/ MOV         R3, R4 
    /*0000aea0*/ MOVT        R3, #0                      @ .LC241 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b68] 
    /*0000aea4*/ STR         R3, [SP, #140]              @ 0x8c 
    /*0000aea8*/ MOVW        R1, #0                      @ .LC278 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2efc] 
    /*0000aeac*/ STR         R3, [SP, #40]               @ 0x28 
    /*0000aeb0*/ MOVW        R3, #0                      @ .LC259 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2cec] 
    /*0000aeb4*/ MOVT        R3, #0                      @ .LC259 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2cec] 
    /*0000aeb8*/ STR         R2, [SP, #20] 
    /*0000aebc*/ STR         R3, [SP, #160]              @ 0xa0 
    /*0000aec0*/ MOVW        R2, #0                      @ .LC258 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ce4] 
    /*0000aec4*/ MOVT        R2, #0                      @ .LC258 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ce4] 
    /*0000aec8*/ STR         R2, [SP, #156]              @ 0x9c 
    /*0000aecc*/ STR         R2, [SP, #36]               @ 0x24 
    /*0000aed0*/ MOV         R2, R3 
    /*0000aed4*/ MOVW        R3, #0                      @ .LC213 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2904] 
    /*0000aed8*/ MOVT        R3, #0                      @ .LC213 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2904] 
    /*0000aedc*/ STR         R3, [SP, #28] 
    /*0000aee0*/ MOVW        R3, #0                      @ .LC244 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b80] 
    /*0000aee4*/ MOVT        R3, #0                      @ .LC244 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b80] 
    /*0000aee8*/ STR         R3, [SP, #16] 
    /*0000aeec*/ MOVW        R3, #0                      @ .LC248 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2b9c] 
    /*0000aef0*/ MOVT        R3, #0                      @ .LC248 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2b9c] 
    /*0000aef4*/ STR         R3, [SP, #144]              @ 0x90 
    /*0000aef8*/ MOVT        R1, #0                      @ .LC278 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2efc] 
    /*0000aefc*/ STR         R3, [SP, #8] 
    /*0000af00*/ MOVW        R3, #0                      @ .LC250 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ba4] 
    /*0000af04*/ MOVT        R3, #0                      @ .LC250 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ba4] 
    /*0000af08*/ STR         R2, [SP, #32] 
    /*0000af0c*/ STR         R3, [SP, #152]              @ 0x98 
    /*0000af10*/ MOVW        R2, #0                      @ .LC214 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2908] 
    /*0000af14*/ MOV         R0, R5 
    /*0000af18*/ MOVT        R2, #0                      @ .LC214 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2908] 
    /*0000af1c*/ STR         R2, [SP, #24] 
    /*0000af20*/ MOVW        R2, #0                      @ .LC260 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2cf4] 
    /*0000af24*/ MOVT        R2, #0                      @ .LC260 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2cf4] 
    /*0000af28*/ STR         R2, [SP, #164]              @ 0xa4 
    /*0000af2c*/ STR         R2, [SP, #12] 
    /*0000af30*/ MOVW        R2, #0                      @ .LC249 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ba0] 
    /*0000af34*/ MOVT        R2, #0                      @ .LC249 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ba0] 
    /*0000af38*/ STR         R2, [SP, #148]              @ 0x94 
    /*0000af3c*/ STR         R2, [SP, #4] 
    /*0000af40*/ MOV         R2, R3 
    /*0000af44*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*0000af48*/ STR         R2, [SP] 
    /*0000af4c*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*0000af50*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*0000af54*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*0000af58*/ MOV         R6, SL 
    /*0000af5c*/ BL          0x000000000000AF5C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000af60*/ MOV         R7, R9 
    /*0000af64*/ MOVW        R3, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*0000af68*/ MOVW        R2, #0                      @ .LC192 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2804] 
    /*0000af6c*/ MOVT        R3, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*0000af70*/ MOVT        R2, #0                      @ .LC192 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2804] 
    /*0000af74*/ STR         R3, [SP, #116]              @ 0x74 
    /*0000af78*/ MOVW        R3, #0                      @ .LC279 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2f28] 
    /*0000af7c*/ STR         R2, [SP, #120]              @ 0x78 
    /*0000af80*/ MOVT        R3, #0                      @ .LC279 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2f28] 
    /*0000af84*/ STR         FP, [SP, #112]              @ 0x70 
    /*0000af88*/ STR         R3, [SP, #124]              @ 0x7c 
    /*0000af8c*/ B           RGN_Proc_xafa0
    RGN_Proc_xaf90:
    /*0000af90*/ LDR         R3, [SP, #112]              @ 0x70 
    /*0000af94*/ ADD         R6, R6, #1 
    /*0000af98*/ CMP         R3, R7 
    /*0000af9c*/ BEQ         RGN_Proc_xb118
    RGN_Proc_xafa0:
    /*0000afa0*/ LDR         FP, [R7, #4]! 
    /*0000afa4*/ CMP         FP, #0 
    /*0000afa8*/ BEQ         RGN_Proc_xaf90
    /*0000afac*/ LDR         R3, [FP, #176]              @ 0xb0 
    /*0000afb0*/ CMP         R3, #2 
    /*0000afb4*/ BNE         RGN_Proc_xaf90
    /*0000afb8*/ LDR         R5, [FP, #308]              @ 0x134 
    /*0000afbc*/ ADD         R3, FP, #308                @ 0x134 
    /*0000afc0*/ STR         R3, [SP, #88]               @ 0x58 
    /*0000afc4*/ CMP         R3, R5 
    /*0000afc8*/ BEQ         RGN_Proc_xaf90
    /*0000afcc*/ STR         R7, [SP, #128]              @ 0x80 
    /*0000afd0*/ ADD         R8, SP, #176                @ 0xb0 
    /*0000afd4*/ MOV         R7, R6 
    /*0000afd8*/ LDR         SL, [R5] 
    /*0000afdc*/ MOV         R6, FP 
    /*0000afe0*/ B           RGN_Proc_xafe8
    RGN_Proc_xafe4:
    /*0000afe4*/ MOV         SL, R3 
    RGN_Proc_xafe8:
    /*0000afe8*/ MOV         R1, R8 
    /*0000afec*/ MOV         R0, R9 
    /*0000aff0*/ BL          0x000000000000AFF0          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000aff4*/ MOV         R2, R7 
    /*0000aff8*/ ADD         R1, R5, #8 
    /*0000affc*/ LDR         R0, [R6, #176]              @ 0xb0 
    /*0000b000*/ BL          0x000000000000B000          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*0000b004*/ SUBS        R4, R0, #0 
    /*0000b008*/ BEQ         RGN_Proc_xb0e0
    /*0000b00c*/ LDR         R3, [R4, #4] 
    /*0000b010*/ CMP         R3, #0 
    /*0000b014*/ BNE         RGN_Proc_xb0e0
    /*0000b018*/ LDR         R0, [R5, #8] 
    /*0000b01c*/ BL          0x0000000000000000 
    /*0000b020*/ LDR         R1, [R4] 
    /*0000b024*/ LDR         LR, [SP, #120]              @ 0x78 
    /*0000b028*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*0000b02c*/ CMP         R1, #0 
    /*0000b030*/ LDR         R1, [R5, #12] 
    /*0000b034*/ LDR         R2, [R4, #116]              @ 0x74 
    /*0000b038*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*0000b03c*/ LDR         FP, [R4, #12] 
    /*0000b040*/ MOVW        IP, #0                      @ .LC193 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2808] 
    /*0000b044*/ STR         R1, [SP, #96]               @ 0x60 
    /*0000b048*/ SUB         R2, R2, #1 
    /*0000b04c*/ LDR         R1, [R4, #8] 
    /*0000b050*/ MOVT        IP, #0                      @ .LC193 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2808] 
    /*0000b054*/ LDR         R5, [R5, #16] 
    /*0000b058*/ STR         R1, [SP, #100]              @ 0x64 
    /*0000b05c*/ LDR         R1, [R4, #16] 
    /*0000b060*/ STR         R1, [SP, #104]              @ 0x68 
    /*0000b064*/ LDR         R1, [SP, #124]              @ 0x7c 
    /*0000b068*/ STR         R0, [SP, #92]               @ 0x5c 
    /*0000b06c*/ LDR         R0, [SP, #116]              @ 0x74 
    /*0000b070*/ MOVEQ       R0, LR 
    /*0000b074*/ LDR         LR, [R4, #20] 
    /*0000b078*/ CMP         R2, #3 
    /*0000b07c*/ ADD         R2, R3, R2, LSL #2 
    /*0000b080*/ LDR         R3, [R6, #176]              @ 0xb0 
    /*0000b084*/ STR         R0, [SP, #108]              @ 0x6c 
    /*0000b088*/ LDR         R0, [R4, #64]               @ 0x40 
    /*0000b08c*/ LDRLS       IP, [R2, #1368]             @ 0x558 
    /*0000b090*/ MOV         R2, R7 
    /*0000b094*/ STR         R0, [SP, #40]               @ 0x28 
    /*0000b098*/ LDR         R4, [R4, #68]               @ 0x44 
    /*0000b09c*/ STR         FP, [SP, #20] 
    /*0000b0a0*/ LDR         FP, [SP, #100]              @ 0x64 
    /*0000b0a4*/ LDR         R0, [SP, #104]              @ 0x68 
    /*0000b0a8*/ STR         R5, [SP, #8] 
    /*0000b0ac*/ STR         FP, [SP, #16] 
    /*0000b0b0*/ LDR         FP, [SP, #108]              @ 0x6c 
    /*0000b0b4*/ STR         R0, [SP, #24] 
    /*0000b0b8*/ LDR         R0, [SP, #84]               @ 0x54 
    /*0000b0bc*/ STR         FP, [SP, #12] 
    /*0000b0c0*/ LDR         R5, [SP, #96]               @ 0x60 
    /*0000b0c4*/ STR         R4, [SP, #36]               @ 0x24 
    /*0000b0c8*/ STR         IP, [SP, #32] 
    /*0000b0cc*/ STR         R5, [SP, #4] 
    /*0000b0d0*/ LDR         R5, [SP, #92]               @ 0x5c 
    /*0000b0d4*/ STR         LR, [SP, #28] 
    /*0000b0d8*/ STR         R5, [SP] 
    /*0000b0dc*/ BL          0x000000000000B0DC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    RGN_Proc_xb0e0:
    /*0000b0e0*/ MOV         R1, R8 
    /*0000b0e4*/ MOV         R0, R9 
    /*0000b0e8*/ BL          0x000000000000B0E8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000b0ec*/ LDR         R3, [SP, #88]               @ 0x58 
    /*0000b0f0*/ MOV         R5, SL 
    /*0000b0f4*/ CMP         R3, SL 
    /*0000b0f8*/ LDR         R3, [SL] 
    /*0000b0fc*/ BNE         RGN_Proc_xafe4
    /*0000b100*/ MOV         R6, R7 
    /*0000b104*/ LDR         R3, [SP, #112]              @ 0x70 
    /*0000b108*/ LDR         R7, [SP, #128]              @ 0x80 
    /*0000b10c*/ ADD         R6, R6, #1 
    /*0000b110*/ CMP         R3, R7 
    /*0000b114*/ BNE         RGN_Proc_xafa0
    RGN_Proc_xb118:
    /*0000b118*/ LDR         R4, [SP, #84]               @ 0x54 
    /*0000b11c*/ MOVW        R1, #0                      @ .LC204 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*0000b120*/ MOVT        R1, #0                      @ .LC204 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*0000b124*/ LDR         FP, [SP, #112]              @ 0x70 
    /*0000b128*/ LDR         R9, [PC, #-3336]            @ 0x000000000000a428 
    /*0000b12c*/ MOV         SL, #0 
    /*0000b130*/ MOV         R0, R4 
    /*0000b134*/ MOV         R5, SL 
    /*0000b138*/ BL          0x000000000000B138          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000b13c*/ MOV         R0, R4 
    /*0000b140*/ MOVW        R1, #0                      @ .LC280 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2f54] 
    /*0000b144*/ MOVT        R1, #0                      @ .LC280 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2f54] 
    /*0000b148*/ BL          0x000000000000B148          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000b14c*/ LDR         LR, [SP, #156]              @ 0x9c 
    /*0000b150*/ LDR         R3, [SP, #164]              @ 0xa4 
    /*0000b154*/ MOV         R0, R4 
    /*0000b158*/ LDR         R2, [SP, #148]              @ 0x94 
    /*0000b15c*/ MOVW        R1, #0                      @ .LC281 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2fb0] 
    /*0000b160*/ STR         LR, [SP, #64]               @ 0x40 
    /*0000b164*/ MOVT        R1, #0                      @ .LC281 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2fb0] 
    /*0000b168*/ LDR         LR, [SP, #160]              @ 0xa0 
    /*0000b16c*/ MOV         R7, R9 
    /*0000b170*/ STR         R3, [SP, #12] 
    /*0000b174*/ STR         R2, [SP, #4] 
    /*0000b178*/ LDR         R3, [SP, #144]              @ 0x90 
    /*0000b17c*/ LDR         R2, [SP, #152]              @ 0x98 
    /*0000b180*/ LDR         IP, [SP, #140]              @ 0x8c 
    /*0000b184*/ STR         LR, [SP, #60]               @ 0x3c 
    /*0000b188*/ MOVW        LR, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*0000b18c*/ STR         R3, [SP, #8] 
    /*0000b190*/ MOV         R4, LR 
    /*0000b194*/ STR         R2, [SP] 
    /*0000b198*/ MOVW        R3, #0                      @ .LC265 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2dd0] 
    /*0000b19c*/ MOVW        R2, #0                      @ .LC266 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2dd8] 
    /*0000b1a0*/ MOVT        R3, #0                      @ .LC265 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2dd0] 
    /*0000b1a4*/ MOVT        R2, #0                      @ .LC266 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2dd8] 
    /*0000b1a8*/ STR         R3, [SP, #52]               @ 0x34 
    /*0000b1ac*/ STR         R2, [SP, #48]               @ 0x30 
    /*0000b1b0*/ MOVW        R3, #0                      @ .LC267 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2de0] 
    /*0000b1b4*/ MOVW        R2, #0                      @ .LC268 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2de4] 
    /*0000b1b8*/ MOVT        R3, #0                      @ .LC267 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2de0] 
    /*0000b1bc*/ MOVT        R2, #0                      @ .LC268 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2de4] 
    /*0000b1c0*/ STR         R3, [SP, #44]               @ 0x2c 
    /*0000b1c4*/ STR         R2, [SP, #40]               @ 0x28 
    /*0000b1c8*/ MOVW        R3, #0                      @ .LC269 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2de8] 
    /*0000b1cc*/ MOVW        R2, #0                      @ .LC270 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2dec] 
    /*0000b1d0*/ MOVT        R3, #0                      @ .LC269 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2de8] 
    /*0000b1d4*/ MOVT        R2, #0                      @ .LC270 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2dec] 
    /*0000b1d8*/ STR         R3, [SP, #36]               @ 0x24 
    /*0000b1dc*/ STR         R2, [SP, #32] 
    /*0000b1e0*/ MOVW        R3, #0                      @ .LC271 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2df0] 
    /*0000b1e4*/ MOVW        R2, #0                      @ .LC272 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2df4] 
    /*0000b1e8*/ MOVT        R3, #0                      @ .LC271 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2df0] 
    /*0000b1ec*/ MOVT        R2, #0                      @ .LC272 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2df4] 
    /*0000b1f0*/ STR         R3, [SP, #28] 
    /*0000b1f4*/ STR         R2, [SP, #24] 
    /*0000b1f8*/ MOVW        R3, #0                      @ .LC273 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2df8] 
    /*0000b1fc*/ MOVW        R2, #0                      @ .LC274 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2dfc] 
    /*0000b200*/ MOVT        R3, #0                      @ .LC273 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2df8] 
    /*0000b204*/ MOVT        R2, #0                      @ .LC274 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2dfc] 
    /*0000b208*/ STR         R3, [SP, #20] 
    /*0000b20c*/ STR         R2, [SP, #16] 
    /*0000b210*/ MOVW        R3, #0                      @ .LC206 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*0000b214*/ MOVW        R2, #0                      @ .LC207 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28c0] 
    /*0000b218*/ MOVT        R3, #0                      @ .LC206 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*0000b21c*/ MOVT        R2, #0                      @ .LC207 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28c0] 
    /*0000b220*/ STR         IP, [SP, #68]               @ 0x44 
    /*0000b224*/ MOVW        IP, #0                      @ .LC264 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2dc8] 
    /*0000b228*/ MOVT        IP, #0                      @ .LC264 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2dc8] 
    /*0000b22c*/ STR         IP, [SP, #56]               @ 0x38 
    /*0000b230*/ BL          0x000000000000B230          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000b234*/ MOV         LR, R4 
    /*0000b238*/ MOVW        R3, #0                      @ .LC192 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2804] 
    /*0000b23c*/ MOVT        LR, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*0000b240*/ MOVW        R2, #0                      @ .LC282 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ff0] 
    /*0000b244*/ MOVT        R3, #0                      @ .LC192 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2804] 
    /*0000b248*/ MOVT        R2, #0                      @ .LC282 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ff0] 
    /*0000b24c*/ STR         LR, [SP, #140]              @ 0x8c 
    /*0000b250*/ STR         R3, [SP, #144]              @ 0x90 
    /*0000b254*/ STR         R2, [SP, #148]              @ 0x94 
    /*0000b258*/ STR         FP, [SP, #92]               @ 0x5c 
    /*0000b25c*/ B           RGN_Proc_xb270
    RGN_Proc_xb260:
    /*0000b260*/ LDR         R3, [SP, #92]               @ 0x5c 
    /*0000b264*/ ADD         R5, R5, #1 
    /*0000b268*/ CMP         R3, R7 
    /*0000b26c*/ BEQ         RGN_Proc_x95d0
    RGN_Proc_xb270:
    /*0000b270*/ LDR         R6, [R7, #4]! 
    /*0000b274*/ CMP         R6, #0 
    /*0000b278*/ BEQ         RGN_Proc_xb260
    /*0000b27c*/ LDR         R3, [R6, #176]              @ 0xb0 
    /*0000b280*/ CMP         R3, #2 
    /*0000b284*/ BNE         RGN_Proc_xb260
    /*0000b288*/ LDR         R4, [R6, #308]              @ 0x134 
    /*0000b28c*/ ADD         FP, R6, #308                @ 0x134 
    /*0000b290*/ CMP         FP, R4 
    /*0000b294*/ BEQ         RGN_Proc_xb260
    /*0000b298*/ STR         R7, [SP, #152]              @ 0x98 
    /*0000b29c*/ ADD         R8, SP, #176                @ 0xb0 
    /*0000b2a0*/ LDR         SL, [R4] 
    /*0000b2a4*/ MOV         R7, FP 
    /*0000b2a8*/ B           RGN_Proc_xb2cc
    RGN_Proc_xb2ac:
    /*0000b2ac*/ MOV         R1, R8 
    /*0000b2b0*/ MOV         R0, R9 
    /*0000b2b4*/ BL          0x000000000000B2B4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000b2b8*/ CMP         R7, SL 
    /*0000b2bc*/ LDR         R3, [SL] 
    /*0000b2c0*/ MOV         R4, SL 
    /*0000b2c4*/ BEQ         RGN_Proc_xb450
    /*0000b2c8*/ MOV         SL, R3 
    RGN_Proc_xb2cc:
    /*0000b2cc*/ MOV         R1, R8 
    /*0000b2d0*/ MOV         R0, R9 
    /*0000b2d4*/ BL          0x000000000000B2D4          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000b2d8*/ MOV         R2, R5 
    /*0000b2dc*/ ADD         R1, R4, #8 
    /*0000b2e0*/ LDR         R0, [R6, #176]              @ 0xb0 
    /*0000b2e4*/ BL          0x000000000000B2E4          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*0000b2e8*/ SUBS        R3, R0, #0 
    /*0000b2ec*/ STR         R3, [SP, #88]               @ 0x58 
    /*0000b2f0*/ BEQ         RGN_Proc_xb2ac
    /*0000b2f4*/ LDR         R3, [SP, #88]               @ 0x58 
    /*0000b2f8*/ LDR         R3, [R3, #4] 
    /*0000b2fc*/ CMP         R3, #1 
    /*0000b300*/ BNE         RGN_Proc_xb2ac
    /*0000b304*/ LDR         R0, [R4, #8] 
    /*0000b308*/ BL          0x0000000000000000 
    /*0000b30c*/ LDR         R1, [SP, #88]               @ 0x58 
    /*0000b310*/ LDR         LR, [SP, #144]              @ 0x90 
    /*0000b314*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*0000b318*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*0000b31c*/ LDR         IP, [R1] 
    /*0000b320*/ LDR         R2, [R1, #116]              @ 0x74 
    /*0000b324*/ CMP         IP, #0 
    /*0000b328*/ SUB         R2, R2, #1 
    /*0000b32c*/ STR         R0, [SP, #96]               @ 0x60 
    /*0000b330*/ LDR         R0, [R1, #24] 
    /*0000b334*/ LDR         R1, [SP, #140]              @ 0x8c 
    /*0000b338*/ MOV         FP, R1 
    /*0000b33c*/ MOVEQ       FP, LR 
    /*0000b340*/ CMP         R0, #0 
    /*0000b344*/ LDR         R0, [SP, #88]               @ 0x58 
    /*0000b348*/ MOV         IP, FP 
    /*0000b34c*/ MOVW        FP, #0                      @ .LC193 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2808] 
    /*0000b350*/ MOVEQ       R1, LR 
    /*0000b354*/ STR         R1, [SP, #132]              @ 0x84 
    /*0000b358*/ LDR         R1, [R4, #12] 
    /*0000b35c*/ CMP         R2, #3 
    /*0000b360*/ ADD         R2, R3, R2, LSL #2 
    /*0000b364*/ LDR         LR, [R0, #52]               @ 0x34 
    /*0000b368*/ MOVT        FP, #0                      @ .LC193 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2808] 
    /*0000b36c*/ LDR         R3, [R6, #176]              @ 0xb0 
    /*0000b370*/ STR         R1, [SP, #100]              @ 0x64 
    /*0000b374*/ LDR         R1, [R4, #16] 
    /*0000b378*/ LDRLS       FP, [R2, #1368]             @ 0x558 
    /*0000b37c*/ MOV         R2, R5 
    /*0000b380*/ LDR         R4, [R0, #56]               @ 0x38 
    /*0000b384*/ STR         R1, [SP, #104]              @ 0x68 
    /*0000b388*/ LDR         R1, [R0, #32] 
    /*0000b38c*/ STR         R1, [SP, #108]              @ 0x6c 
    /*0000b390*/ LDR         R1, [R0, #36]               @ 0x24 
    /*0000b394*/ STR         R1, [SP, #112]              @ 0x70 
    /*0000b398*/ LDR         R1, [R0, #40]               @ 0x28 
    /*0000b39c*/ STR         R1, [SP, #116]              @ 0x74 
    /*0000b3a0*/ LDR         R1, [R0, #44]               @ 0x2c 
    /*0000b3a4*/ STR         R1, [SP, #120]              @ 0x78 
    /*0000b3a8*/ LDR         R1, [R0, #48]               @ 0x30 
    /*0000b3ac*/ STR         R1, [SP, #124]              @ 0x7c 
    /*0000b3b0*/ LDR         R1, [R0, #60]               @ 0x3c 
    /*0000b3b4*/ LDR         R0, [R0, #28] 
    /*0000b3b8*/ STR         R1, [SP, #128]              @ 0x80 
    /*0000b3bc*/ LDR         R1, [SP, #88]               @ 0x58 
    /*0000b3c0*/ LDR         R1, [R1, #64]               @ 0x40 
    /*0000b3c4*/ STR         R1, [SP, #68]               @ 0x44 
    /*0000b3c8*/ LDR         R1, [SP, #88]               @ 0x58 
    /*0000b3cc*/ LDR         R1, [R1, #68]               @ 0x44 
    /*0000b3d0*/ STR         LR, [SP, #36]               @ 0x24 
    /*0000b3d4*/ LDR         LR, [SP, #124]              @ 0x7c 
    /*0000b3d8*/ STR         FP, [SP, #60]               @ 0x3c 
    /*0000b3dc*/ STR         IP, [SP, #12] 
    /*0000b3e0*/ STR         LR, [SP, #32] 
    /*0000b3e4*/ LDR         LR, [SP, #120]              @ 0x78 
    /*0000b3e8*/ LDR         FP, [SP, #132]              @ 0x84 
    /*0000b3ec*/ LDR         IP, [SP, #104]              @ 0x68 
    /*0000b3f0*/ STR         LR, [SP, #28] 
    /*0000b3f4*/ LDR         LR, [SP, #116]              @ 0x74 
    /*0000b3f8*/ STR         FP, [SP, #48]               @ 0x30 
    /*0000b3fc*/ STR         IP, [SP, #8] 
    /*0000b400*/ STR         LR, [SP, #24] 
    /*0000b404*/ MOV         LR, #1 
    /*0000b408*/ STR         LR, [SP, #56]               @ 0x38 
    /*0000b40c*/ LDR         LR, [SP, #112]              @ 0x70 
    /*0000b410*/ LDR         FP, [SP, #128]              @ 0x80 
    /*0000b414*/ LDR         IP, [SP, #100]              @ 0x64 
    /*0000b418*/ STR         LR, [SP, #20] 
    /*0000b41c*/ LDR         LR, [SP, #108]              @ 0x6c 
    /*0000b420*/ STR         R1, [SP, #64]               @ 0x40 
    /*0000b424*/ STR         R0, [SP, #52]               @ 0x34 
    /*0000b428*/ LDR         R1, [SP, #148]              @ 0x94 
    /*0000b42c*/ LDR         R0, [SP, #84]               @ 0x54 
    /*0000b430*/ STR         FP, [SP, #44]               @ 0x2c 
    /*0000b434*/ STR         R4, [SP, #40]               @ 0x28 
    /*0000b438*/ STR         LR, [SP, #16] 
    /*0000b43c*/ STR         IP, [SP, #4] 
    /*0000b440*/ LDR         IP, [SP, #96]               @ 0x60 
    /*0000b444*/ STR         IP, [SP] 
    /*0000b448*/ BL          0x000000000000B448          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000b44c*/ B           RGN_Proc_xb2ac
    RGN_Proc_xb450:
    /*0000b450*/ LDR         R7, [SP, #152]              @ 0x98 
    /*0000b454*/ B           RGN_Proc_xb260
    RGN_Proc_xb458:
    /*0000b458*/ LDR         R6, [SP, #160]              @ 0xa0 
    /*0000b45c*/ B           RGN_Proc_xab70
    RGN_Proc_xb460:
    /*0000b460*/ MOV         R4, R3 
    RGN_Proc_xb464:
    /*0000b464*/ CMP         R4, #328                    @ 0x148 
    /*0000b468*/ BEQ         RGN_Proc_xa630
    /*0000b46c*/ ADD         R7, SP, #176                @ 0xb0 
    /*0000b470*/ LDR         R0, [PC, #428]              @ 0x000000000000b624 
    /*0000b474*/ MOV         R1, R7 
    /*0000b478*/ BL          0x000000000000B478          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*0000b47c*/ LDR         R2, [SP, #88]               @ 0x58 
    /*0000b480*/ ADD         R1, R5, #8 
    /*0000b484*/ LDR         R0, [R9, #176]              @ 0xb0 
    /*0000b488*/ BL          0x000000000000B488          @ RgnGetChnRgnAttr [R_ARM_CALL: .text+0x1238] 
    /*0000b48c*/ SUBS        R3, R0, #0 
    /*0000b490*/ MOV         R8, R3 
    /*0000b494*/ BEQ         RGN_Proc_xb5c8
    /*0000b498*/ LDR         R0, [R5, #8] 
    /*0000b49c*/ BL          0x0000000000000000 
    /*0000b4a0*/ LDR         R2, [R5, #12] 
    /*0000b4a4*/ LDR         R1, [R8, #152]              @ 0x98 
    /*0000b4a8*/ MOVW        R3, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*0000b4ac*/ MOVT        R3, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*0000b4b0*/ LDR         IP, [R8] 
    /*0000b4b4*/ STR         R2, [SP, #100]              @ 0x64 
    /*0000b4b8*/ LDR         R2, [R5, #16] 
    /*0000b4bc*/ LDR         R5, [R8, #12] 
    /*0000b4c0*/ STR         R2, [SP, #104]              @ 0x68 
    /*0000b4c4*/ LDR         R2, [R8, #76]               @ 0x4c 
    /*0000b4c8*/ STR         R2, [SP, #108]              @ 0x6c 
    /*0000b4cc*/ LDR         R2, [R8, #80]               @ 0x50 
    /*0000b4d0*/ STR         R2, [SP, #112]              @ 0x70 
    /*0000b4d4*/ STR         R0, [SP, #96]               @ 0x60 
    /*0000b4d8*/ LDR         R0, [R9, #208]              @ 0xd0 
    /*0000b4dc*/ CMP         R0, #0 
    /*0000b4e0*/ MOVEQ       R3, R6 
    /*0000b4e4*/ CMP         R1, #2 
    /*0000b4e8*/ STR         R3, [SP, #116]              @ 0x74 
    /*0000b4ec*/ MOVWLS      R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*0000b4f0*/ MOVTLS      R0, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*0000b4f4*/ ADDLS       R1, R0, R1, LSL #2 
    /*0000b4f8*/ MOVWHI      LR, #0                      @ .LC193 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2808] 
    /*0000b4fc*/ LDR         R0, [R4, #-84]              @ 0xffffffac 
    /*0000b500*/ MOVTHI      LR, #0                      @ .LC193 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2808] 
    /*0000b504*/ LDRLS       LR, [R1, #1356]             @ 0x54c 
    /*0000b508*/ CMP         IP, #0 
    /*0000b50c*/ MOVW        R1, #0                      @ .LC191 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2800] 
    /*0000b510*/ MOVT        R1, #0                      @ .LC191 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2800] 
    /*0000b514*/ LDR         R3, [R9, #176]              @ 0xb0 
    /*0000b518*/ MOVEQ       IP, R6 
    /*0000b51c*/ MOVNE       IP, R1 
    /*0000b520*/ CMP         R0, #0 
    /*0000b524*/ LDR         R4, [R8, #8] 
    /*0000b528*/ STR         R3, [SP, #92]               @ 0x5c 
    /*0000b52c*/ MOV         R3, R8 
    /*0000b530*/ LDR         R8, [R8, #64]               @ 0x40 
    /*0000b534*/ BEQ         RGN_Proc_xb5d8
    /*0000b538*/ LDR         R0, [R3, #120]              @ 0x78 
    /*0000b53c*/ LDR         R2, [R3, #128]              @ 0x80 
    /*0000b540*/ CMP         R0, #0 
    /*0000b544*/ LDR         R3, [R3, #124]              @ 0x7c 
    /*0000b548*/ MOVEQ       R0, R6 
    /*0000b54c*/ MOVNE       R0, R1 
    /*0000b550*/ CMP         R2, #0 
    /*0000b554*/ STR         R3, [SP, #120]              @ 0x78 
    /*0000b558*/ MOVEQ       R1, R6 
    RGN_Proc_xb55c:
    /*0000b55c*/ STR         IP, [SP, #20] 
    /*0000b560*/ LDR         IP, [SP, #116]              @ 0x74 
    /*0000b564*/ LDR         R2, [SP, #120]              @ 0x78 
    /*0000b568*/ STR         R0, [SP, #44]               @ 0x2c 
    /*0000b56c*/ STR         IP, [SP, #12] 
    /*0000b570*/ LDR         IP, [SP, #104]              @ 0x68 
    /*0000b574*/ LDR         R0, [SP, #112]              @ 0x70 
    /*0000b578*/ STR         R8, [SP, #40]               @ 0x28 
    /*0000b57c*/ STR         IP, [SP, #8] 
    /*0000b580*/ LDR         IP, [SP, #100]              @ 0x64 
    /*0000b584*/ LDR         R8, [SP, #108]              @ 0x6c 
    /*0000b588*/ STR         R1, [SP, #52]               @ 0x34 
    /*0000b58c*/ MOVW        R1, #0                      @ .LC251 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2ba8] 
    /*0000b590*/ STR         IP, [SP, #4] 
    /*0000b594*/ MOVT        R1, #0                      @ .LC251 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2ba8] 
    /*0000b598*/ LDR         IP, [SP, #96]               @ 0x60 
    /*0000b59c*/ STR         R2, [SP, #48]               @ 0x30 
    /*0000b5a0*/ STR         R0, [SP, #36]               @ 0x24 
    /*0000b5a4*/ LDR         R3, [SP, #92]               @ 0x5c 
    /*0000b5a8*/ LDR         R2, [SP, #88]               @ 0x58 
    /*0000b5ac*/ LDR         R0, [SP, #84]               @ 0x54 
    /*0000b5b0*/ STR         R5, [SP, #28] 
    /*0000b5b4*/ STR         R4, [SP, #24] 
    /*0000b5b8*/ STR         LR, [SP, #16] 
    /*0000b5bc*/ STR         IP, [SP] 
    /*0000b5c0*/ STR         R8, [SP, #32] 
    /*0000b5c4*/ BL          0x000000000000B5C4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    RGN_Proc_xb5c8:
    /*0000b5c8*/ MOV         R1, R7 
    /*0000b5cc*/ LDR         R0, [PC, #80]               @ 0x000000000000b624 
    /*0000b5d0*/ BL          0x000000000000B5D0          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000b5d4*/ B           RGN_Proc_xa630
    RGN_Proc_xb5d8:
    /*0000b5d8*/ STR         R0, [SP, #120]              @ 0x78 
    /*0000b5dc*/ MOV         R1, R6 
    /*0000b5e0*/ MOV         R0, R6 
    /*0000b5e4*/ B           RGN_Proc_xb55c
    RGN_Proc_xb5e8:
    /*0000b5e8*/ MOV         R8, R6 
    RGN_Proc_xb5ec:
    /*0000b5ec*/ LDR         R3, [SP, #132]              @ 0x84 
    /*0000b5f0*/ LDR         R2, [SP, #140]              @ 0x8c 
    /*0000b5f4*/ CMP         R3, R2 
    /*0000b5f8*/ LDR         R3, [SP, #88]               @ 0x58 
    /*0000b5fc*/ ADD         R3, R3, #1 
    /*0000b600*/ STR         R3, [SP, #88]               @ 0x58 
    /*0000b604*/ BNE         RGN_Proc_xa5a4
    /*0000b608*/ LDR         R3, [SP, #184]              @ 0xb8 
    /*0000b60c*/ CMP         R3, #0 
    /*0000b610*/ BEQ         RGN_Proc_x95c4
    /*0000b614*/ B           RGN_Proc_xa64c
    RGN_Proc_xb618:
    /*0000b618*/ MOVW        R1, #0                      @ .LC193 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2808] 
    /*0000b61c*/ MOVT        R1, #0                      @ .LC193 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2808] 
    /*0000b620*/ B           RGN_Proc_x9a0c
    /*0000b624*/ ANDEQ       R0, R0, R4 
FUNC_END RGN_Proc


FUNC_BEGIN RgnPoll
    /*0000b628*/ MOV         R0, #0 
    /*0000b62c*/ BX          LR 
FUNC_END RgnPoll


FUNC_BEGIN Rgn_Restore
    /*0000b630*/ MOV         R0, #0 
    /*0000b634*/ BX          LR 
FUNC_END Rgn_Restore


FUNC_BEGIN RGN_GetVerMagic
    /*0000b638*/ MOVW        R0, #61479                  @ 0xf027 
    /*0000b63c*/ MOVT        R0, #307                    @ 0x133 
    /*0000b640*/ BX          LR 
FUNC_END RGN_GetVerMagic


FUNC_BEGIN RgnProc
    /*0000b644*/ PUSH        {R4, LR} 
    /*0000b648*/ MOVW        R1, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3344] 
    /*0000b64c*/ MOVT        R1, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3344] 
    /*0000b650*/ MOV         R4, R0 
    /*0000b654*/ BL          0x000000000000B654          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000b658*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000b65c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*0000b660*/ LDR         R3, [R3] 
    /*0000b664*/ CMP         R3, #0 
    /*0000b668*/ BNE         RgnProc_xb674
    /*0000b66c*/ MOV         R0, R4 
    /*0000b670*/ BL          0x000000000000B670          @ RGN_Proc [R_ARM_CALL: .text+0x9528] 
    RgnProc_xb674:
    /*0000b674*/ MOV         R0, #0 
    /*0000b678*/ POP         {R4, PC} 
FUNC_END RgnProc


FUNC_BEGIN RGN_Exit
    /*0000b67c*/ PUSH        {R4, LR} 
    /*0000b680*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000b684*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*0000b688*/ LDR         R3, [R4] 
    /*0000b68c*/ CMP         R3, #2 
    /*0000b690*/ POPEQ       {R4, PC} 
    /*0000b694*/ BL          0x000000000000B694          @ RgnExit [R_ARM_CALL: .text+0x8458] 
    /*0000b698*/ MOV         R3, #2 
    /*0000b69c*/ STR         R3, [R4] 
    /*0000b6a0*/ POP         {R4, PC} 
FUNC_END RGN_Exit


FUNC_BEGIN RGN_Notify
    /*0000b6a4*/ CMP         R0, #17 
    /*0000b6a8*/ MOVWEQ      R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000b6ac*/ MOVTEQ      R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*0000b6b0*/ MOVEQ       R2, #1 
    /*0000b6b4*/ STREQ       R2, [R3] 
    RGN_Notify_xb6b8:
    /*0000b6b8*/ B           RGN_Notify_xb6b8            @ RgnNotify [R_ARM_JUMP24: .text+0x85cc] 
FUNC_END RGN_Notify


FUNC_BEGIN RGN_QueryState
    /*0000b6bc*/ PUSH        {R4, LR} 
    /*0000b6c0*/ MOV         R4, R0 
    /*0000b6c4*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c18] 
    /*0000b6c8*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c18] 
    /*0000b6cc*/ BL          0x000000000000B6CC          @ osal_atomic_read [R_ARM_CALL: +0x0] 
    /*0000b6d0*/ CMP         R0, #0 
    /*0000b6d4*/ MOVEQ       R3, #17 
    /*0000b6d8*/ MOVNE       R3, #34                     @ 0x22 
    /*0000b6dc*/ STR         R3, [R4] 
    /*0000b6e0*/ POP         {R4, PC} 
FUNC_END RGN_QueryState


FUNC_BEGIN RgnIoctl
    /*0000b6e4*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000b6e8*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*0000b6ec*/ LDR         R3, [R3] 
    /*0000b6f0*/ CMP         R3, #0 
    /*0000b6f4*/ BNE         RgnIoctl_xb894
    /*0000b6f8*/ PUSH        {R4, R5, LR} 
    /*0000b6fc*/ MOV         R4, R0 
    /*0000b700*/ SUB         SP, SP, #108                @ 0x6c 
    /*0000b704*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c18] 
    /*0000b708*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c18] 
    /*0000b70c*/ MOV         R5, R1 
    /*0000b710*/ BL          0x000000000000B710          @ osal_atomic_inc_return [R_ARM_CALL: +0x0] 
    /*0000b714*/ MOV         R3, #20992                  @ 0x5200 
    /*0000b718*/ MOVT        R3, #16416                  @ 0x4020 
    /*0000b71c*/ CMP         R4, R3 
    /*0000b720*/ BEQ         RgnIoctl_xb9bc
    /*0000b724*/ BLS         RgnIoctl_xb7a0
    /*0000b728*/ MOVW        R3, #21005                  @ 0x520d 
    /*0000b72c*/ MOVT        R3, #49164                  @ 0xc00c 
    /*0000b730*/ CMP         R4, R3 
    /*0000b734*/ BEQ         RgnIoctl_xb958
    /*0000b738*/ BHI         RgnIoctl_xb7f4
    /*0000b73c*/ MOVW        R3, #20998                  @ 0x5206 
    /*0000b740*/ MOVT        R3, #16468                  @ 0x4054 
    /*0000b744*/ CMP         R4, R3 
    /*0000b748*/ BEQ         RgnIoctl_xb940
    /*0000b74c*/ MOVW        R3, #21000                  @ 0x5208 
    /*0000b750*/ MOVT        R3, #16468                  @ 0x4054 
    /*0000b754*/ CMP         R4, R3 
    /*0000b758*/ BEQ         RgnIoctl_xb928
    /*0000b75c*/ MOVW        R3, #20994                  @ 0x5202 
    /*0000b760*/ MOVT        R3, #16416                  @ 0x4020 
    /*0000b764*/ CMP         R4, R3 
    /*0000b768*/ BEQ         RgnIoctl_xb87c
    RgnIoctl_xb76c:
    /*0000b76c*/ MOV         R1, #3 
    /*0000b770*/ UXTB        R4, R4 
    /*0000b774*/ MOV         R2, #204                    @ 0xcc 
    /*0000b778*/ STR         R4, [SP, #4] 
    /*0000b77c*/ STR         R2, [SP] 
    /*0000b780*/ MOV         R0, R1 
    /*0000b784*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x568] 
    /*0000b788*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3478] 
    /*0000b78c*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x568] 
    /*0000b790*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3478] 
    /*0000b794*/ BL          0x000000000000B794          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b798*/ MVN         R4, #0 
    /*0000b79c*/ B           RgnIoctl_xb864
    RgnIoctl_xb7a0:
    /*0000b7a0*/ MOVW        R3, #21004                  @ 0x520c 
    /*0000b7a4*/ MOVT        R3, #16388                  @ 0x4004 
    /*0000b7a8*/ CMP         R4, R3 
    /*0000b7ac*/ BEQ         RgnIoctl_xb8dc
    /*0000b7b0*/ BLS         RgnIoctl_xb838
    /*0000b7b4*/ MOVW        R3, #21001                  @ 0x5209 
    /*0000b7b8*/ MOVT        R3, #16400                  @ 0x4010 
    /*0000b7bc*/ CMP         R4, R3 
    /*0000b7c0*/ BEQ         RgnIoctl_xb914
    /*0000b7c4*/ MOVW        R3, #20996                  @ 0x5204 
    /*0000b7c8*/ MOVT        R3, #16408                  @ 0x4018 
    /*0000b7cc*/ CMP         R4, R3 
    /*0000b7d0*/ BEQ         RgnIoctl_xb8fc
    /*0000b7d4*/ MOVW        R3, #21006                  @ 0x520e 
    /*0000b7d8*/ MOVT        R3, #16388                  @ 0x4004 
    /*0000b7dc*/ CMP         R4, R3 
    /*0000b7e0*/ BNE         RgnIoctl_xb76c
    /*0000b7e4*/ LDR         R0, [R5] 
    /*0000b7e8*/ BL          0x000000000000B7E8          @ RgnBatchEnd [R_ARM_CALL: .text+0x9234] 
    /*0000b7ec*/ MOV         R4, R0 
    /*0000b7f0*/ B           RgnIoctl_xb864
    RgnIoctl_xb7f4:
    /*0000b7f4*/ MOVW        R3, #21002                  @ 0x520a 
    /*0000b7f8*/ MOVT        R3, #49200                  @ 0xc030 
    /*0000b7fc*/ CMP         R4, R3 
    /*0000b800*/ BEQ         RgnIoctl_xb8b8
    /*0000b804*/ MOVW        R3, #20999                  @ 0x5207 
    /*0000b808*/ MOVT        R3, #49236                  @ 0xc054 
    /*0000b80c*/ CMP         R4, R3 
    /*0000b810*/ BEQ         RgnIoctl_xb8a0
    /*0000b814*/ MOVW        R3, #20995                  @ 0x5203 
    /*0000b818*/ MOVT        R3, #49184                  @ 0xc020 
    /*0000b81c*/ CMP         R4, R3 
    /*0000b820*/ BNE         RgnIoctl_xb76c
    /*0000b824*/ ADD         R1, R5, #8 
    /*0000b828*/ LDR         R0, [R5, #4] 
    /*0000b82c*/ BL          0x000000000000B82C          @ RgnGetAttr [R_ARM_CALL: .text+0x5548] 
    /*0000b830*/ MOV         R4, R0 
    /*0000b834*/ B           RgnIoctl_xb864
    RgnIoctl_xb838:
    /*0000b838*/ MOVW        R3, #20993                  @ 0x5201 
    /*0000b83c*/ MOVT        R3, #16388                  @ 0x4004 
    /*0000b840*/ CMP         R4, R3 
    /*0000b844*/ BEQ         RgnIoctl_xb8ec
    /*0000b848*/ MOVW        R3, #21003                  @ 0x520b 
    /*0000b84c*/ MOVT        R3, #16388                  @ 0x4004 
    /*0000b850*/ CMP         R4, R3 
    /*0000b854*/ BNE         RgnIoctl_xb76c
    /*0000b858*/ LDR         R0, [R5] 
    /*0000b85c*/ BL          0x000000000000B85C          @ RgnPutCanvasInfo [R_ARM_CALL: .text+0x70d0] 
    /*0000b860*/ MOV         R4, R0 
    RgnIoctl_xb864:
    /*0000b864*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c18] 
    /*0000b868*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c18] 
    /*0000b86c*/ BL          0x000000000000B86C          @ osal_atomic_dec_return [R_ARM_CALL: +0x0] 
    /*0000b870*/ MOV         R0, R4 
    /*0000b874*/ ADD         SP, SP, #108                @ 0x6c 
    /*0000b878*/ POP         {R4, R5, PC} 
    RgnIoctl_xb87c:
    /*0000b87c*/ MOV         R1, R5 
    /*0000b880*/ LDR         R0, [R5, #4] 
    /*0000b884*/ LDR         R2, [R1], #8 
    /*0000b888*/ BL          0x000000000000B888          @ RgnSetAttr [R_ARM_CALL: .text+0x56a8] 
    /*0000b88c*/ MOV         R4, R0 
    /*0000b890*/ B           RgnIoctl_xb864
    RgnIoctl_xb894:
    /*0000b894*/ MOVW        R0, #32786                  @ 0x8012 
    /*0000b898*/ MOVT        R0, #40963                  @ 0xa003 
    /*0000b89c*/ BX          LR 
    RgnIoctl_xb8a0:
    /*0000b8a0*/ ADD         R2, R5, #16 
    /*0000b8a4*/ ADD         R1, R5, #4 
    /*0000b8a8*/ LDR         R0, [R5] 
    /*0000b8ac*/ BL          0x000000000000B8AC          @ RgnGetDisplayAttr [R_ARM_CALL: .text+0x7ed0] 
    /*0000b8b0*/ MOV         R4, R0 
    /*0000b8b4*/ B           RgnIoctl_xb864
    RgnIoctl_xb8b8:
    /*0000b8b8*/ LDR         IP, [R5] 
    /*0000b8bc*/ ADD         R3, R5, #8 
    /*0000b8c0*/ LDR         R0, [R5, #4] 
    /*0000b8c4*/ ADD         R2, R5, #12 
    /*0000b8c8*/ ADD         R1, R5, #16 
    /*0000b8cc*/ STR         IP, [SP] 
    /*0000b8d0*/ BL          0x000000000000B8D0          @ RgnGetCanvasInfo [R_ARM_CALL: .text+0x6b00] 
    /*0000b8d4*/ MOV         R4, R0 
    /*0000b8d8*/ B           RgnIoctl_xb864
    RgnIoctl_xb8dc:
    /*0000b8dc*/ LDR         R0, [R5] 
    /*0000b8e0*/ BL          0x000000000000B8E0          @ RgnUpdateCanvas [R_ARM_CALL: .text+0x71a0] 
    /*0000b8e4*/ MOV         R4, R0 
    /*0000b8e8*/ B           RgnIoctl_xb864
    RgnIoctl_xb8ec:
    /*0000b8ec*/ LDR         R0, [R5] 
    /*0000b8f0*/ BL          0x000000000000B8F0          @ RgnDestroy [R_ARM_CALL: .text+0x50d4] 
    /*0000b8f4*/ MOV         R4, R0 
    /*0000b8f8*/ B           RgnIoctl_xb864
    RgnIoctl_xb8fc:
    /*0000b8fc*/ MOV         R1, R5 
    /*0000b900*/ LDR         R0, [R5, #4] 
    /*0000b904*/ LDR         R2, [R1], #8 
    /*0000b908*/ BL          0x000000000000B908          @ RgnSetBitMap [R_ARM_CALL: .text+0x6000] 
    /*0000b90c*/ MOV         R4, R0 
    /*0000b910*/ B           RgnIoctl_xb864
    RgnIoctl_xb914:
    /*0000b914*/ ADD         R1, R5, #4 
    /*0000b918*/ LDR         R0, [R5] 
    /*0000b91c*/ BL          0x000000000000B91C          @ RgnDetachFrmChn [R_ARM_CALL: .text+0x7bac] 
    /*0000b920*/ MOV         R4, R0 
    /*0000b924*/ B           RgnIoctl_xb864
    RgnIoctl_xb928:
    /*0000b928*/ ADD         R2, R5, #16 
    /*0000b92c*/ ADD         R1, R5, #4 
    /*0000b930*/ LDR         R0, [R5] 
    /*0000b934*/ BL          0x000000000000B934          @ RgnAttachToChn [R_ARM_CALL: .text+0x7568] 
    /*0000b938*/ MOV         R4, R0 
    /*0000b93c*/ B           RgnIoctl_xb864
    RgnIoctl_xb940:
    /*0000b940*/ ADD         R2, R5, #16 
    /*0000b944*/ ADD         R1, R5, #4 
    /*0000b948*/ LDR         R0, [R5] 
    /*0000b94c*/ BL          0x000000000000B94C          @ RgnSetDisplayAttr [R_ARM_CALL: .text+0x8064] 
    /*0000b950*/ MOV         R4, R0 
    /*0000b954*/ B           RgnIoctl_xb864
    RgnIoctl_xb958:
    /*0000b958*/ LDR         R2, [R5, #4] 
    /*0000b95c*/ SUB         R3, R2, #1 
    /*0000b960*/ CMP         R3, #23 
    /*0000b964*/ BHI         RgnIoctl_xb9d0
    /*0000b968*/ LDR         R1, [R5, #8] 
    /*0000b96c*/ CMP         R1, #0 
    /*0000b970*/ BEQ         RgnIoctl_xba1c
    /*0000b974*/ LSL         R2, R2, #2 
    /*0000b978*/ ADD         R0, SP, #8 
    /*0000b97c*/ BL          0x000000000000B97C          @ osal_copy_from_user [R_ARM_CALL: +0x0] 
    /*0000b980*/ CMP         R0, #0 
    /*0000b984*/ BEQ         RgnIoctl_xba04
    /*0000b988*/ LDR         R2, [R5, #4] 
    /*0000b98c*/ MOV         R1, #3 
    /*0000b990*/ MOV         IP, #184                    @ 0xb8 
    /*0000b994*/ MOV         R0, R1 
    /*0000b998*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x568] 
    /*0000b99c*/ STR         IP, [SP] 
    /*0000b9a0*/ STR         R2, [SP, #4] 
    /*0000b9a4*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x568] 
    /*0000b9a8*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x342c] 
    /*0000b9ac*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x342c] 
    /*0000b9b0*/ BL          0x000000000000B9B0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b9b4*/ MVN         R4, #0 
    /*0000b9b8*/ B           RgnIoctl_xb864
    RgnIoctl_xb9bc:
    /*0000b9bc*/ ADD         R1, R5, #8 
    /*0000b9c0*/ LDR         R0, [R5, #4] 
    /*0000b9c4*/ BL          0x000000000000B9C4          @ RgnCreate [R_ARM_CALL: .text+0x4e2c] 
    /*0000b9c8*/ MOV         R4, R0 
    /*0000b9cc*/ B           RgnIoctl_xb864
    RgnIoctl_xb9d0:
    /*0000b9d0*/ MOV         R1, #3 
    /*0000b9d4*/ STR         R2, [SP, #4] 
    /*0000b9d8*/ MOV         R0, R1 
    /*0000b9dc*/ MOV         R2, #169                    @ 0xa9 
    /*0000b9e0*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x568] 
    /*0000b9e4*/ STR         R2, [SP] 
    /*0000b9e8*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x568] 
    /*0000b9ec*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x33a4] 
    /*0000b9f0*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x33a4] 
    /*0000b9f4*/ MOVW        R4, #32771                  @ 0x8003 
    /*0000b9f8*/ BL          0x000000000000B9F8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000b9fc*/ MOVT        R4, #40963                  @ 0xa003 
    /*0000ba00*/ B           RgnIoctl_xb864
    RgnIoctl_xba04:
    /*0000ba04*/ ADD         R2, SP, #8 
    /*0000ba08*/ LDR         R1, [R5, #4] 
    /*0000ba0c*/ MOV         R0, R5 
    /*0000ba10*/ BL          0x000000000000BA10          @ RgnBatchBegin [R_ARM_CALL: .text+0x8f28] 
    /*0000ba14*/ MOV         R4, R0 
    /*0000ba18*/ B           RgnIoctl_xb864
    RgnIoctl_xba1c:
    /*0000ba1c*/ MOV         R1, #3 
    /*0000ba20*/ MOV         R3, #175                    @ 0xaf 
    /*0000ba24*/ MOV         R0, R1 
    /*0000ba28*/ STR         R3, [SP] 
    /*0000ba2c*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x33fc] 
    /*0000ba30*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .rodata+0x568] 
    /*0000ba34*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x33fc] 
    /*0000ba38*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .rodata+0x568] 
    /*0000ba3c*/ MOVW        R4, #32771                  @ 0x8003 
    /*0000ba40*/ BL          0x000000000000BA40          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000ba44*/ MOVT        R4, #40963                  @ 0xa003 
    /*0000ba48*/ B           RgnIoctl_xb864
FUNC_END RgnIoctl


FUNC_BEGIN RGN_Init
    /*0000ba4c*/ PUSH        {R4, R5, R6, LR} 
    /*0000ba50*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*0000ba54*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .data+0x0] 
    /*0000ba58*/ LDR         R0, [R5] 
    /*0000ba5c*/ CMP         R0, #0 
    /*0000ba60*/ MOVEQ       R4, R0 
    /*0000ba64*/ BEQ         RGN_Init_xba7c
    /*0000ba68*/ CMP         R0, #1 
    /*0000ba6c*/ BEQ         RGN_Init_xbab4
    /*0000ba70*/ BL          0x000000000000BA70          @ RgnInit [R_ARM_CALL: .text+0x8450] 
    /*0000ba74*/ SUBS        R4, R0, #0 
    /*0000ba78*/ BEQ         RGN_Init_xba84
    RGN_Init_xba7c:
    /*0000ba7c*/ MOV         R0, R4 
    /*0000ba80*/ POP         {R4, R5, R6, PC} 
    RGN_Init_xba84:
    /*0000ba84*/ MOV         R0, #2 
    /*0000ba88*/ STR         R4, [R5] 
    /*0000ba8c*/ BL          0x000000000000BA8C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000ba90*/ LDR         R3, [R0, #12] 
    /*0000ba94*/ CMP         R3, #0 
    /*0000ba98*/ BEQ         RGN_Init_xbac0
    /*0000ba9c*/ MOV         R0, #2 
    /*0000baa0*/ BL          0x000000000000BAA0          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000baa4*/ LDR         R3, [R0, #12] 
    /*0000baa8*/ BLX         R3 
    /*0000baac*/ MOV         R0, R4 
    /*0000bab0*/ POP         {R4, R5, R6, PC} 
    RGN_Init_xbab4:
    /*0000bab4*/ MOVW        R4, #32786                  @ 0x8012 
    /*0000bab8*/ MOVT        R4, #40963                  @ 0xa003 
    /*0000babc*/ B           RGN_Init_xba7c
    RGN_Init_xbac0:
    /*0000bac0*/ MVN         R4, #0 
    /*0000bac4*/ B           RGN_Init_xba7c
FUNC_END RGN_Init


FUNC_BEGIN RgnClose
    /*0000bac8*/ MOV         R0, #0 
    /*0000bacc*/ BX          LR 
FUNC_END RgnClose


FUNC_BEGIN Rgn_Freeze
    /*0000bad0*/ MOV         R0, #0 
    /*0000bad4*/ BX          LR 
FUNC_END Rgn_Freeze


FUNC_BEGIN RgnOpen
    /*0000bad8*/ MOV         R0, #0 
    /*0000badc*/ BX          LR 
FUNC_END RgnOpen


FUNC_BEGIN RGN_ModInit
    /*0000bae0*/ PUSH        {R4, R5, R6, LR} 
    /*0000bae4*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c18] 
    /*0000bae8*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c18] 
    /*0000baec*/ MOV         R0, R4 
    /*0000baf0*/ BL          0x000000000000BAF0          @ osal_atomic_init [R_ARM_CALL: +0x0] 
    /*0000baf4*/ SUBS        R5, R0, #0 
    /*0000baf8*/ BNE         RGN_ModInit_xbbc4
    /*0000bafc*/ MOV         R1, R5 
    /*0000bb00*/ MOV         R0, R4 
    /*0000bb04*/ BL          0x000000000000BB04          @ osal_atomic_set [R_ARM_CALL: +0x0] 
    /*0000bb08*/ BL          0x000000000000BB08          @ RGN_LockInit [R_ARM_CALL: .text+0xc80] 
    /*0000bb0c*/ SUBS        R1, R0, #0 
    /*0000bb10*/ BNE         RGN_ModInit_xbba4
    /*0000bb14*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x34e4] 
    /*0000bb18*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x34e4] 
    /*0000bb1c*/ BL          0x000000000000BB1C          @ osal_createdev [R_ARM_CALL: +0x0] 
    /*0000bb20*/ CMP         R0, #0 
    /*0000bb24*/ STR         R0, [R4, #4] 
    /*0000bb28*/ BEQ         RGN_ModInit_xbc38
    /*0000bb2c*/ LDR         R6, [PC, #352]              @ 0x000000000000bc94 
    /*0000bb30*/ MOV         R2, #16 
    /*0000bb34*/ STR         R2, [R0, #52]               @ 0x34 
    /*0000bb38*/ ADD         R2, R6, #32 
    /*0000bb3c*/ STR         R2, [R0, #60]               @ 0x3c 
    /*0000bb40*/ STR         R6, [R0, #56]               @ 0x38 
    /*0000bb44*/ BL          0x000000000000BB44          @ osal_registerdevice [R_ARM_CALL: +0x0] 
    /*0000bb48*/ CMP         R0, #0 
    /*0000bb4c*/ BLT         RGN_ModInit_xbc0c
    /*0000bb50*/ ADD         R0, R6, #112                @ 0x70 
    /*0000bb54*/ BL          0x000000000000BB54          @ CMPI_RegisterModule [R_ARM_CALL: +0x0] 
    /*0000bb58*/ CMP         R0, #0 
    /*0000bb5c*/ BNE         RGN_ModInit_xbbd8
    /*0000bb60*/ BL          0x000000000000BB60          @ RgnCfgCapa [R_ARM_CALL: .text+0x41c] 
    /*0000bb64*/ MOV         R1, R5 
    /*0000bb68*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x34e4] 
    /*0000bb6c*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x34e4] 
    /*0000bb70*/ BL          0x000000000000BB70          @ osal_create_proc_entry [R_ARM_CALL: +0x0] 
    /*0000bb74*/ CMP         R0, #0 
    /*0000bb78*/ BEQ         RGN_ModInit_xbc58
    /*0000bb7c*/ MOVW        R3, #0                      @ RgnProc [R_ARM_MOVW_ABS_NC: .text+0xb644] 
    /*0000bb80*/ MOVW        R1, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3560] 
    /*0000bb84*/ MOVT        R3, #0                      @ RgnProc [R_ARM_MOVT_ABS: .text+0xb644] 
    /*0000bb88*/ MOVT        R1, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3560] 
    /*0000bb8c*/ STR         R3, [R0, #60]               @ 0x3c 
    /*0000bb90*/ MOVW        R0, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x356c] 
    /*0000bb94*/ MOVT        R0, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x356c] 
    /*0000bb98*/ BL          0x000000000000BB98          @ osal_printk [R_ARM_CALL: +0x0] 
    RGN_ModInit_xbb9c:
    /*0000bb9c*/ MOV         R0, R5 
    /*0000bba0*/ POP         {R4, R5, R6, PC} 
    RGN_ModInit_xbba4:
    /*0000bba4*/ MOVW        R0, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x34c0] 
    /*0000bba8*/ MOVT        R0, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x34c0] 
    /*0000bbac*/ MOV         R5, R1 
    /*0000bbb0*/ BL          0x000000000000BBB0          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000bbb4*/ MOV         R0, R4 
    /*0000bbb8*/ BL          0x000000000000BBB8          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*0000bbbc*/ MOV         R0, R5 
    /*0000bbc0*/ POP         {R4, R5, R6, PC} 
    RGN_ModInit_xbbc4:
    /*0000bbc4*/ MOVW        R0, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x34ac] 
    /*0000bbc8*/ MOVT        R0, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x34ac] 
    /*0000bbcc*/ BL          0x000000000000BBCC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000bbd0*/ MVN         R5, #0 
    /*0000bbd4*/ B           RGN_ModInit_xbb9c
    RGN_ModInit_xbbd8:
    /*0000bbd8*/ LDR         R0, [R4, #4] 
    /*0000bbdc*/ BL          0x000000000000BBDC          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*0000bbe0*/ LDR         R0, [R4, #4] 
    /*0000bbe4*/ BL          0x000000000000BBE4          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*0000bbe8*/ STR         R5, [R4, #4] 
    /*0000bbec*/ BL          0x000000000000BBEC          @ RGN_LockExit [R_ARM_CALL: .text+0xcfc] 
    /*0000bbf0*/ MOV         R0, R4 
    /*0000bbf4*/ BL          0x000000000000BBF4          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*0000bbf8*/ MOVW        R0, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3524] 
    /*0000bbfc*/ MOVT        R0, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3524] 
    /*0000bc00*/ MVN         R5, #0 
    /*0000bc04*/ BL          0x000000000000BC04          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000bc08*/ B           RGN_ModInit_xbb9c
    RGN_ModInit_xbc0c:
    /*0000bc0c*/ LDR         R0, [R4, #4] 
    /*0000bc10*/ BL          0x000000000000BC10          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*0000bc14*/ STR         R5, [R4, #4] 
    /*0000bc18*/ BL          0x000000000000BC18          @ RGN_LockExit [R_ARM_CALL: .text+0xcfc] 
    /*0000bc1c*/ MOV         R0, R4 
    /*0000bc20*/ BL          0x000000000000BC20          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*0000bc24*/ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3504] 
    /*0000bc28*/ MOVT        R0, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3504] 
    /*0000bc2c*/ MVN         R5, #0 
    /*0000bc30*/ BL          0x000000000000BC30          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000bc34*/ B           RGN_ModInit_xbb9c
    RGN_ModInit_xbc38:
    /*0000bc38*/ BL          0x000000000000BC38          @ RGN_LockExit [R_ARM_CALL: .text+0xcfc] 
    /*0000bc3c*/ MOV         R0, R4 
    /*0000bc40*/ BL          0x000000000000BC40          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*0000bc44*/ MOVW        R0, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x34e8] 
    /*0000bc48*/ MOVT        R0, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x34e8] 
    /*0000bc4c*/ MVN         R5, #0 
    /*0000bc50*/ BL          0x000000000000BC50          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000bc54*/ B           RGN_ModInit_xbb9c
    RGN_ModInit_xbc58:
    /*0000bc58*/ MOV         R0, #3 
    /*0000bc5c*/ BL          0x000000000000BC5C          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    /*0000bc60*/ LDR         R0, [R4, #4] 
    /*0000bc64*/ BL          0x000000000000BC64          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*0000bc68*/ LDR         R0, [R4, #4] 
    /*0000bc6c*/ BL          0x000000000000BC6C          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*0000bc70*/ STR         R5, [R4, #4] 
    /*0000bc74*/ BL          0x000000000000BC74          @ RGN_LockExit [R_ARM_CALL: .text+0xcfc] 
    /*0000bc78*/ MOV         R0, R4 
    /*0000bc7c*/ BL          0x000000000000BC7C          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*0000bc80*/ MOVW        R0, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3544] 
    /*0000bc84*/ MOVT        R0, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3544] 
    /*0000bc88*/ MVN         R5, #0 
    /*0000bc8c*/ BL          0x000000000000BC8C          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000bc90*/ B           RGN_ModInit_xbb9c
    /*0000bc94*/ ANDEQ       R0, R0, R4 
FUNC_END RGN_ModInit


FUNC_BEGIN RGN_ModExit
    /*0000bc98*/ PUSH        {R4, LR} 
    /*0000bc9c*/ MOV         R1, #0 
    /*0000bca0*/ MOVW        R0, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x34e4] 
    /*0000bca4*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c18] 
    /*0000bca8*/ MOVT        R0, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x34e4] 
    /*0000bcac*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c18] 
    /*0000bcb0*/ BL          0x000000000000BCB0          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*0000bcb4*/ MOV         R0, #3 
    /*0000bcb8*/ BL          0x000000000000BCB8          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    /*0000bcbc*/ LDR         R0, [R4, #4] 
    /*0000bcc0*/ BL          0x000000000000BCC0          @ osal_deregisterdevice [R_ARM_CALL: +0x0] 
    /*0000bcc4*/ LDR         R0, [R4, #4] 
    /*0000bcc8*/ BL          0x000000000000BCC8          @ osal_destroydev [R_ARM_CALL: +0x0] 
    /*0000bccc*/ MOV         R3, #0 
    /*0000bcd0*/ STR         R3, [R4, #4] 
    /*0000bcd4*/ BL          0x000000000000BCD4          @ RGN_LockExit [R_ARM_CALL: .text+0xcfc] 
    /*0000bcd8*/ MOV         R0, R4 
    /*0000bcdc*/ BL          0x000000000000BCDC          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*0000bce0*/ MOVW        R1, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3560] 
    /*0000bce4*/ MOVW        R0, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x358c] 
    /*0000bce8*/ MOVT        R1, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3560] 
    /*0000bcec*/ MOVT        R0, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x358c] 
    /*0000bcf0*/ POP         {R4, LR} 
    RGN_ModExit_xbcf4:
    /*0000bcf4*/ B           RGN_ModExit_xbcf4           @ osal_printk [R_ARM_JUMP24: +0x0] 
FUNC_END RGN_ModExit


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .init.text
@ Size: 0x10
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN rgn_mod_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ BL          0x0000000000000004          @ RGN_ModInit [R_ARM_CALL: .text+0xbae0] 
    /*00000008*/ MOV         R0, #0 
    /*0000000c*/ POP         {R4, PC} 
FUNC_END rgn_mod_init




@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .exit.text
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN rgn_mod_exit
    rgn_mod_exit_x0:
    /*00000000*/ B           rgn_mod_exit_x0             @ RGN_ModExit [R_ARM_JUMP24: .text+0xbc98] 
FUNC_END rgn_mod_exit




