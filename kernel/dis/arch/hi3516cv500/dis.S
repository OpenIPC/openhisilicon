@
@ -- FILES --
@

@ dis_list.c
@ dis.c
@ dis_debug.c
@ dis_drv_comm.c
@ dis_drv.c
@ dis_hal.c
@ hiisp_dis_type.c
@ dis_processor.c
@ dis_motion_filter.c
@ hiisp_dis_cfg_parse.c
@ hiisp_dis_comm_fw.c
@ dis_init.c
@ hi3516cv500_dis.mod.c


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text
@ Size: 0xe744
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN DIS_InitNodeBuf
    /*00000000*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00000004*/ MOV         R9, #1312                   @ 0x520 
    /*00000008*/ MUL         R8, R9, R1 
    /*0000000c*/ MOV         R5, R0 
    /*00000010*/ ADD         R3, R0, #20 
    /*00000014*/ ADD         R7, R0, #12 
    /*00000018*/ STR         R3, [R0, #20] 
    /*0000001c*/ SUB         SP, SP, #20 
    /*00000020*/ STR         R3, [R0, #24] 
    /*00000024*/ MOV         R6, R1 
    /*00000028*/ STR         R7, [R0, #12] 
    /*0000002c*/ MOV         R0, R8 
    /*00000030*/ STR         R7, [R5, #16] 
    /*00000034*/ BL          0x0000000000000034          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*00000038*/ SUBS        R4, R0, #0 
    /*0000003c*/ BEQ         DIS_InitNodeBuf_xb0
    /*00000040*/ MOV         R2, R8 
    /*00000044*/ MOV         R1, #0 
    /*00000048*/ BL          0x0000000000000048          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000004c*/ CMP         R6, #0 
    /*00000050*/ STR         R4, [R5] 
    /*00000054*/ BEQ         DIS_InitNodeBuf_xa4
    /*00000058*/ MOV         R2, #0 
    /*0000005c*/ MOVW        LR, #1300                   @ 0x514 
    /*00000060*/ MOV         R1, R2 
    /*00000064*/ B           DIS_InitNodeBuf_x6c
    DIS_InitNodeBuf_x68:
    /*00000068*/ LDR         R4, [R5] 
    DIS_InitNodeBuf_x6c:
    /*0000006c*/ LDR         R0, [R5, #16] 
    /*00000070*/ ADD         R3, R4, R2 
    /*00000074*/ ADD         R1, R1, #1 
    /*00000078*/ ADD         IP, R3, LR 
    /*0000007c*/ CMP         R6, R1 
    /*00000080*/ STR         IP, [R5, #16] 
    /*00000084*/ STR         R7, [R3, #1300]             @ 0x514 
    /*00000088*/ ADD         R2, R2, #1312               @ 0x520 
    /*0000008c*/ STR         R0, [R3, #1304]             @ 0x518 
    /*00000090*/ STR         IP, [R0] 
    /*00000094*/ LDR         R3, [R5, #8] 
    /*00000098*/ ADD         R3, R3, #1 
    /*0000009c*/ STR         R3, [R5, #8] 
    /*000000a0*/ BNE         DIS_InitNodeBuf_x68
    DIS_InitNodeBuf_xa4:
    /*000000a4*/ MOV         R0, #0 
    DIS_InitNodeBuf_xa8:
    /*000000a8*/ ADD         SP, SP, #20 
    /*000000ac*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    DIS_InitNodeBuf_xb0:
    /*000000b0*/ MOV         R2, #36                     @ 0x24 
    /*000000b4*/ STR         R9, [SP, #8] 
    /*000000b8*/ STR         R2, [SP] 
    /*000000bc*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x0] 
    /*000000c0*/ STR         R6, [SP, #4] 
    /*000000c4*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x0] 
    /*000000c8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x0] 
    /*000000cc*/ MOV         R1, #17 
    /*000000d0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x0] 
    /*000000d4*/ MOV         R0, #1 
    /*000000d8*/ BL          0x00000000000000D8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000000dc*/ MOVW        R0, #32781                  @ 0x800d 
    /*000000e0*/ MOVT        R0, #40977                  @ 0xa011 
    /*000000e4*/ B           DIS_InitNodeBuf_xa8
FUNC_END DIS_InitNodeBuf


FUNC_BEGIN DIS_ReleaseNodeBuf
    /*000000e8*/ PUSH        {R4, R5, R6, LR} 
    /*000000ec*/ MOV         R5, R0 
    /*000000f0*/ LDR         R3, [R0, #672]              @ 0x2a0 
    /*000000f4*/ MOV         R4, R1 
    /*000000f8*/ CMP         R3, #0 
    /*000000fc*/ BEQ         DIS_ReleaseNodeBuf_x10c
    /*00000100*/ MOV         R1, #11 
    /*00000104*/ ADD         R0, R0, #336                @ 0x150 
    /*00000108*/ BL          0x0000000000000108          @ DIS_VbSub [R_ARM_CALL: .text+0x7c8] 
    DIS_ReleaseNodeBuf_x10c:
    /*0000010c*/ LDR         R2, [R4, #16] 
    /*00000110*/ ADD         R3, R5, #1296               @ 0x510 
    /*00000114*/ ADD         R3, R3, #4 
    /*00000118*/ ADD         R1, R4, #12 
    /*0000011c*/ STR         R3, [R4, #16] 
    /*00000120*/ STR         R2, [R5, #1304]             @ 0x518 
    /*00000124*/ STR         R1, [R5, #1300]             @ 0x514 
    /*00000128*/ STR         R3, [R2] 
    /*0000012c*/ LDR         R3, [R4, #8] 
    /*00000130*/ ADD         R3, R3, #1 
    /*00000134*/ STR         R3, [R4, #8] 
    /*00000138*/ POP         {R4, R5, R6, PC} 
FUNC_END DIS_ReleaseNodeBuf


FUNC_BEGIN DIS_DeInitNodeBuf
    /*0000013c*/ LDR         R3, [R0] 
    /*00000140*/ CMP         R3, #0 
    /*00000144*/ BXEQ        LR 
    /*00000148*/ PUSH        {R4, LR} 
    /*0000014c*/ SUB         SP, SP, #8 
    /*00000150*/ MOV         R4, R0 
    /*00000154*/ MOV         R1, #17 
    /*00000158*/ STR         R3, [SP, #4] 
    /*0000015c*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x48] 
    /*00000160*/ MOV         R3, #69                     @ 0x45 
    /*00000164*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x48] 
    /*00000168*/ STR         R3, [SP] 
    /*0000016c*/ MOV         R0, #7 
    /*00000170*/ LDR         R3, [PC, #24]               @ 0x0000000000000190 
    /*00000174*/ BL          0x0000000000000174          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000178*/ LDR         R0, [R4] 
    /*0000017c*/ BL          0x000000000000017C          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*00000180*/ MOV         R3, #0 
    /*00000184*/ STR         R3, [R4] 
    /*00000188*/ ADD         SP, SP, #8 
    /*0000018c*/ POP         {R4, PC} 
    /*00000190*/ ANDEQ       R0, R0, R0, LSL R0 
FUNC_END DIS_DeInitNodeBuf


FUNC_BEGIN DIS_GetNodeFromFreeList
    /*00000194*/ MOV         R3, R0 
    /*00000198*/ PUSH        {R4, R5, R6, LR} 
    /*0000019c*/ LDR         R4, [R3, #12]! 
    /*000001a0*/ CMP         R4, R3 
    /*000001a4*/ BEQ         DIS_GetNodeFromFreeList_x210
    /*000001a8*/ CMP         R4, #0 
    /*000001ac*/ MOV         R5, R0 
    /*000001b0*/ BEQ         DIS_GetNodeFromFreeList_x1f0
    DIS_GetNodeFromFreeList_x1b4:
    /*000001b4*/ LDR         R1, [R4, #4] 
    /*000001b8*/ MOV         R2, #256                    @ 0x100 
    /*000001bc*/ LDR         IP, [R4] 
    /*000001c0*/ MOVT        R2, #16 
    /*000001c4*/ MOV         R3, #512                    @ 0x200 
    /*000001c8*/ MOVT        R3, #32 
    /*000001cc*/ SUB         R0, R4, #1296               @ 0x510 
    /*000001d0*/ STR         R1, [IP, #4] 
    /*000001d4*/ SUB         R0, R0, #4 
    /*000001d8*/ STR         IP, [R1] 
    /*000001dc*/ STM         R4, {R2, R3} 
    /*000001e0*/ LDR         R3, [R5, #8] 
    /*000001e4*/ SUB         R3, R3, #1 
    /*000001e8*/ STR         R3, [R5, #8] 
    /*000001ec*/ POP         {R4, R5, R6, PC} 
    DIS_GetNodeFromFreeList_x1f0:
    /*000001f0*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7c] 
    /*000001f4*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x80] 
    /*000001f8*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7c] 
    /*000001fc*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x80] 
    /*00000200*/ MOV         R2, #89                     @ 0x59 
    /*00000204*/ LDR         R1, [PC, #12]               @ 0x0000000000000218 
    /*00000208*/ BL          0x0000000000000208          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000020c*/ B           DIS_GetNodeFromFreeList_x1b4
    DIS_GetNodeFromFreeList_x210:
    /*00000210*/ MOV         R0, #0 
    /*00000214*/ POP         {R4, R5, R6, PC} 
    /*00000218*/ ANDEQ       R0, R0, R4, LSR #32 
FUNC_END DIS_GetNodeFromFreeList


FUNC_BEGIN DIS_QueryNodeFromBusyList
    /*0000021c*/ PUSH        {R4, LR} 
    /*00000220*/ LDR         R4, [R0, #20]! 
    /*00000224*/ CMP         R4, R0 
    /*00000228*/ BEQ         DIS_QueryNodeFromBusyList_x260
    /*0000022c*/ CMP         R4, #0 
    /*00000230*/ BEQ         DIS_QueryNodeFromBusyList_x240
    DIS_QueryNodeFromBusyList_x234:
    /*00000234*/ SUB         R0, R4, #1296               @ 0x510 
    /*00000238*/ SUB         R0, R0, #4 
    /*0000023c*/ POP         {R4, PC} 
    DIS_QueryNodeFromBusyList_x240:
    /*00000240*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7c] 
    /*00000244*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x80] 
    /*00000248*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7c] 
    /*0000024c*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x80] 
    /*00000250*/ MOV         R2, #113                    @ 0x71 
    /*00000254*/ LDR         R1, [PC, #12]               @ 0x0000000000000268 
    /*00000258*/ BL          0x0000000000000258          @ osal_panic [R_ARM_CALL: +0x0] 
    /*0000025c*/ B           DIS_QueryNodeFromBusyList_x234
    DIS_QueryNodeFromBusyList_x260:
    /*00000260*/ MOV         R0, #0 
    /*00000264*/ POP         {R4, PC} 
    /*00000268*/ ANDEQ       R0, R0, IP, LSR R0 
FUNC_END DIS_QueryNodeFromBusyList


FUNC_BEGIN DIS_PutNodeToFreeList
    /*0000026c*/ LDR         R2, [R0, #16] 
    /*00000270*/ ADD         R3, R1, #1296               @ 0x510 
    /*00000274*/ ADD         R3, R3, #4 
    /*00000278*/ ADD         IP, R0, #12 
    /*0000027c*/ STR         R3, [R0, #16] 
    /*00000280*/ STR         R2, [R1, #1304]             @ 0x518 
    /*00000284*/ STR         IP, [R1, #1300]             @ 0x514 
    /*00000288*/ STR         R3, [R2] 
    /*0000028c*/ LDR         R3, [R0, #8] 
    /*00000290*/ ADD         R3, R3, #1 
    /*00000294*/ STR         R3, [R0, #8] 
    /*00000298*/ BX          LR 
FUNC_END DIS_PutNodeToFreeList


FUNC_BEGIN DIS_GetHeadNodeFromBusyList
    /*0000029c*/ MOV         R3, R0 
    /*000002a0*/ PUSH        {R4, R5, R6, LR} 
    /*000002a4*/ LDR         R4, [R3, #20]! 
    /*000002a8*/ CMP         R4, R3 
    /*000002ac*/ BEQ         DIS_GetHeadNodeFromBusyList_x318
    /*000002b0*/ CMP         R4, #0 
    /*000002b4*/ MOV         R5, R0 
    /*000002b8*/ BEQ         DIS_GetHeadNodeFromBusyList_x2f8
    DIS_GetHeadNodeFromBusyList_x2bc:
    /*000002bc*/ LDR         R1, [R4, #4] 
    /*000002c0*/ MOV         R2, #256                    @ 0x100 
    /*000002c4*/ LDR         IP, [R4] 
    /*000002c8*/ MOVT        R2, #16 
    /*000002cc*/ MOV         R3, #512                    @ 0x200 
    /*000002d0*/ MOVT        R3, #32 
    /*000002d4*/ SUB         R0, R4, #1296               @ 0x510 
    /*000002d8*/ STR         R1, [IP, #4] 
    /*000002dc*/ SUB         R0, R0, #4 
    /*000002e0*/ STR         IP, [R1] 
    /*000002e4*/ STM         R4, {R2, R3} 
    /*000002e8*/ LDR         R3, [R5, #4] 
    /*000002ec*/ SUB         R3, R3, #1 
    /*000002f0*/ STR         R3, [R5, #4] 
    /*000002f4*/ POP         {R4, R5, R6, PC} 
    DIS_GetHeadNodeFromBusyList_x2f8:
    /*000002f8*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7c] 
    /*000002fc*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x80] 
    /*00000300*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7c] 
    /*00000304*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x80] 
    /*00000308*/ MOV         R2, #142                    @ 0x8e 
    /*0000030c*/ LDR         R1, [PC, #12]               @ 0x0000000000000320 
    /*00000310*/ BL          0x0000000000000310          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00000314*/ B           DIS_GetHeadNodeFromBusyList_x2bc
    DIS_GetHeadNodeFromBusyList_x318:
    /*00000318*/ MOV         R0, #0 
    /*0000031c*/ POP         {R4, R5, R6, PC} 
    /*00000320*/ ANDEQ       R0, R0, R8, ASR R0 
FUNC_END DIS_GetHeadNodeFromBusyList


FUNC_BEGIN DIS_GetNodeFromBusyList
    /*00000324*/ LDR         R2, [R0, #20] 
    /*00000328*/ ADD         R3, R0, #20 
    /*0000032c*/ CMP         R2, R3 
    /*00000330*/ BXEQ        LR 
    /*00000334*/ LDR         IP, [R1, #1304]             @ 0x518 
    /*00000338*/ MOV         R2, #256                    @ 0x100 
    /*0000033c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000340*/ MOVT        R2, #16 
    /*00000344*/ LDR         LR, [R1, #1300]             @ 0x514 
    /*00000348*/ MOV         R3, #512                    @ 0x200 
    /*0000034c*/ MOVT        R3, #32 
    /*00000350*/ STR         IP, [LR, #4] 
    /*00000354*/ STR         LR, [IP] 
    /*00000358*/ STR         R2, [R1, #1300]             @ 0x514 
    /*0000035c*/ STR         R3, [R1, #1304]             @ 0x518 
    /*00000360*/ LDR         R3, [R0, #4] 
    /*00000364*/ SUB         R3, R3, #1 
    /*00000368*/ STR         R3, [R0, #4] 
    /*0000036c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END DIS_GetNodeFromBusyList


FUNC_BEGIN DIS_QueryNextNode
    /*00000370*/ ADD         R3, R0, #1296               @ 0x510 
    /*00000374*/ LDR         R0, [R0, #1300]             @ 0x514 
    /*00000378*/ ADD         R3, R3, #4 
    /*0000037c*/ CMP         R0, R3 
    /*00000380*/ CMPNE       R0, #0 
    /*00000384*/ BEQ         DIS_QueryNextNode_x3a0
    /*00000388*/ ADD         R1, R1, #20 
    /*0000038c*/ CMP         R0, R1 
    /*00000390*/ BEQ         DIS_QueryNextNode_x3a0
    /*00000394*/ SUB         R0, R0, #1296               @ 0x510 
    /*00000398*/ SUB         R0, R0, #4 
    /*0000039c*/ BX          LR 
    DIS_QueryNextNode_x3a0:
    /*000003a0*/ MOV         R0, #0 
    /*000003a4*/ BX          LR 
FUNC_END DIS_QueryNextNode


FUNC_BEGIN DIS_QueryPrevNode
    /*000003a8*/ ADD         R3, R0, #1296               @ 0x510 
    /*000003ac*/ LDR         R2, [R0, #1300]             @ 0x514 
    /*000003b0*/ ADD         R3, R3, #4 
    /*000003b4*/ CMP         R2, R3 
    /*000003b8*/ BEQ         DIS_QueryPrevNode_x3e0
    /*000003bc*/ LDR         R0, [R0, #1304]             @ 0x518 
    /*000003c0*/ CMP         R0, #0 
    /*000003c4*/ BXEQ        LR 
    /*000003c8*/ ADD         R1, R1, #20 
    /*000003cc*/ CMP         R0, R1 
    /*000003d0*/ BEQ         DIS_QueryPrevNode_x3e0
    /*000003d4*/ SUB         R0, R0, #1296               @ 0x510 
    /*000003d8*/ SUB         R0, R0, #4 
    /*000003dc*/ BX          LR 
    DIS_QueryPrevNode_x3e0:
    /*000003e0*/ MOV         R0, #0 
    /*000003e4*/ BX          LR 
FUNC_END DIS_QueryPrevNode


FUNC_BEGIN DIS_PutNodeToBusyList
    /*000003e8*/ PUSH        {R4, R5, R6, LR} 
    /*000003ec*/ MOV         R5, R1 
    /*000003f0*/ LDR         R3, [R1, #672]              @ 0x2a0 
    /*000003f4*/ MOV         R4, R0 
    /*000003f8*/ CMP         R3, #0 
    /*000003fc*/ BEQ         DIS_PutNodeToBusyList_x40c
    /*00000400*/ MOV         R1, #11 
    /*00000404*/ ADD         R0, R5, #336                @ 0x150 
    /*00000408*/ BL          0x0000000000000408          @ DIS_VbAdd [R_ARM_CALL: .text+0x898] 
    DIS_PutNodeToBusyList_x40c:
    /*0000040c*/ LDR         R2, [R4, #24] 
    /*00000410*/ ADD         R3, R5, #1296               @ 0x510 
    /*00000414*/ ADD         R3, R3, #4 
    /*00000418*/ ADD         R1, R4, #20 
    /*0000041c*/ STR         R3, [R4, #24] 
    /*00000420*/ STR         R2, [R5, #1304]             @ 0x518 
    /*00000424*/ STR         R1, [R5, #1300]             @ 0x514 
    /*00000428*/ STR         R3, [R2] 
    /*0000042c*/ LDR         R3, [R4, #4] 
    /*00000430*/ ADD         R3, R3, #1 
    /*00000434*/ STR         R3, [R4, #4] 
    /*00000438*/ POP         {R4, R5, R6, PC} 
FUNC_END DIS_PutNodeToBusyList


FUNC_BEGIN DIS_GetBusyNodeNum
    /*0000043c*/ LDR         R0, [R0, #4] 
    /*00000440*/ BX          LR 
FUNC_END DIS_GetBusyNodeNum


FUNC_BEGIN DIS_GetFreeNodeNum
    /*00000444*/ LDR         R0, [R0, #8] 
    /*00000448*/ BX          LR 
FUNC_END DIS_GetFreeNodeNum


FUNC_BEGIN DIS_GetVerMagic
    /*0000044c*/ MOVW        R0, #61479                  @ 0xf027 
    /*00000450*/ MOVT        R0, #307                    @ 0x133 
    /*00000454*/ BX          LR 
FUNC_END DIS_GetVerMagic


FUNC_BEGIN DIS_QueryState
    /*00000458*/ PUSH        {R4, LR} 
    /*0000045c*/ MOV         R4, R0 
    /*00000460*/ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000464*/ MOVT        R0, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000468*/ BL          0x0000000000000468          @ osal_atomic_read [R_ARM_CALL: +0x0] 
    /*0000046c*/ CMP         R0, #0 
    /*00000470*/ MOVEQ       R3, #17 
    /*00000474*/ MOVNE       R3, #34                     @ 0x22 
    /*00000478*/ STR         R3, [R4] 
    /*0000047c*/ POP         {R4, PC} 
FUNC_END DIS_QueryState


FUNC_BEGIN DIS_WaitEventCallback
    /*00000480*/ CMP         R0, #0 
    /*00000484*/ BEQ         DIS_WaitEventCallback_x49c
    /*00000488*/ LDR         R3, [R0] 
    /*0000048c*/ LDR         R0, [R3] 
    /*00000490*/ CLZ         R0, R0 
    /*00000494*/ LSR         R0, R0, #5 
    /*00000498*/ BX          LR 
    DIS_WaitEventCallback_x49c:
    /*0000049c*/ PUSH        {R4, LR} 
    /*000004a0*/ SUB         SP, SP, #8 
    /*000004a4*/ MOV         R4, R0 
    /*000004a8*/ MOVW        R2, #386                    @ 0x182 
    /*000004ac*/ MOV         R1, #17 
    /*000004b0*/ STR         R2, [SP] 
    /*000004b4*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x74] 
    /*000004b8*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc0] 
    /*000004bc*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x74] 
    /*000004c0*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc0] 
    /*000004c4*/ MOV         R0, #3 
    /*000004c8*/ BL          0x00000000000004C8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000004cc*/ MOV         R0, R4 
    /*000004d0*/ ADD         SP, SP, #8 
    /*000004d4*/ POP         {R4, PC} 
FUNC_END DIS_WaitEventCallback


FUNC_BEGIN DIS_Init
    /*000004d8*/ PUSH        {R4, R5, LR} 
    /*000004dc*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000004e0*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000004e4*/ SUB         SP, SP, #12 
    /*000004e8*/ LDR         R3, [R4, #4] 
    /*000004ec*/ CMP         R3, #1 
    /*000004f0*/ MOVEQ       R5, #0 
    /*000004f4*/ BEQ         DIS_Init_x554
    /*000004f8*/ ADD         R0, R4, #8 
    /*000004fc*/ BL          0x00000000000004FC          @ osal_atomic_init [R_ARM_CALL: +0x0] 
    /*00000500*/ SUBS        R5, R0, #0 
    /*00000504*/ BNE         DIS_Init_x560
    /*00000508*/ MOV         R1, R5 
    /*0000050c*/ ADD         R0, R4, #8 
    /*00000510*/ BL          0x0000000000000510          @ osal_atomic_set [R_ARM_CALL: +0x0] 
    /*00000514*/ BL          0x0000000000000514          @ DIS_DRV_SOFTRSTRegInit [R_ARM_CALL: .text+0x5948] 
    /*00000518*/ MOV         R0, R5 
    /*0000051c*/ MOV         R1, #1 
    /*00000520*/ BL          0x0000000000000520          @ DIS_DRV_ClkEnable [R_ARM_CALL: .text+0x60f0] 
    /*00000524*/ MOVW        R3, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x0] 
    /*00000528*/ MOVT        R3, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x0] 
    /*0000052c*/ LDR         R0, [R3] 
    /*00000530*/ BL          0x0000000000000530          @ DIS_DrvCommOpen [R_ARM_CALL: .text+0x55f4] 
    /*00000534*/ SUBS        R5, R0, #0 
    /*00000538*/ BNE         DIS_Init_x590
    /*0000053c*/ MOV         R3, #1 
    /*00000540*/ STR         R3, [R4, #4] 
    /*00000544*/ BL          0x0000000000000544          @ DIS_DRV_Reset [R_ARM_CALL: .text+0x6100] 
    /*00000548*/ MOV         R1, R5 
    /*0000054c*/ MOV         R0, R5 
    /*00000550*/ BL          0x0000000000000550          @ DIS_DRV_ClkEnable [R_ARM_CALL: .text+0x60f0] 
    DIS_Init_x554:
    /*00000554*/ MOV         R0, R5 
    /*00000558*/ ADD         SP, SP, #12 
    /*0000055c*/ POP         {R4, R5, PC} 
    DIS_Init_x560:
    /*00000560*/ STR         R5, [SP, #4] 
    /*00000564*/ MOVW        R3, #2436                   @ 0x984 
    /*00000568*/ MOV         R1, #17 
    /*0000056c*/ STR         R3, [SP] 
    /*00000570*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xec] 
    /*00000574*/ LDR         R3, [PC, #68]               @ 0x00000000000005c0 
    /*00000578*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0xec] 
    /*0000057c*/ MOV         R0, #3 
    /*00000580*/ BL          0x0000000000000580          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000584*/ MOV         R0, R5 
    /*00000588*/ ADD         SP, SP, #12 
    /*0000058c*/ POP         {R4, R5, PC} 
    DIS_Init_x590:
    /*00000590*/ STR         R5, [SP, #4] 
    /*00000594*/ MOVW        R3, #2449                   @ 0x991 
    /*00000598*/ MOV         R1, #17 
    /*0000059c*/ STR         R3, [SP] 
    /*000005a0*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x120] 
    /*000005a4*/ LDR         R3, [PC, #20]               @ 0x00000000000005c0 
    /*000005a8*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x120] 
    /*000005ac*/ MOV         R0, #3 
    /*000005b0*/ BL          0x00000000000005B0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000005b4*/ MOV         R0, R5 
    /*000005b8*/ ADD         SP, SP, #12 
    /*000005bc*/ POP         {R4, R5, PC} 
    /*000005c0*/ ANDEQ       R0, R0, IP, LSL #1 
FUNC_END DIS_Init


FUNC_BEGIN DIS_CheckNullPtr.part.2
    /*000005c4*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000005c8*/ SUB         SP, SP, #12 
    /*000005cc*/ MOV         R3, #84                     @ 0x54 
    /*000005d0*/ MOV         R1, #17 
    /*000005d4*/ STR         R3, [SP] 
    /*000005d8*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x150] 
    /*000005dc*/ LDR         R3, [PC, #24]               @ 0x00000000000005fc 
    /*000005e0*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x150] 
    /*000005e4*/ MOV         R0, #3 
    /*000005e8*/ BL          0x00000000000005E8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000005ec*/ MOVW        R0, #32774                  @ 0x8006 
    /*000005f0*/ MOVT        R0, #40977                  @ 0xa011 
    /*000005f4*/ ADD         SP, SP, #12 
    /*000005f8*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*000005fc*/ MULEQ       R0, R8, R0 
FUNC_END DIS_CheckNullPtr.part.2


FUNC_BEGIN DIS_CheckChnId.part.3
    /*00000600*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000604*/ SUB         SP, SP, #20 
    /*00000608*/ MOV         R3, #3 
    /*0000060c*/ MOV         R1, #17 
    /*00000610*/ STMIB       SP, {R0, R3} 
    /*00000614*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x178] 
    /*00000618*/ MOV         R0, R3 
    /*0000061c*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x178] 
    /*00000620*/ MOV         R3, #94                     @ 0x5e 
    /*00000624*/ STR         R3, [SP] 
    /*00000628*/ LDR         R3, [PC, #16]               @ 0x0000000000000640 
    /*0000062c*/ BL          0x000000000000062C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000630*/ MOVW        R0, #32770                  @ 0x8002 
    /*00000634*/ MOVT        R0, #40977                  @ 0xa011 
    /*00000638*/ ADD         SP, SP, #20 
    /*0000063c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00000640*/ ANDEQ       R0, R0, IP, LSR #1 
FUNC_END DIS_CheckChnId.part.3


FUNC_BEGIN DIS_GetDSFCfg
    /*00000644*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00000648*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000064c*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000650*/ SUB         SP, SP, #20 
    /*00000654*/ MOV         R2, #0 
    /*00000658*/ LDR         R3, [R4, #4] 
    /*0000065c*/ STR         R2, [SP, #12] 
    /*00000660*/ CMP         R3, #1 
    /*00000664*/ BNE         DIS_GetDSFCfg_x750
    /*00000668*/ CMP         R0, #3 
    /*0000066c*/ MOV         R6, R1 
    /*00000670*/ MOV         R5, R0 
    /*00000674*/ BHI         DIS_GetDSFCfg_x6d8
    DIS_GetDSFCfg_x678:
    /*00000678*/ CMP         R6, #0 
    /*0000067c*/ BEQ         DIS_GetDSFCfg_x77c
    DIS_GetDSFCfg_x680:
    /*00000680*/ LDR         R0, [PC, #272]              @ 0x0000000000000798 
    /*00000684*/ STR         R6, [SP, #12] 
    /*00000688*/ BL          0x0000000000000688          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*0000068c*/ SUBS        R7, R0, #0 
    /*00000690*/ BNE         DIS_GetDSFCfg_x78c
    /*00000694*/ ADD         R3, R4, R5, LSL #2 
    /*00000698*/ LDR         R3, [R3, #16] 
    /*0000069c*/ CMP         R3, #0 
    /*000006a0*/ BEQ         DIS_GetDSFCfg_x720
    /*000006a4*/ MOV         R3, #20 
    /*000006a8*/ MLA         R4, R3, R5, R4 
    /*000006ac*/ LDR         R3, [R4, #40]               @ 0x28 
    /*000006b0*/ CMP         R3, #0 
    /*000006b4*/ BEQ         DIS_GetDSFCfg_x6f0
    /*000006b8*/ MOV         R0, R5 
    /*000006bc*/ ADD         R1, SP, #12 
    /*000006c0*/ BL          0x00000000000006C0          @ DIS_DrvGetDSFConfig [R_ARM_CALL: .text+0x5e5c] 
    DIS_GetDSFCfg_x6c4:
    /*000006c4*/ LDR         R0, [PC, #204]              @ 0x0000000000000798 
    /*000006c8*/ BL          0x00000000000006C8          @ osal_up [R_ARM_CALL: +0x0] 
    /*000006cc*/ MOV         R0, R7 
    /*000006d0*/ ADD         SP, SP, #20 
    /*000006d4*/ POP         {R4, R5, R6, R7, PC} 
    DIS_GetDSFCfg_x6d8:
    /*000006d8*/ BL          0x0000000000000600 
    /*000006dc*/ SUBS        R7, R0, #0 
    /*000006e0*/ BEQ         DIS_GetDSFCfg_x678
    DIS_GetDSFCfg_x6e4:
    /*000006e4*/ MOV         R0, R7 
    /*000006e8*/ ADD         SP, SP, #20 
    /*000006ec*/ POP         {R4, R5, R6, R7, PC} 
    DIS_GetDSFCfg_x6f0:
    /*000006f0*/ MOVW        R3, #585                    @ 0x249 
    /*000006f4*/ STR         R5, [SP, #4] 
    /*000006f8*/ STR         R3, [SP] 
    /*000006fc*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x210] 
    /*00000700*/ LDR         R3, [PC, #148]              @ 0x000000000000079c 
    /*00000704*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x210] 
    /*00000708*/ MOV         R1, #17 
    /*0000070c*/ MOV         R0, #3 
    /*00000710*/ MOVW        R7, #32777                  @ 0x8009 
    /*00000714*/ BL          0x0000000000000714          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000718*/ MOVT        R7, #40977                  @ 0xa011 
    /*0000071c*/ B           DIS_GetDSFCfg_x6c4
    DIS_GetDSFCfg_x720:
    /*00000720*/ MOVW        R3, #578                    @ 0x242 
    /*00000724*/ STR         R5, [SP, #4] 
    /*00000728*/ STR         R3, [SP] 
    /*0000072c*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e0] 
    /*00000730*/ LDR         R3, [PC, #100]              @ 0x000000000000079c 
    /*00000734*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e0] 
    /*00000738*/ MOV         R1, #17 
    /*0000073c*/ MOV         R0, #3 
    /*00000740*/ MOVW        R7, #32773                  @ 0x8005 
    /*00000744*/ BL          0x0000000000000744          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000748*/ MOVT        R7, #40977                  @ 0xa011 
    /*0000074c*/ B           DIS_GetDSFCfg_x6c4
    DIS_GetDSFCfg_x750:
    /*00000750*/ MOVW        R3, #567                    @ 0x237 
    /*00000754*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b0] 
    /*00000758*/ STR         R3, [SP] 
    /*0000075c*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b0] 
    /*00000760*/ LDR         R3, [PC, #52]               @ 0x000000000000079c 
    /*00000764*/ MOV         R1, #17 
    /*00000768*/ MOV         R0, #3 
    /*0000076c*/ MOVW        R7, #32784                  @ 0x8010 
    /*00000770*/ BL          0x0000000000000770          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000774*/ MOVT        R7, #40977                  @ 0xa011 
    /*00000778*/ B           DIS_GetDSFCfg_x6e4
    DIS_GetDSFCfg_x77c:
    /*0000077c*/ BL          0x00000000000005C4 
    /*00000780*/ SUBS        R7, R0, #0 
    /*00000784*/ BNE         DIS_GetDSFCfg_x6e4
    /*00000788*/ B           DIS_GetDSFCfg_x680
    DIS_GetDSFCfg_x78c:
    /*0000078c*/ MOV         R7, #65024                  @ 0xfe00 
    /*00000790*/ MOVT        R7, #65535                  @ 0xffff 
    /*00000794*/ B           DIS_GetDSFCfg_x6e4
    /*00000798*/ ANDEQ       R0, R0, IP 
    /*0000079c*/ STRHEQ      R0, [R0], -IP 
FUNC_END DIS_GetDSFCfg


FUNC_BEGIN DIS_CheckNullPtr
    /*000007a0*/ CMP         R0, #0 
    /*000007a4*/ BEQ         DIS_CheckNullPtr_x7b0
    /*000007a8*/ MOV         R0, #0 
    /*000007ac*/ BX          LR 
    DIS_CheckNullPtr_x7b0:
    /*000007b0*/ B           0x00000000000005C4 
FUNC_END DIS_CheckNullPtr


FUNC_BEGIN DIS_CheckChnId
    /*000007b4*/ CMP         R0, #3 
    /*000007b8*/ BHI         DIS_CheckChnId_x7c4
    /*000007bc*/ MOV         R0, #0 
    /*000007c0*/ BX          LR 
    DIS_CheckChnId_x7c4:
    /*000007c4*/ B           0x0000000000000600 
FUNC_END DIS_CheckChnId


FUNC_BEGIN DIS_VbSub
    /*000007c8*/ SUBS        R3, R0, #0 
    /*000007cc*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000007d0*/ SUB         SP, SP, #12 
    /*000007d4*/ BEQ         DIS_VbSub_x868
    /*000007d8*/ MOV         R0, #1 
    /*000007dc*/ LDR         R4, [R3, #328]              @ 0x148 
    /*000007e0*/ LDRD        R6, [R3, #120]              @ 0x78 
    /*000007e4*/ MOV         R5, R1 
    /*000007e8*/ BL          0x00000000000007E8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000007ec*/ MOV         R2, R6 
    /*000007f0*/ MOV         R3, R7 
    /*000007f4*/ LDR         R1, [R0, #84]               @ 0x54 
    /*000007f8*/ MOV         R0, R4 
    /*000007fc*/ BLX         R1 
    /*00000800*/ CMP         R0, #0 
    /*00000804*/ BEQ         DIS_VbSub_x860
    /*00000808*/ MOV         R0, #1 
    /*0000080c*/ BL          0x000000000000080C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00000810*/ STR         R5, [SP] 
    /*00000814*/ MOV         R2, R6 
    /*00000818*/ MOV         R3, R7 
    /*0000081c*/ LDR         R1, [R0, #64]               @ 0x40 
    /*00000820*/ MOV         R0, R4 
    /*00000824*/ BLX         R1 
    /*00000828*/ SUBS        R4, R0, #0 
    /*0000082c*/ BEQ         DIS_VbSub_x854
    /*00000830*/ MOV         R3, #120                    @ 0x78 
    /*00000834*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x26c] 
    /*00000838*/ STR         R3, [SP] 
    /*0000083c*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x26c] 
    /*00000840*/ STR         R4, [SP, #4] 
    /*00000844*/ MOV         R1, #17 
    /*00000848*/ LDR         R3, [PC, #68]               @ 0x0000000000000894 
    /*0000084c*/ MOV         R0, #3 
    /*00000850*/ BL          0x0000000000000850          @ HI_LOG [R_ARM_CALL: +0x0] 
    DIS_VbSub_x854:
    /*00000854*/ MOV         R0, R4 
    /*00000858*/ ADD         SP, SP, #12 
    /*0000085c*/ POP         {R4, R5, R6, R7, PC} 
    DIS_VbSub_x860:
    /*00000860*/ MVN         R4, #0 
    /*00000864*/ B           DIS_VbSub_x854
    DIS_VbSub_x868:
    /*00000868*/ MOV         R3, #108                    @ 0x6c 
    /*0000086c*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x240] 
    /*00000870*/ STR         R3, [SP] 
    /*00000874*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x240] 
    /*00000878*/ LDR         R3, [PC, #20]               @ 0x0000000000000894 
    /*0000087c*/ MOV         R1, #17 
    /*00000880*/ MOV         R0, #3 
    /*00000884*/ MOVW        R4, #32774                  @ 0x8006 
    /*00000888*/ BL          0x0000000000000888          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000088c*/ MOVT        R4, #40977                  @ 0xa011 
    /*00000890*/ B           DIS_VbSub_x854
    /*00000894*/ ANDEQ       R0, R0, IP, ASR #1 
FUNC_END DIS_VbSub


FUNC_BEGIN DIS_VbAdd
    /*00000898*/ SUBS        R3, R0, #0 
    /*0000089c*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000008a0*/ SUB         SP, SP, #12 
    /*000008a4*/ BEQ         DIS_VbAdd_x908
    /*000008a8*/ MOV         R0, #1 
    /*000008ac*/ LDR         R4, [R3, #328]              @ 0x148 
    /*000008b0*/ LDRD        R6, [R3, #120]              @ 0x78 
    /*000008b4*/ MOV         R5, R1 
    /*000008b8*/ BL          0x00000000000008B8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000008bc*/ MOV         R2, R6 
    /*000008c0*/ MOV         R3, R7 
    /*000008c4*/ LDR         R1, [R0, #84]               @ 0x54 
    /*000008c8*/ MOV         R0, R4 
    /*000008cc*/ BLX         R1 
    /*000008d0*/ CMP         R0, #0 
    /*000008d4*/ BEQ         DIS_VbAdd_x900
    /*000008d8*/ MOV         R0, #1 
    /*000008dc*/ BL          0x00000000000008DC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000008e0*/ STR         R5, [SP] 
    /*000008e4*/ MOV         R2, R6 
    /*000008e8*/ MOV         R3, R7 
    /*000008ec*/ LDR         R1, [R0, #60]               @ 0x3c 
    /*000008f0*/ MOV         R0, R4 
    /*000008f4*/ BLX         R1 
    DIS_VbAdd_x8f8:
    /*000008f8*/ ADD         SP, SP, #12 
    /*000008fc*/ POP         {R4, R5, R6, R7, PC} 
    DIS_VbAdd_x900:
    /*00000900*/ MVN         R0, #0 
    /*00000904*/ B           DIS_VbAdd_x8f8
    DIS_VbAdd_x908:
    /*00000908*/ MOV         R3, #136                    @ 0x88 
    /*0000090c*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x240] 
    /*00000910*/ STR         R3, [SP] 
    /*00000914*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x240] 
    /*00000918*/ LDR         R3, [PC, #20]               @ 0x0000000000000934 
    /*0000091c*/ MOV         R1, #17 
    /*00000920*/ MOV         R0, #3 
    /*00000924*/ BL          0x0000000000000924          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000928*/ MOVW        R0, #32774                  @ 0x8006 
    /*0000092c*/ MOVT        R0, #40977                  @ 0xa011 
    /*00000930*/ B           DIS_VbAdd_x8f8
    /*00000934*/ LDRDEQ      R0, [R0], -R8 
FUNC_END DIS_VbAdd


FUNC_BEGIN DIS_CheckConfig
    /*00000938*/ PUSH        {R4, LR} 
    /*0000093c*/ SUB         SP, SP, #8 
    /*00000940*/ LDR         R3, [R1, #8] 
    /*00000944*/ BIC         R2, R3, #2 
    /*00000948*/ CMP         R2, #8 
    /*0000094c*/ BNE         DIS_CheckConfig_x9c4
    /*00000950*/ LDR         R3, [R1, #12] 
    /*00000954*/ MOV         R4, R1 
    /*00000958*/ CMP         R3, #1 
    /*0000095c*/ BHI         DIS_CheckConfig_x984
    /*00000960*/ LDM         R1, {R0, R1} 
    /*00000964*/ BL          0x0000000000000964          @ DIS_DRV_CheckCommWH [R_ARM_CALL: .text+0x61ac] 
    /*00000968*/ CMP         R0, #0 
    /*0000096c*/ BNE         DIS_CheckConfig_x9b8
    /*00000970*/ LDR         R3, [R4, #28] 
    /*00000974*/ CMP         R3, #0 
    /*00000978*/ BNE         DIS_CheckConfig_x9dc
    DIS_CheckConfig_x97c:
    /*0000097c*/ ADD         SP, SP, #8 
    /*00000980*/ POP         {R4, PC} 
    DIS_CheckConfig_x984:
    /*00000984*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2dc] 
    /*00000988*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2dc] 
    /*0000098c*/ STR         R3, [SP, #4] 
    /*00000990*/ MOV         R3, #163                    @ 0xa3 
    /*00000994*/ STR         R3, [SP] 
    DIS_CheckConfig_x998:
    /*00000998*/ LDR         R3, [PC, #84]               @ 0x00000000000009f4 
    /*0000099c*/ MOV         R1, #17 
    /*000009a0*/ MOV         R0, #3 
    /*000009a4*/ BL          0x00000000000009A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000009a8*/ MOVW        R0, #32771                  @ 0x8003 
    /*000009ac*/ MOVT        R0, #40977                  @ 0xa011 
    /*000009b0*/ ADD         SP, SP, #8 
    /*000009b4*/ POP         {R4, PC} 
    DIS_CheckConfig_x9b8:
    /*000009b8*/ MOVW        R0, #32771                  @ 0x8003 
    /*000009bc*/ MOVT        R0, #40977                  @ 0xa011 
    /*000009c0*/ B           DIS_CheckConfig_x97c
    DIS_CheckConfig_x9c4:
    /*000009c4*/ STR         R3, [SP, #4] 
    /*000009c8*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29c] 
    /*000009cc*/ MOV         R3, #157                    @ 0x9d 
    /*000009d0*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29c] 
    /*000009d4*/ STR         R3, [SP] 
    /*000009d8*/ B           DIS_CheckConfig_x998
    DIS_CheckConfig_x9dc:
    /*000009dc*/ STR         R3, [SP, #4] 
    /*000009e0*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x318] 
    /*000009e4*/ MOV         R3, #177                    @ 0xb1 
    /*000009e8*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x318] 
    /*000009ec*/ STR         R3, [SP] 
    /*000009f0*/ B           DIS_CheckConfig_x998
    /*000009f4*/ ANDEQ       R0, R0, R4, ROR #1 
FUNC_END DIS_CheckConfig


FUNC_BEGIN DIS_ProcessInit
    /*000009f8*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*000009fc*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000a00*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000a04*/ SUB         SP, SP, #16 
    /*00000a08*/ LDR         R3, [R4, #4] 
    /*00000a0c*/ CMP         R3, #1 
    /*00000a10*/ BNE         DIS_ProcessInit_xb44
    /*00000a14*/ CMP         R0, #3 
    /*00000a18*/ MOV         R6, R1 
    /*00000a1c*/ MOV         R8, R2 
    /*00000a20*/ MOV         R5, R0 
    /*00000a24*/ BHI         DIS_ProcessInit_xaf0
    DIS_ProcessInit_xa28:
    /*00000a28*/ CMP         R6, #0 
    /*00000a2c*/ BEQ         DIS_ProcessInit_xb70
    DIS_ProcessInit_xa30:
    /*00000a30*/ CMP         R8, #0 
    /*00000a34*/ BEQ         DIS_ProcessInit_xb80
    DIS_ProcessInit_xa38:
    /*00000a38*/ LDR         R0, [PC, #556]              @ 0x0000000000000c6c 
    /*00000a3c*/ BL          0x0000000000000A3C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00000a40*/ CMP         R0, #0 
    /*00000a44*/ BNE         DIS_ProcessInit_xb38
    /*00000a48*/ ADD         R3, R4, R5, LSL #2 
    /*00000a4c*/ LDR         R9, [R3, #16] 
    /*00000a50*/ CMP         R9, #0 
    /*00000a54*/ BEQ         DIS_ProcessInit_xc3c
    /*00000a58*/ MOV         R1, R8 
    /*00000a5c*/ MOV         R0, R6 
    /*00000a60*/ BL          0x0000000000000A60          @ DIS_CheckConfig [R_ARM_CALL: .text+0x938] 
    /*00000a64*/ SUBS        R7, R0, #0 
    /*00000a68*/ BNE         DIS_ProcessInit_xadc
    /*00000a6c*/ MOV         SL, #20 
    /*00000a70*/ MUL         SL, SL, R5 
    /*00000a74*/ ADD         R4, R4, SL 
    /*00000a78*/ LDR         R3, [R4, #40]               @ 0x28 
    /*00000a7c*/ CMP         R3, #1 
    /*00000a80*/ BEQ         DIS_ProcessInit_xb08
    /*00000a84*/ ADD         R3, R9, #28 
    /*00000a88*/ MOV         R0, R6 
    /*00000a8c*/ MOV         R1, R8 
    /*00000a90*/ MOV         R6, R3 
    /*00000a94*/ MOV         R2, R3 
    /*00000a98*/ BL          0x0000000000000A98          @ DIS_DrvCalcFirmwareAttr [R_ARM_CALL: .text+0x5e0c] 
    /*00000a9c*/ MOV         R1, R6 
    /*00000aa0*/ MOV         R0, R5 
    /*00000aa4*/ BL          0x0000000000000AA4          @ DIS_DrvAlgorithmInit [R_ARM_CALL: .text+0x5e58] 
    /*00000aa8*/ SUBS        R7, R0, #0 
    /*00000aac*/ BEQ         DIS_ProcessInit_xb90
    /*00000ab0*/ MOVW        R3, #331                    @ 0x14b 
    /*00000ab4*/ STR         R5, [SP, #4] 
    /*00000ab8*/ STR         R3, [SP] 
    /*00000abc*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3b4] 
    /*00000ac0*/ LDR         R3, [PC, #424]              @ 0x0000000000000c70 
    /*00000ac4*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3b4] 
    /*00000ac8*/ MOV         R1, #17 
    /*00000acc*/ MOV         R0, #3 
    /*00000ad0*/ BL          0x0000000000000AD0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000ad4*/ MOVW        R7, #32776                  @ 0x8008 
    /*00000ad8*/ MOVT        R7, #40977                  @ 0xa011 
    DIS_ProcessInit_xadc:
    /*00000adc*/ LDR         R0, [PC, #392]              @ 0x0000000000000c6c 
    /*00000ae0*/ BL          0x0000000000000AE0          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000ae4*/ MOV         R0, R7 
    /*00000ae8*/ ADD         SP, SP, #16 
    /*00000aec*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    DIS_ProcessInit_xaf0:
    /*00000af0*/ BL          0x0000000000000600 
    /*00000af4*/ SUBS        R7, R0, #0 
    /*00000af8*/ BEQ         DIS_ProcessInit_xa28
    DIS_ProcessInit_xafc:
    /*00000afc*/ MOV         R0, R7 
    /*00000b00*/ ADD         SP, SP, #16 
    /*00000b04*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    DIS_ProcessInit_xb08:
    /*00000b08*/ MOVW        R3, #322                    @ 0x142 
    /*00000b0c*/ STR         R5, [SP, #4] 
    /*00000b10*/ STR         R3, [SP] 
    /*00000b14*/ MOVW        R2, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x37c] 
    /*00000b18*/ LDR         R3, [PC, #336]              @ 0x0000000000000c70 
    /*00000b1c*/ MOVT        R2, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x37c] 
    /*00000b20*/ MOV         R1, #17 
    /*00000b24*/ MOV         R0, #3 
    /*00000b28*/ MOVW        R7, #32777                  @ 0x8009 
    /*00000b2c*/ BL          0x0000000000000B2C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000b30*/ MOVT        R7, #40977                  @ 0xa011 
    /*00000b34*/ B           DIS_ProcessInit_xadc
    DIS_ProcessInit_xb38:
    /*00000b38*/ MOV         R7, #65024                  @ 0xfe00 
    /*00000b3c*/ MOVT        R7, #65535                  @ 0xffff 
    /*00000b40*/ B           DIS_ProcessInit_xafc
    DIS_ProcessInit_xb44:
    /*00000b44*/ MOVW        R3, #299                    @ 0x12b 
    /*00000b48*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b0] 
    /*00000b4c*/ STR         R3, [SP] 
    /*00000b50*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b0] 
    /*00000b54*/ LDR         R3, [PC, #276]              @ 0x0000000000000c70 
    /*00000b58*/ MOV         R1, #17 
    /*00000b5c*/ MOV         R0, #3 
    /*00000b60*/ MOVW        R7, #32784                  @ 0x8010 
    /*00000b64*/ BL          0x0000000000000B64          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000b68*/ MOVT        R7, #40977                  @ 0xa011 
    /*00000b6c*/ B           DIS_ProcessInit_xafc
    DIS_ProcessInit_xb70:
    /*00000b70*/ BL          0x00000000000005C4 
    /*00000b74*/ SUBS        R7, R0, #0 
    /*00000b78*/ BNE         DIS_ProcessInit_xafc
    /*00000b7c*/ B           DIS_ProcessInit_xa30
    DIS_ProcessInit_xb80:
    /*00000b80*/ BL          0x00000000000005C4 
    /*00000b84*/ SUBS        R7, R0, #0 
    /*00000b88*/ BNE         DIS_ProcessInit_xafc
    /*00000b8c*/ B           DIS_ProcessInit_xa38
    DIS_ProcessInit_xb90:
    /*00000b90*/ LDR         R6, [PC, #220]              @ 0x0000000000000c74 
    /*00000b94*/ ADD         SL, SL, #16 
    /*00000b98*/ ADD         R1, SP, #12 
    /*00000b9c*/ MOV         R5, #1 
    /*00000ba0*/ ADD         SL, R6, SL 
    /*00000ba4*/ MOV         R0, SL 
    /*00000ba8*/ BL          0x0000000000000BA8          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000bac*/ LDRH        R0, [R9, #172]              @ 0xac 
    /*00000bb0*/ LDRH        R2, [R9, #174]              @ 0xae 
    /*00000bb4*/ ADD         R3, R9, #496                @ 0x1f0 
    /*00000bb8*/ MOVW        R1, #498                    @ 0x1f2 
    /*00000bbc*/ STR         R7, [R9, #504]              @ 0x1f8 
    /*00000bc0*/ STRH        R0, [R9, R1] 
    /*00000bc4*/ ADD         R0, R9, #556                @ 0x22c 
    /*00000bc8*/ STR         R5, [R9, #532]              @ 0x214 
    /*00000bcc*/ STR         R5, [R9, #540]              @ 0x21c 
    /*00000bd0*/ STR         R5, [R9, #512]              @ 0x200 
    /*00000bd4*/ STRH        R2, [R3] 
    /*00000bd8*/ MOV         R2, #128                    @ 0x80 
    /*00000bdc*/ LDR         R1, [R9, #112]              @ 0x70 
    /*00000be0*/ LDR         R3, [R9, #108]              @ 0x6c 
    /*00000be4*/ MUL         R3, R3, R1 
    /*00000be8*/ MOV         R1, R7 
    /*00000bec*/ LSR         R3, R3, #4 
    /*00000bf0*/ STR         R3, [R9, #500]              @ 0x1f4 
    /*00000bf4*/ BL          0x0000000000000BF4          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00000bf8*/ ADD         R1, SP, #12 
    /*00000bfc*/ MOV         R0, SL 
    /*00000c00*/ STR         R5, [R4, #40]               @ 0x28 
    /*00000c04*/ BL          0x0000000000000C04          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000c08*/ SUB         R0, R6, #20 
    /*00000c0c*/ BL          0x0000000000000C0C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00000c10*/ SUB         R0, R6, #24 
    /*00000c14*/ BL          0x0000000000000C14          @ osal_atomic_read [R_ARM_CALL: +0x0] 
    /*00000c18*/ CMP         R0, #0 
    /*00000c1c*/ BNE         DIS_ProcessInit_xc30
    /*00000c20*/ MOV         R1, R5 
    /*00000c24*/ BL          0x0000000000000C24          @ DIS_DRV_ClkEnable [R_ARM_CALL: .text+0x60f0] 
    /*00000c28*/ MOV         R0, R7 
    /*00000c2c*/ BL          0x0000000000000C2C          @ DIS_DRV_Reset [R_ARM_CALL: .text+0x6100] 
    DIS_ProcessInit_xc30:
    /*00000c30*/ LDR         R0, [PC, #64]               @ 0x0000000000000c78 
    /*00000c34*/ BL          0x0000000000000C34          @ osal_atomic_inc_return [R_ARM_CALL: +0x0] 
    /*00000c38*/ B           DIS_ProcessInit_xafc
    DIS_ProcessInit_xc3c:
    /*00000c3c*/ MOVW        R3, #309                    @ 0x135 
    /*00000c40*/ STR         R5, [SP, #4] 
    /*00000c44*/ STR         R3, [SP] 
    /*00000c48*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x350] 
    /*00000c4c*/ LDR         R3, [PC, #28]               @ 0x0000000000000c70 
    /*00000c50*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x350] 
    /*00000c54*/ MOV         R1, #17 
    /*00000c58*/ MOV         R0, #3 
    /*00000c5c*/ MOVW        R7, #32773                  @ 0x8005 
    /*00000c60*/ BL          0x0000000000000C60          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000c64*/ MOVT        R7, #40977                  @ 0xa011 
    /*00000c68*/ B           DIS_ProcessInit_xadc
    /*00000c6c*/ ANDEQ       R0, R0, IP 
    /*00000c70*/ STRDEQ      R0, [R0], -R4 
    /*00000c74*/ ANDEQ       R0, R0, R0, LSR #32 
    /*00000c78*/ ANDEQ       R0, R0, R8 
FUNC_END DIS_ProcessInit


FUNC_BEGIN DIS_CheckAttr
    /*00000c7c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000c80*/ SUB         SP, SP, #28 
    /*00000c84*/ LDR         R3, [R0, #8] 
    /*00000c88*/ CMP         R3, #6 
    /*00000c8c*/ BHI         DIS_CheckAttr_xd70
    /*00000c90*/ LDR         R3, [R0, #12] 
    /*00000c94*/ CMP         R3, #1000                   @ 0x3e8 
    /*00000c98*/ BHI         DIS_CheckAttr_xdb0
    /*00000c9c*/ LDR         R1, [R0, #16] 
    /*00000ca0*/ MOV         R2, #2304                   @ 0x900 
    /*00000ca4*/ MOVT        R2, #61                     @ 0x3d 
    /*00000ca8*/ ADD         R3, R1, #1998848            @ 0x1e8000 
    /*00000cac*/ ADD         R3, R3, #1152               @ 0x480 
    /*00000cb0*/ CMP         R3, R2 
    /*00000cb4*/ BHI         DIS_CheckAttr_xdd8
    /*00000cb8*/ LDR         R3, [R0, #24] 
    /*00000cbc*/ LDR         R2, [R0, #28] 
    /*00000cc0*/ CMP         R3, #1000                   @ 0x3e8 
    /*00000cc4*/ BHI         DIS_CheckAttr_xd2c
    /*00000cc8*/ CMP         R2, #1000                   @ 0x3e8 
    /*00000ccc*/ BHI         DIS_CheckAttr_xd2c
    /*00000cd0*/ LDR         R3, [R0, #32] 
    /*00000cd4*/ LDR         R2, [R0] 
    /*00000cd8*/ CMP         R3, #1 
    /*00000cdc*/ BHI         DIS_CheckAttr_xcf4
    /*00000ce0*/ CMP         R2, #1 
    /*00000ce4*/ MOVLS       R0, #0 
    /*00000ce8*/ BHI         DIS_CheckAttr_xcf4
    DIS_CheckAttr_xcec:
    /*00000cec*/ ADD         SP, SP, #28 
    /*00000cf0*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    DIS_CheckAttr_xcf4:
    /*00000cf4*/ STR         R3, [SP, #8] 
    /*00000cf8*/ MOV         R1, #17 
    /*00000cfc*/ STR         R2, [SP, #4] 
    /*00000d00*/ MOV         R3, #228                    @ 0xe4 
    /*00000d04*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x52c] 
    /*00000d08*/ STR         R3, [SP] 
    /*00000d0c*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x52c] 
    /*00000d10*/ LDR         R3, [PC, #240]              @ 0x0000000000000e08 
    /*00000d14*/ MOV         R0, #3 
    /*00000d18*/ BL          0x0000000000000D18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000d1c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000d20*/ MOVT        R0, #40977                  @ 0xa011 
    /*00000d24*/ ADD         SP, SP, #28 
    /*00000d28*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    DIS_CheckAttr_xd2c:
    /*00000d2c*/ STR         R2, [SP, #12] 
    /*00000d30*/ MOV         R1, #1000                   @ 0x3e8 
    /*00000d34*/ STR         R3, [SP, #4] 
    /*00000d38*/ MOV         R2, #0 
    /*00000d3c*/ STR         R1, [SP, #16] 
    /*00000d40*/ MOV         R3, #221                    @ 0xdd 
    /*00000d44*/ STR         R2, [SP, #8] 
    /*00000d48*/ MOV         R1, #17 
    /*00000d4c*/ STR         R3, [SP] 
    /*00000d50*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x4cc] 
    /*00000d54*/ LDR         R3, [PC, #172]              @ 0x0000000000000e08 
    /*00000d58*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x4cc] 
    /*00000d5c*/ MOV         R0, #3 
    /*00000d60*/ BL          0x0000000000000D60          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000d64*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000d68*/ MOVT        R0, #40977                  @ 0xa011 
    /*00000d6c*/ B           DIS_CheckAttr_xcec
    DIS_CheckAttr_xd70:
    /*00000d70*/ MOV         R2, #6 
    /*00000d74*/ STR         R2, [SP, #12] 
    /*00000d78*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x3f4] 
    /*00000d7c*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x3f4] 
    /*00000d80*/ STR         R3, [SP, #4] 
    /*00000d84*/ MOV         R3, #0 
    /*00000d88*/ STR         R3, [SP, #8] 
    /*00000d8c*/ MOV         R3, #192                    @ 0xc0 
    /*00000d90*/ STR         R3, [SP] 
    DIS_CheckAttr_xd94:
    /*00000d94*/ LDR         R3, [PC, #108]              @ 0x0000000000000e08 
    /*00000d98*/ MOV         R1, #17 
    /*00000d9c*/ MOV         R0, #3 
    /*00000da0*/ BL          0x0000000000000DA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000da4*/ MOVW        R0, #32771                  @ 0x8003 
    /*00000da8*/ MOVT        R0, #40977                  @ 0xa011 
    /*00000dac*/ B           DIS_CheckAttr_xcec
    DIS_CheckAttr_xdb0:
    /*00000db0*/ MOV         R2, #1000                   @ 0x3e8 
    /*00000db4*/ STR         R3, [SP, #4] 
    /*00000db8*/ STR         R2, [SP, #12] 
    /*00000dbc*/ MOV         R3, #0 
    /*00000dc0*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x438] 
    /*00000dc4*/ STR         R3, [SP, #8] 
    /*00000dc8*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x438] 
    /*00000dcc*/ MOV         R3, #199                    @ 0xc7 
    /*00000dd0*/ STR         R3, [SP] 
    /*00000dd4*/ B           DIS_CheckAttr_xd94
    DIS_CheckAttr_xdd8:
    /*00000dd8*/ MOVW        R2, #33920                  @ 0x8480 
    /*00000ddc*/ MOVW        R3, #31616                  @ 0x7b80 
    /*00000de0*/ MOVT        R2, #30 
    /*00000de4*/ MOVT        R3, #65505                  @ 0xffe1 
    /*00000de8*/ STR         R2, [SP, #12] 
    /*00000dec*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x480] 
    /*00000df0*/ STR         R3, [SP, #8] 
    /*00000df4*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x480] 
    /*00000df8*/ MOV         R3, #207                    @ 0xcf 
    /*00000dfc*/ STR         R1, [SP, #4] 
    /*00000e00*/ STR         R3, [SP] 
    /*00000e04*/ B           DIS_CheckAttr_xd94
    /*00000e08*/ ANDEQ       R0, R0, R4, LSL #2 
FUNC_END DIS_CheckAttr


FUNC_BEGIN DIS_GetChnId
    /*00000e0c*/ LDR         R1, [PC, #92]               @ 0x0000000000000e70 
    /*00000e10*/ MOV         R3, #0 
    DIS_GetChnId_xe14:
    /*00000e14*/ LDR         R2, [R1], #4 
    /*00000e18*/ CMP         R2, #0 
    /*00000e1c*/ BEQ         DIS_GetChnId_xe64
    /*00000e20*/ ADD         R3, R3, #1 
    /*00000e24*/ CMP         R3, #4 
    /*00000e28*/ BNE         DIS_GetChnId_xe14
    /*00000e2c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000e30*/ SUB         SP, SP, #12 
    /*00000e34*/ MOV         R3, #252                    @ 0xfc 
    /*00000e38*/ MOV         R1, #17 
    /*00000e3c*/ STR         R3, [SP] 
    /*00000e40*/ MOVW        R2, #0                      @ .LC21 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x570] 
    /*00000e44*/ LDR         R3, [PC, #40]               @ 0x0000000000000e74 
    /*00000e48*/ MOVT        R2, #0                      @ .LC21 [R_ARM_MOVT_ABS: .rodata.str1.4+0x570] 
    /*00000e4c*/ MOV         R0, #3 
    /*00000e50*/ BL          0x0000000000000E50          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00000e54*/ MOVW        R0, #32786                  @ 0x8012 
    /*00000e58*/ MOVT        R0, #40977                  @ 0xa011 
    /*00000e5c*/ ADD         SP, SP, #12 
    /*00000e60*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    DIS_GetChnId_xe64:
    /*00000e64*/ STR         R3, [R0] 
    /*00000e68*/ MOV         R0, R2 
    /*00000e6c*/ BX          LR 
    /*00000e70*/ ANDEQ       R0, R0, R0, LSL R0 
    /*00000e74*/ ANDEQ       R0, R0, R4, LSL R1 
FUNC_END DIS_GetChnId


FUNC_BEGIN DIS_CalculateFirmwareAttr
    /*00000e78*/ MOV         IP, R1 
    /*00000e7c*/ MOV         R1, R2 
    /*00000e80*/ PUSH        {R4, R5, R6, LR} 
    /*00000e84*/ MOV         R2, R3 
    /*00000e88*/ MOV         R4, R3 
    /*00000e8c*/ MOV         R5, R0 
    /*00000e90*/ MOV         R0, IP 
    /*00000e94*/ BL          0x0000000000000E94          @ DIS_DrvCalcFirmwareAttr [R_ARM_CALL: .text+0x5e0c] 
    /*00000e98*/ MOV         R1, R4 
    /*00000e9c*/ MOV         R0, R5 
    /*00000ea0*/ POP         {R4, R5, R6, LR} 
    DIS_CalculateFirmwareAttr_xea4:
    /*00000ea4*/ B           DIS_CalculateFirmwareAttr_xea4@ DIS_DrvAlgorithmInit [R_ARM_JUMP24: .text+0x5e58] 
FUNC_END DIS_CalculateFirmwareAttr


FUNC_BEGIN DIS_CalculateDynamicAttr
    /*00000ea8*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000eac*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000eb0*/ ADD         R3, R3, R0, LSL #2 
    /*00000eb4*/ PUSH        {R4, LR} 
    /*00000eb8*/ LDR         R3, [R3, #16] 
    /*00000ebc*/ LDR         R2, [R1, #16] 
    /*00000ec0*/ LDR         IP, [R3, #104]              @ 0x68 
    /*00000ec4*/ STR         R2, [R3, #296]              @ 0x128 
    /*00000ec8*/ LDR         R2, [R1, #8] 
    /*00000ecc*/ CMP         IP, #0 
    /*00000ed0*/ ADD         R2, R2, #2 
    /*00000ed4*/ STR         R2, [R3, #100]              @ 0x64 
    /*00000ed8*/ LDR         R2, [R1, #20] 
    /*00000edc*/ STR         R2, [R3, #300]              @ 0x12c 
    /*00000ee0*/ LDR         R2, [R1, #12] 
    /*00000ee4*/ STR         R2, [R3, #412]              @ 0x19c 
    /*00000ee8*/ LDR         R2, [R1] 
    /*00000eec*/ STR         R2, [R3, #460]              @ 0x1cc 
    /*00000ef0*/ LDR         R2, [R1, #32] 
    /*00000ef4*/ STR         R2, [R3, #464]              @ 0x1d0 
    /*00000ef8*/ MOVEQ       R2, #1000                   @ 0x3e8 
    /*00000efc*/ LDRNE       R2, [R1, #24] 
    /*00000f00*/ STREQ       R2, [R3, #244]              @ 0xf4 
    /*00000f04*/ STRNE       R2, [R3, #244]              @ 0xf4 
    /*00000f08*/ LDRNE       R2, [R1, #28] 
    /*00000f0c*/ ADD         R1, R3, #28 
    /*00000f10*/ STR         R2, [R3, #248]              @ 0xf8 
    /*00000f14*/ BL          0x0000000000000F14          @ DIS_DrvCalcDynamicAttr [R_ARM_CALL: .text+0x5e54] 
    /*00000f18*/ MOV         R0, #0 
    /*00000f1c*/ POP         {R4, PC} 
FUNC_END DIS_CalculateDynamicAttr


FUNC_BEGIN DIS_DeInitChnCtx
    /*00000f20*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000f24*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000f28*/ PUSH        {R4, LR} 
    /*00000f2c*/ ADD         R4, R3, R0, LSL #2 
    /*00000f30*/ LDR         R0, [R4, #16] 
    /*00000f34*/ BL          0x0000000000000F34          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00000f38*/ MOV         R3, #0 
    /*00000f3c*/ STR         R3, [R4, #16] 
    /*00000f40*/ POP         {R4, PC} 
FUNC_END DIS_DeInitChnCtx


FUNC_BEGIN DIS_InitChnCtx
    /*00000f44*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00000f48*/ MOV         R1, #0 
    /*00000f4c*/ SUB         SP, SP, #20 
    /*00000f50*/ MOV         R8, R0 
    /*00000f54*/ MOV         R0, #684                    @ 0x2ac 
    /*00000f58*/ BL          0x0000000000000F58          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*00000f5c*/ SUBS        R5, R0, #0 
    /*00000f60*/ BEQ         DIS_InitChnCtx_x1018
    /*00000f64*/ MOV         R9, #20 
    /*00000f68*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00000f6c*/ MUL         R6, R9, R8 
    /*00000f70*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00000f74*/ MOV         R2, #684                    @ 0x2ac 
    /*00000f78*/ MOV         R1, #0 
    /*00000f7c*/ BL          0x0000000000000F7C          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00000f80*/ ADD         R1, SP, #12 
    /*00000f84*/ ADD         R7, R4, R6 
    /*00000f88*/ ADD         R6, R4, R6 
    /*00000f8c*/ ADD         R7, R7, #48                 @ 0x30 
    /*00000f90*/ MOV         R0, R7 
    /*00000f94*/ BL          0x0000000000000F94          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00000f98*/ MOV         R3, #1 
    /*00000f9c*/ MOV         R0, R7 
    /*00000fa0*/ STR         R3, [R6, #32] 
    /*00000fa4*/ ADD         R1, SP, #12 
    /*00000fa8*/ BL          0x0000000000000FA8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00000fac*/ MOV         R3, #0 
    /*00000fb0*/ MOV         R1, R9 
    /*00000fb4*/ MOV         R0, R5 
    /*00000fb8*/ STR         R3, [R5, #548]              @ 0x224 
    /*00000fbc*/ STR         R3, [R5, #544]              @ 0x220 
    /*00000fc0*/ BL          0x0000000000000FC0          @ DIS_InitNodeBuf [R_ARM_CALL: .text+0x0] 
    /*00000fc4*/ SUBS        R6, R0, #0 
    /*00000fc8*/ ADDEQ       R4, R4, R8, LSL #2 
    /*00000fcc*/ STREQ       R5, [R4, #16] 
    /*00000fd0*/ BNE         DIS_InitChnCtx_xfe0
    /*00000fd4*/ MOV         R0, R6 
    /*00000fd8*/ ADD         SP, SP, #20 
    /*00000fdc*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    DIS_InitChnCtx_xfe0:
    /*00000fe0*/ MOV         R1, #17 
    /*00000fe4*/ STR         R6, [SP, #4] 
    /*00000fe8*/ MOVW        R3, #547                    @ 0x223 
    /*00000fec*/ MOVW        R2, #0                      @ .LC23 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5ec] 
    /*00000ff0*/ STR         R3, [SP] 
    /*00000ff4*/ MOVT        R2, #0                      @ .LC23 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5ec] 
    /*00000ff8*/ LDR         R3, [PC, #80]               @ 0x0000000000001050 
    /*00000ffc*/ MOV         R0, #3 
    /*00001000*/ BL          0x0000000000001000          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001004*/ MOV         R0, R5 
    /*00001008*/ BL          0x0000000000001008          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000100c*/ MOV         R0, R6 
    /*00001010*/ ADD         SP, SP, #20 
    /*00001014*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    DIS_InitChnCtx_x1018:
    /*00001018*/ MOV         R2, #684                    @ 0x2ac 
    /*0000101c*/ MOVW        R3, #529                    @ 0x211 
    /*00001020*/ STR         R2, [SP, #4] 
    /*00001024*/ MOV         R1, #17 
    /*00001028*/ STR         R3, [SP] 
    /*0000102c*/ MOVW        R2, #0                      @ .LC22 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x5a4] 
    /*00001030*/ LDR         R3, [PC, #24]               @ 0x0000000000001050 
    /*00001034*/ MOVT        R2, #0                      @ .LC22 [R_ARM_MOVT_ABS: .rodata.str1.4+0x5a4] 
    /*00001038*/ MOV         R0, #3 
    /*0000103c*/ MOV         R6, R5 
    /*00001040*/ BL          0x0000000000001040          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001044*/ MOV         R0, R6 
    /*00001048*/ ADD         SP, SP, #20 
    /*0000104c*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    /*00001050*/ ANDEQ       R0, R0, R4, LSR #2 
FUNC_END DIS_InitChnCtx


FUNC_BEGIN DIS_CreateChn
    /*00001054*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00001058*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000105c*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001060*/ SUB         SP, SP, #16 
    /*00001064*/ LDR         R3, [R4, #4] 
    /*00001068*/ CMP         R3, #1 
    /*0000106c*/ BNE         DIS_CreateChn_x1178
    /*00001070*/ CMP         R0, #0 
    /*00001074*/ MOV         R5, R0 
    /*00001078*/ BEQ         DIS_CreateChn_x1168
    DIS_CreateChn_x107c:
    /*0000107c*/ LDR         R0, [PC, #288]              @ 0x00000000000011a4 
    /*00001080*/ BL          0x0000000000001080          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001084*/ CMP         R0, #0 
    /*00001088*/ BNE         DIS_CreateChn_x115c
    /*0000108c*/ MOV         R0, R5 
    /*00001090*/ BL          0x0000000000001090          @ DIS_GetChnId [R_ARM_CALL: .text+0xe0c] 
    /*00001094*/ SUBS        R6, R0, #0 
    /*00001098*/ BNE         DIS_CreateChn_x10e4
    /*0000109c*/ LDR         R0, [R5] 
    /*000010a0*/ BL          0x00000000000010A0          @ DIS_InitChnCtx [R_ARM_CALL: .text+0xf44] 
    /*000010a4*/ SUBS        R6, R0, #0 
    /*000010a8*/ BEQ         DIS_CreateChn_x110c
    /*000010ac*/ MOV         R3, #620                    @ 0x26c 
    /*000010b0*/ MOVW        R2, #0                      @ .LC25 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x650] 
    /*000010b4*/ STR         R3, [SP] 
    /*000010b8*/ MOVT        R2, #0                      @ .LC25 [R_ARM_MOVT_ABS: .rodata.str1.4+0x650] 
    /*000010bc*/ STR         R6, [SP, #4] 
    /*000010c0*/ MOV         R1, #17 
    /*000010c4*/ LDR         R3, [PC, #220]              @ 0x00000000000011a8 
    /*000010c8*/ MOV         R0, #3 
    /*000010cc*/ BL          0x00000000000010CC          @ HI_LOG [R_ARM_CALL: +0x0] 
    DIS_CreateChn_x10d0:
    /*000010d0*/ LDR         R0, [PC, #204]              @ 0x00000000000011a4 
    /*000010d4*/ BL          0x00000000000010D4          @ osal_up [R_ARM_CALL: +0x0] 
    DIS_CreateChn_x10d8:
    /*000010d8*/ MOV         R0, R6 
    /*000010dc*/ ADD         SP, SP, #16 
    /*000010e0*/ POP         {R4, R5, R6, R7, R8, PC} 
    DIS_CreateChn_x10e4:
    /*000010e4*/ MOV         R3, #612                    @ 0x264 
    /*000010e8*/ STR         R6, [SP, #4] 
    /*000010ec*/ STR         R3, [SP] 
    /*000010f0*/ MOVW        R2, #0                      @ .LC24 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x61c] 
    /*000010f4*/ LDR         R3, [PC, #172]              @ 0x00000000000011a8 
    /*000010f8*/ MOVT        R2, #0                      @ .LC24 [R_ARM_MOVT_ABS: .rodata.str1.4+0x61c] 
    /*000010fc*/ MOV         R1, #17 
    /*00001100*/ MOV         R0, #3 
    /*00001104*/ BL          0x0000000000001104          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001108*/ B           DIS_CreateChn_x10d0
    DIS_CreateChn_x110c:
    /*0000110c*/ LDR         R3, [R5] 
    /*00001110*/ MOV         R7, #20 
    /*00001114*/ LDR         R8, [PC, #144]              @ 0x00000000000011ac 
    /*00001118*/ ADD         R1, SP, #12 
    /*0000111c*/ MLA         R3, R7, R3, R4 
    /*00001120*/ STR         R6, [R3, #36]               @ 0x24 
    /*00001124*/ LDR         R0, [R5] 
    /*00001128*/ MLA         R0, R7, R0, R8 
    /*0000112c*/ ADD         R0, R0, #16 
    /*00001130*/ BL          0x0000000000001130          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001134*/ LDR         R3, [R5] 
    /*00001138*/ MOV         R2, #2 
    /*0000113c*/ ADD         R1, SP, #12 
    /*00001140*/ MLA         R4, R7, R3, R4 
    /*00001144*/ STR         R2, [R4, #32] 
    /*00001148*/ LDR         R0, [R5] 
    /*0000114c*/ MLA         R0, R7, R0, R8 
    /*00001150*/ ADD         R0, R0, #16 
    /*00001154*/ BL          0x0000000000001154          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001158*/ B           DIS_CreateChn_x10d0
    DIS_CreateChn_x115c:
    /*0000115c*/ MOV         R6, #65024                  @ 0xfe00 
    /*00001160*/ MOVT        R6, #65535                  @ 0xffff 
    /*00001164*/ B           DIS_CreateChn_x10d8
    DIS_CreateChn_x1168:
    /*00001168*/ BL          0x00000000000005C4 
    /*0000116c*/ SUBS        R6, R0, #0 
    /*00001170*/ BNE         DIS_CreateChn_x10d8
    /*00001174*/ B           DIS_CreateChn_x107c
    DIS_CreateChn_x1178:
    /*00001178*/ MOVW        R3, #602                    @ 0x25a 
    /*0000117c*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b0] 
    /*00001180*/ STR         R3, [SP] 
    /*00001184*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b0] 
    /*00001188*/ LDR         R3, [PC, #24]               @ 0x00000000000011a8 
    /*0000118c*/ MOV         R1, #17 
    /*00001190*/ MOV         R0, #3 
    /*00001194*/ MOVW        R6, #32784                  @ 0x8010 
    /*00001198*/ BL          0x0000000000001198          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000119c*/ MOVT        R6, #40977                  @ 0xa011 
    /*000011a0*/ B           DIS_CreateChn_x10d8
    /*000011a4*/ ANDEQ       R0, R0, IP 
    /*000011a8*/ ANDEQ       R0, R0, R4, LSR R1 
    /*000011ac*/ ANDEQ       R0, R0, R0, LSR #32 
FUNC_END DIS_CreateChn


FUNC_BEGIN DIS_InitNodeInfo
    /*000011b0*/ ADD         R2, R0, #464                @ 0x1d0 
    /*000011b4*/ ADD         R3, R0, #848                @ 0x350 
    /*000011b8*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*000011bc*/ MOV         R4, R0 
    /*000011c0*/ LDRD        R6, [R2, #-8] 
    /*000011c4*/ MOV         R5, #4 
    /*000011c8*/ MOV         R9, #0 
    /*000011cc*/ STRD        R6, [R3] 
    /*000011d0*/ LDR         R3, [R0, #480]              @ 0x1e0 
    /*000011d4*/ LDR         LR, [R4, #340]              @ 0x154 
    /*000011d8*/ LDR         R0, [R0, #380]              @ 0x17c 
    /*000011dc*/ STR         R3, [R4, #856]              @ 0x358 
    /*000011e0*/ STR         R0, [R4, #864]              @ 0x360 
    DIS_InitNodeInfo_x11e4:
    /*000011e4*/ LSR         R1, LR, #1 
    /*000011e8*/ LSR         R2, R0, #1 
    /*000011ec*/ SUBS        R5, R5, #1 
    /*000011f0*/ ADD         R1, R1, #1 
    /*000011f4*/ ADD         R2, R2, #15 
    /*000011f8*/ MLA         R9, LR, R0, R9 
    /*000011fc*/ BIC         LR, R1, #1 
    /*00001200*/ BIC         R0, R2, #15 
    /*00001204*/ BNE         DIS_InitNodeInfo_x11e4
    /*00001208*/ ADD         IP, R3, R9 
    /*0000120c*/ ADDS        R6, R6, R9 
    /*00001210*/ ADD         R3, R4, #944                @ 0x3b0 
    /*00001214*/ ADD         R8, R4, #1168               @ 0x490 
    /*00001218*/ ADC         R7, R7, #0 
    /*0000121c*/ ADDS        R2, R6, #32 
    /*00001220*/ STRD        R6, [R3] 
    /*00001224*/ ADD         LR, R4, #1120               @ 0x460 
    /*00001228*/ STR         IP, [R4, #952]              @ 0x3b8 
    /*0000122c*/ ADC         R3, R7, #0 
    /*00001230*/ ADD         R1, R4, #1136               @ 0x470 
    /*00001234*/ STRD        R2, [R8] 
    /*00001238*/ ADDS        R2, R6, #544                @ 0x220 
    /*0000123c*/ ADD         R3, IP, #32 
    /*00001240*/ ADD         SL, R4, #1184               @ 0x4a0 
    /*00001244*/ STR         R3, [R4, #1176]             @ 0x498 
    /*00001248*/ MOVW        R8, #4640                   @ 0x1220 
    /*0000124c*/ ADC         R3, R7, #0 
    /*00001250*/ ADDS        R8, R8, R6 
    /*00001254*/ STRD        R2, [LR, #-8] 
    /*00001258*/ ADD         R0, IP, #544                @ 0x220 
    /*0000125c*/ ADD         R2, IP, #4608               @ 0x1200 
    /*00001260*/ ADD         R3, IP, #12800              @ 0x3200 
    /*00001264*/ STR         R0, [R4, #1120]             @ 0x460 
    /*00001268*/ MOV         R9, #0 
    /*0000126c*/ ADC         R9, R9, R7 
    /*00001270*/ STRD        R8, [R1] 
    /*00001274*/ MOVW        R8, #12832                  @ 0x3220 
    /*00001278*/ ADDS        R6, R6, R8 
    /*0000127c*/ ADD         R3, R3, #32 
    /*00001280*/ MOV         R1, R5 
    /*00001284*/ ADD         R2, R2, #32 
    /*00001288*/ MOV         R9, #0 
    /*0000128c*/ STR         R2, [R4, #1144]             @ 0x478 
    /*00001290*/ ADC         R7, R7, R9 
    /*00001294*/ MOV         R2, #4096                   @ 0x1000 
    /*00001298*/ STRD        R6, [SL] 
    /*0000129c*/ STR         R3, [R4, #1192]             @ 0x4a8 
    /*000012a0*/ BL          0x00000000000012A0          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000012a4*/ MOV         R1, R5 
    /*000012a8*/ MOV         R2, #4096                   @ 0x1000 
    /*000012ac*/ LDR         R0, [R4, #1144]             @ 0x478 
    /*000012b0*/ BL          0x00000000000012B0          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000012b4*/ LDR         R0, [R4, #1176]             @ 0x498 
    /*000012b8*/ MOV         R2, #300                    @ 0x12c 
    /*000012bc*/ MOV         R1, #1 
    /*000012c0*/ ADD         R0, R0, #16 
    /*000012c4*/ BL          0x00000000000012C4          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000012c8*/ LDR         R0, [R4, #1192]             @ 0x4a8 
    /*000012cc*/ MOV         R2, #300                    @ 0x12c 
    /*000012d0*/ MOV         R1, #1 
    /*000012d4*/ POP         {R4, R5, R6, R7, R8, R9, SL, LR} 
    DIS_InitNodeInfo_x12d8:
    /*000012d8*/ B           DIS_InitNodeInfo_x12d8      @ osal_memset [R_ARM_JUMP24: +0x0] 
FUNC_END DIS_InitNodeInfo


FUNC_BEGIN DIS_SetUnitMatrixMotionData
    /*000012dc*/ MOV         R2, #0 
    /*000012e0*/ MOV         R3, #0 
    /*000012e4*/ PUSH        {R4, R5} 
    /*000012e8*/ MOV         R4, #1048576                @ 0x100000 
    /*000012ec*/ MOV         R5, #0 
    /*000012f0*/ STRD        R2, [R0, #8] 
    /*000012f4*/ STRD        R4, [R0] 
    /*000012f8*/ STRD        R4, [R0, #32] 
    /*000012fc*/ STRD        R2, [R0, #16] 
    /*00001300*/ STRD        R2, [R0, #24] 
    /*00001304*/ STRD        R2, [R0, #40]               @ 0x28 
    /*00001308*/ POP         {R4, R5} 
    /*0000130c*/ BX          LR 
FUNC_END DIS_SetUnitMatrixMotionData


FUNC_BEGIN DIS_SetUnitMotionData
    DIS_SetUnitMotionData_x1310:
    /*00001310*/ B           DIS_SetUnitMotionData_x1310 @ DIS_DrvSetUnitMotionData [R_ARM_JUMP24: .text+0x5e64] 
FUNC_END DIS_SetUnitMotionData


FUNC_BEGIN DIS_IsBypass
    /*00001314*/ LDR         R3, [R1, #672]              @ 0x2a0 
    /*00001318*/ CMP         R3, #0 
    /*0000131c*/ BEQ         DIS_IsBypass_x1350
    /*00001320*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001324*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001328*/ MOV         R2, #20 
    /*0000132c*/ MLA         R0, R2, R0, R3 
    /*00001330*/ LDR         R3, [R0, #40]               @ 0x28 
    /*00001334*/ CMP         R3, #0 
    /*00001338*/ BEQ         DIS_IsBypass_x1350
    /*0000133c*/ LDR         R0, [R1, #664]              @ 0x298 
    /*00001340*/ CMN         R0, #1 
    /*00001344*/ MOVNE       R0, #0 
    /*00001348*/ MOVEQ       R0, #1 
    /*0000134c*/ BX          LR 
    DIS_IsBypass_x1350:
    /*00001350*/ MOV         R0, #1 
    /*00001354*/ BX          LR 
FUNC_END DIS_IsBypass


FUNC_BEGIN DIS_BypassSetMotionData
    /*00001358*/ LDR         R3, [R1, #672]              @ 0x2a0 
    /*0000135c*/ SUB         R3, R3, #1 
    /*00001360*/ CMP         R3, #1 
    /*00001364*/ BXHI        LR 
    /*00001368*/ ADD         R1, R1, #680                @ 0x2a8 
    DIS_BypassSetMotionData_x136c:
    /*0000136c*/ B           DIS_BypassSetMotionData_x136c@ DIS_DrvSetUnitMotionData [R_ARM_JUMP24: .text+0x5e64] 
FUNC_END DIS_BypassSetMotionData


FUNC_BEGIN DIS_SetVgsFpdCfg
    /*00001370*/ PUSH        {R4, R5, R6, LR} 
    /*00001374*/ SUBS        R5, R0, #0 
    /*00001378*/ MOV         R4, R1 
    /*0000137c*/ BEQ         DIS_SetVgsFpdCfg_x1420
    DIS_SetVgsFpdCfg_x1380:
    /*00001380*/ CMP         R4, #0 
    /*00001384*/ BEQ         DIS_SetVgsFpdCfg_x1430
    DIS_SetVgsFpdCfg_x1388:
    /*00001388*/ LDR         R3, [R4] 
    /*0000138c*/ ADD         R1, R4, #22 
    /*00001390*/ MOV         R2, #10 
    /*00001394*/ ADD         R0, R5, #238                @ 0xee 
    /*00001398*/ STR         R3, [R5, #216]              @ 0xd8 
    /*0000139c*/ LDR         R3, [R4, #4] 
    /*000013a0*/ STR         R3, [R5, #220]              @ 0xdc 
    /*000013a4*/ LDRH        R3, [R4, #8] 
    /*000013a8*/ STRH        R3, [R5, #224]              @ 0xe0 
    /*000013ac*/ LDRH        R3, [R4, #10] 
    /*000013b0*/ STRH        R3, [R5, #226]              @ 0xe2 
    /*000013b4*/ LDRH        R3, [R4, #12] 
    /*000013b8*/ STRH        R3, [R5, #228]              @ 0xe4 
    /*000013bc*/ LDRB        R3, [R4, #14] 
    /*000013c0*/ STRB        R3, [R5, #230]              @ 0xe6 
    /*000013c4*/ LDRH        R3, [R4, #16] 
    /*000013c8*/ STRH        R3, [R5, #232]              @ 0xe8 
    /*000013cc*/ LDRB        R3, [R4, #18] 
    /*000013d0*/ STRB        R3, [R5, #234]              @ 0xea 
    /*000013d4*/ LDRH        R3, [R4, #20] 
    /*000013d8*/ STRH        R3, [R5, #236]              @ 0xec 
    /*000013dc*/ BL          0x00000000000013DC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000013e0*/ ADD         R1, R4, #32 
    /*000013e4*/ MOV         R2, #10 
    /*000013e8*/ ADD         R0, R5, #248                @ 0xf8 
    /*000013ec*/ BL          0x00000000000013EC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000013f0*/ ADD         R0, R5, #256                @ 0x100 
    /*000013f4*/ MOV         R2, #32 
    /*000013f8*/ ADD         R1, R4, #42                 @ 0x2a 
    /*000013fc*/ ADD         R0, R0, #2 
    /*00001400*/ BL          0x0000000000001400          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00001404*/ LDRH        R2, [R4, #74]               @ 0x4a 
    /*00001408*/ MOVW        R3, #290                    @ 0x122 
    /*0000140c*/ MOV         R0, #0 
    /*00001410*/ STRH        R2, [R5, R3] 
    /*00001414*/ LDR         R3, [R4, #76]               @ 0x4c 
    /*00001418*/ STR         R3, [R5, #292]              @ 0x124 
    /*0000141c*/ POP         {R4, R5, R6, PC} 
    DIS_SetVgsFpdCfg_x1420:
    /*00001420*/ BL          0x00000000000005C4 
    /*00001424*/ CMP         R0, #0 
    /*00001428*/ POPNE       {R4, R5, R6, PC} 
    /*0000142c*/ B           DIS_SetVgsFpdCfg_x1380
    DIS_SetVgsFpdCfg_x1430:
    /*00001430*/ BL          0x00000000000005C4 
    /*00001434*/ CMP         R0, #0 
    /*00001438*/ BEQ         DIS_SetVgsFpdCfg_x1388
    /*0000143c*/ POP         {R4, R5, R6, PC} 
FUNC_END DIS_SetVgsFpdCfg


FUNC_BEGIN DIS_ForceEnableFpdAndMerge
    /*00001440*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001444*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001448*/ ADD         R0, R3, R0, LSL #2 
    /*0000144c*/ MOV         R2, #1 
    /*00001450*/ LDR         R3, [R0, #16] 
    /*00001454*/ STR         R2, [R3, #512]              @ 0x200 
    /*00001458*/ STR         R2, [R3, #520]              @ 0x208 
    /*0000145c*/ BX          LR 
FUNC_END DIS_ForceEnableFpdAndMerge


FUNC_BEGIN DIS_CheckFpdStatus
    /*00001460*/ LDR         R2, [R1, #1260]             @ 0x4ec 
    /*00001464*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001468*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000146c*/ ADD         R0, R3, R0, LSL #2 
    /*00001470*/ CMP         R2, #0 
    /*00001474*/ LDR         R3, [R0, #16] 
    /*00001478*/ BEQ         DIS_CheckFpdStatus_x14a4
    /*0000147c*/ MOV         R2, #0 
    /*00001480*/ STR         R2, [R3, #508]              @ 0x1fc 
    /*00001484*/ LDR         R0, [R1, #1120]             @ 0x460 
    /*00001488*/ MOVW        R1, #498                    @ 0x1f2 
    /*0000148c*/ LDRH        R1, [R3, R1] 
    /*00001490*/ LDR         R0, [R0] 
    /*00001494*/ CMP         R0, R1 
    /*00001498*/ MOVCC       R2, #1 
    /*0000149c*/ STR         R2, [R3, #512]              @ 0x200 
    /*000014a0*/ BX          LR 
    DIS_CheckFpdStatus_x14a4:
    /*000014a4*/ LDR         R2, [R3, #508]              @ 0x1fc 
    /*000014a8*/ ADD         R2, R2, #1 
    /*000014ac*/ STR         R2, [R3, #508]              @ 0x1fc 
    /*000014b0*/ BX          LR 
FUNC_END DIS_CheckFpdStatus


FUNC_BEGIN GetpCurrInlierIdxAddr
    /*000014b4*/ LDR         R3, [R0, #1176]             @ 0x498 
    /*000014b8*/ ADD         R3, R3, #16 
    /*000014bc*/ STR         R3, [R1] 
    /*000014c0*/ BX          LR 
FUNC_END GetpCurrInlierIdxAddr


FUNC_BEGIN DIS_UpdateFpdAndMergeFlag
    /*000014c4*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000014c8*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000014cc*/ ADD         R0, R3, R0, LSL #2 
    /*000014d0*/ LDR         R2, [R1, #1144]             @ 0x478 
    /*000014d4*/ MOVW        R1, #498                    @ 0x1f2 
    /*000014d8*/ LDR         R3, [R0, #16] 
    /*000014dc*/ LDR         R2, [R2] 
    /*000014e0*/ LDRH        R1, [R3, R1] 
    /*000014e4*/ CMP         R2, R1 
    /*000014e8*/ BCS         DIS_UpdateFpdAndMergeFlag_x1500
    /*000014ec*/ LDR         R1, [R3, #528]              @ 0x210 
    /*000014f0*/ CMP         R1, #0 
    /*000014f4*/ MOVEQ       R1, #1 
    /*000014f8*/ STREQ       R1, [R3, #512]              @ 0x200 
    /*000014fc*/ STREQ       R1, [R3, #520]              @ 0x208 
    DIS_UpdateFpdAndMergeFlag_x1500:
    /*00001500*/ ADD         R1, R3, #496                @ 0x1f0 
    /*00001504*/ LDRH        R1, [R1] 
    /*00001508*/ CMP         R2, R1 
    /*0000150c*/ MOVCC       R2, #0 
    /*00001510*/ STRCC       R2, [R3, #520]              @ 0x208 
    /*00001514*/ BX          LR 
FUNC_END DIS_UpdateFpdAndMergeFlag


FUNC_BEGIN DIS_SubmmitGmeToGme
    DIS_SubmmitGmeToGme_x1518:
    /*00001518*/ B           DIS_SubmmitGmeToGme_x1518   @ DIS_DRV_SubmmitToGme [R_ARM_JUMP24: .text+0x4a04] 
FUNC_END DIS_SubmmitGmeToGme


FUNC_BEGIN DIS_PrepareSubmmitToGme
    /*0000151c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001520*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001524*/ ADD         R3, R3, R0, LSL #2 
    /*00001528*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000152c*/ MOV         R5, R0 
    /*00001530*/ LDR         R4, [R3, #16] 
    /*00001534*/ SUB         SP, SP, #8 
    /*00001538*/ MOV         R0, R4 
    /*0000153c*/ BL          0x000000000000153C          @ DIS_QueryNodeFromBusyList [R_ARM_CALL: .text+0x21c] 
    /*00001540*/ SUBS        R6, R0, #0 
    /*00001544*/ BEQ         DIS_PrepareSubmmitToGme_x15e0
    DIS_PrepareSubmmitToGme_x1548:
    /*00001548*/ MOV         R1, R4 
    /*0000154c*/ MOV         R0, R6 
    /*00001550*/ BL          0x0000000000001550          @ DIS_QueryNextNode [R_ARM_CALL: .text+0x370] 
    /*00001554*/ SUBS        R7, R0, #0 
    /*00001558*/ BEQ         DIS_PrepareSubmmitToGme_x15f0
    DIS_PrepareSubmmitToGme_x155c:
    /*0000155c*/ MOV         R2, #1 
    /*00001560*/ MOV         R1, R6 
    /*00001564*/ STR         R2, [R7, #1156]             @ 0x484 
    /*00001568*/ ADD         R3, R4, #28 
    /*0000156c*/ MOV         R2, R7 
    /*00001570*/ MOV         R0, R5 
    /*00001574*/ BL          0x0000000000001574          @ DIS_DRV_SubmmitToGme [R_ARM_CALL: .text+0x4a04] 
    /*00001578*/ SUBS        R8, R0, #0 
    /*0000157c*/ BNE         DIS_PrepareSubmmitToGme_x1598
    /*00001580*/ LDR         R3, [R4, #548]              @ 0x224 
    /*00001584*/ ADD         R3, R3, #1 
    /*00001588*/ STR         R3, [R4, #548]              @ 0x224 
    DIS_PrepareSubmmitToGme_x158c:
    /*0000158c*/ MOV         R0, R8 
    /*00001590*/ ADD         SP, SP, #8 
    /*00001594*/ POP         {R4, R5, R6, R7, R8, PC} 
    DIS_PrepareSubmmitToGme_x1598:
    /*00001598*/ MOVW        R3, #32777                  @ 0x8009 
    /*0000159c*/ MOVT        R3, #40977                  @ 0xa011 
    /*000015a0*/ CMP         R8, R3 
    /*000015a4*/ MOVW        R2, #0                      @ .LC26 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x688] 
    /*000015a8*/ MOV         R1, #17 
    /*000015ac*/ MOVT        R2, #0                      @ .LC26 [R_ARM_MOVT_ABS: .rodata.str1.4+0x688] 
    /*000015b0*/ LDREQ       R3, [R4, #548]              @ 0x224 
    /*000015b4*/ MOV         R0, #3 
    /*000015b8*/ ADDEQ       R3, R3, #1 
    /*000015bc*/ STREQ       R3, [R4, #548]              @ 0x224 
    /*000015c0*/ MOVW        R3, #1724                   @ 0x6bc 
    /*000015c4*/ STR         R5, [SP, #4] 
    /*000015c8*/ STR         R3, [SP] 
    /*000015cc*/ LDR         R3, [PC, #44]               @ 0x0000000000001600 
    /*000015d0*/ BL          0x00000000000015D0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000015d4*/ MOV         R3, #2 
    /*000015d8*/ STR         R3, [R7, #1156]             @ 0x484 
    /*000015dc*/ B           DIS_PrepareSubmmitToGme_x158c
    DIS_PrepareSubmmitToGme_x15e0:
    /*000015e0*/ BL          0x00000000000005C4 
    /*000015e4*/ SUBS        R8, R0, #0 
    /*000015e8*/ BNE         DIS_PrepareSubmmitToGme_x158c
    /*000015ec*/ B           DIS_PrepareSubmmitToGme_x1548
    DIS_PrepareSubmmitToGme_x15f0:
    /*000015f0*/ BL          0x00000000000005C4 
    /*000015f4*/ SUBS        R8, R0, #0 
    /*000015f8*/ BNE         DIS_PrepareSubmmitToGme_x158c
    /*000015fc*/ B           DIS_PrepareSubmmitToGme_x155c
    /*00001600*/ ANDEQ       R0, R0, R4, ASR #2 
FUNC_END DIS_PrepareSubmmitToGme


FUNC_BEGIN DIS_GetGmeMotionData
    /*00001604*/ PUSH        {R4, R5, R6, LR} 
    /*00001608*/ SUBS        R5, R0, #0 
    /*0000160c*/ MOV         R4, R1 
    /*00001610*/ BEQ         DIS_GetGmeMotionData_x1648
    DIS_GetGmeMotionData_x1614:
    /*00001614*/ MOV         R1, #0 
    /*00001618*/ MOV         R2, #72                     @ 0x48 
    /*0000161c*/ MOV         R0, R4 
    /*00001620*/ BL          0x0000000000001620          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00001624*/ SUB         R1, R4, #8 
    /*00001628*/ SUB         IP, R5, #4 
    /*0000162c*/ ADD         R0, R5, #20 
    DIS_GetGmeMotionData_x1630:
    /*00001630*/ LDR         R2, [IP, #4]! 
    /*00001634*/ CMP         IP, R0 
    /*00001638*/ ASR         R3, R2, #31 
    /*0000163c*/ STRD        R2, [R1, #8]! 
    /*00001640*/ BNE         DIS_GetGmeMotionData_x1630
    /*00001644*/ POP         {R4, R5, R6, PC} 
    DIS_GetGmeMotionData_x1648:
    /*00001648*/ MOVW        R3, #0                      @ .LC27 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6bc] 
    /*0000164c*/ MOVW        R0, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6d0] 
    /*00001650*/ MOVT        R3, #0                      @ .LC27 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6bc] 
    /*00001654*/ MOVT        R0, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6d0] 
    /*00001658*/ MOVW        R2, #1740                   @ 0x6cc 
    /*0000165c*/ LDR         R1, [PC, #4]                @ 0x0000000000001668 
    /*00001660*/ BL          0x0000000000001660          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00001664*/ B           DIS_GetGmeMotionData_x1614
    /*00001668*/ ANDEQ       R0, R0, IP, ASR R1 
FUNC_END DIS_GetGmeMotionData


FUNC_BEGIN DIS_GetGmeRunInfo
    /*0000166c*/ PUSH        {R4, R5, R6, LR} 
    /*00001670*/ ADD         R4, R1, #1200               @ 0x4b0 
    /*00001674*/ MOV         R3, R1 
    /*00001678*/ MOV         R5, R0 
    /*0000167c*/ MOV         R1, R2 
    /*00001680*/ MOV         R0, R3 
    /*00001684*/ MOV         R2, R4 
    /*00001688*/ BL          0x0000000000001688          @ DIS_DrvGetGMEStatus [R_ARM_CALL: .text+0x5f0c] 
    /*0000168c*/ MOV         R1, R4 
    /*00001690*/ MOV         R0, R5 
    /*00001694*/ POP         {R4, R5, R6, LR} 
    DIS_GetGmeRunInfo_x1698:
    /*00001698*/ B           DIS_GetGmeRunInfo_x1698     @ DIS_DrvCalinlierOverlapRatio [R_ARM_JUMP24: .text+0x5f98] 
FUNC_END DIS_GetGmeRunInfo


FUNC_BEGIN DIS_NotifyCaller
    /*0000169c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000016a0*/ SUB         SP, SP, #12 
    /*000016a4*/ LDR         R3, [R0, #840]              @ 0x348 
    /*000016a8*/ CMP         R3, #2 
    /*000016ac*/ BEQ         DIS_NotifyCaller_x16e0
    /*000016b0*/ LDR         LR, [R0, #824]              @ 0x338 
    /*000016b4*/ MOV         R3, #2 
    /*000016b8*/ STR         R3, [R0, #840]              @ 0x348 
    /*000016bc*/ CMP         LR, #0 
    /*000016c0*/ BEQ         DIS_NotifyCaller_x1704
    /*000016c4*/ MOV         R3, R0 
    /*000016c8*/ ADD         R0, R0, #828                @ 0x33c 
    /*000016cc*/ MOV         IP, LR 
    /*000016d0*/ LDM         R0, {R0, R1, R2} 
    /*000016d4*/ ADD         SP, SP, #12 
    /*000016d8*/ POP         {LR}                        @ (ldr lr, [sp], #4) 
    /*000016dc*/ BX          IP 
    DIS_NotifyCaller_x16e0:
    /*000016e0*/ STR         R3, [SP, #4] 
    /*000016e4*/ MOVW        R2, #0                      @ .LC29 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x710] 
    /*000016e8*/ MOVW        R3, #1868                   @ 0x74c 
    /*000016ec*/ MOVT        R2, #0                      @ .LC29 [R_ARM_MOVT_ABS: .rodata.str1.4+0x710] 
    /*000016f0*/ STR         R3, [SP] 
    /*000016f4*/ MOV         R1, #17 
    /*000016f8*/ LDR         R3, [PC, #12]               @ 0x000000000000170c 
    /*000016fc*/ MOV         R0, #7 
    /*00001700*/ BL          0x0000000000001700          @ HI_LOG [R_ARM_CALL: +0x0] 
    DIS_NotifyCaller_x1704:
    /*00001704*/ ADD         SP, SP, #12 
    /*00001708*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*0000170c*/ ANDEQ       R0, R0, R4, ROR R1 
FUNC_END DIS_NotifyCaller


FUNC_BEGIN DIS_CleanBusyList
    /*00001710*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001714*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001718*/ ADD         R0, R3, R0, LSL #2 
    /*0000171c*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00001720*/ LDR         R5, [R0, #16] 
    /*00001724*/ MOV         R7, R5 
    /*00001728*/ LDR         R3, [R7, #20]! 
    /*0000172c*/ CMP         R7, R3 
    /*00001730*/ LDR         R4, [R3] 
    /*00001734*/ POPEQ       {R4, R5, R6, R7, R8, PC} 
    /*00001738*/ SUB         R4, R4, #1296               @ 0x510 
    /*0000173c*/ MOVW        R8, #1300                   @ 0x514 
    /*00001740*/ SUB         R4, R4, #4 
    DIS_CleanBusyList_x1744:
    /*00001744*/ MOV         R0, R5 
    /*00001748*/ BL          0x0000000000001748          @ DIS_GetHeadNodeFromBusyList [R_ARM_CALL: .text+0x29c] 
    /*0000174c*/ SUBS        R6, R0, #0 
    /*00001750*/ BEQ         DIS_CleanBusyList_x1764
    /*00001754*/ BL          0x0000000000001754          @ DIS_NotifyCaller [R_ARM_CALL: .text+0x169c] 
    /*00001758*/ MOV         R1, R5 
    /*0000175c*/ MOV         R0, R6 
    /*00001760*/ BL          0x0000000000001760          @ DIS_ReleaseNodeBuf [R_ARM_CALL: .text+0xe8] 
    DIS_CleanBusyList_x1764:
    /*00001764*/ LDR         R3, [R4, #1300]             @ 0x514 
    /*00001768*/ ADD         R4, R4, R8 
    /*0000176c*/ CMP         R4, R7 
    /*00001770*/ SUB         R4, R3, #1296               @ 0x510 
    /*00001774*/ SUB         R4, R4, #4 
    /*00001778*/ BNE         DIS_CleanBusyList_x1744
    /*0000177c*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END DIS_CleanBusyList


FUNC_BEGIN DIS_ProcessDeInit
    /*00001780*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00001784*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001788*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000178c*/ SUB         SP, SP, #16 
    /*00001790*/ LDR         R3, [R4, #4] 
    /*00001794*/ CMP         R3, #0 
    /*00001798*/ BEQ         DIS_ProcessDeInit_x1944
    /*0000179c*/ CMP         R0, #3 
    /*000017a0*/ MOV         R5, R0 
    /*000017a4*/ BHI         DIS_ProcessDeInit_x18c0
    DIS_ProcessDeInit_x17a8:
    /*000017a8*/ LDR         R0, [PC, #512]              @ 0x00000000000019b0 
    /*000017ac*/ BL          0x00000000000017AC          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000017b0*/ SUBS        SL, R0, #0 
    /*000017b4*/ BNE         DIS_ProcessDeInit_x1938
    /*000017b8*/ ADD         R3, R4, R5, LSL #2 
    /*000017bc*/ LDR         R7, [R3, #16] 
    /*000017c0*/ CMP         R7, #0 
    /*000017c4*/ BEQ         DIS_ProcessDeInit_x1900
    /*000017c8*/ MOV         R9, #20 
    /*000017cc*/ LDR         R8, [PC, #480]              @ 0x00000000000019b4 
    /*000017d0*/ MUL         R9, R9, R5 
    /*000017d4*/ ADD         R1, SP, #12 
    /*000017d8*/ ADD         R6, R9, #16 
    /*000017dc*/ ADD         R4, R4, R9 
    /*000017e0*/ ADD         R6, R8, R6 
    /*000017e4*/ MOV         R0, R6 
    /*000017e8*/ BL          0x00000000000017E8          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000017ec*/ LDR         R3, [R4, #40]               @ 0x28 
    /*000017f0*/ ADD         R1, SP, #12 
    /*000017f4*/ CMP         R3, #0 
    /*000017f8*/ BEQ         DIS_ProcessDeInit_x1970
    /*000017fc*/ STR         SL, [R4, #40]               @ 0x28 
    /*00001800*/ MOV         R0, R6 
    /*00001804*/ BL          0x0000000000001804          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001808*/ LDR         R2, [R7, #548]              @ 0x224 
    /*0000180c*/ ADD         R3, R9, #4 
    /*00001810*/ CMP         R2, #0 
    /*00001814*/ ADD         R3, R8, R3 
    /*00001818*/ STR         R3, [SP, #8] 
    /*0000181c*/ BEQ         DIS_ProcessDeInit_x18d4
    DIS_ProcessDeInit_x1820:
    /*00001820*/ ADD         R9, R9, #12 
    /*00001824*/ MOVW        R4, #0                      @ DIS_WaitEventCallback [R_ARM_MOVW_ABS_NC: .text+0x480] 
    /*00001828*/ ADD         R8, R8, R9 
    /*0000182c*/ MOVT        R4, #0                      @ DIS_WaitEventCallback [R_ARM_MOVT_ABS: .text+0x480] 
    /*00001830*/ B           DIS_ProcessDeInit_x1844
    DIS_ProcessDeInit_x1834:
    /*00001834*/ BL          0x0000000000001834          @ osal_wait_uninterruptible [R_ARM_CALL: +0x0] 
    /*00001838*/ CMP         R0, #0 
    /*0000183c*/ BLT         DIS_ProcessDeInit_x185c
    /*00001840*/ LDR         R3, [SP, #8] 
    DIS_ProcessDeInit_x1844:
    /*00001844*/ LDR         R3, [R3] 
    /*00001848*/ ADD         R2, SP, #8 
    /*0000184c*/ MOV         R1, R4 
    /*00001850*/ MOV         R0, R8 
    /*00001854*/ CMP         R3, #0 
    /*00001858*/ BNE         DIS_ProcessDeInit_x1834
    DIS_ProcessDeInit_x185c:
    /*0000185c*/ MOV         R0, R5 
    /*00001860*/ BL          0x0000000000001860          @ DIS_DrvAlgorithmDeInit [R_ARM_CALL: .text+0x5e60] 
    /*00001864*/ ADD         R1, SP, #12 
    /*00001868*/ MOV         R0, R6 
    /*0000186c*/ BL          0x000000000000186C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001870*/ MOV         R0, R5 
    /*00001874*/ BL          0x0000000000001710 
    /*00001878*/ MOV         R2, #76                     @ 0x4c 
    /*0000187c*/ MOV         R1, #0 
    /*00001880*/ ADD         R0, R7, #468                @ 0x1d4 
    /*00001884*/ BL          0x0000000000001884          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00001888*/ ADD         R1, SP, #12 
    /*0000188c*/ MOV         R0, R6 
    /*00001890*/ BL          0x0000000000001890          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001894*/ LDR         R0, [PC, #276]              @ 0x00000000000019b0 
    /*00001898*/ BL          0x0000000000001898          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000189c*/ LDR         R0, [PC, #276]              @ 0x00000000000019b8 
    /*000018a0*/ BL          0x00000000000018A0          @ osal_atomic_dec_return [R_ARM_CALL: +0x0] 
    /*000018a4*/ LDR         R0, [PC, #268]              @ 0x00000000000019b8 
    /*000018a8*/ BL          0x00000000000018A8          @ osal_atomic_read [R_ARM_CALL: +0x0] 
    /*000018ac*/ SUBS        R4, R0, #0 
    /*000018b0*/ MOVNE       R0, #0 
    /*000018b4*/ BEQ         DIS_ProcessDeInit_x18e4
    DIS_ProcessDeInit_x18b8:
    /*000018b8*/ ADD         SP, SP, #16 
    /*000018bc*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    DIS_ProcessDeInit_x18c0:
    /*000018c0*/ BL          0x0000000000000600 
    /*000018c4*/ CMP         R0, #0 
    /*000018c8*/ BEQ         DIS_ProcessDeInit_x17a8
    /*000018cc*/ ADD         SP, SP, #16 
    /*000018d0*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    DIS_ProcessDeInit_x18d4:
    /*000018d4*/ LDR         R2, [R7, #544]              @ 0x220 
    /*000018d8*/ CMP         R2, #0 
    /*000018dc*/ BEQ         DIS_ProcessDeInit_x185c
    /*000018e0*/ B           DIS_ProcessDeInit_x1820
    DIS_ProcessDeInit_x18e4:
    /*000018e4*/ BL          0x00000000000018E4          @ DIS_DRV_Reset [R_ARM_CALL: .text+0x6100] 
    /*000018e8*/ MOV         R1, R4 
    /*000018ec*/ MOV         R0, R4 
    /*000018f0*/ BL          0x00000000000018F0          @ DIS_DRV_ClkEnable [R_ARM_CALL: .text+0x60f0] 
    /*000018f4*/ MOV         R0, R4 
    /*000018f8*/ ADD         SP, SP, #16 
    /*000018fc*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    DIS_ProcessDeInit_x1900:
    /*00001900*/ MOV         R3, #440                    @ 0x1b8 
    /*00001904*/ MOV         R1, #17 
    /*00001908*/ STR         R3, [SP] 
    /*0000190c*/ MOVW        R2, #0                      @ .LC30 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x744] 
    /*00001910*/ LDR         R3, [PC, #164]              @ 0x00000000000019bc 
    /*00001914*/ MOVT        R2, #0                      @ .LC30 [R_ARM_MOVT_ABS: .rodata.str1.4+0x744] 
    /*00001918*/ STR         R5, [SP, #4] 
    /*0000191c*/ MOV         R0, #3 
    /*00001920*/ BL          0x0000000000001920          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001924*/ LDR         R0, [PC, #132]              @ 0x00000000000019b0 
    /*00001928*/ BL          0x0000000000001928          @ osal_up [R_ARM_CALL: +0x0] 
    /*0000192c*/ MOVW        R0, #32773                  @ 0x8005 
    /*00001930*/ MOVT        R0, #40977                  @ 0xa011 
    /*00001934*/ B           DIS_ProcessDeInit_x18b8
    DIS_ProcessDeInit_x1938:
    /*00001938*/ MOV         R0, #65024                  @ 0xfe00 
    /*0000193c*/ MOVT        R0, #65535                  @ 0xffff 
    /*00001940*/ B           DIS_ProcessDeInit_x18b8
    DIS_ProcessDeInit_x1944:
    /*00001944*/ MOVW        R3, #433                    @ 0x1b1 
    /*00001948*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b0] 
    /*0000194c*/ STR         R3, [SP] 
    /*00001950*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b0] 
    /*00001954*/ LDR         R3, [PC, #96]               @ 0x00000000000019bc 
    /*00001958*/ MOV         R1, #17 
    /*0000195c*/ MOV         R0, #3 
    /*00001960*/ BL          0x0000000000001960          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001964*/ MOVW        R0, #32784                  @ 0x8010 
    /*00001968*/ MOVT        R0, #40977                  @ 0xa011 
    /*0000196c*/ B           DIS_ProcessDeInit_x18b8
    DIS_ProcessDeInit_x1970:
    /*00001970*/ MOV         R0, R6 
    /*00001974*/ BL          0x0000000000001974          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001978*/ SUB         R0, R8, #20 
    /*0000197c*/ BL          0x000000000000197C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001980*/ MOVW        R3, #451                    @ 0x1c3 
    /*00001984*/ STR         R5, [SP, #4] 
    /*00001988*/ MOVW        R2, #0                      @ .LC31 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x778] 
    /*0000198c*/ STR         R3, [SP] 
    /*00001990*/ MOVT        R2, #0                      @ .LC31 [R_ARM_MOVT_ABS: .rodata.str1.4+0x778] 
    /*00001994*/ LDR         R3, [PC, #32]               @ 0x00000000000019bc 
    /*00001998*/ MOV         R1, #17 
    /*0000199c*/ MOV         R0, #6 
    /*000019a0*/ BL          0x00000000000019A0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000019a4*/ MOVW        R0, #32777                  @ 0x8009 
    /*000019a8*/ MOVT        R0, #40977                  @ 0xa011 
    /*000019ac*/ B           DIS_ProcessDeInit_x18b8
    /*000019b0*/ ANDEQ       R0, R0, IP 
    /*000019b4*/ ANDEQ       R0, R0, R0, LSR #32 
    /*000019b8*/ ANDEQ       R0, R0, R8 
    /*000019bc*/ ANDEQ       R0, R0, R8, LSL #3 
FUNC_END DIS_ProcessDeInit


FUNC_BEGIN DIS_DestroyChn
    /*000019c0*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*000019c4*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000019c8*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000019cc*/ SUB         SP, SP, #20 
    /*000019d0*/ LDR         R3, [R4, #4] 
    /*000019d4*/ CMP         R3, #0 
    /*000019d8*/ BEQ         DIS_DestroyChn_x1ac4
    /*000019dc*/ CMP         R0, #3 
    /*000019e0*/ MOV         R6, R0 
    /*000019e4*/ BHI         DIS_DestroyChn_x1a6c
    DIS_DestroyChn_x19e8:
    /*000019e8*/ MOV         R5, #20 
    /*000019ec*/ LDR         R8, [PC, #252]              @ 0x0000000000001af0 
    /*000019f0*/ MUL         R5, R5, R6 
    /*000019f4*/ ADD         R1, SP, #12 
    /*000019f8*/ MOV         R9, #3 
    /*000019fc*/ ADD         R7, R5, #16 
    /*00001a00*/ ADD         R5, R4, R5 
    /*00001a04*/ ADD         R7, R8, R7 
    /*00001a08*/ MOV         R0, R7 
    /*00001a0c*/ BL          0x0000000000001A0C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001a10*/ ADD         R1, SP, #12 
    /*00001a14*/ MOV         R0, R7 
    /*00001a18*/ STR         R9, [R5, #32] 
    /*00001a1c*/ BL          0x0000000000001A1C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00001a20*/ MOV         R0, R6 
    /*00001a24*/ BL          0x0000000000001A24          @ DIS_ProcessDeInit [R_ARM_CALL: .text+0x1780] 
    /*00001a28*/ SUB         R0, R8, #20 
    /*00001a2c*/ BL          0x0000000000001A2C          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001a30*/ SUBS        R5, R0, #0 
    /*00001a34*/ BNE         DIS_DestroyChn_x1ab8
    /*00001a38*/ ADD         R4, R4, R6, LSL #2 
    /*00001a3c*/ LDR         R0, [R4, #16] 
    /*00001a40*/ CMP         R0, #0 
    /*00001a44*/ BEQ         DIS_DestroyChn_x1a80
    /*00001a48*/ BL          0x0000000000001A48          @ DIS_DeInitNodeBuf [R_ARM_CALL: .text+0x13c] 
    /*00001a4c*/ LDR         R0, [R4, #16] 
    /*00001a50*/ BL          0x0000000000001A50          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00001a54*/ SUB         R0, R8, #20 
    /*00001a58*/ STR         R5, [R4, #16] 
    /*00001a5c*/ BL          0x0000000000001A5C          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001a60*/ MOV         R0, R5 
    DIS_DestroyChn_x1a64:
    /*00001a64*/ ADD         SP, SP, #20 
    /*00001a68*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    DIS_DestroyChn_x1a6c:
    /*00001a6c*/ BL          0x0000000000000600 
    /*00001a70*/ CMP         R0, #0 
    /*00001a74*/ BEQ         DIS_DestroyChn_x19e8
    /*00001a78*/ ADD         SP, SP, #20 
    /*00001a7c*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    DIS_DestroyChn_x1a80:
    /*00001a80*/ MOVW        R3, #655                    @ 0x28f 
    /*00001a84*/ MOV         R1, #17 
    /*00001a88*/ STR         R3, [SP] 
    /*00001a8c*/ MOV         R0, R9 
    /*00001a90*/ LDR         R3, [PC, #92]               @ 0x0000000000001af4 
    /*00001a94*/ MOVW        R2, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7bc] 
    /*00001a98*/ STR         R6, [SP, #4] 
    /*00001a9c*/ MOVT        R2, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7bc] 
    /*00001aa0*/ BL          0x0000000000001AA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001aa4*/ SUB         R0, R8, #20 
    /*00001aa8*/ BL          0x0000000000001AA8          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001aac*/ MOVW        R0, #32773                  @ 0x8005 
    /*00001ab0*/ MOVT        R0, #40977                  @ 0xa011 
    /*00001ab4*/ B           DIS_DestroyChn_x1a64
    DIS_DestroyChn_x1ab8:
    /*00001ab8*/ MOV         R0, #65024                  @ 0xfe00 
    /*00001abc*/ MOVT        R0, #65535                  @ 0xffff 
    /*00001ac0*/ B           DIS_DestroyChn_x1a64
    DIS_DestroyChn_x1ac4:
    /*00001ac4*/ MOVW        R3, #642                    @ 0x282 
    /*00001ac8*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b0] 
    /*00001acc*/ STR         R3, [SP] 
    /*00001ad0*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b0] 
    /*00001ad4*/ LDR         R3, [PC, #24]               @ 0x0000000000001af4 
    /*00001ad8*/ MOV         R1, #17 
    /*00001adc*/ MOV         R0, #3 
    /*00001ae0*/ BL          0x0000000000001AE0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001ae4*/ MOVW        R0, #32784                  @ 0x8010 
    /*00001ae8*/ MOVT        R0, #40977                  @ 0xa011 
    /*00001aec*/ B           DIS_DestroyChn_x1a64
    /*00001af0*/ ANDEQ       R0, R0, R0, LSR #32 
    /*00001af4*/ MULEQ       R0, IP, R1 
FUNC_END DIS_DestroyChn


FUNC_BEGIN DIS_Notify
    /*00001af8*/ CMP         R0, #17 
    /*00001afc*/ BXNE        LR 
    /*00001b00*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00001b04*/ SUB         SP, SP, #16 
    /*00001b08*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001b0c*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001b10*/ MOV         R1, R0 
    /*00001b14*/ MOVW        R3, #2540                   @ 0x9ec 
    /*00001b18*/ MOVW        R2, #0                      @ .LC33 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7ec] 
    /*00001b1c*/ STR         R3, [SP] 
    /*00001b20*/ MOVT        R2, #0                      @ .LC33 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7ec] 
    /*00001b24*/ LDR         R3, [PC, #224]              @ 0x0000000000001c0c 
    /*00001b28*/ MOV         R0, #6 
    /*00001b2c*/ BL          0x0000000000001B2C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001b30*/ LDR         R3, [R4, #4] 
    /*00001b34*/ CMP         R3, #0 
    /*00001b38*/ BNE         DIS_Notify_x1b44
    DIS_Notify_x1b3c:
    /*00001b3c*/ ADD         SP, SP, #16 
    /*00001b40*/ POP         {R4, R5, R6, R7, R8, PC} 
    DIS_Notify_x1b44:
    /*00001b44*/ MOV         R0, #1 
    /*00001b48*/ MOV         R3, #2 
    /*00001b4c*/ STR         R3, [R4, #4] 
    /*00001b50*/ BL          0x0000000000001B50          @ DIS_DrvCommNotify [R_ARM_CALL: .text+0x58b0] 
    /*00001b54*/ ADD         R0, R4, #12 
    /*00001b58*/ BL          0x0000000000001B58          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00001b5c*/ SUBS        R7, R0, #0 
    /*00001b60*/ BNE         DIS_Notify_x1b3c
    /*00001b64*/ MOVW        R6, #0                      @ DIS_WaitEventCallback [R_ARM_MOVW_ABS_NC: .text+0x480] 
    /*00001b68*/ ADD         R8, R4, #16 
    /*00001b6c*/ MOVT        R6, #0                      @ DIS_WaitEventCallback [R_ARM_MOVT_ABS: .text+0x480] 
    /*00001b70*/ ADD         R4, R4, #48                 @ 0x30 
    DIS_Notify_x1b74:
    /*00001b74*/ LDR         R2, [R8], #4 
    /*00001b78*/ CMP         R2, #0 
    /*00001b7c*/ BEQ         DIS_Notify_x1bf0
    /*00001b80*/ LDR         R1, [R2, #548]              @ 0x224 
    /*00001b84*/ SUB         R3, R4, #12 
    /*00001b88*/ STR         R3, [SP, #12] 
    /*00001b8c*/ CMP         R1, #0 
    /*00001b90*/ BNE         DIS_Notify_x1ba0
    /*00001b94*/ LDR         R2, [R2, #544]              @ 0x220 
    /*00001b98*/ CMP         R2, #0 
    /*00001b9c*/ BEQ         DIS_Notify_x1bd0
    DIS_Notify_x1ba0:
    /*00001ba0*/ SUB         R5, R4, #4 
    /*00001ba4*/ B           DIS_Notify_x1bb8
    DIS_Notify_x1ba8:
    /*00001ba8*/ BL          0x0000000000001BA8          @ osal_wait_uninterruptible [R_ARM_CALL: +0x0] 
    /*00001bac*/ CMP         R0, #0 
    /*00001bb0*/ BLT         DIS_Notify_x1bd0
    /*00001bb4*/ LDR         R3, [SP, #12] 
    DIS_Notify_x1bb8:
    /*00001bb8*/ LDR         R3, [R3] 
    /*00001bbc*/ ADD         R2, SP, #12 
    /*00001bc0*/ MOV         R1, R6 
    /*00001bc4*/ MOV         R0, R5 
    /*00001bc8*/ CMP         R3, #0 
    /*00001bcc*/ BNE         DIS_Notify_x1ba8
    DIS_Notify_x1bd0:
    /*00001bd0*/ ADD         R1, SP, #8 
    /*00001bd4*/ MOV         R0, R4 
    /*00001bd8*/ BL          0x0000000000001BD8          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00001bdc*/ MOV         R0, R7 
    /*00001be0*/ BL          0x0000000000001710 
    /*00001be4*/ ADD         R1, SP, #8 
    /*00001be8*/ MOV         R0, R4 
    /*00001bec*/ BL          0x0000000000001BEC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    DIS_Notify_x1bf0:
    /*00001bf0*/ ADD         R7, R7, #1 
    /*00001bf4*/ ADD         R4, R4, #20 
    /*00001bf8*/ CMP         R7, #4 
    /*00001bfc*/ BNE         DIS_Notify_x1b74
    /*00001c00*/ LDR         R0, [PC, #8]                @ 0x0000000000001c10 
    /*00001c04*/ BL          0x0000000000001C04          @ osal_up [R_ARM_CALL: +0x0] 
    /*00001c08*/ B           DIS_Notify_x1b3c
    /*00001c0c*/ ANDEQ       R0, R0, IP, LSR #3 
    /*00001c10*/ ANDEQ       R0, R0, IP 
FUNC_END DIS_Notify


FUNC_BEGIN DIS_ProcessBypassTask
    /*00001c14*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00001c18*/ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001c1c*/ MOVT        R6, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001c20*/ MOV         R5, R1 
    /*00001c24*/ ADD         R3, R6, R0, LSL #2 
    /*00001c28*/ MOV         R1, #1 
    /*00001c2c*/ MOV         R2, #3 
    /*00001c30*/ SUB         SP, SP, #8 
    /*00001c34*/ LDR         R4, [R3, #16] 
    /*00001c38*/ MOV         R7, R0 
    /*00001c3c*/ LDR         R3, [R4, #592]              @ 0x250 
    /*00001c40*/ MOV         R0, R4 
    /*00001c44*/ STR         R1, [R4, #532]              @ 0x214 
    /*00001c48*/ ADD         R3, R3, R1 
    /*00001c4c*/ STR         R3, [R4, #592]              @ 0x250 
    /*00001c50*/ LDR         R3, [R5, #316]              @ 0x13c 
    /*00001c54*/ STR         R2, [R5, #1156]             @ 0x484 
    /*00001c58*/ LDR         R2, [R4, #588]              @ 0x24c 
    /*00001c5c*/ CMP         R3, #0 
    /*00001c60*/ STR         R2, [R5, #1268]             @ 0x4f4 
    /*00001c64*/ MOVNE       R2, #0 
    /*00001c68*/ STRNE       R2, [R3, #72]               @ 0x48 
    /*00001c6c*/ STRNE       R2, [R3, #232]              @ 0xe8 
    /*00001c70*/ STRNE       R2, [R3, #152]              @ 0x98 
    /*00001c74*/ BL          0x0000000000001C74          @ DIS_GetBusyNodeNum [R_ARM_CALL: .text+0x43c] 
    /*00001c78*/ SUBS        R8, R0, #0 
    /*00001c7c*/ BEQ         DIS_ProcessBypassTask_x1cd8
    /*00001c80*/ MOV         R0, R4 
    /*00001c84*/ BL          0x0000000000001C84          @ DIS_QueryNodeFromBusyList [R_ARM_CALL: .text+0x21c] 
    /*00001c88*/ SUBS        R6, R0, #0 
    /*00001c8c*/ MOVWEQ      R3, #826                    @ 0x33a 
    /*00001c90*/ MOVWEQ      R2, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x828] 
    /*00001c94*/ STREQ       R3, [SP] 
    /*00001c98*/ MOVTEQ      R2, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x828] 
    /*00001c9c*/ BEQ         DIS_ProcessBypassTask_x1d58
    /*00001ca0*/ MOV         R0, R4 
    /*00001ca4*/ BL          0x0000000000001CA4          @ DIS_GetBusyNodeNum [R_ARM_CALL: .text+0x43c] 
    /*00001ca8*/ CMP         R0, #1 
    /*00001cac*/ BNE         DIS_ProcessBypassTask_x1cbc
    /*00001cb0*/ LDR         R3, [R6, #840]              @ 0x348 
    /*00001cb4*/ CMP         R3, #2 
    /*00001cb8*/ BEQ         DIS_ProcessBypassTask_x1d14
    DIS_ProcessBypassTask_x1cbc:
    /*00001cbc*/ MOV         R0, R4 
    /*00001cc0*/ MOV         R1, R5 
    /*00001cc4*/ BL          0x0000000000001CC4          @ DIS_PutNodeToBusyList [R_ARM_CALL: .text+0x3e8] 
    /*00001cc8*/ MOV         R8, #0 
    /*00001ccc*/ MOV         R0, R8 
    /*00001cd0*/ ADD         SP, SP, #8 
    /*00001cd4*/ POP         {R4, R5, R6, R7, R8, PC} 
    DIS_ProcessBypassTask_x1cd8:
    /*00001cd8*/ MOV         R0, R5 
    /*00001cdc*/ BL          0x0000000000001CDC          @ DIS_NotifyCaller [R_ARM_CALL: .text+0x169c] 
    /*00001ce0*/ MOV         R0, R5 
    /*00001ce4*/ MOV         R1, R4 
    /*00001ce8*/ BL          0x0000000000001CE8          @ DIS_ReleaseNodeBuf [R_ARM_CALL: .text+0xe8] 
    /*00001cec*/ MOV         R0, #20 
    /*00001cf0*/ MUL         R7, R0, R7 
    /*00001cf4*/ LDR         R0, [PC, #116]              @ 0x0000000000001d70 
    /*00001cf8*/ ADD         R6, R6, R7 
    /*00001cfc*/ ADD         R0, R7, R0 
    /*00001d00*/ STR         R8, [R6, #36]               @ 0x24 
    /*00001d04*/ BL          0x0000000000001D04          @ osal_wakeup [R_ARM_CALL: +0x0] 
    DIS_ProcessBypassTask_x1d08:
    /*00001d08*/ MOV         R0, R8 
    /*00001d0c*/ ADD         SP, SP, #8 
    /*00001d10*/ POP         {R4, R5, R6, R7, R8, PC} 
    DIS_ProcessBypassTask_x1d14:
    /*00001d14*/ MOV         R0, R4 
    /*00001d18*/ BL          0x0000000000001D18          @ DIS_GetHeadNodeFromBusyList [R_ARM_CALL: .text+0x29c] 
    /*00001d1c*/ CMP         R0, #0 
    /*00001d20*/ BEQ         DIS_ProcessBypassTask_x1d48
    /*00001d24*/ MOV         R1, R4 
    /*00001d28*/ MOV         R8, #0 
    /*00001d2c*/ BL          0x0000000000001D2C          @ DIS_ReleaseNodeBuf [R_ARM_CALL: .text+0xe8] 
    /*00001d30*/ MOV         R0, R5 
    /*00001d34*/ BL          0x0000000000001D34          @ DIS_NotifyCaller [R_ARM_CALL: .text+0x169c] 
    /*00001d38*/ MOV         R1, R4 
    /*00001d3c*/ MOV         R0, R5 
    /*00001d40*/ BL          0x0000000000001D40          @ DIS_ReleaseNodeBuf [R_ARM_CALL: .text+0xe8] 
    /*00001d44*/ B           DIS_ProcessBypassTask_x1d08
    DIS_ProcessBypassTask_x1d48:
    /*00001d48*/ MOVW        R2, #0                      @ .LC35 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x858] 
    /*00001d4c*/ MOVT        R2, #0                      @ .LC35 [R_ARM_MOVT_ABS: .rodata.str1.4+0x858] 
    /*00001d50*/ MOVW        R3, #838                    @ 0x346 
    /*00001d54*/ STR         R3, [SP] 
    DIS_ProcessBypassTask_x1d58:
    /*00001d58*/ LDR         R3, [PC, #20]               @ 0x0000000000001d74 
    /*00001d5c*/ MOV         R1, #17 
    /*00001d60*/ MOVW        R8, #32782                  @ 0x800e 
    /*00001d64*/ BL          0x0000000000001D64          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00001d68*/ MOVT        R8, #40977                  @ 0xa011 
    /*00001d6c*/ B           DIS_ProcessBypassTask_x1d08
    /*00001d70*/ ANDEQ       R0, R0, IP, LSR #32 
    /*00001d74*/ @           <UNDEFINED> instruction: 0x000001b8 
FUNC_END DIS_ProcessBypassTask


FUNC_BEGIN DIS_PyramidExceptionProcess
    /*00001d78*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001d7c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001d80*/ ADD         R0, R3, R0, LSL #2 
    /*00001d84*/ PUSH        {R4, R5, R6, LR} 
    /*00001d88*/ MOV         R5, R1 
    /*00001d8c*/ LDR         R4, [R0, #16] 
    /*00001d90*/ MOV         R0, R4 
    /*00001d94*/ BL          0x0000000000001D94          @ DIS_GetNodeFromBusyList [R_ARM_CALL: .text+0x324] 
    /*00001d98*/ MOV         R0, R5 
    /*00001d9c*/ BL          0x0000000000001D9C          @ DIS_NotifyCaller [R_ARM_CALL: .text+0x169c] 
    /*00001da0*/ MOV         R1, R4 
    /*00001da4*/ MOV         R0, R5 
    /*00001da8*/ BL          0x0000000000001DA8          @ DIS_ReleaseNodeBuf [R_ARM_CALL: .text+0xe8] 
    /*00001dac*/ MOV         R0, R4 
    /*00001db0*/ BL          0x0000000000001DB0          @ DIS_GetBusyNodeNum [R_ARM_CALL: .text+0x43c] 
    /*00001db4*/ CMP         R0, #0 
    /*00001db8*/ MOVEQ       R3, #1 
    /*00001dbc*/ STREQ       R3, [R4, #532]              @ 0x214 
    /*00001dc0*/ POP         {R4, R5, R6, PC} 
FUNC_END DIS_PyramidExceptionProcess


FUNC_BEGIN DIS_GeneratePyramid
    /*00001dc4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00001dc8*/ SUB         SP, SP, #156                @ 0x9c 
    /*00001dcc*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00001dd0*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00001dd4*/ MOV         R5, R0 
    /*00001dd8*/ STR         R0, [SP, #40]               @ 0x28 
    /*00001ddc*/ ADD         R0, SP, #68                 @ 0x44 
    /*00001de0*/ MOV         R6, #0 
    /*00001de4*/ MOV         R7, R3 
    /*00001de8*/ STR         R3, [SP, #20] 
    /*00001dec*/ MOV         R4, R1 
    /*00001df0*/ STR         R2, [SP, #48]               @ 0x30 
    /*00001df4*/ STR         R6, [SP, #80]               @ 0x50 
    /*00001df8*/ STR         R6, [SP, #60]               @ 0x3c 
    /*00001dfc*/ STR         R6, [SP, #76]               @ 0x4c 
    /*00001e00*/ STR         R6, [SP, #84]               @ 0x54 
    /*00001e04*/ BL          0x0000000000001E04          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00001e08*/ ADD         R3, R7, R5, LSL #2 
    /*00001e0c*/ MOV         R0, #45                     @ 0x2d 
    /*00001e10*/ LDR         R8, [R3, #16] 
    /*00001e14*/ BL          0x0000000000001E14          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00001e18*/ SUBS        R5, R0, #0 
    /*00001e1c*/ BEQ         DIS_GeneratePyramid_x2254
    /*00001e20*/ LDR         R3, [R5] 
    /*00001e24*/ CMP         R3, R6 
    /*00001e28*/ BEQ         DIS_GeneratePyramid_x2254
    /*00001e2c*/ LDR         R3, [R5, #32] 
    /*00001e30*/ CMP         R3, R6 
    /*00001e34*/ BEQ         DIS_GeneratePyramid_x2254
    /*00001e38*/ LDR         R3, [R5, #12] 
    /*00001e3c*/ CMP         R3, R6 
    /*00001e40*/ BEQ         DIS_GeneratePyramid_x2254
    /*00001e44*/ LDR         R3, [R5, #64]               @ 0x40 
    /*00001e48*/ CMP         R3, R6 
    /*00001e4c*/ BEQ         DIS_GeneratePyramid_x2254
    /*00001e50*/ LDR         R2, [SP, #68]               @ 0x44 
    /*00001e54*/ ADD         IP, SP, #88                 @ 0x58 
    /*00001e58*/ LDR         R0, [SP, #72]               @ 0x48 
    /*00001e5c*/ MOVW        LR, #0                      @ DIS_VgsCallback [R_ARM_MOVW_ABS_NC: .text+0x2f58] 
    /*00001e60*/ LDR         R7, [R4, #1272]             @ 0x4f8 
    /*00001e64*/ MOVT        LR, #0                      @ DIS_VgsCallback [R_ARM_MOVT_ABS: .text+0x2f58] 
    /*00001e68*/ ASR         R3, R2, #31 
    /*00001e6c*/ STRD        R2, [SP, #112]              @ 0x70 
    /*00001e70*/ LDR         R2, [SP, #40]               @ 0x28 
    /*00001e74*/ ASR         R1, R0, #31 
    /*00001e78*/ STRD        R0, [SP, #120]              @ 0x78 
    /*00001e7c*/ MOV         R3, R6 
    /*00001e80*/ LDR         FP, [R8, #504]              @ 0x1f8 
    /*00001e84*/ MOV         R9, #1 
    /*00001e88*/ MOV         R0, R2 
    /*00001e8c*/ ASR         R1, R2, #31 
    /*00001e90*/ STRD        R0, [SP, #24] 
    /*00001e94*/ MOV         R1, R6 
    /*00001e98*/ STR         R6, [SP, #100]              @ 0x64 
    /*00001e9c*/ ADD         R0, SP, #64                 @ 0x40 
    /*00001ea0*/ STR         R6, [SP, #108]              @ 0x6c 
    /*00001ea4*/ STR         R7, [SP, #104]              @ 0x68 
    /*00001ea8*/ LDRD        R6, [SP, #24] 
    /*00001eac*/ STR         FP, [R4, #1264]             @ 0x4f0 
    /*00001eb0*/ STR         R4, [SP, #96]               @ 0x60 
    /*00001eb4*/ STR         R2, [SP] 
    /*00001eb8*/ MOV         R2, #17 
    /*00001ebc*/ STRD        R6, [SP, #88]               @ 0x58 
    /*00001ec0*/ STR         LR, [SP, #144]              @ 0x90 
    /*00001ec4*/ STR         R9, [SP, #140]              @ 0x8c 
    /*00001ec8*/ STR         IP, [SP, #4] 
    /*00001ecc*/ LDR         R6, [R5] 
    /*00001ed0*/ BLX         R6 
    /*00001ed4*/ SUBS        R7, R0, #0 
    /*00001ed8*/ BNE         DIS_GeneratePyramid_x2288
    /*00001edc*/ LDR         R1, [SP, #40]               @ 0x28 
    /*00001ee0*/ MOV         R3, #20 
    /*00001ee4*/ LDR         R0, [SP, #20] 
    /*00001ee8*/ LDR         R2, [R8, #544]              @ 0x220 
    /*00001eec*/ MLA         R3, R3, R1, R0 
    /*00001ef0*/ ADD         R2, R2, #1 
    /*00001ef4*/ STR         R2, [R8, #544]              @ 0x220 
    /*00001ef8*/ STR         R9, [R3, #36]               @ 0x24 
    /*00001efc*/ LDR         R3, [R5, #64]               @ 0x40 
    /*00001f00*/ BLX         R3 
    /*00001f04*/ CMP         R0, #0 
    /*00001f08*/ MOV         R9, R0 
    /*00001f0c*/ STR         R0, [SP, #76]               @ 0x4c 
    /*00001f10*/ BEQ         DIS_GeneratePyramid_x2238
    /*00001f14*/ LDR         LR, [SP, #40]               @ 0x28 
    /*00001f18*/ ADD         R1, R0, #672                @ 0x2a0 
    /*00001f1c*/ LDRD        SL, [SP, #24] 
    /*00001f20*/ MOV         IP, #17 
    /*00001f24*/ MOV         R2, #336                    @ 0x150 
    /*00001f28*/ MOV         R3, #39                     @ 0x27 
    /*00001f2c*/ ADD         R6, R0, R2 
    /*00001f30*/ STRD        SL, [R1] 
    /*00001f34*/ ADD         R1, R4, R2 
    /*00001f38*/ STR         LR, [R0, #732]              @ 0x2dc 
    /*00001f3c*/ STR         IP, [R0, #724]              @ 0x2d4 
    /*00001f40*/ STR         R7, [R0, #728]              @ 0x2d8 
    /*00001f44*/ STR         R7, [R0, #720]              @ 0x2d0 
    /*00001f48*/ STR         R3, [R4, #348]              @ 0x15c 
    /*00001f4c*/ STR         R7, [R4, #364]              @ 0x16c 
    /*00001f50*/ BL          0x0000000000001F50          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00001f54*/ ADD         R3, R4, #848                @ 0x350 
    /*00001f58*/ MOV         R0, R6 
    /*00001f5c*/ MOV         R2, #336                    @ 0x150 
    /*00001f60*/ LDRD        SL, [R3] 
    /*00001f64*/ MOV         R1, R9 
    /*00001f68*/ STRD        SL, [R9, #120]              @ 0x78 
    /*00001f6c*/ LDR         R3, [R4, #856]              @ 0x358 
    /*00001f70*/ STR         R7, [R9, #148]              @ 0x94 
    /*00001f74*/ STR         R3, [R9, #144]              @ 0x90 
    /*00001f78*/ BL          0x0000000000001F78          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00001f7c*/ LDR         R2, [R9, #44]               @ 0x2c 
    /*00001f80*/ LDR         R1, [R9, #4] 
    /*00001f84*/ ADD         IP, R4, #880                @ 0x370 
    /*00001f88*/ LDRD        SL, [R9, #120]              @ 0x78 
    /*00001f8c*/ LSR         R0, R2, #1 
    /*00001f90*/ MUL         R2, R1, R2 
    /*00001f94*/ LSR         R1, R1, #1 
    /*00001f98*/ ADD         R1, R1, #1 
    /*00001f9c*/ ADD         R0, R0, #15 
    /*00001fa0*/ BIC         R3, R0, #15 
    /*00001fa4*/ BIC         LR, R1, #1 
    /*00001fa8*/ ADDS        SL, SL, R2 
    /*00001fac*/ STR         R3, [SP, #24] 
    /*00001fb0*/ MOV         R3, #0 
    /*00001fb4*/ ADC         FP, FP, R3 
    /*00001fb8*/ STRD        SL, [R6, #120]              @ 0x78 
    /*00001fbc*/ LDRD        R0, [R9, #144]              @ 0x90 
    /*00001fc0*/ ADDS        R0, R0, R2 
    /*00001fc4*/ ADC         R1, R1, R3 
    /*00001fc8*/ MOV         R2, R0 
    /*00001fcc*/ MOV         R3, R1 
    /*00001fd0*/ STRD        R2, [R6, #144]              @ 0x90 
    /*00001fd4*/ LDR         R3, [R9] 
    /*00001fd8*/ LDR         R2, [SP, #24] 
    /*00001fdc*/ STR         LR, [R9, #340]              @ 0x154 
    /*00001fe0*/ LSR         R3, R3, #1 
    /*00001fe4*/ ADD         R3, R3, #1 
    /*00001fe8*/ STR         R2, [R9, #380]              @ 0x17c 
    /*00001fec*/ BIC         R3, R3, #1 
    /*00001ff0*/ STR         R3, [R9, #336]              @ 0x150 
    /*00001ff4*/ STRD        SL, [IP, #-8] 
    /*00001ff8*/ LDR         R3, [R6, #144]              @ 0x90 
    /*00001ffc*/ LDR         R6, [R4, #1260]             @ 0x4ec 
    /*00002000*/ STR         R3, [R4, #880]              @ 0x370 
    /*00002004*/ CMP         R6, #1 
    /*00002008*/ LDR         R3, [R9, #380]              @ 0x17c 
    /*0000200c*/ STR         R3, [R4, #888]              @ 0x378 
    /*00002010*/ LDRNE       R3, [SP, #20] 
    /*00002014*/ STRNE       R7, [R3, #332]              @ 0x14c 
    /*00002018*/ BEQ         DIS_GeneratePyramid_x21fc
    DIS_GeneratePyramid_x201c:
    /*0000201c*/ LDR         R0, [SP, #20] 
    /*00002020*/ MOV         R3, #5 
    /*00002024*/ MOV         R1, R9 
    /*00002028*/ LDR         R2, [PC, #752]              @ 0x0000000000002320 
    /*0000202c*/ STR         R3, [R0, #292]              @ 0x124 
    /*00002030*/ LDR         R0, [SP, #64]               @ 0x40 
    /*00002034*/ LDR         R3, [R5, #32] 
    /*00002038*/ BLX         R3 
    /*0000203c*/ SUBS        R7, R0, #0 
    /*00002040*/ BNE         DIS_GeneratePyramid_x21e0
    /*00002044*/ ADD         R6, SP, #76                 @ 0x4c 
    /*00002048*/ ADD         FP, R4, #896                @ 0x380 
    /*0000204c*/ ADD         R3, R4, #944                @ 0x3b0 
    /*00002050*/ STR         FP, [SP, #24] 
    /*00002054*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00002058*/ STR         R7, [SP, #52]               @ 0x34 
    DIS_GeneratePyramid_x205c:
    /*0000205c*/ LDR         R3, [R5, #64]               @ 0x40 
    /*00002060*/ BLX         R3 
    /*00002064*/ MOV         R3, R6 
    /*00002068*/ MOV         R2, #744                    @ 0x2e8 
    /*0000206c*/ ADD         R6, R6, #4 
    /*00002070*/ CMP         R0, #0 
    /*00002074*/ STR         R0, [R6] 
    /*00002078*/ MOV         R4, R0 
    /*0000207c*/ ADD         R7, R0, #336                @ 0x150 
    /*00002080*/ BEQ         DIS_GeneratePyramid_x22c0
    /*00002084*/ LDR         R1, [R3] 
    /*00002088*/ MOV         R9, #0 
    /*0000208c*/ BL          0x000000000000208C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002090*/ MOV         R2, #336                    @ 0x150 
    /*00002094*/ MOV         R1, R7 
    /*00002098*/ MOV         R0, R4 
    /*0000209c*/ BL          0x000000000000209C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000020a0*/ LDR         R8, [R4, #44]               @ 0x2c 
    /*000020a4*/ LDR         R3, [R4, #4] 
    /*000020a8*/ MOV         R2, #320                    @ 0x140 
    /*000020ac*/ LDR         IP, [R4] 
    /*000020b0*/ LSR         LR, R8, #1 
    /*000020b4*/ LDRD        R0, [R4, #120]              @ 0x78 
    /*000020b8*/ MUL         R8, R3, R8 
    /*000020bc*/ LSR         IP, IP, #1 
    /*000020c0*/ LSR         R3, R3, #1 
    /*000020c4*/ ADD         LR, LR, #15 
    /*000020c8*/ ADD         R3, R3, #1 
    /*000020cc*/ ADD         IP, IP, #1 
    /*000020d0*/ ADDS        SL, R8, R0 
    /*000020d4*/ BIC         LR, LR, #15 
    /*000020d8*/ ADC         FP, R9, R1 
    /*000020dc*/ STR         LR, [R4, #380]              @ 0x17c 
    /*000020e0*/ STRD        SL, [SP, #32] 
    /*000020e4*/ BIC         R3, R3, #1 
    /*000020e8*/ BIC         IP, IP, #1 
    /*000020ec*/ STR         R3, [R4, #340]              @ 0x154 
    /*000020f0*/ STR         IP, [R4, #336]              @ 0x150 
    /*000020f4*/ MOV         R1, #0 
    /*000020f8*/ STRD        SL, [R7, #120]              @ 0x78 
    /*000020fc*/ LDRD        SL, [R4, #144]              @ 0x90 
    /*00002100*/ LDR         IP, [SP, #24] 
    /*00002104*/ ADDS        SL, SL, R8 
    /*00002108*/ LDR         R0, [PC, #528]              @ 0x0000000000002320 
    /*0000210c*/ ADC         FP, FP, R9 
    /*00002110*/ MOV         R8, SL 
    /*00002114*/ MOV         R9, FP 
    /*00002118*/ LDRD        SL, [SP, #32] 
    /*0000211c*/ STRD        R8, [R7, #144]              @ 0x90 
    /*00002120*/ STRD        SL, [IP] 
    /*00002124*/ LDR         R3, [R7, #144]              @ 0x90 
    /*00002128*/ STR         R3, [IP, #8] 
    /*0000212c*/ LDR         R3, [R4, #380]              @ 0x17c 
    /*00002130*/ STR         R3, [IP, #16] 
    /*00002134*/ ADD         R3, IP, #24 
    /*00002138*/ STR         R3, [SP, #24] 
    /*0000213c*/ BL          0x000000000000213C          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00002140*/ LDR         R0, [SP, #20] 
    /*00002144*/ MOV         R3, #5 
    /*00002148*/ MOV         R1, R4 
    /*0000214c*/ LDR         R2, [PC, #460]              @ 0x0000000000002320 
    /*00002150*/ STR         R3, [R0, #292]              @ 0x124 
    /*00002154*/ LDR         R0, [SP, #64]               @ 0x40 
    /*00002158*/ LDR         R3, [R5, #32] 
    /*0000215c*/ BLX         R3 
    /*00002160*/ CMP         R0, #0 
    /*00002164*/ BNE         DIS_GeneratePyramid_x22e0
    /*00002168*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*0000216c*/ LDR         R2, [SP, #24] 
    /*00002170*/ CMP         R2, R3 
    /*00002174*/ BNE         DIS_GeneratePyramid_x205c
    /*00002178*/ MOV         R2, R0 
    /*0000217c*/ LDR         R3, [R5, #12] 
    /*00002180*/ MOV         R1, #1 
    /*00002184*/ LDR         R0, [SP, #64]               @ 0x40 
    /*00002188*/ BLX         R3 
    /*0000218c*/ SUBS        R7, R0, #0 
    /*00002190*/ BNE         DIS_GeneratePyramid_x21a0
    DIS_GeneratePyramid_x2194:
    /*00002194*/ MOV         R0, R7 
    /*00002198*/ ADD         SP, SP, #156                @ 0x9c 
    /*0000219c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_GeneratePyramid_x21a0:
    /*000021a0*/ LDR         R3, [SP, #40]               @ 0x28 
    /*000021a4*/ MOVW        R2, #0                      @ .LC41 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x9c4] 
    /*000021a8*/ MOVT        R2, #0                      @ .LC41 [R_ARM_MOVT_ABS: .rodata.str1.4+0x9c4] 
    /*000021ac*/ STR         R3, [SP, #4] 
    /*000021b0*/ MOVW        R3, #1275                   @ 0x4fb 
    /*000021b4*/ STR         R3, [SP] 
    DIS_GeneratePyramid_x21b8:
    /*000021b8*/ LDR         R3, [PC, #356]              @ 0x0000000000002324 
    /*000021bc*/ MOV         R1, #17 
    /*000021c0*/ MOV         R0, #3 
    /*000021c4*/ BL          0x00000000000021C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000021c8*/ LDR         R3, [SP, #64]               @ 0x40 
    /*000021cc*/ LDR         R2, [SP, #48]               @ 0x30 
    /*000021d0*/ MOV         R0, R7 
    /*000021d4*/ STR         R3, [R2] 
    /*000021d8*/ ADD         SP, SP, #156                @ 0x9c 
    /*000021dc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_GeneratePyramid_x21e0:
    /*000021e0*/ LDR         R3, [SP, #40]               @ 0x28 
    /*000021e4*/ MOVW        R2, #0                      @ .LC39 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x948] 
    /*000021e8*/ MOVT        R2, #0                      @ .LC39 [R_ARM_MOVT_ABS: .rodata.str1.4+0x948] 
    /*000021ec*/ STR         R3, [SP, #4] 
    /*000021f0*/ MOVW        R3, #1224                   @ 0x4c8 
    /*000021f4*/ STR         R3, [SP] 
    /*000021f8*/ B           DIS_GeneratePyramid_x21b8
    DIS_GeneratePyramid_x21fc:
    /*000021fc*/ ADD         R1, SP, #60                 @ 0x3c 
    /*00002200*/ LDR         R0, [SP, #40]               @ 0x28 
    /*00002204*/ BL          0x0000000000002204          @ DIS_DrvGetFpdCfg [R_ARM_CALL: .text+0x5e6c] 
    /*00002208*/ LDR         SL, [SP, #20] 
    /*0000220c*/ LDR         R1, [SP, #60]               @ 0x3c 
    /*00002210*/ ADD         R0, SL, #112                @ 0x70 
    /*00002214*/ BL          0x0000000000002214          @ DIS_SetVgsFpdCfg [R_ARM_CALL: .text+0x1370] 
    /*00002218*/ ADD         R3, R4, #1120               @ 0x460 
    /*0000221c*/ STR         R6, [R8, #528]              @ 0x210 
    /*00002220*/ ADD         R1, SL, #320                @ 0x140 
    /*00002224*/ STR         R7, [R8, #512]              @ 0x200 
    /*00002228*/ LDRD        R2, [R3, #-8] 
    /*0000222c*/ STR         R6, [SL, #316]              @ 0x13c 
    /*00002230*/ STRD        R2, [R1] 
    /*00002234*/ B           DIS_GeneratePyramid_x201c
    DIS_GeneratePyramid_x2238:
    /*00002238*/ LDR         R3, [SP, #40]               @ 0x28 
    /*0000223c*/ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x908] 
    /*00002240*/ MOVT        R2, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x908] 
    /*00002244*/ STR         R3, [SP, #4] 
    /*00002248*/ MOVW        R3, #1164                   @ 0x48c 
    /*0000224c*/ STR         R3, [SP] 
    /*00002250*/ B           DIS_GeneratePyramid_x21b8
    DIS_GeneratePyramid_x2254:
    /*00002254*/ MOVW        R3, #1133                   @ 0x46d 
    /*00002258*/ MOVW        R2, #0                      @ .LC36 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x888] 
    /*0000225c*/ STR         R3, [SP] 
    /*00002260*/ MOVT        R2, #0                      @ .LC36 [R_ARM_MOVT_ABS: .rodata.str1.4+0x888] 
    /*00002264*/ LDR         R3, [PC, #184]              @ 0x0000000000002324 
    /*00002268*/ MOV         R1, #17 
    /*0000226c*/ MOV         R0, #3 
    /*00002270*/ BL          0x0000000000002270          @ HI_LOG [R_ARM_CALL: +0x0] 
    DIS_GeneratePyramid_x2274:
    /*00002274*/ MOV         R1, R4 
    /*00002278*/ LDR         R0, [SP, #40]               @ 0x28 
    /*0000227c*/ BL          0x000000000000227C          @ DIS_PyramidExceptionProcess [R_ARM_CALL: .text+0x1d78] 
    /*00002280*/ MVN         R7, #0 
    /*00002284*/ B           DIS_GeneratePyramid_x2194
    DIS_GeneratePyramid_x2288:
    /*00002288*/ MOV         R0, R8 
    /*0000228c*/ BL          0x000000000000228C          @ DIS_GetBusyNodeNum [R_ARM_CALL: .text+0x43c] 
    /*00002290*/ LDR         R3, [SP, #40]               @ 0x28 
    /*00002294*/ MOVW        R2, #0                      @ .LC37 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x8bc] 
    /*00002298*/ MOV         R1, #17 
    /*0000229c*/ MOVT        R2, #0                      @ .LC37 [R_ARM_MOVT_ABS: .rodata.str1.4+0x8bc] 
    /*000022a0*/ STR         R3, [SP, #4] 
    /*000022a4*/ MOV         R3, #1152                   @ 0x480 
    /*000022a8*/ STR         R3, [SP] 
    /*000022ac*/ LDR         R3, [PC, #112]              @ 0x0000000000002324 
    /*000022b0*/ STR         R0, [SP, #8] 
    /*000022b4*/ MOV         R0, #3 
    /*000022b8*/ BL          0x00000000000022B8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000022bc*/ B           DIS_GeneratePyramid_x2274
    DIS_GeneratePyramid_x22c0:
    /*000022c0*/ LDR         R3, [SP, #40]               @ 0x28 
    /*000022c4*/ MOVW        R2, #0                      @ .LC38 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x908] 
    /*000022c8*/ LDR         R7, [SP, #52]               @ 0x34 
    /*000022cc*/ MOVT        R2, #0                      @ .LC38 [R_ARM_MOVT_ABS: .rodata.str1.4+0x908] 
    /*000022d0*/ STR         R3, [SP, #4] 
    /*000022d4*/ MOVW        R3, #1235                   @ 0x4d3 
    /*000022d8*/ STR         R3, [SP] 
    /*000022dc*/ B           DIS_GeneratePyramid_x21b8
    DIS_GeneratePyramid_x22e0:
    /*000022e0*/ LDR         R3, [SP, #40]               @ 0x28 
    /*000022e4*/ MOV         R4, R0 
    /*000022e8*/ MOVW        R2, #0                      @ .LC40 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x988] 
    /*000022ec*/ MOV         R1, #17 
    /*000022f0*/ MOVT        R2, #0                      @ .LC40 [R_ARM_MOVT_ABS: .rodata.str1.4+0x988] 
    /*000022f4*/ MOV         R0, #3 
    /*000022f8*/ STR         R3, [SP, #4] 
    /*000022fc*/ MOV         R3, #1264                   @ 0x4f0 
    /*00002300*/ STR         R3, [SP] 
    /*00002304*/ MOV         R7, R4 
    /*00002308*/ LDR         R3, [PC, #20]               @ 0x0000000000002324 
    /*0000230c*/ BL          0x000000000000230C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002310*/ LDR         R3, [SP, #64]               @ 0x40 
    /*00002314*/ LDR         R2, [SP, #48]               @ 0x30 
    /*00002318*/ STR         R3, [R2] 
    /*0000231c*/ B           DIS_GeneratePyramid_x2194
    /*00002320*/ ANDEQ       R0, R0, R0, ROR R0 
    /*00002324*/ LDRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
FUNC_END DIS_GeneratePyramid


FUNC_BEGIN DIS_ProcessGME
    /*00002328*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*0000232c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002330*/ ADD         R3, R3, R0, LSL #2 
    /*00002334*/ PUSH        {R4, LR} 
    /*00002338*/ SUB         SP, SP, #8 
    /*0000233c*/ LDR         R3, [R3, #16] 
    /*00002340*/ LDR         LR, [R3, #508]              @ 0x1fc 
    /*00002344*/ LDR         IP, [R3, #500]              @ 0x1f4 
    /*00002348*/ CMP         LR, IP 
    /*0000234c*/ LDRCC       R3, [R3, #512]              @ 0x200 
    /*00002350*/ MOVCS       IP, #1 
    /*00002354*/ STRCS       IP, [R3, #512]              @ 0x200 
    /*00002358*/ STRCS       IP, [R3, #520]              @ 0x208 
    /*0000235c*/ MOVCS       R3, IP 
    /*00002360*/ LDR         IP, [R1, #316]              @ 0x13c 
    /*00002364*/ STR         R3, [R1, #1260]             @ 0x4ec 
    /*00002368*/ CMP         IP, #0 
    /*0000236c*/ MOVNE       R3, #0 
    /*00002370*/ STRNE       R3, [IP, #232]              @ 0xe8 
    /*00002374*/ BL          0x0000000000002374          @ DIS_GeneratePyramid [R_ARM_CALL: .text+0x1dc4] 
    /*00002378*/ SUBS        R4, R0, #0 
    /*0000237c*/ BEQ         DIS_ProcessGME_x23a4
    /*00002380*/ MOVW        R3, #765                    @ 0x2fd 
    /*00002384*/ MOVW        R2, #0                      @ .LC42 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa00] 
    /*00002388*/ STR         R3, [SP] 
    /*0000238c*/ MOVT        R2, #0                      @ .LC42 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa00] 
    /*00002390*/ STR         R4, [SP, #4] 
    /*00002394*/ MOV         R1, #17 
    /*00002398*/ LDR         R3, [PC, #16]               @ 0x00000000000023b0 
    /*0000239c*/ MOV         R0, #3 
    /*000023a0*/ BL          0x00000000000023A0          @ HI_LOG [R_ARM_CALL: +0x0] 
    DIS_ProcessGME_x23a4:
    /*000023a4*/ MOV         R0, R4 
    /*000023a8*/ ADD         SP, SP, #8 
    /*000023ac*/ POP         {R4, PC} 
    /*000023b0*/ ANDEQ       R0, R0, R4, ROR #3 
FUNC_END DIS_ProcessGME


FUNC_BEGIN DIS_ProcessNormalTask
    /*000023b4*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*000023b8*/ MOV         R7, R0 
    /*000023bc*/ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000023c0*/ MOVT        R0, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000023c4*/ MOV         R8, R2 
    /*000023c8*/ ADD         R2, R0, R7, LSL #2 
    /*000023cc*/ SUB         SP, SP, #12 
    /*000023d0*/ MOV         R0, R8 
    /*000023d4*/ MOV         R5, R1 
    /*000023d8*/ MOV         R9, R3 
    /*000023dc*/ LDR         R4, [R2, #16] 
    /*000023e0*/ BL          0x00000000000023E0          @ DIS_CheckAttr [R_ARM_CALL: .text+0xc7c] 
    /*000023e4*/ SUBS        R6, R0, #0 
    /*000023e8*/ BEQ         DIS_ProcessNormalTask_x2424
    /*000023ec*/ MOVW        R3, #875                    @ 0x36b 
    /*000023f0*/ MOVW        R2, #0                      @ .LC43 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa3c] 
    /*000023f4*/ STR         R3, [SP] 
    /*000023f8*/ MOVT        R2, #0                      @ .LC43 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa3c] 
    /*000023fc*/ LDR         R3, [PC, #336]              @ 0x0000000000002554 
    /*00002400*/ MOV         R1, #17 
    /*00002404*/ MOV         R0, #3 
    /*00002408*/ BL          0x0000000000002408          @ HI_LOG [R_ARM_CALL: +0x0] 
    DIS_ProcessNormalTask_x240c:
    /*0000240c*/ MOV         R1, R5 
    /*00002410*/ MOV         R0, R4 
    /*00002414*/ BL          0x0000000000002414          @ DIS_PutNodeToFreeList [R_ARM_CALL: .text+0x26c] 
    DIS_ProcessNormalTask_x2418:
    /*00002418*/ MOV         R0, R6 
    /*0000241c*/ ADD         SP, SP, #12 
    /*00002420*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    DIS_ProcessNormalTask_x2424:
    /*00002424*/ ADD         R0, R5, #336                @ 0x150 
    /*00002428*/ BL          0x0000000000002428          @ DIS_DrvCheckSingleYImage [R_ARM_CALL: .text+0x5950] 
    /*0000242c*/ SUBS        R6, R0, #0 
    /*00002430*/ BNE         DIS_ProcessNormalTask_x24f0
    /*00002434*/ MOV         R1, R8 
    /*00002438*/ MOV         R0, R7 
    /*0000243c*/ BL          0x000000000000243C          @ DIS_CalculateDynamicAttr [R_ARM_CALL: .text+0xea8] 
    /*00002440*/ LDR         R2, [R4, #504]              @ 0x1f8 
    /*00002444*/ LDR         R3, [R4, #596]              @ 0x254 
    /*00002448*/ MOV         R0, R5 
    /*0000244c*/ ADD         R2, R2, #1 
    /*00002450*/ STR         R2, [R4, #504]              @ 0x1f8 
    /*00002454*/ ADD         R3, R3, #1 
    /*00002458*/ STR         R3, [R4, #596]              @ 0x254 
    /*0000245c*/ BL          0x000000000000245C          @ DIS_InitNodeInfo [R_ARM_CALL: .text+0x11b0] 
    /*00002460*/ LDR         R3, [R8, #32] 
    /*00002464*/ STR         R3, [R5, #1256]             @ 0x4e8 
    /*00002468*/ LDR         R3, [R4, #588]              @ 0x24c 
    /*0000246c*/ STR         R3, [R5, #1268]             @ 0x4f4 
    /*00002470*/ LDR         R3, [R4, #504]              @ 0x1f8 
    /*00002474*/ STR         R3, [R5, #1264]             @ 0x4f0 
    /*00002478*/ LDR         R3, [R4, #84]               @ 0x54 
    /*0000247c*/ STR         R3, [R5, #752]              @ 0x2f0 
    /*00002480*/ LDR         R3, [R4, #88]               @ 0x58 
    /*00002484*/ STR         R3, [R5, #756]              @ 0x2f4 
    /*00002488*/ LDR         R3, [R4, #76]               @ 0x4c 
    /*0000248c*/ LSR         R3, R3, #1 
    /*00002490*/ STR         R3, [R5, #1244]             @ 0x4dc 
    /*00002494*/ LDR         R3, [R4, #80]               @ 0x50 
    /*00002498*/ LSR         R3, R3, #1 
    /*0000249c*/ STR         R3, [R5, #1248]             @ 0x4e0 
    /*000024a0*/ LDR         R3, [R4, #532]              @ 0x214 
    /*000024a4*/ STR         R3, [R5, #1272]             @ 0x4f8 
    /*000024a8*/ LDR         R3, [R4, #532]              @ 0x214 
    /*000024ac*/ CMP         R3, #1 
    /*000024b0*/ BEQ         DIS_ProcessNormalTask_x2514
    DIS_ProcessNormalTask_x24b4:
    /*000024b4*/ MOV         R1, R5 
    /*000024b8*/ MOV         R0, R4 
    /*000024bc*/ BL          0x00000000000024BC          @ DIS_PutNodeToBusyList [R_ARM_CALL: .text+0x3e8] 
    /*000024c0*/ LDR         R3, [R4, #444]              @ 0x1bc 
    /*000024c4*/ CMP         R3, #1 
    /*000024c8*/ BHI         DIS_ProcessNormalTask_x2528
    /*000024cc*/ MOV         R3, #3 
    /*000024d0*/ MOV         R2, R9 
    /*000024d4*/ MOV         R0, R7 
    /*000024d8*/ MOV         R1, R5 
    /*000024dc*/ STR         R3, [R5, #1160]             @ 0x488 
    /*000024e0*/ BL          0x00000000000024E0          @ DIS_ProcessGME [R_ARM_CALL: .text+0x2328] 
    /*000024e4*/ MOV         R0, R6 
    /*000024e8*/ ADD         SP, SP, #12 
    /*000024ec*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    DIS_ProcessNormalTask_x24f0:
    /*000024f0*/ MOVW        R3, #882                    @ 0x372 
    /*000024f4*/ MOVW        R2, #0                      @ .LC44 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa68] 
    /*000024f8*/ STR         R3, [SP] 
    /*000024fc*/ MOVT        R2, #0                      @ .LC44 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa68] 
    /*00002500*/ LDR         R3, [PC, #76]               @ 0x0000000000002554 
    /*00002504*/ MOV         R1, #17 
    /*00002508*/ MOV         R0, #3 
    /*0000250c*/ BL          0x000000000000250C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002510*/ B           DIS_ProcessNormalTask_x240c
    DIS_ProcessNormalTask_x2514:
    /*00002514*/ MOV         R1, R5 
    /*00002518*/ MOV         R0, R7 
    /*0000251c*/ BL          0x000000000000251C          @ DIS_DrvInitGmeRansacSeed [R_ARM_CALL: .text+0x5f7c] 
    /*00002520*/ STR         R6, [R4, #532]              @ 0x214 
    /*00002524*/ B           DIS_ProcessNormalTask_x24b4
    DIS_ProcessNormalTask_x2528:
    /*00002528*/ MOVW        R3, #923                    @ 0x39b 
    /*0000252c*/ MOVW        R2, #0                      @ .LC45 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xa94] 
    /*00002530*/ STR         R3, [SP] 
    /*00002534*/ MOVT        R2, #0                      @ .LC45 [R_ARM_MOVT_ABS: .rodata.str1.4+0xa94] 
    /*00002538*/ LDR         R3, [PC, #20]               @ 0x0000000000002554 
    /*0000253c*/ MOV         R1, #17 
    /*00002540*/ MOV         R0, #3 
    /*00002544*/ MOVW        R6, #32776                  @ 0x8008 
    /*00002548*/ BL          0x0000000000002548          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000254c*/ MOVT        R6, #40977                  @ 0xa011 
    /*00002550*/ B           DIS_ProcessNormalTask_x2418
    /*00002554*/ STRDEQ      R0, [R0], -R4 
FUNC_END DIS_ProcessNormalTask


FUNC_BEGIN DIS_Process
    /*00002558*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000255c*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002560*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002564*/ SUB         SP, SP, #28 
    /*00002568*/ MOV         R8, R2 
    /*0000256c*/ MVN         R2, #0 
    /*00002570*/ LDR         R3, [R4, #4] 
    /*00002574*/ STR         R2, [SP, #20] 
    /*00002578*/ CMP         R3, #1 
    /*0000257c*/ BNE         DIS_Process_x276c
    /*00002580*/ CMP         R0, #3 
    /*00002584*/ MOV         R7, R1 
    /*00002588*/ MOV         R5, R0 
    /*0000258c*/ BHI         DIS_Process_x2688
    DIS_Process_x2590:
    /*00002590*/ CMP         R7, #0 
    /*00002594*/ BEQ         DIS_Process_x2798
    DIS_Process_x2598:
    /*00002598*/ CMP         R8, #0 
    /*0000259c*/ BEQ         DIS_Process_x27cc
    DIS_Process_x25a0:
    /*000025a0*/ ADD         R3, R4, R5, LSL #2 
    /*000025a4*/ LDR         SL, [R3, #16] 
    /*000025a8*/ CMP         SL, #0 
    /*000025ac*/ BEQ         DIS_Process_x273c
    /*000025b0*/ MOV         R3, #20 
    /*000025b4*/ LDR         R9, [PC, #600]              @ 0x0000000000002814 
    /*000025b8*/ MUL         R3, R3, R5 
    /*000025bc*/ ADD         R1, SP, #16 
    /*000025c0*/ ADD         R9, R3, R9 
    /*000025c4*/ ADD         R4, R4, R3 
    /*000025c8*/ MOV         R0, R9 
    /*000025cc*/ BL          0x00000000000025CC          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000025d0*/ LDR         R6, [R4, #32] 
    /*000025d4*/ CMP         R6, #2 
    /*000025d8*/ BEQ         DIS_Process_x26a0
    /*000025dc*/ MOV         R3, #996                    @ 0x3e4 
    /*000025e0*/ STR         R5, [SP, #4] 
    /*000025e4*/ STR         R3, [SP] 
    /*000025e8*/ MOVW        R2, #0                      @ .LC32 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x7bc] 
    /*000025ec*/ LDR         R3, [PC, #548]              @ 0x0000000000002818 
    /*000025f0*/ MOVT        R2, #0                      @ .LC32 [R_ARM_MOVT_ABS: .rodata.str1.4+0x7bc] 
    /*000025f4*/ MOV         R1, #17 
    /*000025f8*/ MOV         R0, #3 
    /*000025fc*/ BL          0x00000000000025FC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002600*/ MOVW        R6, #32770                  @ 0x8002 
    /*00002604*/ MOVT        R6, #40977                  @ 0xa011 
    DIS_Process_x2608:
    /*00002608*/ MOV         R3, #1040                   @ 0x410 
    /*0000260c*/ STR         R6, [SP, #8] 
    /*00002610*/ STR         R3, [SP] 
    /*00002614*/ MOVW        R2, #0                      @ .LC48 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb14] 
    /*00002618*/ LDR         R3, [PC, #504]              @ 0x0000000000002818 
    /*0000261c*/ MOVT        R2, #0                      @ .LC48 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb14] 
    /*00002620*/ STR         R5, [SP, #4] 
    /*00002624*/ MOV         R1, #17 
    /*00002628*/ MOV         R0, #3 
    /*0000262c*/ BL          0x000000000000262C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002630*/ LDR         R3, [SL, #608]              @ 0x260 
    /*00002634*/ ADD         R3, R3, #1 
    /*00002638*/ STR         R3, [SL, #608]              @ 0x260 
    DIS_Process_x263c:
    /*0000263c*/ ADD         R1, SP, #16 
    /*00002640*/ MOV         R0, R9 
    /*00002644*/ BL          0x0000000000002644          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00002648*/ LDR         R3, [SP, #20] 
    /*0000264c*/ CMN         R3, #1 
    /*00002650*/ BEQ         DIS_Process_x2694
    /*00002654*/ MOV         R0, #45                     @ 0x2d 
    /*00002658*/ BL          0x0000000000002658          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*0000265c*/ CMP         R0, #0 
    /*00002660*/ BEQ         DIS_Process_x27a8
    /*00002664*/ LDR         R3, [R0, #12] 
    /*00002668*/ CMP         R3, #0 
    /*0000266c*/ BEQ         DIS_Process_x27a8
    /*00002670*/ LDR         R3, [R0, #4] 
    /*00002674*/ LDR         R0, [SP, #20] 
    /*00002678*/ BLX         R3 
    /*0000267c*/ MOV         R0, R6 
    /*00002680*/ ADD         SP, SP, #28 
    /*00002684*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_Process_x2688:
    /*00002688*/ BL          0x0000000000000600 
    /*0000268c*/ SUBS        R6, R0, #0 
    /*00002690*/ BEQ         DIS_Process_x2590
    DIS_Process_x2694:
    /*00002694*/ MOV         R0, R6 
    /*00002698*/ ADD         SP, SP, #28 
    /*0000269c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_Process_x26a0:
    /*000026a0*/ MOV         R0, SL 
    /*000026a4*/ BL          0x00000000000026A4          @ DIS_GetNodeFromFreeList [R_ARM_CALL: .text+0x194] 
    /*000026a8*/ SUBS        FP, R0, #0 
    /*000026ac*/ BEQ         DIS_Process_x27dc
    /*000026b0*/ LDR         R3, [SL, #588]              @ 0x24c 
    /*000026b4*/ MOV         R2, #1312                   @ 0x520 
    /*000026b8*/ MOV         R1, #0 
    /*000026bc*/ ADD         R3, R3, #1 
    /*000026c0*/ STR         R3, [SL, #588]              @ 0x24c 
    /*000026c4*/ BL          0x00000000000026C4          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000026c8*/ MOV         R1, R7 
    /*000026cc*/ MOV         R2, #848                    @ 0x350 
    /*000026d0*/ MOV         R0, FP 
    /*000026d4*/ BL          0x00000000000026D4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000026d8*/ LDR         R3, [FP, #672]              @ 0x2a0 
    /*000026dc*/ CMP         R3, #0 
    /*000026e0*/ BEQ         DIS_Process_x2718
    /*000026e4*/ LDR         R3, [R4, #40]               @ 0x28 
    /*000026e8*/ CMP         R3, #0 
    /*000026ec*/ BEQ         DIS_Process_x2718
    /*000026f0*/ LDR         R3, [FP, #664]              @ 0x298 
    /*000026f4*/ CMN         R3, #1 
    /*000026f8*/ BEQ         DIS_Process_x2718
    /*000026fc*/ MOV         R2, R8 
    /*00002700*/ MOV         R1, FP 
    /*00002704*/ ADD         R3, SP, #20 
    /*00002708*/ MOV         R0, R5 
    /*0000270c*/ BL          0x000000000000270C          @ DIS_ProcessNormalTask [R_ARM_CALL: .text+0x23b4] 
    /*00002710*/ MOV         R6, R0 
    /*00002714*/ B           DIS_Process_x2730
    DIS_Process_x2718:
    /*00002718*/ MOV         R3, #0 
    /*0000271c*/ MOV         R1, FP 
    /*00002720*/ STR         R3, [FP, #672]              @ 0x2a0 
    /*00002724*/ MOV         R0, R5 
    /*00002728*/ BL          0x0000000000002728          @ DIS_ProcessBypassTask [R_ARM_CALL: .text+0x1c14] 
    /*0000272c*/ MOV         R6, R0 
    DIS_Process_x2730:
    /*00002730*/ CMP         R6, #0 
    /*00002734*/ BEQ         DIS_Process_x263c
    /*00002738*/ B           DIS_Process_x2608
    DIS_Process_x273c:
    /*0000273c*/ MOVW        R3, #987                    @ 0x3db 
    /*00002740*/ STR         R5, [SP, #4] 
    /*00002744*/ STR         R3, [SP] 
    /*00002748*/ MOVW        R2, #0                      @ .LC46 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xabc] 
    /*0000274c*/ LDR         R3, [PC, #196]              @ 0x0000000000002818 
    /*00002750*/ MOVT        R2, #0                      @ .LC46 [R_ARM_MOVT_ABS: .rodata.str1.4+0xabc] 
    /*00002754*/ MOV         R1, #17 
    /*00002758*/ MOV         R0, #3 
    /*0000275c*/ MOVW        R6, #32774                  @ 0x8006 
    /*00002760*/ BL          0x0000000000002760          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002764*/ MOVT        R6, #40977                  @ 0xa011 
    /*00002768*/ B           DIS_Process_x2694
    DIS_Process_x276c:
    /*0000276c*/ MOV         R3, #972                    @ 0x3cc 
    /*00002770*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b0] 
    /*00002774*/ STR         R3, [SP] 
    /*00002778*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b0] 
    /*0000277c*/ LDR         R3, [PC, #148]              @ 0x0000000000002818 
    /*00002780*/ MOV         R1, #17 
    /*00002784*/ MOV         R0, #3 
    /*00002788*/ MOVW        R6, #32784                  @ 0x8010 
    /*0000278c*/ BL          0x000000000000278C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002790*/ MOVT        R6, #40977                  @ 0xa011 
    /*00002794*/ B           DIS_Process_x2694
    DIS_Process_x2798:
    /*00002798*/ BL          0x00000000000005C4 
    /*0000279c*/ SUBS        R6, R0, #0 
    /*000027a0*/ BNE         DIS_Process_x2694
    /*000027a4*/ B           DIS_Process_x2598
    DIS_Process_x27a8:
    /*000027a8*/ MOVW        R3, #1051                   @ 0x41b 
    /*000027ac*/ MOVW        R2, #0                      @ .LC49 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xb5c] 
    /*000027b0*/ STR         R3, [SP] 
    /*000027b4*/ MOVT        R2, #0                      @ .LC49 [R_ARM_MOVT_ABS: .rodata.str1.4+0xb5c] 
    /*000027b8*/ LDR         R3, [PC, #88]               @ 0x0000000000002818 
    /*000027bc*/ MOV         R1, #17 
    /*000027c0*/ MOV         R0, #3 
    /*000027c4*/ BL          0x00000000000027C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000027c8*/ B           DIS_Process_x2694
    DIS_Process_x27cc:
    /*000027cc*/ BL          0x00000000000005C4 
    /*000027d0*/ SUBS        R6, R0, #0 
    /*000027d4*/ BNE         DIS_Process_x2694
    /*000027d8*/ B           DIS_Process_x25a0
    DIS_Process_x27dc:
    /*000027dc*/ LDR         R3, [SL, #604]              @ 0x25c 
    /*000027e0*/ MOVW        IP, #1006                   @ 0x3ee 
    /*000027e4*/ MOV         R0, R6 
    /*000027e8*/ MOVW        R2, #0                      @ .LC47 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xae8] 
    /*000027ec*/ ADD         R3, R3, #1 
    /*000027f0*/ MOVT        R2, #0                      @ .LC47 [R_ARM_MOVT_ABS: .rodata.str1.4+0xae8] 
    /*000027f4*/ STR         R3, [SL, #604]              @ 0x25c 
    /*000027f8*/ MOV         R1, #17 
    /*000027fc*/ STR         IP, [SP] 
    /*00002800*/ MOVW        R6, #32782                  @ 0x800e 
    /*00002804*/ LDR         R3, [PC, #12]               @ 0x0000000000002818 
    /*00002808*/ MOVT        R6, #40977                  @ 0xa011 
    /*0000280c*/ BL          0x000000000000280C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002810*/ B           DIS_Process_x2608
    /*00002814*/ ANDEQ       R0, R0, R0, LSR R0 
    /*00002818*/ ANDEQ       R0, R0, IP, LSL #4 
FUNC_END DIS_Process


FUNC_BEGIN DIS_SendNode
    /*0000281c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002820*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002824*/ ADD         R3, R3, R0, LSL #2 
    /*00002828*/ PUSH        {R4, R5, R6, LR} 
    /*0000282c*/ MOV         R6, R0 
    /*00002830*/ SUB         SP, SP, #16 
    /*00002834*/ LDR         R4, [R3, #16] 
    /*00002838*/ B           DIS_SendNode_x2850
    DIS_SendNode_x283c:
    /*0000283c*/ BL          0x000000000000283C          @ DIS_GetHeadNodeFromBusyList [R_ARM_CALL: .text+0x29c] 
    /*00002840*/ CMP         R0, #0 
    /*00002844*/ BEQ         DIS_SendNode_x28f0
    DIS_SendNode_x2848:
    /*00002848*/ MOV         R1, R4 
    /*0000284c*/ BL          0x000000000000284C          @ DIS_ReleaseNodeBuf [R_ARM_CALL: .text+0xe8] 
    DIS_SendNode_x2850:
    /*00002850*/ MOV         R0, R4 
    /*00002854*/ BL          0x0000000000002854          @ DIS_GetBusyNodeNum [R_ARM_CALL: .text+0x43c] 
    /*00002858*/ CMP         R0, #0 
    /*0000285c*/ BEQ         DIS_SendNode_x28e8
    /*00002860*/ MOV         R0, R4 
    /*00002864*/ BL          0x0000000000002864          @ DIS_QueryNodeFromBusyList [R_ARM_CALL: .text+0x21c] 
    /*00002868*/ SUBS        R5, R0, #0 
    /*0000286c*/ BEQ         DIS_SendNode_x28f8
    /*00002870*/ LDR         R3, [R5, #1156]             @ 0x484 
    /*00002874*/ CMP         R3, #3 
    /*00002878*/ BNE         DIS_SendNode_x291c
    /*0000287c*/ BL          0x000000000000287C          @ DIS_NotifyCaller [R_ARM_CALL: .text+0x169c] 
    /*00002880*/ LDR         R3, [R5, #672]              @ 0x2a0 
    /*00002884*/ MOV         R0, R4 
    /*00002888*/ CMP         R3, #0 
    /*0000288c*/ BEQ         DIS_SendNode_x283c
    /*00002890*/ BL          0x0000000000002890          @ DIS_GetBusyNodeNum [R_ARM_CALL: .text+0x43c] 
    /*00002894*/ CMP         R0, #1 
    /*00002898*/ BLE         DIS_SendNode_x28e8
    /*0000289c*/ MOV         R0, R5 
    /*000028a0*/ MOV         R1, R4 
    /*000028a4*/ BL          0x00000000000028A4          @ DIS_QueryNextNode [R_ARM_CALL: .text+0x370] 
    /*000028a8*/ SUBS        R3, R0, #0 
    /*000028ac*/ BEQ         DIS_SendNode_x294c
    /*000028b0*/ LDR         R3, [R3, #672]              @ 0x2a0 
    /*000028b4*/ CMP         R3, #0 
    /*000028b8*/ BNE         DIS_SendNode_x28e8
    /*000028bc*/ MOV         R0, R4 
    /*000028c0*/ BL          0x00000000000028C0          @ DIS_GetHeadNodeFromBusyList [R_ARM_CALL: .text+0x29c] 
    /*000028c4*/ CMP         R0, #0 
    /*000028c8*/ BNE         DIS_SendNode_x2848
    /*000028cc*/ MOVW        R3, #1414                   @ 0x586 
    DIS_SendNode_x28d0:
    /*000028d0*/ MOVW        R2, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc04] 
    /*000028d4*/ STR         R3, [SP] 
    /*000028d8*/ MOVT        R2, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc04] 
    /*000028dc*/ LDR         R3, [PC, #136]              @ 0x000000000000296c 
    /*000028e0*/ MOV         R1, #17 
    /*000028e4*/ BL          0x00000000000028E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    DIS_SendNode_x28e8:
    /*000028e8*/ ADD         SP, SP, #16 
    /*000028ec*/ POP         {R4, R5, R6, PC} 
    DIS_SendNode_x28f0:
    /*000028f0*/ MOVW        R3, #1390                   @ 0x56e 
    /*000028f4*/ B           DIS_SendNode_x28d0
    DIS_SendNode_x28f8:
    /*000028f8*/ MOVW        R3, #1372                   @ 0x55c 
    /*000028fc*/ MOV         R1, #17 
    /*00002900*/ STR         R3, [SP] 
    /*00002904*/ MOVW        R2, #0                      @ .LC34 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x828] 
    /*00002908*/ LDR         R3, [PC, #92]               @ 0x000000000000296c 
    /*0000290c*/ MOVT        R2, #0                      @ .LC34 [R_ARM_MOVT_ABS: .rodata.str1.4+0x828] 
    /*00002910*/ BL          0x0000000000002910          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002914*/ ADD         SP, SP, #16 
    /*00002918*/ POP         {R4, R5, R6, PC} 
    DIS_SendNode_x291c:
    /*0000291c*/ STR         R3, [SP, #8] 
    /*00002920*/ MOV         R1, #17 
    /*00002924*/ MOVW        R3, #1379                   @ 0x563 
    /*00002928*/ STR         R6, [SP, #4] 
    /*0000292c*/ STR         R3, [SP] 
    /*00002930*/ MOVW        R2, #0                      @ .LC50 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xbc4] 
    /*00002934*/ LDR         R3, [PC, #48]               @ 0x000000000000296c 
    /*00002938*/ MOVT        R2, #0                      @ .LC50 [R_ARM_MOVT_ABS: .rodata.str1.4+0xbc4] 
    /*0000293c*/ MOV         R0, #7 
    /*00002940*/ BL          0x0000000000002940          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002944*/ ADD         SP, SP, #16 
    /*00002948*/ POP         {R4, R5, R6, PC} 
    DIS_SendNode_x294c:
    /*0000294c*/ MOVW        R3, #1404                   @ 0x57c 
    /*00002950*/ MOVW        R2, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc34] 
    /*00002954*/ STR         R3, [SP] 
    /*00002958*/ MOVT        R2, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc34] 
    /*0000295c*/ LDR         R3, [PC, #8]                @ 0x000000000000296c 
    /*00002960*/ MOV         R1, #17 
    /*00002964*/ BL          0x0000000000002964          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002968*/ B           DIS_SendNode_x28e8
    /*0000296c*/ ANDEQ       R0, R0, R8, LSL R2 
FUNC_END DIS_SendNode


FUNC_BEGIN DIS_ProcessFirstFrame
    /*00002970*/ PUSH        {R4, R5, R6, R8, R9, LR} 
    /*00002974*/ MOV         R4, R1 
    /*00002978*/ ADD         R1, R1, #1120               @ 0x460 
    /*0000297c*/ MOV         R2, #0 
    /*00002980*/ MOV         R3, #3 
    /*00002984*/ STR         R2, [R4, #1272]             @ 0x4f8 
    /*00002988*/ STR         R3, [R4, #1156]             @ 0x484 
    /*0000298c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002990*/ LDRD        R8, [R1, #-8] 
    /*00002994*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002998*/ ADD         R3, R3, R0, LSL #2 
    /*0000299c*/ ADD         R1, R4, #1136               @ 0x470 
    /*000029a0*/ MOV         R6, R0 
    /*000029a4*/ STRD        R8, [R1] 
    /*000029a8*/ ADD         R1, R4, #680                @ 0x2a8 
    /*000029ac*/ LDR         IP, [R4, #1120]             @ 0x460 
    /*000029b0*/ LDR         R5, [R3, #16] 
    /*000029b4*/ STR         IP, [R4, #1144]             @ 0x478 
    /*000029b8*/ STR         R2, [R5, #528]              @ 0x210 
    /*000029bc*/ BL          0x00000000000029BC          @ DIS_DrvSetUnitMotionData [R_ARM_CALL: .text+0x5e64] 
    /*000029c0*/ MOV         R0, R5 
    /*000029c4*/ BL          0x00000000000029C4          @ DIS_QueryNodeFromBusyList [R_ARM_CALL: .text+0x21c] 
    /*000029c8*/ CMP         R4, R0 
    /*000029cc*/ BEQ         DIS_ProcessFirstFrame_x29d8
    /*000029d0*/ MOV         R0, #0 
    /*000029d4*/ POP         {R4, R5, R6, R8, R9, PC} 
    DIS_ProcessFirstFrame_x29d8:
    /*000029d8*/ MOV         R0, R6 
    /*000029dc*/ BL          0x00000000000029DC          @ DIS_SendNode [R_ARM_CALL: .text+0x281c] 
    /*000029e0*/ MOV         R0, #0 
    /*000029e4*/ POP         {R4, R5, R6, R8, R9, PC} 
FUNC_END DIS_ProcessFirstFrame


FUNC_BEGIN DIS_FpdMerge
    /*000029e8*/ PUSH        {R4, LR} 
    /*000029ec*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000029f0*/ LDR         R4, [R1, #1120]             @ 0x460 
    /*000029f4*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000029f8*/ ADD         R3, R3, R0, LSL #2 
    /*000029fc*/ LDR         R2, [R1, #1144]             @ 0x478 
    /*00002a00*/ SUB         SP, SP, #8 
    /*00002a04*/ LDR         IP, [R4] 
    /*00002a08*/ LDR         R3, [R3, #16] 
    /*00002a0c*/ CMP         IP, #0 
    /*00002a10*/ MOVNE       IP, #0 
    /*00002a14*/ STRNE       IP, [R3, #528]              @ 0x210 
    /*00002a18*/ LDR         IP, [R3, #520]              @ 0x208 
    /*00002a1c*/ MOV         R3, R4 
    /*00002a20*/ STR         IP, [SP] 
    /*00002a24*/ BL          0x0000000000002A24          @ DIS_DrvFpdMerge [R_ARM_CALL: .text+0x5e70] 
    /*00002a28*/ MOV         R3, #0 
    /*00002a2c*/ MOV         R0, R3 
    /*00002a30*/ STR         R3, [R4] 
    /*00002a34*/ ADD         SP, SP, #8 
    /*00002a38*/ POP         {R4, PC} 
FUNC_END DIS_FpdMerge


FUNC_BEGIN DIS_SetGmeMotion
    /*00002a3c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002a40*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002a44*/ ADD         R3, R3, R0, LSL #2 
    /*00002a48*/ PUSH        {R4, R6, R7, LR} 
    /*00002a4c*/ MOV         R4, R2 
    /*00002a50*/ LDR         R3, [R3, #16] 
    /*00002a54*/ LDR         R3, [R3, #464]              @ 0x1d0 
    /*00002a58*/ CMP         R3, #1 
    /*00002a5c*/ BEQ         DIS_SetGmeMotion_x2a94
    DIS_SetGmeMotion_x2a60:
    /*00002a60*/ MOV         R2, R4 
    /*00002a64*/ BL          0x0000000000002A64          @ DIS_DrvSetGmeMotion [R_ARM_CALL: .text+0x5db8] 
    /*00002a68*/ LDR         R0, [R4, #20] 
    /*00002a6c*/ CMP         R0, #0 
    /*00002a70*/ BNE         DIS_SetGmeMotion_x2a8c
    /*00002a74*/ LDR         R3, [R4, #24] 
    /*00002a78*/ CMP         R3, #0 
    /*00002a7c*/ LDREQ       R0, [R4, #28] 
    /*00002a80*/ CLZEQ       R0, R0 
    /*00002a84*/ LSREQ       R0, R0, #5 
    /*00002a88*/ POP         {R4, R6, R7, PC} 
    DIS_SetGmeMotion_x2a8c:
    /*00002a8c*/ MOV         R0, #0 
    /*00002a90*/ POP         {R4, R6, R7, PC} 
    DIS_SetGmeMotion_x2a94:
    /*00002a94*/ MOV         R2, #0 
    /*00002a98*/ MOV         R3, #0 
    /*00002a9c*/ MOV         R6, #1048576                @ 0x100000 
    /*00002aa0*/ MOV         R7, #0 
    /*00002aa4*/ STRD        R2, [R1, #8] 
    /*00002aa8*/ STRD        R6, [R1] 
    /*00002aac*/ STRD        R6, [R1, #32] 
    /*00002ab0*/ STRD        R2, [R1, #16] 
    /*00002ab4*/ STRD        R2, [R1, #24] 
    /*00002ab8*/ STRD        R2, [R1, #40]               @ 0x28 
    /*00002abc*/ B           DIS_SetGmeMotion_x2a60
FUNC_END DIS_SetGmeMotion


FUNC_BEGIN DIS_ExceptionProcess
    /*00002ac0*/ PUSH        {R4, R5, R6, LR} 
    /*00002ac4*/ SUB         SP, SP, #80                 @ 0x50 
    /*00002ac8*/ MOV         R4, R1 
    /*00002acc*/ MOV         R5, R0 
    /*00002ad0*/ MOV         R2, #72                     @ 0x48 
    /*00002ad4*/ ADD         R0, SP, #8 
    /*00002ad8*/ MOV         R1, #0 
    /*00002adc*/ BL          0x0000000000002ADC          @ memset [R_ARM_CALL: +0x0] 
    /*00002ae0*/ LDR         R3, [R4, #1272]             @ 0x4f8 
    /*00002ae4*/ CMP         R3, #1 
    /*00002ae8*/ BEQ         DIS_ExceptionProcess_x2b6c
    /*00002aec*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002af0*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002af4*/ ADD         R3, R3, R5, LSL #2 
    /*00002af8*/ MOV         R0, R4 
    /*00002afc*/ LDR         R1, [R3, #16] 
    /*00002b00*/ BL          0x0000000000002B00          @ DIS_QueryPrevNode [R_ARM_CALL: .text+0x3a8] 
    /*00002b04*/ SUBS        R6, R0, #0 
    /*00002b08*/ BEQ         DIS_ExceptionProcess_x2b84
    DIS_ExceptionProcess_x2b0c:
    /*00002b0c*/ LDR         R3, [R6, #1156]             @ 0x484 
    /*00002b10*/ CMP         R3, #0 
    /*00002b14*/ BEQ         DIS_ExceptionProcess_x2ba4
    /*00002b18*/ MOV         R1, R4 
    /*00002b1c*/ MOV         R0, R5 
    /*00002b20*/ BL          0x0000000000002B20          @ DIS_FpdMerge [R_ARM_CALL: .text+0x29e8] 
    /*00002b24*/ MOV         R3, #1 
    /*00002b28*/ ADD         R1, SP, #8 
    /*00002b2c*/ ADD         R2, R4, #1200               @ 0x4b0 
    /*00002b30*/ STR         R3, [R4, #1220]             @ 0x4c4 
    /*00002b34*/ MOV         R0, R5 
    /*00002b38*/ BL          0x0000000000002B38          @ DIS_SetGmeMotion [R_ARM_CALL: .text+0x2a3c] 
    /*00002b3c*/ LDR         R3, [R4, #316]              @ 0x13c 
    /*00002b40*/ MOV         R2, R6 
    /*00002b44*/ MOV         R1, R4 
    /*00002b48*/ CMP         R3, #0 
    /*00002b4c*/ STRNE       R0, [R3, #72]               @ 0x48 
    /*00002b50*/ MOV         R3, #3 
    /*00002b54*/ MOV         R0, R5 
    /*00002b58*/ STR         R3, [R4, #1156]             @ 0x484 
    /*00002b5c*/ BL          0x0000000000002B5C          @ DIS_MotionFilter [R_ARM_CALL: .text+0x2de4] 
    /*00002b60*/ MOV         R0, #0 
    DIS_ExceptionProcess_x2b64:
    /*00002b64*/ ADD         SP, SP, #80                 @ 0x50 
    /*00002b68*/ POP         {R4, R5, R6, PC} 
    DIS_ExceptionProcess_x2b6c:
    /*00002b6c*/ MOV         R1, R4 
    /*00002b70*/ MOV         R0, R5 
    /*00002b74*/ BL          0x0000000000002B74          @ DIS_ProcessFirstFrame [R_ARM_CALL: .text+0x2970] 
    /*00002b78*/ MOV         R0, #0 
    /*00002b7c*/ ADD         SP, SP, #80                 @ 0x50 
    /*00002b80*/ POP         {R4, R5, R6, PC} 
    DIS_ExceptionProcess_x2b84:
    /*00002b84*/ MOVW        R3, #0                      @ .LC53 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc64] 
    /*00002b88*/ MOVW        R0, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6d0] 
    /*00002b8c*/ MOVT        R3, #0                      @ .LC53 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc64] 
    /*00002b90*/ MOVT        R0, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6d0] 
    /*00002b94*/ MOVW        R2, #1449                   @ 0x5a9 
    /*00002b98*/ LDR         R1, [PC, #48]               @ 0x0000000000002bd0 
    /*00002b9c*/ BL          0x0000000000002B9C          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00002ba0*/ B           DIS_ExceptionProcess_x2b0c
    DIS_ExceptionProcess_x2ba4:
    /*00002ba4*/ STR         R3, [SP, #4] 
    /*00002ba8*/ MOVW        R2, #0                      @ .LC54 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc7c] 
    /*00002bac*/ MOVW        R3, #1454                   @ 0x5ae 
    /*00002bb0*/ MOVT        R2, #0                      @ .LC54 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc7c] 
    /*00002bb4*/ STR         R3, [SP] 
    /*00002bb8*/ MOV         R1, #17 
    /*00002bbc*/ LDR         R3, [PC, #12]               @ 0x0000000000002bd0 
    /*00002bc0*/ MOV         R0, #3 
    /*00002bc4*/ BL          0x0000000000002BC4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002bc8*/ MVN         R0, #0 
    /*00002bcc*/ B           DIS_ExceptionProcess_x2b64
    /*00002bd0*/ ANDEQ       R0, R0, R8, LSR #4 
FUNC_END DIS_ExceptionProcess


FUNC_BEGIN DIS_ProcessBusyList
    /*00002bd4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00002bd8*/ LSL         R8, R0, #2 
    /*00002bdc*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002be0*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002be4*/ ADD         R3, R4, R8 
    /*00002be8*/ SUB         SP, SP, #24 
    /*00002bec*/ MOV         R6, R0 
    /*00002bf0*/ LDR         R5, [R3, #16] 
    /*00002bf4*/ MOV         R0, R5 
    /*00002bf8*/ BL          0x0000000000002BF8          @ DIS_GetBusyNodeNum [R_ARM_CALL: .text+0x43c] 
    /*00002bfc*/ CMP         R0, #0 
    /*00002c00*/ BNE         DIS_ProcessBusyList_x2c10
    DIS_ProcessBusyList_x2c04:
    /*00002c04*/ MOV         R0, #0 
    DIS_ProcessBusyList_x2c08:
    /*00002c08*/ ADD         SP, SP, #24 
    /*00002c0c*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    DIS_ProcessBusyList_x2c10:
    /*00002c10*/ MOV         R0, R5 
    /*00002c14*/ BL          0x0000000000002C14          @ DIS_QueryNodeFromBusyList [R_ARM_CALL: .text+0x21c] 
    /*00002c18*/ SUBS        R9, R0, #0 
    /*00002c1c*/ MOVWEQ      R3, #1494                   @ 0x5d6 
    /*00002c20*/ MOVWEQ      R2, #0                      @ .LC55 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xcb4] 
    /*00002c24*/ STREQ       R3, [SP] 
    /*00002c28*/ MOVTEQ      R2, #0                      @ .LC55 [R_ARM_MOVT_ABS: .rodata.str1.4+0xcb4] 
    /*00002c2c*/ BEQ         DIS_ProcessBusyList_x2dc8
    /*00002c30*/ LDR         R3, [R9, #1156]             @ 0x484 
    /*00002c34*/ LDR         R2, [R9, #1152]             @ 0x480 
    /*00002c38*/ CMP         R3, #3 
    /*00002c3c*/ BNE         DIS_ProcessBusyList_x2c48
    /*00002c40*/ CMP         R2, #0 
    /*00002c44*/ BNE         DIS_ProcessBusyList_x2c88
    DIS_ProcessBusyList_x2c48:
    /*00002c48*/ LDR         R1, [R9, #1268]             @ 0x4f4 
    /*00002c4c*/ MOVW        IP, #1503                   @ 0x5df 
    /*00002c50*/ STR         R2, [SP, #12] 
    /*00002c54*/ MOV         R0, #7 
    /*00002c58*/ STR         R3, [SP, #8] 
    /*00002c5c*/ MOVW        R2, #0                      @ .LC56 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xce0] 
    /*00002c60*/ STR         R1, [SP, #16] 
    /*00002c64*/ MOVT        R2, #0                      @ .LC56 [R_ARM_MOVT_ABS: .rodata.str1.4+0xce0] 
    /*00002c68*/ MOV         R1, #17 
    /*00002c6c*/ STR         R6, [SP, #4] 
    /*00002c70*/ STR         IP, [SP] 
    /*00002c74*/ LDR         R3, [PC, #356]              @ 0x0000000000002de0 
    /*00002c78*/ BL          0x0000000000002C78          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002c7c*/ MOV         R0, R9 
    /*00002c80*/ BL          0x0000000000002C80          @ DIS_NotifyCaller [R_ARM_CALL: .text+0x169c] 
    /*00002c84*/ B           DIS_ProcessBusyList_x2c04
    DIS_ProcessBusyList_x2c88:
    /*00002c88*/ MOV         R0, R6 
    /*00002c8c*/ BL          0x0000000000002C8C          @ DIS_SendNode [R_ARM_CALL: .text+0x281c] 
    /*00002c90*/ MOV         R0, R5 
    /*00002c94*/ BL          0x0000000000002C94          @ DIS_GetBusyNodeNum [R_ARM_CALL: .text+0x43c] 
    /*00002c98*/ CMP         R0, #1 
    /*00002c9c*/ BLE         DIS_ProcessBusyList_x2c04
    /*00002ca0*/ MOV         R1, R5 
    /*00002ca4*/ MOV         R0, R9 
    /*00002ca8*/ BL          0x0000000000002CA8          @ DIS_QueryNextNode [R_ARM_CALL: .text+0x370] 
    /*00002cac*/ SUBS        SL, R0, #0 
    /*00002cb0*/ BEQ         DIS_ProcessBusyList_x2db8
    /*00002cb4*/ LDR         R7, [SL, #1152]             @ 0x480 
    /*00002cb8*/ CMP         R7, #0 
    /*00002cbc*/ BEQ         DIS_ProcessBusyList_x2d64
    /*00002cc0*/ LDR         R3, [R9, #1144]             @ 0x478 
    /*00002cc4*/ CMP         R7, #2 
    /*00002cc8*/ LDR         R1, [R3] 
    /*00002ccc*/ LDR         R3, [R9, #1152]             @ 0x480 
    /*00002cd0*/ BNE         DIS_ProcessBusyList_x2cdc
    /*00002cd4*/ CMP         R3, #2 
    /*00002cd8*/ BEQ         DIS_ProcessBusyList_x2d9c
    DIS_ProcessBusyList_x2cdc:
    /*00002cdc*/ ADD         R4, R4, R8 
    /*00002ce0*/ STR         R3, [SP, #12] 
    /*00002ce4*/ STR         R1, [SP, #4] 
    /*00002ce8*/ MOVW        R3, #1540                   @ 0x604 
    /*00002cec*/ MOV         R1, #17 
    /*00002cf0*/ STR         R3, [SP] 
    /*00002cf4*/ STR         R7, [SP, #8] 
    /*00002cf8*/ MOVW        R2, #0                      @ .LC58 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd88] 
    /*00002cfc*/ LDR         R3, [PC, #220]              @ 0x0000000000002de0 
    /*00002d00*/ MOVT        R2, #0                      @ .LC58 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd88] 
    /*00002d04*/ MOV         R0, #7 
    /*00002d08*/ BL          0x0000000000002D08          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002d0c*/ LDR         R2, [R9, #1144]             @ 0x478 
    /*00002d10*/ LDR         R3, [R4, #16] 
    /*00002d14*/ MOVW        R1, #498                    @ 0x1f2 
    /*00002d18*/ LDR         R2, [R2] 
    /*00002d1c*/ LDRH        R1, [R3, R1] 
    /*00002d20*/ CMP         R2, R1 
    /*00002d24*/ BCS         DIS_ProcessBusyList_x2d3c
    /*00002d28*/ LDR         R1, [R3, #528]              @ 0x210 
    /*00002d2c*/ CMP         R1, #0 
    /*00002d30*/ MOVEQ       R1, #1 
    /*00002d34*/ STREQ       R1, [R3, #512]              @ 0x200 
    /*00002d38*/ STREQ       R1, [R3, #520]              @ 0x208 
    DIS_ProcessBusyList_x2d3c:
    /*00002d3c*/ ADD         R1, R3, #496                @ 0x1f0 
    /*00002d40*/ MOV         R0, R6 
    /*00002d44*/ LDRH        R1, [R1] 
    /*00002d48*/ CMP         R2, R1 
    /*00002d4c*/ MOV         R1, SL 
    /*00002d50*/ MOVCC       R2, #0 
    /*00002d54*/ STRCC       R2, [R3, #520]              @ 0x208 
    /*00002d58*/ ADD         SP, SP, #24 
    /*00002d5c*/ POP         {R4, R5, R6, R7, R8, R9, SL, LR} 
    DIS_ProcessBusyList_x2d60:
    /*00002d60*/ B           DIS_ProcessBusyList_x2d60   @ DIS_ExceptionProcess [R_ARM_JUMP24: .text+0x2ac0] 
    DIS_ProcessBusyList_x2d64:
    /*00002d64*/ LDR         R1, [SL, #1268]             @ 0x4f4 
    /*00002d68*/ MOVW        R3, #1526                   @ 0x5f6 
    /*00002d6c*/ STR         R7, [SP, #8] 
    /*00002d70*/ MOVW        R2, #0                      @ .LC57 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xd40] 
    /*00002d74*/ STR         R3, [SP] 
    /*00002d78*/ MOVT        R2, #0                      @ .LC57 [R_ARM_MOVT_ABS: .rodata.str1.4+0xd40] 
    /*00002d7c*/ STR         R1, [SP, #12] 
    /*00002d80*/ MOV         R0, #7 
    /*00002d84*/ STR         R6, [SP, #4] 
    /*00002d88*/ MOV         R1, #17 
    /*00002d8c*/ LDR         R3, [PC, #76]               @ 0x0000000000002de0 
    /*00002d90*/ BL          0x0000000000002D90          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002d94*/ MOV         R0, R7 
    /*00002d98*/ B           DIS_ProcessBusyList_x2c08
    DIS_ProcessBusyList_x2d9c:
    /*00002d9c*/ LDRH        R2, [R5, #174]              @ 0xae 
    /*00002da0*/ CMP         R1, R2 
    /*00002da4*/ BCC         DIS_ProcessBusyList_x2cdc
    /*00002da8*/ MOV         R0, R6 
    /*00002dac*/ ADD         SP, SP, #24 
    /*00002db0*/ POP         {R4, R5, R6, R7, R8, R9, SL, LR} 
    DIS_ProcessBusyList_x2db4:
    /*00002db4*/ B           DIS_ProcessBusyList_x2db4   @ DIS_PrepareSubmmitToGme [R_ARM_JUMP24: .text+0x151c] 
    DIS_ProcessBusyList_x2db8:
    /*00002db8*/ MOVW        R2, #0                      @ .LC52 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc34] 
    /*00002dbc*/ MOVT        R2, #0                      @ .LC52 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc34] 
    /*00002dc0*/ MOVW        R3, #1519                   @ 0x5ef 
    /*00002dc4*/ STR         R3, [SP] 
    DIS_ProcessBusyList_x2dc8:
    /*00002dc8*/ LDR         R3, [PC, #16]               @ 0x0000000000002de0 
    /*00002dcc*/ MOV         R1, #17 
    /*00002dd0*/ BL          0x0000000000002DD0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002dd4*/ MOVW        R0, #32782                  @ 0x800e 
    /*00002dd8*/ MOVT        R0, #40977                  @ 0xa011 
    /*00002ddc*/ B           DIS_ProcessBusyList_x2c08
    /*00002de0*/ ANDEQ       R0, R0, R0, ASR #4 
FUNC_END DIS_ProcessBusyList


FUNC_BEGIN DIS_MotionFilter
    /*00002de4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00002de8*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002dec*/ LDR         R8, [R1, #672]              @ 0x2a0 
    /*00002df0*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002df4*/ ADD         R3, R5, R0, LSL #2 
    /*00002df8*/ SUB         SP, SP, #16 
    /*00002dfc*/ CMP         R8, #1 
    /*00002e00*/ MOV         R6, R1 
    /*00002e04*/ MOV         R7, R0 
    /*00002e08*/ LDR         R9, [R1, #316]              @ 0x13c 
    /*00002e0c*/ LDR         R4, [R3, #16] 
    /*00002e10*/ BEQ         DIS_MotionFilter_x2efc
    /*00002e14*/ SUB         R8, R8, #2 
    /*00002e18*/ CMP         R9, #0 
    /*00002e1c*/ CLZ         R8, R8 
    /*00002e20*/ LSR         R8, R8, #5 
    /*00002e24*/ MOVEQ       R8, #0 
    /*00002e28*/ CMP         R8, #0 
    /*00002e2c*/ MOVNE       R3, #0 
    /*00002e30*/ STRNE       R3, [R9, #152]              @ 0x98 
    DIS_MotionFilter_x2e34:
    /*00002e34*/ MOV         R0, R4 
    /*00002e38*/ BL          0x0000000000002E38          @ DIS_GetHeadNodeFromBusyList [R_ARM_CALL: .text+0x29c] 
    /*00002e3c*/ CMP         R0, #0 
    /*00002e40*/ BEQ         DIS_MotionFilter_x2f28
    /*00002e44*/ MOV         R1, R4 
    /*00002e48*/ BL          0x0000000000002E48          @ DIS_ReleaseNodeBuf [R_ARM_CALL: .text+0xe8] 
    /*00002e4c*/ MOV         R0, R7 
    /*00002e50*/ BL          0x0000000000002E50          @ DIS_ProcessBusyList [R_ARM_CALL: .text+0x2bd4] 
    /*00002e54*/ ADD         R0, SP, #8 
    /*00002e58*/ LDR         R8, [R6, #1276]             @ 0x4fc 
    /*00002e5c*/ LDR         R6, [R6, #1280]             @ 0x500 
    /*00002e60*/ BL          0x0000000000002E60          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00002e64*/ LDR         R3, [SP, #8] 
    /*00002e68*/ MOVW        R1, #16960                  @ 0x4240 
    /*00002e6c*/ LDR         R2, [SP, #12] 
    /*00002e70*/ MOVT        R1, #15 
    /*00002e74*/ SUB         R3, R3, R8 
    /*00002e78*/ LDR         R0, [R4, #652]              @ 0x28c 
    /*00002e7c*/ SUB         R2, R2, R6 
    /*00002e80*/ MLA         R3, R1, R3, R2 
    /*00002e84*/ CMP         R3, R0 
    /*00002e88*/ STR         R3, [R4, #648]              @ 0x288 
    /*00002e8c*/ STRHI       R3, [R4, #652]              @ 0x28c 
    /*00002e90*/ MOV         R3, #20 
    /*00002e94*/ MLA         R3, R3, R7, R5 
    /*00002e98*/ LDR         R3, [R3, #40]               @ 0x28 
    /*00002e9c*/ CMP         R3, #0 
    /*00002ea0*/ BNE         DIS_MotionFilter_x2ee4
    DIS_MotionFilter_x2ea4:
    /*00002ea4*/ LDR         R0, [R4, #548]              @ 0x224 
    /*00002ea8*/ CMP         R0, #0 
    /*00002eac*/ BNE         DIS_MotionFilter_x2ef0
    /*00002eb0*/ LDR         R4, [R4, #544]              @ 0x220 
    /*00002eb4*/ CMP         R4, #0 
    /*00002eb8*/ BNE         DIS_MotionFilter_x2ef4
    /*00002ebc*/ MOV         R3, #20 
    /*00002ec0*/ LDR         R0, [PC, #136]              @ 0x0000000000002f50 
    /*00002ec4*/ MUL         R7, R3, R7 
    /*00002ec8*/ ADD         R5, R5, R7 
    /*00002ecc*/ ADD         R0, R7, R0 
    /*00002ed0*/ STR         R4, [R5, #36]               @ 0x24 
    /*00002ed4*/ BL          0x0000000000002ED4          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00002ed8*/ MOV         R0, R4 
    /*00002edc*/ ADD         SP, SP, #16 
    /*00002ee0*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    DIS_MotionFilter_x2ee4:
    /*00002ee4*/ LDR         R3, [R5, #4] 
    /*00002ee8*/ CMP         R3, #2 
    /*00002eec*/ BEQ         DIS_MotionFilter_x2ea4
    DIS_MotionFilter_x2ef0:
    /*00002ef0*/ MOV         R0, #0 
    DIS_MotionFilter_x2ef4:
    /*00002ef4*/ ADD         SP, SP, #16 
    /*00002ef8*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    DIS_MotionFilter_x2efc:
    /*00002efc*/ ADD         SL, R1, #680                @ 0x2a8 
    /*00002f00*/ MOV         R1, SL 
    /*00002f04*/ BL          0x0000000000002F04          @ DIS_DrvGetStabilizedMotion [R_ARM_CALL: .text+0x5e68] 
    /*00002f08*/ CMP         R9, #0 
    /*00002f0c*/ BEQ         DIS_MotionFilter_x2e34
    /*00002f10*/ MOV         R1, SL 
    /*00002f14*/ MOV         R2, #72                     @ 0x48 
    /*00002f18*/ ADD         R0, R9, #80                 @ 0x50 
    /*00002f1c*/ BL          0x0000000000002F1C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00002f20*/ STR         R8, [R9, #152]              @ 0x98 
    /*00002f24*/ B           DIS_MotionFilter_x2e34
    DIS_MotionFilter_x2f28:
    /*00002f28*/ MOVW        R3, #1830                   @ 0x726 
    /*00002f2c*/ MOVW        R2, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc04] 
    /*00002f30*/ STR         R3, [SP] 
    /*00002f34*/ MOVT        R2, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc04] 
    /*00002f38*/ LDR         R3, [PC, #20]               @ 0x0000000000002f54 
    /*00002f3c*/ MOV         R1, #17 
    /*00002f40*/ BL          0x0000000000002F40          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00002f44*/ MOVW        R0, #32782                  @ 0x800e 
    /*00002f48*/ MOVT        R0, #40977                  @ 0xa011 
    /*00002f4c*/ B           DIS_MotionFilter_x2ef4
    /*00002f50*/ ANDEQ       R0, R0, IP, LSR #32 
    /*00002f54*/ ANDEQ       R0, R0, R4, ASR R2 
FUNC_END DIS_MotionFilter


FUNC_BEGIN DIS_VgsCallback
    /*00002f58*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00002f5c*/ SUBS        R4, R3, #0 
    /*00002f60*/ SUB         SP, SP, #52                 @ 0x34 
    /*00002f64*/ MOV         R3, #0 
    /*00002f68*/ STR         R3, [SP, #36]               @ 0x24 
    /*00002f6c*/ BEQ         DIS_VgsCallback_x316c
    DIS_VgsCallback_x2f70:
    /*00002f70*/ LDR         R5, [R4, #8] 
    /*00002f74*/ LDR         R3, [R4, #16] 
    /*00002f78*/ CMP         R5, #0 
    /*00002f7c*/ LDR         R9, [R4] 
    /*00002f80*/ LDR         R8, [R4, #24] 
    /*00002f84*/ STR         R3, [SP, #20] 
    /*00002f88*/ LDR         SL, [R4, #32] 
    /*00002f8c*/ BEQ         DIS_VgsCallback_x3200
    DIS_VgsCallback_x2f90:
    /*00002f90*/ ADD         R0, SP, #40                 @ 0x28 
    /*00002f94*/ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00002f98*/ LSL         FP, R9, #2 
    /*00002f9c*/ MOVT        R6, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00002fa0*/ BL          0x0000000000002FA0          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00002fa4*/ ADD         R3, R6, FP 
    /*00002fa8*/ LDR         R7, [R3, #16] 
    /*00002fac*/ CMP         R7, #0 
    /*00002fb0*/ BEQ         DIS_VgsCallback_x332c
    /*00002fb4*/ MOV         R0, #20 
    /*00002fb8*/ LDR         R2, [PC, #928]              @ 0x0000000000003360 
    /*00002fbc*/ MUL         R3, R0, R9 
    /*00002fc0*/ ADD         R1, SP, #36                 @ 0x24 
    /*00002fc4*/ STR         R3, [SP, #28] 
    /*00002fc8*/ ADD         R3, R3, #16 
    /*00002fcc*/ ADD         R3, R2, R3 
    /*00002fd0*/ STR         R3, [SP, #24] 
    /*00002fd4*/ MOV         R0, R3 
    /*00002fd8*/ BL          0x0000000000002FD8          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00002fdc*/ LDR         R1, [SP, #40]               @ 0x28 
    /*00002fe0*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*00002fe4*/ MOVW        R0, #16960                  @ 0x4240 
    /*00002fe8*/ SUB         R8, R1, R8 
    /*00002fec*/ MOVT        R0, #15 
    /*00002ff0*/ SUB         SL, R2, SL 
    /*00002ff4*/ LDR         R3, [R7, #632]              @ 0x278 
    /*00002ff8*/ MLA         R8, R0, R8, SL 
    /*00002ffc*/ CMP         R8, R3 
    /*00003000*/ ADD         R3, R5, #1296               @ 0x510 
    /*00003004*/ ADD         R3, R3, #4 
    /*00003008*/ STRHI       R8, [R7, #632]              @ 0x278 
    /*0000300c*/ CMP         R3, #0 
    /*00003010*/ STR         R8, [R7, #628]              @ 0x274 
    /*00003014*/ STR         R1, [R5, #1276]             @ 0x4fc 
    /*00003018*/ STR         R2, [R5, #1280]             @ 0x500 
    /*0000301c*/ BEQ         DIS_VgsCallback_x31e0
    DIS_VgsCallback_x3020:
    /*00003020*/ LDR         R3, [R4, #48]               @ 0x30 
    /*00003024*/ CMP         R3, #1 
    /*00003028*/ BEQ         DIS_VgsCallback_x30b8
    /*0000302c*/ STR         R3, [SP, #8] 
    /*00003030*/ MOV         R1, #17 
    /*00003034*/ MOVW        R3, #1608                   @ 0x648 
    /*00003038*/ STR         R9, [SP, #4] 
    /*0000303c*/ STR         R3, [SP] 
    /*00003040*/ MOVW        R2, #0                      @ .LC61 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe14] 
    /*00003044*/ LDR         R3, [PC, #792]              @ 0x0000000000003364 
    /*00003048*/ MOVT        R2, #0                      @ .LC61 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe14] 
    /*0000304c*/ MOV         R0, #3 
    /*00003050*/ BL          0x0000000000003050          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003054*/ MOV         R1, R5 
    /*00003058*/ MOV         R3, #1 
    /*0000305c*/ STR         R3, [R5, #1152]             @ 0x480 
    DIS_VgsCallback_x3060:
    /*00003060*/ MOV         R3, #0 
    /*00003064*/ MOV         R0, R9 
    /*00003068*/ STR         R3, [R7, #520]              @ 0x208 
    /*0000306c*/ BL          0x000000000000306C          @ DIS_ExceptionProcess [R_ARM_CALL: .text+0x2ac0] 
    /*00003070*/ MOV         FP, R0 
    DIS_VgsCallback_x3074:
    /*00003074*/ MOV         R2, #20 
    /*00003078*/ LDR         R3, [R7, #544]              @ 0x220 
    /*0000307c*/ MLA         R2, R2, R9, R6 
    /*00003080*/ SUB         R3, R3, #1 
    /*00003084*/ STR         R3, [R7, #544]              @ 0x220 
    /*00003088*/ LDR         R2, [R2, #40]               @ 0x28 
    /*0000308c*/ CMP         R2, #0 
    /*00003090*/ BEQ         DIS_VgsCallback_x313c
    /*00003094*/ LDR         R2, [R6, #4] 
    /*00003098*/ CMP         R2, #2 
    /*0000309c*/ BEQ         DIS_VgsCallback_x313c
    DIS_VgsCallback_x30a0:
    /*000030a0*/ ADD         R1, SP, #36                 @ 0x24 
    /*000030a4*/ LDR         R0, [SP, #24] 
    /*000030a8*/ BL          0x00000000000030A8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    DIS_VgsCallback_x30ac:
    /*000030ac*/ MOV         R0, FP 
    /*000030b0*/ ADD         SP, SP, #52                 @ 0x34 
    /*000030b4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_VgsCallback_x30b8:
    /*000030b8*/ LDR         R1, [R5, #1260]             @ 0x4ec 
    /*000030bc*/ ADD         R2, R6, FP 
    /*000030c0*/ MOV         R0, #2 
    /*000030c4*/ STR         R0, [R5, #1152]             @ 0x480 
    /*000030c8*/ CMP         R1, #0 
    /*000030cc*/ LDR         R2, [R2, #16] 
    /*000030d0*/ BNE         DIS_VgsCallback_x317c
    /*000030d4*/ LDR         R3, [R2, #508]              @ 0x1fc 
    /*000030d8*/ ADD         R3, R3, #1 
    /*000030dc*/ STR         R3, [R2, #508]              @ 0x1fc 
    DIS_VgsCallback_x30e0:
    /*000030e0*/ LDR         R3, [SP, #20] 
    /*000030e4*/ CMP         R3, #1 
    /*000030e8*/ BEQ         DIS_VgsCallback_x3210
    /*000030ec*/ MOV         R1, R7 
    /*000030f0*/ MOV         R0, R5 
    /*000030f4*/ BL          0x00000000000030F4          @ DIS_QueryPrevNode [R_ARM_CALL: .text+0x3a8] 
    /*000030f8*/ SUBS        R4, R0, #0 
    /*000030fc*/ BEQ         DIS_VgsCallback_x32f4
    /*00003100*/ LDR         R3, [R4, #1152]             @ 0x480 
    /*00003104*/ CMP         R3, #2 
    /*00003108*/ BEQ         DIS_VgsCallback_x31a4
    /*0000310c*/ STR         R3, [SP, #8] 
    /*00003110*/ MOV         R1, #17 
    /*00003114*/ MOVW        R3, #1638                   @ 0x666 
    /*00003118*/ STR         R9, [SP, #4] 
    /*0000311c*/ STR         R3, [SP] 
    /*00003120*/ MOVW        R2, #0                      @ .LC63 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe8c] 
    /*00003124*/ LDR         R3, [PC, #568]              @ 0x0000000000003364 
    /*00003128*/ MOVT        R2, #0                      @ .LC63 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe8c] 
    /*0000312c*/ MOV         R0, #3 
    /*00003130*/ BL          0x0000000000003130          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003134*/ MOV         R1, R5 
    /*00003138*/ B           DIS_VgsCallback_x3060
    DIS_VgsCallback_x313c:
    /*0000313c*/ LDR         R2, [R7, #548]              @ 0x224 
    /*00003140*/ ORRS        R3, R3, R2 
    /*00003144*/ BNE         DIS_VgsCallback_x30a0
    /*00003148*/ MOV         R2, #20 
    /*0000314c*/ LDR         R1, [SP, #28] 
    /*00003150*/ MLA         R6, R2, R9, R6 
    /*00003154*/ LDR         R2, [PC, #516]              @ 0x0000000000003360 
    /*00003158*/ ADD         R0, R1, #12 
    /*0000315c*/ ADD         R0, R2, R0 
    /*00003160*/ STR         R3, [R6, #36]               @ 0x24 
    /*00003164*/ BL          0x0000000000003164          @ osal_wakeup [R_ARM_CALL: +0x0] 
    /*00003168*/ B           DIS_VgsCallback_x30a0
    DIS_VgsCallback_x316c:
    /*0000316c*/ BL          0x00000000000005C4 
    /*00003170*/ SUBS        FP, R0, #0 
    /*00003174*/ BNE         DIS_VgsCallback_x30ac
    /*00003178*/ B           DIS_VgsCallback_x2f70
    DIS_VgsCallback_x317c:
    /*0000317c*/ MOV         R1, #0 
    /*00003180*/ STR         R1, [R2, #508]              @ 0x1fc 
    /*00003184*/ LDR         R0, [R5, #1120]             @ 0x460 
    /*00003188*/ MOVW        R1, #498                    @ 0x1f2 
    /*0000318c*/ LDRH        R1, [R2, R1] 
    /*00003190*/ LDR         R0, [R0] 
    /*00003194*/ CMP         R0, R1 
    /*00003198*/ MOVCS       R3, #0 
    /*0000319c*/ STR         R3, [R2, #512]              @ 0x200 
    /*000031a0*/ B           DIS_VgsCallback_x30e0
    DIS_VgsCallback_x31a4:
    /*000031a4*/ LDR         R8, [R4, #1156]             @ 0x484 
    /*000031a8*/ CMP         R8, #3 
    /*000031ac*/ BEQ         DIS_VgsCallback_x3224
    /*000031b0*/ MOVW        R3, #1646                   @ 0x66e 
    /*000031b4*/ STR         R8, [SP, #8] 
    /*000031b8*/ STR         R3, [SP] 
    /*000031bc*/ MOVW        R2, #0                      @ .LC64 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xec4] 
    /*000031c0*/ STR         R9, [SP, #4] 
    /*000031c4*/ MOVT        R2, #0                      @ .LC64 [R_ARM_MOVT_ABS: .rodata.str1.4+0xec4] 
    /*000031c8*/ LDR         R3, [PC, #404]              @ 0x0000000000003364 
    /*000031cc*/ MOV         R1, #17 
    /*000031d0*/ MOV         R0, #7 
    /*000031d4*/ MOV         FP, #0 
    /*000031d8*/ BL          0x00000000000031D8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000031dc*/ B           DIS_VgsCallback_x3074
    DIS_VgsCallback_x31e0:
    /*000031e0*/ MOVW        R3, #0                      @ .LC60 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe00] 
    /*000031e4*/ MOVW        R0, #0                      @ .LC28 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x6d0] 
    /*000031e8*/ MOVT        R3, #0                      @ .LC60 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe00] 
    /*000031ec*/ MOVT        R0, #0                      @ .LC28 [R_ARM_MOVT_ABS: .rodata.str1.4+0x6d0] 
    /*000031f0*/ MOVW        R2, #1603                   @ 0x643 
    /*000031f4*/ LDR         R1, [PC, #360]              @ 0x0000000000003364 
    /*000031f8*/ BL          0x00000000000031F8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*000031fc*/ B           DIS_VgsCallback_x3020
    DIS_VgsCallback_x3200:
    /*00003200*/ BL          0x00000000000005C4 
    /*00003204*/ SUBS        FP, R0, #0 
    /*00003208*/ BNE         DIS_VgsCallback_x30ac
    /*0000320c*/ B           DIS_VgsCallback_x2f90
    DIS_VgsCallback_x3210:
    /*00003210*/ MOV         R1, R5 
    /*00003214*/ MOV         R0, R9 
    /*00003218*/ BL          0x0000000000003218          @ DIS_ProcessFirstFrame [R_ARM_CALL: .text+0x2970] 
    /*0000321c*/ MOV         FP, #0 
    /*00003220*/ B           DIS_VgsCallback_x3074
    DIS_VgsCallback_x3224:
    /*00003224*/ LDR         R3, [R4, #1144]             @ 0x478 
    /*00003228*/ LDRH        R2, [R7, #174]              @ 0xae 
    /*0000322c*/ LDR         R3, [R3] 
    /*00003230*/ CMP         R3, R2 
    /*00003234*/ BCS         DIS_VgsCallback_x32bc
    /*00003238*/ ADD         FP, R6, FP 
    /*0000323c*/ STR         R3, [SP, #8] 
    /*00003240*/ MOV         R1, #17 
    /*00003244*/ MOVW        R3, #1657                   @ 0x679 
    /*00003248*/ STR         R9, [SP, #4] 
    /*0000324c*/ MOVW        R2, #0                      @ .LC65 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf00] 
    /*00003250*/ STR         R3, [SP] 
    /*00003254*/ MOVT        R2, #0                      @ .LC65 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf00] 
    /*00003258*/ LDR         R3, [PC, #260]              @ 0x0000000000003364 
    /*0000325c*/ MOV         R0, #4 
    /*00003260*/ BL          0x0000000000003260          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003264*/ LDR         R2, [R4, #1144]             @ 0x478 
    /*00003268*/ LDR         R3, [FP, #16] 
    /*0000326c*/ MOVW        R1, #498                    @ 0x1f2 
    /*00003270*/ LDR         R2, [R2] 
    /*00003274*/ LDRH        R1, [R3, R1] 
    /*00003278*/ CMP         R2, R1 
    /*0000327c*/ BCS         DIS_VgsCallback_x3294
    /*00003280*/ LDR         R1, [R3, #528]              @ 0x210 
    /*00003284*/ CMP         R1, #0 
    /*00003288*/ MOVEQ       R1, #1 
    /*0000328c*/ STREQ       R1, [R3, #512]              @ 0x200 
    /*00003290*/ STREQ       R1, [R3, #520]              @ 0x208 
    DIS_VgsCallback_x3294:
    /*00003294*/ ADD         R1, R3, #496                @ 0x1f0 
    /*00003298*/ MOV         R0, R9 
    /*0000329c*/ LDRH        R1, [R1] 
    /*000032a0*/ CMP         R2, R1 
    /*000032a4*/ MOV         R1, R5 
    /*000032a8*/ MOVCC       R2, #0 
    /*000032ac*/ STRCC       R2, [R3, #520]              @ 0x208 
    /*000032b0*/ BL          0x00000000000032B0          @ DIS_ExceptionProcess [R_ARM_CALL: .text+0x2ac0] 
    /*000032b4*/ MOV         FP, R0 
    /*000032b8*/ B           DIS_VgsCallback_x3074
    DIS_VgsCallback_x32bc:
    /*000032bc*/ MOV         R0, R9 
    /*000032c0*/ BL          0x00000000000032C0          @ DIS_PrepareSubmmitToGme [R_ARM_CALL: .text+0x151c] 
    /*000032c4*/ SUBS        FP, R0, #0 
    /*000032c8*/ BEQ         DIS_VgsCallback_x3074
    /*000032cc*/ MOVW        R3, #1665                   @ 0x681 
    /*000032d0*/ STR         R9, [SP, #4] 
    /*000032d4*/ STR         R3, [SP] 
    /*000032d8*/ MOV         R0, R8 
    /*000032dc*/ MOVW        R2, #0                      @ .LC66 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf34] 
    /*000032e0*/ LDR         R3, [PC, #124]              @ 0x0000000000003364 
    /*000032e4*/ MOVT        R2, #0                      @ .LC66 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf34] 
    /*000032e8*/ MOV         R1, #17 
    /*000032ec*/ BL          0x00000000000032EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000032f0*/ B           DIS_VgsCallback_x3074
    DIS_VgsCallback_x32f4:
    /*000032f4*/ MOV         R0, R7 
    /*000032f8*/ MOVW        FP, #32774                  @ 0x8006 
    /*000032fc*/ BL          0x00000000000032FC          @ DIS_GetBusyNodeNum [R_ARM_CALL: .text+0x43c] 
    /*00003300*/ MOVW        R3, #1631                   @ 0x65f 
    /*00003304*/ MOVW        R2, #0                      @ .LC62 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xe4c] 
    /*00003308*/ STR         R3, [SP] 
    /*0000330c*/ MOVT        R2, #0                      @ .LC62 [R_ARM_MOVT_ABS: .rodata.str1.4+0xe4c] 
    /*00003310*/ LDR         R3, [PC, #76]               @ 0x0000000000003364 
    /*00003314*/ MOV         R1, #17 
    /*00003318*/ MOVT        FP, #40977                  @ 0xa011 
    /*0000331c*/ STR         R0, [SP, #4] 
    /*00003320*/ MOV         R0, #3 
    /*00003324*/ BL          0x0000000000003324          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003328*/ B           DIS_VgsCallback_x3074
    DIS_VgsCallback_x332c:
    /*0000332c*/ MOVW        R3, #1587                   @ 0x633 
    /*00003330*/ STR         R7, [SP, #8] 
    /*00003334*/ STR         R3, [SP] 
    /*00003338*/ MOVW        R2, #0                      @ .LC59 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xdcc] 
    /*0000333c*/ STR         R9, [SP, #4] 
    /*00003340*/ MOVT        R2, #0                      @ .LC59 [R_ARM_MOVT_ABS: .rodata.str1.4+0xdcc] 
    /*00003344*/ LDR         R3, [PC, #24]               @ 0x0000000000003364 
    /*00003348*/ MOV         R1, #17 
    /*0000334c*/ MOV         R0, #3 
    /*00003350*/ MOVW        FP, #32774                  @ 0x8006 
    /*00003354*/ BL          0x0000000000003354          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003358*/ MOVT        FP, #40977                  @ 0xa011 
    /*0000335c*/ B           DIS_VgsCallback_x30ac
    /*00003360*/ ANDEQ       R0, R0, R0, LSR #32 
    /*00003364*/ ANDEQ       R0, R0, R8, ROR #4 
FUNC_END DIS_VgsCallback


FUNC_BEGIN DIS_IntCallBack
    /*00003368*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*0000336c*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003370*/ LSL         SL, R0, #2 
    /*00003374*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003378*/ ADD         R3, R5, SL 
    /*0000337c*/ SUB         SP, SP, #24 
    /*00003380*/ MOV         R7, R0 
    /*00003384*/ LDR         R4, [R3, #16] 
    /*00003388*/ CMP         R4, #0 
    /*0000338c*/ BEQ         DIS_IntCallBack_x3590
    /*00003390*/ MOV         R6, #20 
    /*00003394*/ MOV         R8, R1 
    /*00003398*/ MLA         R6, R6, R0, R5 
    /*0000339c*/ ADD         R1, SP, #20 
    /*000033a0*/ ADD         R6, R6, #48                 @ 0x30 
    /*000033a4*/ MOV         R0, R6 
    /*000033a8*/ BL          0x00000000000033A8          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000033ac*/ LDR         R3, [R4, #548]              @ 0x224 
    /*000033b0*/ LDR         R2, [R4, #660]              @ 0x294 
    /*000033b4*/ MOV         R0, R4 
    /*000033b8*/ SUB         R3, R3, #1 
    /*000033bc*/ STR         R3, [R4, #548]              @ 0x224 
    /*000033c0*/ LDR         R3, [R8, #4] 
    /*000033c4*/ CMP         R3, R2 
    /*000033c8*/ STR         R3, [R4, #656]              @ 0x290 
    /*000033cc*/ STRHI       R3, [R4, #660]              @ 0x294 
    /*000033d0*/ BL          0x00000000000033D0          @ DIS_QueryNodeFromBusyList [R_ARM_CALL: .text+0x21c] 
    /*000033d4*/ SUBS        R9, R0, #0 
    /*000033d8*/ MOVWEQ      R3, #1926                   @ 0x786 
    /*000033dc*/ BEQ         DIS_IntCallBack_x3560
    /*000033e0*/ MOV         R1, R4 
    /*000033e4*/ BL          0x00000000000033E4          @ DIS_QueryNextNode [R_ARM_CALL: .text+0x370] 
    /*000033e8*/ SUBS        R4, R0, #0 
    /*000033ec*/ BEQ         DIS_IntCallBack_x355c
    /*000033f0*/ ADD         R8, R4, #1200               @ 0x4b0 
    /*000033f4*/ MOV         R1, R9 
    /*000033f8*/ MOV         R2, R8 
    /*000033fc*/ BL          0x00000000000033FC          @ DIS_DrvGetGMEStatus [R_ARM_CALL: .text+0x5f0c] 
    /*00003400*/ MOV         R1, R8 
    /*00003404*/ MOV         R0, R7 
    /*00003408*/ BL          0x0000000000003408          @ DIS_DrvCalinlierOverlapRatio [R_ARM_CALL: .text+0x5f98] 
    /*0000340c*/ LDR         R3, [R4, #1220]             @ 0x4c4 
    /*00003410*/ LDR         R2, [R4, #1228]             @ 0x4cc 
    /*00003414*/ CMP         R3, #1 
    /*00003418*/ BEQ         DIS_IntCallBack_x3524
    /*0000341c*/ CMP         R2, #1 
    /*00003420*/ BEQ         DIS_IntCallBack_x3524
    DIS_IntCallBack_x3424:
    /*00003424*/ ADD         R5, R5, SL 
    /*00003428*/ LDR         R2, [R4, #1144]             @ 0x478 
    /*0000342c*/ MOVW        R1, #498                    @ 0x1f2 
    /*00003430*/ LDR         R3, [R5, #16] 
    /*00003434*/ LDR         R2, [R2] 
    /*00003438*/ LDRH        R1, [R3, R1] 
    /*0000343c*/ CMP         R2, R1 
    /*00003440*/ BCC         DIS_IntCallBack_x350c
    DIS_IntCallBack_x3444:
    /*00003444*/ ADD         R1, R3, #496                @ 0x1f0 
    /*00003448*/ ADD         R5, R4, #960                @ 0x3c0 
    /*0000344c*/ MOV         R0, R7 
    /*00003450*/ LDRH        R1, [R1] 
    /*00003454*/ CMP         R2, R1 
    /*00003458*/ MOV         R1, R4 
    /*0000345c*/ MOVCC       R2, #0 
    /*00003460*/ STRCC       R2, [R3, #520]              @ 0x208 
    /*00003464*/ BL          0x0000000000003464          @ DIS_FpdMerge [R_ARM_CALL: .text+0x29e8] 
    /*00003468*/ LDRD        R2, [R4, #240]              @ 0xf0 
    /*0000346c*/ LDR         R0, [R4, #1264]             @ 0x4f0 
    /*00003470*/ STR         R8, [SP] 
    /*00003474*/ BL          0x0000000000003474          @ DIS_DebugOutputGMEStatus [R_ARM_CALL: .text+0x4254] 
    /*00003478*/ MOV         R1, R5 
    /*0000347c*/ LDR         R0, [R4, #952]              @ 0x3b8 
    /*00003480*/ BL          0x0000000000003480          @ DIS_GetGmeMotionData [R_ARM_CALL: .text+0x1604] 
    /*00003484*/ ADD         R0, R4, #1280               @ 0x500 
    /*00003488*/ MOV         R3, #3 
    /*0000348c*/ ADD         R0, R0, #4 
    /*00003490*/ STR         R3, [R4, #1156]             @ 0x484 
    /*00003494*/ BL          0x0000000000003494          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00003498*/ MOV         R2, R8 
    /*0000349c*/ MOV         R1, R5 
    /*000034a0*/ MOV         R0, R7 
    /*000034a4*/ BL          0x00000000000034A4          @ DIS_SetGmeMotion [R_ARM_CALL: .text+0x2a3c] 
    /*000034a8*/ LDR         R3, [R4, #316]              @ 0x13c 
    /*000034ac*/ CMP         R3, #0 
    /*000034b0*/ BEQ         DIS_IntCallBack_x34c8
    /*000034b4*/ STR         R0, [R3, #72]               @ 0x48 
    /*000034b8*/ MOV         R2, #72                     @ 0x48 
    /*000034bc*/ MOV         R0, R3 
    /*000034c0*/ MOV         R1, R5 
    /*000034c4*/ BL          0x00000000000034C4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    DIS_IntCallBack_x34c8:
    /*000034c8*/ ADD         R0, R4, #1280               @ 0x500 
    /*000034cc*/ ADD         R0, R0, #12 
    /*000034d0*/ BL          0x00000000000034D0          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000034d4*/ LDRD        R2, [R4, #240]              @ 0xf0 
    /*000034d8*/ LDR         R0, [R4, #1264]             @ 0x4f0 
    /*000034dc*/ STR         R5, [SP] 
    /*000034e0*/ BL          0x00000000000034E0          @ DIS_DebugOutputGmeMotion [R_ARM_CALL: .text+0x4480] 
    /*000034e4*/ MOV         R2, R9 
    /*000034e8*/ MOV         R1, R4 
    /*000034ec*/ MOV         R0, R7 
    /*000034f0*/ BL          0x00000000000034F0          @ DIS_MotionFilter [R_ARM_CALL: .text+0x2de4] 
    /*000034f4*/ MOV         R0, R6 
    /*000034f8*/ ADD         R1, SP, #20 
    /*000034fc*/ BL          0x00000000000034FC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00003500*/ MOV         R0, #0 
    DIS_IntCallBack_x3504:
    /*00003504*/ ADD         SP, SP, #24 
    /*00003508*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    DIS_IntCallBack_x350c:
    /*0000350c*/ LDR         R1, [R3, #528]              @ 0x210 
    /*00003510*/ CMP         R1, #0 
    /*00003514*/ MOVEQ       R1, #1 
    /*00003518*/ STREQ       R1, [R3, #512]              @ 0x200 
    /*0000351c*/ STREQ       R1, [R3, #520]              @ 0x208 
    /*00003520*/ B           DIS_IntCallBack_x3444
    DIS_IntCallBack_x3524:
    /*00003524*/ STR         R2, [SP, #8] 
    /*00003528*/ MOVW        R1, #1951                   @ 0x79f 
    /*0000352c*/ STR         R3, [SP, #4] 
    /*00003530*/ MOVW        R2, #0                      @ .LC68 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfb0] 
    /*00003534*/ STR         R1, [SP] 
    /*00003538*/ MOVT        R2, #0                      @ .LC68 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfb0] 
    /*0000353c*/ LDR         R3, [PC, #128]              @ 0x00000000000035c4 
    /*00003540*/ MOV         R1, #17 
    /*00003544*/ MOV         R0, #7 
    /*00003548*/ BL          0x0000000000003548          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000354c*/ LDR         R3, [R4, #1144]             @ 0x478 
    /*00003550*/ MOV         R2, #0 
    /*00003554*/ STR         R2, [R3] 
    /*00003558*/ B           DIS_IntCallBack_x3424
    DIS_IntCallBack_x355c:
    /*0000355c*/ MOVW        R3, #1935                   @ 0x78f 
    DIS_IntCallBack_x3560:
    /*00003560*/ STR         R3, [SP] 
    /*00003564*/ MOV         R1, #17 
    /*00003568*/ LDR         R3, [PC, #84]               @ 0x00000000000035c4 
    /*0000356c*/ MOVW        R2, #0                      @ .LC51 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xc04] 
    /*00003570*/ MOVT        R2, #0                      @ .LC51 [R_ARM_MOVT_ABS: .rodata.str1.4+0xc04] 
    /*00003574*/ BL          0x0000000000003574          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003578*/ MOV         R0, R6 
    /*0000357c*/ ADD         R1, SP, #20 
    /*00003580*/ BL          0x0000000000003580          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00003584*/ MOVW        R0, #32782                  @ 0x800e 
    /*00003588*/ MOVT        R0, #40977                  @ 0xa011 
    /*0000358c*/ B           DIS_IntCallBack_x3504
    DIS_IntCallBack_x3590:
    /*00003590*/ MOVW        R3, #1895                   @ 0x767 
    /*00003594*/ STR         R0, [SP, #4] 
    /*00003598*/ STR         R3, [SP] 
    /*0000359c*/ MOVW        R2, #0                      @ .LC67 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xf74] 
    /*000035a0*/ STR         R4, [SP, #8] 
    /*000035a4*/ MOVT        R2, #0                      @ .LC67 [R_ARM_MOVT_ABS: .rodata.str1.4+0xf74] 
    /*000035a8*/ LDR         R3, [PC, #20]               @ 0x00000000000035c4 
    /*000035ac*/ MOV         R1, #17 
    /*000035b0*/ MOV         R0, #3 
    /*000035b4*/ BL          0x00000000000035B4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000035b8*/ MOVW        R0, #32774                  @ 0x8006 
    /*000035bc*/ MOVT        R0, #40977                  @ 0xa011 
    /*000035c0*/ B           DIS_IntCallBack_x3504
    /*000035c4*/ ANDEQ       R0, R0, R8, ROR R2 
FUNC_END DIS_IntCallBack


FUNC_BEGIN DIS_GetStabilizedMotion
    /*000035c8*/ PUSH        {R4, LR} 
    /*000035cc*/ BL          0x00000000000035CC          @ DIS_DrvGetStabilizedMotion [R_ARM_CALL: .text+0x5e68] 
    /*000035d0*/ MOV         R0, #0 
    /*000035d4*/ POP         {R4, PC} 
FUNC_END DIS_GetStabilizedMotion


FUNC_BEGIN DIS_ProcChnSysConfig
    /*000035d8*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*000035dc*/ SUB         SP, SP, #16 
    /*000035e0*/ MOV         R6, R0 
    /*000035e4*/ MOVW        R1, #0                      @ .LC69 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0xfe4] 
    /*000035e8*/ MOVT        R1, #0                      @ .LC69 [R_ARM_MOVT_ABS: .rodata.str1.4+0xfe4] 
    /*000035ec*/ LDR         R5, [PC, #280]              @ 0x000000000000370c 
    /*000035f0*/ BL          0x00000000000035F0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000035f4*/ MOVW        R2, #0                      @ .LC73 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1074] 
    /*000035f8*/ MOVW        R3, #0                      @ .LC74 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1080] 
    /*000035fc*/ MOVT        R2, #0                      @ .LC73 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1074] 
    /*00003600*/ MOVT        R3, #0                      @ .LC74 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1080] 
    /*00003604*/ STR         R2, [SP, #12] 
    /*00003608*/ STR         R3, [SP, #8] 
    /*0000360c*/ MOVW        R2, #0                      @ .LC75 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x108c] 
    /*00003610*/ MOVW        R3, #0                      @ .LC76 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1098] 
    /*00003614*/ MOVT        R2, #0                      @ .LC75 [R_ARM_MOVT_ABS: .rodata.str1.4+0x108c] 
    /*00003618*/ MOVT        R3, #0                      @ .LC76 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1098] 
    /*0000361c*/ STR         R2, [SP, #4] 
    /*00003620*/ STR         R3, [SP] 
    /*00003624*/ MOV         R0, R6 
    /*00003628*/ MOVW        R3, #0                      @ .LC70 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1048] 
    /*0000362c*/ MOVW        R2, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1054] 
    /*00003630*/ MOVT        R3, #0                      @ .LC70 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1048] 
    /*00003634*/ MOVT        R2, #0                      @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1054] 
    /*00003638*/ MOVW        R1, #0                      @ .LC72 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x105c] 
    /*0000363c*/ MOVT        R1, #0                      @ .LC72 [R_ARM_MOVT_ABS: .rodata.str1.4+0x105c] 
    /*00003640*/ BL          0x0000000000003640          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003644*/ MOV         R0, R6 
    /*00003648*/ MOVW        R1, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a0] 
    /*0000364c*/ MOVT        R1, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a0] 
    /*00003650*/ BL          0x0000000000003650          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003654*/ MOVW        R9, #0                      @ .LC78 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a4] 
    /*00003658*/ MOVW        SL, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a0] 
    /*0000365c*/ SUB         R8, R5, #16 
    /*00003660*/ SUB         R7, R5, #20 
    /*00003664*/ MOVT        R9, #0                      @ .LC78 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a4] 
    /*00003668*/ MOVT        SL, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a0] 
    /*0000366c*/ MOV         R4, #0 
    DIS_ProcChnSysConfig_x3670:
    /*00003670*/ MOV         R0, R7 
    /*00003674*/ BL          0x0000000000003674          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00003678*/ MOV         R2, R4 
    /*0000367c*/ MOV         R1, R9 
    /*00003680*/ CMP         R0, #0 
    /*00003684*/ MOV         R0, R6 
    /*00003688*/ BNE         DIS_ProcChnSysConfig_x36fc
    /*0000368c*/ LDR         IP, [R8], #4 
    /*00003690*/ CMP         IP, #0 
    /*00003694*/ BEQ         DIS_ProcChnSysConfig_x36d8
    /*00003698*/ LDR         R3, [R5, #8] 
    /*0000369c*/ CMP         R3, #0 
    /*000036a0*/ BEQ         DIS_ProcChnSysConfig_x36d8
    /*000036a4*/ LDR         LR, [IP, #88]               @ 0x58 
    /*000036a8*/ LDR         R3, [IP, #52]               @ 0x34 
    /*000036ac*/ STR         LR, [SP, #12] 
    /*000036b0*/ LDR         LR, [IP, #84]               @ 0x54 
    /*000036b4*/ STR         LR, [SP, #8] 
    /*000036b8*/ LDR         LR, [IP, #80]               @ 0x50 
    /*000036bc*/ STR         LR, [SP, #4] 
    /*000036c0*/ LDR         IP, [IP, #76]               @ 0x4c 
    /*000036c4*/ STR         IP, [SP] 
    /*000036c8*/ BL          0x00000000000036C8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000036cc*/ MOV         R1, SL 
    /*000036d0*/ MOV         R0, R6 
    /*000036d4*/ BL          0x00000000000036D4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    DIS_ProcChnSysConfig_x36d8:
    /*000036d8*/ MOV         R0, R7 
    /*000036dc*/ ADD         R4, R4, #1 
    /*000036e0*/ BL          0x00000000000036E0          @ osal_up [R_ARM_CALL: +0x0] 
    /*000036e4*/ CMP         R4, #4 
    /*000036e8*/ ADD         R5, R5, #20 
    /*000036ec*/ BNE         DIS_ProcChnSysConfig_x3670
    /*000036f0*/ MOV         R0, #0 
    /*000036f4*/ ADD         SP, SP, #16 
    /*000036f8*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    DIS_ProcChnSysConfig_x36fc:
    /*000036fc*/ MOV         R0, #65024                  @ 0xfe00 
    /*00003700*/ MOVT        R0, #65535                  @ 0xffff 
    /*00003704*/ ADD         SP, SP, #16 
    /*00003708*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    /*0000370c*/ ANDEQ       R0, R0, R0, LSR #32 
FUNC_END DIS_ProcChnSysConfig


FUNC_BEGIN DIS_ProcChnAttr
    /*00003710*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00003714*/ SUB         SP, SP, #52                 @ 0x34 
    /*00003718*/ LDR         R5, [PC, #424]              @ 0x00000000000038c8 
    /*0000371c*/ MOV         R7, R0 
    /*00003720*/ MOVW        R1, #0                      @ .LC81 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10c4] 
    /*00003724*/ MOVT        R1, #0                      @ .LC81 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10c4] 
    /*00003728*/ BL          0x0000000000003728          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*0000372c*/ MOVW        SL, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10bc] 
    /*00003730*/ MOVW        R2, #0                      @ .LC84 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1154] 
    /*00003734*/ MOVW        R3, #0                      @ .LC85 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1160] 
    /*00003738*/ MOVT        R2, #0                      @ .LC84 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1154] 
    /*0000373c*/ MOVT        R3, #0                      @ .LC85 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1160] 
    /*00003740*/ STR         R2, [SP, #24] 
    /*00003744*/ MOV         R0, R7 
    /*00003748*/ STR         R3, [SP, #20] 
    /*0000374c*/ MOVW        R2, #0                      @ .LC86 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x116c] 
    /*00003750*/ MOVW        R3, #0                      @ .LC87 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1178] 
    /*00003754*/ MOVT        R2, #0                      @ .LC86 [R_ARM_MOVT_ABS: .rodata.str1.4+0x116c] 
    /*00003758*/ MOVT        R3, #0                      @ .LC87 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1178] 
    /*0000375c*/ STR         R2, [SP, #16] 
    /*00003760*/ STR         R3, [SP, #12] 
    /*00003764*/ MOVW        R2, #0                      @ .LC88 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1184] 
    /*00003768*/ MOVW        R3, #0                      @ .LC89 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x118c] 
    /*0000376c*/ MOVT        R2, #0                      @ .LC88 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1184] 
    /*00003770*/ MOVT        R3, #0                      @ .LC89 [R_ARM_MOVT_ABS: .rodata.str1.4+0x118c] 
    /*00003774*/ STR         R2, [SP, #8] 
    /*00003778*/ STR         R3, [SP, #4] 
    /*0000377c*/ MOVW        R2, #0                      @ .LC90 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1194] 
    /*00003780*/ MOVW        R3, #0                      @ .LC82 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1128] 
    /*00003784*/ MOVT        R2, #0                      @ .LC90 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1194] 
    /*00003788*/ MOVT        R3, #0                      @ .LC82 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1128] 
    /*0000378c*/ STR         R2, [SP] 
    /*00003790*/ MOVW        R1, #0                      @ .LC83 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1130] 
    /*00003794*/ MOVW        R2, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1054] 
    /*00003798*/ MOVT        R1, #0                      @ .LC83 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1130] 
    /*0000379c*/ MOVT        R2, #0                      @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1054] 
    /*000037a0*/ MOVW        R9, #0                      @ .LC80 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10c0] 
    /*000037a4*/ SUB         R8, R5, #20 
    /*000037a8*/ MOVT        SL, #0                      @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10bc] 
    /*000037ac*/ MOVT        R9, #0                      @ .LC80 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10c0] 
    /*000037b0*/ BL          0x00000000000037B0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000037b4*/ MOV         R4, #0 
    /*000037b8*/ MOV         R0, R7 
    /*000037bc*/ MOVW        R1, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a0] 
    /*000037c0*/ MOVT        R1, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a0] 
    /*000037c4*/ SUB         FP, R5, #16 
    /*000037c8*/ BL          0x00000000000037C8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000037cc*/ STR         FP, [SP, #32] 
    DIS_ProcChnAttr_x37d0:
    /*000037d0*/ MOV         R0, R8 
    /*000037d4*/ BL          0x00000000000037D4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000037d8*/ MOVW        R1, #0                      @ .LC91 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x119c] 
    /*000037dc*/ MOV         R2, R4 
    /*000037e0*/ MOVT        R1, #0                      @ .LC91 [R_ARM_MOVT_ABS: .rodata.str1.4+0x119c] 
    /*000037e4*/ CMP         R0, #0 
    /*000037e8*/ MOV         R0, R7 
    /*000037ec*/ BNE         DIS_ProcChnAttr_x38b8
    /*000037f0*/ LDR         R3, [SP, #32] 
    /*000037f4*/ LDR         IP, [R3], #4 
    /*000037f8*/ CMP         IP, #0 
    /*000037fc*/ STR         R3, [SP, #32] 
    /*00003800*/ BEQ         DIS_ProcChnAttr_x3894
    /*00003804*/ LDR         R3, [R5, #8] 
    /*00003808*/ CMP         R3, #0 
    /*0000380c*/ BEQ         DIS_ProcChnAttr_x3894
    /*00003810*/ LDR         LR, [IP, #412]              @ 0x19c 
    /*00003814*/ LDR         R3, [IP, #460]              @ 0x1cc 
    /*00003818*/ LDR         FP, [IP, #248]              @ 0xf8 
    /*0000381c*/ STR         LR, [SP, #36]               @ 0x24 
    /*00003820*/ CMP         R3, #1 
    /*00003824*/ LDR         LR, [IP, #296]              @ 0x128 
    /*00003828*/ LDR         R6, [IP, #100]              @ 0x64 
    /*0000382c*/ MOVEQ       R3, SL 
    /*00003830*/ MOVNE       R3, R9 
    /*00003834*/ STR         LR, [SP, #40]               @ 0x28 
    /*00003838*/ SUB         R6, R6, #2 
    /*0000383c*/ LDR         LR, [IP, #300]              @ 0x12c 
    /*00003840*/ STR         LR, [SP, #44]               @ 0x2c 
    /*00003844*/ LDR         LR, [IP, #464]              @ 0x1d0 
    /*00003848*/ STR         FP, [SP, #24] 
    /*0000384c*/ LDR         IP, [IP, #244]              @ 0xf4 
    /*00003850*/ CMP         LR, #1 
    /*00003854*/ STR         R6, [SP] 
    /*00003858*/ MOVEQ       LR, SL 
    /*0000385c*/ MOVNE       LR, R9 
    /*00003860*/ STR         IP, [SP, #20] 
    /*00003864*/ STR         LR, [SP, #16] 
    /*00003868*/ LDR         R6, [SP, #44]               @ 0x2c 
    /*0000386c*/ LDR         IP, [SP, #40]               @ 0x28 
    /*00003870*/ LDR         LR, [SP, #36]               @ 0x24 
    /*00003874*/ STR         R6, [SP, #12] 
    /*00003878*/ STR         IP, [SP, #8] 
    /*0000387c*/ STR         LR, [SP, #4] 
    /*00003880*/ BL          0x0000000000003880          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003884*/ MOVW        R1, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a0] 
    /*00003888*/ MOV         R0, R7 
    /*0000388c*/ MOVT        R1, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a0] 
    /*00003890*/ BL          0x0000000000003890          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    DIS_ProcChnAttr_x3894:
    /*00003894*/ MOV         R0, R8 
    /*00003898*/ ADD         R4, R4, #1 
    /*0000389c*/ BL          0x000000000000389C          @ osal_up [R_ARM_CALL: +0x0] 
    /*000038a0*/ CMP         R4, #4 
    /*000038a4*/ ADD         R5, R5, #20 
    /*000038a8*/ BNE         DIS_ProcChnAttr_x37d0
    /*000038ac*/ MOV         R0, #0 
    /*000038b0*/ ADD         SP, SP, #52                 @ 0x34 
    /*000038b4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_ProcChnAttr_x38b8:
    /*000038b8*/ MOV         R0, #65024                  @ 0xfe00 
    /*000038bc*/ MOVT        R0, #65535                  @ 0xffff 
    /*000038c0*/ ADD         SP, SP, #52                 @ 0x34 
    /*000038c4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    /*000038c8*/ ANDEQ       R0, R0, R0, LSR #32 
FUNC_END DIS_ProcChnAttr


FUNC_BEGIN DIS_ProcChnConfig
    /*000038cc*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000038d0*/ SUB         SP, SP, #60                 @ 0x3c 
    /*000038d4*/ MOV         R9, R0 
    /*000038d8*/ MOVW        R1, #0                      @ .LC102 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1208] 
    /*000038dc*/ MOVT        R1, #0                      @ .LC102 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1208] 
    /*000038e0*/ LDR         R5, [PC, #620]              @ 0x0000000000003b54 
    /*000038e4*/ BL          0x00000000000038E4          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*000038e8*/ MOVW        R2, #0                      @ .LC105 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12b0] 
    /*000038ec*/ MOVW        R3, #0                      @ .LC106 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12b8] 
    /*000038f0*/ MOVT        R2, #0                      @ .LC105 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12b0] 
    /*000038f4*/ MOVT        R3, #0                      @ .LC106 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12b8] 
    /*000038f8*/ STR         R2, [SP, #32] 
    /*000038fc*/ STR         R3, [SP, #28] 
    /*00003900*/ MOVW        R2, #0                      @ .LC107 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12c4] 
    /*00003904*/ MOVW        R3, #0                      @ .LC108 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12d4] 
    /*00003908*/ MOVT        R2, #0                      @ .LC107 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12c4] 
    /*0000390c*/ MOVT        R3, #0                      @ .LC108 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12d4] 
    /*00003910*/ STR         R2, [SP, #24] 
    /*00003914*/ STR         R3, [SP, #20] 
    /*00003918*/ MOVW        R2, #0                      @ .LC109 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12e0] 
    /*0000391c*/ MOVW        R3, #0                      @ .LC110 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12e8] 
    /*00003920*/ MOVT        R2, #0                      @ .LC109 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12e0] 
    /*00003924*/ MOVT        R3, #0                      @ .LC110 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12e8] 
    /*00003928*/ STR         R2, [SP, #16] 
    /*0000392c*/ STR         R3, [SP, #12] 
    /*00003930*/ MOVW        R2, #0                      @ .LC111 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12f4] 
    /*00003934*/ MOVW        R3, #0                      @ .LC112 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x12fc] 
    /*00003938*/ MOVT        R2, #0                      @ .LC111 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12f4] 
    /*0000393c*/ MOVT        R3, #0                      @ .LC112 [R_ARM_MOVT_ABS: .rodata.str1.4+0x12fc] 
    /*00003940*/ STR         R2, [SP, #8] 
    /*00003944*/ STR         R3, [SP, #4] 
    /*00003948*/ MOVW        R2, #0                      @ .LC113 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1304] 
    /*0000394c*/ MOV         R0, R9 
    /*00003950*/ MOVT        R2, #0                      @ .LC113 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1304] 
    /*00003954*/ MOVW        R3, #0                      @ .LC103 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x127c] 
    /*00003958*/ STR         R2, [SP] 
    /*0000395c*/ MOVT        R3, #0                      @ .LC103 [R_ARM_MOVT_ABS: .rodata.str1.4+0x127c] 
    /*00003960*/ MOVW        R2, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1054] 
    /*00003964*/ MOVW        R1, #0                      @ .LC104 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1284] 
    /*00003968*/ MOVT        R2, #0                      @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1054] 
    /*0000396c*/ MOVT        R1, #0                      @ .LC104 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1284] 
    /*00003970*/ MOVW        R8, #0                      @ .LC79 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10bc] 
    /*00003974*/ BL          0x0000000000003974          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003978*/ MOV         R0, R9 
    /*0000397c*/ MOVW        R1, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a0] 
    /*00003980*/ MOVT        R1, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a0] 
    /*00003984*/ BL          0x0000000000003984          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003988*/ MOVW        R7, #0                      @ .LC80 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10c0] 
    /*0000398c*/ SUB         SL, R5, #16 
    /*00003990*/ SUB         R6, R5, #20 
    /*00003994*/ MOVT        R8, #0                      @ .LC79 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10bc] 
    /*00003998*/ MOVT        R7, #0                      @ .LC80 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10c0] 
    /*0000399c*/ MOV         R4, #0 
    DIS_ProcChnConfig_x39a0:
    /*000039a0*/ MOV         R0, R6 
    /*000039a4*/ BL          0x00000000000039A4          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*000039a8*/ CMP         R0, #0 
    /*000039ac*/ BNE         DIS_ProcChnConfig_x3b44
    /*000039b0*/ LDR         R0, [SL], #4 
    /*000039b4*/ CMP         R0, #0 
    /*000039b8*/ BEQ         DIS_ProcChnConfig_x3afc
    /*000039bc*/ LDR         R3, [R5, #8] 
    /*000039c0*/ CMP         R3, #0 
    /*000039c4*/ BEQ         DIS_ProcChnConfig_x3afc
    /*000039c8*/ LDR         R2, [R0, #444]              @ 0x1bc 
    /*000039cc*/ CMP         R2, #0 
    /*000039d0*/ MOVWEQ      R3, #0                      @ .LC92 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11c0] 
    /*000039d4*/ MOVTEQ      R3, #0                      @ .LC92 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11c0] 
    /*000039d8*/ BEQ         DIS_ProcChnConfig_x39fc
    /*000039dc*/ CMP         R2, #1 
    /*000039e0*/ BEQ         DIS_ProcChnConfig_x3b20
    /*000039e4*/ MOVW        R1, #0                      @ .LC94 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11d8] 
    /*000039e8*/ CMP         R2, #2 
    /*000039ec*/ MOVT        R1, #0                      @ .LC94 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11d8] 
    /*000039f0*/ MOVW        R3, #0                      @ .LC95 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11e0] 
    /*000039f4*/ MOVT        R3, #0                      @ .LC95 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11e0] 
    /*000039f8*/ MOVEQ       R3, R1 
    DIS_ProcChnConfig_x39fc:
    /*000039fc*/ LDR         R2, [R0, #96]               @ 0x60 
    /*00003a00*/ CMP         R2, #0 
    /*00003a04*/ MOVWEQ      IP, #0                      @ .LC96 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11e4] 
    /*00003a08*/ MOVTEQ      IP, #0                      @ .LC96 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11e4] 
    /*00003a0c*/ BEQ         DIS_ProcChnConfig_x3a30
    /*00003a10*/ CMP         R2, #1 
    /*00003a14*/ BEQ         DIS_ProcChnConfig_x3b38
    /*00003a18*/ MOVW        R1, #0                      @ .LC95 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11e0] 
    /*00003a1c*/ CMP         R2, #2 
    /*00003a20*/ MOVT        R1, #0                      @ .LC95 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11e0] 
    /*00003a24*/ MOVW        IP, #0                      @ .LC98 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11f0] 
    /*00003a28*/ MOVT        IP, #0                      @ .LC98 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11f0] 
    /*00003a2c*/ MOVNE       IP, R1 
    DIS_ProcChnConfig_x3a30:
    /*00003a30*/ LDR         R2, [R0, #44]               @ 0x2c 
    /*00003a34*/ CMP         R2, #0 
    /*00003a38*/ MOVWEQ      LR, #0                      @ .LC99 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11f8] 
    /*00003a3c*/ MOVTEQ      LR, #0                      @ .LC99 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11f8] 
    /*00003a40*/ BEQ         DIS_ProcChnConfig_x3a64
    /*00003a44*/ CMP         R2, #1 
    /*00003a48*/ BEQ         DIS_ProcChnConfig_x3b2c
    /*00003a4c*/ MOVW        R1, #0                      @ .LC95 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11e0] 
    /*00003a50*/ CMP         R2, #2 
    /*00003a54*/ MOVT        R1, #0                      @ .LC95 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11e0] 
    /*00003a58*/ MOVW        LR, #0                      @ .LC101 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1200] 
    /*00003a5c*/ MOVT        LR, #0                      @ .LC101 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1200] 
    /*00003a60*/ MOVNE       LR, R1 
    DIS_ProcChnConfig_x3a64:
    /*00003a64*/ LDR         R2, [R0, #448]              @ 0x1c0 
    /*00003a68*/ LDR         FP, [R0, #104]              @ 0x68 
    /*00003a6c*/ LDR         R1, [R0, #452]              @ 0x1c4 
    /*00003a70*/ STR         R2, [SP, #40]               @ 0x28 
    /*00003a74*/ CMP         FP, #1 
    /*00003a78*/ LDR         R2, [R0, #108]              @ 0x6c 
    /*00003a7c*/ MOVEQ       FP, R8 
    /*00003a80*/ MOVNE       FP, R7 
    /*00003a84*/ STR         R1, [SP, #44]               @ 0x2c 
    /*00003a88*/ MOVW        R1, #0                      @ .LC114 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x130c] 
    /*00003a8c*/ STR         R2, [SP, #48]               @ 0x30 
    /*00003a90*/ MOVT        R1, #0                      @ .LC114 [R_ARM_MOVT_ABS: .rodata.str1.4+0x130c] 
    /*00003a94*/ LDR         R2, [R0, #304]              @ 0x130 
    /*00003a98*/ STR         R2, [SP, #52]               @ 0x34 
    /*00003a9c*/ LDR         R2, [R0, #456]              @ 0x1c8 
    /*00003aa0*/ LDRB        R0, [R0, #308]              @ 0x134 
    /*00003aa4*/ STR         FP, [SP, #28] 
    /*00003aa8*/ CMP         R2, #1 
    /*00003aac*/ LDR         FP, [SP, #52]               @ 0x34 
    /*00003ab0*/ MOVEQ       R2, R8 
    /*00003ab4*/ MOVNE       R2, R7 
    /*00003ab8*/ STR         R0, [SP, #24] 
    /*00003abc*/ MOV         R0, R9 
    /*00003ac0*/ STR         FP, [SP, #20] 
    /*00003ac4*/ LDR         FP, [SP, #48]               @ 0x30 
    /*00003ac8*/ STR         R2, [SP, #32] 
    /*00003acc*/ MOV         R2, R4 
    /*00003ad0*/ STM         SP, {IP, LR} 
    /*00003ad4*/ STR         FP, [SP, #16] 
    /*00003ad8*/ LDR         FP, [SP, #44]               @ 0x2c 
    /*00003adc*/ STR         FP, [SP, #12] 
    /*00003ae0*/ LDR         FP, [SP, #40]               @ 0x28 
    /*00003ae4*/ STR         FP, [SP, #8] 
    /*00003ae8*/ BL          0x0000000000003AE8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003aec*/ MOVW        R1, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a0] 
    /*00003af0*/ MOV         R0, R9 
    /*00003af4*/ MOVT        R1, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a0] 
    /*00003af8*/ BL          0x0000000000003AF8          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    DIS_ProcChnConfig_x3afc:
    /*00003afc*/ MOV         R0, R6 
    /*00003b00*/ ADD         R4, R4, #1 
    /*00003b04*/ BL          0x0000000000003B04          @ osal_up [R_ARM_CALL: +0x0] 
    /*00003b08*/ CMP         R4, #4 
    /*00003b0c*/ ADD         R5, R5, #20 
    /*00003b10*/ BNE         DIS_ProcChnConfig_x39a0
    /*00003b14*/ MOV         R0, #0 
    /*00003b18*/ ADD         SP, SP, #60                 @ 0x3c 
    /*00003b1c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_ProcChnConfig_x3b20:
    /*00003b20*/ MOVW        R3, #0                      @ .LC93 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11cc] 
    /*00003b24*/ MOVT        R3, #0                      @ .LC93 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11cc] 
    /*00003b28*/ B           DIS_ProcChnConfig_x39fc
    DIS_ProcChnConfig_x3b2c:
    /*00003b2c*/ MOVW        LR, #0                      @ .LC100 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11fc] 
    /*00003b30*/ MOVT        LR, #0                      @ .LC100 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11fc] 
    /*00003b34*/ B           DIS_ProcChnConfig_x3a64
    DIS_ProcChnConfig_x3b38:
    /*00003b38*/ MOVW        IP, #0                      @ .LC97 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x11e8] 
    /*00003b3c*/ MOVT        IP, #0                      @ .LC97 [R_ARM_MOVT_ABS: .rodata.str1.4+0x11e8] 
    /*00003b40*/ B           DIS_ProcChnConfig_x3a30
    DIS_ProcChnConfig_x3b44:
    /*00003b44*/ MOV         R0, #65024                  @ 0xfe00 
    /*00003b48*/ MOVT        R0, #65535                  @ 0xffff 
    /*00003b4c*/ ADD         SP, SP, #60                 @ 0x3c 
    /*00003b50*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    /*00003b54*/ ANDEQ       R0, R0, R0, LSR #32 
FUNC_END DIS_ProcChnConfig


FUNC_BEGIN DIS_ProcDbgInfo
    /*00003b58*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00003b5c*/ SUB         SP, SP, #60                 @ 0x3c 
    /*00003b60*/ LDR         R8, [PC, #716]              @ 0x0000000000003e34 
    /*00003b64*/ MOV         R6, R0 
    /*00003b68*/ MOVW        R1, #0                      @ .LC115 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1338] 
    /*00003b6c*/ MOVT        R1, #0                      @ .LC115 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1338] 
    /*00003b70*/ BL          0x0000000000003B70          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003b74*/ SUB         FP, R8, #16 
    /*00003b78*/ MOVW        R2, #0                      @ .LC118 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13dc] 
    /*00003b7c*/ MOVW        R3, #0                      @ .LC119 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13e4] 
    /*00003b80*/ MOVT        R2, #0                      @ .LC118 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13dc] 
    /*00003b84*/ MOVT        R3, #0                      @ .LC119 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13e4] 
    /*00003b88*/ STR         R2, [SP, #32] 
    /*00003b8c*/ MOV         R0, R6 
    /*00003b90*/ STR         R3, [SP, #28] 
    /*00003b94*/ MOVW        R2, #0                      @ .LC120 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13ec] 
    /*00003b98*/ MOVW        R3, #0                      @ .LC121 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13f8] 
    /*00003b9c*/ MOVT        R2, #0                      @ .LC120 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13ec] 
    /*00003ba0*/ MOVT        R3, #0                      @ .LC121 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13f8] 
    /*00003ba4*/ STR         R2, [SP, #24] 
    /*00003ba8*/ STR         R3, [SP, #20] 
    /*00003bac*/ MOVW        R2, #0                      @ .LC122 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1404] 
    /*00003bb0*/ MOVW        R3, #0                      @ .LC123 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1410] 
    /*00003bb4*/ MOVT        R2, #0                      @ .LC122 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1404] 
    /*00003bb8*/ MOVT        R3, #0                      @ .LC123 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1410] 
    /*00003bbc*/ STR         R2, [SP, #16] 
    /*00003bc0*/ STR         R3, [SP, #12] 
    /*00003bc4*/ MOVW        R2, #0                      @ .LC124 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x141c] 
    /*00003bc8*/ MOVW        R3, #0                      @ .LC125 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1424] 
    /*00003bcc*/ MOVT        R2, #0                      @ .LC124 [R_ARM_MOVT_ABS: .rodata.str1.4+0x141c] 
    /*00003bd0*/ MOVT        R3, #0                      @ .LC125 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1424] 
    /*00003bd4*/ STR         R2, [SP, #8] 
    /*00003bd8*/ STR         R3, [SP, #4] 
    /*00003bdc*/ MOVW        R2, #0                      @ .LC126 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x142c] 
    /*00003be0*/ MOVW        R3, #0                      @ .LC116 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13a8] 
    /*00003be4*/ MOVT        R2, #0                      @ .LC126 [R_ARM_MOVT_ABS: .rodata.str1.4+0x142c] 
    /*00003be8*/ MOVT        R3, #0                      @ .LC116 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13a8] 
    /*00003bec*/ STR         R2, [SP] 
    /*00003bf0*/ MOVW        R1, #0                      @ .LC117 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x13b0] 
    /*00003bf4*/ MOVW        R2, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1054] 
    /*00003bf8*/ MOVT        R1, #0                      @ .LC117 [R_ARM_MOVT_ABS: .rodata.str1.4+0x13b0] 
    /*00003bfc*/ MOVT        R2, #0                      @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1054] 
    /*00003c00*/ BL          0x0000000000003C00          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003c04*/ MOV         R0, R6 
    /*00003c08*/ MOVW        R1, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a0] 
    /*00003c0c*/ MOVT        R1, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a0] 
    /*00003c10*/ BL          0x0000000000003C10          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003c14*/ SUB         SL, R8, #20 
    /*00003c18*/ MOV         R9, R8 
    /*00003c1c*/ STR         R8, [SP, #52]               @ 0x34 
    /*00003c20*/ MOV         R7, #0 
    /*00003c24*/ MOV         R8, FP 
    /*00003c28*/ MOVW        R3, #0                      @ .LC127 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1434] 
    /*00003c2c*/ MOVW        R2, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a0] 
    /*00003c30*/ MOVT        R3, #0                      @ .LC127 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1434] 
    /*00003c34*/ MOVT        R2, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a0] 
    /*00003c38*/ STR         R3, [SP, #40]               @ 0x28 
    /*00003c3c*/ STR         R2, [SP, #44]               @ 0x2c 
    /*00003c40*/ STR         FP, [SP, #48]               @ 0x30 
    DIS_ProcDbgInfo_x3c44:
    /*00003c44*/ MOV         R0, SL 
    /*00003c48*/ BL          0x0000000000003C48          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00003c4c*/ SUBS        R5, R0, #0 
    /*00003c50*/ MOV         R0, SL 
    /*00003c54*/ BNE         DIS_ProcDbgInfo_x3e24
    /*00003c58*/ LDR         R4, [R8], #4 
    /*00003c5c*/ CMP         R4, #0 
    /*00003c60*/ BEQ         DIS_ProcDbgInfo_x3ce4
    /*00003c64*/ LDR         R3, [R9, #8] 
    /*00003c68*/ CMP         R3, #0 
    /*00003c6c*/ BEQ         DIS_ProcDbgInfo_x3ce4
    /*00003c70*/ MOV         R0, R4 
    /*00003c74*/ BL          0x0000000000003C74          @ DIS_GetFreeNodeNum [R_ARM_CALL: .text+0x444] 
    /*00003c78*/ MOV         FP, R0 
    /*00003c7c*/ MOV         R0, R4 
    /*00003c80*/ BL          0x0000000000003C80          @ DIS_GetBusyNodeNum [R_ARM_CALL: .text+0x43c] 
    /*00003c84*/ LDR         R1, [R4, #604]              @ 0x25c 
    /*00003c88*/ MOV         R3, #20 
    /*00003c8c*/ MOV         R2, R7 
    /*00003c90*/ STR         R1, [SP, #32] 
    /*00003c94*/ LDR         IP, [R4, #608]              @ 0x260 
    /*00003c98*/ LDR         R1, [SP, #40]               @ 0x28 
    /*00003c9c*/ STR         IP, [SP, #28] 
    /*00003ca0*/ LDR         IP, [R4, #548]              @ 0x224 
    /*00003ca4*/ STR         IP, [SP, #24] 
    /*00003ca8*/ LDR         IP, [R4, #544]              @ 0x220 
    /*00003cac*/ STR         IP, [SP, #20] 
    /*00003cb0*/ LDR         IP, [R4, #596]              @ 0x254 
    /*00003cb4*/ STR         IP, [SP, #16] 
    /*00003cb8*/ LDR         IP, [R4, #592]              @ 0x250 
    /*00003cbc*/ STR         IP, [SP, #12] 
    /*00003cc0*/ LDR         IP, [R4, #588]              @ 0x24c 
    /*00003cc4*/ STR         FP, [SP] 
    /*00003cc8*/ STMIB       SP, {R0, IP} 
    /*00003ccc*/ MOV         R0, R6 
    /*00003cd0*/ BL          0x0000000000003CD0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003cd4*/ LDR         R1, [SP, #44]               @ 0x2c 
    /*00003cd8*/ MOV         R0, R6 
    /*00003cdc*/ BL          0x0000000000003CDC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003ce0*/ MOV         R0, SL 
    DIS_ProcDbgInfo_x3ce4:
    /*00003ce4*/ ADD         R7, R7, #1 
    /*00003ce8*/ BL          0x0000000000003CE8          @ osal_up [R_ARM_CALL: +0x0] 
    /*00003cec*/ CMP         R7, #4 
    /*00003cf0*/ ADD         R9, R9, #20 
    /*00003cf4*/ BNE         DIS_ProcDbgInfo_x3c44
    /*00003cf8*/ MOVW        R1, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a0] 
    /*00003cfc*/ MOV         R0, R6 
    /*00003d00*/ MOVT        R1, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a0] 
    /*00003d04*/ LDR         FP, [SP, #48]               @ 0x30 
    /*00003d08*/ LDR         R8, [SP, #52]               @ 0x34 
    /*00003d0c*/ BL          0x0000000000003D0C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003d10*/ MOVW        R2, #0                      @ .LC130 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1484] 
    /*00003d14*/ MOVW        R3, #0                      @ .LC131 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1490] 
    /*00003d18*/ MOVT        R2, #0                      @ .LC130 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1484] 
    /*00003d1c*/ MOVT        R3, #0                      @ .LC131 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1490] 
    /*00003d20*/ STR         R2, [SP, #16] 
    /*00003d24*/ MOVW        R1, #0                      @ .LC129 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1468] 
    /*00003d28*/ STR         R3, [SP, #12] 
    /*00003d2c*/ MOVT        R1, #0                      @ .LC129 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1468] 
    /*00003d30*/ MOVW        R2, #0                      @ .LC132 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x149c] 
    /*00003d34*/ MOVW        R3, #0                      @ .LC133 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14a8] 
    /*00003d38*/ MOVT        R2, #0                      @ .LC132 [R_ARM_MOVT_ABS: .rodata.str1.4+0x149c] 
    /*00003d3c*/ MOVT        R3, #0                      @ .LC133 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14a8] 
    /*00003d40*/ STR         R2, [SP, #8] 
    /*00003d44*/ MOV         R0, R6 
    /*00003d48*/ STR         R3, [SP, #4] 
    /*00003d4c*/ MOVW        R2, #0                      @ .LC134 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14b0] 
    /*00003d50*/ MOVW        R3, #0                      @ .LC128 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1460] 
    /*00003d54*/ MOVT        R2, #0                      @ .LC134 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14b0] 
    /*00003d58*/ MOVT        R3, #0                      @ .LC128 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1460] 
    /*00003d5c*/ STR         R2, [SP] 
    /*00003d60*/ MOVW        R2, #0                      @ .LC71 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1054] 
    /*00003d64*/ MOVT        R2, #0                      @ .LC71 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1054] 
    /*00003d68*/ BL          0x0000000000003D68          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003d6c*/ MOVW        R1, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a0] 
    /*00003d70*/ MOV         R0, R6 
    /*00003d74*/ MOVT        R1, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a0] 
    /*00003d78*/ BL          0x0000000000003D78          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003d7c*/ LDR         R4, [PC, #180]              @ 0x0000000000003e38 
    /*00003d80*/ MOVW        R7, #0                      @ .LC135 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14b8] 
    /*00003d84*/ MOVW        R9, #0                      @ .LC77 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x10a0] 
    /*00003d88*/ MOVT        R7, #0                      @ .LC135 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14b8] 
    /*00003d8c*/ MOVT        R9, #0                      @ .LC77 [R_ARM_MOVT_ABS: .rodata.str1.4+0x10a0] 
    DIS_ProcDbgInfo_x3d90:
    /*00003d90*/ MOV         R0, R4 
    /*00003d94*/ BL          0x0000000000003D94          @ osal_down_interruptible [R_ARM_CALL: +0x0] 
    /*00003d98*/ MOV         R2, R5 
    /*00003d9c*/ MOV         R1, R7 
    /*00003da0*/ CMP         R0, #0 
    /*00003da4*/ MOV         R0, R6 
    /*00003da8*/ BNE         DIS_ProcDbgInfo_x3e24
    /*00003dac*/ LDR         IP, [FP], #4 
    /*00003db0*/ CMP         IP, #0 
    /*00003db4*/ BEQ         DIS_ProcDbgInfo_x3e00
    /*00003db8*/ LDR         R3, [R8, #8] 
    /*00003dbc*/ CMP         R3, #0 
    /*00003dc0*/ BEQ         DIS_ProcDbgInfo_x3e00
    /*00003dc4*/ LDR         LR, [IP, #660]              @ 0x294 
    /*00003dc8*/ LDR         R3, [IP, #628]              @ 0x274 
    /*00003dcc*/ STR         LR, [SP, #16] 
    /*00003dd0*/ LDR         LR, [IP, #652]              @ 0x28c 
    /*00003dd4*/ STR         LR, [SP, #12] 
    /*00003dd8*/ LDR         LR, [IP, #632]              @ 0x278 
    /*00003ddc*/ STR         LR, [SP, #8] 
    /*00003de0*/ LDR         LR, [IP, #656]              @ 0x290 
    /*00003de4*/ STR         LR, [SP, #4] 
    /*00003de8*/ LDR         IP, [IP, #648]              @ 0x288 
    /*00003dec*/ STR         IP, [SP] 
    /*00003df0*/ BL          0x0000000000003DF0          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003df4*/ MOV         R1, R9 
    /*00003df8*/ MOV         R0, R6 
    /*00003dfc*/ BL          0x0000000000003DFC          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    DIS_ProcDbgInfo_x3e00:
    /*00003e00*/ MOV         R0, R4 
    /*00003e04*/ ADD         R5, R5, #1 
    /*00003e08*/ BL          0x0000000000003E08          @ osal_up [R_ARM_CALL: +0x0] 
    /*00003e0c*/ CMP         R5, #4 
    /*00003e10*/ ADD         R8, R8, #20 
    /*00003e14*/ BNE         DIS_ProcDbgInfo_x3d90
    /*00003e18*/ MOV         R0, #0 
    /*00003e1c*/ ADD         SP, SP, #60                 @ 0x3c 
    /*00003e20*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_ProcDbgInfo_x3e24:
    /*00003e24*/ MOV         R0, #65024                  @ 0xfe00 
    /*00003e28*/ MOVT        R0, #65535                  @ 0xffff 
    /*00003e2c*/ ADD         SP, SP, #60                 @ 0x3c 
    /*00003e30*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    /*00003e34*/ ANDEQ       R0, R0, R0, LSR #32 
    /*00003e38*/ ANDEQ       R0, R0, IP 
FUNC_END DIS_ProcDbgInfo


FUNC_BEGIN DIS_ProcShow
    /*00003e3c*/ PUSH        {R4, LR} 
    /*00003e40*/ MOVW        R1, #0                      @ .LC136 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x14d4] 
    /*00003e44*/ MOV         R4, R0 
    /*00003e48*/ MOVT        R1, #0                      @ .LC136 [R_ARM_MOVT_ABS: .rodata.str1.4+0x14d4] 
    /*00003e4c*/ BL          0x0000000000003E4C          @ osal_seq_printf [R_ARM_CALL: +0x0] 
    /*00003e50*/ MOV         R0, R4 
    /*00003e54*/ BL          0x0000000000003E54          @ DIS_ProcChnConfig [R_ARM_CALL: .text+0x38cc] 
    /*00003e58*/ MOV         R0, R4 
    /*00003e5c*/ BL          0x0000000000003E5C          @ DIS_ProcChnAttr [R_ARM_CALL: .text+0x3710] 
    /*00003e60*/ MOV         R0, R4 
    /*00003e64*/ BL          0x0000000000003E64          @ DIS_ProcChnSysConfig [R_ARM_CALL: .text+0x35d8] 
    /*00003e68*/ MOV         R0, R4 
    /*00003e6c*/ BL          0x0000000000003E6C          @ DIS_ProcDbgInfo [R_ARM_CALL: .text+0x3b58] 
    /*00003e70*/ MOV         R0, #0 
    /*00003e74*/ POP         {R4, PC} 
FUNC_END DIS_ProcShow


FUNC_BEGIN DIS_DestroyAllChn
    /*00003e78*/ PUSH        {R4, R5, LR} 
    /*00003e7c*/ SUB         SP, SP, #12 
    /*00003e80*/ LDR         R5, [PC, #72]               @ 0x0000000000003ed0 
    /*00003e84*/ MOV         R4, #0 
    DIS_DestroyAllChn_x3e88:
    /*00003e88*/ LDR         R3, [R5], #4 
    /*00003e8c*/ MOV         R0, R4 
    /*00003e90*/ ADD         R4, R4, #1 
    /*00003e94*/ CMP         R3, #0 
    /*00003e98*/ BEQ         DIS_DestroyAllChn_x3ea0
    /*00003e9c*/ BL          0x0000000000003E9C          @ DIS_DestroyChn [R_ARM_CALL: .text+0x19c0] 
    DIS_DestroyAllChn_x3ea0:
    /*00003ea0*/ CMP         R4, #4 
    /*00003ea4*/ BNE         DIS_DestroyAllChn_x3e88
    /*00003ea8*/ MOVW        R3, #2474                   @ 0x9aa 
    /*00003eac*/ MOV         R1, #17 
    /*00003eb0*/ STR         R3, [SP] 
    /*00003eb4*/ MOVW        R2, #0                      @ .LC137 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1534] 
    /*00003eb8*/ LDR         R3, [PC, #20]               @ 0x0000000000003ed4 
    /*00003ebc*/ MOVT        R2, #0                      @ .LC137 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1534] 
    /*00003ec0*/ MOV         R0, #7 
    /*00003ec4*/ BL          0x0000000000003EC4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00003ec8*/ ADD         SP, SP, #12 
    /*00003ecc*/ POP         {R4, R5, PC} 
    /*00003ed0*/ ANDEQ       R0, R0, R0, LSL R0 
    /*00003ed4*/ ANDEQ       R0, R0, R8, LSL #5 
FUNC_END DIS_DestroyAllChn


FUNC_BEGIN DIS_Exit
    /*00003ed8*/ PUSH        {R4, LR} 
    /*00003edc*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00003ee0*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00003ee4*/ SUB         SP, SP, #8 
    /*00003ee8*/ LDR         R3, [R4, #4] 
    /*00003eec*/ CMP         R3, #0 
    /*00003ef0*/ BEQ         DIS_Exit_x3f34
    /*00003ef4*/ BL          0x0000000000003EF4          @ DIS_DestroyAllChn [R_ARM_CALL: .text+0x3e78] 
    /*00003ef8*/ BL          0x0000000000003EF8          @ DIS_DrvCommExit [R_ARM_CALL: .text+0x58ac] 
    /*00003efc*/ BL          0x0000000000003EFC          @ DIS_DrvCommClose [R_ARM_CALL: .text+0x5814] 
    /*00003f00*/ MOV         R3, #0 
    /*00003f04*/ STR         R3, [R4, #4] 
    /*00003f08*/ BL          0x0000000000003F08          @ DIS_DRV_SOFTRSTRegExit [R_ARM_CALL: .text+0x594c] 
    /*00003f0c*/ ADD         R0, R4, #8 
    /*00003f10*/ BL          0x0000000000003F10          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*00003f14*/ MOVW        R3, #2501                   @ 0x9c5 
    /*00003f18*/ MOVW        R2, #0                      @ .LC138 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x156c] 
    /*00003f1c*/ STR         R3, [SP] 
    /*00003f20*/ MOVT        R2, #0                      @ .LC138 [R_ARM_MOVT_ABS: .rodata.str1.4+0x156c] 
    /*00003f24*/ LDR         R3, [PC, #16]               @ 0x0000000000003f3c 
    /*00003f28*/ MOV         R1, #17 
    /*00003f2c*/ MOV         R0, #6 
    /*00003f30*/ BL          0x0000000000003F30          @ HI_LOG [R_ARM_CALL: +0x0] 
    DIS_Exit_x3f34:
    /*00003f34*/ ADD         SP, SP, #8 
    /*00003f38*/ POP         {R4, PC} 
    /*00003f3c*/ MULEQ       R0, IP, R2 
FUNC_END DIS_Exit


FUNC_BEGIN DIS_SynchCtxInit
    /*00003f40*/ PUSH        {R4, R5, LR} 
    /*00003f44*/ SUB         SP, SP, #12 
    /*00003f48*/ LDR         R4, [PC, #192]              @ 0x0000000000004010 
    /*00003f4c*/ ADD         R5, R4, #80                 @ 0x50 
    DIS_SynchCtxInit_x3f50:
    /*00003f50*/ MOV         R0, R4 
    /*00003f54*/ BL          0x0000000000003F54          @ osal_wait_init [R_ARM_CALL: +0x0] 
    /*00003f58*/ CMP         R0, #0 
    /*00003f5c*/ ADD         R0, R4, #4 
    /*00003f60*/ ADD         R4, R4, #20 
    /*00003f64*/ BNE         DIS_SynchCtxInit_x3f84
    /*00003f68*/ BL          0x0000000000003F68          @ osal_spin_lock_init [R_ARM_CALL: +0x0] 
    /*00003f6c*/ CMP         R0, #0 
    /*00003f70*/ BNE         DIS_SynchCtxInit_x3fec
    /*00003f74*/ CMP         R4, R5 
    /*00003f78*/ BNE         DIS_SynchCtxInit_x3f50
    /*00003f7c*/ ADD         SP, SP, #12 
    /*00003f80*/ POP         {R4, R5, PC} 
    DIS_SynchCtxInit_x3f84:
    /*00003f84*/ MOVW        R3, #2586                   @ 0xa1a 
    /*00003f88*/ MOVW        R2, #0                      @ .LC139 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15a4] 
    /*00003f8c*/ STR         R3, [SP] 
    /*00003f90*/ MOVT        R2, #0                      @ .LC139 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15a4] 
    /*00003f94*/ LDR         R3, [PC, #120]              @ 0x0000000000004014 
    /*00003f98*/ MOV         R1, #17 
    /*00003f9c*/ MOV         R0, #3 
    /*00003fa0*/ BL          0x0000000000003FA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    DIS_SynchCtxInit_x3fa4:
    /*00003fa4*/ LDR         R4, [PC, #108]              @ 0x0000000000004018 
    /*00003fa8*/ ADD         R5, R4, #80                 @ 0x50 
    DIS_SynchCtxInit_x3fac:
    /*00003fac*/ LDR         R3, [R4, #12] 
    /*00003fb0*/ ADD         R0, R4, #12 
    /*00003fb4*/ CMP         R3, #0 
    /*00003fb8*/ BEQ         DIS_SynchCtxInit_x3fc0
    /*00003fbc*/ BL          0x0000000000003FBC          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    DIS_SynchCtxInit_x3fc0:
    /*00003fc0*/ LDR         R3, [R4, #16] 
    /*00003fc4*/ ADD         R0, R4, #16 
    /*00003fc8*/ ADD         R4, R4, #20 
    /*00003fcc*/ CMP         R3, #0 
    /*00003fd0*/ BEQ         DIS_SynchCtxInit_x3fd8
    /*00003fd4*/ BL          0x0000000000003FD4          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    DIS_SynchCtxInit_x3fd8:
    /*00003fd8*/ CMP         R4, R5 
    /*00003fdc*/ BNE         DIS_SynchCtxInit_x3fac
    /*00003fe0*/ MVN         R0, #0 
    /*00003fe4*/ ADD         SP, SP, #12 
    /*00003fe8*/ POP         {R4, R5, PC} 
    DIS_SynchCtxInit_x3fec:
    /*00003fec*/ MOV         R3, #2592                   @ 0xa20 
    /*00003ff0*/ MOVW        R2, #0                      @ .LC140 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x15d8] 
    /*00003ff4*/ STR         R3, [SP] 
    /*00003ff8*/ MOVT        R2, #0                      @ .LC140 [R_ARM_MOVT_ABS: .rodata.str1.4+0x15d8] 
    /*00003ffc*/ LDR         R3, [PC, #16]               @ 0x0000000000004014 
    /*00004000*/ MOV         R1, #17 
    /*00004004*/ MOV         R0, #3 
    /*00004008*/ BL          0x0000000000004008          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000400c*/ B           DIS_SynchCtxInit_x3fa4
    /*00004010*/ ANDEQ       R0, R0, IP, LSR #32 
    /*00004014*/ ANDEQ       R0, R0, R8, LSR #5 
    /*00004018*/ ANDEQ       R0, R0, R0, LSR #32 
FUNC_END DIS_SynchCtxInit


FUNC_BEGIN DIS_SynchCtxDeInit
    /*0000401c*/ PUSH        {R4, LR} 
    /*00004020*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00004024*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*00004028*/ ADD         R0, R4, #44                 @ 0x2c 
    /*0000402c*/ BL          0x000000000000402C          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00004030*/ ADD         R0, R4, #48                 @ 0x30 
    /*00004034*/ BL          0x0000000000004034          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00004038*/ ADD         R0, R4, #64                 @ 0x40 
    /*0000403c*/ BL          0x000000000000403C          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00004040*/ ADD         R0, R4, #68                 @ 0x44 
    /*00004044*/ BL          0x0000000000004044          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00004048*/ ADD         R0, R4, #84                 @ 0x54 
    /*0000404c*/ BL          0x000000000000404C          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00004050*/ ADD         R0, R4, #88                 @ 0x58 
    /*00004054*/ BL          0x0000000000004054          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00004058*/ ADD         R0, R4, #104                @ 0x68 
    /*0000405c*/ BL          0x000000000000405C          @ osal_wait_destory [R_ARM_CALL: +0x0] 
    /*00004060*/ ADD         R0, R4, #108                @ 0x6c 
    /*00004064*/ POP         {R4, LR} 
    DIS_SynchCtxDeInit_x4068:
    /*00004068*/ B           DIS_SynchCtxDeInit_x4068    @ osal_spin_lock_destory [R_ARM_JUMP24: +0x0] 
FUNC_END DIS_SynchCtxDeInit


FUNC_BEGIN DIS_ModInit
    /*0000406c*/ PUSH        {R4, LR} 
    /*00004070*/ SUB         SP, SP, #8 
    /*00004074*/ LDR         R0, [PC, #380]              @ 0x00000000000041f8 
    /*00004078*/ BL          0x0000000000004078          @ CMPI_RegisterModule [R_ARM_CALL: +0x0] 
    /*0000407c*/ CMP         R0, #0 
    /*00004080*/ BNE         DIS_ModInit_x4108
    /*00004084*/ MOV         R1, #1 
    /*00004088*/ LDR         R0, [PC, #364]              @ 0x00000000000041fc 
    /*0000408c*/ BL          0x000000000000408C          @ osal_sema_init [R_ARM_CALL: +0x0] 
    /*00004090*/ SUBS        R1, R0, #0 
    /*00004094*/ BNE         DIS_ModInit_x4144
    /*00004098*/ MOVW        R0, #0                      @ .LC143 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1674] 
    /*0000409c*/ MOVT        R0, #0                      @ .LC143 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1674] 
    /*000040a0*/ BL          0x00000000000040A0          @ osal_create_proc_entry [R_ARM_CALL: +0x0] 
    /*000040a4*/ CMP         R0, #0 
    /*000040a8*/ BEQ         DIS_ModInit_x41d4
    /*000040ac*/ MOVW        R3, #0                      @ DIS_ProcShow [R_ARM_MOVW_ABS_NC: .text+0x3e3c] 
    /*000040b0*/ MOVT        R3, #0                      @ DIS_ProcShow [R_ARM_MOVT_ABS: .text+0x3e3c] 
    /*000040b4*/ STR         R3, [R0, #60]               @ 0x3c 
    /*000040b8*/ BL          0x00000000000040B8          @ DIS_SynchCtxInit [R_ARM_CALL: .text+0x3f40] 
    /*000040bc*/ CMP         R0, #0 
    /*000040c0*/ BNE         DIS_ModInit_x4170
    /*000040c4*/ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000040c8*/ MOVT        R0, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000040cc*/ BL          0x00000000000040CC          @ osal_atomic_init [R_ARM_CALL: +0x0] 
    /*000040d0*/ SUBS        R4, R0, #0 
    /*000040d4*/ BNE         DIS_ModInit_x41ac
    /*000040d8*/ MOV         R1, R4 
    /*000040dc*/ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*000040e0*/ MOVT        R0, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*000040e4*/ BL          0x00000000000040E4          @ osal_atomic_set [R_ARM_CALL: +0x0] 
    /*000040e8*/ MOVW        R1, #0                      @ .LC147 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170c] 
    /*000040ec*/ MOVW        R0, #0                      @ .LC148 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1718] 
    /*000040f0*/ MOVT        R1, #0                      @ .LC147 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170c] 
    /*000040f4*/ MOVT        R0, #0                      @ .LC148 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1718] 
    /*000040f8*/ BL          0x00000000000040F8          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000040fc*/ MOV         R0, R4 
    /*00004100*/ ADD         SP, SP, #8 
    /*00004104*/ POP         {R4, PC} 
    DIS_ModInit_x4108:
    /*00004108*/ MOV         R3, #2736                   @ 0xab0 
    /*0000410c*/ MOVW        R2, #0                      @ .LC141 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1610] 
    /*00004110*/ STR         R3, [SP] 
    /*00004114*/ MOVT        R2, #0                      @ .LC141 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1610] 
    /*00004118*/ LDR         R3, [PC, #224]              @ 0x0000000000004200 
    /*0000411c*/ MOV         R1, #17 
    /*00004120*/ MOV         R0, #3 
    /*00004124*/ BL          0x0000000000004124          @ HI_LOG [R_ARM_CALL: +0x0] 
    DIS_ModInit_x4128:
    /*00004128*/ MOVW        R0, #0                      @ .LC149 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1734] 
    /*0000412c*/ MOVT        R0, #0                      @ .LC149 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1734] 
    /*00004130*/ BL          0x0000000000004130          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00004134*/ MVN         R4, #0 
    /*00004138*/ MOV         R0, R4 
    /*0000413c*/ ADD         SP, SP, #8 
    /*00004140*/ POP         {R4, PC} 
    DIS_ModInit_x4144:
    /*00004144*/ MOVW        R3, #2743                   @ 0xab7 
    /*00004148*/ MOVW        R2, #0                      @ .LC142 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1644] 
    /*0000414c*/ STR         R3, [SP] 
    /*00004150*/ MOVT        R2, #0                      @ .LC142 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1644] 
    /*00004154*/ LDR         R3, [PC, #164]              @ 0x0000000000004200 
    /*00004158*/ MOV         R1, #17 
    /*0000415c*/ MOV         R0, #3 
    /*00004160*/ BL          0x0000000000004160          @ HI_LOG [R_ARM_CALL: +0x0] 
    DIS_ModInit_x4164:
    /*00004164*/ MOV         R0, #17 
    /*00004168*/ BL          0x0000000000004168          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    /*0000416c*/ B           DIS_ModInit_x4128
    DIS_ModInit_x4170:
    /*00004170*/ MOVW        R3, #2759                   @ 0xac7 
    /*00004174*/ MOVW        R2, #0                      @ .LC145 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16a8] 
    /*00004178*/ STR         R3, [SP] 
    /*0000417c*/ MOVT        R2, #0                      @ .LC145 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16a8] 
    /*00004180*/ LDR         R3, [PC, #120]              @ 0x0000000000004200 
    /*00004184*/ MOV         R1, #17 
    /*00004188*/ MOV         R0, #3 
    /*0000418c*/ BL          0x000000000000418C          @ HI_LOG [R_ARM_CALL: +0x0] 
    DIS_ModInit_x4190:
    /*00004190*/ MOVW        R0, #0                      @ .LC143 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1674] 
    /*00004194*/ MOV         R1, #0 
    /*00004198*/ MOVT        R0, #0                      @ .LC143 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1674] 
    /*0000419c*/ BL          0x000000000000419C          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    DIS_ModInit_x41a0:
    /*000041a0*/ LDR         R0, [PC, #84]               @ 0x00000000000041fc 
    /*000041a4*/ BL          0x00000000000041A4          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*000041a8*/ B           DIS_ModInit_x4164
    DIS_ModInit_x41ac:
    /*000041ac*/ MOVW        R3, #2765                   @ 0xacd 
    /*000041b0*/ MOV         R1, #17 
    /*000041b4*/ STR         R3, [SP] 
    /*000041b8*/ MOVW        R2, #0                      @ .LC146 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x16dc] 
    /*000041bc*/ LDR         R3, [PC, #60]               @ 0x0000000000004200 
    /*000041c0*/ MOVT        R2, #0                      @ .LC146 [R_ARM_MOVT_ABS: .rodata.str1.4+0x16dc] 
    /*000041c4*/ MOV         R0, #3 
    /*000041c8*/ BL          0x00000000000041C8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000041cc*/ BL          0x00000000000041CC          @ DIS_SynchCtxDeInit [R_ARM_CALL: .text+0x401c] 
    /*000041d0*/ B           DIS_ModInit_x4190
    DIS_ModInit_x41d4:
    /*000041d4*/ MOVW        R3, #2750                   @ 0xabe 
    /*000041d8*/ MOVW        R2, #0                      @ .LC144 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1678] 
    /*000041dc*/ STR         R3, [SP] 
    /*000041e0*/ MOVT        R2, #0                      @ .LC144 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1678] 
    /*000041e4*/ LDR         R3, [PC, #20]               @ 0x0000000000004200 
    /*000041e8*/ MOV         R1, #17 
    /*000041ec*/ MOV         R0, #3 
    /*000041f0*/ BL          0x00000000000041F0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000041f4*/ B           DIS_ModInit_x41a0
    /*000041f8*/ ANDEQ       R0, R0, R4 
    /*000041fc*/ ANDEQ       R0, R0, IP 
    /*00004200*/ @           <UNDEFINED> instruction: 0x000002bc 
FUNC_END DIS_ModInit


FUNC_BEGIN DIS_ModExit
    /*00004204*/ PUSH        {R4, LR} 
    /*00004208*/ MOV         R1, #0 
    /*0000420c*/ MOVW        R0, #0                      @ .LC143 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1674] 
    /*00004210*/ MOVW        R4, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x0] 
    /*00004214*/ MOVT        R0, #0                      @ .LC143 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1674] 
    /*00004218*/ MOVT        R4, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x0] 
    /*0000421c*/ BL          0x000000000000421C          @ osal_remove_proc_entry [R_ARM_CALL: +0x0] 
    /*00004220*/ ADD         R0, R4, #12 
    /*00004224*/ BL          0x0000000000004224          @ osal_sema_destory [R_ARM_CALL: +0x0] 
    /*00004228*/ MOV         R0, #17 
    /*0000422c*/ BL          0x000000000000422C          @ CMPI_UnRegisterModule [R_ARM_CALL: +0x0] 
    /*00004230*/ MOV         R0, R4 
    /*00004234*/ BL          0x0000000000004234          @ osal_atomic_destory [R_ARM_CALL: +0x0] 
    /*00004238*/ BL          0x0000000000004238          @ DIS_SynchCtxDeInit [R_ARM_CALL: .text+0x401c] 
    /*0000423c*/ MOVW        R1, #0                      @ .LC147 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x170c] 
    /*00004240*/ MOVW        R0, #0                      @ .LC150 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x174c] 
    /*00004244*/ MOVT        R1, #0                      @ .LC147 [R_ARM_MOVT_ABS: .rodata.str1.4+0x170c] 
    /*00004248*/ MOVT        R0, #0                      @ .LC150 [R_ARM_MOVT_ABS: .rodata.str1.4+0x174c] 
    /*0000424c*/ POP         {R4, LR} 
    DIS_ModExit_x4250:
    /*00004250*/ B           DIS_ModExit_x4250           @ osal_printk [R_ARM_JUMP24: +0x0] 
FUNC_END DIS_ModExit


FUNC_BEGIN DIS_DebugOutputGMEStatus
    /*00004254*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00004258*/ SUB         SP, SP, #16 
    /*0000425c*/ SUB         R0, R0, #1 
    /*00004260*/ MOV         R1, #17 
    /*00004264*/ LDR         R5, [SP, #40]               @ 0x28 
    /*00004268*/ MOVW        R7, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x2c8] 
    /*0000426c*/ STRD        R2, [SP, #8] 
    /*00004270*/ MOVW        R2, #546                    @ 0x222 
    /*00004274*/ STR         R0, [SP, #4] 
    /*00004278*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x2c8] 
    /*0000427c*/ STR         R2, [SP] 
    /*00004280*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x2c8] 
    /*00004284*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1768] 
    /*00004288*/ MOV         R0, #7 
    /*0000428c*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1768] 
    /*00004290*/ MOVW        R6, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19a4] 
    /*00004294*/ BL          0x0000000000004294          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004298*/ LDR         R2, [R5] 
    /*0000429c*/ MOVW        R1, #547                    @ 0x223 
    /*000042a0*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x2c8] 
    /*000042a4*/ STR         R1, [SP] 
    /*000042a8*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x2c8] 
    /*000042ac*/ STR         R2, [SP, #4] 
    /*000042b0*/ MOV         R1, #17 
    /*000042b4*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17ac] 
    /*000042b8*/ MOV         R0, #7 
    /*000042bc*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17ac] 
    /*000042c0*/ MOVT        R6, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19a4] 
    /*000042c4*/ BL          0x00000000000042C4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000042c8*/ LDR         R2, [R5, #4] 
    /*000042cc*/ MOV         R3, R7 
    /*000042d0*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x2c8] 
    /*000042d4*/ MOV         R1, #548                    @ 0x224 
    /*000042d8*/ MOV         R0, #7 
    /*000042dc*/ STR         R1, [SP] 
    /*000042e0*/ MOV         R1, #17 
    /*000042e4*/ STR         R2, [SP, #4] 
    /*000042e8*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x17dc] 
    /*000042ec*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x17dc] 
    /*000042f0*/ MOV         R7, R3 
    /*000042f4*/ BL          0x00000000000042F4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000042f8*/ LDR         R2, [R5, #8] 
    /*000042fc*/ MOVW        R1, #549                    @ 0x225 
    /*00004300*/ MOV         R3, R7 
    /*00004304*/ STR         R1, [SP] 
    /*00004308*/ MOV         R0, #7 
    /*0000430c*/ STR         R2, [SP, #4] 
    /*00004310*/ MOV         R1, #17 
    /*00004314*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x180c] 
    /*00004318*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x180c] 
    /*0000431c*/ BL          0x000000000000431C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004320*/ LDR         R2, [R5, #16] 
    /*00004324*/ MOVW        R1, #551                    @ 0x227 
    /*00004328*/ MOV         R3, R7 
    /*0000432c*/ STR         R1, [SP] 
    /*00004330*/ MOV         R0, #7 
    /*00004334*/ STR         R2, [SP, #4] 
    /*00004338*/ MOV         R1, #17 
    /*0000433c*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1838] 
    /*00004340*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1838] 
    /*00004344*/ BL          0x0000000000004344          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004348*/ LDR         R2, [R5, #20] 
    /*0000434c*/ MOV         R1, #552                    @ 0x228 
    /*00004350*/ MOV         R3, R7 
    /*00004354*/ STR         R1, [SP] 
    /*00004358*/ MOV         R0, #7 
    /*0000435c*/ STR         R2, [SP, #4] 
    /*00004360*/ MOV         R1, #17 
    /*00004364*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1868] 
    /*00004368*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1868] 
    /*0000436c*/ BL          0x000000000000436C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004370*/ LDR         R2, [R5, #24] 
    /*00004374*/ MOVW        R1, #553                    @ 0x229 
    /*00004378*/ MOV         R3, R7 
    /*0000437c*/ STR         R1, [SP] 
    /*00004380*/ MOV         R0, #7 
    /*00004384*/ STR         R2, [SP, #4] 
    /*00004388*/ MOV         R1, #17 
    /*0000438c*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1898] 
    /*00004390*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1898] 
    /*00004394*/ BL          0x0000000000004394          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004398*/ LDR         R2, [R5, #28] 
    /*0000439c*/ MOVW        R1, #554                    @ 0x22a 
    /*000043a0*/ MOV         R3, R7 
    /*000043a4*/ STR         R1, [SP] 
    /*000043a8*/ MOV         R0, #7 
    /*000043ac*/ STR         R2, [SP, #4] 
    /*000043b0*/ MOV         R1, #17 
    /*000043b4*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18c8] 
    /*000043b8*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18c8] 
    /*000043bc*/ BL          0x00000000000043BC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000043c0*/ LDR         R2, [R5, #40]               @ 0x28 
    /*000043c4*/ MOVW        R1, #555                    @ 0x22b 
    /*000043c8*/ MOV         R3, R7 
    /*000043cc*/ STR         R1, [SP] 
    /*000043d0*/ MOV         R0, #7 
    /*000043d4*/ STR         R2, [SP, #4] 
    /*000043d8*/ MOV         R1, #17 
    /*000043dc*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x18f8] 
    /*000043e0*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x18f8] 
    /*000043e4*/ BL          0x00000000000043E4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000043e8*/ LDR         R1, [R5, #48]               @ 0x30 
    /*000043ec*/ LDR         R2, [R5, #44]               @ 0x2c 
    /*000043f0*/ MOV         R0, #556                    @ 0x22c 
    /*000043f4*/ MOV         R3, R7 
    /*000043f8*/ STR         R0, [SP] 
    /*000043fc*/ STR         R1, [SP, #8] 
    /*00004400*/ MOV         R0, #7 
    /*00004404*/ STR         R2, [SP, #4] 
    /*00004408*/ MOV         R1, #17 
    /*0000440c*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1930] 
    /*00004410*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1930] 
    /*00004414*/ BL          0x0000000000004414          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004418*/ MOVW        R2, #557                    @ 0x22d 
    /*0000441c*/ MOV         R3, R7 
    /*00004420*/ STR         R2, [SP] 
    /*00004424*/ MOV         R1, #17 
    /*00004428*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1968] 
    /*0000442c*/ MOV         R0, #7 
    /*00004430*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1968] 
    /*00004434*/ BL          0x0000000000004434          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004438*/ MOV         R4, #0 
    /*0000443c*/ MOV         R8, #560                    @ 0x230 
    DIS_DebugOutputGMEStatus_x4440:
    /*00004440*/ LDR         LR, [R5, #32] 
    /*00004444*/ MOV         R3, R7 
    /*00004448*/ LDR         IP, [R5, #36]               @ 0x24 
    /*0000444c*/ MOV         R2, R6 
    /*00004450*/ MOV         R1, #17 
    /*00004454*/ MOV         R0, #7 
    /*00004458*/ LDRB        LR, [LR, R4] 
    /*0000445c*/ STR         LR, [SP, #8] 
    /*00004460*/ LDRB        IP, [IP, R4] 
    /*00004464*/ ADD         R4, R4, #1 
    /*00004468*/ STM         SP, {R8, IP} 
    /*0000446c*/ BL          0x000000000000446C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004470*/ CMP         R4, #300                    @ 0x12c 
    /*00004474*/ BNE         DIS_DebugOutputGMEStatus_x4440
    /*00004478*/ ADD         SP, SP, #16 
    /*0000447c*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END DIS_DebugOutputGMEStatus


FUNC_BEGIN DIS_DebugOutputGmeMotion
    /*00004480*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00004484*/ SUB         SP, SP, #60                 @ 0x3c 
    /*00004488*/ LDR         R5, [PC, #140]              @ 0x000000000000451c 
    /*0000448c*/ SUB         R1, R0, #1 
    /*00004490*/ LDR         R4, [SP, #88]               @ 0x58 
    /*00004494*/ SUB         R0, R0, #2 
    /*00004498*/ STRD        R2, [SP, #8] 
    /*0000449c*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x19c8] 
    /*000044a0*/ STR         R1, [SP, #20] 
    /*000044a4*/ MOV         R3, R5 
    /*000044a8*/ STR         R0, [SP, #16] 
    /*000044ac*/ MOV         R1, #664                    @ 0x298 
    /*000044b0*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x19c8] 
    /*000044b4*/ STR         R1, [SP] 
    /*000044b8*/ MOV         R0, #7 
    /*000044bc*/ MOV         R1, #17 
    /*000044c0*/ BL          0x00000000000044C0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000044c4*/ LDRD        R0, [R4, #24] 
    /*000044c8*/ LDRD        R6, [R4, #40]               @ 0x28 
    /*000044cc*/ MOV         R3, R5 
    /*000044d0*/ LDRD        R8, [R4, #32] 
    /*000044d4*/ MOVW        IP, #671                    @ 0x29f 
    /*000044d8*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a08] 
    /*000044dc*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a08] 
    /*000044e0*/ STRD        R6, [SP, #48]               @ 0x30 
    /*000044e4*/ LDRD        R6, [R4, #16] 
    /*000044e8*/ STRD        R0, [SP, #32] 
    /*000044ec*/ MOV         R1, #17 
    /*000044f0*/ STRD        R8, [SP, #40]               @ 0x28 
    /*000044f4*/ MOV         R0, #7 
    /*000044f8*/ LDRD        R8, [R4, #8] 
    /*000044fc*/ LDRD        R4, [R4] 
    /*00004500*/ STRD        R6, [SP, #24] 
    /*00004504*/ STRD        R8, [SP, #16] 
    /*00004508*/ STRD        R4, [SP, #8] 
    /*0000450c*/ STR         IP, [SP] 
    /*00004510*/ BL          0x0000000000004510          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004514*/ ADD         SP, SP, #60                 @ 0x3c 
    /*00004518*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    /*0000451c*/ ANDEQ       R0, R0, R4, ROR #5 
FUNC_END DIS_DebugOutputGmeMotion


FUNC_BEGIN DIS_DrvGetNodeFromWaitList.part.1
    /*00004520*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00004524*/ SUB         SP, SP, #12 
    /*00004528*/ MOV         R2, #180                    @ 0xb4 
    /*0000452c*/ MOV         R1, #17 
    /*00004530*/ STR         R2, [SP] 
    /*00004534*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x300] 
    /*00004538*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a44] 
    /*0000453c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x300] 
    /*00004540*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a44] 
    /*00004544*/ MOV         R0, #3 
    /*00004548*/ BL          0x0000000000004548          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000454c*/ ADD         SP, SP, #12 
    /*00004550*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END DIS_DrvGetNodeFromWaitList.part.1


FUNC_BEGIN DIS_DrvStartTimer
    /*00004554*/ PUSH        {R4, LR} 
    /*00004558*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*0000455c*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*00004560*/ SUB         SP, SP, #8 
    /*00004564*/ ADD         R1, SP, #4 
    /*00004568*/ MOV         R0, R4 
    /*0000456c*/ BL          0x000000000000456C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00004570*/ LDR         R3, [R4, #4] 
    /*00004574*/ CMP         R3, #0 
    /*00004578*/ BNE         DIS_DrvStartTimer_x4590
    /*0000457c*/ MOV         R1, #10 
    /*00004580*/ ADD         R0, R4, #8 
    /*00004584*/ BL          0x0000000000004584          @ osal_set_timer [R_ARM_CALL: +0x0] 
    /*00004588*/ MOV         R3, #1 
    /*0000458c*/ STR         R3, [R4, #4] 
    DIS_DrvStartTimer_x4590:
    /*00004590*/ ADD         R1, SP, #4 
    /*00004594*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*00004598*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*0000459c*/ BL          0x000000000000459C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000045a0*/ ADD         SP, SP, #8 
    /*000045a4*/ POP         {R4, PC} 
FUNC_END DIS_DrvStartTimer


FUNC_BEGIN DIS_DrvGetNodeFromFreeList
    /*000045a8*/ MOV         R3, R0 
    /*000045ac*/ PUSH        {R4, R5, LR} 
    /*000045b0*/ SUB         SP, SP, #12 
    /*000045b4*/ LDR         R4, [R3, #56]!              @ 0x38 
    /*000045b8*/ CMP         R4, R3 
    /*000045bc*/ BEQ         DIS_DrvGetNodeFromFreeList_x4628
    /*000045c0*/ CMP         R4, #0 
    /*000045c4*/ MOV         R5, R0 
    /*000045c8*/ BEQ         DIS_DrvGetNodeFromFreeList_x4608
    DIS_DrvGetNodeFromFreeList_x45cc:
    /*000045cc*/ LDR         R1, [R4, #4] 
    /*000045d0*/ MOV         R2, #256                    @ 0x100 
    /*000045d4*/ LDR         IP, [R4] 
    /*000045d8*/ MOVT        R2, #16 
    /*000045dc*/ MOV         R3, #512                    @ 0x200 
    /*000045e0*/ SUB         R0, R4, #56                 @ 0x38 
    /*000045e4*/ MOVT        R3, #32 
    /*000045e8*/ STR         R1, [IP, #4] 
    /*000045ec*/ STR         IP, [R1] 
    /*000045f0*/ STM         R4, {R2, R3} 
    /*000045f4*/ LDR         R3, [R5, #96]               @ 0x60 
    /*000045f8*/ SUB         R3, R3, #1 
    /*000045fc*/ STR         R3, [R5, #96]               @ 0x60 
    DIS_DrvGetNodeFromFreeList_x4600:
    /*00004600*/ ADD         SP, SP, #12 
    /*00004604*/ POP         {R4, R5, PC} 
    DIS_DrvGetNodeFromFreeList_x4608:
    /*00004608*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1aa0] 
    /*0000460c*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1aa4] 
    /*00004610*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1aa0] 
    /*00004614*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1aa4] 
    /*00004618*/ MOV         R2, #138                    @ 0x8a 
    /*0000461c*/ LDR         R1, [PC, #44]               @ 0x0000000000004650 
    /*00004620*/ BL          0x0000000000004620          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004624*/ B           DIS_DrvGetNodeFromFreeList_x45cc
    DIS_DrvGetNodeFromFreeList_x4628:
    /*00004628*/ MOV         R3, #131                    @ 0x83 
    /*0000462c*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1a70] 
    /*00004630*/ STR         R3, [SP] 
    /*00004634*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1a70] 
    /*00004638*/ LDR         R3, [PC, #16]               @ 0x0000000000004650 
    /*0000463c*/ MOV         R1, #17 
    /*00004640*/ MOV         R0, #4 
    /*00004644*/ BL          0x0000000000004644          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004648*/ MOV         R0, #0 
    /*0000464c*/ B           DIS_DrvGetNodeFromFreeList_x4600
    /*00004650*/ ANDEQ       R0, R0, IP, LSL R3 
FUNC_END DIS_DrvGetNodeFromFreeList


FUNC_BEGIN DIS_DrvPutNodeToFreeList
    /*00004654*/ LDR         R3, [R0, #60]               @ 0x3c 
    /*00004658*/ ADD         R2, R1, #56                 @ 0x38 
    /*0000465c*/ ADD         IP, R0, #56                 @ 0x38 
    /*00004660*/ STR         R2, [R0, #60]               @ 0x3c 
    /*00004664*/ STR         IP, [R1, #56]               @ 0x38 
    /*00004668*/ STR         R3, [R1, #60]               @ 0x3c 
    /*0000466c*/ STR         R2, [R3] 
    /*00004670*/ LDR         R3, [R0, #96]               @ 0x60 
    /*00004674*/ ADD         R3, R3, #1 
    /*00004678*/ STR         R3, [R0, #96]               @ 0x60 
    /*0000467c*/ BX          LR 
FUNC_END DIS_DrvPutNodeToFreeList


FUNC_BEGIN DIS_DrvQueryFreeNodeNum
    /*00004680*/ LDR         R0, [R0, #96]               @ 0x60 
    /*00004684*/ BX          LR 
FUNC_END DIS_DrvQueryFreeNodeNum


FUNC_BEGIN DIS_DrvQueryWaitNodeNum
    /*00004688*/ LDR         R0, [R0, #100]              @ 0x64 
    /*0000468c*/ BX          LR 
FUNC_END DIS_DrvQueryWaitNodeNum


FUNC_BEGIN DIS_DrvPutNodeToWaitList
    /*00004690*/ LDR         R3, [R0, #68]               @ 0x44 
    /*00004694*/ ADD         R2, R1, #56                 @ 0x38 
    /*00004698*/ ADD         IP, R0, #64                 @ 0x40 
    /*0000469c*/ STR         R2, [R0, #68]               @ 0x44 
    /*000046a0*/ STR         IP, [R1, #56]               @ 0x38 
    /*000046a4*/ STR         R3, [R1, #60]               @ 0x3c 
    /*000046a8*/ STR         R2, [R3] 
    /*000046ac*/ LDR         R3, [R0, #100]              @ 0x64 
    /*000046b0*/ ADD         R3, R3, #1 
    /*000046b4*/ STR         R3, [R0, #100]              @ 0x64 
    /*000046b8*/ BX          LR 
FUNC_END DIS_DrvPutNodeToWaitList


FUNC_BEGIN DIS_DrvGetNodeFromWaitList
    /*000046bc*/ LDR         R2, [R0, #64]               @ 0x40 
    /*000046c0*/ ADD         R3, R0, #64                 @ 0x40 
    /*000046c4*/ CMP         R2, R3 
    /*000046c8*/ BEQ         DIS_DrvGetNodeFromWaitList_x4708
    /*000046cc*/ LDR         IP, [R1, #60]               @ 0x3c 
    /*000046d0*/ MOV         R2, #256                    @ 0x100 
    /*000046d4*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000046d8*/ MOVT        R2, #16 
    /*000046dc*/ LDR         LR, [R1, #56]               @ 0x38 
    /*000046e0*/ MOV         R3, #512                    @ 0x200 
    /*000046e4*/ MOVT        R3, #32 
    /*000046e8*/ STR         IP, [LR, #4] 
    /*000046ec*/ STR         LR, [IP] 
    /*000046f0*/ STR         R2, [R1, #56]               @ 0x38 
    /*000046f4*/ STR         R3, [R1, #60]               @ 0x3c 
    /*000046f8*/ LDR         R3, [R0, #100]              @ 0x64 
    /*000046fc*/ SUB         R3, R3, #1 
    /*00004700*/ STR         R3, [R0, #100]              @ 0x64 
    /*00004704*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    DIS_DrvGetNodeFromWaitList_x4708:
    /*00004708*/ B           0x0000000000004520 
FUNC_END DIS_DrvGetNodeFromWaitList


FUNC_BEGIN DIS_DrvGetNodeFromBusyList
    /*0000470c*/ PUSH        {R4, R5, LR} 
    /*00004710*/ LSL         R4, R2, #1 
    /*00004714*/ ADD         LR, R4, R3 
    /*00004718*/ ADD         IP, R3, #9 
    /*0000471c*/ ADD         R5, LR, #9 
    /*00004720*/ ADD         IP, IP, R4 
    /*00004724*/ SUB         SP, SP, #20 
    /*00004728*/ LDR         R4, [R0, R5, LSL #3] 
    /*0000472c*/ ADD         IP, R0, IP, LSL #3 
    /*00004730*/ CMP         R4, IP 
    /*00004734*/ BEQ         DIS_DrvGetNodeFromBusyList_x4784
    /*00004738*/ LDR         IP, [R1, #60]               @ 0x3c 
    /*0000473c*/ ADD         LR, R0, LR, LSL #2 
    /*00004740*/ LDR         R4, [R1, #56]               @ 0x38 
    /*00004744*/ MOV         R2, #256                    @ 0x100 
    /*00004748*/ MOV         R3, #512                    @ 0x200 
    /*0000474c*/ MOVT        R2, #16 
    /*00004750*/ MOVT        R3, #32 
    /*00004754*/ STR         IP, [R4, #4] 
    /*00004758*/ STR         R4, [IP] 
    /*0000475c*/ STR         R2, [R1, #56]               @ 0x38 
    /*00004760*/ STR         R3, [R1, #60]               @ 0x3c 
    /*00004764*/ LDR         R3, [LR, #88]               @ 0x58 
    /*00004768*/ SUB         R3, R3, #1 
    /*0000476c*/ STR         R3, [LR, #88]               @ 0x58 
    /*00004770*/ LDR         R3, [R0, #104]              @ 0x68 
    /*00004774*/ SUB         R3, R3, #1 
    /*00004778*/ STR         R3, [R0, #104]              @ 0x68 
    DIS_DrvGetNodeFromBusyList_x477c:
    /*0000477c*/ ADD         SP, SP, #20 
    /*00004780*/ POP         {R4, R5, PC} 
    DIS_DrvGetNodeFromBusyList_x4784:
    /*00004784*/ STR         R3, [SP, #8] 
    /*00004788*/ MOV         R1, #17 
    /*0000478c*/ MOV         R3, #196                    @ 0xc4 
    /*00004790*/ STR         R2, [SP, #4] 
    /*00004794*/ STR         R3, [SP] 
    /*00004798*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ae4] 
    /*0000479c*/ LDR         R3, [PC, #12]               @ 0x00000000000047b0 
    /*000047a0*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ae4] 
    /*000047a4*/ MOV         R0, #3 
    /*000047a8*/ BL          0x00000000000047A8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000047ac*/ B           DIS_DrvGetNodeFromBusyList_x477c
    /*000047b0*/ ANDEQ       R0, R0, R8, LSR R3 
FUNC_END DIS_DrvGetNodeFromBusyList


FUNC_BEGIN DIS_DrvPutNodeToBusyList
    /*000047b4*/ LSL         R2, R2, #1 
    /*000047b8*/ ADD         IP, R3, R2 
    /*000047bc*/ ADD         R3, R3, #9 
    /*000047c0*/ PUSH        {R4, LR} 
    /*000047c4*/ ADD         R4, R0, IP, LSL #3 
    /*000047c8*/ ADD         R3, R3, R2 
    /*000047cc*/ ADD         IP, R0, IP, LSL #2 
    /*000047d0*/ LDR         R2, [R4, #76]               @ 0x4c 
    /*000047d4*/ ADD         LR, R1, #56                 @ 0x38 
    /*000047d8*/ ADD         R3, R0, R3, LSL #3 
    /*000047dc*/ STR         LR, [R4, #76]               @ 0x4c 
    /*000047e0*/ STR         R3, [R1, #56]               @ 0x38 
    /*000047e4*/ STR         R2, [R1, #60]               @ 0x3c 
    /*000047e8*/ STR         LR, [R2] 
    /*000047ec*/ LDR         R3, [IP, #88]               @ 0x58 
    /*000047f0*/ ADD         R3, R3, #1 
    /*000047f4*/ STR         R3, [IP, #88]               @ 0x58 
    /*000047f8*/ LDR         R3, [R0, #104]              @ 0x68 
    /*000047fc*/ ADD         R3, R3, #1 
    /*00004800*/ STR         R3, [R0, #104]              @ 0x68 
    /*00004804*/ POP         {R4, PC} 
FUNC_END DIS_DrvPutNodeToBusyList


FUNC_BEGIN DIS_DrvQueryBusyNodeNum
    /*00004808*/ ADD         R2, R2, R1, LSL #1 
    /*0000480c*/ ADD         R2, R2, #22 
    /*00004810*/ LDR         R0, [R0, R2, LSL #2] 
    /*00004814*/ BX          LR 
FUNC_END DIS_DrvQueryBusyNodeNum


FUNC_BEGIN DIS_DrvQueryBusyTotalNodeNum
    /*00004818*/ LDR         R0, [R0, #104]              @ 0x68 
    /*0000481c*/ BX          LR 
FUNC_END DIS_DrvQueryBusyTotalNodeNum


FUNC_BEGIN DIS_DrvCommSubmit2Vgs
    /*00004820*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00004824*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*00004828*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*0000482c*/ SUB         SP, SP, #12 
    /*00004830*/ LDR         IP, [R4, #20] 
    /*00004834*/ LDR         LR, [IP, #100]              @ 0x64 
    /*00004838*/ CMP         LR, #0 
    /*0000483c*/ MOVEQ       R0, LR 
    /*00004840*/ BNE         DIS_DrvCommSubmit2Vgs_x484c
    DIS_DrvCommSubmit2Vgs_x4844:
    /*00004844*/ ADD         SP, SP, #12 
    /*00004848*/ POP         {R4, R5, R6, R7, PC} 
    DIS_DrvCommSubmit2Vgs_x484c:
    /*0000484c*/ LDR         R6, [IP, #64]               @ 0x40 
    /*00004850*/ ADD         R3, R0, #6 
    /*00004854*/ MOV         R2, R1 
    /*00004858*/ MOV         R7, R0 
    /*0000485c*/ SUB         R1, R6, #56                 @ 0x38 
    /*00004860*/ LDR         R5, [IP, R3, LSL #2] 
    /*00004864*/ BL          0x0000000000004864          @ DIS_DrvProcessVgs [R_ARM_CALL: .text+0x5bb0] 
    /*00004868*/ CMP         R0, #0 
    /*0000486c*/ BNE         DIS_DrvCommSubmit2Vgs_x4918
    /*00004870*/ LDR         R3, [R4, #20] 
    /*00004874*/ ADD         R2, R3, #64                 @ 0x40 
    /*00004878*/ LDR         R1, [R3, #64]               @ 0x40 
    /*0000487c*/ CMP         R1, R2 
    /*00004880*/ BEQ         DIS_DrvCommSubmit2Vgs_x4908
    /*00004884*/ LDR         IP, [R6, #4] 
    /*00004888*/ MOV         R1, #256                    @ 0x100 
    /*0000488c*/ LDR         LR, [R6] 
    /*00004890*/ MOVT        R1, #16 
    /*00004894*/ MOV         R2, #512                    @ 0x200 
    /*00004898*/ MOVT        R2, #32 
    /*0000489c*/ STR         IP, [LR, #4] 
    /*000048a0*/ STR         LR, [IP] 
    /*000048a4*/ STM         R6, {R1, R2} 
    /*000048a8*/ LDR         R2, [R3, #100]              @ 0x64 
    /*000048ac*/ SUB         R2, R2, #1 
    /*000048b0*/ STR         R2, [R3, #100]              @ 0x64 
    DIS_DrvCommSubmit2Vgs_x48b4:
    /*000048b4*/ LDR         R2, [R4, #20] 
    /*000048b8*/ ADD         R3, R5, R7, LSL #1 
    /*000048bc*/ ADD         R1, R3, #9 
    /*000048c0*/ ADD         LR, R2, R3, LSL #3 
    /*000048c4*/ ADD         R3, R2, R3, LSL #2 
    /*000048c8*/ ADD         R1, R2, R1, LSL #3 
    /*000048cc*/ LDR         IP, [LR, #76]               @ 0x4c 
    /*000048d0*/ STR         R6, [LR, #76]               @ 0x4c 
    /*000048d4*/ MOV         LR, #1 
    /*000048d8*/ STM         R6, {R1, IP} 
    /*000048dc*/ STR         R6, [IP] 
    /*000048e0*/ LDR         R1, [R3, #88]               @ 0x58 
    /*000048e4*/ LDR         IP, [R4, #20] 
    /*000048e8*/ ADD         R1, R1, LR 
    /*000048ec*/ STR         R1, [R3, #88]               @ 0x58 
    /*000048f0*/ LDR         R3, [R2, #104]              @ 0x68 
    /*000048f4*/ ADD         R3, R3, LR 
    /*000048f8*/ STR         R3, [R2, #104]              @ 0x68 
    /*000048fc*/ STR         LR, [IP, #36]               @ 0x24 
    /*00004900*/ ADD         SP, SP, #12 
    /*00004904*/ POP         {R4, R5, R6, R7, PC} 
    DIS_DrvCommSubmit2Vgs_x4908:
    /*00004908*/ STR         R0, [SP, #4] 
    /*0000490c*/ BL          0x0000000000004520 
    /*00004910*/ LDR         R0, [SP, #4] 
    /*00004914*/ B           DIS_DrvCommSubmit2Vgs_x48b4
    DIS_DrvCommSubmit2Vgs_x4918:
    /*00004918*/ BL          0x0000000000004918          @ DIS_DrvStartTimer [R_ARM_CALL: .text+0x4554] 
    /*0000491c*/ MOVW        R0, #32777                  @ 0x8009 
    /*00004920*/ MOVT        R0, #40977                  @ 0xa011 
    /*00004924*/ B           DIS_DrvCommSubmit2Vgs_x4844
FUNC_END DIS_DrvCommSubmit2Vgs


FUNC_BEGIN DisTimerISR
    /*00004928*/ PUSH        {R4, R5, LR} 
    /*0000492c*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*00004930*/ SUB         SP, SP, #36                 @ 0x24 
    /*00004934*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*00004938*/ ADD         R0, SP, #16 
    /*0000493c*/ MVN         R3, #0 
    /*00004940*/ STR         R3, [SP, #12] 
    /*00004944*/ BL          0x0000000000004944          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00004948*/ MOV         R0, R4 
    /*0000494c*/ ADD         R1, SP, #8 
    /*00004950*/ BL          0x0000000000004950          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00004954*/ MOV         R0, R4 
    /*00004958*/ ADD         R1, SP, #8 
    /*0000495c*/ MOV         R5, #0 
    /*00004960*/ STR         R5, [R4, #4] 
    /*00004964*/ BL          0x0000000000004964          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00004968*/ ADD         R1, SP, #8 
    /*0000496c*/ ADD         R0, R4, #24 
    /*00004970*/ BL          0x0000000000004970          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00004974*/ ADD         R1, SP, #12 
    /*00004978*/ MOV         R0, R5 
    /*0000497c*/ BL          0x000000000000497C          @ DIS_DrvCommSubmit2Vgs [R_ARM_CALL: .text+0x4820] 
    /*00004980*/ ADD         R1, SP, #8 
    /*00004984*/ ADD         R0, R4, #24 
    /*00004988*/ BL          0x0000000000004988          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000498c*/ LDR         R3, [SP, #12] 
    /*00004990*/ CMN         R3, #1 
    /*00004994*/ BEQ         DisTimerISR_x49c0
    /*00004998*/ MOV         R0, #45                     @ 0x2d 
    /*0000499c*/ BL          0x000000000000499C          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000049a0*/ CMP         R0, #0 
    /*000049a4*/ BEQ         DisTimerISR_x49d0
    /*000049a8*/ LDR         R3, [R0, #12] 
    /*000049ac*/ CMP         R3, R5 
    /*000049b0*/ BEQ         DisTimerISR_x49d0
    /*000049b4*/ LDR         R3, [R0, #4] 
    /*000049b8*/ LDR         R0, [SP, #12] 
    /*000049bc*/ BLX         R3 
    DisTimerISR_x49c0:
    /*000049c0*/ ADD         R0, SP, #24 
    /*000049c4*/ BL          0x00000000000049C4          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000049c8*/ ADD         SP, SP, #36                 @ 0x24 
    /*000049cc*/ POP         {R4, R5, PC} 
    DisTimerISR_x49d0:
    /*000049d0*/ MOV         R1, #17 
    /*000049d4*/ MOVW        R3, #285                    @ 0x11d 
    /*000049d8*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b28] 
    /*000049dc*/ STR         R3, [SP] 
    /*000049e0*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b28] 
    /*000049e4*/ LDR         R3, [PC, #20]               @ 0x0000000000004a00 
    /*000049e8*/ MOV         R0, #3 
    /*000049ec*/ BL          0x00000000000049EC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000049f0*/ ADD         R0, SP, #24 
    /*000049f4*/ BL          0x00000000000049F4          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000049f8*/ ADD         SP, SP, #36                 @ 0x24 
    /*000049fc*/ POP         {R4, R5, PC} 
    /*00004a00*/ ANDEQ       R0, R0, R4, ASR R3 
FUNC_END DisTimerISR


FUNC_BEGIN DIS_DRV_SubmmitToGme
    /*00004a04*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00004a08*/ SUB         SP, SP, #20 
    /*00004a0c*/ MOV         R8, R0 
    /*00004a10*/ MOV         R0, R1 
    /*00004a14*/ MOV         R5, R1 
    /*00004a18*/ MOV         R6, R2 
    /*00004a1c*/ MOV         R7, R3 
    /*00004a20*/ MVN         R3, #0 
    /*00004a24*/ STR         R3, [SP, #12] 
    /*00004a28*/ BL          0x0000000000004A28          @ DIS_CheckNullPtr [R_ARM_CALL: .text+0x7a0] 
    /*00004a2c*/ SUBS        R4, R0, #0 
    /*00004a30*/ BEQ         DIS_DRV_SubmmitToGme_x4a40
    DIS_DRV_SubmmitToGme_x4a34:
    /*00004a34*/ MOV         R0, R4 
    /*00004a38*/ ADD         SP, SP, #20 
    /*00004a3c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_DRV_SubmmitToGme_x4a40:
    /*00004a40*/ MOV         R0, R6 
    /*00004a44*/ BL          0x0000000000004A44          @ DIS_CheckNullPtr [R_ARM_CALL: .text+0x7a0] 
    /*00004a48*/ SUBS        R4, R0, #0 
    /*00004a4c*/ BNE         DIS_DRV_SubmmitToGme_x4a34
    /*00004a50*/ MOV         R0, R7 
    /*00004a54*/ BL          0x0000000000004A54          @ DIS_CheckNullPtr [R_ARM_CALL: .text+0x7a0] 
    /*00004a58*/ SUBS        R4, R0, #0 
    /*00004a5c*/ BNE         DIS_DRV_SubmmitToGme_x4a34
    /*00004a60*/ MOVW        R9, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*00004a64*/ MOVT        R9, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*00004a68*/ ADD         R0, R9, #24 
    /*00004a6c*/ ADD         R1, SP, #8 
    /*00004a70*/ BL          0x0000000000004A70          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00004a74*/ LDR         R0, [R9, #20] 
    /*00004a78*/ LDR         R3, [R0, #44]               @ 0x2c 
    /*00004a7c*/ CMP         R3, #1 
    /*00004a80*/ BEQ         DIS_DRV_SubmmitToGme_x4b8c
    /*00004a84*/ MOV         FP, #1 
    /*00004a88*/ STR         FP, [R0, #40]               @ 0x28 
    /*00004a8c*/ BL          0x0000000000004A8C          @ DIS_DrvGetNodeFromFreeList [R_ARM_CALL: .text+0x45a8] 
    /*00004a90*/ SUBS        SL, R0, #0 
    /*00004a94*/ BEQ         DIS_DRV_SubmmitToGme_x4bc4
    /*00004a98*/ MOV         R3, R6 
    /*00004a9c*/ MOV         R2, R5 
    /*00004aa0*/ MOV         R1, SL 
    /*00004aa4*/ STR         R7, [SP] 
    /*00004aa8*/ MOV         R0, R8 
    /*00004aac*/ BL          0x0000000000004AAC          @ DIS_DrvConvertHwNodeInfo [R_ARM_CALL: .text+0x5fd0] 
    /*00004ab0*/ LDR         R3, [R9, #20] 
    /*00004ab4*/ STR         FP, [SL, #28] 
    /*00004ab8*/ ADD         R1, SL, #56                 @ 0x38 
    /*00004abc*/ STR         R8, [SL, #16] 
    /*00004ac0*/ ADD         R0, R3, #64                 @ 0x40 
    /*00004ac4*/ LDR         R2, [R3, #68]               @ 0x44 
    /*00004ac8*/ STR         R1, [R3, #68]               @ 0x44 
    /*00004acc*/ STR         R0, [SL, #56]               @ 0x38 
    /*00004ad0*/ STR         R2, [SL, #60]               @ 0x3c 
    /*00004ad4*/ STR         R1, [R2] 
    /*00004ad8*/ LDR         R2, [R9, #20] 
    /*00004adc*/ LDR         R1, [R3, #100]              @ 0x64 
    /*00004ae0*/ ADD         R1, R1, #1 
    /*00004ae4*/ STR         R1, [R3, #100]              @ 0x64 
    /*00004ae8*/ LDR         R3, [R2, #24] 
    /*00004aec*/ ADD         R3, R3, #22 
    /*00004af0*/ LDR         R3, [R2, R3, LSL #2] 
    /*00004af4*/ CMP         R3, #0 
    /*00004af8*/ BNE         DIS_DRV_SubmmitToGme_x4b1c
    /*00004afc*/ LDR         R3, [R2, #44]               @ 0x2c 
    /*00004b00*/ CMP         R3, #0 
    /*00004b04*/ BNE         DIS_DRV_SubmmitToGme_x4b1c
    /*00004b08*/ MOV         R0, R4 
    /*00004b0c*/ ADD         R1, SP, #12 
    /*00004b10*/ BL          0x0000000000004B10          @ DIS_DrvCommSubmit2Vgs [R_ARM_CALL: .text+0x4820] 
    /*00004b14*/ LDR         R2, [R9, #20] 
    /*00004b18*/ MOV         R4, R0 
    DIS_DRV_SubmmitToGme_x4b1c:
    /*00004b1c*/ MOV         R3, #0 
    /*00004b20*/ ADD         R1, SP, #8 
    /*00004b24*/ STR         R3, [R2, #40]               @ 0x28 
    /*00004b28*/ LDR         R0, [PC, #192]              @ 0x0000000000004bf0 
    /*00004b2c*/ BL          0x0000000000004B2C          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00004b30*/ LDR         R3, [SP, #12] 
    /*00004b34*/ CMN         R3, #1 
    /*00004b38*/ BEQ         DIS_DRV_SubmmitToGme_x4a34
    /*00004b3c*/ MOV         R0, #45                     @ 0x2d 
    /*00004b40*/ BL          0x0000000000004B40          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00004b44*/ CMP         R0, #0 
    /*00004b48*/ BEQ         DIS_DRV_SubmmitToGme_x4b68
    /*00004b4c*/ LDR         R3, [R0, #12] 
    /*00004b50*/ CMP         R3, #0 
    /*00004b54*/ BEQ         DIS_DRV_SubmmitToGme_x4b68
    /*00004b58*/ LDR         R3, [R0, #4] 
    /*00004b5c*/ LDR         R0, [SP, #12] 
    /*00004b60*/ BLX         R3 
    /*00004b64*/ B           DIS_DRV_SubmmitToGme_x4a34
    DIS_DRV_SubmmitToGme_x4b68:
    /*00004b68*/ MOV         R3, #364                    @ 0x16c 
    /*00004b6c*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b28] 
    /*00004b70*/ STR         R3, [SP] 
    /*00004b74*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b28] 
    /*00004b78*/ LDR         R3, [PC, #116]              @ 0x0000000000004bf4 
    /*00004b7c*/ MOV         R1, #17 
    /*00004b80*/ MOV         R0, #3 
    /*00004b84*/ BL          0x0000000000004B84          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004b88*/ B           DIS_DRV_SubmmitToGme_x4a34
    DIS_DRV_SubmmitToGme_x4b8c:
    /*00004b8c*/ MOV         R3, #316                    @ 0x13c 
    /*00004b90*/ STR         R3, [SP] 
    /*00004b94*/ LDR         R3, [PC, #88]               @ 0x0000000000004bf4 
    /*00004b98*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b90] 
    /*00004b9c*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b90] 
    /*00004ba0*/ MOV         R1, #17 
    /*00004ba4*/ MOV         R0, #3 
    /*00004ba8*/ MOVW        R4, #32773                  @ 0x8005 
    DIS_DRV_SubmmitToGme_x4bac:
    /*00004bac*/ BL          0x0000000000004BAC          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00004bb0*/ ADD         R1, SP, #8 
    /*00004bb4*/ ADD         R0, R9, #24 
    /*00004bb8*/ MOVT        R4, #40977                  @ 0xa011 
    /*00004bbc*/ BL          0x0000000000004BBC          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00004bc0*/ B           DIS_DRV_SubmmitToGme_x4a34
    DIS_DRV_SubmmitToGme_x4bc4:
    /*00004bc4*/ LDR         R0, [R9, #20] 
    /*00004bc8*/ MOVW        IP, #327                    @ 0x147 
    /*00004bcc*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1be4] 
    /*00004bd0*/ LDR         R3, [PC, #28]               @ 0x0000000000004bf4 
    /*00004bd4*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1be4] 
    /*00004bd8*/ MOV         R1, #17 
    /*00004bdc*/ STR         R4, [R0, #40]               @ 0x28 
    /*00004be0*/ MOV         R0, #3 
    /*00004be4*/ STR         IP, [SP] 
    /*00004be8*/ MOVW        R4, #32781                  @ 0x800d 
    /*00004bec*/ B           DIS_DRV_SubmmitToGme_x4bac
    /*00004bf0*/ LDRDEQ      R0, [R0], -IP 
    /*00004bf4*/ ANDEQ       R0, R0, R0, ROR #6 
FUNC_END DIS_DRV_SubmmitToGme


FUNC_BEGIN DIS_DrvCommPrepareHWInfo
    DIS_DrvCommPrepareHWInfo_x4bf8:
    /*00004bf8*/ B           DIS_DrvCommPrepareHWInfo_x4bf8@ DIS_DrvConvertHwNodeInfo [R_ARM_JUMP24: .text+0x5fd0] 
FUNC_END DIS_DrvCommPrepareHWInfo


FUNC_BEGIN DIS_DrvCommStartWaitlist
    /*00004bfc*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00004c00*/ MOVW        R7, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*00004c04*/ MOVT        R7, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*00004c08*/ SUB         SP, SP, #28 
    /*00004c0c*/ LDR         R2, [R7, #20] 
    /*00004c10*/ LDR         R3, [R2, #100]              @ 0x64 
    /*00004c14*/ CMP         R3, #0 
    /*00004c18*/ BEQ         DIS_DrvCommStartWaitlist_x4dcc
    /*00004c1c*/ MOV         IP, R2 
    /*00004c20*/ LDR         R3, [IP, #64]!              @ 0x40 
    /*00004c24*/ CMP         IP, R3 
    /*00004c28*/ SUB         R4, R3, #56                 @ 0x38 
    /*00004c2c*/ LDR         R5, [R3] 
    /*00004c30*/ SUB         R5, R5, #56                 @ 0x38 
    /*00004c34*/ BEQ         DIS_DrvCommStartWaitlist_x4dcc
    /*00004c38*/ ADD         R1, R0, #6 
    /*00004c3c*/ MOV         R6, #0 
    /*00004c40*/ MOV         FP, R6 
    /*00004c44*/ MOV         R8, R6 
    /*00004c48*/ LDR         R9, [R2, R1, LSL #2] 
    /*00004c4c*/ MOVW        LR, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c1c] 
    /*00004c50*/ MOVW        R1, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1aa4] 
    /*00004c54*/ MOVT        LR, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c1c] 
    /*00004c58*/ ADD         R9, R9, R0, LSL #1 
    /*00004c5c*/ MOVT        R1, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1aa4] 
    /*00004c60*/ ADD         SL, R9, #9 
    /*00004c64*/ STR         LR, [SP, #12] 
    /*00004c68*/ STR         R1, [SP, #16] 
    /*00004c6c*/ LSL         SL, SL, #3 
    /*00004c70*/ STR         R6, [SP, #8] 
    DIS_DrvCommStartWaitlist_x4c74:
    /*00004c74*/ CMP         FP, #9 
    /*00004c78*/ BHI         DIS_DrvCommStartWaitlist_x4d48
    /*00004c7c*/ LDR         R1, [R2, #64]               @ 0x40 
    /*00004c80*/ CMP         R1, IP 
    /*00004c84*/ BEQ         DIS_DrvCommStartWaitlist_x4dd8
    /*00004c88*/ LDR         IP, [R4, #60]               @ 0x3c 
    /*00004c8c*/ MOV         R0, #256                    @ 0x100 
    /*00004c90*/ LDR         LR, [R4, #56]               @ 0x38 
    /*00004c94*/ MOVT        R0, #16 
    /*00004c98*/ MOV         R1, #512                    @ 0x200 
    /*00004c9c*/ MOVT        R1, #32 
    /*00004ca0*/ STR         IP, [LR, #4] 
    /*00004ca4*/ STR         LR, [IP] 
    /*00004ca8*/ STR         R0, [R4, #56]               @ 0x38 
    /*00004cac*/ STR         R1, [R4, #60]               @ 0x3c 
    /*00004cb0*/ LDR         R1, [R2, #100]              @ 0x64 
    /*00004cb4*/ SUB         R1, R1, #1 
    /*00004cb8*/ STR         R1, [R2, #100]              @ 0x64 
    DIS_DrvCommStartWaitlist_x4cbc:
    /*00004cbc*/ LDR         R2, [R7, #20] 
    /*00004cc0*/ MOV         R1, R6 
    /*00004cc4*/ ADD         LR, R2, R9, LSL #3 
    /*00004cc8*/ ADD         R0, R2, R9, LSL #2 
    /*00004ccc*/ LDR         IP, [LR, #76]               @ 0x4c 
    /*00004cd0*/ STR         R3, [LR, #76]               @ 0x4c 
    /*00004cd4*/ ADD         LR, R2, SL 
    /*00004cd8*/ STR         LR, [R4, #56]               @ 0x38 
    /*00004cdc*/ STR         IP, [R4, #60]               @ 0x3c 
    /*00004ce0*/ STR         R3, [IP] 
    /*00004ce4*/ LDR         R3, [R0, #88]               @ 0x58 
    /*00004ce8*/ ADD         R3, R3, #1 
    /*00004cec*/ STR         R3, [R0, #88]               @ 0x58 
    /*00004cf0*/ LDR         R3, [R2, #104]              @ 0x68 
    /*00004cf4*/ ADD         R3, R3, #1 
    /*00004cf8*/ STR         R3, [R2, #104]              @ 0x68 
    /*00004cfc*/ STR         R6, [R4, #20] 
    /*00004d00*/ LDR         R0, [R4, #8] 
    /*00004d04*/ BL          0x0000000000004D04          @ DIS_HAL_SetNodeID [R_ARM_CALL: .text+0x6258] 
    /*00004d08*/ MOV         R1, #0 
    /*00004d0c*/ LDR         R0, [R4, #8] 
    /*00004d10*/ BL          0x0000000000004D10          @ DIS_HAL_SetMask [R_ARM_CALL: .text+0x625c] 
    /*00004d14*/ CMP         R6, #0 
    /*00004d18*/ STREQ       R4, [SP, #8] 
    /*00004d1c*/ BEQ         DIS_DrvCommStartWaitlist_x4d34
    /*00004d20*/ CMP         R8, #0 
    /*00004d24*/ BEQ         DIS_DrvCommStartWaitlist_x4de8
    DIS_DrvCommStartWaitlist_x4d28:
    /*00004d28*/ LDR         R0, [R8, #8] 
    /*00004d2c*/ LDRD        R2, [R4] 
    /*00004d30*/ BL          0x0000000000004D30          @ DIS_HAL_SetNext [R_ARM_CALL: .text+0x6260] 
    DIS_DrvCommStartWaitlist_x4d34:
    /*00004d34*/ ADD         R0, R4, #40                 @ 0x28 
    /*00004d38*/ LDR         FP, [R4, #32] 
    /*00004d3c*/ BL          0x0000000000004D3C          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00004d40*/ ADD         R6, R6, #1 
    /*00004d44*/ MOV         R8, R4 
    DIS_DrvCommStartWaitlist_x4d48:
    /*00004d48*/ MOV         R3, R5 
    /*00004d4c*/ LDR         R2, [R7, #20] 
    /*00004d50*/ LDR         R0, [R3, #56]!              @ 0x38 
    /*00004d54*/ MOV         R4, R5 
    /*00004d58*/ ADD         IP, R2, #64                 @ 0x40 
    /*00004d5c*/ CMP         R3, IP 
    /*00004d60*/ SUB         R5, R0, #56                 @ 0x38 
    /*00004d64*/ BNE         DIS_DrvCommStartWaitlist_x4c74
    /*00004d68*/ LDR         R3, [SP, #8] 
    /*00004d6c*/ CMP         R3, #0 
    /*00004d70*/ BEQ         DIS_DrvCommStartWaitlist_x4dcc
    /*00004d74*/ CMP         R8, #0 
    /*00004d78*/ BEQ         DIS_DrvCommStartWaitlist_x4e00
    DIS_DrvCommStartWaitlist_x4d7c:
    /*00004d7c*/ MOV         R1, #7 
    /*00004d80*/ LDR         R0, [R8, #8] 
    /*00004d84*/ BL          0x0000000000004D84          @ DIS_HAL_SetMask [R_ARM_CALL: .text+0x625c] 
    /*00004d88*/ MOV         R2, #0 
    /*00004d8c*/ MOV         R3, #0 
    /*00004d90*/ LDR         R0, [R8, #8] 
    /*00004d94*/ BL          0x0000000000004D94          @ DIS_HAL_SetNext [R_ARM_CALL: .text+0x6260] 
    /*00004d98*/ LDR         R2, [R8, #20] 
    /*00004d9c*/ LDR         R3, [R7, #20] 
    /*00004da0*/ MOV         R1, #0 
    /*00004da4*/ MOV         R0, R1 
    /*00004da8*/ ADD         IP, R3, #132                @ 0x84 
    /*00004dac*/ STR         R2, [R3, #52]               @ 0x34 
    /*00004db0*/ LDR         R3, [SP, #8] 
    /*00004db4*/ LDRD        R2, [R3] 
    /*00004db8*/ STM         SP, {R1, IP} 
    /*00004dbc*/ BL          0x0000000000004DBC          @ DIS_HAL_Start [R_ARM_CALL: .text+0x6264] 
    /*00004dc0*/ LDR         R3, [R7, #20] 
    /*00004dc4*/ MOV         R2, #1 
    /*00004dc8*/ STR         R2, [R3, #36]               @ 0x24 
    DIS_DrvCommStartWaitlist_x4dcc:
    /*00004dcc*/ MOV         R0, #0 
    /*00004dd0*/ ADD         SP, SP, #28 
    /*00004dd4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_DrvCommStartWaitlist_x4dd8:
    /*00004dd8*/ STR         R3, [SP, #20] 
    /*00004ddc*/ BL          0x0000000000004520 
    /*00004de0*/ LDR         R3, [SP, #20] 
    /*00004de4*/ B           DIS_DrvCommStartWaitlist_x4cbc
    DIS_DrvCommStartWaitlist_x4de8:
    /*00004de8*/ LDR         R3, [SP, #12] 
    /*00004dec*/ MOVW        R2, #422                    @ 0x1a6 
    /*00004df0*/ LDR         R1, [PC, #40]               @ 0x0000000000004e20 
    /*00004df4*/ LDR         R0, [SP, #16] 
    /*00004df8*/ BL          0x0000000000004DF8          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004dfc*/ B           DIS_DrvCommStartWaitlist_x4d28
    DIS_DrvCommStartWaitlist_x4e00:
    /*00004e00*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c1c] 
    /*00004e04*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1aa4] 
    /*00004e08*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c1c] 
    /*00004e0c*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1aa4] 
    /*00004e10*/ MOVW        R2, #438                    @ 0x1b6 
    /*00004e14*/ LDR         R1, [PC, #4]                @ 0x0000000000004e20 
    /*00004e18*/ BL          0x0000000000004E18          @ osal_panic [R_ARM_CALL: +0x0] 
    /*00004e1c*/ B           DIS_DrvCommStartWaitlist_x4d7c
    /*00004e20*/ ANDEQ       R0, R0, R8, ROR R3 
FUNC_END DIS_DrvCommStartWaitlist


FUNC_BEGIN DIS_DrvCommIntStatistic
    /*00004e24*/ PUSH        {R4, R5, R6, R7, LR} 
    /*00004e28*/ MOVW        IP, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*00004e2c*/ SUB         SP, SP, #20 
    /*00004e30*/ MOVT        IP, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*00004e34*/ ADD         LR, SP, #8 
    /*00004e38*/ MOVW        R6, #16960                  @ 0x4240 
    /*00004e3c*/ LDR         IP, [IP, #20] 
    /*00004e40*/ MOVT        R6, #15 
    /*00004e44*/ STM         LR, {R0, R1} 
    /*00004e48*/ MOV         R1, R0 
    /*00004e4c*/ STM         SP, {R2, R3} 
    /*00004e50*/ MOV         R3, R2 
    /*00004e54*/ LDR         R0, [SP, #4] 
    /*00004e58*/ SUB         R3, R3, R1 
    /*00004e5c*/ LDR         R2, [SP, #12] 
    /*00004e60*/ MOVW        R5, #16959                  @ 0x423f 
    /*00004e64*/ LDR         R4, [IP, #184]              @ 0xb8 
    /*00004e68*/ MOVT        R5, #15 
    /*00004e6c*/ SUB         R0, R0, R2 
    /*00004e70*/ LDR         R7, [IP, #180]              @ 0xb4 
    /*00004e74*/ SUB         R2, R2, R4 
    /*00004e78*/ LDR         R4, [IP, #168]              @ 0xa8 
    /*00004e7c*/ MLA         R3, R6, R3, R0 
    /*00004e80*/ SUB         R1, R1, R7 
    /*00004e84*/ LDR         R0, [IP, #140]              @ 0x8c 
    /*00004e88*/ MLA         R1, R6, R1, R2 
    /*00004e8c*/ CMP         R4, R3 
    /*00004e90*/ LDR         R2, [IP, #156]              @ 0x9c 
    /*00004e94*/ MOVCC       R4, R3 
    /*00004e98*/ STR         R3, [IP, #164]              @ 0xa4 
    /*00004e9c*/ CMP         R1, R5 
    /*00004ea0*/ ADD         R2, R3, R2 
    /*00004ea4*/ STR         R4, [IP, #168]              @ 0xa8 
    /*00004ea8*/ ADD         R3, R0, #1 
    /*00004eac*/ STR         R2, [IP, #156]              @ 0x9c 
    /*00004eb0*/ STR         R3, [IP, #140]              @ 0x8c 
    /*00004eb4*/ BLS         DIS_DrvCommIntStatistic_x4f00
    /*00004eb8*/ LDR         R1, [IP, #144]              @ 0x90 
    /*00004ebc*/ LDR         R0, [IP, #152]              @ 0x98 
    /*00004ec0*/ SUB         R1, R3, R1 
    /*00004ec4*/ STR         R1, [IP, #148]              @ 0x94 
    /*00004ec8*/ CMP         R1, R0 
    /*00004ecc*/ STRHI       R1, [IP, #152]              @ 0x98 
    /*00004ed0*/ LDR         R1, [IP, #160]              @ 0xa0 
    /*00004ed4*/ LDR         R0, [IP, #176]              @ 0xb0 
    /*00004ed8*/ SUB         R1, R2, R1 
    /*00004edc*/ STR         R2, [IP, #160]              @ 0xa0 
    /*00004ee0*/ CMP         R1, R0 
    /*00004ee4*/ STR         R1, [IP, #172]              @ 0xac 
    /*00004ee8*/ STRHI       R1, [IP, #176]              @ 0xb0 
    /*00004eec*/ ADD         R0, IP, #180                @ 0xb4 
    /*00004ef0*/ STR         R3, [IP, #144]              @ 0x90 
    /*00004ef4*/ MOV         R1, LR 
    /*00004ef8*/ MOV         R2, #8 
    /*00004efc*/ BL          0x0000000000004EFC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    DIS_DrvCommIntStatistic_x4f00:
    /*00004f00*/ ADD         SP, SP, #20 
    /*00004f04*/ POP         {R4, R5, R6, R7, PC} 
FUNC_END DIS_DrvCommIntStatistic


FUNC_BEGIN DIS_DrvCommExitEx
    /*00004f08*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00004f0c*/ MOVW        R7, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*00004f10*/ MOVT        R7, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*00004f14*/ SUB         SP, SP, #12 
    /*00004f18*/ LDR         R1, [R7, #20] 
    /*00004f1c*/ LDR         R3, [R1, #104]              @ 0x68 
    /*00004f20*/ CMP         R3, #0 
    /*00004f24*/ MOVWNE      R3, #510                    @ 0x1fe 
    /*00004f28*/ MOVWNE      R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c34] 
    /*00004f2c*/ STRNE       R3, [SP] 
    /*00004f30*/ MOVTNE      R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c34] 
    /*00004f34*/ BNE         DIS_DrvCommExitEx_x5018
    /*00004f38*/ MOV         R3, R1 
    /*00004f3c*/ LDR         R6, [R3, #64]!              @ 0x40 
    /*00004f40*/ CMP         R6, R3 
    /*00004f44*/ SUB         R5, R6, #56                 @ 0x38 
    /*00004f48*/ LDR         R4, [R6] 
    /*00004f4c*/ SUB         R4, R4, #56                 @ 0x38 
    /*00004f50*/ BEQ         DIS_DrvCommExitEx_x4fe4
    /*00004f54*/ MOV         R9, #256                    @ 0x100 
    /*00004f58*/ MOV         R8, #512                    @ 0x200 
    /*00004f5c*/ MOVT        R9, #16 
    /*00004f60*/ MOVT        R8, #32 
    /*00004f64*/ MOV         R2, R6 
    /*00004f68*/ B           DIS_DrvCommExitEx_x4f70
    DIS_DrvCommExitEx_x4f6c:
    /*00004f6c*/ LDR         R2, [R1, #64]               @ 0x40 
    DIS_DrvCommExitEx_x4f70:
    /*00004f70*/ CMP         R2, R3 
    /*00004f74*/ BEQ         DIS_DrvCommExitEx_x5000
    /*00004f78*/ LDR         R3, [R5, #60]               @ 0x3c 
    /*00004f7c*/ LDR         R2, [R5, #56]               @ 0x38 
    /*00004f80*/ STR         R3, [R2, #4] 
    /*00004f84*/ STR         R2, [R3] 
    /*00004f88*/ STR         R9, [R5, #56]               @ 0x38 
    /*00004f8c*/ STR         R8, [R5, #60]               @ 0x3c 
    /*00004f90*/ LDR         R3, [R1, #100]              @ 0x64 
    /*00004f94*/ SUB         R3, R3, #1 
    /*00004f98*/ STR         R3, [R1, #100]              @ 0x64 
    DIS_DrvCommExitEx_x4f9c:
    /*00004f9c*/ LDR         R3, [R7, #20] 
    /*00004fa0*/ ADD         R1, R3, #56                 @ 0x38 
    /*00004fa4*/ LDR         R2, [R3, #60]               @ 0x3c 
    /*00004fa8*/ STR         R6, [R3, #60]               @ 0x3c 
    /*00004fac*/ STR         R1, [R5, #56]               @ 0x38 
    /*00004fb0*/ STR         R2, [R5, #60]               @ 0x3c 
    /*00004fb4*/ MOV         R5, R4 
    /*00004fb8*/ STR         R6, [R2] 
    /*00004fbc*/ MOV         R6, R4 
    /*00004fc0*/ LDR         R2, [R3, #96]               @ 0x60 
    /*00004fc4*/ LDR         R1, [R7, #20] 
    /*00004fc8*/ ADD         R2, R2, #1 
    /*00004fcc*/ STR         R2, [R3, #96]               @ 0x60 
    /*00004fd0*/ LDR         R4, [R6, #56]!              @ 0x38 
    /*00004fd4*/ ADD         R3, R1, #64                 @ 0x40 
    /*00004fd8*/ CMP         R6, R3 
    /*00004fdc*/ SUB         R4, R4, #56                 @ 0x38 
    /*00004fe0*/ BNE         DIS_DrvCommExitEx_x4f6c
    DIS_DrvCommExitEx_x4fe4:
    /*00004fe4*/ LDR         R2, [R1, #4] 
    /*00004fe8*/ LDR         R3, [R1, #96]               @ 0x60 
    /*00004fec*/ CMP         R2, R3 
    /*00004ff0*/ MOVEQ       R0, #0 
    /*00004ff4*/ BNE         DIS_DrvCommExitEx_x5008
    DIS_DrvCommExitEx_x4ff8:
    /*00004ff8*/ ADD         SP, SP, #12 
    /*00004ffc*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    DIS_DrvCommExitEx_x5000:
    /*00005000*/ BL          0x0000000000004520 
    /*00005004*/ B           DIS_DrvCommExitEx_x4f9c
    DIS_DrvCommExitEx_x5008:
    /*00005008*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c68] 
    /*0000500c*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c68] 
    /*00005010*/ MOVW        R3, #527                    @ 0x20f 
    /*00005014*/ STR         R3, [SP] 
    DIS_DrvCommExitEx_x5018:
    /*00005018*/ LDR         R3, [PC, #20]               @ 0x0000000000005034 
    /*0000501c*/ MOV         R1, #17 
    /*00005020*/ MOV         R0, #0 
    /*00005024*/ BL          0x0000000000005024          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005028*/ MOVW        R0, #32777                  @ 0x8009 
    /*0000502c*/ MOVT        R0, #40977                  @ 0xa011 
    /*00005030*/ B           DIS_DrvCommExitEx_x4ff8
    /*00005034*/ MULEQ       R0, R4, R3 
FUNC_END DIS_DrvCommExitEx


FUNC_BEGIN DIS_DrvCommListInt
    /*00005038*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000503c*/ MOVW        R7, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*00005040*/ MOVT        R7, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*00005044*/ SUBS        FP, R1, #0 
    /*00005048*/ SUB         SP, SP, #52                 @ 0x34 
    /*0000504c*/ MVN         R3, #0 
    /*00005050*/ LDR         R2, [R7, #20] 
    /*00005054*/ MOV         R4, R0 
    /*00005058*/ STR         R3, [SP, #20] 
    /*0000505c*/ ADD         R1, SP, #16 
    /*00005060*/ LDR         R0, [PC, #552]              @ 0x0000000000005290 
    /*00005064*/ LDREQ       R3, [R2, #116]              @ 0x74 
    /*00005068*/ LDRNE       R3, [R2, #120]              @ 0x78 
    /*0000506c*/ ADDEQ       R3, R3, #1 
    /*00005070*/ ADDNE       R3, R3, #1 
    /*00005074*/ STREQ       R3, [R2, #116]              @ 0x74 
    /*00005078*/ STRNE       R3, [R2, #120]              @ 0x78 
    /*0000507c*/ BL          0x000000000000507C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00005080*/ LDR         R2, [R7, #20] 
    /*00005084*/ ADD         R1, R2, R4, LSL #2 
    /*00005088*/ LDR         R9, [R1, #24] 
    /*0000508c*/ CLZ         R3, R9 
    /*00005090*/ LSR         R3, R3, #5 
    /*00005094*/ STR         R3, [R1, #24] 
    /*00005098*/ LDR         R3, [R2, #44]               @ 0x2c 
    /*0000509c*/ CMP         R3, #1 
    /*000050a0*/ BEQ         DIS_DrvCommListInt_x50b0
    /*000050a4*/ MOV         R0, R4 
    /*000050a8*/ ADD         R1, SP, #20 
    /*000050ac*/ BL          0x00000000000050AC          @ DIS_DrvCommSubmit2Vgs [R_ARM_CALL: .text+0x4820] 
    DIS_DrvCommListInt_x50b0:
    /*000050b0*/ ADD         R1, SP, #16 
    /*000050b4*/ LDR         R0, [PC, #468]              @ 0x0000000000005290 
    /*000050b8*/ BL          0x00000000000050B8          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000050bc*/ LDR         R3, [SP, #20] 
    /*000050c0*/ CMN         R3, #1 
    /*000050c4*/ BEQ         DIS_DrvCommListInt_x50f0
    /*000050c8*/ MOV         R0, #45                     @ 0x2d 
    /*000050cc*/ BL          0x00000000000050CC          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*000050d0*/ CMP         R0, #0 
    /*000050d4*/ BEQ         DIS_DrvCommListInt_x526c
    /*000050d8*/ LDR         R3, [R0, #12] 
    /*000050dc*/ CMP         R3, #0 
    /*000050e0*/ BEQ         DIS_DrvCommListInt_x526c
    /*000050e4*/ LDR         R3, [R0, #4] 
    /*000050e8*/ LDR         R0, [SP, #20] 
    /*000050ec*/ BLX         R3 
    DIS_DrvCommListInt_x50f0:
    /*000050f0*/ ADDS        R3, R9, #0 
    /*000050f4*/ MOVNE       R3, #1 
    /*000050f8*/ STR         R3, [SP, #8] 
    /*000050fc*/ MOV         R2, R3 
    /*00005100*/ LDR         R3, [R7, #20] 
    /*00005104*/ ADD         SL, R2, #9 
    /*00005108*/ LDR         R2, [R3, SL, LSL #3] 
    /*0000510c*/ LSL         SL, SL, #3 
    /*00005110*/ ADD         R1, R3, SL 
    /*00005114*/ CMP         R2, R1 
    /*00005118*/ SUB         R4, R2, #56                 @ 0x38 
    /*0000511c*/ LDR         R6, [R2] 
    /*00005120*/ MOV         R5, R2 
    /*00005124*/ SUB         R6, R6, #56                 @ 0x38 
    /*00005128*/ BEQ         DIS_DrvCommListInt_x5210
    /*0000512c*/ LDR         R8, [PC, #348]              @ 0x0000000000005290 
    /*00005130*/ ADDS        R3, FP, #0 
    /*00005134*/ MOVW        FP, #16960                  @ 0x4240 
    /*00005138*/ MOVT        FP, #15 
    /*0000513c*/ MOVNE       R3, #1 
    /*00005140*/ STR         R3, [SP, #12] 
    DIS_DrvCommListInt_x5144:
    /*00005144*/ ADD         R1, SP, #16 
    /*00005148*/ MOV         R0, R8 
    /*0000514c*/ BL          0x000000000000514C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00005150*/ ADD         R0, R4, #48                 @ 0x30 
    /*00005154*/ BL          0x0000000000005154          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00005158*/ LDR         R1, [R4, #40]               @ 0x28 
    /*0000515c*/ LDR         IP, [R4, #48]               @ 0x30 
    /*00005160*/ MOV         R2, #0 
    /*00005164*/ LDR         R0, [R4, #52]               @ 0x34 
    /*00005168*/ SUB         IP, IP, R1 
    /*0000516c*/ LDR         R1, [R4, #44]               @ 0x2c 
    /*00005170*/ LDR         R3, [SP, #8] 
    /*00005174*/ SUB         R0, R0, R1 
    /*00005178*/ MOV         R1, R4 
    /*0000517c*/ MLA         IP, FP, IP, R0 
    /*00005180*/ LDR         R0, [R7, #20] 
    /*00005184*/ STR         IP, [R4, #36]               @ 0x24 
    /*00005188*/ BL          0x0000000000005188          @ DIS_DrvGetNodeFromBusyList [R_ARM_CALL: .text+0x470c] 
    /*0000518c*/ LDR         R3, [R7, #20] 
    /*00005190*/ LDR         R9, [R4, #16] 
    /*00005194*/ ADD         R1, SP, #16 
    /*00005198*/ ADD         R0, R3, #56                 @ 0x38 
    /*0000519c*/ LDR         IP, [R3, #60]               @ 0x3c 
    /*000051a0*/ STR         R5, [R3, #60]               @ 0x3c 
    /*000051a4*/ STR         R0, [R4, #56]               @ 0x38 
    /*000051a8*/ MOV         R0, R8 
    /*000051ac*/ STR         IP, [R4, #60]               @ 0x3c 
    /*000051b0*/ STR         R5, [IP] 
    /*000051b4*/ MOV         R5, R6 
    /*000051b8*/ LDR         IP, [R3, #96]               @ 0x60 
    /*000051bc*/ ADD         IP, IP, #1 
    /*000051c0*/ STR         IP, [R3, #96]               @ 0x60 
    /*000051c4*/ BL          0x00000000000051C4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000051c8*/ ADD         R0, SP, #24 
    /*000051cc*/ BL          0x00000000000051CC          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000051d0*/ LDR         R3, [R4, #36]               @ 0x24 
    /*000051d4*/ LDR         R2, [SP, #12] 
    /*000051d8*/ ADD         R1, SP, #40                 @ 0x28 
    /*000051dc*/ MOV         R0, R9 
    /*000051e0*/ MOV         R4, R6 
    /*000051e4*/ STR         R3, [SP, #44]               @ 0x2c 
    /*000051e8*/ STR         R2, [SP, #40]               @ 0x28 
    /*000051ec*/ BL          0x00000000000051EC          @ DIS_IntCallBack [R_ARM_CALL: .text+0x3368] 
    /*000051f0*/ ADD         R0, SP, #32 
    /*000051f4*/ BL          0x00000000000051F4          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000051f8*/ LDR         R3, [R7, #20] 
    /*000051fc*/ LDR         R6, [R5, #56]!              @ 0x38 
    /*00005200*/ ADD         R2, R3, SL 
    /*00005204*/ CMP         R5, R2 
    /*00005208*/ SUB         R6, R6, #56                 @ 0x38 
    /*0000520c*/ BNE         DIS_DrvCommListInt_x5144
    DIS_DrvCommListInt_x5210:
    /*00005210*/ LDR         R2, [R3, #44]               @ 0x2c 
    /*00005214*/ CMP         R2, #1 
    /*00005218*/ BEQ         DIS_DrvCommListInt_x5258
    DIS_DrvCommListInt_x521c:
    /*0000521c*/ ADD         R1, SP, #16 
    /*00005220*/ LDR         R0, [PC, #104]              @ 0x0000000000005290 
    /*00005224*/ BL          0x0000000000005224          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00005228*/ LDR         R2, [R7, #20] 
    /*0000522c*/ ADD         R1, SP, #16 
    /*00005230*/ LDR         R0, [PC, #88]               @ 0x0000000000005290 
    /*00005234*/ LDR         R3, [R2, #24] 
    /*00005238*/ ADD         R3, R3, #22 
    /*0000523c*/ LDR         R3, [R2, R3, LSL #2] 
    /*00005240*/ CMP         R3, #0 
    /*00005244*/ STREQ       R3, [R2, #36]               @ 0x24 
    /*00005248*/ BL          0x0000000000005248          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*0000524c*/ MOV         R0, #0 
    /*00005250*/ ADD         SP, SP, #52                 @ 0x34 
    /*00005254*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_DrvCommListInt_x5258:
    /*00005258*/ LDR         R3, [R3, #104]              @ 0x68 
    /*0000525c*/ CMP         R3, #0 
    /*00005260*/ BNE         DIS_DrvCommListInt_x521c
    /*00005264*/ BL          0x0000000000005264          @ DIS_DrvCommExitEx [R_ARM_CALL: .text+0x4f08] 
    /*00005268*/ B           DIS_DrvCommListInt_x521c
    DIS_DrvCommListInt_x526c:
    /*0000526c*/ MOVW        R3, #582                    @ 0x246 
    /*00005270*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1b28] 
    /*00005274*/ STR         R3, [SP] 
    /*00005278*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1b28] 
    /*0000527c*/ LDR         R3, [PC, #16]               @ 0x0000000000005294 
    /*00005280*/ MOV         R1, #17 
    /*00005284*/ MOV         R0, #3 
    /*00005288*/ BL          0x0000000000005288          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000528c*/ B           DIS_DrvCommListInt_x50f0
    /*00005290*/ LDRDEQ      R0, [R0], -IP 
    /*00005294*/ ANDEQ       R0, R0, R8, LSR #7 
FUNC_END DIS_DrvCommListInt


FUNC_BEGIN DIS_DrvCommIrqProc
    /*00005298*/ PUSH        {R4, R5, R6, LR} 
    /*0000529c*/ SUB         SP, SP, #40                 @ 0x28 
    /*000052a0*/ ADD         R5, SP, #24 
    /*000052a4*/ MOV         R4, #0 
    /*000052a8*/ STR         R4, [SP, #20] 
    /*000052ac*/ MOV         R0, R5 
    /*000052b0*/ BL          0x00000000000052B0          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*000052b4*/ MOV         R0, R4 
    /*000052b8*/ ADD         R1, SP, #20 
    /*000052bc*/ BL          0x00000000000052BC          @ DIS_HAL_GetIntStatus [R_ARM_CALL: .text+0x6268] 
    /*000052c0*/ LDR         R0, [SP, #20] 
    /*000052c4*/ BL          0x00000000000052C4          @ DIS_DRV_IsWrongInt [R_ARM_CALL: .text+0x60e8] 
    /*000052c8*/ SUBS        R6, R0, #0 
    /*000052cc*/ BEQ         DIS_DrvCommIrqProc_x52e0
    /*000052d0*/ CMP         R6, #4 
    /*000052d4*/ BEQ         DIS_DrvCommIrqProc_x5384
    /*000052d8*/ CMP         R6, #2 
    /*000052dc*/ BEQ         DIS_DrvCommIrqProc_x53a0
    DIS_DrvCommIrqProc_x52e0:
    /*000052e0*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*000052e4*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    DIS_DrvCommIrqProc_x52e8:
    /*000052e8*/ MOV         R0, #0 
    /*000052ec*/ BL          0x00000000000052EC          @ DIS_HAL_ClrInt [R_ARM_CALL: .text+0x626c] 
    /*000052f0*/ LDR         R1, [R4, #20] 
    /*000052f4*/ MOV         R0, #0 
    /*000052f8*/ ADD         R1, R1, #48                 @ 0x30 
    /*000052fc*/ BL          0x00000000000052FC          @ DIS_HAL_GetNodeID [R_ARM_CALL: .text+0x6270] 
    /*00005300*/ LDR         R3, [R4, #20] 
    /*00005304*/ LDR         R2, [R3, #52]               @ 0x34 
    /*00005308*/ LDR         R3, [R3, #48]               @ 0x30 
    /*0000530c*/ CMP         R2, R3 
    /*00005310*/ BEQ         DIS_DrvCommIrqProc_x5354
    /*00005314*/ STR         R3, [SP, #12] 
    /*00005318*/ MOV         R1, #0 
    /*0000531c*/ STR         R2, [SP, #8] 
    /*00005320*/ MOVW        R3, #687                    @ 0x2af 
    /*00005324*/ STR         R1, [SP, #4] 
    /*00005328*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1c98] 
    /*0000532c*/ STR         R3, [SP] 
    /*00005330*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1c98] 
    /*00005334*/ LDR         R3, [PC, #120]              @ 0x00000000000053b4 
    /*00005338*/ MOV         R1, #17 
    /*0000533c*/ MOV         R0, #3 
    /*00005340*/ BL          0x0000000000005340          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005344*/ LDR         R2, [R4, #20] 
    /*00005348*/ LDR         R3, [R2, #136]              @ 0x88 
    /*0000534c*/ ADD         R3, R3, #1 
    /*00005350*/ STR         R3, [R2, #136]              @ 0x88 
    DIS_DrvCommIrqProc_x5354:
    /*00005354*/ ADD         R4, SP, #32 
    /*00005358*/ MOV         R1, R6 
    /*0000535c*/ MOV         R0, #0 
    /*00005360*/ BL          0x0000000000005360          @ DIS_DrvCommListInt [R_ARM_CALL: .text+0x5038] 
    /*00005364*/ MOV         R0, R4 
    /*00005368*/ BL          0x0000000000005368          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*0000536c*/ LDM         R5, {R0, R1} 
    /*00005370*/ LDM         R4, {R2, R3} 
    /*00005374*/ BL          0x0000000000005374          @ DIS_DrvCommIntStatistic [R_ARM_CALL: .text+0x4e24] 
    /*00005378*/ MOV         R0, #1 
    /*0000537c*/ ADD         SP, SP, #40                 @ 0x28 
    /*00005380*/ POP         {R4, R5, R6, PC} 
    DIS_DrvCommIrqProc_x5384:
    /*00005384*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*00005388*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*0000538c*/ LDR         R2, [R4, #20] 
    /*00005390*/ LDR         R3, [R2, #128]              @ 0x80 
    /*00005394*/ ADD         R3, R3, #1 
    /*00005398*/ STR         R3, [R2, #128]              @ 0x80 
    /*0000539c*/ B           DIS_DrvCommIrqProc_x52e8
    DIS_DrvCommIrqProc_x53a0:
    /*000053a0*/ MOV         R0, R4 
    /*000053a4*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*000053a8*/ BL          0x00000000000053A8          @ DIS_HAL_PrintRegStatus [R_ARM_CALL: .text+0x65e4] 
    /*000053ac*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*000053b0*/ B           DIS_DrvCommIrqProc_x52e8
    /*000053b4*/ @           <UNDEFINED> instruction: 0x000003bc 
FUNC_END DIS_DrvCommIrqProc


FUNC_BEGIN DIS_DrvCommInit
    /*000053b8*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*000053bc*/ LSL         R4, R0, #6 
    /*000053c0*/ SUB         SP, SP, #36                 @ 0x24 
    /*000053c4*/ MOV         R5, R0 
    /*000053c8*/ MOV         R0, R4 
    /*000053cc*/ MOV         R2, #0 
    /*000053d0*/ MOV         R3, #0 
    /*000053d4*/ MOV         R7, #0 
    /*000053d8*/ STRD        R2, [SP, #24] 
    /*000053dc*/ STR         R7, [SP, #20] 
    /*000053e0*/ BL          0x00000000000053E0          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*000053e4*/ SUBS        R6, R0, #0 
    /*000053e8*/ BEQ         DIS_DrvCommInit_x5528
    /*000053ec*/ MOV         R2, R4 
    /*000053f0*/ MOV         R1, R7 
    /*000053f4*/ BL          0x00000000000053F4          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000053f8*/ MOV         R8, #16 
    /*000053fc*/ MOV         R3, #224                    @ 0xe0 
    /*00005400*/ MOV         R9, #0 
    /*00005404*/ UMLAL       R8, R9, R3, R5 
    /*00005408*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*0000540c*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*00005410*/ MOVW        R1, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d28] 
    /*00005414*/ ADD         R3, SP, #20 
    /*00005418*/ MOVT        R1, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d28] 
    /*0000541c*/ LDR         IP, [R4, #20] 
    /*00005420*/ ADD         R2, SP, #24 
    /*00005424*/ MOV         R0, R7 
    /*00005428*/ STR         R6, [IP] 
    /*0000542c*/ STR         R8, [SP] 
    /*00005430*/ BL          0x0000000000005430          @ CMPI_MmzMallocNocache [R_ARM_CALL: +0x0] 
    /*00005434*/ SUBS        R6, R0, #0 
    /*00005438*/ BNE         DIS_DrvCommInit_x54f0
    /*0000543c*/ MOV         R2, R8 
    /*00005440*/ MOV         R1, R6 
    /*00005444*/ LDR         R0, [SP, #20] 
    /*00005448*/ BL          0x0000000000005448          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000544c*/ LDRD        R8, [SP, #24] 
    /*00005450*/ LDR         R3, [R4, #20] 
    /*00005454*/ ADDS        R0, R8, #15 
    /*00005458*/ LDR         R2, [SP, #20] 
    /*0000545c*/ ADC         R1, R9, #0 
    /*00005460*/ CMP         R5, #0 
    /*00005464*/ BIC         R0, R0, #15 
    /*00005468*/ STRD        R8, [R3, #8] 
    /*0000546c*/ STR         R2, [R3, #16] 
    /*00005470*/ BEQ         DIS_DrvCommInit_x54e4
    /*00005474*/ MOV         IP, R6 
    /*00005478*/ B           DIS_DrvCommInit_x5480
    DIS_DrvCommInit_x547c:
    /*0000547c*/ LDR         R3, [R4, #20] 
    DIS_DrvCommInit_x5480:
    /*00005480*/ LDR         LR, [R3] 
    /*00005484*/ LSL         R3, IP, #6 
    /*00005488*/ ADD         IP, IP, #1 
    /*0000548c*/ ADD         R2, LR, R3 
    /*00005490*/ STRD        R0, [LR, R3] 
    /*00005494*/ ADD         R8, R2, #56                 @ 0x38 
    /*00005498*/ LDR         R3, [R4, #20] 
    /*0000549c*/ LDR         R7, [SP, #24] 
    /*000054a0*/ LDR         LR, [SP, #20] 
    /*000054a4*/ ADD         R9, R3, #56                 @ 0x38 
    /*000054a8*/ SUB         R7, R0, R7 
    /*000054ac*/ ADDS        R0, R0, #224                @ 0xe0 
    /*000054b0*/ ADD         LR, LR, R7 
    /*000054b4*/ STR         LR, [R2, #8] 
    /*000054b8*/ LDR         LR, [R3, #60]               @ 0x3c 
    /*000054bc*/ ADC         R1, R1, #0 
    /*000054c0*/ STR         R8, [R3, #60]               @ 0x3c 
    /*000054c4*/ CMP         R5, IP 
    /*000054c8*/ STR         R9, [R2, #56]               @ 0x38 
    /*000054cc*/ STR         LR, [R2, #60]               @ 0x3c 
    /*000054d0*/ STR         R8, [LR] 
    /*000054d4*/ LDR         R2, [R3, #96]               @ 0x60 
    /*000054d8*/ ADD         R2, R2, #1 
    /*000054dc*/ STR         R2, [R3, #96]               @ 0x60 
    /*000054e0*/ BNE         DIS_DrvCommInit_x547c
    DIS_DrvCommInit_x54e4:
    /*000054e4*/ MOV         R0, R6 
    /*000054e8*/ ADD         SP, SP, #36                 @ 0x24 
    /*000054ec*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    DIS_DrvCommInit_x54f0:
    /*000054f0*/ LDR         R3, [R4, #20] 
    /*000054f4*/ MOVW        R6, #32781                  @ 0x800d 
    /*000054f8*/ MOVT        R6, #40977                  @ 0xa011 
    /*000054fc*/ LDR         R0, [R3] 
    /*00005500*/ BL          0x0000000000005500          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*00005504*/ MOV         R3, #744                    @ 0x2e8 
    /*00005508*/ MOV         R0, R7 
    /*0000550c*/ STR         R3, [SP] 
    /*00005510*/ MOVW        R2, #0                      @ .LC14 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d34] 
    /*00005514*/ LDR         R3, [PC, #68]               @ 0x0000000000005560 
    /*00005518*/ MOVT        R2, #0                      @ .LC14 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d34] 
    /*0000551c*/ MOV         R1, #17 
    /*00005520*/ BL          0x0000000000005520          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005524*/ B           DIS_DrvCommInit_x54e4
    DIS_DrvCommInit_x5528:
    /*00005528*/ MOV         R2, #4 
    /*0000552c*/ MOVW        R3, #725                    @ 0x2d5 
    /*00005530*/ STR         R2, [SP, #8] 
    /*00005534*/ MOV         R1, #17 
    /*00005538*/ STR         R3, [SP] 
    /*0000553c*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ce0] 
    /*00005540*/ STR         R5, [SP, #4] 
    /*00005544*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ce0] 
    /*00005548*/ LDR         R3, [PC, #16]               @ 0x0000000000005560 
    /*0000554c*/ MOV         R0, #1 
    /*00005550*/ MOVW        R6, #32781                  @ 0x800d 
    /*00005554*/ BL          0x0000000000005554          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005558*/ MOVT        R6, #40977                  @ 0xa011 
    /*0000555c*/ B           DIS_DrvCommInit_x54e4
    /*00005560*/ LDRDEQ      R0, [R0], -R0               @ <UNPREDICTABLE> 
FUNC_END DIS_DrvCommInit


FUNC_BEGIN DIS_DrvCommDeInit
    /*00005564*/ PUSH        {R4, LR} 
    /*00005568*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*0000556c*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*00005570*/ SUB         SP, SP, #8 
    /*00005574*/ LDR         R3, [R4, #20] 
    /*00005578*/ LDR         R2, [R3] 
    /*0000557c*/ CMP         R2, #0 
    /*00005580*/ BEQ         DIS_DrvCommDeInit_x55c4
    /*00005584*/ STR         R2, [SP, #4] 
    /*00005588*/ MOVW        R3, #770                    @ 0x302 
    /*0000558c*/ MOV         R1, #17 
    /*00005590*/ STR         R3, [SP] 
    /*00005594*/ MOVW        R2, #0                      @ .LC15 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1d70] 
    /*00005598*/ LDR         R3, [PC, #80]               @ 0x00000000000055f0 
    /*0000559c*/ MOVT        R2, #0                      @ .LC15 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1d70] 
    /*000055a0*/ MOV         R0, #7 
    /*000055a4*/ BL          0x00000000000055A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000055a8*/ LDR         R3, [R4, #20] 
    /*000055ac*/ LDR         R0, [R3] 
    /*000055b0*/ BL          0x00000000000055B0          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*000055b4*/ LDR         R3, [R4, #20] 
    /*000055b8*/ MOV         R2, #0 
    /*000055bc*/ STR         R2, [R3] 
    /*000055c0*/ LDR         R3, [R4, #20] 
    DIS_DrvCommDeInit_x55c4:
    /*000055c4*/ LDRD        R0, [R3, #8] 
    /*000055c8*/ LDR         R2, [R3, #16] 
    /*000055cc*/ BL          0x00000000000055CC          @ CMPI_MmzFree [R_ARM_CALL: +0x0] 
    /*000055d0*/ LDR         R3, [R4, #20] 
    /*000055d4*/ MOV         R0, #0 
    /*000055d8*/ MOV         R1, #0 
    /*000055dc*/ MOV         R2, #0 
    /*000055e0*/ STRD        R0, [R3, #8] 
    /*000055e4*/ STR         R2, [R3, #16] 
    /*000055e8*/ ADD         SP, SP, #8 
    /*000055ec*/ POP         {R4, PC} 
    /*000055f0*/ ANDEQ       R0, R0, R0, ROR #7 
FUNC_END DIS_DrvCommDeInit


FUNC_BEGIN DIS_DrvCommOpen
    /*000055f4*/ PUSH        {R4, R5, R6, LR} 
    /*000055f8*/ MOV         R5, R0 
    /*000055fc*/ SUB         SP, SP, #16 
    /*00005600*/ MOV         R0, #192                    @ 0xc0 
    /*00005604*/ BL          0x0000000000005604          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*00005608*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*0000560c*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*00005610*/ CMP         R0, #0 
    /*00005614*/ STR         R0, [R4, #20] 
    /*00005618*/ BNE         DIS_DrvCommOpen_x564c
    /*0000561c*/ STR         R0, [SP, #4] 
    /*00005620*/ MOV         R3, #796                    @ 0x31c 
    /*00005624*/ MOVW        R2, #0                      @ .LC16 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1da8] 
    /*00005628*/ STR         R3, [SP] 
    /*0000562c*/ MOVT        R2, #0                      @ .LC16 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1da8] 
    /*00005630*/ LDR         R3, [PC, #472]              @ 0x0000000000005810 
    /*00005634*/ MOV         R1, #17 
    /*00005638*/ MOV         R0, #3 
    /*0000563c*/ BL          0x000000000000563C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005640*/ MVN         R0, #0 
    DIS_DrvCommOpen_x5644:
    /*00005644*/ ADD         SP, SP, #16 
    /*00005648*/ POP         {R4, R5, R6, PC} 
    DIS_DrvCommOpen_x564c:
    /*0000564c*/ MOV         R2, #192                    @ 0xc0 
    /*00005650*/ MOV         R1, #0 
    /*00005654*/ BL          0x0000000000005654          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00005658*/ LDR         R3, [R4, #20] 
    /*0000565c*/ MOVW        R6, #0                      @ .LANCHOR2 [R_ARM_MOVW_ABS_NC: .data+0x5c] 
    /*00005660*/ MOVT        R6, #0                      @ .LANCHOR2 [R_ARM_MOVT_ABS: .data+0x5c] 
    /*00005664*/ MOVW        R2, #0                      @ g_ps32NodeIdErrCnt [R_ARM_MOVW_ABS_NC: .bss+0x1e0] 
    /*00005668*/ MOVT        R2, #0                      @ g_ps32NodeIdErrCnt [R_ARM_MOVT_ABS: .bss+0x1e0] 
    /*0000566c*/ ADD         R1, R3, #56                 @ 0x38 
    /*00005670*/ ADD         IP, R3, #64                 @ 0x40 
    /*00005674*/ ADD         R0, R3, #72                 @ 0x48 
    /*00005678*/ STR         R1, [R3, #56]               @ 0x38 
    /*0000567c*/ STR         R1, [R3, #60]               @ 0x3c 
    /*00005680*/ ADD         R1, R3, #80                 @ 0x50 
    /*00005684*/ STR         IP, [R3, #64]               @ 0x40 
    /*00005688*/ STR         IP, [R3, #68]               @ 0x44 
    /*0000568c*/ ADD         IP, R3, #136                @ 0x88 
    /*00005690*/ STR         R0, [R3, #72]               @ 0x48 
    /*00005694*/ STR         R0, [R3, #76]               @ 0x4c 
    /*00005698*/ STR         R1, [R3, #80]               @ 0x50 
    /*0000569c*/ STR         R1, [R3, #84]               @ 0x54 
    /*000056a0*/ LDR         R0, [R6] 
    /*000056a4*/ STR         IP, [R2] 
    /*000056a8*/ BL          0x00000000000056A8          @ DIS_DrvCommInit [R_ARM_CALL: .text+0x53b8] 
    /*000056ac*/ CMP         R0, #0 
    /*000056b0*/ BNE         DIS_DrvCommOpen_x5704
    /*000056b4*/ LDR         R3, [R4, #20] 
    /*000056b8*/ MOVW        R0, #0                      @ DIS_DrvCommIrqProc [R_ARM_MOVW_ABS_NC: .text+0x5298] 
    /*000056bc*/ LDR         R2, [R6] 
    /*000056c0*/ MOVT        R0, #0                      @ DIS_DrvCommIrqProc [R_ARM_MOVT_ABS: .text+0x5298] 
    /*000056c4*/ STR         R5, [R3, #20] 
    /*000056c8*/ STR         R2, [R3, #4] 
    /*000056cc*/ BL          0x00000000000056CC          @ DIS_DRV_RequestIrq [R_ARM_CALL: .text+0x6108] 
    /*000056d0*/ CMP         R0, #0 
    /*000056d4*/ BEQ         DIS_DrvCommOpen_x572c
    DIS_DrvCommOpen_x56d8:
    /*000056d8*/ LDR         R3, [R4, #20] 
    /*000056dc*/ CMP         R3, #0 
    /*000056e0*/ MVNEQ       R0, #0 
    /*000056e4*/ BEQ         DIS_DrvCommOpen_x5644
    /*000056e8*/ BL          0x00000000000056E8          @ DIS_DrvCommDeInit [R_ARM_CALL: .text+0x5564] 
    /*000056ec*/ LDR         R0, [R4, #20] 
    /*000056f0*/ BL          0x00000000000056F0          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*000056f4*/ MOV         R3, #0 
    /*000056f8*/ MVN         R0, #0 
    /*000056fc*/ STR         R3, [R4, #20] 
    /*00005700*/ B           DIS_DrvCommOpen_x5644
    DIS_DrvCommOpen_x5704:
    /*00005704*/ STR         R0, [SP, #4] 
    /*00005708*/ MOVW        R3, #823                    @ 0x337 
    /*0000570c*/ MOVW        R2, #0                      @ .LC17 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1df4] 
    /*00005710*/ STR         R3, [SP] 
    /*00005714*/ MOVT        R2, #0                      @ .LC17 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1df4] 
    /*00005718*/ LDR         R3, [PC, #240]              @ 0x0000000000005810 
    /*0000571c*/ MOV         R1, #17 
    /*00005720*/ MOV         R0, #3 
    /*00005724*/ BL          0x0000000000005724          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005728*/ B           DIS_DrvCommOpen_x56d8
    DIS_DrvCommOpen_x572c:
    /*0000572c*/ LDR         R3, [R4, #20] 
    /*00005730*/ STR         R0, [R3, #24] 
    /*00005734*/ BL          0x0000000000005734          @ DIS_HAL_Open [R_ARM_CALL: .text+0x6248] 
    /*00005738*/ CMP         R0, #0 
    /*0000573c*/ BEQ         DIS_DrvCommOpen_x5768
    /*00005740*/ STR         R0, [SP, #4] 
    /*00005744*/ MOV         R3, #848                    @ 0x350 
    /*00005748*/ MOVW        R2, #0                      @ .LC18 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e28] 
    /*0000574c*/ STR         R3, [SP] 
    /*00005750*/ MOVT        R2, #0                      @ .LC18 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e28] 
    /*00005754*/ LDR         R3, [PC, #180]              @ 0x0000000000005810 
    /*00005758*/ MOV         R1, #17 
    /*0000575c*/ MOV         R0, #3 
    /*00005760*/ BL          0x0000000000005760          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005764*/ B           DIS_DrvCommOpen_x56d8
    DIS_DrvCommOpen_x5768:
    /*00005768*/ ADD         R0, R4, #24 
    /*0000576c*/ BL          0x000000000000576C          @ osal_spin_lock_init [R_ARM_CALL: +0x0] 
    /*00005770*/ CMP         R0, #0 
    /*00005774*/ STRNE       R0, [SP, #4] 
    /*00005778*/ MOVWNE      R3, #855                    @ 0x357 
    /*0000577c*/ BNE         DIS_DrvCommOpen_x5798
    /*00005780*/ MOV         R0, R4 
    /*00005784*/ BL          0x0000000000005784          @ osal_spin_lock_init [R_ARM_CALL: +0x0] 
    /*00005788*/ CMP         R0, #0 
    /*0000578c*/ BEQ         DIS_DrvCommOpen_x57b8
    /*00005790*/ MOVW        R3, #863                    @ 0x35f 
    /*00005794*/ STR         R0, [SP, #4] 
    DIS_DrvCommOpen_x5798:
    /*00005798*/ STR         R3, [SP] 
    /*0000579c*/ MOVW        R2, #0                      @ .LC19 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e5c] 
    /*000057a0*/ LDR         R3, [PC, #104]              @ 0x0000000000005810 
    /*000057a4*/ MOVT        R2, #0                      @ .LC19 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e5c] 
    /*000057a8*/ MOV         R1, #17 
    /*000057ac*/ MOV         R0, #3 
    /*000057b0*/ BL          0x00000000000057B0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*000057b4*/ B           DIS_DrvCommOpen_x56d8
    DIS_DrvCommOpen_x57b8:
    /*000057b8*/ ADD         R0, R4, #8 
    /*000057bc*/ BL          0x00000000000057BC          @ osal_timer_init [R_ARM_CALL: +0x0] 
    /*000057c0*/ CMP         R0, #0 
    /*000057c4*/ BNE         DIS_DrvCommOpen_x57e8
    /*000057c8*/ STR         R0, [R4, #16] 
    /*000057cc*/ MOVW        R3, #0                      @ DisTimerISR [R_ARM_MOVW_ABS_NC: .text+0x4928] 
    /*000057d0*/ STR         R0, [SP, #12] 
    /*000057d4*/ MOVT        R3, #0                      @ DisTimerISR [R_ARM_MOVT_ABS: .text+0x4928] 
    /*000057d8*/ STR         R3, [R4, #12] 
    /*000057dc*/ BL          0x00000000000057DC          @ DIS_DRV_PerformaceLimit [R_ARM_CALL: .text+0x6118] 
    /*000057e0*/ LDR         R0, [SP, #12] 
    /*000057e4*/ B           DIS_DrvCommOpen_x5644
    DIS_DrvCommOpen_x57e8:
    /*000057e8*/ STR         R0, [SP, #4] 
    /*000057ec*/ MOVW        R3, #871                    @ 0x367 
    /*000057f0*/ MOVW        R2, #0                      @ .LC20 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1e90] 
    /*000057f4*/ STR         R3, [SP] 
    /*000057f8*/ MOVT        R2, #0                      @ .LC20 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1e90] 
    /*000057fc*/ LDR         R3, [PC, #12]               @ 0x0000000000005810 
    /*00005800*/ MOV         R1, #17 
    /*00005804*/ MOV         R0, #3 
    /*00005808*/ BL          0x0000000000005808          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000580c*/ B           DIS_DrvCommOpen_x56d8
    /*00005810*/ STRDEQ      R0, [R0], -R4 
FUNC_END DIS_DrvCommOpen


FUNC_BEGIN DIS_DrvCommClose
    /*00005814*/ PUSH        {R4, R5, LR} 
    /*00005818*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*0000581c*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*00005820*/ SUB         SP, SP, #12 
    /*00005824*/ LDR         R3, [R4, #20] 
    /*00005828*/ CMP         R3, #0 
    /*0000582c*/ BEQ         DIS_DrvCommClose_x5844
    /*00005830*/ BL          0x0000000000005830          @ DIS_DrvCommDeInit [R_ARM_CALL: .text+0x5564] 
    /*00005834*/ LDR         R0, [R4, #20] 
    /*00005838*/ BL          0x0000000000005838          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*0000583c*/ MOV         R3, #0 
    /*00005840*/ STR         R3, [R4, #20] 
    DIS_DrvCommClose_x5844:
    /*00005844*/ LDR         R0, [PC, #88]               @ 0x00000000000058a4 
    /*00005848*/ MOV         R5, #0 
    /*0000584c*/ BL          0x000000000000584C          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00005850*/ ADD         R1, SP, #4 
    /*00005854*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*00005858*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*0000585c*/ BL          0x000000000000585C          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*00005860*/ ADD         R1, SP, #4 
    /*00005864*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*00005868*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*0000586c*/ STR         R5, [R4, #4] 
    /*00005870*/ BL          0x0000000000005870          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*00005874*/ LDR         R0, [PC, #44]               @ 0x00000000000058a8 
    /*00005878*/ BL          0x0000000000005878          @ osal_del_timer [R_ARM_CALL: +0x0] 
    /*0000587c*/ LDR         R0, [PC, #36]               @ 0x00000000000058a8 
    /*00005880*/ BL          0x0000000000005880          @ osal_timer_destory [R_ARM_CALL: +0x0] 
    /*00005884*/ MOVW        R0, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*00005888*/ MOVT        R0, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*0000588c*/ BL          0x000000000000588C          @ osal_spin_lock_destory [R_ARM_CALL: +0x0] 
    /*00005890*/ BL          0x0000000000005890          @ DIS_HAL_Close [R_ARM_CALL: .text+0x6250] 
    /*00005894*/ BL          0x0000000000005894          @ DIS_DRV_FreetIrq [R_ARM_CALL: .text+0x6110] 
    /*00005898*/ MOV         R0, R5 
    /*0000589c*/ ADD         SP, SP, #12 
    /*000058a0*/ POP         {R4, R5, PC} 
    /*000058a4*/ LDRDEQ      R0, [R0], -IP 
    /*000058a8*/ ANDEQ       R0, R0, IP, ASR #3 
FUNC_END DIS_DrvCommClose


FUNC_BEGIN DIS_DrvCommExit
    DIS_DrvCommExit_x58ac:
    /*000058ac*/ B           DIS_DrvCommExit_x58ac       @ DIS_DrvCommExitEx [R_ARM_JUMP24: .text+0x4f08] 
FUNC_END DIS_DrvCommExit


FUNC_BEGIN DIS_DrvCommNotify
    /*000058b0*/ PUSH        {R4, R5, R6, LR} 
    /*000058b4*/ MOVW        R4, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1c4] 
    /*000058b8*/ MOVT        R4, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1c4] 
    /*000058bc*/ SUB         SP, SP, #8 
    /*000058c0*/ ADD         R5, R4, #24 
    /*000058c4*/ MOV         R6, R0 
    /*000058c8*/ ADD         R1, SP, #4 
    /*000058cc*/ MOV         R0, R5 
    /*000058d0*/ BL          0x00000000000058D0          @ osal_spin_lock_irqsave [R_ARM_CALL: +0x0] 
    /*000058d4*/ LDR         R3, [R4, #20] 
    /*000058d8*/ MOV         R0, R5 
    /*000058dc*/ ADD         R1, SP, #4 
    /*000058e0*/ STR         R6, [R3, #44]               @ 0x2c 
    /*000058e4*/ BL          0x00000000000058E4          @ osal_spin_unlock_irqrestore [R_ARM_CALL: +0x0] 
    /*000058e8*/ MOV         R0, #0 
    /*000058ec*/ ADD         SP, SP, #8 
    /*000058f0*/ POP         {R4, R5, R6, PC} 
FUNC_END DIS_DrvCommNotify


FUNC_BEGIN DIS_VgsGmeCallback
    /*000058f4*/ PUSH        {R4, R5, R6, R7, LR} 
    /*000058f8*/ MOV         R0, R3 
    /*000058fc*/ SUB         SP, SP, #20 
    /*00005900*/ MOV         R4, R3 
    /*00005904*/ BL          0x0000000000005904          @ DIS_CheckNullPtr [R_ARM_CALL: .text+0x7a0] 
    /*00005908*/ SUBS        R5, R0, #0 
    /*0000590c*/ BNE         DIS_VgsGmeCallback_x593c
    /*00005910*/ ADD         R7, SP, #8 
    /*00005914*/ MOV         R0, SP 
    /*00005918*/ BL          0x0000000000005918          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*0000591c*/ LDR         R1, [R4, #48]               @ 0x30 
    /*00005920*/ LDR         R0, [R4] 
    /*00005924*/ BL          0x0000000000005924          @ DIS_DrvCommListInt [R_ARM_CALL: .text+0x5038] 
    /*00005928*/ MOV         R0, R7 
    /*0000592c*/ BL          0x000000000000592C          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00005930*/ LDM         R7, {R2, R3} 
    /*00005934*/ LDM         SP, {R0, R1} 
    /*00005938*/ BL          0x0000000000005938          @ DIS_DrvCommIntStatistic [R_ARM_CALL: .text+0x4e24] 
    DIS_VgsGmeCallback_x593c:
    /*0000593c*/ MOV         R0, R5 
    /*00005940*/ ADD         SP, SP, #20 
    /*00005944*/ POP         {R4, R5, R6, R7, PC} 
FUNC_END DIS_VgsGmeCallback


FUNC_BEGIN DIS_DRV_SOFTRSTRegInit
    /*00005948*/ BX          LR 
FUNC_END DIS_DRV_SOFTRSTRegInit


FUNC_BEGIN DIS_DRV_SOFTRSTRegExit
    /*0000594c*/ BX          LR 
FUNC_END DIS_DRV_SOFTRSTRegExit


FUNC_BEGIN DIS_DrvCheckSingleYImage
    /*00005950*/ PUSH        {R4, LR} 
    /*00005954*/ SUB         SP, SP, #16 
    /*00005958*/ LDR         R2, [R0] 
    /*0000595c*/ LDR         R1, [R0, #4] 
    /*00005960*/ TST         R2, #1 
    /*00005964*/ BNE         DIS_DrvCheckSingleYImage_x59e8
    /*00005968*/ TST         R1, #1 
    /*0000596c*/ BNE         DIS_DrvCheckSingleYImage_x59e8
    /*00005970*/ LDR         R3, [R0, #44]               @ 0x2c 
    /*00005974*/ CMP         R3, #15 
    /*00005978*/ BLS         DIS_DrvCheckSingleYImage_x5a20
    /*0000597c*/ TST         R3, #15 
    /*00005980*/ BNE         DIS_DrvCheckSingleYImage_x5a20
    /*00005984*/ LDR         IP, [R0, #120]              @ 0x78 
    /*00005988*/ LDR         LR, [R0, #124]              @ 0x7c 
    /*0000598c*/ AND         R3, IP, #1 
    /*00005990*/ ORRS        R4, IP, LR 
    /*00005994*/ ORREQ       R3, R3, #1 
    /*00005998*/ CMP         R3, #0 
    /*0000599c*/ BNE         DIS_DrvCheckSingleYImage_x5a54
    /*000059a0*/ LDR         R3, [R0, #20] 
    /*000059a4*/ CMP         R3, #0 
    /*000059a8*/ BNE         DIS_DrvCheckSingleYImage_x5ac0
    /*000059ac*/ LDR         R3, [R0, #16] 
    /*000059b0*/ CMP         R3, #0 
    /*000059b4*/ BNE         DIS_DrvCheckSingleYImage_x5af4
    /*000059b8*/ LDR         R3, [R0, #12] 
    /*000059bc*/ CMP         R3, #39                     @ 0x27 
    /*000059c0*/ BNE         DIS_DrvCheckSingleYImage_x5a8c
    /*000059c4*/ SUB         R3, R2, #320                @ 0x140 
    /*000059c8*/ CMP         R3, #704                    @ 0x2c0 
    /*000059cc*/ BHI         DIS_DrvCheckSingleYImage_x5b28
    /*000059d0*/ SUB         R3, R1, #180                @ 0xb4 
    /*000059d4*/ CMP         R3, #844                    @ 0x34c 
    /*000059d8*/ MOVLS       R0, #0 
    /*000059dc*/ BHI         DIS_DrvCheckSingleYImage_x5b6c
    DIS_DrvCheckSingleYImage_x59e0:
    /*000059e0*/ ADD         SP, SP, #16 
    /*000059e4*/ POP         {R4, PC} 
    DIS_DrvCheckSingleYImage_x59e8:
    /*000059e8*/ STR         R1, [SP, #8] 
    /*000059ec*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x404] 
    /*000059f0*/ MOV         R1, #70                     @ 0x46 
    /*000059f4*/ STR         R2, [SP, #4] 
    /*000059f8*/ STR         R1, [SP] 
    /*000059fc*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x404] 
    /*00005a00*/ MOVW        R2, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1ec0] 
    /*00005a04*/ MOV         R1, #17 
    /*00005a08*/ MOVT        R2, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1ec0] 
    /*00005a0c*/ MOV         R0, #3 
    /*00005a10*/ BL          0x0000000000005A10          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005a14*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005a18*/ MOVT        R0, #40977                  @ 0xa011 
    /*00005a1c*/ B           DIS_DrvCheckSingleYImage_x59e0
    DIS_DrvCheckSingleYImage_x5a20:
    /*00005a20*/ STR         R3, [SP, #4] 
    /*00005a24*/ MOV         R2, #77                     @ 0x4d 
    /*00005a28*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x404] 
    /*00005a2c*/ STR         R2, [SP] 
    /*00005a30*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x404] 
    /*00005a34*/ MOVW        R2, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1efc] 
    /*00005a38*/ MOV         R1, #17 
    /*00005a3c*/ MOVT        R2, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1efc] 
    /*00005a40*/ MOV         R0, #3 
    /*00005a44*/ BL          0x0000000000005A44          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005a48*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005a4c*/ MOVT        R0, #40977                  @ 0xa011 
    /*00005a50*/ B           DIS_DrvCheckSingleYImage_x59e0
    DIS_DrvCheckSingleYImage_x5a54:
    /*00005a54*/ MOV         R2, #83                     @ 0x53 
    /*00005a58*/ STR         IP, [SP, #8] 
    /*00005a5c*/ STR         R2, [SP] 
    /*00005a60*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x404] 
    /*00005a64*/ STR         LR, [SP, #12] 
    /*00005a68*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x404] 
    /*00005a6c*/ MOVW        R2, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f34] 
    /*00005a70*/ MOV         R1, #17 
    /*00005a74*/ MOVT        R2, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f34] 
    /*00005a78*/ MOV         R0, #3 
    /*00005a7c*/ BL          0x0000000000005A7C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005a80*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005a84*/ MOVT        R0, #40977                  @ 0xa011 
    /*00005a88*/ B           DIS_DrvCheckSingleYImage_x59e0
    DIS_DrvCheckSingleYImage_x5a8c:
    /*00005a8c*/ STR         R3, [SP, #4] 
    /*00005a90*/ MOV         R2, #101                    @ 0x65 
    /*00005a94*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x404] 
    /*00005a98*/ STR         R2, [SP] 
    /*00005a9c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x404] 
    /*00005aa0*/ MOVW        R2, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fe0] 
    /*00005aa4*/ MOV         R1, #17 
    /*00005aa8*/ MOVT        R2, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fe0] 
    /*00005aac*/ MOV         R0, #3 
    /*00005ab0*/ BL          0x0000000000005AB0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005ab4*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005ab8*/ MOVT        R0, #40977                  @ 0xa011 
    /*00005abc*/ B           DIS_DrvCheckSingleYImage_x59e0
    DIS_DrvCheckSingleYImage_x5ac0:
    /*00005ac0*/ STR         R3, [SP, #4] 
    /*00005ac4*/ MOV         R2, #89                     @ 0x59 
    /*00005ac8*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x404] 
    /*00005acc*/ STR         R2, [SP] 
    /*00005ad0*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x404] 
    /*00005ad4*/ MOVW        R2, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1f68] 
    /*00005ad8*/ MOV         R1, #17 
    /*00005adc*/ MOVT        R2, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1f68] 
    /*00005ae0*/ MOV         R0, #3 
    /*00005ae4*/ BL          0x0000000000005AE4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005ae8*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005aec*/ MOVT        R0, #40977                  @ 0xa011 
    /*00005af0*/ B           DIS_DrvCheckSingleYImage_x59e0
    DIS_DrvCheckSingleYImage_x5af4:
    /*00005af4*/ STR         R3, [SP, #4] 
    /*00005af8*/ MOV         R2, #95                     @ 0x5f 
    /*00005afc*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x404] 
    /*00005b00*/ STR         R2, [SP] 
    /*00005b04*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x404] 
    /*00005b08*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x1fa4] 
    /*00005b0c*/ MOV         R1, #17 
    /*00005b10*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x1fa4] 
    /*00005b14*/ MOV         R0, #3 
    /*00005b18*/ BL          0x0000000000005B18          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005b1c*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005b20*/ MOVT        R0, #40977                  @ 0xa011 
    /*00005b24*/ B           DIS_DrvCheckSingleYImage_x59e0
    DIS_DrvCheckSingleYImage_x5b28:
    /*00005b28*/ STR         R2, [SP, #4] 
    /*00005b2c*/ MOV         R3, #320                    @ 0x140 
    /*00005b30*/ MOV         R2, #1024                   @ 0x400 
    /*00005b34*/ STR         R3, [SP, #8] 
    /*00005b38*/ STR         R2, [SP, #12] 
    /*00005b3c*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x404] 
    /*00005b40*/ MOV         R2, #108                    @ 0x6c 
    /*00005b44*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x404] 
    /*00005b48*/ STR         R2, [SP] 
    /*00005b4c*/ MOV         R1, #17 
    /*00005b50*/ MOVW        R2, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x201c] 
    /*00005b54*/ MOV         R0, #3 
    /*00005b58*/ MOVT        R2, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x201c] 
    /*00005b5c*/ BL          0x0000000000005B5C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005b60*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005b64*/ MOVT        R0, #40977                  @ 0xa011 
    /*00005b68*/ B           DIS_DrvCheckSingleYImage_x59e0
    DIS_DrvCheckSingleYImage_x5b6c:
    /*00005b6c*/ STR         R1, [SP, #4] 
    /*00005b70*/ MOV         R2, #1024                   @ 0x400 
    /*00005b74*/ MOV         R3, #180                    @ 0xb4 
    /*00005b78*/ STR         R2, [SP, #12] 
    /*00005b7c*/ STR         R3, [SP, #8] 
    /*00005b80*/ MOV         R2, #114                    @ 0x72 
    /*00005b84*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x404] 
    /*00005b88*/ STR         R2, [SP] 
    /*00005b8c*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x404] 
    /*00005b90*/ MOVW        R2, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2058] 
    /*00005b94*/ MOV         R1, #17 
    /*00005b98*/ MOVT        R2, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2058] 
    /*00005b9c*/ MOV         R0, #3 
    /*00005ba0*/ BL          0x0000000000005BA0          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005ba4*/ MOVW        R0, #32771                  @ 0x8003 
    /*00005ba8*/ MOVT        R0, #40977                  @ 0xa011 
    /*00005bac*/ B           DIS_DrvCheckSingleYImage_x59e0
FUNC_END DIS_DrvCheckSingleYImage


FUNC_BEGIN DIS_DrvProcessVgs
    /*00005bb0*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00005bb4*/ SUB         SP, SP, #92                 @ 0x5c 
    /*00005bb8*/ MOV         R7, R0 
    /*00005bbc*/ MOV         R0, #45                     @ 0x2d 
    /*00005bc0*/ MOV         R9, R1 
    /*00005bc4*/ STR         R2, [SP, #12] 
    /*00005bc8*/ BL          0x0000000000005BC8          @ CMPI_GetModuleFuncById [R_ARM_CALL: +0x0] 
    /*00005bcc*/ SUBS        R4, R0, #0 
    /*00005bd0*/ BEQ         DIS_DrvProcessVgs_x5d88
    /*00005bd4*/ LDR         R3, [R4] 
    /*00005bd8*/ CMP         R3, #0 
    /*00005bdc*/ BEQ         DIS_DrvProcessVgs_x5d88
    /*00005be0*/ LDR         R3, [R4, #32] 
    /*00005be4*/ CMP         R3, #0 
    /*00005be8*/ BEQ         DIS_DrvProcessVgs_x5d88
    /*00005bec*/ LDR         R3, [R4, #12] 
    /*00005bf0*/ CMP         R3, #0 
    /*00005bf4*/ BEQ         DIS_DrvProcessVgs_x5d88
    /*00005bf8*/ MOVW        R5, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1e0] 
    /*00005bfc*/ MOVT        R5, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1e0] 
    /*00005c00*/ MOV         R3, #0 
    /*00005c04*/ STR         R7, [SP] 
    /*00005c08*/ LDR         R1, [R5] 
    /*00005c0c*/ MOV         R0, R7 
    /*00005c10*/ STR         R3, [SP, #52]               @ 0x34 
    /*00005c14*/ MOVW        R2, #0                      @ DIS_VgsGmeCallback [R_ARM_MOVW_ABS_NC: .text+0x58f4] 
    /*00005c18*/ MOV         R8, #1 
    /*00005c1c*/ MOVT        R2, #0                      @ DIS_VgsGmeCallback [R_ARM_MOVT_ABS: .text+0x58f4] 
    /*00005c20*/ STR         R1, [SP, #48]               @ 0x30 
    /*00005c24*/ ASR         R1, R7, #31 
    /*00005c28*/ STR         R2, [SP, #80]               @ 0x50 
    /*00005c2c*/ ADD         R2, SP, #24 
    /*00005c30*/ STRD        R0, [SP, #24] 
    /*00005c34*/ MOV         FP, R1 
    /*00005c38*/ STR         R2, [SP, #4] 
    /*00005c3c*/ MOV         R1, R3 
    /*00005c40*/ STR         R8, [SP, #76]               @ 0x4c 
    /*00005c44*/ MOV         R2, #17 
    /*00005c48*/ LDR         R6, [R4] 
    /*00005c4c*/ ADD         R0, SP, #20 
    /*00005c50*/ BLX         R6 
    /*00005c54*/ MOV         SL, R7 
    /*00005c58*/ SUBS        R6, R0, #0 
    /*00005c5c*/ BNE         DIS_DrvProcessVgs_x5d1c
    /*00005c60*/ LDR         R3, [R4, #64]               @ 0x40 
    /*00005c64*/ BLX         R3 
    /*00005c68*/ CMP         R0, #0 
    /*00005c6c*/ BEQ         DIS_DrvProcessVgs_x5d68
    /*00005c70*/ ADD         R3, R0, #672                @ 0x2a0 
    /*00005c74*/ MOV         R2, R5 
    /*00005c78*/ MOV         R1, R0 
    /*00005c7c*/ STRD        SL, [R3] 
    /*00005c80*/ MOV         R3, #17 
    /*00005c84*/ STR         R6, [R0, #728]              @ 0x2d8 
    /*00005c88*/ STR         R6, [R0, #720]              @ 0x2d0 
    /*00005c8c*/ STR         R7, [R0, #732]              @ 0x2dc 
    /*00005c90*/ STR         R3, [R0, #724]              @ 0x2d4 
    /*00005c94*/ LDR         IP, [R9, #8] 
    /*00005c98*/ LDR         R3, [R4, #48]               @ 0x30 
    /*00005c9c*/ LDR         R0, [SP, #20] 
    /*00005ca0*/ STR         IP, [R2, #4]! 
    /*00005ca4*/ BLX         R3 
    /*00005ca8*/ SUBS        R6, R0, #0 
    /*00005cac*/ BNE         DIS_DrvProcessVgs_x5ce0
    /*00005cb0*/ MOV         R2, R6 
    /*00005cb4*/ LDR         R3, [R4, #12] 
    /*00005cb8*/ MOV         R1, R8 
    /*00005cbc*/ LDR         R0, [SP, #20] 
    /*00005cc0*/ BLX         R3 
    /*00005cc4*/ SUBS        R6, R0, #0 
    /*00005cc8*/ BNE         DIS_DrvProcessVgs_x5d4c
    /*00005ccc*/ ADD         R0, R9, #40                 @ 0x28 
    /*00005cd0*/ BL          0x0000000000005CD0          @ osal_gettimeofday [R_ARM_CALL: +0x0] 
    /*00005cd4*/ MOV         R0, R6 
    /*00005cd8*/ ADD         SP, SP, #92                 @ 0x5c 
    /*00005cdc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_DrvProcessVgs_x5ce0:
    /*00005ce0*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2128] 
    /*00005ce4*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2128] 
    /*00005ce8*/ MOV         R1, #17 
    /*00005cec*/ MOV         R3, #194                    @ 0xc2 
    /*00005cf0*/ STR         R7, [SP, #4] 
    /*00005cf4*/ STR         R3, [SP] 
    DIS_DrvProcessVgs_x5cf8:
    /*00005cf8*/ LDR         R3, [PC, #180]              @ 0x0000000000005db4 
    DIS_DrvProcessVgs_x5cfc:
    /*00005cfc*/ MOV         R0, #3 
    /*00005d00*/ BL          0x0000000000005D00          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005d04*/ LDR         R3, [SP, #20] 
    /*00005d08*/ LDR         R2, [SP, #12] 
    /*00005d0c*/ STR         R3, [R2] 
    DIS_DrvProcessVgs_x5d10:
    /*00005d10*/ MOV         R0, R6 
    /*00005d14*/ ADD         SP, SP, #92                 @ 0x5c 
    /*00005d18*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_DrvProcessVgs_x5d1c:
    /*00005d1c*/ STR         R7, [SP, #4] 
    /*00005d20*/ MOV         R3, #171                    @ 0xab 
    /*00005d24*/ MOV         R1, #17 
    /*00005d28*/ STR         R3, [SP] 
    /*00005d2c*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x20c4] 
    /*00005d30*/ LDR         R3, [PC, #124]              @ 0x0000000000005db4 
    /*00005d34*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x20c4] 
    /*00005d38*/ MOV         R0, #3 
    /*00005d3c*/ BL          0x0000000000005D3C          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005d40*/ MOV         R0, R6 
    /*00005d44*/ ADD         SP, SP, #92                 @ 0x5c 
    /*00005d48*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_DrvProcessVgs_x5d4c:
    /*00005d4c*/ MOV         R3, #203                    @ 0xcb 
    /*00005d50*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2160] 
    /*00005d54*/ STR         R7, [SP, #4] 
    /*00005d58*/ MOV         R1, #17 
    /*00005d5c*/ STR         R3, [SP] 
    /*00005d60*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2160] 
    /*00005d64*/ B           DIS_DrvProcessVgs_x5cf8
    DIS_DrvProcessVgs_x5d68:
    /*00005d68*/ MOV         R3, #178                    @ 0xb2 
    /*00005d6c*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x20f8] 
    /*00005d70*/ STR         R3, [SP] 
    /*00005d74*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x20f8] 
    /*00005d78*/ STR         R7, [SP, #4] 
    /*00005d7c*/ MOV         R1, #17 
    /*00005d80*/ LDR         R3, [PC, #44]               @ 0x0000000000005db4 
    /*00005d84*/ B           DIS_DrvProcessVgs_x5cfc
    DIS_DrvProcessVgs_x5d88:
    /*00005d88*/ MOV         R3, #157                    @ 0x9d 
    /*00005d8c*/ MOVW        R2, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2090] 
    /*00005d90*/ STR         R3, [SP] 
    /*00005d94*/ MOVT        R2, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2090] 
    /*00005d98*/ LDR         R3, [PC, #20]               @ 0x0000000000005db4 
    /*00005d9c*/ MOV         R1, #17 
    /*00005da0*/ MOV         R0, #3 
    /*00005da4*/ MOVW        R6, #32784                  @ 0x8010 
    /*00005da8*/ BL          0x0000000000005DA8          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00005dac*/ MOVT        R6, #40977                  @ 0xa011 
    /*00005db0*/ B           DIS_DrvProcessVgs_x5d10
    /*00005db4*/ ANDEQ       R0, R0, R0, LSR #8 
FUNC_END DIS_DrvProcessVgs


FUNC_BEGIN DIS_DrvSetGmeMotion
    /*00005db8*/ MOV         R3, R1 
    /*00005dbc*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00005dc0*/ SUB         SP, SP, #52                 @ 0x34 
    /*00005dc4*/ LDRD        R6, [R3] 
    /*00005dc8*/ LDRD        R4, [R3, #8] 
    /*00005dcc*/ MOV         R1, SP 
    /*00005dd0*/ LDRD        R8, [R3, #16] 
    /*00005dd4*/ STRD        R6, [SP] 
    /*00005dd8*/ STRD        R4, [SP, #8] 
    /*00005ddc*/ STRD        R8, [SP, #16] 
    /*00005de0*/ LDRD        R4, [R3, #24] 
    /*00005de4*/ LDRD        R6, [R3, #32] 
    /*00005de8*/ LDRD        R8, [R3, #40]               @ 0x28 
    /*00005dec*/ STRD        R4, [SP, #24] 
    /*00005df0*/ STRD        R6, [SP, #32] 
    /*00005df4*/ STRD        R8, [SP, #40]               @ 0x28 
    /*00005df8*/ BL          0x0000000000005DF8          @ DisSetGmeMotion [R_ARM_CALL: .text+0x7f98] 
    /*00005dfc*/ MOV         R0, #0 
    /*00005e00*/ ADD         SP, SP, #52                 @ 0x34 
    /*00005e04*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
FUNC_END DIS_DrvSetGmeMotion


FUNC_BEGIN DIS_DrvSetConfig
    DIS_DrvSetConfig_x5e08:
    /*00005e08*/ B           DIS_DrvSetConfig_x5e08      @ hiisp_dis_SetConfig [R_ARM_JUMP24: .text+0xc6c8] 
FUNC_END DIS_DrvSetConfig


FUNC_BEGIN DIS_DrvCalcFirmwareAttr
    /*00005e0c*/ PUSH        {R4, R5, R6, LR} 
    /*00005e10*/ SUB         SP, SP, #352                @ 0x160 
    /*00005e14*/ MOV         R5, R0 
    /*00005e18*/ MOV         R6, R1 
    /*00005e1c*/ MOV         R4, R2 
    /*00005e20*/ MOV         R0, SP 
    /*00005e24*/ MOV         R2, #352                    @ 0x160 
    /*00005e28*/ MOV         R1, #0 
    /*00005e2c*/ BL          0x0000000000005E2C          @ memset [R_ARM_CALL: +0x0] 
    /*00005e30*/ MOV         R2, SP 
    /*00005e34*/ MOV         R1, R6 
    /*00005e38*/ MOV         R0, R5 
    /*00005e3c*/ BL          0x0000000000005E3C          @ hiisp_dis_SetConfig [R_ARM_CALL: .text+0xc6c8] 
    /*00005e40*/ MOV         R1, R4 
    /*00005e44*/ MOV         R0, SP 
    /*00005e48*/ BL          0x0000000000005E48          @ hiisp_dis_CalcFirmwareAttr [R_ARM_CALL: .text+0xcaa0] 
    /*00005e4c*/ ADD         SP, SP, #352                @ 0x160 
    /*00005e50*/ POP         {R4, R5, R6, PC} 
FUNC_END DIS_DrvCalcFirmwareAttr


FUNC_BEGIN DIS_DrvCalcDynamicAttr
    DIS_DrvCalcDynamicAttr_x5e54:
    /*00005e54*/ B           DIS_DrvCalcDynamicAttr_x5e54@ hiisp_dis_setDynamicAttr [R_ARM_JUMP24: .text+0x75c0] 
FUNC_END DIS_DrvCalcDynamicAttr


FUNC_BEGIN DIS_DrvAlgorithmInit
    DIS_DrvAlgorithmInit_x5e58:
    /*00005e58*/ B           DIS_DrvAlgorithmInit_x5e58  @ hiisp_dis_init [R_ARM_JUMP24: .text+0x7740] 
FUNC_END DIS_DrvAlgorithmInit


FUNC_BEGIN DIS_DrvGetDSFConfig
    DIS_DrvGetDSFConfig_x5e5c:
    /*00005e5c*/ B           DIS_DrvGetDSFConfig_x5e5c   @ hiisp_get_dsf_hw_cfg [R_ARM_JUMP24: .text+0x7f44] 
FUNC_END DIS_DrvGetDSFConfig


FUNC_BEGIN DIS_DrvAlgorithmDeInit
    DIS_DrvAlgorithmDeInit_x5e60:
    /*00005e60*/ B           DIS_DrvAlgorithmDeInit_x5e60@ hiisp_dis_uninit [R_ARM_JUMP24: .text+0x7edc] 
FUNC_END DIS_DrvAlgorithmDeInit


FUNC_BEGIN DIS_DrvSetUnitMotionData
    DIS_DrvSetUnitMotionData_x5e64:
    /*00005e64*/ B           DIS_DrvSetUnitMotionData_x5e64@ hiisp_dis_setUnitMotionData [R_ARM_JUMP24: .text+0x75f4] 
FUNC_END DIS_DrvSetUnitMotionData


FUNC_BEGIN DIS_DrvGetStabilizedMotion
    DIS_DrvGetStabilizedMotion_x5e68:
    /*00005e68*/ B           DIS_DrvGetStabilizedMotion_x5e68@ DisGetStabilizedMotion [R_ARM_JUMP24: .text+0x806c] 
FUNC_END DIS_DrvGetStabilizedMotion


FUNC_BEGIN DIS_DrvGetFpdCfg
    DIS_DrvGetFpdCfg_x5e6c:
    /*00005e6c*/ B           DIS_DrvGetFpdCfg_x5e6c      @ hiisp_get_fpd_hw_cfg [R_ARM_JUMP24: .text+0x7f28] 
FUNC_END DIS_DrvGetFpdCfg


FUNC_BEGIN DIS_DrvFpdMerge
    /*00005e70*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00005e74*/ MOV         R8, R0 
    /*00005e78*/ SUB         SP, SP, #16 
    /*00005e7c*/ MOV         R0, R1 
    /*00005e80*/ MOV         R4, R1 
    /*00005e84*/ MOV         R6, R2 
    /*00005e88*/ MOV         R7, R3 
    /*00005e8c*/ BL          0x0000000000005E8C          @ DIS_CheckNullPtr [R_ARM_CALL: .text+0x7a0] 
    /*00005e90*/ SUBS        R5, R0, #0 
    /*00005e94*/ BNE         DIS_DrvFpdMerge_x5f00
    /*00005e98*/ MOVW        R2, #0                      @ stDISFwCfg [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*00005e9c*/ MOVT        R2, #0                      @ stDISFwCfg [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*00005ea0*/ MOV         IP, R7 
    /*00005ea4*/ MOV         R3, #1344                   @ 0x540 
    /*00005ea8*/ MLA         R3, R3, R8, R2 
    /*00005eac*/ LDR         R1, [IP], #16 
    /*00005eb0*/ MOV         R2, R6 
    /*00005eb4*/ MOV         R0, R8 
    /*00005eb8*/ LDR         R8, [R2], #16 
    /*00005ebc*/ STR         R1, [SP, #4] 
    /*00005ec0*/ LDR         R1, [SP, #40]               @ 0x28 
    /*00005ec4*/ STR         SP, [R3, #48]               @ 0x30 
    /*00005ec8*/ STR         R2, [SP, #8] 
    /*00005ecc*/ STR         R1, [R3, #60]               @ 0x3c 
    /*00005ed0*/ ADD         R1, SP, #8 
    /*00005ed4*/ STR         R1, [R3, #52]               @ 0x34 
    /*00005ed8*/ MOV         R1, R3 
    /*00005edc*/ LDR         LR, [R4, #1232]             @ 0x4d0 
    /*00005ee0*/ STR         IP, [SP] 
    /*00005ee4*/ STR         R8, [SP, #12] 
    /*00005ee8*/ STR         LR, [R3, #56]               @ 0x38 
    /*00005eec*/ BL          0x0000000000005EEC          @ hiisp_dis_mergePtsVecs_fw [R_ARM_CALL: .text+0xd91c] 
    /*00005ef0*/ LDR         R2, [SP, #4] 
    /*00005ef4*/ LDR         R3, [SP, #12] 
    /*00005ef8*/ STR         R2, [R7] 
    /*00005efc*/ STR         R3, [R6] 
    DIS_DrvFpdMerge_x5f00:
    /*00005f00*/ MOV         R0, R5 
    /*00005f04*/ ADD         SP, SP, #16 
    /*00005f08*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END DIS_DrvFpdMerge


FUNC_BEGIN DIS_DrvGetGMEStatus
    /*00005f0c*/ LDR         IP, [R0, #1176]             @ 0x498 
    /*00005f10*/ PUSH        {R4, R5, LR} 
    /*00005f14*/ MOV         R1, IP 
    /*00005f18*/ LDR         R3, [R1], #16 
    /*00005f1c*/ UBFX        LR, R3, #0, #9 
    /*00005f20*/ UBFX        R5, R3, #20, #1 
    /*00005f24*/ STR         R1, [R2, #32] 
    /*00005f28*/ UBFX        R4, R3, #19, #1 
    /*00005f2c*/ STR         LR, [R2] 
    /*00005f30*/ UBFX        R1, R3, #10, #9 
    /*00005f34*/ UBFX        LR, R3, #9, #1 
    /*00005f38*/ STR         R5, [R2, #28] 
    /*00005f3c*/ STR         R4, [R2, #24] 
    /*00005f40*/ UBFX        R3, R3, #21, #9 
    /*00005f44*/ STR         LR, [R2, #20] 
    /*00005f48*/ STR         R1, [R2, #8] 
    /*00005f4c*/ STR         R3, [R2, #12] 
    /*00005f50*/ LDR         R3, [IP, #4] 
    /*00005f54*/ STR         R3, [R2, #16] 
    /*00005f58*/ LDR         R3, [R0, #1192]             @ 0x4a8 
    /*00005f5c*/ STR         R3, [R2, #36]               @ 0x24 
    /*00005f60*/ POP         {R4, R5, PC} 
FUNC_END DIS_DrvGetGMEStatus


FUNC_BEGIN DIS_DrvSetGmeRansacSeed
    /*00005f64*/ MOVW        R2, #0                      @ stDISFwCfg [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*00005f68*/ MOVT        R2, #0                      @ stDISFwCfg [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*00005f6c*/ MOV         R3, #1344                   @ 0x540 
    /*00005f70*/ MLA         R0, R3, R0, R2 
    /*00005f74*/ STR         R1, [R0, #324]              @ 0x144 
    /*00005f78*/ BX          LR 
FUNC_END DIS_DrvSetGmeRansacSeed


FUNC_BEGIN DIS_DrvInitGmeRansacSeed
    /*00005f7c*/ MOVW        R2, #0                      @ stDISFwCfg [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*00005f80*/ MOVT        R2, #0                      @ stDISFwCfg [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*00005f84*/ MOV         R3, #1344                   @ 0x540 
    /*00005f88*/ MLA         R0, R3, R0, R2 
    /*00005f8c*/ LDR         R3, [R0, #324]              @ 0x144 
    /*00005f90*/ STR         R3, [R1, #1216]             @ 0x4c0 
    /*00005f94*/ BX          LR 
FUNC_END DIS_DrvInitGmeRansacSeed


FUNC_BEGIN DIS_DrvCalinlierOverlapRatio
    /*00005f98*/ PUSH        {R4, R5, R6, LR} 
    /*00005f9c*/ MOV         R4, #1344                   @ 0x540 
    /*00005fa0*/ MUL         R5, R4, R0 
    /*00005fa4*/ MOV         R6, R1 
    /*00005fa8*/ MOV         R0, R1 
    /*00005fac*/ MOVW        R4, #0                      @ stDISFwCfg [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*00005fb0*/ MOVT        R4, #0                      @ stDISFwCfg [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*00005fb4*/ ADD         R1, R5, #64                 @ 0x40 
    /*00005fb8*/ ADD         R1, R4, R1 
    /*00005fbc*/ ADD         R4, R4, R5 
    /*00005fc0*/ BL          0x0000000000005FC0          @ hiisp_dis_inlierOverlapRatio_fw [R_ARM_CALL: .text+0xe560] 
    /*00005fc4*/ LDR         R3, [R6, #16] 
    /*00005fc8*/ STR         R3, [R4, #324]              @ 0x144 
    /*00005fcc*/ POP         {R4, R5, R6, PC} 
FUNC_END DIS_DrvCalinlierOverlapRatio


FUNC_BEGIN DIS_DrvConvertHwNodeInfo
    /*00005fd0*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00005fd4*/ SUB         SP, SP, #88                 @ 0x58 
    /*00005fd8*/ MOV         R6, R1 
    /*00005fdc*/ MOV         R7, R0 
    /*00005fe0*/ MOV         R1, #0 
    /*00005fe4*/ MOV         R5, R2 
    /*00005fe8*/ ADD         R0, SP, #24 
    /*00005fec*/ MOV         R2, #32 
    /*00005ff0*/ MOV         R4, R3 
    /*00005ff4*/ BL          0x0000000000005FF4          @ memset [R_ARM_CALL: +0x0] 
    /*00005ff8*/ ADD         R0, SP, #56                 @ 0x38 
    /*00005ffc*/ MOV         R2, #32 
    /*00006000*/ MOV         R1, #0 
    /*00006004*/ ADD         SL, SP, #24 
    /*00006008*/ BL          0x0000000000006008          @ memset [R_ARM_CALL: +0x0] 
    /*0000600c*/ LDR         R6, [R6, #8] 
    /*00006010*/ ADD         R9, SP, #56                 @ 0x38 
    /*00006014*/ ADD         R8, SP, #8 
    /*00006018*/ MOV         IP, #848                    @ 0x350 
    /*0000601c*/ MOV         R3, #0 
    /*00006020*/ STR         R3, [SP, #12] 
    /*00006024*/ STR         R3, [SP, #8] 
    /*00006028*/ STR         R3, [SP, #16] 
    /*0000602c*/ STR         R3, [SP, #20] 
    DIS_DrvConvertHwNodeInfo_x6030:
    /*00006030*/ MOV         LR, R4 
    /*00006034*/ LDRD        R2, [R5, IP] 
    /*00006038*/ LDRD        R0, [LR, IP]! 
    /*0000603c*/ ADD         IP, IP, #24 
    /*00006040*/ CMP         IP, #944                    @ 0x3b0 
    /*00006044*/ STRD        R2, [R9], #8 
    /*00006048*/ LDR         LR, [LR, #16] 
    /*0000604c*/ STRD        R0, [SL], #8 
    /*00006050*/ STR         LR, [R8], #4 
    /*00006054*/ BNE         DIS_DrvConvertHwNodeInfo_x6030
    /*00006058*/ LDR         R3, [PC, #132]              @ 0x00000000000060e4 
    /*0000605c*/ MOV         R0, #1344                   @ 0x540 
    /*00006060*/ MOV         R1, R6 
    /*00006064*/ MLA         R0, R0, R7, R3 
    /*00006068*/ BL          0x0000000000006068          @ DIS_HAL_SetGmeCfg [R_ARM_CALL: .text+0x6278] 
    /*0000606c*/ MOV         R3, R6 
    /*00006070*/ ADD         R2, SP, #8 
    /*00006074*/ ADD         R1, SP, #24 
    /*00006078*/ ADD         R0, SP, #56                 @ 0x38 
    /*0000607c*/ BL          0x000000000000607C          @ DIS_HAL_SetPyramidAddr [R_ARM_CALL: .text+0x6520] 
    /*00006080*/ ADD         R3, R4, #1168               @ 0x490 
    /*00006084*/ MOV         R2, R6 
    /*00006088*/ LDRD        R0, [R3] 
    /*0000608c*/ BL          0x000000000000608C          @ DIS_HAL_SetDebugAddr [R_ARM_CALL: .text+0x6514] 
    /*00006090*/ ADD         R3, R4, #944                @ 0x3b0 
    /*00006094*/ MOV         R2, R6 
    /*00006098*/ LDRD        R0, [R3] 
    /*0000609c*/ BL          0x000000000000609C          @ DIS_HAL_SetMotionDataAddr [R_ARM_CALL: .text+0x6508] 
    /*000060a0*/ ADD         R3, R4, #1136               @ 0x470 
    /*000060a4*/ ADD         R1, R5, #1136               @ 0x470 
    /*000060a8*/ ADD         R5, R5, #1168               @ 0x490 
    /*000060ac*/ STR         R6, [SP] 
    /*000060b0*/ ADD         R4, R4, #1184               @ 0x4a0 
    /*000060b4*/ LDRD        R2, [R3] 
    /*000060b8*/ LDRD        R0, [R1] 
    /*000060bc*/ BL          0x00000000000060BC          @ DIS_HAL_SetFpdAddr [R_ARM_CALL: .text+0x64d8] 
    /*000060c0*/ STR         R6, [SP] 
    /*000060c4*/ LDRD        R0, [R5] 
    /*000060c8*/ LDRD        R2, [R4] 
    /*000060cc*/ ADDS        R0, R0, #16 
    /*000060d0*/ ADC         R1, R1, #0 
    /*000060d4*/ BL          0x00000000000060D4          @ DIS_HAL_SetInlierIndexAddr [R_ARM_CALL: .text+0x64f0] 
    /*000060d8*/ MOV         R0, #0 
    /*000060dc*/ ADD         SP, SP, #88                 @ 0x58 
    /*000060e0*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    /*000060e4*/ ANDEQ       R0, R0, IP, LSL #2          @ stDISFwCfg [R_ARM_ABS32: .bss+0x1f8] 
FUNC_END DIS_DrvConvertHwNodeInfo


FUNC_BEGIN DIS_DRV_IsWrongInt
    /*000060e8*/ MOV         R0, #0 
    /*000060ec*/ BX          LR 
FUNC_END DIS_DRV_IsWrongInt


FUNC_BEGIN DIS_DRV_ClkEnable
    /*000060f0*/ MOV         R0, #0 
    /*000060f4*/ BX          LR 
FUNC_END DIS_DRV_ClkEnable


FUNC_BEGIN DIS_DRV_ResetSelect
    /*000060f8*/ MOV         R0, #0 
    /*000060fc*/ BX          LR 
FUNC_END DIS_DRV_ResetSelect


FUNC_BEGIN DIS_DRV_Reset
    /*00006100*/ MOV         R0, #0 
    /*00006104*/ BX          LR 
FUNC_END DIS_DRV_Reset


FUNC_BEGIN DIS_DRV_RequestIrq
    /*00006108*/ MOV         R0, #0 
    /*0000610c*/ BX          LR 
FUNC_END DIS_DRV_RequestIrq


FUNC_BEGIN DIS_DRV_FreetIrq
    /*00006110*/ MOV         R0, #0 
    /*00006114*/ BX          LR 
FUNC_END DIS_DRV_FreetIrq


FUNC_BEGIN DIS_DRV_PerformaceLimit
    /*00006118*/ MOVW        R3, #0                      @ reg_otp_base_va [R_ARM_MOVW_ABS_NC: +0x0] 
    /*0000611c*/ MOVT        R3, #0                      @ reg_otp_base_va [R_ARM_MOVT_ABS: +0x0] 
    /*00006120*/ LDR         R3, [R3] 
    /*00006124*/ LDRB        R3, [R3, #40]               @ 0x28 
    /*00006128*/ SUB         R2, R3, #1 
    /*0000612c*/ CMP         R2, #1 
    /*00006130*/ BLS         DIS_DRV_PerformaceLimit_x6174
    /*00006134*/ SUB         R2, R3, #5 
    /*00006138*/ CMP         R2, #1 
    /*0000613c*/ BLS         DIS_DRV_PerformaceLimit_x6190
    /*00006140*/ CMP         R3, #12 
    /*00006144*/ BHI         DIS_DRV_PerformaceLimit_x6174
    /*00006148*/ MOVW        R2, #4609                   @ 0x1201 
    /*0000614c*/ MVN         R3, R2, LSR R3 
    /*00006150*/ TST         R3, #1 
    /*00006154*/ BNE         DIS_DRV_PerformaceLimit_x6174
    /*00006158*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1e0] 
    /*0000615c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1e0] 
    /*00006160*/ MOV         R1, #3840                   @ 0xf00 
    /*00006164*/ MOV         R2, #2160                   @ 0x870 
    /*00006168*/ STR         R1, [R3, #8] 
    /*0000616c*/ STR         R2, [R3, #12] 
    /*00006170*/ BX          LR 
    DIS_DRV_PerformaceLimit_x6174:
    /*00006174*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1e0] 
    /*00006178*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1e0] 
    /*0000617c*/ MOV         R1, #2304                   @ 0x900 
    /*00006180*/ MOV         R2, #1296                   @ 0x510 
    /*00006184*/ STR         R1, [R3, #8] 
    /*00006188*/ STR         R2, [R3, #12] 
    /*0000618c*/ BX          LR 
    DIS_DRV_PerformaceLimit_x6190:
    /*00006190*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1e0] 
    /*00006194*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1e0] 
    /*00006198*/ MOV         R1, #2688                   @ 0xa80 
    /*0000619c*/ MOVW        R2, #1944                   @ 0x798 
    /*000061a0*/ STR         R1, [R3, #8] 
    /*000061a4*/ STR         R2, [R3, #12] 
    /*000061a8*/ BX          LR 
FUNC_END DIS_DRV_PerformaceLimit


FUNC_BEGIN DIS_DRV_CheckCommWH
    /*000061ac*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .bss+0x1e0] 
    /*000061b0*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .bss+0x1e0] 
    /*000061b4*/ LDR         R2, [R3, #8] 
    /*000061b8*/ LDR         IP, [R3, #12] 
    /*000061bc*/ CMP         R2, R0 
    /*000061c0*/ BCC         DIS_DRV_CheckCommWH_x61ec
    /*000061c4*/ CMP         IP, R1 
    /*000061c8*/ BCC         DIS_DRV_CheckCommWH_x61ec
    /*000061cc*/ CMP         R1, #720                    @ 0x2d0 
    /*000061d0*/ CMPCS       R0, #1280                   @ 0x500 
    /*000061d4*/ BCC         DIS_DRV_CheckCommWH_x61ec
    /*000061d8*/ ORR         R3, R0, R1 
    /*000061dc*/ ANDS        R3, R3, #1 
    /*000061e0*/ BNE         DIS_DRV_CheckCommWH_x61ec
    /*000061e4*/ MOV         R0, R3 
    /*000061e8*/ BX          LR 
    DIS_DRV_CheckCommWH_x61ec:
    /*000061ec*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000061f0*/ SUB         SP, SP, #36                 @ 0x24 
    /*000061f4*/ MOV         LR, #720                    @ 0x2d0 
    /*000061f8*/ MOV         R3, #556                    @ 0x22c 
    /*000061fc*/ STR         IP, [SP, #24] 
    /*00006200*/ MOV         IP, #1280                   @ 0x500 
    /*00006204*/ STR         R2, [SP, #16] 
    /*00006208*/ MOVW        R2, #0                      @ .LC13 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2194] 
    /*0000620c*/ STR         R1, [SP, #8] 
    /*00006210*/ MOVT        R2, #0                      @ .LC13 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2194] 
    /*00006214*/ STR         R0, [SP, #4] 
    /*00006218*/ MOV         R1, #17 
    /*0000621c*/ STR         R3, [SP] 
    /*00006220*/ MOV         R0, #3 
    /*00006224*/ STR         LR, [SP, #20] 
    /*00006228*/ STR         IP, [SP, #12] 
    /*0000622c*/ LDR         R3, [PC, #16]               @ 0x0000000000006244 
    /*00006230*/ BL          0x0000000000006230          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*00006234*/ MOVW        R0, #32771                  @ 0x8003 
    /*00006238*/ MOVT        R0, #40977                  @ 0xa011 
    /*0000623c*/ ADD         SP, SP, #36                 @ 0x24 
    /*00006240*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00006244*/ ANDEQ       R0, R0, R4, LSR R4 
FUNC_END DIS_DRV_CheckCommWH


FUNC_BEGIN DIS_HAL_Open
    /*00006248*/ MOV         R0, #0 
    /*0000624c*/ BX          LR 
FUNC_END DIS_HAL_Open


FUNC_BEGIN DIS_HAL_Close
    /*00006250*/ MOV         R0, #0 
    /*00006254*/ BX          LR 
FUNC_END DIS_HAL_Close


FUNC_BEGIN DIS_HAL_SetNodeID
    /*00006258*/ BX          LR 
FUNC_END DIS_HAL_SetNodeID


FUNC_BEGIN DIS_HAL_SetMask
    /*0000625c*/ BX          LR 
FUNC_END DIS_HAL_SetMask


FUNC_BEGIN DIS_HAL_SetNext
    /*00006260*/ BX          LR 
FUNC_END DIS_HAL_SetNext


FUNC_BEGIN DIS_HAL_Start
    /*00006264*/ BX          LR 
FUNC_END DIS_HAL_Start


FUNC_BEGIN DIS_HAL_GetIntStatus
    /*00006268*/ BX          LR 
FUNC_END DIS_HAL_GetIntStatus


FUNC_BEGIN DIS_HAL_ClrInt
    /*0000626c*/ BX          LR 
FUNC_END DIS_HAL_ClrInt


FUNC_BEGIN DIS_HAL_GetNodeID
    /*00006270*/ BX          LR 
FUNC_END DIS_HAL_GetNodeID


FUNC_BEGIN DIS_HAL_SetOutstanding
    /*00006274*/ BX          LR 
FUNC_END DIS_HAL_SetOutstanding


FUNC_BEGIN DIS_HAL_SetGmeCfg
    /*00006278*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000627c*/ LDR         R2, [R0, #36]               @ 0x24 
    /*00006280*/ LDR         R3, [R1] 
    /*00006284*/ LDR         IP, [R0, #4] 
    /*00006288*/ SUBS        R2, R2, #8 
    /*0000628c*/ BFI         R3, IP, #0, #1 
    /*00006290*/ MOVNE       R2, #1 
    /*00006294*/ BFI         R3, R2, #1, #1 
    /*00006298*/ STR         R3, [R1] 
    /*0000629c*/ LDR         IP, [R0, #12] 
    /*000062a0*/ LDR         R3, [R1, #4] 
    /*000062a4*/ LDR         R2, [R0, #16] 
    /*000062a8*/ SUB         IP, IP, #1 
    /*000062ac*/ BFI         R3, IP, #0, #13 
    /*000062b0*/ SUB         R2, R2, #1 
    /*000062b4*/ BFI         R3, R2, #16, #13 
    /*000062b8*/ STR         R3, [R1, #4] 
    /*000062bc*/ LDR         IP, [R0, #20] 
    /*000062c0*/ LDR         R3, [R1, #8] 
    /*000062c4*/ LDR         R2, [R0, #24] 
    /*000062c8*/ SUB         IP, IP, #1 
    /*000062cc*/ BFI         R3, IP, #0, #13 
    /*000062d0*/ SUB         R2, R2, #1 
    /*000062d4*/ BFI         R3, R2, #16, #13 
    /*000062d8*/ STR         R3, [R1, #8] 
    /*000062dc*/ LDR         R3, [R1, #12] 
    /*000062e0*/ LDR         R2, [R0, #56]               @ 0x38 
    /*000062e4*/ BFI         R3, R2, #0, #31 
    /*000062e8*/ STR         R3, [R1, #12] 
    /*000062ec*/ LDR         R3, [R1, #16] 
    /*000062f0*/ LDR         IP, [R0, #28] 
    /*000062f4*/ LDRB        R2, [R0, #32] 
    /*000062f8*/ BFI         R3, IP, #0, #1 
    /*000062fc*/ BFI         R3, R2, #1, #3 
    /*00006300*/ STR         R3, [R1, #16] 
    /*00006304*/ LDR         R3, [R1, #20] 
    /*00006308*/ LDR         IP, [R0, #44]               @ 0x2c 
    /*0000630c*/ LDRB        R2, [R0, #42]               @ 0x2a 
    /*00006310*/ BFI         R3, IP, #0, #8 
    /*00006314*/ BFI         R3, R2, #8, #8 
    /*00006318*/ STR         R3, [R1, #20] 
    /*0000631c*/ LDRH        R2, [R0, #40]               @ 0x28 
    /*00006320*/ LDR         R3, [R1, #24] 
    /*00006324*/ BFI         R3, R2, #0, #6 
    /*00006328*/ STR         R3, [R1, #24] 
    /*0000632c*/ LDRB        R2, [R0, #153]              @ 0x99 
    /*00006330*/ LDR         R3, [R1, #28] 
    /*00006334*/ LDRB        IP, [R0, #152]              @ 0x98 
    /*00006338*/ BFI         R3, R2, #0, #4 
    /*0000633c*/ LDRB        R2, [R0, #132]              @ 0x84 
    /*00006340*/ BFI         R3, IP, #4, #4 
    /*00006344*/ BFI         R3, R2, #8, #4 
    /*00006348*/ STR         R3, [R1, #28] 
    /*0000634c*/ LDR         R3, [R1, #32] 
    /*00006350*/ LDR         R2, [R0, #148]              @ 0x94 
    /*00006354*/ BFI         R3, R2, #0, #21 
    /*00006358*/ STR         R3, [R1, #32] 
    /*0000635c*/ LDR         R3, [R1, #36]               @ 0x24 
    /*00006360*/ LDR         IP, [R0, #136]              @ 0x88 
    /*00006364*/ LDR         R2, [R0, #140]              @ 0x8c 
    /*00006368*/ BFI         R3, IP, #0, #13 
    /*0000636c*/ BFI         R3, R2, #16, #13 
    /*00006370*/ STR         R3, [R1, #36]               @ 0x24 
    /*00006374*/ LDR         R3, [R1, #40]               @ 0x28 
    /*00006378*/ LDRH        IP, [R0, #64]               @ 0x40 
    /*0000637c*/ LDRH        R2, [R0, #66]               @ 0x42 
    /*00006380*/ BFI         R3, IP, #0, #10 
    /*00006384*/ LDRB        IP, [R0, #62]               @ 0x3e 
    /*00006388*/ LDRB        LR, [R0, #68]               @ 0x44 
    /*0000638c*/ BFI         R3, R2, #10, #4 
    /*00006390*/ SUBS        IP, IP, #6 
    /*00006394*/ LDRH        R2, [R0, #60]               @ 0x3c 
    /*00006398*/ BFI         R3, LR, #14, #8 
    /*0000639c*/ MOVNE       IP, #1 
    /*000063a0*/ SUBS        R2, R2, #6 
    /*000063a4*/ BFI         R3, IP, #30, #1 
    /*000063a8*/ MOVNE       R2, #1 
    /*000063ac*/ BFI         R3, R2, #31, #1 
    /*000063b0*/ STR         R3, [R1, #40]               @ 0x28 
    /*000063b4*/ LDR         R3, [R1, #44]               @ 0x2c 
    /*000063b8*/ LDR         R2, [R0, #76]               @ 0x4c 
    /*000063bc*/ BFI         R3, R2, #0, #30 
    /*000063c0*/ STR         R3, [R1, #44]               @ 0x2c 
    /*000063c4*/ LDR         R3, [R1, #48]               @ 0x30 
    /*000063c8*/ LDR         IP, [R0, #84]               @ 0x54 
    /*000063cc*/ LDR         R2, [R0, #80]               @ 0x50 
    /*000063d0*/ BFI         R3, IP, #0, #10 
    /*000063d4*/ BFI         R3, R2, #16, #7 
    /*000063d8*/ STR         R3, [R1, #48]               @ 0x30 
    /*000063dc*/ LDRH        IP, [R0, #122]              @ 0x7a 
    /*000063e0*/ LDR         R3, [R1, #52]               @ 0x34 
    /*000063e4*/ LDRH        R2, [R0, #124]              @ 0x7c 
    /*000063e8*/ BFI         R3, IP, #0, #11 
    /*000063ec*/ LDRB        IP, [R0, #121]              @ 0x79 
    /*000063f0*/ BFI         R3, R2, #11, #11 
    /*000063f4*/ LDRB        R2, [R0, #120]              @ 0x78 
    /*000063f8*/ BFI         R3, IP, #22, #5 
    /*000063fc*/ BFI         R3, R2, #27, #5 
    /*00006400*/ STR         R3, [R1, #52]               @ 0x34 
    /*00006404*/ LDR         R2, [R0, #160]              @ 0xa0 
    /*00006408*/ LDR         R3, [R1, #56]               @ 0x38 
    /*0000640c*/ LDR         IP, [R0, #164]              @ 0xa4 
    /*00006410*/ BFI         R3, R2, #0, #10 
    /*00006414*/ LDRB        R2, [R0, #201]              @ 0xc9 
    /*00006418*/ BFI         R3, IP, #10, #10 
    /*0000641c*/ LDRB        IP, [R0, #200]              @ 0xc8 
    /*00006420*/ BFI         R3, R2, #20, #4 
    /*00006424*/ LDR         R2, [R0, #8] 
    /*00006428*/ BFI         R3, IP, #24, #3 
    /*0000642c*/ BFI         R3, R2, #31, #1 
    /*00006430*/ STR         R3, [R1, #56]               @ 0x38 
    /*00006434*/ LDR         IP, [R0, #168]              @ 0xa8 
    /*00006438*/ LDR         R3, [R1, #60]               @ 0x3c 
    /*0000643c*/ LDR         R2, [R0, #172]              @ 0xac 
    /*00006440*/ SUB         IP, IP, #1 
    /*00006444*/ BFI         R3, IP, #0, #11 
    /*00006448*/ SUB         R2, R2, #1 
    /*0000644c*/ BFI         R3, R2, #16, #11 
    /*00006450*/ STR         R3, [R1, #60]               @ 0x3c 
    /*00006454*/ LDR         R3, [R1, #64]               @ 0x40 
    /*00006458*/ LDR         IP, [R0, #176]              @ 0xb0 
    /*0000645c*/ LDR         R2, [R0, #180]              @ 0xb4 
    /*00006460*/ BFI         R3, IP, #0, #11 
    /*00006464*/ BFI         R3, R2, #16, #11 
    /*00006468*/ STR         R3, [R1, #64]               @ 0x40 
    /*0000646c*/ LDR         R3, [R1, #68]               @ 0x44 
    /*00006470*/ LDR         IP, [R0, #184]              @ 0xb8 
    /*00006474*/ LDR         R2, [R0, #188]              @ 0xbc 
    /*00006478*/ BFI         R3, IP, #0, #11 
    /*0000647c*/ BFI         R3, R2, #16, #11 
    /*00006480*/ STR         R3, [R1, #68]               @ 0x44 
    /*00006484*/ LDR         R3, [R1, #72]               @ 0x48 
    /*00006488*/ LDR         IP, [R0, #192]              @ 0xc0 
    /*0000648c*/ LDR         R2, [R0, #196]              @ 0xc4 
    /*00006490*/ BFI         R3, IP, #0, #7 
    /*00006494*/ BFI         R3, R2, #7, #7 
    /*00006498*/ STR         R3, [R1, #72]               @ 0x48 
    /*0000649c*/ LDRH        IP, [R0, #48]               @ 0x30 
    /*000064a0*/ LDR         R3, [R1, #76]               @ 0x4c 
    /*000064a4*/ LDRH        R2, [R0, #50]               @ 0x32 
    /*000064a8*/ BFI         R3, IP, #0, #11 
    /*000064ac*/ BFI         R3, R2, #16, #11 
    /*000064b0*/ STR         R3, [R1, #76]               @ 0x4c 
    /*000064b4*/ LDRH        IP, [R0, #52]               @ 0x34 
    /*000064b8*/ LDRH        R2, [R0, #54]               @ 0x36 
    /*000064bc*/ LDR         R3, [R1, #80]               @ 0x50 
    /*000064c0*/ SUB         R0, IP, #1 
    /*000064c4*/ SUB         R2, R2, #1 
    /*000064c8*/ BFI         R3, R0, #0, #11 
    /*000064cc*/ BFI         R3, R2, #16, #11 
    /*000064d0*/ STR         R3, [R1, #80]               @ 0x50 
    /*000064d4*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END DIS_HAL_SetGmeCfg


FUNC_BEGIN DIS_HAL_SetFpdAddr
    /*000064d8*/ LDR         IP, [SP] 
    /*000064dc*/ STR         R1, [IP, #164]              @ 0xa4 
    /*000064e0*/ STR         R0, [IP, #168]              @ 0xa8 
    /*000064e4*/ STR         R3, [IP, #180]              @ 0xb4 
    /*000064e8*/ STR         R2, [IP, #184]              @ 0xb8 
    /*000064ec*/ BX          LR 
FUNC_END DIS_HAL_SetFpdAddr


FUNC_BEGIN DIS_HAL_SetInlierIndexAddr
    /*000064f0*/ LDR         IP, [SP] 
    /*000064f4*/ STR         R1, [IP, #172]              @ 0xac 
    /*000064f8*/ STR         R0, [IP, #176]              @ 0xb0 
    /*000064fc*/ STR         R3, [IP, #204]              @ 0xcc 
    /*00006500*/ STR         R2, [IP, #208]              @ 0xd0 
    /*00006504*/ BX          LR 
FUNC_END DIS_HAL_SetInlierIndexAddr


FUNC_BEGIN DIS_HAL_SetMotionDataAddr
    /*00006508*/ STR         R1, [R2, #188]              @ 0xbc 
    /*0000650c*/ STR         R0, [R2, #192]              @ 0xc0 
    /*00006510*/ BX          LR 
FUNC_END DIS_HAL_SetMotionDataAddr


FUNC_BEGIN DIS_HAL_SetDebugAddr
    /*00006514*/ STR         R1, [R2, #196]              @ 0xc4 
    /*00006518*/ STR         R0, [R2, #200]              @ 0xc8 
    /*0000651c*/ BX          LR 
FUNC_END DIS_HAL_SetDebugAddr


FUNC_BEGIN DIS_HAL_SetPyramidAddr
    /*00006520*/ LDR         IP, [R1, #4] 
    /*00006524*/ STR         IP, [R3, #116]              @ 0x74 
    /*00006528*/ LDR         IP, [R1] 
    /*0000652c*/ STR         IP, [R3, #120]              @ 0x78 
    /*00006530*/ LDR         IP, [R1, #12] 
    /*00006534*/ STR         IP, [R3, #124]              @ 0x7c 
    /*00006538*/ LDR         IP, [R1, #8] 
    /*0000653c*/ STR         IP, [R3, #128]              @ 0x80 
    /*00006540*/ LDR         IP, [R1, #20] 
    /*00006544*/ STR         IP, [R3, #132]              @ 0x84 
    /*00006548*/ LDR         IP, [R1, #16] 
    /*0000654c*/ STR         IP, [R3, #136]              @ 0x88 
    /*00006550*/ LDR         IP, [R1, #28] 
    /*00006554*/ STR         IP, [R3, #140]              @ 0x8c 
    /*00006558*/ LDR         R1, [R1, #24] 
    /*0000655c*/ STR         R1, [R3, #144]              @ 0x90 
    /*00006560*/ LDR         R1, [R0, #4] 
    /*00006564*/ STR         R1, [R3, #84]               @ 0x54 
    /*00006568*/ LDR         R1, [R0] 
    /*0000656c*/ STR         R1, [R3, #88]               @ 0x58 
    /*00006570*/ LDR         R1, [R0, #12] 
    /*00006574*/ STR         R1, [R3, #92]               @ 0x5c 
    /*00006578*/ LDR         R1, [R0, #8] 
    /*0000657c*/ STR         R1, [R3, #96]               @ 0x60 
    /*00006580*/ LDR         R1, [R0, #20] 
    /*00006584*/ STR         R1, [R3, #100]              @ 0x64 
    /*00006588*/ LDR         R1, [R0, #16] 
    /*0000658c*/ STR         R1, [R3, #104]              @ 0x68 
    /*00006590*/ LDR         R1, [R0, #28] 
    /*00006594*/ STR         R1, [R3, #108]              @ 0x6c 
    /*00006598*/ LDR         R1, [R0, #24] 
    /*0000659c*/ STR         R1, [R3, #112]              @ 0x70 
    /*000065a0*/ LDR         R1, [R3, #148]              @ 0x94 
    /*000065a4*/ LDR         R0, [R2] 
    /*000065a8*/ BFI         R1, R0, #0, #16 
    /*000065ac*/ STR         R1, [R3, #148]              @ 0x94 
    /*000065b0*/ LDR         R1, [R3, #152]              @ 0x98 
    /*000065b4*/ LDR         R0, [R2, #4] 
    /*000065b8*/ BFI         R1, R0, #0, #16 
    /*000065bc*/ STR         R1, [R3, #152]              @ 0x98 
    /*000065c0*/ LDR         R1, [R3, #156]              @ 0x9c 
    /*000065c4*/ LDR         R0, [R2, #8] 
    /*000065c8*/ BFI         R1, R0, #0, #16 
    /*000065cc*/ STR         R1, [R3, #156]              @ 0x9c 
    /*000065d0*/ LDR         R1, [R2, #12] 
    /*000065d4*/ LDR         R2, [R3, #160]              @ 0xa0 
    /*000065d8*/ BFI         R2, R1, #0, #16 
    /*000065dc*/ STR         R2, [R3, #160]              @ 0xa0 
    /*000065e0*/ BX          LR 
FUNC_END DIS_HAL_SetPyramidAddr


FUNC_BEGIN DIS_HAL_PrintRegStatus
    /*000065e4*/ BX          LR 
FUNC_END DIS_HAL_PrintRegStatus


FUNC_BEGIN DIS_MotionModel_VEC_popfront
    /*000065e8*/ LDR         R3, [R0, #8] 
    /*000065ec*/ CMP         R3, #0 
    /*000065f0*/ BXEQ        LR 
    /*000065f4*/ LDR         R2, [R0, #12] 
    /*000065f8*/ SUB         R3, R3, #1 
    /*000065fc*/ LDR         R1, [R0, #4] 
    /*00006600*/ ADD         R2, R2, #1 
    /*00006604*/ STR         R3, [R0, #8] 
    /*00006608*/ CMP         R2, R1 
    /*0000660c*/ STR         R2, [R0, #12] 
    /*00006610*/ MOVCS       R3, #0 
    /*00006614*/ STRCS       R3, [R0, #12] 
    /*00006618*/ BX          LR 
FUNC_END DIS_MotionModel_VEC_popfront


FUNC_BEGIN DIS_MotionModel_VEC_popback
    /*0000661c*/ LDR         R3, [R0, #8] 
    /*00006620*/ CMP         R3, #0 
    /*00006624*/ BXEQ        LR 
    /*00006628*/ LDR         R2, [R0, #16] 
    /*0000662c*/ SUB         R3, R3, #1 
    /*00006630*/ STR         R3, [R0, #8] 
    /*00006634*/ CMP         R2, #0 
    /*00006638*/ SUBNE       R2, R2, #1 
    /*0000663c*/ STRNE       R2, [R0, #16] 
    /*00006640*/ LDREQ       R3, [R0, #4] 
    /*00006644*/ SUBEQ       R3, R3, #1 
    /*00006648*/ STREQ       R3, [R0, #16] 
    /*0000664c*/ BX          LR 
FUNC_END DIS_MotionModel_VEC_popback


FUNC_BEGIN DIS_MotionModel_VEC_clearVector
    /*00006650*/ MOV         R3, #0 
    /*00006654*/ STR         R3, [R0, #8] 
    /*00006658*/ STR         R3, [R0, #16] 
    /*0000665c*/ STR         R3, [R0, #12] 
    /*00006660*/ BX          LR 
FUNC_END DIS_MotionModel_VEC_clearVector


FUNC_BEGIN DIS_MotionModel_VEC_getElement
    /*00006664*/ PUSH        {R4, R5, R6, LR} 
    /*00006668*/ MOV         R3, R1 
    /*0000666c*/ LDR         R1, [R0, #4] 
    /*00006670*/ MOV         R5, R0 
    /*00006674*/ MOV         R4, R2 
    /*00006678*/ LDR         R0, [R0, #12] 
    /*0000667c*/ CMP         R1, #0 
    /*00006680*/ BEQ         DIS_MotionModel_VEC_getElement_x6690
    /*00006684*/ ADD         R0, R0, R3 
    /*00006688*/ BL          0x0000000000006688          @ __aeabi_uidivmod [R_ARM_CALL: +0x0] 
    /*0000668c*/ LSL         R1, R1, #2 
    DIS_MotionModel_VEC_getElement_x6690:
    /*00006690*/ LDR         R3, [R5] 
    /*00006694*/ LDR         R3, [R3, R1] 
    /*00006698*/ LDRD        R0, [R3] 
    /*0000669c*/ STRD        R0, [R4] 
    /*000066a0*/ LDRD        R0, [R3, #8] 
    /*000066a4*/ STRD        R0, [R4, #8] 
    /*000066a8*/ LDRD        R0, [R3, #16] 
    /*000066ac*/ STRD        R0, [R4, #16] 
    /*000066b0*/ LDRD        R0, [R3, #24] 
    /*000066b4*/ STRD        R0, [R4, #24] 
    /*000066b8*/ LDRD        R0, [R3, #32] 
    /*000066bc*/ STRD        R0, [R4, #32] 
    /*000066c0*/ LDRD        R2, [R3, #40]               @ 0x28 
    /*000066c4*/ STRD        R2, [R4, #40]               @ 0x28 
    /*000066c8*/ POP         {R4, R5, R6, PC} 
FUNC_END DIS_MotionModel_VEC_getElement


FUNC_BEGIN DIS_MotionModel_VEC_getFirstElement
    /*000066cc*/ LDR         R2, [R0, #12] 
    /*000066d0*/ LDR         R3, [R0] 
    /*000066d4*/ PUSH        {R4, R5} 
    /*000066d8*/ LDR         R3, [R3, R2, LSL #2] 
    /*000066dc*/ LDRD        R4, [R3] 
    /*000066e0*/ STRD        R4, [R1] 
    /*000066e4*/ LDRD        R4, [R3, #8] 
    /*000066e8*/ STRD        R4, [R1, #8] 
    /*000066ec*/ LDRD        R4, [R3, #16] 
    /*000066f0*/ STRD        R4, [R1, #16] 
    /*000066f4*/ LDRD        R4, [R3, #24] 
    /*000066f8*/ STRD        R4, [R1, #24] 
    /*000066fc*/ LDRD        R4, [R3, #32] 
    /*00006700*/ STRD        R4, [R1, #32] 
    /*00006704*/ LDRD        R2, [R3, #40]               @ 0x28 
    /*00006708*/ POP         {R4, R5} 
    /*0000670c*/ STRD        R2, [R1, #40]               @ 0x28 
    /*00006710*/ BX          LR 
FUNC_END DIS_MotionModel_VEC_getFirstElement


FUNC_BEGIN DIS_MotionModel_VEC_getLastElement
    /*00006714*/ PUSH        {R4, R5} 
    /*00006718*/ LDR         R3, [R0, #16] 
    /*0000671c*/ LDR         R2, [R0, #4] 
    /*00006720*/ CMP         R3, #0 
    /*00006724*/ LDR         R0, [R0] 
    /*00006728*/ SUBEQ       R3, R2, #1 
    /*0000672c*/ SUBNE       R3, R3, #1 
    /*00006730*/ CMP         R3, R2 
    /*00006734*/ MOVCS       R3, R2 
    /*00006738*/ LDR         R3, [R0, R3, LSL #2] 
    /*0000673c*/ LDRD        R4, [R3] 
    /*00006740*/ STRD        R4, [R1] 
    /*00006744*/ LDRD        R4, [R3, #8] 
    /*00006748*/ STRD        R4, [R1, #8] 
    /*0000674c*/ LDRD        R4, [R3, #16] 
    /*00006750*/ STRD        R4, [R1, #16] 
    /*00006754*/ LDRD        R4, [R3, #24] 
    /*00006758*/ STRD        R4, [R1, #24] 
    /*0000675c*/ LDRD        R4, [R3, #32] 
    /*00006760*/ STRD        R4, [R1, #32] 
    /*00006764*/ LDRD        R2, [R3, #40]               @ 0x28 
    /*00006768*/ POP         {R4, R5} 
    /*0000676c*/ STRD        R2, [R1, #40]               @ 0x28 
    /*00006770*/ BX          LR 
FUNC_END DIS_MotionModel_VEC_getLastElement


FUNC_BEGIN HIISP_CQ_VECTOR_pop_front
    /*00006774*/ LDR         R3, [R0, #20] 
    /*00006778*/ CMP         R3, #0 
    /*0000677c*/ BXEQ        LR 
    /*00006780*/ LDR         R2, [R0, #24] 
    /*00006784*/ SUB         R3, R3, #1 
    /*00006788*/ LDR         R1, [R0, #16] 
    /*0000678c*/ ADD         R2, R2, #1 
    /*00006790*/ STR         R3, [R0, #20] 
    /*00006794*/ CMP         R2, R1 
    /*00006798*/ STR         R2, [R0, #24] 
    /*0000679c*/ MOVCS       R3, #0 
    /*000067a0*/ STRCS       R3, [R0, #24] 
    /*000067a4*/ BX          LR 
FUNC_END HIISP_CQ_VECTOR_pop_front


FUNC_BEGIN HIISP_CQ_VECTOR_pop_back
    /*000067a8*/ LDR         R3, [R0, #20] 
    /*000067ac*/ CMP         R3, #0 
    /*000067b0*/ BXEQ        LR 
    /*000067b4*/ LDR         R2, [R0, #28] 
    /*000067b8*/ SUB         R3, R3, #1 
    /*000067bc*/ STR         R3, [R0, #20] 
    /*000067c0*/ CMP         R2, #0 
    /*000067c4*/ SUBNE       R2, R2, #1 
    /*000067c8*/ STRNE       R2, [R0, #28] 
    /*000067cc*/ LDREQ       R3, [R0, #16] 
    /*000067d0*/ SUBEQ       R3, R3, #1 
    /*000067d4*/ STREQ       R3, [R0, #28] 
    /*000067d8*/ BX          LR 
FUNC_END HIISP_CQ_VECTOR_pop_back


FUNC_BEGIN HIISP_CQ_VECTOR_clear
    /*000067dc*/ MOV         R3, #0 
    /*000067e0*/ STR         R3, [R0, #20] 
    /*000067e4*/ STR         R3, [R0, #24] 
    /*000067e8*/ STR         R3, [R0, #28] 
    /*000067ec*/ BX          LR 
FUNC_END HIISP_CQ_VECTOR_clear


FUNC_BEGIN DIS_MotionModel_VEC_pushback
    /*000067f0*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000067f4*/ SUB         SP, SP, #12 
    /*000067f8*/ LDMIB       R0, {R2, R3} 
    /*000067fc*/ ADD         R3, R3, #1 
    /*00006800*/ CMP         R3, R2 
    /*00006804*/ BHI         DIS_MotionModel_VEC_pushback_x6874
    /*00006808*/ LDR         LR, [R0, #16] 
    /*0000680c*/ LDR         IP, [R0] 
    /*00006810*/ LDRD        R2, [R1] 
    /*00006814*/ LDR         IP, [IP, LR, LSL #2] 
    /*00006818*/ STRD        R2, [IP] 
    /*0000681c*/ LDRD        R2, [R1, #8] 
    /*00006820*/ STRD        R2, [IP, #8] 
    /*00006824*/ LDRD        R2, [R1, #16] 
    /*00006828*/ STRD        R2, [IP, #16] 
    /*0000682c*/ LDRD        R2, [R1, #24] 
    /*00006830*/ STRD        R2, [IP, #24] 
    /*00006834*/ LDRD        R2, [R1, #32] 
    /*00006838*/ STRD        R2, [IP, #32] 
    /*0000683c*/ LDRD        R2, [R1, #40]               @ 0x28 
    /*00006840*/ STRD        R2, [IP, #40]               @ 0x28 
    /*00006844*/ LDR         R3, [R0, #16] 
    /*00006848*/ LDR         R1, [R0, #4] 
    /*0000684c*/ ADD         R3, R3, #1 
    /*00006850*/ LDR         R2, [R0, #8] 
    /*00006854*/ CMP         R3, R1 
    /*00006858*/ STR         R3, [R0, #16] 
    /*0000685c*/ ADD         R3, R2, #1 
    /*00006860*/ STR         R3, [R0, #8] 
    /*00006864*/ MOVCS       R3, #0 
    /*00006868*/ STRCS       R3, [R0, #16] 
    /*0000686c*/ ADD         SP, SP, #12 
    /*00006870*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    DIS_MotionModel_VEC_pushback_x6874:
    /*00006874*/ MOVW        R3, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x22a8] 
    /*00006878*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x448] 
    /*0000687c*/ MOVT        R3, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x22a8] 
    /*00006880*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x448] 
    /*00006884*/ STR         R3, [SP] 
    /*00006888*/ MOVW        R1, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21e4] 
    /*0000688c*/ MOV         R3, #150                    @ 0x96 
    /*00006890*/ MOVT        R1, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21e4] 
    /*00006894*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2250] 
    /*00006898*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2250] 
    /*0000689c*/ BL          0x000000000000689C          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000068a0*/ ADD         SP, SP, #12 
    /*000068a4*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END DIS_MotionModel_VEC_pushback


FUNC_BEGIN HIISP_CQ_VECTOR_get_element
    /*000068a8*/ PUSH        {R4, R5, R6, LR} 
    /*000068ac*/ MOV         R3, R0 
    /*000068b0*/ LDR         R0, [R0, #24] 
    /*000068b4*/ MOV         R5, R2 
    /*000068b8*/ LDR         R6, [R3, #4] 
    /*000068bc*/ ADD         R0, R1, R0 
    /*000068c0*/ LDR         R1, [R3, #16] 
    /*000068c4*/ LDR         R4, [R3, #32] 
    /*000068c8*/ BL          0x00000000000068C8          @ __aeabi_uidivmod [R_ARM_CALL: +0x0] 
    /*000068cc*/ MOV         R2, R6 
    /*000068d0*/ MOV         R0, R5 
    /*000068d4*/ MLA         R1, R6, R1, R4 
    /*000068d8*/ POP         {R4, R5, R6, LR} 
    HIISP_CQ_VECTOR_get_element_x68dc:
    /*000068dc*/ B           HIISP_CQ_VECTOR_get_element_x68dc@ osal_memcpy [R_ARM_JUMP24: +0x0] 
FUNC_END HIISP_CQ_VECTOR_get_element


FUNC_BEGIN HIISP_CQ_VECTOR_push_back
    /*000068e0*/ PUSH        {R4, LR} 
    /*000068e4*/ SUB         SP, SP, #8 
    /*000068e8*/ LDR         R3, [R0, #20] 
    /*000068ec*/ LDR         R2, [R0, #16] 
    /*000068f0*/ ADD         R3, R3, #1 
    /*000068f4*/ CMP         R3, R2 
    /*000068f8*/ BHI         HIISP_CQ_VECTOR_push_back_x6944
    /*000068fc*/ MOV         R4, R0 
    /*00006900*/ LDR         R3, [R0, #32] 
    /*00006904*/ LDR         R2, [R0, #4] 
    /*00006908*/ LDR         R0, [R0, #28] 
    /*0000690c*/ MLA         R0, R0, R2, R3 
    /*00006910*/ BL          0x0000000000006910          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00006914*/ LDR         R3, [R4, #28] 
    /*00006918*/ LDR         R1, [R4, #16] 
    /*0000691c*/ ADD         R3, R3, #1 
    /*00006920*/ LDR         R2, [R4, #20] 
    /*00006924*/ CMP         R3, R1 
    /*00006928*/ STR         R3, [R4, #28] 
    /*0000692c*/ ADD         R3, R2, #1 
    /*00006930*/ STR         R3, [R4, #20] 
    /*00006934*/ MOVCS       R3, #0 
    /*00006938*/ STRCS       R3, [R4, #28] 
    /*0000693c*/ ADD         SP, SP, #8 
    /*00006940*/ POP         {R4, PC} 
    HIISP_CQ_VECTOR_push_back_x6944:
    /*00006944*/ LDR         R2, [PC, #40]               @ 0x0000000000006974 
    /*00006948*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x22d4] 
    /*0000694c*/ MOVW        R1, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21e4] 
    /*00006950*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x22d4] 
    /*00006954*/ MOVT        R1, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21e4] 
    /*00006958*/ STR         R3, [SP] 
    /*0000695c*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2250] 
    /*00006960*/ MOVW        R3, #317                    @ 0x13d 
    /*00006964*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2250] 
    /*00006968*/ BL          0x0000000000006968          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000696c*/ ADD         SP, SP, #8 
    /*00006970*/ POP         {R4, PC} 
    /*00006974*/ ANDEQ       R0, R0, R8, ROR #8 
FUNC_END HIISP_CQ_VECTOR_push_back


FUNC_BEGIN DIS_ptsVec_clearVector
    /*00006978*/ MOV         R3, #0 
    /*0000697c*/ STR         R3, [R0, #4] 
    /*00006980*/ BX          LR 
FUNC_END DIS_ptsVec_clearVector


FUNC_BEGIN DIS_ptsVec_getElement
    /*00006984*/ LDR         R3, [R0, #4] 
    /*00006988*/ CMP         R3, R1 
    /*0000698c*/ BLS         DIS_ptsVec_getElement_x69b0
    /*00006990*/ LDR         R3, [R0] 
    /*00006994*/ LDR         R3, [R3, R1, LSL #3] 
    /*00006998*/ STR         R3, [R2] 
    /*0000699c*/ LDR         R3, [R0] 
    /*000069a0*/ ADD         R1, R3, R1, LSL #3 
    /*000069a4*/ LDR         R3, [R1, #4] 
    /*000069a8*/ STR         R3, [R2, #4] 
    /*000069ac*/ BX          LR 
    DIS_ptsVec_getElement_x69b0:
    /*000069b0*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*000069b4*/ SUB         SP, SP, #12 
    /*000069b8*/ LDR         R2, [PC, #40]               @ 0x00000000000069e8 
    /*000069bc*/ MOVW        R3, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2300] 
    /*000069c0*/ MOVW        R1, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21e4] 
    /*000069c4*/ MOVT        R3, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2300] 
    /*000069c8*/ MOVT        R1, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21e4] 
    /*000069cc*/ STR         R3, [SP] 
    /*000069d0*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2250] 
    /*000069d4*/ MOV         R3, #29 
    /*000069d8*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2250] 
    /*000069dc*/ BL          0x00000000000069DC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000069e0*/ ADD         SP, SP, #12 
    /*000069e4*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*000069e8*/ ANDEQ       R0, R0, R4, LSL #9 
FUNC_END DIS_ptsVec_getElement


FUNC_BEGIN DIS_ptsVec_appendElement
    /*000069ec*/ LDR         R3, [R0, #4] 
    /*000069f0*/ ADD         R2, R3, #1 
    /*000069f4*/ CMP         R2, #300                    @ 0x12c 
    /*000069f8*/ BHI         DIS_ptsVec_appendElement_x6a28
    /*000069fc*/ LDR         IP, [R1] 
    /*00006a00*/ LDR         R2, [R0] 
    /*00006a04*/ STR         IP, [R2, R3, LSL #3] 
    /*00006a08*/ LDR         R2, [R0] 
    /*00006a0c*/ LDR         R1, [R1, #4] 
    /*00006a10*/ ADD         R3, R2, R3, LSL #3 
    /*00006a14*/ STR         R1, [R3, #4] 
    /*00006a18*/ LDR         R3, [R0, #4] 
    /*00006a1c*/ ADD         R3, R3, #1 
    /*00006a20*/ STR         R3, [R0, #4] 
    /*00006a24*/ BX          LR 
    DIS_ptsVec_appendElement_x6a28:
    /*00006a28*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00006a2c*/ SUB         SP, SP, #12 
    /*00006a30*/ LDR         R2, [PC, #40]               @ 0x0000000000006a60 
    /*00006a34*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2320] 
    /*00006a38*/ MOVW        R1, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21e4] 
    /*00006a3c*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2320] 
    /*00006a40*/ MOVT        R1, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21e4] 
    /*00006a44*/ STR         R3, [SP] 
    /*00006a48*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2250] 
    /*00006a4c*/ MOV         R3, #38                     @ 0x26 
    /*00006a50*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2250] 
    /*00006a54*/ BL          0x0000000000006A54          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00006a58*/ ADD         SP, SP, #12 
    /*00006a5c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00006a60*/ MULEQ       R0, IP, R4 
FUNC_END DIS_ptsVec_appendElement


FUNC_BEGIN DIS_ptsVec_replaceElement
    /*00006a64*/ LDR         R3, [R0, #4] 
    /*00006a68*/ CMP         R3, R1 
    /*00006a6c*/ BLS         DIS_ptsVec_replaceElement_x6a90
    /*00006a70*/ LDR         IP, [R2] 
    /*00006a74*/ LDR         R3, [R0] 
    /*00006a78*/ STR         IP, [R3, R1, LSL #3] 
    /*00006a7c*/ LDR         R3, [R0] 
    /*00006a80*/ LDR         R2, [R2, #4] 
    /*00006a84*/ ADD         R1, R3, R1, LSL #3 
    /*00006a88*/ STR         R2, [R1, #4] 
    /*00006a8c*/ BX          LR 
    DIS_ptsVec_replaceElement_x6a90:
    /*00006a90*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00006a94*/ SUB         SP, SP, #12 
    /*00006a98*/ LDR         R2, [PC, #40]               @ 0x0000000000006ac8 
    /*00006a9c*/ MOVW        R3, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2300] 
    /*00006aa0*/ MOVW        R1, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21e4] 
    /*00006aa4*/ MOVT        R3, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2300] 
    /*00006aa8*/ MOVT        R1, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21e4] 
    /*00006aac*/ STR         R3, [SP] 
    /*00006ab0*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2250] 
    /*00006ab4*/ MOV         R3, #47                     @ 0x2f 
    /*00006ab8*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2250] 
    /*00006abc*/ BL          0x0000000000006ABC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00006ac0*/ ADD         SP, SP, #12 
    /*00006ac4*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00006ac8*/ @           <UNDEFINED> instruction: 0x000004b8 
FUNC_END DIS_ptsVec_replaceElement


FUNC_BEGIN DIS_ptsVec_copyVector
    /*00006acc*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00006ad0*/ SUB         SP, SP, #12 
    /*00006ad4*/ LDR         R3, [R0, #4] 
    /*00006ad8*/ CMP         R3, #300                    @ 0x12c 
    /*00006adc*/ BHI         DIS_ptsVec_copyVector_x6b3c
    /*00006ae0*/ MOV         R3, #0 
    /*00006ae4*/ STR         R3, [R1, #4] 
    /*00006ae8*/ LDR         R2, [R0, #4] 
    /*00006aec*/ CMP         R2, R3 
    /*00006af0*/ BEQ         DIS_ptsVec_copyVector_x6b30
    DIS_ptsVec_copyVector_x6af4:
    /*00006af4*/ LDR         IP, [R0] 
    /*00006af8*/ LSL         LR, R3, #3 
    /*00006afc*/ LDR         R2, [R1] 
    /*00006b00*/ LDR         IP, [IP, R3, LSL #3] 
    /*00006b04*/ STR         IP, [R2, R3, LSL #3] 
    /*00006b08*/ ADD         R3, R3, #1 
    /*00006b0c*/ LDR         IP, [R0] 
    /*00006b10*/ LDR         R2, [R1] 
    /*00006b14*/ ADD         IP, IP, LR 
    /*00006b18*/ ADD         R2, R2, LR 
    /*00006b1c*/ LDR         IP, [IP, #4] 
    /*00006b20*/ STR         IP, [R2, #4] 
    /*00006b24*/ LDR         R2, [R0, #4] 
    /*00006b28*/ CMP         R2, R3 
    /*00006b2c*/ BHI         DIS_ptsVec_copyVector_x6af4
    DIS_ptsVec_copyVector_x6b30:
    /*00006b30*/ STR         R2, [R1, #4] 
    /*00006b34*/ ADD         SP, SP, #12 
    /*00006b38*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    DIS_ptsVec_copyVector_x6b3c:
    /*00006b3c*/ LDR         R2, [PC, #40]               @ 0x0000000000006b6c 
    /*00006b40*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2340] 
    /*00006b44*/ MOVW        R1, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x21e4] 
    /*00006b48*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2340] 
    /*00006b4c*/ MOVT        R1, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x21e4] 
    /*00006b50*/ STR         R3, [SP] 
    /*00006b54*/ MOVW        R0, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2250] 
    /*00006b58*/ MOV         R3, #57                     @ 0x39 
    /*00006b5c*/ MOVT        R0, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2250] 
    /*00006b60*/ BL          0x0000000000006B60          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00006b64*/ ADD         SP, SP, #12 
    /*00006b68*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*00006b6c*/ LDRDEQ      R0, [R0], -R4 
FUNC_END DIS_ptsVec_copyVector


FUNC_BEGIN DIS_resetMotionModel
    /*00006b70*/ MOV         R2, #0 
    /*00006b74*/ MOV         R3, #0 
    /*00006b78*/ PUSH        {R4, R5} 
    /*00006b7c*/ MOV         R4, #1048576                @ 0x100000 
    /*00006b80*/ MOV         R5, #0 
    /*00006b84*/ STRD        R2, [R0, #8] 
    /*00006b88*/ STRD        R4, [R0] 
    /*00006b8c*/ STRD        R4, [R0, #32] 
    /*00006b90*/ STRD        R2, [R0, #16] 
    /*00006b94*/ STRD        R2, [R0, #24] 
    /*00006b98*/ STRD        R2, [R0, #40]               @ 0x28 
    /*00006b9c*/ POP         {R4, R5} 
    /*00006ba0*/ BX          LR 
FUNC_END DIS_resetMotionModel


FUNC_BEGIN DIS_copyMotionModel
    /*00006ba4*/ LDRD        R2, [R1] 
    /*00006ba8*/ STRD        R2, [R0] 
    /*00006bac*/ LDRD        R2, [R1, #8] 
    /*00006bb0*/ STRD        R2, [R0, #8] 
    /*00006bb4*/ LDRD        R2, [R1, #16] 
    /*00006bb8*/ STRD        R2, [R0, #16] 
    /*00006bbc*/ LDRD        R2, [R1, #24] 
    /*00006bc0*/ STRD        R2, [R0, #24] 
    /*00006bc4*/ LDRD        R2, [R1, #32] 
    /*00006bc8*/ STRD        R2, [R0, #32] 
    /*00006bcc*/ LDRD        R2, [R1, #40]               @ 0x28 
    /*00006bd0*/ STRD        R2, [R0, #40]               @ 0x28 
    /*00006bd4*/ BX          LR 
FUNC_END DIS_copyMotionModel


FUNC_BEGIN motionModel2array
    /*00006bd8*/ LDRD        R2, [R0] 
    /*00006bdc*/ STRD        R2, [R1] 
    /*00006be0*/ LDRD        R2, [R0, #8] 
    /*00006be4*/ STRD        R2, [R1, #8] 
    /*00006be8*/ LDRD        R2, [R0, #16] 
    /*00006bec*/ STRD        R2, [R1, #16] 
    /*00006bf0*/ LDRD        R2, [R0, #24] 
    /*00006bf4*/ STRD        R2, [R1, #24] 
    /*00006bf8*/ LDRD        R2, [R0, #32] 
    /*00006bfc*/ STRD        R2, [R1, #32] 
    /*00006c00*/ LDRD        R2, [R0, #40]               @ 0x28 
    /*00006c04*/ STRD        R2, [R1, #40]               @ 0x28 
    /*00006c08*/ BX          LR 
FUNC_END motionModel2array


FUNC_BEGIN array2motionModel
    /*00006c0c*/ LDRD        R2, [R1] 
    /*00006c10*/ STRD        R2, [R0] 
    /*00006c14*/ LDRD        R2, [R1, #8] 
    /*00006c18*/ STRD        R2, [R0, #8] 
    /*00006c1c*/ LDRD        R2, [R1, #16] 
    /*00006c20*/ STRD        R2, [R0, #16] 
    /*00006c24*/ LDRD        R2, [R1, #24] 
    /*00006c28*/ STRD        R2, [R0, #24] 
    /*00006c2c*/ LDRD        R2, [R1, #32] 
    /*00006c30*/ STRD        R2, [R0, #32] 
    /*00006c34*/ LDRD        R2, [R1, #40]               @ 0x28 
    /*00006c38*/ STRD        R2, [R0, #40]               @ 0x28 
    /*00006c3c*/ BX          LR 
FUNC_END array2motionModel


FUNC_BEGIN DIS_MotionModel_VEC_init
    /*00006c40*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00006c44*/ MOV         R8, R1 
    /*00006c48*/ MOV         R7, R0 
    /*00006c4c*/ MOV         R1, R2 
    /*00006c50*/ ADD         R0, R0, #20 
    /*00006c54*/ MOV         R2, #6 
    /*00006c58*/ BL          0x0000000000006C58          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00006c5c*/ LSL         R0, R8, #2 
    /*00006c60*/ MOV         R1, #0 
    /*00006c64*/ BL          0x0000000000006C64          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*00006c68*/ SUBS        R5, R0, #0 
    /*00006c6c*/ BEQ         DIS_MotionModel_VEC_init_x6d7c
    /*00006c70*/ MOV         R0, #48                     @ 0x30 
    /*00006c74*/ STR         R5, [R7] 
    /*00006c78*/ MUL         R0, R0, R8 
    /*00006c7c*/ MOV         R1, #0 
    /*00006c80*/ BL          0x0000000000006C80          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*00006c84*/ SUBS        R6, R0, #0 
    /*00006c88*/ BEQ         DIS_MotionModel_VEC_init_x6d84
    /*00006c8c*/ CMP         R8, #0 
    /*00006c90*/ STR         R6, [R5] 
    /*00006c94*/ SUBNE       R5, R5, #4 
    /*00006c98*/ MOVNE       R9, #0 
    /*00006c9c*/ MOVNE       R4, #1 
    /*00006ca0*/ BEQ         DIS_MotionModel_VEC_init_x6d0c
    DIS_MotionModel_VEC_init_x6ca4:
    /*00006ca4*/ STR         R6, [R5, #4] 
    /*00006ca8*/ MOV         R2, #48                     @ 0x30 
    /*00006cac*/ MOV         R1, #0 
    /*00006cb0*/ MOV         R0, R6 
    /*00006cb4*/ BL          0x0000000000006CB4          @ osal_memset [R_ARM_CALL: +0x0] 
    /*00006cb8*/ LDRB        R2, [R7, #20] 
    /*00006cbc*/ LDR         R1, [R5, #4] 
    /*00006cc0*/ ADD         R9, R9, #1 
    /*00006cc4*/ SUB         R3, R2, #32 
    /*00006cc8*/ RSB         R0, R2, #32 
    /*00006ccc*/ LSL         R3, R4, R3 
    /*00006cd0*/ LSL         R2, R4, R2 
    /*00006cd4*/ ORR         R3, R3, R4, LSR R0 
    /*00006cd8*/ STM         R1, {R2, R3} 
    /*00006cdc*/ LDRB        R2, [R7, #24] 
    /*00006ce0*/ CMP         R8, R9 
    /*00006ce4*/ LDR         R1, [R5, #4]! 
    /*00006ce8*/ ADD         R6, R6, #48                 @ 0x30 
    /*00006cec*/ SUB         R3, R2, #32 
    /*00006cf0*/ RSB         R0, R2, #32 
    /*00006cf4*/ LSL         R3, R4, R3 
    /*00006cf8*/ LSL         R2, R4, R2 
    /*00006cfc*/ ORR         R3, R3, R4, LSR R0 
    /*00006d00*/ STR         R2, [R1, #32] 
    /*00006d04*/ STR         R3, [R1, #36]               @ 0x24 
    /*00006d08*/ BNE         DIS_MotionModel_VEC_init_x6ca4
    DIS_MotionModel_VEC_init_x6d0c:
    /*00006d0c*/ MOV         R3, #0 
    /*00006d10*/ STR         R8, [R7, #4] 
    /*00006d14*/ MOVW        R2, #0                      @ DIS_MotionModel_VEC_popfront [R_ARM_MOVW_ABS_NC: .text+0x65e8] 
    /*00006d18*/ MOVW        R1, #0                      @ DIS_MotionModel_VEC_popback [R_ARM_MOVW_ABS_NC: .text+0x661c] 
    /*00006d1c*/ MOVT        R2, #0                      @ DIS_MotionModel_VEC_popfront [R_ARM_MOVT_ABS: .text+0x65e8] 
    /*00006d20*/ MOVT        R1, #0                      @ DIS_MotionModel_VEC_popback [R_ARM_MOVT_ABS: .text+0x661c] 
    /*00006d24*/ STR         R2, [R7, #28] 
    /*00006d28*/ MOV         R0, R3 
    /*00006d2c*/ STR         R1, [R7, #32] 
    /*00006d30*/ MOVW        R2, #0                      @ DIS_MotionModel_VEC_pushback [R_ARM_MOVW_ABS_NC: .text+0x67f0] 
    /*00006d34*/ MOVW        R1, #0                      @ DIS_MotionModel_VEC_clearVector [R_ARM_MOVW_ABS_NC: .text+0x6650] 
    /*00006d38*/ MOVT        R2, #0                      @ DIS_MotionModel_VEC_pushback [R_ARM_MOVT_ABS: .text+0x67f0] 
    /*00006d3c*/ MOVT        R1, #0                      @ DIS_MotionModel_VEC_clearVector [R_ARM_MOVT_ABS: .text+0x6650] 
    /*00006d40*/ STR         R2, [R7, #36]               @ 0x24 
    /*00006d44*/ STR         R1, [R7, #40]               @ 0x28 
    /*00006d48*/ MOVW        R2, #0                      @ DIS_MotionModel_VEC_getElement [R_ARM_MOVW_ABS_NC: .text+0x6664] 
    /*00006d4c*/ MOVW        R1, #0                      @ DIS_MotionModel_VEC_getFirstElement [R_ARM_MOVW_ABS_NC: .text+0x66cc] 
    /*00006d50*/ MOVT        R2, #0                      @ DIS_MotionModel_VEC_getElement [R_ARM_MOVT_ABS: .text+0x6664] 
    /*00006d54*/ MOVT        R1, #0                      @ DIS_MotionModel_VEC_getFirstElement [R_ARM_MOVT_ABS: .text+0x66cc] 
    /*00006d58*/ STR         R2, [R7, #44]               @ 0x2c 
    /*00006d5c*/ STR         R1, [R7, #48]               @ 0x30 
    /*00006d60*/ MOVW        R2, #0                      @ DIS_MotionModel_VEC_getLastElement [R_ARM_MOVW_ABS_NC: .text+0x6714] 
    /*00006d64*/ STR         R3, [R7, #8] 
    /*00006d68*/ MOVT        R2, #0                      @ DIS_MotionModel_VEC_getLastElement [R_ARM_MOVT_ABS: .text+0x6714] 
    /*00006d6c*/ STR         R3, [R7, #12] 
    /*00006d70*/ STR         R2, [R7, #52]               @ 0x34 
    /*00006d74*/ STR         R3, [R7, #16] 
    /*00006d78*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    DIS_MotionModel_VEC_init_x6d7c:
    /*00006d7c*/ MVN         R0, #0 
    /*00006d80*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    DIS_MotionModel_VEC_init_x6d84:
    /*00006d84*/ LDR         R0, [R7] 
    /*00006d88*/ BL          0x0000000000006D88          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00006d8c*/ MVN         R0, #0 
    /*00006d90*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
FUNC_END DIS_MotionModel_VEC_init


FUNC_BEGIN DIS_MotionModel_VEC_uninit
    /*00006d94*/ LDR         R3, [R0] 
    /*00006d98*/ CMP         R3, #0 
    /*00006d9c*/ BXEQ        LR 
    /*00006da0*/ PUSH        {R4, LR} 
    /*00006da4*/ MOV         R4, R0 
    /*00006da8*/ LDR         R0, [R3] 
    /*00006dac*/ CMP         R0, #0 
    /*00006db0*/ BEQ         DIS_MotionModel_VEC_uninit_x6dd0
    /*00006db4*/ BL          0x0000000000006DB4          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00006db8*/ LDR         R3, [R4] 
    /*00006dbc*/ MOV         R2, #0 
    /*00006dc0*/ STR         R2, [R3] 
    /*00006dc4*/ LDR         R3, [R4] 
    /*00006dc8*/ CMP         R3, R2 
    /*00006dcc*/ POPEQ       {R4, PC} 
    DIS_MotionModel_VEC_uninit_x6dd0:
    /*00006dd0*/ MOV         R0, R3 
    /*00006dd4*/ BL          0x0000000000006DD4          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*00006dd8*/ MOV         R3, #0 
    /*00006ddc*/ STR         R3, [R4] 
    /*00006de0*/ POP         {R4, PC} 
FUNC_END DIS_MotionModel_VEC_uninit


FUNC_BEGIN HIISP_CQ_VECTOR_INIT
    /*00006de4*/ PUSH        {R4, LR} 
    /*00006de8*/ MOV         R4, R0 
    /*00006dec*/ STR         R1, [R0] 
    /*00006df0*/ STR         R2, [R0, #12] 
    /*00006df4*/ MOV         R0, #0 
    /*00006df8*/ STR         R3, [R4, #16] 
    /*00006dfc*/ STR         R0, [R4, #20] 
    /*00006e00*/ STR         R0, [R4, #24] 
    /*00006e04*/ STR         R0, [R4, #28] 
    /*00006e08*/ CMP         R1, #7 
    /*00006e0c*/ LDRLS       PC, [PC, R1, LSL #2] 
    /*00006e10*/ B           HIISP_CQ_VECTOR_INIT_x6e34
    /*00006e14*/ ANDEQ       R6, R0, R4, LSR LR 
    /*00006e18*/ ANDEQ       R6, R0, R4, LSR LR 
    /*00006e1c*/ MULEQ       R0, IP, LR 
    /*00006e20*/ MULEQ       R0, IP, LR 
    /*00006e24*/ ANDEQ       R6, R0, R8, LSR #29 
    /*00006e28*/ ANDEQ       R6, R0, R8, LSR #29 
    /*00006e2c*/ @           <UNDEFINED> instruction: 0x00006eb4 
    /*00006e30*/ @           <UNDEFINED> instruction: 0x00006eb4 
    HIISP_CQ_VECTOR_INIT_x6e34:
    /*00006e34*/ MOV         R1, #1 
    /*00006e38*/ STRB        R1, [R4, #8] 
    HIISP_CQ_VECTOR_INIT_x6e3c:
    /*00006e3c*/ MUL         R2, R1, R2 
    /*00006e40*/ STR         R2, [R4, #4] 
    /*00006e44*/ MUL         R0, R2, R3 
    /*00006e48*/ BL          0x0000000000006E48          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*00006e4c*/ CMP         R0, #0 
    /*00006e50*/ STR         R0, [R4, #32] 
    /*00006e54*/ BEQ         HIISP_CQ_VECTOR_INIT_x6ec0
    /*00006e58*/ MOV         R0, #0 
    /*00006e5c*/ MOVW        R2, #0                      @ HIISP_CQ_VECTOR_pop_back [R_ARM_MOVW_ABS_NC: .text+0x67a8] 
    /*00006e60*/ MOVW        R3, #0                      @ HIISP_CQ_VECTOR_pop_front [R_ARM_MOVW_ABS_NC: .text+0x6774] 
    /*00006e64*/ MOVT        R2, #0                      @ HIISP_CQ_VECTOR_pop_back [R_ARM_MOVT_ABS: .text+0x67a8] 
    /*00006e68*/ MOVT        R3, #0                      @ HIISP_CQ_VECTOR_pop_front [R_ARM_MOVT_ABS: .text+0x6774] 
    /*00006e6c*/ STR         R2, [R4, #40]               @ 0x28 
    /*00006e70*/ STR         R3, [R4, #36]               @ 0x24 
    /*00006e74*/ MOVW        R2, #0                      @ HIISP_CQ_VECTOR_push_back [R_ARM_MOVW_ABS_NC: .text+0x68e0] 
    /*00006e78*/ MOVW        R3, #0                      @ HIISP_CQ_VECTOR_get_element [R_ARM_MOVW_ABS_NC: .text+0x68a8] 
    /*00006e7c*/ MOVT        R2, #0                      @ HIISP_CQ_VECTOR_push_back [R_ARM_MOVT_ABS: .text+0x68e0] 
    /*00006e80*/ MOVT        R3, #0                      @ HIISP_CQ_VECTOR_get_element [R_ARM_MOVT_ABS: .text+0x68a8] 
    /*00006e84*/ STR         R2, [R4, #44]               @ 0x2c 
    /*00006e88*/ STR         R3, [R4, #52]               @ 0x34 
    /*00006e8c*/ MOVW        R3, #0                      @ HIISP_CQ_VECTOR_clear [R_ARM_MOVW_ABS_NC: .text+0x67dc] 
    /*00006e90*/ MOVT        R3, #0                      @ HIISP_CQ_VECTOR_clear [R_ARM_MOVT_ABS: .text+0x67dc] 
    /*00006e94*/ STR         R3, [R4, #48]               @ 0x30 
    /*00006e98*/ POP         {R4, PC} 
    /*00006e9c*/ MOV         R1, #2 
    /*00006ea0*/ STRB        R1, [R4, #8] 
    /*00006ea4*/ B           HIISP_CQ_VECTOR_INIT_x6e3c
    /*00006ea8*/ MOV         R1, #4 
    /*00006eac*/ STRB        R1, [R4, #8] 
    /*00006eb0*/ B           HIISP_CQ_VECTOR_INIT_x6e3c
    /*00006eb4*/ MOV         R1, #8 
    /*00006eb8*/ STRB        R1, [R4, #8] 
    /*00006ebc*/ B           HIISP_CQ_VECTOR_INIT_x6e3c
    HIISP_CQ_VECTOR_INIT_x6ec0:
    /*00006ec0*/ MOVW        R0, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2364] 
    /*00006ec4*/ MOVT        R0, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2364] 
    /*00006ec8*/ BL          0x0000000000006EC8          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00006ecc*/ MVN         R0, #0 
    /*00006ed0*/ POP         {R4, PC} 
FUNC_END HIISP_CQ_VECTOR_INIT


FUNC_BEGIN HIISP_CQ_VECTOR_UNINIT
    /*00006ed4*/ LDR         R3, [R0, #32] 
    /*00006ed8*/ CMP         R3, #0 
    /*00006edc*/ BXEQ        LR 
    /*00006ee0*/ PUSH        {R4, LR} 
    /*00006ee4*/ MOV         R4, R0 
    /*00006ee8*/ MOV         R0, R3 
    /*00006eec*/ BL          0x0000000000006EEC          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*00006ef0*/ MOV         R3, #0 
    /*00006ef4*/ STR         R3, [R4, #32] 
    /*00006ef8*/ POP         {R4, PC} 
FUNC_END HIISP_CQ_VECTOR_UNINIT


FUNC_BEGIN DIS_SQRT_HW
    /*00006efc*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00006f00*/ SUB         SP, SP, #332                @ 0x14c 
    /*00006f04*/ MOV         R4, R0 
    /*00006f08*/ MOV         R1, #0 
    /*00006f0c*/ STRD        R2, [SP, #56]               @ 0x38 
    /*00006f10*/ ADD         R0, SP, #72                 @ 0x48 
    /*00006f14*/ MOV         R2, #256                    @ 0x100 
    /*00006f18*/ BL          0x0000000000006F18          @ memset [R_ARM_CALL: +0x0] 
    /*00006f1c*/ CMP         R4, #31 
    /*00006f20*/ MOV         R0, #0 
    /*00006f24*/ BHI         DIS_SQRT_HW_x7138
    /*00006f28*/ MOV         R2, #1 
    /*00006f2c*/ SUB         IP, R4, #32 
    /*00006f30*/ RSB         LR, R4, #32 
    /*00006f34*/ LSL         R1, R2, IP 
    /*00006f38*/ LSL         R0, R2, R4 
    /*00006f3c*/ ORR         R1, R1, R2, LSR LR 
    /*00006f40*/ SUBS        R0, R0, #1 
    /*00006f44*/ LSL         IP, R4, #3 
    /*00006f48*/ SBC         R1, R1, #0 
    /*00006f4c*/ ADD         LR, SP, #72                 @ 0x48 
    /*00006f50*/ UMULL       R6, R7, R0, R0 
    /*00006f54*/ STRD        R0, [LR, IP] 
    /*00006f58*/ MUL         R1, R0, R1 
    /*00006f5c*/ MOV         R3, #0 
    /*00006f60*/ CMP         R4, R3 
    /*00006f64*/ ADD         IP, LR, IP 
    /*00006f68*/ STRD        R6, [SP, #48]               @ 0x30 
    /*00006f6c*/ ADD         R1, R7, R1, LSL R2 
    /*00006f70*/ STR         R1, [SP, #52]               @ 0x34 
    /*00006f74*/ BEQ         DIS_SQRT_HW_x7134
    /*00006f78*/ SUB         R1, R4, #-2147483647        @ 0x80000001 
    /*00006f7c*/ MOV         R6, IP 
    /*00006f80*/ MOV         R5, R2 
    /*00006f84*/ MOV         LR, R3 
    /*00006f88*/ LSL         IP, R1, #1 
    /*00006f8c*/ STRD        SL, [SP] 
    /*00006f90*/ MOV         R1, R6 
    /*00006f94*/ STRD        R8, [SP, #8] 
    /*00006f98*/ B           DIS_SQRT_HW_x704c
    DIS_SQRT_HW_x6f9c:
    /*00006f9c*/ LDRD        R2, [R1] 
    /*00006fa0*/ SUB         FP, R4, #33                 @ 0x21 
    /*00006fa4*/ SUB         SL, IP, #32 
    /*00006fa8*/ LSL         FP, R5, FP 
    /*00006fac*/ LSL         SL, R5, SL 
    /*00006fb0*/ STR         SL, [SP, #20] 
    /*00006fb4*/ ORR         R7, FP, R5, LSR R7 
    /*00006fb8*/ STR         R7, [SP, #28] 
    /*00006fbc*/ LSL         R7, R3, R4 
    /*00006fc0*/ STR         R7, [SP, #4] 
    /*00006fc4*/ LDR         R7, [SP, #20] 
    /*00006fc8*/ SUB         R1, R1, #8 
    /*00006fcc*/ ORR         R7, R7, R5, LSR R6 
    /*00006fd0*/ STR         R7, [SP, #20] 
    /*00006fd4*/ LDR         R7, [SP, #4] 
    /*00006fd8*/ SUB         R6, R4, #32 
    /*00006fdc*/ ORR         R6, R7, R2, LSL R6 
    /*00006fe0*/ STR         R6, [SP, #4] 
    /*00006fe4*/ LSL         R7, R5, R0 
    /*00006fe8*/ STR         R7, [SP, #24] 
    /*00006fec*/ LDR         R7, [SP, #4] 
    /*00006ff0*/ RSB         R6, R4, #32 
    /*00006ff4*/ LSL         R4, R2, R4 
    /*00006ff8*/ STR         R4, [SP] 
    /*00006ffc*/ ORR         R6, R7, R2, LSR R6 
    /*00007000*/ STR         R6, [SP, #4] 
    /*00007004*/ LSL         R6, R5, IP 
    /*00007008*/ STR         R6, [SP, #16] 
    /*0000700c*/ LDRD        R6, [SP, #24] 
    /*00007010*/ SUB         IP, IP, #2 
    /*00007014*/ ADDS        R6, R6, R2 
    /*00007018*/ MOV         R2, R8 
    /*0000701c*/ ADC         R7, R7, R3 
    /*00007020*/ MOV         R3, R9 
    /*00007024*/ LDRD        R8, [SP] 
    /*00007028*/ STRD        R6, [R1] 
    /*0000702c*/ LDRD        R6, [SP, #16] 
    /*00007030*/ ADDS        R8, R8, R2 
    /*00007034*/ ADC         R9, R9, R3 
    /*00007038*/ ADDS        R6, R6, R8 
    /*0000703c*/ ADC         R7, R7, R9 
    /*00007040*/ SUBS        R4, R0, #0 
    /*00007044*/ STRD        R6, [SP, #48]               @ 0x30 
    /*00007048*/ BEQ         DIS_SQRT_HW_x7134
    DIS_SQRT_HW_x704c:
    /*0000704c*/ LDRD        R8, [SP, #48]               @ 0x30 
    /*00007050*/ SUB         R7, R4, #33                 @ 0x21 
    /*00007054*/ LDRD        SL, [SP, #56]               @ 0x38 
    /*00007058*/ SUB         R0, R4, #1 
    /*0000705c*/ RSB         R3, IP, #32 
    /*00007060*/ STR         R7, [SP, #68]               @ 0x44 
    /*00007064*/ CMP         R8, SL 
    /*00007068*/ MOV         R2, R7 
    /*0000706c*/ SBCS        SL, R9, FP 
    /*00007070*/ RSB         R7, R0, #32 
    /*00007074*/ MOV         R6, R3 
    /*00007078*/ MOV         R3, R7 
    /*0000707c*/ BLT         DIS_SQRT_HW_x6f9c
    /*00007080*/ LDRD        R6, [R1] 
    /*00007084*/ SUB         R8, IP, #32 
    /*00007088*/ RSB         R9, IP, #32 
    /*0000708c*/ LSL         R2, R5, R2 
    /*00007090*/ LSL         R8, R5, R8 
    /*00007094*/ STR         R2, [SP, #44]               @ 0x2c 
    /*00007098*/ LSL         R2, R7, R4 
    /*0000709c*/ STR         R2, [SP, #12] 
    /*000070a0*/ ORR         R8, R8, R5, LSR R9 
    /*000070a4*/ STR         R8, [SP, #36]               @ 0x24 
    /*000070a8*/ LDR         R8, [SP, #12] 
    /*000070ac*/ LSL         R2, R5, IP 
    /*000070b0*/ STR         R2, [SP, #32] 
    /*000070b4*/ SUB         R2, R4, #32 
    /*000070b8*/ ORR         R2, R8, R6, LSL R2 
    /*000070bc*/ STR         R2, [SP, #12] 
    /*000070c0*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*000070c4*/ SUB         R1, R1, #8 
    /*000070c8*/ SUB         IP, IP, #2 
    /*000070cc*/ ORR         R3, R2, R5, LSR R3 
    /*000070d0*/ LDR         R2, [SP, #12] 
    /*000070d4*/ STR         R3, [SP, #44]               @ 0x2c 
    /*000070d8*/ RSB         R3, R4, #32 
    /*000070dc*/ ORR         R3, R2, R6, LSR R3 
    /*000070e0*/ STR         R3, [SP, #12] 
    /*000070e4*/ LSL         R3, R5, R0 
    /*000070e8*/ STR         R3, [SP, #40]               @ 0x28 
    /*000070ec*/ LSL         R3, R6, R4 
    /*000070f0*/ STR         R3, [SP, #8] 
    /*000070f4*/ LDRD        R2, [SP, #40]               @ 0x28 
    /*000070f8*/ LDRD        R8, [SP, #8] 
    /*000070fc*/ SUBS        R2, R6, R2 
    /*00007100*/ SBC         R3, R7, R3 
    /*00007104*/ MOV         R6, R2 
    /*00007108*/ MOV         R7, R3 
    /*0000710c*/ LDRD        R2, [SP, #48]               @ 0x30 
    /*00007110*/ STRD        R6, [R1] 
    /*00007114*/ LDRD        R6, [SP, #32] 
    /*00007118*/ SUBS        R8, R2, R8 
    /*0000711c*/ SBC         R9, R3, R9 
    /*00007120*/ ADDS        R6, R6, R8 
    /*00007124*/ ADC         R7, R7, R9 
    /*00007128*/ SUBS        R4, R0, #0 
    /*0000712c*/ STRD        R6, [SP, #48]               @ 0x30 
    /*00007130*/ BNE         DIS_SQRT_HW_x704c
    DIS_SQRT_HW_x7134:
    /*00007134*/ LDR         R0, [SP, #72]               @ 0x48 
    DIS_SQRT_HW_x7138:
    /*00007138*/ ADD         SP, SP, #332                @ 0x14c 
    /*0000713c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
FUNC_END DIS_SQRT_HW


FUNC_BEGIN Number2Bits
    /*00007140*/ CMP         R0, #1 
    /*00007144*/ MOV         R3, #0 
    /*00007148*/ BEQ         Number2Bits_x7160
    Number2Bits_x714c:
    /*0000714c*/ ADD         R3, R3, #1 
    /*00007150*/ UXTB        R3, R3 
    /*00007154*/ LSR         R2, R0, R3 
    /*00007158*/ CMP         R2, #1 
    /*0000715c*/ BNE         Number2Bits_x714c
    Number2Bits_x7160:
    /*00007160*/ MOV         R0, R3 
    /*00007164*/ BX          LR 
FUNC_END Number2Bits


FUNC_BEGIN Polar2Cartesian
    /*00007168*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000716c*/ SUB         SP, SP, #300                @ 0x12c 
    /*00007170*/ ADD         R4, SP, #152                @ 0x98 
    /*00007174*/ ADD         R5, SP, #224                @ 0xe0 
    /*00007178*/ MOV         R6, R1 
    /*0000717c*/ MOV         R7, R2 
    /*00007180*/ MOV         R8, R0 
    /*00007184*/ MOV         R2, #72                     @ 0x48 
    /*00007188*/ MOV         R1, #0 
    /*0000718c*/ MOV         R0, R4 
    /*00007190*/ BL          0x0000000000007190          @ memset [R_ARM_CALL: +0x0] 
    /*00007194*/ MOV         R2, #72                     @ 0x48 
    /*00007198*/ MOV         R1, #0 
    /*0000719c*/ MOV         R0, R5 
    /*000071a0*/ BL          0x00000000000071A0          @ memset [R_ARM_CALL: +0x0] 
    /*000071a4*/ MOV         R2, #68                     @ 0x44 
    /*000071a8*/ LDR         R1, [PC, #804]              @ 0x00000000000074d4 
    /*000071ac*/ ADD         R0, SP, #16 
    /*000071b0*/ BL          0x00000000000071B0          @ memcpy [R_ARM_CALL: +0x0] 
    /*000071b4*/ MOV         R2, #68                     @ 0x44 
    /*000071b8*/ LDR         R1, [PC, #792]              @ 0x00000000000074d8 
    /*000071bc*/ ADD         R0, SP, #84                 @ 0x54 
    /*000071c0*/ BL          0x00000000000071C0          @ memcpy [R_ARM_CALL: +0x0] 
    /*000071c4*/ LDR         LR, [R6] 
    /*000071c8*/ MOV         R0, #0 
    /*000071cc*/ LDR         IP, [R6, #4] 
    /*000071d0*/ STR         R0, [R8] 
    /*000071d4*/ SUB         R2, R7, #8 
    /*000071d8*/ STR         R0, [R8, #4] 
    /*000071dc*/ MOV         R3, #256                    @ 0x100 
    /*000071e0*/ LDR         R1, [R6] 
    /*000071e4*/ LSL         R3, R3, R2 
    /*000071e8*/ CMP         R1, #0 
    /*000071ec*/ RSBLT       R1, R1, #0 
    /*000071f0*/ CMP         R3, LR 
    /*000071f4*/ CMPNE       LR, R0 
    /*000071f8*/ UBFX        R1, R1, #14, #2 
    /*000071fc*/ STR         R1, [SP, #8] 
    /*00007200*/ MOVEQ       R7, #1 
    /*00007204*/ MOVNE       R7, #0 
    /*00007208*/ STR         R1, [R6, #8] 
    /*0000720c*/ BEQ         Polar2Cartesian_x7350
    /*00007210*/ MOV         R1, #64                     @ 0x40 
    /*00007214*/ LSL         R1, R1, R2 
    /*00007218*/ CMP         R1, LR 
    /*0000721c*/ BEQ         Polar2Cartesian_x7418
    /*00007220*/ MOV         R0, #128                    @ 0x80 
    /*00007224*/ LSL         R0, R0, R2 
    /*00007228*/ CMP         R0, LR 
    /*0000722c*/ BEQ         Polar2Cartesian_x746c
    /*00007230*/ MOV         R6, #192                    @ 0xc0 
    /*00007234*/ LSL         R2, R6, R2 
    /*00007238*/ CMP         R2, LR 
    /*0000723c*/ BEQ         Polar2Cartesian_x7494
    /*00007240*/ CMP         R1, LR 
    /*00007244*/ BCC         Polar2Cartesian_x73fc
    Polar2Cartesian_x7248:
    /*00007248*/ ASR         R1, IP, #31 
    /*0000724c*/ MOV         R0, IP 
    /*00007250*/ LSL         LR, LR, #2 
    /*00007254*/ ASR         R6, R1, #31 
    /*00007258*/ EOR         R2, R6, IP 
    /*0000725c*/ SUBS        R2, R2, R6 
    /*00007260*/ CMP         IP, #0 
    /*00007264*/ SBCS        R1, R1, #0 
    /*00007268*/ BLT         Polar2Cartesian_x748c
    Polar2Cartesian_x726c:
    /*0000726c*/ MOV         R7, #0 
    /*00007270*/ MOV         R1, #16 
    /*00007274*/ STR         R2, [SP, #152]              @ 0x98 
    /*00007278*/ MOV         R6, R7 
    /*0000727c*/ MOV         SL, R7 
    /*00007280*/ MOV         IP, R7 
    /*00007284*/ MOV         R3, R7 
    /*00007288*/ MOV         R9, R7 
    /*0000728c*/ MVN         FP, #0 
    /*00007290*/ STR         R1, [SP, #4] 
    /*00007294*/ STR         R8, [SP, #12] 
    /*00007298*/ B           Polar2Cartesian_x732c
    Polar2Cartesian_x729c:
    /*0000729c*/ MOVLT       R0, #1 
    /*000072a0*/ MVNGE       R0, #0 
    Polar2Cartesian_x72a4:
    /*000072a4*/ ADD         R1, SP, #16 
    /*000072a8*/ CMP         R6, #0 
    /*000072ac*/ ADD         IP, R3, #1 
    /*000072b0*/ LDR         R1, [R1, R7, LSL #2] 
    /*000072b4*/ MLS         SL, R1, R0, SL 
    /*000072b8*/ MOVEQ       R1, R6 
    /*000072bc*/ BEQ         Polar2Cartesian_x72d4
    /*000072c0*/ BLE         Polar2Cartesian_x743c
    /*000072c4*/ CMP         R7, #0 
    /*000072c8*/ BEQ         Polar2Cartesian_x74cc
    /*000072cc*/ MOV         R1, #1 
    /*000072d0*/ LSL         R1, R1, FP 
    Polar2Cartesian_x72d4:
    /*000072d4*/ ADD         R1, R1, R6 
    /*000072d8*/ ASR         R1, R1, R3 
    /*000072dc*/ MLA         R2, R0, R1, R2 
    /*000072e0*/ STR         R2, [R4, IP, LSL #2] 
    /*000072e4*/ LDR         R8, [R4, R7, LSL #2] 
    /*000072e8*/ CMP         R8, #0 
    /*000072ec*/ MOVEQ       R1, R8 
    /*000072f0*/ BEQ         Polar2Cartesian_x7308
    /*000072f4*/ BLE         Polar2Cartesian_x7454
    /*000072f8*/ CMP         R7, #0 
    /*000072fc*/ BEQ         Polar2Cartesian_x74bc
    /*00007300*/ MOV         R1, #1 
    /*00007304*/ LSL         R1, R1, FP 
    Polar2Cartesian_x7308:
    /*00007308*/ ADD         R1, R1, R8 
    /*0000730c*/ CMP         IP, #17 
    /*00007310*/ ASR         R1, R1, R3 
    /*00007314*/ MLS         R6, R0, R1, R6 
    /*00007318*/ STR         R6, [R5, IP, LSL #2] 
    /*0000731c*/ BEQ         Polar2Cartesian_x7370
    Polar2Cartesian_x7320:
    /*00007320*/ ADD         R7, R7, #1 
    /*00007324*/ ADD         FP, FP, #1 
    /*00007328*/ MOV         R3, IP 
    Polar2Cartesian_x732c:
    /*0000732c*/ CMP         LR, SL 
    /*00007330*/ BNE         Polar2Cartesian_x729c
    /*00007334*/ LDR         R1, [SP, #4] 
    /*00007338*/ ADD         R9, R9, #1 
    /*0000733c*/ CMP         R9, #1 
    /*00007340*/ MOV         R0, #0 
    /*00007344*/ MOVEQ       R1, IP 
    /*00007348*/ STR         R1, [SP, #4] 
    /*0000734c*/ B           Polar2Cartesian_x72a4
    Polar2Cartesian_x7350:
    /*00007350*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x60] 
    /*00007354*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x60] 
    /*00007358*/ STR         R0, [R8, #4] 
    /*0000735c*/ LDR         R3, [R3] 
    /*00007360*/ MUL         IP, IP, R3 
    /*00007364*/ STR         IP, [R8] 
    Polar2Cartesian_x7368:
    /*00007368*/ ADD         SP, SP, #300                @ 0x12c 
    /*0000736c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    Polar2Cartesian_x7370:
    /*00007370*/ LDR         R2, [SP, #8] 
    /*00007374*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x60] 
    /*00007378*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x60] 
    /*0000737c*/ LDR         R8, [SP, #12] 
    /*00007380*/ LDR         R1, [R3, R2, LSL #2] 
    /*00007384*/ LSL         R0, R2, #2 
    /*00007388*/ EOR         R2, R1, R1, ASR #31 
    /*0000738c*/ SUB         R2, R2, R1, ASR #31 
    /*00007390*/ CMP         R2, #1 
    /*00007394*/ BNE         Polar2Cartesian_x74b4
    /*00007398*/ ADD         R3, R3, R0 
    /*0000739c*/ LDR         IP, [R3, #20] 
    /*000073a0*/ EOR         R3, IP, IP, ASR #31 
    /*000073a4*/ SUB         R3, R3, IP, ASR #31 
    /*000073a8*/ CMP         R3, #1 
    /*000073ac*/ BNE         Polar2Cartesian_x74b4
    /*000073b0*/ LDR         R2, [SP, #4] 
    /*000073b4*/ ADD         R3, SP, #296                @ 0x128 
    /*000073b8*/ LDR         R6, [SP, #292]              @ 0x124 
    /*000073bc*/ MOV         R0, #0 
    /*000073c0*/ ADD         R3, R3, R2, LSL #2 
    /*000073c4*/ LDR         R2, [SP, #220]              @ 0xdc 
    /*000073c8*/ LDR         R4, [R3, #-212]             @ 0xffffff2c 
    /*000073cc*/ SMULL       R6, R7, R4, R6 
    /*000073d0*/ SMULL       R4, R5, R4, R2 
    /*000073d4*/ LSR         R2, R6, #15 
    /*000073d8*/ ORR         R2, R2, R7, LSL #17 
    /*000073dc*/ LSR         R3, R4, #15 
    /*000073e0*/ ORR         R3, R3, R5, LSL #17 
    /*000073e4*/ MUL         R2, R2, IP 
    /*000073e8*/ MUL         R3, R3, R1 
    /*000073ec*/ STR         R2, [R8, #4] 
    /*000073f0*/ STR         R3, [R8] 
    /*000073f4*/ ADD         SP, SP, #300                @ 0x12c 
    /*000073f8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    Polar2Cartesian_x73fc:
    /*000073fc*/ CMP         R0, LR 
    /*00007400*/ SUBHI       LR, R0, LR 
    /*00007404*/ BHI         Polar2Cartesian_x7248
    /*00007408*/ CMP         R2, LR 
    /*0000740c*/ SUBHI       LR, LR, R0 
    /*00007410*/ SUBLS       LR, R3, LR 
    /*00007414*/ B           Polar2Cartesian_x7248
    Polar2Cartesian_x7418:
    /*00007418*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x60] 
    /*0000741c*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x60] 
    /*00007420*/ STR         R7, [R8] 
    /*00007424*/ MOV         R0, R7 
    /*00007428*/ LDR         R3, [R3, #24] 
    /*0000742c*/ MUL         IP, IP, R3 
    /*00007430*/ STR         IP, [R8, #4] 
    /*00007434*/ ADD         SP, SP, #300                @ 0x12c 
    /*00007438*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    Polar2Cartesian_x743c:
    /*0000743c*/ CMP         R7, #0 
    /*00007440*/ BEQ         Polar2Cartesian_x74cc
    /*00007444*/ MOV         R1, #1 
    /*00007448*/ LSL         R1, R1, FP 
    /*0000744c*/ RSB         R1, R1, #0 
    /*00007450*/ B           Polar2Cartesian_x72d4
    Polar2Cartesian_x7454:
    /*00007454*/ CMP         R7, #0 
    /*00007458*/ BEQ         Polar2Cartesian_x74bc
    /*0000745c*/ MOV         R1, #1 
    /*00007460*/ LSL         R1, R1, FP 
    /*00007464*/ RSB         R1, R1, #0 
    /*00007468*/ B           Polar2Cartesian_x7308
    Polar2Cartesian_x746c:
    /*0000746c*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x60] 
    /*00007470*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x60] 
    /*00007474*/ MOV         R0, R7 
    /*00007478*/ LDR         R3, [R3, #8] 
    /*0000747c*/ STR         R7, [R8, #4] 
    /*00007480*/ MUL         IP, IP, R3 
    /*00007484*/ STR         IP, [R8] 
    /*00007488*/ B           Polar2Cartesian_x7368
    Polar2Cartesian_x748c:
    /*0000748c*/ RSBS        R2, R2, #0 
    /*00007490*/ B           Polar2Cartesian_x726c
    Polar2Cartesian_x7494:
    /*00007494*/ MOVW        R3, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .data+0x60] 
    /*00007498*/ MOVT        R3, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .data+0x60] 
    /*0000749c*/ STR         R7, [R8] 
    /*000074a0*/ MOV         R0, R7 
    /*000074a4*/ LDR         R3, [R3, #32] 
    /*000074a8*/ MUL         IP, IP, R3 
    /*000074ac*/ STR         IP, [R8, #4] 
    /*000074b0*/ B           Polar2Cartesian_x7368
    Polar2Cartesian_x74b4:
    /*000074b4*/ MVN         R0, #0 
    /*000074b8*/ B           Polar2Cartesian_x7368
    Polar2Cartesian_x74bc:
    /*000074bc*/ ASR         R3, R8, R3 
    /*000074c0*/ MLS         R6, R0, R3, R6 
    /*000074c4*/ STR         R6, [R5, IP, LSL #2] 
    /*000074c8*/ B           Polar2Cartesian_x7320
    Polar2Cartesian_x74cc:
    /*000074cc*/ MOV         R1, R7 
    /*000074d0*/ B           Polar2Cartesian_x72d4
    /*000074d4*/ ANDEQ       R0, R0, IP, ROR #9 
    /*000074d8*/ ANDEQ       R0, R0, R0, LSR R5 
FUNC_END Polar2Cartesian


FUNC_BEGIN DIS_SignedRightShift
    /*000074dc*/ CMP         R0, #0 
    /*000074e0*/ SBCS        R3, R1, #0 
    /*000074e4*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*000074e8*/ MOV         R6, R0 
    /*000074ec*/ MOV         R7, R1 
    /*000074f0*/ BLT         DIS_SignedRightShift_x7510
    /*000074f4*/ RSB         IP, R2, #32 
    /*000074f8*/ LSR         R0, R6, R2 
    /*000074fc*/ SUB         R3, R2, #32 
    /*00007500*/ ORR         R0, R0, R7, LSL IP 
    /*00007504*/ ORR         R0, R0, R7, LSR R3 
    /*00007508*/ LSR         R1, R7, R2 
    /*0000750c*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    DIS_SignedRightShift_x7510:
    /*00007510*/ SUB         IP, R2, #32 
    /*00007514*/ MOV         R3, #1 
    /*00007518*/ RSB         LR, R2, #32 
    /*0000751c*/ LSL         R9, R3, IP 
    /*00007520*/ LSL         R8, R3, R2 
    /*00007524*/ ORR         R9, R9, R3, LSR LR 
    /*00007528*/ SUBS        R8, R8, #1 
    /*0000752c*/ LSR         R0, R6, R2 
    /*00007530*/ SBC         R9, R9, #0 
    /*00007534*/ RSB         R3, R2, #64                 @ 0x40 
    /*00007538*/ ORR         R0, R0, R7, LSL LR 
    /*0000753c*/ LSR         R1, R7, R2 
    /*00007540*/ ORR         R0, R0, R7, LSR IP 
    /*00007544*/ LSL         R5, R9, R3 
    /*00007548*/ SUB         IP, R3, #32 
    /*0000754c*/ RSB         R2, R3, #32 
    /*00007550*/ ORR         R5, R5, R8, LSL IP 
    /*00007554*/ LSL         R4, R8, R3 
    /*00007558*/ ORR         R5, R5, R8, LSR R2 
    /*0000755c*/ ORR         R0, R0, R4 
    /*00007560*/ ORR         R1, R1, R5 
    /*00007564*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
FUNC_END DIS_SignedRightShift


FUNC_BEGIN DIS_SignedLeftShift
    /*00007568*/ PUSH        {R4, R5, R6, R7} 
    /*0000756c*/ ASR         R6, R1, #31 
    /*00007570*/ EOR         R4, R6, R0 
    /*00007574*/ EOR         R5, R6, R1 
    /*00007578*/ SUBS        R4, R4, R6 
    /*0000757c*/ SUB         IP, R2, #32 
    /*00007580*/ SBC         R5, R5, R6 
    /*00007584*/ CMP         R0, #0 
    /*00007588*/ SBCS        R3, R1, #0 
    /*0000758c*/ RSB         R3, R2, #32 
    /*00007590*/ LSL         R1, R5, R2 
    /*00007594*/ LSL         R0, R4, R2 
    /*00007598*/ ORR         R1, R1, R4, LSL IP 
    /*0000759c*/ ORR         R1, R1, R4, LSR R3 
    /*000075a0*/ BLT         DIS_SignedLeftShift_x75ac
    /*000075a4*/ POP         {R4, R5, R6, R7} 
    /*000075a8*/ BX          LR 
    DIS_SignedLeftShift_x75ac:
    /*000075ac*/ RSBS        R0, R0, #0 
    /*000075b0*/ RSC         R1, R1, #0 
    /*000075b4*/ ORR         R1, R1, #-2147483648        @ 0x80000000 
    /*000075b8*/ POP         {R4, R5, R6, R7} 
    /*000075bc*/ BX          LR 
FUNC_END DIS_SignedLeftShift


FUNC_BEGIN hiisp_dis_setDynamicAttr
    /*000075c0*/ MOV         R3, #1344                   @ 0x540 
    /*000075c4*/ MOVW        IP, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*000075c8*/ MUL         R3, R3, R0 
    /*000075cc*/ MOVT        IP, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*000075d0*/ MOV         R0, R1 
    /*000075d4*/ PUSH        {R4, LR} 
    /*000075d8*/ ADD         R2, R3, #472                @ 0x1d8 
    /*000075dc*/ ADD         R3, R3, #268                @ 0x10c 
    /*000075e0*/ ADD         R2, IP, R2 
    /*000075e4*/ ADD         R1, IP, R3 
    /*000075e8*/ BL          0x00000000000075E8          @ hiisp_dis_dynamic_cfg_parse [R_ARM_CALL: .text+0xd6ec] 
    /*000075ec*/ MOV         R0, #0 
    /*000075f0*/ POP         {R4, PC} 
FUNC_END hiisp_dis_setDynamicAttr


FUNC_BEGIN hiisp_dis_setUnitMotionData
    /*000075f4*/ MOVW        IP, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*000075f8*/ MOVT        IP, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*000075fc*/ MOV         R3, #1344                   @ 0x540 
    /*00007600*/ MLA         R3, R3, R0, IP 
    /*00007604*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00007608*/ MOV         R4, #0 
    /*0000760c*/ MOV         R5, #0 
    /*00007610*/ MOV         R6, #32768                  @ 0x8000 
    /*00007614*/ STRD        R4, [R1, #16] 
    /*00007618*/ MOV         R7, #0 
    /*0000761c*/ STRD        R4, [R1, #40]               @ 0x28 
    /*00007620*/ STRD        R6, [R1] 
    /*00007624*/ STRD        R6, [R1, #32] 
    /*00007628*/ STRD        R4, [R1, #8] 
    /*0000762c*/ STRD        R4, [R1, #24] 
    /*00007630*/ LDR         LR, [R3, #520]              @ 0x208 
    /*00007634*/ LDR         R8, [R3, #516]              @ 0x204 
    /*00007638*/ ASR         R7, LR, #31 
    /*0000763c*/ ASR         R5, R8, #31 
    /*00007640*/ LSL         R6, R8, #15 
    /*00007644*/ LSL         R2, R7, #15 
    /*00007648*/ STR         R6, [R1, #16] 
    /*0000764c*/ LSL         R4, R5, #15 
    /*00007650*/ ORR         R2, R2, LR, LSR #17 
    /*00007654*/ LSL         R5, LR, #15 
    /*00007658*/ ORR         R4, R4, R8, LSR #17 
    /*0000765c*/ STR         R5, [R1, #40]               @ 0x28 
    /*00007660*/ STR         R4, [R1, #20] 
    /*00007664*/ STR         R2, [R1, #44]               @ 0x2c 
    /*00007668*/ LDR         LR, [R3, #660]              @ 0x294 
    /*0000766c*/ CMP         LR, #8192                   @ 0x2000 
    /*00007670*/ BEQ         hiisp_dis_setUnitMotionData_x76e8
    /*00007674*/ ASR         R5, LR, #31 
    /*00007678*/ LSL         R2, LR, #15 
    /*0000767c*/ CMP         R2, #0 
    /*00007680*/ LSL         R3, R5, #15 
    /*00007684*/ ORR         R3, R3, LR, LSR #17 
    /*00007688*/ SBCS        LR, R3, #0 
    /*0000768c*/ BLT         hiisp_dis_setUnitMotionData_x76f0
    /*00007690*/ ADDS        R2, R2, #4096               @ 0x1000 
    /*00007694*/ ADC         R3, R3, #0 
    /*00007698*/ LSR         R4, R2, #13 
    /*0000769c*/ ORR         R4, R4, R3, LSL #19 
    /*000076a0*/ ASR         R5, R3, #13 
    hiisp_dis_setUnitMotionData_x76a4:
    /*000076a4*/ MOV         R3, #1344                   @ 0x540 
    /*000076a8*/ STRD        R4, [R1] 
    /*000076ac*/ MLA         R0, R3, R0, IP 
    /*000076b0*/ LDR         R0, [R0, #660]              @ 0x294 
    /*000076b4*/ ASR         R5, R0, #31 
    /*000076b8*/ LSL         R2, R0, #15 
    /*000076bc*/ CMP         R2, #0 
    /*000076c0*/ LSL         R3, R5, #15 
    /*000076c4*/ ORR         R3, R3, R0, LSR #17 
    /*000076c8*/ SBCS        R0, R3, #0 
    /*000076cc*/ BLT         hiisp_dis_setUnitMotionData_x7718
    /*000076d0*/ ADDS        R2, R2, #4096               @ 0x1000 
    /*000076d4*/ ADC         R3, R3, #0 
    /*000076d8*/ LSR         R4, R2, #13 
    /*000076dc*/ ORR         R4, R4, R3, LSL #19 
    /*000076e0*/ ASR         R5, R3, #13 
    hiisp_dis_setUnitMotionData_x76e4:
    /*000076e4*/ STRD        R4, [R1, #32] 
    hiisp_dis_setUnitMotionData_x76e8:
    /*000076e8*/ MOV         R0, #0 
    /*000076ec*/ POP         {R4, R5, R6, R7, R8, PC} 
    hiisp_dis_setUnitMotionData_x76f0:
    /*000076f0*/ MOV         R4, #4096                   @ 0x1000 
    /*000076f4*/ SUBS        R2, R4, R2 
    /*000076f8*/ MOV         R5, #0 
    /*000076fc*/ SBC         R3, R5, R3 
    /*00007700*/ LSR         R4, R2, #13 
    /*00007704*/ ORR         R4, R4, R3, LSL #19 
    /*00007708*/ ASR         R5, R3, #13 
    /*0000770c*/ RSBS        R4, R4, #0 
    /*00007710*/ RSC         R5, R5, #0 
    /*00007714*/ B           hiisp_dis_setUnitMotionData_x76a4
    hiisp_dis_setUnitMotionData_x7718:
    /*00007718*/ MOV         R6, #4096                   @ 0x1000 
    /*0000771c*/ SUBS        R2, R6, R2 
    /*00007720*/ MOV         R7, #0 
    /*00007724*/ SBC         R3, R7, R3 
    /*00007728*/ LSR         R4, R2, #13 
    /*0000772c*/ ORR         R4, R4, R3, LSL #19 
    /*00007730*/ ASR         R5, R3, #13 
    /*00007734*/ RSBS        R4, R4, #0 
    /*00007738*/ RSC         R5, R5, #0 
    /*0000773c*/ B           hiisp_dis_setUnitMotionData_x76e4
FUNC_END hiisp_dis_setUnitMotionData


FUNC_BEGIN hiisp_dis_init
    /*00007740*/ MOV         R2, #1344                   @ 0x540 
    /*00007744*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00007748*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*0000774c*/ MUL         R6, R2, R0 
    /*00007750*/ LDR         R7, [R1, #72]               @ 0x48 
    /*00007754*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*00007758*/ SUB         SP, SP, #292                @ 0x124 
    /*0000775c*/ MOV         R4, R1 
    /*00007760*/ MOV         R1, #0 
    /*00007764*/ ADD         R3, R5, R6 
    /*00007768*/ STR         R0, [SP] 
    /*0000776c*/ UXTH        R7, R7 
    /*00007770*/ STR         R3, [SP, #8] 
    /*00007774*/ MOV         R0, R3 
    /*00007778*/ LDR         R8, [R4, #48]               @ 0x30 
    /*0000777c*/ LDR         R9, [R4, #52]               @ 0x34 
    /*00007780*/ MOV         SL, #768                    @ 0x300 
    /*00007784*/ LDR         FP, [R4, #80]               @ 0x50 
    /*00007788*/ BL          0x0000000000007788          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000778c*/ LDR         R1, [R4, #20] 
    /*00007790*/ ADD         R0, R7, R7, LSL #1 
    /*00007794*/ LDRB        IP, [R4] 
    /*00007798*/ MOV         R2, #31 
    /*0000779c*/ LSL         R0, R0, #1 
    /*000077a0*/ STRH        R0, [SP, #72]               @ 0x48 
    /*000077a4*/ STR         R1, [R4, #24] 
    /*000077a8*/ ADD         R0, SP, #92                 @ 0x5c 
    /*000077ac*/ MOV         R1, #0 
    /*000077b0*/ STRB        IP, [SP, #66]               @ 0x42 
    /*000077b4*/ STR         SL, [SP, #76]               @ 0x4c 
    /*000077b8*/ MOV         IP, #6 
    /*000077bc*/ STRH        IP, [SP, #64]               @ 0x40 
    /*000077c0*/ STRH        IP, [SP, #70]               @ 0x46 
    /*000077c4*/ MOV         IP, #552                    @ 0x228 
    /*000077c8*/ STRH        IP, [SP, #68]               @ 0x44 
    /*000077cc*/ MOV         IP, #121                    @ 0x79 
    /*000077d0*/ STR         IP, [SP, #84]               @ 0x54 
    /*000077d4*/ MOV         IP, #51                     @ 0x33 
    /*000077d8*/ STR         IP, [SP, #88]               @ 0x58 
    /*000077dc*/ BL          0x00000000000077DC          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000077e0*/ LDR         R2, [R4, #16] 
    /*000077e4*/ CMP         R2, #1 
    /*000077e8*/ BEQ         hiisp_dis_init_x7e44
    /*000077ec*/ BCC         hiisp_dis_init_x7e28
    /*000077f0*/ CMP         R2, #2 
    /*000077f4*/ BEQ         hiisp_dis_init_x7af4
    /*000077f8*/ MOV         R3, #160                    @ 0xa0 
    /*000077fc*/ MOV         R2, #8 
    /*00007800*/ STR         R3, [SP, #12] 
    /*00007804*/ STR         SL, [SP, #76]               @ 0x4c 
    /*00007808*/ STR         R2, [R4, #84]               @ 0x54 
    hiisp_dis_init_x780c:
    /*0000780c*/ LDR         R2, [R4, #68]               @ 0x44 
    /*00007810*/ CMP         R2, #0 
    /*00007814*/ BEQ         hiisp_dis_init_x7b18
    hiisp_dis_init_x7818:
    /*00007818*/ CMP         R2, #2 
    /*0000781c*/ BNE         hiisp_dis_init_x7c34
    /*00007820*/ LDR         R1, [SP, #76]               @ 0x4c 
    /*00007824*/ MOV         R2, #4 
    /*00007828*/ MOV         R3, #8 
    /*0000782c*/ STR         R3, [SP, #4] 
    /*00007830*/ LSL         R1, R1, #1 
    /*00007834*/ STR         R1, [SP, #76]               @ 0x4c 
    hiisp_dis_init_x7838:
    /*00007838*/ CMP         FP, #30 
    /*0000783c*/ BHI         hiisp_dis_init_x7b3c
    hiisp_dis_init_x7840:
    /*00007840*/ LDR         R3, [SP, #76]               @ 0x4c 
    /*00007844*/ LSL         R3, R3, #1 
    /*00007848*/ STR         R3, [SP, #76]               @ 0x4c 
    hiisp_dis_init_x784c:
    /*0000784c*/ CMP         R8, #1280                   @ 0x500 
    /*00007850*/ CMPCC       R9, #720                    @ 0x2d0 
    /*00007854*/ MOVCC       SL, #1 
    /*00007858*/ MOVCS       SL, #0 
    /*0000785c*/ BCS         hiisp_dis_init_x7b58
    /*00007860*/ CMP         R2, #1 
    /*00007864*/ MOVEQ       SL, #0 
    /*00007868*/ LSRNE       R2, R2, #1 
    /*0000786c*/ MOVNE       SL, #0 
    hiisp_dis_init_x7870:
    /*00007870*/ MOV         R1, R9 
    /*00007874*/ MOV         R0, R8 
    /*00007878*/ BL          0x0000000000007878          @ hiisp_dis_LimitDownSampleFactor [R_ARM_CALL: .text+0xd5b8] 
    /*0000787c*/ CMP         R0, #0 
    /*00007880*/ BEQ         hiisp_dis_init_x7e8c
    /*00007884*/ LDRB        R3, [R4] 
    /*00007888*/ LDR         R2, [SP, #76]               @ 0x4c 
    /*0000788c*/ CMP         R3, #4 
    /*00007890*/ LDR         R1, [R4, #104]              @ 0x68 
    /*00007894*/ STRHEQ      R3, [SP, #70]               @ 0x46 
    /*00007898*/ LSLEQ       R7, R7, #2 
    /*0000789c*/ LSLEQ       R2, R2, #1 
    /*000078a0*/ STREQ       R2, [SP, #76]               @ 0x4c 
    /*000078a4*/ STRHEQ      R7, [SP, #72]               @ 0x48 
    /*000078a8*/ CMP         R1, #0 
    /*000078ac*/ SMULL       R2, R3, R2, R2 
    /*000078b0*/ LSR         R2, R2, #10 
    /*000078b4*/ ORR         R2, R2, R3, LSL #22 
    /*000078b8*/ STR         R2, [SP, #80]               @ 0x50 
    /*000078bc*/ BNE         hiisp_dis_init_x7bd4
    /*000078c0*/ LDRH        R3, [R4, #144]              @ 0x90 
    /*000078c4*/ LDRB        R0, [R4, #112]              @ 0x70 
    /*000078c8*/ STR         R3, [SP, #24] 
    /*000078cc*/ LDR         R3, [R4, #116]              @ 0x74 
    /*000078d0*/ STR         R3, [SP, #4] 
    hiisp_dis_init_x78d4:
    /*000078d4*/ CMP         R0, #1 
    /*000078d8*/ MOV         R2, #60                     @ 0x3c 
    /*000078dc*/ ADD         R1, SP, #64                 @ 0x40 
    /*000078e0*/ ADD         R0, R4, #156                @ 0x9c 
    /*000078e4*/ MOVLS       R3, #0 
    /*000078e8*/ MOVHI       R3, #1 
    /*000078ec*/ STR         R3, [R4, #108]              @ 0x6c 
    /*000078f0*/ BL          0x00000000000078F0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000078f4*/ LDRB        R8, [R4, #112]              @ 0x70 
    /*000078f8*/ LDR         R7, [R4, #48]               @ 0x30 
    /*000078fc*/ CMP         R8, #0 
    /*00007900*/ SUB         R9, R7, #1 
    /*00007904*/ BEQ         hiisp_dis_init_x7bb0
    /*00007908*/ MOV         R0, R9 
    /*0000790c*/ MOV         R1, R8 
    /*00007910*/ BL          0x0000000000007910          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00007914*/ MOV         R1, R8 
    /*00007918*/ MOV         R9, R0 
    /*0000791c*/ LDR         R0, [R4, #52]               @ 0x34 
    /*00007920*/ ADD         R3, R9, #1 
    /*00007924*/ STR         R3, [R4, #128]              @ 0x80 
    /*00007928*/ SUB         R0, R0, #1 
    /*0000792c*/ STR         R3, [SP, #12] 
    /*00007930*/ BL          0x0000000000007930          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00007934*/ MOV         R3, R0 
    /*00007938*/ STR         R0, [SP, #28] 
    /*0000793c*/ LDRH        R0, [R4, #96]               @ 0x60 
    /*00007940*/ ADD         SL, R3, #1 
    /*00007944*/ STR         SL, [R4, #132]              @ 0x84 
    hiisp_dis_init_x7948:
    /*00007948*/ MOV         R1, R8 
    /*0000794c*/ BL          0x000000000000794C          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*00007950*/ MOV         R1, R8 
    /*00007954*/ STRH        R0, [R4, #96]               @ 0x60 
    /*00007958*/ STR         R0, [SP, #20] 
    /*0000795c*/ LDRH        R0, [R4, #98]               @ 0x62 
    /*00007960*/ BL          0x0000000000007960          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*00007964*/ MOV         R1, R8 
    /*00007968*/ MOV         FP, R0 
    /*0000796c*/ LDRH        R0, [R4, #100]              @ 0x64 
    /*00007970*/ STRH        FP, [R4, #98]               @ 0x62 
    /*00007974*/ SUB         R0, R0, #1 
    /*00007978*/ BL          0x0000000000007978          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000797c*/ LDRH        IP, [R4, #102]              @ 0x66 
    /*00007980*/ MOV         R1, R8 
    /*00007984*/ UXTH        R3, R0 
    /*00007988*/ SUB         R0, IP, #1 
    /*0000798c*/ ADD         R8, R3, #1 
    /*00007990*/ STR         R3, [SP, #16] 
    /*00007994*/ UXTH        R8, R8 
    /*00007998*/ STRH        R8, [R4, #100]              @ 0x64 
    /*0000799c*/ BL          0x000000000000799C          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*000079a0*/ LDR         R3, [SP, #12] 
    /*000079a4*/ LDR         R2, [SP, #20] 
    /*000079a8*/ TST         R3, #1 
    /*000079ac*/ LDR         R3, [SP, #16] 
    /*000079b0*/ STRNE       R9, [R4, #128]              @ 0x80 
    /*000079b4*/ LDREQ       R9, [R4, #128]              @ 0x80 
    /*000079b8*/ MOVNE       R8, R3 
    /*000079bc*/ STRHNE      R3, [R4, #100]              @ 0x64 
    /*000079c0*/ TST         SL, #1 
    /*000079c4*/ ADD         R8, R8, R2 
    /*000079c8*/ UXTH        R9, R9 
    /*000079cc*/ LDRNE       R3, [SP, #28] 
    /*000079d0*/ STRNE       R3, [R4, #132]              @ 0x84 
    /*000079d4*/ UXTH        R0, R0 
    /*000079d8*/ ADD         R1, R0, #1 
    /*000079dc*/ STRH        R1, [R4, #102]              @ 0x66 
    /*000079e0*/ STRHNE      R0, [R4, #102]              @ 0x66 
    /*000079e4*/ CMP         R8, R9 
    /*000079e8*/ BGT         hiisp_dis_init_x7e8c
    /*000079ec*/ LDRH        R3, [R4, #102]              @ 0x66 
    /*000079f0*/ LDRH        R2, [R4, #132]              @ 0x84 
    /*000079f4*/ ADD         FP, R3, FP 
    /*000079f8*/ CMP         FP, R2 
    /*000079fc*/ BGT         hiisp_dis_init_x7e8c
    /*00007a00*/ LSR         R7, R7, #1 
    /*00007a04*/ ADD         R1, R6, #76                 @ 0x4c 
    /*00007a08*/ ADD         R3, R7, #128                @ 0x80 
    /*00007a0c*/ STR         R7, [R4, #36]               @ 0x24 
    /*00007a10*/ STR         R3, [R4, #40]               @ 0x28 
    /*00007a14*/ ADD         R1, R5, R1 
    /*00007a18*/ MOV         R0, R4 
    /*00007a1c*/ SUB         R7, R7, #128                @ 0x80 
    /*00007a20*/ STR         R7, [R4, #44]               @ 0x2c 
    /*00007a24*/ BL          0x0000000000007A24          @ hiisp_dis_DSF_cfg_parse [R_ARM_CALL: .text+0xcd9c] 
    /*00007a28*/ ADD         R1, R6, #188                @ 0xbc 
    /*00007a2c*/ ADD         R1, R5, R1 
    /*00007a30*/ MOV         R0, R4 
    /*00007a34*/ BL          0x0000000000007A34          @ hiisp_dis_FPD_cfg_parse [R_ARM_CALL: .text+0xd108] 
    /*00007a38*/ ADD         R1, R6, #268                @ 0x10c 
    /*00007a3c*/ ADD         R1, R5, R1 
    /*00007a40*/ MOV         R0, R4 
    /*00007a44*/ BL          0x0000000000007A44          @ hiisp_dis_GME_cfg_parse [R_ARM_CALL: .text+0xd210] 
    /*00007a48*/ ADD         R3, R6, #424                @ 0x1a8 
    /*00007a4c*/ ADD         R0, R6, #288                @ 0x120 
    /*00007a50*/ ADD         R3, R5, R3 
    /*00007a54*/ ADD         R0, R5, R0 
    /*00007a58*/ MOV         R2, #6 
    /*00007a5c*/ LDR         R1, [R4, #140]              @ 0x8c 
    /*00007a60*/ BL          0x0000000000007A60          @ hiisp_dis_LDC_cfg_parse [R_ARM_CALL: .text+0xd3c0] 
    /*00007a64*/ LDR         R2, [SP] 
    /*00007a68*/ MOV         R3, #1344                   @ 0x540 
    /*00007a6c*/ MLA         R3, R3, R2, R5 
    /*00007a70*/ LDR         R2, [R4, #20] 
    /*00007a74*/ STR         R2, [R3, #304]              @ 0x130 
    /*00007a78*/ LDR         R3, [R4, #16] 
    /*00007a7c*/ LDR         R1, [R4, #132]              @ 0x84 
    /*00007a80*/ CMP         R3, #1 
    /*00007a84*/ LDR         R2, [R4, #128]              @ 0x80 
    /*00007a88*/ LDRH        R0, [R4, #120]              @ 0x78 
    /*00007a8c*/ MOVNE       R3, #0 
    /*00007a90*/ STR         R3, [SP, #32] 
    /*00007a94*/ LDR         R3, [SP, #4] 
    /*00007a98*/ STR         R1, [SP, #40]               @ 0x28 
    /*00007a9c*/ STR         R2, [SP, #36]               @ 0x24 
    /*00007aa0*/ ADD         R2, SP, #32 
    /*00007aa4*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00007aa8*/ ADD         R3, R6, #220                @ 0xdc 
    /*00007aac*/ ADD         R3, R5, R3 
    /*00007ab0*/ STR         R3, [SP, #52]               @ 0x34 
    /*00007ab4*/ ADD         R3, R6, #210                @ 0xd2 
    /*00007ab8*/ STRH        R0, [SP, #58]               @ 0x3a 
    /*00007abc*/ ADD         R3, R5, R3 
    /*00007ac0*/ STR         R3, [SP, #48]               @ 0x30 
    /*00007ac4*/ LDRH        R3, [SP, #24] 
    /*00007ac8*/ LDR         R1, [SP, #8] 
    /*00007acc*/ LDR         R0, [SP] 
    /*00007ad0*/ STRH        R3, [SP, #56]               @ 0x38 
    /*00007ad4*/ MOV         R3, #1 
    /*00007ad8*/ STRB        R3, [SP, #60]               @ 0x3c 
    /*00007adc*/ BL          0x0000000000007ADC          @ hiisp_dis_mergePtsVecs_fw_init [R_ARM_CALL: .text+0xe20c] 
    /*00007ae0*/ SUBS        R8, R0, #0 
    /*00007ae4*/ BEQ         hiisp_dis_init_x7c44
    hiisp_dis_init_x7ae8:
    /*00007ae8*/ MOV         R0, R8 
    /*00007aec*/ ADD         SP, SP, #292                @ 0x124 
    /*00007af0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    hiisp_dis_init_x7af4:
    /*00007af4*/ MOV         R2, #3072                   @ 0xc00 
    /*00007af8*/ STR         R2, [SP, #76]               @ 0x4c 
    /*00007afc*/ MOV         R2, #8 
    /*00007b00*/ STR         R2, [R4, #84]               @ 0x54 
    /*00007b04*/ LDR         R2, [R4, #68]               @ 0x44 
    /*00007b08*/ MOV         R3, #249                    @ 0xf9 
    /*00007b0c*/ STR         R3, [SP, #12] 
    /*00007b10*/ CMP         R2, #0 
    /*00007b14*/ BNE         hiisp_dis_init_x7818
    hiisp_dis_init_x7b18:
    /*00007b18*/ LDR         R1, [SP, #76]               @ 0x4c 
    /*00007b1c*/ CMP         FP, #30 
    /*00007b20*/ MOV         R3, #16 
    /*00007b24*/ MOV         R2, #1 
    /*00007b28*/ STR         R3, [SP, #4] 
    /*00007b2c*/ ADD         R1, R1, R1, LSR #31 
    /*00007b30*/ ASR         R1, R1, R2 
    /*00007b34*/ STR         R1, [SP, #76]               @ 0x4c 
    /*00007b38*/ BLS         hiisp_dis_init_x7840
    hiisp_dis_init_x7b3c:
    /*00007b3c*/ SUB         R3, FP, #31 
    /*00007b40*/ CMP         R3, #29 
    /*00007b44*/ LDRHI       R3, [SP, #76]               @ 0x4c 
    /*00007b48*/ ADDHI       R3, R3, R3, LSR #31 
    /*00007b4c*/ ASRHI       R3, R3, #1 
    /*00007b50*/ STRHI       R3, [SP, #76]               @ 0x4c 
    /*00007b54*/ B           hiisp_dis_init_x784c
    hiisp_dis_init_x7b58:
    /*00007b58*/ MOVW        R3, #1080                   @ 0x438 
    /*00007b5c*/ CMP         R8, #1920                   @ 0x780 
    /*00007b60*/ CMPLS       R9, R3 
    /*00007b64*/ MOVLS       R3, #1 
    /*00007b68*/ MOVHI       R3, #0 
    /*00007b6c*/ BLS         hiisp_dis_init_x7870
    /*00007b70*/ CMP         R8, #3840                   @ 0xf00 
    /*00007b74*/ CMPLS       R9, #2160                   @ 0x870 
    /*00007b78*/ BLS         hiisp_dis_init_x7e70
    /*00007b7c*/ MOVW        R3, #4320                   @ 0x10e0 
    /*00007b80*/ CMP         R8, #7680                   @ 0x1e00 
    /*00007b84*/ CMPLS       R9, R3 
    /*00007b88*/ BHI         hiisp_dis_init_x7e9c
    /*00007b8c*/ LSL         R2, R2, #2 
    /*00007b90*/ LDR         R3, [SP, #76]               @ 0x4c 
    /*00007b94*/ MOV         SL, #1 
    /*00007b98*/ UXTB        R2, R2 
    /*00007b9c*/ CMP         R2, #8 
    /*00007ba0*/ LSL         R3, R3, #2 
    /*00007ba4*/ STR         R3, [SP, #76]               @ 0x4c 
    /*00007ba8*/ MOVCS       R2, #8 
    /*00007bac*/ B           hiisp_dis_init_x7870
    hiisp_dis_init_x7bb0:
    /*00007bb0*/ LDR         SL, [R4, #52]               @ 0x34 
    /*00007bb4*/ MOV         R8, #1 
    /*00007bb8*/ STR         R7, [SP, #12] 
    /*00007bbc*/ SUB         R3, SL, #1 
    /*00007bc0*/ STR         R7, [R4, #128]              @ 0x80 
    /*00007bc4*/ STR         R3, [SP, #28] 
    /*00007bc8*/ LDRH        R0, [R4, #96]               @ 0x60 
    /*00007bcc*/ STR         SL, [R4, #132]              @ 0x84 
    /*00007bd0*/ B           hiisp_dis_init_x7948
    hiisp_dis_init_x7bd4:
    /*00007bd4*/ LDR         R3, [SP, #4] 
    /*00007bd8*/ MOV         R2, #200                    @ 0xc8 
    /*00007bdc*/ STRB        R0, [R4, #112]              @ 0x70 
    /*00007be0*/ STR         R2, [SP, #24] 
    /*00007be4*/ STR         R3, [R4, #116]              @ 0x74 
    /*00007be8*/ LDRB        R3, [SP, #12] 
    /*00007bec*/ STRH        R2, [R4, #144]              @ 0x90 
    /*00007bf0*/ MOV         R2, #30 
    /*00007bf4*/ STR         SL, [R4, #8] 
    /*00007bf8*/ STRB        R3, [R4, #152]              @ 0x98 
    /*00007bfc*/ MOV         R3, #300                    @ 0x12c 
    /*00007c00*/ STRH        R2, [R4, #146]              @ 0x92 
    /*00007c04*/ MOV         R2, #3 
    /*00007c08*/ STRH        R3, [R4, #120]              @ 0x78 
    /*00007c0c*/ STR         R3, [R4, #140]              @ 0x8c 
    /*00007c10*/ MOV         R3, #75                     @ 0x4b 
    /*00007c14*/ STRB        R2, [R4, #122]              @ 0x7a 
    /*00007c18*/ MOV         R2, #10 
    /*00007c1c*/ STR         R3, [R4, #148]              @ 0x94 
    /*00007c20*/ MOV         R3, #0 
    /*00007c24*/ STRB        R2, [R4, #136]              @ 0x88 
    /*00007c28*/ STRH        R3, [R4, #124]              @ 0x7c 
    /*00007c2c*/ STR         R3, [R4, #4] 
    /*00007c30*/ B           hiisp_dis_init_x78d4
    hiisp_dis_init_x7c34:
    /*00007c34*/ MOV         R3, #16 
    /*00007c38*/ MOV         R2, #2 
    /*00007c3c*/ STR         R3, [SP, #4] 
    /*00007c40*/ B           hiisp_dis_init_x7838
    hiisp_dis_init_x7c44:
    /*00007c44*/ ADD         R7, R6, #64                 @ 0x40 
    /*00007c48*/ LDRH        R1, [R4, #120]              @ 0x78 
    /*00007c4c*/ ADD         R7, R5, R7 
    /*00007c50*/ MOV         R0, R7 
    /*00007c54*/ BL          0x0000000000007C54          @ hiisp_dis_inlierOverlapRatio_fw_init [R_ARM_CALL: .text+0xe65c] 
    /*00007c58*/ SUBS        R8, R0, #0 
    /*00007c5c*/ BNE         hiisp_dis_init_x7e5c
    /*00007c60*/ LDR         R8, [R4, #224]              @ 0xe0 
    /*00007c64*/ MOVW        R2, #34079                  @ 0x851f 
    /*00007c68*/ MOVT        R2, #20971                  @ 0x51eb 
    /*00007c6c*/ LDR         LR, [R4, #360]              @ 0x168 
    /*00007c70*/ LDR         R1, [R4, #356]              @ 0x164 
    /*00007c74*/ MOVW        R0, #19923                  @ 0x4dd3 
    /*00007c78*/ LSL         R8, R8, #10 
    /*00007c7c*/ LDR         IP, [R4, #384]              @ 0x180 
    /*00007c80*/ LSL         LR, LR, #13 
    /*00007c84*/ MOVT        R0, #4194                   @ 0x1062 
    /*00007c88*/ ASR         R3, R8, #31 
    /*00007c8c*/ LSL         R1, R1, #13 
    /*00007c90*/ SMULL       R8, R9, R8, R2 
    /*00007c94*/ LSL         IP, IP, #13 
    /*00007c98*/ SMULL       SL, FP, LR, R2 
    /*00007c9c*/ LDR         R8, [R4, #416]              @ 0x1a0 
    /*00007ca0*/ ASR         LR, LR, #31 
    /*00007ca4*/ LDR         SL, [R4, #16] 
    /*00007ca8*/ RSB         R3, R3, R9, ASR #5 
    /*00007cac*/ STR         R3, [SP, #176]              @ 0xb0 
    /*00007cb0*/ SMULL       R2, R3, R1, R2 
    /*00007cb4*/ ASR         R2, R1, #31 
    /*00007cb8*/ SMULL       R0, R1, IP, R0 
    /*00007cbc*/ LDR         R0, [R4, #48]               @ 0x30 
    /*00007cc0*/ ASR         IP, IP, #31 
    /*00007cc4*/ STR         R8, [SP, #128]              @ 0x80 
    /*00007cc8*/ RSB         R2, R2, R3, ASR #5 
    /*00007ccc*/ LDR         R3, [R4, #64]               @ 0x40 
    /*00007cd0*/ LDR         R8, [R4, #52]               @ 0x34 
    /*00007cd4*/ RSB         LR, LR, FP, ASR #5 
    /*00007cd8*/ LDRB        R9, [R4] 
    /*00007cdc*/ RSB         IP, IP, R1, ASR #6 
    /*00007ce0*/ STR         LR, [SP, #184]              @ 0xb8 
    /*00007ce4*/ LDR         R1, [R4, #76]               @ 0x4c 
    /*00007ce8*/ STR         R2, [SP, #180]              @ 0xb4 
    /*00007cec*/ STR         IP, [SP, #256]              @ 0x100 
    /*00007cf0*/ LDR         R2, [R4, #68]               @ 0x44 
    /*00007cf4*/ LDR         IP, [R4, #300]              @ 0x12c 
    /*00007cf8*/ LDR         LR, [R4, #88]               @ 0x58 
    /*00007cfc*/ STR         R0, [SP, #136]              @ 0x88 
    /*00007d00*/ STR         R3, [SP, #160]              @ 0xa0 
    /*00007d04*/ LDR         R0, [R4, #80]               @ 0x50 
    /*00007d08*/ LDR         R3, [R4, #352]              @ 0x160 
    /*00007d0c*/ STR         R8, [SP, #140]              @ 0x8c 
    /*00007d10*/ STR         R2, [SP, #156]              @ 0x9c 
    /*00007d14*/ MOV         R2, #28 
    /*00007d18*/ STR         R1, [SP, #152]              @ 0x98 
    /*00007d1c*/ ADD         R1, R4, #388                @ 0x184 
    /*00007d20*/ STR         R0, [SP, #144]              @ 0x90 
    /*00007d24*/ ADD         R0, SP, #260                @ 0x104 
    /*00007d28*/ STR         IP, [SP, #148]              @ 0x94 
    /*00007d2c*/ STR         R3, [SP, #172]              @ 0xac 
    /*00007d30*/ STR         SL, [SP, #124]              @ 0x7c 
    /*00007d34*/ STRB        R9, [SP, #132]              @ 0x84 
    /*00007d38*/ STR         LR, [SP, #164]              @ 0xa4 
    /*00007d3c*/ LDRB        LR, [R4, #297]              @ 0x129 
    /*00007d40*/ LDR         R3, [R4, #308]              @ 0x134 
    /*00007d44*/ LDR         IP, [R4, #304]              @ 0x130 
    /*00007d48*/ STRB        LR, [SP, #168]              @ 0xa8 
    /*00007d4c*/ STR         R3, [SP, #188]              @ 0xbc 
    /*00007d50*/ LDR         R3, [R4, #312]              @ 0x138 
    /*00007d54*/ STR         IP, [SP, #192]              @ 0xc0 
    /*00007d58*/ LDR         IP, [R4, #316]              @ 0x13c 
    /*00007d5c*/ STR         R3, [SP, #196]              @ 0xc4 
    /*00007d60*/ LDR         R3, [R4, #320]              @ 0x140 
    /*00007d64*/ STR         IP, [SP, #200]              @ 0xc8 
    /*00007d68*/ LDR         IP, [R4, #324]              @ 0x144 
    /*00007d6c*/ STR         R3, [SP, #204]              @ 0xcc 
    /*00007d70*/ LDR         R3, [R4, #332]              @ 0x14c 
    /*00007d74*/ STR         IP, [SP, #208]              @ 0xd0 
    /*00007d78*/ LDR         IP, [R4, #336]              @ 0x150 
    /*00007d7c*/ STR         R3, [SP, #216]              @ 0xd8 
    /*00007d80*/ LDR         R3, [R4, #340]              @ 0x154 
    /*00007d84*/ STR         IP, [SP, #220]              @ 0xdc 
    /*00007d88*/ LDR         IP, [R4, #344]              @ 0x158 
    /*00007d8c*/ STR         R3, [SP, #224]              @ 0xe0 
    /*00007d90*/ LDRB        R3, [R4, #348]              @ 0x15c 
    /*00007d94*/ STR         IP, [SP, #228]              @ 0xe4 
    /*00007d98*/ LDR         IP, [R4, #364]              @ 0x16c 
    /*00007d9c*/ STRB        R3, [SP, #232]              @ 0xe8 
    /*00007da0*/ LDR         R3, [R4, #368]              @ 0x170 
    /*00007da4*/ STR         IP, [SP, #236]              @ 0xec 
    /*00007da8*/ LDR         IP, [R4, #372]              @ 0x174 
    /*00007dac*/ STR         R3, [SP, #240]              @ 0xf0 
    /*00007db0*/ LDR         R3, [R4, #376]              @ 0x178 
    /*00007db4*/ STR         IP, [SP, #244]              @ 0xf4 
    /*00007db8*/ STR         R3, [SP, #248]              @ 0xf8 
    /*00007dbc*/ LDR         R3, [R4, #380]              @ 0x17c 
    /*00007dc0*/ STR         R3, [SP, #252]              @ 0xfc 
    /*00007dc4*/ BL          0x0000000000007DC4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00007dc8*/ ADD         R1, R6, #472                @ 0x1d8 
    /*00007dcc*/ ADD         R2, SP, #124                @ 0x7c 
    /*00007dd0*/ ADD         R1, R5, R1 
    /*00007dd4*/ LDR         R0, [SP] 
    /*00007dd8*/ BL          0x0000000000007DD8          @ DIS_MOTION_FILTER_init [R_ARM_CALL: .text+0xbc54] 
    /*00007ddc*/ SUBS        R8, R0, #0 
    /*00007de0*/ BNE         hiisp_dis_init_x7eb4
    /*00007de4*/ LDR         R3, [R4, #88]               @ 0x58 
    /*00007de8*/ CMP         R3, #1 
    /*00007dec*/ BEQ         hiisp_dis_init_x7ec8
    /*00007df0*/ LDR         R2, [SP] 
    /*00007df4*/ MOV         R3, #1344                   @ 0x540 
    /*00007df8*/ MOV         R0, R8 
    /*00007dfc*/ MLA         R5, R3, R2, R5 
    /*00007e00*/ LDR         R3, [R5, #508]              @ 0x1fc 
    /*00007e04*/ ADD         R3, R3, #1 
    /*00007e08*/ BIC         R3, R3, #1 
    /*00007e0c*/ STR         R3, [R4, #56]               @ 0x38 
    /*00007e10*/ LDR         R3, [R5, #512]              @ 0x200 
    /*00007e14*/ ADD         R3, R3, #1 
    /*00007e18*/ BIC         R3, R3, #1 
    /*00007e1c*/ STR         R3, [R4, #60]               @ 0x3c 
    /*00007e20*/ ADD         SP, SP, #292                @ 0x124 
    /*00007e24*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    hiisp_dis_init_x7e28:
    /*00007e28*/ MOV         R2, #1536                   @ 0x600 
    /*00007e2c*/ MOV         R3, #243                    @ 0xf3 
    /*00007e30*/ STR         R2, [SP, #76]               @ 0x4c 
    /*00007e34*/ MOV         R2, #32 
    /*00007e38*/ STR         R3, [SP, #12] 
    /*00007e3c*/ STR         R2, [R4, #84]               @ 0x54 
    /*00007e40*/ B           hiisp_dis_init_x780c
    hiisp_dis_init_x7e44:
    /*00007e44*/ MOV         R2, #8 
    /*00007e48*/ MOV         R3, #160                    @ 0xa0 
    /*00007e4c*/ STR         SL, [SP, #76]               @ 0x4c 
    /*00007e50*/ STR         R2, [R4, #84]               @ 0x54 
    /*00007e54*/ STR         R3, [SP, #12] 
    /*00007e58*/ B           hiisp_dis_init_x780c
    hiisp_dis_init_x7e5c:
    /*00007e5c*/ LDR         R0, [SP, #8] 
    /*00007e60*/ BL          0x0000000000007E60          @ hiisp_dis_mergePtsVecs_fw_uinit [R_ARM_CALL: .text+0xe4c8] 
    /*00007e64*/ MOV         R0, R8 
    /*00007e68*/ ADD         SP, SP, #292                @ 0x124 
    /*00007e6c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    hiisp_dis_init_x7e70:
    /*00007e70*/ LDR         R1, [SP, #76]               @ 0x4c 
    /*00007e74*/ LSL         R2, R2, #1 
    /*00007e78*/ MOV         SL, R3 
    /*00007e7c*/ UXTB        R2, R2 
    /*00007e80*/ LSL         R3, R1, #1 
    /*00007e84*/ STR         R3, [SP, #76]               @ 0x4c 
    /*00007e88*/ B           hiisp_dis_init_x7870
    hiisp_dis_init_x7e8c:
    /*00007e8c*/ MVN         R8, #0 
    /*00007e90*/ MOV         R0, R8 
    /*00007e94*/ ADD         SP, SP, #292                @ 0x124 
    /*00007e98*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    hiisp_dis_init_x7e9c:
    /*00007e9c*/ LDR         R3, [SP, #76]               @ 0x4c 
    /*00007ea0*/ MOV         R2, #8 
    /*00007ea4*/ MOV         SL, #1 
    /*00007ea8*/ LSL         R3, R3, #3 
    /*00007eac*/ STR         R3, [SP, #76]               @ 0x4c 
    /*00007eb0*/ B           hiisp_dis_init_x7870
    hiisp_dis_init_x7eb4:
    /*00007eb4*/ LDR         R0, [SP, #8] 
    /*00007eb8*/ BL          0x0000000000007EB8          @ hiisp_dis_mergePtsVecs_fw_uinit [R_ARM_CALL: .text+0xe4c8] 
    /*00007ebc*/ MOV         R0, R7 
    /*00007ec0*/ BL          0x0000000000007EC0          @ hiisp_dis_inlierOverlapRatio_fw_uinit [R_ARM_CALL: .text+0xe708] 
    /*00007ec4*/ B           hiisp_dis_init_x7ae8
    hiisp_dis_init_x7ec8:
    /*00007ec8*/ LDR         R2, [R4, #48]               @ 0x30 
    /*00007ecc*/ LDR         R3, [R4, #52]               @ 0x34 
    /*00007ed0*/ STR         R2, [R4, #56]               @ 0x38 
    /*00007ed4*/ STR         R3, [R4, #60]               @ 0x3c 
    /*00007ed8*/ B           hiisp_dis_init_x7ae8
FUNC_END hiisp_dis_init


FUNC_BEGIN hiisp_dis_uninit
    /*00007edc*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00007ee0*/ MOV         R7, #1344                   @ 0x540 
    /*00007ee4*/ MUL         R4, R7, R0 
    /*00007ee8*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*00007eec*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*00007ef0*/ MOV         R8, R0 
    /*00007ef4*/ ADD         R6, R5, R4 
    /*00007ef8*/ MOV         R0, R6 
    /*00007efc*/ BL          0x0000000000007EFC          @ hiisp_dis_mergePtsVecs_fw_uinit [R_ARM_CALL: .text+0xe4c8] 
    /*00007f00*/ ADD         R0, R4, #64                 @ 0x40 
    /*00007f04*/ ADD         R0, R5, R0 
    /*00007f08*/ BL          0x0000000000007F08          @ hiisp_dis_inlierOverlapRatio_fw_uinit [R_ARM_CALL: .text+0xe708] 
    /*00007f0c*/ MOV         R0, R8 
    /*00007f10*/ BL          0x0000000000007F10          @ DIS_MOTION_FILTER_uninit [R_ARM_CALL: .text+0xc660] 
    /*00007f14*/ MOV         R2, R7 
    /*00007f18*/ MOV         R0, R6 
    /*00007f1c*/ MOV         R1, #0 
    /*00007f20*/ POP         {R4, R5, R6, R7, R8, LR} 
    hiisp_dis_uninit_x7f24:
    /*00007f24*/ B           hiisp_dis_uninit_x7f24      @ osal_memset [R_ARM_JUMP24: +0x0] 
FUNC_END hiisp_dis_uninit


FUNC_BEGIN hiisp_get_fpd_hw_cfg
    /*00007f28*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*00007f2c*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*00007f30*/ MOV         R3, #1344                   @ 0x540 
    /*00007f34*/ MLA         R0, R3, R0, R2 
    /*00007f38*/ ADD         R0, R0, #188                @ 0xbc 
    /*00007f3c*/ STR         R0, [R1] 
    /*00007f40*/ BX          LR 
FUNC_END hiisp_get_fpd_hw_cfg


FUNC_BEGIN hiisp_get_dsf_hw_cfg
    /*00007f44*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*00007f48*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*00007f4c*/ MOV         R3, #1344                   @ 0x540 
    /*00007f50*/ LDR         IP, [R1] 
    /*00007f54*/ MLA         R1, R3, R0, R2 
    /*00007f58*/ MOV         R2, #112                    @ 0x70 
    /*00007f5c*/ MOV         R0, IP 
    /*00007f60*/ ADD         R1, R1, #76                 @ 0x4c 
    hiisp_get_dsf_hw_cfg_x7f64:
    /*00007f64*/ B           hiisp_get_dsf_hw_cfg_x7f64  @ osal_memcpy [R_ARM_JUMP24: +0x0] 
FUNC_END hiisp_get_dsf_hw_cfg


FUNC_BEGIN hiisp_get_gme_hw_cfg
    /*00007f68*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*00007f6c*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*00007f70*/ MOV         R3, #1344                   @ 0x540 
    /*00007f74*/ MLA         R0, R3, R0, R2 
    /*00007f78*/ ADD         R0, R0, #268                @ 0x10c 
    /*00007f7c*/ BX          LR 
FUNC_END hiisp_get_gme_hw_cfg


FUNC_BEGIN hiisp_get_motion_filter
    /*00007f80*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*00007f84*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*00007f88*/ MOV         R3, #1344                   @ 0x540 
    /*00007f8c*/ MLA         R0, R3, R0, R2 
    /*00007f90*/ ADD         R0, R0, #472                @ 0x1d8 
    /*00007f94*/ BX          LR 
FUNC_END hiisp_get_motion_filter


FUNC_BEGIN DisSetGmeMotion
    /*00007f98*/ CMP         R1, #0 
    /*00007f9c*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00007fa0*/ SUB         SP, SP, #12 
    /*00007fa4*/ BEQ         DisSetGmeMotion_x800c
    /*00007fa8*/ CMP         R2, #0 
    /*00007fac*/ BEQ         DisSetGmeMotion_x803c
    /*00007fb0*/ MOVW        IP, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*00007fb4*/ MOVT        IP, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*00007fb8*/ MOV         LR, #1344                   @ 0x540 
    /*00007fbc*/ MLA         R3, LR, R0, IP 
    /*00007fc0*/ LDR         R3, [R3, #1340]             @ 0x53c 
    /*00007fc4*/ CMP         R3, #0 
    /*00007fc8*/ BEQ         DisSetGmeMotion_x7fd8
    /*00007fcc*/ ADD         SP, SP, #12 
    /*00007fd0*/ POP         {LR}                        @ (ldr lr, [sp], #4) 
    /*00007fd4*/ BX          R3 
    DisSetGmeMotion_x7fd8:
    /*00007fd8*/ MOVW        R3, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2488] 
    /*00007fdc*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x574] 
    /*00007fe0*/ MOVT        R3, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2488] 
    /*00007fe4*/ MOVW        R1, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2394] 
    /*00007fe8*/ STR         R3, [SP] 
    /*00007fec*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2400] 
    /*00007ff0*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x574] 
    /*00007ff4*/ MOVT        R1, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2394] 
    /*00007ff8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2400] 
    /*00007ffc*/ MOVW        R3, #587                    @ 0x24b 
    /*00008000*/ BL          0x0000000000008000          @ osal_printk [R_ARM_CALL: +0x0] 
    DisSetGmeMotion_x8004:
    /*00008004*/ ADD         SP, SP, #12 
    /*00008008*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    DisSetGmeMotion_x800c:
    /*0000800c*/ MOVW        R3, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2458] 
    /*00008010*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x574] 
    /*00008014*/ MOVT        R3, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2458] 
    /*00008018*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x574] 
    /*0000801c*/ STR         R3, [SP] 
    /*00008020*/ MOVW        R1, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2394] 
    /*00008024*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2400] 
    /*00008028*/ MOVT        R1, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2394] 
    /*0000802c*/ MOVW        R3, #581                    @ 0x245 
    /*00008030*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2400] 
    /*00008034*/ BL          0x0000000000008034          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00008038*/ B           DisSetGmeMotion_x8004
    DisSetGmeMotion_x803c:
    /*0000803c*/ MOVW        R3, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2470] 
    /*00008040*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x574] 
    /*00008044*/ MOVT        R3, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2470] 
    /*00008048*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x574] 
    /*0000804c*/ STR         R3, [SP] 
    /*00008050*/ MOVW        R1, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2394] 
    /*00008054*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2400] 
    /*00008058*/ MOVT        R1, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2394] 
    /*0000805c*/ MOVW        R3, #582                    @ 0x246 
    /*00008060*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2400] 
    /*00008064*/ BL          0x0000000000008064          @ osal_printk [R_ARM_CALL: +0x0] 
    /*00008068*/ B           DisSetGmeMotion_x8004
FUNC_END DisSetGmeMotion


FUNC_BEGIN DisGetStabilizedMotion
    /*0000806c*/ CMP         R1, #0 
    /*00008070*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00008074*/ SUB         SP, SP, #12 
    /*00008078*/ BEQ         DisGetStabilizedMotion_x80d4
    /*0000807c*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1f8] 
    /*00008080*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1f8] 
    /*00008084*/ MOV         IP, #1344                   @ 0x540 
    /*00008088*/ MLA         R3, IP, R0, R2 
    /*0000808c*/ LDR         R3, [R3, #1332]             @ 0x534 
    /*00008090*/ CMP         R3, #0 
    /*00008094*/ BEQ         DisGetStabilizedMotion_x80a4
    /*00008098*/ ADD         SP, SP, #12 
    /*0000809c*/ POP         {LR}                        @ (ldr lr, [sp], #4) 
    /*000080a0*/ BX          R3 
    DisGetStabilizedMotion_x80a4:
    /*000080a4*/ MOVW        R3, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24d0] 
    /*000080a8*/ MOVW        R1, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2394] 
    /*000080ac*/ MOVT        R3, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24d0] 
    /*000080b0*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2400] 
    /*000080b4*/ STR         R3, [SP] 
    /*000080b8*/ MOVT        R1, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2394] 
    /*000080bc*/ MOVW        R3, #601                    @ 0x259 
    /*000080c0*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2400] 
    /*000080c4*/ LDR         R2, [PC, #52]               @ 0x0000000000008100 
    /*000080c8*/ BL          0x00000000000080C8          @ osal_printk [R_ARM_CALL: +0x0] 
    DisGetStabilizedMotion_x80cc:
    /*000080cc*/ ADD         SP, SP, #12 
    /*000080d0*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    DisGetStabilizedMotion_x80d4:
    /*000080d4*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x24b4] 
    /*000080d8*/ MOVW        R1, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2394] 
    /*000080dc*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x24b4] 
    /*000080e0*/ MOVT        R1, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2394] 
    /*000080e4*/ STR         R3, [SP] 
    /*000080e8*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2400] 
    /*000080ec*/ MOV         R3, #596                    @ 0x254 
    /*000080f0*/ LDR         R2, [PC, #8]                @ 0x0000000000008100 
    /*000080f4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2400] 
    /*000080f8*/ BL          0x00000000000080F8          @ osal_printk [R_ARM_CALL: +0x0] 
    /*000080fc*/ B           DisGetStabilizedMotion_x80cc
    /*00008100*/ ANDEQ       R0, R0, R4, LSL #11 
FUNC_END DisGetStabilizedMotion


FUNC_BEGIN inlierOverlapRatio
    /*00008104*/ PUSH        {R4, LR} 
    /*00008108*/ ADD         R4, R1, #296                @ 0x128 
    /*0000810c*/ SUB         R2, R1, #1 
    /*00008110*/ SUB         R0, R0, #1 
    /*00008114*/ MOV         R1, #0 
    /*00008118*/ ADD         R4, R4, #3 
    /*0000811c*/ MOV         IP, R1 
    inlierOverlapRatio_x8120:
    /*00008120*/ LDRB        R3, [R2, #1]! 
    /*00008124*/ LDRB        LR, [R0, #1]! 
    /*00008128*/ CMP         R4, R2 
    /*0000812c*/ ADD         R1, R1, R3 
    /*00008130*/ AND         R3, R3, LR 
    /*00008134*/ ADD         IP, IP, R3 
    /*00008138*/ BNE         inlierOverlapRatio_x8120
    /*0000813c*/ CMP         R1, #0 
    /*00008140*/ CMPNE       IP, #0 
    /*00008144*/ MOVNE       R0, #1 
    /*00008148*/ MOVEQ       R0, #0 
    /*0000814c*/ POPEQ       {R4, PC} 
    /*00008150*/ LSL         R0, IP, #13 
    /*00008154*/ BL          0x0000000000008154          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*00008158*/ POP         {R4, PC} 
FUNC_END inlierOverlapRatio


FUNC_BEGIN attenuateMotion
    /*0000815c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00008160*/ SUBS        R5, R1, #0 
    /*00008164*/ SUB         SP, SP, #28 
    /*00008168*/ BLE         attenuateMotion_x82dc
    /*0000816c*/ RSB         R3, R5, #8192               @ 0x2000 
    /*00008170*/ LDR         R6, [R0] 
    /*00008174*/ LDR         R1, [R0, #4] 
    /*00008178*/ LSL         R5, R5, #7 
    /*0000817c*/ ASR         R7, R3, #31 
    /*00008180*/ MOV         SL, R3 
    /*00008184*/ LDR         LR, [R0, #36]               @ 0x24 
    /*00008188*/ MOV         FP, R7 
    /*0000818c*/ STRD        SL, [SP] 
    /*00008190*/ UMULL       SL, FP, R6, R3 
    /*00008194*/ LDR         R4, [R0, #32] 
    /*00008198*/ MUL         R1, R3, R1 
    /*0000819c*/ LDR         IP, [R0, #12] 
    /*000081a0*/ MLA         R1, R6, R7, R1 
    /*000081a4*/ LDR         R7, [SP, #4] 
    /*000081a8*/ LSR         R8, SL, #13 
    /*000081ac*/ MOV         SL, R5 
    /*000081b0*/ MUL         LR, R3, LR 
    /*000081b4*/ ADD         FP, R1, FP 
    /*000081b8*/ MLA         LR, R4, R7, LR 
    /*000081bc*/ ORR         R8, R8, FP, LSL #19 
    /*000081c0*/ ASR         R9, FP, #13 
    /*000081c4*/ ASR         FP, R5, #31 
    /*000081c8*/ LDR         R5, [R0, #8] 
    /*000081cc*/ STRD        SL, [SP, #8] 
    /*000081d0*/ ADDS        SL, SL, R8 
    /*000081d4*/ ADC         FP, FP, R9 
    /*000081d8*/ LDR         R2, [R0, #28] 
    /*000081dc*/ MOV         R7, R5 
    /*000081e0*/ MOV         R8, SL 
    /*000081e4*/ UMULL       R4, R5, R4, R3 
    /*000081e8*/ MOV         R9, FP 
    /*000081ec*/ LDRD        SL, [SP] 
    /*000081f0*/ ADD         R5, LR, R5 
    /*000081f4*/ LDR         LR, [R0, #24] 
    /*000081f8*/ MUL         IP, R3, IP 
    /*000081fc*/ STRD        R8, [R0] 
    /*00008200*/ MUL         R2, R3, R2 
    /*00008204*/ LSR         R8, R4, #13 
    /*00008208*/ MLA         IP, R7, FP, IP 
    /*0000820c*/ STR         R7, [SP, #20] 
    /*00008210*/ MLA         R2, LR, FP, R2 
    /*00008214*/ LDRD        SL, [SP, #8] 
    /*00008218*/ ORR         R8, R8, R5, LSL #19 
    /*0000821c*/ ASR         R9, R5, #13 
    /*00008220*/ ADDS        SL, SL, R8 
    /*00008224*/ LDR         R5, [SP, #20] 
    /*00008228*/ ADC         FP, FP, R9 
    /*0000822c*/ LDR         R1, [R0, #20] 
    /*00008230*/ MOV         R8, SL 
    /*00008234*/ LDR         R7, [R0, #16] 
    /*00008238*/ MOV         R9, FP 
    /*0000823c*/ STRD        R8, [R0, #32] 
    /*00008240*/ UMULL       R8, R9, R5, R3 
    /*00008244*/ LDRD        R4, [SP] 
    /*00008248*/ LDR         R4, [R0, #24] 
    /*0000824c*/ MUL         R6, R3, R1 
    /*00008250*/ LDR         R1, [R0, #44]               @ 0x2c 
    /*00008254*/ UMULL       SL, FP, R4, R3 
    /*00008258*/ LDR         LR, [R0, #40]               @ 0x28 
    /*0000825c*/ MUL         R1, R3, R1 
    /*00008260*/ ADD         FP, R2, FP 
    /*00008264*/ MLA         R1, LR, R5, R1 
    /*00008268*/ ADD         R9, IP, R9 
    /*0000826c*/ MLA         R6, R7, R5, R6 
    /*00008270*/ LSR         IP, R8, #13 
    /*00008274*/ UMULL       R4, R5, R7, R3 
    /*00008278*/ ORR         IP, IP, R9, LSL #19 
    /*0000827c*/ UMULL       R2, R3, LR, R3 
    /*00008280*/ ADD         R5, R6, R5 
    /*00008284*/ STR         IP, [R0, #8] 
    /*00008288*/ LSR         IP, SL, #13 
    /*0000828c*/ ASR         LR, R9, #13 
    /*00008290*/ ORR         IP, IP, FP, LSL #19 
    /*00008294*/ STR         LR, [R0, #12] 
    /*00008298*/ ASR         LR, FP, #13 
    /*0000829c*/ STR         IP, [R0, #24] 
    /*000082a0*/ ASR         IP, R5, #13 
    /*000082a4*/ STRD        R2, [SP] 
    /*000082a8*/ ADD         R3, R1, R3 
    /*000082ac*/ STR         R3, [SP, #4] 
    /*000082b0*/ LSR         R1, R4, #13 
    /*000082b4*/ LDRD        R6, [SP] 
    /*000082b8*/ ORR         R1, R1, R5, LSL #19 
    /*000082bc*/ STR         LR, [R0, #28] 
    /*000082c0*/ STR         R1, [R0, #16] 
    /*000082c4*/ LSR         R2, R6, #13 
    /*000082c8*/ STR         IP, [R0, #20] 
    /*000082cc*/ ORR         R2, R2, R7, LSL #19 
    /*000082d0*/ ASR         R3, R7, #13 
    /*000082d4*/ STR         R2, [R0, #40]               @ 0x28 
    /*000082d8*/ STR         R3, [R0, #44]               @ 0x2c 
    attenuateMotion_x82dc:
    /*000082dc*/ ADD         SP, SP, #28 
    /*000082e0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
FUNC_END attenuateMotion


FUNC_BEGIN interpolateMotion
    /*000082e4*/ CMP         R2, #8192                   @ 0x2000 
    /*000082e8*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000082ec*/ MOV         LR, R3 
    /*000082f0*/ SUB         SP, SP, #28 
    /*000082f4*/ BHI         interpolateMotion_x8584
    /*000082f8*/ MOV         IP, R2 
    /*000082fc*/ MOV         SL, #8192                   @ 0x2000 
    /*00008300*/ ASR         R3, IP, #31 
    /*00008304*/ MOV         R4, IP 
    /*00008308*/ MOV         FP, #0 
    /*0000830c*/ LDR         R6, [R0, #4] 
    /*00008310*/ MOV         R5, R3 
    /*00008314*/ STRD        R4, [SP] 
    /*00008318*/ SUBS        R4, SL, IP 
    /*0000831c*/ LDR         R3, [R1, #4] 
    /*00008320*/ SBC         R5, FP, R5 
    /*00008324*/ LDR         R2, [R0] 
    /*00008328*/ MOV         SL, R4 
    /*0000832c*/ LDR         R4, [R1] 
    /*00008330*/ MUL         R3, SL, R3 
    /*00008334*/ MOV         FP, R5 
    /*00008338*/ MLA         R3, R4, R5, R3 
    /*0000833c*/ LDR         R7, [SP, #4] 
    /*00008340*/ UMULL       R4, R5, R4, SL 
    /*00008344*/ MUL         R6, IP, R6 
    /*00008348*/ ADD         R5, R3, R5 
    /*0000834c*/ MLA         R6, R2, R7, R6 
    /*00008350*/ ASR         R9, R5, #13 
    /*00008354*/ UMULL       R2, R3, R2, IP 
    /*00008358*/ LSR         R8, R4, #13 
    /*0000835c*/ ORR         R8, R8, R5, LSL #19 
    /*00008360*/ LDR         R5, [SP, #4] 
    /*00008364*/ ADD         R3, R6, R3 
    /*00008368*/ LSR         R4, R2, #13 
    /*0000836c*/ ORR         R4, R4, R3, LSL #19 
    /*00008370*/ ASR         R3, R3, #13 
    /*00008374*/ STR         R4, [SP, #8] 
    /*00008378*/ STR         R3, [SP, #12] 
    /*0000837c*/ LDRD        R6, [SP, #8] 
    /*00008380*/ ADDS        R6, R6, R8 
    /*00008384*/ ADC         R7, R7, R9 
    /*00008388*/ STRD        R6, [LR] 
    /*0000838c*/ LDR         R3, [R0, #12] 
    /*00008390*/ LDR         R4, [R0, #8] 
    /*00008394*/ LDR         R6, [R1, #12] 
    /*00008398*/ MUL         R3, IP, R3 
    /*0000839c*/ LDR         R2, [R1, #8] 
    /*000083a0*/ MLA         R3, R4, R5, R3 
    /*000083a4*/ UMULL       R4, R5, R4, IP 
    /*000083a8*/ MUL         R6, SL, R6 
    /*000083ac*/ ADD         R5, R3, R5 
    /*000083b0*/ MLA         R6, R2, FP, R6 
    /*000083b4*/ UMULL       R2, R3, R2, SL 
    /*000083b8*/ LSR         R7, R4, #13 
    /*000083bc*/ ORR         R7, R7, R5, LSL #19 
    /*000083c0*/ ASR         R5, R5, #13 
    /*000083c4*/ ADD         R3, R6, R3 
    /*000083c8*/ STR         R5, [SP, #20] 
    /*000083cc*/ STR         R7, [SP, #16] 
    /*000083d0*/ LDRD        R6, [SP, #16] 
    /*000083d4*/ ASR         R9, R3, #13 
    /*000083d8*/ LSR         R8, R2, #13 
    /*000083dc*/ LDR         R5, [SP, #4] 
    /*000083e0*/ ORR         R8, R8, R3, LSL #19 
    /*000083e4*/ ADDS        R6, R6, R8 
    /*000083e8*/ ADC         R7, R7, R9 
    /*000083ec*/ MOV         R8, R6 
    /*000083f0*/ MOV         R9, R7 
    /*000083f4*/ STRD        R8, [LR, #8] 
    /*000083f8*/ LDR         R3, [R0, #28] 
    /*000083fc*/ LDR         R4, [R0, #24] 
    /*00008400*/ LDR         R2, [R1, #28] 
    /*00008404*/ MUL         R3, IP, R3 
    /*00008408*/ LDR         R6, [R1, #24] 
    /*0000840c*/ MLA         R3, R4, R5, R3 
    /*00008410*/ UMULL       R4, R5, R4, IP 
    /*00008414*/ MUL         R2, SL, R2 
    /*00008418*/ ADD         R5, R3, R5 
    /*0000841c*/ LDR         R3, [R1, #24] 
    /*00008420*/ MLA         R6, R6, FP, R2 
    /*00008424*/ LSR         R7, R4, #13 
    /*00008428*/ UMULL       R2, R3, R3, SL 
    /*0000842c*/ ORR         R7, R7, R5, LSL #19 
    /*00008430*/ ASR         R4, R5, #13 
    /*00008434*/ STR         R7, [SP, #8] 
    /*00008438*/ ADD         R3, R6, R3 
    /*0000843c*/ STR         R4, [SP, #12] 
    /*00008440*/ LDRD        R6, [SP, #8] 
    /*00008444*/ ASR         R9, R3, #13 
    /*00008448*/ LSR         R8, R2, #13 
    /*0000844c*/ ORR         R8, R8, R3, LSL #19 
    /*00008450*/ ADDS        R6, R6, R8 
    /*00008454*/ ADC         R7, R7, R9 
    /*00008458*/ STRD        R6, [LR, #24] 
    /*0000845c*/ LDR         R5, [R1, #36]               @ 0x24 
    /*00008460*/ LDR         R3, [R0, #36]               @ 0x24 
    /*00008464*/ LDR         R4, [R0, #32] 
    /*00008468*/ MUL         R8, SL, R5 
    /*0000846c*/ LDR         R2, [R1, #32] 
    /*00008470*/ LDR         R5, [SP, #4] 
    /*00008474*/ MUL         R3, IP, R3 
    /*00008478*/ MLA         R3, R4, R5, R3 
    /*0000847c*/ UMULL       R4, R5, R4, IP 
    /*00008480*/ MLA         R8, R2, FP, R8 
    /*00008484*/ ADD         R5, R3, R5 
    /*00008488*/ UMULL       R2, R3, R2, SL 
    /*0000848c*/ LSR         R9, R4, #13 
    /*00008490*/ ORR         R9, R9, R5, LSL #19 
    /*00008494*/ ASR         R5, R5, #13 
    /*00008498*/ ADD         R3, R8, R3 
    /*0000849c*/ STR         R9, [SP, #16] 
    /*000084a0*/ STR         R5, [SP, #20] 
    /*000084a4*/ ASR         R7, R3, #13 
    /*000084a8*/ LDR         R5, [SP, #4] 
    /*000084ac*/ LSR         R6, R2, #13 
    /*000084b0*/ ORR         R6, R6, R3, LSL #19 
    /*000084b4*/ LDRD        R2, [SP, #16] 
    /*000084b8*/ ADDS        R2, R2, R6 
    /*000084bc*/ ADC         R3, R3, R7 
    /*000084c0*/ STRD        R2, [LR, #32] 
    /*000084c4*/ LDR         R3, [R0, #20] 
    /*000084c8*/ LDR         R4, [R0, #16] 
    /*000084cc*/ LDR         R6, [R1, #20] 
    /*000084d0*/ MUL         R3, IP, R3 
    /*000084d4*/ LDR         R2, [R1, #16] 
    /*000084d8*/ MLA         R3, R4, R5, R3 
    /*000084dc*/ UMULL       R4, R5, R4, IP 
    /*000084e0*/ MUL         R6, SL, R6 
    /*000084e4*/ ADD         R5, R3, R5 
    /*000084e8*/ MLA         R6, R2, FP, R6 
    /*000084ec*/ UMULL       R2, R3, R2, SL 
    /*000084f0*/ ADD         R3, R6, R3 
    /*000084f4*/ LSR         R6, R4, #13 
    /*000084f8*/ ORR         R6, R6, R5, LSL #19 
    /*000084fc*/ ASR         R4, R5, #13 
    /*00008500*/ STR         R6, [SP, #8] 
    /*00008504*/ ASR         R9, R3, #13 
    /*00008508*/ STR         R4, [SP, #12] 
    /*0000850c*/ LSR         R8, R2, #13 
    /*00008510*/ LDRD        R6, [SP, #8] 
    /*00008514*/ ORR         R8, R8, R3, LSL #19 
    /*00008518*/ ADDS        R6, R6, R8 
    /*0000851c*/ ADC         R7, R7, R9 
    /*00008520*/ STRD        R6, [LR, #16] 
    /*00008524*/ LDR         R3, [R1, #44]               @ 0x2c 
    /*00008528*/ LDR         R8, [R0, #44]               @ 0x2c 
    /*0000852c*/ LDR         R2, [R0, #40]               @ 0x28 
    /*00008530*/ LDR         R1, [R1, #40]               @ 0x28 
    /*00008534*/ MUL         R0, SL, R3 
    /*00008538*/ LDR         R3, [SP, #4] 
    /*0000853c*/ MUL         R8, IP, R8 
    /*00008540*/ MLA         R0, R1, FP, R0 
    /*00008544*/ MLA         R8, R2, R3, R8 
    /*00008548*/ UMULL       SL, FP, R1, SL 
    /*0000854c*/ UMULL       R2, R3, R2, IP 
    /*00008550*/ ADD         FP, R0, FP 
    /*00008554*/ ADD         R3, R8, R3 
    /*00008558*/ LSR         R6, SL, #13 
    /*0000855c*/ ORR         R6, R6, FP, LSL #19 
    /*00008560*/ ASR         R7, FP, #13 
    /*00008564*/ LSR         R1, R2, #13 
    /*00008568*/ ORR         R1, R1, R3, LSL #19 
    /*0000856c*/ ASR         R3, R3, #13 
    /*00008570*/ ADDS        R4, R1, R6 
    /*00008574*/ ADC         R5, R3, R7 
    /*00008578*/ STRD        R4, [LR, #40]               @ 0x28 
    /*0000857c*/ ADD         SP, SP, #28 
    /*00008580*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    interpolateMotion_x8584:
    /*00008584*/ MOV         R0, R3 
    /*00008588*/ ADD         SP, SP, #28 
    /*0000858c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    interpolateMotion_x8590:
    /*00008590*/ B           interpolateMotion_x8590     @ DIS_resetMotionModel [R_ARM_JUMP24: .text+0x6b70] 
FUNC_END interpolateMotion


FUNC_BEGIN inverseModel
    /*00008594*/ PUSH        {R3, R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00008598*/ LDR         LR, [R0, #32] 
    /*0000859c*/ LDR         R3, [R0, #4] 
    /*000085a0*/ LDR         R4, [R0] 
    /*000085a4*/ LDR         IP, [R0, #36]               @ 0x24 
    /*000085a8*/ MUL         R3, LR, R3 
    /*000085ac*/ LDR         R2, [R0, #8] 
    /*000085b0*/ MLA         R3, IP, R4, R3 
    /*000085b4*/ LDR         R6, [R0, #28] 
    /*000085b8*/ UMULL       R4, R5, R4, LR 
    /*000085bc*/ LDR         R7, [R0, #24] 
    /*000085c0*/ LDR         R8, [R0, #12] 
    /*000085c4*/ MUL         R6, R2, R6 
    /*000085c8*/ ADD         R5, R3, R5 
    /*000085cc*/ MLA         R8, R7, R8, R6 
    /*000085d0*/ UMULL       R2, R3, R2, R7 
    /*000085d4*/ SUBS        R2, R4, R2 
    /*000085d8*/ ADD         R3, R8, R3 
    /*000085dc*/ SBC         R3, R5, R3 
    /*000085e0*/ MOV         R4, #1024                   @ 0x400 
    /*000085e4*/ LSR         R6, R2, #20 
    /*000085e8*/ MOV         R5, #0 
    /*000085ec*/ ORR         R6, R6, R3, LSL #12 
    /*000085f0*/ ASR         R7, R3, #20 
    /*000085f4*/ CMP         R4, R6 
    /*000085f8*/ SBCS        R3, R5, R7 
    /*000085fc*/ POPGE       {R3, R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    /*00008600*/ MOV         R4, R0 
    /*00008604*/ MOV         R5, R1 
    /*00008608*/ MOV         R0, LR 
    /*0000860c*/ MOV         R1, IP 
    /*00008610*/ MOV         R2, #20 
    /*00008614*/ BL          0x0000000000008614          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*00008618*/ MOV         R3, R7 
    /*0000861c*/ MOV         R2, R6 
    /*00008620*/ BL          0x0000000000008620          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*00008624*/ MOV         R2, #20 
    /*00008628*/ STRD        R0, [R5] 
    /*0000862c*/ LDRD        R0, [R4, #8] 
    /*00008630*/ BL          0x0000000000008630          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*00008634*/ MOV         R2, R6 
    /*00008638*/ MOV         R3, R7 
    /*0000863c*/ RSBS        R0, R0, #0 
    /*00008640*/ RSC         R1, R1, #0 
    /*00008644*/ BL          0x0000000000008644          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*00008648*/ MOV         R3, R7 
    /*0000864c*/ MOV         R2, R6 
    /*00008650*/ STRD        R0, [R5, #8] 
    /*00008654*/ LDR         R9, [R4, #8] 
    /*00008658*/ LDR         R1, [R4, #44]               @ 0x2c 
    /*0000865c*/ LDR         R8, [R4, #40]               @ 0x28 
    /*00008660*/ LDR         FP, [R4, #12] 
    /*00008664*/ MUL         R1, R9, R1 
    /*00008668*/ LDR         SL, [R4, #16] 
    /*0000866c*/ LDR         LR, [R4, #36]               @ 0x24 
    /*00008670*/ MLA         R1, R8, FP, R1 
    /*00008674*/ LDR         R0, [R4, #32] 
    /*00008678*/ UMULL       R8, R9, R9, R8 
    /*0000867c*/ LDR         IP, [R4, #20] 
    /*00008680*/ MUL         LR, SL, LR 
    /*00008684*/ ADD         R9, R1, R9 
    /*00008688*/ MLA         IP, R0, IP, LR 
    /*0000868c*/ UMULL       R0, R1, SL, R0 
    /*00008690*/ ADD         R1, IP, R1 
    /*00008694*/ SUBS        R0, R8, R0 
    /*00008698*/ SBC         R1, R9, R1 
    /*0000869c*/ BL          0x000000000000869C          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*000086a0*/ MOV         R2, #20 
    /*000086a4*/ STRD        R0, [R5, #16] 
    /*000086a8*/ LDRD        R0, [R4, #24] 
    /*000086ac*/ BL          0x00000000000086AC          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*000086b0*/ MOV         R3, R7 
    /*000086b4*/ MOV         R2, R6 
    /*000086b8*/ RSBS        R0, R0, #0 
    /*000086bc*/ RSC         R1, R1, #0 
    /*000086c0*/ BL          0x00000000000086C0          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*000086c4*/ MOV         R2, #20 
    /*000086c8*/ STRD        R0, [R5, #24] 
    /*000086cc*/ LDRD        R0, [R4] 
    /*000086d0*/ BL          0x00000000000086D0          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*000086d4*/ MOV         R2, R6 
    /*000086d8*/ MOV         R3, R7 
    /*000086dc*/ BL          0x00000000000086DC          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*000086e0*/ MOV         R3, R7 
    /*000086e4*/ MOV         R2, R6 
    /*000086e8*/ STRD        R0, [R5, #32] 
    /*000086ec*/ LDR         R9, [R4, #24] 
    /*000086f0*/ LDR         R8, [R4, #20] 
    /*000086f4*/ LDR         R7, [R4, #4] 
    /*000086f8*/ LDR         R1, [R4, #40]               @ 0x28 
    /*000086fc*/ LDR         R6, [R4, #16] 
    /*00008700*/ LDR         R0, [R4] 
    /*00008704*/ LDR         LR, [R4, #28] 
    /*00008708*/ LDR         IP, [R4, #44]               @ 0x2c 
    /*0000870c*/ MUL         R8, R9, R8 
    /*00008710*/ MUL         R4, R1, R7 
    /*00008714*/ MLA         LR, R6, LR, R8 
    /*00008718*/ MLA         IP, R0, IP, R4 
    /*0000871c*/ UMULL       R6, R7, R9, R6 
    /*00008720*/ UMULL       R0, R1, R1, R0 
    /*00008724*/ ADD         R7, LR, R7 
    /*00008728*/ ADD         R1, IP, R1 
    /*0000872c*/ SUBS        R0, R6, R0 
    /*00008730*/ SBC         R1, R7, R1 
    /*00008734*/ BL          0x0000000000008734          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*00008738*/ STRD        R0, [R5, #40]               @ 0x28 
    /*0000873c*/ POP         {R3, R4, R5, R6, R7, R8, R9, SL, FP, PC} 
FUNC_END inverseModel


FUNC_BEGIN cornerMotionCoef
    /*00008740*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00008744*/ SUB         SP, SP, #28 
    /*00008748*/ MOV         R9, R0 
    /*0000874c*/ MOV         R3, #0 
    /*00008750*/ MOV         R8, R1 
    /*00008754*/ STR         R3, [SP, #8] 
    /*00008758*/ ADD         R1, R1, #32 
    /*0000875c*/ STR         R2, [SP, #20] 
    /*00008760*/ STR         R1, [SP, #16] 
    /*00008764*/ STR         R3, [SP, #4] 
    cornerMotionCoef_x8768:
    /*00008768*/ MOV         R2, #6 
    /*0000876c*/ LDRD        R0, [R9] 
    /*00008770*/ BL          0x0000000000008770          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008774*/ MOV         R2, #6 
    /*00008778*/ MOV         R3, R0 
    /*0000877c*/ LDR         R0, [R8] 
    /*00008780*/ ASR         R5, R0, #31 
    /*00008784*/ MUL         IP, R0, R1 
    /*00008788*/ UMULL       R6, R7, R0, R3 
    /*0000878c*/ LDRD        R0, [R9, #8] 
    /*00008790*/ MLA         R3, R3, R5, IP 
    /*00008794*/ ADD         R7, R3, R7 
    /*00008798*/ BL          0x0000000000008798          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000879c*/ LDR         R3, [R8, #4] 
    /*000087a0*/ MOV         R2, #4 
    /*000087a4*/ ASR         FP, R3, #31 
    /*000087a8*/ UMULL       R4, R5, R3, R0 
    /*000087ac*/ MUL         R3, R3, R1 
    /*000087b0*/ ADDS        R4, R4, R6 
    /*000087b4*/ MLA         R3, R0, FP, R3 
    /*000087b8*/ LDRD        R0, [R9, #16] 
    /*000087bc*/ ADD         R5, R3, R5 
    /*000087c0*/ ADC         R5, R5, R7 
    /*000087c4*/ BL          0x00000000000087C4          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*000087c8*/ MOV         R2, #10 
    /*000087cc*/ ADDS        R0, R0, R4 
    /*000087d0*/ ADC         R1, R1, R5 
    /*000087d4*/ BL          0x00000000000087D4          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*000087d8*/ MOV         R2, #6 
    /*000087dc*/ STR         R0, [SP, #12] 
    /*000087e0*/ LDRD        R0, [R9, #24] 
    /*000087e4*/ BL          0x00000000000087E4          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*000087e8*/ LDR         IP, [R8] 
    /*000087ec*/ MOV         R2, #6 
    /*000087f0*/ ASR         R5, IP, #31 
    /*000087f4*/ UMULL       R6, R7, IP, R0 
    /*000087f8*/ MUL         IP, IP, R1 
    /*000087fc*/ MLA         IP, R0, R5, IP 
    /*00008800*/ LDRD        R0, [R9, #32] 
    /*00008804*/ ADD         R7, IP, R7 
    /*00008808*/ BL          0x0000000000008808          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000880c*/ LDR         IP, [R8, #4] 
    /*00008810*/ MOV         R2, #4 
    /*00008814*/ ASR         FP, IP, #31 
    /*00008818*/ UMULL       R4, R5, IP, R0 
    /*0000881c*/ MUL         IP, IP, R1 
    /*00008820*/ ADDS        R4, R4, R6 
    /*00008824*/ MLA         IP, R0, FP, IP 
    /*00008828*/ LDRD        R0, [R9, #40]               @ 0x28 
    /*0000882c*/ ADD         R5, IP, R5 
    /*00008830*/ ADC         R5, R5, R7 
    /*00008834*/ BL          0x0000000000008834          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*00008838*/ MOV         R2, #10 
    /*0000883c*/ ADDS        R0, R0, R4 
    /*00008840*/ ADC         R1, R1, R5 
    /*00008844*/ BL          0x0000000000008844          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008848*/ LDM         R8, {R1, R2} 
    /*0000884c*/ LDR         R3, [SP, #12] 
    /*00008850*/ ADD         R8, R8, #8 
    /*00008854*/ SUB         R3, R3, R1, LSL #4 
    /*00008858*/ LDR         R1, [SP, #4] 
    /*0000885c*/ CMP         R3, #0 
    /*00008860*/ RSBLT       R3, R3, #0 
    /*00008864*/ ADD         R3, R1, R3 
    /*00008868*/ STR         R3, [SP, #4] 
    /*0000886c*/ LDR         R3, [SP, #16] 
    /*00008870*/ SUB         R0, R0, R2, LSL #4 
    /*00008874*/ CMP         R0, #0 
    /*00008878*/ RSBLT       R0, R0, #0 
    /*0000887c*/ CMP         R8, R3 
    /*00008880*/ LDR         R3, [SP, #8] 
    /*00008884*/ ADD         R3, R3, R0 
    /*00008888*/ STR         R3, [SP, #8] 
    /*0000888c*/ BNE         cornerMotionCoef_x8768
    /*00008890*/ LDR         R5, [SP, #20] 
    /*00008894*/ LDR         R3, [SP, #4] 
    /*00008898*/ LDR         R1, [R5] 
    /*0000889c*/ LSL         R0, R3, #7 
    /*000088a0*/ CMP         R1, #0 
    /*000088a4*/ MOVEQ       R1, #1 
    /*000088a8*/ BL          0x00000000000088A8          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000088ac*/ LDR         R1, [R5, #4] 
    /*000088b0*/ LDR         R3, [SP, #8] 
    /*000088b4*/ CMP         R1, #0 
    /*000088b8*/ MOVEQ       R1, #1 
    /*000088bc*/ MOV         R4, R0 
    /*000088c0*/ LSL         R0, R3, #7 
    /*000088c4*/ BL          0x00000000000088C4          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*000088c8*/ CMP         R0, R4 
    /*000088cc*/ MOVLT       R0, R4 
    /*000088d0*/ ADD         SP, SP, #28 
    /*000088d4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
FUNC_END cornerMotionCoef


FUNC_BEGIN confineStabMotion
    /*000088d8*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000088dc*/ SUB         SP, SP, #124                @ 0x7c 
    /*000088e0*/ MOV         R4, R0 
    /*000088e4*/ MOV         SL, R1 
    /*000088e8*/ STR         R0, [SP, #28] 
    /*000088ec*/ ADD         FP, R4, #52                 @ 0x34 
    /*000088f0*/ MOV         R2, #48                     @ 0x30 
    /*000088f4*/ ADD         R0, SP, #72                 @ 0x48 
    /*000088f8*/ ADD         R3, R4, #84                 @ 0x54 
    /*000088fc*/ STR         R3, [SP, #24] 
    /*00008900*/ BL          0x0000000000008900          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00008904*/ MOV         R3, #0 
    /*00008908*/ STR         R3, [SP, #20] 
    /*0000890c*/ MOV         R3, SL 
    /*00008910*/ MOV         SL, FP 
    /*00008914*/ MOV         FP, R3 
    confineStabMotion_x8918:
    /*00008918*/ MOV         R2, #48                     @ 0x30 
    /*0000891c*/ ADD         R1, SP, #72                 @ 0x48 
    /*00008920*/ MOV         R0, FP 
    /*00008924*/ MOV         R9, #4 
    /*00008928*/ BL          0x0000000000008928          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000892c*/ LDR         R1, [SP, #20] 
    /*00008930*/ MOV         R0, FP 
    /*00008934*/ MOV         R8, #0 
    /*00008938*/ BL          0x000000000000815C 
    confineStabMotion_x893c:
    /*0000893c*/ MOV         R2, #6 
    /*00008940*/ LDRD        R0, [FP] 
    /*00008944*/ BL          0x0000000000008944          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008948*/ LDR         R3, [SL, R8, LSL #3] 
    /*0000894c*/ MOV         R2, #6 
    /*00008950*/ ASR         R5, R3, #31 
    /*00008954*/ UMULL       R6, R7, R3, R0 
    /*00008958*/ MUL         R3, R3, R1 
    /*0000895c*/ MLA         R3, R0, R5, R3 
    /*00008960*/ LDRD        R0, [FP, #8] 
    /*00008964*/ ADD         R7, R3, R7 
    /*00008968*/ BL          0x0000000000008968          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000896c*/ LDR         R3, [SL, R9] 
    /*00008970*/ MOV         R2, #4 
    /*00008974*/ MOV         R4, R3 
    /*00008978*/ ASR         R5, R3, #31 
    /*0000897c*/ STRD        R4, [SP] 
    /*00008980*/ UMULL       R4, R5, R3, R0 
    /*00008984*/ MUL         R3, R3, R1 
    /*00008988*/ LDR         R1, [SP, #4] 
    /*0000898c*/ ADDS        R4, R4, R6 
    /*00008990*/ MLA         R3, R0, R1, R3 
    /*00008994*/ LDRD        R0, [FP, #16] 
    /*00008998*/ ADD         R5, R3, R5 
    /*0000899c*/ ADC         R5, R5, R7 
    /*000089a0*/ BL          0x00000000000089A0          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*000089a4*/ MOV         R2, #10 
    /*000089a8*/ ADDS        R0, R0, R4 
    /*000089ac*/ ADC         R1, R1, R5 
    /*000089b0*/ BL          0x00000000000089B0          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*000089b4*/ ADD         R3, SP, #40                 @ 0x28 
    /*000089b8*/ MOV         R2, #6 
    /*000089bc*/ STR         R0, [R3, R8, LSL #3] 
    /*000089c0*/ LDRD        R0, [FP, #24] 
    /*000089c4*/ BL          0x00000000000089C4          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*000089c8*/ LDR         R3, [SL, R8, LSL #3] 
    /*000089cc*/ MOV         R2, #6 
    /*000089d0*/ ADD         R8, R8, #1 
    /*000089d4*/ ASR         R5, R3, #31 
    /*000089d8*/ UMULL       R6, R7, R3, R0 
    /*000089dc*/ MUL         R3, R3, R1 
    /*000089e0*/ MLA         R3, R0, R5, R3 
    /*000089e4*/ LDRD        R0, [FP, #32] 
    /*000089e8*/ ADD         R7, R3, R7 
    /*000089ec*/ BL          0x00000000000089EC          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*000089f0*/ LDR         R3, [SL, R9] 
    /*000089f4*/ MOV         R2, #4 
    /*000089f8*/ MOV         R4, R3 
    /*000089fc*/ ASR         R5, R3, #31 
    /*00008a00*/ STRD        R4, [SP] 
    /*00008a04*/ UMULL       R4, R5, R3, R0 
    /*00008a08*/ MUL         R3, R3, R1 
    /*00008a0c*/ LDR         R1, [SP, #4] 
    /*00008a10*/ ADDS        R4, R4, R6 
    /*00008a14*/ MLA         R3, R0, R1, R3 
    /*00008a18*/ LDRD        R0, [FP, #40]               @ 0x28 
    /*00008a1c*/ ADD         R5, R3, R5 
    /*00008a20*/ ADC         R5, R5, R7 
    /*00008a24*/ BL          0x0000000000008A24          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*00008a28*/ MOV         R2, #10 
    /*00008a2c*/ ADDS        R0, R0, R4 
    /*00008a30*/ ADC         R1, R1, R5 
    /*00008a34*/ BL          0x0000000000008A34          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008a38*/ ADD         R3, SP, #40                 @ 0x28 
    /*00008a3c*/ CMP         R8, #4 
    /*00008a40*/ STR         R0, [R3, R9] 
    /*00008a44*/ ADD         R9, R9, #8 
    /*00008a48*/ BNE         confineStabMotion_x893c
    /*00008a4c*/ LDR         IP, [SP, #28] 
    /*00008a50*/ MOV         R3, #0 
    /*00008a54*/ LDR         R1, [IP, #84]               @ 0x54 
    confineStabMotion_x8a58:
    /*00008a58*/ ADD         R2, SP, #40                 @ 0x28 
    /*00008a5c*/ LDR         R2, [R2, R3] 
    /*00008a60*/ CMP         R2, R1 
    /*00008a64*/ BLE         confineStabMotion_x8a98
    /*00008a68*/ ADD         R0, SP, #40                 @ 0x28 
    /*00008a6c*/ ADD         LR, R0, R3 
    /*00008a70*/ LDR         R0, [IP, #100]              @ 0x64 
    /*00008a74*/ CMP         R2, R0 
    /*00008a78*/ BGE         confineStabMotion_x8a98
    /*00008a7c*/ LDR         R2, [LR, #4] 
    /*00008a80*/ LDR         R0, [IP, #88]               @ 0x58 
    /*00008a84*/ CMP         R2, R0 
    /*00008a88*/ BLE         confineStabMotion_x8a98
    /*00008a8c*/ LDR         R0, [IP, #104]              @ 0x68 
    /*00008a90*/ CMP         R2, R0 
    /*00008a94*/ BLT         confineStabMotion_x8bfc
    confineStabMotion_x8a98:
    /*00008a98*/ ADD         R3, R3, #8 
    /*00008a9c*/ CMP         R3, #32 
    /*00008aa0*/ BNE         confineStabMotion_x8a58
    /*00008aa4*/ LDR         R3, [SP, #28] 
    /*00008aa8*/ MOV         R2, #1 
    /*00008aac*/ STR         FP, [SP, #36]               @ 0x24 
    /*00008ab0*/ ADD         R3, R3, #88                 @ 0x58 
    /*00008ab4*/ STR         R2, [SP] 
    /*00008ab8*/ STR         SL, [SP, #32] 
    /*00008abc*/ MOV         FP, R3 
    confineStabMotion_x8ac0:
    /*00008ac0*/ LDR         R3, [SP] 
    /*00008ac4*/ ADD         R4, SP, #40                 @ 0x28 
    /*00008ac8*/ LDR         R1, [SP, #24] 
    /*00008acc*/ MOV         R5, #1 
    /*00008ad0*/ AND         R3, R3, #3 
    /*00008ad4*/ LDR         R6, [FP] 
    /*00008ad8*/ LDR         R7, [FP, #-4] 
    /*00008adc*/ ADD         R2, R1, R3, LSL #3 
    /*00008ae0*/ LDR         SL, [R1, R3, LSL #3] 
    /*00008ae4*/ STR         FP, [SP, #16] 
    /*00008ae8*/ LDR         R3, [R2, #4] 
    /*00008aec*/ SUB         R9, SL, R7 
    /*00008af0*/ MOV         FP, SL 
    /*00008af4*/ SUB         R8, R3, R6 
    /*00008af8*/ STR         R3, [SP, #12] 
    confineStabMotion_x8afc:
    /*00008afc*/ LDR         R0, [R4, #4] 
    /*00008b00*/ AND         IP, R5, #3 
    /*00008b04*/ LDR         R3, [R4] 
    /*00008b08*/ ADD         LR, SP, #40                 @ 0x28 
    /*00008b0c*/ SUB         R1, R0, R6 
    /*00008b10*/ ADD         LR, LR, IP, LSL #3 
    /*00008b14*/ SUB         R2, R3, R7 
    /*00008b18*/ ADD         SL, SP, #40                 @ 0x28 
    /*00008b1c*/ MUL         R1, R9, R1 
    /*00008b20*/ LDR         IP, [SL, IP, LSL #3] 
    /*00008b24*/ LDR         LR, [LR, #4] 
    /*00008b28*/ MLS         R2, R8, R2, R1 
    /*00008b2c*/ SUB         R1, LR, R6 
    /*00008b30*/ CMP         R2, #0 
    /*00008b34*/ SUB         R2, IP, R7 
    /*00008b38*/ BLT         confineStabMotion_x8b94
    /*00008b3c*/ MUL         R1, R9, R1 
    /*00008b40*/ MOVNE       SL, #1 
    /*00008b44*/ MOVEQ       SL, #0 
    /*00008b48*/ MLS         R2, R8, R2, R1 
    /*00008b4c*/ CMP         R2, #0 
    /*00008b50*/ BLT         confineStabMotion_x8c24
    confineStabMotion_x8b54:
    /*00008b54*/ ADD         R5, R5, #1 
    /*00008b58*/ ADD         R4, R4, #8 
    /*00008b5c*/ CMP         R5, #5 
    /*00008b60*/ BNE         confineStabMotion_x8afc
    /*00008b64*/ LDR         R3, [SP] 
    /*00008b68*/ LDR         FP, [SP, #16] 
    /*00008b6c*/ ADD         R3, R3, #1 
    /*00008b70*/ STR         R3, [SP] 
    /*00008b74*/ CMP         R3, #5 
    /*00008b78*/ ADD         FP, FP, #8 
    /*00008b7c*/ BNE         confineStabMotion_x8ac0
    /*00008b80*/ LDR         R3, [SP, #20] 
    /*00008b84*/ ADDS        R0, R3, #0 
    /*00008b88*/ MOVNE       R0, #1 
    /*00008b8c*/ ADD         SP, SP, #124                @ 0x7c 
    /*00008b90*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    confineStabMotion_x8b94:
    /*00008b94*/ MUL         R1, R9, R1 
    /*00008b98*/ MLS         R2, R8, R2, R1 
    /*00008b9c*/ CMP         R2, #0 
    /*00008ba0*/ BLT         confineStabMotion_x8b54
    confineStabMotion_x8ba4:
    /*00008ba4*/ BEQ         confineStabMotion_x8b54
    /*00008ba8*/ SUB         IP, IP, R3 
    /*00008bac*/ SUB         R1, R6, R0 
    /*00008bb0*/ SUB         LR, LR, R0 
    /*00008bb4*/ SUB         R2, R7, R3 
    /*00008bb8*/ MUL         R1, IP, R1 
    /*00008bbc*/ SUB         R3, FP, R3 
    /*00008bc0*/ MLS         R2, LR, R2, R1 
    /*00008bc4*/ CMP         R2, #0 
    /*00008bc8*/ LDR         R2, [SP, #12] 
    /*00008bcc*/ SUB         R0, R2, R0 
    /*00008bd0*/ BLT         confineStabMotion_x8c2c
    /*00008bd4*/ MUL         IP, IP, R0 
    /*00008bd8*/ MOVNE       R2, #1 
    /*00008bdc*/ MOVEQ       R2, #0 
    /*00008be0*/ MLS         LR, LR, R3, IP 
    /*00008be4*/ CMP         LR, #0 
    /*00008be8*/ BGE         confineStabMotion_x8b54
    /*00008bec*/ CMP         R2, #0 
    confineStabMotion_x8bf0:
    /*00008bf0*/ BEQ         confineStabMotion_x8b54
    /*00008bf4*/ LDR         SL, [SP, #32] 
    /*00008bf8*/ LDR         FP, [SP, #36]               @ 0x24 
    confineStabMotion_x8bfc:
    /*00008bfc*/ LDR         R3, [SP, #20] 
    /*00008c00*/ ADD         R3, R3, #163                @ 0xa3 
    /*00008c04*/ STR         R3, [SP, #20] 
    /*00008c08*/ MOV         R2, R3 
    /*00008c0c*/ MOVW        R3, #8313                   @ 0x2079 
    /*00008c10*/ CMP         R2, R3 
    /*00008c14*/ BNE         confineStabMotion_x8918
    /*00008c18*/ MOV         R0, #1 
    /*00008c1c*/ ADD         SP, SP, #124                @ 0x7c 
    /*00008c20*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    confineStabMotion_x8c24:
    /*00008c24*/ CMP         SL, #0 
    /*00008c28*/ B           confineStabMotion_x8ba4
    confineStabMotion_x8c2c:
    /*00008c2c*/ MUL         IP, IP, R0 
    /*00008c30*/ MLS         IP, LR, R3, IP 
    /*00008c34*/ CMP         IP, #0 
    /*00008c38*/ BGE         confineStabMotion_x8bf0
    /*00008c3c*/ B           confineStabMotion_x8b54
FUNC_END confineStabMotion


FUNC_BEGIN modelsProduct
    /*00008c40*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00008c44*/ MOV         R3, R1 
    /*00008c48*/ MOV         R5, R0 
    /*00008c4c*/ LDRD        R6, [R3] 
    /*00008c50*/ LDRD        R8, [R5, #24] 
    /*00008c54*/ SUB         SP, SP, #84                 @ 0x54 
    /*00008c58*/ LDRD        SL, [R5, #32] 
    /*00008c5c*/ MOV         R4, R2 
    /*00008c60*/ STRD        R6, [SP, #32] 
    /*00008c64*/ MOV         R2, #0 
    /*00008c68*/ LDRD        R6, [R5, #8] 
    /*00008c6c*/ STRD        R8, [SP, #8] 
    /*00008c70*/ LDRD        R8, [R3, #8] 
    /*00008c74*/ LDR         IP, [R3, #20] 
    /*00008c78*/ STRD        R6, [SP] 
    /*00008c7c*/ STRD        SL, [SP, #16] 
    /*00008c80*/ LDR         R7, [R3, #16] 
    /*00008c84*/ LDR         R6, [R3, #40]               @ 0x28 
    /*00008c88*/ LDRD        SL, [R3, #24] 
    /*00008c8c*/ STRD        R8, [SP, #40]               @ 0x28 
    /*00008c90*/ LDRD        R0, [R0] 
    /*00008c94*/ LDRD        R8, [R3, #32] 
    /*00008c98*/ LDR         R3, [R3, #44]               @ 0x2c 
    /*00008c9c*/ STR         IP, [SP, #64]               @ 0x40 
    /*00008ca0*/ STRD        R8, [SP, #56]               @ 0x38 
    /*00008ca4*/ STR         R3, [SP, #68]               @ 0x44 
    /*00008ca8*/ STRD        SL, [SP, #48]               @ 0x30 
    /*00008cac*/ STRD        R0, [SP, #24] 
    /*00008cb0*/ BL          0x0000000000008CB0          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008cb4*/ MOV         R2, #0 
    /*00008cb8*/ MOV         R8, R0 
    /*00008cbc*/ MOV         R9, R1 
    /*00008cc0*/ LDRD        R0, [SP, #32] 
    /*00008cc4*/ BL          0x0000000000008CC4          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008cc8*/ MOV         R2, #20 
    /*00008ccc*/ UMULL       SL, FP, R8, R0 
    /*00008cd0*/ MUL         R1, R8, R1 
    /*00008cd4*/ MLA         R0, R0, R9, R1 
    /*00008cd8*/ ADD         FP, R0, FP 
    /*00008cdc*/ MOV         R0, SL 
    /*00008ce0*/ MOV         R1, FP 
    /*00008ce4*/ BL          0x0000000000008CE4          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008ce8*/ MOV         R2, #0 
    /*00008cec*/ STRD        R0, [SP, #72]               @ 0x48 
    /*00008cf0*/ LDRD        R0, [SP] 
    /*00008cf4*/ BL          0x0000000000008CF4          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008cf8*/ MOV         R2, #0 
    /*00008cfc*/ MOV         FP, R0 
    /*00008d00*/ MOV         SL, R1 
    /*00008d04*/ LDRD        R0, [SP, #48]               @ 0x30 
    /*00008d08*/ BL          0x0000000000008D08          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008d0c*/ MOV         R2, #20 
    /*00008d10*/ UMULL       R8, R9, FP, R0 
    /*00008d14*/ MUL         R1, FP, R1 
    /*00008d18*/ MLA         R0, R0, SL, R1 
    /*00008d1c*/ ADD         R9, R0, R9 
    /*00008d20*/ MOV         R0, R8 
    /*00008d24*/ MOV         R1, R9 
    /*00008d28*/ BL          0x0000000000008D28          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008d2c*/ LDRD        SL, [SP, #72]               @ 0x48 
    /*00008d30*/ MOV         R2, #0 
    /*00008d34*/ ADDS        SL, SL, R0 
    /*00008d38*/ ADC         FP, FP, R1 
    /*00008d3c*/ LDRD        R0, [SP, #24] 
    /*00008d40*/ MOV         R8, SL 
    /*00008d44*/ MOV         R9, FP 
    /*00008d48*/ STRD        R8, [R4] 
    /*00008d4c*/ BL          0x0000000000008D4C          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008d50*/ MOV         R2, #0 
    /*00008d54*/ MOV         R8, R0 
    /*00008d58*/ MOV         R9, R1 
    /*00008d5c*/ LDRD        R0, [SP, #40]               @ 0x28 
    /*00008d60*/ BL          0x0000000000008D60          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008d64*/ MOV         R2, #20 
    /*00008d68*/ UMULL       SL, FP, R8, R0 
    /*00008d6c*/ MUL         R1, R8, R1 
    /*00008d70*/ MLA         R0, R0, R9, R1 
    /*00008d74*/ ADD         FP, R0, FP 
    /*00008d78*/ MOV         R0, SL 
    /*00008d7c*/ MOV         R1, FP 
    /*00008d80*/ BL          0x0000000000008D80          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008d84*/ MOV         R2, #0 
    /*00008d88*/ STRD        R0, [SP, #72]               @ 0x48 
    /*00008d8c*/ LDRD        R0, [SP] 
    /*00008d90*/ BL          0x0000000000008D90          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008d94*/ MOV         R2, #0 
    /*00008d98*/ MOV         FP, R0 
    /*00008d9c*/ MOV         SL, R1 
    /*00008da0*/ LDRD        R0, [SP, #56]               @ 0x38 
    /*00008da4*/ BL          0x0000000000008DA4          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008da8*/ MOV         R2, #20 
    /*00008dac*/ UMULL       R8, R9, FP, R0 
    /*00008db0*/ MUL         R1, FP, R1 
    /*00008db4*/ MLA         R0, R0, SL, R1 
    /*00008db8*/ ADD         R9, R0, R9 
    /*00008dbc*/ MOV         R0, R8 
    /*00008dc0*/ MOV         R1, R9 
    /*00008dc4*/ BL          0x0000000000008DC4          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008dc8*/ LDRD        R8, [SP, #72]               @ 0x48 
    /*00008dcc*/ MOV         R2, #0 
    /*00008dd0*/ ADDS        R8, R8, R0 
    /*00008dd4*/ ADC         R9, R9, R1 
    /*00008dd8*/ LDRD        R0, [SP, #24] 
    /*00008ddc*/ STRD        R8, [R4, #8] 
    /*00008de0*/ BL          0x0000000000008DE0          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008de4*/ LDR         IP, [SP, #64]               @ 0x40 
    /*00008de8*/ MOV         R2, #20 
    /*00008dec*/ UMULL       R8, R9, R7, R0 
    /*00008df0*/ MUL         R1, R7, R1 
    /*00008df4*/ MLA         R0, R0, IP, R1 
    /*00008df8*/ ADD         R9, R0, R9 
    /*00008dfc*/ MOV         R0, R8 
    /*00008e00*/ MOV         R1, R9 
    /*00008e04*/ BL          0x0000000000008E04          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008e08*/ MOV         R2, #0 
    /*00008e0c*/ MOV         R8, R0 
    /*00008e10*/ MOV         R9, R1 
    /*00008e14*/ LDRD        R0, [SP] 
    /*00008e18*/ BL          0x0000000000008E18          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008e1c*/ LDR         R3, [SP, #68]               @ 0x44 
    /*00008e20*/ MOV         R2, #20 
    /*00008e24*/ UMULL       SL, FP, R6, R0 
    /*00008e28*/ MUL         R1, R6, R1 
    /*00008e2c*/ MLA         R0, R0, R3, R1 
    /*00008e30*/ ADD         FP, R0, FP 
    /*00008e34*/ MOV         R0, SL 
    /*00008e38*/ MOV         R1, FP 
    /*00008e3c*/ BL          0x0000000000008E3C          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008e40*/ LDRD        SL, [R5, #16] 
    /*00008e44*/ MOV         R2, #0 
    /*00008e48*/ ADDS        R8, R8, R0 
    /*00008e4c*/ ADC         R9, R9, R1 
    /*00008e50*/ ADDS        SL, SL, R8 
    /*00008e54*/ ADC         FP, FP, R9 
    /*00008e58*/ LDRD        R0, [SP, #8] 
    /*00008e5c*/ MOV         R8, SL 
    /*00008e60*/ MOV         R9, FP 
    /*00008e64*/ STRD        R8, [R4, #16] 
    /*00008e68*/ BL          0x0000000000008E68          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008e6c*/ MOV         R2, #0 
    /*00008e70*/ MOV         R8, R0 
    /*00008e74*/ MOV         R9, R1 
    /*00008e78*/ LDRD        R0, [SP, #32] 
    /*00008e7c*/ BL          0x0000000000008E7C          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008e80*/ MOV         R2, #20 
    /*00008e84*/ UMULL       SL, FP, R8, R0 
    /*00008e88*/ MUL         R1, R8, R1 
    /*00008e8c*/ MLA         R0, R0, R9, R1 
    /*00008e90*/ ADD         FP, R0, FP 
    /*00008e94*/ MOV         R0, SL 
    /*00008e98*/ MOV         R1, FP 
    /*00008e9c*/ BL          0x0000000000008E9C          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008ea0*/ MOV         R2, #0 
    /*00008ea4*/ STRD        R0, [SP] 
    /*00008ea8*/ LDRD        R0, [SP, #16] 
    /*00008eac*/ BL          0x0000000000008EAC          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008eb0*/ MOV         R2, #0 
    /*00008eb4*/ MOV         FP, R0 
    /*00008eb8*/ MOV         SL, R1 
    /*00008ebc*/ LDRD        R0, [SP, #48]               @ 0x30 
    /*00008ec0*/ BL          0x0000000000008EC0          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008ec4*/ MOV         R2, #20 
    /*00008ec8*/ UMULL       R8, R9, FP, R0 
    /*00008ecc*/ MUL         R1, FP, R1 
    /*00008ed0*/ MLA         R0, R0, SL, R1 
    /*00008ed4*/ ADD         R9, R0, R9 
    /*00008ed8*/ MOV         R0, R8 
    /*00008edc*/ MOV         R1, R9 
    /*00008ee0*/ BL          0x0000000000008EE0          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008ee4*/ LDRD        R8, [SP] 
    /*00008ee8*/ MOV         R2, #0 
    /*00008eec*/ ADDS        R8, R8, R0 
    /*00008ef0*/ ADC         R9, R9, R1 
    /*00008ef4*/ LDRD        R0, [SP, #8] 
    /*00008ef8*/ STRD        R8, [R4, #24] 
    /*00008efc*/ BL          0x0000000000008EFC          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008f00*/ MOV         R2, #0 
    /*00008f04*/ MOV         R8, R0 
    /*00008f08*/ MOV         R9, R1 
    /*00008f0c*/ LDRD        R0, [SP, #40]               @ 0x28 
    /*00008f10*/ BL          0x0000000000008F10          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008f14*/ MOV         R2, #20 
    /*00008f18*/ UMULL       SL, FP, R8, R0 
    /*00008f1c*/ MUL         R1, R8, R1 
    /*00008f20*/ MLA         R0, R0, R9, R1 
    /*00008f24*/ ADD         FP, R0, FP 
    /*00008f28*/ MOV         R0, SL 
    /*00008f2c*/ MOV         R1, FP 
    /*00008f30*/ BL          0x0000000000008F30          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008f34*/ MOV         R2, #0 
    /*00008f38*/ STRD        R0, [SP] 
    /*00008f3c*/ LDRD        R0, [SP, #16] 
    /*00008f40*/ BL          0x0000000000008F40          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008f44*/ MOV         R2, #0 
    /*00008f48*/ MOV         FP, R0 
    /*00008f4c*/ STR         R1, [SP, #24] 
    /*00008f50*/ LDRD        R0, [SP, #56]               @ 0x38 
    /*00008f54*/ BL          0x0000000000008F54          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008f58*/ LDR         R3, [SP, #24] 
    /*00008f5c*/ MOV         R2, #20 
    /*00008f60*/ UMULL       R8, R9, FP, R0 
    /*00008f64*/ MUL         R1, FP, R1 
    /*00008f68*/ MLA         R0, R0, R3, R1 
    /*00008f6c*/ ADD         R9, R0, R9 
    /*00008f70*/ MOV         R0, R8 
    /*00008f74*/ MOV         R1, R9 
    /*00008f78*/ BL          0x0000000000008F78          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008f7c*/ LDRD        R8, [SP] 
    /*00008f80*/ MOV         R2, #0 
    /*00008f84*/ ADDS        R8, R8, R0 
    /*00008f88*/ ADC         R9, R9, R1 
    /*00008f8c*/ LDRD        R0, [SP, #8] 
    /*00008f90*/ STRD        R8, [R4, #32] 
    /*00008f94*/ BL          0x0000000000008F94          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008f98*/ LDR         IP, [SP, #64]               @ 0x40 
    /*00008f9c*/ MOV         R2, #20 
    /*00008fa0*/ UMULL       R8, R9, R7, R0 
    /*00008fa4*/ MUL         R1, R7, R1 
    /*00008fa8*/ MLA         R1, R0, IP, R1 
    /*00008fac*/ MOV         R0, R8 
    /*00008fb0*/ ADD         R9, R1, R9 
    /*00008fb4*/ MOV         R1, R9 
    /*00008fb8*/ BL          0x0000000000008FB8          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008fbc*/ MOV         R2, #0 
    /*00008fc0*/ MOV         SL, R0 
    /*00008fc4*/ MOV         FP, R1 
    /*00008fc8*/ LDRD        R0, [SP, #16] 
    /*00008fcc*/ BL          0x0000000000008FCC          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008fd0*/ LDR         R3, [SP, #68]               @ 0x44 
    /*00008fd4*/ MOV         R2, #20 
    /*00008fd8*/ UMULL       R8, R9, R6, R0 
    /*00008fdc*/ MUL         R1, R6, R1 
    /*00008fe0*/ MLA         R1, R0, R3, R1 
    /*00008fe4*/ MOV         R0, R8 
    /*00008fe8*/ ADD         R9, R1, R9 
    /*00008fec*/ MOV         R1, R9 
    /*00008ff0*/ BL          0x0000000000008FF0          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*00008ff4*/ LDRD        R2, [R5, #40]               @ 0x28 
    /*00008ff8*/ ADDS        R6, R0, SL 
    /*00008ffc*/ ADC         R7, R1, FP 
    /*00009000*/ ADDS        R0, R2, R6 
    /*00009004*/ ADC         R1, R3, R7 
    /*00009008*/ STRD        R0, [R4, #40]               @ 0x28 
    /*0000900c*/ ADD         SP, SP, #84                 @ 0x54 
    /*00009010*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
FUNC_END modelsProduct


FUNC_BEGIN getMatricesGeometricMean
    /*00009014*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00009018*/ SUB         SP, SP, #196                @ 0xc4 
    /*0000901c*/ MOV         R6, R2 
    /*00009020*/ MOV         R5, R0 
    /*00009024*/ MOV         R2, #48                     @ 0x30 
    /*00009028*/ MOV         FP, R1 
    /*0000902c*/ ADD         R0, SP, #96                 @ 0x60 
    /*00009030*/ MOV         R1, #0 
    /*00009034*/ STR         R3, [SP, #36]               @ 0x24 
    /*00009038*/ BL          0x0000000000009038          @ memset [R_ARM_CALL: +0x0] 
    /*0000903c*/ ADD         R0, SP, #144                @ 0x90 
    /*00009040*/ MOV         R2, #48                     @ 0x30 
    /*00009044*/ MOV         R1, #0 
    /*00009048*/ BL          0x0000000000009048          @ memset [R_ARM_CALL: +0x0] 
    /*0000904c*/ CMP         R6, #1 
    /*00009050*/ BEQ         getMatricesGeometricMean_x9318
    /*00009054*/ LDRH        SL, [R5, #8] 
    /*00009058*/ RSB         R6, R6, #1 
    /*0000905c*/ SUB         R7, SL, #1 
    /*00009060*/ UXTH        R7, R7 
    /*00009064*/ ADD         R6, R7, R6 
    /*00009068*/ UXTH        R6, R6 
    /*0000906c*/ SXTH        R4, R6 
    /*00009070*/ CMP         R4, #0 
    /*00009074*/ BLT         getMatricesGeometricMean_x9300
    /*00009078*/ LDR         R3, [SP, #36]               @ 0x24 
    /*0000907c*/ SXTH        R7, R7 
    /*00009080*/ ADD         R0, SP, #48                 @ 0x30 
    /*00009084*/ LDR         R2, [R3, #60]               @ 0x3c 
    /*00009088*/ LDR         R3, [R3, #56]               @ 0x38 
    /*0000908c*/ STR         R2, [SP, #40]               @ 0x28 
    /*00009090*/ STR         R3, [SP, #44]               @ 0x2c 
    /*00009094*/ BL          0x0000000000009094          @ DIS_resetMotionModel [R_ARM_CALL: .text+0x6b70] 
    /*00009098*/ CMP         R7, R4 
    /*0000909c*/ BLT         getMatricesGeometricMean_x90ec
    getMatricesGeometricMean_x90a0:
    /*000090a0*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*000090a4*/ MOV         R1, R4 
    /*000090a8*/ ADD         R2, SP, #96                 @ 0x60 
    /*000090ac*/ MOV         R0, R5 
    /*000090b0*/ CMP         R3, #0 
    /*000090b4*/ BEQ         getMatricesGeometricMean_x90bc
    /*000090b8*/ BLX         R3 
    getMatricesGeometricMean_x90bc:
    /*000090bc*/ ADD         R4, R4, #1 
    /*000090c0*/ ADD         R2, SP, #144                @ 0x90 
    /*000090c4*/ ADD         R1, SP, #48                 @ 0x30 
    /*000090c8*/ ADD         R0, SP, #96                 @ 0x60 
    /*000090cc*/ BL          0x0000000000008C40 
    /*000090d0*/ SXTH        R4, R4 
    /*000090d4*/ MOV         R2, #48                     @ 0x30 
    /*000090d8*/ ADD         R1, SP, #144                @ 0x90 
    /*000090dc*/ ADD         R0, SP, R2 
    /*000090e0*/ BL          0x00000000000090E0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000090e4*/ CMP         R7, R4 
    /*000090e8*/ BGE         getMatricesGeometricMean_x90a0
    getMatricesGeometricMean_x90ec:
    /*000090ec*/ LDRD        R2, [SP, #56]               @ 0x38 
    /*000090f0*/ SUB         SL, SL, R6 
    /*000090f4*/ LDRD        R4, [SP, #48]               @ 0x30 
    /*000090f8*/ SXTH        SL, SL 
    /*000090fc*/ LDRD        R6, [SP, #80]               @ 0x50 
    /*00009100*/ STRD        R2, [SP, #16] 
    /*00009104*/ CMP         SL, #1 
    /*00009108*/ LDRD        R2, [SP, #64]               @ 0x40 
    /*0000910c*/ STRD        R2, [SP] 
    /*00009110*/ LDRD        R2, [SP, #72]               @ 0x48 
    /*00009114*/ STRD        R2, [SP, #24] 
    /*00009118*/ LDRD        R2, [SP, #88]               @ 0x58 
    /*0000911c*/ STRD        R2, [SP, #8] 
    /*00009120*/ BLE         getMatricesGeometricMean_x9344
    /*00009124*/ CMP         R5, #0 
    /*00009128*/ CMPEQ       R4, #10485760               @ 0xa00000 
    /*0000912c*/ STRLS       SL, [SP, #32] 
    /*00009130*/ MOVLS       SL, FP 
    /*00009134*/ BLS         getMatricesGeometricMean_x91f0
    /*00009138*/ B           getMatricesGeometricMean_x9300
    getMatricesGeometricMean_x913c:
    /*0000913c*/ ADDS        R2, R4, #1048576            @ 0x100000 
    /*00009140*/ ADC         R3, R5, #0 
    getMatricesGeometricMean_x9144:
    /*00009144*/ BL          0x0000000000009144          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*00009148*/ CMP         R0, #0 
    /*0000914c*/ MOV         R3, R1 
    /*00009150*/ SBCS        R1, R3, #0 
    /*00009154*/ MOV         R2, R0 
    /*00009158*/ STRD        R2, [SP] 
    /*0000915c*/ MOVGE       R3, R2 
    /*00009160*/ LDRLT       R3, [SP] 
    /*00009164*/ LDR         R2, [SL, #28] 
    /*00009168*/ RSBLT       R3, R3, #0 
    /*0000916c*/ CMP         R3, R2, LSL #11 
    /*00009170*/ BHI         getMatricesGeometricMean_x9300
    /*00009174*/ MOV         R2, #20 
    /*00009178*/ LDRD        R0, [SP, #8] 
    /*0000917c*/ BL          0x000000000000917C          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*00009180*/ MOV         R2, #0 
    /*00009184*/ MVN         R3, #0 
    /*00009188*/ MOVT        R2, #65520                  @ 0xfff0 
    /*0000918c*/ CMP         R7, R3 
    /*00009190*/ CMPEQ       R6, R2 
    /*00009194*/ BEQ         getMatricesGeometricMean_x930c
    /*00009198*/ ADDS        R2, R6, #1048576            @ 0x100000 
    /*0000919c*/ ADC         R3, R7, #0 
    getMatricesGeometricMean_x91a0:
    /*000091a0*/ BL          0x00000000000091A0          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*000091a4*/ CMP         R0, #0 
    /*000091a8*/ MOV         R3, R1 
    /*000091ac*/ SBCS        R1, R3, #0 
    /*000091b0*/ MOV         R2, R0 
    /*000091b4*/ STRD        R2, [SP, #8] 
    /*000091b8*/ MOVGE       R3, R2 
    /*000091bc*/ LDRLT       R3, [SP, #8] 
    /*000091c0*/ LDR         R2, [SL, #32] 
    /*000091c4*/ RSBLT       R3, R3, #0 
    /*000091c8*/ CMP         R3, R2, LSL #11 
    /*000091cc*/ BHI         getMatricesGeometricMean_x9300
    /*000091d0*/ LDR         R3, [SP, #32] 
    /*000091d4*/ ASR         R3, R3, #1 
    /*000091d8*/ STR         R3, [SP, #32] 
    /*000091dc*/ CMP         R3, #1 
    /*000091e0*/ BEQ         getMatricesGeometricMean_x9340
    /*000091e4*/ CMP         R5, #0 
    /*000091e8*/ CMPEQ       R4, #10485760               @ 0xa00000 
    /*000091ec*/ BHI         getMatricesGeometricMean_x9300
    getMatricesGeometricMean_x91f0:
    /*000091f0*/ MOV         R0, R4 
    /*000091f4*/ MOV         R1, R5 
    /*000091f8*/ MOV         R2, #20 
    /*000091fc*/ BL          0x00000000000091FC          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*00009200*/ MOV         R2, R0 
    /*00009204*/ MOV         R3, R1 
    /*00009208*/ MOV         R0, #22 
    /*0000920c*/ BL          0x000000000000920C          @ DIS_SQRT_HW [R_ARM_CALL: .text+0x6efc] 
    /*00009210*/ CMP         R7, #0 
    /*00009214*/ CMPEQ       R6, #10485760               @ 0xa00000 
    /*00009218*/ MOV         R4, R0 
    /*0000921c*/ ASR         R5, R0, #31 
    /*00009220*/ BHI         getMatricesGeometricMean_x9300
    /*00009224*/ MOV         R0, R6 
    /*00009228*/ MOV         R1, R7 
    /*0000922c*/ MOV         R2, #20 
    /*00009230*/ BL          0x0000000000009230          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*00009234*/ MOV         R3, R1 
    /*00009238*/ MOV         R2, R0 
    /*0000923c*/ MOV         R0, #22 
    /*00009240*/ BL          0x0000000000009240          @ DIS_SQRT_HW [R_ARM_CALL: .text+0x6efc] 
    /*00009244*/ MOV         R2, #20 
    /*00009248*/ MOV         R6, R0 
    /*0000924c*/ ASR         R7, R0, #31 
    /*00009250*/ ADDS        R8, R4, R6 
    /*00009254*/ LDRD        R0, [SP, #16] 
    /*00009258*/ ADC         R9, R5, R7 
    /*0000925c*/ BL          0x000000000000925C          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*00009260*/ ORRS        R3, R8, R9 
    /*00009264*/ MOVEQ       R8, #1 
    /*00009268*/ MOVEQ       R9, #0 
    /*0000926c*/ MOV         R2, R8 
    /*00009270*/ MOV         R3, R9 
    /*00009274*/ BL          0x0000000000009274          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*00009278*/ MOV         R2, R0 
    /*0000927c*/ MOV         R3, R1 
    /*00009280*/ STRD        R2, [SP, #16] 
    /*00009284*/ ADDS        R2, R0, #1048576            @ 0x100000 
    /*00009288*/ ADC         R3, R1, #0 
    /*0000928c*/ CMP         R3, #0 
    /*00009290*/ CMPEQ       R2, #2097152                @ 0x200000 
    /*00009294*/ BHI         getMatricesGeometricMean_x9300
    /*00009298*/ MOV         R2, #20 
    /*0000929c*/ LDRD        R0, [SP, #24] 
    /*000092a0*/ BL          0x00000000000092A0          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*000092a4*/ MOV         R2, R8 
    /*000092a8*/ MOV         R3, R9 
    /*000092ac*/ BL          0x00000000000092AC          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*000092b0*/ MOV         R2, R0 
    /*000092b4*/ MOV         R3, R1 
    /*000092b8*/ STRD        R2, [SP, #24] 
    /*000092bc*/ ADDS        R2, R0, #1048576            @ 0x100000 
    /*000092c0*/ ADC         R3, R1, #0 
    /*000092c4*/ CMP         R3, #0 
    /*000092c8*/ CMPEQ       R2, #2097152                @ 0x200000 
    /*000092cc*/ BHI         getMatricesGeometricMean_x9300
    /*000092d0*/ MOV         R2, #20 
    /*000092d4*/ LDRD        R0, [SP] 
    /*000092d8*/ BL          0x00000000000092D8          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*000092dc*/ MOV         R2, #0 
    /*000092e0*/ MVN         R3, #0 
    /*000092e4*/ MOVT        R2, #65520                  @ 0xfff0 
    /*000092e8*/ CMP         R5, R3 
    /*000092ec*/ CMPEQ       R4, R2 
    /*000092f0*/ BNE         getMatricesGeometricMean_x913c
    /*000092f4*/ MOV         R2, #1 
    /*000092f8*/ MOV         R3, #0 
    /*000092fc*/ B           getMatricesGeometricMean_x9144
    getMatricesGeometricMean_x9300:
    /*00009300*/ MOV         R0, #0 
    getMatricesGeometricMean_x9304:
    /*00009304*/ ADD         SP, SP, #196                @ 0xc4 
    /*00009308*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    getMatricesGeometricMean_x930c:
    /*0000930c*/ MOV         R2, #1 
    /*00009310*/ MOV         R3, #0 
    /*00009314*/ B           getMatricesGeometricMean_x91a0
    getMatricesGeometricMean_x9318:
    /*00009318*/ LDR         R3, [R5, #52]               @ 0x34 
    /*0000931c*/ CMP         R3, #0 
    /*00009320*/ MOVEQ       R0, R6 
    /*00009324*/ BEQ         getMatricesGeometricMean_x9304
    /*00009328*/ MOV         R0, R5 
    /*0000932c*/ LDR         R1, [SP, #232]              @ 0xe8 
    /*00009330*/ BLX         R3 
    /*00009334*/ MOV         R0, R6 
    /*00009338*/ ADD         SP, SP, #196                @ 0xc4 
    /*0000933c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    getMatricesGeometricMean_x9340:
    /*00009340*/ MOV         FP, SL 
    getMatricesGeometricMean_x9344:
    /*00009344*/ LDR         R3, [SP, #232]              @ 0xe8 
    /*00009348*/ LDR         R0, [SP, #232]              @ 0xe8 
    /*0000934c*/ STRD        R4, [R3] 
    /*00009350*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*00009354*/ STRD        R6, [R0, #32] 
    /*00009358*/ RSB         R1, R3, #8192               @ 0x2000 
    /*0000935c*/ LDRD        R2, [SP, #16] 
    /*00009360*/ STRD        R2, [R0, #8] 
    /*00009364*/ LDRD        R2, [SP] 
    /*00009368*/ STRD        R2, [R0, #16] 
    /*0000936c*/ LDRD        R2, [SP, #24] 
    /*00009370*/ STRD        R2, [R0, #24] 
    /*00009374*/ LDRD        R2, [SP, #8] 
    /*00009378*/ STRD        R2, [R0, #40]               @ 0x28 
    /*0000937c*/ BL          0x000000000000815C 
    /*00009380*/ LDR         R1, [SP, #36]               @ 0x24 
    /*00009384*/ ADD         R3, SP, #144                @ 0x90 
    /*00009388*/ LDR         R2, [SP, #40]               @ 0x28 
    /*0000938c*/ ADD         R1, R1, #8 
    /*00009390*/ LDR         R0, [SP, #232]              @ 0xe8 
    /*00009394*/ BL          0x00000000000082E4 
    /*00009398*/ ADD         R1, SP, #144                @ 0x90 
    /*0000939c*/ MOV         R2, #48                     @ 0x30 
    /*000093a0*/ LDR         R0, [SP, #232]              @ 0xe8 
    /*000093a4*/ BL          0x00000000000093A4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000093a8*/ ADD         R0, FP, #208                @ 0xd0 
    /*000093ac*/ MOV         R2, #48                     @ 0x30 
    /*000093b0*/ LDR         R1, [SP, #232]              @ 0xe8 
    /*000093b4*/ BL          0x00000000000093B4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*000093b8*/ MOV         R0, #1 
    /*000093bc*/ B           getMatricesGeometricMean_x9304
FUNC_END getMatricesGeometricMean


FUNC_BEGIN updateLongTermMotion
    /*000093c0*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*000093c4*/ SUB         SP, SP, #436                @ 0x1b4 
    /*000093c8*/ ADD         R9, SP, #240                @ 0xf0 
    /*000093cc*/ MOV         R6, R0 
    /*000093d0*/ MOV         R4, R1 
    /*000093d4*/ MOV         R7, R2 
    /*000093d8*/ MOV         R1, #0 
    /*000093dc*/ MOV         R2, #48                     @ 0x30 
    /*000093e0*/ ADD         R0, SP, #192                @ 0xc0 
    /*000093e4*/ MOV         R5, #344                    @ 0x158 
    /*000093e8*/ STR         R3, [SP, #100]              @ 0x64 
    /*000093ec*/ BL          0x00000000000093EC          @ memset [R_ARM_CALL: +0x0] 
    /*000093f0*/ MUL         R5, R5, R6 
    /*000093f4*/ MOV         R2, #48                     @ 0x30 
    /*000093f8*/ MOV         R1, #0 
    /*000093fc*/ MOV         R0, R9 
    /*00009400*/ BL          0x0000000000009400          @ memset [R_ARM_CALL: +0x0] 
    /*00009404*/ MOV         R2, #48                     @ 0x30 
    /*00009408*/ MOV         R1, #0 
    /*0000940c*/ ADD         R0, SP, #288                @ 0x120 
    /*00009410*/ BL          0x0000000000009410          @ memset [R_ARM_CALL: +0x0] 
    /*00009414*/ MOV         R2, #48                     @ 0x30 
    /*00009418*/ MOV         R1, #0 
    /*0000941c*/ ADD         R0, SP, #336                @ 0x150 
    /*00009420*/ BL          0x0000000000009420          @ memset [R_ARM_CALL: +0x0] 
    /*00009424*/ MOVW        SL, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x16f8] 
    /*00009428*/ MOV         R2, #48                     @ 0x30 
    /*0000942c*/ ADD         R0, SP, #384                @ 0x180 
    /*00009430*/ MOV         R1, #0 
    /*00009434*/ MOVT        SL, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x16f8] 
    /*00009438*/ BL          0x0000000000009438          @ memset [R_ARM_CALL: +0x0] 
    /*0000943c*/ ADD         R3, SL, R5 
    /*00009440*/ LDR         R2, [R3, #112]              @ 0x70 
    /*00009444*/ CMP         R2, #1 
    /*00009448*/ BLS         updateLongTermMotion_x9ab4
    /*0000944c*/ LDR         FP, [SL, R5] 
    /*00009450*/ LDR         R3, [FP, #164]              @ 0xa4 
    /*00009454*/ CMP         R2, R3 
    /*00009458*/ MOVCS       R2, R3 
    /*0000945c*/ CMP         R2, #1 
    /*00009460*/ BLS         updateLongTermMotion_x9b6c
    /*00009464*/ MOV         R3, #2 
    updateLongTermMotion_x9468:
    /*00009468*/ UXTH        R5, R3 
    /*0000946c*/ LSL         R3, R5, #1 
    /*00009470*/ CMP         R2, R3 
    /*00009474*/ BCS         updateLongTermMotion_x9468
    updateLongTermMotion_x9478:
    /*00009478*/ MOV         R0, R9 
    /*0000947c*/ BL          0x000000000000947C          @ DIS_resetMotionModel [R_ARM_CALL: .text+0x6b70] 
    /*00009480*/ MOV         R0, #344                    @ 0x158 
    /*00009484*/ MUL         R2, R0, R6 
    /*00009488*/ ADD         R3, SL, R2 
    /*0000948c*/ LDR         R3, [R3, #156]              @ 0x9c 
    /*00009490*/ STR         R2, [SP, #108]              @ 0x6c 
    /*00009494*/ CMP         R3, #0 
    /*00009498*/ BEQ         updateLongTermMotion_x94ac
    /*0000949c*/ ADD         R0, R2, #104                @ 0x68 
    /*000094a0*/ ADD         R1, SP, #384                @ 0x180 
    /*000094a4*/ ADD         R0, SL, R0 
    /*000094a8*/ BLX         R3 
    updateLongTermMotion_x94ac:
    /*000094ac*/ CMP         R5, #0 
    /*000094b0*/ LDR         R3, [SP, #108]              @ 0x6c 
    /*000094b4*/ BEQ         updateLongTermMotion_x9a4c
    /*000094b8*/ ADD         R1, R3, #104                @ 0x68 
    /*000094bc*/ ADD         R2, R3, #56                 @ 0x38 
    /*000094c0*/ ADD         R1, SL, R1 
    /*000094c4*/ ADD         R3, R3, #8 
    /*000094c8*/ ADD         R2, SL, R2 
    /*000094cc*/ ADD         R3, SL, R3 
    /*000094d0*/ STR         SL, [SP, #112]              @ 0x70 
    /*000094d4*/ MOV         R8, R5 
    /*000094d8*/ MOV         SL, R1 
    /*000094dc*/ STR         R2, [SP, #96]               @ 0x60 
    /*000094e0*/ STR         R3, [SP, #104]              @ 0x68 
    /*000094e4*/ STR         R5, [SP, #116]              @ 0x74 
    /*000094e8*/ STR         R4, [SP, #36]               @ 0x24 
    /*000094ec*/ STR         R7, [SP, #32] 
    updateLongTermMotion_x94f0:
    /*000094f0*/ STR         R9, [SP] 
    /*000094f4*/ MOV         R2, R8 
    /*000094f8*/ LDR         R3, [SP, #32] 
    /*000094fc*/ MOV         R1, FP 
    /*00009500*/ MOV         R0, SL 
    /*00009504*/ BL          0x0000000000009014 
    /*00009508*/ CMP         R0, #0 
    /*0000950c*/ BEQ         updateLongTermMotion_x9680
    /*00009510*/ LDR         R3, [SP, #36]               @ 0x24 
    /*00009514*/ LDR         R3, [R3, #8] 
    /*00009518*/ CMP         R3, #0 
    /*0000951c*/ BEQ         updateLongTermMotion_x95cc
    /*00009520*/ LDR         IP, [SP, #260]              @ 0x104 
    /*00009524*/ ADD         R3, SP, #256                @ 0x100 
    /*00009528*/ MOVW        R1, #290                    @ 0x122 
    /*0000952c*/ LDRD        R2, [R3] 
    /*00009530*/ ASR         IP, IP, #31 
    /*00009534*/ STR         IP, [SP, #16] 
    /*00009538*/ ADD         IP, SP, #400                @ 0x190 
    /*0000953c*/ LDRH        R0, [FP, R1] 
    /*00009540*/ MOV         R1, #0 
    /*00009544*/ LDRD        R6, [IP] 
    /*00009548*/ LDR         IP, [SP, #16] 
    /*0000954c*/ STR         IP, [SP, #20] 
    /*00009550*/ LDRD        R4, [SP, #16] 
    /*00009554*/ EOR         R4, R4, R2 
    /*00009558*/ EOR         R5, R5, R3 
    /*0000955c*/ MOV         R2, R4 
    /*00009560*/ MOV         R3, R5 
    /*00009564*/ LDRD        R4, [SP, #16] 
    /*00009568*/ SUBS        R4, R2, R4 
    /*0000956c*/ SBC         R5, R3, R5 
    /*00009570*/ SUBS        R2, R4, R6 
    /*00009574*/ SBC         R3, R5, R7 
    /*00009578*/ MOV         R6, R2 
    /*0000957c*/ MOV         R7, R3 
    /*00009580*/ ASR         R3, R3, #31 
    /*00009584*/ STR         R3, [SP, #40]               @ 0x28 
    /*00009588*/ STR         R3, [SP, #44]               @ 0x2c 
    /*0000958c*/ LDRD        R4, [SP, #40]               @ 0x28 
    /*00009590*/ EOR         R6, R6, R4 
    /*00009594*/ EOR         R7, R7, R5 
    /*00009598*/ SUBS        R4, R6, R4 
    /*0000959c*/ SBC         R5, R7, R5 
    /*000095a0*/ LSR         IP, R4, #10 
    /*000095a4*/ ASR         R3, R5, #10 
    /*000095a8*/ ORR         IP, IP, R5, LSL #22 
    /*000095ac*/ STR         R3, [SP, #28] 
    /*000095b0*/ STR         IP, [SP, #24] 
    /*000095b4*/ LDRD        R2, [SP, #24] 
    /*000095b8*/ CMP         R0, R2 
    /*000095bc*/ SBCS        R3, R1, R3 
    /*000095c0*/ BLT         updateLongTermMotion_x9680
    /*000095c4*/ CMP         R8, #1 
    /*000095c8*/ BEQ         updateLongTermMotion_x99f8
    updateLongTermMotion_x95cc:
    /*000095cc*/ LDR         R3, [SP, #36]               @ 0x24 
    /*000095d0*/ LDR         R3, [R3, #12] 
    /*000095d4*/ CMP         R3, #0 
    /*000095d8*/ BEQ         updateLongTermMotion_x96d8
    /*000095dc*/ LDR         IP, [SP, #284]              @ 0x11c 
    /*000095e0*/ ADD         R3, SP, #280                @ 0x118 
    /*000095e4*/ MOVW        R1, #294                    @ 0x126 
    /*000095e8*/ LDRD        R2, [R3] 
    /*000095ec*/ ASR         IP, IP, #31 
    /*000095f0*/ STR         IP, [SP, #48]               @ 0x30 
    /*000095f4*/ ADD         IP, SP, #424                @ 0x1a8 
    /*000095f8*/ LDRH        R0, [FP, R1] 
    /*000095fc*/ MOV         R1, #0 
    /*00009600*/ LDRD        R6, [IP] 
    /*00009604*/ LDR         IP, [SP, #48]               @ 0x30 
    /*00009608*/ STR         IP, [SP, #52]               @ 0x34 
    /*0000960c*/ LDRD        R4, [SP, #48]               @ 0x30 
    /*00009610*/ EOR         R4, R4, R2 
    /*00009614*/ EOR         R5, R5, R3 
    /*00009618*/ MOV         R2, R4 
    /*0000961c*/ MOV         R3, R5 
    /*00009620*/ LDRD        R4, [SP, #48]               @ 0x30 
    /*00009624*/ SUBS        R4, R2, R4 
    /*00009628*/ SBC         R5, R3, R5 
    /*0000962c*/ SUBS        R2, R4, R6 
    /*00009630*/ SBC         R3, R5, R7 
    /*00009634*/ MOV         R6, R2 
    /*00009638*/ MOV         R7, R3 
    /*0000963c*/ ASR         R3, R3, #31 
    /*00009640*/ STR         R3, [SP, #72]               @ 0x48 
    /*00009644*/ STR         R3, [SP, #76]               @ 0x4c 
    /*00009648*/ LDRD        R4, [SP, #72]               @ 0x48 
    /*0000964c*/ EOR         R6, R6, R4 
    /*00009650*/ EOR         R7, R7, R5 
    /*00009654*/ SUBS        R4, R6, R4 
    /*00009658*/ SBC         R5, R7, R5 
    /*0000965c*/ LSR         IP, R4, #10 
    /*00009660*/ ASR         R3, R5, #10 
    /*00009664*/ ORR         IP, IP, R5, LSL #22 
    /*00009668*/ STR         R3, [SP, #60]               @ 0x3c 
    /*0000966c*/ STR         IP, [SP, #56]               @ 0x38 
    /*00009670*/ LDRD        R2, [SP, #56]               @ 0x38 
    /*00009674*/ CMP         R0, R2 
    /*00009678*/ SBCS        R3, R1, R3 
    /*0000967c*/ BGE         updateLongTermMotion_x96d0
    updateLongTermMotion_x9680:
    /*00009680*/ UBFX        R8, R8, #1, #16 
    /*00009684*/ CMP         R8, #0 
    /*00009688*/ BNE         updateLongTermMotion_x94f0
    /*0000968c*/ LDR         R2, [FP, #116]              @ 0x74 
    /*00009690*/ MOV         R3, #1 
    /*00009694*/ LDR         SL, [SP, #112]              @ 0x70 
    /*00009698*/ CMP         R2, #2 
    /*0000969c*/ STR         R3, [FP, #164]              @ 0xa4 
    /*000096a0*/ BEQ         updateLongTermMotion_x9a6c
    updateLongTermMotion_x96a4:
    /*000096a4*/ MOVW        R4, #1228                   @ 0x4cc 
    updateLongTermMotion_x96a8:
    /*000096a8*/ LDR         R3, [SP, #108]              @ 0x6c 
    /*000096ac*/ MOV         R1, R4 
    /*000096b0*/ ADD         R0, R3, #8 
    /*000096b4*/ ADD         R0, SL, R0 
    /*000096b8*/ BL          0x000000000000815C 
    /*000096bc*/ MOV         R1, R4 
    /*000096c0*/ LDR         R0, [SP, #96]               @ 0x60 
    /*000096c4*/ BL          0x000000000000815C 
    /*000096c8*/ LDR         R3, [FP, #164]              @ 0xa4 
    /*000096cc*/ B           updateLongTermMotion_x9a6c
    updateLongTermMotion_x96d0:
    /*000096d0*/ CMP         R8, #1 
    /*000096d4*/ BEQ         updateLongTermMotion_x99f8
    updateLongTermMotion_x96d8:
    /*000096d8*/ LDR         R3, [FP, #128]              @ 0x80 
    /*000096dc*/ CMP         R3, #0 
    /*000096e0*/ BEQ         updateLongTermMotion_x99f8
    /*000096e4*/ MOV         R2, #48                     @ 0x30 
    /*000096e8*/ MOV         R1, #0 
    /*000096ec*/ ADD         R0, SP, #384                @ 0x180 
    /*000096f0*/ BL          0x00000000000096F0          @ osal_memset [R_ARM_CALL: +0x0] 
    /*000096f4*/ ADD         R2, SP, #288                @ 0x120 
    /*000096f8*/ LDR         R1, [SP, #96]               @ 0x60 
    /*000096fc*/ MOV         R0, R9 
    /*00009700*/ BL          0x0000000000008C40 
    /*00009704*/ ADD         R0, SP, #384                @ 0x180 
    /*00009708*/ BL          0x0000000000009708          @ DIS_resetMotionModel [R_ARM_CALL: .text+0x6b70] 
    /*0000970c*/ ADD         R1, SP, #336                @ 0x150 
    /*00009710*/ LDR         R0, [SP, #104]              @ 0x68 
    /*00009714*/ BL          0x0000000000008594 
    /*00009718*/ ADD         R2, SP, #384                @ 0x180 
    /*0000971c*/ ADD         R1, SP, #336                @ 0x150 
    /*00009720*/ ADD         R0, SP, #288                @ 0x120 
    /*00009724*/ BL          0x0000000000008C40 
    /*00009728*/ ADD         R3, SP, #384                @ 0x180 
    /*0000972c*/ LDRD        R0, [R3] 
    /*00009730*/ ADD         R3, SP, #408                @ 0x198 
    /*00009734*/ LDRD        R6, [R3] 
    /*00009738*/ UMULL       R2, R3, R0, R0 
    /*0000973c*/ MUL         IP, R0, R1 
    /*00009740*/ MUL         LR, R6, R7 
    /*00009744*/ MOV         R5, R3 
    /*00009748*/ MOV         R4, R2 
    /*0000974c*/ STRD        R4, [SP, #64]               @ 0x40 
    /*00009750*/ ADD         IP, R5, IP, LSL #1 
    /*00009754*/ STR         IP, [SP, #68]               @ 0x44 
    /*00009758*/ LDR         IP, [FP, #140]              @ 0x8c 
    /*0000975c*/ UMULL       R2, R3, R6, R6 
    /*00009760*/ LDRD        R4, [SP, #64]               @ 0x40 
    /*00009764*/ MUL         IP, IP, IP 
    /*00009768*/ ADDS        R4, R4, R2 
    /*0000976c*/ ADD         R3, R3, LR, LSL #1 
    /*00009770*/ ADC         R5, R5, R3 
    /*00009774*/ LSR         LR, R4, #20 
    /*00009778*/ ASR         R3, R5, #20 
    /*0000977c*/ ORR         LR, LR, R5, LSL #12 
    /*00009780*/ STR         R3, [SP, #12] 
    /*00009784*/ ASR         IP, IP, #6 
    /*00009788*/ STR         LR, [SP, #8] 
    /*0000978c*/ LDRD        R4, [SP, #8] 
    /*00009790*/ ASR         R3, IP, #31 
    /*00009794*/ CMP         R4, IP 
    /*00009798*/ SBCS        R3, R5, R3 
    /*0000979c*/ BLT         updateLongTermMotion_x9680
    /*000097a0*/ LDR         R2, [FP, #136]              @ 0x88 
    /*000097a4*/ MUL         R2, R2, R2 
    /*000097a8*/ ASR         R2, R2, #6 
    /*000097ac*/ CMP         R2, R4 
    /*000097b0*/ ASR         R3, R2, #31 
    /*000097b4*/ SBCS        R3, R3, R5 
    /*000097b8*/ BLT         updateLongTermMotion_x9680
    /*000097bc*/ ADD         R3, SP, #392                @ 0x188 
    /*000097c0*/ LDRD        R2, [R3] 
    /*000097c4*/ MOV         R5, R3 
    /*000097c8*/ ASR         R3, R3, #31 
    /*000097cc*/ STR         R3, [SP, #80]               @ 0x50 
    /*000097d0*/ MOV         R4, R2 
    /*000097d4*/ STR         R3, [SP, #84]               @ 0x54 
    /*000097d8*/ LDRD        R2, [SP, #80]               @ 0x50 
    /*000097dc*/ STRD        R4, [SP, #128]              @ 0x80 
    /*000097e0*/ EOR         R3, R3, R5 
    /*000097e4*/ EOR         R2, R2, R4 
    /*000097e8*/ LDRD        R4, [SP, #80]               @ 0x50 
    /*000097ec*/ SUBS        R4, R2, R4 
    /*000097f0*/ SBC         R5, R3, R5 
    /*000097f4*/ CMP         R4, #1 
    /*000097f8*/ SBCS        R3, R5, #0 
    /*000097fc*/ STRD        R4, [SP, #64]               @ 0x40 
    /*00009800*/ BLT         updateLongTermMotion_x9b28
    updateLongTermMotion_x9804:
    /*00009804*/ LDRD        R4, [SP, #64]               @ 0x40 
    /*00009808*/ LSR         R3, R4, #7 
    /*0000980c*/ STR         R3, [SP, #88]               @ 0x58 
    /*00009810*/ LDR         R2, [SP, #88]               @ 0x58 
    /*00009814*/ LDR         R3, [FP, #144]              @ 0x90 
    /*00009818*/ ORR         R2, R2, R5, LSL #25 
    /*0000981c*/ STR         R2, [SP, #88]               @ 0x58 
    /*00009820*/ ASR         R2, R5, #7 
    /*00009824*/ STR         R2, [SP, #92]               @ 0x5c 
    /*00009828*/ MOV         R4, R3 
    /*0000982c*/ ASR         R5, R3, #31 
    /*00009830*/ LDRD        R2, [SP, #88]               @ 0x58 
    /*00009834*/ STRD        R4, [SP, #152]              @ 0x98 
    /*00009838*/ CMP         R4, R2 
    /*0000983c*/ SBCS        R3, R5, R3 
    /*00009840*/ BLT         updateLongTermMotion_x9894
    /*00009844*/ ASR         R3, R7, #31 
    /*00009848*/ STR         R3, [SP, #136]              @ 0x88 
    /*0000984c*/ STR         R3, [SP, #140]              @ 0x8c 
    /*00009850*/ LDRD        R4, [SP, #136]              @ 0x88 
    /*00009854*/ MOV         R2, R4 
    /*00009858*/ EOR         R2, R2, R6 
    /*0000985c*/ SUBS        R4, R2, R4 
    /*00009860*/ MOV         R3, R5 
    /*00009864*/ EOR         R3, R3, R7 
    /*00009868*/ SBC         R5, R3, R5 
    /*0000986c*/ LSR         IP, R4, #7 
    /*00009870*/ ASR         R3, R5, #7 
    /*00009874*/ ORR         IP, IP, R5, LSL #25 
    /*00009878*/ STR         R3, [SP, #124]              @ 0x7c 
    /*0000987c*/ STR         IP, [SP, #120]              @ 0x78 
    /*00009880*/ LDRD        R4, [SP, #152]              @ 0x98 
    /*00009884*/ LDRD        R2, [SP, #120]              @ 0x78 
    /*00009888*/ CMP         R4, R2 
    /*0000988c*/ SBCS        R3, R5, R3 
    /*00009890*/ BGE         updateLongTermMotion_x99f8
    updateLongTermMotion_x9894:
    /*00009894*/ LDRD        R4, [SP, #128]              @ 0x80 
    /*00009898*/ ORRS        R3, R6, R7 
    /*0000989c*/ MOVEQ       R3, #1 
    /*000098a0*/ MOVNE       R3, #0 
    /*000098a4*/ MOV         R2, R4 
    /*000098a8*/ ORRS        R2, R2, R5 
    /*000098ac*/ ORREQ       R3, R3, #1 
    /*000098b0*/ ORRS        R2, R0, R1 
    /*000098b4*/ ORREQ       R3, R3, #1 
    /*000098b8*/ CMP         R3, #0 
    /*000098bc*/ BNE         updateLongTermMotion_x9680
    /*000098c0*/ ASR         R3, R1, #31 
    /*000098c4*/ STR         R3, [SP, #168]              @ 0xa8 
    /*000098c8*/ LDR         IP, [SP, #168]              @ 0xa8 
    /*000098cc*/ ADDS        R2, R4, R6 
    /*000098d0*/ ADC         R3, R5, R7 
    /*000098d4*/ STR         IP, [SP, #172]              @ 0xac 
    /*000098d8*/ ASR         IP, R3, #31 
    /*000098dc*/ LDRD        R4, [SP, #168]              @ 0xa8 
    /*000098e0*/ STR         IP, [SP, #160]              @ 0xa0 
    /*000098e4*/ EOR         R4, R4, R0 
    /*000098e8*/ EOR         R5, R5, R1 
    /*000098ec*/ MOV         R0, R4 
    /*000098f0*/ MOV         R1, R5 
    /*000098f4*/ LDRD        R4, [SP, #168]              @ 0xa8 
    /*000098f8*/ SUBS        R4, R0, R4 
    /*000098fc*/ SBC         R5, R1, R5 
    /*00009900*/ LSR         IP, R4, #10 
    /*00009904*/ STR         IP, [SP, #144]              @ 0x90 
    /*00009908*/ LDR         IP, [SP, #160]              @ 0xa0 
    /*0000990c*/ MOV         R1, R5 
    /*00009910*/ ASR         R1, R1, #10 
    /*00009914*/ STR         R1, [SP, #148]              @ 0x94 
    /*00009918*/ STR         IP, [SP, #164]              @ 0xa4 
    /*0000991c*/ LDR         IP, [SP, #144]              @ 0x90 
    /*00009920*/ ORR         IP, IP, R5, LSL #22 
    /*00009924*/ LDRD        R4, [SP, #160]              @ 0xa0 
    /*00009928*/ STR         IP, [SP, #144]              @ 0x90 
    /*0000992c*/ EOR         R4, R4, R2 
    /*00009930*/ EOR         R5, R5, R3 
    /*00009934*/ LDRD        R0, [SP, #144]              @ 0x90 
    /*00009938*/ MOV         R2, R4 
    /*0000993c*/ MOV         R3, R5 
    /*00009940*/ LDRD        R4, [SP, #160]              @ 0xa0 
    /*00009944*/ SUBS        R4, R2, R4 
    /*00009948*/ SBC         R5, R3, R5 
    /*0000994c*/ ORRS        R1, R0, R1 
    /*00009950*/ LSL         R0, R4, #10 
    /*00009954*/ LSL         R1, R5, #10 
    /*00009958*/ ORR         R1, R1, R4, LSR #22 
    /*0000995c*/ BEQ         updateLongTermMotion_x9b10
    updateLongTermMotion_x9960:
    /*00009960*/ ASR         R3, R7, #31 
    /*00009964*/ STR         R3, [SP, #176]              @ 0xb0 
    /*00009968*/ LDR         IP, [SP, #176]              @ 0xb0 
    /*0000996c*/ LDRD        R2, [SP, #144]              @ 0x90 
    /*00009970*/ STR         IP, [SP, #180]              @ 0xb4 
    /*00009974*/ BL          0x0000000000009974          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*00009978*/ MOV         R4, R0 
    /*0000997c*/ MOV         R5, R1 
    /*00009980*/ LDRD        R0, [SP, #176]              @ 0xb0 
    /*00009984*/ MOV         R2, R0 
    /*00009988*/ EOR         R2, R2, R6 
    /*0000998c*/ SUBS        R0, R2, R0 
    /*00009990*/ MOV         R3, R1 
    /*00009994*/ EOR         R3, R3, R7 
    /*00009998*/ SBC         R1, R3, R1 
    /*0000999c*/ MOV         R2, R0 
    /*000099a0*/ MOV         R3, R1 
    /*000099a4*/ LDRD        R0, [SP, #64]               @ 0x40 
    /*000099a8*/ CMP         R2, R0 
    /*000099ac*/ SBCS        R3, R3, R1 
    /*000099b0*/ BGE         updateLongTermMotion_x9b74
    /*000099b4*/ MOV         R2, #7 
    /*000099b8*/ LDRD        R0, [SP, #128]              @ 0x80 
    /*000099bc*/ BL          0x00000000000099BC          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*000099c0*/ MOV         R2, R6 
    /*000099c4*/ MOV         R3, R7 
    /*000099c8*/ BL          0x00000000000099C8          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*000099cc*/ MOV         IP, R0 
    updateLongTermMotion_x99d0:
    /*000099d0*/ LDR         R2, [FP, #152]              @ 0x98 
    /*000099d4*/ CMP         R2, R4 
    /*000099d8*/ ASR         R3, R2, #31 
    /*000099dc*/ SBCS        R3, R3, R5 
    /*000099e0*/ BGE         updateLongTermMotion_x99f8
    /*000099e4*/ LDR         R3, [FP, #148]              @ 0x94 
    /*000099e8*/ EOR         R0, IP, IP, ASR #31 
    /*000099ec*/ SUB         R0, R0, IP, ASR #31 
    /*000099f0*/ CMP         R0, R3 
    /*000099f4*/ BGT         updateLongTermMotion_x9680
    updateLongTermMotion_x99f8:
    /*000099f8*/ LDR         R5, [SP, #116]              @ 0x74 
    /*000099fc*/ LDR         SL, [SP, #112]              @ 0x70 
    /*00009a00*/ CMP         R8, R5 
    /*00009a04*/ BEQ         updateLongTermMotion_x9a58
    /*00009a08*/ LDR         R2, [FP, #116]              @ 0x74 
    /*00009a0c*/ ADD         R3, R8, #1 
    /*00009a10*/ STR         R3, [FP, #164]              @ 0xa4 
    /*00009a14*/ CMP         R2, #2 
    /*00009a18*/ BEQ         updateLongTermMotion_x9a6c
    /*00009a1c*/ CMP         R8, #1 
    /*00009a20*/ BLS         updateLongTermMotion_x96a4
    /*00009a24*/ CMP         R8, #2 
    /*00009a28*/ BEQ         updateLongTermMotion_x9b64
    /*00009a2c*/ CMP         R8, #4 
    /*00009a30*/ BEQ         updateLongTermMotion_x9b20
    /*00009a34*/ CMP         R8, #8 
    /*00009a38*/ BEQ         updateLongTermMotion_x9b94
    /*00009a3c*/ CMP         R8, #16 
    /*00009a40*/ BNE         updateLongTermMotion_x9a6c
    /*00009a44*/ MOV         R4, #163                    @ 0xa3 
    /*00009a48*/ B           updateLongTermMotion_x96a8
    updateLongTermMotion_x9a4c:
    /*00009a4c*/ ADD         R3, R3, #56                 @ 0x38 
    /*00009a50*/ ADD         R3, SL, R3 
    /*00009a54*/ STR         R3, [SP, #96]               @ 0x60 
    updateLongTermMotion_x9a58:
    /*00009a58*/ LDR         R3, [FP, #164]              @ 0xa4 
    /*00009a5c*/ MOV         R2, #0 
    /*00009a60*/ STR         R2, [FP, #192]              @ 0xc0 
    /*00009a64*/ ADD         R3, R3, #1 
    /*00009a68*/ STR         R3, [FP, #164]              @ 0xa4 
    updateLongTermMotion_x9a6c:
    /*00009a6c*/ LDR         R2, [FP, #120]              @ 0x78 
    /*00009a70*/ MOV         R0, R9 
    /*00009a74*/ LDR         R4, [SP, #96]               @ 0x60 
    /*00009a78*/ CMP         R2, R3 
    /*00009a7c*/ STRCC       R2, [FP, #164]              @ 0xa4 
    /*00009a80*/ ADD         R2, SP, #192                @ 0xc0 
    /*00009a84*/ MOV         R1, R4 
    /*00009a88*/ BL          0x0000000000008C40 
    /*00009a8c*/ ADD         R1, SP, #192                @ 0xc0 
    /*00009a90*/ MOV         R0, R4 
    /*00009a94*/ MOV         R2, #48                     @ 0x30 
    /*00009a98*/ BL          0x0000000000009A98          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00009a9c*/ MOV         R1, R9 
    /*00009aa0*/ LDR         R0, [SP, #100]              @ 0x64 
    /*00009aa4*/ MOV         R2, #48                     @ 0x30 
    /*00009aa8*/ BL          0x0000000000009AA8          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00009aac*/ ADD         SP, SP, #436                @ 0x1b4 
    /*00009ab0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    updateLongTermMotion_x9ab4:
    /*00009ab4*/ LDR         R3, [R3, #152]              @ 0x98 
    /*00009ab8*/ CMP         R3, #0 
    /*00009abc*/ BEQ         updateLongTermMotion_x9ad0
    /*00009ac0*/ ADD         R0, R5, #104                @ 0x68 
    /*00009ac4*/ ADD         R1, SP, #384                @ 0x180 
    /*00009ac8*/ ADD         R0, SL, R0 
    /*00009acc*/ BLX         R3 
    updateLongTermMotion_x9ad0:
    /*00009ad0*/ ADD         R5, R5, #56                 @ 0x38 
    /*00009ad4*/ ADD         R2, SP, #192                @ 0xc0 
    /*00009ad8*/ ADD         R5, SL, R5 
    /*00009adc*/ ADD         R0, SP, #384                @ 0x180 
    /*00009ae0*/ MOV         R1, R5 
    /*00009ae4*/ BL          0x0000000000008C40 
    /*00009ae8*/ ADD         R1, SP, #192                @ 0xc0 
    /*00009aec*/ MOV         R0, R5 
    /*00009af0*/ MOV         R2, #48                     @ 0x30 
    /*00009af4*/ BL          0x0000000000009AF4          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00009af8*/ ADD         R1, SP, #384                @ 0x180 
    /*00009afc*/ LDR         R0, [SP, #100]              @ 0x64 
    /*00009b00*/ MOV         R2, #48                     @ 0x30 
    /*00009b04*/ BL          0x0000000000009B04          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00009b08*/ ADD         SP, SP, #436                @ 0x1b4 
    /*00009b0c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    updateLongTermMotion_x9b10:
    /*00009b10*/ MOV         R2, #1 
    /*00009b14*/ MOV         R3, #0 
    /*00009b18*/ STRD        R2, [SP, #144]              @ 0x90 
    /*00009b1c*/ B           updateLongTermMotion_x9960
    updateLongTermMotion_x9b20:
    /*00009b20*/ MOVW        R4, #655                    @ 0x28f 
    /*00009b24*/ B           updateLongTermMotion_x96a8
    updateLongTermMotion_x9b28:
    /*00009b28*/ ASR         R3, R7, #31 
    /*00009b2c*/ STR         R3, [SP, #184]              @ 0xb8 
    /*00009b30*/ STR         R3, [SP, #188]              @ 0xbc 
    /*00009b34*/ LDRD        R4, [SP, #184]              @ 0xb8 
    /*00009b38*/ MOV         R2, R4 
    /*00009b3c*/ EOR         R2, R2, R6 
    /*00009b40*/ MOV         R3, R5 
    /*00009b44*/ SUBS        R4, R2, R4 
    /*00009b48*/ EOR         R3, R3, R7 
    /*00009b4c*/ SBC         R5, R3, R5 
    /*00009b50*/ CMP         R4, #1 
    /*00009b54*/ MOV         R3, R5 
    /*00009b58*/ SBCS        R3, R3, #0 
    /*00009b5c*/ BGE         updateLongTermMotion_x9804
    /*00009b60*/ B           updateLongTermMotion_x99f8
    updateLongTermMotion_x9b64:
    /*00009b64*/ MOVW        R4, #819                    @ 0x333 
    /*00009b68*/ B           updateLongTermMotion_x96a8
    updateLongTermMotion_x9b6c:
    /*00009b6c*/ MOV         R5, #1 
    /*00009b70*/ B           updateLongTermMotion_x9478
    updateLongTermMotion_x9b74:
    /*00009b74*/ MOV         R2, #7 
    /*00009b78*/ MOV         R0, R6 
    /*00009b7c*/ MOV         R1, R7 
    /*00009b80*/ BL          0x0000000000009B80          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*00009b84*/ LDRD        R2, [SP, #128]              @ 0x80 
    /*00009b88*/ BL          0x0000000000009B88          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*00009b8c*/ MOV         IP, R0 
    /*00009b90*/ B           updateLongTermMotion_x99d0
    updateLongTermMotion_x9b94:
    /*00009b94*/ MOVW        R4, #409                    @ 0x199 
    /*00009b98*/ B           updateLongTermMotion_x96a8
FUNC_END updateLongTermMotion


FUNC_BEGIN calcCorrCoef.constprop.1
    /*00009b9c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00009ba0*/ SUB         SP, SP, #140                @ 0x8c 
    /*00009ba4*/ MOV         R7, R1 
    /*00009ba8*/ MOV         R6, R2 
    /*00009bac*/ MOV         R5, R0 
    /*00009bb0*/ MOV         R2, #40                     @ 0x28 
    /*00009bb4*/ MOV         R1, #0 
    /*00009bb8*/ ADD         R0, SP, #56                 @ 0x38 
    /*00009bbc*/ MOV         R4, R3 
    /*00009bc0*/ BL          0x0000000000009BC0          @ memset [R_ARM_CALL: +0x0] 
    /*00009bc4*/ MOV         R2, #40                     @ 0x28 
    /*00009bc8*/ MOV         R1, #0 
    /*00009bcc*/ ADD         R0, SP, #96                 @ 0x60 
    /*00009bd0*/ BL          0x0000000000009BD0          @ memset [R_ARM_CALL: +0x0] 
    /*00009bd4*/ SMULL       R0, R1, R7, R6 
    /*00009bd8*/ LDR         R2, [R5, #20] 
    /*00009bdc*/ SMULL       SL, FP, R7, R7 
    /*00009be0*/ LDR         R3, [R5, #16] 
    /*00009be4*/ SMULL       R8, R9, R6, R6 
    /*00009be8*/ STRD        R0, [SP] 
    /*00009bec*/ ASR         R1, R6, #31 
    /*00009bf0*/ CMP         R2, R3 
    /*00009bf4*/ MOV         R0, R6 
    /*00009bf8*/ MOV         R2, R7 
    /*00009bfc*/ ASR         R3, R7, #31 
    /*00009c00*/ STRD        R0, [SP, #8] 
    /*00009c04*/ MOV         R7, R1 
    /*00009c08*/ LDRD        R0, [SP] 
    /*00009c0c*/ STRD        SL, [SP, #72]               @ 0x48 
    /*00009c10*/ STRD        R8, [SP, #80]               @ 0x50 
    /*00009c14*/ STRD        R2, [SP, #56]               @ 0x38 
    /*00009c18*/ STRD        R6, [SP, #64]               @ 0x40 
    /*00009c1c*/ STRD        R0, [SP, #88]               @ 0x58 
    /*00009c20*/ BCS         calcCorrCoef.constprop.1_x9cf0
    /*00009c24*/ LDRD        R6, [R4] 
    /*00009c28*/ STRD        R6, [SP, #16] 
    /*00009c2c*/ LDRD        R6, [R4, #8] 
    /*00009c30*/ STRD        R6, [SP, #24] 
    /*00009c34*/ LDRD        R6, [R4, #16] 
    /*00009c38*/ STRD        R6, [SP, #32] 
    /*00009c3c*/ LDRD        R6, [R4, #24] 
    /*00009c40*/ STRD        R6, [SP, #40]               @ 0x28 
    /*00009c44*/ LDRD        R6, [R4, #32] 
    /*00009c48*/ STRD        R6, [SP, #48]               @ 0x30 
    calcCorrCoef.constprop.1_x9c4c:
    /*00009c4c*/ LDRD        R6, [SP, #16] 
    /*00009c50*/ LDRD        R0, [SP, #8] 
    /*00009c54*/ ADDS        R6, R6, R2 
    /*00009c58*/ ADC         R7, R7, R3 
    /*00009c5c*/ MOV         R2, R6 
    /*00009c60*/ MOV         R3, R7 
    /*00009c64*/ LDRD        R6, [SP, #24] 
    /*00009c68*/ STRD        R2, [R4] 
    /*00009c6c*/ ADDS        R0, R0, R6 
    /*00009c70*/ LDRD        R2, [SP, #32] 
    /*00009c74*/ ADC         R1, R1, R7 
    /*00009c78*/ MOV         R6, R0 
    /*00009c7c*/ ADDS        R2, R2, SL 
    /*00009c80*/ MOV         R7, R1 
    /*00009c84*/ STRD        R6, [R4, #8] 
    /*00009c88*/ LDRD        R6, [SP, #40]               @ 0x28 
    /*00009c8c*/ ADC         R3, R3, FP 
    /*00009c90*/ LDRD        R0, [SP] 
    /*00009c94*/ ADDS        R6, R6, R8 
    /*00009c98*/ STRD        R2, [R4, #16] 
    /*00009c9c*/ LDRD        R2, [SP, #48]               @ 0x30 
    /*00009ca0*/ ADC         R7, R7, R9 
    /*00009ca4*/ MOV         R8, R6 
    /*00009ca8*/ ADDS        R0, R0, R2 
    /*00009cac*/ MOV         R9, R7 
    /*00009cb0*/ ADC         R1, R1, R3 
    /*00009cb4*/ STRD        R8, [R4, #24] 
    /*00009cb8*/ STRD        R0, [R4, #32] 
    /*00009cbc*/ LDR         R3, [R5, #44]               @ 0x2c 
    /*00009cc0*/ CMP         R3, #0 
    /*00009cc4*/ BEQ         calcCorrCoef.constprop.1_x9cd4
    /*00009cc8*/ ADD         R1, SP, #56                 @ 0x38 
    /*00009ccc*/ MOV         R0, R5 
    /*00009cd0*/ BLX         R3 
    calcCorrCoef.constprop.1_x9cd4:
    /*00009cd4*/ LDR         R6, [R5, #20] 
    /*00009cd8*/ LDR         R3, [R5, #16] 
    /*00009cdc*/ CMP         R6, R3 
    /*00009ce0*/ BCS         calcCorrCoef.constprop.1_x9e00
    calcCorrCoef.constprop.1_x9ce4:
    /*00009ce4*/ MOV         R0, #1048576                @ 0x100000 
    /*00009ce8*/ ADD         SP, SP, #140                @ 0x8c 
    /*00009cec*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    calcCorrCoef.constprop.1_x9cf0:
    /*00009cf0*/ LDR         R3, [R5, #52]               @ 0x34 
    /*00009cf4*/ CMP         R3, #0 
    /*00009cf8*/ BEQ         calcCorrCoef.constprop.1_x9f38
    /*00009cfc*/ ADD         R2, SP, #96                 @ 0x60 
    /*00009d00*/ MOV         R1, #0 
    /*00009d04*/ MOV         R0, R5 
    /*00009d08*/ BLX         R3 
    /*00009d0c*/ LDRD        R2, [SP, #120]              @ 0x78 
    /*00009d10*/ LDRD        R0, [SP, #96]               @ 0x60 
    /*00009d14*/ LDRD        R6, [SP, #104]              @ 0x68 
    /*00009d18*/ LDRD        R8, [SP, #112]              @ 0x70 
    /*00009d1c*/ LDRD        SL, [SP, #128]              @ 0x80 
    /*00009d20*/ STRD        R2, [SP] 
    calcCorrCoef.constprop.1_x9d24:
    /*00009d24*/ LDRD        R2, [R4] 
    /*00009d28*/ SUBS        R2, R2, R0 
    /*00009d2c*/ SBC         R3, R3, R1 
    /*00009d30*/ STRD        R2, [R4] 
    /*00009d34*/ STRD        R2, [SP, #16] 
    /*00009d38*/ LDRD        R2, [R4, #8] 
    /*00009d3c*/ LDRD        R0, [R4, #24] 
    /*00009d40*/ SUBS        R2, R2, R6 
    /*00009d44*/ SBC         R3, R3, R7 
    /*00009d48*/ LDRD        R6, [R4, #16] 
    /*00009d4c*/ STRD        R2, [R4, #8] 
    /*00009d50*/ SUBS        R6, R6, R8 
    /*00009d54*/ STRD        R2, [SP, #24] 
    /*00009d58*/ SBC         R7, R7, R9 
    /*00009d5c*/ LDRD        R2, [SP] 
    /*00009d60*/ MOV         R8, R6 
    /*00009d64*/ SUBS        R2, R0, R2 
    /*00009d68*/ MOV         R9, R7 
    /*00009d6c*/ LDRD        R6, [R4, #32] 
    /*00009d70*/ SBC         R3, R1, R3 
    /*00009d74*/ MOV         R0, R2 
    /*00009d78*/ MOV         R2, R8 
    /*00009d7c*/ SUBS        R6, R6, SL 
    /*00009d80*/ MOV         R1, R3 
    /*00009d84*/ SBC         R7, R7, FP 
    /*00009d88*/ MOV         R3, R9 
    /*00009d8c*/ STRD        R0, [R4, #24] 
    /*00009d90*/ STRD        R2, [R4, #16] 
    /*00009d94*/ STRD        R6, [R4, #32] 
    /*00009d98*/ LDR         R3, [R5, #36]               @ 0x24 
    /*00009d9c*/ STRD        R8, [SP, #32] 
    /*00009da0*/ CMP         R3, #0 
    /*00009da4*/ STRD        R0, [SP, #40]               @ 0x28 
    /*00009da8*/ STRD        R6, [SP, #48]               @ 0x30 
    /*00009dac*/ BEQ         calcCorrCoef.constprop.1_x9f60
    /*00009db0*/ MOV         R0, R5 
    /*00009db4*/ BLX         R3 
    /*00009db8*/ LDRD        R0, [R4, #8] 
    /*00009dbc*/ LDRD        R2, [R4] 
    /*00009dc0*/ LDRD        SL, [SP, #72]               @ 0x48 
    /*00009dc4*/ STRD        R0, [SP, #24] 
    /*00009dc8*/ LDRD        R0, [SP, #64]               @ 0x40 
    /*00009dcc*/ STRD        R2, [SP, #16] 
    /*00009dd0*/ LDRD        R8, [SP, #80]               @ 0x50 
    /*00009dd4*/ STRD        R0, [SP, #8] 
    /*00009dd8*/ LDRD        R0, [R4, #16] 
    /*00009ddc*/ LDRD        R2, [SP, #56]               @ 0x38 
    /*00009de0*/ STRD        R0, [SP, #32] 
    /*00009de4*/ LDRD        R0, [R4, #24] 
    /*00009de8*/ STRD        R0, [SP, #40]               @ 0x28 
    /*00009dec*/ LDRD        R0, [R4, #32] 
    /*00009df0*/ STRD        R0, [SP, #48]               @ 0x30 
    /*00009df4*/ LDRD        R0, [SP, #88]               @ 0x58 
    /*00009df8*/ STRD        R0, [SP] 
    /*00009dfc*/ B           calcCorrCoef.constprop.1_x9c4c
    calcCorrCoef.constprop.1_x9e00:
    /*00009e00*/ LDR         R2, [R4, #16] 
    /*00009e04*/ MOV         R0, #31 
    /*00009e08*/ LDR         IP, [R4, #20] 
    /*00009e0c*/ LDM         R4, {R7, SL} 
    /*00009e10*/ UMULL       R2, R3, R2, R6 
    /*00009e14*/ LDR         FP, [R4, #8] 
    /*00009e18*/ MUL         R1, R7, SL 
    /*00009e1c*/ UMULL       R8, R9, R7, R7 
    /*00009e20*/ STRD        R2, [SP] 
    /*00009e24*/ LDR         R3, [R4, #32] 
    /*00009e28*/ ADD         R9, R9, R1, LSL #1 
    /*00009e2c*/ STR         R3, [SP, #24] 
    /*00009e30*/ LDR         R3, [R4, #36]               @ 0x24 
    /*00009e34*/ STR         R3, [SP, #32] 
    /*00009e38*/ LDR         R3, [SP, #4] 
    /*00009e3c*/ MLA         R3, R6, IP, R3 
    /*00009e40*/ STR         R3, [SP, #4] 
    /*00009e44*/ LDR         R3, [R4, #12] 
    /*00009e48*/ STR         R3, [SP, #40]               @ 0x28 
    /*00009e4c*/ LDRD        R2, [SP] 
    /*00009e50*/ SUBS        R2, R2, R8 
    /*00009e54*/ SBC         R3, R3, R9 
    /*00009e58*/ BL          0x0000000000009E58          @ DIS_SQRT_HW [R_ARM_CALL: .text+0x6efc] 
    /*00009e5c*/ LDR         R3, [R5, #20] 
    /*00009e60*/ LDR         R2, [R4, #24] 
    /*00009e64*/ LDR         IP, [R4, #28] 
    /*00009e68*/ LDR         R1, [R4, #12] 
    /*00009e6c*/ UMULL       R8, R9, R2, R3 
    /*00009e70*/ LDR         R2, [R4, #8] 
    /*00009e74*/ MUL         R1, R2, R1 
    /*00009e78*/ MLA         R9, R3, IP, R9 
    /*00009e7c*/ UMULL       R2, R3, R2, R2 
    /*00009e80*/ STRD        R2, [SP] 
    /*00009e84*/ ADD         R3, R3, R1, LSL #1 
    /*00009e88*/ MOV         LR, R0 
    /*00009e8c*/ STMIB       SP, {R3, LR} 
    /*00009e90*/ ASR         R3, LR, #31 
    /*00009e94*/ MOV         R0, #31 
    /*00009e98*/ MOV         R4, LR 
    /*00009e9c*/ MOV         R5, R3 
    /*00009ea0*/ LDRD        R2, [SP] 
    /*00009ea4*/ STRD        R4, [SP, #16] 
    /*00009ea8*/ SUBS        R2, R8, R2 
    /*00009eac*/ SBC         R3, R9, R3 
    /*00009eb0*/ BL          0x0000000000009EB0          @ DIS_SQRT_HW [R_ARM_CALL: .text+0x6efc] 
    /*00009eb4*/ LDR         LR, [SP, #8] 
    /*00009eb8*/ MOV         R3, R5 
    /*00009ebc*/ ORRS        R3, LR, R3 
    /*00009ec0*/ MOVEQ       R3, #1 
    /*00009ec4*/ MOVNE       R3, #0 
    /*00009ec8*/ ASR         R5, R0, #31 
    /*00009ecc*/ MOV         R4, R0 
    /*00009ed0*/ ORRS        R2, R0, R5 
    /*00009ed4*/ ORREQ       R3, R3, #1 
    /*00009ed8*/ CMP         R3, #0 
    /*00009edc*/ BNE         calcCorrCoef.constprop.1_x9ce4
    /*00009ee0*/ LDR         R3, [SP, #24] 
    /*00009ee4*/ MOV         R2, #10 
    /*00009ee8*/ UMULL       R0, R1, R7, FP 
    /*00009eec*/ UMULL       R8, R9, R6, R3 
    /*00009ef0*/ LDR         R3, [SP, #40]               @ 0x28 
    /*00009ef4*/ SUBS        R0, R8, R0 
    /*00009ef8*/ MUL         R3, R7, R3 
    /*00009efc*/ MLA         SL, FP, SL, R3 
    /*00009f00*/ LDR         R3, [SP, #32] 
    /*00009f04*/ MLA         R9, R6, R3, R9 
    /*00009f08*/ ADD         R3, SL, R1 
    /*00009f0c*/ SBC         R1, R9, R3 
    /*00009f10*/ BL          0x0000000000009F10          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*00009f14*/ LDRD        R2, [SP, #16] 
    /*00009f18*/ BL          0x0000000000009F18          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*00009f1c*/ MOV         R2, #10 
    /*00009f20*/ BL          0x0000000000009F20          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*00009f24*/ MOV         R2, R4 
    /*00009f28*/ MOV         R3, R5 
    /*00009f2c*/ BL          0x0000000000009F2C          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*00009f30*/ ADD         SP, SP, #140                @ 0x8c 
    /*00009f34*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    calcCorrCoef.constprop.1_x9f38:
    /*00009f38*/ MOV         SL, #0 
    /*00009f3c*/ MOV         FP, #0 
    /*00009f40*/ MOV         R8, SL 
    /*00009f44*/ STRD        SL, [SP] 
    /*00009f48*/ MOV         R9, FP 
    /*00009f4c*/ MOV         R6, SL 
    /*00009f50*/ MOV         R7, FP 
    /*00009f54*/ MOV         R0, SL 
    /*00009f58*/ MOV         R1, FP 
    /*00009f5c*/ B           calcCorrCoef.constprop.1_x9d24
    calcCorrCoef.constprop.1_x9f60:
    /*00009f60*/ LDRD        R0, [SP, #64]               @ 0x40 
    /*00009f64*/ LDRD        R2, [SP, #56]               @ 0x38 
    /*00009f68*/ LDRD        SL, [SP, #72]               @ 0x48 
    /*00009f6c*/ STRD        R0, [SP, #8] 
    /*00009f70*/ LDRD        R0, [SP, #88]               @ 0x58 
    /*00009f74*/ LDRD        R8, [SP, #80]               @ 0x50 
    /*00009f78*/ STRD        R0, [SP] 
    /*00009f7c*/ B           calcCorrCoef.constprop.1_x9c4c
FUNC_END calcCorrCoef.constprop.1


FUNC_BEGIN setGmeMotion
    /*00009f80*/ MOV         R3, #344                    @ 0x158 
    /*00009f84*/ MUL         R3, R3, R0 
    /*00009f88*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00009f8c*/ MOVW        R6, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x16f8] 
    /*00009f90*/ MOVT        R6, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x16f8] 
    /*00009f94*/ SUB         SP, SP, #28 
    /*00009f98*/ MOV         R8, R0 
    /*00009f9c*/ MOV         R4, R2 
    /*00009fa0*/ LDR         R5, [R6, R3] 
    /*00009fa4*/ MOV         R7, R1 
    /*00009fa8*/ LDR         R3, [R5, #24] 
    /*00009fac*/ CMP         R3, #1 
    /*00009fb0*/ ADDNE       R9, R5, #432                @ 0x1b0 
    /*00009fb4*/ BEQ         setGmeMotion_xa1a4
    setGmeMotion_x9fb8:
    /*00009fb8*/ CMP         R3, #0 
    /*00009fbc*/ BEQ         setGmeMotion_xa00c
    setGmeMotion_x9fc0:
    /*00009fc0*/ MOV         R2, #56                     @ 0x38 
    /*00009fc4*/ MOV         R1, R4 
    /*00009fc8*/ ADD         R0, R5, #372                @ 0x174 
    /*00009fcc*/ BL          0x0000000000009FCC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*00009fd0*/ LDR         R3, [R4, #20] 
    /*00009fd4*/ CMP         R3, #0 
    /*00009fd8*/ BNE         setGmeMotion_x9ff4
    /*00009fdc*/ LDR         R3, [R4, #24] 
    /*00009fe0*/ CMP         R3, #0 
    /*00009fe4*/ BNE         setGmeMotion_x9ff4
    /*00009fe8*/ LDR         R3, [R4, #28] 
    /*00009fec*/ CMP         R3, #0 
    /*00009ff0*/ BEQ         setGmeMotion_xa020
    setGmeMotion_x9ff4:
    /*00009ff4*/ MOV         R0, R9 
    /*00009ff8*/ BL          0x0000000000009FF8          @ DIS_resetMotionModel [R_ARM_CALL: .text+0x6b70] 
    /*00009ffc*/ MOV         R3, #0 
    /*0000a000*/ STR         R3, [R5, #368]              @ 0x170 
    /*0000a004*/ ADD         SP, SP, #28 
    /*0000a008*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    setGmeMotion_xa00c:
    /*0000a00c*/ MOV         R1, R7 
    /*0000a010*/ MOV         R2, #48                     @ 0x30 
    /*0000a014*/ MOV         R0, R9 
    /*0000a018*/ BL          0x000000000000A018          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000a01c*/ B           setGmeMotion_x9fc0
    setGmeMotion_xa020:
    /*0000a020*/ MOV         R3, #344                    @ 0x158 
    /*0000a024*/ MUL         R3, R3, R8 
    /*0000a028*/ LDR         SL, [R6, R3] 
    /*0000a02c*/ LDRB        R3, [SL, #160]              @ 0xa0 
    /*0000a030*/ CMP         R3, #2 
    /*0000a034*/ BEQ         setGmeMotion_xa2e8
    /*0000a038*/ MVN         R2, #55                     @ 0x37 
    /*0000a03c*/ MOV         R1, #28 
    /*0000a040*/ SMLABB      R1, R1, R3, R2 
    /*0000a044*/ MOVW        R2, #26215                  @ 0x6667 
    /*0000a048*/ MOVT        R2, #26214                  @ 0x6666 
    /*0000a04c*/ LDR         R0, [R4, #8] 
    /*0000a050*/ SUB         R0, R1, R0 
    /*0000a054*/ SMULL       R2, R3, R1, R2 
    /*0000a058*/ ASR         R1, R1, #31 
    /*0000a05c*/ RSB         R1, R1, R3, ASR #1 
    /*0000a060*/ ADD         R1, R1, #1 
    /*0000a064*/ CMP         R0, R1 
    /*0000a068*/ MOVGE       R0, R1 
    /*0000a06c*/ CMP         R1, #0 
    /*0000a070*/ BIC         R0, R0, R0, ASR #31 
    /*0000a074*/ MOVEQ       R1, #1 
    /*0000a078*/ LSL         R0, R0, #13 
    setGmeMotion_xa07c:
    /*0000a07c*/ BL          0x000000000000A07C          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000a080*/ MOV         R7, #344                    @ 0x158 
    /*0000a084*/ MLA         R7, R7, R8, R6 
    /*0000a088*/ MOV         R1, R0 
    /*0000a08c*/ MOV         R0, R9 
    /*0000a090*/ BL          0x000000000000815C 
    /*0000a094*/ LDR         R1, [R4, #8] 
    /*0000a098*/ LDR         R0, [R4, #44]               @ 0x2c 
    /*0000a09c*/ CMP         R1, #1 
    /*0000a0a0*/ LDR         R3, [R7, #192]              @ 0xc0 
    /*0000a0a4*/ MOVLT       R1, #1 
    /*0000a0a8*/ SUB         R0, R0, R3 
    /*0000a0ac*/ BL          0x000000000000A0AC          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000a0b0*/ LDR         R3, [R7, #196]              @ 0xc4 
    /*0000a0b4*/ STR         R0, [R7, #192]              @ 0xc0 
    /*0000a0b8*/ LDR         R1, [R4, #8] 
    /*0000a0bc*/ LDR         R0, [R4, #48]               @ 0x30 
    /*0000a0c0*/ CMP         R1, #1 
    /*0000a0c4*/ SUB         R0, R0, R3 
    /*0000a0c8*/ MOVLT       R1, #1 
    /*0000a0cc*/ BL          0x000000000000A0CC          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000a0d0*/ LDRB        IP, [R7, #220]              @ 0xdc 
    /*0000a0d4*/ MOV         R2, #86                     @ 0x56 
    /*0000a0d8*/ LDRB        FP, [R7, #228]              @ 0xe4 
    /*0000a0dc*/ LDR         R3, [R7, #224]              @ 0xe0 
    /*0000a0e0*/ MLA         R2, R2, R8, IP 
    /*0000a0e4*/ ADD         FP, FP, #1 
    /*0000a0e8*/ CMP         FP, #5 
    /*0000a0ec*/ ADD         R2, R2, #50                 @ 0x32 
    /*0000a0f0*/ MOVGE       FP, #5 
    /*0000a0f4*/ STRB        FP, [R7, #228]              @ 0xe4 
    /*0000a0f8*/ MOV         R1, FP 
    /*0000a0fc*/ LDR         LR, [R6, R2, LSL #2] 
    /*0000a100*/ SUB         R3, R3, LR 
    /*0000a104*/ STR         R3, [R7, #224]              @ 0xe0 
    /*0000a108*/ STR         R0, [R7, #196]              @ 0xc4 
    /*0000a10c*/ ADD         R0, IP, #1 
    /*0000a110*/ LDR         LR, [R4, #4] 
    /*0000a114*/ LDR         IP, [R4, #40]               @ 0x28 
    /*0000a118*/ CMP         IP, LR 
    /*0000a11c*/ MOVGE       IP, LR 
    /*0000a120*/ ADD         R3, R3, IP 
    /*0000a124*/ STR         IP, [R6, R2, LSL #2] 
    /*0000a128*/ STR         R3, [R7, #224]              @ 0xe0 
    /*0000a12c*/ STR         R3, [SP, #4] 
    /*0000a130*/ BL          0x000000000000A130          @ __aeabi_idivmod [R_ARM_CALL: +0x0] 
    /*0000a134*/ LDR         R3, [SP, #4] 
    /*0000a138*/ MOV         R0, R3 
    /*0000a13c*/ STRB        R1, [R7, #220]              @ 0xdc 
    /*0000a140*/ MOV         R1, FP 
    /*0000a144*/ BL          0x000000000000A144          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000a148*/ LDR         R1, [SL, #156]              @ 0x9c 
    /*0000a14c*/ CMP         R1, #0 
    /*0000a150*/ MOVEQ       R1, #1 
    /*0000a154*/ LSL         R0, R0, #13 
    /*0000a158*/ BL          0x000000000000A158          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000a15c*/ CMP         R0, #8192                   @ 0x2000 
    /*0000a160*/ MOVLT       R1, R0 
    /*0000a164*/ MOVGE       R1, #8192                   @ 0x2000 
    /*0000a168*/ RSB         R1, R1, #8192               @ 0x2000 
    /*0000a16c*/ CMP         R1, #0 
    /*0000a170*/ BLE         setGmeMotion_xa17c
    /*0000a174*/ MOV         R0, R9 
    /*0000a178*/ BL          0x000000000000815C 
    setGmeMotion_xa17c:
    /*0000a17c*/ MOV         R0, #344                    @ 0x158 
    /*0000a180*/ ADD         R1, R4, #44                 @ 0x2c 
    /*0000a184*/ MLA         R0, R0, R8, R6 
    /*0000a188*/ MOV         R2, #8 
    /*0000a18c*/ ADD         R0, R0, #192                @ 0xc0 
    /*0000a190*/ BL          0x000000000000A190          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000a194*/ MOV         R3, #1 
    /*0000a198*/ STR         R3, [R5, #368]              @ 0x170 
    /*0000a19c*/ ADD         SP, SP, #28 
    /*0000a1a0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    setGmeMotion_xa1a4:
    /*0000a1a4*/ ADD         IP, R5, #432                @ 0x1b0 
    /*0000a1a8*/ LDR         R3, [R5, #28] 
    /*0000a1ac*/ LDR         FP, [R5, #32] 
    /*0000a1b0*/ MOV         R2, #48                     @ 0x30 
    /*0000a1b4*/ MOV         R0, IP 
    /*0000a1b8*/ STR         IP, [SP, #8] 
    /*0000a1bc*/ LSR         LR, R3, #1 
    /*0000a1c0*/ MOV         R9, IP 
    /*0000a1c4*/ LSR         R3, FP, #1 
    /*0000a1c8*/ STR         LR, [SP, #12] 
    /*0000a1cc*/ STR         R3, [SP, #4] 
    /*0000a1d0*/ BL          0x000000000000A1D0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000a1d4*/ LDRD        R0, [R7] 
    /*0000a1d8*/ LDR         R2, [R7, #8] 
    /*0000a1dc*/ LDR         R3, [SP, #4] 
    /*0000a1e0*/ SUBS        SL, R0, #1048576            @ 0x100000 
    /*0000a1e4*/ LDR         LR, [SP, #12] 
    /*0000a1e8*/ SBC         FP, R1, #0 
    /*0000a1ec*/ LDR         IP, [R7, #12] 
    /*0000a1f0*/ UMULL       R0, R1, R2, R3 
    /*0000a1f4*/ UMULL       R2, R3, SL, LR 
    /*0000a1f8*/ STRD        R2, [SP, #16] 
    /*0000a1fc*/ LDR         R3, [SP, #4] 
    /*0000a200*/ MLA         R1, R3, IP, R1 
    /*0000a204*/ LDR         R3, [SP, #20] 
    /*0000a208*/ LDR         IP, [SP, #8] 
    /*0000a20c*/ MLA         R3, LR, FP, R3 
    /*0000a210*/ LDRD        SL, [IP, #16] 
    /*0000a214*/ STR         R3, [SP, #20] 
    /*0000a218*/ LDRD        R2, [SP, #16] 
    /*0000a21c*/ ADDS        R2, R2, R0 
    /*0000a220*/ ADC         R3, R3, R1 
    /*0000a224*/ CMP         R2, #0 
    /*0000a228*/ SBCS        R1, R3, #0 
    /*0000a22c*/ BLT         setGmeMotion_xa328
    /*0000a230*/ ADDS        R2, R2, #512                @ 0x200 
    /*0000a234*/ ADC         R3, R3, #0 
    /*0000a238*/ LSR         R0, R2, #10 
    /*0000a23c*/ ORR         R0, R0, R3, LSL #22 
    /*0000a240*/ ASR         R1, R3, #10 
    setGmeMotion_xa244:
    /*0000a244*/ ADDS        R2, SL, R0 
    /*0000a248*/ LDR         IP, [SP, #8] 
    /*0000a24c*/ ADC         R3, FP, R1 
    /*0000a250*/ MOV         R0, R2 
    /*0000a254*/ MOV         R1, R3 
    /*0000a258*/ STRD        R0, [IP, #16] 
    /*0000a25c*/ LDRD        R0, [R7, #32] 
    /*0000a260*/ LDR         IP, [R7, #24] 
    /*0000a264*/ SUBS        R2, R0, #1048576            @ 0x100000 
    /*0000a268*/ LDR         LR, [R7, #28] 
    /*0000a26c*/ SBC         R3, R1, #0 
    /*0000a270*/ LDR         R1, [SP, #12] 
    /*0000a274*/ MOV         SL, R2 
    /*0000a278*/ LDR         R2, [SP, #4] 
    /*0000a27c*/ MOV         FP, R3 
    /*0000a280*/ UMULL       R0, R1, IP, R1 
    /*0000a284*/ LDR         IP, [SP, #4] 
    /*0000a288*/ UMULL       R2, R3, SL, R2 
    /*0000a28c*/ ADDS        R2, R2, R0 
    /*0000a290*/ MLA         R3, IP, FP, R3 
    /*0000a294*/ LDR         IP, [SP, #8] 
    /*0000a298*/ LDRD        SL, [IP, #40]               @ 0x28 
    /*0000a29c*/ LDR         IP, [SP, #12] 
    /*0000a2a0*/ MLA         R1, IP, LR, R1 
    /*0000a2a4*/ ADC         R3, R3, R1 
    /*0000a2a8*/ CMP         R2, #0 
    /*0000a2ac*/ SBCS        R1, R3, #0 
    /*0000a2b0*/ BLT         setGmeMotion_xa300
    /*0000a2b4*/ ADDS        R2, R2, #512                @ 0x200 
    /*0000a2b8*/ ADC         R3, R3, #0 
    /*0000a2bc*/ LSR         R0, R2, #10 
    /*0000a2c0*/ ORR         R0, R0, R3, LSL #22 
    /*0000a2c4*/ ASR         R1, R3, #10 
    setGmeMotion_xa2c8:
    /*0000a2c8*/ ADDS        R2, SL, R0 
    /*0000a2cc*/ ADC         R3, FP, R1 
    /*0000a2d0*/ MOV         R0, R2 
    /*0000a2d4*/ MOV         R1, R3 
    /*0000a2d8*/ LDR         R3, [SP, #8] 
    /*0000a2dc*/ STRD        R0, [R3, #40]               @ 0x28 
    /*0000a2e0*/ LDR         R3, [R5, #24] 
    /*0000a2e4*/ B           setGmeMotion_x9fb8
    setGmeMotion_xa2e8:
    /*0000a2e8*/ LDR         R0, [R4, #8] 
    /*0000a2ec*/ MOV         R1, #1 
    /*0000a2f0*/ RSB         R0, R0, #0 
    /*0000a2f4*/ USAT        R0, #1, R0 
    /*0000a2f8*/ LSL         R0, R0, #13 
    /*0000a2fc*/ B           setGmeMotion_xa07c
    setGmeMotion_xa300:
    /*0000a300*/ MOV         R0, #512                    @ 0x200 
    /*0000a304*/ SUBS        R2, R0, R2 
    /*0000a308*/ MOV         R1, #0 
    /*0000a30c*/ SBC         R3, R1, R3 
    /*0000a310*/ LSR         R0, R2, #10 
    /*0000a314*/ ORR         R0, R0, R3, LSL #22 
    /*0000a318*/ ASR         R1, R3, #10 
    /*0000a31c*/ RSBS        R0, R0, #0 
    /*0000a320*/ RSC         R1, R1, #0 
    /*0000a324*/ B           setGmeMotion_xa2c8
    setGmeMotion_xa328:
    /*0000a328*/ MOV         R0, #512                    @ 0x200 
    /*0000a32c*/ SUBS        R2, R0, R2 
    /*0000a330*/ MOV         R1, #0 
    /*0000a334*/ SBC         R3, R1, R3 
    /*0000a338*/ LSR         R0, R2, #10 
    /*0000a33c*/ ORR         R0, R0, R3, LSL #22 
    /*0000a340*/ ASR         R1, R3, #10 
    /*0000a344*/ RSBS        R0, R0, #0 
    /*0000a348*/ RSC         R1, R1, #0 
    /*0000a34c*/ B           setGmeMotion_xa244
FUNC_END setGmeMotion


FUNC_BEGIN getStabilizedMotion
    /*0000a350*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000a354*/ SUB         SP, SP, #300                @ 0x12c 
    /*0000a358*/ MOV         R4, R0 
    /*0000a35c*/ MOV         R2, #48                     @ 0x30 
    /*0000a360*/ STR         R0, [SP, #12] 
    /*0000a364*/ MOV         SL, R1 
    /*0000a368*/ ADD         R0, SP, #104                @ 0x68 
    /*0000a36c*/ MOV         R1, #0 
    /*0000a370*/ BL          0x000000000000A370          @ memset [R_ARM_CALL: +0x0] 
    /*0000a374*/ MOV         R2, #48                     @ 0x30 
    /*0000a378*/ MOV         R1, #0 
    /*0000a37c*/ ADD         R0, SP, #152                @ 0x98 
    /*0000a380*/ BL          0x000000000000A380          @ memset [R_ARM_CALL: +0x0] 
    /*0000a384*/ MOV         R2, #48                     @ 0x30 
    /*0000a388*/ ADD         R0, SP, #200                @ 0xc8 
    /*0000a38c*/ MOV         R1, #0 
    /*0000a390*/ BL          0x000000000000A390          @ memset [R_ARM_CALL: +0x0] 
    /*0000a394*/ MOV         R3, #344                    @ 0x158 
    /*0000a398*/ MUL         R3, R3, R4 
    /*0000a39c*/ MOVW        R2, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x16f8] 
    /*0000a3a0*/ MOVT        R2, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x16f8] 
    /*0000a3a4*/ STR         R2, [SP, #8] 
    /*0000a3a8*/ LDR         FP, [R2, R3] 
    /*0000a3ac*/ LDR         R3, [FP, #4] 
    /*0000a3b0*/ CMP         R3, #1 
    /*0000a3b4*/ BLS         getStabilizedMotion_xa6a0
    /*0000a3b8*/ CMP         R3, #2 
    /*0000a3bc*/ BEQ         getStabilizedMotion_xb6d4
    /*0000a3c0*/ LDR         R3, [FP, #812]              @ 0x32c 
    /*0000a3c4*/ LDR         R4, [FP, #312]              @ 0x138 
    /*0000a3c8*/ STR         R3, [SP, #40]               @ 0x28 
    /*0000a3cc*/ CMP         R4, #0 
    /*0000a3d0*/ LDR         R3, [FP, #816]              @ 0x330 
    /*0000a3d4*/ STR         R3, [SP, #16] 
    /*0000a3d8*/ BNE         getStabilizedMotion_xb31c
    /*0000a3dc*/ MOV         R5, R4 
    /*0000a3e0*/ MOV         R6, R4 
    /*0000a3e4*/ MOV         R7, R4 
    /*0000a3e8*/ MOV         R2, #0 
    /*0000a3ec*/ MOV         R3, #0 
    /*0000a3f0*/ STR         R4, [SP, #24] 
    /*0000a3f4*/ STRD        R2, [SP, #64]               @ 0x40 
    /*0000a3f8*/ STRD        R2, [SP, #56]               @ 0x38 
    /*0000a3fc*/ STR         R4, [SP, #48]               @ 0x30 
    /*0000a400*/ STR         R4, [SP, #76]               @ 0x4c 
    /*0000a404*/ STR         R4, [SP, #80]               @ 0x50 
    /*0000a408*/ STR         R4, [SP, #32] 
    /*0000a40c*/ STR         R4, [SP, #72]               @ 0x48 
    getStabilizedMotion_xa410:
    /*0000a410*/ ADD         R3, FP, #680                @ 0x2a8 
    /*0000a414*/ MOV         R2, R6 
    /*0000a418*/ MOV         R1, R7 
    /*0000a41c*/ ADD         R0, FP, #508                @ 0x1fc 
    /*0000a420*/ BL          0x0000000000009B9C 
    /*0000a424*/ LDR         R3, [SP, #16] 
    /*0000a428*/ CMP         R3, #0 
    /*0000a42c*/ STR         R0, [SP, #44]               @ 0x2c 
    /*0000a430*/ BNE         getStabilizedMotion_xb190
    /*0000a434*/ LDR         R1, [FP, #824]              @ 0x338 
    /*0000a438*/ STR         R5, [FP, #824]              @ 0x338 
    /*0000a43c*/ STR         R1, [FP, #820]              @ 0x334 
    getStabilizedMotion_xa440:
    /*0000a440*/ CMP         R5, R1 
    /*0000a444*/ LDR         R3, [SP, #48]               @ 0x30 
    /*0000a448*/ MOVLT       R5, R1 
    /*0000a44c*/ CMP         R5, #1 
    /*0000a450*/ STR         R5, [FP, #828]              @ 0x33c 
    /*0000a454*/ MOVGE       R1, R5 
    /*0000a458*/ MOVLT       R1, #1 
    /*0000a45c*/ CMP         R1, R3 
    /*0000a460*/ BEQ         getStabilizedMotion_xb6c4
    /*0000a464*/ SUB         R8, R3, R1 
    /*0000a468*/ EOR         R3, R8, R8, ASR #31 
    /*0000a46c*/ SUB         R3, R3, R8, ASR #31 
    /*0000a470*/ CMP         R3, #0 
    /*0000a474*/ BLE         getStabilizedMotion_xb7f0
    getStabilizedMotion_xa478:
    /*0000a478*/ MVN         R0, #-16777216              @ 0xff000000 
    /*0000a47c*/ CMP         R3, R0 
    /*0000a480*/ MOVLT       R8, R3 
    /*0000a484*/ MOVGE       R8, R0 
    /*0000a488*/ LSL         R0, R8, #7 
    getStabilizedMotion_xa48c:
    /*0000a48c*/ BL          0x000000000000A48C          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000a490*/ LDR         R6, [FP, #800]              @ 0x320 
    /*0000a494*/ LDR         R3, [FP, #484]              @ 0x1e4 
    /*0000a498*/ MOV         R2, #3 
    /*0000a49c*/ LDR         R7, [FP, #804]              @ 0x324 
    /*0000a4a0*/ LDR         R8, [FP, #480]              @ 0x1e0 
    /*0000a4a4*/ MOV         R9, R3 
    /*0000a4a8*/ LDR         R5, [FP, #500]              @ 0x1f4 
    /*0000a4ac*/ CMP         R0, R6 
    /*0000a4b0*/ LDRD        R0, [SP, #56]               @ 0x38 
    /*0000a4b4*/ MOVLE       R6, #0 
    /*0000a4b8*/ MOVGT       R6, #1 
    /*0000a4bc*/ BL          0x000000000000A4BC          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000a4c0*/ ASR         R3, R7, #31 
    /*0000a4c4*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*0000a4c8*/ CMP         R7, R0 
    /*0000a4cc*/ SBCS        R3, R3, R1 
    /*0000a4d0*/ MOVLT       R3, #1 
    /*0000a4d4*/ MOVGE       R3, #0 
    /*0000a4d8*/ CMP         R2, R5 
    /*0000a4dc*/ MOVGE       R3, #0 
    /*0000a4e0*/ ANDLT       R3, R3, #1 
    /*0000a4e4*/ ORRS        R3, R6, R3 
    /*0000a4e8*/ MOVNE       R8, #0 
    /*0000a4ec*/ BNE         getStabilizedMotion_xa4fc
    /*0000a4f0*/ ADD         R5, R9, #1 
    /*0000a4f4*/ CMP         R5, R8 
    /*0000a4f8*/ MOVCC       R8, R5 
    getStabilizedMotion_xa4fc:
    /*0000a4fc*/ ADD         R3, FP, #720                @ 0x2d0 
    /*0000a500*/ STR         R8, [FP, #484]              @ 0x1e4 
    /*0000a504*/ LDR         R2, [SP, #32] 
    /*0000a508*/ ADD         R0, FP, #564                @ 0x234 
    /*0000a50c*/ LDR         R1, [SP, #72]               @ 0x48 
    /*0000a510*/ BL          0x0000000000009B9C 
    /*0000a514*/ LDR         R3, [SP, #16] 
    /*0000a518*/ CMP         R3, #0 
    /*0000a51c*/ MOV         R7, R0 
    /*0000a520*/ BNE         getStabilizedMotion_xb168
    /*0000a524*/ LDR         R1, [FP, #836]              @ 0x344 
    /*0000a528*/ LDR         R3, [SP, #24] 
    /*0000a52c*/ STR         R1, [FP, #832]              @ 0x340 
    /*0000a530*/ STR         R3, [FP, #836]              @ 0x344 
    getStabilizedMotion_xa534:
    /*0000a534*/ LDR         R3, [SP, #24] 
    /*0000a538*/ CMP         R3, R1 
    /*0000a53c*/ MOVLT       R3, R1 
    /*0000a540*/ CMP         R3, #1 
    /*0000a544*/ STR         R3, [FP, #840]              @ 0x348 
    /*0000a548*/ MOVGE       R1, R3 
    /*0000a54c*/ MOVLT       R1, #1 
    /*0000a550*/ CMP         R1, R4 
    /*0000a554*/ BEQ         getStabilizedMotion_xb6cc
    /*0000a558*/ SUB         R4, R4, R1 
    /*0000a55c*/ EOR         R3, R4, R4, ASR #31 
    /*0000a560*/ SUB         R3, R3, R4, ASR #31 
    /*0000a564*/ CMP         R3, #0 
    /*0000a568*/ BLE         getStabilizedMotion_xb7f8
    getStabilizedMotion_xa56c:
    /*0000a56c*/ MVN         R0, #-16777216              @ 0xff000000 
    /*0000a570*/ CMP         R3, R0 
    /*0000a574*/ MOVLT       R4, R3 
    /*0000a578*/ MOVGE       R4, R0 
    /*0000a57c*/ LSL         R0, R4, #7 
    getStabilizedMotion_xa580:
    /*0000a580*/ BL          0x000000000000A580          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000a584*/ LDR         R5, [FP, #800]              @ 0x320 
    /*0000a588*/ MOV         R2, #3 
    /*0000a58c*/ LDR         R6, [FP, #804]              @ 0x324 
    /*0000a590*/ LDR         R8, [FP, #480]              @ 0x1e0 
    /*0000a594*/ LDR         R9, [FP, #500]              @ 0x1f4 
    /*0000a598*/ LDR         R4, [FP, #488]              @ 0x1e8 
    /*0000a59c*/ CMP         R0, R5 
    /*0000a5a0*/ LDRD        R0, [SP, #64]               @ 0x40 
    /*0000a5a4*/ MOVLE       R5, #0 
    /*0000a5a8*/ MOVGT       R5, #1 
    /*0000a5ac*/ BL          0x000000000000A5AC          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000a5b0*/ ASR         R3, R6, #31 
    /*0000a5b4*/ CMP         R6, R0 
    /*0000a5b8*/ SBCS        R3, R3, R1 
    /*0000a5bc*/ MOVLT       R3, #1 
    /*0000a5c0*/ MOVGE       R3, #0 
    /*0000a5c4*/ CMP         R7, R9 
    /*0000a5c8*/ MOVGE       R7, #0 
    /*0000a5cc*/ ANDLT       R7, R3, #1 
    /*0000a5d0*/ ORRS        R3, R5, R7 
    /*0000a5d4*/ MOVNE       R8, #0 
    /*0000a5d8*/ BNE         getStabilizedMotion_xa5e8
    /*0000a5dc*/ ADD         R4, R4, #1 
    /*0000a5e0*/ CMP         R4, R8 
    /*0000a5e4*/ MOVCC       R8, R4 
    getStabilizedMotion_xa5e8:
    /*0000a5e8*/ LDR         R4, [FP, #484]              @ 0x1e4 
    /*0000a5ec*/ LDR         R3, [FP] 
    /*0000a5f0*/ CMP         R8, R4 
    /*0000a5f4*/ STR         R8, [FP, #488]              @ 0x1e8 
    /*0000a5f8*/ SUB         R3, R3, #1 
    /*0000a5fc*/ MOVCS       R8, R4 
    /*0000a600*/ CMP         R3, #1 
    /*0000a604*/ MOV         R0, R8 
    /*0000a608*/ BLS         getStabilizedMotion_xb800
    getStabilizedMotion_xa60c:
    /*0000a60c*/ MOV         R3, #0 
    getStabilizedMotion_xa610:
    /*0000a610*/ STR         R3, [FP, #492]              @ 0x1ec 
    /*0000a614*/ LDR         R3, [SP, #16] 
    /*0000a618*/ LDR         R2, [SP, #40]               @ 0x28 
    /*0000a61c*/ ADD         R3, R3, #1 
    /*0000a620*/ LDR         R1, [FP, #528]              @ 0x210 
    /*0000a624*/ CMP         R2, R3 
    /*0000a628*/ SUBLS       R3, R3, R2 
    /*0000a62c*/ LDR         R2, [FP, #524]              @ 0x20c 
    /*0000a630*/ STR         R3, [FP, #816]              @ 0x330 
    /*0000a634*/ CMP         R1, R2 
    /*0000a638*/ BCC         getStabilizedMotion_xa6a0
    /*0000a63c*/ LDR         R3, [FP, #312]              @ 0x138 
    /*0000a640*/ CMP         R3, #0 
    /*0000a644*/ BEQ         getStabilizedMotion_xa6a0
    /*0000a648*/ LDR         R2, [FP, #368]              @ 0x170 
    /*0000a64c*/ CMP         R2, #0 
    /*0000a650*/ BEQ         getStabilizedMotion_xb5c4
    /*0000a654*/ LDR         R1, [FP, #480]              @ 0x1e0 
    /*0000a658*/ LDR         R3, [FP, #492]              @ 0x1ec 
    /*0000a65c*/ CMP         R3, R1 
    /*0000a660*/ BEQ         getStabilizedMotion_xba2c
    /*0000a664*/ LDR         R3, [FP, #484]              @ 0x1e4 
    /*0000a668*/ CMP         R1, R3 
    /*0000a66c*/ BEQ         getStabilizedMotion_xba20
    getStabilizedMotion_xa670:
    /*0000a670*/ CMP         R1, #0 
    /*0000a674*/ ADD         R3, FP, #432                @ 0x1b0 
    /*0000a678*/ STR         R2, [FP, #308]              @ 0x134 
    /*0000a67c*/ LSL         R0, R0, #13 
    /*0000a680*/ MOVEQ       R1, #1 
    /*0000a684*/ MOV         R4, R3 
    /*0000a688*/ STR         R3, [SP, #16] 
    /*0000a68c*/ BL          0x000000000000A68C          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000a690*/ MOV         R1, R0 
    /*0000a694*/ MOV         R0, R4 
    /*0000a698*/ BL          0x000000000000815C 
    /*0000a69c*/ B           getStabilizedMotion_xa6b0
    getStabilizedMotion_xa6a0:
    /*0000a6a0*/ LDR         R3, [FP, #368]              @ 0x170 
    /*0000a6a4*/ ADD         R2, FP, #432                @ 0x1b0 
    /*0000a6a8*/ STR         R2, [SP, #16] 
    /*0000a6ac*/ STR         R3, [FP, #308]              @ 0x134 
    getStabilizedMotion_xa6b0:
    /*0000a6b0*/ LDR         R2, [SP, #12] 
    /*0000a6b4*/ MOV         R3, #344                    @ 0x158 
    /*0000a6b8*/ LDR         R0, [SP, #16] 
    /*0000a6bc*/ MOV         R5, #0 
    /*0000a6c0*/ STR         R5, [SP, #96]               @ 0x60 
    /*0000a6c4*/ MUL         R3, R3, R2 
    /*0000a6c8*/ LDR         R2, [SP, #8] 
    /*0000a6cc*/ STR         R5, [SP, #248]              @ 0xf8 
    /*0000a6d0*/ ADD         R4, R3, #280                @ 0x118 
    /*0000a6d4*/ ADD         R6, R2, R3 
    /*0000a6d8*/ ADD         R4, R2, R4 
    /*0000a6dc*/ LDR         R1, [R2, R3] 
    /*0000a6e0*/ STR         R3, [SP, #40]               @ 0x28 
    /*0000a6e4*/ ADD         R2, R1, #28 
    /*0000a6e8*/ ADD         R1, R1, #52                 @ 0x34 
    /*0000a6ec*/ BL          0x0000000000008740 
    /*0000a6f0*/ LDR         R2, [R6, #300]              @ 0x12c 
    /*0000a6f4*/ LDR         R3, [R6, #296]              @ 0x128 
    /*0000a6f8*/ CMP         R2, R3 
    /*0000a6fc*/ STR         R0, [SP, #96]               @ 0x60 
    /*0000a700*/ BCS         getStabilizedMotion_xaf7c
    /*0000a704*/ LDR         R2, [R6, #336]              @ 0x150 
    /*0000a708*/ ADD         R3, SP, #248                @ 0xf8 
    /*0000a70c*/ STR         R3, [SP, #24] 
    getStabilizedMotion_xa710:
    /*0000a710*/ ADD         R0, R2, R0 
    /*0000a714*/ LDR         R1, [SP, #8] 
    /*0000a718*/ LDR         R2, [SP, #12] 
    /*0000a71c*/ MOV         R3, #344                    @ 0x158 
    /*0000a720*/ MLA         R3, R3, R2, R1 
    /*0000a724*/ LDR         R2, [R3, #324]              @ 0x144 
    /*0000a728*/ STR         R0, [R3, #336]              @ 0x150 
    /*0000a72c*/ CMP         R2, #0 
    /*0000a730*/ BEQ         getStabilizedMotion_xa740
    /*0000a734*/ MOV         R0, R4 
    /*0000a738*/ ADD         R1, SP, #96                 @ 0x60 
    /*0000a73c*/ BLX         R2 
    getStabilizedMotion_xa740:
    /*0000a740*/ LDR         R1, [SP, #8] 
    /*0000a744*/ MOV         R3, #344                    @ 0x158 
    /*0000a748*/ LDR         R2, [SP, #12] 
    /*0000a74c*/ MLA         R3, R3, R2, R1 
    /*0000a750*/ LDR         R1, [R3, #300]              @ 0x12c 
    /*0000a754*/ CMP         R1, #0 
    /*0000a758*/ MOVEQ       R0, R1 
    /*0000a75c*/ BEQ         getStabilizedMotion_xa768
    /*0000a760*/ LDR         R0, [R3, #336]              @ 0x150 
    /*0000a764*/ BL          0x000000000000A764          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    getStabilizedMotion_xa768:
    /*0000a768*/ LDR         R3, [FP, #264]              @ 0x108 
    /*0000a76c*/ STR         R0, [FP, #304]              @ 0x130 
    /*0000a770*/ CMP         R3, #0 
    /*0000a774*/ BEQ         getStabilizedMotion_xa7cc
    /*0000a778*/ LDR         R3, [FP, #308]              @ 0x134 
    /*0000a77c*/ CMP         R3, #0 
    /*0000a780*/ BNE         getStabilizedMotion_xb3c0
    /*0000a784*/ LDR         R0, [FP, #276]              @ 0x114 
    /*0000a788*/ MOV         R2, #95                     @ 0x5f 
    /*0000a78c*/ LDR         R3, [FP, #272]              @ 0x110 
    /*0000a790*/ MOVW        R4, #34079                  @ 0x851f 
    /*0000a794*/ MOVT        R4, #20971                  @ 0x51eb 
    /*0000a798*/ MOV         R1, #1 
    /*0000a79c*/ STR         R1, [FP, #280]              @ 0x118 
    /*0000a7a0*/ MUL         R3, R2, R3 
    /*0000a7a4*/ STR         R1, [FP, #284]              @ 0x11c 
    /*0000a7a8*/ MUL         R2, R2, R0 
    /*0000a7ac*/ SMULL       R0, R1, R3, R4 
    /*0000a7b0*/ ASR         R3, R3, #31 
    /*0000a7b4*/ SMULL       R4, R5, R2, R4 
    /*0000a7b8*/ ASR         R2, R2, #31 
    /*0000a7bc*/ RSB         R3, R3, R1, ASR #5 
    /*0000a7c0*/ STR         R3, [FP, #272]              @ 0x110 
    /*0000a7c4*/ RSB         R3, R2, R5, ASR #5 
    /*0000a7c8*/ STR         R3, [FP, #276]              @ 0x114 
    getStabilizedMotion_xa7cc:
    /*0000a7cc*/ LDR         R6, [SP, #40]               @ 0x28 
    /*0000a7d0*/ ADD         R2, SP, #104                @ 0x68 
    /*0000a7d4*/ LDR         R5, [SP, #8] 
    /*0000a7d8*/ ADD         R3, R6, #8 
    /*0000a7dc*/ LDR         R7, [SP, #16] 
    /*0000a7e0*/ ADD         R4, R5, R3 
    /*0000a7e4*/ STR         R4, [SP, #32] 
    /*0000a7e8*/ MOV         R0, R7 
    /*0000a7ec*/ MOV         R1, R4 
    /*0000a7f0*/ BL          0x0000000000008C40 
    /*0000a7f4*/ ADD         R1, SP, #104                @ 0x68 
    /*0000a7f8*/ MOV         R0, R4 
    /*0000a7fc*/ MOV         R2, #48                     @ 0x30 
    /*0000a800*/ BL          0x000000000000A800          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000a804*/ LDR         R1, [SP, #12] 
    /*0000a808*/ MOV         R3, #344                    @ 0x158 
    /*0000a80c*/ MLA         R3, R3, R1, R5 
    /*0000a810*/ LDR         R3, [R3, #140]              @ 0x8c 
    /*0000a814*/ CMP         R3, #0 
    /*0000a818*/ BEQ         getStabilizedMotion_xa82c
    /*0000a81c*/ ADD         R0, R6, #104                @ 0x68 
    /*0000a820*/ MOV         R1, R7 
    /*0000a824*/ ADD         R0, R5, R0 
    /*0000a828*/ BLX         R3 
    getStabilizedMotion_xa82c:
    /*0000a82c*/ LDR         R3, [FP, #12] 
    /*0000a830*/ CMP         R3, #1 
    /*0000a834*/ BEQ         getStabilizedMotion_xb2c8
    /*0000a838*/ ADD         R3, SP, #200                @ 0xc8 
    /*0000a83c*/ ADD         R2, FP, #200                @ 0xc8 
    /*0000a840*/ ADD         R1, FP, #272                @ 0x110 
    /*0000a844*/ LDR         R0, [SP, #12] 
    /*0000a848*/ BL          0x00000000000093C0 
    /*0000a84c*/ LDR         R3, [SP, #40]               @ 0x28 
    /*0000a850*/ LDR         R2, [SP, #8] 
    /*0000a854*/ ADD         R3, R3, #56                 @ 0x38 
    /*0000a858*/ ADD         R3, R2, R3 
    /*0000a85c*/ STR         R3, [SP, #44]               @ 0x2c 
    getStabilizedMotion_xa860:
    /*0000a860*/ LDR         R3, [FP, #308]              @ 0x134 
    /*0000a864*/ CMP         R3, #0 
    /*0000a868*/ BNE         getStabilizedMotion_xaf50
    /*0000a86c*/ LDR         R2, [SP, #12] 
    /*0000a870*/ MOV         R3, #344                    @ 0x158 
    /*0000a874*/ MUL         R3, R3, R2 
    /*0000a878*/ LDR         R2, [SP, #8] 
    /*0000a87c*/ LDR         R4, [R2, R3] 
    /*0000a880*/ LDR         R3, [R4, #172]              @ 0xac 
    /*0000a884*/ CMP         R3, #0 
    /*0000a888*/ LDREQ       R1, [R4, #180]              @ 0xb4 
    /*0000a88c*/ STREQ       R1, [R4, #192]              @ 0xc0 
    /*0000a890*/ BNE         getStabilizedMotion_xaf70
    getStabilizedMotion_xa894:
    /*0000a894*/ LDR         R5, [SP, #32] 
    /*0000a898*/ MOV         R0, R5 
    /*0000a89c*/ BL          0x000000000000815C 
    /*0000a8a0*/ LDR         R1, [R4, #192]              @ 0xc0 
    /*0000a8a4*/ LDR         R4, [SP, #44]               @ 0x2c 
    /*0000a8a8*/ MOV         R0, R4 
    /*0000a8ac*/ BL          0x000000000000815C 
    /*0000a8b0*/ ADD         R1, SP, #104                @ 0x68 
    /*0000a8b4*/ MOV         R0, R5 
    /*0000a8b8*/ BL          0x0000000000008594 
    /*0000a8bc*/ MOV         R0, R4 
    /*0000a8c0*/ ADD         R2, SP, #152                @ 0x98 
    /*0000a8c4*/ ADD         R1, SP, #104                @ 0x68 
    /*0000a8c8*/ BL          0x0000000000008C40 
    /*0000a8cc*/ LDR         R4, [SP, #24] 
    /*0000a8d0*/ LDR         R3, [FP, #200]              @ 0xc8 
    /*0000a8d4*/ MOV         R2, #48                     @ 0x30 
    /*0000a8d8*/ MOV         R1, #0 
    /*0000a8dc*/ MOV         R0, R4 
    /*0000a8e0*/ STR         R3, [SP, #72]               @ 0x48 
    /*0000a8e4*/ BL          0x000000000000A8E4          @ memset [R_ARM_CALL: +0x0] 
    /*0000a8e8*/ MOV         R1, R4 
    /*0000a8ec*/ ADD         R0, SP, #152                @ 0x98 
    /*0000a8f0*/ BL          0x0000000000008594 
    /*0000a8f4*/ LDR         R3, [FP, #24] 
    /*0000a8f8*/ CMP         R3, #0 
    /*0000a8fc*/ LDREQ       R4, [FP, #28] 
    /*0000a900*/ BEQ         getStabilizedMotion_xa914
    /*0000a904*/ CMP         R3, #1 
    /*0000a908*/ LDRNE       R4, [FP, #28] 
    /*0000a90c*/ MOVNE       R3, #0 
    /*0000a910*/ BEQ         getStabilizedMotion_xb5a4
    getStabilizedMotion_xa914:
    /*0000a914*/ LDR         R2, [FP, #32] 
    /*0000a918*/ LSR         R1, R4, #1 
    /*0000a91c*/ STR         R3, [SP, #64]               @ 0x40 
    /*0000a920*/ STR         R1, [SP, #48]               @ 0x30 
    /*0000a924*/ LSR         R2, R2, #1 
    /*0000a928*/ STR         R1, [SP, #24] 
    /*0000a92c*/ STR         R2, [SP, #56]               @ 0x38 
    /*0000a930*/ MOV         R5, R2 
    getStabilizedMotion_xa934:
    /*0000a934*/ LDR         R6, [SP, #280]              @ 0x118 
    /*0000a938*/ LDR         R1, [SP, #248]              @ 0xf8 
    /*0000a93c*/ LDR         R8, [SP, #272]              @ 0x110 
    /*0000a940*/ LDR         LR, [SP, #252]              @ 0xfc 
    /*0000a944*/ LSR         R7, R6, #6 
    /*0000a948*/ LDR         IP, [SP, #276]              @ 0x114 
    /*0000a94c*/ LSR         R1, R1, #6 
    /*0000a950*/ LDR         R0, [SP, #284]              @ 0x11c 
    /*0000a954*/ LSR         R8, R8, #6 
    /*0000a958*/ ORR         R1, R1, LR, LSL #26 
    /*0000a95c*/ ASR         LR, LR, #6 
    /*0000a960*/ ORR         R8, R8, IP, LSL #26 
    /*0000a964*/ ASR         IP, IP, #6 
    /*0000a968*/ STR         IP, [SP, #76]               @ 0x4c 
    /*0000a96c*/ ORR         IP, R7, R0, LSL #26 
    /*0000a970*/ LDR         R7, [SP, #24] 
    /*0000a974*/ ASR         R0, R0, #6 
    /*0000a978*/ MUL         R6, R1, R3 
    /*0000a97c*/ LDR         R2, [SP, #256]              @ 0x100 
    /*0000a980*/ MLA         LR, R7, LR, R6 
    /*0000a984*/ LDR         R9, [SP, #260]              @ 0x104 
    /*0000a988*/ UMULL       R6, R7, R1, R7 
    /*0000a98c*/ LSR         R2, R2, #6 
    /*0000a990*/ MUL         R1, R8, R3 
    /*0000a994*/ ADD         R3, LR, R7 
    /*0000a998*/ LDR         LR, [SP, #64]               @ 0x40 
    /*0000a99c*/ ORR         R2, R2, R9, LSL #26 
    /*0000a9a0*/ ASR         R9, R9, #6 
    /*0000a9a4*/ MOV         R7, R3 
    /*0000a9a8*/ STR         IP, [SP, #80]               @ 0x50 
    /*0000a9ac*/ MUL         R3, R2, LR 
    /*0000a9b0*/ MLA         R9, R5, R9, R3 
    /*0000a9b4*/ UMULL       R2, R3, R2, R5 
    /*0000a9b8*/ MUL         LR, IP, LR 
    /*0000a9bc*/ ADDS        R6, R6, R2 
    /*0000a9c0*/ ADD         R3, R9, R3 
    /*0000a9c4*/ LDR         R9, [SP, #268]              @ 0x10c 
    /*0000a9c8*/ ADC         R7, R7, R3 
    /*0000a9cc*/ LDR         R3, [SP, #76]               @ 0x4c 
    /*0000a9d0*/ STRD        R6, [SP, #64]               @ 0x40 
    /*0000a9d4*/ LDR         R6, [SP, #24] 
    /*0000a9d8*/ MLA         R0, R5, R0, LR 
    /*0000a9dc*/ LDR         IP, [SP, #264]              @ 0x108 
    /*0000a9e0*/ MLA         R1, R6, R3, R1 
    /*0000a9e4*/ LSL         R3, R9, #4 
    /*0000a9e8*/ UMULL       R6, R7, R8, R6 
    /*0000a9ec*/ LDRD        R8, [SP, #64]               @ 0x40 
    /*0000a9f0*/ LSL         R2, IP, #4 
    /*0000a9f4*/ LDR         LR, [SP, #292]              @ 0x124 
    /*0000a9f8*/ ADD         R1, R1, R7 
    /*0000a9fc*/ ADDS        R8, R8, R2 
    /*0000aa00*/ ORR         R3, R3, IP, LSR #28 
    /*0000aa04*/ LDR         IP, [SP, #288]              @ 0x120 
    /*0000aa08*/ ADC         R9, R9, R3 
    /*0000aa0c*/ STRD        R6, [SP, #24] 
    /*0000aa10*/ STR         R1, [SP, #28] 
    /*0000aa14*/ MOV         R3, R9 
    /*0000aa18*/ LDR         R1, [SP, #80]               @ 0x50 
    /*0000aa1c*/ LSL         R9, LR, #4 
    /*0000aa20*/ ORR         R9, R9, IP, LSR #28 
    /*0000aa24*/ UMULL       R6, R7, R1, R5 
    /*0000aa28*/ LDR         R1, [FP, #44]               @ 0x2c 
    /*0000aa2c*/ ADD         R7, R0, R7 
    /*0000aa30*/ LDR         R0, [SP, #72]               @ 0x48 
    /*0000aa34*/ MUL         R5, R1, R0 
    /*0000aa38*/ LDRD        R0, [SP, #24] 
    /*0000aa3c*/ ADDS        R0, R0, R6 
    /*0000aa40*/ ADC         R1, R1, R7 
    /*0000aa44*/ MOV         R6, R0 
    /*0000aa48*/ LSR         R0, R8, #14 
    /*0000aa4c*/ LSL         R8, IP, #4 
    /*0000aa50*/ ORR         R0, R0, R3, LSL #18 
    /*0000aa54*/ ADDS        R2, R6, R8 
    /*0000aa58*/ ADC         R3, R1, R9 
    /*0000aa5c*/ ASR         R1, R5, #10 
    /*0000aa60*/ CMP         R4, #0 
    /*0000aa64*/ MOV         R7, R3 
    /*0000aa68*/ LDR         R3, [SP, #48]               @ 0x30 
    /*0000aa6c*/ MOVEQ       R4, #1 
    /*0000aa70*/ SUB         R3, R0, R3 
    /*0000aa74*/ EOR         R0, R3, R3, ASR #31 
    /*0000aa78*/ SUB         R0, R0, R3, ASR #31 
    /*0000aa7c*/ LSR         R3, R2, #14 
    /*0000aa80*/ LDR         R2, [SP, #56]               @ 0x38 
    /*0000aa84*/ ORR         R3, R3, R7, LSL #18 
    /*0000aa88*/ SUB         R0, R0, R1 
    /*0000aa8c*/ SUB         R3, R3, R2 
    /*0000aa90*/ BIC         R0, R0, R0, ASR #31 
    /*0000aa94*/ EOR         R5, R3, R3, ASR #31 
    /*0000aa98*/ SUB         R5, R5, R3, ASR #31 
    /*0000aa9c*/ LSL         R0, R0, #13 
    /*0000aaa0*/ SUB         R5, R5, R1 
    /*0000aaa4*/ MOV         R1, R4 
    /*0000aaa8*/ BL          0x000000000000AAA8          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000aaac*/ MOV         R1, R4 
    /*0000aab0*/ BIC         R5, R5, R5, ASR #31 
    /*0000aab4*/ MOV         R4, #20 
    /*0000aab8*/ MOV         R6, R0 
    /*0000aabc*/ LSL         R0, R5, #13 
    /*0000aac0*/ BL          0x000000000000AAC0          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000aac4*/ CMP         R6, R0 
    /*0000aac8*/ MOVGE       R3, R6 
    /*0000aacc*/ MOVLT       R3, R0 
    /*0000aad0*/ MUL         R4, R4, R3 
    /*0000aad4*/ CMP         R4, #0 
    /*0000aad8*/ BLT         getStabilizedMotion_xb2e4
    /*0000aadc*/ CMP         R4, #8192                   @ 0x2000 
    /*0000aae0*/ MOVGE       R4, #8192                   @ 0x2000 
    /*0000aae4*/ CMP         R4, #0 
    /*0000aae8*/ BEQ         getStabilizedMotion_xb2e4
    getStabilizedMotion_xaaec:
    /*0000aaec*/ LDR         R5, [SP, #32] 
    /*0000aaf0*/ MOV         R1, R4 
    /*0000aaf4*/ MOV         R0, R5 
    /*0000aaf8*/ BL          0x000000000000815C 
    /*0000aafc*/ LDR         R6, [SP, #44]               @ 0x2c 
    /*0000ab00*/ MOV         R1, R4 
    /*0000ab04*/ MOV         R0, R6 
    /*0000ab08*/ BL          0x000000000000815C 
    /*0000ab0c*/ MOV         R0, R5 
    /*0000ab10*/ ADD         R1, SP, #104                @ 0x68 
    /*0000ab14*/ ADD         R5, FP, #208                @ 0xd0 
    /*0000ab18*/ BL          0x0000000000008594 
    /*0000ab1c*/ ADD         R2, SP, #152                @ 0x98 
    /*0000ab20*/ MOV         R1, R6 
    /*0000ab24*/ ADD         R0, SP, #104                @ 0x68 
    /*0000ab28*/ BL          0x0000000000008C40 
    /*0000ab2c*/ MOV         R2, R4 
    /*0000ab30*/ LDR         R0, [SP, #16] 
    /*0000ab34*/ MOV         R1, R5 
    /*0000ab38*/ ADD         R3, SP, #104                @ 0x68 
    /*0000ab3c*/ BL          0x00000000000082E4 
    /*0000ab40*/ MOV         R0, R5 
    /*0000ab44*/ MOV         R2, #48                     @ 0x30 
    /*0000ab48*/ ADD         R1, SP, #104                @ 0x68 
    /*0000ab4c*/ BL          0x000000000000AB4C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    getStabilizedMotion_xab50:
    /*0000ab50*/ LDR         R3, [FP, #264]              @ 0x108 
    /*0000ab54*/ CMP         R3, #0 
    /*0000ab58*/ BEQ         getStabilizedMotion_xac50
    /*0000ab5c*/ LDR         R3, [FP, #280]              @ 0x118 
    /*0000ab60*/ CMP         R3, #0 
    /*0000ab64*/ BEQ         getStabilizedMotion_xac44
    /*0000ab68*/ LDRD        R0, [SP, #152]              @ 0x98 
    /*0000ab6c*/ LDR         IP, [FP, #272]              @ 0x110 
    /*0000ab70*/ CMP         R0, #0 
    /*0000ab74*/ LDRD        R6, [SP, #160]              @ 0xa0 
    /*0000ab78*/ SBCS        R3, R1, #0 
    /*0000ab7c*/ ASR         R9, IP, #31 
    /*0000ab80*/ BLT         getStabilizedMotion_xb5d4
    /*0000ab84*/ ADDS        R4, R0, #64                 @ 0x40 
    /*0000ab88*/ ADC         R5, R1, #0 
    /*0000ab8c*/ LSR         R3, R4, #7 
    /*0000ab90*/ ORR         R3, R3, R5, LSL #25 
    /*0000ab94*/ MOV         R0, R3 
    /*0000ab98*/ ASR         R3, R5, #7 
    /*0000ab9c*/ MOV         R1, R3 
    getStabilizedMotion_xaba0:
    /*0000aba0*/ MUL         R3, R0, R9 
    /*0000aba4*/ MLA         R3, IP, R1, R3 
    /*0000aba8*/ UMULL       R0, R1, R0, IP 
    /*0000abac*/ ADD         R3, R3, R1 
    /*0000abb0*/ CMP         R0, #0 
    /*0000abb4*/ MOV         R1, R3 
    /*0000abb8*/ SBCS        R3, R1, #0 
    /*0000abbc*/ BLT         getStabilizedMotion_xb8c4
    /*0000abc0*/ ADDS        R0, R0, #32 
    /*0000abc4*/ ADC         R1, R1, #0 
    /*0000abc8*/ LSR         R2, R0, #6 
    /*0000abcc*/ ORR         R2, R2, R1, LSL #26 
    /*0000abd0*/ ASR         R3, R1, #6 
    getStabilizedMotion_xabd4:
    /*0000abd4*/ LDRD        R0, [SP, #176]              @ 0xb0 
    /*0000abd8*/ ADDS        R4, R2, R6 
    /*0000abdc*/ ADC         R5, R3, R7 
    /*0000abe0*/ LDRD        R6, [SP, #184]              @ 0xb8 
    /*0000abe4*/ CMP         R0, #0 
    /*0000abe8*/ STRD        R4, [SP, #160]              @ 0xa0 
    /*0000abec*/ SBCS        R3, R1, #0 
    /*0000abf0*/ BLT         getStabilizedMotion_xb89c
    /*0000abf4*/ ADDS        R0, R0, #64                 @ 0x40 
    /*0000abf8*/ ADC         R1, R1, #0 
    /*0000abfc*/ LSR         R4, R0, #7 
    /*0000ac00*/ ORR         R4, R4, R1, LSL #25 
    /*0000ac04*/ ASR         R5, R1, #7 
    getStabilizedMotion_xac08:
    /*0000ac08*/ MUL         R1, R4, R9 
    /*0000ac0c*/ MLA         R1, IP, R5, R1 
    /*0000ac10*/ UMULL       R4, R5, R4, IP 
    /*0000ac14*/ ADD         R5, R1, R5 
    /*0000ac18*/ CMP         R4, #0 
    /*0000ac1c*/ SBCS        R3, R5, #0 
    /*0000ac20*/ BLT         getStabilizedMotion_xb8ec
    /*0000ac24*/ ADDS        R2, R4, #32 
    /*0000ac28*/ ADC         R3, R5, #0 
    /*0000ac2c*/ LSR         R4, R2, #6 
    /*0000ac30*/ ORR         R4, R4, R3, LSL #26 
    /*0000ac34*/ ASR         R5, R3, #6 
    getStabilizedMotion_xac38:
    /*0000ac38*/ ADDS        R0, R4, R6 
    /*0000ac3c*/ ADC         R1, R5, R7 
    /*0000ac40*/ STRD        R0, [SP, #184]              @ 0xb8 
    getStabilizedMotion_xac44:
    /*0000ac44*/ LDR         R3, [FP, #284]              @ 0x11c 
    /*0000ac48*/ CMP         R3, #0 
    /*0000ac4c*/ BNE         getStabilizedMotion_xafec
    getStabilizedMotion_xac50:
    /*0000ac50*/ ADD         R1, SP, #152                @ 0x98 
    /*0000ac54*/ MOV         R0, FP 
    /*0000ac58*/ BL          0x00000000000088D8 
    /*0000ac5c*/ CMP         R0, #0 
    /*0000ac60*/ BNE         getStabilizedMotion_xb1b0
    /*0000ac64*/ LDR         R3, [FP, #24] 
    /*0000ac68*/ CMP         R3, #1 
    /*0000ac6c*/ BEQ         getStabilizedMotion_xb1cc
    getStabilizedMotion_xac70:
    /*0000ac70*/ ADD         R1, SP, #104                @ 0x68 
    /*0000ac74*/ ADD         R0, SP, #152                @ 0x98 
    /*0000ac78*/ BL          0x0000000000008594 
    /*0000ac7c*/ ADD         R1, SP, #104                @ 0x68 
    /*0000ac80*/ ADD         R0, SP, #152                @ 0x98 
    /*0000ac84*/ MOV         R2, #48                     @ 0x30 
    /*0000ac88*/ BL          0x000000000000AC88          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000ac8c*/ LDRB        R4, [FP, #300]              @ 0x12c 
    /*0000ac90*/ SUB         R5, R4, #10 
    /*0000ac94*/ UXTB        R3, R5 
    /*0000ac98*/ CMP         R3, #10 
    /*0000ac9c*/ BHI         getStabilizedMotion_xb130
    /*0000aca0*/ RSB         R4, R4, #20 
    /*0000aca4*/ LDRD        R0, [SP, #152]              @ 0x98 
    /*0000aca8*/ MOV         R2, R4 
    /*0000acac*/ BL          0x000000000000ACAC          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000acb0*/ MOV         R2, R4 
    /*0000acb4*/ STRD        R0, [SL] 
    /*0000acb8*/ LDRD        R0, [SP, #160]              @ 0xa0 
    /*0000acbc*/ BL          0x000000000000ACBC          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000acc0*/ MOV         R2, R5 
    /*0000acc4*/ STRD        R0, [SL, #8] 
    /*0000acc8*/ LDRD        R0, [SP, #168]              @ 0xa8 
    /*0000accc*/ BL          0x000000000000ACCC          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*0000acd0*/ MOV         R2, R4 
    /*0000acd4*/ STRD        R0, [SL, #16] 
    /*0000acd8*/ LDRD        R0, [SP, #176]              @ 0xb0 
    /*0000acdc*/ BL          0x000000000000ACDC          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000ace0*/ MOV         R2, R4 
    /*0000ace4*/ STRD        R0, [SL, #24] 
    /*0000ace8*/ LDRD        R0, [SP, #184]              @ 0xb8 
    /*0000acec*/ BL          0x000000000000ACEC          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000acf0*/ MOV         R2, R5 
    /*0000acf4*/ STRD        R0, [SL, #32] 
    /*0000acf8*/ LDRD        R0, [SP, #192]              @ 0xc0 
    /*0000acfc*/ BL          0x000000000000ACFC          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*0000ad00*/ MOV         R2, R0 
    /*0000ad04*/ MOV         R3, R1 
    /*0000ad08*/ STRD        R2, [SP, #48]               @ 0x30 
    /*0000ad0c*/ STRD        R2, [SL, #40]               @ 0x28 
    getStabilizedMotion_xad10:
    /*0000ad10*/ LDR         R5, [SL, #4] 
    /*0000ad14*/ LDR         R3, [FP, #44]               @ 0x2c 
    /*0000ad18*/ LDR         R1, [SL, #28] 
    /*0000ad1c*/ STR         R5, [SP, #32] 
    /*0000ad20*/ LDR         R5, [SL, #12] 
    /*0000ad24*/ ASR         R7, R3, #31 
    /*0000ad28*/ LDR         R2, [FP, #48]               @ 0x30 
    /*0000ad2c*/ LDR         R4, [SL] 
    /*0000ad30*/ LDR         IP, [SL, #24] 
    /*0000ad34*/ STR         R5, [SP, #44]               @ 0x2c 
    /*0000ad38*/ MOV         R8, R2 
    /*0000ad3c*/ MUL         R5, R3, R1 
    /*0000ad40*/ ASR         R9, R2, #31 
    /*0000ad44*/ LDR         R0, [SL, #36]               @ 0x24 
    /*0000ad48*/ STRD        R8, [SP, #16] 
    /*0000ad4c*/ STR         R1, [SP, #56]               @ 0x38 
    /*0000ad50*/ MUL         R8, R4, R7 
    /*0000ad54*/ LDR         R9, [SL, #32] 
    /*0000ad58*/ MLA         R1, IP, R7, R5 
    /*0000ad5c*/ LDR         R5, [SP, #20] 
    /*0000ad60*/ UMULL       R6, R7, R3, IP 
    /*0000ad64*/ STR         R9, [SP, #72]               @ 0x48 
    /*0000ad68*/ STR         R0, [SP, #64]               @ 0x40 
    /*0000ad6c*/ MUL         R0, R2, R0 
    /*0000ad70*/ LDR         LR, [SL, #8] 
    /*0000ad74*/ MLA         R9, R9, R5, R0 
    /*0000ad78*/ ADD         R0, R1, R7 
    /*0000ad7c*/ LDR         R1, [SP, #72]               @ 0x48 
    /*0000ad80*/ STRD        R6, [SP, #24] 
    /*0000ad84*/ STR         R0, [SP, #28] 
    /*0000ad88*/ UMULL       R6, R7, R2, R1 
    /*0000ad8c*/ LDR         R1, [SP, #32] 
    /*0000ad90*/ MUL         R5, LR, R5 
    /*0000ad94*/ ADD         R7, R9, R7 
    /*0000ad98*/ MLA         R0, R3, R1, R8 
    /*0000ad9c*/ LDRD        R8, [SP, #24] 
    /*0000ada0*/ LDR         R1, [SP, #44]               @ 0x2c 
    /*0000ada4*/ ADDS        R8, R8, R6 
    /*0000ada8*/ ADC         R9, R9, R7 
    /*0000adac*/ STRD        R8, [SP, #16] 
    /*0000adb0*/ UMULL       R8, R9, R4, R3 
    /*0000adb4*/ MLA         R5, R2, R1, R5 
    /*0000adb8*/ ADD         R9, R0, R9 
    /*0000adbc*/ UMULL       R6, R7, LR, R2 
    /*0000adc0*/ LDRD        R0, [SP, #48]               @ 0x30 
    /*0000adc4*/ LDRD        R2, [SP, #16] 
    /*0000adc8*/ ADD         R7, R5, R7 
    /*0000adcc*/ ADDS        R0, R0, R2 
    /*0000add0*/ ADC         R1, R1, R3 
    /*0000add4*/ MOV         R2, R0 
    /*0000add8*/ MOV         R3, R1 
    /*0000addc*/ LDRD        R0, [SL, #16] 
    /*0000ade0*/ STRD        R2, [SL, #40]               @ 0x28 
    /*0000ade4*/ ADDS        R2, R8, R6 
    /*0000ade8*/ ADC         R3, R9, R7 
    /*0000adec*/ ADDS        R0, R0, R2 
    /*0000adf0*/ ADC         R1, R1, R3 
    /*0000adf4*/ MOV         R2, R0 
    /*0000adf8*/ MOV         R3, R1 
    /*0000adfc*/ STRD        R2, [SL, #16] 
    /*0000ae00*/ LDR         R5, [FP, #188]              @ 0xbc 
    /*0000ae04*/ CMP         R5, #8192                   @ 0x2000 
    /*0000ae08*/ BEQ         getStabilizedMotion_xaf0c
    /*0000ae0c*/ LDR         R1, [SP, #32] 
    /*0000ae10*/ ASR         R3, R5, #31 
    /*0000ae14*/ UMULL       R6, R7, R5, R4 
    /*0000ae18*/ MUL         R0, R5, R1 
    /*0000ae1c*/ CMP         R6, #0 
    /*0000ae20*/ MLA         R0, R4, R3, R0 
    /*0000ae24*/ ADD         R7, R0, R7 
    /*0000ae28*/ SBCS        R3, R7, #0 
    /*0000ae2c*/ BLT         getStabilizedMotion_xb624
    /*0000ae30*/ ADDS        R4, R6, #4096               @ 0x1000 
    /*0000ae34*/ ADC         R5, R7, #0 
    /*0000ae38*/ LSR         R0, R4, #13 
    /*0000ae3c*/ ORR         R0, R0, R5, LSL #19 
    /*0000ae40*/ ASR         R1, R5, #13 
    getStabilizedMotion_xae44:
    /*0000ae44*/ STRD        R0, [SL] 
    /*0000ae48*/ LDR         R0, [FP, #188]              @ 0xbc 
    /*0000ae4c*/ LDR         R3, [SP, #44]               @ 0x2c 
    /*0000ae50*/ ASR         R5, R0, #31 
    /*0000ae54*/ MUL         R2, R0, R3 
    /*0000ae58*/ MLA         R4, LR, R5, R2 
    /*0000ae5c*/ UMULL       R0, R1, R0, LR 
    /*0000ae60*/ ADD         R1, R4, R1 
    /*0000ae64*/ CMP         R0, #0 
    /*0000ae68*/ SBCS        R3, R1, #0 
    /*0000ae6c*/ BLT         getStabilizedMotion_xb674
    /*0000ae70*/ ADDS        R2, R0, #4096               @ 0x1000 
    /*0000ae74*/ ADC         R3, R1, #0 
    /*0000ae78*/ LSR         R0, R2, #13 
    /*0000ae7c*/ ORR         R0, R0, R3, LSL #19 
    /*0000ae80*/ ASR         R1, R3, #13 
    getStabilizedMotion_xae84:
    /*0000ae84*/ STRD        R0, [SL, #8] 
    /*0000ae88*/ LDR         R0, [FP, #188]              @ 0xbc 
    /*0000ae8c*/ LDR         R3, [SP, #56]               @ 0x38 
    /*0000ae90*/ ASR         R5, R0, #31 
    /*0000ae94*/ MUL         R2, R0, R3 
    /*0000ae98*/ MLA         LR, IP, R5, R2 
    /*0000ae9c*/ UMULL       R0, R1, R0, IP 
    /*0000aea0*/ ADD         R1, LR, R1 
    /*0000aea4*/ CMP         R0, #0 
    /*0000aea8*/ SBCS        R3, R1, #0 
    /*0000aeac*/ BLT         getStabilizedMotion_xb69c
    /*0000aeb0*/ ADDS        R2, R0, #4096               @ 0x1000 
    /*0000aeb4*/ ADC         R3, R1, #0 
    /*0000aeb8*/ LSR         R0, R2, #13 
    /*0000aebc*/ ORR         R0, R0, R3, LSL #19 
    /*0000aec0*/ ASR         R1, R3, #13 
    getStabilizedMotion_xaec4:
    /*0000aec4*/ STRD        R0, [SL, #24] 
    /*0000aec8*/ LDR         IP, [FP, #188]              @ 0xbc 
    /*0000aecc*/ LDR         R1, [SP, #64]               @ 0x40 
    /*0000aed0*/ ASR         R3, IP, #31 
    /*0000aed4*/ MUL         R0, IP, R1 
    /*0000aed8*/ LDR         R1, [SP, #72]               @ 0x48 
    /*0000aedc*/ MLA         R0, R1, R3, R0 
    /*0000aee0*/ UMULL       R4, R5, IP, R1 
    /*0000aee4*/ ADD         R5, R0, R5 
    /*0000aee8*/ CMP         R4, #0 
    /*0000aeec*/ SBCS        R3, R5, #0 
    /*0000aef0*/ BLT         getStabilizedMotion_xb64c
    /*0000aef4*/ ADDS        R2, R4, #4096               @ 0x1000 
    /*0000aef8*/ ADC         R3, R5, #0 
    /*0000aefc*/ LSR         R4, R2, #13 
    /*0000af00*/ ORR         R4, R4, R3, LSL #19 
    /*0000af04*/ ASR         R5, R3, #13 
    getStabilizedMotion_xaf08:
    /*0000af08*/ STRD        R4, [SL, #32] 
    getStabilizedMotion_xaf0c:
    /*0000af0c*/ LDR         R0, [SP, #12] 
    /*0000af10*/ MOV         R3, #344                    @ 0x158 
    /*0000af14*/ LDR         IP, [SP, #8] 
    /*0000af18*/ LDR         R2, [FP, #120]              @ 0x78 
    /*0000af1c*/ MLA         R3, R3, R0, IP 
    /*0000af20*/ LDR         R1, [R3, #112]              @ 0x70 
    /*0000af24*/ CMP         R1, R2 
    /*0000af28*/ BCC         getStabilizedMotion_xaf48
    /*0000af2c*/ LDR         R3, [R3, #132]              @ 0x84 
    /*0000af30*/ CMP         R3, #0 
    /*0000af34*/ BEQ         getStabilizedMotion_xaf48
    /*0000af38*/ LDR         R2, [SP, #40]               @ 0x28 
    /*0000af3c*/ ADD         R0, R2, #104                @ 0x68 
    /*0000af40*/ ADD         R0, IP, R0 
    /*0000af44*/ BLX         R3 
    getStabilizedMotion_xaf48:
    /*0000af48*/ ADD         SP, SP, #300                @ 0x12c 
    /*0000af4c*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    getStabilizedMotion_xaf50:
    /*0000af50*/ LDR         R2, [SP, #12] 
    /*0000af54*/ MOV         R3, #344                    @ 0x158 
    /*0000af58*/ MUL         R3, R3, R2 
    /*0000af5c*/ LDR         R2, [SP, #8] 
    /*0000af60*/ LDR         R4, [R2, R3] 
    /*0000af64*/ LDR         R3, [R4, #172]              @ 0xac 
    /*0000af68*/ CMP         R3, #0 
    /*0000af6c*/ BEQ         getStabilizedMotion_xb0c8
    getStabilizedMotion_xaf70:
    /*0000af70*/ LDR         R1, [R4, #176]              @ 0xb0 
    /*0000af74*/ STR         R1, [R4, #192]              @ 0xc0 
    /*0000af78*/ B           getStabilizedMotion_xa894
    getStabilizedMotion_xaf7c:
    /*0000af7c*/ LDR         R3, [R6, #332]              @ 0x14c 
    /*0000af80*/ CMP         R3, #0 
    /*0000af84*/ ADDEQ       R3, SP, #248                @ 0xf8 
    /*0000af88*/ STREQ       R3, [SP, #24] 
    /*0000af8c*/ BEQ         getStabilizedMotion_xafa4
    /*0000af90*/ ADD         R2, SP, #248                @ 0xf8 
    /*0000af94*/ MOV         R1, R5 
    /*0000af98*/ MOV         R0, R4 
    /*0000af9c*/ STR         R2, [SP, #24] 
    /*0000afa0*/ BLX         R3 
    getStabilizedMotion_xafa4:
    /*0000afa4*/ LDR         R2, [SP, #12] 
    /*0000afa8*/ MOV         R3, #344                    @ 0x158 
    /*0000afac*/ LDR         R1, [SP, #8] 
    /*0000afb0*/ MLA         R3, R3, R2, R1 
    /*0000afb4*/ LDR         R3, [R3, #316]              @ 0x13c 
    /*0000afb8*/ CMP         R3, #0 
    /*0000afbc*/ BEQ         getStabilizedMotion_xafc8
    /*0000afc0*/ MOV         R0, R4 
    /*0000afc4*/ BLX         R3 
    getStabilizedMotion_xafc8:
    /*0000afc8*/ LDR         R2, [SP, #12] 
    /*0000afcc*/ MOV         R3, #344                    @ 0x158 
    /*0000afd0*/ LDR         R0, [SP, #8] 
    /*0000afd4*/ LDR         R1, [SP, #248]              @ 0xf8 
    /*0000afd8*/ MLA         R3, R3, R2, R0 
    /*0000afdc*/ LDR         R0, [SP, #96]               @ 0x60 
    /*0000afe0*/ LDR         R2, [R3, #336]              @ 0x150 
    /*0000afe4*/ SUB         R2, R2, R1 
    /*0000afe8*/ B           getStabilizedMotion_xa710
    getStabilizedMotion_xafec:
    /*0000afec*/ LDRD        R8, [SP, #160]              @ 0xa0 
    /*0000aff0*/ LDR         IP, [FP, #276]              @ 0x114 
    /*0000aff4*/ CMP         R8, #0 
    /*0000aff8*/ SBCS        R3, R9, #0 
    /*0000affc*/ MOV         R2, IP 
    /*0000b000*/ ASR         R3, IP, #31 
    /*0000b004*/ STRD        R2, [SP, #16] 
    /*0000b008*/ BLT         getStabilizedMotion_xb5fc
    /*0000b00c*/ ADDS        R4, R8, #64                 @ 0x40 
    /*0000b010*/ ADC         R5, R9, #0 
    /*0000b014*/ LSR         R6, R4, #7 
    /*0000b018*/ ORR         R6, R6, R5, LSL #25 
    /*0000b01c*/ ASR         R7, R5, #7 
    getStabilizedMotion_xb020:
    /*0000b020*/ LDR         R3, [SP, #20] 
    /*0000b024*/ MUL         R3, R6, R3 
    /*0000b028*/ MLA         R3, IP, R7, R3 
    /*0000b02c*/ UMULL       R6, R7, R6, IP 
    /*0000b030*/ ADD         R7, R3, R7 
    /*0000b034*/ CMP         R6, #0 
    /*0000b038*/ SBCS        R3, R7, #0 
    /*0000b03c*/ BLT         getStabilizedMotion_xb964
    /*0000b040*/ ADDS        R0, R6, #32 
    /*0000b044*/ ADC         R1, R7, #0 
    /*0000b048*/ LSR         R2, R0, #6 
    /*0000b04c*/ ORR         R2, R2, R1, LSL #26 
    /*0000b050*/ ASR         R3, R1, #6 
    getStabilizedMotion_xb054:
    /*0000b054*/ LDRD        R4, [SP, #184]              @ 0xb8 
    /*0000b058*/ ADDS        R6, R2, R8 
    /*0000b05c*/ ADC         R7, R3, R9 
    /*0000b060*/ STRD        R6, [SP, #160]              @ 0xa0 
    /*0000b064*/ CMP         R4, #0 
    /*0000b068*/ SBCS        R3, R5, #0 
    /*0000b06c*/ BLT         getStabilizedMotion_xb93c
    /*0000b070*/ ADDS        R0, R4, #64                 @ 0x40 
    /*0000b074*/ ADC         R1, R5, #0 
    /*0000b078*/ LSR         R6, R0, #7 
    /*0000b07c*/ ORR         R6, R6, R1, LSL #25 
    /*0000b080*/ ASR         R7, R1, #7 
    getStabilizedMotion_xb084:
    /*0000b084*/ LDR         R3, [SP, #20] 
    /*0000b088*/ MUL         R1, R6, R3 
    /*0000b08c*/ MLA         R1, IP, R7, R1 
    /*0000b090*/ UMULL       R6, R7, R6, IP 
    /*0000b094*/ ADD         R7, R1, R7 
    /*0000b098*/ CMP         R6, #0 
    /*0000b09c*/ SBCS        R3, R7, #0 
    /*0000b0a0*/ BLT         getStabilizedMotion_xb914
    /*0000b0a4*/ ADDS        R2, R6, #32 
    /*0000b0a8*/ ADC         R3, R7, #0 
    /*0000b0ac*/ LSR         R6, R2, #6 
    /*0000b0b0*/ ORR         R6, R6, R3, LSL #26 
    /*0000b0b4*/ ASR         R7, R3, #6 
    getStabilizedMotion_xb0b8:
    /*0000b0b8*/ ADDS        R0, R6, R4 
    /*0000b0bc*/ ADC         R1, R7, R5 
    /*0000b0c0*/ STRD        R0, [SP, #184]              @ 0xb8 
    /*0000b0c4*/ B           getStabilizedMotion_xac50
    getStabilizedMotion_xb0c8:
    /*0000b0c8*/ ADD         R2, R4, #28 
    /*0000b0cc*/ ADD         R1, R4, #52                 @ 0x34 
    /*0000b0d0*/ LDR         R0, [SP, #32] 
    /*0000b0d4*/ BL          0x0000000000008740 
    /*0000b0d8*/ LDR         R3, [R4, #20] 
    /*0000b0dc*/ LDR         R1, [R4, #180]              @ 0xb4 
    /*0000b0e0*/ CMP         R3, #31 
    /*0000b0e4*/ LDR         R3, [R4, #176]              @ 0xb0 
    /*0000b0e8*/ LDRB        R2, [R4, #184]              @ 0xb8 
    /*0000b0ec*/ STR         R0, [R4, #192]              @ 0xc0 
    /*0000b0f0*/ LSLLS       R0, R0, #2 
    /*0000b0f4*/ CMP         R0, R1 
    /*0000b0f8*/ MOVGE       R0, R1 
    /*0000b0fc*/ CMP         R0, R3 
    /*0000b100*/ MOVLT       R0, R3 
    /*0000b104*/ STR         R0, [R4, #192]              @ 0xc0 
    /*0000b108*/ ASR         R1, R0, #31 
    /*0000b10c*/ BL          0x000000000000B10C          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*0000b110*/ MOV         R2, #4 
    /*0000b114*/ BL          0x000000000000B114          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000b118*/ CMP         R0, #8192                   @ 0x2000 
    /*0000b11c*/ MOVLT       R1, R0 
    /*0000b120*/ MOVGE       R1, #8192                   @ 0x2000 
    /*0000b124*/ BIC         R1, R1, R1, ASR #31 
    /*0000b128*/ STR         R1, [R4, #192]              @ 0xc0 
    /*0000b12c*/ B           getStabilizedMotion_xa894
    getStabilizedMotion_xb130:
    /*0000b130*/ MOVW        R3, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x25c8] 
    /*0000b134*/ MOVW        R2, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x59c] 
    /*0000b138*/ MOVT        R3, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x25c8] 
    /*0000b13c*/ MOVT        R2, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x59c] 
    /*0000b140*/ STR         R3, [SP] 
    /*0000b144*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2500] 
    /*0000b148*/ MOVW        R3, #523                    @ 0x20b 
    /*0000b14c*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2570] 
    /*0000b150*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2500] 
    /*0000b154*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2570] 
    /*0000b158*/ BL          0x000000000000B158          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000b15c*/ LDRD        R2, [SL, #40]               @ 0x28 
    /*0000b160*/ STRD        R2, [SP, #48]               @ 0x30 
    /*0000b164*/ B           getStabilizedMotion_xad10
    getStabilizedMotion_xb168:
    /*0000b168*/ LDR         R3, [FP, #836]              @ 0x344 
    /*0000b16c*/ LDR         R1, [FP, #832]              @ 0x340 
    /*0000b170*/ EOR         R2, R3, R3, ASR #31 
    /*0000b174*/ SUB         R2, R2, R3, ASR #31 
    /*0000b178*/ LDR         R3, [SP, #24] 
    /*0000b17c*/ CMP         R3, R2 
    /*0000b180*/ MOVLT       R3, R2 
    /*0000b184*/ STR         R3, [SP, #24] 
    /*0000b188*/ STR         R3, [FP, #836]              @ 0x344 
    /*0000b18c*/ B           getStabilizedMotion_xa534
    getStabilizedMotion_xb190:
    /*0000b190*/ LDR         R3, [FP, #824]              @ 0x338 
    /*0000b194*/ LDR         R1, [FP, #820]              @ 0x334 
    /*0000b198*/ EOR         R2, R3, R3, ASR #31 
    /*0000b19c*/ SUB         R2, R2, R3, ASR #31 
    /*0000b1a0*/ CMP         R5, R2 
    /*0000b1a4*/ MOVLT       R5, R2 
    /*0000b1a8*/ STR         R5, [FP, #824]              @ 0x338 
    /*0000b1ac*/ B           getStabilizedMotion_xa440
    getStabilizedMotion_xb1b0:
    /*0000b1b0*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*0000b1b4*/ ADD         R1, SP, #152                @ 0x98 
    /*0000b1b8*/ LDR         R0, [SP, #32] 
    /*0000b1bc*/ BL          0x0000000000008C40 
    /*0000b1c0*/ LDR         R3, [FP, #24] 
    /*0000b1c4*/ CMP         R3, #1 
    /*0000b1c8*/ BNE         getStabilizedMotion_xac70
    getStabilizedMotion_xb1cc:
    /*0000b1cc*/ LDR         R4, [FP, #28] 
    /*0000b1d0*/ MOV         R2, #48                     @ 0x30 
    /*0000b1d4*/ LDR         R5, [FP, #32] 
    /*0000b1d8*/ ADD         R1, SP, #152                @ 0x98 
    /*0000b1dc*/ ADD         R0, SP, #104                @ 0x68 
    /*0000b1e0*/ BL          0x000000000000B1E0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000b1e4*/ LDRD        R8, [SP, #152]              @ 0x98 
    /*0000b1e8*/ LDR         R2, [SP, #160]              @ 0xa0 
    /*0000b1ec*/ LSR         R4, R4, #1 
    /*0000b1f0*/ SUBS        R0, R8, #1048576            @ 0x100000 
    /*0000b1f4*/ LSR         R5, R5, #1 
    /*0000b1f8*/ SBC         R1, R9, #0 
    /*0000b1fc*/ UMULL       R6, R7, R2, R5 
    /*0000b200*/ MOV         R9, R1 
    /*0000b204*/ UMULL       R2, R3, R0, R4 
    /*0000b208*/ LDR         R1, [SP, #164]              @ 0xa4 
    /*0000b20c*/ ADDS        R8, R2, R6 
    /*0000b210*/ MLA         R7, R5, R1, R7 
    /*0000b214*/ LDRD        R0, [SP, #120]              @ 0x78 
    /*0000b218*/ MLA         R3, R4, R9, R3 
    /*0000b21c*/ ADC         R9, R3, R7 
    /*0000b220*/ CMP         R8, #0 
    /*0000b224*/ SBCS        R3, R9, #0 
    /*0000b228*/ BLT         getStabilizedMotion_xb9f8
    /*0000b22c*/ ADDS        R2, R8, #512                @ 0x200 
    /*0000b230*/ ADC         R3, R9, #0 
    /*0000b234*/ LSR         IP, R2, #10 
    /*0000b238*/ ORR         IP, IP, R3, LSL #22 
    /*0000b23c*/ ASR         R3, R3, #10 
    /*0000b240*/ MOV         R6, IP 
    /*0000b244*/ MOV         R7, R3 
    getStabilizedMotion_xb248:
    /*0000b248*/ LDRD        R8, [SP, #184]              @ 0xb8 
    /*0000b24c*/ SUBS        R6, R0, R6 
    /*0000b250*/ SBC         R7, R1, R7 
    /*0000b254*/ LDR         R0, [SP, #176]              @ 0xb0 
    /*0000b258*/ SUBS        R2, R8, #1048576            @ 0x100000 
    /*0000b25c*/ STRD        R6, [SP, #120]              @ 0x78 
    /*0000b260*/ SBC         R3, R9, #0 
    /*0000b264*/ LDR         IP, [SP, #180]              @ 0xb4 
    /*0000b268*/ UMULL       R0, R1, R0, R4 
    /*0000b26c*/ MOV         R7, R3 
    /*0000b270*/ UMULL       R2, R3, R2, R5 
    /*0000b274*/ LDRD        R8, [SP, #144]              @ 0x90 
    /*0000b278*/ MLA         R1, R4, IP, R1 
    /*0000b27c*/ ADDS        R4, R2, R0 
    /*0000b280*/ MLA         R3, R5, R7, R3 
    /*0000b284*/ ADC         R5, R3, R1 
    /*0000b288*/ CMP         R4, #0 
    /*0000b28c*/ SBCS        R3, R5, #0 
    /*0000b290*/ BLT         getStabilizedMotion_xb98c
    /*0000b294*/ ADDS        R2, R4, #512                @ 0x200 
    /*0000b298*/ ADC         R3, R5, #0 
    /*0000b29c*/ LSR         R4, R2, #10 
    /*0000b2a0*/ ORR         R4, R4, R3, LSL #22 
    /*0000b2a4*/ ASR         R5, R3, #10 
    getStabilizedMotion_xb2a8:
    /*0000b2a8*/ SUBS        R6, R8, R4 
    /*0000b2ac*/ MOV         R2, #48                     @ 0x30 
    /*0000b2b0*/ ADD         R1, SP, #104                @ 0x68 
    /*0000b2b4*/ ADD         R0, SP, #152                @ 0x98 
    /*0000b2b8*/ SBC         R7, R9, R5 
    /*0000b2bc*/ STRD        R6, [SP, #144]              @ 0x90 
    /*0000b2c0*/ BL          0x000000000000B2C0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000b2c4*/ B           getStabilizedMotion_xac70
    getStabilizedMotion_xb2c8:
    /*0000b2c8*/ LDR         R3, [SP, #40]               @ 0x28 
    /*0000b2cc*/ ADD         R0, R3, #56                 @ 0x38 
    /*0000b2d0*/ LDR         R3, [SP, #8] 
    /*0000b2d4*/ ADD         R0, R3, R0 
    /*0000b2d8*/ STR         R0, [SP, #44]               @ 0x2c 
    /*0000b2dc*/ BL          0x000000000000B2DC          @ DIS_resetMotionModel [R_ARM_CALL: .text+0x6b70] 
    /*0000b2e0*/ B           getStabilizedMotion_xa860
    getStabilizedMotion_xb2e4:
    /*0000b2e4*/ MOV         R3, #20 
    /*0000b2e8*/ MUL         R6, R3, R6 
    /*0000b2ec*/ MUL         R0, R3, R0 
    /*0000b2f0*/ CMP         R6, #8192                   @ 0x2000 
    /*0000b2f4*/ MOVGE       R6, #8192                   @ 0x2000 
    /*0000b2f8*/ CMP         R0, #8192                   @ 0x2000 
    /*0000b2fc*/ MOVGE       R0, #8192                   @ 0x2000 
    /*0000b300*/ BIC         R0, R0, R0, ASR #31 
    /*0000b304*/ CMP         R6, #0 
    /*0000b308*/ ORRGE       R0, R0, R6 
    /*0000b30c*/ CMP         R0, #0 
    /*0000b310*/ BEQ         getStabilizedMotion_xab50
    /*0000b314*/ MOV         R4, #0 
    /*0000b318*/ B           getStabilizedMotion_xaaec
    getStabilizedMotion_xb31c:
    /*0000b31c*/ LDR         R7, [FP, #448]              @ 0x1c0 
    /*0000b320*/ ADD         R3, FP, #464                @ 0x1d0 
    /*0000b324*/ MOV         R2, #20 
    /*0000b328*/ LDR         R6, [FP, #336]              @ 0x150 
    /*0000b32c*/ LDRD        R0, [R3, #-8] 
    /*0000b330*/ EOR         R3, R7, R7, ASR #31 
    /*0000b334*/ SUB         R3, R3, R7, ASR #31 
    /*0000b338*/ STR         R3, [SP, #48]               @ 0x30 
    /*0000b33c*/ LDR         R3, [FP, #472]              @ 0x1d8 
    /*0000b340*/ MOV         R8, R6 
    /*0000b344*/ EOR         R5, R6, R6, ASR #31 
    /*0000b348*/ SUB         R5, R5, R6, ASR #31 
    /*0000b34c*/ MOV         R4, R3 
    /*0000b350*/ STR         R3, [SP, #72]               @ 0x48 
    /*0000b354*/ LDR         R3, [FP, #360]              @ 0x168 
    /*0000b358*/ MOV         R9, R3 
    /*0000b35c*/ BL          0x000000000000B35C          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*0000b360*/ ADD         R3, FP, #432                @ 0x1b0 
    /*0000b364*/ CMP         R9, #0 
    /*0000b368*/ STR         R9, [SP, #32] 
    /*0000b36c*/ RSBLT       R9, R9, #0 
    /*0000b370*/ LDRD        R2, [R3] 
    /*0000b374*/ CMP         R4, #0 
    /*0000b378*/ RSBLT       R4, R4, #0 
    /*0000b37c*/ STR         R9, [SP, #24] 
    /*0000b380*/ ASR         R9, R6, #31 
    /*0000b384*/ STRD        R8, [SP, #56]               @ 0x38 
    /*0000b388*/ BL          0x000000000000B388          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*0000b38c*/ ADD         R3, FP, #352                @ 0x160 
    /*0000b390*/ MOV         R2, #20 
    /*0000b394*/ STR         R0, [SP, #80]               @ 0x50 
    /*0000b398*/ LDRD        R0, [R3, #-8] 
    /*0000b39c*/ BL          0x000000000000B39C          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*0000b3a0*/ ADD         R3, FP, #320                @ 0x140 
    /*0000b3a4*/ LDRD        R2, [R3] 
    /*0000b3a8*/ BL          0x000000000000B3A8          @ osal_div64_s64 [R_ARM_CALL: +0x0] 
    /*0000b3ac*/ LDR         R2, [SP, #32] 
    /*0000b3b0*/ ASR         R3, R2, #31 
    /*0000b3b4*/ STRD        R2, [SP, #64]               @ 0x40 
    /*0000b3b8*/ STR         R0, [SP, #76]               @ 0x4c 
    /*0000b3bc*/ B           getStabilizedMotion_xa410
    getStabilizedMotion_xb3c0:
    /*0000b3c0*/ MOV         R2, #0 
    /*0000b3c4*/ STR         R2, [SP, #96]               @ 0x60 
    /*0000b3c8*/ STR         R2, [SP, #100]              @ 0x64 
    /*0000b3cc*/ ADD         R3, SP, #96                 @ 0x60 
    /*0000b3d0*/ STR         R2, [FP, #272]              @ 0x110 
    /*0000b3d4*/ MOV         R9, R2 
    /*0000b3d8*/ STR         R2, [FP, #276]              @ 0x114 
    /*0000b3dc*/ MOV         R8, R2 
    /*0000b3e0*/ STR         R2, [SP, #248]              @ 0xf8 
    /*0000b3e4*/ STR         R2, [SP, #252]              @ 0xfc 
    /*0000b3e8*/ LDR         R1, [FP, #28] 
    /*0000b3ec*/ STR         R3, [SP, #48]               @ 0x30 
    /*0000b3f0*/ ADD         R3, SP, #88                 @ 0x58 
    /*0000b3f4*/ STR         R2, [SP, #260]              @ 0x104 
    /*0000b3f8*/ STR         R3, [SP, #56]               @ 0x38 
    /*0000b3fc*/ STR         R1, [SP, #256]              @ 0x100 
    /*0000b400*/ STR         R1, [SP, #264]              @ 0x108 
    /*0000b404*/ LDR         R3, [SP, #24] 
    /*0000b408*/ LDR         R1, [FP, #32] 
    /*0000b40c*/ LDR         R6, [SP, #16] 
    /*0000b410*/ MOV         R7, R3 
    /*0000b414*/ STR         R2, [SP, #72]               @ 0x48 
    /*0000b418*/ STR         R2, [SP, #88]               @ 0x58 
    /*0000b41c*/ STR         R2, [SP, #44]               @ 0x2c 
    /*0000b420*/ STR         R2, [SP, #272]              @ 0x110 
    /*0000b424*/ STR         R2, [SP, #92]               @ 0x5c 
    /*0000b428*/ STR         R1, [SP, #268]              @ 0x10c 
    /*0000b42c*/ STR         R1, [SP, #276]              @ 0x114 
    /*0000b430*/ STR         FP, [SP, #80]               @ 0x50 
    /*0000b434*/ STR         SL, [SP, #84]               @ 0x54 
    getStabilizedMotion_xb438:
    /*0000b438*/ MOV         R2, #8 
    /*0000b43c*/ LDRD        R0, [R6] 
    /*0000b440*/ BL          0x000000000000B440          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000b444*/ MOV         R2, R8 
    /*0000b448*/ ASR         R3, R8, #31 
    /*0000b44c*/ STRD        R2, [SP, #32] 
    /*0000b450*/ LDR         R3, [SP, #36]               @ 0x24 
    /*0000b454*/ MOV         R2, #8 
    /*0000b458*/ ASR         R5, R9, #31 
    /*0000b45c*/ MOV         R4, R9 
    /*0000b460*/ UMULL       SL, FP, R0, R8 
    /*0000b464*/ MUL         R0, R0, R3 
    /*0000b468*/ MLA         R3, R8, R1, R0 
    /*0000b46c*/ LDRD        R0, [R6, #8] 
    /*0000b470*/ ADD         FP, R3, FP 
    /*0000b474*/ BL          0x000000000000B474          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000b478*/ STRD        R4, [SP, #64]               @ 0x40 
    /*0000b47c*/ MOV         R2, #2 
    /*0000b480*/ MUL         R3, R0, R5 
    /*0000b484*/ MLA         R3, R9, R1, R3 
    /*0000b488*/ UMULL       R4, R5, R0, R9 
    /*0000b48c*/ LDRD        R0, [R6, #16] 
    /*0000b490*/ ADD         R3, R3, R5 
    /*0000b494*/ ADDS        R4, R4, SL 
    /*0000b498*/ ADC         R5, R3, FP 
    /*0000b49c*/ BL          0x000000000000B49C          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*0000b4a0*/ MOV         R2, #12 
    /*0000b4a4*/ ADDS        R0, R0, R4 
    /*0000b4a8*/ ADC         R1, R1, R5 
    /*0000b4ac*/ BL          0x000000000000B4AC          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000b4b0*/ MOV         R2, #8 
    /*0000b4b4*/ STR         R0, [SP, #76]               @ 0x4c 
    /*0000b4b8*/ LDRD        R0, [R6, #24] 
    /*0000b4bc*/ BL          0x000000000000B4BC          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000b4c0*/ UMULL       R2, R3, R8, R0 
    /*0000b4c4*/ MUL         R1, R8, R1 
    /*0000b4c8*/ MOV         FP, R3 
    /*0000b4cc*/ LDR         R3, [SP, #36]               @ 0x24 
    /*0000b4d0*/ MOV         SL, R2 
    /*0000b4d4*/ MOV         R2, #8 
    /*0000b4d8*/ MLA         R3, R0, R3, R1 
    /*0000b4dc*/ LDRD        R0, [R6, #32] 
    /*0000b4e0*/ ADD         R3, R3, FP 
    /*0000b4e4*/ MOV         FP, R3 
    /*0000b4e8*/ BL          0x000000000000B4E8          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000b4ec*/ LDR         R3, [SP, #68]               @ 0x44 
    /*0000b4f0*/ MOV         R2, #2 
    /*0000b4f4*/ MUL         R1, R9, R1 
    /*0000b4f8*/ MLA         R3, R0, R3, R1 
    /*0000b4fc*/ UMULL       R4, R5, R9, R0 
    /*0000b500*/ LDRD        R0, [R6, #40]               @ 0x28 
    /*0000b504*/ ADD         R5, R3, R5 
    /*0000b508*/ ADDS        SL, SL, R4 
    /*0000b50c*/ ADC         FP, FP, R5 
    /*0000b510*/ BL          0x000000000000B510          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*0000b514*/ MOV         R2, #12 
    /*0000b518*/ ADDS        R0, R0, SL 
    /*0000b51c*/ ADC         R1, R1, FP 
    /*0000b520*/ BL          0x000000000000B520          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000b524*/ LDR         R3, [SP, #76]               @ 0x4c 
    /*0000b528*/ CMP         R3, R8 
    /*0000b52c*/ SUB         R3, R3, R8 
    /*0000b530*/ MVNLE       R2, #0 
    /*0000b534*/ MOVGT       R2, #1 
    /*0000b538*/ EOR         R8, R3, R3, ASR #31 
    /*0000b53c*/ SUB         R8, R8, R3, ASR #31 
    /*0000b540*/ CMP         R0, R9 
    /*0000b544*/ SUB         IP, R0, R9 
    /*0000b548*/ ADD         R0, SP, #272                @ 0x110 
    /*0000b54c*/ MVNLE       R1, #0 
    /*0000b550*/ MOVGT       R1, #1 
    /*0000b554*/ CMP         R7, R0 
    /*0000b558*/ LDR         R0, [SP, #56]               @ 0x38 
    /*0000b55c*/ EOR         R3, IP, IP, ASR #31 
    /*0000b560*/ SUB         R3, R3, IP, ASR #31 
    /*0000b564*/ STRH        R2, [R0], #2 
    /*0000b568*/ LDR         R2, [SP, #44]               @ 0x2c 
    /*0000b56c*/ STR         R0, [SP, #56]               @ 0x38 
    /*0000b570*/ ADD         R2, R2, R8 
    /*0000b574*/ STR         R2, [SP, #44]               @ 0x2c 
    /*0000b578*/ LDR         R2, [SP, #48]               @ 0x30 
    /*0000b57c*/ STRH        R1, [R2], #2 
    /*0000b580*/ STR         R2, [SP, #48]               @ 0x30 
    /*0000b584*/ LDR         R2, [SP, #72]               @ 0x48 
    /*0000b588*/ ADD         R3, R2, R3 
    /*0000b58c*/ STR         R3, [SP, #72]               @ 0x48 
    /*0000b590*/ BEQ         getStabilizedMotion_xb6e8
    /*0000b594*/ LDR         R8, [R7, #8] 
    /*0000b598*/ ADD         R7, R7, #8 
    /*0000b59c*/ LDR         R9, [R7, #4] 
    /*0000b5a0*/ B           getStabilizedMotion_xb438
    getStabilizedMotion_xb5a4:
    /*0000b5a4*/ MOV         R5, #0 
    /*0000b5a8*/ LDR         R4, [FP, #28] 
    /*0000b5ac*/ STR         R5, [SP, #64]               @ 0x40 
    /*0000b5b0*/ MOV         R3, R5 
    /*0000b5b4*/ STR         R5, [SP, #24] 
    /*0000b5b8*/ STR         R5, [SP, #56]               @ 0x38 
    /*0000b5bc*/ STR         R5, [SP, #48]               @ 0x30 
    /*0000b5c0*/ B           getStabilizedMotion_xa934
    getStabilizedMotion_xb5c4:
    /*0000b5c4*/ STR         R3, [FP, #308]              @ 0x134 
    /*0000b5c8*/ ADD         R3, FP, #320                @ 0x140 
    /*0000b5cc*/ STR         R3, [SP, #16] 
    /*0000b5d0*/ B           getStabilizedMotion_xa6b0
    getStabilizedMotion_xb5d4:
    /*0000b5d4*/ MOV         R4, #64                     @ 0x40 
    /*0000b5d8*/ SUBS        R2, R4, R0 
    /*0000b5dc*/ MOV         R5, #0 
    /*0000b5e0*/ SBC         R3, R5, R1 
    /*0000b5e4*/ LSR         R0, R2, #7 
    /*0000b5e8*/ ORR         R0, R0, R3, LSL #25 
    /*0000b5ec*/ ASR         R1, R3, #7 
    /*0000b5f0*/ RSBS        R0, R0, #0 
    /*0000b5f4*/ RSC         R1, R1, #0 
    /*0000b5f8*/ B           getStabilizedMotion_xaba0
    getStabilizedMotion_xb5fc:
    /*0000b5fc*/ MOV         R4, #64                     @ 0x40 
    /*0000b600*/ SUBS        R2, R4, R8 
    /*0000b604*/ MOV         R5, #0 
    /*0000b608*/ SBC         R3, R5, R9 
    /*0000b60c*/ LSR         R0, R2, #7 
    /*0000b610*/ ORR         R0, R0, R3, LSL #25 
    /*0000b614*/ ASR         R1, R3, #7 
    /*0000b618*/ RSBS        R6, R0, #0 
    /*0000b61c*/ RSC         R7, R1, #0 
    /*0000b620*/ B           getStabilizedMotion_xb020
    getStabilizedMotion_xb624:
    /*0000b624*/ MOV         R2, #4096                   @ 0x1000 
    /*0000b628*/ SUBS        R2, R2, R6 
    /*0000b62c*/ MOV         R3, #0 
    /*0000b630*/ SBC         R3, R3, R7 
    /*0000b634*/ LSR         R2, R2, #13 
    /*0000b638*/ ORR         R2, R2, R3, LSL #19 
    /*0000b63c*/ ASR         R3, R3, #13 
    /*0000b640*/ RSBS        R0, R2, #0 
    /*0000b644*/ RSC         R1, R3, #0 
    /*0000b648*/ B           getStabilizedMotion_xae44
    getStabilizedMotion_xb64c:
    /*0000b64c*/ MOV         R0, #4096                   @ 0x1000 
    /*0000b650*/ SUBS        R2, R0, R4 
    /*0000b654*/ MOV         R1, #0 
    /*0000b658*/ SBC         R3, R1, R5 
    /*0000b65c*/ LSR         R0, R2, #13 
    /*0000b660*/ ORR         R0, R0, R3, LSL #19 
    /*0000b664*/ ASR         R1, R3, #13 
    /*0000b668*/ RSBS        R4, R0, #0 
    /*0000b66c*/ RSC         R5, R1, #0 
    /*0000b670*/ B           getStabilizedMotion_xaf08
    getStabilizedMotion_xb674:
    /*0000b674*/ MOV         R4, #4096                   @ 0x1000 
    /*0000b678*/ SUBS        R2, R4, R0 
    /*0000b67c*/ MOV         R5, #0 
    /*0000b680*/ SBC         R3, R5, R1 
    /*0000b684*/ LSR         R4, R2, #13 
    /*0000b688*/ ORR         R4, R4, R3, LSL #19 
    /*0000b68c*/ ASR         R5, R3, #13 
    /*0000b690*/ RSBS        R0, R4, #0 
    /*0000b694*/ RSC         R1, R5, #0 
    /*0000b698*/ B           getStabilizedMotion_xae84
    getStabilizedMotion_xb69c:
    /*0000b69c*/ MOV         R4, #4096                   @ 0x1000 
    /*0000b6a0*/ SUBS        R2, R4, R0 
    /*0000b6a4*/ MOV         R5, #0 
    /*0000b6a8*/ SBC         R3, R5, R1 
    /*0000b6ac*/ LSR         R4, R2, #13 
    /*0000b6b0*/ ORR         R4, R4, R3, LSL #19 
    /*0000b6b4*/ ASR         R5, R3, #13 
    /*0000b6b8*/ RSBS        R0, R4, #0 
    /*0000b6bc*/ RSC         R1, R5, #0 
    /*0000b6c0*/ B           getStabilizedMotion_xaec4
    getStabilizedMotion_xb6c4:
    /*0000b6c4*/ MOV         R3, #0 
    /*0000b6c8*/ B           getStabilizedMotion_xa478
    getStabilizedMotion_xb6cc:
    /*0000b6cc*/ MOV         R3, #0 
    /*0000b6d0*/ B           getStabilizedMotion_xa56c
    getStabilizedMotion_xb6d4:
    /*0000b6d4*/ LDR         R3, [FP, #312]              @ 0x138 
    /*0000b6d8*/ ADD         R2, FP, #320                @ 0x140 
    /*0000b6dc*/ STR         R2, [SP, #16] 
    /*0000b6e0*/ STR         R3, [FP, #308]              @ 0x134 
    /*0000b6e4*/ B           getStabilizedMotion_xa6b0
    getStabilizedMotion_xb6e8:
    /*0000b6e8*/ LDR         FP, [SP, #80]               @ 0x50 
    /*0000b6ec*/ LDRH        R1, [SP, #90]               @ 0x5a 
    /*0000b6f0*/ LDRH        R3, [SP, #88]               @ 0x58 
    /*0000b6f4*/ ADD         R2, FP, #288                @ 0x120 
    /*0000b6f8*/ LDR         R0, [SP, #44]               @ 0x2c 
    /*0000b6fc*/ LDRH        IP, [SP, #92]               @ 0x5c 
    /*0000b700*/ ADD         R3, R3, R1 
    /*0000b704*/ LDRH        R1, [SP, #94]               @ 0x5e 
    /*0000b708*/ LDRH        R2, [R2] 
    /*0000b70c*/ ADD         R3, R3, IP 
    /*0000b710*/ ADD         R3, R3, R1 
    /*0000b714*/ ASR         R0, R0, #2 
    /*0000b718*/ CMP         R0, R2 
    /*0000b71c*/ LDR         SL, [SP, #84]               @ 0x54 
    /*0000b720*/ SXTH        R3, R3 
    /*0000b724*/ MOVLE       R2, #0 
    /*0000b728*/ MOVGT       R2, #1 
    /*0000b72c*/ CMP         R3, #4 
    /*0000b730*/ MOVNE       R1, #0 
    /*0000b734*/ ANDEQ       R1, R2, #1 
    /*0000b738*/ CMP         R1, #0 
    /*0000b73c*/ BNE         getStabilizedMotion_xba3c
    /*0000b740*/ CMN         R3, #4 
    /*0000b744*/ MOVNE       R3, #0 
    /*0000b748*/ MOVEQ       R3, #1 
    /*0000b74c*/ ANDS        R3, R2, R3 
    /*0000b750*/ STREQ       R3, [FP, #272]              @ 0x110 
    /*0000b754*/ STREQ       R3, [FP, #280]              @ 0x118 
    /*0000b758*/ BNE         getStabilizedMotion_xb9d0
    getStabilizedMotion_xb75c:
    /*0000b75c*/ LDRH        R1, [SP, #98]               @ 0x62 
    /*0000b760*/ ADD         R2, FP, #292                @ 0x124 
    /*0000b764*/ LDRH        R3, [SP, #96]               @ 0x60 
    /*0000b768*/ LDR         R0, [SP, #72]               @ 0x48 
    /*0000b76c*/ LDRH        IP, [SP, #100]              @ 0x64 
    /*0000b770*/ ADD         R3, R3, R1 
    /*0000b774*/ LDRH        R1, [SP, #102]              @ 0x66 
    /*0000b778*/ LDRH        R2, [R2] 
    /*0000b77c*/ ADD         R3, R3, IP 
    /*0000b780*/ ADD         R3, R3, R1 
    /*0000b784*/ ASR         R0, R0, #2 
    /*0000b788*/ CMP         R0, R2 
    /*0000b78c*/ SXTH        R3, R3 
    /*0000b790*/ MOVLE       R2, #0 
    /*0000b794*/ MOVGT       R2, #1 
    /*0000b798*/ CMP         R3, #4 
    /*0000b79c*/ MOVNE       R1, #0 
    /*0000b7a0*/ ANDEQ       R1, R2, #1 
    /*0000b7a4*/ CMP         R1, #0 
    /*0000b7a8*/ BNE         getStabilizedMotion_xba68
    /*0000b7ac*/ CMN         R3, #4 
    /*0000b7b0*/ MOVNE       R3, #0 
    /*0000b7b4*/ MOVEQ       R3, #1 
    /*0000b7b8*/ ANDS        R3, R2, R3 
    /*0000b7bc*/ STREQ       R3, [FP, #276]              @ 0x114 
    /*0000b7c0*/ STREQ       R3, [FP, #284]              @ 0x11c 
    /*0000b7c4*/ BEQ         getStabilizedMotion_xa7cc
    /*0000b7c8*/ LDR         R1, [FP, #32] 
    /*0000b7cc*/ LDR         R3, [FP, #268]              @ 0x10c 
    /*0000b7d0*/ CMP         R1, #0 
    /*0000b7d4*/ MUL         R0, R3, R0 
    /*0000b7d8*/ MOVEQ       R1, #1 
    /*0000b7dc*/ BL          0x000000000000B7DC          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000b7e0*/ MOV         R3, #1 
    /*0000b7e4*/ STR         R3, [FP, #284]              @ 0x11c 
    /*0000b7e8*/ STR         R0, [FP, #276]              @ 0x114 
    /*0000b7ec*/ B           getStabilizedMotion_xa7cc
    getStabilizedMotion_xb7f0:
    /*0000b7f0*/ MOV         R0, #0 
    /*0000b7f4*/ B           getStabilizedMotion_xa48c
    getStabilizedMotion_xb7f8:
    /*0000b7f8*/ MOV         R0, #0 
    /*0000b7fc*/ B           getStabilizedMotion_xa580
    getStabilizedMotion_xb800:
    /*0000b800*/ LDR         R4, [SP, #76]               @ 0x4c 
    /*0000b804*/ ADD         R3, FP, #760                @ 0x2f8 
    /*0000b808*/ LDR         R1, [SP, #80]               @ 0x50 
    /*0000b80c*/ ADD         R0, FP, #620                @ 0x26c 
    /*0000b810*/ MOV         R2, R4 
    /*0000b814*/ BL          0x0000000000009B9C 
    /*0000b818*/ LDR         R3, [SP, #16] 
    /*0000b81c*/ CMP         R3, #0 
    /*0000b820*/ BEQ         getStabilizedMotion_xb9b4
    /*0000b824*/ LDR         R3, [FP, #848]              @ 0x350 
    /*0000b828*/ LDR         R2, [SP, #76]               @ 0x4c 
    /*0000b82c*/ LDR         R1, [FP, #844]              @ 0x34c 
    /*0000b830*/ EOR         IP, R3, R3, ASR #31 
    /*0000b834*/ CMP         R2, #0 
    /*0000b838*/ SUB         IP, IP, R3, ASR #31 
    /*0000b83c*/ RSBLT       R2, R2, #0 
    /*0000b840*/ CMP         IP, R2 
    /*0000b844*/ MOVGE       R3, IP 
    /*0000b848*/ MOVLT       R3, R2 
    /*0000b84c*/ STR         R3, [FP, #848]              @ 0x350 
    getStabilizedMotion_xb850:
    /*0000b850*/ CMP         R3, R1 
    /*0000b854*/ LDR         R2, [FP, #504]              @ 0x1f8 
    /*0000b858*/ LDR         IP, [FP, #808]              @ 0x328 
    /*0000b85c*/ MOVLT       R3, R1 
    /*0000b860*/ STR         R3, [FP, #852]              @ 0x354 
    /*0000b864*/ CMP         R3, IP 
    /*0000b868*/ CMPGT       R0, R2 
    /*0000b86c*/ LDR         R3, [FP, #492]              @ 0x1ec 
    /*0000b870*/ LDR         R2, [FP, #480]              @ 0x1e0 
    /*0000b874*/ MOVGT       R0, #1 
    /*0000b878*/ MOVLE       R0, #0 
    /*0000b87c*/ BLE         getStabilizedMotion_xa60c
    /*0000b880*/ ADD         R3, R3, #1 
    /*0000b884*/ CMP         R3, R2 
    /*0000b888*/ MOVCS       R3, R2 
    /*0000b88c*/ CMP         R8, R3 
    /*0000b890*/ MOVCC       R0, R8 
    /*0000b894*/ MOVCS       R0, R3 
    /*0000b898*/ B           getStabilizedMotion_xa610
    getStabilizedMotion_xb89c:
    /*0000b89c*/ MOV         R4, #64                     @ 0x40 
    /*0000b8a0*/ SUBS        R0, R4, R0 
    /*0000b8a4*/ MOV         R5, #0 
    /*0000b8a8*/ SBC         R1, R5, R1 
    /*0000b8ac*/ LSR         R2, R0, #7 
    /*0000b8b0*/ ORR         R2, R2, R1, LSL #25 
    /*0000b8b4*/ ASR         R3, R1, #7 
    /*0000b8b8*/ RSBS        R4, R2, #0 
    /*0000b8bc*/ RSC         R5, R3, #0 
    /*0000b8c0*/ B           getStabilizedMotion_xac08
    getStabilizedMotion_xb8c4:
    /*0000b8c4*/ MOV         R4, #32 
    /*0000b8c8*/ SUBS        R0, R4, R0 
    /*0000b8cc*/ MOV         R5, #0 
    /*0000b8d0*/ SBC         R1, R5, R1 
    /*0000b8d4*/ LSR         R4, R0, #6 
    /*0000b8d8*/ ORR         R4, R4, R1, LSL #26 
    /*0000b8dc*/ ASR         R5, R1, #6 
    /*0000b8e0*/ RSBS        R2, R4, #0 
    /*0000b8e4*/ RSC         R3, R5, #0 
    /*0000b8e8*/ B           getStabilizedMotion_xabd4
    getStabilizedMotion_xb8ec:
    /*0000b8ec*/ MOV         R0, #32 
    /*0000b8f0*/ SUBS        R2, R0, R4 
    /*0000b8f4*/ MOV         R1, #0 
    /*0000b8f8*/ SBC         R3, R1, R5 
    /*0000b8fc*/ LSR         R0, R2, #6 
    /*0000b900*/ ORR         R0, R0, R3, LSL #26 
    /*0000b904*/ ASR         R1, R3, #6 
    /*0000b908*/ RSBS        R4, R0, #0 
    /*0000b90c*/ RSC         R5, R1, #0 
    /*0000b910*/ B           getStabilizedMotion_xac38
    getStabilizedMotion_xb914:
    /*0000b914*/ MOV         R0, #32 
    /*0000b918*/ SUBS        R2, R0, R6 
    /*0000b91c*/ MOV         R1, #0 
    /*0000b920*/ SBC         R3, R1, R7 
    /*0000b924*/ LSR         R0, R2, #6 
    /*0000b928*/ ORR         R0, R0, R3, LSL #26 
    /*0000b92c*/ ASR         R1, R3, #6 
    /*0000b930*/ RSBS        R6, R0, #0 
    /*0000b934*/ RSC         R7, R1, #0 
    /*0000b938*/ B           getStabilizedMotion_xb0b8
    getStabilizedMotion_xb93c:
    /*0000b93c*/ MOV         R0, #64                     @ 0x40 
    /*0000b940*/ SUBS        R0, R0, R4 
    /*0000b944*/ MOV         R1, #0 
    /*0000b948*/ SBC         R1, R1, R5 
    /*0000b94c*/ LSR         R2, R0, #7 
    /*0000b950*/ ORR         R2, R2, R1, LSL #25 
    /*0000b954*/ ASR         R3, R1, #7 
    /*0000b958*/ RSBS        R6, R2, #0 
    /*0000b95c*/ RSC         R7, R3, #0 
    /*0000b960*/ B           getStabilizedMotion_xb084
    getStabilizedMotion_xb964:
    /*0000b964*/ MOV         R4, #32 
    /*0000b968*/ SUBS        R0, R4, R6 
    /*0000b96c*/ MOV         R5, #0 
    /*0000b970*/ SBC         R1, R5, R7 
    /*0000b974*/ LSR         R6, R0, #6 
    /*0000b978*/ ORR         R6, R6, R1, LSL #26 
    /*0000b97c*/ ASR         R7, R1, #6 
    /*0000b980*/ RSBS        R2, R6, #0 
    /*0000b984*/ RSC         R3, R7, #0 
    /*0000b988*/ B           getStabilizedMotion_xb054
    getStabilizedMotion_xb98c:
    /*0000b98c*/ MOV         R0, #512                    @ 0x200 
    /*0000b990*/ SUBS        R2, R0, R4 
    /*0000b994*/ MOV         R1, #0 
    /*0000b998*/ SBC         R3, R1, R5 
    /*0000b99c*/ LSR         R0, R2, #10 
    /*0000b9a0*/ ORR         R0, R0, R3, LSL #22 
    /*0000b9a4*/ ASR         R1, R3, #10 
    /*0000b9a8*/ RSBS        R4, R0, #0 
    /*0000b9ac*/ RSC         R5, R1, #0 
    /*0000b9b0*/ B           getStabilizedMotion_xb2a8
    getStabilizedMotion_xb9b4:
    /*0000b9b4*/ LDR         R1, [FP, #848]              @ 0x350 
    /*0000b9b8*/ CMP         R4, #0 
    /*0000b9bc*/ MOV         R3, R4 
    /*0000b9c0*/ RSBLT       R3, R4, #0 
    /*0000b9c4*/ STR         R3, [FP, #848]              @ 0x350 
    /*0000b9c8*/ STR         R1, [FP, #844]              @ 0x34c 
    /*0000b9cc*/ B           getStabilizedMotion_xb850
    getStabilizedMotion_xb9d0:
    /*0000b9d0*/ LDR         R1, [FP, #32] 
    /*0000b9d4*/ LDR         R3, [FP, #268]              @ 0x10c 
    /*0000b9d8*/ CMP         R1, #0 
    /*0000b9dc*/ MUL         R0, R3, R0 
    /*0000b9e0*/ MOVEQ       R1, #1 
    /*0000b9e4*/ BL          0x000000000000B9E4          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000b9e8*/ MOV         R3, #1 
    /*0000b9ec*/ STR         R3, [FP, #280]              @ 0x118 
    /*0000b9f0*/ STR         R0, [FP, #272]              @ 0x110 
    /*0000b9f4*/ B           getStabilizedMotion_xb75c
    getStabilizedMotion_xb9f8:
    /*0000b9f8*/ MOV         R6, #512                    @ 0x200 
    /*0000b9fc*/ SUBS        R2, R6, R8 
    /*0000ba00*/ MOV         R7, #0 
    /*0000ba04*/ SBC         R3, R7, R9 
    /*0000ba08*/ LSR         R6, R2, #10 
    /*0000ba0c*/ ORR         R6, R6, R3, LSL #22 
    /*0000ba10*/ ASR         R7, R3, #10 
    /*0000ba14*/ RSBS        R6, R6, #0 
    /*0000ba18*/ RSC         R7, R7, #0 
    /*0000ba1c*/ B           getStabilizedMotion_xb248
    getStabilizedMotion_xba20:
    /*0000ba20*/ LDR         R3, [FP, #488]              @ 0x1e8 
    /*0000ba24*/ CMP         R1, R3 
    /*0000ba28*/ BNE         getStabilizedMotion_xa670
    getStabilizedMotion_xba2c:
    /*0000ba2c*/ ADD         R3, FP, #432                @ 0x1b0 
    /*0000ba30*/ STR         R2, [FP, #308]              @ 0x134 
    /*0000ba34*/ STR         R3, [SP, #16] 
    /*0000ba38*/ B           getStabilizedMotion_xa6b0
    getStabilizedMotion_xba3c:
    /*0000ba3c*/ LDR         R3, [FP, #268]              @ 0x10c 
    /*0000ba40*/ LDR         R1, [FP, #32] 
    /*0000ba44*/ MUL         R0, R3, R0 
    /*0000ba48*/ CMP         R1, #0 
    /*0000ba4c*/ MOVEQ       R1, #1 
    /*0000ba50*/ RSB         R0, R0, #0 
    /*0000ba54*/ BL          0x000000000000BA54          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000ba58*/ MOV         R3, #1 
    /*0000ba5c*/ STR         R3, [FP, #280]              @ 0x118 
    /*0000ba60*/ STR         R0, [FP, #272]              @ 0x110 
    /*0000ba64*/ B           getStabilizedMotion_xb75c
    getStabilizedMotion_xba68:
    /*0000ba68*/ LDR         R3, [FP, #268]              @ 0x10c 
    /*0000ba6c*/ LDR         R1, [FP, #32] 
    /*0000ba70*/ MUL         R0, R3, R0 
    /*0000ba74*/ CMP         R1, #0 
    /*0000ba78*/ MOVEQ       R1, #1 
    /*0000ba7c*/ RSB         R0, R0, #0 
    /*0000ba80*/ BL          0x000000000000BA80          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000ba84*/ MOV         R3, #1 
    /*0000ba88*/ STR         R3, [FP, #284]              @ 0x11c 
    /*0000ba8c*/ STR         R0, [FP, #276]              @ 0x114 
    /*0000ba90*/ B           getStabilizedMotion_xa7cc
FUNC_END getStabilizedMotion


FUNC_BEGIN setGyroMotion
    /*0000ba94*/ MOV         R3, #344                    @ 0x158 
    /*0000ba98*/ MOVW        IP, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x16f8] 
    /*0000ba9c*/ MUL         R0, R3, R0 
    /*0000baa0*/ MOVT        IP, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x16f8] 
    /*0000baa4*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000baa8*/ MOV         R5, R2 
    /*0000baac*/ SUB         SP, SP, #20 
    /*0000bab0*/ MOV         R6, R1 
    /*0000bab4*/ LDR         R4, [IP, R0] 
    /*0000bab8*/ LDR         R3, [R4, #24] 
    /*0000babc*/ CMP         R3, #1 
    /*0000bac0*/ BEQ         setGyroMotion_xbb04
    setGyroMotion_xbac4:
    /*0000bac4*/ CMP         R3, #0 
    /*0000bac8*/ BEQ         setGyroMotion_xbaf0
    setGyroMotion_xbacc:
    /*0000bacc*/ CMP         R5, #0 
    /*0000bad0*/ STR         R5, [R4, #312]              @ 0x138 
    /*0000bad4*/ BEQ         setGyroMotion_xbae0
    /*0000bad8*/ ADD         SP, SP, #20 
    /*0000badc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    setGyroMotion_xbae0:
    /*0000bae0*/ ADD         R0, R4, #320                @ 0x140 
    /*0000bae4*/ ADD         SP, SP, #20 
    /*0000bae8*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    setGyroMotion_xbaec:
    /*0000baec*/ B           setGyroMotion_xbaec         @ DIS_resetMotionModel [R_ARM_JUMP24: .text+0x6b70] 
    setGyroMotion_xbaf0:
    /*0000baf0*/ MOV         R1, R6 
    /*0000baf4*/ MOV         R2, #48                     @ 0x30 
    /*0000baf8*/ ADD         R0, R4, #320                @ 0x140 
    /*0000bafc*/ BL          0x000000000000BAFC          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000bb00*/ B           setGyroMotion_xbacc
    setGyroMotion_xbb04:
    /*0000bb04*/ ADD         R7, R4, #320                @ 0x140 
    /*0000bb08*/ LDR         R9, [R4, #28] 
    /*0000bb0c*/ LDR         R8, [R4, #32] 
    /*0000bb10*/ MOV         R2, #48                     @ 0x30 
    /*0000bb14*/ MOV         R0, R7 
    /*0000bb18*/ BL          0x000000000000BB18          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000bb1c*/ LDRD        SL, [R6] 
    /*0000bb20*/ LDR         R2, [R6, #8] 
    /*0000bb24*/ LSR         R9, R9, #1 
    /*0000bb28*/ SUBS        SL, SL, #1048576            @ 0x100000 
    /*0000bb2c*/ LSR         R8, R8, #1 
    /*0000bb30*/ SBC         FP, FP, #0 
    /*0000bb34*/ LDR         IP, [R6, #12] 
    /*0000bb38*/ UMULL       R0, R1, R2, R8 
    /*0000bb3c*/ UMULL       R2, R3, SL, R9 
    /*0000bb40*/ MLA         R1, R8, IP, R1 
    /*0000bb44*/ STRD        R2, [SP] 
    /*0000bb48*/ LDRD        R2, [R7, #16] 
    /*0000bb4c*/ STRD        R2, [SP, #8] 
    /*0000bb50*/ LDR         R3, [SP, #4] 
    /*0000bb54*/ MLA         R3, R9, FP, R3 
    /*0000bb58*/ STR         R3, [SP, #4] 
    /*0000bb5c*/ LDRD        R2, [SP] 
    /*0000bb60*/ ADDS        R2, R2, R0 
    /*0000bb64*/ ADC         R3, R3, R1 
    /*0000bb68*/ CMP         R2, #0 
    /*0000bb6c*/ SBCS        R1, R3, #0 
    /*0000bb70*/ BLT         setGyroMotion_xbc2c
    /*0000bb74*/ ADDS        R2, R2, #512                @ 0x200 
    /*0000bb78*/ ADC         R3, R3, #0 
    /*0000bb7c*/ LSR         R0, R2, #10 
    /*0000bb80*/ ORR         R0, R0, R3, LSL #22 
    /*0000bb84*/ ASR         R1, R3, #10 
    setGyroMotion_xbb88:
    /*0000bb88*/ LDRD        R2, [SP, #8] 
    /*0000bb8c*/ ADDS        R2, R2, R0 
    /*0000bb90*/ ADC         R3, R3, R1 
    /*0000bb94*/ MOV         R0, R2 
    /*0000bb98*/ MOV         R1, R3 
    /*0000bb9c*/ STRD        R0, [R7, #16] 
    /*0000bba0*/ LDRD        SL, [R6, #32] 
    /*0000bba4*/ LDR         R0, [R6, #24] 
    /*0000bba8*/ SUBS        SL, SL, #1048576            @ 0x100000 
    /*0000bbac*/ LDR         IP, [R6, #28] 
    /*0000bbb0*/ SBC         FP, FP, #0 
    /*0000bbb4*/ UMULL       R2, R3, SL, R8 
    /*0000bbb8*/ UMULL       R0, R1, R0, R9 
    /*0000bbbc*/ MLA         R3, R8, FP, R3 
    /*0000bbc0*/ ADDS        R2, R2, R0 
    /*0000bbc4*/ LDRD        SL, [R7, #40]               @ 0x28 
    /*0000bbc8*/ MLA         R1, R9, IP, R1 
    /*0000bbcc*/ ADC         R3, R3, R1 
    /*0000bbd0*/ CMP         R2, #0 
    /*0000bbd4*/ SBCS        R1, R3, #0 
    /*0000bbd8*/ BLT         setGyroMotion_xbc04
    /*0000bbdc*/ ADDS        R2, R2, #512                @ 0x200 
    /*0000bbe0*/ ADC         R3, R3, #0 
    /*0000bbe4*/ LSR         R0, R2, #10 
    /*0000bbe8*/ ORR         R0, R0, R3, LSL #22 
    /*0000bbec*/ ASR         R1, R3, #10 
    setGyroMotion_xbbf0:
    /*0000bbf0*/ ADDS        R0, R0, SL 
    /*0000bbf4*/ ADC         R1, R1, FP 
    /*0000bbf8*/ STRD        R0, [R7, #40]               @ 0x28 
    /*0000bbfc*/ LDR         R3, [R4, #24] 
    /*0000bc00*/ B           setGyroMotion_xbac4
    setGyroMotion_xbc04:
    /*0000bc04*/ MOV         R0, #512                    @ 0x200 
    /*0000bc08*/ SUBS        R2, R0, R2 
    /*0000bc0c*/ MOV         R1, #0 
    /*0000bc10*/ SBC         R3, R1, R3 
    /*0000bc14*/ LSR         R0, R2, #10 
    /*0000bc18*/ ORR         R0, R0, R3, LSL #22 
    /*0000bc1c*/ ASR         R1, R3, #10 
    /*0000bc20*/ RSBS        R0, R0, #0 
    /*0000bc24*/ RSC         R1, R1, #0 
    /*0000bc28*/ B           setGyroMotion_xbbf0
    setGyroMotion_xbc2c:
    /*0000bc2c*/ MOV         R0, #512                    @ 0x200 
    /*0000bc30*/ SUBS        R2, R0, R2 
    /*0000bc34*/ MOV         R1, #0 
    /*0000bc38*/ SBC         R3, R1, R3 
    /*0000bc3c*/ LSR         R0, R2, #10 
    /*0000bc40*/ ORR         R0, R0, R3, LSL #22 
    /*0000bc44*/ ASR         R1, R3, #10 
    /*0000bc48*/ RSBS        R0, R0, #0 
    /*0000bc4c*/ RSC         R1, R1, #0 
    /*0000bc50*/ B           setGyroMotion_xbb88
FUNC_END setGyroMotion


FUNC_BEGIN DIS_MOTION_FILTER_init
    /*0000bc54*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000bc58*/ SUB         SP, SP, #36                 @ 0x24 
    /*0000bc5c*/ MOV         R3, #344                    @ 0x158 
    /*0000bc60*/ MOV         R5, R2 
    /*0000bc64*/ STR         R0, [SP, #16] 
    /*0000bc68*/ MOV         R2, R3 
    /*0000bc6c*/ MUL         R0, R3, R0 
    /*0000bc70*/ LDR         R3, [R5, #12] 
    /*0000bc74*/ MOVW        SL, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x16f8] 
    /*0000bc78*/ MOVT        SL, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x16f8] 
    /*0000bc7c*/ MOV         R4, R1 
    /*0000bc80*/ MOV         R1, #0 
    /*0000bc84*/ MOV         R7, R3 
    /*0000bc88*/ STR         R3, [SP, #8] 
    /*0000bc8c*/ LDR         R3, [R5, #16] 
    /*0000bc90*/ MOV         R6, R0 
    /*0000bc94*/ STR         R0, [SP] 
    /*0000bc98*/ LDR         R0, [PC, #2492]             @ 0x000000000000c65c 
    /*0000bc9c*/ MOV         R8, R3 
    /*0000bca0*/ STR         R3, [SP, #12] 
    /*0000bca4*/ LDR         LR, [R0] 
    /*0000bca8*/ LDRH        IP, [R0, #4] 
    /*0000bcac*/ ADD         R0, SL, R6 
    /*0000bcb0*/ STR         LR, [SP, #26] 
    /*0000bcb4*/ STRH        IP, [SP, #30] 
    /*0000bcb8*/ BL          0x000000000000BCB8          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000bcbc*/ STR         R4, [SL, R6] 
    /*0000bcc0*/ MOV         R3, #8192                   @ 0x2000 
    /*0000bcc4*/ LDR         R2, [R5] 
    /*0000bcc8*/ LDR         R0, [SP, #12] 
    /*0000bccc*/ STR         R2, [R4] 
    /*0000bcd0*/ LDR         R2, [R5, #4] 
    /*0000bcd4*/ STR         R2, [R4, #4] 
    /*0000bcd8*/ LDRB        R2, [R5, #8] 
    /*0000bcdc*/ STRB        R2, [R4, #8] 
    /*0000bce0*/ LDR         R2, [R5, #12] 
    /*0000bce4*/ STR         R2, [R4, #28] 
    /*0000bce8*/ LDR         R9, [R5, #16] 
    /*0000bcec*/ STR         R2, [SP, #4] 
    /*0000bcf0*/ STR         R9, [R4, #32] 
    /*0000bcf4*/ LDR         R2, [R5, #32] 
    /*0000bcf8*/ STR         R2, [R4, #116]              @ 0x74 
    /*0000bcfc*/ LDR         R2, [R5, #28] 
    /*0000bd00*/ STR         R2, [R4, #12] 
    /*0000bd04*/ LDRB        R2, [R5, #44]               @ 0x2c 
    /*0000bd08*/ STRB        R2, [R4, #300]              @ 0x12c 
    /*0000bd0c*/ LDR         R2, [R5, #20] 
    /*0000bd10*/ STR         R2, [R4, #20] 
    /*0000bd14*/ LDR         LR, [R5, #24] 
    /*0000bd18*/ STR         R3, [R4, #16] 
    /*0000bd1c*/ LDR         R3, [SP, #8] 
    /*0000bd20*/ STR         LR, [R4, #24] 
    /*0000bd24*/ LDR         R1, [R5, #36]               @ 0x24 
    /*0000bd28*/ RSB         R1, R1, #4096               @ 0x1000 
    /*0000bd2c*/ MUL         R1, R7, R1 
    /*0000bd30*/ ADD         R1, R1, #2048               @ 0x800 
    /*0000bd34*/ UBFX        R1, R1, #12, #19 
    /*0000bd38*/ STR         R1, [R4, #36]               @ 0x24 
    /*0000bd3c*/ LDR         R2, [R5, #36]               @ 0x24 
    /*0000bd40*/ TST         R1, #1 
    /*0000bd44*/ SUBNE       R1, R1, #1 
    /*0000bd48*/ STRNE       R1, [R4, #36]               @ 0x24 
    /*0000bd4c*/ RSB         R2, R2, #4096               @ 0x1000 
    /*0000bd50*/ SUB         R3, R3, R1 
    /*0000bd54*/ MUL         R2, R8, R2 
    /*0000bd58*/ LSR         R3, R3, #1 
    /*0000bd5c*/ ADD         R2, R2, #2048               @ 0x800 
    /*0000bd60*/ UBFX        R2, R2, #12, #19 
    /*0000bd64*/ STR         R2, [R4, #40]               @ 0x28 
    /*0000bd68*/ TST         R2, #1 
    /*0000bd6c*/ SUBNE       R2, R2, #1 
    /*0000bd70*/ STRNE       R2, [R4, #40]               @ 0x28 
    /*0000bd74*/ SUB         IP, R0, R2 
    /*0000bd78*/ MOV         R0, #8192                   @ 0x2000 
    /*0000bd7c*/ STR         R0, [R4, #188]              @ 0xbc 
    /*0000bd80*/ LSR         IP, IP, #1 
    /*0000bd84*/ STR         R3, [R4, #44]               @ 0x2c 
    /*0000bd88*/ STR         IP, [R4, #48]               @ 0x30 
    /*0000bd8c*/ LDR         R6, [R5, #40]               @ 0x28 
    /*0000bd90*/ CMP         R6, #1 
    /*0000bd94*/ BEQ         DIS_MOTION_FILTER_init_xc3b4
    DIS_MOTION_FILTER_init_xbd98:
    /*0000bd98*/ CMP         LR, #1 
    /*0000bd9c*/ BEQ         DIS_MOTION_FILTER_init_xc07c
    /*0000bda0*/ MOV         R0, #0 
    /*0000bda4*/ MOV         FP, R0 
    /*0000bda8*/ MOV         R6, R0 
    /*0000bdac*/ MOV         LR, R0 
    DIS_MOTION_FILTER_init_xbdb0:
    /*0000bdb0*/ ADD         R1, R3, R1 
    /*0000bdb4*/ ADD         R2, IP, R2 
    /*0000bdb8*/ LSL         IP, IP, #4 
    /*0000bdbc*/ STR         IP, [R4, #88]               @ 0x58 
    /*0000bdc0*/ STR         IP, [R4, #96]               @ 0x60 
    /*0000bdc4*/ MOV         R7, #0 
    /*0000bdc8*/ LDR         IP, [SP, #4] 
    /*0000bdcc*/ ADD         R8, R4, #32 
    /*0000bdd0*/ LSL         R1, R1, #4 
    /*0000bdd4*/ LSL         R2, R2, #4 
    /*0000bdd8*/ STR         R1, [R4, #92]               @ 0x5c 
    /*0000bddc*/ LSL         R3, R3, #4 
    /*0000bde0*/ STR         R1, [R4, #100]              @ 0x64 
    /*0000bde4*/ MOV         R1, R7 
    /*0000bde8*/ STR         R2, [R4, #104]              @ 0x68 
    /*0000bdec*/ STR         R2, [R4, #112]              @ 0x70 
    /*0000bdf0*/ MOV         R2, R4 
    /*0000bdf4*/ STR         R3, [R4, #84]               @ 0x54 
    /*0000bdf8*/ STR         R3, [R4, #108]              @ 0x6c 
    /*0000bdfc*/ STR         IP, [R4, #60]               @ 0x3c 
    /*0000be00*/ STR         IP, [R4, #68]               @ 0x44 
    /*0000be04*/ STR         R9, [R4, #72]               @ 0x48 
    /*0000be08*/ STR         R9, [R4, #80]               @ 0x50 
    /*0000be0c*/ STR         R7, [R4, #52]               @ 0x34 
    /*0000be10*/ STR         R7, [R4, #56]               @ 0x38 
    /*0000be14*/ STR         R7, [R4, #64]               @ 0x40 
    /*0000be18*/ STR         R7, [R4, #76]               @ 0x4c 
    DIS_MOTION_FILTER_init_xbe1c:
    /*0000be1c*/ LDR         IP, [R2, #56]               @ 0x38 
    /*0000be20*/ ADD         R7, FP, R3 
    /*0000be24*/ LDR         R3, [R2, #88]               @ 0x58 
    /*0000be28*/ ADD         R1, LR, R1 
    /*0000be2c*/ STR         R7, [R2, #84]               @ 0x54 
    /*0000be30*/ ADD         R2, R2, #8 
    /*0000be34*/ STR         R1, [R2, #44]               @ 0x2c 
    /*0000be38*/ ADD         R3, R3, R0 
    /*0000be3c*/ ADD         R1, IP, R6 
    /*0000be40*/ STR         R3, [R2, #80]               @ 0x50 
    /*0000be44*/ STR         R1, [R2, #48]               @ 0x30 
    /*0000be48*/ CMP         R8, R2 
    /*0000be4c*/ LDRNE       R1, [R2, #52]               @ 0x34 
    /*0000be50*/ LDRNE       R3, [R2, #84]               @ 0x54 
    /*0000be54*/ BNE         DIS_MOTION_FILTER_init_xbe1c
    /*0000be58*/ LDR         R3, [R5, #20] 
    /*0000be5c*/ CMP         R3, #32 
    /*0000be60*/ MOVLS       R1, #32 
    /*0000be64*/ STRLS       R1, [R4, #120]              @ 0x78 
    /*0000be68*/ BLS         DIS_MOTION_FILTER_init_xbe7c
    /*0000be6c*/ CMP         R3, #64                     @ 0x40 
    /*0000be70*/ MOVLS       R1, #64                     @ 0x40 
    /*0000be74*/ STRLS       R1, [R4, #120]              @ 0x78 
    /*0000be78*/ BHI         DIS_MOTION_FILTER_init_xbedc
    DIS_MOTION_FILTER_init_xbe7c:
    /*0000be7c*/ LDR         R3, [R5] 
    /*0000be80*/ CMP         R3, #1 
    /*0000be84*/ BEQ         DIS_MOTION_FILTER_init_xbed0
    /*0000be88*/ BCC         DIS_MOTION_FILTER_init_xbed0
    /*0000be8c*/ CMP         R3, #2 
    /*0000be90*/ LSLEQ       R1, R1, #2 
    /*0000be94*/ STREQ       R1, [R4, #120]              @ 0x78 
    DIS_MOTION_FILTER_init_xbe98:
    /*0000be98*/ LDR         R3, [SP] 
    /*0000be9c*/ ADD         R2, SP, #26 
    /*0000bea0*/ STR         R1, [R4, #164]              @ 0xa4 
    /*0000bea4*/ ADD         R3, R3, #104                @ 0x68 
    /*0000bea8*/ ADD         FP, SL, R3 
    /*0000beac*/ LSR         R3, R1, #2 
    /*0000beb0*/ STR         R3, [R4, #124]              @ 0x7c 
    /*0000beb4*/ MOV         R0, FP 
    /*0000beb8*/ BL          0x000000000000BEB8          @ DIS_MotionModel_VEC_init [R_ARM_CALL: .text+0x6c40] 
    /*0000bebc*/ SUBS        R7, R0, #0 
    /*0000bec0*/ BEQ         DIS_MOTION_FILTER_init_xbef0
    DIS_MOTION_FILTER_init_xbec4:
    /*0000bec4*/ MOV         R0, R7 
    /*0000bec8*/ ADD         SP, SP, #36                 @ 0x24 
    /*0000becc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_MOTION_FILTER_init_xbed0:
    /*0000bed0*/ LSL         R1, R1, #1 
    /*0000bed4*/ STR         R1, [R4, #120]              @ 0x78 
    /*0000bed8*/ B           DIS_MOTION_FILTER_init_xbe98
    DIS_MOTION_FILTER_init_xbedc:
    /*0000bedc*/ CMP         R3, #128                    @ 0x80 
    /*0000bee0*/ MOVLS       R1, #128                    @ 0x80 
    /*0000bee4*/ MOVHI       R1, #256                    @ 0x100 
    /*0000bee8*/ STR         R1, [R4, #120]              @ 0x78 
    /*0000beec*/ B           DIS_MOTION_FILTER_init_xbe7c
    DIS_MOTION_FILTER_init_xbef0:
    /*0000bef0*/ LDR         R6, [SP] 
    /*0000bef4*/ ADD         R0, R6, #56                 @ 0x38 
    /*0000bef8*/ ADD         R0, SL, R0 
    /*0000befc*/ BL          0x000000000000BEFC          @ DIS_resetMotionModel [R_ARM_CALL: .text+0x6b70] 
    /*0000bf00*/ ADD         R0, R6, #8 
    /*0000bf04*/ ADD         R0, SL, R0 
    /*0000bf08*/ BL          0x000000000000BF08          @ DIS_resetMotionModel [R_ARM_CALL: .text+0x6b70] 
    /*0000bf0c*/ LDR         R3, [R5, #48]               @ 0x30 
    /*0000bf10*/ ADD         R0, R4, #208                @ 0xd0 
    /*0000bf14*/ CMP         R3, #1 
    /*0000bf18*/ LDRNE       R3, [R5, #52]               @ 0x34 
    /*0000bf1c*/ MOVEQ       R3, #1024                   @ 0x400 
    /*0000bf20*/ STREQ       R3, [R4, #200]              @ 0xc8 
    /*0000bf24*/ MOVEQ       R3, #8192                   @ 0x2000 
    /*0000bf28*/ STREQ       R3, [R4, #260]              @ 0x104 
    /*0000bf2c*/ STRNE       R3, [R4, #200]              @ 0xc8 
    /*0000bf30*/ LDRNE       R3, [R5, #60]               @ 0x3c 
    /*0000bf34*/ STRNE       R3, [R4, #260]              @ 0x104 
    /*0000bf38*/ LDRNE       R3, [R5, #56]               @ 0x38 
    /*0000bf3c*/ STR         R3, [R4, #256]              @ 0x100 
    /*0000bf40*/ BL          0x000000000000BF40          @ DIS_resetMotionModel [R_ARM_CALL: .text+0x6b70] 
    /*0000bf44*/ LDR         R1, [R5] 
    /*0000bf48*/ LDR         R3, [R5, #20] 
    /*0000bf4c*/ MOV         R2, #15 
    /*0000bf50*/ CMP         R1, #1 
    /*0000bf54*/ STR         R2, [R4, #812]              @ 0x32c 
    /*0000bf58*/ MOV         R2, #0 
    /*0000bf5c*/ STR         R2, [R4, #816]              @ 0x330 
    /*0000bf60*/ STR         R3, [R4, #496]              @ 0x1f0 
    /*0000bf64*/ STR         R2, [R4, #484]              @ 0x1e4 
    /*0000bf68*/ STR         R2, [R4, #488]              @ 0x1e8 
    /*0000bf6c*/ STR         R2, [R4, #492]              @ 0x1ec 
    /*0000bf70*/ BEQ         DIS_MOTION_FILTER_init_xc3f4
    /*0000bf74*/ BCC         DIS_MOTION_FILTER_init_xbf80
    /*0000bf78*/ CMP         R1, #2 
    /*0000bf7c*/ BNE         DIS_MOTION_FILTER_init_xbfac
    DIS_MOTION_FILTER_init_xbf80:
    /*0000bf80*/ MOV         R2, #524288                 @ 0x80000 
    /*0000bf84*/ STR         R2, [R4, #500]              @ 0x1f4 
    /*0000bf88*/ MOV         R2, #786432                 @ 0xc0000 
    /*0000bf8c*/ STR         R2, [R4, #504]              @ 0x1f8 
    /*0000bf90*/ MOV         R2, #10 
    /*0000bf94*/ STR         R2, [R4, #480]              @ 0x1e0 
    /*0000bf98*/ MOV         R2, #256                    @ 0x100 
    /*0000bf9c*/ STR         R2, [R4, #800]              @ 0x320 
    /*0000bfa0*/ STR         R2, [R4, #804]              @ 0x324 
    /*0000bfa4*/ MOV         R2, #524                    @ 0x20c 
    /*0000bfa8*/ STR         R2, [R4, #808]              @ 0x328 
    DIS_MOTION_FILTER_init_xbfac:
    /*0000bfac*/ LDR         R2, [R5, #136]              @ 0x88 
    /*0000bfb0*/ CMP         R2, #0 
    /*0000bfb4*/ LDRNE       IP, [R4, #480]              @ 0x1e0 
    /*0000bfb8*/ BNE         DIS_MOTION_FILTER_init_xc040
    /*0000bfbc*/ LDR         IP, [R5, #140]              @ 0x8c 
    /*0000bfc0*/ MOVW        R0, #19923                  @ 0x4dd3 
    /*0000bfc4*/ MOVT        R0, #4194                   @ 0x1062 
    /*0000bfc8*/ MOVW        R1, #34079                  @ 0x851f 
    /*0000bfcc*/ MOVT        R1, #20971                  @ 0x51eb 
    /*0000bfd0*/ MOVW        R2, #5977                   @ 0x1759 
    /*0000bfd4*/ LSL         IP, IP, #20 
    /*0000bfd8*/ MOVT        R2, #53687                  @ 0xd1b7 
    /*0000bfdc*/ UMULL       R6, R7, IP, R0 
    /*0000bfe0*/ LSR         IP, R7, #6 
    /*0000bfe4*/ STR         IP, [R4, #500]              @ 0x1f4 
    /*0000bfe8*/ LDR         R8, [R5, #144]              @ 0x90 
    /*0000bfec*/ LSL         R8, R8, #20 
    /*0000bff0*/ UMULL       R8, R9, R8, R0 
    /*0000bff4*/ LSR         R0, R9, #6 
    /*0000bff8*/ STR         R0, [R4, #504]              @ 0x1f8 
    /*0000bffc*/ LDR         IP, [R5, #160]              @ 0xa0 
    /*0000c000*/ STR         IP, [R4, #480]              @ 0x1e0 
    /*0000c004*/ LDR         R8, [R5, #156]              @ 0x9c 
    /*0000c008*/ LSL         R8, R8, #7 
    /*0000c00c*/ UMULL       R8, R9, R8, R1 
    /*0000c010*/ LSR         R0, R9, #5 
    /*0000c014*/ STR         R0, [R4, #800]              @ 0x320 
    /*0000c018*/ LDR         R6, [R5, #148]              @ 0x94 
    /*0000c01c*/ LSL         R6, R6, #7 
    /*0000c020*/ UMULL       R6, R7, R6, R1 
    /*0000c024*/ LSR         R1, R7, #5 
    /*0000c028*/ STR         R1, [R4, #804]              @ 0x324 
    /*0000c02c*/ LDR         R0, [R5, #152]              @ 0x98 
    /*0000c030*/ LSL         R0, R0, #20 
    /*0000c034*/ UMULL       R0, R1, R0, R2 
    /*0000c038*/ LSR         R2, R1, #13 
    /*0000c03c*/ STR         R2, [R4, #808]              @ 0x328 
    DIS_MOTION_FILTER_init_xc040:
    /*0000c040*/ CMP         IP, #1 
    /*0000c044*/ ADD         R6, R4, #508                @ 0x1fc 
    /*0000c048*/ MOV         R2, #5 
    /*0000c04c*/ MOV         R1, #7 
    /*0000c050*/ MOVCC       IP, #1 
    /*0000c054*/ MOV         R0, R6 
    /*0000c058*/ STR         IP, [R4, #480]              @ 0x1e0 
    /*0000c05c*/ BL          0x000000000000C05C          @ HIISP_CQ_VECTOR_INIT [R_ARM_CALL: .text+0x6de4] 
    /*0000c060*/ SUBS        R7, R0, #0 
    /*0000c064*/ BEQ         DIS_MOTION_FILTER_init_xc0a4
    DIS_MOTION_FILTER_init_xc068:
    /*0000c068*/ MOV         R0, FP 
    /*0000c06c*/ BL          0x000000000000C06C          @ DIS_MotionModel_VEC_uninit [R_ARM_CALL: .text+0x6d94] 
    /*0000c070*/ MOV         R0, R7 
    /*0000c074*/ ADD         SP, SP, #36                 @ 0x24 
    /*0000c078*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_MOTION_FILTER_init_xc07c:
    /*0000c07c*/ LDR         R0, [SP, #4] 
    /*0000c080*/ ADD         R6, R9, R9, LSR #31 
    /*0000c084*/ ASR         R6, R6, #1 
    /*0000c088*/ ADD         LR, R0, R0, LSR #31 
    /*0000c08c*/ RSB         R6, R6, #0 
    /*0000c090*/ ASR         LR, LR, #1 
    /*0000c094*/ LSL         R0, R6, #4 
    /*0000c098*/ RSB         LR, LR, #0 
    /*0000c09c*/ LSL         FP, LR, #4 
    /*0000c0a0*/ B           DIS_MOTION_FILTER_init_xbdb0
    DIS_MOTION_FILTER_init_xc0a4:
    /*0000c0a4*/ ADD         R3, R4, #564                @ 0x234 
    /*0000c0a8*/ MOV         R2, #5 
    /*0000c0ac*/ STR         R3, [SP, #4] 
    /*0000c0b0*/ MOV         R1, #7 
    /*0000c0b4*/ MOV         R0, R3 
    /*0000c0b8*/ LDR         R3, [R4, #496]              @ 0x1f0 
    /*0000c0bc*/ BL          0x000000000000C0BC          @ HIISP_CQ_VECTOR_INIT [R_ARM_CALL: .text+0x6de4] 
    /*0000c0c0*/ SUBS        R7, R0, #0 
    /*0000c0c4*/ BNE         DIS_MOTION_FILTER_init_xc450
    /*0000c0c8*/ ADD         R3, R4, #620                @ 0x26c 
    /*0000c0cc*/ MOV         R2, #5 
    /*0000c0d0*/ STR         R3, [SP, #20] 
    /*0000c0d4*/ MOV         R1, #7 
    /*0000c0d8*/ MOV         R0, R3 
    /*0000c0dc*/ LDR         R3, [R4, #496]              @ 0x1f0 
    /*0000c0e0*/ BL          0x000000000000C0E0          @ HIISP_CQ_VECTOR_INIT [R_ARM_CALL: .text+0x6de4] 
    /*0000c0e4*/ SUBS        R7, R0, #0 
    /*0000c0e8*/ BNE         DIS_MOTION_FILTER_init_xc45c
    /*0000c0ec*/ MOV         R2, #40                     @ 0x28 
    /*0000c0f0*/ MOV         R1, R7 
    /*0000c0f4*/ ADD         R0, R4, #680                @ 0x2a8 
    /*0000c0f8*/ BL          0x000000000000C0F8          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000c0fc*/ MOV         R2, #40                     @ 0x28 
    /*0000c100*/ MOV         R1, R7 
    /*0000c104*/ ADD         R0, R4, #720                @ 0x2d0 
    /*0000c108*/ BL          0x000000000000C108          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000c10c*/ MOV         R2, #40                     @ 0x28 
    /*0000c110*/ MOV         R1, R7 
    /*0000c114*/ ADD         R0, R4, #760                @ 0x2f8 
    /*0000c118*/ BL          0x000000000000C118          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000c11c*/ MOV         R2, #12 
    /*0000c120*/ MOV         R1, R7 
    /*0000c124*/ ADD         R0, R4, #820                @ 0x334 
    /*0000c128*/ BL          0x000000000000C128          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000c12c*/ MOV         R2, #12 
    /*0000c130*/ MOV         R1, R7 
    /*0000c134*/ ADD         R0, R4, #832                @ 0x340 
    /*0000c138*/ BL          0x000000000000C138          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000c13c*/ MOV         R2, #12 
    /*0000c140*/ MOV         R1, R7 
    /*0000c144*/ ADD         R0, R4, #844                @ 0x34c 
    /*0000c148*/ BL          0x000000000000C148          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000c14c*/ LDR         R3, [SP] 
    /*0000c150*/ MOV         R2, #20 
    /*0000c154*/ MOV         R1, R7 
    /*0000c158*/ ADD         R0, R3, #200                @ 0xc8 
    /*0000c15c*/ ADD         R0, SL, R0 
    /*0000c160*/ BL          0x000000000000C160          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000c164*/ LDR         R3, [R5, #64]               @ 0x40 
    /*0000c168*/ CMP         R3, #1 
    /*0000c16c*/ STR         R3, [R4, #168]              @ 0xa8 
    /*0000c170*/ LDR         R3, [R5, #68]               @ 0x44 
    /*0000c174*/ STR         R3, [R4, #128]              @ 0x80 
    /*0000c178*/ BEQ         DIS_MOTION_FILTER_init_xc470
    /*0000c17c*/ LDR         R0, [R5, #72]               @ 0x48 
    /*0000c180*/ MOVW        R3, #19923                  @ 0x4dd3 
    /*0000c184*/ MOVT        R3, #4194                   @ 0x1062 
    /*0000c188*/ MOVW        R2, #34079                  @ 0x851f 
    /*0000c18c*/ MOVT        R2, #20971                  @ 0x51eb 
    /*0000c190*/ LSL         R0, R0, #13 
    /*0000c194*/ UMULL       R0, R1, R0, R3 
    /*0000c198*/ LSR         R1, R1, #6 
    /*0000c19c*/ STR         R1, [R4, #136]              @ 0x88 
    /*0000c1a0*/ LDR         R0, [R5, #76]               @ 0x4c 
    /*0000c1a4*/ LSL         R0, R0, #13 
    /*0000c1a8*/ UMULL       R0, R1, R0, R3 
    /*0000c1ac*/ LSR         R1, R1, #6 
    /*0000c1b0*/ STR         R1, [R4, #140]              @ 0x8c 
    /*0000c1b4*/ LDR         R0, [R5, #80]               @ 0x50 
    /*0000c1b8*/ LSL         R0, R0, #13 
    /*0000c1bc*/ UMULL       R0, R1, R0, R3 
    /*0000c1c0*/ LSR         R1, R1, #6 
    /*0000c1c4*/ STR         R1, [R4, #144]              @ 0x90 
    /*0000c1c8*/ LDR         R1, [R5, #84]               @ 0x54 
    /*0000c1cc*/ LSL         R1, R1, #7 
    /*0000c1d0*/ STR         R1, [R4, #148]              @ 0x94 
    /*0000c1d4*/ LDR         R0, [R5, #88]               @ 0x58 
    /*0000c1d8*/ LSL         R0, R0, #20 
    /*0000c1dc*/ UMULL       R0, R1, R0, R3 
    /*0000c1e0*/ LSR         R1, R1, #6 
    /*0000c1e4*/ STR         R1, [R4, #152]              @ 0x98 
    /*0000c1e8*/ LDR         IP, [R5, #92]               @ 0x5c 
    /*0000c1ec*/ LSL         IP, IP, #13 
    /*0000c1f0*/ SMULL       R0, R1, IP, R2 
    /*0000c1f4*/ ASR         IP, IP, #31 
    /*0000c1f8*/ RSB         IP, IP, R1, ASR #5 
    /*0000c1fc*/ STR         IP, [R4, #156]              @ 0x9c 
    /*0000c200*/ LDR         R2, [R5, #96]               @ 0x60 
    /*0000c204*/ STR         R2, [R4, #172]              @ 0xac 
    /*0000c208*/ LDR         R0, [R5, #104]              @ 0x68 
    /*0000c20c*/ LSL         R0, R0, #13 
    /*0000c210*/ UMULL       R0, R1, R0, R3 
    /*0000c214*/ LSR         R2, R1, #6 
    /*0000c218*/ STR         R2, [R4, #180]              @ 0xb4 
    /*0000c21c*/ LDR         R2, [R5, #100]              @ 0x64 
    /*0000c220*/ LSL         R2, R2, #13 
    /*0000c224*/ UMULL       R2, R3, R2, R3 
    /*0000c228*/ LSR         R3, R3, #6 
    /*0000c22c*/ STR         R3, [R4, #176]              @ 0xb0 
    /*0000c230*/ LDRB        R3, [R5, #108]              @ 0x6c 
    /*0000c234*/ CMP         R3, #8 
    /*0000c238*/ MOVCS       R3, #8 
    DIS_MOTION_FILTER_init_xc23c:
    /*0000c23c*/ LDR         R8, [R4, #28] 
    /*0000c240*/ MOVW        R0, #34079                  @ 0x851f 
    /*0000c244*/ MOVT        R0, #20971                  @ 0x51eb 
    /*0000c248*/ ADD         R2, R4, #288                @ 0x120 
    /*0000c24c*/ STRB        R3, [R4, #184]              @ 0xb8 
    /*0000c250*/ MOVW        R3, #290                    @ 0x122 
    /*0000c254*/ UMULL       R8, R9, R8, R0 
    /*0000c258*/ LDR         IP, [R5, #132]              @ 0x84 
    /*0000c25c*/ ADD         R7, R4, #292                @ 0x124 
    /*0000c260*/ MOVW        LR, #294                    @ 0x126 
    /*0000c264*/ CMP         IP, #0 
    /*0000c268*/ LSR         R1, R9, #4 
    /*0000c26c*/ STRH        R1, [R2] 
    /*0000c270*/ LDR         R8, [R4, #32] 
    /*0000c274*/ LSR         R1, R9, #5 
    /*0000c278*/ STRH        R1, [R4, R3] 
    /*0000c27c*/ MOV         R3, #0 
    /*0000c280*/ UMULL       R0, R1, R8, R0 
    /*0000c284*/ LSR         R0, R1, #4 
    /*0000c288*/ LSR         R1, R1, #5 
    /*0000c28c*/ STRH        R0, [R7] 
    /*0000c290*/ STRH        R1, [R4, LR] 
    /*0000c294*/ STR         R3, [R4, #272]              @ 0x110 
    /*0000c298*/ STR         R3, [R4, #276]              @ 0x114 
    /*0000c29c*/ STR         R3, [R4, #284]              @ 0x11c 
    /*0000c2a0*/ STR         R3, [R4, #280]              @ 0x118 
    /*0000c2a4*/ MOVGT       R3, #1 
    /*0000c2a8*/ STRGT       IP, [R4, #268]              @ 0x10c 
    /*0000c2ac*/ STR         R3, [R4, #264]              @ 0x108 
    /*0000c2b0*/ LDR         R3, [R5, #112]              @ 0x70 
    /*0000c2b4*/ CMP         R3, #0 
    /*0000c2b8*/ STR         R3, [R4, #296]              @ 0x128 
    /*0000c2bc*/ BNE         DIS_MOTION_FILTER_init_xc328
    /*0000c2c0*/ LDR         LR, [R4, #28] 
    /*0000c2c4*/ MOVW        R3, #19923                  @ 0x4dd3 
    /*0000c2c8*/ LDR         R0, [R5, #116]              @ 0x74 
    /*0000c2cc*/ MOVT        R3, #4194                   @ 0x1062 
    /*0000c2d0*/ MOVW        IP, #290                    @ 0x122 
    /*0000c2d4*/ MOVW        R8, #294                    @ 0x126 
    /*0000c2d8*/ MUL         R0, R0, LR 
    /*0000c2dc*/ UMULL       R0, R1, R0, R3 
    /*0000c2e0*/ LSR         R1, R1, #6 
    /*0000c2e4*/ STRH        R1, [R2] 
    /*0000c2e8*/ LDR         R0, [R5, #120]              @ 0x78 
    /*0000c2ec*/ LDR         R2, [R4, #32] 
    /*0000c2f0*/ MUL         R0, R0, LR 
    /*0000c2f4*/ UMULL       R0, R1, R0, R3 
    /*0000c2f8*/ LSR         R1, R1, #6 
    /*0000c2fc*/ STRH        R1, [R4, IP] 
    /*0000c300*/ LDR         R0, [R5, #124]              @ 0x7c 
    /*0000c304*/ MUL         R0, R0, R2 
    /*0000c308*/ UMULL       R0, R1, R0, R3 
    /*0000c30c*/ LSR         R1, R1, #6 
    /*0000c310*/ STRH        R1, [R7] 
    /*0000c314*/ LDR         R1, [R5, #128]              @ 0x80 
    /*0000c318*/ MUL         R2, R1, R2 
    /*0000c31c*/ UMULL       R2, R3, R2, R3 
    /*0000c320*/ LSR         R3, R3, #6 
    /*0000c324*/ STRH        R3, [R4, R8] 
    DIS_MOTION_FILTER_init_xc328:
    /*0000c328*/ LDR         R3, [SP] 
    /*0000c32c*/ MOV         R2, #1 
    /*0000c330*/ MOV         R1, #5 
    /*0000c334*/ ADD         R0, R3, #280                @ 0x118 
    /*0000c338*/ LDR         R3, [R5, #20] 
    /*0000c33c*/ ADD         R0, SL, R0 
    /*0000c340*/ BL          0x000000000000C340          @ HIISP_CQ_VECTOR_INIT [R_ARM_CALL: .text+0x6de4] 
    /*0000c344*/ SUBS        R7, R0, #0 
    /*0000c348*/ BNE         DIS_MOTION_FILTER_init_xc42c
    /*0000c34c*/ LDR         R3, [SP] 
    /*0000c350*/ STR         R7, [R4, #304]              @ 0x130 
    /*0000c354*/ ADD         R0, R3, #232                @ 0xe8 
    /*0000c358*/ MOVW        R3, #0                      @ getStabilizedMotion [R_ARM_MOVW_ABS_NC: .text+0xa350] 
    /*0000c35c*/ ADD         R0, SL, R0 
    /*0000c360*/ MOVT        R3, #0                      @ getStabilizedMotion [R_ARM_MOVT_ABS: .text+0xa350] 
    /*0000c364*/ STR         R3, [R4, #860]              @ 0x35c 
    /*0000c368*/ MOVW        R3, #0                      @ setGmeMotion [R_ARM_MOVW_ABS_NC: .text+0x9f80] 
    /*0000c36c*/ MOVT        R3, #0                      @ setGmeMotion [R_ARM_MOVT_ABS: .text+0x9f80] 
    /*0000c370*/ STR         R3, [R4, #868]              @ 0x364 
    /*0000c374*/ MOVW        R3, #0                      @ setGyroMotion [R_ARM_MOVW_ABS_NC: .text+0xba94] 
    /*0000c378*/ MOVT        R3, #0                      @ setGyroMotion [R_ARM_MOVT_ABS: .text+0xba94] 
    /*0000c37c*/ STR         R3, [R4, #864]              @ 0x360 
    /*0000c380*/ BL          0x000000000000C380          @ DIS_resetMotionModel [R_ARM_CALL: .text+0x6b70] 
    /*0000c384*/ LDR         R3, [SP, #16] 
    /*0000c388*/ MOV         R1, #344                    @ 0x158 
    /*0000c38c*/ MOV         R0, R7 
    /*0000c390*/ MLA         R1, R1, R3, SL 
    /*0000c394*/ LDR         R3, [SP, #8] 
    /*0000c398*/ LSR         R2, R3, #1 
    /*0000c39c*/ LDR         R3, [SP, #12] 
    /*0000c3a0*/ STR         R2, [R1, #192]              @ 0xc0 
    /*0000c3a4*/ LSR         R3, R3, #1 
    /*0000c3a8*/ STR         R3, [R1, #196]              @ 0xc4 
    /*0000c3ac*/ ADD         SP, SP, #36                 @ 0x24 
    /*0000c3b0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    DIS_MOTION_FILTER_init_xc3b4:
    /*0000c3b4*/ LDR         R0, [R5, #36]               @ 0x24 
    /*0000c3b8*/ MOV         R2, R6 
    /*0000c3bc*/ ASR         R1, R0, #31 
    /*0000c3c0*/ BL          0x000000000000C3C0          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*0000c3c4*/ LDR         R3, [R4, #28] 
    /*0000c3c8*/ LDR         R6, [R4, #188]              @ 0xbc 
    /*0000c3cc*/ LDR         LR, [R4, #24] 
    /*0000c3d0*/ STR         R3, [SP, #4] 
    /*0000c3d4*/ LDR         R9, [R4, #32] 
    /*0000c3d8*/ LDR         R3, [R4, #44]               @ 0x2c 
    /*0000c3dc*/ LDR         IP, [R4, #48]               @ 0x30 
    /*0000c3e0*/ LDR         R1, [R4, #36]               @ 0x24 
    /*0000c3e4*/ LDR         R2, [R4, #40]               @ 0x28 
    /*0000c3e8*/ SUB         R0, R6, R0 
    /*0000c3ec*/ STR         R0, [R4, #188]              @ 0xbc 
    /*0000c3f0*/ B           DIS_MOTION_FILTER_init_xbd98
    DIS_MOTION_FILTER_init_xc3f4:
    /*0000c3f4*/ MOVW        R2, #39321                  @ 0x9999 
    /*0000c3f8*/ MOVT        R2, #9 
    /*0000c3fc*/ STR         R2, [R4, #504]              @ 0x1f8 
    /*0000c400*/ MOV         R2, #524288                 @ 0x80000 
    /*0000c404*/ STR         R2, [R4, #500]              @ 0x1f4 
    /*0000c408*/ MOV         R2, #10 
    /*0000c40c*/ STR         R2, [R4, #480]              @ 0x1e0 
    /*0000c410*/ MOV         R2, #256                    @ 0x100 
    /*0000c414*/ STR         R2, [R4, #800]              @ 0x320 
    /*0000c418*/ MOV         R2, #384                    @ 0x180 
    /*0000c41c*/ STR         R2, [R4, #804]              @ 0x324 
    /*0000c420*/ MOV         R2, #524                    @ 0x20c 
    /*0000c424*/ STR         R2, [R4, #808]              @ 0x328 
    /*0000c428*/ B           DIS_MOTION_FILTER_init_xbfac
    DIS_MOTION_FILTER_init_xc42c:
    /*0000c42c*/ MOV         R0, FP 
    /*0000c430*/ BL          0x000000000000C430          @ DIS_MotionModel_VEC_uninit [R_ARM_CALL: .text+0x6d94] 
    /*0000c434*/ MOV         R0, R6 
    /*0000c438*/ BL          0x000000000000C438          @ HIISP_CQ_VECTOR_UNINIT [R_ARM_CALL: .text+0x6ed4] 
    /*0000c43c*/ LDR         R0, [SP, #4] 
    /*0000c440*/ BL          0x000000000000C440          @ HIISP_CQ_VECTOR_UNINIT [R_ARM_CALL: .text+0x6ed4] 
    /*0000c444*/ LDR         R0, [SP, #20] 
    /*0000c448*/ BL          0x000000000000C448          @ HIISP_CQ_VECTOR_UNINIT [R_ARM_CALL: .text+0x6ed4] 
    /*0000c44c*/ B           DIS_MOTION_FILTER_init_xbec4
    DIS_MOTION_FILTER_init_xc450:
    /*0000c450*/ MOV         R0, R6 
    /*0000c454*/ BL          0x000000000000C454          @ HIISP_CQ_VECTOR_UNINIT [R_ARM_CALL: .text+0x6ed4] 
    /*0000c458*/ B           DIS_MOTION_FILTER_init_xc068
    DIS_MOTION_FILTER_init_xc45c:
    /*0000c45c*/ MOV         R0, R6 
    /*0000c460*/ BL          0x000000000000C460          @ HIISP_CQ_VECTOR_UNINIT [R_ARM_CALL: .text+0x6ed4] 
    /*0000c464*/ LDR         R0, [SP, #4] 
    /*0000c468*/ BL          0x000000000000C468          @ HIISP_CQ_VECTOR_UNINIT [R_ARM_CALL: .text+0x6ed4] 
    /*0000c46c*/ B           DIS_MOTION_FILTER_init_xc068
    DIS_MOTION_FILTER_init_xc470:
    /*0000c470*/ LDR         R3, [R5] 
    /*0000c474*/ LDR         R2, [R5, #20] 
    /*0000c478*/ CMP         R3, #0 
    /*0000c47c*/ BNE         DIS_MOTION_FILTER_init_xc51c
    /*0000c480*/ CMP         R2, #32 
    /*0000c484*/ STR         R3, [R4, #172]              @ 0xac 
    /*0000c488*/ MOVW        R3, #39321                  @ 0x9999 
    /*0000c48c*/ MOVT        R3, #1 
    /*0000c490*/ STR         R3, [R4, #152]              @ 0x98 
    /*0000c494*/ MOVW        R3, #819                    @ 0x333 
    /*0000c498*/ STR         R3, [R4, #180]              @ 0xb4 
    /*0000c49c*/ MOVWLS      R3, #1638                   @ 0x666 
    /*0000c4a0*/ STRHI       R3, [R4, #156]              @ 0x9c 
    /*0000c4a4*/ MOVWHI      R3, #8601                   @ 0x2199 
    /*0000c4a8*/ STRLS       R3, [R4, #156]              @ 0x9c 
    /*0000c4ac*/ MOVWLS      R3, #9011                   @ 0x2333 
    /*0000c4b0*/ STRHI       R3, [R4, #136]              @ 0x88 
    /*0000c4b4*/ MOVWHI      R3, #7782                   @ 0x1e66 
    /*0000c4b8*/ STRLS       R3, [R4, #136]              @ 0x88 
    /*0000c4bc*/ MOVWLS      R3, #7372                   @ 0x1ccc 
    /*0000c4c0*/ STR         R3, [R4, #140]              @ 0x8c 
    /*0000c4c4*/ MOV         R2, #81                     @ 0x51 
    /*0000c4c8*/ LDR         R3, [R4, #116]              @ 0x74 
    /*0000c4cc*/ STR         R2, [R4, #176]              @ 0xb0 
    /*0000c4d0*/ MOV         R2, #2 
    /*0000c4d4*/ CMP         R3, #2 
    /*0000c4d8*/ STRB        R2, [R4, #184]              @ 0xb8 
    /*0000c4dc*/ MOV         R2, #245                    @ 0xf5 
    /*0000c4e0*/ STR         R2, [R4, #144]              @ 0x90 
    /*0000c4e4*/ MOVNE       R3, #2 
    /*0000c4e8*/ MOV         R2, #1280                   @ 0x500 
    /*0000c4ec*/ STR         R2, [R4, #148]              @ 0x94 
    /*0000c4f0*/ BNE         DIS_MOTION_FILTER_init_xc23c
    /*0000c4f4*/ LDR         R0, [R4, #180]              @ 0xb4 
    /*0000c4f8*/ LDR         R1, [R4, #152]              @ 0x98 
    /*0000c4fc*/ LDR         R2, [R4, #148]              @ 0x94 
    /*0000c500*/ LSL         R0, R0, #1 
    /*0000c504*/ STR         R0, [R4, #180]              @ 0xb4 
    /*0000c508*/ LSL         R1, R1, #1 
    /*0000c50c*/ STR         R1, [R4, #152]              @ 0x98 
    /*0000c510*/ LSL         R2, R2, #1 
    /*0000c514*/ STR         R2, [R4, #148]              @ 0x94 
    /*0000c518*/ B           DIS_MOTION_FILTER_init_xc23c
    DIS_MOTION_FILTER_init_xc51c:
    /*0000c51c*/ CMP         R3, #1 
    /*0000c520*/ BEQ         DIS_MOTION_FILTER_init_xc5a4
    /*0000c524*/ CMP         R3, #2 
    /*0000c528*/ BEQ         DIS_MOTION_FILTER_init_xc600
    /*0000c52c*/ CMP         R2, #32 
    /*0000c530*/ MOVW        R3, #409                    @ 0x199 
    /*0000c534*/ MOV         R2, #2048                   @ 0x800 
    /*0000c538*/ STR         R3, [R4, #176]              @ 0xb0 
    /*0000c53c*/ STR         R2, [R4, #180]              @ 0xb4 
    /*0000c540*/ MOV         R3, #4 
    /*0000c544*/ MOV         R2, #245                    @ 0xf5 
    /*0000c548*/ MOV         R1, #1280                   @ 0x500 
    /*0000c54c*/ STR         R2, [R4, #144]              @ 0x90 
    /*0000c550*/ MOVW        R2, #31457                  @ 0x7ae1 
    /*0000c554*/ STR         R7, [R4, #172]              @ 0xac 
    /*0000c558*/ STRB        R3, [R4, #184]              @ 0xb8 
    /*0000c55c*/ STR         R1, [R4, #148]              @ 0x94 
    /*0000c560*/ STR         R2, [R4, #152]              @ 0x98 
    /*0000c564*/ BLS         DIS_MOTION_FILTER_init_xc584
    /*0000c568*/ MOVW        R2, #8601                   @ 0x2199 
    /*0000c56c*/ STR         R2, [R4, #136]              @ 0x88 
    /*0000c570*/ MOVW        R2, #7782                   @ 0x1e66 
    /*0000c574*/ STR         R2, [R4, #140]              @ 0x8c 
    /*0000c578*/ MOVW        R2, #4915                   @ 0x1333 
    /*0000c57c*/ STR         R2, [R4, #156]              @ 0x9c 
    /*0000c580*/ B           DIS_MOTION_FILTER_init_xc23c
    DIS_MOTION_FILTER_init_xc584:
    /*0000c584*/ MOVW        R3, #1638                   @ 0x666 
    /*0000c588*/ STR         R3, [R4, #156]              @ 0x9c 
    /*0000c58c*/ MOVW        R3, #9011                   @ 0x2333 
    /*0000c590*/ STR         R3, [R4, #136]              @ 0x88 
    /*0000c594*/ MOVW        R3, #7372                   @ 0x1ccc 
    /*0000c598*/ STR         R3, [R4, #140]              @ 0x8c 
    /*0000c59c*/ MOV         R3, #4 
    /*0000c5a0*/ B           DIS_MOTION_FILTER_init_xc23c
    DIS_MOTION_FILTER_init_xc5a4:
    /*0000c5a4*/ CMP         R2, #32 
    /*0000c5a8*/ MOVW        R3, #39321                  @ 0x9999 
    /*0000c5ac*/ MOVW        R2, #409                    @ 0x199 
    /*0000c5b0*/ MOVT        R3, #1 
    /*0000c5b4*/ STR         R2, [R4, #176]              @ 0xb0 
    /*0000c5b8*/ MOV         R2, #2048                   @ 0x800 
    /*0000c5bc*/ STR         R3, [R4, #152]              @ 0x98 
    /*0000c5c0*/ MOV         R3, #4 
    /*0000c5c4*/ STR         R2, [R4, #180]              @ 0xb4 
    /*0000c5c8*/ MOV         R2, #245                    @ 0xf5 
    /*0000c5cc*/ STR         R7, [R4, #172]              @ 0xac 
    /*0000c5d0*/ STR         R2, [R4, #144]              @ 0x90 
    /*0000c5d4*/ MOV         R2, #1280                   @ 0x500 
    /*0000c5d8*/ STRB        R3, [R4, #184]              @ 0xb8 
    /*0000c5dc*/ STR         R2, [R4, #148]              @ 0x94 
    /*0000c5e0*/ BLS         DIS_MOTION_FILTER_init_xc584
    /*0000c5e4*/ MOVW        R2, #8601                   @ 0x2199 
    /*0000c5e8*/ STR         R2, [R4, #136]              @ 0x88 
    /*0000c5ec*/ MOVW        R2, #7782                   @ 0x1e66 
    /*0000c5f0*/ STR         R2, [R4, #140]              @ 0x8c 
    /*0000c5f4*/ MOVW        R2, #1638                   @ 0x666 
    /*0000c5f8*/ STR         R2, [R4, #156]              @ 0x9c 
    /*0000c5fc*/ B           DIS_MOTION_FILTER_init_xc23c
    DIS_MOTION_FILTER_init_xc600:
    /*0000c600*/ CMP         R2, #32 
    /*0000c604*/ MOVW        R3, #39321                  @ 0x9999 
    /*0000c608*/ MOV         R2, #81                     @ 0x51 
    /*0000c60c*/ MOVT        R3, #1 
    /*0000c610*/ STR         R2, [R4, #176]              @ 0xb0 
    /*0000c614*/ MOV         R2, #2048                   @ 0x800 
    /*0000c618*/ STR         R3, [R4, #152]              @ 0x98 
    /*0000c61c*/ MOV         R3, #4 
    /*0000c620*/ STR         R2, [R4, #180]              @ 0xb4 
    /*0000c624*/ MOV         R2, #245                    @ 0xf5 
    /*0000c628*/ STR         R7, [R4, #172]              @ 0xac 
    /*0000c62c*/ STR         R2, [R4, #144]              @ 0x90 
    /*0000c630*/ MOV         R2, #1280                   @ 0x500 
    /*0000c634*/ STRB        R3, [R4, #184]              @ 0xb8 
    /*0000c638*/ STR         R2, [R4, #148]              @ 0x94 
    /*0000c63c*/ BLS         DIS_MOTION_FILTER_init_xc584
    /*0000c640*/ MOVW        R2, #8601                   @ 0x2199 
    /*0000c644*/ STR         R2, [R4, #136]              @ 0x88 
    /*0000c648*/ MOVW        R2, #7782                   @ 0x1e66 
    /*0000c64c*/ STR         R2, [R4, #140]              @ 0x8c 
    /*0000c650*/ MOVW        R2, #819                    @ 0x333 
    /*0000c654*/ STR         R2, [R4, #156]              @ 0x9c 
    /*0000c658*/ B           DIS_MOTION_FILTER_init_xc23c
    /*0000c65c*/ @           <UNDEFINED> instruction: 0x000005b0 
FUNC_END DIS_MOTION_FILTER_init


FUNC_BEGIN DIS_MOTION_FILTER_uninit
    /*0000c660*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000c664*/ MOV         R4, #344                    @ 0x158 
    /*0000c668*/ MUL         R4, R4, R0 
    /*0000c66c*/ MOVW        R5, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x16f8] 
    /*0000c670*/ MOVT        R5, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x16f8] 
    /*0000c674*/ ADD         R0, R4, #104                @ 0x68 
    /*0000c678*/ ADD         R7, R5, R4 
    /*0000c67c*/ ADD         R0, R5, R0 
    /*0000c680*/ LDR         R6, [R5, R4] 
    /*0000c684*/ BL          0x000000000000C684          @ DIS_MotionModel_VEC_uninit [R_ARM_CALL: .text+0x6d94] 
    /*0000c688*/ CMP         R6, #0 
    /*0000c68c*/ BEQ         DIS_MOTION_FILTER_uninit_xc6a8
    /*0000c690*/ ADD         R0, R6, #508                @ 0x1fc 
    /*0000c694*/ BL          0x000000000000C694          @ HIISP_CQ_VECTOR_UNINIT [R_ARM_CALL: .text+0x6ed4] 
    /*0000c698*/ ADD         R0, R6, #564                @ 0x234 
    /*0000c69c*/ BL          0x000000000000C69C          @ HIISP_CQ_VECTOR_UNINIT [R_ARM_CALL: .text+0x6ed4] 
    /*0000c6a0*/ ADD         R0, R6, #620                @ 0x26c 
    /*0000c6a4*/ BL          0x000000000000C6A4          @ HIISP_CQ_VECTOR_UNINIT [R_ARM_CALL: .text+0x6ed4] 
    DIS_MOTION_FILTER_uninit_xc6a8:
    /*0000c6a8*/ ADD         R0, R4, #280                @ 0x118 
    /*0000c6ac*/ ADD         R0, R5, R0 
    /*0000c6b0*/ BL          0x000000000000C6B0          @ HIISP_CQ_VECTOR_UNINIT [R_ARM_CALL: .text+0x6ed4] 
    /*0000c6b4*/ MOV         R0, R7 
    /*0000c6b8*/ MOV         R2, #344                    @ 0x158 
    /*0000c6bc*/ MOV         R1, #0 
    /*0000c6c0*/ POP         {R4, R5, R6, R7, R8, LR} 
    DIS_MOTION_FILTER_uninit_xc6c4:
    /*0000c6c4*/ B           DIS_MOTION_FILTER_uninit_xc6c4@ osal_memset [R_ARM_JUMP24: +0x0] 
FUNC_END DIS_MOTION_FILTER_uninit


FUNC_BEGIN hiisp_dis_SetConfig
    /*0000c6c8*/ LDR         R3, [R0] 
    /*0000c6cc*/ CMP         R3, #0 
    /*0000c6d0*/ BEQ         hiisp_dis_SetConfig_xc91c
    /*0000c6d4*/ CMP         R3, #1 
    /*0000c6d8*/ BNE         hiisp_dis_SetConfig_xc904
    /*0000c6dc*/ MOV         R3, #6 
    /*0000c6e0*/ STR         R3, [R2] 
    hiisp_dis_SetConfig_xc6e4:
    /*0000c6e4*/ PUSH        {R4, R5, R6, R7, LR} 
    /*0000c6e8*/ MOV         IP, #2 
    /*0000c6ec*/ LDR         R3, [R0] 
    /*0000c6f0*/ STR         R3, [R2, #12] 
    /*0000c6f4*/ LDR         R3, [R0, #8] 
    /*0000c6f8*/ STR         R3, [R2, #16] 
    /*0000c6fc*/ LDR         R3, [R1, #8] 
    /*0000c700*/ STR         R3, [R2, #20] 
    /*0000c704*/ LDR         R3, [R1, #12] 
    /*0000c708*/ STR         R3, [R2, #24] 
    /*0000c70c*/ LDR         R3, [R1] 
    /*0000c710*/ STR         R3, [R2, #28] 
    /*0000c714*/ LDR         R3, [R1, #4] 
    /*0000c718*/ STR         R3, [R2, #32] 
    /*0000c71c*/ LDR         R3, [R0, #16] 
    /*0000c720*/ LSL         R3, R3, #13 
    /*0000c724*/ RSB         R3, R3, #819200             @ 0xc8000 
    /*0000c728*/ LSR         R3, R3, #1 
    /*0000c72c*/ STR         R3, [R2, #36]               @ 0x24 
    /*0000c730*/ LDR         R3, [R0, #4] 
    /*0000c734*/ STR         IP, [R2, #48]               @ 0x30 
    /*0000c738*/ MOVW        IP, #19000                  @ 0x4a38 
    /*0000c73c*/ STR         R3, [R2, #44]               @ 0x2c 
    /*0000c740*/ LDR         R3, [R0, #32] 
    /*0000c744*/ STR         R3, [R2, #52]               @ 0x34 
    /*0000c748*/ LDR         R3, [R0, #20] 
    /*0000c74c*/ STR         R3, [R2, #56]               @ 0x38 
    /*0000c750*/ LDR         R3, [R0, #8] 
    /*0000c754*/ STR         IP, [R2, #176]              @ 0xb0 
    /*0000c758*/ SUB         R3, R3, #1 
    /*0000c75c*/ CMP         R3, #1 
    /*0000c760*/ MOVLS       R3, #0 
    /*0000c764*/ STRLS       R3, [R2, #52]               @ 0x34 
    /*0000c768*/ MOV         R3, #1200                   @ 0x4b0 
    /*0000c76c*/ STR         R3, [R2, #180]              @ 0xb4 
    /*0000c770*/ LDR         R3, [R0, #24] 
    /*0000c774*/ STR         R3, [R2, #184]              @ 0xb8 
    /*0000c778*/ LDR         R3, [R0, #28] 
    /*0000c77c*/ STR         R3, [R2, #188]              @ 0xbc 
    /*0000c780*/ LDR         R3, [R1, #28] 
    /*0000c784*/ CMP         R3, #0 
    /*0000c788*/ BNE         hiisp_dis_SetConfig_xc928
    /*0000c78c*/ MOV         IP, #10 
    /*0000c790*/ MOV         LR, #1 
    /*0000c794*/ MOV         R4, #100                    @ 0x64 
    /*0000c798*/ MOV         R5, #2 
    /*0000c79c*/ STR         IP, [R2, #60]               @ 0x3c 
    /*0000c7a0*/ MOV         R6, #16 
    /*0000c7a4*/ STR         IP, [R2, #104]              @ 0x68 
    /*0000c7a8*/ MOV         R7, #150                    @ 0x96 
    /*0000c7ac*/ STR         IP, [R2, #216]              @ 0xd8 
    /*0000c7b0*/ STR         R4, [R2, #72]               @ 0x48 
    /*0000c7b4*/ STR         R4, [R2, #76]               @ 0x4c 
    /*0000c7b8*/ STR         R4, [R2, #224]              @ 0xe0 
    /*0000c7bc*/ STR         LR, [R2, #80]               @ 0x50 
    /*0000c7c0*/ STR         LR, [R2, #192]              @ 0xc0 
    /*0000c7c4*/ STR         LR, [R2, #196]              @ 0xc4 
    /*0000c7c8*/ STR         LR, [R2, #200]              @ 0xc8 
    /*0000c7cc*/ STR         LR, [R2, #228]              @ 0xe4 
    /*0000c7d0*/ STR         R5, [R2, #84]               @ 0x54 
    /*0000c7d4*/ MOV         R5, #300                    @ 0x12c 
    /*0000c7d8*/ STR         R6, [R2, #88]               @ 0x58 
    /*0000c7dc*/ MOV         R6, #3 
    /*0000c7e0*/ STR         R5, [R2, #92]               @ 0x5c 
    /*0000c7e4*/ MOV         R5, #50                     @ 0x32 
    /*0000c7e8*/ STR         R6, [R2, #96]               @ 0x60 
    /*0000c7ec*/ MOV         R6, #75                     @ 0x4b 
    /*0000c7f0*/ STR         R5, [R2, #108]              @ 0x6c 
    /*0000c7f4*/ STR         R5, [R2, #232]              @ 0xe8 
    /*0000c7f8*/ MOV         R5, #512                    @ 0x200 
    /*0000c7fc*/ STR         R7, [R2, #112]              @ 0x70 
    /*0000c800*/ MOV         R7, #144                    @ 0x90 
    /*0000c804*/ STR         R6, [R2, #116]              @ 0x74 
    /*0000c808*/ MOVW        R6, #575                    @ 0x23f 
    /*0000c80c*/ STR         R5, [R2, #120]              @ 0x78 
    /*0000c810*/ STR         R5, [R2, #124]              @ 0x7c 
    /*0000c814*/ MOVW        R5, #1050                   @ 0x41a 
    /*0000c818*/ STR         R7, [R2, #128]              @ 0x80 
    /*0000c81c*/ MOV         R7, #250                    @ 0xfa 
    /*0000c820*/ STR         R6, [R2, #132]              @ 0x84 
    /*0000c824*/ MOVW        R6, #950                    @ 0x3b6 
    /*0000c828*/ STR         R5, [R2, #204]              @ 0xcc 
    /*0000c82c*/ MOV         R5, #30 
    /*0000c830*/ STR         R6, [R2, #208]              @ 0xd0 
    /*0000c834*/ MOV         R6, #4 
    /*0000c838*/ STR         R5, [R2, #212]              @ 0xd4 
    /*0000c83c*/ STR         R5, [R2, #220]              @ 0xdc 
    /*0000c840*/ MOV         R5, #20 
    /*0000c844*/ STR         R3, [R2, #4] 
    /*0000c848*/ STR         R3, [R2, #64]               @ 0x40 
    /*0000c84c*/ STR         R3, [R2, #68]               @ 0x44 
    /*0000c850*/ STR         R3, [R2, #100]              @ 0x64 
    /*0000c854*/ STR         R7, [R2, #236]              @ 0xec 
    /*0000c858*/ STR         R6, [R2, #240]              @ 0xf0 
    /*0000c85c*/ MOV         R6, #500                    @ 0x1f4 
    /*0000c860*/ STR         IP, [R2, #248]              @ 0xf8 
    /*0000c864*/ STR         R4, [R2, #252]              @ 0xfc 
    /*0000c868*/ STR         R4, [R2, #256]              @ 0x100 
    /*0000c86c*/ MOVW        R4, #750                    @ 0x2ee 
    /*0000c870*/ STR         LR, [R2, #260]              @ 0x104 
    /*0000c874*/ STR         IP, [R2, #268]              @ 0x10c 
    /*0000c878*/ STR         IP, [R2, #276]              @ 0x114 
    /*0000c87c*/ STR         LR, [R2, #284]              @ 0x11c 
    /*0000c880*/ MOV         LR, #200                    @ 0xc8 
    /*0000c884*/ STR         IP, [R2, #308]              @ 0x134 
    /*0000c888*/ MOV         IP, #5 
    /*0000c88c*/ STR         R3, [R2, #244]              @ 0xf4 
    /*0000c890*/ STR         R3, [R2, #280]              @ 0x118 
    /*0000c894*/ STR         R5, [R2, #264]              @ 0x108 
    /*0000c898*/ STR         R5, [R2, #272]              @ 0x110 
    /*0000c89c*/ STR         R6, [R2, #288]              @ 0x120 
    /*0000c8a0*/ STR         R4, [R2, #292]              @ 0x124 
    /*0000c8a4*/ STR         LR, [R2, #304]              @ 0x130 
    /*0000c8a8*/ STR         LR, [R2, #296]              @ 0x128 
    /*0000c8ac*/ STR         IP, [R2, #300]              @ 0x12c 
    hiisp_dis_SetConfig_xc8b0:
    /*0000c8b0*/ LDR         R3, [R1, #16] 
    /*0000c8b4*/ MOV         LR, #15 
    /*0000c8b8*/ MOV         IP, #4 
    /*0000c8bc*/ STR         R3, [R2, #312]              @ 0x138 
    /*0000c8c0*/ LDR         R3, [R1, #20] 
    /*0000c8c4*/ STR         R3, [R2, #316]              @ 0x13c 
    /*0000c8c8*/ LDR         R3, [R1, #24] 
    /*0000c8cc*/ STR         LR, [R2, #324]              @ 0x144 
    /*0000c8d0*/ STR         IP, [R2, #328]              @ 0x148 
    /*0000c8d4*/ STR         R3, [R2, #320]              @ 0x140 
    /*0000c8d8*/ LDR         R3, [R0, #36]               @ 0x24 
    /*0000c8dc*/ STR         R3, [R2, #340]              @ 0x154 
    /*0000c8e0*/ LDR         R3, [R0, #12] 
    /*0000c8e4*/ STR         R3, [R2, #332]              @ 0x14c 
    /*0000c8e8*/ LDR         R3, [R0, #16] 
    /*0000c8ec*/ STR         R3, [R2, #336]              @ 0x150 
    /*0000c8f0*/ LDR         R3, [R0] 
    /*0000c8f4*/ STR         R3, [R2, #348]              @ 0x15c 
    /*0000c8f8*/ LDR         R3, [R0, #36]               @ 0x24 
    /*0000c8fc*/ STR         R3, [R2, #40]               @ 0x28 
    /*0000c900*/ POP         {R4, R5, R6, R7, PC} 
    hiisp_dis_SetConfig_xc904:
    /*0000c904*/ MOVW        R1, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .rodata+0x5b8] 
    /*0000c908*/ MOVW        R0, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x25f0] 
    /*0000c90c*/ MOVT        R1, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .rodata+0x5b8] 
    /*0000c910*/ MOVT        R0, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x25f0] 
    /*0000c914*/ MOV         R2, #181                    @ 0xb5 
    hiisp_dis_SetConfig_xc918:
    /*0000c918*/ B           hiisp_dis_SetConfig_xc918   @ osal_printk [R_ARM_JUMP24: +0x0] 
    hiisp_dis_SetConfig_xc91c:
    /*0000c91c*/ MOV         R3, #4 
    /*0000c920*/ STR         R3, [R2] 
    /*0000c924*/ B           hiisp_dis_SetConfig_xc6e4
    hiisp_dis_SetConfig_xc928:
    /*0000c928*/ LDR         IP, [R1, #32] 
    /*0000c92c*/ MOV         R3, #512                    @ 0x200 
    /*0000c930*/ STR         IP, [R2, #60]               @ 0x3c 
    /*0000c934*/ LDR         IP, [R1, #40]               @ 0x28 
    /*0000c938*/ STR         IP, [R2, #8] 
    /*0000c93c*/ LDR         IP, [R1, #36]               @ 0x24 
    /*0000c940*/ STR         IP, [R2, #4] 
    /*0000c944*/ LDR         IP, [R1, #44]               @ 0x2c 
    /*0000c948*/ STR         IP, [R2, #64]               @ 0x40 
    /*0000c94c*/ LDR         IP, [R1, #48]               @ 0x30 
    /*0000c950*/ STR         IP, [R2, #68]               @ 0x44 
    /*0000c954*/ LDR         IP, [R1, #52]               @ 0x34 
    /*0000c958*/ STR         IP, [R2, #72]               @ 0x48 
    /*0000c95c*/ LDR         IP, [R1, #56]               @ 0x38 
    /*0000c960*/ STR         IP, [R2, #76]               @ 0x4c 
    /*0000c964*/ LDR         IP, [R1, #60]               @ 0x3c 
    /*0000c968*/ STR         IP, [R2, #80]               @ 0x50 
    /*0000c96c*/ LDR         IP, [R1, #64]               @ 0x40 
    /*0000c970*/ STR         IP, [R2, #84]               @ 0x54 
    /*0000c974*/ LDR         IP, [R1, #68]               @ 0x44 
    /*0000c978*/ STR         IP, [R2, #88]               @ 0x58 
    /*0000c97c*/ LDR         IP, [R1, #72]               @ 0x48 
    /*0000c980*/ STR         IP, [R2, #92]               @ 0x5c 
    /*0000c984*/ LDR         IP, [R1, #76]               @ 0x4c 
    /*0000c988*/ STR         IP, [R2, #96]               @ 0x60 
    /*0000c98c*/ LDR         IP, [R1, #80]               @ 0x50 
    /*0000c990*/ STR         IP, [R2, #100]              @ 0x64 
    /*0000c994*/ LDR         IP, [R1, #84]               @ 0x54 
    /*0000c998*/ STR         IP, [R2, #104]              @ 0x68 
    /*0000c99c*/ LDR         IP, [R1, #88]               @ 0x58 
    /*0000c9a0*/ STR         IP, [R2, #108]              @ 0x6c 
    /*0000c9a4*/ LDR         IP, [R1, #92]               @ 0x5c 
    /*0000c9a8*/ STR         IP, [R2, #112]              @ 0x70 
    /*0000c9ac*/ LDR         IP, [R1, #96]               @ 0x60 
    /*0000c9b0*/ STR         R3, [R2, #120]              @ 0x78 
    /*0000c9b4*/ STR         R3, [R2, #124]              @ 0x7c 
    /*0000c9b8*/ STR         IP, [R2, #116]              @ 0x74 
    /*0000c9bc*/ LDR         R3, [R1, #108]              @ 0x6c 
    /*0000c9c0*/ STR         R3, [R2, #128]              @ 0x80 
    /*0000c9c4*/ LDR         R3, [R1, #112]              @ 0x70 
    /*0000c9c8*/ STR         R3, [R2, #132]              @ 0x84 
    /*0000c9cc*/ LDR         R3, [R1, #116]              @ 0x74 
    /*0000c9d0*/ STR         R3, [R2, #192]              @ 0xc0 
    /*0000c9d4*/ LDR         R3, [R1, #168]              @ 0xa8 
    /*0000c9d8*/ STR         R3, [R2, #224]              @ 0xe0 
    /*0000c9dc*/ LDR         R3, [R1, #172]              @ 0xac 
    /*0000c9e0*/ STR         R3, [R2, #244]              @ 0xf4 
    /*0000c9e4*/ LDR         R3, [R1, #176]              @ 0xb0 
    /*0000c9e8*/ STR         R3, [R2, #248]              @ 0xf8 
    /*0000c9ec*/ LDR         R3, [R1, #180]              @ 0xb4 
    /*0000c9f0*/ STR         R3, [R2, #252]              @ 0xfc 
    /*0000c9f4*/ LDR         R3, [R1, #120]              @ 0x78 
    /*0000c9f8*/ STR         R3, [R2, #196]              @ 0xc4 
    /*0000c9fc*/ LDR         R3, [R1, #124]              @ 0x7c 
    /*0000ca00*/ STR         R3, [R2, #200]              @ 0xc8 
    /*0000ca04*/ LDR         R3, [R1, #128]              @ 0x80 
    /*0000ca08*/ STR         R3, [R2, #204]              @ 0xcc 
    /*0000ca0c*/ LDR         R3, [R1, #132]              @ 0x84 
    /*0000ca10*/ STR         R3, [R2, #208]              @ 0xd0 
    /*0000ca14*/ LDR         R3, [R1, #136]              @ 0x88 
    /*0000ca18*/ STR         R3, [R2, #212]              @ 0xd4 
    /*0000ca1c*/ LDR         R3, [R1, #140]              @ 0x8c 
    /*0000ca20*/ STR         R3, [R2, #216]              @ 0xd8 
    /*0000ca24*/ LDR         R3, [R1, #144]              @ 0x90 
    /*0000ca28*/ STR         R3, [R2, #220]              @ 0xdc 
    /*0000ca2c*/ LDR         R3, [R1, #164]              @ 0xa4 
    /*0000ca30*/ STR         R3, [R2, #256]              @ 0x100 
    /*0000ca34*/ LDR         R3, [R1, #184]              @ 0xb8 
    /*0000ca38*/ STR         R3, [R2, #260]              @ 0x104 
    /*0000ca3c*/ LDR         R3, [R1, #188]              @ 0xbc 
    /*0000ca40*/ STR         R3, [R2, #264]              @ 0x108 
    /*0000ca44*/ LDR         R3, [R1, #192]              @ 0xc0 
    /*0000ca48*/ STR         R3, [R2, #268]              @ 0x10c 
    /*0000ca4c*/ LDR         R3, [R1, #196]              @ 0xc4 
    /*0000ca50*/ STR         R3, [R2, #272]              @ 0x110 
    /*0000ca54*/ LDR         R3, [R1, #200]              @ 0xc8 
    /*0000ca58*/ STR         R3, [R2, #276]              @ 0x114 
    /*0000ca5c*/ LDR         R3, [R1, #204]              @ 0xcc 
    /*0000ca60*/ STR         R3, [R2, #280]              @ 0x118 
    /*0000ca64*/ LDR         R3, [R1, #208]              @ 0xd0 
    /*0000ca68*/ STR         R3, [R2, #284]              @ 0x11c 
    /*0000ca6c*/ LDR         R3, [R1, #212]              @ 0xd4 
    /*0000ca70*/ STR         R3, [R2, #288]              @ 0x120 
    /*0000ca74*/ LDR         R3, [R1, #216]              @ 0xd8 
    /*0000ca78*/ STR         R3, [R2, #292]              @ 0x124 
    /*0000ca7c*/ LDR         R3, [R1, #228]              @ 0xe4 
    /*0000ca80*/ STR         R3, [R2, #304]              @ 0x130 
    /*0000ca84*/ LDR         R3, [R1, #232]              @ 0xe8 
    /*0000ca88*/ STR         R3, [R2, #308]              @ 0x134 
    /*0000ca8c*/ LDR         R3, [R1, #220]              @ 0xdc 
    /*0000ca90*/ STR         R3, [R2, #296]              @ 0x128 
    /*0000ca94*/ LDR         R3, [R1, #224]              @ 0xe0 
    /*0000ca98*/ STR         R3, [R2, #300]              @ 0x12c 
    /*0000ca9c*/ B           hiisp_dis_SetConfig_xc8b0
FUNC_END hiisp_dis_SetConfig


FUNC_BEGIN hiisp_dis_CalcFirmwareAttr
    /*0000caa0*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000caa4*/ MOV         R4, R1 
    /*0000caa8*/ LDR         R1, [R0] 
    /*0000caac*/ MOV         R2, #0 
    /*0000cab0*/ STR         R2, [R4, #56]               @ 0x38 
    /*0000cab4*/ MOV         R5, R0 
    /*0000cab8*/ STR         R2, [R4, #60]               @ 0x3c 
    /*0000cabc*/ MOVW        R3, #34079                  @ 0x851f 
    /*0000cac0*/ STRB        R1, [R4] 
    /*0000cac4*/ MOVT        R3, #20971                  @ 0x51eb 
    /*0000cac8*/ LDR         IP, [R0, #44]               @ 0x2c 
    /*0000cacc*/ ADD         R1, R0, #140                @ 0x8c 
    /*0000cad0*/ MOV         R2, #36                     @ 0x24 
    /*0000cad4*/ ADD         R0, R4, #232                @ 0xe8 
    /*0000cad8*/ STR         IP, [R4, #68]               @ 0x44 
    /*0000cadc*/ LDR         IP, [R5, #8] 
    /*0000cae0*/ STR         IP, [R4, #8] 
    /*0000cae4*/ LDR         IP, [R5, #12] 
    /*0000cae8*/ STR         IP, [R4, #12] 
    /*0000caec*/ LDR         IP, [R5] 
    /*0000caf0*/ STRB        IP, [R4] 
    /*0000caf4*/ LDR         IP, [R5, #4] 
    /*0000caf8*/ STR         IP, [R4, #4] 
    /*0000cafc*/ LDR         IP, [R5, #24] 
    /*0000cb00*/ STR         IP, [R4, #28] 
    /*0000cb04*/ LDR         IP, [R5, #20] 
    /*0000cb08*/ STR         IP, [R4, #20] 
    /*0000cb0c*/ LDR         IP, [R5, #28] 
    /*0000cb10*/ STR         IP, [R4, #48]               @ 0x30 
    /*0000cb14*/ LDR         IP, [R5, #32] 
    /*0000cb18*/ STR         IP, [R4, #52]               @ 0x34 
    /*0000cb1c*/ LDR         IP, [R5, #56]               @ 0x38 
    /*0000cb20*/ STR         IP, [R4, #80]               @ 0x50 
    /*0000cb24*/ LDR         R6, [R5, #36]               @ 0x24 
    /*0000cb28*/ UMULL       R6, R7, R6, R3 
    /*0000cb2c*/ LSR         IP, R7, #5 
    /*0000cb30*/ STR         IP, [R4, #64]               @ 0x40 
    /*0000cb34*/ LDR         IP, [R5, #280]              @ 0x118 
    /*0000cb38*/ STR         IP, [R4, #384]              @ 0x180 
    /*0000cb3c*/ LDR         IP, [R5, #52]               @ 0x34 
    /*0000cb40*/ STR         IP, [R4, #76]               @ 0x4c 
    /*0000cb44*/ LDR         IP, [R5, #16] 
    /*0000cb48*/ STR         IP, [R4, #16] 
    /*0000cb4c*/ LDR         IP, [R5, #48]               @ 0x30 
    /*0000cb50*/ STR         IP, [R4, #72]               @ 0x48 
    /*0000cb54*/ LDR         IP, [R5, #60]               @ 0x3c 
    /*0000cb58*/ STR         IP, [R4, #92]               @ 0x5c 
    /*0000cb5c*/ LDR         IP, [R5, #312]              @ 0x138 
    /*0000cb60*/ STR         IP, [R4, #36]               @ 0x24 
    /*0000cb64*/ LDR         IP, [R5, #316]              @ 0x13c 
    /*0000cb68*/ STR         IP, [R4, #40]               @ 0x28 
    /*0000cb6c*/ LDR         IP, [R5, #320]              @ 0x140 
    /*0000cb70*/ STR         IP, [R4, #44]               @ 0x2c 
    /*0000cb74*/ LDR         IP, [R5, #64]               @ 0x40 
    /*0000cb78*/ LDR         R6, [R5, #28] 
    /*0000cb7c*/ MUL         R6, R6, IP 
    /*0000cb80*/ UMULL       R6, R7, R6, R3 
    /*0000cb84*/ LSR         IP, R7, #5 
    /*0000cb88*/ STRH        IP, [R4, #96]               @ 0x60 
    /*0000cb8c*/ LDR         IP, [R5, #68]               @ 0x44 
    /*0000cb90*/ LDR         R6, [R5, #32] 
    /*0000cb94*/ MUL         R6, R6, IP 
    /*0000cb98*/ UMULL       R6, R7, R6, R3 
    /*0000cb9c*/ LSR         IP, R7, #5 
    /*0000cba0*/ STRH        IP, [R4, #98]               @ 0x62 
    /*0000cba4*/ LDR         IP, [R5, #72]               @ 0x48 
    /*0000cba8*/ LDR         R6, [R5, #28] 
    /*0000cbac*/ MUL         R6, R6, IP 
    /*0000cbb0*/ UMULL       R6, R7, R6, R3 
    /*0000cbb4*/ LSR         IP, R7, #5 
    /*0000cbb8*/ STRH        IP, [R4, #100]              @ 0x64 
    /*0000cbbc*/ LDR         IP, [R5, #76]               @ 0x4c 
    /*0000cbc0*/ LDR         R6, [R5, #32] 
    /*0000cbc4*/ MUL         R6, R6, IP 
    /*0000cbc8*/ UMULL       R6, R7, R6, R3 
    /*0000cbcc*/ LSR         R3, R7, #5 
    /*0000cbd0*/ STRH        R3, [R4, #102]              @ 0x66 
    /*0000cbd4*/ LDR         R3, [R5, #80]               @ 0x50 
    /*0000cbd8*/ STR         R3, [R4, #104]              @ 0x68 
    /*0000cbdc*/ LDR         R3, [R5, #84]               @ 0x54 
    /*0000cbe0*/ STRB        R3, [R4, #112]              @ 0x70 
    /*0000cbe4*/ LDR         R3, [R5, #88]               @ 0x58 
    /*0000cbe8*/ STR         R3, [R4, #116]              @ 0x74 
    /*0000cbec*/ LDR         R3, [R5, #92]               @ 0x5c 
    /*0000cbf0*/ STRH        R3, [R4, #120]              @ 0x78 
    /*0000cbf4*/ LDR         R3, [R5, #96]               @ 0x60 
    /*0000cbf8*/ STRB        R3, [R4, #122]              @ 0x7a 
    /*0000cbfc*/ LDR         R3, [R5, #100]              @ 0x64 
    /*0000cc00*/ STRH        R3, [R4, #124]              @ 0x7c 
    /*0000cc04*/ LDR         R3, [R5, #104]              @ 0x68 
    /*0000cc08*/ STRB        R3, [R4, #136]              @ 0x88 
    /*0000cc0c*/ LDR         R3, [R5, #132]              @ 0x84 
    /*0000cc10*/ STR         R3, [R4, #140]              @ 0x8c 
    /*0000cc14*/ LDR         R3, [R5, #112]              @ 0x70 
    /*0000cc18*/ STRH        R3, [R4, #144]              @ 0x90 
    /*0000cc1c*/ LDR         R3, [R5, #108]              @ 0x6c 
    /*0000cc20*/ STRH        R3, [R4, #146]              @ 0x92 
    /*0000cc24*/ LDR         R3, [R5, #116]              @ 0x74 
    /*0000cc28*/ STR         R3, [R4, #148]              @ 0x94 
    /*0000cc2c*/ LDR         R3, [R5, #128]              @ 0x80 
    /*0000cc30*/ STRB        R3, [R4, #152]              @ 0x98 
    /*0000cc34*/ LDR         R3, [R5, #120]              @ 0x78 
    /*0000cc38*/ STR         R3, [R4, #216]              @ 0xd8 
    /*0000cc3c*/ LDR         R3, [R5, #124]              @ 0x7c 
    /*0000cc40*/ STR         R3, [R4, #220]              @ 0xdc 
    /*0000cc44*/ LDR         R3, [R5, #136]              @ 0x88 
    /*0000cc48*/ STR         R3, [R4, #228]              @ 0xe4 
    /*0000cc4c*/ LDR         R3, [R5, #176]              @ 0xb0 
    /*0000cc50*/ STR         R3, [R4, #268]              @ 0x10c 
    /*0000cc54*/ LDR         R3, [R5, #180]              @ 0xb4 
    /*0000cc58*/ STR         R3, [R4, #272]              @ 0x110 
    /*0000cc5c*/ LDR         R3, [R5, #184]              @ 0xb8 
    /*0000cc60*/ STR         R3, [R4, #276]              @ 0x114 
    /*0000cc64*/ LDR         R3, [R5, #188]              @ 0xbc 
    /*0000cc68*/ STRB        R3, [R4, #280]              @ 0x118 
    /*0000cc6c*/ BL          0x000000000000CC6C          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000cc70*/ LDR         R3, [R5, #200]              @ 0xc8 
    /*0000cc74*/ ADD         R1, R5, #284                @ 0x11c 
    /*0000cc78*/ MOV         R2, #28 
    /*0000cc7c*/ ADD         R0, R4, #388                @ 0x184 
    /*0000cc80*/ STR         R3, [R4, #304]              @ 0x130 
    /*0000cc84*/ LDR         R3, [R5, #196]              @ 0xc4 
    /*0000cc88*/ STR         R3, [R4, #308]              @ 0x134 
    /*0000cc8c*/ LDR         R3, [R5, #204]              @ 0xcc 
    /*0000cc90*/ STR         R3, [R4, #312]              @ 0x138 
    /*0000cc94*/ LDR         R3, [R5, #208]              @ 0xd0 
    /*0000cc98*/ STR         R3, [R4, #316]              @ 0x13c 
    /*0000cc9c*/ LDR         R3, [R5, #212]              @ 0xd4 
    /*0000cca0*/ STR         R3, [R4, #320]              @ 0x140 
    /*0000cca4*/ LDR         R3, [R5, #216]              @ 0xd8 
    /*0000cca8*/ STR         R3, [R4, #324]              @ 0x144 
    /*0000ccac*/ LDR         R3, [R5, #220]              @ 0xdc 
    /*0000ccb0*/ STR         R3, [R4, #328]              @ 0x148 
    /*0000ccb4*/ LDR         R3, [R5, #224]              @ 0xe0 
    /*0000ccb8*/ STR         R3, [R4, #332]              @ 0x14c 
    /*0000ccbc*/ LDR         R3, [R5, #228]              @ 0xe4 
    /*0000ccc0*/ STR         R3, [R4, #336]              @ 0x150 
    /*0000ccc4*/ LDR         R3, [R5, #232]              @ 0xe8 
    /*0000ccc8*/ STR         R3, [R4, #340]              @ 0x154 
    /*0000cccc*/ LDR         R3, [R5, #236]              @ 0xec 
    /*0000ccd0*/ STR         R3, [R4, #344]              @ 0x158 
    /*0000ccd4*/ LDR         R3, [R5, #240]              @ 0xf0 
    /*0000ccd8*/ STRB        R3, [R4, #348]              @ 0x15c 
    /*0000ccdc*/ LDR         R3, [R5, #256]              @ 0x100 
    /*0000cce0*/ STR         R3, [R4, #224]              @ 0xe0 
    /*0000cce4*/ LDR         R3, [R5, #244]              @ 0xf4 
    /*0000cce8*/ STR         R3, [R4, #352]              @ 0x160 
    /*0000ccec*/ LDR         R3, [R5, #252]              @ 0xfc 
    /*0000ccf0*/ STR         R3, [R4, #360]              @ 0x168 
    /*0000ccf4*/ LDR         R3, [R5, #248]              @ 0xf8 
    /*0000ccf8*/ STR         R3, [R4, #356]              @ 0x164 
    /*0000ccfc*/ LDR         R3, [R5, #256]              @ 0x100 
    /*0000cd00*/ STR         R3, [R4, #224]              @ 0xe0 
    /*0000cd04*/ LDR         R3, [R5, #260]              @ 0x104 
    /*0000cd08*/ STR         R3, [R4, #364]              @ 0x16c 
    /*0000cd0c*/ LDR         R3, [R5, #264]              @ 0x108 
    /*0000cd10*/ STR         R3, [R4, #368]              @ 0x170 
    /*0000cd14*/ LDR         R3, [R5, #268]              @ 0x10c 
    /*0000cd18*/ STR         R3, [R4, #372]              @ 0x174 
    /*0000cd1c*/ LDR         R3, [R5, #272]              @ 0x110 
    /*0000cd20*/ STR         R3, [R4, #376]              @ 0x178 
    /*0000cd24*/ LDR         R3, [R5, #276]              @ 0x114 
    /*0000cd28*/ STR         R3, [R4, #380]              @ 0x17c 
    /*0000cd2c*/ LDR         R3, [R5, #280]              @ 0x118 
    /*0000cd30*/ STR         R3, [R4, #384]              @ 0x180 
    /*0000cd34*/ BL          0x000000000000CD34          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000cd38*/ LDR         R2, [R4, #52]               @ 0x34 
    /*0000cd3c*/ LDR         R3, [R4, #64]               @ 0x40 
    /*0000cd40*/ LDR         R1, [R4, #48]               @ 0x30 
    /*0000cd44*/ STR         R2, [R4, #288]              @ 0x120 
    /*0000cd48*/ STR         R3, [R4, #292]              @ 0x124 
    /*0000cd4c*/ STR         R1, [R4, #284]              @ 0x11c 
    /*0000cd50*/ LDR         R3, [R5, #324]              @ 0x144 
    /*0000cd54*/ LDR         R2, [R4, #20] 
    /*0000cd58*/ STRB        R3, [R4, #297]              @ 0x129 
    /*0000cd5c*/ LDR         R3, [R5, #328]              @ 0x148 
    /*0000cd60*/ STRB        R2, [R4, #296]              @ 0x128 
    /*0000cd64*/ STRB        R3, [R4, #298]              @ 0x12a 
    /*0000cd68*/ LDR         R3, [R5, #340]              @ 0x154 
    /*0000cd6c*/ STR         R3, [R4, #88]               @ 0x58 
    /*0000cd70*/ LDR         R3, [R5, #340]              @ 0x154 
    /*0000cd74*/ STR         R3, [R4, #428]              @ 0x1ac 
    /*0000cd78*/ LDR         R3, [R5, #332]              @ 0x14c 
    /*0000cd7c*/ STR         R3, [R4, #420]              @ 0x1a4 
    /*0000cd80*/ LDR         R3, [R5, #336]              @ 0x150 
    /*0000cd84*/ STR         R3, [R4, #424]              @ 0x1a8 
    /*0000cd88*/ LDR         R3, [R5, #348]              @ 0x15c 
    /*0000cd8c*/ STR         R3, [R4, #416]              @ 0x1a0 
    /*0000cd90*/ LDR         R3, [R5, #192]              @ 0xc0 
    /*0000cd94*/ STR         R3, [R4, #300]              @ 0x12c 
    /*0000cd98*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END hiisp_dis_CalcFirmwareAttr


FUNC_BEGIN hiisp_dis_DSF_cfg_parse
    /*0000cd9c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000cda0*/ MOV         R5, R0 
    /*0000cda4*/ LDR         R3, [R0, #28] 
    /*0000cda8*/ SUB         SP, SP, #20 
    /*0000cdac*/ MOV         R4, R1 
    /*0000cdb0*/ LDR         R9, [R0, #36]               @ 0x24 
    /*0000cdb4*/ CMP         R3, #0 
    /*0000cdb8*/ LDR         R8, [R0, #40]               @ 0x28 
    /*0000cdbc*/ LDR         R7, [R0, #44]               @ 0x2c 
    /*0000cdc0*/ LDRB        R6, [R0, #112]              @ 0x70 
    /*0000cdc4*/ BNE         hiisp_dis_DSF_cfg_parse_xce84
    /*0000cdc8*/ LDR         R2, [R0, #108]              @ 0x6c 
    /*0000cdcc*/ STR         R2, [R1] 
    /*0000cdd0*/ LDRB        R2, [R0, #112]              @ 0x70 
    /*0000cdd4*/ STRB        R2, [R1, #4] 
    /*0000cdd8*/ LDR         R2, [R0, #8] 
    /*0000cddc*/ STR         R2, [R1, #8] 
    /*0000cde0*/ LDR         R2, [R0, #20] 
    /*0000cde4*/ CMP         R2, #10 
    /*0000cde8*/ MOV         R2, #0 
    /*0000cdec*/ LDREQ       R3, [R0, #24] 
    /*0000cdf0*/ STR         R2, [R1, #32] 
    /*0000cdf4*/ SUBEQ       R3, R3, #8 
    /*0000cdf8*/ CLZEQ       R3, R3 
    /*0000cdfc*/ LSREQ       R3, R3, #5 
    /*0000ce00*/ STR         R3, [R1, #12] 
    /*0000ce04*/ LDR         R3, [R0, #128]              @ 0x80 
    /*0000ce08*/ STR         R2, [R1, #16] 
    /*0000ce0c*/ SUB         R3, R3, #1 
    /*0000ce10*/ STR         R3, [R1, #36]               @ 0x24 
    /*0000ce14*/ LDR         SL, [R0, #48]               @ 0x30 
    /*0000ce18*/ MUL         R3, R6, R3 
    /*0000ce1c*/ SUB         SL, SL, #1 
    /*0000ce20*/ STR         R2, [R1, #24] 
    /*0000ce24*/ ADD         R3, R3, #1 
    /*0000ce28*/ CMP         SL, R3 
    /*0000ce2c*/ MOVCS       SL, R3 
    /*0000ce30*/ STR         SL, [R1, #20] 
    /*0000ce34*/ LDR         R0, [R0, #52]               @ 0x34 
    /*0000ce38*/ CMP         R6, R2 
    /*0000ce3c*/ STR         R2, [R1, #40]               @ 0x28 
    /*0000ce40*/ SUB         R0, R0, #1 
    /*0000ce44*/ STR         R0, [R1, #28] 
    /*0000ce48*/ LDR         R3, [R5, #132]              @ 0x84 
    /*0000ce4c*/ MOVNE       FP, R6 
    /*0000ce50*/ MOVEQ       FP, #1 
    /*0000ce54*/ SUB         R3, R3, #1 
    /*0000ce58*/ MOV         R1, FP 
    /*0000ce5c*/ STR         R3, [R4, #44]               @ 0x2c 
    /*0000ce60*/ BL          0x000000000000CE60          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000ce64*/ MOV         R1, FP 
    /*0000ce68*/ ADD         R3, R0, #1 
    /*0000ce6c*/ MOV         R0, SL 
    /*0000ce70*/ STR         R3, [R4, #52]               @ 0x34 
    /*0000ce74*/ BL          0x000000000000CE74          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000ce78*/ ADD         R0, R0, #1 
    /*0000ce7c*/ STR         R0, [R4, #48]               @ 0x30 
    /*0000ce80*/ LDR         R3, [R5, #28] 
    hiisp_dis_DSF_cfg_parse_xce84:
    /*0000ce84*/ CMP         R3, #1 
    /*0000ce88*/ BEQ         hiisp_dis_DSF_cfg_parse_xce94
    hiisp_dis_DSF_cfg_parse_xce8c:
    /*0000ce8c*/ ADD         SP, SP, #20 
    /*0000ce90*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    hiisp_dis_DSF_cfg_parse_xce94:
    /*0000ce94*/ LDR         R3, [R5, #108]              @ 0x6c 
    /*0000ce98*/ SUB         R0, R9, #1 
    /*0000ce9c*/ ADD         R0, R0, R6 
    /*0000cea0*/ STR         R3, [R4] 
    /*0000cea4*/ LDRB        R3, [R5, #112]              @ 0x70 
    /*0000cea8*/ STRB        R3, [R4, #4] 
    /*0000ceac*/ LDR         R3, [R5, #8] 
    /*0000ceb0*/ STR         R3, [R4, #8] 
    /*0000ceb4*/ LDR         R3, [R5, #20] 
    /*0000ceb8*/ CMP         R3, #10 
    /*0000cebc*/ LDREQ       R3, [R5, #24] 
    /*0000cec0*/ MOVNE       R3, #0 
    /*0000cec4*/ SUBEQ       R3, R3, #8 
    /*0000cec8*/ CLZEQ       R3, R3 
    /*0000cecc*/ LSREQ       R3, R3, #5 
    /*0000ced0*/ STR         R3, [R4, #12] 
    /*0000ced4*/ LDR         R3, [R5, #108]              @ 0x6c 
    /*0000ced8*/ STR         R3, [R4, #56]               @ 0x38 
    /*0000cedc*/ LDRB        R3, [R5, #112]              @ 0x70 
    /*0000cee0*/ STRB        R3, [R4, #60]               @ 0x3c 
    /*0000cee4*/ LDR         R3, [R5, #8] 
    /*0000cee8*/ STR         R3, [R4, #64]               @ 0x40 
    /*0000ceec*/ LDR         R3, [R5, #20] 
    /*0000cef0*/ CMP         R3, #10 
    /*0000cef4*/ LDREQ       R3, [R5, #24] 
    /*0000cef8*/ MOVNE       R3, #0 
    /*0000cefc*/ SUBEQ       R3, R3, #8 
    /*0000cf00*/ CLZEQ       R3, R3 
    /*0000cf04*/ LSREQ       R3, R3, #5 
    /*0000cf08*/ CMP         R6, #0 
    /*0000cf0c*/ STR         R3, [R4, #68]               @ 0x44 
    /*0000cf10*/ MOVNE       SL, R6 
    /*0000cf14*/ MOVEQ       SL, #1 
    /*0000cf18*/ MOV         R1, SL 
    /*0000cf1c*/ BL          0x000000000000CF1C          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000cf20*/ MOV         R3, #0 
    /*0000cf24*/ STR         R3, [R4, #16] 
    /*0000cf28*/ STR         R3, [R4, #32] 
    /*0000cf2c*/ STR         R3, [R4, #24] 
    /*0000cf30*/ ADD         R0, R0, #7 
    /*0000cf34*/ BIC         FP, R0, #7 
    /*0000cf38*/ SUB         R9, FP, #1 
    /*0000cf3c*/ STR         R9, [R4, #36]               @ 0x24 
    /*0000cf40*/ MUL         R9, R9, R6 
    /*0000cf44*/ ADD         R2, R9, #1 
    /*0000cf48*/ CMP         R8, R2 
    /*0000cf4c*/ STR         R9, [R4, #20] 
    /*0000cf50*/ LDR         R0, [R5, #52]               @ 0x34 
    /*0000cf54*/ STR         R3, [R4, #40]               @ 0x28 
    /*0000cf58*/ SUB         R0, R0, #1 
    /*0000cf5c*/ STR         R0, [R4, #28] 
    /*0000cf60*/ LDR         R3, [R5, #132]              @ 0x84 
    /*0000cf64*/ STR         R2, [R4, #20] 
    /*0000cf68*/ SUB         R3, R3, #1 
    /*0000cf6c*/ STR         R3, [R4, #44]               @ 0x2c 
    /*0000cf70*/ BCC         hiisp_dis_DSF_cfg_parse_xd080
    /*0000cf74*/ MOV         R1, SL 
    /*0000cf78*/ STR         R2, [SP, #12] 
    /*0000cf7c*/ BL          0x000000000000CF7C          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000cf80*/ LDR         R2, [SP, #12] 
    /*0000cf84*/ MOV         R1, SL 
    /*0000cf88*/ ADD         R3, R0, #1 
    /*0000cf8c*/ MOV         R0, R2 
    /*0000cf90*/ STR         R3, [R4, #52]               @ 0x34 
    /*0000cf94*/ BL          0x000000000000CF94          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000cf98*/ ADD         R0, R0, #1 
    /*0000cf9c*/ STR         R0, [R4, #48]               @ 0x30 
    /*0000cfa0*/ LDR         R0, [R5, #128]              @ 0x80 
    /*0000cfa4*/ CMP         FP, R0 
    /*0000cfa8*/ BCS         hiisp_dis_DSF_cfg_parse_xd058
    hiisp_dis_DSF_cfg_parse_xcfac:
    /*0000cfac*/ SUB         R0, R0, FP 
    /*0000cfb0*/ ANDS        R3, R0, #1 
    /*0000cfb4*/ BNE         hiisp_dis_DSF_cfg_parse_xd0ac
    /*0000cfb8*/ MLA         R2, R0, R6, R6 
    /*0000cfbc*/ STR         R3, [R4, #80]               @ 0x50 
    /*0000cfc0*/ SUB         R9, R9, R6 
    /*0000cfc4*/ SUB         R0, R0, #1 
    /*0000cfc8*/ STR         R9, [R4, #72]               @ 0x48 
    /*0000cfcc*/ MOV         R1, #2 
    /*0000cfd0*/ STR         R0, [R4, #92]               @ 0x5c 
    /*0000cfd4*/ STR         R1, [R4, #88]               @ 0x58 
    /*0000cfd8*/ STR         R2, [R4, #76]               @ 0x4c 
    /*0000cfdc*/ LDR         R0, [R5, #52]               @ 0x34 
    /*0000cfe0*/ STR         R3, [R4, #96]               @ 0x60 
    /*0000cfe4*/ SUB         R0, R0, #1 
    /*0000cfe8*/ STR         R0, [R4, #84]               @ 0x54 
    /*0000cfec*/ LDR         R3, [R5, #132]              @ 0x84 
    /*0000cff0*/ SUB         R3, R3, #1 
    /*0000cff4*/ STR         R3, [R4, #100]              @ 0x64 
    /*0000cff8*/ LDR         R1, [R5, #128]              @ 0x80 
    /*0000cffc*/ LDR         R3, [R5, #48]               @ 0x30 
    /*0000d000*/ SUB         R1, R1, #1 
    /*0000d004*/ SUB         R3, R3, #1 
    /*0000d008*/ MUL         R6, R6, R1 
    /*0000d00c*/ ADD         R6, R6, #1 
    /*0000d010*/ CMP         R6, R3 
    /*0000d014*/ ADDLS       R2, R2, #1 
    /*0000d018*/ STRLS       R2, [R4, #76]               @ 0x4c 
    /*0000d01c*/ CMP         R7, R9 
    /*0000d020*/ BHI         hiisp_dis_DSF_cfg_parse_xd0d8
    /*0000d024*/ MOV         R1, SL 
    /*0000d028*/ SUB         R9, R9, R7 
    /*0000d02c*/ STR         R9, [R4, #72]               @ 0x48 
    /*0000d030*/ BL          0x000000000000D030          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000d034*/ MOV         R1, SL 
    /*0000d038*/ ADD         R3, R0, #1 
    /*0000d03c*/ LDR         R0, [R4, #76]               @ 0x4c 
    /*0000d040*/ STR         R3, [R4, #108]              @ 0x6c 
    /*0000d044*/ BL          0x000000000000D044          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000d048*/ ADD         R0, R0, #1 
    /*0000d04c*/ STR         R0, [R4, #104]              @ 0x68 
    /*0000d050*/ ADD         SP, SP, #20 
    /*0000d054*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    hiisp_dis_DSF_cfg_parse_xd058:
    /*0000d058*/ MOVW        R3, #431                    @ 0x1af 
    /*0000d05c*/ MOVW        R2, #0                      @ .LC4 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2720] 
    /*0000d060*/ STR         R3, [SP] 
    /*0000d064*/ MOVT        R2, #0                      @ .LC4 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2720] 
    /*0000d068*/ LDR         R3, [PC, #148]              @ 0x000000000000d104 
    /*0000d06c*/ MOV         R1, #17 
    /*0000d070*/ MOV         R0, #3 
    /*0000d074*/ BL          0x000000000000D074          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000d078*/ LDR         R0, [R5, #128]              @ 0x80 
    /*0000d07c*/ B           hiisp_dis_DSF_cfg_parse_xcfac
    hiisp_dis_DSF_cfg_parse_xd080:
    /*0000d080*/ MOVW        R3, #0                      @ .LC3 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x26e4] 
    /*0000d084*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2618] 
    /*0000d088*/ MOVT        R3, #0                      @ .LC3 [R_ARM_MOVT_ABS: .rodata.str1.4+0x26e4] 
    /*0000d08c*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2618] 
    /*0000d090*/ STR         R3, [SP] 
    /*0000d094*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x268c] 
    /*0000d098*/ MOV         R3, #424                    @ 0x1a8 
    /*0000d09c*/ LDR         R2, [PC, #96]               @ 0x000000000000d104 
    /*0000d0a0*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x268c] 
    /*0000d0a4*/ BL          0x000000000000D0A4          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000d0a8*/ B           hiisp_dis_DSF_cfg_parse_xce8c
    hiisp_dis_DSF_cfg_parse_xd0ac:
    /*0000d0ac*/ MOVW        R3, #0                      @ .LC5 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x27a8] 
    /*0000d0b0*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2618] 
    /*0000d0b4*/ MOVT        R3, #0                      @ .LC5 [R_ARM_MOVT_ABS: .rodata.str1.4+0x27a8] 
    /*0000d0b8*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2618] 
    /*0000d0bc*/ STR         R3, [SP] 
    /*0000d0c0*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x268c] 
    /*0000d0c4*/ MOVW        R3, #434                    @ 0x1b2 
    /*0000d0c8*/ LDR         R2, [PC, #52]               @ 0x000000000000d104 
    /*0000d0cc*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x268c] 
    /*0000d0d0*/ BL          0x000000000000D0D0          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000d0d4*/ B           hiisp_dis_DSF_cfg_parse_xce8c
    hiisp_dis_DSF_cfg_parse_xd0d8:
    /*0000d0d8*/ MOVW        R3, #0                      @ .LC6 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x27c8] 
    /*0000d0dc*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2618] 
    /*0000d0e0*/ MOVT        R3, #0                      @ .LC6 [R_ARM_MOVT_ABS: .rodata.str1.4+0x27c8] 
    /*0000d0e4*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2618] 
    /*0000d0e8*/ STR         R3, [SP] 
    /*0000d0ec*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x268c] 
    /*0000d0f0*/ MOV         R3, #452                    @ 0x1c4 
    /*0000d0f4*/ LDR         R2, [PC, #8]                @ 0x000000000000d104 
    /*0000d0f8*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x268c] 
    /*0000d0fc*/ BL          0x000000000000D0FC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000d100*/ B           hiisp_dis_DSF_cfg_parse_xce8c
    /*0000d104*/ ANDEQ       R0, R0, IP, ASR #11 
FUNC_END hiisp_dis_DSF_cfg_parse


FUNC_BEGIN hiisp_dis_FPD_cfg_parse
    /*0000d108*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000d10c*/ MOV         R3, #1 
    /*0000d110*/ LDRH        LR, [R0, #102]              @ 0x66 
    /*0000d114*/ MOV         R2, #4 
    /*0000d118*/ LDRH        R5, [R0, #96]               @ 0x60 
    /*0000d11c*/ MOV         R4, #16 
    /*0000d120*/ LDRH        R6, [R0, #98]               @ 0x62 
    /*0000d124*/ SUB         LR, LR, #4 
    /*0000d128*/ LDRH        IP, [R0, #100]              @ 0x64 
    /*0000d12c*/ MOV         R8, #5 
    /*0000d130*/ STR         R3, [R1, #4] 
    /*0000d134*/ UXTH        LR, LR 
    /*0000d138*/ LDR         R3, [R0, #24] 
    /*0000d13c*/ STRH        R4, [R1, #12] 
    /*0000d140*/ ADD         R4, R1, #32 
    /*0000d144*/ STRH        R2, [R1, #8] 
    /*0000d148*/ STR         R3, [R1] 
    /*0000d14c*/ ADD         R3, R1, #22 
    /*0000d150*/ STRH        R2, [R1, #10] 
    /*0000d154*/ LDRB        R2, [R0, #122]              @ 0x7a 
    /*0000d158*/ STRB        R2, [R1, #14] 
    /*0000d15c*/ MOV         R2, #0 
    /*0000d160*/ LDRH        R7, [R0, #124]              @ 0x7c 
    /*0000d164*/ STRH        R7, [R1, #16] 
    /*0000d168*/ LDR         R7, [R0, #116]              @ 0x74 
    /*0000d16c*/ STRB        R8, [R1, #18] 
    /*0000d170*/ STRH        R7, [R1, #20] 
    /*0000d174*/ LDRH        R0, [R0, #120]              @ 0x78 
    /*0000d178*/ STRH        R0, [R1, #74]               @ 0x4a 
    hiisp_dis_FPD_cfg_parse_xd17c:
    /*0000d17c*/ ADD         R0, R6, R2, ASR #2 
    /*0000d180*/ STRH        R0, [R3], #2 
    /*0000d184*/ CMP         R3, R4 
    /*0000d188*/ ADD         R2, R2, LR 
    /*0000d18c*/ BNE         hiisp_dis_FPD_cfg_parse_xd17c
    /*0000d190*/ SUB         IP, IP, #4 
    /*0000d194*/ ADD         LR, R1, #42                 @ 0x2a 
    /*0000d198*/ MOV         R2, #0 
    /*0000d19c*/ UXTH        IP, IP 
    hiisp_dis_FPD_cfg_parse_xd1a0:
    /*0000d1a0*/ ADD         R0, R5, R2, ASR #2 
    /*0000d1a4*/ STRH        R0, [R3], #2 
    /*0000d1a8*/ CMP         LR, R3 
    /*0000d1ac*/ ADD         R2, R2, IP 
    /*0000d1b0*/ BNE         hiisp_dis_FPD_cfg_parse_xd1a0
    /*0000d1b4*/ MOV         R3, #0 
    /*0000d1b8*/ STR         R3, [R1, #76]               @ 0x4c 
    /*0000d1bc*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END hiisp_dis_FPD_cfg_parse


FUNC_BEGIN hiisp_dis_LKT_cfg_parse
    /*0000d1c0*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000d1c4*/ MOVW        R2, #409                    @ 0x199 
    /*0000d1c8*/ LDR         LR, [R0, #20] 
    /*0000d1cc*/ MOV         IP, #245                    @ 0xf5 
    /*0000d1d0*/ LDRB        R3, [R0, #136]              @ 0x88 
    /*0000d1d4*/ CMP         LR, #10 
    /*0000d1d8*/ STR         R2, [R1, #12] 
    /*0000d1dc*/ STR         IP, [R1, #8] 
    /*0000d1e0*/ MOVW        R2, #655                    @ 0x28f 
    /*0000d1e4*/ STRB        R3, [R1, #4] 
    /*0000d1e8*/ MOVNE       R2, #40                     @ 0x28 
    /*0000d1ec*/ MOVW        R3, #65500                  @ 0xffdc 
    /*0000d1f0*/ STR         R2, [R1, #16] 
    /*0000d1f4*/ MOVNE       R3, #4000                   @ 0xfa0 
    /*0000d1f8*/ MOV         R2, #8 
    /*0000d1fc*/ STR         R3, [R1, #20] 
    /*0000d200*/ MOV         R3, #10 
    /*0000d204*/ STRB        R2, [R1, #25] 
    /*0000d208*/ STRB        R3, [R1, #24] 
    /*0000d20c*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
FUNC_END hiisp_dis_LKT_cfg_parse


FUNC_BEGIN hiisp_dis_GME_cfg_parse
    /*0000d210*/ PUSH        {R4, LR} 
    /*0000d214*/ MOV         R4, R1 
    /*0000d218*/ LDR         IP, [R0, #20] 
    /*0000d21c*/ MOV         R1, #245                    @ 0xf5 
    /*0000d220*/ LDRB        R3, [R0, #136]              @ 0x88 
    /*0000d224*/ MOVW        R2, #409                    @ 0x199 
    /*0000d228*/ CMP         IP, #10 
    /*0000d22c*/ STR         R1, [R4, #136]              @ 0x88 
    /*0000d230*/ STR         R2, [R4, #140]              @ 0x8c 
    /*0000d234*/ MOV         R1, #8 
    /*0000d238*/ STRB        R3, [R4, #132]              @ 0x84 
    /*0000d23c*/ MOVW        R2, #655                    @ 0x28f 
    /*0000d240*/ STRB        R1, [R4, #153]              @ 0x99 
    /*0000d244*/ MOVNE       R2, #40                     @ 0x28 
    /*0000d248*/ MOVW        R3, #65500                  @ 0xffdc 
    /*0000d24c*/ STR         R2, [R4, #144]              @ 0x90 
    /*0000d250*/ MOVNE       R3, #4000                   @ 0xfa0 
    /*0000d254*/ MOV         R2, #10 
    /*0000d258*/ STR         R3, [R4, #148]              @ 0x94 
    /*0000d25c*/ MOV         R3, #0 
    /*0000d260*/ STRB        R2, [R4, #152]              @ 0x98 
    /*0000d264*/ MOV         R2, #1 
    /*0000d268*/ STR         R3, [R4] 
    /*0000d26c*/ STR         R2, [R4, #4] 
    /*0000d270*/ LDR         R3, [R0, #4] 
    /*0000d274*/ STR         R3, [R4, #8] 
    /*0000d278*/ LDR         R3, [R0, #48]               @ 0x30 
    /*0000d27c*/ STR         R3, [R4, #12] 
    /*0000d280*/ LDR         R3, [R0, #52]               @ 0x34 
    /*0000d284*/ STR         R3, [R4, #16] 
    /*0000d288*/ LDR         R3, [R0, #128]              @ 0x80 
    /*0000d28c*/ STR         R3, [R4, #20] 
    /*0000d290*/ LDR         R3, [R0, #132]              @ 0x84 
    /*0000d294*/ STR         R3, [R4, #24] 
    /*0000d298*/ LDR         R3, [R0, #108]              @ 0x6c 
    /*0000d29c*/ CMP         R3, #0 
    /*0000d2a0*/ STR         R3, [R4, #28] 
    /*0000d2a4*/ LDRH        R1, [R0, #146]              @ 0x92 
    /*0000d2a8*/ STRH        R1, [R4, #40]               @ 0x28 
    /*0000d2ac*/ LDRB        R1, [R0, #152]              @ 0x98 
    /*0000d2b0*/ STRB        R1, [R4, #42]               @ 0x2a 
    /*0000d2b4*/ LDR         R1, [R0, #148]              @ 0x94 
    /*0000d2b8*/ STR         R1, [R4, #44]               @ 0x2c 
    /*0000d2bc*/ LDRH        R1, [R0, #96]               @ 0x60 
    /*0000d2c0*/ STRH        R1, [R4, #48]               @ 0x30 
    /*0000d2c4*/ LDRH        R1, [R0, #98]               @ 0x62 
    /*0000d2c8*/ STRH        R1, [R4, #50]               @ 0x32 
    /*0000d2cc*/ LDRH        R1, [R0, #100]              @ 0x64 
    /*0000d2d0*/ STRH        R1, [R4, #52]               @ 0x34 
    /*0000d2d4*/ LDRH        R1, [R0, #102]              @ 0x66 
    /*0000d2d8*/ STRBEQ      R3, [R4, #32] 
    /*0000d2dc*/ STRH        R1, [R4, #54]               @ 0x36 
    /*0000d2e0*/ BEQ         hiisp_dis_GME_cfg_parse_xd318
    /*0000d2e4*/ STRB        R2, [R4, #32] 
    /*0000d2e8*/ LDRB        R3, [R0, #112]              @ 0x70 
    /*0000d2ec*/ ASR         R3, R3, R2 
    /*0000d2f0*/ CMP         R3, R2 
    /*0000d2f4*/ BEQ         hiisp_dis_GME_cfg_parse_xd318
    /*0000d2f8*/ MOV         R3, #2 
    hiisp_dis_GME_cfg_parse_xd2fc:
    /*0000d2fc*/ STRB        R3, [R4, #32] 
    /*0000d300*/ ADD         R1, R3, #1 
    /*0000d304*/ LDRB        R2, [R0, #112]              @ 0x70 
    /*0000d308*/ ASR         R2, R2, R3 
    /*0000d30c*/ UXTB        R3, R1 
    /*0000d310*/ CMP         R2, #1 
    /*0000d314*/ BNE         hiisp_dis_GME_cfg_parse_xd2fc
    hiisp_dis_GME_cfg_parse_xd318:
    /*0000d318*/ ADD         R1, R0, #156                @ 0x9c 
    /*0000d31c*/ MOV         R2, #60                     @ 0x3c 
    /*0000d320*/ ADD         R0, R4, R2 
    /*0000d324*/ BL          0x000000000000D324          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000d328*/ MOV         IP, #1 
    /*0000d32c*/ MOV         R2, #0 
    /*0000d330*/ ADD         R0, R4, #88                 @ 0x58 
    /*0000d334*/ MOV         R1, IP 
    /*0000d338*/ MOV         R3, R2 
    /*0000d33c*/ STRB        IP, [R4, #88]               @ 0x58 
    /*0000d340*/ STRB        IP, [R4, #93]               @ 0x5d 
    /*0000d344*/ STRB        IP, [R4, #94]               @ 0x5e 
    /*0000d348*/ STRB        IP, [R4, #97]               @ 0x61 
    /*0000d34c*/ STRB        IP, [R4, #99]               @ 0x63 
    /*0000d350*/ STRB        IP, [R4, #103]              @ 0x67 
    /*0000d354*/ STRB        IP, [R4, #106]              @ 0x6a 
    /*0000d358*/ STRB        IP, [R4, #107]              @ 0x6b 
    /*0000d35c*/ STRB        IP, [R4, #108]              @ 0x6c 
    /*0000d360*/ STRB        IP, [R4, #110]              @ 0x6e 
    /*0000d364*/ STRB        IP, [R4, #116]              @ 0x74 
    /*0000d368*/ STRB        IP, [R4, #117]              @ 0x75 
    /*0000d36c*/ B           hiisp_dis_GME_cfg_parse_xd374
    hiisp_dis_GME_cfg_parse_xd370:
    /*0000d370*/ LDRB        R1, [R0, #1]! 
    hiisp_dis_GME_cfg_parse_xd374:
    /*0000d374*/ ADD         R2, R2, R1, LSL R3 
    /*0000d378*/ ADD         R3, R3, #1 
    /*0000d37c*/ CMP         R3, #31 
    /*0000d380*/ BNE         hiisp_dis_GME_cfg_parse_xd370
    /*0000d384*/ LDRH        R3, [R4, #12] 
    /*0000d388*/ MOVW        R0, #43691                  @ 0xaaab 
    /*0000d38c*/ MOVT        R0, #43690                  @ 0xaaaa 
    /*0000d390*/ STR         R2, [R4, #56]               @ 0x38 
    /*0000d394*/ LDRH        R1, [R4, #16] 
    /*0000d398*/ MOV         IP, #8 
    /*0000d39c*/ UMULL       R2, R3, R3, R0 
    /*0000d3a0*/ STRB        IP, [R4, #120]              @ 0x78 
    /*0000d3a4*/ LSR         R2, R1, #1 
    /*0000d3a8*/ MOV         R1, #16 
    /*0000d3ac*/ STRH        R2, [R4, #124]              @ 0x7c 
    /*0000d3b0*/ STRB        R1, [R4, #121]              @ 0x79 
    /*0000d3b4*/ LSR         R3, R3, #1 
    /*0000d3b8*/ STRH        R3, [R4, #122]              @ 0x7a 
    /*0000d3bc*/ POP         {R4, PC} 
FUNC_END hiisp_dis_GME_cfg_parse


FUNC_BEGIN hiisp_dis_LDC_cfg_parse
    /*0000d3c0*/ CMP         R1, #600                    @ 0x258 
    /*0000d3c4*/ PUSH        {R4, R5, R6, R7, LR} 
    /*0000d3c8*/ MOVCS       R1, #600                    @ 0x258 
    /*0000d3cc*/ CMP         R1, #300                    @ 0x12c 
    /*0000d3d0*/ MOV         R4, R3 
    /*0000d3d4*/ MOV         R3, #1 
    /*0000d3d8*/ MOV         R5, R0 
    /*0000d3dc*/ STRB        R2, [R4, #44]               @ 0x2c 
    /*0000d3e0*/ LSL         R2, R3, R2 
    /*0000d3e4*/ STR         R2, [R4, #36]               @ 0x24 
    /*0000d3e8*/ SUB         SP, SP, #12 
    /*0000d3ec*/ STR         R2, [R4, #40]               @ 0x28 
    /*0000d3f0*/ LDR         R2, [R0] 
    /*0000d3f4*/ MOVCS       R0, #0 
    /*0000d3f8*/ MOVCC       R0, #1 
    /*0000d3fc*/ CMP         R1, #300                    @ 0x12c 
    /*0000d400*/ SUB         R1, R1, #300                @ 0x12c 
    /*0000d404*/ MVNCC       R3, #0 
    /*0000d408*/ STR         R2, [R4, #12] 
    /*0000d40c*/ MUL         R1, R3, R1 
    /*0000d410*/ LDR         R2, [R5, #4] 
    /*0000d414*/ STR         R0, [R4] 
    /*0000d418*/ MOVW        R0, #19923                  @ 0x4dd3 
    /*0000d41c*/ MOVT        R0, #4194                   @ 0x1062 
    /*0000d420*/ STR         R2, [R4, #16] 
    /*0000d424*/ LSL         R6, R1, #10 
    /*0000d428*/ LSL         R1, R1, #18 
    /*0000d42c*/ UMULL       R6, R7, R6, R0 
    /*0000d430*/ UMULL       R0, R1, R1, R0 
    /*0000d434*/ LSR         R2, R7, #6 
    /*0000d438*/ MUL         R3, R3, R2 
    /*0000d43c*/ LSR         R0, R1, #6 
    /*0000d440*/ CMP         R3, #1024                   @ 0x400 
    /*0000d444*/ RSBNE       R1, R3, #1024               @ 0x400 
    /*0000d448*/ MOVEQ       R1, #1 
    /*0000d44c*/ BL          0x000000000000D44C          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000d450*/ MOV         R3, #12 
    /*0000d454*/ STRB        R3, [R4, #45]               @ 0x2d 
    /*0000d458*/ STR         R0, [R4, #4] 
    /*0000d45c*/ LDR         R2, [R5] 
    /*0000d460*/ LSR         R2, R2, #1 
    /*0000d464*/ STR         R2, [R4, #28] 
    /*0000d468*/ LDR         R3, [R5, #4] 
    /*0000d46c*/ STR         R2, [R4, #20] 
    /*0000d470*/ LSR         R3, R3, #1 
    /*0000d474*/ STR         R3, [R4, #32] 
    /*0000d478*/ STR         R3, [R4, #24] 
    /*0000d47c*/ LDM         R5, {R1, R2} 
    /*0000d480*/ LSR         R1, R1, #1 
    /*0000d484*/ LSR         R2, R2, #1 
    /*0000d488*/ UMULL       R2, R3, R2, R2 
    /*0000d48c*/ UMLAL       R2, R3, R1, R1 
    /*0000d490*/ CMP         R2, #0 
    /*0000d494*/ BEQ         hiisp_dis_LDC_cfg_parse_xd4b8
    /*0000d498*/ MOV         R1, R2 
    /*0000d49c*/ MOV         R0, #16777216               @ 0x1000000 
    /*0000d4a0*/ BL          0x000000000000D4A0          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000d4a4*/ CMP         R0, #1024                   @ 0x400 
    /*0000d4a8*/ STR         R0, [R4, #8] 
    /*0000d4ac*/ BCS         hiisp_dis_LDC_cfg_parse_xd4c0
    /*0000d4b0*/ ADD         SP, SP, #12 
    /*0000d4b4*/ POP         {R4, R5, R6, R7, PC} 
    hiisp_dis_LDC_cfg_parse_xd4b8:
    /*0000d4b8*/ MOV         R3, #16777216               @ 0x1000000 
    /*0000d4bc*/ STR         R3, [R4, #8] 
    hiisp_dis_LDC_cfg_parse_xd4c0:
    /*0000d4c0*/ LDR         R2, [PC, #40]               @ 0x000000000000d4f0 
    /*0000d4c4*/ MOVW        R3, #0                      @ .LC7 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2808] 
    /*0000d4c8*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2618] 
    /*0000d4cc*/ MOVT        R3, #0                      @ .LC7 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2808] 
    /*0000d4d0*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2618] 
    /*0000d4d4*/ STR         R3, [SP] 
    /*0000d4d8*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x268c] 
    /*0000d4dc*/ MOVW        R3, #730                    @ 0x2da 
    /*0000d4e0*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x268c] 
    /*0000d4e4*/ BL          0x000000000000D4E4          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000d4e8*/ ADD         SP, SP, #12 
    /*0000d4ec*/ POP         {R4, R5, R6, R7, PC} 
    /*0000d4f0*/ ANDEQ       R0, R0, R4, ROR #11 
FUNC_END hiisp_dis_LDC_cfg_parse


FUNC_BEGIN adaptiveAdjustRansacAndMfParam
    /*0000d4f4*/ PUSH        {R4, R5, R6, R7, LR} 
    /*0000d4f8*/ SUB         SP, SP, #12 
    /*0000d4fc*/ LDR         R6, [R0, #92]               @ 0x5c 
    /*0000d500*/ CMP         R6, #0 
    /*0000d504*/ BEQ         adaptiveAdjustRansacAndMfParam_xd584
    /*0000d508*/ LSL         R6, R6, #13 
    /*0000d50c*/ MOVW        R3, #19923                  @ 0x4dd3 
    /*0000d510*/ MOVT        R3, #4194                   @ 0x1062 
    /*0000d514*/ LDR         R0, [R1, #304]              @ 0x130 
    /*0000d518*/ MOV         R5, R2 
    /*0000d51c*/ LDR         R4, [R2, #72]               @ 0x48 
    /*0000d520*/ UMULL       R6, R7, R6, R3 
    /*0000d524*/ MOV         R2, #13 
    /*0000d528*/ ASR         R1, R0, #31 
    /*0000d52c*/ BL          0x000000000000D52C          @ DIS_SignedLeftShift [R_ARM_CALL: .text+0x7568] 
    /*0000d530*/ MUL         R4, R4, R4 
    /*0000d534*/ LSR         R6, R7, #6 
    /*0000d538*/ CMP         R6, #0 
    /*0000d53c*/ MOVNE       R1, R6 
    /*0000d540*/ MOVEQ       R1, #1 
    /*0000d544*/ BL          0x000000000000D544          @ __aeabi_idiv [R_ARM_CALL: +0x0] 
    /*0000d548*/ ASR         R4, R4, #10 
    /*0000d54c*/ MOVW        R3, #65535                  @ 0xffff 
    /*0000d550*/ MOVT        R3, #15 
    /*0000d554*/ CMP         R0, #8192                   @ 0x2000 
    /*0000d558*/ MOVLT       R0, #8192                   @ 0x2000 
    /*0000d55c*/ MUL         R0, R0, R0 
    /*0000d560*/ ASR         R0, R0, #26 
    /*0000d564*/ MUL         R0, R4, R0 
    /*0000d568*/ CMP         R0, R3 
    /*0000d56c*/ MOVGE       R0, R3 
    /*0000d570*/ CMP         R0, R4 
    /*0000d574*/ MOVLT       R0, R4 
    /*0000d578*/ STR         R0, [R5, #76]               @ 0x4c 
    /*0000d57c*/ ADD         SP, SP, #12 
    /*0000d580*/ POP         {R4, R5, R6, R7, PC} 
    adaptiveAdjustRansacAndMfParam_xd584:
    /*0000d584*/ LDR         R2, [PC, #40]               @ 0x000000000000d5b4 
    /*0000d588*/ MOVW        R3, #0                      @ .LC8 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2830] 
    /*0000d58c*/ MOVW        R1, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2618] 
    /*0000d590*/ MOVT        R3, #0                      @ .LC8 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2830] 
    /*0000d594*/ MOVT        R1, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2618] 
    /*0000d598*/ STR         R3, [SP] 
    /*0000d59c*/ MOVW        R0, #0                      @ .LC2 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x268c] 
    /*0000d5a0*/ MOVW        R3, #739                    @ 0x2e3 
    /*0000d5a4*/ MOVT        R0, #0                      @ .LC2 [R_ARM_MOVT_ABS: .rodata.str1.4+0x268c] 
    /*0000d5a8*/ BL          0x000000000000D5A8          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000d5ac*/ ADD         SP, SP, #12 
    /*0000d5b0*/ POP         {R4, R5, R6, R7, PC} 
    /*0000d5b4*/ STRDEQ      R0, [R0], -IP 
FUNC_END adaptiveAdjustRansacAndMfParam


FUNC_BEGIN hiisp_dis_LimitDownSampleFactor
    /*0000d5b8*/ MOV         R3, #360                    @ 0x168 
    /*0000d5bc*/ MUL         R3, R3, R0 
    /*0000d5c0*/ PUSH        {R4, R5, R6, LR} 
    /*0000d5c4*/ SUB         SP, SP, #16 
    /*0000d5c8*/ CMP         R3, R1, LSL #10 
    /*0000d5cc*/ BHI         hiisp_dis_LimitDownSampleFactor_xd650
    /*0000d5d0*/ MOV         R3, #640                    @ 0x280 
    /*0000d5d4*/ MUL         R3, R3, R1 
    /*0000d5d8*/ CMP         R3, R0, LSL #10 
    /*0000d5dc*/ BHI         hiisp_dis_LimitDownSampleFactor_xd6b4
    /*0000d5e0*/ SXTB        R2, R2 
    /*0000d5e4*/ MOV         R3, #9 
    /*0000d5e8*/ MOV         R5, #320                    @ 0x140 
    /*0000d5ec*/ MOV         R6, #180                    @ 0xb4 
    hiisp_dis_LimitDownSampleFactor_xd5f0:
    /*0000d5f0*/ LSL         IP, R2, #10 
    /*0000d5f4*/ ADD         R4, R2, R2, LSR #31 
    /*0000d5f8*/ CMP         R1, IP 
    /*0000d5fc*/ CMPLS       R0, IP 
    /*0000d600*/ LSL         IP, R2, #1 
    /*0000d604*/ MUL         LR, R5, R2 
    /*0000d608*/ SXTBHI      R2, IP 
    /*0000d60c*/ BHI         hiisp_dis_LimitDownSampleFactor_xd628
    /*0000d610*/ CMP         R0, LR 
    /*0000d614*/ MUL         IP, R6, R2 
    /*0000d618*/ BCC         hiisp_dis_LimitDownSampleFactor_xd624
    /*0000d61c*/ CMP         R1, IP 
    /*0000d620*/ BCS         hiisp_dis_LimitDownSampleFactor_xd634
    hiisp_dis_LimitDownSampleFactor_xd624:
    /*0000d624*/ SBFX        R2, R4, #1, #8 
    hiisp_dis_LimitDownSampleFactor_xd628:
    /*0000d628*/ SUB         R3, R3, #1 
    /*0000d62c*/ ANDS        R3, R3, #255                @ 0xff 
    /*0000d630*/ BNE         hiisp_dis_LimitDownSampleFactor_xd5f0
    hiisp_dis_LimitDownSampleFactor_xd634:
    /*0000d634*/ CMP         R2, #8 
    /*0000d638*/ BGT         hiisp_dis_LimitDownSampleFactor_xd684
    /*0000d63c*/ CMP         R2, #0 
    /*0000d640*/ UXTBGT      R0, R2 
    /*0000d644*/ BLE         hiisp_dis_LimitDownSampleFactor_xd6d0
    /*0000d648*/ ADD         SP, SP, #16 
    /*0000d64c*/ POP         {R4, R5, R6, PC} 
    hiisp_dis_LimitDownSampleFactor_xd650:
    /*0000d650*/ MOVW        R2, #0                      @ .LC9 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2858] 
    /*0000d654*/ MOVT        R2, #0                      @ .LC9 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2858] 
    /*0000d658*/ MOVW        R3, #767                    @ 0x2ff 
    /*0000d65c*/ STR         R1, [SP, #8] 
    /*0000d660*/ STR         R0, [SP, #4] 
    /*0000d664*/ STR         R3, [SP] 
    hiisp_dis_LimitDownSampleFactor_xd668:
    /*0000d668*/ LDR         R3, [PC, #120]              @ 0x000000000000d6e8 
    /*0000d66c*/ MOV         R1, #17 
    /*0000d670*/ MOV         R0, #3 
    /*0000d674*/ BL          0x000000000000D674          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000d678*/ MOV         R0, #0 
    /*0000d67c*/ ADD         SP, SP, #16 
    /*0000d680*/ POP         {R4, R5, R6, PC} 
    hiisp_dis_LimitDownSampleFactor_xd684:
    /*0000d684*/ STR         R2, [SP, #4] 
    /*0000d688*/ MOVW        R2, #0                      @ .LC11 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2918] 
    /*0000d68c*/ MOVT        R2, #0                      @ .LC11 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2918] 
    /*0000d690*/ MOVW        R3, #801                    @ 0x321 
    /*0000d694*/ STR         R3, [SP] 
    hiisp_dis_LimitDownSampleFactor_xd698:
    /*0000d698*/ LDR         R3, [PC, #72]               @ 0x000000000000d6e8 
    /*0000d69c*/ MOV         R1, #17 
    /*0000d6a0*/ MOV         R0, #3 
    /*0000d6a4*/ BL          0x000000000000D6A4          @ HI_LOG [R_ARM_CALL: +0x0] 
    /*0000d6a8*/ MOV         R0, #0 
    /*0000d6ac*/ ADD         SP, SP, #16 
    /*0000d6b0*/ POP         {R4, R5, R6, PC} 
    hiisp_dis_LimitDownSampleFactor_xd6b4:
    /*0000d6b4*/ MOVW        R3, #775                    @ 0x307 
    /*0000d6b8*/ MOVW        R2, #0                      @ .LC10 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x28b8] 
    /*0000d6bc*/ STR         R1, [SP, #8] 
    /*0000d6c0*/ MOVT        R2, #0                      @ .LC10 [R_ARM_MOVT_ABS: .rodata.str1.4+0x28b8] 
    /*0000d6c4*/ STR         R0, [SP, #4] 
    /*0000d6c8*/ STR         R3, [SP] 
    /*0000d6cc*/ B           hiisp_dis_LimitDownSampleFactor_xd668
    hiisp_dis_LimitDownSampleFactor_xd6d0:
    /*0000d6d0*/ STR         R2, [SP, #4] 
    /*0000d6d4*/ MOVW        R3, #807                    @ 0x327 
    /*0000d6d8*/ MOVW        R2, #0                      @ .LC12 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2984] 
    /*0000d6dc*/ STR         R3, [SP] 
    /*0000d6e0*/ MOVT        R2, #0                      @ .LC12 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2984] 
    /*0000d6e4*/ B           hiisp_dis_LimitDownSampleFactor_xd698
    /*0000d6e8*/ ANDEQ       R0, R0, IP, LSL R6 
FUNC_END hiisp_dis_LimitDownSampleFactor


FUNC_BEGIN hiisp_dis_dynamic_cfg_parse
    /*0000d6ec*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000d6f0*/ MOV         R4, R0 
    /*0000d6f4*/ LDR         R3, [R4, #384]              @ 0x180 
    /*0000d6f8*/ MOVW        R0, #19923                  @ 0x4dd3 
    /*0000d6fc*/ MOVT        R0, #4194                   @ 0x1062 
    /*0000d700*/ LDRH        R7, [R1, #66]               @ 0x42 
    /*0000d704*/ MOV         R5, R1 
    /*0000d708*/ MOV         R6, R2 
    /*0000d70c*/ LSL         R3, R3, #13 
    /*0000d710*/ LDR         R2, [R4, #72]               @ 0x48 
    /*0000d714*/ LSL         R7, R7, #1 
    /*0000d718*/ SMULL       R0, R1, R3, R0 
    /*0000d71c*/ ASR         R3, R3, #31 
    /*0000d720*/ STRB        R2, [R6, #160]              @ 0xa0 
    /*0000d724*/ UXTH        R7, R7 
    /*0000d728*/ RSB         R3, R3, R1, ASR #6 
    /*0000d72c*/ STR         R3, [R6, #268]              @ 0x10c 
    /*0000d730*/ CMP         R3, #0 
    /*0000d734*/ MOVLE       R3, #0 
    /*0000d738*/ MOVGT       R3, #1 
    /*0000d73c*/ STR         R3, [R6, #264]              @ 0x108 
    /*0000d740*/ LDR         R3, [R4, #16] 
    /*0000d744*/ CMP         R3, #0 
    /*0000d748*/ BNE         hiisp_dis_dynamic_cfg_parse_xd75c
    /*0000d74c*/ MOV         R2, R5 
    /*0000d750*/ MOV         R1, R6 
    /*0000d754*/ MOV         R0, R4 
    /*0000d758*/ BL          0x000000000000D758          @ adaptiveAdjustRansacAndMfParam [R_ARM_CALL: .text+0xd4f4] 
    hiisp_dis_dynamic_cfg_parse_xd75c:
    /*0000d75c*/ LDRH        R3, [R5, #66]               @ 0x42 
    /*0000d760*/ MOVW        R1, #2047                   @ 0x7ff 
    /*0000d764*/ CMP         R7, R3 
    /*0000d768*/ MOVLT       R7, R3 
    /*0000d76c*/ STRH        R7, [R5, #68]               @ 0x44 
    /*0000d770*/ LDR         R3, [R4, #216]              @ 0xd8 
    /*0000d774*/ RSB         R3, R3, R3, LSL #11 
    /*0000d778*/ UBFX        R3, R3, #10, #16 
    /*0000d77c*/ STRH        R3, [R5, #122]              @ 0x7a 
    /*0000d780*/ LDR         R2, [R4, #220]              @ 0xdc 
    /*0000d784*/ CMP         R3, R1 
    /*0000d788*/ MOVCS       R3, R1 
    /*0000d78c*/ STRH        R3, [R5, #122]              @ 0x7a 
    /*0000d790*/ RSB         R2, R2, R2, LSL #11 
    /*0000d794*/ UBFX        R3, R2, #10, #16 
    /*0000d798*/ CMP         R3, R1 
    /*0000d79c*/ MOVCS       R3, R1 
    /*0000d7a0*/ STRH        R3, [R5, #124]              @ 0x7c 
    /*0000d7a4*/ LDR         R3, [R4, #436]              @ 0x1b4 
    /*0000d7a8*/ STR         R3, [R6, #856]              @ 0x358 
    /*0000d7ac*/ POP         {R4, R5, R6, R7, R8, PC} 
FUNC_END hiisp_dis_dynamic_cfg_parse


FUNC_BEGIN getSortedIdx
    /*0000d7b0*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000d7b4*/ SUB         SP, SP, #20 
    /*0000d7b8*/ CMP         R2, R3 
    /*0000d7bc*/ STR         R3, [SP, #12] 
    /*0000d7c0*/ LDRB        R3, [R0, R2] 
    /*0000d7c4*/ STR         R2, [SP, #4] 
    /*0000d7c8*/ STR         R3, [SP, #8] 
    /*0000d7cc*/ BGE         getSortedIdx_xd89c
    /*0000d7d0*/ MOV         R6, R0 
    /*0000d7d4*/ MOV         R5, R1 
    getSortedIdx_xd7d8:
    /*0000d7d8*/ LDR         R3, [SP, #12] 
    /*0000d7dc*/ LDR         R2, [SP, #4] 
    /*0000d7e0*/ CMP         R3, R2 
    /*0000d7e4*/ BLE         getSortedIdx_xd90c
    /*0000d7e8*/ LDR         R3, [SP, #12] 
    /*0000d7ec*/ LDR         R1, [SP, #8] 
    /*0000d7f0*/ LDRB        R2, [R6, R3] 
    /*0000d7f4*/ MOV         R7, R3 
    /*0000d7f8*/ ADD         FP, R5, R1, LSL #1 
    /*0000d7fc*/ LDR         R1, [SP, #4] 
    /*0000d800*/ ADD         LR, R6, R1 
    /*0000d804*/ MOV         R4, R1 
    /*0000d808*/ SXTH        R1, R2 
    getSortedIdx_xd80c:
    /*0000d80c*/ LSL         R3, R1, #1 
    /*0000d810*/ LDRH        R9, [FP] 
    /*0000d814*/ ADD         R8, R5, R3 
    /*0000d818*/ LDRH        R3, [R5, R3] 
    /*0000d81c*/ CMP         R3, R9 
    /*0000d820*/ ADDCC       SL, R6, R7 
    /*0000d824*/ BCC         getSortedIdx_xd8a4
    /*0000d828*/ SUB         R3, R7, #1 
    /*0000d82c*/ ADD         R0, R6, R3 
    /*0000d830*/ B           getSortedIdx_xd850
    getSortedIdx_xd834:
    /*0000d834*/ LDRB        R2, [R0], #-1 
    /*0000d838*/ SXTH        R1, R2 
    /*0000d83c*/ LSL         IP, R1, #1 
    /*0000d840*/ ADD         R8, R5, IP 
    /*0000d844*/ LDRH        IP, [R5, IP] 
    /*0000d848*/ CMP         IP, R9 
    /*0000d84c*/ BCC         getSortedIdx_xd8a4
    getSortedIdx_xd850:
    /*0000d850*/ CMP         R3, R4 
    /*0000d854*/ MOV         R7, R3 
    /*0000d858*/ MOV         SL, R0 
    /*0000d85c*/ SUB         R3, R3, #1 
    /*0000d860*/ BNE         getSortedIdx_xd834
    getSortedIdx_xd864:
    /*0000d864*/ LDRB        R3, [SP, #8] 
    /*0000d868*/ MOV         R1, R5 
    /*0000d86c*/ LDR         R2, [SP, #4] 
    /*0000d870*/ MOV         R0, R6 
    /*0000d874*/ STRB        R3, [LR] 
    /*0000d878*/ SUB         R3, R4, #1 
    /*0000d87c*/ BL          0x000000000000D7B0 
    /*0000d880*/ ADD         R3, R4, #1 
    /*0000d884*/ LDR         R2, [SP, #12] 
    /*0000d888*/ STR         R3, [SP, #4] 
    /*0000d88c*/ CMP         R3, R2 
    /*0000d890*/ LDRB        R3, [R6, R3] 
    /*0000d894*/ STR         R3, [SP, #8] 
    /*0000d898*/ BLT         getSortedIdx_xd7d8
    getSortedIdx_xd89c:
    /*0000d89c*/ ADD         SP, SP, #20 
    /*0000d8a0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    getSortedIdx_xd8a4:
    /*0000d8a4*/ STRB        R2, [LR] 
    /*0000d8a8*/ LDRH        R3, [R8] 
    /*0000d8ac*/ LDRH        R0, [FP] 
    /*0000d8b0*/ CMP         R3, R0 
    /*0000d8b4*/ BLS         getSortedIdx_xd8d4
    /*0000d8b8*/ B           getSortedIdx_xd8f4
    getSortedIdx_xd8bc:
    /*0000d8bc*/ LDRB        R2, [LR, #1]! 
    /*0000d8c0*/ SXTH        R1, R2 
    /*0000d8c4*/ LSL         R3, R1, #1 
    /*0000d8c8*/ LDRH        R3, [R5, R3] 
    /*0000d8cc*/ CMP         R3, R0 
    /*0000d8d0*/ BHI         getSortedIdx_xd8f0
    getSortedIdx_xd8d4:
    /*0000d8d4*/ ADD         R4, R4, #1 
    /*0000d8d8*/ CMP         R4, R7 
    /*0000d8dc*/ BLT         getSortedIdx_xd8bc
    /*0000d8e0*/ LDRB        R3, [R6, R4] 
    /*0000d8e4*/ ADD         LR, R6, R4 
    /*0000d8e8*/ STRB        R3, [SL] 
    /*0000d8ec*/ B           getSortedIdx_xd864
    getSortedIdx_xd8f0:
    /*0000d8f0*/ ADD         LR, R6, R4 
    getSortedIdx_xd8f4:
    /*0000d8f4*/ CMP         R7, R4 
    /*0000d8f8*/ STRB        R2, [SL] 
    /*0000d8fc*/ BGT         getSortedIdx_xd80c
    /*0000d900*/ STRB        R2, [LR] 
    /*0000d904*/ STRB        R2, [R6, R7] 
    /*0000d908*/ B           getSortedIdx_xd864
    getSortedIdx_xd90c:
    /*0000d90c*/ LDR         R3, [SP, #4] 
    /*0000d910*/ ADD         LR, R6, R3 
    /*0000d914*/ MOV         R4, R3 
    /*0000d918*/ B           getSortedIdx_xd864
FUNC_END getSortedIdx


FUNC_BEGIN hiisp_dis_mergePtsVecs_fw
    /*0000d91c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000d920*/ SUB         SP, SP, #148                @ 0x94 
    /*0000d924*/ LDR         R3, [R1, #48]               @ 0x30 
    /*0000d928*/ MOV         R4, R1 
    /*0000d92c*/ STR         R0, [SP, #36]               @ 0x24 
    /*0000d930*/ MOV         R2, #34                     @ 0x22 
    /*0000d934*/ MOV         R1, #0 
    /*0000d938*/ ADD         R0, SP, #110                @ 0x6e 
    /*0000d93c*/ MOV         R7, R3 
    /*0000d940*/ STR         R3, [SP, #16] 
    /*0000d944*/ LDRH        R3, [R4, #26] 
    /*0000d948*/ LDR         R6, [R4, #52]               @ 0x34 
    /*0000d94c*/ LDRH        R8, [R4, #12] 
    /*0000d950*/ STR         R3, [SP, #32] 
    /*0000d954*/ LDR         R3, [R4, #56]               @ 0x38 
    /*0000d958*/ LDRB        R5, [R4, #32] 
    /*0000d95c*/ STR         R3, [SP, #20] 
    /*0000d960*/ LDR         R3, [R4, #28] 
    /*0000d964*/ STR         R3, [SP, #4] 
    /*0000d968*/ LDR         R3, [R4, #16] 
    /*0000d96c*/ STR         R3, [SP, #24] 
    /*0000d970*/ LDR         R3, [R4, #20] 
    /*0000d974*/ STR         R3, [SP, #28] 
    /*0000d978*/ BL          0x000000000000D978          @ memset [R_ARM_CALL: +0x0] 
    /*0000d97c*/ ADD         R0, SP, #78                 @ 0x4e 
    /*0000d980*/ MOV         R2, #32 
    /*0000d984*/ MOV         R1, #0 
    /*0000d988*/ BL          0x000000000000D988          @ memset [R_ARM_CALL: +0x0] 
    /*0000d98c*/ LDR         R7, [R7, #4] 
    /*0000d990*/ CMP         R7, #0 
    /*0000d994*/ BEQ         hiisp_dis_mergePtsVecs_fw_xdfcc
    /*0000d998*/ LDR         R3, [R4, #60]               @ 0x3c 
    /*0000d99c*/ CMP         R3, #0 
    /*0000d9a0*/ BEQ         hiisp_dis_mergePtsVecs_fw_xe09c
    /*0000d9a4*/ LDR         R3, [R4, #8] 
    /*0000d9a8*/ CMP         R3, #0 
    /*0000d9ac*/ LDRNE       R3, [SP, #4] 
    /*0000d9b0*/ MOVNE       R7, #0 
    /*0000d9b4*/ SUBNE       R9, R3, #4 
    /*0000d9b8*/ BEQ         hiisp_dis_mergePtsVecs_fw_xd9dc
    hiisp_dis_mergePtsVecs_fw_xd9bc:
    /*0000d9bc*/ LDR         R2, [R4, #4] 
    /*0000d9c0*/ MOV         R1, #0 
    /*0000d9c4*/ LDR         R0, [R9, #4]! 
    /*0000d9c8*/ ADD         R7, R7, #1 
    /*0000d9cc*/ BL          0x000000000000D9CC          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000d9d0*/ LDR         R3, [R4, #8] 
    /*0000d9d4*/ CMP         R3, R7 
    /*0000d9d8*/ BHI         hiisp_dis_mergePtsVecs_fw_xd9bc
    hiisp_dis_mergePtsVecs_fw_xd9dc:
    /*0000d9dc*/ LDR         R7, [R6, #4] 
    /*0000d9e0*/ ADD         R5, R5, #6 
    /*0000d9e4*/ CMP         R8, R7 
    /*0000d9e8*/ UXTB        R5, R5 
    /*0000d9ec*/ BCC         hiisp_dis_mergePtsVecs_fw_xdd98
    /*0000d9f0*/ CMP         R7, #0 
    /*0000d9f4*/ BEQ         hiisp_dis_mergePtsVecs_fw_xdb4c
    /*0000d9f8*/ ADD         R9, SP, #60                 @ 0x3c 
    /*0000d9fc*/ LDR         FP, [SP, #24] 
    /*0000da00*/ STR         R9, [SP, #12] 
    /*0000da04*/ MOV         R8, #0 
    /*0000da08*/ LDR         R9, [SP, #28] 
    /*0000da0c*/ MOV         SL, R6 
    hiisp_dis_mergePtsVecs_fw_xda10:
    /*0000da10*/ MOV         R1, R8 
    /*0000da14*/ LDR         R2, [SP, #12] 
    /*0000da18*/ MOV         R0, SL 
    /*0000da1c*/ ADD         R8, R8, #1 
    /*0000da20*/ BL          0x000000000000DA20          @ DIS_ptsVec_getElement [R_ARM_CALL: .text+0x6984] 
    /*0000da24*/ LDR         R0, [SP, #60]               @ 0x3c 
    /*0000da28*/ MOV         R2, #6 
    /*0000da2c*/ ASR         R1, R0, #31 
    /*0000da30*/ BL          0x000000000000DA30          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000da34*/ MOV         R2, #6 
    /*0000da38*/ MOV         R6, R0 
    /*0000da3c*/ LDR         R0, [SP, #64]               @ 0x40 
    /*0000da40*/ UXTH        R6, R6 
    /*0000da44*/ ASR         R1, R0, #31 
    /*0000da48*/ BL          0x000000000000DA48          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000da4c*/ LDRH        R3, [FP, #2] 
    /*0000da50*/ LDRH        R2, [FP, #6] 
    /*0000da54*/ LDRH        R1, [FP, #4] 
    /*0000da58*/ MOV         R7, R3 
    /*0000da5c*/ LDRH        R3, [R9, #4] 
    /*0000da60*/ STR         R2, [SP, #8] 
    /*0000da64*/ CMP         R3, R6 
    /*0000da68*/ LDRH        R2, [R9, #2] 
    /*0000da6c*/ LDR         LR, [R4, #4] 
    /*0000da70*/ MOVCS       R3, #0 
    /*0000da74*/ MOVCC       R3, #1 
    /*0000da78*/ CMP         R2, R6 
    /*0000da7c*/ ADDCC       R3, R3, #1 
    /*0000da80*/ LDRH        R2, [R9, #6] 
    /*0000da84*/ LDR         IP, [R4, #8] 
    /*0000da88*/ SUB         LR, LR, #1 
    /*0000da8c*/ CMP         R2, R6 
    /*0000da90*/ ADDCC       R3, R3, #1 
    /*0000da94*/ LDR         R6, [SP, #60]               @ 0x3c 
    /*0000da98*/ LSL         LR, LR, R5 
    /*0000da9c*/ SUB         IP, IP, #1 
    /*0000daa0*/ UXTH        R3, R3 
    /*0000daa4*/ MOV         R2, R5 
    /*0000daa8*/ LSL         IP, IP, R5 
    /*0000daac*/ UXTH        R0, R0 
    /*0000dab0*/ CMP         R1, R0 
    /*0000dab4*/ MOVCS       R1, #0 
    /*0000dab8*/ MOVCC       R1, #1 
    /*0000dabc*/ CMP         R7, R0 
    /*0000dac0*/ ADDCC       R1, R1, #1 
    /*0000dac4*/ LDR         R7, [SP, #8] 
    /*0000dac8*/ CMP         R7, R0 
    /*0000dacc*/ ADDCC       R1, R1, #1 
    /*0000dad0*/ LDR         R0, [SP, #64]               @ 0x40 
    /*0000dad4*/ CMP         LR, R6 
    /*0000dad8*/ UXTH        R1, R1 
    /*0000dadc*/ MOVCS       LR, R6 
    /*0000dae0*/ CMP         R0, IP 
    /*0000dae4*/ ADD         R3, R3, R1, LSL #2 
    /*0000dae8*/ STR         LR, [SP, #60]               @ 0x3c 
    /*0000daec*/ MOVCC       IP, R0 
    /*0000daf0*/ STR         IP, [SP, #64]               @ 0x40 
    /*0000daf4*/ LSL         R3, R3, #1 
    /*0000daf8*/ ADD         IP, SP, #78                 @ 0x4e 
    /*0000dafc*/ MOV         R0, LR 
    /*0000db00*/ ASR         R1, LR, #31 
    /*0000db04*/ LDRH        IP, [IP, R3] 
    /*0000db08*/ ADD         LR, SP, #78                 @ 0x4e 
    /*0000db0c*/ ADD         IP, IP, #1 
    /*0000db10*/ STRH        IP, [LR, R3] 
    /*0000db14*/ BL          0x000000000000DB14          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000db18*/ MOV         R2, R5 
    /*0000db1c*/ MOV         R6, R0 
    /*0000db20*/ LDR         R0, [SP, #64]               @ 0x40 
    /*0000db24*/ ASR         R1, R0, #31 
    /*0000db28*/ BL          0x000000000000DB28          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000db2c*/ LDR         R3, [SP, #4] 
    /*0000db30*/ MOV         R2, #1 
    /*0000db34*/ LDR         R3, [R3, R0, LSL #2] 
    /*0000db38*/ STRB        R2, [R3, R6] 
    /*0000db3c*/ LDR         R7, [SL, #4] 
    /*0000db40*/ CMP         R7, R8 
    /*0000db44*/ BHI         hiisp_dis_mergePtsVecs_fw_xda10
    /*0000db48*/ MOV         R6, SL 
    hiisp_dis_mergePtsVecs_fw_xdb4c:
    /*0000db4c*/ LDR         R8, [R4] 
    /*0000db50*/ CMP         R8, #1 
    /*0000db54*/ BEQ         hiisp_dis_mergePtsVecs_fw_xdf14
    hiisp_dis_mergePtsVecs_fw_xdb58:
    /*0000db58*/ CMP         R8, #0 
    /*0000db5c*/ BNE         hiisp_dis_mergePtsVecs_fw_xe0d4
    /*0000db60*/ LDR         R2, [SP, #36]               @ 0x24 
    /*0000db64*/ MOVW        R3, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1c58] 
    /*0000db68*/ MOVT        R3, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1c58] 
    /*0000db6c*/ ADD         R1, SP, #78                 @ 0x4e 
    /*0000db70*/ ADD         R3, R3, R2, LSL #4 
    /*0000db74*/ MOV         R2, R8 
    /*0000db78*/ STR         R3, [SP, #12] 
    /*0000db7c*/ MOV         R0, R3 
    /*0000db80*/ MOV         R3, #15 
    /*0000db84*/ BL          0x000000000000D7B0 
    /*0000db88*/ LDR         R2, [SP, #28] 
    /*0000db8c*/ LDR         R1, [SP, #16] 
    /*0000db90*/ LDRH        FP, [R2, #2] 
    /*0000db94*/ LDR         R2, [SP, #24] 
    /*0000db98*/ LDR         R3, [R1, #4] 
    /*0000db9c*/ LDRH        R2, [R2, #2] 
    /*0000dba0*/ CMP         R3, #0 
    /*0000dba4*/ STR         R3, [SP, #36]               @ 0x24 
    /*0000dba8*/ STR         R2, [SP, #8] 
    /*0000dbac*/ BEQ         hiisp_dis_mergePtsVecs_fw_xe1f8
    /*0000dbb0*/ ADD         R0, SP, #112                @ 0x70 
    /*0000dbb4*/ STR         R5, [SP, #40]               @ 0x28 
    /*0000dbb8*/ STR         R6, [SP, #44]               @ 0x2c 
    /*0000dbbc*/ ADD         R9, SP, #68                 @ 0x44 
    /*0000dbc0*/ STR         R7, [SP, #48]               @ 0x30 
    /*0000dbc4*/ MOV         SL, R8 
    /*0000dbc8*/ STR         R4, [SP, #52]               @ 0x34 
    /*0000dbcc*/ MOV         R5, R3 
    /*0000dbd0*/ MOV         R7, R8 
    /*0000dbd4*/ MOV         R6, R0 
    /*0000dbd8*/ MOV         R4, R1 
    /*0000dbdc*/ B           hiisp_dis_mergePtsVecs_fw_xdc20
    hiisp_dis_mergePtsVecs_fw_xdbe0:
    /*0000dbe0*/ LDR         R2, [SP, #68]               @ 0x44 
    /*0000dbe4*/ CMP         FP, R2, ASR #6 
    /*0000dbe8*/ BGT         hiisp_dis_mergePtsVecs_fw_xdc14
    /*0000dbec*/ STRH        R8, [R6], #2 
    hiisp_dis_mergePtsVecs_fw_xdbf0:
    /*0000dbf0*/ LDR         R2, [SP, #24] 
    /*0000dbf4*/ ADD         R3, SL, #1 
    /*0000dbf8*/ AND         SL, R3, #3 
    /*0000dbfc*/ ADD         R2, R2, R7, LSL #1 
    /*0000dc00*/ LDRH        R3, [R2, #2] 
    /*0000dc04*/ STR         R3, [SP, #8] 
    /*0000dc08*/ LDR         R3, [SP, #28] 
    /*0000dc0c*/ ADD         R2, R3, SL, LSL #1 
    /*0000dc10*/ LDRH        FP, [R2, #2] 
    hiisp_dis_mergePtsVecs_fw_xdc14:
    /*0000dc14*/ ADD         R8, R8, #1 
    /*0000dc18*/ CMP         R5, R8 
    /*0000dc1c*/ BEQ         hiisp_dis_mergePtsVecs_fw_xdc60
    hiisp_dis_mergePtsVecs_fw_xdc20:
    /*0000dc20*/ MOV         R2, R9 
    /*0000dc24*/ MOV         R1, R8 
    /*0000dc28*/ MOV         R0, R4 
    /*0000dc2c*/ BL          0x000000000000DC2C          @ DIS_ptsVec_getElement [R_ARM_CALL: .text+0x6984] 
    /*0000dc30*/ CMP         SL, #3 
    /*0000dc34*/ BNE         hiisp_dis_mergePtsVecs_fw_xdbe0
    /*0000dc38*/ LDR         R2, [SP, #72]               @ 0x48 
    /*0000dc3c*/ ADD         R1, R7, #1 
    /*0000dc40*/ LDR         R3, [SP, #8] 
    /*0000dc44*/ CMP         R3, R2, ASR #6 
    /*0000dc48*/ STRHLE      R8, [R6], #2 
    /*0000dc4c*/ UXTHLE      R7, R1 
    /*0000dc50*/ BLE         hiisp_dis_mergePtsVecs_fw_xdbf0
    /*0000dc54*/ ADD         R8, R8, #1 
    /*0000dc58*/ CMP         R5, R8 
    /*0000dc5c*/ BNE         hiisp_dis_mergePtsVecs_fw_xdc20
    hiisp_dis_mergePtsVecs_fw_xdc60:
    /*0000dc60*/ MOV         R1, R6 
    /*0000dc64*/ LDR         R5, [SP, #40]               @ 0x28 
    /*0000dc68*/ LDR         R7, [SP, #48]               @ 0x30 
    /*0000dc6c*/ LDR         R6, [SP, #44]               @ 0x2c 
    /*0000dc70*/ LDR         R4, [SP, #52]               @ 0x34 
    hiisp_dis_mergePtsVecs_fw_xdc74:
    /*0000dc74*/ LDR         R9, [R4, #4] 
    /*0000dc78*/ LDR         R8, [R4, #8] 
    /*0000dc7c*/ LDR         R3, [SP, #12] 
    /*0000dc80*/ SUB         R9, R9, #1 
    /*0000dc84*/ SUB         R8, R8, #1 
    /*0000dc88*/ LDR         SL, [SP, #32] 
    /*0000dc8c*/ SUB         R2, R3, #1 
    /*0000dc90*/ STR         R2, [SP, #8] 
    /*0000dc94*/ LDRH        R2, [SP, #36]               @ 0x24 
    /*0000dc98*/ LSL         R9, R9, R5 
    /*0000dc9c*/ LSL         R8, R8, R5 
    /*0000dca0*/ ADD         R3, R3, #15 
    /*0000dca4*/ STR         R7, [SP, #12] 
    /*0000dca8*/ STR         R3, [SP, #24] 
    /*0000dcac*/ STRH        R2, [R1] 
    hiisp_dis_mergePtsVecs_fw_xdcb0:
    /*0000dcb0*/ LDR         R2, [SP, #8] 
    /*0000dcb4*/ LDRB        R3, [R2, #1]! 
    /*0000dcb8*/ STR         R2, [SP, #8] 
    /*0000dcbc*/ ADD         R2, SP, #144                @ 0x90 
    /*0000dcc0*/ ADD         R3, R2, R3, LSL #1 
    /*0000dcc4*/ LDRH        R4, [R3, #-34]              @ 0xffffffde 
    /*0000dcc8*/ LDRH        R3, [R3, #-32]              @ 0xffffffe0 
    /*0000dccc*/ CMP         R4, R3 
    /*0000dcd0*/ BCS         hiisp_dis_mergePtsVecs_fw_xdd7c
    /*0000dcd4*/ LDR         R2, [R6, #4] 
    /*0000dcd8*/ CMP         R2, SL 
    /*0000dcdc*/ BEQ         hiisp_dis_mergePtsVecs_fw_xe0e8
    /*0000dce0*/ SUB         R7, R3, R4 
    /*0000dce4*/ SUB         R7, R7, #1 
    /*0000dce8*/ UXTAH       R7, R4, R7 
    /*0000dcec*/ B           hiisp_dis_mergePtsVecs_fw_xdcfc
    hiisp_dis_mergePtsVecs_fw_xdcf0:
    /*0000dcf0*/ LDR         R2, [R6, #4] 
    /*0000dcf4*/ CMP         R2, SL 
    /*0000dcf8*/ BEQ         hiisp_dis_mergePtsVecs_fw_xe0e8
    hiisp_dis_mergePtsVecs_fw_xdcfc:
    /*0000dcfc*/ ADD         R2, SP, #68                 @ 0x44 
    /*0000dd00*/ MOV         R1, R4 
    /*0000dd04*/ LDR         R0, [SP, #16] 
    /*0000dd08*/ BL          0x000000000000DD08          @ DIS_ptsVec_getElement [R_ARM_CALL: .text+0x6984] 
    /*0000dd0c*/ LDR         R3, [SP, #68]               @ 0x44 
    /*0000dd10*/ LDR         IP, [SP, #72]               @ 0x48 
    /*0000dd14*/ MOV         R2, R5 
    /*0000dd18*/ CMP         R9, R3 
    /*0000dd1c*/ MOVCC       R3, R9 
    /*0000dd20*/ CMP         IP, R8 
    /*0000dd24*/ MOV         R0, R3 
    /*0000dd28*/ ASR         R1, R3, #31 
    /*0000dd2c*/ MOVCS       IP, R8 
    /*0000dd30*/ STR         R3, [SP, #68]               @ 0x44 
    /*0000dd34*/ STR         IP, [SP, #72]               @ 0x48 
    /*0000dd38*/ BL          0x000000000000DD38          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000dd3c*/ MOV         R2, R5 
    /*0000dd40*/ MOV         FP, R0 
    /*0000dd44*/ LDR         R0, [SP, #72]               @ 0x48 
    /*0000dd48*/ ASR         R1, R0, #31 
    /*0000dd4c*/ BL          0x000000000000DD4C          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000dd50*/ LDR         R3, [SP, #4] 
    /*0000dd54*/ LDR         R2, [R3, R0, LSL #2] 
    /*0000dd58*/ LDRB        R3, [R2, FP] 
    /*0000dd5c*/ CMP         R3, #0 
    /*0000dd60*/ BNE         hiisp_dis_mergePtsVecs_fw_xdd70
    /*0000dd64*/ ADD         R1, SP, #68                 @ 0x44 
    /*0000dd68*/ MOV         R0, R6 
    /*0000dd6c*/ BL          0x000000000000DD6C          @ DIS_ptsVec_appendElement [R_ARM_CALL: .text+0x69ec] 
    hiisp_dis_mergePtsVecs_fw_xdd70:
    /*0000dd70*/ CMP         R4, R7 
    /*0000dd74*/ ADD         R4, R4, #1 
    /*0000dd78*/ BNE         hiisp_dis_mergePtsVecs_fw_xdcf0
    hiisp_dis_mergePtsVecs_fw_xdd7c:
    /*0000dd7c*/ LDR         R3, [SP, #24] 
    /*0000dd80*/ LDR         R2, [SP, #8] 
    /*0000dd84*/ CMP         R3, R2 
    /*0000dd88*/ BNE         hiisp_dis_mergePtsVecs_fw_xdcb0
    /*0000dd8c*/ LDR         R7, [SP, #12] 
    hiisp_dis_mergePtsVecs_fw_xdd90:
    /*0000dd90*/ LDR         R2, [R6, #4] 
    /*0000dd94*/ B           hiisp_dis_mergePtsVecs_fw_xe0ec
    hiisp_dis_mergePtsVecs_fw_xdd98:
    /*0000dd98*/ ADD         R9, SP, #60                 @ 0x3c 
    /*0000dd9c*/ LDR         SL, [SP, #28] 
    /*0000dda0*/ STR         R9, [SP, #8] 
    /*0000dda4*/ MOV         R8, #0 
    /*0000dda8*/ LDR         R9, [SP, #24] 
    hiisp_dis_mergePtsVecs_fw_xddac:
    /*0000ddac*/ MOV         R1, R8 
    /*0000ddb0*/ LDR         R2, [SP, #8] 
    /*0000ddb4*/ MOV         R0, R6 
    /*0000ddb8*/ BL          0x000000000000DDB8          @ DIS_ptsVec_getElement [R_ARM_CALL: .text+0x6984] 
    /*0000ddbc*/ LDR         R0, [SP, #60]               @ 0x3c 
    /*0000ddc0*/ MOV         R2, #6 
    /*0000ddc4*/ ASR         R1, R0, #31 
    /*0000ddc8*/ BL          0x000000000000DDC8          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000ddcc*/ MOV         R2, #6 
    /*0000ddd0*/ MOV         R7, R0 
    /*0000ddd4*/ LDR         R0, [SP, #64]               @ 0x40 
    /*0000ddd8*/ UXTH        R7, R7 
    /*0000dddc*/ ASR         R1, R0, #31 
    /*0000dde0*/ BL          0x000000000000DDE0          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000dde4*/ LDRH        R3, [R9, #2] 
    /*0000dde8*/ LDRH        R2, [R9, #6] 
    /*0000ddec*/ LDRH        R1, [R9, #4] 
    /*0000ddf0*/ MOV         FP, R3 
    /*0000ddf4*/ LDRH        R3, [SL, #4] 
    /*0000ddf8*/ STR         R2, [SP, #12] 
    /*0000ddfc*/ CMP         R3, R7 
    /*0000de00*/ LDRH        R2, [SL, #2] 
    /*0000de04*/ LDR         LR, [R4, #4] 
    /*0000de08*/ MOVCS       R3, #0 
    /*0000de0c*/ MOVCC       R3, #1 
    /*0000de10*/ CMP         R2, R7 
    /*0000de14*/ ADDCC       R3, R3, #1 
    /*0000de18*/ LDRH        R2, [SL, #6] 
    /*0000de1c*/ LDR         IP, [R4, #8] 
    /*0000de20*/ SUB         LR, LR, #1 
    /*0000de24*/ CMP         R2, R7 
    /*0000de28*/ ADDCC       R3, R3, #1 
    /*0000de2c*/ LDR         R7, [SP, #60]               @ 0x3c 
    /*0000de30*/ LSL         LR, LR, R5 
    /*0000de34*/ SUB         IP, IP, #1 
    /*0000de38*/ UXTH        R3, R3 
    /*0000de3c*/ MOV         R2, R5 
    /*0000de40*/ LSL         IP, IP, R5 
    /*0000de44*/ UXTH        R0, R0 
    /*0000de48*/ CMP         R1, R0 
    /*0000de4c*/ MOVCS       R1, #0 
    /*0000de50*/ MOVCC       R1, #1 
    /*0000de54*/ CMP         FP, R0 
    /*0000de58*/ ADDCC       R1, R1, #1 
    /*0000de5c*/ LDR         FP, [SP, #12] 
    /*0000de60*/ CMP         FP, R0 
    /*0000de64*/ ADDCC       R1, R1, #1 
    /*0000de68*/ LDR         R0, [SP, #64]               @ 0x40 
    /*0000de6c*/ CMP         LR, R7 
    /*0000de70*/ UXTH        R1, R1 
    /*0000de74*/ MOVCS       LR, R7 
    /*0000de78*/ CMP         R0, IP 
    /*0000de7c*/ ADD         R3, R3, R1, LSL #2 
    /*0000de80*/ STR         LR, [SP, #60]               @ 0x3c 
    /*0000de84*/ MOVCC       IP, R0 
    /*0000de88*/ STR         IP, [SP, #64]               @ 0x40 
    /*0000de8c*/ LSL         R3, R3, #1 
    /*0000de90*/ ADD         IP, SP, #78                 @ 0x4e 
    /*0000de94*/ MOV         R0, LR 
    /*0000de98*/ ASR         R1, LR, #31 
    /*0000de9c*/ LDRH        IP, [IP, R3] 
    /*0000dea0*/ ADD         LR, SP, #78                 @ 0x4e 
    /*0000dea4*/ ADD         IP, IP, #1 
    /*0000dea8*/ STRH        IP, [LR, R3] 
    /*0000deac*/ BL          0x000000000000DEAC          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000deb0*/ MOV         R2, R5 
    /*0000deb4*/ MOV         R7, R0 
    /*0000deb8*/ LDR         R0, [SP, #64]               @ 0x40 
    /*0000debc*/ ASR         R1, R0, #31 
    /*0000dec0*/ BL          0x000000000000DEC0          @ DIS_SignedRightShift [R_ARM_CALL: .text+0x74dc] 
    /*0000dec4*/ LDR         R3, [SP, #4] 
    /*0000dec8*/ LDR         R2, [SP, #8] 
    /*0000decc*/ LSR         R1, R8, #1 
    /*0000ded0*/ MOV         IP, #1 
    /*0000ded4*/ LDR         R3, [R3, R0, LSL #2] 
    /*0000ded8*/ MOV         R0, R6 
    /*0000dedc*/ STRB        IP, [R3, R7] 
    /*0000dee0*/ BL          0x000000000000DEE0          @ DIS_ptsVec_replaceElement [R_ARM_CALL: .text+0x6a64] 
    /*0000dee4*/ LDR         R2, [SP, #20] 
    /*0000dee8*/ LDRB        R3, [R2, R8] 
    /*0000deec*/ STRB        R3, [R2, R8, LSR #1] 
    /*0000def0*/ ADD         R8, R8, #2 
    /*0000def4*/ LDR         R7, [R6, #4] 
    /*0000def8*/ CMP         R7, R8 
    /*0000defc*/ BHI         hiisp_dis_mergePtsVecs_fw_xddac
    /*0000df00*/ LSR         R7, R7, #1 
    /*0000df04*/ STR         R7, [R6, #4] 
    /*0000df08*/ LDR         R8, [R4] 
    /*0000df0c*/ CMP         R8, #1 
    /*0000df10*/ BNE         hiisp_dis_mergePtsVecs_fw_xdb58
    hiisp_dis_mergePtsVecs_fw_xdf14:
    /*0000df14*/ LDR         R3, [SP, #16] 
    /*0000df18*/ LDR         R2, [R3, #4] 
    /*0000df1c*/ CMP         R2, #0 
    /*0000df20*/ BEQ         hiisp_dis_mergePtsVecs_fw_xe0f0
    /*0000df24*/ LDR         R2, [SP, #32] 
    /*0000df28*/ CMP         R2, R7 
    /*0000df2c*/ BEQ         hiisp_dis_mergePtsVecs_fw_xe204
    /*0000df30*/ STR         R7, [SP, #8] 
    /*0000df34*/ MOV         R8, #0 
    /*0000df38*/ MOV         R7, R4 
    /*0000df3c*/ ADD         R9, SP, #68                 @ 0x44 
    /*0000df40*/ MOV         SL, R3 
    /*0000df44*/ LDR         FP, [SP, #4] 
    /*0000df48*/ MOV         R4, R2 
    /*0000df4c*/ B           hiisp_dis_mergePtsVecs_fw_xdf68
    hiisp_dis_mergePtsVecs_fw_xdf50:
    /*0000df50*/ LDR         R2, [SL, #4] 
    /*0000df54*/ CMP         R2, R8 
    /*0000df58*/ BLS         hiisp_dis_mergePtsVecs_fw_xe108
    /*0000df5c*/ LDR         R2, [R6, #4] 
    /*0000df60*/ CMP         R2, R4 
    /*0000df64*/ BEQ         hiisp_dis_mergePtsVecs_fw_xe1d8
    hiisp_dis_mergePtsVecs_fw_xdf68:
    /*0000df68*/ MOV         R1, R8 
    /*0000df6c*/ MOV         R2, R9 
    /*0000df70*/ MOV         R0, SL 
    /*0000df74*/ ADD         R8, R8, #2 
    /*0000df78*/ BL          0x000000000000DF78          @ DIS_ptsVec_getElement [R_ARM_CALL: .text+0x6984] 
    /*0000df7c*/ LDR         R0, [SP, #72]               @ 0x48 
    /*0000df80*/ LDR         R1, [R7, #8] 
    /*0000df84*/ LDR         R2, [SP, #68]               @ 0x44 
    /*0000df88*/ ASR         IP, R0, R5 
    /*0000df8c*/ SUB         R0, R1, #1 
    /*0000df90*/ CMP         R0, IP 
    /*0000df94*/ LDR         R1, [R7, #4] 
    /*0000df98*/ MOVCS       R0, IP 
    /*0000df9c*/ ASR         IP, R2, R5 
    /*0000dfa0*/ SUB         R2, R1, #1 
    /*0000dfa4*/ LDR         R1, [FP, R0, LSL #2] 
    /*0000dfa8*/ CMP         R2, IP 
    /*0000dfac*/ MOVCS       R2, IP 
    /*0000dfb0*/ LDRB        R2, [R1, R2] 
    /*0000dfb4*/ CMP         R2, #0 
    /*0000dfb8*/ BNE         hiisp_dis_mergePtsVecs_fw_xdf50
    /*0000dfbc*/ MOV         R1, R9 
    /*0000dfc0*/ MOV         R0, R6 
    /*0000dfc4*/ BL          0x000000000000DFC4          @ DIS_ptsVec_appendElement [R_ARM_CALL: .text+0x69ec] 
    /*0000dfc8*/ B           hiisp_dis_mergePtsVecs_fw_xdf50
    hiisp_dis_mergePtsVecs_fw_xdfcc:
    /*0000dfcc*/ LDRB        R3, [R4, #33]               @ 0x21 
    /*0000dfd0*/ MOV         R1, R7 
    /*0000dfd4*/ LDR         R5, [R4, #44]               @ 0x2c 
    /*0000dfd8*/ LDR         R8, [R4, #36]               @ 0x24 
    /*0000dfdc*/ ADD         R3, R3, #6 
    /*0000dfe0*/ LDR         R9, [R4, #40]               @ 0x28 
    /*0000dfe4*/ MOV         R0, R5 
    /*0000dfe8*/ UXTB        R4, R3 
    /*0000dfec*/ MUL         R2, R9, R8 
    /*0000dff0*/ BL          0x000000000000DFF0          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000dff4*/ LDR         R2, [R6, #4] 
    /*0000dff8*/ CMP         R2, #0 
    /*0000dffc*/ BEQ         hiisp_dis_mergePtsVecs_fw_xe090
    /*0000e000*/ LDR         FP, [SP, #20] 
    /*0000e004*/ SUB         LR, R8, #1 
    /*0000e008*/ SUB         IP, R9, #1 
    /*0000e00c*/ MOV         R1, R7 
    /*0000e010*/ MOV         R9, #1 
    hiisp_dis_mergePtsVecs_fw_xe014:
    /*0000e014*/ LDR         R0, [R6] 
    /*0000e018*/ LSL         SL, R1, #3 
    /*0000e01c*/ ADD         R3, R0, SL 
    /*0000e020*/ LDR         R0, [R0, R1, LSL #3] 
    /*0000e024*/ LDR         R3, [R3, #4] 
    /*0000e028*/ ASR         R0, R0, R4 
    /*0000e02c*/ ASR         R3, R3, R4 
    /*0000e030*/ CMP         R3, IP 
    /*0000e034*/ MOVCS       R3, IP 
    /*0000e038*/ CMP         R0, LR 
    /*0000e03c*/ MOVCS       R0, LR 
    /*0000e040*/ MLA         R0, R8, R3, R0 
    /*0000e044*/ LDRB        R3, [R5, R0] 
    /*0000e048*/ CMP         R3, #0 
    /*0000e04c*/ BNE         hiisp_dis_mergePtsVecs_fw_xe084
    /*0000e050*/ STRB        R9, [R5, R0] 
    /*0000e054*/ LDR         R3, [R6] 
    /*0000e058*/ LDR         R2, [R3, R1, LSL #3] 
    /*0000e05c*/ STR         R2, [R3, R7, LSL #3] 
    /*0000e060*/ LDR         R3, [R6] 
    /*0000e064*/ ADD         SL, R3, SL 
    /*0000e068*/ ADD         R3, R3, R7, LSL #3 
    /*0000e06c*/ LDR         R2, [SL, #4] 
    /*0000e070*/ STR         R2, [R3, #4] 
    /*0000e074*/ LDRB        R3, [FP, R1] 
    /*0000e078*/ STRB        R3, [FP, R7] 
    /*0000e07c*/ ADD         R7, R7, #1 
    /*0000e080*/ LDR         R2, [R6, #4] 
    hiisp_dis_mergePtsVecs_fw_xe084:
    /*0000e084*/ ADD         R1, R1, #1 
    /*0000e088*/ CMP         R1, R2 
    /*0000e08c*/ BCC         hiisp_dis_mergePtsVecs_fw_xe014
    hiisp_dis_mergePtsVecs_fw_xe090:
    /*0000e090*/ STR         R7, [R6, #4] 
    hiisp_dis_mergePtsVecs_fw_xe094:
    /*0000e094*/ ADD         SP, SP, #148                @ 0x94 
    /*0000e098*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    hiisp_dis_mergePtsVecs_fw_xe09c:
    /*0000e09c*/ LDR         R4, [SP, #16] 
    /*0000e0a0*/ MOV         R2, #2400                   @ 0x960 
    /*0000e0a4*/ STR         R7, [R6, #4] 
    /*0000e0a8*/ LDR         R0, [R6] 
    /*0000e0ac*/ LDR         R1, [R4] 
    /*0000e0b0*/ BL          0x000000000000E0B0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000e0b4*/ LDR         R0, [SP, #20] 
    /*0000e0b8*/ CMP         R0, #0 
    /*0000e0bc*/ BEQ         hiisp_dis_mergePtsVecs_fw_xe094
    /*0000e0c0*/ LDR         R2, [R4, #4] 
    /*0000e0c4*/ MOV         R1, #1 
    /*0000e0c8*/ BL          0x000000000000E0C8          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000e0cc*/ ADD         SP, SP, #148                @ 0x94 
    /*0000e0d0*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    hiisp_dis_mergePtsVecs_fw_xe0d4:
    /*0000e0d4*/ MOVW        R0, #0                      @ .LC0 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x29f4] 
    /*0000e0d8*/ MOVT        R0, #0                      @ .LC0 [R_ARM_MOVT_ABS: .rodata.str1.4+0x29f4] 
    /*0000e0dc*/ BL          0x000000000000E0DC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000e0e0*/ ADD         SP, SP, #148                @ 0x94 
    /*0000e0e4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    hiisp_dis_mergePtsVecs_fw_xe0e8:
    /*0000e0e8*/ LDR         R7, [SP, #12] 
    hiisp_dis_mergePtsVecs_fw_xe0ec:
    /*0000e0ec*/ SUB         R2, R2, R7 
    hiisp_dis_mergePtsVecs_fw_xe0f0:
    /*0000e0f0*/ LDR         R3, [SP, #20] 
    /*0000e0f4*/ MOV         R1, #1 
    /*0000e0f8*/ ADD         R0, R3, R7 
    /*0000e0fc*/ BL          0x000000000000E0FC          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000e100*/ ADD         SP, SP, #148                @ 0x94 
    /*0000e104*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    hiisp_dis_mergePtsVecs_fw_xe108:
    /*0000e108*/ CMP         R2, #1 
    /*0000e10c*/ MOV         R4, R7 
    /*0000e110*/ LDR         R7, [SP, #8] 
    /*0000e114*/ BLS         hiisp_dis_mergePtsVecs_fw_xdd90
    /*0000e118*/ LDR         R3, [R6, #4] 
    /*0000e11c*/ LDR         R2, [SP, #32] 
    /*0000e120*/ CMP         R2, R3 
    /*0000e124*/ MOVEQ       R3, R2 
    /*0000e128*/ SUBEQ       R2, R3, R7 
    /*0000e12c*/ BEQ         hiisp_dis_mergePtsVecs_fw_xe0f0
    /*0000e130*/ LDR         FP, [SP, #4] 
    /*0000e134*/ MOV         R8, #1 
    /*0000e138*/ STR         R7, [SP, #4] 
    /*0000e13c*/ MOV         R7, R4 
    /*0000e140*/ LDR         SL, [SP, #16] 
    /*0000e144*/ LDR         R4, [SP, #32] 
    /*0000e148*/ B           hiisp_dis_mergePtsVecs_fw_xe164
    hiisp_dis_mergePtsVecs_fw_xe14c:
    /*0000e14c*/ LDR         R2, [SL, #4] 
    /*0000e150*/ CMP         R2, R8 
    /*0000e154*/ BLS         hiisp_dis_mergePtsVecs_fw_xe1c8
    /*0000e158*/ LDR         R2, [R6, #4] 
    /*0000e15c*/ CMP         R2, R4 
    /*0000e160*/ BEQ         hiisp_dis_mergePtsVecs_fw_xe1e8
    hiisp_dis_mergePtsVecs_fw_xe164:
    /*0000e164*/ MOV         R1, R8 
    /*0000e168*/ MOV         R2, R9 
    /*0000e16c*/ MOV         R0, SL 
    /*0000e170*/ ADD         R8, R8, #2 
    /*0000e174*/ BL          0x000000000000E174          @ DIS_ptsVec_getElement [R_ARM_CALL: .text+0x6984] 
    /*0000e178*/ LDR         R0, [SP, #72]               @ 0x48 
    /*0000e17c*/ LDR         R1, [R7, #8] 
    /*0000e180*/ LDR         R2, [SP, #68]               @ 0x44 
    /*0000e184*/ ASR         IP, R0, R5 
    /*0000e188*/ SUB         R0, R1, #1 
    /*0000e18c*/ CMP         R0, IP 
    /*0000e190*/ LDR         R1, [R7, #4] 
    /*0000e194*/ MOVCS       R0, IP 
    /*0000e198*/ ASR         IP, R2, R5 
    /*0000e19c*/ SUB         R2, R1, #1 
    /*0000e1a0*/ LDR         R1, [FP, R0, LSL #2] 
    /*0000e1a4*/ CMP         R2, IP 
    /*0000e1a8*/ MOVCS       R2, IP 
    /*0000e1ac*/ LDRB        R2, [R1, R2] 
    /*0000e1b0*/ CMP         R2, #0 
    /*0000e1b4*/ BNE         hiisp_dis_mergePtsVecs_fw_xe14c
    /*0000e1b8*/ MOV         R1, R9 
    /*0000e1bc*/ MOV         R0, R6 
    /*0000e1c0*/ BL          0x000000000000E1C0          @ DIS_ptsVec_appendElement [R_ARM_CALL: .text+0x69ec] 
    /*0000e1c4*/ B           hiisp_dis_mergePtsVecs_fw_xe14c
    hiisp_dis_mergePtsVecs_fw_xe1c8:
    /*0000e1c8*/ LDR         R7, [SP, #4] 
    /*0000e1cc*/ LDR         R2, [R6, #4] 
    /*0000e1d0*/ SUB         R2, R2, R7 
    /*0000e1d4*/ B           hiisp_dis_mergePtsVecs_fw_xe0f0
    hiisp_dis_mergePtsVecs_fw_xe1d8:
    /*0000e1d8*/ LDR         R7, [SP, #8] 
    /*0000e1dc*/ LDR         R3, [SP, #32] 
    /*0000e1e0*/ SUB         R2, R3, R7 
    /*0000e1e4*/ B           hiisp_dis_mergePtsVecs_fw_xe0f0
    hiisp_dis_mergePtsVecs_fw_xe1e8:
    /*0000e1e8*/ LDR         R7, [SP, #4] 
    /*0000e1ec*/ LDR         R3, [SP, #32] 
    /*0000e1f0*/ SUB         R2, R3, R7 
    /*0000e1f4*/ B           hiisp_dis_mergePtsVecs_fw_xe0f0
    hiisp_dis_mergePtsVecs_fw_xe1f8:
    /*0000e1f8*/ ADD         R3, SP, #112                @ 0x70 
    /*0000e1fc*/ MOV         R1, R3 
    /*0000e200*/ B           hiisp_dis_mergePtsVecs_fw_xdc74
    hiisp_dis_mergePtsVecs_fw_xe204:
    /*0000e204*/ MOV         R2, #0 
    /*0000e208*/ B           hiisp_dis_mergePtsVecs_fw_xe0f0
FUNC_END hiisp_dis_mergePtsVecs_fw


FUNC_BEGIN hiisp_dis_mergePtsVecs_fw_init
    /*0000e20c*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*0000e210*/ SUB         SP, SP, #28 
    /*0000e214*/ LDR         R3, [R2, #8] 
    /*0000e218*/ MOV         FP, R1 
    /*0000e21c*/ STR         R0, [SP, #16] 
    /*0000e220*/ MOV         R1, #0 
    /*0000e224*/ MOV         R0, #10 
    /*0000e228*/ MOV         SL, R2 
    /*0000e22c*/ STR         R3, [SP, #8] 
    /*0000e230*/ LDR         R7, [R2, #4] 
    /*0000e234*/ LDR         R4, [R2, #12] 
    /*0000e238*/ LDRB        R8, [R2, #28] 
    /*0000e23c*/ BL          0x000000000000E23C          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*0000e240*/ SUBS        R3, R0, #0 
    /*0000e244*/ STR         R3, [SP] 
    /*0000e248*/ BEQ         hiisp_dis_mergePtsVecs_fw_init_xe3d8
    /*0000e24c*/ MOV         R1, #0 
    /*0000e250*/ MOV         R0, #10 
    /*0000e254*/ BL          0x000000000000E254          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*0000e258*/ SUBS        R3, R0, #0 
    /*0000e25c*/ STR         R3, [SP, #4] 
    /*0000e260*/ BEQ         hiisp_dis_mergePtsVecs_fw_init_xe428
    /*0000e264*/ MOV         R0, R4 
    /*0000e268*/ SUB         R4, R7, #1 
    /*0000e26c*/ BL          0x000000000000E26C          @ Number2Bits [R_ARM_CALL: .text+0x7140] 
    /*0000e270*/ LDR         R3, [SP, #8] 
    /*0000e274*/ MOV         R1, #0 
    /*0000e278*/ SUB         R6, R3, #1 
    /*0000e27c*/ LDR         R3, [SP] 
    /*0000e280*/ SUB         LR, R3, #2 
    /*0000e284*/ LDR         R3, [SP, #4] 
    /*0000e288*/ SUB         IP, R3, #2 
    /*0000e28c*/ LSR         R4, R4, R0 
    /*0000e290*/ LSR         R6, R6, R0 
    /*0000e294*/ ADD         R4, R4, #1 
    /*0000e298*/ ADD         R6, R6, #1 
    /*0000e29c*/ MOV         R9, R0 
    hiisp_dis_mergePtsVecs_fw_init_xe2a0:
    /*0000e2a0*/ LDR         R3, [SL, #16] 
    /*0000e2a4*/ LDRH        R3, [R3, R1] 
    /*0000e2a8*/ ADD         R3, R3, #2 
    /*0000e2ac*/ STRH        R3, [LR, #2]! 
    /*0000e2b0*/ LDR         R3, [SL, #20] 
    /*0000e2b4*/ LDRH        R3, [R3, R1] 
    /*0000e2b8*/ ADD         R1, R1, #2 
    /*0000e2bc*/ CMP         R1, #10 
    /*0000e2c0*/ ADD         R3, R3, #2 
    /*0000e2c4*/ STRH        R3, [IP, #2]! 
    /*0000e2c8*/ BNE         hiisp_dis_mergePtsVecs_fw_init_xe2a0
    /*0000e2cc*/ LSL         R5, R6, #2 
    /*0000e2d0*/ MOV         R1, #0 
    /*0000e2d4*/ MOV         R0, R5 
    /*0000e2d8*/ BL          0x000000000000E2D8          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*0000e2dc*/ SUBS        R3, R0, #0 
    /*0000e2e0*/ STR         R3, [SP, #12] 
    /*0000e2e4*/ BEQ         hiisp_dis_mergePtsVecs_fw_init_xe3f8
    /*0000e2e8*/ MOV         R1, #0 
    /*0000e2ec*/ MUL         R0, R6, R4 
    /*0000e2f0*/ BL          0x000000000000E2F0          @ osal_kmalloc [R_ARM_CALL: +0x0] 
    /*0000e2f4*/ SUBS        R3, R0, #0 
    /*0000e2f8*/ STR         R3, [SP, #20] 
    /*0000e2fc*/ BEQ         hiisp_dis_mergePtsVecs_fw_init_xe490
    /*0000e300*/ CMP         R6, #0 
    /*0000e304*/ BEQ         hiisp_dis_mergePtsVecs_fw_init_xe32c
    /*0000e308*/ LDR         R3, [SP, #12] 
    /*0000e30c*/ SUB         R5, R5, #4 
    /*0000e310*/ LDR         R1, [SP, #20] 
    /*0000e314*/ SUB         R0, R3, #4 
    /*0000e318*/ ADD         R5, R3, R5 
    hiisp_dis_mergePtsVecs_fw_init_xe31c:
    /*0000e31c*/ STR         R1, [R0, #4]! 
    /*0000e320*/ CMP         R0, R5 
    /*0000e324*/ ADD         R1, R1, R4 
    /*0000e328*/ BNE         hiisp_dis_mergePtsVecs_fw_init_xe31c
    hiisp_dis_mergePtsVecs_fw_init_xe32c:
    /*0000e32c*/ LDR         R3, [SP, #16] 
    /*0000e330*/ MOVW        R0, #0                      @ .LANCHOR0 [R_ARM_MOVW_ABS_NC: .bss+0x1c58] 
    /*0000e334*/ MOVT        R0, #0                      @ .LANCHOR0 [R_ARM_MOVT_ABS: .bss+0x1c58] 
    /*0000e338*/ MOV         R1, #0 
    /*0000e33c*/ ADD         R0, R0, R3, LSL #4 
    /*0000e340*/ SUB         R0, R0, #1 
    hiisp_dis_mergePtsVecs_fw_init_xe344:
    /*0000e344*/ STRB        R1, [R0, #1]! 
    /*0000e348*/ ADD         R1, R1, #1 
    /*0000e34c*/ CMP         R1, #16 
    /*0000e350*/ BNE         hiisp_dis_mergePtsVecs_fw_init_xe344
    /*0000e354*/ SUB         R1, R8, #1 
    /*0000e358*/ LDR         R3, [SP, #8] 
    /*0000e35c*/ MOV         R5, #1 
    /*0000e360*/ LSL         R5, R5, R1 
    /*0000e364*/ ADD         R7, R7, R5 
    /*0000e368*/ ADD         R5, R3, R5 
    /*0000e36c*/ LSR         R7, R7, R8 
    /*0000e370*/ LSR         R5, R5, R8 
    /*0000e374*/ MUL         R0, R5, R7 
    /*0000e378*/ BL          0x000000000000E378          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*0000e37c*/ SUBS        R1, R0, #0 
    /*0000e380*/ BEQ         hiisp_dis_mergePtsVecs_fw_init_xe450
    /*0000e384*/ LDR         IP, [SL] 
    /*0000e388*/ MOV         R0, #0 
    /*0000e38c*/ LDR         R3, [SP] 
    /*0000e390*/ STMIB       FP, {R4, R6} 
    /*0000e394*/ STR         IP, [FP] 
    /*0000e398*/ LDRH        IP, [SL, #24] 
    /*0000e39c*/ STR         R3, [FP, #16] 
    /*0000e3a0*/ LDR         R3, [SP, #4] 
    /*0000e3a4*/ STRH        IP, [FP, #12] 
    /*0000e3a8*/ STR         R3, [FP, #20] 
    /*0000e3ac*/ LDR         R3, [SP, #12] 
    /*0000e3b0*/ LDRH        R2, [SL, #26] 
    /*0000e3b4*/ STRB        R9, [FP, #32] 
    /*0000e3b8*/ STR         R3, [FP, #28] 
    /*0000e3bc*/ STRH        R2, [FP, #26] 
    /*0000e3c0*/ STRB        R8, [FP, #33]               @ 0x21 
    /*0000e3c4*/ STR         R7, [FP, #36]               @ 0x24 
    /*0000e3c8*/ STR         R5, [FP, #40]               @ 0x28 
    /*0000e3cc*/ STR         R1, [FP, #44]               @ 0x2c 
    hiisp_dis_mergePtsVecs_fw_init_xe3d0:
    /*0000e3d0*/ ADD         SP, SP, #28 
    /*0000e3d4*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    hiisp_dis_mergePtsVecs_fw_init_xe3d8:
    /*0000e3d8*/ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x63c] 
    /*0000e3dc*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a28] 
    /*0000e3e0*/ MOVT        R1, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x63c] 
    /*0000e3e4*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a28] 
    /*0000e3e8*/ MOVW        R2, #363                    @ 0x16b 
    /*0000e3ec*/ BL          0x000000000000E3EC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000e3f0*/ MVN         R0, #0 
    /*0000e3f4*/ B           hiisp_dis_mergePtsVecs_fw_init_xe3d0
    hiisp_dis_mergePtsVecs_fw_init_xe3f8:
    /*0000e3f8*/ MOVW        R2, #389                    @ 0x185 
    /*0000e3fc*/ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x63c] 
    /*0000e400*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a28] 
    /*0000e404*/ MOVT        R1, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x63c] 
    /*0000e408*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a28] 
    /*0000e40c*/ BL          0x000000000000E40C          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000e410*/ LDR         R0, [SP] 
    /*0000e414*/ BL          0x000000000000E414          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e418*/ LDR         R0, [SP, #4] 
    /*0000e41c*/ BL          0x000000000000E41C          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e420*/ MVN         R0, #0 
    /*0000e424*/ B           hiisp_dis_mergePtsVecs_fw_init_xe3d0
    hiisp_dis_mergePtsVecs_fw_init_xe428:
    /*0000e428*/ MOVW        R2, #369                    @ 0x171 
    /*0000e42c*/ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x63c] 
    /*0000e430*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a28] 
    /*0000e434*/ MOVT        R1, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x63c] 
    /*0000e438*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a28] 
    /*0000e43c*/ BL          0x000000000000E43C          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000e440*/ LDR         R0, [SP] 
    /*0000e444*/ BL          0x000000000000E444          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e448*/ MVN         R0, #0 
    /*0000e44c*/ B           hiisp_dis_mergePtsVecs_fw_init_xe3d0
    hiisp_dis_mergePtsVecs_fw_init_xe450:
    /*0000e450*/ MOVW        R2, #426                    @ 0x1aa 
    /*0000e454*/ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x63c] 
    /*0000e458*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a28] 
    /*0000e45c*/ MOVT        R1, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x63c] 
    /*0000e460*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a28] 
    /*0000e464*/ BL          0x000000000000E464          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000e468*/ LDR         R0, [SP] 
    /*0000e46c*/ BL          0x000000000000E46C          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e470*/ LDR         R0, [SP, #4] 
    /*0000e474*/ BL          0x000000000000E474          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e478*/ LDR         R0, [SP, #12] 
    /*0000e47c*/ BL          0x000000000000E47C          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e480*/ LDR         R0, [SP, #20] 
    /*0000e484*/ BL          0x000000000000E484          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e488*/ MVN         R0, #0 
    /*0000e48c*/ B           hiisp_dis_mergePtsVecs_fw_init_xe3d0
    hiisp_dis_mergePtsVecs_fw_init_xe490:
    /*0000e490*/ MOV         R2, #400                    @ 0x190 
    /*0000e494*/ MOVW        R1, #0                      @ .LANCHOR1 [R_ARM_MOVW_ABS_NC: .rodata+0x63c] 
    /*0000e498*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a28] 
    /*0000e49c*/ MOVT        R1, #0                      @ .LANCHOR1 [R_ARM_MOVT_ABS: .rodata+0x63c] 
    /*0000e4a0*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a28] 
    /*0000e4a4*/ BL          0x000000000000E4A4          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000e4a8*/ LDR         R0, [SP] 
    /*0000e4ac*/ BL          0x000000000000E4AC          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e4b0*/ LDR         R0, [SP, #4] 
    /*0000e4b4*/ BL          0x000000000000E4B4          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e4b8*/ LDR         R0, [SP, #12] 
    /*0000e4bc*/ BL          0x000000000000E4BC          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e4c0*/ MVN         R0, #0 
    /*0000e4c4*/ B           hiisp_dis_mergePtsVecs_fw_init_xe3d0
FUNC_END hiisp_dis_mergePtsVecs_fw_init


FUNC_BEGIN hiisp_dis_mergePtsVecs_fw_uinit
    /*0000e4c8*/ PUSH        {R4, LR} 
    /*0000e4cc*/ MOV         R4, R0 
    /*0000e4d0*/ LDR         R0, [R0, #16] 
    /*0000e4d4*/ CMP         R0, #0 
    /*0000e4d8*/ BEQ         hiisp_dis_mergePtsVecs_fw_uinit_xe4e8
    /*0000e4dc*/ BL          0x000000000000E4DC          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e4e0*/ MOV         R3, #0 
    /*0000e4e4*/ STR         R3, [R4, #16] 
    hiisp_dis_mergePtsVecs_fw_uinit_xe4e8:
    /*0000e4e8*/ LDR         R0, [R4, #20] 
    /*0000e4ec*/ CMP         R0, #0 
    /*0000e4f0*/ BEQ         hiisp_dis_mergePtsVecs_fw_uinit_xe500
    /*0000e4f4*/ BL          0x000000000000E4F4          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e4f8*/ MOV         R3, #0 
    /*0000e4fc*/ STR         R3, [R4, #20] 
    hiisp_dis_mergePtsVecs_fw_uinit_xe500:
    /*0000e500*/ LDR         R0, [R4, #28] 
    /*0000e504*/ CMP         R0, #0 
    /*0000e508*/ BEQ         hiisp_dis_mergePtsVecs_fw_uinit_xe544
    /*0000e50c*/ LDR         R3, [R0] 
    /*0000e510*/ CMP         R3, #0 
    /*0000e514*/ BEQ         hiisp_dis_mergePtsVecs_fw_uinit_xe538
    /*0000e518*/ MOV         R0, R3 
    /*0000e51c*/ BL          0x000000000000E51C          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e520*/ LDR         R3, [R4, #28] 
    /*0000e524*/ MOV         R2, #0 
    /*0000e528*/ STR         R2, [R3] 
    /*0000e52c*/ LDR         R0, [R4, #28] 
    /*0000e530*/ CMP         R0, R2 
    /*0000e534*/ BEQ         hiisp_dis_mergePtsVecs_fw_uinit_xe544
    hiisp_dis_mergePtsVecs_fw_uinit_xe538:
    /*0000e538*/ BL          0x000000000000E538          @ osal_kfree [R_ARM_CALL: +0x0] 
    /*0000e53c*/ MOV         R3, #0 
    /*0000e540*/ STR         R3, [R4, #28] 
    hiisp_dis_mergePtsVecs_fw_uinit_xe544:
    /*0000e544*/ LDR         R0, [R4, #44]               @ 0x2c 
    /*0000e548*/ CMP         R0, #0 
    /*0000e54c*/ POPEQ       {R4, PC} 
    /*0000e550*/ BL          0x000000000000E550          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*0000e554*/ MOV         R3, #0 
    /*0000e558*/ STR         R3, [R4, #44]               @ 0x2c 
    /*0000e55c*/ POP         {R4, PC} 
FUNC_END hiisp_dis_mergePtsVecs_fw_uinit


FUNC_BEGIN hiisp_dis_inlierOverlapRatio_fw
    /*0000e560*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*0000e564*/ SUB         SP, SP, #304                @ 0x130 
    /*0000e568*/ LDR         R5, [R0, #36]               @ 0x24 
    /*0000e56c*/ MOV         R8, R0 
    /*0000e570*/ MOV         R2, #300                    @ 0x12c 
    /*0000e574*/ MOV         R1, #0 
    /*0000e578*/ ADD         R0, SP, #4 
    /*0000e57c*/ LDR         R4, [R8, #32] 
    /*0000e580*/ LDR         R7, [R8, #8] 
    /*0000e584*/ LDR         R6, [R8] 
    /*0000e588*/ BL          0x000000000000E588          @ memset [R_ARM_CALL: +0x0] 
    /*0000e58c*/ MOV         R2, #0 
    /*0000e590*/ SUB         R1, R5, #1 
    /*0000e594*/ ADD         R3, SP, #4 
    /*0000e598*/ ADD         IP, SP, #304                @ 0x130 
    /*0000e59c*/ MOV         LR, R2 
    hiisp_dis_inlierOverlapRatio_fw_xe5a0:
    /*0000e5a0*/ LDRB        R0, [R1, #1]! 
    /*0000e5a4*/ ADD         R3, R3, #1 
    /*0000e5a8*/ CMP         R0, #2 
    /*0000e5ac*/ STRBEQ      LR, [R3, #-1] 
    /*0000e5b0*/ LDRBNE      R0, [R4, R2] 
    /*0000e5b4*/ ADDNE       R2, R2, #1 
    /*0000e5b8*/ STRBNE      R0, [R3, #-1] 
    /*0000e5bc*/ CMP         R3, IP 
    /*0000e5c0*/ BNE         hiisp_dis_inlierOverlapRatio_fw_xe5a0
    /*0000e5c4*/ ADD         R1, SP, #4 
    /*0000e5c8*/ MOV         R2, #300                    @ 0x12c 
    /*0000e5cc*/ MOV         R0, R4 
    /*0000e5d0*/ BL          0x000000000000E5D0          @ osal_memcpy [R_ARM_CALL: +0x0] 
    /*0000e5d4*/ CMP         R6, #0 
    /*0000e5d8*/ BEQ         hiisp_dis_inlierOverlapRatio_fw_xe64c
    /*0000e5dc*/ SUB         R4, R4, #1 
    /*0000e5e0*/ ADD         R1, R5, R6 
    /*0000e5e4*/ MOV         R0, #0 
    hiisp_dis_inlierOverlapRatio_fw_xe5e8:
    /*0000e5e8*/ LDRB        R3, [R4, #1]! 
    /*0000e5ec*/ LDRB        R2, [R5], #1 
    /*0000e5f0*/ CMP         R3, #1 
    /*0000e5f4*/ CMPEQ       R2, #1 
    /*0000e5f8*/ MOVEQ       R3, #1 
    /*0000e5fc*/ MOVNE       R3, #0 
    /*0000e600*/ CMP         R1, R5 
    /*0000e604*/ ADD         R0, R0, R3 
    /*0000e608*/ BNE         hiisp_dis_inlierOverlapRatio_fw_xe5e8
    /*0000e60c*/ CMP         R7, #0 
    /*0000e610*/ BEQ         hiisp_dis_inlierOverlapRatio_fw_xe644
    /*0000e614*/ LSL         R0, R0, #13 
    /*0000e618*/ MOV         R1, R7 
    /*0000e61c*/ BL          0x000000000000E61C          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    /*0000e620*/ MOV         R3, R0 
    hiisp_dis_inlierOverlapRatio_fw_xe624:
    /*0000e624*/ LSL         R0, R7, #13 
    /*0000e628*/ MOV         R1, R6 
    /*0000e62c*/ MOV         R7, R3 
    /*0000e630*/ BL          0x000000000000E630          @ __aeabi_uidiv [R_ARM_CALL: +0x0] 
    hiisp_dis_inlierOverlapRatio_fw_xe634:
    /*0000e634*/ STR         R7, [R8, #40]               @ 0x28 
    /*0000e638*/ STR         R0, [R8, #4] 
    /*0000e63c*/ ADD         SP, SP, #304                @ 0x130 
    /*0000e640*/ POP         {R4, R5, R6, R7, R8, PC} 
    hiisp_dis_inlierOverlapRatio_fw_xe644:
    /*0000e644*/ MOV         R3, #1 
    /*0000e648*/ B           hiisp_dis_inlierOverlapRatio_fw_xe624
    hiisp_dis_inlierOverlapRatio_fw_xe64c:
    /*0000e64c*/ CLZ         R7, R7 
    /*0000e650*/ MOV         R0, #1 
    /*0000e654*/ LSR         R7, R7, #5 
    /*0000e658*/ B           hiisp_dis_inlierOverlapRatio_fw_xe634
FUNC_END hiisp_dis_inlierOverlapRatio_fw


FUNC_BEGIN hiisp_dis_inlierOverlapRatio_fw_init
    /*0000e65c*/ PUSH        {R4, R5, R6, LR} 
    /*0000e660*/ MOV         R4, R0 
    /*0000e664*/ MOV         R0, R1 
    /*0000e668*/ MOV         R5, R1 
    /*0000e66c*/ BL          0x000000000000E66C          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*0000e670*/ CMP         R0, #0 
    /*0000e674*/ STR         R0, [R4, #4] 
    /*0000e678*/ BEQ         hiisp_dis_inlierOverlapRatio_fw_init_xe6c0
    /*0000e67c*/ MOV         R0, R5 
    /*0000e680*/ BL          0x000000000000E680          @ osal_vmalloc [R_ARM_CALL: +0x0] 
    /*0000e684*/ CMP         R0, #0 
    /*0000e688*/ MOV         R6, R0 
    /*0000e68c*/ STR         R0, [R4] 
    /*0000e690*/ BEQ         hiisp_dis_inlierOverlapRatio_fw_init_xe6dc
    /*0000e694*/ MOV         R2, R5 
    /*0000e698*/ STR         R5, [R4, #8] 
    /*0000e69c*/ MOV         R1, #1 
    /*0000e6a0*/ LDR         R0, [R4, #4] 
    /*0000e6a4*/ BL          0x000000000000E6A4          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000e6a8*/ MOV         R2, R5 
    /*0000e6ac*/ LDR         R0, [R4] 
    /*0000e6b0*/ MOV         R1, #1 
    /*0000e6b4*/ BL          0x000000000000E6B4          @ osal_memset [R_ARM_CALL: +0x0] 
    /*0000e6b8*/ MOV         R0, #0 
    /*0000e6bc*/ POP         {R4, R5, R6, PC} 
    hiisp_dis_inlierOverlapRatio_fw_init_xe6c0:
    /*0000e6c0*/ MOVW        R2, #558                    @ 0x22e 
    /*0000e6c4*/ LDR         R1, [PC, #56]               @ 0x000000000000e704 
    /*0000e6c8*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a28] 
    /*0000e6cc*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a28] 
    /*0000e6d0*/ BL          0x000000000000E6D0          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000e6d4*/ MVN         R0, #0 
    /*0000e6d8*/ POP         {R4, R5, R6, PC} 
    hiisp_dis_inlierOverlapRatio_fw_init_xe6dc:
    /*0000e6dc*/ MOVW        R2, #565                    @ 0x235 
    /*0000e6e0*/ LDR         R1, [PC, #28]               @ 0x000000000000e704 
    /*0000e6e4*/ MOVW        R0, #0                      @ .LC1 [R_ARM_MOVW_ABS_NC: .rodata.str1.4+0x2a28] 
    /*0000e6e8*/ MOVT        R0, #0                      @ .LC1 [R_ARM_MOVT_ABS: .rodata.str1.4+0x2a28] 
    /*0000e6ec*/ BL          0x000000000000E6EC          @ osal_printk [R_ARM_CALL: +0x0] 
    /*0000e6f0*/ LDR         R0, [R4, #4] 
    /*0000e6f4*/ BL          0x000000000000E6F4          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*0000e6f8*/ STR         R6, [R4, #4] 
    /*0000e6fc*/ MVN         R0, #0 
    /*0000e700*/ POP         {R4, R5, R6, PC} 
    /*0000e704*/ ANDEQ       R0, R0, IP, ASR R6 
FUNC_END hiisp_dis_inlierOverlapRatio_fw_init


FUNC_BEGIN hiisp_dis_inlierOverlapRatio_fw_uinit
    /*0000e708*/ PUSH        {R4, LR} 
    /*0000e70c*/ MOV         R4, R0 
    /*0000e710*/ LDR         R0, [R0, #4] 
    /*0000e714*/ CMP         R0, #0 
    /*0000e718*/ BEQ         hiisp_dis_inlierOverlapRatio_fw_uinit_xe728
    /*0000e71c*/ BL          0x000000000000E71C          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*0000e720*/ MOV         R3, #0 
    /*0000e724*/ STR         R3, [R4, #4] 
    hiisp_dis_inlierOverlapRatio_fw_uinit_xe728:
    /*0000e728*/ LDR         R0, [R4] 
    /*0000e72c*/ CMP         R0, #0 
    /*0000e730*/ POPEQ       {R4, PC} 
    /*0000e734*/ BL          0x000000000000E734          @ osal_vfree [R_ARM_CALL: +0x0] 
    /*0000e738*/ MOV         R3, #0 
    /*0000e73c*/ STR         R3, [R4] 
    /*0000e740*/ POP         {R4, PC} 
FUNC_END hiisp_dis_inlierOverlapRatio_fw_uinit


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .init.text
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN dis_mod_init
    dis_mod_init_x0:
    /*00000000*/ B           dis_mod_init_x0             @ DIS_ModInit [R_ARM_JUMP24: .text+0x406c] 
FUNC_END dis_mod_init




@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .exit.text
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN dis_mod_exit
    dis_mod_exit_x0:
    /*00000000*/ B           dis_mod_exit_x0             @ DIS_ModExit [R_ARM_JUMP24: .text+0x4204] 
FUNC_END dis_mod_exit




