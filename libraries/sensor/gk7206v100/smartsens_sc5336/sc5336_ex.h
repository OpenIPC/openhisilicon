#ifndef __SC5336_EX_H__
#define __SC5336_EX_H__

#ifdef __cplusplus
extern "C" {
#endif

// Piris attr
static const xmedia_isp_piris_attr g_sc5336_piris_attr = {
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

static const xmedia_isp_awb_ccm g_sc5336_awb_ccm = {
    4,
    {
        {
            6250,
            /*{
                0x0292,  0x81C6,  0x0034,
                0x806E,  0x01D1,  0x8063,
                0x8021,  0x815B,  0x027C
            },*/
            /*{
                0x022D,  0x80EC,  0x8041,
                0x803D,  0x01B8,  0x807B,
                0x8019,  0x809F,  0x01B8
            },*/
            {
                0x0233,  0x813C,  0x0009,
                0x8046,  0x01AF,  0x8069,
                0x8010,  0x80F0,  0x0200
            }
        },

        {
            4850,
            /*{
                0x02A5,  0x81D3,  0x002E,
                0x806D,  0x01AB,  0x803E,
                0x8023,  0x819F,  0x02C2
            },*/
            {
                0x0296,  0x8126,  0x8070,
                0x804D,  0x01AD,  0x8060,
                0x801E,  0x80DF,  0x01FD
            },
        },

        {
            3650,
            /*{
                0x026E,  0x8172,  0x0004,
                0x809F,  0x01D6,  0x8037,
                0x8047,  0x81C4,  0x030B
            },*/
            {
                0x01FE,  0x8094,  0x806A,
                0x8052,  0x01C8,  0x8076,
                0x802D,  0x80D3,  0x0200
            },
        },

        {
            2650,
            /*{
                0x01E5,  0x80B7,  0x802E,
                0x8096,  0x01A6,  0x8010,
                0x80B9,  0x82B1,  0x046A
            },*/
            {
                0x0222,  0x80B7,  0x806B,
                0x803A,  0x01B0,  0x8076,
                0x8042,  0x8151,  0x0293
            },
        },
    },
};

static const xmedia_isp_awb_agc_table g_sc5336_awb_agc_table = {
    // bvalid
    1,

    // saturation
    {125, 125, 123, 120, 118, 115, 110, 110, 105, 105, 100, 100, 90, 90, 90, 90}
};

#ifdef __cplusplus
}
#endif

#endif