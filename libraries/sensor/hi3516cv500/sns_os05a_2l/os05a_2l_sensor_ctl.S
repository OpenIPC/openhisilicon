@
@ -- FILES --
@

@ os05a_2l_sensor_ctl.c


unsigned char data[0] = {
};

unsigned char bss[0];

unsigned char data.g_fd[16] = {
    /*0000*/ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
};

_RODATA___FUNCTION___10518_0000: .ascii "os05a_2l_i2c_init\0"


_RODATA___FUNCTION___10534_0000: .ascii "os05a_2l_write_register\0",
_RODATA___FUNCTION___10534_0018: .ascii ""


_RODATA___FUNCTION___10569_0000: .ascii "os05a_2l_init\0"


_RODATA_OS05A_2L_ADDR_BYTE_0000: .ascii "\x02\0\0\0",
_RODATA_OS05A_2L_ADDR_BYTE_0004: .ascii ""


_RODATA_OS05A_2L_DATA_BYTE_0000: .ascii "\x01\0\0\0",
_RODATA_OS05A_2L_DATA_BYTE_0004: .ascii ""


_RODATA_OS05A_2L_I2C_ADDR_0000: .ascii "l"


_RODATA_OS05A_2L_I2C_INIT_STR1_4_0000: .ascii "/dev/i2c-%u\0",
_RODATA_OS05A_2L_I2C_INIT_STR1_4_000c: .ascii "[Func]:%s [Line]:%d [Info]:Open /dev/hi_i2c_drv-%u error!\n\0\0",
_RODATA_OS05A_2L_I2C_INIT_STR1_4_0048: .ascii "[Func]:%s [Line]:%d [Info]:I2C_SLAVE_FORCE error!\n\0"


_RODATA_OS05A_2L_INIT_STR1_4_0000: .ascii "[Func]:%s [Line]:%d [Info]:Not Support Image Mode %d\n\0"


_RODATA_OS05A_2L_INIT_5M_2688X1944_10BIT_WDR30_STR1_4_0000: .ascii "-------OV os05a_2lane_init_5M_2688x1944_10bit_wdr26 Initial OK!-------\0"


_RODATA_OS05A_2L_INIT_5M_2688X1944_12BIT_LINEAR30_STR1_4_0000: .ascii "-------OV os05a_2lane_init_5M_2688x1944_12bit_linear30 Initial OK!-------\0"


_RODATA_OS05A_2L_WRITE_REGISTER_STR1_4_0000: .ascii "[Func]:%s [Line]:%d [Info]:I2C_WRITE error!\n\0"


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.os05a_2l_i2c_init
@ Size: 0x14c
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN os05a_2l_i2c_init
    /*00000000*/ PUSH        {R4, R5, R6, R7, R8, R9, LR} 
    /*00000004*/ SUB         SP, SP, #36                 @ 0x24 
    /*00000008*/ LDR         R4, WORD_0124               @ LDR         R4, [PC, #276]              @ 0x0000000000000124 
    /*0000000c*/ ADD         R7, SP, #12 
    /*00000010*/ LDR         R8, WORD_0128               @ LDR         R8, [PC, #272]              @ 0x0000000000000128 
    /*00000014*/ ADD         R4, PC, R4 
    /*00000018*/ LDR         R3, WORD_012c               @ LDR         R3, [PC, #268]              @ 0x000000000000012c 
    /*0000001c*/ ADD         R8, PC, R8 
    /*00000020*/ LDR         R5, [R4, R3] 
    /*00000024*/ LDR         R2, [R8, R0, LSL #2] 
    /*00000028*/ VMOV.I32    Q8, #0                      @ 0x00000000 
    /*0000002c*/ LDR         R3, [R5] 
    /*00000030*/ CMP         R2, #0 
    /*00000034*/ STR         R3, [SP, #28] 
    /*00000038*/ VST1.8      {D16-D17}, [R7] 
    /*0000003c*/ BLT         os05a_2l_i2c_init_x60
    os05a_2l_i2c_init_x40:
    /*00000040*/ MOV         R7, #0 
    os05a_2l_i2c_init_x44:
    /*00000044*/ LDR         R2, [SP, #28] 
    /*00000048*/ MOV         R0, R7 
    /*0000004c*/ LDR         R3, [R5] 
    /*00000050*/ CMP         R2, R3 
    /*00000054*/ BNE         os05a_2l_i2c_init_x120
    /*00000058*/ ADD         SP, SP, #36                 @ 0x24 
    /*0000005c*/ POP         {R4, R5, R6, R7, R8, R9, PC} 
    os05a_2l_i2c_init_x60:
    /*00000060*/ LDR         R3, WORD_0130               @ LDR         R3, [PC, #200]              @ 0x0000000000000130 
    /*00000064*/ MOV         R6, R0 
    /*00000068*/ LDR         R2, WORD_0134               @ LDR         R2, [PC, #196]              @ 0x0000000000000134 
    /*0000006c*/ MOV         R1, #16 
    /*00000070*/ LDR         R3, [R4, R3] 
    /*00000074*/ MOV         R0, R7 
    /*00000078*/ ADD         R2, PC, R2 
    /*0000007c*/ LDRB        R9, [R3, R6] 
    /*00000080*/ MOV         R3, R9 
    /*00000084*/ BL          snprintf
    /*00000088*/ MOV         R0, R7 
    /*0000008c*/ MOV         R2, #384                    @ 0x180 
    /*00000090*/ MOV         R1, #2 
    /*00000094*/ BL          open
    /*00000098*/ STR         R0, [R8, R6, LSL #2] 
    /*0000009c*/ CMP         R0, #0 
    /*000000a0*/ BLT         os05a_2l_i2c_init_xf0
    /*000000a4*/ MOV         R2, #54                     @ 0x36 
    /*000000a8*/ MOVW        R1, #1798                   @ 0x706 
    /*000000ac*/ BL          ioctl
    /*000000b0*/ SUBS        R7, R0, #0 
    /*000000b4*/ BGE         os05a_2l_i2c_init_x40
    /*000000b8*/ LDR         R1, WORD_0138               @ LDR         R1, [PC, #120]              @ 0x0000000000000138 
    /*000000bc*/ MOV         R3, #86                     @ 0x56 
    /*000000c0*/ LDR         R2, WORD_013c               @ LDR         R2, [PC, #116]              @ 0x000000000000013c 
    /*000000c4*/ LDR         R0, [R4, R1] 
    /*000000c8*/ LDR         R1, WORD_0140               @ LDR         R1, [PC, #112]              @ 0x0000000000000140 
    /*000000cc*/ ADD         R2, PC, R2 
    /*000000d0*/ ADD         R1, PC, R1 
    /*000000d4*/ LDR         R0, [R0] 
    /*000000d8*/ BL          fprintf
    /*000000dc*/ LDR         R0, [R8, R6, LSL #2] 
    /*000000e0*/ BL          close
    /*000000e4*/ MVN         R3, #0 
    /*000000e8*/ STR         R3, [R8, R6, LSL #2] 
    /*000000ec*/ B           os05a_2l_i2c_init_x44
    os05a_2l_i2c_init_xf0:
    /*000000f0*/ LDR         R1, WORD_0138               @ LDR         R1, [PC, #64]               @ 0x0000000000000138 
    /*000000f4*/ MOV         R3, #79                     @ 0x4f 
    /*000000f8*/ LDR         R2, WORD_0144               @ LDR         R2, [PC, #68]               @ 0x0000000000000144 
    /*000000fc*/ MVN         R7, #0 
    /*00000100*/ LDR         R0, [R4, R1] 
    /*00000104*/ LDR         R1, WORD_0148               @ LDR         R1, [PC, #60]               @ 0x0000000000000148 
    /*00000108*/ ADD         R2, PC, R2 
    /*0000010c*/ ADD         R1, PC, R1 
    /*00000110*/ LDR         R0, [R0] 
    /*00000114*/ STR         R9, [SP] 
    /*00000118*/ BL          fprintf
    /*0000011c*/ B           os05a_2l_i2c_init_x44
    os05a_2l_i2c_init_x120:
    /*00000120*/ BL          __stack_chk_fail
    /*00000124*/ WORD_0124: .word 0x00000108
    /*00000128*/ WORD_0128: .word 0x00000104
    /*0000012c*/ WORD_012c: .word 0x00000000
    /*00000130*/ WORD_0130: .word 0x00000000
    /*00000134*/ WORD_0134: .word 0x000000b4
    /*00000138*/ WORD_0138: .word 0x00000000
    /*0000013c*/ WORD_013c: .word 0x00000068
    /*00000140*/ WORD_0140: .word 0x00000068
    /*00000144*/ WORD_0144: .word 0x00000034
    /*00000148*/ WORD_0148: .word 0x00000034
FUNC_END os05a_2l_i2c_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.os05a_2l_i2c_exit
@ Size: 0x40
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN os05a_2l_i2c_exit
    /*00000000*/ PUSH        {R4, R5, R6, LR} 
    /*00000004*/ LDR         R5, WORD_003c               @ LDR         R5, [PC, #48]               @ 0x000000000000003c 
    /*00000008*/ ADD         R5, PC, R5 
    /*0000000c*/ LDR         R3, [R5, R0, LSL #2] 
    /*00000010*/ CMP         R3, #0 
    /*00000014*/ BLT         os05a_2l_i2c_exit_x34
    /*00000018*/ MOV         R4, R0 
    /*0000001c*/ MOV         R0, R3 
    /*00000020*/ BL          close
    /*00000024*/ MVN         R3, #0 
    /*00000028*/ MOV         R0, #0 
    /*0000002c*/ STR         R3, [R5, R4, LSL #2] 
    /*00000030*/ POP         {R4, R5, R6, PC} 
    os05a_2l_i2c_exit_x34:
    /*00000034*/ MVN         R0, #0 
    /*00000038*/ POP         {R4, R5, R6, PC} 
    /*0000003c*/ WORD_003c: .word 0x0000002c
FUNC_END os05a_2l_i2c_exit


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.os05a_2l_read_register
@ Size: 0x8
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN os05a_2l_read_register
    /*00000000*/ MOV         R0, #0 
    /*00000004*/ BX          LR 
FUNC_END os05a_2l_read_register


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.os05a_2l_write_register
@ Size: 0xc0
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN os05a_2l_write_register
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
    /*00000030*/ BLT         os05a_2l_write_register_x5c
    /*00000034*/ STRB        R1, [SP, #5] 
    /*00000038*/ MOV         LR, R2 
    /*0000003c*/ ASR         R1, R1, #8 
    /*00000040*/ MOV         R2, #3 
    /*00000044*/ STRB        R1, [SP, #4] 
    /*00000048*/ ADD         R1, SP, #4 
    /*0000004c*/ STRB        LR, [SP, #6] 
    /*00000050*/ BL          write
    /*00000054*/ CMP         R0, #0 
    /*00000058*/ BLT         os05a_2l_write_register_x78
    os05a_2l_write_register_x5c:
    /*0000005c*/ MOV         R0, #0 
    os05a_2l_write_register_x60:
    /*00000060*/ LDR         R2, [SP, #12] 
    /*00000064*/ LDR         R3, [R5] 
    /*00000068*/ CMP         R2, R3 
    /*0000006c*/ BNE         os05a_2l_write_register_xa4
    /*00000070*/ ADD         SP, SP, #20 
    /*00000074*/ POP         {R4, R5, PC} 
    os05a_2l_write_register_x78:
    /*00000078*/ LDR         R1, WORD_00b4               @ LDR         R1, [PC, #52]               @ 0x00000000000000b4 
    /*0000007c*/ MOV         R3, #170                    @ 0xaa 
    /*00000080*/ LDR         R2, WORD_00b8               @ LDR         R2, [PC, #48]               @ 0x00000000000000b8 
    /*00000084*/ LDR         R0, [R4, R1] 
    /*00000088*/ LDR         R1, WORD_00bc               @ LDR         R1, [PC, #44]               @ 0x00000000000000bc 
    /*0000008c*/ ADD         R2, PC, R2 
    /*00000090*/ ADD         R1, PC, R1 
    /*00000094*/ LDR         R0, [R0] 
    /*00000098*/ BL          fprintf
    /*0000009c*/ MVN         R0, #0 
    /*000000a0*/ B           os05a_2l_write_register_x60
    os05a_2l_write_register_xa4:
    /*000000a4*/ BL          __stack_chk_fail
    /*000000a8*/ WORD_00a8: .word 0x00000090
    /*000000ac*/ WORD_00ac: .word 0x00000090
    /*000000b0*/ WORD_00b0: .word 0x00000000
    /*000000b4*/ WORD_00b4: .word 0x00000000
    /*000000b8*/ WORD_00b8: .word 0x00000024
    /*000000bc*/ WORD_00bc: .word 0x00000024
FUNC_END os05a_2l_write_register


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.os05a_2l_standby
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN os05a_2l_standby
    /*00000000*/ BX          LR 
FUNC_END os05a_2l_standby


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.os05a_2l_restart
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN os05a_2l_restart
    /*00000000*/ BX          LR 
FUNC_END os05a_2l_restart


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.os05a_2l_prog
@ Size: 0x54
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN os05a_2l_prog
    /*00000000*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000004*/ MOV         R6, R0 
    /*00000008*/ SUB         R4, R1, #4 
    /*0000000c*/ MOVW        R7, #65535                  @ 0xffff 
    /*00000010*/ MOVW        R5, #65534                  @ 0xfffe 
    /*00000014*/ MOV         R8, #1000                   @ 0x3e8 
    os05a_2l_prog_x18:
    /*00000018*/ LDR         R2, [R4, #4]! 
    /*0000001c*/ MOV         R0, R6 
    /*00000020*/ LSR         IP, R2, #16 
    /*00000024*/ UXTH        R3, R2 
    /*00000028*/ CMP         IP, R5 
    /*0000002c*/ MOV         R2, R3 
    /*00000030*/ MOV         R1, IP 
    /*00000034*/ BEQ         os05a_2l_prog_x48
    /*00000038*/ CMP         IP, R7 
    /*0000003c*/ POPEQ       {R4, R5, R6, R7, R8, PC} 
    /*00000040*/ BL          os05a_2l_write_register
    /*00000044*/ B           os05a_2l_prog_x18
    os05a_2l_prog_x48:
    /*00000048*/ MUL         R0, R8, R3 
    /*0000004c*/ BL          usleep
    /*00000050*/ B           os05a_2l_prog_x18
FUNC_END os05a_2l_prog


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.os05a_2l_default_reg_init
@ Size: 0x6c
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN os05a_2l_default_reg_init
    /*00000000*/ LDR         R3, WORD_0064               @ LDR         R3, [PC, #92]               @ 0x0000000000000064 
    /*00000004*/ LDR         R2, WORD_0068               @ LDR         R2, [PC, #92]               @ 0x0000000000000068 
    /*00000008*/ ADD         R3, PC, R3 
    /*0000000c*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000010*/ LDR         R6, [R3, R2] 
    /*00000014*/ LDR         R3, [R6, R0, LSL #2] 
    /*00000018*/ LDR         R2, [R3, #20] 
    /*0000001c*/ CMP         R2, #0 
    /*00000020*/ POPEQ       {R4, R5, R6, R7, R8, PC} 
    /*00000024*/ MOV         R5, R0 
    /*00000028*/ MOV         R2, #0 
    /*0000002c*/ MOV         R7, #24 
    os05a_2l_default_reg_init_x30:
    /*00000030*/ ADD         R4, R2, #1 
    /*00000034*/ MOV         R0, R5 
    /*00000038*/ MLA         R2, R7, R2, R3 
    /*0000003c*/ MLA         R3, R7, R4, R3 
    /*00000040*/ LDR         R1, [R2, #36]               @ 0x24 
    /*00000044*/ LDR         R2, [R3, #20] 
    /*00000048*/ BL          os05a_2l_write_register
    /*0000004c*/ LDR         R3, [R6, R5, LSL #2] 
    /*00000050*/ MOV         R2, R4 
    /*00000054*/ LDR         R1, [R3, #20] 
    /*00000058*/ CMP         R1, R4 
    /*0000005c*/ BHI         os05a_2l_default_reg_init_x30
    /*00000060*/ POP         {R4, R5, R6, R7, R8, PC} 
    /*00000064*/ WORD_0064: .word 0x00000054
    /*00000068*/ WORD_0068: .word 0x00000000
FUNC_END os05a_2l_default_reg_init


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.os05a_2l_exit
@ Size: 0x4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN os05a_2l_exit
    os05a_2l_exit_x0:
    /*00000000*/ B           os05a_2l_exit_x0            @ os05a_2l_i2c_exit [R_ARM_JUMP24: .text.os05a_2l_i2c_exit+0x0] 
FUNC_END os05a_2l_exit


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.os05a_2l_init_5M_2688x1944_10bit_wdr30
@ Size: 0xe00
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN os05a_2l_init_5M_2688x1944_10bit_wdr30
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #1 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOV         R1, #17920                  @ 0x4600 
    /*00000010*/ BL          os05a_2l_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #4 
    /*0000001c*/ MOVW        R1, #17921                  @ 0x4601 
    /*00000020*/ BL          os05a_2l_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #0 
    /*0000002c*/ MOVW        R1, #17923                  @ 0x4603 
    /*00000030*/ BL          os05a_2l_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #1 
    /*0000003c*/ MOVW        R1, #259                    @ 0x103 
    /*00000040*/ BL          os05a_2l_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #1 
    /*0000004c*/ MOVW        R1, #771                    @ 0x303 
    /*00000050*/ BL          os05a_2l_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #90                     @ 0x5a 
    /*0000005c*/ MOVW        R1, #773                    @ 0x305 
    /*00000060*/ BL          os05a_2l_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #0 
    /*0000006c*/ MOVW        R1, #774                    @ 0x306 
    /*00000070*/ BL          os05a_2l_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #0 
    /*0000007c*/ MOVW        R1, #775                    @ 0x307 
    /*00000080*/ BL          os05a_2l_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #3 
    /*0000008c*/ MOV         R1, #776                    @ 0x308 
    /*00000090*/ BL          os05a_2l_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #4 
    /*0000009c*/ MOVW        R1, #777                    @ 0x309 
    /*000000a0*/ BL          os05a_2l_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #0 
    /*000000ac*/ MOVW        R1, #810                    @ 0x32a 
    /*000000b0*/ BL          os05a_2l_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #9 
    /*000000bc*/ MOVW        R1, #798                    @ 0x31e 
    /*000000c0*/ BL          os05a_2l_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #72                     @ 0x48 
    /*000000cc*/ MOVW        R1, #805                    @ 0x325 
    /*000000d0*/ BL          os05a_2l_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #7 
    /*000000dc*/ MOV         R1, #808                    @ 0x328 
    /*000000e0*/ BL          os05a_2l_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #17 
    /*000000ec*/ MOVW        R1, #12301                  @ 0x300d 
    /*000000f0*/ BL          os05a_2l_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #17 
    /*000000fc*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000100*/ BL          os05a_2l_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #17 
    /*0000010c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000110*/ BL          os05a_2l_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #1 
    /*0000011c*/ MOVW        R1, #12304                  @ 0x3010 
    /*00000120*/ BL          os05a_2l_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #33                     @ 0x21 
    /*0000012c*/ MOVW        R1, #12306                  @ 0x3012 
    /*00000130*/ BL          os05a_2l_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #240                    @ 0xf0 
    /*0000013c*/ MOVW        R1, #12310                  @ 0x3016 
    /*00000140*/ BL          os05a_2l_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #240                    @ 0xf0 
    /*0000014c*/ MOVW        R1, #12312                  @ 0x3018 
    /*00000150*/ BL          os05a_2l_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #240                    @ 0xf0 
    /*0000015c*/ MOVW        R1, #12328                  @ 0x3028 
    /*00000160*/ BL          os05a_2l_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #152                    @ 0x98 
    /*0000016c*/ MOVW        R1, #12318                  @ 0x301e 
    /*00000170*/ BL          os05a_2l_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #4 
    /*0000017c*/ MOVW        R1, #12304                  @ 0x3010 
    /*00000180*/ BL          os05a_2l_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #6 
    /*0000018c*/ MOVW        R1, #12305                  @ 0x3011 
    /*00000190*/ BL          os05a_2l_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #169                    @ 0xa9 
    /*0000019c*/ MOVW        R1, #12337                  @ 0x3031 
    /*000001a0*/ BL          os05a_2l_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #72                     @ 0x48 
    /*000001ac*/ MOVW        R1, #12547                  @ 0x3103 
    /*000001b0*/ BL          os05a_2l_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #1 
    /*000001bc*/ MOVW        R1, #12548                  @ 0x3104 
    /*000001c0*/ BL          os05a_2l_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #16 
    /*000001cc*/ MOVW        R1, #12550                  @ 0x3106 
    /*000001d0*/ BL          os05a_2l_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #9 
    /*000001dc*/ MOVW        R1, #13569                  @ 0x3501 
    /*000001e0*/ BL          os05a_2l_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #44                     @ 0x2c 
    /*000001ec*/ MOVW        R1, #13570                  @ 0x3502 
    /*000001f0*/ BL          os05a_2l_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #131                    @ 0x83 
    /*000001fc*/ MOVW        R1, #13573                  @ 0x3505 
    /*00000200*/ BL          os05a_2l_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #0 
    /*0000020c*/ MOVW        R1, #13576                  @ 0x3508 
    /*00000210*/ BL          os05a_2l_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #128                    @ 0x80 
    /*0000021c*/ MOVW        R1, #13577                  @ 0x3509 
    /*00000220*/ BL          os05a_2l_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #4 
    /*0000022c*/ MOVW        R1, #13578                  @ 0x350a 
    /*00000230*/ BL          os05a_2l_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #0 
    /*0000023c*/ MOVW        R1, #13579                  @ 0x350b 
    /*00000240*/ BL          os05a_2l_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #0 
    /*0000024c*/ MOVW        R1, #13580                  @ 0x350c 
    /*00000250*/ BL          os05a_2l_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #128                    @ 0x80 
    /*0000025c*/ MOVW        R1, #13581                  @ 0x350d 
    /*00000260*/ BL          os05a_2l_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #4 
    /*0000026c*/ MOVW        R1, #13582                  @ 0x350e 
    /*00000270*/ BL          os05a_2l_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #0 
    /*0000027c*/ MOVW        R1, #13583                  @ 0x350f 
    /*00000280*/ BL          os05a_2l_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #0 
    /*0000028c*/ MOV         R1, #13824                  @ 0x3600 
    /*00000290*/ BL          os05a_2l_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #255                    @ 0xff 
    /*0000029c*/ MOVW        R1, #13862                  @ 0x3626 
    /*000002a0*/ BL          os05a_2l_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #80                     @ 0x50 
    /*000002ac*/ MOVW        R1, #13829                  @ 0x3605 
    /*000002b0*/ BL          os05a_2l_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #181                    @ 0xb5 
    /*000002bc*/ MOVW        R1, #13833                  @ 0x3609 
    /*000002c0*/ BL          os05a_2l_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #105                    @ 0x69 
    /*000002cc*/ MOVW        R1, #13840                  @ 0x3610 
    /*000002d0*/ BL          os05a_2l_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #1 
    /*000002dc*/ MOVW        R1, #13836                  @ 0x360c 
    /*000002e0*/ BL          os05a_2l_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #164                    @ 0xa4 
    /*000002ec*/ MOVW        R1, #13864                  @ 0x3628 
    /*000002f0*/ BL          os05a_2l_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #106                    @ 0x6a 
    /*000002fc*/ MOVW        R1, #13865                  @ 0x3629 
    /*00000300*/ BL          os05a_2l_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #16 
    /*0000030c*/ MOVW        R1, #13869                  @ 0x362d 
    /*00000310*/ BL          os05a_2l_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #66                     @ 0x42 
    /*0000031c*/ MOVW        R1, #13920                  @ 0x3660 
    /*00000320*/ BL          os05a_2l_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #7 
    /*0000032c*/ MOVW        R1, #13921                  @ 0x3661 
    /*00000330*/ BL          os05a_2l_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #0 
    /*0000033c*/ MOVW        R1, #13922                  @ 0x3662 
    /*00000340*/ BL          os05a_2l_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #40                     @ 0x28 
    /*0000034c*/ MOVW        R1, #13923                  @ 0x3663 
    /*00000350*/ BL          os05a_2l_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #13 
    /*0000035c*/ MOVW        R1, #13924                  @ 0x3664 
    /*00000360*/ BL          os05a_2l_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #56                     @ 0x38 
    /*0000036c*/ MOVW        R1, #13930                  @ 0x366a 
    /*00000370*/ BL          os05a_2l_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #160                    @ 0xa0 
    /*0000037c*/ MOVW        R1, #13931                  @ 0x366b 
    /*00000380*/ BL          os05a_2l_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #0 
    /*0000038c*/ MOVW        R1, #13933                  @ 0x366d 
    /*00000390*/ BL          os05a_2l_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #0 
    /*0000039c*/ MOVW        R1, #13934                  @ 0x366e 
    /*000003a0*/ BL          os05a_2l_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #0 
    /*000003ac*/ MOV         R1, #13952                  @ 0x3680 
    /*000003b0*/ BL          os05a_2l_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #0 
    /*000003bc*/ MOV         R1, #14016                  @ 0x36c0 
    /*000003c0*/ BL          os05a_2l_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #129                    @ 0x81 
    /*000003cc*/ MOVW        R1, #13857                  @ 0x3621 
    /*000003d0*/ BL          os05a_2l_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #49                     @ 0x31 
    /*000003dc*/ MOVW        R1, #13876                  @ 0x3634 
    /*000003e0*/ BL          os05a_2l_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #0 
    /*000003ec*/ MOVW        R1, #13856                  @ 0x3620 
    /*000003f0*/ BL          os05a_2l_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #0 
    /*000003fc*/ MOVW        R1, #13858                  @ 0x3622 
    /*00000400*/ BL          os05a_2l_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #208                    @ 0xd0 
    /*0000040c*/ MOVW        R1, #13866                  @ 0x362a 
    /*00000410*/ BL          os05a_2l_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #140                    @ 0x8c 
    /*0000041c*/ MOVW        R1, #13870                  @ 0x362e 
    /*00000420*/ BL          os05a_2l_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #152                    @ 0x98 
    /*0000042c*/ MOVW        R1, #13871                  @ 0x362f 
    /*00000430*/ BL          os05a_2l_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #176                    @ 0xb0 
    /*0000043c*/ MOVW        R1, #13872                  @ 0x3630 
    /*00000440*/ BL          os05a_2l_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #215                    @ 0xd7 
    /*0000044c*/ MOVW        R1, #13873                  @ 0x3631 
    /*00000450*/ BL          os05a_2l_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #15 
    /*0000045c*/ MOVW        R1, #14081                  @ 0x3701 
    /*00000460*/ BL          os05a_2l_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #2 
    /*0000046c*/ MOVW        R1, #14135                  @ 0x3737 
    /*00000470*/ BL          os05a_2l_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #4 
    /*0000047c*/ MOVW        R1, #14145                  @ 0x3741 
    /*00000480*/ BL          os05a_2l_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #15 
    /*0000048c*/ MOVW        R1, #14140                  @ 0x373c 
    /*00000490*/ BL          os05a_2l_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #2 
    /*0000049c*/ MOVW        R1, #14139                  @ 0x373b 
    /*000004a0*/ BL          os05a_2l_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #0 
    /*000004ac*/ MOVW        R1, #14085                  @ 0x3705 
    /*000004b0*/ BL          os05a_2l_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #80                     @ 0x50 
    /*000004bc*/ MOVW        R1, #14086                  @ 0x3706 
    /*000004c0*/ BL          os05a_2l_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #0 
    /*000004cc*/ MOVW        R1, #14090                  @ 0x370a 
    /*000004d0*/ BL          os05a_2l_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #228                    @ 0xe4 
    /*000004dc*/ MOVW        R1, #14091                  @ 0x370b 
    /*000004e0*/ BL          os05a_2l_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #74                     @ 0x4a 
    /*000004ec*/ MOVW        R1, #14089                  @ 0x3709 
    /*000004f0*/ BL          os05a_2l_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #33                     @ 0x21 
    /*000004fc*/ MOVW        R1, #14100                  @ 0x3714 
    /*00000500*/ BL          os05a_2l_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #0 
    /*0000050c*/ MOVW        R1, #14108                  @ 0x371c 
    /*00000510*/ BL          os05a_2l_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #8 
    /*0000051c*/ MOVW        R1, #14109                  @ 0x371d 
    /*00000520*/ BL          os05a_2l_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #14 
    /*0000052c*/ MOVW        R1, #14174                  @ 0x375e 
    /*00000530*/ BL          os05a_2l_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #19 
    /*0000053c*/ MOVW        R1, #14176                  @ 0x3760 
    /*00000540*/ BL          os05a_2l_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #16 
    /*0000054c*/ MOVW        R1, #14198                  @ 0x3776 
    /*00000550*/ BL          os05a_2l_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #2 
    /*0000055c*/ MOVW        R1, #14209                  @ 0x3781 
    /*00000560*/ BL          os05a_2l_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #4 
    /*0000056c*/ MOVW        R1, #14210                  @ 0x3782 
    /*00000570*/ BL          os05a_2l_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #2 
    /*0000057c*/ MOVW        R1, #14211                  @ 0x3783 
    /*00000580*/ BL          os05a_2l_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #8 
    /*0000058c*/ MOVW        R1, #14212                  @ 0x3784 
    /*00000590*/ BL          os05a_2l_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #8 
    /*0000059c*/ MOVW        R1, #14213                  @ 0x3785 
    /*000005a0*/ BL          os05a_2l_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #1 
    /*000005ac*/ MOVW        R1, #14216                  @ 0x3788 
    /*000005b0*/ BL          os05a_2l_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #1 
    /*000005bc*/ MOVW        R1, #14217                  @ 0x3789 
    /*000005c0*/ BL          os05a_2l_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #4 
    /*000005cc*/ MOVW        R1, #14231                  @ 0x3797 
    /*000005d0*/ BL          os05a_2l_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #1 
    /*000005dc*/ MOVW        R1, #14232                  @ 0x3798 
    /*000005e0*/ BL          os05a_2l_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #0 
    /*000005ec*/ MOVW        R1, #14233                  @ 0x3799 
    /*000005f0*/ BL          os05a_2l_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #2 
    /*000005fc*/ MOVW        R1, #14177                  @ 0x3761 
    /*00000600*/ BL          os05a_2l_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #13 
    /*0000060c*/ MOVW        R1, #14178                  @ 0x3762 
    /*00000610*/ BL          os05a_2l_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #0 
    /*0000061c*/ MOV         R1, #14336                  @ 0x3800 
    /*00000620*/ BL          os05a_2l_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #0 
    /*0000062c*/ MOVW        R1, #14337                  @ 0x3801 
    /*00000630*/ BL          os05a_2l_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #0 
    /*0000063c*/ MOVW        R1, #14338                  @ 0x3802 
    /*00000640*/ BL          os05a_2l_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #12 
    /*0000064c*/ MOVW        R1, #14339                  @ 0x3803 
    /*00000650*/ BL          os05a_2l_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #14 
    /*0000065c*/ MOVW        R1, #14340                  @ 0x3804 
    /*00000660*/ BL          os05a_2l_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #255                    @ 0xff 
    /*0000066c*/ MOVW        R1, #14341                  @ 0x3805 
    /*00000670*/ BL          os05a_2l_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #8 
    /*0000067c*/ MOVW        R1, #14342                  @ 0x3806 
    /*00000680*/ BL          os05a_2l_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #111                    @ 0x6f 
    /*0000068c*/ MOVW        R1, #14343                  @ 0x3807 
    /*00000690*/ BL          os05a_2l_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #10 
    /*0000069c*/ MOVW        R1, #14344                  @ 0x3808 
    /*000006a0*/ BL          os05a_2l_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #32 
    /*000006ac*/ MOVW        R1, #14345                  @ 0x3809 
    /*000006b0*/ BL          os05a_2l_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #7 
    /*000006bc*/ MOVW        R1, #14346                  @ 0x380a 
    /*000006c0*/ BL          os05a_2l_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #152                    @ 0x98 
    /*000006cc*/ MOVW        R1, #14347                  @ 0x380b 
    /*000006d0*/ BL          os05a_2l_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #3 
    /*000006dc*/ MOVW        R1, #14348                  @ 0x380c 
    /*000006e0*/ BL          os05a_2l_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #240                    @ 0xf0 
    /*000006ec*/ MOVW        R1, #14349                  @ 0x380d 
    /*000006f0*/ BL          os05a_2l_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #9 
    /*000006fc*/ MOVW        R1, #14350                  @ 0x380e 
    /*00000700*/ BL          os05a_2l_write_register
    /*00000704*/ MOV         R0, R4 
    /*00000708*/ MOV         R2, #76                     @ 0x4c 
    /*0000070c*/ MOVW        R1, #14351                  @ 0x380f 
    /*00000710*/ BL          os05a_2l_write_register
    /*00000714*/ MOV         R0, R4 
    /*00000718*/ MOV         R2, #4 
    /*0000071c*/ MOVW        R1, #14355                  @ 0x3813 
    /*00000720*/ BL          os05a_2l_write_register
    /*00000724*/ MOV         R0, R4 
    /*00000728*/ MOV         R2, #1 
    /*0000072c*/ MOVW        R1, #14356                  @ 0x3814 
    /*00000730*/ BL          os05a_2l_write_register
    /*00000734*/ MOV         R0, R4 
    /*00000738*/ MOV         R2, #1 
    /*0000073c*/ MOVW        R1, #14357                  @ 0x3815 
    /*00000740*/ BL          os05a_2l_write_register
    /*00000744*/ MOV         R0, R4 
    /*00000748*/ MOV         R2, #1 
    /*0000074c*/ MOVW        R1, #14358                  @ 0x3816 
    /*00000750*/ BL          os05a_2l_write_register
    /*00000754*/ MOV         R0, R4 
    /*00000758*/ MOV         R2, #1 
    /*0000075c*/ MOVW        R1, #14359                  @ 0x3817 
    /*00000760*/ BL          os05a_2l_write_register
    /*00000764*/ MOV         R0, R4 
    /*00000768*/ MOV         R2, #8 
    /*0000076c*/ MOVW        R1, #14364                  @ 0x381c 
    /*00000770*/ BL          os05a_2l_write_register
    /*00000774*/ MOV         R0, R4 
    /*00000778*/ MOV         R2, #0 
    /*0000077c*/ MOVW        R1, #14368                  @ 0x3820 
    /*00000780*/ BL          os05a_2l_write_register
    /*00000784*/ MOV         R0, R4 
    /*00000788*/ MOV         R2, #36                     @ 0x24 
    /*0000078c*/ MOVW        R1, #14369                  @ 0x3821 
    /*00000790*/ BL          os05a_2l_write_register
    /*00000794*/ MOV         R0, R4 
    /*00000798*/ MOV         R2, #8 
    /*0000079c*/ MOVW        R1, #14371                  @ 0x3823 
    /*000007a0*/ BL          os05a_2l_write_register
    /*000007a4*/ MOV         R0, R4 
    /*000007a8*/ MOV         R2, #0 
    /*000007ac*/ MOVW        R1, #14374                  @ 0x3826 
    /*000007b0*/ BL          os05a_2l_write_register
    /*000007b4*/ MOV         R0, R4 
    /*000007b8*/ MOV         R2, #1 
    /*000007bc*/ MOVW        R1, #14375                  @ 0x3827 
    /*000007c0*/ BL          os05a_2l_write_register
    /*000007c4*/ MOV         R0, R4 
    /*000007c8*/ MOV         R2, #2 
    /*000007cc*/ MOVW        R1, #14386                  @ 0x3832 
    /*000007d0*/ BL          os05a_2l_write_register
    /*000007d4*/ MOV         R0, R4 
    /*000007d8*/ MOV         R2, #72                     @ 0x48 
    /*000007dc*/ MOVW        R1, #14396                  @ 0x383c 
    /*000007e0*/ BL          os05a_2l_write_register
    /*000007e4*/ MOV         R0, R4 
    /*000007e8*/ MOV         R2, #255                    @ 0xff 
    /*000007ec*/ MOVW        R1, #14397                  @ 0x383d 
    /*000007f0*/ BL          os05a_2l_write_register
    /*000007f4*/ MOV         R0, R4 
    /*000007f8*/ MOV         R2, #32 
    /*000007fc*/ MOVW        R1, #14403                  @ 0x3843 
    /*00000800*/ BL          os05a_2l_write_register
    /*00000804*/ MOV         R0, R4 
    /*00000808*/ MOV         R2, #8 
    /*0000080c*/ MOVW        R1, #14381                  @ 0x382d 
    /*00000810*/ BL          os05a_2l_write_register
    /*00000814*/ MOV         R0, R4 
    /*00000818*/ MOV         R2, #11 
    /*0000081c*/ MOVW        R1, #15749                  @ 0x3d85 
    /*00000820*/ BL          os05a_2l_write_register
    /*00000824*/ MOV         R0, R4 
    /*00000828*/ MOV         R2, #64                     @ 0x40 
    /*0000082c*/ MOVW        R1, #15748                  @ 0x3d84 
    /*00000830*/ BL          os05a_2l_write_register
    /*00000834*/ MOV         R0, R4 
    /*00000838*/ MOV         R2, #99                     @ 0x63 
    /*0000083c*/ MOVW        R1, #15756                  @ 0x3d8c 
    /*00000840*/ BL          os05a_2l_write_register
    /*00000844*/ MOV         R0, R4 
    /*00000848*/ MOV         R2, #0 
    /*0000084c*/ MOVW        R1, #15757                  @ 0x3d8d 
    /*00000850*/ BL          os05a_2l_write_register
    /*00000854*/ MOV         R0, R4 
    /*00000858*/ MOV         R2, #120                    @ 0x78 
    /*0000085c*/ MOV         R1, #16384                  @ 0x4000 
    /*00000860*/ BL          os05a_2l_write_register
    /*00000864*/ MOV         R0, R4 
    /*00000868*/ MOV         R2, #43                     @ 0x2b 
    /*0000086c*/ MOVW        R1, #16385                  @ 0x4001 
    /*00000870*/ BL          os05a_2l_write_register
    /*00000874*/ MOV         R0, R4 
    /*00000878*/ MOV         R2, #64                     @ 0x40 
    /*0000087c*/ MOVW        R1, #16389                  @ 0x4005 
    /*00000880*/ BL          os05a_2l_write_register
    /*00000884*/ MOV         R0, R4 
    /*00000888*/ MOV         R2, #47                     @ 0x2f 
    /*0000088c*/ MOVW        R1, #16424                  @ 0x4028 
    /*00000890*/ BL          os05a_2l_write_register
    /*00000894*/ MOV         R0, R4 
    /*00000898*/ MOV         R2, #1 
    /*0000089c*/ MOVW        R1, #16394                  @ 0x400a 
    /*000008a0*/ BL          os05a_2l_write_register
    /*000008a4*/ MOV         R0, R4 
    /*000008a8*/ MOV         R2, #18 
    /*000008ac*/ MOVW        R1, #16400                  @ 0x4010 
    /*000008b0*/ BL          os05a_2l_write_register
    /*000008b4*/ MOV         R0, R4 
    /*000008b8*/ MOV         R2, #2 
    /*000008bc*/ MOVW        R1, #16392                  @ 0x4008 
    /*000008c0*/ BL          os05a_2l_write_register
    /*000008c4*/ MOV         R0, R4 
    /*000008c8*/ MOV         R2, #13 
    /*000008cc*/ MOVW        R1, #16393                  @ 0x4009 
    /*000008d0*/ BL          os05a_2l_write_register
    /*000008d4*/ MOV         R0, R4 
    /*000008d8*/ MOV         R2, #88                     @ 0x58 
    /*000008dc*/ MOVW        R1, #16410                  @ 0x401a 
    /*000008e0*/ BL          os05a_2l_write_register
    /*000008e4*/ MOV         R0, R4 
    /*000008e8*/ MOV         R2, #0 
    /*000008ec*/ MOVW        R1, #16464                  @ 0x4050 
    /*000008f0*/ BL          os05a_2l_write_register
    /*000008f4*/ MOV         R0, R4 
    /*000008f8*/ MOV         R2, #1 
    /*000008fc*/ MOVW        R1, #16465                  @ 0x4051 
    /*00000900*/ BL          os05a_2l_write_register
    /*00000904*/ MOV         R0, R4 
    /*00000908*/ MOV         R2, #0 
    /*0000090c*/ MOVW        R1, #16466                  @ 0x4052 
    /*00000910*/ BL          os05a_2l_write_register
    /*00000914*/ MOV         R0, R4 
    /*00000918*/ MOV         R2, #128                    @ 0x80 
    /*0000091c*/ MOVW        R1, #16467                  @ 0x4053 
    /*00000920*/ BL          os05a_2l_write_register
    /*00000924*/ MOV         R0, R4 
    /*00000928*/ MOV         R2, #0 
    /*0000092c*/ MOVW        R1, #16468                  @ 0x4054 
    /*00000930*/ BL          os05a_2l_write_register
    /*00000934*/ MOV         R0, R4 
    /*00000938*/ MOV         R2, #128                    @ 0x80 
    /*0000093c*/ MOVW        R1, #16469                  @ 0x4055 
    /*00000940*/ BL          os05a_2l_write_register
    /*00000944*/ MOV         R0, R4 
    /*00000948*/ MOV         R2, #0 
    /*0000094c*/ MOVW        R1, #16470                  @ 0x4056 
    /*00000950*/ BL          os05a_2l_write_register
    /*00000954*/ MOV         R0, R4 
    /*00000958*/ MOV         R2, #128                    @ 0x80 
    /*0000095c*/ MOVW        R1, #16471                  @ 0x4057 
    /*00000960*/ BL          os05a_2l_write_register
    /*00000964*/ MOV         R0, R4 
    /*00000968*/ MOV         R2, #0 
    /*0000096c*/ MOVW        R1, #16472                  @ 0x4058 
    /*00000970*/ BL          os05a_2l_write_register
    /*00000974*/ MOV         R0, R4 
    /*00000978*/ MOV         R2, #128                    @ 0x80 
    /*0000097c*/ MOVW        R1, #16473                  @ 0x4059 
    /*00000980*/ BL          os05a_2l_write_register
    /*00000984*/ MOV         R0, R4 
    /*00000988*/ MOV         R2, #255                    @ 0xff 
    /*0000098c*/ MOVW        R1, #17163                  @ 0x430b 
    /*00000990*/ BL          os05a_2l_write_register
    /*00000994*/ MOV         R0, R4 
    /*00000998*/ MOV         R2, #255                    @ 0xff 
    /*0000099c*/ MOVW        R1, #17164                  @ 0x430c 
    /*000009a0*/ BL          os05a_2l_write_register
    /*000009a4*/ MOV         R0, R4 
    /*000009a8*/ MOV         R2, #0 
    /*000009ac*/ MOVW        R1, #17165                  @ 0x430d 
    /*000009b0*/ BL          os05a_2l_write_register
    /*000009b4*/ MOV         R0, R4 
    /*000009b8*/ MOV         R2, #0 
    /*000009bc*/ MOVW        R1, #17166                  @ 0x430e 
    /*000009c0*/ BL          os05a_2l_write_register
    /*000009c4*/ MOV         R0, R4 
    /*000009c8*/ MOV         R2, #24 
    /*000009cc*/ MOVW        R1, #17665                  @ 0x4501 
    /*000009d0*/ BL          os05a_2l_write_register
    /*000009d4*/ MOV         R0, R4 
    /*000009d8*/ MOV         R2, #0 
    /*000009dc*/ MOVW        R1, #17666                  @ 0x4502 
    /*000009e0*/ BL          os05a_2l_write_register
    /*000009e4*/ MOV         R0, R4 
    /*000009e8*/ MOV         R2, #0 
    /*000009ec*/ MOVW        R1, #17987                  @ 0x4643 
    /*000009f0*/ BL          os05a_2l_write_register
    /*000009f4*/ MOV         R0, R4 
    /*000009f8*/ MOV         R2, #1 
    /*000009fc*/ MOVW        R1, #17984                  @ 0x4640 
    /*00000a00*/ BL          os05a_2l_write_register
    /*00000a04*/ MOV         R0, R4 
    /*00000a08*/ MOV         R2, #4 
    /*00000a0c*/ MOVW        R1, #17985                  @ 0x4641 
    /*00000a10*/ BL          os05a_2l_write_register
    /*00000a14*/ MOV         R0, R4 
    /*00000a18*/ MOV         R2, #4 
    /*00000a1c*/ MOVW        R1, #18446                  @ 0x480e 
    /*00000a20*/ BL          os05a_2l_write_register
    /*00000a24*/ MOV         R0, R4 
    /*00000a28*/ MOV         R2, #152                    @ 0x98 
    /*00000a2c*/ MOVW        R1, #18451                  @ 0x4813 
    /*00000a30*/ BL          os05a_2l_write_register
    /*00000a34*/ MOV         R0, R4 
    /*00000a38*/ MOV         R2, #43                     @ 0x2b 
    /*00000a3c*/ MOVW        R1, #18453                  @ 0x4815 
    /*00000a40*/ BL          os05a_2l_write_register
    /*00000a44*/ MOV         R0, R4 
    /*00000a48*/ MOV         R2, #54                     @ 0x36 
    /*00000a4c*/ MOVW        R1, #18542                  @ 0x486e 
    /*00000a50*/ BL          os05a_2l_write_register
    /*00000a54*/ MOV         R0, R4 
    /*00000a58*/ MOV         R2, #132                    @ 0x84 
    /*00000a5c*/ MOVW        R1, #18543                  @ 0x486f 
    /*00000a60*/ BL          os05a_2l_write_register
    /*00000a64*/ MOV         R0, R4 
    /*00000a68*/ MOV         R2, #0 
    /*00000a6c*/ MOVW        R1, #18528                  @ 0x4860 
    /*00000a70*/ BL          os05a_2l_write_register
    /*00000a74*/ MOV         R0, R4 
    /*00000a78*/ MOV         R2, #160                    @ 0xa0 
    /*00000a7c*/ MOVW        R1, #18529                  @ 0x4861 
    /*00000a80*/ BL          os05a_2l_write_register
    /*00000a84*/ MOV         R0, R4 
    /*00000a88*/ MOV         R2, #5 
    /*00000a8c*/ MOVW        R1, #18507                  @ 0x484b 
    /*00000a90*/ BL          os05a_2l_write_register
    /*00000a94*/ MOV         R0, R4 
    /*00000a98*/ MOV         R2, #0 
    /*00000a9c*/ MOVW        R1, #18512                  @ 0x4850 
    /*00000aa0*/ BL          os05a_2l_write_register
    /*00000aa4*/ MOV         R0, R4 
    /*00000aa8*/ MOV         R2, #170                    @ 0xaa 
    /*00000aac*/ MOVW        R1, #18513                  @ 0x4851 
    /*00000ab0*/ BL          os05a_2l_write_register
    /*00000ab4*/ MOV         R0, R4 
    /*00000ab8*/ MOV         R2, #255                    @ 0xff 
    /*00000abc*/ MOVW        R1, #18514                  @ 0x4852 
    /*00000ac0*/ BL          os05a_2l_write_register
    /*00000ac4*/ MOV         R0, R4 
    /*00000ac8*/ MOV         R2, #138                    @ 0x8a 
    /*00000acc*/ MOVW        R1, #18515                  @ 0x4853 
    /*00000ad0*/ BL          os05a_2l_write_register
    /*00000ad4*/ MOV         R0, R4 
    /*00000ad8*/ MOV         R2, #8 
    /*00000adc*/ MOVW        R1, #18516                  @ 0x4854 
    /*00000ae0*/ BL          os05a_2l_write_register
    /*00000ae4*/ MOV         R0, R4 
    /*00000ae8*/ MOV         R2, #48                     @ 0x30 
    /*00000aec*/ MOVW        R1, #18517                  @ 0x4855 
    /*00000af0*/ BL          os05a_2l_write_register
    /*00000af4*/ MOV         R0, R4 
    /*00000af8*/ MOV         R2, #0 
    /*00000afc*/ MOV         R1, #18432                  @ 0x4800 
    /*00000b00*/ BL          os05a_2l_write_register
    /*00000b04*/ MOV         R0, R4 
    /*00000b08*/ MOV         R2, #11 
    /*00000b0c*/ MOVW        R1, #18487                  @ 0x4837 
    /*00000b10*/ BL          os05a_2l_write_register
    /*00000b14*/ MOV         R0, R4 
    /*00000b18*/ MOV         R2, #63                     @ 0x3f 
    /*00000b1c*/ MOVW        R1, #18506                  @ 0x484a 
    /*00000b20*/ BL          os05a_2l_write_register
    /*00000b24*/ MOV         R0, R4 
    /*00000b28*/ MOV         R2, #201                    @ 0xc9 
    /*00000b2c*/ MOV         R1, #20480                  @ 0x5000 
    /*00000b30*/ BL          os05a_2l_write_register
    /*00000b34*/ MOV         R0, R4 
    /*00000b38*/ MOV         R2, #67                     @ 0x43 
    /*00000b3c*/ MOVW        R1, #20481                  @ 0x5001 
    /*00000b40*/ BL          os05a_2l_write_register
    /*00000b44*/ MOV         R0, R4 
    /*00000b48*/ MOV         R2, #0 
    /*00000b4c*/ MOVW        R1, #20482                  @ 0x5002 
    /*00000b50*/ BL          os05a_2l_write_register
    /*00000b54*/ MOV         R0, R4 
    /*00000b58*/ MOV         R2, #3 
    /*00000b5c*/ MOVW        R1, #21009                  @ 0x5211 
    /*00000b60*/ BL          os05a_2l_write_register
    /*00000b64*/ MOV         R0, R4 
    /*00000b68*/ MOV         R2, #3 
    /*00000b6c*/ MOVW        R1, #21137                  @ 0x5291 
    /*00000b70*/ BL          os05a_2l_write_register
    /*00000b74*/ MOV         R0, R4 
    /*00000b78*/ MOV         R2, #15 
    /*00000b7c*/ MOVW        R1, #21005                  @ 0x520d 
    /*00000b80*/ BL          os05a_2l_write_register
    /*00000b84*/ MOV         R0, R4 
    /*00000b88*/ MOV         R2, #253                    @ 0xfd 
    /*00000b8c*/ MOVW        R1, #21006                  @ 0x520e 
    /*00000b90*/ BL          os05a_2l_write_register
    /*00000b94*/ MOV         R0, R4 
    /*00000b98*/ MOV         R2, #165                    @ 0xa5 
    /*00000b9c*/ MOVW        R1, #21007                  @ 0x520f 
    /*00000ba0*/ BL          os05a_2l_write_register
    /*00000ba4*/ MOV         R0, R4 
    /*00000ba8*/ MOV         R2, #165                    @ 0xa5 
    /*00000bac*/ MOVW        R1, #21008                  @ 0x5210 
    /*00000bb0*/ BL          os05a_2l_write_register
    /*00000bb4*/ MOV         R0, R4 
    /*00000bb8*/ MOV         R2, #15 
    /*00000bbc*/ MOVW        R1, #21133                  @ 0x528d 
    /*00000bc0*/ BL          os05a_2l_write_register
    /*00000bc4*/ MOV         R0, R4 
    /*00000bc8*/ MOV         R2, #253                    @ 0xfd 
    /*00000bcc*/ MOVW        R1, #21134                  @ 0x528e 
    /*00000bd0*/ BL          os05a_2l_write_register
    /*00000bd4*/ MOV         R0, R4 
    /*00000bd8*/ MOV         R2, #165                    @ 0xa5 
    /*00000bdc*/ MOVW        R1, #21135                  @ 0x528f 
    /*00000be0*/ BL          os05a_2l_write_register
    /*00000be4*/ MOV         R0, R4 
    /*00000be8*/ MOV         R2, #165                    @ 0xa5 
    /*00000bec*/ MOVW        R1, #21136                  @ 0x5290 
    /*00000bf0*/ BL          os05a_2l_write_register
    /*00000bf4*/ MOV         R0, R4 
    /*00000bf8*/ MOV         R2, #64                     @ 0x40 
    /*00000bfc*/ MOVW        R1, #20484                  @ 0x5004 
    /*00000c00*/ BL          os05a_2l_write_register
    /*00000c04*/ MOV         R0, R4 
    /*00000c08*/ MOV         R2, #0 
    /*00000c0c*/ MOVW        R1, #20485                  @ 0x5005 
    /*00000c10*/ BL          os05a_2l_write_register
    /*00000c14*/ MOV         R0, R4 
    /*00000c18*/ MOV         R2, #0 
    /*00000c1c*/ MOVW        R1, #20864                  @ 0x5180 
    /*00000c20*/ BL          os05a_2l_write_register
    /*00000c24*/ MOV         R0, R4 
    /*00000c28*/ MOV         R2, #16 
    /*00000c2c*/ MOVW        R1, #20865                  @ 0x5181 
    /*00000c30*/ BL          os05a_2l_write_register
    /*00000c34*/ MOV         R0, R4 
    /*00000c38*/ MOV         R2, #15 
    /*00000c3c*/ MOVW        R1, #20866                  @ 0x5182 
    /*00000c40*/ BL          os05a_2l_write_register
    /*00000c44*/ MOV         R0, R4 
    /*00000c48*/ MOV         R2, #255                    @ 0xff 
    /*00000c4c*/ MOVW        R1, #20867                  @ 0x5183 
    /*00000c50*/ BL          os05a_2l_write_register
    /*00000c54*/ MOV         R0, R4 
    /*00000c58*/ MOV         R2, #3 
    /*00000c5c*/ MOVW        R1, #22539                  @ 0x580b 
    /*00000c60*/ BL          os05a_2l_write_register
    /*00000c64*/ MOV         R0, R4 
    /*00000c68*/ MOV         R2, #3 
    /*00000c6c*/ MOV         R1, #19712                  @ 0x4d00 
    /*00000c70*/ BL          os05a_2l_write_register
    /*00000c74*/ MOV         R0, R4 
    /*00000c78*/ MOV         R2, #233                    @ 0xe9 
    /*00000c7c*/ MOVW        R1, #19713                  @ 0x4d01 
    /*00000c80*/ BL          os05a_2l_write_register
    /*00000c84*/ MOV         R0, R4 
    /*00000c88*/ MOV         R2, #186                    @ 0xba 
    /*00000c8c*/ MOVW        R1, #19714                  @ 0x4d02 
    /*00000c90*/ BL          os05a_2l_write_register
    /*00000c94*/ MOV         R0, R4 
    /*00000c98*/ MOV         R2, #102                    @ 0x66 
    /*00000c9c*/ MOVW        R1, #19715                  @ 0x4d03 
    /*00000ca0*/ BL          os05a_2l_write_register
    /*00000ca4*/ MOV         R0, R4 
    /*00000ca8*/ MOV         R2, #70                     @ 0x46 
    /*00000cac*/ MOVW        R1, #19716                  @ 0x4d04 
    /*00000cb0*/ BL          os05a_2l_write_register
    /*00000cb4*/ MOV         R0, R4 
    /*00000cb8*/ MOV         R2, #165                    @ 0xa5 
    /*00000cbc*/ MOVW        R1, #19717                  @ 0x4d05 
    /*00000cc0*/ BL          os05a_2l_write_register
    /*00000cc4*/ MOV         R0, R4 
    /*00000cc8*/ MOV         R2, #60                     @ 0x3c 
    /*00000ccc*/ MOVW        R1, #13827                  @ 0x3603 
    /*00000cd0*/ BL          os05a_2l_write_register
    /*00000cd4*/ MOV         R0, R4 
    /*00000cd8*/ MOV         R2, #38                     @ 0x26 
    /*00000cdc*/ MOVW        R1, #14083                  @ 0x3703 
    /*00000ce0*/ BL          os05a_2l_write_register
    /*00000ce4*/ MOV         R0, R4 
    /*00000ce8*/ MOV         R2, #73                     @ 0x49 
    /*00000cec*/ MOVW        R1, #14089                  @ 0x3709 
    /*00000cf0*/ BL          os05a_2l_write_register
    /*00000cf4*/ MOV         R0, R4 
    /*00000cf8*/ MOV         R2, #45                     @ 0x2d 
    /*00000cfc*/ MOVW        R1, #14088                  @ 0x3708 
    /*00000d00*/ BL          os05a_2l_write_register
    /*00000d04*/ MOV         R0, R4 
    /*00000d08*/ MOV         R2, #28 
    /*00000d0c*/ MOVW        R1, #14105                  @ 0x3719 
    /*00000d10*/ BL          os05a_2l_write_register
    /*00000d14*/ MOV         R0, R4 
    /*00000d18*/ MOV         R2, #6 
    /*00000d1c*/ MOVW        R1, #14106                  @ 0x371a 
    /*00000d20*/ BL          os05a_2l_write_register
    /*00000d24*/ MOV         R0, R4 
    /*00000d28*/ MOV         R2, #121                    @ 0x79 
    /*00000d2c*/ MOV         R1, #16384                  @ 0x4000 
    /*00000d30*/ BL          os05a_2l_write_register
    /*00000d34*/ MOV         R0, R4 
    /*00000d38*/ MOV         R2, #4 
    /*00000d3c*/ MOVW        R1, #14348                  @ 0x380c 
    /*00000d40*/ BL          os05a_2l_write_register
    /*00000d44*/ MOV         R0, R4 
    /*00000d48*/ MOV         R2, #16 
    /*00000d4c*/ MOVW        R1, #14349                  @ 0x380d 
    /*00000d50*/ BL          os05a_2l_write_register
    /*00000d54*/ MOV         R0, R4 
    /*00000d58*/ MOV         R2, #7 
    /*00000d5c*/ MOVW        R1, #14350                  @ 0x380e 
    /*00000d60*/ BL          os05a_2l_write_register
    /*00000d64*/ MOV         R0, R4 
    /*00000d68*/ MOV         R2, #205                    @ 0xcd 
    /*00000d6c*/ MOVW        R1, #14351                  @ 0x380f 
    /*00000d70*/ BL          os05a_2l_write_register
    /*00000d74*/ MOV         R0, R4 
    /*00000d78*/ MOV         R2, #6 
    /*00000d7c*/ MOVW        R1, #13569                  @ 0x3501 
    /*00000d80*/ BL          os05a_2l_write_register
    /*00000d84*/ MOV         R0, R4 
    /*00000d88*/ MOV         R2, #200                    @ 0xc8 
    /*00000d8c*/ MOVW        R1, #13570                  @ 0x3502 
    /*00000d90*/ BL          os05a_2l_write_register
    /*00000d94*/ MOV         R0, R4 
    /*00000d98*/ MOV         R2, #0 
    /*00000d9c*/ MOVW        R1, #13585                  @ 0x3511 
    /*00000da0*/ BL          os05a_2l_write_register
    /*00000da4*/ MOV         R2, #32 
    /*00000da8*/ MOVW        R1, #13586                  @ 0x3512 
    /*00000dac*/ MOV         R0, R4 
    /*00000db0*/ BL          os05a_2l_write_register
    /*00000db4*/ MOV         R0, R4 
    /*00000db8*/ BL          os05a_2l_default_reg_init
    /*00000dbc*/ MOV         R0, R4 
    /*00000dc0*/ MOV         R2, #1 
    /*00000dc4*/ MOV         R1, #256                    @ 0x100 
    /*00000dc8*/ BL          os05a_2l_write_register
    /*00000dcc*/ MOV         R0, R4 
    /*00000dd0*/ MOV         R2, #0 
    /*00000dd4*/ MOV         R1, #256                    @ 0x100 
    /*00000dd8*/ BL          os05a_2l_write_register
    /*00000ddc*/ MOV         R0, R4 
    /*00000de0*/ MOV         R2, #1 
    /*00000de4*/ MOV         R1, #256                    @ 0x100 
    /*00000de8*/ BL          os05a_2l_write_register
    /*00000dec*/ LDR         R0, WORD_0dfc               @ LDR         R0, [PC, #8]                @ 0x0000000000000dfc 
    /*00000df0*/ POP         {R4, LR} 
    /*00000df4*/ ADD         R0, PC, R0 
    os05a_2l_init_5M_2688x1944_10bit_wdr30_xdf8:
    /*00000df8*/ B           os05a_2l_init_5M_2688x1944_10bit_wdr30_xdf8@ puts [R_ARM_JUMP24: +0x0] 
    /*00000dfc*/ WORD_0dfc: .word 0x00000000
FUNC_END os05a_2l_init_5M_2688x1944_10bit_wdr30


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.os05a_2l_init_5M_2688x1944_12bit_linear30
@ Size: 0xde0
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN os05a_2l_init_5M_2688x1944_12bit_linear30
    /*00000000*/ PUSH        {R4, LR} 
    /*00000004*/ MOV         R2, #1 
    /*00000008*/ MOV         R4, R0 
    /*0000000c*/ MOV         R1, #17920                  @ 0x4600 
    /*00000010*/ BL          os05a_2l_write_register
    /*00000014*/ MOV         R0, R4 
    /*00000018*/ MOV         R2, #4 
    /*0000001c*/ MOVW        R1, #17921                  @ 0x4601 
    /*00000020*/ BL          os05a_2l_write_register
    /*00000024*/ MOV         R0, R4 
    /*00000028*/ MOV         R2, #0 
    /*0000002c*/ MOVW        R1, #17923                  @ 0x4603 
    /*00000030*/ BL          os05a_2l_write_register
    /*00000034*/ MOV         R0, R4 
    /*00000038*/ MOV         R2, #1 
    /*0000003c*/ MOVW        R1, #259                    @ 0x103 
    /*00000040*/ BL          os05a_2l_write_register
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ MOV         R2, #1 
    /*0000004c*/ MOVW        R1, #771                    @ 0x303 
    /*00000050*/ BL          os05a_2l_write_register
    /*00000054*/ MOV         R0, R4 
    /*00000058*/ MOV         R2, #50                     @ 0x32 
    /*0000005c*/ MOVW        R1, #773                    @ 0x305 
    /*00000060*/ BL          os05a_2l_write_register
    /*00000064*/ MOV         R0, R4 
    /*00000068*/ MOV         R2, #0 
    /*0000006c*/ MOVW        R1, #774                    @ 0x306 
    /*00000070*/ BL          os05a_2l_write_register
    /*00000074*/ MOV         R0, R4 
    /*00000078*/ MOV         R2, #0 
    /*0000007c*/ MOVW        R1, #775                    @ 0x307 
    /*00000080*/ BL          os05a_2l_write_register
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOV         R2, #3 
    /*0000008c*/ MOV         R1, #776                    @ 0x308 
    /*00000090*/ BL          os05a_2l_write_register
    /*00000094*/ MOV         R0, R4 
    /*00000098*/ MOV         R2, #4 
    /*0000009c*/ MOVW        R1, #777                    @ 0x309 
    /*000000a0*/ BL          os05a_2l_write_register
    /*000000a4*/ MOV         R0, R4 
    /*000000a8*/ MOV         R2, #0 
    /*000000ac*/ MOVW        R1, #810                    @ 0x32a 
    /*000000b0*/ BL          os05a_2l_write_register
    /*000000b4*/ MOV         R0, R4 
    /*000000b8*/ MOV         R2, #9 
    /*000000bc*/ MOVW        R1, #798                    @ 0x31e 
    /*000000c0*/ BL          os05a_2l_write_register
    /*000000c4*/ MOV         R0, R4 
    /*000000c8*/ MOV         R2, #72                     @ 0x48 
    /*000000cc*/ MOVW        R1, #805                    @ 0x325 
    /*000000d0*/ BL          os05a_2l_write_register
    /*000000d4*/ MOV         R0, R4 
    /*000000d8*/ MOV         R2, #7 
    /*000000dc*/ MOV         R1, #808                    @ 0x328 
    /*000000e0*/ BL          os05a_2l_write_register
    /*000000e4*/ MOV         R0, R4 
    /*000000e8*/ MOV         R2, #17 
    /*000000ec*/ MOVW        R1, #12301                  @ 0x300d 
    /*000000f0*/ BL          os05a_2l_write_register
    /*000000f4*/ MOV         R0, R4 
    /*000000f8*/ MOV         R2, #17 
    /*000000fc*/ MOVW        R1, #12302                  @ 0x300e 
    /*00000100*/ BL          os05a_2l_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #17 
    /*0000010c*/ MOVW        R1, #12303                  @ 0x300f 
    /*00000110*/ BL          os05a_2l_write_register
    /*00000114*/ MOV         R0, R4 
    /*00000118*/ MOV         R2, #1 
    /*0000011c*/ MOVW        R1, #12304                  @ 0x3010 
    /*00000120*/ BL          os05a_2l_write_register
    /*00000124*/ MOV         R0, R4 
    /*00000128*/ MOV         R2, #33                     @ 0x21 
    /*0000012c*/ MOVW        R1, #12306                  @ 0x3012 
    /*00000130*/ BL          os05a_2l_write_register
    /*00000134*/ MOV         R0, R4 
    /*00000138*/ MOV         R2, #240                    @ 0xf0 
    /*0000013c*/ MOVW        R1, #12310                  @ 0x3016 
    /*00000140*/ BL          os05a_2l_write_register
    /*00000144*/ MOV         R0, R4 
    /*00000148*/ MOV         R2, #240                    @ 0xf0 
    /*0000014c*/ MOVW        R1, #12312                  @ 0x3018 
    /*00000150*/ BL          os05a_2l_write_register
    /*00000154*/ MOV         R0, R4 
    /*00000158*/ MOV         R2, #240                    @ 0xf0 
    /*0000015c*/ MOVW        R1, #12328                  @ 0x3028 
    /*00000160*/ BL          os05a_2l_write_register
    /*00000164*/ MOV         R0, R4 
    /*00000168*/ MOV         R2, #152                    @ 0x98 
    /*0000016c*/ MOVW        R1, #12318                  @ 0x301e 
    /*00000170*/ BL          os05a_2l_write_register
    /*00000174*/ MOV         R0, R4 
    /*00000178*/ MOV         R2, #4 
    /*0000017c*/ MOVW        R1, #12304                  @ 0x3010 
    /*00000180*/ BL          os05a_2l_write_register
    /*00000184*/ MOV         R0, R4 
    /*00000188*/ MOV         R2, #6 
    /*0000018c*/ MOVW        R1, #12305                  @ 0x3011 
    /*00000190*/ BL          os05a_2l_write_register
    /*00000194*/ MOV         R0, R4 
    /*00000198*/ MOV         R2, #169                    @ 0xa9 
    /*0000019c*/ MOVW        R1, #12337                  @ 0x3031 
    /*000001a0*/ BL          os05a_2l_write_register
    /*000001a4*/ MOV         R0, R4 
    /*000001a8*/ MOV         R2, #72                     @ 0x48 
    /*000001ac*/ MOVW        R1, #12547                  @ 0x3103 
    /*000001b0*/ BL          os05a_2l_write_register
    /*000001b4*/ MOV         R0, R4 
    /*000001b8*/ MOV         R2, #1 
    /*000001bc*/ MOVW        R1, #12548                  @ 0x3104 
    /*000001c0*/ BL          os05a_2l_write_register
    /*000001c4*/ MOV         R0, R4 
    /*000001c8*/ MOV         R2, #16 
    /*000001cc*/ MOVW        R1, #12550                  @ 0x3106 
    /*000001d0*/ BL          os05a_2l_write_register
    /*000001d4*/ MOV         R0, R4 
    /*000001d8*/ MOV         R2, #7 
    /*000001dc*/ MOVW        R1, #13569                  @ 0x3501 
    /*000001e0*/ BL          os05a_2l_write_register
    /*000001e4*/ MOV         R0, R4 
    /*000001e8*/ MOV         R2, #223                    @ 0xdf 
    /*000001ec*/ MOVW        R1, #13570                  @ 0x3502 
    /*000001f0*/ BL          os05a_2l_write_register
    /*000001f4*/ MOV         R0, R4 
    /*000001f8*/ MOV         R2, #131                    @ 0x83 
    /*000001fc*/ MOVW        R1, #13573                  @ 0x3505 
    /*00000200*/ BL          os05a_2l_write_register
    /*00000204*/ MOV         R0, R4 
    /*00000208*/ MOV         R2, #0 
    /*0000020c*/ MOVW        R1, #13576                  @ 0x3508 
    /*00000210*/ BL          os05a_2l_write_register
    /*00000214*/ MOV         R0, R4 
    /*00000218*/ MOV         R2, #128                    @ 0x80 
    /*0000021c*/ MOVW        R1, #13577                  @ 0x3509 
    /*00000220*/ BL          os05a_2l_write_register
    /*00000224*/ MOV         R0, R4 
    /*00000228*/ MOV         R2, #4 
    /*0000022c*/ MOVW        R1, #13578                  @ 0x350a 
    /*00000230*/ BL          os05a_2l_write_register
    /*00000234*/ MOV         R0, R4 
    /*00000238*/ MOV         R2, #0 
    /*0000023c*/ MOVW        R1, #13579                  @ 0x350b 
    /*00000240*/ BL          os05a_2l_write_register
    /*00000244*/ MOV         R0, R4 
    /*00000248*/ MOV         R2, #0 
    /*0000024c*/ MOVW        R1, #13580                  @ 0x350c 
    /*00000250*/ BL          os05a_2l_write_register
    /*00000254*/ MOV         R0, R4 
    /*00000258*/ MOV         R2, #128                    @ 0x80 
    /*0000025c*/ MOVW        R1, #13581                  @ 0x350d 
    /*00000260*/ BL          os05a_2l_write_register
    /*00000264*/ MOV         R0, R4 
    /*00000268*/ MOV         R2, #4 
    /*0000026c*/ MOVW        R1, #13582                  @ 0x350e 
    /*00000270*/ BL          os05a_2l_write_register
    /*00000274*/ MOV         R0, R4 
    /*00000278*/ MOV         R2, #0 
    /*0000027c*/ MOVW        R1, #13583                  @ 0x350f 
    /*00000280*/ BL          os05a_2l_write_register
    /*00000284*/ MOV         R0, R4 
    /*00000288*/ MOV         R2, #0 
    /*0000028c*/ MOV         R1, #13824                  @ 0x3600 
    /*00000290*/ BL          os05a_2l_write_register
    /*00000294*/ MOV         R0, R4 
    /*00000298*/ MOV         R2, #255                    @ 0xff 
    /*0000029c*/ MOVW        R1, #13862                  @ 0x3626 
    /*000002a0*/ BL          os05a_2l_write_register
    /*000002a4*/ MOV         R0, R4 
    /*000002a8*/ MOV         R2, #80                     @ 0x50 
    /*000002ac*/ MOVW        R1, #13829                  @ 0x3605 
    /*000002b0*/ BL          os05a_2l_write_register
    /*000002b4*/ MOV         R0, R4 
    /*000002b8*/ MOV         R2, #181                    @ 0xb5 
    /*000002bc*/ MOVW        R1, #13833                  @ 0x3609 
    /*000002c0*/ BL          os05a_2l_write_register
    /*000002c4*/ MOV         R0, R4 
    /*000002c8*/ MOV         R2, #105                    @ 0x69 
    /*000002cc*/ MOVW        R1, #13840                  @ 0x3610 
    /*000002d0*/ BL          os05a_2l_write_register
    /*000002d4*/ MOV         R0, R4 
    /*000002d8*/ MOV         R2, #1 
    /*000002dc*/ MOVW        R1, #13836                  @ 0x360c 
    /*000002e0*/ BL          os05a_2l_write_register
    /*000002e4*/ MOV         R0, R4 
    /*000002e8*/ MOV         R2, #164                    @ 0xa4 
    /*000002ec*/ MOVW        R1, #13864                  @ 0x3628 
    /*000002f0*/ BL          os05a_2l_write_register
    /*000002f4*/ MOV         R0, R4 
    /*000002f8*/ MOV         R2, #106                    @ 0x6a 
    /*000002fc*/ MOVW        R1, #13865                  @ 0x3629 
    /*00000300*/ BL          os05a_2l_write_register
    /*00000304*/ MOV         R0, R4 
    /*00000308*/ MOV         R2, #16 
    /*0000030c*/ MOVW        R1, #13869                  @ 0x362d 
    /*00000310*/ BL          os05a_2l_write_register
    /*00000314*/ MOV         R0, R4 
    /*00000318*/ MOV         R2, #67                     @ 0x43 
    /*0000031c*/ MOVW        R1, #13920                  @ 0x3660 
    /*00000320*/ BL          os05a_2l_write_register
    /*00000324*/ MOV         R0, R4 
    /*00000328*/ MOV         R2, #6 
    /*0000032c*/ MOVW        R1, #13921                  @ 0x3661 
    /*00000330*/ BL          os05a_2l_write_register
    /*00000334*/ MOV         R0, R4 
    /*00000338*/ MOV         R2, #0 
    /*0000033c*/ MOVW        R1, #13922                  @ 0x3662 
    /*00000340*/ BL          os05a_2l_write_register
    /*00000344*/ MOV         R0, R4 
    /*00000348*/ MOV         R2, #40                     @ 0x28 
    /*0000034c*/ MOVW        R1, #13923                  @ 0x3663 
    /*00000350*/ BL          os05a_2l_write_register
    /*00000354*/ MOV         R0, R4 
    /*00000358*/ MOV         R2, #13 
    /*0000035c*/ MOVW        R1, #13924                  @ 0x3664 
    /*00000360*/ BL          os05a_2l_write_register
    /*00000364*/ MOV         R0, R4 
    /*00000368*/ MOV         R2, #56                     @ 0x38 
    /*0000036c*/ MOVW        R1, #13930                  @ 0x366a 
    /*00000370*/ BL          os05a_2l_write_register
    /*00000374*/ MOV         R0, R4 
    /*00000378*/ MOV         R2, #160                    @ 0xa0 
    /*0000037c*/ MOVW        R1, #13931                  @ 0x366b 
    /*00000380*/ BL          os05a_2l_write_register
    /*00000384*/ MOV         R0, R4 
    /*00000388*/ MOV         R2, #0 
    /*0000038c*/ MOVW        R1, #13933                  @ 0x366d 
    /*00000390*/ BL          os05a_2l_write_register
    /*00000394*/ MOV         R0, R4 
    /*00000398*/ MOV         R2, #0 
    /*0000039c*/ MOVW        R1, #13934                  @ 0x366e 
    /*000003a0*/ BL          os05a_2l_write_register
    /*000003a4*/ MOV         R0, R4 
    /*000003a8*/ MOV         R2, #0 
    /*000003ac*/ MOV         R1, #13952                  @ 0x3680 
    /*000003b0*/ BL          os05a_2l_write_register
    /*000003b4*/ MOV         R0, R4 
    /*000003b8*/ MOV         R2, #129                    @ 0x81 
    /*000003bc*/ MOVW        R1, #13857                  @ 0x3621 
    /*000003c0*/ BL          os05a_2l_write_register
    /*000003c4*/ MOV         R0, R4 
    /*000003c8*/ MOV         R2, #49                     @ 0x31 
    /*000003cc*/ MOVW        R1, #13876                  @ 0x3634 
    /*000003d0*/ BL          os05a_2l_write_register
    /*000003d4*/ MOV         R0, R4 
    /*000003d8*/ MOV         R2, #0 
    /*000003dc*/ MOVW        R1, #13856                  @ 0x3620 
    /*000003e0*/ BL          os05a_2l_write_register
    /*000003e4*/ MOV         R0, R4 
    /*000003e8*/ MOV         R2, #0 
    /*000003ec*/ MOVW        R1, #13858                  @ 0x3622 
    /*000003f0*/ BL          os05a_2l_write_register
    /*000003f4*/ MOV         R0, R4 
    /*000003f8*/ MOV         R2, #208                    @ 0xd0 
    /*000003fc*/ MOVW        R1, #13866                  @ 0x362a 
    /*00000400*/ BL          os05a_2l_write_register
    /*00000404*/ MOV         R0, R4 
    /*00000408*/ MOV         R2, #140                    @ 0x8c 
    /*0000040c*/ MOVW        R1, #13870                  @ 0x362e 
    /*00000410*/ BL          os05a_2l_write_register
    /*00000414*/ MOV         R0, R4 
    /*00000418*/ MOV         R2, #152                    @ 0x98 
    /*0000041c*/ MOVW        R1, #13871                  @ 0x362f 
    /*00000420*/ BL          os05a_2l_write_register
    /*00000424*/ MOV         R0, R4 
    /*00000428*/ MOV         R2, #176                    @ 0xb0 
    /*0000042c*/ MOVW        R1, #13872                  @ 0x3630 
    /*00000430*/ BL          os05a_2l_write_register
    /*00000434*/ MOV         R0, R4 
    /*00000438*/ MOV         R2, #215                    @ 0xd7 
    /*0000043c*/ MOVW        R1, #13873                  @ 0x3631 
    /*00000440*/ BL          os05a_2l_write_register
    /*00000444*/ MOV         R0, R4 
    /*00000448*/ MOV         R2, #15 
    /*0000044c*/ MOVW        R1, #14081                  @ 0x3701 
    /*00000450*/ BL          os05a_2l_write_register
    /*00000454*/ MOV         R0, R4 
    /*00000458*/ MOV         R2, #2 
    /*0000045c*/ MOVW        R1, #14135                  @ 0x3737 
    /*00000460*/ BL          os05a_2l_write_register
    /*00000464*/ MOV         R0, R4 
    /*00000468*/ MOV         R2, #4 
    /*0000046c*/ MOVW        R1, #14145                  @ 0x3741 
    /*00000470*/ BL          os05a_2l_write_register
    /*00000474*/ MOV         R0, R4 
    /*00000478*/ MOV         R2, #15 
    /*0000047c*/ MOVW        R1, #14140                  @ 0x373c 
    /*00000480*/ BL          os05a_2l_write_register
    /*00000484*/ MOV         R0, R4 
    /*00000488*/ MOV         R2, #2 
    /*0000048c*/ MOVW        R1, #14139                  @ 0x373b 
    /*00000490*/ BL          os05a_2l_write_register
    /*00000494*/ MOV         R0, R4 
    /*00000498*/ MOV         R2, #0 
    /*0000049c*/ MOVW        R1, #14085                  @ 0x3705 
    /*000004a0*/ BL          os05a_2l_write_register
    /*000004a4*/ MOV         R0, R4 
    /*000004a8*/ MOV         R2, #80                     @ 0x50 
    /*000004ac*/ MOVW        R1, #14086                  @ 0x3706 
    /*000004b0*/ BL          os05a_2l_write_register
    /*000004b4*/ MOV         R0, R4 
    /*000004b8*/ MOV         R2, #0 
    /*000004bc*/ MOVW        R1, #14090                  @ 0x370a 
    /*000004c0*/ BL          os05a_2l_write_register
    /*000004c4*/ MOV         R0, R4 
    /*000004c8*/ MOV         R2, #228                    @ 0xe4 
    /*000004cc*/ MOVW        R1, #14091                  @ 0x370b 
    /*000004d0*/ BL          os05a_2l_write_register
    /*000004d4*/ MOV         R0, R4 
    /*000004d8*/ MOV         R2, #74                     @ 0x4a 
    /*000004dc*/ MOVW        R1, #14089                  @ 0x3709 
    /*000004e0*/ BL          os05a_2l_write_register
    /*000004e4*/ MOV         R0, R4 
    /*000004e8*/ MOV         R2, #33                     @ 0x21 
    /*000004ec*/ MOVW        R1, #14100                  @ 0x3714 
    /*000004f0*/ BL          os05a_2l_write_register
    /*000004f4*/ MOV         R0, R4 
    /*000004f8*/ MOV         R2, #0 
    /*000004fc*/ MOVW        R1, #14108                  @ 0x371c 
    /*00000500*/ BL          os05a_2l_write_register
    /*00000504*/ MOV         R0, R4 
    /*00000508*/ MOV         R2, #8 
    /*0000050c*/ MOVW        R1, #14109                  @ 0x371d 
    /*00000510*/ BL          os05a_2l_write_register
    /*00000514*/ MOV         R0, R4 
    /*00000518*/ MOV         R2, #11 
    /*0000051c*/ MOVW        R1, #14174                  @ 0x375e 
    /*00000520*/ BL          os05a_2l_write_register
    /*00000524*/ MOV         R0, R4 
    /*00000528*/ MOV         R2, #16 
    /*0000052c*/ MOVW        R1, #14198                  @ 0x3776 
    /*00000530*/ BL          os05a_2l_write_register
    /*00000534*/ MOV         R0, R4 
    /*00000538*/ MOV         R2, #2 
    /*0000053c*/ MOVW        R1, #14209                  @ 0x3781 
    /*00000540*/ BL          os05a_2l_write_register
    /*00000544*/ MOV         R0, R4 
    /*00000548*/ MOV         R2, #4 
    /*0000054c*/ MOVW        R1, #14210                  @ 0x3782 
    /*00000550*/ BL          os05a_2l_write_register
    /*00000554*/ MOV         R0, R4 
    /*00000558*/ MOV         R2, #2 
    /*0000055c*/ MOVW        R1, #14211                  @ 0x3783 
    /*00000560*/ BL          os05a_2l_write_register
    /*00000564*/ MOV         R0, R4 
    /*00000568*/ MOV         R2, #8 
    /*0000056c*/ MOVW        R1, #14212                  @ 0x3784 
    /*00000570*/ BL          os05a_2l_write_register
    /*00000574*/ MOV         R0, R4 
    /*00000578*/ MOV         R2, #8 
    /*0000057c*/ MOVW        R1, #14213                  @ 0x3785 
    /*00000580*/ BL          os05a_2l_write_register
    /*00000584*/ MOV         R0, R4 
    /*00000588*/ MOV         R2, #1 
    /*0000058c*/ MOVW        R1, #14216                  @ 0x3788 
    /*00000590*/ BL          os05a_2l_write_register
    /*00000594*/ MOV         R0, R4 
    /*00000598*/ MOV         R2, #1 
    /*0000059c*/ MOVW        R1, #14217                  @ 0x3789 
    /*000005a0*/ BL          os05a_2l_write_register
    /*000005a4*/ MOV         R0, R4 
    /*000005a8*/ MOV         R2, #4 
    /*000005ac*/ MOVW        R1, #14231                  @ 0x3797 
    /*000005b0*/ BL          os05a_2l_write_register
    /*000005b4*/ MOV         R0, R4 
    /*000005b8*/ MOV         R2, #2 
    /*000005bc*/ MOVW        R1, #14177                  @ 0x3761 
    /*000005c0*/ BL          os05a_2l_write_register
    /*000005c4*/ MOV         R0, R4 
    /*000005c8*/ MOV         R2, #13 
    /*000005cc*/ MOVW        R1, #14178                  @ 0x3762 
    /*000005d0*/ BL          os05a_2l_write_register
    /*000005d4*/ MOV         R0, R4 
    /*000005d8*/ MOV         R2, #0 
    /*000005dc*/ MOV         R1, #14336                  @ 0x3800 
    /*000005e0*/ BL          os05a_2l_write_register
    /*000005e4*/ MOV         R0, R4 
    /*000005e8*/ MOV         R2, #0 
    /*000005ec*/ MOVW        R1, #14337                  @ 0x3801 
    /*000005f0*/ BL          os05a_2l_write_register
    /*000005f4*/ MOV         R0, R4 
    /*000005f8*/ MOV         R2, #0 
    /*000005fc*/ MOVW        R1, #14338                  @ 0x3802 
    /*00000600*/ BL          os05a_2l_write_register
    /*00000604*/ MOV         R0, R4 
    /*00000608*/ MOV         R2, #12 
    /*0000060c*/ MOVW        R1, #14339                  @ 0x3803 
    /*00000610*/ BL          os05a_2l_write_register
    /*00000614*/ MOV         R0, R4 
    /*00000618*/ MOV         R2, #14 
    /*0000061c*/ MOVW        R1, #14340                  @ 0x3804 
    /*00000620*/ BL          os05a_2l_write_register
    /*00000624*/ MOV         R0, R4 
    /*00000628*/ MOV         R2, #255                    @ 0xff 
    /*0000062c*/ MOVW        R1, #14341                  @ 0x3805 
    /*00000630*/ BL          os05a_2l_write_register
    /*00000634*/ MOV         R0, R4 
    /*00000638*/ MOV         R2, #8 
    /*0000063c*/ MOVW        R1, #14342                  @ 0x3806 
    /*00000640*/ BL          os05a_2l_write_register
    /*00000644*/ MOV         R0, R4 
    /*00000648*/ MOV         R2, #111                    @ 0x6f 
    /*0000064c*/ MOVW        R1, #14343                  @ 0x3807 
    /*00000650*/ BL          os05a_2l_write_register
    /*00000654*/ MOV         R0, R4 
    /*00000658*/ MOV         R2, #10 
    /*0000065c*/ MOVW        R1, #14344                  @ 0x3808 
    /*00000660*/ BL          os05a_2l_write_register
    /*00000664*/ MOV         R0, R4 
    /*00000668*/ MOV         R2, #32 
    /*0000066c*/ MOVW        R1, #14345                  @ 0x3809 
    /*00000670*/ BL          os05a_2l_write_register
    /*00000674*/ MOV         R0, R4 
    /*00000678*/ MOV         R2, #7 
    /*0000067c*/ MOVW        R1, #14346                  @ 0x380a 
    /*00000680*/ BL          os05a_2l_write_register
    /*00000684*/ MOV         R0, R4 
    /*00000688*/ MOV         R2, #152                    @ 0x98 
    /*0000068c*/ MOVW        R1, #14347                  @ 0x380b 
    /*00000690*/ BL          os05a_2l_write_register
    /*00000694*/ MOV         R0, R4 
    /*00000698*/ MOV         R2, #8 
    /*0000069c*/ MOVW        R1, #14348                  @ 0x380c 
    /*000006a0*/ BL          os05a_2l_write_register
    /*000006a4*/ MOV         R0, R4 
    /*000006a8*/ MOV         R2, #0 
    /*000006ac*/ MOVW        R1, #14349                  @ 0x380d 
    /*000006b0*/ BL          os05a_2l_write_register
    /*000006b4*/ MOV         R0, R4 
    /*000006b8*/ MOV         R2, #8 
    /*000006bc*/ MOVW        R1, #14350                  @ 0x380e 
    /*000006c0*/ BL          os05a_2l_write_register
    /*000006c4*/ MOV         R0, R4 
    /*000006c8*/ MOV         R2, #0 
    /*000006cc*/ MOVW        R1, #14351                  @ 0x380f 
    /*000006d0*/ BL          os05a_2l_write_register
    /*000006d4*/ MOV         R0, R4 
    /*000006d8*/ MOV         R2, #4 
    /*000006dc*/ MOVW        R1, #14355                  @ 0x3813 
    /*000006e0*/ BL          os05a_2l_write_register
    /*000006e4*/ MOV         R0, R4 
    /*000006e8*/ MOV         R2, #1 
    /*000006ec*/ MOVW        R1, #14356                  @ 0x3814 
    /*000006f0*/ BL          os05a_2l_write_register
    /*000006f4*/ MOV         R0, R4 
    /*000006f8*/ MOV         R2, #1 
    /*000006fc*/ MOVW        R1, #14357                  @ 0x3815 
    /*00000700*/ BL          os05a_2l_write_register
    /*00000704*/ MOV         R0, R4 
    /*00000708*/ MOV         R2, #1 
    /*0000070c*/ MOVW        R1, #14358                  @ 0x3816 
    /*00000710*/ BL          os05a_2l_write_register
    /*00000714*/ MOV         R0, R4 
    /*00000718*/ MOV         R2, #1 
    /*0000071c*/ MOVW        R1, #14359                  @ 0x3817 
    /*00000720*/ BL          os05a_2l_write_register
    /*00000724*/ MOV         R0, R4 
    /*00000728*/ MOV         R2, #0 
    /*0000072c*/ MOVW        R1, #14364                  @ 0x381c 
    /*00000730*/ BL          os05a_2l_write_register
    /*00000734*/ MOV         R0, R4 
    /*00000738*/ MOV         R2, #0 
    /*0000073c*/ MOVW        R1, #14368                  @ 0x3820 
    /*00000740*/ BL          os05a_2l_write_register
    /*00000744*/ MOV         R0, R4 
    /*00000748*/ MOV         R2, #4 
    /*0000074c*/ MOVW        R1, #14369                  @ 0x3821 
    /*00000750*/ BL          os05a_2l_write_register
    /*00000754*/ MOV         R0, R4 
    /*00000758*/ MOV         R2, #24 
    /*0000075c*/ MOVW        R1, #14371                  @ 0x3823 
    /*00000760*/ BL          os05a_2l_write_register
    /*00000764*/ MOV         R0, R4 
    /*00000768*/ MOV         R2, #0 
    /*0000076c*/ MOVW        R1, #14374                  @ 0x3826 
    /*00000770*/ BL          os05a_2l_write_register
    /*00000774*/ MOV         R0, R4 
    /*00000778*/ MOV         R2, #1 
    /*0000077c*/ MOVW        R1, #14375                  @ 0x3827 
    /*00000780*/ BL          os05a_2l_write_register
    /*00000784*/ MOV         R0, R4 
    /*00000788*/ MOV         R2, #2 
    /*0000078c*/ MOVW        R1, #14386                  @ 0x3832 
    /*00000790*/ BL          os05a_2l_write_register
    /*00000794*/ MOV         R0, R4 
    /*00000798*/ MOV         R2, #72                     @ 0x48 
    /*0000079c*/ MOVW        R1, #14396                  @ 0x383c 
    /*000007a0*/ BL          os05a_2l_write_register
    /*000007a4*/ MOV         R0, R4 
    /*000007a8*/ MOV         R2, #255                    @ 0xff 
    /*000007ac*/ MOVW        R1, #14397                  @ 0x383d 
    /*000007b0*/ BL          os05a_2l_write_register
    /*000007b4*/ MOV         R0, R4 
    /*000007b8*/ MOV         R2, #32 
    /*000007bc*/ MOVW        R1, #14403                  @ 0x3843 
    /*000007c0*/ BL          os05a_2l_write_register
    /*000007c4*/ MOV         R0, R4 
    /*000007c8*/ MOV         R2, #8 
    /*000007cc*/ MOVW        R1, #14381                  @ 0x382d 
    /*000007d0*/ BL          os05a_2l_write_register
    /*000007d4*/ MOV         R0, R4 
    /*000007d8*/ MOV         R2, #11 
    /*000007dc*/ MOVW        R1, #15749                  @ 0x3d85 
    /*000007e0*/ BL          os05a_2l_write_register
    /*000007e4*/ MOV         R0, R4 
    /*000007e8*/ MOV         R2, #64                     @ 0x40 
    /*000007ec*/ MOVW        R1, #15748                  @ 0x3d84 
    /*000007f0*/ BL          os05a_2l_write_register
    /*000007f4*/ MOV         R0, R4 
    /*000007f8*/ MOV         R2, #99                     @ 0x63 
    /*000007fc*/ MOVW        R1, #15756                  @ 0x3d8c 
    /*00000800*/ BL          os05a_2l_write_register
    /*00000804*/ MOV         R0, R4 
    /*00000808*/ MOV         R2, #0 
    /*0000080c*/ MOVW        R1, #15757                  @ 0x3d8d 
    /*00000810*/ BL          os05a_2l_write_register
    /*00000814*/ MOV         R0, R4 
    /*00000818*/ MOV         R2, #120                    @ 0x78 
    /*0000081c*/ MOV         R1, #16384                  @ 0x4000 
    /*00000820*/ BL          os05a_2l_write_register
    /*00000824*/ MOV         R0, R4 
    /*00000828*/ MOV         R2, #43                     @ 0x2b 
    /*0000082c*/ MOVW        R1, #16385                  @ 0x4001 
    /*00000830*/ BL          os05a_2l_write_register
    /*00000834*/ MOV         R0, R4 
    /*00000838*/ MOV         R2, #64                     @ 0x40 
    /*0000083c*/ MOVW        R1, #16389                  @ 0x4005 
    /*00000840*/ BL          os05a_2l_write_register
    /*00000844*/ MOV         R0, R4 
    /*00000848*/ MOV         R2, #47                     @ 0x2f 
    /*0000084c*/ MOVW        R1, #16424                  @ 0x4028 
    /*00000850*/ BL          os05a_2l_write_register
    /*00000854*/ MOV         R0, R4 
    /*00000858*/ MOV         R2, #1 
    /*0000085c*/ MOVW        R1, #16394                  @ 0x400a 
    /*00000860*/ BL          os05a_2l_write_register
    /*00000864*/ MOV         R0, R4 
    /*00000868*/ MOV         R2, #18 
    /*0000086c*/ MOVW        R1, #16400                  @ 0x4010 
    /*00000870*/ BL          os05a_2l_write_register
    /*00000874*/ MOV         R0, R4 
    /*00000878*/ MOV         R2, #2 
    /*0000087c*/ MOVW        R1, #16392                  @ 0x4008 
    /*00000880*/ BL          os05a_2l_write_register
    /*00000884*/ MOV         R0, R4 
    /*00000888*/ MOV         R2, #13 
    /*0000088c*/ MOVW        R1, #16393                  @ 0x4009 
    /*00000890*/ BL          os05a_2l_write_register
    /*00000894*/ MOV         R0, R4 
    /*00000898*/ MOV         R2, #88                     @ 0x58 
    /*0000089c*/ MOVW        R1, #16410                  @ 0x401a 
    /*000008a0*/ BL          os05a_2l_write_register
    /*000008a4*/ MOV         R0, R4 
    /*000008a8*/ MOV         R2, #0 
    /*000008ac*/ MOVW        R1, #16464                  @ 0x4050 
    /*000008b0*/ BL          os05a_2l_write_register
    /*000008b4*/ MOV         R0, R4 
    /*000008b8*/ MOV         R2, #1 
    /*000008bc*/ MOVW        R1, #16465                  @ 0x4051 
    /*000008c0*/ BL          os05a_2l_write_register
    /*000008c4*/ MOV         R0, R4 
    /*000008c8*/ MOV         R2, #0 
    /*000008cc*/ MOVW        R1, #16466                  @ 0x4052 
    /*000008d0*/ BL          os05a_2l_write_register
    /*000008d4*/ MOV         R0, R4 
    /*000008d8*/ MOV         R2, #128                    @ 0x80 
    /*000008dc*/ MOVW        R1, #16467                  @ 0x4053 
    /*000008e0*/ BL          os05a_2l_write_register
    /*000008e4*/ MOV         R0, R4 
    /*000008e8*/ MOV         R2, #0 
    /*000008ec*/ MOVW        R1, #16468                  @ 0x4054 
    /*000008f0*/ BL          os05a_2l_write_register
    /*000008f4*/ MOV         R0, R4 
    /*000008f8*/ MOV         R2, #128                    @ 0x80 
    /*000008fc*/ MOVW        R1, #16469                  @ 0x4055 
    /*00000900*/ BL          os05a_2l_write_register
    /*00000904*/ MOV         R0, R4 
    /*00000908*/ MOV         R2, #0 
    /*0000090c*/ MOVW        R1, #16470                  @ 0x4056 
    /*00000910*/ BL          os05a_2l_write_register
    /*00000914*/ MOV         R0, R4 
    /*00000918*/ MOV         R2, #128                    @ 0x80 
    /*0000091c*/ MOVW        R1, #16471                  @ 0x4057 
    /*00000920*/ BL          os05a_2l_write_register
    /*00000924*/ MOV         R0, R4 
    /*00000928*/ MOV         R2, #0 
    /*0000092c*/ MOVW        R1, #16472                  @ 0x4058 
    /*00000930*/ BL          os05a_2l_write_register
    /*00000934*/ MOV         R0, R4 
    /*00000938*/ MOV         R2, #128                    @ 0x80 
    /*0000093c*/ MOVW        R1, #16473                  @ 0x4059 
    /*00000940*/ BL          os05a_2l_write_register
    /*00000944*/ MOV         R0, R4 
    /*00000948*/ MOV         R2, #255                    @ 0xff 
    /*0000094c*/ MOVW        R1, #17163                  @ 0x430b 
    /*00000950*/ BL          os05a_2l_write_register
    /*00000954*/ MOV         R0, R4 
    /*00000958*/ MOV         R2, #255                    @ 0xff 
    /*0000095c*/ MOVW        R1, #17164                  @ 0x430c 
    /*00000960*/ BL          os05a_2l_write_register
    /*00000964*/ MOV         R0, R4 
    /*00000968*/ MOV         R2, #0 
    /*0000096c*/ MOVW        R1, #17165                  @ 0x430d 
    /*00000970*/ BL          os05a_2l_write_register
    /*00000974*/ MOV         R0, R4 
    /*00000978*/ MOV         R2, #0 
    /*0000097c*/ MOVW        R1, #17166                  @ 0x430e 
    /*00000980*/ BL          os05a_2l_write_register
    /*00000984*/ MOV         R0, R4 
    /*00000988*/ MOV         R2, #24 
    /*0000098c*/ MOVW        R1, #17665                  @ 0x4501 
    /*00000990*/ BL          os05a_2l_write_register
    /*00000994*/ MOV         R0, R4 
    /*00000998*/ MOV         R2, #0 
    /*0000099c*/ MOVW        R1, #17666                  @ 0x4502 
    /*000009a0*/ BL          os05a_2l_write_register
    /*000009a4*/ MOV         R0, R4 
    /*000009a8*/ MOV         R2, #0 
    /*000009ac*/ MOVW        R1, #17987                  @ 0x4643 
    /*000009b0*/ BL          os05a_2l_write_register
    /*000009b4*/ MOV         R0, R4 
    /*000009b8*/ MOV         R2, #1 
    /*000009bc*/ MOVW        R1, #17984                  @ 0x4640 
    /*000009c0*/ BL          os05a_2l_write_register
    /*000009c4*/ MOV         R0, R4 
    /*000009c8*/ MOV         R2, #4 
    /*000009cc*/ MOVW        R1, #17985                  @ 0x4641 
    /*000009d0*/ BL          os05a_2l_write_register
    /*000009d4*/ MOV         R0, R4 
    /*000009d8*/ MOV         R2, #0 
    /*000009dc*/ MOVW        R1, #18446                  @ 0x480e 
    /*000009e0*/ BL          os05a_2l_write_register
    /*000009e4*/ MOV         R0, R4 
    /*000009e8*/ MOV         R2, #0 
    /*000009ec*/ MOVW        R1, #18451                  @ 0x4813 
    /*000009f0*/ BL          os05a_2l_write_register
    /*000009f4*/ MOV         R0, R4 
    /*000009f8*/ MOV         R2, #43                     @ 0x2b 
    /*000009fc*/ MOVW        R1, #18453                  @ 0x4815 
    /*00000a00*/ BL          os05a_2l_write_register
    /*00000a04*/ MOV         R0, R4 
    /*00000a08*/ MOV         R2, #54                     @ 0x36 
    /*00000a0c*/ MOVW        R1, #18542                  @ 0x486e 
    /*00000a10*/ BL          os05a_2l_write_register
    /*00000a14*/ MOV         R0, R4 
    /*00000a18*/ MOV         R2, #132                    @ 0x84 
    /*00000a1c*/ MOVW        R1, #18543                  @ 0x486f 
    /*00000a20*/ BL          os05a_2l_write_register
    /*00000a24*/ MOV         R0, R4 
    /*00000a28*/ MOV         R2, #0 
    /*00000a2c*/ MOVW        R1, #18528                  @ 0x4860 
    /*00000a30*/ BL          os05a_2l_write_register
    /*00000a34*/ MOV         R0, R4 
    /*00000a38*/ MOV         R2, #160                    @ 0xa0 
    /*00000a3c*/ MOVW        R1, #18529                  @ 0x4861 
    /*00000a40*/ BL          os05a_2l_write_register
    /*00000a44*/ MOV         R0, R4 
    /*00000a48*/ MOV         R2, #5 
    /*00000a4c*/ MOVW        R1, #18507                  @ 0x484b 
    /*00000a50*/ BL          os05a_2l_write_register
    /*00000a54*/ MOV         R0, R4 
    /*00000a58*/ MOV         R2, #0 
    /*00000a5c*/ MOVW        R1, #18512                  @ 0x4850 
    /*00000a60*/ BL          os05a_2l_write_register
    /*00000a64*/ MOV         R0, R4 
    /*00000a68*/ MOV         R2, #170                    @ 0xaa 
    /*00000a6c*/ MOVW        R1, #18513                  @ 0x4851 
    /*00000a70*/ BL          os05a_2l_write_register
    /*00000a74*/ MOV         R0, R4 
    /*00000a78*/ MOV         R2, #255                    @ 0xff 
    /*00000a7c*/ MOVW        R1, #18514                  @ 0x4852 
    /*00000a80*/ BL          os05a_2l_write_register
    /*00000a84*/ MOV         R0, R4 
    /*00000a88*/ MOV         R2, #138                    @ 0x8a 
    /*00000a8c*/ MOVW        R1, #18515                  @ 0x4853 
    /*00000a90*/ BL          os05a_2l_write_register
    /*00000a94*/ MOV         R0, R4 
    /*00000a98*/ MOV         R2, #8 
    /*00000a9c*/ MOVW        R1, #18516                  @ 0x4854 
    /*00000aa0*/ BL          os05a_2l_write_register
    /*00000aa4*/ MOV         R0, R4 
    /*00000aa8*/ MOV         R2, #48                     @ 0x30 
    /*00000aac*/ MOVW        R1, #18517                  @ 0x4855 
    /*00000ab0*/ BL          os05a_2l_write_register
    /*00000ab4*/ MOV         R0, R4 
    /*00000ab8*/ MOV         R2, #0 
    /*00000abc*/ MOV         R1, #18432                  @ 0x4800 
    /*00000ac0*/ BL          os05a_2l_write_register
    /*00000ac4*/ MOV         R0, R4 
    /*00000ac8*/ MOV         R2, #20 
    /*00000acc*/ MOVW        R1, #18487                  @ 0x4837 
    /*00000ad0*/ BL          os05a_2l_write_register
    /*00000ad4*/ MOV         R0, R4 
    /*00000ad8*/ MOV         R2, #201                    @ 0xc9 
    /*00000adc*/ MOV         R1, #20480                  @ 0x5000 
    /*00000ae0*/ BL          os05a_2l_write_register
    /*00000ae4*/ MOV         R0, R4 
    /*00000ae8*/ MOV         R2, #67                     @ 0x43 
    /*00000aec*/ MOVW        R1, #20481                  @ 0x5001 
    /*00000af0*/ BL          os05a_2l_write_register
    /*00000af4*/ MOV         R0, R4 
    /*00000af8*/ MOV         R2, #3 
    /*00000afc*/ MOVW        R1, #21009                  @ 0x5211 
    /*00000b00*/ BL          os05a_2l_write_register
    /*00000b04*/ MOV         R0, R4 
    /*00000b08*/ MOV         R2, #3 
    /*00000b0c*/ MOVW        R1, #21137                  @ 0x5291 
    /*00000b10*/ BL          os05a_2l_write_register
    /*00000b14*/ MOV         R0, R4 
    /*00000b18*/ MOV         R2, #15 
    /*00000b1c*/ MOVW        R1, #21005                  @ 0x520d 
    /*00000b20*/ BL          os05a_2l_write_register
    /*00000b24*/ MOV         R0, R4 
    /*00000b28*/ MOV         R2, #253                    @ 0xfd 
    /*00000b2c*/ MOVW        R1, #21006                  @ 0x520e 
    /*00000b30*/ BL          os05a_2l_write_register
    /*00000b34*/ MOV         R0, R4 
    /*00000b38*/ MOV         R2, #165                    @ 0xa5 
    /*00000b3c*/ MOVW        R1, #21007                  @ 0x520f 
    /*00000b40*/ BL          os05a_2l_write_register
    /*00000b44*/ MOV         R0, R4 
    /*00000b48*/ MOV         R2, #165                    @ 0xa5 
    /*00000b4c*/ MOVW        R1, #21008                  @ 0x5210 
    /*00000b50*/ BL          os05a_2l_write_register
    /*00000b54*/ MOV         R0, R4 
    /*00000b58*/ MOV         R2, #15 
    /*00000b5c*/ MOVW        R1, #21133                  @ 0x528d 
    /*00000b60*/ BL          os05a_2l_write_register
    /*00000b64*/ MOV         R0, R4 
    /*00000b68*/ MOV         R2, #253                    @ 0xfd 
    /*00000b6c*/ MOVW        R1, #21134                  @ 0x528e 
    /*00000b70*/ BL          os05a_2l_write_register
    /*00000b74*/ MOV         R0, R4 
    /*00000b78*/ MOV         R2, #165                    @ 0xa5 
    /*00000b7c*/ MOVW        R1, #21135                  @ 0x528f 
    /*00000b80*/ BL          os05a_2l_write_register
    /*00000b84*/ MOV         R0, R4 
    /*00000b88*/ MOV         R2, #165                    @ 0xa5 
    /*00000b8c*/ MOVW        R1, #21136                  @ 0x5290 
    /*00000b90*/ BL          os05a_2l_write_register
    /*00000b94*/ MOV         R0, R4 
    /*00000b98*/ MOV         R2, #64                     @ 0x40 
    /*00000b9c*/ MOVW        R1, #20484                  @ 0x5004 
    /*00000ba0*/ BL          os05a_2l_write_register
    /*00000ba4*/ MOV         R0, R4 
    /*00000ba8*/ MOV         R2, #0 
    /*00000bac*/ MOVW        R1, #20485                  @ 0x5005 
    /*00000bb0*/ BL          os05a_2l_write_register
    /*00000bb4*/ MOV         R0, R4 
    /*00000bb8*/ MOV         R2, #0 
    /*00000bbc*/ MOVW        R1, #20864                  @ 0x5180 
    /*00000bc0*/ BL          os05a_2l_write_register
    /*00000bc4*/ MOV         R0, R4 
    /*00000bc8*/ MOV         R2, #16 
    /*00000bcc*/ MOVW        R1, #20865                  @ 0x5181 
    /*00000bd0*/ BL          os05a_2l_write_register
    /*00000bd4*/ MOV         R0, R4 
    /*00000bd8*/ MOV         R2, #15 
    /*00000bdc*/ MOVW        R1, #20866                  @ 0x5182 
    /*00000be0*/ BL          os05a_2l_write_register
    /*00000be4*/ MOV         R0, R4 
    /*00000be8*/ MOV         R2, #255                    @ 0xff 
    /*00000bec*/ MOVW        R1, #20867                  @ 0x5183 
    /*00000bf0*/ BL          os05a_2l_write_register
    /*00000bf4*/ MOV         R0, R4 
    /*00000bf8*/ MOV         R2, #3 
    /*00000bfc*/ MOVW        R1, #22539                  @ 0x580b 
    /*00000c00*/ BL          os05a_2l_write_register
    /*00000c04*/ MOV         R0, R4 
    /*00000c08*/ MOV         R2, #3 
    /*00000c0c*/ MOV         R1, #19712                  @ 0x4d00 
    /*00000c10*/ BL          os05a_2l_write_register
    /*00000c14*/ MOV         R0, R4 
    /*00000c18*/ MOV         R2, #233                    @ 0xe9 
    /*00000c1c*/ MOVW        R1, #19713                  @ 0x4d01 
    /*00000c20*/ BL          os05a_2l_write_register
    /*00000c24*/ MOV         R0, R4 
    /*00000c28*/ MOV         R2, #186                    @ 0xba 
    /*00000c2c*/ MOVW        R1, #19714                  @ 0x4d02 
    /*00000c30*/ BL          os05a_2l_write_register
    /*00000c34*/ MOV         R0, R4 
    /*00000c38*/ MOV         R2, #102                    @ 0x66 
    /*00000c3c*/ MOVW        R1, #19715                  @ 0x4d03 
    /*00000c40*/ BL          os05a_2l_write_register
    /*00000c44*/ MOV         R0, R4 
    /*00000c48*/ MOV         R2, #70                     @ 0x46 
    /*00000c4c*/ MOVW        R1, #19716                  @ 0x4d04 
    /*00000c50*/ BL          os05a_2l_write_register
    /*00000c54*/ MOV         R0, R4 
    /*00000c58*/ MOV         R2, #165                    @ 0xa5 
    /*00000c5c*/ MOVW        R1, #19717                  @ 0x4d05 
    /*00000c60*/ BL          os05a_2l_write_register
    /*00000c64*/ MOV         R0, R4 
    /*00000c68*/ MOV         R2, #60                     @ 0x3c 
    /*00000c6c*/ MOVW        R1, #13827                  @ 0x3603 
    /*00000c70*/ BL          os05a_2l_write_register
    /*00000c74*/ MOV         R0, R4 
    /*00000c78*/ MOV         R2, #38                     @ 0x26 
    /*00000c7c*/ MOVW        R1, #14083                  @ 0x3703 
    /*00000c80*/ BL          os05a_2l_write_register
    /*00000c84*/ MOV         R0, R4 
    /*00000c88*/ MOV         R2, #74                     @ 0x4a 
    /*00000c8c*/ MOVW        R1, #14089                  @ 0x3709 
    /*00000c90*/ BL          os05a_2l_write_register
    /*00000c94*/ MOV         R0, R4 
    /*00000c98*/ MOV         R2, #38                     @ 0x26 
    /*00000c9c*/ MOVW        R1, #14088                  @ 0x3708 
    /*00000ca0*/ BL          os05a_2l_write_register
    /*00000ca4*/ MOV         R0, R4 
    /*00000ca8*/ MOV         R2, #24 
    /*00000cac*/ MOVW        R1, #14105                  @ 0x3719 
    /*00000cb0*/ BL          os05a_2l_write_register
    /*00000cb4*/ MOV         R0, R4 
    /*00000cb8*/ MOV         R2, #5 
    /*00000cbc*/ MOVW        R1, #14106                  @ 0x371a 
    /*00000cc0*/ BL          os05a_2l_write_register
    /*00000cc4*/ MOV         R0, R4 
    /*00000cc8*/ MOV         R2, #2 
    /*00000ccc*/ MOVW        R1, #803                    @ 0x323 
    /*00000cd0*/ BL          os05a_2l_write_register
    /*00000cd4*/ MOV         R0, R4 
    /*00000cd8*/ MOV         R2, #90                     @ 0x5a 
    /*00000cdc*/ MOVW        R1, #805                    @ 0x325 
    /*00000ce0*/ BL          os05a_2l_write_register
    /*00000ce4*/ MOV         R0, R4 
    /*00000ce8*/ MOV         R2, #2 
    /*00000cec*/ MOVW        R1, #809                    @ 0x329 
    /*00000cf0*/ BL          os05a_2l_write_register
    /*00000cf4*/ MOV         R0, R4 
    /*00000cf8*/ MOV         R2, #7 
    /*00000cfc*/ MOV         R1, #808                    @ 0x328 
    /*00000d00*/ BL          os05a_2l_write_register
    /*00000d04*/ MOV         R0, R4 
    /*00000d08*/ MOV         R2, #1 
    /*00000d0c*/ MOVW        R1, #810                    @ 0x32a 
    /*00000d10*/ BL          os05a_2l_write_register
    /*00000d14*/ MOV         R0, R4 
    /*00000d18*/ MOV         R2, #16 
    /*00000d1c*/ MOVW        R1, #12550                  @ 0x3106 
    /*00000d20*/ BL          os05a_2l_write_register
    /*00000d24*/ MOV         R0, R4 
    /*00000d28*/ MOV         R2, #5 
    /*00000d2c*/ MOVW        R1, #14348                  @ 0x380c 
    /*00000d30*/ BL          os05a_2l_write_register
    /*00000d34*/ MOV         R0, R4 
    /*00000d38*/ MOV         R2, #224                    @ 0xe0 
    /*00000d3c*/ MOVW        R1, #14349                  @ 0x380d 
    /*00000d40*/ BL          os05a_2l_write_register
    /*00000d44*/ MOV         R0, R4 
    /*00000d48*/ MOV         R2, #7 
    /*00000d4c*/ MOVW        R1, #14350                  @ 0x380e 
    /*00000d50*/ BL          os05a_2l_write_register
    /*00000d54*/ MOV         R0, R4 
    /*00000d58*/ MOV         R2, #203                    @ 0xcb 
    /*00000d5c*/ MOVW        R1, #14351                  @ 0x380f 
    /*00000d60*/ BL          os05a_2l_write_register
    /*00000d64*/ MOV         R0, R4 
    /*00000d68*/ MOV         R2, #5 
    /*00000d6c*/ MOVW        R1, #13569                  @ 0x3501 
    /*00000d70*/ BL          os05a_2l_write_register
    /*00000d74*/ MOV         R0, R4 
    /*00000d78*/ MOV         R2, #240                    @ 0xf0 
    /*00000d7c*/ MOVW        R1, #13570                  @ 0x3502 
    /*00000d80*/ BL          os05a_2l_write_register
    /*00000d84*/ MOV         R2, #121                    @ 0x79 
    /*00000d88*/ MOV         R1, #16384                  @ 0x4000 
    /*00000d8c*/ MOV         R0, R4 
    /*00000d90*/ BL          os05a_2l_write_register
    /*00000d94*/ MOV         R0, R4 
    /*00000d98*/ BL          os05a_2l_default_reg_init
    /*00000d9c*/ MOV         R0, R4 
    /*00000da0*/ MOV         R2, #1 
    /*00000da4*/ MOV         R1, #256                    @ 0x100 
    /*00000da8*/ BL          os05a_2l_write_register
    /*00000dac*/ MOV         R0, R4 
    /*00000db0*/ MOV         R2, #0 
    /*00000db4*/ MOV         R1, #256                    @ 0x100 
    /*00000db8*/ BL          os05a_2l_write_register
    /*00000dbc*/ MOV         R0, R4 
    /*00000dc0*/ MOV         R2, #1 
    /*00000dc4*/ MOV         R1, #256                    @ 0x100 
    /*00000dc8*/ BL          os05a_2l_write_register
    /*00000dcc*/ LDR         R0, WORD_0ddc               @ LDR         R0, [PC, #8]                @ 0x0000000000000ddc 
    /*00000dd0*/ POP         {R4, LR} 
    /*00000dd4*/ ADD         R0, PC, R0 
    os05a_2l_init_5M_2688x1944_12bit_linear30_xdd8:
    /*00000dd8*/ B           os05a_2l_init_5M_2688x1944_12bit_linear30_xdd8@ puts [R_ARM_JUMP24: +0x0] 
    /*00000ddc*/ WORD_0ddc: .word 0x00000000
FUNC_END os05a_2l_init_5M_2688x1944_12bit_linear30


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.os05a_2l_init
@ Size: 0xf0
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN os05a_2l_init
    /*00000000*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000004*/ SUB         SP, SP, #8 
    /*00000008*/ LDR         R5, WORD_00d4               @ LDR         R5, [PC, #196]              @ 0x00000000000000d4 
    /*0000000c*/ MOV         R4, R0 
    /*00000010*/ LDR         R3, WORD_00d8               @ LDR         R3, [PC, #192]              @ 0x00000000000000d8 
    /*00000014*/ ADD         R5, PC, R5 
    /*00000018*/ LDR         R6, [R5, R3] 
    /*0000001c*/ LDR         R3, [R6, R0, LSL #2] 
    /*00000020*/ LDR         R8, [R3] 
    /*00000024*/ LDRB        R7, [R3, #8] 
    /*00000028*/ BL          os05a_2l_i2c_init
    /*0000002c*/ CMP         R8, #0 
    /*00000030*/ BNE         os05a_2l_init_x50
    /*00000034*/ CMP         R7, #0 
    /*00000038*/ BEQ         os05a_2l_init_x8c
    /*0000003c*/ CMP         R7, #1 
    /*00000040*/ BNE         os05a_2l_init_xa8
    os05a_2l_init_x44:
    /*00000044*/ MOV         R0, R4 
    /*00000048*/ BL          os05a_2l_init_5M_2688x1944_10bit_wdr30
    /*0000004c*/ B           os05a_2l_init_x94
    os05a_2l_init_x50:
    /*00000050*/ CMP         R7, #0 
    /*00000054*/ BEQ         os05a_2l_init_x8c
    /*00000058*/ CMP         R7, #1 
    /*0000005c*/ BEQ         os05a_2l_init_x44
    /*00000060*/ LDR         R1, WORD_00dc               @ LDR         R1, [PC, #116]              @ 0x00000000000000dc 
    /*00000064*/ MOV         R3, #272                    @ 0x110 
    /*00000068*/ LDR         R2, WORD_00e0               @ LDR         R2, [PC, #112]              @ 0x00000000000000e0 
    /*0000006c*/ LDR         R0, [R5, R1] 
    /*00000070*/ LDR         R1, WORD_00e4               @ LDR         R1, [PC, #108]              @ 0x00000000000000e4 
    /*00000074*/ ADD         R2, PC, R2 
    /*00000078*/ ADD         R1, PC, R1 
    /*0000007c*/ LDR         R0, [R0] 
    /*00000080*/ STR         R7, [SP] 
    /*00000084*/ BL          fprintf
    /*00000088*/ B           os05a_2l_init_x94
    os05a_2l_init_x8c:
    /*0000008c*/ MOV         R0, R4 
    /*00000090*/ BL          os05a_2l_init_5M_2688x1944_12bit_linear30
    os05a_2l_init_x94:
    /*00000094*/ LDR         R3, [R6, R4, LSL #2] 
    /*00000098*/ MOV         R2, #1 
    /*0000009c*/ STR         R2, [R3] 
    /*000000a0*/ ADD         SP, SP, #8 
    /*000000a4*/ POP         {R4, R5, R6, R7, R8, PC} 
    os05a_2l_init_xa8:
    /*000000a8*/ LDR         R1, WORD_00dc               @ LDR         R1, [PC, #44]               @ 0x00000000000000dc 
    /*000000ac*/ MOV         R3, #253                    @ 0xfd 
    /*000000b0*/ LDR         R2, WORD_00e8               @ LDR         R2, [PC, #48]               @ 0x00000000000000e8 
    /*000000b4*/ LDR         R0, [R5, R1] 
    /*000000b8*/ LDR         R1, WORD_00ec               @ LDR         R1, [PC, #44]               @ 0x00000000000000ec 
    /*000000bc*/ ADD         R2, PC, R2 
    /*000000c0*/ ADD         R1, PC, R1 
    /*000000c4*/ LDR         R0, [R0] 
    /*000000c8*/ STR         R7, [SP] 
    /*000000cc*/ BL          fprintf
    /*000000d0*/ B           os05a_2l_init_x94
    /*000000d4*/ WORD_00d4: .word 0x000000b8
    /*000000d8*/ WORD_00d8: .word 0x00000000
    /*000000dc*/ WORD_00dc: .word 0x00000000
    /*000000e0*/ WORD_00e0: .word 0x00000064
    /*000000e4*/ WORD_00e4: .word 0x00000064
    /*000000e8*/ WORD_00e8: .word 0x00000024
    /*000000ec*/ WORD_00ec: .word 0x00000024
FUNC_END os05a_2l_init


