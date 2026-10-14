#ifndef __SC4336_EX_H__
#define __SC4336_EX_H__

#ifdef __cplusplus
extern "C" {
#endif

// Piris attr
static const xmedia_isp_piris_attr g_sc4336_piris_attr = {
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

static const xmedia_isp_awb_ccm g_sc4336_awb_ccm = {
    4,
    {
        {
            6150,
            {
                0x0292,  0x81C6,  0x0034,
                0x806E,  0x01D1,  0x8063,
                0x8021,  0x815B,  0x027C
            },
        },

        {
            4850,
            {
                0x02A5,  0x81D3,  0x002E,
                0x806D,  0x01AB,  0x803E,
                0x8023,  0x819F,  0x02C2
            },
        },

        {
            3650,
            {
                0x026E,  0x8172,  0x0004,
                0x809F,  0x01D6,  0x8037,
                0x8047,  0x81C4,  0x030B
            },
        },

        {
            2650,
            {
                0x01E5,  0x80B7,  0x802E,
                0x8096,  0x01A6,  0x8010,
                0x80B9,  0x82B1,  0x046A
            },
        },
    },
};

static const xmedia_isp_awb_agc_table g_sc4336_awb_agc_table = {
    /* bvalid */
    1,

    /* saturation */
    /*  1,    2,    4,    8,   16,   32,   64,  128,  256,  512, 1024, 2048, 4096, 8192, 16384, 32768 */
    {100, 100, 100, 100, 95, 95, 95, 95, 90, 90, 90, 80, 80, 80, 80, 80}
};

#ifdef __cplusplus
}
#endif

#endif