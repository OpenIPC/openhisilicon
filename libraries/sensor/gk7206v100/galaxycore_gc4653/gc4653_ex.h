#ifndef __GC4653_EX_H__
#define __GC4653_EX_H__

#ifdef __cplusplus
extern "C" {
#endif

// Piris attr
static const xmedia_isp_piris_attr g_gc4653_piris_attr = {
    0,
    1,
    94,
    62,
    // Step-F number mapping table. Must be from small to large. F1.0 is 1024 and F32.0 is 1
    { 30,  35,  40,  45,  50,  56,  61,  67,  73,  79,  85,  92,  98,  105, 112, 120, 127, 135, 143, 150, 158,
      166, 174, 183, 191, 200, 208, 217, 225, 234, 243, 252, 261, 270, 279, 289, 298, 307, 316, 325, 335, 344,
      353, 362, 372, 381, 390, 399, 408, 417, 426, 435, 444, 453, 462, 470, 478, 486, 493, 500, 506, 512 },
    XMEDIA_ISP_IRIS_F_NO_1_4,
    XMEDIA_ISP_IRIS_F_NO_5_6
};

static const xmedia_isp_awb_ccm g_gc4653_awb_ccm = {
    4,
    {
        {
            6175,
            { 440, 32967, 15, 32852, 424, 32852, 11, 33014, 491 },
        },
        {
            4990,
            { 443, 32969, 14, 32876, 407, 32811, 32769, 33012, 501 },
        },
        {
            3865,
            { 458, 32977, 7,  32853, 389, 32816, 24, 33018, 482 },
        },
        {
            2450,
            { 427, 32931, 32776, 32897, 401, 32784, 4, 33168, 652 },
        },
    },
};


static const xmedia_isp_awb_agc_table g_gc4653_awb_agc_table = {
    // bvalid
    1,

    // 1,  2,  4,  8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768
    // saturation
    { 0x80, 0x76, 0x70, 0x6C, 0x69, 0x66, 0x5A, 0x4E, 0x44, 0x40, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38 }
};

#ifdef __cplusplus
}
#endif

#endif