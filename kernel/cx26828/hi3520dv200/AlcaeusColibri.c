/*+++ *******************************************************************\
*
*  Copyright and Disclaimer:
*
*     ---------------------------------------------------------------
*     This software is provided "AS IS" without warranty of any kind,
*     either expressed or implied, including but not limited to the
*     implied warranties of noninfringement, merchantability and/or
*     fitness for a particular purpose.
*     ---------------------------------------------------------------
*
*     Copyright (c) 2012 Conexant Systems, Inc.
*     All rights reserved.
*
\******************************************************************* ---*/


#include "AlcaeusColibriRegisters.h"
#include "AlcaeusColibri.h"

/* APIs */
/*******************************************************************************************************
 * ALCS_initColibri()
 *
 *******************************************************************************************************/



int ALCS_initColibri(const CX_COMMUNICATION *p_comm, const unsigned char freq_clk)
{
	// Step 1 - Setup clocks and tune analog filter
	int result = TRUE;

	result &= writeByte(p_comm, ALCS_AFEA_SUP_BLK_TUNE1, 0x41);
	result &= writeByte(p_comm, ALCS_AFEA_SUP_BLK_TUNE2, 0xC0);
	result &= writeByte(p_comm, ALCS_AFEA_SUP_BLK_XTAL, 0x0F);
	result &= writeByte(p_comm, ALCS_AFEA_SUP_BLK_PLL, 0x3F);
	result &= writeByte(p_comm, ALCS_AFEA_SUP_BLK_PLL2, freq_clk);  //48MHz crystal
	result &= writeByte(p_comm, ALCS_AFEA_SUP_BLK_PLL3, 0x67);
	result &= writeByte(p_comm, ALCS_AFEA_SUP_BLK_REF, 0x13); // default value (bit[4:3]ref_adc=0x2), Set ref_adc = 0x3 to get maximum input amplitude
	result &= writeByte(p_comm, ALCS_AFEA_SUP_BLK_PWRDN, 0x18);
	result &= writeByte(p_comm, ALCS_AFEA_SUP_BLK_TUNE3, 0x40);

	result &= writeByte(p_comm, ALCS_AFEB_SUP_BLK_TUNE1, 0x41);
	result &= writeByte(p_comm, ALCS_AFEB_SUP_BLK_TUNE2, 0xC0);
	result &= writeByte(p_comm, ALCS_AFEB_SUP_BLK_XTAL, 0x0F);
	result &= writeByte(p_comm, ALCS_AFEB_SUP_BLK_PLL, 0x3F);
	result &= writeByte(p_comm, ALCS_AFEB_SUP_BLK_PLL2, freq_clk);  //48MHz crystal
	result &= writeByte(p_comm, ALCS_AFEB_SUP_BLK_PLL3, 0x67);
	result &= writeByte(p_comm, ALCS_AFEB_SUP_BLK_REF, 0x13); // default value (bit[4:3]ref_adc=0x2), Set ref_adc = 0x3 to get maximum input amplitude
	result &= writeByte(p_comm, ALCS_AFEB_SUP_BLK_PWRDN, 0x18);
	result &= writeByte(p_comm, ALCS_AFEB_SUP_BLK_TUNE3, 0x40);

	p_comm->sleep(3);

	result &= writeByte(p_comm, ALCS_AFEA_SUP_BLK_TUNE3, 0x00);
	result &= writeByte(p_comm, ALCS_AFEA_ADC_INT5_STAB_REF, 0x52); // Set int5_boost = 0
	result &= writeByte(p_comm, ALCS_AFEB_SUP_BLK_TUNE3, 0x00);
	result &= writeByte(p_comm, ALCS_AFEB_ADC_INT5_STAB_REF, 0x52); // Set int5_boost = 0

	// Step 2 - power up all 8 channels, clear pd_buffer
	result &= writeByte(p_comm, ALCS_AFEA_CH1_ADC_PWRDN_CLAMP, 0x00);
	result &= writeByte(p_comm, ALCS_AFEA_CH2_ADC_PWRDN_CLAMP, 0x00);
	result &= writeByte(p_comm, ALCS_AFEA_CH3_ADC_PWRDN_CLAMP, 0x00);
	result &= writeByte(p_comm, ALCS_AFEA_CH4_ADC_PWRDN_CLAMP, 0x00);
	result &= writeByte(p_comm, ALCS_AFEB_CH1_ADC_PWRDN_CLAMP, 0x00);
	result &= writeByte(p_comm, ALCS_AFEB_CH2_ADC_PWRDN_CLAMP, 0x00);
	result &= writeByte(p_comm, ALCS_AFEB_CH3_ADC_PWRDN_CLAMP, 0x00);
	result &= writeByte(p_comm, ALCS_AFEB_CH4_ADC_PWRDN_CLAMP, 0x00);

	// Step 3 - Enable channel calibration
	result &= writeByte(p_comm, ALCS_AFEA_ADC_COM_QUANT, 0x02);
	result &= writeByte(p_comm, ALCS_AFEB_ADC_COM_QUANT, 0x02);

	result &= writeByte(p_comm, ALCS_AFEA_CH1_ADC_FB_FRCRST, 0x17);
	result &= writeByte(p_comm, ALCS_AFEA_CH2_ADC_FB_FRCRST, 0x17);
	result &= writeByte(p_comm, ALCS_AFEA_CH3_ADC_FB_FRCRST, 0x17);
	result &= writeByte(p_comm, ALCS_AFEA_CH4_ADC_FB_FRCRST, 0x17);
	result &= writeByte(p_comm, ALCS_AFEB_CH1_ADC_FB_FRCRST, 0x17);
	result &= writeByte(p_comm, ALCS_AFEB_CH2_ADC_FB_FRCRST, 0x17);
	result &= writeByte(p_comm, ALCS_AFEB_CH3_ADC_FB_FRCRST, 0x17);
	result &= writeByte(p_comm, ALCS_AFEB_CH4_ADC_FB_FRCRST, 0x17);

	p_comm->sleep(3);

	result &= writeByte(p_comm, ALCS_AFEA_CH1_ADC_CAL_ATEST, 0x10);
	result &= writeByte(p_comm, ALCS_AFEA_CH2_ADC_CAL_ATEST, 0x10);
	result &= writeByte(p_comm, ALCS_AFEA_CH3_ADC_CAL_ATEST, 0x10);
	result &= writeByte(p_comm, ALCS_AFEA_CH4_ADC_CAL_ATEST, 0x10);
	result &= writeByte(p_comm, ALCS_AFEB_CH1_ADC_CAL_ATEST, 0x10);
	result &= writeByte(p_comm, ALCS_AFEB_CH2_ADC_CAL_ATEST, 0x10);
	result &= writeByte(p_comm, ALCS_AFEB_CH3_ADC_CAL_ATEST, 0x10);
	result &= writeByte(p_comm, ALCS_AFEB_CH4_ADC_CAL_ATEST, 0x10);

	p_comm->sleep(3);

	result &= writeByte(p_comm, ALCS_AFEA_CH1_ADC_FB_FRCRST, 0x07);
	result &= writeByte(p_comm, ALCS_AFEA_CH2_ADC_FB_FRCRST, 0x07);
	result &= writeByte(p_comm, ALCS_AFEA_CH3_ADC_FB_FRCRST, 0x07);
	result &= writeByte(p_comm, ALCS_AFEA_CH4_ADC_FB_FRCRST, 0x07);
	result &= writeByte(p_comm, ALCS_AFEB_CH1_ADC_FB_FRCRST, 0x07);
	result &= writeByte(p_comm, ALCS_AFEB_CH2_ADC_FB_FRCRST, 0x07);
	result &= writeByte(p_comm, ALCS_AFEB_CH3_ADC_FB_FRCRST, 0x07);
	result &= writeByte(p_comm, ALCS_AFEB_CH4_ADC_FB_FRCRST, 0x07);

	// Step 4 - Setup each channel as single-ended input
	result &= writeByte(p_comm, ALCS_AFEA_CH1_ADC_NTF_PRECLMP_EN, 0xF0);
	result &= writeByte(p_comm, ALCS_AFEA_CH2_ADC_NTF_PRECLMP_EN, 0xF0);
	result &= writeByte(p_comm, ALCS_AFEA_CH3_ADC_NTF_PRECLMP_EN, 0xF0);
	result &= writeByte(p_comm, ALCS_AFEA_CH4_ADC_NTF_PRECLMP_EN, 0xF0);
	result &= writeByte(p_comm, ALCS_AFEB_CH1_ADC_NTF_PRECLMP_EN, 0xF0);
	result &= writeByte(p_comm, ALCS_AFEB_CH2_ADC_NTF_PRECLMP_EN, 0xF0);
	result &= writeByte(p_comm, ALCS_AFEB_CH3_ADC_NTF_PRECLMP_EN, 0xF0);
	result &= writeByte(p_comm, ALCS_AFEB_CH4_ADC_NTF_PRECLMP_EN, 0xF0);

	result &= writeByte(p_comm, ALCS_AFEA_CH1_ADC_INPUT, 0x00);
	result &= writeByte(p_comm, ALCS_AFEA_CH2_ADC_INPUT, 0x00);
	result &= writeByte(p_comm, ALCS_AFEA_CH3_ADC_INPUT, 0x00);
	result &= writeByte(p_comm, ALCS_AFEA_CH4_ADC_INPUT, 0x00);
	result &= writeByte(p_comm, ALCS_AFEB_CH1_ADC_INPUT, 0x00);
	result &= writeByte(p_comm, ALCS_AFEB_CH2_ADC_INPUT, 0x00);
	result &= writeByte(p_comm, ALCS_AFEB_CH3_ADC_INPUT, 0x00);
	result &= writeByte(p_comm, ALCS_AFEB_CH4_ADC_INPUT, 0x00);

	result &= writeByte(p_comm, ALCS_AFEA_CH1_ADC_DCSERVO_DEM, 0x01);
	result &= writeByte(p_comm, ALCS_AFEA_CH2_ADC_DCSERVO_DEM, 0x01);
	result &= writeByte(p_comm, ALCS_AFEA_CH3_ADC_DCSERVO_DEM, 0x01);
	result &= writeByte(p_comm, ALCS_AFEA_CH4_ADC_DCSERVO_DEM, 0x01);
	result &= writeByte(p_comm, ALCS_AFEB_CH1_ADC_DCSERVO_DEM, 0x01);
	result &= writeByte(p_comm, ALCS_AFEB_CH2_ADC_DCSERVO_DEM, 0x01);
	result &= writeByte(p_comm, ALCS_AFEB_CH3_ADC_DCSERVO_DEM, 0x01);
	result &= writeByte(p_comm, ALCS_AFEB_CH4_ADC_DCSERVO_DEM, 0x01);

	result &= writeByte(p_comm, ALCS_AFEA_CH1_ADC_CTRL_DAC1, 0x05);  // default value (bit[1:0]dac23_gain=0x1). Set dac23_gain = 0x3 to get maximum input amplitude
	result &= writeByte(p_comm, ALCS_AFEA_CH2_ADC_CTRL_DAC1, 0x05);  // default value (bit[1:0]dac23_gain=0x1). Set dac23_gain = 0x3 to get maximum input amplitude
	result &= writeByte(p_comm, ALCS_AFEA_CH3_ADC_CTRL_DAC1, 0x05);  // default value (bit[1:0]dac23_gain=0x1). Set dac23_gain = 0x3 to get maximum input amplitude
	result &= writeByte(p_comm, ALCS_AFEA_CH4_ADC_CTRL_DAC1, 0x05);  // default value (bit[1:0]dac23_gain=0x1). Set dac23_gain = 0x3 to get maximum input amplitude
	result &= writeByte(p_comm, ALCS_AFEB_CH1_ADC_CTRL_DAC1, 0x05);  // default value (bit[1:0]dac23_gain=0x1). Set dac23_gain = 0x3 to get maximum input amplitude
	result &= writeByte(p_comm, ALCS_AFEB_CH2_ADC_CTRL_DAC1, 0x05);  // default value (bit[1:0]dac23_gain=0x1). Set dac23_gain = 0x3 to get maximum input amplitude
	result &= writeByte(p_comm, ALCS_AFEB_CH3_ADC_CTRL_DAC1, 0x05);  // default value (bit[1:0]dac23_gain=0x1). Set dac23_gain = 0x3 to get maximum input amplitude
	result &= writeByte(p_comm, ALCS_AFEB_CH4_ADC_CTRL_DAC1, 0x05);  // default value (bit[1:0]dac23_gain=0x1). Set dac23_gain = 0x3 to get maximum input amplitude

	result &= writeByte(p_comm, ALCS_AFEA_CH1_ADC_CTRL_DAC23, 0x67);
	result &= writeByte(p_comm, ALCS_AFEA_CH2_ADC_CTRL_DAC23, 0x67);
	result &= writeByte(p_comm, ALCS_AFEA_CH3_ADC_CTRL_DAC23, 0x67);
	result &= writeByte(p_comm, ALCS_AFEA_CH4_ADC_CTRL_DAC23, 0x67);
	result &= writeByte(p_comm, ALCS_AFEB_CH1_ADC_CTRL_DAC23, 0x67);
	result &= writeByte(p_comm, ALCS_AFEB_CH2_ADC_CTRL_DAC23, 0x67);
	result &= writeByte(p_comm, ALCS_AFEB_CH3_ADC_CTRL_DAC23, 0x67);
	result &= writeByte(p_comm, ALCS_AFEB_CH4_ADC_CTRL_DAC23, 0x67);

	// Disable for termination resistor and 100kOhm resistor to mid voltage: term_en to 0, res_en to 0
	result &= writeByte(p_comm, ALCS_AFEA_CH1_ADC_QGAIN_RES_TRM, 0xf2);
	result &= writeByte(p_comm, ALCS_AFEA_CH2_ADC_QGAIN_RES_TRM, 0xf2);
	result &= writeByte(p_comm, ALCS_AFEA_CH3_ADC_QGAIN_RES_TRM, 0xf2);
	result &= writeByte(p_comm, ALCS_AFEA_CH4_ADC_QGAIN_RES_TRM, 0xf2);
	result &= writeByte(p_comm, ALCS_AFEB_CH1_ADC_QGAIN_RES_TRM, 0xf2);
	result &= writeByte(p_comm, ALCS_AFEB_CH2_ADC_QGAIN_RES_TRM, 0xf2);
	result &= writeByte(p_comm, ALCS_AFEB_CH3_ADC_QGAIN_RES_TRM, 0xf2);
	result &= writeByte(p_comm, ALCS_AFEB_CH4_ADC_QGAIN_RES_TRM, 0xf2);
	
	return result;

}


