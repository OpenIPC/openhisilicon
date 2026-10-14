#ifndef __SC500AI_EX_H__
#define __SC500AI_EX_H__

#ifdef __cplusplus
extern "C" {
#endif

// Piris attr
static const xmedia_isp_piris_attr g_sc500ai_piris_attr = {
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

static const xmedia_isp_awb_ccm g_sc500ai_awb_ccm = {
     4,
    {
        {
            6150,
            {
                569, 33198, 117, 32838, 328, 32770, 0, 33019, 507
            },
        },
        {
            4850,
            {
                606, 33210, 92, 32837, 331, 32774, 32769, 33085, 574
            },
        },
        {
            3650,
            {
                523, 33069, 34, 32879, 392, 32793, 32785, 33014, 519
            },
        },
        {
            2650,
            {
                594, 33130, 24, 32892, 353, 27, 32803, 33246, 769
            },
        },
    },
};

static const xmedia_isp_awb_agc_table g_sc500ai_awb_agc_ir_table = {
    // bvalid 
    1,

    // saturation 
    //  1,    2,    4,    8,   16,   32,   64,  128,  256,  512, 1024, 2048, 4096, 8192, 16384, 32768 
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }
};

static const xmedia_isp_awb_agc_table g_sc500ai_awb_agc_table = {
    // bvalid 
    1,
    // saturation 
    //  1,    2,    4,    8,   16,   32,   64,  128,  256,  512, 1024, 2048, 4096, 8192, 16384, 32768 
    { 110, 110, 108, 108, 105, 100, 95, 95, 90, 90, 90, 80, 80, 80, 80, 80}
};

static const xmedia_isp_awb_ccm g_sc500ai_awb_wdr_ccm = {
    5,
    {
        {
            6800,
            {
                0x0134,  0x8075,  0x0041,
                0x802B,  0x0140,  0x8015,
                0x0010,  0x805A,  0x014A
            },
        },

        {
            6000,
            {
                0x0164,  0x80A5,  0x0041,
                0x802F,  0x0162,  0x8033,
                0x000F,  0x8075,  0x0166
            },
        },

        {
            4850,
            {
                0x0159,  0x8085,  0x002C,
                0x803E,  0x017A,  0x803C,
                0x001E,  0x807E,  0x0160
            },
        },

        {
            3650,
            {
                0x01A7, 0x80C7, 0x0020,
                0x8038, 0x0149, 0x8011,
                0x0016, 0x8088, 0x0172
            },
        },

        {
            2650,
            {
                0x0167, 0x80A2, 0x003B,
                0x8033, 0x0135, 0x8002,
                0x0014, 0x80AC, 0x0198
            },
        },
    },
};

static const xmedia_isp_awb_agc_table g_sc500ai_awb_wdr_agc_table = {
    // bvalid 
    1,
    // saturation 
    //  1,    2,    4,    8,   16,   32,   64,  128,  256,  512, 1024, 2048, 4096, 8192, 16384, 32768 
    { 110, 110, 108, 108, 105, 100, 95, 95, 90, 90, 90, 80, 80, 80, 80, 80}
};

#ifdef __cplusplus
}
#endif

#endif