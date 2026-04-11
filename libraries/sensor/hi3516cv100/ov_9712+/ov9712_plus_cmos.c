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
     5000,
     {
        0x0146, 0x8068, 0x0021,
        0x800c, 0x00e9, 0x0022,
        0x0010, 0x80bd, 0x01ac
     },
     3200,
     {
      0x016a, 0x8034, 0x8036,
      0x801d, 0x00da, 0x0042,
      0x8017, 0x81d2, 0x02e9
     },
     2700,
     {
      0x016a, 0x8034, 0x8036,
      0x801d, 0x00da, 0x0042,
      0x8017, 0x81d2, 0x02e9
     }
    },


    // black level
    {68,64,68,68},

    //calibration reference color temperature
    5000,

    //WB gain at 5000, must keep consistent with calibration color temperature
      {0x1d4, 0x100, 0x100, 0x1cc},  //ov9712+

    // WB curve parameters, must keep consistent with reference color temperature.
    {-133,650,261,159186,128,-107849},

	// hist_thresh
    {0xd,0x28,0x60,0x80},

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

    2
};

static cmos_isp_agc_table_t st_isp_agc_table =
{
#if CMOS_OV9712_MORE_SHARPEN
    //sharpen_alt_d
    {80,75,70,65,55,45,35,20},

    //sharpen_alt_ud
    {75,70,65,60,50,40,25,10},

    //snr_thresh
    {0x23,0x28,0x2b,0x35,0x3f,0x46,0x4b,0x4f},
#else    
    //sharpen_alt_d
    {0x8e,0x8b,0x88,0x83,0x7d,0x76,0x75,0x74},

    //sharpen_alt_ud
    {0x8f,0x89,0x7e,0x78,0x6f,0x44,0x40,0x35},

    //snr_thresh
    {0x19,0x1e,0x2d,0x32,0x39,0x3f,0x48,0x4b},
#endif

    //demosaic_lum_thresh
    {80,64,64,48,48,32,32,16},

    //demosaic_np_offset
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x30},

    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55},

    /* saturation */
    {0x90,0x90,0x80,0x80,0x68,0x48,0x35,0x30}

};

static cmos_isp_noise_table_t st_isp_noise_table =
{
    //nosie_profile_weight_lut
    {
        0x00,0x04,0x10,0x16,0x1a,0x1d,0x1f,0x21,0x23,0x24,0x26,0x27,0x28,0x29,0x2a,0x2a,0x2b,
        0x2c,0x2d,0x2d,0x2e,0x2e,0x2f,0x2f,0x30,0x30,0x31,0x31,0x32,0x32,0x33,0x33,0x33,0x34,
        0x34,0x34,0x35,0x35,0x35,0x36,0x36,0x36,0x37,0x37,0x37,0x37,0x38,0x38,0x38,0x38,0x39,
        0x39,0x39,0x39,0x39,0x3a,0x3a,0x3a,0x3a,0x3b,0x3b,0x3b,0x3b,0x3b,0x3b,0x3c,0x3c,0x3c,
        0x3c,0x3c,0x3c,0x3d,0x3d,0x3d,0x3d,0x3d,0x3d,0x3e,0x3e,0x3e,0x3e,0x3e,0x3e,0x3e,0x3f,
        0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x41,0x41,0x41,
        0x41,0x41,0x41,0x41,0x41,0x41,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x43,
        0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43

    },

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
    205,

    /*aa_slope*/
    191,
#if CMOS_OV9712_MORE_SHARPEN
    /*va_slope*/
    193,

    /*uu_slope*/
    200,
#else
    /*va_slope*/
    0xec,

    /*uu_slope*/
    0x89,
#endif

    /*sat_slope*/
    0x5d,

    /*ac_slope*/
    0xcf,

    /*vh_thresh*/
    16,

    /*aa_thresh*/
    10,

    /*va_thresh*/
    10,

    /*uu_thresh*/
    10,

    /*sat_thresh*/
    0x171,

    /*ac_thresh*/
    0x1b3
};

static cmos_isp_shading_table_t st_isp_shading_table =
{
    /*shading_center_r*/
    0x28f, 0x16c,

    /*shading_center_g*/
    0x28e, 0x177,

    /*shading_center_b*/
    0x293, 0x16c,

    {
            0x1000,0x100c,0x1035,0x1065,0x1099,0x10cd,0x1102,0x1137,0x116c,0x11a1,0x11d5,0x1209,
            0x123d,0x126f,0x12a2,0x12d3,0x1303,0x1335,0x1365,0x1394,0x13c2,0x13f0,0x141c,0x1449,
            0x1475,0x149f,0x14c9,0x14f3,0x151b,0x1543,0x156a,0x1590,0x15b5,0x15d9,0x15fd,0x1620,
            0x1642,0x1663,0x1684,0x16a3,0x16c2,0x16e0,0x16fd,0x1719,0x1735,0x1750,0x176a,0x1783,
            0x179b,0x17b3,0x17ca,0x17e0,0x17f5,0x180a,0x181e,0x1831,0x1843,0x1855,0x1866,0x1877,
            0x1887,0x1896,0x18a4,0x18b2,0x18bf,0x18cb,0x18d7,0x18e3,0x18ef,0x18f8,0x1903,0x190d,
            0x1916,0x191e,0x1926,0x192d,0x1935,0x193c,0x1942,0x1948,0x194d,0x1953,0x1958,0x195d,
            0x1961,0x1964,0x1968,0x196c,0x196f,0x1972,0x1975,0x1977,0x1978,0x197b,0x197d,0x197e,
            0x1981,0x1982,0x1983,0x1984,0x1986,0x1987,0x1988,0x1989,0x198a,0x198a,0x198a,0x198c,
            0x198d,0x198e,0x198e,0x198f,0x1990,0x1991,0x1992,0x1993,0x1994,0x1996,0x1997,0x1999,
            0x199a,0x199c,0x199d,0x199f,0x19a2,0x19a5,0x19a7,0x19aa,0x19ac
    },

    {
            0x1000,0x1005,0x1028,0x1055,0x1087,0x10bb,0x10f0,0x1125,0x115b,0x1190,0x11c5,0x11fa,
            0x122f,0x1263,0x1296,0x12c9,0x12fc,0x132d,0x135e,0x138f,0x13be,0x13ed,0x141b,0x1449,
            0x1475,0x14a1,0x14cc,0x14f6,0x1520,0x1548,0x1570,0x1597,0x15bd,0x15e2,0x1606,0x1629,
            0x164b,0x166d,0x168e,0x16ad,0x16cc,0x16ea,0x1707,0x1724,0x173f,0x175a,0x1773,0x178c,
            0x17a4,0x17bc,0x17d2,0x17e8,0x17fc,0x1810,0x1824,0x1836,0x1848,0x1859,0x1869,0x1879,
            0x1888,0x1896,0x18a4,0x18b1,0x18bd,0x18c9,0x18d4,0x18df,0x18e9,0x18f2,0x18fb,0x1904,
            0x190c,0x1914,0x191b,0x1921,0x1928,0x192d,0x1933,0x1938,0x193d,0x1941,0x1945,0x1949,
            0x194d,0x1950,0x1953,0x1956,0x1959,0x195b,0x195d,0x195f,0x1961,0x1963,0x1965,0x1966,
            0x1967,0x1969,0x196a,0x196b,0x196d,0x196e,0x196f,0x1970,0x1972,0x1973,0x1974,0x1975,
            0x1977,0x1978,0x197a,0x197c,0x197e,0x1980,0x1982,0x1984,0x1987,0x1989,0x198c,0x198f,
            0x1992,0x1996,0x199a,0x199e,0x19a2,0x19a6,0x19ab,0x19b0,0x19b4
    },

    {
           0x1000,0x1000,0x1011,0x102f,0x1054,0x107c,0x10a5,0x10cf,0x10fa,0x1125,0x1150,0x117c,
           0x11a7,0x11d1,0x11fc,0x1226,0x1250,0x1279,0x12a2,0x12ca,0x12f2,0x131a,0x1340,0x1367,
           0x138c,0x13b1,0x13d6,0x13fa,0x141d,0x143f,0x1461,0x1483,0x14a3,0x14c3,0x14e3,0x1502,
           0x1520,0x153d,0x155a,0x1576,0x1591,0x15ac,0x15c6,0x15e0,0x15f8,0x1610,0x1628,0x163f,
           0x1655,0x166b,0x1680,0x1694,0x16a8,0x16bb,0x16cd,0x16df,0x16f1,0x1701,0x1712,0x1721,
           0x1730,0x173f,0x174d,0x175b,0x1768,0x1774,0x1780,0x178c,0x1797,0x17a2,0x17ac,0x17b6,
           0x17bf,0x17c8,0x17d1,0x17d9,0x17e1,0x17e8,0x17ef,0x17f6,0x17fd,0x1803,0x1809,0x180f,
           0x1814,0x1819,0x181e,0x1822,0x1827,0x182b,0x182f,0x1833,0x1836,0x183a,0x183d,0x1840,
           0x1843,0x1846,0x1849,0x184b,0x184e,0x1850,0x1853,0x1855,0x1857,0x185a,0x185c,0x185e,
           0x1860,0x1862,0x1864,0x1867,0x1869,0x186b,0x186d,0x1870,0x1872,0x1875,0x1877,0x187a,
           0x187d,0x1880,0x1883,0x1886,0x1889,0x188c,0x1890,0x1894,0x1895

    },

    /*shading_off_center_r_g_b*/
    0xef0, 0xec3, 0xecd,

    /*shading_table_nobe_number*/
    129
};


static cmos_isp_gamma_table_t st_isp_gamma_table =
{

   
   { 0,  54, 106, 158, 209, 259, 308, 356, 403, 450, 495, 540, 584, 628, 670, 713, 
    754, 795, 835, 874, 913, 951, 989,1026,1062,1098,1133,1168,1203,1236,1270,1303, 
    1335,1367,1398,1429,1460,1490,1520,1549,1578,1607,1635,1663,1690,1717,1744,1770,
    1796,1822,1848,1873,1897,1922,1946,1970,1993,2017,2040,2062,2085,2107,2129,2150, 
    2172,2193,2214,2235,2255,2275,2295,2315,2335,2354,2373,2392,2411,2429,2447,2465,
    2483,2501,2519,2536,2553,2570,2587,2603,2620,2636,2652,2668,2684,2700,2715,2731, 
    2746,2761,2776,2790,2805,2819,2834,2848,2862,2876,2890,2903,2917,2930,2944,2957,
    2970,2983,2996,3008,3021,3033,3046,3058,3070,3082,3094,3106,3118,3129,3141,3152, 
    3164,3175,3186,3197,3208,3219,3230,3240,3251,3262,3272,3282,3293,3303,3313,3323, 
    3333,3343,3352,3362,3372,3381,3391,3400,3410,3419,3428,3437,3446,3455,3464,3473, 
    3482,3490,3499,3508,3516,3525,3533,3541,3550,3558,3566,3574,3582,3590,3598,3606, 
    3614,3621,3629,3637,3644,3652,3660,3667,3674,3682,3689,3696,3703,3711,3718,3725, 
    3732,3739,3746,3752,3759,3766,3773,3779,3786,3793,3799,3806,3812,3819,3825,3831, 
    3838,3844,3850,3856,3863,3869,3875,3881,3887,3893,3899,3905,3910,3916,3922,3928, 
    3933,3939,3945,3950,3956,3962,3967,3973,3978,3983,3989,3994,3999,4005,4010,4015, 
    4020,4026,4031,4036,4041,4046,4051,4056,4061,4066,4071,4076,4081,4085,4090,4095, 4095}

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
