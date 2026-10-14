#ifndef __BF3A03_EX_H__
#define __BF3A03_EX_H__

#ifdef __cplusplus
extern "C" {
#endif

// Piris attr
static const xmedia_isp_piris_attr g_bf3a03_piris_attr = {
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

static const xmedia_isp_awb_ccm g_bf3a03_awb_ccm = {
    3,
    {
        {
            6300,
            {699,33237,26,32804,344,32820,32792,33114,626},
        },
        {
            3850,
            {622,33009,32893,32805,442,32917,32787,33015,522},
        },
        {
            2750,
            {556,32913,32923,32888,554,32946,32770,33052,542},
        },
    },
};

static const xmedia_isp_awb_agc_table g_bf3a03_awb_agc_table = {
    // bvalid
    1,

    // 1,  2,  4,  8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768
    // saturation
    { 125,  120,  115,   115,  115,  115,  115,  105,  96,   86,   86,   76,   56,   56,    56,    56}
};

#ifdef __cplusplus
}
#endif

#endif
