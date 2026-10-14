#ifndef __GC2083_EX_H__
#define __GC2083_EX_H__

#ifdef __cplusplus
extern "C" {
#endif

// Piris attr
static const xmedia_isp_piris_attr g_gc2083_piris_attr = {
    0,
    1,
    94,
    62,
     // Step-F number mapping table. Must be from small to large. F1.0 is 1024 and F32.0 is 1
    {30, 35, 40, 45, 50, 56, 61, 67, 73, 79, 85, 92, 98, 105, 112, 120, 127, 135, 143, 150, 158,
     166, 174, 183, 191, 200, 208, 217, 225, 234, 243, 252, 261, 270, 279, 289, 298, 307, 316, 325, 335, 344,
     353, 362, 372, 381, 390, 399, 408, 417, 426, 435, 444, 453, 462, 470, 478, 486, 493, 500, 506, 512},
    XMEDIA_ISP_IRIS_F_NO_1_4,
    XMEDIA_ISP_IRIS_F_NO_5_6
};

static const xmedia_isp_awb_ccm g_gc2083_awb_ccm = {
     4,
    {
        {
            6150,
            {
                0x01F3,  0x80E1,  0x8012,
                0x8054,  0x019E,  0x804A,
                0x8001,  0x80E2,  0x01E3
            },
        },

        {
            4850,
            {
                0x01F6,  0x80D2,  0x8024,
                0x805C,  0x01A5,  0x8049,
                0x8005,  0x80FE,  0x0203
            },
        },

        {
            3650,
            {
                0x01E6,  0x805E,  0x8088,
                0x809A,  0x01FB,  0x8061,
                0x8024,  0x80FE,  0x0222
            },
        },
        {
            2650,
            {
                0x01F9,  0x80D5,  0x8024,
                0x8082,  0x01A9,  0x8027,
                0x802D,  0x821A,  0x0347
            },
        },

    }
};

static const xmedia_isp_awb_agc_table g_gc2083_awb_agc_ir_table = {
    // bvalid
    1,

    // saturation
    //  1,    2,    4,    8,   16,   32,   64,  128,  256,  512, 1024, 2048, 4096, 8192, 16384, 32768
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }
};

static const xmedia_isp_awb_agc_table g_gc2083_awb_agc_table = {
    // bvalid
    1,
    // saturation
    //  1,    2,    4,    8,   16,   32,   64,  128,  256,  512, 1024, 2048, 4096, 8192, 16384, 32768
    {120,120,115,110,100,90,95,90,72,64,56,56,56,56,56,56}
};

#ifdef __cplusplus
}
#endif

#endif