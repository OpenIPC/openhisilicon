#if !defined(__IMX236_CMOS_H_)
#define __IMX236_CMOS_H_

#include <stdio.h>
#include <unistd.h>		
#include <string.h>
#include <assert.h>
#include "hi_comm_sns.h"
#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


#define EXPOSURE_ADDR (0x220) 
#define LONG_EXPOSURE_ADDR (0x223)
#define PGC_ADDR (0x214)
#define VMAX_ADDR (0x218)
#define HMAX_ADDR (0x21B)
#define CMOS_IMX236_ISP_WRITE_SENSOR_ENABLE (1)
/****************************************************************************
 * local variables															*
 ****************************************************************************/
extern const unsigned int sensor_i2c_addr;
extern const unsigned int sensor_addr_byte;
extern const unsigned int sensor_data_byte;

static cmos_inttime_t cmos_inttime;
static cmos_gains_t cmos_gains;
HI_U8 gu8SensorMode = 0;


static cmos_isp_default_t st_coms_isp_default_lin =
{
    // color correction matrix
    {
        4850,
        {
            0x01f7, 0x80ef, 0x8008,
            0x804b, 0x019d, 0x8052,
            0x0015, 0x8133, 0x021d
        },
        3140,
        {
            0x0199, 0x8080, 0x8019,
            0x808e, 0x01b3, 0x8025,
            0x000f, 0x817c, 0x026c
        },
        2470,
        {
            0x0147, 0x8025, 0x8022,
            0x8086, 0x018b, 0x8005,
            0x801e, 0x8228, 0x0346
        }
    },
	// black level for R, Gr, Gb, B channels
    {0xf0,0xf0,0xf0,0xf0},

    // calibration reference color temperature
    4850,

    //WB gain at 5000K, must keep consistent with calibration color temperature
    
	{0x190, 0x100, 0x100, 0x204},

    // WB curve parameters, must keep consistent with reference color temperature.
	//{63, 65, -127, 201983, 128, -151034},
	{62, 68, -125, 201189, 128, -150451},
	// hist_thresh
    {0xd,0x28,0x60,0x80},

    0x00,	// iridix_balck
    0x0,	// 0:rggb; 2: gbrg

    // gain
    0xFB,	0x1,

    // iridix space, intensity, slope_max, white level
    0x02,	0x08,	0x80, 	0x8ff,

    0x1, 	// balance_fe
    0x80,	// ae compensation
    0x20, 	// sinter threshold

    0x1,        //0: use default profile table; 1: use calibrated profile lut, the setting for nr0 and nr1 must be correct.
    0,
    1528,
#if CMOS_IMX236_ISP_WRITE_SENSOR_ENABLE
    2
#else
    1
#endif
};

static cmos_isp_default_t st_coms_isp_default_wdr =
{
    // color correction matrix
    {
        4850,
        {
            0x022d, 0x80fc, 0x8031,
            0x803a, 0x01a9, 0x806f,
            0x002e, 0x8130, 0x0201
        },
        3140,
        {
            0x01dc, 0x808e, 0x804e,
            0x809f, 0x01f5, 0x8056,
            0x0013, 0x8186, 0x0272
        },
        2470,
        {
            0x018d, 0x807d, 0x8010,
            0x8072, 0x016f, 0x2,
            0x8006, 0x8235, 0x033b
        }
    },

	// black level for R, Gr, Gb, B channels
    {0x0,0x0,0x0,0x0},

    // calibration reference color temperature
    4850,

    //WB gain at 5000K, must keep consistent with calibration color temperature
	{0x190, 0x100, 0x100, 0x206},
	
    // WB curve parameters, must keep consistent with reference color temperature.
    {63, 65, -127, 201983, 128, -151034},
	
    // hist_thresh
    {0x20,0x40,0x60,0x80},
   
    0x00,   // iridix_balck
    0x0,    // 0:rggb; 2: gbrg

    // gain
    0x4,    0x1,

    // iridix space, intensity, slope_max, white level
    0x08,   0x01,   0x20,   0xfff,

    0x0,    // balance_fe
    0x80,   // ae compensation
    0x9,    // sinter threshold
    
    0x0,     //noise profile=0, use the default noise profile lut, don't need to set nr0 and nr1
    0x0,
    0x0,
#if CMOS_IMX236_ISP_WRITE_SENSOR_ENABLE
    2
#else
    1
#endif
};

static cmos_isp_agc_table_t st_isp_agc_table =
{
    //sharpen_alt_d
    {0x88,0x85,0x80,0x7b,0x78,0x72,0x70,0x60},

    //sharpen_alt_ud
    {0xc8,0xc0,0xb8,0xb0,0xa8,0xa0,0x72,0x4b},

    //snr_thresh
    {0x06,0x8,0xb,0x16,0x22,0x28,0x32,0x54},

    //demosaic_lum_thresh
    {0x60,0x60,0x80,0x80,0x80,0x80,0x80,0x80},

    //demosaic_np_offset
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x30},

    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37},

    /* saturation */
    {0x80,0x80,0x80,0x80,0x68,0x48,0x35,0x30}
};


static cmos_isp_noise_table_t st_isp_noise_table =
{
  //nosie_profile_weight_lut

    {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x0c,0x11,0x14,0x17,0x19,0x1b,0x1c,0x1e,0x1f,
        0x20,0x21,0x22,0x23,0x24,0x24,0x25,0x26,0x26,0x27,0x28,0x28,0x29,0x29,0x2a,0x2a,0x2a,
        0x2b,0x2b,0x2c,0x2c,0x2c,0x2d,0x2d,0x2d,0x2e,0x2e,0x2e,0x2f,0x2f,0x2f,0x30,0x30,0x30,
        0x30,0x31,0x31,0x31,0x31,0x32,0x32,0x32,0x32,0x32,0x33,0x33,0x33,0x33,0x34,0x34,0x34,
        0x34,0x34,0x34,0x35,0x35,0x35,0x35,0x35,0x35,0x36,0x36,0x36,0x36,0x36,0x36,0x37,0x37,
        0x37,0x37,0x37,0x37,0x37,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x39,0x39,0x39,0x39,0x39,
        0x39,0x39,0x39,0x39,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3b,0x3b,0x3b,0x3b,
        0x3b,0x3b,0x3b,0x3b,0x3b,0x3c,0x3c,0x3c,0x3c
    },
  
    {
        0x04,0x0c,0x11,0x14,0x17,0x19,0x1b,0x1c,0x1e,0x1f,
        0x20,0x21,0x22,0x23,0x24,0x24,0x25,0x26,0x26,0x27,0x28,0x28,0x29,0x29,0x2a,0x2a,0x2a,
        0x2b,0x2b,0x2c,0x2c,0x2c,0x2d,0x2d,0x2d,0x2e,0x2e,0x2e,0x2f,0x2f,0x2f,0x30,0x30,0x30,
        0x30,0x31,0x31,0x31,0x31,0x32,0x32,0x32,0x32,0x32,0x33,0x33,0x33,0x33,0x34,0x34,0x34,
        0x34,0x34,0x34,0x35,0x35,0x35,0x35,0x35,0x35,0x36,0x36,0x36,0x36,0x36,0x36,0x37,0x37,
        0x37,0x37,0x37,0x37,0x37,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x39,0x39,0x39,0x39,0x39,
        0x39,0x39,0x39,0x39,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3b,0x3b,0x3b,0x3b,
        0x3b,0x3b,0x3b,0x3b,0x3b,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c
    },
};

static cmos_isp_noise_table_t st_isp_noise_table_wdr =
{
    /*nosie_profile_weight_lut WDR*/
    {
        0,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,45,48,57,
            63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,
            63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,
            63,63,63,63,63,63,63,63,63,63,63,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
            64,64,64,64,64,64,64,64,64,64,64,64,64,64,64
    },

  //demosaic_weight_lut
    {
        0,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,45,48,57,
            63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,
            63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,
            63,63,63,63,63,63,63,63,63,63,63,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
            64,64,64,64,64,64,64,64,64,64,64,64,64,64,64
    }
};

static cmos_isp_demosaic_t st_isp_demosaic =
{
    /*vh_slope*/
    0xd8,

    /*aa_slope*/
    0xce,

    /*va_slope*/
    0xe0,

    /*uu_slope*/
    0xc4,

    /*sat_slope*/
    0x5d,

    /*ac_slope*/
    0xcf,

    /*vh_thresh*/
    0x4c,

    /*aa_thresh*/
    0x5b,

    /*va_thresh*/
    0x39,

    /*uu_thresh*/
    0x3d,

    /*sat_thresh*/
    0x171,

    /*ac_thresh*/
    0x1b3,

};

extern void sensor_linner_init();
extern void sensor_wdr_init();

/*
 * This function initialises an instance of cmos_inttime_t.
 */
static __inline cmos_inttime_const_ptr_t cmos_inttime_initialize()
{
    cmos_inttime.full_lines_std = 1125;
    cmos_inttime.full_lines_std_30fps = 1125;
    cmos_inttime.full_lines_std_25fps = 1350;    
    
    //cmos_inttime.full_lines = 1125;
    //cmos_inttime.full_lines_limit = 65535;
    cmos_inttime.max_lines_target = 1123;
    cmos_inttime.min_lines_target = 2;
    cmos_inttime.exposure_shift = 0;
    //cmos_inttime.vblanking_lines = 1125;

   //cmos_inttime.exposure_ashort = 0;

    cmos_inttime.lines_per_500ms = 16875; // 500ms / 29.63us = 16874
    cmos_inttime.flicker_freq = 0;//60*256;//50*256;

    switch(gu8SensorMode)
    {
        default:
        case 0: //linear mode

        break;
        case 1: //WDR mode for short exposure, Exposure ratio = 16X
            cmos_inttime.max_lines_target = 70;
            cmos_inttime.min_lines_target = 8;
        break;
    }
    return &cmos_inttime;
}

/*
 * This function applies the new integration time to the ISP registers.
 */
static __inline void cmos_inttime_update(cmos_inttime_ptr_t p_inttime) 
{

    HI_U16 exp_time,current;
    ISP_I2C_DATA_S stI2cData;
    static HI_U32 _last_exposure_time = 0xFFFFFFFF;
    
    if(_last_exposure_time == p_inttime->exposure_ashort)
    {
        return;
    }else
    {
        _last_exposure_time = p_inttime->exposure_ashort;
    }
    
    switch(gu8SensorMode)
    {
        default:
        case 0: //linear mode
            exp_time = p_inttime->full_lines - p_inttime->exposure_ashort;  
            current = sensor_read_register(EXPOSURE_ADDR+2);
#if CMOS_IMX236_ISP_WRITE_SENSOR_ENABLE
            
            stI2cData.bDelayCfg = HI_FALSE;
            stI2cData.u8DevAddr = sensor_i2c_addr;
            stI2cData.u32AddrByteNum = sensor_addr_byte;
            stI2cData.u32DataByteNum = sensor_data_byte;
            
            stI2cData.u32RegAddr = 0x3020;
            stI2cData.u32Data = (exp_time & 0xFF);
            HI_MPI_ISP_I2cWrite(&stI2cData);  

            stI2cData.u32RegAddr = 0x3020+1;
            stI2cData.u32Data = ((exp_time & 0xFF00) >> 8);
            HI_MPI_ISP_I2cWrite(&stI2cData);

            
            stI2cData.u32RegAddr = 0x3020+2;
            stI2cData.u32Data = ((((exp_time & 0x10000) >> 16)+(current&0xFE)) );
            HI_MPI_ISP_I2cWrite(&stI2cData);
            
#else   
            sensor_write_register(EXPOSURE_ADDR, exp_time & 0xFF);
            sensor_write_register(EXPOSURE_ADDR + 1, (exp_time & 0xFF00) >> 8);
            current = sensor_read_register(EXPOSURE_ADDR+2);
            sensor_write_register(EXPOSURE_ADDR+2, (((exp_time & 0x10000) >> 16)+(current&0xFE)) );
#endif

        break;
        
        case 1: //WDR mode
            //short exposure
            if (p_inttime->exposure_ashort < p_inttime->min_lines_target)
            {
                p_inttime->exposure_ashort = p_inttime->min_lines_target;
            }
            if (p_inttime->exposure_ashort > p_inttime->max_lines_target)
            {
                p_inttime->exposure_ashort = p_inttime->max_lines_target;
            }
            
            exp_time = p_inttime->full_lines - p_inttime->exposure_ashort;
            current = sensor_read_register(EXPOSURE_ADDR+2);
#if CMOS_IMX236_ISP_WRITE_SENSOR_ENABLE

            stI2cData.bDelayCfg = HI_FALSE;
            stI2cData.u8DevAddr = sensor_i2c_addr;
            stI2cData.u32AddrByteNum = sensor_addr_byte;
            stI2cData.u32DataByteNum = sensor_data_byte;

            stI2cData.u32RegAddr = 0x3020;
            stI2cData.u32Data = exp_time & 0xFF;
            HI_MPI_ISP_I2cWrite(&stI2cData);  

            stI2cData.u32RegAddr = 0x3020+1;
            stI2cData.u32Data = ((exp_time & 0xFF00) >> 8);
            HI_MPI_ISP_I2cWrite(&stI2cData);  

            
            stI2cData.u32RegAddr = 0x3020+2;
            stI2cData.u32Data = (((exp_time & 0x10000) >> 16)+(current&0xFE));
            HI_MPI_ISP_I2cWrite(&stI2cData);  

            //long exposure
            exp_time = p_inttime->full_lines - (p_inttime->exposure_ashort << 4);
            stI2cData.u32RegAddr = 0x3023;
            stI2cData.u32Data = (exp_time & 0xFF);
            HI_MPI_ISP_I2cWrite(&stI2cData);

            stI2cData.u32RegAddr = 0x3023+1;
            stI2cData.u32Data = ( (exp_time & 0xFF00) >> 8);
            HI_MPI_ISP_I2cWrite(&stI2cData);

            current = sensor_read_register(LONG_EXPOSURE_ADDR+2);
            stI2cData.u32RegAddr = 0x3023+2;
            stI2cData.u32Data = (((exp_time & 0x10000) >> 16)+(current&0xFE));
            HI_MPI_ISP_I2cWrite(&stI2cData);
#else
            sensor_write_register(EXPOSURE_ADDR, exp_time & 0xFF);
            sensor_write_register(EXPOSURE_ADDR+1, (exp_time & 0xFF00) >> 8);
            current = sensor_read_register(EXPOSURE_ADDR+2);
            sensor_write_register(EXPOSURE_ADDR+2, (((exp_time & 0x10000) >> 16)+(current&0xFE)) );

            //long exposure
            exp_time = p_inttime->full_lines - (p_inttime->exposure_ashort << 4);
            
            sensor_write_register(LONG_EXPOSURE_ADDR, exp_time & 0xFF);
            sensor_write_register(LONG_EXPOSURE_ADDR+1, (exp_time & 0xFF00) >> 8);
            //current = sensor_read_register(LONG_EXPOSURE_ADDR+2);
            sensor_write_register(LONG_EXPOSURE_ADDR+2, (((exp_time & 0x10000) >> 16)+(current&0xFE)) );
#endif
        break;
    }

}

/*
 * This function applies the new vert blanking porch to the ISP registers.
 */
static __inline void cmos_vblanking_update(cmos_inttime_const_ptr_t p_inttime)
{
    HI_U16 vmax = p_inttime->full_lines;
    static HI_U16 last_vblanking_lines = 0xFFFF;
    
    if(vmax != last_vblanking_lines)
    {
        sensor_write_register(VMAX_ADDR, (vmax&0x00ff));
        sensor_write_register(VMAX_ADDR+1, ((vmax&0xff00) >> 8));
        last_vblanking_lines = vmax;
    }else
    {

    }
    return;
}

static __inline HI_U16 vblanking_calculate(
		cmos_inttime_ptr_t p_inttime)
{
    if (p_inttime->exposure_ashort >= p_inttime->full_lines - 2)
    {
        p_inttime->exposure_ashort = p_inttime->full_lines - 2;
    }
    else if(p_inttime->exposure_ashort < 1)
    {
        p_inttime->exposure_ashort = 1;
    }

    p_inttime->vblanking_lines = p_inttime->full_lines - p_inttime->full_lines_std;
    return p_inttime->exposure_ashort;
}


/* Set fps base */
static __inline void cmos_fps_set(
		cmos_inttime_ptr_t p_inttime,
		const HI_U8 fps
		)
{
    if (0 == gu8SensorMode) /*linear mode*/
    {
        switch(fps)
        {
            case 30:
                // Change the frame rate via changing the vertical blanking
                p_inttime->full_lines_std = p_inttime->full_lines_std_30fps;
                //p_inttime->max_lines = 1123;
                sensor_write_register(VMAX_ADDR, 0x65);
                sensor_write_register(VMAX_ADDR+1, 0x04);
                p_inttime->lines_per_500ms = p_inttime->full_lines_std_30fps * 30 / 2;
            break;
            
            case 25:
                // Change the frame rate via changing the vertical blanking
                p_inttime->full_lines_std = p_inttime->full_lines_std_25fps;
                //p_inttime->max_lines = 1348;
                sensor_write_register(VMAX_ADDR, 0x46);
                sensor_write_register(VMAX_ADDR+1, 0x05);
                p_inttime->lines_per_500ms = p_inttime->full_lines_std_25fps * 25 / 2;
            break;
            default:
                break;
    	}
    }
    else if (1 == gu8SensorMode)   /* Built-in WDR */
    {
        switch(fps)
        {
            case 30:
                sensor_write_register(HMAX_ADDR, 0x30);
                sensor_write_register(HMAX_ADDR+1, 0x11);
            break;
            case 25:
                sensor_write_register(HMAX_ADDR, 0xA0);
                sensor_write_register(HMAX_ADDR+1, 0x14);
            break;
            default:
                break;
    	}
    }
}

/*
 * This function initialises an instance of cmos_gains_t.
 */
static __inline cmos_gains_ptr_t cmos_gains_initialize()
{
    cmos_gains.again_shift = 10;
    cmos_gains.dgain_shift = 0;

    cmos_gains.isp_dgain_shift = 8;
    cmos_gains.isp_dgain = 1 << cmos_gains.isp_dgain_shift;
    cmos_gains.isp_dgain_delay_cfg = HI_FALSE;

    switch(gu8SensorMode)
    {
        default:
        case 0: //linear mode
            cmos_gains.max_again = 257217;
            cmos_gains.max_dgain = 1;
            cmos_gains.max_again_target = 257217;
            cmos_gains.max_dgain_target = 1;
            cmos_gains.max_isp_dgain_target = 4 << cmos_gains.isp_dgain_shift;
        break;
        case 1: //WDR mode
            cmos_gains.max_again = 4076;
            cmos_gains.max_dgain = 1;
            cmos_gains.max_again_target = 4076;
            cmos_gains.max_dgain_target = 1;
            cmos_gains.max_isp_dgain_target = 32 << cmos_gains.isp_dgain_shift;
        break;
    }

	return &cmos_gains;
}


static HI_U32 gain_table[481]=
{
    1024, 1035, 1047, 1059, 1072, 1084, 1097, 1109, 1122, 1135, 1148, 1162, 1175, 1189, 1203, 1217, 1231, 1245, 1259, 1274, 
    1289, 1304, 1319, 1334, 1349, 1365, 1381, 1397, 1413, 1429, 1446, 1463, 1480, 1497, 1514, 1532, 1549, 1567, 1585, 1604, 
    1622, 1641, 1660, 1679, 1699, 1719, 1739, 1759, 1779, 1800, 1820, 1842, 1863, 1884, 1906, 1928, 1951, 1973, 1996, 2019, 
    2043, 2066, 2090, 2114, 2139, 2164, 2189, 2214, 2240, 2266, 2292, 2318, 2345, 2373, 2400, 2428, 2456, 2484, 2513, 2542, 
    2572, 2601, 2632, 2662, 2693, 2724, 2756, 2788, 2820, 2852, 2886, 2919, 2953, 2987, 3022, 3057, 3092, 3128, 3164, 3201, 
    3238, 3275, 3313, 3351, 3390, 3430, 3469, 3509, 3550, 3591, 3633, 3675, 3717, 3760, 3804, 3848, 3893, 3938, 3983, 4029, 
    4076, 4123, 4171, 4219, 4268, 4318, 4368, 4418, 4469, 4521, 4574, 4627, 4680, 4734, 4789, 4845, 4901, 4957, 5015, 5073, 
    5132, 5191, 5251, 5312, 5374, 5436, 5499, 5562, 5627, 5692, 5758, 5825, 5892, 5960, 6029, 6099, 6170, 6241, 6313, 6387, 
    6461, 6535, 6611, 6688, 6765, 6843, 6923, 7003, 7084, 7166, 7249, 7333, 7418, 7504, 7591, 7678, 7767, 7857, 7948, 8040, 
    8133, 8228, 8323, 8419, 8517, 8615, 8715, 8816, 8918, 9021, 9126, 9232, 9338, 9447, 9556, 9667, 9779, 9892, 10006, 10122, 
    10240, 10358, 10478, 10599, 10722, 10846, 10972, 11099, 11227, 11357, 11489, 11622, 11757, 11893, 12030, 12170, 12311, 12453, 12597, 12743, 
    12891, 13040, 13191, 13344, 13498, 13655, 13813, 13973, 14135, 14298, 14464, 14631, 14801, 14972, 15146, 15321, 15498, 15678, 15859, 16043, 
    16229, 16417, 16607, 16799, 16994, 17190, 17390, 17591, 17795, 18001, 18209, 18420, 18633, 18849, 19067, 19288, 19511, 19737, 19966, 20197, 
    20431, 20668, 20907, 21149, 21394, 21642, 21892, 22146, 22402, 22662, 22924, 23189, 23458, 23730, 24004, 24282, 24564, 24848, 25136, 25427, 
    25721, 26019, 26320, 26625, 26933, 27245, 27561, 27880, 28203, 28529, 28860, 29194, 29532, 29874, 30220, 30570, 30924, 31282, 31644, 32011, 
    32381, 32756, 33135, 33519, 33907, 34300, 34697, 35099, 35505, 35916, 36332, 36753, 37179, 37609, 38045, 38485, 38931, 39382, 39838, 40299, 
    40766, 41238, 41715, 42198, 42687, 43181, 43681, 44187, 44699, 45216, 45740, 46270, 46805, 47347, 47896, 48450, 49011, 49579, 50153, 50734, 
    51321, 51915, 52517, 53125, 53740, 54362, 54992, 55628, 56272, 56924, 57583, 58250, 58925, 59607, 60297, 60995, 61702, 62416, 63139, 63870, 
    64610, 65358, 66114, 66880, 67655, 68438, 69230, 70032, 70843, 71663, 72493, 73333, 74182, 75041, 75910, 76789, 77678, 78577, 79487, 80408, 
    81339, 82281, 83233, 84197, 85172, 86158, 87156, 88165, 89186, 90219, 91264, 92320, 93389, 94471, 95565, 96671, 97791, 98923, 100069, 101227, 
    102400, 103585, 104785, 105998, 107225, 108467, 109723, 110994, 112279, 113579, 114894, 116225, 117570, 118932, 120309, 121702, 123111, 124537, 125979, 127438, 
    128913, 130406, 131916, 133444, 134989, 136552, 138133, 139733, 141351, 142988, 144643, 146318, 148013, 149726, 151460, 153214, 154988, 156783, 158598, 160435, 
    162293, 164172, 166073, 167996, 169941, 171909, 173900, 175913, 177950, 180011, 182095, 184204, 186337, 188495, 190677, 192885, 195119, 197378, 199664, 201976, 
    204314, 206680, 209073, 211494, 213943, 216421, 218927, 221462, 224026, 226620, 229244, 231899, 234584, 237301, 240048, 242828, 245640, 248484, 251362, 254272, 
    257217
};


/*
 * This function applies the new gains to the ISP registers.
 */
static __inline void cmos_gains_update(cmos_gains_const_ptr_t p_gains)
{    
    HI_U32 u32Tmp = p_gains->again_db + p_gains->dgain_db;

    u32Tmp = u32Tmp > 0x1E0 ? 0x1E0 : u32Tmp;

    static HI_U32 _last_gain = 0xFFFFFFFF;
    if(_last_gain == u32Tmp)
    {
        return;
    }else
    {
        _last_gain = u32Tmp;
    }

#if CMOS_IMX236_ISP_WRITE_SENSOR_ENABLE
    
    ISP_I2C_DATA_S stI2cData;
    stI2cData.bDelayCfg = HI_FALSE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;

    stI2cData.u32RegAddr = 0x3014;
    stI2cData.u32Data = (u32Tmp & 0xff);
    HI_MPI_ISP_I2cWrite(&stI2cData);  

    stI2cData.u32RegAddr = 0x3014+1;
    stI2cData.u32Data = ((u32Tmp & 0x100) >> 8);
    HI_MPI_ISP_I2cWrite(&stI2cData);  
#else
    sensor_write_register(PGC_ADDR, u32Tmp & 0xff);
    sensor_write_register(PGC_ADDR + 1, (u32Tmp & 0x100) >> 8);
#endif
}

static __inline HI_U32 analog_gain_from_exposure_calculate(
        cmos_gains_ptr_t p_gains,
        HI_U32 exposure,
        HI_U32 exposure_max,
        HI_U32 exposure_shift)
{
    HI_U32 again = 1024;
    HI_U32 step = 0;
    HI_S32 shift = 0;
    HI_S32 i = 0;
    HI_U32 u32MaxStep;

    while (exposure > (1 << 20))
    {
        exposure     >>= 1;
        exposure_max >>= 1;
        ++shift;
    }
    exposure_max = (exposure_max == 0)? 1: exposure_max;

    switch(gu8SensorMode)
    {
        default:
        case 0:
            u32MaxStep = 481;
        break;
        case 1:
            u32MaxStep = 121;
        break;
    }
    
    if(exposure > exposure_max)

    {
        again = (exposure << 10) / exposure_max;
        again = again < 1024? 1024: again;
        again = again > p_gains->max_again_target? p_gains->max_again_target: again;
    
        if (again >= gain_table[u32MaxStep - 1])
        {
             again = gain_table[u32MaxStep - 1];
             step = u32MaxStep - 1;
        }
        else
        {
            for(i = 1; i < u32MaxStep; i++)
            {
                if(again < gain_table[i])
                {
                    again = gain_table[i-1];
                    step = i-1 ;
                    break;
                }
            }
        }
        
        exposure = (exposure << 10) / again;
    }

    p_gains->again = again;
    p_gains->again_db = step;
    
    return exposure << shift;
}

static __inline HI_U32 isp_digital_gain_from_exposure_calculate(
        cmos_gains_ptr_t p_gains,
        HI_U32 exposure,
        HI_U32 exposure_max,
        HI_U32 exposure_shift)
{
    HI_U32 isp_dgain = (1 << p_gains->isp_dgain_shift);
    HI_S32 shift = 0;

    while (exposure > (1 << 22))
    {
        exposure >>= 1;
        exposure_max >>= 1;
        ++shift;
    }
    exposure_max = (exposure_max == 0)? 1: exposure_max;

    if(exposure > exposure_max)
    {
        isp_dgain = ((exposure << p_gains->isp_dgain_shift) + (exposure_max >> 1)) / exposure_max;
        exposure = exposure_max;
        isp_dgain = (isp_dgain > p_gains->max_isp_dgain_target) ? (p_gains->max_isp_dgain_target) : isp_dgain;        
    }
    
    p_gains->isp_dgain = isp_dgain;

    return exposure << shift;
}


static __inline HI_U32 digital_gain_from_exposure_calculate(
        cmos_gains_ptr_t p_gains,
        HI_U32 exposure,
        HI_U32 exposure_max,
        HI_U32 exposure_shift)
{
    int shft = 0;
    
    while (exposure > (1<<20)) /* analog use (1<<22) for analog exposure is bigger. */
    {
        exposure >>= 1;
        exposure_max >>= 1;
        ++shft;
    }

    p_gains->dgain = 1<<p_gains->dgain_shift;
    p_gains->dgain_db = 0;
  
    return exposure << shft;
}

void cmos_set_wdr_mode(HI_U8 u8Mode)
{
    switch(u8Mode)
    {
        //LINE MODE
        case 0:
            gu8SensorMode = 0;
            sensor_linner_init();
        break;
        //WDR MODE
        case 1:
            gu8SensorMode = 1;
            sensor_wdr_init();
        break;

        default:
            printf("NOT support this mode!\n");
            return;
        break;
    }
    
    return;
}

void setup_sensor(int isp_mode)
{
    if(0 == isp_mode) /* setup for ISP 'normal mode' */
    {
        sensor_write_register(VMAX_ADDR, 0x65);
        sensor_write_register(VMAX_ADDR + 1, 0x04);       
    }
    else if(1 == isp_mode) /* setup for ISP pixel calibration mode */
    {
        /* Sensor must be programmed for slow frame rate (5 fps and below) */
        /* change frame rate to 5 fps by setting 1 frame length = 1125 * (30/5) */
        sensor_write_register(VMAX_ADDR, 0x5E);
        sensor_write_register(VMAX_ADDR + 1, 0x1A);

        /* Analog and Digital gains both must be programmed for their minimum values */
        sensor_write_register(PGC_ADDR, 0x00);
        sensor_write_register(PGC_ADDR + 1, 0x00);
    }

    return;
}

static __inline HI_U32 cmos_get_ISO(cmos_gains_ptr_t p_gains)
{
    HI_U32  _again = p_gains->again == 0 ? 1 : p_gains->again;
    HI_U32 _dgain = p_gains->dgain == 0 ? 1 : p_gains->dgain;
    HI_U32 _isp_dgain = p_gains->isp_dgain == 0 ? 1 : p_gains->isp_dgain;

    p_gains->iso =  (((HI_U64)_again * _dgain * _isp_dgain * 100) 
        >> (p_gains->again_shift + p_gains->dgain_shift + p_gains->isp_dgain_shift));

    return p_gains->iso;
}

/* Note: The unit of return value is 1db.  */
static HI_U8 cmos_get_analog_gain(cmos_gains_ptr_t cmos_gains)
{
    return (cmos_gains->again_db *  3 / 10); 
}

/* Note: The unit of return value is 1db.  */
static HI_U8 cmos_get_digital_gain(cmos_gains_ptr_t cmos_gains)
{
    return  (cmos_gains->dgain_db *  3 / 10); 
}

static HI_U32 cmos_get_isp_default(cmos_isp_default_ptr_t p_coms_isp_default)
{
    if (NULL == p_coms_isp_default)
    {
        printf("null pointer when get isp default value!\n");
        return -1;
    }

    switch(gu8SensorMode)
    {
        default:
        case 0:
            memcpy(p_coms_isp_default, &st_coms_isp_default_lin, sizeof(cmos_isp_default_t));
        break;
        case 1:
            memcpy(p_coms_isp_default, &st_coms_isp_default_wdr, sizeof(cmos_isp_default_t));
        break;
    }
    
    return 0;
}


HI_U32 cmos_get_isp_speical_alg(void)
{
    return isp_special_alg_awb;
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

    switch(gu8SensorMode)
    {
        default:
        case 0:
            memcpy(p_cmos_isp_noise_table, &st_isp_noise_table, sizeof(cmos_isp_noise_table_t));
        break;
        case 1:
            memcpy(p_cmos_isp_noise_table, &st_isp_noise_table_wdr, sizeof(cmos_isp_noise_table_t));
        break;
    }
    return 0;
}

HI_U32 cmos_get_sensor_mode(void)
{
    switch(gu8SensorMode)
    {
        default:
        case 0:
            return isp_special_alg_imx104_lin;

        break;
        case 1:
            return isp_special_alg_imx104_wdr;

        break;
    }
}

static HI_S32 cmos_get_sensor_max_resolution(cmos_sensor_max_resolution_ptr_t p_cmos_sensor_max_resolution)
{
     if(NULL == p_cmos_sensor_max_resolution)
     {
       printf("null pointer when get sensor max resolution value!\n");
       return -1;
     }

     p_cmos_sensor_max_resolution->u32MaxWidth  = 1920;
     p_cmos_sensor_max_resolution->u32MaxHeight = 1080;

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
    .pfn_cmos_get_isp_special_alg = cmos_get_sensor_mode,
    .pfn_cmos_get_isp_agc_table = cmos_get_isp_agc_table,
    .pfn_cmos_get_isp_noise_table = cmos_get_isp_noise_table,
    .pfn_cmos_get_isp_demosaic = cmos_get_isp_demosaic,
    .pfn_cmos_get_isp_shading_table = NULL,
    .pfn_cmos_get_isp_gamma_table = NULL,    
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
    cmos_set_wdr_mode(u8Mode);
    return 0;

}

int  sensor_mode_get()
{
    return (int)gu8SensorMode;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif // __IMX236_CMOS_H_
