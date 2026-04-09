@
@ -- FILES --
@

@ mn34220_cmos.c


unsigned char data[0] = {
};

unsigned char bss[0];

unsigned char bss.g_astMn34220[16];

unsigned char bss.genFSWDRMode[16];

unsigned char bss.gu32MaxTimeGetCnt[16];

unsigned char bss.u8Count.10601[1];

unsigned char data.bFirst.10600[4] = {
    /*0000*/ 0x01, 0x00, 0x00, 0x00, 
};

unsigned char data.g_aunMn34220BusInfo[4] = {
    /*0000*/ 0x00, 0xff, 0xff, 0xff, 
};

unsigned char data.g_stAwbAgcTable[20] = {
    /*0000*/ 0x01, 0x00, 0x00, 0x00, 0x80, 0x7a, 0x78, 0x74, 0x68, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x38, 
    /*0010*/ 0x38, 0x38, 0x38, 0x38, 
};

unsigned char data.g_stAwbAgcTableFSWDR[20] = {
    /*0000*/ 0x01, 0x00, 0x00, 0x00, 0x78, 0x78, 0x6e, 0x64, 0x5e, 0x58, 0x50, 0x48, 0x40, 0x38, 0x38, 0x38, 
    /*0010*/ 0x38, 0x38, 0x38, 0x38, 
};

unsigned char data.g_stAwbCcm[142] = {
    /*0000*/ 0x03, 0x00, 0x24, 0x13, 0xc4, 0x01, 0xb8, 0x80, 0x0c, 0x80, 0x4d, 0x80, 0x64, 0x01, 0x17, 0x80, 
    /*0010*/ 0x01, 0x80, 0xc5, 0x80, 0xc6, 0x01, 0x0a, 0x0f, 0xc0, 0x01, 0xab, 0x80, 0x15, 0x80, 0x41, 0x80, 
    /*0020*/ 0x64, 0x01, 0x23, 0x80, 0x01, 0x80, 0xda, 0x80, 0xdb, 0x01, 0x5a, 0x0a, 0x98, 0x01, 0x7a, 0x80, 
    /*0030*/ 0x1e, 0x80, 0x56, 0x80, 0x7d, 0x01, 0x27, 0x80, 0x01, 0x80, 0x58, 0x81, 0x58, 0x02, 0x34, 0x08, 
    /*0040*/ 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0050*/ 0x00, 0x01, 0x40, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
    /*0060*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x78, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0070*/ 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe8, 0x03, 0x00, 0x01, 0x00, 0x00, 
    /*0080*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
};

unsigned char data.g_stAwbCcmFsWdr[142] = {
    /*0000*/ 0x03, 0x00, 0x24, 0x13, 0x12, 0x01, 0x01, 0x80, 0x11, 0x80, 0x4c, 0x80, 0x66, 0x01, 0x1a, 0x80, 
    /*0010*/ 0x01, 0x80, 0x22, 0x80, 0x23, 0x01, 0x0a, 0x0f, 0x12, 0x01, 0x01, 0x80, 0x11, 0x80, 0x5c, 0x80, 
    /*0020*/ 0x76, 0x01, 0x1a, 0x80, 0x01, 0x80, 0x32, 0x80, 0x33, 0x01, 0x5a, 0x0a, 0x12, 0x01, 0x01, 0x80, 
    /*0030*/ 0x11, 0x80, 0x6c, 0x80, 0x86, 0x01, 0x1a, 0x80, 0x01, 0x80, 0x42, 0x80, 0x43, 0x01, 0x34, 0x08, 
    /*0040*/ 0x12, 0x01, 0x11, 0x80, 0x01, 0x80, 0x6c, 0x80, 0x86, 0x01, 0x1a, 0x80, 0x01, 0x80, 0x42, 0x80, 
    /*0050*/ 0x43, 0x01, 0x40, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
    /*0060*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x78, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0070*/ 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe8, 0x03, 0x00, 0x01, 0x00, 0x00, 
    /*0080*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
};

unsigned char data.g_stDngColorParam[12] = {
    /*0000*/ 0x7a, 0x01, 0x00, 0x01, 0xae, 0x01, 0xb7, 0x01, 0x00, 0x01, 0xb7, 0x01, 
};

unsigned char data.g_stIspNoiseCalibration[604] = {
    /*0000*/ 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd4, 0x42, 0xa8, 0xa7, 0x8f, 0x3c, 0xfc, 0xa5, 0xc5, 0x3c, 
    /*0010*/ 0x00, 0x00, 0xea, 0x42, 0xc7, 0xd7, 0x9e, 0x3c, 0xa6, 0x47, 0xd3, 0x3c, 0x00, 0x00, 0x63, 0x43, 
    /*0020*/ 0x10, 0x91, 0x1a, 0x3d, 0x59, 0x8a, 0x24, 0x3d, 0x00, 0x00, 0xeb, 0x43, 0xe1, 0x45, 0x9f, 0x3d, 
    /*0030*/ 0x35, 0x7f, 0xac, 0x3d, 0x00, 0x60, 0xda, 0x44, 0x11, 0x18, 0x93, 0x3e, 0x7d, 0x3e, 0xc2, 0x3e, 
    /*0040*/ 0x00, 0x20, 0x7a, 0x45, 0x9d, 0xd8, 0x27, 0x3f, 0xad, 0xf7, 0x93, 0x3f, 0x00, 0xd8, 0xca, 0x45, 
    /*0050*/ 0x47, 0x54, 0x88, 0x3f, 0xc7, 0x7f, 0x15, 0x40, 0x00, 0x84, 0x79, 0x46, 0x4c, 0x1c, 0x2e, 0x40, 
    /*0060*/ 0x01, 0x4e, 0x27, 0x41, 0x00, 0x54, 0xe8, 0x46, 0x08, 0x81, 0xa5, 0x40, 0xbf, 0x60, 0xf6, 0x41, 
    /*0070*/ 0x00, 0xcf, 0x67, 0x47, 0x80, 0x84, 0x2c, 0x41, 0x3a, 0xea, 0xc8, 0x42, 0x00, 0x6a, 0xc9, 0x47, 
    /*0080*/ 0x14, 0x00, 0xa8, 0x41, 0x1e, 0x63, 0x52, 0x43, 0x40, 0x38, 0x69, 0x48, 0x72, 0x31, 0xe6, 0x41, 
    /*0090*/ 0xd2, 0xe7, 0x80, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*00a0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*00b0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*00c0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*00d0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*00e0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*00f0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0100*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0110*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0120*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0130*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0140*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0150*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0160*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0170*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0180*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0190*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*01a0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*01b0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*01c0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*01d0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*01e0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*01f0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0200*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0210*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0220*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0230*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0240*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0250*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

unsigned char data.gstPirisAttr[2080] = {
    /*0000*/ 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x5e, 0x00, 0x3e, 0x00, 0x1e, 0x00, 0x23, 0x00, 
    /*0010*/ 0x28, 0x00, 0x2d, 0x00, 0x32, 0x00, 0x38, 0x00, 0x3d, 0x00, 0x43, 0x00, 0x49, 0x00, 0x4f, 0x00, 
    /*0020*/ 0x55, 0x00, 0x5c, 0x00, 0x62, 0x00, 0x69, 0x00, 0x70, 0x00, 0x78, 0x00, 0x7f, 0x00, 0x87, 0x00, 
    /*0030*/ 0x8f, 0x00, 0x96, 0x00, 0x9e, 0x00, 0xa6, 0x00, 0xae, 0x00, 0xb7, 0x00, 0xbf, 0x00, 0xc8, 0x00, 
    /*0040*/ 0xd0, 0x00, 0xd9, 0x00, 0xe1, 0x00, 0xea, 0x00, 0xf3, 0x00, 0xfc, 0x00, 0x05, 0x01, 0x0e, 0x01, 
    /*0050*/ 0x17, 0x01, 0x21, 0x01, 0x2a, 0x01, 0x33, 0x01, 0x3c, 0x01, 0x45, 0x01, 0x4f, 0x01, 0x58, 0x01, 
    /*0060*/ 0x61, 0x01, 0x6a, 0x01, 0x74, 0x01, 0x7d, 0x01, 0x86, 0x01, 0x8f, 0x01, 0x98, 0x01, 0xa1, 0x01, 
    /*0070*/ 0xaa, 0x01, 0xb3, 0x01, 0xbc, 0x01, 0xc5, 0x01, 0xce, 0x01, 0xd6, 0x01, 0xde, 0x01, 0xe6, 0x01, 
    /*0080*/ 0xed, 0x01, 0xf4, 0x01, 0xfa, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0090*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*00a0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*00b0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*00c0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*00d0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*00e0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*00f0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0100*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0110*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0120*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0130*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0140*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0150*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0160*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0170*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0180*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0190*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*01a0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*01b0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*01c0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*01d0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*01e0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*01f0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0200*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0210*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0220*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0230*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0240*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0250*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0260*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0270*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0280*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0290*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*02a0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*02b0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*02c0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*02d0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*02e0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*02f0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0300*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0310*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0320*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0330*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0340*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0350*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0360*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0370*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0380*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0390*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*03a0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*03b0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*03c0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*03d0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*03e0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*03f0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0400*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0410*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0420*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0430*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0440*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0450*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0460*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0470*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0480*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0490*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*04a0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*04b0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*04c0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*04d0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*04e0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*04f0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0500*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0510*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0520*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0530*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0540*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0550*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0560*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0570*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0580*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0590*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*05a0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*05b0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*05c0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*05d0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*05e0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*05f0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0600*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0610*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0620*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0630*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0640*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0650*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0660*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0670*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0680*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0690*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*06a0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*06b0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*06c0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*06d0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*06e0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*06f0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0700*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0710*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0720*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0730*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0740*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0750*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0760*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0770*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0780*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0790*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*07a0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*07b0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*07c0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*07d0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*07e0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*07f0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0800*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 
    /*0810*/ 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 
};

unsigned char data.rel.stSnsMn34220Obj[36] = {
    /*0000*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0010*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /*0020*/ 0x00, 0x00, 0x00, 0x00, 
};

unsigned char rel.data.rel.stSnsMn34220Obj[56] = {
    /*0000*/ 0x00, 0x00, 0x00, 0x00, 0x02, 0x2a, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x63, 0x00, 0x00, 
    /*0010*/ 0x08, 0x00, 0x00, 0x00, 0x02, 0x07, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x02, 0x64, 0x01, 0x00, 
    /*0020*/ 0x10, 0x00, 0x00, 0x00, 0x02, 0x65, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x02, 0x4d, 0x01, 0x00, 
    /*0030*/ 0x1c, 0x00, 0x00, 0x00, 0x02, 0x66, 0x01, 0x00, 
};

_RODATA___FUNCTION___10543_0000: .ascii "cmos_get_ae_default\0",
_RODATA___FUNCTION___10543_0014: .ascii ""


_RODATA___FUNCTION___10558_0000: .ascii "cmos_fps_set_2To1_LINE\0"


_RODATA___FUNCTION___10565_0000: .ascii "cmos_fps_set_3To1_LINE\0"


_RODATA___FUNCTION___10572_0000: .ascii "cmos_fps_set_4To1_LINE\0"


_RODATA___FUNCTION___10579_0000: .ascii "cmos_fps_set_None_LINE\0"


_RODATA___FUNCTION___10587_0000: .ascii "cmos_fps_set\0"


_RODATA___FUNCTION___10594_0000: .ascii "cmos_slow_framerate_set\0",
_RODATA___FUNCTION___10594_0018: .ascii ""


_RODATA___FUNCTION___10602_0000: .ascii "cmos_inttime_update\0",
_RODATA___FUNCTION___10602_0014: .ascii ""


_RODATA___FUNCTION___10610_0000: .ascii "cmos_again_calc_table\0"


_RODATA___FUNCTION___10620_0000: .ascii "cmos_dgain_calc_table\0"


_RODATA___FUNCTION___10632_0000: .ascii "cmos_gains_update\0"


_RODATA___FUNCTION___10657_0000: .ascii "cmos_get_inttime_max\0"


_RODATA___FUNCTION___10680_0000: .ascii "cmos_ae_fswdr_attr_set\0"


_RODATA___FUNCTION___10690_0000: .ascii "cmos_get_awb_default\0"


_RODATA___FUNCTION___10708_0000: .ascii "cmos_get_isp_default\0"


_RODATA___FUNCTION___10728_0000: .ascii "cmos_get_isp_black_level\0"


_RODATA___FUNCTION___10739_0000: .ascii "cmos_set_pixel_detect\0"


_RODATA___FUNCTION___10745_0000: .ascii "cmos_set_wdr_mode\0"


_RODATA___FUNCTION___10760_0000: .ascii "cmos_get_sns_regs_info\0"


_RODATA___FUNCTION___10773_0000: .ascii "cmos_set_image_mode_VGA\0",
_RODATA___FUNCTION___10773_0018: .ascii ""


_RODATA___FUNCTION___10780_0000: .ascii "cmos_set_image_mode_720P\0"


_RODATA___FUNCTION___10787_0000: .ascii "cmos_set_image_mode_1080P\0"


_RODATA___FUNCTION___10795_0000: .ascii "cmos_set_image_mode\0",
_RODATA___FUNCTION___10795_0014: .ascii ""


_RODATA___FUNCTION___10800_0000: .ascii "sensor_global_init\0"


_RODATA___FUNCTION___10813_0000: .ascii "sensor_ctx_init\0",
_RODATA___FUNCTION___10813_0010: .ascii ""


_RODATA___FUNCTION___10828_0000: .ascii "sensor_register_callback\0"


_RODATA___FUNCTION___10835_0000: .ascii "sensor_unregister_callback\0"


_RODATA_AD_GAIN_TABLE_0000: .ascii "\0\x04\0\0",
_RODATA_AD_GAIN_TABLE_0004: .ascii ".\x04\0\0",
_RODATA_AD_GAIN_TABLE_0008: .ascii "\\x04\0\0",
_RODATA_AD_GAIN_TABLE_000c: .ascii "\x8e\x04\0\0",
_RODATA_AD_GAIN_TABLE_0010: .ascii "\xc1\x04\0\0",
_RODATA_AD_GAIN_TABLE_0014: .ascii "\xf7\x04\0\0",
_RODATA_AD_GAIN_TABLE_0018: .ascii "/\x05\0\0",
_RODATA_AD_GAIN_TABLE_001c: .ascii "j\x05\0\0",
_RODATA_AD_GAIN_TABLE_0020: .ascii "\xa6\x05\0\0",
_RODATA_AD_GAIN_TABLE_0024: .ascii "\xe7\x05\0\0",
_RODATA_AD_GAIN_TABLE_0028: .ascii ")\x06\0\0",
_RODATA_AD_GAIN_TABLE_002c: .ascii "o\x06\0\0",
_RODATA_AD_GAIN_TABLE_0030: .ascii "\xb7\x06\0\0",
_RODATA_AD_GAIN_TABLE_0034: .ascii "\x04\x07\0\0",
_RODATA_AD_GAIN_TABLE_0038: .ascii "R\x07\0\0",
_RODATA_AD_GAIN_TABLE_003c: .ascii "\xa6\x07\0\0",
_RODATA_AD_GAIN_TABLE_0040: .ascii "\xfb\x07\0\0",
_RODATA_AD_GAIN_TABLE_0044: .ascii "W\x08\0\0",
_RODATA_AD_GAIN_TABLE_0048: .ascii "\xb3\x08\0\0",
_RODATA_AD_GAIN_TABLE_004c: .ascii "\x17\t\0\0",
_RODATA_AD_GAIN_TABLE_0050: .ascii "|\t\0\0",
_RODATA_AD_GAIN_TABLE_0054: .ascii "\xe9\t\0\0",
_RODATA_AD_GAIN_TABLE_0058: .ascii "W\n\0\0",
_RODATA_AD_GAIN_TABLE_005c: .ascii "\xce\n\0\0",
_RODATA_AD_GAIN_TABLE_0060: .ascii "F\x0b\0\0",
_RODATA_AD_GAIN_TABLE_0064: .ascii "\xc7\x0b\0\0",
_RODATA_AD_GAIN_TABLE_0068: .ascii "J\x0c\0\0",
_RODATA_AD_GAIN_TABLE_006c: .ascii "\xd7\x0c\0\0",
_RODATA_AD_GAIN_TABLE_0070: .ascii "f\r\0\0",
_RODATA_AD_GAIN_TABLE_0074: .ascii "\xff\r\0\0",
_RODATA_AD_GAIN_TABLE_0078: .ascii "\x9b\x0e\0\0",
_RODATA_AD_GAIN_TABLE_007c: .ascii "C\x0f\0\0",
_RODATA_AD_GAIN_TABLE_0080: .ascii "\xed\x0f\0\0",
_RODATA_AD_GAIN_TABLE_0084: .ascii "\xa3\x10\0\0",
_RODATA_AD_GAIN_TABLE_0088: .ascii "\\x11\0\0",
_RODATA_AD_GAIN_TABLE_008c: .ascii "#\x12\0\0",
_RODATA_AD_GAIN_TABLE_0090: .ascii "\xed\x12\0\0",
_RODATA_AD_GAIN_TABLE_0094: .ascii "\xc6\x13\0\0",
_RODATA_AD_GAIN_TABLE_0098: .ascii "\xa2\x14\0\0",
_RODATA_AD_GAIN_TABLE_009c: .ascii "\x8e\x15\0\0",
_RODATA_AD_GAIN_TABLE_00a0: .ascii "~\x16\0\0",
_RODATA_AD_GAIN_TABLE_00a4: .ascii "\x80\x17\0\0",
_RODATA_AD_GAIN_TABLE_00a8: .ascii "\x86\x18\0\0",
_RODATA_AD_GAIN_TABLE_00ac: .ascii "\x9e\x19\0\0",
_RODATA_AD_GAIN_TABLE_00b0: .ascii "\xbc\x1a\0\0",
_RODATA_AD_GAIN_TABLE_00b4: .ascii "\xee\x1b\0\0",
_RODATA_AD_GAIN_TABLE_00b8: .ascii "%\x1d\0\0",
_RODATA_AD_GAIN_TABLE_00bc: .ascii "s\x1e\0\0",
_RODATA_AD_GAIN_TABLE_00c0: .ascii "\xc6\x1f\0\0",
_RODATA_AD_GAIN_TABLE_00c4: .ascii "2!\0\0",
_RODATA_AD_GAIN_TABLE_00c8: .ascii "\xa3\"\0\0",
_RODATA_AD_GAIN_TABLE_00cc: .ascii "0$\0\0",
_RODATA_AD_GAIN_TABLE_00d0: .ascii "\xc3%\0\0",
_RODATA_AD_GAIN_TABLE_00d4: .ascii "t'\0\0",
_RODATA_AD_GAIN_TABLE_00d8: .ascii "+)\0\0",
_RODATA_AD_GAIN_TABLE_00dc: .ascii "\x02+\0\0",
_RODATA_AD_GAIN_TABLE_00e0: .ascii "\xe1,\0\0",
_RODATA_AD_GAIN_TABLE_00e4: .ascii "\xe3.\0\0",
_RODATA_AD_GAIN_TABLE_00e8: .ascii "\xee0\0\0",
_RODATA_AD_GAIN_TABLE_00ec: .ascii "\x1e3\0\0",
_RODATA_AD_GAIN_TABLE_00f0: .ascii "W5\0\0",
_RODATA_AD_GAIN_TABLE_00f4: .ascii "\xba7\0\0",
_RODATA_AD_GAIN_TABLE_00f8: .ascii "':\0\0",
_RODATA_AD_GAIN_TABLE_00fc: .ascii "\xc0<\0\0",
_RODATA_AD_GAIN_TABLE_0100: .ascii "e?\0\0",
_RODATA_AD_GAIN_TABLE_0104: .ascii ";B\0\0",
_RODATA_AD_GAIN_TABLE_0108: .ascii "\x1dE\0\0",
_RODATA_AD_GAIN_TABLE_010c: .ascii "4H\0\0",
_RODATA_AD_GAIN_TABLE_0110: .ascii "YK\0\0",
_RODATA_AD_GAIN_TABLE_0114: .ascii "\xb7N\0\0",
_RODATA_AD_GAIN_TABLE_0118: .ascii "$R\0\0",
_RODATA_AD_GAIN_TABLE_011c: .ascii "\xd0U\0\0",
_RODATA_AD_GAIN_TABLE_0120: .ascii "\x8dY\0\0",
_RODATA_AD_GAIN_TABLE_0124: .ascii "\x8e]\0\0",
_RODATA_AD_GAIN_TABLE_0128: .ascii "\xa0a\0\0",
_RODATA_AD_GAIN_TABLE_012c: .ascii "\xfee\0\0",
_RODATA_AD_GAIN_TABLE_0130: .ascii "nj\0\0",
_RODATA_AD_GAIN_TABLE_0134: .ascii "0o\0\0",
_RODATA_AD_GAIN_TABLE_0138: .ascii "\x07t\0\0",
_RODATA_AD_GAIN_TABLE_013c: .ascii "7y\0\0",
_RODATA_AD_GAIN_TABLE_0140: .ascii "~~\0\0",
_RODATA_AD_GAIN_TABLE_0144: .ascii ""


_RODATA_CMOS_FPS_SET_STR1_4_0000: .ascii "[Func]:%s [Line]:%d [Info]:Not support Fps: %f\n\0",
_RODATA_CMOS_FPS_SET_STR1_4_0030: .ascii "[Func]:%s [Line]:%d [Info]:Not support! u8ImgMode:%d, f32Fps:%f\n\0"


_RODATA_CMOS_GET_INTTIME_MAX_STR1_4_0000: .ascii "[Func]:%s [Line]:%d [Info]:Manaul ExpRatio is too large!\n\0"


_RODATA_CMOS_SET_IMAGE_MODE_STR1_4_0000: .ascii "[Func]:%s [Line]:%d [Info]:Not support! Width:%d, Height:%d, Fps:%f, WDRMode:%d\n\0"


_RODATA_CMOS_SET_PIXEL_DETECT_STR1_4_0000: .ascii "[Func]:%s [Line]:%d [Info]:Null Pointer!\n\0"


_RODATA_CMOS_SET_WDR_MODE_STR1_4_0000: .ascii "linear mode\0",
_RODATA_CMOS_SET_WDR_MODE_STR1_4_000c: .ascii "2to1 line WDR mode\0\0",
_RODATA_CMOS_SET_WDR_MODE_STR1_4_0020: .ascii "2to1 half-rate frame WDR mode\0\0\0",
_RODATA_CMOS_SET_WDR_MODE_STR1_4_0040: .ascii "2to1 full-rate frame WDR mode\0\0\0",
_RODATA_CMOS_SET_WDR_MODE_STR1_4_0060: .ascii "3to1 line WDR mode\0\0",
_RODATA_CMOS_SET_WDR_MODE_STR1_4_0074: .ascii "4to1 line WDR mode\0\0",
_RODATA_CMOS_SET_WDR_MODE_STR1_4_0088: .ascii "[Func]:%s [Line]:%d [Info]:NOT support this mode!\n\0"


_RODATA_G_STCMOSDPC_0000: .ascii "\0\0\0\0",
_RODATA_G_STCMOSDPC_0004: .ascii "\0\0\x98\0",
_RODATA_G_STCMOSDPC_0008: .ascii "\xdc\0\xdc\0",
_RODATA_G_STCMOSDPC_000c: .ascii "\xdc\0\xdc\0",
_RODATA_G_STCMOSDPC_0010: .ascii "\xdc\0\xdc\0",
_RODATA_G_STCMOSDPC_0014: .ascii "\x98\0\x98\0",
_RODATA_G_STCMOSDPC_0018: .ascii "\x98\0\x98\0",
_RODATA_G_STCMOSDPC_001c: .ascii "\x98\0\x98\0",
_RODATA_G_STCMOSDPC_0020: .ascii "\0\0\0\0",
_RODATA_G_STCMOSDPC_0024: .ascii "\0\0\0\0",
_RODATA_G_STCMOSDPC_0028: .ascii "\0\0\0\0",
_RODATA_G_STCMOSDPC_002c: .ascii "\0\0\0\0",
_RODATA_G_STCMOSDPC_0030: .ascii "\0\0\0\0",
_RODATA_G_STCMOSDPC_0034: .ascii "2\02\0",
_RODATA_G_STCMOSDPC_0038: .ascii "2\02\0",
_RODATA_G_STCMOSDPC_003c: .ascii "2\02\0",
_RODATA_G_STCMOSDPC_0040: .ascii ""


_RODATA_G_STCMOSLSC_0000: .ascii "\0\x10\x04\0",
_RODATA_G_STCMOSLSC_0004: .ascii "k\x02(\x02\xde\x01\xa8\x01y\x01I\x01*\x01\t\x01\xee\0\xd5\0",
_RODATA_G_STCMOSLSC_0018: .ascii "\xc4\0\xb0\0",
_RODATA_G_STCMOSLSC_001c: .ascii "\xa2\0\x9b\0",
_RODATA_G_STCMOSLSC_0020: .ascii "\x94\0\x8f\0",
_RODATA_G_STCMOSLSC_0024: .ascii "\x94\0\x93\0",
_RODATA_G_STCMOSLSC_0028: .ascii "\x97\0\x9f\0",
_RODATA_G_STCMOSLSC_002c: .ascii "\xad\0\xb8\0",
_RODATA_G_STCMOSLSC_0030: .ascii "\xc8\0\xdb\0",
_RODATA_G_STCMOSLSC_0034: .ascii "\r\x01J\x013\x01Z\x01\x84\x01\xc1\x01\xfd\x01;\x02\x84\x02N\x02\x12\x02\xd3\x01\x9a\x01h\x01=\x01\x1b\x01\xfa\0\xe4\0",
_RODATA_G_STCMOSLSC_0058: .ascii "\xca\0\xbb\0",
_RODATA_G_STCMOSLSC_005c: .ascii "\xa7\0\x9a\0",
_RODATA_G_STCMOSLSC_0060: .ascii "\x93\0\x87\0",
_RODATA_G_STCMOSLSC_0064: .ascii "\x88\0\x86\0",
_RODATA_G_STCMOSLSC_0068: .ascii "\x85\0\x8d\0",
_RODATA_G_STCMOSLSC_006c: .ascii "\x98\0\xa0\0",
_RODATA_G_STCMOSLSC_0070: .ascii "\xab\0\xbd\0",
_RODATA_G_STCMOSLSC_0074: .ascii "\xce\0\xf8\0",
_RODATA_G_STCMOSLSC_0078: .ascii ".\x01#\x01L\x01y\x01\xaf\x01\xe8\x01-\x02g\x025\x02\xf9\x01\xba\x01\x8a\x01X\x01,\x01\r\x01\xe9\0",
_RODATA_G_STCMOSLSC_0098: .ascii "\xd3\0\xbf\0",
_RODATA_G_STCMOSLSC_009c: .ascii "\xad\0\x9b\0",
_RODATA_G_STCMOSLSC_00a0: .ascii "\x8c\0~\0",
_RODATA_G_STCMOSLSC_00a4: .ascii "{\0y\0",
_RODATA_G_STCMOSLSC_00a8: .ascii "t\0y\0",
_RODATA_G_STCMOSLSC_00ac: .ascii "{\0\x86\0",
_RODATA_G_STCMOSLSC_00b0: .ascii "\x90\0\x9f\0",
_RODATA_G_STCMOSLSC_00b4: .ascii "\xaf\0\xc1\0",
_RODATA_G_STCMOSLSC_00b8: .ascii "\xd8\0\xff\0",
_RODATA_G_STCMOSLSC_00bc: .ascii "\x14\x017\x01d\x01\x98\x01\xd1\x01\x13\x02S\x02\x1f\x02\xe5\x01\xa8\x01r\x01F\x01\x1a\x01\xfc\0",
_RODATA_G_STCMOSLSC_00d8: .ascii "\xde\0\xc8\0",
_RODATA_G_STCMOSLSC_00dc: .ascii "\xb1\0\x9d\0",
_RODATA_G_STCMOSLSC_00e0: .ascii "\x8a\0\x7f\0",
_RODATA_G_STCMOSLSC_00e4: .ascii "s\0l\0",
_RODATA_G_STCMOSLSC_00e8: .ascii "f\0e\0",
_RODATA_G_STCMOSLSC_00ec: .ascii "h\0l\0",
_RODATA_G_STCMOSLSC_00f0: .ascii "t\0\x80\0",
_RODATA_G_STCMOSLSC_00f4: .ascii "\x92\0\xa0\0",
_RODATA_G_STCMOSLSC_00f8: .ascii "\xb5\0\xc9\0",
_RODATA_G_STCMOSLSC_00fc: .ascii "\xe4\0\x03\x01(\x01T\x01\x83\x01\xbc\x01\xfe\x01;\x02\x10\x02\xd3\x01\x94\x01`\x016\x01\x0e\x01\xec\0\xd3\0",
_RODATA_G_STCMOSLSC_011c: .ascii "\xbc\0\xa4\0",
_RODATA_G_STCMOSLSC_0120: .ascii "\x91\0\x81\0",
_RODATA_G_STCMOSLSC_0124: .ascii "o\0g\0",
_RODATA_G_STCMOSLSC_0128: .ascii "]\0Y\0",
_RODATA_G_STCMOSLSC_012c: .ascii "Y\0Y\0",
_RODATA_G_STCMOSLSC_0130: .ascii "_\0j\0",
_RODATA_G_STCMOSLSC_0134: .ascii "u\0\x84\0",
_RODATA_G_STCMOSLSC_0138: .ascii "\x94\0\xab\0",
_RODATA_G_STCMOSLSC_013c: .ascii "\xbf\0\xda\0",
_RODATA_G_STCMOSLSC_0140: .ascii "\xf7\0\x1b\x01I\x01s\x01\xac\x01\xed\x01)\x02\xfa\x01\xc3\x01\x85\x01R\x01$\x01\x01\x01\xe2\0",
_RODATA_G_STCMOSLSC_015c: .ascii "\xcd\0\xaf\0",
_RODATA_G_STCMOSLSC_0160: .ascii "\x9c\0\x86\0",
_RODATA_G_STCMOSLSC_0164: .ascii "t\0e\0",
_RODATA_G_STCMOSLSC_0168: .ascii "Z\0R\0",
_RODATA_G_STCMOSLSC_016c: .ascii "L\0L\0",
_RODATA_G_STCMOSLSC_0170: .ascii "M\0R\0",
_RODATA_G_STCMOSLSC_0174: .ascii "^\0k\0",
_RODATA_G_STCMOSLSC_0178: .ascii "z\0\x8a\0",
_RODATA_G_STCMOSLSC_017c: .ascii "\x9f\0\xb5\0",
_RODATA_G_STCMOSLSC_0180: .ascii "\xcb\0\xeb\0",
_RODATA_G_STCMOSLSC_0184: .ascii "\r\x017\x01e\x01\x9c\x01\xda\x01\x18\x02\xe1\x01\xb1\x01w\x01H\x01\x1a\x01\xf8\0",
_RODATA_G_STCMOSLSC_019c: .ascii "\xd7\0\xbf\0",
_RODATA_G_STCMOSLSC_01a0: .ascii "\xa6\0\x90\0",
_RODATA_G_STCMOSLSC_01a4: .ascii "|\0h\0",
_RODATA_G_STCMOSLSC_01a8: .ascii "[\0M\0",
_RODATA_G_STCMOSLSC_01ac: .ascii "D\0@\0",
_RODATA_G_STCMOSLSC_01b0: .ascii ">\0@\0",
_RODATA_G_STCMOSLSC_01b4: .ascii "J\0Q\0",
_RODATA_G_STCMOSLSC_01b8: .ascii "\\0l\0",
_RODATA_G_STCMOSLSC_01bc: .ascii "}\0\x95\0",
_RODATA_G_STCMOSLSC_01c0: .ascii "\xab\0\xc3\0",
_RODATA_G_STCMOSLSC_01c4: .ascii "\xe2\0\x01\x01'\x01T\x01\x8f\x01\xcd\x01\0\x02\xd8\x01\xa2\x01i\x01<\x01\x11\x01\xee\0\xd0\0",
_RODATA_G_STCMOSLSC_01e0: .ascii "\xb6\0\x9e\0",
_RODATA_G_STCMOSLSC_01e4: .ascii "\x87\0s\0",
_RODATA_G_STCMOSLSC_01e8: .ascii "_\0O\0",
_RODATA_G_STCMOSLSC_01ec: .ascii "D\08\0",
_RODATA_G_STCMOSLSC_01f0: .ascii "6\05\0",
_RODATA_G_STCMOSLSC_01f4: .ascii "6\0?\0",
_RODATA_G_STCMOSLSC_01f8: .ascii "G\0U\0",
_RODATA_G_STCMOSLSC_01fc: .ascii "d\0t\0",
_RODATA_G_STCMOSLSC_0200: .ascii "\x8d\0\xa3\0",
_RODATA_G_STCMOSLSC_0204: .ascii "\xbd\0\xd7\0",
_RODATA_G_STCMOSLSC_0208: .ascii "\xf8\0\x1e\x01J\x01\x82\x01\xc2\x01\xf4\x01\xc8\x01\x97\x01_\x011\x01\r\x01\xe9\0",
_RODATA_G_STCMOSLSC_0220: .ascii "\xc9\0\xae\0",
_RODATA_G_STCMOSLSC_0224: .ascii "\x98\0~\0",
_RODATA_G_STCMOSLSC_0228: .ascii "i\0Y\0",
_RODATA_G_STCMOSLSC_022c: .ascii "J\0<\0",
_RODATA_G_STCMOSLSC_0230: .ascii "2\0+\0",
_RODATA_G_STCMOSLSC_0234: .ascii "*\0+\0",
_RODATA_G_STCMOSLSC_0238: .ascii "3\0A\0",
_RODATA_G_STCMOSLSC_023c: .ascii "J\0U\0",
_RODATA_G_STCMOSLSC_0240: .ascii "k\0\x82\0",
_RODATA_G_STCMOSLSC_0244: .ascii "\x9e\0\xb4\0",
_RODATA_G_STCMOSLSC_0248: .ascii "\xd0\0\xed\0",
_RODATA_G_STCMOSLSC_024c: .ascii "\x13\x01B\x01u\x01\xb3\x01\xe5\x01\xc1\x01\x89\x01T\x01&\x01\x03\x01\xdf\0\xc5\0",
_RODATA_G_STCMOSLSC_0264: .ascii "\xa4\0\x8b\0",
_RODATA_G_STCMOSLSC_0268: .ascii "w\0b\0",
_RODATA_G_STCMOSLSC_026c: .ascii "N\0>\0",
_RODATA_G_STCMOSLSC_0270: .ascii "4\0(\0",
_RODATA_G_STCMOSLSC_0274: .ascii " \0\x1f\0",
_RODATA_G_STCMOSLSC_0278: .ascii "#\0+\0",
_RODATA_G_STCMOSLSC_027c: .ascii "5\0C\0",
_RODATA_G_STCMOSLSC_0280: .ascii "S\0d\0",
_RODATA_G_STCMOSLSC_0284: .ascii "z\0\x95\0",
_RODATA_G_STCMOSLSC_0288: .ascii "\xac\0\xca\0",
_RODATA_G_STCMOSLSC_028c: .ascii "\xe8\0\x0c\x01:\x01k\x01\xa9\x01\xe2\x01\xb6\x01\x82\x01O\x01$\x01\xf9\0\xdb\0",
_RODATA_G_STCMOSLSC_02a4: .ascii "\xbf\0\xa0\0",
_RODATA_G_STCMOSLSC_02a8: .ascii "\x84\0m\0",
_RODATA_G_STCMOSLSC_02ac: .ascii "W\0F\0",
_RODATA_G_STCMOSLSC_02b0: .ascii "4\0)\0",
_RODATA_G_STCMOSLSC_02b4: .ascii "!\0\x17\0",
_RODATA_G_STCMOSLSC_02b8: .ascii "\x18\0\x17\0",
_RODATA_G_STCMOSLSC_02bc: .ascii "!\0,\0",
_RODATA_G_STCMOSLSC_02c0: .ascii ":\0K\0",
_RODATA_G_STCMOSLSC_02c4: .ascii "^\0s\0",
_RODATA_G_STCMOSLSC_02c8: .ascii "\x8e\0\xa5\0",
_RODATA_G_STCMOSLSC_02cc: .ascii "\xc2\0\xe3\0",
_RODATA_G_STCMOSLSC_02d0: .ascii "\x05\x015\x01b\x01\xa0\x01\xd8\x01\xb0\x01w\x01F\x01\x1d\x01\xf6\0",
_RODATA_G_STCMOSLSC_02e4: .ascii "\xd7\0\xb8\0",
_RODATA_G_STCMOSLSC_02e8: .ascii "\x99\0\x81\0",
_RODATA_G_STCMOSLSC_02ec: .ascii "j\0S\0",
_RODATA_G_STCMOSLSC_02f0: .ascii "A\0/\0",
_RODATA_G_STCMOSLSC_02f4: .ascii "!\0\x18\0",
_RODATA_G_STCMOSLSC_02f8: .ascii "\x11\0\x0f\0",
_RODATA_G_STCMOSLSC_02fc: .ascii "\x11\0\x1b\0",
_RODATA_G_STCMOSLSC_0300: .ascii "%\05\0",
_RODATA_G_STCMOSLSC_0304: .ascii "G\0[\0",
_RODATA_G_STCMOSLSC_0308: .ascii "l\0\x86\0",
_RODATA_G_STCMOSLSC_030c: .ascii "\xa2\0\xbc\0",
_RODATA_G_STCMOSLSC_0310: .ascii "\xdf\0\x01\x01)\x01_\x01\x98\x01\xcd\x01\xa9\x01u\x01@\x01\x12\x01\xef\0\xd4\0",
_RODATA_G_STCMOSLSC_0328: .ascii "\xb4\0\x96\0",
_RODATA_G_STCMOSLSC_032c: .ascii "}\0e\0",
_RODATA_G_STCMOSLSC_0330: .ascii "P\09\0",
_RODATA_G_STCMOSLSC_0334: .ascii "-\0\x1c\0",
_RODATA_G_STCMOSLSC_0338: .ascii "\x11\0\t\0",
_RODATA_G_STCMOSLSC_033c: .ascii "\n\0\x0b\0",
_RODATA_G_STCMOSLSC_0340: .ascii "\x15\0\x1f\0",
_RODATA_G_STCMOSLSC_0344: .ascii "/\0?\0",
_RODATA_G_STCMOSLSC_0348: .ascii "W\0i\0",
_RODATA_G_STCMOSLSC_034c: .ascii "\x83\0\x9e\0",
_RODATA_G_STCMOSLSC_0350: .ascii "\xb9\0\xd9\0",
_RODATA_G_STCMOSLSC_0354: .ascii "\xfb\0&\x01V\x01\x92\x01\xc3\x01\xa6\x01q\x01:\x01\x11\x01\xef\0",
_RODATA_G_STCMOSLSC_0368: .ascii "\xce\0\xb1\0",
_RODATA_G_STCMOSLSC_036c: .ascii "\x95\0z\0",
_RODATA_G_STCMOSLSC_0370: .ascii "e\0N\0",
_RODATA_G_STCMOSLSC_0374: .ascii "9\0(\0",
_RODATA_G_STCMOSLSC_0378: .ascii "\x17\0\t\0",
_RODATA_G_STCMOSLSC_037c: .ascii "\x05\0\x03\0",
_RODATA_G_STCMOSLSC_0380: .ascii "\x07\0\x10\0",
_RODATA_G_STCMOSLSC_0384: .ascii "\x1b\0+\0",
_RODATA_G_STCMOSLSC_0388: .ascii ">\0Q\0",
_RODATA_G_STCMOSLSC_038c: .ascii "g\0\x82\0",
_RODATA_G_STCMOSLSC_0390: .ascii "\x9c\0\xba\0",
_RODATA_G_STCMOSLSC_0394: .ascii "\xd4\0\xfa\0",
_RODATA_G_STCMOSLSC_0398: .ascii "$\x01R\x01\x8d\x01\xbe\x01\xa5\x01m\x014\x01\r\x01\xe9\0\xd0\0",
_RODATA_G_STCMOSLSC_03ac: .ascii "\xae\0\x92\0",
_RODATA_G_STCMOSLSC_03b0: .ascii "z\0]\0",
_RODATA_G_STCMOSLSC_03b4: .ascii "K\03\0",
_RODATA_G_STCMOSLSC_03b8: .ascii "&\0\x15\0",
_RODATA_G_STCMOSLSC_03bc: .ascii "\x08\0\x02\0",
_RODATA_G_STCMOSLSC_03c0: .ascii "\x04\0\x08\0",
_RODATA_G_STCMOSLSC_03c4: .ascii "\x0c\0\x16\0",
_RODATA_G_STCMOSLSC_03c8: .ascii "(\0:\0",
_RODATA_G_STCMOSLSC_03cc: .ascii "O\0e\0",
_RODATA_G_STCMOSLSC_03d0: .ascii "\x7f\0\x9a\0",
_RODATA_G_STCMOSLSC_03d4: .ascii "\xb8\0\xd6\0",
_RODATA_G_STCMOSLSC_03d8: .ascii "\xf7\0#\x01Q\x01\x8a\x01\xbf\x01\x9b\x01g\x017\x01\x0c\x01\xe8\0",
_RODATA_G_STCMOSLSC_03ec: .ascii "\xcb\0\xab\0",
_RODATA_G_STCMOSLSC_03f0: .ascii "\x8f\0x\0",
_RODATA_G_STCMOSLSC_03f4: .ascii "]\0G\0",
_RODATA_G_STCMOSLSC_03f8: .ascii "3\0'\0",
_RODATA_G_STCMOSLSC_03fc: .ascii "\x11\0\x04\0",
_RODATA_G_STCMOSLSC_0400: .ascii "\0\0\x02\0",
_RODATA_G_STCMOSLSC_0404: .ascii "\x06\0\x0e\0",
_RODATA_G_STCMOSLSC_0408: .ascii "\x16\0'\0",
_RODATA_G_STCMOSLSC_040c: .ascii ";\0P\0",
_RODATA_G_STCMOSLSC_0410: .ascii "e\0}\0",
_RODATA_G_STCMOSLSC_0414: .ascii "\x98\0\xb7\0",
_RODATA_G_STCMOSLSC_0418: .ascii "\xd4\0\xf4\0",
_RODATA_G_STCMOSLSC_041c: .ascii "!\x01Q\x01\x8d\x01\xbe\x01\x9d\x01e\x014\x01\x0f\x01\xe8\0\xc8\0",
_RODATA_G_STCMOSLSC_0430: .ascii "\xac\0\x8f\0",
_RODATA_G_STCMOSLSC_0434: .ascii "u\0\\0",
_RODATA_G_STCMOSLSC_0438: .ascii "H\07\0",
_RODATA_G_STCMOSLSC_043c: .ascii "4\0\x11\0",
_RODATA_G_STCMOSLSC_0440: .ascii "\x04\0\0\0",
_RODATA_G_STCMOSLSC_0444: .ascii "\0\0\x03\0",
_RODATA_G_STCMOSLSC_0448: .ascii "\n\0\x16\0",
_RODATA_G_STCMOSLSC_044c: .ascii "%\09\0",
_RODATA_G_STCMOSLSC_0450: .ascii "P\0d\0",
_RODATA_G_STCMOSLSC_0454: .ascii "~\0\x9a\0",
_RODATA_G_STCMOSLSC_0458: .ascii "\xb5\0\xd5\0",
_RODATA_G_STCMOSLSC_045c: .ascii "\xf8\0 \x01P\x01\x8b\x01\xbd\x01\x98\x01j\x015\x01\x0e\x01\xe8\0",
_RODATA_G_STCMOSLSC_0470: .ascii "\xc9\0\xab\0",
_RODATA_G_STCMOSLSC_0474: .ascii "\x8b\0t\0",
_RODATA_G_STCMOSLSC_0478: .ascii "\\0K\0",
_RODATA_G_STCMOSLSC_047c: .ascii "9\06\0",
_RODATA_G_STCMOSLSC_0480: .ascii "\x15\0\x06\0",
_RODATA_G_STCMOSLSC_0484: .ascii "\x01\0\x01\0",
_RODATA_G_STCMOSLSC_0488: .ascii "\x03\0\x0c\0",
_RODATA_G_STCMOSLSC_048c: .ascii "\x17\0&\0",
_RODATA_G_STCMOSLSC_0490: .ascii ";\0N\0",
_RODATA_G_STCMOSLSC_0494: .ascii "e\0\x7f\0",
_RODATA_G_STCMOSLSC_0498: .ascii "\x98\0\xb6\0",
_RODATA_G_STCMOSLSC_049c: .ascii "\xd5\0\xfa\0",
_RODATA_G_STCMOSLSC_04a0: .ascii "\x1e\x01P\x01\x87\x01\xc0\x01\x99\x01g\x014\x01\r\x01\xea\0\xca\0",
_RODATA_G_STCMOSLSC_04b4: .ascii "\xae\0\x8e\0",
_RODATA_G_STCMOSLSC_04b8: .ascii "s\0`\0",
_RODATA_G_STCMOSLSC_04bc: .ascii "K\09\0",
_RODATA_G_STCMOSLSC_04c0: .ascii "2\0\x16\0",
_RODATA_G_STCMOSLSC_04c4: .ascii "\x05\0\x04\0",
_RODATA_G_STCMOSLSC_04c8: .ascii "\x02\0\x06\0",
_RODATA_G_STCMOSLSC_04cc: .ascii "\r\0\x19\0",
_RODATA_G_STCMOSLSC_04d0: .ascii "*\0<\0",
_RODATA_G_STCMOSLSC_04d4: .ascii "N\0f\0",
_RODATA_G_STCMOSLSC_04d8: .ascii "\x83\0\x9a\0",
_RODATA_G_STCMOSLSC_04dc: .ascii "\xbc\0\xd3\0",
_RODATA_G_STCMOSLSC_04e0: .ascii "\xf9\0\x1f\x01Q\x01\x8a\x01\xc0\x01\x9d\x01k\x015\x01\x10\x01\xef\0",
_RODATA_G_STCMOSLSC_04f4: .ascii "\xce\0\xae\0",
_RODATA_G_STCMOSLSC_04f8: .ascii "\x94\0{\0",
_RODATA_G_STCMOSLSC_04fc: .ascii "d\0M\0",
_RODATA_G_STCMOSLSC_0500: .ascii ";\0+\0",
_RODATA_G_STCMOSLSC_0504: .ascii "\x19\0\t\0",
_RODATA_G_STCMOSLSC_0508: .ascii "\t\0\x07\0",
_RODATA_G_STCMOSLSC_050c: .ascii "\x0c\0\x0f\0",
_RODATA_G_STCMOSLSC_0510: .ascii "\x1c\0,\0",
_RODATA_G_STCMOSLSC_0514: .ascii "A\0T\0",
_RODATA_G_STCMOSLSC_0518: .ascii "j\0\x82\0",
_RODATA_G_STCMOSLSC_051c: .ascii "\x9a\0\xbb\0",
_RODATA_G_STCMOSLSC_0520: .ascii "\xd8\0\xfc\0",
_RODATA_G_STCMOSLSC_0524: .ascii "$\x01V\x01\x8c\x01\xc0\x01\xa5\x01p\x019\x01\x14\x01\xee\0\xd1\0",
_RODATA_G_STCMOSLSC_0538: .ascii "\xb1\0\x95\0",
_RODATA_G_STCMOSLSC_053c: .ascii "~\0e\0",
_RODATA_G_STCMOSLSC_0540: .ascii "Q\0=\0",
_RODATA_G_STCMOSLSC_0544: .ascii "/\0\x1f\0",
_RODATA_G_STCMOSLSC_0548: .ascii "\x14\0\x0c\0",
_RODATA_G_STCMOSLSC_054c: .ascii "\r\0\x0e\0",
_RODATA_G_STCMOSLSC_0550: .ascii "\x16\0#\0",
_RODATA_G_STCMOSLSC_0554: .ascii "1\0B\0",
_RODATA_G_STCMOSLSC_0558: .ascii "V\0o\0",
_RODATA_G_STCMOSLSC_055c: .ascii "\x86\0\x9e\0",
_RODATA_G_STCMOSLSC_0560: .ascii "\xbb\0\xda\0",
_RODATA_G_STCMOSLSC_0564: .ascii "\0\x01(\x01X\x01\x90\x01\xc3\x01\xa9\x01r\x01D\x01\x15\x01\xf2\0",
_RODATA_G_STCMOSLSC_0578: .ascii "\xd3\0\xb9\0",
_RODATA_G_STCMOSLSC_057c: .ascii "\x99\0\x80\0",
_RODATA_G_STCMOSLSC_0580: .ascii "k\0X\0",
_RODATA_G_STCMOSLSC_0584: .ascii "C\03\0",
_RODATA_G_STCMOSLSC_0588: .ascii "(\0\x1b\0",
_RODATA_G_STCMOSLSC_058c: .ascii "\x16\0\x14\0",
_RODATA_G_STCMOSLSC_0590: .ascii "\x15\0\x1b\0",
_RODATA_G_STCMOSLSC_0594: .ascii "*\0;\0",
_RODATA_G_STCMOSLSC_0598: .ascii "H\0[\0",
_RODATA_G_STCMOSLSC_059c: .ascii "u\0\x8a\0",
_RODATA_G_STCMOSLSC_05a0: .ascii "\xa4\0\xc1\0",
_RODATA_G_STCMOSLSC_05a4: .ascii "\xdc\0\x03\x01,\x01]\x01\x96\x01\xcc\x01\xb2\x01{\x01G\x01\x1b\x01\xfe\0\xda\0",
_RODATA_G_STCMOSLSC_05bc: .ascii "\xbd\0\x9f\0",
_RODATA_G_STCMOSLSC_05c0: .ascii "\x87\0r\0",
_RODATA_G_STCMOSLSC_05c4: .ascii "_\0J\0",
_RODATA_G_STCMOSLSC_05c8: .ascii ";\0.\0",
_RODATA_G_STCMOSLSC_05cc: .ascii "$\0\x1d\0",
_RODATA_G_STCMOSLSC_05d0: .ascii "\x1b\0\x1d\0",
_RODATA_G_STCMOSLSC_05d4: .ascii "$\02\0",
_RODATA_G_STCMOSLSC_05d8: .ascii "C\0Q\0",
_RODATA_G_STCMOSLSC_05dc: .ascii "d\0y\0",
_RODATA_G_STCMOSLSC_05e0: .ascii "\x90\0\xaa\0",
_RODATA_G_STCMOSLSC_05e4: .ascii "\xc8\0\xeb\0",
_RODATA_G_STCMOSLSC_05e8: .ascii "\x07\x014\x01d\x01\x9e\x01\xd2\x01\xb3\x01\x87\x01N\x01%\x01\x02\x01\xe2\0\xc1\0",
_RODATA_G_STCMOSLSC_0600: .ascii "\xa6\0\x8e\0",
_RODATA_G_STCMOSLSC_0604: .ascii "w\0e\0",
_RODATA_G_STCMOSLSC_0608: .ascii "S\0D\0",
_RODATA_G_STCMOSLSC_060c: .ascii "7\0,\0",
_RODATA_G_STCMOSLSC_0610: .ascii "'\0%\0",
_RODATA_G_STCMOSLSC_0614: .ascii "&\0+\0",
_RODATA_G_STCMOSLSC_0618: .ascii "7\0E\0",
_RODATA_G_STCMOSLSC_061c: .ascii "U\0g\0",
_RODATA_G_STCMOSLSC_0620: .ascii "\x7f\0\x97\0",
_RODATA_G_STCMOSLSC_0624: .ascii "\xb3\0\xcb\0",
_RODATA_G_STCMOSLSC_0628: .ascii "\xec\0\x0f\x01<\x01i\x01\xa4\x01\xdd\x01\xc1\x01\x8b\x01Y\x01.\x01\x07\x01\xe4\0",
_RODATA_G_STCMOSLSC_0640: .ascii "\xca\0\xaf\0",
_RODATA_G_STCMOSLSC_0644: .ascii "\x94\0\x80\0",
_RODATA_G_STCMOSLSC_0648: .ascii "n\0Z\0",
_RODATA_G_STCMOSLSC_064c: .ascii "M\0@\0",
_RODATA_G_STCMOSLSC_0650: .ascii "6\00\0",
_RODATA_G_STCMOSLSC_0654: .ascii "0\01\0",
_RODATA_G_STCMOSLSC_0658: .ascii "6\0A\0",
_RODATA_G_STCMOSLSC_065c: .ascii "N\0_\0",
_RODATA_G_STCMOSLSC_0660: .ascii "t\0\x89\0",
_RODATA_G_STCMOSLSC_0664: .ascii "\x9b\0\xb5\0",
_RODATA_G_STCMOSLSC_0668: .ascii "\xd1\0\xf1\0",
_RODATA_G_STCMOSLSC_066c: .ascii "\x1a\x01A\x01s\x01\xaa\x01\xe2\x01\xc9\x01\x98\x01b\x013\x01\x12\x01\xec\0\xd5\0",
_RODATA_G_STCMOSLSC_0684: .ascii "\xbc\0\x9e\0",
_RODATA_G_STCMOSLSC_0688: .ascii "\x88\0u\0",
_RODATA_G_STCMOSLSC_068c: .ascii "e\0V\0",
_RODATA_G_STCMOSLSC_0690: .ascii "L\0B\0",
_RODATA_G_STCMOSLSC_0694: .ascii ";\0:\0",
_RODATA_G_STCMOSLSC_0698: .ascii "?\0@\0",
_RODATA_G_STCMOSLSC_069c: .ascii "L\0W\0",
_RODATA_G_STCMOSLSC_06a0: .ascii "j\0|\0",
_RODATA_G_STCMOSLSC_06a4: .ascii "\x91\0\xa5\0",
_RODATA_G_STCMOSLSC_06a8: .ascii "\xc0\0\xda\0",
_RODATA_G_STCMOSLSC_06ac: .ascii "\xf8\0\x1e\x01H\x01{\x01\xb7\x01\xef\x01\xde\x01\xa5\x01o\x01@\x01\x19\x01\xf8\0",
_RODATA_G_STCMOSLSC_06c4: .ascii "\xda\0\xc4\0",
_RODATA_G_STCMOSLSC_06c8: .ascii "\xa7\0\x92\0",
_RODATA_G_STCMOSLSC_06cc: .ascii "\x7f\0m\0",
_RODATA_G_STCMOSLSC_06d0: .ascii "d\0W\0",
_RODATA_G_STCMOSLSC_06d4: .ascii "N\0H\0",
_RODATA_G_STCMOSLSC_06d8: .ascii "E\0H\0",
_RODATA_G_STCMOSLSC_06dc: .ascii "O\0V\0",
_RODATA_G_STCMOSLSC_06e0: .ascii "b\0s\0",
_RODATA_G_STCMOSLSC_06e4: .ascii "\x86\0\x98\0",
_RODATA_G_STCMOSLSC_06e8: .ascii "\xb1\0\xc5\0",
_RODATA_G_STCMOSLSC_06ec: .ascii "\xe6\0\x05\x01(\x01U\x01\x88\x01\xc4\x01\xfd\x01\xe8\x01\xb2\x01{\x01K\x01&\x01\x03\x01\xe8\0",
_RODATA_G_STCMOSLSC_0708: .ascii "\xcc\0\xb1\0",
_RODATA_G_STCMOSLSC_070c: .ascii "\x9e\0\x89\0",
_RODATA_G_STCMOSLSC_0710: .ascii "}\0o\0",
_RODATA_G_STCMOSLSC_0714: .ascii "`\0Y\0",
_RODATA_G_STCMOSLSC_0718: .ascii "S\0S\0",
_RODATA_G_STCMOSLSC_071c: .ascii "U\0[\0",
_RODATA_G_STCMOSLSC_0720: .ascii "d\0o\0",
_RODATA_G_STCMOSLSC_0724: .ascii "\x80\0\x8f\0",
_RODATA_G_STCMOSLSC_0728: .ascii "\xa5\0\xbe\0",
_RODATA_G_STCMOSLSC_072c: .ascii "\xd4\0\xf0\0",
_RODATA_G_STCMOSLSC_0730: .ascii "\r\x015\x01_\x01\x8f\x01\xd6\x01\x0c\x02\xf9\x01\xbf\x01\x8a\x01Z\x011\x01\x0e\x01\xf2\0\xd7\0",
_RODATA_G_STCMOSLSC_074c: .ascii "\xbf\0\xab\0",
_RODATA_G_STCMOSLSC_0750: .ascii "\x99\0\x89\0",
_RODATA_G_STCMOSLSC_0754: .ascii "y\0p\0",
_RODATA_G_STCMOSLSC_0758: .ascii "e\0a\0",
_RODATA_G_STCMOSLSC_075c: .ascii "`\0b\0",
_RODATA_G_STCMOSLSC_0760: .ascii "j\0r\0",
_RODATA_G_STCMOSLSC_0764: .ascii "{\0\x8c\0",
_RODATA_G_STCMOSLSC_0768: .ascii "\x9e\0\xb3\0",
_RODATA_G_STCMOSLSC_076c: .ascii "\xc9\0\xe2\0",
_RODATA_G_STCMOSLSC_0770: .ascii "\xfb\0\x17\x01=\x01i\x01\x9f\x01\xdf\x01\x1e\x02\x0c\x02\xcf\x01\x98\x01f\x01?\x01\x19\x01\xfb\0",
_RODATA_G_STCMOSLSC_078c: .ascii "\xe0\0\xca\0",
_RODATA_G_STCMOSLSC_0790: .ascii "\xb6\0\xa7\0",
_RODATA_G_STCMOSLSC_0794: .ascii "\x95\0\x85\0",
_RODATA_G_STCMOSLSC_0798: .ascii "}\0t\0",
_RODATA_G_STCMOSLSC_079c: .ascii "n\0p\0",
_RODATA_G_STCMOSLSC_07a0: .ascii "p\0t\0",
_RODATA_G_STCMOSLSC_07a4: .ascii "}\0\x87\0",
_RODATA_G_STCMOSLSC_07a8: .ascii "\x98\0\xa9\0",
_RODATA_G_STCMOSLSC_07ac: .ascii "\xbe\0\xd2\0",
_RODATA_G_STCMOSLSC_07b0: .ascii "\xee\0\x06\x01%\x01L\x01v\x01\xaf\x01\xf4\x01-\x02\x1c\x02\xe3\x01\xa9\x01x\x01K\x01&\x01\t\x01\xee\0",
_RODATA_G_STCMOSLSC_07d0: .ascii "\xd6\0\xc3\0",
_RODATA_G_STCMOSLSC_07d4: .ascii "\xb4\0\xa5\0",
_RODATA_G_STCMOSLSC_07d8: .ascii "\x96\0\x8d\0",
_RODATA_G_STCMOSLSC_07dc: .ascii "\x83\0}\0",
_RODATA_G_STCMOSLSC_07e0: .ascii "\x7f\0|\0",
_RODATA_G_STCMOSLSC_07e4: .ascii "\x83\0\x8f\0",
_RODATA_G_STCMOSLSC_07e8: .ascii "\x94\0\xa6\0",
_RODATA_G_STCMOSLSC_07ec: .ascii "\xb7\0\xcc\0",
_RODATA_G_STCMOSLSC_07f0: .ascii "\xe0\0\xf6\0",
_RODATA_G_STCMOSLSC_07f4: .ascii "\x15\x013\x01]\x01\x89\x01\xc1\x01\x04\x02?\x02-\x02\xf1\x01\xbb\x01\x87\x01W\x012\x01\x18\x01\xfe\0\xe6\0",
_RODATA_G_STCMOSLSC_0814: .ascii "\xd1\0\xc3\0",
_RODATA_G_STCMOSLSC_0818: .ascii "\xb3\0\xa7\0",
_RODATA_G_STCMOSLSC_081c: .ascii "\x98\0\x91\0",
_RODATA_G_STCMOSLSC_0820: .ascii "\x8b\0\x8e\0",
_RODATA_G_STCMOSLSC_0824: .ascii "\x8d\0\x91\0",
_RODATA_G_STCMOSLSC_0828: .ascii "\x9b\0\xa6\0",
_RODATA_G_STCMOSLSC_082c: .ascii "\xb5\0\xc5\0",
_RODATA_G_STCMOSLSC_0830: .ascii "\xd9\0\xf1\0",
_RODATA_G_STCMOSLSC_0834: .ascii "\x07\x01\"\x01D\x01k\x01\x9d\x01\xd5\x01\x18\x02R\x02D\x02\x04\x02\xc8\x01\x8e\x01g\x017\x01'\x01\x08\x01\xf5\0\xdd\0",
_RODATA_G_STCMOSLSC_0858: .ascii "\xcc\0\xbf\0",
_RODATA_G_STCMOSLSC_085c: .ascii "\xb1\0\xa6\0",
_RODATA_G_STCMOSLSC_0860: .ascii "\x9f\0\x9d\0",
_RODATA_G_STCMOSLSC_0864: .ascii "\x99\0\x9a\0",
_RODATA_G_STCMOSLSC_0868: .ascii "\x98\0\xa5\0",
_RODATA_G_STCMOSLSC_086c: .ascii "\xb3\0\xc0\0",
_RODATA_G_STCMOSLSC_0870: .ascii "\xce\0\xe2\0",
_RODATA_G_STCMOSLSC_0874: .ascii "\xf7\0\x0b\x01.\x01U\x01z\x01\xa6\x01\xe9\x01#\x02b\x02|\x02C\x02\xfd\x01\xbe\x01\x8b\x01b\x018\x01\x14\x01\xfb\0",
_RODATA_G_STCMOSLSC_0898: .ascii "\xe1\0\xd2\0",
_RODATA_G_STCMOSLSC_089c: .ascii "\xbb\0\xb1\0",
_RODATA_G_STCMOSLSC_08a0: .ascii "\xa9\0\xa3\0",
_RODATA_G_STCMOSLSC_08a4: .ascii "\x9e\0\x9d\0",
_RODATA_G_STCMOSLSC_08a8: .ascii "\x9d\0\xa0\0",
_RODATA_G_STCMOSLSC_08ac: .ascii "\xb1\0\xb7\0",
_RODATA_G_STCMOSLSC_08b0: .ascii "\xc3\0\xd8\0",
_RODATA_G_STCMOSLSC_08b4: .ascii "\xea\0\x1d\x01Z\x01A\x01p\x01\xa4\x01\xd8\x01\x1c\x02h\x02\xab\x02e\x02.\x02\xe8\x01\xb1\x01~\x01S\x01(\x01\x07\x01\xef\0\xd9\0",
_RODATA_G_STCMOSLSC_08dc: .ascii "\xc6\0\xb3\0",
_RODATA_G_STCMOSLSC_08e0: .ascii "\xa6\0\x9b\0",
_RODATA_G_STCMOSLSC_08e4: .ascii "\x94\0\x90\0",
_RODATA_G_STCMOSLSC_08e8: .ascii "\x91\0\x94\0",
_RODATA_G_STCMOSLSC_08ec: .ascii "\x96\0\xa0\0",
_RODATA_G_STCMOSLSC_08f0: .ascii "\xac\0\xb7\0",
_RODATA_G_STCMOSLSC_08f4: .ascii "\xcc\0\xdd\0",
_RODATA_G_STCMOSLSC_08f8: .ascii "\x01\x01=\x017\x01e\x01\x93\x01\xc5\x01\x06\x02S\x02\x97\x02J\x02\x17\x02\xd0\x01\x97\x01d\x01=\x01\x13\x01\xf4\0\xde\0",
_RODATA_G_STCMOSLSC_091c: .ascii "\xc8\0\xb4\0",
_RODATA_G_STCMOSLSC_0920: .ascii "\xa7\0\x96\0",
_RODATA_G_STCMOSLSC_0924: .ascii "\x8c\0\x83\0",
_RODATA_G_STCMOSLSC_0928: .ascii "~\0~\0",
_RODATA_G_STCMOSLSC_092c: .ascii "\x81\0\x84\0",
_RODATA_G_STCMOSLSC_0930: .ascii "\x8f\0\x9a\0",
_RODATA_G_STCMOSLSC_0934: .ascii "\xa9\0\xbb\0",
_RODATA_G_STCMOSLSC_0938: .ascii "\xcd\0\xe8\0",
_RODATA_G_STCMOSLSC_093c: .ascii "\x0f\x01(\x01P\x01z\x01\xb1\x01\xec\x016\x02t\x02/\x02\xfb\x01\xbb\x01\x81\x01T\x01+\x01\x08\x01\xe8\0",
_RODATA_G_STCMOSLSC_095c: .ascii "\xd0\0\xba\0",
_RODATA_G_STCMOSLSC_0960: .ascii "\xa6\0\x96\0",
_RODATA_G_STCMOSLSC_0964: .ascii "\x87\0{\0",
_RODATA_G_STCMOSLSC_0968: .ascii "t\0o\0",
_RODATA_G_STCMOSLSC_096c: .ascii "n\0n\0",
_RODATA_G_STCMOSLSC_0970: .ascii "u\0\x7f\0",
_RODATA_G_STCMOSLSC_0974: .ascii "\x8c\0\x98\0",
_RODATA_G_STCMOSLSC_0978: .ascii "\xab\0\xc2\0",
_RODATA_G_STCMOSLSC_097c: .ascii "\xd9\0\xf3\0",
_RODATA_G_STCMOSLSC_0980: .ascii "\x16\x01;\x01h\x01\x9d\x01\xd6\x01 \x02\\x02\x1c\x02\xe9\x01\xa8\x01s\x01@\x01\x1b\x01\xf8\0",
_RODATA_G_STCMOSLSC_099c: .ascii "\xde\0\xc5\0",
_RODATA_G_STCMOSLSC_09a0: .ascii "\xaf\0\x9b\0",
_RODATA_G_STCMOSLSC_09a4: .ascii "\x88\0w\0",
_RODATA_G_STCMOSLSC_09a8: .ascii "m\0d\0",
_RODATA_G_STCMOSLSC_09ac: .ascii "^\0]\0",
_RODATA_G_STCMOSLSC_09b0: .ascii "a\0g\0",
_RODATA_G_STCMOSLSC_09b4: .ascii "o\0\x80\0",
_RODATA_G_STCMOSLSC_09b8: .ascii "\x8b\0\x9f\0",
_RODATA_G_STCMOSLSC_09bc: .ascii "\xb6\0\xca\0",
_RODATA_G_STCMOSLSC_09c0: .ascii "\xe7\0\x05\x01-\x01Z\x01\x8d\x01\xc1\x01\n\x02N\x02\t\x02\xd3\x01\x99\x01d\x014\x01\x0e\x01\xee\0\xd2\0",
_RODATA_G_STCMOSLSC_09e0: .ascii "\xba\0\xa4\0",
_RODATA_G_STCMOSLSC_09e4: .ascii "\x8e\0|\0",
_RODATA_G_STCMOSLSC_09e8: .ascii "k\0a\0",
_RODATA_G_STCMOSLSC_09ec: .ascii "X\0S\0",
_RODATA_G_STCMOSLSC_09f0: .ascii "P\0T\0",
_RODATA_G_STCMOSLSC_09f4: .ascii "Z\0d\0",
_RODATA_G_STCMOSLSC_09f8: .ascii "p\0\x80\0",
_RODATA_G_STCMOSLSC_09fc: .ascii "\x92\0\xa8\0",
_RODATA_G_STCMOSLSC_0a00: .ascii "\xc1\0\xde\0",
_RODATA_G_STCMOSLSC_0a04: .ascii "\xfa\0\x1d\x01G\x01{\x01\xb7\x01\xf7\x018\x02\xf6\x01\xc5\x01\x8b\x01T\x01'\x01\x01\x01\xe6\0",
_RODATA_G_STCMOSLSC_0a20: .ascii "\xc7\0\xaf\0",
_RODATA_G_STCMOSLSC_0a24: .ascii "\x97\0\x82\0",
_RODATA_G_STCMOSLSC_0a28: .ascii "p\0b\0",
_RODATA_G_STCMOSLSC_0a2c: .ascii "T\0K\0",
_RODATA_G_STCMOSLSC_0a30: .ascii "D\0F\0",
_RODATA_G_STCMOSLSC_0a34: .ascii "G\0N\0",
_RODATA_G_STCMOSLSC_0a38: .ascii "V\0d\0",
_RODATA_G_STCMOSLSC_0a3c: .ascii "t\0\x85\0",
_RODATA_G_STCMOSLSC_0a40: .ascii "\x9d\0\xb6\0",
_RODATA_G_STCMOSLSC_0a44: .ascii "\xd1\0\xee\0",
_RODATA_G_STCMOSLSC_0a48: .ascii "\x10\x01<\x01j\x01\xa6\x01\xe6\x01-\x02\xed\x01\xb6\x01y\x01H\x01\x1c\x01\xfe\0",
_RODATA_G_STCMOSLSC_0a60: .ascii "\xda\0\xbc\0",
_RODATA_G_STCMOSLSC_0a64: .ascii "\xa7\0\x8e\0",
_RODATA_G_STCMOSLSC_0a68: .ascii "x\0e\0",
_RODATA_G_STCMOSLSC_0a6c: .ascii "U\0J\0",
_RODATA_G_STCMOSLSC_0a70: .ascii "A\08\0",
_RODATA_G_STCMOSLSC_0a74: .ascii "9\0:\0",
_RODATA_G_STCMOSLSC_0a78: .ascii "C\0L\0",
_RODATA_G_STCMOSLSC_0a7c: .ascii "Z\0h\0",
_RODATA_G_STCMOSLSC_0a80: .ascii "~\0\x94\0",
_RODATA_G_STCMOSLSC_0a84: .ascii "\xab\0\xc7\0",
_RODATA_G_STCMOSLSC_0a88: .ascii "\xe5\0\x03\x010\x01Z\x01\x93\x01\xdb\x01\x1b\x02\xde\x01\xad\x01p\x01;\x01\x18\x01\xf0\0\xd3\0",
_RODATA_G_STCMOSLSC_0aa4: .ascii "\xb3\0\x9d\0",
_RODATA_G_STCMOSLSC_0aa8: .ascii "\x85\0o\0",
_RODATA_G_STCMOSLSC_0aac: .ascii "\\0L\0",
_RODATA_G_STCMOSLSC_0ab0: .ascii "=\05\0",
_RODATA_G_STCMOSLSC_0ab4: .ascii "-\0*\0",
_RODATA_G_STCMOSLSC_0ab8: .ascii ".\07\0",
_RODATA_G_STCMOSLSC_0abc: .ascii "C\0O\0",
_RODATA_G_STCMOSLSC_0ac0: .ascii "a\0v\0",
_RODATA_G_STCMOSLSC_0ac4: .ascii "\x8b\0\xa3\0",
_RODATA_G_STCMOSLSC_0ac8: .ascii "\xbc\0\xdb\0",
_RODATA_G_STCMOSLSC_0acc: .ascii "\xf9\0#\x01S\x01\x87\x01\xcc\x01\x0b\x02\xca\x01\x9f\x01e\x010\x01\x0c\x01\xea\0",
_RODATA_G_STCMOSLSC_0ae4: .ascii "\xcd\0\xaf\0",
_RODATA_G_STCMOSLSC_0ae8: .ascii "\x95\0}\0",
_RODATA_G_STCMOSLSC_0aec: .ascii "e\0S\0",
_RODATA_G_STCMOSLSC_0af0: .ascii "F\06\0",
_RODATA_G_STCMOSLSC_0af4: .ascii "(\0$\0",
_RODATA_G_STCMOSLSC_0af8: .ascii "\"\0%\0",
_RODATA_G_STCMOSLSC_0afc: .ascii ".\08\0",
_RODATA_G_STCMOSLSC_0b00: .ascii "H\0X\0",
_RODATA_G_STCMOSLSC_0b04: .ascii "l\0\x80\0",
_RODATA_G_STCMOSLSC_0b08: .ascii "\x9f\0\xb4\0",
_RODATA_G_STCMOSLSC_0b0c: .ascii "\xd3\0\xf4\0",
_RODATA_G_STCMOSLSC_0b10: .ascii "\x1e\x01K\x01\x81\x01\xc3\x01\xfc\x01\xca\x01\x97\x01\\x01)\x01\x07\x01\xe6\0\xc6\0",
_RODATA_G_STCMOSLSC_0b28: .ascii "\xa8\0\x8e\0",
_RODATA_G_STCMOSLSC_0b2c: .ascii "v\0]\0",
_RODATA_G_STCMOSLSC_0b30: .ascii "M\0<\0",
_RODATA_G_STCMOSLSC_0b34: .ascii ",\0!\0",
_RODATA_G_STCMOSLSC_0b38: .ascii "\x18\0\x19\0",
_RODATA_G_STCMOSLSC_0b3c: .ascii "\x1c\0'\0",
_RODATA_G_STCMOSLSC_0b40: .ascii "/\0A\0",
_RODATA_G_STCMOSLSC_0b44: .ascii "R\0h\0",
_RODATA_G_STCMOSLSC_0b48: .ascii "y\0\x94\0",
_RODATA_G_STCMOSLSC_0b4c: .ascii "\xb2\0\xce\0",
_RODATA_G_STCMOSLSC_0b50: .ascii "\xee\0\x16\x01I\x01z\x01\xb9\x01\xfa\x01\xbe\x01\x8c\x01V\x01#\x01\0\x01\xdc\0",
_RODATA_G_STCMOSLSC_0b68: .ascii "\xbe\0\xa3\0",
_RODATA_G_STCMOSLSC_0b6c: .ascii "\x89\0o\0",
_RODATA_G_STCMOSLSC_0b70: .ascii "Z\0F\0",
_RODATA_G_STCMOSLSC_0b74: .ascii "3\0%\0",
_RODATA_G_STCMOSLSC_0b78: .ascii "\x19\0\x0f\0",
_RODATA_G_STCMOSLSC_0b7c: .ascii "\x0e\0\x16\0",
_RODATA_G_STCMOSLSC_0b80: .ascii "\x1d\0(\0",
_RODATA_G_STCMOSLSC_0b84: .ascii "9\0I\0",
_RODATA_G_STCMOSLSC_0b88: .ascii "_\0u\0",
_RODATA_G_STCMOSLSC_0b8c: .ascii "\x8d\0\xaa\0",
_RODATA_G_STCMOSLSC_0b90: .ascii "\xc7\0\xe9\0",
_RODATA_G_STCMOSLSC_0b94: .ascii "\x0f\x01<\x01r\x01\xb3\x01\xec\x01\xb4\x01\x88\x01Q\x01!\x01\xfc\0",
_RODATA_G_STCMOSLSC_0ba8: .ascii "\xde\0\xbd\0",
_RODATA_G_STCMOSLSC_0bac: .ascii "\x9e\0\x85\0",
_RODATA_G_STCMOSLSC_0bb0: .ascii "k\0T\0",
_RODATA_G_STCMOSLSC_0bb4: .ascii "@\01\0",
_RODATA_G_STCMOSLSC_0bb8: .ascii "\x1e\0\x12\0",
_RODATA_G_STCMOSLSC_0bbc: .ascii "\x0b\0\t\0",
_RODATA_G_STCMOSLSC_0bc0: .ascii "\x0f\0\x16\0",
_RODATA_G_STCMOSLSC_0bc4: .ascii "$\04\0",
_RODATA_G_STCMOSLSC_0bc8: .ascii "D\0\\0",
_RODATA_G_STCMOSLSC_0bcc: .ascii "p\0\x89\0",
_RODATA_G_STCMOSLSC_0bd0: .ascii "\xa6\0\xc6\0",
_RODATA_G_STCMOSLSC_0bd4: .ascii "\xe5\0\n\x017\x01k\x01\xac\x01\xe5\x01\xb1\x01\x82\x01H\x01\x1d\x01\xfa\0\xdb\0",
_RODATA_G_STCMOSLSC_0bec: .ascii "\xb9\0\x9d\0",
_RODATA_G_STCMOSLSC_0bf0: .ascii "\x81\0h\0",
_RODATA_G_STCMOSLSC_0bf4: .ascii "R\0>\0",
_RODATA_G_STCMOSLSC_0bf8: .ascii ",\0\x1b\0",
_RODATA_G_STCMOSLSC_0bfc: .ascii "\x0c\0\x06\0",
_RODATA_G_STCMOSLSC_0c00: .ascii "\x07\0\t\0",
_RODATA_G_STCMOSLSC_0c04: .ascii "\x11\0 \0",
_RODATA_G_STCMOSLSC_0c08: .ascii "1\0A\0",
_RODATA_G_STCMOSLSC_0c0c: .ascii "Y\0n\0",
_RODATA_G_STCMOSLSC_0c10: .ascii "\x88\0\xa2\0",
_RODATA_G_STCMOSLSC_0c14: .ascii "\xc1\0\xdf\0",
_RODATA_G_STCMOSLSC_0c18: .ascii "\x06\x014\x01e\x01\xa6\x01\xde\x01\xae\x01}\x01G\x01\x19\x01\xf7\0",
_RODATA_G_STCMOSLSC_0c2c: .ascii "\xd6\0\xb7\0",
_RODATA_G_STCMOSLSC_0c30: .ascii "\x9a\0\x80\0",
_RODATA_G_STCMOSLSC_0c34: .ascii "g\0S\0",
_RODATA_G_STCMOSLSC_0c38: .ascii ":\0)\0",
_RODATA_G_STCMOSLSC_0c3c: .ascii "\x18\0\n\0",
_RODATA_G_STCMOSLSC_0c40: .ascii "\x03\0\x04\0",
_RODATA_G_STCMOSLSC_0c44: .ascii "\x06\0\x12\0",
_RODATA_G_STCMOSLSC_0c48: .ascii "\x1a\0,\0",
_RODATA_G_STCMOSLSC_0c4c: .ascii "?\0V\0",
_RODATA_G_STCMOSLSC_0c50: .ascii "m\0\x86\0",
_RODATA_G_STCMOSLSC_0c54: .ascii "\xa1\0\xc1\0",
_RODATA_G_STCMOSLSC_0c58: .ascii "\xde\0\x03\x01.\x01f\x01\xa2\x01\xdc\x01\xa3\x01\x7f\x01G\x01\x1c\x01\xf3\0\xd5\0",
_RODATA_G_STCMOSLSC_0c70: .ascii "\xb4\0\x98\0",
_RODATA_G_STCMOSLSC_0c74: .ascii "}\0c\0",
_RODATA_G_STCMOSLSC_0c78: .ascii "N\0;\0",
_RODATA_G_STCMOSLSC_0c7c: .ascii ")\0\x13\0",
_RODATA_G_STCMOSLSC_0c80: .ascii "\n\0\x01\0",
_RODATA_G_STCMOSLSC_0c84: .ascii "\0\0\x04\0",
_RODATA_G_STCMOSLSC_0c88: .ascii "\x0e\0\x18\0",
_RODATA_G_STCMOSLSC_0c8c: .ascii "+\0@\0",
_RODATA_G_STCMOSLSC_0c90: .ascii "U\0o\0",
_RODATA_G_STCMOSLSC_0c94: .ascii "\x84\0\xa3\0",
_RODATA_G_STCMOSLSC_0c98: .ascii "\xbf\0\xdf\0",
_RODATA_G_STCMOSLSC_0c9c: .ascii "\x02\x01,\x01c\x01\xa0\x01\xda\x01\xac\x01|\x01D\x01\x16\x01\xf5\0",
_RODATA_G_STCMOSLSC_0cb0: .ascii "\xd6\0\xb5\0",
_RODATA_G_STCMOSLSC_0cb4: .ascii "\x98\0}\0",
_RODATA_G_STCMOSLSC_0cb8: .ascii "c\0L\0",
_RODATA_G_STCMOSLSC_0cbc: .ascii ";\07\0",
_RODATA_G_STCMOSLSC_0cc0: .ascii "\x14\0\x07\0",
_RODATA_G_STCMOSLSC_0cc4: .ascii "\0\0\x01\0",
_RODATA_G_STCMOSLSC_0cc8: .ascii "\x05\0\x0c\0",
_RODATA_G_STCMOSLSC_0ccc: .ascii "\x1a\0*\0",
_RODATA_G_STCMOSLSC_0cd0: .ascii "@\0U\0",
_RODATA_G_STCMOSLSC_0cd4: .ascii "m\0\x83\0",
_RODATA_G_STCMOSLSC_0cd8: .ascii "\xa1\0\xbe\0",
_RODATA_G_STCMOSLSC_0cdc: .ascii "\xdf\0\x01\x01/\x01f\x01\xa0\x01\xd7\x01\xa7\x01}\x01F\x01\x18\x01\xf8\0\xd4\0",
_RODATA_G_STCMOSLSC_0cf4: .ascii "\xb6\0\x98\0",
_RODATA_G_STCMOSLSC_0cf8: .ascii "\x80\0h\0",
_RODATA_G_STCMOSLSC_0cfc: .ascii "R\0?\0",
_RODATA_G_STCMOSLSC_0d00: .ascii ":\0\x18\0",
_RODATA_G_STCMOSLSC_0d04: .ascii "\t\0\x01\0",
_RODATA_G_STCMOSLSC_0d08: .ascii "\x02\0\x06\0",
_RODATA_G_STCMOSLSC_0d0c: .ascii "\x0e\0\x1a\0",
_RODATA_G_STCMOSLSC_0d10: .ascii ",\0@\0",
_RODATA_G_STCMOSLSC_0d14: .ascii "T\0l\0",
_RODATA_G_STCMOSLSC_0d18: .ascii "\x88\0\xa0\0",
_RODATA_G_STCMOSLSC_0d1c: .ascii "\xc2\0\xe0\0",
_RODATA_G_STCMOSLSC_0d20: .ascii "\x03\x01-\x01`\x01\x9f\x01\xd5\x01\xaa\x01|\x01E\x01\x19\x01\xf9\0",
_RODATA_G_STCMOSLSC_0d34: .ascii "\xd9\0\xb6\0",
_RODATA_G_STCMOSLSC_0d38: .ascii "\x9b\0\x82\0",
_RODATA_G_STCMOSLSC_0d3c: .ascii "k\0R\0",
_RODATA_G_STCMOSLSC_0d40: .ascii ">\0:\0",
_RODATA_G_STCMOSLSC_0d44: .ascii "\x1b\0\x0b\0",
_RODATA_G_STCMOSLSC_0d48: .ascii "\x03\0\x05\0",
_RODATA_G_STCMOSLSC_0d4c: .ascii "\x07\0\r\0",
_RODATA_G_STCMOSLSC_0d50: .ascii "\x1c\0-\0",
_RODATA_G_STCMOSLSC_0d54: .ascii "C\0W\0",
_RODATA_G_STCMOSLSC_0d58: .ascii "l\0\x88\0",
_RODATA_G_STCMOSLSC_0d5c: .ascii "\xa1\0\xc0\0",
_RODATA_G_STCMOSLSC_0d60: .ascii "\xe3\0\x02\x01.\x01b\x01\xa0\x01\xdc\x01\xb0\x01\x7f\x01H\x01\x1f\x01\xfd\0\xd8\0",
_RODATA_G_STCMOSLSC_0d78: .ascii "\xb8\0\x9c\0",
_RODATA_G_STCMOSLSC_0d7c: .ascii "\x83\0m\0",
_RODATA_G_STCMOSLSC_0d80: .ascii "U\0?\0",
_RODATA_G_STCMOSLSC_0d84: .ascii "-\0\x1e\0",
_RODATA_G_STCMOSLSC_0d88: .ascii "\x10\0\x07\0",
_RODATA_G_STCMOSLSC_0d8c: .ascii "\x07\0\x0b\0",
_RODATA_G_STCMOSLSC_0d90: .ascii "\x13\0 \0",
_RODATA_G_STCMOSLSC_0d94: .ascii "1\0F\0",
_RODATA_G_STCMOSLSC_0d98: .ascii "[\0r\0",
_RODATA_G_STCMOSLSC_0d9c: .ascii "\x8c\0\xa7\0",
_RODATA_G_STCMOSLSC_0da0: .ascii "\xc6\0\xe2\0",
_RODATA_G_STCMOSLSC_0da4: .ascii "\x07\x011\x01g\x01\xa3\x01\xdf\x01\xb4\x01\x85\x01M\x01$\x01\xfe\0",
_RODATA_G_STCMOSLSC_0db8: .ascii "\xde\0\xbc\0",
_RODATA_G_STCMOSLSC_0dbc: .ascii "\xa3\0\x87\0",
_RODATA_G_STCMOSLSC_0dc0: .ascii "n\0\\0",
_RODATA_G_STCMOSLSC_0dc4: .ascii "F\06\0",
_RODATA_G_STCMOSLSC_0dc8: .ascii "$\0\x16\0",
_RODATA_G_STCMOSLSC_0dcc: .ascii "\x0f\0\x0f\0",
_RODATA_G_STCMOSLSC_0dd0: .ascii "\x13\0\x19\0",
_RODATA_G_STCMOSLSC_0dd4: .ascii "$\06\0",
_RODATA_G_STCMOSLSC_0dd8: .ascii "I\0_\0",
_RODATA_G_STCMOSLSC_0ddc: .ascii "w\0\x8f\0",
_RODATA_G_STCMOSLSC_0de0: .ascii "\xad\0\xca\0",
_RODATA_G_STCMOSLSC_0de4: .ascii "\xe5\0\r\x019\x01k\x01\xa6\x01\xe2\x01\xb7\x01\x89\x01R\x01'\x01\x03\x01\xe2\0",
_RODATA_G_STCMOSLSC_0dfc: .ascii "\xc3\0\xa6\0",
_RODATA_G_STCMOSLSC_0e00: .ascii "\x8d\0v\0",
_RODATA_G_STCMOSLSC_0e04: .ascii "]\0L\0",
_RODATA_G_STCMOSLSC_0e08: .ascii "<\0,\0",
_RODATA_G_STCMOSLSC_0e0c: .ascii "\x1c\0\x18\0",
_RODATA_G_STCMOSLSC_0e10: .ascii "\x18\0\x18\0",
_RODATA_G_STCMOSLSC_0e14: .ascii "!\0+\0",
_RODATA_G_STCMOSLSC_0e18: .ascii "=\0N\0",
_RODATA_G_STCMOSLSC_0e1c: .ascii "d\0{\0",
_RODATA_G_STCMOSLSC_0e20: .ascii "\x96\0\xb0\0",
_RODATA_G_STCMOSLSC_0e24: .ascii "\xcf\0\xee\0",
_RODATA_G_STCMOSLSC_0e28: .ascii "\x11\x01<\x01q\x01\xaf\x01\xe7\x01\xbe\x01\x91\x01Z\x01-\x01\x0b\x01\xe3\0\xca\0",
_RODATA_G_STCMOSLSC_0e40: .ascii "\xac\0\x90\0",
_RODATA_G_STCMOSLSC_0e44: .ascii "z\0c\0",
_RODATA_G_STCMOSLSC_0e48: .ascii "W\0>\0",
_RODATA_G_STCMOSLSC_0e4c: .ascii "2\0&\0",
_RODATA_G_STCMOSLSC_0e50: .ascii " \0\x1f\0",
_RODATA_G_STCMOSLSC_0e54: .ascii "#\0+\0",
_RODATA_G_STCMOSLSC_0e58: .ascii "4\0D\0",
_RODATA_G_STCMOSLSC_0e5c: .ascii "W\0i\0",
_RODATA_G_STCMOSLSC_0e60: .ascii "\x83\0\x9c\0",
_RODATA_G_STCMOSLSC_0e64: .ascii "\xb4\0\xd2\0",
_RODATA_G_STCMOSLSC_0e68: .ascii "\xf7\0\x19\x01E\x01u\x01\xb8\x01\xf3\x01\xca\x01\x9a\x01b\x019\x01\x10\x01\xea\0",
_RODATA_G_STCMOSLSC_0e80: .ascii "\xcc\0\xb2\0",
_RODATA_G_STCMOSLSC_0e84: .ascii "\x98\0\x81\0",
_RODATA_G_STCMOSLSC_0e88: .ascii "l\0Y\0",
_RODATA_G_STCMOSLSC_0e8c: .ascii "J\0;\0",
_RODATA_G_STCMOSLSC_0e90: .ascii "2\0,\0",
_RODATA_G_STCMOSLSC_0e94: .ascii ")\0+\0",
_RODATA_G_STCMOSLSC_0e98: .ascii "/\0=\0",
_RODATA_G_STCMOSLSC_0e9c: .ascii "J\0[\0",
_RODATA_G_STCMOSLSC_0ea0: .ascii "s\0\x88\0",
_RODATA_G_STCMOSLSC_0ea4: .ascii "\xa2\0\xbc\0",
_RODATA_G_STCMOSLSC_0ea8: .ascii "\xd7\0\xfd\0",
_RODATA_G_STCMOSLSC_0eac: .ascii "\x1e\x01G\x01\x80\x01\xbf\x01\xfb\x01\xc9\x01\xa4\x01j\x01?\x01\x14\x01\xf4\0\xd5\0",
_RODATA_G_STCMOSLSC_0ec4: .ascii "\xba\0\x9e\0",
_RODATA_G_STCMOSLSC_0ec8: .ascii "\x8a\0w\0",
_RODATA_G_STCMOSLSC_0ecc: .ascii "c\0T\0",
_RODATA_G_STCMOSLSC_0ed0: .ascii "E\0:\0",
_RODATA_G_STCMOSLSC_0ed4: .ascii "6\03\0",
_RODATA_G_STCMOSLSC_0ed8: .ascii "5\0:\0",
_RODATA_G_STCMOSLSC_0edc: .ascii "H\0T\0",
_RODATA_G_STCMOSLSC_0ee0: .ascii "f\0y\0",
_RODATA_G_STCMOSLSC_0ee4: .ascii "\x8f\0\xa8\0",
_RODATA_G_STCMOSLSC_0ee8: .ascii "\xc6\0\xe1\0",
_RODATA_G_STCMOSLSC_0eec: .ascii "\x04\x01'\x01O\x01\x84\x01\xc8\x01\x03\x02\xe3\x01\xb0\x01w\x01E\x01\x1e\x01\xfd\0",
_RODATA_G_STCMOSLSC_0f04: .ascii "\xdc\0\xc3\0",
_RODATA_G_STCMOSLSC_0f08: .ascii "\xa5\0\x90\0",
_RODATA_G_STCMOSLSC_0f0c: .ascii "{\0m\0",
_RODATA_G_STCMOSLSC_0f10: .ascii "]\0P\0",
_RODATA_G_STCMOSLSC_0f14: .ascii "F\0B\0",
_RODATA_G_STCMOSLSC_0f18: .ascii "A\0@\0",
_RODATA_G_STCMOSLSC_0f1c: .ascii "G\0R\0",
_RODATA_G_STCMOSLSC_0f20: .ascii "a\0p\0",
_RODATA_G_STCMOSLSC_0f24: .ascii "\x85\0\x9b\0",
_RODATA_G_STCMOSLSC_0f28: .ascii "\xb3\0\xce\0",
_RODATA_G_STCMOSLSC_0f2c: .ascii "\xe7\0\t\x011\x01\\x01\x92\x01\xd1\x01\x13\x02\xed\x01\xbf\x01\x86\x01R\x01*\x01\x08\x01\xe7\0",
_RODATA_G_STCMOSLSC_0f48: .ascii "\xcd\0\xb1\0",
_RODATA_G_STCMOSLSC_0f4c: .ascii "\x9c\0\x87\0",
_RODATA_G_STCMOSLSC_0f50: .ascii "x\0j\0",
_RODATA_G_STCMOSLSC_0f54: .ascii "]\0U\0",
_RODATA_G_STCMOSLSC_0f58: .ascii "O\0L\0",
_RODATA_G_STCMOSLSC_0f5c: .ascii "M\0T\0",
_RODATA_G_STCMOSLSC_0f60: .ascii "\\0m\0",
_RODATA_G_STCMOSLSC_0f64: .ascii "}\0\x8f\0",
_RODATA_G_STCMOSLSC_0f68: .ascii "\xa4\0\xbb\0",
_RODATA_G_STCMOSLSC_0f6c: .ascii "\xd7\0\xf4\0",
_RODATA_G_STCMOSLSC_0f70: .ascii "\x12\x017\x01i\x01\xa0\x01\xde\x01\x1a\x02\xfa\x01\xd2\x01\x90\x01`\x013\x01\x10\x01\xf2\0\xd9\0",
_RODATA_G_STCMOSLSC_0f8c: .ascii "\xbb\0\xa7\0",
_RODATA_G_STCMOSLSC_0f90: .ascii "\x96\0\x84\0",
_RODATA_G_STCMOSLSC_0f94: .ascii "u\0h\0",
_RODATA_G_STCMOSLSC_0f98: .ascii "^\0\\0",
_RODATA_G_STCMOSLSC_0f9c: .ascii "Z\0\\0",
_RODATA_G_STCMOSLSC_0fa0: .ascii "`\0m\0",
_RODATA_G_STCMOSLSC_0fa4: .ascii "w\0\x8a\0",
_RODATA_G_STCMOSLSC_0fa8: .ascii "\x99\0\xad\0",
_RODATA_G_STCMOSLSC_0fac: .ascii "\xc6\0\xe1\0",
_RODATA_G_STCMOSLSC_0fb0: .ascii "\xfd\0\x1d\x01F\x01s\x01\xab\x01\xeb\x01*\x02\r\x02\xdd\x01\xa0\x01o\x01@\x01\x1a\x01\xfc\0",
_RODATA_G_STCMOSLSC_0fcc: .ascii "\xe5\0\xc7\0",
_RODATA_G_STCMOSLSC_0fd0: .ascii "\xb3\0\xa3\0",
_RODATA_G_STCMOSLSC_0fd4: .ascii "\x8f\0\x81\0",
_RODATA_G_STCMOSLSC_0fd8: .ascii "u\0m\0",
_RODATA_G_STCMOSLSC_0fdc: .ascii "i\0f\0",
_RODATA_G_STCMOSLSC_0fe0: .ascii "i\0q\0",
_RODATA_G_STCMOSLSC_0fe4: .ascii "x\0\x85\0",
_RODATA_G_STCMOSLSC_0fe8: .ascii "\x93\0\xa5\0",
_RODATA_G_STCMOSLSC_0fec: .ascii "\xbb\0\xd3\0",
_RODATA_G_STCMOSLSC_0ff0: .ascii "\xec\0\x08\x01(\x01R\x01\x80\x01\xb9\x01\xfe\x01<\x02\x1e\x02\xee\x01\xb2\x01y\x01P\x01,\x01\x0c\x01\xef\0",
_RODATA_G_STCMOSLSC_1010: .ascii "\xd6\0\xc0\0",
_RODATA_G_STCMOSLSC_1014: .ascii "\xad\0\x9d\0",
_RODATA_G_STCMOSLSC_1018: .ascii "\x8f\0\x83\0",
_RODATA_G_STCMOSLSC_101c: .ascii "z\0y\0",
_RODATA_G_STCMOSLSC_1020: .ascii "t\0y\0",
_RODATA_G_STCMOSLSC_1024: .ascii "~\0\x86\0",
_RODATA_G_STCMOSLSC_1028: .ascii "\x91\0\x9f\0",
_RODATA_G_STCMOSLSC_102c: .ascii "\xb3\0\xcc\0",
_RODATA_G_STCMOSLSC_1030: .ascii "\xdf\0\xf8\0",
_RODATA_G_STCMOSLSC_1034: .ascii "\x14\x01;\x01b\x01\x8f\x01\xca\x01\x0e\x02N\x021\x02\x01\x02\xbf\x01\x8c\x01`\x017\x01\x19\x01\xfd\0\xe6\0",
_RODATA_G_STCMOSLSC_1054: .ascii "\xcd\0\xbd\0",
_RODATA_G_STCMOSLSC_1058: .ascii "\xad\0\x9e\0",
_RODATA_G_STCMOSLSC_105c: .ascii "\x94\0\x8a\0",
_RODATA_G_STCMOSLSC_1060: .ascii "\x88\0\x84\0",
_RODATA_G_STCMOSLSC_1064: .ascii "\x85\0\x8c\0",
_RODATA_G_STCMOSLSC_1068: .ascii "\x97\0\xa2\0",
_RODATA_G_STCMOSLSC_106c: .ascii "\xb0\0\xc5\0",
_RODATA_G_STCMOSLSC_1070: .ascii "\xd6\0\xf0\0",
_RODATA_G_STCMOSLSC_1074: .ascii "\x06\x01#\x01G\x01m\x01\xa1\x01\xda\x01\x1d\x02`\x02D\x02\x11\x02\xd4\x01\x9c\x01q\x01G\x01(\x01\x0e\x01\xf3\0\xdf\0",
_RODATA_G_STCMOSLSC_1098: .ascii "\xcc\0\xbc\0",
_RODATA_G_STCMOSLSC_109c: .ascii "\xaf\0\xa5\0",
_RODATA_G_STCMOSLSC_10a0: .ascii "\x9c\0\x96\0",
_RODATA_G_STCMOSLSC_10a4: .ascii "\x94\0\x97\0",
_RODATA_G_STCMOSLSC_10a8: .ascii "\x9c\0\xa7\0",
_RODATA_G_STCMOSLSC_10ac: .ascii "\xb0\0\xc1\0",
_RODATA_G_STCMOSLSC_10b0: .ascii "\xd4\0\xe7\0",
_RODATA_G_STCMOSLSC_10b4: .ascii "\xfd\0\x13\x011\x01V\x01\x81\x01\xaf\x01\xf1\x01:\x02v\x02\\x02$\x02\xe1\x01\xa9\x01\x7f\x01S\x016\x01\x17\x01\xfd\0",
_RODATA_G_STCMOSLSC_10d8: .ascii "\xeb\0\xda\0",
_RODATA_G_STCMOSLSC_10dc: .ascii "\xc5\0\xbe\0",
_RODATA_G_STCMOSLSC_10e0: .ascii "\xb2\0\xaa\0",
_RODATA_G_STCMOSLSC_10e4: .ascii "\xa2\0\x9f\0",
_RODATA_G_STCMOSLSC_10e8: .ascii "\xa3\0\xa9\0",
_RODATA_G_STCMOSLSC_10ec: .ascii "\xb5\0\xbb\0",
_RODATA_G_STCMOSLSC_10f0: .ascii "\xcc\0\xda\0",
_RODATA_G_STCMOSLSC_10f4: .ascii "\xf1\0\x04\x01\"\x01>\x01a\x01\x8e\x01\xc4\x01\xff\x01K\x02\x85\x02\x8b\x02H\x02\xfd\x01\xc1\x01\x8e\x01`\x019\x01\x15\x01\xfd\0\xe8\0",
_RODATA_G_STCMOSLSC_111c: .ascii "\xd7\0\xc4\0",
_RODATA_G_STCMOSLSC_1120: .ascii "\xb6\0\xad\0",
_RODATA_G_STCMOSLSC_1124: .ascii "\xa3\0\xa4\0",
_RODATA_G_STCMOSLSC_1128: .ascii "\xa4\0\xa3\0",
_RODATA_G_STCMOSLSC_112c: .ascii "\xa9\0\xb5\0",
_RODATA_G_STCMOSLSC_1130: .ascii "\xbc\0\xc8\0",
_RODATA_G_STCMOSLSC_1134: .ascii "\xde\0\xf4\0",
_RODATA_G_STCMOSLSC_1138: .ascii "&\x01f\x01M\x01z\x01\xa5\x01\xdd\x01\x1f\x02l\x02\xb1\x02q\x022\x02\xea\x01\xb0\x01\x80\x01R\x01+\x01\x0c\x01\xf2\0",
_RODATA_G_STCMOSLSC_115c: .ascii "\xdc\0\xca\0",
_RODATA_G_STCMOSLSC_1160: .ascii "\xb9\0\xab\0",
_RODATA_G_STCMOSLSC_1164: .ascii "\xa1\0\x9a\0",
_RODATA_G_STCMOSLSC_1168: .ascii "\x96\0\x96\0",
_RODATA_G_STCMOSLSC_116c: .ascii "\x99\0\xa0\0",
_RODATA_G_STCMOSLSC_1170: .ascii "\xa6\0\xb0\0",
_RODATA_G_STCMOSLSC_1174: .ascii "\xbf\0\xd2\0",
_RODATA_G_STCMOSLSC_1178: .ascii "\xe5\0\n\x01C\x01?\x01k\x01\x9c\x01\xcd\x01\x0e\x02V\x02\x9c\x02V\x02\x16\x02\xd6\x01\x9d\x01j\x01B\x01\x1d\x01\xfd\0",
_RODATA_G_STCMOSLSC_119c: .ascii "\xe4\0\xcc\0",
_RODATA_G_STCMOSLSC_11a0: .ascii "\xb9\0\xa9\0",
_RODATA_G_STCMOSLSC_11a4: .ascii "\x9b\0\x91\0",
_RODATA_G_STCMOSLSC_11a8: .ascii "\x87\0\x84\0",
_RODATA_G_STCMOSLSC_11ac: .ascii "\x83\0\x84\0",
_RODATA_G_STCMOSLSC_11b0: .ascii "\x8b\0\x94\0",
_RODATA_G_STCMOSLSC_11b4: .ascii "\xa0\0\xae\0",
_RODATA_G_STCMOSLSC_11b8: .ascii "\xc1\0\xd7\0",
_RODATA_G_STCMOSLSC_11bc: .ascii "\xec\0\x13\x01.\x01V\x01\x83\x01\xb7\x01\xf5\x01=\x02\x84\x02@\x02\x01\x02\xc3\x01\x85\x01U\x01-\x01\x0e\x01\xee\0\xd4\0",
_RODATA_G_STCMOSLSC_11e0: .ascii "\xbe\0\xa9\0",
_RODATA_G_STCMOSLSC_11e4: .ascii "\x9b\0\x8b\0",
_RODATA_G_STCMOSLSC_11e8: .ascii "~\0y\0",
_RODATA_G_STCMOSLSC_11ec: .ascii "t\0s\0",
_RODATA_G_STCMOSLSC_11f0: .ascii "t\0z\0",
_RODATA_G_STCMOSLSC_11f4: .ascii "\x88\0\x92\0",
_RODATA_G_STCMOSLSC_11f8: .ascii "\x9d\0\xb2\0",
_RODATA_G_STCMOSLSC_11fc: .ascii "\xc8\0\xe1\0",
_RODATA_G_STCMOSLSC_1200: .ascii "\xfc\0\x1c\x01C\x01r\x01\xa1\x01\xdd\x01&\x02f\x02)\x02\xeb\x01\xad\x01v\x01B\x01\x1f\x01\xfb\0\xe1\0",
_RODATA_G_STCMOSLSC_1220: .ascii "\xc9\0\xb0\0",
_RODATA_G_STCMOSLSC_1224: .ascii "\x9f\0\x89\0",
_RODATA_G_STCMOSLSC_1228: .ascii "|\0q\0",
_RODATA_G_STCMOSLSC_122c: .ascii "j\0d\0",
_RODATA_G_STCMOSLSC_1230: .ascii "b\0e\0",
_RODATA_G_STCMOSLSC_1234: .ascii "l\0t\0",
_RODATA_G_STCMOSLSC_1238: .ascii "\x84\0\x90\0",
_RODATA_G_STCMOSLSC_123c: .ascii "\xa4\0\xba\0",
_RODATA_G_STCMOSLSC_1240: .ascii "\xd2\0\xed\0",
_RODATA_G_STCMOSLSC_1244: .ascii "\x0f\x012\x01_\x01\x8f\x01\xce\x01\x0f\x02R\x02\x1a\x02\xd7\x01\x9c\x01c\x016\x01\x10\x01\xf1\0",
_RODATA_G_STCMOSLSC_1260: .ascii "\xd5\0\xbc\0",
_RODATA_G_STCMOSLSC_1264: .ascii "\xa6\0\x92\0",
_RODATA_G_STCMOSLSC_1268: .ascii "\x81\0r\0",
_RODATA_G_STCMOSLSC_126c: .ascii "d\0Y\0",
_RODATA_G_STCMOSLSC_1270: .ascii "W\0X\0",
_RODATA_G_STCMOSLSC_1274: .ascii "Y\0^\0",
_RODATA_G_STCMOSLSC_1278: .ascii "i\0u\0",
_RODATA_G_STCMOSLSC_127c: .ascii "\x84\0\x97\0",
_RODATA_G_STCMOSLSC_1280: .ascii "\xad\0\xc5\0",
_RODATA_G_STCMOSLSC_1284: .ascii "\xe1\0\x02\x01$\x01Q\x01\x83\x01\xbc\x01\xff\x01;\x02\x01\x02\xc5\x01\x8a\x01U\x01)\x01\x06\x01\xe8\0\xcc\0",
_RODATA_G_STCMOSLSC_12a4: .ascii "\xb1\0\x9a\0",
_RODATA_G_STCMOSLSC_12a8: .ascii "\x86\0t\0",
_RODATA_G_STCMOSLSC_12ac: .ascii "g\0X\0",
_RODATA_G_STCMOSLSC_12b0: .ascii "M\0K\0",
_RODATA_G_STCMOSLSC_12b4: .ascii "K\0J\0",
_RODATA_G_STCMOSLSC_12b8: .ascii "Q\0\\0",
_RODATA_G_STCMOSLSC_12bc: .ascii "j\0|\0",
_RODATA_G_STCMOSLSC_12c0: .ascii "\x8b\0\xa4\0",
_RODATA_G_STCMOSLSC_12c4: .ascii "\xbc\0\xd5\0",
_RODATA_G_STCMOSLSC_12c8: .ascii "\xf9\0\x15\x01@\x01u\x01\xac\x01\xef\x01)\x02\xf5\x01\xb9\x01~\x01K\x01\x1d\x01\xfc\0\xde\0",
_RODATA_G_STCMOSLSC_12e4: .ascii "\xc0\0\xa9\0",
_RODATA_G_STCMOSLSC_12e8: .ascii "\x91\0|\0",
_RODATA_G_STCMOSLSC_12ec: .ascii "j\0Y\0",
_RODATA_G_STCMOSLSC_12f0: .ascii "M\0C\0",
_RODATA_G_STCMOSLSC_12f4: .ascii "?\0<\0",
_RODATA_G_STCMOSLSC_12f8: .ascii "?\0F\0",
_RODATA_G_STCMOSLSC_12fc: .ascii "O\0^\0",
_RODATA_G_STCMOSLSC_1300: .ascii "n\0\x84\0",
_RODATA_G_STCMOSLSC_1304: .ascii "\x99\0\xb0\0",
_RODATA_G_STCMOSLSC_1308: .ascii "\xc9\0\xeb\0",
_RODATA_G_STCMOSLSC_130c: .ascii "\t\x015\x01c\x01\x9e\x01\xde\x01\x1e\x02\xea\x01\xaf\x01v\x01B\x01\x16\x01\xf6\0",
_RODATA_G_STCMOSLSC_1324: .ascii "\xd2\0\xb7\0",
_RODATA_G_STCMOSLSC_1328: .ascii "\x9e\0\x87\0",
_RODATA_G_STCMOSLSC_132c: .ascii "s\0`\0",
_RODATA_G_STCMOSLSC_1330: .ascii "N\0C\0",
_RODATA_G_STCMOSLSC_1334: .ascii "8\01\0",
_RODATA_G_STCMOSLSC_1338: .ascii "/\02\0",
_RODATA_G_STCMOSLSC_133c: .ascii "<\0E\0",
_RODATA_G_STCMOSLSC_1340: .ascii "T\0e\0",
_RODATA_G_STCMOSLSC_1344: .ascii "z\0\x91\0",
_RODATA_G_STCMOSLSC_1348: .ascii "\xaa\0\xc3\0",
_RODATA_G_STCMOSLSC_134c: .ascii "\xe0\0\x01\x01'\x01X\x01\x8f\x01\xd1\x01\x11\x02\xde\x01\xa2\x01h\x017\x01\x10\x01\xee\0\xcd\0",
_RODATA_G_STCMOSLSC_1368: .ascii "\xb1\0\x98\0",
_RODATA_G_STCMOSLSC_136c: .ascii "\x7f\0g\0",
_RODATA_G_STCMOSLSC_1370: .ascii "V\0D\0",
_RODATA_G_STCMOSLSC_1374: .ascii "9\0.\0",
_RODATA_G_STCMOSLSC_1378: .ascii "'\0'\0",
_RODATA_G_STCMOSLSC_137c: .ascii "*\03\0",
_RODATA_G_STCMOSLSC_1380: .ascii "=\0J\0",
_RODATA_G_STCMOSLSC_1384: .ascii "\\0q\0",
_RODATA_G_STCMOSLSC_1388: .ascii "\x87\0\xa3\0",
_RODATA_G_STCMOSLSC_138c: .ascii "\xba\0\xda\0",
_RODATA_G_STCMOSLSC_1390: .ascii "\xfa\0 \x01N\x01\x83\x01\xc6\x01\x08\x02\xce\x01\x99\x01`\x010\x01\x07\x01\xe8\0",
_RODATA_G_STCMOSLSC_13a8: .ascii "\xc8\0\xab\0",
_RODATA_G_STCMOSLSC_13ac: .ascii "\x94\0y\0",
_RODATA_G_STCMOSLSC_13b0: .ascii "_\0P\0",
_RODATA_G_STCMOSLSC_13b4: .ascii "=\0/\0",
_RODATA_G_STCMOSLSC_13b8: .ascii "\"\0\x1a\0",
_RODATA_G_STCMOSLSC_13bc: .ascii "\x1e\0\x1e\0",
_RODATA_G_STCMOSLSC_13c0: .ascii "(\02\0",
_RODATA_G_STCMOSLSC_13c4: .ascii "C\0U\0",
_RODATA_G_STCMOSLSC_13c8: .ascii "k\0\x7f\0",
_RODATA_G_STCMOSLSC_13cc: .ascii "\x9a\0\xb4\0",
_RODATA_G_STCMOSLSC_13d0: .ascii "\xd1\0\xf3\0",
_RODATA_G_STCMOSLSC_13d4: .ascii "\x1c\x01G\x01}\x01\xc0\x01\xfa\x01\xc8\x01\x91\x01Y\x01*\x01\x01\x01\xe1\0\xc3\0",
_RODATA_G_STCMOSLSC_13ec: .ascii "\xa4\0\x8c\0",
_RODATA_G_STCMOSLSC_13f0: .ascii "s\0[\0",
_RODATA_G_STCMOSLSC_13f4: .ascii "H\04\0",
_RODATA_G_STCMOSLSC_13f8: .ascii "(\0\x1c\0",
_RODATA_G_STCMOSLSC_13fc: .ascii "\x13\0\x13\0",
_RODATA_G_STCMOSLSC_1400: .ascii "\x14\0!\0",
_RODATA_G_STCMOSLSC_1404: .ascii "+\0;\0",
_RODATA_G_STCMOSLSC_1408: .ascii "L\0e\0",
_RODATA_G_STCMOSLSC_140c: .ascii "z\0\x92\0",
_RODATA_G_STCMOSLSC_1410: .ascii "\xae\0\xcf\0",
_RODATA_G_STCMOSLSC_1414: .ascii "\xeb\0\x16\x01E\x01x\x01\xb7\x01\xf6\x01\xbf\x01\x88\x01S\x01\x1e\x01\x01\x01\xde\0",
_RODATA_G_STCMOSLSC_142c: .ascii "\xbe\0\xa0\0",
_RODATA_G_STCMOSLSC_1430: .ascii "\x86\0l\0",
_RODATA_G_STCMOSLSC_1434: .ascii "T\0C\0",
_RODATA_G_STCMOSLSC_1438: .ascii "/\0 \0",
_RODATA_G_STCMOSLSC_143c: .ascii "\x14\0\r\0",
_RODATA_G_STCMOSLSC_1440: .ascii "\x0c\0\x10\0",
_RODATA_G_STCMOSLSC_1444: .ascii "\x1b\0&\0",
_RODATA_G_STCMOSLSC_1448: .ascii "6\0F\0",
_RODATA_G_STCMOSLSC_144c: .ascii "`\0u\0",
_RODATA_G_STCMOSLSC_1450: .ascii "\x91\0\xa9\0",
_RODATA_G_STCMOSLSC_1454: .ascii "\xc9\0\xea\0",
_RODATA_G_STCMOSLSC_1458: .ascii "\x10\x01<\x01o\x01\xb0\x01\xeb\x01\xb8\x01\x81\x01M\x01!\x01\xfc\0",
_RODATA_G_STCMOSLSC_146c: .ascii "\xdb\0\xb8\0",
_RODATA_G_STCMOSLSC_1470: .ascii "\x9d\0\x85\0",
_RODATA_G_STCMOSLSC_1474: .ascii "g\0T\0",
_RODATA_G_STCMOSLSC_1478: .ascii ">\0.\0",
_RODATA_G_STCMOSLSC_147c: .ascii "\x1c\0\x0f\0",
_RODATA_G_STCMOSLSC_1480: .ascii "\x07\0\x07\0",
_RODATA_G_STCMOSLSC_1484: .ascii "\n\0\x15\0",
_RODATA_G_STCMOSLSC_1488: .ascii "!\02\0",
_RODATA_G_STCMOSLSC_148c: .ascii "F\0Z\0",
_RODATA_G_STCMOSLSC_1490: .ascii "r\0\x8b\0",
_RODATA_G_STCMOSLSC_1494: .ascii "\xa8\0\xc7\0",
_RODATA_G_STCMOSLSC_1498: .ascii "\xe4\0\x0c\x017\x01k\x01\xab\x01\xe5\x01\xbf\x01\x7f\x01J\x01\x1e\x01\xfa\0\xd6\0",
_RODATA_G_STCMOSLSC_14b0: .ascii "\xb8\0\x9d\0",
_RODATA_G_STCMOSLSC_14b4: .ascii "\x81\0i\0",
_RODATA_G_STCMOSLSC_14b8: .ascii "O\0<\0",
_RODATA_G_STCMOSLSC_14bc: .ascii "(\0\x19\0",
_RODATA_G_STCMOSLSC_14c0: .ascii "\x0b\0\x04\0",
_RODATA_G_STCMOSLSC_14c4: .ascii "\x03\0\t\0",
_RODATA_G_STCMOSLSC_14c8: .ascii "\x11\0\x1d\0",
_RODATA_G_STCMOSLSC_14cc: .ascii ".\0C\0",
_RODATA_G_STCMOSLSC_14d0: .ascii "V\0o\0",
_RODATA_G_STCMOSLSC_14d4: .ascii "\x8a\0\xa4\0",
_RODATA_G_STCMOSLSC_14d8: .ascii "\xc6\0\xe2\0",
_RODATA_G_STCMOSLSC_14dc: .ascii "\n\x019\x01j\x01\xad\x01\xe2\x01\xb5\x01\x7f\x01G\x01\x1c\x01\xf9\0",
_RODATA_G_STCMOSLSC_14f0: .ascii "\xd7\0\xb6\0",
_RODATA_G_STCMOSLSC_14f4: .ascii "\x97\0}\0",
_RODATA_G_STCMOSLSC_14f8: .ascii "g\0O\0",
_RODATA_G_STCMOSLSC_14fc: .ascii ":\0,\0",
_RODATA_G_STCMOSLSC_1500: .ascii "\x16\0\x08\0",
_RODATA_G_STCMOSLSC_1504: .ascii "\x03\0\x03\0",
_RODATA_G_STCMOSLSC_1508: .ascii "\x07\0\x12\0",
_RODATA_G_STCMOSLSC_150c: .ascii "\x1b\0.\0",
_RODATA_G_STCMOSLSC_1510: .ascii "A\0W\0",
_RODATA_G_STCMOSLSC_1514: .ascii "q\0\x8b\0",
_RODATA_G_STCMOSLSC_1518: .ascii "\xa7\0\xc3\0",
_RODATA_G_STCMOSLSC_151c: .ascii "\xe0\0\t\x017\x01h\x01\xa5\x01\xdb\x01\xb1\x01\x7f\x01I\x01\x1b\x01\xfa\0\xd4\0",
_RODATA_G_STCMOSLSC_1534: .ascii "\xb8\0\x96\0",
_RODATA_G_STCMOSLSC_1538: .ascii "\x7f\0f\0",
_RODATA_G_STCMOSLSC_153c: .ascii "P\0=\0",
_RODATA_G_STCMOSLSC_1540: .ascii "7\0\x16\0",
_RODATA_G_STCMOSLSC_1544: .ascii "\x08\0\x01\0",
_RODATA_G_STCMOSLSC_1548: .ascii "\0\0\x06\0",
_RODATA_G_STCMOSLSC_154c: .ascii "\x0f\0\x18\0",
_RODATA_G_STCMOSLSC_1550: .ascii ".\0B\0",
_RODATA_G_STCMOSLSC_1554: .ascii "W\0o\0",
_RODATA_G_STCMOSLSC_1558: .ascii "\x89\0\xa4\0",
_RODATA_G_STCMOSLSC_155c: .ascii "\xc2\0\xe3\0",
_RODATA_G_STCMOSLSC_1560: .ascii "\x07\x015\x01j\x01\xa3\x01\xdf\x01\xb2\x01\x7f\x01I\x01\x1b\x01\xf8\0",
_RODATA_G_STCMOSLSC_1574: .ascii "\xd8\0\xb9\0",
_RODATA_G_STCMOSLSC_1578: .ascii "\x9a\0\x7f\0",
_RODATA_G_STCMOSLSC_157c: .ascii "g\0R\0",
_RODATA_G_STCMOSLSC_1580: .ascii "A\0<\0",
_RODATA_G_STCMOSLSC_1584: .ascii "\x16\0\x0b\0",
_RODATA_G_STCMOSLSC_1588: .ascii "\x03\0\x03\0",
_RODATA_G_STCMOSLSC_158c: .ascii "\x07\0\x0f\0",
_RODATA_G_STCMOSLSC_1590: .ascii "\x19\00\0",
_RODATA_G_STCMOSLSC_1594: .ascii "C\0X\0",
_RODATA_G_STCMOSLSC_1598: .ascii "m\0\x88\0",
_RODATA_G_STCMOSLSC_159c: .ascii "\xa3\0\xc6\0",
_RODATA_G_STCMOSLSC_15a0: .ascii "\xe4\0\x07\x014\x01g\x01\xa2\x01\xdd\x01\xb9\x01\x80\x01H\x01 \x01\xf9\0\xd9\0",
_RODATA_G_STCMOSLSC_15b8: .ascii "\xba\0\x9c\0",
_RODATA_G_STCMOSLSC_15bc: .ascii "\x82\0h\0",
_RODATA_G_STCMOSLSC_15c0: .ascii "T\0?\0",
_RODATA_G_STCMOSLSC_15c4: .ascii "6\0\x19\0",
_RODATA_G_STCMOSLSC_15c8: .ascii "\r\0\x05\0",
_RODATA_G_STCMOSLSC_15cc: .ascii "\x05\0\t\0",
_RODATA_G_STCMOSLSC_15d0: .ascii "\x10\0\x1e\0",
_RODATA_G_STCMOSLSC_15d4: .ascii "/\0C\0",
_RODATA_G_STCMOSLSC_15d8: .ascii "Z\0p\0",
_RODATA_G_STCMOSLSC_15dc: .ascii "\x8b\0\xa6\0",
_RODATA_G_STCMOSLSC_15e0: .ascii "\xc5\0\xe5\0",
_RODATA_G_STCMOSLSC_15e4: .ascii "\n\x016\x01g\x01\xa4\x01\xdd\x01\xb9\x01\x83\x01K\x01\"\x01\xfe\0",
_RODATA_G_STCMOSLSC_15f8: .ascii "\xda\0\xbc\0",
_RODATA_G_STCMOSLSC_15fc: .ascii "\x9d\0\x84\0",
_RODATA_G_STCMOSLSC_1600: .ascii "j\0V\0",
_RODATA_G_STCMOSLSC_1604: .ascii "A\0.\0",
_RODATA_G_STCMOSLSC_1608: .ascii "\x1e\0\x0f\0",
_RODATA_G_STCMOSLSC_160c: .ascii "\x0b\0\n\0",
_RODATA_G_STCMOSLSC_1610: .ascii "\x0c\0\x14\0",
_RODATA_G_STCMOSLSC_1614: .ascii "#\04\0",
_RODATA_G_STCMOSLSC_1618: .ascii "J\0^\0",
_RODATA_G_STCMOSLSC_161c: .ascii "t\0\x8d\0",
_RODATA_G_STCMOSLSC_1620: .ascii "\xab\0\xc6\0",
_RODATA_G_STCMOSLSC_1624: .ascii "\xe7\0\n\x019\x01n\x01\xa8\x01\xe0\x01\xc0\x01\x8b\x01P\x01#\x01\0\x01\xdd\0",
_RODATA_G_STCMOSLSC_163c: .ascii "\xbf\0\xa5\0",
_RODATA_G_STCMOSLSC_1640: .ascii "\x86\0m\0",
_RODATA_G_STCMOSLSC_1644: .ascii "Z\0F\0",
_RODATA_G_STCMOSLSC_1648: .ascii "4\0$\0",
_RODATA_G_STCMOSLSC_164c: .ascii "\x16\0\x11\0",
_RODATA_G_STCMOSLSC_1650: .ascii "\x10\0\x12\0",
_RODATA_G_STCMOSLSC_1654: .ascii "\x18\0&\0",
_RODATA_G_STCMOSLSC_1658: .ascii "9\0L\0",
_RODATA_G_STCMOSLSC_165c: .ascii "a\0z\0",
_RODATA_G_STCMOSLSC_1660: .ascii "\x91\0\xae\0",
_RODATA_G_STCMOSLSC_1664: .ascii "\xcb\0\xec\0",
_RODATA_G_STCMOSLSC_1668: .ascii "\x15\x01>\x01q\x01\xab\x01\xe9\x01\xc5\x01\x8d\x01U\x01*\x01\x06\x01\xe3\0\xc2\0",
_RODATA_G_STCMOSLSC_1680: .ascii "\xa6\0\x8c\0",
_RODATA_G_STCMOSLSC_1684: .ascii "v\0]\0",
_RODATA_G_STCMOSLSC_1688: .ascii "L\0<\0",
_RODATA_G_STCMOSLSC_168c: .ascii ",\0\x1f\0",
_RODATA_G_STCMOSLSC_1690: .ascii "\x19\0\x18\0",
_RODATA_G_STCMOSLSC_1694: .ascii "\x1a\0\"\0",
_RODATA_G_STCMOSLSC_1698: .ascii "0\0@\0",
_RODATA_G_STCMOSLSC_169c: .ascii "Q\0f\0",
_RODATA_G_STCMOSLSC_16a0: .ascii "\x81\0\x96\0",
_RODATA_G_STCMOSLSC_16a4: .ascii "\xb3\0\xd1\0",
_RODATA_G_STCMOSLSC_16a8: .ascii "\xf1\0\x17\x01A\x01t\x01\xb3\x01\xed\x01\xd0\x01\x94\x01]\x012\x01\x0b\x01\xe7\0",
_RODATA_G_STCMOSLSC_16c0: .ascii "\xc9\0\xac\0",
_RODATA_G_STCMOSLSC_16c4: .ascii "\x93\0|\0",
_RODATA_G_STCMOSLSC_16c8: .ascii "f\0S\0",
_RODATA_G_STCMOSLSC_16cc: .ascii "C\05\0",
_RODATA_G_STCMOSLSC_16d0: .ascii "&\0\"\0",
_RODATA_G_STCMOSLSC_16d4: .ascii "\x1f\0#\0",
_RODATA_G_STCMOSLSC_16d8: .ascii ")\07\0",
_RODATA_G_STCMOSLSC_16dc: .ascii "F\0V\0",
_RODATA_G_STCMOSLSC_16e0: .ascii "m\0\x84\0",
_RODATA_G_STCMOSLSC_16e4: .ascii "\x9f\0\xba\0",
_RODATA_G_STCMOSLSC_16e8: .ascii "\xd4\0\xf7\0",
_RODATA_G_STCMOSLSC_16ec: .ascii "\x1a\x01H\x01}\x01\xbb\x01\xf3\x01\xd3\x01\x9c\x01f\x018\x01\x13\x01\xed\0\xce\0",
_RODATA_G_STCMOSLSC_1704: .ascii "\xb2\0\x98\0",
_RODATA_G_STCMOSLSC_1708: .ascii "\x82\0p\0",
_RODATA_G_STCMOSLSC_170c: .ascii "X\0H\0",
_RODATA_G_STCMOSLSC_1710: .ascii "=\02\0",
_RODATA_G_STCMOSLSC_1714: .ascii "+\0*\0",
_RODATA_G_STCMOSLSC_1718: .ascii ",\04\0",
_RODATA_G_STCMOSLSC_171c: .ascii "@\0P\0",
_RODATA_G_STCMOSLSC_1720: .ascii "_\0u\0",
_RODATA_G_STCMOSLSC_1724: .ascii "\x8b\0\xa3\0",
_RODATA_G_STCMOSLSC_1728: .ascii "\xc0\0\xdb\0",
_RODATA_G_STCMOSLSC_172c: .ascii "\xfe\0\"\x01N\x01\x84\x01\xc5\x01\x05\x02\xe1\x01\xa7\x01n\x01>\x01\x1a\x01\xf5\0",
_RODATA_G_STCMOSLSC_1744: .ascii "\xd6\0\xbb\0",
_RODATA_G_STCMOSLSC_1748: .ascii "\x9d\0\x87\0",
_RODATA_G_STCMOSLSC_174c: .ascii "v\0b\0",
_RODATA_G_STCMOSLSC_1750: .ascii "T\0E\0",
_RODATA_G_STCMOSLSC_1754: .ascii "<\07\0",
_RODATA_G_STCMOSLSC_1758: .ascii "5\07\0",
_RODATA_G_STCMOSLSC_175c: .ascii "@\0J\0",
_RODATA_G_STCMOSLSC_1760: .ascii "Y\0i\0",
_RODATA_G_STCMOSLSC_1764: .ascii "{\0\x93\0",
_RODATA_G_STCMOSLSC_1768: .ascii "\xad\0\xc5\0",
_RODATA_G_STCMOSLSC_176c: .ascii "\xe4\0\x01\x01(\x01S\x01\x87\x01\xcd\x01\n\x02\xf0\x01\xb3\x01w\x01F\x01 \x01\xfe\0\xdc\0",
_RODATA_G_STCMOSLSC_1788: .ascii "\xc5\0\xa8\0",
_RODATA_G_STCMOSLSC_178c: .ascii "\x8f\0}\0",
_RODATA_G_STCMOSLSC_1790: .ascii "l\0]\0",
_RODATA_G_STCMOSLSC_1794: .ascii "O\0G\0",
_RODATA_G_STCMOSLSC_1798: .ascii "A\0?\0",
_RODATA_G_STCMOSLSC_179c: .ascii "D\0I\0",
_RODATA_G_STCMOSLSC_17a0: .ascii "T\0b\0",
_RODATA_G_STCMOSLSC_17a4: .ascii "t\0\x86\0",
_RODATA_G_STCMOSLSC_17a8: .ascii "\x9b\0\xb1\0",
_RODATA_G_STCMOSLSC_17ac: .ascii "\xd1\0\xf0\0",
_RODATA_G_STCMOSLSC_17b0: .ascii "\x0c\x012\x01^\x01\x97\x01\xd8\x01\x11\x02\0\x02\xc4\x01\x84\x01W\x01,\x01\x07\x01\xe8\0\xd0\0",
_RODATA_G_STCMOSLSC_17cc: .ascii "\xb3\0\x9a\0",
_RODATA_G_STCMOSLSC_17d0: .ascii "\x87\0v\0",
_RODATA_G_STCMOSLSC_17d4: .ascii "f\0Z\0",
_RODATA_G_STCMOSLSC_17d8: .ascii "Q\0O\0",
_RODATA_G_STCMOSLSC_17dc: .ascii "M\0P\0",
_RODATA_G_STCMOSLSC_17e0: .ascii "U\0^\0",
_RODATA_G_STCMOSLSC_17e4: .ascii "m\0\x7f\0",
_RODATA_G_STCMOSLSC_17e8: .ascii "\x93\0\xa3\0",
_RODATA_G_STCMOSLSC_17ec: .ascii "\xbd\0\xd9\0",
_RODATA_G_STCMOSLSC_17f0: .ascii "\xf9\0\x19\x01?\x01n\x01\xa2\x01\xe3\x01\x1d\x02\x0f\x02\xd0\x01\x93\x01c\x018\x01\x12\x01\xf0\0",
_RODATA_G_STCMOSLSC_180c: .ascii "\xd6\0\xbc\0",
_RODATA_G_STCMOSLSC_1810: .ascii "\xa5\0\x94\0",
_RODATA_G_STCMOSLSC_1814: .ascii "\x83\0s\0",
_RODATA_G_STCMOSLSC_1818: .ascii "i\0a\0",
_RODATA_G_STCMOSLSC_181c: .ascii "Z\0Y\0",
_RODATA_G_STCMOSLSC_1820: .ascii "]\0`\0",
_RODATA_G_STCMOSLSC_1824: .ascii "k\0y\0",
_RODATA_G_STCMOSLSC_1828: .ascii "\x8b\0\x9e\0",
_RODATA_G_STCMOSLSC_182c: .ascii "\xb4\0\xc7\0",
_RODATA_G_STCMOSLSC_1830: .ascii "\xe3\0\x04\x01!\x01I\x01{\x01\xae\x01\xf3\x01.\x02\x14\x02\xde\x01\xa4\x01o\x01E\x01\x1d\x01\xfd\0\xe1\0",
_RODATA_G_STCMOSLSC_1850: .ascii "\xc7\0\xb4\0",
_RODATA_G_STCMOSLSC_1854: .ascii "\x9f\0\x8f\0",
_RODATA_G_STCMOSLSC_1858: .ascii "\x82\0t\0",
_RODATA_G_STCMOSLSC_185c: .ascii "m\0g\0",
_RODATA_G_STCMOSLSC_1860: .ascii "f\0k\0",
_RODATA_G_STCMOSLSC_1864: .ascii "m\0z\0",
_RODATA_G_STCMOSLSC_1868: .ascii "\x86\0\x96\0",
_RODATA_G_STCMOSLSC_186c: .ascii "\xa8\0\xbf\0",
_RODATA_G_STCMOSLSC_1870: .ascii "\xd7\0\xef\0",
_RODATA_G_STCMOSLSC_1874: .ascii "\x0c\x01+\x01U\x01\x83\x01\xbd\x01\x03\x02F\x02,\x02\xf1\x01\xb2\x01\x7f\x01Q\x01,\x01\t\x01\xed\0\xd4\0",
_RODATA_G_STCMOSLSC_1894: .ascii "\xc2\0\xab\0",
_RODATA_G_STCMOSLSC_1898: .ascii "\x9f\0\x91\0",
_RODATA_G_STCMOSLSC_189c: .ascii "\x83\0z\0",
_RODATA_G_STCMOSLSC_18a0: .ascii "u\0x\0",
_RODATA_G_STCMOSLSC_18a4: .ascii "z\0~\0",
_RODATA_G_STCMOSLSC_18a8: .ascii "\x8a\0\x96\0",
_RODATA_G_STCMOSLSC_18ac: .ascii "\xa3\0\xb8\0",
_RODATA_G_STCMOSLSC_18b0: .ascii "\xcb\0\xe1\0",
_RODATA_G_STCMOSLSC_18b4: .ascii "\xfe\0\x1a\x01:\x01e\x01\x90\x01\xce\x01\x0f\x02R\x02B\x02\x01\x02\xc2\x01\x8d\x01a\x016\x01\x19\x01\xfd\0",
_RODATA_G_STCMOSLSC_18d4: .ascii "\xe4\0\xcc\0",
_RODATA_G_STCMOSLSC_18d8: .ascii "\xb9\0\xae\0",
_RODATA_G_STCMOSLSC_18dc: .ascii "\x9d\0\x94\0",
_RODATA_G_STCMOSLSC_18e0: .ascii "\x8f\0\x88\0",
_RODATA_G_STCMOSLSC_18e4: .ascii "\x83\0\x86\0",
_RODATA_G_STCMOSLSC_18e8: .ascii "\x8d\0\x9a\0",
_RODATA_G_STCMOSLSC_18ec: .ascii "\xa3\0\xb1\0",
_RODATA_G_STCMOSLSC_18f0: .ascii "\xc4\0\xd8\0",
_RODATA_G_STCMOSLSC_18f4: .ascii "\xf1\0\t\x01*\x01H\x01t\x01\xa4\x01\xe1\x01'\x02k\x02X\x02\x13\x02\xd4\x01\x9f\x01o\x01E\x01(\x01\n\x01\xf1\0",
_RODATA_G_STCMOSLSC_1918: .ascii "\xdc\0\xcc\0",
_RODATA_G_STCMOSLSC_191c: .ascii "\xbd\0\xb0\0",
_RODATA_G_STCMOSLSC_1920: .ascii "\xa2\0\x9d\0",
_RODATA_G_STCMOSLSC_1924: .ascii "\x98\0\x94\0",
_RODATA_G_STCMOSLSC_1928: .ascii "\x98\0\x9f\0",
_RODATA_G_STCMOSLSC_192c: .ascii "\xa8\0\xb2\0",
_RODATA_G_STCMOSLSC_1930: .ascii "\xc1\0\xd6\0",
_RODATA_G_STCMOSLSC_1934: .ascii "\xe9\0\x01\x01\x18\x018\x01Y\x01\x83\x01\xb8\x01\xf6\x01@\x02\x87\x02a\x02!\x02\xde\x01\xad\x01\x7f\x01P\x011\x01\x1b\x01\xfd\0\xe6\0",
_RODATA_G_STCMOSLSC_195c: .ascii "\xd8\0\xcc\0",
_RODATA_G_STCMOSLSC_1960: .ascii "\xbc\0\xa9\0",
_RODATA_G_STCMOSLSC_1964: .ascii "\xac\0\xa0\0",
_RODATA_G_STCMOSLSC_1968: .ascii "\x9d\0\xa0\0",
_RODATA_G_STCMOSLSC_196c: .ascii "\xa9\0\xb5\0",
_RODATA_G_STCMOSLSC_1970: .ascii "\xbe\0\xcb\0",
_RODATA_G_STCMOSLSC_1974: .ascii "\xe0\0\xf7\0",
_RODATA_G_STCMOSLSC_1978: .ascii "\t\x01#\x01B\x01f\x01\x93\x01\xc9\x01\x08\x02O\x02\x8f\x02\x8e\x02H\x02\x02\x02\xbf\x01\x86\x01_\x01/\x01\x0c\x01\xfb\0",
_RODATA_G_STCMOSLSC_199c: .ascii "\xda\0\xcc\0",
_RODATA_G_STCMOSLSC_19a0: .ascii "\xbe\0\xb4\0",
_RODATA_G_STCMOSLSC_19a4: .ascii "\xa9\0\xa4\0",
_RODATA_G_STCMOSLSC_19a8: .ascii "\xa1\0\xa4\0",
_RODATA_G_STCMOSLSC_19ac: .ascii "\xa5\0\xa9\0",
_RODATA_G_STCMOSLSC_19b0: .ascii "\xb5\0\xba\0",
_RODATA_G_STCMOSLSC_19b4: .ascii "\xcd\0\xe2\0",
_RODATA_G_STCMOSLSC_19b8: .ascii "\xf4\0 \x01]\x01K\x01p\x01\xa5\x01\xd5\x01 \x02c\x02\xb6\x02|\x025\x02\xf1\x01\xaf\x01{\x01M\x01'\x01\x05\x01\xec\0\xd4\0",
_RODATA_G_STCMOSLSC_19e0: .ascii "\xc2\0\xb2\0",
_RODATA_G_STCMOSLSC_19e4: .ascii "\xa3\0\x96\0",
_RODATA_G_STCMOSLSC_19e8: .ascii "\x92\0\x95\0",
_RODATA_G_STCMOSLSC_19ec: .ascii "\x93\0\x93\0",
_RODATA_G_STCMOSLSC_19f0: .ascii "\x9c\0\xa3\0",
_RODATA_G_STCMOSLSC_19f4: .ascii "\xb1\0\xc0\0",
_RODATA_G_STCMOSLSC_19f8: .ascii "\xd1\0\xe4\0",
_RODATA_G_STCMOSLSC_19fc: .ascii "\x05\x01>\x019\x01b\x01\x93\x01\xcd\x01\x07\x02R\x02\x9e\x02Q\x02\"\x02\xd8\x01\x9f\x01n\x019\x01\x14\x01\xf4\0\xde\0",
_RODATA_G_STCMOSLSC_1a20: .ascii "\xc4\0\xb3\0",
_RODATA_G_STCMOSLSC_1a24: .ascii "\xa2\0\x92\0",
_RODATA_G_STCMOSLSC_1a28: .ascii "\x88\0\x83\0",
_RODATA_G_STCMOSLSC_1a2c: .ascii "\x82\0\x81\0",
_RODATA_G_STCMOSLSC_1a30: .ascii "\x7f\0\x88\0",
_RODATA_G_STCMOSLSC_1a34: .ascii "\x94\0\xa1\0",
_RODATA_G_STCMOSLSC_1a38: .ascii "\xac\0\xbe\0",
_RODATA_G_STCMOSLSC_1a3c: .ascii "\xd2\0\xec\0",
_RODATA_G_STCMOSLSC_1a40: .ascii "\x12\x01(\x01P\x01}\x01\xb0\x01\xf1\x019\x02t\x029\x02\x03\x02\xc2\x01\x8e\x01^\x01,\x01\x05\x01\xe9\0",
_RODATA_G_STCMOSLSC_1a60: .ascii "\xd1\0\xb9\0",
_RODATA_G_STCMOSLSC_1a64: .ascii "\xa7\0\x97\0",
_RODATA_G_STCMOSLSC_1a68: .ascii "\x86\0{\0",
_RODATA_G_STCMOSLSC_1a6c: .ascii "s\0k\0",
_RODATA_G_STCMOSLSC_1a70: .ascii "o\0k\0",
_RODATA_G_STCMOSLSC_1a74: .ascii "v\0\x84\0",
_RODATA_G_STCMOSLSC_1a78: .ascii "\x8f\0\x9e\0",
_RODATA_G_STCMOSLSC_1a7c: .ascii "\xaf\0\xc1\0",
_RODATA_G_STCMOSLSC_1a80: .ascii "\xdb\0\xf6\0",
_RODATA_G_STCMOSLSC_1a84: .ascii "\x18\x01=\x01e\x01\xa0\x01\xd8\x01\"\x02f\x02(\x02\xf5\x01\xac\x01u\x01D\x01\x1d\x01\xfc\0",
_RODATA_G_STCMOSLSC_1aa0: .ascii "\xe3\0\xc2\0",
_RODATA_G_STCMOSLSC_1aa4: .ascii "\xb0\0\x97\0",
_RODATA_G_STCMOSLSC_1aa8: .ascii "\x89\0|\0",
_RODATA_G_STCMOSLSC_1aac: .ascii "q\0g\0",
_RODATA_G_STCMOSLSC_1ab0: .ascii "`\0a\0",
_RODATA_G_STCMOSLSC_1ab4: .ascii "^\0g\0",
_RODATA_G_STCMOSLSC_1ab8: .ascii "t\0~\0",
_RODATA_G_STCMOSLSC_1abc: .ascii "\x8b\0\x9e\0",
_RODATA_G_STCMOSLSC_1ac0: .ascii "\xb5\0\xcf\0",
_RODATA_G_STCMOSLSC_1ac4: .ascii "\xe6\0\x07\x01+\x01U\x01\x8b\x01\xc9\x01\x12\x02R\x02\x08\x02\xd6\x01\x9a\x01a\x016\x01\n\x01\xed\0\xd9\0",
_RODATA_G_STCMOSLSC_1ae4: .ascii "\xbe\0\xa4\0",
_RODATA_G_STCMOSLSC_1ae8: .ascii "\x8d\0~\0",
_RODATA_G_STCMOSLSC_1aec: .ascii "n\0b\0",
_RODATA_G_STCMOSLSC_1af0: .ascii "\\0S\0",
_RODATA_G_STCMOSLSC_1af4: .ascii "S\0X\0",
_RODATA_G_STCMOSLSC_1af8: .ascii "[\0b\0",
_RODATA_G_STCMOSLSC_1afc: .ascii "o\0~\0",
_RODATA_G_STCMOSLSC_1b00: .ascii "\x91\0\xa8\0",
_RODATA_G_STCMOSLSC_1b04: .ascii "\xbe\0\xd3\0",
_RODATA_G_STCMOSLSC_1b08: .ascii "\xf5\0\x1b\x01D\x01z\x01\xb2\x01\xff\x01=\x02\xfa\x01\xca\x01\x83\x01U\x01*\x01\x04\x01\xe4\0",
_RODATA_G_STCMOSLSC_1b24: .ascii "\xcb\0\xb0\0",
_RODATA_G_STCMOSLSC_1b28: .ascii "\x9b\0\x83\0",
_RODATA_G_STCMOSLSC_1b2c: .ascii "u\0a\0",
_RODATA_G_STCMOSLSC_1b30: .ascii "W\0M\0",
_RODATA_G_STCMOSLSC_1b34: .ascii "K\0H\0",
_RODATA_G_STCMOSLSC_1b38: .ascii "H\0P\0",
_RODATA_G_STCMOSLSC_1b3c: .ascii "Y\0g\0",
_RODATA_G_STCMOSLSC_1b40: .ascii "x\0\x86\0",
_RODATA_G_STCMOSLSC_1b44: .ascii "\x9d\0\xb4\0",
_RODATA_G_STCMOSLSC_1b48: .ascii "\xcb\0\xec\0",
_RODATA_G_STCMOSLSC_1b4c: .ascii "\r\x013\x01e\x01\xa4\x01\xe5\x01,\x02\xec\x01\xb8\x01}\x01A\x01\x1b\x01\xf7\0",
_RODATA_G_STCMOSLSC_1b64: .ascii "\xda\0\xc1\0",
_RODATA_G_STCMOSLSC_1b68: .ascii "\xad\0\x94\0",
_RODATA_G_STCMOSLSC_1b6c: .ascii "z\0d\0",
_RODATA_G_STCMOSLSC_1b70: .ascii "Y\0N\0",
_RODATA_G_STCMOSLSC_1b74: .ascii "E\0>\0",
_RODATA_G_STCMOSLSC_1b78: .ascii "=\0;\0",
_RODATA_G_STCMOSLSC_1b7c: .ascii "C\0M\0",
_RODATA_G_STCMOSLSC_1b80: .ascii "[\0k\0",
_RODATA_G_STCMOSLSC_1b84: .ascii "\x81\0\x96\0",
_RODATA_G_STCMOSLSC_1b88: .ascii "\xaa\0\xc6\0",
_RODATA_G_STCMOSLSC_1b8c: .ascii "\xe0\0\x04\x01(\x01\\x01\x90\x01\xda\x01\x15\x02\xda\x01\xad\x01m\x019\x01\x13\x01\xed\0\xcd\0",
_RODATA_G_STCMOSLSC_1ba8: .ascii "\xb7\0\x9f\0",
_RODATA_G_STCMOSLSC_1bac: .ascii "\x89\0p\0",
_RODATA_G_STCMOSLSC_1bb0: .ascii "a\0N\0",
_RODATA_G_STCMOSLSC_1bb4: .ascii "D\0:\0",
_RODATA_G_STCMOSLSC_1bb8: .ascii "2\0/\0",
_RODATA_G_STCMOSLSC_1bbc: .ascii "4\0<\0",
_RODATA_G_STCMOSLSC_1bc0: .ascii "E\0Q\0",
_RODATA_G_STCMOSLSC_1bc4: .ascii "e\0u\0",
_RODATA_G_STCMOSLSC_1bc8: .ascii "\x8d\0\xa0\0",
_RODATA_G_STCMOSLSC_1bcc: .ascii "\xbc\0\xdb\0",
_RODATA_G_STCMOSLSC_1bd0: .ascii "\xf7\0\x1e\x01P\x01\x85\x01\xcc\x01\x01\x02\xd2\x01\xa1\x01]\x01-\x01\n\x01\xe8\0",
_RODATA_G_STCMOSLSC_1be8: .ascii "\xca\0\xb1\0",
_RODATA_G_STCMOSLSC_1bec: .ascii "\x92\0\x80\0",
_RODATA_G_STCMOSLSC_1bf0: .ascii "g\0W\0",
_RODATA_G_STCMOSLSC_1bf4: .ascii "I\08\0",
_RODATA_G_STCMOSLSC_1bf8: .ascii "/\0(\0",
_RODATA_G_STCMOSLSC_1bfc: .ascii "'\0)\0",
_RODATA_G_STCMOSLSC_1c00: .ascii "3\0<\0",
_RODATA_G_STCMOSLSC_1c04: .ascii "H\0]\0",
_RODATA_G_STCMOSLSC_1c08: .ascii "n\0\x85\0",
_RODATA_G_STCMOSLSC_1c0c: .ascii "\x9e\0\xb4\0",
_RODATA_G_STCMOSLSC_1c10: .ascii "\xd4\0\xf3\0",
_RODATA_G_STCMOSLSC_1c14: .ascii "\x19\x01H\x01\x7f\x01\xba\x01\xf8\x01\xc5\x01\x9b\x01]\x01)\x01\x01\x01\xe3\0\xc5\0",
_RODATA_G_STCMOSLSC_1c2c: .ascii "\xa7\0\x8b\0",
_RODATA_G_STCMOSLSC_1c30: .ascii "w\0[\0",
_RODATA_G_STCMOSLSC_1c34: .ascii "Q\0A\0",
_RODATA_G_STCMOSLSC_1c38: .ascii "4\0$\0",
_RODATA_G_STCMOSLSC_1c3c: .ascii "\x1c\0\x1e\0",
_RODATA_G_STCMOSLSC_1c40: .ascii "\"\0'\0",
_RODATA_G_STCMOSLSC_1c44: .ascii "3\0=\0",
_RODATA_G_STCMOSLSC_1c48: .ascii "T\0e\0",
_RODATA_G_STCMOSLSC_1c4c: .ascii "\x81\0\x93\0",
_RODATA_G_STCMOSLSC_1c50: .ascii "\xb2\0\xce\0",
_RODATA_G_STCMOSLSC_1c54: .ascii "\xeb\0\x14\x01F\x01y\x01\xb4\x01\xfc\x01\xb8\x01\x91\x01X\x01\"\x01\x03\x01\xde\0",
_RODATA_G_STCMOSLSC_1c6c: .ascii "\xbb\0\xa0\0",
_RODATA_G_STCMOSLSC_1c70: .ascii "\x8c\0q\0",
_RODATA_G_STCMOSLSC_1c74: .ascii "\\0I\0",
_RODATA_G_STCMOSLSC_1c78: .ascii "9\0*\0",
_RODATA_G_STCMOSLSC_1c7c: .ascii "\x1d\0\x16\0",
_RODATA_G_STCMOSLSC_1c80: .ascii "\x15\0\x1b\0",
_RODATA_G_STCMOSLSC_1c84: .ascii "!\0(\0",
_RODATA_G_STCMOSLSC_1c88: .ascii ":\0L\0",
_RODATA_G_STCMOSLSC_1c8c: .ascii "_\0z\0",
_RODATA_G_STCMOSLSC_1c90: .ascii "\x8d\0\xa9\0",
_RODATA_G_STCMOSLSC_1c94: .ascii "\xc7\0\xe4\0",
_RODATA_G_STCMOSLSC_1c98: .ascii "\x0c\x01=\x01r\x01\xb3\x01\xed\x01\xbc\x01\x89\x01P\x01 \x01\xfb\0",
_RODATA_G_STCMOSLSC_1cac: .ascii "\xd9\0\xbe\0",
_RODATA_G_STCMOSLSC_1cb0: .ascii "\xa0\0\x87\0",
_RODATA_G_STCMOSLSC_1cb4: .ascii "o\0W\0",
_RODATA_G_STCMOSLSC_1cb8: .ascii "C\00\0",
_RODATA_G_STCMOSLSC_1cbc: .ascii "$\0\x17\0",
_RODATA_G_STCMOSLSC_1cc0: .ascii "\x11\0\r\0",
_RODATA_G_STCMOSLSC_1cc4: .ascii "\x15\0\x18\0",
_RODATA_G_STCMOSLSC_1cc8: .ascii "$\07\0",
_RODATA_G_STCMOSLSC_1ccc: .ascii "F\0_\0",
_RODATA_G_STCMOSLSC_1cd0: .ascii "r\0\x8b\0",
_RODATA_G_STCMOSLSC_1cd4: .ascii "\xa6\0\xbf\0",
_RODATA_G_STCMOSLSC_1cd8: .ascii "\xe3\0\x05\x017\x01k\x01\xad\x01\xe9\x01\xb6\x01\x84\x01H\x01\x1c\x01\xf6\0\xd4\0",
_RODATA_G_STCMOSLSC_1cf0: .ascii "\xb6\0\x9a\0",
_RODATA_G_STCMOSLSC_1cf4: .ascii "\x87\0j\0",
_RODATA_G_STCMOSLSC_1cf8: .ascii "S\0>\0",
_RODATA_G_STCMOSLSC_1cfc: .ascii "*\0\x1e\0",
_RODATA_G_STCMOSLSC_1d00: .ascii "\x11\0\x07\0",
_RODATA_G_STCMOSLSC_1d04: .ascii "\t\0\x0b\0",
_RODATA_G_STCMOSLSC_1d08: .ascii "\x13\0!\0",
_RODATA_G_STCMOSLSC_1d0c: .ascii "4\0G\0",
_RODATA_G_STCMOSLSC_1d10: .ascii "[\0p\0",
_RODATA_G_STCMOSLSC_1d14: .ascii "\x8a\0\xa3\0",
_RODATA_G_STCMOSLSC_1d18: .ascii "\xc2\0\xe0\0",
_RODATA_G_STCMOSLSC_1d1c: .ascii "\x06\x012\x01h\x01\xab\x01\xe4\x01\xac\x01\x81\x01G\x01\x1a\x01\xf1\0",
_RODATA_G_STCMOSLSC_1d30: .ascii "\xd3\0\xb8\0",
_RODATA_G_STCMOSLSC_1d34: .ascii "\x98\0\x80\0",
_RODATA_G_STCMOSLSC_1d38: .ascii "e\0O\0",
_RODATA_G_STCMOSLSC_1d3c: .ascii "B\0)\0",
_RODATA_G_STCMOSLSC_1d40: .ascii "\x1b\0\x0f\0",
_RODATA_G_STCMOSLSC_1d44: .ascii "\x06\0\x05\0",
_RODATA_G_STCMOSLSC_1d48: .ascii "\x06\0\x10\0",
_RODATA_G_STCMOSLSC_1d4c: .ascii "\x1a\02\0",
_RODATA_G_STCMOSLSC_1d50: .ascii "D\0\\0",
_RODATA_G_STCMOSLSC_1d54: .ascii "r\0\x88\0",
_RODATA_G_STCMOSLSC_1d58: .ascii "\xa3\0\xc2\0",
_RODATA_G_STCMOSLSC_1d5c: .ascii "\xe4\0\x07\x011\x01`\x01\xa4\x01\xd4\x01\xad\x01|\x01>\x01\x12\x01\xf0\0\xd3\0",
_RODATA_G_STCMOSLSC_1d74: .ascii "\xb5\0\x94\0",
_RODATA_G_STCMOSLSC_1d78: .ascii "}\0f\0",
_RODATA_G_STCMOSLSC_1d7c: .ascii "R\0<\0",
_RODATA_G_STCMOSLSC_1d80: .ascii "*\0\x19\0",
_RODATA_G_STCMOSLSC_1d84: .ascii "\n\0\x02\0",
_RODATA_G_STCMOSLSC_1d88: .ascii "\0\0\x06\0",
_RODATA_G_STCMOSLSC_1d8c: .ascii "\x10\0\x1d\0",
_RODATA_G_STCMOSLSC_1d90: .ascii "-\0D\0",
_RODATA_G_STCMOSLSC_1d94: .ascii "W\0o\0",
_RODATA_G_STCMOSLSC_1d98: .ascii "\x8b\0\xa3\0",
_RODATA_G_STCMOSLSC_1d9c: .ascii "\xbd\0\xde\0",
_RODATA_G_STCMOSLSC_1da0: .ascii "\xff\01\x01`\x01\x9e\x01\xd8\x01\xaf\x01|\x01C\x01\x14\x01\xf7\0",
_RODATA_G_STCMOSLSC_1db4: .ascii "\xd3\0\xb4\0",
_RODATA_G_STCMOSLSC_1db8: .ascii "\x92\0}\0",
_RODATA_G_STCMOSLSC_1dbc: .ascii "d\0O\0",
_RODATA_G_STCMOSLSC_1dc0: .ascii ":\0:\0",
_RODATA_G_STCMOSLSC_1dc4: .ascii "\x17\0\x07\0",
_RODATA_G_STCMOSLSC_1dc8: .ascii "\x02\0\x01\0",
_RODATA_G_STCMOSLSC_1dcc: .ascii "\x05\0\x0c\0",
_RODATA_G_STCMOSLSC_1dd0: .ascii "\x18\0/\0",
_RODATA_G_STCMOSLSC_1dd4: .ascii "A\0W\0",
_RODATA_G_STCMOSLSC_1dd8: .ascii "p\0\x8b\0",
_RODATA_G_STCMOSLSC_1ddc: .ascii "\xa5\0\xc0\0",
_RODATA_G_STCMOSLSC_1de0: .ascii "\xdf\0\x02\x013\x01_\x01\xa3\x01\xd8\x01\xa8\x01}\x01I\x01\x19\x01\xf2\0\xd2\0",
_RODATA_G_STCMOSLSC_1df8: .ascii "\xb5\0\x92\0",
_RODATA_G_STCMOSLSC_1dfc: .ascii "~\0h\0",
_RODATA_G_STCMOSLSC_1e00: .ascii "N\0C\0",
_RODATA_G_STCMOSLSC_1e04: .ascii ";\0\x19\0",
_RODATA_G_STCMOSLSC_1e08: .ascii "\x0c\0\x03\0",
_RODATA_G_STCMOSLSC_1e0c: .ascii "\x03\0\x07\0",
_RODATA_G_STCMOSLSC_1e10: .ascii "\x11\0\x1d\0",
_RODATA_G_STCMOSLSC_1e14: .ascii "/\0E\0",
_RODATA_G_STCMOSLSC_1e18: .ascii "\\0s\0",
_RODATA_G_STCMOSLSC_1e1c: .ascii "\x8a\0\xa6\0",
_RODATA_G_STCMOSLSC_1e20: .ascii "\xc1\0\xe3\0",
_RODATA_G_STCMOSLSC_1e24: .ascii "\t\x011\x01^\x01\xa5\x01\xd8\x01\xa8\x01\x7f\x01G\x01\x19\x01\xf5\0",
_RODATA_G_STCMOSLSC_1e38: .ascii "\xd7\0\xb6\0",
_RODATA_G_STCMOSLSC_1e3c: .ascii "\x9b\0\x80\0",
_RODATA_G_STCMOSLSC_1e40: .ascii "h\0Q\0",
_RODATA_G_STCMOSLSC_1e44: .ascii "?\07\0",
_RODATA_G_STCMOSLSC_1e48: .ascii "\x1b\0\x0c\0",
_RODATA_G_STCMOSLSC_1e4c: .ascii "\x06\0\x05\0",
_RODATA_G_STCMOSLSC_1e50: .ascii "\n\0\x10\0",
_RODATA_G_STCMOSLSC_1e54: .ascii "\x1d\0/\0",
_RODATA_G_STCMOSLSC_1e58: .ascii "C\0Z\0",
_RODATA_G_STCMOSLSC_1e5c: .ascii "s\0\x8b\0",
_RODATA_G_STCMOSLSC_1e60: .ascii "\xa4\0\xc8\0",
_RODATA_G_STCMOSLSC_1e64: .ascii "\xe2\0\x08\x014\x01c\x01\xa7\x01\xda\x01\xb3\x01\x83\x01J\x01\x1d\x01\xf9\0\xd5\0",
_RODATA_G_STCMOSLSC_1e7c: .ascii "\xb9\0\x9f\0",
_RODATA_G_STCMOSLSC_1e80: .ascii "\x86\0j\0",
_RODATA_G_STCMOSLSC_1e84: .ascii "V\0?\0",
_RODATA_G_STCMOSLSC_1e88: .ascii "-\0!\0",
_RODATA_G_STCMOSLSC_1e8c: .ascii "\r\0\t\0",
_RODATA_G_STCMOSLSC_1e90: .ascii "\x07\0\x0e\0",
_RODATA_G_STCMOSLSC_1e94: .ascii "\x13\0\"\0",
_RODATA_G_STCMOSLSC_1e98: .ascii "1\0J\0",
_RODATA_G_STCMOSLSC_1e9c: .ascii "V\0v\0",
_RODATA_G_STCMOSLSC_1ea0: .ascii "\x8d\0\xaa\0",
_RODATA_G_STCMOSLSC_1ea4: .ascii "\xca\0\xe6\0",
_RODATA_G_STCMOSLSC_1ea8: .ascii "\x03\x017\x01g\x01\xa9\x01\xe3\x01\xbc\x01\x8b\x01Q\x01%\x01\xfb\0",
_RODATA_G_STCMOSLSC_1ebc: .ascii "\xd9\0\xba\0",
_RODATA_G_STCMOSLSC_1ec0: .ascii "\xa0\0\x8a\0",
_RODATA_G_STCMOSLSC_1ec4: .ascii "k\0Y\0",
_RODATA_G_STCMOSLSC_1ec8: .ascii "F\06\0",
_RODATA_G_STCMOSLSC_1ecc: .ascii "$\0\x15\0",
_RODATA_G_STCMOSLSC_1ed0: .ascii "\x10\0\x0f\0",
_RODATA_G_STCMOSLSC_1ed4: .ascii "\x12\0\x1b\0",
_RODATA_G_STCMOSLSC_1ed8: .ascii "'\08\0",
_RODATA_G_STCMOSLSC_1edc: .ascii "N\0b\0",
_RODATA_G_STCMOSLSC_1ee0: .ascii "u\0\x92\0",
_RODATA_G_STCMOSLSC_1ee4: .ascii "\xad\0\xc9\0",
_RODATA_G_STCMOSLSC_1ee8: .ascii "\xed\0\x0e\x01:\x01q\x01\xac\x01\xec\x01\xb9\x01\x90\x01U\x01*\x01\x06\x01\xde\0",
_RODATA_G_STCMOSLSC_1f00: .ascii "\xc4\0\xa3\0",
_RODATA_G_STCMOSLSC_1f04: .ascii "\x8a\0p\0",
_RODATA_G_STCMOSLSC_1f08: .ascii "]\0L\0",
_RODATA_G_STCMOSLSC_1f0c: .ascii "=\0+\0",
_RODATA_G_STCMOSLSC_1f10: .ascii "\x1f\0\x1a\0",
_RODATA_G_STCMOSLSC_1f14: .ascii "\x18\0\x19\0",
_RODATA_G_STCMOSLSC_1f18: .ascii "\"\0.\0",
_RODATA_G_STCMOSLSC_1f1c: .ascii "@\0R\0",
_RODATA_G_STCMOSLSC_1f20: .ascii "f\0\x81\0",
_RODATA_G_STCMOSLSC_1f24: .ascii "\x94\0\xad\0",
_RODATA_G_STCMOSLSC_1f28: .ascii "\xd3\0\xed\0",
_RODATA_G_STCMOSLSC_1f2c: .ascii "\x14\x01?\x01u\x01\xb9\x01\xe8\x01\xcd\x01\x99\x01V\x01+\x01\x05\x01\xe3\0\xc6\0",
_RODATA_G_STCMOSLSC_1f44: .ascii "\xa7\0\x8c\0",
_RODATA_G_STCMOSLSC_1f48: .ascii "w\0f\0",
_RODATA_G_STCMOSLSC_1f4c: .ascii "S\0E\0",
_RODATA_G_STCMOSLSC_1f50: .ascii "9\0'\0",
_RODATA_G_STCMOSLSC_1f54: .ascii " \0#\0",
_RODATA_G_STCMOSLSC_1f58: .ascii "!\0'\0",
_RODATA_G_STCMOSLSC_1f5c: .ascii "6\0G\0",
_RODATA_G_STCMOSLSC_1f60: .ascii "W\0o\0",
_RODATA_G_STCMOSLSC_1f64: .ascii "\x86\0\x9a\0",
_RODATA_G_STCMOSLSC_1f68: .ascii "\xb8\0\xd8\0",
_RODATA_G_STCMOSLSC_1f6c: .ascii "\xf6\0\x15\x01F\x01~\x01\xc0\x01\xee\x01\xd1\x01\x9d\x01e\x01/\x01\x0b\x01\xeb\0",
_RODATA_G_STCMOSLSC_1f84: .ascii "\xcb\0\xae\0",
_RODATA_G_STCMOSLSC_1f88: .ascii "\x97\0\x82\0",
_RODATA_G_STCMOSLSC_1f8c: .ascii "l\0\\0",
_RODATA_G_STCMOSLSC_1f90: .ascii "K\0@\0",
_RODATA_G_STCMOSLSC_1f94: .ascii "2\0*\0",
_RODATA_G_STCMOSLSC_1f98: .ascii "*\0-\0",
_RODATA_G_STCMOSLSC_1f9c: .ascii "3\0>\0",
_RODATA_G_STCMOSLSC_1fa0: .ascii "O\0a\0",
_RODATA_G_STCMOSLSC_1fa4: .ascii "u\0\x89\0",
_RODATA_G_STCMOSLSC_1fa8: .ascii "\xa1\0\xbe\0",
_RODATA_G_STCMOSLSC_1fac: .ascii "\xda\0\xf9\0",
_RODATA_G_STCMOSLSC_1fb0: .ascii "\x1d\x01K\x01\x84\x01\xc4\x01\x01\x02\xda\x01\xb4\x01l\x01:\x01\x10\x01\xf0\0\xd2\0",
_RODATA_G_STCMOSLSC_1fc8: .ascii "\xb6\0\x9e\0",
_RODATA_G_STCMOSLSC_1fcc: .ascii "\x8d\0x\0",
_RODATA_G_STCMOSLSC_1fd0: .ascii "e\0T\0",
_RODATA_G_STCMOSLSC_1fd4: .ascii "G\0=\0",
_RODATA_G_STCMOSLSC_1fd8: .ascii "6\08\0",
_RODATA_G_STCMOSLSC_1fdc: .ascii "8\0>\0",
_RODATA_G_STCMOSLSC_1fe0: .ascii "J\0X\0",
_RODATA_G_STCMOSLSC_1fe4: .ascii "i\0x\0",
_RODATA_G_STCMOSLSC_1fe8: .ascii "\x92\0\xad\0",
_RODATA_G_STCMOSLSC_1fec: .ascii "\xbf\0\xe3\0",
_RODATA_G_STCMOSLSC_1ff0: .ascii "\xff\0&\x01R\x01\x86\x01\xcd\x01\r\x02\xf1\x01\xbb\x01{\x01L\x01\x1c\x01\xf7\0",
_RODATA_G_STCMOSLSC_2008: .ascii "\xda\0\xc4\0",
_RODATA_G_STCMOSLSC_200c: .ascii "\xa6\0\x90\0",
_RODATA_G_STCMOSLSC_2010: .ascii "\x7f\0m\0",
_RODATA_G_STCMOSLSC_2014: .ascii "\\0Q\0",
_RODATA_G_STCMOSLSC_2018: .ascii "G\0@\0",
_RODATA_G_STCMOSLSC_201c: .ascii "C\0A\0",
_RODATA_G_STCMOSLSC_2020: .ascii "J\0U\0",
_RODATA_G_STCMOSLSC_2024: .ascii "c\0q\0",
_RODATA_G_STCMOSLSC_2028: .ascii "\x84\0\x9b\0",
_RODATA_G_STCMOSLSC_202c: .ascii "\xad\0\xca\0",
_RODATA_G_STCMOSLSC_2030: .ascii "\xe9\0\x06\x011\x01_\x01\x98\x01\xd8\x01\x15\x02\x01\x02\xce\x01\x8e\x01W\x01+\x01\x03\x01\xe2\0",
_RODATA_G_STCMOSLSC_204c: .ascii "\xc7\0\xb1\0",
_RODATA_G_STCMOSLSC_2050: .ascii "\x9b\0\x87\0",
_RODATA_G_STCMOSLSC_2054: .ascii "x\0g\0",
_RODATA_G_STCMOSLSC_2058: .ascii "[\0T\0",
_RODATA_G_STCMOSLSC_205c: .ascii "M\0Q\0",
_RODATA_G_STCMOSLSC_2060: .ascii "M\0X\0",
_RODATA_G_STCMOSLSC_2064: .ascii "_\0l\0",
_RODATA_G_STCMOSLSC_2068: .ascii "{\0\x93\0",
_RODATA_G_STCMOSLSC_206c: .ascii "\xa5\0\xb8\0",
_RODATA_G_STCMOSLSC_2070: .ascii "\xd6\0\xf4\0",
_RODATA_G_STCMOSLSC_2074: .ascii "\x11\x01;\x01j\x01\xa3\x01\xe4\x01)\x02\r\x02\xde\x01\x9d\x01d\x01:\x01\x0f\x01\xf2\0\xd1\0",
_RODATA_G_STCMOSLSC_2090: .ascii "\xbb\0\xa9\0",
_RODATA_G_STCMOSLSC_2094: .ascii "\x93\0\x7f\0",
_RODATA_G_STCMOSLSC_2098: .ascii "s\0j\0",
_RODATA_G_STCMOSLSC_209c: .ascii "c\0\\0",
_RODATA_G_STCMOSLSC_20a0: .ascii "Y\0_\0",
_RODATA_G_STCMOSLSC_20a4: .ascii "a\0m\0",
_RODATA_G_STCMOSLSC_20a8: .ascii "w\0\x8a\0",
_RODATA_G_STCMOSLSC_20ac: .ascii "\xa0\0\xb0\0",
_RODATA_G_STCMOSLSC_20b0: .ascii "\xc7\0\xdc\0",
_RODATA_G_STCMOSLSC_20b4: .ascii "\xf9\0\x1e\x01F\x01u\x01\xad\x01\xf3\x01*\x02\x17\x02\xe8\x01\xaa\x01o\x01G\x01\x19\x01\0\x01\xe2\0\xc8\0",
_RODATA_G_STCMOSLSC_20d4: .ascii "\xb8\0\xa0\0",
_RODATA_G_STCMOSLSC_20d8: .ascii "\x92\0\x82\0",
_RODATA_G_STCMOSLSC_20dc: .ascii "x\0l\0",
_RODATA_G_STCMOSLSC_20e0: .ascii "k\0f\0",
_RODATA_G_STCMOSLSC_20e4: .ascii "l\0l\0",
_RODATA_G_STCMOSLSC_20e8: .ascii "{\0\x86\0",
_RODATA_G_STCMOSLSC_20ec: .ascii "\x95\0\xa9\0",
_RODATA_G_STCMOSLSC_20f0: .ascii "\xbf\0\xd0\0",
_RODATA_G_STCMOSLSC_20f4: .ascii "\xed\0\x07\x01$\x01V\x01\x82\x01\xbb\x01\x01\x02;\x02%\x02\xfb\x01\xb9\x01\x7f\x01X\x01(\x01\x06\x01\xf2\0",
_RODATA_G_STCMOSLSC_2114: .ascii "\xd4\0\xc1\0",
_RODATA_G_STCMOSLSC_2118: .ascii "\xb0\0\x9d\0",
_RODATA_G_STCMOSLSC_211c: .ascii "\x8f\0\x86\0",
_RODATA_G_STCMOSLSC_2120: .ascii "w\0y\0",
_RODATA_G_STCMOSLSC_2124: .ascii "s\0y\0",
_RODATA_G_STCMOSLSC_2128: .ascii "~\0\x89\0",
_RODATA_G_STCMOSLSC_212c: .ascii "\x94\0\xa5\0",
_RODATA_G_STCMOSLSC_2130: .ascii "\xb6\0\xcf\0",
_RODATA_G_STCMOSLSC_2134: .ascii "\xdd\0\xf8\0",
_RODATA_G_STCMOSLSC_2138: .ascii "\x18\x013\x01]\x01\x90\x01\xcc\x01\x0c\x02T\x022\x02\n\x02\xc2\x01\x8f\x01e\x013\x01\x17\x01\xff\0\xe4\0",
_RODATA_G_STCMOSLSC_2158: .ascii "\xcd\0\xbf\0",
_RODATA_G_STCMOSLSC_215c: .ascii "\xb4\0\xa2\0",
_RODATA_G_STCMOSLSC_2160: .ascii "\x96\0\x8c\0",
_RODATA_G_STCMOSLSC_2164: .ascii "\x85\0\x85\0",
_RODATA_G_STCMOSLSC_2168: .ascii "\x87\0\x8a\0",
_RODATA_G_STCMOSLSC_216c: .ascii "\x93\0\x9f\0",
_RODATA_G_STCMOSLSC_2170: .ascii "\xb3\0\xc1\0",
_RODATA_G_STCMOSLSC_2174: .ascii "\xd6\0\xeb\0",
_RODATA_G_STCMOSLSC_2178: .ascii "\x06\x01&\x01G\x01k\x01\x9e\x01\xdd\x01(\x02l\x02Y\x02\x1e\x02\xd7\x01\xa3\x01p\x01C\x01(\x01\x0b\x01\xf1\0\xdc\0",
_RODATA_G_STCMOSLSC_219c: .ascii "\xcd\0\xbf\0",
_RODATA_G_STCMOSLSC_21a0: .ascii "\xb0\0\xa4\0",
_RODATA_G_STCMOSLSC_21a4: .ascii "\x9d\0\x97\0",
_RODATA_G_STCMOSLSC_21a8: .ascii "\x96\0\x98\0",
_RODATA_G_STCMOSLSC_21ac: .ascii "\x97\0\xa1\0",
_RODATA_G_STCMOSLSC_21b0: .ascii "\xab\0\xc0\0",
_RODATA_G_STCMOSLSC_21b4: .ascii "\xd2\0\xe4\0",
_RODATA_G_STCMOSLSC_21b8: .ascii "\xf9\0\x16\x01.\x01Z\x01}\x01\xaf\x01\xf5\x01<\x02\x80\x02b\x02*\x02\xea\x01\xb1\x01|\x01M\x012\x01\x1c\x01\xf7\0",
_RODATA_G_STCMOSLSC_21dc: .ascii "\xea\0\xd2\0",
_RODATA_G_STCMOSLSC_21e0: .ascii "\xca\0\xc0\0",
_RODATA_G_STCMOSLSC_21e4: .ascii "\xb4\0\xa9\0",
_RODATA_G_STCMOSLSC_21e8: .ascii "\xa2\0\x9f\0",
_RODATA_G_STCMOSLSC_21ec: .ascii "\xa3\0\xa5\0",
_RODATA_G_STCMOSLSC_21f0: .ascii "\xae\0\xb9\0",
_RODATA_G_STCMOSLSC_21f4: .ascii "\xc7\0\xd3\0",
_RODATA_G_STCMOSLSC_21f8: .ascii "\xed\0\x02\x01\x1f\x017\x01^\x01\x84\x01\xbf\x01\x06\x02W\x02\x89\x02\x99\x03@\x03\xdf\x02\x89\x02>\x02\x07\x02\xcd\x01\x93\x01p\x01_\x015\x01\x13\x01\xfc\0\xea\0",
_RODATA_G_STCMOSLSC_2228: .ascii "\xdb\0\xdd\0",
_RODATA_G_STCMOSLSC_222c: .ascii "\xda\0\xd9\0",
_RODATA_G_STCMOSLSC_2230: .ascii "\xe4\0\xee\0",
_RODATA_G_STCMOSLSC_2234: .ascii "\xfd\0\x14\x01/\x01K\x01n\x01\x9d\x01\xd4\x01\t\x02N\x02\x9b\x02\xee\x02d\x03\xe2\x03\x8f\x03)\x03\xc6\x02v\x02*\x02\xed\x01\xb5\x01\x81\x01_\x01G\x01!\x01\x02\x01\xf0\0",
_RODATA_G_STCMOSLSC_2268: .ascii "\xd9\0\xd0\0",
_RODATA_G_STCMOSLSC_226c: .ascii "\xc6\0\xc7\0",
_RODATA_G_STCMOSLSC_2270: .ascii "\xcb\0\xd2\0",
_RODATA_G_STCMOSLSC_2274: .ascii "\xdc\0\xee\0",
_RODATA_G_STCMOSLSC_2278: .ascii "\0\x01\x1e\x018\x01d\x01\x8c\x01\xbb\x01\xf4\x01C\x02\x86\x02\xd7\x02A\x03\xa7\x03r\x03\t\x03\xb0\x02Y\x02\x0f\x02\xd6\x01\x9a\x01l\x01@\x010\x01\t\x01\xef\0",
_RODATA_G_STCMOSLSC_22a8: .ascii "\xdc\0\xc4\0",
_RODATA_G_STCMOSLSC_22ac: .ascii "\xb7\0\xb1\0",
_RODATA_G_STCMOSLSC_22b0: .ascii "\xaf\0\xb9\0",
_RODATA_G_STCMOSLSC_22b4: .ascii "\xbe\0\xc2\0",
_RODATA_G_STCMOSLSC_22b8: .ascii "\xd5\0\xeb\0",
_RODATA_G_STCMOSLSC_22bc: .ascii "\x05\x01\"\x01L\x01z\x01\xa4\x01\xde\x01%\x02o\x02\xb8\x02\x19\x03\x84\x03U\x03\xed\x02\x90\x02B\x02\xf7\x01\xbb\x01\x8a\x01T\x01.\x01\x0f\x01\xee\0",
_RODATA_G_STCMOSLSC_22e8: .ascii "\xd8\0\xc6\0",
_RODATA_G_STCMOSLSC_22ec: .ascii "\xaf\0\xa7\0",
_RODATA_G_STCMOSLSC_22f0: .ascii "\x9b\0\x9c\0",
_RODATA_G_STCMOSLSC_22f4: .ascii "\xa6\0\xaa\0",
_RODATA_G_STCMOSLSC_22f8: .ascii "\xaf\0\xbe\0",
_RODATA_G_STCMOSLSC_22fc: .ascii "\xd8\0\xf7\0",
_RODATA_G_STCMOSLSC_2300: .ascii "\x10\x01:\x01f\x01\x91\x01\xc8\x01\x16\x02Y\x02\xa3\x02\x02\x03e\x03*\x03\xd8\x02v\x02)\x02\xe3\x01\xa6\x01t\x01@\x01\x1c\x01\xf6\0",
_RODATA_G_STCMOSLSC_2328: .ascii "\xda\0\xc6\0",
_RODATA_G_STCMOSLSC_232c: .ascii "\xaf\0\x9d\0",
_RODATA_G_STCMOSLSC_2330: .ascii "\x92\0\x89\0",
_RODATA_G_STCMOSLSC_2334: .ascii "\x88\0\x91\0",
_RODATA_G_STCMOSLSC_2338: .ascii "\x97\0\x9c\0",
_RODATA_G_STCMOSLSC_233c: .ascii "\xac\0\xcb\0",
_RODATA_G_STCMOSLSC_2340: .ascii "\xec\0\x02\x01%\x01P\x01\x80\x01\xb6\x01\0\x02G\x02\x89\x02\xe8\x02O\x03\x0f\x03\xbb\x02g\x02\x13\x02\xcc\x01\x94\x01`\x010\x01\r\x01\xeb\0\xcc\0",
_RODATA_G_STCMOSLSC_236c: .ascii "\xb1\0\x99\0",
_RODATA_G_STCMOSLSC_2370: .ascii "\x87\0|\0",
_RODATA_G_STCMOSLSC_2374: .ascii "w\0t\0",
_RODATA_G_STCMOSLSC_2378: .ascii "y\0\x82\0",
_RODATA_G_STCMOSLSC_237c: .ascii "\x8c\0\x9a\0",
_RODATA_G_STCMOSLSC_2380: .ascii "\xba\0\xd7\0",
_RODATA_G_STCMOSLSC_2384: .ascii "\xed\0\x15\x01<\x01p\x01\xa4\x01\xe4\x01,\x02w\x02\xd2\x02*\x03\xf6\x02\xa9\x02O\x02\xff\x01\xbd\x01\x85\x01K\x01$\x01\xfd\0\xdb\0",
_RODATA_G_STCMOSLSC_23ac: .ascii "\xba\0\xa0\0",
_RODATA_G_STCMOSLSC_23b0: .ascii "\x89\0v\0",
_RODATA_G_STCMOSLSC_23b4: .ascii "l\0b\0",
_RODATA_G_STCMOSLSC_23b8: .ascii "a\0e\0",
_RODATA_G_STCMOSLSC_23bc: .ascii "q\0{\0",
_RODATA_G_STCMOSLSC_23c0: .ascii "\x89\0\xa4\0",
_RODATA_G_STCMOSLSC_23c4: .ascii "\xc6\0\xde\0",
_RODATA_G_STCMOSLSC_23c8: .ascii "\x07\x01)\x01Z\x01\x91\x01\xc7\x01\x13\x02b\x02\xb4\x02\x19\x03\xde\x02\x8d\x02?\x02\xef\x01\xb2\x01y\x01B\x01\x14\x01\xed\0",
_RODATA_G_STCMOSLSC_23ec: .ascii "\xce\0\xa9\0",
_RODATA_G_STCMOSLSC_23f0: .ascii "\x8e\0y\0",
_RODATA_G_STCMOSLSC_23f4: .ascii "f\0\\0",
_RODATA_G_STCMOSLSC_23f8: .ascii "Q\0O\0",
_RODATA_G_STCMOSLSC_23fc: .ascii "S\0^\0",
_RODATA_G_STCMOSLSC_2400: .ascii "j\0v\0",
_RODATA_G_STCMOSLSC_2404: .ascii "\x94\0\xae\0",
_RODATA_G_STCMOSLSC_2408: .ascii "\xcf\0\xf9\0",
_RODATA_G_STCMOSLSC_240c: .ascii "\x1c\x01K\x01|\x01\xbf\x01\x04\x02H\x02\xa0\x02\xf5\x02\xc9\x02\x82\x02)\x02\xe0\x01\xa5\x01n\x015\x01\t\x01\xdb\0\xb8\0",
_RODATA_G_STCMOSLSC_2430: .ascii "\x9b\0}\0",
_RODATA_G_STCMOSLSC_2434: .ascii "j\0W\0",
_RODATA_G_STCMOSLSC_2438: .ascii "J\0A\0",
_RODATA_G_STCMOSLSC_243c: .ascii "?\0A\0",
_RODATA_G_STCMOSLSC_2440: .ascii "N\0[\0",
_RODATA_G_STCMOSLSC_2444: .ascii "l\0\x88\0",
_RODATA_G_STCMOSLSC_2448: .ascii "\xa5\0\xc9\0",
_RODATA_G_STCMOSLSC_244c: .ascii "\xeb\0\x12\x01>\x01o\x01\xb1\x01\xf5\x01=\x02\x93\x02\xe3\x02\xbf\x02q\x02\x1c\x02\xd1\x01\x99\x01^\x01)\x01\xf9\0\xcf\0",
_RODATA_G_STCMOSLSC_2470: .ascii "\xab\0\x92\0",
_RODATA_G_STCMOSLSC_2474: .ascii "r\0[\0",
_RODATA_G_STCMOSLSC_2478: .ascii "G\0;\0",
_RODATA_G_STCMOSLSC_247c: .ascii "1\0/\0",
_RODATA_G_STCMOSLSC_2480: .ascii "2\0?\0",
_RODATA_G_STCMOSLSC_2484: .ascii "K\0`\0",
_RODATA_G_STCMOSLSC_2488: .ascii "{\0\x9c\0",
_RODATA_G_STCMOSLSC_248c: .ascii "\xc1\0\xe7\0",
_RODATA_G_STCMOSLSC_2490: .ascii "\x05\x016\x01f\x01\xa6\x01\xea\x012\x02\x8d\x02\xd4\x02\xb9\x02b\x02\x11\x02\xc7\x01\x87\x01N\x01\x1b\x01\xef\0",
_RODATA_G_STCMOSLSC_24b0: .ascii "\xc9\0\xa2\0",
_RODATA_G_STCMOSLSC_24b4: .ascii "\x81\0c\0",
_RODATA_G_STCMOSLSC_24b8: .ascii "M\09\0",
_RODATA_G_STCMOSLSC_24bc: .ascii "+\0!\0",
_RODATA_G_STCMOSLSC_24c0: .ascii "!\0&\0",
_RODATA_G_STCMOSLSC_24c4: .ascii "/\0>\0",
_RODATA_G_STCMOSLSC_24c8: .ascii "X\0p\0",
_RODATA_G_STCMOSLSC_24cc: .ascii "\x8f\0\xb5\0",
_RODATA_G_STCMOSLSC_24d0: .ascii "\xda\0\xfb\0",
_RODATA_G_STCMOSLSC_24d4: .ascii "*\x01\\x01\x96\x01\xe1\x01)\x02\x83\x02\xc8\x02\xab\x02Z\x02\x05\x02\xbd\x01x\x01E\x01\x11\x01\xe8\0\xc1\0",
_RODATA_G_STCMOSLSC_24f4: .ascii "\x9a\0v\0",
_RODATA_G_STCMOSLSC_24f8: .ascii "[\0@\0",
_RODATA_G_STCMOSLSC_24fc: .ascii ",\0\x1b\0",
_RODATA_G_STCMOSLSC_2500: .ascii "\x15\0\x12\0",
_RODATA_G_STCMOSLSC_2504: .ascii "\x19\0&\0",
_RODATA_G_STCMOSLSC_2508: .ascii "6\0N\0",
_RODATA_G_STCMOSLSC_250c: .ascii "i\0\x80\0",
_RODATA_G_STCMOSLSC_2510: .ascii "\xaa\0\xce\0",
_RODATA_G_STCMOSLSC_2514: .ascii "\xf3\0\"\x01S\x01\x8a\x01\xd6\x01\x1e\x02w\x02\xc4\x02\x9c\x02N\x02\xfb\x01\xb0\x01n\x01<\x01\r\x01\xe0\0",
_RODATA_G_STCMOSLSC_2534: .ascii "\xb5\0\x92\0",
_RODATA_G_STCMOSLSC_2538: .ascii "n\0S\0",
_RODATA_G_STCMOSLSC_253c: .ascii "7\0\x1f\0",
_RODATA_G_STCMOSLSC_2540: .ascii "\x11\0\x08\0",
_RODATA_G_STCMOSLSC_2544: .ascii "\x07\0\x10\0",
_RODATA_G_STCMOSLSC_2548: .ascii "\x1a\0-\0",
_RODATA_G_STCMOSLSC_254c: .ascii "F\0a\0",
_RODATA_G_STCMOSLSC_2550: .ascii "{\0\x9c\0",
_RODATA_G_STCMOSLSC_2554: .ascii "\xc3\0\xee\0",
_RODATA_G_STCMOSLSC_2558: .ascii "\x18\x01J\x01\x87\x01\xca\x01\x10\x02h\x02\xb7\x02\x91\x02A\x02\xf2\x01\xad\x01p\x01:\x01\n\x01\xdd\0\xb1\0",
_RODATA_G_STCMOSLSC_2578: .ascii "\x8c\0i\0",
_RODATA_G_STCMOSLSC_257c: .ascii "I\00\0",
_RODATA_G_STCMOSLSC_2580: .ascii "\x1c\0\x0c\0",
_RODATA_G_STCMOSLSC_2584: .ascii "\x01\0\0\0",
_RODATA_G_STCMOSLSC_2588: .ascii "\x08\0\x13\0",
_RODATA_G_STCMOSLSC_258c: .ascii "&\0?\0",
_RODATA_G_STCMOSLSC_2590: .ascii "X\0w\0",
_RODATA_G_STCMOSLSC_2594: .ascii "\x97\0\xbc\0",
_RODATA_G_STCMOSLSC_2598: .ascii "\xe9\0\x14\x01F\x01|\x01\xc6\x01\x0c\x02a\x02\xaa\x02\xbc\x02<\x02\xf5\x01\xb0\x01k\x014\x01\x06\x01\xde\0",
_RODATA_G_STCMOSLSC_25b8: .ascii "\xb1\0\x88\0",
_RODATA_G_STCMOSLSC_25bc: .ascii "a\0D\0",
_RODATA_G_STCMOSLSC_25c0: .ascii ".\0\x17\0",
_RODATA_G_STCMOSLSC_25c4: .ascii "\x04\0\0\0",
_RODATA_G_STCMOSLSC_25c8: .ascii "\0\0\x04\0",
_RODATA_G_STCMOSLSC_25cc: .ascii "\x0e\0 \0",
_RODATA_G_STCMOSLSC_25d0: .ascii ":\0T\0",
_RODATA_G_STCMOSLSC_25d4: .ascii "q\0\x95\0",
_RODATA_G_STCMOSLSC_25d8: .ascii "\xba\0\xe5\0",
_RODATA_G_STCMOSLSC_25dc: .ascii "\x13\x01A\x01\x7f\x01\xbd\x01\x06\x02X\x02\xa4\x02\x1e\x03X\x02\xf8\x01\xad\x01h\x01/\x01\x02\x01\xd9\0\xaf\0",
_RODATA_G_STCMOSLSC_25fc: .ascii "\x84\0c\0",
_RODATA_G_STCMOSLSC_2600: .ascii "D\0-\0",
_RODATA_G_STCMOSLSC_2604: .ascii "\x12\0\x03\0",
_RODATA_G_STCMOSLSC_2608: .ascii "\x05\0\x03\0",
_RODATA_G_STCMOSLSC_260c: .ascii "\x01\0\x0b\0",
_RODATA_G_STCMOSLSC_2610: .ascii "\x1d\06\0",
_RODATA_G_STCMOSLSC_2614: .ascii "S\0o\0",
_RODATA_G_STCMOSLSC_2618: .ascii "\x92\0\xb2\0",
_RODATA_G_STCMOSLSC_261c: .ascii "\xe5\0\x10\x01>\x01{\x01\xc1\x01\x04\x02W\x02\xa7\x02:\x03v\x02\xf3\x01\xa9\x01e\x01/\x01\xfd\0\xd3\0",
_RODATA_G_STCMOSLSC_263c: .ascii "\xab\0\x85\0",
_RODATA_G_STCMOSLSC_2640: .ascii "b\0D\0",
_RODATA_G_STCMOSLSC_2644: .ascii "-\0\x13\0",
_RODATA_G_STCMOSLSC_2648: .ascii "\x02\0\x03\0",
_RODATA_G_STCMOSLSC_264c: .ascii "\x03\0\x02\0",
_RODATA_G_STCMOSLSC_2650: .ascii "\x08\0\x1d\0",
_RODATA_G_STCMOSLSC_2654: .ascii "5\0M\0",
_RODATA_G_STCMOSLSC_2658: .ascii "n\0\x93\0",
_RODATA_G_STCMOSLSC_265c: .ascii "\xb6\0\xe5\0",
_RODATA_G_STCMOSLSC_2660: .ascii "\x0f\x01A\x01y\x01\xba\x01\x02\x02Y\x02\x9f\x026\x03`\x02\xef\x01\xa7\x01f\x01+\x01\xfd\0",
_RODATA_G_STCMOSLSC_267c: .ascii "\xd3\0\xa7\0",
_RODATA_G_STCMOSLSC_2680: .ascii "\x85\0`\0",
_RODATA_G_STCMOSLSC_2684: .ascii "B\0.\0",
_RODATA_G_STCMOSLSC_2688: .ascii "\x15\0\x05\0",
_RODATA_G_STCMOSLSC_268c: .ascii "\x02\0\x03\0",
_RODATA_G_STCMOSLSC_2690: .ascii "\x03\0\t\0",
_RODATA_G_STCMOSLSC_2694: .ascii "\x1d\05\0",
_RODATA_G_STCMOSLSC_2698: .ascii "O\0k\0",
_RODATA_G_STCMOSLSC_269c: .ascii "\x91\0\xb7\0",
_RODATA_G_STCMOSLSC_26a0: .ascii "\xe3\0\x11\x01?\x01u\x01\xba\x01\x02\x02U\x02\x9b\x02\xf0\x029\x02\xe6\x01\xa5\x01e\x01.\x01\x03\x01\xd4\0",
_RODATA_G_STCMOSLSC_26c0: .ascii "\xac\0\x87\0",
_RODATA_G_STCMOSLSC_26c4: .ascii "c\0H\0",
_RODATA_G_STCMOSLSC_26c8: .ascii "0\0\x18\0",
_RODATA_G_STCMOSLSC_26cc: .ascii "\x07\0\x02\0",
_RODATA_G_STCMOSLSC_26d0: .ascii "\x01\0\x05\0",
_RODATA_G_STCMOSLSC_26d4: .ascii "\x0b\0 \0",
_RODATA_G_STCMOSLSC_26d8: .ascii "6\0Q\0",
_RODATA_G_STCMOSLSC_26dc: .ascii "p\0\x96\0",
_RODATA_G_STCMOSLSC_26e0: .ascii "\xb8\0\xe4\0",
_RODATA_G_STCMOSLSC_26e4: .ascii "\x12\x01?\x01v\x01\xb8\x01\x08\x02V\x02\xa4\x02\xa9\x02V\x02\xef\x01\xaa\x01m\x014\x01\x04\x01\xda\0\xaf\0",
_RODATA_G_STCMOSLSC_2704: .ascii "\x8b\0g\0",
_RODATA_G_STCMOSLSC_2708: .ascii "L\01\0",
_RODATA_G_STCMOSLSC_270c: .ascii "\x1d\0\x0b\0",
_RODATA_G_STCMOSLSC_2710: .ascii "\x05\0\x04\0",
_RODATA_G_STCMOSLSC_2714: .ascii "\x08\0\x10\0",
_RODATA_G_STCMOSLSC_2718: .ascii "\"\0;\0",
_RODATA_G_STCMOSLSC_271c: .ascii "U\0s\0",
_RODATA_G_STCMOSLSC_2720: .ascii "\x94\0\xbb\0",
_RODATA_G_STCMOSLSC_2724: .ascii "\xe6\0\x16\x01D\x01y\x01\xba\x01\x08\x02]\x02\xad\x02\xd2\x02\x96\x02\xf4\x01\xb0\x01q\x016\x01\x06\x01\xdc\0",
_RODATA_G_STCMOSLSC_2744: .ascii "\xb6\0\x91\0",
_RODATA_G_STCMOSLSC_2748: .ascii "p\0Q\0",
_RODATA_G_STCMOSLSC_274c: .ascii "8\0$\0",
_RODATA_G_STCMOSLSC_2750: .ascii "\x15\0\x0b\0",
_RODATA_G_STCMOSLSC_2754: .ascii "\n\0\r\0",
_RODATA_G_STCMOSLSC_2758: .ascii "\x18\0(\0",
_RODATA_G_STCMOSLSC_275c: .ascii "@\0[\0",
_RODATA_G_STCMOSLSC_2760: .ascii "x\0\x99\0",
_RODATA_G_STCMOSLSC_2764: .ascii "\xbf\0\xed\0",
_RODATA_G_STCMOSLSC_2768: .ascii "\x1c\x01K\x01\x81\x01\xc3\x01\x0c\x02g\x02\xac\x02\xe8\x02\xae\x02\xf8\x01\xb5\x01u\x01>\x01\x0e\x01\xe4\0\xbc\0",
_RODATA_G_STCMOSLSC_2788: .ascii "\x96\0x\0",
_RODATA_G_STCMOSLSC_278c: .ascii "X\0@\0",
_RODATA_G_STCMOSLSC_2790: .ascii "*\0\x1d\0",
_RODATA_G_STCMOSLSC_2794: .ascii "\x17\0\x14\0",
_RODATA_G_STCMOSLSC_2798: .ascii "\x1a\0$\0",
_RODATA_G_STCMOSLSC_279c: .ascii "2\0F\0",
_RODATA_G_STCMOSLSC_27a0: .ascii "c\0~\0",
_RODATA_G_STCMOSLSC_27a4: .ascii "\xa2\0\xc5\0",
_RODATA_G_STCMOSLSC_27a8: .ascii "\xf1\0\x1e\x01Q\x01\x88\x01\xc8\x01\x13\x02n\x02\xb8\x02\xd0\x02\xa0\x02\x02\x02\xbf\x01~\x01D\x01\x18\x01\xec\0",
_RODATA_G_STCMOSLSC_27c8: .ascii "\xc0\0\x9f\0",
_RODATA_G_STCMOSLSC_27cc: .ascii "|\0`\0",
_RODATA_G_STCMOSLSC_27d0: .ascii "J\07\0",
_RODATA_G_STCMOSLSC_27d4: .ascii ".\0,\0",
_RODATA_G_STCMOSLSC_27d8: .ascii "\x1d\0#\0",
_RODATA_G_STCMOSLSC_27dc: .ascii ".\0=\0",
_RODATA_G_STCMOSLSC_27e0: .ascii "O\0k\0",
_RODATA_G_STCMOSLSC_27e4: .ascii "\x87\0\xad\0",
_RODATA_G_STCMOSLSC_27e8: .ascii "\xcd\0\xf7\0",
_RODATA_G_STCMOSLSC_27ec: .ascii "\"\x01W\x01\x90\x01\xce\x01\x1a\x02q\x02\xbb\x02\xb0\x02x\x02\n\x02\xc6\x01\x86\x01O\x01 \x01\xf5\0\xcc\0",
_RODATA_G_STCMOSLSC_280c: .ascii "\xa8\0\x89\0",
_RODATA_G_STCMOSLSC_2810: .ascii "p\0X\0",
_RODATA_G_STCMOSLSC_2814: .ascii "E\0A\0",
_RODATA_G_STCMOSLSC_2818: .ascii "9\0-\0",
_RODATA_G_STCMOSLSC_281c: .ascii "0\0:\0",
_RODATA_G_STCMOSLSC_2820: .ascii "K\0^\0",
_RODATA_G_STCMOSLSC_2824: .ascii "t\0\x91\0",
_RODATA_G_STCMOSLSC_2828: .ascii "\xb8\0\xd4\0",
_RODATA_G_STCMOSLSC_282c: .ascii "\xfe\03\x01g\x01\x9b\x01\xde\x01 \x02w\x02\xc7\x02\xbe\x02o\x02\x16\x02\xdc\x01\x94\x01X\x01-\x01\x01\x01\xd9\0\xb5\0",
_RODATA_G_STCMOSLSC_2850: .ascii "\x99\0x\0",
_RODATA_G_STCMOSLSC_2854: .ascii "d\0S\0",
_RODATA_G_STCMOSLSC_2858: .ascii "I\0G\0",
_RODATA_G_STCMOSLSC_285c: .ascii "=\0=\0",
_RODATA_G_STCMOSLSC_2860: .ascii "H\0W\0",
_RODATA_G_STCMOSLSC_2864: .ascii "h\0\x7f\0",
_RODATA_G_STCMOSLSC_2868: .ascii "\x9d\0\xc0\0",
_RODATA_G_STCMOSLSC_286c: .ascii "\xdd\0\x08\x01?\x01m\x01\xa3\x01\xe7\x01)\x02\x81\x02\xcf\x02\xd2\x02\x84\x02%\x02\xe9\x01\xa8\x01f\x016\x01\x08\x01\xe4\0",
_RODATA_G_STCMOSLSC_2890: .ascii "\xc5\0\xa2\0",
_RODATA_G_STCMOSLSC_2894: .ascii "\x87\0q\0",
_RODATA_G_STCMOSLSC_2898: .ascii "c\0T\0",
_RODATA_G_STCMOSLSC_289c: .ascii "U\0L\0",
_RODATA_G_STCMOSLSC_28a0: .ascii "O\0X\0",
_RODATA_G_STCMOSLSC_28a4: .ascii "c\0y\0",
_RODATA_G_STCMOSLSC_28a8: .ascii "\x8c\0\xab\0",
_RODATA_G_STCMOSLSC_28ac: .ascii "\xca\0\xec\0",
_RODATA_G_STCMOSLSC_28b0: .ascii "\x16\x01F\x01t\x01\xad\x01\xf2\x01;\x02\x91\x02\xe1\x02\xe4\x02\x99\x026\x02\xf8\x01\xbb\x01s\x01D\x01\x19\x01\xf0\0\xcf\0",
_RODATA_G_STCMOSLSC_28d4: .ascii "\xb5\0\x9a\0",
_RODATA_G_STCMOSLSC_28d8: .ascii "\x81\0r\0",
_RODATA_G_STCMOSLSC_28dc: .ascii "c\0]\0",
_RODATA_G_STCMOSLSC_28e0: .ascii "b\0_\0",
_RODATA_G_STCMOSLSC_28e4: .ascii "f\0t\0",
_RODATA_G_STCMOSLSC_28e8: .ascii "\x86\0\x9f\0",
_RODATA_G_STCMOSLSC_28ec: .ascii "\xb7\0\xd6\0",
_RODATA_G_STCMOSLSC_28f0: .ascii "\xfb\0\x1f\x01N\x01\x84\x01\xbe\x01\xff\x01I\x02\xa3\x02\xed\x02\xf4\x02\xa6\x02V\x02\x03\x02\xc0\x01\x84\x01V\x01\"\x01\0\x01\xe1\0\xc0\0",
_RODATA_G_STCMOSLSC_2918: .ascii "\xa8\0\x95\0",
_RODATA_G_STCMOSLSC_291c: .ascii "\x7f\0s\0",
_RODATA_G_STCMOSLSC_2920: .ascii "p\0q\0",
_RODATA_G_STCMOSLSC_2924: .ascii "o\0x\0",
_RODATA_G_STCMOSLSC_2928: .ascii "\x83\0\x93\0",
_RODATA_G_STCMOSLSC_292c: .ascii "\xab\0\xc4\0",
_RODATA_G_STCMOSLSC_2930: .ascii "\xe3\0\x07\x01/\x01a\x01\x96\x01\xce\x01\x13\x02\\x02\xb2\x02\x06\x03\x0c\x03\xb8\x02l\x02\x14\x02\xce\x01\x9b\x01d\x015\x01\x10\x01\xf2\0",
_RODATA_G_STCMOSLSC_2958: .ascii "\xd4\0\xb9\0",
_RODATA_G_STCMOSLSC_295c: .ascii "\xa4\0\x93\0",
_RODATA_G_STCMOSLSC_2960: .ascii "\x86\0\x82\0",
_RODATA_G_STCMOSLSC_2964: .ascii "\x7f\0\x7f\0",
_RODATA_G_STCMOSLSC_2968: .ascii "\x86\0\x96\0",
_RODATA_G_STCMOSLSC_296c: .ascii "\xa6\0\xbd\0",
_RODATA_G_STCMOSLSC_2970: .ascii "\xd7\0\xf2\0",
_RODATA_G_STCMOSLSC_2974: .ascii "\x15\x01@\x01q\x01\xa6\x01\xda\x01$\x02p\x02\xc7\x02\x1c\x03\"\x03\xcd\x02\x7f\x02(\x02\xe2\x01\xac\x01w\x01H\x01#\x01\x01\x01\xe6\0",
_RODATA_G_STCMOSLSC_299c: .ascii "\xcd\0\xb2\0",
_RODATA_G_STCMOSLSC_29a0: .ascii "\xa5\0\x99\0",
_RODATA_G_STCMOSLSC_29a4: .ascii "\x92\0\x8e\0",
_RODATA_G_STCMOSLSC_29a8: .ascii "\x90\0\x9c\0",
_RODATA_G_STCMOSLSC_29ac: .ascii "\xa7\0\xb6\0",
_RODATA_G_STCMOSLSC_29b0: .ascii "\xce\0\xe8\0",
_RODATA_G_STCMOSLSC_29b4: .ascii "\x01\x01'\x01O\x01\x81\x01\xb9\x01\xf7\x01<\x02\x87\x02\xdf\x028\x039\x03\xe8\x02\x8d\x02<\x02\xfb\x01\xc0\x01\x8d\x01Y\x013\x01\x13\x01\xf6\0\xdb\0",
_RODATA_G_STCMOSLSC_29e0: .ascii "\xc5\0\xba\0",
_RODATA_G_STCMOSLSC_29e4: .ascii "\xaa\0\xa3\0",
_RODATA_G_STCMOSLSC_29e8: .ascii "\xa1\0\xa5\0",
_RODATA_G_STCMOSLSC_29ec: .ascii "\xae\0\xbb\0",
_RODATA_G_STCMOSLSC_29f0: .ascii "\xca\0\xde\0",
_RODATA_G_STCMOSLSC_29f4: .ascii "\xf8\0\x15\x018\x01d\x01\x95\x01\xe0\x01\x10\x02P\x02\xa1\x02\xfb\x02U\x03\\x03\t\x03\xa6\x02W\x02\x16\x02\xd3\x01\x9d\x01q\x01D\x01(\x01\x07\x01\xf1\0\xdf\0",
_RODATA_G_STCMOSLSC_2a24: .ascii "\xce\0\xc2\0",
_RODATA_G_STCMOSLSC_2a28: .ascii "\xb8\0\xb4\0",
_RODATA_G_STCMOSLSC_2a2c: .ascii "\xba\0\xc0\0",
_RODATA_G_STCMOSLSC_2a30: .ascii "\xcf\0\xdf\0",
_RODATA_G_STCMOSLSC_2a34: .ascii "\xf2\0\r\x01(\x01R\x01w\x01\xb8\x01\xfe\x01(\x02n\x02\xbd\x02\x16\x03v\x03\x7f\x03\x18\x03\xbe\x02n\x02)\x02\xe5\x01\xb1\x01|\x01Y\x017\x01\x18\x01\0\x01\xeb\0\xdc\0",
_RODATA_G_STCMOSLSC_2a68: .ascii "\xd3\0\xc6\0",
_RODATA_G_STCMOSLSC_2a6c: .ascii "\xc6\0\xcc\0",
_RODATA_G_STCMOSLSC_2a70: .ascii "\xd4\0\xdd\0",
_RODATA_G_STCMOSLSC_2a74: .ascii "\xed\0\x02\x01\x1f\x018\x01]\x01\x8b\x01\xcd\x01\x07\x02=\x02}\x02\xcc\x021\x03\x88\x03$\x03\xcf\x02y\x02.\x02\xf5\x01\xc7\x01\x91\x01h\x01H\x016\x01\x13\x01\xf4\0\xe4\0",
_RODATA_G_STCMOSLSC_2aa8: .ascii "\xd4\0\xcd\0",
_RODATA_G_STCMOSLSC_2aac: .ascii "\xc9\0\xc9\0",
_RODATA_G_STCMOSLSC_2ab0: .ascii "\xcc\0\xd4\0",
_RODATA_G_STCMOSLSC_2ab4: .ascii "\xe2\0\xf2\0",
_RODATA_G_STCMOSLSC_2ab8: .ascii "\x03\x01\x1f\x019\x01d\x01\x86\x01\xb6\x01\xeb\x01,\x02p\x02\xba\x02!\x03\x92\x03\r\x03\xb7\x02g\x02 \x02\xe1\x01\xb4\x01\x83\x01V\x01:\x01&\x01\x05\x01\xe6\0",
_RODATA_G_STCMOSLSC_2ae8: .ascii "\xd7\0\xc3\0",
_RODATA_G_STCMOSLSC_2aec: .ascii "\xbd\0\xb8\0",
_RODATA_G_STCMOSLSC_2af0: .ascii "\xb7\0\xbf\0",
_RODATA_G_STCMOSLSC_2af4: .ascii "\xc5\0\xd0\0",
_RODATA_G_STCMOSLSC_2af8: .ascii "\xe4\0\xf3\0",
_RODATA_G_STCMOSLSC_2afc: .ascii "\x0e\x01+\x01Q\x01z\x01\xa5\x01\xd8\x01\x1f\x02`\x02\xa7\x02\x05\x03g\x03\xf1\x02\x9d\x02P\x02\x07\x02\xcb\x01\x9a\x01l\x01A\x01\x1f\x01\x0f\x01\xe9\0",
_RODATA_G_STCMOSLSC_2b28: .ascii "\xd6\0\xc2\0",
_RODATA_G_STCMOSLSC_2b2c: .ascii "\xae\0\xa8\0",
_RODATA_G_STCMOSLSC_2b30: .ascii "\xa5\0\xa5\0",
_RODATA_G_STCMOSLSC_2b34: .ascii "\xae\0\xb1\0",
_RODATA_G_STCMOSLSC_2b38: .ascii "\xba\0\xcf\0",
_RODATA_G_STCMOSLSC_2b3c: .ascii "\xe0\0\xfa\0",
_RODATA_G_STCMOSLSC_2b40: .ascii "\x15\x01=\x01j\x01\x93\x01\xc0\x01\x08\x02E\x02\x8d\x02\xeb\x02I\x03\xd2\x02\x84\x028\x02\xf8\x01\xb8\x01\x82\x01[\x010\x01\t\x01\xec\0",
_RODATA_G_STCMOSLSC_2b68: .ascii "\xd6\0\xc5\0",
_RODATA_G_STCMOSLSC_2b6c: .ascii "\xb1\0\x9c\0",
_RODATA_G_STCMOSLSC_2b70: .ascii "\x92\0\x8f\0",
_RODATA_G_STCMOSLSC_2b74: .ascii "\x8d\0\x97\0",
_RODATA_G_STCMOSLSC_2b78: .ascii "\xa0\0\xa6\0",
_RODATA_G_STCMOSLSC_2b7c: .ascii "\xb8\0\xd1\0",
_RODATA_G_STCMOSLSC_2b80: .ascii "\xef\0\x06\x01,\x01U\x01|\x01\xae\x01\xf5\x015\x02u\x02\xcd\x02-\x03\xb8\x02m\x02%\x02\xe2\x01\xa9\x01s\x01H\x01\x1f\x01\xfa\0",
_RODATA_G_STCMOSLSC_2ba8: .ascii "\xdf\0\xc5\0",
_RODATA_G_STCMOSLSC_2bac: .ascii "\xaf\0\x9c\0",
_RODATA_G_STCMOSLSC_2bb0: .ascii "\x89\0\x80\0",
_RODATA_G_STCMOSLSC_2bb4: .ascii "|\0~\0",
_RODATA_G_STCMOSLSC_2bb8: .ascii "\x88\0\x8e\0",
_RODATA_G_STCMOSLSC_2bbc: .ascii "\x96\0\xa5\0",
_RODATA_G_STCMOSLSC_2bc0: .ascii "\xc5\0\xe3\0",
_RODATA_G_STCMOSLSC_2bc4: .ascii "\xf7\0\x1b\x01B\x01n\x01\xa1\x01\xe6\x01'\x02f\x02\xb8\x02\x16\x03\x9c\x02Y\x02\x13\x02\xcf\x01\x97\x01e\x018\x01\x11\x01\xf1\0\xd0\0",
_RODATA_G_STCMOSLSC_2bec: .ascii "\xb4\0\x9e\0",
_RODATA_G_STCMOSLSC_2bf0: .ascii "\x8b\0{\0",
_RODATA_G_STCMOSLSC_2bf4: .ascii "q\0j\0",
_RODATA_G_STCMOSLSC_2bf8: .ascii "m\0r\0",
_RODATA_G_STCMOSLSC_2bfc: .ascii "}\0\x85\0",
_RODATA_G_STCMOSLSC_2c00: .ascii "\x94\0\xb3\0",
_RODATA_G_STCMOSLSC_2c04: .ascii "\xd1\0\xe8\0",
_RODATA_G_STCMOSLSC_2c08: .ascii "\x0b\x014\x01^\x01\x92\x01\xd1\x01\x10\x02U\x02\xa7\x02\xfe\x02\x8b\x02G\x02\x01\x02\xbf\x01\x8e\x01[\x01*\x01\x04\x01\xe4\0",
_RODATA_G_STCMOSLSC_2c2c: .ascii "\xc4\0\xa5\0",
_RODATA_G_STCMOSLSC_2c30: .ascii "\x8e\0y\0",
_RODATA_G_STCMOSLSC_2c34: .ascii "k\0`\0",
_RODATA_G_STCMOSLSC_2c38: .ascii "[\0Y\0",
_RODATA_G_STCMOSLSC_2c3c: .ascii "]\0j\0",
_RODATA_G_STCMOSLSC_2c40: .ascii "w\0\x85\0",
_RODATA_G_STCMOSLSC_2c44: .ascii "\xa0\0\xbf\0",
_RODATA_G_STCMOSLSC_2c48: .ascii "\xd8\0\xfe\0",
_RODATA_G_STCMOSLSC_2c4c: .ascii " \x01Q\x01\x83\x01\xb7\x01\xf8\x01C\x02\x8e\x02\xe3\x02z\x029\x02\xef\x01\xb4\x01\x83\x01Q\x01\x1b\x01\xf6\0",
_RODATA_G_STCMOSLSC_2c6c: .ascii "\xd3\0\xb5\0",
_RODATA_G_STCMOSLSC_2c70: .ascii "\x96\0}\0",
_RODATA_G_STCMOSLSC_2c74: .ascii "k\0[\0",
_RODATA_G_STCMOSLSC_2c78: .ascii "M\0I\0",
_RODATA_G_STCMOSLSC_2c7c: .ascii "I\0K\0",
_RODATA_G_STCMOSLSC_2c80: .ascii "X\0f\0",
_RODATA_G_STCMOSLSC_2c84: .ascii "w\0\x94\0",
_RODATA_G_STCMOSLSC_2c88: .ascii "\xaa\0\xc9\0",
_RODATA_G_STCMOSLSC_2c8c: .ascii "\xf3\0\x15\x01C\x01v\x01\xaa\x01\xe7\x011\x02\x81\x02\xcc\x02k\x02*\x02\xe5\x01\xa6\x01t\x01D\x01\x11\x01\xe8\0\xc4\0",
_RODATA_G_STCMOSLSC_2cb0: .ascii "\xa5\0\x88\0",
_RODATA_G_STCMOSLSC_2cb4: .ascii "p\0]\0",
_RODATA_G_STCMOSLSC_2cb8: .ascii "M\0@\0",
_RODATA_G_STCMOSLSC_2cbc: .ascii "9\09\0",
_RODATA_G_STCMOSLSC_2cc0: .ascii ">\0J\0",
_RODATA_G_STCMOSLSC_2cc4: .ascii "Y\0i\0",
_RODATA_G_STCMOSLSC_2cc8: .ascii "\x86\0\xa2\0",
_RODATA_G_STCMOSLSC_2ccc: .ascii "\xca\0\xea\0",
_RODATA_G_STCMOSLSC_2cd0: .ascii "\x0b\x017\x01h\x01\x9e\x01\xdd\x01#\x02r\x02\xb8\x02]\x02\x1c\x02\xd8\x01\x99\x01h\x017\x01\x07\x01\xe0\0",
_RODATA_G_STCMOSLSC_2cf0: .ascii "\xbc\0\x9b\0",
_RODATA_G_STCMOSLSC_2cf4: .ascii "\x81\0f\0",
_RODATA_G_STCMOSLSC_2cf8: .ascii "Q\0@\0",
_RODATA_G_STCMOSLSC_2cfc: .ascii "3\0,\0",
_RODATA_G_STCMOSLSC_2d00: .ascii "-\0-\0",
_RODATA_G_STCMOSLSC_2d04: .ascii "<\0L\0",
_RODATA_G_STCMOSLSC_2d08: .ascii "_\0{\0",
_RODATA_G_STCMOSLSC_2d0c: .ascii "\x9a\0\xc2\0",
_RODATA_G_STCMOSLSC_2d10: .ascii "\xe2\0\0\x011\x01]\x01\x96\x01\xd6\x01\x1d\x02k\x02\xad\x02S\x02\x14\x02\xcd\x01\x8f\x01Y\x01,\x01\xfc\0",
_RODATA_G_STCMOSLSC_2d30: .ascii "\xd7\0\xb2\0",
_RODATA_G_STCMOSLSC_2d34: .ascii "\x93\0s\0",
_RODATA_G_STCMOSLSC_2d38: .ascii "Y\0C\0",
_RODATA_G_STCMOSLSC_2d3c: .ascii "4\0&\0",
_RODATA_G_STCMOSLSC_2d40: .ascii " \0\x1e\0",
_RODATA_G_STCMOSLSC_2d44: .ascii "\"\00\0",
_RODATA_G_STCMOSLSC_2d48: .ascii "?\0W\0",
_RODATA_G_STCMOSLSC_2d4c: .ascii "p\0\x90\0",
_RODATA_G_STCMOSLSC_2d50: .ascii "\xb8\0\xd8\0",
_RODATA_G_STCMOSLSC_2d54: .ascii "\xf7\0$\x01T\x01\x88\x01\xd0\x01\x11\x02b\x02\xa4\x02O\x02\x08\x02\xc2\x01\x89\x01P\x01!\x01\xf4\0\xd0\0",
_RODATA_G_STCMOSLSC_2d74: .ascii "\xac\0\x89\0",
_RODATA_G_STCMOSLSC_2d78: .ascii "k\0N\0",
_RODATA_G_STCMOSLSC_2d7c: .ascii ";\0(\0",
_RODATA_G_STCMOSLSC_2d80: .ascii "\x18\0\x15\0",
_RODATA_G_STCMOSLSC_2d84: .ascii "\x0f\0\x18\0",
_RODATA_G_STCMOSLSC_2d88: .ascii "%\06\0",
_RODATA_G_STCMOSLSC_2d8c: .ascii "P\0i\0",
_RODATA_G_STCMOSLSC_2d90: .ascii "\x83\0\xac\0",
_RODATA_G_STCMOSLSC_2d94: .ascii "\xcb\0\xef\0",
_RODATA_G_STCMOSLSC_2d98: .ascii "\x1e\x01K\x01\x80\x01\xc7\x01\x06\x02Z\x02\xa3\x02G\x02\x01\x02\xbd\x01\x80\x01L\x01\x19\x01\xf2\0",
_RODATA_G_STCMOSLSC_2db4: .ascii "\xcb\0\xa4\0",
_RODATA_G_STCMOSLSC_2db8: .ascii "\x82\0e\0",
_RODATA_G_STCMOSLSC_2dbc: .ascii "G\01\0",
_RODATA_G_STCMOSLSC_2dc0: .ascii "\x1d\0\x10\0",
_RODATA_G_STCMOSLSC_2dc4: .ascii "\x08\0\x06\0",
_RODATA_G_STCMOSLSC_2dc8: .ascii "\x0e\0\x1a\0",
_RODATA_G_STCMOSLSC_2dcc: .ascii "/\0I\0",
_RODATA_G_STCMOSLSC_2dd0: .ascii "a\0|\0",
_RODATA_G_STCMOSLSC_2dd4: .ascii "\x9e\0\xc3\0",
_RODATA_G_STCMOSLSC_2dd8: .ascii "\xec\0\x16\x01C\x01z\x01\xc0\x01\x01\x02L\x02\x92\x02:\x02\xf7\x01\xb8\x01{\x01F\x01\x16\x01\xf0\0\xc8\0",
_RODATA_G_STCMOSLSC_2df8: .ascii "\xa2\0~\0",
_RODATA_G_STCMOSLSC_2dfc: .ascii "^\0C\0",
_RODATA_G_STCMOSLSC_2e00: .ascii ",\0\x17\0",
_RODATA_G_STCMOSLSC_2e04: .ascii "\x07\0\0\0",
_RODATA_G_STCMOSLSC_2e08: .ascii "\0\0\x08\0",
_RODATA_G_STCMOSLSC_2e0c: .ascii "\x16\0(\0",
_RODATA_G_STCMOSLSC_2e10: .ascii "C\0Y\0",
_RODATA_G_STCMOSLSC_2e14: .ascii "w\0\x99\0",
_RODATA_G_STCMOSLSC_2e18: .ascii "\xbc\0\xe4\0",
_RODATA_G_STCMOSLSC_2e1c: .ascii "\x13\x01A\x01x\x01\xb7\x01\xfa\x01F\x02\x8c\x02c\x02\xf3\x01\xb6\x01~\x01A\x01\x14\x01\xec\0",
_RODATA_G_STCMOSLSC_2e38: .ascii "\xc6\0\xa1\0",
_RODATA_G_STCMOSLSC_2e3c: .ascii "z\0Z\0",
_RODATA_G_STCMOSLSC_2e40: .ascii ">\0&\0",
_RODATA_G_STCMOSLSC_2e44: .ascii "\x0f\0\x03\0",
_RODATA_G_STCMOSLSC_2e48: .ascii "\x02\0\x01\0",
_RODATA_G_STCMOSLSC_2e4c: .ascii "\x04\0\x0f\0",
_RODATA_G_STCMOSLSC_2e50: .ascii "\"\0?\0",
_RODATA_G_STCMOSLSC_2e54: .ascii "V\0t\0",
_RODATA_G_STCMOSLSC_2e58: .ascii "\x96\0\xb9\0",
_RODATA_G_STCMOSLSC_2e5c: .ascii "\xe2\0\x11\x01=\x01r\x01\xb3\x01\xf3\x01@\x02\x85\x02\xc8\x02\x07\x02\xb7\x01~\x01?\x01\x12\x01\xe8\0\xc4\0",
_RODATA_G_STCMOSLSC_2e7c: .ascii "\x9e\0w\0",
_RODATA_G_STCMOSLSC_2e80: .ascii "V\0;\0",
_RODATA_G_STCMOSLSC_2e84: .ascii "$\0\x0e\0",
_RODATA_G_STCMOSLSC_2e88: .ascii "\x02\0\x06\0",
_RODATA_G_STCMOSLSC_2e8c: .ascii "\x01\0\x01\0",
_RODATA_G_STCMOSLSC_2e90: .ascii "\x0b\0!\0",
_RODATA_G_STCMOSLSC_2e94: .ascii ";\0R\0",
_RODATA_G_STCMOSLSC_2e98: .ascii "p\0\x97\0",
_RODATA_G_STCMOSLSC_2e9c: .ascii "\xb8\0\xe2\0",
_RODATA_G_STCMOSLSC_2ea0: .ascii "\x11\x01<\x01q\x01\xb3\x01\xed\x01@\x02\x7f\x02\xdc\x02$\x02\xb4\x01{\x01>\x01\r\x01\xe6\0",
_RODATA_G_STCMOSLSC_2ebc: .ascii "\xbf\0\x98\0",
_RODATA_G_STCMOSLSC_2ec0: .ascii "v\0V\0",
_RODATA_G_STCMOSLSC_2ec4: .ascii "<\0%\0",
_RODATA_G_STCMOSLSC_2ec8: .ascii "\x10\0\x01\0",
_RODATA_G_STCMOSLSC_2ecc: .ascii "\x06\0\x05\0",
_RODATA_G_STCMOSLSC_2ed0: .ascii "\x04\0\x0b\0",
_RODATA_G_STCMOSLSC_2ed4: .ascii "\x1f\08\0",
_RODATA_G_STCMOSLSC_2ed8: .ascii "Q\0q\0",
_RODATA_G_STCMOSLSC_2edc: .ascii "\x94\0\xb8\0",
_RODATA_G_STCMOSLSC_2ee0: .ascii "\xe3\0\x0e\x01=\x01o\x01\xb2\x01\xed\x01>\x02\x81\x02\xd0\x02\x0f\x02\xb3\x01{\x01@\x01\x12\x01\xe5\0\xbe\0",
_RODATA_G_STCMOSLSC_2f00: .ascii "\x98\0w\0",
_RODATA_G_STCMOSLSC_2f04: .ascii "W\0<\0",
_RODATA_G_STCMOSLSC_2f08: .ascii "'\0\x14\0",
_RODATA_G_STCMOSLSC_2f0c: .ascii "\x03\0\x03\0",
_RODATA_G_STCMOSLSC_2f10: .ascii "\x02\0\x05\0",
_RODATA_G_STCMOSLSC_2f14: .ascii "\x0b\0 \0",
_RODATA_G_STCMOSLSC_2f18: .ascii "8\0R\0",
_RODATA_G_STCMOSLSC_2f1c: .ascii "s\0\x93\0",
_RODATA_G_STCMOSLSC_2f20: .ascii "\xb7\0\xe3\0",
_RODATA_G_STCMOSLSC_2f24: .ascii "\x0e\x01:\x01n\x01\xb0\x01\xef\x01?\x02~\x02\x91\x02\xf1\x01\xaf\x01w\x01B\x01\x11\x01\xe5\0",
_RODATA_G_STCMOSLSC_2f40: .ascii "\xc3\0\x9d\0",
_RODATA_G_STCMOSLSC_2f44: .ascii "y\0Z\0",
_RODATA_G_STCMOSLSC_2f48: .ascii "=\0+\0",
_RODATA_G_STCMOSLSC_2f4c: .ascii "\x13\0\x07\0",
_RODATA_G_STCMOSLSC_2f50: .ascii "\x02\0\x02\0",
_RODATA_G_STCMOSLSC_2f54: .ascii "\x06\0\x0e\0",
_RODATA_G_STCMOSLSC_2f58: .ascii "\"\0<\0",
_RODATA_G_STCMOSLSC_2f5c: .ascii "T\0t\0",
_RODATA_G_STCMOSLSC_2f60: .ascii "\x96\0\xbb\0",
_RODATA_G_STCMOSLSC_2f64: .ascii "\xe6\0\x0f\x01<\x01p\x01\xb1\x01\xf0\x01?\x02\x7f\x02Q\x02\t\x02\xb6\x01}\x01D\x01\x16\x01\xe9\0\xc5\0",
_RODATA_G_STCMOSLSC_2f84: .ascii "\x9f\0~\0",
_RODATA_G_STCMOSLSC_2f88: .ascii "_\0D\0",
_RODATA_G_STCMOSLSC_2f8c: .ascii "-\0\x1a\0",
_RODATA_G_STCMOSLSC_2f90: .ascii "\n\0\x05\0",
_RODATA_G_STCMOSLSC_2f94: .ascii "\x07\0\t\0",
_RODATA_G_STCMOSLSC_2f98: .ascii "\x13\0(\0",
_RODATA_G_STCMOSLSC_2f9c: .ascii "?\0Z\0",
_RODATA_G_STCMOSLSC_2fa0: .ascii "u\0\x99\0",
_RODATA_G_STCMOSLSC_2fa4: .ascii "\xbb\0\xe8\0",
_RODATA_G_STCMOSLSC_2fa8: .ascii "\x12\x01=\x01v\x01\xb2\x01\xf4\x01E\x02\x87\x02\x8d\x02D\x02\xb7\x01\x7f\x01K\x01\x18\x01\xee\0",
_RODATA_G_STCMOSLSC_2fc4: .ascii "\xc7\0\xa5\0",
_RODATA_G_STCMOSLSC_2fc8: .ascii "\x83\0d\0",
_RODATA_G_STCMOSLSC_2fcc: .ascii "L\05\0",
_RODATA_G_STCMOSLSC_2fd0: .ascii "\"\0\x13\0",
_RODATA_G_STCMOSLSC_2fd4: .ascii "\r\0\n\0",
_RODATA_G_STCMOSLSC_2fd8: .ascii "\x0f\0\x1c\0",
_RODATA_G_STCMOSLSC_2fdc: .ascii "-\0B\0",
_RODATA_G_STCMOSLSC_2fe0: .ascii "^\0}\0",
_RODATA_G_STCMOSLSC_2fe4: .ascii "\x9d\0\xbe\0",
_RODATA_G_STCMOSLSC_2fe8: .ascii "\xed\0\x16\x01E\x01{\x01\xb6\x01\xf8\x01J\x02\x8c\x02\x9f\x02Y\x02\xbb\x01\x84\x01O\x01\x1f\x01\xf4\0\xcd\0",
_RODATA_G_STCMOSLSC_3008: .ascii "\xa7\0\x8c\0",
_RODATA_G_STCMOSLSC_300c: .ascii "n\0R\0",
_RODATA_G_STCMOSLSC_3010: .ascii "=\0)\0",
_RODATA_G_STCMOSLSC_3014: .ascii "\x1c\0\x19\0",
_RODATA_G_STCMOSLSC_3018: .ascii "\x14\0\x1a\0",
_RODATA_G_STCMOSLSC_301c: .ascii "&\06\0",
_RODATA_G_STCMOSLSC_3020: .ascii "J\0e\0",
_RODATA_G_STCMOSLSC_3024: .ascii "\x84\0\xa7\0",
_RODATA_G_STCMOSLSC_3028: .ascii "\xc8\0\xef\0",
_RODATA_G_STCMOSLSC_302c: .ascii "\x1e\x01M\x01\x81\x01\xb9\x01\0\x02R\x02\x96\x02~\x02S\x02\xc3\x01\x8a\x01V\x01&\x01\xfc\0",
_RODATA_G_STCMOSLSC_3048: .ascii "\xd8\0\xb1\0",
_RODATA_G_STCMOSLSC_304c: .ascii "\x96\0v\0",
_RODATA_G_STCMOSLSC_3050: .ascii "[\0F\0",
_RODATA_G_STCMOSLSC_3054: .ascii "4\0.\0",
_RODATA_G_STCMOSLSC_3058: .ascii "-\0\x1f\0",
_RODATA_G_STCMOSLSC_305c: .ascii "&\0/\0",
_RODATA_G_STCMOSLSC_3060: .ascii "?\0U\0",
_RODATA_G_STCMOSLSC_3064: .ascii "l\0\x8c\0",
_RODATA_G_STCMOSLSC_3068: .ascii "\xb3\0\xd1\0",
_RODATA_G_STCMOSLSC_306c: .ascii "\xf2\0'\x01T\x01\x85\x01\xc3\x01\x07\x02T\x02\xa5\x02T\x02 \x02\xd0\x01\x95\x01[\x01-\x01\x05\x01\xe0\0",
_RODATA_G_STCMOSLSC_308c: .ascii "\xba\0\x9d\0",
_RODATA_G_STCMOSLSC_3090: .ascii "\x7f\0f\0",
_RODATA_G_STCMOSLSC_3094: .ascii "R\0?\0",
_RODATA_G_STCMOSLSC_3098: .ascii "@\09\0",
_RODATA_G_STCMOSLSC_309c: .ascii "0\01\0",
_RODATA_G_STCMOSLSC_30a0: .ascii ">\0O\0",
_RODATA_G_STCMOSLSC_30a4: .ascii "c\0x\0",
_RODATA_G_STCMOSLSC_30a8: .ascii "\x98\0\xb9\0",
_RODATA_G_STCMOSLSC_30ac: .ascii "\xd8\0\xfe\0",
_RODATA_G_STCMOSLSC_30b0: .ascii "5\x01b\x01\x8e\x01\xcc\x01\x0c\x02^\x02\xa0\x02e\x02\"\x02\xd8\x01\xa8\x01i\x014\x01\x11\x01\xe7\0\xc5\0",
_RODATA_G_STCMOSLSC_30d0: .ascii "\xa7\0\x8a\0",
_RODATA_G_STCMOSLSC_30d4: .ascii "q\0\\0",
_RODATA_G_STCMOSLSC_30d8: .ascii "N\0I\0",
_RODATA_G_STCMOSLSC_30dc: .ascii "H\0?\0",
_RODATA_G_STCMOSLSC_30e0: .ascii "@\0L\0",
_RODATA_G_STCMOSLSC_30e4: .ascii "\\0n\0",
_RODATA_G_STCMOSLSC_30e8: .ascii "\x81\0\xa0\0",
_RODATA_G_STCMOSLSC_30ec: .ascii "\xc2\0\xe2\0",
_RODATA_G_STCMOSLSC_30f0: .ascii "\x07\x01;\x01i\x01\x99\x01\xd5\x01\x15\x02h\x02\xac\x02w\x025\x02\xe4\x01\xb6\x01y\x01A\x01\x1a\x01\xf4\0",
_RODATA_G_STCMOSLSC_3110: .ascii "\xd1\0\xb3\0",
_RODATA_G_STCMOSLSC_3114: .ascii "\x99\0}\0",
_RODATA_G_STCMOSLSC_3118: .ascii "l\0\\0",
_RODATA_G_STCMOSLSC_311c: .ascii "Q\0S\0",
_RODATA_G_STCMOSLSC_3120: .ascii "O\0O\0",
_RODATA_G_STCMOSLSC_3124: .ascii "Z\0i\0",
_RODATA_G_STCMOSLSC_3128: .ascii "z\0\x8f\0",
_RODATA_G_STCMOSLSC_312c: .ascii "\xab\0\xcb\0",
_RODATA_G_STCMOSLSC_3130: .ascii "\xec\0\x12\x01E\x01o\x01\xa6\x01\xe1\x01!\x02p\x02\xbd\x02\x82\x02D\x02\xf2\x01\xc0\x01\x88\x01O\x01%\x01\xff\0\xde\0",
_RODATA_G_STCMOSLSC_3154: .ascii "\xbe\0\xa5\0",
_RODATA_G_STCMOSLSC_3158: .ascii "\x8f\0y\0",
_RODATA_G_STCMOSLSC_315c: .ascii "l\0^\0",
_RODATA_G_STCMOSLSC_3160: .ascii "^\0c\0",
_RODATA_G_STCMOSLSC_3164: .ascii "`\0g\0",
_RODATA_G_STCMOSLSC_3168: .ascii "u\0\x89\0",
_RODATA_G_STCMOSLSC_316c: .ascii "\xa0\0\xb8\0",
_RODATA_G_STCMOSLSC_3170: .ascii "\xd8\0\xf8\0",
_RODATA_G_STCMOSLSC_3174: .ascii "\x1e\x01L\x01x\x01\xae\x01\xec\x011\x02\x82\x02\xcb\x02\x93\x02L\x02\r\x02\xcb\x01\x8f\x01^\x010\x01\x0b\x01\xea\0\xce\0",
_RODATA_G_STCMOSLSC_3198: .ascii "\xb3\0\x9d\0",
_RODATA_G_STCMOSLSC_319c: .ascii "\x8c\0z\0",
_RODATA_G_STCMOSLSC_31a0: .ascii "p\0p\0",
_RODATA_G_STCMOSLSC_31a4: .ascii "p\0p\0",
_RODATA_G_STCMOSLSC_31a8: .ascii "v\0\x87\0",
_RODATA_G_STCMOSLSC_31ac: .ascii "\x96\0\xae\0",
_RODATA_G_STCMOSLSC_31b0: .ascii "\xc7\0\xe3\0",
_RODATA_G_STCMOSLSC_31b4: .ascii "\x06\x01-\x01[\x01\x8a\x01\xbb\x01\xf9\x01B\x02\x8f\x02\xdc\x02\xa6\x02^\x02!\x02\xd6\x01\x9c\x01n\x01A\x01\x1a\x01\xfb\0",
_RODATA_G_STCMOSLSC_31d8: .ascii "\xdd\0\xc2\0",
_RODATA_G_STCMOSLSC_31dc: .ascii "\xad\0\x9a\0",
_RODATA_G_STCMOSLSC_31e0: .ascii "\x8b\0\x82\0",
_RODATA_G_STCMOSLSC_31e4: .ascii "~\0}\0",
_RODATA_G_STCMOSLSC_31e8: .ascii "|\0\x88\0",
_RODATA_G_STCMOSLSC_31ec: .ascii "\x97\0\xa6\0",
_RODATA_G_STCMOSLSC_31f0: .ascii "\xbd\0\xd6\0",
_RODATA_G_STCMOSLSC_31f4: .ascii "\xf3\0\x14\x01;\x01h\x01\x97\x01\xcd\x01\t\x02W\x02\xa7\x02\xf4\x02\xb9\x02q\x020\x02\xe8\x01\xb0\x01}\x01R\x01*\x01\r\x01\xee\0",
_RODATA_G_STCMOSLSC_321c: .ascii "\xd4\0\xbf\0",
_RODATA_G_STCMOSLSC_3220: .ascii "\xaa\0\x9c\0",
_RODATA_G_STCMOSLSC_3224: .ascii "\x93\0\x90\0",
_RODATA_G_STCMOSLSC_3228: .ascii "\x8e\0\x90\0",
_RODATA_G_STCMOSLSC_322c: .ascii "\x99\0\xa8\0",
_RODATA_G_STCMOSLSC_3230: .ascii "\xb8\0\xce\0",
_RODATA_G_STCMOSLSC_3234: .ascii "\xe6\0\x03\x01$\x01M\x01y\x01\xad\x01\xe0\x01\x1e\x02h\x02\xb7\x02\x06\x03\xcd\x02\x91\x02@\x02\xfc\x01\xc2\x01\x90\x01f\x01<\x01\x1e\x01\xfe\0\xe7\0",
_RODATA_G_STCMOSLSC_3260: .ascii "\xcf\0\xbc\0",
_RODATA_G_STCMOSLSC_3264: .ascii "\xb1\0\xa8\0",
_RODATA_G_STCMOSLSC_3268: .ascii "\xa3\0\xa0\0",
_RODATA_G_STCMOSLSC_326c: .ascii "\xa3\0\xac\0",
_RODATA_G_STCMOSLSC_3270: .ascii "\xba\0\xca\0",
_RODATA_G_STCMOSLSC_3274: .ascii "\xe0\0\xf7\0",
_RODATA_G_STCMOSLSC_3278: .ascii "\x14\x018\x01^\x01\x8a\x01\xd0\x01\xf8\x013\x02}\x02\xcf\x02\x1d\x03\xef\x02\xa9\x02W\x02\x13\x02\xdb\x01\xa8\x01x\x01Q\x01-\x01\x11\x01\xf8\0\xe4\0",
_RODATA_G_STCMOSLSC_32a4: .ascii "\xd4\0\xc7\0",
_RODATA_G_STCMOSLSC_32a8: .ascii "\xbb\0\xb2\0",
_RODATA_G_STCMOSLSC_32ac: .ascii "\xb6\0\xb5\0",
_RODATA_G_STCMOSLSC_32b0: .ascii "\xc1\0\xcd\0",
_RODATA_G_STCMOSLSC_32b4: .ascii "\xdf\0\xf2\0",
_RODATA_G_STCMOSLSC_32b8: .ascii "\x0b\x01&\x01L\x01s\x01\xac\x01\xee\x01\x0f\x02P\x02\x95\x02\xea\x02;\x03\x17\x03\xb7\x02h\x02(\x02\xef\x01\xb5\x01\x89\x01a\x01;\x01!\x01\x06\x01\xf0\0\xde\0",
_RODATA_G_STCMOSLSC_32e8: .ascii "\xd7\0\xc9\0",
_RODATA_G_STCMOSLSC_32ec: .ascii "\xc2\0\xc3\0",
_RODATA_G_STCMOSLSC_32f0: .ascii "\xc8\0\xd3\0",
_RODATA_G_STCMOSLSC_32f4: .ascii "\xd9\0\xed\0",
_RODATA_G_STCMOSLSC_32f8: .ascii "\0\x01\x19\x013\x01Z\x01\x89\x01\xc1\x01\xfd\x01!\x02`\x02\xa9\x02\x02\x03W\x03+\x03\xcd\x02w\x022\x02\xf2\x01\xc6\x01\x8f\x01f\x01H\x016\x01\x16\x01\xf5\0",
_RODATA_G_STCMOSLSC_3328: .ascii "\xe3\0\xd5\0",
_RODATA_G_STCMOSLSC_332c: .ascii "\xc9\0\xc8\0",
_RODATA_G_STCMOSLSC_3330: .ascii "\xc9\0\xc9\0",
_RODATA_G_STCMOSLSC_3334: .ascii "\xd5\0\xdf\0",
_RODATA_G_STCMOSLSC_3338: .ascii "\xf1\0\x02\x01\x1c\x018\x01Z\x01\x87\x01\xb4\x01\xe7\x01-\x02p\x02\xbd\x02#\x03\x8d\x03\x13\x03\xb9\x02g\x02\x1e\x02\xe1\x01\xb1\x01\x82\x01T\x01;\x01$\x01\x04\x01\xe7\0\xd9\0",
_RODATA_G_STCMOSLSC_336c: .ascii "\xc5\0\xbb\0",
_RODATA_G_STCMOSLSC_3370: .ascii "\xb4\0\xb6\0",
_RODATA_G_STCMOSLSC_3374: .ascii "\xbf\0\xc4\0",
_RODATA_G_STCMOSLSC_3378: .ascii "\xd1\0\xe3\0",
_RODATA_G_STCMOSLSC_337c: .ascii "\xf4\0\r\x01+\x01Q\x01x\x01\xa4\x01\xd7\x01\x1b\x02]\x02\xa6\x02\x06\x03g\x03\xf4\x02\x9e\x02M\x02\x0b\x02\xca\x01\x98\x01m\x01A\x01\x1b\x01\r\x01\xea\0\xd1\0",
_RODATA_G_STCMOSLSC_33ac: .ascii "\xc4\0\xb1\0",
_RODATA_G_STCMOSLSC_33b0: .ascii "\xa8\0\xa3\0",
_RODATA_G_STCMOSLSC_33b4: .ascii "\xa3\0\xb0\0",
_RODATA_G_STCMOSLSC_33b8: .ascii "\xb3\0\xb9\0",
_RODATA_G_STCMOSLSC_33bc: .ascii "\xc9\0\xdf\0",
_RODATA_G_STCMOSLSC_33c0: .ascii "\xfb\0\x16\x01=\x01h\x01\x90\x01\xc2\x01\x04\x02C\x02\x8e\x02\xec\x02H\x03\xda\x02\x85\x029\x02\xf6\x01\xb9\x01\x87\x01Z\x01-\x01\n\x01\xef\0\xd3\0",
_RODATA_G_STCMOSLSC_33ec: .ascii "\xbf\0\xb0\0",
_RODATA_G_STCMOSLSC_33f0: .ascii "\x9e\0\x92\0",
_RODATA_G_STCMOSLSC_33f4: .ascii "\x8e\0\x91\0",
_RODATA_G_STCMOSLSC_33f8: .ascii "\x9a\0\x9f\0",
_RODATA_G_STCMOSLSC_33fc: .ascii "\xa6\0\xb6\0",
_RODATA_G_STCMOSLSC_3400: .ascii "\xce\0\xeb\0",
_RODATA_G_STCMOSLSC_3404: .ascii "\x02\x01-\x01Y\x01{\x01\xb3\x01\xf7\x015\x02{\x02\xd0\x02*\x03\xb8\x02n\x02!\x02\xe4\x01\xa8\x01v\x01I\x01\x1e\x01\xfe\0\xdf\0",
_RODATA_G_STCMOSLSC_342c: .ascii "\xc5\0\xaf\0",
_RODATA_G_STCMOSLSC_3430: .ascii "\x9b\0\x8c\0",
_RODATA_G_STCMOSLSC_3434: .ascii "\x80\0~\0",
_RODATA_G_STCMOSLSC_3438: .ascii "z\0\x89\0",
_RODATA_G_STCMOSLSC_343c: .ascii "\x8e\0\x98\0",
_RODATA_G_STCMOSLSC_3440: .ascii "\xa6\0\xc4\0",
_RODATA_G_STCMOSLSC_3444: .ascii "\xe2\0\xf5\0",
_RODATA_G_STCMOSLSC_3448: .ascii "\x19\x01B\x01o\x01\x9f\x01\xe4\x01'\x02i\x02\xba\x02\x15\x03\xa3\x02[\x02\x13\x02\xd0\x01\x93\x01c\x017\x01\x0e\x01\xee\0",
_RODATA_G_STCMOSLSC_346c: .ascii "\xd2\0\xb5\0",
_RODATA_G_STCMOSLSC_3470: .ascii "\xa0\0\x8a\0",
_RODATA_G_STCMOSLSC_3474: .ascii "z\0q\0",
_RODATA_G_STCMOSLSC_3478: .ascii "k\0j\0",
_RODATA_G_STCMOSLSC_347c: .ascii "q\0|\0",
_RODATA_G_STCMOSLSC_3480: .ascii "\x84\0\x95\0",
_RODATA_G_STCMOSLSC_3484: .ascii "\xb2\0\xcf\0",
_RODATA_G_STCMOSLSC_3488: .ascii "\xe9\0\t\x013\x01`\x01\x8e\x01\xcd\x01\x10\x02S\x02\xa1\x02\xfb\x02\x8e\x02I\x02\x01\x02\xbf\x01\x8a\x01W\x01-\x01\x03\x01\xe3\0\xc4\0",
_RODATA_G_STCMOSLSC_34b0: .ascii "\xa2\0\x8e\0",
_RODATA_G_STCMOSLSC_34b4: .ascii "x\0i\0",
_RODATA_G_STCMOSLSC_34b8: .ascii "`\0Z\0",
_RODATA_G_STCMOSLSC_34bc: .ascii "X\0]\0",
_RODATA_G_STCMOSLSC_34c0: .ascii "l\0v\0",
_RODATA_G_STCMOSLSC_34c4: .ascii "\x86\0\xa1\0",
_RODATA_G_STCMOSLSC_34c8: .ascii "\xc0\0\xdb\0",
_RODATA_G_STCMOSLSC_34cc: .ascii "\xfc\0\x1f\x01N\x01\x82\x01\xb9\x01\xf8\x01B\x02\x90\x02\xe7\x02y\x028\x02\xef\x01\xb4\x01\x80\x01R\x01#\x01\xf5\0\xd4\0",
_RODATA_G_STCMOSLSC_34f0: .ascii "\xb5\0\x95\0",
_RODATA_G_STCMOSLSC_34f4: .ascii "~\0k\0",
_RODATA_G_STCMOSLSC_34f8: .ascii "Y\0P\0",
_RODATA_G_STCMOSLSC_34fc: .ascii "H\0G\0",
_RODATA_G_STCMOSLSC_3500: .ascii "N\0X\0",
_RODATA_G_STCMOSLSC_3504: .ascii "f\0u\0",
_RODATA_G_STCMOSLSC_3508: .ascii "\x92\0\xaa\0",
_RODATA_G_STCMOSLSC_350c: .ascii "\xca\0\xf1\0",
_RODATA_G_STCMOSLSC_3510: .ascii "\x14\x01?\x01q\x01\xab\x01\xe9\x013\x02\x7f\x02\xc9\x02p\x02+\x02\xe1\x01\xa5\x01x\x01G\x01\x15\x01\xea\0",
_RODATA_G_STCMOSLSC_3530: .ascii "\xc5\0\xa5\0",
_RODATA_G_STCMOSLSC_3534: .ascii "\x8a\0s\0",
_RODATA_G_STCMOSLSC_3538: .ascii "^\0M\0",
_RODATA_G_STCMOSLSC_353c: .ascii "A\0;\0",
_RODATA_G_STCMOSLSC_3540: .ascii ":\0=\0",
_RODATA_G_STCMOSLSC_3544: .ascii "K\0Y\0",
_RODATA_G_STCMOSLSC_3548: .ascii "j\0\x84\0",
_RODATA_G_STCMOSLSC_354c: .ascii "\xa2\0\xc5\0",
_RODATA_G_STCMOSLSC_3550: .ascii "\xe7\0\x0c\x016\x01e\x01\x9e\x01\xde\x01%\x02q\x02\xbd\x02h\x02\"\x02\xda\x01\x9f\x01h\x017\x01\x08\x01\xdf\0\xbb\0",
_RODATA_G_STCMOSLSC_3574: .ascii "\x9a\0\x7f\0",
_RODATA_G_STCMOSLSC_3578: .ascii "d\0R\0",
_RODATA_G_STCMOSLSC_357c: .ascii "@\05\0",
_RODATA_G_STCMOSLSC_3580: .ascii ",\0,\0",
_RODATA_G_STCMOSLSC_3584: .ascii "-\0=\0",
_RODATA_G_STCMOSLSC_3588: .ascii "K\0_\0",
_RODATA_G_STCMOSLSC_358c: .ascii "z\0\x9b\0",
_RODATA_G_STCMOSLSC_3590: .ascii "\xc2\0\xe1\0",
_RODATA_G_STCMOSLSC_3594: .ascii "\x01\x01.\x01\\x01\x98\x01\xd9\x01\x1e\x02j\x02\xae\x02X\x02\x17\x02\xce\x01\x93\x01\\x01*\x01\xff\0\xd5\0",
_RODATA_G_STCMOSLSC_35b4: .ascii "\xb1\0\x94\0",
_RODATA_G_STCMOSLSC_35b8: .ascii "u\0Y\0",
_RODATA_G_STCMOSLSC_35bc: .ascii "D\02\0",
_RODATA_G_STCMOSLSC_35c0: .ascii "&\0\x1e\0",
_RODATA_G_STCMOSLSC_35c4: .ascii "\x1e\0 \0",
_RODATA_G_STCMOSLSC_35c8: .ascii "0\0A\0",
_RODATA_G_STCMOSLSC_35cc: .ascii "W\0p\0",
_RODATA_G_STCMOSLSC_35d0: .ascii "\x8d\0\xb6\0",
_RODATA_G_STCMOSLSC_35d4: .ascii "\xd7\0\xf7\0",
_RODATA_G_STCMOSLSC_35d8: .ascii "%\x01U\x01\x8c\x01\xd2\x01\x12\x02a\x02\xa7\x02O\x02\x0b\x02\xc5\x01\x88\x01T\x01\"\x01\xf8\0",
_RODATA_G_STCMOSLSC_35f4: .ascii "\xce\0\xac\0",
_RODATA_G_STCMOSLSC_35f8: .ascii "\x8e\0l\0",
_RODATA_G_STCMOSLSC_35fc: .ascii "R\0:\0",
_RODATA_G_STCMOSLSC_3600: .ascii "'\0\x19\0",
_RODATA_G_STCMOSLSC_3604: .ascii "\x13\0\x10\0",
_RODATA_G_STCMOSLSC_3608: .ascii "\x18\0$\0",
_RODATA_G_STCMOSLSC_360c: .ascii "6\0P\0",
_RODATA_G_STCMOSLSC_3610: .ascii "h\0\x82\0",
_RODATA_G_STCMOSLSC_3614: .ascii "\xaa\0\xca\0",
_RODATA_G_STCMOSLSC_3618: .ascii "\xee\0\x1e\x01M\x01\x82\x01\xc2\x01\x05\x02Y\x02\x9d\x02A\x02\x01\x02\xbd\x01\x84\x01O\x01\x1c\x01\xf2\0\xcc\0",
_RODATA_G_STCMOSLSC_3638: .ascii "\xa5\0\x85\0",
_RODATA_G_STCMOSLSC_363c: .ascii "e\0J\0",
_RODATA_G_STCMOSLSC_3640: .ascii "0\0\x1e\0",
_RODATA_G_STCMOSLSC_3644: .ascii "\x11\0\x07\0",
_RODATA_G_STCMOSLSC_3648: .ascii "\x07\0\r\0",
_RODATA_G_STCMOSLSC_364c: .ascii "\x1b\0/\0",
_RODATA_G_STCMOSLSC_3650: .ascii "H\0_\0",
_RODATA_G_STCMOSLSC_3654: .ascii "}\0\x9e\0",
_RODATA_G_STCMOSLSC_3658: .ascii "\xc3\0\xeb\0",
_RODATA_G_STCMOSLSC_365c: .ascii "\x18\x01H\x01{\x01\xc0\x01\xff\x01M\x02\x97\x02;\x02\xfa\x01\xb8\x01}\x01I\x01\x16\x01\xee\0",
_RODATA_G_STCMOSLSC_3678: .ascii "\xcc\0\xa3\0",
_RODATA_G_STCMOSLSC_367c: .ascii "\x80\0`\0",
_RODATA_G_STCMOSLSC_3680: .ascii "D\0*\0",
_RODATA_G_STCMOSLSC_3684: .ascii "\x18\0\x07\0",
_RODATA_G_STCMOSLSC_3688: .ascii "\0\0\0\0",
_RODATA_G_STCMOSLSC_368c: .ascii "\t\0\x14\0",
_RODATA_G_STCMOSLSC_3690: .ascii "(\0C\0",
_RODATA_G_STCMOSLSC_3694: .ascii "X\0x\0",
_RODATA_G_STCMOSLSC_3698: .ascii "\x98\0\xbd\0",
_RODATA_G_STCMOSLSC_369c: .ascii "\xe7\0\x13\x01B\x01r\x01\xb4\x01\xfa\x01E\x02\x8a\x02k\x02\xfb\x01\xba\x01\x80\x01G\x01\x17\x01\xef\0\xca\0",
_RODATA_G_STCMOSLSC_36bc: .ascii "\xa1\0|\0",
_RODATA_G_STCMOSLSC_36c0: .ascii "[\0@\0",
_RODATA_G_STCMOSLSC_36c4: .ascii "(\0\x11\0",
_RODATA_G_STCMOSLSC_36c8: .ascii "\x04\0\x03\0",
_RODATA_G_STCMOSLSC_36cc: .ascii "\x01\0\x06\0",
_RODATA_G_STCMOSLSC_36d0: .ascii "\x10\0#\0",
_RODATA_G_STCMOSLSC_36d4: .ascii ">\0U\0",
_RODATA_G_STCMOSLSC_36d8: .ascii "u\0\x98\0",
_RODATA_G_STCMOSLSC_36dc: .ascii "\xba\0\xe5\0",
_RODATA_G_STCMOSLSC_36e0: .ascii "\x12\x01>\x01q\x01\xb0\x01\xf4\x01>\x02\x8b\x02\xca\x02\x0e\x02\xbd\x01\x7f\x01B\x01\x14\x01\xe8\0",
_RODATA_G_STCMOSLSC_36fc: .ascii "\xc5\0\x9e\0",
_RODATA_G_STCMOSLSC_3700: .ascii "y\0Z\0",
_RODATA_G_STCMOSLSC_3704: .ascii ";\0'\0",
_RODATA_G_STCMOSLSC_3708: .ascii "\x10\0\x02\0",
_RODATA_G_STCMOSLSC_370c: .ascii "\x04\0\x02\0",
_RODATA_G_STCMOSLSC_3710: .ascii "\0\0\x0b\0",
_RODATA_G_STCMOSLSC_3714: .ascii "!\07\0",
_RODATA_G_STCMOSLSC_3718: .ascii "T\0s\0",
_RODATA_G_STCMOSLSC_371c: .ascii "\x95\0\xb9\0",
_RODATA_G_STCMOSLSC_3720: .ascii "\xe3\0\x11\x01<\x01p\x01\xb0\x01\xf2\x01>\x02\x86\x02\xde\x02*\x02\xb6\x01~\x01@\x01\x13\x01\xe5\0\xbe\0",
_RODATA_G_STCMOSLSC_3740: .ascii "\x98\0y\0",
_RODATA_G_STCMOSLSC_3744: .ascii "Z\0:\0",
_RODATA_G_STCMOSLSC_3748: .ascii "'\0\x11\0",
_RODATA_G_STCMOSLSC_374c: .ascii "\x01\0\x03\0",
_RODATA_G_STCMOSLSC_3750: .ascii "\x05\0\x03\0",
_RODATA_G_STCMOSLSC_3754: .ascii "\x0b\0!\0",
_RODATA_G_STCMOSLSC_3758: .ascii "7\0R\0",
_RODATA_G_STCMOSLSC_375c: .ascii "q\0\x92\0",
_RODATA_G_STCMOSLSC_3760: .ascii "\xb8\0\xe3\0",
_RODATA_G_STCMOSLSC_3764: .ascii "\x0f\x01<\x01q\x01\xb3\x01\xf2\x019\x02\x7f\x02\xd6\x02\x15\x02\xb0\x01y\x01=\x01\x13\x01\xe7\0",
_RODATA_G_STCMOSLSC_3780: .ascii "\xbe\0\x99\0",
_RODATA_G_STCMOSLSC_3784: .ascii "y\0X\0",
_RODATA_G_STCMOSLSC_3788: .ascii ">\0)\0",
_RODATA_G_STCMOSLSC_378c: .ascii "\x15\0\x04\0",
_RODATA_G_STCMOSLSC_3790: .ascii "\x01\0\x04\0",
_RODATA_G_STCMOSLSC_3794: .ascii "\x05\0\x0b\0",
_RODATA_G_STCMOSLSC_3798: .ascii "\x1f\09\0",
_RODATA_G_STCMOSLSC_379c: .ascii "S\0s\0",
_RODATA_G_STCMOSLSC_37a0: .ascii "\x94\0\xb8\0",
_RODATA_G_STCMOSLSC_37a4: .ascii "\xe1\0\r\x018\x01m\x01\xb2\x01\xf2\x01=\x02\x81\x02\x93\x02\xf4\x01\xb0\x01y\x01D\x01\x14\x01\xe6\0\xc0\0",
_RODATA_G_STCMOSLSC_37c4: .ascii "\x9c\0|\0",
_RODATA_G_STCMOSLSC_37c8: .ascii "Z\0A\0",
_RODATA_G_STCMOSLSC_37cc: .ascii ",\0\x16\0",
_RODATA_G_STCMOSLSC_37d0: .ascii "\x07\0\x02\0",
_RODATA_G_STCMOSLSC_37d4: .ascii "\x04\0\x07\0",
_RODATA_G_STCMOSLSC_37d8: .ascii "\x0e\0!\0",
_RODATA_G_STCMOSLSC_37dc: .ascii "9\0V\0",
_RODATA_G_STCMOSLSC_37e0: .ascii "t\0\x96\0",
_RODATA_G_STCMOSLSC_37e4: .ascii "\xbc\0\xe4\0",
_RODATA_G_STCMOSLSC_37e8: .ascii "\x0f\x019\x01n\x01\xae\x01\xf4\x01B\x02\x82\x02S\x02\x10\x02\xb5\x01}\x01F\x01\x15\x01\xec\0",
_RODATA_G_STCMOSLSC_3804: .ascii "\xc4\0\x9e\0",
_RODATA_G_STCMOSLSC_3808: .ascii "\x82\0^\0",
_RODATA_G_STCMOSLSC_380c: .ascii "F\0.\0",
_RODATA_G_STCMOSLSC_3810: .ascii "\x1a\0\x0b\0",
_RODATA_G_STCMOSLSC_3814: .ascii "\x06\0\x07\0",
_RODATA_G_STCMOSLSC_3818: .ascii "\t\0\x14\0",
_RODATA_G_STCMOSLSC_381c: .ascii "%\0?\0",
_RODATA_G_STCMOSLSC_3820: .ascii "Y\0x\0",
_RODATA_G_STCMOSLSC_3824: .ascii "\x99\0\xbb\0",
_RODATA_G_STCMOSLSC_3828: .ascii "\xe6\0\x14\x01=\x01u\x01\xb1\x01\xf5\x01G\x02\x87\x02\x85\x02J\x02\xba\x01\x82\x01K\x01\x1b\x01\xef\0\xca\0",
_RODATA_G_STCMOSLSC_3848: .ascii "\xa5\0\x84\0",
_RODATA_G_STCMOSLSC_384c: .ascii "f\0J\0",
_RODATA_G_STCMOSLSC_3850: .ascii "6\0#\0",
_RODATA_G_STCMOSLSC_3854: .ascii "\x15\0\x0f\0",
_RODATA_G_STCMOSLSC_3858: .ascii "\x0b\0\x10\0",
_RODATA_G_STCMOSLSC_385c: .ascii "\x1d\0,\0",
_RODATA_G_STCMOSLSC_3860: .ascii "F\0_\0",
_RODATA_G_STCMOSLSC_3864: .ascii "|\0\x9e\0",
_RODATA_G_STCMOSLSC_3868: .ascii "\xc1\0\xeb\0",
_RODATA_G_STCMOSLSC_386c: .ascii "\x17\x01C\x01|\x01\xb8\x01\xf8\x01I\x02\x93\x02\x98\x02^\x02\xc1\x01\x88\x01Q\x01 \x01\xf3\0",
_RODATA_G_STCMOSLSC_3888: .ascii "\xcf\0\xac\0",
_RODATA_G_STCMOSLSC_388c: .ascii "\x8e\0n\0",
_RODATA_G_STCMOSLSC_3890: .ascii "T\0?\0",
_RODATA_G_STCMOSLSC_3894: .ascii "*\0\x1d\0",
_RODATA_G_STCMOSLSC_3898: .ascii "\x1b\0\x13\0",
_RODATA_G_STCMOSLSC_389c: .ascii "\x1a\0&\0",
_RODATA_G_STCMOSLSC_38a0: .ascii "7\0L\0",
_RODATA_G_STCMOSLSC_38a4: .ascii "g\0\x83\0",
_RODATA_G_STCMOSLSC_38a8: .ascii "\xa9\0\xc6\0",
_RODATA_G_STCMOSLSC_38ac: .ascii "\xf2\0\x1f\x01J\x01\x82\x01\xba\x01\xfe\x01P\x02\x92\x02|\x02Q\x02\xc5\x01\x8b\x01Y\x01)\x01\xfd\0\xd6\0",
_RODATA_G_STCMOSLSC_38cc: .ascii "\xb1\0\x96\0",
_RODATA_G_STCMOSLSC_38d0: .ascii "y\0[\0",
_RODATA_G_STCMOSLSC_38d4: .ascii "G\04\0",
_RODATA_G_STCMOSLSC_38d8: .ascii ".\0-\0",
_RODATA_G_STCMOSLSC_38dc: .ascii "\x1f\0%\0",
_RODATA_G_STCMOSLSC_38e0: .ascii "3\0A\0",
_RODATA_G_STCMOSLSC_38e4: .ascii "V\0n\0",
_RODATA_G_STCMOSLSC_38e8: .ascii "\x8e\0\xb3\0",
_RODATA_G_STCMOSLSC_38ec: .ascii "\xce\0\xf5\0",
_RODATA_G_STCMOSLSC_38f0: .ascii ")\x01U\x01\x87\x01\xc3\x01\x06\x02T\x02\x95\x02X\x02&\x02\xd0\x01\x96\x01b\x01-\x01\x05\x01\xe0\0\xba\0",
_RODATA_G_STCMOSLSC_3910: .ascii "\x9e\0\x80\0",
_RODATA_G_STCMOSLSC_3914: .ascii "f\0P\0",
_RODATA_G_STCMOSLSC_3918: .ascii "B\0@\0",
_RODATA_G_STCMOSLSC_391c: .ascii "9\01\0",
_RODATA_G_STCMOSLSC_3920: .ascii "1\0@\0",
_RODATA_G_STCMOSLSC_3924: .ascii "O\0b\0",
_RODATA_G_STCMOSLSC_3928: .ascii "y\0\x99\0",
_RODATA_G_STCMOSLSC_392c: .ascii "\xb9\0\xd9\0",
_RODATA_G_STCMOSLSC_3930: .ascii "\xfe\01\x01d\x01\x92\x01\xca\x01\x0e\x02[\x02\xa3\x02f\x02#\x02\xdb\x01\xab\x01m\x01:\x01\x11\x01\xe8\0",
_RODATA_G_STCMOSLSC_3950: .ascii "\xc6\0\xa9\0",
_RODATA_G_STCMOSLSC_3954: .ascii "\x8d\0r\0",
_RODATA_G_STCMOSLSC_3958: .ascii "_\0P\0",
_RODATA_G_STCMOSLSC_395c: .ascii "I\0E\0",
_RODATA_G_STCMOSLSC_3960: .ascii "?\0A\0",
_RODATA_G_STCMOSLSC_3964: .ascii "J\0]\0",
_RODATA_G_STCMOSLSC_3968: .ascii "o\0\x82\0",
_RODATA_G_STCMOSLSC_396c: .ascii "\xa3\0\xc3\0",
_RODATA_G_STCMOSLSC_3970: .ascii "\xe1\0\x08\x01:\x01k\x01\x9c\x01\xd5\x01\x19\x02e\x02\xac\x02w\x027\x02\xe4\x01\xb4\x01{\x01C\x01\x1c\x01\xf3\0\xd2\0",
_RODATA_G_STCMOSLSC_3994: .ascii "\xb3\0\x98\0",
_RODATA_G_STCMOSLSC_3998: .ascii "\x81\0m\0",
_RODATA_G_STCMOSLSC_399c: .ascii "^\0R\0",
_RODATA_G_STCMOSLSC_39a0: .ascii "Q\0L\0",
_RODATA_G_STCMOSLSC_39a4: .ascii "R\0X\0",
_RODATA_G_STCMOSLSC_39a8: .ascii "i\0{\0",
_RODATA_G_STCMOSLSC_39ac: .ascii "\x90\0\xac\0",
_RODATA_G_STCMOSLSC_39b0: .ascii "\xcb\0\xeb\0",
_RODATA_G_STCMOSLSC_39b4: .ascii "\x12\x01B\x01q\x01\xa2\x01\xe0\x01'\x02t\x02\xb9\x02\x80\x02G\x02\xf4\x01\xc2\x01\x8a\x01M\x01&\x01\xff\0",
_RODATA_G_STCMOSLSC_39d4: .ascii "\xde\0\xbf\0",
_RODATA_G_STCMOSLSC_39d8: .ascii "\xa5\0\x90\0",
_RODATA_G_STCMOSLSC_39dc: .ascii "~\0n\0",
_RODATA_G_STCMOSLSC_39e0: .ascii "a\0^\0",
_RODATA_G_STCMOSLSC_39e4: .ascii "a\0`\0",
_RODATA_G_STCMOSLSC_39e8: .ascii "g\0u\0",
_RODATA_G_STCMOSLSC_39ec: .ascii "\x87\0\x9d\0",
_RODATA_G_STCMOSLSC_39f0: .ascii "\xbd\0\xd9\0",
_RODATA_G_STCMOSLSC_39f4: .ascii "\xf6\0\x1f\x01N\x01z\x01\xae\x01\xec\x012\x02\x82\x02\xcb\x02\x9a\x02O\x02\x0e\x02\xcb\x01\x91\x01a\x013\x01\r\x01\xea\0",
_RODATA_G_STCMOSLSC_3a18: .ascii "\xce\0\xb3\0",
_RODATA_G_STCMOSLSC_3a1c: .ascii "\x9f\0\x8a\0",
_RODATA_G_STCMOSLSC_3a20: .ascii "|\0q\0",
_RODATA_G_STCMOSLSC_3a24: .ascii "o\0r\0",
_RODATA_G_STCMOSLSC_3a28: .ascii "n\0y\0",
_RODATA_G_STCMOSLSC_3a2c: .ascii "\x88\0\x96\0",
_RODATA_G_STCMOSLSC_3a30: .ascii "\xad\0\xca\0",
_RODATA_G_STCMOSLSC_3a34: .ascii "\xe5\0\x07\x01.\x01\\x01\x8c\x01\xbd\x01\xfa\x01C\x02\x92\x02\xde\x02\xae\x02]\x02\x1f\x02\xd7\x01\xa0\x01o\x01A\x01\x1a\x01\xf8\0\xde\0",
_RODATA_G_STCMOSLSC_3a5c: .ascii "\xc4\0\xad\0",
_RODATA_G_STCMOSLSC_3a60: .ascii "\x99\0\x8c\0",
_RODATA_G_STCMOSLSC_3a64: .ascii "\x84\0\x80\0",
_RODATA_G_STCMOSLSC_3a68: .ascii "\x7f\0\x7f\0",
_RODATA_G_STCMOSLSC_3a6c: .ascii "\x89\0\x97\0",
_RODATA_G_STCMOSLSC_3a70: .ascii "\xa6\0\xbd\0",
_RODATA_G_STCMOSLSC_3a74: .ascii "\xd4\0\xf4\0",
_RODATA_G_STCMOSLSC_3a78: .ascii "\x15\x01;\x01i\x01\x9a\x01\xd1\x01\x06\x02U\x02\xa3\x02\xf2\x02\xbd\x02u\x02/\x02\xeb\x01\xb0\x01\x7f\x01Q\x01-\x01\x0c\x01\xef\0\xd5\0",
_RODATA_G_STCMOSLSC_3aa0: .ascii "\xbe\0\xae\0",
_RODATA_G_STCMOSLSC_3aa4: .ascii "\x9e\0\x97\0",
_RODATA_G_STCMOSLSC_3aa8: .ascii "\x90\0\x8e\0",
_RODATA_G_STCMOSLSC_3aac: .ascii "\x91\0\x9c\0",
_RODATA_G_STCMOSLSC_3ab0: .ascii "\xa8\0\xb9\0",
_RODATA_G_STCMOSLSC_3ab4: .ascii "\xcd\0\xe5\0",
_RODATA_G_STCMOSLSC_3ab8: .ascii "\x05\x01$\x01M\x01w\x01\xae\x01\xe4\x01 \x02k\x02\xb9\x02\x07\x03\xd4\x02\x96\x02C\x02\xfe\x01\xc6\x01\x93\x01e\x01?\x01\x1a\x01\0\x01\xe6\0\xd1\0",
_RODATA_G_STCMOSLSC_3ae4: .ascii "\xc0\0\xb5\0",
_RODATA_G_STCMOSLSC_3ae8: .ascii "\xa8\0\xa0\0",
_RODATA_G_STCMOSLSC_3aec: .ascii "\xa0\0\xa5\0",
_RODATA_G_STCMOSLSC_3af0: .ascii "\xaa\0\xb8\0",
_RODATA_G_STCMOSLSC_3af4: .ascii "\xca\0\xe0\0",
_RODATA_G_STCMOSLSC_3af8: .ascii "\xfc\0\x15\x017\x01]\x01\x8a\x01\xce\x01\xf9\x016\x02\x82\x02\xd4\x02$\x03\xf8\x02\xab\x02]\x02\x15\x02\xde\x01\xa9\x01|\x01P\x01.\x01\x10\x01\xfb\0",
_RODATA_G_STCMOSLSC_3b24: .ascii "\xe6\0\xd2\0",
_RODATA_G_STCMOSLSC_3b28: .ascii "\xc6\0\xba\0",
_RODATA_G_STCMOSLSC_3b2c: .ascii "\xb6\0\xb6\0",
_RODATA_G_STCMOSLSC_3b30: .ascii "\xba\0\xc2\0",
_RODATA_G_STCMOSLSC_3b34: .ascii "\xcd\0\xdd\0",
_RODATA_G_STCMOSLSC_3b38: .ascii "\xf4\0\n\x01'\x01K\x01q\x01\xaa\x01\xec\x01\x0f\x02R\x02\x9a\x02\xe9\x02A\x03\x10\x03\xb8\x02j\x02/\x02\xf0\x01\xb7\x01\x8a\x01[\x01=\x01\x1f\x01\x08\x01\xf2\0",
_RODATA_G_STCMOSLSC_3b68: .ascii "\xde\0\xd4\0",
_RODATA_G_STCMOSLSC_3b6c: .ascii "\xca\0\xc4\0",
_RODATA_G_STCMOSLSC_3b70: .ascii "\xc3\0\xc7\0",
_RODATA_G_STCMOSLSC_3b74: .ascii "\xcf\0\xdb\0",
_RODATA_G_STCMOSLSC_3b78: .ascii "\xf0\0\x01\x01\x1d\x015\x01[\x01\x84\x01\xc1\x01\xff\x01'\x02e\x02\xaa\x02\xfb\x02`\x03\xf0\x02\x9d\x02J\x02\x0b\x02\xd2\x01\xae\x01u\x01Q\x017\x01,\x01\x05\x01\xeb\0\xd7\0",
_RODATA_G_STCMOSLSC_3bac: .ascii "\xc9\0\xbd\0",
_RODATA_G_STCMOSLSC_3bb0: .ascii "\xbd\0\xc0\0",
_RODATA_G_STCMOSLSC_3bb4: .ascii "\xc2\0\xca\0",
_RODATA_G_STCMOSLSC_3bb8: .ascii "\xdc\0\xec\0",
_RODATA_G_STCMOSLSC_3bbc: .ascii "\xfb\0\x0f\x01)\x01K\x01x\x01\xa7\x01\xd2\x01\x1b\x02\\x02\x9c\x02\x05\x03^\x03\xd4\x02\x84\x02>\x02\xff\x01\xc5\x01\x95\x01j\x01C\x01*\x01\x1c\x01\xf5\0\xe1\0",
_RODATA_G_STCMOSLSC_3bec: .ascii "\xcf\0\xbb\0",
_RODATA_G_STCMOSLSC_3bf0: .ascii "\xb1\0\xae\0",
_RODATA_G_STCMOSLSC_3bf4: .ascii "\xb2\0\xb8\0",
_RODATA_G_STCMOSLSC_3bf8: .ascii "\xbf\0\xcd\0",
_RODATA_G_STCMOSLSC_3bfc: .ascii "\xde\0\xed\0",
_RODATA_G_STCMOSLSC_3c00: .ascii "\x02\x01\x1e\x01J\x01o\x01\x95\x01\xc6\x01\t\x02K\x02\x8e\x02\xe7\x02H\x03\xb8\x02o\x02(\x02\xe8\x01\xad\x01\x81\x01U\x011\x01\x14\x01\x02\x01\xe1\0",
_RODATA_G_STCMOSLSC_3c2c: .ascii "\xca\0\xbe\0",
_RODATA_G_STCMOSLSC_3c30: .ascii "\xa8\0\xa0\0",
_RODATA_G_STCMOSLSC_3c34: .ascii "\x9c\0\x9d\0",
_RODATA_G_STCMOSLSC_3c38: .ascii "\xa6\0\xad\0",
_RODATA_G_STCMOSLSC_3c3c: .ascii "\xb4\0\xc1\0",
_RODATA_G_STCMOSLSC_3c40: .ascii "\xd6\0\xf3\0",
_RODATA_G_STCMOSLSC_3c44: .ascii "\x0e\x018\x01_\x01\x83\x01\xb0\x01\xf5\x010\x02x\x02\xcc\x02%\x03\xa5\x02T\x02\x0e\x02\xd6\x01\x9b\x01l\x01F\x01\x1f\x01\xfa\0\xe2\0",
_RODATA_G_STCMOSLSC_3c6c: .ascii "\xcc\0\xbb\0",
_RODATA_G_STCMOSLSC_3c70: .ascii "\xab\0\x97\0",
_RODATA_G_STCMOSLSC_3c74: .ascii "\x8e\0\x8b\0",
_RODATA_G_STCMOSLSC_3c78: .ascii "\x8b\0\x93\0",
_RODATA_G_STCMOSLSC_3c7c: .ascii "\x9c\0\xa2\0",
_RODATA_G_STCMOSLSC_3c80: .ascii "\xac\0\xcb\0",
_RODATA_G_STCMOSLSC_3c84: .ascii "\xe6\0\xfa\0",
_RODATA_G_STCMOSLSC_3c88: .ascii "&\x01M\x01t\x01\xa4\x01\xe7\x01\x1d\x02a\x02\xb8\x02\x0f\x03\x8f\x02C\x02\xfe\x01\xc0\x01\x8b\x01[\x018\x01\x0e\x01\xf1\0",
_RODATA_G_STCMOSLSC_3cac: .ascii "\xd3\0\xbc\0",
_RODATA_G_STCMOSLSC_3cb0: .ascii "\xa8\0\x96\0",
_RODATA_G_STCMOSLSC_3cb4: .ascii "\x89\0~\0",
_RODATA_G_STCMOSLSC_3cb8: .ascii "y\0x\0",
_RODATA_G_STCMOSLSC_3cbc: .ascii "\x83\0\x8a\0",
_RODATA_G_STCMOSLSC_3cc0: .ascii "\x92\0\xa2\0",
_RODATA_G_STCMOSLSC_3cc4: .ascii "\xbf\0\xda\0",
_RODATA_G_STCMOSLSC_3cc8: .ascii "\xee\0\x14\x01;\x01b\x01\x93\x01\xd6\x01\x12\x02S\x02\x9d\x02\xf5\x02q\x02/\x02\xe9\x01\xb1\x01z\x01N\x01&\x01\x03\x01\xe4\0\xc8\0",
_RODATA_G_STCMOSLSC_3cf0: .ascii "\xad\0\x97\0",
_RODATA_G_STCMOSLSC_3cf4: .ascii "\x85\0x\0",
_RODATA_G_STCMOSLSC_3cf8: .ascii "m\0h\0",
_RODATA_G_STCMOSLSC_3cfc: .ascii "j\0n\0",
_RODATA_G_STCMOSLSC_3d00: .ascii "t\0\x81\0",
_RODATA_G_STCMOSLSC_3d04: .ascii "\x90\0\xb0\0",
_RODATA_G_STCMOSLSC_3d08: .ascii "\xcb\0\xe0\0",
_RODATA_G_STCMOSLSC_3d0c: .ascii "\x07\x01*\x01U\x01\x89\x01\xc5\x01\xff\x01@\x02\x8b\x02\xdb\x02^\x02\x1e\x02\xdf\x01\xa8\x01q\x01F\x01\x1b\x01\xf1\0\xd7\0",
_RODATA_G_STCMOSLSC_3d30: .ascii "\xbb\0\x9f\0",
_RODATA_G_STCMOSLSC_3d34: .ascii "\x88\0v\0",
_RODATA_G_STCMOSLSC_3d38: .ascii "g\0]\0",
_RODATA_G_STCMOSLSC_3d3c: .ascii "W\0[\0",
_RODATA_G_STCMOSLSC_3d40: .ascii "Z\0i\0",
_RODATA_G_STCMOSLSC_3d44: .ascii "q\0\x84\0",
_RODATA_G_STCMOSLSC_3d48: .ascii "\xa0\0\xbd\0",
_RODATA_G_STCMOSLSC_3d4c: .ascii "\xd4\0\xf9\0",
_RODATA_G_STCMOSLSC_3d50: .ascii "\x19\x01J\x01{\x01\xae\x01\xe9\x012\x02{\x02\xc8\x02S\x02\x14\x02\xd3\x01\x97\x01k\x01@\x01\x11\x01\xe8\0",
_RODATA_G_STCMOSLSC_3d70: .ascii "\xc5\0\xa8\0",
_RODATA_G_STCMOSLSC_3d74: .ascii "\x90\0|\0",
_RODATA_G_STCMOSLSC_3d78: .ascii "h\0X\0",
_RODATA_G_STCMOSLSC_3d7c: .ascii "N\0F\0",
_RODATA_G_STCMOSLSC_3d80: .ascii "J\0L\0",
_RODATA_G_STCMOSLSC_3d84: .ascii "X\0a\0",
_RODATA_G_STCMOSLSC_3d88: .ascii "s\0\x8d\0",
_RODATA_G_STCMOSLSC_3d8c: .ascii "\xa4\0\xc6\0",
_RODATA_G_STCMOSLSC_3d90: .ascii "\xed\0\x10\x019\x01g\x01\x9f\x01\xdc\x01#\x02k\x02\xab\x02I\x02\x06\x02\xc4\x01\x8c\x01]\x010\x01\x05\x01\xd6\0\xba\0",
_RODATA_G_STCMOSLSC_3db4: .ascii "\x9b\0\x83\0",
_RODATA_G_STCMOSLSC_3db8: .ascii "k\0Z\0",
_RODATA_G_STCMOSLSC_3dbc: .ascii "J\0<\0",
_RODATA_G_STCMOSLSC_3dc0: .ascii ":\07\0",
_RODATA_G_STCMOSLSC_3dc4: .ascii ";\0L\0",
_RODATA_G_STCMOSLSC_3dc8: .ascii "X\0g\0",
_RODATA_G_STCMOSLSC_3dcc: .ascii "\x82\0\x9f\0",
_RODATA_G_STCMOSLSC_3dd0: .ascii "\xc7\0\xe7\0",
_RODATA_G_STCMOSLSC_3dd4: .ascii "\x01\x01/\x01]\x01\x95\x01\xd2\x01\x14\x02\\x02\xa1\x028\x02\xfd\x01\xbb\x01\x7f\x01P\x01#\x01\xf7\0\xd1\0",
_RODATA_G_STCMOSLSC_3df4: .ascii "\xb1\0\x94\0",
_RODATA_G_STCMOSLSC_3df8: .ascii "x\0d\0",
_RODATA_G_STCMOSLSC_3dfc: .ascii "N\0=\0",
_RODATA_G_STCMOSLSC_3e00: .ascii "0\0.\0",
_RODATA_G_STCMOSLSC_3e04: .ascii "(\0.\0",
_RODATA_G_STCMOSLSC_3e08: .ascii ">\0K\0",
_RODATA_G_STCMOSLSC_3e0c: .ascii "`\0w\0",
_RODATA_G_STCMOSLSC_3e10: .ascii "\x9a\0\xc3\0",
_RODATA_G_STCMOSLSC_3e14: .ascii "\xdc\0\xfe\0",
_RODATA_G_STCMOSLSC_3e18: .ascii ")\x01U\x01\x8c\x01\xc9\x01\r\x02V\x02\x95\x024\x02\xf0\x01\xaf\x01u\x01A\x01\x17\x01\xf0\0",
_RODATA_G_STCMOSLSC_3e34: .ascii "\xcb\0\xa8\0",
_RODATA_G_STCMOSLSC_3e38: .ascii "\x8a\0q\0",
_RODATA_G_STCMOSLSC_3e3c: .ascii "X\0>\0",
_RODATA_G_STCMOSLSC_3e40: .ascii "/\0#\0",
_RODATA_G_STCMOSLSC_3e44: .ascii "\x1e\0\x1d\0",
_RODATA_G_STCMOSLSC_3e48: .ascii "#\00\0",
_RODATA_G_STCMOSLSC_3e4c: .ascii "A\0X\0",
_RODATA_G_STCMOSLSC_3e50: .ascii "r\0\x91\0",
_RODATA_G_STCMOSLSC_3e54: .ascii "\xbb\0\xd3\0",
_RODATA_G_STCMOSLSC_3e58: .ascii "\xf3\0\"\x01Q\x01\x80\x01\xc5\x01\x02\x02M\x02\x87\x02\"\x02\xe8\x01\xa6\x01m\x01<\x01\r\x01\xe6\0\xc5\0",
_RODATA_G_STCMOSLSC_3e78: .ascii "\xa6\0\x82\0",
_RODATA_G_STCMOSLSC_3e7c: .ascii "g\0N\0",
_RODATA_G_STCMOSLSC_3e80: .ascii "7\0#\0",
_RODATA_G_STCMOSLSC_3e84: .ascii "\x16\0\x12\0",
_RODATA_G_STCMOSLSC_3e88: .ascii "\x10\0\x18\0",
_RODATA_G_STCMOSLSC_3e8c: .ascii "#\07\0",
_RODATA_G_STCMOSLSC_3e90: .ascii "O\0i\0",
_RODATA_G_STCMOSLSC_3e94: .ascii "\x84\0\xa9\0",
_RODATA_G_STCMOSLSC_3e98: .ascii "\xc7\0\xea\0",
_RODATA_G_STCMOSLSC_3e9c: .ascii "\x1d\x01I\x01x\x01\xbe\x01\xfe\x01G\x02\x8a\x02\x1e\x02\xe0\x01\x9d\x01i\x018\x01\x0b\x01\xe3\0",
_RODATA_G_STCMOSLSC_3eb8: .ascii "\xc3\0\x9d\0",
_RODATA_G_STCMOSLSC_3ebc: .ascii "}\0a\0",
_RODATA_G_STCMOSLSC_3ec0: .ascii "D\0.\0",
_RODATA_G_STCMOSLSC_3ec4: .ascii "\x1d\0\x0f\0",
_RODATA_G_STCMOSLSC_3ec8: .ascii "\x08\0\x06\0",
_RODATA_G_STCMOSLSC_3ecc: .ascii "\x0e\0\x1a\0",
_RODATA_G_STCMOSLSC_3ed0: .ascii "-\0G\0",
_RODATA_G_STCMOSLSC_3ed4: .ascii "]\0|\0",
_RODATA_G_STCMOSLSC_3ed8: .ascii "\x9c\0\xbf\0",
_RODATA_G_STCMOSLSC_3edc: .ascii "\xe6\0\x19\x01A\x01q\x01\xb4\x01\xf5\x01A\x02y\x02\x1b\x02\xde\x01\x9b\x01h\x012\x01\x07\x01\xdf\0\xbc\0",
_RODATA_G_STCMOSLSC_3efc: .ascii "\x9a\0x\0",
_RODATA_G_STCMOSLSC_3f00: .ascii "]\0@\0",
_RODATA_G_STCMOSLSC_3f04: .ascii ")\0\x15\0",
_RODATA_G_STCMOSLSC_3f08: .ascii "\x07\0\0\0",
_RODATA_G_STCMOSLSC_3f0c: .ascii "\0\0\n\0",
_RODATA_G_STCMOSLSC_3f10: .ascii "\x13\0'\0",
_RODATA_G_STCMOSLSC_3f14: .ascii "E\0X\0",
_RODATA_G_STCMOSLSC_3f18: .ascii "w\0\x9b\0",
_RODATA_G_STCMOSLSC_3f1c: .ascii "\xbb\0\xe5\0",
_RODATA_G_STCMOSLSC_3f20: .ascii "\x13\x01<\x01p\x01\xae\x01\xf3\x018\x02v\x02D\x02\xd8\x01\x9f\x01h\x013\x01\x07\x01\xdd\0",
_RODATA_G_STCMOSLSC_3f3c: .ascii "\xba\0\x97\0",
_RODATA_G_STCMOSLSC_3f40: .ascii "u\0V\0",
_RODATA_G_STCMOSLSC_3f44: .ascii "<\0(\0",
_RODATA_G_STCMOSLSC_3f48: .ascii "\x12\0\x04\0",
_RODATA_G_STCMOSLSC_3f4c: .ascii "\x03\0\x01\0",
_RODATA_G_STCMOSLSC_3f50: .ascii "\x04\0\x11\0",
_RODATA_G_STCMOSLSC_3f54: .ascii "!\0=\0",
_RODATA_G_STCMOSLSC_3f58: .ascii "X\0w\0",
_RODATA_G_STCMOSLSC_3f5c: .ascii "\x97\0\xbc\0",
_RODATA_G_STCMOSLSC_3f60: .ascii "\xe1\0\x0f\x01;\x01o\x01\xa9\x01\xe9\x012\x02v\x02\xa3\x02\xec\x01\xa4\x01j\x01/\x01\x02\x01\xdb\0\xba\0",
_RODATA_G_STCMOSLSC_3f80: .ascii "\x94\0p\0",
_RODATA_G_STCMOSLSC_3f84: .ascii "V\0;\0",
_RODATA_G_STCMOSLSC_3f88: .ascii "(\0\x0f\0",
_RODATA_G_STCMOSLSC_3f8c: .ascii "\x01\0\x06\0",
_RODATA_G_STCMOSLSC_3f90: .ascii "\x02\0\x04\0",
_RODATA_G_STCMOSLSC_3f94: .ascii "\x0e\0\x1f\0",
_RODATA_G_STCMOSLSC_3f98: .ascii ":\0U\0",
_RODATA_G_STCMOSLSC_3f9c: .ascii "t\0\x96\0",
_RODATA_G_STCMOSLSC_3fa0: .ascii "\xb5\0\xe0\0",
_RODATA_G_STCMOSLSC_3fa4: .ascii "\r\x01<\x01m\x01\xac\x01\xe5\x010\x02q\x02\xa9\x02\x06\x02\x9e\x01f\x01.\x01\x01\x01\xda\0",
_RODATA_G_STCMOSLSC_3fc0: .ascii "\xb5\0\x92\0",
_RODATA_G_STCMOSLSC_3fc4: .ascii "q\0T\0",
_RODATA_G_STCMOSLSC_3fc8: .ascii ":\0$\0",
_RODATA_G_STCMOSLSC_3fcc: .ascii "\x10\0\x04\0",
_RODATA_G_STCMOSLSC_3fd0: .ascii "\x05\0\x07\0",
_RODATA_G_STCMOSLSC_3fd4: .ascii "\x06\0\r\0",
_RODATA_G_STCMOSLSC_3fd8: .ascii "\x1f\0<\0",
_RODATA_G_STCMOSLSC_3fdc: .ascii "R\0u\0",
_RODATA_G_STCMOSLSC_3fe0: .ascii "\x93\0\xb4\0",
_RODATA_G_STCMOSLSC_3fe4: .ascii "\xe0\0\x0b\x018\x01h\x01\xa7\x01\xe9\x015\x02q\x02\xa6\x02\xf1\x01\x97\x01`\x01)\x01\0\x01\xda\0\xb7\0",
_RODATA_G_STCMOSLSC_4004: .ascii "\x92\0p\0",
_RODATA_G_STCMOSLSC_4008: .ascii "T\0:\0",
_RODATA_G_STCMOSLSC_400c: .ascii "&\0\x12\0",
_RODATA_G_STCMOSLSC_4010: .ascii "\x04\0\x05\0",
_RODATA_G_STCMOSLSC_4014: .ascii "\x05\0\t\0",
_RODATA_G_STCMOSLSC_4018: .ascii "\x0c\0\"\0",
_RODATA_G_STCMOSLSC_401c: .ascii "8\0T\0",
_RODATA_G_STCMOSLSC_4020: .ascii "q\0\x92\0",
_RODATA_G_STCMOSLSC_4024: .ascii "\xb6\0\xe2\0",
_RODATA_G_STCMOSLSC_4028: .ascii "\x0c\x016\x01g\x01\xa4\x01\xe2\x010\x02u\x02g\x02\xd3\x01\x92\x01`\x01(\x01\x01\x01\xd9\0",
_RODATA_G_STCMOSLSC_4044: .ascii "\xbb\0\x96\0",
_RODATA_G_STCMOSLSC_4048: .ascii "w\0V\0",
_RODATA_G_STCMOSLSC_404c: .ascii ";\0*\0",
_RODATA_G_STCMOSLSC_4050: .ascii "\x14\0\x06\0",
_RODATA_G_STCMOSLSC_4054: .ascii "\x03\0\x07\0",
_RODATA_G_STCMOSLSC_4058: .ascii "\x07\0\x10\0",
_RODATA_G_STCMOSLSC_405c: .ascii "&\0=\0",
_RODATA_G_STCMOSLSC_4060: .ascii "W\0t\0",
_RODATA_G_STCMOSLSC_4064: .ascii "\x96\0\xb6\0",
_RODATA_G_STCMOSLSC_4068: .ascii "\xe5\0\x0c\x016\x01h\x01\xa3\x01\xe4\x01-\x02q\x02(\x02\xe9\x01\x97\x01c\x01/\x01\x03\x01\xd9\0\xbb\0",
_RODATA_G_STCMOSLSC_4088: .ascii "\x9a\0|\0",
_RODATA_G_STCMOSLSC_408c: .ascii "\\0E\0",
_RODATA_G_STCMOSLSC_4090: .ascii "/\0\x1c\0",
_RODATA_G_STCMOSLSC_4094: .ascii "\x0b\0\x06\0",
_RODATA_G_STCMOSLSC_4098: .ascii "\x07\0\r\0",
_RODATA_G_STCMOSLSC_409c: .ascii "\x17\0(\0",
_RODATA_G_STCMOSLSC_40a0: .ascii "?\0[\0",
_RODATA_G_STCMOSLSC_40a4: .ascii "w\0\x98\0",
_RODATA_G_STCMOSLSC_40a8: .ascii "\xb8\0\xe7\0",
_RODATA_G_STCMOSLSC_40ac: .ascii "\r\x019\x01j\x01\xa7\x01\xe9\x014\x02y\x02c\x02&\x02\x9a\x01j\x014\x01\x08\x01\xdd\0",
_RODATA_G_STCMOSLSC_40c8: .ascii "\xbc\0\x9d\0",
_RODATA_G_STCMOSLSC_40cc: .ascii "\x80\0e\0",
_RODATA_G_STCMOSLSC_40d0: .ascii "L\07\0",
_RODATA_G_STCMOSLSC_40d4: .ascii "%\0\x16\0",
_RODATA_G_STCMOSLSC_40d8: .ascii "\x0f\0\x0e\0",
_RODATA_G_STCMOSLSC_40dc: .ascii "\x13\0\x1f\0",
_RODATA_G_STCMOSLSC_40e0: .ascii ".\0F\0",
_RODATA_G_STCMOSLSC_40e4: .ascii "^\0y\0",
_RODATA_G_STCMOSLSC_40e8: .ascii "\x9c\0\xbd\0",
_RODATA_G_STCMOSLSC_40ec: .ascii "\xeb\0\x12\x01=\x01p\x01\xb0\x01\xef\x01:\x02\x86\x02q\x026\x02\x9f\x01l\x01<\x01\x0e\x01\xe4\0\xc2\0",
_RODATA_G_STCMOSLSC_410c: .ascii "\x9f\0\x83\0",
_RODATA_G_STCMOSLSC_4110: .ascii "j\0U\0",
_RODATA_G_STCMOSLSC_4114: .ascii "=\0*\0",
_RODATA_G_STCMOSLSC_4118: .ascii "\x1f\0\x19\0",
_RODATA_G_STCMOSLSC_411c: .ascii "\x14\0\x1b\0",
_RODATA_G_STCMOSLSC_4120: .ascii "&\06\0",
_RODATA_G_STCMOSLSC_4124: .ascii "L\0e\0",
_RODATA_G_STCMOSLSC_4128: .ascii "\x84\0\xa4\0",
_RODATA_G_STCMOSLSC_412c: .ascii "\xc2\0\xeb\0",
_RODATA_G_STCMOSLSC_4130: .ascii "\x19\x01F\x01x\x01\xaf\x01\xf3\x01@\x02\x86\x02T\x02.\x02\xa5\x01o\x01C\x01\x11\x01\xf0\0",
_RODATA_G_STCMOSLSC_414c: .ascii "\xc6\0\xa7\0",
_RODATA_G_STCMOSLSC_4150: .ascii "\x8d\0p\0",
_RODATA_G_STCMOSLSC_4154: .ascii "\\0F\0",
_RODATA_G_STCMOSLSC_4158: .ascii "5\0.\0",
_RODATA_G_STCMOSLSC_415c: .ascii "2\0!\0",
_RODATA_G_STCMOSLSC_4160: .ascii "$\03\0",
_RODATA_G_STCMOSLSC_4164: .ascii "@\0U\0",
_RODATA_G_STCMOSLSC_4168: .ascii "l\0\x90\0",
_RODATA_G_STCMOSLSC_416c: .ascii "\xb1\0\xcb\0",
_RODATA_G_STCMOSLSC_4170: .ascii "\xf2\0\x1d\x01P\x01}\x01\xbc\x01\xf9\x01D\x02\x87\x02+\x02\x06\x02\xb0\x01z\x01M\x01\x1b\x01\xf4\0\xce\0",
_RODATA_G_STCMOSLSC_4190: .ascii "\xae\0\x93\0",
_RODATA_G_STCMOSLSC_4194: .ascii "x\0c\0",
_RODATA_G_STCMOSLSC_4198: .ascii "M\0=\0",
_RODATA_G_STCMOSLSC_419c: .ascii "@\08\0",
_RODATA_G_STCMOSLSC_41a0: .ascii "1\01\0",
_RODATA_G_STCMOSLSC_41a4: .ascii "@\0M\0",
_RODATA_G_STCMOSLSC_41a8: .ascii "b\0t\0",
_RODATA_G_STCMOSLSC_41ac: .ascii "\x98\0\xb8\0",
_RODATA_G_STCMOSLSC_41b0: .ascii "\xd7\0\xfb\0",
_RODATA_G_STCMOSLSC_41b4: .ascii "/\x01Z\x01\x8a\x01\xc6\x01\x05\x02H\x02\x90\x026\x02\x01\x02\xbd\x01\x90\x01S\x01$\x01\0\x01\xd9\0\xba\0",
_RODATA_G_STCMOSLSC_41d4: .ascii "\xa1\0\x84\0",
_RODATA_G_STCMOSLSC_41d8: .ascii "k\0W\0",
_RODATA_G_STCMOSLSC_41dc: .ascii "K\0E\0",
_RODATA_G_STCMOSLSC_41e0: .ascii "G\0B\0",
_RODATA_G_STCMOSLSC_41e4: .ascii "D\0P\0",
_RODATA_G_STCMOSLSC_41e8: .ascii "[\0m\0",
_RODATA_G_STCMOSLSC_41ec: .ascii "\x82\0\xa2\0",
_RODATA_G_STCMOSLSC_41f0: .ascii "\xc1\0\xde\0",
_RODATA_G_STCMOSLSC_41f4: .ascii "\x07\x017\x01a\x01\x94\x01\xcc\x01\n\x02W\x02\x97\x02L\x02\x13\x02\xc6\x01\x9b\x01f\x01/\x01\x0b\x01\xe3\0",
_RODATA_G_STCMOSLSC_4214: .ascii "\xc6\0\xaa\0",
_RODATA_G_STCMOSLSC_4218: .ascii "\x90\0w\0",
_RODATA_G_STCMOSLSC_421c: .ascii "g\0[\0",
_RODATA_G_STCMOSLSC_4220: .ascii "L\0M\0",
_RODATA_G_STCMOSLSC_4224: .ascii "M\0Q\0",
_RODATA_G_STCMOSLSC_4228: .ascii "]\0l\0",
_RODATA_G_STCMOSLSC_422c: .ascii "|\0\x8e\0",
_RODATA_G_STCMOSLSC_4230: .ascii "\xab\0\xc9\0",
_RODATA_G_STCMOSLSC_4234: .ascii "\xe8\0\x12\x01@\x01k\x01\x9c\x01\xd6\x01\x1a\x02c\x02\xa7\x02[\x02#\x02\xdb\x01\xa7\x01u\x01<\x01\x15\x01\xf5\0\xd2\0",
_RODATA_G_STCMOSLSC_4258: .ascii "\xb7\0\x9b\0",
_RODATA_G_STCMOSLSC_425c: .ascii "\x86\0v\0",
_RODATA_G_STCMOSLSC_4260: .ascii "h\0[\0",
_RODATA_G_STCMOSLSC_4264: .ascii "[\0_\0",
_RODATA_G_STCMOSLSC_4268: .ascii "a\0i\0",
_RODATA_G_STCMOSLSC_426c: .ascii "x\0\x8a\0",
_RODATA_G_STCMOSLSC_4270: .ascii "\xa0\0\xba\0",
_RODATA_G_STCMOSLSC_4274: .ascii "\xd4\0\xf4\0",
_RODATA_G_STCMOSLSC_4278: .ascii "\x1b\x01I\x01v\x01\xa4\x01\xe0\x01&\x02m\x02\xbb\x02h\x02/\x02\xec\x01\xb1\x01{\x01K\x01!\x01\xff\0",
_RODATA_G_STCMOSLSC_4298: .ascii "\xe0\0\xc5\0",
_RODATA_G_STCMOSLSC_429c: .ascii "\xad\0\x96\0",
_RODATA_G_STCMOSLSC_42a0: .ascii "\x88\0y\0",
_RODATA_G_STCMOSLSC_42a4: .ascii "m\0j\0",
_RODATA_G_STCMOSLSC_42a8: .ascii "n\0l\0",
_RODATA_G_STCMOSLSC_42ac: .ascii "u\0\x87\0",
_RODATA_G_STCMOSLSC_42b0: .ascii "\x96\0\xae\0",
_RODATA_G_STCMOSLSC_42b4: .ascii "\xc9\0\xe2\0",
_RODATA_G_STCMOSLSC_42b8: .ascii "\x03\x01)\x01W\x01\x83\x01\xb2\x01\xeb\x016\x02\x80\x02\xcc\x02}\x024\x02\xfb\x01\xb9\x01\x86\x01X\x012\x01\t\x01\xef\0",
_RODATA_G_STCMOSLSC_42dc: .ascii "\xd5\0\xbc\0",
_RODATA_G_STCMOSLSC_42e0: .ascii "\xa6\0\x97\0",
_RODATA_G_STCMOSLSC_42e4: .ascii "\x89\0\x81\0",
_RODATA_G_STCMOSLSC_42e8: .ascii "}\0}\0",
_RODATA_G_STCMOSLSC_42ec: .ascii "}\0\x86\0",
_RODATA_G_STCMOSLSC_42f0: .ascii "\x94\0\xa4\0",
_RODATA_G_STCMOSLSC_42f4: .ascii "\xba\0\xd5\0",
_RODATA_G_STCMOSLSC_42f8: .ascii "\xf2\0\x12\x016\x01^\x01\x8a\x01\xbc\x01\xfb\x01B\x02\x8e\x02\xda\x02\x8c\x02I\x02\x0e\x02\xcc\x01\x95\x01b\x01@\x01\x1a\x01\0\x01\xe4\0",
_RODATA_G_STCMOSLSC_4320: .ascii "\xcb\0\xb3\0",
_RODATA_G_STCMOSLSC_4324: .ascii "\xa7\0\x98\0",
_RODATA_G_STCMOSLSC_4328: .ascii "\x90\0\x8f\0",
_RODATA_G_STCMOSLSC_432c: .ascii "\x8e\0\x90\0",
_RODATA_G_STCMOSLSC_4330: .ascii "\x97\0\xa2\0",
_RODATA_G_STCMOSLSC_4334: .ascii "\xb3\0\xc9\0",
_RODATA_G_STCMOSLSC_4338: .ascii "\xdf\0\xfe\0",
_RODATA_G_STCMOSLSC_433c: .ascii "\"\x01H\x01m\x01\xa6\x01\xd6\x01\x10\x02Y\x02\xa1\x02\xe8\x02\xa9\x02d\x02\x1f\x02\xda\x01\xaa\x01u\x01Q\x01)\x01\x0e\x01\xf4\0\xdc\0",
_RODATA_G_STCMOSLSC_4364: .ascii "\xca\0\xb7\0",
_RODATA_G_STCMOSLSC_4368: .ascii "\xaa\0\xa3\0",
_RODATA_G_STCMOSLSC_436c: .ascii "\x9c\0\x99\0",
_RODATA_G_STCMOSLSC_4370: .ascii "\x9f\0\xaa\0",
_RODATA_G_STCMOSLSC_4374: .ascii "\xb4\0\xc6\0",
_RODATA_G_STCMOSLSC_4378: .ascii "\xd7\0\xee\0",
_RODATA_G_STCMOSLSC_437c: .ascii "\n\x010\x01V\x01\x83\x01\xc5\x01\xf3\x01)\x02d\x02\xb3\x02\x04\x03\xc0\x02y\x024\x02\xef\x01\xbf\x01\x8d\x01e\x01=\x01\x1d\x01\x06\x01\xeb\0\xdb\0",
_RODATA_G_STCMOSLSC_43a8: .ascii "\xcb\0\xbf\0",
_RODATA_G_STCMOSLSC_43ac: .ascii "\xb6\0\xae\0",
_RODATA_G_STCMOSLSC_43b0: .ascii "\xad\0\xb3\0",
_RODATA_G_STCMOSLSC_43b4: .ascii "\xba\0\xc8\0",
_RODATA_G_STCMOSLSC_43b8: .ascii "\xd6\0\xeb\0",
_RODATA_G_STCMOSLSC_43bc: .ascii "\x03\x01\x1b\x01@\x01f\x01\x9d\x01\xe6\x01\x0b\x02D\x02\x82\x02\xc9\x02 \x03\xd0\x02\x8d\x02E\x02\x04\x02\xda\x01\x9c\x01u\x01K\x01,\x01\x0f\x01\xf9\0",
_RODATA_G_STCMOSLSC_43e8: .ascii "\xe5\0\xdb\0",
_RODATA_G_STCMOSLSC_43ec: .ascii "\xd0\0\xc3\0",
_RODATA_G_STCMOSLSC_43f0: .ascii "\xbc\0\xbb\0",
_RODATA_G_STCMOSLSC_43f4: .ascii "\xbf\0\xc8\0",
_RODATA_G_STCMOSLSC_43f8: .ascii "\xd8\0\xe7\0",
_RODATA_G_STCMOSLSC_43fc: .ascii "\xf7\0\x14\x01+\x01M\x01v\x01\xb2\x01\xf1\x01\x18\x02Y\x02\x9b\x02\xe3\x028\x03\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10\0\x10"


_RODATA_G_STISPANTIFALSECOLOR_0000: .ascii "\x01\0\0\0",
_RODATA_G_STISPANTIFALSECOLOR_0004: .ascii "\n\n\x08\x08\x07\x07\x07\x06\x06\x06\x05\x04\x03\x02\x01\0",
_RODATA_G_STISPANTIFALSECOLOR_0014: .ascii "\x08\x08\x08\x08\x07\x07\x07\x06\x06\x06\x05\x04\x03\x02\x01\0",
_RODATA_G_STISPANTIFALSECOLOR_0024: .ascii ""


_RODATA_G_STISPBAYERNR_0000: .ascii "\x01\0\0\0",
_RODATA_G_STISPBAYERNR_0004: .ascii "\0\0\0\0",
_RODATA_G_STISPBAYERNR_0008: .ascii "\0\0\0\0",
_RODATA_G_STISPBAYERNR_000c: .ascii "`\0\0\x01nnnsxxnKK772.($\x1e\x01\x01\x01\x01\x02\x02\x02\x03\x03\x03\x03\x03\x03\x03\x03\x03\0\0\0\0",
_RODATA_G_STISPBAYERNR_0034: .ascii "\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\0\0\0\0",
_RODATA_G_STISPBAYERNR_0044: .ascii "\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\x01\x01\x01\x01\x02\x02\x02\x03\x03\x03\x03\x03\x03\x03\x03\x03\0\0\0\0",
_RODATA_G_STISPBAYERNR_0064: .ascii "\0\0\0\0",
_RODATA_G_STISPBAYERNR_0068: .ascii "x\0\x8c\0",
_RODATA_G_STISPBAYERNR_006c: .ascii "\x96\0\xc8\0",
_RODATA_G_STISPBAYERNR_0070: .ascii "\xf0\0\x18\x01,\x01T\x01h\x01h\x01T\x01T\x01,\x01,\x01\x18\x01\x18\x01l\0n\0",
_RODATA_G_STISPBAYERNR_008c: .ascii "v\0|\0",
_RODATA_G_STISPBAYERNR_0090: .ascii "\x84\0\x8c\0",
_RODATA_G_STISPBAYERNR_0094: .ascii "\x96\0\xa0\0",
_RODATA_G_STISPBAYERNR_0098: .ascii "\xac\0\xaf\0",
_RODATA_G_STISPBAYERNR_009c: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPBAYERNR_00a0: .ascii "\x94\0\x94\0",
_RODATA_G_STISPBAYERNR_00a4: .ascii "\x8a\0\x8a\0",
_RODATA_G_STISPBAYERNR_00a8: .ascii "l\0n\0",
_RODATA_G_STISPBAYERNR_00ac: .ascii "v\0|\0",
_RODATA_G_STISPBAYERNR_00b0: .ascii "\x84\0\x8c\0",
_RODATA_G_STISPBAYERNR_00b4: .ascii "\x96\0\xa0\0",
_RODATA_G_STISPBAYERNR_00b8: .ascii "\xac\0\xaf\0",
_RODATA_G_STISPBAYERNR_00bc: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPBAYERNR_00c0: .ascii "\x94\0\x94\0",
_RODATA_G_STISPBAYERNR_00c4: .ascii "\x8a\0\x8a\0",
_RODATA_G_STISPBAYERNR_00c8: .ascii "x\0\x8c\0",
_RODATA_G_STISPBAYERNR_00cc: .ascii "\x96\0\xc8\0",
_RODATA_G_STISPBAYERNR_00d0: .ascii "\xf0\0\x18\x01,\x01T\x01h\x01h\x01T\x01T\x01,\x01,\x01\x18\x01\x18\x01\x06\0\x08\0",
_RODATA_G_STISPBAYERNR_00ec: .ascii "\x04\0\x04\0",
_RODATA_G_STISPBAYERNR_00f0: .ascii "\x03\0\x03\0",
_RODATA_G_STISPBAYERNR_00f4: .ascii "\x04\0 \0",
_RODATA_G_STISPBAYERNR_00f8: .ascii "$\0(\0",
_RODATA_G_STISPBAYERNR_00fc: .ascii "4\0<\0",
_RODATA_G_STISPBAYERNR_0100: .ascii "A\0F\0",
_RODATA_G_STISPBAYERNR_0104: .ascii "N\0U\0",
_RODATA_G_STISPBAYERNR_0108: .ascii "<\0<\0",
_RODATA_G_STISPBAYERNR_010c: .ascii "<\0<\0",
_RODATA_G_STISPBAYERNR_0110: .ascii "A\0A\0",
_RODATA_G_STISPBAYERNR_0114: .ascii "A\0A\0",
_RODATA_G_STISPBAYERNR_0118: .ascii "F\0F\0",
_RODATA_G_STISPBAYERNR_011c: .ascii "F\0F\0",
_RODATA_G_STISPBAYERNR_0120: .ascii "F\0F\0",
_RODATA_G_STISPBAYERNR_0124: .ascii "F\0F\0",
_RODATA_G_STISPBAYERNR_0128: .ascii "P\0P\0",
_RODATA_G_STISPBAYERNR_012c: .ascii "P\0U\0",
_RODATA_G_STISPBAYERNR_0130: .ascii "U\0U\0",
_RODATA_G_STISPBAYERNR_0134: .ascii "Z\0Z\0",
_RODATA_G_STISPBAYERNR_0138: .ascii "Z\0_\0",
_RODATA_G_STISPBAYERNR_013c: .ascii "_\0_\0",
_RODATA_G_STISPBAYERNR_0140: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNR_0144: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNR_0148: .ascii "d\0\0\0",
_RODATA_G_STISPBAYERNR_014c: .ascii ""


_RODATA_G_STISPBAYERNRWDR2TO1_0000: .ascii "\x01\0\0\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0004: .ascii "\0\0\0\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0008: .ascii "\0\0\0\0",
_RODATA_G_STISPBAYERNRWDR2TO1_000c: .ascii "`\0\0\x01\x80\x80\x80\x80_KFA<72((\x1e\x1e\x1e\0\x01\x01\x01\x02\x02\x02\x03\x03\x03\x03\x03\x03\x03\x03\x03\0\0\0\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0034: .ascii "\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\0\0\0\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0044: .ascii "\x01\x01\x01\x02\x02\x02\x02\x02\x02\x02\x02\x02\0\x01\x01\x01\x02\x02\x02\x03\x03\x03\x03\x03\x03\x03\x03\x03\x05\x05\x1c\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0064: .ascii "\x1a\x10\0\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0068: .ascii "x\0x\0",
_RODATA_G_STISPBAYERNRWDR2TO1_006c: .ascii "x\0x\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0070: .ascii "x\0x\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0074: .ascii "x\0\x8c\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0078: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPBAYERNRWDR2TO1_007c: .ascii "\xb4\0\xc8\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0080: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0084: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0088: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_008c: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0090: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0094: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0098: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_009c: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00a0: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00a4: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00a8: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00ac: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00b0: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00b4: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00b8: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00bc: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00c0: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00c4: .ascii "n\0n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00c8: .ascii "x\0x\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00cc: .ascii "x\0x\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00d0: .ascii "x\0x\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00d4: .ascii "x\0\x8c\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00d8: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00dc: .ascii "\xb4\0\xc8\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00e0: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00e4: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00e8: .ascii "\n\0\n\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00ec: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00f0: .ascii "#\0(\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00f4: .ascii "-\02\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00f8: .ascii "<\0F\0",
_RODATA_G_STISPBAYERNRWDR2TO1_00fc: .ascii "P\0Z\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0100: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0104: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0108: .ascii "\x96\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_010c: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0110: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0114: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0118: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_011c: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0120: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0124: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0128: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_012c: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0130: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0134: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0138: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_013c: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0140: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0144: .ascii "d\0d\0",
_RODATA_G_STISPBAYERNRWDR2TO1_0148: .ascii "d\0\0\0",
_RODATA_G_STISPBAYERNRWDR2TO1_014c: .ascii ""


_RODATA_G_STISPCA_0000: .ascii "\x01\0\0\0",
_RODATA_G_STISPCA_0004: .ascii "$\0Q\0",
_RODATA_G_STISPCA_0008: .ascii "o\0\x88\0",
_RODATA_G_STISPCA_000c: .ascii "\x9e\0\xb6\0",
_RODATA_G_STISPCA_0010: .ascii "\xcf\0\xe4\0",
_RODATA_G_STISPCA_0014: .ascii "\x03\x01\"\x01=\x01Y\x01q\x01\x8c\x01\xa4\x01\xbc\x01\xd4\x01\xec\x01\x03\x02\x16\x02,\x02>\x02U\x02f\x02x\x02\x88\x02\x9a\x02\xa9\x02\xb9\x02\xc5\x02\xd3\x02\xde\x02\xec\x02\xf6\x02\x03\x03\x0c\x03\x14\x03 \x03(\x03/\x036\x03=\x03E\x03I\x03P\x03V\x03Z\x03`\x03d\x03g\x03n\x03q\x03u\x03z\x03}\x03\x81\x03\x84\x03\x87\x03\x8a\x03\x8d\x03\x90\x03\x93\x03\x96\x03\x99\x03\x9c\x03\x9e\x03\xa1\x03\xa3\x03\xa6\x03\xa8\x03\xaa\x03\xad\x03\xaf\x03\xb1\x03\xb3\x03\xb5\x03\xb7\x03\xb8\x03\xba\x03\xbc\x03\xbe\x03\xc1\x03\xc2\x03\xc4\x03\xc6\x03\xc8\x03\xc9\x03\xca\x03\xcb\x03\xcd\x03\xce\x03\xd0\x03\xd1\x03\xd3\x03\xd4\x03\xd5\x03\xd7\x03\xd8\x03\xd9\x03\xda\x03\xdc\x03\xdd\x03\xde\x03\xdf\x03\xe0\x03\xe1\x03\xe3\x03\xe4\x03\xe5\x03\xe6\x03\xe7\x03\xe8\x03\xe9\x03\xec\x03\xed\x03\xee\x03\xef\x03\xf1\x03\xf2\x03\xf3\x03\xf4\x03\xf6\x03\xf8\x03\xf9\x03\xfb\x03\xfc\x03\xfe\x03\0\x04\x14\x05\x14\x05\xe2\x04\xb0\x04~\x04L\x04\x1a\x04\xe8\x03\xb6\x03\x84\x03\x84\x03 \x03 \x03 \x03 \x03 \x03"


_RODATA_G_STISPCLUT_0000: .ascii "\x01\0\0\0",
_RODATA_G_STISPCLUT_0004: .ascii "\x80\0\0\0",
_RODATA_G_STISPCLUT_0008: .ascii "\x80\0\0\0",
_RODATA_G_STISPCLUT_000c: .ascii "\x80\0\0\0",
_RODATA_G_STISPCLUT_0010: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0014: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0018: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_001c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0020: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0024: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0028: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_002c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0030: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0034: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0038: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_003c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0040: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0044: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0048: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_004c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0050: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0054: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0058: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_005c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0060: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0064: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0068: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_006c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0070: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0074: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0078: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_007c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0080: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0084: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0088: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_008c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0090: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0094: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0098: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_009c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_00a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_00a4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_00ac: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0118: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0120: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0134: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_013c: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0198: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_01a0: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_01c4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_01cc: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0218: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0220: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0254: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_025c: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0298: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_02a0: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_02e4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_02ec: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0318: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0320: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0374: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_037c: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0398: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_03a0: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0404: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_040c: .ascii "@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0418: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0420: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0430: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0440: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0450: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0460: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0470: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0480: .ascii "@\0\x01\x04@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0490: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0494: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0498: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_049c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_04a0: .ascii "@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_04ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_04b0: .ascii "@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_04bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_04c0: .ascii "@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_04cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_04d0: .ascii "@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_04dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_04e0: .ascii "@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_04ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_04f0: .ascii "@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_04fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0500: .ascii "@\0\x01\x04@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_050c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0510: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0518: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_051c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0520: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0524: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0528: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0530: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0538: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_053c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0540: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0548: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_054c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0550: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0558: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_055c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0560: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0568: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_056c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0570: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0578: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_057c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0580: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0588: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_058c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0590: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0598: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_059c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05a0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_05a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05b0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_05b8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_05c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_05fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0600: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0604: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0608: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_060c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0610: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0614: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0618: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_061c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0620: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0624: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0628: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_062c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0630: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0634: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0638: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_063c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0640: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0648: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0650: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0654: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0658: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_065c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0660: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0664: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0668: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_066c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0670: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0674: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0678: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_067c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0680: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0684: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0688: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_068c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0690: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0694: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0698: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_069c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06d0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_06d8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_06e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_06fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0700: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0704: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0708: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_070c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0710: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0714: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0718: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_071c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0720: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0724: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0728: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_072c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0730: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0734: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0738: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_073c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0740: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0744: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0748: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_074c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0750: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0754: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0758: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_075c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0760: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0768: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0770: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0774: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0778: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_077c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0780: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0784: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0788: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_078c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0790: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0794: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0798: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_079c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_07f0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_07f8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0800: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0804: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0808: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_080c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0810: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0814: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0818: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_081c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0820: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0824: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0828: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_082c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0830: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0834: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0838: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_083c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0840: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0844: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0848: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_084c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0850: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0854: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0858: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_085c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0860: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0864: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0868: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_086c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0870: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0874: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0878: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_087c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0880: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0888: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0890: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0894: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0898: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_089c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_08fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0900: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0904: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0908: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_090c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0910: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0918: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_091c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0920: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0924: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_092c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0930: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0934: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_093c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0940: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0944: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_094c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0950: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0954: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_095c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0960: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0964: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_096c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0970: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0974: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_097c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0980: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0984: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_098c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0990: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0994: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0998: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_099c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09a0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_09a8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_09b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_09fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a38: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0a40: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0a48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a50: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0a58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a60: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0a68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a70: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0a78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a80: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0a88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a90: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0a98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0a9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0aa0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0aa8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0aac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ab0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0ab8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0abc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ac0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0ac8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0ad0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ad4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ad8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0adc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ae0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ae4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ae8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0aec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0af0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0af4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0af8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0afc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b50: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0b58: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0b60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0b9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ba0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ba4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ba8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bdc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0be0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0be8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0bf0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bf4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bf8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0bfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c70: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0c78: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0c80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0c9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ca0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ca4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ca8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ccc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cdc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ce0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ce4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ce8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cf0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cf4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cf8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0cfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d00: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0d08: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0d10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d90: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0d98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0d9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0da0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0da4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0dac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0db0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0db4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0dbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0dc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0dc4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0dcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0dd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0dd4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0ddc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0de0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0de4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0dec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0df0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0df4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0dfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e04: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0e0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e20: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0e28: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0e30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0e9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ea0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ea4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ea8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0eac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0eb0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0eb8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0ec0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ec4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ec8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ecc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ed0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ed4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ed8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0edc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ee0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ee4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ee8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0eec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ef0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ef4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ef8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0efc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f48: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0f50: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0f58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f60: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0f68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f70: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0f78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f80: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0f88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f90: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0f98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0f9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0fa0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0fa8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0fac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0fb0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0fb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0fbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0fc0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0fc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0fcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0fd0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0fd8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_0fe0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0fe4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0fe8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0fec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ff0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ff4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ff8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_0ffc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1000: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1004: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1008: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_100c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1010: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1014: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1018: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_101c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1020: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1024: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1028: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_102c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1030: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1034: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1038: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_103c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1040: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1044: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1048: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_104c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1050: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1054: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1058: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_105c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1060: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1068: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1070: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1074: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1078: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_107c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1080: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1084: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1088: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_108c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1090: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1094: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1098: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_109c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_10f0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_10f8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1100: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1104: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1108: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_110c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1110: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1114: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1118: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_111c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1120: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1124: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1128: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_112c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1130: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1134: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1138: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_113c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1140: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1144: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1148: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_114c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1150: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1154: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1158: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_115c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1160: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1164: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1168: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_116c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1170: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1174: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1178: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_117c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1180: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1188: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1190: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1194: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1198: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_119c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_11fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1200: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1204: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1208: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_120c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1210: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1218: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_121c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1220: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1224: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_122c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1230: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1234: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_123c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1240: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1244: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_124c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1250: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1254: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_125c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1260: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1264: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_126c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1270: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1274: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_127c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1280: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1284: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_128c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1290: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1294: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1298: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_129c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12a0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_12a8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_12b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_12fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1300: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1304: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1308: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_130c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1310: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1314: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1318: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_131c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1320: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1324: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1328: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_132c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1330: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1338: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1340: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1344: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1348: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_134c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1350: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1354: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1358: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_135c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1360: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1364: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1368: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_136c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1370: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1374: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1378: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_137c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1380: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1384: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1388: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_138c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1390: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1394: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1398: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_139c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13c0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_13c8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_13d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_13fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1400: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1404: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1408: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_140c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1410: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1414: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1418: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_141c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1420: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1424: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1428: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_142c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1430: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1434: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1438: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_143c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1440: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1444: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1448: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_144c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1450: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1454: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1458: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1460: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1468: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_146c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1470: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1478: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_147c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1480: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1488: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_148c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1490: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1498: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_149c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_14a0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_14a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_14ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_14b0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_14b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_14bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_14c0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_14c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_14cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_14d0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_14d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_14dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_14e0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_14e8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_14f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_14f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_14f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_14fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1500: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1504: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1508: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_150c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1510: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1514: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1518: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_151c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1520: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1524: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1528: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_152c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1530: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1534: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1538: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_153c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1540: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1544: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1548: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_154c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1550: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1554: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1558: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_155c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1560: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1564: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1568: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_156c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1570: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1578: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1580: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1584: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1588: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_158c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1590: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1594: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1598: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_159c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_15fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1600: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1608: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1610: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1614: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1618: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_161c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1620: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1624: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1628: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_162c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1630: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1634: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1638: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_163c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1640: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1644: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1648: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_164c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1650: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1654: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1658: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_165c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1660: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1664: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1668: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_166c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1670: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1674: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1678: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_167c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1680: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1684: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1688: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_168c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1690: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1698: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_169c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_16a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_16a4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_16ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_16b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_16b4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_16bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_16c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_16c4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_16cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_16d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_16d4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_16dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_16e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_16e4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_16ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_16f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_16f4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_16fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1700: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1704: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_170c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1710: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1714: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1718: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_171c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1720: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1728: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1730: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1734: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1738: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_173c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1740: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1744: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1748: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_174c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1750: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1754: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1758: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_175c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1760: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1764: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1768: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_176c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1770: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1774: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1778: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_177c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1780: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1784: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1788: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_178c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1790: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1794: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1798: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_179c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17b0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_17b8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_17c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_17fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1800: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1804: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1808: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_180c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1810: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1814: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1818: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_181c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1820: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1824: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1828: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_182c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1830: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1834: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1838: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_183c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1840: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1848: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1850: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1854: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1858: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_185c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1860: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1864: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1868: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_186c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1870: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1874: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1878: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_187c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1880: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1884: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1888: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_188c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1890: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1894: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1898: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_189c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18d0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_18d8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_18e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_18fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1900: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1904: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1908: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_190c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1910: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1914: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1918: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_191c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1920: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1924: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1928: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_192c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1930: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1934: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1938: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_193c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1940: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1944: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1948: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_194c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1950: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1954: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1958: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_195c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1960: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1964: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1968: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1970: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1978: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_197c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1980: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1988: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_198c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1990: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1998: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_199c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_19a0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_19a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_19ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_19b0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_19b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_19bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_19c0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_19c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_19cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_19d0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_19d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_19dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_19e0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_19e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_19ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_19f0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_19f8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1a00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a80: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1a88: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1a90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1a9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1aa0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1aa4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1aa8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1aac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ab0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ab4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ab8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1abc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ac0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ac4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ac8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1acc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ad0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ad4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ad8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1adc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ae0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ae4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ae8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1aec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1af0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1af4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1af8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1afc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b10: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1b18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b24: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1b2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b34: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1b3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b44: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1b4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b54: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1b5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b64: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1b6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b74: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1b7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b84: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1b8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1b9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ba0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1ba8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1bb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bdc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1be0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1be4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1be8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bf0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bf4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bf8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1bfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c30: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1c38: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1c40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1c9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ca0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ca4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ca8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cc0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1cc8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1cd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cdc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ce0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ce4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ce8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cf0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cf4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cf8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1cfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d50: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1d58: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1d60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1d9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1da0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1da4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1da8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1dac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1db0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1db4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1db8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1dbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1dc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1dc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1dc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1dcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1dd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1dd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1dd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ddc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1de0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1de8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1df0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1df4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1df8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1dfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e78: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1e80: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1e88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e90: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1e98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1e9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ea0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1ea8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1eac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1eb0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1eb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ebc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ec0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1ec8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ecc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ed0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1ed8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1edc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ee0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1ee8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1eec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ef0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1ef8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1efc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f00: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1f08: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1f10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f90: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1f98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1f9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1fa0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1fa4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1fac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1fb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1fb4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1fbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1fc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1fc4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1fcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1fd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1fd4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1fdc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1fe0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1fe4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1fec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ff0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_1ff4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_1ffc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2000: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2004: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_200c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2010: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2014: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2018: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_201c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2020: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2028: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2030: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2034: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2038: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_203c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2040: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2044: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2048: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_204c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2050: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2054: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2058: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_205c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2060: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2064: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2068: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_206c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2070: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2074: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2078: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_207c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2080: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2084: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2088: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_208c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2090: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2094: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2098: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_209c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20b0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_20b8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_20c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_20fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2100: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2104: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2108: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_210c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2110: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2114: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2118: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_211c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2120: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2124: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2128: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_212c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2130: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2134: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2138: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_213c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2140: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2148: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2150: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2154: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2158: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_215c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2160: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2164: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2168: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_216c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2170: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2174: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2178: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_217c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2180: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2184: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2188: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_218c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2190: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2194: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2198: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_219c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21d0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_21d8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_21e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_21fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2200: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2204: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2208: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_220c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2210: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2214: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2218: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_221c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2220: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2224: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2228: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_222c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2230: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2234: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2238: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_223c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2240: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2244: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2248: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_224c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2250: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2254: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2258: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_225c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2260: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2268: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2270: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2274: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2278: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_227c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2280: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2284: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2288: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_228c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2290: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2294: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2298: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_229c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_22f0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_22f8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2300: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2304: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2308: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_230c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2310: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2314: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2318: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_231c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2320: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2324: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2328: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_232c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2330: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2334: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2338: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_233c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2340: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2344: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2348: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_234c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2350: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2354: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2358: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_235c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2360: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2364: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2368: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_236c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2370: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2374: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2378: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_237c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2380: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2384: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2388: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2390: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2398: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_239c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_23a0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_23a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_23ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_23b0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_23b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_23bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_23c0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_23c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_23cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_23d0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_23d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_23dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_23e0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_23e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_23ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_23f0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_23f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_23fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2400: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2408: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_240c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2410: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2418: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_241c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2420: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2424: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_242c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2430: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2434: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_243c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2440: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2444: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_244c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2450: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2454: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_245c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2460: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2464: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_246c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2470: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2474: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_247c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2480: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2484: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_248c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2490: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2494: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2498: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_249c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24a0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_24a8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_24b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_24fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2500: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2504: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2508: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_250c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2510: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2514: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2518: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_251c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2520: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2524: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2528: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_252c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2530: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2538: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2540: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2544: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2548: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_254c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2550: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2554: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2558: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_255c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2560: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2564: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2568: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_256c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2570: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2574: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2578: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_257c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2580: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2584: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2588: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_258c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2590: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2594: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2598: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_259c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25c0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_25c8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_25d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_25fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2600: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2604: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2608: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_260c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2610: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2614: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2618: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_261c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2620: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2624: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2628: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_262c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2630: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2634: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2638: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_263c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2640: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2644: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2648: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_264c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2650: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2658: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2660: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2664: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2668: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_266c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2670: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2674: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2678: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_267c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2680: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2684: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2688: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_268c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2690: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2694: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2698: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_269c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26e0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_26e8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_26f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_26fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2700: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2704: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2708: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_270c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2710: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2714: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2718: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_271c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2720: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2724: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2728: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_272c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2730: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2734: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2738: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_273c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2740: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2744: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2748: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_274c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2750: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2754: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2758: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_275c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2760: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2764: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2768: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_276c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2770: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2778: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2780: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2784: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2788: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_278c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2790: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2794: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2798: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_279c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_27fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2800: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2808: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2810: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2814: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2818: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_281c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2820: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2824: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2828: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_282c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2830: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2834: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2838: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_283c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2840: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2844: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2848: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_284c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2850: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2854: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2858: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_285c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2860: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2864: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2868: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_286c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2870: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2874: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2878: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_287c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2880: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2884: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2888: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_288c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2890: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2894: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2898: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_289c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_28a0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_28a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_28ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_28b0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_28b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_28bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_28c0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_28c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_28cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_28d0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_28d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_28dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_28e0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_28e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_28ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_28f0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_28f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_28fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2900: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2908: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_290c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2910: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2918: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_291c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2920: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2928: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_292c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2930: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2934: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2938: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_293c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2940: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2944: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2948: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_294c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2950: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2954: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2958: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_295c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2960: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2964: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2968: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_296c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2970: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2974: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2978: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_297c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2980: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2984: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2988: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_298c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2990: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2994: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2998: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_299c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29b0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_29b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_29fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a40: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2a48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2a9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2aa0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2aa4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2aa8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2aac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ab0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ab4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ab8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2abc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ac0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ac4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ac8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2acc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ad0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2ad8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2adc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ae0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ae4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ae8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2aec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2af0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2af4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2af8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2afc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b60: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2b68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2b9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ba0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ba4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ba8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bdc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2be0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2be4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2be8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bf0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2bf8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2bfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c80: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2c88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2c9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ca0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ca4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ca8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ccc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cdc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ce0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ce4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ce8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cf0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cf4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cf8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2cfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d10: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_2d18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2d9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2da0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2da4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2da8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2dac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2db0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2db4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2db8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2dbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2dc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2dc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2dc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2dcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2dd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2dd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2dd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ddc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2de0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2de4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2de8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2dec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2df0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2df4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2df8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2dfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2e9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ea0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ea4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ea8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2eac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2eb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2eb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2eb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ebc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ec0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ec4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ec8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ecc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ed0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ed4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ed8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2edc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ee0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ee4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ee8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2eec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ef0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ef4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ef8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2efc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2f9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fa0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fa4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fa8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fdc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fe0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fe4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fe8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2fec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ff0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ff4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ff8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_2ffc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3000: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3004: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3008: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_300c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3010: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3014: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3018: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_301c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3020: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3024: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3028: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_302c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3030: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3034: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3038: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_303c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3040: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3044: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3048: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_304c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3050: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3054: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3058: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_305c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3060: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3064: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3068: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_306c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3070: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3074: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3078: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_307c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3080: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3084: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3088: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_308c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3090: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3094: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3098: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_309c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_30fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3100: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3104: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3108: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_310c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3110: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3114: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3118: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_311c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3120: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3124: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3128: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_312c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3130: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3134: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3138: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_313c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3140: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3144: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3148: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_314c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3150: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3154: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3158: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_315c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3160: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3164: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3168: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_316c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3170: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3174: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3178: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_317c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3180: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3184: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3188: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_318c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3190: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3194: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3198: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_319c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_31fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3200: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3204: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3208: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_320c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3210: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3214: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3218: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_321c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3220: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3224: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3228: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_322c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3230: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3234: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_323c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3240: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3244: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_324c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3250: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3254: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_325c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3260: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3264: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_326c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3270: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3274: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_327c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3280: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3284: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_328c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3290: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3294: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_329c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32b8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_32c0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_32c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32d0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_32d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32e0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_32e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32f0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_32f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_32fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3300: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3308: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_330c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3310: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3318: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_331c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3320: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3328: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_332c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3330: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3338: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_333c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3340: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3348: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3350: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3354: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3358: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_335c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3360: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3364: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3368: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_336c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3370: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3374: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3378: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_337c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3380: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3384: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3388: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_338c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3390: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3394: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3398: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_339c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33d0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_33d8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_33e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_33fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3400: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3404: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3408: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_340c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3410: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3414: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3418: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_341c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3420: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3424: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3428: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_342c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3430: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3434: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3438: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_343c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3440: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3444: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3448: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_344c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3450: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3454: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3458: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_345c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3460: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3468: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3470: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3474: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3478: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_347c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3480: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3484: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3488: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_348c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3490: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3494: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3498: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_349c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_34f0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_34f8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3500: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3504: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3508: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_350c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3510: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3514: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3518: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_351c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3520: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3524: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3528: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_352c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3530: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3534: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3538: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_353c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3540: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3544: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3548: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_354c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3550: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3554: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3558: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_355c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3560: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3564: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3568: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_356c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3570: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3574: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3578: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_357c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3580: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3588: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3590: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3594: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3598: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_359c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_35fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3600: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3604: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3608: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_360c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3610: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3618: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3620: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3624: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3628: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_362c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3630: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3634: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3638: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_363c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3640: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3644: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3648: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_364c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3650: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3654: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3658: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_365c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3660: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3664: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3668: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_366c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3670: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3674: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3678: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_367c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3680: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3684: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3688: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_368c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3690: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3694: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3698: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_369c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_36a0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_36a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_36ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_36b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_36b4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_36bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_36c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_36c4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_36cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_36d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_36d4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_36dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_36e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_36e4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_36ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_36f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_36f4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_36fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3700: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3704: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_370c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3710: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3714: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_371c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3720: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3724: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3728: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_372c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3730: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3738: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3740: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3744: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3748: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_374c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3750: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3754: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3758: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_375c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3760: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3764: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3768: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_376c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3770: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3774: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3778: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_377c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3780: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3784: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3788: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_378c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3790: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3794: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3798: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_379c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37c8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_37d0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_37d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37e0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_37e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37f0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_37f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_37fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3800: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3808: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_380c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3810: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3818: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_381c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3820: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3828: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_382c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3830: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3838: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_383c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3840: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3848: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_384c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3850: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3858: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3860: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3864: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3868: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_386c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3870: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3874: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3878: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_387c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3880: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3884: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3888: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_388c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3890: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3894: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3898: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_389c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38e0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_38e8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_38f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_38fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3900: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3904: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3908: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_390c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3910: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3914: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3918: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_391c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3920: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3924: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3928: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_392c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3930: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3934: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3938: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_393c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3940: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3944: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3948: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_394c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3950: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3954: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3958: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_395c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3960: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3964: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3968: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_396c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3970: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3978: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3980: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3984: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3988: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_398c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3990: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3994: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3998: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_399c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_39fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a00: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3a08: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3a10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3a90: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3a98: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3aa0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3aa4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3aa8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3aac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ab0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ab4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ab8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3abc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ac0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ac4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ac8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3acc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ad0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ad4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ad8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3adc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ae0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ae4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ae8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3aec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3af0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3af4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3af8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3afc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b20: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3b28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b34: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3b3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b44: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3b4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b54: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3b5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b64: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3b6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b74: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3b7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b84: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3b8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3b94: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3b9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ba0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ba4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ba8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bb0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3bb8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3bc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bdc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3be0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3be4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3be8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bf0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bf4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bf8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3bfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c40: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3c48: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3c50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3c9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ca0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ca4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ca8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ccc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cd8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3ce0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3ce8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cf0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3cf8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3cfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d00: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3d08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d10: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3d18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d20: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3d28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d30: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3d38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d40: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3d48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d50: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3d58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d60: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3d68: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3d70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3d9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3da0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3da4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3da8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3dac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3db0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3db4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3db8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3dbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3dc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3dc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3dc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3dcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3dd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3dd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3dd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ddc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3de0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3de4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3de8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3dec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3df0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3df8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3e00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e80: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3e88: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3e90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3e9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ea0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ea4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ea8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3eac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3eb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3eb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3eb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ebc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ec0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ec4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ec8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ecc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ed0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ed4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ed8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3edc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ee0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ee4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ee8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3eec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ef0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ef4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ef8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3efc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f10: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3f18: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3f20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3f9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3fa0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3fa8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3fac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3fb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3fb4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3fbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3fc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3fc4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3fcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3fd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3fd4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3fdc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3fe0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3fe4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3fec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ff0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_3ff4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_3ffc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4000: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4004: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_400c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4010: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4014: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_401c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4020: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4024: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4028: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_402c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4030: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4038: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4040: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4044: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4048: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_404c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4050: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4054: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4058: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_405c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4060: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4064: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4068: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_406c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4070: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4074: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4078: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_407c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4080: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4084: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4088: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_408c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4090: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4094: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4098: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_409c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40c0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_40c8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_40d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_40fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4100: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4104: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4108: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_410c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4110: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4114: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4118: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_411c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4120: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4124: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4128: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_412c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4130: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4134: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4138: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_413c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4140: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4144: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4148: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_414c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4150: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4158: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4160: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4164: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4168: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_416c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4170: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4174: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4178: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_417c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4180: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4184: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4188: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_418c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4190: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4194: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4198: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_419c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41e8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_41f0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_41f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_41fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4200: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4208: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_420c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4210: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4218: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_421c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4220: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4228: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_422c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4230: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4238: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_423c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4240: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4248: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_424c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4250: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4258: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_425c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4260: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4268: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_426c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4270: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4278: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4280: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4284: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4288: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_428c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4290: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4294: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4298: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_429c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_42fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4300: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4308: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4310: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4314: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4318: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_431c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4320: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4324: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4328: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_432c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4330: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4334: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4338: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_433c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4340: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4344: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4348: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_434c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4350: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4354: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4358: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_435c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4360: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4364: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4368: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_436c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4370: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4374: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4378: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_437c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4380: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4384: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4388: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_438c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4390: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4398: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_43a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_43fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4400: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4404: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4408: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_440c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4410: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4414: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4418: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_441c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4420: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4428: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_442c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4430: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4434: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_443c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4440: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4444: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_444c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4450: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4454: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_445c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4460: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4464: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_446c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4470: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4474: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_447c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4480: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4484: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_448c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4490: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4494: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_449c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44b0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_44b8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_44c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_44fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4500: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4504: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4508: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_450c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4510: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4514: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4518: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_451c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4520: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4524: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4528: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_452c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4530: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4534: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4538: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_453c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4540: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4548: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4550: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4554: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4558: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_455c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4560: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4564: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4568: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_456c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4570: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4574: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4578: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_457c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4580: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4584: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4588: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_458c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4590: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4594: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4598: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_459c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45d0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_45d8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_45e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_45fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4600: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4604: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4608: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_460c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4610: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4614: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4618: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_461c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4620: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4624: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4628: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_462c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4630: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4634: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4638: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_463c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4640: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4644: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4648: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_464c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4650: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4654: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4658: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_465c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4660: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4668: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4670: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4674: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4678: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_467c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4680: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4684: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4688: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_468c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4690: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4694: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4698: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_469c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_46f8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4700: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4708: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_470c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4710: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4718: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_471c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4720: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4728: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_472c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4730: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4738: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_473c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4740: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4748: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_474c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4750: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4758: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_475c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4760: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4768: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_476c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4770: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4778: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_477c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4780: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4788: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4790: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4794: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4798: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_479c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_47fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4800: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4804: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4808: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_480c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4810: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4818: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4820: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4824: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4828: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_482c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4830: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4834: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4838: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_483c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4840: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4844: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4848: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_484c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4850: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4854: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4858: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_485c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4860: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4864: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4868: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_486c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4870: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4874: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4878: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_487c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4880: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4884: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4888: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_488c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4890: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4894: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4898: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_489c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_48a0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_48a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_48ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_48b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_48b4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_48bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_48c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_48c4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_48cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_48d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_48d4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_48dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_48e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_48e4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_48ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_48f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_48f4: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_48fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4900: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4904: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_490c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4910: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4914: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_491c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4920: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4924: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4928: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_492c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4930: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4938: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4940: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4944: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4948: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_494c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4950: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4954: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4958: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_495c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4960: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4964: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4968: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_496c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4970: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4974: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4978: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_497c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4980: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4984: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4988: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_498c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4990: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4994: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4998: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_499c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49c0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_49c8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_49d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_49fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a50: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4a58: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4a60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4a9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4aa0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4aa4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4aa8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4aac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ab0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ab4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ab8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4abc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ac0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ac4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ac8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4acc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ad0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ad4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ad8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4adc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ae0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4ae8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4af0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4af4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4af8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4afc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b70: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4b78: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4b80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4b9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ba0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ba4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ba8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bdc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4be0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4be4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4be8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bf0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bf4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bf8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4bfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c08: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4c10: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4c18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c20: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4c28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c30: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4c38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c40: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4c48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c50: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4c58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c60: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4c68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c70: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4c78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c80: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4c88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4c90: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4c98: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4ca0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ca4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ca8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ccc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cdc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ce0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ce4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ce8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cf0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cf4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cf8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4cfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d20: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4d28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d34: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4d3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d44: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4d4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d54: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4d5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d64: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4d6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d74: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4d7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d84: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4d8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4d94: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4d9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4da0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4da4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4da8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4dac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4db0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4db8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4dc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4dc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4dc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4dcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4dd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4dd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4dd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ddc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4de0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4de4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4de8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4dec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4df0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4df4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4df8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4dfc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e40: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4e48: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4e50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e60: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e64: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e68: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e6c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4e9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ea0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ea4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ea8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4eac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4eb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4eb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4eb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ebc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ec0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ec4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ec8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ecc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ed0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4ed8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4ee0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ee4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ee8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4eec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ef0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ef4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ef8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4efc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f00: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f04: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f08: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f0c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f10: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f14: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f18: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f1c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f20: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f24: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f28: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f2c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f30: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f34: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f38: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f3c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f40: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f44: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f48: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f4c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f50: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f54: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f58: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f5c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f60: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4f68: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4f70: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f74: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f78: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f7c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f80: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f84: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f88: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f8c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f90: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f94: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f98: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4f9c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fa0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fa4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fa8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fb0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fb4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fb8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fbc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fc0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fc4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fc8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fcc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fd0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fd4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fd8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fdc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fe0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fe4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fe8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4fec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_4ff0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_4ff8: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5000: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5004: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5008: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_500c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5010: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5014: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5018: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_501c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5020: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5024: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5028: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_502c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5030: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5034: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5038: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_503c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5040: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5044: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5048: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_504c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5050: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5054: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5058: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_505c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5060: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5064: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5068: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_506c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5070: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5074: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5078: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_507c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5080: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5088: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5090: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5094: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5098: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_509c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_50fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5100: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5104: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5108: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_510c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5110: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5114: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5118: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5120: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5128: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_512c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5130: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5138: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_513c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5140: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5148: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_514c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5150: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5158: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_515c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5160: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5168: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_516c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5170: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5178: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_517c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5180: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5188: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_518c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5190: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5198: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_519c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51a0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_51a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_51fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5200: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5204: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5208: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_520c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5210: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5214: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5218: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_521c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5220: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5224: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5228: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_522c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5230: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5238: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_523c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5240: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5244: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5248: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_524c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5250: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5254: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5258: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_525c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5260: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5264: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5268: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_526c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5270: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5274: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5278: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_527c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5280: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5284: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5288: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_528c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5290: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5294: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5298: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_529c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52c0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_52c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_52fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5300: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5304: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5308: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_530c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5310: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5314: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5318: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_531c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5320: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5324: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5328: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_532c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5330: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5334: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5338: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_533c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5340: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5344: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5348: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_534c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5350: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5358: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_535c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5360: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5364: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5368: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_536c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5370: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5374: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5378: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_537c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5380: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5384: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5388: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_538c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5390: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5394: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5398: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_539c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53e0: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_53e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_53fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5400: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5404: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5408: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_540c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5410: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5414: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5418: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_541c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5420: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5424: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5428: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_542c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5430: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5434: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5438: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_543c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5440: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5444: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5448: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_544c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5450: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5454: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5458: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_545c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5460: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5464: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5468: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_546c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5470: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5478: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_547c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5480: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5484: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5488: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_548c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5490: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5494: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5498: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_549c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_54fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5500: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5508: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_550c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5510: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5514: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5518: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_551c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5520: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5524: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5528: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_552c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5530: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5534: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5538: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_553c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5540: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5544: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5548: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_554c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5550: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5554: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5558: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_555c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5560: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5564: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5568: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_556c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5570: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5574: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5578: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_557c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5580: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5584: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5588: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_558c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5590: .ascii "@\0\x01\x04\0\0\0\0",
_RODATA_G_STISPCLUT_5598: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_559c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55a0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55a4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55a8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55ac: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55b0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55b4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55b8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55bc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55c0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55c4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55c8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55cc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55d0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55d4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55d8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55dc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55e0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55e4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55e8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55ec: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55f0: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55f4: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55f8: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_55fc: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5600: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5604: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5608: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_560c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5610: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5614: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5618: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_561c: .ascii "\0\0\0\0",
_RODATA_G_STISPCLUT_5620: .ascii ""


_RODATA_G_STISPDRC_0000: .ascii "\0\0\0\0",
_RODATA_G_STISPDRC_0004: .ascii "\0\x02\0\x02\x02\x02\x08\0",
_RODATA_G_STISPDRC_000c: .ascii "\x06\x06@2#@ @ \0\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\x02\xb46\xb4\0\0\0\0",
_RODATA_G_STISPDRC_0060: .ascii "\xc8\0\x90\x01X\x02\xe8\x03\0\0\xc8\0",
_RODATA_G_STISPDRC_006c: .ascii "\x90\x01X\x02\xe8\x03\xe8\x03\xe8\x03\xe8\x03\xe8\x03\xe8\x03"


_RODATA_G_STISPDRCWDR_0000: .ascii "\x01\0\0\0",
_RODATA_G_STISPDRCWDR_0004: .ascii "\xff\x03\0\x02\x01\x01\t\x08\x03\x03\x1e##  @(\0\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\0\x04\x04\x03\0\x03\xfc\x02\xf8\x02\xf4\x02\xf0\x02\xec\x02\xe8\x02\xe4\x02\xe0\x02\xdc\x02\xd8\x02\xd4\x02\xd0\x02\xcc\x02\xc8\x02\xc4\x02\xc0\x02\xbc\x02\x02\xc8-\x96\0\0\0\0",
_RODATA_G_STISPDRCWDR_0060: .ascii "\xc8\0\x90\x01X\x02\xe8\x03\0\0\xc8\0",
_RODATA_G_STISPDRCWDR_006c: .ascii "\x90\x01X\x02\xe8\x03\xe8\x03\xe8\x03\xe8\x03\xe8\x03\xe8\x03"


_RODATA_G_STISPDEHAZE_0000: .ascii "\0\0\0\0",
_RODATA_G_STISPDEHAZE_0004: .ascii "\0\0\0\0",
_RODATA_G_STISPDEHAZE_0008: .ascii "\0\0\0\0",
_RODATA_G_STISPDEHAZE_000c: .ascii "\x80\x80\0\0",
_RODATA_G_STISPDEHAZE_0010: .ascii ""


_RODATA_G_STISPDEHAZEWDR_0000: .ascii "\x01\0\0\0",
_RODATA_G_STISPDEHAZEWDR_0004: .ascii "\x01\0\0\0",
_RODATA_G_STISPDEHAZEWDR_0008: .ascii "\0\0\0\0",
_RODATA_G_STISPDEHAZEWDR_000c: .ascii "d\x80\0\0",
_RODATA_G_STISPDEHAZEWDR_0010: .ascii ""


_RODATA_G_STISPDEMOSAIC_0000: .ascii "\x01\0\0\0",
_RODATA_G_STISPDEMOSAIC_0004: .ascii "@@@@@@@@@@@@@@@@ \x18\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x03\x03\x03\x05\x07\x07\x07\x07\x07\x07\x07\x07\x07\x07\x07\x07\x01\x02\x02\x03\x03\x03\x04\x04\x05\x05\x07\x07\x07\x07\x07\x07"


_RODATA_G_STISPDEMOSAICWDR_0000: .ascii "\x01\0\0\0",
_RODATA_G_STISPDEMOSAICWDR_0004: .ascii "@@@000    \x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\0\0\0\0",
_RODATA_G_STISPDEMOSAICWDR_0028: .ascii "\0\0\0\0",
_RODATA_G_STISPDEMOSAICWDR_002c: .ascii "\0\0\0\0",
_RODATA_G_STISPDEMOSAICWDR_0030: .ascii "\0\0\0\0",
_RODATA_G_STISPDEMOSAICWDR_0034: .ascii "\x04\x04\x04\x04\x04\x04\x05\x05\x05\x05\x07\x07\x07\x07\x07\x07"


_RODATA_G_STISPDETAIL_0000: .ascii "\x01\0\0\0",
_RODATA_G_STISPDETAIL_0004: .ascii "\0\x01\0\x01\xc8\0\xaa\0",
_RODATA_G_STISPDETAIL_000c: .ascii "\x96\0d\0",
_RODATA_G_STISPDETAIL_0010: .ascii "<\0<\0",
_RODATA_G_STISPDETAIL_0014: .ascii "<\0P\0",
_RODATA_G_STISPDETAIL_0018: .ascii "P\0P\0",
_RODATA_G_STISPDETAIL_001c: .ascii "P\02\0",
_RODATA_G_STISPDETAIL_0020: .ascii "2\0d\0",
_RODATA_G_STISPDETAIL_0024: .ascii "d\0\0\0",
_RODATA_G_STISPDETAIL_0028: .ascii "\0\0\0\0",
_RODATA_G_STISPDETAIL_002c: .ascii "\x96\0\x96\0",
_RODATA_G_STISPDETAIL_0030: .ascii "\x96\0\x96\0",
_RODATA_G_STISPDETAIL_0034: .ascii "\xaa\0\xbe\0",
_RODATA_G_STISPDETAIL_0038: .ascii "\xbe\0\x96\0",
_RODATA_G_STISPDETAIL_003c: .ascii "\x96\0\x96\0",
_RODATA_G_STISPDETAIL_0040: .ascii "\x96\0\x96\0",
_RODATA_G_STISPDETAIL_0044: .ascii "\x96\0\x96\0",
_RODATA_G_STISPDETAIL_0048: .ascii "\x96\0\x96\0",
_RODATA_G_STISPDETAIL_004c: .ascii "\t\0\t\0",
_RODATA_G_STISPDETAIL_0050: .ascii "\t\0\t\0",
_RODATA_G_STISPDETAIL_0054: .ascii "\x08\0\x08\0",
_RODATA_G_STISPDETAIL_0058: .ascii "\x08\0\x08\0",
_RODATA_G_STISPDETAIL_005c: .ascii "\x08\0\x08\0",
_RODATA_G_STISPDETAIL_0060: .ascii "\x08\0\x08\0",
_RODATA_G_STISPDETAIL_0064: .ascii "\x08\0\x08\0",
_RODATA_G_STISPDETAIL_0068: .ascii "\x08\0\x08\0",
_RODATA_G_STISPDETAIL_006c: .ascii "\t\0\t\0",
_RODATA_G_STISPDETAIL_0070: .ascii "\t\0\t\0",
_RODATA_G_STISPDETAIL_0074: .ascii "\x0c\0\x0c\0",
_RODATA_G_STISPDETAIL_0078: .ascii "\x0c\0\x0c\0",
_RODATA_G_STISPDETAIL_007c: .ascii "\x0c\0\x0c\0",
_RODATA_G_STISPDETAIL_0080: .ascii "\x0c\0\x0c\0",
_RODATA_G_STISPDETAIL_0084: .ascii "\x0c\0\x0c\0",
_RODATA_G_STISPDETAIL_0088: .ascii "\x0c\0\x0c\0",
_RODATA_G_STISPDETAIL_008c: .ascii "\x96\0\t\0",
_RODATA_G_STISPDETAIL_0090: .ascii "\t\0\0\0",
_RODATA_G_STISPDETAIL_0094: .ascii ""


_RODATA_G_STISPDETAILWDR2TO1_0000: .ascii "\0\0\0\0",
_RODATA_G_STISPDETAILWDR2TO1_0004: .ascii "\0\x01\0\x01\xc8\0\xaa\0",
_RODATA_G_STISPDETAILWDR2TO1_000c: .ascii "\x96\0d\0",
_RODATA_G_STISPDETAILWDR2TO1_0010: .ascii "<\0<\0",
_RODATA_G_STISPDETAILWDR2TO1_0014: .ascii "<\0P\0",
_RODATA_G_STISPDETAILWDR2TO1_0018: .ascii "P\0P\0",
_RODATA_G_STISPDETAILWDR2TO1_001c: .ascii "P\02\0",
_RODATA_G_STISPDETAILWDR2TO1_0020: .ascii "2\0d\0",
_RODATA_G_STISPDETAILWDR2TO1_0024: .ascii "d\0\0\0",
_RODATA_G_STISPDETAILWDR2TO1_0028: .ascii "\0\0\0\0",
_RODATA_G_STISPDETAILWDR2TO1_002c: .ascii "\x96\0\x96\0",
_RODATA_G_STISPDETAILWDR2TO1_0030: .ascii "\x96\0\x96\0",
_RODATA_G_STISPDETAILWDR2TO1_0034: .ascii "\xaa\0\xbe\0",
_RODATA_G_STISPDETAILWDR2TO1_0038: .ascii "\xbe\0\x96\0",
_RODATA_G_STISPDETAILWDR2TO1_003c: .ascii "\x96\0\x96\0",
_RODATA_G_STISPDETAILWDR2TO1_0040: .ascii "\x96\0\x96\0",
_RODATA_G_STISPDETAILWDR2TO1_0044: .ascii "\x96\0\x96\0",
_RODATA_G_STISPDETAILWDR2TO1_0048: .ascii "\x96\0\x96\0",
_RODATA_G_STISPDETAILWDR2TO1_004c: .ascii "\t\0\t\0",
_RODATA_G_STISPDETAILWDR2TO1_0050: .ascii "\t\0\t\0",
_RODATA_G_STISPDETAILWDR2TO1_0054: .ascii "\x08\0\x08\0",
_RODATA_G_STISPDETAILWDR2TO1_0058: .ascii "\x08\0\x08\0",
_RODATA_G_STISPDETAILWDR2TO1_005c: .ascii "\x08\0\x08\0",
_RODATA_G_STISPDETAILWDR2TO1_0060: .ascii "\x08\0\x08\0",
_RODATA_G_STISPDETAILWDR2TO1_0064: .ascii "\x08\0\x08\0",
_RODATA_G_STISPDETAILWDR2TO1_0068: .ascii "\x08\0\x08\0",
_RODATA_G_STISPDETAILWDR2TO1_006c: .ascii "\t\0\t\0",
_RODATA_G_STISPDETAILWDR2TO1_0070: .ascii "\t\0\t\0",
_RODATA_G_STISPDETAILWDR2TO1_0074: .ascii "\x0c\0\x0c\0",
_RODATA_G_STISPDETAILWDR2TO1_0078: .ascii "\x0c\0\x0c\0",
_RODATA_G_STISPDETAILWDR2TO1_007c: .ascii "\x0c\0\x0c\0",
_RODATA_G_STISPDETAILWDR2TO1_0080: .ascii "\x0c\0\x0c\0",
_RODATA_G_STISPDETAILWDR2TO1_0084: .ascii "\x0c\0\x0c\0",
_RODATA_G_STISPDETAILWDR2TO1_0088: .ascii "\x0c\0\x0c\0",
_RODATA_G_STISPDETAILWDR2TO1_008c: .ascii "\x96\0\t\0",
_RODATA_G_STISPDETAILWDR2TO1_0090: .ascii "\t\0\0\0",
_RODATA_G_STISPDETAILWDR2TO1_0094: .ascii ""


_RODATA_G_STISPEDGEMARK_0000: .ascii "\0\0\0\0",
_RODATA_G_STISPEDGEMARK_0004: .ascii "d\0\0\0",
_RODATA_G_STISPEDGEMARK_0008: .ascii "\0\0\xff\0",
_RODATA_G_STISPEDGEMARK_000c: .ascii ""


_RODATA_G_STISPGAMMA_0000: .ascii "\0\0!\0",
_RODATA_G_STISPGAMMA_0004: .ascii "?\0\\0",
_RODATA_G_STISPGAMMA_0008: .ascii "x\0\x92\0",
_RODATA_G_STISPGAMMA_000c: .ascii "\xac\0\xc4\0",
_RODATA_G_STISPGAMMA_0010: .ascii "\xdc\0\xf3\0",
_RODATA_G_STISPGAMMA_0014: .ascii "\n\x01!\x016\x01K\x01^\x01r\x01\x86\x01\x9a\x01\xaf\x01\xc3\x01\xd6\x01\xe8\x01\xfa\x01\x0b\x02\x1c\x02.\x02@\x02Q\x02b\x02r\x02\x81\x02\x8f\x02\x9e\x02\xad\x02\xbc\x02\xcb\x02\xda\x02\xe8\x02\xf6\x02\x04\x03\x12\x03 \x03.\x03<\x03J\x03W\x03d\x03q\x03~\x03\x8b\x03\x97\x03\xa4\x03\xb0\x03\xbc\x03\xc8\x03\xd5\x03\xe2\x03\xf0\x03\xfe\x03\x0c\x04\x1a\x04'\x042\x04=\x04H\x04S\x04]\x04h\x04r\x04|\x04\x86\x04\x90\x04\x9a\x04\xa4\x04\xae\x04\xb8\x04\xc2\x04\xcc\x04\xd5\x04\xde\x04\xe6\x04\xed\x04\xf3\x04\xf9\x04\0\x05\x08\x05\x10\x05\x19\x05\"\x05*\x052\x05:\x05B\x05J\x05R\x05Z\x05b\x05j\x05q\x05y\x05\x80\x05\x87\x05\x8f\x05\x96\x05\x9e\x05\xa5\x05\xad\x05\xb4\x05\xbb\x05\xc2\x05\xc8\x05\xcf\x05\xd5\x05\xdc\x05\xe2\x05\xe9\x05\xef\x05\xf5\x05\xfb\x05\x01\x06\x07\x06\r\x06\x13\x06\x1a\x06 \x06&\x06,\x062\x068\x06>\x06D\x06I\x06O\x06U\x06Z\x06`\x06f\x06l\x06r\x06w\x06}\x06\x83\x06\x88\x06\x8e\x06\x93\x06\x99\x06\x9e\x06\xa4\x06\xa9\x06\xae\x06\xb3\x06\xb9\x06\xbe\x06\xc3\x06\xc9\x06\xce\x06\xd4\x06\xd9\x06\xdf\x06\xe4\x06\xe9\x06\xee\x06\xf3\x06\xf8\x06\xfd\x06\x02\x07\x07\x07\r\x07\x12\x07\x17\x07\x1c\x07!\x07&\x07+\x07/\x074\x079\x07>\x07C\x07H\x07M\x07R\x07W\x07[\x07`\x07e\x07i\x07n\x07s\x07x\x07|\x07\x81\x07\x86\x07\x8b\x07\x90\x07\x94\x07\x99\x07\x9e\x07\xa2\x07\xa7\x07\xab\x07\xaf\x07\xb4\x07\xb8\x07\xbd\x07\xc1\x07\xc6\x07\xca\x07\xcf\x07\xd4\x07\xd8\x07\xdd\x07\xe1\x07\xe5\x07\xe9\x07\xee\x07\xf2\x07\xf6\x07\xfb\x07\0\x08\x04\x08\x08\x08\r\x08\x11\x08\x15\x08\x19\x08\x1e\x08\"\x08&\x08*\x08.\x082\x086\x08:\x08>\x08C\x08G\x08K\x08P\x08T\x08X\x08\\x08`\x08d\x08h\x08l\x08p\x08t\x08x\x08|\x08\x80\x08\x84\x08\x88\x08\x8c\x08\x90\x08\x94\x08\x98\x08\x9c\x08\xa0\x08\xa4\x08\xa8\x08\xac\x08\xaf\x08\xb3\x08\xb7\x08\xbb\x08\xbf\x08\xc3\x08\xc7\x08\xcb\x08\xcf\x08\xd2\x08\xd6\x08\xda\x08\xdd\x08\xe1\x08\xe5\x08\xe9\x08\xed\x08\xf0\x08\xf4\x08\xf8\x08\xfc\x08\xff\x08\x03\t\x07\t\n\t\x0e\t\x12\t\x16\t\x1a\t\x1d\t!\t%\t(\t+\t/\t3\t6\t:\t>\tB\tE\tI\tL\tO\tS\tV\tZ\t^\ta\te\th\tk\to\tr\tv\tz\t}\t\x81\t\x84\t\x87\t\x8b\t\x8e\t\x92\t\x96\t\x99\t\x9d\t\xa0\t\xa3\t\xa6\t\xaa\t\xad\t\xb0\t\xb4\t\xb8\t\xbb\t\xbe\t\xc2\t\xc5\t\xc8\t\xcb\t\xcf\t\xd3\t\xd6\t\xd9\t\xdd\t\xe0\t\xe3\t\xe6\t\xe9\t\xed\t\xf0\t\xf3\t\xf7\t\xfa\t\xfd\t\0\n\x03\n\x07\n\n\n\r\n\x11\n\x14\n\x17\n\x1a\n\x1d\n!\n$\n'\n+\n.\n1\n4\n7\n:\n=\n@\nD\nG\nJ\nM\nP\nS\nV\nY\n\\n`\nc\nf\ni\nl\no\nr\nu\nx\n{\n~\n\x82\n\x85\n\x88\n\x8b\n\x8e\n\x91\n\x94\n\x97\n\x9a\n\x9d\n\xa0\n\xa3\n\xa6\n\xa9\n\xac\n\xaf\n\xb2\n\xb5\n\xb8\n\xbb\n\xbe\n\xc1\n\xc4\n\xc7\n\xca\n\xcc\n\xcf\n\xd2\n\xd5\n\xd8\n\xdb\n\xde\n\xe1\n\xe4\n\xe7\n\xea\n\xec\n\xef\n\xf2\n\xf5\n\xf8\n\xfb\n\xfe\n\x01\x0b\x04\x0b\x06\x0b\t\x0b\x0c\x0b\x0f\x0b\x12\x0b\x15\x0b\x18\x0b\x1b\x0b\x1d\x0b \x0b#\x0b%\x0b(\x0b+\x0b.\x0b1\x0b4\x0b7\x0b:\x0b=\x0b?\x0bB\x0bE\x0bG\x0bJ\x0bM\x0bP\x0bS\x0bU\x0bX\x0b[\x0b]\x0b`\x0bc\x0bf\x0bh\x0bk\x0bn\x0bq\x0bt\x0bv\x0by\x0b|\x0b~\x0b\x81\x0b\x84\x0b\x87\x0b\x8a\x0b\x8c\x0b\x8f\x0b\x92\x0b\x94\x0b\x96\x0b\x99\x0b\x9c\x0b\x9e\x0b\xa1\x0b\xa4\x0b\xa7\x0b\xaa\x0b\xac\x0b\xaf\x0b\xb2\x0b\xb4\x0b\xb6\x0b\xb9\x0b\xbc\x0b\xbe\x0b\xc1\x0b\xc4\x0b\xc7\x0b\xca\x0b\xcc\x0b\xcf\x0b\xd2\x0b\xd4\x0b\xd7\x0b\xd9\x0b\xdb\x0b\xde\x0b\xe0\x0b\xe3\x0b\xe6\x0b\xe8\x0b\xeb\x0b\xee\x0b\xf1\x0b\xf3\x0b\xf6\x0b\xf8\x0b\xfa\x0b\xfd\x0b\xff\x0b\x02\x0c\x05\x0c\x08\x0c\n\x0c\r\x0c\x10\x0c\x12\x0c\x15\x0c\x17\x0c\x19\x0c\x1c\x0c\x1e\x0c!\x0c$\x0c&\x0c)\x0c+\x0c-\x0c0\x0c2\x0c5\x0c8\x0c:\x0c=\x0c?\x0cB\x0cD\x0cG\x0cI\x0cK\x0cN\x0cP\x0cS\x0cV\x0cX\x0c[\x0c]\x0c_\x0cb\x0cd\x0cg\x0cj\x0cl\x0co\x0cq\x0cs\x0cu\x0cx\x0cz\x0c|\x0c\x7f\x0c\x81\x0c\x84\x0c\x87\x0c\x89\x0c\x8b\x0c\x8e\x0c\x91\x0c\x93\x0c\x96\x0c\x98\x0c\x9a\x0c\x9c\x0c\x9f\x0c\xa1\x0c\xa3\x0c\xa6\x0c\xa9\x0c\xab\x0c\xad\x0c\xaf\x0c\xb2\x0c\xb4\x0c\xb6\x0c\xb9\x0c\xbc\x0c\xbe\x0c\xc0\x0c\xc3\x0c\xc5\x0c\xc7\x0c\xc9\x0c\xcc\x0c\xcf\x0c\xd1\x0c\xd3\x0c\xd6\x0c\xd8\x0c\xda\x0c\xdc\x0c\xdf\x0c\xe2\x0c\xe4\x0c\xe6\x0c\xe9\x0c\xeb\x0c\xed\x0c\xef\x0c\xf1\x0c\xf4\x0c\xf6\x0c\xf8\x0c\xfa\x0c\xfd\x0c\xff\x0c\x01\r\x04\r\x07\r\t\r\x0b\r\x0e\r\x10\r\x12\r\x14\r\x16\r\x19\r\x1b\r\x1d\r \r\"\r$\r&\r(\r+\r-\r/\r2\r4\r6\r8\r:\r=\r?\rA\rD\rF\rH\rJ\rM\rO\rQ\rS\rU\rX\rZ\r\\r_\ra\rc\re\rg\rj\rl\rn\rq\rs\ru\rw\ry\r|\r~\r\x80\r\x83\r\x85\r\x87\r\x89\r\x8b\r\x8d\r\x8f\r\x91\r\x93\r\x96\r\x98\r\x9a\r\x9c\r\x9f\r\xa1\r\xa3\r\xa6\r\xa8\r\xaa\r\xac\r\xae\r\xb0\r\xb2\r\xb4\r\xb7\r\xb9\r\xbb\r\xbd\r\xbf\r\xc1\r\xc3\r\xc5\r\xc8\r\xca\r\xcc\r\xce\r\xd0\r\xd2\r\xd4\r\xd6\r\xd9\r\xdb\r\xdd\r\xdf\r\xe1\r\xe3\r\xe5\r\xe7\r\xe9\r\xec\r\xee\r\xf0\r\xf2\r\xf4\r\xf6\r\xf8\r\xfa\r\xfd\r\xff\r\x01\x0e\x03\x0e\x05\x0e\x07\x0e\t\x0e\x0b\x0e\x0e\x0e\x10\x0e\x12\x0e\x14\x0e\x16\x0e\x18\x0e\x1a\x0e\x1c\x0e\x1e\x0e \x0e\"\x0e$\x0e&\x0e(\x0e*\x0e,\x0e/\x0e1\x0e3\x0e5\x0e7\x0e9\x0e;\x0e=\x0e?\x0eA\x0eC\x0eE\x0eG\x0eI\x0eK\x0eM\x0eO\x0eQ\x0eS\x0eV\x0eX\x0eZ\x0e\\x0e^\x0e`\x0eb\x0ed\x0ef\x0eh\x0ej\x0el\x0en\x0ep\x0er\x0et\x0ev\x0ex\x0ez\x0e|\x0e~\x0e\x80\x0e\x82\x0e\x84\x0e\x86\x0e\x88\x0e\x8a\x0e\x8c\x0e\x8e\x0e\x90\x0e\x92\x0e\x94\x0e\x96\x0e\x98\x0e\x9a\x0e\x9c\x0e\x9e\x0e\xa0\x0e\xa2\x0e\xa4\x0e\xa6\x0e\xa8\x0e\xaa\x0e\xac\x0e\xae\x0e\xb0\x0e\xb2\x0e\xb4\x0e\xb6\x0e\xb7\x0e\xb9\x0e\xbb\x0e\xbd\x0e\xbf\x0e\xc1\x0e\xc3\x0e\xc5\x0e\xc7\x0e\xc9\x0e\xcb\x0e\xcd\x0e\xcf\x0e\xd1\x0e\xd3\x0e\xd5\x0e\xd7\x0e\xd9\x0e\xdb\x0e\xdc\x0e\xde\x0e\xe0\x0e\xe2\x0e\xe4\x0e\xe6\x0e\xe8\x0e\xea\x0e\xec\x0e\xee\x0e\xf0\x0e\xf2\x0e\xf4\x0e\xf6\x0e\xf8\x0e\xfa\x0e\xfc\x0e\xfd\x0e\xff\x0e\x01\x0f\x03\x0f\x05\x0f\x07\x0f\t\x0f\x0b\x0f\r\x0f\x0f\x0f\x11\x0f\x12\x0f\x14\x0f\x16\x0f\x18\x0f\x1a\x0f\x1c\x0f\x1e\x0f \x0f!\x0f#\x0f%\x0f'\x0f)\x0f+\x0f-\x0f/\x0f0\x0f2\x0f4\x0f6\x0f8\x0f:\x0f<\x0f>\x0f@\x0fA\x0fC\x0fE\x0fG\x0fI\x0fK\x0fM\x0fO\x0fP\x0fR\x0fT\x0fV\x0fX\x0fZ\x0f\\x0f^\x0f_\x0fa\x0fc\x0fe\x0fg\x0fi\x0fk\x0fm\x0fn\x0fp\x0fr\x0fs\x0fu\x0fw\x0fy\x0f{\x0f}\x0f\x7f\x0f\x81\x0f\x83\x0f\x84\x0f\x86\x0f\x88\x0f\x89\x0f\x8b\x0f\x8d\x0f\x8f\x0f\x91\x0f\x93\x0f\x95\x0f\x97\x0f\x99\x0f\x9a\x0f\x9c\x0f\x9e\x0f\xa0\x0f\xa1\x0f\xa3\x0f\xa5\x0f\xa6\x0f\xa8\x0f\xaa\x0f\xac\x0f\xae\x0f\xb0\x0f\xb2\x0f\xb4\x0f\xb6\x0f\xb7\x0f\xb9\x0f\xbb\x0f\xbc\x0f\xbe\x0f\xc0\x0f\xc2\x0f\xc4\x0f\xc5\x0f\xc7\x0f\xc9\x0f\xca\x0f\xcc\x0f\xce\x0f\xd0\x0f\xd2\x0f\xd4\x0f\xd6\x0f\xd8\x0f\xda\x0f\xdb\x0f\xdd\x0f\xdf\x0f\xe0\x0f\xe2\x0f\xe4\x0f\xe6\x0f\xe8\x0f\xe9\x0f\xeb\x0f\xed\x0f\xee\x0f\xf0\x0f\xf2\x0f\xf4\x0f\xf6\x0f\xf7\x0f\xf9\x0f\xfb\x0f\xfc\x0f\xfe\x0f\xff\x0f"


_RODATA_G_STISPGAMMAFSWDR_0000: .ascii "\0\0\x17\0",
_RODATA_G_STISPGAMMAFSWDR_0004: .ascii "&\04\0",
_RODATA_G_STISPGAMMAFSWDR_0008: .ascii "@\0L\0",
_RODATA_G_STISPGAMMAFSWDR_000c: .ascii "W\0a\0",
_RODATA_G_STISPGAMMAFSWDR_0010: .ascii "l\0v\0",
_RODATA_G_STISPGAMMAFSWDR_0014: .ascii "\x7f\0\x89\0",
_RODATA_G_STISPGAMMAFSWDR_0018: .ascii "\x92\0\x9b\0",
_RODATA_G_STISPGAMMAFSWDR_001c: .ascii "\xa4\0\xac\0",
_RODATA_G_STISPGAMMAFSWDR_0020: .ascii "\xb5\0\xbd\0",
_RODATA_G_STISPGAMMAFSWDR_0024: .ascii "\xc6\0\xce\0",
_RODATA_G_STISPGAMMAFSWDR_0028: .ascii "\xd6\0\xde\0",
_RODATA_G_STISPGAMMAFSWDR_002c: .ascii "\xe6\0\xee\0",
_RODATA_G_STISPGAMMAFSWDR_0030: .ascii "\xf5\0\xfd\0",
_RODATA_G_STISPGAMMAFSWDR_0034: .ascii "\x04\x01\x0c\x01\x13\x01\x1b\x01\"\x01)\x010\x017\x01?\x01F\x01L\x01S\x01Z\x01a\x01h\x01o\x01u\x01|\x01\x82\x01\x89\x01\x90\x01\x96\x01\x9d\x01\xa3\x01\xa9\x01\xb0\x01\xb6\x01\xbc\x01\xc3\x01\xc9\x01\xcf\x01\xd5\x01\xdb\x01\xe2\x01\xe8\x01\xee\x01\xf4\x01\xfa\x01\0\x02\x06\x02\x0c\x02\x12\x02\x18\x02\x1e\x02#\x02)\x02/\x025\x02;\x02A\x02F\x02L\x02R\x02W\x02]\x02c\x02h\x02n\x02t\x02y\x02\x7f\x02\x84\x02\x8a\x02\x8f\x02\x95\x02\x9b\x02\xa0\x02\xa5\x02\xab\x02\xb0\x02\xb6\x02\xbb\x02\xc1\x02\xc6\x02\xcb\x02\xd1\x02\xd6\x02\xdb\x02\xe1\x02\xe6\x02\xeb\x02\xf1\x02\xf6\x02\xfb\x02\0\x03\x06\x03\x0b\x03\x10\x03\x15\x03\x1a\x03 \x03%\x03*\x03/\x034\x039\x03>\x03D\x03I\x03N\x03S\x03X\x03]\x03b\x03g\x03l\x03q\x03v\x03{\x03\x80\x03\x85\x03\x8a\x03\x8f\x03\x94\x03\x99\x03\x9e\x03\xa3\x03\xa7\x03\xac\x03\xb1\x03\xb6\x03\xbb\x03\xc0\x03\xc5\x03\xca\x03\xce\x03\xd3\x03\xd8\x03\xdd\x03\xe2\x03\xe7\x03\xeb\x03\xf0\x03\xf5\x03\xfa\x03\xfe\x03\x03\x04\x08\x04\r\x04\x11\x04\x16\x04\x1b\x04 \x04$\x04)\x04.\x042\x047\x04<\x04@\x04E\x04J\x04N\x04S\x04X\x04\\x04a\x04f\x04j\x04o\x04s\x04x\x04}\x04\x81\x04\x86\x04\x8a\x04\x8f\x04\x93\x04\x98\x04\x9c\x04\xa1\x04\xa6\x04\xaa\x04\xaf\x04\xb3\x04\xb8\x04\xbc\x04\xc1\x04\xc5\x04\xca\x04\xce\x04\xd3\x04\xd7\x04\xdb\x04\xe0\x04\xe4\x04\xe9\x04\xed\x04\xf2\x04\xf6\x04\xfb\x04\xff\x04\x03\x05\x08\x05\x0c\x05\x11\x05\x15\x05\x19\x05\x1e\x05\"\x05'\x05+\x05/\x054\x058\x05<\x05A\x05E\x05I\x05N\x05R\x05V\x05[\x05_\x05c\x05h\x05l\x05p\x05u\x05y\x05}\x05\x81\x05\x86\x05\x8a\x05\x8e\x05\x93\x05\x97\x05\x9b\x05\x9f\x05\xa4\x05\xa8\x05\xac\x05\xb0\x05\xb5\x05\xb9\x05\xbd\x05\xc1\x05\xc5\x05\xca\x05\xce\x05\xd2\x05\xd6\x05\xda\x05\xdf\x05\xe3\x05\xe7\x05\xeb\x05\xef\x05\xf3\x05\xf8\x05\xfc\x05\0\x06\x04\x06\x08\x06\x0c\x06\x11\x06\x15\x06\x19\x06\x1d\x06!\x06%\x06)\x06.\x062\x066\x06:\x06>\x06B\x06F\x06J\x06N\x06R\x06W\x06[\x06_\x06c\x06g\x06k\x06o\x06s\x06w\x06{\x06\x7f\x06\x83\x06\x87\x06\x8b\x06\x8f\x06\x93\x06\x97\x06\x9b\x06\x9f\x06\xa4\x06\xa8\x06\xac\x06\xb0\x06\xb4\x06\xb8\x06\xbc\x06\xc0\x06\xc4\x06\xc8\x06\xcc\x06\xd0\x06\xd4\x06\xd8\x06\xdb\x06\xdf\x06\xe3\x06\xe7\x06\xeb\x06\xef\x06\xf3\x06\xf7\x06\xfb\x06\xff\x06\x03\x07\x07\x07\x0b\x07\x0f\x07\x13\x07\x17\x07\x1b\x07\x1f\x07#\x07'\x07*\x07.\x072\x076\x07:\x07>\x07B\x07F\x07J\x07N\x07R\x07U\x07Y\x07]\x07a\x07e\x07i\x07m\x07q\x07t\x07x\x07|\x07\x80\x07\x84\x07\x88\x07\x8c\x07\x8f\x07\x93\x07\x97\x07\x9b\x07\x9f\x07\xa3\x07\xa7\x07\xaa\x07\xae\x07\xb2\x07\xb6\x07\xba\x07\xbd\x07\xc1\x07\xc5\x07\xc9\x07\xcd\x07\xd1\x07\xd4\x07\xd8\x07\xdc\x07\xe0\x07\xe4\x07\xe7\x07\xeb\x07\xef\x07\xf3\x07\xf7\x07\xfa\x07\xfe\x07\x02\x08\x06\x08\t\x08\r\x08\x11\x08\x15\x08\x18\x08\x1c\x08 \x08$\x08(\x08+\x08/\x083\x087\x08:\x08>\x08B\x08E\x08I\x08M\x08Q\x08T\x08X\x08\\x08`\x08c\x08g\x08k\x08n\x08r\x08v\x08z\x08}\x08\x81\x08\x85\x08\x88\x08\x8c\x08\x90\x08\x93\x08\x97\x08\x9b\x08\x9f\x08\xa2\x08\xa6\x08\xaa\x08\xad\x08\xb1\x08\xb5\x08\xb8\x08\xbc\x08\xc0\x08\xc3\x08\xc7\x08\xcb\x08\xce\x08\xd2\x08\xd6\x08\xd9\x08\xdd\x08\xe1\x08\xe4\x08\xe8\x08\xec\x08\xef\x08\xf3\x08\xf6\x08\xfa\x08\xfe\x08\x01\t\x05\t\t\t\x0c\t\x10\t\x13\t\x17\t\x1b\t\x1e\t\"\t&\t)\t-\t0\t4\t8\t;\t?\tB\tF\tJ\tM\tQ\tT\tX\t\\t_\tc\tf\tj\tm\tq\tu\tx\t|\t\x7f\t\x83\t\x86\t\x8a\t\x8e\t\x91\t\x95\t\x98\t\x9c\t\x9f\t\xa3\t\xa6\t\xaa\t\xae\t\xb1\t\xb5\t\xb8\t\xbc\t\xbf\t\xc3\t\xc6\t\xca\t\xcd\t\xd1\t\xd4\t\xd8\t\xdc\t\xdf\t\xe3\t\xe6\t\xea\t\xed\t\xf1\t\xf4\t\xf8\t\xfb\t\xff\t\x02\n\x06\n\t\n\r\n\x10\n\x14\n\x17\n\x1b\n\x1e\n\"\n%\n)\n,\n0\n3\n7\n:\n>\nA\nE\nH\nL\nO\nR\nV\nY\n]\n`\nd\ng\nk\nn\nr\nu\ny\n|\n\x7f\n\x83\n\x86\n\x8a\n\x8d\n\x91\n\x94\n\x98\n\x9b\n\x9e\n\xa2\n\xa5\n\xa9\n\xac\n\xb0\n\xb3\n\xb6\n\xba\n\xbd\n\xc1\n\xc4\n\xc8\n\xcb\n\xce\n\xd2\n\xd5\n\xd9\n\xdc\n\xe0\n\xe3\n\xe6\n\xea\n\xed\n\xf1\n\xf4\n\xf7\n\xfb\n\xfe\n\x02\x0b\x05\x0b\x08\x0b\x0c\x0b\x0f\x0b\x13\x0b\x16\x0b\x19\x0b\x1d\x0b \x0b#\x0b'\x0b*\x0b.\x0b1\x0b4\x0b8\x0b;\x0b>\x0bB\x0bE\x0bI\x0bL\x0bO\x0bS\x0bV\x0bY\x0b]\x0b`\x0bd\x0bg\x0bj\x0bn\x0bq\x0bt\x0bx\x0b{\x0b~\x0b\x82\x0b\x85\x0b\x88\x0b\x8c\x0b\x8f\x0b\x92\x0b\x96\x0b\x99\x0b\x9c\x0b\xa0\x0b\xa3\x0b\xa6\x0b\xaa\x0b\xad\x0b\xb0\x0b\xb4\x0b\xb7\x0b\xba\x0b\xbe\x0b\xc1\x0b\xc4\x0b\xc8\x0b\xcb\x0b\xce\x0b\xd2\x0b\xd5\x0b\xd8\x0b\xdc\x0b\xdf\x0b\xe2\x0b\xe6\x0b\xe9\x0b\xec\x0b\xef\x0b\xf3\x0b\xf6\x0b\xf9\x0b\xfd\x0b\0\x0c\x03\x0c\x07\x0c\n\x0c\r\x0c\x10\x0c\x14\x0c\x17\x0c\x1a\x0c\x1e\x0c!\x0c$\x0c(\x0c+\x0c.\x0c1\x0c5\x0c8\x0c;\x0c?\x0cB\x0cE\x0cH\x0cL\x0cO\x0cR\x0cU\x0cY\x0c\\x0c_\x0cb\x0cf\x0ci\x0cl\x0cp\x0cs\x0cv\x0cy\x0c}\x0c\x80\x0c\x83\x0c\x86\x0c\x8a\x0c\x8d\x0c\x90\x0c\x93\x0c\x97\x0c\x9a\x0c\x9d\x0c\xa0\x0c\xa4\x0c\xa7\x0c\xaa\x0c\xad\x0c\xb1\x0c\xb4\x0c\xb7\x0c\xba\x0c\xbe\x0c\xc1\x0c\xc4\x0c\xc7\x0c\xca\x0c\xce\x0c\xd1\x0c\xd4\x0c\xd7\x0c\xdb\x0c\xde\x0c\xe1\x0c\xe4\x0c\xe7\x0c\xeb\x0c\xee\x0c\xf1\x0c\xf4\x0c\xf8\x0c\xfb\x0c\xfe\x0c\x01\r\x04\r\x08\r\x0b\r\x0e\r\x11\r\x14\r\x18\r\x1b\r\x1e\r!\r%\r(\r+\r.\r1\r5\r8\r;\r>\rA\rD\rH\rK\rN\rQ\rT\rX\r[\r^\ra\rd\rh\rk\rn\rq\rt\rw\r{\r~\r\x81\r\x84\r\x87\r\x8a\r\x8e\r\x91\r\x94\r\x97\r\x9a\r\x9d\r\xa1\r\xa4\r\xa7\r\xaa\r\xad\r\xb0\r\xb4\r\xb7\r\xba\r\xbd\r\xc0\r\xc3\r\xc7\r\xca\r\xcd\r\xd0\r\xd3\r\xd6\r\xd9\r\xdd\r\xe0\r\xe3\r\xe6\r\xe9\r\xec\r\xef\r\xf3\r\xf6\r\xf9\r\xfc\r\xff\r\x02\x0e\x05\x0e\t\x0e\x0c\x0e\x0f\x0e\x12\x0e\x15\x0e\x18\x0e\x1b\x0e\x1e\x0e\"\x0e%\x0e(\x0e+\x0e.\x0e1\x0e4\x0e7\x0e;\x0e>\x0eA\x0eD\x0eG\x0eJ\x0eM\x0eP\x0eS\x0eW\x0eZ\x0e]\x0e`\x0ec\x0ef\x0ei\x0el\x0eo\x0es\x0ev\x0ey\x0e|\x0e\x7f\x0e\x82\x0e\x85\x0e\x88\x0e\x8b\x0e\x8e\x0e\x92\x0e\x95\x0e\x98\x0e\x9b\x0e\x9e\x0e\xa1\x0e\xa4\x0e\xa7\x0e\xaa\x0e\xad\x0e\xb0\x0e\xb4\x0e\xb7\x0e\xba\x0e\xbd\x0e\xc0\x0e\xc3\x0e\xc6\x0e\xc9\x0e\xcc\x0e\xcf\x0e\xd2\x0e\xd5\x0e\xd8\x0e\xdc\x0e\xdf\x0e\xe2\x0e\xe5\x0e\xe8\x0e\xeb\x0e\xee\x0e\xf1\x0e\xf4\x0e\xf7\x0e\xfa\x0e\xfd\x0e\0\x0f\x03\x0f\x07\x0f\n\x0f\r\x0f\x10\x0f\x13\x0f\x16\x0f\x19\x0f\x1c\x0f\x1f\x0f\"\x0f%\x0f(\x0f+\x0f.\x0f1\x0f4\x0f7\x0f:\x0f>\x0fA\x0fD\x0fG\x0fJ\x0fM\x0fP\x0fS\x0fV\x0fY\x0f\\x0f_\x0fb\x0fe\x0fh\x0fk\x0fn\x0fq\x0ft\x0fw\x0fz\x0f}\x0f\x80\x0f\x83\x0f\x86\x0f\x89\x0f\x8c\x0f\x90\x0f\x93\x0f\x96\x0f\x99\x0f\x9c\x0f\x9f\x0f\xa2\x0f\xa5\x0f\xa8\x0f\xab\x0f\xae\x0f\xb1\x0f\xb4\x0f\xb7\x0f\xba\x0f\xbd\x0f\xc0\x0f\xc3\x0f\xc6\x0f\xc9\x0f\xcc\x0f\xcf\x0f\xd2\x0f\xd5\x0f\xd8\x0f\xdb\x0f\xde\x0f\xe1\x0f\xe4\x0f\xe7\x0f\xea\x0f\xed\x0f\xf0\x0f\xf3\x0f\xf6\x0f\xf9\x0f\xfc\x0f\xff\x0f"


_RODATA_G_STISPGE_0000: .ascii "\x01\0\0\0",
_RODATA_G_STISPGE_0004: .ascii "\t\t,\x01,\x01,\x01,\x01,\x016\x016\x016\x016\x01@\x01@\x01@\x01@\x01J\x01J\x01J\x01J\x01\x80\0\x80\0",
_RODATA_G_STISPGE_002c: .ascii "\x80\0\x80\0",
_RODATA_G_STISPGE_0030: .ascii "\x81\0\x81\0",
_RODATA_G_STISPGE_0034: .ascii "\x81\0\x81\0",
_RODATA_G_STISPGE_0038: .ascii "\x82\0\x82\0",
_RODATA_G_STISPGE_003c: .ascii "\x82\0\x82\0",
_RODATA_G_STISPGE_0040: .ascii "\x83\0\x83\0",
_RODATA_G_STISPGE_0044: .ascii "\x83\0\x83\0",
_RODATA_G_STISPGE_0048: .ascii "\0\x04\0\x04\0\x04\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08"


_RODATA_G_STISPHLC_0000: .ascii "\0\0\0\0",
_RODATA_G_STISPHLC_0004: .ascii "\xf8\n\0\0",
_RODATA_G_STISPHLC_0008: .ascii ""


_RODATA_G_STISPLDCI_0000: .ascii "\x01\0\0\0",
_RODATA_G_STISPLDCI_0004: .ascii "$2-*(# \x0c\x08\x06\0\0",
_RODATA_G_STISPLDCI_0010: .ascii "\0\0\0\0",
_RODATA_G_STISPLDCI_0014: .ascii "\0PPHHH@\x18\x14\x0c\x08\x06\x02\x01\x01\x01\x01\0\0\0",
_RODATA_G_STISPLDCI_0028: .ascii "\0\0\0\0",
_RODATA_G_STISPLDCI_002c: .ascii "\0\0\0\0",
_RODATA_G_STISPLDCI_0030: .ascii "\0\0\0\0",
_RODATA_G_STISPLDCI_0034: .ascii "\02----\x18\x0c\x08\x06\0\0",
_RODATA_G_STISPLDCI_0040: .ascii "\0\0\0\0",
_RODATA_G_STISPLDCI_0044: .ascii "\0PPPPPH@6$\x08\x06\x02\x01\x01\x01\x01\x80\x80\x80\0\0\0\0",
_RODATA_G_STISPLDCI_005c: .ascii "\0\0\0\0",
_RODATA_G_STISPLDCI_0060: .ascii "\0\0\0\0",
_RODATA_G_STISPLDCI_0064: .ascii "\0\0\x14\0",
_RODATA_G_STISPLDCI_0068: .ascii "\x14\0\x14\0",
_RODATA_G_STISPLDCI_006c: .ascii "\x14\0\x1e\0",
_RODATA_G_STISPLDCI_0070: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPLDCI_0074: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPLDCI_0078: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPLDCI_007c: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPLDCI_0080: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPLDCI_0084: .ascii "\x1e\0\0\0",
_RODATA_G_STISPLDCI_0088: .ascii ""


_RODATA_G_STISPWDR_0000: .ascii "\0\0\0\0",
_RODATA_G_STISPWDR_0004: .ascii "\x01\0\0\0",
_RODATA_G_STISPWDR_0008: .ascii "\xc0\x0f\xc0\x0b\x01\0\0\0",
_RODATA_G_STISPWDR_0010: .ascii "\xf4\x01\xbc\x02\0\0\0\0",
_RODATA_G_STISPWDR_0018: .ascii "\x08\0\0\0",
_RODATA_G_STISPWDR_001c: .ascii "\x01\0\0\0",
_RODATA_G_STISPWDR_0020: .ascii "\x10\x10\x10\x10\x10\x10\x10@\x80\xff\xff\xff\xff\xff\xff\xff\x18\x18\x18\x18\x18\x18\x18\x80\x80\xff\xff\xff\xff\xff\xff\xff\x01\0\0\0",
_RODATA_G_STISPWDR_0044: .ascii "\x0f\x0f\xb8\x0b\x14\0\0\0",
_RODATA_G_STISPWDR_004c: .ascii ""


_RODATA_G_STISPWDRANTIFALSECOLOR_0000: .ascii "\x01\0\0\0",
_RODATA_G_STISPWDRANTIFALSECOLOR_0004: .ascii "\x04\x04\x04\x04\x04\x04\x04\x04\x04\x04\x04\x04\x03\x02\x01\0",
_RODATA_G_STISPWDRANTIFALSECOLOR_0014: .ascii "\x04\x04\x04\x04\x04\x04\x04\x04\x04\x04\x04\x04\x03\x02\x01\0",
_RODATA_G_STISPWDRANTIFALSECOLOR_0024: .ascii ""


_RODATA_G_STISPWDRGE_0000: .ascii "\0\0\0\0",
_RODATA_G_STISPWDRGE_0004: .ascii "\t\t,\x01,\x01,\x01,\x01,\x016\x016\x016\x016\x01@\x01@\x01@\x01@\x01J\x01J\x01J\x01J\x01\x80\0\x80\0",
_RODATA_G_STISPWDRGE_002c: .ascii "\x80\0\x80\0",
_RODATA_G_STISPWDRGE_0030: .ascii "\x81\0\x81\0",
_RODATA_G_STISPWDRGE_0034: .ascii "\x81\0\x81\0",
_RODATA_G_STISPWDRGE_0038: .ascii "\x82\0\x82\0",
_RODATA_G_STISPWDRGE_003c: .ascii "\x82\0\x82\0",
_RODATA_G_STISPWDRGE_0040: .ascii "\x83\0\x83\0",
_RODATA_G_STISPWDRGE_0044: .ascii "\x83\0\x83\0",
_RODATA_G_STISPWDRGE_0048: .ascii "\0\x04\0\x04\0\x04\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08\0\x08"


_RODATA_G_STISPWDRLDCI_0000: .ascii "\0\0\0\0",
_RODATA_G_STISPWDRLDCI_0004: .ascii "$2-*(# \x0c\x08\x06\0\0",
_RODATA_G_STISPWDRLDCI_0010: .ascii "\0\0\0\0",
_RODATA_G_STISPWDRLDCI_0014: .ascii "\0PPHHH@\x18\x14\x0c\x08\x06\x02\x01\x01\x01\x01\0\0\0",
_RODATA_G_STISPWDRLDCI_0028: .ascii "\0\0\0\0",
_RODATA_G_STISPWDRLDCI_002c: .ascii "\0\0\0\0",
_RODATA_G_STISPWDRLDCI_0030: .ascii "\0\0\0\0",
_RODATA_G_STISPWDRLDCI_0034: .ascii "\02----\x18\x0c\x08\x06\0\0",
_RODATA_G_STISPWDRLDCI_0040: .ascii "\0\0\0\0",
_RODATA_G_STISPWDRLDCI_0044: .ascii "\0PPPPPH@6$\x08\x06\x02\x01\x01\x01\x01\x80\x80\x80\0\0\0\0",
_RODATA_G_STISPWDRLDCI_005c: .ascii "\0\0\0\0",
_RODATA_G_STISPWDRLDCI_0060: .ascii "\0\0\0\0",
_RODATA_G_STISPWDRLDCI_0064: .ascii "\0\0\x14\0",
_RODATA_G_STISPWDRLDCI_0068: .ascii "\x14\0\x14\0",
_RODATA_G_STISPWDRLDCI_006c: .ascii "\x14\0\x1e\0",
_RODATA_G_STISPWDRLDCI_0070: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPWDRLDCI_0074: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPWDRLDCI_0078: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPWDRLDCI_007c: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPWDRLDCI_0080: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPWDRLDCI_0084: .ascii "\x1e\0\0\0",
_RODATA_G_STISPWDRLDCI_0088: .ascii ""


_RODATA_G_STISPYUVSHARPEN_0000: .ascii "d\x87\x80\x96\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\xfa\0\xa4\x01\x86\x01\x86\x01\x86\x01\x86\x01\x86\x01r\x01^\x01J\x016\x01\"\x01\x0e\x01\x0e\x01\x0e\x01\x0e\x01\x0e\x01\x0e\x01\n\x01\x04\x01\xf4\0\xe6\0",
_RODATA_G_STISPYUVSHARPEN_0050: .ascii "\xe6\0\xe6\0",
_RODATA_G_STISPYUVSHARPEN_0054: .ascii "\xe6\0\xe6\0",
_RODATA_G_STISPYUVSHARPEN_0058: .ascii "\xe6\0\xd2\0",
_RODATA_G_STISPYUVSHARPEN_005c: .ascii "\xbe\0\xbe\0",
_RODATA_G_STISPYUVSHARPEN_0060: .ascii "\xaa\0\x96\0",
_RODATA_G_STISPYUVSHARPEN_0064: .ascii "x\0{\0",
_RODATA_G_STISPYUVSHARPEN_0068: .ascii "}\0\x80\0",
_RODATA_G_STISPYUVSHARPEN_006c: .ascii "\x82\0\x87\0",
_RODATA_G_STISPYUVSHARPEN_0070: .ascii "\x8c\0\x94\0",
_RODATA_G_STISPYUVSHARPEN_0074: .ascii "\xa0\0\xa8\0",
_RODATA_G_STISPYUVSHARPEN_0078: .ascii "\xb4\0\xbe\0",
_RODATA_G_STISPYUVSHARPEN_007c: .ascii "\xc8\0\xd2\0",
_RODATA_G_STISPYUVSHARPEN_0080: .ascii "\xd2\0\xd2\0",
_RODATA_G_STISPYUVSHARPEN_0084: .ascii "\xd2\0\xd2\0",
_RODATA_G_STISPYUVSHARPEN_0088: .ascii "\xc8\0\xbe\0",
_RODATA_G_STISPYUVSHARPEN_008c: .ascii "\xb9\0\xaf\0",
_RODATA_G_STISPYUVSHARPEN_0090: .ascii "\xa5\0\xa0\0",
_RODATA_G_STISPYUVSHARPEN_0094: .ascii "\x92\0\x88\0",
_RODATA_G_STISPYUVSHARPEN_0098: .ascii "\x82\0\x80\0",
_RODATA_G_STISPYUVSHARPEN_009c: .ascii "}\0{\0",
_RODATA_G_STISPYUVSHARPEN_00a0: .ascii "x\0x\0",
_RODATA_G_STISPYUVSHARPEN_00a4: .ascii "\xa0\0d\0",
_RODATA_G_STISPYUVSHARPEN_00a8: .ascii "7F\n\t\x80\xb4<\x1c \x1f\x17\0",
_RODATA_G_STISPYUVSHARPEN_00b4: .ascii "\xa0\0\x7f\x7f\x7fP<<<<<<<<<<<<\x7f\x7f\x7fP<<<<<<<<<<<<\x7f\x7f\x7fP<<<<<<<<<<<<\x7f\x7f\x7fP<<<<<<<<<<<<\x7f\x7f\x7fP<<<<<<<<<<<<\x7f\x7f\x7fPF<<<<<<<<<<<\x7f\x7f\x7fPPHHHHHHHHHHH\x7f\x7f\x7fZZUUUUUUUUUUU\x7f\x7f\x7fddddddddddddd\x7f\x7f\x7frrrrrrrrrrrrr\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fn\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7fZ\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\xfa\0",
_RODATA_G_STISPYUVSHARPEN_02b8: .ascii "@\x01@\x01\xdc\0d\0",
_RODATA_G_STISPYUVSHARPEN_02c0: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_02c4: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_02c8: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_02cc: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_02d0: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_02d4: .ascii "d\0\x86\x01m\x01^\x01\xf5\0\x96\0",
_RODATA_G_STISPYUVSHARPEN_02e0: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPEN_02e4: .ascii "\x96\0d\0",
_RODATA_G_STISPYUVSHARPEN_02e8: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_02ec: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_02f0: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_02f4: .ascii "d\0\x86\x01\x9a\x016\x01\x0e\x01\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0300: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0304: .ascii "\xc8\0d\0",
_RODATA_G_STISPYUVSHARPEN_0308: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_030c: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_0310: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_0314: .ascii "d\0\x81\x01\x90\x016\x01Y\x01\xfa\0",
_RODATA_G_STISPYUVSHARPEN_0320: .ascii "\xfa\0,\x01\xfa\0\x96\0",
_RODATA_G_STISPYUVSHARPEN_0328: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPEN_032c: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPEN_0330: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPEN_0334: .ascii "\x96\0|\x01\x86\x01,\x01\xa4\x01,\x01,\x01\x90\x01,\x01\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0348: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_034c: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0350: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0354: .ascii "\xc8\0|\x01\x81\x01,\x01\x95\x01^\x01E\x01\x90\x01,\x01\xfa\0",
_RODATA_G_STISPYUVSHARPEN_0368: .ascii "\xfa\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_036c: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0370: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0374: .ascii "\xc8\0\x86\x01|\x01,\x01\x86\x01\x90\x01^\x01\x90\x01,\x01,\x01,\x01\xc8\0",
_RODATA_G_STISPYUVSHARPEN_038c: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0390: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0394: .ascii "\xc8\0\x86\x01w\x01,\x01|\x01h\x01^\x01w\x01^\x01,\x01,\x01\xc8\0",
_RODATA_G_STISPYUVSHARPEN_03ac: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_03b0: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_03b4: .ascii "\xc8\0\x86\x01r\x01,\x01r\x01@\x01^\x01^\x01\x90\x01,\x01,\x01\xc8\0",
_RODATA_G_STISPYUVSHARPEN_03cc: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_03d0: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_03d4: .ascii "\xc8\0\x86\x01m\x01,\x01c\x01\"\x01E\x01E\x01\x90\x01,\x01,\x01\xc8\0",
_RODATA_G_STISPYUVSHARPEN_03ec: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_03f0: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_03f4: .ascii "\xc8\0\x86\x01h\x01,\x01T\x01\x04\x01,\x01,\x01,\x01,\x01,\x01\xc8\0",
_RODATA_G_STISPYUVSHARPEN_040c: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0410: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0414: .ascii "\xc8\0\x86\x01c\x01,\x01@\x01\xf5\0",
_RODATA_G_STISPYUVSHARPEN_0420: .ascii ",\x01,\x01,\x01,\x01,\x01\xfa\0",
_RODATA_G_STISPYUVSHARPEN_042c: .ascii "\xfa\0\xfa\0",
_RODATA_G_STISPYUVSHARPEN_0430: .ascii "\xfa\0\xfa\0",
_RODATA_G_STISPYUVSHARPEN_0434: .ascii "\xfa\0\x86\x01^\x01,\x01,\x01\xe6\0",
_RODATA_G_STISPYUVSHARPEN_0440: .ascii ",\x01,\x01,\x01,\x01,\x01,\x01,\x01,\x01,\x01,\x01,\x01|\x01Y\x01,\x01\x1d\x01\xe6\0",
_RODATA_G_STISPYUVSHARPEN_0460: .ascii ",\x01,\x01,\x01,\x01,\x01,\x01,\x01,\x01,\x01,\x01,\x01r\x01T\x01,\x01\x0e\x01\xe6\0",
_RODATA_G_STISPYUVSHARPEN_0480: .ascii ",\x01,\x01\x90\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01h\x01O\x01,\x01\x0e\x01\xe6\0",
_RODATA_G_STISPYUVSHARPEN_04a0: .ascii ",\x01,\x01\x90\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01Y\x01B\x01(\x01\x0e\x01\xe6\0",
_RODATA_G_STISPYUVSHARPEN_04c0: .ascii ",\x01,\x01\x90\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01E\x01)\x01\x19\x01\x0e\x01\xe6\0",
_RODATA_G_STISPYUVSHARPEN_04e0: .ascii ",\x01,\x01w\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x011\x01\x1d\x01\x0e\x01\x0e\x01\xe6\0",
_RODATA_G_STISPYUVSHARPEN_0500: .ascii ",\x01,\x01,\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01\x1d\x01\x0e\x01\x0e\x01\x0e\x01\xe6\0",
_RODATA_G_STISPYUVSHARPEN_0520: .ascii ",\x01,\x01,\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01\x0e\x01\x0e\x01\x0e\x01\x0e\x01\xe6\0",
_RODATA_G_STISPYUVSHARPEN_0540: .ascii "8\x018\x018\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01\x0e\x01\x0e\x01\x0e\x01\x0e\x01\xeb\0",
_RODATA_G_STISPYUVSHARPEN_0560: .ascii "j\x01j\x01j\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01\x0e\x01\x0e\x01\x0e\x01\x0e\x01\xff\0",
_RODATA_G_STISPYUVSHARPEN_0580: .ascii "\x9c\x01\x9c\x01\x9c\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01\x0e\x01\x0e\x01\x0e\x01\x0e\x01\x0e\x01\xce\x01\xce\x01\xce\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01\x0e\x01\x0e\x01\x0e\x01\x0e\x01\x0e\x01\x90\x01\xf4\x01\xf4\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01\t\x01\t\x01\t\x01\t\x01\x0e\x01,\x01\xf4\x01\xf4\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01\xfc\0",
_RODATA_G_STISPYUVSHARPEN_05f8: .ascii "\xfc\0\xfc\0",
_RODATA_G_STISPYUVSHARPEN_05fc: .ascii "\xfc\0\x0e\x01,\x01\xf4\x01\xf4\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01\xef\0",
_RODATA_G_STISPYUVSHARPEN_0618: .ascii "\xef\0\xef\0",
_RODATA_G_STISPYUVSHARPEN_061c: .ascii "\xef\0\x0e\x01,\x01\xf4\x01\xf4\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01\xe6\0",
_RODATA_G_STISPYUVSHARPEN_0638: .ascii "\xe6\0\xe6\0",
_RODATA_G_STISPYUVSHARPEN_063c: .ascii "\xe6\0\x0e\x01,\x01\xf4\x01\xf4\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01\xe6\0",
_RODATA_G_STISPYUVSHARPEN_0658: .ascii "\xe6\0\xe6\0",
_RODATA_G_STISPYUVSHARPEN_065c: .ascii "\xe6\0\xf2\0",
_RODATA_G_STISPYUVSHARPEN_0660: .ascii ",\x01\xf4\x01\xfe\x01\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01\xd2\0",
_RODATA_G_STISPYUVSHARPEN_0678: .ascii "\xe6\0\xe6\0",
_RODATA_G_STISPYUVSHARPEN_067c: .ascii "\xe6\0\xdf\0",
_RODATA_G_STISPYUVSHARPEN_0680: .ascii ",\x01\xf4\x01X\x02\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01\xa2\0",
_RODATA_G_STISPYUVSHARPEN_0698: .ascii "\xe6\0\xe6\0",
_RODATA_G_STISPYUVSHARPEN_069c: .ascii "\xe6\0\xdc\0",
_RODATA_G_STISPYUVSHARPEN_06a0: .ascii ",\x01\xf4\x01X\x02\x90\x01\x90\x01,\x01,\x01,\x01,\x01,\x01,\x01^\x01^\x01^\x01T\x01\xc8\0",
_RODATA_G_STISPYUVSHARPEN_06c0: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_06c4: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_06c8: .ascii ",\x01,\x01,\x01,\x01,\x01,\x01,\x01^\x01^\x01^\x01T\x01\xc8\0",
_RODATA_G_STISPYUVSHARPEN_06e0: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_06e4: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_06e8: .ascii ",\x01,\x01,\x01,\x01,\x01,\x01,\x01^\x01^\x01^\x01T\x01\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0700: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0704: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0708: .ascii ",\x01,\x01,\x01,\x01,\x01,\x01,\x01d\x01d\x01d\x01T\x01\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0720: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0724: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0728: .ascii ",\x01,\x01,\x01,\x01,\x01,\x01,\x01h\x01h\x01h\x01T\x01\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0740: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0744: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0748: .ascii ",\x01,\x01,\x01,\x01,\x01,\x01,\x01r\x01r\x01r\x01T\x01\xdc\0",
_RODATA_G_STISPYUVSHARPEN_0760: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0764: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_0768: .ascii ",\x01,\x01,\x01,\x01,\x01,\x01,\x01w\x01w\x01w\x01T\x01\xf0\0",
_RODATA_G_STISPYUVSHARPEN_0780: .ascii "\xdc\0\xdc\0",
_RODATA_G_STISPYUVSHARPEN_0784: .ascii "\xdc\0\xdc\0",
_RODATA_G_STISPYUVSHARPEN_0788: .ascii ",\x01,\x01,\x01,\x01,\x01,\x01,\x01\x86\x01\x86\x01\x86\x01T\x01\x04\x01\xf0\0\xdc\0",
_RODATA_G_STISPYUVSHARPEN_07a4: .ascii "\xdc\0\xdc\0",
_RODATA_G_STISPYUVSHARPEN_07a8: .ascii ",\x01,\x01,\x01,\x01,\x01,\x01,\x01\x9a\x01\x9a\x01\x9a\x01T\x01\x18\x01\x04\x01\xdc\0",
_RODATA_G_STISPYUVSHARPEN_07c4: .ascii "\xdc\0\xdc\0",
_RODATA_G_STISPYUVSHARPEN_07c8: .ascii ",\x01,\x01,\x01,\x01,\x01,\x01,\x01\xb8\x01\xb8\x01\xb8\x01T\x01,\x01\x18\x01\xdc\0",
_RODATA_G_STISPYUVSHARPEN_07e4: .ascii "\x0e\x01,\x01J\x01J\x01J\x01J\x01J\x01J\x01J\x01\xbe\x01\xbe\x01\xbe\x01T\x01@\x01,\x01\xdc\0",
_RODATA_G_STISPYUVSHARPEN_0804: .ascii ",\x01^\x01r\x01r\x01r\x01r\x01r\x01r\x01r\x01\xc2\x01\xc2\x01\xc2\x01T\x01T\x01@\x01\xdc\0",
_RODATA_G_STISPYUVSHARPEN_0824: .ascii "^\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\xc2\x01\xc2\x01\xc2\x01T\x01T\x01T\x01\xdc\0",
_RODATA_G_STISPYUVSHARPEN_0844: .ascii "\x90\x01\xc2\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\xc2\x01\xc2\x01\xc2\x01h\x01T\x01T\x01\xf0\0",
_RODATA_G_STISPYUVSHARPEN_0864: .ascii "\x90\x01\xf4\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\xc2\x01\xc2\x01\xc2\x01|\x01T\x01T\x01\x04\x01\x90\x01\xf4\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\xc2\x01\xc2\x01\xc2\x01\x90\x01h\x01T\x01\x18\x01\x90\x01\xf4\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\xc2\x01\xc2\x01\xc2\x01\xa4\x01|\x01h\x01,\x01\xc2\x01\xf4\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\xbe\x01\xbe\x01\xbe\x01\xa4\x01\x90\x01|\x01,\x01\xf4\x01\xf4\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\xb8\x01\xb8\x01\xb8\x01\xa4\x01\xa4\x01\x90\x01,\x01\xf4\x01\xf4\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\xae\x01\xae\x01\xae\x01\xa4\x01\xa4\x01\x90\x01,\x01\xf4\x01\xf4\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x9a\x01\x9a\x01\x9a\x01\xa4\x01\xa4\x01\x90\x01,\x01\xf4\x01\xf4\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01\x90\x01|\x01|\x01|\x01\xa4\x01\xa4\x01\x90\x01,\x01\xf4\x01\xf4\x01\xae\x01\xae\x01\xae\x01\xae\x01\xae\x01\xae\x01\xae\x01T\x01T\x01T\x01\x90\x01\xa4\x01\xf4\x01,\x01\xf4\x01\xf4\x01\xd6\x01\xd6\x01\xd6\x01\xd6\x01\xd6\x01\xd6\x01\xd6\x01\"\x01\"\x01\"\x01|\x01\x90\x01\xf4\x01,\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xfa\0",
_RODATA_G_STISPYUVSHARPEN_09b8: .ascii "\xfa\0\xfa\0",
_RODATA_G_STISPYUVSHARPEN_09bc: .ascii "h\x01|\x01\xf4\x01,\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xdc\0",
_RODATA_G_STISPYUVSHARPEN_09d8: .ascii "\xdc\0\xdc\0",
_RODATA_G_STISPYUVSHARPEN_09dc: .ascii "T\x01h\x01\xf4\x01,\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xc8\0",
_RODATA_G_STISPYUVSHARPEN_09f8: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPEN_09fc: .ascii "@\x01h\x01\xf4\x01,\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xbe\0",
_RODATA_G_STISPYUVSHARPEN_0a18: .ascii "\xbe\0\xbe\0",
_RODATA_G_STISPYUVSHARPEN_0a1c: .ascii ",\x01T\x01\xf4\x01,\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xba\0",
_RODATA_G_STISPYUVSHARPEN_0a38: .ascii "\xba\0\xba\0",
_RODATA_G_STISPYUVSHARPEN_0a3c: .ascii "\"\x016\x01\xf4\x01,\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xb4\0",
_RODATA_G_STISPYUVSHARPEN_0a58: .ascii "\xb4\0\xb4\0",
_RODATA_G_STISPYUVSHARPEN_0a5c: .ascii "\"\x01\x0e\x01\xf4\x01,\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xaa\0",
_RODATA_G_STISPYUVSHARPEN_0a78: .ascii "\xaa\0\xaa\0",
_RODATA_G_STISPYUVSHARPEN_0a7c: .ascii "\x0e\x01\x04\x01\xf4\x01,\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\x96\0",
_RODATA_G_STISPYUVSHARPEN_0a98: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPEN_0a9c: .ascii "\x0e\x01\xfa\0",
_RODATA_G_STISPYUVSHARPEN_0aa0: .ascii "\xf4\x01,\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xf4\x01\xa0\0",
_RODATA_G_STISPYUVSHARPEN_0ab8: .ascii "\xaa\0\xb4\0",
_RODATA_G_STISPYUVSHARPEN_0abc: .ascii "\xb4\0\xa0\0",
_RODATA_G_STISPYUVSHARPEN_0ac0: .ascii "\xa0\0\x80\0",
_RODATA_G_STISPYUVSHARPEN_0ac4: .ascii "\x80\0\x80\0",
_RODATA_G_STISPYUVSHARPEN_0ac8: .ascii "\x80\0\x80\0",
_RODATA_G_STISPYUVSHARPEN_0acc: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_0ad0: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_0ad4: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_0ad8: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_0adc: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_0ae0: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_0ae4: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPEN_0ae8: .ascii "d\0`\0",
_RODATA_G_STISPYUVSHARPEN_0aec: .ascii "`\0`\0",
_RODATA_G_STISPYUVSHARPEN_0af0: .ascii "`\0`\0",
_RODATA_G_STISPYUVSHARPEN_0af4: .ascii "`\07AFPK7(\x1e\x1e\x1e\x1e\n\n\n\n\nFPPZPF22--(\x14\x0f\x0f\x0f\x0f\x08\x08\x07\x07\x07\x06\x05\x05\0\0",
_RODATA_G_STISPYUVSHARPEN_0b20: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPEN_0b24: .ascii "\0\0\t\t\x08\x08\x07\x07\x06\x06\0\0",
_RODATA_G_STISPYUVSHARPEN_0b30: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPEN_0b34: .ascii "\0\0\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80xxxxxx\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0::<>>>>>>>>>>>>>\x1c\x1c\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f                \x1c\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x17\x1b\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\xa0\0",
_RODATA_G_STISPYUVSHARPEN_0ba8: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPEN_0bac: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPEN_0bb0: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPEN_0bb4: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPEN_0bb8: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPEN_0bbc: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPEN_0bc0: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPEN_0bc4: .ascii "\xa0\0"


_RODATA_G_STISPYUVSHARPENWDR_0000: .ascii "d\x87\x80\x96\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0028: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_002c: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0030: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0034: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0038: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_003c: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0040: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0044: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0048: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_004c: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0050: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0054: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0058: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_005c: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0060: .ascii "\x8c\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0064: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0068: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_006c: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0070: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0074: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0078: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_007c: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0080: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0084: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0088: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_008c: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0090: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0094: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0098: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_009c: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_00a0: .ascii "\x96\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_00a4: .ascii "\0\x01d\0",
_RODATA_G_STISPYUVSHARPENWDR_00a8: .ascii "AF\n\t\x80\xb4>\x1f \x1f\x1f\0",
_RODATA_G_STISPYUVSHARPENWDR_00b4: .ascii "\xa0\0\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_02b8: .ascii "\x8c\0<\0",
_RODATA_G_STISPYUVSHARPENWDR_02bc: .ascii "\x1e\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_02c0: .ascii "\x0f\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_02c4: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_02c8: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_02cc: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_02d0: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_02d4: .ascii "\0\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_02d8: .ascii "\x8c\0=\0",
_RODATA_G_STISPYUVSHARPENWDR_02dc: .ascii "\x1f\0\x15\0",
_RODATA_G_STISPYUVSHARPENWDR_02e0: .ascii "\x10\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_02e4: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_02e8: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_02ec: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_02f0: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_02f4: .ascii "\0\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_02f8: .ascii "\x8c\0>\0",
_RODATA_G_STISPYUVSHARPENWDR_02fc: .ascii " \0\x16\0",
_RODATA_G_STISPYUVSHARPENWDR_0300: .ascii "\x11\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0304: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0308: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_030c: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0310: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0314: .ascii "\0\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0318: .ascii "\x8c\0?\0",
_RODATA_G_STISPYUVSHARPENWDR_031c: .ascii "!\0\x18\0",
_RODATA_G_STISPYUVSHARPENWDR_0320: .ascii "\x12\0\x02\0",
_RODATA_G_STISPYUVSHARPENWDR_0324: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0328: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_032c: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0330: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0334: .ascii "\0\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_0338: .ascii "\x8c\0A\0",
_RODATA_G_STISPYUVSHARPENWDR_033c: .ascii "#\0\x1a\0",
_RODATA_G_STISPYUVSHARPENWDR_0340: .ascii "\x14\0\x04\0",
_RODATA_G_STISPYUVSHARPENWDR_0344: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0348: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_034c: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0350: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0354: .ascii "\0\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0358: .ascii "\x96\0B\0",
_RODATA_G_STISPYUVSHARPENWDR_035c: .ascii "$\0\x1c\0",
_RODATA_G_STISPYUVSHARPENWDR_0360: .ascii "\x16\0\x05\0",
_RODATA_G_STISPYUVSHARPENWDR_0364: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0368: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_036c: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0370: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0374: .ascii "\0\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0378: .ascii "\xa0\0C\0",
_RODATA_G_STISPYUVSHARPENWDR_037c: .ascii "%\0\x1e\0",
_RODATA_G_STISPYUVSHARPENWDR_0380: .ascii "\x18\0\x06\0",
_RODATA_G_STISPYUVSHARPENWDR_0384: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0388: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_038c: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0390: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0394: .ascii "\0\0\xa7\0",
_RODATA_G_STISPYUVSHARPENWDR_0398: .ascii "\xa7\0D\0",
_RODATA_G_STISPYUVSHARPENWDR_039c: .ascii "&\0\x1f\0",
_RODATA_G_STISPYUVSHARPENWDR_03a0: .ascii "\x19\0\x07\0",
_RODATA_G_STISPYUVSHARPENWDR_03a4: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_03a8: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_03ac: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_03b0: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_03b4: .ascii "\0\0\xaf\0",
_RODATA_G_STISPYUVSHARPENWDR_03b8: .ascii "\xaf\0F\0",
_RODATA_G_STISPYUVSHARPENWDR_03bc: .ascii "(\0!\0",
_RODATA_G_STISPYUVSHARPENWDR_03c0: .ascii "\x1b\0\x08\0",
_RODATA_G_STISPYUVSHARPENWDR_03c4: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_03c8: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_03cc: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_03d0: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_03d4: .ascii "\0\0\xb6\0",
_RODATA_G_STISPYUVSHARPENWDR_03d8: .ascii "\xb6\0H\0",
_RODATA_G_STISPYUVSHARPENWDR_03dc: .ascii "*\0\"\0",
_RODATA_G_STISPYUVSHARPENWDR_03e0: .ascii "\x1d\0\n\0",
_RODATA_G_STISPYUVSHARPENWDR_03e4: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_03e8: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_03ec: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_03f0: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_03f4: .ascii "\0\0\xbe\0",
_RODATA_G_STISPYUVSHARPENWDR_03f8: .ascii "\xbe\0J\0",
_RODATA_G_STISPYUVSHARPENWDR_03fc: .ascii ",\0$\0",
_RODATA_G_STISPYUVSHARPENWDR_0400: .ascii " \0\x0c\0",
_RODATA_G_STISPYUVSHARPENWDR_0404: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0408: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_040c: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0410: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0414: .ascii "\0\0\xc5\0",
_RODATA_G_STISPYUVSHARPENWDR_0418: .ascii "\xc5\0M\0",
_RODATA_G_STISPYUVSHARPENWDR_041c: .ascii "/\0'\0",
_RODATA_G_STISPYUVSHARPENWDR_0420: .ascii "\"\0\x0f\0",
_RODATA_G_STISPYUVSHARPENWDR_0424: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0428: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_042c: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0430: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0434: .ascii "\0\0\xcd\0",
_RODATA_G_STISPYUVSHARPENWDR_0438: .ascii "\xcd\0P\0",
_RODATA_G_STISPYUVSHARPENWDR_043c: .ascii "2\0*\0",
_RODATA_G_STISPYUVSHARPENWDR_0440: .ascii "$\0\x12\0",
_RODATA_G_STISPYUVSHARPENWDR_0444: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0448: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_044c: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0450: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0454: .ascii "\0\0\xd4\0",
_RODATA_G_STISPYUVSHARPENWDR_0458: .ascii "\xd4\0S\0",
_RODATA_G_STISPYUVSHARPENWDR_045c: .ascii "5\0-\0",
_RODATA_G_STISPYUVSHARPENWDR_0460: .ascii "%\0\x13\0",
_RODATA_G_STISPYUVSHARPENWDR_0464: .ascii "\x01\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0468: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_046c: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0470: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0474: .ascii "\0\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0478: .ascii "\xdc\0V\0",
_RODATA_G_STISPYUVSHARPENWDR_047c: .ascii "8\00\0",
_RODATA_G_STISPYUVSHARPENWDR_0480: .ascii "&\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_0484: .ascii "\x02\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0488: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_048c: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0490: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0494: .ascii "\0\0\xe1\0",
_RODATA_G_STISPYUVSHARPENWDR_0498: .ascii "\xe1\0X\0",
_RODATA_G_STISPYUVSHARPENWDR_049c: .ascii ":\02\0",
_RODATA_G_STISPYUVSHARPENWDR_04a0: .ascii "(\0\x15\0",
_RODATA_G_STISPYUVSHARPENWDR_04a4: .ascii "\x04\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_04a8: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_04ac: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_04b0: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_04b4: .ascii "\0\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_04b8: .ascii "\xe6\0\\0",
_RODATA_G_STISPYUVSHARPENWDR_04bc: .ascii ">\06\0",
_RODATA_G_STISPYUVSHARPENWDR_04c0: .ascii ",\0\x16\0",
_RODATA_G_STISPYUVSHARPENWDR_04c4: .ascii "\x07\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_04c8: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_04cc: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_04d0: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_04d4: .ascii "\0\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_04d8: .ascii "\xe6\0i\0",
_RODATA_G_STISPYUVSHARPENWDR_04dc: .ascii "G\0>\0",
_RODATA_G_STISPYUVSHARPENWDR_04e0: .ascii "3\0\x1a\0",
_RODATA_G_STISPYUVSHARPENWDR_04e4: .ascii "\n\0\x03\0",
_RODATA_G_STISPYUVSHARPENWDR_04e8: .ascii "\x03\0\x03\0",
_RODATA_G_STISPYUVSHARPENWDR_04ec: .ascii "\x03\0\x03\0",
_RODATA_G_STISPYUVSHARPENWDR_04f0: .ascii "\x03\0\x03\0",
_RODATA_G_STISPYUVSHARPENWDR_04f4: .ascii "\x03\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_04f8: .ascii "\xe6\0{\0",
_RODATA_G_STISPYUVSHARPENWDR_04fc: .ascii "M\0F\0",
_RODATA_G_STISPYUVSHARPENWDR_0500: .ascii "9\0\x1f\0",
_RODATA_G_STISPYUVSHARPENWDR_0504: .ascii "\r\0\x07\0",
_RODATA_G_STISPYUVSHARPENWDR_0508: .ascii "\x07\0\x07\0",
_RODATA_G_STISPYUVSHARPENWDR_050c: .ascii "\x07\0\x07\0",
_RODATA_G_STISPYUVSHARPENWDR_0510: .ascii "\x07\0\x07\0",
_RODATA_G_STISPYUVSHARPENWDR_0514: .ascii "\x07\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_0518: .ascii "\xe6\0\x8a\0",
_RODATA_G_STISPYUVSHARPENWDR_051c: .ascii "Q\0I\0",
_RODATA_G_STISPYUVSHARPENWDR_0520: .ascii ">\0#\0",
_RODATA_G_STISPYUVSHARPENWDR_0524: .ascii "\x11\0\x0b\0",
_RODATA_G_STISPYUVSHARPENWDR_0528: .ascii "\x0b\0\x0b\0",
_RODATA_G_STISPYUVSHARPENWDR_052c: .ascii "\x0b\0\x0b\0",
_RODATA_G_STISPYUVSHARPENWDR_0530: .ascii "\x0b\0\x0b\0",
_RODATA_G_STISPYUVSHARPENWDR_0534: .ascii "\x0b\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_0538: .ascii "\xe6\0\x98\0",
_RODATA_G_STISPYUVSHARPENWDR_053c: .ascii "T\0O\0",
_RODATA_G_STISPYUVSHARPENWDR_0540: .ascii "H\0&\0",
_RODATA_G_STISPYUVSHARPENWDR_0544: .ascii "\x16\0\x0e\0",
_RODATA_G_STISPYUVSHARPENWDR_0548: .ascii "\x0e\0\x0e\0",
_RODATA_G_STISPYUVSHARPENWDR_054c: .ascii "\x0e\0\x0e\0",
_RODATA_G_STISPYUVSHARPENWDR_0550: .ascii "\x0e\0\x0e\0",
_RODATA_G_STISPYUVSHARPENWDR_0554: .ascii "\x0e\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_0558: .ascii "\xe6\0\xa1\0",
_RODATA_G_STISPYUVSHARPENWDR_055c: .ascii "W\0S\0",
_RODATA_G_STISPYUVSHARPENWDR_0560: .ascii "P\0)\0",
_RODATA_G_STISPYUVSHARPENWDR_0564: .ascii "\x1a\0\x11\0",
_RODATA_G_STISPYUVSHARPENWDR_0568: .ascii "\x11\0\x11\0",
_RODATA_G_STISPYUVSHARPENWDR_056c: .ascii "\x11\0\x11\0",
_RODATA_G_STISPYUVSHARPENWDR_0570: .ascii "\x11\0\x11\0",
_RODATA_G_STISPYUVSHARPENWDR_0574: .ascii "\x11\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_0578: .ascii "\xe6\0\xa6\0",
_RODATA_G_STISPYUVSHARPENWDR_057c: .ascii "Z\0X\0",
_RODATA_G_STISPYUVSHARPENWDR_0580: .ascii "W\0-\0",
_RODATA_G_STISPYUVSHARPENWDR_0584: .ascii "\x1e\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_0588: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_058c: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_0590: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_0594: .ascii "\x14\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_0598: .ascii "\xe6\0\xaa\0",
_RODATA_G_STISPYUVSHARPENWDR_059c: .ascii "Z\0Z\0",
_RODATA_G_STISPYUVSHARPENWDR_05a0: .ascii "Z\02\0",
_RODATA_G_STISPYUVSHARPENWDR_05a4: .ascii "\x1e\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_05a8: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_05ac: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_05b0: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_05b4: .ascii "\x14\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_05b8: .ascii "\xe6\0\xaa\0",
_RODATA_G_STISPYUVSHARPENWDR_05bc: .ascii "Z\0Z\0",
_RODATA_G_STISPYUVSHARPENWDR_05c0: .ascii "Z\02\0",
_RODATA_G_STISPYUVSHARPENWDR_05c4: .ascii "\x1e\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_05c8: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_05cc: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_05d0: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_05d4: .ascii "\x14\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_05d8: .ascii "\xe6\0\xa7\0",
_RODATA_G_STISPYUVSHARPENWDR_05dc: .ascii "Y\0X\0",
_RODATA_G_STISPYUVSHARPENWDR_05e0: .ascii "W\01\0",
_RODATA_G_STISPYUVSHARPENWDR_05e4: .ascii "\x1d\0\x13\0",
_RODATA_G_STISPYUVSHARPENWDR_05e8: .ascii "\x13\0\x13\0",
_RODATA_G_STISPYUVSHARPENWDR_05ec: .ascii "\x13\0\x13\0",
_RODATA_G_STISPYUVSHARPENWDR_05f0: .ascii "\x13\0\x13\0",
_RODATA_G_STISPYUVSHARPENWDR_05f4: .ascii "\x13\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_05f8: .ascii "\xe6\0\xa3\0",
_RODATA_G_STISPYUVSHARPENWDR_05fc: .ascii "U\0S\0",
_RODATA_G_STISPYUVSHARPENWDR_0600: .ascii "O\0-\0",
_RODATA_G_STISPYUVSHARPENWDR_0604: .ascii "\x19\0\x0f\0",
_RODATA_G_STISPYUVSHARPENWDR_0608: .ascii "\x0f\0\x0f\0",
_RODATA_G_STISPYUVSHARPENWDR_060c: .ascii "\x0f\0\x0f\0",
_RODATA_G_STISPYUVSHARPENWDR_0610: .ascii "\x0f\0\x0f\0",
_RODATA_G_STISPYUVSHARPENWDR_0614: .ascii "\x0f\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_0618: .ascii "\xe6\0\x9f\0",
_RODATA_G_STISPYUVSHARPENWDR_061c: .ascii "M\0K\0",
_RODATA_G_STISPYUVSHARPENWDR_0620: .ascii "G\0&\0",
_RODATA_G_STISPYUVSHARPENWDR_0624: .ascii "\x12\0\n\0",
_RODATA_G_STISPYUVSHARPENWDR_0628: .ascii "\n\0\n\0",
_RODATA_G_STISPYUVSHARPENWDR_062c: .ascii "\n\0\n\0",
_RODATA_G_STISPYUVSHARPENWDR_0630: .ascii "\n\0\n\0",
_RODATA_G_STISPYUVSHARPENWDR_0634: .ascii "\n\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_0638: .ascii "\xe6\0\x91\0",
_RODATA_G_STISPYUVSHARPENWDR_063c: .ascii "G\0@\0",
_RODATA_G_STISPYUVSHARPENWDR_0640: .ascii "9\0\x1e\0",
_RODATA_G_STISPYUVSHARPENWDR_0644: .ascii "\x0c\0\x06\0",
_RODATA_G_STISPYUVSHARPENWDR_0648: .ascii "\x06\0\x06\0",
_RODATA_G_STISPYUVSHARPENWDR_064c: .ascii "\x06\0\x06\0",
_RODATA_G_STISPYUVSHARPENWDR_0650: .ascii "\x06\0\x06\0",
_RODATA_G_STISPYUVSHARPENWDR_0654: .ascii "\x06\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_0658: .ascii "\xe6\0a\0",
_RODATA_G_STISPYUVSHARPENWDR_065c: .ascii "@\04\0",
_RODATA_G_STISPYUVSHARPENWDR_0660: .ascii "'\0\x16\0",
_RODATA_G_STISPYUVSHARPENWDR_0664: .ascii "\x06\0\x02\0",
_RODATA_G_STISPYUVSHARPENWDR_0668: .ascii "\x02\0\x02\0",
_RODATA_G_STISPYUVSHARPENWDR_066c: .ascii "\x02\0\x02\0",
_RODATA_G_STISPYUVSHARPENWDR_0670: .ascii "\x02\0\x02\0",
_RODATA_G_STISPYUVSHARPENWDR_0674: .ascii "\x02\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_0678: .ascii "\xe6\02\0",
_RODATA_G_STISPYUVSHARPENWDR_067c: .ascii "2\0&\0",
_RODATA_G_STISPYUVSHARPENWDR_0680: .ascii "\x1e\0\x0f\0",
_RODATA_G_STISPYUVSHARPENWDR_0684: .ascii "\x05\0\x02\0",
_RODATA_G_STISPYUVSHARPENWDR_0688: .ascii "\x02\0\x02\0",
_RODATA_G_STISPYUVSHARPENWDR_068c: .ascii "\x02\0\x02\0",
_RODATA_G_STISPYUVSHARPENWDR_0690: .ascii "\x02\0\x02\0",
_RODATA_G_STISPYUVSHARPENWDR_0694: .ascii "\x02\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_0698: .ascii "\xe6\0!\0",
_RODATA_G_STISPYUVSHARPENWDR_069c: .ascii "!\0\x1c\0",
_RODATA_G_STISPYUVSHARPENWDR_06a0: .ascii "\x15\0\n\0",
_RODATA_G_STISPYUVSHARPENWDR_06a4: .ascii "\x05\0\x02\0",
_RODATA_G_STISPYUVSHARPENWDR_06a8: .ascii "\x02\0\x02\0",
_RODATA_G_STISPYUVSHARPENWDR_06ac: .ascii "\x02\0\x02\0",
_RODATA_G_STISPYUVSHARPENWDR_06b0: .ascii "\x02\0\x02\0",
_RODATA_G_STISPYUVSHARPENWDR_06b4: .ascii "\x02\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_06b8: .ascii "\xdc\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_06bc: .ascii "\x96\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_06c0: .ascii "\xb4\0\xb4\0",
_RODATA_G_STISPYUVSHARPENWDR_06c4: .ascii "\xb4\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_06c8: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_06cc: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_06d0: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_06d4: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_06d8: .ascii "\xdc\0\x94\0",
_RODATA_G_STISPYUVSHARPENWDR_06dc: .ascii "\x96\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_06e0: .ascii "\xb4\0\xb4\0",
_RODATA_G_STISPYUVSHARPENWDR_06e4: .ascii "\xb4\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_06e8: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_06ec: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_06f0: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_06f4: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_06f8: .ascii "\xdc\0\x91\0",
_RODATA_G_STISPYUVSHARPENWDR_06fc: .ascii "\x96\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0700: .ascii "\xb4\0\xb4\0",
_RODATA_G_STISPYUVSHARPENWDR_0704: .ascii "\xb4\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_0708: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_070c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0710: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0714: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0718: .ascii "\xdc\0\x8e\0",
_RODATA_G_STISPYUVSHARPENWDR_071c: .ascii "\x96\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0720: .ascii "\xb4\0\xb4\0",
_RODATA_G_STISPYUVSHARPENWDR_0724: .ascii "\xb4\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_0728: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_072c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0730: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0734: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0738: .ascii "\xdc\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_073c: .ascii "\x96\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0740: .ascii "\xb4\0\xb4\0",
_RODATA_G_STISPYUVSHARPENWDR_0744: .ascii "\xb4\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_0748: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_074c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0750: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0754: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0758: .ascii "\xdc\0\x88\0",
_RODATA_G_STISPYUVSHARPENWDR_075c: .ascii "\x96\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0760: .ascii "\xb4\0\xb4\0",
_RODATA_G_STISPYUVSHARPENWDR_0764: .ascii "\xb4\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_0768: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_076c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0770: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0774: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0778: .ascii "\xdc\0\x82\0",
_RODATA_G_STISPYUVSHARPENWDR_077c: .ascii "\x94\0\xa3\0",
_RODATA_G_STISPYUVSHARPENWDR_0780: .ascii "\xb4\0\xb4\0",
_RODATA_G_STISPYUVSHARPENWDR_0784: .ascii "\xb4\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_0788: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_078c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0790: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0794: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0798: .ascii "\xdc\0|\0",
_RODATA_G_STISPYUVSHARPENWDR_079c: .ascii "\x90\0\xa8\0",
_RODATA_G_STISPYUVSHARPENWDR_07a0: .ascii "\xb4\0\xb4\0",
_RODATA_G_STISPYUVSHARPENWDR_07a4: .ascii "\xb4\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_07a8: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_07ac: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_07b0: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_07b4: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_07b8: .ascii "\xdc\0v\0",
_RODATA_G_STISPYUVSHARPENWDR_07bc: .ascii "\x8e\0\xaa\0",
_RODATA_G_STISPYUVSHARPENWDR_07c0: .ascii "\xba\0\xba\0",
_RODATA_G_STISPYUVSHARPENWDR_07c4: .ascii "\xba\0j\0",
_RODATA_G_STISPYUVSHARPENWDR_07c8: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_07cc: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_07d0: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_07d4: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_07d8: .ascii "\xdc\0n\0",
_RODATA_G_STISPYUVSHARPENWDR_07dc: .ascii "\x8c\0\xac\0",
_RODATA_G_STISPYUVSHARPENWDR_07e0: .ascii "\xbe\0\xbe\0",
_RODATA_G_STISPYUVSHARPENWDR_07e4: .ascii "\xbe\0n\0",
_RODATA_G_STISPYUVSHARPENWDR_07e8: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_07ec: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_07f0: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_07f4: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_07f8: .ascii "\xdc\0j\0",
_RODATA_G_STISPYUVSHARPENWDR_07fc: .ascii "\x88\0\xb0\0",
_RODATA_G_STISPYUVSHARPENWDR_0800: .ascii "\xc2\0\xc2\0",
_RODATA_G_STISPYUVSHARPENWDR_0804: .ascii "\xc2\0|\0",
_RODATA_G_STISPYUVSHARPENWDR_0808: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_080c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0810: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0814: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0818: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_081c: .ascii "\x96\0\xb4\0",
_RODATA_G_STISPYUVSHARPENWDR_0820: .ascii "\xc8\0\xc8\0",
_RODATA_G_STISPYUVSHARPENWDR_0824: .ascii "\xc8\0\x82\0",
_RODATA_G_STISPYUVSHARPENWDR_0828: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_082c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0830: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0834: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0838: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_083c: .ascii "\xa0\0\xb8\0",
_RODATA_G_STISPYUVSHARPENWDR_0840: .ascii "\xca\0\xca\0",
_RODATA_G_STISPYUVSHARPENWDR_0844: .ascii "\xca\0\x84\0",
_RODATA_G_STISPYUVSHARPENWDR_0848: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_084c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0850: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0854: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0858: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_085c: .ascii "\xaa\0\xbe\0",
_RODATA_G_STISPYUVSHARPENWDR_0860: .ascii "\xce\0\xce\0",
_RODATA_G_STISPYUVSHARPENWDR_0864: .ascii "\xce\0\x88\0",
_RODATA_G_STISPYUVSHARPENWDR_0868: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_086c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0870: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0874: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0878: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_087c: .ascii "\xbe\0\xc6\0",
_RODATA_G_STISPYUVSHARPENWDR_0880: .ascii "\xd4\0\xd4\0",
_RODATA_G_STISPYUVSHARPENWDR_0884: .ascii "\xd4\0\x8e\0",
_RODATA_G_STISPYUVSHARPENWDR_0888: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_088c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0890: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0894: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0898: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_089c: .ascii "\xc8\0\xd4\0",
_RODATA_G_STISPYUVSHARPENWDR_08a0: .ascii "\xdc\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_08a4: .ascii "\xdc\0\x8c\0",
_RODATA_G_STISPYUVSHARPENWDR_08a8: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_08ac: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_08b0: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_08b4: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_08b8: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_08bc: .ascii "\xc8\0\xd7\0",
_RODATA_G_STISPYUVSHARPENWDR_08c0: .ascii "\xe2\0\xe2\0",
_RODATA_G_STISPYUVSHARPENWDR_08c4: .ascii "\xe2\0\x92\0",
_RODATA_G_STISPYUVSHARPENWDR_08c8: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_08cc: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_08d0: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_08d4: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_08d8: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_08dc: .ascii "\xc8\0\xda\0",
_RODATA_G_STISPYUVSHARPENWDR_08e0: .ascii "\xe6\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_08e4: .ascii "\xe6\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_08e8: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_08ec: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_08f0: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_08f4: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_08f8: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_08fc: .ascii "\xc4\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0900: .ascii "\xe6\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_0904: .ascii "\xe6\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0908: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_090c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0910: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0914: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0918: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_091c: .ascii "\xb4\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0920: .ascii "\xe6\0\xe6\0",
_RODATA_G_STISPYUVSHARPENWDR_0924: .ascii "\xe6\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0928: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_092c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0930: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0934: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0938: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_093c: .ascii "\xaa\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0940: .ascii "\xe4\0\xe4\0",
_RODATA_G_STISPYUVSHARPENWDR_0944: .ascii "\xe4\0\x8a\0",
_RODATA_G_STISPYUVSHARPENWDR_0948: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_094c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0950: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0954: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0958: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_095c: .ascii "\xa5\0\xd2\0",
_RODATA_G_STISPYUVSHARPENWDR_0960: .ascii "\xdc\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0964: .ascii "\xdc\0\x82\0",
_RODATA_G_STISPYUVSHARPENWDR_0968: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_096c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0970: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0974: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0978: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_097c: .ascii "\xa0\0\xca\0",
_RODATA_G_STISPYUVSHARPENWDR_0980: .ascii "\xd2\0\xd2\0",
_RODATA_G_STISPYUVSHARPENWDR_0984: .ascii "\xd2\0x\0",
_RODATA_G_STISPYUVSHARPENWDR_0988: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_098c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0990: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0994: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0998: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_099c: .ascii "\x96\0\xb4\0",
_RODATA_G_STISPYUVSHARPENWDR_09a0: .ascii "\xbe\0\xbe\0",
_RODATA_G_STISPYUVSHARPENWDR_09a4: .ascii "\xbe\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_09a8: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_09ac: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_09b0: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_09b4: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_09b8: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_09bc: .ascii "\x82\0\x91\0",
_RODATA_G_STISPYUVSHARPENWDR_09c0: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_09c4: .ascii "\xa0\0Z\0",
_RODATA_G_STISPYUVSHARPENWDR_09c8: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_09cc: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_09d0: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_09d4: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_09d8: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_09dc: .ascii "i\0\x82\0",
_RODATA_G_STISPYUVSHARPENWDR_09e0: .ascii "\x91\0\x91\0",
_RODATA_G_STISPYUVSHARPENWDR_09e4: .ascii "\x91\0K\0",
_RODATA_G_STISPYUVSHARPENWDR_09e8: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_09ec: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_09f0: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_09f4: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_09f8: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_09fc: .ascii "d\0x\0",
_RODATA_G_STISPYUVSHARPENWDR_0a00: .ascii "\x82\0\x82\0",
_RODATA_G_STISPYUVSHARPENWDR_0a04: .ascii "\x82\0<\0",
_RODATA_G_STISPYUVSHARPENWDR_0a08: .ascii "<\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0a0c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0a10: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0a14: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0a18: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_0a1c: .ascii "Z\0`\0",
_RODATA_G_STISPYUVSHARPENWDR_0a20: .ascii "d\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_0a24: .ascii "d\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0a28: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0a2c: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0a30: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0a34: .ascii "2\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0a38: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_0a3c: .ascii "F\0L\0",
_RODATA_G_STISPYUVSHARPENWDR_0a40: .ascii "P\0P\0",
_RODATA_G_STISPYUVSHARPENWDR_0a44: .ascii "P\0(\0",
_RODATA_G_STISPYUVSHARPENWDR_0a48: .ascii "(\0(\0",
_RODATA_G_STISPYUVSHARPENWDR_0a4c: .ascii "(\0(\0",
_RODATA_G_STISPYUVSHARPENWDR_0a50: .ascii "(\0(\0",
_RODATA_G_STISPYUVSHARPENWDR_0a54: .ascii "(\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0a58: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_0a5c: .ascii "8\05\0",
_RODATA_G_STISPYUVSHARPENWDR_0a60: .ascii "2\02\0",
_RODATA_G_STISPYUVSHARPENWDR_0a64: .ascii "2\0\x1e\0",
_RODATA_G_STISPYUVSHARPENWDR_0a68: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPYUVSHARPENWDR_0a6c: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPYUVSHARPENWDR_0a70: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPYUVSHARPENWDR_0a74: .ascii "\x1e\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0a78: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_0a7c: .ascii "$\0 \0",
_RODATA_G_STISPYUVSHARPENWDR_0a80: .ascii "\x1e\0\x1e\0",
_RODATA_G_STISPYUVSHARPENWDR_0a84: .ascii "\x1e\0\x19\0",
_RODATA_G_STISPYUVSHARPENWDR_0a88: .ascii "\x19\0\x19\0",
_RODATA_G_STISPYUVSHARPENWDR_0a8c: .ascii "\x19\0\x19\0",
_RODATA_G_STISPYUVSHARPENWDR_0a90: .ascii "\x19\0\x19\0",
_RODATA_G_STISPYUVSHARPENWDR_0a94: .ascii "\x19\0\xdc\0",
_RODATA_G_STISPYUVSHARPENWDR_0a98: .ascii "\xdc\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_0a9c: .ascii "\x1e\0\x1a\0",
_RODATA_G_STISPYUVSHARPENWDR_0aa0: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_0aa4: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_0aa8: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_0aac: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_0ab0: .ascii "\x14\0\x14\0",
_RODATA_G_STISPYUVSHARPENWDR_0ab4: .ascii "\x14\0\0\x01\0\x01\0\x01\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0ac0: .ascii "\xa0\0\xc8\0",
_RODATA_G_STISPYUVSHARPENWDR_0ac4: .ascii "\xa0\0\x96\0",
_RODATA_G_STISPYUVSHARPENWDR_0ac8: .ascii "\x82\0\x80\0",
_RODATA_G_STISPYUVSHARPENWDR_0acc: .ascii "\x80\0\x80\0",
_RODATA_G_STISPYUVSHARPENWDR_0ad0: .ascii "\x80\0\x80\0",
_RODATA_G_STISPYUVSHARPENWDR_0ad4: .ascii "\x80\0d\0",
_RODATA_G_STISPYUVSHARPENWDR_0ad8: .ascii "d\0\x94\0",
_RODATA_G_STISPYUVSHARPENWDR_0adc: .ascii "`\0`\0",
_RODATA_G_STISPYUVSHARPENWDR_0ae0: .ascii "`\0`\0",
_RODATA_G_STISPYUVSHARPENWDR_0ae4: .ascii "`\0`\0",
_RODATA_G_STISPYUVSHARPENWDR_0ae8: .ascii "`\0`\0",
_RODATA_G_STISPYUVSHARPENWDR_0aec: .ascii "`\0`\0",
_RODATA_G_STISPYUVSHARPENWDR_0af0: .ascii "`\0`\0",
_RODATA_G_STISPYUVSHARPENWDR_0af4: .ascii "`\0AAFFFF77#\x1e\x14\n\n\n\n\nFFPPPP<<-(\x1e\x14\x0f\x0f\x0f\x0f\x08\x08\x07\x07\x07\x06\x05\x05\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0b20: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0b24: .ascii "\0\0\t\t\x08\x08\x07\x07\x06\x06\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0b30: .ascii "\0\0\0\0",
_RODATA_G_STISPYUVSHARPENWDR_0b34: .ascii "\0\0\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0>>>>>>>>>>>>>>>>\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f                \x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0ba8: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0bac: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0bb0: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0bb4: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0bb8: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0bbc: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0bc0: .ascii "\xa0\0\xa0\0",
_RODATA_G_STISPYUVSHARPENWDR_0bc4: .ascii "\xa0\0"


_RODATA_G_STPREGAMMA_0000: .ascii "\0\0\0\0",
_RODATA_G_STPREGAMMA_0004: .ascii "\0\0\0\0",
_RODATA_G_STPREGAMMA_0008: .ascii "\x810\0\0",
_RODATA_G_STPREGAMMA_000c: .ascii "sT\0\0",
_RODATA_G_STPREGAMMA_0010: .ascii "\xcet\0\0",
_RODATA_G_STPREGAMMA_0014: .ascii "\t\x93\0\0",
_RODATA_G_STPREGAMMA_0018: .ascii "\xc5\xaf\0\0",
_RODATA_G_STPREGAMMA_001c: .ascii "_\xcb\0\0",
_RODATA_G_STPREGAMMA_0020: .ascii "\x10\xe6\0\0",
_RODATA_G_STPREGAMMA_0024: .ascii "\0\0\x01\0",
_RODATA_G_STPREGAMMA_0028: .ascii "L\x19\x01\0",
_RODATA_G_STPREGAMMA_002c: .ascii "\x082\x01\0",
_RODATA_G_STPREGAMMA_0030: .ascii "HJ\x01\0",
_RODATA_G_STPREGAMMA_0034: .ascii "\x17b\x01\0",
_RODATA_G_STPREGAMMA_0038: .ascii "\x81y\x01\0",
_RODATA_G_STPREGAMMA_003c: .ascii "\x90\x90\x01\0",
_RODATA_G_STPREGAMMA_0040: .ascii "K\xa7\x01\0",
_RODATA_G_STPREGAMMA_0044: .ascii "\xb9\xbd\x01\0",
_RODATA_G_STPREGAMMA_0048: .ascii "\xdf\xd3\x01\0",
_RODATA_G_STPREGAMMA_004c: .ascii "\xc3\xe9\x01\0",
_RODATA_G_STPREGAMMA_0050: .ascii "i\xff\x01\0",
_RODATA_G_STPREGAMMA_0054: .ascii "\xd6\x14\x02\0",
_RODATA_G_STPREGAMMA_0058: .ascii "\x0b*\x02\0",
_RODATA_G_STPREGAMMA_005c: .ascii "\r?\x02\0",
_RODATA_G_STPREGAMMA_0060: .ascii "\xdeS\x02\0",
_RODATA_G_STPREGAMMA_0064: .ascii "\x81h\x02\0",
_RODATA_G_STPREGAMMA_0068: .ascii "\xf9|\x02\0",
_RODATA_G_STPREGAMMA_006c: .ascii "F\x91\x02\0",
_RODATA_G_STPREGAMMA_0070: .ascii "l\xa5\x02\0",
_RODATA_G_STPREGAMMA_0074: .ascii "l\xb9\x02\0",
_RODATA_G_STPREGAMMA_0078: .ascii "G\xcd\x02\0",
_RODATA_G_STPREGAMMA_007c: .ascii "\xff\xe0\x02\0",
_RODATA_G_STPREGAMMA_0080: .ascii "\x96\xf4\x02\0",
_RODATA_G_STPREGAMMA_0084: .ascii "\x0c\x08\x03\0",
_RODATA_G_STPREGAMMA_0088: .ascii "c\x1b\x03\0",
_RODATA_G_STPREGAMMA_008c: .ascii "\x9d.\x03\0",
_RODATA_G_STPREGAMMA_0090: .ascii "\xb9A\x03\0",
_RODATA_G_STPREGAMMA_0094: .ascii "\xbaT\x03\0",
_RODATA_G_STPREGAMMA_0098: .ascii "\xa0g\x03\0",
_RODATA_G_STPREGAMMA_009c: .ascii "kz\x03\0",
_RODATA_G_STPREGAMMA_00a0: .ascii "\x1e\x8d\x03\0",
_RODATA_G_STPREGAMMA_00a4: .ascii "\xb8\x9f\x03\0",
_RODATA_G_STPREGAMMA_00a8: .ascii ":\xb2\x03\0",
_RODATA_G_STPREGAMMA_00ac: .ascii "\xa5\xc4\x03\0",
_RODATA_G_STPREGAMMA_00b0: .ascii "\xfa\xd6\x03\0",
_RODATA_G_STPREGAMMA_00b4: .ascii "9\xe9\x03\0",
_RODATA_G_STPREGAMMA_00b8: .ascii "b\xfb\x03\0",
_RODATA_G_STPREGAMMA_00bc: .ascii "x\r\x04\0",
_RODATA_G_STPREGAMMA_00c0: .ascii "y\x1f\x04\0",
_RODATA_G_STPREGAMMA_00c4: .ascii "f1\x04\0",
_RODATA_G_STPREGAMMA_00c8: .ascii "@C\x04\0",
_RODATA_G_STPREGAMMA_00cc: .ascii "\x08U\x04\0",
_RODATA_G_STPREGAMMA_00d0: .ascii "\xbef\x04\0",
_RODATA_G_STPREGAMMA_00d4: .ascii "bx\x04\0",
_RODATA_G_STPREGAMMA_00d8: .ascii "\xf4\x89\x04\0",
_RODATA_G_STPREGAMMA_00dc: .ascii "v\x9b\x04\0",
_RODATA_G_STPREGAMMA_00e0: .ascii "\xe7\xac\x04\0",
_RODATA_G_STPREGAMMA_00e4: .ascii "G\xbe\x04\0",
_RODATA_G_STPREGAMMA_00e8: .ascii "\x98\xcf\x04\0",
_RODATA_G_STPREGAMMA_00ec: .ascii "\xda\xe0\x04\0",
_RODATA_G_STPREGAMMA_00f0: .ascii "\x0c\xf2\x04\0",
_RODATA_G_STPREGAMMA_00f4: .ascii "/\x03\x05\0",
_RODATA_G_STPREGAMMA_00f8: .ascii "D\x14\x05\0",
_RODATA_G_STPREGAMMA_00fc: .ascii "J%\x05\0",
_RODATA_G_STPREGAMMA_0100: .ascii "C6\x05\0",
_RODATA_G_STPREGAMMA_0104: .ascii "-G\x05\0",
_RODATA_G_STPREGAMMA_0108: .ascii "\nX\x05\0",
_RODATA_G_STPREGAMMA_010c: .ascii "\xdah\x05\0",
_RODATA_G_STPREGAMMA_0110: .ascii "\x9dy\x05\0",
_RODATA_G_STPREGAMMA_0114: .ascii "S\x8a\x05\0",
_RODATA_G_STPREGAMMA_0118: .ascii "\xfc\x9a\x05\0",
_RODATA_G_STPREGAMMA_011c: .ascii "\x99\xab\x05\0",
_RODATA_G_STPREGAMMA_0120: .ascii "*\xbc\x05\0",
_RODATA_G_STPREGAMMA_0124: .ascii "\xaf\xcc\x05\0",
_RODATA_G_STPREGAMMA_0128: .ascii "(\xdd\x05\0",
_RODATA_G_STPREGAMMA_012c: .ascii "\x96\xed\x05\0",
_RODATA_G_STPREGAMMA_0130: .ascii "\xf8\xfd\x05\0",
_RODATA_G_STPREGAMMA_0134: .ascii "P\x0e\x06\0",
_RODATA_G_STPREGAMMA_0138: .ascii "\x9c\x1e\x06\0",
_RODATA_G_STPREGAMMA_013c: .ascii "\xdd.\x06\0",
_RODATA_G_STPREGAMMA_0140: .ascii "\x14?\x06\0",
_RODATA_G_STPREGAMMA_0144: .ascii "@O\x06\0",
_RODATA_G_STPREGAMMA_0148: .ascii "b_\x06\0",
_RODATA_G_STPREGAMMA_014c: .ascii "zo\x06\0",
_RODATA_G_STPREGAMMA_0150: .ascii "\x87\x7f\x06\0",
_RODATA_G_STPREGAMMA_0154: .ascii "\x8b\x8f\x06\0",
_RODATA_G_STPREGAMMA_0158: .ascii "\x85\x9f\x06\0",
_RODATA_G_STPREGAMMA_015c: .ascii "v\xaf\x06\0",
_RODATA_G_STPREGAMMA_0160: .ascii "]\xbf\x06\0",
_RODATA_G_STPREGAMMA_0164: .ascii ":\xcf\x06\0",
_RODATA_G_STPREGAMMA_0168: .ascii "\x0f\xdf\x06\0",
_RODATA_G_STPREGAMMA_016c: .ascii "\xda\xee\x06\0",
_RODATA_G_STPREGAMMA_0170: .ascii "\x9c\xfe\x06\0",
_RODATA_G_STPREGAMMA_0174: .ascii "V\x0e\x07\0",
_RODATA_G_STPREGAMMA_0178: .ascii "\x06\x1e\x07\0",
_RODATA_G_STPREGAMMA_017c: .ascii "\xae-\x07\0",
_RODATA_G_STPREGAMMA_0180: .ascii "N=\x07\0",
_RODATA_G_STPREGAMMA_0184: .ascii "\xe5L\x07\0",
_RODATA_G_STPREGAMMA_0188: .ascii "t\\x07\0",
_RODATA_G_STPREGAMMA_018c: .ascii "\xfbk\x07\0",
_RODATA_G_STPREGAMMA_0190: .ascii "y{\x07\0",
_RODATA_G_STPREGAMMA_0194: .ascii "\xf0\x8a\x07\0",
_RODATA_G_STPREGAMMA_0198: .ascii "^\x9a\x07\0",
_RODATA_G_STPREGAMMA_019c: .ascii "\xc5\xa9\x07\0",
_RODATA_G_STPREGAMMA_01a0: .ascii "$\xb9\x07\0",
_RODATA_G_STPREGAMMA_01a4: .ascii "|\xc8\x07\0",
_RODATA_G_STPREGAMMA_01a8: .ascii "\xcc\xd7\x07\0",
_RODATA_G_STPREGAMMA_01ac: .ascii "\x14\xe7\x07\0",
_RODATA_G_STPREGAMMA_01b0: .ascii "U\xf6\x07\0",
_RODATA_G_STPREGAMMA_01b4: .ascii "\x8f\x05\x08\0",
_RODATA_G_STPREGAMMA_01b8: .ascii "\xc1\x14\x08\0",
_RODATA_G_STPREGAMMA_01bc: .ascii "\xed#\x08\0",
_RODATA_G_STPREGAMMA_01c0: .ascii "\x113\x08\0",
_RODATA_G_STPREGAMMA_01c4: .ascii "/B\x08\0",
_RODATA_G_STPREGAMMA_01c8: .ascii "EQ\x08\0",
_RODATA_G_STPREGAMMA_01cc: .ascii "U`\x08\0",
_RODATA_G_STPREGAMMA_01d0: .ascii "^o\x08\0",
_RODATA_G_STPREGAMMA_01d4: .ascii "`~\x08\0",
_RODATA_G_STPREGAMMA_01d8: .ascii "[\x8d\x08\0",
_RODATA_G_STPREGAMMA_01dc: .ascii "Q\x9c\x08\0",
_RODATA_G_STPREGAMMA_01e0: .ascii "?\xab\x08\0",
_RODATA_G_STPREGAMMA_01e4: .ascii "'\xba\x08\0",
_RODATA_G_STPREGAMMA_01e8: .ascii "\t\xc9\x08\0",
_RODATA_G_STPREGAMMA_01ec: .ascii "\xe5\xd7\x08\0",
_RODATA_G_STPREGAMMA_01f0: .ascii "\xba\xe6\x08\0",
_RODATA_G_STPREGAMMA_01f4: .ascii "\x89\xf5\x08\0",
_RODATA_G_STPREGAMMA_01f8: .ascii "R\x04\t\0",
_RODATA_G_STPREGAMMA_01fc: .ascii "\x15\x13\t\0",
_RODATA_G_STPREGAMMA_0200: .ascii "\xd2!\t\0",
_RODATA_G_STPREGAMMA_0204: .ascii "\x890\t\0",
_RODATA_G_STPREGAMMA_0208: .ascii ":?\t\0",
_RODATA_G_STPREGAMMA_020c: .ascii "\xe5M\t\0",
_RODATA_G_STPREGAMMA_0210: .ascii "\x8b\\t\0",
_RODATA_G_STPREGAMMA_0214: .ascii "+k\t\0",
_RODATA_G_STPREGAMMA_0218: .ascii "\xc5y\t\0",
_RODATA_G_STPREGAMMA_021c: .ascii "Y\x88\t\0",
_RODATA_G_STPREGAMMA_0220: .ascii "\xe8\x96\t\0",
_RODATA_G_STPREGAMMA_0224: .ascii "r\xa5\t\0",
_RODATA_G_STPREGAMMA_0228: .ascii "\xf6\xb3\t\0",
_RODATA_G_STPREGAMMA_022c: .ascii "t\xc2\t\0",
_RODATA_G_STPREGAMMA_0230: .ascii "\xed\xd0\t\0",
_RODATA_G_STPREGAMMA_0234: .ascii "a\xdf\t\0",
_RODATA_G_STPREGAMMA_0238: .ascii "\xd0\xed\t\0",
_RODATA_G_STPREGAMMA_023c: .ascii "9\xfc\t\0",
_RODATA_G_STPREGAMMA_0240: .ascii "\x9d\n\n\0",
_RODATA_G_STPREGAMMA_0244: .ascii "\xfc\x18\n\0",
_RODATA_G_STPREGAMMA_0248: .ascii "V'\n\0",
_RODATA_G_STPREGAMMA_024c: .ascii "\xab5\n\0",
_RODATA_G_STPREGAMMA_0250: .ascii "\xfbC\n\0",
_RODATA_G_STPREGAMMA_0254: .ascii "ER\n\0",
_RODATA_G_STPREGAMMA_0258: .ascii "\x8b`\n\0",
_RODATA_G_STPREGAMMA_025c: .ascii "\xccn\n\0",
_RODATA_G_STPREGAMMA_0260: .ascii "\x08}\n\0",
_RODATA_G_STPREGAMMA_0264: .ascii "@\x8b\n\0",
_RODATA_G_STPREGAMMA_0268: .ascii "r\x99\n\0",
_RODATA_G_STPREGAMMA_026c: .ascii "\xa0\xa7\n\0",
_RODATA_G_STPREGAMMA_0270: .ascii "\xc9\xb5\n\0",
_RODATA_G_STPREGAMMA_0274: .ascii "\xed\xc3\n\0",
_RODATA_G_STPREGAMMA_0278: .ascii "\r\xd2\n\0",
_RODATA_G_STPREGAMMA_027c: .ascii "(\xe0\n\0",
_RODATA_G_STPREGAMMA_0280: .ascii "?\xee\n\0",
_RODATA_G_STPREGAMMA_0284: .ascii "Q\xfc\n\0",
_RODATA_G_STPREGAMMA_0288: .ascii "^\n\x0b\0",
_RODATA_G_STPREGAMMA_028c: .ascii "g\x18\x0b\0",
_RODATA_G_STPREGAMMA_0290: .ascii "l&\x0b\0",
_RODATA_G_STPREGAMMA_0294: .ascii "l4\x0b\0",
_RODATA_G_STPREGAMMA_0298: .ascii "hB\x0b\0",
_RODATA_G_STPREGAMMA_029c: .ascii "_P\x0b\0",
_RODATA_G_STPREGAMMA_02a0: .ascii "S^\x0b\0",
_RODATA_G_STPREGAMMA_02a4: .ascii "Bl\x0b\0",
_RODATA_G_STPREGAMMA_02a8: .ascii ",z\x0b\0",
_RODATA_G_STPREGAMMA_02ac: .ascii "\x13\x88\x0b\0",
_RODATA_G_STPREGAMMA_02b0: .ascii "\xf5\x95\x0b\0",
_RODATA_G_STPREGAMMA_02b4: .ascii "\xd3\xa3\x0b\0",
_RODATA_G_STPREGAMMA_02b8: .ascii "\xad\xb1\x0b\0",
_RODATA_G_STPREGAMMA_02bc: .ascii "\x83\xbf\x0b\0",
_RODATA_G_STPREGAMMA_02c0: .ascii "U\xcd\x0b\0",
_RODATA_G_STPREGAMMA_02c4: .ascii "#\xdb\x0b\0",
_RODATA_G_STPREGAMMA_02c8: .ascii "\xed\xe8\x0b\0",
_RODATA_G_STPREGAMMA_02cc: .ascii "\xb2\xf6\x0b\0",
_RODATA_G_STPREGAMMA_02d0: .ascii "t\x04\x0c\0",
_RODATA_G_STPREGAMMA_02d4: .ascii "2\x12\x0c\0",
_RODATA_G_STPREGAMMA_02d8: .ascii "\xec\x1f\x0c\0",
_RODATA_G_STPREGAMMA_02dc: .ascii "\xa2-\x0c\0",
_RODATA_G_STPREGAMMA_02e0: .ascii "T;\x0c\0",
_RODATA_G_STPREGAMMA_02e4: .ascii "\x03I\x0c\0",
_RODATA_G_STPREGAMMA_02e8: .ascii "\xaeV\x0c\0",
_RODATA_G_STPREGAMMA_02ec: .ascii "Td\x0c\0",
_RODATA_G_STPREGAMMA_02f0: .ascii "\xf8q\x0c\0",
_RODATA_G_STPREGAMMA_02f4: .ascii "\x97\x7f\x0c\0",
_RODATA_G_STPREGAMMA_02f8: .ascii "3\x8d\x0c\0",
_RODATA_G_STPREGAMMA_02fc: .ascii "\xcb\x9a\x0c\0",
_RODATA_G_STPREGAMMA_0300: .ascii "_\xa8\x0c\0",
_RODATA_G_STPREGAMMA_0304: .ascii "\xf0\xb5\x0c\0",
_RODATA_G_STPREGAMMA_0308: .ascii "}\xc3\x0c\0",
_RODATA_G_STPREGAMMA_030c: .ascii "\x06\xd1\x0c\0",
_RODATA_G_STPREGAMMA_0310: .ascii "\x8c\xde\x0c\0",
_RODATA_G_STPREGAMMA_0314: .ascii "\x0e\xec\x0c\0",
_RODATA_G_STPREGAMMA_0318: .ascii "\x8d\xf9\x0c\0",
_RODATA_G_STPREGAMMA_031c: .ascii "\x08\x07\r\0",
_RODATA_G_STPREGAMMA_0320: .ascii "\x80\x14\r\0",
_RODATA_G_STPREGAMMA_0324: .ascii "\xf5!\r\0",
_RODATA_G_STPREGAMMA_0328: .ascii "f/\r\0",
_RODATA_G_STPREGAMMA_032c: .ascii "\xd3<\r\0",
_RODATA_G_STPREGAMMA_0330: .ascii "=J\r\0",
_RODATA_G_STPREGAMMA_0334: .ascii "\xa4W\r\0",
_RODATA_G_STPREGAMMA_0338: .ascii "\x07e\r\0",
_RODATA_G_STPREGAMMA_033c: .ascii "gr\r\0",
_RODATA_G_STPREGAMMA_0340: .ascii "\xc4\x7f\r\0",
_RODATA_G_STPREGAMMA_0344: .ascii "\x1d\x8d\r\0",
_RODATA_G_STPREGAMMA_0348: .ascii "s\x9a\r\0",
_RODATA_G_STPREGAMMA_034c: .ascii "\xc6\xa7\r\0",
_RODATA_G_STPREGAMMA_0350: .ascii "\x16\xb5\r\0",
_RODATA_G_STPREGAMMA_0354: .ascii "b\xc2\r\0",
_RODATA_G_STPREGAMMA_0358: .ascii "\xab\xcf\r\0",
_RODATA_G_STPREGAMMA_035c: .ascii "\xf1\xdc\r\0",
_RODATA_G_STPREGAMMA_0360: .ascii "4\xea\r\0",
_RODATA_G_STPREGAMMA_0364: .ascii "t\xf7\r\0",
_RODATA_G_STPREGAMMA_0368: .ascii "\xb0\x04\x0e\0",
_RODATA_G_STPREGAMMA_036c: .ascii "\xea\x11\x0e\0",
_RODATA_G_STPREGAMMA_0370: .ascii " \x1f\x0e\0",
_RODATA_G_STPREGAMMA_0374: .ascii "S,\x0e\0",
_RODATA_G_STPREGAMMA_0378: .ascii "\x839\x0e\0",
_RODATA_G_STPREGAMMA_037c: .ascii "\xb0F\x0e\0",
_RODATA_G_STPREGAMMA_0380: .ascii "\xdaS\x0e\0",
_RODATA_G_STPREGAMMA_0384: .ascii "\x01a\x0e\0",
_RODATA_G_STPREGAMMA_0388: .ascii "%n\x0e\0",
_RODATA_G_STPREGAMMA_038c: .ascii "F{\x0e\0",
_RODATA_G_STPREGAMMA_0390: .ascii "d\x88\x0e\0",
_RODATA_G_STPREGAMMA_0394: .ascii "\x7f\x95\x0e\0",
_RODATA_G_STPREGAMMA_0398: .ascii "\x97\xa2\x0e\0",
_RODATA_G_STPREGAMMA_039c: .ascii "\xad\xaf\x0e\0",
_RODATA_G_STPREGAMMA_03a0: .ascii "\xbf\xbc\x0e\0",
_RODATA_G_STPREGAMMA_03a4: .ascii "\xce\xc9\x0e\0",
_RODATA_G_STPREGAMMA_03a8: .ascii "\xdb\xd6\x0e\0",
_RODATA_G_STPREGAMMA_03ac: .ascii "\xe5\xe3\x0e\0",
_RODATA_G_STPREGAMMA_03b0: .ascii "\xeb\xf0\x0e\0",
_RODATA_G_STPREGAMMA_03b4: .ascii "\xef\xfd\x0e\0",
_RODATA_G_STPREGAMMA_03b8: .ascii "\xf0\n\x0f\0",
_RODATA_G_STPREGAMMA_03bc: .ascii "\xef\x17\x0f\0",
_RODATA_G_STPREGAMMA_03c0: .ascii "\xea$\x0f\0",
_RODATA_G_STPREGAMMA_03c4: .ascii "\xe31\x0f\0",
_RODATA_G_STPREGAMMA_03c8: .ascii "\xd9>\x0f\0",
_RODATA_G_STPREGAMMA_03cc: .ascii "\xccK\x0f\0",
_RODATA_G_STPREGAMMA_03d0: .ascii "\xbdX\x0f\0",
_RODATA_G_STPREGAMMA_03d4: .ascii "\xabe\x0f\0",
_RODATA_G_STPREGAMMA_03d8: .ascii "\x96r\x0f\0",
_RODATA_G_STPREGAMMA_03dc: .ascii "~\x7f\x0f\0",
_RODATA_G_STPREGAMMA_03e0: .ascii "d\x8c\x0f\0",
_RODATA_G_STPREGAMMA_03e4: .ascii "G\x99\x0f\0",
_RODATA_G_STPREGAMMA_03e8: .ascii "'\xa6\x0f\0",
_RODATA_G_STPREGAMMA_03ec: .ascii "\x05\xb3\x0f\0",
_RODATA_G_STPREGAMMA_03f0: .ascii "\xe0\xbf\x0f\0",
_RODATA_G_STPREGAMMA_03f4: .ascii "\xb8\xcc\x0f\0",
_RODATA_G_STPREGAMMA_03f8: .ascii "\x8e\xd9\x0f\0",
_RODATA_G_STPREGAMMA_03fc: .ascii "a\xe6\x0f\0",
_RODATA_G_STPREGAMMA_0400: .ascii "2\xf3\x0f\0",
_RODATA_G_STPREGAMMA_0404: .ascii "\xff\xff\x0f\0",
_RODATA_G_STPREGAMMA_0408: .ascii ""


_RODATA_SENSOR_REGISTER_CALLBACK_STR1_4_0000: .ascii "[Func]:%s [Line]:%d [Info]:Isp[%d] SnsCtx malloc memory failed!\n\0\0\0\0",
_RODATA_SENSOR_REGISTER_CALLBACK_STR1_4_0044: .ascii "[Func]:%s [Line]:%d [Info]:sensor register callback function failed!\n\0\0\0",
_RODATA_SENSOR_REGISTER_CALLBACK_STR1_4_008c: .ascii "[Func]:%s [Line]:%d [Info]:sensor register callback function to ae lib failed!\n\0",
_RODATA_SENSOR_REGISTER_CALLBACK_STR1_4_00dc: .ascii "[Func]:%s [Line]:%d [Info]:sensor register callback function to awb lib failed!\n\0"


_RODATA_SENSOR_UNREGISTER_CALLBACK_STR1_4_0000: .ascii "[Func]:%s [Line]:%d [Info]:sensor unregister callback function failed!\n\0",
_RODATA_SENSOR_UNREGISTER_CALLBACK_STR1_4_0048: .ascii "[Func]:%s [Line]:%d [Info]:sensor unregister callback function to ae lib failed!\n\0\0\0",
_RODATA_SENSOR_UNREGISTER_CALLBACK_STR1_4_009c: .ascii "[Func]:%s [Line]:%d [Info]:sensor unregister callback function to awb lib failed!\n\0"


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.mn34220_set_bus_info
@ Size: 0x24
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN mn34220_set_bus_info
    /*00000000*/ LDR         R3, WORD_001c               @ LDR         R3, [PC, #20]               @ 0x000000000000001c 
    /*00000004*/ LDR         R2, WORD_0020               @ LDR         R2, [PC, #20]               @ 0x0000000000000020 
    /*00000008*/ ADD         R3, PC, R3 
    /*0000000c*/ LDR         R3, [R3, R2] 
    /*00000010*/ STRB        R1, [R3, R0] 
    /*00000014*/ MOV         R0, #0 
    /*00000018*/ BX          LR 
    /*0000001c*/ WORD_001c: .word 0x0000000c
    /*00000020*/ WORD_0020: .word 0x00000000
FUNC_END mn34220_set_bus_info


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_set_pixel_detect
@ Size: 0x16c
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_set_pixel_detect
    /*00000000*/ LDR         IP, WORD_0158               @ LDR         IP, [PC, #336]              @ 0x0000000000000158 
    /*00000004*/ LDR         R3, WORD_015c               @ LDR         R3, [PC, #336]              @ 0x000000000000015c 
    /*00000008*/ ADD         IP, PC, IP 
    /*0000000c*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000010*/ LDR         R3, [IP, R3] 
    /*00000014*/ LDR         R5, [R3, R0, LSL #2] 
    /*00000018*/ CMP         R5, #0 
    /*0000001c*/ BEQ         cmos_set_pixel_detect_x130
    /*00000020*/ LDR         R3, [R5, #12] 
    /*00000024*/ CMP         R3, #3 
    /*00000028*/ POPEQ       {R4, R5, R6, R7, R8, PC} 
    /*0000002c*/ LDRB        R3, [R5, #8] 
    /*00000030*/ CMP         R3, #2 
    /*00000034*/ BEQ         cmos_set_pixel_detect_x5c
    /*00000038*/ CMP         R3, #1 
    /*0000003c*/ BEQ         cmos_set_pixel_detect_x118
    /*00000040*/ CMP         R3, #3 
    /*00000044*/ BEQ         cmos_set_pixel_detect_x124
    /*00000048*/ CMP         R3, #4 
    /*0000004c*/ MOVWEQ      R7, #27022                  @ 0x698e 
    /*00000050*/ MOVWEQ      R8, #27024                  @ 0x6990 
    /*00000054*/ BEQ         cmos_set_pixel_detect_x64
    /*00000058*/ POP         {R4, R5, R6, R7, R8, PC} 
    cmos_set_pixel_detect_x5c:
    /*0000005c*/ MOVW        R7, #6754                   @ 0x1a62 
    /*00000060*/ MOVW        R8, #6756                   @ 0x1a64 
    cmos_set_pixel_detect_x64:
    /*00000064*/ CMP         R1, #0 
    /*00000068*/ MOV         R6, R1 
    /*0000006c*/ MOV         R4, R0 
    /*00000070*/ BNE         cmos_set_pixel_detect_x98
    /*00000074*/ LDRB        R2, [R5, #2137]             @ 0x859 
    /*00000078*/ MOV         R1, #832                    @ 0x340 
    /*0000007c*/ BL          mn34220_write_register
    /*00000080*/ LDRB        R2, [R5, #2136]             @ 0x858 
    /*00000084*/ MOV         R0, R4 
    /*00000088*/ MOVW        R1, #833                    @ 0x341 
    /*0000008c*/ BL          mn34220_write_register
    /*00000090*/ STR         R6, [R5, #4] 
    /*00000094*/ POP         {R4, R5, R6, R7, R8, PC} 
    cmos_set_pixel_detect_x98:
    /*00000098*/ LSR         R2, R8, #8 
    /*0000009c*/ MOV         R1, #832                    @ 0x340 
    /*000000a0*/ BL          mn34220_write_register
    /*000000a4*/ AND         R2, R8, #252                @ 0xfc 
    /*000000a8*/ MOVW        R1, #833                    @ 0x341 
    /*000000ac*/ MOV         R0, R4 
    /*000000b0*/ BL          mn34220_write_register
    /*000000b4*/ LSR         R2, R7, #8 
    /*000000b8*/ MOVW        R1, #514                    @ 0x202 
    /*000000bc*/ MOV         R0, R4 
    /*000000c0*/ BL          mn34220_write_register
    /*000000c4*/ AND         R2, R7, #238                @ 0xee 
    /*000000c8*/ MOVW        R1, #515                    @ 0x203 
    /*000000cc*/ MOV         R0, R4 
    /*000000d0*/ BL          mn34220_write_register
    /*000000d4*/ MOV         R2, #1 
    /*000000d8*/ MOV         R1, #516                    @ 0x204 
    /*000000dc*/ MOV         R0, R4 
    /*000000e0*/ BL          mn34220_write_register
    /*000000e4*/ MOV         R2, #0 
    /*000000e8*/ MOVW        R1, #517                    @ 0x205 
    /*000000ec*/ MOV         R0, R4 
    /*000000f0*/ BL          mn34220_write_register
    /*000000f4*/ MOV         R2, #1 
    /*000000f8*/ MOVW        R1, #12552                  @ 0x3108 
    /*000000fc*/ MOV         R0, R4 
    /*00000100*/ BL          mn34220_write_register
    /*00000104*/ MOV         R0, R4 
    /*00000108*/ MOV         R2, #0 
    /*0000010c*/ MOVW        R1, #12553                  @ 0x3109 
    /*00000110*/ POP         {R4, R5, R6, R7, R8, LR} 
    cmos_set_pixel_detect_x114:
    /*00000114*/ B           cmos_set_pixel_detect_x114  @ mn34220_write_register [R_ARM_JUMP24: +0x0] 
    cmos_set_pixel_detect_x118:
    /*00000118*/ MOVW        R7, #13510                  @ 0x34c6 
    /*0000011c*/ MOVW        R8, #13512                  @ 0x34c8 
    /*00000120*/ B           cmos_set_pixel_detect_x64
    cmos_set_pixel_detect_x124:
    /*00000124*/ MOVW        R7, #18022                  @ 0x4666 
    /*00000128*/ MOVW        R8, #18024                  @ 0x4668 
    /*0000012c*/ B           cmos_set_pixel_detect_x64
    cmos_set_pixel_detect_x130:
    /*00000130*/ LDR         R1, WORD_0160               @ LDR         R1, [PC, #40]               @ 0x0000000000000160 
    /*00000134*/ MOVW        R3, #1463                   @ 0x5b7 
    /*00000138*/ LDR         R2, WORD_0164               @ LDR         R2, [PC, #36]               @ 0x0000000000000164 
    /*0000013c*/ LDR         R0, [IP, R1] 
    /*00000140*/ LDR         R1, WORD_0168               @ LDR         R1, [PC, #32]               @ 0x0000000000000168 
    /*00000144*/ ADD         R2, PC, R2 
    /*00000148*/ ADD         R1, PC, R1 
    /*0000014c*/ LDR         R0, [R0] 
    /*00000150*/ POP         {R4, R5, R6, R7, R8, LR} 
    cmos_set_pixel_detect_x154:
    /*00000154*/ B           cmos_set_pixel_detect_x154  @ fprintf [R_ARM_JUMP24: +0x0] 
    /*00000158*/ WORD_0158: .word 0x00000148
    /*0000015c*/ WORD_015c: .word 0x00000000
    /*00000160*/ WORD_0160: .word 0x00000000
    /*00000164*/ WORD_0164: .word 0x00000018
    /*00000168*/ WORD_0168: .word 0x00000018
FUNC_END cmos_set_pixel_detect


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_get_inttime_max
@ Size: 0x95c
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_get_inttime_max
    /*00000000*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000004*/ CMP         R2, #0 
    /*00000008*/ LDR         LR, WORD_08fc               @ LDR         LR, [PC, #2284]             @ 0x00000000000008fc 
    /*0000000c*/ SUB         SP, SP, #20 
    /*00000010*/ ADD         LR, PC, LR 
    /*00000014*/ LDR         R7, [SP, #56]               @ 0x38 
    /*00000018*/ LDR         R4, [SP, #60]               @ 0x3c 
    /*0000001c*/ BEQ         cmos_get_inttime_max_x650
    /*00000020*/ CMP         R3, #0 
    /*00000024*/ MOV         IP, R3 
    /*00000028*/ BEQ         cmos_get_inttime_max_x670
    /*0000002c*/ CMP         R7, #0 
    /*00000030*/ BEQ         cmos_get_inttime_max_x690
    /*00000034*/ CMP         R4, #0 
    /*00000038*/ BEQ         cmos_get_inttime_max_x6b0
    /*0000003c*/ LDR         R3, WORD_0900               @ LDR         R3, [PC, #2236]             @ 0x0000000000000900 
    /*00000040*/ LDR         R3, [LR, R3] 
    /*00000044*/ LDR         R6, [R3, R0, LSL #2] 
    /*00000048*/ CMP         R6, #0 
    /*0000004c*/ BEQ         cmos_get_inttime_max_x32c
    /*00000050*/ LDR         R3, [R6, #12] 
    /*00000054*/ SUB         R4, R3, #4 
    /*00000058*/ CMP         R4, #1 
    /*0000005c*/ BLS         cmos_get_inttime_max_x208
    /*00000060*/ SUB         R8, R3, #7 
    /*00000064*/ CMP         R8, #1 
    /*00000068*/ BLS         cmos_get_inttime_max_xe0
    /*0000006c*/ SUB         R5, R3, #10 
    /*00000070*/ CMP         R5, #1 
    /*00000074*/ BLS         cmos_get_inttime_max_x24c
    /*00000078*/ CMP         R3, #3 
    /*0000007c*/ BEQ         cmos_get_inttime_max_x380
    /*00000080*/ CMP         R3, #6 
    /*00000084*/ BEQ         cmos_get_inttime_max_x174
    /*00000088*/ CMP         R3, #9 
    /*0000008c*/ BEQ         cmos_get_inttime_max_x420
    /*00000090*/ CMP         R1, #1 
    /*00000094*/ BEQ         cmos_get_inttime_max_x76c
    cmos_get_inttime_max_x98:
    /*00000098*/ CMP         R5, #1 
    /*0000009c*/ BLS         cmos_get_inttime_max_x6d0
    /*000000a0*/ CMP         R3, #3 
    /*000000a4*/ BEQ         cmos_get_inttime_max_x60c
    /*000000a8*/ CMP         R3, #6 
    /*000000ac*/ BEQ         cmos_get_inttime_max_x78c
    /*000000b0*/ CMP         R3, #9 
    /*000000b4*/ BEQ         cmos_get_inttime_max_x7ec
    /*000000b8*/ LDR         R3, [IP] 
    cmos_get_inttime_max_xbc:
    /*000000bc*/ STR         R3, [R7] 
    /*000000c0*/ LDR         R3, [IP, #4] 
    /*000000c4*/ STR         R3, [R7, #4] 
    /*000000c8*/ LDR         R3, [IP, #8] 
    /*000000cc*/ STR         R3, [R7, #8] 
    /*000000d0*/ LDR         R3, [IP, #12] 
    /*000000d4*/ STR         R3, [R7, #12] 
    cmos_get_inttime_max_xd8:
    /*000000d8*/ ADD         SP, SP, #20 
    /*000000dc*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    cmos_get_inttime_max_xe0:
    /*000000e0*/ LDR         R9, [R2] 
    /*000000e4*/ LDR         R0, [R2, #4] 
    /*000000e8*/ LDR         R4, [R6, #2128]             @ 0x850 
    /*000000ec*/ CMP         R9, #0 
    /*000000f0*/ MOVEQ       R9, #1 
    /*000000f4*/ CMP         R0, #0 
    /*000000f8*/ SUB         R5, R4, #2 
    /*000000fc*/ MOVEQ       R0, #1 
    /*00000100*/ MUL         R0, R0, R9 
    /*00000104*/ LSL         R5, R5, #12 
    /*00000108*/ UDIV        R0, R5, R0 
    /*0000010c*/ CMP         R0, #1 
    /*00000110*/ BHI         cmos_get_inttime_max_x3d8
    cmos_get_inttime_max_x114:
    /*00000114*/ CMP         R1, #1 
    /*00000118*/ BEQ         cmos_get_inttime_max_x76c
    /*0000011c*/ CMP         R8, #1 
    /*00000120*/ SUBHI       R5, R3, #10 
    /*00000124*/ BHI         cmos_get_inttime_max_x98
    /*00000128*/ SUB         R4, R4, #2 
    /*0000012c*/ MOV         R3, #64                     @ 0x40 
    /*00000130*/ B           cmos_get_inttime_max_x140
    cmos_get_inttime_max_x134:
    /*00000134*/ ADD         R3, R3, #1 
    /*00000138*/ CMP         R3, #4096                   @ 0x1000 
    /*0000013c*/ BEQ         cmos_get_inttime_max_x8c0
    cmos_get_inttime_max_x140:
    /*00000140*/ MUL         R2, R3, R3 
    /*00000144*/ UBFX        R2, R2, #11, #20 
    /*00000148*/ CMP         R2, R4 
    /*0000014c*/ BLS         cmos_get_inttime_max_x134
    /*00000150*/ SUB         R3, R3, #1 
    /*00000154*/ UBFX        R3, R3, #5, #26 
    cmos_get_inttime_max_x158:
    /*00000158*/ STR         R3, [IP, #4] 
    /*0000015c*/ MOV         R3, #2 
    /*00000160*/ STR         R3, [IP] 
    /*00000164*/ LDR         R2, [R6, #2128]             @ 0x850 
    /*00000168*/ SUB         R2, R2, #2 
    /*0000016c*/ STR         R2, [IP, #8] 
    /*00000170*/ B           cmos_get_inttime_max_xbc
    cmos_get_inttime_max_x174:
    /*00000174*/ LDR         R4, WORD_0904               @ LDR         R4, [PC, #1928]             @ 0x0000000000000904 
    /*00000178*/ ADD         R4, PC, R4 
    /*0000017c*/ LDR         R4, [R4, R0, LSL #2] 
    /*00000180*/ CMP         R4, #1 
    /*00000184*/ BEQ         cmos_get_inttime_max_x564
    /*00000188*/ LDR         R9, [R6, #2132]             @ 0x854 
    /*0000018c*/ LDR         R0, [R6, #2140]             @ 0x85c 
    /*00000190*/ LDR         FP, [R2] 
    /*00000194*/ SUB         R4, R9, R0 
    /*00000198*/ LDR         R9, [R6, #2144]             @ 0x860 
    /*0000019c*/ CMP         FP, #0 
    /*000001a0*/ LDR         SL, [R2, #4] 
    /*000001a4*/ STR         R9, [SP, #12] 
    /*000001a8*/ MOVNE       R9, FP 
    /*000001ac*/ MOVEQ       R9, #1 
    /*000001b0*/ CMP         SL, #0 
    /*000001b4*/ STR         R9, [SP] 
    /*000001b8*/ ADD         R0, SL, #64                 @ 0x40 
    /*000001bc*/ LDR         R9, [SP, #12] 
    /*000001c0*/ MOVEQ       SL, #1 
    /*000001c4*/ MUL         FP, FP, R0 
    /*000001c8*/ SUB         R9, R4, R9 
    /*000001cc*/ LDR         R4, [R6, #2128]             @ 0x850 
    /*000001d0*/ SUB         R9, R9, #30 
    /*000001d4*/ STR         R9, [SP, #12] 
    /*000001d8*/ ADD         FP, FP, #4096               @ 0x1000 
    /*000001dc*/ LDR         R9, [SP] 
    /*000001e0*/ SUB         R0, R4, #30 
    /*000001e4*/ MUL         SL, SL, R9 
    /*000001e8*/ LDR         R9, [SP, #12] 
    /*000001ec*/ LSL         R0, R0, #12 
    /*000001f0*/ LSL         R9, R9, #12 
    /*000001f4*/ UDIV        FP, R0, FP 
    /*000001f8*/ UDIV        R9, R9, SL 
    /*000001fc*/ CMP         FP, R9 
    /*00000200*/ MOVCS       FP, R9 
    /*00000204*/ B           cmos_get_inttime_max_x284
    cmos_get_inttime_max_x208:
    /*00000208*/ LDR         R3, [R6, #2128]             @ 0x850 
    /*0000020c*/ LDR         R0, [R2] 
    /*00000210*/ SUB         R3, R3, #2 
    /*00000214*/ CMP         R0, #0 
    /*00000218*/ LSL         R3, R3, #6 
    /*0000021c*/ MOVEQ       R0, #1 
    /*00000220*/ UDIV        R3, R3, R0 
    /*00000224*/ CMP         R3, #1 
    /*00000228*/ BHI         cmos_get_inttime_max_x358
    /*0000022c*/ CMP         R1, #1 
    /*00000230*/ BEQ         cmos_get_inttime_max_x76c
    /*00000234*/ MOV         R3, #2 
    /*00000238*/ STR         R3, [IP] 
    /*0000023c*/ LDR         R2, [R6, #2128]             @ 0x850 
    /*00000240*/ SUB         R2, R2, #2 
    /*00000244*/ STR         R2, [IP, #4] 
    /*00000248*/ B           cmos_get_inttime_max_xbc
    cmos_get_inttime_max_x24c:
    /*0000024c*/ LDM         R2, {R9, FP} 
    /*00000250*/ LDR         R0, [R2, #8] 
    /*00000254*/ LDR         R4, [R6, #2128]             @ 0x850 
    /*00000258*/ CMP         R9, #0 
    /*0000025c*/ MOVEQ       R9, #1 
    /*00000260*/ CMP         FP, #0 
    /*00000264*/ MOVEQ       FP, #1 
    /*00000268*/ SUB         SL, R4, #2 
    /*0000026c*/ CMP         R0, #0 
    /*00000270*/ MUL         FP, FP, R9 
    /*00000274*/ MOVEQ       R0, #1 
    /*00000278*/ MUL         R0, R0, FP 
    /*0000027c*/ LSL         FP, SL, #18 
    /*00000280*/ UDIV        FP, FP, R0 
    cmos_get_inttime_max_x284:
    /*00000284*/ CMP         FP, #1 
    /*00000288*/ BLS         cmos_get_inttime_max_x114
    /*0000028c*/ CMP         R5, #1 
    /*00000290*/ BLS         cmos_get_inttime_max_x4d4
    /*00000294*/ CMP         R3, #9 
    /*00000298*/ CMPNE       R3, #6 
    /*0000029c*/ SUB         R3, R3, #2 
    /*000002a0*/ MOVEQ       R1, #1 
    /*000002a4*/ MOVNE       R1, #0 
    /*000002a8*/ CMP         R3, #1 
    /*000002ac*/ MOVHI       R3, R1 
    /*000002b0*/ ORRLS       R3, R1, #1 
    /*000002b4*/ CMP         R3, #0 
    /*000002b8*/ BEQ         cmos_get_inttime_max_xd8
    /*000002bc*/ STR         FP, [IP] 
    /*000002c0*/ MOV         R0, #2 
    /*000002c4*/ LDR         R3, [R2] 
    /*000002c8*/ MUL         FP, R3, FP 
    /*000002cc*/ LSR         FP, FP, #6 
    /*000002d0*/ STR         FP, [IP, #4] 
    /*000002d4*/ LDR         R3, [R2, #4] 
    /*000002d8*/ MUL         R3, R3, FP 
    /*000002dc*/ LSR         R3, R3, #6 
    /*000002e0*/ STR         R3, [IP, #8] 
    /*000002e4*/ LDR         R1, [R2, #8] 
    /*000002e8*/ MUL         R3, R1, R3 
    /*000002ec*/ LSR         R3, R3, #6 
    /*000002f0*/ STR         R3, [IP, #12] 
    /*000002f4*/ STR         R0, [R7] 
    cmos_get_inttime_max_x2f8:
    /*000002f8*/ LDR         R3, [R2] 
    /*000002fc*/ UBFX        R1, R3, #5, #26 
    /*00000300*/ STR         R1, [R7, #4] 
    /*00000304*/ LDR         R3, [R2, #4] 
    /*00000308*/ MUL         R3, R3, R1 
    /*0000030c*/ LSR         R3, R3, #6 
    /*00000310*/ STR         R3, [R7, #8] 
    /*00000314*/ LDR         R2, [R2, #8] 
    /*00000318*/ MUL         R3, R2, R3 
    /*0000031c*/ LSR         R3, R3, #6 
    /*00000320*/ STR         R3, [R7, #12] 
    /*00000324*/ ADD         SP, SP, #20 
    /*00000328*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    cmos_get_inttime_max_x32c:
    /*0000032c*/ LDR         R1, WORD_0908               @ LDR         R1, [PC, #1492]             @ 0x0000000000000908 
    /*00000330*/ MOVW        R3, #1035                   @ 0x40b 
    /*00000334*/ LDR         R2, WORD_090c               @ LDR         R2, [PC, #1488]             @ 0x000000000000090c 
    /*00000338*/ LDR         R0, [LR, R1] 
    /*0000033c*/ LDR         R1, WORD_0910               @ LDR         R1, [PC, #1484]             @ 0x0000000000000910 
    /*00000340*/ ADD         R2, PC, R2 
    /*00000344*/ ADD         R1, PC, R1 
    cmos_get_inttime_max_x348:
    /*00000348*/ LDR         R0, [R0] 
    /*0000034c*/ ADD         SP, SP, #20 
    /*00000350*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    cmos_get_inttime_max_x354:
    /*00000354*/ B           cmos_get_inttime_max_x354   @ fprintf [R_ARM_JUMP24: +0x0] 
    cmos_get_inttime_max_x358:
    /*00000358*/ STR         R3, [IP] 
    /*0000035c*/ MOV         R1, #2 
    /*00000360*/ LDR         R3, [R6, #2128]             @ 0x850 
    /*00000364*/ SUB         R3, R3, #2 
    /*00000368*/ STR         R3, [IP, #4] 
    /*0000036c*/ STR         R1, [R7] 
    /*00000370*/ LDR         R3, [R2] 
    /*00000374*/ UBFX        R3, R3, #5, #26 
    /*00000378*/ STR         R3, [R7, #4] 
    /*0000037c*/ B           cmos_get_inttime_max_xd8
    cmos_get_inttime_max_x380:
    /*00000380*/ LDR         R4, WORD_0914               @ LDR         R4, [PC, #1420]             @ 0x0000000000000914 
    /*00000384*/ ADD         R4, PC, R4 
    /*00000388*/ LDR         R4, [R4, R0, LSL #2] 
    /*0000038c*/ CMP         R4, #1 
    /*00000390*/ BEQ         cmos_get_inttime_max_x510
    /*00000394*/ LDR         R0, [R6, #2132]             @ 0x854 
    /*00000398*/ LDR         R9, [R6, #2140]             @ 0x85c 
    /*0000039c*/ LDR         R4, [R6, #2128]             @ 0x850 
    /*000003a0*/ SUB         R0, R0, #30 
    /*000003a4*/ LDR         SL, [R2] 
    /*000003a8*/ SUB         R0, R0, R9 
    /*000003ac*/ SUB         R9, R4, #30 
    /*000003b0*/ CMP         SL, #0 
    /*000003b4*/ ADD         FP, SL, #64                 @ 0x40 
    /*000003b8*/ LSL         R0, R0, #6 
    /*000003bc*/ MOVEQ       SL, #1 
    /*000003c0*/ LSL         R9, R9, #6 
    /*000003c4*/ UDIV        SL, R0, SL 
    /*000003c8*/ UDIV        FP, R9, FP 
    /*000003cc*/ CMP         FP, SL 
    /*000003d0*/ MOVCS       FP, SL 
    /*000003d4*/ B           cmos_get_inttime_max_x284
    cmos_get_inttime_max_x3d8:
    /*000003d8*/ STR         R0, [IP] 
    /*000003dc*/ MOV         R1, #2 
    /*000003e0*/ LDR         R3, [R2] 
    /*000003e4*/ MUL         R0, R3, R0 
    /*000003e8*/ LSR         R0, R0, #6 
    /*000003ec*/ STR         R0, [IP, #4] 
    /*000003f0*/ LDR         R3, [R6, #2128]             @ 0x850 
    /*000003f4*/ SUB         R3, R3, #2 
    /*000003f8*/ STR         R3, [IP, #8] 
    /*000003fc*/ STR         R1, [R7] 
    /*00000400*/ LDR         R3, [R2] 
    /*00000404*/ UBFX        R1, R3, #5, #26 
    /*00000408*/ STR         R1, [R7, #4] 
    /*0000040c*/ LDR         R3, [R2, #4] 
    /*00000410*/ MUL         R3, R3, R1 
    /*00000414*/ LSR         R3, R3, #6 
    /*00000418*/ STR         R3, [R7, #8] 
    /*0000041c*/ B           cmos_get_inttime_max_xd8
    cmos_get_inttime_max_x420:
    /*00000420*/ LDR         R4, WORD_0918               @ LDR         R4, [PC, #1264]             @ 0x0000000000000918 
    /*00000424*/ ADD         R4, PC, R4 
    /*00000428*/ LDR         R4, [R4, R0, LSL #2] 
    /*0000042c*/ CMP         R4, #1 
    /*00000430*/ BEQ         cmos_get_inttime_max_x5c0
    /*00000434*/ LDR         R0, [R2, #8] 
    /*00000438*/ LDM         R2, {SL, FP} 
    /*0000043c*/ LDR         R9, [R6, #2132]             @ 0x854 
    /*00000440*/ ADD         R0, R0, #64                 @ 0x40 
    /*00000444*/ MUL         R0, FP, R0 
    /*00000448*/ CMP         SL, #0 
    /*0000044c*/ ADD         R0, R0, #4096               @ 0x1000 
    /*00000450*/ MOVNE       R4, SL 
    /*00000454*/ STR         R0, [SP] 
    /*00000458*/ MOVEQ       R4, #1 
    /*0000045c*/ LDR         R0, [R6, #2140]             @ 0x85c 
    /*00000460*/ CMP         FP, #0 
    /*00000464*/ MOVEQ       FP, #1 
    /*00000468*/ SUB         R9, R9, R0 
    /*0000046c*/ LDR         R0, [R2, #8] 
    /*00000470*/ MUL         FP, FP, R4 
    /*00000474*/ LDR         R4, [R6, #2128]             @ 0x850 
    /*00000478*/ CMP         R0, #0 
    /*0000047c*/ LDR         R0, [R6, #2144]             @ 0x860 
    /*00000480*/ SUB         R9, R9, R0 
    /*00000484*/ LDR         R0, [SP] 
    /*00000488*/ SUB         R9, R9, #30 
    /*0000048c*/ MUL         SL, SL, R0 
    /*00000490*/ LDR         R0, [R2, #8] 
    /*00000494*/ MOVEQ       R0, #1 
    /*00000498*/ ADD         SL, SL, #262144             @ 0x40000 
    /*0000049c*/ STR         SL, [SP] 
    /*000004a0*/ LDR         SL, [R6, #2148]             @ 0x864 
    /*000004a4*/ MUL         FP, R0, FP 
    /*000004a8*/ SUB         R9, R9, SL 
    /*000004ac*/ SUB         R0, R4, #30 
    /*000004b0*/ LDR         SL, [SP] 
    /*000004b4*/ LSL         R0, R0, #18 
    /*000004b8*/ LSL         R9, R9, #18 
    /*000004bc*/ UDIV        R0, R0, SL 
    /*000004c0*/ UDIV        R9, R9, FP 
    /*000004c4*/ CMP         R0, R9 
    /*000004c8*/ MOVCC       FP, R0 
    /*000004cc*/ MOVCS       FP, R9 
    /*000004d0*/ B           cmos_get_inttime_max_x284
    cmos_get_inttime_max_x4d4:
    /*000004d4*/ STR         FP, [IP] 
    /*000004d8*/ MOV         R1, #2 
    /*000004dc*/ LDR         R3, [R2] 
    /*000004e0*/ MUL         FP, R3, FP 
    /*000004e4*/ LSR         FP, FP, #6 
    /*000004e8*/ STR         FP, [IP, #4] 
    /*000004ec*/ LDR         R3, [R2, #4] 
    /*000004f0*/ MUL         FP, R3, FP 
    /*000004f4*/ LSR         FP, FP, #6 
    /*000004f8*/ STR         FP, [IP, #8] 
    /*000004fc*/ LDR         R3, [R6, #2128]             @ 0x850 
    /*00000500*/ SUB         R3, R3, #2 
    /*00000504*/ STR         R3, [IP, #12] 
    /*00000508*/ STR         R1, [R7] 
    /*0000050c*/ B           cmos_get_inttime_max_x2f8
    cmos_get_inttime_max_x510:
    /*00000510*/ LDR         R2, WORD_091c               @ LDR         R2, [PC, #1028]             @ 0x000000000000091c 
    /*00000514*/ MOV         R4, #2 
    /*00000518*/ LDR         R3, [R6, #2132]             @ 0x854 
    /*0000051c*/ ADD         R2, PC, R2 
    /*00000520*/ LDR         LR, WORD_0920               @ LDR         LR, [PC, #1016]             @ 0x0000000000000920 
    /*00000524*/ LDR         R1, [R2, R0, LSL #2] 
    /*00000528*/ LDR         R2, [R6, #2140]             @ 0x85c 
    /*0000052c*/ ADD         LR, PC, LR 
    /*00000530*/ CMP         R1, #1 
    /*00000534*/ SUBHI       R3, R3, #10 
    /*00000538*/ SUBLS       R2, R3, #760                @ 0x2f8 
    /*0000053c*/ SUBHI       R2, R3, R2 
    /*00000540*/ ADD         R1, R1, #1 
    /*00000544*/ LDR         R3, [R6, #2128]             @ 0x850 
    /*00000548*/ STR         R1, [LR, R0, LSL #2] 
    /*0000054c*/ SUB         R3, R3, #10 
    /*00000550*/ CMP         R3, R2 
    /*00000554*/ MOVCS       R3, R2 
    /*00000558*/ STR         R3, [IP] 
    /*0000055c*/ STR         R4, [R7] 
    /*00000560*/ B           cmos_get_inttime_max_xd8
    cmos_get_inttime_max_x564:
    /*00000564*/ LDR         R2, WORD_0924               @ LDR         R2, [PC, #952]              @ 0x0000000000000924 
    /*00000568*/ LDR         R3, [R6, #2132]             @ 0x854 
    /*0000056c*/ ADD         R2, PC, R2 
    /*00000570*/ LDR         LR, [R6, #2140]             @ 0x85c 
    /*00000574*/ LDR         R1, [R2, R0, LSL #2] 
    /*00000578*/ LDR         R2, [R6, #2144]             @ 0x860 
    /*0000057c*/ CMP         R1, #1 
    /*00000580*/ ADD         R1, R1, #1 
    /*00000584*/ SUBHI       R3, R3, LR 
    /*00000588*/ SUBLS       R3, R3, #780                @ 0x30c 
    /*0000058c*/ LDR         LR, WORD_0928               @ LDR         LR, [PC, #916]              @ 0x0000000000000928 
    /*00000590*/ SUBHI       R3, R3, R2 
    /*00000594*/ ADD         LR, PC, LR 
    /*00000598*/ SUBHI       R3, R3, #30 
    /*0000059c*/ LDR         R2, [R6, #2128]             @ 0x850 
    cmos_get_inttime_max_x5a0:
    /*000005a0*/ STR         R1, [LR, R0, LSL #2] 
    /*000005a4*/ SUB         R2, R2, #30 
    /*000005a8*/ CMP         R2, R3 
    /*000005ac*/ MOV         R4, #2 
    /*000005b0*/ MOVCC       R3, R2 
    /*000005b4*/ STR         R3, [IP] 
    /*000005b8*/ STR         R4, [R7] 
    /*000005bc*/ B           cmos_get_inttime_max_xd8
    cmos_get_inttime_max_x5c0:
    /*000005c0*/ LDR         R2, WORD_092c               @ LDR         R2, [PC, #868]              @ 0x000000000000092c 
    /*000005c4*/ LDR         R3, [R6, #2132]             @ 0x854 
    /*000005c8*/ ADD         R2, PC, R2 
    /*000005cc*/ LDR         R4, [R6, #2140]             @ 0x85c 
    /*000005d0*/ LDR         R1, [R2, R0, LSL #2] 
    /*000005d4*/ LDR         LR, [R6, #2148]             @ 0x864 
    /*000005d8*/ LDR         R2, [R6, #2144]             @ 0x860 
    /*000005dc*/ CMP         R1, #1 
    /*000005e0*/ SUBHI       R3, R3, R4 
    /*000005e4*/ SUBLS       R3, R3, #588                @ 0x24c 
    /*000005e8*/ SUBHI       R3, R3, LR 
    /*000005ec*/ SUBLS       R3, R3, #2 
    /*000005f0*/ LDR         LR, WORD_0930               @ LDR         LR, [PC, #824]              @ 0x0000000000000930 
    /*000005f4*/ SUBHI       R3, R3, #30 
    /*000005f8*/ SUBHI       R3, R3, R2 
    /*000005fc*/ ADD         R1, R1, #1 
    /*00000600*/ LDR         R2, [R6, #2128]             @ 0x850 
    /*00000604*/ ADD         LR, PC, LR 
    /*00000608*/ B           cmos_get_inttime_max_x5a0
    cmos_get_inttime_max_x60c:
    /*0000060c*/ LDR         R1, [R6, #2128]             @ 0x850 
    /*00000610*/ MOV         R3, #64                     @ 0x40 
    /*00000614*/ SUB         R1, R1, #30 
    /*00000618*/ B           cmos_get_inttime_max_x628
    cmos_get_inttime_max_x61c:
    /*0000061c*/ ADD         R3, R3, #1 
    /*00000620*/ CMP         R3, #4096                   @ 0x1000 
    /*00000624*/ BEQ         cmos_get_inttime_max_x8d4
    cmos_get_inttime_max_x628:
    /*00000628*/ UBFX        R2, R3, #5, #26 
    /*0000062c*/ ADD         R2, R2, #2 
    /*00000630*/ CMP         R2, R1 
    /*00000634*/ BLS         cmos_get_inttime_max_x61c
    /*00000638*/ SUB         R3, R3, #1 
    /*0000063c*/ UBFX        R3, R3, #5, #26 
    cmos_get_inttime_max_x640:
    /*00000640*/ STR         R3, [IP, #4] 
    /*00000644*/ MOV         R3, #2 
    /*00000648*/ STR         R3, [IP] 
    /*0000064c*/ B           cmos_get_inttime_max_xbc
    cmos_get_inttime_max_x650:
    /*00000650*/ LDR         R1, WORD_0908               @ LDR         R1, [PC, #688]              @ 0x0000000000000908 
    /*00000654*/ MOVW        R3, #1029                   @ 0x405 
    /*00000658*/ LDR         R2, WORD_0934               @ LDR         R2, [PC, #724]              @ 0x0000000000000934 
    /*0000065c*/ LDR         R0, [LR, R1] 
    /*00000660*/ LDR         R1, WORD_0938               @ LDR         R1, [PC, #720]              @ 0x0000000000000938 
    /*00000664*/ ADD         R2, PC, R2 
    /*00000668*/ ADD         R1, PC, R1 
    /*0000066c*/ B           cmos_get_inttime_max_x348
    cmos_get_inttime_max_x670:
    /*00000670*/ LDR         R1, WORD_0908               @ LDR         R1, [PC, #656]              @ 0x0000000000000908 
    /*00000674*/ MOVW        R3, #1030                   @ 0x406 
    /*00000678*/ LDR         R2, WORD_093c               @ LDR         R2, [PC, #700]              @ 0x000000000000093c 
    /*0000067c*/ LDR         R0, [LR, R1] 
    /*00000680*/ LDR         R1, WORD_0940               @ LDR         R1, [PC, #696]              @ 0x0000000000000940 
    /*00000684*/ ADD         R2, PC, R2 
    /*00000688*/ ADD         R1, PC, R1 
    /*0000068c*/ B           cmos_get_inttime_max_x348
    cmos_get_inttime_max_x690:
    /*00000690*/ LDR         R1, WORD_0908               @ LDR         R1, [PC, #624]              @ 0x0000000000000908 
    /*00000694*/ MOVW        R3, #1031                   @ 0x407 
    /*00000698*/ LDR         R2, WORD_0944               @ LDR         R2, [PC, #676]              @ 0x0000000000000944 
    /*0000069c*/ LDR         R0, [LR, R1] 
    /*000006a0*/ LDR         R1, WORD_0948               @ LDR         R1, [PC, #672]              @ 0x0000000000000948 
    /*000006a4*/ ADD         R2, PC, R2 
    /*000006a8*/ ADD         R1, PC, R1 
    /*000006ac*/ B           cmos_get_inttime_max_x348
    cmos_get_inttime_max_x6b0:
    /*000006b0*/ LDR         R1, WORD_0908               @ LDR         R1, [PC, #592]              @ 0x0000000000000908 
    /*000006b4*/ MOVW        R3, #1032                   @ 0x408 
    /*000006b8*/ LDR         R2, WORD_094c               @ LDR         R2, [PC, #652]              @ 0x000000000000094c 
    /*000006bc*/ LDR         R0, [LR, R1] 
    /*000006c0*/ LDR         R1, WORD_0950               @ LDR         R1, [PC, #648]              @ 0x0000000000000950 
    /*000006c4*/ ADD         R2, PC, R2 
    /*000006c8*/ ADD         R1, PC, R1 
    /*000006cc*/ B           cmos_get_inttime_max_x348
    cmos_get_inttime_max_x6d0:
    /*000006d0*/ LDR         R4, [R6, #2128]             @ 0x850 
    /*000006d4*/ MOV         R0, #64                     @ 0x40 
    /*000006d8*/ MOV         R1, #0 
    /*000006dc*/ MOV         R5, #0 
    /*000006e0*/ SUB         R4, R4, #2 
    /*000006e4*/ B           cmos_get_inttime_max_x6fc
    cmos_get_inttime_max_x6e8:
    /*000006e8*/ ADDS        R0, R0, #1 
    /*000006ec*/ ADC         R1, R1, #0 
    /*000006f0*/ CMP         R1, #0 
    /*000006f4*/ CMPEQ       R0, #4096                   @ 0x1000 
    /*000006f8*/ BEQ         cmos_get_inttime_max_x8c8
    cmos_get_inttime_max_x6fc:
    /*000006fc*/ UMULL       R2, R3, R0, R0 
    /*00000700*/ MUL         R8, R0, R1 
    /*00000704*/ ADDS        SL, R2, R2 
    /*00000708*/ ADD         R3, R3, R8, LSL #1 
    /*0000070c*/ ADC         FP, R3, R3 
    /*00000710*/ MUL         R8, SL, R1 
    /*00000714*/ MLA         R8, R0, FP, R8 
    /*00000718*/ UMULL       SL, FP, SL, R0 
    /*0000071c*/ ADD         FP, R8, FP 
    /*00000720*/ LSR         R2, SL, #18 
    /*00000724*/ LSR         R3, FP, #18 
    /*00000728*/ ORR         R2, R2, FP, LSL #14 
    /*0000072c*/ CMP         R3, R5 
    /*00000730*/ CMPEQ       R2, R4 
    /*00000734*/ BLS         cmos_get_inttime_max_x6e8
    /*00000738*/ SUB         LR, R0, #1 
    /*0000073c*/ UBFX        R3, LR, #5, #26 
    /*00000740*/ MUL         LR, R3, LR 
    /*00000744*/ LSR         LR, LR, #6 
    cmos_get_inttime_max_x748:
    /*00000748*/ STR         R3, [IP, #4] 
    /*0000074c*/ MOV         R2, #2 
    /*00000750*/ STR         LR, [IP, #8] 
    /*00000754*/ MOV         R3, R2 
    /*00000758*/ STR         R2, [IP] 
    /*0000075c*/ LDR         R2, [R6, #2128]             @ 0x850 
    /*00000760*/ SUB         R2, R2, #2 
    /*00000764*/ STR         R2, [IP, #12] 
    /*00000768*/ B           cmos_get_inttime_max_xbc
    cmos_get_inttime_max_x76c:
    /*0000076c*/ LDR         R1, WORD_0908               @ LDR         R1, [PC, #404]              @ 0x0000000000000908 
    /*00000770*/ MOV         R3, #1088                   @ 0x440 
    /*00000774*/ LDR         R2, WORD_0954               @ LDR         R2, [PC, #472]              @ 0x0000000000000954 
    /*00000778*/ LDR         R0, [LR, R1] 
    /*0000077c*/ LDR         R1, WORD_0958               @ LDR         R1, [PC, #468]              @ 0x0000000000000958 
    /*00000780*/ ADD         R2, PC, R2 
    /*00000784*/ ADD         R1, PC, R1 
    /*00000788*/ B           cmos_get_inttime_max_x348
    cmos_get_inttime_max_x78c:
    /*0000078c*/ LDR         R0, [R6, #2128]             @ 0x850 
    /*00000790*/ MOV         R1, #64                     @ 0x40 
    /*00000794*/ SUB         R0, R0, #30 
    /*00000798*/ B           cmos_get_inttime_max_x7a8
    cmos_get_inttime_max_x79c:
    /*0000079c*/ ADD         R1, R1, #1 
    /*000007a0*/ CMP         R1, #4096                   @ 0x1000 
    /*000007a4*/ BEQ         cmos_get_inttime_max_x8dc
    cmos_get_inttime_max_x7a8:
    /*000007a8*/ LSL         R3, R1, #1 
    /*000007ac*/ MUL         R2, R1, R3 
    /*000007b0*/ LSR         R3, R3, #6 
    /*000007b4*/ ADD         R3, R3, #2 
    /*000007b8*/ ADD         R3, R3, R2, LSR #12 
    /*000007bc*/ CMP         R3, R0 
    /*000007c0*/ BLS         cmos_get_inttime_max_x79c
    /*000007c4*/ SUB         R1, R1, #1 
    /*000007c8*/ UBFX        R2, R1, #5, #26 
    /*000007cc*/ MUL         R1, R1, R2 
    /*000007d0*/ LSR         R3, R1, #6 
    cmos_get_inttime_max_x7d4:
    /*000007d4*/ STR         R2, [IP, #4] 
    /*000007d8*/ MOV         R2, #2 
    /*000007dc*/ STR         R3, [IP, #8] 
    /*000007e0*/ MOV         R3, R2 
    /*000007e4*/ STR         R2, [IP] 
    /*000007e8*/ B           cmos_get_inttime_max_xbc
    cmos_get_inttime_max_x7ec:
    /*000007ec*/ LDR         R4, [R6, #2128]             @ 0x850 
    /*000007f0*/ MOV         R0, #64                     @ 0x40 
    /*000007f4*/ MOV         R1, #0 
    /*000007f8*/ MOV         R5, #0 
    /*000007fc*/ SUB         R4, R4, #30 
    /*00000800*/ MOV         SL, R7 
    /*00000804*/ B           cmos_get_inttime_max_x81c
    cmos_get_inttime_max_x808:
    /*00000808*/ ADDS        R0, R0, #1 
    /*0000080c*/ ADC         R1, R1, #0 
    /*00000810*/ CMP         R1, #0 
    /*00000814*/ CMPEQ       R0, #4096                   @ 0x1000 
    /*00000818*/ BEQ         cmos_get_inttime_max_x8e8
    cmos_get_inttime_max_x81c:
    /*0000081c*/ UMULL       R8, R9, R0, R0 
    /*00000820*/ MUL         R2, R0, R1 
    /*00000824*/ ADDS        R6, R8, R8 
    /*00000828*/ MOV         LR, R0 
    /*0000082c*/ ADD         R9, R9, R2, LSL #1 
    /*00000830*/ MOV         R8, R6 
    /*00000834*/ LSL         R3, R0, #1 
    /*00000838*/ ADC         R7, R9, R9 
    /*0000083c*/ MOV         R9, R7 
    /*00000840*/ STRD        R8, [SP] 
    /*00000844*/ LSR         R7, R3, #6 
    /*00000848*/ MUL         R6, R3, R0 
    /*0000084c*/ MUL         R3, R8, R1 
    /*00000850*/ MLA         R9, R0, R9, R3 
    /*00000854*/ LDR         R3, [SP] 
    /*00000858*/ ADD         R7, R7, #2 
    /*0000085c*/ ADD         R6, R7, R6, LSR #12 
    /*00000860*/ UMULL       R2, R3, R3, R0 
    /*00000864*/ ADD         R3, R9, R3 
    /*00000868*/ LSR         R8, R2, #18 
    /*0000086c*/ ORR         R8, R8, R3, LSL #14 
    /*00000870*/ ADDS        R2, R8, R6 
    /*00000874*/ LSR         R9, R3, #18 
    /*00000878*/ ADC         R3, R9, #0 
    /*0000087c*/ CMP         R3, R5 
    /*00000880*/ CMPEQ       R2, R4 
    /*00000884*/ BLS         cmos_get_inttime_max_x808
    /*00000888*/ SUB         LR, R0, #1 
    /*0000088c*/ MOV         R7, SL 
    /*00000890*/ UBFX        R1, LR, #5, #26 
    /*00000894*/ MUL         R3, R1, LR 
    /*00000898*/ LSR         R3, R3, #6 
    /*0000089c*/ MUL         LR, R3, LR 
    /*000008a0*/ LSR         R2, LR, #6 
    cmos_get_inttime_max_x8a4:
    /*000008a4*/ STR         R1, [IP, #4] 
    /*000008a8*/ MOV         R1, #2 
    /*000008ac*/ STR         R3, [IP, #8] 
    /*000008b0*/ MOV         R3, R1 
    /*000008b4*/ STR         R2, [IP, #12] 
    /*000008b8*/ STR         R1, [IP] 
    /*000008bc*/ B           cmos_get_inttime_max_xbc
    cmos_get_inttime_max_x8c0:
    /*000008c0*/ MOV         R3, #2 
    /*000008c4*/ B           cmos_get_inttime_max_x158
    cmos_get_inttime_max_x8c8:
    /*000008c8*/ MOV         LR, #2 
    /*000008cc*/ MOV         R3, LR 
    /*000008d0*/ B           cmos_get_inttime_max_x748
    cmos_get_inttime_max_x8d4:
    /*000008d4*/ MOV         R3, #2 
    /*000008d8*/ B           cmos_get_inttime_max_x640
    cmos_get_inttime_max_x8dc:
    /*000008dc*/ MOV         R3, #2 
    /*000008e0*/ MOV         R2, R3 
    /*000008e4*/ B           cmos_get_inttime_max_x7d4
    cmos_get_inttime_max_x8e8:
    /*000008e8*/ MOV         R2, #2 
    /*000008ec*/ MOV         R7, SL 
    /*000008f0*/ MOV         R3, R2 
    /*000008f4*/ MOV         R1, R2 
    /*000008f8*/ B           cmos_get_inttime_max_x8a4
    /*000008fc*/ WORD_08fc: .word 0x000008e4
    /*00000900*/ WORD_0900: .word 0x00000000
    /*00000904*/ WORD_0904: .word 0x00000784
    /*00000908*/ WORD_0908: .word 0x00000000
    /*0000090c*/ WORD_090c: .word 0x000005c4
    /*00000910*/ WORD_0910: .word 0x000005c4
    /*00000914*/ WORD_0914: .word 0x00000588
    /*00000918*/ WORD_0918: .word 0x000004ec
    /*0000091c*/ WORD_091c: .word 0x000003f8
    /*00000920*/ WORD_0920: .word 0x000003ec
    /*00000924*/ WORD_0924: .word 0x000003b0
    /*00000928*/ WORD_0928: .word 0x0000038c
    /*0000092c*/ WORD_092c: .word 0x0000035c
    /*00000930*/ WORD_0930: .word 0x00000324
    /*00000934*/ WORD_0934: .word 0x000002c8
    /*00000938*/ WORD_0938: .word 0x000002c8
    /*0000093c*/ WORD_093c: .word 0x000002b0
    /*00000940*/ WORD_0940: .word 0x000002b0
    /*00000944*/ WORD_0944: .word 0x00000298
    /*00000948*/ WORD_0948: .word 0x00000298
    /*0000094c*/ WORD_094c: .word 0x00000280
    /*00000950*/ WORD_0950: .word 0x00000280
    /*00000954*/ WORD_0954: .word 0x000001cc
    /*00000958*/ WORD_0958: .word 0x000001cc
FUNC_END cmos_get_inttime_max


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_inttime_update
@ Size: 0x214
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_inttime_update
    /*00000000*/ LDR         IP, WORD_01ec               @ LDR         IP, [PC, #484]              @ 0x00000000000001ec 
    /*00000004*/ LDR         R3, WORD_01f0               @ LDR         R3, [PC, #484]              @ 0x00000000000001f0 
    /*00000008*/ ADD         IP, PC, IP 
    /*0000000c*/ LDR         R3, [IP, R3] 
    /*00000010*/ LDR         R3, [R3, R0, LSL #2] 
    /*00000014*/ CMP         R3, #0 
    /*00000018*/ BEQ         cmos_inttime_update_x1b0
    /*0000001c*/ LDR         R2, [R3, #12] 
    /*00000020*/ SUB         R0, R2, #4 
    /*00000024*/ CMP         R0, #1 
    /*00000028*/ BLS         cmos_inttime_update_x68
    /*0000002c*/ CMP         R2, #3 
    /*00000030*/ BEQ         cmos_inttime_update_xe8
    /*00000034*/ CMP         R2, #6 
    /*00000038*/ BEQ         cmos_inttime_update_xa0
    /*0000003c*/ CMP         R2, #9 
    /*00000040*/ BEQ         cmos_inttime_update_x128
    /*00000044*/ LDR         R2, WORD_01f4               @ LDR         R2, [PC, #424]              @ 0x00000000000001f4 
    /*00000048*/ MOV         IP, #1 
    /*0000004c*/ UBFX        R0, R1, #8, #8 
    /*00000050*/ UXTB        R1, R1 
    /*00000054*/ ADD         R2, PC, R2 
    /*00000058*/ STR         R0, [R3, #44]               @ 0x2c 
    /*0000005c*/ STR         IP, [R2] 
    /*00000060*/ STR         R1, [R3, #68]               @ 0x44 
    /*00000064*/ BX          LR 
    cmos_inttime_update_x68:
    /*00000068*/ LDR         R2, WORD_01f8               @ LDR         R2, [PC, #392]              @ 0x00000000000001f8 
    /*0000006c*/ ADD         R2, PC, R2 
    /*00000070*/ LDR         R0, [R2] 
    /*00000074*/ CMP         R0, #0 
    /*00000078*/ UBFX        R0, R1, #8, #8 
    /*0000007c*/ UXTB        R1, R1 
    /*00000080*/ STRNE       R0, [R3, #44]               @ 0x2c 
    /*00000084*/ MOVNE       R0, #0 
    /*00000088*/ STREQ       R0, [R3, #212]              @ 0xd4 
    /*0000008c*/ MOVEQ       R0, #1 
    /*00000090*/ STRNE       R1, [R3, #68]               @ 0x44 
    /*00000094*/ STREQ       R1, [R3, #236]              @ 0xec 
    /*00000098*/ STR         R0, [R2] 
    /*0000009c*/ BX          LR 
    cmos_inttime_update_xa0:
    /*000000a0*/ LDR         R2, WORD_01fc               @ LDR         R2, [PC, #340]              @ 0x00000000000001fc 
    /*000000a4*/ UBFX        IP, R1, #8, #8 
    /*000000a8*/ ADD         R2, PC, R2 
    /*000000ac*/ LDRB        R0, [R2] 
    /*000000b0*/ CMP         R0, #0 
    /*000000b4*/ BEQ         cmos_inttime_update_x17c
    /*000000b8*/ CMP         R0, #1 
    /*000000bc*/ UXTB        R0, R1 
    /*000000c0*/ STREQ       R1, [R3, #2144]             @ 0x860 
    /*000000c4*/ MOVEQ       R1, #2 
    /*000000c8*/ STRNE       R1, [R3, #2148]             @ 0x864 
    /*000000cc*/ MOVNE       R1, #0 
    /*000000d0*/ STREQ       IP, [R3, #212]              @ 0xd4 
    /*000000d4*/ STREQ       R0, [R3, #236]              @ 0xec 
    /*000000d8*/ STRNE       IP, [R3, #44]               @ 0x2c 
    /*000000dc*/ STRNE       R0, [R3, #68]               @ 0x44 
    /*000000e0*/ STRB        R1, [R2] 
    /*000000e4*/ BX          LR 
    cmos_inttime_update_xe8:
    /*000000e8*/ LDR         R2, WORD_0200               @ LDR         R2, [PC, #272]              @ 0x0000000000000200 
    /*000000ec*/ UBFX        IP, R1, #8, #8 
    /*000000f0*/ ADD         R2, PC, R2 
    /*000000f4*/ LDR         R0, [R2] 
    /*000000f8*/ CMP         R0, #0 
    /*000000fc*/ UXTB        R0, R1 
    /*00000100*/ STRNE       R1, [R3, #2140]             @ 0x85c 
    /*00000104*/ MOVNE       R1, #0 
    /*00000108*/ STREQ       R1, [R3, #2144]             @ 0x860 
    /*0000010c*/ MOVEQ       R1, #1 
    /*00000110*/ STRNE       IP, [R3, #212]              @ 0xd4 
    /*00000114*/ STRNE       R0, [R3, #236]              @ 0xec 
    /*00000118*/ STREQ       IP, [R3, #44]               @ 0x2c 
    /*0000011c*/ STREQ       R0, [R3, #68]               @ 0x44 
    /*00000120*/ STR         R1, [R2] 
    /*00000124*/ BX          LR 
    cmos_inttime_update_x128:
    /*00000128*/ LDR         R0, WORD_0204               @ LDR         R0, [PC, #212]              @ 0x0000000000000204 
    /*0000012c*/ UXTB        IP, R1 
    /*00000130*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000134*/ ADD         R0, PC, R0 
    /*00000138*/ UBFX        LR, R1, #8, #8 
    /*0000013c*/ LDRB        R2, [R0] 
    /*00000140*/ CMP         R2, #0 
    /*00000144*/ BEQ         cmos_inttime_update_x1d4
    /*00000148*/ CMP         R2, #1 
    /*0000014c*/ BEQ         cmos_inttime_update_x198
    /*00000150*/ CMP         R2, #2 
    /*00000154*/ STREQ       R1, [R3, #2148]             @ 0x864 
    /*00000158*/ MOVEQ       R2, #3 
    /*0000015c*/ STREQ       LR, [R3, #212]              @ 0xd4 
    /*00000160*/ MOVNE       R2, #0 
    /*00000164*/ STREQ       IP, [R3, #236]              @ 0xec 
    /*00000168*/ STRNE       R1, [R3, #2152]             @ 0x868 
    /*0000016c*/ STRNE       LR, [R3, #44]               @ 0x2c 
    /*00000170*/ STRNE       IP, [R3, #68]               @ 0x44 
    /*00000174*/ STRB        R2, [R0] 
    /*00000178*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    cmos_inttime_update_x17c:
    /*0000017c*/ STR         R1, [R3, #2140]             @ 0x85c 
    /*00000180*/ UXTB        R0, R1 
    /*00000184*/ STR         IP, [R3, #260]              @ 0x104 
    /*00000188*/ MOV         R1, #1 
    /*0000018c*/ STR         R0, [R3, #284]              @ 0x11c 
    /*00000190*/ STRB        R1, [R2] 
    /*00000194*/ BX          LR 
    cmos_inttime_update_x198:
    /*00000198*/ STR         R1, [R3, #2144]             @ 0x860 
    /*0000019c*/ MOV         R2, #2 
    /*000001a0*/ STR         LR, [R3, #260]              @ 0x104 
    /*000001a4*/ STR         IP, [R3, #284]              @ 0x11c 
    /*000001a8*/ STRB        R2, [R0] 
    /*000001ac*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    cmos_inttime_update_x1b0:
    /*000001b0*/ LDR         R1, WORD_0208               @ LDR         R1, [PC, #80]               @ 0x0000000000000208 
    /*000001b4*/ MOVW        R3, #709                    @ 0x2c5 
    /*000001b8*/ LDR         R2, WORD_020c               @ LDR         R2, [PC, #76]               @ 0x000000000000020c 
    /*000001bc*/ LDR         R0, [IP, R1] 
    /*000001c0*/ LDR         R1, WORD_0210               @ LDR         R1, [PC, #72]               @ 0x0000000000000210 
    /*000001c4*/ ADD         R2, PC, R2 
    /*000001c8*/ ADD         R1, PC, R1 
    /*000001cc*/ LDR         R0, [R0] 
    cmos_inttime_update_x1d0:
    /*000001d0*/ B           cmos_inttime_update_x1d0    @ fprintf [R_ARM_JUMP24: +0x0] 
    cmos_inttime_update_x1d4:
    /*000001d4*/ STR         R1, [R3, #2140]             @ 0x85c 
    /*000001d8*/ MOV         R2, #1 
    /*000001dc*/ STR         LR, [R3, #308]              @ 0x134 
    /*000001e0*/ STR         IP, [R3, #332]              @ 0x14c 
    /*000001e4*/ STRB        R2, [R0] 
    /*000001e8*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    /*000001ec*/ WORD_01ec: .word 0x000001dc
    /*000001f0*/ WORD_01f0: .word 0x00000000
    /*000001f4*/ WORD_01f4: .word 0x00000198
    /*000001f8*/ WORD_01f8: .word 0x00000184
    /*000001fc*/ WORD_01fc: .word 0x0000014c
    /*00000200*/ WORD_0200: .word 0x00000108
    /*00000204*/ WORD_0204: .word 0x000000c8
    /*00000208*/ WORD_0208: .word 0x00000000
    /*0000020c*/ WORD_020c: .word 0x00000040
    /*00000210*/ WORD_0210: .word 0x00000040
FUNC_END cmos_inttime_update


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_slow_framerate_set
@ Size: 0x118
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_slow_framerate_set
    /*00000000*/ LDR         IP, WORD_00fc               @ LDR         IP, [PC, #244]              @ 0x00000000000000fc 
    /*00000004*/ CMP         R2, #0 
    /*00000008*/ ADD         IP, PC, IP 
    /*0000000c*/ BEQ         cmos_slow_framerate_set_xb4
    /*00000010*/ LDR         R3, WORD_0100               @ LDR         R3, [PC, #232]              @ 0x0000000000000100 
    /*00000014*/ LDR         R3, [IP, R3] 
    /*00000018*/ LDR         R3, [R3, R0, LSL #2] 
    /*0000001c*/ CMP         R3, #0 
    /*00000020*/ BEQ         cmos_slow_framerate_set_xd8
    /*00000024*/ LDR         IP, [R3, #12] 
    /*00000028*/ MOVW        R0, #65535                  @ 0xffff 
    /*0000002c*/ CMP         R1, R0 
    /*00000030*/ MOVCS       R1, R0 
    /*00000034*/ CMP         IP, #0 
    /*00000038*/ STR         R1, [R3, #2128]             @ 0x850 
    /*0000003c*/ BNE         cmos_slow_framerate_set_x60
    /*00000040*/ LSR         IP, R1, #8 
    /*00000044*/ UXTB        R0, R1 
    /*00000048*/ STR         IP, [R3, #212]              @ 0xd4 
    /*0000004c*/ STR         R0, [R3, #236]              @ 0xec 
    cmos_slow_framerate_set_x50:
    /*00000050*/ STR         R1, [R2, #44]               @ 0x2c 
    /*00000054*/ SUB         R1, R1, #2 
    /*00000058*/ STR         R1, [R2, #48]               @ 0x30 
    /*0000005c*/ BX          LR 
    cmos_slow_framerate_set_x60:
    /*00000060*/ SUB         R0, IP, #3 
    /*00000064*/ CMP         R0, #2 
    /*00000068*/ BLS         cmos_slow_framerate_set_x8c
    /*0000006c*/ CMP         IP, #6 
    /*00000070*/ BEQ         cmos_slow_framerate_set_xa0
    /*00000074*/ CMP         IP, #9 
    /*00000078*/ LSREQ       IP, R1, #8 
    /*0000007c*/ UXTBEQ      R0, R1 
    /*00000080*/ STREQ       IP, [R3, #356]              @ 0x164 
    /*00000084*/ STREQ       R0, [R3, #380]              @ 0x17c 
    /*00000088*/ B           cmos_slow_framerate_set_x50
    cmos_slow_framerate_set_x8c:
    /*0000008c*/ LSR         IP, R1, #8 
    /*00000090*/ UXTB        R0, R1 
    /*00000094*/ STR         IP, [R3, #260]              @ 0x104 
    /*00000098*/ STR         R0, [R3, #284]              @ 0x11c 
    /*0000009c*/ B           cmos_slow_framerate_set_x50
    cmos_slow_framerate_set_xa0:
    /*000000a0*/ LSR         IP, R1, #8 
    /*000000a4*/ UXTB        R0, R1 
    /*000000a8*/ STR         IP, [R3, #308]              @ 0x134 
    /*000000ac*/ STR         R0, [R3, #332]              @ 0x14c 
    /*000000b0*/ B           cmos_slow_framerate_set_x50
    cmos_slow_framerate_set_xb4:
    /*000000b4*/ LDR         R1, WORD_0104               @ LDR         R1, [PC, #72]               @ 0x0000000000000104 
    /*000000b8*/ MOV         R3, #664                    @ 0x298 
    /*000000bc*/ LDR         R2, WORD_0108               @ LDR         R2, [PC, #68]               @ 0x0000000000000108 
    /*000000c0*/ LDR         R0, [IP, R1] 
    /*000000c4*/ LDR         R1, WORD_010c               @ LDR         R1, [PC, #64]               @ 0x000000000000010c 
    /*000000c8*/ ADD         R2, PC, R2 
    /*000000cc*/ ADD         R1, PC, R1 
    /*000000d0*/ LDR         R0, [R0] 
    cmos_slow_framerate_set_xd4:
    /*000000d4*/ B           cmos_slow_framerate_set_xd4 @ fprintf [R_ARM_JUMP24: +0x0] 
    cmos_slow_framerate_set_xd8:
    /*000000d8*/ LDR         R1, WORD_0104               @ LDR         R1, [PC, #36]               @ 0x0000000000000104 
    /*000000dc*/ MOVW        R3, #666                    @ 0x29a 
    /*000000e0*/ LDR         R2, WORD_0110               @ LDR         R2, [PC, #40]               @ 0x0000000000000110 
    /*000000e4*/ LDR         R0, [IP, R1] 
    /*000000e8*/ LDR         R1, WORD_0114               @ LDR         R1, [PC, #36]               @ 0x0000000000000114 
    /*000000ec*/ ADD         R2, PC, R2 
    /*000000f0*/ ADD         R1, PC, R1 
    /*000000f4*/ LDR         R0, [R0] 
    cmos_slow_framerate_set_xf8:
    /*000000f8*/ B           cmos_slow_framerate_set_xf8 @ fprintf [R_ARM_JUMP24: +0x0] 
    /*000000fc*/ WORD_00fc: .word 0x000000ec
    /*00000100*/ WORD_0100: .word 0x00000000
    /*00000104*/ WORD_0104: .word 0x00000000
    /*00000108*/ WORD_0108: .word 0x00000038
    /*0000010c*/ WORD_010c: .word 0x00000038
    /*00000110*/ WORD_0110: .word 0x0000001c
    /*00000114*/ WORD_0114: .word 0x0000001c
FUNC_END cmos_slow_framerate_set


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_fps_set
@ Size: 0x6e0
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_fps_set
    /*00000000*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*00000004*/ CMP         R2, #0 
    /*00000008*/ LDR         LR, WORD_0480               @ LDR         LR, [PC, #1136]             @ 0x0000000000000480 
    /*0000000c*/ SUB         SP, SP, #20 
    /*00000010*/ ADD         LR, PC, LR 
    /*00000014*/ BEQ         cmos_fps_set_x360
    /*00000018*/ LDR         R3, WORD_0484               @ LDR         R3, [PC, #1124]             @ 0x0000000000000484 
    /*0000001c*/ LDR         R3, [LR, R3] 
    /*00000020*/ LDR         R0, [R3, R0, LSL #2] 
    /*00000024*/ CMP         R0, #0 
    /*00000028*/ BEQ         cmos_fps_set_x38c
    /*0000002c*/ LDR         IP, [R0, #12] 
    /*00000030*/ VMOV        S15, R1 
    /*00000034*/ CMP         IP, #3 
    /*00000038*/ VMOV        S13, R1 
    /*0000003c*/ BEQ         cmos_fps_set_x228
    /*00000040*/ CMP         IP, #6 
    /*00000044*/ BEQ         cmos_fps_set_x25c
    /*00000048*/ CMP         IP, #9 
    /*0000004c*/ BEQ         cmos_fps_set_xe4
    /*00000050*/ LDRB        R1, [R0, #8] 
    /*00000054*/ CMP         R1, #2 
    /*00000058*/ BEQ         cmos_fps_set_x1ec
    /*0000005c*/ CMP         R1, #1 
    /*00000060*/ BEQ         cmos_fps_set_x290
    /*00000064*/ CMP         R1, #5 
    /*00000068*/ BEQ         cmos_fps_set_x194
    /*0000006c*/ CMP         R1, #3 
    /*00000070*/ BEQ         cmos_fps_set_x2f4
    /*00000074*/ CMP         R1, #4 
    /*00000078*/ BNE         cmos_fps_set_x55c
    /*0000007c*/ VLDR        S15, [PC, #972]             @ 0x0000000000000450 
    /*00000080*/ VCMPE.F32   S13, S15 
    /*00000084*/ VMRS        APSR_NZCV, FPSCR 
    /*00000088*/ BHI         cmos_fps_set_x4fc
    /*0000008c*/ VMOV.F32    S15, #4                     @ 0x40200000 2.5 
    /*00000090*/ VCMPE.F32   S13, S15 
    /*00000094*/ VMRS        APSR_NZCV, FPSCR 
    /*00000098*/ BLT         cmos_fps_set_x4fc
    /*0000009c*/ VLDR        S15, [PC, #944]             @ 0x0000000000000454 
    /*000000a0*/ MOVW        R3, #40685                  @ 0x9eed 
    /*000000a4*/ MOVT        R3, #16092                  @ 0x3edc 
    /*000000a8*/ VDIV.F32    S14, S15, S13 
    /*000000ac*/ STR         R3, [R2, #72]               @ 0x48 
    /*000000b0*/ VCVT.U32.F32S14, S14 
    cmos_fps_set_xb4:
    /*000000b4*/ VMOV        R1, S14 
    /*000000b8*/ MOVW        R3, #65535                  @ 0xffff 
    /*000000bc*/ CMP         R1, R3 
    /*000000c0*/ MOVCC       R3, R1 
    /*000000c4*/ CMP         IP, #0 
    /*000000c8*/ STR         R3, [R0, #2136]             @ 0x858 
    /*000000cc*/ BNE         cmos_fps_set_x13c
    /*000000d0*/ LSR         IP, R3, #8 
    /*000000d4*/ UXTB        R1, R3 
    /*000000d8*/ STR         IP, [R0, #212]              @ 0xd4 
    /*000000dc*/ STR         R1, [R0, #236]              @ 0xec 
    /*000000e0*/ B           cmos_fps_set_x158
    cmos_fps_set_xe4:
    /*000000e4*/ LDRB        R3, [R0, #8] 
    /*000000e8*/ CMP         R3, #2 
    /*000000ec*/ BEQ         cmos_fps_set_x2cc
    /*000000f0*/ VMOV.F32    S15, #46                    @ 0x41700000 15.0 
    /*000000f4*/ VCMPE.F32   S13, S15 
    /*000000f8*/ VMRS        APSR_NZCV, FPSCR 
    /*000000fc*/ BHI         cmos_fps_set_x420
    /*00000100*/ VMOV.F32    S15, #0                     @ 0x40000000 2.0 
    /*00000104*/ VCMPE.F32   S13, S15 
    /*00000108*/ VMRS        APSR_NZCV, FPSCR 
    /*0000010c*/ BLT         cmos_fps_set_x420
    /*00000110*/ VLDR        S15, [PC, #832]             @ 0x0000000000000458 
    cmos_fps_set_x114:
    /*00000114*/ VDIV.F32    S14, S15, S13 
    /*00000118*/ MOVW        R3, #37329                  @ 0x91d1 
    /*0000011c*/ MOVT        R3, #15836                  @ 0x3ddc 
    /*00000120*/ STR         R3, [R2, #72]               @ 0x48 
    /*00000124*/ VCVT.U32.F32S15, S14 
    /*00000128*/ VMOV        R3, S15 
    cmos_fps_set_x12c:
    /*0000012c*/ MOVW        R1, #65535                  @ 0xffff 
    /*00000130*/ CMP         R3, R1 
    /*00000134*/ MOVCS       R3, R1 
    /*00000138*/ STR         R3, [R0, #2136]             @ 0x858 
    cmos_fps_set_x13c:
    /*0000013c*/ SUB         R1, IP, #2 
    /*00000140*/ CMP         R1, #3 
    /*00000144*/ BHI         cmos_fps_set_x1cc
    /*00000148*/ LSR         IP, R3, #8 
    /*0000014c*/ UXTB        R1, R3 
    /*00000150*/ STR         IP, [R0, #260]              @ 0x104 
    /*00000154*/ STR         R1, [R0, #284]              @ 0x11c 
    cmos_fps_set_x158:
    /*00000158*/ VMOV        S15, R3 
    /*0000015c*/ VMOV.F32    S14, #96                    @ 0x3f000000 0.5 
    /*00000160*/ SUB         R1, R3, #2 
    /*00000164*/ VSTR        S13, [R2, #16] 
    /*00000168*/ STR         R3, [R2, #36]               @ 0x24 
    /*0000016c*/ VCVT.F32.S32S15, S15 
    /*00000170*/ STR         R1, [R2, #48]               @ 0x30 
    /*00000174*/ VMUL.F32    S15, S15, S13 
    /*00000178*/ VMUL.F32    S15, S15, S14 
    /*0000017c*/ VCVT.U32.F32S15, S15 
    /*00000180*/ VSTR        S15, [R2, #8] 
    /*00000184*/ STR         R3, [R0, #2128]             @ 0x850 
    /*00000188*/ STR         R3, [R2, #44]               @ 0x2c 
    cmos_fps_set_x18c:
    /*0000018c*/ ADD         SP, SP, #20 
    /*00000190*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    cmos_fps_set_x194:
    /*00000194*/ VMOV.F32    S15, #62                    @ 0x41f00000 30.0 
    /*00000198*/ VCMPE.F32   S13, S15 
    /*0000019c*/ VMRS        APSR_NZCV, FPSCR 
    /*000001a0*/ BHI         cmos_fps_set_x620
    /*000001a4*/ VMOV.F32    S15, #96                    @ 0x3f000000 0.5 
    /*000001a8*/ VCMPE.F32   S13, S15 
    /*000001ac*/ VMRS        APSR_NZCV, FPSCR 
    /*000001b0*/ BLT         cmos_fps_set_x620
    /*000001b4*/ VLDR        S15, [PC, #672]             @ 0x000000000000045c 
    /*000001b8*/ MOV         R3, #0 
    /*000001bc*/ STR         R3, [R2, #72]               @ 0x48 
    /*000001c0*/ VDIV.F32    S14, S15, S13 
    /*000001c4*/ VCVT.U32.F32S14, S14 
    /*000001c8*/ B           cmos_fps_set_xb4
    cmos_fps_set_x1cc:
    /*000001cc*/ CMP         IP, #6 
    /*000001d0*/ BEQ         cmos_fps_set_x3ac
    /*000001d4*/ CMP         IP, #9 
    /*000001d8*/ LSREQ       IP, R3, #8 
    /*000001dc*/ UXTBEQ      R1, R3 
    /*000001e0*/ STREQ       IP, [R0, #356]              @ 0x164 
    /*000001e4*/ STREQ       R1, [R0, #380]              @ 0x17c 
    /*000001e8*/ B           cmos_fps_set_x158
    cmos_fps_set_x1ec:
    /*000001ec*/ VMOV.F32    S15, #62                    @ 0x41f00000 30.0 
    /*000001f0*/ VCMPE.F32   S13, S15 
    /*000001f4*/ VMRS        APSR_NZCV, FPSCR 
    /*000001f8*/ BHI         cmos_fps_set_x5c0
    /*000001fc*/ VMOV.F32    S15, #112                   @ 0x3f800000 1.0 
    /*00000200*/ VCMPE.F32   S13, S15 
    /*00000204*/ VMRS        APSR_NZCV, FPSCR 
    /*00000208*/ BLT         cmos_fps_set_x5c0
    /*0000020c*/ VLDR        S15, [PC, #588]             @ 0x0000000000000460 
    /*00000210*/ MOVW        R3, #62390                  @ 0xf3b6 
    /*00000214*/ MOVT        R3, #16205                  @ 0x3f4d 
    /*00000218*/ VDIV.F32    S14, S15, S13 
    /*0000021c*/ STR         R3, [R2, #72]               @ 0x48 
    /*00000220*/ VCVT.U32.F32S14, S14 
    /*00000224*/ B           cmos_fps_set_xb4
    cmos_fps_set_x228:
    /*00000228*/ LDRB        R3, [R0, #8] 
    /*0000022c*/ VMOV.F32    S15, #62                    @ 0x41f00000 30.0 
    /*00000230*/ CMP         R3, #2 
    /*00000234*/ VCMPE.F32   S13, S15 
    /*00000238*/ BEQ         cmos_fps_set_x3c0
    /*0000023c*/ VMRS        APSR_NZCV, FPSCR 
    /*00000240*/ BHI         cmos_fps_set_x590
    /*00000244*/ VMOV.F32    S15, #0                     @ 0x40000000 2.0 
    /*00000248*/ VCMPE.F32   S13, S15 
    /*0000024c*/ VMRS        APSR_NZCV, FPSCR 
    /*00000250*/ BLT         cmos_fps_set_x590
    /*00000254*/ VLDR        S15, [PC, #512]             @ 0x000000000000045c 
    /*00000258*/ B           cmos_fps_set_x114
    cmos_fps_set_x25c:
    /*0000025c*/ LDRB        R3, [R0, #8] 
    /*00000260*/ CMP         R3, #2 
    /*00000264*/ BEQ         cmos_fps_set_x3f8
    /*00000268*/ VMOV.F32    S15, #52                    @ 0x41a00000 20.0 
    /*0000026c*/ VCMPE.F32   S13, S15 
    /*00000270*/ VMRS        APSR_NZCV, FPSCR 
    /*00000274*/ BHI         cmos_fps_set_x5f0
    /*00000278*/ VMOV.F32    S15, #0                     @ 0x40000000 2.0 
    /*0000027c*/ VCMPE.F32   S13, S15 
    /*00000280*/ VMRS        APSR_NZCV, FPSCR 
    /*00000284*/ BLT         cmos_fps_set_x5f0
    /*00000288*/ VLDR        S15, [PC, #468]             @ 0x0000000000000464 
    /*0000028c*/ B           cmos_fps_set_x114
    cmos_fps_set_x290:
    /*00000290*/ VLDR        S15, [PC, #464]             @ 0x0000000000000468 
    /*00000294*/ VCMPE.F32   S13, S15 
    /*00000298*/ VMRS        APSR_NZCV, FPSCR 
    /*0000029c*/ BHI         cmos_fps_set_x52c
    /*000002a0*/ VMOV.F32    S15, #120                   @ 0x3fc00000 1.5 
    /*000002a4*/ VCMPE.F32   S13, S15 
    /*000002a8*/ VMRS        APSR_NZCV, FPSCR 
    /*000002ac*/ BLT         cmos_fps_set_x52c
    /*000002b0*/ VLDR        S15, [PC, #436]             @ 0x000000000000046c 
    /*000002b4*/ MOVW        R3, #45823                  @ 0xb2ff 
    /*000002b8*/ MOVT        R3, #16155                  @ 0x3f1b 
    /*000002bc*/ VDIV.F32    S14, S15, S13 
    /*000002c0*/ STR         R3, [R2, #72]               @ 0x48 
    /*000002c4*/ VCVT.U32.F32S14, S14 
    /*000002c8*/ B           cmos_fps_set_xb4
    cmos_fps_set_x2cc:
    /*000002cc*/ VMOV.F32    S15, #62                    @ 0x41f00000 30.0 
    /*000002d0*/ VCMPE.F32   S13, S15 
    /*000002d4*/ VMRS        APSR_NZCV, FPSCR 
    /*000002d8*/ BHI         cmos_fps_set_x650
    /*000002dc*/ VMOV.F32    S15, #112                   @ 0x3f800000 1.0 
    /*000002e0*/ VCMPE.F32   S13, S15 
    /*000002e4*/ VMRS        APSR_NZCV, FPSCR 
    /*000002e8*/ BLT         cmos_fps_set_x650
    /*000002ec*/ VLDR        S15, [PC, #364]             @ 0x0000000000000460 
    /*000002f0*/ B           cmos_fps_set_x114
    cmos_fps_set_x2f4:
    /*000002f4*/ VLDR        S15, [PC, #372]             @ 0x0000000000000470 
    /*000002f8*/ VCMPE.F32   S13, S15 
    /*000002fc*/ VMRS        APSR_NZCV, FPSCR 
    /*00000300*/ BHI         cmos_fps_set_x330
    /*00000304*/ VMOV.F32    S15, #120                   @ 0x3fc00000 1.5 
    /*00000308*/ VCMPE.F32   S13, S15 
    /*0000030c*/ VMRS        APSR_NZCV, FPSCR 
    /*00000310*/ BLT         cmos_fps_set_x330
    /*00000314*/ VLDR        S15, [PC, #344]             @ 0x0000000000000474 
    /*00000318*/ MOVW        R3, #9123                   @ 0x23a3 
    /*0000031c*/ MOVT        R3, #16153                  @ 0x3f19 
    /*00000320*/ VDIV.F32    S14, S15, S13 
    /*00000324*/ STR         R3, [R2, #72]               @ 0x48 
    /*00000328*/ VCVT.U32.F32S14, S14 
    /*0000032c*/ B           cmos_fps_set_xb4
    cmos_fps_set_x330:
    /*00000330*/ LDR         R2, WORD_04f0               @ LDR         R2, [PC, #440]              @ 0x00000000000004f0 
    /*00000334*/ MOVW        R3, #546                    @ 0x222 
    /*00000338*/ VCVT.F64.F32D16, S13 
    /*0000033c*/ LDR         R1, [LR, R2] 
    /*00000340*/ LDR         R2, WORD_0488               @ LDR         R2, [PC, #320]              @ 0x0000000000000488 
    /*00000344*/ LDR         R0, [R1] 
    /*00000348*/ LDR         R1, WORD_048c               @ LDR         R1, [PC, #316]              @ 0x000000000000048c 
    /*0000034c*/ ADD         R2, PC, R2 
    /*00000350*/ VSTR        D16, [SP] 
    /*00000354*/ ADD         R1, PC, R1 
    /*00000358*/ BL          fprintf
    /*0000035c*/ B           cmos_fps_set_x18c
    cmos_fps_set_x360:
    /*00000360*/ LDR         R1, WORD_04f0               @ LDR         R1, [PC, #392]              @ 0x00000000000004f0 
    /*00000364*/ MOVW        R3, #582                    @ 0x246 
    /*00000368*/ LDR         R2, WORD_0490               @ LDR         R2, [PC, #288]              @ 0x0000000000000490 
    /*0000036c*/ LDR         R0, [LR, R1] 
    /*00000370*/ LDR         R1, WORD_0494               @ LDR         R1, [PC, #284]              @ 0x0000000000000494 
    /*00000374*/ ADD         R2, PC, R2 
    /*00000378*/ ADD         R1, PC, R1 
    cmos_fps_set_x37c:
    /*0000037c*/ LDR         R0, [R0] 
    /*00000380*/ ADD         SP, SP, #20 
    /*00000384*/ POP         {LR}                        @ (ldr lr, [sp], #4) 
    cmos_fps_set_x388:
    /*00000388*/ B           cmos_fps_set_x388           @ fprintf [R_ARM_JUMP24: +0x0] 
    cmos_fps_set_x38c:
    /*0000038c*/ LDR         R1, WORD_04f0               @ LDR         R1, [PC, #348]              @ 0x00000000000004f0 
    /*00000390*/ MOV         R3, #584                    @ 0x248 
    /*00000394*/ LDR         R2, WORD_0498               @ LDR         R2, [PC, #252]              @ 0x0000000000000498 
    /*00000398*/ LDR         R0, [LR, R1] 
    /*0000039c*/ LDR         R1, WORD_049c               @ LDR         R1, [PC, #248]              @ 0x000000000000049c 
    /*000003a0*/ ADD         R2, PC, R2 
    /*000003a4*/ ADD         R1, PC, R1 
    /*000003a8*/ B           cmos_fps_set_x37c
    cmos_fps_set_x3ac:
    /*000003ac*/ LSR         IP, R3, #8 
    /*000003b0*/ UXTB        R1, R3 
    /*000003b4*/ STR         IP, [R0, #308]              @ 0x134 
    /*000003b8*/ STR         R1, [R0, #332]              @ 0x14c 
    /*000003bc*/ B           cmos_fps_set_x158
    cmos_fps_set_x3c0:
    /*000003c0*/ VMRS        APSR_NZCV, FPSCR 
    /*000003c4*/ BHI         cmos_fps_set_x680
    /*000003c8*/ VMOV.F32    S15, #112                   @ 0x3f800000 1.0 
    /*000003cc*/ VCMPE.F32   S13, S15 
    /*000003d0*/ VMRS        APSR_NZCV, FPSCR 
    /*000003d4*/ BLT         cmos_fps_set_x680
    /*000003d8*/ VLDR        S15, [PC, #152]             @ 0x0000000000000478 
    /*000003dc*/ MOVW        R3, #18455                  @ 0x4817 
    /*000003e0*/ MOVT        R3, #16016                  @ 0x3e90 
    /*000003e4*/ VDIV.F32    S14, S15, S13 
    /*000003e8*/ STR         R3, [R2, #72]               @ 0x48 
    /*000003ec*/ VCVT.U32.F32S15, S14 
    /*000003f0*/ VMOV        R3, S15 
    /*000003f4*/ B           cmos_fps_set_x12c
    cmos_fps_set_x3f8:
    /*000003f8*/ VMOV.F32    S15, #62                    @ 0x41f00000 30.0 
    /*000003fc*/ VCMPE.F32   S13, S15 
    /*00000400*/ VMRS        APSR_NZCV, FPSCR 
    /*00000404*/ BHI         cmos_fps_set_x6b0
    /*00000408*/ VMOV.F32    S15, #112                   @ 0x3f800000 1.0 
    /*0000040c*/ VCMPE.F32   S13, S15 
    /*00000410*/ VMRS        APSR_NZCV, FPSCR 
    /*00000414*/ BLT         cmos_fps_set_x6b0
    /*00000418*/ VLDR        S15, [PC, #92]              @ 0x000000000000047c 
    /*0000041c*/ B           cmos_fps_set_x114
    cmos_fps_set_x420:
    /*00000420*/ LDR         R2, WORD_04f0               @ LDR         R2, [PC, #200]              @ 0x00000000000004f0 
    /*00000424*/ MOV         R3, #468                    @ 0x1d4 
    /*00000428*/ VCVT.F64.F32D16, S13 
    /*0000042c*/ LDR         R1, [LR, R2] 
    /*00000430*/ LDR         R2, WORD_04a0               @ LDR         R2, [PC, #104]              @ 0x00000000000004a0 
    /*00000434*/ LDR         R0, [R1] 
    /*00000438*/ LDR         R1, WORD_04a4               @ LDR         R1, [PC, #100]              @ 0x00000000000004a4 
    /*0000043c*/ ADD         R2, PC, R2 
    /*00000440*/ VSTR        D16, [SP] 
    /*00000444*/ ADD         R1, PC, R1 
    /*00000448*/ BL          fprintf
    /*0000044c*/ B           cmos_fps_set_x18c
    /*00000450*/ CMNMI       R0, #0 
    /*00000454*/ STMDAMI     R3, {SL, IP, SP, LR, PC} 
    /*00000458*/ LDRTMI      R0, [R0], -R0, LSL #8 
    /*0000045c*/ LDRTMI      R0, [R0], R0, LSL #8 
    /*00000460*/ STRMI       PC, [R3, -R0, LSL #8] 
    /*00000464*/ STRBTMI     FP, [SL], -R0 
    /*00000468*/ RSBSMI      R0, R0, #0 
    /*0000046c*/ STRMI       PC, [R3, R0, LSL #8] 
    /*00000470*/ RSCSMI      R0, R0, #0 
    /*00000474*/ LDRMI       R0, [R0, R0, LSL #8]! 
    /*00000478*/ LDRMI       R9, [R2, -R0, LSL #20] 
    /*0000047c*/ STRMI       LR, [PC, -R0, LSL #12]! 
    /*00000480*/ WORD_0480: .word 0x00000468
    /*00000484*/ WORD_0484: .word 0x00000000
    /*00000488*/ WORD_0488: .word 0x00000134
    /*0000048c*/ WORD_048c: .word 0x00000130
    /*00000490*/ WORD_0490: .word 0x00000114
    /*00000494*/ WORD_0494: .word 0x00000114
    /*00000498*/ WORD_0498: .word 0x000000f0
    /*0000049c*/ WORD_049c: .word 0x000000f0
    /*000004a0*/ WORD_04a0: .word 0x0000005c
    /*000004a4*/ WORD_04a4: .word 0x00000058
    /*000004a8*/ WORD_04a8: .word 0xffffff88
    /*000004ac*/ WORD_04ac: .word 0xffffff84
    /*000004b0*/ WORD_04b0: .word 0xffffff60
    /*000004b4*/ WORD_04b4: .word 0xffffff5c
    /*000004b8*/ WORD_04b8: .word 0xffffff34
    /*000004bc*/ WORD_04bc: .word 0xffffff34
    /*000004c0*/ WORD_04c0: .word 0xffffff0c
    /*000004c4*/ WORD_04c4: .word 0xffffff08
    /*000004c8*/ WORD_04c8: .word 0xfffffee4
    /*000004cc*/ WORD_04cc: .word 0xfffffee0
    /*000004d0*/ WORD_04d0: .word 0xfffffebc
    /*000004d4*/ WORD_04d4: .word 0xfffffeb8
    /*000004d8*/ WORD_04d8: .word 0xfffffe94
    /*000004dc*/ WORD_04dc: .word 0xfffffe90
    /*000004e0*/ WORD_04e0: .word 0xfffffe6c
    /*000004e4*/ WORD_04e4: .word 0xfffffe68
    /*000004e8*/ WORD_04e8: .word 0xfffffe44
    /*000004ec*/ WORD_04ec: .word 0xfffffe40
    /*000004f0*/ WORD_04f0: .word 0x00000000
    /*000004f4*/ WORD_04f4: .word 0xfffffe20
    /*000004f8*/ WORD_04f8: .word 0xfffffe1c
    cmos_fps_set_x4fc:
    /*000004fc*/ LDR         R2, WORD_04f0               @ LDR         R2, [PC, #-20]              @ 0x00000000000004f0 
    /*00000500*/ MOVW        R3, #562                    @ 0x232 
    /*00000504*/ VCVT.F64.F32D16, S13 
    /*00000508*/ LDR         R1, [LR, R2] 
    /*0000050c*/ LDR         R2, WORD_04a8               @ LDR         R2, [PC, #-108]             @ 0x00000000000004a8 
    /*00000510*/ LDR         R0, [R1] 
    /*00000514*/ LDR         R1, WORD_04ac               @ LDR         R1, [PC, #-112]             @ 0x00000000000004ac 
    /*00000518*/ ADD         R2, PC, R2 
    /*0000051c*/ VSTR        D16, [SP] 
    /*00000520*/ ADD         R1, PC, R1 
    /*00000524*/ BL          fprintf
    /*00000528*/ B           cmos_fps_set_x18c
    cmos_fps_set_x52c:
    /*0000052c*/ LDR         R2, WORD_04f0               @ LDR         R2, [PC, #-68]              @ 0x00000000000004f0 
    /*00000530*/ MOVW        R3, #514                    @ 0x202 
    /*00000534*/ VCVT.F64.F32D16, S13 
    /*00000538*/ LDR         R1, [LR, R2] 
    /*0000053c*/ LDR         R2, WORD_04b0               @ LDR         R2, [PC, #-148]             @ 0x00000000000004b0 
    /*00000540*/ LDR         R0, [R1] 
    /*00000544*/ LDR         R1, WORD_04b4               @ LDR         R1, [PC, #-152]             @ 0x00000000000004b4 
    /*00000548*/ ADD         R2, PC, R2 
    /*0000054c*/ VSTR        D16, [SP] 
    /*00000550*/ ADD         R1, PC, R1 
    /*00000554*/ BL          fprintf
    /*00000558*/ B           cmos_fps_set_x18c
    cmos_fps_set_x55c:
    /*0000055c*/ LDR         R2, WORD_04f0               @ LDR         R2, [PC, #-116]             @ 0x00000000000004f0 
    /*00000560*/ MOV         R3, #568                    @ 0x238 
    /*00000564*/ VCVT.F64.F32D16, S15 
    /*00000568*/ LDR         R0, [LR, R2] 
    /*0000056c*/ LDR         R2, WORD_04b8               @ LDR         R2, [PC, #-188]             @ 0x00000000000004b8 
    /*00000570*/ LDR         R0, [R0] 
    /*00000574*/ STR         R1, [SP] 
    /*00000578*/ LDR         R1, WORD_04bc               @ LDR         R1, [PC, #-196]             @ 0x00000000000004bc 
    /*0000057c*/ ADD         R2, PC, R2 
    /*00000580*/ ADD         R1, PC, R1 
    /*00000584*/ VSTR        D16, [SP, #8] 
    /*00000588*/ BL          fprintf
    /*0000058c*/ B           cmos_fps_set_x18c
    cmos_fps_set_x590:
    /*00000590*/ LDR         R2, WORD_04f0               @ LDR         R2, [PC, #-168]             @ 0x00000000000004f0 
    /*00000594*/ MOVW        R3, #386                    @ 0x182 
    /*00000598*/ VCVT.F64.F32D16, S13 
    /*0000059c*/ LDR         R1, [LR, R2] 
    /*000005a0*/ LDR         R2, WORD_04c0               @ LDR         R2, [PC, #-232]             @ 0x00000000000004c0 
    /*000005a4*/ LDR         R0, [R1] 
    /*000005a8*/ LDR         R1, WORD_04c4               @ LDR         R1, [PC, #-236]             @ 0x00000000000004c4 
    /*000005ac*/ ADD         R2, PC, R2 
    /*000005b0*/ VSTR        D16, [SP] 
    /*000005b4*/ ADD         R1, PC, R1 
    /*000005b8*/ BL          fprintf
    /*000005bc*/ B           cmos_fps_set_x18c
    cmos_fps_set_x5c0:
    /*000005c0*/ LDR         R2, WORD_04f0               @ LDR         R2, [PC, #-216]             @ 0x00000000000004f0 
    /*000005c4*/ MOVW        R3, #498                    @ 0x1f2 
    /*000005c8*/ VCVT.F64.F32D16, S13 
    /*000005cc*/ LDR         R1, [LR, R2] 
    /*000005d0*/ LDR         R2, WORD_04c8               @ LDR         R2, [PC, #-272]             @ 0x00000000000004c8 
    /*000005d4*/ LDR         R0, [R1] 
    /*000005d8*/ LDR         R1, WORD_04cc               @ LDR         R1, [PC, #-276]             @ 0x00000000000004cc 
    /*000005dc*/ ADD         R2, PC, R2 
    /*000005e0*/ VSTR        D16, [SP] 
    /*000005e4*/ ADD         R1, PC, R1 
    /*000005e8*/ BL          fprintf
    /*000005ec*/ B           cmos_fps_set_x18c
    cmos_fps_set_x5f0:
    /*000005f0*/ LDR         R2, WORD_04f0               @ LDR         R2, [PC, #-264]             @ 0x00000000000004f0 
    /*000005f4*/ MOVW        R3, #427                    @ 0x1ab 
    /*000005f8*/ VCVT.F64.F32D16, S13 
    /*000005fc*/ LDR         R1, [LR, R2] 
    /*00000600*/ LDR         R2, WORD_04d0               @ LDR         R2, [PC, #-312]             @ 0x00000000000004d0 
    /*00000604*/ LDR         R0, [R1] 
    /*00000608*/ LDR         R1, WORD_04d4               @ LDR         R1, [PC, #-316]             @ 0x00000000000004d4 
    /*0000060c*/ ADD         R2, PC, R2 
    /*00000610*/ VSTR        D16, [SP] 
    /*00000614*/ ADD         R1, PC, R1 
    /*00000618*/ BL          fprintf
    /*0000061c*/ B           cmos_fps_set_x18c
    cmos_fps_set_x620:
    /*00000620*/ LDR         R2, WORD_04f0               @ LDR         R2, [PC, #-312]             @ 0x00000000000004f0 
    /*00000624*/ MOVW        R3, #530                    @ 0x212 
    /*00000628*/ VCVT.F64.F32D16, S13 
    /*0000062c*/ LDR         R1, [LR, R2] 
    /*00000630*/ LDR         R2, WORD_04d8               @ LDR         R2, [PC, #-352]             @ 0x00000000000004d8 
    /*00000634*/ LDR         R0, [R1] 
    /*00000638*/ LDR         R1, WORD_04dc               @ LDR         R1, [PC, #-356]             @ 0x00000000000004dc 
    /*0000063c*/ ADD         R2, PC, R2 
    /*00000640*/ VSTR        D16, [SP] 
    /*00000644*/ ADD         R1, PC, R1 
    /*00000648*/ BL          fprintf
    /*0000064c*/ B           cmos_fps_set_x18c
    cmos_fps_set_x650:
    /*00000650*/ LDR         R2, WORD_04f0               @ LDR         R2, [PC, #-360]             @ 0x00000000000004f0 
    /*00000654*/ MOVW        R3, #453                    @ 0x1c5 
    /*00000658*/ VCVT.F64.F32D16, S13 
    /*0000065c*/ LDR         R1, [LR, R2] 
    /*00000660*/ LDR         R2, WORD_04e0               @ LDR         R2, [PC, #-392]             @ 0x00000000000004e0 
    /*00000664*/ LDR         R0, [R1] 
    /*00000668*/ LDR         R1, WORD_04e4               @ LDR         R1, [PC, #-396]             @ 0x00000000000004e4 
    /*0000066c*/ ADD         R2, PC, R2 
    /*00000670*/ VSTR        D16, [SP] 
    /*00000674*/ ADD         R1, PC, R1 
    /*00000678*/ BL          fprintf
    /*0000067c*/ B           cmos_fps_set_x18c
    cmos_fps_set_x680:
    /*00000680*/ LDR         R2, WORD_04f0               @ LDR         R2, [PC, #-408]             @ 0x00000000000004f0 
    /*00000684*/ MOVW        R3, #371                    @ 0x173 
    /*00000688*/ VCVT.F64.F32D16, S13 
    /*0000068c*/ LDR         R1, [LR, R2] 
    /*00000690*/ LDR         R2, WORD_04e8               @ LDR         R2, [PC, #-432]             @ 0x00000000000004e8 
    /*00000694*/ LDR         R0, [R1] 
    /*00000698*/ LDR         R1, WORD_04ec               @ LDR         R1, [PC, #-436]             @ 0x00000000000004ec 
    /*0000069c*/ ADD         R2, PC, R2 
    /*000006a0*/ VSTR        D16, [SP] 
    /*000006a4*/ ADD         R1, PC, R1 
    /*000006a8*/ BL          fprintf
    /*000006ac*/ B           cmos_fps_set_x18c
    cmos_fps_set_x6b0:
    /*000006b0*/ LDR         R2, WORD_04f0               @ LDR         R2, [PC, #-456]             @ 0x00000000000004f0 
    /*000006b4*/ MOV         R3, #412                    @ 0x19c 
    /*000006b8*/ VCVT.F64.F32D16, S13 
    /*000006bc*/ LDR         R1, [LR, R2] 
    /*000006c0*/ LDR         R2, WORD_04f4               @ LDR         R2, [PC, #-468]             @ 0x00000000000004f4 
    /*000006c4*/ LDR         R0, [R1] 
    /*000006c8*/ LDR         R1, WORD_04f8               @ LDR         R1, [PC, #-472]             @ 0x00000000000004f8 
    /*000006cc*/ ADD         R2, PC, R2 
    /*000006d0*/ VSTR        D16, [SP] 
    /*000006d4*/ ADD         R1, PC, R1 
    /*000006d8*/ BL          fprintf
    /*000006dc*/ B           cmos_fps_set_x18c
FUNC_END cmos_fps_set


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_set_image_mode
@ Size: 0x52c
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_set_image_mode
    /*00000000*/ LDR         IP, WORD_0480               @ LDR         IP, [PC, #1144]             @ 0x0000000000000480 
    /*00000004*/ CMP         R1, #0 
    /*00000008*/ PUSH        {R4, R5, R6, LR} 
    /*0000000c*/ SUB         SP, SP, #24 
    /*00000010*/ ADD         IP, PC, IP 
    /*00000014*/ BEQ         cmos_set_image_mode_x37c
    /*00000018*/ LDR         R3, WORD_0484               @ LDR         R3, [PC, #1124]             @ 0x0000000000000484 
    /*0000001c*/ LDR         R3, [IP, R3] 
    /*00000020*/ LDR         R3, [R3, R0, LSL #2] 
    /*00000024*/ CMP         R3, #0 
    /*00000028*/ BEQ         cmos_set_image_mode_x34c
    /*0000002c*/ LDRH        LR, [R1] 
    /*00000030*/ MOV         R2, #0 
    /*00000034*/ STR         R2, [R3, #4] 
    /*00000038*/ LDRB        R6, [R3, #8] 
    /*0000003c*/ CMP         LR, #640                    @ 0x280 
    /*00000040*/ LDRH        R4, [R1, #2] 
    /*00000044*/ BLS         cmos_set_image_mode_xf0
    /*00000048*/ CMP         LR, #1280                   @ 0x500 
    /*0000004c*/ BLS         cmos_set_image_mode_xf8
    /*00000050*/ CMP         LR, #1920                   @ 0x780 
    /*00000054*/ BHI         cmos_set_image_mode_x1e0
    cmos_set_image_mode_x58:
    /*00000058*/ MOVW        R2, #1080                   @ 0x438 
    /*0000005c*/ CMP         R4, R2 
    /*00000060*/ BHI         cmos_set_image_mode_x1e0
    /*00000064*/ LDR         R5, [R3, #12] 
    /*00000068*/ VMOV.F32    S14, #62                    @ 0x41f00000 30.0 
    /*0000006c*/ CMP         R5, #6 
    /*00000070*/ CMPNE       R5, #3 
    /*00000074*/ VLDR        S15, [R1, #4] 
    /*00000078*/ MOVEQ       R2, #1 
    /*0000007c*/ MOVNE       R2, #0 
    /*00000080*/ VCMPE.F32   S15, S14 
    /*00000084*/ CMP         R5, #9 
    /*00000088*/ ORREQ       R2, R2, #1 
    /*0000008c*/ CMP         R2, #0 
    /*00000090*/ BNE         cmos_set_image_mode_x138
    /*00000094*/ VMRS        APSR_NZCV, FPSCR 
    /*00000098*/ BLS         cmos_set_image_mode_x140
    /*0000009c*/ VLDR        S14, [PC, #976]             @ 0x0000000000000474 
    /*000000a0*/ VCMPE.F32   S15, S14 
    /*000000a4*/ VMRS        APSR_NZCV, FPSCR 
    /*000000a8*/ MOVLS       R2, #1 
    /*000000ac*/ BLS         cmos_set_image_mode_x144
    /*000000b0*/ LDR         R2, WORD_04e0               @ LDR         R2, [PC, #1064]             @ 0x00000000000004e0 
    /*000000b4*/ MOVW        R3, #1969                   @ 0x7b1 
    /*000000b8*/ VCVT.F64.F32D16, S15 
    /*000000bc*/ LDR         R1, [IP, R2] 
    /*000000c0*/ LDR         R2, WORD_0488               @ LDR         R2, [PC, #960]              @ 0x0000000000000488 
    /*000000c4*/ LDR         R0, [R1] 
    /*000000c8*/ LDR         R1, WORD_048c               @ LDR         R1, [PC, #956]              @ 0x000000000000048c 
    /*000000cc*/ ADD         R2, PC, R2 
    /*000000d0*/ STR         R5, [SP, #16] 
    /*000000d4*/ STR         R4, [SP, #4] 
    /*000000d8*/ STR         LR, [SP] 
    /*000000dc*/ VSTR        D16, [SP, #8] 
    /*000000e0*/ ADD         R1, PC, R1 
    /*000000e4*/ BL          fprintf
    /*000000e8*/ MVN         R0, #0 
    /*000000ec*/ B           cmos_set_image_mode_x174
    cmos_set_image_mode_xf0:
    /*000000f0*/ CMP         R4, #480                    @ 0x1e0 
    /*000000f4*/ BLS         cmos_set_image_mode_x17c
    cmos_set_image_mode_xf8:
    /*000000f8*/ CMP         R4, #720                    @ 0x2d0 
    /*000000fc*/ BHI         cmos_set_image_mode_x58
    /*00000100*/ LDR         R5, [R3, #12] 
    /*00000104*/ CMP         R5, #0 
    /*00000108*/ BNE         cmos_set_image_mode_x228
    /*0000010c*/ VLDR        S15, [R1, #4] 
    /*00000110*/ VLDR        S14, [PC, #864]             @ 0x0000000000000478 
    /*00000114*/ VCMPE.F32   S15, S14 
    /*00000118*/ VMRS        APSR_NZCV, FPSCR 
    /*0000011c*/ BHI         cmos_set_image_mode_x434
    /*00000120*/ VMOV.F32    S14, #62                    @ 0x41f00000 30.0 
    /*00000124*/ VCMPE.F32   S15, S14 
    /*00000128*/ VMRS        APSR_NZCV, FPSCR 
    /*0000012c*/ MOVLS       R2, #5 
    /*00000130*/ MOVHI       R2, #3 
    /*00000134*/ B           cmos_set_image_mode_x144
    cmos_set_image_mode_x138:
    /*00000138*/ VMRS        APSR_NZCV, FPSCR 
    /*0000013c*/ BHI         cmos_set_image_mode_x2b0
    cmos_set_image_mode_x140:
    /*00000140*/ MOV         R2, #2 
    cmos_set_image_mode_x144:
    /*00000144*/ LDR         R0, [R3] 
    /*00000148*/ CMP         R0, #0 
    /*0000014c*/ STRBEQ      R2, [R3, #8] 
    /*00000150*/ BEQ         cmos_set_image_mode_x174
    /*00000154*/ CMP         R6, R2 
    /*00000158*/ BEQ         cmos_set_image_mode_x2f0
    /*0000015c*/ VMOV.I32    Q8, #0                      @ 0x00000000 
    /*00000160*/ STRB        R2, [R3, #8] 
    /*00000164*/ ADD         R3, R3, #2128               @ 0x850 
    /*00000168*/ ADD         R3, R3, #12 
    /*0000016c*/ MOV         R0, #0 
    /*00000170*/ VST1.8      {D16-D17}, [R3] 
    cmos_set_image_mode_x174:
    /*00000174*/ ADD         SP, SP, #24 
    /*00000178*/ POP         {R4, R5, R6, PC} 
    cmos_set_image_mode_x17c:
    /*0000017c*/ LDR         R5, [R3, #12] 
    /*00000180*/ CMP         R5, R2 
    /*00000184*/ BNE         cmos_set_image_mode_x3ac
    /*00000188*/ VLDR        S15, [R1, #4] 
    /*0000018c*/ VLDR        S14, [PC, #744]             @ 0x000000000000047c 
    /*00000190*/ VCMPE.F32   S15, S14 
    /*00000194*/ VMRS        APSR_NZCV, FPSCR 
    /*00000198*/ MOVLS       R2, #4 
    /*0000019c*/ BLS         cmos_set_image_mode_x144
    /*000001a0*/ LDR         R2, WORD_04e0               @ LDR         R2, [PC, #824]              @ 0x00000000000004e0 
    /*000001a4*/ MOVW        R3, #1818                   @ 0x71a 
    /*000001a8*/ VCVT.F64.F32D16, S15 
    /*000001ac*/ LDR         R1, [IP, R2] 
    /*000001b0*/ LDR         R2, WORD_0490               @ LDR         R2, [PC, #728]              @ 0x0000000000000490 
    /*000001b4*/ LDR         R0, [R1] 
    /*000001b8*/ LDR         R1, WORD_0494               @ LDR         R1, [PC, #724]              @ 0x0000000000000494 
    /*000001bc*/ ADD         R2, PC, R2 
    /*000001c0*/ STR         R5, [SP, #16] 
    /*000001c4*/ STR         R4, [SP, #4] 
    /*000001c8*/ STR         LR, [SP] 
    /*000001cc*/ VSTR        D16, [SP, #8] 
    /*000001d0*/ ADD         R1, PC, R1 
    /*000001d4*/ BL          fprintf
    /*000001d8*/ MVN         R0, #0 
    /*000001dc*/ B           cmos_set_image_mode_x174
    cmos_set_image_mode_x1e0:
    /*000001e0*/ LDR         R5, [R3, #12] 
    /*000001e4*/ MOVW        R3, #2021                   @ 0x7e5 
    /*000001e8*/ LDR         R0, WORD_04e0               @ LDR         R0, [PC, #752]              @ 0x00000000000004e0 
    /*000001ec*/ LDR         R2, WORD_0498               @ LDR         R2, [PC, #676]              @ 0x0000000000000498 
    /*000001f0*/ LDR         R0, [IP, R0] 
    /*000001f4*/ STR         R5, [SP, #16] 
    /*000001f8*/ VLDR        S15, [R1, #4] 
    /*000001fc*/ LDR         R1, WORD_049c               @ LDR         R1, [PC, #664]              @ 0x000000000000049c 
    /*00000200*/ VCVT.F64.F32D16, S15 
    /*00000204*/ LDR         R0, [R0] 
    /*00000208*/ ADD         R2, PC, R2 
    /*0000020c*/ STR         R4, [SP, #4] 
    /*00000210*/ STR         LR, [SP] 
    /*00000214*/ ADD         R1, PC, R1 
    /*00000218*/ VSTR        D16, [SP, #8] 
    /*0000021c*/ BL          fprintf
    /*00000220*/ MVN         R0, #0 
    /*00000224*/ B           cmos_set_image_mode_x174
    cmos_set_image_mode_x228:
    /*00000228*/ CMP         R5, #3 
    /*0000022c*/ BEQ         cmos_set_image_mode_x25c
    /*00000230*/ CMP         R5, #6 
    /*00000234*/ BEQ         cmos_set_image_mode_x2f8
    /*00000238*/ CMP         R5, #9 
    /*0000023c*/ BNE         cmos_set_image_mode_x3f0
    /*00000240*/ VMOV.F32    S14, #46                    @ 0x41700000 15.0 
    /*00000244*/ VLDR        S15, [R1, #4] 
    /*00000248*/ VCMPE.F32   S15, S14 
    /*0000024c*/ VMRS        APSR_NZCV, FPSCR 
    /*00000250*/ BHI         cmos_set_image_mode_x4ec
    cmos_set_image_mode_x254:
    /*00000254*/ MOV         R2, #5 
    /*00000258*/ B           cmos_set_image_mode_x144
    cmos_set_image_mode_x25c:
    /*0000025c*/ VMOV.F32    S14, #62                    @ 0x41f00000 30.0 
    /*00000260*/ VLDR        S15, [R1, #4] 
    /*00000264*/ VCMPE.F32   S15, S14 
    /*00000268*/ VMRS        APSR_NZCV, FPSCR 
    /*0000026c*/ BLS         cmos_set_image_mode_x254
    /*00000270*/ LDR         R2, WORD_04e0               @ LDR         R2, [PC, #616]              @ 0x00000000000004e0 
    /*00000274*/ MOVW        R3, #1879                   @ 0x757 
    /*00000278*/ VCVT.F64.F32D16, S15 
    /*0000027c*/ LDR         R1, [IP, R2] 
    /*00000280*/ LDR         R2, WORD_04a0               @ LDR         R2, [PC, #536]              @ 0x00000000000004a0 
    /*00000284*/ LDR         R0, [R1] 
    /*00000288*/ LDR         R1, WORD_04a4               @ LDR         R1, [PC, #532]              @ 0x00000000000004a4 
    /*0000028c*/ ADD         R2, PC, R2 
    /*00000290*/ STR         R6, [SP, #16] 
    /*00000294*/ STR         R4, [SP, #4] 
    /*00000298*/ STR         LR, [SP] 
    /*0000029c*/ VSTR        D16, [SP, #8] 
    /*000002a0*/ ADD         R1, PC, R1 
    /*000002a4*/ BL          fprintf
    /*000002a8*/ MVN         R0, #0 
    /*000002ac*/ B           cmos_set_image_mode_x174
    cmos_set_image_mode_x2b0:
    /*000002b0*/ LDR         R2, WORD_04e0               @ LDR         R2, [PC, #552]              @ 0x00000000000004e0 
    /*000002b4*/ MOVW        R3, #1949                   @ 0x79d 
    /*000002b8*/ VCVT.F64.F32D16, S15 
    /*000002bc*/ LDR         R1, [IP, R2] 
    /*000002c0*/ LDR         R2, WORD_04a8               @ LDR         R2, [PC, #480]              @ 0x00000000000004a8 
    /*000002c4*/ LDR         R0, [R1] 
    /*000002c8*/ LDR         R1, WORD_04ac               @ LDR         R1, [PC, #476]              @ 0x00000000000004ac 
    /*000002cc*/ ADD         R2, PC, R2 
    /*000002d0*/ STR         R5, [SP, #16] 
    /*000002d4*/ STR         R4, [SP, #4] 
    /*000002d8*/ STR         LR, [SP] 
    /*000002dc*/ VSTR        D16, [SP, #8] 
    /*000002e0*/ ADD         R1, PC, R1 
    /*000002e4*/ BL          fprintf
    /*000002e8*/ MVN         R0, #0 
    /*000002ec*/ B           cmos_set_image_mode_x174
    cmos_set_image_mode_x2f0:
    /*000002f0*/ MVN         R0, #0 
    /*000002f4*/ B           cmos_set_image_mode_x174
    cmos_set_image_mode_x2f8:
    /*000002f8*/ VMOV.F32    S14, #52                    @ 0x41a00000 20.0 
    /*000002fc*/ VLDR        S15, [R1, #4] 
    /*00000300*/ VCMPE.F32   S15, S14 
    /*00000304*/ VMRS        APSR_NZCV, FPSCR 
    /*00000308*/ BLS         cmos_set_image_mode_x254
    /*0000030c*/ LDR         R2, WORD_04e0               @ LDR         R2, [PC, #460]              @ 0x00000000000004e0 
    /*00000310*/ MOVW        R3, #1895                   @ 0x767 
    /*00000314*/ VCVT.F64.F32D16, S15 
    /*00000318*/ LDR         R1, [IP, R2] 
    /*0000031c*/ LDR         R2, WORD_04b0               @ LDR         R2, [PC, #396]              @ 0x00000000000004b0 
    /*00000320*/ LDR         R0, [R1] 
    /*00000324*/ LDR         R1, WORD_04b4               @ LDR         R1, [PC, #392]              @ 0x00000000000004b4 
    /*00000328*/ ADD         R2, PC, R2 
    /*0000032c*/ STR         R5, [SP, #16] 
    /*00000330*/ STR         R4, [SP, #4] 
    /*00000334*/ STR         LR, [SP] 
    /*00000338*/ VSTR        D16, [SP, #8] 
    /*0000033c*/ ADD         R1, PC, R1 
    /*00000340*/ BL          fprintf
    /*00000344*/ MVN         R0, #0 
    /*00000348*/ B           cmos_set_image_mode_x174
    cmos_set_image_mode_x34c:
    /*0000034c*/ LDR         R1, WORD_04e0               @ LDR         R1, [PC, #396]              @ 0x00000000000004e0 
    /*00000350*/ MOVW        R3, #1986                   @ 0x7c2 
    /*00000354*/ LDR         R2, WORD_04b8               @ LDR         R2, [PC, #348]              @ 0x00000000000004b8 
    /*00000358*/ LDR         R0, [IP, R1] 
    /*0000035c*/ LDR         R1, WORD_04bc               @ LDR         R1, [PC, #344]              @ 0x00000000000004bc 
    /*00000360*/ ADD         R2, PC, R2 
    /*00000364*/ ADD         R1, PC, R1 
    /*00000368*/ LDR         R0, [R0] 
    /*0000036c*/ BL          fprintf
    /*00000370*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000374*/ MOVT        R0, #40988                  @ 0xa01c 
    /*00000378*/ B           cmos_set_image_mode_x174
    cmos_set_image_mode_x37c:
    /*0000037c*/ LDR         R1, WORD_04e0               @ LDR         R1, [PC, #348]              @ 0x00000000000004e0 
    /*00000380*/ MOV         R3, #1984                   @ 0x7c0 
    /*00000384*/ LDR         R2, WORD_04c0               @ LDR         R2, [PC, #308]              @ 0x00000000000004c0 
    /*00000388*/ LDR         R0, [IP, R1] 
    /*0000038c*/ LDR         R1, WORD_04c4               @ LDR         R1, [PC, #304]              @ 0x00000000000004c4 
    /*00000390*/ ADD         R2, PC, R2 
    /*00000394*/ ADD         R1, PC, R1 
    /*00000398*/ LDR         R0, [R0] 
    /*0000039c*/ BL          fprintf
    /*000003a0*/ MOVW        R0, #32774                  @ 0x8006 
    /*000003a4*/ MOVT        R0, #40988                  @ 0xa01c 
    /*000003a8*/ B           cmos_set_image_mode_x174
    cmos_set_image_mode_x3ac:
    /*000003ac*/ LDR         R0, WORD_04e0               @ LDR         R0, [PC, #300]              @ 0x00000000000004e0 
    /*000003b0*/ MOVW        R3, #1828                   @ 0x724 
    /*000003b4*/ LDR         R2, WORD_04c8               @ LDR         R2, [PC, #268]              @ 0x00000000000004c8 
    /*000003b8*/ LDR         R0, [IP, R0] 
    /*000003bc*/ STR         R5, [SP, #16] 
    /*000003c0*/ VLDR        S15, [R1, #4] 
    /*000003c4*/ LDR         R1, WORD_04cc               @ LDR         R1, [PC, #256]              @ 0x00000000000004cc 
    /*000003c8*/ VCVT.F64.F32D16, S15 
    /*000003cc*/ LDR         R0, [R0] 
    /*000003d0*/ ADD         R2, PC, R2 
    /*000003d4*/ STR         R4, [SP, #4] 
    /*000003d8*/ STR         LR, [SP] 
    /*000003dc*/ ADD         R1, PC, R1 
    /*000003e0*/ VSTR        D16, [SP, #8] 
    /*000003e4*/ BL          fprintf
    /*000003e8*/ MVN         R0, #0 
    /*000003ec*/ B           cmos_set_image_mode_x174
    cmos_set_image_mode_x3f0:
    /*000003f0*/ LDR         R0, WORD_04e0               @ LDR         R0, [PC, #232]              @ 0x00000000000004e0 
    /*000003f4*/ MOVW        R3, #1921                   @ 0x781 
    /*000003f8*/ LDR         R2, WORD_04d0               @ LDR         R2, [PC, #208]              @ 0x00000000000004d0 
    /*000003fc*/ LDR         R0, [IP, R0] 
    /*00000400*/ STR         R5, [SP, #16] 
    /*00000404*/ VLDR        S15, [R1, #4] 
    /*00000408*/ LDR         R1, WORD_04d4               @ LDR         R1, [PC, #196]              @ 0x00000000000004d4 
    /*0000040c*/ VCVT.F64.F32D16, S15 
    /*00000410*/ LDR         R0, [R0] 
    /*00000414*/ ADD         R2, PC, R2 
    /*00000418*/ STR         R4, [SP, #4] 
    /*0000041c*/ STR         LR, [SP] 
    /*00000420*/ ADD         R1, PC, R1 
    /*00000424*/ VSTR        D16, [SP, #8] 
    /*00000428*/ BL          fprintf
    /*0000042c*/ MVN         R0, #0 
    /*00000430*/ B           cmos_set_image_mode_x174
    cmos_set_image_mode_x434:
    /*00000434*/ LDR         R2, WORD_04e0               @ LDR         R2, [PC, #164]              @ 0x00000000000004e0 
    /*00000438*/ MOV         R3, #1856                   @ 0x740 
    /*0000043c*/ VCVT.F64.F32D16, S15 
    /*00000440*/ LDR         R1, [IP, R2] 
    /*00000444*/ LDR         R2, WORD_04d8               @ LDR         R2, [PC, #140]              @ 0x00000000000004d8 
    /*00000448*/ LDR         R0, [R1] 
    /*0000044c*/ LDR         R1, WORD_04dc               @ LDR         R1, [PC, #136]              @ 0x00000000000004dc 
    /*00000450*/ ADD         R2, PC, R2 
    /*00000454*/ STR         R5, [SP, #16] 
    /*00000458*/ STR         R4, [SP, #4] 
    /*0000045c*/ STR         LR, [SP] 
    /*00000460*/ VSTR        D16, [SP, #8] 
    /*00000464*/ ADD         R1, PC, R1 
    /*00000468*/ BL          fprintf
    /*0000046c*/ MVN         R0, #0 
    /*00000470*/ B           cmos_set_image_mode_x174
    /*00000474*/ RSBSMI      R0, R0, #0 
    /*00000478*/ RSCSMI      R0, R0, #0 
    /*0000047c*/ CMNMI       R0, #0 
    /*00000480*/ WORD_0480: .word 0x00000468
    /*00000484*/ WORD_0484: .word 0x00000000
    /*00000488*/ WORD_0488: .word 0x000003b4
    /*0000048c*/ WORD_048c: .word 0x000003a4
    /*00000490*/ WORD_0490: .word 0x000002cc
    /*00000494*/ WORD_0494: .word 0x000002bc
    /*00000498*/ WORD_0498: .word 0x00000288
    /*0000049c*/ WORD_049c: .word 0x00000280
    /*000004a0*/ WORD_04a0: .word 0x0000020c
    /*000004a4*/ WORD_04a4: .word 0x000001fc
    /*000004a8*/ WORD_04a8: .word 0x000001d4
    /*000004ac*/ WORD_04ac: .word 0x000001c4
    /*000004b0*/ WORD_04b0: .word 0x00000180
    /*000004b4*/ WORD_04b4: .word 0x00000170
    /*000004b8*/ WORD_04b8: .word 0x00000150
    /*000004bc*/ WORD_04bc: .word 0x00000150
    /*000004c0*/ WORD_04c0: .word 0x00000128
    /*000004c4*/ WORD_04c4: .word 0x00000128
    /*000004c8*/ WORD_04c8: .word 0x000000f0
    /*000004cc*/ WORD_04cc: .word 0x000000e8
    /*000004d0*/ WORD_04d0: .word 0x000000b4
    /*000004d4*/ WORD_04d4: .word 0x000000ac
    /*000004d8*/ WORD_04d8: .word 0x00000080
    /*000004dc*/ WORD_04dc: .word 0x00000070
    /*000004e0*/ WORD_04e0: .word 0x00000000
    /*000004e4*/ WORD_04e4: .word 0xffffffd4
    /*000004e8*/ WORD_04e8: .word 0xffffffc4
    cmos_set_image_mode_x4ec:
    /*000004ec*/ LDR         R2, WORD_04e0               @ LDR         R2, [PC, #-20]              @ 0x00000000000004e0 
    /*000004f0*/ MOVW        R3, #1911                   @ 0x777 
    /*000004f4*/ VCVT.F64.F32D16, S15 
    /*000004f8*/ LDR         R1, [IP, R2] 
    /*000004fc*/ LDR         R2, WORD_04e4               @ LDR         R2, [PC, #-32]              @ 0x00000000000004e4 
    /*00000500*/ LDR         R0, [R1] 
    /*00000504*/ LDR         R1, WORD_04e8               @ LDR         R1, [PC, #-36]              @ 0x00000000000004e8 
    /*00000508*/ ADD         R2, PC, R2 
    /*0000050c*/ STR         R5, [SP, #16] 
    /*00000510*/ STR         R4, [SP, #4] 
    /*00000514*/ STR         LR, [SP] 
    /*00000518*/ VSTR        D16, [SP, #8] 
    /*0000051c*/ ADD         R1, PC, R1 
    /*00000520*/ BL          fprintf
    /*00000524*/ MVN         R0, #0 
    /*00000528*/ B           cmos_set_image_mode_x174
FUNC_END cmos_set_image_mode


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.sensor_register_callback
@ Size: 0x370
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN sensor_register_callback
    /*00000000*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, FP, LR} 
    /*00000004*/ SUBS        R8, R1, #0 
    /*00000008*/ LDR         R4, WORD_02e4               @ LDR         R4, [PC, #724]              @ 0x00000000000002e4 
    /*0000000c*/ LDR         R3, WORD_02e8               @ LDR         R3, [PC, #724]              @ 0x00000000000002e8 
    /*00000010*/ ADD         R4, PC, R4 
    /*00000014*/ VPUSH       {D8-D9} 
    /*00000018*/ LDR         R7, [R4, R3] 
    /*0000001c*/ SUB         SP, SP, #100                @ 0x64 
    /*00000020*/ LDR         R3, [R7] 
    /*00000024*/ STR         R3, [SP, #92]               @ 0x5c 
    /*00000028*/ BEQ         sensor_register_callback_x2b0
    /*0000002c*/ CMP         R2, #0 
    /*00000030*/ MOV         SL, R2 
    /*00000034*/ BEQ         sensor_register_callback_x280
    /*00000038*/ LDR         R3, WORD_02ec               @ LDR         R3, [PC, #684]              @ 0x00000000000002ec 
    /*0000003c*/ MOV         R5, R0 
    /*00000040*/ LDR         FP, [R4, R3] 
    /*00000044*/ LDR         R6, [FP, R0, LSL #2] 
    /*00000048*/ CMP         R6, #0 
    /*0000004c*/ BEQ         sensor_register_callback_x240
    sensor_register_callback_x50:
    /*00000050*/ MOVW        R2, #2156                   @ 0x86c 
    /*00000054*/ MOV         R1, #0 
    /*00000058*/ MOV         R0, R6 
    /*0000005c*/ ADD         R9, SP, #8 
    /*00000060*/ BL          memset
    /*00000064*/ LDR         R3, WORD_02f0               @ LDR         R3, [PC, #644]              @ 0x00000000000002f0 
    /*00000068*/ MOVW        R2, #34220                  @ 0x85ac 
    /*0000006c*/ STR         R6, [FP, R5, LSL #2] 
    /*00000070*/ MOV         R1, R9 
    /*00000074*/ STR         R2, [SP, #8] 
    /*00000078*/ MOV         R0, R5 
    /*0000007c*/ LDR         IP, [R4, R3] 
    /*00000080*/ ADD         R2, SP, #20 
    /*00000084*/ LDR         R3, WORD_02f4               @ LDR         R3, [PC, #616]              @ 0x00000000000002f4 
    /*00000088*/ STR         IP, [SP, #20] 
    /*0000008c*/ LDR         IP, [R4, R3] 
    /*00000090*/ LDR         R3, WORD_02f8               @ LDR         R3, [PC, #608]              @ 0x00000000000002f8 
    /*00000094*/ STR         IP, [SP, #24] 
    /*00000098*/ ADD         R3, PC, R3 
    /*0000009c*/ STR         R3, [SP, #28] 
    /*000000a0*/ LDR         IP, WORD_02fc               @ LDR         IP, [PC, #596]              @ 0x00000000000002fc 
    /*000000a4*/ LDR         R3, WORD_0300               @ LDR         R3, [PC, #596]              @ 0x0000000000000300 
    /*000000a8*/ ADD         IP, PC, IP 
    /*000000ac*/ ADD         R3, PC, R3 
    /*000000b0*/ STR         IP, [SP, #32] 
    /*000000b4*/ STR         R3, [SP, #36]               @ 0x24 
    /*000000b8*/ LDR         IP, WORD_0304               @ LDR         IP, [PC, #580]              @ 0x0000000000000304 
    /*000000bc*/ LDR         R3, WORD_0308               @ LDR         R3, [PC, #580]              @ 0x0000000000000308 
    /*000000c0*/ ADD         IP, PC, IP 
    /*000000c4*/ ADD         R3, PC, R3 
    /*000000c8*/ STR         IP, [SP, #40]               @ 0x28 
    /*000000cc*/ STR         R3, [SP, #44]               @ 0x2c 
    /*000000d0*/ LDR         IP, WORD_030c               @ LDR         IP, [PC, #564]              @ 0x000000000000030c 
    /*000000d4*/ LDR         R3, WORD_0310               @ LDR         R3, [PC, #564]              @ 0x0000000000000310 
    /*000000d8*/ ADD         IP, PC, IP 
    /*000000dc*/ ADD         R3, PC, R3 
    /*000000e0*/ STR         IP, [SP, #52]               @ 0x34 
    /*000000e4*/ STR         R3, [SP, #48]               @ 0x30 
    /*000000e8*/ BL          HI_MPI_ISP_SensorRegCallBack
    /*000000ec*/ SUBS        R6, R0, #0 
    /*000000f0*/ VMOV.I32    Q4, #0                      @ 0x00000000 
    /*000000f4*/ BNE         sensor_register_callback_x218
    /*000000f8*/ LDR         LR, WORD_0314               @ LDR         LR, [PC, #532]              @ 0x0000000000000314 
    /*000000fc*/ MOV         R1, R8 
    /*00000100*/ LDR         IP, WORD_0318               @ LDR         IP, [PC, #528]              @ 0x0000000000000318 
    /*00000104*/ ADD         R3, SP, #56                 @ 0x38 
    /*00000108*/ ADD         LR, PC, LR 
    /*0000010c*/ MOV         R2, R9 
    /*00000110*/ ADD         IP, PC, IP 
    /*00000114*/ STR         LR, [SP, #60]               @ 0x3c 
    /*00000118*/ STR         IP, [SP, #64]               @ 0x40 
    /*0000011c*/ LDR         LR, WORD_031c               @ LDR         LR, [PC, #504]              @ 0x000000000000031c 
    /*00000120*/ LDR         IP, WORD_0320               @ LDR         IP, [PC, #504]              @ 0x0000000000000320 
    /*00000124*/ ADD         LR, PC, LR 
    /*00000128*/ ADD         IP, PC, IP 
    /*0000012c*/ STR         LR, [SP, #68]               @ 0x44 
    /*00000130*/ STR         IP, [SP, #72]               @ 0x48 
    /*00000134*/ LDR         LR, WORD_0324               @ LDR         LR, [PC, #488]              @ 0x0000000000000324 
    /*00000138*/ LDR         IP, WORD_0328               @ LDR         IP, [PC, #488]              @ 0x0000000000000328 
    /*0000013c*/ ADD         LR, PC, LR 
    /*00000140*/ ADD         IP, PC, IP 
    /*00000144*/ LDR         R0, WORD_032c               @ LDR         R0, [PC, #480]              @ 0x000000000000032c 
    /*00000148*/ STR         LR, [SP, #76]               @ 0x4c 
    /*0000014c*/ STR         IP, [SP, #80]               @ 0x50 
    /*00000150*/ LDR         LR, WORD_0330               @ LDR         LR, [PC, #472]              @ 0x0000000000000330 
    /*00000154*/ LDR         IP, WORD_0334               @ LDR         IP, [PC, #472]              @ 0x0000000000000334 
    /*00000158*/ ADD         R0, PC, R0 
    /*0000015c*/ STR         R0, [SP, #56]               @ 0x38 
    /*00000160*/ MOV         R0, R5 
    /*00000164*/ ADD         LR, PC, LR 
    /*00000168*/ ADD         IP, PC, IP 
    /*0000016c*/ STR         LR, [SP, #84]               @ 0x54 
    /*00000170*/ STR         IP, [SP, #88]               @ 0x58 
    /*00000174*/ BL          HI_MPI_AE_SensorRegCallBack
    /*00000178*/ SUBS        R6, R0, #0 
    /*0000017c*/ BNE         sensor_register_callback_x1f0
    /*00000180*/ LDR         IP, WORD_0338               @ LDR         IP, [PC, #432]              @ 0x0000000000000338 
    /*00000184*/ MOV         R2, R9 
    /*00000188*/ MOV         R1, SL 
    /*0000018c*/ MOV         R0, R5 
    /*00000190*/ ADD         IP, PC, IP 
    /*00000194*/ ADD         R3, SP, #12 
    /*00000198*/ VSTR        D8, [SP, #12] 
    /*0000019c*/ STR         IP, [SP, #12] 
    /*000001a0*/ BL          HI_MPI_AWB_SensorRegCallBack
    /*000001a4*/ SUBS        R6, R0, #0 
    /*000001a8*/ BEQ         sensor_register_callback_x1d0
    /*000001ac*/ LDR         R1, WORD_033c               @ LDR         R1, [PC, #392]              @ 0x000000000000033c 
    /*000001b0*/ MOVW        R3, #2168                   @ 0x878 
    /*000001b4*/ LDR         R2, WORD_0340               @ LDR         R2, [PC, #388]              @ 0x0000000000000340 
    /*000001b8*/ LDR         R0, [R4, R1] 
    /*000001bc*/ LDR         R1, WORD_0344               @ LDR         R1, [PC, #384]              @ 0x0000000000000344 
    /*000001c0*/ ADD         R2, PC, R2 
    /*000001c4*/ ADD         R1, PC, R1 
    /*000001c8*/ LDR         R0, [R0] 
    /*000001cc*/ BL          fprintf
    sensor_register_callback_x1d0:
    /*000001d0*/ LDR         R2, [SP, #92]               @ 0x5c 
    /*000001d4*/ MOV         R0, R6 
    /*000001d8*/ LDR         R3, [R7] 
    /*000001dc*/ CMP         R2, R3 
    /*000001e0*/ BNE         sensor_register_callback_x2e0
    /*000001e4*/ ADD         SP, SP, #100                @ 0x64 
    /*000001e8*/ VPOP        {D8-D9} 
    /*000001ec*/ POP         {R4, R5, R6, R7, R8, R9, SL, FP, PC} 
    sensor_register_callback_x1f0:
    /*000001f0*/ LDR         R1, WORD_033c               @ LDR         R1, [PC, #324]              @ 0x000000000000033c 
    /*000001f4*/ MOVW        R3, #2159                   @ 0x86f 
    /*000001f8*/ LDR         R2, WORD_0348               @ LDR         R2, [PC, #328]              @ 0x0000000000000348 
    /*000001fc*/ LDR         R0, [R4, R1] 
    /*00000200*/ LDR         R1, WORD_034c               @ LDR         R1, [PC, #324]              @ 0x000000000000034c 
    /*00000204*/ ADD         R2, PC, R2 
    /*00000208*/ ADD         R1, PC, R1 
    /*0000020c*/ LDR         R0, [R0] 
    /*00000210*/ BL          fprintf
    /*00000214*/ B           sensor_register_callback_x1d0
    sensor_register_callback_x218:
    /*00000218*/ LDR         R1, WORD_033c               @ LDR         R1, [PC, #284]              @ 0x000000000000033c 
    /*0000021c*/ MOVW        R3, #2150                   @ 0x866 
    /*00000220*/ LDR         R2, WORD_0350               @ LDR         R2, [PC, #296]              @ 0x0000000000000350 
    /*00000224*/ LDR         R0, [R4, R1] 
    /*00000228*/ LDR         R1, WORD_0354               @ LDR         R1, [PC, #292]              @ 0x0000000000000354 
    /*0000022c*/ ADD         R2, PC, R2 
    /*00000230*/ ADD         R1, PC, R1 
    /*00000234*/ LDR         R0, [R0] 
    /*00000238*/ BL          fprintf
    /*0000023c*/ B           sensor_register_callback_x1d0
    sensor_register_callback_x240:
    /*00000240*/ MOVW        R0, #2156                   @ 0x86c 
    /*00000244*/ BL          malloc
    /*00000248*/ SUBS        R6, R0, #0 
    /*0000024c*/ BNE         sensor_register_callback_x50
    /*00000250*/ LDR         R1, WORD_033c               @ LDR         R1, [PC, #228]              @ 0x000000000000033c 
    /*00000254*/ MOVW        R3, #2104                   @ 0x838 
    /*00000258*/ LDR         R2, WORD_0358               @ LDR         R2, [PC, #248]              @ 0x0000000000000358 
    /*0000025c*/ MVN         R6, #0 
    /*00000260*/ LDR         R0, [R4, R1] 
    /*00000264*/ LDR         R1, WORD_035c               @ LDR         R1, [PC, #240]              @ 0x000000000000035c 
    /*00000268*/ ADD         R2, PC, R2 
    /*0000026c*/ ADD         R1, PC, R1 
    /*00000270*/ LDR         R0, [R0] 
    /*00000274*/ STR         R5, [SP] 
    /*00000278*/ BL          fprintf
    /*0000027c*/ B           sensor_register_callback_x1d0
    sensor_register_callback_x280:
    /*00000280*/ LDR         R1, WORD_033c               @ LDR         R1, [PC, #180]              @ 0x000000000000033c 
    /*00000284*/ MOVW        R3, #2134                   @ 0x856 
    /*00000288*/ LDR         R2, WORD_0360               @ LDR         R2, [PC, #208]              @ 0x0000000000000360 
    /*0000028c*/ MOVW        R6, #32774                  @ 0x8006 
    /*00000290*/ LDR         R0, [R4, R1] 
    /*00000294*/ LDR         R1, WORD_0364               @ LDR         R1, [PC, #200]              @ 0x0000000000000364 
    /*00000298*/ ADD         R2, PC, R2 
    /*0000029c*/ ADD         R1, PC, R1 
    /*000002a0*/ LDR         R0, [R0] 
    /*000002a4*/ MOVT        R6, #40988                  @ 0xa01c 
    /*000002a8*/ BL          fprintf
    /*000002ac*/ B           sensor_register_callback_x1d0
    sensor_register_callback_x2b0:
    /*000002b0*/ LDR         R1, WORD_033c               @ LDR         R1, [PC, #132]              @ 0x000000000000033c 
    /*000002b4*/ MOVW        R3, #2133                   @ 0x855 
    /*000002b8*/ LDR         R2, WORD_0368               @ LDR         R2, [PC, #168]              @ 0x0000000000000368 
    /*000002bc*/ MOVW        R6, #32774                  @ 0x8006 
    /*000002c0*/ LDR         R0, [R4, R1] 
    /*000002c4*/ LDR         R1, WORD_036c               @ LDR         R1, [PC, #160]              @ 0x000000000000036c 
    /*000002c8*/ ADD         R2, PC, R2 
    /*000002cc*/ ADD         R1, PC, R1 
    /*000002d0*/ LDR         R0, [R0] 
    /*000002d4*/ MOVT        R6, #40988                  @ 0xa01c 
    /*000002d8*/ BL          fprintf
    /*000002dc*/ B           sensor_register_callback_x1d0
    sensor_register_callback_x2e0:
    /*000002e0*/ BL          __stack_chk_fail
    /*000002e4*/ WORD_02e4: .word 0x000002cc
    /*000002e8*/ WORD_02e8: .word 0x00000000
    /*000002ec*/ WORD_02ec: .word 0x00000000
    /*000002f0*/ WORD_02f0: .word 0x00000000
    /*000002f4*/ WORD_02f4: .word 0x00000000
    /*000002f8*/ WORD_02f8: .word 0x00000258
    /*000002fc*/ WORD_02fc: .word 0x0000024c
    /*00000300*/ WORD_0300: .word 0x0000024c
    /*00000304*/ WORD_0304: .word 0x0000023c
    /*00000308*/ WORD_0308: .word 0x0000023c
    /*0000030c*/ WORD_030c: .word 0x0000022c
    /*00000310*/ WORD_0310: .word 0x0000022c
    /*00000314*/ WORD_0314: .word 0x00000204
    /*00000318*/ WORD_0318: .word 0x00000200
    /*0000031c*/ WORD_031c: .word 0x000001f0
    /*00000320*/ WORD_0320: .word 0x000001f0
    /*00000324*/ WORD_0324: .word 0x000001e0
    /*00000328*/ WORD_0328: .word 0x000001e0
    /*0000032c*/ WORD_032c: .word 0x000001cc
    /*00000330*/ WORD_0330: .word 0x000001c4
    /*00000334*/ WORD_0334: .word 0x000001c4
    /*00000338*/ WORD_0338: .word 0x000001a0
    /*0000033c*/ WORD_033c: .word 0x00000000
    /*00000340*/ WORD_0340: .word 0x00000178
    /*00000344*/ WORD_0344: .word 0x00000178
    /*00000348*/ WORD_0348: .word 0x0000013c
    /*0000034c*/ WORD_034c: .word 0x0000013c
    /*00000350*/ WORD_0350: .word 0x0000011c
    /*00000354*/ WORD_0354: .word 0x0000011c
    /*00000358*/ WORD_0358: .word 0x000000e8
    /*0000035c*/ WORD_035c: .word 0x000000e8
    /*00000360*/ WORD_0360: .word 0x000000c0
    /*00000364*/ WORD_0364: .word 0x000000c0
    /*00000368*/ WORD_0368: .word 0x00000098
    /*0000036c*/ WORD_036c: .word 0x00000098
FUNC_END sensor_register_callback


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_get_sns_regs_info
@ Size: 0x3c4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_get_sns_regs_info
    /*00000000*/ PUSH        {R4, R5, R6, R7, R8, R9, SL, LR} 
    /*00000004*/ CMP         R1, #0 
    /*00000008*/ LDR         LR, WORD_0398               @ LDR         LR, [PC, #904]              @ 0x0000000000000398 
    /*0000000c*/ ADD         LR, PC, LR 
    /*00000010*/ BEQ         cmos_get_sns_regs_info_x338
    /*00000014*/ LDR         R3, WORD_039c               @ LDR         R3, [PC, #896]              @ 0x000000000000039c 
    /*00000018*/ LDR         R3, [LR, R3] 
    /*0000001c*/ LDR         R4, [R3, R0, LSL #2] 
    /*00000020*/ CMP         R4, #0 
    /*00000024*/ BEQ         cmos_get_sns_regs_info_x368
    /*00000028*/ LDR         R3, [R4, #4] 
    /*0000002c*/ CMP         R3, #0 
    /*00000030*/ BEQ         cmos_get_sns_regs_info_xd0
    /*00000034*/ LDR         R3, [R1, #1052]             @ 0x41c 
    /*00000038*/ CMP         R3, #0 
    /*0000003c*/ BEQ         cmos_get_sns_regs_info_xd0
    /*00000040*/ LDR         LR, [R4, #20] 
    /*00000044*/ CMP         LR, #0 
    /*00000048*/ MOVNE       R2, #0 
    /*0000004c*/ MOVNE       R3, R4 
    /*00000050*/ MOVNE       R6, R2 
    /*00000054*/ MOVNE       R5, #1 
    /*00000058*/ BEQ         cmos_get_sns_regs_info_x84
    cmos_get_sns_regs_info_x5c:
    /*0000005c*/ LDR         R0, [R3, #1100]             @ 0x44c 
    /*00000060*/ ADD         R2, R2, #1 
    /*00000064*/ LDR         IP, [R3, #44]               @ 0x2c 
    /*00000068*/ ADD         R3, R3, #24 
    /*0000006c*/ CMP         IP, R0 
    /*00000070*/ MOVNE       R0, R5 
    /*00000074*/ MOVEQ       R0, R6 
    /*00000078*/ CMP         R2, LR 
    /*0000007c*/ STR         R0, [R3, #4] 
    /*00000080*/ BNE         cmos_get_sns_regs_info_x5c
    cmos_get_sns_regs_info_x84:
    /*00000084*/ LDR         R3, [R4, #12] 
    /*00000088*/ SUB         R3, R3, #4 
    /*0000008c*/ CMP         R3, #1 
    /*00000090*/ BLS         cmos_get_sns_regs_info_x25c
    cmos_get_sns_regs_info_x94:
    /*00000094*/ MOV         R5, #0 
    /*00000098*/ ADD         R6, R4, #16 
    /*0000009c*/ STR         R5, [R1, #1052]             @ 0x41c 
    /*000000a0*/ MOV         R0, R1 
    /*000000a4*/ MOV         R2, #1056                   @ 0x420 
    /*000000a8*/ MOV         R1, R6 
    /*000000ac*/ BL          memcpy
    /*000000b0*/ MOV         R1, R6 
    /*000000b4*/ MOV         R2, #1056                   @ 0x420 
    /*000000b8*/ ADD         R0, R4, #1072               @ 0x430 
    /*000000bc*/ BL          memcpy
    /*000000c0*/ LDR         R3, [R4, #2128]             @ 0x850 
    /*000000c4*/ MOV         R0, R5 
    /*000000c8*/ STR         R3, [R4, #2132]             @ 0x854 
    /*000000cc*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    cmos_get_sns_regs_info_xd0:
    /*000000d0*/ LDR         R3, WORD_03a0               @ LDR         R3, [PC, #712]              @ 0x00000000000003a0 
    /*000000d4*/ MOV         R2, #0 
    /*000000d8*/ STR         R2, [R4, #16] 
    /*000000dc*/ MOV         IP, #9 
    /*000000e0*/ LDR         R3, [LR, R3] 
    /*000000e4*/ MOV         R2, #2 
    /*000000e8*/ LDR         R7, [R4, #12] 
    /*000000ec*/ LDRB        R3, [R3, R0] 
    /*000000f0*/ STRB        R2, [R4, #24] 
    /*000000f4*/ CMP         R7, #3 
    /*000000f8*/ STRB        R3, [R4, #25] 
    /*000000fc*/ STR         IP, [R4, #20] 
    /*00000100*/ BEQ         cmos_get_sns_regs_info_x224
    /*00000104*/ SUB         R3, R7, #4 
    /*00000108*/ CMP         R3, #1 
    /*0000010c*/ BLS         cmos_get_sns_regs_info_x224
    /*00000110*/ SUB         R3, R7, #6 
    /*00000114*/ CMP         R3, #2 
    /*00000118*/ MOVLS       IP, #13 
    /*0000011c*/ STRLS       IP, [R4, #20] 
    /*00000120*/ BLS         cmos_get_sns_regs_info_x134
    /*00000124*/ SUB         R3, R7, #9 
    /*00000128*/ CMP         R3, #2 
    /*0000012c*/ MOVLS       IP, #15 
    /*00000130*/ STRLS       IP, [R4, #20] 
    cmos_get_sns_regs_info_x134:
    /*00000134*/ LDR         R6, WORD_03a4               @ LDR         R6, [PC, #616]              @ 0x00000000000003a4 
    /*00000138*/ MOV         R3, R4 
    /*0000013c*/ LDR         R5, WORD_03a8               @ LDR         R5, [PC, #612]              @ 0x00000000000003a8 
    /*00000140*/ MOV         R2, #0 
    /*00000144*/ LDR         R8, WORD_03ac               @ LDR         R8, [PC, #608]              @ 0x00000000000003ac 
    /*00000148*/ MOV         R0, #1 
    /*0000014c*/ LDR         R9, [LR, R6] 
    /*00000150*/ LDR         R8, [LR, R8] 
    /*00000154*/ LDR         LR, [LR, R5] 
    /*00000158*/ LDRB        R6, [R8] 
    /*0000015c*/ LDR         R5, [R9] 
    /*00000160*/ LDR         LR, [LR] 
    cmos_get_sns_regs_info_x164:
    /*00000164*/ STR         R0, [R3, #28] 
    /*00000168*/ ADD         R2, R2, #1 
    /*0000016c*/ CMP         IP, R2 
    /*00000170*/ ADD         R3, R3, #24 
    /*00000174*/ STRB        R6, [R3, #10] 
    /*00000178*/ STR         R5, [R3, #16] 
    /*0000017c*/ STR         LR, [R3, #24] 
    /*00000180*/ BHI         cmos_get_sns_regs_info_x164
    /*00000184*/ STRB        R0, [R4, #80]               @ 0x50 
    /*00000188*/ MOV         R6, #12928                  @ 0x3280 
    /*0000018c*/ STR         R6, [R4, #84]               @ 0x54 
    /*00000190*/ MOV         IP, #516                    @ 0x204 
    /*00000194*/ STR         IP, [R4, #108]              @ 0x6c 
    /*00000198*/ MOVW        R2, #517                    @ 0x205 
    /*0000019c*/ STR         R2, [R4, #132]              @ 0x84 
    /*000001a0*/ CMP         R7, #3 
    /*000001a4*/ STRB        R0, [R4, #104]              @ 0x68 
    /*000001a8*/ MOV         R3, #0 
    /*000001ac*/ STRB        R0, [R4, #128]              @ 0x80 
    /*000001b0*/ MOVW        R5, #514                    @ 0x202 
    /*000001b4*/ STRB        R0, [R4, #152]              @ 0x98 
    /*000001b8*/ MOVW        LR, #515                    @ 0x203 
    /*000001bc*/ STRB        R0, [R4, #176]              @ 0xb0 
    /*000001c0*/ MOVW        R8, #12552                  @ 0x3108 
    /*000001c4*/ STRB        R3, [R4, #32] 
    /*000001c8*/ MOVW        R6, #12553                  @ 0x3109 
    /*000001cc*/ STRB        R3, [R4, #56]               @ 0x38 
    /*000001d0*/ MOV         IP, #832                    @ 0x340 
    /*000001d4*/ STRB        R3, [R4, #200]              @ 0xc8 
    /*000001d8*/ MOVW        R2, #833                    @ 0x341 
    /*000001dc*/ STRB        R3, [R4, #224]              @ 0xe0 
    /*000001e0*/ STR         R5, [R4, #36]               @ 0x24 
    /*000001e4*/ STR         LR, [R4, #60]               @ 0x3c 
    /*000001e8*/ STR         R8, [R4, #156]              @ 0x9c 
    /*000001ec*/ STR         R6, [R4, #180]              @ 0xb4 
    /*000001f0*/ STR         IP, [R4, #204]              @ 0xcc 
    /*000001f4*/ STR         R2, [R4, #228]              @ 0xe4 
    /*000001f8*/ BEQ         cmos_get_sns_regs_info_x230
    /*000001fc*/ SUB         R6, R7, #4 
    /*00000200*/ CMP         R6, #1 
    /*00000204*/ BLS         cmos_get_sns_regs_info_x274
    /*00000208*/ CMP         R7, #6 
    /*0000020c*/ BEQ         cmos_get_sns_regs_info_x298
    /*00000210*/ CMP         R7, #9 
    /*00000214*/ BEQ         cmos_get_sns_regs_info_x2dc
    cmos_get_sns_regs_info_x218:
    /*00000218*/ MOV         R3, #1 
    /*0000021c*/ STR         R3, [R4, #4] 
    /*00000220*/ B           cmos_get_sns_regs_info_x94
    cmos_get_sns_regs_info_x224:
    /*00000224*/ MOV         IP, #11 
    /*00000228*/ STR         IP, [R4, #20] 
    /*0000022c*/ B           cmos_get_sns_regs_info_x134
    cmos_get_sns_regs_info_x230:
    /*00000230*/ STRB        R0, [R4, #200]              @ 0xc8 
    /*00000234*/ MOVW        LR, #12586                  @ 0x312a 
    /*00000238*/ STRB        R0, [R4, #224]              @ 0xe0 
    /*0000023c*/ MOVW        R3, #12587                  @ 0x312b 
    /*00000240*/ STRB        R0, [R4, #248]              @ 0xf8 
    /*00000244*/ STR         IP, [R4, #252]              @ 0xfc 
    /*00000248*/ STRB        R0, [R4, #272]              @ 0x110 
    /*0000024c*/ STR         R2, [R4, #276]              @ 0x114 
    /*00000250*/ STR         LR, [R4, #204]              @ 0xcc 
    /*00000254*/ STR         R3, [R4, #228]              @ 0xe4 
    /*00000258*/ B           cmos_get_sns_regs_info_x218
    cmos_get_sns_regs_info_x25c:
    /*0000025c*/ MOV         R3, #1 
    /*00000260*/ STR         R3, [R4, #28] 
    /*00000264*/ STR         R3, [R4, #52]               @ 0x34 
    /*00000268*/ STR         R3, [R4, #196]              @ 0xc4 
    /*0000026c*/ STR         R3, [R4, #220]              @ 0xdc 
    /*00000270*/ B           cmos_get_sns_regs_info_x94
    cmos_get_sns_regs_info_x274:
    /*00000274*/ STRB        R0, [R4, #200]              @ 0xc8 
    /*00000278*/ STR         R5, [R4, #204]              @ 0xcc 
    /*0000027c*/ STRB        R0, [R4, #224]              @ 0xe0 
    /*00000280*/ STR         LR, [R4, #228]              @ 0xe4 
    /*00000284*/ STRB        R3, [R4, #248]              @ 0xf8 
    /*00000288*/ STR         IP, [R4, #252]              @ 0xfc 
    /*0000028c*/ STRB        R3, [R4, #272]              @ 0x110 
    /*00000290*/ STR         R2, [R4, #276]              @ 0x114 
    /*00000294*/ B           cmos_get_sns_regs_info_x218
    cmos_get_sns_regs_info_x298:
    /*00000298*/ STRB        R0, [R4, #200]              @ 0xc8 
    /*0000029c*/ MOVW        R6, #12586                  @ 0x312a 
    /*000002a0*/ STRB        R0, [R4, #224]              @ 0xe0 
    /*000002a4*/ MOVW        R5, #12587                  @ 0x312b 
    /*000002a8*/ STRB        R0, [R4, #248]              @ 0xf8 
    /*000002ac*/ MOVW        LR, #12588                  @ 0x312c 
    /*000002b0*/ STRB        R0, [R4, #272]              @ 0x110 
    /*000002b4*/ MOVW        R3, #12589                  @ 0x312d 
    /*000002b8*/ STRB        R0, [R4, #296]              @ 0x128 
    /*000002bc*/ STR         IP, [R4, #300]              @ 0x12c 
    /*000002c0*/ STRB        R0, [R4, #320]              @ 0x140 
    /*000002c4*/ STR         R2, [R4, #324]              @ 0x144 
    /*000002c8*/ STR         R6, [R4, #204]              @ 0xcc 
    /*000002cc*/ STR         R5, [R4, #228]              @ 0xe4 
    /*000002d0*/ STR         LR, [R4, #252]              @ 0xfc 
    /*000002d4*/ STR         R3, [R4, #276]              @ 0x114 
    /*000002d8*/ B           cmos_get_sns_regs_info_x218
    cmos_get_sns_regs_info_x2dc:
    /*000002dc*/ STR         IP, [R4, #348]              @ 0x15c 
    /*000002e0*/ MOVW        R7, #12586                  @ 0x312a 
    /*000002e4*/ STRB        R0, [R4, #200]              @ 0xc8 
    /*000002e8*/ MOVW        R6, #12587                  @ 0x312b 
    /*000002ec*/ STRB        R0, [R4, #224]              @ 0xe0 
    /*000002f0*/ MOVW        R5, #12588                  @ 0x312c 
    /*000002f4*/ STRB        R0, [R4, #248]              @ 0xf8 
    /*000002f8*/ MOVW        LR, #12589                  @ 0x312d 
    /*000002fc*/ STRB        R0, [R4, #272]              @ 0x110 
    /*00000300*/ MOVW        IP, #12590                  @ 0x312e 
    /*00000304*/ STRB        R0, [R4, #296]              @ 0x128 
    /*00000308*/ MOVW        R3, #12591                  @ 0x312f 
    /*0000030c*/ STRB        R0, [R4, #320]              @ 0x140 
    /*00000310*/ STRB        R0, [R4, #344]              @ 0x158 
    /*00000314*/ STRB        R0, [R4, #368]              @ 0x170 
    /*00000318*/ STR         R2, [R4, #372]              @ 0x174 
    /*0000031c*/ STR         R7, [R4, #204]              @ 0xcc 
    /*00000320*/ STR         R6, [R4, #228]              @ 0xe4 
    /*00000324*/ STR         R5, [R4, #252]              @ 0xfc 
    /*00000328*/ STR         LR, [R4, #276]              @ 0x114 
    /*0000032c*/ STR         IP, [R4, #300]              @ 0x12c 
    /*00000330*/ STR         R3, [R4, #324]              @ 0x144 
    /*00000334*/ B           cmos_get_sns_regs_info_x218
    cmos_get_sns_regs_info_x338:
    /*00000338*/ LDR         R1, WORD_03b0               @ LDR         R1, [PC, #112]              @ 0x00000000000003b0 
    /*0000033c*/ MOVW        R3, #1631                   @ 0x65f 
    /*00000340*/ LDR         R2, WORD_03b4               @ LDR         R2, [PC, #108]              @ 0x00000000000003b4 
    /*00000344*/ LDR         R0, [LR, R1] 
    /*00000348*/ LDR         R1, WORD_03b8               @ LDR         R1, [PC, #104]              @ 0x00000000000003b8 
    /*0000034c*/ ADD         R2, PC, R2 
    /*00000350*/ ADD         R1, PC, R1 
    /*00000354*/ LDR         R0, [R0] 
    /*00000358*/ BL          fprintf
    /*0000035c*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000360*/ MOVT        R0, #40988                  @ 0xa01c 
    /*00000364*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    cmos_get_sns_regs_info_x368:
    /*00000368*/ LDR         R1, WORD_03b0               @ LDR         R1, [PC, #64]               @ 0x00000000000003b0 
    /*0000036c*/ MOVW        R3, #1633                   @ 0x661 
    /*00000370*/ LDR         R2, WORD_03bc               @ LDR         R2, [PC, #68]               @ 0x00000000000003bc 
    /*00000374*/ LDR         R0, [LR, R1] 
    /*00000378*/ LDR         R1, WORD_03c0               @ LDR         R1, [PC, #64]               @ 0x00000000000003c0 
    /*0000037c*/ ADD         R2, PC, R2 
    /*00000380*/ ADD         R1, PC, R1 
    /*00000384*/ LDR         R0, [R0] 
    /*00000388*/ BL          fprintf
    /*0000038c*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000390*/ MOVT        R0, #40988                  @ 0xa01c 
    /*00000394*/ POP         {R4, R5, R6, R7, R8, R9, SL, PC} 
    /*00000398*/ WORD_0398: .word 0x00000384
    /*0000039c*/ WORD_039c: .word 0x00000000
    /*000003a0*/ WORD_03a0: .word 0x00000000
    /*000003a4*/ WORD_03a4: .word 0x00000000
    /*000003a8*/ WORD_03a8: .word 0x00000000
    /*000003ac*/ WORD_03ac: .word 0x00000000
    /*000003b0*/ WORD_03b0: .word 0x00000000
    /*000003b4*/ WORD_03b4: .word 0x00000060
    /*000003b8*/ WORD_03b8: .word 0x00000060
    /*000003bc*/ WORD_03bc: .word 0x00000038
    /*000003c0*/ WORD_03c0: .word 0x00000038
FUNC_END cmos_get_sns_regs_info


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_get_isp_default
@ Size: 0x408
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_get_isp_default
    /*00000000*/ PUSH        {R4, R5, R6, LR} 
    /*00000004*/ SUBS        R4, R1, #0 
    /*00000008*/ LDR         IP, WORD_0370               @ LDR         IP, [PC, #864]              @ 0x0000000000000370 
    /*0000000c*/ ADD         IP, PC, IP 
    /*00000010*/ BEQ         cmos_get_isp_default_x340
    /*00000014*/ LDR         R3, WORD_0374               @ LDR         R3, [PC, #856]              @ 0x0000000000000374 
    /*00000018*/ LDR         R3, [IP, R3] 
    /*0000001c*/ LDR         R5, [R3, R0, LSL #2] 
    /*00000020*/ CMP         R5, #0 
    /*00000024*/ BEQ         cmos_get_isp_default_x310
    /*00000028*/ MOV         R2, #776                    @ 0x308 
    /*0000002c*/ MOV         R1, #0 
    /*00000030*/ MOV         R0, R4 
    /*00000034*/ BL          memset
    /*00000038*/ LDR         R1, WORD_0378               @ LDR         R1, [PC, #824]              @ 0x0000000000000378 
    /*0000003c*/ MOV         R3, #3 
    /*00000040*/ STRB        R3, [R4, #2] 
    /*00000044*/ MOV         R3, #65                     @ 0x41 
    /*00000048*/ STRB        R3, [R4, #1] 
    /*0000004c*/ MOV         R3, #0 
    /*00000050*/ STRB        R3, [R4] 
    /*00000054*/ LDR         R2, WORD_037c               @ LDR         R2, [PC, #800]              @ 0x000000000000037c 
    /*00000058*/ LDR         R3, WORD_0380               @ LDR         R3, [PC, #800]              @ 0x0000000000000380 
    /*0000005c*/ LDR         LR, [R5, #12] 
    /*00000060*/ ADD         R1, PC, R1 
    /*00000064*/ ADD         R2, PC, R2 
    /*00000068*/ CMP         LR, #1 
    /*0000006c*/ ADD         R3, PC, R3 
    /*00000070*/ LDR         IP, WORD_0384               @ LDR         IP, [PC, #780]              @ 0x0000000000000384 
    /*00000074*/ LDR         R0, WORD_0388               @ LDR         R0, [PC, #780]              @ 0x0000000000000388 
    /*00000078*/ STR         R1, [R4, #64]               @ 0x40 
    /*0000007c*/ STR         R2, [R4, #72]               @ 0x48 
    /*00000080*/ LDR         R1, WORD_038c               @ LDR         R1, [PC, #772]              @ 0x000000000000038c 
    /*00000084*/ LDR         R2, WORD_0390               @ LDR         R2, [PC, #772]              @ 0x0000000000000390 
    /*00000088*/ STR         R3, [R4, #40]               @ 0x28 
    /*0000008c*/ LDR         R3, WORD_0394               @ LDR         R3, [PC, #768]              @ 0x0000000000000394 
    /*00000090*/ ADD         IP, PC, IP 
    /*00000094*/ ADD         R0, PC, R0 
    /*00000098*/ ADD         R1, PC, R1 
    /*0000009c*/ ADD         R2, PC, R2 
    /*000000a0*/ ADD         R3, PC, R3 
    /*000000a4*/ STR         IP, [R4, #76]               @ 0x4c 
    /*000000a8*/ STR         R0, [R4, #32] 
    /*000000ac*/ STR         R1, [R4, #44]               @ 0x2c 
    /*000000b0*/ STR         R2, [R4, #28] 
    /*000000b4*/ STR         R3, [R4, #16] 
    /*000000b8*/ BEQ         cmos_get_isp_default_x240
    /*000000bc*/ BCC         cmos_get_isp_default_xcc
    /*000000c0*/ SUB         LR, LR, #3 
    /*000000c4*/ CMP         LR, #8 
    /*000000c8*/ BLS         cmos_get_isp_default_x240
    cmos_get_isp_default_xcc:
    /*000000cc*/ LDRB        R3, [R4] 
    /*000000d0*/ MOV         R2, #604                    @ 0x25c 
    /*000000d4*/ LDR         R1, WORD_0398               @ LDR         R1, [PC, #700]              @ 0x0000000000000398 
    /*000000d8*/ ADD         R0, R4, #84                 @ 0x54 
    /*000000dc*/ LDRB        LR, [R4, #1] 
    /*000000e0*/ ORR         R3, R3, #155                @ 0x9b 
    /*000000e4*/ LDRB        IP, [R4, #2] 
    /*000000e8*/ ADD         R1, PC, R1 
    /*000000ec*/ ORR         LR, LR, #60                 @ 0x3c 
    /*000000f0*/ STRB        R3, [R4] 
    /*000000f4*/ ORR         IP, IP, #4 
    /*000000f8*/ LDR         R3, WORD_039c               @ LDR         R3, [PC, #668]              @ 0x000000000000039c 
    /*000000fc*/ STR         R1, [R4, #12] 
    /*00000100*/ LDR         R1, WORD_03a0               @ LDR         R1, [PC, #664]              @ 0x00000000000003a0 
    /*00000104*/ ADD         R3, PC, R3 
    /*00000108*/ STRB        LR, [R4, #1] 
    /*0000010c*/ STRB        IP, [R4, #2] 
    /*00000110*/ ADD         R1, PC, R1 
    /*00000114*/ STR         R3, [R4, #24] 
    /*00000118*/ LDR         LR, WORD_03a4               @ LDR         LR, [PC, #644]              @ 0x00000000000003a4 
    /*0000011c*/ LDR         IP, WORD_03a8               @ LDR         IP, [PC, #644]              @ 0x00000000000003a8 
    /*00000120*/ LDR         R3, WORD_03ac               @ LDR         R3, [PC, #644]              @ 0x00000000000003ac 
    /*00000124*/ STR         R1, [R4, #52]               @ 0x34 
    /*00000128*/ LDR         R1, WORD_03b0               @ LDR         R1, [PC, #640]              @ 0x00000000000003b0 
    /*0000012c*/ ADD         LR, PC, LR 
    /*00000130*/ ADD         IP, PC, IP 
    /*00000134*/ ADD         R3, PC, R3 
    /*00000138*/ STR         LR, [R4, #8] 
    /*0000013c*/ STR         IP, [R4, #56]               @ 0x38 
    /*00000140*/ ADD         R1, PC, R1 
    /*00000144*/ STR         R3, [R4, #36]               @ 0x24 
    /*00000148*/ LDR         LR, WORD_03b4               @ LDR         LR, [PC, #612]              @ 0x00000000000003b4 
    /*0000014c*/ LDR         IP, WORD_03b8               @ LDR         IP, [PC, #612]              @ 0x00000000000003b8 
    /*00000150*/ LDR         R3, WORD_03bc               @ LDR         R3, [PC, #612]              @ 0x00000000000003bc 
    /*00000154*/ STR         R1, [R4, #48]               @ 0x30 
    /*00000158*/ LDR         R1, WORD_03c0               @ LDR         R1, [PC, #608]              @ 0x00000000000003c0 
    /*0000015c*/ ADD         R3, PC, R3 
    /*00000160*/ ADD         LR, PC, LR 
    /*00000164*/ ADD         IP, PC, IP 
    /*00000168*/ STR         R3, [R4, #80]               @ 0x50 
    /*0000016c*/ STR         LR, [R4, #20] 
    /*00000170*/ STR         IP, [R4, #60]               @ 0x3c 
    /*00000174*/ ADD         R1, PC, R1 
    /*00000178*/ BL          memcpy
    /*0000017c*/ LDR         R3, [R5, #12] 
    /*00000180*/ CMP         R3, #3 
    /*00000184*/ BEQ         cmos_get_isp_default_x2fc
    cmos_get_isp_default_x188:
    /*00000188*/ CMP         R3, #4 
    /*0000018c*/ MOVEQ       R3, #64                     @ 0x40 
    /*00000190*/ MOVEQ       R2, #512                    @ 0x200 
    /*00000194*/ STREQ       R2, [R4, #760]              @ 0x2f8 
    /*00000198*/ STREQ       R3, [R4, #764]              @ 0x2fc 
    /*0000019c*/ STREQ       R3, [R4, #768]              @ 0x300 
    cmos_get_isp_default_x1a0:
    /*000001a0*/ LDR         R3, WORD_03c4               @ LDR         R3, [PC, #540]              @ 0x00000000000003c4 
    /*000001a4*/ MOVW        R2, #34220                  @ 0x85ac 
    /*000001a8*/ STR         R2, [R4, #696]              @ 0x2b8 
    /*000001ac*/ ADD         R6, R4, #740                @ 0x2e4 
    /*000001b0*/ ADD         R3, PC, R3 
    /*000001b4*/ MOV         IP, #0 
    /*000001b8*/ LDRB        LR, [R5, #8] 
    /*000001bc*/ MOVW        R5, #742                    @ 0x2e6 
    /*000001c0*/ LDM         R3!, {R0, R1, R2} 
    /*000001c4*/ MOV         R3, #12 
    /*000001c8*/ STRB        LR, [R4, #700]              @ 0x2bc 
    /*000001cc*/ ADD         LR, R4, #716                @ 0x2cc 
    /*000001d0*/ STR         R1, [R4, #752]              @ 0x2f0 
    /*000001d4*/ MOVW        R1, #718                    @ 0x2ce 
    /*000001d8*/ STR         R2, [R4, #756]              @ 0x2f4 
    /*000001dc*/ MOVW        R2, #4095                   @ 0xfff 
    /*000001e0*/ STRB        R3, [R4, #708]              @ 0x2c4 
    /*000001e4*/ MOV         R3, #1 
    /*000001e8*/ STR         R2, [R4, #720]              @ 0x2d0 
    /*000001ec*/ MOV         R2, #2 
    /*000001f0*/ STR         R0, [R4, #748]              @ 0x2ec 
    /*000001f4*/ MOV         R0, IP 
    /*000001f8*/ STR         R3, [R4, #728]              @ 0x2d8 
    /*000001fc*/ STR         R3, [R4, #724]              @ 0x2d4 
    /*00000200*/ STR         R3, [R4, #736]              @ 0x2e0 
    /*00000204*/ STR         R3, [R4, #732]              @ 0x2dc 
    /*00000208*/ STRH        R2, [R6] 
    /*0000020c*/ STRH        R2, [R4, R5] 
    /*00000210*/ STRH        R2, [LR] 
    /*00000214*/ STR         R3, [R4, #712]              @ 0x2c8 
    /*00000218*/ STRB        R3, [R4, #710]              @ 0x2c6 
    /*0000021c*/ STRB        R2, [R4, #711]              @ 0x2c7 
    /*00000220*/ STRB        R3, [R4, #745]              @ 0x2e9 
    /*00000224*/ STRB        R3, [R4, #746]              @ 0x2ea 
    /*00000228*/ STRB        R2, [R4, #747]              @ 0x2eb 
    /*0000022c*/ STR         R3, [R4, #704]              @ 0x2c0 
    /*00000230*/ STRH        R2, [R4, R1] 
    /*00000234*/ STRB        IP, [R4, #709]              @ 0x2c5 
    /*00000238*/ STRB        IP, [R4, #744]              @ 0x2e8 
    /*0000023c*/ POP         {R4, R5, R6, PC} 
    cmos_get_isp_default_x240:
    /*00000240*/ LDRB        R3, [R4] 
    /*00000244*/ MOV         R2, #604                    @ 0x25c 
    /*00000248*/ LDR         R1, WORD_03c8               @ LDR         R1, [PC, #376]              @ 0x00000000000003c8 
    /*0000024c*/ ADD         R0, R4, #84                 @ 0x54 
    /*00000250*/ LDRB        LR, [R4, #1] 
    /*00000254*/ ORR         R3, R3, #155                @ 0x9b 
    /*00000258*/ LDRB        IP, [R4, #2] 
    /*0000025c*/ ADD         R1, PC, R1 
    /*00000260*/ ORR         LR, LR, #60                 @ 0x3c 
    /*00000264*/ STRB        R3, [R4] 
    /*00000268*/ ORR         IP, IP, #4 
    /*0000026c*/ LDR         R3, WORD_03cc               @ LDR         R3, [PC, #344]              @ 0x00000000000003cc 
    /*00000270*/ STR         R1, [R4, #12] 
    /*00000274*/ LDR         R1, WORD_03d0               @ LDR         R1, [PC, #340]              @ 0x00000000000003d0 
    /*00000278*/ ADD         R3, PC, R3 
    /*0000027c*/ STRB        LR, [R4, #1] 
    /*00000280*/ STRB        IP, [R4, #2] 
    /*00000284*/ ADD         R1, PC, R1 
    /*00000288*/ STR         R3, [R4, #24] 
    /*0000028c*/ LDR         LR, WORD_03d4               @ LDR         LR, [PC, #320]              @ 0x00000000000003d4 
    /*00000290*/ LDR         IP, WORD_03d8               @ LDR         IP, [PC, #320]              @ 0x00000000000003d8 
    /*00000294*/ LDR         R3, WORD_03dc               @ LDR         R3, [PC, #320]              @ 0x00000000000003dc 
    /*00000298*/ STR         R1, [R4, #56]               @ 0x38 
    /*0000029c*/ LDR         R1, WORD_03e0               @ LDR         R1, [PC, #316]              @ 0x00000000000003e0 
    /*000002a0*/ ADD         LR, PC, LR 
    /*000002a4*/ ADD         IP, PC, IP 
    /*000002a8*/ ADD         R3, PC, R3 
    /*000002ac*/ STR         LR, [R4, #8] 
    /*000002b0*/ STR         IP, [R4, #20] 
    /*000002b4*/ ADD         R1, PC, R1 
    /*000002b8*/ STR         R3, [R4, #60]               @ 0x3c 
    /*000002bc*/ LDR         LR, WORD_03e4               @ LDR         LR, [PC, #288]              @ 0x00000000000003e4 
    /*000002c0*/ LDR         IP, WORD_03e8               @ LDR         IP, [PC, #288]              @ 0x00000000000003e8 
    /*000002c4*/ LDR         R3, WORD_03ec               @ LDR         R3, [PC, #288]              @ 0x00000000000003ec 
    /*000002c8*/ STR         R1, [R4, #36]               @ 0x24 
    /*000002cc*/ LDR         R1, WORD_03f0               @ LDR         R1, [PC, #284]              @ 0x00000000000003f0 
    /*000002d0*/ ADD         R3, PC, R3 
    /*000002d4*/ ADD         LR, PC, LR 
    /*000002d8*/ ADD         IP, PC, IP 
    /*000002dc*/ STR         R3, [R4, #80]               @ 0x50 
    /*000002e0*/ STR         LR, [R4, #48]               @ 0x30 
    /*000002e4*/ STR         IP, [R4, #52]               @ 0x34 
    /*000002e8*/ ADD         R1, PC, R1 
    /*000002ec*/ BL          memcpy
    /*000002f0*/ LDR         R3, [R5, #12] 
    /*000002f4*/ CMP         R3, #3 
    /*000002f8*/ BNE         cmos_get_isp_default_x188
    cmos_get_isp_default_x2fc:
    /*000002fc*/ MOV         R3, #64                     @ 0x40 
    /*00000300*/ STR         R3, [R4, #760]              @ 0x2f8 
    /*00000304*/ STR         R3, [R4, #764]              @ 0x2fc 
    /*00000308*/ STR         R3, [R4, #768]              @ 0x300 
    /*0000030c*/ B           cmos_get_isp_default_x1a0
    cmos_get_isp_default_x310:
    /*00000310*/ LDR         R1, WORD_03f4               @ LDR         R1, [PC, #220]              @ 0x00000000000003f4 
    /*00000314*/ MOVW        R3, #1293                   @ 0x50d 
    /*00000318*/ LDR         R2, WORD_03f8               @ LDR         R2, [PC, #216]              @ 0x00000000000003f8 
    /*0000031c*/ LDR         R0, [IP, R1] 
    /*00000320*/ LDR         R1, WORD_03fc               @ LDR         R1, [PC, #212]              @ 0x00000000000003fc 
    /*00000324*/ ADD         R2, PC, R2 
    /*00000328*/ ADD         R1, PC, R1 
    /*0000032c*/ LDR         R0, [R0] 
    /*00000330*/ BL          fprintf
    /*00000334*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000338*/ MOVT        R0, #40988                  @ 0xa01c 
    /*0000033c*/ POP         {R4, R5, R6, PC} 
    cmos_get_isp_default_x340:
    /*00000340*/ LDR         R1, WORD_03f4               @ LDR         R1, [PC, #172]              @ 0x00000000000003f4 
    /*00000344*/ MOVW        R3, #1291                   @ 0x50b 
    /*00000348*/ LDR         R2, WORD_0400               @ LDR         R2, [PC, #176]              @ 0x0000000000000400 
    /*0000034c*/ LDR         R0, [IP, R1] 
    /*00000350*/ LDR         R1, WORD_0404               @ LDR         R1, [PC, #172]              @ 0x0000000000000404 
    /*00000354*/ ADD         R2, PC, R2 
    /*00000358*/ ADD         R1, PC, R1 
    /*0000035c*/ LDR         R0, [R0] 
    /*00000360*/ BL          fprintf
    /*00000364*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000368*/ MOVT        R0, #40988                  @ 0xa01c 
    /*0000036c*/ POP         {R4, R5, R6, PC} 
    /*00000370*/ WORD_0370: .word 0x0000035c
    /*00000374*/ WORD_0374: .word 0x00000000
    /*00000378*/ WORD_0378: .word 0x00000310
    /*0000037c*/ WORD_037c: .word 0x00000310
    /*00000380*/ WORD_0380: .word 0x0000030c
    /*00000384*/ WORD_0384: .word 0x000002ec
    /*00000388*/ WORD_0388: .word 0x000002ec
    /*0000038c*/ WORD_038c: .word 0x000002ec
    /*00000390*/ WORD_0390: .word 0x000002ec
    /*00000394*/ WORD_0394: .word 0x000002ec
    /*00000398*/ WORD_0398: .word 0x000002a8
    /*0000039c*/ WORD_039c: .word 0x00000290
    /*000003a0*/ WORD_03a0: .word 0x00000288
    /*000003a4*/ WORD_03a4: .word 0x00000270
    /*000003a8*/ WORD_03a8: .word 0x00000270
    /*000003ac*/ WORD_03ac: .word 0x00000270
    /*000003b0*/ WORD_03b0: .word 0x00000268
    /*000003b4*/ WORD_03b4: .word 0x0000024c
    /*000003b8*/ WORD_03b8: .word 0x0000024c
    /*000003bc*/ WORD_03bc: .word 0x00000258
    /*000003c0*/ WORD_03c0: .word 0x00000244
    /*000003c4*/ WORD_03c4: .word 0x0000020c
    /*000003c8*/ WORD_03c8: .word 0x00000164
    /*000003cc*/ WORD_03cc: .word 0x0000014c
    /*000003d0*/ WORD_03d0: .word 0x00000144
    /*000003d4*/ WORD_03d4: .word 0x0000012c
    /*000003d8*/ WORD_03d8: .word 0x0000012c
    /*000003dc*/ WORD_03dc: .word 0x0000012c
    /*000003e0*/ WORD_03e0: .word 0x00000124
    /*000003e4*/ WORD_03e4: .word 0x00000108
    /*000003e8*/ WORD_03e8: .word 0x00000108
    /*000003ec*/ WORD_03ec: .word 0x00000114
    /*000003f0*/ WORD_03f0: .word 0x00000100
    /*000003f4*/ WORD_03f4: .word 0x00000000
    /*000003f8*/ WORD_03f8: .word 0x000000cc
    /*000003fc*/ WORD_03fc: .word 0x000000cc
    /*00000400*/ WORD_0400: .word 0x000000a4
    /*00000404*/ WORD_0404: .word 0x000000a4
FUNC_END cmos_get_isp_default


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_get_ae_default
@ Size: 0x57c
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_get_ae_default
    /*00000000*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000004*/ SUBS        R4, R1, #0 
    /*00000008*/ LDR         IP, WORD_038c               @ LDR         IP, [PC, #892]              @ 0x000000000000038c 
    /*0000000c*/ VPUSH       {D8} 
    /*00000010*/ ADD         IP, PC, IP 
    /*00000014*/ BEQ         cmos_get_ae_default_x54c
    /*00000018*/ LDR         R3, WORD_0390               @ LDR         R3, [PC, #880]              @ 0x0000000000000390 
    /*0000001c*/ MOV         R6, R0 
    /*00000020*/ LDR         R3, [IP, R3] 
    /*00000024*/ LDR         R5, [R3, R0, LSL #2] 
    /*00000028*/ CMP         R5, #0 
    /*0000002c*/ BEQ         cmos_get_ae_default_x51c
    /*00000030*/ MOV         R2, #260                    @ 0x104 
    /*00000034*/ MOV         R1, #0 
    /*00000038*/ ADD         R0, R4, #156                @ 0x9c 
    /*0000003c*/ BL          memset
    /*00000040*/ VLDR        S16, [R4, #16] 
    /*00000044*/ MOV         R1, #0 
    /*00000048*/ VLDR        S14, [PC, #820]             @ 0x0000000000000384 
    /*0000004c*/ MOV         R3, #25 
    /*00000050*/ VLDR        S15, [PC, #816]             @ 0x0000000000000388 
    /*00000054*/ MOVW        R0, #62390                  @ 0xf3b6 
    /*00000058*/ VCMP.F32    S16, S14 
    /*0000005c*/ MOVW        IP, #65535                  @ 0xffff 
    /*00000060*/ LDR         R2, [R5, #2136]             @ 0x858 
    /*00000064*/ MOV         LR, #8 
    /*00000068*/ STR         R1, [R4, #12] 
    /*0000006c*/ MOV         R1, #1 
    /*00000070*/ MUL         R3, R3, R2 
    /*00000074*/ VMRS        APSR_NZCV, FPSCR 
    /*00000078*/ VCMP.F32    S16, S15 
    /*0000007c*/ LSR         R3, R3, R1 
    /*00000080*/ STR         R3, [R4, #8] 
    /*00000084*/ MOVW        R3, #36700                  @ 0x8f5c 
    /*00000088*/ MOVT        R3, #16066                  @ 0x3ec2 
    /*0000008c*/ MOVEQ       R7, R1 
    /*00000090*/ STR         R1, [R4, #64]               @ 0x40 
    /*00000094*/ MOVNE       R7, #0 
    /*00000098*/ VMRS        APSR_NZCV, FPSCR 
    /*0000009c*/ MOV         R1, #2048                   @ 0x800 
    /*000000a0*/ STR         R3, [R4, #96]               @ 0x60 
    /*000000a4*/ STR         R3, [R4, #124]              @ 0x7c 
    /*000000a8*/ MOV         R3, #921600                 @ 0xe1000 
    /*000000ac*/ STR         R3, [R4, #24] 
    /*000000b0*/ MOVT        R0, #16205                  @ 0x3f4d 
    /*000000b4*/ ORREQ       R7, R7, #1 
    /*000000b8*/ STR         R1, [R4, #132]              @ 0x84 
    /*000000bc*/ CMP         R7, #0 
    /*000000c0*/ LDR         R1, WORD_0394               @ LDR         R1, [PC, #716]              @ 0x0000000000000394 
    /*000000c4*/ MOVWNE      R3, #45823                  @ 0xb2ff 
    /*000000c8*/ STR         R0, [R4, #72]               @ 0x48 
    /*000000cc*/ MOV         R0, #256                    @ 0x100 
    /*000000d0*/ MOVTNE      R3, #16155                  @ 0x3f1b 
    /*000000d4*/ STR         R2, [R4, #36]               @ 0x24 
    /*000000d8*/ MOV         R2, #2 
    /*000000dc*/ STR         IP, [R4, #40]               @ 0x28 
    /*000000e0*/ MOV         IP, #1065353216             @ 0x3f800000 
    /*000000e4*/ STR         R2, [R4, #92]               @ 0x5c 
    /*000000e8*/ STR         R2, [R4, #120]              @ 0x78 
    /*000000ec*/ MOV         R2, #2080                   @ 0x820 
    /*000000f0*/ STR         R0, [R4, #136]              @ 0x88 
    /*000000f4*/ ADD         R0, R4, #828                @ 0x33c 
    /*000000f8*/ STRNE       R3, [R4, #72]               @ 0x48 
    /*000000fc*/ STR         LR, [R4, #140]              @ 0x8c 
    /*00000100*/ STR         IP, [R4, #68]               @ 0x44 
    /*00000104*/ ADD         R1, PC, R1 
    /*00000108*/ BL          memcpy
    /*0000010c*/ LDR         R3, [R5, #12] 
    /*00000110*/ MOV         R2, #9 
    /*00000114*/ STR         R2, [R4, #2908]             @ 0xb5c 
    /*00000118*/ MOV         R2, #5 
    /*0000011c*/ STR         R2, [R4, #2912]             @ 0xb60 
    /*00000120*/ SUB         R3, R3, #3 
    /*00000124*/ MOV         R2, #0 
    /*00000128*/ STR         R2, [R4, #416]              @ 0x1a0 
    /*0000012c*/ STR         R2, [R4, #156]              @ 0x9c 
    /*00000130*/ STR         R2, [R4, #420]              @ 0x1a4 
    /*00000134*/ CMP         R3, #6 
    /*00000138*/ ADDLS       PC, PC, R3, LSL #2 
    /*0000013c*/ B           cmos_get_ae_default_x48c
    /*00000140*/ B           cmos_get_ae_default_x1f8
    /*00000144*/ B           cmos_get_ae_default_x2f4
    /*00000148*/ B           cmos_get_ae_default_x2f4
    /*0000014c*/ B           cmos_get_ae_default_x3b8
    /*00000150*/ B           cmos_get_ae_default_x48c
    /*00000154*/ B           cmos_get_ae_default_x48c
    /*00000158*/ B           cmos_get_ae_default_x15c
    cmos_get_ae_default_x15c:
    /*0000015c*/ LDR         R3, WORD_0398               @ LDR         R3, [PC, #564]              @ 0x0000000000000398 
    /*00000160*/ MOV         R2, #12 
    /*00000164*/ STRB        R2, [R4] 
    /*00000168*/ MOV         IP, #24 
    /*0000016c*/ ADD         R3, PC, R3 
    /*00000170*/ MOV         R0, #96                     @ 0x60 
    /*00000174*/ STRB        IP, [R4, #1] 
    /*00000178*/ MVN         R2, #127                    @ 0x7f 
    /*0000017c*/ LDR         R1, [R3, R6, LSL #2] 
    /*00000180*/ MOV         R3, #64                     @ 0x40 
    /*00000184*/ STRB        R0, [R4, #2] 
    /*00000188*/ MOVW        LR, #37329                  @ 0x91d1 
    /*0000018c*/ STRB        R2, [R4, #3] 
    /*00000190*/ CMP         R1, #1 
    /*00000194*/ STRB        R3, [R4, #4] 
    /*00000198*/ MOVW        R2, #32382                  @ 0x7e7e 
    /*0000019c*/ LDR         R1, [R5, #2136]             @ 0x858 
    /*000001a0*/ MOV         R3, #1024                   @ 0x400 
    /*000001a4*/ MOVT        LR, #15836                  @ 0x3ddc 
    /*000001a8*/ MOV         R0, #2 
    /*000001ac*/ STR         LR, [R4, #72]               @ 0x48 
    /*000001b0*/ MOVW        IP, #65535                  @ 0xffff 
    /*000001b4*/ STR         R0, [R4, #52]               @ 0x34 
    /*000001b8*/ SUB         R1, R1, #2 
    /*000001bc*/ STR         R1, [R4, #48]               @ 0x30 
    /*000001c0*/ STR         R0, [R4, #60]               @ 0x3c 
    /*000001c4*/ STR         IP, [R4, #56]               @ 0x38 
    /*000001c8*/ STR         R2, [R4, #76]               @ 0x4c 
    /*000001cc*/ STR         R2, [R4, #84]               @ 0x54 
    /*000001d0*/ STR         R2, [R4, #104]              @ 0x68 
    /*000001d4*/ STR         R2, [R4, #112]              @ 0x70 
    /*000001d8*/ STR         R3, [R4, #80]               @ 0x50 
    /*000001dc*/ STR         R3, [R4, #88]               @ 0x58 
    /*000001e0*/ STR         R3, [R4, #108]              @ 0x6c 
    /*000001e4*/ STR         R3, [R4, #116]              @ 0x74 
    /*000001e8*/ BEQ         cmos_get_ae_default_x470
    cmos_get_ae_default_x1ec:
    /*000001ec*/ MOV         R0, #0 
    cmos_get_ae_default_x1f0:
    /*000001f0*/ VPOP        {D8} 
    /*000001f4*/ POP         {R4, R5, R6, R7, R8, PC} 
    cmos_get_ae_default_x1f8:
    /*000001f8*/ ADD         R3, R4, #808                @ 0x328 
    /*000001fc*/ MOV         R2, #0 
    /*00000200*/ CMP         R7, R2 
    /*00000204*/ MOVW        R1, #65535                  @ 0xffff 
    /*00000208*/ STRH        R2, [R3] 
    /*0000020c*/ MOV         R3, #12 
    /*00000210*/ STRB        R3, [R4] 
    /*00000214*/ MOV         R3, #24 
    /*00000218*/ STRB        R3, [R4, #1] 
    /*0000021c*/ MOV         R3, #96                     @ 0x60 
    /*00000220*/ STRB        R3, [R4, #2] 
    /*00000224*/ MVN         R3, #127                    @ 0x7f 
    /*00000228*/ STRB        R3, [R4, #3] 
    /*0000022c*/ MOV         R3, #28 
    /*00000230*/ STRB        R3, [R4, #4] 
    /*00000234*/ MOV         R2, #201                    @ 0xc9 
    /*00000238*/ LDR         R3, [R5, #2136]             @ 0x858 
    /*0000023c*/ STR         R2, [R4, #812]              @ 0x32c 
    /*00000240*/ MOV         R2, #1 
    /*00000244*/ STR         R2, [R4, #2916]             @ 0xb64 
    /*00000248*/ SUB         R3, R3, #2 
    /*0000024c*/ STR         R3, [R4, #48]               @ 0x30 
    /*00000250*/ MOV         R3, #2 
    /*00000254*/ STR         R3, [R4, #52]               @ 0x34 
    /*00000258*/ MOVW        R2, #32382                  @ 0x7e7e 
    /*0000025c*/ STR         R3, [R4, #60]               @ 0x3c 
    /*00000260*/ MOV         R3, #1024                   @ 0x400 
    /*00000264*/ STR         R1, [R4, #56]               @ 0x38 
    /*00000268*/ MOVW        R1, #8134                   @ 0x1fc6 
    /*0000026c*/ STR         R2, [R4, #76]               @ 0x4c 
    /*00000270*/ STR         R2, [R4, #104]              @ 0x68 
    /*00000274*/ STR         R3, [R4, #80]               @ 0x50 
    /*00000278*/ STR         R3, [R4, #88]               @ 0x58 
    /*0000027c*/ STR         R3, [R4, #108]              @ 0x6c 
    /*00000280*/ STR         R3, [R4, #112]              @ 0x70 
    /*00000284*/ STR         R3, [R4, #116]              @ 0x74 
    /*00000288*/ STR         R1, [R4, #84]               @ 0x54 
    /*0000028c*/ BNE         cmos_get_ae_default_x50c
    /*00000290*/ VMOV.F32    S14, #62                    @ 0x41f00000 30.0 
    /*00000294*/ VMOV.F32    S15, #57                    @ 0x41c80000 25.0 
    /*00000298*/ VCMP.F32    S16, S14 
    /*0000029c*/ VMRS        APSR_NZCV, FPSCR 
    /*000002a0*/ VCMP.F32    S16, S15 
    /*000002a4*/ MOVEQ       R3, #1 
    /*000002a8*/ MOVNE       R3, #0 
    /*000002ac*/ VMRS        APSR_NZCV, FPSCR 
    /*000002b0*/ ORREQ       R3, R3, #1 
    /*000002b4*/ CMP         R3, #0 
    /*000002b8*/ MOVWNE      R3, #18455                  @ 0x4817 
    /*000002bc*/ MOVTNE      R3, #16016                  @ 0x3e90 
    /*000002c0*/ STRNE       R3, [R4, #72]               @ 0x48 
    cmos_get_ae_default_x2c4:
    /*000002c4*/ LDR         R3, WORD_039c               @ LDR         R3, [PC, #208]              @ 0x000000000000039c 
    /*000002c8*/ ADD         R3, PC, R3 
    /*000002cc*/ LDR         R3, [R3, R6, LSL #2] 
    /*000002d0*/ CMP         R3, #1 
    /*000002d4*/ BNE         cmos_get_ae_default_x1ec
    /*000002d8*/ MOVW        R3, #32382                  @ 0x7e7e 
    /*000002dc*/ MOV         R2, #56                     @ 0x38 
    /*000002e0*/ STR         R3, [R4, #84]               @ 0x54 
    /*000002e4*/ MOV         R0, #0 
    /*000002e8*/ STR         R3, [R4, #112]              @ 0x70 
    /*000002ec*/ STRB        R2, [R4, #4] 
    /*000002f0*/ B           cmos_get_ae_default_x1f0
    cmos_get_ae_default_x2f4:
    /*000002f4*/ MOV         R2, #12 
    /*000002f8*/ MOV         R3, #24 
    /*000002fc*/ STRB        R2, [R4] 
    /*00000300*/ MOV         R1, #96                     @ 0x60 
    /*00000304*/ STRB        R1, [R4, #2] 
    /*00000308*/ MVN         R2, #127                    @ 0x7f 
    /*0000030c*/ STRB        R2, [R4, #3] 
    /*00000310*/ ADD         LR, R4, #808                @ 0x328 
    /*00000314*/ STRB        R3, [R4, #1] 
    /*00000318*/ MOVW        R2, #32382                  @ 0x7e7e 
    /*0000031c*/ STRB        R3, [R4, #4] 
    /*00000320*/ MOV         R0, #2 
    /*00000324*/ LDR         R3, [R5, #2136]             @ 0x858 
    /*00000328*/ MOV         R1, #1 
    /*0000032c*/ VPOP        {D8} 
    /*00000330*/ STR         R0, [R4, #52]               @ 0x34 
    /*00000334*/ SUB         R3, R3, #2 
    /*00000338*/ STR         R3, [R4, #48]               @ 0x30 
    /*0000033c*/ MOVW        R5, #65535                  @ 0xffff 
    /*00000340*/ STR         R0, [R4, #60]               @ 0x3c 
    /*00000344*/ MOV         R3, #1024                   @ 0x400 
    /*00000348*/ STR         R1, [R4, #2916]             @ 0xb64 
    /*0000034c*/ MOV         IP, #512                    @ 0x200 
    /*00000350*/ STR         R5, [R4, #56]               @ 0x38 
    /*00000354*/ MOV         R0, #0 
    /*00000358*/ STR         R2, [R4, #76]               @ 0x4c 
    /*0000035c*/ STR         R2, [R4, #84]               @ 0x54 
    /*00000360*/ STR         R2, [R4, #104]              @ 0x68 
    /*00000364*/ STR         R2, [R4, #112]              @ 0x70 
    /*00000368*/ STR         R3, [R4, #80]               @ 0x50 
    /*0000036c*/ STR         R3, [R4, #88]               @ 0x58 
    /*00000370*/ STR         R3, [R4, #108]              @ 0x6c 
    /*00000374*/ STR         R3, [R4, #116]              @ 0x74 
    /*00000378*/ STRH        R1, [LR] 
    /*0000037c*/ STR         IP, [R4, #812]              @ 0x32c 
    /*00000380*/ POP         {R4, R5, R6, R7, R8, PC} 
    /*00000384*/ SUBMI       R0, R8, #0 
    /*00000388*/ RSBSMI      R0, R0, #0 
    /*0000038c*/ WORD_038c: .word 0x00000374
    /*00000390*/ WORD_0390: .word 0x00000000
    /*00000394*/ WORD_0394: .word 0x00000288
    /*00000398*/ WORD_0398: .word 0x00000224
    /*0000039c*/ WORD_039c: .word 0x000000cc
    /*000003a0*/ WORD_03a0: .word 0xffffffd0
    /*000003a4*/ WORD_03a4: .word 0xfffffe6c
    /*000003a8*/ WORD_03a8: .word 0xfffffe6c
    /*000003ac*/ WORD_03ac: .word 0x00000000
    /*000003b0*/ WORD_03b0: .word 0xfffffe48
    /*000003b4*/ WORD_03b4: .word 0xfffffe48
    cmos_get_ae_default_x3b8:
    /*000003b8*/ LDR         R3, WORD_03a0               @ LDR         R3, [PC, #-32]              @ 0x00000000000003a0 
    /*000003bc*/ MOV         R2, #12 
    /*000003c0*/ STRB        R2, [R4] 
    /*000003c4*/ MOV         LR, #24 
    /*000003c8*/ ADD         R3, PC, R3 
    /*000003cc*/ MOV         R0, #96                     @ 0x60 
    /*000003d0*/ STRB        LR, [R4, #1] 
    /*000003d4*/ MVN         R2, #127                    @ 0x7f 
    /*000003d8*/ LDR         R1, [R3, R6, LSL #2] 
    /*000003dc*/ MOV         R3, #28 
    /*000003e0*/ STRB        R0, [R4, #2] 
    /*000003e4*/ MOVW        IP, #37329                  @ 0x91d1 
    /*000003e8*/ STRB        R2, [R4, #3] 
    /*000003ec*/ MOV         R0, #2 
    /*000003f0*/ STRB        R3, [R4, #4] 
    /*000003f4*/ MOV         R2, #1 
    /*000003f8*/ LDR         R3, [R5, #2136]             @ 0x858 
    /*000003fc*/ CMP         R1, #1 
    /*00000400*/ MOVT        IP, #15836                  @ 0x3ddc 
    /*00000404*/ ADD         R1, R4, #808                @ 0x328 
    /*00000408*/ STR         IP, [R4, #72]               @ 0x48 
    /*0000040c*/ SUB         R3, R3, #2 
    /*00000410*/ STR         R2, [R4, #2916]             @ 0xb64 
    /*00000414*/ MOVW        IP, #65535                  @ 0xffff 
    /*00000418*/ STR         R3, [R4, #48]               @ 0x30 
    /*0000041c*/ MOVW        R2, #32382                  @ 0x7e7e 
    /*00000420*/ STR         R0, [R4, #52]               @ 0x34 
    /*00000424*/ MOV         R3, #1024                   @ 0x400 
    /*00000428*/ STR         R0, [R4, #60]               @ 0x3c 
    /*0000042c*/ MOV         LR, #0 
    /*00000430*/ STR         IP, [R4, #56]               @ 0x38 
    /*00000434*/ MOV         R0, #512                    @ 0x200 
    /*00000438*/ STR         R2, [R4, #76]               @ 0x4c 
    /*0000043c*/ MOV         IP, #64                     @ 0x40 
    /*00000440*/ STR         R2, [R4, #84]               @ 0x54 
    /*00000444*/ STR         R2, [R4, #104]              @ 0x68 
    /*00000448*/ STR         R2, [R4, #112]              @ 0x70 
    /*0000044c*/ STR         R3, [R4, #80]               @ 0x50 
    /*00000450*/ STR         R3, [R4, #88]               @ 0x58 
    /*00000454*/ STR         R3, [R4, #108]              @ 0x6c 
    /*00000458*/ STR         R3, [R4, #116]              @ 0x74 
    /*0000045c*/ STRH        LR, [R1] 
    /*00000460*/ STR         R0, [R4, #812]              @ 0x32c 
    /*00000464*/ STR         R0, [R4, #816]              @ 0x330 
    /*00000468*/ STR         IP, [R4, #820]              @ 0x334 
    /*0000046c*/ BNE         cmos_get_ae_default_x1ec
    cmos_get_ae_default_x470:
    /*00000470*/ MOV         R2, #56                     @ 0x38 
    /*00000474*/ MOV         R3, #0 
    /*00000478*/ STRB        R2, [R4, #4] 
    /*0000047c*/ MOV         R0, R3 
    /*00000480*/ VPOP        {D8} 
    /*00000484*/ STR         R3, [R4, #2916]             @ 0xb64 
    /*00000488*/ POP         {R4, R5, R6, R7, R8, PC} 
    cmos_get_ae_default_x48c:
    /*0000048c*/ MOV         R3, #13 
    /*00000490*/ MOV         R2, #40                     @ 0x28 
    /*00000494*/ STRB        R3, [R4] 
    /*00000498*/ MOV         R3, #96                     @ 0x60 
    /*0000049c*/ STRB        R2, [R4, #1] 
    /*000004a0*/ MVN         R2, #127                    @ 0x7f 
    /*000004a4*/ STRB        R3, [R4, #2] 
    /*000004a8*/ MOV         R3, #56                     @ 0x38 
    /*000004ac*/ STRB        R2, [R4, #3] 
    /*000004b0*/ MOV         R0, #0 
    /*000004b4*/ STRB        R3, [R4, #4] 
    /*000004b8*/ MOVW        R2, #32382                  @ 0x7e7e 
    /*000004bc*/ LDR         R3, [R5, #2136]             @ 0x858 
    /*000004c0*/ MOV         R1, #2 
    /*000004c4*/ VPOP        {D8} 
    /*000004c8*/ STR         R0, [R4, #2916]             @ 0xb64 
    /*000004cc*/ SUB         R3, R3, #2 
    /*000004d0*/ STR         R3, [R4, #48]               @ 0x30 
    /*000004d4*/ MOVW        IP, #65535                  @ 0xffff 
    /*000004d8*/ STR         R1, [R4, #52]               @ 0x34 
    /*000004dc*/ MOV         R3, #1024                   @ 0x400 
    /*000004e0*/ STR         R1, [R4, #60]               @ 0x3c 
    /*000004e4*/ STR         IP, [R4, #56]               @ 0x38 
    /*000004e8*/ STR         R2, [R4, #76]               @ 0x4c 
    /*000004ec*/ STR         R2, [R4, #84]               @ 0x54 
    /*000004f0*/ STR         R2, [R4, #104]              @ 0x68 
    /*000004f4*/ STR         R2, [R4, #112]              @ 0x70 
    /*000004f8*/ STR         R3, [R4, #80]               @ 0x50 
    /*000004fc*/ STR         R3, [R4, #88]               @ 0x58 
    /*00000500*/ STR         R3, [R4, #108]              @ 0x6c 
    /*00000504*/ STR         R3, [R4, #116]              @ 0x74 
    /*00000508*/ POP         {R4, R5, R6, R7, R8, PC} 
    cmos_get_ae_default_x50c:
    /*0000050c*/ MOVW        R3, #44040                  @ 0xac08 
    /*00000510*/ MOVT        R3, #15964                  @ 0x3e5c 
    /*00000514*/ STR         R3, [R4, #72]               @ 0x48 
    /*00000518*/ B           cmos_get_ae_default_x2c4
    cmos_get_ae_default_x51c:
    /*0000051c*/ LDR         R1, WORD_03ac               @ LDR         R1, [PC, #-376]             @ 0x00000000000003ac 
    /*00000520*/ MOV         R3, #122                    @ 0x7a 
    /*00000524*/ LDR         R2, WORD_03a4               @ LDR         R2, [PC, #-392]             @ 0x00000000000003a4 
    /*00000528*/ LDR         R0, [IP, R1] 
    /*0000052c*/ LDR         R1, WORD_03a8               @ LDR         R1, [PC, #-396]             @ 0x00000000000003a8 
    /*00000530*/ ADD         R2, PC, R2 
    /*00000534*/ ADD         R1, PC, R1 
    /*00000538*/ LDR         R0, [R0] 
    /*0000053c*/ BL          fprintf
    /*00000540*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000544*/ MOVT        R0, #40988                  @ 0xa01c 
    /*00000548*/ B           cmos_get_ae_default_x1f0
    cmos_get_ae_default_x54c:
    /*0000054c*/ LDR         R1, WORD_03ac               @ LDR         R1, [PC, #-424]             @ 0x00000000000003ac 
    /*00000550*/ MOV         R3, #120                    @ 0x78 
    /*00000554*/ LDR         R2, WORD_03b0               @ LDR         R2, [PC, #-428]             @ 0x00000000000003b0 
    /*00000558*/ LDR         R0, [IP, R1] 
    /*0000055c*/ LDR         R1, WORD_03b4               @ LDR         R1, [PC, #-432]             @ 0x00000000000003b4 
    /*00000560*/ ADD         R2, PC, R2 
    /*00000564*/ ADD         R1, PC, R1 
    /*00000568*/ LDR         R0, [R0] 
    /*0000056c*/ BL          fprintf
    /*00000570*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000574*/ MOVT        R0, #40988                  @ 0xa01c 
    /*00000578*/ B           cmos_get_ae_default_x1f0
FUNC_END cmos_get_ae_default


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_get_awb_default
@ Size: 0x1b8
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_get_awb_default
    /*00000000*/ PUSH        {R4, R5, R6, LR} 
    /*00000004*/ SUBS        R4, R1, #0 
    /*00000008*/ LDR         IP, WORD_018c               @ LDR         IP, [PC, #380]              @ 0x000000000000018c 
    /*0000000c*/ ADD         IP, PC, IP 
    /*00000010*/ BEQ         cmos_get_awb_default_x15c
    /*00000014*/ LDR         R3, WORD_0190               @ LDR         R3, [PC, #372]              @ 0x0000000000000190 
    /*00000018*/ LDR         R3, [IP, R3] 
    /*0000001c*/ LDR         R5, [R3, R0, LSL #2] 
    /*00000020*/ CMP         R5, #0 
    /*00000024*/ BEQ         cmos_get_awb_default_x12c
    /*00000028*/ MOV         R2, #216                    @ 0xd8 
    /*0000002c*/ MOV         R1, #0 
    /*00000030*/ MOV         R0, R4 
    /*00000034*/ BL          memset
    /*00000038*/ LDR         R3, [R5, #12] 
    /*0000003c*/ MOVW        R1, #43463                  @ 0xa9c7 
    /*00000040*/ MOVT        R1, #2 
    /*00000044*/ MOVW        R2, #7582                   @ 0x1d9e 
    /*00000048*/ MOVT        R2, #65534                  @ 0xfffe 
    /*0000004c*/ MOVW        R0, #5000                   @ 0x1388 
    /*00000050*/ STR         R1, [R4, #24] 
    /*00000054*/ MOV         R1, #496                    @ 0x1f0 
    /*00000058*/ STR         R2, [R4, #32] 
    /*0000005c*/ SUB         R3, R3, #3 
    /*00000060*/ STRH        R1, [R4, #2] 
    /*00000064*/ MOV         R2, #256                    @ 0x100 
    /*00000068*/ STRH        R0, [R4] 
    /*0000006c*/ MOV         R1, #416                    @ 0x1a0 
    /*00000070*/ STRH        R2, [R4, #4] 
    /*00000074*/ MOV         IP, #41                     @ 0x29 
    /*00000078*/ STRH        R2, [R4, #6] 
    /*0000007c*/ MOV         R0, #127                    @ 0x7f 
    /*00000080*/ STRH        R1, [R4, #8] 
    /*00000084*/ MOV         R2, #128                    @ 0x80 
    /*00000088*/ STR         IP, [R4, #12] 
    /*0000008c*/ MVN         R1, #86                     @ 0x56 
    /*00000090*/ STR         R0, [R4, #16] 
    /*00000094*/ STR         R1, [R4, #20] 
    /*00000098*/ STR         R2, [R4, #28] 
    /*0000009c*/ CMP         R3, #6 
    /*000000a0*/ ADDLS       PC, PC, R3, LSL #2 
    /*000000a4*/ B           cmos_get_awb_default_x10c
    /*000000a8*/ B           cmos_get_awb_default_xc4
    /*000000ac*/ B           cmos_get_awb_default_xc4
    /*000000b0*/ B           cmos_get_awb_default_x10c
    /*000000b4*/ B           cmos_get_awb_default_xc4
    /*000000b8*/ B           cmos_get_awb_default_x10c
    /*000000bc*/ B           cmos_get_awb_default_x10c
    /*000000c0*/ B           cmos_get_awb_default_xc4
    cmos_get_awb_default_xc4:
    /*000000c4*/ LDR         R1, WORD_0194               @ LDR         R1, [PC, #200]              @ 0x0000000000000194 
    /*000000c8*/ MOV         R2, #142                    @ 0x8e 
    /*000000cc*/ ADD         R0, R4, #64                 @ 0x40 
    /*000000d0*/ ADD         R1, PC, R1 
    /*000000d4*/ BL          memcpy
    /*000000d8*/ LDR         IP, WORD_0198               @ LDR         IP, [PC, #184]              @ 0x0000000000000198 
    /*000000dc*/ ADD         IP, PC, IP 
    cmos_get_awb_default_xe0:
    /*000000e0*/ LDM         IP!, {R0, R1, R2, R3} 
    /*000000e4*/ ADD         LR, R4, #44                 @ 0x2c 
    /*000000e8*/ MOV         R5, #0 
    /*000000ec*/ STR         R0, [R4, #44]               @ 0x2c 
    /*000000f0*/ LDR         R0, [IP] 
    /*000000f4*/ STR         R1, [R4, #48]               @ 0x30 
    /*000000f8*/ STR         R0, [R4, #60]               @ 0x3c 
    /*000000fc*/ STR         R2, [R4, #52]               @ 0x34 
    /*00000100*/ STR         R3, [R4, #56]               @ 0x38 
    cmos_get_awb_default_x104:
    /*00000104*/ MOV         R0, R5 
    /*00000108*/ POP         {R4, R5, R6, PC} 
    cmos_get_awb_default_x10c:
    /*0000010c*/ LDR         R1, WORD_019c               @ LDR         R1, [PC, #136]              @ 0x000000000000019c 
    /*00000110*/ MOV         R2, #142                    @ 0x8e 
    /*00000114*/ ADD         R0, R4, #64                 @ 0x40 
    /*00000118*/ ADD         R1, PC, R1 
    /*0000011c*/ BL          memcpy
    /*00000120*/ LDR         IP, WORD_01a0               @ LDR         IP, [PC, #120]              @ 0x00000000000001a0 
    /*00000124*/ ADD         IP, PC, IP 
    /*00000128*/ B           cmos_get_awb_default_xe0
    cmos_get_awb_default_x12c:
    /*0000012c*/ LDR         R1, WORD_01a4               @ LDR         R1, [PC, #112]              @ 0x00000000000001a4 
    /*00000130*/ MOVW        R3, #1231                   @ 0x4cf 
    /*00000134*/ LDR         R2, WORD_01a8               @ LDR         R2, [PC, #108]              @ 0x00000000000001a8 
    /*00000138*/ MOVW        R5, #32774                  @ 0x8006 
    /*0000013c*/ LDR         R0, [IP, R1] 
    /*00000140*/ LDR         R1, WORD_01ac               @ LDR         R1, [PC, #100]              @ 0x00000000000001ac 
    /*00000144*/ ADD         R2, PC, R2 
    /*00000148*/ ADD         R1, PC, R1 
    /*0000014c*/ LDR         R0, [R0] 
    /*00000150*/ MOVT        R5, #40988                  @ 0xa01c 
    /*00000154*/ BL          fprintf
    /*00000158*/ B           cmos_get_awb_default_x104
    cmos_get_awb_default_x15c:
    /*0000015c*/ LDR         R1, WORD_01a4               @ LDR         R1, [PC, #64]               @ 0x00000000000001a4 
    /*00000160*/ MOVW        R3, #1229                   @ 0x4cd 
    /*00000164*/ LDR         R2, WORD_01b0               @ LDR         R2, [PC, #68]               @ 0x00000000000001b0 
    /*00000168*/ MOVW        R5, #32774                  @ 0x8006 
    /*0000016c*/ LDR         R0, [IP, R1] 
    /*00000170*/ LDR         R1, WORD_01b4               @ LDR         R1, [PC, #60]               @ 0x00000000000001b4 
    /*00000174*/ ADD         R2, PC, R2 
    /*00000178*/ ADD         R1, PC, R1 
    /*0000017c*/ LDR         R0, [R0] 
    /*00000180*/ MOVT        R5, #40988                  @ 0xa01c 
    /*00000184*/ BL          fprintf
    /*00000188*/ B           cmos_get_awb_default_x104
    /*0000018c*/ WORD_018c: .word 0x00000178
    /*00000190*/ WORD_0190: .word 0x00000000
    /*00000194*/ WORD_0194: .word 0x000000bc
    /*00000198*/ WORD_0198: .word 0x000000b4
    /*0000019c*/ WORD_019c: .word 0x0000007c
    /*000001a0*/ WORD_01a0: .word 0x00000074
    /*000001a4*/ WORD_01a4: .word 0x00000000
    /*000001a8*/ WORD_01a8: .word 0x0000005c
    /*000001ac*/ WORD_01ac: .word 0x0000005c
    /*000001b0*/ WORD_01b0: .word 0x00000034
    /*000001b4*/ WORD_01b4: .word 0x00000034
FUNC_END cmos_get_awb_default


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_set_wdr_mode
@ Size: 0x248
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_set_wdr_mode
    /*00000000*/ LDR         IP, WORD_0214               @ LDR         IP, [PC, #524]              @ 0x0000000000000214 
    /*00000004*/ LDR         R3, WORD_0218               @ LDR         R3, [PC, #524]              @ 0x0000000000000218 
    /*00000008*/ ADD         IP, PC, IP 
    /*0000000c*/ PUSH        {R4, LR} 
    /*00000010*/ LDR         R3, [IP, R3] 
    /*00000014*/ LDR         R4, [R3, R0, LSL #2] 
    /*00000018*/ CMP         R4, #0 
    /*0000001c*/ BEQ         cmos_set_wdr_mode_x1e4
    /*00000020*/ MOV         R3, #0 
    /*00000024*/ STR         R3, [R4, #4] 
    /*00000028*/ CMP         R1, #9 
    /*0000002c*/ ADDLS       PC, PC, R1, LSL #2 
    /*00000030*/ B           cmos_set_wdr_mode_x194
    /*00000034*/ B           cmos_set_wdr_mode_xa8
    /*00000038*/ B           cmos_set_wdr_mode_x194
    /*0000003c*/ B           cmos_set_wdr_mode_x194
    /*00000040*/ B           cmos_set_wdr_mode_xf0
    /*00000044*/ B           cmos_set_wdr_mode_x110
    /*00000048*/ B           cmos_set_wdr_mode_x13c
    /*0000004c*/ B           cmos_set_wdr_mode_x168
    /*00000050*/ B           cmos_set_wdr_mode_x194
    /*00000054*/ B           cmos_set_wdr_mode_x194
    /*00000058*/ B           cmos_set_wdr_mode_x5c
    cmos_set_wdr_mode_x5c:
    /*0000005c*/ LDRB        R3, [R4, #8] 
    /*00000060*/ LDR         R0, WORD_021c               @ LDR         R0, [PC, #436]              @ 0x000000000000021c 
    /*00000064*/ CMP         R3, #2 
    /*00000068*/ ADD         R0, PC, R0 
    /*0000006c*/ MOVWEQ      R3, #1126                   @ 0x466 
    /*00000070*/ MOVWNE      R3, #751                    @ 0x2ef 
    /*00000074*/ STR         R3, [R4, #2136]             @ 0x858 
    /*00000078*/ MOV         R3, #9 
    /*0000007c*/ STR         R3, [R4, #12] 
    /*00000080*/ BL          puts
    cmos_set_wdr_mode_x84:
    /*00000084*/ VMOV.I32    Q8, #0                      @ 0x00000000 
    /*00000088*/ LDR         R2, [R4, #2136]             @ 0x858 
    /*0000008c*/ ADD         R3, R4, #2128               @ 0x850 
    /*00000090*/ ADD         R3, R3, #12 
    /*00000094*/ MOV         R0, #0 
    /*00000098*/ STR         R2, [R4, #2128]             @ 0x850 
    /*0000009c*/ STR         R2, [R4, #2132]             @ 0x854 
    /*000000a0*/ VST1.8      {D16-D17}, [R3] 
    /*000000a4*/ POP         {R4, PC} 
    cmos_set_wdr_mode_xa8:
    /*000000a8*/ LDRB        R3, [R4, #8] 
    /*000000ac*/ CMP         R3, #2 
    /*000000b0*/ BEQ         cmos_set_wdr_mode_x1c0
    /*000000b4*/ CMP         R3, #1 
    /*000000b8*/ BEQ         cmos_set_wdr_mode_x1c0
    /*000000bc*/ CMP         R3, #3 
    /*000000c0*/ MOVWEQ      R3, #751                    @ 0x2ef 
    /*000000c4*/ STREQ       R3, [R4, #2136]             @ 0x858 
    /*000000c8*/ BEQ         cmos_set_wdr_mode_xd8
    /*000000cc*/ CMP         R3, #4 
    /*000000d0*/ MOVWEQ      R3, #563                    @ 0x233 
    /*000000d4*/ STREQ       R3, [R4, #2136]             @ 0x858 
    cmos_set_wdr_mode_xd8:
    /*000000d8*/ LDR         R0, WORD_0220               @ LDR         R0, [PC, #320]              @ 0x0000000000000220 
    /*000000dc*/ MOV         R3, #0 
    /*000000e0*/ STR         R3, [R4, #12] 
    /*000000e4*/ ADD         R0, PC, R0 
    /*000000e8*/ BL          puts
    /*000000ec*/ B           cmos_set_wdr_mode_x84
    cmos_set_wdr_mode_xf0:
    /*000000f0*/ LDR         R0, WORD_0224               @ LDR         R0, [PC, #300]              @ 0x0000000000000224 
    /*000000f4*/ MOVW        R3, #1251                   @ 0x4e3 
    /*000000f8*/ STR         R3, [R4, #2136]             @ 0x858 
    /*000000fc*/ MOV         R3, #3 
    /*00000100*/ STR         R3, [R4, #12] 
    /*00000104*/ ADD         R0, PC, R0 
    /*00000108*/ BL          puts
    /*0000010c*/ B           cmos_set_wdr_mode_x84
    cmos_set_wdr_mode_x110:
    /*00000110*/ LDRB        R3, [R4, #8] 
    /*00000114*/ CMP         R3, #2 
    /*00000118*/ BEQ         cmos_set_wdr_mode_x1cc
    /*0000011c*/ CMP         R3, #1 
    /*00000120*/ BEQ         cmos_set_wdr_mode_x1cc
    cmos_set_wdr_mode_x124:
    /*00000124*/ LDR         R0, WORD_0228               @ LDR         R0, [PC, #252]              @ 0x0000000000000228 
    /*00000128*/ MOV         R3, #4 
    /*0000012c*/ STR         R3, [R4, #12] 
    /*00000130*/ ADD         R0, PC, R0 
    /*00000134*/ BL          puts
    /*00000138*/ B           cmos_set_wdr_mode_x84
    cmos_set_wdr_mode_x13c:
    /*0000013c*/ LDRB        R3, [R4, #8] 
    /*00000140*/ CMP         R3, #2 
    /*00000144*/ BEQ         cmos_set_wdr_mode_x1d8
    /*00000148*/ CMP         R3, #1 
    /*0000014c*/ BEQ         cmos_set_wdr_mode_x1d8
    cmos_set_wdr_mode_x150:
    /*00000150*/ LDR         R0, WORD_022c               @ LDR         R0, [PC, #212]              @ 0x000000000000022c 
    /*00000154*/ MOV         R3, #5 
    /*00000158*/ STR         R3, [R4, #12] 
    /*0000015c*/ ADD         R0, PC, R0 
    /*00000160*/ BL          puts
    /*00000164*/ B           cmos_set_wdr_mode_x84
    cmos_set_wdr_mode_x168:
    /*00000168*/ LDRB        R3, [R4, #8] 
    /*0000016c*/ LDR         R0, WORD_0230               @ LDR         R0, [PC, #188]              @ 0x0000000000000230 
    /*00000170*/ CMP         R3, #2 
    /*00000174*/ ADD         R0, PC, R0 
    /*00000178*/ MOVWEQ      R3, #1501                   @ 0x5dd 
    /*0000017c*/ MOVWNE      R3, #751                    @ 0x2ef 
    /*00000180*/ STR         R3, [R4, #2136]             @ 0x858 
    /*00000184*/ MOV         R3, #6 
    /*00000188*/ STR         R3, [R4, #12] 
    /*0000018c*/ BL          puts
    /*00000190*/ B           cmos_set_wdr_mode_x84
    cmos_set_wdr_mode_x194:
    /*00000194*/ LDR         R1, WORD_0234               @ LDR         R1, [PC, #152]              @ 0x0000000000000234 
    /*00000198*/ MOVW        R3, #1615                   @ 0x64f 
    /*0000019c*/ LDR         R2, WORD_0238               @ LDR         R2, [PC, #148]              @ 0x0000000000000238 
    /*000001a0*/ LDR         R0, [IP, R1] 
    /*000001a4*/ LDR         R1, WORD_023c               @ LDR         R1, [PC, #144]              @ 0x000000000000023c 
    /*000001a8*/ ADD         R2, PC, R2 
    /*000001ac*/ ADD         R1, PC, R1 
    /*000001b0*/ LDR         R0, [R0] 
    /*000001b4*/ BL          fprintf
    /*000001b8*/ MVN         R0, #0 
    /*000001bc*/ POP         {R4, PC} 
    cmos_set_wdr_mode_x1c0:
    /*000001c0*/ MOVW        R3, #1126                   @ 0x466 
    /*000001c4*/ STR         R3, [R4, #2136]             @ 0x858 
    /*000001c8*/ B           cmos_set_wdr_mode_xd8
    cmos_set_wdr_mode_x1cc:
    /*000001cc*/ MOVW        R3, #1126                   @ 0x466 
    /*000001d0*/ STR         R3, [R4, #2136]             @ 0x858 
    /*000001d4*/ B           cmos_set_wdr_mode_x124
    cmos_set_wdr_mode_x1d8:
    /*000001d8*/ MOVW        R3, #1126                   @ 0x466 
    /*000001dc*/ STR         R3, [R4, #2136]             @ 0x858 
    /*000001e0*/ B           cmos_set_wdr_mode_x150
    cmos_set_wdr_mode_x1e4:
    /*000001e4*/ LDR         R1, WORD_0234               @ LDR         R1, [PC, #72]               @ 0x0000000000000234 
    /*000001e8*/ MOVW        R3, #1522                   @ 0x5f2 
    /*000001ec*/ LDR         R2, WORD_0240               @ LDR         R2, [PC, #76]               @ 0x0000000000000240 
    /*000001f0*/ LDR         R0, [IP, R1] 
    /*000001f4*/ LDR         R1, WORD_0244               @ LDR         R1, [PC, #72]               @ 0x0000000000000244 
    /*000001f8*/ ADD         R2, PC, R2 
    /*000001fc*/ ADD         R1, PC, R1 
    /*00000200*/ LDR         R0, [R0] 
    /*00000204*/ BL          fprintf
    /*00000208*/ MOVW        R0, #32774                  @ 0x8006 
    /*0000020c*/ MOVT        R0, #40988                  @ 0xa01c 
    /*00000210*/ POP         {R4, PC} 
    /*00000214*/ WORD_0214: .word 0x00000204
    /*00000218*/ WORD_0218: .word 0x00000000
    /*0000021c*/ WORD_021c: .word 0x000001ac
    /*00000220*/ WORD_0220: .word 0x00000134
    /*00000224*/ WORD_0224: .word 0x00000118
    /*00000228*/ WORD_0228: .word 0x000000f0
    /*0000022c*/ WORD_022c: .word 0x000000c8
    /*00000230*/ WORD_0230: .word 0x000000b4
    /*00000234*/ WORD_0234: .word 0x00000000
    /*00000238*/ WORD_0238: .word 0x00000088
    /*0000023c*/ WORD_023c: .word 0x00000088
    /*00000240*/ WORD_0240: .word 0x00000040
    /*00000244*/ WORD_0244: .word 0x00000040
FUNC_END cmos_set_wdr_mode


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_get_isp_black_level
@ Size: 0xc4
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_get_isp_black_level
    /*00000000*/ LDR         IP, WORD_00a8               @ LDR         IP, [PC, #160]              @ 0x00000000000000a8 
    /*00000004*/ CMP         R1, #0 
    /*00000008*/ PUSH        {R4, LR} 
    /*0000000c*/ ADD         IP, PC, IP 
    /*00000010*/ BEQ         cmos_get_isp_black_level_x48
    /*00000014*/ LDR         R3, WORD_00ac               @ LDR         R3, [PC, #144]              @ 0x00000000000000ac 
    /*00000018*/ LDR         R3, [IP, R3] 
    /*0000001c*/ LDR         R3, [R3, R0, LSL #2] 
    /*00000020*/ CMP         R3, #0 
    /*00000024*/ BEQ         cmos_get_isp_black_level_x78
    /*00000028*/ MOV         R3, #256                    @ 0x100 
    /*0000002c*/ MOV         R0, #0 
    /*00000030*/ STR         R0, [R1] 
    /*00000034*/ STRH        R3, [R1, #4] 
    /*00000038*/ STRH        R3, [R1, #6] 
    /*0000003c*/ STRH        R3, [R1, #8] 
    /*00000040*/ STRH        R3, [R1, #10] 
    /*00000044*/ POP         {R4, PC} 
    cmos_get_isp_black_level_x48:
    /*00000048*/ LDR         R1, WORD_00b0               @ LDR         R1, [PC, #96]               @ 0x00000000000000b0 
    /*0000004c*/ MOVW        R3, #1441                   @ 0x5a1 
    /*00000050*/ LDR         R2, WORD_00b4               @ LDR         R2, [PC, #92]               @ 0x00000000000000b4 
    /*00000054*/ LDR         R0, [IP, R1] 
    /*00000058*/ LDR         R1, WORD_00b8               @ LDR         R1, [PC, #88]               @ 0x00000000000000b8 
    /*0000005c*/ ADD         R2, PC, R2 
    /*00000060*/ ADD         R1, PC, R1 
    /*00000064*/ LDR         R0, [R0] 
    /*00000068*/ BL          fprintf
    /*0000006c*/ MOVW        R0, #32774                  @ 0x8006 
    /*00000070*/ MOVT        R0, #40988                  @ 0xa01c 
    /*00000074*/ POP         {R4, PC} 
    cmos_get_isp_black_level_x78:
    /*00000078*/ LDR         R1, WORD_00b0               @ LDR         R1, [PC, #48]               @ 0x00000000000000b0 
    /*0000007c*/ MOVW        R3, #1443                   @ 0x5a3 
    /*00000080*/ LDR         R2, WORD_00bc               @ LDR         R2, [PC, #52]               @ 0x00000000000000bc 
    /*00000084*/ LDR         R0, [IP, R1] 
    /*00000088*/ LDR         R1, WORD_00c0               @ LDR         R1, [PC, #48]               @ 0x00000000000000c0 
    /*0000008c*/ ADD         R2, PC, R2 
    /*00000090*/ ADD         R1, PC, R1 
    /*00000094*/ LDR         R0, [R0] 
    /*00000098*/ BL          fprintf
    /*0000009c*/ MOVW        R0, #32774                  @ 0x8006 
    /*000000a0*/ MOVT        R0, #40988                  @ 0xa01c 
    /*000000a4*/ POP         {R4, PC} 
    /*000000a8*/ WORD_00a8: .word 0x00000094
    /*000000ac*/ WORD_00ac: .word 0x00000000
    /*000000b0*/ WORD_00b0: .word 0x00000000
    /*000000b4*/ WORD_00b4: .word 0x00000050
    /*000000b8*/ WORD_00b8: .word 0x00000050
    /*000000bc*/ WORD_00bc: .word 0x00000028
    /*000000c0*/ WORD_00c0: .word 0x00000028
FUNC_END cmos_get_isp_black_level


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_ae_fswdr_attr_set
@ Size: 0x70
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_ae_fswdr_attr_set
    /*00000000*/ LDR         IP, WORD_0058               @ LDR         IP, [PC, #80]               @ 0x0000000000000058 
    /*00000004*/ CMP         R1, #0 
    /*00000008*/ ADD         IP, PC, IP 
    /*0000000c*/ BEQ         cmos_ae_fswdr_attr_set_x34
    /*00000010*/ LDR         R2, WORD_005c               @ LDR         R2, [PC, #68]               @ 0x000000000000005c 
    /*00000014*/ LDR         R3, WORD_0060               @ LDR         R3, [PC, #68]               @ 0x0000000000000060 
    /*00000018*/ LDR         IP, [R1] 
    /*0000001c*/ MOV         R1, #0 
    /*00000020*/ ADD         R2, PC, R2 
    /*00000024*/ ADD         R3, PC, R3 
    /*00000028*/ STR         IP, [R2, R0, LSL #2] 
    /*0000002c*/ STR         R1, [R3, R0, LSL #2] 
    /*00000030*/ BX          LR 
    cmos_ae_fswdr_attr_set_x34:
    /*00000034*/ LDR         R1, WORD_0064               @ LDR         R1, [PC, #40]               @ 0x0000000000000064 
    /*00000038*/ MOV         R3, #1200                   @ 0x4b0 
    /*0000003c*/ LDR         R2, WORD_0068               @ LDR         R2, [PC, #36]               @ 0x0000000000000068 
    /*00000040*/ LDR         R0, [IP, R1] 
    /*00000044*/ LDR         R1, WORD_006c               @ LDR         R1, [PC, #32]               @ 0x000000000000006c 
    /*00000048*/ ADD         R2, PC, R2 
    /*0000004c*/ ADD         R1, PC, R1 
    /*00000050*/ LDR         R0, [R0] 
    cmos_ae_fswdr_attr_set_x54:
    /*00000054*/ B           cmos_ae_fswdr_attr_set_x54  @ fprintf [R_ARM_JUMP24: +0x0] 
    /*00000058*/ WORD_0058: .word 0x00000048
    /*0000005c*/ WORD_005c: .word 0x00000034
    /*00000060*/ WORD_0060: .word 0x00000034
    /*00000064*/ WORD_0064: .word 0x00000000
    /*00000068*/ WORD_0068: .word 0x00000018
    /*0000006c*/ WORD_006c: .word 0x00000018
FUNC_END cmos_ae_fswdr_attr_set


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_dgain_calc_table
@ Size: 0xf0
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_dgain_calc_table
    /*00000000*/ LDR         R0, WORD_00d0               @ LDR         R0, [PC, #200]              @ 0x00000000000000d0 
    /*00000004*/ CMP         R1, #0 
    /*00000008*/ ADD         R0, PC, R0 
    /*0000000c*/ BEQ         cmos_dgain_calc_table_x88
    /*00000010*/ CMP         R2, #0 
    /*00000014*/ BEQ         cmos_dgain_calc_table_xac
    /*00000018*/ LDR         IP, [R1] 
    /*0000001c*/ MOVW        R3, #32381                  @ 0x7e7d 
    /*00000020*/ CMP         IP, R3 
    /*00000024*/ BHI         cmos_dgain_calc_table_x74
    /*00000028*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000002c*/ MOVW        R0, #1070                   @ 0x42e 
    /*00000030*/ LDR         LR, WORD_00d4               @ LDR         LR, [PC, #156]              @ 0x00000000000000d4 
    /*00000034*/ MOV         R3, #1 
    /*00000038*/ ADD         LR, PC, LR 
    /*0000003c*/ B           cmos_dgain_calc_table_x50
    cmos_dgain_calc_table_x40:
    /*00000040*/ ADD         R3, R3, #1 
    /*00000044*/ CMP         R3, #81                     @ 0x51 
    /*00000048*/ POPEQ       {PC}                        @ (ldreq pc, [sp], #4) 
    /*0000004c*/ LDR         R0, [LR, R3, LSL #2] 
    cmos_dgain_calc_table_x50:
    /*00000050*/ CMP         IP, R0 
    /*00000054*/ BCS         cmos_dgain_calc_table_x40
    /*00000058*/ LDR         R0, WORD_00d8               @ LDR         R0, [PC, #120]              @ 0x00000000000000d8 
    /*0000005c*/ SUB         R3, R3, #1 
    /*00000060*/ ADD         R0, PC, R0 
    /*00000064*/ LDR         R0, [R0, R3, LSL #2] 
    /*00000068*/ STR         R0, [R1] 
    /*0000006c*/ STR         R3, [R2] 
    /*00000070*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    cmos_dgain_calc_table_x74:
    /*00000074*/ MOVW        R0, #32382                  @ 0x7e7e 
    /*00000078*/ MOV         R3, #80                     @ 0x50 
    /*0000007c*/ STR         R0, [R1] 
    /*00000080*/ STR         R3, [R2] 
    /*00000084*/ BX          LR 
    cmos_dgain_calc_table_x88:
    /*00000088*/ LDR         R1, WORD_00dc               @ LDR         R1, [PC, #76]               @ 0x00000000000000dc 
    /*0000008c*/ MOV         R3, #848                    @ 0x350 
    /*00000090*/ LDR         R2, WORD_00e0               @ LDR         R2, [PC, #72]               @ 0x00000000000000e0 
    /*00000094*/ LDR         R0, [R0, R1] 
    /*00000098*/ LDR         R1, WORD_00e4               @ LDR         R1, [PC, #68]               @ 0x00000000000000e4 
    /*0000009c*/ ADD         R2, PC, R2 
    /*000000a0*/ ADD         R1, PC, R1 
    /*000000a4*/ LDR         R0, [R0] 
    cmos_dgain_calc_table_xa8:
    /*000000a8*/ B           cmos_dgain_calc_table_xa8   @ fprintf [R_ARM_JUMP24: +0x0] 
    cmos_dgain_calc_table_xac:
    /*000000ac*/ LDR         R1, WORD_00dc               @ LDR         R1, [PC, #40]               @ 0x00000000000000dc 
    /*000000b0*/ MOVW        R3, #849                    @ 0x351 
    /*000000b4*/ LDR         R2, WORD_00e8               @ LDR         R2, [PC, #44]               @ 0x00000000000000e8 
    /*000000b8*/ LDR         R0, [R0, R1] 
    /*000000bc*/ LDR         R1, WORD_00ec               @ LDR         R1, [PC, #40]               @ 0x00000000000000ec 
    /*000000c0*/ ADD         R2, PC, R2 
    /*000000c4*/ ADD         R1, PC, R1 
    /*000000c8*/ LDR         R0, [R0] 
    cmos_dgain_calc_table_xcc:
    /*000000cc*/ B           cmos_dgain_calc_table_xcc   @ fprintf [R_ARM_JUMP24: +0x0] 
    /*000000d0*/ WORD_00d0: .word 0x000000c0
    /*000000d4*/ WORD_00d4: .word 0x00000094
    /*000000d8*/ WORD_00d8: .word 0x00000070
    /*000000dc*/ WORD_00dc: .word 0x00000000
    /*000000e0*/ WORD_00e0: .word 0x0000003c
    /*000000e4*/ WORD_00e4: .word 0x0000003c
    /*000000e8*/ WORD_00e8: .word 0x00000020
    /*000000ec*/ WORD_00ec: .word 0x00000020
FUNC_END cmos_dgain_calc_table


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_again_calc_table
@ Size: 0xf0
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_again_calc_table
    /*00000000*/ LDR         R0, WORD_00d0               @ LDR         R0, [PC, #200]              @ 0x00000000000000d0 
    /*00000004*/ CMP         R1, #0 
    /*00000008*/ ADD         R0, PC, R0 
    /*0000000c*/ BEQ         cmos_again_calc_table_x88
    /*00000010*/ CMP         R2, #0 
    /*00000014*/ BEQ         cmos_again_calc_table_xac
    /*00000018*/ LDR         IP, [R1] 
    /*0000001c*/ MOVW        R3, #32381                  @ 0x7e7d 
    /*00000020*/ CMP         IP, R3 
    /*00000024*/ BHI         cmos_again_calc_table_x74
    /*00000028*/ PUSH        {LR}                        @ (str lr, [sp, #-4]!) 
    /*0000002c*/ MOVW        R0, #1070                   @ 0x42e 
    /*00000030*/ LDR         LR, WORD_00d4               @ LDR         LR, [PC, #156]              @ 0x00000000000000d4 
    /*00000034*/ MOV         R3, #1 
    /*00000038*/ ADD         LR, PC, LR 
    /*0000003c*/ B           cmos_again_calc_table_x50
    cmos_again_calc_table_x40:
    /*00000040*/ ADD         R3, R3, #1 
    /*00000044*/ CMP         R3, #81                     @ 0x51 
    /*00000048*/ POPEQ       {PC}                        @ (ldreq pc, [sp], #4) 
    /*0000004c*/ LDR         R0, [LR, R3, LSL #2] 
    cmos_again_calc_table_x50:
    /*00000050*/ CMP         IP, R0 
    /*00000054*/ BCS         cmos_again_calc_table_x40
    /*00000058*/ LDR         R0, WORD_00d8               @ LDR         R0, [PC, #120]              @ 0x00000000000000d8 
    /*0000005c*/ SUB         R3, R3, #1 
    /*00000060*/ ADD         R0, PC, R0 
    /*00000064*/ LDR         R0, [R0, R3, LSL #2] 
    /*00000068*/ STR         R0, [R1] 
    /*0000006c*/ STR         R3, [R2] 
    /*00000070*/ POP         {PC}                        @ (ldr pc, [sp], #4) 
    cmos_again_calc_table_x74:
    /*00000074*/ MOVW        R0, #32382                  @ 0x7e7e 
    /*00000078*/ MOV         R3, #80                     @ 0x50 
    /*0000007c*/ STR         R0, [R1] 
    /*00000080*/ STR         R3, [R2] 
    /*00000084*/ BX          LR 
    cmos_again_calc_table_x88:
    /*00000088*/ LDR         R1, WORD_00dc               @ LDR         R1, [PC, #76]               @ 0x00000000000000dc 
    /*0000008c*/ MOVW        R3, #821                    @ 0x335 
    /*00000090*/ LDR         R2, WORD_00e0               @ LDR         R2, [PC, #72]               @ 0x00000000000000e0 
    /*00000094*/ LDR         R0, [R0, R1] 
    /*00000098*/ LDR         R1, WORD_00e4               @ LDR         R1, [PC, #68]               @ 0x00000000000000e4 
    /*0000009c*/ ADD         R2, PC, R2 
    /*000000a0*/ ADD         R1, PC, R1 
    /*000000a4*/ LDR         R0, [R0] 
    cmos_again_calc_table_xa8:
    /*000000a8*/ B           cmos_again_calc_table_xa8   @ fprintf [R_ARM_JUMP24: +0x0] 
    cmos_again_calc_table_xac:
    /*000000ac*/ LDR         R1, WORD_00dc               @ LDR         R1, [PC, #40]               @ 0x00000000000000dc 
    /*000000b0*/ MOVW        R3, #822                    @ 0x336 
    /*000000b4*/ LDR         R2, WORD_00e8               @ LDR         R2, [PC, #44]               @ 0x00000000000000e8 
    /*000000b8*/ LDR         R0, [R0, R1] 
    /*000000bc*/ LDR         R1, WORD_00ec               @ LDR         R1, [PC, #40]               @ 0x00000000000000ec 
    /*000000c0*/ ADD         R2, PC, R2 
    /*000000c4*/ ADD         R1, PC, R1 
    /*000000c8*/ LDR         R0, [R0] 
    cmos_again_calc_table_xcc:
    /*000000cc*/ B           cmos_again_calc_table_xcc   @ fprintf [R_ARM_JUMP24: +0x0] 
    /*000000d0*/ WORD_00d0: .word 0x000000c0
    /*000000d4*/ WORD_00d4: .word 0x00000094
    /*000000d8*/ WORD_00d8: .word 0x00000070
    /*000000dc*/ WORD_00dc: .word 0x00000000
    /*000000e0*/ WORD_00e0: .word 0x0000003c
    /*000000e4*/ WORD_00e4: .word 0x0000003c
    /*000000e8*/ WORD_00e8: .word 0x00000020
    /*000000ec*/ WORD_00ec: .word 0x00000020
FUNC_END cmos_again_calc_table


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.cmos_gains_update
@ Size: 0xb8
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN cmos_gains_update
    /*00000000*/ LDR         IP, WORD_00a4               @ LDR         IP, [PC, #156]              @ 0x00000000000000a4 
    /*00000004*/ ADD         R1, R1, #64                 @ 0x40 
    /*00000008*/ LDR         R3, WORD_00a8               @ LDR         R3, [PC, #152]              @ 0x00000000000000a8 
    /*0000000c*/ ADD         R2, R2, #64                 @ 0x40 
    /*00000010*/ ADD         IP, PC, IP 
    /*00000014*/ LSL         R1, R1, #2 
    /*00000018*/ LDR         R3, [IP, R3] 
    /*0000001c*/ LSL         R2, R2, #2 
    /*00000020*/ LDR         R3, [R3, R0, LSL #2] 
    /*00000024*/ CMP         R3, #0 
    /*00000028*/ BEQ         cmos_gains_update_x80
    /*0000002c*/ LDR         IP, [R3, #116]              @ 0x74 
    /*00000030*/ UBFX        R0, R1, #8, #8 
    /*00000034*/ CMP         IP, R0 
    /*00000038*/ UXTBNE      R1, R1 
    /*0000003c*/ BEQ         cmos_gains_update_x64
    cmos_gains_update_x40:
    /*00000040*/ MOV         IP, #0 
    /*00000044*/ STR         IP, [R3, #92]               @ 0x5c 
    cmos_gains_update_x48:
    /*00000048*/ UBFX        IP, R2, #8, #8 
    /*0000004c*/ UXTB        R2, R2 
    /*00000050*/ STR         R0, [R3, #116]              @ 0x74 
    /*00000054*/ STR         R1, [R3, #140]              @ 0x8c 
    /*00000058*/ STR         IP, [R3, #164]              @ 0xa4 
    /*0000005c*/ STR         R2, [R3, #188]              @ 0xbc 
    /*00000060*/ BX          LR 
    cmos_gains_update_x64:
    /*00000064*/ LDR         IP, [R3, #140]              @ 0x8c 
    /*00000068*/ UXTB        R1, R1 
    /*0000006c*/ CMP         IP, R1 
    /*00000070*/ MOVEQ       IP, #48                     @ 0x30 
    /*00000074*/ STREQ       IP, [R3, #92]               @ 0x5c 
    /*00000078*/ BNE         cmos_gains_update_x40
    /*0000007c*/ B           cmos_gains_update_x48
    cmos_gains_update_x80:
    /*00000080*/ LDR         R1, WORD_00ac               @ LDR         R1, [PC, #36]               @ 0x00000000000000ac 
    /*00000084*/ MOVW        R3, #878                    @ 0x36e 
    /*00000088*/ LDR         R2, WORD_00b0               @ LDR         R2, [PC, #32]               @ 0x00000000000000b0 
    /*0000008c*/ LDR         R0, [IP, R1] 
    /*00000090*/ LDR         R1, WORD_00b4               @ LDR         R1, [PC, #28]               @ 0x00000000000000b4 
    /*00000094*/ ADD         R2, PC, R2 
    /*00000098*/ ADD         R1, PC, R1 
    /*0000009c*/ LDR         R0, [R0] 
    cmos_gains_update_xa0:
    /*000000a0*/ B           cmos_gains_update_xa0       @ fprintf [R_ARM_JUMP24: +0x0] 
    /*000000a4*/ WORD_00a4: .word 0x0000008c
    /*000000a8*/ WORD_00a8: .word 0x00000000
    /*000000ac*/ WORD_00ac: .word 0x00000000
    /*000000b0*/ WORD_00b0: .word 0x00000014
    /*000000b4*/ WORD_00b4: .word 0x00000014
FUNC_END cmos_gains_update


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.sensor_unregister_callback
@ Size: 0x1a0
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN sensor_unregister_callback
    /*00000000*/ PUSH        {R4, R5, R6, R7, R8, LR} 
    /*00000004*/ SUBS        R8, R1, #0 
    /*00000008*/ LDR         R4, WORD_016c               @ LDR         R4, [PC, #348]              @ 0x000000000000016c 
    /*0000000c*/ ADD         R4, PC, R4 
    /*00000010*/ BEQ         sensor_unregister_callback_x13c
    /*00000014*/ CMP         R2, #0 
    /*00000018*/ MOV         R7, R2 
    /*0000001c*/ BEQ         sensor_unregister_callback_x10c
    /*00000020*/ MOVW        R1, #34220                  @ 0x85ac 
    /*00000024*/ MOV         R6, R0 
    /*00000028*/ BL          HI_MPI_ISP_SensorUnRegCallBack
    /*0000002c*/ SUBS        R5, R0, #0 
    /*00000030*/ BNE         sensor_unregister_callback_xb8
    /*00000034*/ MOV         R1, R8 
    /*00000038*/ MOVW        R2, #34220                  @ 0x85ac 
    /*0000003c*/ MOV         R0, R6 
    /*00000040*/ BL          HI_MPI_AE_SensorUnRegCallBack
    /*00000044*/ SUBS        R5, R0, #0 
    /*00000048*/ BNE         sensor_unregister_callback_x8c
    /*0000004c*/ MOV         R1, R7 
    /*00000050*/ MOVW        R2, #34220                  @ 0x85ac 
    /*00000054*/ MOV         R0, R6 
    /*00000058*/ BL          HI_MPI_AWB_SensorUnRegCallBack
    /*0000005c*/ SUBS        R5, R0, #0 
    /*00000060*/ BNE         sensor_unregister_callback_xe4
    /*00000064*/ LDR         R3, WORD_0170               @ LDR         R3, [PC, #260]              @ 0x0000000000000170 
    /*00000068*/ LDR         R4, [R4, R3] 
    /*0000006c*/ LDR         R0, [R4, R6, LSL #2] 
    /*00000070*/ CMP         R0, #0 
    /*00000074*/ BEQ         sensor_unregister_callback_x7c
    /*00000078*/ BL          free
    sensor_unregister_callback_x7c:
    /*0000007c*/ MOV         R3, #0 
    /*00000080*/ STR         R3, [R4, R6, LSL #2] 
    sensor_unregister_callback_x84:
    /*00000084*/ MOV         R0, R5 
    /*00000088*/ POP         {R4, R5, R6, R7, R8, PC} 
    sensor_unregister_callback_x8c:
    /*0000008c*/ LDR         R1, WORD_0174               @ LDR         R1, [PC, #224]              @ 0x0000000000000174 
    /*00000090*/ MOVW        R3, #2194                   @ 0x892 
    /*00000094*/ LDR         R2, WORD_0178               @ LDR         R2, [PC, #220]              @ 0x0000000000000178 
    /*00000098*/ LDR         R0, [R4, R1] 
    /*0000009c*/ LDR         R1, WORD_017c               @ LDR         R1, [PC, #216]              @ 0x000000000000017c 
    /*000000a0*/ ADD         R2, PC, R2 
    /*000000a4*/ ADD         R1, PC, R1 
    /*000000a8*/ LDR         R0, [R0] 
    /*000000ac*/ BL          fprintf
    /*000000b0*/ MOV         R0, R5 
    /*000000b4*/ POP         {R4, R5, R6, R7, R8, PC} 
    sensor_unregister_callback_xb8:
    /*000000b8*/ LDR         R1, WORD_0174               @ LDR         R1, [PC, #180]              @ 0x0000000000000174 
    /*000000bc*/ MOVW        R3, #2186                   @ 0x88a 
    /*000000c0*/ LDR         R2, WORD_0180               @ LDR         R2, [PC, #184]              @ 0x0000000000000180 
    /*000000c4*/ LDR         R0, [R4, R1] 
    /*000000c8*/ LDR         R1, WORD_0184               @ LDR         R1, [PC, #180]              @ 0x0000000000000184 
    /*000000cc*/ ADD         R2, PC, R2 
    /*000000d0*/ ADD         R1, PC, R1 
    /*000000d4*/ LDR         R0, [R0] 
    /*000000d8*/ BL          fprintf
    /*000000dc*/ MOV         R0, R5 
    /*000000e0*/ POP         {R4, R5, R6, R7, R8, PC} 
    sensor_unregister_callback_xe4:
    /*000000e4*/ LDR         R1, WORD_0174               @ LDR         R1, [PC, #136]              @ 0x0000000000000174 
    /*000000e8*/ MOVW        R3, #2202                   @ 0x89a 
    /*000000ec*/ LDR         R2, WORD_0188               @ LDR         R2, [PC, #148]              @ 0x0000000000000188 
    /*000000f0*/ LDR         R0, [R4, R1] 
    /*000000f4*/ LDR         R1, WORD_018c               @ LDR         R1, [PC, #144]              @ 0x000000000000018c 
    /*000000f8*/ ADD         R2, PC, R2 
    /*000000fc*/ ADD         R1, PC, R1 
    /*00000100*/ LDR         R0, [R0] 
    /*00000104*/ BL          fprintf
    /*00000108*/ B           sensor_unregister_callback_x84
    sensor_unregister_callback_x10c:
    /*0000010c*/ LDR         R1, WORD_0174               @ LDR         R1, [PC, #96]               @ 0x0000000000000174 
    /*00000110*/ MOVW        R3, #2180                   @ 0x884 
    /*00000114*/ LDR         R2, WORD_0190               @ LDR         R2, [PC, #116]              @ 0x0000000000000190 
    /*00000118*/ MOVW        R5, #32774                  @ 0x8006 
    /*0000011c*/ LDR         R0, [R4, R1] 
    /*00000120*/ LDR         R1, WORD_0194               @ LDR         R1, [PC, #108]              @ 0x0000000000000194 
    /*00000124*/ ADD         R2, PC, R2 
    /*00000128*/ ADD         R1, PC, R1 
    /*0000012c*/ LDR         R0, [R0] 
    /*00000130*/ MOVT        R5, #40988                  @ 0xa01c 
    /*00000134*/ BL          fprintf
    /*00000138*/ B           sensor_unregister_callback_x84
    sensor_unregister_callback_x13c:
    /*0000013c*/ LDR         R1, WORD_0174               @ LDR         R1, [PC, #48]               @ 0x0000000000000174 
    /*00000140*/ MOVW        R3, #2179                   @ 0x883 
    /*00000144*/ LDR         R2, WORD_0198               @ LDR         R2, [PC, #76]               @ 0x0000000000000198 
    /*00000148*/ MOVW        R5, #32774                  @ 0x8006 
    /*0000014c*/ LDR         R0, [R4, R1] 
    /*00000150*/ LDR         R1, WORD_019c               @ LDR         R1, [PC, #68]               @ 0x000000000000019c 
    /*00000154*/ ADD         R2, PC, R2 
    /*00000158*/ ADD         R1, PC, R1 
    /*0000015c*/ LDR         R0, [R0] 
    /*00000160*/ MOVT        R5, #40988                  @ 0xa01c 
    /*00000164*/ BL          fprintf
    /*00000168*/ B           sensor_unregister_callback_x84
    /*0000016c*/ WORD_016c: .word 0x00000158
    /*00000170*/ WORD_0170: .word 0x00000000
    /*00000174*/ WORD_0174: .word 0x00000000
    /*00000178*/ WORD_0178: .word 0x000000d0
    /*0000017c*/ WORD_017c: .word 0x000000d0
    /*00000180*/ WORD_0180: .word 0x000000ac
    /*00000184*/ WORD_0184: .word 0x000000ac
    /*00000188*/ WORD_0188: .word 0x00000088
    /*0000018c*/ WORD_018c: .word 0x00000088
    /*00000190*/ WORD_0190: .word 0x00000064
    /*00000194*/ WORD_0194: .word 0x00000064
    /*00000198*/ WORD_0198: .word 0x0000003c
    /*0000019c*/ WORD_019c: .word 0x0000003c
FUNC_END sensor_unregister_callback


@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
@ .text.sensor_global_init
@ Size: 0xa8
@ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

FUNC_BEGIN sensor_global_init
    /*00000000*/ LDR         R1, WORD_0094               @ LDR         R1, [PC, #140]              @ 0x0000000000000094 
    /*00000004*/ LDR         R3, WORD_0098               @ LDR         R3, [PC, #140]              @ 0x0000000000000098 
    /*00000008*/ ADD         R1, PC, R1 
    /*0000000c*/ PUSH        {R4, R5, R6, LR} 
    /*00000010*/ LDR         R3, [R1, R3] 
    /*00000014*/ LDR         R4, [R3, R0, LSL #2] 
    /*00000018*/ CMP         R4, #0 
    /*0000001c*/ BEQ         sensor_global_init_x6c
    /*00000020*/ MOV         R5, #0 
    /*00000024*/ MOVW        R3, #1126                   @ 0x466 
    /*00000028*/ STR         R5, [R4] 
    /*0000002c*/ MOV         R0, #2 
    /*00000030*/ STRB        R0, [R4, #8] 
    /*00000034*/ MOV         R1, R5 
    /*00000038*/ STR         R5, [R4, #4] 
    /*0000003c*/ MOV         R2, #1056                   @ 0x420 
    /*00000040*/ STR         R5, [R4, #12] 
    /*00000044*/ ADD         R0, R4, #16 
    /*00000048*/ STR         R3, [R4, #2136]             @ 0x858 
    /*0000004c*/ STR         R3, [R4, #2128]             @ 0x850 
    /*00000050*/ STR         R3, [R4, #2132]             @ 0x854 
    /*00000054*/ BL          memset
    /*00000058*/ MOV         R1, R5 
    /*0000005c*/ ADD         R0, R4, #1072               @ 0x430 
    /*00000060*/ MOV         R2, #1056                   @ 0x420 
    /*00000064*/ POP         {R4, R5, R6, LR} 
    sensor_global_init_x68:
    /*00000068*/ B           sensor_global_init_x68      @ memset [R_ARM_JUMP24: +0x0] 
    sensor_global_init_x6c:
    /*0000006c*/ LDR         R0, WORD_009c               @ LDR         R0, [PC, #40]               @ 0x000000000000009c 
    /*00000070*/ MOVW        R3, #2050                   @ 0x802 
    /*00000074*/ LDR         R2, WORD_00a0               @ LDR         R2, [PC, #36]               @ 0x00000000000000a0 
    /*00000078*/ LDR         R0, [R1, R0] 
    /*0000007c*/ LDR         R1, WORD_00a4               @ LDR         R1, [PC, #32]               @ 0x00000000000000a4 
    /*00000080*/ ADD         R2, PC, R2 
    /*00000084*/ ADD         R1, PC, R1 
    /*00000088*/ LDR         R0, [R0] 
    /*0000008c*/ POP         {R4, R5, R6, LR} 
    sensor_global_init_x90:
    /*00000090*/ B           sensor_global_init_x90      @ fprintf [R_ARM_JUMP24: +0x0] 
    /*00000094*/ WORD_0094: .word 0x00000084
    /*00000098*/ WORD_0098: .word 0x00000000
    /*0000009c*/ WORD_009c: .word 0x00000000
    /*000000a0*/ WORD_00a0: .word 0x00000018
    /*000000a4*/ WORD_00a4: .word 0x00000018
FUNC_END sensor_global_init


