#ifndef __OX05B1S_EX_H__
#define __OX05B1S_EX_H__

#ifdef __cplusplus
extern "C" {
#endif

// Piris attr
static const xmedia_isp_piris_attr g_ox05b1s_piris_attr = {
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

static const xmedia_isp_awb_ccm g_ox05b1s_awb_ccm = {
    4,
    {
        {
            7200,
            {   402,32891,32791,32833,426,32873,19,32922,391},
        },
        {
            5000, //ct
            {   402,32885,32797,32859,467,32888,36,32965,417}, //ccm
        },
        {
            3800, //ct
            {   399,32856,32823,32873,429,32836,39,32963,412}, //ccm
        },
        {
            2500, //ct
            {   538,33004,32814,32846,375,32809,127,33216,577}, //ccm
        },
    },
};

static const xmedia_isp_awb_agc_table g_ox05b1s_awb_agc_table = {
    /* bvalid */
    1,

    /* 1,  2,  4,  8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768 */
    /* saturation */
    {120,100,100,100,100,100,90,80,80,80,80,80,80,80,80,80}
};

#ifdef __cplusplus
}
#endif

#endif
