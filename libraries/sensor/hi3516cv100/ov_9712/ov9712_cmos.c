#if !defined(__OV9712_CMOS_H_)
#define __OV9712_CMOS_H_

#include <stdio.h>
#include <string.h>
#include "hi_comm_sns.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "hi_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/****************************************************************************
 * local variables															*
 ****************************************************************************/

extern const unsigned int sensor_i2c_addr;
extern const unsigned int sensor_addr_byte;
extern const unsigned int sensor_data_byte;

static cmos_inttime_t cmos_inttime;
static cmos_gains_t cmos_gains;
static HI_U8 gu8SensorMode = 0;

static HI_U8 gu8Fps = 30;

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/

/*set Frame End Update Mode 2 with HI_MPI_ISP_SetAEAttr and set this value 1 to avoid flicker in antiflicker mode */
/*when use Frame End Update Mode 2, the speed of i2c will affect whole system's performance                       */
/*increase I2C_DFT_RATE in Hii2c.c to 400000 to increase the speed of i2c                                         */
#define CMOS_OV9712_ISP_WRITE_SENSOR_ENABLE (1)

/*change this value to 1 to make the image looks more sharpen*/    
#define CMOS_OV9712_MORE_SHARPEN (1)

/* To change the mode of slow framerate. When the value is 0, add the line numbers to slow framerate.
 * When the value is 1, add the line length to slow framerate. */
#define CMOS_OV9712_SLOW_FRAMERATE_MODE (0)

static cmos_isp_default_t st_coms_isp_default =
{
	// color matrix[9]
    {
        5300,
        {   0x0254, 0x8102, 0x8052,
            0x8041, 0x01d5, 0x8094,
            0x8027, 0x8136, 0x025d
        },
        3558,
        {   0x02a0, 0x8125, 0x807b,
            0x8043, 0x0156, 0x8013,
            0x802a, 0x8268, 0x0392
        },
        3084,
        {   0x023a, 0x808d, 0x80ad,
            0x8033, 0x01c0, 0x808d,
            0x8071, 0x82af, 0x0420
        }
    },


    // black level
    {64,64,64,64},

    //calibration reference color temperature
    5000,

    //WB gain at 5000, must keep consistent with calibration color temperature
    {0x015b, 0x100, 0x100, 0x0199},

    // WB curve parameters, must keep consistent with reference color temperature.
    {127, -23, -152, 154393, 128, -105036},

	// hist_thresh
	{0xd,0x28,0x60,0x80},
    //{0x10,0x40,0xc0,0xf0},

	0x0,	// iridix_balck
	0x3,	// bggr

	/* limit max gain for reducing noise,    */
	0x1f,	0x1,

	// iridix
	0x04,	0x08,	0xa0, 	0x4ff,

	0x1, 	// balance_fe
	0x80,	// ae compensation
	0x15, 	// sinter threshold

	0x0,  0,  0,  //noise profile=0, use the default noise profile lut, don't need to set nr0 and nr1

#if CMOS_OV9712_ISP_WRITE_SENSOR_ENABLE
    2
#else
    1
#endif

};

static cmos_isp_agc_table_t st_isp_agc_table =
{
#if CMOS_OV9712_MORE_SHARPEN
    //sharpen_alt_d
    {0x50,0x4b,0x46,0x41,0x37,0x2c,0x1e,0xf},

    //sharpen_alt_ud
    {0x4b,0x46,0x41,0x3c,0x32,0x28,0x19,0xa},

    //snr_thresh
    {0x10,0x14,0x1a,0x22,0x2a,0x37,0x46,0x50},
#else    
    //sharpen_alt_d
    {0x8e,0x8b,0x88,0x83,0x7d,0x76,0x75,0x74},

    //sharpen_alt_ud
    {0x8f,0x89,0x7e,0x78,0x6f,0x44,0x40,0x35},

    //snr_thresh
    {0x19,0x1e,0x2d,0x32,0x39,0x3f,0x48,0x4b},
#endif

    //demosaic_lum_thresh
    {0x50,0x50,0x40,0x40,0x30,0x30,0x20,0x20},

    //demosaic_np_offset
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x30},

    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55},

    /* saturation */
    {0x80,0x80,0x80,0x80,0x68,0x48,0x35,0x30}

};

static cmos_isp_noise_table_t st_isp_noise_table =
{
    //nosie_profile_weight_lut
    {0, 27, 31, 33, 35, 36, 37, 38, 39, 40, 40, 41, 41, 42, 42, 43,
    43, 43, 44, 44, 44, 45, 45, 45, 45, 46, 46, 46, 46, 46, 47, 47,
    47, 47, 47, 48, 48, 48, 48, 48, 48, 48, 49, 49, 49, 49, 49, 49,
    49, 49, 50, 50, 50, 50, 50, 50, 50, 50, 50, 51, 51, 51, 51, 51,
    51, 51, 51, 51, 51, 51, 51, 52, 52, 52, 52, 52, 52, 52, 52, 52,
    52, 52, 52, 52, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
    53, 53, 53, 53, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
    54, 54, 54, 54, 54, 54, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55},

    //demosaic_weight_lut
    {0, 27, 31, 33, 35, 36, 37, 38, 39, 40, 40, 41, 41, 42, 42, 43,
    43, 43, 44, 44, 44, 45, 45, 45, 45, 46, 46, 46, 46, 46, 47, 47,
    47, 47, 47, 48, 48, 48, 48, 48, 48, 48, 49, 49, 49, 49, 49, 49,
    49, 49, 50, 50, 50, 50, 50, 50, 50, 50, 50, 51, 51, 51, 51, 51,
    51, 51, 51, 51, 51, 51, 51, 52, 52, 52, 52, 52, 52, 52, 52, 52,
    52, 52, 52, 52, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
    53, 53, 53, 53, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
    54, 54, 54, 54, 54, 54, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55}
};

static cmos_isp_demosaic_t st_isp_demosaic =
{
    /*vh_slope*/
    0xcd,

    /*aa_slope*/
    0xbf,
#if CMOS_OV9712_MORE_SHARPEN
    /*va_slope*/
    0xc1,

    /*uu_slope*/
    0xc8,
#else
    /*va_slope*/
    0xc1,

    /*uu_slope*/
    0xa0,
#endif

    /*sat_slope*/
    0x5d,

    /*ac_slope*/
    0xcf,

    /*vh_thresh*/
    0x10,

    /*aa_thresh*/
    0x10,

    /*va_thresh*/
    0x10,

    /*uu_thresh*/
    0xa,

    /*sat_thresh*/
    0x171,

    /*ac_thresh*/
    0x1b3
};

static cmos_isp_shading_table_t st_isp_shading_table =
{
    /*shading_center_r*/
    0x27a, 0x168,

    /*shading_center_g*/
    0x276, 0x16f,

    /*shading_center_b*/
    0x27a, 0x16c,

    /*shading_table_r*/
    {0x1000,0x1018,0x1028,0x103a,0x104c,0x105c,0x1072,0x1089,0x109e,0x10ba,0x10d5,0x10ef,
	0x110b,0x112b,0x114c,0x116d,0x118b,0x11ae,0x11d0,0x11f5,0x1218,0x123e,0x1260,0x1283,
	0x12aa,0x12cf,0x12f7,0x131b,0x1341,0x1369,0x138f,0x13b5,0x13db,0x1401,0x1423,0x1446,
	0x146d,0x148f,0x14b4,0x14d7,0x14fe,0x151e,0x153e,0x155a,0x1579,0x159a,0x15b7,0x15d3,
	0x15f4,0x1612,0x162e,0x164d,0x1663,0x167f,0x169a,0x16b1,0x16cb,0x16e4,0x16fc,0x170f,
	0x1727,0x173e,0x1753,0x176a,0x1783,0x1793,0x17a5,0x17b8,0x17c9,0x17da,0x17ec,0x17fe,
	0x180d,0x181a,0x182a,0x183b,0x184c,0x185a,0x1865,0x1876,0x1883,0x1890,0x189f,0x18a9,
	0x18b5,0x18c3,0x18cf,0x18d8,0x18e2,0x18e8,0x18ec,0x18f5,0x1901,0x190e,0x191f,0x1934,
	0x1946,0x1955,0x1968,0x197e,0x1993,0x19a4,0x19b5,0x19cc,0x19e1,0x19f5,0x1a06,0x1a16,
	0x1a2a,0x1a3d,0x1a4f,0x1a5d,0x1a6e,0x1a84,0x1a96,0x1aa7,0x1abb,0x1ad2,0x1ae5,0x1af9,
	0x1b0e,0x1b27,0x1b40,0x1b59,0x1b68,0x1b72,0x1b91,0x1bbf,0x1bf6},

    /*shading_table_g*/
    {0x1000,0x1013,0x1022,0x1033,0x1043,0x1054,0x1066,0x107b,0x108e,0x10a5,0x10bc,0x10d7,
	0x10f1,0x110c,0x112a,0x114b,0x116b,0x118c,0x11ae,0x11d0,0x11f3,0x1216,0x1238,0x125c,
	0x1282,0x12a9,0x12ce,0x12f3,0x131b,0x133f,0x1363,0x1386,0x13a9,0x13cc,0x13f0,0x1412,
	0x1434,0x1457,0x1479,0x149c,0x14bc,0x14da,0x14fa,0x1519,0x1537,0x1557,0x1575,0x1590,
	0x15ac,0x15c8,0x15e1,0x15fd,0x1617,0x162f,0x1648,0x165f,0x1673,0x168b,0x16a1,0x16b5,
	0x16c9,0x16db,0x16ee,0x1702,0x1714,0x1726,0x1736,0x1744,0x1752,0x1760,0x176c,0x1778,
	0x1785,0x1794,0x179e,0x17a8,0x17b5,0x17c1,0x17ca,0x17d5,0x17e1,0x17e9,0x17ef,0x17f3,
	0x17f9,0x17ff,0x1800,0x1801,0x1803,0x1806,0x180d,0x181c,0x182f,0x183e,0x184a,0x1854,
	0x185e,0x186a,0x1878,0x188b,0x189b,0x18a8,0x18b5,0x18c2,0x18d0,0x18e0,0x18ef,0x18fc,
	0x1907,0x1910,0x191a,0x1927,0x1936,0x1941,0x194a,0x1958,0x1965,0x1971,0x197e,0x1989,
	0x199c,0x19aa,0x19b7,0x19bd,0x19cc,0x19cc,0x19ce,0x19f6,0x1a0a},

    /*shading_table_b*/
    {0x1000,0x1012,0x101c,0x1025,0x102c,0x1031,0x103b,0x1045,0x104f,0x1059,0x1060,0x1072,
	0x107f,0x108d,0x109f,0x10b5,0x10c9,0x10dc,0x10ee,0x1109,0x111c,0x1137,0x1150,0x1167,
	0x1183,0x119e,0x11bb,0x11d5,0x11f4,0x120c,0x1228,0x1242,0x125a,0x1276,0x1292,0x12ac,
	0x12c7,0x12e0,0x12f9,0x1313,0x132d,0x1346,0x135d,0x1372,0x138a,0x13a4,0x13bb,0x13cf,
	0x13e5,0x13fb,0x1410,0x1427,0x143c,0x1450,0x1462,0x1475,0x1488,0x1496,0x14a5,0x14b7,
	0x14ca,0x14d7,0x14e5,0x14f4,0x1503,0x1511,0x151d,0x1529,0x1532,0x153c,0x1548,0x1552,
	0x155b,0x1566,0x156c,0x1573,0x157d,0x1587,0x158d,0x1593,0x159b,0x15a3,0x15a9,0x15ac,
	0x15b1,0x15b4,0x15b5,0x15b5,0x15b6,0x15b5,0x15b5,0x15ba,0x15c4,0x15d3,0x15e2,0x15ef,
	0x15f9,0x1604,0x1611,0x161e,0x1626,0x1632,0x1641,0x164c,0x165b,0x1667,0x1672,0x1679,
	0x1681,0x168e,0x1699,0x16a0,0x16aa,0x16bc,0x16ce,0x16dc,0x16e9,0x16f1,0x16f9,0x170c,
	0x1722,0x1735,0x173d,0x1739,0x1739,0x173c,0x173c,0x1731,0x1724},

    /*shading_off_center_r_g_b*/
    0xf57, 0xf0e, 0xf42,

    /*shading_table_nobe_number*/
    129
};


static cmos_isp_gamma_table_t st_isp_gamma_table =
{
    {0  , 27  , 60  , 100 , 140 , 178 , 216 , 242 , 276 , 312 , 346 , 380 , 412 , 444 , 476 , 508,
    540 , 572 , 604 , 636 , 667 , 698 , 729 , 760 , 791 , 822 , 853 , 884 , 915 , 945 , 975 , 1005,
    1035, 1065, 1095, 1125, 1155, 1185, 1215, 1245, 1275, 1305, 1335, 1365, 1395, 1425, 1455, 1485,
    1515, 1544, 1573, 1602, 1631, 1660, 1689, 1718, 1746, 1774, 1802, 1830, 1858, 1886, 1914, 1942,
    1970, 1998, 2026, 2054, 2082, 2110, 2136, 2162, 2186, 2220, 2244, 2268, 2292, 2316, 2340, 2362,
    2384, 2406, 2428, 2448, 2468, 2488, 2508, 2528, 2548, 2568, 2588, 2608, 2628, 2648, 2668, 2688,
    2708, 2728, 2748, 2768, 2788, 2808, 2828, 2846, 2862, 2876, 2890, 2903, 2917, 2930, 2944, 2957,
    2970, 2983, 2996, 3008, 3021, 3033, 3046, 3058, 3070, 3082, 3094, 3106, 3118, 3129, 3141, 3152,
    3164, 3175, 3186, 3197, 3208, 3219, 3230, 3240, 3251, 3262, 3272, 3282, 3293, 3303, 3313, 3323,
    3333, 3343, 3352, 3362, 3372, 3381, 3391, 3400, 3410, 3419, 3428, 3437, 3446, 3455, 3464, 3473,
    3482, 3490, 3499, 3508, 3516, 3525, 3533, 3541, 3550, 3558, 3566, 3574, 3582, 3590, 3598, 3606,
    3614, 3621, 3629, 3637, 3644, 3652, 3660, 3667, 3674, 3682, 3689, 3696, 3703, 3711, 3718, 3725,
    3732, 3739, 3746, 3752, 3759, 3766, 3773, 3779, 3786, 3793, 3799, 3806, 3812, 3819, 3825, 3831,
    3838, 3844, 3850, 3856, 3863, 3869, 3875, 3881, 3887, 3893, 3899, 3905, 3910, 3916, 3922, 3928,
    3933, 3939, 3945, 3950, 3956, 3962, 3967, 3973, 3978, 3983, 3989, 3994, 3999, 4005, 4010, 4015,
    4020, 4026, 4031, 4036, 4041, 4046, 4051, 4056, 4061, 4066, 4071, 4076, 4081, 4085, 4090, 4095, 4095}
};


/*
 * This function initialises an instance of cmos_inttime_t.
 */
static __inline cmos_inttime_const_ptr_t cmos_inttime_initialize()
{
    cmos_inttime.full_lines_std = 810;
	cmos_inttime.full_lines_std_30fps = 810;
	cmos_inttime.full_lines = 810;
	cmos_inttime.full_lines_limit = 65535;
    cmos_inttime.max_lines = 806;
    cmos_inttime.min_lines = 2;
	cmos_inttime.max_lines_target = cmos_inttime.max_lines;
	cmos_inttime.min_lines_target = cmos_inttime.min_lines;

	cmos_inttime.vblanking_lines = 0;

	cmos_inttime.exposure_ashort = 0;
	cmos_inttime.exposure_shift = 0;

	cmos_inttime.lines_per_500ms = cmos_inttime.full_lines_std_30fps * 30 / 2; 
	cmos_inttime.flicker_freq = 0;//60*256;//50*256;

	return &cmos_inttime;
}

/*
 * This function applies the new integration time to the ISP registers.
 */
static __inline void cmos_inttime_update(cmos_inttime_ptr_t p_inttime)
{
#if CMOS_OV9712_SLOW_FRAMERATE_MODE
    p_inttime->exposure_ashort = (p_inttime->exposure_ashort << 4) / p_inttime->slow_framerate;
    if(p_inttime->exposure_ashort >= p_inttime->full_lines_std - 4)
	{
		p_inttime->exposure_ashort = p_inttime->full_lines_std - 4;
	}
#endif
#if CMOS_OV9712_ISP_WRITE_SENSOR_ENABLE
    ISP_I2C_DATA_S stI2cData;
    
    stI2cData.bDelayCfg = HI_FALSE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;
    stI2cData.u32RegAddr = 0x10;
    stI2cData.u32Data = p_inttime->exposure_ashort & 0xFF;
    HI_MPI_ISP_I2cWrite(&stI2cData);
    
    stI2cData.u32RegAddr = 0x16;
    stI2cData.u32Data = (p_inttime->exposure_ashort >> 8) & 0xFF;
    HI_MPI_ISP_I2cWrite(&stI2cData);
#else
	HI_U32 _curr = p_inttime->exposure_ashort;
    
    sensor_write_register(0x10, _curr & 0xFF);
    sensor_write_register(0x16, (_curr >> 8) & 0xFF);    
#endif

    return;
}

/*
 * This function applies the new vert blanking porch to the ISP registers.
 */
static __inline void cmos_vblanking_update(cmos_inttime_const_ptr_t p_inttime)
{
#if CMOS_OV9712_SLOW_FRAMERATE_MODE
    HI_U32 u32Tp = 50760;   /* (0x698 * 30) = 50640*/
    u32Tp = (((u32Tp * p_inttime->slow_framerate) / gu8Fps) >> 4) + 3;
    if (u32Tp > 0x2000)     /* the register 0x2a adn 0x2b's max value is 0x2000 */
    {
        u32Tp = 0x2000;
        p_inttime->slow_framerate = ((gu8Fps * u32Tp) << 4) / 50760;
        printf("Warning! The slow_framerate is out of ov9712's range!\n");
    }
    
    #if CMOS_OV9712_ISP_WRITE_SENSOR_ENABLE
    ISP_I2C_DATA_S stI2cData;
    
    stI2cData.bDelayCfg = HI_TRUE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;
    stI2cData.u32RegAddr = 0x2a;
    stI2cData.u32Data = u32Tp & 0xfc;
    HI_MPI_ISP_I2cWrite(&stI2cData);
    
    stI2cData.u32RegAddr = 0x2b;
    stI2cData.u32Data = (u32Tp & 0xff00) >> 8;
    HI_MPI_ISP_I2cWrite(&stI2cData);
    #else
    sensor_write_register(0x2a, u32Tp & 0xfc);
    sensor_write_register(0x2b, (u32Tp & 0xff00) >> 8);
    #endif
#else
    static HI_U16 last_vblanking_lines = 65535;
    if(p_inttime->vblanking_lines != last_vblanking_lines)
    {
        sensor_write_register(0x2d, p_inttime->vblanking_lines & 0xff);
        sensor_write_register(0x2e, (p_inttime->vblanking_lines & 0xff00) >> 8);
        last_vblanking_lines = p_inttime->vblanking_lines;
    }
#endif

    return;
}

static __inline HI_U16 vblanking_calculate(
		cmos_inttime_ptr_t p_inttime)
{
	if(p_inttime->exposure_ashort >= p_inttime->full_lines - 4)
	{
		p_inttime->exposure_ashort = p_inttime->full_lines - 4;
	}

	p_inttime->vblanking_lines = p_inttime->full_lines - p_inttime->full_lines_std_30fps;

	return p_inttime->exposure_ashort;
}

/* Set fps base */
static __inline void cmos_fps_set(
		cmos_inttime_ptr_t p_inttime,
		const HI_U8 fps
		)
{
	switch(fps)
	{
		case 30:
            p_inttime->lines_per_500ms = cmos_inttime.full_lines_std_30fps * 30 / 2;
            gu8Fps = 30;
            sensor_write_register(0x2a, 0x9c);
            sensor_write_register(0x2b, 0x6);
		break;

		case 25:
            /* do not change full_lines_std */
			p_inttime->lines_per_500ms = cmos_inttime.full_lines_std_30fps * 25 / 2;
            gu8Fps = 25;
            sensor_write_register(0x2a, 0xf0);
            sensor_write_register(0x2b, 0x7);
        break;

		default:
		break;
	}
    
	return;
}

/*
 * This function initialises an instance of cmos_gains_t.
 */
static __inline cmos_gains_ptr_t cmos_gains_initialize()
{
    cmos_gains.max_again = 496;
	cmos_gains.max_dgain = 1;

	cmos_gains.again_shift = 4;
	cmos_gains.dgain_shift = 0;
	cmos_gains.dgain_fine_shift = 0;

    cmos_gains.isp_dgain_shift = 8;
    cmos_gains.isp_dgain = 1 << cmos_gains.isp_dgain_shift;
    cmos_gains.max_isp_dgain_target = 4 << cmos_gains.isp_dgain_shift;
    cmos_gains.isp_dgain_delay_cfg = HI_TRUE;

	return &cmos_gains;
}

static __inline HI_U32 cmos_get_ISO(cmos_gains_ptr_t p_gains)
{
	HI_U32 _again = p_gains->again == 0 ? 1 : p_gains->again;
	HI_U32 _dgain = p_gains->dgain == 0 ? 1 : p_gains->dgain;
    HI_U32 _isp_dgain = p_gains->isp_dgain == 0 ? 1 : p_gains->isp_dgain;

	p_gains->iso =  ((_again * _dgain * _isp_dgain * 100) 
                     >> (p_gains->again_shift + p_gains->dgain_shift 
                         + p_gains->isp_dgain_shift));
	return p_gains->iso;

}

/*
 * This function applies the new gains to the ISP registers.
 */
static __inline void cmos_gains_update(cmos_gains_const_ptr_t p_gains)
{
#if CMOS_OV9712_ISP_WRITE_SENSOR_ENABLE
    ISP_I2C_DATA_S stI2cData;

    stI2cData.bDelayCfg = HI_TRUE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;
    stI2cData.u32RegAddr = 0x00;
    stI2cData.u32Data = p_gains->again_db;
    HI_MPI_ISP_I2cWrite(&stI2cData);
    
#else
    sensor_write_register(0x00, p_gains->again_db);
#endif

	return;
}

static __inline HI_U32 analog_gain_from_exposure_calculate(
		cmos_gains_ptr_t p_gains,
		HI_U32 exposure,
		HI_U32 exposure_max,
		HI_U32 exposure_shift)
{
    HI_U32 _again = 1 << p_gains->again_shift;
    int shift = 0;
    HI_U8 i = 0;

    while (exposure > (1<<26))
    {
        exposure >>= 1;
        exposure_max >>= 1;
        ++shift;
    }

    if(exposure > exposure_max)
    {
        _again = (exposure << p_gains->again_shift) / exposure_max;
        _again = _again < 16? 16: _again;
    	_again = _again > p_gains->max_again_target ? p_gains->max_again_target : _again;
        
        for(i = 0; i < 5; i++)
        {
            if(_again < 32)
            {
                break;
            }
            _again >>= 1;
        }
    }
    else
    {
    }

    p_gains->again_db = (1 << (i + 4)) + _again - 32;
    p_gains->again = _again << i;

    return (exposure << (shift + 4)) / p_gains->again;

}

static __inline HI_U32 isp_digital_gain_from_exposure_calculate(
		cmos_gains_ptr_t p_gains,
		HI_U32 exposure,
		HI_U32 exposure_max,
		HI_U32 exposure_shift)
{
    HI_U32 isp_dgain = (1 << p_gains->isp_dgain_shift);
    int shft = 0;

    while (exposure > (1 << 22))
	{
		exposure >>= 1;
		exposure_max >>= 1;
		++shft;
	}
    exposure_max = (0 == exposure_max)? 1: exposure_max;

    if(exposure > exposure_max)
    {
        isp_dgain = ((exposure << p_gains->isp_dgain_shift) + (exposure_max >> 1)) / exposure_max;
        exposure = exposure_max;
        isp_dgain = (isp_dgain > p_gains->max_isp_dgain_target) ? (p_gains->max_isp_dgain_target) : isp_dgain;        
    }
    else
	{
	}
    p_gains->isp_dgain = isp_dgain;

    return exposure << shft;
}
static __inline HI_U32 digital_gain_from_exposure_calculate(
		cmos_gains_ptr_t p_gains,
		HI_U32 exposure,
		HI_U32 exposure_max,
		HI_U32 exposure_shift)
{
    int shft = 0;    
    while (exposure > (1 << 22))
	{
		exposure >>= 1;
		exposure_max >>= 1;
		++shft;
	}
    
	p_gains->dgain = 1;

	return exposure << shft;
}

static HI_U32 cmos_get_isp_agc_table(cmos_isp_agc_table_ptr_t p_cmos_isp_agc_table)
{
	if (NULL == p_cmos_isp_agc_table)
	{
	    printf("null pointer when get isp agc table value!\n");
	    return -1;
	}
    memcpy(p_cmos_isp_agc_table, &st_isp_agc_table, sizeof(cmos_isp_agc_table_t));
    return 0;
}

static HI_U32 cmos_get_isp_noise_table(cmos_isp_noise_table_ptr_t p_cmos_isp_noise_table)
{
	if (NULL == p_cmos_isp_noise_table)
	{
	    printf("null pointer when get isp noise table value!\n");
	    return -1;
	}
    memcpy(p_cmos_isp_noise_table, &st_isp_noise_table, sizeof(cmos_isp_noise_table_t));
    return 0;
}

static HI_U32 cmos_get_isp_demosaic(cmos_isp_demosaic_ptr_t p_cmos_isp_demosaic)
{
   if (NULL == p_cmos_isp_demosaic)
   {
	    printf("null pointer when get isp demosaic value!\n");
	    return -1;
   }
   memcpy(p_cmos_isp_demosaic, &st_isp_demosaic,sizeof(cmos_isp_demosaic_t));
   return 0;

}

static HI_U32 cmos_get_isp_shading_table(cmos_isp_shading_table_ptr_t p_cmos_isp_shading_table)
{
	if (NULL == p_cmos_isp_shading_table)
	{
	    printf("null pointer when get isp shading table value!\n");
	    return -1;
	}
    memcpy(p_cmos_isp_shading_table, &st_isp_shading_table, sizeof(cmos_isp_shading_table_t));
    return 0;
}

static HI_U32 cmos_get_isp_gamma_table(cmos_isp_gamma_table_ptr_t p_cmos_isp_gamma_table)
{
   if(NULL == p_cmos_isp_gamma_table)
   {
    printf("null pointer when get isp gamma table!\n");
    return -1;
   }
   memcpy(p_cmos_isp_gamma_table,&st_isp_gamma_table, sizeof(cmos_isp_gamma_table_t));
   return 0;
   
}



static void setup_sensor(int isp_mode)
{
	if(0 == isp_mode) /* ISP 'normal' isp_mode */
	{
        sensor_write_register(0x2d, 0x0);
        sensor_write_register(0x2e, 0x0);
	}
	else if(1 == isp_mode) /* ISP pixel calibration isp_mode */
	{
        /* 5 fps */
        sensor_write_register(0x2d, 0xd2); 
        sensor_write_register(0x2e, 0x0f); 
        
        /* min gain */
        sensor_write_register(0x0, 0x00);               

        /* max exposure time*/
        sensor_write_register(0x10, 0xf8);
    	sensor_write_register(0x16, 0x12);
	}
}

static HI_U32 cmos_gains_lin_to_db_convert(HI_U32 data, HI_U32 shift_in)
{
    #define PRECISION 8
    HI_U32 _res = 0;
    if(0 == data)
        return _res;

    data = data << PRECISION; // to ensure precision.
    for(;;)
    {
        /* Note to avoid endless loop here. */
        data = (data * 913) >> 10;
        // data = (data*913 + (1<<9)) >> 10; // endless loop when shift_in is 0. */
        if(data <= ((1<<shift_in) << PRECISION))
        {
            break;
        }
        ++_res;
    }
    return _res;
}

static HI_U8 cmos_get_analog_gain(cmos_gains_ptr_t p_gains)
{
    return cmos_gains_lin_to_db_convert(p_gains->again, p_gains->again_shift);
}

static HI_U8 cmos_get_digital_gain(cmos_gains_ptr_t p_gains)
{
    return 0;
}

static HI_U32 cmos_get_isp_default(cmos_isp_default_ptr_t p_coms_isp_default)
{
	if (NULL == p_coms_isp_default)
	{
	    printf("null pointer when get isp default value!\n");
	    return -1;
	}
    memcpy(p_coms_isp_default, &st_coms_isp_default, sizeof(cmos_isp_default_t));
    return 0;
}


static HI_S32 cmos_get_sensor_max_resolution(cmos_sensor_max_resolution_ptr_t p_cmos_sensor_max_resolution)
{
     if(NULL == p_cmos_sensor_max_resolution)
     {
       printf("null pointer when get sensor max resolution value!\n");
       return -1;
     }

     p_cmos_sensor_max_resolution->u32MaxWidth  = 1280;
     p_cmos_sensor_max_resolution->u32MaxHeight = 720;

    return 0;
}


/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

SENSOR_EXP_FUNC_S stSensorExpFuncs =
{
    .pfn_cmos_inttime_initialize = cmos_inttime_initialize,
    .pfn_cmos_inttime_update = cmos_inttime_update,

    .pfn_cmos_gains_initialize = cmos_gains_initialize,
    .pfn_cmos_gains_update = cmos_gains_update,
    .pfn_cmos_gains_update2 = NULL,
    .pfn_analog_gain_from_exposure_calculate = analog_gain_from_exposure_calculate,
    .pfn_digital_gain_from_exposure_calculate = digital_gain_from_exposure_calculate,
    .pfn_isp_digital_gain_from_exposure_calculate = isp_digital_gain_from_exposure_calculate,

    .pfn_cmos_fps_set = cmos_fps_set,
    .pfn_vblanking_calculate = vblanking_calculate,
    .pfn_cmos_vblanking_front_update = cmos_vblanking_update,

    .pfn_setup_sensor = setup_sensor,

	.pfn_cmos_get_analog_gain = cmos_get_analog_gain,
	.pfn_cmos_get_digital_gain = cmos_get_digital_gain,
	.pfn_cmos_get_digital_fine_gain = NULL,
    .pfn_cmos_get_iso = cmos_get_ISO,

	.pfn_cmos_get_isp_default = cmos_get_isp_default,
	.pfn_cmos_get_isp_special_alg = NULL,
	.pfn_cmos_get_isp_agc_table = cmos_get_isp_agc_table,
	.pfn_cmos_get_isp_noise_table = cmos_get_isp_noise_table,
	.pfn_cmos_get_isp_demosaic = cmos_get_isp_demosaic,
	.pfn_cmos_get_isp_shading_table = cmos_get_isp_shading_table,
	.pfn_cmos_get_isp_gamma_table = cmos_get_isp_gamma_table,
    .pfn_cmos_get_sensor_max_resolution = cmos_get_sensor_max_resolution,
    .pfn_cmos_set_sensor_image_mode = NULL,

};

int sensor_register_callback(void)
{
	int ret;
	ret = HI_MPI_ISP_SensorRegCallBack(&stSensorExpFuncs);
	if (ret)
	{
	    printf("sensor register callback function failed!\n");
	    return ret;
	}

	return 0;
}

//chang sensor mode
int sensor_mode_set(HI_U8 u8Mode)
{
    switch(u8Mode)
    {
        //sensor mode 0
        case 0:
            gu8SensorMode = 0;
            // TODO:
        break;
        //sensor mode 1
        case 1:
            gu8SensorMode = 1;
             // TODO:
        break;

        default:
            printf("NOT support this mode!\n");
            return -1;
        break;
    }
    return 0;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif // __OV9712_CMOS_H_
