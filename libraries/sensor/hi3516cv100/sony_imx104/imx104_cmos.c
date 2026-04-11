#if !defined(__IMX104_CMOS_H_)
#define __IMX104_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hi_comm_sns.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "hi_isp_debug.h"
#include "hi_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/* Note: format of address is special.
 * chip_id + reg_adddr */
#define EXPOSURE_ADDR (0x220) //2:chip_id, 20: reg addr.
#define LONG_EXPOSURE_ADDR (0x223)
#define PGC_ADDR (0x214)
#define VMAX_ADDR (0x218)

#define CMOS_IMX104_ISP_WRITE_SENSOR_ENABLE (1)
/****************************************************************************
 * local variables															*
 ****************************************************************************/

static cmos_inttime_t cmos_inttime;
static cmos_gains_t cmos_gains;
HI_U8 gu8SensorMode = 0;

static cmos_isp_default_t st_coms_isp_default_lin =
{
    // color correction matrix
    {
        5000,
    	{
    		0x01D3, 0x80A0, 0x8033,
    		0x8036, 0x0192, 0x805C,
    		0x0023, 0x80CC, 0x01A9,
    	},
        3200,
    	{
    		0x01DA, 0x8094, 0x8046,
    		0x8064, 0x01B0, 0x804C,
    		0x0034, 0x8107, 0x01D3,
    	},
    	2600,
    	{
    		0x0231, 0x80E9, 0x8048,
    		0x8019, 0x014D, 0x8034,
    		0x0079, 0x81CA, 0x0251,
    	}
    },

	// black level for R, Gr, Gb, B channels
	{0xF0,0xF0,0xF0,0xF0},

	//calibration reference color temperature
	5000,

	//WB gain at 5000K, must keep consistent with calibration color temperature
	{0x01D6, 0x100, 0x100, 0x01DA},

	// WB curve parameters, must keep consistent with reference color temperature.
	{19, 147, -89, 186629, 128, -137989},

	// hist_thresh
	{0xd,0x28,0x60,0x80},

	0x00,	// iridix_balck
	0x2,	// 0:rggb; 2: gbrg

	// max again, max dgain
	0x10,	0x10,

	// iridix
	0x04,	0x08,	0xa0, 	0xfff,

	0x1, 	// balance_fe
	0x80,	// ae compensation
	0x8, 	// sinter threshold

	0x1,  //0: use default profile table; 1: use calibrated profile lut, the setting for nr0 and nr1 must be correct.
	0,    //nr0
	455,  //nr1

    2
};

static cmos_isp_default_t st_coms_isp_default_wdr =
{
    // color correction matrix
    {
        5000,
    	{
    		0x01D3, 0x80A0, 0x8033,
    		0x8036, 0x0192, 0x805C,
    		0x0023, 0x80CC, 0x01A9,
    	},
        3200,
    	{
    		0x01DA, 0x8094, 0x8046,
    		0x8064, 0x01B0, 0x804C,
    		0x0034, 0x8107, 0x01D3,
    	},
    	2600,
    	{
    		0x0231, 0x80E9, 0x8048,
    		0x8019, 0x014D, 0x8034,
    		0x0079, 0x81CA, 0x0251,
    	}
    },


	// black level(wdr)
	{0x00,0x00,0x00,0x00},

	//calibration reference color temperature
	5000,

	//WB gain at 5000K, must keep consistent with calibration color temperature
	{0x01D6, 0x100, 0x100, 0x01DA},

	// WB curve parameters, must keep consistent with reference color temperature.
	{19, 147, -89, 186629, 128, -137989},

	// hist_thresh
	{0x20,0x40,0x60,0x80},

	0x00,	// iridix_balck
	0x2,	// rggb

	// gain
	0x1,	0x4,

	//wdr_variance_space, wdr_variance_intensity, slope_max_write,  white_level_write
	0x08,	0x01,	0x3c, 	0xFFF,

	0x0, 	// balance_fe
	0x80,	// ae compensation
	0x9, 	// sinter threshold

	0x0,     //noise profile=0, use the default noise profile lut, don't need to set nr0 and nr1
	0x0,
	0x0,
      2
};


static cmos_isp_agc_table_t st_isp_agc_table =
{
    //sharpen_alt_d
    //{45,45,45,40,40,35,33,15},
    {80,75,70,65,60,50,40,30},

    //sharpen_alt_ud
    //{30,30,30,28,28,25,22,10},
    {75,70,65,60,50,40,30,20},

    //snr_thresh
    //{33,41,49,57,65,73,81,89},
    {8,12,18,26,36,46,56,70},

    //demosaic_lum_thresh
    {0x50,0x50,0x40,0x40,0x30,0x30,0x20,0x20},

    //demosaic_np_offset
    {0,8,16,24,32,40,48,56},

    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37},

    /* saturation */
    {0x80,0x80,0x6C,0x48,0x44,0x40,0x3C,0x38}
};

static cmos_isp_noise_table_t st_isp_noise_table =
{
  //nosie_profile_weight_lut
    {
        0,0,0,0,0,0,0,15,25,29,31,33,32,35,36,37,37,38,39,39,40,40,40,41,41,41,42,42,42,43,43,43,43,44,44,44,44,44,45,45,45,45,45,45,45,46,46,46,46,46,46,47,47,47,47,47,47,47,47,48,48,48,48,48,48,48,48,48,49,49,49,49,49,49,49,49,49,49,49,49,50,50,50,50,50,50,50,50,50,50,50,50,50,50,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52
    },

  //demosaic_weight_lut
    {
        0,15,25,29,31,33,34,35,36,37,37,38,39,39,40,40,40,41,41,41,42,42,42,43,43,43,43,44,44,44,44,44,45,45,45,45,45,45,46,46,46,46,46,46,46,47,47,47,47,47,47,47,47,48,48,48,48,48,48,48,48,48,49,49,49,49,49,49,49,49,49,49,49,49,49,50,50,50,50,50,50,50,50,50,50,50,50,50,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52
    }
};

static cmos_isp_noise_table_t st_isp_noise_table_wdr =
{
    /*nosie_profile_weight_lut WDR*/
    {
        0,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,45,48,57,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64
    },

  //demosaic_weight_lut
    {
        0,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,45,48,57,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64
    }
};


static cmos_isp_demosaic_t st_isp_demosaic =
{
    /*vh_slope*/
    0xc6,

    /*aa_slope*/
    0xc3,

    /*va_slope*/
    0xc4,

    /*uu_slope*/
    0xcd,

    /*sat_slope*/
    0x5d,

    /*ac_slope*/
    0xcf,

    /*vh_thresh*/
    0x0,

    /*aa_thresh*/
    0x0,

    /*va_thresh*/
    0x0,

    /*uu_thresh*/
    0x6,

    /*sat_thresh*/
    0x171,

    /*ac_thresh*/
    0x1b3,
};
/*
 * This function initialises an instance of cmos_inttime_t.
 */
static __inline cmos_inttime_const_ptr_t cmos_inttime_initialize()
{
	cmos_inttime.full_lines_std = 750;
	cmos_inttime.full_lines_std_30fps = 750;
	cmos_inttime.full_lines = 750;
	cmos_inttime.full_lines_limit = 65535;
	cmos_inttime.max_lines_target = 748;
	cmos_inttime.min_lines_target = 2;
	cmos_inttime.vblanking_lines = 0;

	cmos_inttime.exposure_ashort = 0;

	cmos_inttime.lines_per_500ms = 750*30/2; //
	cmos_inttime.flicker_freq = 0;//60*256;//50*256;

    switch(gu8SensorMode)
    {
        default:
        case 0: //linear mode

        break;
        case 1: //WDR mode for short exposure, Exposure ratio = 16X
            cmos_inttime.max_lines_target = 46;
            cmos_inttime.min_lines_target = 5;
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

    switch(gu8SensorMode)
    {
        default:
        case 0: //linear mode
            //Integration time = (VMAX - (SHS1+1)) + tOFFSET
            exp_time = p_inttime->full_lines - p_inttime->exposure_ashort - 1;
            current = sensor_read_register(EXPOSURE_ADDR+2);
#if CMOS_IMX104_ISP_WRITE_SENSOR_ENABLE

            ISP_SSP_DATA_S   stSspData;
            stSspData.bDelayCfg = HI_FALSE;
            stSspData.u32DevAddr = 0x02;
            stSspData.u32DevAddrByteNum = 1;
            stSspData.u32RegAddr = 0x20;
            stSspData.u32RegAddrByteNum = 1;
            stSspData.u32Data = (exp_time & 0xFF);
            stSspData.u32DataByteNum = 1;
            HI_MPI_ISP_SSPWrite(&stSspData);

            stSspData.u32RegAddr = 0x21;
            stSspData.u32RegAddrByteNum = 1;
            stSspData.u32Data = ((exp_time & 0xFF00) >> 8);            
            stSspData.u32DataByteNum = 1;
            HI_MPI_ISP_SSPWrite(&stSspData);

            stSspData.u32RegAddr = 0x22;
            stSspData.u32RegAddrByteNum = 1;
            stSspData.u32Data = (((exp_time & 0x10000) >> 16)+(current&0xFE));            
            stSspData.u32DataByteNum = 1;            
            HI_MPI_ISP_SSPWrite(&stSspData);



#else   


            sensor_write_register(EXPOSURE_ADDR, exp_time & 0xFF);
            sensor_write_register(EXPOSURE_ADDR+1, (exp_time & 0xFF00) >> 8);
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
            exp_time = p_inttime->full_lines - p_inttime->exposure_ashort - 1;
            current = sensor_read_register(EXPOSURE_ADDR+2);
#if CMOS_IMX104_ISP_WRITE_SENSOR_ENABLE

            stSspData.bDelayCfg = HI_FALSE;
            stSspData.u32DevAddr = 0x02;
            stSspData.u32DevAddrByteNum = 1;
            stSspData.u32RegAddr = 0x20;
            stSspData.u32RegAddrByteNum = 1;
            stSspData.u32Data = (exp_time & 0xFF);
            stSspData.u32DataByteNum = 1;
            HI_MPI_ISP_SSPWrite(&stSspData);

            stSspData.u32RegAddr = 0x21;
            stSspData.u32RegAddrByteNum = 1;
            stSspData.u32Data = ((exp_time & 0xFF00) >> 8);
            stSspData.u32DataByteNum = 1;
            HI_MPI_ISP_SSPWrite(&stSspData);

            stSspData.u32RegAddr = 0x22;
            stSspData.u32RegAddrByteNum = 1;
            stSspData.u32Data = (((exp_time & 0x10000) >> 16)+(current&0xFE));
            stSspData.u32DataByteNum = 1;
            HI_MPI_ISP_SSPWrite(&stSspData);

            exp_time = p_inttime->full_lines - (p_inttime->exposure_ashort << 4) - 1;
            current = sensor_read_register(LONG_EXPOSURE_ADDR+2);

            stSspData.u32RegAddr = 0x23;
            stSspData.u32RegAddrByteNum = 1;
            stSspData.u32Data = (exp_time & 0xFF);
            stSspData.u32DataByteNum = 1;
            HI_MPI_ISP_SSPWrite(&stSspData);

            stSspData.u32RegAddr = 0x24;
            stSspData.u32RegAddrByteNum = 1;
            stSspData.u32Data = ((exp_time & 0xFF00) >> 8);
            stSspData.u32DataByteNum = 1;
            HI_MPI_ISP_SSPWrite(&stSspData);

            stSspData.u32RegAddr = 0x25;
            stSspData.u32RegAddrByteNum = 1;
            stSspData.u32Data = (((exp_time & 0x10000) >> 16)+(current&0xFE));
            stSspData.u32DataByteNum = 1;
            HI_MPI_ISP_SSPWrite(&stSspData);

            
            
#else


            sensor_write_register(EXPOSURE_ADDR, exp_time & 0xFF);
            sensor_write_register(EXPOSURE_ADDR+1, (exp_time & 0xFF00) >> 8);
            sensor_write_register(EXPOSURE_ADDR+2, (((exp_time & 0x10000) >> 16)+(current&0xFE)) );

            //long exposure
            exp_time = p_inttime->full_lines - (p_inttime->exposure_ashort << 4) - 1;
            current = sensor_read_register(LONG_EXPOSURE_ADDR+2);

            sensor_write_register(LONG_EXPOSURE_ADDR, exp_time & 0xFF);
            sensor_write_register(LONG_EXPOSURE_ADDR+1, (exp_time & 0xFF00) >> 8);
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
    HI_U16  current;
    current = sensor_read_register(VMAX_ADDR+2);

	sensor_write_register(VMAX_ADDR, (vmax&0x00ff));
	sensor_write_register(VMAX_ADDR+1, ((vmax&0xff00) >> 8));
	sensor_write_register(VMAX_ADDR+2,(((vmax & 0x10000) >> 16)+(current&0xFE)));
	return;
}

static __inline HI_U16 vblanking_calculate(
		cmos_inttime_ptr_t p_inttime)
{
	if (p_inttime->exposure_ashort >= p_inttime->full_lines - 2)
	{
		p_inttime->exposure_ashort = p_inttime->full_lines - 2;
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
	switch(fps)
	{
		case 30:
			// Change the frame rate via changing the vertical blanking
			p_inttime->full_lines_std = 750;
			sensor_write_register(VMAX_ADDR, 0xEE);
			sensor_write_register(VMAX_ADDR+1, 0x02);
			p_inttime->lines_per_500ms = 750 * 30 / 2;
		break;

		case 25:
			// Change the frame rate via changing the vertical blanking
			p_inttime->full_lines_std = 900;
			sensor_write_register(VMAX_ADDR, 0x84);
			sensor_write_register(VMAX_ADDR+1, 0x03);
			p_inttime->lines_per_500ms = 900 * 25 / 2;
		break;

		default:
		break;
	}

}

/*
 * This function initialises an instance of cmos_gains_t.
 */
static __inline cmos_gains_ptr_t cmos_gains_initialize()
{
    cmos_gains.again = 1;
    cmos_gains.dgain = 1;
    cmos_gains.isp_dgain = 1;
    cmos_gains.again_db = 1;
    cmos_gains.dgain_db = 1;
	cmos_gains.again_shift = 10;
	cmos_gains.dgain_shift = 10;
	cmos_gains.dgain_fine_shift = 0;

    cmos_gains.isp_dgain_shift = 8;
    cmos_gains.isp_dgain = 1 << cmos_gains.isp_dgain_shift;
    cmos_gains.max_isp_dgain_target = 4 << cmos_gains.isp_dgain_shift;
    cmos_gains.isp_dgain_delay_cfg = HI_TRUE;

    switch(gu8SensorMode)
    {
        default:
        case 0: //linear mode
            cmos_gains.max_again = 16229;
            cmos_gains.max_dgain = 16229;
            cmos_gains.max_again_target = 16229;
            cmos_gains.max_dgain_target = 16229;
            cmos_gains.max_isp_dgain_target = 4 << cmos_gains.isp_dgain_shift;
        break;
        case 1: //WDR mode
            cmos_gains.max_again = 1 << cmos_gains.again_shift;
            cmos_gains.max_dgain = 4077;
            cmos_gains.max_again_target = 1 << cmos_gains.again_shift;
            cmos_gains.max_dgain_target = 4077;
            cmos_gains.max_isp_dgain_target = 32 << cmos_gains.isp_dgain_shift;
        break;
    }

	return &cmos_gains;
}

static HI_U32 gain_table[81] =
{
    1024 , 1060 ,  1097 ,  1136 ,  1176,  1217 , 1260 ,  1304,  1350 ,  1397 ,  1446 ,  1497 , 1550 , 1604 ,  1661 ,  1719 , 
    1780 , 1842 ,  1907 ,  1974 ,  2043,  2115 , 2189 ,  2266,  2346 ,  2428 ,  2514 ,  2602 , 2693 , 2788 ,  2886 ,  2987 , 
    3092 , 3201 ,  3314 ,  3430 ,  3551,  3675 , 3805 ,  3938,  4077 ,  4220 ,  4368 ,  4522 , 4681 , 4845 ,  5015 ,  5192 , 
    5374 , 5563 ,  5758 ,  5961 ,  6170,  6387 , 6611 ,  6844,  7084 ,  7333 ,  7591 ,  7858 , 8134 , 8420 ,  8716 ,  9022 , 
    9339 , 9667 , 10007 , 10359 , 10723, 11099 ,11489 , 11893, 12311 , 12744 , 13192 , 13655 ,14135 ,14632 , 15146 , 15678 , 
    16229     
};

/*
 * This function applies the new gains to the ISP registers.
 */
static __inline void cmos_gains_update(cmos_gains_const_ptr_t p_gains)
{
#if CMOS_IMX104_ISP_WRITE_SENSOR_ENABLE 

    ISP_SSP_DATA_S   stSspData;

    switch(gu8SensorMode)
    {
        default:
        case 0: /* linear mode */
            /* analog gain = APGC * 0.3 db, APGC = [0x0,0x50], ag_db=[0db, 24db] */
            /* digital gain = DPGC * 0.3 db, DPCG = [0x50,0xA0], dg_db=[0db, 24db] */
            
       
            stSspData.bDelayCfg = HI_TRUE;
            stSspData.u32DevAddr = 0x02;
            stSspData.u32DevAddrByteNum = 1;
            stSspData.u32RegAddr = 0x14;
            stSspData.u32RegAddrByteNum = 1;
            stSspData.u32Data = p_gains->again_db + p_gains->dgain_db;
            stSspData.u32DataByteNum = 1;

            if((p_gains->again_db + p_gains->dgain_db) <= 0xA0)
            {


              HI_MPI_ISP_SSPWrite(&stSspData);
            }
            else
            {

              
                stSspData.u32Data = 0xA0;
                stSspData.u32DataByteNum = 1;
                HI_MPI_ISP_SSPWrite(&stSspData);
            }
            

        break;
        case 1: //WDR mode

            /* analog gain : 4.5dB fixed
              * digital gain : 0 to 12dB  0.3dB step
              * DPGC = [00h,28h];
              * digital_gain = DPGC * 0.3 db. */

            stSspData.bDelayCfg = HI_TRUE;
            stSspData.u32DevAddr = 0x02;
            stSspData.u32DevAddrByteNum = 1;
            stSspData.u32RegAddr = 0x14;
            stSspData.u32RegAddrByteNum = 1;
            stSspData.u32Data =  p_gains->dgain_db;
            stSspData.u32DataByteNum = 1;

            if(p_gains->dgain_db <= 40)
            {
             HI_MPI_ISP_SSPWrite(&stSspData);
            }
            else
            {
            stSspData.u32Data = 0x28;
            stSspData.u32DataByteNum = 1;
            HI_MPI_ISP_SSPWrite(&stSspData);
            }
 
        break;
    }


#else

    switch(gu8SensorMode)
    {
        default:
        case 0: /* linear mode */
            /* analog gain = APGC * 0.3 db, APGC = [0x0,0x50], ag_db=[0db, 24db] */
            /* digital gain = DPGC * 0.3 db, DPCG = [0x50,0xA0], dg_db=[0db, 24db] */            
            if((p_gains->again_db + p_gains->dgain_db) <= 0xA0)
            {
                sensor_write_register(PGC_ADDR, p_gains->again_db + p_gains->dgain_db);
            }
            else
            {
                sensor_write_register(PGC_ADDR, 0xA0);
            }
            
        break;
        case 1: //WDR mode

            /* analog gain : 4.5dB fixed
              * digital gain : 0 to 12dB  0.3dB step
              * DPGC = [00h,28h];
              * digital_gain = DPGC * 0.3 db. */

            if(p_gains->dgain_db <= 40)
            {
                sensor_write_register(PGC_ADDR, p_gains->dgain_db);
            }
            else
            {
                sensor_write_register(PGC_ADDR, 0x28);
            }

        break;
    }

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

    while (exposure > (1 << 20))
    {
        exposure     >>= 1;
        exposure_max >>= 1;
        ++shift;
    }
    exposure_max = (exposure_max == 0)? 1: exposure_max;

    /*WDR mode, again 4.5dB fixed*/
    if((1 != gu8SensorMode) && (exposure > exposure_max))
    {
        again = (exposure << 10) / exposure_max;
        again = again < 1024? 1024: again;
        again = again > p_gains->max_again_target? p_gains->max_again_target: again;
    
        if (again >= gain_table[80])
        {
             again = gain_table[80];
             step = 80;
        }
        else
        {
            for(i = 1; i < 81; i++)
            {
                if(again < gain_table[i])
                {
                    again = gain_table[i-1];
                    step = i-1;
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
        isp_dgain = (isp_dgain > p_gains->max_isp_dgain_target)? (p_gains->max_isp_dgain_target): isp_dgain;        
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
    HI_U32 dgain = 1024;
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
        case 0: /*linear mode, dgain: 0.3 to 24dB(step 0.3dB)*/
            u32MaxStep = 81;
        break;
        case 1: /*WDR mode, dgain: 0.3 to 12dB(step 0.3dB), but now fixed to 0dB*/
            u32MaxStep = 41;
        break;
    }
    
    if(exposure > exposure_max)
    {
        dgain = (exposure << 10) / exposure_max;
        dgain = dgain < 1024? 1024: dgain;
        dgain = dgain > p_gains->max_dgain_target? p_gains->max_dgain_target: dgain;
    
        if (dgain >= gain_table[u32MaxStep - 1])
        {
             dgain = gain_table[u32MaxStep - 1];
             step = u32MaxStep - 1;
        }
        else
        {
            for(i = 1; i < u32MaxStep; i++)
            {
                if(dgain < gain_table[i])
                {
                    dgain = gain_table[i-1];
                    step = i-1 ;
                    break;
                }
            }
        }
        
        exposure = (exposure << 10) / dgain;
    }
 
    p_gains->dgain = dgain;
    p_gains->dgain_db = step;
    
    return exposure << shift;

}

static void setup_sensor(int isp_mode)
{
	if(0 == isp_mode) /* setup for ISP 'normal mode' */
	{
        sensor_write_register(VMAX_ADDR, 0xEE);
        sensor_write_register(VMAX_ADDR + 1, 0x02);
	}
	else if(1 == isp_mode) /* setup for ISP pixel calibration mode */
	{
        /* Sensor must be programmed for slow frame rate (5 fps and below)*/
        /* change frame rate to 5 fps by setting 1 frame length = 750 * 30 / 5 */
        sensor_write_register(VMAX_ADDR, 0x94);
        sensor_write_register(VMAX_ADDR + 1, 0x11);

        /* max Exposure time */
		sensor_write_register(EXPOSURE_ADDR, 0x00);
		sensor_write_register(EXPOSURE_ADDR + 1, 0x00);

        /* Analog and Digital gains both must be programmed for their minimum values */
		sensor_write_register(PGC_ADDR, 0x00);

	}
}

static __inline HI_U32 cmos_get_ISO(cmos_gains_ptr_t p_gains)
{

	HI_U32 _again = p_gains->again == 0 ? 1 : p_gains->again;
	HI_U32 _dgain = p_gains->dgain == 0 ? 1 : p_gains->dgain;
    HI_U32 _ispdgain = p_gains->isp_dgain == 0 ? 1 : p_gains->isp_dgain;

		p_gains->iso =  (((HI_U64)_again * _dgain * _ispdgain * 100) 
        >> (p_gains->again_shift + p_gains->dgain_shift 
        + p_gains->isp_dgain_shift));

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

/*
static HI_U8 cmos_get_digital_fine_gain(cmos_gains_ptr_t cmos_gains)
{
    return cmos_gains->dgain_fine;
}
*/

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

int sensor_mode_set(HI_U8 u8Mode)
{
    switch(u8Mode)
    {
        //720P30 linear
        case 0:
            gu8SensorMode = 0;
            sensor_write_register(0x20C, 0x00);
            sensor_write_register(0x20F, 0x01);
            sensor_write_register(0x210, 0x39);
            sensor_write_register(0x212, 0x50);
            sensor_write_register(0x265, 0x20);
            sensor_write_register(0x286, 0x01);
            sensor_write_register(0x2CF, 0xD1);
            sensor_write_register(0x2D0, 0x1B);
            sensor_write_register(0x2D2, 0x5F);
            sensor_write_register(0x2D3, 0x00);
            sensor_write_register(0x461, 0x9B);
            sensor_write_register(0x466, 0xD0);
            sensor_write_register(0x467, 0x08);

            printf("imx104 linear mode\n");
        break;

        //720P30 wdr
        case 1:
            gu8SensorMode = 1;
            sensor_write_register(0x20C, 0x02);
            sensor_write_register(0x20F, 0x05);
            sensor_write_register(0x210, 0x38);
            sensor_write_register(0x212, 0x0F);
            sensor_write_register(0x265, 0x00);
            sensor_write_register(0x286, 0x10);
            sensor_write_register(0x2CF, 0xE1);
            sensor_write_register(0x2D0, 0x29);
            sensor_write_register(0x2D2, 0x9B);
            sensor_write_register(0x2D3, 0x01);
            sensor_write_register(0x461, 0x9B);
            sensor_write_register(0x466, 0xD0);
            sensor_write_register(0x467, 0x08);

            printf("imx104 wdr mode\n");
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

#endif // __IMX104_CMOS_H_