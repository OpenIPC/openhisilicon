@
@ -- FILES --
@

@ mn34220_sensor_ctl.c


unsigned char data[0] = {
};

unsigned char bss[0];

unsigned char data.g_fd[16] = {
    /*0000*/ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
};

_RODATA___FUNCTION___10514_0000: .ascii "mn34220_i2c_init\0"


_RODATA___FUNCTION___10530_0000: .ascii "mn34220_write_register\0"


_RODATA_MN34220_ADDR_BYTE_0000: .ascii "\x02\0\0\0",
_RODATA_MN34220_ADDR_BYTE_0004: .ascii ""


_RODATA_MN34220_DATA_BYTE_0000: .ascii "\x01\0\0\0",
_RODATA_MN34220_DATA_BYTE_0004: .ascii ""


_RODATA_MN34220_I2C_ADDR_0000: .ascii "l"


_RODATA_MN34220_I2C_INIT_STR1_4_0000: .ascii "/dev/i2c-%u\0",
_RODATA_MN34220_I2C_INIT_STR1_4_000c: .ascii "[Func]:%s [Line]:%d [Info]:Open /dev/hi_i2c_drv-%u error!\n\0\0",
_RODATA_MN34220_I2C_INIT_STR1_4_0048: .ascii "[Func]:%s [Line]:%d [Info]:I2C_SLAVE_FORCE error!\n\0"


_RODATA_MN34220_LINEAR_1080P30_INIT_STR1_4_0000: .ascii "===panasonic mn34220 mn34220 1080P30fps linear mode init success!=====\0"


_RODATA_MN34220_LINEAR_1080P60_INIT_STR1_4_0000: .ascii "===panasonic mn34220 mn34220 1080P60fps linear mode init success!=====\0"


_RODATA_MN34220_LINEAR_720P120_INIT_STR1_4_0000: .ascii "===panasonic mn34220 mn34220 720P120fps linear mode init success!=====\0"


_RODATA_MN34220_LINEAR_720P30_INIT_STR1_4_0000: .ascii "===panasonic mn34220 mn34220 720P30fps linear mode init success!=====\0"


_RODATA_MN34220_LINEAR_VGA240_INIT_STR1_4_0000: .ascii "===panasonic mn34220 mn34220 VGA 240fps linear mode init success!=====\0"


_RODATA_MN34220_WDR_1080P_2TO1_INIT_STR1_4_0000: .ascii "===panasonic mn34220 sensor 1080P30fps 2to1 WDR(30fps)(MIPI port) init success!=====\0"


_RODATA_MN34220_WDR_1080P_3TO1_INIT_STR1_4_0000: .ascii "===panasonic mn34220 mn34220 1080P30fps 3to1 WDR(90fps->30fps) init success!=====\0"


_RODATA_MN34220_WDR_1080P_4TO1_INIT_STR1_4_0000: .ascii "===panasonic mn34220 mn34220 1080P30fps 4to1 WDR(120fps->30fps) init success!=====\0"


_RODATA_MN34220_WDR_720P_3TO1_INIT_STR1_4_0000: .ascii "===panasonic mn34220 mn34220 720P20fps 3to1 WDR(60fps->20fps) init success!=====\0"


_RODATA_MN34220_WDR_720P_4TO1_INIT_STR1_4_0000: .ascii "===panasonic mn34220 mn34220 720P15fps 4to1 WDR(60fps->15fps) init success!=====\0"


_RODATA_MN34220_WDR_INIT_STR1_4_0000: .ascii "===panasonic mn34220 mn34220 1080P30fps 2to1 WDR(60fps->30fps) init success!=====\0"


_RODATA_MN34220_WRITE_REGISTER_STR1_4_0000: .ascii "[Func]:%s [Line]:%d [Info]:I2C_WRITE DATA error!\n\0"


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_i2c_init
@ Size: 0x140
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_i2c_init
    /*00000000*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00000004*/ SUB         SP, SP, #36                 @ 0x24 
    /*00000008*/ LDR         R4, WORD_0118               @ LDR         R4, [PC, #264]              @ 0x0000000000000118 
    /*0000000c*/ LDR         R7, WORD_011c               @ LDR         R7, [PC, #264]              @ 0x000000000000011c 
    /*00000010*/ ADD         R4, PC, R4 
    /*00000014*/ LDR         R3, WORD_0120               @ LDR         R3, [PC, #260]              @ 0x0000000000000120 
    /*00000018*/ ADD         R7, PC, R7 
    /*0000001c*/ LDR         R5, [R4, R3] 
    /*00000020*/ LDR         R3, [R7, R0, LSL #2] 
    /*00000024*/ LDR         R2, [R5] 
    /*00000028*/ CMP         R3, #0 
    /*0000002c*/ STR         R2, [SP, #28] 
    /*00000030*/ BLT         mn34220_i2c_init_x54
    mn34220_i2c_init_x34:
    /*00000034*/ MOV         R8, #0 
    mn34220_i2c_init_x38:
    /*00000038*/ LDR         R2, [SP, #28] 
    /*0000003c*/ MOV         R0, R8 
    /*00000040*/ LDR         R3, [R5] 
    /*00000044*/ CMP         R2, R3 
    /*00000048*/ BNE         mn34220_i2c_init_x114
    /*0000004c*/ ADD         SP, SP, #36                 @ 0x24 
    /*00000050*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    mn34220_i2c_init_x54:
    /*00000054*/ LDR         R3, WORD_0124               @ LDR         R3, [PC, #200]              @ 0x0000000000000124 
    /*00000058*/ MOV         R6, R0 
    /*0000005c*/ LDR         R2, WORD_0128               @ LDR         R2, [PC, #196]              @ 0x0000000000000128 
    /*00000060*/ ADD         R8, SP, #12 
    /*00000064*/ LDR         R3, [R4, R3] 
    /*00000068*/ MOV         R1, #16 
    /*0000006c*/ ADD         R2, PC, R2 
    /*00000070*/ MOV         R0, R8 
    /*00000074*/ LDRB        R9, [R3, R6] 
    /*00000078*/ MOV         R3, R9 
    /*0000007c*/ BL          snprintf
    /*00000080*/ MOV         R0, R8 
    /*00000084*/ MOV         R1, #2 
    /*00000088*/ BL          open
    /*0000008c*/ STR         R0, [R7, R6, LSL #2] 
    /*00000090*/ CMP         R0, #0 
    /*00000094*/ BLT         mn34220_i2c_init_xe4
    /*00000098*/ MOV         R2, #54                     @ 0x36 
    /*0000009c*/ MOVW        R1, #1798                   @ 0x706 
    /*000000a0*/ BL          ioctl
    /*000000a4*/ SUBS        R8, R0, #0 
    /*000000a8*/ BGE         mn34220_i2c_init_x34
    /*000000ac*/ LDR         R1, WORD_012c               @ LDR         R1, [PC, #120]              @ 0x000000000000012c 
    /*000000b0*/ MOV         R3, #68                     @ 0x44 
    /*000000b4*/ LDR         R2, WORD_0130               @ LDR         R2, [PC, #116]              @ 0x0000000000000130 
    /*000000b8*/ LDR         R0, [R4, R1] 
    /*000000bc*/ LDR         R1, WORD_0134               @ LDR         R1, [PC, #112]              @ 0x0000000000000134 
    /*000000c0*/ ADD         R2, PC, R2 
    /*000000c4*/ ADD         R1, PC, R1 
    /*000000c8*/ LDR         R0, [R0] 
    /*000000cc*/ BL          fprintf
    /*000000d0*/ LDR         R0, [R7, R6, LSL #2] 
    /*000000d4*/ BL          close
    /*000000d8*/ MVN         R3, #0 
    /*000000dc*/ STR         R3, [R7, R6, LSL #2] 
    /*000000e0*/ B           mn34220_i2c_init_x38
    mn34220_i2c_init_xe4:
    /*000000e4*/ LDR         R1, WORD_012c               @ LDR         R1, [PC, #64]               @ 0x000000000000012c 
    /*000000e8*/ MOV         R3, #61                     @ 0x3d 
    /*000000ec*/ LDR         R2, WORD_0138               @ LDR         R2, [PC, #68]               @ 0x0000000000000138 
    /*000000f0*/ MVN         R8, #0 
    /*000000f4*/ LDR         R0, [R4, R1] 
    /*000000f8*/ LDR         R1, WORD_013c               @ LDR         R1, [PC, #60]               @ 0x000000000000013c 
    /*000000fc*/ ADD         R2, PC, R2 
    /*00000100*/ ADD         R1, PC, R1 
    /*00000104*/ LDR         R0, [R0] 
    /*00000108*/ STR         R9, [SP] 
    /*0000010c*/ BL          fprintf
    /*00000110*/ B           mn34220_i2c_init_x38
    mn34220_i2c_init_x114:
    /*00000114*/ BL          __stack_chk_fail
    /*00000118*/ WORD_0118: .word 0x00000100
    /*0000011c*/ WORD_011c: .word 0x000000fc
    /*00000120*/ WORD_0120: .word 0x00000000
    /*00000124*/ WORD_0124: .word 0x00000000
    /*00000128*/ WORD_0128: .word 0x000000b4
    /*0000012c*/ WORD_012c: .word 0x00000000
    /*00000130*/ WORD_0130: .word 0x00000068
    /*00000134*/ WORD_0134: .word 0x00000068
    /*00000138*/ WORD_0138: .word 0x00000034
    /*0000013c*/ WORD_013c: .word 0x00000034
FUNC_END mn34220_i2c_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_i2c_exit
@ Size: 0x40
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_i2c_exit
    /*00000000*/ PUSH        {R4, R5, R6, LR} 
    /*00000004*/ LDR         R5, WORD_003c               @ LDR         R5, [PC, #48]               @ 0x000000000000003c 
    /*00000008*/ ADD         R5, PC, R5 
    /*0000000c*/ LDR         R3, [R5, R0, LSL #2] 
    /*00000010*/ CMP         R3, #0 
    /*00000014*/ BLT         mn34220_i2c_exit_x34
    /*00000018*/ MOV         R4, R0 
    /*0000001c*/ MOV         R0, R3 
    /*00000020*/ BL          close
    /*00000024*/ MVN         R3, #0 
    /*00000028*/ MOV         R0, #0 
    /*0000002c*/ STR         R3, [R5, R4, LSL #2] 
    /*00000030*/ POP         {R4, R5, R6, PC} 
    mn34220_i2c_exit_x34:
    /*00000034*/ MVN         R0, #0 
    /*00000038*/ POP         {R4, R5, R6, PC} 
    /*0000003c*/ WORD_003c: .word 0x0000002c
FUNC_END mn34220_i2c_exit


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_read_register
@ Size: 0x8
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_read_register
    /*00000000*/ MOV         R0, #0 
    /*00000004*/ BX          LR 
FUNC_END mn34220_read_register


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_write_register
@ Size: 0xc0
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_write_register
    /*00000000*/ PUSH        {R4, R5, LR} 
    /*00000004*/ SUB         SP, SP, #20 
    /*00000008*/ LDR         R4, WORD_00a8               @ LDR         R4, [PC, #152]              @ 0x00000000000000a8 
    /*0000000c*/ LDR         R3, WORD_00ac               @ LDR         R3, [PC, #152]              @ 0x00000000000000ac 
    /*00000010*/ ADD         R4, PC, R4 
    /*00000014*/ ADD         R3, PC, R3 
    /*00000018*/ LDR         IP, WORD_00b0               @ LDR         IP, [PC, #144]              @ 0x00000000000000b0 
    /*0000001c*/ LDR         R0, [R3, R0, LSL #2] 
    /*00000020*/ LDR         R5, [R4, IP] 
    /*00000024*/ CMP         R0, #0 
    /*00000028*/ LDR         R3, [R5] 
    /*0000002c*/ STR         R3, [SP, #12] 
    /*00000030*/ BLT         mn34220_write_register_x5c
    /*00000034*/ STRB        R1, [SP, #5] 
    /*00000038*/ MOV         LR, R2 
    /*0000003c*/ ASR         R1, R1, #8 
    /*00000040*/ MOV         R2, #3 
    /*00000044*/ STRB        R1, [SP, #4] 
    /*00000048*/ ADD         R1, SP, #4 
    /*0000004c*/ STRB        LR, [SP, #6] 
    /*00000050*/ BL          write
    /*00000054*/ CMP         R0, #0 
    /*00000058*/ BLT         mn34220_write_register_x78
    mn34220_write_register_x5c:
    /*0000005c*/ MOV         R0, #0 
    mn34220_write_register_x60:
    /*00000060*/ LDR         R2, [SP, #12] 
    /*00000064*/ LDR         R3, [R5] 
    /*00000068*/ CMP         R2, R3 
    /*0000006c*/ BNE         mn34220_write_register_xa4
    /*00000070*/ ADD         SP, SP, #20 
    /*00000074*/ POP         {R4, R5, PC} 
    mn34220_write_register_x78:
    /*00000078*/ LDR         R1, WORD_00b4               @ LDR         R1, [PC, #52]               @ 0x00000000000000b4 
    /*0000007c*/ MOV         R3, #135                    @ 0x87 
    /*00000080*/ LDR         R2, WORD_00b8               @ LDR         R2, [PC, #48]               @ 0x00000000000000b8 
    /*00000084*/ LDR         R0, [R4, R1] 
    /*00000088*/ LDR         R1, WORD_00bc               @ LDR         R1, [PC, #44]               @ 0x00000000000000bc 
    /*0000008c*/ ADD         R2, PC, R2 
    /*00000090*/ ADD         R1, PC, R1 
    /*00000094*/ LDR         R0, [R0] 
    /*00000098*/ BL          fprintf
    /*0000009c*/ MVN         R0, #0 
    /*000000a0*/ B           mn34220_write_register_x60
    mn34220_write_register_xa4:
    /*000000a4*/ BL          __stack_chk_fail
    /*000000a8*/ WORD_00a8: .word 0x00000090
    /*000000ac*/ WORD_00ac: .word 0x00000090
    /*000000b0*/ WORD_00b0: .word 0x00000000
    /*000000b4*/ WORD_00b4: .word 0x00000000
    /*000000b8*/ WORD_00b8: .word 0x00000024
    /*000000bc*/ WORD_00bc: .word 0x00000024
FUNC_END mn34220_write_register


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_standby
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_standby
    /*00000000*/ BX          LR 
FUNC_END mn34220_standby


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_restart
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_restart
    /*00000000*/ BX          LR 
FUNC_END mn34220_restart


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_exit
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_exit
    mn34220_exit_x0:
    /*00000000*/ B           mn34220_exit_x0             @ mn34220_i2c_exit [R_ARM_JUMP24: .text.mn34220_i2c_exit+0x0] 
FUNC_END mn34220_exit


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_linear_1080p60_init
@ Size: 0x7d8
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_linear_1080p60_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #1 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000010*/ BL          mn34220_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #0 
    /*0000001c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000020*/ BL          mn34220_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #2 
    /*0000002c*/ MOVW        R1, #773                    @ 0x305 
    /*00000030*/ BL          mn34220_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #36                     @ 0x24 
    /*0000003c*/ MOVW        R1, #775                    @ 0x307 
    /*00000040*/ BL          mn34220_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #0 
    /*0000004c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000050*/ BL          mn34220_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #3 
    /*0000005c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000060*/ BL          mn34220_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #12 
    /*0000006c*/ MOVW        R1, #274                    @ 0x112 
    /*00000070*/ BL          mn34220_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #12 
    /*0000007c*/ MOVW        R1, #275                    @ 0x113 
    /*00000080*/ BL          mn34220_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #3 
    /*0000008c*/ MOVW        R1, #12292                  @ 0x3004 
    /*00000090*/ BL          mn34220_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #100                    @ 0x64 
    /*0000009c*/ MOVW        R1, #12293                  @ 0x3005 
    /*000000a0*/ BL          mn34220_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #16 
    /*000000ac*/ MOVW        R1, #12295                  @ 0x3007 
    /*000000b0*/ BL          mn34220_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #144                    @ 0x90 
    /*000000bc*/ MOVW        R1, #12296                  @ 0x3008 
    /*000000c0*/ BL          mn34220_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #0 
    /*000000cc*/ MOVW        R1, #12299                  @ 0x300b 
    /*000000d0*/ BL          mn34220_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #67                     @ 0x43 
    /*000000dc*/ MOVW        R1, #12312                  @ 0x3018 
    /*000000e0*/ BL          mn34220_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #16 
    /*000000ec*/ MOVW        R1, #12313                  @ 0x3019 
    /*000000f0*/ BL          mn34220_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #185                    @ 0xb9 
    /*000000fc*/ MOVW        R1, #12314                  @ 0x301a 
    /*00000100*/ BL          mn34220_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #0 
    /*0000010c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000110*/ BL          mn34220_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #83                     @ 0x53 
    /*0000011c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000120*/ BL          mn34220_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #0 
    /*0000012c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000130*/ BL          mn34220_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #0 
    /*0000013c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000140*/ BL          mn34220_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #4 
    /*0000014c*/ MOVW        R1, #514                    @ 0x202 
    /*00000150*/ BL          mn34220_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #99                     @ 0x63 
    /*0000015c*/ MOVW        R1, #515                    @ 0x203 
    /*00000160*/ BL          mn34220_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #4 
    /*0000016c*/ MOV         R1, #832                    @ 0x340 
    /*00000170*/ BL          mn34220_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #101                    @ 0x65 
    /*0000017c*/ MOVW        R1, #833                    @ 0x341 
    /*00000180*/ BL          mn34220_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #8 
    /*0000018c*/ MOVW        R1, #834                    @ 0x342 
    /*00000190*/ BL          mn34220_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #152                    @ 0x98 
    /*0000019c*/ MOVW        R1, #835                    @ 0x343 
    /*000001a0*/ BL          mn34220_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #0 
    /*000001ac*/ MOVW        R1, #838                    @ 0x346 
    /*000001b0*/ BL          mn34220_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #60                     @ 0x3c 
    /*000001bc*/ MOVW        R1, #839                    @ 0x347 
    /*000001c0*/ BL          mn34220_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #4 
    /*000001cc*/ MOVW        R1, #842                    @ 0x34a 
    /*000001d0*/ BL          mn34220_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #127                    @ 0x7f 
    /*000001dc*/ MOVW        R1, #843                    @ 0x34b 
    /*000001e0*/ BL          mn34220_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #4 
    /*000001ec*/ MOVW        R1, #846                    @ 0x34e 
    /*000001f0*/ BL          mn34220_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #68                     @ 0x44 
    /*000001fc*/ MOVW        R1, #847                    @ 0x34f 
    /*00000200*/ BL          mn34220_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #0 
    /*0000020c*/ MOVW        R1, #12342                  @ 0x3036 
    /*00000210*/ BL          mn34220_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #46                     @ 0x2e 
    /*0000021c*/ MOVW        R1, #12345                  @ 0x3039 
    /*00000220*/ BL          mn34220_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #44                     @ 0x2c 
    /*0000022c*/ MOVW        R1, #12353                  @ 0x3041 
    /*00000230*/ BL          mn34220_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #15 
    /*0000023c*/ MOVW        R1, #12376                  @ 0x3058 
    /*00000240*/ BL          mn34220_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #12 
    /*0000024c*/ MOVW        R1, #12398                  @ 0x306e 
    /*00000250*/ BL          mn34220_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #0 
    /*0000025c*/ MOVW        R1, #12399                  @ 0x306f 
    /*00000260*/ BL          mn34220_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #1 
    /*0000026c*/ MOVW        R1, #12404                  @ 0x3074 
    /*00000270*/ BL          mn34220_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #0 
    /*0000027c*/ MOVW        R1, #12440                  @ 0x3098 
    /*00000280*/ BL          mn34220_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #0 
    /*0000028c*/ MOVW        R1, #12441                  @ 0x3099 
    /*00000290*/ BL          mn34220_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #1 
    /*0000029c*/ MOVW        R1, #12442                  @ 0x309a 
    /*000002a0*/ BL          mn34220_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #0 
    /*000002ac*/ MOVW        R1, #12545                  @ 0x3101 
    /*000002b0*/ BL          mn34220_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #4 
    /*000002bc*/ MOVW        R1, #12548                  @ 0x3104 
    /*000002c0*/ BL          mn34220_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #0 
    /*000002cc*/ MOVW        R1, #12550                  @ 0x3106 
    /*000002d0*/ BL          mn34220_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #192                    @ 0xc0 
    /*000002dc*/ MOVW        R1, #12551                  @ 0x3107 
    /*000002e0*/ BL          mn34220_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #0 
    /*000002ec*/ MOVW        R1, #12587                  @ 0x312b 
    /*000002f0*/ BL          mn34220_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #0 
    /*000002fc*/ MOVW        R1, #12589                  @ 0x312d 
    /*00000300*/ BL          mn34220_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #0 
    /*0000030c*/ MOVW        R1, #12591                  @ 0x312f 
    /*00000310*/ BL          mn34220_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #64                     @ 0x40 
    /*0000031c*/ MOVW        R1, #12609                  @ 0x3141 
    /*00000320*/ BL          mn34220_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #2 
    /*0000032c*/ MOVW        R1, #12611                  @ 0x3143 
    /*00000330*/ BL          mn34220_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #2 
    /*0000033c*/ MOVW        R1, #12612                  @ 0x3144 
    /*00000340*/ BL          mn34220_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #2 
    /*0000034c*/ MOVW        R1, #12613                  @ 0x3145 
    /*00000350*/ BL          mn34220_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #0 
    /*0000035c*/ MOVW        R1, #12614                  @ 0x3146 
    /*00000360*/ BL          mn34220_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #2 
    /*0000036c*/ MOVW        R1, #12615                  @ 0x3147 
    /*00000370*/ BL          mn34220_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #2 
    /*0000037c*/ MOVW        R1, #12616                  @ 0x3148 
    /*00000380*/ BL          mn34220_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #2 
    /*0000038c*/ MOVW        R1, #12617                  @ 0x3149 
    /*00000390*/ BL          mn34220_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #1 
    /*0000039c*/ MOVW        R1, #12618                  @ 0x314a 
    /*000003a0*/ BL          mn34220_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #2 
    /*000003ac*/ MOVW        R1, #12619                  @ 0x314b 
    /*000003b0*/ BL          mn34220_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #2 
    /*000003bc*/ MOVW        R1, #12620                  @ 0x314c 
    /*000003c0*/ BL          mn34220_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #2 
    /*000003cc*/ MOVW        R1, #12621                  @ 0x314d 
    /*000003d0*/ BL          mn34220_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #1 
    /*000003dc*/ MOVW        R1, #12622                  @ 0x314e 
    /*000003e0*/ BL          mn34220_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #2 
    /*000003ec*/ MOVW        R1, #12623                  @ 0x314f 
    /*000003f0*/ BL          mn34220_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #2 
    /*000003fc*/ MOVW        R1, #12624                  @ 0x3150 
    /*00000400*/ BL          mn34220_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #4 
    /*0000040c*/ MOVW        R1, #12626                  @ 0x3152 
    /*00000410*/ BL          mn34220_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #227                    @ 0xe3 
    /*0000041c*/ MOVW        R1, #12627                  @ 0x3153 
    /*00000420*/ BL          mn34220_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #202                    @ 0xca 
    /*0000042c*/ MOVW        R1, #12629                  @ 0x3155 
    /*00000430*/ BL          mn34220_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #202                    @ 0xca 
    /*0000043c*/ MOVW        R1, #12631                  @ 0x3157 
    /*00000440*/ BL          mn34220_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #202                    @ 0xca 
    /*0000044c*/ MOVW        R1, #12633                  @ 0x3159 
    /*00000450*/ BL          mn34220_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #202                    @ 0xca 
    /*0000045c*/ MOVW        R1, #12635                  @ 0x315b 
    /*00000460*/ BL          mn34220_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #202                    @ 0xca 
    /*0000046c*/ MOVW        R1, #12637                  @ 0x315d 
    /*00000470*/ BL          mn34220_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #202                    @ 0xca 
    /*0000047c*/ MOVW        R1, #12639                  @ 0x315f 
    /*00000480*/ BL          mn34220_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #202                    @ 0xca 
    /*0000048c*/ MOVW        R1, #12641                  @ 0x3161 
    /*00000490*/ BL          mn34220_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #202                    @ 0xca 
    /*0000049c*/ MOVW        R1, #12643                  @ 0x3163 
    /*000004a0*/ BL          mn34220_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #202                    @ 0xca 
    /*000004ac*/ MOVW        R1, #12645                  @ 0x3165 
    /*000004b0*/ BL          mn34220_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #202                    @ 0xca 
    /*000004bc*/ MOVW        R1, #12647                  @ 0x3167 
    /*000004c0*/ BL          mn34220_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #202                    @ 0xca 
    /*000004cc*/ MOVW        R1, #12649                  @ 0x3169 
    /*000004d0*/ BL          mn34220_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #202                    @ 0xca 
    /*000004dc*/ MOVW        R1, #12651                  @ 0x316b 
    /*000004e0*/ BL          mn34220_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #202                    @ 0xca 
    /*000004ec*/ MOVW        R1, #12653                  @ 0x316d 
    /*000004f0*/ BL          mn34220_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #198                    @ 0xc6 
    /*000004fc*/ MOVW        R1, #12655                  @ 0x316f 
    /*00000500*/ BL          mn34220_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #202                    @ 0xca 
    /*0000050c*/ MOVW        R1, #12657                  @ 0x3171 
    /*00000510*/ BL          mn34220_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #202                    @ 0xca 
    /*0000051c*/ MOVW        R1, #12659                  @ 0x3173 
    /*00000520*/ BL          mn34220_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #128                    @ 0x80 
    /*0000052c*/ MOVW        R1, #12661                  @ 0x3175 
    /*00000530*/ BL          mn34220_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #32 
    /*0000053c*/ MOVW        R1, #12686                  @ 0x318e 
    /*00000540*/ BL          mn34220_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #112                    @ 0x70 
    /*0000054c*/ MOVW        R1, #12687                  @ 0x318f 
    /*00000550*/ BL          mn34220_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #8 
    /*0000055c*/ MOVW        R1, #12694                  @ 0x3196 
    /*00000560*/ BL          mn34220_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #2 
    /*0000056c*/ MOVW        R1, #12796                  @ 0x31fc 
    /*00000570*/ BL          mn34220_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #7 
    /*0000057c*/ MOVW        R1, #12798                  @ 0x31fe 
    /*00000580*/ BL          mn34220_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #113                    @ 0x71 
    /*0000058c*/ MOVW        R1, #12860                  @ 0x323c 
    /*00000590*/ BL          mn34220_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #1 
    /*0000059c*/ MOVW        R1, #12862                  @ 0x323e 
    /*000005a0*/ BL          mn34220_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #215                    @ 0xd7 
    /*000005ac*/ MOVW        R1, #12867                  @ 0x3243 
    /*000005b0*/ BL          mn34220_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #1 
    /*000005bc*/ MOVW        R1, #12870                  @ 0x3246 
    /*000005c0*/ BL          mn34220_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #121                    @ 0x79 
    /*000005cc*/ MOVW        R1, #12871                  @ 0x3247 
    /*000005d0*/ BL          mn34220_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #0 
    /*000005dc*/ MOVW        R1, #12872                  @ 0x3248 
    /*000005e0*/ BL          mn34220_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #0 
    /*000005ec*/ MOVW        R1, #12873                  @ 0x3249 
    /*000005f0*/ BL          mn34220_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #48                     @ 0x30 
    /*000005fc*/ MOVW        R1, #12874                  @ 0x324a 
    /*00000600*/ BL          mn34220_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #24 
    /*0000060c*/ MOVW        R1, #12875                  @ 0x324b 
    /*00000610*/ BL          mn34220_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #2 
    /*0000061c*/ MOVW        R1, #12876                  @ 0x324c 
    /*00000620*/ BL          mn34220_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #222                    @ 0xde 
    /*0000062c*/ MOVW        R1, #12883                  @ 0x3253 
    /*00000630*/ BL          mn34220_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #17 
    /*0000063c*/ MOVW        R1, #12886                  @ 0x3256 
    /*00000640*/ BL          mn34220_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #1 
    /*0000064c*/ MOVW        R1, #12888                  @ 0x3258 
    /*00000650*/ BL          mn34220_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #73                     @ 0x49 
    /*0000065c*/ MOVW        R1, #12889                  @ 0x3259 
    /*00000660*/ BL          mn34220_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #57                     @ 0x39 
    /*0000066c*/ MOVW        R1, #12890                  @ 0x325a 
    /*00000670*/ BL          mn34220_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #70                     @ 0x46 
    /*0000067c*/ MOVW        R1, #12914                  @ 0x3272 
    /*00000680*/ BL          mn34220_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #48                     @ 0x30 
    /*0000068c*/ MOV         R1, #12928                  @ 0x3280 
    /*00000690*/ BL          mn34220_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #14 
    /*0000069c*/ MOVW        R1, #12930                  @ 0x3282 
    /*000006a0*/ BL          mn34220_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #27 
    /*000006ac*/ MOVW        R1, #12933                  @ 0x3285 
    /*000006b0*/ BL          mn34220_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #1 
    /*000006bc*/ MOVW        R1, #12936                  @ 0x3288 
    /*000006c0*/ BL          mn34220_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #0 
    /*000006cc*/ MOVW        R1, #12937                  @ 0x3289 
    /*000006d0*/ BL          mn34220_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #5 
    /*000006dc*/ MOVW        R1, #13070                  @ 0x330e 
    /*000006e0*/ BL          mn34220_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #2 
    /*000006ec*/ MOVW        R1, #13072                  @ 0x3310 
    /*000006f0*/ BL          mn34220_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #31 
    /*000006fc*/ MOVW        R1, #13077                  @ 0x3315 
    /*00000700*/ BL          mn34220_write_register
    /*00000704*/ MOV         R0, R4 
    /*00000708*/ MOV         R2, #2 
    /*0000070c*/ MOVW        R1, #13082                  @ 0x331a 
    /*00000710*/ BL          mn34220_write_register
    /*00000714*/ MOV         R0, R4 
    /*00000718*/ MOV         R2, #2 
    /*0000071c*/ MOVW        R1, #13083                  @ 0x331b 
    /*00000720*/ BL          mn34220_write_register
    /*00000724*/ MOV         R0, R4 
    /*00000728*/ MOV         R2, #2 
    /*0000072c*/ MOVW        R1, #13100                  @ 0x332c 
    /*00000730*/ BL          mn34220_write_register
    /*00000734*/ MOV         R0, R4 
    /*00000738*/ MOV         R2, #2 
    /*0000073c*/ MOVW        R1, #13113                  @ 0x3339 
    /*00000740*/ BL          mn34220_write_register
    /*00000744*/ MOV         R0, R4 
    /*00000748*/ MOV         R2, #3 
    /*0000074c*/ MOVW        R1, #13163                  @ 0x336b 
    /*00000750*/ BL          mn34220_write_register
    /*00000754*/ MOV         R0, R4 
    /*00000758*/ MOV         R2, #3 
    /*0000075c*/ MOVW        R1, #13215                  @ 0x339f 
    /*00000760*/ BL          mn34220_write_register
    /*00000764*/ MOV         R0, R4 
    /*00000768*/ MOV         R2, #3 
    /*0000076c*/ MOVW        R1, #13218                  @ 0x33a2 
    /*00000770*/ BL          mn34220_write_register
    /*00000774*/ MOV         R0, R4 
    /*00000778*/ MOV         R2, #3 
    /*0000077c*/ MOVW        R1, #13219                  @ 0x33a3 
    /*00000780*/ BL          mn34220_write_register
    /*00000784*/ MOV         R0, R4 
    /*00000788*/ MOV         R2, #0 
    /*0000078c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000790*/ BL          mn34220_write_register
    /*00000794*/ MOV         R0, R4 
    /*00000798*/ MOV         R2, #211                    @ 0xd3 
    /*0000079c*/ MOVW        R1, #12289                  @ 0x3001 
    /*000007a0*/ BL          mn34220_write_register
    /*000007a4*/ MOV         R0, R4 
    /*000007a8*/ MOV         R2, #1 
    /*000007ac*/ MOV         R1, #256                    @ 0x100 
    /*000007b0*/ BL          mn34220_write_register
    /*000007b4*/ MOV         R0, R4 
    /*000007b8*/ MOV         R2, #0 
    /*000007bc*/ MOVW        R1, #257                    @ 0x101 
    /*000007c0*/ BL          mn34220_write_register
    /*000007c4*/ LDR         R0, WORD_07d4               @ LDR         R0, [PC, #8]                @ 0x00000000000007d4 
    /*000007c8*/ POP         {R4, LR} 
    /*000007cc*/ ADD         R0, PC, R0 
    mn34220_linear_1080p60_init_x7d0:
    /*000007d0*/ B           mn34220_linear_1080p60_init_x7d0@ puts [R_ARM_JUMP24: +0x0] 
    /*000007d4*/ WORD_07d4: .word 0x00000000
FUNC_END mn34220_linear_1080p60_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_linear_1080p30_init
@ Size: 0x7d8
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_linear_1080p30_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #1 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000010*/ BL          mn34220_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #0 
    /*0000001c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000020*/ BL          mn34220_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #2 
    /*0000002c*/ MOVW        R1, #773                    @ 0x305 
    /*00000030*/ BL          mn34220_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #36                     @ 0x24 
    /*0000003c*/ MOVW        R1, #775                    @ 0x307 
    /*00000040*/ BL          mn34220_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #0 
    /*0000004c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000050*/ BL          mn34220_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #3 
    /*0000005c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000060*/ BL          mn34220_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #12 
    /*0000006c*/ MOVW        R1, #274                    @ 0x112 
    /*00000070*/ BL          mn34220_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #12 
    /*0000007c*/ MOVW        R1, #275                    @ 0x113 
    /*00000080*/ BL          mn34220_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #3 
    /*0000008c*/ MOVW        R1, #12292                  @ 0x3004 
    /*00000090*/ BL          mn34220_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #100                    @ 0x64 
    /*0000009c*/ MOVW        R1, #12293                  @ 0x3005 
    /*000000a0*/ BL          mn34220_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #20 
    /*000000ac*/ MOVW        R1, #12295                  @ 0x3007 
    /*000000b0*/ BL          mn34220_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #144                    @ 0x90 
    /*000000bc*/ MOVW        R1, #12296                  @ 0x3008 
    /*000000c0*/ BL          mn34220_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #0 
    /*000000cc*/ MOVW        R1, #12299                  @ 0x300b 
    /*000000d0*/ BL          mn34220_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #67                     @ 0x43 
    /*000000dc*/ MOVW        R1, #12312                  @ 0x3018 
    /*000000e0*/ BL          mn34220_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #16 
    /*000000ec*/ MOVW        R1, #12313                  @ 0x3019 
    /*000000f0*/ BL          mn34220_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #185                    @ 0xb9 
    /*000000fc*/ MOVW        R1, #12314                  @ 0x301a 
    /*00000100*/ BL          mn34220_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #0 
    /*0000010c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000110*/ BL          mn34220_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #83                     @ 0x53 
    /*0000011c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000120*/ BL          mn34220_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #0 
    /*0000012c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000130*/ BL          mn34220_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #0 
    /*0000013c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000140*/ BL          mn34220_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #4 
    /*0000014c*/ MOVW        R1, #514                    @ 0x202 
    /*00000150*/ BL          mn34220_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #99                     @ 0x63 
    /*0000015c*/ MOVW        R1, #515                    @ 0x203 
    /*00000160*/ BL          mn34220_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #4 
    /*0000016c*/ MOV         R1, #832                    @ 0x340 
    /*00000170*/ BL          mn34220_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #101                    @ 0x65 
    /*0000017c*/ MOVW        R1, #833                    @ 0x341 
    /*00000180*/ BL          mn34220_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #8 
    /*0000018c*/ MOVW        R1, #834                    @ 0x342 
    /*00000190*/ BL          mn34220_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #152                    @ 0x98 
    /*0000019c*/ MOVW        R1, #835                    @ 0x343 
    /*000001a0*/ BL          mn34220_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #0 
    /*000001ac*/ MOVW        R1, #838                    @ 0x346 
    /*000001b0*/ BL          mn34220_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #60                     @ 0x3c 
    /*000001bc*/ MOVW        R1, #839                    @ 0x347 
    /*000001c0*/ BL          mn34220_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #4 
    /*000001cc*/ MOVW        R1, #842                    @ 0x34a 
    /*000001d0*/ BL          mn34220_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #127                    @ 0x7f 
    /*000001dc*/ MOVW        R1, #843                    @ 0x34b 
    /*000001e0*/ BL          mn34220_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #4 
    /*000001ec*/ MOVW        R1, #846                    @ 0x34e 
    /*000001f0*/ BL          mn34220_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #68                     @ 0x44 
    /*000001fc*/ MOVW        R1, #847                    @ 0x34f 
    /*00000200*/ BL          mn34220_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #0 
    /*0000020c*/ MOVW        R1, #12342                  @ 0x3036 
    /*00000210*/ BL          mn34220_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #46                     @ 0x2e 
    /*0000021c*/ MOVW        R1, #12345                  @ 0x3039 
    /*00000220*/ BL          mn34220_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #44                     @ 0x2c 
    /*0000022c*/ MOVW        R1, #12353                  @ 0x3041 
    /*00000230*/ BL          mn34220_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #15 
    /*0000023c*/ MOVW        R1, #12376                  @ 0x3058 
    /*00000240*/ BL          mn34220_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #12 
    /*0000024c*/ MOVW        R1, #12398                  @ 0x306e 
    /*00000250*/ BL          mn34220_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #0 
    /*0000025c*/ MOVW        R1, #12399                  @ 0x306f 
    /*00000260*/ BL          mn34220_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #1 
    /*0000026c*/ MOVW        R1, #12404                  @ 0x3074 
    /*00000270*/ BL          mn34220_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #0 
    /*0000027c*/ MOVW        R1, #12440                  @ 0x3098 
    /*00000280*/ BL          mn34220_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #0 
    /*0000028c*/ MOVW        R1, #12441                  @ 0x3099 
    /*00000290*/ BL          mn34220_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #1 
    /*0000029c*/ MOVW        R1, #12442                  @ 0x309a 
    /*000002a0*/ BL          mn34220_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #0 
    /*000002ac*/ MOVW        R1, #12545                  @ 0x3101 
    /*000002b0*/ BL          mn34220_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #4 
    /*000002bc*/ MOVW        R1, #12548                  @ 0x3104 
    /*000002c0*/ BL          mn34220_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #0 
    /*000002cc*/ MOVW        R1, #12550                  @ 0x3106 
    /*000002d0*/ BL          mn34220_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #192                    @ 0xc0 
    /*000002dc*/ MOVW        R1, #12551                  @ 0x3107 
    /*000002e0*/ BL          mn34220_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #0 
    /*000002ec*/ MOVW        R1, #12587                  @ 0x312b 
    /*000002f0*/ BL          mn34220_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #0 
    /*000002fc*/ MOVW        R1, #12589                  @ 0x312d 
    /*00000300*/ BL          mn34220_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #0 
    /*0000030c*/ MOVW        R1, #12591                  @ 0x312f 
    /*00000310*/ BL          mn34220_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #64                     @ 0x40 
    /*0000031c*/ MOVW        R1, #12609                  @ 0x3141 
    /*00000320*/ BL          mn34220_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #2 
    /*0000032c*/ MOVW        R1, #12611                  @ 0x3143 
    /*00000330*/ BL          mn34220_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #2 
    /*0000033c*/ MOVW        R1, #12612                  @ 0x3144 
    /*00000340*/ BL          mn34220_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #2 
    /*0000034c*/ MOVW        R1, #12613                  @ 0x3145 
    /*00000350*/ BL          mn34220_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #0 
    /*0000035c*/ MOVW        R1, #12614                  @ 0x3146 
    /*00000360*/ BL          mn34220_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #2 
    /*0000036c*/ MOVW        R1, #12615                  @ 0x3147 
    /*00000370*/ BL          mn34220_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #2 
    /*0000037c*/ MOVW        R1, #12616                  @ 0x3148 
    /*00000380*/ BL          mn34220_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #2 
    /*0000038c*/ MOVW        R1, #12617                  @ 0x3149 
    /*00000390*/ BL          mn34220_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #1 
    /*0000039c*/ MOVW        R1, #12618                  @ 0x314a 
    /*000003a0*/ BL          mn34220_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #2 
    /*000003ac*/ MOVW        R1, #12619                  @ 0x314b 
    /*000003b0*/ BL          mn34220_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #2 
    /*000003bc*/ MOVW        R1, #12620                  @ 0x314c 
    /*000003c0*/ BL          mn34220_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #2 
    /*000003cc*/ MOVW        R1, #12621                  @ 0x314d 
    /*000003d0*/ BL          mn34220_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #1 
    /*000003dc*/ MOVW        R1, #12622                  @ 0x314e 
    /*000003e0*/ BL          mn34220_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #2 
    /*000003ec*/ MOVW        R1, #12623                  @ 0x314f 
    /*000003f0*/ BL          mn34220_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #2 
    /*000003fc*/ MOVW        R1, #12624                  @ 0x3150 
    /*00000400*/ BL          mn34220_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #4 
    /*0000040c*/ MOVW        R1, #12626                  @ 0x3152 
    /*00000410*/ BL          mn34220_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #227                    @ 0xe3 
    /*0000041c*/ MOVW        R1, #12627                  @ 0x3153 
    /*00000420*/ BL          mn34220_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #202                    @ 0xca 
    /*0000042c*/ MOVW        R1, #12629                  @ 0x3155 
    /*00000430*/ BL          mn34220_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #202                    @ 0xca 
    /*0000043c*/ MOVW        R1, #12631                  @ 0x3157 
    /*00000440*/ BL          mn34220_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #202                    @ 0xca 
    /*0000044c*/ MOVW        R1, #12633                  @ 0x3159 
    /*00000450*/ BL          mn34220_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #202                    @ 0xca 
    /*0000045c*/ MOVW        R1, #12635                  @ 0x315b 
    /*00000460*/ BL          mn34220_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #202                    @ 0xca 
    /*0000046c*/ MOVW        R1, #12637                  @ 0x315d 
    /*00000470*/ BL          mn34220_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #202                    @ 0xca 
    /*0000047c*/ MOVW        R1, #12639                  @ 0x315f 
    /*00000480*/ BL          mn34220_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #202                    @ 0xca 
    /*0000048c*/ MOVW        R1, #12641                  @ 0x3161 
    /*00000490*/ BL          mn34220_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #202                    @ 0xca 
    /*0000049c*/ MOVW        R1, #12643                  @ 0x3163 
    /*000004a0*/ BL          mn34220_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #202                    @ 0xca 
    /*000004ac*/ MOVW        R1, #12645                  @ 0x3165 
    /*000004b0*/ BL          mn34220_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #202                    @ 0xca 
    /*000004bc*/ MOVW        R1, #12647                  @ 0x3167 
    /*000004c0*/ BL          mn34220_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #202                    @ 0xca 
    /*000004cc*/ MOVW        R1, #12649                  @ 0x3169 
    /*000004d0*/ BL          mn34220_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #202                    @ 0xca 
    /*000004dc*/ MOVW        R1, #12651                  @ 0x316b 
    /*000004e0*/ BL          mn34220_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #202                    @ 0xca 
    /*000004ec*/ MOVW        R1, #12653                  @ 0x316d 
    /*000004f0*/ BL          mn34220_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #198                    @ 0xc6 
    /*000004fc*/ MOVW        R1, #12655                  @ 0x316f 
    /*00000500*/ BL          mn34220_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #202                    @ 0xca 
    /*0000050c*/ MOVW        R1, #12657                  @ 0x3171 
    /*00000510*/ BL          mn34220_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #202                    @ 0xca 
    /*0000051c*/ MOVW        R1, #12659                  @ 0x3173 
    /*00000520*/ BL          mn34220_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #128                    @ 0x80 
    /*0000052c*/ MOVW        R1, #12661                  @ 0x3175 
    /*00000530*/ BL          mn34220_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #32 
    /*0000053c*/ MOVW        R1, #12686                  @ 0x318e 
    /*00000540*/ BL          mn34220_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #112                    @ 0x70 
    /*0000054c*/ MOVW        R1, #12687                  @ 0x318f 
    /*00000550*/ BL          mn34220_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #8 
    /*0000055c*/ MOVW        R1, #12694                  @ 0x3196 
    /*00000560*/ BL          mn34220_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #2 
    /*0000056c*/ MOVW        R1, #12796                  @ 0x31fc 
    /*00000570*/ BL          mn34220_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #7 
    /*0000057c*/ MOVW        R1, #12798                  @ 0x31fe 
    /*00000580*/ BL          mn34220_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #113                    @ 0x71 
    /*0000058c*/ MOVW        R1, #12860                  @ 0x323c 
    /*00000590*/ BL          mn34220_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #1 
    /*0000059c*/ MOVW        R1, #12862                  @ 0x323e 
    /*000005a0*/ BL          mn34220_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #215                    @ 0xd7 
    /*000005ac*/ MOVW        R1, #12867                  @ 0x3243 
    /*000005b0*/ BL          mn34220_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #1 
    /*000005bc*/ MOVW        R1, #12870                  @ 0x3246 
    /*000005c0*/ BL          mn34220_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #56                     @ 0x38 
    /*000005cc*/ MOVW        R1, #12871                  @ 0x3247 
    /*000005d0*/ BL          mn34220_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #3 
    /*000005dc*/ MOVW        R1, #12872                  @ 0x3248 
    /*000005e0*/ BL          mn34220_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #226                    @ 0xe2 
    /*000005ec*/ MOVW        R1, #12873                  @ 0x3249 
    /*000005f0*/ BL          mn34220_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #48                     @ 0x30 
    /*000005fc*/ MOVW        R1, #12874                  @ 0x324a 
    /*00000600*/ BL          mn34220_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #24 
    /*0000060c*/ MOVW        R1, #12875                  @ 0x324b 
    /*00000610*/ BL          mn34220_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #2 
    /*0000061c*/ MOVW        R1, #12876                  @ 0x324c 
    /*00000620*/ BL          mn34220_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #222                    @ 0xde 
    /*0000062c*/ MOVW        R1, #12883                  @ 0x3253 
    /*00000630*/ BL          mn34220_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #17 
    /*0000063c*/ MOVW        R1, #12886                  @ 0x3256 
    /*00000640*/ BL          mn34220_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #1 
    /*0000064c*/ MOVW        R1, #12888                  @ 0x3258 
    /*00000650*/ BL          mn34220_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #104                    @ 0x68 
    /*0000065c*/ MOVW        R1, #12889                  @ 0x3259 
    /*00000660*/ BL          mn34220_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #57                     @ 0x39 
    /*0000066c*/ MOVW        R1, #12890                  @ 0x325a 
    /*00000670*/ BL          mn34220_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #70                     @ 0x46 
    /*0000067c*/ MOVW        R1, #12914                  @ 0x3272 
    /*00000680*/ BL          mn34220_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #48                     @ 0x30 
    /*0000068c*/ MOV         R1, #12928                  @ 0x3280 
    /*00000690*/ BL          mn34220_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #14 
    /*0000069c*/ MOVW        R1, #12930                  @ 0x3282 
    /*000006a0*/ BL          mn34220_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #27 
    /*000006ac*/ MOVW        R1, #12933                  @ 0x3285 
    /*000006b0*/ BL          mn34220_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #1 
    /*000006bc*/ MOVW        R1, #12936                  @ 0x3288 
    /*000006c0*/ BL          mn34220_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #0 
    /*000006cc*/ MOVW        R1, #12937                  @ 0x3289 
    /*000006d0*/ BL          mn34220_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #5 
    /*000006dc*/ MOVW        R1, #13070                  @ 0x330e 
    /*000006e0*/ BL          mn34220_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #2 
    /*000006ec*/ MOVW        R1, #13072                  @ 0x3310 
    /*000006f0*/ BL          mn34220_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #31 
    /*000006fc*/ MOVW        R1, #13077                  @ 0x3315 
    /*00000700*/ BL          mn34220_write_register
    /*00000704*/ MOV         R0, R4 
    /*00000708*/ MOV         R2, #2 
    /*0000070c*/ MOVW        R1, #13082                  @ 0x331a 
    /*00000710*/ BL          mn34220_write_register
    /*00000714*/ MOV         R0, R4 
    /*00000718*/ MOV         R2, #2 
    /*0000071c*/ MOVW        R1, #13083                  @ 0x331b 
    /*00000720*/ BL          mn34220_write_register
    /*00000724*/ MOV         R0, R4 
    /*00000728*/ MOV         R2, #2 
    /*0000072c*/ MOVW        R1, #13100                  @ 0x332c 
    /*00000730*/ BL          mn34220_write_register
    /*00000734*/ MOV         R0, R4 
    /*00000738*/ MOV         R2, #2 
    /*0000073c*/ MOVW        R1, #13113                  @ 0x3339 
    /*00000740*/ BL          mn34220_write_register
    /*00000744*/ MOV         R0, R4 
    /*00000748*/ MOV         R2, #3 
    /*0000074c*/ MOVW        R1, #13163                  @ 0x336b 
    /*00000750*/ BL          mn34220_write_register
    /*00000754*/ MOV         R0, R4 
    /*00000758*/ MOV         R2, #3 
    /*0000075c*/ MOVW        R1, #13215                  @ 0x339f 
    /*00000760*/ BL          mn34220_write_register
    /*00000764*/ MOV         R0, R4 
    /*00000768*/ MOV         R2, #3 
    /*0000076c*/ MOVW        R1, #13218                  @ 0x33a2 
    /*00000770*/ BL          mn34220_write_register
    /*00000774*/ MOV         R0, R4 
    /*00000778*/ MOV         R2, #3 
    /*0000077c*/ MOVW        R1, #13219                  @ 0x33a3 
    /*00000780*/ BL          mn34220_write_register
    /*00000784*/ MOV         R0, R4 
    /*00000788*/ MOV         R2, #0 
    /*0000078c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000790*/ BL          mn34220_write_register
    /*00000794*/ MOV         R0, R4 
    /*00000798*/ MOV         R2, #211                    @ 0xd3 
    /*0000079c*/ MOVW        R1, #12289                  @ 0x3001 
    /*000007a0*/ BL          mn34220_write_register
    /*000007a4*/ MOV         R0, R4 
    /*000007a8*/ MOV         R2, #1 
    /*000007ac*/ MOV         R1, #256                    @ 0x100 
    /*000007b0*/ BL          mn34220_write_register
    /*000007b4*/ MOV         R0, R4 
    /*000007b8*/ MOV         R2, #0 
    /*000007bc*/ MOVW        R1, #257                    @ 0x101 
    /*000007c0*/ BL          mn34220_write_register
    /*000007c4*/ LDR         R0, WORD_07d4               @ LDR         R0, [PC, #8]                @ 0x00000000000007d4 
    /*000007c8*/ POP         {R4, LR} 
    /*000007cc*/ ADD         R0, PC, R0 
    mn34220_linear_1080p30_init_x7d0:
    /*000007d0*/ B           mn34220_linear_1080p30_init_x7d0@ puts [R_ARM_JUMP24: +0x0] 
    /*000007d4*/ WORD_07d4: .word 0x00000000
FUNC_END mn34220_linear_1080p30_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_linear_720p120_init
@ Size: 0x7d8
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_linear_720p120_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #1 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000010*/ BL          mn34220_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #0 
    /*0000001c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000020*/ BL          mn34220_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #2 
    /*0000002c*/ MOVW        R1, #773                    @ 0x305 
    /*00000030*/ BL          mn34220_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #48                     @ 0x30 
    /*0000003c*/ MOVW        R1, #775                    @ 0x307 
    /*00000040*/ BL          mn34220_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #0 
    /*0000004c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000050*/ BL          mn34220_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #3 
    /*0000005c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000060*/ BL          mn34220_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #12 
    /*0000006c*/ MOVW        R1, #274                    @ 0x112 
    /*00000070*/ BL          mn34220_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #12 
    /*0000007c*/ MOVW        R1, #275                    @ 0x113 
    /*00000080*/ BL          mn34220_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #3 
    /*0000008c*/ MOVW        R1, #12292                  @ 0x3004 
    /*00000090*/ BL          mn34220_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #103                    @ 0x67 
    /*0000009c*/ MOVW        R1, #12293                  @ 0x3005 
    /*000000a0*/ BL          mn34220_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #16 
    /*000000ac*/ MOVW        R1, #12295                  @ 0x3007 
    /*000000b0*/ BL          mn34220_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #144                    @ 0x90 
    /*000000bc*/ MOVW        R1, #12296                  @ 0x3008 
    /*000000c0*/ BL          mn34220_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #0 
    /*000000cc*/ MOVW        R1, #12299                  @ 0x300b 
    /*000000d0*/ BL          mn34220_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #67                     @ 0x43 
    /*000000dc*/ MOVW        R1, #12312                  @ 0x3018 
    /*000000e0*/ BL          mn34220_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #16 
    /*000000ec*/ MOVW        R1, #12313                  @ 0x3019 
    /*000000f0*/ BL          mn34220_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #185                    @ 0xb9 
    /*000000fc*/ MOVW        R1, #12314                  @ 0x301a 
    /*00000100*/ BL          mn34220_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #0 
    /*0000010c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000110*/ BL          mn34220_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #83                     @ 0x53 
    /*0000011c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000120*/ BL          mn34220_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #0 
    /*0000012c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000130*/ BL          mn34220_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #0 
    /*0000013c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000140*/ BL          mn34220_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #2 
    /*0000014c*/ MOVW        R1, #514                    @ 0x202 
    /*00000150*/ BL          mn34220_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #236                    @ 0xec 
    /*0000015c*/ MOVW        R1, #515                    @ 0x203 
    /*00000160*/ BL          mn34220_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #2 
    /*0000016c*/ MOV         R1, #832                    @ 0x340 
    /*00000170*/ BL          mn34220_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #238                    @ 0xee 
    /*0000017c*/ MOVW        R1, #833                    @ 0x341 
    /*00000180*/ BL          mn34220_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #8 
    /*0000018c*/ MOVW        R1, #834                    @ 0x342 
    /*00000190*/ BL          mn34220_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #160                    @ 0xa0 
    /*0000019c*/ MOVW        R1, #835                    @ 0x343 
    /*000001a0*/ BL          mn34220_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #0 
    /*000001ac*/ MOVW        R1, #838                    @ 0x346 
    /*000001b0*/ BL          mn34220_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #242                    @ 0xf2 
    /*000001bc*/ MOVW        R1, #839                    @ 0x347 
    /*000001c0*/ BL          mn34220_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #3 
    /*000001cc*/ MOVW        R1, #842                    @ 0x34a 
    /*000001d0*/ BL          mn34220_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #201                    @ 0xc9 
    /*000001dc*/ MOVW        R1, #843                    @ 0x34b 
    /*000001e0*/ BL          mn34220_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #2 
    /*000001ec*/ MOVW        R1, #846                    @ 0x34e 
    /*000001f0*/ BL          mn34220_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #216                    @ 0xd8 
    /*000001fc*/ MOVW        R1, #847                    @ 0x34f 
    /*00000200*/ BL          mn34220_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #0 
    /*0000020c*/ MOVW        R1, #12342                  @ 0x3036 
    /*00000210*/ BL          mn34220_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #46                     @ 0x2e 
    /*0000021c*/ MOVW        R1, #12345                  @ 0x3039 
    /*00000220*/ BL          mn34220_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #18 
    /*0000022c*/ MOVW        R1, #12353                  @ 0x3041 
    /*00000230*/ BL          mn34220_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #15 
    /*0000023c*/ MOVW        R1, #12376                  @ 0x3058 
    /*00000240*/ BL          mn34220_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #12 
    /*0000024c*/ MOVW        R1, #12398                  @ 0x306e 
    /*00000250*/ BL          mn34220_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #0 
    /*0000025c*/ MOVW        R1, #12399                  @ 0x306f 
    /*00000260*/ BL          mn34220_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #1 
    /*0000026c*/ MOVW        R1, #12404                  @ 0x3074 
    /*00000270*/ BL          mn34220_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #0 
    /*0000027c*/ MOVW        R1, #12440                  @ 0x3098 
    /*00000280*/ BL          mn34220_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #64                     @ 0x40 
    /*0000028c*/ MOVW        R1, #12441                  @ 0x3099 
    /*00000290*/ BL          mn34220_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #16 
    /*0000029c*/ MOVW        R1, #12442                  @ 0x309a 
    /*000002a0*/ BL          mn34220_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #0 
    /*000002ac*/ MOVW        R1, #12545                  @ 0x3101 
    /*000002b0*/ BL          mn34220_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #4 
    /*000002bc*/ MOVW        R1, #12548                  @ 0x3104 
    /*000002c0*/ BL          mn34220_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #0 
    /*000002cc*/ MOVW        R1, #12550                  @ 0x3106 
    /*000002d0*/ BL          mn34220_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #192                    @ 0xc0 
    /*000002dc*/ MOVW        R1, #12551                  @ 0x3107 
    /*000002e0*/ BL          mn34220_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #0 
    /*000002ec*/ MOVW        R1, #12587                  @ 0x312b 
    /*000002f0*/ BL          mn34220_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #0 
    /*000002fc*/ MOVW        R1, #12589                  @ 0x312d 
    /*00000300*/ BL          mn34220_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #0 
    /*0000030c*/ MOVW        R1, #12591                  @ 0x312f 
    /*00000310*/ BL          mn34220_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #112                    @ 0x70 
    /*0000031c*/ MOVW        R1, #12609                  @ 0x3141 
    /*00000320*/ BL          mn34220_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #1 
    /*0000032c*/ MOVW        R1, #12611                  @ 0x3143 
    /*00000330*/ BL          mn34220_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #3 
    /*0000033c*/ MOVW        R1, #12612                  @ 0x3144 
    /*00000340*/ BL          mn34220_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #2 
    /*0000034c*/ MOVW        R1, #12613                  @ 0x3145 
    /*00000350*/ BL          mn34220_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #5 
    /*0000035c*/ MOVW        R1, #12614                  @ 0x3146 
    /*00000360*/ BL          mn34220_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #0 
    /*0000036c*/ MOVW        R1, #12615                  @ 0x3147 
    /*00000370*/ BL          mn34220_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #0 
    /*0000037c*/ MOVW        R1, #12616                  @ 0x3148 
    /*00000380*/ BL          mn34220_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #0 
    /*0000038c*/ MOVW        R1, #12617                  @ 0x3149 
    /*00000390*/ BL          mn34220_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #3 
    /*0000039c*/ MOVW        R1, #12618                  @ 0x314a 
    /*000003a0*/ BL          mn34220_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #1 
    /*000003ac*/ MOVW        R1, #12619                  @ 0x314b 
    /*000003b0*/ BL          mn34220_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #1 
    /*000003bc*/ MOVW        R1, #12620                  @ 0x314c 
    /*000003c0*/ BL          mn34220_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #1 
    /*000003cc*/ MOVW        R1, #12621                  @ 0x314d 
    /*000003d0*/ BL          mn34220_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #2 
    /*000003dc*/ MOVW        R1, #12622                  @ 0x314e 
    /*000003e0*/ BL          mn34220_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #2 
    /*000003ec*/ MOVW        R1, #12623                  @ 0x314f 
    /*000003f0*/ BL          mn34220_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #2 
    /*000003fc*/ MOVW        R1, #12624                  @ 0x3150 
    /*00000400*/ BL          mn34220_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #1 
    /*0000040c*/ MOVW        R1, #12626                  @ 0x3152 
    /*00000410*/ BL          mn34220_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #227                    @ 0xe3 
    /*0000041c*/ MOVW        R1, #12627                  @ 0x3153 
    /*00000420*/ BL          mn34220_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #17 
    /*0000042c*/ MOVW        R1, #12629                  @ 0x3155 
    /*00000430*/ BL          mn34220_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #48                     @ 0x30 
    /*0000043c*/ MOVW        R1, #12631                  @ 0x3157 
    /*00000440*/ BL          mn34220_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #51                     @ 0x33 
    /*0000044c*/ MOVW        R1, #12633                  @ 0x3159 
    /*00000450*/ BL          mn34220_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #54                     @ 0x36 
    /*0000045c*/ MOVW        R1, #12635                  @ 0x315b 
    /*00000460*/ BL          mn34220_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #53                     @ 0x35 
    /*0000046c*/ MOVW        R1, #12637                  @ 0x315d 
    /*00000470*/ BL          mn34220_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #60                     @ 0x3c 
    /*0000047c*/ MOVW        R1, #12639                  @ 0x315f 
    /*00000480*/ BL          mn34220_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #63                     @ 0x3f 
    /*0000048c*/ MOVW        R1, #12641                  @ 0x3161 
    /*00000490*/ BL          mn34220_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #58                     @ 0x3a 
    /*0000049c*/ MOVW        R1, #12643                  @ 0x3163 
    /*000004a0*/ BL          mn34220_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #57                     @ 0x39 
    /*000004ac*/ MOVW        R1, #12645                  @ 0x3165 
    /*000004b0*/ BL          mn34220_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #40                     @ 0x28 
    /*000004bc*/ MOVW        R1, #12647                  @ 0x3167 
    /*000004c0*/ BL          mn34220_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #43                     @ 0x2b 
    /*000004cc*/ MOVW        R1, #12649                  @ 0x3169 
    /*000004d0*/ BL          mn34220_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #46                     @ 0x2e 
    /*000004dc*/ MOVW        R1, #12651                  @ 0x316b 
    /*000004e0*/ BL          mn34220_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #45                     @ 0x2d 
    /*000004ec*/ MOVW        R1, #12653                  @ 0x316d 
    /*000004f0*/ BL          mn34220_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #34                     @ 0x22 
    /*000004fc*/ MOVW        R1, #12655                  @ 0x316f 
    /*00000500*/ BL          mn34220_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #34                     @ 0x22 
    /*0000050c*/ MOVW        R1, #12657                  @ 0x3171 
    /*00000510*/ BL          mn34220_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #97                     @ 0x61 
    /*0000051c*/ MOVW        R1, #12659                  @ 0x3173 
    /*00000520*/ BL          mn34220_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #128                    @ 0x80 
    /*0000052c*/ MOVW        R1, #12661                  @ 0x3175 
    /*00000530*/ BL          mn34220_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #32 
    /*0000053c*/ MOVW        R1, #12686                  @ 0x318e 
    /*00000540*/ BL          mn34220_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #112                    @ 0x70 
    /*0000054c*/ MOVW        R1, #12687                  @ 0x318f 
    /*00000550*/ BL          mn34220_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #8 
    /*0000055c*/ MOVW        R1, #12694                  @ 0x3196 
    /*00000560*/ BL          mn34220_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #3 
    /*0000056c*/ MOVW        R1, #12796                  @ 0x31fc 
    /*00000570*/ BL          mn34220_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #6 
    /*0000057c*/ MOVW        R1, #12798                  @ 0x31fe 
    /*00000580*/ BL          mn34220_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #113                    @ 0x71 
    /*0000058c*/ MOVW        R1, #12860                  @ 0x323c 
    /*00000590*/ BL          mn34220_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #1 
    /*0000059c*/ MOVW        R1, #12862                  @ 0x323e 
    /*000005a0*/ BL          mn34220_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #117                    @ 0x75 
    /*000005ac*/ MOVW        R1, #12867                  @ 0x3243 
    /*000005b0*/ BL          mn34220_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #1 
    /*000005bc*/ MOVW        R1, #12870                  @ 0x3246 
    /*000005c0*/ BL          mn34220_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #165                    @ 0xa5 
    /*000005cc*/ MOVW        R1, #12871                  @ 0x3247 
    /*000005d0*/ BL          mn34220_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #0 
    /*000005dc*/ MOVW        R1, #12872                  @ 0x3248 
    /*000005e0*/ BL          mn34220_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #0 
    /*000005ec*/ MOVW        R1, #12873                  @ 0x3249 
    /*000005f0*/ BL          mn34220_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #48                     @ 0x30 
    /*000005fc*/ MOVW        R1, #12874                  @ 0x324a 
    /*00000600*/ BL          mn34220_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #27 
    /*0000060c*/ MOVW        R1, #12875                  @ 0x324b 
    /*00000610*/ BL          mn34220_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #2 
    /*0000061c*/ MOVW        R1, #12876                  @ 0x324c 
    /*00000620*/ BL          mn34220_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #123                    @ 0x7b 
    /*0000062c*/ MOVW        R1, #12883                  @ 0x3253 
    /*00000630*/ BL          mn34220_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #50                     @ 0x32 
    /*0000063c*/ MOVW        R1, #12886                  @ 0x3256 
    /*00000640*/ BL          mn34220_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #1 
    /*0000064c*/ MOVW        R1, #12888                  @ 0x3258 
    /*00000650*/ BL          mn34220_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #154                    @ 0x9a 
    /*0000065c*/ MOVW        R1, #12889                  @ 0x3259 
    /*00000660*/ BL          mn34220_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #20 
    /*0000066c*/ MOVW        R1, #12890                  @ 0x325a 
    /*00000670*/ BL          mn34220_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #12 
    /*0000067c*/ MOVW        R1, #12914                  @ 0x3272 
    /*00000680*/ BL          mn34220_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #48                     @ 0x30 
    /*0000068c*/ MOV         R1, #12928                  @ 0x3280 
    /*00000690*/ BL          mn34220_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #14 
    /*0000069c*/ MOVW        R1, #12930                  @ 0x3282 
    /*000006a0*/ BL          mn34220_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #27 
    /*000006ac*/ MOVW        R1, #12933                  @ 0x3285 
    /*000006b0*/ BL          mn34220_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #1 
    /*000006bc*/ MOVW        R1, #12936                  @ 0x3288 
    /*000006c0*/ BL          mn34220_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #0 
    /*000006cc*/ MOVW        R1, #12937                  @ 0x3289 
    /*000006d0*/ BL          mn34220_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #5 
    /*000006dc*/ MOVW        R1, #13070                  @ 0x330e 
    /*000006e0*/ BL          mn34220_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #2 
    /*000006ec*/ MOVW        R1, #13072                  @ 0x3310 
    /*000006f0*/ BL          mn34220_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #31 
    /*000006fc*/ MOVW        R1, #13077                  @ 0x3315 
    /*00000700*/ BL          mn34220_write_register
    /*00000704*/ MOV         R0, R4 
    /*00000708*/ MOV         R2, #3 
    /*0000070c*/ MOVW        R1, #13082                  @ 0x331a 
    /*00000710*/ BL          mn34220_write_register
    /*00000714*/ MOV         R0, R4 
    /*00000718*/ MOV         R2, #3 
    /*0000071c*/ MOVW        R1, #13083                  @ 0x331b 
    /*00000720*/ BL          mn34220_write_register
    /*00000724*/ MOV         R0, R4 
    /*00000728*/ MOV         R2, #0 
    /*0000072c*/ MOVW        R1, #13100                  @ 0x332c 
    /*00000730*/ BL          mn34220_write_register
    /*00000734*/ MOV         R0, R4 
    /*00000738*/ MOV         R2, #3 
    /*0000073c*/ MOVW        R1, #13113                  @ 0x3339 
    /*00000740*/ BL          mn34220_write_register
    /*00000744*/ MOV         R0, R4 
    /*00000748*/ MOV         R2, #2 
    /*0000074c*/ MOVW        R1, #13163                  @ 0x336b 
    /*00000750*/ BL          mn34220_write_register
    /*00000754*/ MOV         R0, R4 
    /*00000758*/ MOV         R2, #1 
    /*0000075c*/ MOVW        R1, #13215                  @ 0x339f 
    /*00000760*/ BL          mn34220_write_register
    /*00000764*/ MOV         R0, R4 
    /*00000768*/ MOV         R2, #1 
    /*0000076c*/ MOVW        R1, #13218                  @ 0x33a2 
    /*00000770*/ BL          mn34220_write_register
    /*00000774*/ MOV         R0, R4 
    /*00000778*/ MOV         R2, #1 
    /*0000077c*/ MOVW        R1, #13219                  @ 0x33a3 
    /*00000780*/ BL          mn34220_write_register
    /*00000784*/ MOV         R0, R4 
    /*00000788*/ MOV         R2, #0 
    /*0000078c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000790*/ BL          mn34220_write_register
    /*00000794*/ MOV         R0, R4 
    /*00000798*/ MOV         R2, #211                    @ 0xd3 
    /*0000079c*/ MOVW        R1, #12289                  @ 0x3001 
    /*000007a0*/ BL          mn34220_write_register
    /*000007a4*/ MOV         R0, R4 
    /*000007a8*/ MOV         R2, #1 
    /*000007ac*/ MOV         R1, #256                    @ 0x100 
    /*000007b0*/ BL          mn34220_write_register
    /*000007b4*/ MOV         R0, R4 
    /*000007b8*/ MOV         R2, #0 
    /*000007bc*/ MOVW        R1, #257                    @ 0x101 
    /*000007c0*/ BL          mn34220_write_register
    /*000007c4*/ LDR         R0, WORD_07d4               @ LDR         R0, [PC, #8]                @ 0x00000000000007d4 
    /*000007c8*/ POP         {R4, LR} 
    /*000007cc*/ ADD         R0, PC, R0 
    mn34220_linear_720p120_init_x7d0:
    /*000007d0*/ B           mn34220_linear_720p120_init_x7d0@ puts [R_ARM_JUMP24: +0x0] 
    /*000007d4*/ WORD_07d4: .word 0x00000000
FUNC_END mn34220_linear_720p120_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_linear_720p30_init
@ Size: 0x3278
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_linear_720p30_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #0 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOVW        R1, #12322                  @ 0x3022 
    /*00000010*/ BL          mn34220_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #0 
    /*0000001c*/ MOVW        R1, #12323                  @ 0x3023 
    /*00000020*/ BL          mn34220_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #1 
    /*0000002c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000030*/ BL          mn34220_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #0 
    /*0000003c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000040*/ BL          mn34220_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #6 
    /*0000004c*/ MOVW        R1, #12290                  @ 0x3002 
    /*00000050*/ BL          mn34220_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #24 
    /*0000005c*/ MOVW        R1, #12291                  @ 0x3003 
    /*00000060*/ BL          mn34220_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #0 
    /*0000006c*/ MOV         R1, #772                    @ 0x304 
    /*00000070*/ BL          mn34220_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #2 
    /*0000007c*/ MOVW        R1, #773                    @ 0x305 
    /*00000080*/ BL          mn34220_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #0 
    /*0000008c*/ MOVW        R1, #774                    @ 0x306 
    /*00000090*/ BL          mn34220_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #36                     @ 0x24 
    /*0000009c*/ MOVW        R1, #775                    @ 0x307 
    /*000000a0*/ BL          mn34220_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #0 
    /*000000ac*/ MOV         R1, #12288                  @ 0x3000 
    /*000000b0*/ BL          mn34220_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #3 
    /*000000bc*/ MOVW        R1, #12289                  @ 0x3001 
    /*000000c0*/ BL          mn34220_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #0 
    /*000000cc*/ MOVW        R1, #258                    @ 0x102 
    /*000000d0*/ BL          mn34220_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #0 
    /*000000dc*/ MOVW        R1, #259                    @ 0x103 
    /*000000e0*/ BL          mn34220_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #0 
    /*000000ec*/ MOV         R1, #260                    @ 0x104 
    /*000000f0*/ BL          mn34220_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #0 
    /*000000fc*/ MOVW        R1, #261                    @ 0x105 
    /*00000100*/ BL          mn34220_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #0 
    /*0000010c*/ MOV         R1, #272                    @ 0x110 
    /*00000110*/ BL          mn34220_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #0 
    /*0000011c*/ MOVW        R1, #273                    @ 0x111 
    /*00000120*/ BL          mn34220_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #12 
    /*0000012c*/ MOVW        R1, #274                    @ 0x112 
    /*00000130*/ BL          mn34220_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #12 
    /*0000013c*/ MOVW        R1, #275                    @ 0x113 
    /*00000140*/ BL          mn34220_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #0 
    /*0000014c*/ MOV         R1, #288                    @ 0x120 
    /*00000150*/ BL          mn34220_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #0 
    /*0000015c*/ MOVW        R1, #289                    @ 0x121 
    /*00000160*/ BL          mn34220_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #0 
    /*0000016c*/ MOV         R1, #768                    @ 0x300 
    /*00000170*/ BL          mn34220_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #16 
    /*0000017c*/ MOVW        R1, #769                    @ 0x301 
    /*00000180*/ BL          mn34220_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #0 
    /*0000018c*/ MOVW        R1, #770                    @ 0x302 
    /*00000190*/ BL          mn34220_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #1 
    /*0000019c*/ MOVW        R1, #771                    @ 0x303 
    /*000001a0*/ BL          mn34220_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #3 
    /*000001ac*/ MOVW        R1, #12292                  @ 0x3004 
    /*000001b0*/ BL          mn34220_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #100                    @ 0x64 
    /*000001bc*/ MOVW        R1, #12293                  @ 0x3005 
    /*000001c0*/ BL          mn34220_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #0 
    /*000001cc*/ MOVW        R1, #12294                  @ 0x3006 
    /*000001d0*/ BL          mn34220_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #20 
    /*000001dc*/ MOVW        R1, #12295                  @ 0x3007 
    /*000001e0*/ BL          mn34220_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #144                    @ 0x90 
    /*000001ec*/ MOVW        R1, #12296                  @ 0x3008 
    /*000001f0*/ BL          mn34220_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #6 
    /*000001fc*/ MOVW        R1, #12297                  @ 0x3009 
    /*00000200*/ BL          mn34220_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #255                    @ 0xff 
    /*0000020c*/ MOVW        R1, #12298                  @ 0x300a 
    /*00000210*/ BL          mn34220_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #0 
    /*0000021c*/ MOVW        R1, #12299                  @ 0x300b 
    /*00000220*/ BL          mn34220_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #0 
    /*0000022c*/ MOVW        R1, #12300                  @ 0x300c 
    /*00000230*/ BL          mn34220_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #1 
    /*0000023c*/ MOVW        R1, #12301                  @ 0x300d 
    /*00000240*/ BL          mn34220_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #0 
    /*0000024c*/ MOVW        R1, #12304                  @ 0x3010 
    /*00000250*/ BL          mn34220_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #0 
    /*0000025c*/ MOVW        R1, #12305                  @ 0x3011 
    /*00000260*/ BL          mn34220_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #0 
    /*0000026c*/ MOVW        R1, #12306                  @ 0x3012 
    /*00000270*/ BL          mn34220_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #0 
    /*0000027c*/ MOVW        R1, #12307                  @ 0x3013 
    /*00000280*/ BL          mn34220_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #0 
    /*0000028c*/ MOVW        R1, #12308                  @ 0x3014 
    /*00000290*/ BL          mn34220_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #1 
    /*0000029c*/ MOVW        R1, #12309                  @ 0x3015 
    /*000002a0*/ BL          mn34220_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #0 
    /*000002ac*/ MOVW        R1, #12310                  @ 0x3016 
    /*000002b0*/ BL          mn34220_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #0 
    /*000002bc*/ MOVW        R1, #12311                  @ 0x3017 
    /*000002c0*/ BL          mn34220_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #67                     @ 0x43 
    /*000002cc*/ MOVW        R1, #12312                  @ 0x3018 
    /*000002d0*/ BL          mn34220_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #4 
    /*000002dc*/ MOVW        R1, #12313                  @ 0x3019 
    /*000002e0*/ BL          mn34220_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #57                     @ 0x39 
    /*000002ec*/ MOVW        R1, #12314                  @ 0x301a 
    /*000002f0*/ BL          mn34220_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #0 
    /*000002fc*/ MOVW        R1, #12315                  @ 0x301b 
    /*00000300*/ BL          mn34220_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #44                     @ 0x2c 
    /*0000030c*/ MOVW        R1, #12316                  @ 0x301c 
    /*00000310*/ BL          mn34220_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #27 
    /*0000031c*/ MOVW        R1, #12317                  @ 0x301d 
    /*00000320*/ BL          mn34220_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #13 
    /*0000032c*/ MOVW        R1, #12318                  @ 0x301e 
    /*00000330*/ BL          mn34220_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #31 
    /*0000033c*/ MOVW        R1, #12319                  @ 0x301f 
    /*00000340*/ BL          mn34220_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #8 
    /*0000034c*/ MOVW        R1, #12320                  @ 0x3020 
    /*00000350*/ BL          mn34220_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #0 
    /*0000035c*/ MOVW        R1, #12321                  @ 0x3021 
    /*00000360*/ BL          mn34220_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #0 
    /*0000036c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000370*/ BL          mn34220_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #83                     @ 0x53 
    /*0000037c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000380*/ BL          mn34220_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #0 
    /*0000038c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000390*/ BL          mn34220_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #0 
    /*0000039c*/ MOVW        R1, #12303                  @ 0x300f 
    /*000003a0*/ BL          mn34220_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #0 
    /*000003ac*/ MOV         R1, #255                    @ 0xff 
    /*000003b0*/ BL          mn34220_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #0 
    /*000003bc*/ MOV         R1, #255                    @ 0xff 
    /*000003c0*/ BL          mn34220_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #0 
    /*000003cc*/ MOV         R1, #255                    @ 0xff 
    /*000003d0*/ BL          mn34220_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #0 
    /*000003dc*/ MOV         R1, #255                    @ 0xff 
    /*000003e0*/ BL          mn34220_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #0 
    /*000003ec*/ MOV         R1, #255                    @ 0xff 
    /*000003f0*/ BL          mn34220_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #0 
    /*000003fc*/ MOV         R1, #255                    @ 0xff 
    /*00000400*/ BL          mn34220_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #0 
    /*0000040c*/ MOV         R1, #255                    @ 0xff 
    /*00000410*/ BL          mn34220_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #0 
    /*0000041c*/ MOV         R1, #255                    @ 0xff 
    /*00000420*/ BL          mn34220_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #0 
    /*0000042c*/ MOV         R1, #255                    @ 0xff 
    /*00000430*/ BL          mn34220_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #0 
    /*0000043c*/ MOV         R1, #255                    @ 0xff 
    /*00000440*/ BL          mn34220_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #2 
    /*0000044c*/ MOVW        R1, #514                    @ 0x202 
    /*00000450*/ BL          mn34220_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #236                    @ 0xec 
    /*0000045c*/ MOVW        R1, #515                    @ 0x203 
    /*00000460*/ BL          mn34220_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #1 
    /*0000046c*/ MOV         R1, #516                    @ 0x204 
    /*00000470*/ BL          mn34220_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #0 
    /*0000047c*/ MOVW        R1, #517                    @ 0x205 
    /*00000480*/ BL          mn34220_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #1 
    /*0000048c*/ MOVW        R1, #526                    @ 0x20e 
    /*00000490*/ BL          mn34220_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #0 
    /*0000049c*/ MOVW        R1, #527                    @ 0x20f 
    /*000004a0*/ BL          mn34220_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #1 
    /*000004ac*/ MOV         R1, #528                    @ 0x210 
    /*000004b0*/ BL          mn34220_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #0 
    /*000004bc*/ MOVW        R1, #529                    @ 0x211 
    /*000004c0*/ BL          mn34220_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #1 
    /*000004cc*/ MOVW        R1, #530                    @ 0x212 
    /*000004d0*/ BL          mn34220_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #0 
    /*000004dc*/ MOVW        R1, #531                    @ 0x213 
    /*000004e0*/ BL          mn34220_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #1 
    /*000004ec*/ MOV         R1, #532                    @ 0x214 
    /*000004f0*/ BL          mn34220_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #0 
    /*000004fc*/ MOVW        R1, #533                    @ 0x215 
    /*00000500*/ BL          mn34220_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #0 
    /*0000050c*/ MOV         R1, #544                    @ 0x220 
    /*00000510*/ BL          mn34220_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #0 
    /*0000051c*/ MOVW        R1, #545                    @ 0x221 
    /*00000520*/ BL          mn34220_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #0 
    /*0000052c*/ MOVW        R1, #830                    @ 0x33e 
    /*00000530*/ BL          mn34220_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #0 
    /*0000053c*/ MOVW        R1, #831                    @ 0x33f 
    /*00000540*/ BL          mn34220_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #2 
    /*0000054c*/ MOV         R1, #832                    @ 0x340 
    /*00000550*/ BL          mn34220_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #238                    @ 0xee 
    /*0000055c*/ MOVW        R1, #833                    @ 0x341 
    /*00000560*/ BL          mn34220_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #12 
    /*0000056c*/ MOVW        R1, #834                    @ 0x342 
    /*00000570*/ BL          mn34220_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #228                    @ 0xe4 
    /*0000057c*/ MOVW        R1, #835                    @ 0x343 
    /*00000580*/ BL          mn34220_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #0 
    /*0000058c*/ MOV         R1, #836                    @ 0x344 
    /*00000590*/ BL          mn34220_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #0 
    /*0000059c*/ MOVW        R1, #837                    @ 0x345 
    /*000005a0*/ BL          mn34220_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #0 
    /*000005ac*/ MOVW        R1, #838                    @ 0x346 
    /*000005b0*/ BL          mn34220_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #242                    @ 0xf2 
    /*000005bc*/ MOVW        R1, #839                    @ 0x347 
    /*000005c0*/ BL          mn34220_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #0 
    /*000005cc*/ MOV         R1, #840                    @ 0x348 
    /*000005d0*/ BL          mn34220_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #0 
    /*000005dc*/ MOVW        R1, #841                    @ 0x349 
    /*000005e0*/ BL          mn34220_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #3 
    /*000005ec*/ MOVW        R1, #842                    @ 0x34a 
    /*000005f0*/ BL          mn34220_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #201                    @ 0xc9 
    /*000005fc*/ MOVW        R1, #843                    @ 0x34b 
    /*00000600*/ BL          mn34220_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #0 
    /*0000060c*/ MOV         R1, #844                    @ 0x34c 
    /*00000610*/ BL          mn34220_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #0 
    /*0000061c*/ MOVW        R1, #845                    @ 0x34d 
    /*00000620*/ BL          mn34220_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #2 
    /*0000062c*/ MOVW        R1, #846                    @ 0x34e 
    /*00000630*/ BL          mn34220_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #216                    @ 0xd8 
    /*0000063c*/ MOVW        R1, #847                    @ 0x34f 
    /*00000640*/ BL          mn34220_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #0 
    /*0000064c*/ MOV         R1, #896                    @ 0x380 
    /*00000650*/ BL          mn34220_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #1 
    /*0000065c*/ MOVW        R1, #897                    @ 0x381 
    /*00000660*/ BL          mn34220_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #0 
    /*0000066c*/ MOVW        R1, #898                    @ 0x382 
    /*00000670*/ BL          mn34220_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #1 
    /*0000067c*/ MOVW        R1, #899                    @ 0x383 
    /*00000680*/ BL          mn34220_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #0 
    /*0000068c*/ MOV         R1, #900                    @ 0x384 
    /*00000690*/ BL          mn34220_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #1 
    /*0000069c*/ MOVW        R1, #901                    @ 0x385 
    /*000006a0*/ BL          mn34220_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #0 
    /*000006ac*/ MOVW        R1, #902                    @ 0x386 
    /*000006b0*/ BL          mn34220_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #1 
    /*000006bc*/ MOVW        R1, #903                    @ 0x387 
    /*000006c0*/ BL          mn34220_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #0 
    /*000006cc*/ MOV         R1, #1280                   @ 0x500 
    /*000006d0*/ BL          mn34220_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #0 
    /*000006dc*/ MOVW        R1, #1281                   @ 0x501 
    /*000006e0*/ BL          mn34220_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #0 
    /*000006ec*/ MOV         R1, #1536                   @ 0x600 
    /*000006f0*/ BL          mn34220_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #0 
    /*000006fc*/ MOVW        R1, #1537                   @ 0x601 
    /*00000700*/ BL          mn34220_write_register
    /*00000704*/ MOV         R0, R4 
    /*00000708*/ MOV         R2, #0 
    /*0000070c*/ MOVW        R1, #1538                   @ 0x602 
    /*00000710*/ BL          mn34220_write_register
    /*00000714*/ MOV         R0, R4 
    /*00000718*/ MOV         R2, #0 
    /*0000071c*/ MOVW        R1, #1539                   @ 0x603 
    /*00000720*/ BL          mn34220_write_register
    /*00000724*/ MOV         R0, R4 
    /*00000728*/ MOV         R2, #0 
    /*0000072c*/ MOVW        R1, #1540                   @ 0x604 
    /*00000730*/ BL          mn34220_write_register
    /*00000734*/ MOV         R0, R4 
    /*00000738*/ MOV         R2, #0 
    /*0000073c*/ MOVW        R1, #1541                   @ 0x605 
    /*00000740*/ BL          mn34220_write_register
    /*00000744*/ MOV         R0, R4 
    /*00000748*/ MOV         R2, #0 
    /*0000074c*/ MOVW        R1, #1542                   @ 0x606 
    /*00000750*/ BL          mn34220_write_register
    /*00000754*/ MOV         R0, R4 
    /*00000758*/ MOV         R2, #0 
    /*0000075c*/ MOVW        R1, #1543                   @ 0x607 
    /*00000760*/ BL          mn34220_write_register
    /*00000764*/ MOV         R0, R4 
    /*00000768*/ MOV         R2, #0 
    /*0000076c*/ MOVW        R1, #1544                   @ 0x608 
    /*00000770*/ BL          mn34220_write_register
    /*00000774*/ MOV         R0, R4 
    /*00000778*/ MOV         R2, #0 
    /*0000077c*/ MOVW        R1, #1545                   @ 0x609 
    /*00000780*/ BL          mn34220_write_register
    /*00000784*/ MOV         R0, R4 
    /*00000788*/ MOV         R2, #0 
    /*0000078c*/ MOVW        R1, #1546                   @ 0x60a 
    /*00000790*/ BL          mn34220_write_register
    /*00000794*/ MOV         R0, R4 
    /*00000798*/ MOV         R2, #0 
    /*0000079c*/ MOVW        R1, #1547                   @ 0x60b 
    /*000007a0*/ BL          mn34220_write_register
    /*000007a4*/ MOV         R0, R4 
    /*000007a8*/ MOV         R2, #0 
    /*000007ac*/ MOVW        R1, #1548                   @ 0x60c 
    /*000007b0*/ BL          mn34220_write_register
    /*000007b4*/ MOV         R0, R4 
    /*000007b8*/ MOV         R2, #0 
    /*000007bc*/ MOVW        R1, #1549                   @ 0x60d 
    /*000007c0*/ BL          mn34220_write_register
    /*000007c4*/ MOV         R0, R4 
    /*000007c8*/ MOV         R2, #0 
    /*000007cc*/ MOVW        R1, #1550                   @ 0x60e 
    /*000007d0*/ BL          mn34220_write_register
    /*000007d4*/ MOV         R0, R4 
    /*000007d8*/ MOV         R2, #0 
    /*000007dc*/ MOVW        R1, #1551                   @ 0x60f 
    /*000007e0*/ BL          mn34220_write_register
    /*000007e4*/ MOV         R0, R4 
    /*000007e8*/ MOV         R2, #0 
    /*000007ec*/ MOV         R1, #1552                   @ 0x610 
    /*000007f0*/ BL          mn34220_write_register
    /*000007f4*/ MOV         R0, R4 
    /*000007f8*/ MOV         R2, #0 
    /*000007fc*/ MOVW        R1, #1553                   @ 0x611 
    /*00000800*/ BL          mn34220_write_register
    /*00000804*/ MOV         R0, R4 
    /*00000808*/ MOV         R2, #0 
    /*0000080c*/ MOVW        R1, #12336                  @ 0x3030 
    /*00000810*/ BL          mn34220_write_register
    /*00000814*/ MOV         R0, R4 
    /*00000818*/ MOV         R2, #0 
    /*0000081c*/ MOVW        R1, #12337                  @ 0x3031 
    /*00000820*/ BL          mn34220_write_register
    /*00000824*/ MOV         R0, R4 
    /*00000828*/ MOV         R2, #0 
    /*0000082c*/ MOVW        R1, #12338                  @ 0x3032 
    /*00000830*/ BL          mn34220_write_register
    /*00000834*/ MOV         R0, R4 
    /*00000838*/ MOV         R2, #0 
    /*0000083c*/ MOVW        R1, #12339                  @ 0x3033 
    /*00000840*/ BL          mn34220_write_register
    /*00000844*/ MOV         R0, R4 
    /*00000848*/ MOV         R2, #0 
    /*0000084c*/ MOVW        R1, #12340                  @ 0x3034 
    /*00000850*/ BL          mn34220_write_register
    /*00000854*/ MOV         R0, R4 
    /*00000858*/ MOV         R2, #0 
    /*0000085c*/ MOVW        R1, #12341                  @ 0x3035 
    /*00000860*/ BL          mn34220_write_register
    /*00000864*/ MOV         R0, R4 
    /*00000868*/ MOV         R2, #0 
    /*0000086c*/ MOVW        R1, #12342                  @ 0x3036 
    /*00000870*/ BL          mn34220_write_register
    /*00000874*/ MOV         R0, R4 
    /*00000878*/ MOV         R2, #16 
    /*0000087c*/ MOVW        R1, #12343                  @ 0x3037 
    /*00000880*/ BL          mn34220_write_register
    /*00000884*/ MOV         R0, R4 
    /*00000888*/ MOV         R2, #255                    @ 0xff 
    /*0000088c*/ MOVW        R1, #12344                  @ 0x3038 
    /*00000890*/ BL          mn34220_write_register
    /*00000894*/ MOV         R0, R4 
    /*00000898*/ MOV         R2, #36                     @ 0x24 
    /*0000089c*/ MOVW        R1, #12345                  @ 0x3039 
    /*000008a0*/ BL          mn34220_write_register
    /*000008a4*/ MOV         R0, R4 
    /*000008a8*/ MOV         R2, #0 
    /*000008ac*/ MOVW        R1, #12346                  @ 0x303a 
    /*000008b0*/ BL          mn34220_write_register
    /*000008b4*/ MOV         R0, R4 
    /*000008b8*/ MOV         R2, #0 
    /*000008bc*/ MOVW        R1, #12347                  @ 0x303b 
    /*000008c0*/ BL          mn34220_write_register
    /*000008c4*/ MOV         R0, R4 
    /*000008c8*/ MOV         R2, #0 
    /*000008cc*/ MOVW        R1, #12348                  @ 0x303c 
    /*000008d0*/ BL          mn34220_write_register
    /*000008d4*/ MOV         R0, R4 
    /*000008d8*/ MOV         R2, #40                     @ 0x28 
    /*000008dc*/ MOVW        R1, #12349                  @ 0x303d 
    /*000008e0*/ BL          mn34220_write_register
    /*000008e4*/ MOV         R0, R4 
    /*000008e8*/ MOV         R2, #3 
    /*000008ec*/ MOVW        R1, #12350                  @ 0x303e 
    /*000008f0*/ BL          mn34220_write_register
    /*000008f4*/ MOV         R0, R4 
    /*000008f8*/ MOV         R2, #3 
    /*000008fc*/ MOVW        R1, #12351                  @ 0x303f 
    /*00000900*/ BL          mn34220_write_register
    /*00000904*/ MOV         R0, R4 
    /*00000908*/ MOV         R2, #192                    @ 0xc0 
    /*0000090c*/ MOV         R1, #12352                  @ 0x3040 
    /*00000910*/ BL          mn34220_write_register
    /*00000914*/ MOV         R0, R4 
    /*00000918*/ MOV         R2, #44                     @ 0x2c 
    /*0000091c*/ MOVW        R1, #12353                  @ 0x3041 
    /*00000920*/ BL          mn34220_write_register
    /*00000924*/ MOV         R0, R4 
    /*00000928*/ MOV         R2, #80                     @ 0x50 
    /*0000092c*/ MOVW        R1, #12354                  @ 0x3042 
    /*00000930*/ BL          mn34220_write_register
    /*00000934*/ MOV         R0, R4 
    /*00000938*/ MOV         R2, #16 
    /*0000093c*/ MOVW        R1, #12355                  @ 0x3043 
    /*00000940*/ BL          mn34220_write_register
    /*00000944*/ MOV         R0, R4 
    /*00000948*/ MOV         R2, #0 
    /*0000094c*/ MOVW        R1, #12356                  @ 0x3044 
    /*00000950*/ BL          mn34220_write_register
    /*00000954*/ MOV         R0, R4 
    /*00000958*/ MOV         R2, #0 
    /*0000095c*/ MOVW        R1, #12357                  @ 0x3045 
    /*00000960*/ BL          mn34220_write_register
    /*00000964*/ MOV         R0, R4 
    /*00000968*/ MOV         R2, #0 
    /*0000096c*/ MOVW        R1, #12368                  @ 0x3050 
    /*00000970*/ BL          mn34220_write_register
    /*00000974*/ MOV         R0, R4 
    /*00000978*/ MOV         R2, #0 
    /*0000097c*/ MOVW        R1, #12369                  @ 0x3051 
    /*00000980*/ BL          mn34220_write_register
    /*00000984*/ MOV         R0, R4 
    /*00000988*/ MOV         R2, #144                    @ 0x90 
    /*0000098c*/ MOVW        R1, #12370                  @ 0x3052 
    /*00000990*/ BL          mn34220_write_register
    /*00000994*/ MOV         R0, R4 
    /*00000998*/ MOV         R2, #34                     @ 0x22 
    /*0000099c*/ MOVW        R1, #12371                  @ 0x3053 
    /*000009a0*/ BL          mn34220_write_register
    /*000009a4*/ MOV         R0, R4 
    /*000009a8*/ MOV         R2, #48                     @ 0x30 
    /*000009ac*/ MOVW        R1, #12372                  @ 0x3054 
    /*000009b0*/ BL          mn34220_write_register
    /*000009b4*/ MOV         R0, R4 
    /*000009b8*/ MOV         R2, #0 
    /*000009bc*/ MOVW        R1, #12373                  @ 0x3055 
    /*000009c0*/ BL          mn34220_write_register
    /*000009c4*/ MOV         R0, R4 
    /*000009c8*/ MOV         R2, #1 
    /*000009cc*/ MOVW        R1, #12374                  @ 0x3056 
    /*000009d0*/ BL          mn34220_write_register
    /*000009d4*/ MOV         R0, R4 
    /*000009d8*/ MOV         R2, #255                    @ 0xff 
    /*000009dc*/ MOVW        R1, #12375                  @ 0x3057 
    /*000009e0*/ BL          mn34220_write_register
    /*000009e4*/ MOV         R0, R4 
    /*000009e8*/ MOV         R2, #15 
    /*000009ec*/ MOVW        R1, #12376                  @ 0x3058 
    /*000009f0*/ BL          mn34220_write_register
    /*000009f4*/ MOV         R0, R4 
    /*000009f8*/ MOV         R2, #254                    @ 0xfe 
    /*000009fc*/ MOVW        R1, #12377                  @ 0x3059 
    /*00000a00*/ BL          mn34220_write_register
    /*00000a04*/ MOV         R0, R4 
    /*00000a08*/ MOV         R2, #0 
    /*00000a0c*/ MOVW        R1, #12378                  @ 0x305a 
    /*00000a10*/ BL          mn34220_write_register
    /*00000a14*/ MOV         R0, R4 
    /*00000a18*/ MOV         R2, #1 
    /*00000a1c*/ MOVW        R1, #12379                  @ 0x305b 
    /*00000a20*/ BL          mn34220_write_register
    /*00000a24*/ MOV         R0, R4 
    /*00000a28*/ MOV         R2, #0 
    /*00000a2c*/ MOVW        R1, #12380                  @ 0x305c 
    /*00000a30*/ BL          mn34220_write_register
    /*00000a34*/ MOV         R0, R4 
    /*00000a38*/ MOV         R2, #49                     @ 0x31 
    /*00000a3c*/ MOVW        R1, #12381                  @ 0x305d 
    /*00000a40*/ BL          mn34220_write_register
    /*00000a44*/ MOV         R0, R4 
    /*00000a48*/ MOV         R2, #0 
    /*00000a4c*/ MOVW        R1, #12382                  @ 0x305e 
    /*00000a50*/ BL          mn34220_write_register
    /*00000a54*/ MOV         R0, R4 
    /*00000a58*/ MOV         R2, #0 
    /*00000a5c*/ MOVW        R1, #12383                  @ 0x305f 
    /*00000a60*/ BL          mn34220_write_register
    /*00000a64*/ MOV         R0, R4 
    /*00000a68*/ MOV         R2, #0 
    /*00000a6c*/ MOVW        R1, #12384                  @ 0x3060 
    /*00000a70*/ BL          mn34220_write_register
    /*00000a74*/ MOV         R0, R4 
    /*00000a78*/ MOV         R2, #0 
    /*00000a7c*/ MOVW        R1, #12385                  @ 0x3061 
    /*00000a80*/ BL          mn34220_write_register
    /*00000a84*/ MOV         R0, R4 
    /*00000a88*/ MOV         R2, #32 
    /*00000a8c*/ MOVW        R1, #12386                  @ 0x3062 
    /*00000a90*/ BL          mn34220_write_register
    /*00000a94*/ MOV         R0, R4 
    /*00000a98*/ MOV         R2, #20 
    /*00000a9c*/ MOVW        R1, #12387                  @ 0x3063 
    /*00000aa0*/ BL          mn34220_write_register
    /*00000aa4*/ MOV         R0, R4 
    /*00000aa8*/ MOV         R2, #16 
    /*00000aac*/ MOVW        R1, #12388                  @ 0x3064 
    /*00000ab0*/ BL          mn34220_write_register
    /*00000ab4*/ MOV         R0, R4 
    /*00000ab8*/ MOV         R2, #50                     @ 0x32 
    /*00000abc*/ MOVW        R1, #12389                  @ 0x3065 
    /*00000ac0*/ BL          mn34220_write_register
    /*00000ac4*/ MOV         R0, R4 
    /*00000ac8*/ MOV         R2, #0 
    /*00000acc*/ MOVW        R1, #12390                  @ 0x3066 
    /*00000ad0*/ BL          mn34220_write_register
    /*00000ad4*/ MOV         R0, R4 
    /*00000ad8*/ MOV         R2, #2 
    /*00000adc*/ MOVW        R1, #12391                  @ 0x3067 
    /*00000ae0*/ BL          mn34220_write_register
    /*00000ae4*/ MOV         R0, R4 
    /*00000ae8*/ MOV         R2, #0 
    /*00000aec*/ MOVW        R1, #12392                  @ 0x3068 
    /*00000af0*/ BL          mn34220_write_register
    /*00000af4*/ MOV         R0, R4 
    /*00000af8*/ MOV         R2, #0 
    /*00000afc*/ MOVW        R1, #12393                  @ 0x3069 
    /*00000b00*/ BL          mn34220_write_register
    /*00000b04*/ MOV         R0, R4 
    /*00000b08*/ MOV         R2, #0 
    /*00000b0c*/ MOVW        R1, #12394                  @ 0x306a 
    /*00000b10*/ BL          mn34220_write_register
    /*00000b14*/ MOV         R0, R4 
    /*00000b18*/ MOV         R2, #3 
    /*00000b1c*/ MOVW        R1, #12395                  @ 0x306b 
    /*00000b20*/ BL          mn34220_write_register
    /*00000b24*/ MOV         R0, R4 
    /*00000b28*/ MOV         R2, #0 
    /*00000b2c*/ MOVW        R1, #12396                  @ 0x306c 
    /*00000b30*/ BL          mn34220_write_register
    /*00000b34*/ MOV         R0, R4 
    /*00000b38*/ MOV         R2, #1 
    /*00000b3c*/ MOVW        R1, #12397                  @ 0x306d 
    /*00000b40*/ BL          mn34220_write_register
    /*00000b44*/ MOV         R0, R4 
    /*00000b48*/ MOV         R2, #63                     @ 0x3f 
    /*00000b4c*/ MOVW        R1, #12398                  @ 0x306e 
    /*00000b50*/ BL          mn34220_write_register
    /*00000b54*/ MOV         R0, R4 
    /*00000b58*/ MOV         R2, #255                    @ 0xff 
    /*00000b5c*/ MOVW        R1, #12399                  @ 0x306f 
    /*00000b60*/ BL          mn34220_write_register
    /*00000b64*/ MOV         R0, R4 
    /*00000b68*/ MOV         R2, #63                     @ 0x3f 
    /*00000b6c*/ MOVW        R1, #12400                  @ 0x3070 
    /*00000b70*/ BL          mn34220_write_register
    /*00000b74*/ MOV         R0, R4 
    /*00000b78*/ MOV         R2, #255                    @ 0xff 
    /*00000b7c*/ MOVW        R1, #12401                  @ 0x3071 
    /*00000b80*/ BL          mn34220_write_register
    /*00000b84*/ MOV         R0, R4 
    /*00000b88*/ MOV         R2, #63                     @ 0x3f 
    /*00000b8c*/ MOVW        R1, #12402                  @ 0x3072 
    /*00000b90*/ BL          mn34220_write_register
    /*00000b94*/ MOV         R0, R4 
    /*00000b98*/ MOV         R2, #255                    @ 0xff 
    /*00000b9c*/ MOVW        R1, #12403                  @ 0x3073 
    /*00000ba0*/ BL          mn34220_write_register
    /*00000ba4*/ MOV         R0, R4 
    /*00000ba8*/ MOV         R2, #0 
    /*00000bac*/ MOVW        R1, #12404                  @ 0x3074 
    /*00000bb0*/ BL          mn34220_write_register
    /*00000bb4*/ MOV         R0, R4 
    /*00000bb8*/ MOV         R2, #1 
    /*00000bbc*/ MOVW        R1, #12405                  @ 0x3075 
    /*00000bc0*/ BL          mn34220_write_register
    /*00000bc4*/ MOV         R0, R4 
    /*00000bc8*/ MOV         R2, #66                     @ 0x42 
    /*00000bcc*/ MOVW        R1, #12406                  @ 0x3076 
    /*00000bd0*/ BL          mn34220_write_register
    /*00000bd4*/ MOV         R0, R4 
    /*00000bd8*/ MOV         R2, #0 
    /*00000bdc*/ MOVW        R1, #12407                  @ 0x3077 
    /*00000be0*/ BL          mn34220_write_register
    /*00000be4*/ MOV         R0, R4 
    /*00000be8*/ MOV         R2, #0 
    /*00000bec*/ MOVW        R1, #12408                  @ 0x3078 
    /*00000bf0*/ BL          mn34220_write_register
    /*00000bf4*/ MOV         R0, R4 
    /*00000bf8*/ MOV         R2, #19 
    /*00000bfc*/ MOVW        R1, #12409                  @ 0x3079 
    /*00000c00*/ BL          mn34220_write_register
    /*00000c04*/ MOV         R0, R4 
    /*00000c08*/ MOV         R2, #18 
    /*00000c0c*/ MOVW        R1, #12410                  @ 0x307a 
    /*00000c10*/ BL          mn34220_write_register
    /*00000c14*/ MOV         R0, R4 
    /*00000c18*/ MOV         R2, #48                     @ 0x30 
    /*00000c1c*/ MOVW        R1, #12411                  @ 0x307b 
    /*00000c20*/ BL          mn34220_write_register
    /*00000c24*/ MOV         R0, R4 
    /*00000c28*/ MOV         R2, #43                     @ 0x2b 
    /*00000c2c*/ MOVW        R1, #12412                  @ 0x307c 
    /*00000c30*/ BL          mn34220_write_register
    /*00000c34*/ MOV         R0, R4 
    /*00000c38*/ MOV         R2, #16 
    /*00000c3c*/ MOVW        R1, #12413                  @ 0x307d 
    /*00000c40*/ BL          mn34220_write_register
    /*00000c44*/ MOV         R0, R4 
    /*00000c48*/ MOV         R2, #0 
    /*00000c4c*/ MOVW        R1, #12414                  @ 0x307e 
    /*00000c50*/ BL          mn34220_write_register
    /*00000c54*/ MOV         R0, R4 
    /*00000c58*/ MOV         R2, #0 
    /*00000c5c*/ MOVW        R1, #12415                  @ 0x307f 
    /*00000c60*/ BL          mn34220_write_register
    /*00000c64*/ MOV         R0, R4 
    /*00000c68*/ MOV         R2, #0 
    /*00000c6c*/ MOV         R1, #12416                  @ 0x3080 
    /*00000c70*/ BL          mn34220_write_register
    /*00000c74*/ MOV         R0, R4 
    /*00000c78*/ MOV         R2, #0 
    /*00000c7c*/ MOVW        R1, #12417                  @ 0x3081 
    /*00000c80*/ BL          mn34220_write_register
    /*00000c84*/ MOV         R0, R4 
    /*00000c88*/ MOV         R2, #0 
    /*00000c8c*/ MOVW        R1, #12418                  @ 0x3082 
    /*00000c90*/ BL          mn34220_write_register
    /*00000c94*/ MOV         R0, R4 
    /*00000c98*/ MOV         R2, #0 
    /*00000c9c*/ MOVW        R1, #12419                  @ 0x3083 
    /*00000ca0*/ BL          mn34220_write_register
    /*00000ca4*/ MOV         R0, R4 
    /*00000ca8*/ MOV         R2, #0 
    /*00000cac*/ MOVW        R1, #12420                  @ 0x3084 
    /*00000cb0*/ BL          mn34220_write_register
    /*00000cb4*/ MOV         R0, R4 
    /*00000cb8*/ MOV         R2, #48                     @ 0x30 
    /*00000cbc*/ MOVW        R1, #12421                  @ 0x3085 
    /*00000cc0*/ BL          mn34220_write_register
    /*00000cc4*/ MOV         R0, R4 
    /*00000cc8*/ MOV         R2, #35                     @ 0x23 
    /*00000ccc*/ MOVW        R1, #12422                  @ 0x3086 
    /*00000cd0*/ BL          mn34220_write_register
    /*00000cd4*/ MOV         R0, R4 
    /*00000cd8*/ MOV         R2, #3 
    /*00000cdc*/ MOVW        R1, #12423                  @ 0x3087 
    /*00000ce0*/ BL          mn34220_write_register
    /*00000ce4*/ MOV         R0, R4 
    /*00000ce8*/ MOV         R2, #20 
    /*00000cec*/ MOVW        R1, #12424                  @ 0x3088 
    /*00000cf0*/ BL          mn34220_write_register
    /*00000cf4*/ MOV         R0, R4 
    /*00000cf8*/ MOV         R2, #4 
    /*00000cfc*/ MOVW        R1, #12425                  @ 0x3089 
    /*00000d00*/ BL          mn34220_write_register
    /*00000d04*/ MOV         R0, R4 
    /*00000d08*/ MOV         R2, #1 
    /*00000d0c*/ MOVW        R1, #12426                  @ 0x308a 
    /*00000d10*/ BL          mn34220_write_register
    /*00000d14*/ MOV         R0, R4 
    /*00000d18*/ MOV         R2, #11 
    /*00000d1c*/ MOVW        R1, #12427                  @ 0x308b 
    /*00000d20*/ BL          mn34220_write_register
    /*00000d24*/ MOV         R0, R4 
    /*00000d28*/ MOV         R2, #8 
    /*00000d2c*/ MOVW        R1, #12428                  @ 0x308c 
    /*00000d30*/ BL          mn34220_write_register
    /*00000d34*/ MOV         R0, R4 
    /*00000d38*/ MOV         R2, #4 
    /*00000d3c*/ MOVW        R1, #12429                  @ 0x308d 
    /*00000d40*/ BL          mn34220_write_register
    /*00000d44*/ MOV         R0, R4 
    /*00000d48*/ MOV         R2, #4 
    /*00000d4c*/ MOVW        R1, #12430                  @ 0x308e 
    /*00000d50*/ BL          mn34220_write_register
    /*00000d54*/ MOV         R0, R4 
    /*00000d58*/ MOV         R2, #9 
    /*00000d5c*/ MOVW        R1, #12431                  @ 0x308f 
    /*00000d60*/ BL          mn34220_write_register
    /*00000d64*/ MOV         R0, R4 
    /*00000d68*/ MOV         R2, #5 
    /*00000d6c*/ MOVW        R1, #12432                  @ 0x3090 
    /*00000d70*/ BL          mn34220_write_register
    /*00000d74*/ MOV         R0, R4 
    /*00000d78*/ MOV         R2, #0 
    /*00000d7c*/ MOVW        R1, #12433                  @ 0x3091 
    /*00000d80*/ BL          mn34220_write_register
    /*00000d84*/ MOV         R0, R4 
    /*00000d88*/ MOV         R2, #0 
    /*00000d8c*/ MOVW        R1, #12434                  @ 0x3092 
    /*00000d90*/ BL          mn34220_write_register
    /*00000d94*/ MOV         R0, R4 
    /*00000d98*/ MOV         R2, #9 
    /*00000d9c*/ MOVW        R1, #12435                  @ 0x3093 
    /*00000da0*/ BL          mn34220_write_register
    /*00000da4*/ MOV         R0, R4 
    /*00000da8*/ MOV         R2, #0 
    /*00000dac*/ MOVW        R1, #12436                  @ 0x3094 
    /*00000db0*/ BL          mn34220_write_register
    /*00000db4*/ MOV         R0, R4 
    /*00000db8*/ MOV         R2, #0 
    /*00000dbc*/ MOVW        R1, #12437                  @ 0x3095 
    /*00000dc0*/ BL          mn34220_write_register
    /*00000dc4*/ MOV         R0, R4 
    /*00000dc8*/ MOV         R2, #0 
    /*00000dcc*/ MOVW        R1, #12438                  @ 0x3096 
    /*00000dd0*/ BL          mn34220_write_register
    /*00000dd4*/ MOV         R0, R4 
    /*00000dd8*/ MOV         R2, #0 
    /*00000ddc*/ MOVW        R1, #12439                  @ 0x3097 
    /*00000de0*/ BL          mn34220_write_register
    /*00000de4*/ MOV         R0, R4 
    /*00000de8*/ MOV         R2, #0 
    /*00000dec*/ MOVW        R1, #12440                  @ 0x3098 
    /*00000df0*/ BL          mn34220_write_register
    /*00000df4*/ MOV         R0, R4 
    /*00000df8*/ MOV         R2, #0 
    /*00000dfc*/ MOVW        R1, #12441                  @ 0x3099 
    /*00000e00*/ BL          mn34220_write_register
    /*00000e04*/ MOV         R0, R4 
    /*00000e08*/ MOV         R2, #0 
    /*00000e0c*/ MOVW        R1, #12442                  @ 0x309a 
    /*00000e10*/ BL          mn34220_write_register
    /*00000e14*/ MOV         R0, R4 
    /*00000e18*/ MOV         R2, #0 
    /*00000e1c*/ MOVW        R1, #12443                  @ 0x309b 
    /*00000e20*/ BL          mn34220_write_register
    /*00000e24*/ MOV         R0, R4 
    /*00000e28*/ MOV         R2, #83                     @ 0x53 
    /*00000e2c*/ MOVW        R1, #12444                  @ 0x309c 
    /*00000e30*/ BL          mn34220_write_register
    /*00000e34*/ MOV         R0, R4 
    /*00000e38*/ MOV         R2, #84                     @ 0x54 
    /*00000e3c*/ MOVW        R1, #12445                  @ 0x309d 
    /*00000e40*/ BL          mn34220_write_register
    /*00000e44*/ MOV         R0, R4 
    /*00000e48*/ MOV         R2, #1 
    /*00000e4c*/ MOVW        R1, #12446                  @ 0x309e 
    /*00000e50*/ BL          mn34220_write_register
    /*00000e54*/ MOV         R0, R4 
    /*00000e58*/ MOV         R2, #0 
    /*00000e5c*/ MOVW        R1, #12447                  @ 0x309f 
    /*00000e60*/ BL          mn34220_write_register
    /*00000e64*/ MOV         R0, R4 
    /*00000e68*/ MOV         R2, #0 
    /*00000e6c*/ MOV         R1, #12544                  @ 0x3100 
    /*00000e70*/ BL          mn34220_write_register
    /*00000e74*/ MOV         R0, R4 
    /*00000e78*/ MOV         R2, #0 
    /*00000e7c*/ MOVW        R1, #12545                  @ 0x3101 
    /*00000e80*/ BL          mn34220_write_register
    /*00000e84*/ MOV         R0, R4 
    /*00000e88*/ MOV         R2, #0 
    /*00000e8c*/ MOVW        R1, #12546                  @ 0x3102 
    /*00000e90*/ BL          mn34220_write_register
    /*00000e94*/ MOV         R0, R4 
    /*00000e98*/ MOV         R2, #0 
    /*00000e9c*/ MOVW        R1, #12547                  @ 0x3103 
    /*00000ea0*/ BL          mn34220_write_register
    /*00000ea4*/ MOV         R0, R4 
    /*00000ea8*/ MOV         R2, #4 
    /*00000eac*/ MOVW        R1, #12548                  @ 0x3104 
    /*00000eb0*/ BL          mn34220_write_register
    /*00000eb4*/ MOV         R0, R4 
    /*00000eb8*/ MOV         R2, #0 
    /*00000ebc*/ MOVW        R1, #12549                  @ 0x3105 
    /*00000ec0*/ BL          mn34220_write_register
    /*00000ec4*/ MOV         R0, R4 
    /*00000ec8*/ MOV         R2, #0 
    /*00000ecc*/ MOVW        R1, #12550                  @ 0x3106 
    /*00000ed0*/ BL          mn34220_write_register
    /*00000ed4*/ MOV         R0, R4 
    /*00000ed8*/ MOV         R2, #192                    @ 0xc0 
    /*00000edc*/ MOVW        R1, #12551                  @ 0x3107 
    /*00000ee0*/ BL          mn34220_write_register
    /*00000ee4*/ MOV         R0, R4 
    /*00000ee8*/ MOV         R2, #1 
    /*00000eec*/ MOVW        R1, #12552                  @ 0x3108 
    /*00000ef0*/ BL          mn34220_write_register
    /*00000ef4*/ MOV         R0, R4 
    /*00000ef8*/ MOV         R2, #0 
    /*00000efc*/ MOVW        R1, #12553                  @ 0x3109 
    /*00000f00*/ BL          mn34220_write_register
    /*00000f04*/ MOV         R0, R4 
    /*00000f08*/ MOV         R2, #1 
    /*00000f0c*/ MOVW        R1, #12554                  @ 0x310a 
    /*00000f10*/ BL          mn34220_write_register
    /*00000f14*/ MOV         R0, R4 
    /*00000f18*/ MOV         R2, #0 
    /*00000f1c*/ MOVW        R1, #12555                  @ 0x310b 
    /*00000f20*/ BL          mn34220_write_register
    /*00000f24*/ MOV         R0, R4 
    /*00000f28*/ MOV         R2, #1 
    /*00000f2c*/ MOVW        R1, #12556                  @ 0x310c 
    /*00000f30*/ BL          mn34220_write_register
    /*00000f34*/ MOV         R0, R4 
    /*00000f38*/ MOV         R2, #0 
    /*00000f3c*/ MOVW        R1, #12557                  @ 0x310d 
    /*00000f40*/ BL          mn34220_write_register
    /*00000f44*/ MOV         R0, R4 
    /*00000f48*/ MOV         R2, #1 
    /*00000f4c*/ MOVW        R1, #12558                  @ 0x310e 
    /*00000f50*/ BL          mn34220_write_register
    /*00000f54*/ MOV         R0, R4 
    /*00000f58*/ MOV         R2, #0 
    /*00000f5c*/ MOVW        R1, #12559                  @ 0x310f 
    /*00000f60*/ BL          mn34220_write_register
    /*00000f64*/ MOV         R0, R4 
    /*00000f68*/ MOV         R2, #1 
    /*00000f6c*/ MOVW        R1, #12560                  @ 0x3110 
    /*00000f70*/ BL          mn34220_write_register
    /*00000f74*/ MOV         R0, R4 
    /*00000f78*/ MOV         R2, #0 
    /*00000f7c*/ MOVW        R1, #12561                  @ 0x3111 
    /*00000f80*/ BL          mn34220_write_register
    /*00000f84*/ MOV         R0, R4 
    /*00000f88*/ MOV         R2, #1 
    /*00000f8c*/ MOVW        R1, #12562                  @ 0x3112 
    /*00000f90*/ BL          mn34220_write_register
    /*00000f94*/ MOV         R0, R4 
    /*00000f98*/ MOV         R2, #0 
    /*00000f9c*/ MOVW        R1, #12563                  @ 0x3113 
    /*00000fa0*/ BL          mn34220_write_register
    /*00000fa4*/ MOV         R0, R4 
    /*00000fa8*/ MOV         R2, #1 
    /*00000fac*/ MOVW        R1, #12564                  @ 0x3114 
    /*00000fb0*/ BL          mn34220_write_register
    /*00000fb4*/ MOV         R0, R4 
    /*00000fb8*/ MOV         R2, #0 
    /*00000fbc*/ MOVW        R1, #12565                  @ 0x3115 
    /*00000fc0*/ BL          mn34220_write_register
    /*00000fc4*/ MOV         R0, R4 
    /*00000fc8*/ MOV         R2, #1 
    /*00000fcc*/ MOVW        R1, #12566                  @ 0x3116 
    /*00000fd0*/ BL          mn34220_write_register
    /*00000fd4*/ MOV         R0, R4 
    /*00000fd8*/ MOV         R2, #0 
    /*00000fdc*/ MOVW        R1, #12567                  @ 0x3117 
    /*00000fe0*/ BL          mn34220_write_register
    /*00000fe4*/ MOV         R0, R4 
    /*00000fe8*/ MOV         R2, #1 
    /*00000fec*/ MOVW        R1, #12568                  @ 0x3118 
    /*00000ff0*/ BL          mn34220_write_register
    /*00000ff4*/ MOV         R0, R4 
    /*00000ff8*/ MOV         R2, #0 
    /*00000ffc*/ MOVW        R1, #12569                  @ 0x3119 
    /*00001000*/ BL          mn34220_write_register
    /*00001004*/ MOV         R0, R4 
    /*00001008*/ MOV         R2, #1 
    /*0000100c*/ MOVW        R1, #12570                  @ 0x311a 
    /*00001010*/ BL          mn34220_write_register
    /*00001014*/ MOV         R0, R4 
    /*00001018*/ MOV         R2, #0 
    /*0000101c*/ MOVW        R1, #12571                  @ 0x311b 
    /*00001020*/ BL          mn34220_write_register
    /*00001024*/ MOV         R0, R4 
    /*00001028*/ MOV         R2, #1 
    /*0000102c*/ MOVW        R1, #12572                  @ 0x311c 
    /*00001030*/ BL          mn34220_write_register
    /*00001034*/ MOV         R0, R4 
    /*00001038*/ MOV         R2, #0 
    /*0000103c*/ MOVW        R1, #12573                  @ 0x311d 
    /*00001040*/ BL          mn34220_write_register
    /*00001044*/ MOV         R0, R4 
    /*00001048*/ MOV         R2, #1 
    /*0000104c*/ MOVW        R1, #12574                  @ 0x311e 
    /*00001050*/ BL          mn34220_write_register
    /*00001054*/ MOV         R0, R4 
    /*00001058*/ MOV         R2, #0 
    /*0000105c*/ MOVW        R1, #12575                  @ 0x311f 
    /*00001060*/ BL          mn34220_write_register
    /*00001064*/ MOV         R0, R4 
    /*00001068*/ MOV         R2, #1 
    /*0000106c*/ MOVW        R1, #12576                  @ 0x3120 
    /*00001070*/ BL          mn34220_write_register
    /*00001074*/ MOV         R0, R4 
    /*00001078*/ MOV         R2, #0 
    /*0000107c*/ MOVW        R1, #12577                  @ 0x3121 
    /*00001080*/ BL          mn34220_write_register
    /*00001084*/ MOV         R0, R4 
    /*00001088*/ MOV         R2, #0 
    /*0000108c*/ MOVW        R1, #12578                  @ 0x3122 
    /*00001090*/ BL          mn34220_write_register
    /*00001094*/ MOV         R0, R4 
    /*00001098*/ MOV         R2, #0 
    /*0000109c*/ MOVW        R1, #12579                  @ 0x3123 
    /*000010a0*/ BL          mn34220_write_register
    /*000010a4*/ MOV         R0, R4 
    /*000010a8*/ MOV         R2, #0 
    /*000010ac*/ MOVW        R1, #12580                  @ 0x3124 
    /*000010b0*/ BL          mn34220_write_register
    /*000010b4*/ MOV         R0, R4 
    /*000010b8*/ MOV         R2, #0 
    /*000010bc*/ MOVW        R1, #12581                  @ 0x3125 
    /*000010c0*/ BL          mn34220_write_register
    /*000010c4*/ MOV         R0, R4 
    /*000010c8*/ MOV         R2, #0 
    /*000010cc*/ MOVW        R1, #12582                  @ 0x3126 
    /*000010d0*/ BL          mn34220_write_register
    /*000010d4*/ MOV         R0, R4 
    /*000010d8*/ MOV         R2, #2 
    /*000010dc*/ MOVW        R1, #12583                  @ 0x3127 
    /*000010e0*/ BL          mn34220_write_register
    /*000010e4*/ MOV         R0, R4 
    /*000010e8*/ MOV         R2, #0 
    /*000010ec*/ MOVW        R1, #12584                  @ 0x3128 
    /*000010f0*/ BL          mn34220_write_register
    /*000010f4*/ MOV         R0, R4 
    /*000010f8*/ MOV         R2, #0 
    /*000010fc*/ MOVW        R1, #12585                  @ 0x3129 
    /*00001100*/ BL          mn34220_write_register
    /*00001104*/ MOV         R0, R4 
    /*00001108*/ MOV         R2, #0 
    /*0000110c*/ MOVW        R1, #12586                  @ 0x312a 
    /*00001110*/ BL          mn34220_write_register
    /*00001114*/ MOV         R0, R4 
    /*00001118*/ MOV         R2, #0 
    /*0000111c*/ MOVW        R1, #12587                  @ 0x312b 
    /*00001120*/ BL          mn34220_write_register
    /*00001124*/ MOV         R0, R4 
    /*00001128*/ MOV         R2, #0 
    /*0000112c*/ MOVW        R1, #12588                  @ 0x312c 
    /*00001130*/ BL          mn34220_write_register
    /*00001134*/ MOV         R0, R4 
    /*00001138*/ MOV         R2, #0 
    /*0000113c*/ MOVW        R1, #12589                  @ 0x312d 
    /*00001140*/ BL          mn34220_write_register
    /*00001144*/ MOV         R0, R4 
    /*00001148*/ MOV         R2, #0 
    /*0000114c*/ MOVW        R1, #12590                  @ 0x312e 
    /*00001150*/ BL          mn34220_write_register
    /*00001154*/ MOV         R0, R4 
    /*00001158*/ MOV         R2, #0 
    /*0000115c*/ MOVW        R1, #12591                  @ 0x312f 
    /*00001160*/ BL          mn34220_write_register
    /*00001164*/ MOV         R0, R4 
    /*00001168*/ MOV         R2, #15 
    /*0000116c*/ MOVW        R1, #12592                  @ 0x3130 
    /*00001170*/ BL          mn34220_write_register
    /*00001174*/ MOV         R0, R4 
    /*00001178*/ MOV         R2, #1 
    /*0000117c*/ MOVW        R1, #12593                  @ 0x3131 
    /*00001180*/ BL          mn34220_write_register
    /*00001184*/ MOV         R0, R4 
    /*00001188*/ MOV         R2, #103                    @ 0x67 
    /*0000118c*/ MOVW        R1, #12594                  @ 0x3132 
    /*00001190*/ BL          mn34220_write_register
    /*00001194*/ MOV         R0, R4 
    /*00001198*/ MOV         R2, #16 
    /*0000119c*/ MOVW        R1, #12595                  @ 0x3133 
    /*000011a0*/ BL          mn34220_write_register
    /*000011a4*/ MOV         R0, R4 
    /*000011a8*/ MOV         R2, #112                    @ 0x70 
    /*000011ac*/ MOVW        R1, #12596                  @ 0x3134 
    /*000011b0*/ BL          mn34220_write_register
    /*000011b4*/ MOV         R0, R4 
    /*000011b8*/ MOV         R2, #8 
    /*000011bc*/ MOVW        R1, #12597                  @ 0x3135 
    /*000011c0*/ BL          mn34220_write_register
    /*000011c4*/ MOV         R0, R4 
    /*000011c8*/ MOV         R2, #3 
    /*000011cc*/ MOVW        R1, #12598                  @ 0x3136 
    /*000011d0*/ BL          mn34220_write_register
    /*000011d4*/ MOV         R0, R4 
    /*000011d8*/ MOV         R2, #255                    @ 0xff 
    /*000011dc*/ MOVW        R1, #12599                  @ 0x3137 
    /*000011e0*/ BL          mn34220_write_register
    /*000011e4*/ MOV         R0, R4 
    /*000011e8*/ MOV         R2, #0 
    /*000011ec*/ MOVW        R1, #12600                  @ 0x3138 
    /*000011f0*/ BL          mn34220_write_register
    /*000011f4*/ MOV         R0, R4 
    /*000011f8*/ MOV         R2, #0 
    /*000011fc*/ MOVW        R1, #12601                  @ 0x3139 
    /*00001200*/ BL          mn34220_write_register
    /*00001204*/ MOV         R0, R4 
    /*00001208*/ MOV         R2, #0 
    /*0000120c*/ MOVW        R1, #12602                  @ 0x313a 
    /*00001210*/ BL          mn34220_write_register
    /*00001214*/ MOV         R0, R4 
    /*00001218*/ MOV         R2, #1 
    /*0000121c*/ MOVW        R1, #12603                  @ 0x313b 
    /*00001220*/ BL          mn34220_write_register
    /*00001224*/ MOV         R0, R4 
    /*00001228*/ MOV         R2, #0 
    /*0000122c*/ MOVW        R1, #12604                  @ 0x313c 
    /*00001230*/ BL          mn34220_write_register
    /*00001234*/ MOV         R0, R4 
    /*00001238*/ MOV         R2, #0 
    /*0000123c*/ MOVW        R1, #12605                  @ 0x313d 
    /*00001240*/ BL          mn34220_write_register
    /*00001244*/ MOV         R0, R4 
    /*00001248*/ MOV         R2, #0 
    /*0000124c*/ MOV         R1, #12608                  @ 0x3140 
    /*00001250*/ BL          mn34220_write_register
    /*00001254*/ MOV         R0, R4 
    /*00001258*/ MOV         R2, #64                     @ 0x40 
    /*0000125c*/ MOVW        R1, #12609                  @ 0x3141 
    /*00001260*/ BL          mn34220_write_register
    /*00001264*/ MOV         R0, R4 
    /*00001268*/ MOV         R2, #80                     @ 0x50 
    /*0000126c*/ MOVW        R1, #12610                  @ 0x3142 
    /*00001270*/ BL          mn34220_write_register
    /*00001274*/ MOV         R0, R4 
    /*00001278*/ MOV         R2, #2 
    /*0000127c*/ MOVW        R1, #12611                  @ 0x3143 
    /*00001280*/ BL          mn34220_write_register
    /*00001284*/ MOV         R0, R4 
    /*00001288*/ MOV         R2, #2 
    /*0000128c*/ MOVW        R1, #12612                  @ 0x3144 
    /*00001290*/ BL          mn34220_write_register
    /*00001294*/ MOV         R0, R4 
    /*00001298*/ MOV         R2, #2 
    /*0000129c*/ MOVW        R1, #12613                  @ 0x3145 
    /*000012a0*/ BL          mn34220_write_register
    /*000012a4*/ MOV         R0, R4 
    /*000012a8*/ MOV         R2, #0 
    /*000012ac*/ MOVW        R1, #12614                  @ 0x3146 
    /*000012b0*/ BL          mn34220_write_register
    /*000012b4*/ MOV         R0, R4 
    /*000012b8*/ MOV         R2, #2 
    /*000012bc*/ MOVW        R1, #12615                  @ 0x3147 
    /*000012c0*/ BL          mn34220_write_register
    /*000012c4*/ MOV         R0, R4 
    /*000012c8*/ MOV         R2, #2 
    /*000012cc*/ MOVW        R1, #12616                  @ 0x3148 
    /*000012d0*/ BL          mn34220_write_register
    /*000012d4*/ MOV         R0, R4 
    /*000012d8*/ MOV         R2, #2 
    /*000012dc*/ MOVW        R1, #12617                  @ 0x3149 
    /*000012e0*/ BL          mn34220_write_register
    /*000012e4*/ MOV         R0, R4 
    /*000012e8*/ MOV         R2, #1 
    /*000012ec*/ MOVW        R1, #12618                  @ 0x314a 
    /*000012f0*/ BL          mn34220_write_register
    /*000012f4*/ MOV         R0, R4 
    /*000012f8*/ MOV         R2, #2 
    /*000012fc*/ MOVW        R1, #12619                  @ 0x314b 
    /*00001300*/ BL          mn34220_write_register
    /*00001304*/ MOV         R0, R4 
    /*00001308*/ MOV         R2, #2 
    /*0000130c*/ MOVW        R1, #12620                  @ 0x314c 
    /*00001310*/ BL          mn34220_write_register
    /*00001314*/ MOV         R0, R4 
    /*00001318*/ MOV         R2, #2 
    /*0000131c*/ MOVW        R1, #12621                  @ 0x314d 
    /*00001320*/ BL          mn34220_write_register
    /*00001324*/ MOV         R0, R4 
    /*00001328*/ MOV         R2, #1 
    /*0000132c*/ MOVW        R1, #12622                  @ 0x314e 
    /*00001330*/ BL          mn34220_write_register
    /*00001334*/ MOV         R0, R4 
    /*00001338*/ MOV         R2, #2 
    /*0000133c*/ MOVW        R1, #12623                  @ 0x314f 
    /*00001340*/ BL          mn34220_write_register
    /*00001344*/ MOV         R0, R4 
    /*00001348*/ MOV         R2, #2 
    /*0000134c*/ MOVW        R1, #12624                  @ 0x3150 
    /*00001350*/ BL          mn34220_write_register
    /*00001354*/ MOV         R0, R4 
    /*00001358*/ MOV         R2, #1 
    /*0000135c*/ MOVW        R1, #12625                  @ 0x3151 
    /*00001360*/ BL          mn34220_write_register
    /*00001364*/ MOV         R0, R4 
    /*00001368*/ MOV         R2, #1 
    /*0000136c*/ MOVW        R1, #12626                  @ 0x3152 
    /*00001370*/ BL          mn34220_write_register
    /*00001374*/ MOV         R0, R4 
    /*00001378*/ MOV         R2, #241                    @ 0xf1 
    /*0000137c*/ MOVW        R1, #12627                  @ 0x3153 
    /*00001380*/ BL          mn34220_write_register
    /*00001384*/ MOV         R0, R4 
    /*00001388*/ MOV         R2, #0 
    /*0000138c*/ MOVW        R1, #12628                  @ 0x3154 
    /*00001390*/ BL          mn34220_write_register
    /*00001394*/ MOV         R0, R4 
    /*00001398*/ MOV         R2, #202                    @ 0xca 
    /*0000139c*/ MOVW        R1, #12629                  @ 0x3155 
    /*000013a0*/ BL          mn34220_write_register
    /*000013a4*/ MOV         R0, R4 
    /*000013a8*/ MOV         R2, #0 
    /*000013ac*/ MOVW        R1, #12630                  @ 0x3156 
    /*000013b0*/ BL          mn34220_write_register
    /*000013b4*/ MOV         R0, R4 
    /*000013b8*/ MOV         R2, #202                    @ 0xca 
    /*000013bc*/ MOVW        R1, #12631                  @ 0x3157 
    /*000013c0*/ BL          mn34220_write_register
    /*000013c4*/ MOV         R0, R4 
    /*000013c8*/ MOV         R2, #0 
    /*000013cc*/ MOVW        R1, #12632                  @ 0x3158 
    /*000013d0*/ BL          mn34220_write_register
    /*000013d4*/ MOV         R0, R4 
    /*000013d8*/ MOV         R2, #202                    @ 0xca 
    /*000013dc*/ MOVW        R1, #12633                  @ 0x3159 
    /*000013e0*/ BL          mn34220_write_register
    /*000013e4*/ MOV         R0, R4 
    /*000013e8*/ MOV         R2, #0 
    /*000013ec*/ MOVW        R1, #12634                  @ 0x315a 
    /*000013f0*/ BL          mn34220_write_register
    /*000013f4*/ MOV         R0, R4 
    /*000013f8*/ MOV         R2, #202                    @ 0xca 
    /*000013fc*/ MOVW        R1, #12635                  @ 0x315b 
    /*00001400*/ BL          mn34220_write_register
    /*00001404*/ MOV         R0, R4 
    /*00001408*/ MOV         R2, #0 
    /*0000140c*/ MOVW        R1, #12636                  @ 0x315c 
    /*00001410*/ BL          mn34220_write_register
    /*00001414*/ MOV         R0, R4 
    /*00001418*/ MOV         R2, #202                    @ 0xca 
    /*0000141c*/ MOVW        R1, #12637                  @ 0x315d 
    /*00001420*/ BL          mn34220_write_register
    /*00001424*/ MOV         R0, R4 
    /*00001428*/ MOV         R2, #0 
    /*0000142c*/ MOVW        R1, #12638                  @ 0x315e 
    /*00001430*/ BL          mn34220_write_register
    /*00001434*/ MOV         R0, R4 
    /*00001438*/ MOV         R2, #202                    @ 0xca 
    /*0000143c*/ MOVW        R1, #12639                  @ 0x315f 
    /*00001440*/ BL          mn34220_write_register
    /*00001444*/ MOV         R0, R4 
    /*00001448*/ MOV         R2, #0 
    /*0000144c*/ MOVW        R1, #12640                  @ 0x3160 
    /*00001450*/ BL          mn34220_write_register
    /*00001454*/ MOV         R0, R4 
    /*00001458*/ MOV         R2, #202                    @ 0xca 
    /*0000145c*/ MOVW        R1, #12641                  @ 0x3161 
    /*00001460*/ BL          mn34220_write_register
    /*00001464*/ MOV         R0, R4 
    /*00001468*/ MOV         R2, #0 
    /*0000146c*/ MOVW        R1, #12642                  @ 0x3162 
    /*00001470*/ BL          mn34220_write_register
    /*00001474*/ MOV         R0, R4 
    /*00001478*/ MOV         R2, #202                    @ 0xca 
    /*0000147c*/ MOVW        R1, #12643                  @ 0x3163 
    /*00001480*/ BL          mn34220_write_register
    /*00001484*/ MOV         R0, R4 
    /*00001488*/ MOV         R2, #0 
    /*0000148c*/ MOVW        R1, #12644                  @ 0x3164 
    /*00001490*/ BL          mn34220_write_register
    /*00001494*/ MOV         R0, R4 
    /*00001498*/ MOV         R2, #202                    @ 0xca 
    /*0000149c*/ MOVW        R1, #12645                  @ 0x3165 
    /*000014a0*/ BL          mn34220_write_register
    /*000014a4*/ MOV         R0, R4 
    /*000014a8*/ MOV         R2, #0 
    /*000014ac*/ MOVW        R1, #12646                  @ 0x3166 
    /*000014b0*/ BL          mn34220_write_register
    /*000014b4*/ MOV         R0, R4 
    /*000014b8*/ MOV         R2, #202                    @ 0xca 
    /*000014bc*/ MOVW        R1, #12647                  @ 0x3167 
    /*000014c0*/ BL          mn34220_write_register
    /*000014c4*/ MOV         R0, R4 
    /*000014c8*/ MOV         R2, #0 
    /*000014cc*/ MOVW        R1, #12648                  @ 0x3168 
    /*000014d0*/ BL          mn34220_write_register
    /*000014d4*/ MOV         R0, R4 
    /*000014d8*/ MOV         R2, #202                    @ 0xca 
    /*000014dc*/ MOVW        R1, #12649                  @ 0x3169 
    /*000014e0*/ BL          mn34220_write_register
    /*000014e4*/ MOV         R0, R4 
    /*000014e8*/ MOV         R2, #0 
    /*000014ec*/ MOVW        R1, #12650                  @ 0x316a 
    /*000014f0*/ BL          mn34220_write_register
    /*000014f4*/ MOV         R0, R4 
    /*000014f8*/ MOV         R2, #202                    @ 0xca 
    /*000014fc*/ MOVW        R1, #12651                  @ 0x316b 
    /*00001500*/ BL          mn34220_write_register
    /*00001504*/ MOV         R0, R4 
    /*00001508*/ MOV         R2, #0 
    /*0000150c*/ MOVW        R1, #12652                  @ 0x316c 
    /*00001510*/ BL          mn34220_write_register
    /*00001514*/ MOV         R0, R4 
    /*00001518*/ MOV         R2, #202                    @ 0xca 
    /*0000151c*/ MOVW        R1, #12653                  @ 0x316d 
    /*00001520*/ BL          mn34220_write_register
    /*00001524*/ MOV         R0, R4 
    /*00001528*/ MOV         R2, #0 
    /*0000152c*/ MOVW        R1, #12654                  @ 0x316e 
    /*00001530*/ BL          mn34220_write_register
    /*00001534*/ MOV         R0, R4 
    /*00001538*/ MOV         R2, #202                    @ 0xca 
    /*0000153c*/ MOVW        R1, #12655                  @ 0x316f 
    /*00001540*/ BL          mn34220_write_register
    /*00001544*/ MOV         R0, R4 
    /*00001548*/ MOV         R2, #0 
    /*0000154c*/ MOVW        R1, #12656                  @ 0x3170 
    /*00001550*/ BL          mn34220_write_register
    /*00001554*/ MOV         R0, R4 
    /*00001558*/ MOV         R2, #202                    @ 0xca 
    /*0000155c*/ MOVW        R1, #12657                  @ 0x3171 
    /*00001560*/ BL          mn34220_write_register
    /*00001564*/ MOV         R0, R4 
    /*00001568*/ MOV         R2, #0 
    /*0000156c*/ MOVW        R1, #12658                  @ 0x3172 
    /*00001570*/ BL          mn34220_write_register
    /*00001574*/ MOV         R0, R4 
    /*00001578*/ MOV         R2, #202                    @ 0xca 
    /*0000157c*/ MOVW        R1, #12659                  @ 0x3173 
    /*00001580*/ BL          mn34220_write_register
    /*00001584*/ MOV         R0, R4 
    /*00001588*/ MOV         R2, #11 
    /*0000158c*/ MOVW        R1, #12660                  @ 0x3174 
    /*00001590*/ BL          mn34220_write_register
    /*00001594*/ MOV         R0, R4 
    /*00001598*/ MOV         R2, #128                    @ 0x80 
    /*0000159c*/ MOVW        R1, #12661                  @ 0x3175 
    /*000015a0*/ BL          mn34220_write_register
    /*000015a4*/ MOV         R0, R4 
    /*000015a8*/ MOV         R2, #136                    @ 0x88 
    /*000015ac*/ MOVW        R1, #12662                  @ 0x3176 
    /*000015b0*/ BL          mn34220_write_register
    /*000015b4*/ MOV         R0, R4 
    /*000015b8*/ MOV         R2, #160                    @ 0xa0 
    /*000015bc*/ MOVW        R1, #12663                  @ 0x3177 
    /*000015c0*/ BL          mn34220_write_register
    /*000015c4*/ MOV         R0, R4 
    /*000015c8*/ MOV         R2, #8 
    /*000015cc*/ MOVW        R1, #12664                  @ 0x3178 
    /*000015d0*/ BL          mn34220_write_register
    /*000015d4*/ MOV         R0, R4 
    /*000015d8*/ MOV         R2, #0 
    /*000015dc*/ MOVW        R1, #12665                  @ 0x3179 
    /*000015e0*/ BL          mn34220_write_register
    /*000015e4*/ MOV         R0, R4 
    /*000015e8*/ MOV         R2, #160                    @ 0xa0 
    /*000015ec*/ MOVW        R1, #12666                  @ 0x317a 
    /*000015f0*/ BL          mn34220_write_register
    /*000015f4*/ MOV         R0, R4 
    /*000015f8*/ MOV         R2, #162                    @ 0xa2 
    /*000015fc*/ MOVW        R1, #12667                  @ 0x317b 
    /*00001600*/ BL          mn34220_write_register
    /*00001604*/ MOV         R0, R4 
    /*00001608*/ MOV         R2, #160                    @ 0xa0 
    /*0000160c*/ MOVW        R1, #12668                  @ 0x317c 
    /*00001610*/ BL          mn34220_write_register
    /*00001614*/ MOV         R0, R4 
    /*00001618*/ MOV         R2, #2 
    /*0000161c*/ MOVW        R1, #12669                  @ 0x317d 
    /*00001620*/ BL          mn34220_write_register
    /*00001624*/ MOV         R0, R4 
    /*00001628*/ MOV         R2, #0 
    /*0000162c*/ MOVW        R1, #12670                  @ 0x317e 
    /*00001630*/ BL          mn34220_write_register
    /*00001634*/ MOV         R0, R4 
    /*00001638*/ MOV         R2, #0 
    /*0000163c*/ MOVW        R1, #12671                  @ 0x317f 
    /*00001640*/ BL          mn34220_write_register
    /*00001644*/ MOV         R0, R4 
    /*00001648*/ MOV         R2, #0 
    /*0000164c*/ MOV         R1, #12672                  @ 0x3180 
    /*00001650*/ BL          mn34220_write_register
    /*00001654*/ MOV         R0, R4 
    /*00001658*/ MOV         R2, #130                    @ 0x82 
    /*0000165c*/ MOVW        R1, #12673                  @ 0x3181 
    /*00001660*/ BL          mn34220_write_register
    /*00001664*/ MOV         R0, R4 
    /*00001668*/ MOV         R2, #32 
    /*0000166c*/ MOVW        R1, #12674                  @ 0x3182 
    /*00001670*/ BL          mn34220_write_register
    /*00001674*/ MOV         R0, R4 
    /*00001678*/ MOV         R2, #32 
    /*0000167c*/ MOVW        R1, #12675                  @ 0x3183 
    /*00001680*/ BL          mn34220_write_register
    /*00001684*/ MOV         R0, R4 
    /*00001688*/ MOV         R2, #2 
    /*0000168c*/ MOVW        R1, #12676                  @ 0x3184 
    /*00001690*/ BL          mn34220_write_register
    /*00001694*/ MOV         R0, R4 
    /*00001698*/ MOV         R2, #0 
    /*0000169c*/ MOVW        R1, #12677                  @ 0x3185 
    /*000016a0*/ BL          mn34220_write_register
    /*000016a4*/ MOV         R0, R4 
    /*000016a8*/ MOV         R2, #72                     @ 0x48 
    /*000016ac*/ MOVW        R1, #12678                  @ 0x3186 
    /*000016b0*/ BL          mn34220_write_register
    /*000016b4*/ MOV         R0, R4 
    /*000016b8*/ MOV         R2, #16 
    /*000016bc*/ MOVW        R1, #12679                  @ 0x3187 
    /*000016c0*/ BL          mn34220_write_register
    /*000016c4*/ MOV         R0, R4 
    /*000016c8*/ MOV         R2, #0 
    /*000016cc*/ MOVW        R1, #12680                  @ 0x3188 
    /*000016d0*/ BL          mn34220_write_register
    /*000016d4*/ MOV         R0, R4 
    /*000016d8*/ MOV         R2, #222                    @ 0xde 
    /*000016dc*/ MOVW        R1, #12681                  @ 0x3189 
    /*000016e0*/ BL          mn34220_write_register
    /*000016e4*/ MOV         R0, R4 
    /*000016e8*/ MOV         R2, #107                    @ 0x6b 
    /*000016ec*/ MOVW        R1, #12682                  @ 0x318a 
    /*000016f0*/ BL          mn34220_write_register
    /*000016f4*/ MOV         R0, R4 
    /*000016f8*/ MOV         R2, #15 
    /*000016fc*/ MOVW        R1, #12683                  @ 0x318b 
    /*00001700*/ BL          mn34220_write_register
    /*00001704*/ MOV         R0, R4 
    /*00001708*/ MOV         R2, #31 
    /*0000170c*/ MOVW        R1, #12684                  @ 0x318c 
    /*00001710*/ BL          mn34220_write_register
    /*00001714*/ MOV         R0, R4 
    /*00001718*/ MOV         R2, #0 
    /*0000171c*/ MOVW        R1, #12685                  @ 0x318d 
    /*00001720*/ BL          mn34220_write_register
    /*00001724*/ MOV         R0, R4 
    /*00001728*/ MOV         R2, #32 
    /*0000172c*/ MOVW        R1, #12686                  @ 0x318e 
    /*00001730*/ BL          mn34220_write_register
    /*00001734*/ MOV         R0, R4 
    /*00001738*/ MOV         R2, #112                    @ 0x70 
    /*0000173c*/ MOVW        R1, #12687                  @ 0x318f 
    /*00001740*/ BL          mn34220_write_register
    /*00001744*/ MOV         R0, R4 
    /*00001748*/ MOV         R2, #0 
    /*0000174c*/ MOVW        R1, #12688                  @ 0x3190 
    /*00001750*/ BL          mn34220_write_register
    /*00001754*/ MOV         R0, R4 
    /*00001758*/ MOV         R2, #0 
    /*0000175c*/ MOVW        R1, #12689                  @ 0x3191 
    /*00001760*/ BL          mn34220_write_register
    /*00001764*/ MOV         R0, R4 
    /*00001768*/ MOV         R2, #0 
    /*0000176c*/ MOVW        R1, #12690                  @ 0x3192 
    /*00001770*/ BL          mn34220_write_register
    /*00001774*/ MOV         R0, R4 
    /*00001778*/ MOV         R2, #0 
    /*0000177c*/ MOVW        R1, #12691                  @ 0x3193 
    /*00001780*/ BL          mn34220_write_register
    /*00001784*/ MOV         R0, R4 
    /*00001788*/ MOV         R2, #0 
    /*0000178c*/ MOVW        R1, #12692                  @ 0x3194 
    /*00001790*/ BL          mn34220_write_register
    /*00001794*/ MOV         R0, R4 
    /*00001798*/ MOV         R2, #64                     @ 0x40 
    /*0000179c*/ MOVW        R1, #12693                  @ 0x3195 
    /*000017a0*/ BL          mn34220_write_register
    /*000017a4*/ MOV         R0, R4 
    /*000017a8*/ MOV         R2, #8 
    /*000017ac*/ MOVW        R1, #12694                  @ 0x3196 
    /*000017b0*/ BL          mn34220_write_register
    /*000017b4*/ MOV         R0, R4 
    /*000017b8*/ MOV         R2, #6 
    /*000017bc*/ MOVW        R1, #12695                  @ 0x3197 
    /*000017c0*/ BL          mn34220_write_register
    /*000017c4*/ MOV         R0, R4 
    /*000017c8*/ MOV         R2, #0 
    /*000017cc*/ MOVW        R1, #12696                  @ 0x3198 
    /*000017d0*/ BL          mn34220_write_register
    /*000017d4*/ MOV         R0, R4 
    /*000017d8*/ MOV         R2, #0 
    /*000017dc*/ MOVW        R1, #12697                  @ 0x3199 
    /*000017e0*/ BL          mn34220_write_register
    /*000017e4*/ MOV         R0, R4 
    /*000017e8*/ MOV         R2, #14 
    /*000017ec*/ MOVW        R1, #12698                  @ 0x319a 
    /*000017f0*/ BL          mn34220_write_register
    /*000017f4*/ MOV         R0, R4 
    /*000017f8*/ MOV         R2, #16 
    /*000017fc*/ MOVW        R1, #12699                  @ 0x319b 
    /*00001800*/ BL          mn34220_write_register
    /*00001804*/ MOV         R0, R4 
    /*00001808*/ MOV         R2, #50                     @ 0x32 
    /*0000180c*/ MOVW        R1, #12700                  @ 0x319c 
    /*00001810*/ BL          mn34220_write_register
    /*00001814*/ MOV         R0, R4 
    /*00001818*/ MOV         R2, #84                     @ 0x54 
    /*0000181c*/ MOVW        R1, #12701                  @ 0x319d 
    /*00001820*/ BL          mn34220_write_register
    /*00001824*/ MOV         R0, R4 
    /*00001828*/ MOV         R2, #118                    @ 0x76 
    /*0000182c*/ MOVW        R1, #12702                  @ 0x319e 
    /*00001830*/ BL          mn34220_write_register
    /*00001834*/ MOV         R0, R4 
    /*00001838*/ MOV         R2, #152                    @ 0x98 
    /*0000183c*/ MOVW        R1, #12703                  @ 0x319f 
    /*00001840*/ BL          mn34220_write_register
    /*00001844*/ MOV         R0, R4 
    /*00001848*/ MOV         R2, #0 
    /*0000184c*/ MOVW        R1, #12704                  @ 0x31a0 
    /*00001850*/ BL          mn34220_write_register
    /*00001854*/ MOV         R0, R4 
    /*00001858*/ MOV         R2, #0 
    /*0000185c*/ MOVW        R1, #12705                  @ 0x31a1 
    /*00001860*/ BL          mn34220_write_register
    /*00001864*/ MOV         R0, R4 
    /*00001868*/ MOV         R2, #0 
    /*0000186c*/ MOVW        R1, #12706                  @ 0x31a2 
    /*00001870*/ BL          mn34220_write_register
    /*00001874*/ MOV         R0, R4 
    /*00001878*/ MOV         R2, #16 
    /*0000187c*/ MOVW        R1, #12707                  @ 0x31a3 
    /*00001880*/ BL          mn34220_write_register
    /*00001884*/ MOV         R0, R4 
    /*00001888*/ MOV         R2, #50                     @ 0x32 
    /*0000188c*/ MOVW        R1, #12708                  @ 0x31a4 
    /*00001890*/ BL          mn34220_write_register
    /*00001894*/ MOV         R0, R4 
    /*00001898*/ MOV         R2, #84                     @ 0x54 
    /*0000189c*/ MOVW        R1, #12709                  @ 0x31a5 
    /*000018a0*/ BL          mn34220_write_register
    /*000018a4*/ MOV         R0, R4 
    /*000018a8*/ MOV         R2, #134                    @ 0x86 
    /*000018ac*/ MOVW        R1, #12710                  @ 0x31a6 
    /*000018b0*/ BL          mn34220_write_register
    /*000018b4*/ MOV         R0, R4 
    /*000018b8*/ MOV         R2, #9 
    /*000018bc*/ MOVW        R1, #12711                  @ 0x31a7 
    /*000018c0*/ BL          mn34220_write_register
    /*000018c4*/ MOV         R0, R4 
    /*000018c8*/ MOV         R2, #0 
    /*000018cc*/ MOVW        R1, #12712                  @ 0x31a8 
    /*000018d0*/ BL          mn34220_write_register
    /*000018d4*/ MOV         R0, R4 
    /*000018d8*/ MOV         R2, #0 
    /*000018dc*/ MOVW        R1, #12713                  @ 0x31a9 
    /*000018e0*/ BL          mn34220_write_register
    /*000018e4*/ MOV         R0, R4 
    /*000018e8*/ MOV         R2, #0 
    /*000018ec*/ MOVW        R1, #12714                  @ 0x31aa 
    /*000018f0*/ BL          mn34220_write_register
    /*000018f4*/ MOV         R0, R4 
    /*000018f8*/ MOV         R2, #36                     @ 0x24 
    /*000018fc*/ MOVW        R1, #12715                  @ 0x31ab 
    /*00001900*/ BL          mn34220_write_register
    /*00001904*/ MOV         R0, R4 
    /*00001908*/ MOV         R2, #0 
    /*0000190c*/ MOVW        R1, #12716                  @ 0x31ac 
    /*00001910*/ BL          mn34220_write_register
    /*00001914*/ MOV         R0, R4 
    /*00001918*/ MOV         R2, #137                    @ 0x89 
    /*0000191c*/ MOVW        R1, #12717                  @ 0x31ad 
    /*00001920*/ BL          mn34220_write_register
    /*00001924*/ MOV         R0, R4 
    /*00001928*/ MOV         R2, #2 
    /*0000192c*/ MOVW        R1, #12718                  @ 0x31ae 
    /*00001930*/ BL          mn34220_write_register
    /*00001934*/ MOV         R0, R4 
    /*00001938*/ MOV         R2, #0 
    /*0000193c*/ MOVW        R1, #12719                  @ 0x31af 
    /*00001940*/ BL          mn34220_write_register
    /*00001944*/ MOV         R0, R4 
    /*00001948*/ MOV         R2, #0 
    /*0000194c*/ MOVW        R1, #12720                  @ 0x31b0 
    /*00001950*/ BL          mn34220_write_register
    /*00001954*/ MOV         R0, R4 
    /*00001958*/ MOV         R2, #0 
    /*0000195c*/ MOVW        R1, #12721                  @ 0x31b1 
    /*00001960*/ BL          mn34220_write_register
    /*00001964*/ MOV         R0, R4 
    /*00001968*/ MOV         R2, #0 
    /*0000196c*/ MOVW        R1, #12722                  @ 0x31b2 
    /*00001970*/ BL          mn34220_write_register
    /*00001974*/ MOV         R0, R4 
    /*00001978*/ MOV         R2, #0 
    /*0000197c*/ MOVW        R1, #12723                  @ 0x31b3 
    /*00001980*/ BL          mn34220_write_register
    /*00001984*/ MOV         R0, R4 
    /*00001988*/ MOV         R2, #0 
    /*0000198c*/ MOVW        R1, #12724                  @ 0x31b4 
    /*00001990*/ BL          mn34220_write_register
    /*00001994*/ MOV         R0, R4 
    /*00001998*/ MOV         R2, #0 
    /*0000199c*/ MOVW        R1, #12725                  @ 0x31b5 
    /*000019a0*/ BL          mn34220_write_register
    /*000019a4*/ MOV         R0, R4 
    /*000019a8*/ MOV         R2, #0 
    /*000019ac*/ MOVW        R1, #12726                  @ 0x31b6 
    /*000019b0*/ BL          mn34220_write_register
    /*000019b4*/ MOV         R0, R4 
    /*000019b8*/ MOV         R2, #0 
    /*000019bc*/ MOVW        R1, #12727                  @ 0x31b7 
    /*000019c0*/ BL          mn34220_write_register
    /*000019c4*/ MOV         R0, R4 
    /*000019c8*/ MOV         R2, #0 
    /*000019cc*/ MOVW        R1, #12728                  @ 0x31b8 
    /*000019d0*/ BL          mn34220_write_register
    /*000019d4*/ MOV         R0, R4 
    /*000019d8*/ MOV         R2, #0 
    /*000019dc*/ MOVW        R1, #12729                  @ 0x31b9 
    /*000019e0*/ BL          mn34220_write_register
    /*000019e4*/ MOV         R0, R4 
    /*000019e8*/ MOV         R2, #0 
    /*000019ec*/ MOVW        R1, #12730                  @ 0x31ba 
    /*000019f0*/ BL          mn34220_write_register
    /*000019f4*/ MOV         R0, R4 
    /*000019f8*/ MOV         R2, #0 
    /*000019fc*/ MOVW        R1, #12731                  @ 0x31bb 
    /*00001a00*/ BL          mn34220_write_register
    /*00001a04*/ MOV         R0, R4 
    /*00001a08*/ MOV         R2, #0 
    /*00001a0c*/ MOVW        R1, #12732                  @ 0x31bc 
    /*00001a10*/ BL          mn34220_write_register
    /*00001a14*/ MOV         R0, R4 
    /*00001a18*/ MOV         R2, #0 
    /*00001a1c*/ MOVW        R1, #12733                  @ 0x31bd 
    /*00001a20*/ BL          mn34220_write_register
    /*00001a24*/ MOV         R0, R4 
    /*00001a28*/ MOV         R2, #0 
    /*00001a2c*/ MOVW        R1, #12734                  @ 0x31be 
    /*00001a30*/ BL          mn34220_write_register
    /*00001a34*/ MOV         R0, R4 
    /*00001a38*/ MOV         R2, #0 
    /*00001a3c*/ MOVW        R1, #12735                  @ 0x31bf 
    /*00001a40*/ BL          mn34220_write_register
    /*00001a44*/ MOV         R0, R4 
    /*00001a48*/ MOV         R2, #0 
    /*00001a4c*/ MOV         R1, #12736                  @ 0x31c0 
    /*00001a50*/ BL          mn34220_write_register
    /*00001a54*/ MOV         R0, R4 
    /*00001a58*/ MOV         R2, #0 
    /*00001a5c*/ MOVW        R1, #12737                  @ 0x31c1 
    /*00001a60*/ BL          mn34220_write_register
    /*00001a64*/ MOV         R0, R4 
    /*00001a68*/ MOV         R2, #0 
    /*00001a6c*/ MOVW        R1, #12738                  @ 0x31c2 
    /*00001a70*/ BL          mn34220_write_register
    /*00001a74*/ MOV         R0, R4 
    /*00001a78*/ MOV         R2, #0 
    /*00001a7c*/ MOVW        R1, #12739                  @ 0x31c3 
    /*00001a80*/ BL          mn34220_write_register
    /*00001a84*/ MOV         R0, R4 
    /*00001a88*/ MOV         R2, #0 
    /*00001a8c*/ MOVW        R1, #12740                  @ 0x31c4 
    /*00001a90*/ BL          mn34220_write_register
    /*00001a94*/ MOV         R0, R4 
    /*00001a98*/ MOV         R2, #0 
    /*00001a9c*/ MOVW        R1, #12741                  @ 0x31c5 
    /*00001aa0*/ BL          mn34220_write_register
    /*00001aa4*/ MOV         R0, R4 
    /*00001aa8*/ MOV         R2, #0 
    /*00001aac*/ MOVW        R1, #12742                  @ 0x31c6 
    /*00001ab0*/ BL          mn34220_write_register
    /*00001ab4*/ MOV         R0, R4 
    /*00001ab8*/ MOV         R2, #0 
    /*00001abc*/ MOVW        R1, #12743                  @ 0x31c7 
    /*00001ac0*/ BL          mn34220_write_register
    /*00001ac4*/ MOV         R0, R4 
    /*00001ac8*/ MOV         R2, #0 
    /*00001acc*/ MOVW        R1, #12744                  @ 0x31c8 
    /*00001ad0*/ BL          mn34220_write_register
    /*00001ad4*/ MOV         R0, R4 
    /*00001ad8*/ MOV         R2, #0 
    /*00001adc*/ MOVW        R1, #12745                  @ 0x31c9 
    /*00001ae0*/ BL          mn34220_write_register
    /*00001ae4*/ MOV         R0, R4 
    /*00001ae8*/ MOV         R2, #0 
    /*00001aec*/ MOVW        R1, #12746                  @ 0x31ca 
    /*00001af0*/ BL          mn34220_write_register
    /*00001af4*/ MOV         R0, R4 
    /*00001af8*/ MOV         R2, #0 
    /*00001afc*/ MOVW        R1, #12747                  @ 0x31cb 
    /*00001b00*/ BL          mn34220_write_register
    /*00001b04*/ MOV         R0, R4 
    /*00001b08*/ MOV         R2, #0 
    /*00001b0c*/ MOVW        R1, #12748                  @ 0x31cc 
    /*00001b10*/ BL          mn34220_write_register
    /*00001b14*/ MOV         R0, R4 
    /*00001b18*/ MOV         R2, #0 
    /*00001b1c*/ MOVW        R1, #12749                  @ 0x31cd 
    /*00001b20*/ BL          mn34220_write_register
    /*00001b24*/ MOV         R0, R4 
    /*00001b28*/ MOV         R2, #0 
    /*00001b2c*/ MOVW        R1, #12750                  @ 0x31ce 
    /*00001b30*/ BL          mn34220_write_register
    /*00001b34*/ MOV         R0, R4 
    /*00001b38*/ MOV         R2, #0 
    /*00001b3c*/ MOVW        R1, #12751                  @ 0x31cf 
    /*00001b40*/ BL          mn34220_write_register
    /*00001b44*/ MOV         R0, R4 
    /*00001b48*/ MOV         R2, #0 
    /*00001b4c*/ MOVW        R1, #12752                  @ 0x31d0 
    /*00001b50*/ BL          mn34220_write_register
    /*00001b54*/ MOV         R0, R4 
    /*00001b58*/ MOV         R2, #0 
    /*00001b5c*/ MOVW        R1, #12753                  @ 0x31d1 
    /*00001b60*/ BL          mn34220_write_register
    /*00001b64*/ MOV         R0, R4 
    /*00001b68*/ MOV         R2, #0 
    /*00001b6c*/ MOVW        R1, #12754                  @ 0x31d2 
    /*00001b70*/ BL          mn34220_write_register
    /*00001b74*/ MOV         R0, R4 
    /*00001b78*/ MOV         R2, #0 
    /*00001b7c*/ MOVW        R1, #12755                  @ 0x31d3 
    /*00001b80*/ BL          mn34220_write_register
    /*00001b84*/ MOV         R0, R4 
    /*00001b88*/ MOV         R2, #0 
    /*00001b8c*/ MOVW        R1, #12756                  @ 0x31d4 
    /*00001b90*/ BL          mn34220_write_register
    /*00001b94*/ MOV         R0, R4 
    /*00001b98*/ MOV         R2, #0 
    /*00001b9c*/ MOVW        R1, #12757                  @ 0x31d5 
    /*00001ba0*/ BL          mn34220_write_register
    /*00001ba4*/ MOV         R0, R4 
    /*00001ba8*/ MOV         R2, #0 
    /*00001bac*/ MOVW        R1, #12758                  @ 0x31d6 
    /*00001bb0*/ BL          mn34220_write_register
    /*00001bb4*/ MOV         R0, R4 
    /*00001bb8*/ MOV         R2, #0 
    /*00001bbc*/ MOVW        R1, #12759                  @ 0x31d7 
    /*00001bc0*/ BL          mn34220_write_register
    /*00001bc4*/ MOV         R0, R4 
    /*00001bc8*/ MOV         R2, #0 
    /*00001bcc*/ MOVW        R1, #12760                  @ 0x31d8 
    /*00001bd0*/ BL          mn34220_write_register
    /*00001bd4*/ MOV         R0, R4 
    /*00001bd8*/ MOV         R2, #0 
    /*00001bdc*/ MOVW        R1, #12761                  @ 0x31d9 
    /*00001be0*/ BL          mn34220_write_register
    /*00001be4*/ MOV         R0, R4 
    /*00001be8*/ MOV         R2, #0 
    /*00001bec*/ MOVW        R1, #12762                  @ 0x31da 
    /*00001bf0*/ BL          mn34220_write_register
    /*00001bf4*/ MOV         R0, R4 
    /*00001bf8*/ MOV         R2, #0 
    /*00001bfc*/ MOVW        R1, #12763                  @ 0x31db 
    /*00001c00*/ BL          mn34220_write_register
    /*00001c04*/ MOV         R0, R4 
    /*00001c08*/ MOV         R2, #0 
    /*00001c0c*/ MOVW        R1, #12764                  @ 0x31dc 
    /*00001c10*/ BL          mn34220_write_register
    /*00001c14*/ MOV         R0, R4 
    /*00001c18*/ MOV         R2, #0 
    /*00001c1c*/ MOVW        R1, #12765                  @ 0x31dd 
    /*00001c20*/ BL          mn34220_write_register
    /*00001c24*/ MOV         R0, R4 
    /*00001c28*/ MOV         R2, #0 
    /*00001c2c*/ MOVW        R1, #12766                  @ 0x31de 
    /*00001c30*/ BL          mn34220_write_register
    /*00001c34*/ MOV         R0, R4 
    /*00001c38*/ MOV         R2, #0 
    /*00001c3c*/ MOVW        R1, #12767                  @ 0x31df 
    /*00001c40*/ BL          mn34220_write_register
    /*00001c44*/ MOV         R0, R4 
    /*00001c48*/ MOV         R2, #0 
    /*00001c4c*/ MOVW        R1, #12768                  @ 0x31e0 
    /*00001c50*/ BL          mn34220_write_register
    /*00001c54*/ MOV         R0, R4 
    /*00001c58*/ MOV         R2, #0 
    /*00001c5c*/ MOVW        R1, #12769                  @ 0x31e1 
    /*00001c60*/ BL          mn34220_write_register
    /*00001c64*/ MOV         R0, R4 
    /*00001c68*/ MOV         R2, #0 
    /*00001c6c*/ MOVW        R1, #12770                  @ 0x31e2 
    /*00001c70*/ BL          mn34220_write_register
    /*00001c74*/ MOV         R0, R4 
    /*00001c78*/ MOV         R2, #0 
    /*00001c7c*/ MOVW        R1, #12771                  @ 0x31e3 
    /*00001c80*/ BL          mn34220_write_register
    /*00001c84*/ MOV         R0, R4 
    /*00001c88*/ MOV         R2, #0 
    /*00001c8c*/ MOVW        R1, #12772                  @ 0x31e4 
    /*00001c90*/ BL          mn34220_write_register
    /*00001c94*/ MOV         R0, R4 
    /*00001c98*/ MOV         R2, #0 
    /*00001c9c*/ MOVW        R1, #12773                  @ 0x31e5 
    /*00001ca0*/ BL          mn34220_write_register
    /*00001ca4*/ MOV         R0, R4 
    /*00001ca8*/ MOV         R2, #0 
    /*00001cac*/ MOVW        R1, #12774                  @ 0x31e6 
    /*00001cb0*/ BL          mn34220_write_register
    /*00001cb4*/ MOV         R0, R4 
    /*00001cb8*/ MOV         R2, #0 
    /*00001cbc*/ MOVW        R1, #12775                  @ 0x31e7 
    /*00001cc0*/ BL          mn34220_write_register
    /*00001cc4*/ MOV         R0, R4 
    /*00001cc8*/ MOV         R2, #0 
    /*00001ccc*/ MOVW        R1, #12776                  @ 0x31e8 
    /*00001cd0*/ BL          mn34220_write_register
    /*00001cd4*/ MOV         R0, R4 
    /*00001cd8*/ MOV         R2, #0 
    /*00001cdc*/ MOVW        R1, #12777                  @ 0x31e9 
    /*00001ce0*/ BL          mn34220_write_register
    /*00001ce4*/ MOV         R0, R4 
    /*00001ce8*/ MOV         R2, #0 
    /*00001cec*/ MOVW        R1, #12778                  @ 0x31ea 
    /*00001cf0*/ BL          mn34220_write_register
    /*00001cf4*/ MOV         R0, R4 
    /*00001cf8*/ MOV         R2, #0 
    /*00001cfc*/ MOVW        R1, #12779                  @ 0x31eb 
    /*00001d00*/ BL          mn34220_write_register
    /*00001d04*/ MOV         R0, R4 
    /*00001d08*/ MOV         R2, #0 
    /*00001d0c*/ MOVW        R1, #12780                  @ 0x31ec 
    /*00001d10*/ BL          mn34220_write_register
    /*00001d14*/ MOV         R0, R4 
    /*00001d18*/ MOV         R2, #0 
    /*00001d1c*/ MOVW        R1, #12781                  @ 0x31ed 
    /*00001d20*/ BL          mn34220_write_register
    /*00001d24*/ MOV         R0, R4 
    /*00001d28*/ MOV         R2, #0 
    /*00001d2c*/ MOVW        R1, #12782                  @ 0x31ee 
    /*00001d30*/ BL          mn34220_write_register
    /*00001d34*/ MOV         R0, R4 
    /*00001d38*/ MOV         R2, #0 
    /*00001d3c*/ MOVW        R1, #12783                  @ 0x31ef 
    /*00001d40*/ BL          mn34220_write_register
    /*00001d44*/ MOV         R0, R4 
    /*00001d48*/ MOV         R2, #0 
    /*00001d4c*/ MOVW        R1, #12784                  @ 0x31f0 
    /*00001d50*/ BL          mn34220_write_register
    /*00001d54*/ MOV         R0, R4 
    /*00001d58*/ MOV         R2, #0 
    /*00001d5c*/ MOVW        R1, #12785                  @ 0x31f1 
    /*00001d60*/ BL          mn34220_write_register
    /*00001d64*/ MOV         R0, R4 
    /*00001d68*/ MOV         R2, #0 
    /*00001d6c*/ MOVW        R1, #12786                  @ 0x31f2 
    /*00001d70*/ BL          mn34220_write_register
    /*00001d74*/ MOV         R0, R4 
    /*00001d78*/ MOV         R2, #0 
    /*00001d7c*/ MOVW        R1, #12787                  @ 0x31f3 
    /*00001d80*/ BL          mn34220_write_register
    /*00001d84*/ MOV         R0, R4 
    /*00001d88*/ MOV         R2, #0 
    /*00001d8c*/ MOVW        R1, #12788                  @ 0x31f4 
    /*00001d90*/ BL          mn34220_write_register
    /*00001d94*/ MOV         R0, R4 
    /*00001d98*/ MOV         R2, #0 
    /*00001d9c*/ MOVW        R1, #12789                  @ 0x31f5 
    /*00001da0*/ BL          mn34220_write_register
    /*00001da4*/ MOV         R0, R4 
    /*00001da8*/ MOV         R2, #0 
    /*00001dac*/ MOVW        R1, #12790                  @ 0x31f6 
    /*00001db0*/ BL          mn34220_write_register
    /*00001db4*/ MOV         R0, R4 
    /*00001db8*/ MOV         R2, #3 
    /*00001dbc*/ MOVW        R1, #12791                  @ 0x31f7 
    /*00001dc0*/ BL          mn34220_write_register
    /*00001dc4*/ MOV         R0, R4 
    /*00001dc8*/ MOV         R2, #3 
    /*00001dcc*/ MOVW        R1, #12792                  @ 0x31f8 
    /*00001dd0*/ BL          mn34220_write_register
    /*00001dd4*/ MOV         R0, R4 
    /*00001dd8*/ MOV         R2, #3 
    /*00001ddc*/ MOVW        R1, #12793                  @ 0x31f9 
    /*00001de0*/ BL          mn34220_write_register
    /*00001de4*/ MOV         R0, R4 
    /*00001de8*/ MOV         R2, #6 
    /*00001dec*/ MOVW        R1, #12794                  @ 0x31fa 
    /*00001df0*/ BL          mn34220_write_register
    /*00001df4*/ MOV         R0, R4 
    /*00001df8*/ MOV         R2, #8 
    /*00001dfc*/ MOVW        R1, #12795                  @ 0x31fb 
    /*00001e00*/ BL          mn34220_write_register
    /*00001e04*/ MOV         R0, R4 
    /*00001e08*/ MOV         R2, #2 
    /*00001e0c*/ MOVW        R1, #12796                  @ 0x31fc 
    /*00001e10*/ BL          mn34220_write_register
    /*00001e14*/ MOV         R0, R4 
    /*00001e18*/ MOV         R2, #8 
    /*00001e1c*/ MOVW        R1, #12797                  @ 0x31fd 
    /*00001e20*/ BL          mn34220_write_register
    /*00001e24*/ MOV         R0, R4 
    /*00001e28*/ MOV         R2, #7 
    /*00001e2c*/ MOVW        R1, #12798                  @ 0x31fe 
    /*00001e30*/ BL          mn34220_write_register
    /*00001e34*/ MOV         R0, R4 
    /*00001e38*/ MOV         R2, #0 
    /*00001e3c*/ MOVW        R1, #12799                  @ 0x31ff 
    /*00001e40*/ BL          mn34220_write_register
    /*00001e44*/ MOV         R0, R4 
    /*00001e48*/ MOV         R2, #0 
    /*00001e4c*/ MOV         R1, #12800                  @ 0x3200 
    /*00001e50*/ BL          mn34220_write_register
    /*00001e54*/ MOV         R0, R4 
    /*00001e58*/ MOV         R2, #0 
    /*00001e5c*/ MOVW        R1, #12801                  @ 0x3201 
    /*00001e60*/ BL          mn34220_write_register
    /*00001e64*/ MOV         R0, R4 
    /*00001e68*/ MOV         R2, #0 
    /*00001e6c*/ MOVW        R1, #12802                  @ 0x3202 
    /*00001e70*/ BL          mn34220_write_register
    /*00001e74*/ MOV         R0, R4 
    /*00001e78*/ MOV         R2, #0 
    /*00001e7c*/ MOVW        R1, #12803                  @ 0x3203 
    /*00001e80*/ BL          mn34220_write_register
    /*00001e84*/ MOV         R0, R4 
    /*00001e88*/ MOV         R2, #0 
    /*00001e8c*/ MOVW        R1, #12804                  @ 0x3204 
    /*00001e90*/ BL          mn34220_write_register
    /*00001e94*/ MOV         R0, R4 
    /*00001e98*/ MOV         R2, #0 
    /*00001e9c*/ MOVW        R1, #12805                  @ 0x3205 
    /*00001ea0*/ BL          mn34220_write_register
    /*00001ea4*/ MOV         R0, R4 
    /*00001ea8*/ MOV         R2, #0 
    /*00001eac*/ MOVW        R1, #12806                  @ 0x3206 
    /*00001eb0*/ BL          mn34220_write_register
    /*00001eb4*/ MOV         R0, R4 
    /*00001eb8*/ MOV         R2, #0 
    /*00001ebc*/ MOVW        R1, #12807                  @ 0x3207 
    /*00001ec0*/ BL          mn34220_write_register
    /*00001ec4*/ MOV         R0, R4 
    /*00001ec8*/ MOV         R2, #0 
    /*00001ecc*/ MOVW        R1, #12808                  @ 0x3208 
    /*00001ed0*/ BL          mn34220_write_register
    /*00001ed4*/ MOV         R0, R4 
    /*00001ed8*/ MOV         R2, #0 
    /*00001edc*/ MOVW        R1, #12809                  @ 0x3209 
    /*00001ee0*/ BL          mn34220_write_register
    /*00001ee4*/ MOV         R0, R4 
    /*00001ee8*/ MOV         R2, #0 
    /*00001eec*/ MOVW        R1, #12810                  @ 0x320a 
    /*00001ef0*/ BL          mn34220_write_register
    /*00001ef4*/ MOV         R0, R4 
    /*00001ef8*/ MOV         R2, #9 
    /*00001efc*/ MOVW        R1, #12811                  @ 0x320b 
    /*00001f00*/ BL          mn34220_write_register
    /*00001f04*/ MOV         R0, R4 
    /*00001f08*/ MOV         R2, #0 
    /*00001f0c*/ MOVW        R1, #12812                  @ 0x320c 
    /*00001f10*/ BL          mn34220_write_register
    /*00001f14*/ MOV         R0, R4 
    /*00001f18*/ MOV         R2, #26 
    /*00001f1c*/ MOVW        R1, #12813                  @ 0x320d 
    /*00001f20*/ BL          mn34220_write_register
    /*00001f24*/ MOV         R0, R4 
    /*00001f28*/ MOV         R2, #0 
    /*00001f2c*/ MOVW        R1, #12814                  @ 0x320e 
    /*00001f30*/ BL          mn34220_write_register
    /*00001f34*/ MOV         R0, R4 
    /*00001f38*/ MOV         R2, #27 
    /*00001f3c*/ MOVW        R1, #12815                  @ 0x320f 
    /*00001f40*/ BL          mn34220_write_register
    /*00001f44*/ MOV         R0, R4 
    /*00001f48*/ MOV         R2, #0 
    /*00001f4c*/ MOVW        R1, #12816                  @ 0x3210 
    /*00001f50*/ BL          mn34220_write_register
    /*00001f54*/ MOV         R0, R4 
    /*00001f58*/ MOV         R2, #27 
    /*00001f5c*/ MOVW        R1, #12817                  @ 0x3211 
    /*00001f60*/ BL          mn34220_write_register
    /*00001f64*/ MOV         R0, R4 
    /*00001f68*/ MOV         R2, #3 
    /*00001f6c*/ MOVW        R1, #12818                  @ 0x3212 
    /*00001f70*/ BL          mn34220_write_register
    /*00001f74*/ MOV         R0, R4 
    /*00001f78*/ MOV         R2, #11 
    /*00001f7c*/ MOVW        R1, #12819                  @ 0x3213 
    /*00001f80*/ BL          mn34220_write_register
    /*00001f84*/ MOV         R0, R4 
    /*00001f88*/ MOV         R2, #0 
    /*00001f8c*/ MOVW        R1, #12820                  @ 0x3214 
    /*00001f90*/ BL          mn34220_write_register
    /*00001f94*/ MOV         R0, R4 
    /*00001f98*/ MOV         R2, #0 
    /*00001f9c*/ MOVW        R1, #12821                  @ 0x3215 
    /*00001fa0*/ BL          mn34220_write_register
    /*00001fa4*/ MOV         R0, R4 
    /*00001fa8*/ MOV         R2, #0 
    /*00001fac*/ MOVW        R1, #12822                  @ 0x3216 
    /*00001fb0*/ BL          mn34220_write_register
    /*00001fb4*/ MOV         R0, R4 
    /*00001fb8*/ MOV         R2, #0 
    /*00001fbc*/ MOVW        R1, #12823                  @ 0x3217 
    /*00001fc0*/ BL          mn34220_write_register
    /*00001fc4*/ MOV         R0, R4 
    /*00001fc8*/ MOV         R2, #0 
    /*00001fcc*/ MOVW        R1, #12824                  @ 0x3218 
    /*00001fd0*/ BL          mn34220_write_register
    /*00001fd4*/ MOV         R0, R4 
    /*00001fd8*/ MOV         R2, #0 
    /*00001fdc*/ MOVW        R1, #12825                  @ 0x3219 
    /*00001fe0*/ BL          mn34220_write_register
    /*00001fe4*/ MOV         R0, R4 
    /*00001fe8*/ MOV         R2, #0 
    /*00001fec*/ MOVW        R1, #12826                  @ 0x321a 
    /*00001ff0*/ BL          mn34220_write_register
    /*00001ff4*/ MOV         R0, R4 
    /*00001ff8*/ MOV         R2, #0 
    /*00001ffc*/ MOVW        R1, #12827                  @ 0x321b 
    /*00002000*/ BL          mn34220_write_register
    /*00002004*/ MOV         R0, R4 
    /*00002008*/ MOV         R2, #0 
    /*0000200c*/ MOVW        R1, #12828                  @ 0x321c 
    /*00002010*/ BL          mn34220_write_register
    /*00002014*/ MOV         R0, R4 
    /*00002018*/ MOV         R2, #0 
    /*0000201c*/ MOVW        R1, #12829                  @ 0x321d 
    /*00002020*/ BL          mn34220_write_register
    /*00002024*/ MOV         R0, R4 
    /*00002028*/ MOV         R2, #0 
    /*0000202c*/ MOVW        R1, #12830                  @ 0x321e 
    /*00002030*/ BL          mn34220_write_register
    /*00002034*/ MOV         R0, R4 
    /*00002038*/ MOV         R2, #0 
    /*0000203c*/ MOVW        R1, #12831                  @ 0x321f 
    /*00002040*/ BL          mn34220_write_register
    /*00002044*/ MOV         R0, R4 
    /*00002048*/ MOV         R2, #0 
    /*0000204c*/ MOVW        R1, #12832                  @ 0x3220 
    /*00002050*/ BL          mn34220_write_register
    /*00002054*/ MOV         R0, R4 
    /*00002058*/ MOV         R2, #0 
    /*0000205c*/ MOVW        R1, #12833                  @ 0x3221 
    /*00002060*/ BL          mn34220_write_register
    /*00002064*/ MOV         R0, R4 
    /*00002068*/ MOV         R2, #255                    @ 0xff 
    /*0000206c*/ MOVW        R1, #12834                  @ 0x3222 
    /*00002070*/ BL          mn34220_write_register
    /*00002074*/ MOV         R0, R4 
    /*00002078*/ MOV         R2, #255                    @ 0xff 
    /*0000207c*/ MOVW        R1, #12835                  @ 0x3223 
    /*00002080*/ BL          mn34220_write_register
    /*00002084*/ MOV         R0, R4 
    /*00002088*/ MOV         R2, #255                    @ 0xff 
    /*0000208c*/ MOVW        R1, #12836                  @ 0x3224 
    /*00002090*/ BL          mn34220_write_register
    /*00002094*/ MOV         R0, R4 
    /*00002098*/ MOV         R2, #255                    @ 0xff 
    /*0000209c*/ MOVW        R1, #12837                  @ 0x3225 
    /*000020a0*/ BL          mn34220_write_register
    /*000020a4*/ MOV         R0, R4 
    /*000020a8*/ MOV         R2, #255                    @ 0xff 
    /*000020ac*/ MOVW        R1, #12838                  @ 0x3226 
    /*000020b0*/ BL          mn34220_write_register
    /*000020b4*/ MOV         R0, R4 
    /*000020b8*/ MOV         R2, #255                    @ 0xff 
    /*000020bc*/ MOVW        R1, #12839                  @ 0x3227 
    /*000020c0*/ BL          mn34220_write_register
    /*000020c4*/ MOV         R0, R4 
    /*000020c8*/ MOV         R2, #255                    @ 0xff 
    /*000020cc*/ MOVW        R1, #12840                  @ 0x3228 
    /*000020d0*/ BL          mn34220_write_register
    /*000020d4*/ MOV         R0, R4 
    /*000020d8*/ MOV         R2, #255                    @ 0xff 
    /*000020dc*/ MOVW        R1, #12841                  @ 0x3229 
    /*000020e0*/ BL          mn34220_write_register
    /*000020e4*/ MOV         R0, R4 
    /*000020e8*/ MOV         R2, #255                    @ 0xff 
    /*000020ec*/ MOVW        R1, #12842                  @ 0x322a 
    /*000020f0*/ BL          mn34220_write_register
    /*000020f4*/ MOV         R0, R4 
    /*000020f8*/ MOV         R2, #255                    @ 0xff 
    /*000020fc*/ MOVW        R1, #12843                  @ 0x322b 
    /*00002100*/ BL          mn34220_write_register
    /*00002104*/ MOV         R0, R4 
    /*00002108*/ MOV         R2, #255                    @ 0xff 
    /*0000210c*/ MOVW        R1, #12844                  @ 0x322c 
    /*00002110*/ BL          mn34220_write_register
    /*00002114*/ MOV         R0, R4 
    /*00002118*/ MOV         R2, #255                    @ 0xff 
    /*0000211c*/ MOVW        R1, #12845                  @ 0x322d 
    /*00002120*/ BL          mn34220_write_register
    /*00002124*/ MOV         R0, R4 
    /*00002128*/ MOV         R2, #255                    @ 0xff 
    /*0000212c*/ MOVW        R1, #12846                  @ 0x322e 
    /*00002130*/ BL          mn34220_write_register
    /*00002134*/ MOV         R0, R4 
    /*00002138*/ MOV         R2, #255                    @ 0xff 
    /*0000213c*/ MOVW        R1, #12847                  @ 0x322f 
    /*00002140*/ BL          mn34220_write_register
    /*00002144*/ MOV         R0, R4 
    /*00002148*/ MOV         R2, #255                    @ 0xff 
    /*0000214c*/ MOVW        R1, #12848                  @ 0x3230 
    /*00002150*/ BL          mn34220_write_register
    /*00002154*/ MOV         R0, R4 
    /*00002158*/ MOV         R2, #255                    @ 0xff 
    /*0000215c*/ MOVW        R1, #12849                  @ 0x3231 
    /*00002160*/ BL          mn34220_write_register
    /*00002164*/ MOV         R0, R4 
    /*00002168*/ MOV         R2, #0 
    /*0000216c*/ MOVW        R1, #12850                  @ 0x3232 
    /*00002170*/ BL          mn34220_write_register
    /*00002174*/ MOV         R0, R4 
    /*00002178*/ MOV         R2, #0 
    /*0000217c*/ MOVW        R1, #12851                  @ 0x3233 
    /*00002180*/ BL          mn34220_write_register
    /*00002184*/ MOV         R0, R4 
    /*00002188*/ MOV         R2, #1 
    /*0000218c*/ MOVW        R1, #12852                  @ 0x3234 
    /*00002190*/ BL          mn34220_write_register
    /*00002194*/ MOV         R0, R4 
    /*00002198*/ MOV         R2, #186                    @ 0xba 
    /*0000219c*/ MOVW        R1, #12853                  @ 0x3235 
    /*000021a0*/ BL          mn34220_write_register
    /*000021a4*/ MOV         R0, R4 
    /*000021a8*/ MOV         R2, #0 
    /*000021ac*/ MOVW        R1, #12854                  @ 0x3236 
    /*000021b0*/ BL          mn34220_write_register
    /*000021b4*/ MOV         R0, R4 
    /*000021b8*/ MOV         R2, #0 
    /*000021bc*/ MOVW        R1, #12855                  @ 0x3237 
    /*000021c0*/ BL          mn34220_write_register
    /*000021c4*/ MOV         R0, R4 
    /*000021c8*/ MOV         R2, #1 
    /*000021cc*/ MOVW        R1, #12856                  @ 0x3238 
    /*000021d0*/ BL          mn34220_write_register
    /*000021d4*/ MOV         R0, R4 
    /*000021d8*/ MOV         R2, #186                    @ 0xba 
    /*000021dc*/ MOVW        R1, #12857                  @ 0x3239 
    /*000021e0*/ BL          mn34220_write_register
    /*000021e4*/ MOV         R0, R4 
    /*000021e8*/ MOV         R2, #1 
    /*000021ec*/ MOVW        R1, #12858                  @ 0x323a 
    /*000021f0*/ BL          mn34220_write_register
    /*000021f4*/ MOV         R0, R4 
    /*000021f8*/ MOV         R2, #129                    @ 0x81 
    /*000021fc*/ MOVW        R1, #12859                  @ 0x323b 
    /*00002200*/ BL          mn34220_write_register
    /*00002204*/ MOV         R0, R4 
    /*00002208*/ MOV         R2, #113                    @ 0x71 
    /*0000220c*/ MOVW        R1, #12860                  @ 0x323c 
    /*00002210*/ BL          mn34220_write_register
    /*00002214*/ MOV         R0, R4 
    /*00002218*/ MOV         R2, #128                    @ 0x80 
    /*0000221c*/ MOVW        R1, #12861                  @ 0x323d 
    /*00002220*/ BL          mn34220_write_register
    /*00002224*/ MOV         R0, R4 
    /*00002228*/ MOV         R2, #1 
    /*0000222c*/ MOVW        R1, #12862                  @ 0x323e 
    /*00002230*/ BL          mn34220_write_register
    /*00002234*/ MOV         R0, R4 
    /*00002238*/ MOV         R2, #0 
    /*0000223c*/ MOVW        R1, #12863                  @ 0x323f 
    /*00002240*/ BL          mn34220_write_register
    /*00002244*/ MOV         R0, R4 
    /*00002248*/ MOV         R2, #21 
    /*0000224c*/ MOV         R1, #12864                  @ 0x3240 
    /*00002250*/ BL          mn34220_write_register
    /*00002254*/ MOV         R0, R4 
    /*00002258*/ MOV         R2, #17 
    /*0000225c*/ MOVW        R1, #12865                  @ 0x3241 
    /*00002260*/ BL          mn34220_write_register
    /*00002264*/ MOV         R0, R4 
    /*00002268*/ MOV         R2, #0 
    /*0000226c*/ MOVW        R1, #12866                  @ 0x3242 
    /*00002270*/ BL          mn34220_write_register
    /*00002274*/ MOV         R0, R4 
    /*00002278*/ MOV         R2, #215                    @ 0xd7 
    /*0000227c*/ MOVW        R1, #12867                  @ 0x3243 
    /*00002280*/ BL          mn34220_write_register
    /*00002284*/ MOV         R0, R4 
    /*00002288*/ MOV         R2, #0 
    /*0000228c*/ MOVW        R1, #12868                  @ 0x3244 
    /*00002290*/ BL          mn34220_write_register
    /*00002294*/ MOV         R0, R4 
    /*00002298*/ MOV         R2, #24 
    /*0000229c*/ MOVW        R1, #12869                  @ 0x3245 
    /*000022a0*/ BL          mn34220_write_register
    /*000022a4*/ MOV         R0, R4 
    /*000022a8*/ MOV         R2, #1 
    /*000022ac*/ MOVW        R1, #12870                  @ 0x3246 
    /*000022b0*/ BL          mn34220_write_register
    /*000022b4*/ MOV         R0, R4 
    /*000022b8*/ MOV         R2, #113                    @ 0x71 
    /*000022bc*/ MOVW        R1, #12871                  @ 0x3247 
    /*000022c0*/ BL          mn34220_write_register
    /*000022c4*/ MOV         R0, R4 
    /*000022c8*/ MOV         R2, #0 
    /*000022cc*/ MOVW        R1, #12872                  @ 0x3248 
    /*000022d0*/ BL          mn34220_write_register
    /*000022d4*/ MOV         R0, R4 
    /*000022d8*/ MOV         R2, #0 
    /*000022dc*/ MOVW        R1, #12873                  @ 0x3249 
    /*000022e0*/ BL          mn34220_write_register
    /*000022e4*/ MOV         R0, R4 
    /*000022e8*/ MOV         R2, #48                     @ 0x30 
    /*000022ec*/ MOVW        R1, #12874                  @ 0x324a 
    /*000022f0*/ BL          mn34220_write_register
    /*000022f4*/ MOV         R0, R4 
    /*000022f8*/ MOV         R2, #52                     @ 0x34 
    /*000022fc*/ MOVW        R1, #12875                  @ 0x324b 
    /*00002300*/ BL          mn34220_write_register
    /*00002304*/ MOV         R0, R4 
    /*00002308*/ MOV         R2, #2 
    /*0000230c*/ MOVW        R1, #12876                  @ 0x324c 
    /*00002310*/ BL          mn34220_write_register
    /*00002314*/ MOV         R0, R4 
    /*00002318*/ MOV         R2, #0 
    /*0000231c*/ MOVW        R1, #12877                  @ 0x324d 
    /*00002320*/ BL          mn34220_write_register
    /*00002324*/ MOV         R0, R4 
    /*00002328*/ MOV         R2, #0 
    /*0000232c*/ MOVW        R1, #12878                  @ 0x324e 
    /*00002330*/ BL          mn34220_write_register
    /*00002334*/ MOV         R0, R4 
    /*00002338*/ MOV         R2, #24 
    /*0000233c*/ MOVW        R1, #12879                  @ 0x324f 
    /*00002340*/ BL          mn34220_write_register
    /*00002344*/ MOV         R0, R4 
    /*00002348*/ MOV         R2, #49                     @ 0x31 
    /*0000234c*/ MOVW        R1, #12880                  @ 0x3250 
    /*00002350*/ BL          mn34220_write_register
    /*00002354*/ MOV         R0, R4 
    /*00002358*/ MOV         R2, #14 
    /*0000235c*/ MOVW        R1, #12881                  @ 0x3251 
    /*00002360*/ BL          mn34220_write_register
    /*00002364*/ MOV         R0, R4 
    /*00002368*/ MOV         R2, #0 
    /*0000236c*/ MOVW        R1, #12882                  @ 0x3252 
    /*00002370*/ BL          mn34220_write_register
    /*00002374*/ MOV         R0, R4 
    /*00002378*/ MOV         R2, #222                    @ 0xde 
    /*0000237c*/ MOVW        R1, #12883                  @ 0x3253 
    /*00002380*/ BL          mn34220_write_register
    /*00002384*/ MOV         R0, R4 
    /*00002388*/ MOV         R2, #3 
    /*0000238c*/ MOVW        R1, #12884                  @ 0x3254 
    /*00002390*/ BL          mn34220_write_register
    /*00002394*/ MOV         R0, R4 
    /*00002398*/ MOV         R2, #8 
    /*0000239c*/ MOVW        R1, #12885                  @ 0x3255 
    /*000023a0*/ BL          mn34220_write_register
    /*000023a4*/ MOV         R0, R4 
    /*000023a8*/ MOV         R2, #17 
    /*000023ac*/ MOVW        R1, #12886                  @ 0x3256 
    /*000023b0*/ BL          mn34220_write_register
    /*000023b4*/ MOV         R0, R4 
    /*000023b8*/ MOV         R2, #0 
    /*000023bc*/ MOVW        R1, #12887                  @ 0x3257 
    /*000023c0*/ BL          mn34220_write_register
    /*000023c4*/ MOV         R0, R4 
    /*000023c8*/ MOV         R2, #1 
    /*000023cc*/ MOVW        R1, #12888                  @ 0x3258 
    /*000023d0*/ BL          mn34220_write_register
    /*000023d4*/ MOV         R0, R4 
    /*000023d8*/ MOV         R2, #65                     @ 0x41 
    /*000023dc*/ MOVW        R1, #12889                  @ 0x3259 
    /*000023e0*/ BL          mn34220_write_register
    /*000023e4*/ MOV         R0, R4 
    /*000023e8*/ MOV         R2, #57                     @ 0x39 
    /*000023ec*/ MOVW        R1, #12890                  @ 0x325a 
    /*000023f0*/ BL          mn34220_write_register
    /*000023f4*/ MOV         R0, R4 
    /*000023f8*/ MOV         R2, #0 
    /*000023fc*/ MOVW        R1, #12891                  @ 0x325b 
    /*00002400*/ BL          mn34220_write_register
    /*00002404*/ MOV         R0, R4 
    /*00002408*/ MOV         R2, #0 
    /*0000240c*/ MOVW        R1, #12892                  @ 0x325c 
    /*00002410*/ BL          mn34220_write_register
    /*00002414*/ MOV         R0, R4 
    /*00002418*/ MOV         R2, #12 
    /*0000241c*/ MOVW        R1, #12893                  @ 0x325d 
    /*00002420*/ BL          mn34220_write_register
    /*00002424*/ MOV         R0, R4 
    /*00002428*/ MOV         R2, #132                    @ 0x84 
    /*0000242c*/ MOVW        R1, #12894                  @ 0x325e 
    /*00002430*/ BL          mn34220_write_register
    /*00002434*/ MOV         R0, R4 
    /*00002438*/ MOV         R2, #0 
    /*0000243c*/ MOVW        R1, #12895                  @ 0x325f 
    /*00002440*/ BL          mn34220_write_register
    /*00002444*/ MOV         R0, R4 
    /*00002448*/ MOV         R2, #0 
    /*0000244c*/ MOVW        R1, #12896                  @ 0x3260 
    /*00002450*/ BL          mn34220_write_register
    /*00002454*/ MOV         R0, R4 
    /*00002458*/ MOV         R2, #0 
    /*0000245c*/ MOVW        R1, #12897                  @ 0x3261 
    /*00002460*/ BL          mn34220_write_register
    /*00002464*/ MOV         R0, R4 
    /*00002468*/ MOV         R2, #1 
    /*0000246c*/ MOVW        R1, #12912                  @ 0x3270 
    /*00002470*/ BL          mn34220_write_register
    /*00002474*/ MOV         R0, R4 
    /*00002478*/ MOV         R2, #0 
    /*0000247c*/ MOVW        R1, #12913                  @ 0x3271 
    /*00002480*/ BL          mn34220_write_register
    /*00002484*/ MOV         R0, R4 
    /*00002488*/ MOV         R2, #70                     @ 0x46 
    /*0000248c*/ MOVW        R1, #12914                  @ 0x3272 
    /*00002490*/ BL          mn34220_write_register
    /*00002494*/ MOV         R0, R4 
    /*00002498*/ MOV         R2, #171                    @ 0xab 
    /*0000249c*/ MOVW        R1, #12915                  @ 0x3273 
    /*000024a0*/ BL          mn34220_write_register
    /*000024a4*/ MOV         R0, R4 
    /*000024a8*/ MOV         R2, #1 
    /*000024ac*/ MOVW        R1, #12916                  @ 0x3274 
    /*000024b0*/ BL          mn34220_write_register
    /*000024b4*/ MOV         R0, R4 
    /*000024b8*/ MOV         R2, #24 
    /*000024bc*/ MOVW        R1, #12917                  @ 0x3275 
    /*000024c0*/ BL          mn34220_write_register
    /*000024c4*/ MOV         R0, R4 
    /*000024c8*/ MOV         R2, #48                     @ 0x30 
    /*000024cc*/ MOV         R1, #12928                  @ 0x3280 
    /*000024d0*/ BL          mn34220_write_register
    /*000024d4*/ MOV         R0, R4 
    /*000024d8*/ MOV         R2, #16 
    /*000024dc*/ MOVW        R1, #12929                  @ 0x3281 
    /*000024e0*/ BL          mn34220_write_register
    /*000024e4*/ MOV         R0, R4 
    /*000024e8*/ MOV         R2, #14 
    /*000024ec*/ MOVW        R1, #12930                  @ 0x3282 
    /*000024f0*/ BL          mn34220_write_register
    /*000024f4*/ MOV         R0, R4 
    /*000024f8*/ MOV         R2, #0 
    /*000024fc*/ MOVW        R1, #12931                  @ 0x3283 
    /*00002500*/ BL          mn34220_write_register
    /*00002504*/ MOV         R0, R4 
    /*00002508*/ MOV         R2, #0 
    /*0000250c*/ MOVW        R1, #12932                  @ 0x3284 
    /*00002510*/ BL          mn34220_write_register
    /*00002514*/ MOV         R0, R4 
    /*00002518*/ MOV         R2, #27 
    /*0000251c*/ MOVW        R1, #12933                  @ 0x3285 
    /*00002520*/ BL          mn34220_write_register
    /*00002524*/ MOV         R0, R4 
    /*00002528*/ MOV         R2, #24 
    /*0000252c*/ MOVW        R1, #12934                  @ 0x3286 
    /*00002530*/ BL          mn34220_write_register
    /*00002534*/ MOV         R0, R4 
    /*00002538*/ MOV         R2, #0 
    /*0000253c*/ MOVW        R1, #12935                  @ 0x3287 
    /*00002540*/ BL          mn34220_write_register
    /*00002544*/ MOV         R0, R4 
    /*00002548*/ MOV         R2, #1 
    /*0000254c*/ MOVW        R1, #12936                  @ 0x3288 
    /*00002550*/ BL          mn34220_write_register
    /*00002554*/ MOV         R0, R4 
    /*00002558*/ MOV         R2, #0 
    /*0000255c*/ MOVW        R1, #12937                  @ 0x3289 
    /*00002560*/ BL          mn34220_write_register
    /*00002564*/ MOV         R0, R4 
    /*00002568*/ MOV         R2, #232                    @ 0xe8 
    /*0000256c*/ MOVW        R1, #12938                  @ 0x328a 
    /*00002570*/ BL          mn34220_write_register
    /*00002574*/ MOV         R0, R4 
    /*00002578*/ MOV         R2, #4 
    /*0000257c*/ MOVW        R1, #12939                  @ 0x328b 
    /*00002580*/ BL          mn34220_write_register
    /*00002584*/ MOV         R0, R4 
    /*00002588*/ MOV         R2, #117                    @ 0x75 
    /*0000258c*/ MOVW        R1, #12940                  @ 0x328c 
    /*00002590*/ BL          mn34220_write_register
    /*00002594*/ MOV         R0, R4 
    /*00002598*/ MOV         R2, #0 
    /*0000259c*/ MOVW        R1, #12941                  @ 0x328d 
    /*000025a0*/ BL          mn34220_write_register
    /*000025a4*/ MOV         R0, R4 
    /*000025a8*/ MOV         R2, #3 
    /*000025ac*/ MOV         R1, #13056                  @ 0x3300 
    /*000025b0*/ BL          mn34220_write_register
    /*000025b4*/ MOV         R0, R4 
    /*000025b8*/ MOV         R2, #1 
    /*000025bc*/ MOVW        R1, #13057                  @ 0x3301 
    /*000025c0*/ BL          mn34220_write_register
    /*000025c4*/ MOV         R0, R4 
    /*000025c8*/ MOV         R2, #0 
    /*000025cc*/ MOVW        R1, #13058                  @ 0x3302 
    /*000025d0*/ BL          mn34220_write_register
    /*000025d4*/ MOV         R0, R4 
    /*000025d8*/ MOV         R2, #7 
    /*000025dc*/ MOVW        R1, #13059                  @ 0x3303 
    /*000025e0*/ BL          mn34220_write_register
    /*000025e4*/ MOV         R0, R4 
    /*000025e8*/ MOV         R2, #6 
    /*000025ec*/ MOVW        R1, #13060                  @ 0x3304 
    /*000025f0*/ BL          mn34220_write_register
    /*000025f4*/ MOV         R0, R4 
    /*000025f8*/ MOV         R2, #10 
    /*000025fc*/ MOVW        R1, #13061                  @ 0x3305 
    /*00002600*/ BL          mn34220_write_register
    /*00002604*/ MOV         R0, R4 
    /*00002608*/ MOV         R2, #25 
    /*0000260c*/ MOVW        R1, #13062                  @ 0x3306 
    /*00002610*/ BL          mn34220_write_register
    /*00002614*/ MOV         R0, R4 
    /*00002618*/ MOV         R2, #25 
    /*0000261c*/ MOVW        R1, #13063                  @ 0x3307 
    /*00002620*/ BL          mn34220_write_register
    /*00002624*/ MOV         R0, R4 
    /*00002628*/ MOV         R2, #0 
    /*0000262c*/ MOVW        R1, #13064                  @ 0x3308 
    /*00002630*/ BL          mn34220_write_register
    /*00002634*/ MOV         R0, R4 
    /*00002638*/ MOV         R2, #0 
    /*0000263c*/ MOVW        R1, #13065                  @ 0x3309 
    /*00002640*/ BL          mn34220_write_register
    /*00002644*/ MOV         R0, R4 
    /*00002648*/ MOV         R2, #30 
    /*0000264c*/ MOVW        R1, #13066                  @ 0x330a 
    /*00002650*/ BL          mn34220_write_register
    /*00002654*/ MOV         R0, R4 
    /*00002658*/ MOV         R2, #30 
    /*0000265c*/ MOVW        R1, #13067                  @ 0x330b 
    /*00002660*/ BL          mn34220_write_register
    /*00002664*/ MOV         R0, R4 
    /*00002668*/ MOV         R2, #0 
    /*0000266c*/ MOVW        R1, #13068                  @ 0x330c 
    /*00002670*/ BL          mn34220_write_register
    /*00002674*/ MOV         R0, R4 
    /*00002678*/ MOV         R2, #0 
    /*0000267c*/ MOVW        R1, #13069                  @ 0x330d 
    /*00002680*/ BL          mn34220_write_register
    /*00002684*/ MOV         R0, R4 
    /*00002688*/ MOV         R2, #3 
    /*0000268c*/ MOVW        R1, #13070                  @ 0x330e 
    /*00002690*/ BL          mn34220_write_register
    /*00002694*/ MOV         R0, R4 
    /*00002698*/ MOV         R2, #17 
    /*0000269c*/ MOVW        R1, #13071                  @ 0x330f 
    /*000026a0*/ BL          mn34220_write_register
    /*000026a4*/ MOV         R0, R4 
    /*000026a8*/ MOV         R2, #2 
    /*000026ac*/ MOVW        R1, #13072                  @ 0x3310 
    /*000026b0*/ BL          mn34220_write_register
    /*000026b4*/ MOV         R0, R4 
    /*000026b8*/ MOV         R2, #0 
    /*000026bc*/ MOVW        R1, #13073                  @ 0x3311 
    /*000026c0*/ BL          mn34220_write_register
    /*000026c4*/ MOV         R0, R4 
    /*000026c8*/ MOV         R2, #0 
    /*000026cc*/ MOVW        R1, #13074                  @ 0x3312 
    /*000026d0*/ BL          mn34220_write_register
    /*000026d4*/ MOV         R0, R4 
    /*000026d8*/ MOV         R2, #0 
    /*000026dc*/ MOVW        R1, #13075                  @ 0x3313 
    /*000026e0*/ BL          mn34220_write_register
    /*000026e4*/ MOV         R0, R4 
    /*000026e8*/ MOV         R2, #3 
    /*000026ec*/ MOVW        R1, #13076                  @ 0x3314 
    /*000026f0*/ BL          mn34220_write_register
    /*000026f4*/ MOV         R0, R4 
    /*000026f8*/ MOV         R2, #25 
    /*000026fc*/ MOVW        R1, #13077                  @ 0x3315 
    /*00002700*/ BL          mn34220_write_register
    /*00002704*/ MOV         R0, R4 
    /*00002708*/ MOV         R2, #10 
    /*0000270c*/ MOVW        R1, #13078                  @ 0x3316 
    /*00002710*/ BL          mn34220_write_register
    /*00002714*/ MOV         R0, R4 
    /*00002718*/ MOV         R2, #0 
    /*0000271c*/ MOVW        R1, #13079                  @ 0x3317 
    /*00002720*/ BL          mn34220_write_register
    /*00002724*/ MOV         R0, R4 
    /*00002728*/ MOV         R2, #6 
    /*0000272c*/ MOVW        R1, #13080                  @ 0x3318 
    /*00002730*/ BL          mn34220_write_register
    /*00002734*/ MOV         R0, R4 
    /*00002738*/ MOV         R2, #0 
    /*0000273c*/ MOVW        R1, #13081                  @ 0x3319 
    /*00002740*/ BL          mn34220_write_register
    /*00002744*/ MOV         R0, R4 
    /*00002748*/ MOV         R2, #2 
    /*0000274c*/ MOVW        R1, #13082                  @ 0x331a 
    /*00002750*/ BL          mn34220_write_register
    /*00002754*/ MOV         R0, R4 
    /*00002758*/ MOV         R2, #2 
    /*0000275c*/ MOVW        R1, #13083                  @ 0x331b 
    /*00002760*/ BL          mn34220_write_register
    /*00002764*/ MOV         R0, R4 
    /*00002768*/ MOV         R2, #0 
    /*0000276c*/ MOVW        R1, #13084                  @ 0x331c 
    /*00002770*/ BL          mn34220_write_register
    /*00002774*/ MOV         R0, R4 
    /*00002778*/ MOV         R2, #0 
    /*0000277c*/ MOVW        R1, #13085                  @ 0x331d 
    /*00002780*/ BL          mn34220_write_register
    /*00002784*/ MOV         R0, R4 
    /*00002788*/ MOV         R2, #3 
    /*0000278c*/ MOVW        R1, #13086                  @ 0x331e 
    /*00002790*/ BL          mn34220_write_register
    /*00002794*/ MOV         R0, R4 
    /*00002798*/ MOV         R2, #3 
    /*0000279c*/ MOVW        R1, #13087                  @ 0x331f 
    /*000027a0*/ BL          mn34220_write_register
    /*000027a4*/ MOV         R0, R4 
    /*000027a8*/ MOV         R2, #1 
    /*000027ac*/ MOVW        R1, #13088                  @ 0x3320 
    /*000027b0*/ BL          mn34220_write_register
    /*000027b4*/ MOV         R0, R4 
    /*000027b8*/ MOV         R2, #1 
    /*000027bc*/ MOVW        R1, #13089                  @ 0x3321 
    /*000027c0*/ BL          mn34220_write_register
    /*000027c4*/ MOV         R0, R4 
    /*000027c8*/ MOV         R2, #0 
    /*000027cc*/ MOVW        R1, #13090                  @ 0x3322 
    /*000027d0*/ BL          mn34220_write_register
    /*000027d4*/ MOV         R0, R4 
    /*000027d8*/ MOV         R2, #0 
    /*000027dc*/ MOVW        R1, #13091                  @ 0x3323 
    /*000027e0*/ BL          mn34220_write_register
    /*000027e4*/ MOV         R0, R4 
    /*000027e8*/ MOV         R2, #0 
    /*000027ec*/ MOVW        R1, #13092                  @ 0x3324 
    /*000027f0*/ BL          mn34220_write_register
    /*000027f4*/ MOV         R0, R4 
    /*000027f8*/ MOV         R2, #0 
    /*000027fc*/ MOVW        R1, #13093                  @ 0x3325 
    /*00002800*/ BL          mn34220_write_register
    /*00002804*/ MOV         R0, R4 
    /*00002808*/ MOV         R2, #10 
    /*0000280c*/ MOVW        R1, #13094                  @ 0x3326 
    /*00002810*/ BL          mn34220_write_register
    /*00002814*/ MOV         R0, R4 
    /*00002818*/ MOV         R2, #0 
    /*0000281c*/ MOVW        R1, #13095                  @ 0x3327 
    /*00002820*/ BL          mn34220_write_register
    /*00002824*/ MOV         R0, R4 
    /*00002828*/ MOV         R2, #0 
    /*0000282c*/ MOVW        R1, #13096                  @ 0x3328 
    /*00002830*/ BL          mn34220_write_register
    /*00002834*/ MOV         R0, R4 
    /*00002838*/ MOV         R2, #0 
    /*0000283c*/ MOVW        R1, #13097                  @ 0x3329 
    /*00002840*/ BL          mn34220_write_register
    /*00002844*/ MOV         R0, R4 
    /*00002848*/ MOV         R2, #3 
    /*0000284c*/ MOVW        R1, #13098                  @ 0x332a 
    /*00002850*/ BL          mn34220_write_register
    /*00002854*/ MOV         R0, R4 
    /*00002858*/ MOV         R2, #3 
    /*0000285c*/ MOVW        R1, #13099                  @ 0x332b 
    /*00002860*/ BL          mn34220_write_register
    /*00002864*/ MOV         R0, R4 
    /*00002868*/ MOV         R2, #2 
    /*0000286c*/ MOVW        R1, #13100                  @ 0x332c 
    /*00002870*/ BL          mn34220_write_register
    /*00002874*/ MOV         R0, R4 
    /*00002878*/ MOV         R2, #0 
    /*0000287c*/ MOVW        R1, #13101                  @ 0x332d 
    /*00002880*/ BL          mn34220_write_register
    /*00002884*/ MOV         R0, R4 
    /*00002888*/ MOV         R2, #10 
    /*0000288c*/ MOVW        R1, #13102                  @ 0x332e 
    /*00002890*/ BL          mn34220_write_register
    /*00002894*/ MOV         R0, R4 
    /*00002898*/ MOV         R2, #0 
    /*0000289c*/ MOVW        R1, #13103                  @ 0x332f 
    /*000028a0*/ BL          mn34220_write_register
    /*000028a4*/ MOV         R0, R4 
    /*000028a8*/ MOV         R2, #6 
    /*000028ac*/ MOVW        R1, #13104                  @ 0x3330 
    /*000028b0*/ BL          mn34220_write_register
    /*000028b4*/ MOV         R0, R4 
    /*000028b8*/ MOV         R2, #0 
    /*000028bc*/ MOVW        R1, #13105                  @ 0x3331 
    /*000028c0*/ BL          mn34220_write_register
    /*000028c4*/ MOV         R0, R4 
    /*000028c8*/ MOV         R2, #18 
    /*000028cc*/ MOVW        R1, #13106                  @ 0x3332 
    /*000028d0*/ BL          mn34220_write_register
    /*000028d4*/ MOV         R0, R4 
    /*000028d8*/ MOV         R2, #0 
    /*000028dc*/ MOVW        R1, #13107                  @ 0x3333 
    /*000028e0*/ BL          mn34220_write_register
    /*000028e4*/ MOV         R0, R4 
    /*000028e8*/ MOV         R2, #3 
    /*000028ec*/ MOVW        R1, #13108                  @ 0x3334 
    /*000028f0*/ BL          mn34220_write_register
    /*000028f4*/ MOV         R0, R4 
    /*000028f8*/ MOV         R2, #0 
    /*000028fc*/ MOVW        R1, #13109                  @ 0x3335 
    /*00002900*/ BL          mn34220_write_register
    /*00002904*/ MOV         R0, R4 
    /*00002908*/ MOV         R2, #3 
    /*0000290c*/ MOVW        R1, #13110                  @ 0x3336 
    /*00002910*/ BL          mn34220_write_register
    /*00002914*/ MOV         R0, R4 
    /*00002918*/ MOV         R2, #3 
    /*0000291c*/ MOVW        R1, #13111                  @ 0x3337 
    /*00002920*/ BL          mn34220_write_register
    /*00002924*/ MOV         R0, R4 
    /*00002928*/ MOV         R2, #0 
    /*0000292c*/ MOVW        R1, #13112                  @ 0x3338 
    /*00002930*/ BL          mn34220_write_register
    /*00002934*/ MOV         R0, R4 
    /*00002938*/ MOV         R2, #2 
    /*0000293c*/ MOVW        R1, #13113                  @ 0x3339 
    /*00002940*/ BL          mn34220_write_register
    /*00002944*/ MOV         R0, R4 
    /*00002948*/ MOV         R2, #8 
    /*0000294c*/ MOVW        R1, #13114                  @ 0x333a 
    /*00002950*/ BL          mn34220_write_register
    /*00002954*/ MOV         R0, R4 
    /*00002958*/ MOV         R2, #0 
    /*0000295c*/ MOVW        R1, #13115                  @ 0x333b 
    /*00002960*/ BL          mn34220_write_register
    /*00002964*/ MOV         R0, R4 
    /*00002968*/ MOV         R2, #0 
    /*0000296c*/ MOVW        R1, #13116                  @ 0x333c 
    /*00002970*/ BL          mn34220_write_register
    /*00002974*/ MOV         R0, R4 
    /*00002978*/ MOV         R2, #0 
    /*0000297c*/ MOVW        R1, #13117                  @ 0x333d 
    /*00002980*/ BL          mn34220_write_register
    /*00002984*/ MOV         R0, R4 
    /*00002988*/ MOV         R2, #0 
    /*0000298c*/ MOVW        R1, #13118                  @ 0x333e 
    /*00002990*/ BL          mn34220_write_register
    /*00002994*/ MOV         R0, R4 
    /*00002998*/ MOV         R2, #0 
    /*0000299c*/ MOVW        R1, #13119                  @ 0x333f 
    /*000029a0*/ BL          mn34220_write_register
    /*000029a4*/ MOV         R0, R4 
    /*000029a8*/ MOV         R2, #0 
    /*000029ac*/ MOV         R1, #13120                  @ 0x3340 
    /*000029b0*/ BL          mn34220_write_register
    /*000029b4*/ MOV         R0, R4 
    /*000029b8*/ MOV         R2, #0 
    /*000029bc*/ MOVW        R1, #13121                  @ 0x3341 
    /*000029c0*/ BL          mn34220_write_register
    /*000029c4*/ MOV         R0, R4 
    /*000029c8*/ MOV         R2, #0 
    /*000029cc*/ MOVW        R1, #13122                  @ 0x3342 
    /*000029d0*/ BL          mn34220_write_register
    /*000029d4*/ MOV         R0, R4 
    /*000029d8*/ MOV         R2, #0 
    /*000029dc*/ MOVW        R1, #13123                  @ 0x3343 
    /*000029e0*/ BL          mn34220_write_register
    /*000029e4*/ MOV         R0, R4 
    /*000029e8*/ MOV         R2, #0 
    /*000029ec*/ MOVW        R1, #13124                  @ 0x3344 
    /*000029f0*/ BL          mn34220_write_register
    /*000029f4*/ MOV         R0, R4 
    /*000029f8*/ MOV         R2, #0 
    /*000029fc*/ MOVW        R1, #13125                  @ 0x3345 
    /*00002a00*/ BL          mn34220_write_register
    /*00002a04*/ MOV         R0, R4 
    /*00002a08*/ MOV         R2, #0 
    /*00002a0c*/ MOVW        R1, #13126                  @ 0x3346 
    /*00002a10*/ BL          mn34220_write_register
    /*00002a14*/ MOV         R0, R4 
    /*00002a18*/ MOV         R2, #0 
    /*00002a1c*/ MOVW        R1, #13127                  @ 0x3347 
    /*00002a20*/ BL          mn34220_write_register
    /*00002a24*/ MOV         R0, R4 
    /*00002a28*/ MOV         R2, #7 
    /*00002a2c*/ MOVW        R1, #13128                  @ 0x3348 
    /*00002a30*/ BL          mn34220_write_register
    /*00002a34*/ MOV         R0, R4 
    /*00002a38*/ MOV         R2, #0 
    /*00002a3c*/ MOVW        R1, #13129                  @ 0x3349 
    /*00002a40*/ BL          mn34220_write_register
    /*00002a44*/ MOV         R0, R4 
    /*00002a48*/ MOV         R2, #5 
    /*00002a4c*/ MOVW        R1, #13130                  @ 0x334a 
    /*00002a50*/ BL          mn34220_write_register
    /*00002a54*/ MOV         R0, R4 
    /*00002a58*/ MOV         R2, #0 
    /*00002a5c*/ MOVW        R1, #13131                  @ 0x334b 
    /*00002a60*/ BL          mn34220_write_register
    /*00002a64*/ MOV         R0, R4 
    /*00002a68*/ MOV         R2, #5 
    /*00002a6c*/ MOVW        R1, #13132                  @ 0x334c 
    /*00002a70*/ BL          mn34220_write_register
    /*00002a74*/ MOV         R0, R4 
    /*00002a78*/ MOV         R2, #28 
    /*00002a7c*/ MOVW        R1, #13133                  @ 0x334d 
    /*00002a80*/ BL          mn34220_write_register
    /*00002a84*/ MOV         R0, R4 
    /*00002a88*/ MOV         R2, #0 
    /*00002a8c*/ MOVW        R1, #13134                  @ 0x334e 
    /*00002a90*/ BL          mn34220_write_register
    /*00002a94*/ MOV         R0, R4 
    /*00002a98*/ MOV         R2, #3 
    /*00002a9c*/ MOVW        R1, #13135                  @ 0x334f 
    /*00002aa0*/ BL          mn34220_write_register
    /*00002aa4*/ MOV         R0, R4 
    /*00002aa8*/ MOV         R2, #14 
    /*00002aac*/ MOVW        R1, #13136                  @ 0x3350 
    /*00002ab0*/ BL          mn34220_write_register
    /*00002ab4*/ MOV         R0, R4 
    /*00002ab8*/ MOV         R2, #8 
    /*00002abc*/ MOVW        R1, #13137                  @ 0x3351 
    /*00002ac0*/ BL          mn34220_write_register
    /*00002ac4*/ MOV         R0, R4 
    /*00002ac8*/ MOV         R2, #6 
    /*00002acc*/ MOVW        R1, #13138                  @ 0x3352 
    /*00002ad0*/ BL          mn34220_write_register
    /*00002ad4*/ MOV         R0, R4 
    /*00002ad8*/ MOV         R2, #5 
    /*00002adc*/ MOVW        R1, #13139                  @ 0x3353 
    /*00002ae0*/ BL          mn34220_write_register
    /*00002ae4*/ MOV         R0, R4 
    /*00002ae8*/ MOV         R2, #8 
    /*00002aec*/ MOVW        R1, #13140                  @ 0x3354 
    /*00002af0*/ BL          mn34220_write_register
    /*00002af4*/ MOV         R0, R4 
    /*00002af8*/ MOV         R2, #25 
    /*00002afc*/ MOVW        R1, #13141                  @ 0x3355 
    /*00002b00*/ BL          mn34220_write_register
    /*00002b04*/ MOV         R0, R4 
    /*00002b08*/ MOV         R2, #0 
    /*00002b0c*/ MOVW        R1, #13142                  @ 0x3356 
    /*00002b10*/ BL          mn34220_write_register
    /*00002b14*/ MOV         R0, R4 
    /*00002b18*/ MOV         R2, #3 
    /*00002b1c*/ MOVW        R1, #13143                  @ 0x3357 
    /*00002b20*/ BL          mn34220_write_register
    /*00002b24*/ MOV         R0, R4 
    /*00002b28*/ MOV         R2, #9 
    /*00002b2c*/ MOVW        R1, #13144                  @ 0x3358 
    /*00002b30*/ BL          mn34220_write_register
    /*00002b34*/ MOV         R0, R4 
    /*00002b38*/ MOV         R2, #11 
    /*00002b3c*/ MOVW        R1, #13145                  @ 0x3359 
    /*00002b40*/ BL          mn34220_write_register
    /*00002b44*/ MOV         R0, R4 
    /*00002b48*/ MOV         R2, #13 
    /*00002b4c*/ MOVW        R1, #13146                  @ 0x335a 
    /*00002b50*/ BL          mn34220_write_register
    /*00002b54*/ MOV         R0, R4 
    /*00002b58*/ MOV         R2, #0 
    /*00002b5c*/ MOVW        R1, #13147                  @ 0x335b 
    /*00002b60*/ BL          mn34220_write_register
    /*00002b64*/ MOV         R0, R4 
    /*00002b68*/ MOV         R2, #0 
    /*00002b6c*/ MOVW        R1, #13148                  @ 0x335c 
    /*00002b70*/ BL          mn34220_write_register
    /*00002b74*/ MOV         R0, R4 
    /*00002b78*/ MOV         R2, #1 
    /*00002b7c*/ MOVW        R1, #13149                  @ 0x335d 
    /*00002b80*/ BL          mn34220_write_register
    /*00002b84*/ MOV         R0, R4 
    /*00002b88*/ MOV         R2, #13 
    /*00002b8c*/ MOVW        R1, #13150                  @ 0x335e 
    /*00002b90*/ BL          mn34220_write_register
    /*00002b94*/ MOV         R0, R4 
    /*00002b98*/ MOV         R2, #1 
    /*00002b9c*/ MOVW        R1, #13151                  @ 0x335f 
    /*00002ba0*/ BL          mn34220_write_register
    /*00002ba4*/ MOV         R0, R4 
    /*00002ba8*/ MOV         R2, #13 
    /*00002bac*/ MOVW        R1, #13152                  @ 0x3360 
    /*00002bb0*/ BL          mn34220_write_register
    /*00002bb4*/ MOV         R0, R4 
    /*00002bb8*/ MOV         R2, #7 
    /*00002bbc*/ MOVW        R1, #13153                  @ 0x3361 
    /*00002bc0*/ BL          mn34220_write_register
    /*00002bc4*/ MOV         R0, R4 
    /*00002bc8*/ MOV         R2, #13 
    /*00002bcc*/ MOVW        R1, #13154                  @ 0x3362 
    /*00002bd0*/ BL          mn34220_write_register
    /*00002bd4*/ MOV         R0, R4 
    /*00002bd8*/ MOV         R2, #0 
    /*00002bdc*/ MOVW        R1, #13155                  @ 0x3363 
    /*00002be0*/ BL          mn34220_write_register
    /*00002be4*/ MOV         R0, R4 
    /*00002be8*/ MOV         R2, #0 
    /*00002bec*/ MOVW        R1, #13156                  @ 0x3364 
    /*00002bf0*/ BL          mn34220_write_register
    /*00002bf4*/ MOV         R0, R4 
    /*00002bf8*/ MOV         R2, #1 
    /*00002bfc*/ MOVW        R1, #13157                  @ 0x3365 
    /*00002c00*/ BL          mn34220_write_register
    /*00002c04*/ MOV         R0, R4 
    /*00002c08*/ MOV         R2, #13 
    /*00002c0c*/ MOVW        R1, #13158                  @ 0x3366 
    /*00002c10*/ BL          mn34220_write_register
    /*00002c14*/ MOV         R0, R4 
    /*00002c18*/ MOV         R2, #0 
    /*00002c1c*/ MOVW        R1, #13159                  @ 0x3367 
    /*00002c20*/ BL          mn34220_write_register
    /*00002c24*/ MOV         R0, R4 
    /*00002c28*/ MOV         R2, #13 
    /*00002c2c*/ MOVW        R1, #13160                  @ 0x3368 
    /*00002c30*/ BL          mn34220_write_register
    /*00002c34*/ MOV         R0, R4 
    /*00002c38*/ MOV         R2, #1 
    /*00002c3c*/ MOVW        R1, #13161                  @ 0x3369 
    /*00002c40*/ BL          mn34220_write_register
    /*00002c44*/ MOV         R0, R4 
    /*00002c48*/ MOV         R2, #0 
    /*00002c4c*/ MOVW        R1, #13162                  @ 0x336a 
    /*00002c50*/ BL          mn34220_write_register
    /*00002c54*/ MOV         R0, R4 
    /*00002c58*/ MOV         R2, #3 
    /*00002c5c*/ MOVW        R1, #13163                  @ 0x336b 
    /*00002c60*/ BL          mn34220_write_register
    /*00002c64*/ MOV         R0, R4 
    /*00002c68*/ MOV         R2, #3 
    /*00002c6c*/ MOVW        R1, #13164                  @ 0x336c 
    /*00002c70*/ BL          mn34220_write_register
    /*00002c74*/ MOV         R0, R4 
    /*00002c78*/ MOV         R2, #0 
    /*00002c7c*/ MOVW        R1, #13165                  @ 0x336d 
    /*00002c80*/ BL          mn34220_write_register
    /*00002c84*/ MOV         R0, R4 
    /*00002c88*/ MOV         R2, #2 
    /*00002c8c*/ MOVW        R1, #13166                  @ 0x336e 
    /*00002c90*/ BL          mn34220_write_register
    /*00002c94*/ MOV         R0, R4 
    /*00002c98*/ MOV         R2, #0 
    /*00002c9c*/ MOVW        R1, #13167                  @ 0x336f 
    /*00002ca0*/ BL          mn34220_write_register
    /*00002ca4*/ MOV         R0, R4 
    /*00002ca8*/ MOV         R2, #0 
    /*00002cac*/ MOVW        R1, #13168                  @ 0x3370 
    /*00002cb0*/ BL          mn34220_write_register
    /*00002cb4*/ MOV         R0, R4 
    /*00002cb8*/ MOV         R2, #0 
    /*00002cbc*/ MOVW        R1, #13169                  @ 0x3371 
    /*00002cc0*/ BL          mn34220_write_register
    /*00002cc4*/ MOV         R0, R4 
    /*00002cc8*/ MOV         R2, #0 
    /*00002ccc*/ MOVW        R1, #13170                  @ 0x3372 
    /*00002cd0*/ BL          mn34220_write_register
    /*00002cd4*/ MOV         R0, R4 
    /*00002cd8*/ MOV         R2, #0 
    /*00002cdc*/ MOVW        R1, #13171                  @ 0x3373 
    /*00002ce0*/ BL          mn34220_write_register
    /*00002ce4*/ MOV         R0, R4 
    /*00002ce8*/ MOV         R2, #2 
    /*00002cec*/ MOVW        R1, #13172                  @ 0x3374 
    /*00002cf0*/ BL          mn34220_write_register
    /*00002cf4*/ MOV         R0, R4 
    /*00002cf8*/ MOV         R2, #0 
    /*00002cfc*/ MOVW        R1, #13173                  @ 0x3375 
    /*00002d00*/ BL          mn34220_write_register
    /*00002d04*/ MOV         R0, R4 
    /*00002d08*/ MOV         R2, #6 
    /*00002d0c*/ MOVW        R1, #13174                  @ 0x3376 
    /*00002d10*/ BL          mn34220_write_register
    /*00002d14*/ MOV         R0, R4 
    /*00002d18*/ MOV         R2, #1 
    /*00002d1c*/ MOVW        R1, #13175                  @ 0x3377 
    /*00002d20*/ BL          mn34220_write_register
    /*00002d24*/ MOV         R0, R4 
    /*00002d28*/ MOV         R2, #6 
    /*00002d2c*/ MOVW        R1, #13176                  @ 0x3378 
    /*00002d30*/ BL          mn34220_write_register
    /*00002d34*/ MOV         R0, R4 
    /*00002d38*/ MOV         R2, #6 
    /*00002d3c*/ MOVW        R1, #13177                  @ 0x3379 
    /*00002d40*/ BL          mn34220_write_register
    /*00002d44*/ MOV         R0, R4 
    /*00002d48*/ MOV         R2, #9 
    /*00002d4c*/ MOVW        R1, #13178                  @ 0x337a 
    /*00002d50*/ BL          mn34220_write_register
    /*00002d54*/ MOV         R0, R4 
    /*00002d58*/ MOV         R2, #0 
    /*00002d5c*/ MOVW        R1, #13179                  @ 0x337b 
    /*00002d60*/ BL          mn34220_write_register
    /*00002d64*/ MOV         R0, R4 
    /*00002d68*/ MOV         R2, #13 
    /*00002d6c*/ MOVW        R1, #13180                  @ 0x337c 
    /*00002d70*/ BL          mn34220_write_register
    /*00002d74*/ MOV         R0, R4 
    /*00002d78*/ MOV         R2, #0 
    /*00002d7c*/ MOVW        R1, #13181                  @ 0x337d 
    /*00002d80*/ BL          mn34220_write_register
    /*00002d84*/ MOV         R0, R4 
    /*00002d88*/ MOV         R2, #0 
    /*00002d8c*/ MOVW        R1, #13182                  @ 0x337e 
    /*00002d90*/ BL          mn34220_write_register
    /*00002d94*/ MOV         R0, R4 
    /*00002d98*/ MOV         R2, #1 
    /*00002d9c*/ MOVW        R1, #13183                  @ 0x337f 
    /*00002da0*/ BL          mn34220_write_register
    /*00002da4*/ MOV         R0, R4 
    /*00002da8*/ MOV         R2, #0 
    /*00002dac*/ MOV         R1, #13184                  @ 0x3380 
    /*00002db0*/ BL          mn34220_write_register
    /*00002db4*/ MOV         R0, R4 
    /*00002db8*/ MOV         R2, #0 
    /*00002dbc*/ MOVW        R1, #13185                  @ 0x3381 
    /*00002dc0*/ BL          mn34220_write_register
    /*00002dc4*/ MOV         R0, R4 
    /*00002dc8*/ MOV         R2, #0 
    /*00002dcc*/ MOVW        R1, #13186                  @ 0x3382 
    /*00002dd0*/ BL          mn34220_write_register
    /*00002dd4*/ MOV         R0, R4 
    /*00002dd8*/ MOV         R2, #0 
    /*00002ddc*/ MOVW        R1, #13187                  @ 0x3383 
    /*00002de0*/ BL          mn34220_write_register
    /*00002de4*/ MOV         R0, R4 
    /*00002de8*/ MOV         R2, #13 
    /*00002dec*/ MOVW        R1, #13188                  @ 0x3384 
    /*00002df0*/ BL          mn34220_write_register
    /*00002df4*/ MOV         R0, R4 
    /*00002df8*/ MOV         R2, #0 
    /*00002dfc*/ MOVW        R1, #13189                  @ 0x3385 
    /*00002e00*/ BL          mn34220_write_register
    /*00002e04*/ MOV         R0, R4 
    /*00002e08*/ MOV         R2, #0 
    /*00002e0c*/ MOVW        R1, #13190                  @ 0x3386 
    /*00002e10*/ BL          mn34220_write_register
    /*00002e14*/ MOV         R0, R4 
    /*00002e18*/ MOV         R2, #1 
    /*00002e1c*/ MOVW        R1, #13191                  @ 0x3387 
    /*00002e20*/ BL          mn34220_write_register
    /*00002e24*/ MOV         R0, R4 
    /*00002e28*/ MOV         R2, #13 
    /*00002e2c*/ MOVW        R1, #13192                  @ 0x3388 
    /*00002e30*/ BL          mn34220_write_register
    /*00002e34*/ MOV         R0, R4 
    /*00002e38*/ MOV         R2, #0 
    /*00002e3c*/ MOVW        R1, #13193                  @ 0x3389 
    /*00002e40*/ BL          mn34220_write_register
    /*00002e44*/ MOV         R0, R4 
    /*00002e48*/ MOV         R2, #0 
    /*00002e4c*/ MOVW        R1, #13194                  @ 0x338a 
    /*00002e50*/ BL          mn34220_write_register
    /*00002e54*/ MOV         R0, R4 
    /*00002e58*/ MOV         R2, #1 
    /*00002e5c*/ MOVW        R1, #13195                  @ 0x338b 
    /*00002e60*/ BL          mn34220_write_register
    /*00002e64*/ MOV         R0, R4 
    /*00002e68*/ MOV         R2, #4 
    /*00002e6c*/ MOVW        R1, #13196                  @ 0x338c 
    /*00002e70*/ BL          mn34220_write_register
    /*00002e74*/ MOV         R0, R4 
    /*00002e78*/ MOV         R2, #8 
    /*00002e7c*/ MOVW        R1, #13197                  @ 0x338d 
    /*00002e80*/ BL          mn34220_write_register
    /*00002e84*/ MOV         R0, R4 
    /*00002e88*/ MOV         R2, #10 
    /*00002e8c*/ MOVW        R1, #13198                  @ 0x338e 
    /*00002e90*/ BL          mn34220_write_register
    /*00002e94*/ MOV         R0, R4 
    /*00002e98*/ MOV         R2, #9 
    /*00002e9c*/ MOVW        R1, #13199                  @ 0x338f 
    /*00002ea0*/ BL          mn34220_write_register
    /*00002ea4*/ MOV         R0, R4 
    /*00002ea8*/ MOV         R2, #2 
    /*00002eac*/ MOVW        R1, #13200                  @ 0x3390 
    /*00002eb0*/ BL          mn34220_write_register
    /*00002eb4*/ MOV         R0, R4 
    /*00002eb8*/ MOV         R2, #15 
    /*00002ebc*/ MOVW        R1, #13201                  @ 0x3391 
    /*00002ec0*/ BL          mn34220_write_register
    /*00002ec4*/ MOV         R0, R4 
    /*00002ec8*/ MOV         R2, #4 
    /*00002ecc*/ MOVW        R1, #13202                  @ 0x3392 
    /*00002ed0*/ BL          mn34220_write_register
    /*00002ed4*/ MOV         R0, R4 
    /*00002ed8*/ MOV         R2, #9 
    /*00002edc*/ MOVW        R1, #13203                  @ 0x3393 
    /*00002ee0*/ BL          mn34220_write_register
    /*00002ee4*/ MOV         R0, R4 
    /*00002ee8*/ MOV         R2, #1 
    /*00002eec*/ MOVW        R1, #13204                  @ 0x3394 
    /*00002ef0*/ BL          mn34220_write_register
    /*00002ef4*/ MOV         R0, R4 
    /*00002ef8*/ MOV         R2, #10 
    /*00002efc*/ MOVW        R1, #13205                  @ 0x3395 
    /*00002f00*/ BL          mn34220_write_register
    /*00002f04*/ MOV         R0, R4 
    /*00002f08*/ MOV         R2, #7 
    /*00002f0c*/ MOVW        R1, #13206                  @ 0x3396 
    /*00002f10*/ BL          mn34220_write_register
    /*00002f14*/ MOV         R0, R4 
    /*00002f18*/ MOV         R2, #24 
    /*00002f1c*/ MOVW        R1, #13207                  @ 0x3397 
    /*00002f20*/ BL          mn34220_write_register
    /*00002f24*/ MOV         R0, R4 
    /*00002f28*/ MOV         R2, #0 
    /*00002f2c*/ MOVW        R1, #13208                  @ 0x3398 
    /*00002f30*/ BL          mn34220_write_register
    /*00002f34*/ MOV         R0, R4 
    /*00002f38*/ MOV         R2, #1 
    /*00002f3c*/ MOVW        R1, #13209                  @ 0x3399 
    /*00002f40*/ BL          mn34220_write_register
    /*00002f44*/ MOV         R0, R4 
    /*00002f48*/ MOV         R2, #1 
    /*00002f4c*/ MOVW        R1, #13210                  @ 0x339a 
    /*00002f50*/ BL          mn34220_write_register
    /*00002f54*/ MOV         R0, R4 
    /*00002f58*/ MOV         R2, #8 
    /*00002f5c*/ MOVW        R1, #13211                  @ 0x339b 
    /*00002f60*/ BL          mn34220_write_register
    /*00002f64*/ MOV         R0, R4 
    /*00002f68*/ MOV         R2, #13 
    /*00002f6c*/ MOVW        R1, #13212                  @ 0x339c 
    /*00002f70*/ BL          mn34220_write_register
    /*00002f74*/ MOV         R0, R4 
    /*00002f78*/ MOV         R2, #1 
    /*00002f7c*/ MOVW        R1, #13213                  @ 0x339d 
    /*00002f80*/ BL          mn34220_write_register
    /*00002f84*/ MOV         R0, R4 
    /*00002f88*/ MOV         R2, #0 
    /*00002f8c*/ MOVW        R1, #13214                  @ 0x339e 
    /*00002f90*/ BL          mn34220_write_register
    /*00002f94*/ MOV         R0, R4 
    /*00002f98*/ MOV         R2, #3 
    /*00002f9c*/ MOVW        R1, #13215                  @ 0x339f 
    /*00002fa0*/ BL          mn34220_write_register
    /*00002fa4*/ MOV         R0, R4 
    /*00002fa8*/ MOV         R2, #0 
    /*00002fac*/ MOVW        R1, #13216                  @ 0x33a0 
    /*00002fb0*/ BL          mn34220_write_register
    /*00002fb4*/ MOV         R0, R4 
    /*00002fb8*/ MOV         R2, #1 
    /*00002fbc*/ MOVW        R1, #13217                  @ 0x33a1 
    /*00002fc0*/ BL          mn34220_write_register
    /*00002fc4*/ MOV         R0, R4 
    /*00002fc8*/ MOV         R2, #3 
    /*00002fcc*/ MOVW        R1, #13218                  @ 0x33a2 
    /*00002fd0*/ BL          mn34220_write_register
    /*00002fd4*/ MOV         R0, R4 
    /*00002fd8*/ MOV         R2, #3 
    /*00002fdc*/ MOVW        R1, #13219                  @ 0x33a3 
    /*00002fe0*/ BL          mn34220_write_register
    /*00002fe4*/ MOV         R0, R4 
    /*00002fe8*/ MOV         R2, #0 
    /*00002fec*/ MOVW        R1, #13220                  @ 0x33a4 
    /*00002ff0*/ BL          mn34220_write_register
    /*00002ff4*/ MOV         R0, R4 
    /*00002ff8*/ MOV         R2, #3 
    /*00002ffc*/ MOVW        R1, #13221                  @ 0x33a5 
    /*00003000*/ BL          mn34220_write_register
    /*00003004*/ MOV         R0, R4 
    /*00003008*/ MOV         R2, #6 
    /*0000300c*/ MOVW        R1, #13222                  @ 0x33a6 
    /*00003010*/ BL          mn34220_write_register
    /*00003014*/ MOV         R0, R4 
    /*00003018*/ MOV         R2, #0 
    /*0000301c*/ MOVW        R1, #13223                  @ 0x33a7 
    /*00003020*/ BL          mn34220_write_register
    /*00003024*/ MOV         R0, R4 
    /*00003028*/ MOV         R2, #0 
    /*0000302c*/ MOVW        R1, #13224                  @ 0x33a8 
    /*00003030*/ BL          mn34220_write_register
    /*00003034*/ MOV         R0, R4 
    /*00003038*/ MOV         R2, #0 
    /*0000303c*/ MOVW        R1, #13225                  @ 0x33a9 
    /*00003040*/ BL          mn34220_write_register
    /*00003044*/ MOV         R0, R4 
    /*00003048*/ MOV         R2, #0 
    /*0000304c*/ MOVW        R1, #13226                  @ 0x33aa 
    /*00003050*/ BL          mn34220_write_register
    /*00003054*/ MOV         R0, R4 
    /*00003058*/ MOV         R2, #0 
    /*0000305c*/ MOVW        R1, #13227                  @ 0x33ab 
    /*00003060*/ BL          mn34220_write_register
    /*00003064*/ MOV         R0, R4 
    /*00003068*/ MOV         R2, #3 
    /*0000306c*/ MOVW        R1, #13228                  @ 0x33ac 
    /*00003070*/ BL          mn34220_write_register
    /*00003074*/ MOV         R0, R4 
    /*00003078*/ MOV         R2, #20 
    /*0000307c*/ MOVW        R1, #13229                  @ 0x33ad 
    /*00003080*/ BL          mn34220_write_register
    /*00003084*/ MOV         R0, R4 
    /*00003088*/ MOV         R2, #63                     @ 0x3f 
    /*0000308c*/ MOVW        R1, #13230                  @ 0x33ae 
    /*00003090*/ BL          mn34220_write_register
    /*00003094*/ MOV         R0, R4 
    /*00003098*/ MOV         R2, #17 
    /*0000309c*/ MOVW        R1, #13231                  @ 0x33af 
    /*000030a0*/ BL          mn34220_write_register
    /*000030a4*/ MOV         R0, R4 
    /*000030a8*/ MOV         R2, #10 
    /*000030ac*/ MOVW        R1, #13232                  @ 0x33b0 
    /*000030b0*/ BL          mn34220_write_register
    /*000030b4*/ MOV         R0, R4 
    /*000030b8*/ MOV         R2, #26 
    /*000030bc*/ MOVW        R1, #13233                  @ 0x33b1 
    /*000030c0*/ BL          mn34220_write_register
    /*000030c4*/ MOV         R0, R4 
    /*000030c8*/ MOV         R2, #0 
    /*000030cc*/ MOVW        R1, #13234                  @ 0x33b2 
    /*000030d0*/ BL          mn34220_write_register
    /*000030d4*/ MOV         R0, R4 
    /*000030d8*/ MOV         R2, #0 
    /*000030dc*/ MOVW        R1, #13235                  @ 0x33b3 
    /*000030e0*/ BL          mn34220_write_register
    /*000030e4*/ MOV         R0, R4 
    /*000030e8*/ MOV         R2, #0 
    /*000030ec*/ MOVW        R1, #13236                  @ 0x33b4 
    /*000030f0*/ BL          mn34220_write_register
    /*000030f4*/ MOV         R0, R4 
    /*000030f8*/ MOV         R2, #0 
    /*000030fc*/ MOVW        R1, #13237                  @ 0x33b5 
    /*00003100*/ BL          mn34220_write_register
    /*00003104*/ MOV         R0, R4 
    /*00003108*/ MOV         R2, #0 
    /*0000310c*/ MOV         R1, #255                    @ 0xff 
    /*00003110*/ BL          mn34220_write_register
    /*00003114*/ MOV         R0, R4 
    /*00003118*/ MOV         R2, #0 
    /*0000311c*/ MOV         R1, #255                    @ 0xff 
    /*00003120*/ BL          mn34220_write_register
    /*00003124*/ MOV         R0, R4 
    /*00003128*/ MOV         R2, #0 
    /*0000312c*/ MOV         R1, #255                    @ 0xff 
    /*00003130*/ BL          mn34220_write_register
    /*00003134*/ MOV         R0, R4 
    /*00003138*/ MOV         R2, #0 
    /*0000313c*/ MOV         R1, #255                    @ 0xff 
    /*00003140*/ BL          mn34220_write_register
    /*00003144*/ MOV         R0, R4 
    /*00003148*/ MOV         R2, #0 
    /*0000314c*/ MOV         R1, #255                    @ 0xff 
    /*00003150*/ BL          mn34220_write_register
    /*00003154*/ MOV         R0, R4 
    /*00003158*/ MOV         R2, #0 
    /*0000315c*/ MOV         R1, #255                    @ 0xff 
    /*00003160*/ BL          mn34220_write_register
    /*00003164*/ MOV         R0, R4 
    /*00003168*/ MOV         R2, #0 
    /*0000316c*/ MOV         R1, #255                    @ 0xff 
    /*00003170*/ BL          mn34220_write_register
    /*00003174*/ MOV         R0, R4 
    /*00003178*/ MOV         R2, #0 
    /*0000317c*/ MOV         R1, #255                    @ 0xff 
    /*00003180*/ BL          mn34220_write_register
    /*00003184*/ MOV         R0, R4 
    /*00003188*/ MOV         R2, #0 
    /*0000318c*/ MOV         R1, #255                    @ 0xff 
    /*00003190*/ BL          mn34220_write_register
    /*00003194*/ MOV         R0, R4 
    /*00003198*/ MOV         R2, #0 
    /*0000319c*/ MOV         R1, #255                    @ 0xff 
    /*000031a0*/ BL          mn34220_write_register
    /*000031a4*/ MOV         R0, R4 
    /*000031a8*/ MOV         R2, #0 
    /*000031ac*/ MOV         R1, #255                    @ 0xff 
    /*000031b0*/ BL          mn34220_write_register
    /*000031b4*/ MOV         R0, R4 
    /*000031b8*/ MOV         R2, #0 
    /*000031bc*/ MOV         R1, #255                    @ 0xff 
    /*000031c0*/ BL          mn34220_write_register
    /*000031c4*/ MOV         R0, R4 
    /*000031c8*/ MOV         R2, #0 
    /*000031cc*/ MOV         R1, #255                    @ 0xff 
    /*000031d0*/ BL          mn34220_write_register
    /*000031d4*/ MOV         R0, R4 
    /*000031d8*/ MOV         R2, #0 
    /*000031dc*/ MOV         R1, #255                    @ 0xff 
    /*000031e0*/ BL          mn34220_write_register
    /*000031e4*/ MOV         R0, R4 
    /*000031e8*/ MOV         R2, #0 
    /*000031ec*/ MOV         R1, #255                    @ 0xff 
    /*000031f0*/ BL          mn34220_write_register
    /*000031f4*/ MOV         R0, R4 
    /*000031f8*/ MOV         R2, #0 
    /*000031fc*/ MOV         R1, #255                    @ 0xff 
    /*00003200*/ BL          mn34220_write_register
    /*00003204*/ MOV         R0, R4 
    /*00003208*/ MOV         R2, #0 
    /*0000320c*/ MOV         R1, #255                    @ 0xff 
    /*00003210*/ BL          mn34220_write_register
    /*00003214*/ MOV         R0, R4 
    /*00003218*/ MOV         R2, #0 
    /*0000321c*/ MOV         R1, #255                    @ 0xff 
    /*00003220*/ BL          mn34220_write_register
    /*00003224*/ MOV         R0, R4 
    /*00003228*/ MOV         R2, #0 
    /*0000322c*/ MOV         R1, #12288                  @ 0x3000 
    /*00003230*/ BL          mn34220_write_register
    /*00003234*/ MOV         R0, R4 
    /*00003238*/ MOV         R2, #211                    @ 0xd3 
    /*0000323c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00003240*/ BL          mn34220_write_register
    /*00003244*/ MOV         R0, R4 
    /*00003248*/ MOV         R2, #1 
    /*0000324c*/ MOV         R1, #256                    @ 0x100 
    /*00003250*/ BL          mn34220_write_register
    /*00003254*/ MOV         R0, R4 
    /*00003258*/ MOV         R2, #0 
    /*0000325c*/ MOVW        R1, #257                    @ 0x101 
    /*00003260*/ BL          mn34220_write_register
    /*00003264*/ LDR         R0, WORD_3274               @ LDR         R0, [PC, #8]                @ 0x0000000000003274 
    /*00003268*/ POP         {R4, LR} 
    /*0000326c*/ ADD         R0, PC, R0 
    mn34220_linear_720p30_init_x3270:
    /*00003270*/ B           mn34220_linear_720p30_init_x3270@ puts [R_ARM_JUMP24: +0x0] 
    /*00003274*/ WORD_3274: .word 0x00000000
FUNC_END mn34220_linear_720p30_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_linear_VGA240_init
@ Size: 0x7d8
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_linear_VGA240_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #1 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000010*/ BL          mn34220_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #0 
    /*0000001c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000020*/ BL          mn34220_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #2 
    /*0000002c*/ MOVW        R1, #773                    @ 0x305 
    /*00000030*/ BL          mn34220_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #48                     @ 0x30 
    /*0000003c*/ MOVW        R1, #775                    @ 0x307 
    /*00000040*/ BL          mn34220_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #0 
    /*0000004c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000050*/ BL          mn34220_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #3 
    /*0000005c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000060*/ BL          mn34220_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #12 
    /*0000006c*/ MOVW        R1, #274                    @ 0x112 
    /*00000070*/ BL          mn34220_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #12 
    /*0000007c*/ MOVW        R1, #275                    @ 0x113 
    /*00000080*/ BL          mn34220_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #4 
    /*0000008c*/ MOVW        R1, #12292                  @ 0x3004 
    /*00000090*/ BL          mn34220_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #103                    @ 0x67 
    /*0000009c*/ MOVW        R1, #12293                  @ 0x3005 
    /*000000a0*/ BL          mn34220_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #16 
    /*000000ac*/ MOVW        R1, #12295                  @ 0x3007 
    /*000000b0*/ BL          mn34220_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #0 
    /*000000bc*/ MOVW        R1, #12296                  @ 0x3008 
    /*000000c0*/ BL          mn34220_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #0 
    /*000000cc*/ MOVW        R1, #12299                  @ 0x300b 
    /*000000d0*/ BL          mn34220_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #67                     @ 0x43 
    /*000000dc*/ MOVW        R1, #12312                  @ 0x3018 
    /*000000e0*/ BL          mn34220_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #16 
    /*000000ec*/ MOVW        R1, #12313                  @ 0x3019 
    /*000000f0*/ BL          mn34220_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #185                    @ 0xb9 
    /*000000fc*/ MOVW        R1, #12314                  @ 0x301a 
    /*00000100*/ BL          mn34220_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #0 
    /*0000010c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000110*/ BL          mn34220_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #83                     @ 0x53 
    /*0000011c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000120*/ BL          mn34220_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #0 
    /*0000012c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000130*/ BL          mn34220_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #0 
    /*0000013c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000140*/ BL          mn34220_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #2 
    /*0000014c*/ MOVW        R1, #514                    @ 0x202 
    /*00000150*/ BL          mn34220_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #48                     @ 0x30 
    /*0000015c*/ MOVW        R1, #515                    @ 0x203 
    /*00000160*/ BL          mn34220_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #2 
    /*0000016c*/ MOV         R1, #832                    @ 0x340 
    /*00000170*/ BL          mn34220_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #50                     @ 0x32 
    /*0000017c*/ MOVW        R1, #833                    @ 0x341 
    /*00000180*/ BL          mn34220_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #8 
    /*0000018c*/ MOVW        R1, #834                    @ 0x342 
    /*00000190*/ BL          mn34220_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #152                    @ 0x98 
    /*0000019c*/ MOVW        R1, #835                    @ 0x343 
    /*000001a0*/ BL          mn34220_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #1 
    /*000001ac*/ MOVW        R1, #838                    @ 0x346 
    /*000001b0*/ BL          mn34220_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #104                    @ 0x68 
    /*000001bc*/ MOVW        R1, #839                    @ 0x347 
    /*000001c0*/ BL          mn34220_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #3 
    /*000001cc*/ MOVW        R1, #842                    @ 0x34a 
    /*000001d0*/ BL          mn34220_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #83                     @ 0x53 
    /*000001dc*/ MOVW        R1, #843                    @ 0x34b 
    /*000001e0*/ BL          mn34220_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #1 
    /*000001ec*/ MOVW        R1, #846                    @ 0x34e 
    /*000001f0*/ BL          mn34220_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #236                    @ 0xec 
    /*000001fc*/ MOVW        R1, #847                    @ 0x34f 
    /*00000200*/ BL          mn34220_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #0 
    /*0000020c*/ MOVW        R1, #12342                  @ 0x3036 
    /*00000210*/ BL          mn34220_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #46                     @ 0x2e 
    /*0000021c*/ MOVW        R1, #12345                  @ 0x3039 
    /*00000220*/ BL          mn34220_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #18 
    /*0000022c*/ MOVW        R1, #12353                  @ 0x3041 
    /*00000230*/ BL          mn34220_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #3 
    /*0000023c*/ MOVW        R1, #12376                  @ 0x3058 
    /*00000240*/ BL          mn34220_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #9 
    /*0000024c*/ MOVW        R1, #12398                  @ 0x306e 
    /*00000250*/ BL          mn34220_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #0 
    /*0000025c*/ MOVW        R1, #12399                  @ 0x306f 
    /*00000260*/ BL          mn34220_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #1 
    /*0000026c*/ MOVW        R1, #12404                  @ 0x3074 
    /*00000270*/ BL          mn34220_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #0 
    /*0000027c*/ MOVW        R1, #12440                  @ 0x3098 
    /*00000280*/ BL          mn34220_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #64                     @ 0x40 
    /*0000028c*/ MOVW        R1, #12441                  @ 0x3099 
    /*00000290*/ BL          mn34220_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #16 
    /*0000029c*/ MOVW        R1, #12442                  @ 0x309a 
    /*000002a0*/ BL          mn34220_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #0 
    /*000002ac*/ MOVW        R1, #12545                  @ 0x3101 
    /*000002b0*/ BL          mn34220_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #4 
    /*000002bc*/ MOVW        R1, #12548                  @ 0x3104 
    /*000002c0*/ BL          mn34220_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #0 
    /*000002cc*/ MOVW        R1, #12550                  @ 0x3106 
    /*000002d0*/ BL          mn34220_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #64                     @ 0x40 
    /*000002dc*/ MOVW        R1, #12551                  @ 0x3107 
    /*000002e0*/ BL          mn34220_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #0 
    /*000002ec*/ MOVW        R1, #12587                  @ 0x312b 
    /*000002f0*/ BL          mn34220_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #0 
    /*000002fc*/ MOVW        R1, #12589                  @ 0x312d 
    /*00000300*/ BL          mn34220_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #0 
    /*0000030c*/ MOVW        R1, #12591                  @ 0x312f 
    /*00000310*/ BL          mn34220_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #112                    @ 0x70 
    /*0000031c*/ MOVW        R1, #12609                  @ 0x3141 
    /*00000320*/ BL          mn34220_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #1 
    /*0000032c*/ MOVW        R1, #12611                  @ 0x3143 
    /*00000330*/ BL          mn34220_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #3 
    /*0000033c*/ MOVW        R1, #12612                  @ 0x3144 
    /*00000340*/ BL          mn34220_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #2 
    /*0000034c*/ MOVW        R1, #12613                  @ 0x3145 
    /*00000350*/ BL          mn34220_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #5 
    /*0000035c*/ MOVW        R1, #12614                  @ 0x3146 
    /*00000360*/ BL          mn34220_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #0 
    /*0000036c*/ MOVW        R1, #12615                  @ 0x3147 
    /*00000370*/ BL          mn34220_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #0 
    /*0000037c*/ MOVW        R1, #12616                  @ 0x3148 
    /*00000380*/ BL          mn34220_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #0 
    /*0000038c*/ MOVW        R1, #12617                  @ 0x3149 
    /*00000390*/ BL          mn34220_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #3 
    /*0000039c*/ MOVW        R1, #12618                  @ 0x314a 
    /*000003a0*/ BL          mn34220_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #1 
    /*000003ac*/ MOVW        R1, #12619                  @ 0x314b 
    /*000003b0*/ BL          mn34220_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #1 
    /*000003bc*/ MOVW        R1, #12620                  @ 0x314c 
    /*000003c0*/ BL          mn34220_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #1 
    /*000003cc*/ MOVW        R1, #12621                  @ 0x314d 
    /*000003d0*/ BL          mn34220_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #2 
    /*000003dc*/ MOVW        R1, #12622                  @ 0x314e 
    /*000003e0*/ BL          mn34220_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #2 
    /*000003ec*/ MOVW        R1, #12623                  @ 0x314f 
    /*000003f0*/ BL          mn34220_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #2 
    /*000003fc*/ MOVW        R1, #12624                  @ 0x3150 
    /*00000400*/ BL          mn34220_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #1 
    /*0000040c*/ MOVW        R1, #12626                  @ 0x3152 
    /*00000410*/ BL          mn34220_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #7 
    /*0000041c*/ MOVW        R1, #12627                  @ 0x3153 
    /*00000420*/ BL          mn34220_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #17 
    /*0000042c*/ MOVW        R1, #12629                  @ 0x3155 
    /*00000430*/ BL          mn34220_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #48                     @ 0x30 
    /*0000043c*/ MOVW        R1, #12631                  @ 0x3157 
    /*00000440*/ BL          mn34220_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #51                     @ 0x33 
    /*0000044c*/ MOVW        R1, #12633                  @ 0x3159 
    /*00000450*/ BL          mn34220_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #54                     @ 0x36 
    /*0000045c*/ MOVW        R1, #12635                  @ 0x315b 
    /*00000460*/ BL          mn34220_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #53                     @ 0x35 
    /*0000046c*/ MOVW        R1, #12637                  @ 0x315d 
    /*00000470*/ BL          mn34220_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #60                     @ 0x3c 
    /*0000047c*/ MOVW        R1, #12639                  @ 0x315f 
    /*00000480*/ BL          mn34220_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #63                     @ 0x3f 
    /*0000048c*/ MOVW        R1, #12641                  @ 0x3161 
    /*00000490*/ BL          mn34220_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #58                     @ 0x3a 
    /*0000049c*/ MOVW        R1, #12643                  @ 0x3163 
    /*000004a0*/ BL          mn34220_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #57                     @ 0x39 
    /*000004ac*/ MOVW        R1, #12645                  @ 0x3165 
    /*000004b0*/ BL          mn34220_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #40                     @ 0x28 
    /*000004bc*/ MOVW        R1, #12647                  @ 0x3167 
    /*000004c0*/ BL          mn34220_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #43                     @ 0x2b 
    /*000004cc*/ MOVW        R1, #12649                  @ 0x3169 
    /*000004d0*/ BL          mn34220_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #46                     @ 0x2e 
    /*000004dc*/ MOVW        R1, #12651                  @ 0x316b 
    /*000004e0*/ BL          mn34220_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #45                     @ 0x2d 
    /*000004ec*/ MOVW        R1, #12653                  @ 0x316d 
    /*000004f0*/ BL          mn34220_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #34                     @ 0x22 
    /*000004fc*/ MOVW        R1, #12655                  @ 0x316f 
    /*00000500*/ BL          mn34220_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #34                     @ 0x22 
    /*0000050c*/ MOVW        R1, #12657                  @ 0x3171 
    /*00000510*/ BL          mn34220_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #97                     @ 0x61 
    /*0000051c*/ MOVW        R1, #12659                  @ 0x3173 
    /*00000520*/ BL          mn34220_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #128                    @ 0x80 
    /*0000052c*/ MOVW        R1, #12661                  @ 0x3175 
    /*00000530*/ BL          mn34220_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #32 
    /*0000053c*/ MOVW        R1, #12686                  @ 0x318e 
    /*00000540*/ BL          mn34220_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #112                    @ 0x70 
    /*0000054c*/ MOVW        R1, #12687                  @ 0x318f 
    /*00000550*/ BL          mn34220_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #8 
    /*0000055c*/ MOVW        R1, #12694                  @ 0x3196 
    /*00000560*/ BL          mn34220_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #3 
    /*0000056c*/ MOVW        R1, #12796                  @ 0x31fc 
    /*00000570*/ BL          mn34220_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #6 
    /*0000057c*/ MOVW        R1, #12798                  @ 0x31fe 
    /*00000580*/ BL          mn34220_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #113                    @ 0x71 
    /*0000058c*/ MOVW        R1, #12860                  @ 0x323c 
    /*00000590*/ BL          mn34220_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #1 
    /*0000059c*/ MOVW        R1, #12862                  @ 0x323e 
    /*000005a0*/ BL          mn34220_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #117                    @ 0x75 
    /*000005ac*/ MOVW        R1, #12867                  @ 0x3243 
    /*000005b0*/ BL          mn34220_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #0 
    /*000005bc*/ MOVW        R1, #12870                  @ 0x3246 
    /*000005c0*/ BL          mn34220_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #201                    @ 0xc9 
    /*000005cc*/ MOVW        R1, #12871                  @ 0x3247 
    /*000005d0*/ BL          mn34220_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #0 
    /*000005dc*/ MOVW        R1, #12872                  @ 0x3248 
    /*000005e0*/ BL          mn34220_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #0 
    /*000005ec*/ MOVW        R1, #12873                  @ 0x3249 
    /*000005f0*/ BL          mn34220_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #48                     @ 0x30 
    /*000005fc*/ MOVW        R1, #12874                  @ 0x324a 
    /*00000600*/ BL          mn34220_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #27 
    /*0000060c*/ MOVW        R1, #12875                  @ 0x324b 
    /*00000610*/ BL          mn34220_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #2 
    /*0000061c*/ MOVW        R1, #12876                  @ 0x324c 
    /*00000620*/ BL          mn34220_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #123                    @ 0x7b 
    /*0000062c*/ MOVW        R1, #12883                  @ 0x3253 
    /*00000630*/ BL          mn34220_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #50                     @ 0x32 
    /*0000063c*/ MOVW        R1, #12886                  @ 0x3256 
    /*00000640*/ BL          mn34220_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #0 
    /*0000064c*/ MOVW        R1, #12888                  @ 0x3258 
    /*00000650*/ BL          mn34220_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #92                     @ 0x5c 
    /*0000065c*/ MOVW        R1, #12889                  @ 0x3259 
    /*00000660*/ BL          mn34220_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #20 
    /*0000066c*/ MOVW        R1, #12890                  @ 0x325a 
    /*00000670*/ BL          mn34220_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #12 
    /*0000067c*/ MOVW        R1, #12914                  @ 0x3272 
    /*00000680*/ BL          mn34220_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #48                     @ 0x30 
    /*0000068c*/ MOV         R1, #12928                  @ 0x3280 
    /*00000690*/ BL          mn34220_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #7 
    /*0000069c*/ MOVW        R1, #12930                  @ 0x3282 
    /*000006a0*/ BL          mn34220_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #25 
    /*000006ac*/ MOVW        R1, #12933                  @ 0x3285 
    /*000006b0*/ BL          mn34220_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #0 
    /*000006bc*/ MOVW        R1, #12936                  @ 0x3288 
    /*000006c0*/ BL          mn34220_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #64                     @ 0x40 
    /*000006cc*/ MOVW        R1, #12937                  @ 0x3289 
    /*000006d0*/ BL          mn34220_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #5 
    /*000006dc*/ MOVW        R1, #13070                  @ 0x330e 
    /*000006e0*/ BL          mn34220_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #2 
    /*000006ec*/ MOVW        R1, #13072                  @ 0x3310 
    /*000006f0*/ BL          mn34220_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #31 
    /*000006fc*/ MOVW        R1, #13077                  @ 0x3315 
    /*00000700*/ BL          mn34220_write_register
    /*00000704*/ MOV         R0, R4 
    /*00000708*/ MOV         R2, #3 
    /*0000070c*/ MOVW        R1, #13082                  @ 0x331a 
    /*00000710*/ BL          mn34220_write_register
    /*00000714*/ MOV         R0, R4 
    /*00000718*/ MOV         R2, #3 
    /*0000071c*/ MOVW        R1, #13083                  @ 0x331b 
    /*00000720*/ BL          mn34220_write_register
    /*00000724*/ MOV         R0, R4 
    /*00000728*/ MOV         R2, #0 
    /*0000072c*/ MOVW        R1, #13100                  @ 0x332c 
    /*00000730*/ BL          mn34220_write_register
    /*00000734*/ MOV         R0, R4 
    /*00000738*/ MOV         R2, #3 
    /*0000073c*/ MOVW        R1, #13113                  @ 0x3339 
    /*00000740*/ BL          mn34220_write_register
    /*00000744*/ MOV         R0, R4 
    /*00000748*/ MOV         R2, #2 
    /*0000074c*/ MOVW        R1, #13163                  @ 0x336b 
    /*00000750*/ BL          mn34220_write_register
    /*00000754*/ MOV         R0, R4 
    /*00000758*/ MOV         R2, #1 
    /*0000075c*/ MOVW        R1, #13215                  @ 0x339f 
    /*00000760*/ BL          mn34220_write_register
    /*00000764*/ MOV         R0, R4 
    /*00000768*/ MOV         R2, #1 
    /*0000076c*/ MOVW        R1, #13218                  @ 0x33a2 
    /*00000770*/ BL          mn34220_write_register
    /*00000774*/ MOV         R0, R4 
    /*00000778*/ MOV         R2, #1 
    /*0000077c*/ MOVW        R1, #13219                  @ 0x33a3 
    /*00000780*/ BL          mn34220_write_register
    /*00000784*/ MOV         R0, R4 
    /*00000788*/ MOV         R2, #0 
    /*0000078c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000790*/ BL          mn34220_write_register
    /*00000794*/ MOV         R0, R4 
    /*00000798*/ MOV         R2, #211                    @ 0xd3 
    /*0000079c*/ MOVW        R1, #12289                  @ 0x3001 
    /*000007a0*/ BL          mn34220_write_register
    /*000007a4*/ MOV         R0, R4 
    /*000007a8*/ MOV         R2, #1 
    /*000007ac*/ MOV         R1, #256                    @ 0x100 
    /*000007b0*/ BL          mn34220_write_register
    /*000007b4*/ MOV         R0, R4 
    /*000007b8*/ MOV         R2, #0 
    /*000007bc*/ MOVW        R1, #257                    @ 0x101 
    /*000007c0*/ BL          mn34220_write_register
    /*000007c4*/ LDR         R0, WORD_07d4               @ LDR         R0, [PC, #8]                @ 0x00000000000007d4 
    /*000007c8*/ POP         {R4, LR} 
    /*000007cc*/ ADD         R0, PC, R0 
    mn34220_linear_VGA240_init_x7d0:
    /*000007d0*/ B           mn34220_linear_VGA240_init_x7d0@ puts [R_ARM_JUMP24: +0x0] 
    /*000007d4*/ WORD_07d4: .word 0x00000000
FUNC_END mn34220_linear_VGA240_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_wdr_init
@ Size: 0x7d8
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_wdr_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #1 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000010*/ BL          mn34220_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #0 
    /*0000001c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000020*/ BL          mn34220_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #2 
    /*0000002c*/ MOVW        R1, #773                    @ 0x305 
    /*00000030*/ BL          mn34220_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #48                     @ 0x30 
    /*0000003c*/ MOVW        R1, #775                    @ 0x307 
    /*00000040*/ BL          mn34220_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #0 
    /*0000004c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000050*/ BL          mn34220_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #3 
    /*0000005c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000060*/ BL          mn34220_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #12 
    /*0000006c*/ MOVW        R1, #274                    @ 0x112 
    /*00000070*/ BL          mn34220_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #12 
    /*0000007c*/ MOVW        R1, #275                    @ 0x113 
    /*00000080*/ BL          mn34220_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #3 
    /*0000008c*/ MOVW        R1, #12292                  @ 0x3004 
    /*00000090*/ BL          mn34220_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #103                    @ 0x67 
    /*0000009c*/ MOVW        R1, #12293                  @ 0x3005 
    /*000000a0*/ BL          mn34220_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #16 
    /*000000ac*/ MOVW        R1, #12295                  @ 0x3007 
    /*000000b0*/ BL          mn34220_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #144                    @ 0x90 
    /*000000bc*/ MOVW        R1, #12296                  @ 0x3008 
    /*000000c0*/ BL          mn34220_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #0 
    /*000000cc*/ MOVW        R1, #12299                  @ 0x300b 
    /*000000d0*/ BL          mn34220_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #67                     @ 0x43 
    /*000000dc*/ MOVW        R1, #12312                  @ 0x3018 
    /*000000e0*/ BL          mn34220_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #16 
    /*000000ec*/ MOVW        R1, #12313                  @ 0x3019 
    /*000000f0*/ BL          mn34220_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #185                    @ 0xb9 
    /*000000fc*/ MOVW        R1, #12314                  @ 0x301a 
    /*00000100*/ BL          mn34220_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #0 
    /*0000010c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000110*/ BL          mn34220_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #83                     @ 0x53 
    /*0000011c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000120*/ BL          mn34220_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #0 
    /*0000012c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000130*/ BL          mn34220_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #0 
    /*0000013c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000140*/ BL          mn34220_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #0 
    /*0000014c*/ MOVW        R1, #514                    @ 0x202 
    /*00000150*/ BL          mn34220_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #32 
    /*0000015c*/ MOVW        R1, #515                    @ 0x203 
    /*00000160*/ BL          mn34220_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #4 
    /*0000016c*/ MOV         R1, #832                    @ 0x340 
    /*00000170*/ BL          mn34220_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #226                    @ 0xe2 
    /*0000017c*/ MOVW        R1, #833                    @ 0x341 
    /*00000180*/ BL          mn34220_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #20 
    /*0000018c*/ MOVW        R1, #834                    @ 0x342 
    /*00000190*/ BL          mn34220_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #160                    @ 0xa0 
    /*0000019c*/ MOVW        R1, #835                    @ 0x343 
    /*000001a0*/ BL          mn34220_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #0 
    /*000001ac*/ MOVW        R1, #838                    @ 0x346 
    /*000001b0*/ BL          mn34220_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #60                     @ 0x3c 
    /*000001bc*/ MOVW        R1, #839                    @ 0x347 
    /*000001c0*/ BL          mn34220_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #4 
    /*000001cc*/ MOVW        R1, #842                    @ 0x34a 
    /*000001d0*/ BL          mn34220_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #127                    @ 0x7f 
    /*000001dc*/ MOVW        R1, #843                    @ 0x34b 
    /*000001e0*/ BL          mn34220_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #4 
    /*000001ec*/ MOVW        R1, #846                    @ 0x34e 
    /*000001f0*/ BL          mn34220_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #68                     @ 0x44 
    /*000001fc*/ MOVW        R1, #847                    @ 0x34f 
    /*00000200*/ BL          mn34220_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #0 
    /*0000020c*/ MOVW        R1, #12342                  @ 0x3036 
    /*00000210*/ BL          mn34220_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #46                     @ 0x2e 
    /*0000021c*/ MOVW        R1, #12345                  @ 0x3039 
    /*00000220*/ BL          mn34220_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #44                     @ 0x2c 
    /*0000022c*/ MOVW        R1, #12353                  @ 0x3041 
    /*00000230*/ BL          mn34220_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #15 
    /*0000023c*/ MOVW        R1, #12376                  @ 0x3058 
    /*00000240*/ BL          mn34220_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #12 
    /*0000024c*/ MOVW        R1, #12398                  @ 0x306e 
    /*00000250*/ BL          mn34220_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #0 
    /*0000025c*/ MOVW        R1, #12399                  @ 0x306f 
    /*00000260*/ BL          mn34220_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #1 
    /*0000026c*/ MOVW        R1, #12404                  @ 0x3074 
    /*00000270*/ BL          mn34220_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #0 
    /*0000027c*/ MOVW        R1, #12440                  @ 0x3098 
    /*00000280*/ BL          mn34220_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #0 
    /*0000028c*/ MOVW        R1, #12441                  @ 0x3099 
    /*00000290*/ BL          mn34220_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #1 
    /*0000029c*/ MOVW        R1, #12442                  @ 0x309a 
    /*000002a0*/ BL          mn34220_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #1 
    /*000002ac*/ MOVW        R1, #12545                  @ 0x3101 
    /*000002b0*/ BL          mn34220_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #4 
    /*000002bc*/ MOVW        R1, #12548                  @ 0x3104 
    /*000002c0*/ BL          mn34220_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #0 
    /*000002cc*/ MOVW        R1, #12550                  @ 0x3106 
    /*000002d0*/ BL          mn34220_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #192                    @ 0xc0 
    /*000002dc*/ MOVW        R1, #12551                  @ 0x3107 
    /*000002e0*/ BL          mn34220_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #32 
    /*000002ec*/ MOVW        R1, #12587                  @ 0x312b 
    /*000002f0*/ BL          mn34220_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #32 
    /*000002fc*/ MOVW        R1, #12589                  @ 0x312d 
    /*00000300*/ BL          mn34220_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #32 
    /*0000030c*/ MOVW        R1, #12591                  @ 0x312f 
    /*00000310*/ BL          mn34220_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #64                     @ 0x40 
    /*0000031c*/ MOVW        R1, #12609                  @ 0x3141 
    /*00000320*/ BL          mn34220_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #3 
    /*0000032c*/ MOVW        R1, #12611                  @ 0x3143 
    /*00000330*/ BL          mn34220_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #4 
    /*0000033c*/ MOVW        R1, #12612                  @ 0x3144 
    /*00000340*/ BL          mn34220_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #3 
    /*0000034c*/ MOVW        R1, #12613                  @ 0x3145 
    /*00000350*/ BL          mn34220_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #5 
    /*0000035c*/ MOVW        R1, #12614                  @ 0x3146 
    /*00000360*/ BL          mn34220_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #5 
    /*0000036c*/ MOVW        R1, #12615                  @ 0x3147 
    /*00000370*/ BL          mn34220_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #2 
    /*0000037c*/ MOVW        R1, #12616                  @ 0x3148 
    /*00000380*/ BL          mn34220_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #2 
    /*0000038c*/ MOVW        R1, #12617                  @ 0x3149 
    /*00000390*/ BL          mn34220_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #5 
    /*0000039c*/ MOVW        R1, #12618                  @ 0x314a 
    /*000003a0*/ BL          mn34220_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #3 
    /*000003ac*/ MOVW        R1, #12619                  @ 0x314b 
    /*000003b0*/ BL          mn34220_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #6 
    /*000003bc*/ MOVW        R1, #12620                  @ 0x314c 
    /*000003c0*/ BL          mn34220_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #7 
    /*000003cc*/ MOVW        R1, #12621                  @ 0x314d 
    /*000003d0*/ BL          mn34220_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #6 
    /*000003dc*/ MOVW        R1, #12622                  @ 0x314e 
    /*000003e0*/ BL          mn34220_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #6 
    /*000003ec*/ MOVW        R1, #12623                  @ 0x314f 
    /*000003f0*/ BL          mn34220_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #7 
    /*000003fc*/ MOVW        R1, #12624                  @ 0x3150 
    /*00000400*/ BL          mn34220_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #6 
    /*0000040c*/ MOVW        R1, #12626                  @ 0x3152 
    /*00000410*/ BL          mn34220_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #227                    @ 0xe3 
    /*0000041c*/ MOVW        R1, #12627                  @ 0x3153 
    /*00000420*/ BL          mn34220_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #202                    @ 0xca 
    /*0000042c*/ MOVW        R1, #12629                  @ 0x3155 
    /*00000430*/ BL          mn34220_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #202                    @ 0xca 
    /*0000043c*/ MOVW        R1, #12631                  @ 0x3157 
    /*00000440*/ BL          mn34220_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #202                    @ 0xca 
    /*0000044c*/ MOVW        R1, #12633                  @ 0x3159 
    /*00000450*/ BL          mn34220_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #202                    @ 0xca 
    /*0000045c*/ MOVW        R1, #12635                  @ 0x315b 
    /*00000460*/ BL          mn34220_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #202                    @ 0xca 
    /*0000046c*/ MOVW        R1, #12637                  @ 0x315d 
    /*00000470*/ BL          mn34220_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #202                    @ 0xca 
    /*0000047c*/ MOVW        R1, #12639                  @ 0x315f 
    /*00000480*/ BL          mn34220_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #202                    @ 0xca 
    /*0000048c*/ MOVW        R1, #12641                  @ 0x3161 
    /*00000490*/ BL          mn34220_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #202                    @ 0xca 
    /*0000049c*/ MOVW        R1, #12643                  @ 0x3163 
    /*000004a0*/ BL          mn34220_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #202                    @ 0xca 
    /*000004ac*/ MOVW        R1, #12645                  @ 0x3165 
    /*000004b0*/ BL          mn34220_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #202                    @ 0xca 
    /*000004bc*/ MOVW        R1, #12647                  @ 0x3167 
    /*000004c0*/ BL          mn34220_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #202                    @ 0xca 
    /*000004cc*/ MOVW        R1, #12649                  @ 0x3169 
    /*000004d0*/ BL          mn34220_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #202                    @ 0xca 
    /*000004dc*/ MOVW        R1, #12651                  @ 0x316b 
    /*000004e0*/ BL          mn34220_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #202                    @ 0xca 
    /*000004ec*/ MOVW        R1, #12653                  @ 0x316d 
    /*000004f0*/ BL          mn34220_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #198                    @ 0xc6 
    /*000004fc*/ MOVW        R1, #12655                  @ 0x316f 
    /*00000500*/ BL          mn34220_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #202                    @ 0xca 
    /*0000050c*/ MOVW        R1, #12657                  @ 0x3171 
    /*00000510*/ BL          mn34220_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #202                    @ 0xca 
    /*0000051c*/ MOVW        R1, #12659                  @ 0x3173 
    /*00000520*/ BL          mn34220_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #128                    @ 0x80 
    /*0000052c*/ MOVW        R1, #12661                  @ 0x3175 
    /*00000530*/ BL          mn34220_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #32 
    /*0000053c*/ MOVW        R1, #12686                  @ 0x318e 
    /*00000540*/ BL          mn34220_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #112                    @ 0x70 
    /*0000054c*/ MOVW        R1, #12687                  @ 0x318f 
    /*00000550*/ BL          mn34220_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #8 
    /*0000055c*/ MOVW        R1, #12694                  @ 0x3196 
    /*00000560*/ BL          mn34220_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #2 
    /*0000056c*/ MOVW        R1, #12796                  @ 0x31fc 
    /*00000570*/ BL          mn34220_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #7 
    /*0000057c*/ MOVW        R1, #12798                  @ 0x31fe 
    /*00000580*/ BL          mn34220_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #112                    @ 0x70 
    /*0000058c*/ MOVW        R1, #12860                  @ 0x323c 
    /*00000590*/ BL          mn34220_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #0 
    /*0000059c*/ MOVW        R1, #12862                  @ 0x323e 
    /*000005a0*/ BL          mn34220_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #209                    @ 0xd1 
    /*000005ac*/ MOVW        R1, #12867                  @ 0x3243 
    /*000005b0*/ BL          mn34220_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #1 
    /*000005bc*/ MOVW        R1, #12870                  @ 0x3246 
    /*000005c0*/ BL          mn34220_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #214                    @ 0xd6 
    /*000005cc*/ MOVW        R1, #12871                  @ 0x3247 
    /*000005d0*/ BL          mn34220_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #0 
    /*000005dc*/ MOVW        R1, #12872                  @ 0x3248 
    /*000005e0*/ BL          mn34220_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #0 
    /*000005ec*/ MOVW        R1, #12873                  @ 0x3249 
    /*000005f0*/ BL          mn34220_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #48                     @ 0x30 
    /*000005fc*/ MOVW        R1, #12874                  @ 0x324a 
    /*00000600*/ BL          mn34220_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #24 
    /*0000060c*/ MOVW        R1, #12875                  @ 0x324b 
    /*00000610*/ BL          mn34220_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #2 
    /*0000061c*/ MOVW        R1, #12876                  @ 0x324c 
    /*00000620*/ BL          mn34220_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #212                    @ 0xd4 
    /*0000062c*/ MOVW        R1, #12883                  @ 0x3253 
    /*00000630*/ BL          mn34220_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #17 
    /*0000063c*/ MOVW        R1, #12886                  @ 0x3256 
    /*00000640*/ BL          mn34220_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #1 
    /*0000064c*/ MOVW        R1, #12888                  @ 0x3258 
    /*00000650*/ BL          mn34220_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #230                    @ 0xe6 
    /*0000065c*/ MOVW        R1, #12889                  @ 0x3259 
    /*00000660*/ BL          mn34220_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #57                     @ 0x39 
    /*0000066c*/ MOVW        R1, #12890                  @ 0x325a 
    /*00000670*/ BL          mn34220_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #85                     @ 0x55 
    /*0000067c*/ MOVW        R1, #12914                  @ 0x3272 
    /*00000680*/ BL          mn34220_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #48                     @ 0x30 
    /*0000068c*/ MOV         R1, #12928                  @ 0x3280 
    /*00000690*/ BL          mn34220_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #14 
    /*0000069c*/ MOVW        R1, #12930                  @ 0x3282 
    /*000006a0*/ BL          mn34220_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #27 
    /*000006ac*/ MOVW        R1, #12933                  @ 0x3285 
    /*000006b0*/ BL          mn34220_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #1 
    /*000006bc*/ MOVW        R1, #12936                  @ 0x3288 
    /*000006c0*/ BL          mn34220_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #0 
    /*000006cc*/ MOVW        R1, #12937                  @ 0x3289 
    /*000006d0*/ BL          mn34220_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #5 
    /*000006dc*/ MOVW        R1, #13070                  @ 0x330e 
    /*000006e0*/ BL          mn34220_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #2 
    /*000006ec*/ MOVW        R1, #13072                  @ 0x3310 
    /*000006f0*/ BL          mn34220_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #31 
    /*000006fc*/ MOVW        R1, #13077                  @ 0x3315 
    /*00000700*/ BL          mn34220_write_register
    /*00000704*/ MOV         R0, R4 
    /*00000708*/ MOV         R2, #2 
    /*0000070c*/ MOVW        R1, #13082                  @ 0x331a 
    /*00000710*/ BL          mn34220_write_register
    /*00000714*/ MOV         R0, R4 
    /*00000718*/ MOV         R2, #2 
    /*0000071c*/ MOVW        R1, #13083                  @ 0x331b 
    /*00000720*/ BL          mn34220_write_register
    /*00000724*/ MOV         R0, R4 
    /*00000728*/ MOV         R2, #2 
    /*0000072c*/ MOVW        R1, #13100                  @ 0x332c 
    /*00000730*/ BL          mn34220_write_register
    /*00000734*/ MOV         R0, R4 
    /*00000738*/ MOV         R2, #2 
    /*0000073c*/ MOVW        R1, #13113                  @ 0x3339 
    /*00000740*/ BL          mn34220_write_register
    /*00000744*/ MOV         R0, R4 
    /*00000748*/ MOV         R2, #3 
    /*0000074c*/ MOVW        R1, #13163                  @ 0x336b 
    /*00000750*/ BL          mn34220_write_register
    /*00000754*/ MOV         R0, R4 
    /*00000758*/ MOV         R2, #3 
    /*0000075c*/ MOVW        R1, #13215                  @ 0x339f 
    /*00000760*/ BL          mn34220_write_register
    /*00000764*/ MOV         R0, R4 
    /*00000768*/ MOV         R2, #3 
    /*0000076c*/ MOVW        R1, #13218                  @ 0x33a2 
    /*00000770*/ BL          mn34220_write_register
    /*00000774*/ MOV         R0, R4 
    /*00000778*/ MOV         R2, #3 
    /*0000077c*/ MOVW        R1, #13219                  @ 0x33a3 
    /*00000780*/ BL          mn34220_write_register
    /*00000784*/ MOV         R0, R4 
    /*00000788*/ MOV         R2, #0 
    /*0000078c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000790*/ BL          mn34220_write_register
    /*00000794*/ MOV         R0, R4 
    /*00000798*/ MOV         R2, #211                    @ 0xd3 
    /*0000079c*/ MOVW        R1, #12289                  @ 0x3001 
    /*000007a0*/ BL          mn34220_write_register
    /*000007a4*/ MOV         R0, R4 
    /*000007a8*/ MOV         R2, #1 
    /*000007ac*/ MOV         R1, #256                    @ 0x100 
    /*000007b0*/ BL          mn34220_write_register
    /*000007b4*/ MOV         R0, R4 
    /*000007b8*/ MOV         R2, #0 
    /*000007bc*/ MOVW        R1, #257                    @ 0x101 
    /*000007c0*/ BL          mn34220_write_register
    /*000007c4*/ LDR         R0, WORD_07d4               @ LDR         R0, [PC, #8]                @ 0x00000000000007d4 
    /*000007c8*/ POP         {R4, LR} 
    /*000007cc*/ ADD         R0, PC, R0 
    mn34220_wdr_init_x7d0:
    /*000007d0*/ B           mn34220_wdr_init_x7d0       @ puts [R_ARM_JUMP24: +0x0] 
    /*000007d4*/ WORD_07d4: .word 0x00000000
FUNC_END mn34220_wdr_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_wdr_720p_2to1_init
@ Size: 0x30a8
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_wdr_720p_2to1_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #0 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOVW        R1, #12322                  @ 0x3022 
    /*00000010*/ BL          mn34220_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #51                     @ 0x33 
    /*0000001c*/ MOVW        R1, #12323                  @ 0x3023 
    /*00000020*/ BL          mn34220_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #1 
    /*0000002c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000030*/ BL          mn34220_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #0 
    /*0000003c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000040*/ BL          mn34220_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #6 
    /*0000004c*/ MOVW        R1, #12290                  @ 0x3002 
    /*00000050*/ BL          mn34220_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #24 
    /*0000005c*/ MOVW        R1, #12291                  @ 0x3003 
    /*00000060*/ BL          mn34220_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #0 
    /*0000006c*/ MOV         R1, #772                    @ 0x304 
    /*00000070*/ BL          mn34220_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #2 
    /*0000007c*/ MOVW        R1, #773                    @ 0x305 
    /*00000080*/ BL          mn34220_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #0 
    /*0000008c*/ MOVW        R1, #774                    @ 0x306 
    /*00000090*/ BL          mn34220_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #48                     @ 0x30 
    /*0000009c*/ MOVW        R1, #775                    @ 0x307 
    /*000000a0*/ BL          mn34220_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #0 
    /*000000ac*/ MOV         R1, #12288                  @ 0x3000 
    /*000000b0*/ BL          mn34220_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #3 
    /*000000bc*/ MOVW        R1, #12289                  @ 0x3001 
    /*000000c0*/ BL          mn34220_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #0 
    /*000000cc*/ MOVW        R1, #258                    @ 0x102 
    /*000000d0*/ BL          mn34220_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #0 
    /*000000dc*/ MOVW        R1, #259                    @ 0x103 
    /*000000e0*/ BL          mn34220_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #0 
    /*000000ec*/ MOV         R1, #260                    @ 0x104 
    /*000000f0*/ BL          mn34220_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #0 
    /*000000fc*/ MOVW        R1, #261                    @ 0x105 
    /*00000100*/ BL          mn34220_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #0 
    /*0000010c*/ MOV         R1, #272                    @ 0x110 
    /*00000110*/ BL          mn34220_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #0 
    /*0000011c*/ MOVW        R1, #273                    @ 0x111 
    /*00000120*/ BL          mn34220_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #12 
    /*0000012c*/ MOVW        R1, #274                    @ 0x112 
    /*00000130*/ BL          mn34220_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #12 
    /*0000013c*/ MOVW        R1, #275                    @ 0x113 
    /*00000140*/ BL          mn34220_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #0 
    /*0000014c*/ MOV         R1, #288                    @ 0x120 
    /*00000150*/ BL          mn34220_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #0 
    /*0000015c*/ MOVW        R1, #289                    @ 0x121 
    /*00000160*/ BL          mn34220_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #0 
    /*0000016c*/ MOV         R1, #768                    @ 0x300 
    /*00000170*/ BL          mn34220_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #16 
    /*0000017c*/ MOVW        R1, #769                    @ 0x301 
    /*00000180*/ BL          mn34220_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #0 
    /*0000018c*/ MOVW        R1, #770                    @ 0x302 
    /*00000190*/ BL          mn34220_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #1 
    /*0000019c*/ MOVW        R1, #771                    @ 0x303 
    /*000001a0*/ BL          mn34220_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #3 
    /*000001ac*/ MOVW        R1, #12292                  @ 0x3004 
    /*000001b0*/ BL          mn34220_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #103                    @ 0x67 
    /*000001bc*/ MOVW        R1, #12293                  @ 0x3005 
    /*000001c0*/ BL          mn34220_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #0 
    /*000001cc*/ MOVW        R1, #12294                  @ 0x3006 
    /*000001d0*/ BL          mn34220_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #20 
    /*000001dc*/ MOVW        R1, #12295                  @ 0x3007 
    /*000001e0*/ BL          mn34220_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #144                    @ 0x90 
    /*000001ec*/ MOVW        R1, #12296                  @ 0x3008 
    /*000001f0*/ BL          mn34220_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #6 
    /*000001fc*/ MOVW        R1, #12297                  @ 0x3009 
    /*00000200*/ BL          mn34220_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #255                    @ 0xff 
    /*0000020c*/ MOVW        R1, #12298                  @ 0x300a 
    /*00000210*/ BL          mn34220_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #0 
    /*0000021c*/ MOVW        R1, #12299                  @ 0x300b 
    /*00000220*/ BL          mn34220_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #0 
    /*0000022c*/ MOVW        R1, #12300                  @ 0x300c 
    /*00000230*/ BL          mn34220_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #1 
    /*0000023c*/ MOVW        R1, #12301                  @ 0x300d 
    /*00000240*/ BL          mn34220_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #0 
    /*0000024c*/ MOVW        R1, #12304                  @ 0x3010 
    /*00000250*/ BL          mn34220_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #0 
    /*0000025c*/ MOVW        R1, #12305                  @ 0x3011 
    /*00000260*/ BL          mn34220_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #0 
    /*0000026c*/ MOVW        R1, #12306                  @ 0x3012 
    /*00000270*/ BL          mn34220_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #0 
    /*0000027c*/ MOVW        R1, #12307                  @ 0x3013 
    /*00000280*/ BL          mn34220_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #0 
    /*0000028c*/ MOVW        R1, #12308                  @ 0x3014 
    /*00000290*/ BL          mn34220_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #1 
    /*0000029c*/ MOVW        R1, #12309                  @ 0x3015 
    /*000002a0*/ BL          mn34220_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #0 
    /*000002ac*/ MOVW        R1, #12310                  @ 0x3016 
    /*000002b0*/ BL          mn34220_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #0 
    /*000002bc*/ MOVW        R1, #12311                  @ 0x3017 
    /*000002c0*/ BL          mn34220_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #67                     @ 0x43 
    /*000002cc*/ MOVW        R1, #12312                  @ 0x3018 
    /*000002d0*/ BL          mn34220_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #16 
    /*000002dc*/ MOVW        R1, #12313                  @ 0x3019 
    /*000002e0*/ BL          mn34220_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #185                    @ 0xb9 
    /*000002ec*/ MOVW        R1, #12314                  @ 0x301a 
    /*000002f0*/ BL          mn34220_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #0 
    /*000002fc*/ MOVW        R1, #12315                  @ 0x301b 
    /*00000300*/ BL          mn34220_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #43                     @ 0x2b 
    /*0000030c*/ MOVW        R1, #12316                  @ 0x301c 
    /*00000310*/ BL          mn34220_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #27 
    /*0000031c*/ MOVW        R1, #12317                  @ 0x301d 
    /*00000320*/ BL          mn34220_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #13 
    /*0000032c*/ MOVW        R1, #12318                  @ 0x301e 
    /*00000330*/ BL          mn34220_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #16 
    /*0000033c*/ MOVW        R1, #12319                  @ 0x301f 
    /*00000340*/ BL          mn34220_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #8 
    /*0000034c*/ MOVW        R1, #12320                  @ 0x3020 
    /*00000350*/ BL          mn34220_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #0 
    /*0000035c*/ MOVW        R1, #12321                  @ 0x3021 
    /*00000360*/ BL          mn34220_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #0 
    /*0000036c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000370*/ BL          mn34220_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #83                     @ 0x53 
    /*0000037c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000380*/ BL          mn34220_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #0 
    /*0000038c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000390*/ BL          mn34220_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #0 
    /*0000039c*/ MOVW        R1, #12303                  @ 0x300f 
    /*000003a0*/ BL          mn34220_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #2 
    /*000003ac*/ MOVW        R1, #514                    @ 0x202 
    /*000003b0*/ BL          mn34220_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #236                    @ 0xec 
    /*000003bc*/ MOVW        R1, #515                    @ 0x203 
    /*000003c0*/ BL          mn34220_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #1 
    /*000003cc*/ MOV         R1, #516                    @ 0x204 
    /*000003d0*/ BL          mn34220_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #0 
    /*000003dc*/ MOVW        R1, #517                    @ 0x205 
    /*000003e0*/ BL          mn34220_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #1 
    /*000003ec*/ MOVW        R1, #526                    @ 0x20e 
    /*000003f0*/ BL          mn34220_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #0 
    /*000003fc*/ MOVW        R1, #527                    @ 0x20f 
    /*00000400*/ BL          mn34220_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #1 
    /*0000040c*/ MOV         R1, #528                    @ 0x210 
    /*00000410*/ BL          mn34220_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #0 
    /*0000041c*/ MOVW        R1, #529                    @ 0x211 
    /*00000420*/ BL          mn34220_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #1 
    /*0000042c*/ MOVW        R1, #530                    @ 0x212 
    /*00000430*/ BL          mn34220_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #0 
    /*0000043c*/ MOVW        R1, #531                    @ 0x213 
    /*00000440*/ BL          mn34220_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #1 
    /*0000044c*/ MOV         R1, #532                    @ 0x214 
    /*00000450*/ BL          mn34220_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #0 
    /*0000045c*/ MOVW        R1, #533                    @ 0x215 
    /*00000460*/ BL          mn34220_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #0 
    /*0000046c*/ MOV         R1, #544                    @ 0x220 
    /*00000470*/ BL          mn34220_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #0 
    /*0000047c*/ MOVW        R1, #545                    @ 0x221 
    /*00000480*/ BL          mn34220_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #0 
    /*0000048c*/ MOVW        R1, #830                    @ 0x33e 
    /*00000490*/ BL          mn34220_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #0 
    /*0000049c*/ MOVW        R1, #831                    @ 0x33f 
    /*000004a0*/ BL          mn34220_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #2 
    /*000004ac*/ MOV         R1, #832                    @ 0x340 
    /*000004b0*/ BL          mn34220_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #238                    @ 0xee 
    /*000004bc*/ MOVW        R1, #833                    @ 0x341 
    /*000004c0*/ BL          mn34220_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #17 
    /*000004cc*/ MOVW        R1, #834                    @ 0x342 
    /*000004d0*/ BL          mn34220_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #48                     @ 0x30 
    /*000004dc*/ MOVW        R1, #835                    @ 0x343 
    /*000004e0*/ BL          mn34220_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #0 
    /*000004ec*/ MOV         R1, #836                    @ 0x344 
    /*000004f0*/ BL          mn34220_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #0 
    /*000004fc*/ MOVW        R1, #837                    @ 0x345 
    /*00000500*/ BL          mn34220_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #0 
    /*0000050c*/ MOVW        R1, #838                    @ 0x346 
    /*00000510*/ BL          mn34220_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #242                    @ 0xf2 
    /*0000051c*/ MOVW        R1, #839                    @ 0x347 
    /*00000520*/ BL          mn34220_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #0 
    /*0000052c*/ MOV         R1, #840                    @ 0x348 
    /*00000530*/ BL          mn34220_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #0 
    /*0000053c*/ MOVW        R1, #841                    @ 0x349 
    /*00000540*/ BL          mn34220_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #3 
    /*0000054c*/ MOVW        R1, #842                    @ 0x34a 
    /*00000550*/ BL          mn34220_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #201                    @ 0xc9 
    /*0000055c*/ MOVW        R1, #843                    @ 0x34b 
    /*00000560*/ BL          mn34220_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #0 
    /*0000056c*/ MOV         R1, #844                    @ 0x34c 
    /*00000570*/ BL          mn34220_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #0 
    /*0000057c*/ MOVW        R1, #845                    @ 0x34d 
    /*00000580*/ BL          mn34220_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #2 
    /*0000058c*/ MOVW        R1, #846                    @ 0x34e 
    /*00000590*/ BL          mn34220_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #216                    @ 0xd8 
    /*0000059c*/ MOVW        R1, #847                    @ 0x34f 
    /*000005a0*/ BL          mn34220_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #0 
    /*000005ac*/ MOV         R1, #896                    @ 0x380 
    /*000005b0*/ BL          mn34220_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #1 
    /*000005bc*/ MOVW        R1, #897                    @ 0x381 
    /*000005c0*/ BL          mn34220_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #0 
    /*000005cc*/ MOVW        R1, #898                    @ 0x382 
    /*000005d0*/ BL          mn34220_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #1 
    /*000005dc*/ MOVW        R1, #899                    @ 0x383 
    /*000005e0*/ BL          mn34220_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #0 
    /*000005ec*/ MOV         R1, #900                    @ 0x384 
    /*000005f0*/ BL          mn34220_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #1 
    /*000005fc*/ MOVW        R1, #901                    @ 0x385 
    /*00000600*/ BL          mn34220_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #0 
    /*0000060c*/ MOVW        R1, #902                    @ 0x386 
    /*00000610*/ BL          mn34220_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #1 
    /*0000061c*/ MOVW        R1, #903                    @ 0x387 
    /*00000620*/ BL          mn34220_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #0 
    /*0000062c*/ MOV         R1, #1280                   @ 0x500 
    /*00000630*/ BL          mn34220_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #0 
    /*0000063c*/ MOVW        R1, #1281                   @ 0x501 
    /*00000640*/ BL          mn34220_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #0 
    /*0000064c*/ MOV         R1, #1536                   @ 0x600 
    /*00000650*/ BL          mn34220_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #0 
    /*0000065c*/ MOVW        R1, #1537                   @ 0x601 
    /*00000660*/ BL          mn34220_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #0 
    /*0000066c*/ MOVW        R1, #1538                   @ 0x602 
    /*00000670*/ BL          mn34220_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #0 
    /*0000067c*/ MOVW        R1, #1539                   @ 0x603 
    /*00000680*/ BL          mn34220_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #0 
    /*0000068c*/ MOVW        R1, #1540                   @ 0x604 
    /*00000690*/ BL          mn34220_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #0 
    /*0000069c*/ MOVW        R1, #1541                   @ 0x605 
    /*000006a0*/ BL          mn34220_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #0 
    /*000006ac*/ MOVW        R1, #1542                   @ 0x606 
    /*000006b0*/ BL          mn34220_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #0 
    /*000006bc*/ MOVW        R1, #1543                   @ 0x607 
    /*000006c0*/ BL          mn34220_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #0 
    /*000006cc*/ MOVW        R1, #1544                   @ 0x608 
    /*000006d0*/ BL          mn34220_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #0 
    /*000006dc*/ MOVW        R1, #1545                   @ 0x609 
    /*000006e0*/ BL          mn34220_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #0 
    /*000006ec*/ MOVW        R1, #1546                   @ 0x60a 
    /*000006f0*/ BL          mn34220_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #0 
    /*000006fc*/ MOVW        R1, #1547                   @ 0x60b 
    /*00000700*/ BL          mn34220_write_register
    /*00000704*/ MOV         R0, R4 
    /*00000708*/ MOV         R2, #0 
    /*0000070c*/ MOVW        R1, #1548                   @ 0x60c 
    /*00000710*/ BL          mn34220_write_register
    /*00000714*/ MOV         R0, R4 
    /*00000718*/ MOV         R2, #0 
    /*0000071c*/ MOVW        R1, #1549                   @ 0x60d 
    /*00000720*/ BL          mn34220_write_register
    /*00000724*/ MOV         R0, R4 
    /*00000728*/ MOV         R2, #0 
    /*0000072c*/ MOVW        R1, #1550                   @ 0x60e 
    /*00000730*/ BL          mn34220_write_register
    /*00000734*/ MOV         R0, R4 
    /*00000738*/ MOV         R2, #0 
    /*0000073c*/ MOVW        R1, #1551                   @ 0x60f 
    /*00000740*/ BL          mn34220_write_register
    /*00000744*/ MOV         R0, R4 
    /*00000748*/ MOV         R2, #0 
    /*0000074c*/ MOV         R1, #1552                   @ 0x610 
    /*00000750*/ BL          mn34220_write_register
    /*00000754*/ MOV         R0, R4 
    /*00000758*/ MOV         R2, #0 
    /*0000075c*/ MOVW        R1, #1553                   @ 0x611 
    /*00000760*/ BL          mn34220_write_register
    /*00000764*/ MOV         R0, R4 
    /*00000768*/ MOV         R2, #0 
    /*0000076c*/ MOVW        R1, #12336                  @ 0x3030 
    /*00000770*/ BL          mn34220_write_register
    /*00000774*/ MOV         R0, R4 
    /*00000778*/ MOV         R2, #0 
    /*0000077c*/ MOVW        R1, #12337                  @ 0x3031 
    /*00000780*/ BL          mn34220_write_register
    /*00000784*/ MOV         R0, R4 
    /*00000788*/ MOV         R2, #0 
    /*0000078c*/ MOVW        R1, #12338                  @ 0x3032 
    /*00000790*/ BL          mn34220_write_register
    /*00000794*/ MOV         R0, R4 
    /*00000798*/ MOV         R2, #0 
    /*0000079c*/ MOVW        R1, #12339                  @ 0x3033 
    /*000007a0*/ BL          mn34220_write_register
    /*000007a4*/ MOV         R0, R4 
    /*000007a8*/ MOV         R2, #0 
    /*000007ac*/ MOVW        R1, #12340                  @ 0x3034 
    /*000007b0*/ BL          mn34220_write_register
    /*000007b4*/ MOV         R0, R4 
    /*000007b8*/ MOV         R2, #0 
    /*000007bc*/ MOVW        R1, #12341                  @ 0x3035 
    /*000007c0*/ BL          mn34220_write_register
    /*000007c4*/ MOV         R0, R4 
    /*000007c8*/ MOV         R2, #0 
    /*000007cc*/ MOVW        R1, #12342                  @ 0x3036 
    /*000007d0*/ BL          mn34220_write_register
    /*000007d4*/ MOV         R0, R4 
    /*000007d8*/ MOV         R2, #16 
    /*000007dc*/ MOVW        R1, #12343                  @ 0x3037 
    /*000007e0*/ BL          mn34220_write_register
    /*000007e4*/ MOV         R0, R4 
    /*000007e8*/ MOV         R2, #255                    @ 0xff 
    /*000007ec*/ MOVW        R1, #12344                  @ 0x3038 
    /*000007f0*/ BL          mn34220_write_register
    /*000007f4*/ MOV         R0, R4 
    /*000007f8*/ MOV         R2, #46                     @ 0x2e 
    /*000007fc*/ MOVW        R1, #12345                  @ 0x3039 
    /*00000800*/ BL          mn34220_write_register
    /*00000804*/ MOV         R0, R4 
    /*00000808*/ MOV         R2, #0 
    /*0000080c*/ MOVW        R1, #12346                  @ 0x303a 
    /*00000810*/ BL          mn34220_write_register
    /*00000814*/ MOV         R0, R4 
    /*00000818*/ MOV         R2, #0 
    /*0000081c*/ MOVW        R1, #12347                  @ 0x303b 
    /*00000820*/ BL          mn34220_write_register
    /*00000824*/ MOV         R0, R4 
    /*00000828*/ MOV         R2, #0 
    /*0000082c*/ MOVW        R1, #12348                  @ 0x303c 
    /*00000830*/ BL          mn34220_write_register
    /*00000834*/ MOV         R0, R4 
    /*00000838*/ MOV         R2, #40                     @ 0x28 
    /*0000083c*/ MOVW        R1, #12349                  @ 0x303d 
    /*00000840*/ BL          mn34220_write_register
    /*00000844*/ MOV         R0, R4 
    /*00000848*/ MOV         R2, #3 
    /*0000084c*/ MOVW        R1, #12350                  @ 0x303e 
    /*00000850*/ BL          mn34220_write_register
    /*00000854*/ MOV         R0, R4 
    /*00000858*/ MOV         R2, #3 
    /*0000085c*/ MOVW        R1, #12351                  @ 0x303f 
    /*00000860*/ BL          mn34220_write_register
    /*00000864*/ MOV         R0, R4 
    /*00000868*/ MOV         R2, #192                    @ 0xc0 
    /*0000086c*/ MOV         R1, #12352                  @ 0x3040 
    /*00000870*/ BL          mn34220_write_register
    /*00000874*/ MOV         R0, R4 
    /*00000878*/ MOV         R2, #18 
    /*0000087c*/ MOVW        R1, #12353                  @ 0x3041 
    /*00000880*/ BL          mn34220_write_register
    /*00000884*/ MOV         R0, R4 
    /*00000888*/ MOV         R2, #80                     @ 0x50 
    /*0000088c*/ MOVW        R1, #12354                  @ 0x3042 
    /*00000890*/ BL          mn34220_write_register
    /*00000894*/ MOV         R0, R4 
    /*00000898*/ MOV         R2, #16 
    /*0000089c*/ MOVW        R1, #12355                  @ 0x3043 
    /*000008a0*/ BL          mn34220_write_register
    /*000008a4*/ MOV         R0, R4 
    /*000008a8*/ MOV         R2, #0 
    /*000008ac*/ MOVW        R1, #12356                  @ 0x3044 
    /*000008b0*/ BL          mn34220_write_register
    /*000008b4*/ MOV         R0, R4 
    /*000008b8*/ MOV         R2, #0 
    /*000008bc*/ MOVW        R1, #12357                  @ 0x3045 
    /*000008c0*/ BL          mn34220_write_register
    /*000008c4*/ MOV         R0, R4 
    /*000008c8*/ MOV         R2, #0 
    /*000008cc*/ MOVW        R1, #12368                  @ 0x3050 
    /*000008d0*/ BL          mn34220_write_register
    /*000008d4*/ MOV         R0, R4 
    /*000008d8*/ MOV         R2, #0 
    /*000008dc*/ MOVW        R1, #12369                  @ 0x3051 
    /*000008e0*/ BL          mn34220_write_register
    /*000008e4*/ MOV         R0, R4 
    /*000008e8*/ MOV         R2, #144                    @ 0x90 
    /*000008ec*/ MOVW        R1, #12370                  @ 0x3052 
    /*000008f0*/ BL          mn34220_write_register
    /*000008f4*/ MOV         R0, R4 
    /*000008f8*/ MOV         R2, #34                     @ 0x22 
    /*000008fc*/ MOVW        R1, #12371                  @ 0x3053 
    /*00000900*/ BL          mn34220_write_register
    /*00000904*/ MOV         R0, R4 
    /*00000908*/ MOV         R2, #48                     @ 0x30 
    /*0000090c*/ MOVW        R1, #12372                  @ 0x3054 
    /*00000910*/ BL          mn34220_write_register
    /*00000914*/ MOV         R0, R4 
    /*00000918*/ MOV         R2, #0 
    /*0000091c*/ MOVW        R1, #12373                  @ 0x3055 
    /*00000920*/ BL          mn34220_write_register
    /*00000924*/ MOV         R0, R4 
    /*00000928*/ MOV         R2, #1 
    /*0000092c*/ MOVW        R1, #12374                  @ 0x3056 
    /*00000930*/ BL          mn34220_write_register
    /*00000934*/ MOV         R0, R4 
    /*00000938*/ MOV         R2, #255                    @ 0xff 
    /*0000093c*/ MOVW        R1, #12375                  @ 0x3057 
    /*00000940*/ BL          mn34220_write_register
    /*00000944*/ MOV         R0, R4 
    /*00000948*/ MOV         R2, #15 
    /*0000094c*/ MOVW        R1, #12376                  @ 0x3058 
    /*00000950*/ BL          mn34220_write_register
    /*00000954*/ MOV         R0, R4 
    /*00000958*/ MOV         R2, #254                    @ 0xfe 
    /*0000095c*/ MOVW        R1, #12377                  @ 0x3059 
    /*00000960*/ BL          mn34220_write_register
    /*00000964*/ MOV         R0, R4 
    /*00000968*/ MOV         R2, #0 
    /*0000096c*/ MOVW        R1, #12378                  @ 0x305a 
    /*00000970*/ BL          mn34220_write_register
    /*00000974*/ MOV         R0, R4 
    /*00000978*/ MOV         R2, #1 
    /*0000097c*/ MOVW        R1, #12379                  @ 0x305b 
    /*00000980*/ BL          mn34220_write_register
    /*00000984*/ MOV         R0, R4 
    /*00000988*/ MOV         R2, #0 
    /*0000098c*/ MOVW        R1, #12380                  @ 0x305c 
    /*00000990*/ BL          mn34220_write_register
    /*00000994*/ MOV         R0, R4 
    /*00000998*/ MOV         R2, #49                     @ 0x31 
    /*0000099c*/ MOVW        R1, #12381                  @ 0x305d 
    /*000009a0*/ BL          mn34220_write_register
    /*000009a4*/ MOV         R0, R4 
    /*000009a8*/ MOV         R2, #0 
    /*000009ac*/ MOVW        R1, #12382                  @ 0x305e 
    /*000009b0*/ BL          mn34220_write_register
    /*000009b4*/ MOV         R0, R4 
    /*000009b8*/ MOV         R2, #0 
    /*000009bc*/ MOVW        R1, #12383                  @ 0x305f 
    /*000009c0*/ BL          mn34220_write_register
    /*000009c4*/ MOV         R0, R4 
    /*000009c8*/ MOV         R2, #0 
    /*000009cc*/ MOVW        R1, #12384                  @ 0x3060 
    /*000009d0*/ BL          mn34220_write_register
    /*000009d4*/ MOV         R0, R4 
    /*000009d8*/ MOV         R2, #0 
    /*000009dc*/ MOVW        R1, #12385                  @ 0x3061 
    /*000009e0*/ BL          mn34220_write_register
    /*000009e4*/ MOV         R0, R4 
    /*000009e8*/ MOV         R2, #32 
    /*000009ec*/ MOVW        R1, #12386                  @ 0x3062 
    /*000009f0*/ BL          mn34220_write_register
    /*000009f4*/ MOV         R0, R4 
    /*000009f8*/ MOV         R2, #20 
    /*000009fc*/ MOVW        R1, #12387                  @ 0x3063 
    /*00000a00*/ BL          mn34220_write_register
    /*00000a04*/ MOV         R0, R4 
    /*00000a08*/ MOV         R2, #16 
    /*00000a0c*/ MOVW        R1, #12388                  @ 0x3064 
    /*00000a10*/ BL          mn34220_write_register
    /*00000a14*/ MOV         R0, R4 
    /*00000a18*/ MOV         R2, #50                     @ 0x32 
    /*00000a1c*/ MOVW        R1, #12389                  @ 0x3065 
    /*00000a20*/ BL          mn34220_write_register
    /*00000a24*/ MOV         R0, R4 
    /*00000a28*/ MOV         R2, #0 
    /*00000a2c*/ MOVW        R1, #12390                  @ 0x3066 
    /*00000a30*/ BL          mn34220_write_register
    /*00000a34*/ MOV         R0, R4 
    /*00000a38*/ MOV         R2, #2 
    /*00000a3c*/ MOVW        R1, #12391                  @ 0x3067 
    /*00000a40*/ BL          mn34220_write_register
    /*00000a44*/ MOV         R0, R4 
    /*00000a48*/ MOV         R2, #0 
    /*00000a4c*/ MOVW        R1, #12392                  @ 0x3068 
    /*00000a50*/ BL          mn34220_write_register
    /*00000a54*/ MOV         R0, R4 
    /*00000a58*/ MOV         R2, #0 
    /*00000a5c*/ MOVW        R1, #12393                  @ 0x3069 
    /*00000a60*/ BL          mn34220_write_register
    /*00000a64*/ MOV         R0, R4 
    /*00000a68*/ MOV         R2, #0 
    /*00000a6c*/ MOVW        R1, #12394                  @ 0x306a 
    /*00000a70*/ BL          mn34220_write_register
    /*00000a74*/ MOV         R0, R4 
    /*00000a78*/ MOV         R2, #3 
    /*00000a7c*/ MOVW        R1, #12395                  @ 0x306b 
    /*00000a80*/ BL          mn34220_write_register
    /*00000a84*/ MOV         R0, R4 
    /*00000a88*/ MOV         R2, #0 
    /*00000a8c*/ MOVW        R1, #12396                  @ 0x306c 
    /*00000a90*/ BL          mn34220_write_register
    /*00000a94*/ MOV         R0, R4 
    /*00000a98*/ MOV         R2, #1 
    /*00000a9c*/ MOVW        R1, #12397                  @ 0x306d 
    /*00000aa0*/ BL          mn34220_write_register
    /*00000aa4*/ MOV         R0, R4 
    /*00000aa8*/ MOV         R2, #12 
    /*00000aac*/ MOVW        R1, #12398                  @ 0x306e 
    /*00000ab0*/ BL          mn34220_write_register
    /*00000ab4*/ MOV         R0, R4 
    /*00000ab8*/ MOV         R2, #0 
    /*00000abc*/ MOVW        R1, #12399                  @ 0x306f 
    /*00000ac0*/ BL          mn34220_write_register
    /*00000ac4*/ MOV         R0, R4 
    /*00000ac8*/ MOV         R2, #63                     @ 0x3f 
    /*00000acc*/ MOVW        R1, #12400                  @ 0x3070 
    /*00000ad0*/ BL          mn34220_write_register
    /*00000ad4*/ MOV         R0, R4 
    /*00000ad8*/ MOV         R2, #255                    @ 0xff 
    /*00000adc*/ MOVW        R1, #12401                  @ 0x3071 
    /*00000ae0*/ BL          mn34220_write_register
    /*00000ae4*/ MOV         R0, R4 
    /*00000ae8*/ MOV         R2, #63                     @ 0x3f 
    /*00000aec*/ MOVW        R1, #12402                  @ 0x3072 
    /*00000af0*/ BL          mn34220_write_register
    /*00000af4*/ MOV         R0, R4 
    /*00000af8*/ MOV         R2, #255                    @ 0xff 
    /*00000afc*/ MOVW        R1, #12403                  @ 0x3073 
    /*00000b00*/ BL          mn34220_write_register
    /*00000b04*/ MOV         R0, R4 
    /*00000b08*/ MOV         R2, #1 
    /*00000b0c*/ MOVW        R1, #12404                  @ 0x3074 
    /*00000b10*/ BL          mn34220_write_register
    /*00000b14*/ MOV         R0, R4 
    /*00000b18*/ MOV         R2, #1 
    /*00000b1c*/ MOVW        R1, #12405                  @ 0x3075 
    /*00000b20*/ BL          mn34220_write_register
    /*00000b24*/ MOV         R0, R4 
    /*00000b28*/ MOV         R2, #66                     @ 0x42 
    /*00000b2c*/ MOVW        R1, #12406                  @ 0x3076 
    /*00000b30*/ BL          mn34220_write_register
    /*00000b34*/ MOV         R0, R4 
    /*00000b38*/ MOV         R2, #0 
    /*00000b3c*/ MOVW        R1, #12407                  @ 0x3077 
    /*00000b40*/ BL          mn34220_write_register
    /*00000b44*/ MOV         R0, R4 
    /*00000b48*/ MOV         R2, #0 
    /*00000b4c*/ MOVW        R1, #12408                  @ 0x3078 
    /*00000b50*/ BL          mn34220_write_register
    /*00000b54*/ MOV         R0, R4 
    /*00000b58*/ MOV         R2, #19 
    /*00000b5c*/ MOVW        R1, #12409                  @ 0x3079 
    /*00000b60*/ BL          mn34220_write_register
    /*00000b64*/ MOV         R0, R4 
    /*00000b68*/ MOV         R2, #18 
    /*00000b6c*/ MOVW        R1, #12410                  @ 0x307a 
    /*00000b70*/ BL          mn34220_write_register
    /*00000b74*/ MOV         R0, R4 
    /*00000b78*/ MOV         R2, #48                     @ 0x30 
    /*00000b7c*/ MOVW        R1, #12411                  @ 0x307b 
    /*00000b80*/ BL          mn34220_write_register
    /*00000b84*/ MOV         R0, R4 
    /*00000b88*/ MOV         R2, #43                     @ 0x2b 
    /*00000b8c*/ MOVW        R1, #12412                  @ 0x307c 
    /*00000b90*/ BL          mn34220_write_register
    /*00000b94*/ MOV         R0, R4 
    /*00000b98*/ MOV         R2, #16 
    /*00000b9c*/ MOVW        R1, #12413                  @ 0x307d 
    /*00000ba0*/ BL          mn34220_write_register
    /*00000ba4*/ MOV         R0, R4 
    /*00000ba8*/ MOV         R2, #0 
    /*00000bac*/ MOVW        R1, #12414                  @ 0x307e 
    /*00000bb0*/ BL          mn34220_write_register
    /*00000bb4*/ MOV         R0, R4 
    /*00000bb8*/ MOV         R2, #0 
    /*00000bbc*/ MOVW        R1, #12415                  @ 0x307f 
    /*00000bc0*/ BL          mn34220_write_register
    /*00000bc4*/ MOV         R0, R4 
    /*00000bc8*/ MOV         R2, #0 
    /*00000bcc*/ MOV         R1, #12416                  @ 0x3080 
    /*00000bd0*/ BL          mn34220_write_register
    /*00000bd4*/ MOV         R0, R4 
    /*00000bd8*/ MOV         R2, #0 
    /*00000bdc*/ MOVW        R1, #12417                  @ 0x3081 
    /*00000be0*/ BL          mn34220_write_register
    /*00000be4*/ MOV         R0, R4 
    /*00000be8*/ MOV         R2, #0 
    /*00000bec*/ MOVW        R1, #12418                  @ 0x3082 
    /*00000bf0*/ BL          mn34220_write_register
    /*00000bf4*/ MOV         R0, R4 
    /*00000bf8*/ MOV         R2, #0 
    /*00000bfc*/ MOVW        R1, #12419                  @ 0x3083 
    /*00000c00*/ BL          mn34220_write_register
    /*00000c04*/ MOV         R0, R4 
    /*00000c08*/ MOV         R2, #0 
    /*00000c0c*/ MOVW        R1, #12420                  @ 0x3084 
    /*00000c10*/ BL          mn34220_write_register
    /*00000c14*/ MOV         R0, R4 
    /*00000c18*/ MOV         R2, #48                     @ 0x30 
    /*00000c1c*/ MOVW        R1, #12421                  @ 0x3085 
    /*00000c20*/ BL          mn34220_write_register
    /*00000c24*/ MOV         R0, R4 
    /*00000c28*/ MOV         R2, #35                     @ 0x23 
    /*00000c2c*/ MOVW        R1, #12422                  @ 0x3086 
    /*00000c30*/ BL          mn34220_write_register
    /*00000c34*/ MOV         R0, R4 
    /*00000c38*/ MOV         R2, #3 
    /*00000c3c*/ MOVW        R1, #12423                  @ 0x3087 
    /*00000c40*/ BL          mn34220_write_register
    /*00000c44*/ MOV         R0, R4 
    /*00000c48*/ MOV         R2, #20 
    /*00000c4c*/ MOVW        R1, #12424                  @ 0x3088 
    /*00000c50*/ BL          mn34220_write_register
    /*00000c54*/ MOV         R0, R4 
    /*00000c58*/ MOV         R2, #4 
    /*00000c5c*/ MOVW        R1, #12425                  @ 0x3089 
    /*00000c60*/ BL          mn34220_write_register
    /*00000c64*/ MOV         R0, R4 
    /*00000c68*/ MOV         R2, #1 
    /*00000c6c*/ MOVW        R1, #12426                  @ 0x308a 
    /*00000c70*/ BL          mn34220_write_register
    /*00000c74*/ MOV         R0, R4 
    /*00000c78*/ MOV         R2, #11 
    /*00000c7c*/ MOVW        R1, #12427                  @ 0x308b 
    /*00000c80*/ BL          mn34220_write_register
    /*00000c84*/ MOV         R0, R4 
    /*00000c88*/ MOV         R2, #8 
    /*00000c8c*/ MOVW        R1, #12428                  @ 0x308c 
    /*00000c90*/ BL          mn34220_write_register
    /*00000c94*/ MOV         R0, R4 
    /*00000c98*/ MOV         R2, #4 
    /*00000c9c*/ MOVW        R1, #12429                  @ 0x308d 
    /*00000ca0*/ BL          mn34220_write_register
    /*00000ca4*/ MOV         R0, R4 
    /*00000ca8*/ MOV         R2, #4 
    /*00000cac*/ MOVW        R1, #12430                  @ 0x308e 
    /*00000cb0*/ BL          mn34220_write_register
    /*00000cb4*/ MOV         R0, R4 
    /*00000cb8*/ MOV         R2, #9 
    /*00000cbc*/ MOVW        R1, #12431                  @ 0x308f 
    /*00000cc0*/ BL          mn34220_write_register
    /*00000cc4*/ MOV         R0, R4 
    /*00000cc8*/ MOV         R2, #5 
    /*00000ccc*/ MOVW        R1, #12432                  @ 0x3090 
    /*00000cd0*/ BL          mn34220_write_register
    /*00000cd4*/ MOV         R0, R4 
    /*00000cd8*/ MOV         R2, #0 
    /*00000cdc*/ MOVW        R1, #12433                  @ 0x3091 
    /*00000ce0*/ BL          mn34220_write_register
    /*00000ce4*/ MOV         R0, R4 
    /*00000ce8*/ MOV         R2, #0 
    /*00000cec*/ MOVW        R1, #12434                  @ 0x3092 
    /*00000cf0*/ BL          mn34220_write_register
    /*00000cf4*/ MOV         R0, R4 
    /*00000cf8*/ MOV         R2, #9 
    /*00000cfc*/ MOVW        R1, #12435                  @ 0x3093 
    /*00000d00*/ BL          mn34220_write_register
    /*00000d04*/ MOV         R0, R4 
    /*00000d08*/ MOV         R2, #0 
    /*00000d0c*/ MOVW        R1, #12436                  @ 0x3094 
    /*00000d10*/ BL          mn34220_write_register
    /*00000d14*/ MOV         R0, R4 
    /*00000d18*/ MOV         R2, #0 
    /*00000d1c*/ MOVW        R1, #12437                  @ 0x3095 
    /*00000d20*/ BL          mn34220_write_register
    /*00000d24*/ MOV         R0, R4 
    /*00000d28*/ MOV         R2, #0 
    /*00000d2c*/ MOVW        R1, #12438                  @ 0x3096 
    /*00000d30*/ BL          mn34220_write_register
    /*00000d34*/ MOV         R0, R4 
    /*00000d38*/ MOV         R2, #0 
    /*00000d3c*/ MOVW        R1, #12439                  @ 0x3097 
    /*00000d40*/ BL          mn34220_write_register
    /*00000d44*/ MOV         R0, R4 
    /*00000d48*/ MOV         R2, #0 
    /*00000d4c*/ MOVW        R1, #12440                  @ 0x3098 
    /*00000d50*/ BL          mn34220_write_register
    /*00000d54*/ MOV         R0, R4 
    /*00000d58*/ MOV         R2, #64                     @ 0x40 
    /*00000d5c*/ MOVW        R1, #12441                  @ 0x3099 
    /*00000d60*/ BL          mn34220_write_register
    /*00000d64*/ MOV         R0, R4 
    /*00000d68*/ MOV         R2, #16 
    /*00000d6c*/ MOVW        R1, #12442                  @ 0x309a 
    /*00000d70*/ BL          mn34220_write_register
    /*00000d74*/ MOV         R0, R4 
    /*00000d78*/ MOV         R2, #0 
    /*00000d7c*/ MOVW        R1, #12443                  @ 0x309b 
    /*00000d80*/ BL          mn34220_write_register
    /*00000d84*/ MOV         R0, R4 
    /*00000d88*/ MOV         R2, #83                     @ 0x53 
    /*00000d8c*/ MOVW        R1, #12444                  @ 0x309c 
    /*00000d90*/ BL          mn34220_write_register
    /*00000d94*/ MOV         R0, R4 
    /*00000d98*/ MOV         R2, #84                     @ 0x54 
    /*00000d9c*/ MOVW        R1, #12445                  @ 0x309d 
    /*00000da0*/ BL          mn34220_write_register
    /*00000da4*/ MOV         R0, R4 
    /*00000da8*/ MOV         R2, #1 
    /*00000dac*/ MOVW        R1, #12446                  @ 0x309e 
    /*00000db0*/ BL          mn34220_write_register
    /*00000db4*/ MOV         R0, R4 
    /*00000db8*/ MOV         R2, #0 
    /*00000dbc*/ MOVW        R1, #12447                  @ 0x309f 
    /*00000dc0*/ BL          mn34220_write_register
    /*00000dc4*/ MOV         R0, R4 
    /*00000dc8*/ MOV         R2, #0 
    /*00000dcc*/ MOV         R1, #12544                  @ 0x3100 
    /*00000dd0*/ BL          mn34220_write_register
    /*00000dd4*/ MOV         R0, R4 
    /*00000dd8*/ MOV         R2, #1 
    /*00000ddc*/ MOVW        R1, #12545                  @ 0x3101 
    /*00000de0*/ BL          mn34220_write_register
    /*00000de4*/ MOV         R0, R4 
    /*00000de8*/ MOV         R2, #0 
    /*00000dec*/ MOVW        R1, #12546                  @ 0x3102 
    /*00000df0*/ BL          mn34220_write_register
    /*00000df4*/ MOV         R0, R4 
    /*00000df8*/ MOV         R2, #0 
    /*00000dfc*/ MOVW        R1, #12547                  @ 0x3103 
    /*00000e00*/ BL          mn34220_write_register
    /*00000e04*/ MOV         R0, R4 
    /*00000e08*/ MOV         R2, #4 
    /*00000e0c*/ MOVW        R1, #12548                  @ 0x3104 
    /*00000e10*/ BL          mn34220_write_register
    /*00000e14*/ MOV         R0, R4 
    /*00000e18*/ MOV         R2, #0 
    /*00000e1c*/ MOVW        R1, #12549                  @ 0x3105 
    /*00000e20*/ BL          mn34220_write_register
    /*00000e24*/ MOV         R0, R4 
    /*00000e28*/ MOV         R2, #0 
    /*00000e2c*/ MOVW        R1, #12550                  @ 0x3106 
    /*00000e30*/ BL          mn34220_write_register
    /*00000e34*/ MOV         R0, R4 
    /*00000e38*/ MOV         R2, #192                    @ 0xc0 
    /*00000e3c*/ MOVW        R1, #12551                  @ 0x3107 
    /*00000e40*/ BL          mn34220_write_register
    /*00000e44*/ MOV         R0, R4 
    /*00000e48*/ MOV         R2, #1 
    /*00000e4c*/ MOVW        R1, #12552                  @ 0x3108 
    /*00000e50*/ BL          mn34220_write_register
    /*00000e54*/ MOV         R0, R4 
    /*00000e58*/ MOV         R2, #0 
    /*00000e5c*/ MOVW        R1, #12553                  @ 0x3109 
    /*00000e60*/ BL          mn34220_write_register
    /*00000e64*/ MOV         R0, R4 
    /*00000e68*/ MOV         R2, #1 
    /*00000e6c*/ MOVW        R1, #12554                  @ 0x310a 
    /*00000e70*/ BL          mn34220_write_register
    /*00000e74*/ MOV         R0, R4 
    /*00000e78*/ MOV         R2, #0 
    /*00000e7c*/ MOVW        R1, #12555                  @ 0x310b 
    /*00000e80*/ BL          mn34220_write_register
    /*00000e84*/ MOV         R0, R4 
    /*00000e88*/ MOV         R2, #1 
    /*00000e8c*/ MOVW        R1, #12556                  @ 0x310c 
    /*00000e90*/ BL          mn34220_write_register
    /*00000e94*/ MOV         R0, R4 
    /*00000e98*/ MOV         R2, #0 
    /*00000e9c*/ MOVW        R1, #12557                  @ 0x310d 
    /*00000ea0*/ BL          mn34220_write_register
    /*00000ea4*/ MOV         R0, R4 
    /*00000ea8*/ MOV         R2, #1 
    /*00000eac*/ MOVW        R1, #12558                  @ 0x310e 
    /*00000eb0*/ BL          mn34220_write_register
    /*00000eb4*/ MOV         R0, R4 
    /*00000eb8*/ MOV         R2, #0 
    /*00000ebc*/ MOVW        R1, #12559                  @ 0x310f 
    /*00000ec0*/ BL          mn34220_write_register
    /*00000ec4*/ MOV         R0, R4 
    /*00000ec8*/ MOV         R2, #1 
    /*00000ecc*/ MOVW        R1, #12560                  @ 0x3110 
    /*00000ed0*/ BL          mn34220_write_register
    /*00000ed4*/ MOV         R0, R4 
    /*00000ed8*/ MOV         R2, #0 
    /*00000edc*/ MOVW        R1, #12561                  @ 0x3111 
    /*00000ee0*/ BL          mn34220_write_register
    /*00000ee4*/ MOV         R0, R4 
    /*00000ee8*/ MOV         R2, #1 
    /*00000eec*/ MOVW        R1, #12562                  @ 0x3112 
    /*00000ef0*/ BL          mn34220_write_register
    /*00000ef4*/ MOV         R0, R4 
    /*00000ef8*/ MOV         R2, #0 
    /*00000efc*/ MOVW        R1, #12563                  @ 0x3113 
    /*00000f00*/ BL          mn34220_write_register
    /*00000f04*/ MOV         R0, R4 
    /*00000f08*/ MOV         R2, #1 
    /*00000f0c*/ MOVW        R1, #12564                  @ 0x3114 
    /*00000f10*/ BL          mn34220_write_register
    /*00000f14*/ MOV         R0, R4 
    /*00000f18*/ MOV         R2, #0 
    /*00000f1c*/ MOVW        R1, #12565                  @ 0x3115 
    /*00000f20*/ BL          mn34220_write_register
    /*00000f24*/ MOV         R0, R4 
    /*00000f28*/ MOV         R2, #1 
    /*00000f2c*/ MOVW        R1, #12566                  @ 0x3116 
    /*00000f30*/ BL          mn34220_write_register
    /*00000f34*/ MOV         R0, R4 
    /*00000f38*/ MOV         R2, #0 
    /*00000f3c*/ MOVW        R1, #12567                  @ 0x3117 
    /*00000f40*/ BL          mn34220_write_register
    /*00000f44*/ MOV         R0, R4 
    /*00000f48*/ MOV         R2, #1 
    /*00000f4c*/ MOVW        R1, #12568                  @ 0x3118 
    /*00000f50*/ BL          mn34220_write_register
    /*00000f54*/ MOV         R0, R4 
    /*00000f58*/ MOV         R2, #0 
    /*00000f5c*/ MOVW        R1, #12569                  @ 0x3119 
    /*00000f60*/ BL          mn34220_write_register
    /*00000f64*/ MOV         R0, R4 
    /*00000f68*/ MOV         R2, #1 
    /*00000f6c*/ MOVW        R1, #12570                  @ 0x311a 
    /*00000f70*/ BL          mn34220_write_register
    /*00000f74*/ MOV         R0, R4 
    /*00000f78*/ MOV         R2, #0 
    /*00000f7c*/ MOVW        R1, #12571                  @ 0x311b 
    /*00000f80*/ BL          mn34220_write_register
    /*00000f84*/ MOV         R0, R4 
    /*00000f88*/ MOV         R2, #1 
    /*00000f8c*/ MOVW        R1, #12572                  @ 0x311c 
    /*00000f90*/ BL          mn34220_write_register
    /*00000f94*/ MOV         R0, R4 
    /*00000f98*/ MOV         R2, #0 
    /*00000f9c*/ MOVW        R1, #12573                  @ 0x311d 
    /*00000fa0*/ BL          mn34220_write_register
    /*00000fa4*/ MOV         R0, R4 
    /*00000fa8*/ MOV         R2, #1 
    /*00000fac*/ MOVW        R1, #12574                  @ 0x311e 
    /*00000fb0*/ BL          mn34220_write_register
    /*00000fb4*/ MOV         R0, R4 
    /*00000fb8*/ MOV         R2, #0 
    /*00000fbc*/ MOVW        R1, #12575                  @ 0x311f 
    /*00000fc0*/ BL          mn34220_write_register
    /*00000fc4*/ MOV         R0, R4 
    /*00000fc8*/ MOV         R2, #1 
    /*00000fcc*/ MOVW        R1, #12576                  @ 0x3120 
    /*00000fd0*/ BL          mn34220_write_register
    /*00000fd4*/ MOV         R0, R4 
    /*00000fd8*/ MOV         R2, #0 
    /*00000fdc*/ MOVW        R1, #12577                  @ 0x3121 
    /*00000fe0*/ BL          mn34220_write_register
    /*00000fe4*/ MOV         R0, R4 
    /*00000fe8*/ MOV         R2, #0 
    /*00000fec*/ MOVW        R1, #12578                  @ 0x3122 
    /*00000ff0*/ BL          mn34220_write_register
    /*00000ff4*/ MOV         R0, R4 
    /*00000ff8*/ MOV         R2, #0 
    /*00000ffc*/ MOVW        R1, #12579                  @ 0x3123 
    /*00001000*/ BL          mn34220_write_register
    /*00001004*/ MOV         R0, R4 
    /*00001008*/ MOV         R2, #0 
    /*0000100c*/ MOVW        R1, #12580                  @ 0x3124 
    /*00001010*/ BL          mn34220_write_register
    /*00001014*/ MOV         R0, R4 
    /*00001018*/ MOV         R2, #0 
    /*0000101c*/ MOVW        R1, #12581                  @ 0x3125 
    /*00001020*/ BL          mn34220_write_register
    /*00001024*/ MOV         R0, R4 
    /*00001028*/ MOV         R2, #0 
    /*0000102c*/ MOVW        R1, #12582                  @ 0x3126 
    /*00001030*/ BL          mn34220_write_register
    /*00001034*/ MOV         R0, R4 
    /*00001038*/ MOV         R2, #2 
    /*0000103c*/ MOVW        R1, #12583                  @ 0x3127 
    /*00001040*/ BL          mn34220_write_register
    /*00001044*/ MOV         R0, R4 
    /*00001048*/ MOV         R2, #0 
    /*0000104c*/ MOVW        R1, #12584                  @ 0x3128 
    /*00001050*/ BL          mn34220_write_register
    /*00001054*/ MOV         R0, R4 
    /*00001058*/ MOV         R2, #0 
    /*0000105c*/ MOVW        R1, #12585                  @ 0x3129 
    /*00001060*/ BL          mn34220_write_register
    /*00001064*/ MOV         R0, R4 
    /*00001068*/ MOV         R2, #0 
    /*0000106c*/ MOVW        R1, #12586                  @ 0x312a 
    /*00001070*/ BL          mn34220_write_register
    /*00001074*/ MOV         R0, R4 
    /*00001078*/ MOV         R2, #32 
    /*0000107c*/ MOVW        R1, #12587                  @ 0x312b 
    /*00001080*/ BL          mn34220_write_register
    /*00001084*/ MOV         R0, R4 
    /*00001088*/ MOV         R2, #0 
    /*0000108c*/ MOVW        R1, #12588                  @ 0x312c 
    /*00001090*/ BL          mn34220_write_register
    /*00001094*/ MOV         R0, R4 
    /*00001098*/ MOV         R2, #32 
    /*0000109c*/ MOVW        R1, #12589                  @ 0x312d 
    /*000010a0*/ BL          mn34220_write_register
    /*000010a4*/ MOV         R0, R4 
    /*000010a8*/ MOV         R2, #0 
    /*000010ac*/ MOVW        R1, #12590                  @ 0x312e 
    /*000010b0*/ BL          mn34220_write_register
    /*000010b4*/ MOV         R0, R4 
    /*000010b8*/ MOV         R2, #32 
    /*000010bc*/ MOVW        R1, #12591                  @ 0x312f 
    /*000010c0*/ BL          mn34220_write_register
    /*000010c4*/ MOV         R0, R4 
    /*000010c8*/ MOV         R2, #15 
    /*000010cc*/ MOVW        R1, #12592                  @ 0x3130 
    /*000010d0*/ BL          mn34220_write_register
    /*000010d4*/ MOV         R0, R4 
    /*000010d8*/ MOV         R2, #1 
    /*000010dc*/ MOVW        R1, #12593                  @ 0x3131 
    /*000010e0*/ BL          mn34220_write_register
    /*000010e4*/ MOV         R0, R4 
    /*000010e8*/ MOV         R2, #103                    @ 0x67 
    /*000010ec*/ MOVW        R1, #12594                  @ 0x3132 
    /*000010f0*/ BL          mn34220_write_register
    /*000010f4*/ MOV         R0, R4 
    /*000010f8*/ MOV         R2, #16 
    /*000010fc*/ MOVW        R1, #12595                  @ 0x3133 
    /*00001100*/ BL          mn34220_write_register
    /*00001104*/ MOV         R0, R4 
    /*00001108*/ MOV         R2, #112                    @ 0x70 
    /*0000110c*/ MOVW        R1, #12596                  @ 0x3134 
    /*00001110*/ BL          mn34220_write_register
    /*00001114*/ MOV         R0, R4 
    /*00001118*/ MOV         R2, #8 
    /*0000111c*/ MOVW        R1, #12597                  @ 0x3135 
    /*00001120*/ BL          mn34220_write_register
    /*00001124*/ MOV         R0, R4 
    /*00001128*/ MOV         R2, #3 
    /*0000112c*/ MOVW        R1, #12598                  @ 0x3136 
    /*00001130*/ BL          mn34220_write_register
    /*00001134*/ MOV         R0, R4 
    /*00001138*/ MOV         R2, #255                    @ 0xff 
    /*0000113c*/ MOVW        R1, #12599                  @ 0x3137 
    /*00001140*/ BL          mn34220_write_register
    /*00001144*/ MOV         R0, R4 
    /*00001148*/ MOV         R2, #0 
    /*0000114c*/ MOVW        R1, #12600                  @ 0x3138 
    /*00001150*/ BL          mn34220_write_register
    /*00001154*/ MOV         R0, R4 
    /*00001158*/ MOV         R2, #0 
    /*0000115c*/ MOVW        R1, #12601                  @ 0x3139 
    /*00001160*/ BL          mn34220_write_register
    /*00001164*/ MOV         R0, R4 
    /*00001168*/ MOV         R2, #0 
    /*0000116c*/ MOVW        R1, #12602                  @ 0x313a 
    /*00001170*/ BL          mn34220_write_register
    /*00001174*/ MOV         R0, R4 
    /*00001178*/ MOV         R2, #1 
    /*0000117c*/ MOVW        R1, #12603                  @ 0x313b 
    /*00001180*/ BL          mn34220_write_register
    /*00001184*/ MOV         R0, R4 
    /*00001188*/ MOV         R2, #0 
    /*0000118c*/ MOVW        R1, #12604                  @ 0x313c 
    /*00001190*/ BL          mn34220_write_register
    /*00001194*/ MOV         R0, R4 
    /*00001198*/ MOV         R2, #0 
    /*0000119c*/ MOVW        R1, #12605                  @ 0x313d 
    /*000011a0*/ BL          mn34220_write_register
    /*000011a4*/ MOV         R0, R4 
    /*000011a8*/ MOV         R2, #0 
    /*000011ac*/ MOV         R1, #12608                  @ 0x3140 
    /*000011b0*/ BL          mn34220_write_register
    /*000011b4*/ MOV         R0, R4 
    /*000011b8*/ MOV         R2, #112                    @ 0x70 
    /*000011bc*/ MOVW        R1, #12609                  @ 0x3141 
    /*000011c0*/ BL          mn34220_write_register
    /*000011c4*/ MOV         R0, R4 
    /*000011c8*/ MOV         R2, #80                     @ 0x50 
    /*000011cc*/ MOVW        R1, #12610                  @ 0x3142 
    /*000011d0*/ BL          mn34220_write_register
    /*000011d4*/ MOV         R0, R4 
    /*000011d8*/ MOV         R2, #1 
    /*000011dc*/ MOVW        R1, #12611                  @ 0x3143 
    /*000011e0*/ BL          mn34220_write_register
    /*000011e4*/ MOV         R0, R4 
    /*000011e8*/ MOV         R2, #3 
    /*000011ec*/ MOVW        R1, #12612                  @ 0x3144 
    /*000011f0*/ BL          mn34220_write_register
    /*000011f4*/ MOV         R0, R4 
    /*000011f8*/ MOV         R2, #2 
    /*000011fc*/ MOVW        R1, #12613                  @ 0x3145 
    /*00001200*/ BL          mn34220_write_register
    /*00001204*/ MOV         R0, R4 
    /*00001208*/ MOV         R2, #5 
    /*0000120c*/ MOVW        R1, #12614                  @ 0x3146 
    /*00001210*/ BL          mn34220_write_register
    /*00001214*/ MOV         R0, R4 
    /*00001218*/ MOV         R2, #0 
    /*0000121c*/ MOVW        R1, #12615                  @ 0x3147 
    /*00001220*/ BL          mn34220_write_register
    /*00001224*/ MOV         R0, R4 
    /*00001228*/ MOV         R2, #0 
    /*0000122c*/ MOVW        R1, #12616                  @ 0x3148 
    /*00001230*/ BL          mn34220_write_register
    /*00001234*/ MOV         R0, R4 
    /*00001238*/ MOV         R2, #0 
    /*0000123c*/ MOVW        R1, #12617                  @ 0x3149 
    /*00001240*/ BL          mn34220_write_register
    /*00001244*/ MOV         R0, R4 
    /*00001248*/ MOV         R2, #3 
    /*0000124c*/ MOVW        R1, #12618                  @ 0x314a 
    /*00001250*/ BL          mn34220_write_register
    /*00001254*/ MOV         R0, R4 
    /*00001258*/ MOV         R2, #1 
    /*0000125c*/ MOVW        R1, #12619                  @ 0x314b 
    /*00001260*/ BL          mn34220_write_register
    /*00001264*/ MOV         R0, R4 
    /*00001268*/ MOV         R2, #1 
    /*0000126c*/ MOVW        R1, #12620                  @ 0x314c 
    /*00001270*/ BL          mn34220_write_register
    /*00001274*/ MOV         R0, R4 
    /*00001278*/ MOV         R2, #1 
    /*0000127c*/ MOVW        R1, #12621                  @ 0x314d 
    /*00001280*/ BL          mn34220_write_register
    /*00001284*/ MOV         R0, R4 
    /*00001288*/ MOV         R2, #2 
    /*0000128c*/ MOVW        R1, #12622                  @ 0x314e 
    /*00001290*/ BL          mn34220_write_register
    /*00001294*/ MOV         R0, R4 
    /*00001298*/ MOV         R2, #2 
    /*0000129c*/ MOVW        R1, #12623                  @ 0x314f 
    /*000012a0*/ BL          mn34220_write_register
    /*000012a4*/ MOV         R0, R4 
    /*000012a8*/ MOV         R2, #2 
    /*000012ac*/ MOVW        R1, #12624                  @ 0x3150 
    /*000012b0*/ BL          mn34220_write_register
    /*000012b4*/ MOV         R0, R4 
    /*000012b8*/ MOV         R2, #1 
    /*000012bc*/ MOVW        R1, #12625                  @ 0x3151 
    /*000012c0*/ BL          mn34220_write_register
    /*000012c4*/ MOV         R0, R4 
    /*000012c8*/ MOV         R2, #1 
    /*000012cc*/ MOVW        R1, #12626                  @ 0x3152 
    /*000012d0*/ BL          mn34220_write_register
    /*000012d4*/ MOV         R0, R4 
    /*000012d8*/ MOV         R2, #227                    @ 0xe3 
    /*000012dc*/ MOVW        R1, #12627                  @ 0x3153 
    /*000012e0*/ BL          mn34220_write_register
    /*000012e4*/ MOV         R0, R4 
    /*000012e8*/ MOV         R2, #0 
    /*000012ec*/ MOVW        R1, #12628                  @ 0x3154 
    /*000012f0*/ BL          mn34220_write_register
    /*000012f4*/ MOV         R0, R4 
    /*000012f8*/ MOV         R2, #17 
    /*000012fc*/ MOVW        R1, #12629                  @ 0x3155 
    /*00001300*/ BL          mn34220_write_register
    /*00001304*/ MOV         R0, R4 
    /*00001308*/ MOV         R2, #0 
    /*0000130c*/ MOVW        R1, #12630                  @ 0x3156 
    /*00001310*/ BL          mn34220_write_register
    /*00001314*/ MOV         R0, R4 
    /*00001318*/ MOV         R2, #48                     @ 0x30 
    /*0000131c*/ MOVW        R1, #12631                  @ 0x3157 
    /*00001320*/ BL          mn34220_write_register
    /*00001324*/ MOV         R0, R4 
    /*00001328*/ MOV         R2, #0 
    /*0000132c*/ MOVW        R1, #12632                  @ 0x3158 
    /*00001330*/ BL          mn34220_write_register
    /*00001334*/ MOV         R0, R4 
    /*00001338*/ MOV         R2, #51                     @ 0x33 
    /*0000133c*/ MOVW        R1, #12633                  @ 0x3159 
    /*00001340*/ BL          mn34220_write_register
    /*00001344*/ MOV         R0, R4 
    /*00001348*/ MOV         R2, #0 
    /*0000134c*/ MOVW        R1, #12634                  @ 0x315a 
    /*00001350*/ BL          mn34220_write_register
    /*00001354*/ MOV         R0, R4 
    /*00001358*/ MOV         R2, #54                     @ 0x36 
    /*0000135c*/ MOVW        R1, #12635                  @ 0x315b 
    /*00001360*/ BL          mn34220_write_register
    /*00001364*/ MOV         R0, R4 
    /*00001368*/ MOV         R2, #0 
    /*0000136c*/ MOVW        R1, #12636                  @ 0x315c 
    /*00001370*/ BL          mn34220_write_register
    /*00001374*/ MOV         R0, R4 
    /*00001378*/ MOV         R2, #53                     @ 0x35 
    /*0000137c*/ MOVW        R1, #12637                  @ 0x315d 
    /*00001380*/ BL          mn34220_write_register
    /*00001384*/ MOV         R0, R4 
    /*00001388*/ MOV         R2, #0 
    /*0000138c*/ MOVW        R1, #12638                  @ 0x315e 
    /*00001390*/ BL          mn34220_write_register
    /*00001394*/ MOV         R0, R4 
    /*00001398*/ MOV         R2, #60                     @ 0x3c 
    /*0000139c*/ MOVW        R1, #12639                  @ 0x315f 
    /*000013a0*/ BL          mn34220_write_register
    /*000013a4*/ MOV         R0, R4 
    /*000013a8*/ MOV         R2, #0 
    /*000013ac*/ MOVW        R1, #12640                  @ 0x3160 
    /*000013b0*/ BL          mn34220_write_register
    /*000013b4*/ MOV         R0, R4 
    /*000013b8*/ MOV         R2, #63                     @ 0x3f 
    /*000013bc*/ MOVW        R1, #12641                  @ 0x3161 
    /*000013c0*/ BL          mn34220_write_register
    /*000013c4*/ MOV         R0, R4 
    /*000013c8*/ MOV         R2, #0 
    /*000013cc*/ MOVW        R1, #12642                  @ 0x3162 
    /*000013d0*/ BL          mn34220_write_register
    /*000013d4*/ MOV         R0, R4 
    /*000013d8*/ MOV         R2, #58                     @ 0x3a 
    /*000013dc*/ MOVW        R1, #12643                  @ 0x3163 
    /*000013e0*/ BL          mn34220_write_register
    /*000013e4*/ MOV         R0, R4 
    /*000013e8*/ MOV         R2, #0 
    /*000013ec*/ MOVW        R1, #12644                  @ 0x3164 
    /*000013f0*/ BL          mn34220_write_register
    /*000013f4*/ MOV         R0, R4 
    /*000013f8*/ MOV         R2, #57                     @ 0x39 
    /*000013fc*/ MOVW        R1, #12645                  @ 0x3165 
    /*00001400*/ BL          mn34220_write_register
    /*00001404*/ MOV         R0, R4 
    /*00001408*/ MOV         R2, #0 
    /*0000140c*/ MOVW        R1, #12646                  @ 0x3166 
    /*00001410*/ BL          mn34220_write_register
    /*00001414*/ MOV         R0, R4 
    /*00001418*/ MOV         R2, #40                     @ 0x28 
    /*0000141c*/ MOVW        R1, #12647                  @ 0x3167 
    /*00001420*/ BL          mn34220_write_register
    /*00001424*/ MOV         R0, R4 
    /*00001428*/ MOV         R2, #0 
    /*0000142c*/ MOVW        R1, #12648                  @ 0x3168 
    /*00001430*/ BL          mn34220_write_register
    /*00001434*/ MOV         R0, R4 
    /*00001438*/ MOV         R2, #43                     @ 0x2b 
    /*0000143c*/ MOVW        R1, #12649                  @ 0x3169 
    /*00001440*/ BL          mn34220_write_register
    /*00001444*/ MOV         R0, R4 
    /*00001448*/ MOV         R2, #0 
    /*0000144c*/ MOVW        R1, #12650                  @ 0x316a 
    /*00001450*/ BL          mn34220_write_register
    /*00001454*/ MOV         R0, R4 
    /*00001458*/ MOV         R2, #46                     @ 0x2e 
    /*0000145c*/ MOVW        R1, #12651                  @ 0x316b 
    /*00001460*/ BL          mn34220_write_register
    /*00001464*/ MOV         R0, R4 
    /*00001468*/ MOV         R2, #0 
    /*0000146c*/ MOVW        R1, #12652                  @ 0x316c 
    /*00001470*/ BL          mn34220_write_register
    /*00001474*/ MOV         R0, R4 
    /*00001478*/ MOV         R2, #45                     @ 0x2d 
    /*0000147c*/ MOVW        R1, #12653                  @ 0x316d 
    /*00001480*/ BL          mn34220_write_register
    /*00001484*/ MOV         R0, R4 
    /*00001488*/ MOV         R2, #0 
    /*0000148c*/ MOVW        R1, #12654                  @ 0x316e 
    /*00001490*/ BL          mn34220_write_register
    /*00001494*/ MOV         R0, R4 
    /*00001498*/ MOV         R2, #34                     @ 0x22 
    /*0000149c*/ MOVW        R1, #12655                  @ 0x316f 
    /*000014a0*/ BL          mn34220_write_register
    /*000014a4*/ MOV         R0, R4 
    /*000014a8*/ MOV         R2, #0 
    /*000014ac*/ MOVW        R1, #12656                  @ 0x3170 
    /*000014b0*/ BL          mn34220_write_register
    /*000014b4*/ MOV         R0, R4 
    /*000014b8*/ MOV         R2, #34                     @ 0x22 
    /*000014bc*/ MOVW        R1, #12657                  @ 0x3171 
    /*000014c0*/ BL          mn34220_write_register
    /*000014c4*/ MOV         R0, R4 
    /*000014c8*/ MOV         R2, #0 
    /*000014cc*/ MOVW        R1, #12658                  @ 0x3172 
    /*000014d0*/ BL          mn34220_write_register
    /*000014d4*/ MOV         R0, R4 
    /*000014d8*/ MOV         R2, #97                     @ 0x61 
    /*000014dc*/ MOVW        R1, #12659                  @ 0x3173 
    /*000014e0*/ BL          mn34220_write_register
    /*000014e4*/ MOV         R0, R4 
    /*000014e8*/ MOV         R2, #11 
    /*000014ec*/ MOVW        R1, #12660                  @ 0x3174 
    /*000014f0*/ BL          mn34220_write_register
    /*000014f4*/ MOV         R0, R4 
    /*000014f8*/ MOV         R2, #128                    @ 0x80 
    /*000014fc*/ MOVW        R1, #12661                  @ 0x3175 
    /*00001500*/ BL          mn34220_write_register
    /*00001504*/ MOV         R0, R4 
    /*00001508*/ MOV         R2, #136                    @ 0x88 
    /*0000150c*/ MOVW        R1, #12662                  @ 0x3176 
    /*00001510*/ BL          mn34220_write_register
    /*00001514*/ MOV         R0, R4 
    /*00001518*/ MOV         R2, #160                    @ 0xa0 
    /*0000151c*/ MOVW        R1, #12663                  @ 0x3177 
    /*00001520*/ BL          mn34220_write_register
    /*00001524*/ MOV         R0, R4 
    /*00001528*/ MOV         R2, #8 
    /*0000152c*/ MOVW        R1, #12664                  @ 0x3178 
    /*00001530*/ BL          mn34220_write_register
    /*00001534*/ MOV         R0, R4 
    /*00001538*/ MOV         R2, #0 
    /*0000153c*/ MOVW        R1, #12665                  @ 0x3179 
    /*00001540*/ BL          mn34220_write_register
    /*00001544*/ MOV         R0, R4 
    /*00001548*/ MOV         R2, #160                    @ 0xa0 
    /*0000154c*/ MOVW        R1, #12666                  @ 0x317a 
    /*00001550*/ BL          mn34220_write_register
    /*00001554*/ MOV         R0, R4 
    /*00001558*/ MOV         R2, #162                    @ 0xa2 
    /*0000155c*/ MOVW        R1, #12667                  @ 0x317b 
    /*00001560*/ BL          mn34220_write_register
    /*00001564*/ MOV         R0, R4 
    /*00001568*/ MOV         R2, #160                    @ 0xa0 
    /*0000156c*/ MOVW        R1, #12668                  @ 0x317c 
    /*00001570*/ BL          mn34220_write_register
    /*00001574*/ MOV         R0, R4 
    /*00001578*/ MOV         R2, #2 
    /*0000157c*/ MOVW        R1, #12669                  @ 0x317d 
    /*00001580*/ BL          mn34220_write_register
    /*00001584*/ MOV         R0, R4 
    /*00001588*/ MOV         R2, #0 
    /*0000158c*/ MOVW        R1, #12670                  @ 0x317e 
    /*00001590*/ BL          mn34220_write_register
    /*00001594*/ MOV         R0, R4 
    /*00001598*/ MOV         R2, #0 
    /*0000159c*/ MOVW        R1, #12671                  @ 0x317f 
    /*000015a0*/ BL          mn34220_write_register
    /*000015a4*/ MOV         R0, R4 
    /*000015a8*/ MOV         R2, #0 
    /*000015ac*/ MOV         R1, #12672                  @ 0x3180 
    /*000015b0*/ BL          mn34220_write_register
    /*000015b4*/ MOV         R0, R4 
    /*000015b8*/ MOV         R2, #130                    @ 0x82 
    /*000015bc*/ MOVW        R1, #12673                  @ 0x3181 
    /*000015c0*/ BL          mn34220_write_register
    /*000015c4*/ MOV         R0, R4 
    /*000015c8*/ MOV         R2, #32 
    /*000015cc*/ MOVW        R1, #12674                  @ 0x3182 
    /*000015d0*/ BL          mn34220_write_register
    /*000015d4*/ MOV         R0, R4 
    /*000015d8*/ MOV         R2, #32 
    /*000015dc*/ MOVW        R1, #12675                  @ 0x3183 
    /*000015e0*/ BL          mn34220_write_register
    /*000015e4*/ MOV         R0, R4 
    /*000015e8*/ MOV         R2, #2 
    /*000015ec*/ MOVW        R1, #12676                  @ 0x3184 
    /*000015f0*/ BL          mn34220_write_register
    /*000015f4*/ MOV         R0, R4 
    /*000015f8*/ MOV         R2, #0 
    /*000015fc*/ MOVW        R1, #12677                  @ 0x3185 
    /*00001600*/ BL          mn34220_write_register
    /*00001604*/ MOV         R0, R4 
    /*00001608*/ MOV         R2, #72                     @ 0x48 
    /*0000160c*/ MOVW        R1, #12678                  @ 0x3186 
    /*00001610*/ BL          mn34220_write_register
    /*00001614*/ MOV         R0, R4 
    /*00001618*/ MOV         R2, #16 
    /*0000161c*/ MOVW        R1, #12679                  @ 0x3187 
    /*00001620*/ BL          mn34220_write_register
    /*00001624*/ MOV         R0, R4 
    /*00001628*/ MOV         R2, #0 
    /*0000162c*/ MOVW        R1, #12680                  @ 0x3188 
    /*00001630*/ BL          mn34220_write_register
    /*00001634*/ MOV         R0, R4 
    /*00001638*/ MOV         R2, #222                    @ 0xde 
    /*0000163c*/ MOVW        R1, #12681                  @ 0x3189 
    /*00001640*/ BL          mn34220_write_register
    /*00001644*/ MOV         R0, R4 
    /*00001648*/ MOV         R2, #107                    @ 0x6b 
    /*0000164c*/ MOVW        R1, #12682                  @ 0x318a 
    /*00001650*/ BL          mn34220_write_register
    /*00001654*/ MOV         R0, R4 
    /*00001658*/ MOV         R2, #15 
    /*0000165c*/ MOVW        R1, #12683                  @ 0x318b 
    /*00001660*/ BL          mn34220_write_register
    /*00001664*/ MOV         R0, R4 
    /*00001668*/ MOV         R2, #31 
    /*0000166c*/ MOVW        R1, #12684                  @ 0x318c 
    /*00001670*/ BL          mn34220_write_register
    /*00001674*/ MOV         R0, R4 
    /*00001678*/ MOV         R2, #0 
    /*0000167c*/ MOVW        R1, #12685                  @ 0x318d 
    /*00001680*/ BL          mn34220_write_register
    /*00001684*/ MOV         R0, R4 
    /*00001688*/ MOV         R2, #32 
    /*0000168c*/ MOVW        R1, #12686                  @ 0x318e 
    /*00001690*/ BL          mn34220_write_register
    /*00001694*/ MOV         R0, R4 
    /*00001698*/ MOV         R2, #112                    @ 0x70 
    /*0000169c*/ MOVW        R1, #12687                  @ 0x318f 
    /*000016a0*/ BL          mn34220_write_register
    /*000016a4*/ MOV         R0, R4 
    /*000016a8*/ MOV         R2, #0 
    /*000016ac*/ MOVW        R1, #12688                  @ 0x3190 
    /*000016b0*/ BL          mn34220_write_register
    /*000016b4*/ MOV         R0, R4 
    /*000016b8*/ MOV         R2, #0 
    /*000016bc*/ MOVW        R1, #12689                  @ 0x3191 
    /*000016c0*/ BL          mn34220_write_register
    /*000016c4*/ MOV         R0, R4 
    /*000016c8*/ MOV         R2, #0 
    /*000016cc*/ MOVW        R1, #12690                  @ 0x3192 
    /*000016d0*/ BL          mn34220_write_register
    /*000016d4*/ MOV         R0, R4 
    /*000016d8*/ MOV         R2, #0 
    /*000016dc*/ MOVW        R1, #12691                  @ 0x3193 
    /*000016e0*/ BL          mn34220_write_register
    /*000016e4*/ MOV         R0, R4 
    /*000016e8*/ MOV         R2, #0 
    /*000016ec*/ MOVW        R1, #12692                  @ 0x3194 
    /*000016f0*/ BL          mn34220_write_register
    /*000016f4*/ MOV         R0, R4 
    /*000016f8*/ MOV         R2, #64                     @ 0x40 
    /*000016fc*/ MOVW        R1, #12693                  @ 0x3195 
    /*00001700*/ BL          mn34220_write_register
    /*00001704*/ MOV         R0, R4 
    /*00001708*/ MOV         R2, #8 
    /*0000170c*/ MOVW        R1, #12694                  @ 0x3196 
    /*00001710*/ BL          mn34220_write_register
    /*00001714*/ MOV         R0, R4 
    /*00001718*/ MOV         R2, #6 
    /*0000171c*/ MOVW        R1, #12695                  @ 0x3197 
    /*00001720*/ BL          mn34220_write_register
    /*00001724*/ MOV         R0, R4 
    /*00001728*/ MOV         R2, #0 
    /*0000172c*/ MOVW        R1, #12696                  @ 0x3198 
    /*00001730*/ BL          mn34220_write_register
    /*00001734*/ MOV         R0, R4 
    /*00001738*/ MOV         R2, #0 
    /*0000173c*/ MOVW        R1, #12697                  @ 0x3199 
    /*00001740*/ BL          mn34220_write_register
    /*00001744*/ MOV         R0, R4 
    /*00001748*/ MOV         R2, #14 
    /*0000174c*/ MOVW        R1, #12698                  @ 0x319a 
    /*00001750*/ BL          mn34220_write_register
    /*00001754*/ MOV         R0, R4 
    /*00001758*/ MOV         R2, #16 
    /*0000175c*/ MOVW        R1, #12699                  @ 0x319b 
    /*00001760*/ BL          mn34220_write_register
    /*00001764*/ MOV         R0, R4 
    /*00001768*/ MOV         R2, #50                     @ 0x32 
    /*0000176c*/ MOVW        R1, #12700                  @ 0x319c 
    /*00001770*/ BL          mn34220_write_register
    /*00001774*/ MOV         R0, R4 
    /*00001778*/ MOV         R2, #84                     @ 0x54 
    /*0000177c*/ MOVW        R1, #12701                  @ 0x319d 
    /*00001780*/ BL          mn34220_write_register
    /*00001784*/ MOV         R0, R4 
    /*00001788*/ MOV         R2, #118                    @ 0x76 
    /*0000178c*/ MOVW        R1, #12702                  @ 0x319e 
    /*00001790*/ BL          mn34220_write_register
    /*00001794*/ MOV         R0, R4 
    /*00001798*/ MOV         R2, #152                    @ 0x98 
    /*0000179c*/ MOVW        R1, #12703                  @ 0x319f 
    /*000017a0*/ BL          mn34220_write_register
    /*000017a4*/ MOV         R0, R4 
    /*000017a8*/ MOV         R2, #0 
    /*000017ac*/ MOVW        R1, #12704                  @ 0x31a0 
    /*000017b0*/ BL          mn34220_write_register
    /*000017b4*/ MOV         R0, R4 
    /*000017b8*/ MOV         R2, #0 
    /*000017bc*/ MOVW        R1, #12705                  @ 0x31a1 
    /*000017c0*/ BL          mn34220_write_register
    /*000017c4*/ MOV         R0, R4 
    /*000017c8*/ MOV         R2, #0 
    /*000017cc*/ MOVW        R1, #12706                  @ 0x31a2 
    /*000017d0*/ BL          mn34220_write_register
    /*000017d4*/ MOV         R0, R4 
    /*000017d8*/ MOV         R2, #16 
    /*000017dc*/ MOVW        R1, #12707                  @ 0x31a3 
    /*000017e0*/ BL          mn34220_write_register
    /*000017e4*/ MOV         R0, R4 
    /*000017e8*/ MOV         R2, #50                     @ 0x32 
    /*000017ec*/ MOVW        R1, #12708                  @ 0x31a4 
    /*000017f0*/ BL          mn34220_write_register
    /*000017f4*/ MOV         R0, R4 
    /*000017f8*/ MOV         R2, #84                     @ 0x54 
    /*000017fc*/ MOVW        R1, #12709                  @ 0x31a5 
    /*00001800*/ BL          mn34220_write_register
    /*00001804*/ MOV         R0, R4 
    /*00001808*/ MOV         R2, #134                    @ 0x86 
    /*0000180c*/ MOVW        R1, #12710                  @ 0x31a6 
    /*00001810*/ BL          mn34220_write_register
    /*00001814*/ MOV         R0, R4 
    /*00001818*/ MOV         R2, #9 
    /*0000181c*/ MOVW        R1, #12711                  @ 0x31a7 
    /*00001820*/ BL          mn34220_write_register
    /*00001824*/ MOV         R0, R4 
    /*00001828*/ MOV         R2, #0 
    /*0000182c*/ MOVW        R1, #12712                  @ 0x31a8 
    /*00001830*/ BL          mn34220_write_register
    /*00001834*/ MOV         R0, R4 
    /*00001838*/ MOV         R2, #0 
    /*0000183c*/ MOVW        R1, #12713                  @ 0x31a9 
    /*00001840*/ BL          mn34220_write_register
    /*00001844*/ MOV         R0, R4 
    /*00001848*/ MOV         R2, #0 
    /*0000184c*/ MOVW        R1, #12714                  @ 0x31aa 
    /*00001850*/ BL          mn34220_write_register
    /*00001854*/ MOV         R0, R4 
    /*00001858*/ MOV         R2, #36                     @ 0x24 
    /*0000185c*/ MOVW        R1, #12715                  @ 0x31ab 
    /*00001860*/ BL          mn34220_write_register
    /*00001864*/ MOV         R0, R4 
    /*00001868*/ MOV         R2, #0 
    /*0000186c*/ MOVW        R1, #12716                  @ 0x31ac 
    /*00001870*/ BL          mn34220_write_register
    /*00001874*/ MOV         R0, R4 
    /*00001878*/ MOV         R2, #137                    @ 0x89 
    /*0000187c*/ MOVW        R1, #12717                  @ 0x31ad 
    /*00001880*/ BL          mn34220_write_register
    /*00001884*/ MOV         R0, R4 
    /*00001888*/ MOV         R2, #2 
    /*0000188c*/ MOVW        R1, #12718                  @ 0x31ae 
    /*00001890*/ BL          mn34220_write_register
    /*00001894*/ MOV         R0, R4 
    /*00001898*/ MOV         R2, #0 
    /*0000189c*/ MOVW        R1, #12719                  @ 0x31af 
    /*000018a0*/ BL          mn34220_write_register
    /*000018a4*/ MOV         R0, R4 
    /*000018a8*/ MOV         R2, #0 
    /*000018ac*/ MOVW        R1, #12720                  @ 0x31b0 
    /*000018b0*/ BL          mn34220_write_register
    /*000018b4*/ MOV         R0, R4 
    /*000018b8*/ MOV         R2, #0 
    /*000018bc*/ MOVW        R1, #12721                  @ 0x31b1 
    /*000018c0*/ BL          mn34220_write_register
    /*000018c4*/ MOV         R0, R4 
    /*000018c8*/ MOV         R2, #0 
    /*000018cc*/ MOVW        R1, #12722                  @ 0x31b2 
    /*000018d0*/ BL          mn34220_write_register
    /*000018d4*/ MOV         R0, R4 
    /*000018d8*/ MOV         R2, #0 
    /*000018dc*/ MOVW        R1, #12723                  @ 0x31b3 
    /*000018e0*/ BL          mn34220_write_register
    /*000018e4*/ MOV         R0, R4 
    /*000018e8*/ MOV         R2, #0 
    /*000018ec*/ MOVW        R1, #12724                  @ 0x31b4 
    /*000018f0*/ BL          mn34220_write_register
    /*000018f4*/ MOV         R0, R4 
    /*000018f8*/ MOV         R2, #0 
    /*000018fc*/ MOVW        R1, #12725                  @ 0x31b5 
    /*00001900*/ BL          mn34220_write_register
    /*00001904*/ MOV         R0, R4 
    /*00001908*/ MOV         R2, #0 
    /*0000190c*/ MOVW        R1, #12726                  @ 0x31b6 
    /*00001910*/ BL          mn34220_write_register
    /*00001914*/ MOV         R0, R4 
    /*00001918*/ MOV         R2, #0 
    /*0000191c*/ MOVW        R1, #12727                  @ 0x31b7 
    /*00001920*/ BL          mn34220_write_register
    /*00001924*/ MOV         R0, R4 
    /*00001928*/ MOV         R2, #0 
    /*0000192c*/ MOVW        R1, #12728                  @ 0x31b8 
    /*00001930*/ BL          mn34220_write_register
    /*00001934*/ MOV         R0, R4 
    /*00001938*/ MOV         R2, #0 
    /*0000193c*/ MOVW        R1, #12729                  @ 0x31b9 
    /*00001940*/ BL          mn34220_write_register
    /*00001944*/ MOV         R0, R4 
    /*00001948*/ MOV         R2, #0 
    /*0000194c*/ MOVW        R1, #12730                  @ 0x31ba 
    /*00001950*/ BL          mn34220_write_register
    /*00001954*/ MOV         R0, R4 
    /*00001958*/ MOV         R2, #0 
    /*0000195c*/ MOVW        R1, #12731                  @ 0x31bb 
    /*00001960*/ BL          mn34220_write_register
    /*00001964*/ MOV         R0, R4 
    /*00001968*/ MOV         R2, #0 
    /*0000196c*/ MOVW        R1, #12732                  @ 0x31bc 
    /*00001970*/ BL          mn34220_write_register
    /*00001974*/ MOV         R0, R4 
    /*00001978*/ MOV         R2, #0 
    /*0000197c*/ MOVW        R1, #12733                  @ 0x31bd 
    /*00001980*/ BL          mn34220_write_register
    /*00001984*/ MOV         R0, R4 
    /*00001988*/ MOV         R2, #0 
    /*0000198c*/ MOVW        R1, #12734                  @ 0x31be 
    /*00001990*/ BL          mn34220_write_register
    /*00001994*/ MOV         R0, R4 
    /*00001998*/ MOV         R2, #0 
    /*0000199c*/ MOVW        R1, #12735                  @ 0x31bf 
    /*000019a0*/ BL          mn34220_write_register
    /*000019a4*/ MOV         R0, R4 
    /*000019a8*/ MOV         R2, #0 
    /*000019ac*/ MOV         R1, #12736                  @ 0x31c0 
    /*000019b0*/ BL          mn34220_write_register
    /*000019b4*/ MOV         R0, R4 
    /*000019b8*/ MOV         R2, #0 
    /*000019bc*/ MOVW        R1, #12737                  @ 0x31c1 
    /*000019c0*/ BL          mn34220_write_register
    /*000019c4*/ MOV         R0, R4 
    /*000019c8*/ MOV         R2, #0 
    /*000019cc*/ MOVW        R1, #12738                  @ 0x31c2 
    /*000019d0*/ BL          mn34220_write_register
    /*000019d4*/ MOV         R0, R4 
    /*000019d8*/ MOV         R2, #0 
    /*000019dc*/ MOVW        R1, #12739                  @ 0x31c3 
    /*000019e0*/ BL          mn34220_write_register
    /*000019e4*/ MOV         R0, R4 
    /*000019e8*/ MOV         R2, #0 
    /*000019ec*/ MOVW        R1, #12740                  @ 0x31c4 
    /*000019f0*/ BL          mn34220_write_register
    /*000019f4*/ MOV         R0, R4 
    /*000019f8*/ MOV         R2, #0 
    /*000019fc*/ MOVW        R1, #12741                  @ 0x31c5 
    /*00001a00*/ BL          mn34220_write_register
    /*00001a04*/ MOV         R0, R4 
    /*00001a08*/ MOV         R2, #0 
    /*00001a0c*/ MOVW        R1, #12742                  @ 0x31c6 
    /*00001a10*/ BL          mn34220_write_register
    /*00001a14*/ MOV         R0, R4 
    /*00001a18*/ MOV         R2, #0 
    /*00001a1c*/ MOVW        R1, #12743                  @ 0x31c7 
    /*00001a20*/ BL          mn34220_write_register
    /*00001a24*/ MOV         R0, R4 
    /*00001a28*/ MOV         R2, #0 
    /*00001a2c*/ MOVW        R1, #12744                  @ 0x31c8 
    /*00001a30*/ BL          mn34220_write_register
    /*00001a34*/ MOV         R0, R4 
    /*00001a38*/ MOV         R2, #0 
    /*00001a3c*/ MOVW        R1, #12745                  @ 0x31c9 
    /*00001a40*/ BL          mn34220_write_register
    /*00001a44*/ MOV         R0, R4 
    /*00001a48*/ MOV         R2, #0 
    /*00001a4c*/ MOVW        R1, #12746                  @ 0x31ca 
    /*00001a50*/ BL          mn34220_write_register
    /*00001a54*/ MOV         R0, R4 
    /*00001a58*/ MOV         R2, #0 
    /*00001a5c*/ MOVW        R1, #12747                  @ 0x31cb 
    /*00001a60*/ BL          mn34220_write_register
    /*00001a64*/ MOV         R0, R4 
    /*00001a68*/ MOV         R2, #0 
    /*00001a6c*/ MOVW        R1, #12748                  @ 0x31cc 
    /*00001a70*/ BL          mn34220_write_register
    /*00001a74*/ MOV         R0, R4 
    /*00001a78*/ MOV         R2, #0 
    /*00001a7c*/ MOVW        R1, #12749                  @ 0x31cd 
    /*00001a80*/ BL          mn34220_write_register
    /*00001a84*/ MOV         R0, R4 
    /*00001a88*/ MOV         R2, #0 
    /*00001a8c*/ MOVW        R1, #12750                  @ 0x31ce 
    /*00001a90*/ BL          mn34220_write_register
    /*00001a94*/ MOV         R0, R4 
    /*00001a98*/ MOV         R2, #0 
    /*00001a9c*/ MOVW        R1, #12751                  @ 0x31cf 
    /*00001aa0*/ BL          mn34220_write_register
    /*00001aa4*/ MOV         R0, R4 
    /*00001aa8*/ MOV         R2, #0 
    /*00001aac*/ MOVW        R1, #12752                  @ 0x31d0 
    /*00001ab0*/ BL          mn34220_write_register
    /*00001ab4*/ MOV         R0, R4 
    /*00001ab8*/ MOV         R2, #0 
    /*00001abc*/ MOVW        R1, #12753                  @ 0x31d1 
    /*00001ac0*/ BL          mn34220_write_register
    /*00001ac4*/ MOV         R0, R4 
    /*00001ac8*/ MOV         R2, #0 
    /*00001acc*/ MOVW        R1, #12754                  @ 0x31d2 
    /*00001ad0*/ BL          mn34220_write_register
    /*00001ad4*/ MOV         R0, R4 
    /*00001ad8*/ MOV         R2, #0 
    /*00001adc*/ MOVW        R1, #12755                  @ 0x31d3 
    /*00001ae0*/ BL          mn34220_write_register
    /*00001ae4*/ MOV         R0, R4 
    /*00001ae8*/ MOV         R2, #0 
    /*00001aec*/ MOVW        R1, #12756                  @ 0x31d4 
    /*00001af0*/ BL          mn34220_write_register
    /*00001af4*/ MOV         R0, R4 
    /*00001af8*/ MOV         R2, #0 
    /*00001afc*/ MOVW        R1, #12757                  @ 0x31d5 
    /*00001b00*/ BL          mn34220_write_register
    /*00001b04*/ MOV         R0, R4 
    /*00001b08*/ MOV         R2, #0 
    /*00001b0c*/ MOVW        R1, #12758                  @ 0x31d6 
    /*00001b10*/ BL          mn34220_write_register
    /*00001b14*/ MOV         R0, R4 
    /*00001b18*/ MOV         R2, #0 
    /*00001b1c*/ MOVW        R1, #12759                  @ 0x31d7 
    /*00001b20*/ BL          mn34220_write_register
    /*00001b24*/ MOV         R0, R4 
    /*00001b28*/ MOV         R2, #0 
    /*00001b2c*/ MOVW        R1, #12760                  @ 0x31d8 
    /*00001b30*/ BL          mn34220_write_register
    /*00001b34*/ MOV         R0, R4 
    /*00001b38*/ MOV         R2, #0 
    /*00001b3c*/ MOVW        R1, #12761                  @ 0x31d9 
    /*00001b40*/ BL          mn34220_write_register
    /*00001b44*/ MOV         R0, R4 
    /*00001b48*/ MOV         R2, #0 
    /*00001b4c*/ MOVW        R1, #12762                  @ 0x31da 
    /*00001b50*/ BL          mn34220_write_register
    /*00001b54*/ MOV         R0, R4 
    /*00001b58*/ MOV         R2, #0 
    /*00001b5c*/ MOVW        R1, #12763                  @ 0x31db 
    /*00001b60*/ BL          mn34220_write_register
    /*00001b64*/ MOV         R0, R4 
    /*00001b68*/ MOV         R2, #0 
    /*00001b6c*/ MOVW        R1, #12764                  @ 0x31dc 
    /*00001b70*/ BL          mn34220_write_register
    /*00001b74*/ MOV         R0, R4 
    /*00001b78*/ MOV         R2, #0 
    /*00001b7c*/ MOVW        R1, #12765                  @ 0x31dd 
    /*00001b80*/ BL          mn34220_write_register
    /*00001b84*/ MOV         R0, R4 
    /*00001b88*/ MOV         R2, #0 
    /*00001b8c*/ MOVW        R1, #12766                  @ 0x31de 
    /*00001b90*/ BL          mn34220_write_register
    /*00001b94*/ MOV         R0, R4 
    /*00001b98*/ MOV         R2, #0 
    /*00001b9c*/ MOVW        R1, #12767                  @ 0x31df 
    /*00001ba0*/ BL          mn34220_write_register
    /*00001ba4*/ MOV         R0, R4 
    /*00001ba8*/ MOV         R2, #0 
    /*00001bac*/ MOVW        R1, #12768                  @ 0x31e0 
    /*00001bb0*/ BL          mn34220_write_register
    /*00001bb4*/ MOV         R0, R4 
    /*00001bb8*/ MOV         R2, #0 
    /*00001bbc*/ MOVW        R1, #12769                  @ 0x31e1 
    /*00001bc0*/ BL          mn34220_write_register
    /*00001bc4*/ MOV         R0, R4 
    /*00001bc8*/ MOV         R2, #0 
    /*00001bcc*/ MOVW        R1, #12770                  @ 0x31e2 
    /*00001bd0*/ BL          mn34220_write_register
    /*00001bd4*/ MOV         R0, R4 
    /*00001bd8*/ MOV         R2, #0 
    /*00001bdc*/ MOVW        R1, #12771                  @ 0x31e3 
    /*00001be0*/ BL          mn34220_write_register
    /*00001be4*/ MOV         R0, R4 
    /*00001be8*/ MOV         R2, #0 
    /*00001bec*/ MOVW        R1, #12772                  @ 0x31e4 
    /*00001bf0*/ BL          mn34220_write_register
    /*00001bf4*/ MOV         R0, R4 
    /*00001bf8*/ MOV         R2, #0 
    /*00001bfc*/ MOVW        R1, #12773                  @ 0x31e5 
    /*00001c00*/ BL          mn34220_write_register
    /*00001c04*/ MOV         R0, R4 
    /*00001c08*/ MOV         R2, #0 
    /*00001c0c*/ MOVW        R1, #12774                  @ 0x31e6 
    /*00001c10*/ BL          mn34220_write_register
    /*00001c14*/ MOV         R0, R4 
    /*00001c18*/ MOV         R2, #0 
    /*00001c1c*/ MOVW        R1, #12775                  @ 0x31e7 
    /*00001c20*/ BL          mn34220_write_register
    /*00001c24*/ MOV         R0, R4 
    /*00001c28*/ MOV         R2, #0 
    /*00001c2c*/ MOVW        R1, #12776                  @ 0x31e8 
    /*00001c30*/ BL          mn34220_write_register
    /*00001c34*/ MOV         R0, R4 
    /*00001c38*/ MOV         R2, #0 
    /*00001c3c*/ MOVW        R1, #12777                  @ 0x31e9 
    /*00001c40*/ BL          mn34220_write_register
    /*00001c44*/ MOV         R0, R4 
    /*00001c48*/ MOV         R2, #0 
    /*00001c4c*/ MOVW        R1, #12778                  @ 0x31ea 
    /*00001c50*/ BL          mn34220_write_register
    /*00001c54*/ MOV         R0, R4 
    /*00001c58*/ MOV         R2, #0 
    /*00001c5c*/ MOVW        R1, #12779                  @ 0x31eb 
    /*00001c60*/ BL          mn34220_write_register
    /*00001c64*/ MOV         R0, R4 
    /*00001c68*/ MOV         R2, #0 
    /*00001c6c*/ MOVW        R1, #12780                  @ 0x31ec 
    /*00001c70*/ BL          mn34220_write_register
    /*00001c74*/ MOV         R0, R4 
    /*00001c78*/ MOV         R2, #0 
    /*00001c7c*/ MOVW        R1, #12781                  @ 0x31ed 
    /*00001c80*/ BL          mn34220_write_register
    /*00001c84*/ MOV         R0, R4 
    /*00001c88*/ MOV         R2, #0 
    /*00001c8c*/ MOVW        R1, #12782                  @ 0x31ee 
    /*00001c90*/ BL          mn34220_write_register
    /*00001c94*/ MOV         R0, R4 
    /*00001c98*/ MOV         R2, #0 
    /*00001c9c*/ MOVW        R1, #12783                  @ 0x31ef 
    /*00001ca0*/ BL          mn34220_write_register
    /*00001ca4*/ MOV         R0, R4 
    /*00001ca8*/ MOV         R2, #0 
    /*00001cac*/ MOVW        R1, #12784                  @ 0x31f0 
    /*00001cb0*/ BL          mn34220_write_register
    /*00001cb4*/ MOV         R0, R4 
    /*00001cb8*/ MOV         R2, #0 
    /*00001cbc*/ MOVW        R1, #12785                  @ 0x31f1 
    /*00001cc0*/ BL          mn34220_write_register
    /*00001cc4*/ MOV         R0, R4 
    /*00001cc8*/ MOV         R2, #0 
    /*00001ccc*/ MOVW        R1, #12786                  @ 0x31f2 
    /*00001cd0*/ BL          mn34220_write_register
    /*00001cd4*/ MOV         R0, R4 
    /*00001cd8*/ MOV         R2, #0 
    /*00001cdc*/ MOVW        R1, #12787                  @ 0x31f3 
    /*00001ce0*/ BL          mn34220_write_register
    /*00001ce4*/ MOV         R0, R4 
    /*00001ce8*/ MOV         R2, #0 
    /*00001cec*/ MOVW        R1, #12788                  @ 0x31f4 
    /*00001cf0*/ BL          mn34220_write_register
    /*00001cf4*/ MOV         R0, R4 
    /*00001cf8*/ MOV         R2, #0 
    /*00001cfc*/ MOVW        R1, #12789                  @ 0x31f5 
    /*00001d00*/ BL          mn34220_write_register
    /*00001d04*/ MOV         R0, R4 
    /*00001d08*/ MOV         R2, #0 
    /*00001d0c*/ MOVW        R1, #12790                  @ 0x31f6 
    /*00001d10*/ BL          mn34220_write_register
    /*00001d14*/ MOV         R0, R4 
    /*00001d18*/ MOV         R2, #3 
    /*00001d1c*/ MOVW        R1, #12791                  @ 0x31f7 
    /*00001d20*/ BL          mn34220_write_register
    /*00001d24*/ MOV         R0, R4 
    /*00001d28*/ MOV         R2, #3 
    /*00001d2c*/ MOVW        R1, #12792                  @ 0x31f8 
    /*00001d30*/ BL          mn34220_write_register
    /*00001d34*/ MOV         R0, R4 
    /*00001d38*/ MOV         R2, #3 
    /*00001d3c*/ MOVW        R1, #12793                  @ 0x31f9 
    /*00001d40*/ BL          mn34220_write_register
    /*00001d44*/ MOV         R0, R4 
    /*00001d48*/ MOV         R2, #6 
    /*00001d4c*/ MOVW        R1, #12794                  @ 0x31fa 
    /*00001d50*/ BL          mn34220_write_register
    /*00001d54*/ MOV         R0, R4 
    /*00001d58*/ MOV         R2, #8 
    /*00001d5c*/ MOVW        R1, #12795                  @ 0x31fb 
    /*00001d60*/ BL          mn34220_write_register
    /*00001d64*/ MOV         R0, R4 
    /*00001d68*/ MOV         R2, #3 
    /*00001d6c*/ MOVW        R1, #12796                  @ 0x31fc 
    /*00001d70*/ BL          mn34220_write_register
    /*00001d74*/ MOV         R0, R4 
    /*00001d78*/ MOV         R2, #8 
    /*00001d7c*/ MOVW        R1, #12797                  @ 0x31fd 
    /*00001d80*/ BL          mn34220_write_register
    /*00001d84*/ MOV         R0, R4 
    /*00001d88*/ MOV         R2, #6 
    /*00001d8c*/ MOVW        R1, #12798                  @ 0x31fe 
    /*00001d90*/ BL          mn34220_write_register
    /*00001d94*/ MOV         R0, R4 
    /*00001d98*/ MOV         R2, #0 
    /*00001d9c*/ MOVW        R1, #12799                  @ 0x31ff 
    /*00001da0*/ BL          mn34220_write_register
    /*00001da4*/ MOV         R0, R4 
    /*00001da8*/ MOV         R2, #0 
    /*00001dac*/ MOV         R1, #12800                  @ 0x3200 
    /*00001db0*/ BL          mn34220_write_register
    /*00001db4*/ MOV         R0, R4 
    /*00001db8*/ MOV         R2, #0 
    /*00001dbc*/ MOVW        R1, #12801                  @ 0x3201 
    /*00001dc0*/ BL          mn34220_write_register
    /*00001dc4*/ MOV         R0, R4 
    /*00001dc8*/ MOV         R2, #0 
    /*00001dcc*/ MOVW        R1, #12802                  @ 0x3202 
    /*00001dd0*/ BL          mn34220_write_register
    /*00001dd4*/ MOV         R0, R4 
    /*00001dd8*/ MOV         R2, #0 
    /*00001ddc*/ MOVW        R1, #12803                  @ 0x3203 
    /*00001de0*/ BL          mn34220_write_register
    /*00001de4*/ MOV         R0, R4 
    /*00001de8*/ MOV         R2, #0 
    /*00001dec*/ MOVW        R1, #12804                  @ 0x3204 
    /*00001df0*/ BL          mn34220_write_register
    /*00001df4*/ MOV         R0, R4 
    /*00001df8*/ MOV         R2, #0 
    /*00001dfc*/ MOVW        R1, #12805                  @ 0x3205 
    /*00001e00*/ BL          mn34220_write_register
    /*00001e04*/ MOV         R0, R4 
    /*00001e08*/ MOV         R2, #0 
    /*00001e0c*/ MOVW        R1, #12806                  @ 0x3206 
    /*00001e10*/ BL          mn34220_write_register
    /*00001e14*/ MOV         R0, R4 
    /*00001e18*/ MOV         R2, #0 
    /*00001e1c*/ MOVW        R1, #12807                  @ 0x3207 
    /*00001e20*/ BL          mn34220_write_register
    /*00001e24*/ MOV         R0, R4 
    /*00001e28*/ MOV         R2, #0 
    /*00001e2c*/ MOVW        R1, #12808                  @ 0x3208 
    /*00001e30*/ BL          mn34220_write_register
    /*00001e34*/ MOV         R0, R4 
    /*00001e38*/ MOV         R2, #0 
    /*00001e3c*/ MOVW        R1, #12809                  @ 0x3209 
    /*00001e40*/ BL          mn34220_write_register
    /*00001e44*/ MOV         R0, R4 
    /*00001e48*/ MOV         R2, #0 
    /*00001e4c*/ MOVW        R1, #12810                  @ 0x320a 
    /*00001e50*/ BL          mn34220_write_register
    /*00001e54*/ MOV         R0, R4 
    /*00001e58*/ MOV         R2, #9 
    /*00001e5c*/ MOVW        R1, #12811                  @ 0x320b 
    /*00001e60*/ BL          mn34220_write_register
    /*00001e64*/ MOV         R0, R4 
    /*00001e68*/ MOV         R2, #0 
    /*00001e6c*/ MOVW        R1, #12812                  @ 0x320c 
    /*00001e70*/ BL          mn34220_write_register
    /*00001e74*/ MOV         R0, R4 
    /*00001e78*/ MOV         R2, #26 
    /*00001e7c*/ MOVW        R1, #12813                  @ 0x320d 
    /*00001e80*/ BL          mn34220_write_register
    /*00001e84*/ MOV         R0, R4 
    /*00001e88*/ MOV         R2, #0 
    /*00001e8c*/ MOVW        R1, #12814                  @ 0x320e 
    /*00001e90*/ BL          mn34220_write_register
    /*00001e94*/ MOV         R0, R4 
    /*00001e98*/ MOV         R2, #27 
    /*00001e9c*/ MOVW        R1, #12815                  @ 0x320f 
    /*00001ea0*/ BL          mn34220_write_register
    /*00001ea4*/ MOV         R0, R4 
    /*00001ea8*/ MOV         R2, #0 
    /*00001eac*/ MOVW        R1, #12816                  @ 0x3210 
    /*00001eb0*/ BL          mn34220_write_register
    /*00001eb4*/ MOV         R0, R4 
    /*00001eb8*/ MOV         R2, #27 
    /*00001ebc*/ MOVW        R1, #12817                  @ 0x3211 
    /*00001ec0*/ BL          mn34220_write_register
    /*00001ec4*/ MOV         R0, R4 
    /*00001ec8*/ MOV         R2, #3 
    /*00001ecc*/ MOVW        R1, #12818                  @ 0x3212 
    /*00001ed0*/ BL          mn34220_write_register
    /*00001ed4*/ MOV         R0, R4 
    /*00001ed8*/ MOV         R2, #11 
    /*00001edc*/ MOVW        R1, #12819                  @ 0x3213 
    /*00001ee0*/ BL          mn34220_write_register
    /*00001ee4*/ MOV         R0, R4 
    /*00001ee8*/ MOV         R2, #0 
    /*00001eec*/ MOVW        R1, #12820                  @ 0x3214 
    /*00001ef0*/ BL          mn34220_write_register
    /*00001ef4*/ MOV         R0, R4 
    /*00001ef8*/ MOV         R2, #0 
    /*00001efc*/ MOVW        R1, #12821                  @ 0x3215 
    /*00001f00*/ BL          mn34220_write_register
    /*00001f04*/ MOV         R0, R4 
    /*00001f08*/ MOV         R2, #0 
    /*00001f0c*/ MOVW        R1, #12822                  @ 0x3216 
    /*00001f10*/ BL          mn34220_write_register
    /*00001f14*/ MOV         R0, R4 
    /*00001f18*/ MOV         R2, #0 
    /*00001f1c*/ MOVW        R1, #12823                  @ 0x3217 
    /*00001f20*/ BL          mn34220_write_register
    /*00001f24*/ MOV         R0, R4 
    /*00001f28*/ MOV         R2, #0 
    /*00001f2c*/ MOVW        R1, #12824                  @ 0x3218 
    /*00001f30*/ BL          mn34220_write_register
    /*00001f34*/ MOV         R0, R4 
    /*00001f38*/ MOV         R2, #0 
    /*00001f3c*/ MOVW        R1, #12825                  @ 0x3219 
    /*00001f40*/ BL          mn34220_write_register
    /*00001f44*/ MOV         R0, R4 
    /*00001f48*/ MOV         R2, #0 
    /*00001f4c*/ MOVW        R1, #12826                  @ 0x321a 
    /*00001f50*/ BL          mn34220_write_register
    /*00001f54*/ MOV         R0, R4 
    /*00001f58*/ MOV         R2, #0 
    /*00001f5c*/ MOVW        R1, #12827                  @ 0x321b 
    /*00001f60*/ BL          mn34220_write_register
    /*00001f64*/ MOV         R0, R4 
    /*00001f68*/ MOV         R2, #0 
    /*00001f6c*/ MOVW        R1, #12828                  @ 0x321c 
    /*00001f70*/ BL          mn34220_write_register
    /*00001f74*/ MOV         R0, R4 
    /*00001f78*/ MOV         R2, #0 
    /*00001f7c*/ MOVW        R1, #12829                  @ 0x321d 
    /*00001f80*/ BL          mn34220_write_register
    /*00001f84*/ MOV         R0, R4 
    /*00001f88*/ MOV         R2, #0 
    /*00001f8c*/ MOVW        R1, #12830                  @ 0x321e 
    /*00001f90*/ BL          mn34220_write_register
    /*00001f94*/ MOV         R0, R4 
    /*00001f98*/ MOV         R2, #0 
    /*00001f9c*/ MOVW        R1, #12831                  @ 0x321f 
    /*00001fa0*/ BL          mn34220_write_register
    /*00001fa4*/ MOV         R0, R4 
    /*00001fa8*/ MOV         R2, #0 
    /*00001fac*/ MOVW        R1, #12832                  @ 0x3220 
    /*00001fb0*/ BL          mn34220_write_register
    /*00001fb4*/ MOV         R0, R4 
    /*00001fb8*/ MOV         R2, #0 
    /*00001fbc*/ MOVW        R1, #12833                  @ 0x3221 
    /*00001fc0*/ BL          mn34220_write_register
    /*00001fc4*/ MOV         R0, R4 
    /*00001fc8*/ MOV         R2, #255                    @ 0xff 
    /*00001fcc*/ MOVW        R1, #12834                  @ 0x3222 
    /*00001fd0*/ BL          mn34220_write_register
    /*00001fd4*/ MOV         R0, R4 
    /*00001fd8*/ MOV         R2, #255                    @ 0xff 
    /*00001fdc*/ MOVW        R1, #12835                  @ 0x3223 
    /*00001fe0*/ BL          mn34220_write_register
    /*00001fe4*/ MOV         R0, R4 
    /*00001fe8*/ MOV         R2, #255                    @ 0xff 
    /*00001fec*/ MOVW        R1, #12836                  @ 0x3224 
    /*00001ff0*/ BL          mn34220_write_register
    /*00001ff4*/ MOV         R0, R4 
    /*00001ff8*/ MOV         R2, #255                    @ 0xff 
    /*00001ffc*/ MOVW        R1, #12837                  @ 0x3225 
    /*00002000*/ BL          mn34220_write_register
    /*00002004*/ MOV         R0, R4 
    /*00002008*/ MOV         R2, #255                    @ 0xff 
    /*0000200c*/ MOVW        R1, #12838                  @ 0x3226 
    /*00002010*/ BL          mn34220_write_register
    /*00002014*/ MOV         R0, R4 
    /*00002018*/ MOV         R2, #255                    @ 0xff 
    /*0000201c*/ MOVW        R1, #12839                  @ 0x3227 
    /*00002020*/ BL          mn34220_write_register
    /*00002024*/ MOV         R0, R4 
    /*00002028*/ MOV         R2, #255                    @ 0xff 
    /*0000202c*/ MOVW        R1, #12840                  @ 0x3228 
    /*00002030*/ BL          mn34220_write_register
    /*00002034*/ MOV         R0, R4 
    /*00002038*/ MOV         R2, #255                    @ 0xff 
    /*0000203c*/ MOVW        R1, #12841                  @ 0x3229 
    /*00002040*/ BL          mn34220_write_register
    /*00002044*/ MOV         R0, R4 
    /*00002048*/ MOV         R2, #255                    @ 0xff 
    /*0000204c*/ MOVW        R1, #12842                  @ 0x322a 
    /*00002050*/ BL          mn34220_write_register
    /*00002054*/ MOV         R0, R4 
    /*00002058*/ MOV         R2, #255                    @ 0xff 
    /*0000205c*/ MOVW        R1, #12843                  @ 0x322b 
    /*00002060*/ BL          mn34220_write_register
    /*00002064*/ MOV         R0, R4 
    /*00002068*/ MOV         R2, #255                    @ 0xff 
    /*0000206c*/ MOVW        R1, #12844                  @ 0x322c 
    /*00002070*/ BL          mn34220_write_register
    /*00002074*/ MOV         R0, R4 
    /*00002078*/ MOV         R2, #255                    @ 0xff 
    /*0000207c*/ MOVW        R1, #12845                  @ 0x322d 
    /*00002080*/ BL          mn34220_write_register
    /*00002084*/ MOV         R0, R4 
    /*00002088*/ MOV         R2, #255                    @ 0xff 
    /*0000208c*/ MOVW        R1, #12846                  @ 0x322e 
    /*00002090*/ BL          mn34220_write_register
    /*00002094*/ MOV         R0, R4 
    /*00002098*/ MOV         R2, #255                    @ 0xff 
    /*0000209c*/ MOVW        R1, #12847                  @ 0x322f 
    /*000020a0*/ BL          mn34220_write_register
    /*000020a4*/ MOV         R0, R4 
    /*000020a8*/ MOV         R2, #255                    @ 0xff 
    /*000020ac*/ MOVW        R1, #12848                  @ 0x3230 
    /*000020b0*/ BL          mn34220_write_register
    /*000020b4*/ MOV         R0, R4 
    /*000020b8*/ MOV         R2, #255                    @ 0xff 
    /*000020bc*/ MOVW        R1, #12849                  @ 0x3231 
    /*000020c0*/ BL          mn34220_write_register
    /*000020c4*/ MOV         R0, R4 
    /*000020c8*/ MOV         R2, #0 
    /*000020cc*/ MOVW        R1, #12850                  @ 0x3232 
    /*000020d0*/ BL          mn34220_write_register
    /*000020d4*/ MOV         R0, R4 
    /*000020d8*/ MOV         R2, #0 
    /*000020dc*/ MOVW        R1, #12851                  @ 0x3233 
    /*000020e0*/ BL          mn34220_write_register
    /*000020e4*/ MOV         R0, R4 
    /*000020e8*/ MOV         R2, #1 
    /*000020ec*/ MOVW        R1, #12852                  @ 0x3234 
    /*000020f0*/ BL          mn34220_write_register
    /*000020f4*/ MOV         R0, R4 
    /*000020f8*/ MOV         R2, #186                    @ 0xba 
    /*000020fc*/ MOVW        R1, #12853                  @ 0x3235 
    /*00002100*/ BL          mn34220_write_register
    /*00002104*/ MOV         R0, R4 
    /*00002108*/ MOV         R2, #0 
    /*0000210c*/ MOVW        R1, #12854                  @ 0x3236 
    /*00002110*/ BL          mn34220_write_register
    /*00002114*/ MOV         R0, R4 
    /*00002118*/ MOV         R2, #0 
    /*0000211c*/ MOVW        R1, #12855                  @ 0x3237 
    /*00002120*/ BL          mn34220_write_register
    /*00002124*/ MOV         R0, R4 
    /*00002128*/ MOV         R2, #1 
    /*0000212c*/ MOVW        R1, #12856                  @ 0x3238 
    /*00002130*/ BL          mn34220_write_register
    /*00002134*/ MOV         R0, R4 
    /*00002138*/ MOV         R2, #186                    @ 0xba 
    /*0000213c*/ MOVW        R1, #12857                  @ 0x3239 
    /*00002140*/ BL          mn34220_write_register
    /*00002144*/ MOV         R0, R4 
    /*00002148*/ MOV         R2, #1 
    /*0000214c*/ MOVW        R1, #12858                  @ 0x323a 
    /*00002150*/ BL          mn34220_write_register
    /*00002154*/ MOV         R0, R4 
    /*00002158*/ MOV         R2, #129                    @ 0x81 
    /*0000215c*/ MOVW        R1, #12859                  @ 0x323b 
    /*00002160*/ BL          mn34220_write_register
    /*00002164*/ MOV         R0, R4 
    /*00002168*/ MOV         R2, #112                    @ 0x70 
    /*0000216c*/ MOVW        R1, #12860                  @ 0x323c 
    /*00002170*/ BL          mn34220_write_register
    /*00002174*/ MOV         R0, R4 
    /*00002178*/ MOV         R2, #128                    @ 0x80 
    /*0000217c*/ MOVW        R1, #12861                  @ 0x323d 
    /*00002180*/ BL          mn34220_write_register
    /*00002184*/ MOV         R0, R4 
    /*00002188*/ MOV         R2, #0 
    /*0000218c*/ MOVW        R1, #12862                  @ 0x323e 
    /*00002190*/ BL          mn34220_write_register
    /*00002194*/ MOV         R0, R4 
    /*00002198*/ MOV         R2, #0 
    /*0000219c*/ MOVW        R1, #12863                  @ 0x323f 
    /*000021a0*/ BL          mn34220_write_register
    /*000021a4*/ MOV         R0, R4 
    /*000021a8*/ MOV         R2, #21 
    /*000021ac*/ MOV         R1, #12864                  @ 0x3240 
    /*000021b0*/ BL          mn34220_write_register
    /*000021b4*/ MOV         R0, R4 
    /*000021b8*/ MOV         R2, #17 
    /*000021bc*/ MOVW        R1, #12865                  @ 0x3241 
    /*000021c0*/ BL          mn34220_write_register
    /*000021c4*/ MOV         R0, R4 
    /*000021c8*/ MOV         R2, #0 
    /*000021cc*/ MOVW        R1, #12866                  @ 0x3242 
    /*000021d0*/ BL          mn34220_write_register
    /*000021d4*/ MOV         R0, R4 
    /*000021d8*/ MOV         R2, #117                    @ 0x75 
    /*000021dc*/ MOVW        R1, #12867                  @ 0x3243 
    /*000021e0*/ BL          mn34220_write_register
    /*000021e4*/ MOV         R0, R4 
    /*000021e8*/ MOV         R2, #0 
    /*000021ec*/ MOVW        R1, #12868                  @ 0x3244 
    /*000021f0*/ BL          mn34220_write_register
    /*000021f4*/ MOV         R0, R4 
    /*000021f8*/ MOV         R2, #24 
    /*000021fc*/ MOVW        R1, #12869                  @ 0x3245 
    /*00002200*/ BL          mn34220_write_register
    /*00002204*/ MOV         R0, R4 
    /*00002208*/ MOV         R2, #3 
    /*0000220c*/ MOVW        R1, #12870                  @ 0x3246 
    /*00002210*/ BL          mn34220_write_register
    /*00002214*/ MOV         R0, R4 
    /*00002218*/ MOV         R2, #59                     @ 0x3b 
    /*0000221c*/ MOVW        R1, #12871                  @ 0x3247 
    /*00002220*/ BL          mn34220_write_register
    /*00002224*/ MOV         R0, R4 
    /*00002228*/ MOV         R2, #1 
    /*0000222c*/ MOVW        R1, #12872                  @ 0x3248 
    /*00002230*/ BL          mn34220_write_register
    /*00002234*/ MOV         R0, R4 
    /*00002238*/ MOV         R2, #171                    @ 0xab 
    /*0000223c*/ MOVW        R1, #12873                  @ 0x3249 
    /*00002240*/ BL          mn34220_write_register
    /*00002244*/ MOV         R0, R4 
    /*00002248*/ MOV         R2, #48                     @ 0x30 
    /*0000224c*/ MOVW        R1, #12874                  @ 0x324a 
    /*00002250*/ BL          mn34220_write_register
    /*00002254*/ MOV         R0, R4 
    /*00002258*/ MOV         R2, #27 
    /*0000225c*/ MOVW        R1, #12875                  @ 0x324b 
    /*00002260*/ BL          mn34220_write_register
    /*00002264*/ MOV         R0, R4 
    /*00002268*/ MOV         R2, #2 
    /*0000226c*/ MOVW        R1, #12876                  @ 0x324c 
    /*00002270*/ BL          mn34220_write_register
    /*00002274*/ MOV         R0, R4 
    /*00002278*/ MOV         R2, #0 
    /*0000227c*/ MOVW        R1, #12877                  @ 0x324d 
    /*00002280*/ BL          mn34220_write_register
    /*00002284*/ MOV         R0, R4 
    /*00002288*/ MOV         R2, #0 
    /*0000228c*/ MOVW        R1, #12878                  @ 0x324e 
    /*00002290*/ BL          mn34220_write_register
    /*00002294*/ MOV         R0, R4 
    /*00002298*/ MOV         R2, #24 
    /*0000229c*/ MOVW        R1, #12879                  @ 0x324f 
    /*000022a0*/ BL          mn34220_write_register
    /*000022a4*/ MOV         R0, R4 
    /*000022a8*/ MOV         R2, #49                     @ 0x31 
    /*000022ac*/ MOVW        R1, #12880                  @ 0x3250 
    /*000022b0*/ BL          mn34220_write_register
    /*000022b4*/ MOV         R0, R4 
    /*000022b8*/ MOV         R2, #14 
    /*000022bc*/ MOVW        R1, #12881                  @ 0x3251 
    /*000022c0*/ BL          mn34220_write_register
    /*000022c4*/ MOV         R0, R4 
    /*000022c8*/ MOV         R2, #0 
    /*000022cc*/ MOVW        R1, #12882                  @ 0x3252 
    /*000022d0*/ BL          mn34220_write_register
    /*000022d4*/ MOV         R0, R4 
    /*000022d8*/ MOV         R2, #123                    @ 0x7b 
    /*000022dc*/ MOVW        R1, #12883                  @ 0x3253 
    /*000022e0*/ BL          mn34220_write_register
    /*000022e4*/ MOV         R0, R4 
    /*000022e8*/ MOV         R2, #3 
    /*000022ec*/ MOVW        R1, #12884                  @ 0x3254 
    /*000022f0*/ BL          mn34220_write_register
    /*000022f4*/ MOV         R0, R4 
    /*000022f8*/ MOV         R2, #8 
    /*000022fc*/ MOVW        R1, #12885                  @ 0x3255 
    /*00002300*/ BL          mn34220_write_register
    /*00002304*/ MOV         R0, R4 
    /*00002308*/ MOV         R2, #50                     @ 0x32 
    /*0000230c*/ MOVW        R1, #12886                  @ 0x3256 
    /*00002310*/ BL          mn34220_write_register
    /*00002314*/ MOV         R0, R4 
    /*00002318*/ MOV         R2, #0 
    /*0000231c*/ MOVW        R1, #12887                  @ 0x3257 
    /*00002320*/ BL          mn34220_write_register
    /*00002324*/ MOV         R0, R4 
    /*00002328*/ MOV         R2, #3 
    /*0000232c*/ MOVW        R1, #12888                  @ 0x3258 
    /*00002330*/ BL          mn34220_write_register
    /*00002334*/ MOV         R0, R4 
    /*00002338*/ MOV         R2, #4 
    /*0000233c*/ MOVW        R1, #12889                  @ 0x3259 
    /*00002340*/ BL          mn34220_write_register
    /*00002344*/ MOV         R0, R4 
    /*00002348*/ MOV         R2, #20 
    /*0000234c*/ MOVW        R1, #12890                  @ 0x325a 
    /*00002350*/ BL          mn34220_write_register
    /*00002354*/ MOV         R0, R4 
    /*00002358*/ MOV         R2, #0 
    /*0000235c*/ MOVW        R1, #12891                  @ 0x325b 
    /*00002360*/ BL          mn34220_write_register
    /*00002364*/ MOV         R0, R4 
    /*00002368*/ MOV         R2, #0 
    /*0000236c*/ MOVW        R1, #12892                  @ 0x325c 
    /*00002370*/ BL          mn34220_write_register
    /*00002374*/ MOV         R0, R4 
    /*00002378*/ MOV         R2, #12 
    /*0000237c*/ MOVW        R1, #12893                  @ 0x325d 
    /*00002380*/ BL          mn34220_write_register
    /*00002384*/ MOV         R0, R4 
    /*00002388*/ MOV         R2, #132                    @ 0x84 
    /*0000238c*/ MOVW        R1, #12894                  @ 0x325e 
    /*00002390*/ BL          mn34220_write_register
    /*00002394*/ MOV         R0, R4 
    /*00002398*/ MOV         R2, #0 
    /*0000239c*/ MOVW        R1, #12895                  @ 0x325f 
    /*000023a0*/ BL          mn34220_write_register
    /*000023a4*/ MOV         R0, R4 
    /*000023a8*/ MOV         R2, #0 
    /*000023ac*/ MOVW        R1, #12896                  @ 0x3260 
    /*000023b0*/ BL          mn34220_write_register
    /*000023b4*/ MOV         R0, R4 
    /*000023b8*/ MOV         R2, #0 
    /*000023bc*/ MOVW        R1, #12897                  @ 0x3261 
    /*000023c0*/ BL          mn34220_write_register
    /*000023c4*/ MOV         R0, R4 
    /*000023c8*/ MOV         R2, #1 
    /*000023cc*/ MOVW        R1, #12912                  @ 0x3270 
    /*000023d0*/ BL          mn34220_write_register
    /*000023d4*/ MOV         R0, R4 
    /*000023d8*/ MOV         R2, #0 
    /*000023dc*/ MOVW        R1, #12913                  @ 0x3271 
    /*000023e0*/ BL          mn34220_write_register
    /*000023e4*/ MOV         R0, R4 
    /*000023e8*/ MOV         R2, #12 
    /*000023ec*/ MOVW        R1, #12914                  @ 0x3272 
    /*000023f0*/ BL          mn34220_write_register
    /*000023f4*/ MOV         R0, R4 
    /*000023f8*/ MOV         R2, #171                    @ 0xab 
    /*000023fc*/ MOVW        R1, #12915                  @ 0x3273 
    /*00002400*/ BL          mn34220_write_register
    /*00002404*/ MOV         R0, R4 
    /*00002408*/ MOV         R2, #1 
    /*0000240c*/ MOVW        R1, #12916                  @ 0x3274 
    /*00002410*/ BL          mn34220_write_register
    /*00002414*/ MOV         R0, R4 
    /*00002418*/ MOV         R2, #24 
    /*0000241c*/ MOVW        R1, #12917                  @ 0x3275 
    /*00002420*/ BL          mn34220_write_register
    /*00002424*/ MOV         R0, R4 
    /*00002428*/ MOV         R2, #48                     @ 0x30 
    /*0000242c*/ MOV         R1, #12928                  @ 0x3280 
    /*00002430*/ BL          mn34220_write_register
    /*00002434*/ MOV         R0, R4 
    /*00002438*/ MOV         R2, #16 
    /*0000243c*/ MOVW        R1, #12929                  @ 0x3281 
    /*00002440*/ BL          mn34220_write_register
    /*00002444*/ MOV         R0, R4 
    /*00002448*/ MOV         R2, #14 
    /*0000244c*/ MOVW        R1, #12930                  @ 0x3282 
    /*00002450*/ BL          mn34220_write_register
    /*00002454*/ MOV         R0, R4 
    /*00002458*/ MOV         R2, #0 
    /*0000245c*/ MOVW        R1, #12931                  @ 0x3283 
    /*00002460*/ BL          mn34220_write_register
    /*00002464*/ MOV         R0, R4 
    /*00002468*/ MOV         R2, #0 
    /*0000246c*/ MOVW        R1, #12932                  @ 0x3284 
    /*00002470*/ BL          mn34220_write_register
    /*00002474*/ MOV         R0, R4 
    /*00002478*/ MOV         R2, #27 
    /*0000247c*/ MOVW        R1, #12933                  @ 0x3285 
    /*00002480*/ BL          mn34220_write_register
    /*00002484*/ MOV         R0, R4 
    /*00002488*/ MOV         R2, #24 
    /*0000248c*/ MOVW        R1, #12934                  @ 0x3286 
    /*00002490*/ BL          mn34220_write_register
    /*00002494*/ MOV         R0, R4 
    /*00002498*/ MOV         R2, #0 
    /*0000249c*/ MOVW        R1, #12935                  @ 0x3287 
    /*000024a0*/ BL          mn34220_write_register
    /*000024a4*/ MOV         R0, R4 
    /*000024a8*/ MOV         R2, #1 
    /*000024ac*/ MOVW        R1, #12936                  @ 0x3288 
    /*000024b0*/ BL          mn34220_write_register
    /*000024b4*/ MOV         R0, R4 
    /*000024b8*/ MOV         R2, #0 
    /*000024bc*/ MOVW        R1, #12937                  @ 0x3289 
    /*000024c0*/ BL          mn34220_write_register
    /*000024c4*/ MOV         R0, R4 
    /*000024c8*/ MOV         R2, #232                    @ 0xe8 
    /*000024cc*/ MOVW        R1, #12938                  @ 0x328a 
    /*000024d0*/ BL          mn34220_write_register
    /*000024d4*/ MOV         R0, R4 
    /*000024d8*/ MOV         R2, #4 
    /*000024dc*/ MOVW        R1, #12939                  @ 0x328b 
    /*000024e0*/ BL          mn34220_write_register
    /*000024e4*/ MOV         R0, R4 
    /*000024e8*/ MOV         R2, #117                    @ 0x75 
    /*000024ec*/ MOVW        R1, #12940                  @ 0x328c 
    /*000024f0*/ BL          mn34220_write_register
    /*000024f4*/ MOV         R0, R4 
    /*000024f8*/ MOV         R2, #0 
    /*000024fc*/ MOVW        R1, #12941                  @ 0x328d 
    /*00002500*/ BL          mn34220_write_register
    /*00002504*/ MOV         R0, R4 
    /*00002508*/ MOV         R2, #3 
    /*0000250c*/ MOV         R1, #13056                  @ 0x3300 
    /*00002510*/ BL          mn34220_write_register
    /*00002514*/ MOV         R0, R4 
    /*00002518*/ MOV         R2, #1 
    /*0000251c*/ MOVW        R1, #13057                  @ 0x3301 
    /*00002520*/ BL          mn34220_write_register
    /*00002524*/ MOV         R0, R4 
    /*00002528*/ MOV         R2, #0 
    /*0000252c*/ MOVW        R1, #13058                  @ 0x3302 
    /*00002530*/ BL          mn34220_write_register
    /*00002534*/ MOV         R0, R4 
    /*00002538*/ MOV         R2, #7 
    /*0000253c*/ MOVW        R1, #13059                  @ 0x3303 
    /*00002540*/ BL          mn34220_write_register
    /*00002544*/ MOV         R0, R4 
    /*00002548*/ MOV         R2, #6 
    /*0000254c*/ MOVW        R1, #13060                  @ 0x3304 
    /*00002550*/ BL          mn34220_write_register
    /*00002554*/ MOV         R0, R4 
    /*00002558*/ MOV         R2, #10 
    /*0000255c*/ MOVW        R1, #13061                  @ 0x3305 
    /*00002560*/ BL          mn34220_write_register
    /*00002564*/ MOV         R0, R4 
    /*00002568*/ MOV         R2, #25 
    /*0000256c*/ MOVW        R1, #13062                  @ 0x3306 
    /*00002570*/ BL          mn34220_write_register
    /*00002574*/ MOV         R0, R4 
    /*00002578*/ MOV         R2, #25 
    /*0000257c*/ MOVW        R1, #13063                  @ 0x3307 
    /*00002580*/ BL          mn34220_write_register
    /*00002584*/ MOV         R0, R4 
    /*00002588*/ MOV         R2, #0 
    /*0000258c*/ MOVW        R1, #13064                  @ 0x3308 
    /*00002590*/ BL          mn34220_write_register
    /*00002594*/ MOV         R0, R4 
    /*00002598*/ MOV         R2, #0 
    /*0000259c*/ MOVW        R1, #13065                  @ 0x3309 
    /*000025a0*/ BL          mn34220_write_register
    /*000025a4*/ MOV         R0, R4 
    /*000025a8*/ MOV         R2, #30 
    /*000025ac*/ MOVW        R1, #13066                  @ 0x330a 
    /*000025b0*/ BL          mn34220_write_register
    /*000025b4*/ MOV         R0, R4 
    /*000025b8*/ MOV         R2, #30 
    /*000025bc*/ MOVW        R1, #13067                  @ 0x330b 
    /*000025c0*/ BL          mn34220_write_register
    /*000025c4*/ MOV         R0, R4 
    /*000025c8*/ MOV         R2, #0 
    /*000025cc*/ MOVW        R1, #13068                  @ 0x330c 
    /*000025d0*/ BL          mn34220_write_register
    /*000025d4*/ MOV         R0, R4 
    /*000025d8*/ MOV         R2, #0 
    /*000025dc*/ MOVW        R1, #13069                  @ 0x330d 
    /*000025e0*/ BL          mn34220_write_register
    /*000025e4*/ MOV         R0, R4 
    /*000025e8*/ MOV         R2, #5 
    /*000025ec*/ MOVW        R1, #13070                  @ 0x330e 
    /*000025f0*/ BL          mn34220_write_register
    /*000025f4*/ MOV         R0, R4 
    /*000025f8*/ MOV         R2, #17 
    /*000025fc*/ MOVW        R1, #13071                  @ 0x330f 
    /*00002600*/ BL          mn34220_write_register
    /*00002604*/ MOV         R0, R4 
    /*00002608*/ MOV         R2, #2 
    /*0000260c*/ MOVW        R1, #13072                  @ 0x3310 
    /*00002610*/ BL          mn34220_write_register
    /*00002614*/ MOV         R0, R4 
    /*00002618*/ MOV         R2, #0 
    /*0000261c*/ MOVW        R1, #13073                  @ 0x3311 
    /*00002620*/ BL          mn34220_write_register
    /*00002624*/ MOV         R0, R4 
    /*00002628*/ MOV         R2, #0 
    /*0000262c*/ MOVW        R1, #13074                  @ 0x3312 
    /*00002630*/ BL          mn34220_write_register
    /*00002634*/ MOV         R0, R4 
    /*00002638*/ MOV         R2, #0 
    /*0000263c*/ MOVW        R1, #13075                  @ 0x3313 
    /*00002640*/ BL          mn34220_write_register
    /*00002644*/ MOV         R0, R4 
    /*00002648*/ MOV         R2, #3 
    /*0000264c*/ MOVW        R1, #13076                  @ 0x3314 
    /*00002650*/ BL          mn34220_write_register
    /*00002654*/ MOV         R0, R4 
    /*00002658*/ MOV         R2, #31 
    /*0000265c*/ MOVW        R1, #13077                  @ 0x3315 
    /*00002660*/ BL          mn34220_write_register
    /*00002664*/ MOV         R0, R4 
    /*00002668*/ MOV         R2, #10 
    /*0000266c*/ MOVW        R1, #13078                  @ 0x3316 
    /*00002670*/ BL          mn34220_write_register
    /*00002674*/ MOV         R0, R4 
    /*00002678*/ MOV         R2, #0 
    /*0000267c*/ MOVW        R1, #13079                  @ 0x3317 
    /*00002680*/ BL          mn34220_write_register
    /*00002684*/ MOV         R0, R4 
    /*00002688*/ MOV         R2, #6 
    /*0000268c*/ MOVW        R1, #13080                  @ 0x3318 
    /*00002690*/ BL          mn34220_write_register
    /*00002694*/ MOV         R0, R4 
    /*00002698*/ MOV         R2, #0 
    /*0000269c*/ MOVW        R1, #13081                  @ 0x3319 
    /*000026a0*/ BL          mn34220_write_register
    /*000026a4*/ MOV         R0, R4 
    /*000026a8*/ MOV         R2, #3 
    /*000026ac*/ MOVW        R1, #13082                  @ 0x331a 
    /*000026b0*/ BL          mn34220_write_register
    /*000026b4*/ MOV         R0, R4 
    /*000026b8*/ MOV         R2, #3 
    /*000026bc*/ MOVW        R1, #13083                  @ 0x331b 
    /*000026c0*/ BL          mn34220_write_register
    /*000026c4*/ MOV         R0, R4 
    /*000026c8*/ MOV         R2, #0 
    /*000026cc*/ MOVW        R1, #13084                  @ 0x331c 
    /*000026d0*/ BL          mn34220_write_register
    /*000026d4*/ MOV         R0, R4 
    /*000026d8*/ MOV         R2, #0 
    /*000026dc*/ MOVW        R1, #13085                  @ 0x331d 
    /*000026e0*/ BL          mn34220_write_register
    /*000026e4*/ MOV         R0, R4 
    /*000026e8*/ MOV         R2, #3 
    /*000026ec*/ MOVW        R1, #13086                  @ 0x331e 
    /*000026f0*/ BL          mn34220_write_register
    /*000026f4*/ MOV         R0, R4 
    /*000026f8*/ MOV         R2, #3 
    /*000026fc*/ MOVW        R1, #13087                  @ 0x331f 
    /*00002700*/ BL          mn34220_write_register
    /*00002704*/ MOV         R0, R4 
    /*00002708*/ MOV         R2, #1 
    /*0000270c*/ MOVW        R1, #13088                  @ 0x3320 
    /*00002710*/ BL          mn34220_write_register
    /*00002714*/ MOV         R0, R4 
    /*00002718*/ MOV         R2, #1 
    /*0000271c*/ MOVW        R1, #13089                  @ 0x3321 
    /*00002720*/ BL          mn34220_write_register
    /*00002724*/ MOV         R0, R4 
    /*00002728*/ MOV         R2, #0 
    /*0000272c*/ MOVW        R1, #13090                  @ 0x3322 
    /*00002730*/ BL          mn34220_write_register
    /*00002734*/ MOV         R0, R4 
    /*00002738*/ MOV         R2, #0 
    /*0000273c*/ MOVW        R1, #13091                  @ 0x3323 
    /*00002740*/ BL          mn34220_write_register
    /*00002744*/ MOV         R0, R4 
    /*00002748*/ MOV         R2, #0 
    /*0000274c*/ MOVW        R1, #13092                  @ 0x3324 
    /*00002750*/ BL          mn34220_write_register
    /*00002754*/ MOV         R0, R4 
    /*00002758*/ MOV         R2, #0 
    /*0000275c*/ MOVW        R1, #13093                  @ 0x3325 
    /*00002760*/ BL          mn34220_write_register
    /*00002764*/ MOV         R0, R4 
    /*00002768*/ MOV         R2, #10 
    /*0000276c*/ MOVW        R1, #13094                  @ 0x3326 
    /*00002770*/ BL          mn34220_write_register
    /*00002774*/ MOV         R0, R4 
    /*00002778*/ MOV         R2, #0 
    /*0000277c*/ MOVW        R1, #13095                  @ 0x3327 
    /*00002780*/ BL          mn34220_write_register
    /*00002784*/ MOV         R0, R4 
    /*00002788*/ MOV         R2, #0 
    /*0000278c*/ MOVW        R1, #13096                  @ 0x3328 
    /*00002790*/ BL          mn34220_write_register
    /*00002794*/ MOV         R0, R4 
    /*00002798*/ MOV         R2, #0 
    /*0000279c*/ MOVW        R1, #13097                  @ 0x3329 
    /*000027a0*/ BL          mn34220_write_register
    /*000027a4*/ MOV         R0, R4 
    /*000027a8*/ MOV         R2, #3 
    /*000027ac*/ MOVW        R1, #13098                  @ 0x332a 
    /*000027b0*/ BL          mn34220_write_register
    /*000027b4*/ MOV         R0, R4 
    /*000027b8*/ MOV         R2, #3 
    /*000027bc*/ MOVW        R1, #13099                  @ 0x332b 
    /*000027c0*/ BL          mn34220_write_register
    /*000027c4*/ MOV         R0, R4 
    /*000027c8*/ MOV         R2, #0 
    /*000027cc*/ MOVW        R1, #13100                  @ 0x332c 
    /*000027d0*/ BL          mn34220_write_register
    /*000027d4*/ MOV         R0, R4 
    /*000027d8*/ MOV         R2, #0 
    /*000027dc*/ MOVW        R1, #13101                  @ 0x332d 
    /*000027e0*/ BL          mn34220_write_register
    /*000027e4*/ MOV         R0, R4 
    /*000027e8*/ MOV         R2, #10 
    /*000027ec*/ MOVW        R1, #13102                  @ 0x332e 
    /*000027f0*/ BL          mn34220_write_register
    /*000027f4*/ MOV         R0, R4 
    /*000027f8*/ MOV         R2, #0 
    /*000027fc*/ MOVW        R1, #13103                  @ 0x332f 
    /*00002800*/ BL          mn34220_write_register
    /*00002804*/ MOV         R0, R4 
    /*00002808*/ MOV         R2, #6 
    /*0000280c*/ MOVW        R1, #13104                  @ 0x3330 
    /*00002810*/ BL          mn34220_write_register
    /*00002814*/ MOV         R0, R4 
    /*00002818*/ MOV         R2, #0 
    /*0000281c*/ MOVW        R1, #13105                  @ 0x3331 
    /*00002820*/ BL          mn34220_write_register
    /*00002824*/ MOV         R0, R4 
    /*00002828*/ MOV         R2, #18 
    /*0000282c*/ MOVW        R1, #13106                  @ 0x3332 
    /*00002830*/ BL          mn34220_write_register
    /*00002834*/ MOV         R0, R4 
    /*00002838*/ MOV         R2, #0 
    /*0000283c*/ MOVW        R1, #13107                  @ 0x3333 
    /*00002840*/ BL          mn34220_write_register
    /*00002844*/ MOV         R0, R4 
    /*00002848*/ MOV         R2, #3 
    /*0000284c*/ MOVW        R1, #13108                  @ 0x3334 
    /*00002850*/ BL          mn34220_write_register
    /*00002854*/ MOV         R0, R4 
    /*00002858*/ MOV         R2, #0 
    /*0000285c*/ MOVW        R1, #13109                  @ 0x3335 
    /*00002860*/ BL          mn34220_write_register
    /*00002864*/ MOV         R0, R4 
    /*00002868*/ MOV         R2, #3 
    /*0000286c*/ MOVW        R1, #13110                  @ 0x3336 
    /*00002870*/ BL          mn34220_write_register
    /*00002874*/ MOV         R0, R4 
    /*00002878*/ MOV         R2, #3 
    /*0000287c*/ MOVW        R1, #13111                  @ 0x3337 
    /*00002880*/ BL          mn34220_write_register
    /*00002884*/ MOV         R0, R4 
    /*00002888*/ MOV         R2, #0 
    /*0000288c*/ MOVW        R1, #13112                  @ 0x3338 
    /*00002890*/ BL          mn34220_write_register
    /*00002894*/ MOV         R0, R4 
    /*00002898*/ MOV         R2, #3 
    /*0000289c*/ MOVW        R1, #13113                  @ 0x3339 
    /*000028a0*/ BL          mn34220_write_register
    /*000028a4*/ MOV         R0, R4 
    /*000028a8*/ MOV         R2, #8 
    /*000028ac*/ MOVW        R1, #13114                  @ 0x333a 
    /*000028b0*/ BL          mn34220_write_register
    /*000028b4*/ MOV         R0, R4 
    /*000028b8*/ MOV         R2, #0 
    /*000028bc*/ MOVW        R1, #13115                  @ 0x333b 
    /*000028c0*/ BL          mn34220_write_register
    /*000028c4*/ MOV         R0, R4 
    /*000028c8*/ MOV         R2, #0 
    /*000028cc*/ MOVW        R1, #13116                  @ 0x333c 
    /*000028d0*/ BL          mn34220_write_register
    /*000028d4*/ MOV         R0, R4 
    /*000028d8*/ MOV         R2, #0 
    /*000028dc*/ MOVW        R1, #13117                  @ 0x333d 
    /*000028e0*/ BL          mn34220_write_register
    /*000028e4*/ MOV         R0, R4 
    /*000028e8*/ MOV         R2, #0 
    /*000028ec*/ MOVW        R1, #13118                  @ 0x333e 
    /*000028f0*/ BL          mn34220_write_register
    /*000028f4*/ MOV         R0, R4 
    /*000028f8*/ MOV         R2, #0 
    /*000028fc*/ MOVW        R1, #13119                  @ 0x333f 
    /*00002900*/ BL          mn34220_write_register
    /*00002904*/ MOV         R0, R4 
    /*00002908*/ MOV         R2, #0 
    /*0000290c*/ MOV         R1, #13120                  @ 0x3340 
    /*00002910*/ BL          mn34220_write_register
    /*00002914*/ MOV         R0, R4 
    /*00002918*/ MOV         R2, #0 
    /*0000291c*/ MOVW        R1, #13121                  @ 0x3341 
    /*00002920*/ BL          mn34220_write_register
    /*00002924*/ MOV         R0, R4 
    /*00002928*/ MOV         R2, #0 
    /*0000292c*/ MOVW        R1, #13122                  @ 0x3342 
    /*00002930*/ BL          mn34220_write_register
    /*00002934*/ MOV         R0, R4 
    /*00002938*/ MOV         R2, #0 
    /*0000293c*/ MOVW        R1, #13123                  @ 0x3343 
    /*00002940*/ BL          mn34220_write_register
    /*00002944*/ MOV         R0, R4 
    /*00002948*/ MOV         R2, #0 
    /*0000294c*/ MOVW        R1, #13124                  @ 0x3344 
    /*00002950*/ BL          mn34220_write_register
    /*00002954*/ MOV         R0, R4 
    /*00002958*/ MOV         R2, #0 
    /*0000295c*/ MOVW        R1, #13125                  @ 0x3345 
    /*00002960*/ BL          mn34220_write_register
    /*00002964*/ MOV         R0, R4 
    /*00002968*/ MOV         R2, #0 
    /*0000296c*/ MOVW        R1, #13126                  @ 0x3346 
    /*00002970*/ BL          mn34220_write_register
    /*00002974*/ MOV         R0, R4 
    /*00002978*/ MOV         R2, #0 
    /*0000297c*/ MOVW        R1, #13127                  @ 0x3347 
    /*00002980*/ BL          mn34220_write_register
    /*00002984*/ MOV         R0, R4 
    /*00002988*/ MOV         R2, #7 
    /*0000298c*/ MOVW        R1, #13128                  @ 0x3348 
    /*00002990*/ BL          mn34220_write_register
    /*00002994*/ MOV         R0, R4 
    /*00002998*/ MOV         R2, #0 
    /*0000299c*/ MOVW        R1, #13129                  @ 0x3349 
    /*000029a0*/ BL          mn34220_write_register
    /*000029a4*/ MOV         R0, R4 
    /*000029a8*/ MOV         R2, #5 
    /*000029ac*/ MOVW        R1, #13130                  @ 0x334a 
    /*000029b0*/ BL          mn34220_write_register
    /*000029b4*/ MOV         R0, R4 
    /*000029b8*/ MOV         R2, #0 
    /*000029bc*/ MOVW        R1, #13131                  @ 0x334b 
    /*000029c0*/ BL          mn34220_write_register
    /*000029c4*/ MOV         R0, R4 
    /*000029c8*/ MOV         R2, #5 
    /*000029cc*/ MOVW        R1, #13132                  @ 0x334c 
    /*000029d0*/ BL          mn34220_write_register
    /*000029d4*/ MOV         R0, R4 
    /*000029d8*/ MOV         R2, #28 
    /*000029dc*/ MOVW        R1, #13133                  @ 0x334d 
    /*000029e0*/ BL          mn34220_write_register
    /*000029e4*/ MOV         R0, R4 
    /*000029e8*/ MOV         R2, #0 
    /*000029ec*/ MOVW        R1, #13134                  @ 0x334e 
    /*000029f0*/ BL          mn34220_write_register
    /*000029f4*/ MOV         R0, R4 
    /*000029f8*/ MOV         R2, #3 
    /*000029fc*/ MOVW        R1, #13135                  @ 0x334f 
    /*00002a00*/ BL          mn34220_write_register
    /*00002a04*/ MOV         R0, R4 
    /*00002a08*/ MOV         R2, #14 
    /*00002a0c*/ MOVW        R1, #13136                  @ 0x3350 
    /*00002a10*/ BL          mn34220_write_register
    /*00002a14*/ MOV         R0, R4 
    /*00002a18*/ MOV         R2, #8 
    /*00002a1c*/ MOVW        R1, #13137                  @ 0x3351 
    /*00002a20*/ BL          mn34220_write_register
    /*00002a24*/ MOV         R0, R4 
    /*00002a28*/ MOV         R2, #6 
    /*00002a2c*/ MOVW        R1, #13138                  @ 0x3352 
    /*00002a30*/ BL          mn34220_write_register
    /*00002a34*/ MOV         R0, R4 
    /*00002a38*/ MOV         R2, #5 
    /*00002a3c*/ MOVW        R1, #13139                  @ 0x3353 
    /*00002a40*/ BL          mn34220_write_register
    /*00002a44*/ MOV         R0, R4 
    /*00002a48*/ MOV         R2, #8 
    /*00002a4c*/ MOVW        R1, #13140                  @ 0x3354 
    /*00002a50*/ BL          mn34220_write_register
    /*00002a54*/ MOV         R0, R4 
    /*00002a58*/ MOV         R2, #25 
    /*00002a5c*/ MOVW        R1, #13141                  @ 0x3355 
    /*00002a60*/ BL          mn34220_write_register
    /*00002a64*/ MOV         R0, R4 
    /*00002a68*/ MOV         R2, #0 
    /*00002a6c*/ MOVW        R1, #13142                  @ 0x3356 
    /*00002a70*/ BL          mn34220_write_register
    /*00002a74*/ MOV         R0, R4 
    /*00002a78*/ MOV         R2, #3 
    /*00002a7c*/ MOVW        R1, #13143                  @ 0x3357 
    /*00002a80*/ BL          mn34220_write_register
    /*00002a84*/ MOV         R0, R4 
    /*00002a88*/ MOV         R2, #9 
    /*00002a8c*/ MOVW        R1, #13144                  @ 0x3358 
    /*00002a90*/ BL          mn34220_write_register
    /*00002a94*/ MOV         R0, R4 
    /*00002a98*/ MOV         R2, #11 
    /*00002a9c*/ MOVW        R1, #13145                  @ 0x3359 
    /*00002aa0*/ BL          mn34220_write_register
    /*00002aa4*/ MOV         R0, R4 
    /*00002aa8*/ MOV         R2, #13 
    /*00002aac*/ MOVW        R1, #13146                  @ 0x335a 
    /*00002ab0*/ BL          mn34220_write_register
    /*00002ab4*/ MOV         R0, R4 
    /*00002ab8*/ MOV         R2, #0 
    /*00002abc*/ MOVW        R1, #13147                  @ 0x335b 
    /*00002ac0*/ BL          mn34220_write_register
    /*00002ac4*/ MOV         R0, R4 
    /*00002ac8*/ MOV         R2, #0 
    /*00002acc*/ MOVW        R1, #13148                  @ 0x335c 
    /*00002ad0*/ BL          mn34220_write_register
    /*00002ad4*/ MOV         R0, R4 
    /*00002ad8*/ MOV         R2, #1 
    /*00002adc*/ MOVW        R1, #13149                  @ 0x335d 
    /*00002ae0*/ BL          mn34220_write_register
    /*00002ae4*/ MOV         R0, R4 
    /*00002ae8*/ MOV         R2, #13 
    /*00002aec*/ MOVW        R1, #13150                  @ 0x335e 
    /*00002af0*/ BL          mn34220_write_register
    /*00002af4*/ MOV         R0, R4 
    /*00002af8*/ MOV         R2, #1 
    /*00002afc*/ MOVW        R1, #13151                  @ 0x335f 
    /*00002b00*/ BL          mn34220_write_register
    /*00002b04*/ MOV         R0, R4 
    /*00002b08*/ MOV         R2, #13 
    /*00002b0c*/ MOVW        R1, #13152                  @ 0x3360 
    /*00002b10*/ BL          mn34220_write_register
    /*00002b14*/ MOV         R0, R4 
    /*00002b18*/ MOV         R2, #7 
    /*00002b1c*/ MOVW        R1, #13153                  @ 0x3361 
    /*00002b20*/ BL          mn34220_write_register
    /*00002b24*/ MOV         R0, R4 
    /*00002b28*/ MOV         R2, #13 
    /*00002b2c*/ MOVW        R1, #13154                  @ 0x3362 
    /*00002b30*/ BL          mn34220_write_register
    /*00002b34*/ MOV         R0, R4 
    /*00002b38*/ MOV         R2, #0 
    /*00002b3c*/ MOVW        R1, #13155                  @ 0x3363 
    /*00002b40*/ BL          mn34220_write_register
    /*00002b44*/ MOV         R0, R4 
    /*00002b48*/ MOV         R2, #0 
    /*00002b4c*/ MOVW        R1, #13156                  @ 0x3364 
    /*00002b50*/ BL          mn34220_write_register
    /*00002b54*/ MOV         R0, R4 
    /*00002b58*/ MOV         R2, #1 
    /*00002b5c*/ MOVW        R1, #13157                  @ 0x3365 
    /*00002b60*/ BL          mn34220_write_register
    /*00002b64*/ MOV         R0, R4 
    /*00002b68*/ MOV         R2, #13 
    /*00002b6c*/ MOVW        R1, #13158                  @ 0x3366 
    /*00002b70*/ BL          mn34220_write_register
    /*00002b74*/ MOV         R0, R4 
    /*00002b78*/ MOV         R2, #0 
    /*00002b7c*/ MOVW        R1, #13159                  @ 0x3367 
    /*00002b80*/ BL          mn34220_write_register
    /*00002b84*/ MOV         R0, R4 
    /*00002b88*/ MOV         R2, #13 
    /*00002b8c*/ MOVW        R1, #13160                  @ 0x3368 
    /*00002b90*/ BL          mn34220_write_register
    /*00002b94*/ MOV         R0, R4 
    /*00002b98*/ MOV         R2, #1 
    /*00002b9c*/ MOVW        R1, #13161                  @ 0x3369 
    /*00002ba0*/ BL          mn34220_write_register
    /*00002ba4*/ MOV         R0, R4 
    /*00002ba8*/ MOV         R2, #0 
    /*00002bac*/ MOVW        R1, #13162                  @ 0x336a 
    /*00002bb0*/ BL          mn34220_write_register
    /*00002bb4*/ MOV         R0, R4 
    /*00002bb8*/ MOV         R2, #2 
    /*00002bbc*/ MOVW        R1, #13163                  @ 0x336b 
    /*00002bc0*/ BL          mn34220_write_register
    /*00002bc4*/ MOV         R0, R4 
    /*00002bc8*/ MOV         R2, #3 
    /*00002bcc*/ MOVW        R1, #13164                  @ 0x336c 
    /*00002bd0*/ BL          mn34220_write_register
    /*00002bd4*/ MOV         R0, R4 
    /*00002bd8*/ MOV         R2, #0 
    /*00002bdc*/ MOVW        R1, #13165                  @ 0x336d 
    /*00002be0*/ BL          mn34220_write_register
    /*00002be4*/ MOV         R0, R4 
    /*00002be8*/ MOV         R2, #2 
    /*00002bec*/ MOVW        R1, #13166                  @ 0x336e 
    /*00002bf0*/ BL          mn34220_write_register
    /*00002bf4*/ MOV         R0, R4 
    /*00002bf8*/ MOV         R2, #0 
    /*00002bfc*/ MOVW        R1, #13167                  @ 0x336f 
    /*00002c00*/ BL          mn34220_write_register
    /*00002c04*/ MOV         R0, R4 
    /*00002c08*/ MOV         R2, #0 
    /*00002c0c*/ MOVW        R1, #13168                  @ 0x3370 
    /*00002c10*/ BL          mn34220_write_register
    /*00002c14*/ MOV         R0, R4 
    /*00002c18*/ MOV         R2, #0 
    /*00002c1c*/ MOVW        R1, #13169                  @ 0x3371 
    /*00002c20*/ BL          mn34220_write_register
    /*00002c24*/ MOV         R0, R4 
    /*00002c28*/ MOV         R2, #0 
    /*00002c2c*/ MOVW        R1, #13170                  @ 0x3372 
    /*00002c30*/ BL          mn34220_write_register
    /*00002c34*/ MOV         R0, R4 
    /*00002c38*/ MOV         R2, #0 
    /*00002c3c*/ MOVW        R1, #13171                  @ 0x3373 
    /*00002c40*/ BL          mn34220_write_register
    /*00002c44*/ MOV         R0, R4 
    /*00002c48*/ MOV         R2, #2 
    /*00002c4c*/ MOVW        R1, #13172                  @ 0x3374 
    /*00002c50*/ BL          mn34220_write_register
    /*00002c54*/ MOV         R0, R4 
    /*00002c58*/ MOV         R2, #0 
    /*00002c5c*/ MOVW        R1, #13173                  @ 0x3375 
    /*00002c60*/ BL          mn34220_write_register
    /*00002c64*/ MOV         R0, R4 
    /*00002c68*/ MOV         R2, #6 
    /*00002c6c*/ MOVW        R1, #13174                  @ 0x3376 
    /*00002c70*/ BL          mn34220_write_register
    /*00002c74*/ MOV         R0, R4 
    /*00002c78*/ MOV         R2, #1 
    /*00002c7c*/ MOVW        R1, #13175                  @ 0x3377 
    /*00002c80*/ BL          mn34220_write_register
    /*00002c84*/ MOV         R0, R4 
    /*00002c88*/ MOV         R2, #6 
    /*00002c8c*/ MOVW        R1, #13176                  @ 0x3378 
    /*00002c90*/ BL          mn34220_write_register
    /*00002c94*/ MOV         R0, R4 
    /*00002c98*/ MOV         R2, #6 
    /*00002c9c*/ MOVW        R1, #13177                  @ 0x3379 
    /*00002ca0*/ BL          mn34220_write_register
    /*00002ca4*/ MOV         R0, R4 
    /*00002ca8*/ MOV         R2, #9 
    /*00002cac*/ MOVW        R1, #13178                  @ 0x337a 
    /*00002cb0*/ BL          mn34220_write_register
    /*00002cb4*/ MOV         R0, R4 
    /*00002cb8*/ MOV         R2, #0 
    /*00002cbc*/ MOVW        R1, #13179                  @ 0x337b 
    /*00002cc0*/ BL          mn34220_write_register
    /*00002cc4*/ MOV         R0, R4 
    /*00002cc8*/ MOV         R2, #13 
    /*00002ccc*/ MOVW        R1, #13180                  @ 0x337c 
    /*00002cd0*/ BL          mn34220_write_register
    /*00002cd4*/ MOV         R0, R4 
    /*00002cd8*/ MOV         R2, #0 
    /*00002cdc*/ MOVW        R1, #13181                  @ 0x337d 
    /*00002ce0*/ BL          mn34220_write_register
    /*00002ce4*/ MOV         R0, R4 
    /*00002ce8*/ MOV         R2, #0 
    /*00002cec*/ MOVW        R1, #13182                  @ 0x337e 
    /*00002cf0*/ BL          mn34220_write_register
    /*00002cf4*/ MOV         R0, R4 
    /*00002cf8*/ MOV         R2, #1 
    /*00002cfc*/ MOVW        R1, #13183                  @ 0x337f 
    /*00002d00*/ BL          mn34220_write_register
    /*00002d04*/ MOV         R0, R4 
    /*00002d08*/ MOV         R2, #0 
    /*00002d0c*/ MOV         R1, #13184                  @ 0x3380 
    /*00002d10*/ BL          mn34220_write_register
    /*00002d14*/ MOV         R0, R4 
    /*00002d18*/ MOV         R2, #0 
    /*00002d1c*/ MOVW        R1, #13185                  @ 0x3381 
    /*00002d20*/ BL          mn34220_write_register
    /*00002d24*/ MOV         R0, R4 
    /*00002d28*/ MOV         R2, #0 
    /*00002d2c*/ MOVW        R1, #13186                  @ 0x3382 
    /*00002d30*/ BL          mn34220_write_register
    /*00002d34*/ MOV         R0, R4 
    /*00002d38*/ MOV         R2, #0 
    /*00002d3c*/ MOVW        R1, #13187                  @ 0x3383 
    /*00002d40*/ BL          mn34220_write_register
    /*00002d44*/ MOV         R0, R4 
    /*00002d48*/ MOV         R2, #13 
    /*00002d4c*/ MOVW        R1, #13188                  @ 0x3384 
    /*00002d50*/ BL          mn34220_write_register
    /*00002d54*/ MOV         R0, R4 
    /*00002d58*/ MOV         R2, #0 
    /*00002d5c*/ MOVW        R1, #13189                  @ 0x3385 
    /*00002d60*/ BL          mn34220_write_register
    /*00002d64*/ MOV         R0, R4 
    /*00002d68*/ MOV         R2, #0 
    /*00002d6c*/ MOVW        R1, #13190                  @ 0x3386 
    /*00002d70*/ BL          mn34220_write_register
    /*00002d74*/ MOV         R0, R4 
    /*00002d78*/ MOV         R2, #1 
    /*00002d7c*/ MOVW        R1, #13191                  @ 0x3387 
    /*00002d80*/ BL          mn34220_write_register
    /*00002d84*/ MOV         R0, R4 
    /*00002d88*/ MOV         R2, #13 
    /*00002d8c*/ MOVW        R1, #13192                  @ 0x3388 
    /*00002d90*/ BL          mn34220_write_register
    /*00002d94*/ MOV         R0, R4 
    /*00002d98*/ MOV         R2, #0 
    /*00002d9c*/ MOVW        R1, #13193                  @ 0x3389 
    /*00002da0*/ BL          mn34220_write_register
    /*00002da4*/ MOV         R0, R4 
    /*00002da8*/ MOV         R2, #0 
    /*00002dac*/ MOVW        R1, #13194                  @ 0x338a 
    /*00002db0*/ BL          mn34220_write_register
    /*00002db4*/ MOV         R0, R4 
    /*00002db8*/ MOV         R2, #1 
    /*00002dbc*/ MOVW        R1, #13195                  @ 0x338b 
    /*00002dc0*/ BL          mn34220_write_register
    /*00002dc4*/ MOV         R0, R4 
    /*00002dc8*/ MOV         R2, #4 
    /*00002dcc*/ MOVW        R1, #13196                  @ 0x338c 
    /*00002dd0*/ BL          mn34220_write_register
    /*00002dd4*/ MOV         R0, R4 
    /*00002dd8*/ MOV         R2, #8 
    /*00002ddc*/ MOVW        R1, #13197                  @ 0x338d 
    /*00002de0*/ BL          mn34220_write_register
    /*00002de4*/ MOV         R0, R4 
    /*00002de8*/ MOV         R2, #10 
    /*00002dec*/ MOVW        R1, #13198                  @ 0x338e 
    /*00002df0*/ BL          mn34220_write_register
    /*00002df4*/ MOV         R0, R4 
    /*00002df8*/ MOV         R2, #9 
    /*00002dfc*/ MOVW        R1, #13199                  @ 0x338f 
    /*00002e00*/ BL          mn34220_write_register
    /*00002e04*/ MOV         R0, R4 
    /*00002e08*/ MOV         R2, #2 
    /*00002e0c*/ MOVW        R1, #13200                  @ 0x3390 
    /*00002e10*/ BL          mn34220_write_register
    /*00002e14*/ MOV         R0, R4 
    /*00002e18*/ MOV         R2, #15 
    /*00002e1c*/ MOVW        R1, #13201                  @ 0x3391 
    /*00002e20*/ BL          mn34220_write_register
    /*00002e24*/ MOV         R0, R4 
    /*00002e28*/ MOV         R2, #4 
    /*00002e2c*/ MOVW        R1, #13202                  @ 0x3392 
    /*00002e30*/ BL          mn34220_write_register
    /*00002e34*/ MOV         R0, R4 
    /*00002e38*/ MOV         R2, #9 
    /*00002e3c*/ MOVW        R1, #13203                  @ 0x3393 
    /*00002e40*/ BL          mn34220_write_register
    /*00002e44*/ MOV         R0, R4 
    /*00002e48*/ MOV         R2, #1 
    /*00002e4c*/ MOVW        R1, #13204                  @ 0x3394 
    /*00002e50*/ BL          mn34220_write_register
    /*00002e54*/ MOV         R0, R4 
    /*00002e58*/ MOV         R2, #10 
    /*00002e5c*/ MOVW        R1, #13205                  @ 0x3395 
    /*00002e60*/ BL          mn34220_write_register
    /*00002e64*/ MOV         R0, R4 
    /*00002e68*/ MOV         R2, #7 
    /*00002e6c*/ MOVW        R1, #13206                  @ 0x3396 
    /*00002e70*/ BL          mn34220_write_register
    /*00002e74*/ MOV         R0, R4 
    /*00002e78*/ MOV         R2, #24 
    /*00002e7c*/ MOVW        R1, #13207                  @ 0x3397 
    /*00002e80*/ BL          mn34220_write_register
    /*00002e84*/ MOV         R0, R4 
    /*00002e88*/ MOV         R2, #0 
    /*00002e8c*/ MOVW        R1, #13208                  @ 0x3398 
    /*00002e90*/ BL          mn34220_write_register
    /*00002e94*/ MOV         R0, R4 
    /*00002e98*/ MOV         R2, #1 
    /*00002e9c*/ MOVW        R1, #13209                  @ 0x3399 
    /*00002ea0*/ BL          mn34220_write_register
    /*00002ea4*/ MOV         R0, R4 
    /*00002ea8*/ MOV         R2, #1 
    /*00002eac*/ MOVW        R1, #13210                  @ 0x339a 
    /*00002eb0*/ BL          mn34220_write_register
    /*00002eb4*/ MOV         R0, R4 
    /*00002eb8*/ MOV         R2, #8 
    /*00002ebc*/ MOVW        R1, #13211                  @ 0x339b 
    /*00002ec0*/ BL          mn34220_write_register
    /*00002ec4*/ MOV         R0, R4 
    /*00002ec8*/ MOV         R2, #13 
    /*00002ecc*/ MOVW        R1, #13212                  @ 0x339c 
    /*00002ed0*/ BL          mn34220_write_register
    /*00002ed4*/ MOV         R0, R4 
    /*00002ed8*/ MOV         R2, #1 
    /*00002edc*/ MOVW        R1, #13213                  @ 0x339d 
    /*00002ee0*/ BL          mn34220_write_register
    /*00002ee4*/ MOV         R0, R4 
    /*00002ee8*/ MOV         R2, #0 
    /*00002eec*/ MOVW        R1, #13214                  @ 0x339e 
    /*00002ef0*/ BL          mn34220_write_register
    /*00002ef4*/ MOV         R0, R4 
    /*00002ef8*/ MOV         R2, #1 
    /*00002efc*/ MOVW        R1, #13215                  @ 0x339f 
    /*00002f00*/ BL          mn34220_write_register
    /*00002f04*/ MOV         R0, R4 
    /*00002f08*/ MOV         R2, #0 
    /*00002f0c*/ MOVW        R1, #13216                  @ 0x33a0 
    /*00002f10*/ BL          mn34220_write_register
    /*00002f14*/ MOV         R0, R4 
    /*00002f18*/ MOV         R2, #1 
    /*00002f1c*/ MOVW        R1, #13217                  @ 0x33a1 
    /*00002f20*/ BL          mn34220_write_register
    /*00002f24*/ MOV         R0, R4 
    /*00002f28*/ MOV         R2, #1 
    /*00002f2c*/ MOVW        R1, #13218                  @ 0x33a2 
    /*00002f30*/ BL          mn34220_write_register
    /*00002f34*/ MOV         R0, R4 
    /*00002f38*/ MOV         R2, #1 
    /*00002f3c*/ MOVW        R1, #13219                  @ 0x33a3 
    /*00002f40*/ BL          mn34220_write_register
    /*00002f44*/ MOV         R0, R4 
    /*00002f48*/ MOV         R2, #0 
    /*00002f4c*/ MOVW        R1, #13220                  @ 0x33a4 
    /*00002f50*/ BL          mn34220_write_register
    /*00002f54*/ MOV         R0, R4 
    /*00002f58*/ MOV         R2, #3 
    /*00002f5c*/ MOVW        R1, #13221                  @ 0x33a5 
    /*00002f60*/ BL          mn34220_write_register
    /*00002f64*/ MOV         R0, R4 
    /*00002f68*/ MOV         R2, #6 
    /*00002f6c*/ MOVW        R1, #13222                  @ 0x33a6 
    /*00002f70*/ BL          mn34220_write_register
    /*00002f74*/ MOV         R0, R4 
    /*00002f78*/ MOV         R2, #0 
    /*00002f7c*/ MOVW        R1, #13223                  @ 0x33a7 
    /*00002f80*/ BL          mn34220_write_register
    /*00002f84*/ MOV         R0, R4 
    /*00002f88*/ MOV         R2, #0 
    /*00002f8c*/ MOVW        R1, #13224                  @ 0x33a8 
    /*00002f90*/ BL          mn34220_write_register
    /*00002f94*/ MOV         R0, R4 
    /*00002f98*/ MOV         R2, #0 
    /*00002f9c*/ MOVW        R1, #13225                  @ 0x33a9 
    /*00002fa0*/ BL          mn34220_write_register
    /*00002fa4*/ MOV         R0, R4 
    /*00002fa8*/ MOV         R2, #0 
    /*00002fac*/ MOVW        R1, #13226                  @ 0x33aa 
    /*00002fb0*/ BL          mn34220_write_register
    /*00002fb4*/ MOV         R0, R4 
    /*00002fb8*/ MOV         R2, #0 
    /*00002fbc*/ MOVW        R1, #13227                  @ 0x33ab 
    /*00002fc0*/ BL          mn34220_write_register
    /*00002fc4*/ MOV         R0, R4 
    /*00002fc8*/ MOV         R2, #3 
    /*00002fcc*/ MOVW        R1, #13228                  @ 0x33ac 
    /*00002fd0*/ BL          mn34220_write_register
    /*00002fd4*/ MOV         R0, R4 
    /*00002fd8*/ MOV         R2, #20 
    /*00002fdc*/ MOVW        R1, #13229                  @ 0x33ad 
    /*00002fe0*/ BL          mn34220_write_register
    /*00002fe4*/ MOV         R0, R4 
    /*00002fe8*/ MOV         R2, #63                     @ 0x3f 
    /*00002fec*/ MOVW        R1, #13230                  @ 0x33ae 
    /*00002ff0*/ BL          mn34220_write_register
    /*00002ff4*/ MOV         R0, R4 
    /*00002ff8*/ MOV         R2, #17 
    /*00002ffc*/ MOVW        R1, #13231                  @ 0x33af 
    /*00003000*/ BL          mn34220_write_register
    /*00003004*/ MOV         R0, R4 
    /*00003008*/ MOV         R2, #10 
    /*0000300c*/ MOVW        R1, #13232                  @ 0x33b0 
    /*00003010*/ BL          mn34220_write_register
    /*00003014*/ MOV         R0, R4 
    /*00003018*/ MOV         R2, #26 
    /*0000301c*/ MOVW        R1, #13233                  @ 0x33b1 
    /*00003020*/ BL          mn34220_write_register
    /*00003024*/ MOV         R0, R4 
    /*00003028*/ MOV         R2, #0 
    /*0000302c*/ MOVW        R1, #13234                  @ 0x33b2 
    /*00003030*/ BL          mn34220_write_register
    /*00003034*/ MOV         R0, R4 
    /*00003038*/ MOV         R2, #0 
    /*0000303c*/ MOVW        R1, #13235                  @ 0x33b3 
    /*00003040*/ BL          mn34220_write_register
    /*00003044*/ MOV         R0, R4 
    /*00003048*/ MOV         R2, #0 
    /*0000304c*/ MOVW        R1, #13236                  @ 0x33b4 
    /*00003050*/ BL          mn34220_write_register
    /*00003054*/ MOV         R0, R4 
    /*00003058*/ MOV         R2, #0 
    /*0000305c*/ MOVW        R1, #13237                  @ 0x33b5 
    /*00003060*/ BL          mn34220_write_register
    /*00003064*/ MOV         R0, R4 
    /*00003068*/ MOV         R2, #0 
    /*0000306c*/ MOV         R1, #12288                  @ 0x3000 
    /*00003070*/ BL          mn34220_write_register
    /*00003074*/ MOV         R0, R4 
    /*00003078*/ MOV         R2, #211                    @ 0xd3 
    /*0000307c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00003080*/ BL          mn34220_write_register
    /*00003084*/ MOV         R0, R4 
    /*00003088*/ MOV         R2, #1 
    /*0000308c*/ MOV         R1, #256                    @ 0x100 
    /*00003090*/ BL          mn34220_write_register
    /*00003094*/ MOV         R0, R4 
    /*00003098*/ MOV         R2, #0 
    /*0000309c*/ MOVW        R1, #257                    @ 0x101 
    /*000030a0*/ POP         {R4, LR} 
    mn34220_wdr_720p_2to1_init_x30a4:
    /*000030a4*/ B           mn34220_wdr_720p_2to1_init_x30a4@ mn34220_write_register [R_ARM_JUMP24: .text.mn34220_write_register+0x0] 
FUNC_END mn34220_wdr_720p_2to1_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_wdr_720p_3to1_init
@ Size: 0x778
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_wdr_720p_3to1_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #1 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000010*/ BL          mn34220_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #0 
    /*0000001c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000020*/ BL          mn34220_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #2 
    /*0000002c*/ MOVW        R1, #773                    @ 0x305 
    /*00000030*/ BL          mn34220_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #48                     @ 0x30 
    /*0000003c*/ MOVW        R1, #775                    @ 0x307 
    /*00000040*/ BL          mn34220_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #0 
    /*0000004c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000050*/ BL          mn34220_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #3 
    /*0000005c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000060*/ BL          mn34220_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #12 
    /*0000006c*/ MOVW        R1, #274                    @ 0x112 
    /*00000070*/ BL          mn34220_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #12 
    /*0000007c*/ MOVW        R1, #275                    @ 0x113 
    /*00000080*/ BL          mn34220_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #20 
    /*0000008c*/ MOVW        R1, #12295                  @ 0x3007 
    /*00000090*/ BL          mn34220_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #144                    @ 0x90 
    /*0000009c*/ MOVW        R1, #12296                  @ 0x3008 
    /*000000a0*/ BL          mn34220_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #0 
    /*000000ac*/ MOVW        R1, #12299                  @ 0x300b 
    /*000000b0*/ BL          mn34220_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #67                     @ 0x43 
    /*000000bc*/ MOVW        R1, #12312                  @ 0x3018 
    /*000000c0*/ BL          mn34220_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #16 
    /*000000cc*/ MOVW        R1, #12313                  @ 0x3019 
    /*000000d0*/ BL          mn34220_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #185                    @ 0xb9 
    /*000000dc*/ MOVW        R1, #12314                  @ 0x301a 
    /*000000e0*/ BL          mn34220_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #0 
    /*000000ec*/ MOV         R1, #12288                  @ 0x3000 
    /*000000f0*/ BL          mn34220_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #83                     @ 0x53 
    /*000000fc*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000100*/ BL          mn34220_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #0 
    /*0000010c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000110*/ BL          mn34220_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #0 
    /*0000011c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000120*/ BL          mn34220_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #236                    @ 0xec 
    /*0000012c*/ MOVW        R1, #515                    @ 0x203 
    /*00000130*/ BL          mn34220_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #2 
    /*0000013c*/ MOV         R1, #832                    @ 0x340 
    /*00000140*/ BL          mn34220_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #238                    @ 0xee 
    /*0000014c*/ MOVW        R1, #833                    @ 0x341 
    /*00000150*/ BL          mn34220_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #25 
    /*0000015c*/ MOVW        R1, #834                    @ 0x342 
    /*00000160*/ BL          mn34220_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #200                    @ 0xc8 
    /*0000016c*/ MOVW        R1, #835                    @ 0x343 
    /*00000170*/ BL          mn34220_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #242                    @ 0xf2 
    /*0000017c*/ MOVW        R1, #839                    @ 0x347 
    /*00000180*/ BL          mn34220_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #3 
    /*0000018c*/ MOVW        R1, #842                    @ 0x34a 
    /*00000190*/ BL          mn34220_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #201                    @ 0xc9 
    /*0000019c*/ MOVW        R1, #843                    @ 0x34b 
    /*000001a0*/ BL          mn34220_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #2 
    /*000001ac*/ MOVW        R1, #846                    @ 0x34e 
    /*000001b0*/ BL          mn34220_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #216                    @ 0xd8 
    /*000001bc*/ MOVW        R1, #847                    @ 0x34f 
    /*000001c0*/ BL          mn34220_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #0 
    /*000001cc*/ MOVW        R1, #12342                  @ 0x3036 
    /*000001d0*/ BL          mn34220_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #46                     @ 0x2e 
    /*000001dc*/ MOVW        R1, #12345                  @ 0x3039 
    /*000001e0*/ BL          mn34220_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #18 
    /*000001ec*/ MOVW        R1, #12353                  @ 0x3041 
    /*000001f0*/ BL          mn34220_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #15 
    /*000001fc*/ MOVW        R1, #12376                  @ 0x3058 
    /*00000200*/ BL          mn34220_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #12 
    /*0000020c*/ MOVW        R1, #12398                  @ 0x306e 
    /*00000210*/ BL          mn34220_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #0 
    /*0000021c*/ MOVW        R1, #12399                  @ 0x306f 
    /*00000220*/ BL          mn34220_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #1 
    /*0000022c*/ MOVW        R1, #12404                  @ 0x3074 
    /*00000230*/ BL          mn34220_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #0 
    /*0000023c*/ MOVW        R1, #12440                  @ 0x3098 
    /*00000240*/ BL          mn34220_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #64                     @ 0x40 
    /*0000024c*/ MOVW        R1, #12441                  @ 0x3099 
    /*00000250*/ BL          mn34220_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #16 
    /*0000025c*/ MOVW        R1, #12442                  @ 0x309a 
    /*00000260*/ BL          mn34220_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #2 
    /*0000026c*/ MOVW        R1, #12545                  @ 0x3101 
    /*00000270*/ BL          mn34220_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #4 
    /*0000027c*/ MOVW        R1, #12548                  @ 0x3104 
    /*00000280*/ BL          mn34220_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #0 
    /*0000028c*/ MOVW        R1, #12550                  @ 0x3106 
    /*00000290*/ BL          mn34220_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #192                    @ 0xc0 
    /*0000029c*/ MOVW        R1, #12551                  @ 0x3107 
    /*000002a0*/ BL          mn34220_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #32 
    /*000002ac*/ MOVW        R1, #12587                  @ 0x312b 
    /*000002b0*/ BL          mn34220_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #32 
    /*000002bc*/ MOVW        R1, #12589                  @ 0x312d 
    /*000002c0*/ BL          mn34220_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #32 
    /*000002cc*/ MOVW        R1, #12591                  @ 0x312f 
    /*000002d0*/ BL          mn34220_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #112                    @ 0x70 
    /*000002dc*/ MOVW        R1, #12609                  @ 0x3141 
    /*000002e0*/ BL          mn34220_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #1 
    /*000002ec*/ MOVW        R1, #12611                  @ 0x3143 
    /*000002f0*/ BL          mn34220_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #3 
    /*000002fc*/ MOVW        R1, #12612                  @ 0x3144 
    /*00000300*/ BL          mn34220_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #2 
    /*0000030c*/ MOVW        R1, #12613                  @ 0x3145 
    /*00000310*/ BL          mn34220_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #0 
    /*0000031c*/ MOVW        R1, #12615                  @ 0x3147 
    /*00000320*/ BL          mn34220_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #0 
    /*0000032c*/ MOVW        R1, #12616                  @ 0x3148 
    /*00000330*/ BL          mn34220_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #0 
    /*0000033c*/ MOVW        R1, #12617                  @ 0x3149 
    /*00000340*/ BL          mn34220_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #3 
    /*0000034c*/ MOVW        R1, #12618                  @ 0x314a 
    /*00000350*/ BL          mn34220_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #1 
    /*0000035c*/ MOVW        R1, #12619                  @ 0x314b 
    /*00000360*/ BL          mn34220_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #1 
    /*0000036c*/ MOVW        R1, #12620                  @ 0x314c 
    /*00000370*/ BL          mn34220_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #1 
    /*0000037c*/ MOVW        R1, #12621                  @ 0x314d 
    /*00000380*/ BL          mn34220_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #2 
    /*0000038c*/ MOVW        R1, #12622                  @ 0x314e 
    /*00000390*/ BL          mn34220_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #2 
    /*0000039c*/ MOVW        R1, #12623                  @ 0x314f 
    /*000003a0*/ BL          mn34220_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #2 
    /*000003ac*/ MOVW        R1, #12624                  @ 0x3150 
    /*000003b0*/ BL          mn34220_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #1 
    /*000003bc*/ MOVW        R1, #12626                  @ 0x3152 
    /*000003c0*/ BL          mn34220_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #227                    @ 0xe3 
    /*000003cc*/ MOVW        R1, #12627                  @ 0x3153 
    /*000003d0*/ BL          mn34220_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #17 
    /*000003dc*/ MOVW        R1, #12629                  @ 0x3155 
    /*000003e0*/ BL          mn34220_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #48                     @ 0x30 
    /*000003ec*/ MOVW        R1, #12631                  @ 0x3157 
    /*000003f0*/ BL          mn34220_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #51                     @ 0x33 
    /*000003fc*/ MOVW        R1, #12633                  @ 0x3159 
    /*00000400*/ BL          mn34220_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #54                     @ 0x36 
    /*0000040c*/ MOVW        R1, #12635                  @ 0x315b 
    /*00000410*/ BL          mn34220_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #53                     @ 0x35 
    /*0000041c*/ MOVW        R1, #12637                  @ 0x315d 
    /*00000420*/ BL          mn34220_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #60                     @ 0x3c 
    /*0000042c*/ MOVW        R1, #12639                  @ 0x315f 
    /*00000430*/ BL          mn34220_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #63                     @ 0x3f 
    /*0000043c*/ MOVW        R1, #12641                  @ 0x3161 
    /*00000440*/ BL          mn34220_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #58                     @ 0x3a 
    /*0000044c*/ MOVW        R1, #12643                  @ 0x3163 
    /*00000450*/ BL          mn34220_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #57                     @ 0x39 
    /*0000045c*/ MOVW        R1, #12645                  @ 0x3165 
    /*00000460*/ BL          mn34220_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #40                     @ 0x28 
    /*0000046c*/ MOVW        R1, #12647                  @ 0x3167 
    /*00000470*/ BL          mn34220_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #43                     @ 0x2b 
    /*0000047c*/ MOVW        R1, #12649                  @ 0x3169 
    /*00000480*/ BL          mn34220_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #46                     @ 0x2e 
    /*0000048c*/ MOVW        R1, #12651                  @ 0x316b 
    /*00000490*/ BL          mn34220_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #45                     @ 0x2d 
    /*0000049c*/ MOVW        R1, #12653                  @ 0x316d 
    /*000004a0*/ BL          mn34220_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #34                     @ 0x22 
    /*000004ac*/ MOVW        R1, #12655                  @ 0x316f 
    /*000004b0*/ BL          mn34220_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #34                     @ 0x22 
    /*000004bc*/ MOVW        R1, #12657                  @ 0x3171 
    /*000004c0*/ BL          mn34220_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #97                     @ 0x61 
    /*000004cc*/ MOVW        R1, #12659                  @ 0x3173 
    /*000004d0*/ BL          mn34220_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #128                    @ 0x80 
    /*000004dc*/ MOVW        R1, #12661                  @ 0x3175 
    /*000004e0*/ BL          mn34220_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #32 
    /*000004ec*/ MOVW        R1, #12686                  @ 0x318e 
    /*000004f0*/ BL          mn34220_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #112                    @ 0x70 
    /*000004fc*/ MOVW        R1, #12687                  @ 0x318f 
    /*00000500*/ BL          mn34220_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #8 
    /*0000050c*/ MOVW        R1, #12694                  @ 0x3196 
    /*00000510*/ BL          mn34220_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #3 
    /*0000051c*/ MOVW        R1, #12796                  @ 0x31fc 
    /*00000520*/ BL          mn34220_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #6 
    /*0000052c*/ MOVW        R1, #12798                  @ 0x31fe 
    /*00000530*/ BL          mn34220_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #1 
    /*0000053c*/ MOVW        R1, #12801                  @ 0x3201 
    /*00000540*/ BL          mn34220_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #3 
    /*0000054c*/ MOVW        R1, #12802                  @ 0x3202 
    /*00000550*/ BL          mn34220_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #53                     @ 0x35 
    /*0000055c*/ MOVW        R1, #12803                  @ 0x3203 
    /*00000560*/ BL          mn34220_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #112                    @ 0x70 
    /*0000056c*/ MOVW        R1, #12860                  @ 0x323c 
    /*00000570*/ BL          mn34220_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #0 
    /*0000057c*/ MOVW        R1, #12862                  @ 0x323e 
    /*00000580*/ BL          mn34220_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #117                    @ 0x75 
    /*0000058c*/ MOVW        R1, #12867                  @ 0x3243 
    /*00000590*/ BL          mn34220_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #3 
    /*0000059c*/ MOVW        R1, #12870                  @ 0x3246 
    /*000005a0*/ BL          mn34220_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #59                     @ 0x3b 
    /*000005ac*/ MOVW        R1, #12871                  @ 0x3247 
    /*000005b0*/ BL          mn34220_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #1 
    /*000005bc*/ MOVW        R1, #12872                  @ 0x3248 
    /*000005c0*/ BL          mn34220_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #171                    @ 0xab 
    /*000005cc*/ MOVW        R1, #12873                  @ 0x3249 
    /*000005d0*/ BL          mn34220_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #48                     @ 0x30 
    /*000005dc*/ MOVW        R1, #12874                  @ 0x324a 
    /*000005e0*/ BL          mn34220_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #27 
    /*000005ec*/ MOVW        R1, #12875                  @ 0x324b 
    /*000005f0*/ BL          mn34220_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #2 
    /*000005fc*/ MOVW        R1, #12876                  @ 0x324c 
    /*00000600*/ BL          mn34220_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #123                    @ 0x7b 
    /*0000060c*/ MOVW        R1, #12883                  @ 0x3253 
    /*00000610*/ BL          mn34220_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #50                     @ 0x32 
    /*0000061c*/ MOVW        R1, #12886                  @ 0x3256 
    /*00000620*/ BL          mn34220_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #3 
    /*0000062c*/ MOVW        R1, #12888                  @ 0x3258 
    /*00000630*/ BL          mn34220_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #4 
    /*0000063c*/ MOVW        R1, #12889                  @ 0x3259 
    /*00000640*/ BL          mn34220_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #20 
    /*0000064c*/ MOVW        R1, #12890                  @ 0x325a 
    /*00000650*/ BL          mn34220_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #12 
    /*0000065c*/ MOVW        R1, #12914                  @ 0x3272 
    /*00000660*/ BL          mn34220_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #48                     @ 0x30 
    /*0000066c*/ MOV         R1, #12928                  @ 0x3280 
    /*00000670*/ BL          mn34220_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #1 
    /*0000067c*/ MOVW        R1, #12936                  @ 0x3288 
    /*00000680*/ BL          mn34220_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #5 
    /*0000068c*/ MOVW        R1, #13070                  @ 0x330e 
    /*00000690*/ BL          mn34220_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #2 
    /*0000069c*/ MOVW        R1, #13072                  @ 0x3310 
    /*000006a0*/ BL          mn34220_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #31 
    /*000006ac*/ MOVW        R1, #13077                  @ 0x3315 
    /*000006b0*/ BL          mn34220_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #3 
    /*000006bc*/ MOVW        R1, #13082                  @ 0x331a 
    /*000006c0*/ BL          mn34220_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #3 
    /*000006cc*/ MOVW        R1, #13083                  @ 0x331b 
    /*000006d0*/ BL          mn34220_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #3 
    /*000006dc*/ MOVW        R1, #13113                  @ 0x3339 
    /*000006e0*/ BL          mn34220_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #2 
    /*000006ec*/ MOVW        R1, #13163                  @ 0x336b 
    /*000006f0*/ BL          mn34220_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #1 
    /*000006fc*/ MOVW        R1, #13215                  @ 0x339f 
    /*00000700*/ BL          mn34220_write_register
    /*00000704*/ MOV         R0, R4 
    /*00000708*/ MOV         R2, #1 
    /*0000070c*/ MOVW        R1, #13218                  @ 0x33a2 
    /*00000710*/ BL          mn34220_write_register
    /*00000714*/ MOV         R0, R4 
    /*00000718*/ MOV         R2, #1 
    /*0000071c*/ MOVW        R1, #13219                  @ 0x33a3 
    /*00000720*/ BL          mn34220_write_register
    /*00000724*/ MOV         R0, R4 
    /*00000728*/ MOV         R2, #0 
    /*0000072c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000730*/ BL          mn34220_write_register
    /*00000734*/ MOV         R0, R4 
    /*00000738*/ MOV         R2, #211                    @ 0xd3 
    /*0000073c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000740*/ BL          mn34220_write_register
    /*00000744*/ MOV         R0, R4 
    /*00000748*/ MOV         R2, #1 
    /*0000074c*/ MOV         R1, #256                    @ 0x100 
    /*00000750*/ BL          mn34220_write_register
    /*00000754*/ MOV         R0, R4 
    /*00000758*/ MOV         R2, #0 
    /*0000075c*/ MOVW        R1, #257                    @ 0x101 
    /*00000760*/ BL          mn34220_write_register
    /*00000764*/ LDR         R0, WORD_0774               @ LDR         R0, [PC, #8]                @ 0x0000000000000774 
    /*00000768*/ POP         {R4, LR} 
    /*0000076c*/ ADD         R0, PC, R0 
    mn34220_wdr_720p_3to1_init_x770:
    /*00000770*/ B           mn34220_wdr_720p_3to1_init_x770@ puts [R_ARM_JUMP24: +0x0] 
    /*00000774*/ WORD_0774: .word 0x00000000
FUNC_END mn34220_wdr_720p_3to1_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_wdr_720p_4to1_init
@ Size: 0x748
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_wdr_720p_4to1_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #1 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000010*/ BL          mn34220_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #0 
    /*0000001c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000020*/ BL          mn34220_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #2 
    /*0000002c*/ MOVW        R1, #773                    @ 0x305 
    /*00000030*/ BL          mn34220_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #48                     @ 0x30 
    /*0000003c*/ MOVW        R1, #775                    @ 0x307 
    /*00000040*/ BL          mn34220_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #0 
    /*0000004c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000050*/ BL          mn34220_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #3 
    /*0000005c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000060*/ BL          mn34220_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #12 
    /*0000006c*/ MOVW        R1, #274                    @ 0x112 
    /*00000070*/ BL          mn34220_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #12 
    /*0000007c*/ MOVW        R1, #275                    @ 0x113 
    /*00000080*/ BL          mn34220_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #20 
    /*0000008c*/ MOVW        R1, #12295                  @ 0x3007 
    /*00000090*/ BL          mn34220_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #144                    @ 0x90 
    /*0000009c*/ MOVW        R1, #12296                  @ 0x3008 
    /*000000a0*/ BL          mn34220_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #0 
    /*000000ac*/ MOVW        R1, #12299                  @ 0x300b 
    /*000000b0*/ BL          mn34220_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #67                     @ 0x43 
    /*000000bc*/ MOVW        R1, #12312                  @ 0x3018 
    /*000000c0*/ BL          mn34220_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #16 
    /*000000cc*/ MOVW        R1, #12313                  @ 0x3019 
    /*000000d0*/ BL          mn34220_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #185                    @ 0xb9 
    /*000000dc*/ MOVW        R1, #12314                  @ 0x301a 
    /*000000e0*/ BL          mn34220_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #0 
    /*000000ec*/ MOV         R1, #12288                  @ 0x3000 
    /*000000f0*/ BL          mn34220_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #83                     @ 0x53 
    /*000000fc*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000100*/ BL          mn34220_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #0 
    /*0000010c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000110*/ BL          mn34220_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #0 
    /*0000011c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000120*/ BL          mn34220_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #236                    @ 0xec 
    /*0000012c*/ MOVW        R1, #515                    @ 0x203 
    /*00000130*/ BL          mn34220_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #2 
    /*0000013c*/ MOV         R1, #832                    @ 0x340 
    /*00000140*/ BL          mn34220_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #238                    @ 0xee 
    /*0000014c*/ MOVW        R1, #833                    @ 0x341 
    /*00000150*/ BL          mn34220_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #34                     @ 0x22 
    /*0000015c*/ MOVW        R1, #834                    @ 0x342 
    /*00000160*/ BL          mn34220_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #96                     @ 0x60 
    /*0000016c*/ MOVW        R1, #835                    @ 0x343 
    /*00000170*/ BL          mn34220_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #242                    @ 0xf2 
    /*0000017c*/ MOVW        R1, #839                    @ 0x347 
    /*00000180*/ BL          mn34220_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #3 
    /*0000018c*/ MOVW        R1, #842                    @ 0x34a 
    /*00000190*/ BL          mn34220_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #201                    @ 0xc9 
    /*0000019c*/ MOVW        R1, #843                    @ 0x34b 
    /*000001a0*/ BL          mn34220_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #2 
    /*000001ac*/ MOVW        R1, #846                    @ 0x34e 
    /*000001b0*/ BL          mn34220_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #216                    @ 0xd8 
    /*000001bc*/ MOVW        R1, #847                    @ 0x34f 
    /*000001c0*/ BL          mn34220_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #0 
    /*000001cc*/ MOVW        R1, #12342                  @ 0x3036 
    /*000001d0*/ BL          mn34220_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #46                     @ 0x2e 
    /*000001dc*/ MOVW        R1, #12345                  @ 0x3039 
    /*000001e0*/ BL          mn34220_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #18 
    /*000001ec*/ MOVW        R1, #12353                  @ 0x3041 
    /*000001f0*/ BL          mn34220_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #15 
    /*000001fc*/ MOVW        R1, #12376                  @ 0x3058 
    /*00000200*/ BL          mn34220_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #12 
    /*0000020c*/ MOVW        R1, #12398                  @ 0x306e 
    /*00000210*/ BL          mn34220_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #0 
    /*0000021c*/ MOVW        R1, #12399                  @ 0x306f 
    /*00000220*/ BL          mn34220_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #1 
    /*0000022c*/ MOVW        R1, #12404                  @ 0x3074 
    /*00000230*/ BL          mn34220_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #0 
    /*0000023c*/ MOVW        R1, #12440                  @ 0x3098 
    /*00000240*/ BL          mn34220_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #64                     @ 0x40 
    /*0000024c*/ MOVW        R1, #12441                  @ 0x3099 
    /*00000250*/ BL          mn34220_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #16 
    /*0000025c*/ MOVW        R1, #12442                  @ 0x309a 
    /*00000260*/ BL          mn34220_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #3 
    /*0000026c*/ MOVW        R1, #12545                  @ 0x3101 
    /*00000270*/ BL          mn34220_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #4 
    /*0000027c*/ MOVW        R1, #12548                  @ 0x3104 
    /*00000280*/ BL          mn34220_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #0 
    /*0000028c*/ MOVW        R1, #12550                  @ 0x3106 
    /*00000290*/ BL          mn34220_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #192                    @ 0xc0 
    /*0000029c*/ MOVW        R1, #12551                  @ 0x3107 
    /*000002a0*/ BL          mn34220_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #32 
    /*000002ac*/ MOVW        R1, #12587                  @ 0x312b 
    /*000002b0*/ BL          mn34220_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #32 
    /*000002bc*/ MOVW        R1, #12589                  @ 0x312d 
    /*000002c0*/ BL          mn34220_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #32 
    /*000002cc*/ MOVW        R1, #12591                  @ 0x312f 
    /*000002d0*/ BL          mn34220_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #112                    @ 0x70 
    /*000002dc*/ MOVW        R1, #12609                  @ 0x3141 
    /*000002e0*/ BL          mn34220_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #1 
    /*000002ec*/ MOVW        R1, #12611                  @ 0x3143 
    /*000002f0*/ BL          mn34220_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #3 
    /*000002fc*/ MOVW        R1, #12612                  @ 0x3144 
    /*00000300*/ BL          mn34220_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #2 
    /*0000030c*/ MOVW        R1, #12613                  @ 0x3145 
    /*00000310*/ BL          mn34220_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #0 
    /*0000031c*/ MOVW        R1, #12615                  @ 0x3147 
    /*00000320*/ BL          mn34220_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #0 
    /*0000032c*/ MOVW        R1, #12616                  @ 0x3148 
    /*00000330*/ BL          mn34220_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #0 
    /*0000033c*/ MOVW        R1, #12617                  @ 0x3149 
    /*00000340*/ BL          mn34220_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #3 
    /*0000034c*/ MOVW        R1, #12618                  @ 0x314a 
    /*00000350*/ BL          mn34220_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #1 
    /*0000035c*/ MOVW        R1, #12619                  @ 0x314b 
    /*00000360*/ BL          mn34220_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #1 
    /*0000036c*/ MOVW        R1, #12620                  @ 0x314c 
    /*00000370*/ BL          mn34220_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #1 
    /*0000037c*/ MOVW        R1, #12621                  @ 0x314d 
    /*00000380*/ BL          mn34220_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #2 
    /*0000038c*/ MOVW        R1, #12622                  @ 0x314e 
    /*00000390*/ BL          mn34220_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #2 
    /*0000039c*/ MOVW        R1, #12623                  @ 0x314f 
    /*000003a0*/ BL          mn34220_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #2 
    /*000003ac*/ MOVW        R1, #12624                  @ 0x3150 
    /*000003b0*/ BL          mn34220_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #1 
    /*000003bc*/ MOVW        R1, #12626                  @ 0x3152 
    /*000003c0*/ BL          mn34220_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #227                    @ 0xe3 
    /*000003cc*/ MOVW        R1, #12627                  @ 0x3153 
    /*000003d0*/ BL          mn34220_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #17 
    /*000003dc*/ MOVW        R1, #12629                  @ 0x3155 
    /*000003e0*/ BL          mn34220_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #48                     @ 0x30 
    /*000003ec*/ MOVW        R1, #12631                  @ 0x3157 
    /*000003f0*/ BL          mn34220_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #51                     @ 0x33 
    /*000003fc*/ MOVW        R1, #12633                  @ 0x3159 
    /*00000400*/ BL          mn34220_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #54                     @ 0x36 
    /*0000040c*/ MOVW        R1, #12635                  @ 0x315b 
    /*00000410*/ BL          mn34220_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #53                     @ 0x35 
    /*0000041c*/ MOVW        R1, #12637                  @ 0x315d 
    /*00000420*/ BL          mn34220_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #60                     @ 0x3c 
    /*0000042c*/ MOVW        R1, #12639                  @ 0x315f 
    /*00000430*/ BL          mn34220_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #63                     @ 0x3f 
    /*0000043c*/ MOVW        R1, #12641                  @ 0x3161 
    /*00000440*/ BL          mn34220_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #58                     @ 0x3a 
    /*0000044c*/ MOVW        R1, #12643                  @ 0x3163 
    /*00000450*/ BL          mn34220_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #57                     @ 0x39 
    /*0000045c*/ MOVW        R1, #12645                  @ 0x3165 
    /*00000460*/ BL          mn34220_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #40                     @ 0x28 
    /*0000046c*/ MOVW        R1, #12647                  @ 0x3167 
    /*00000470*/ BL          mn34220_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #43                     @ 0x2b 
    /*0000047c*/ MOVW        R1, #12649                  @ 0x3169 
    /*00000480*/ BL          mn34220_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #46                     @ 0x2e 
    /*0000048c*/ MOVW        R1, #12651                  @ 0x316b 
    /*00000490*/ BL          mn34220_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #45                     @ 0x2d 
    /*0000049c*/ MOVW        R1, #12653                  @ 0x316d 
    /*000004a0*/ BL          mn34220_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #34                     @ 0x22 
    /*000004ac*/ MOVW        R1, #12655                  @ 0x316f 
    /*000004b0*/ BL          mn34220_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #34                     @ 0x22 
    /*000004bc*/ MOVW        R1, #12657                  @ 0x3171 
    /*000004c0*/ BL          mn34220_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #97                     @ 0x61 
    /*000004cc*/ MOVW        R1, #12659                  @ 0x3173 
    /*000004d0*/ BL          mn34220_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #128                    @ 0x80 
    /*000004dc*/ MOVW        R1, #12661                  @ 0x3175 
    /*000004e0*/ BL          mn34220_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #32 
    /*000004ec*/ MOVW        R1, #12686                  @ 0x318e 
    /*000004f0*/ BL          mn34220_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #112                    @ 0x70 
    /*000004fc*/ MOVW        R1, #12687                  @ 0x318f 
    /*00000500*/ BL          mn34220_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #8 
    /*0000050c*/ MOVW        R1, #12694                  @ 0x3196 
    /*00000510*/ BL          mn34220_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #3 
    /*0000051c*/ MOVW        R1, #12796                  @ 0x31fc 
    /*00000520*/ BL          mn34220_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #6 
    /*0000052c*/ MOVW        R1, #12798                  @ 0x31fe 
    /*00000530*/ BL          mn34220_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #112                    @ 0x70 
    /*0000053c*/ MOVW        R1, #12860                  @ 0x323c 
    /*00000540*/ BL          mn34220_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #0 
    /*0000054c*/ MOVW        R1, #12862                  @ 0x323e 
    /*00000550*/ BL          mn34220_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #117                    @ 0x75 
    /*0000055c*/ MOVW        R1, #12867                  @ 0x3243 
    /*00000560*/ BL          mn34220_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #3 
    /*0000056c*/ MOVW        R1, #12870                  @ 0x3246 
    /*00000570*/ BL          mn34220_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #59                     @ 0x3b 
    /*0000057c*/ MOVW        R1, #12871                  @ 0x3247 
    /*00000580*/ BL          mn34220_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #1 
    /*0000058c*/ MOVW        R1, #12872                  @ 0x3248 
    /*00000590*/ BL          mn34220_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #171                    @ 0xab 
    /*0000059c*/ MOVW        R1, #12873                  @ 0x3249 
    /*000005a0*/ BL          mn34220_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #48                     @ 0x30 
    /*000005ac*/ MOVW        R1, #12874                  @ 0x324a 
    /*000005b0*/ BL          mn34220_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #27 
    /*000005bc*/ MOVW        R1, #12875                  @ 0x324b 
    /*000005c0*/ BL          mn34220_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #2 
    /*000005cc*/ MOVW        R1, #12876                  @ 0x324c 
    /*000005d0*/ BL          mn34220_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #123                    @ 0x7b 
    /*000005dc*/ MOVW        R1, #12883                  @ 0x3253 
    /*000005e0*/ BL          mn34220_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #50                     @ 0x32 
    /*000005ec*/ MOVW        R1, #12886                  @ 0x3256 
    /*000005f0*/ BL          mn34220_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #3 
    /*000005fc*/ MOVW        R1, #12888                  @ 0x3258 
    /*00000600*/ BL          mn34220_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #4 
    /*0000060c*/ MOVW        R1, #12889                  @ 0x3259 
    /*00000610*/ BL          mn34220_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #20 
    /*0000061c*/ MOVW        R1, #12890                  @ 0x325a 
    /*00000620*/ BL          mn34220_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #12 
    /*0000062c*/ MOVW        R1, #12914                  @ 0x3272 
    /*00000630*/ BL          mn34220_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #48                     @ 0x30 
    /*0000063c*/ MOV         R1, #12928                  @ 0x3280 
    /*00000640*/ BL          mn34220_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #1 
    /*0000064c*/ MOVW        R1, #12936                  @ 0x3288 
    /*00000650*/ BL          mn34220_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #5 
    /*0000065c*/ MOVW        R1, #13070                  @ 0x330e 
    /*00000660*/ BL          mn34220_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #2 
    /*0000066c*/ MOVW        R1, #13072                  @ 0x3310 
    /*00000670*/ BL          mn34220_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #31 
    /*0000067c*/ MOVW        R1, #13077                  @ 0x3315 
    /*00000680*/ BL          mn34220_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #3 
    /*0000068c*/ MOVW        R1, #13082                  @ 0x331a 
    /*00000690*/ BL          mn34220_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #3 
    /*0000069c*/ MOVW        R1, #13083                  @ 0x331b 
    /*000006a0*/ BL          mn34220_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #3 
    /*000006ac*/ MOVW        R1, #13113                  @ 0x3339 
    /*000006b0*/ BL          mn34220_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #2 
    /*000006bc*/ MOVW        R1, #13163                  @ 0x336b 
    /*000006c0*/ BL          mn34220_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #1 
    /*000006cc*/ MOVW        R1, #13215                  @ 0x339f 
    /*000006d0*/ BL          mn34220_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #1 
    /*000006dc*/ MOVW        R1, #13218                  @ 0x33a2 
    /*000006e0*/ BL          mn34220_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #1 
    /*000006ec*/ MOVW        R1, #13219                  @ 0x33a3 
    /*000006f0*/ BL          mn34220_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #0 
    /*000006fc*/ MOV         R1, #12288                  @ 0x3000 
    /*00000700*/ BL          mn34220_write_register
    /*00000704*/ MOV         R0, R4 
    /*00000708*/ MOV         R2, #211                    @ 0xd3 
    /*0000070c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000710*/ BL          mn34220_write_register
    /*00000714*/ MOV         R0, R4 
    /*00000718*/ MOV         R2, #1 
    /*0000071c*/ MOV         R1, #256                    @ 0x100 
    /*00000720*/ BL          mn34220_write_register
    /*00000724*/ MOV         R0, R4 
    /*00000728*/ MOV         R2, #0 
    /*0000072c*/ MOVW        R1, #257                    @ 0x101 
    /*00000730*/ BL          mn34220_write_register
    /*00000734*/ LDR         R0, WORD_0744               @ LDR         R0, [PC, #8]                @ 0x0000000000000744 
    /*00000738*/ POP         {R4, LR} 
    /*0000073c*/ ADD         R0, PC, R0 
    mn34220_wdr_720p_4to1_init_x740:
    /*00000740*/ B           mn34220_wdr_720p_4to1_init_x740@ puts [R_ARM_JUMP24: +0x0] 
    /*00000744*/ WORD_0744: .word 0x00000000
FUNC_END mn34220_wdr_720p_4to1_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_wdr_1080p_4to1_init
@ Size: 0x718
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_wdr_1080p_4to1_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #1 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000010*/ BL          mn34220_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #0 
    /*0000001c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000020*/ BL          mn34220_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #2 
    /*0000002c*/ MOVW        R1, #773                    @ 0x305 
    /*00000030*/ BL          mn34220_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #48                     @ 0x30 
    /*0000003c*/ MOVW        R1, #775                    @ 0x307 
    /*00000040*/ BL          mn34220_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #0 
    /*0000004c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000050*/ BL          mn34220_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #3 
    /*0000005c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000060*/ BL          mn34220_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #12 
    /*0000006c*/ MOVW        R1, #274                    @ 0x112 
    /*00000070*/ BL          mn34220_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #12 
    /*0000007c*/ MOVW        R1, #275                    @ 0x113 
    /*00000080*/ BL          mn34220_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #4 
    /*0000008c*/ MOVW        R1, #12292                  @ 0x3004 
    /*00000090*/ BL          mn34220_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #0 
    /*0000009c*/ MOVW        R1, #12296                  @ 0x3008 
    /*000000a0*/ BL          mn34220_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #0 
    /*000000ac*/ MOVW        R1, #12299                  @ 0x300b 
    /*000000b0*/ BL          mn34220_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #67                     @ 0x43 
    /*000000bc*/ MOVW        R1, #12312                  @ 0x3018 
    /*000000c0*/ BL          mn34220_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #16 
    /*000000cc*/ MOVW        R1, #12313                  @ 0x3019 
    /*000000d0*/ BL          mn34220_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #185                    @ 0xb9 
    /*000000dc*/ MOVW        R1, #12314                  @ 0x301a 
    /*000000e0*/ BL          mn34220_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #31 
    /*000000ec*/ MOVW        R1, #12319                  @ 0x301f 
    /*000000f0*/ BL          mn34220_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #0 
    /*000000fc*/ MOV         R1, #12288                  @ 0x3000 
    /*00000100*/ BL          mn34220_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #83                     @ 0x53 
    /*0000010c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000110*/ BL          mn34220_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #0 
    /*0000011c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000120*/ BL          mn34220_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #0 
    /*0000012c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000130*/ BL          mn34220_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #3 
    /*0000013c*/ MOVW        R1, #514                    @ 0x202 
    /*00000140*/ BL          mn34220_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #99                     @ 0x63 
    /*0000014c*/ MOVW        R1, #515                    @ 0x203 
    /*00000150*/ BL          mn34220_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #34                     @ 0x22 
    /*0000015c*/ MOVW        R1, #834                    @ 0x342 
    /*00000160*/ BL          mn34220_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #96                     @ 0x60 
    /*0000016c*/ MOVW        R1, #835                    @ 0x343 
    /*00000170*/ BL          mn34220_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #4 
    /*0000017c*/ MOVW        R1, #12336                  @ 0x3030 
    /*00000180*/ BL          mn34220_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #0 
    /*0000018c*/ MOVW        R1, #12342                  @ 0x3036 
    /*00000190*/ BL          mn34220_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #46                     @ 0x2e 
    /*0000019c*/ MOVW        R1, #12345                  @ 0x3039 
    /*000001a0*/ BL          mn34220_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #18 
    /*000001ac*/ MOVW        R1, #12353                  @ 0x3041 
    /*000001b0*/ BL          mn34220_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #3 
    /*000001bc*/ MOVW        R1, #12376                  @ 0x3058 
    /*000001c0*/ BL          mn34220_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #9 
    /*000001cc*/ MOVW        R1, #12398                  @ 0x306e 
    /*000001d0*/ BL          mn34220_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #0 
    /*000001dc*/ MOVW        R1, #12399                  @ 0x306f 
    /*000001e0*/ BL          mn34220_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #1 
    /*000001ec*/ MOVW        R1, #12404                  @ 0x3074 
    /*000001f0*/ BL          mn34220_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #0 
    /*000001fc*/ MOVW        R1, #12440                  @ 0x3098 
    /*00000200*/ BL          mn34220_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #64                     @ 0x40 
    /*0000020c*/ MOVW        R1, #12441                  @ 0x3099 
    /*00000210*/ BL          mn34220_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #16 
    /*0000021c*/ MOVW        R1, #12442                  @ 0x309a 
    /*00000220*/ BL          mn34220_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #3 
    /*0000022c*/ MOVW        R1, #12545                  @ 0x3101 
    /*00000230*/ BL          mn34220_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #4 
    /*0000023c*/ MOVW        R1, #12548                  @ 0x3104 
    /*00000240*/ BL          mn34220_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #0 
    /*0000024c*/ MOVW        R1, #12550                  @ 0x3106 
    /*00000250*/ BL          mn34220_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #64                     @ 0x40 
    /*0000025c*/ MOVW        R1, #12551                  @ 0x3107 
    /*00000260*/ BL          mn34220_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #32 
    /*0000026c*/ MOVW        R1, #12587                  @ 0x312b 
    /*00000270*/ BL          mn34220_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #32 
    /*0000027c*/ MOVW        R1, #12589                  @ 0x312d 
    /*00000280*/ BL          mn34220_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #32 
    /*0000028c*/ MOVW        R1, #12591                  @ 0x312f 
    /*00000290*/ BL          mn34220_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #112                    @ 0x70 
    /*0000029c*/ MOVW        R1, #12609                  @ 0x3141 
    /*000002a0*/ BL          mn34220_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #1 
    /*000002ac*/ MOVW        R1, #12611                  @ 0x3143 
    /*000002b0*/ BL          mn34220_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #3 
    /*000002bc*/ MOVW        R1, #12612                  @ 0x3144 
    /*000002c0*/ BL          mn34220_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #2 
    /*000002cc*/ MOVW        R1, #12613                  @ 0x3145 
    /*000002d0*/ BL          mn34220_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #0 
    /*000002dc*/ MOVW        R1, #12615                  @ 0x3147 
    /*000002e0*/ BL          mn34220_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #0 
    /*000002ec*/ MOVW        R1, #12616                  @ 0x3148 
    /*000002f0*/ BL          mn34220_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #0 
    /*000002fc*/ MOVW        R1, #12617                  @ 0x3149 
    /*00000300*/ BL          mn34220_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #3 
    /*0000030c*/ MOVW        R1, #12618                  @ 0x314a 
    /*00000310*/ BL          mn34220_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #1 
    /*0000031c*/ MOVW        R1, #12619                  @ 0x314b 
    /*00000320*/ BL          mn34220_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #1 
    /*0000032c*/ MOVW        R1, #12620                  @ 0x314c 
    /*00000330*/ BL          mn34220_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #1 
    /*0000033c*/ MOVW        R1, #12621                  @ 0x314d 
    /*00000340*/ BL          mn34220_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #2 
    /*0000034c*/ MOVW        R1, #12622                  @ 0x314e 
    /*00000350*/ BL          mn34220_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #2 
    /*0000035c*/ MOVW        R1, #12623                  @ 0x314f 
    /*00000360*/ BL          mn34220_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #2 
    /*0000036c*/ MOVW        R1, #12624                  @ 0x3150 
    /*00000370*/ BL          mn34220_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #1 
    /*0000037c*/ MOVW        R1, #12626                  @ 0x3152 
    /*00000380*/ BL          mn34220_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #7 
    /*0000038c*/ MOVW        R1, #12627                  @ 0x3153 
    /*00000390*/ BL          mn34220_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #17 
    /*0000039c*/ MOVW        R1, #12629                  @ 0x3155 
    /*000003a0*/ BL          mn34220_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #48                     @ 0x30 
    /*000003ac*/ MOVW        R1, #12631                  @ 0x3157 
    /*000003b0*/ BL          mn34220_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #51                     @ 0x33 
    /*000003bc*/ MOVW        R1, #12633                  @ 0x3159 
    /*000003c0*/ BL          mn34220_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #54                     @ 0x36 
    /*000003cc*/ MOVW        R1, #12635                  @ 0x315b 
    /*000003d0*/ BL          mn34220_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #53                     @ 0x35 
    /*000003dc*/ MOVW        R1, #12637                  @ 0x315d 
    /*000003e0*/ BL          mn34220_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #60                     @ 0x3c 
    /*000003ec*/ MOVW        R1, #12639                  @ 0x315f 
    /*000003f0*/ BL          mn34220_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #63                     @ 0x3f 
    /*000003fc*/ MOVW        R1, #12641                  @ 0x3161 
    /*00000400*/ BL          mn34220_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #58                     @ 0x3a 
    /*0000040c*/ MOVW        R1, #12643                  @ 0x3163 
    /*00000410*/ BL          mn34220_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #57                     @ 0x39 
    /*0000041c*/ MOVW        R1, #12645                  @ 0x3165 
    /*00000420*/ BL          mn34220_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #40                     @ 0x28 
    /*0000042c*/ MOVW        R1, #12647                  @ 0x3167 
    /*00000430*/ BL          mn34220_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #43                     @ 0x2b 
    /*0000043c*/ MOVW        R1, #12649                  @ 0x3169 
    /*00000440*/ BL          mn34220_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #46                     @ 0x2e 
    /*0000044c*/ MOVW        R1, #12651                  @ 0x316b 
    /*00000450*/ BL          mn34220_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #45                     @ 0x2d 
    /*0000045c*/ MOVW        R1, #12653                  @ 0x316d 
    /*00000460*/ BL          mn34220_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #34                     @ 0x22 
    /*0000046c*/ MOVW        R1, #12655                  @ 0x316f 
    /*00000470*/ BL          mn34220_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #34                     @ 0x22 
    /*0000047c*/ MOVW        R1, #12657                  @ 0x3171 
    /*00000480*/ BL          mn34220_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #97                     @ 0x61 
    /*0000048c*/ MOVW        R1, #12659                  @ 0x3173 
    /*00000490*/ BL          mn34220_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #128                    @ 0x80 
    /*0000049c*/ MOVW        R1, #12661                  @ 0x3175 
    /*000004a0*/ BL          mn34220_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #32 
    /*000004ac*/ MOVW        R1, #12686                  @ 0x318e 
    /*000004b0*/ BL          mn34220_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #112                    @ 0x70 
    /*000004bc*/ MOVW        R1, #12687                  @ 0x318f 
    /*000004c0*/ BL          mn34220_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #8 
    /*000004cc*/ MOVW        R1, #12694                  @ 0x3196 
    /*000004d0*/ BL          mn34220_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #3 
    /*000004dc*/ MOVW        R1, #12796                  @ 0x31fc 
    /*000004e0*/ BL          mn34220_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #6 
    /*000004ec*/ MOVW        R1, #12798                  @ 0x31fe 
    /*000004f0*/ BL          mn34220_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #112                    @ 0x70 
    /*000004fc*/ MOVW        R1, #12860                  @ 0x323c 
    /*00000500*/ BL          mn34220_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #0 
    /*0000050c*/ MOVW        R1, #12862                  @ 0x323e 
    /*00000510*/ BL          mn34220_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #117                    @ 0x75 
    /*0000051c*/ MOVW        R1, #12867                  @ 0x3243 
    /*00000520*/ BL          mn34220_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #0 
    /*0000052c*/ MOVW        R1, #12870                  @ 0x3246 
    /*00000530*/ BL          mn34220_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #201                    @ 0xc9 
    /*0000053c*/ MOVW        R1, #12871                  @ 0x3247 
    /*00000540*/ BL          mn34220_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #48                     @ 0x30 
    /*0000054c*/ MOVW        R1, #12874                  @ 0x324a 
    /*00000550*/ BL          mn34220_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #27 
    /*0000055c*/ MOVW        R1, #12875                  @ 0x324b 
    /*00000560*/ BL          mn34220_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #2 
    /*0000056c*/ MOVW        R1, #12876                  @ 0x324c 
    /*00000570*/ BL          mn34220_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #123                    @ 0x7b 
    /*0000057c*/ MOVW        R1, #12883                  @ 0x3253 
    /*00000580*/ BL          mn34220_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #50                     @ 0x32 
    /*0000058c*/ MOVW        R1, #12886                  @ 0x3256 
    /*00000590*/ BL          mn34220_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #0 
    /*0000059c*/ MOVW        R1, #12888                  @ 0x3258 
    /*000005a0*/ BL          mn34220_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #92                     @ 0x5c 
    /*000005ac*/ MOVW        R1, #12889                  @ 0x3259 
    /*000005b0*/ BL          mn34220_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #20 
    /*000005bc*/ MOVW        R1, #12890                  @ 0x325a 
    /*000005c0*/ BL          mn34220_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #12 
    /*000005cc*/ MOVW        R1, #12914                  @ 0x3272 
    /*000005d0*/ BL          mn34220_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #48                     @ 0x30 
    /*000005dc*/ MOV         R1, #12928                  @ 0x3280 
    /*000005e0*/ BL          mn34220_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #7 
    /*000005ec*/ MOVW        R1, #12930                  @ 0x3282 
    /*000005f0*/ BL          mn34220_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #25 
    /*000005fc*/ MOVW        R1, #12933                  @ 0x3285 
    /*00000600*/ BL          mn34220_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #0 
    /*0000060c*/ MOVW        R1, #12936                  @ 0x3288 
    /*00000610*/ BL          mn34220_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #64                     @ 0x40 
    /*0000061c*/ MOVW        R1, #12937                  @ 0x3289 
    /*00000620*/ BL          mn34220_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #5 
    /*0000062c*/ MOVW        R1, #13070                  @ 0x330e 
    /*00000630*/ BL          mn34220_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #2 
    /*0000063c*/ MOVW        R1, #13072                  @ 0x3310 
    /*00000640*/ BL          mn34220_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #31 
    /*0000064c*/ MOVW        R1, #13077                  @ 0x3315 
    /*00000650*/ BL          mn34220_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #3 
    /*0000065c*/ MOVW        R1, #13082                  @ 0x331a 
    /*00000660*/ BL          mn34220_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #3 
    /*0000066c*/ MOVW        R1, #13083                  @ 0x331b 
    /*00000670*/ BL          mn34220_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #3 
    /*0000067c*/ MOVW        R1, #13113                  @ 0x3339 
    /*00000680*/ BL          mn34220_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #2 
    /*0000068c*/ MOVW        R1, #13163                  @ 0x336b 
    /*00000690*/ BL          mn34220_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #1 
    /*0000069c*/ MOVW        R1, #13215                  @ 0x339f 
    /*000006a0*/ BL          mn34220_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #1 
    /*000006ac*/ MOVW        R1, #13218                  @ 0x33a2 
    /*000006b0*/ BL          mn34220_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #1 
    /*000006bc*/ MOVW        R1, #13219                  @ 0x33a3 
    /*000006c0*/ BL          mn34220_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #0 
    /*000006cc*/ MOV         R1, #12288                  @ 0x3000 
    /*000006d0*/ BL          mn34220_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #211                    @ 0xd3 
    /*000006dc*/ MOVW        R1, #12289                  @ 0x3001 
    /*000006e0*/ BL          mn34220_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #1 
    /*000006ec*/ MOV         R1, #256                    @ 0x100 
    /*000006f0*/ BL          mn34220_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #0 
    /*000006fc*/ MOVW        R1, #257                    @ 0x101 
    /*00000700*/ BL          mn34220_write_register
    /*00000704*/ LDR         R0, WORD_0714               @ LDR         R0, [PC, #8]                @ 0x0000000000000714 
    /*00000708*/ POP         {R4, LR} 
    /*0000070c*/ ADD         R0, PC, R0 
    mn34220_wdr_1080p_4to1_init_x710:
    /*00000710*/ B           mn34220_wdr_1080p_4to1_init_x710@ puts [R_ARM_JUMP24: +0x0] 
    /*00000714*/ WORD_0714: .word 0x00000000
FUNC_END mn34220_wdr_1080p_4to1_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_wdr_1080p_3to1_init
@ Size: 0x748
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_wdr_1080p_3to1_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #1 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000010*/ BL          mn34220_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #0 
    /*0000001c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000020*/ BL          mn34220_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #0 
    /*0000002c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000030*/ BL          mn34220_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #3 
    /*0000003c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000040*/ BL          mn34220_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #12 
    /*0000004c*/ MOVW        R1, #274                    @ 0x112 
    /*00000050*/ BL          mn34220_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #12 
    /*0000005c*/ MOVW        R1, #275                    @ 0x113 
    /*00000060*/ BL          mn34220_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #4 
    /*0000006c*/ MOVW        R1, #12292                  @ 0x3004 
    /*00000070*/ BL          mn34220_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #0 
    /*0000007c*/ MOVW        R1, #12296                  @ 0x3008 
    /*00000080*/ BL          mn34220_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #0 
    /*0000008c*/ MOVW        R1, #12299                  @ 0x300b 
    /*00000090*/ BL          mn34220_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #67                     @ 0x43 
    /*0000009c*/ MOVW        R1, #12312                  @ 0x3018 
    /*000000a0*/ BL          mn34220_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #16 
    /*000000ac*/ MOVW        R1, #12313                  @ 0x3019 
    /*000000b0*/ BL          mn34220_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #185                    @ 0xb9 
    /*000000bc*/ MOVW        R1, #12314                  @ 0x301a 
    /*000000c0*/ BL          mn34220_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #0 
    /*000000cc*/ MOV         R1, #12288                  @ 0x3000 
    /*000000d0*/ BL          mn34220_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #83                     @ 0x53 
    /*000000dc*/ MOVW        R1, #12289                  @ 0x3001 
    /*000000e0*/ BL          mn34220_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #0 
    /*000000ec*/ MOVW        R1, #12302                  @ 0x300e 
    /*000000f0*/ BL          mn34220_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #0 
    /*000000fc*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000100*/ BL          mn34220_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #0 
    /*0000010c*/ MOVW        R1, #514                    @ 0x202 
    /*00000110*/ BL          mn34220_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #32 
    /*0000011c*/ MOVW        R1, #515                    @ 0x203 
    /*00000120*/ BL          mn34220_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #2 
    /*0000012c*/ MOVW        R1, #773                    @ 0x305 
    /*00000130*/ BL          mn34220_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #48                     @ 0x30 
    /*0000013c*/ MOVW        R1, #775                    @ 0x307 
    /*00000140*/ BL          mn34220_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #5 
    /*0000014c*/ MOV         R1, #832                    @ 0x340 
    /*00000150*/ BL          mn34220_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #220                    @ 0xdc 
    /*0000015c*/ MOVW        R1, #833                    @ 0x341 
    /*00000160*/ BL          mn34220_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #25 
    /*0000016c*/ MOVW        R1, #834                    @ 0x342 
    /*00000170*/ BL          mn34220_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #200                    @ 0xc8 
    /*0000017c*/ MOVW        R1, #835                    @ 0x343 
    /*00000180*/ BL          mn34220_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #0 
    /*0000018c*/ MOVW        R1, #12342                  @ 0x3036 
    /*00000190*/ BL          mn34220_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #46                     @ 0x2e 
    /*0000019c*/ MOVW        R1, #12345                  @ 0x3039 
    /*000001a0*/ BL          mn34220_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #18 
    /*000001ac*/ MOVW        R1, #12353                  @ 0x3041 
    /*000001b0*/ BL          mn34220_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #3 
    /*000001bc*/ MOVW        R1, #12376                  @ 0x3058 
    /*000001c0*/ BL          mn34220_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #9 
    /*000001cc*/ MOVW        R1, #12398                  @ 0x306e 
    /*000001d0*/ BL          mn34220_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #0 
    /*000001dc*/ MOVW        R1, #12399                  @ 0x306f 
    /*000001e0*/ BL          mn34220_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #1 
    /*000001ec*/ MOVW        R1, #12404                  @ 0x3074 
    /*000001f0*/ BL          mn34220_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #4 
    /*000001fc*/ MOVW        R1, #12440                  @ 0x3098 
    /*00000200*/ BL          mn34220_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #0 
    /*0000020c*/ MOVW        R1, #12441                  @ 0x3099 
    /*00000210*/ BL          mn34220_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #16 
    /*0000021c*/ MOVW        R1, #12442                  @ 0x309a 
    /*00000220*/ BL          mn34220_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #2 
    /*0000022c*/ MOVW        R1, #12545                  @ 0x3101 
    /*00000230*/ BL          mn34220_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #4 
    /*0000023c*/ MOVW        R1, #12548                  @ 0x3104 
    /*00000240*/ BL          mn34220_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #0 
    /*0000024c*/ MOVW        R1, #12550                  @ 0x3106 
    /*00000250*/ BL          mn34220_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #64                     @ 0x40 
    /*0000025c*/ MOVW        R1, #12551                  @ 0x3107 
    /*00000260*/ BL          mn34220_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #32 
    /*0000026c*/ MOVW        R1, #12587                  @ 0x312b 
    /*00000270*/ BL          mn34220_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #32 
    /*0000027c*/ MOVW        R1, #12589                  @ 0x312d 
    /*00000280*/ BL          mn34220_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #32 
    /*0000028c*/ MOVW        R1, #12591                  @ 0x312f 
    /*00000290*/ BL          mn34220_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #112                    @ 0x70 
    /*0000029c*/ MOVW        R1, #12609                  @ 0x3141 
    /*000002a0*/ BL          mn34220_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #1 
    /*000002ac*/ MOVW        R1, #12611                  @ 0x3143 
    /*000002b0*/ BL          mn34220_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #3 
    /*000002bc*/ MOVW        R1, #12612                  @ 0x3144 
    /*000002c0*/ BL          mn34220_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #2 
    /*000002cc*/ MOVW        R1, #12613                  @ 0x3145 
    /*000002d0*/ BL          mn34220_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #0 
    /*000002dc*/ MOVW        R1, #12615                  @ 0x3147 
    /*000002e0*/ BL          mn34220_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #0 
    /*000002ec*/ MOVW        R1, #12616                  @ 0x3148 
    /*000002f0*/ BL          mn34220_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #0 
    /*000002fc*/ MOVW        R1, #12617                  @ 0x3149 
    /*00000300*/ BL          mn34220_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #3 
    /*0000030c*/ MOVW        R1, #12618                  @ 0x314a 
    /*00000310*/ BL          mn34220_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #1 
    /*0000031c*/ MOVW        R1, #12619                  @ 0x314b 
    /*00000320*/ BL          mn34220_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #1 
    /*0000032c*/ MOVW        R1, #12620                  @ 0x314c 
    /*00000330*/ BL          mn34220_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #1 
    /*0000033c*/ MOVW        R1, #12621                  @ 0x314d 
    /*00000340*/ BL          mn34220_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #2 
    /*0000034c*/ MOVW        R1, #12622                  @ 0x314e 
    /*00000350*/ BL          mn34220_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #2 
    /*0000035c*/ MOVW        R1, #12623                  @ 0x314f 
    /*00000360*/ BL          mn34220_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #2 
    /*0000036c*/ MOVW        R1, #12624                  @ 0x3150 
    /*00000370*/ BL          mn34220_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #1 
    /*0000037c*/ MOVW        R1, #12626                  @ 0x3152 
    /*00000380*/ BL          mn34220_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #7 
    /*0000038c*/ MOVW        R1, #12627                  @ 0x3153 
    /*00000390*/ BL          mn34220_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #17 
    /*0000039c*/ MOVW        R1, #12629                  @ 0x3155 
    /*000003a0*/ BL          mn34220_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #48                     @ 0x30 
    /*000003ac*/ MOVW        R1, #12631                  @ 0x3157 
    /*000003b0*/ BL          mn34220_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #51                     @ 0x33 
    /*000003bc*/ MOVW        R1, #12633                  @ 0x3159 
    /*000003c0*/ BL          mn34220_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #54                     @ 0x36 
    /*000003cc*/ MOVW        R1, #12635                  @ 0x315b 
    /*000003d0*/ BL          mn34220_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #53                     @ 0x35 
    /*000003dc*/ MOVW        R1, #12637                  @ 0x315d 
    /*000003e0*/ BL          mn34220_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #60                     @ 0x3c 
    /*000003ec*/ MOVW        R1, #12639                  @ 0x315f 
    /*000003f0*/ BL          mn34220_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #63                     @ 0x3f 
    /*000003fc*/ MOVW        R1, #12641                  @ 0x3161 
    /*00000400*/ BL          mn34220_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #58                     @ 0x3a 
    /*0000040c*/ MOVW        R1, #12643                  @ 0x3163 
    /*00000410*/ BL          mn34220_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #57                     @ 0x39 
    /*0000041c*/ MOVW        R1, #12645                  @ 0x3165 
    /*00000420*/ BL          mn34220_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #40                     @ 0x28 
    /*0000042c*/ MOVW        R1, #12647                  @ 0x3167 
    /*00000430*/ BL          mn34220_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #43                     @ 0x2b 
    /*0000043c*/ MOVW        R1, #12649                  @ 0x3169 
    /*00000440*/ BL          mn34220_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #46                     @ 0x2e 
    /*0000044c*/ MOVW        R1, #12651                  @ 0x316b 
    /*00000450*/ BL          mn34220_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #45                     @ 0x2d 
    /*0000045c*/ MOVW        R1, #12653                  @ 0x316d 
    /*00000460*/ BL          mn34220_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #34                     @ 0x22 
    /*0000046c*/ MOVW        R1, #12655                  @ 0x316f 
    /*00000470*/ BL          mn34220_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #34                     @ 0x22 
    /*0000047c*/ MOVW        R1, #12657                  @ 0x3171 
    /*00000480*/ BL          mn34220_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #97                     @ 0x61 
    /*0000048c*/ MOVW        R1, #12659                  @ 0x3173 
    /*00000490*/ BL          mn34220_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #128                    @ 0x80 
    /*0000049c*/ MOVW        R1, #12661                  @ 0x3175 
    /*000004a0*/ BL          mn34220_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #32 
    /*000004ac*/ MOVW        R1, #12686                  @ 0x318e 
    /*000004b0*/ BL          mn34220_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #112                    @ 0x70 
    /*000004bc*/ MOVW        R1, #12687                  @ 0x318f 
    /*000004c0*/ BL          mn34220_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #8 
    /*000004cc*/ MOVW        R1, #12694                  @ 0x3196 
    /*000004d0*/ BL          mn34220_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #3 
    /*000004dc*/ MOVW        R1, #12796                  @ 0x31fc 
    /*000004e0*/ BL          mn34220_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #6 
    /*000004ec*/ MOVW        R1, #12798                  @ 0x31fe 
    /*000004f0*/ BL          mn34220_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #1 
    /*000004fc*/ MOVW        R1, #12801                  @ 0x3201 
    /*00000500*/ BL          mn34220_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #3 
    /*0000050c*/ MOVW        R1, #12802                  @ 0x3202 
    /*00000510*/ BL          mn34220_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #53                     @ 0x35 
    /*0000051c*/ MOVW        R1, #12803                  @ 0x3203 
    /*00000520*/ BL          mn34220_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #112                    @ 0x70 
    /*0000052c*/ MOVW        R1, #12860                  @ 0x323c 
    /*00000530*/ BL          mn34220_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #0 
    /*0000053c*/ MOVW        R1, #12862                  @ 0x323e 
    /*00000540*/ BL          mn34220_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #117                    @ 0x75 
    /*0000054c*/ MOVW        R1, #12867                  @ 0x3243 
    /*00000550*/ BL          mn34220_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #0 
    /*0000055c*/ MOVW        R1, #12870                  @ 0x3246 
    /*00000560*/ BL          mn34220_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #201                    @ 0xc9 
    /*0000056c*/ MOVW        R1, #12871                  @ 0x3247 
    /*00000570*/ BL          mn34220_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #48                     @ 0x30 
    /*0000057c*/ MOVW        R1, #12874                  @ 0x324a 
    /*00000580*/ BL          mn34220_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #27 
    /*0000058c*/ MOVW        R1, #12875                  @ 0x324b 
    /*00000590*/ BL          mn34220_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #2 
    /*0000059c*/ MOVW        R1, #12876                  @ 0x324c 
    /*000005a0*/ BL          mn34220_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #123                    @ 0x7b 
    /*000005ac*/ MOVW        R1, #12883                  @ 0x3253 
    /*000005b0*/ BL          mn34220_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #50                     @ 0x32 
    /*000005bc*/ MOVW        R1, #12886                  @ 0x3256 
    /*000005c0*/ BL          mn34220_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #0 
    /*000005cc*/ MOVW        R1, #12888                  @ 0x3258 
    /*000005d0*/ BL          mn34220_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #92                     @ 0x5c 
    /*000005dc*/ MOVW        R1, #12889                  @ 0x3259 
    /*000005e0*/ BL          mn34220_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #20 
    /*000005ec*/ MOVW        R1, #12890                  @ 0x325a 
    /*000005f0*/ BL          mn34220_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #12 
    /*000005fc*/ MOVW        R1, #12914                  @ 0x3272 
    /*00000600*/ BL          mn34220_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #48                     @ 0x30 
    /*0000060c*/ MOV         R1, #12928                  @ 0x3280 
    /*00000610*/ BL          mn34220_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #7 
    /*0000061c*/ MOVW        R1, #12930                  @ 0x3282 
    /*00000620*/ BL          mn34220_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #25 
    /*0000062c*/ MOVW        R1, #12933                  @ 0x3285 
    /*00000630*/ BL          mn34220_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #0 
    /*0000063c*/ MOVW        R1, #12936                  @ 0x3288 
    /*00000640*/ BL          mn34220_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #64                     @ 0x40 
    /*0000064c*/ MOVW        R1, #12937                  @ 0x3289 
    /*00000650*/ BL          mn34220_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #5 
    /*0000065c*/ MOVW        R1, #13070                  @ 0x330e 
    /*00000660*/ BL          mn34220_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #2 
    /*0000066c*/ MOVW        R1, #13072                  @ 0x3310 
    /*00000670*/ BL          mn34220_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #31 
    /*0000067c*/ MOVW        R1, #13077                  @ 0x3315 
    /*00000680*/ BL          mn34220_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #3 
    /*0000068c*/ MOVW        R1, #13082                  @ 0x331a 
    /*00000690*/ BL          mn34220_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #3 
    /*0000069c*/ MOVW        R1, #13083                  @ 0x331b 
    /*000006a0*/ BL          mn34220_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #3 
    /*000006ac*/ MOVW        R1, #13113                  @ 0x3339 
    /*000006b0*/ BL          mn34220_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #2 
    /*000006bc*/ MOVW        R1, #13163                  @ 0x336b 
    /*000006c0*/ BL          mn34220_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #1 
    /*000006cc*/ MOVW        R1, #13215                  @ 0x339f 
    /*000006d0*/ BL          mn34220_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #1 
    /*000006dc*/ MOVW        R1, #13218                  @ 0x33a2 
    /*000006e0*/ BL          mn34220_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #1 
    /*000006ec*/ MOVW        R1, #13219                  @ 0x33a3 
    /*000006f0*/ BL          mn34220_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #0 
    /*000006fc*/ MOV         R1, #12288                  @ 0x3000 
    /*00000700*/ BL          mn34220_write_register
    /*00000704*/ MOV         R0, R4 
    /*00000708*/ MOV         R2, #211                    @ 0xd3 
    /*0000070c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000710*/ BL          mn34220_write_register
    /*00000714*/ MOV         R0, R4 
    /*00000718*/ MOV         R2, #1 
    /*0000071c*/ MOV         R1, #256                    @ 0x100 
    /*00000720*/ BL          mn34220_write_register
    /*00000724*/ MOV         R0, R4 
    /*00000728*/ MOV         R2, #0 
    /*0000072c*/ MOVW        R1, #257                    @ 0x101 
    /*00000730*/ BL          mn34220_write_register
    /*00000734*/ LDR         R0, WORD_0744               @ LDR         R0, [PC, #8]                @ 0x0000000000000744 
    /*00000738*/ POP         {R4, LR} 
    /*0000073c*/ ADD         R0, PC, R0 
    mn34220_wdr_1080p_3to1_init_x740:
    /*00000740*/ B           mn34220_wdr_1080p_3to1_init_x740@ puts [R_ARM_JUMP24: +0x0] 
    /*00000744*/ WORD_0744: .word 0x00000000
FUNC_END mn34220_wdr_1080p_3to1_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_wdr_1080P_2to1_init
@ Size: 0x3278
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_wdr_1080P_2to1_init
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #0 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOVW        R1, #12322                  @ 0x3022 
    /*00000010*/ BL          mn34220_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #0 
    /*0000001c*/ MOVW        R1, #12323                  @ 0x3023 
    /*00000020*/ BL          mn34220_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #1 
    /*0000002c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000030*/ BL          mn34220_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #0 
    /*0000003c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000040*/ BL          mn34220_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #6 
    /*0000004c*/ MOVW        R1, #12290                  @ 0x3002 
    /*00000050*/ BL          mn34220_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #24 
    /*0000005c*/ MOVW        R1, #12291                  @ 0x3003 
    /*00000060*/ BL          mn34220_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #0 
    /*0000006c*/ MOV         R1, #772                    @ 0x304 
    /*00000070*/ BL          mn34220_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #2 
    /*0000007c*/ MOVW        R1, #773                    @ 0x305 
    /*00000080*/ BL          mn34220_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #0 
    /*0000008c*/ MOVW        R1, #774                    @ 0x306 
    /*00000090*/ BL          mn34220_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #48                     @ 0x30 
    /*0000009c*/ MOVW        R1, #775                    @ 0x307 
    /*000000a0*/ BL          mn34220_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #0 
    /*000000ac*/ MOV         R1, #12288                  @ 0x3000 
    /*000000b0*/ BL          mn34220_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #3 
    /*000000bc*/ MOVW        R1, #12289                  @ 0x3001 
    /*000000c0*/ BL          mn34220_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #0 
    /*000000cc*/ MOVW        R1, #258                    @ 0x102 
    /*000000d0*/ BL          mn34220_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #0 
    /*000000dc*/ MOVW        R1, #259                    @ 0x103 
    /*000000e0*/ BL          mn34220_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #0 
    /*000000ec*/ MOV         R1, #260                    @ 0x104 
    /*000000f0*/ BL          mn34220_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #0 
    /*000000fc*/ MOVW        R1, #261                    @ 0x105 
    /*00000100*/ BL          mn34220_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #0 
    /*0000010c*/ MOV         R1, #272                    @ 0x110 
    /*00000110*/ BL          mn34220_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #0 
    /*0000011c*/ MOVW        R1, #273                    @ 0x111 
    /*00000120*/ BL          mn34220_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #12 
    /*0000012c*/ MOVW        R1, #274                    @ 0x112 
    /*00000130*/ BL          mn34220_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #12 
    /*0000013c*/ MOVW        R1, #275                    @ 0x113 
    /*00000140*/ BL          mn34220_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #0 
    /*0000014c*/ MOV         R1, #288                    @ 0x120 
    /*00000150*/ BL          mn34220_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #0 
    /*0000015c*/ MOVW        R1, #289                    @ 0x121 
    /*00000160*/ BL          mn34220_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #0 
    /*0000016c*/ MOV         R1, #768                    @ 0x300 
    /*00000170*/ BL          mn34220_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #16 
    /*0000017c*/ MOVW        R1, #769                    @ 0x301 
    /*00000180*/ BL          mn34220_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #0 
    /*0000018c*/ MOVW        R1, #770                    @ 0x302 
    /*00000190*/ BL          mn34220_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #1 
    /*0000019c*/ MOVW        R1, #771                    @ 0x303 
    /*000001a0*/ BL          mn34220_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #3 
    /*000001ac*/ MOVW        R1, #12292                  @ 0x3004 
    /*000001b0*/ BL          mn34220_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #103                    @ 0x67 
    /*000001bc*/ MOVW        R1, #12293                  @ 0x3005 
    /*000001c0*/ BL          mn34220_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #0 
    /*000001cc*/ MOVW        R1, #12294                  @ 0x3006 
    /*000001d0*/ BL          mn34220_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #20 
    /*000001dc*/ MOVW        R1, #12295                  @ 0x3007 
    /*000001e0*/ BL          mn34220_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #144                    @ 0x90 
    /*000001ec*/ MOVW        R1, #12296                  @ 0x3008 
    /*000001f0*/ BL          mn34220_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #6 
    /*000001fc*/ MOVW        R1, #12297                  @ 0x3009 
    /*00000200*/ BL          mn34220_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #255                    @ 0xff 
    /*0000020c*/ MOVW        R1, #12298                  @ 0x300a 
    /*00000210*/ BL          mn34220_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #0 
    /*0000021c*/ MOVW        R1, #12299                  @ 0x300b 
    /*00000220*/ BL          mn34220_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #0 
    /*0000022c*/ MOVW        R1, #12300                  @ 0x300c 
    /*00000230*/ BL          mn34220_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #1 
    /*0000023c*/ MOVW        R1, #12301                  @ 0x300d 
    /*00000240*/ BL          mn34220_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #0 
    /*0000024c*/ MOVW        R1, #12304                  @ 0x3010 
    /*00000250*/ BL          mn34220_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #0 
    /*0000025c*/ MOVW        R1, #12305                  @ 0x3011 
    /*00000260*/ BL          mn34220_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #0 
    /*0000026c*/ MOVW        R1, #12306                  @ 0x3012 
    /*00000270*/ BL          mn34220_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #0 
    /*0000027c*/ MOVW        R1, #12307                  @ 0x3013 
    /*00000280*/ BL          mn34220_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #0 
    /*0000028c*/ MOVW        R1, #12308                  @ 0x3014 
    /*00000290*/ BL          mn34220_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #1 
    /*0000029c*/ MOVW        R1, #12309                  @ 0x3015 
    /*000002a0*/ BL          mn34220_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #0 
    /*000002ac*/ MOVW        R1, #12310                  @ 0x3016 
    /*000002b0*/ BL          mn34220_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #0 
    /*000002bc*/ MOVW        R1, #12311                  @ 0x3017 
    /*000002c0*/ BL          mn34220_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #67                     @ 0x43 
    /*000002cc*/ MOVW        R1, #12312                  @ 0x3018 
    /*000002d0*/ BL          mn34220_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #4 
    /*000002dc*/ MOVW        R1, #12313                  @ 0x3019 
    /*000002e0*/ BL          mn34220_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #57                     @ 0x39 
    /*000002ec*/ MOVW        R1, #12314                  @ 0x301a 
    /*000002f0*/ BL          mn34220_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #0 
    /*000002fc*/ MOVW        R1, #12315                  @ 0x301b 
    /*00000300*/ BL          mn34220_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #44                     @ 0x2c 
    /*0000030c*/ MOVW        R1, #12316                  @ 0x301c 
    /*00000310*/ BL          mn34220_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #27 
    /*0000031c*/ MOVW        R1, #12317                  @ 0x301d 
    /*00000320*/ BL          mn34220_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #13 
    /*0000032c*/ MOVW        R1, #12318                  @ 0x301e 
    /*00000330*/ BL          mn34220_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #31 
    /*0000033c*/ MOVW        R1, #12319                  @ 0x301f 
    /*00000340*/ BL          mn34220_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #8 
    /*0000034c*/ MOVW        R1, #12320                  @ 0x3020 
    /*00000350*/ BL          mn34220_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #0 
    /*0000035c*/ MOVW        R1, #12321                  @ 0x3021 
    /*00000360*/ BL          mn34220_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #0 
    /*0000036c*/ MOV         R1, #12288                  @ 0x3000 
    /*00000370*/ BL          mn34220_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #83                     @ 0x53 
    /*0000037c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00000380*/ BL          mn34220_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #0 
    /*0000038c*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000390*/ BL          mn34220_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #0 
    /*0000039c*/ MOVW        R1, #12303                  @ 0x300f 
    /*000003a0*/ BL          mn34220_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #0 
    /*000003ac*/ MOV         R1, #255                    @ 0xff 
    /*000003b0*/ BL          mn34220_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #0 
    /*000003bc*/ MOV         R1, #255                    @ 0xff 
    /*000003c0*/ BL          mn34220_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #0 
    /*000003cc*/ MOV         R1, #255                    @ 0xff 
    /*000003d0*/ BL          mn34220_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #0 
    /*000003dc*/ MOV         R1, #255                    @ 0xff 
    /*000003e0*/ BL          mn34220_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #0 
    /*000003ec*/ MOV         R1, #255                    @ 0xff 
    /*000003f0*/ BL          mn34220_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #0 
    /*000003fc*/ MOV         R1, #255                    @ 0xff 
    /*00000400*/ BL          mn34220_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #0 
    /*0000040c*/ MOV         R1, #255                    @ 0xff 
    /*00000410*/ BL          mn34220_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #0 
    /*0000041c*/ MOV         R1, #255                    @ 0xff 
    /*00000420*/ BL          mn34220_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #0 
    /*0000042c*/ MOV         R1, #255                    @ 0xff 
    /*00000430*/ BL          mn34220_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #0 
    /*0000043c*/ MOV         R1, #255                    @ 0xff 
    /*00000440*/ BL          mn34220_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #2 
    /*0000044c*/ MOVW        R1, #514                    @ 0x202 
    /*00000450*/ BL          mn34220_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #32 
    /*0000045c*/ MOVW        R1, #515                    @ 0x203 
    /*00000460*/ BL          mn34220_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #1 
    /*0000046c*/ MOV         R1, #516                    @ 0x204 
    /*00000470*/ BL          mn34220_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #0 
    /*0000047c*/ MOVW        R1, #517                    @ 0x205 
    /*00000480*/ BL          mn34220_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #1 
    /*0000048c*/ MOVW        R1, #526                    @ 0x20e 
    /*00000490*/ BL          mn34220_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #0 
    /*0000049c*/ MOVW        R1, #527                    @ 0x20f 
    /*000004a0*/ BL          mn34220_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #1 
    /*000004ac*/ MOV         R1, #528                    @ 0x210 
    /*000004b0*/ BL          mn34220_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #0 
    /*000004bc*/ MOVW        R1, #529                    @ 0x211 
    /*000004c0*/ BL          mn34220_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #1 
    /*000004cc*/ MOVW        R1, #530                    @ 0x212 
    /*000004d0*/ BL          mn34220_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #0 
    /*000004dc*/ MOVW        R1, #531                    @ 0x213 
    /*000004e0*/ BL          mn34220_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #1 
    /*000004ec*/ MOV         R1, #532                    @ 0x214 
    /*000004f0*/ BL          mn34220_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #0 
    /*000004fc*/ MOVW        R1, #533                    @ 0x215 
    /*00000500*/ BL          mn34220_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #0 
    /*0000050c*/ MOV         R1, #544                    @ 0x220 
    /*00000510*/ BL          mn34220_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #0 
    /*0000051c*/ MOVW        R1, #545                    @ 0x221 
    /*00000520*/ BL          mn34220_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #0 
    /*0000052c*/ MOVW        R1, #830                    @ 0x33e 
    /*00000530*/ BL          mn34220_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #0 
    /*0000053c*/ MOVW        R1, #831                    @ 0x33f 
    /*00000540*/ BL          mn34220_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #4 
    /*0000054c*/ MOV         R1, #832                    @ 0x340 
    /*00000550*/ BL          mn34220_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #226                    @ 0xe2 
    /*0000055c*/ MOVW        R1, #833                    @ 0x341 
    /*00000560*/ BL          mn34220_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #20 
    /*0000056c*/ MOVW        R1, #834                    @ 0x342 
    /*00000570*/ BL          mn34220_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #160                    @ 0xa0 
    /*0000057c*/ MOVW        R1, #835                    @ 0x343 
    /*00000580*/ BL          mn34220_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #0 
    /*0000058c*/ MOV         R1, #836                    @ 0x344 
    /*00000590*/ BL          mn34220_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #0 
    /*0000059c*/ MOVW        R1, #837                    @ 0x345 
    /*000005a0*/ BL          mn34220_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #0 
    /*000005ac*/ MOVW        R1, #838                    @ 0x346 
    /*000005b0*/ BL          mn34220_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #60                     @ 0x3c 
    /*000005bc*/ MOVW        R1, #839                    @ 0x347 
    /*000005c0*/ BL          mn34220_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #0 
    /*000005cc*/ MOV         R1, #840                    @ 0x348 
    /*000005d0*/ BL          mn34220_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #0 
    /*000005dc*/ MOVW        R1, #841                    @ 0x349 
    /*000005e0*/ BL          mn34220_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #4 
    /*000005ec*/ MOVW        R1, #842                    @ 0x34a 
    /*000005f0*/ BL          mn34220_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #127                    @ 0x7f 
    /*000005fc*/ MOVW        R1, #843                    @ 0x34b 
    /*00000600*/ BL          mn34220_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #0 
    /*0000060c*/ MOV         R1, #844                    @ 0x34c 
    /*00000610*/ BL          mn34220_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #0 
    /*0000061c*/ MOVW        R1, #845                    @ 0x34d 
    /*00000620*/ BL          mn34220_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #4 
    /*0000062c*/ MOVW        R1, #846                    @ 0x34e 
    /*00000630*/ BL          mn34220_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #68                     @ 0x44 
    /*0000063c*/ MOVW        R1, #847                    @ 0x34f 
    /*00000640*/ BL          mn34220_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #0 
    /*0000064c*/ MOV         R1, #896                    @ 0x380 
    /*00000650*/ BL          mn34220_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #1 
    /*0000065c*/ MOVW        R1, #897                    @ 0x381 
    /*00000660*/ BL          mn34220_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #0 
    /*0000066c*/ MOVW        R1, #898                    @ 0x382 
    /*00000670*/ BL          mn34220_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #1 
    /*0000067c*/ MOVW        R1, #899                    @ 0x383 
    /*00000680*/ BL          mn34220_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #0 
    /*0000068c*/ MOV         R1, #900                    @ 0x384 
    /*00000690*/ BL          mn34220_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #1 
    /*0000069c*/ MOVW        R1, #901                    @ 0x385 
    /*000006a0*/ BL          mn34220_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #0 
    /*000006ac*/ MOVW        R1, #902                    @ 0x386 
    /*000006b0*/ BL          mn34220_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #1 
    /*000006bc*/ MOVW        R1, #903                    @ 0x387 
    /*000006c0*/ BL          mn34220_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #0 
    /*000006cc*/ MOV         R1, #1280                   @ 0x500 
    /*000006d0*/ BL          mn34220_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #0 
    /*000006dc*/ MOVW        R1, #1281                   @ 0x501 
    /*000006e0*/ BL          mn34220_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #0 
    /*000006ec*/ MOV         R1, #1536                   @ 0x600 
    /*000006f0*/ BL          mn34220_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #0 
    /*000006fc*/ MOVW        R1, #1537                   @ 0x601 
    /*00000700*/ BL          mn34220_write_register
    /*00000704*/ MOV         R0, R4 
    /*00000708*/ MOV         R2, #0 
    /*0000070c*/ MOVW        R1, #1538                   @ 0x602 
    /*00000710*/ BL          mn34220_write_register
    /*00000714*/ MOV         R0, R4 
    /*00000718*/ MOV         R2, #0 
    /*0000071c*/ MOVW        R1, #1539                   @ 0x603 
    /*00000720*/ BL          mn34220_write_register
    /*00000724*/ MOV         R0, R4 
    /*00000728*/ MOV         R2, #0 
    /*0000072c*/ MOVW        R1, #1540                   @ 0x604 
    /*00000730*/ BL          mn34220_write_register
    /*00000734*/ MOV         R0, R4 
    /*00000738*/ MOV         R2, #0 
    /*0000073c*/ MOVW        R1, #1541                   @ 0x605 
    /*00000740*/ BL          mn34220_write_register
    /*00000744*/ MOV         R0, R4 
    /*00000748*/ MOV         R2, #0 
    /*0000074c*/ MOVW        R1, #1542                   @ 0x606 
    /*00000750*/ BL          mn34220_write_register
    /*00000754*/ MOV         R0, R4 
    /*00000758*/ MOV         R2, #0 
    /*0000075c*/ MOVW        R1, #1543                   @ 0x607 
    /*00000760*/ BL          mn34220_write_register
    /*00000764*/ MOV         R0, R4 
    /*00000768*/ MOV         R2, #0 
    /*0000076c*/ MOVW        R1, #1544                   @ 0x608 
    /*00000770*/ BL          mn34220_write_register
    /*00000774*/ MOV         R0, R4 
    /*00000778*/ MOV         R2, #0 
    /*0000077c*/ MOVW        R1, #1545                   @ 0x609 
    /*00000780*/ BL          mn34220_write_register
    /*00000784*/ MOV         R0, R4 
    /*00000788*/ MOV         R2, #0 
    /*0000078c*/ MOVW        R1, #1546                   @ 0x60a 
    /*00000790*/ BL          mn34220_write_register
    /*00000794*/ MOV         R0, R4 
    /*00000798*/ MOV         R2, #0 
    /*0000079c*/ MOVW        R1, #1547                   @ 0x60b 
    /*000007a0*/ BL          mn34220_write_register
    /*000007a4*/ MOV         R0, R4 
    /*000007a8*/ MOV         R2, #0 
    /*000007ac*/ MOVW        R1, #1548                   @ 0x60c 
    /*000007b0*/ BL          mn34220_write_register
    /*000007b4*/ MOV         R0, R4 
    /*000007b8*/ MOV         R2, #0 
    /*000007bc*/ MOVW        R1, #1549                   @ 0x60d 
    /*000007c0*/ BL          mn34220_write_register
    /*000007c4*/ MOV         R0, R4 
    /*000007c8*/ MOV         R2, #0 
    /*000007cc*/ MOVW        R1, #1550                   @ 0x60e 
    /*000007d0*/ BL          mn34220_write_register
    /*000007d4*/ MOV         R0, R4 
    /*000007d8*/ MOV         R2, #0 
    /*000007dc*/ MOVW        R1, #1551                   @ 0x60f 
    /*000007e0*/ BL          mn34220_write_register
    /*000007e4*/ MOV         R0, R4 
    /*000007e8*/ MOV         R2, #0 
    /*000007ec*/ MOV         R1, #1552                   @ 0x610 
    /*000007f0*/ BL          mn34220_write_register
    /*000007f4*/ MOV         R0, R4 
    /*000007f8*/ MOV         R2, #0 
    /*000007fc*/ MOVW        R1, #1553                   @ 0x611 
    /*00000800*/ BL          mn34220_write_register
    /*00000804*/ MOV         R0, R4 
    /*00000808*/ MOV         R2, #0 
    /*0000080c*/ MOVW        R1, #12336                  @ 0x3030 
    /*00000810*/ BL          mn34220_write_register
    /*00000814*/ MOV         R0, R4 
    /*00000818*/ MOV         R2, #0 
    /*0000081c*/ MOVW        R1, #12337                  @ 0x3031 
    /*00000820*/ BL          mn34220_write_register
    /*00000824*/ MOV         R0, R4 
    /*00000828*/ MOV         R2, #0 
    /*0000082c*/ MOVW        R1, #12338                  @ 0x3032 
    /*00000830*/ BL          mn34220_write_register
    /*00000834*/ MOV         R0, R4 
    /*00000838*/ MOV         R2, #0 
    /*0000083c*/ MOVW        R1, #12339                  @ 0x3033 
    /*00000840*/ BL          mn34220_write_register
    /*00000844*/ MOV         R0, R4 
    /*00000848*/ MOV         R2, #0 
    /*0000084c*/ MOVW        R1, #12340                  @ 0x3034 
    /*00000850*/ BL          mn34220_write_register
    /*00000854*/ MOV         R0, R4 
    /*00000858*/ MOV         R2, #0 
    /*0000085c*/ MOVW        R1, #12341                  @ 0x3035 
    /*00000860*/ BL          mn34220_write_register
    /*00000864*/ MOV         R0, R4 
    /*00000868*/ MOV         R2, #0 
    /*0000086c*/ MOVW        R1, #12342                  @ 0x3036 
    /*00000870*/ BL          mn34220_write_register
    /*00000874*/ MOV         R0, R4 
    /*00000878*/ MOV         R2, #16 
    /*0000087c*/ MOVW        R1, #12343                  @ 0x3037 
    /*00000880*/ BL          mn34220_write_register
    /*00000884*/ MOV         R0, R4 
    /*00000888*/ MOV         R2, #255                    @ 0xff 
    /*0000088c*/ MOVW        R1, #12344                  @ 0x3038 
    /*00000890*/ BL          mn34220_write_register
    /*00000894*/ MOV         R0, R4 
    /*00000898*/ MOV         R2, #36                     @ 0x24 
    /*0000089c*/ MOVW        R1, #12345                  @ 0x3039 
    /*000008a0*/ BL          mn34220_write_register
    /*000008a4*/ MOV         R0, R4 
    /*000008a8*/ MOV         R2, #0 
    /*000008ac*/ MOVW        R1, #12346                  @ 0x303a 
    /*000008b0*/ BL          mn34220_write_register
    /*000008b4*/ MOV         R0, R4 
    /*000008b8*/ MOV         R2, #0 
    /*000008bc*/ MOVW        R1, #12347                  @ 0x303b 
    /*000008c0*/ BL          mn34220_write_register
    /*000008c4*/ MOV         R0, R4 
    /*000008c8*/ MOV         R2, #0 
    /*000008cc*/ MOVW        R1, #12348                  @ 0x303c 
    /*000008d0*/ BL          mn34220_write_register
    /*000008d4*/ MOV         R0, R4 
    /*000008d8*/ MOV         R2, #40                     @ 0x28 
    /*000008dc*/ MOVW        R1, #12349                  @ 0x303d 
    /*000008e0*/ BL          mn34220_write_register
    /*000008e4*/ MOV         R0, R4 
    /*000008e8*/ MOV         R2, #3 
    /*000008ec*/ MOVW        R1, #12350                  @ 0x303e 
    /*000008f0*/ BL          mn34220_write_register
    /*000008f4*/ MOV         R0, R4 
    /*000008f8*/ MOV         R2, #3 
    /*000008fc*/ MOVW        R1, #12351                  @ 0x303f 
    /*00000900*/ BL          mn34220_write_register
    /*00000904*/ MOV         R0, R4 
    /*00000908*/ MOV         R2, #192                    @ 0xc0 
    /*0000090c*/ MOV         R1, #12352                  @ 0x3040 
    /*00000910*/ BL          mn34220_write_register
    /*00000914*/ MOV         R0, R4 
    /*00000918*/ MOV         R2, #44                     @ 0x2c 
    /*0000091c*/ MOVW        R1, #12353                  @ 0x3041 
    /*00000920*/ BL          mn34220_write_register
    /*00000924*/ MOV         R0, R4 
    /*00000928*/ MOV         R2, #80                     @ 0x50 
    /*0000092c*/ MOVW        R1, #12354                  @ 0x3042 
    /*00000930*/ BL          mn34220_write_register
    /*00000934*/ MOV         R0, R4 
    /*00000938*/ MOV         R2, #16 
    /*0000093c*/ MOVW        R1, #12355                  @ 0x3043 
    /*00000940*/ BL          mn34220_write_register
    /*00000944*/ MOV         R0, R4 
    /*00000948*/ MOV         R2, #0 
    /*0000094c*/ MOVW        R1, #12356                  @ 0x3044 
    /*00000950*/ BL          mn34220_write_register
    /*00000954*/ MOV         R0, R4 
    /*00000958*/ MOV         R2, #0 
    /*0000095c*/ MOVW        R1, #12357                  @ 0x3045 
    /*00000960*/ BL          mn34220_write_register
    /*00000964*/ MOV         R0, R4 
    /*00000968*/ MOV         R2, #0 
    /*0000096c*/ MOVW        R1, #12368                  @ 0x3050 
    /*00000970*/ BL          mn34220_write_register
    /*00000974*/ MOV         R0, R4 
    /*00000978*/ MOV         R2, #0 
    /*0000097c*/ MOVW        R1, #12369                  @ 0x3051 
    /*00000980*/ BL          mn34220_write_register
    /*00000984*/ MOV         R0, R4 
    /*00000988*/ MOV         R2, #144                    @ 0x90 
    /*0000098c*/ MOVW        R1, #12370                  @ 0x3052 
    /*00000990*/ BL          mn34220_write_register
    /*00000994*/ MOV         R0, R4 
    /*00000998*/ MOV         R2, #34                     @ 0x22 
    /*0000099c*/ MOVW        R1, #12371                  @ 0x3053 
    /*000009a0*/ BL          mn34220_write_register
    /*000009a4*/ MOV         R0, R4 
    /*000009a8*/ MOV         R2, #48                     @ 0x30 
    /*000009ac*/ MOVW        R1, #12372                  @ 0x3054 
    /*000009b0*/ BL          mn34220_write_register
    /*000009b4*/ MOV         R0, R4 
    /*000009b8*/ MOV         R2, #0 
    /*000009bc*/ MOVW        R1, #12373                  @ 0x3055 
    /*000009c0*/ BL          mn34220_write_register
    /*000009c4*/ MOV         R0, R4 
    /*000009c8*/ MOV         R2, #1 
    /*000009cc*/ MOVW        R1, #12374                  @ 0x3056 
    /*000009d0*/ BL          mn34220_write_register
    /*000009d4*/ MOV         R0, R4 
    /*000009d8*/ MOV         R2, #255                    @ 0xff 
    /*000009dc*/ MOVW        R1, #12375                  @ 0x3057 
    /*000009e0*/ BL          mn34220_write_register
    /*000009e4*/ MOV         R0, R4 
    /*000009e8*/ MOV         R2, #15 
    /*000009ec*/ MOVW        R1, #12376                  @ 0x3058 
    /*000009f0*/ BL          mn34220_write_register
    /*000009f4*/ MOV         R0, R4 
    /*000009f8*/ MOV         R2, #254                    @ 0xfe 
    /*000009fc*/ MOVW        R1, #12377                  @ 0x3059 
    /*00000a00*/ BL          mn34220_write_register
    /*00000a04*/ MOV         R0, R4 
    /*00000a08*/ MOV         R2, #0 
    /*00000a0c*/ MOVW        R1, #12378                  @ 0x305a 
    /*00000a10*/ BL          mn34220_write_register
    /*00000a14*/ MOV         R0, R4 
    /*00000a18*/ MOV         R2, #1 
    /*00000a1c*/ MOVW        R1, #12379                  @ 0x305b 
    /*00000a20*/ BL          mn34220_write_register
    /*00000a24*/ MOV         R0, R4 
    /*00000a28*/ MOV         R2, #0 
    /*00000a2c*/ MOVW        R1, #12380                  @ 0x305c 
    /*00000a30*/ BL          mn34220_write_register
    /*00000a34*/ MOV         R0, R4 
    /*00000a38*/ MOV         R2, #49                     @ 0x31 
    /*00000a3c*/ MOVW        R1, #12381                  @ 0x305d 
    /*00000a40*/ BL          mn34220_write_register
    /*00000a44*/ MOV         R0, R4 
    /*00000a48*/ MOV         R2, #0 
    /*00000a4c*/ MOVW        R1, #12382                  @ 0x305e 
    /*00000a50*/ BL          mn34220_write_register
    /*00000a54*/ MOV         R0, R4 
    /*00000a58*/ MOV         R2, #0 
    /*00000a5c*/ MOVW        R1, #12383                  @ 0x305f 
    /*00000a60*/ BL          mn34220_write_register
    /*00000a64*/ MOV         R0, R4 
    /*00000a68*/ MOV         R2, #0 
    /*00000a6c*/ MOVW        R1, #12384                  @ 0x3060 
    /*00000a70*/ BL          mn34220_write_register
    /*00000a74*/ MOV         R0, R4 
    /*00000a78*/ MOV         R2, #0 
    /*00000a7c*/ MOVW        R1, #12385                  @ 0x3061 
    /*00000a80*/ BL          mn34220_write_register
    /*00000a84*/ MOV         R0, R4 
    /*00000a88*/ MOV         R2, #32 
    /*00000a8c*/ MOVW        R1, #12386                  @ 0x3062 
    /*00000a90*/ BL          mn34220_write_register
    /*00000a94*/ MOV         R0, R4 
    /*00000a98*/ MOV         R2, #20 
    /*00000a9c*/ MOVW        R1, #12387                  @ 0x3063 
    /*00000aa0*/ BL          mn34220_write_register
    /*00000aa4*/ MOV         R0, R4 
    /*00000aa8*/ MOV         R2, #16 
    /*00000aac*/ MOVW        R1, #12388                  @ 0x3064 
    /*00000ab0*/ BL          mn34220_write_register
    /*00000ab4*/ MOV         R0, R4 
    /*00000ab8*/ MOV         R2, #50                     @ 0x32 
    /*00000abc*/ MOVW        R1, #12389                  @ 0x3065 
    /*00000ac0*/ BL          mn34220_write_register
    /*00000ac4*/ MOV         R0, R4 
    /*00000ac8*/ MOV         R2, #0 
    /*00000acc*/ MOVW        R1, #12390                  @ 0x3066 
    /*00000ad0*/ BL          mn34220_write_register
    /*00000ad4*/ MOV         R0, R4 
    /*00000ad8*/ MOV         R2, #2 
    /*00000adc*/ MOVW        R1, #12391                  @ 0x3067 
    /*00000ae0*/ BL          mn34220_write_register
    /*00000ae4*/ MOV         R0, R4 
    /*00000ae8*/ MOV         R2, #0 
    /*00000aec*/ MOVW        R1, #12392                  @ 0x3068 
    /*00000af0*/ BL          mn34220_write_register
    /*00000af4*/ MOV         R0, R4 
    /*00000af8*/ MOV         R2, #0 
    /*00000afc*/ MOVW        R1, #12393                  @ 0x3069 
    /*00000b00*/ BL          mn34220_write_register
    /*00000b04*/ MOV         R0, R4 
    /*00000b08*/ MOV         R2, #0 
    /*00000b0c*/ MOVW        R1, #12394                  @ 0x306a 
    /*00000b10*/ BL          mn34220_write_register
    /*00000b14*/ MOV         R0, R4 
    /*00000b18*/ MOV         R2, #3 
    /*00000b1c*/ MOVW        R1, #12395                  @ 0x306b 
    /*00000b20*/ BL          mn34220_write_register
    /*00000b24*/ MOV         R0, R4 
    /*00000b28*/ MOV         R2, #0 
    /*00000b2c*/ MOVW        R1, #12396                  @ 0x306c 
    /*00000b30*/ BL          mn34220_write_register
    /*00000b34*/ MOV         R0, R4 
    /*00000b38*/ MOV         R2, #1 
    /*00000b3c*/ MOVW        R1, #12397                  @ 0x306d 
    /*00000b40*/ BL          mn34220_write_register
    /*00000b44*/ MOV         R0, R4 
    /*00000b48*/ MOV         R2, #63                     @ 0x3f 
    /*00000b4c*/ MOVW        R1, #12398                  @ 0x306e 
    /*00000b50*/ BL          mn34220_write_register
    /*00000b54*/ MOV         R0, R4 
    /*00000b58*/ MOV         R2, #255                    @ 0xff 
    /*00000b5c*/ MOVW        R1, #12399                  @ 0x306f 
    /*00000b60*/ BL          mn34220_write_register
    /*00000b64*/ MOV         R0, R4 
    /*00000b68*/ MOV         R2, #63                     @ 0x3f 
    /*00000b6c*/ MOVW        R1, #12400                  @ 0x3070 
    /*00000b70*/ BL          mn34220_write_register
    /*00000b74*/ MOV         R0, R4 
    /*00000b78*/ MOV         R2, #255                    @ 0xff 
    /*00000b7c*/ MOVW        R1, #12401                  @ 0x3071 
    /*00000b80*/ BL          mn34220_write_register
    /*00000b84*/ MOV         R0, R4 
    /*00000b88*/ MOV         R2, #63                     @ 0x3f 
    /*00000b8c*/ MOVW        R1, #12402                  @ 0x3072 
    /*00000b90*/ BL          mn34220_write_register
    /*00000b94*/ MOV         R0, R4 
    /*00000b98*/ MOV         R2, #255                    @ 0xff 
    /*00000b9c*/ MOVW        R1, #12403                  @ 0x3073 
    /*00000ba0*/ BL          mn34220_write_register
    /*00000ba4*/ MOV         R0, R4 
    /*00000ba8*/ MOV         R2, #0 
    /*00000bac*/ MOVW        R1, #12404                  @ 0x3074 
    /*00000bb0*/ BL          mn34220_write_register
    /*00000bb4*/ MOV         R0, R4 
    /*00000bb8*/ MOV         R2, #1 
    /*00000bbc*/ MOVW        R1, #12405                  @ 0x3075 
    /*00000bc0*/ BL          mn34220_write_register
    /*00000bc4*/ MOV         R0, R4 
    /*00000bc8*/ MOV         R2, #66                     @ 0x42 
    /*00000bcc*/ MOVW        R1, #12406                  @ 0x3076 
    /*00000bd0*/ BL          mn34220_write_register
    /*00000bd4*/ MOV         R0, R4 
    /*00000bd8*/ MOV         R2, #0 
    /*00000bdc*/ MOVW        R1, #12407                  @ 0x3077 
    /*00000be0*/ BL          mn34220_write_register
    /*00000be4*/ MOV         R0, R4 
    /*00000be8*/ MOV         R2, #0 
    /*00000bec*/ MOVW        R1, #12408                  @ 0x3078 
    /*00000bf0*/ BL          mn34220_write_register
    /*00000bf4*/ MOV         R0, R4 
    /*00000bf8*/ MOV         R2, #19 
    /*00000bfc*/ MOVW        R1, #12409                  @ 0x3079 
    /*00000c00*/ BL          mn34220_write_register
    /*00000c04*/ MOV         R0, R4 
    /*00000c08*/ MOV         R2, #18 
    /*00000c0c*/ MOVW        R1, #12410                  @ 0x307a 
    /*00000c10*/ BL          mn34220_write_register
    /*00000c14*/ MOV         R0, R4 
    /*00000c18*/ MOV         R2, #48                     @ 0x30 
    /*00000c1c*/ MOVW        R1, #12411                  @ 0x307b 
    /*00000c20*/ BL          mn34220_write_register
    /*00000c24*/ MOV         R0, R4 
    /*00000c28*/ MOV         R2, #43                     @ 0x2b 
    /*00000c2c*/ MOVW        R1, #12412                  @ 0x307c 
    /*00000c30*/ BL          mn34220_write_register
    /*00000c34*/ MOV         R0, R4 
    /*00000c38*/ MOV         R2, #16 
    /*00000c3c*/ MOVW        R1, #12413                  @ 0x307d 
    /*00000c40*/ BL          mn34220_write_register
    /*00000c44*/ MOV         R0, R4 
    /*00000c48*/ MOV         R2, #0 
    /*00000c4c*/ MOVW        R1, #12414                  @ 0x307e 
    /*00000c50*/ BL          mn34220_write_register
    /*00000c54*/ MOV         R0, R4 
    /*00000c58*/ MOV         R2, #0 
    /*00000c5c*/ MOVW        R1, #12415                  @ 0x307f 
    /*00000c60*/ BL          mn34220_write_register
    /*00000c64*/ MOV         R0, R4 
    /*00000c68*/ MOV         R2, #0 
    /*00000c6c*/ MOV         R1, #12416                  @ 0x3080 
    /*00000c70*/ BL          mn34220_write_register
    /*00000c74*/ MOV         R0, R4 
    /*00000c78*/ MOV         R2, #0 
    /*00000c7c*/ MOVW        R1, #12417                  @ 0x3081 
    /*00000c80*/ BL          mn34220_write_register
    /*00000c84*/ MOV         R0, R4 
    /*00000c88*/ MOV         R2, #0 
    /*00000c8c*/ MOVW        R1, #12418                  @ 0x3082 
    /*00000c90*/ BL          mn34220_write_register
    /*00000c94*/ MOV         R0, R4 
    /*00000c98*/ MOV         R2, #0 
    /*00000c9c*/ MOVW        R1, #12419                  @ 0x3083 
    /*00000ca0*/ BL          mn34220_write_register
    /*00000ca4*/ MOV         R0, R4 
    /*00000ca8*/ MOV         R2, #0 
    /*00000cac*/ MOVW        R1, #12420                  @ 0x3084 
    /*00000cb0*/ BL          mn34220_write_register
    /*00000cb4*/ MOV         R0, R4 
    /*00000cb8*/ MOV         R2, #48                     @ 0x30 
    /*00000cbc*/ MOVW        R1, #12421                  @ 0x3085 
    /*00000cc0*/ BL          mn34220_write_register
    /*00000cc4*/ MOV         R0, R4 
    /*00000cc8*/ MOV         R2, #35                     @ 0x23 
    /*00000ccc*/ MOVW        R1, #12422                  @ 0x3086 
    /*00000cd0*/ BL          mn34220_write_register
    /*00000cd4*/ MOV         R0, R4 
    /*00000cd8*/ MOV         R2, #3 
    /*00000cdc*/ MOVW        R1, #12423                  @ 0x3087 
    /*00000ce0*/ BL          mn34220_write_register
    /*00000ce4*/ MOV         R0, R4 
    /*00000ce8*/ MOV         R2, #20 
    /*00000cec*/ MOVW        R1, #12424                  @ 0x3088 
    /*00000cf0*/ BL          mn34220_write_register
    /*00000cf4*/ MOV         R0, R4 
    /*00000cf8*/ MOV         R2, #4 
    /*00000cfc*/ MOVW        R1, #12425                  @ 0x3089 
    /*00000d00*/ BL          mn34220_write_register
    /*00000d04*/ MOV         R0, R4 
    /*00000d08*/ MOV         R2, #1 
    /*00000d0c*/ MOVW        R1, #12426                  @ 0x308a 
    /*00000d10*/ BL          mn34220_write_register
    /*00000d14*/ MOV         R0, R4 
    /*00000d18*/ MOV         R2, #11 
    /*00000d1c*/ MOVW        R1, #12427                  @ 0x308b 
    /*00000d20*/ BL          mn34220_write_register
    /*00000d24*/ MOV         R0, R4 
    /*00000d28*/ MOV         R2, #8 
    /*00000d2c*/ MOVW        R1, #12428                  @ 0x308c 
    /*00000d30*/ BL          mn34220_write_register
    /*00000d34*/ MOV         R0, R4 
    /*00000d38*/ MOV         R2, #4 
    /*00000d3c*/ MOVW        R1, #12429                  @ 0x308d 
    /*00000d40*/ BL          mn34220_write_register
    /*00000d44*/ MOV         R0, R4 
    /*00000d48*/ MOV         R2, #4 
    /*00000d4c*/ MOVW        R1, #12430                  @ 0x308e 
    /*00000d50*/ BL          mn34220_write_register
    /*00000d54*/ MOV         R0, R4 
    /*00000d58*/ MOV         R2, #9 
    /*00000d5c*/ MOVW        R1, #12431                  @ 0x308f 
    /*00000d60*/ BL          mn34220_write_register
    /*00000d64*/ MOV         R0, R4 
    /*00000d68*/ MOV         R2, #5 
    /*00000d6c*/ MOVW        R1, #12432                  @ 0x3090 
    /*00000d70*/ BL          mn34220_write_register
    /*00000d74*/ MOV         R0, R4 
    /*00000d78*/ MOV         R2, #0 
    /*00000d7c*/ MOVW        R1, #12433                  @ 0x3091 
    /*00000d80*/ BL          mn34220_write_register
    /*00000d84*/ MOV         R0, R4 
    /*00000d88*/ MOV         R2, #0 
    /*00000d8c*/ MOVW        R1, #12434                  @ 0x3092 
    /*00000d90*/ BL          mn34220_write_register
    /*00000d94*/ MOV         R0, R4 
    /*00000d98*/ MOV         R2, #9 
    /*00000d9c*/ MOVW        R1, #12435                  @ 0x3093 
    /*00000da0*/ BL          mn34220_write_register
    /*00000da4*/ MOV         R0, R4 
    /*00000da8*/ MOV         R2, #0 
    /*00000dac*/ MOVW        R1, #12436                  @ 0x3094 
    /*00000db0*/ BL          mn34220_write_register
    /*00000db4*/ MOV         R0, R4 
    /*00000db8*/ MOV         R2, #0 
    /*00000dbc*/ MOVW        R1, #12437                  @ 0x3095 
    /*00000dc0*/ BL          mn34220_write_register
    /*00000dc4*/ MOV         R0, R4 
    /*00000dc8*/ MOV         R2, #0 
    /*00000dcc*/ MOVW        R1, #12438                  @ 0x3096 
    /*00000dd0*/ BL          mn34220_write_register
    /*00000dd4*/ MOV         R0, R4 
    /*00000dd8*/ MOV         R2, #0 
    /*00000ddc*/ MOVW        R1, #12439                  @ 0x3097 
    /*00000de0*/ BL          mn34220_write_register
    /*00000de4*/ MOV         R0, R4 
    /*00000de8*/ MOV         R2, #0 
    /*00000dec*/ MOVW        R1, #12440                  @ 0x3098 
    /*00000df0*/ BL          mn34220_write_register
    /*00000df4*/ MOV         R0, R4 
    /*00000df8*/ MOV         R2, #0 
    /*00000dfc*/ MOVW        R1, #12441                  @ 0x3099 
    /*00000e00*/ BL          mn34220_write_register
    /*00000e04*/ MOV         R0, R4 
    /*00000e08*/ MOV         R2, #0 
    /*00000e0c*/ MOVW        R1, #12442                  @ 0x309a 
    /*00000e10*/ BL          mn34220_write_register
    /*00000e14*/ MOV         R0, R4 
    /*00000e18*/ MOV         R2, #0 
    /*00000e1c*/ MOVW        R1, #12443                  @ 0x309b 
    /*00000e20*/ BL          mn34220_write_register
    /*00000e24*/ MOV         R0, R4 
    /*00000e28*/ MOV         R2, #83                     @ 0x53 
    /*00000e2c*/ MOVW        R1, #12444                  @ 0x309c 
    /*00000e30*/ BL          mn34220_write_register
    /*00000e34*/ MOV         R0, R4 
    /*00000e38*/ MOV         R2, #84                     @ 0x54 
    /*00000e3c*/ MOVW        R1, #12445                  @ 0x309d 
    /*00000e40*/ BL          mn34220_write_register
    /*00000e44*/ MOV         R0, R4 
    /*00000e48*/ MOV         R2, #1 
    /*00000e4c*/ MOVW        R1, #12446                  @ 0x309e 
    /*00000e50*/ BL          mn34220_write_register
    /*00000e54*/ MOV         R0, R4 
    /*00000e58*/ MOV         R2, #0 
    /*00000e5c*/ MOVW        R1, #12447                  @ 0x309f 
    /*00000e60*/ BL          mn34220_write_register
    /*00000e64*/ MOV         R0, R4 
    /*00000e68*/ MOV         R2, #0 
    /*00000e6c*/ MOV         R1, #12544                  @ 0x3100 
    /*00000e70*/ BL          mn34220_write_register
    /*00000e74*/ MOV         R0, R4 
    /*00000e78*/ MOV         R2, #1 
    /*00000e7c*/ MOVW        R1, #12545                  @ 0x3101 
    /*00000e80*/ BL          mn34220_write_register
    /*00000e84*/ MOV         R0, R4 
    /*00000e88*/ MOV         R2, #0 
    /*00000e8c*/ MOVW        R1, #12546                  @ 0x3102 
    /*00000e90*/ BL          mn34220_write_register
    /*00000e94*/ MOV         R0, R4 
    /*00000e98*/ MOV         R2, #0 
    /*00000e9c*/ MOVW        R1, #12547                  @ 0x3103 
    /*00000ea0*/ BL          mn34220_write_register
    /*00000ea4*/ MOV         R0, R4 
    /*00000ea8*/ MOV         R2, #4 
    /*00000eac*/ MOVW        R1, #12548                  @ 0x3104 
    /*00000eb0*/ BL          mn34220_write_register
    /*00000eb4*/ MOV         R0, R4 
    /*00000eb8*/ MOV         R2, #0 
    /*00000ebc*/ MOVW        R1, #12549                  @ 0x3105 
    /*00000ec0*/ BL          mn34220_write_register
    /*00000ec4*/ MOV         R0, R4 
    /*00000ec8*/ MOV         R2, #0 
    /*00000ecc*/ MOVW        R1, #12550                  @ 0x3106 
    /*00000ed0*/ BL          mn34220_write_register
    /*00000ed4*/ MOV         R0, R4 
    /*00000ed8*/ MOV         R2, #192                    @ 0xc0 
    /*00000edc*/ MOVW        R1, #12551                  @ 0x3107 
    /*00000ee0*/ BL          mn34220_write_register
    /*00000ee4*/ MOV         R0, R4 
    /*00000ee8*/ MOV         R2, #1 
    /*00000eec*/ MOVW        R1, #12552                  @ 0x3108 
    /*00000ef0*/ BL          mn34220_write_register
    /*00000ef4*/ MOV         R0, R4 
    /*00000ef8*/ MOV         R2, #0 
    /*00000efc*/ MOVW        R1, #12553                  @ 0x3109 
    /*00000f00*/ BL          mn34220_write_register
    /*00000f04*/ MOV         R0, R4 
    /*00000f08*/ MOV         R2, #1 
    /*00000f0c*/ MOVW        R1, #12554                  @ 0x310a 
    /*00000f10*/ BL          mn34220_write_register
    /*00000f14*/ MOV         R0, R4 
    /*00000f18*/ MOV         R2, #0 
    /*00000f1c*/ MOVW        R1, #12555                  @ 0x310b 
    /*00000f20*/ BL          mn34220_write_register
    /*00000f24*/ MOV         R0, R4 
    /*00000f28*/ MOV         R2, #1 
    /*00000f2c*/ MOVW        R1, #12556                  @ 0x310c 
    /*00000f30*/ BL          mn34220_write_register
    /*00000f34*/ MOV         R0, R4 
    /*00000f38*/ MOV         R2, #0 
    /*00000f3c*/ MOVW        R1, #12557                  @ 0x310d 
    /*00000f40*/ BL          mn34220_write_register
    /*00000f44*/ MOV         R0, R4 
    /*00000f48*/ MOV         R2, #1 
    /*00000f4c*/ MOVW        R1, #12558                  @ 0x310e 
    /*00000f50*/ BL          mn34220_write_register
    /*00000f54*/ MOV         R0, R4 
    /*00000f58*/ MOV         R2, #0 
    /*00000f5c*/ MOVW        R1, #12559                  @ 0x310f 
    /*00000f60*/ BL          mn34220_write_register
    /*00000f64*/ MOV         R0, R4 
    /*00000f68*/ MOV         R2, #1 
    /*00000f6c*/ MOVW        R1, #12560                  @ 0x3110 
    /*00000f70*/ BL          mn34220_write_register
    /*00000f74*/ MOV         R0, R4 
    /*00000f78*/ MOV         R2, #0 
    /*00000f7c*/ MOVW        R1, #12561                  @ 0x3111 
    /*00000f80*/ BL          mn34220_write_register
    /*00000f84*/ MOV         R0, R4 
    /*00000f88*/ MOV         R2, #1 
    /*00000f8c*/ MOVW        R1, #12562                  @ 0x3112 
    /*00000f90*/ BL          mn34220_write_register
    /*00000f94*/ MOV         R0, R4 
    /*00000f98*/ MOV         R2, #0 
    /*00000f9c*/ MOVW        R1, #12563                  @ 0x3113 
    /*00000fa0*/ BL          mn34220_write_register
    /*00000fa4*/ MOV         R0, R4 
    /*00000fa8*/ MOV         R2, #1 
    /*00000fac*/ MOVW        R1, #12564                  @ 0x3114 
    /*00000fb0*/ BL          mn34220_write_register
    /*00000fb4*/ MOV         R0, R4 
    /*00000fb8*/ MOV         R2, #0 
    /*00000fbc*/ MOVW        R1, #12565                  @ 0x3115 
    /*00000fc0*/ BL          mn34220_write_register
    /*00000fc4*/ MOV         R0, R4 
    /*00000fc8*/ MOV         R2, #1 
    /*00000fcc*/ MOVW        R1, #12566                  @ 0x3116 
    /*00000fd0*/ BL          mn34220_write_register
    /*00000fd4*/ MOV         R0, R4 
    /*00000fd8*/ MOV         R2, #0 
    /*00000fdc*/ MOVW        R1, #12567                  @ 0x3117 
    /*00000fe0*/ BL          mn34220_write_register
    /*00000fe4*/ MOV         R0, R4 
    /*00000fe8*/ MOV         R2, #1 
    /*00000fec*/ MOVW        R1, #12568                  @ 0x3118 
    /*00000ff0*/ BL          mn34220_write_register
    /*00000ff4*/ MOV         R0, R4 
    /*00000ff8*/ MOV         R2, #0 
    /*00000ffc*/ MOVW        R1, #12569                  @ 0x3119 
    /*00001000*/ BL          mn34220_write_register
    /*00001004*/ MOV         R0, R4 
    /*00001008*/ MOV         R2, #1 
    /*0000100c*/ MOVW        R1, #12570                  @ 0x311a 
    /*00001010*/ BL          mn34220_write_register
    /*00001014*/ MOV         R0, R4 
    /*00001018*/ MOV         R2, #0 
    /*0000101c*/ MOVW        R1, #12571                  @ 0x311b 
    /*00001020*/ BL          mn34220_write_register
    /*00001024*/ MOV         R0, R4 
    /*00001028*/ MOV         R2, #1 
    /*0000102c*/ MOVW        R1, #12572                  @ 0x311c 
    /*00001030*/ BL          mn34220_write_register
    /*00001034*/ MOV         R0, R4 
    /*00001038*/ MOV         R2, #0 
    /*0000103c*/ MOVW        R1, #12573                  @ 0x311d 
    /*00001040*/ BL          mn34220_write_register
    /*00001044*/ MOV         R0, R4 
    /*00001048*/ MOV         R2, #1 
    /*0000104c*/ MOVW        R1, #12574                  @ 0x311e 
    /*00001050*/ BL          mn34220_write_register
    /*00001054*/ MOV         R0, R4 
    /*00001058*/ MOV         R2, #0 
    /*0000105c*/ MOVW        R1, #12575                  @ 0x311f 
    /*00001060*/ BL          mn34220_write_register
    /*00001064*/ MOV         R0, R4 
    /*00001068*/ MOV         R2, #1 
    /*0000106c*/ MOVW        R1, #12576                  @ 0x3120 
    /*00001070*/ BL          mn34220_write_register
    /*00001074*/ MOV         R0, R4 
    /*00001078*/ MOV         R2, #0 
    /*0000107c*/ MOVW        R1, #12577                  @ 0x3121 
    /*00001080*/ BL          mn34220_write_register
    /*00001084*/ MOV         R0, R4 
    /*00001088*/ MOV         R2, #0 
    /*0000108c*/ MOVW        R1, #12578                  @ 0x3122 
    /*00001090*/ BL          mn34220_write_register
    /*00001094*/ MOV         R0, R4 
    /*00001098*/ MOV         R2, #0 
    /*0000109c*/ MOVW        R1, #12579                  @ 0x3123 
    /*000010a0*/ BL          mn34220_write_register
    /*000010a4*/ MOV         R0, R4 
    /*000010a8*/ MOV         R2, #0 
    /*000010ac*/ MOVW        R1, #12580                  @ 0x3124 
    /*000010b0*/ BL          mn34220_write_register
    /*000010b4*/ MOV         R0, R4 
    /*000010b8*/ MOV         R2, #0 
    /*000010bc*/ MOVW        R1, #12581                  @ 0x3125 
    /*000010c0*/ BL          mn34220_write_register
    /*000010c4*/ MOV         R0, R4 
    /*000010c8*/ MOV         R2, #0 
    /*000010cc*/ MOVW        R1, #12582                  @ 0x3126 
    /*000010d0*/ BL          mn34220_write_register
    /*000010d4*/ MOV         R0, R4 
    /*000010d8*/ MOV         R2, #2 
    /*000010dc*/ MOVW        R1, #12583                  @ 0x3127 
    /*000010e0*/ BL          mn34220_write_register
    /*000010e4*/ MOV         R0, R4 
    /*000010e8*/ MOV         R2, #0 
    /*000010ec*/ MOVW        R1, #12584                  @ 0x3128 
    /*000010f0*/ BL          mn34220_write_register
    /*000010f4*/ MOV         R0, R4 
    /*000010f8*/ MOV         R2, #0 
    /*000010fc*/ MOVW        R1, #12585                  @ 0x3129 
    /*00001100*/ BL          mn34220_write_register
    /*00001104*/ MOV         R0, R4 
    /*00001108*/ MOV         R2, #0 
    /*0000110c*/ MOVW        R1, #12586                  @ 0x312a 
    /*00001110*/ BL          mn34220_write_register
    /*00001114*/ MOV         R0, R4 
    /*00001118*/ MOV         R2, #32 
    /*0000111c*/ MOVW        R1, #12587                  @ 0x312b 
    /*00001120*/ BL          mn34220_write_register
    /*00001124*/ MOV         R0, R4 
    /*00001128*/ MOV         R2, #0 
    /*0000112c*/ MOVW        R1, #12588                  @ 0x312c 
    /*00001130*/ BL          mn34220_write_register
    /*00001134*/ MOV         R0, R4 
    /*00001138*/ MOV         R2, #32 
    /*0000113c*/ MOVW        R1, #12589                  @ 0x312d 
    /*00001140*/ BL          mn34220_write_register
    /*00001144*/ MOV         R0, R4 
    /*00001148*/ MOV         R2, #0 
    /*0000114c*/ MOVW        R1, #12590                  @ 0x312e 
    /*00001150*/ BL          mn34220_write_register
    /*00001154*/ MOV         R0, R4 
    /*00001158*/ MOV         R2, #32 
    /*0000115c*/ MOVW        R1, #12591                  @ 0x312f 
    /*00001160*/ BL          mn34220_write_register
    /*00001164*/ MOV         R0, R4 
    /*00001168*/ MOV         R2, #15 
    /*0000116c*/ MOVW        R1, #12592                  @ 0x3130 
    /*00001170*/ BL          mn34220_write_register
    /*00001174*/ MOV         R0, R4 
    /*00001178*/ MOV         R2, #1 
    /*0000117c*/ MOVW        R1, #12593                  @ 0x3131 
    /*00001180*/ BL          mn34220_write_register
    /*00001184*/ MOV         R0, R4 
    /*00001188*/ MOV         R2, #103                    @ 0x67 
    /*0000118c*/ MOVW        R1, #12594                  @ 0x3132 
    /*00001190*/ BL          mn34220_write_register
    /*00001194*/ MOV         R0, R4 
    /*00001198*/ MOV         R2, #16 
    /*0000119c*/ MOVW        R1, #12595                  @ 0x3133 
    /*000011a0*/ BL          mn34220_write_register
    /*000011a4*/ MOV         R0, R4 
    /*000011a8*/ MOV         R2, #112                    @ 0x70 
    /*000011ac*/ MOVW        R1, #12596                  @ 0x3134 
    /*000011b0*/ BL          mn34220_write_register
    /*000011b4*/ MOV         R0, R4 
    /*000011b8*/ MOV         R2, #8 
    /*000011bc*/ MOVW        R1, #12597                  @ 0x3135 
    /*000011c0*/ BL          mn34220_write_register
    /*000011c4*/ MOV         R0, R4 
    /*000011c8*/ MOV         R2, #3 
    /*000011cc*/ MOVW        R1, #12598                  @ 0x3136 
    /*000011d0*/ BL          mn34220_write_register
    /*000011d4*/ MOV         R0, R4 
    /*000011d8*/ MOV         R2, #255                    @ 0xff 
    /*000011dc*/ MOVW        R1, #12599                  @ 0x3137 
    /*000011e0*/ BL          mn34220_write_register
    /*000011e4*/ MOV         R0, R4 
    /*000011e8*/ MOV         R2, #0 
    /*000011ec*/ MOVW        R1, #12600                  @ 0x3138 
    /*000011f0*/ BL          mn34220_write_register
    /*000011f4*/ MOV         R0, R4 
    /*000011f8*/ MOV         R2, #0 
    /*000011fc*/ MOVW        R1, #12601                  @ 0x3139 
    /*00001200*/ BL          mn34220_write_register
    /*00001204*/ MOV         R0, R4 
    /*00001208*/ MOV         R2, #0 
    /*0000120c*/ MOVW        R1, #12602                  @ 0x313a 
    /*00001210*/ BL          mn34220_write_register
    /*00001214*/ MOV         R0, R4 
    /*00001218*/ MOV         R2, #1 
    /*0000121c*/ MOVW        R1, #12603                  @ 0x313b 
    /*00001220*/ BL          mn34220_write_register
    /*00001224*/ MOV         R0, R4 
    /*00001228*/ MOV         R2, #0 
    /*0000122c*/ MOVW        R1, #12604                  @ 0x313c 
    /*00001230*/ BL          mn34220_write_register
    /*00001234*/ MOV         R0, R4 
    /*00001238*/ MOV         R2, #0 
    /*0000123c*/ MOVW        R1, #12605                  @ 0x313d 
    /*00001240*/ BL          mn34220_write_register
    /*00001244*/ MOV         R0, R4 
    /*00001248*/ MOV         R2, #0 
    /*0000124c*/ MOV         R1, #12608                  @ 0x3140 
    /*00001250*/ BL          mn34220_write_register
    /*00001254*/ MOV         R0, R4 
    /*00001258*/ MOV         R2, #64                     @ 0x40 
    /*0000125c*/ MOVW        R1, #12609                  @ 0x3141 
    /*00001260*/ BL          mn34220_write_register
    /*00001264*/ MOV         R0, R4 
    /*00001268*/ MOV         R2, #80                     @ 0x50 
    /*0000126c*/ MOVW        R1, #12610                  @ 0x3142 
    /*00001270*/ BL          mn34220_write_register
    /*00001274*/ MOV         R0, R4 
    /*00001278*/ MOV         R2, #3 
    /*0000127c*/ MOVW        R1, #12611                  @ 0x3143 
    /*00001280*/ BL          mn34220_write_register
    /*00001284*/ MOV         R0, R4 
    /*00001288*/ MOV         R2, #4 
    /*0000128c*/ MOVW        R1, #12612                  @ 0x3144 
    /*00001290*/ BL          mn34220_write_register
    /*00001294*/ MOV         R0, R4 
    /*00001298*/ MOV         R2, #3 
    /*0000129c*/ MOVW        R1, #12613                  @ 0x3145 
    /*000012a0*/ BL          mn34220_write_register
    /*000012a4*/ MOV         R0, R4 
    /*000012a8*/ MOV         R2, #5 
    /*000012ac*/ MOVW        R1, #12614                  @ 0x3146 
    /*000012b0*/ BL          mn34220_write_register
    /*000012b4*/ MOV         R0, R4 
    /*000012b8*/ MOV         R2, #5 
    /*000012bc*/ MOVW        R1, #12615                  @ 0x3147 
    /*000012c0*/ BL          mn34220_write_register
    /*000012c4*/ MOV         R0, R4 
    /*000012c8*/ MOV         R2, #2 
    /*000012cc*/ MOVW        R1, #12616                  @ 0x3148 
    /*000012d0*/ BL          mn34220_write_register
    /*000012d4*/ MOV         R0, R4 
    /*000012d8*/ MOV         R2, #2 
    /*000012dc*/ MOVW        R1, #12617                  @ 0x3149 
    /*000012e0*/ BL          mn34220_write_register
    /*000012e4*/ MOV         R0, R4 
    /*000012e8*/ MOV         R2, #5 
    /*000012ec*/ MOVW        R1, #12618                  @ 0x314a 
    /*000012f0*/ BL          mn34220_write_register
    /*000012f4*/ MOV         R0, R4 
    /*000012f8*/ MOV         R2, #3 
    /*000012fc*/ MOVW        R1, #12619                  @ 0x314b 
    /*00001300*/ BL          mn34220_write_register
    /*00001304*/ MOV         R0, R4 
    /*00001308*/ MOV         R2, #6 
    /*0000130c*/ MOVW        R1, #12620                  @ 0x314c 
    /*00001310*/ BL          mn34220_write_register
    /*00001314*/ MOV         R0, R4 
    /*00001318*/ MOV         R2, #7 
    /*0000131c*/ MOVW        R1, #12621                  @ 0x314d 
    /*00001320*/ BL          mn34220_write_register
    /*00001324*/ MOV         R0, R4 
    /*00001328*/ MOV         R2, #6 
    /*0000132c*/ MOVW        R1, #12622                  @ 0x314e 
    /*00001330*/ BL          mn34220_write_register
    /*00001334*/ MOV         R0, R4 
    /*00001338*/ MOV         R2, #6 
    /*0000133c*/ MOVW        R1, #12623                  @ 0x314f 
    /*00001340*/ BL          mn34220_write_register
    /*00001344*/ MOV         R0, R4 
    /*00001348*/ MOV         R2, #7 
    /*0000134c*/ MOVW        R1, #12624                  @ 0x3150 
    /*00001350*/ BL          mn34220_write_register
    /*00001354*/ MOV         R0, R4 
    /*00001358*/ MOV         R2, #1 
    /*0000135c*/ MOVW        R1, #12625                  @ 0x3151 
    /*00001360*/ BL          mn34220_write_register
    /*00001364*/ MOV         R0, R4 
    /*00001368*/ MOV         R2, #6 
    /*0000136c*/ MOVW        R1, #12626                  @ 0x3152 
    /*00001370*/ BL          mn34220_write_register
    /*00001374*/ MOV         R0, R4 
    /*00001378*/ MOV         R2, #241                    @ 0xf1 
    /*0000137c*/ MOVW        R1, #12627                  @ 0x3153 
    /*00001380*/ BL          mn34220_write_register
    /*00001384*/ MOV         R0, R4 
    /*00001388*/ MOV         R2, #0 
    /*0000138c*/ MOVW        R1, #12628                  @ 0x3154 
    /*00001390*/ BL          mn34220_write_register
    /*00001394*/ MOV         R0, R4 
    /*00001398*/ MOV         R2, #202                    @ 0xca 
    /*0000139c*/ MOVW        R1, #12629                  @ 0x3155 
    /*000013a0*/ BL          mn34220_write_register
    /*000013a4*/ MOV         R0, R4 
    /*000013a8*/ MOV         R2, #0 
    /*000013ac*/ MOVW        R1, #12630                  @ 0x3156 
    /*000013b0*/ BL          mn34220_write_register
    /*000013b4*/ MOV         R0, R4 
    /*000013b8*/ MOV         R2, #202                    @ 0xca 
    /*000013bc*/ MOVW        R1, #12631                  @ 0x3157 
    /*000013c0*/ BL          mn34220_write_register
    /*000013c4*/ MOV         R0, R4 
    /*000013c8*/ MOV         R2, #0 
    /*000013cc*/ MOVW        R1, #12632                  @ 0x3158 
    /*000013d0*/ BL          mn34220_write_register
    /*000013d4*/ MOV         R0, R4 
    /*000013d8*/ MOV         R2, #202                    @ 0xca 
    /*000013dc*/ MOVW        R1, #12633                  @ 0x3159 
    /*000013e0*/ BL          mn34220_write_register
    /*000013e4*/ MOV         R0, R4 
    /*000013e8*/ MOV         R2, #0 
    /*000013ec*/ MOVW        R1, #12634                  @ 0x315a 
    /*000013f0*/ BL          mn34220_write_register
    /*000013f4*/ MOV         R0, R4 
    /*000013f8*/ MOV         R2, #202                    @ 0xca 
    /*000013fc*/ MOVW        R1, #12635                  @ 0x315b 
    /*00001400*/ BL          mn34220_write_register
    /*00001404*/ MOV         R0, R4 
    /*00001408*/ MOV         R2, #0 
    /*0000140c*/ MOVW        R1, #12636                  @ 0x315c 
    /*00001410*/ BL          mn34220_write_register
    /*00001414*/ MOV         R0, R4 
    /*00001418*/ MOV         R2, #202                    @ 0xca 
    /*0000141c*/ MOVW        R1, #12637                  @ 0x315d 
    /*00001420*/ BL          mn34220_write_register
    /*00001424*/ MOV         R0, R4 
    /*00001428*/ MOV         R2, #0 
    /*0000142c*/ MOVW        R1, #12638                  @ 0x315e 
    /*00001430*/ BL          mn34220_write_register
    /*00001434*/ MOV         R0, R4 
    /*00001438*/ MOV         R2, #202                    @ 0xca 
    /*0000143c*/ MOVW        R1, #12639                  @ 0x315f 
    /*00001440*/ BL          mn34220_write_register
    /*00001444*/ MOV         R0, R4 
    /*00001448*/ MOV         R2, #0 
    /*0000144c*/ MOVW        R1, #12640                  @ 0x3160 
    /*00001450*/ BL          mn34220_write_register
    /*00001454*/ MOV         R0, R4 
    /*00001458*/ MOV         R2, #202                    @ 0xca 
    /*0000145c*/ MOVW        R1, #12641                  @ 0x3161 
    /*00001460*/ BL          mn34220_write_register
    /*00001464*/ MOV         R0, R4 
    /*00001468*/ MOV         R2, #0 
    /*0000146c*/ MOVW        R1, #12642                  @ 0x3162 
    /*00001470*/ BL          mn34220_write_register
    /*00001474*/ MOV         R0, R4 
    /*00001478*/ MOV         R2, #202                    @ 0xca 
    /*0000147c*/ MOVW        R1, #12643                  @ 0x3163 
    /*00001480*/ BL          mn34220_write_register
    /*00001484*/ MOV         R0, R4 
    /*00001488*/ MOV         R2, #0 
    /*0000148c*/ MOVW        R1, #12644                  @ 0x3164 
    /*00001490*/ BL          mn34220_write_register
    /*00001494*/ MOV         R0, R4 
    /*00001498*/ MOV         R2, #202                    @ 0xca 
    /*0000149c*/ MOVW        R1, #12645                  @ 0x3165 
    /*000014a0*/ BL          mn34220_write_register
    /*000014a4*/ MOV         R0, R4 
    /*000014a8*/ MOV         R2, #0 
    /*000014ac*/ MOVW        R1, #12646                  @ 0x3166 
    /*000014b0*/ BL          mn34220_write_register
    /*000014b4*/ MOV         R0, R4 
    /*000014b8*/ MOV         R2, #202                    @ 0xca 
    /*000014bc*/ MOVW        R1, #12647                  @ 0x3167 
    /*000014c0*/ BL          mn34220_write_register
    /*000014c4*/ MOV         R0, R4 
    /*000014c8*/ MOV         R2, #0 
    /*000014cc*/ MOVW        R1, #12648                  @ 0x3168 
    /*000014d0*/ BL          mn34220_write_register
    /*000014d4*/ MOV         R0, R4 
    /*000014d8*/ MOV         R2, #202                    @ 0xca 
    /*000014dc*/ MOVW        R1, #12649                  @ 0x3169 
    /*000014e0*/ BL          mn34220_write_register
    /*000014e4*/ MOV         R0, R4 
    /*000014e8*/ MOV         R2, #0 
    /*000014ec*/ MOVW        R1, #12650                  @ 0x316a 
    /*000014f0*/ BL          mn34220_write_register
    /*000014f4*/ MOV         R0, R4 
    /*000014f8*/ MOV         R2, #202                    @ 0xca 
    /*000014fc*/ MOVW        R1, #12651                  @ 0x316b 
    /*00001500*/ BL          mn34220_write_register
    /*00001504*/ MOV         R0, R4 
    /*00001508*/ MOV         R2, #0 
    /*0000150c*/ MOVW        R1, #12652                  @ 0x316c 
    /*00001510*/ BL          mn34220_write_register
    /*00001514*/ MOV         R0, R4 
    /*00001518*/ MOV         R2, #202                    @ 0xca 
    /*0000151c*/ MOVW        R1, #12653                  @ 0x316d 
    /*00001520*/ BL          mn34220_write_register
    /*00001524*/ MOV         R0, R4 
    /*00001528*/ MOV         R2, #0 
    /*0000152c*/ MOVW        R1, #12654                  @ 0x316e 
    /*00001530*/ BL          mn34220_write_register
    /*00001534*/ MOV         R0, R4 
    /*00001538*/ MOV         R2, #202                    @ 0xca 
    /*0000153c*/ MOVW        R1, #12655                  @ 0x316f 
    /*00001540*/ BL          mn34220_write_register
    /*00001544*/ MOV         R0, R4 
    /*00001548*/ MOV         R2, #0 
    /*0000154c*/ MOVW        R1, #12656                  @ 0x3170 
    /*00001550*/ BL          mn34220_write_register
    /*00001554*/ MOV         R0, R4 
    /*00001558*/ MOV         R2, #202                    @ 0xca 
    /*0000155c*/ MOVW        R1, #12657                  @ 0x3171 
    /*00001560*/ BL          mn34220_write_register
    /*00001564*/ MOV         R0, R4 
    /*00001568*/ MOV         R2, #0 
    /*0000156c*/ MOVW        R1, #12658                  @ 0x3172 
    /*00001570*/ BL          mn34220_write_register
    /*00001574*/ MOV         R0, R4 
    /*00001578*/ MOV         R2, #202                    @ 0xca 
    /*0000157c*/ MOVW        R1, #12659                  @ 0x3173 
    /*00001580*/ BL          mn34220_write_register
    /*00001584*/ MOV         R0, R4 
    /*00001588*/ MOV         R2, #11 
    /*0000158c*/ MOVW        R1, #12660                  @ 0x3174 
    /*00001590*/ BL          mn34220_write_register
    /*00001594*/ MOV         R0, R4 
    /*00001598*/ MOV         R2, #128                    @ 0x80 
    /*0000159c*/ MOVW        R1, #12661                  @ 0x3175 
    /*000015a0*/ BL          mn34220_write_register
    /*000015a4*/ MOV         R0, R4 
    /*000015a8*/ MOV         R2, #136                    @ 0x88 
    /*000015ac*/ MOVW        R1, #12662                  @ 0x3176 
    /*000015b0*/ BL          mn34220_write_register
    /*000015b4*/ MOV         R0, R4 
    /*000015b8*/ MOV         R2, #160                    @ 0xa0 
    /*000015bc*/ MOVW        R1, #12663                  @ 0x3177 
    /*000015c0*/ BL          mn34220_write_register
    /*000015c4*/ MOV         R0, R4 
    /*000015c8*/ MOV         R2, #8 
    /*000015cc*/ MOVW        R1, #12664                  @ 0x3178 
    /*000015d0*/ BL          mn34220_write_register
    /*000015d4*/ MOV         R0, R4 
    /*000015d8*/ MOV         R2, #0 
    /*000015dc*/ MOVW        R1, #12665                  @ 0x3179 
    /*000015e0*/ BL          mn34220_write_register
    /*000015e4*/ MOV         R0, R4 
    /*000015e8*/ MOV         R2, #160                    @ 0xa0 
    /*000015ec*/ MOVW        R1, #12666                  @ 0x317a 
    /*000015f0*/ BL          mn34220_write_register
    /*000015f4*/ MOV         R0, R4 
    /*000015f8*/ MOV         R2, #162                    @ 0xa2 
    /*000015fc*/ MOVW        R1, #12667                  @ 0x317b 
    /*00001600*/ BL          mn34220_write_register
    /*00001604*/ MOV         R0, R4 
    /*00001608*/ MOV         R2, #160                    @ 0xa0 
    /*0000160c*/ MOVW        R1, #12668                  @ 0x317c 
    /*00001610*/ BL          mn34220_write_register
    /*00001614*/ MOV         R0, R4 
    /*00001618*/ MOV         R2, #2 
    /*0000161c*/ MOVW        R1, #12669                  @ 0x317d 
    /*00001620*/ BL          mn34220_write_register
    /*00001624*/ MOV         R0, R4 
    /*00001628*/ MOV         R2, #0 
    /*0000162c*/ MOVW        R1, #12670                  @ 0x317e 
    /*00001630*/ BL          mn34220_write_register
    /*00001634*/ MOV         R0, R4 
    /*00001638*/ MOV         R2, #0 
    /*0000163c*/ MOVW        R1, #12671                  @ 0x317f 
    /*00001640*/ BL          mn34220_write_register
    /*00001644*/ MOV         R0, R4 
    /*00001648*/ MOV         R2, #0 
    /*0000164c*/ MOV         R1, #12672                  @ 0x3180 
    /*00001650*/ BL          mn34220_write_register
    /*00001654*/ MOV         R0, R4 
    /*00001658*/ MOV         R2, #130                    @ 0x82 
    /*0000165c*/ MOVW        R1, #12673                  @ 0x3181 
    /*00001660*/ BL          mn34220_write_register
    /*00001664*/ MOV         R0, R4 
    /*00001668*/ MOV         R2, #32 
    /*0000166c*/ MOVW        R1, #12674                  @ 0x3182 
    /*00001670*/ BL          mn34220_write_register
    /*00001674*/ MOV         R0, R4 
    /*00001678*/ MOV         R2, #32 
    /*0000167c*/ MOVW        R1, #12675                  @ 0x3183 
    /*00001680*/ BL          mn34220_write_register
    /*00001684*/ MOV         R0, R4 
    /*00001688*/ MOV         R2, #2 
    /*0000168c*/ MOVW        R1, #12676                  @ 0x3184 
    /*00001690*/ BL          mn34220_write_register
    /*00001694*/ MOV         R0, R4 
    /*00001698*/ MOV         R2, #0 
    /*0000169c*/ MOVW        R1, #12677                  @ 0x3185 
    /*000016a0*/ BL          mn34220_write_register
    /*000016a4*/ MOV         R0, R4 
    /*000016a8*/ MOV         R2, #72                     @ 0x48 
    /*000016ac*/ MOVW        R1, #12678                  @ 0x3186 
    /*000016b0*/ BL          mn34220_write_register
    /*000016b4*/ MOV         R0, R4 
    /*000016b8*/ MOV         R2, #16 
    /*000016bc*/ MOVW        R1, #12679                  @ 0x3187 
    /*000016c0*/ BL          mn34220_write_register
    /*000016c4*/ MOV         R0, R4 
    /*000016c8*/ MOV         R2, #0 
    /*000016cc*/ MOVW        R1, #12680                  @ 0x3188 
    /*000016d0*/ BL          mn34220_write_register
    /*000016d4*/ MOV         R0, R4 
    /*000016d8*/ MOV         R2, #222                    @ 0xde 
    /*000016dc*/ MOVW        R1, #12681                  @ 0x3189 
    /*000016e0*/ BL          mn34220_write_register
    /*000016e4*/ MOV         R0, R4 
    /*000016e8*/ MOV         R2, #107                    @ 0x6b 
    /*000016ec*/ MOVW        R1, #12682                  @ 0x318a 
    /*000016f0*/ BL          mn34220_write_register
    /*000016f4*/ MOV         R0, R4 
    /*000016f8*/ MOV         R2, #15 
    /*000016fc*/ MOVW        R1, #12683                  @ 0x318b 
    /*00001700*/ BL          mn34220_write_register
    /*00001704*/ MOV         R0, R4 
    /*00001708*/ MOV         R2, #31 
    /*0000170c*/ MOVW        R1, #12684                  @ 0x318c 
    /*00001710*/ BL          mn34220_write_register
    /*00001714*/ MOV         R0, R4 
    /*00001718*/ MOV         R2, #0 
    /*0000171c*/ MOVW        R1, #12685                  @ 0x318d 
    /*00001720*/ BL          mn34220_write_register
    /*00001724*/ MOV         R0, R4 
    /*00001728*/ MOV         R2, #32 
    /*0000172c*/ MOVW        R1, #12686                  @ 0x318e 
    /*00001730*/ BL          mn34220_write_register
    /*00001734*/ MOV         R0, R4 
    /*00001738*/ MOV         R2, #112                    @ 0x70 
    /*0000173c*/ MOVW        R1, #12687                  @ 0x318f 
    /*00001740*/ BL          mn34220_write_register
    /*00001744*/ MOV         R0, R4 
    /*00001748*/ MOV         R2, #0 
    /*0000174c*/ MOVW        R1, #12688                  @ 0x3190 
    /*00001750*/ BL          mn34220_write_register
    /*00001754*/ MOV         R0, R4 
    /*00001758*/ MOV         R2, #0 
    /*0000175c*/ MOVW        R1, #12689                  @ 0x3191 
    /*00001760*/ BL          mn34220_write_register
    /*00001764*/ MOV         R0, R4 
    /*00001768*/ MOV         R2, #0 
    /*0000176c*/ MOVW        R1, #12690                  @ 0x3192 
    /*00001770*/ BL          mn34220_write_register
    /*00001774*/ MOV         R0, R4 
    /*00001778*/ MOV         R2, #0 
    /*0000177c*/ MOVW        R1, #12691                  @ 0x3193 
    /*00001780*/ BL          mn34220_write_register
    /*00001784*/ MOV         R0, R4 
    /*00001788*/ MOV         R2, #0 
    /*0000178c*/ MOVW        R1, #12692                  @ 0x3194 
    /*00001790*/ BL          mn34220_write_register
    /*00001794*/ MOV         R0, R4 
    /*00001798*/ MOV         R2, #64                     @ 0x40 
    /*0000179c*/ MOVW        R1, #12693                  @ 0x3195 
    /*000017a0*/ BL          mn34220_write_register
    /*000017a4*/ MOV         R0, R4 
    /*000017a8*/ MOV         R2, #8 
    /*000017ac*/ MOVW        R1, #12694                  @ 0x3196 
    /*000017b0*/ BL          mn34220_write_register
    /*000017b4*/ MOV         R0, R4 
    /*000017b8*/ MOV         R2, #6 
    /*000017bc*/ MOVW        R1, #12695                  @ 0x3197 
    /*000017c0*/ BL          mn34220_write_register
    /*000017c4*/ MOV         R0, R4 
    /*000017c8*/ MOV         R2, #0 
    /*000017cc*/ MOVW        R1, #12696                  @ 0x3198 
    /*000017d0*/ BL          mn34220_write_register
    /*000017d4*/ MOV         R0, R4 
    /*000017d8*/ MOV         R2, #0 
    /*000017dc*/ MOVW        R1, #12697                  @ 0x3199 
    /*000017e0*/ BL          mn34220_write_register
    /*000017e4*/ MOV         R0, R4 
    /*000017e8*/ MOV         R2, #14 
    /*000017ec*/ MOVW        R1, #12698                  @ 0x319a 
    /*000017f0*/ BL          mn34220_write_register
    /*000017f4*/ MOV         R0, R4 
    /*000017f8*/ MOV         R2, #16 
    /*000017fc*/ MOVW        R1, #12699                  @ 0x319b 
    /*00001800*/ BL          mn34220_write_register
    /*00001804*/ MOV         R0, R4 
    /*00001808*/ MOV         R2, #50                     @ 0x32 
    /*0000180c*/ MOVW        R1, #12700                  @ 0x319c 
    /*00001810*/ BL          mn34220_write_register
    /*00001814*/ MOV         R0, R4 
    /*00001818*/ MOV         R2, #84                     @ 0x54 
    /*0000181c*/ MOVW        R1, #12701                  @ 0x319d 
    /*00001820*/ BL          mn34220_write_register
    /*00001824*/ MOV         R0, R4 
    /*00001828*/ MOV         R2, #118                    @ 0x76 
    /*0000182c*/ MOVW        R1, #12702                  @ 0x319e 
    /*00001830*/ BL          mn34220_write_register
    /*00001834*/ MOV         R0, R4 
    /*00001838*/ MOV         R2, #152                    @ 0x98 
    /*0000183c*/ MOVW        R1, #12703                  @ 0x319f 
    /*00001840*/ BL          mn34220_write_register
    /*00001844*/ MOV         R0, R4 
    /*00001848*/ MOV         R2, #0 
    /*0000184c*/ MOVW        R1, #12704                  @ 0x31a0 
    /*00001850*/ BL          mn34220_write_register
    /*00001854*/ MOV         R0, R4 
    /*00001858*/ MOV         R2, #0 
    /*0000185c*/ MOVW        R1, #12705                  @ 0x31a1 
    /*00001860*/ BL          mn34220_write_register
    /*00001864*/ MOV         R0, R4 
    /*00001868*/ MOV         R2, #0 
    /*0000186c*/ MOVW        R1, #12706                  @ 0x31a2 
    /*00001870*/ BL          mn34220_write_register
    /*00001874*/ MOV         R0, R4 
    /*00001878*/ MOV         R2, #16 
    /*0000187c*/ MOVW        R1, #12707                  @ 0x31a3 
    /*00001880*/ BL          mn34220_write_register
    /*00001884*/ MOV         R0, R4 
    /*00001888*/ MOV         R2, #50                     @ 0x32 
    /*0000188c*/ MOVW        R1, #12708                  @ 0x31a4 
    /*00001890*/ BL          mn34220_write_register
    /*00001894*/ MOV         R0, R4 
    /*00001898*/ MOV         R2, #84                     @ 0x54 
    /*0000189c*/ MOVW        R1, #12709                  @ 0x31a5 
    /*000018a0*/ BL          mn34220_write_register
    /*000018a4*/ MOV         R0, R4 
    /*000018a8*/ MOV         R2, #134                    @ 0x86 
    /*000018ac*/ MOVW        R1, #12710                  @ 0x31a6 
    /*000018b0*/ BL          mn34220_write_register
    /*000018b4*/ MOV         R0, R4 
    /*000018b8*/ MOV         R2, #9 
    /*000018bc*/ MOVW        R1, #12711                  @ 0x31a7 
    /*000018c0*/ BL          mn34220_write_register
    /*000018c4*/ MOV         R0, R4 
    /*000018c8*/ MOV         R2, #0 
    /*000018cc*/ MOVW        R1, #12712                  @ 0x31a8 
    /*000018d0*/ BL          mn34220_write_register
    /*000018d4*/ MOV         R0, R4 
    /*000018d8*/ MOV         R2, #0 
    /*000018dc*/ MOVW        R1, #12713                  @ 0x31a9 
    /*000018e0*/ BL          mn34220_write_register
    /*000018e4*/ MOV         R0, R4 
    /*000018e8*/ MOV         R2, #0 
    /*000018ec*/ MOVW        R1, #12714                  @ 0x31aa 
    /*000018f0*/ BL          mn34220_write_register
    /*000018f4*/ MOV         R0, R4 
    /*000018f8*/ MOV         R2, #36                     @ 0x24 
    /*000018fc*/ MOVW        R1, #12715                  @ 0x31ab 
    /*00001900*/ BL          mn34220_write_register
    /*00001904*/ MOV         R0, R4 
    /*00001908*/ MOV         R2, #0 
    /*0000190c*/ MOVW        R1, #12716                  @ 0x31ac 
    /*00001910*/ BL          mn34220_write_register
    /*00001914*/ MOV         R0, R4 
    /*00001918*/ MOV         R2, #137                    @ 0x89 
    /*0000191c*/ MOVW        R1, #12717                  @ 0x31ad 
    /*00001920*/ BL          mn34220_write_register
    /*00001924*/ MOV         R0, R4 
    /*00001928*/ MOV         R2, #2 
    /*0000192c*/ MOVW        R1, #12718                  @ 0x31ae 
    /*00001930*/ BL          mn34220_write_register
    /*00001934*/ MOV         R0, R4 
    /*00001938*/ MOV         R2, #0 
    /*0000193c*/ MOVW        R1, #12719                  @ 0x31af 
    /*00001940*/ BL          mn34220_write_register
    /*00001944*/ MOV         R0, R4 
    /*00001948*/ MOV         R2, #0 
    /*0000194c*/ MOVW        R1, #12720                  @ 0x31b0 
    /*00001950*/ BL          mn34220_write_register
    /*00001954*/ MOV         R0, R4 
    /*00001958*/ MOV         R2, #0 
    /*0000195c*/ MOVW        R1, #12721                  @ 0x31b1 
    /*00001960*/ BL          mn34220_write_register
    /*00001964*/ MOV         R0, R4 
    /*00001968*/ MOV         R2, #0 
    /*0000196c*/ MOVW        R1, #12722                  @ 0x31b2 
    /*00001970*/ BL          mn34220_write_register
    /*00001974*/ MOV         R0, R4 
    /*00001978*/ MOV         R2, #0 
    /*0000197c*/ MOVW        R1, #12723                  @ 0x31b3 
    /*00001980*/ BL          mn34220_write_register
    /*00001984*/ MOV         R0, R4 
    /*00001988*/ MOV         R2, #0 
    /*0000198c*/ MOVW        R1, #12724                  @ 0x31b4 
    /*00001990*/ BL          mn34220_write_register
    /*00001994*/ MOV         R0, R4 
    /*00001998*/ MOV         R2, #0 
    /*0000199c*/ MOVW        R1, #12725                  @ 0x31b5 
    /*000019a0*/ BL          mn34220_write_register
    /*000019a4*/ MOV         R0, R4 
    /*000019a8*/ MOV         R2, #0 
    /*000019ac*/ MOVW        R1, #12726                  @ 0x31b6 
    /*000019b0*/ BL          mn34220_write_register
    /*000019b4*/ MOV         R0, R4 
    /*000019b8*/ MOV         R2, #0 
    /*000019bc*/ MOVW        R1, #12727                  @ 0x31b7 
    /*000019c0*/ BL          mn34220_write_register
    /*000019c4*/ MOV         R0, R4 
    /*000019c8*/ MOV         R2, #0 
    /*000019cc*/ MOVW        R1, #12728                  @ 0x31b8 
    /*000019d0*/ BL          mn34220_write_register
    /*000019d4*/ MOV         R0, R4 
    /*000019d8*/ MOV         R2, #0 
    /*000019dc*/ MOVW        R1, #12729                  @ 0x31b9 
    /*000019e0*/ BL          mn34220_write_register
    /*000019e4*/ MOV         R0, R4 
    /*000019e8*/ MOV         R2, #0 
    /*000019ec*/ MOVW        R1, #12730                  @ 0x31ba 
    /*000019f0*/ BL          mn34220_write_register
    /*000019f4*/ MOV         R0, R4 
    /*000019f8*/ MOV         R2, #0 
    /*000019fc*/ MOVW        R1, #12731                  @ 0x31bb 
    /*00001a00*/ BL          mn34220_write_register
    /*00001a04*/ MOV         R0, R4 
    /*00001a08*/ MOV         R2, #0 
    /*00001a0c*/ MOVW        R1, #12732                  @ 0x31bc 
    /*00001a10*/ BL          mn34220_write_register
    /*00001a14*/ MOV         R0, R4 
    /*00001a18*/ MOV         R2, #0 
    /*00001a1c*/ MOVW        R1, #12733                  @ 0x31bd 
    /*00001a20*/ BL          mn34220_write_register
    /*00001a24*/ MOV         R0, R4 
    /*00001a28*/ MOV         R2, #0 
    /*00001a2c*/ MOVW        R1, #12734                  @ 0x31be 
    /*00001a30*/ BL          mn34220_write_register
    /*00001a34*/ MOV         R0, R4 
    /*00001a38*/ MOV         R2, #0 
    /*00001a3c*/ MOVW        R1, #12735                  @ 0x31bf 
    /*00001a40*/ BL          mn34220_write_register
    /*00001a44*/ MOV         R0, R4 
    /*00001a48*/ MOV         R2, #0 
    /*00001a4c*/ MOV         R1, #12736                  @ 0x31c0 
    /*00001a50*/ BL          mn34220_write_register
    /*00001a54*/ MOV         R0, R4 
    /*00001a58*/ MOV         R2, #0 
    /*00001a5c*/ MOVW        R1, #12737                  @ 0x31c1 
    /*00001a60*/ BL          mn34220_write_register
    /*00001a64*/ MOV         R0, R4 
    /*00001a68*/ MOV         R2, #0 
    /*00001a6c*/ MOVW        R1, #12738                  @ 0x31c2 
    /*00001a70*/ BL          mn34220_write_register
    /*00001a74*/ MOV         R0, R4 
    /*00001a78*/ MOV         R2, #0 
    /*00001a7c*/ MOVW        R1, #12739                  @ 0x31c3 
    /*00001a80*/ BL          mn34220_write_register
    /*00001a84*/ MOV         R0, R4 
    /*00001a88*/ MOV         R2, #0 
    /*00001a8c*/ MOVW        R1, #12740                  @ 0x31c4 
    /*00001a90*/ BL          mn34220_write_register
    /*00001a94*/ MOV         R0, R4 
    /*00001a98*/ MOV         R2, #0 
    /*00001a9c*/ MOVW        R1, #12741                  @ 0x31c5 
    /*00001aa0*/ BL          mn34220_write_register
    /*00001aa4*/ MOV         R0, R4 
    /*00001aa8*/ MOV         R2, #0 
    /*00001aac*/ MOVW        R1, #12742                  @ 0x31c6 
    /*00001ab0*/ BL          mn34220_write_register
    /*00001ab4*/ MOV         R0, R4 
    /*00001ab8*/ MOV         R2, #0 
    /*00001abc*/ MOVW        R1, #12743                  @ 0x31c7 
    /*00001ac0*/ BL          mn34220_write_register
    /*00001ac4*/ MOV         R0, R4 
    /*00001ac8*/ MOV         R2, #0 
    /*00001acc*/ MOVW        R1, #12744                  @ 0x31c8 
    /*00001ad0*/ BL          mn34220_write_register
    /*00001ad4*/ MOV         R0, R4 
    /*00001ad8*/ MOV         R2, #0 
    /*00001adc*/ MOVW        R1, #12745                  @ 0x31c9 
    /*00001ae0*/ BL          mn34220_write_register
    /*00001ae4*/ MOV         R0, R4 
    /*00001ae8*/ MOV         R2, #0 
    /*00001aec*/ MOVW        R1, #12746                  @ 0x31ca 
    /*00001af0*/ BL          mn34220_write_register
    /*00001af4*/ MOV         R0, R4 
    /*00001af8*/ MOV         R2, #0 
    /*00001afc*/ MOVW        R1, #12747                  @ 0x31cb 
    /*00001b00*/ BL          mn34220_write_register
    /*00001b04*/ MOV         R0, R4 
    /*00001b08*/ MOV         R2, #0 
    /*00001b0c*/ MOVW        R1, #12748                  @ 0x31cc 
    /*00001b10*/ BL          mn34220_write_register
    /*00001b14*/ MOV         R0, R4 
    /*00001b18*/ MOV         R2, #0 
    /*00001b1c*/ MOVW        R1, #12749                  @ 0x31cd 
    /*00001b20*/ BL          mn34220_write_register
    /*00001b24*/ MOV         R0, R4 
    /*00001b28*/ MOV         R2, #0 
    /*00001b2c*/ MOVW        R1, #12750                  @ 0x31ce 
    /*00001b30*/ BL          mn34220_write_register
    /*00001b34*/ MOV         R0, R4 
    /*00001b38*/ MOV         R2, #0 
    /*00001b3c*/ MOVW        R1, #12751                  @ 0x31cf 
    /*00001b40*/ BL          mn34220_write_register
    /*00001b44*/ MOV         R0, R4 
    /*00001b48*/ MOV         R2, #0 
    /*00001b4c*/ MOVW        R1, #12752                  @ 0x31d0 
    /*00001b50*/ BL          mn34220_write_register
    /*00001b54*/ MOV         R0, R4 
    /*00001b58*/ MOV         R2, #0 
    /*00001b5c*/ MOVW        R1, #12753                  @ 0x31d1 
    /*00001b60*/ BL          mn34220_write_register
    /*00001b64*/ MOV         R0, R4 
    /*00001b68*/ MOV         R2, #0 
    /*00001b6c*/ MOVW        R1, #12754                  @ 0x31d2 
    /*00001b70*/ BL          mn34220_write_register
    /*00001b74*/ MOV         R0, R4 
    /*00001b78*/ MOV         R2, #0 
    /*00001b7c*/ MOVW        R1, #12755                  @ 0x31d3 
    /*00001b80*/ BL          mn34220_write_register
    /*00001b84*/ MOV         R0, R4 
    /*00001b88*/ MOV         R2, #0 
    /*00001b8c*/ MOVW        R1, #12756                  @ 0x31d4 
    /*00001b90*/ BL          mn34220_write_register
    /*00001b94*/ MOV         R0, R4 
    /*00001b98*/ MOV         R2, #0 
    /*00001b9c*/ MOVW        R1, #12757                  @ 0x31d5 
    /*00001ba0*/ BL          mn34220_write_register
    /*00001ba4*/ MOV         R0, R4 
    /*00001ba8*/ MOV         R2, #0 
    /*00001bac*/ MOVW        R1, #12758                  @ 0x31d6 
    /*00001bb0*/ BL          mn34220_write_register
    /*00001bb4*/ MOV         R0, R4 
    /*00001bb8*/ MOV         R2, #0 
    /*00001bbc*/ MOVW        R1, #12759                  @ 0x31d7 
    /*00001bc0*/ BL          mn34220_write_register
    /*00001bc4*/ MOV         R0, R4 
    /*00001bc8*/ MOV         R2, #0 
    /*00001bcc*/ MOVW        R1, #12760                  @ 0x31d8 
    /*00001bd0*/ BL          mn34220_write_register
    /*00001bd4*/ MOV         R0, R4 
    /*00001bd8*/ MOV         R2, #0 
    /*00001bdc*/ MOVW        R1, #12761                  @ 0x31d9 
    /*00001be0*/ BL          mn34220_write_register
    /*00001be4*/ MOV         R0, R4 
    /*00001be8*/ MOV         R2, #0 
    /*00001bec*/ MOVW        R1, #12762                  @ 0x31da 
    /*00001bf0*/ BL          mn34220_write_register
    /*00001bf4*/ MOV         R0, R4 
    /*00001bf8*/ MOV         R2, #0 
    /*00001bfc*/ MOVW        R1, #12763                  @ 0x31db 
    /*00001c00*/ BL          mn34220_write_register
    /*00001c04*/ MOV         R0, R4 
    /*00001c08*/ MOV         R2, #0 
    /*00001c0c*/ MOVW        R1, #12764                  @ 0x31dc 
    /*00001c10*/ BL          mn34220_write_register
    /*00001c14*/ MOV         R0, R4 
    /*00001c18*/ MOV         R2, #0 
    /*00001c1c*/ MOVW        R1, #12765                  @ 0x31dd 
    /*00001c20*/ BL          mn34220_write_register
    /*00001c24*/ MOV         R0, R4 
    /*00001c28*/ MOV         R2, #0 
    /*00001c2c*/ MOVW        R1, #12766                  @ 0x31de 
    /*00001c30*/ BL          mn34220_write_register
    /*00001c34*/ MOV         R0, R4 
    /*00001c38*/ MOV         R2, #0 
    /*00001c3c*/ MOVW        R1, #12767                  @ 0x31df 
    /*00001c40*/ BL          mn34220_write_register
    /*00001c44*/ MOV         R0, R4 
    /*00001c48*/ MOV         R2, #0 
    /*00001c4c*/ MOVW        R1, #12768                  @ 0x31e0 
    /*00001c50*/ BL          mn34220_write_register
    /*00001c54*/ MOV         R0, R4 
    /*00001c58*/ MOV         R2, #0 
    /*00001c5c*/ MOVW        R1, #12769                  @ 0x31e1 
    /*00001c60*/ BL          mn34220_write_register
    /*00001c64*/ MOV         R0, R4 
    /*00001c68*/ MOV         R2, #0 
    /*00001c6c*/ MOVW        R1, #12770                  @ 0x31e2 
    /*00001c70*/ BL          mn34220_write_register
    /*00001c74*/ MOV         R0, R4 
    /*00001c78*/ MOV         R2, #0 
    /*00001c7c*/ MOVW        R1, #12771                  @ 0x31e3 
    /*00001c80*/ BL          mn34220_write_register
    /*00001c84*/ MOV         R0, R4 
    /*00001c88*/ MOV         R2, #0 
    /*00001c8c*/ MOVW        R1, #12772                  @ 0x31e4 
    /*00001c90*/ BL          mn34220_write_register
    /*00001c94*/ MOV         R0, R4 
    /*00001c98*/ MOV         R2, #0 
    /*00001c9c*/ MOVW        R1, #12773                  @ 0x31e5 
    /*00001ca0*/ BL          mn34220_write_register
    /*00001ca4*/ MOV         R0, R4 
    /*00001ca8*/ MOV         R2, #0 
    /*00001cac*/ MOVW        R1, #12774                  @ 0x31e6 
    /*00001cb0*/ BL          mn34220_write_register
    /*00001cb4*/ MOV         R0, R4 
    /*00001cb8*/ MOV         R2, #0 
    /*00001cbc*/ MOVW        R1, #12775                  @ 0x31e7 
    /*00001cc0*/ BL          mn34220_write_register
    /*00001cc4*/ MOV         R0, R4 
    /*00001cc8*/ MOV         R2, #0 
    /*00001ccc*/ MOVW        R1, #12776                  @ 0x31e8 
    /*00001cd0*/ BL          mn34220_write_register
    /*00001cd4*/ MOV         R0, R4 
    /*00001cd8*/ MOV         R2, #0 
    /*00001cdc*/ MOVW        R1, #12777                  @ 0x31e9 
    /*00001ce0*/ BL          mn34220_write_register
    /*00001ce4*/ MOV         R0, R4 
    /*00001ce8*/ MOV         R2, #0 
    /*00001cec*/ MOVW        R1, #12778                  @ 0x31ea 
    /*00001cf0*/ BL          mn34220_write_register
    /*00001cf4*/ MOV         R0, R4 
    /*00001cf8*/ MOV         R2, #0 
    /*00001cfc*/ MOVW        R1, #12779                  @ 0x31eb 
    /*00001d00*/ BL          mn34220_write_register
    /*00001d04*/ MOV         R0, R4 
    /*00001d08*/ MOV         R2, #0 
    /*00001d0c*/ MOVW        R1, #12780                  @ 0x31ec 
    /*00001d10*/ BL          mn34220_write_register
    /*00001d14*/ MOV         R0, R4 
    /*00001d18*/ MOV         R2, #0 
    /*00001d1c*/ MOVW        R1, #12781                  @ 0x31ed 
    /*00001d20*/ BL          mn34220_write_register
    /*00001d24*/ MOV         R0, R4 
    /*00001d28*/ MOV         R2, #0 
    /*00001d2c*/ MOVW        R1, #12782                  @ 0x31ee 
    /*00001d30*/ BL          mn34220_write_register
    /*00001d34*/ MOV         R0, R4 
    /*00001d38*/ MOV         R2, #0 
    /*00001d3c*/ MOVW        R1, #12783                  @ 0x31ef 
    /*00001d40*/ BL          mn34220_write_register
    /*00001d44*/ MOV         R0, R4 
    /*00001d48*/ MOV         R2, #0 
    /*00001d4c*/ MOVW        R1, #12784                  @ 0x31f0 
    /*00001d50*/ BL          mn34220_write_register
    /*00001d54*/ MOV         R0, R4 
    /*00001d58*/ MOV         R2, #0 
    /*00001d5c*/ MOVW        R1, #12785                  @ 0x31f1 
    /*00001d60*/ BL          mn34220_write_register
    /*00001d64*/ MOV         R0, R4 
    /*00001d68*/ MOV         R2, #0 
    /*00001d6c*/ MOVW        R1, #12786                  @ 0x31f2 
    /*00001d70*/ BL          mn34220_write_register
    /*00001d74*/ MOV         R0, R4 
    /*00001d78*/ MOV         R2, #0 
    /*00001d7c*/ MOVW        R1, #12787                  @ 0x31f3 
    /*00001d80*/ BL          mn34220_write_register
    /*00001d84*/ MOV         R0, R4 
    /*00001d88*/ MOV         R2, #0 
    /*00001d8c*/ MOVW        R1, #12788                  @ 0x31f4 
    /*00001d90*/ BL          mn34220_write_register
    /*00001d94*/ MOV         R0, R4 
    /*00001d98*/ MOV         R2, #0 
    /*00001d9c*/ MOVW        R1, #12789                  @ 0x31f5 
    /*00001da0*/ BL          mn34220_write_register
    /*00001da4*/ MOV         R0, R4 
    /*00001da8*/ MOV         R2, #0 
    /*00001dac*/ MOVW        R1, #12790                  @ 0x31f6 
    /*00001db0*/ BL          mn34220_write_register
    /*00001db4*/ MOV         R0, R4 
    /*00001db8*/ MOV         R2, #3 
    /*00001dbc*/ MOVW        R1, #12791                  @ 0x31f7 
    /*00001dc0*/ BL          mn34220_write_register
    /*00001dc4*/ MOV         R0, R4 
    /*00001dc8*/ MOV         R2, #3 
    /*00001dcc*/ MOVW        R1, #12792                  @ 0x31f8 
    /*00001dd0*/ BL          mn34220_write_register
    /*00001dd4*/ MOV         R0, R4 
    /*00001dd8*/ MOV         R2, #3 
    /*00001ddc*/ MOVW        R1, #12793                  @ 0x31f9 
    /*00001de0*/ BL          mn34220_write_register
    /*00001de4*/ MOV         R0, R4 
    /*00001de8*/ MOV         R2, #6 
    /*00001dec*/ MOVW        R1, #12794                  @ 0x31fa 
    /*00001df0*/ BL          mn34220_write_register
    /*00001df4*/ MOV         R0, R4 
    /*00001df8*/ MOV         R2, #8 
    /*00001dfc*/ MOVW        R1, #12795                  @ 0x31fb 
    /*00001e00*/ BL          mn34220_write_register
    /*00001e04*/ MOV         R0, R4 
    /*00001e08*/ MOV         R2, #2 
    /*00001e0c*/ MOVW        R1, #12796                  @ 0x31fc 
    /*00001e10*/ BL          mn34220_write_register
    /*00001e14*/ MOV         R0, R4 
    /*00001e18*/ MOV         R2, #8 
    /*00001e1c*/ MOVW        R1, #12797                  @ 0x31fd 
    /*00001e20*/ BL          mn34220_write_register
    /*00001e24*/ MOV         R0, R4 
    /*00001e28*/ MOV         R2, #7 
    /*00001e2c*/ MOVW        R1, #12798                  @ 0x31fe 
    /*00001e30*/ BL          mn34220_write_register
    /*00001e34*/ MOV         R0, R4 
    /*00001e38*/ MOV         R2, #0 
    /*00001e3c*/ MOVW        R1, #12799                  @ 0x31ff 
    /*00001e40*/ BL          mn34220_write_register
    /*00001e44*/ MOV         R0, R4 
    /*00001e48*/ MOV         R2, #0 
    /*00001e4c*/ MOV         R1, #12800                  @ 0x3200 
    /*00001e50*/ BL          mn34220_write_register
    /*00001e54*/ MOV         R0, R4 
    /*00001e58*/ MOV         R2, #0 
    /*00001e5c*/ MOVW        R1, #12801                  @ 0x3201 
    /*00001e60*/ BL          mn34220_write_register
    /*00001e64*/ MOV         R0, R4 
    /*00001e68*/ MOV         R2, #0 
    /*00001e6c*/ MOVW        R1, #12802                  @ 0x3202 
    /*00001e70*/ BL          mn34220_write_register
    /*00001e74*/ MOV         R0, R4 
    /*00001e78*/ MOV         R2, #0 
    /*00001e7c*/ MOVW        R1, #12803                  @ 0x3203 
    /*00001e80*/ BL          mn34220_write_register
    /*00001e84*/ MOV         R0, R4 
    /*00001e88*/ MOV         R2, #0 
    /*00001e8c*/ MOVW        R1, #12804                  @ 0x3204 
    /*00001e90*/ BL          mn34220_write_register
    /*00001e94*/ MOV         R0, R4 
    /*00001e98*/ MOV         R2, #0 
    /*00001e9c*/ MOVW        R1, #12805                  @ 0x3205 
    /*00001ea0*/ BL          mn34220_write_register
    /*00001ea4*/ MOV         R0, R4 
    /*00001ea8*/ MOV         R2, #0 
    /*00001eac*/ MOVW        R1, #12806                  @ 0x3206 
    /*00001eb0*/ BL          mn34220_write_register
    /*00001eb4*/ MOV         R0, R4 
    /*00001eb8*/ MOV         R2, #0 
    /*00001ebc*/ MOVW        R1, #12807                  @ 0x3207 
    /*00001ec0*/ BL          mn34220_write_register
    /*00001ec4*/ MOV         R0, R4 
    /*00001ec8*/ MOV         R2, #0 
    /*00001ecc*/ MOVW        R1, #12808                  @ 0x3208 
    /*00001ed0*/ BL          mn34220_write_register
    /*00001ed4*/ MOV         R0, R4 
    /*00001ed8*/ MOV         R2, #0 
    /*00001edc*/ MOVW        R1, #12809                  @ 0x3209 
    /*00001ee0*/ BL          mn34220_write_register
    /*00001ee4*/ MOV         R0, R4 
    /*00001ee8*/ MOV         R2, #0 
    /*00001eec*/ MOVW        R1, #12810                  @ 0x320a 
    /*00001ef0*/ BL          mn34220_write_register
    /*00001ef4*/ MOV         R0, R4 
    /*00001ef8*/ MOV         R2, #9 
    /*00001efc*/ MOVW        R1, #12811                  @ 0x320b 
    /*00001f00*/ BL          mn34220_write_register
    /*00001f04*/ MOV         R0, R4 
    /*00001f08*/ MOV         R2, #0 
    /*00001f0c*/ MOVW        R1, #12812                  @ 0x320c 
    /*00001f10*/ BL          mn34220_write_register
    /*00001f14*/ MOV         R0, R4 
    /*00001f18*/ MOV         R2, #26 
    /*00001f1c*/ MOVW        R1, #12813                  @ 0x320d 
    /*00001f20*/ BL          mn34220_write_register
    /*00001f24*/ MOV         R0, R4 
    /*00001f28*/ MOV         R2, #0 
    /*00001f2c*/ MOVW        R1, #12814                  @ 0x320e 
    /*00001f30*/ BL          mn34220_write_register
    /*00001f34*/ MOV         R0, R4 
    /*00001f38*/ MOV         R2, #27 
    /*00001f3c*/ MOVW        R1, #12815                  @ 0x320f 
    /*00001f40*/ BL          mn34220_write_register
    /*00001f44*/ MOV         R0, R4 
    /*00001f48*/ MOV         R2, #0 
    /*00001f4c*/ MOVW        R1, #12816                  @ 0x3210 
    /*00001f50*/ BL          mn34220_write_register
    /*00001f54*/ MOV         R0, R4 
    /*00001f58*/ MOV         R2, #27 
    /*00001f5c*/ MOVW        R1, #12817                  @ 0x3211 
    /*00001f60*/ BL          mn34220_write_register
    /*00001f64*/ MOV         R0, R4 
    /*00001f68*/ MOV         R2, #3 
    /*00001f6c*/ MOVW        R1, #12818                  @ 0x3212 
    /*00001f70*/ BL          mn34220_write_register
    /*00001f74*/ MOV         R0, R4 
    /*00001f78*/ MOV         R2, #11 
    /*00001f7c*/ MOVW        R1, #12819                  @ 0x3213 
    /*00001f80*/ BL          mn34220_write_register
    /*00001f84*/ MOV         R0, R4 
    /*00001f88*/ MOV         R2, #0 
    /*00001f8c*/ MOVW        R1, #12820                  @ 0x3214 
    /*00001f90*/ BL          mn34220_write_register
    /*00001f94*/ MOV         R0, R4 
    /*00001f98*/ MOV         R2, #0 
    /*00001f9c*/ MOVW        R1, #12821                  @ 0x3215 
    /*00001fa0*/ BL          mn34220_write_register
    /*00001fa4*/ MOV         R0, R4 
    /*00001fa8*/ MOV         R2, #0 
    /*00001fac*/ MOVW        R1, #12822                  @ 0x3216 
    /*00001fb0*/ BL          mn34220_write_register
    /*00001fb4*/ MOV         R0, R4 
    /*00001fb8*/ MOV         R2, #0 
    /*00001fbc*/ MOVW        R1, #12823                  @ 0x3217 
    /*00001fc0*/ BL          mn34220_write_register
    /*00001fc4*/ MOV         R0, R4 
    /*00001fc8*/ MOV         R2, #0 
    /*00001fcc*/ MOVW        R1, #12824                  @ 0x3218 
    /*00001fd0*/ BL          mn34220_write_register
    /*00001fd4*/ MOV         R0, R4 
    /*00001fd8*/ MOV         R2, #0 
    /*00001fdc*/ MOVW        R1, #12825                  @ 0x3219 
    /*00001fe0*/ BL          mn34220_write_register
    /*00001fe4*/ MOV         R0, R4 
    /*00001fe8*/ MOV         R2, #0 
    /*00001fec*/ MOVW        R1, #12826                  @ 0x321a 
    /*00001ff0*/ BL          mn34220_write_register
    /*00001ff4*/ MOV         R0, R4 
    /*00001ff8*/ MOV         R2, #0 
    /*00001ffc*/ MOVW        R1, #12827                  @ 0x321b 
    /*00002000*/ BL          mn34220_write_register
    /*00002004*/ MOV         R0, R4 
    /*00002008*/ MOV         R2, #0 
    /*0000200c*/ MOVW        R1, #12828                  @ 0x321c 
    /*00002010*/ BL          mn34220_write_register
    /*00002014*/ MOV         R0, R4 
    /*00002018*/ MOV         R2, #0 
    /*0000201c*/ MOVW        R1, #12829                  @ 0x321d 
    /*00002020*/ BL          mn34220_write_register
    /*00002024*/ MOV         R0, R4 
    /*00002028*/ MOV         R2, #0 
    /*0000202c*/ MOVW        R1, #12830                  @ 0x321e 
    /*00002030*/ BL          mn34220_write_register
    /*00002034*/ MOV         R0, R4 
    /*00002038*/ MOV         R2, #0 
    /*0000203c*/ MOVW        R1, #12831                  @ 0x321f 
    /*00002040*/ BL          mn34220_write_register
    /*00002044*/ MOV         R0, R4 
    /*00002048*/ MOV         R2, #0 
    /*0000204c*/ MOVW        R1, #12832                  @ 0x3220 
    /*00002050*/ BL          mn34220_write_register
    /*00002054*/ MOV         R0, R4 
    /*00002058*/ MOV         R2, #0 
    /*0000205c*/ MOVW        R1, #12833                  @ 0x3221 
    /*00002060*/ BL          mn34220_write_register
    /*00002064*/ MOV         R0, R4 
    /*00002068*/ MOV         R2, #255                    @ 0xff 
    /*0000206c*/ MOVW        R1, #12834                  @ 0x3222 
    /*00002070*/ BL          mn34220_write_register
    /*00002074*/ MOV         R0, R4 
    /*00002078*/ MOV         R2, #255                    @ 0xff 
    /*0000207c*/ MOVW        R1, #12835                  @ 0x3223 
    /*00002080*/ BL          mn34220_write_register
    /*00002084*/ MOV         R0, R4 
    /*00002088*/ MOV         R2, #255                    @ 0xff 
    /*0000208c*/ MOVW        R1, #12836                  @ 0x3224 
    /*00002090*/ BL          mn34220_write_register
    /*00002094*/ MOV         R0, R4 
    /*00002098*/ MOV         R2, #255                    @ 0xff 
    /*0000209c*/ MOVW        R1, #12837                  @ 0x3225 
    /*000020a0*/ BL          mn34220_write_register
    /*000020a4*/ MOV         R0, R4 
    /*000020a8*/ MOV         R2, #255                    @ 0xff 
    /*000020ac*/ MOVW        R1, #12838                  @ 0x3226 
    /*000020b0*/ BL          mn34220_write_register
    /*000020b4*/ MOV         R0, R4 
    /*000020b8*/ MOV         R2, #255                    @ 0xff 
    /*000020bc*/ MOVW        R1, #12839                  @ 0x3227 
    /*000020c0*/ BL          mn34220_write_register
    /*000020c4*/ MOV         R0, R4 
    /*000020c8*/ MOV         R2, #255                    @ 0xff 
    /*000020cc*/ MOVW        R1, #12840                  @ 0x3228 
    /*000020d0*/ BL          mn34220_write_register
    /*000020d4*/ MOV         R0, R4 
    /*000020d8*/ MOV         R2, #255                    @ 0xff 
    /*000020dc*/ MOVW        R1, #12841                  @ 0x3229 
    /*000020e0*/ BL          mn34220_write_register
    /*000020e4*/ MOV         R0, R4 
    /*000020e8*/ MOV         R2, #255                    @ 0xff 
    /*000020ec*/ MOVW        R1, #12842                  @ 0x322a 
    /*000020f0*/ BL          mn34220_write_register
    /*000020f4*/ MOV         R0, R4 
    /*000020f8*/ MOV         R2, #255                    @ 0xff 
    /*000020fc*/ MOVW        R1, #12843                  @ 0x322b 
    /*00002100*/ BL          mn34220_write_register
    /*00002104*/ MOV         R0, R4 
    /*00002108*/ MOV         R2, #255                    @ 0xff 
    /*0000210c*/ MOVW        R1, #12844                  @ 0x322c 
    /*00002110*/ BL          mn34220_write_register
    /*00002114*/ MOV         R0, R4 
    /*00002118*/ MOV         R2, #255                    @ 0xff 
    /*0000211c*/ MOVW        R1, #12845                  @ 0x322d 
    /*00002120*/ BL          mn34220_write_register
    /*00002124*/ MOV         R0, R4 
    /*00002128*/ MOV         R2, #255                    @ 0xff 
    /*0000212c*/ MOVW        R1, #12846                  @ 0x322e 
    /*00002130*/ BL          mn34220_write_register
    /*00002134*/ MOV         R0, R4 
    /*00002138*/ MOV         R2, #255                    @ 0xff 
    /*0000213c*/ MOVW        R1, #12847                  @ 0x322f 
    /*00002140*/ BL          mn34220_write_register
    /*00002144*/ MOV         R0, R4 
    /*00002148*/ MOV         R2, #255                    @ 0xff 
    /*0000214c*/ MOVW        R1, #12848                  @ 0x3230 
    /*00002150*/ BL          mn34220_write_register
    /*00002154*/ MOV         R0, R4 
    /*00002158*/ MOV         R2, #255                    @ 0xff 
    /*0000215c*/ MOVW        R1, #12849                  @ 0x3231 
    /*00002160*/ BL          mn34220_write_register
    /*00002164*/ MOV         R0, R4 
    /*00002168*/ MOV         R2, #0 
    /*0000216c*/ MOVW        R1, #12850                  @ 0x3232 
    /*00002170*/ BL          mn34220_write_register
    /*00002174*/ MOV         R0, R4 
    /*00002178*/ MOV         R2, #0 
    /*0000217c*/ MOVW        R1, #12851                  @ 0x3233 
    /*00002180*/ BL          mn34220_write_register
    /*00002184*/ MOV         R0, R4 
    /*00002188*/ MOV         R2, #1 
    /*0000218c*/ MOVW        R1, #12852                  @ 0x3234 
    /*00002190*/ BL          mn34220_write_register
    /*00002194*/ MOV         R0, R4 
    /*00002198*/ MOV         R2, #186                    @ 0xba 
    /*0000219c*/ MOVW        R1, #12853                  @ 0x3235 
    /*000021a0*/ BL          mn34220_write_register
    /*000021a4*/ MOV         R0, R4 
    /*000021a8*/ MOV         R2, #0 
    /*000021ac*/ MOVW        R1, #12854                  @ 0x3236 
    /*000021b0*/ BL          mn34220_write_register
    /*000021b4*/ MOV         R0, R4 
    /*000021b8*/ MOV         R2, #0 
    /*000021bc*/ MOVW        R1, #12855                  @ 0x3237 
    /*000021c0*/ BL          mn34220_write_register
    /*000021c4*/ MOV         R0, R4 
    /*000021c8*/ MOV         R2, #1 
    /*000021cc*/ MOVW        R1, #12856                  @ 0x3238 
    /*000021d0*/ BL          mn34220_write_register
    /*000021d4*/ MOV         R0, R4 
    /*000021d8*/ MOV         R2, #186                    @ 0xba 
    /*000021dc*/ MOVW        R1, #12857                  @ 0x3239 
    /*000021e0*/ BL          mn34220_write_register
    /*000021e4*/ MOV         R0, R4 
    /*000021e8*/ MOV         R2, #1 
    /*000021ec*/ MOVW        R1, #12858                  @ 0x323a 
    /*000021f0*/ BL          mn34220_write_register
    /*000021f4*/ MOV         R0, R4 
    /*000021f8*/ MOV         R2, #129                    @ 0x81 
    /*000021fc*/ MOVW        R1, #12859                  @ 0x323b 
    /*00002200*/ BL          mn34220_write_register
    /*00002204*/ MOV         R0, R4 
    /*00002208*/ MOV         R2, #112                    @ 0x70 
    /*0000220c*/ MOVW        R1, #12860                  @ 0x323c 
    /*00002210*/ BL          mn34220_write_register
    /*00002214*/ MOV         R0, R4 
    /*00002218*/ MOV         R2, #128                    @ 0x80 
    /*0000221c*/ MOVW        R1, #12861                  @ 0x323d 
    /*00002220*/ BL          mn34220_write_register
    /*00002224*/ MOV         R0, R4 
    /*00002228*/ MOV         R2, #0 
    /*0000222c*/ MOVW        R1, #12862                  @ 0x323e 
    /*00002230*/ BL          mn34220_write_register
    /*00002234*/ MOV         R0, R4 
    /*00002238*/ MOV         R2, #0 
    /*0000223c*/ MOVW        R1, #12863                  @ 0x323f 
    /*00002240*/ BL          mn34220_write_register
    /*00002244*/ MOV         R0, R4 
    /*00002248*/ MOV         R2, #21 
    /*0000224c*/ MOV         R1, #12864                  @ 0x3240 
    /*00002250*/ BL          mn34220_write_register
    /*00002254*/ MOV         R0, R4 
    /*00002258*/ MOV         R2, #17 
    /*0000225c*/ MOVW        R1, #12865                  @ 0x3241 
    /*00002260*/ BL          mn34220_write_register
    /*00002264*/ MOV         R0, R4 
    /*00002268*/ MOV         R2, #0 
    /*0000226c*/ MOVW        R1, #12866                  @ 0x3242 
    /*00002270*/ BL          mn34220_write_register
    /*00002274*/ MOV         R0, R4 
    /*00002278*/ MOV         R2, #209                    @ 0xd1 
    /*0000227c*/ MOVW        R1, #12867                  @ 0x3243 
    /*00002280*/ BL          mn34220_write_register
    /*00002284*/ MOV         R0, R4 
    /*00002288*/ MOV         R2, #0 
    /*0000228c*/ MOVW        R1, #12868                  @ 0x3244 
    /*00002290*/ BL          mn34220_write_register
    /*00002294*/ MOV         R0, R4 
    /*00002298*/ MOV         R2, #24 
    /*0000229c*/ MOVW        R1, #12869                  @ 0x3245 
    /*000022a0*/ BL          mn34220_write_register
    /*000022a4*/ MOV         R0, R4 
    /*000022a8*/ MOV         R2, #1 
    /*000022ac*/ MOVW        R1, #12870                  @ 0x3246 
    /*000022b0*/ BL          mn34220_write_register
    /*000022b4*/ MOV         R0, R4 
    /*000022b8*/ MOV         R2, #160                    @ 0xa0 
    /*000022bc*/ MOVW        R1, #12871                  @ 0x3247 
    /*000022c0*/ BL          mn34220_write_register
    /*000022c4*/ MOV         R0, R4 
    /*000022c8*/ MOV         R2, #0 
    /*000022cc*/ MOVW        R1, #12872                  @ 0x3248 
    /*000022d0*/ BL          mn34220_write_register
    /*000022d4*/ MOV         R0, R4 
    /*000022d8*/ MOV         R2, #0 
    /*000022dc*/ MOVW        R1, #12873                  @ 0x3249 
    /*000022e0*/ BL          mn34220_write_register
    /*000022e4*/ MOV         R0, R4 
    /*000022e8*/ MOV         R2, #48                     @ 0x30 
    /*000022ec*/ MOVW        R1, #12874                  @ 0x324a 
    /*000022f0*/ BL          mn34220_write_register
    /*000022f4*/ MOV         R0, R4 
    /*000022f8*/ MOV         R2, #52                     @ 0x34 
    /*000022fc*/ MOVW        R1, #12875                  @ 0x324b 
    /*00002300*/ BL          mn34220_write_register
    /*00002304*/ MOV         R0, R4 
    /*00002308*/ MOV         R2, #2 
    /*0000230c*/ MOVW        R1, #12876                  @ 0x324c 
    /*00002310*/ BL          mn34220_write_register
    /*00002314*/ MOV         R0, R4 
    /*00002318*/ MOV         R2, #0 
    /*0000231c*/ MOVW        R1, #12877                  @ 0x324d 
    /*00002320*/ BL          mn34220_write_register
    /*00002324*/ MOV         R0, R4 
    /*00002328*/ MOV         R2, #0 
    /*0000232c*/ MOVW        R1, #12878                  @ 0x324e 
    /*00002330*/ BL          mn34220_write_register
    /*00002334*/ MOV         R0, R4 
    /*00002338*/ MOV         R2, #24 
    /*0000233c*/ MOVW        R1, #12879                  @ 0x324f 
    /*00002340*/ BL          mn34220_write_register
    /*00002344*/ MOV         R0, R4 
    /*00002348*/ MOV         R2, #49                     @ 0x31 
    /*0000234c*/ MOVW        R1, #12880                  @ 0x3250 
    /*00002350*/ BL          mn34220_write_register
    /*00002354*/ MOV         R0, R4 
    /*00002358*/ MOV         R2, #14 
    /*0000235c*/ MOVW        R1, #12881                  @ 0x3251 
    /*00002360*/ BL          mn34220_write_register
    /*00002364*/ MOV         R0, R4 
    /*00002368*/ MOV         R2, #0 
    /*0000236c*/ MOVW        R1, #12882                  @ 0x3252 
    /*00002370*/ BL          mn34220_write_register
    /*00002374*/ MOV         R0, R4 
    /*00002378*/ MOV         R2, #212                    @ 0xd4 
    /*0000237c*/ MOVW        R1, #12883                  @ 0x3253 
    /*00002380*/ BL          mn34220_write_register
    /*00002384*/ MOV         R0, R4 
    /*00002388*/ MOV         R2, #3 
    /*0000238c*/ MOVW        R1, #12884                  @ 0x3254 
    /*00002390*/ BL          mn34220_write_register
    /*00002394*/ MOV         R0, R4 
    /*00002398*/ MOV         R2, #8 
    /*0000239c*/ MOVW        R1, #12885                  @ 0x3255 
    /*000023a0*/ BL          mn34220_write_register
    /*000023a4*/ MOV         R0, R4 
    /*000023a8*/ MOV         R2, #17 
    /*000023ac*/ MOVW        R1, #12886                  @ 0x3256 
    /*000023b0*/ BL          mn34220_write_register
    /*000023b4*/ MOV         R0, R4 
    /*000023b8*/ MOV         R2, #0 
    /*000023bc*/ MOVW        R1, #12887                  @ 0x3257 
    /*000023c0*/ BL          mn34220_write_register
    /*000023c4*/ MOV         R0, R4 
    /*000023c8*/ MOV         R2, #1 
    /*000023cc*/ MOVW        R1, #12888                  @ 0x3258 
    /*000023d0*/ BL          mn34220_write_register
    /*000023d4*/ MOV         R0, R4 
    /*000023d8*/ MOV         R2, #144                    @ 0x90 
    /*000023dc*/ MOVW        R1, #12889                  @ 0x3259 
    /*000023e0*/ BL          mn34220_write_register
    /*000023e4*/ MOV         R0, R4 
    /*000023e8*/ MOV         R2, #57                     @ 0x39 
    /*000023ec*/ MOVW        R1, #12890                  @ 0x325a 
    /*000023f0*/ BL          mn34220_write_register
    /*000023f4*/ MOV         R0, R4 
    /*000023f8*/ MOV         R2, #0 
    /*000023fc*/ MOVW        R1, #12891                  @ 0x325b 
    /*00002400*/ BL          mn34220_write_register
    /*00002404*/ MOV         R0, R4 
    /*00002408*/ MOV         R2, #0 
    /*0000240c*/ MOVW        R1, #12892                  @ 0x325c 
    /*00002410*/ BL          mn34220_write_register
    /*00002414*/ MOV         R0, R4 
    /*00002418*/ MOV         R2, #12 
    /*0000241c*/ MOVW        R1, #12893                  @ 0x325d 
    /*00002420*/ BL          mn34220_write_register
    /*00002424*/ MOV         R0, R4 
    /*00002428*/ MOV         R2, #132                    @ 0x84 
    /*0000242c*/ MOVW        R1, #12894                  @ 0x325e 
    /*00002430*/ BL          mn34220_write_register
    /*00002434*/ MOV         R0, R4 
    /*00002438*/ MOV         R2, #0 
    /*0000243c*/ MOVW        R1, #12895                  @ 0x325f 
    /*00002440*/ BL          mn34220_write_register
    /*00002444*/ MOV         R0, R4 
    /*00002448*/ MOV         R2, #0 
    /*0000244c*/ MOVW        R1, #12896                  @ 0x3260 
    /*00002450*/ BL          mn34220_write_register
    /*00002454*/ MOV         R0, R4 
    /*00002458*/ MOV         R2, #0 
    /*0000245c*/ MOVW        R1, #12897                  @ 0x3261 
    /*00002460*/ BL          mn34220_write_register
    /*00002464*/ MOV         R0, R4 
    /*00002468*/ MOV         R2, #1 
    /*0000246c*/ MOVW        R1, #12912                  @ 0x3270 
    /*00002470*/ BL          mn34220_write_register
    /*00002474*/ MOV         R0, R4 
    /*00002478*/ MOV         R2, #0 
    /*0000247c*/ MOVW        R1, #12913                  @ 0x3271 
    /*00002480*/ BL          mn34220_write_register
    /*00002484*/ MOV         R0, R4 
    /*00002488*/ MOV         R2, #85                     @ 0x55 
    /*0000248c*/ MOVW        R1, #12914                  @ 0x3272 
    /*00002490*/ BL          mn34220_write_register
    /*00002494*/ MOV         R0, R4 
    /*00002498*/ MOV         R2, #171                    @ 0xab 
    /*0000249c*/ MOVW        R1, #12915                  @ 0x3273 
    /*000024a0*/ BL          mn34220_write_register
    /*000024a4*/ MOV         R0, R4 
    /*000024a8*/ MOV         R2, #1 
    /*000024ac*/ MOVW        R1, #12916                  @ 0x3274 
    /*000024b0*/ BL          mn34220_write_register
    /*000024b4*/ MOV         R0, R4 
    /*000024b8*/ MOV         R2, #24 
    /*000024bc*/ MOVW        R1, #12917                  @ 0x3275 
    /*000024c0*/ BL          mn34220_write_register
    /*000024c4*/ MOV         R0, R4 
    /*000024c8*/ MOV         R2, #48                     @ 0x30 
    /*000024cc*/ MOV         R1, #12928                  @ 0x3280 
    /*000024d0*/ BL          mn34220_write_register
    /*000024d4*/ MOV         R0, R4 
    /*000024d8*/ MOV         R2, #16 
    /*000024dc*/ MOVW        R1, #12929                  @ 0x3281 
    /*000024e0*/ BL          mn34220_write_register
    /*000024e4*/ MOV         R0, R4 
    /*000024e8*/ MOV         R2, #14 
    /*000024ec*/ MOVW        R1, #12930                  @ 0x3282 
    /*000024f0*/ BL          mn34220_write_register
    /*000024f4*/ MOV         R0, R4 
    /*000024f8*/ MOV         R2, #0 
    /*000024fc*/ MOVW        R1, #12931                  @ 0x3283 
    /*00002500*/ BL          mn34220_write_register
    /*00002504*/ MOV         R0, R4 
    /*00002508*/ MOV         R2, #0 
    /*0000250c*/ MOVW        R1, #12932                  @ 0x3284 
    /*00002510*/ BL          mn34220_write_register
    /*00002514*/ MOV         R0, R4 
    /*00002518*/ MOV         R2, #27 
    /*0000251c*/ MOVW        R1, #12933                  @ 0x3285 
    /*00002520*/ BL          mn34220_write_register
    /*00002524*/ MOV         R0, R4 
    /*00002528*/ MOV         R2, #24 
    /*0000252c*/ MOVW        R1, #12934                  @ 0x3286 
    /*00002530*/ BL          mn34220_write_register
    /*00002534*/ MOV         R0, R4 
    /*00002538*/ MOV         R2, #0 
    /*0000253c*/ MOVW        R1, #12935                  @ 0x3287 
    /*00002540*/ BL          mn34220_write_register
    /*00002544*/ MOV         R0, R4 
    /*00002548*/ MOV         R2, #1 
    /*0000254c*/ MOVW        R1, #12936                  @ 0x3288 
    /*00002550*/ BL          mn34220_write_register
    /*00002554*/ MOV         R0, R4 
    /*00002558*/ MOV         R2, #0 
    /*0000255c*/ MOVW        R1, #12937                  @ 0x3289 
    /*00002560*/ BL          mn34220_write_register
    /*00002564*/ MOV         R0, R4 
    /*00002568*/ MOV         R2, #232                    @ 0xe8 
    /*0000256c*/ MOVW        R1, #12938                  @ 0x328a 
    /*00002570*/ BL          mn34220_write_register
    /*00002574*/ MOV         R0, R4 
    /*00002578*/ MOV         R2, #4 
    /*0000257c*/ MOVW        R1, #12939                  @ 0x328b 
    /*00002580*/ BL          mn34220_write_register
    /*00002584*/ MOV         R0, R4 
    /*00002588*/ MOV         R2, #117                    @ 0x75 
    /*0000258c*/ MOVW        R1, #12940                  @ 0x328c 
    /*00002590*/ BL          mn34220_write_register
    /*00002594*/ MOV         R0, R4 
    /*00002598*/ MOV         R2, #0 
    /*0000259c*/ MOVW        R1, #12941                  @ 0x328d 
    /*000025a0*/ BL          mn34220_write_register
    /*000025a4*/ MOV         R0, R4 
    /*000025a8*/ MOV         R2, #3 
    /*000025ac*/ MOV         R1, #13056                  @ 0x3300 
    /*000025b0*/ BL          mn34220_write_register
    /*000025b4*/ MOV         R0, R4 
    /*000025b8*/ MOV         R2, #1 
    /*000025bc*/ MOVW        R1, #13057                  @ 0x3301 
    /*000025c0*/ BL          mn34220_write_register
    /*000025c4*/ MOV         R0, R4 
    /*000025c8*/ MOV         R2, #0 
    /*000025cc*/ MOVW        R1, #13058                  @ 0x3302 
    /*000025d0*/ BL          mn34220_write_register
    /*000025d4*/ MOV         R0, R4 
    /*000025d8*/ MOV         R2, #7 
    /*000025dc*/ MOVW        R1, #13059                  @ 0x3303 
    /*000025e0*/ BL          mn34220_write_register
    /*000025e4*/ MOV         R0, R4 
    /*000025e8*/ MOV         R2, #6 
    /*000025ec*/ MOVW        R1, #13060                  @ 0x3304 
    /*000025f0*/ BL          mn34220_write_register
    /*000025f4*/ MOV         R0, R4 
    /*000025f8*/ MOV         R2, #10 
    /*000025fc*/ MOVW        R1, #13061                  @ 0x3305 
    /*00002600*/ BL          mn34220_write_register
    /*00002604*/ MOV         R0, R4 
    /*00002608*/ MOV         R2, #25 
    /*0000260c*/ MOVW        R1, #13062                  @ 0x3306 
    /*00002610*/ BL          mn34220_write_register
    /*00002614*/ MOV         R0, R4 
    /*00002618*/ MOV         R2, #25 
    /*0000261c*/ MOVW        R1, #13063                  @ 0x3307 
    /*00002620*/ BL          mn34220_write_register
    /*00002624*/ MOV         R0, R4 
    /*00002628*/ MOV         R2, #0 
    /*0000262c*/ MOVW        R1, #13064                  @ 0x3308 
    /*00002630*/ BL          mn34220_write_register
    /*00002634*/ MOV         R0, R4 
    /*00002638*/ MOV         R2, #0 
    /*0000263c*/ MOVW        R1, #13065                  @ 0x3309 
    /*00002640*/ BL          mn34220_write_register
    /*00002644*/ MOV         R0, R4 
    /*00002648*/ MOV         R2, #30 
    /*0000264c*/ MOVW        R1, #13066                  @ 0x330a 
    /*00002650*/ BL          mn34220_write_register
    /*00002654*/ MOV         R0, R4 
    /*00002658*/ MOV         R2, #30 
    /*0000265c*/ MOVW        R1, #13067                  @ 0x330b 
    /*00002660*/ BL          mn34220_write_register
    /*00002664*/ MOV         R0, R4 
    /*00002668*/ MOV         R2, #0 
    /*0000266c*/ MOVW        R1, #13068                  @ 0x330c 
    /*00002670*/ BL          mn34220_write_register
    /*00002674*/ MOV         R0, R4 
    /*00002678*/ MOV         R2, #0 
    /*0000267c*/ MOVW        R1, #13069                  @ 0x330d 
    /*00002680*/ BL          mn34220_write_register
    /*00002684*/ MOV         R0, R4 
    /*00002688*/ MOV         R2, #3 
    /*0000268c*/ MOVW        R1, #13070                  @ 0x330e 
    /*00002690*/ BL          mn34220_write_register
    /*00002694*/ MOV         R0, R4 
    /*00002698*/ MOV         R2, #17 
    /*0000269c*/ MOVW        R1, #13071                  @ 0x330f 
    /*000026a0*/ BL          mn34220_write_register
    /*000026a4*/ MOV         R0, R4 
    /*000026a8*/ MOV         R2, #2 
    /*000026ac*/ MOVW        R1, #13072                  @ 0x3310 
    /*000026b0*/ BL          mn34220_write_register
    /*000026b4*/ MOV         R0, R4 
    /*000026b8*/ MOV         R2, #0 
    /*000026bc*/ MOVW        R1, #13073                  @ 0x3311 
    /*000026c0*/ BL          mn34220_write_register
    /*000026c4*/ MOV         R0, R4 
    /*000026c8*/ MOV         R2, #0 
    /*000026cc*/ MOVW        R1, #13074                  @ 0x3312 
    /*000026d0*/ BL          mn34220_write_register
    /*000026d4*/ MOV         R0, R4 
    /*000026d8*/ MOV         R2, #0 
    /*000026dc*/ MOVW        R1, #13075                  @ 0x3313 
    /*000026e0*/ BL          mn34220_write_register
    /*000026e4*/ MOV         R0, R4 
    /*000026e8*/ MOV         R2, #3 
    /*000026ec*/ MOVW        R1, #13076                  @ 0x3314 
    /*000026f0*/ BL          mn34220_write_register
    /*000026f4*/ MOV         R0, R4 
    /*000026f8*/ MOV         R2, #25 
    /*000026fc*/ MOVW        R1, #13077                  @ 0x3315 
    /*00002700*/ BL          mn34220_write_register
    /*00002704*/ MOV         R0, R4 
    /*00002708*/ MOV         R2, #10 
    /*0000270c*/ MOVW        R1, #13078                  @ 0x3316 
    /*00002710*/ BL          mn34220_write_register
    /*00002714*/ MOV         R0, R4 
    /*00002718*/ MOV         R2, #0 
    /*0000271c*/ MOVW        R1, #13079                  @ 0x3317 
    /*00002720*/ BL          mn34220_write_register
    /*00002724*/ MOV         R0, R4 
    /*00002728*/ MOV         R2, #6 
    /*0000272c*/ MOVW        R1, #13080                  @ 0x3318 
    /*00002730*/ BL          mn34220_write_register
    /*00002734*/ MOV         R0, R4 
    /*00002738*/ MOV         R2, #0 
    /*0000273c*/ MOVW        R1, #13081                  @ 0x3319 
    /*00002740*/ BL          mn34220_write_register
    /*00002744*/ MOV         R0, R4 
    /*00002748*/ MOV         R2, #2 
    /*0000274c*/ MOVW        R1, #13082                  @ 0x331a 
    /*00002750*/ BL          mn34220_write_register
    /*00002754*/ MOV         R0, R4 
    /*00002758*/ MOV         R2, #2 
    /*0000275c*/ MOVW        R1, #13083                  @ 0x331b 
    /*00002760*/ BL          mn34220_write_register
    /*00002764*/ MOV         R0, R4 
    /*00002768*/ MOV         R2, #0 
    /*0000276c*/ MOVW        R1, #13084                  @ 0x331c 
    /*00002770*/ BL          mn34220_write_register
    /*00002774*/ MOV         R0, R4 
    /*00002778*/ MOV         R2, #0 
    /*0000277c*/ MOVW        R1, #13085                  @ 0x331d 
    /*00002780*/ BL          mn34220_write_register
    /*00002784*/ MOV         R0, R4 
    /*00002788*/ MOV         R2, #3 
    /*0000278c*/ MOVW        R1, #13086                  @ 0x331e 
    /*00002790*/ BL          mn34220_write_register
    /*00002794*/ MOV         R0, R4 
    /*00002798*/ MOV         R2, #3 
    /*0000279c*/ MOVW        R1, #13087                  @ 0x331f 
    /*000027a0*/ BL          mn34220_write_register
    /*000027a4*/ MOV         R0, R4 
    /*000027a8*/ MOV         R2, #1 
    /*000027ac*/ MOVW        R1, #13088                  @ 0x3320 
    /*000027b0*/ BL          mn34220_write_register
    /*000027b4*/ MOV         R0, R4 
    /*000027b8*/ MOV         R2, #1 
    /*000027bc*/ MOVW        R1, #13089                  @ 0x3321 
    /*000027c0*/ BL          mn34220_write_register
    /*000027c4*/ MOV         R0, R4 
    /*000027c8*/ MOV         R2, #0 
    /*000027cc*/ MOVW        R1, #13090                  @ 0x3322 
    /*000027d0*/ BL          mn34220_write_register
    /*000027d4*/ MOV         R0, R4 
    /*000027d8*/ MOV         R2, #0 
    /*000027dc*/ MOVW        R1, #13091                  @ 0x3323 
    /*000027e0*/ BL          mn34220_write_register
    /*000027e4*/ MOV         R0, R4 
    /*000027e8*/ MOV         R2, #0 
    /*000027ec*/ MOVW        R1, #13092                  @ 0x3324 
    /*000027f0*/ BL          mn34220_write_register
    /*000027f4*/ MOV         R0, R4 
    /*000027f8*/ MOV         R2, #0 
    /*000027fc*/ MOVW        R1, #13093                  @ 0x3325 
    /*00002800*/ BL          mn34220_write_register
    /*00002804*/ MOV         R0, R4 
    /*00002808*/ MOV         R2, #10 
    /*0000280c*/ MOVW        R1, #13094                  @ 0x3326 
    /*00002810*/ BL          mn34220_write_register
    /*00002814*/ MOV         R0, R4 
    /*00002818*/ MOV         R2, #0 
    /*0000281c*/ MOVW        R1, #13095                  @ 0x3327 
    /*00002820*/ BL          mn34220_write_register
    /*00002824*/ MOV         R0, R4 
    /*00002828*/ MOV         R2, #0 
    /*0000282c*/ MOVW        R1, #13096                  @ 0x3328 
    /*00002830*/ BL          mn34220_write_register
    /*00002834*/ MOV         R0, R4 
    /*00002838*/ MOV         R2, #0 
    /*0000283c*/ MOVW        R1, #13097                  @ 0x3329 
    /*00002840*/ BL          mn34220_write_register
    /*00002844*/ MOV         R0, R4 
    /*00002848*/ MOV         R2, #3 
    /*0000284c*/ MOVW        R1, #13098                  @ 0x332a 
    /*00002850*/ BL          mn34220_write_register
    /*00002854*/ MOV         R0, R4 
    /*00002858*/ MOV         R2, #3 
    /*0000285c*/ MOVW        R1, #13099                  @ 0x332b 
    /*00002860*/ BL          mn34220_write_register
    /*00002864*/ MOV         R0, R4 
    /*00002868*/ MOV         R2, #2 
    /*0000286c*/ MOVW        R1, #13100                  @ 0x332c 
    /*00002870*/ BL          mn34220_write_register
    /*00002874*/ MOV         R0, R4 
    /*00002878*/ MOV         R2, #0 
    /*0000287c*/ MOVW        R1, #13101                  @ 0x332d 
    /*00002880*/ BL          mn34220_write_register
    /*00002884*/ MOV         R0, R4 
    /*00002888*/ MOV         R2, #10 
    /*0000288c*/ MOVW        R1, #13102                  @ 0x332e 
    /*00002890*/ BL          mn34220_write_register
    /*00002894*/ MOV         R0, R4 
    /*00002898*/ MOV         R2, #0 
    /*0000289c*/ MOVW        R1, #13103                  @ 0x332f 
    /*000028a0*/ BL          mn34220_write_register
    /*000028a4*/ MOV         R0, R4 
    /*000028a8*/ MOV         R2, #6 
    /*000028ac*/ MOVW        R1, #13104                  @ 0x3330 
    /*000028b0*/ BL          mn34220_write_register
    /*000028b4*/ MOV         R0, R4 
    /*000028b8*/ MOV         R2, #0 
    /*000028bc*/ MOVW        R1, #13105                  @ 0x3331 
    /*000028c0*/ BL          mn34220_write_register
    /*000028c4*/ MOV         R0, R4 
    /*000028c8*/ MOV         R2, #18 
    /*000028cc*/ MOVW        R1, #13106                  @ 0x3332 
    /*000028d0*/ BL          mn34220_write_register
    /*000028d4*/ MOV         R0, R4 
    /*000028d8*/ MOV         R2, #0 
    /*000028dc*/ MOVW        R1, #13107                  @ 0x3333 
    /*000028e0*/ BL          mn34220_write_register
    /*000028e4*/ MOV         R0, R4 
    /*000028e8*/ MOV         R2, #3 
    /*000028ec*/ MOVW        R1, #13108                  @ 0x3334 
    /*000028f0*/ BL          mn34220_write_register
    /*000028f4*/ MOV         R0, R4 
    /*000028f8*/ MOV         R2, #0 
    /*000028fc*/ MOVW        R1, #13109                  @ 0x3335 
    /*00002900*/ BL          mn34220_write_register
    /*00002904*/ MOV         R0, R4 
    /*00002908*/ MOV         R2, #3 
    /*0000290c*/ MOVW        R1, #13110                  @ 0x3336 
    /*00002910*/ BL          mn34220_write_register
    /*00002914*/ MOV         R0, R4 
    /*00002918*/ MOV         R2, #3 
    /*0000291c*/ MOVW        R1, #13111                  @ 0x3337 
    /*00002920*/ BL          mn34220_write_register
    /*00002924*/ MOV         R0, R4 
    /*00002928*/ MOV         R2, #0 
    /*0000292c*/ MOVW        R1, #13112                  @ 0x3338 
    /*00002930*/ BL          mn34220_write_register
    /*00002934*/ MOV         R0, R4 
    /*00002938*/ MOV         R2, #2 
    /*0000293c*/ MOVW        R1, #13113                  @ 0x3339 
    /*00002940*/ BL          mn34220_write_register
    /*00002944*/ MOV         R0, R4 
    /*00002948*/ MOV         R2, #8 
    /*0000294c*/ MOVW        R1, #13114                  @ 0x333a 
    /*00002950*/ BL          mn34220_write_register
    /*00002954*/ MOV         R0, R4 
    /*00002958*/ MOV         R2, #0 
    /*0000295c*/ MOVW        R1, #13115                  @ 0x333b 
    /*00002960*/ BL          mn34220_write_register
    /*00002964*/ MOV         R0, R4 
    /*00002968*/ MOV         R2, #0 
    /*0000296c*/ MOVW        R1, #13116                  @ 0x333c 
    /*00002970*/ BL          mn34220_write_register
    /*00002974*/ MOV         R0, R4 
    /*00002978*/ MOV         R2, #0 
    /*0000297c*/ MOVW        R1, #13117                  @ 0x333d 
    /*00002980*/ BL          mn34220_write_register
    /*00002984*/ MOV         R0, R4 
    /*00002988*/ MOV         R2, #0 
    /*0000298c*/ MOVW        R1, #13118                  @ 0x333e 
    /*00002990*/ BL          mn34220_write_register
    /*00002994*/ MOV         R0, R4 
    /*00002998*/ MOV         R2, #0 
    /*0000299c*/ MOVW        R1, #13119                  @ 0x333f 
    /*000029a0*/ BL          mn34220_write_register
    /*000029a4*/ MOV         R0, R4 
    /*000029a8*/ MOV         R2, #0 
    /*000029ac*/ MOV         R1, #13120                  @ 0x3340 
    /*000029b0*/ BL          mn34220_write_register
    /*000029b4*/ MOV         R0, R4 
    /*000029b8*/ MOV         R2, #0 
    /*000029bc*/ MOVW        R1, #13121                  @ 0x3341 
    /*000029c0*/ BL          mn34220_write_register
    /*000029c4*/ MOV         R0, R4 
    /*000029c8*/ MOV         R2, #0 
    /*000029cc*/ MOVW        R1, #13122                  @ 0x3342 
    /*000029d0*/ BL          mn34220_write_register
    /*000029d4*/ MOV         R0, R4 
    /*000029d8*/ MOV         R2, #0 
    /*000029dc*/ MOVW        R1, #13123                  @ 0x3343 
    /*000029e0*/ BL          mn34220_write_register
    /*000029e4*/ MOV         R0, R4 
    /*000029e8*/ MOV         R2, #0 
    /*000029ec*/ MOVW        R1, #13124                  @ 0x3344 
    /*000029f0*/ BL          mn34220_write_register
    /*000029f4*/ MOV         R0, R4 
    /*000029f8*/ MOV         R2, #0 
    /*000029fc*/ MOVW        R1, #13125                  @ 0x3345 
    /*00002a00*/ BL          mn34220_write_register
    /*00002a04*/ MOV         R0, R4 
    /*00002a08*/ MOV         R2, #0 
    /*00002a0c*/ MOVW        R1, #13126                  @ 0x3346 
    /*00002a10*/ BL          mn34220_write_register
    /*00002a14*/ MOV         R0, R4 
    /*00002a18*/ MOV         R2, #0 
    /*00002a1c*/ MOVW        R1, #13127                  @ 0x3347 
    /*00002a20*/ BL          mn34220_write_register
    /*00002a24*/ MOV         R0, R4 
    /*00002a28*/ MOV         R2, #7 
    /*00002a2c*/ MOVW        R1, #13128                  @ 0x3348 
    /*00002a30*/ BL          mn34220_write_register
    /*00002a34*/ MOV         R0, R4 
    /*00002a38*/ MOV         R2, #0 
    /*00002a3c*/ MOVW        R1, #13129                  @ 0x3349 
    /*00002a40*/ BL          mn34220_write_register
    /*00002a44*/ MOV         R0, R4 
    /*00002a48*/ MOV         R2, #5 
    /*00002a4c*/ MOVW        R1, #13130                  @ 0x334a 
    /*00002a50*/ BL          mn34220_write_register
    /*00002a54*/ MOV         R0, R4 
    /*00002a58*/ MOV         R2, #0 
    /*00002a5c*/ MOVW        R1, #13131                  @ 0x334b 
    /*00002a60*/ BL          mn34220_write_register
    /*00002a64*/ MOV         R0, R4 
    /*00002a68*/ MOV         R2, #5 
    /*00002a6c*/ MOVW        R1, #13132                  @ 0x334c 
    /*00002a70*/ BL          mn34220_write_register
    /*00002a74*/ MOV         R0, R4 
    /*00002a78*/ MOV         R2, #28 
    /*00002a7c*/ MOVW        R1, #13133                  @ 0x334d 
    /*00002a80*/ BL          mn34220_write_register
    /*00002a84*/ MOV         R0, R4 
    /*00002a88*/ MOV         R2, #0 
    /*00002a8c*/ MOVW        R1, #13134                  @ 0x334e 
    /*00002a90*/ BL          mn34220_write_register
    /*00002a94*/ MOV         R0, R4 
    /*00002a98*/ MOV         R2, #3 
    /*00002a9c*/ MOVW        R1, #13135                  @ 0x334f 
    /*00002aa0*/ BL          mn34220_write_register
    /*00002aa4*/ MOV         R0, R4 
    /*00002aa8*/ MOV         R2, #14 
    /*00002aac*/ MOVW        R1, #13136                  @ 0x3350 
    /*00002ab0*/ BL          mn34220_write_register
    /*00002ab4*/ MOV         R0, R4 
    /*00002ab8*/ MOV         R2, #8 
    /*00002abc*/ MOVW        R1, #13137                  @ 0x3351 
    /*00002ac0*/ BL          mn34220_write_register
    /*00002ac4*/ MOV         R0, R4 
    /*00002ac8*/ MOV         R2, #6 
    /*00002acc*/ MOVW        R1, #13138                  @ 0x3352 
    /*00002ad0*/ BL          mn34220_write_register
    /*00002ad4*/ MOV         R0, R4 
    /*00002ad8*/ MOV         R2, #5 
    /*00002adc*/ MOVW        R1, #13139                  @ 0x3353 
    /*00002ae0*/ BL          mn34220_write_register
    /*00002ae4*/ MOV         R0, R4 
    /*00002ae8*/ MOV         R2, #8 
    /*00002aec*/ MOVW        R1, #13140                  @ 0x3354 
    /*00002af0*/ BL          mn34220_write_register
    /*00002af4*/ MOV         R0, R4 
    /*00002af8*/ MOV         R2, #25 
    /*00002afc*/ MOVW        R1, #13141                  @ 0x3355 
    /*00002b00*/ BL          mn34220_write_register
    /*00002b04*/ MOV         R0, R4 
    /*00002b08*/ MOV         R2, #0 
    /*00002b0c*/ MOVW        R1, #13142                  @ 0x3356 
    /*00002b10*/ BL          mn34220_write_register
    /*00002b14*/ MOV         R0, R4 
    /*00002b18*/ MOV         R2, #3 
    /*00002b1c*/ MOVW        R1, #13143                  @ 0x3357 
    /*00002b20*/ BL          mn34220_write_register
    /*00002b24*/ MOV         R0, R4 
    /*00002b28*/ MOV         R2, #9 
    /*00002b2c*/ MOVW        R1, #13144                  @ 0x3358 
    /*00002b30*/ BL          mn34220_write_register
    /*00002b34*/ MOV         R0, R4 
    /*00002b38*/ MOV         R2, #11 
    /*00002b3c*/ MOVW        R1, #13145                  @ 0x3359 
    /*00002b40*/ BL          mn34220_write_register
    /*00002b44*/ MOV         R0, R4 
    /*00002b48*/ MOV         R2, #13 
    /*00002b4c*/ MOVW        R1, #13146                  @ 0x335a 
    /*00002b50*/ BL          mn34220_write_register
    /*00002b54*/ MOV         R0, R4 
    /*00002b58*/ MOV         R2, #0 
    /*00002b5c*/ MOVW        R1, #13147                  @ 0x335b 
    /*00002b60*/ BL          mn34220_write_register
    /*00002b64*/ MOV         R0, R4 
    /*00002b68*/ MOV         R2, #0 
    /*00002b6c*/ MOVW        R1, #13148                  @ 0x335c 
    /*00002b70*/ BL          mn34220_write_register
    /*00002b74*/ MOV         R0, R4 
    /*00002b78*/ MOV         R2, #1 
    /*00002b7c*/ MOVW        R1, #13149                  @ 0x335d 
    /*00002b80*/ BL          mn34220_write_register
    /*00002b84*/ MOV         R0, R4 
    /*00002b88*/ MOV         R2, #13 
    /*00002b8c*/ MOVW        R1, #13150                  @ 0x335e 
    /*00002b90*/ BL          mn34220_write_register
    /*00002b94*/ MOV         R0, R4 
    /*00002b98*/ MOV         R2, #1 
    /*00002b9c*/ MOVW        R1, #13151                  @ 0x335f 
    /*00002ba0*/ BL          mn34220_write_register
    /*00002ba4*/ MOV         R0, R4 
    /*00002ba8*/ MOV         R2, #13 
    /*00002bac*/ MOVW        R1, #13152                  @ 0x3360 
    /*00002bb0*/ BL          mn34220_write_register
    /*00002bb4*/ MOV         R0, R4 
    /*00002bb8*/ MOV         R2, #7 
    /*00002bbc*/ MOVW        R1, #13153                  @ 0x3361 
    /*00002bc0*/ BL          mn34220_write_register
    /*00002bc4*/ MOV         R0, R4 
    /*00002bc8*/ MOV         R2, #13 
    /*00002bcc*/ MOVW        R1, #13154                  @ 0x3362 
    /*00002bd0*/ BL          mn34220_write_register
    /*00002bd4*/ MOV         R0, R4 
    /*00002bd8*/ MOV         R2, #0 
    /*00002bdc*/ MOVW        R1, #13155                  @ 0x3363 
    /*00002be0*/ BL          mn34220_write_register
    /*00002be4*/ MOV         R0, R4 
    /*00002be8*/ MOV         R2, #0 
    /*00002bec*/ MOVW        R1, #13156                  @ 0x3364 
    /*00002bf0*/ BL          mn34220_write_register
    /*00002bf4*/ MOV         R0, R4 
    /*00002bf8*/ MOV         R2, #1 
    /*00002bfc*/ MOVW        R1, #13157                  @ 0x3365 
    /*00002c00*/ BL          mn34220_write_register
    /*00002c04*/ MOV         R0, R4 
    /*00002c08*/ MOV         R2, #13 
    /*00002c0c*/ MOVW        R1, #13158                  @ 0x3366 
    /*00002c10*/ BL          mn34220_write_register
    /*00002c14*/ MOV         R0, R4 
    /*00002c18*/ MOV         R2, #0 
    /*00002c1c*/ MOVW        R1, #13159                  @ 0x3367 
    /*00002c20*/ BL          mn34220_write_register
    /*00002c24*/ MOV         R0, R4 
    /*00002c28*/ MOV         R2, #13 
    /*00002c2c*/ MOVW        R1, #13160                  @ 0x3368 
    /*00002c30*/ BL          mn34220_write_register
    /*00002c34*/ MOV         R0, R4 
    /*00002c38*/ MOV         R2, #1 
    /*00002c3c*/ MOVW        R1, #13161                  @ 0x3369 
    /*00002c40*/ BL          mn34220_write_register
    /*00002c44*/ MOV         R0, R4 
    /*00002c48*/ MOV         R2, #0 
    /*00002c4c*/ MOVW        R1, #13162                  @ 0x336a 
    /*00002c50*/ BL          mn34220_write_register
    /*00002c54*/ MOV         R0, R4 
    /*00002c58*/ MOV         R2, #3 
    /*00002c5c*/ MOVW        R1, #13163                  @ 0x336b 
    /*00002c60*/ BL          mn34220_write_register
    /*00002c64*/ MOV         R0, R4 
    /*00002c68*/ MOV         R2, #3 
    /*00002c6c*/ MOVW        R1, #13164                  @ 0x336c 
    /*00002c70*/ BL          mn34220_write_register
    /*00002c74*/ MOV         R0, R4 
    /*00002c78*/ MOV         R2, #0 
    /*00002c7c*/ MOVW        R1, #13165                  @ 0x336d 
    /*00002c80*/ BL          mn34220_write_register
    /*00002c84*/ MOV         R0, R4 
    /*00002c88*/ MOV         R2, #2 
    /*00002c8c*/ MOVW        R1, #13166                  @ 0x336e 
    /*00002c90*/ BL          mn34220_write_register
    /*00002c94*/ MOV         R0, R4 
    /*00002c98*/ MOV         R2, #0 
    /*00002c9c*/ MOVW        R1, #13167                  @ 0x336f 
    /*00002ca0*/ BL          mn34220_write_register
    /*00002ca4*/ MOV         R0, R4 
    /*00002ca8*/ MOV         R2, #0 
    /*00002cac*/ MOVW        R1, #13168                  @ 0x3370 
    /*00002cb0*/ BL          mn34220_write_register
    /*00002cb4*/ MOV         R0, R4 
    /*00002cb8*/ MOV         R2, #0 
    /*00002cbc*/ MOVW        R1, #13169                  @ 0x3371 
    /*00002cc0*/ BL          mn34220_write_register
    /*00002cc4*/ MOV         R0, R4 
    /*00002cc8*/ MOV         R2, #0 
    /*00002ccc*/ MOVW        R1, #13170                  @ 0x3372 
    /*00002cd0*/ BL          mn34220_write_register
    /*00002cd4*/ MOV         R0, R4 
    /*00002cd8*/ MOV         R2, #0 
    /*00002cdc*/ MOVW        R1, #13171                  @ 0x3373 
    /*00002ce0*/ BL          mn34220_write_register
    /*00002ce4*/ MOV         R0, R4 
    /*00002ce8*/ MOV         R2, #2 
    /*00002cec*/ MOVW        R1, #13172                  @ 0x3374 
    /*00002cf0*/ BL          mn34220_write_register
    /*00002cf4*/ MOV         R0, R4 
    /*00002cf8*/ MOV         R2, #0 
    /*00002cfc*/ MOVW        R1, #13173                  @ 0x3375 
    /*00002d00*/ BL          mn34220_write_register
    /*00002d04*/ MOV         R0, R4 
    /*00002d08*/ MOV         R2, #6 
    /*00002d0c*/ MOVW        R1, #13174                  @ 0x3376 
    /*00002d10*/ BL          mn34220_write_register
    /*00002d14*/ MOV         R0, R4 
    /*00002d18*/ MOV         R2, #1 
    /*00002d1c*/ MOVW        R1, #13175                  @ 0x3377 
    /*00002d20*/ BL          mn34220_write_register
    /*00002d24*/ MOV         R0, R4 
    /*00002d28*/ MOV         R2, #6 
    /*00002d2c*/ MOVW        R1, #13176                  @ 0x3378 
    /*00002d30*/ BL          mn34220_write_register
    /*00002d34*/ MOV         R0, R4 
    /*00002d38*/ MOV         R2, #6 
    /*00002d3c*/ MOVW        R1, #13177                  @ 0x3379 
    /*00002d40*/ BL          mn34220_write_register
    /*00002d44*/ MOV         R0, R4 
    /*00002d48*/ MOV         R2, #9 
    /*00002d4c*/ MOVW        R1, #13178                  @ 0x337a 
    /*00002d50*/ BL          mn34220_write_register
    /*00002d54*/ MOV         R0, R4 
    /*00002d58*/ MOV         R2, #0 
    /*00002d5c*/ MOVW        R1, #13179                  @ 0x337b 
    /*00002d60*/ BL          mn34220_write_register
    /*00002d64*/ MOV         R0, R4 
    /*00002d68*/ MOV         R2, #13 
    /*00002d6c*/ MOVW        R1, #13180                  @ 0x337c 
    /*00002d70*/ BL          mn34220_write_register
    /*00002d74*/ MOV         R0, R4 
    /*00002d78*/ MOV         R2, #0 
    /*00002d7c*/ MOVW        R1, #13181                  @ 0x337d 
    /*00002d80*/ BL          mn34220_write_register
    /*00002d84*/ MOV         R0, R4 
    /*00002d88*/ MOV         R2, #0 
    /*00002d8c*/ MOVW        R1, #13182                  @ 0x337e 
    /*00002d90*/ BL          mn34220_write_register
    /*00002d94*/ MOV         R0, R4 
    /*00002d98*/ MOV         R2, #1 
    /*00002d9c*/ MOVW        R1, #13183                  @ 0x337f 
    /*00002da0*/ BL          mn34220_write_register
    /*00002da4*/ MOV         R0, R4 
    /*00002da8*/ MOV         R2, #0 
    /*00002dac*/ MOV         R1, #13184                  @ 0x3380 
    /*00002db0*/ BL          mn34220_write_register
    /*00002db4*/ MOV         R0, R4 
    /*00002db8*/ MOV         R2, #0 
    /*00002dbc*/ MOVW        R1, #13185                  @ 0x3381 
    /*00002dc0*/ BL          mn34220_write_register
    /*00002dc4*/ MOV         R0, R4 
    /*00002dc8*/ MOV         R2, #0 
    /*00002dcc*/ MOVW        R1, #13186                  @ 0x3382 
    /*00002dd0*/ BL          mn34220_write_register
    /*00002dd4*/ MOV         R0, R4 
    /*00002dd8*/ MOV         R2, #0 
    /*00002ddc*/ MOVW        R1, #13187                  @ 0x3383 
    /*00002de0*/ BL          mn34220_write_register
    /*00002de4*/ MOV         R0, R4 
    /*00002de8*/ MOV         R2, #13 
    /*00002dec*/ MOVW        R1, #13188                  @ 0x3384 
    /*00002df0*/ BL          mn34220_write_register
    /*00002df4*/ MOV         R0, R4 
    /*00002df8*/ MOV         R2, #0 
    /*00002dfc*/ MOVW        R1, #13189                  @ 0x3385 
    /*00002e00*/ BL          mn34220_write_register
    /*00002e04*/ MOV         R0, R4 
    /*00002e08*/ MOV         R2, #0 
    /*00002e0c*/ MOVW        R1, #13190                  @ 0x3386 
    /*00002e10*/ BL          mn34220_write_register
    /*00002e14*/ MOV         R0, R4 
    /*00002e18*/ MOV         R2, #1 
    /*00002e1c*/ MOVW        R1, #13191                  @ 0x3387 
    /*00002e20*/ BL          mn34220_write_register
    /*00002e24*/ MOV         R0, R4 
    /*00002e28*/ MOV         R2, #13 
    /*00002e2c*/ MOVW        R1, #13192                  @ 0x3388 
    /*00002e30*/ BL          mn34220_write_register
    /*00002e34*/ MOV         R0, R4 
    /*00002e38*/ MOV         R2, #0 
    /*00002e3c*/ MOVW        R1, #13193                  @ 0x3389 
    /*00002e40*/ BL          mn34220_write_register
    /*00002e44*/ MOV         R0, R4 
    /*00002e48*/ MOV         R2, #0 
    /*00002e4c*/ MOVW        R1, #13194                  @ 0x338a 
    /*00002e50*/ BL          mn34220_write_register
    /*00002e54*/ MOV         R0, R4 
    /*00002e58*/ MOV         R2, #1 
    /*00002e5c*/ MOVW        R1, #13195                  @ 0x338b 
    /*00002e60*/ BL          mn34220_write_register
    /*00002e64*/ MOV         R0, R4 
    /*00002e68*/ MOV         R2, #4 
    /*00002e6c*/ MOVW        R1, #13196                  @ 0x338c 
    /*00002e70*/ BL          mn34220_write_register
    /*00002e74*/ MOV         R0, R4 
    /*00002e78*/ MOV         R2, #8 
    /*00002e7c*/ MOVW        R1, #13197                  @ 0x338d 
    /*00002e80*/ BL          mn34220_write_register
    /*00002e84*/ MOV         R0, R4 
    /*00002e88*/ MOV         R2, #10 
    /*00002e8c*/ MOVW        R1, #13198                  @ 0x338e 
    /*00002e90*/ BL          mn34220_write_register
    /*00002e94*/ MOV         R0, R4 
    /*00002e98*/ MOV         R2, #9 
    /*00002e9c*/ MOVW        R1, #13199                  @ 0x338f 
    /*00002ea0*/ BL          mn34220_write_register
    /*00002ea4*/ MOV         R0, R4 
    /*00002ea8*/ MOV         R2, #2 
    /*00002eac*/ MOVW        R1, #13200                  @ 0x3390 
    /*00002eb0*/ BL          mn34220_write_register
    /*00002eb4*/ MOV         R0, R4 
    /*00002eb8*/ MOV         R2, #15 
    /*00002ebc*/ MOVW        R1, #13201                  @ 0x3391 
    /*00002ec0*/ BL          mn34220_write_register
    /*00002ec4*/ MOV         R0, R4 
    /*00002ec8*/ MOV         R2, #4 
    /*00002ecc*/ MOVW        R1, #13202                  @ 0x3392 
    /*00002ed0*/ BL          mn34220_write_register
    /*00002ed4*/ MOV         R0, R4 
    /*00002ed8*/ MOV         R2, #9 
    /*00002edc*/ MOVW        R1, #13203                  @ 0x3393 
    /*00002ee0*/ BL          mn34220_write_register
    /*00002ee4*/ MOV         R0, R4 
    /*00002ee8*/ MOV         R2, #1 
    /*00002eec*/ MOVW        R1, #13204                  @ 0x3394 
    /*00002ef0*/ BL          mn34220_write_register
    /*00002ef4*/ MOV         R0, R4 
    /*00002ef8*/ MOV         R2, #10 
    /*00002efc*/ MOVW        R1, #13205                  @ 0x3395 
    /*00002f00*/ BL          mn34220_write_register
    /*00002f04*/ MOV         R0, R4 
    /*00002f08*/ MOV         R2, #7 
    /*00002f0c*/ MOVW        R1, #13206                  @ 0x3396 
    /*00002f10*/ BL          mn34220_write_register
    /*00002f14*/ MOV         R0, R4 
    /*00002f18*/ MOV         R2, #24 
    /*00002f1c*/ MOVW        R1, #13207                  @ 0x3397 
    /*00002f20*/ BL          mn34220_write_register
    /*00002f24*/ MOV         R0, R4 
    /*00002f28*/ MOV         R2, #0 
    /*00002f2c*/ MOVW        R1, #13208                  @ 0x3398 
    /*00002f30*/ BL          mn34220_write_register
    /*00002f34*/ MOV         R0, R4 
    /*00002f38*/ MOV         R2, #1 
    /*00002f3c*/ MOVW        R1, #13209                  @ 0x3399 
    /*00002f40*/ BL          mn34220_write_register
    /*00002f44*/ MOV         R0, R4 
    /*00002f48*/ MOV         R2, #1 
    /*00002f4c*/ MOVW        R1, #13210                  @ 0x339a 
    /*00002f50*/ BL          mn34220_write_register
    /*00002f54*/ MOV         R0, R4 
    /*00002f58*/ MOV         R2, #8 
    /*00002f5c*/ MOVW        R1, #13211                  @ 0x339b 
    /*00002f60*/ BL          mn34220_write_register
    /*00002f64*/ MOV         R0, R4 
    /*00002f68*/ MOV         R2, #13 
    /*00002f6c*/ MOVW        R1, #13212                  @ 0x339c 
    /*00002f70*/ BL          mn34220_write_register
    /*00002f74*/ MOV         R0, R4 
    /*00002f78*/ MOV         R2, #1 
    /*00002f7c*/ MOVW        R1, #13213                  @ 0x339d 
    /*00002f80*/ BL          mn34220_write_register
    /*00002f84*/ MOV         R0, R4 
    /*00002f88*/ MOV         R2, #0 
    /*00002f8c*/ MOVW        R1, #13214                  @ 0x339e 
    /*00002f90*/ BL          mn34220_write_register
    /*00002f94*/ MOV         R0, R4 
    /*00002f98*/ MOV         R2, #3 
    /*00002f9c*/ MOVW        R1, #13215                  @ 0x339f 
    /*00002fa0*/ BL          mn34220_write_register
    /*00002fa4*/ MOV         R0, R4 
    /*00002fa8*/ MOV         R2, #0 
    /*00002fac*/ MOVW        R1, #13216                  @ 0x33a0 
    /*00002fb0*/ BL          mn34220_write_register
    /*00002fb4*/ MOV         R0, R4 
    /*00002fb8*/ MOV         R2, #1 
    /*00002fbc*/ MOVW        R1, #13217                  @ 0x33a1 
    /*00002fc0*/ BL          mn34220_write_register
    /*00002fc4*/ MOV         R0, R4 
    /*00002fc8*/ MOV         R2, #3 
    /*00002fcc*/ MOVW        R1, #13218                  @ 0x33a2 
    /*00002fd0*/ BL          mn34220_write_register
    /*00002fd4*/ MOV         R0, R4 
    /*00002fd8*/ MOV         R2, #3 
    /*00002fdc*/ MOVW        R1, #13219                  @ 0x33a3 
    /*00002fe0*/ BL          mn34220_write_register
    /*00002fe4*/ MOV         R0, R4 
    /*00002fe8*/ MOV         R2, #0 
    /*00002fec*/ MOVW        R1, #13220                  @ 0x33a4 
    /*00002ff0*/ BL          mn34220_write_register
    /*00002ff4*/ MOV         R0, R4 
    /*00002ff8*/ MOV         R2, #3 
    /*00002ffc*/ MOVW        R1, #13221                  @ 0x33a5 
    /*00003000*/ BL          mn34220_write_register
    /*00003004*/ MOV         R0, R4 
    /*00003008*/ MOV         R2, #6 
    /*0000300c*/ MOVW        R1, #13222                  @ 0x33a6 
    /*00003010*/ BL          mn34220_write_register
    /*00003014*/ MOV         R0, R4 
    /*00003018*/ MOV         R2, #0 
    /*0000301c*/ MOVW        R1, #13223                  @ 0x33a7 
    /*00003020*/ BL          mn34220_write_register
    /*00003024*/ MOV         R0, R4 
    /*00003028*/ MOV         R2, #0 
    /*0000302c*/ MOVW        R1, #13224                  @ 0x33a8 
    /*00003030*/ BL          mn34220_write_register
    /*00003034*/ MOV         R0, R4 
    /*00003038*/ MOV         R2, #0 
    /*0000303c*/ MOVW        R1, #13225                  @ 0x33a9 
    /*00003040*/ BL          mn34220_write_register
    /*00003044*/ MOV         R0, R4 
    /*00003048*/ MOV         R2, #0 
    /*0000304c*/ MOVW        R1, #13226                  @ 0x33aa 
    /*00003050*/ BL          mn34220_write_register
    /*00003054*/ MOV         R0, R4 
    /*00003058*/ MOV         R2, #0 
    /*0000305c*/ MOVW        R1, #13227                  @ 0x33ab 
    /*00003060*/ BL          mn34220_write_register
    /*00003064*/ MOV         R0, R4 
    /*00003068*/ MOV         R2, #3 
    /*0000306c*/ MOVW        R1, #13228                  @ 0x33ac 
    /*00003070*/ BL          mn34220_write_register
    /*00003074*/ MOV         R0, R4 
    /*00003078*/ MOV         R2, #20 
    /*0000307c*/ MOVW        R1, #13229                  @ 0x33ad 
    /*00003080*/ BL          mn34220_write_register
    /*00003084*/ MOV         R0, R4 
    /*00003088*/ MOV         R2, #63                     @ 0x3f 
    /*0000308c*/ MOVW        R1, #13230                  @ 0x33ae 
    /*00003090*/ BL          mn34220_write_register
    /*00003094*/ MOV         R0, R4 
    /*00003098*/ MOV         R2, #17 
    /*0000309c*/ MOVW        R1, #13231                  @ 0x33af 
    /*000030a0*/ BL          mn34220_write_register
    /*000030a4*/ MOV         R0, R4 
    /*000030a8*/ MOV         R2, #10 
    /*000030ac*/ MOVW        R1, #13232                  @ 0x33b0 
    /*000030b0*/ BL          mn34220_write_register
    /*000030b4*/ MOV         R0, R4 
    /*000030b8*/ MOV         R2, #26 
    /*000030bc*/ MOVW        R1, #13233                  @ 0x33b1 
    /*000030c0*/ BL          mn34220_write_register
    /*000030c4*/ MOV         R0, R4 
    /*000030c8*/ MOV         R2, #0 
    /*000030cc*/ MOVW        R1, #13234                  @ 0x33b2 
    /*000030d0*/ BL          mn34220_write_register
    /*000030d4*/ MOV         R0, R4 
    /*000030d8*/ MOV         R2, #0 
    /*000030dc*/ MOVW        R1, #13235                  @ 0x33b3 
    /*000030e0*/ BL          mn34220_write_register
    /*000030e4*/ MOV         R0, R4 
    /*000030e8*/ MOV         R2, #0 
    /*000030ec*/ MOVW        R1, #13236                  @ 0x33b4 
    /*000030f0*/ BL          mn34220_write_register
    /*000030f4*/ MOV         R0, R4 
    /*000030f8*/ MOV         R2, #0 
    /*000030fc*/ MOVW        R1, #13237                  @ 0x33b5 
    /*00003100*/ BL          mn34220_write_register
    /*00003104*/ MOV         R0, R4 
    /*00003108*/ MOV         R2, #0 
    /*0000310c*/ MOV         R1, #255                    @ 0xff 
    /*00003110*/ BL          mn34220_write_register
    /*00003114*/ MOV         R0, R4 
    /*00003118*/ MOV         R2, #0 
    /*0000311c*/ MOV         R1, #255                    @ 0xff 
    /*00003120*/ BL          mn34220_write_register
    /*00003124*/ MOV         R0, R4 
    /*00003128*/ MOV         R2, #0 
    /*0000312c*/ MOV         R1, #255                    @ 0xff 
    /*00003130*/ BL          mn34220_write_register
    /*00003134*/ MOV         R0, R4 
    /*00003138*/ MOV         R2, #0 
    /*0000313c*/ MOV         R1, #255                    @ 0xff 
    /*00003140*/ BL          mn34220_write_register
    /*00003144*/ MOV         R0, R4 
    /*00003148*/ MOV         R2, #0 
    /*0000314c*/ MOV         R1, #255                    @ 0xff 
    /*00003150*/ BL          mn34220_write_register
    /*00003154*/ MOV         R0, R4 
    /*00003158*/ MOV         R2, #0 
    /*0000315c*/ MOV         R1, #255                    @ 0xff 
    /*00003160*/ BL          mn34220_write_register
    /*00003164*/ MOV         R0, R4 
    /*00003168*/ MOV         R2, #0 
    /*0000316c*/ MOV         R1, #255                    @ 0xff 
    /*00003170*/ BL          mn34220_write_register
    /*00003174*/ MOV         R0, R4 
    /*00003178*/ MOV         R2, #0 
    /*0000317c*/ MOV         R1, #255                    @ 0xff 
    /*00003180*/ BL          mn34220_write_register
    /*00003184*/ MOV         R0, R4 
    /*00003188*/ MOV         R2, #0 
    /*0000318c*/ MOV         R1, #255                    @ 0xff 
    /*00003190*/ BL          mn34220_write_register
    /*00003194*/ MOV         R0, R4 
    /*00003198*/ MOV         R2, #0 
    /*0000319c*/ MOV         R1, #255                    @ 0xff 
    /*000031a0*/ BL          mn34220_write_register
    /*000031a4*/ MOV         R0, R4 
    /*000031a8*/ MOV         R2, #0 
    /*000031ac*/ MOV         R1, #255                    @ 0xff 
    /*000031b0*/ BL          mn34220_write_register
    /*000031b4*/ MOV         R0, R4 
    /*000031b8*/ MOV         R2, #0 
    /*000031bc*/ MOV         R1, #255                    @ 0xff 
    /*000031c0*/ BL          mn34220_write_register
    /*000031c4*/ MOV         R0, R4 
    /*000031c8*/ MOV         R2, #0 
    /*000031cc*/ MOV         R1, #255                    @ 0xff 
    /*000031d0*/ BL          mn34220_write_register
    /*000031d4*/ MOV         R0, R4 
    /*000031d8*/ MOV         R2, #0 
    /*000031dc*/ MOV         R1, #255                    @ 0xff 
    /*000031e0*/ BL          mn34220_write_register
    /*000031e4*/ MOV         R0, R4 
    /*000031e8*/ MOV         R2, #0 
    /*000031ec*/ MOV         R1, #255                    @ 0xff 
    /*000031f0*/ BL          mn34220_write_register
    /*000031f4*/ MOV         R0, R4 
    /*000031f8*/ MOV         R2, #0 
    /*000031fc*/ MOV         R1, #255                    @ 0xff 
    /*00003200*/ BL          mn34220_write_register
    /*00003204*/ MOV         R0, R4 
    /*00003208*/ MOV         R2, #0 
    /*0000320c*/ MOV         R1, #255                    @ 0xff 
    /*00003210*/ BL          mn34220_write_register
    /*00003214*/ MOV         R0, R4 
    /*00003218*/ MOV         R2, #0 
    /*0000321c*/ MOV         R1, #255                    @ 0xff 
    /*00003220*/ BL          mn34220_write_register
    /*00003224*/ MOV         R0, R4 
    /*00003228*/ MOV         R2, #0 
    /*0000322c*/ MOV         R1, #12288                  @ 0x3000 
    /*00003230*/ BL          mn34220_write_register
    /*00003234*/ MOV         R0, R4 
    /*00003238*/ MOV         R2, #211                    @ 0xd3 
    /*0000323c*/ MOVW        R1, #12289                  @ 0x3001 
    /*00003240*/ BL          mn34220_write_register
    /*00003244*/ MOV         R0, R4 
    /*00003248*/ MOV         R2, #1 
    /*0000324c*/ MOV         R1, #256                    @ 0x100 
    /*00003250*/ BL          mn34220_write_register
    /*00003254*/ MOV         R0, R4 
    /*00003258*/ MOV         R2, #0 
    /*0000325c*/ MOVW        R1, #257                    @ 0x101 
    /*00003260*/ BL          mn34220_write_register
    /*00003264*/ LDR         R0, WORD_3274               @ LDR         R0, [PC, #8]                @ 0x0000000000003274 
    /*00003268*/ POP         {R4, LR} 
    /*0000326c*/ ADD         R0, PC, R0 
    mn34220_wdr_1080P_2to1_init_x3270:
    /*00003270*/ B           mn34220_wdr_1080P_2to1_init_x3270@ puts [R_ARM_JUMP24: +0x0] 
    /*00003274*/ WORD_3274: .word 0x00000000
FUNC_END mn34220_wdr_1080P_2to1_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_init
@ Size: 0x13c
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_init
    /*00000000*/ LDR         R3, WORD_0134               @ LDR         R3, [PC, #300]              @ 0x0000000000000134 
    /*00000004*/ LDR         R2, WORD_0138               @ LDR         R2, [PC, #300]              @ 0x0000000000000138 
    /*00000008*/ ADD         R3, PC, R3 
    /*0000000c*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000010*/ MOV         R4, R0 
    /*00000014*/ LDR         R5, [R3, R2] 
    /*00000018*/ LDR         R3, [R5, R0, LSL #2] 
    /*0000001c*/ LDR         R7, [R3, #12] 
    /*00000020*/ LDRB        R6, [R3, #8] 
    /*00000024*/ BL          mn34220_i2c_init
    /*00000028*/ CMP         R0, #0 
    /*0000002c*/ POPNE       {R4, R5, R6, R7, R8, PC} 
    /*00000030*/ CMP         R7, #3 
    /*00000034*/ BEQ         mn34220_init_x80
    /*00000038*/ CMP         R7, #6 
    /*0000003c*/ BEQ         mn34220_init_x9c
    /*00000040*/ CMP         R7, #9 
    /*00000044*/ BEQ         mn34220_init_xb8
    /*00000048*/ CMP         R6, #1 
    /*0000004c*/ BEQ         mn34220_init_xd4
    /*00000050*/ CMP         R6, #2 
    /*00000054*/ BEQ         mn34220_init_xe0
    /*00000058*/ CMP         R6, #3 
    /*0000005c*/ BEQ         mn34220_init_xf8
    /*00000060*/ CMP         R6, #4 
    /*00000064*/ BEQ         mn34220_init_x104
    /*00000068*/ CMP         R6, #5 
    /*0000006c*/ BEQ         mn34220_init_x110
    mn34220_init_x70:
    /*00000070*/ LDR         R3, [R5, R4, LSL #2] 
    /*00000074*/ MOV         R2, #1 
    /*00000078*/ STR         R2, [R3] 
    /*0000007c*/ POP         {R4, R5, R6, R7, R8, PC} 
    mn34220_init_x80:
    /*00000080*/ CMP         R6, #2 
    /*00000084*/ BEQ         mn34220_init_xec
    /*00000088*/ CMP         R6, #5 
    /*0000008c*/ BNE         mn34220_init_x70
    /*00000090*/ MOV         R0, R4 
    /*00000094*/ BL          mn34220_wdr_720p_2to1_init
    /*00000098*/ B           mn34220_init_x70
    mn34220_init_x9c:
    /*0000009c*/ CMP         R6, #2 
    /*000000a0*/ BEQ         mn34220_init_x11c
    /*000000a4*/ CMP         R6, #5 
    /*000000a8*/ BNE         mn34220_init_x70
    /*000000ac*/ MOV         R0, R4 
    /*000000b0*/ BL          mn34220_wdr_720p_3to1_init
    /*000000b4*/ B           mn34220_init_x70
    mn34220_init_xb8:
    /*000000b8*/ CMP         R6, #2 
    /*000000bc*/ BEQ         mn34220_init_x128
    /*000000c0*/ CMP         R6, #5 
    /*000000c4*/ BNE         mn34220_init_x70
    /*000000c8*/ MOV         R0, R4 
    /*000000cc*/ BL          mn34220_wdr_720p_4to1_init
    /*000000d0*/ B           mn34220_init_x70
    mn34220_init_xd4:
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ BL          mn34220_linear_1080p60_init
    /*000000dc*/ B           mn34220_init_x70
    mn34220_init_xe0:
    /*000000e0*/ MOV         R0, R4 
    /*000000e4*/ BL          mn34220_linear_1080p30_init
    /*000000e8*/ B           mn34220_init_x70
    mn34220_init_xec:
    /*000000ec*/ MOV         R0, R4 
    /*000000f0*/ BL          mn34220_wdr_1080P_2to1_init
    /*000000f4*/ B           mn34220_init_x70
    mn34220_init_xf8:
    /*000000f8*/ MOV         R0, R4 
    /*000000fc*/ BL          mn34220_linear_720p120_init
    /*00000100*/ B           mn34220_init_x70
    mn34220_init_x104:
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ BL          mn34220_linear_VGA240_init
    /*0000010c*/ B           mn34220_init_x70
    mn34220_init_x110:
    /*00000110*/ MOV         R0, R4 
    /*00000114*/ BL          mn34220_linear_720p30_init
    /*00000118*/ B           mn34220_init_x70
    mn34220_init_x11c:
    /*0000011c*/ MOV         R0, R4 
    /*00000120*/ BL          mn34220_wdr_1080p_3to1_init
    /*00000124*/ B           mn34220_init_x70
    mn34220_init_x128:
    /*00000128*/ MOV         R0, R4 
    /*0000012c*/ BL          mn34220_wdr_1080p_4to1_init
    /*00000130*/ B           mn34220_init_x70
    /*00000134*/ WORD_0134: .word 0x00000124
    /*00000138*/ WORD_0138: .word 0x00000000
FUNC_END mn34220_init


