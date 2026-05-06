/********************************************************************************
*
*  Copyright (C) 2017	NEXTCHIP Inc. All rights reserved.
*  Module		: Equalizer module- sub module
*  Description	: EQ common
*  Author		:
*  Date         :
*  Version		: Version 2.0
*
********************************************************************************
*  History      :
*
*
********************************************************************************/
#include <linux/string.h>
#include <linux/delay.h>

#include "video.h"
#include "common.h"
#include "eq_common.h"
#include "eq.h"

/*******************************************************************************
 * extern variable
 *******************************************************************************/
extern unsigned int 	nvp6134_iic_addr[4];
extern unsigned char 	ch_mode_status[16];
extern unsigned char	ch_vfmt_status[16];
extern unsigned int		nvp6134_cnt;
extern nvp6134_equalizer s_eq;
extern nvp6134_equalizer s_eq_type;

/*******************************************************************************
 * internal variable
 *******************************************************************************/

static int get_eq_setting_value( unsigned char ch, int tbl_num, int stage, int val);

/*******************************************************************************
 *
 *	This table is EQ distance table
 *  The customer can change this value according to the environment.
 *  	- EQ_DEFAULT_STAGE_VAL : Nextchip default distance value
 *		- EQ_USER_STAGE_VAL    : Custormer distance value
 *		s_eq_distance_table[eq_tbl_num][0][EQ_DEFAULT_STAGE_VAL]
 *		s_eq_distance_table[eq_tbl_num][0][EQ_USER_STAGE_VAL]
 *			{
 *
 *	 // in case of AHD, CHD
 *	 //Y SUM, reserved(customer value)
 *		{  0,     0},	// stage 0 : No EQ(NO INPUT)
 *		{381,     0},	// stage 1 : 0~100M(short)
 *		....
 *		....
 *
 *	 // in case of THD
 *	 //EQ pattern, reserved(customer value)
 *		{  0,     0},	// stage 0 : No EQ(NO INPUT)
 *		{381,     0},	// stage 1 : 0~100M(short)
 *		....
 *		....
 *
 *******************************************************************************/
#define EQ_DEFAULT_STAGE_VAL	0
#define EQ_USER_STAGE_VAL		1
unsigned int     g_slp_ahd[16] = {[0 ... 15] = 0};

static const int s_eq_ahd_bwmode_ahd_3m_18p_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    { 3600, 3960, 3600, 3960 },	/* stage 1 : 0~50M(short) */
    { 3961, 4047, 3961, 4047 },	/* stage 2 : 50~150M */
    { 4048,   95, 4048,   95 },	/* stage 3 : 150~250M */
    {   96,  267,   96,  267 },	/* stage 4 : 250~350M */
    {  268,  449,  268,  449 },	/* stage 5 : 350~450M */
    {  450,  700,  450,  700 },	/* stage 6 : 450~550M */
};

static const int s_eq_ahd_bwmode_ahd_3m_25p_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    { 3000, 3319, 3000, 3319 },	/* stage 1 : 0~50M(short) */
    { 3320, 3547, 3320, 3547 },	/* stage 2 : 50~150M */
    { 3548, 3964, 3548, 3964 },	/* stage 3 : 150~250M */
    { 3965,  386, 3965,  386 },	/* stage 4 : 250~350M */
    {  387,  853,  387,  853 },	/* stage 5 : 350~450M */
    {  854, 1100,  854, 1100 },	/* stage 6 : 450~550M */
};

static const int s_eq_ahd_bwmode_ahd_3m_30p_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    { 3000, 3337, 3000, 3337 },	/* stage 1 : 0~50M(short) */
    { 3338, 3668, 3338, 3668 },	/* stage 2 : 50~150M */
    { 3669,  113, 3669,  113 },	/* stage 3 : 150~250M */
    {  114,  711,  114,  711 },	/* stage 4 : 250~350M */
    {  712, 1205,  712, 1205 },	/* stage 5 : 350~450M */
    { 1206, 1500, 1206, 1500 },	/* stage 6 : 450~550M */
};

static const int s_eq_ahd_bwmode_ahd_4m_15p_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    { 3600, 3960, 3600, 3960 },	/* stage 1 : 0~50M(short) */
    { 3961, 4047, 3961, 4047 },	/* stage 2 : 50~150M */
    { 4048,   95, 4048,   95 },	/* stage 3 : 150~250M */
    {   96,  267,   96,  267 },	/* stage 4 : 250~350M */
    {  268,  449,  268,  449 },	/* stage 5 : 350~450M */
    {  450,  700,  450,  700 },	/* stage 6 : 450~550M */
};

static const int s_eq_ahd_bwmode_ahd_4m_25p_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    { 3000, 3319, 3000, 3319 },	/* stage 1 : 0~50M(short) */
    { 3320, 3547, 3320, 3547 },	/* stage 2 : 50~150M */
    { 3548, 3964, 3548, 3964 },	/* stage 3 : 150~250M */
    { 3965,  386, 3965,  386 },	/* stage 4 : 250~350M */
    {  387,  853,  387,  853 },	/* stage 5 : 350~450M */
    {  854, 1100,  854, 1100 },	/* stage 6 : 450~550M */
};

static const int s_eq_ahd_bwmode_ahd_4m_30p_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    { 3000, 3337, 3000, 3337 },	/* stage 1 : 0~50M(short) */
    { 3338, 3668, 3338, 3668 },	/* stage 2 : 50~150M */
    { 3669,  113, 3669,  113 },	/* stage 3 : 150~250M */
    {  114,  711,  114,  711 },	/* stage 4 : 250~350M */
    {  712, 1205,  712, 1205 },	/* stage 5 : 350~450M */
    { 1206, 1500, 1206, 1500 },	/* stage 6 : 450~550M */
};

static const int s_eq_ahd_bwmode_ahd_5m_12p_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    {  100,  292,  100,  292 },	/* stage 1 : 0~50M(short) */
    {  293,  363,  293,  363 },	/* stage 2 : 50~150M */
    {  364,  474,  364,  474 },	/* stage 3 : 150~250M */
    {  475,  606,  475,  606 },	/* stage 4 : 250~350M */
    {  607,  744,  607,  744 },	/* stage 5 : 350~450M */
    {  745,  900,  745,  900 },	/* stage 6 : 450~550M */
};

static const int s_eq_ahd_bwmode_ahd_5m_20p_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    { 3100, 3321, 3100, 3321 },	/* stage 1 : 0~50M(short) */
    { 3322, 3591, 3322, 3591 },	/* stage 2 : 50~150M */
    { 3592, 4073, 3592, 4073 },	/* stage 3 : 150~250M */
    { 4074,  461, 4074,  461 },	/* stage 4 : 250~350M */
    {  462,  909,  462,  909 },	/* stage 5 : 350~450M */
    {  910, 1400,  910, 1400 },	/* stage 6 : 450~550M */
};

static const int s_eq_chd_720p25_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    {  200,  351,  200,  351 },	/* stage 1 : 0~50M(short) */
    {  352,  407,  352,  407 },	/* stage 2 : 50~150M */
    {  408,  532,  408,  532 },	/* stage 3 : 150~250M */
    {  533,  747,  533,  747 },	/* stage 4 : 250~350M */
    {  748, 1043,  748, 1043 },	/* stage 5 : 350~450M */
    { 1044, 1500, 1044, 1500 },	/* stage 6 : 450~550M */
};

static const int s_eq_chd_720p30_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    {  200,  341,  200,  341 },	/* stage 1 : 0~50M(short) */
    {  342,  416,  342,  416 },	/* stage 2 : 50~150M */
    {  417,  572,  417,  572 },	/* stage 3 : 150~250M */
    {  573,  792,  573,  792 },	/* stage 4 : 250~350M */
    {  793, 1051,  793, 1051 },	/* stage 5 : 350~450M */
    //{ 1502, 1400, 1502, 1400 },	/* stage 6 : 450~550M */
	{ 1052, 1502, 1502, 1400 },	/* stage 6 : 450~550M */
};

static const int s_eq_chd_720p50_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    { 3100, 3778, 3100, 3778 },	/* stage 1 : 0~50M(short) */
    { 3779,  100, 3779, 4095 },	/* stage 2 : 50~150M */
    {  101,  761,    0,  761 },	/* stage 3 : 150~250M */
    {  762, 1573,  762, 1573 },	/* stage 4 : 250~350M */
    { 1574, 2358, 1574, 2358 },	/* stage 5 : 350~450M */
    { 2359, 3099, 2359, 3099 },	/* stage 6 : 450~550M */
};

static const int s_eq_chd_720p60_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    {  500, 1226,  500, 1226 },	/* stage 1 : 0~50M(short) */
    { 1227, 1721, 1227, 1721 },	/* stage 2 : 50~150M */
    { 1722, 2476, 1722, 2476 },	/* stage 3 : 150~250M */
    { 2477, 3142, 2477, 3142 },	/* stage 4 : 250~350M */
    { 3143, 3701, 3143, 3701 },	/* stage 5 : 350~450M */
    { 3702,  499, 3702,  499 },	/* stage 6 : 450~550M */
};

static const int s_eq_chd_1080p25_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    {  100,  416,  100,  416 },	/* stage 1 : 0~50M(short) */
    {  417,  704,  417,  704 },	/* stage 2 : 50~150M */
    {  705, 1199,  705, 1199 },	/* stage 3 : 150~250M */
    { 1200, 1759, 1200, 1759 },	/* stage 4 : 250~350M */
    { 1760, 2238, 1760, 2238 },	/* stage 5 : 350~450M */
    { 2239, 3200, 2239, 3200 },	/* stage 6 : 450~550M */
};

static const int s_eq_chd_1080p30_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    { 2300, 2743, 2300, 2743 },	/* stage 1 : 0~50M(short) */
    { 2744, 3074, 2744, 3074 },	/* stage 2 : 50~150M */
    { 3075, 3790, 3075, 3790 },	/* stage 3 : 150~250M */
    { 3791,  641, 3791,  641 },	/* stage 4 : 250~350M */
    {  642, 1530,  642, 1530 },	/* stage 5 : 350~450M */
    { 1531, 2299, 1531, 2299 },	/* stage 6 : 450~550M */
};

static const int s_eq_thd_bwmode_tvi_5m_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    {  100,  359,  100,  359 },	/* stage 1 : 0~50M(short) */
    {  360,  514,  360,  514 },	/* stage 2 : 50~150M */
    {  515,  749,  515,  749 },	/* stage 3 : 150~250M */
    {  750, 1006,  750, 1006 },	/* stage 4 : 250~350M */
    { 1007, 1200, 1007, 1200 },	/* stage 5 : 350~450M */
    { 1201, 1500, 1201, 1500 },	/* stage 6 : 450~550M */
};

static const int s_eq_thd_bwmode_tvi_3m_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    { 3000, 3736, 3000, 3736 },	/* stage 1 : 0~50M(short) */
    { 3737, 3960, 3737, 3960 },	/* stage 2 : 50~150M */
    { 3961,  306, 3961,  306 },	/* stage 3 : 150~250M */
    {  307,  705,  307,  705 },	/* stage 4 : 250~350M */
    {  706, 1057,  706, 1057 },	/* stage 5 : 350~450M */
    { 1058, 1500, 1058, 1500 },	/* stage 6 : 450~550M */
};

static const int s_eq_thd_bwmode_tfth30p_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    {  225,  700,  225,  700 },	/* stage 5 : 350~450M */
    {  701, 2047, 701, 2047  },	/* stage 6 : 450~550M */
    { 2048, 3120, 2048, 3120 },	/* stage 1 : 0~50M(short) */
    { 3121, 3204, 3121, 3204 },	/* stage 2 : 50~150M */
    { 3205, 3464, 3205, 3464 },	/* stage 3 : 150~250M */
    { 3465,  224, 3465,  224 },	/* stage 4 : 250~350M */
};

static const int s_eq_thd_bwmode_tfth25p_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    { 	 0,    0,    0,   0  },	/* stage 0 : No EQ(NO INPUT) */
    { 	 1,  356,    1, 356  },	/* stage 1 : 0~50M(short) */
    {  357,  540,  357, 540  },	/* stage 2 : 50~150M */
    {  541,  820,  541, 820  },	/* stage 3 : 150~250M */
    {  821, 1072,  821, 1072 },	/* stage 4 : 250~350M */
    { 1073, 1280, 1073, 1280 },	/* stage 5 : 350~450M */
    { 1281, 2000, 1281, 2000 },	/* stage 6 : 450~550M */
};

static const int s_eq_thd_bwmode_720_30PA_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
    {	0,    0,   0,    0 },	/* stage 0 : No EQ(NO INPUT) */
    {	1,  324,   1,  324 },	/* stage 1 : 0~50M(short) */
    { 325,  388, 325,  388 },	/* stage 2 : 50~150M */
    { 389,  484, 389,  484 },	/* stage 3 : 150~250M */
    { 485,  604, 485,  604 },	/* stage 4 : 250~350M */
    { 605,  764, 605,  764 },	/* stage 5 : 350~450M */
    { 765, 1200, 765, 1200 },	/* stage 6 : 450~550M */
};

static const int s_eq_thd_bwmode_720_25PA_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
	{	0,    0,   0,    0 },	/* stage 0 : No EQ(NO INPUT) */
	{	1,  320,   1,  320 },	/* stage 1 : 0~50M(short) */
	{ 321,  384, 321,  384 },	/* stage 2 : 50~150M */
	{ 385,  472, 385,  472 },	/* stage 3 : 150~250M */
	{ 473,  592, 473,  592 },	/* stage 4 : 250~350M */
	{ 593,  724, 593,  724 },	/* stage 5 : 350~450M */
	{ 725, 1200, 725, 1200 },	/* stage 6 : 450~550M */
};

static const int s_eq_thd_bwmode_720_30PB_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
	{	0,    0,   0,    0 },	/* stage 0 : No EQ(NO INPUT) */
	{	1,  324,   1,  324 },	/* stage 1 : 0~50M(short) */
	{ 325,  388, 325,  388 },	/* stage 2 : 50~150M */
	{ 389,  484, 389,  484 },	/* stage 3 : 150~250M */
	{ 485,  604, 485,  604 },	/* stage 4 : 250~350M */
	{ 605,  764, 605,  764 },	/* stage 5 : 350~450M */
	{ 765, 1200, 765, 1200 },	/* stage 6 : 450~550M */
};

static const int s_eq_thd_bwmode_720_25PB_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
	{	0,    0,   0,    0 },	/* stage 0 : No EQ(NO INPUT) */
	{	1,  320,   1,  320 },	/* stage 1 : 0~50M(short) */
	{ 321,  384, 321,  384 },	/* stage 2 : 50~150M */
	{ 385,  472, 385,  472 },	/* stage 3 : 150~250M */
	{ 473,  592, 473,  592 },	/* stage 4 : 250~350M */
	{ 593,  724, 593,  724 },	/* stage 5 : 350~450M */
	{ 725, 1200, 725, 1200 },	/* stage 6 : 450~550M */
};

static const int s_eq_thd_bwmode_720P50_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
	{ 	 0,    0,    0,   0  },	/* stage 0 : No EQ(NO INPUT) */
	{ 	 1,  356,    1, 356  },	/* stage 1 : 0~50M(short) */
	{  357,  524,  357, 524  },	/* stage 2 : 50~150M */
	{  525,  780,  525, 780  },	/* stage 3 : 150~250M */
	{  781, 1028,  781, 1028 },	/* stage 4 : 250~350M */
	{ 1029, 1232, 1029, 1232 },	/* stage 5 : 350~450M */
	{ 1233, 2000, 1233, 2000 },	/* stage 6 : 450~550M */
};

static const int s_eq_thd_bwmode_720P60_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
	{    0,    0,    0,    0 },	/* stage 0 : No EQ(NO INPUT) */
	{ 2560, 3696, 2560, 3696 },	/* stage 1 : 0~50M(short) */
	{ 3697, 3935, 3697, 3935 },	/* stage 2 : 50~150M */
	{ 3936,  239, 3936,  239 },	/* stage 3 : 150~250M */
	{  240,  659,  240,  659 },	/* stage 4 : 250~350M */
	{  660, 1040,  660, 1040 },	/* stage 5 : 350~450M */
	{ 1041, 2559, 1041, 2559 },	/* stage 6 : 450~550M */
};

static const int s_eq_ahd_bwmode_720_25P_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
	{	0,    0,   0,    0 },	/* stage 0 : No EQ(NO INPUT) */
	{	1,  440,   1,  440 },	/* stage 1 : 0~50M(short) */
	{ 441,  496, 441,  496 },	/* stage 2 : 50~150M */
	{ 497,  588, 497,  588 },	/* stage 3 : 150~250M */
	{ 589,  712, 589,  712 },	/* stage 4 : 250~350M */
	{ 713,  872, 713,  872 },	/* stage 5 : 350~450M */
	{ 873, 1300, 873, 1300 },	/* stage 6 : 450~550M */
};

static const int s_eq_ahd_bwmode_720_30P_table[7][4] =
{
//  COAX[min,max], UTP[min, max]
	{	0,    0,   0,    0 },	/* stage 0 : No EQ(NO INPUT) */
	{	1,  449,   1,  449 },	/* stage 1 : 0~50M(short) */
	{ 450,  529, 450,  529 },	/* stage 2 : 50~150M */
	{ 530,  620, 530,  620 },	/* stage 3 : 150~250M */
	{ 621,  748, 621,  748 },	/* stage 4 : 250~350M */
	{ 749,  928, 749,  928 },	/* stage 5 : 350~450M */
	{ 929, 1300, 929, 1300 },	/* stage 6 : 450~550M */
};

static const int s_eq_thd_5m_re_table[7][2] =
{
	{	0,    0},	/* stage 0 : No EQ(NO INPUT) */
	{ 516,    0},	/* stage 1 : 0~50M(short) */
	{1009,    0},	/* stage 2 : 50~150M */
	{1574,    0},	/* stage 3 : 150~250M */
	{2045,    0},	/* stage 4 : 250~350M */
	{2046,    0},	/* stage 5 : 350~450M */
	{2046,    0},	/* stage 6 : 450~550M */
};

static const int s_eq_thd_3m_re_table[7][2] =
{
	{	0,    0},	/* stage 0 : No EQ(NO INPUT) */
	{ 400,    0},	/* stage 1 : 0~50M(short) */
	{1091,    0},	/* stage 2 : 50~150M */
	{1648,    0},	/* stage 3 : 150~250M */
	{2045,    0},	/* stage 4 : 250~350M */
	{2046,    0},	/* stage 5 : 350~450M */
	{2046,    0},	/* stage 6 : 450~550M */
};

static const int s_eq_thd_re_table[7][2] =
{
	{	0,    0},	/* stage 0 : No EQ(NO INPUT) */
	{	1,    0},	/* stage 1 : 0~50M(short) */
	{	1,    0},	/* stage 2 : 50~150M */
	{	1,    0},	/* stage 3 : 150~250M */
	{1400,    0},	/* stage 4 : 250~350M */
	{2000,    0},	/* stage 5 : 350~450M */
	{2046,    0},	/* stage 6 : 450~550M */
};

static const int s_eq_thd_re_table_720P_A[7][2] =
{
	{	0,    0},	/* stage 0 : No EQ(NO INPUT) */
	{	1,    0},	/* stage 1 : 0~50M(short) */
	{	1,    0},	/* stage 2 : 50~150M */
	{	1,    0},	/* stage 3 : 150~250M */
	{   1,    0},	/* stage 4 : 250~350M */
	{1821,    0},	/* stage 5 : 350~450M */
	{2046,    0},	/* stage 6 : 450~550M */
};

static const int s_eq_cvi_re_table_720P50[7][2] =
{
	{	0,    0},	/* stage 0 : No EQ(NO INPUT) */
	{	1,    0},	/* stage 1 : 0~50M(short) */
	{	1,    0},	/* stage 2 : 50~150M */
	{	1,    0},	/* stage 3 : 150~250M */
	{   1,    0},	/* stage 4 : 250~350M */
	{   1,    0},	/* stage 5 : 350~450M */
	{2046,    0},	/* stage 6 : 450~550M */
};

static const int s_eq_cvi_re_table_720P60[7][2] =
{
	{	0,    0},	/* stage 0 : No EQ(NO INPUT) */
	{	1,    0},	/* stage 1 : 0~50M(short) */
	{	1,    0},	/* stage 2 : 50~150M */
	{	1,    0},	/* stage 3 : 150~250M */
	{   1,    0},	/* stage 4 : 250~350M */
	{   1,    0},	/* stage 5 : 350~450M */
	{2046,    0},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_1080p2550[7][2] =
{
   /*Y SUM, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{371,   380},	/* stage 1 : 0~50M(short) */
	{291,   280},	/* stage 2 : 50~150M */
	{223,   210},	/* stage 3 : 150~250M */
	{179,   150},	/* stage 4 : 250~350M */
	{146,   135},	/* stage 5 : 350~450M */
	{132,    93},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_1080p3060[7][2] =
{
   /*Y SUM, UTP */
	{  0,     0},		/* stage 0 : No EQ(NO INPUT) */
	{387,   350},	/* stage 1 : 0~50M(short) */
	{301,   270},	/* stage 2 : 50~150M */
	{227,   200},	/* stage 3 : 150~250M */
	{181,   170},	/* stage 4 : 250~350M */
	{150,    90},	/* stage 5 : 350~450M */
	{137,     0},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_720p2550[7][2] =
{
  /*Y SUM, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{440,   440},	/* stage 1 : 0~50M(short) */
	{395,   335},	/* stage 2 : 50~150M */
	{315,   290},	/* stage 3 : 150~250M */
	{255,   245},	/* stage 4 : 250~350M */
	{215,   200},	/* stage 5 : 350~450M */
	{190,     0},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_720p3060[7][2] =
{
  /*Y SUM, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{440,   370},	/* stage 1 : 0~50M(short) */
	{395,   300},	/* stage 2 : 50~150M */
	{315,   240},	/* stage 3 : 150~250M */
	{255,   217},	/* stage 4 : 250~350M */
	{215,   180},	/* stage 5 : 350~450M */
	{190,     0},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_720p2550_gain[7][2] =
{
  /*Y SUM,  UTP */
	{  0,      0},	/* stage 0 : No EQ(NO INPUT) */
	{178,    196},	/* stage 1 : 0~50M(short) */
	{285,    430},	/* stage 2 : 50~150M */
	{457,   1050},	/* stage 3 : 150~250M */
	{637,   1051},	/* stage 4 : 250~350M */
	{1073,  2046},	/* stage 5 : 350~450M */
	{1600,  2046},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_720p3060_gain[7][2] =
{
  /*Y SUM,  UTP */
	{  0,      0},	/* stage 0 : No EQ(NO INPUT) */
	{159,    196},	/* stage 1 : 0~50M(short) */
	{249,    430},	/* stage 2 : 50~150M */
	{365,   1050},	/* stage 3 : 150~250M */
	{591,   1051},	/* stage 4 : 250~350M */
	{1086,  2046},	/* stage 5 : 350~450M */
	{1600,  2046},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_chd_1080p30[7][2] =
{
  /*EQ pattern, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{ 78,   230},	/* stage 1 : 0~50M(short) */
	{209,   450},	/* stage 2 : 50~150M */
	{451,  1400},	/* stage 3 : 150~250M */
	{1026, 2047},	/* stage 4 : 250~350M */
	{1753, 2047},	/* stage 5 : 350~450M */
	{2046, 2047},	/* stage 6 : 450~550M */
};


static const int s_eq_distance_chd_1080p25[7][2] =
{
  /*EQ pattern, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{ 78,   230},	/* stage 1 : 0~50M(short) */
	{206,   450},	/* stage 2 : 50~150M */
	{441,  1400},	/* stage 3 : 150~250M */
	{1030, 2047},	/* stage 4 : 250~350M */
	{1764, 2047},	/* stage 5 : 350~450M */
	{2046, 2047},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_chd_720p30[7][2] =
{
  /*EQ pattern, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{ 57,   140},	/* stage 1 : 0~50M(short) */
	{110,   320},	/* stage 2 : 50~150M */
	{219,   900},	/* stage 3 : 150~250M */
	{443,  1600},	/* stage 4 : 250~350M */
	{860,  2047},	/* stage 5 : 350~450M */
	{1500, 2047},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_chd_720p60[7][2] =
{
  /*EQ pattern, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{225,   850},	/* stage 1 : 0~50M(short) */
	{561,  1200},	/* stage 2 : 50~150M */
	{1400, 1600},	/* stage 3 : 150~250M */
	{1701, 2047},	/* stage 4 : 250~350M */
	{1801, 2047},	/* stage 5 : 350~450M */
	{1801, 2047},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_chd_720p25[7][2] =
{
  /*EQ pattern, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{ 59,   140},	/* stage 1 : 0~50M(short) */
	{110,   320},	/* stage 2 : 50~150M */
	{216,   900},	/* stage 3 : 150~250M */
	{440,  1600},	/* stage 4 : 250~350M */
	{865,  2047},	/* stage 5 : 350~450M */
	{1450, 2047},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_chd_720p50[7][2] =
{
  /*EQ pattern, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{240,   550},	/* stage 1 : 0~50M(short) */
	{592,   850},	/* stage 2 : 50~150M */
	{1450, 1600},	/* stage 3 : 150~250M */
	{1701, 2047},	/* stage 4 : 250~350M */
	{1801, 2047},	/* stage 5 : 350~450M */
	{1801, 2047},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_thd_1080p25[7][2] =
{
 /*EQ pattern, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{188,   451},	/* stage 1 : 0~50M(short) */
	{717,   900},	/* stage 2 : 50~150M */
	{1500, 2047},	/* stage 3 : 150~250M */
	{1600, 2047},	/* stage 4 : 250~350M */
	{1701, 2047},	/* stage 5 : 350~450M */
	{1701, 2047},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_thd_1080p30[7][2] =
{
 /*EQ pattern, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{262,   451},	/* stage 1 : 0~50M(short) */
	{573,   900},	/* stage 2 : 50~150M */
	{1400, 2047},	/* stage 3 : 150~250M */
	{1500, 2047},	/* stage 4 : 250~350M */
	{1601, 2047},	/* stage 5 : 350~450M */
	{1601, 2047},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_thd_720p25A[7][2] =
{
 /*EQ pattern, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{196,   451},	/* stage 1 : 0~50M(short) */
	{662,  1600},	/* stage 2 : 50~150M */
	{1395, 2047},	/* stage 3 : 150~250M */
	{1950, 2047},	/* stage 4 : 250~350M */
	{2046, 2047},	/* stage 5 : 350~450M */
	{2046, 2047},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_thd_720p30A[7][2] =
{
 /*EQ pattern, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{184,   451},	/* stage 1 : 0~50M(short) */
	{517,  1600},	/* stage 2 : 50~150M */
	{1254, 2047},	/* stage 3 : 150~250M */
	{1632, 2047},	/* stage 4 : 250~350M */
	{1900, 2047},	/* stage 5 : 350~450M */
	{1901, 2047},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_thd_720p25B[7][2] =
{
 /*EQ pattern, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{126,   220},	/* stage 1 : 0~50M(short) */
	{241,   680},	/* stage 2 : 50~150M */
	{428,  1400},	/* stage 3 : 150~250M */
	{943,  1401},	/* stage 4 : 250~350M */
	{1699, 2046},	/* stage 5 : 350~450M */
	{1850, 2046},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_thd_720p30B[7][2] =
{
 /*EQ pattern, UTP */
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{125,   220},	/* stage 1 : 0~50M(short) */
	{240,   680},	/* stage 2 : 50~150M */
	{427,  1400},	/* stage 3 : 150~250M */
	{935,  2046},	/* stage 4 : 250~350M */
	{1691, 2046},	/* stage 5 : 350~450M */
	{1850, 2046},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_3m_18p[7][2] =
{
 /*EQ pattern, reserved*/
	{  0,     0},		/* stage 0 : No EQ(NO INPUT) */
	{516,     0},	/* stage 1 : 0~50M(short) */
	{395,     0},	/* stage 2 : 50~150M */
	{307,     0},	/* stage 3 : 150~250M */
	{246,     0},	/* stage 4 : 250~350M */
	{203,     0},	/* stage 5 : 350~450M */
	{ 91,     0},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_3m_25p[7][2] =
{
 /*EQ pattern, reserved*/
	{  0,     0},		/* stage 0 : No EQ(NO INPUT) */
	{405,     0},	/* stage 1 : 0~50M(short) */
	{255,     0},	/* stage 2 : 50~150M */
	{180,     0},	/* stage 3 : 150~250M */
	{125,     0},	/* stage 4 : 250~350M */
	{ 99,     0},	/* stage 5 : 350~450M */
	{ 50,     0},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_3m_30p[7][2] =
{
 /*EQ pattern, reserved*/
	{  0,     0},		/* stage 0 : No EQ(NO INPUT) */
	{405,     0},	/* stage 1 : 0~50M(short) */
	{255,     0},	/* stage 2 : 50~150M */
	{180,     0},	/* stage 3 : 150~250M */
	{125,     0},	/* stage 4 : 250~350M */
	{ 99,     0},	/* stage 5 : 350~450M */
	{ 50,     0},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_thd_3m_18p[7][2] =
{
 /*EQ pattern, reserved*/
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{725,     0},	/* stage 1 : 0~50M(short) */
	{1527,     0},	/* stage 2 : 50~150M */
	{2046,    0},	/* stage 3 : 150~250M */
	{2046,    0},	/* stage 4 : 250~350M */
	{2046,    0},	/* stage 5 : 350~450M */
	{2046,    0},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_thd_5m_12_5p[7][2] =
{
 /*EQ pattern, reserved*/
	{  0,     0},	/* stage 0 : No EQ(NO INPUT) */
	{ 516,     0},	/* stage 1 : 0~50M(short) */
	{1009,     0},	/* stage 2 : 50~150M */
	{1737,    0},	/* stage 3 : 150~250M */
	{2046,    0},	/* stage 4 : 250~350M */
	{2046,    0},	/* stage 5 : 350~450M */
	{2046,    0},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_4m_25p[7][2] =
{
 /*EQ pattern, reserved*/
	{  0,     0},		/* stage 0 : No EQ(NO INPUT) */
	{405,     0},	/* stage 1 : 0~50M(short) */
	{255,     0},	/* stage 2 : 50~150M */
	{180,     0},	/* stage 3 : 150~250M */
	{125,     0},	/* stage 4 : 250~350M */
	{ 99,     0},	/* stage 5 : 350~450M */
	{ 50,     0},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_4m_30p[7][2] =
{
 /*EQ pattern, reserved*/
	{  0,     0},		/* stage 0 : No EQ(NO INPUT) */
	{405,     0},	/* stage 1 : 0~50M(short) */
	{255,     0},	/* stage 2 : 50~150M */
	{180,     0},	/* stage 3 : 150~250M */
	{125,     0},	/* stage 4 : 250~350M */
	{ 99,     0},	/* stage 5 : 350~450M */
	{ 50,     0},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_4m_nrt[7][2] =
{
 /*EQ pattern, reserved*/
	{  0,     0},		/* stage 0 : No EQ(NO INPUT) */
	{515,     0},	/* stage 1 : 0~50M(short) */
	{387,     0},	/* stage 2 : 50~150M */
	{300,     0},	/* stage 3 : 150~250M */
	{239,     0},	/* stage 4 : 250~350M */
	{194,     0},	/* stage 5 : 350~450M */
	{101,     0},	/* stage 6 : 450~550M */
};


static const int s_eq_distance_5m_12_5p[7][2] =
{
 /*EQ pattern, reserved*/
	{  0,     0},		/* stage 0 : No EQ(NO INPUT) */
	{515,     0},	/* stage 1 : 0~50M(short) */
	{387,     0},	/* stage 2 : 50~150M */
	{300,     0},	/* stage 3 : 150~250M */
	{239,     0},	/* stage 4 : 250~350M */
	{194,     0},	/* stage 5 : 350~450M */
	{101,     0},	/* stage 6 : 450~550M */
};

static const int s_eq_distance_5m_20p[7][2] =
{
 /*EQ pattern, reserved*/
	{  0,     0},		/* stage 0 : No EQ(NO INPUT) */
	{405,     0},	/* stage 1 : 0~50M(short) */
	{255,     0},	/* stage 2 : 50~150M */
	{180,     0},	/* stage 3 : 150~250M */
	{125,     0},	/* stage 4 : 250~350M */
	{ 99,     0},	/* stage 5 : 350~450M */
	{ 50,     0},	/* stage 6 : 450~550M */
};

/*******************************************************************************
 *
 *	This table is EQ distance table
 *  The customer can change this value according to the environment.
 *  	- EQ_DEFAULT_STAGE_VAL : Nextchip default distance value
 *		- EQ_USER_STAGE_VAL    : Custormer distance value
 *		- EQ_H_DELAY_VAL	   : H delay value
 *		- EQ_recovery_val      : EQ Recovery start point value
 *		- EQ_recovery_val  	   : EQ Recovery end point value
 *		- Reserved value	   : Reserved value
 *		s_eq_distance_table[eq_tbl_num][0][EQ_DEFAULT_STAGE_VAL]
 *		s_eq_distance_table[eq_tbl_num][0][EQ_USER_STAGE_VAL]
 *		s_eq_distance_table[eq_tbl_num][0][EQ_H_DELAY_VAL]
 *		s_eq_distance_table[eq_tbl_num][0][EQ_S_RECERV_VAL]
 *		s_eq_distance_table[eq_tbl_num][0][EQ_E_RECERV_VAL]
 *		s_eq_distance_table[eq_tbl_num][0][RESERVED]
 *
 *			{
 *
 *******************************************************************************/
#define ACC_MIN_MARGIN	(0.9)
#define ACC_MAX_MARGIN	(1.1)

/* AFHD 25P, 50P */
static const int s_eq_1080p2550[7][8] =
{
	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
	{0x03, 0xa2, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
	{0xc3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
	{0x93, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
};

static const int s_eq_A_coeff_1080p2550[7][7] =
{
	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
};

static const int s_eq_B_coeff_1080p2550[7][7] =
{
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
};


static const int s_eq_color_1080p2550[7][18] =
{
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8c,       0x50,     0x76,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x80,        0xDC,     0x57,   0xa8,     0x88,    0x09,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8c,       0x50,     0x76,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x80,        0xDC,     0x57,   0xa8,     0x82,    0x09,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x85,       0x70,     0x7e,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x87,        0xDC,     0x57,   0xa8,     0x7c,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x8b,       0x30,     0x7d,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x81,        0xDC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x8b,       0x30,     0x7e,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x82,        0xDC,     0x57,   0xa8,     0x81,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8b,       0x20,     0x70,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x7b,        0xDC,     0x57,   0x70,     0x80,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8a,       0x20,     0x70,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x7d,        0xDC,     0x57,   0x50,     0x80,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
};

/* AFHD 30P, 60P */
static const int s_eq_1080p3060[7][8] =
{
 	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa2, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xc3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0x93, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
};

static const int s_eq_A_coeff_1080p3060[7][7] =
{
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
};

static const int s_eq_B_coeff_1080p3060[7][7] =
{
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
};

static const int s_eq_color_1080p3060[7][18] =
{
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8c,       0x50,     0x76,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x80,        0xDC,     0x57,   0xa8,     0x88,    0x09,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8c,       0x50,     0x76,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x80,        0xDC,     0x57,   0xa8,     0x88,    0x09,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x85,       0x70,     0x7e,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x87,        0xDC,     0x57,   0xa8,     0x82,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x8b,       0x30,     0x7d,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x81,        0xDC,     0x57,   0xa8,     0x86,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x8b,       0x30,     0x7e,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x82,        0xDC,     0x57,   0xa8,     0x87,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8b,       0x20,     0x70,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x7b,        0xDC,     0x57,   0x70,     0x86,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8a,       0x20,     0x70,     0xff, 0x10,   0x10,    0xf6,     0xf4,     0x7d,        0xDC,     0x57,   0x50,     0x86,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
};

/* AHD 25, 50P */
static const int s_eq_720p2550[7][8] =
{
	/*AEQ, DEQ1, DEQ2,  G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa1, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0x03, 0xa2, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0x03, 0xa3, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xd3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0xc3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 //{0x93, 0xa7, 0xa7,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 7 : 550~650M */
 //{0x33, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 8 : 650M ~  */ 		
};

static const int s_eq_A_coeff_720p2550[7][7] =
{
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */	 	
};

static const int s_eq_B_coeff_720p2550[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 7 : 550~650M */
//	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 8 : 650M ~ */

 };

static const int s_eq_color_720p2550[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8d,       0x20,     0x7b,     0x02, 0x00,   0x00,    0x00,     0x00,     0x80,        0xDC,     0x57,   0xa8,     0x80,   0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8d,       0x20,     0x7b,     0x02, 0x00,   0x00,    0x00,     0x00,     0x80,        0xDC,     0x57,   0xa8,     0x80,   0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x84,       0x40,     0x76,     0x02, 0x00,   0x00,    0x00,     0x00,     0x86,        0xDC,     0x57,   0xa8,     0x7d,   0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x8c,       0x40,     0x72,     0x02, 0x00,   0x00,    0x00,     0x00,     0x84,        0xDC,     0x57,   0xa8,     0x7a,   0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x8b,       0x40,     0x74,     0x02, 0x00,   0x00,    0x00,     0x00,     0x80,        0xDC,     0x57,   0xa8,     0x75,   0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x84,       0x20,     0x76,     0x02, 0x00,   0x00,    0x00,     0x00,     0x88,        0xDC,     0x57,   0xa8,     0x77,   0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x84,       0x20,     0x76,     0x02, 0x00,   0x00,    0x00,     0x00,     0x88,        0xDC,     0x57,   0xa8,     0x79,   0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };

/* AHD 30, 60P */
 static const int s_eq_720p3060[7][8] =
{
 	/*AEQ, DEQ1, DEQ2,  G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa1, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0x03, 0xa2, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0x03, 0xa3, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xd3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0xc3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
  //{0x93, 0xa7, 0xa7,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 7 : 550~650M */
  //{0x33, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 8 : 650M ~  */ 	
};

static const int s_eq_A_coeff_720p3060[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */	 	
 };

static const int s_eq_B_coeff_720p3060[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 7 : 550~650M */
//	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 8 : 650M ~ */
 };

static const int s_eq_color_720p3060[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8d,       0x20,     0x7b,     0x01, 0x00,   0x00,    0x00,     0x00,     0x80,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8d,       0x20,     0x7b,     0x01, 0x00,   0x00,    0x00,     0x00,     0x80,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x88,       0x30,     0x7f,     0x01, 0x00,   0x00,    0x00,     0x00,     0x85,        0xDC,     0x57,   0xa8,    0x7d,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x88,       0x40,     0x7e,     0x00, 0x00,   0x00,    0x00,     0x00,     0x84,        0xDC,     0x57,   0xa8,    0x7a,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x8a,       0x40,     0x7e,     0x00, 0x00,   0x00,    0x00,     0x00,     0x7e,        0xDC,     0x57,   0xa8,    0x73,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x82,       0x20,     0x76,     0x00, 0x00,   0x00,    0x00,     0x00,     0x84,        0xDC,     0x57,   0xa8,    0x82,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x83,       0x20,     0x76,     0x00, 0x00,   0x00,    0x00,     0x00,     0x86,        0xDC,     0x57,   0xa8,    0x7b,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };

/* AHD 25, 50P NO LE version. - no update yet */
 static const int s_eq_720p2550_gain[7][8] =
 {
 	/*AEQ, DEQ1, DEQ2,  G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa1, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0x03, 0xa2, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0x03, 0xa3, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xd3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0xc3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
  //{0x93, 0xa7, 0xa7,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 7 : 550~650M */
  //{0x33, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 8 : 650M ~  */ 	 	
 	
 };

static const int s_eq_A_coeff_720p2550_gain[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */	 
 };

static const int s_eq_B_coeff_720p2550_gain[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 7 : 550~650M */
//	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 8 : 650M ~ */
 };

static const int s_eq_color_720p2550_gain[7][18] =
 {
  	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8d,       0x20,     0x7b,     0x01, 0x00,   0x00,    0x00,     0x00,     0x80,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8d,       0x20,     0x7b,     0x01, 0x00,   0x00,    0x00,     0x00,     0x80,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x88,       0x30,     0x7f,     0x01, 0x00,   0x00,    0x00,     0x00,     0x85,        0xDC,     0x57,   0xa8,    0x7d,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x88,       0x40,     0x7e,     0x00, 0x00,   0x00,    0x00,     0x00,     0x84,        0xDC,     0x57,   0xa8,    0x7a,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x8a,       0x40,     0x7e,     0x00, 0x00,   0x00,    0x00,     0x00,     0x7e,        0xDC,     0x57,   0xa8,    0x73,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x82,       0x20,     0x76,     0x00, 0x00,   0x00,    0x00,     0x00,     0x84,        0xDC,     0x57,   0xa8,    0x82,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x83,       0x20,     0x76,     0x00, 0x00,   0x00,    0x00,     0x00,     0x86,        0xDC,     0x57,   0xa8,    0x7b,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };

/* AHD 30, 60P - no update yet */
 static const int s_eq_720p3060_gain[7][8] =
 {
	/*AEQ, DEQ1, DEQ2,  G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa1, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0x03, 0xa2, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0x03, 0xa3, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xd3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0xc3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
  //{0x93, 0xa7, 0xa7,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 7 : 550~650M */
  //{0x33, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 8 : 650M ~  */
 };

static const int s_eq_A_coeff_720p3060_gain[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */	 
 };

static const int s_eq_B_coeff_720p3060_gain[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 7 : 550~650M */
//	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 8 : 650M ~ */
 };

static const int s_eq_color_720p3060_gain[7][18] =
{
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8d,       0x20,     0x7b,     0x01, 0x00,   0x00,    0x00,     0x00,     0x80,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8d,       0x20,     0x7b,     0x01, 0x00,   0x00,    0x00,     0x00,     0x80,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x88,       0x30,     0x7f,     0x01, 0x00,   0x00,    0x00,     0x00,     0x85,        0xDC,     0x57,   0xa8,    0x7d,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x88,       0x40,     0x7e,     0x00, 0x00,   0x00,    0x00,     0x00,     0x84,        0xDC,     0x57,   0xa8,    0x7a,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x8a,       0x40,     0x7e,     0x00, 0x00,   0x00,    0x00,     0x00,     0x7e,        0xDC,     0x57,   0xa8,    0x73,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x82,       0x20,     0x76,     0x00, 0x00,   0x00,    0x00,     0x00,     0x84,        0xDC,     0x57,   0xa8,    0x82,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x83,       0x20,     0x76,     0x00, 0x00,   0x00,    0x00,     0x00,     0x86,        0xDC,     0x57,   0xa8,    0x7b,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };

#ifdef SUPPORT_HI3520D 
/* CFHD 30P - no update yet . now, HI3531A CVI 1080@30p value */
static const int s_eq_chd_1080p30[7][8] =
{
	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
	{0x03, 0xa7, 0xa3,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
	{0xc3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
	{0x93, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
};

static const int s_eq_chd_A_coeff_1080p30[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_chd_B_coeff_1080p30[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_chd_color_1080p30[7][18] =
 {
  	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8b,       0x20,     0x83,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x80,        0xDC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8b,       0x20,     0x83,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x80,        0xDC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x8b,       0x20,     0x83,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x84,        0xDC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x87,       0x00,     0x82,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x8d,        0xDC,     0x57,   0xa8,     0x82,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x84,       0x00,     0x73,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x88,        0xDC,     0x57,   0xa8,     0x82,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8c,       0x00,     0x76,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x7f,        0xDC,     0x57,   0x50,     0x80,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8a,       0x00,     0x70,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x83,        0xDC,     0x57,   0x30,     0x80,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };

#else
/* CFHD 30P */
static const int s_eq_chd_1080p30[7][8] =
{
	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
	{0x03, 0xa2, 0xa2,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
	{0xd3, 0x00, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
	{0x93, 0x00, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
	{0x53, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
	{0x93, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
};

static const int s_eq_chd_A_coeff_1080p30[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_chd_B_coeff_1080p30[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };

static const int s_eq_chd_color_1080p30[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8b,       0x20,     0x83,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x80,        0xDC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8b,       0x20,     0x60,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x80,        0xDC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x8b,       0x20,     0x60,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x84,        0xDC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x87,       0x00,     0x60,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x8d,        0xDC,     0x57,   0xa8,     0x82,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x84,       0x00,     0x60,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x88,        0xDC,     0x57,   0xa8,     0x82,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8c,       0x00,     0x60,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x7f,        0xDC,     0x57,   0x50,     0x80,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8a,       0x00,     0x60,     0x21, 0x00,   0x08,    0xfa,     0xf5,     0x83,        0xDC,     0x57,   0x30,     0x80,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };
#endif



#ifdef SUPPORT_HI3520D
/* CFHD 25P - no update yet . now, HI3531A CVI 1080@30p value */
static const int s_eq_chd_1080p25[7][8] =
{
	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
	{0x03, 0xa7, 0xa3,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
	{0xc3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
	{0x93, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
};

static const int s_eq_chd_A_coeff_1080p25[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_chd_B_coeff_1080p25[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };

static const int s_eq_chd_color_1080p25[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8c,       0x20,     0x83,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x7e,        0xDC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8c,       0x20,     0x85,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x7e,        0xDC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x8c,       0x20,     0x85,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x82,        0xDC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x88,       0x00,     0x84,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa8,     0x82,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x85,       0x00,     0x75,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x86,        0xDC,     0x57,   0xa8,     0x82,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8d,       0x00,     0x78,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x7d,        0xDC,     0x57,   0x50,     0x80,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8b,       0x00,     0x72,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x81,        0xDC,     0x57,   0x30,     0x80,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };
#else
/* CFHD 25P - no update yet . now, HI3531A CVI 1080@30p value */
static const int s_eq_chd_1080p25[7][8] =
{
	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
	{0x03, 0xa2, 0xa2,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
	{0xd3, 0x00, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
	{0x93, 0x00, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
	{0x53, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
	{0x93, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
};


static const int s_eq_chd_A_coeff_1080p25[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_chd_B_coeff_1080p25[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };

static const int s_eq_chd_color_1080p25[7][18] =
 {
	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8c,       0x20,     0x83,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x7e,        0xDC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8c,       0x20,     0x60,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x7e,        0xDC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x8c,       0x20,     0x60,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x82,        0xDC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x88,       0x00,     0x60,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa8,     0x82,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x85,       0x00,     0x60,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x86,        0xDC,     0x57,   0xa8,     0x82,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8d,       0x00,     0x60,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x7d,        0xDC,     0x57,   0x50,     0x80,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8b,       0x00,     0x60,     0x20, 0x00,   0x08,    0xfa,     0xf5,     0x81,        0xDC,     0x57,   0x30,     0x80,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };
#endif


#ifdef SUPPORT_HI3520D
/* CHD 30P - no update yet . now, HI3531A CVI 720@30p value */
static const int s_eq_chd_720p30[7][8] =
 {
	/*AEQ, DEQ1, DEQ2,  G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa1, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0x03, 0xa2, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0x03, 0xa3, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xd3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0xc3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 //{0x93, 0xa7, 0xa7,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 7 : 550~650M */
 //{0x33, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 8 : 650M ~  */ 
 };

static const int s_eq_chd_A_coeff_720p30[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */	 
 };

static const int s_eq_chd_B_coeff_720p30[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_chd_color_720p30[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x08,         0x8c,       0x20,     0x84,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x08,         0x8c,       0x20,     0x84,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x08,         0x87,       0x20,     0x88,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x8c,        0xDC,     0x57,   0xa8,    0x7d,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x08,         0x87,       0x40,     0x87,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa8,    0x7a,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x08,         0x89,       0x40,     0x87,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x85,        0xDC,     0x57,   0xa8,    0x73,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x08,         0x81,       0x20,     0x7f,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa0,    0x82,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x08,         0x82,       0x20,     0x7f,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x8d,        0xDC,     0x57,   0x90,    0x7b,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
};

#else
/* CHD 30P - no update yet . */
static const int s_eq_chd_720p30[7][8] =
 {
	/*AEQ, DEQ1, DEQ2,  G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa2, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0x03, 0xa2, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xe3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xc3, 0xa1, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0x93, 0x00, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 //{0x93, 0xa7, 0xa7,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 7 : 550~650M */
 //{0x33, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 8 : 650M ~  */
 };

static const int s_eq_chd_A_coeff_720p30[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */
 };

static const int s_eq_chd_B_coeff_720p30[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 7 : 550~650M */
// 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 8 : 650M~ */
 };

static const int s_eq_chd_color_720p30[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x08,         0x8c,       0x20,     0x68,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x08,         0x8c,       0x20,     0x68,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x08,         0x87,       0x20,     0x68,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x8c,        0xDC,     0x57,   0xa8,    0x7d,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x08,         0x87,       0x40,     0x68,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa8,    0x7a,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x08,         0x89,       0x40,     0x68,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x85,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x08,         0x81,       0x20,     0x68,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa0,    0x82,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x08,         0x82,       0x20,     0x68,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x8d,        0xDC,     0x57,   0x90,    0x7b,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };
#endif



#ifdef SUPPORT_HI3520D
/* CHD 25P - no update yet . now, HI3531A CVI 720@25p value */
static const int s_eq_chd_720p25[7][8] =
 {
	/*AEQ, DEQ1, DEQ2,  G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa1, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0x03, 0xa2, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0x03, 0xa3, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xd3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0xc3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 //{0x93, 0xa7, 0xa7,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 7 : 550~650M */
 //{0x33, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 8 : 650M ~  */ 
 };

static const int s_eq_chd_A_coeff_720p25[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */	 
 };

static const int s_eq_chd_B_coeff_720p25[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */	 
 };


static const int s_eq_chd_color_720p25[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x08,         0x8a,       0x20,     0x84,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,     0xd0,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x08,         0x8a,       0x20,     0x84,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,     0xd0,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x08,         0x85,       0x20,     0x88,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x8c,        0xDC,     0x57,   0xa8,     0xcd,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x08,         0x85,       0x40,     0x87,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa8,     0xca,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x08,         0x87,       0x40,     0x87,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x85,        0xDC,     0x57,   0xa8,     0xc3,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x08,         0x7f,       0x20,     0x7f,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa0,     0xd2,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x08,         0x80,       0x20,     0x7f,     0x33, 0x00,   0x00,    0xfa,     0xf5,     0x8d,        0xDC,     0x57,   0x90,     0xcb,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };

#else
/* CHD 25P - no update yet . */
static const int s_eq_chd_720p25[7][8] =
 {
	/*AEQ, DEQ1, DEQ2,  G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa2, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0x03, 0xa2, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xe3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xc3, 0xa1, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0x93, 0x00, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 //{0x93, 0xa7, 0xa7,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 7 : 550~650M */
 //{0x33, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 8 : 650M ~  */
 };

static const int s_eq_chd_A_coeff_720p25[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */
 };

static const int s_eq_chd_B_coeff_720p25[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */
 };

static const int s_eq_chd_color_720p25[7][18] =
 {
	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x08,         0x8a,       0x20,     0x68,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,     0xd0,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x08,         0x8a,       0x20,     0x68,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,     0xd0,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x08,         0x85,       0x20,     0x68,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x8c,        0xDC,     0x57,   0xa8,     0xcd,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x08,         0x85,       0x40,     0x68,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa8,     0xca,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x08,         0x87,       0x40,     0x68,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x85,        0xDC,     0x57,   0xa8,     0xc3,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x08,         0x7f,       0x20,     0x68,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa0,     0xd2,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x08,         0x80,       0x20,     0x58,     0x1d, 0x00,   0x00,    0xfa,     0xf5,     0x8d,        0xDC,     0x57,   0x90,     0xcb,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
  };
#endif


#ifdef SUPPORT_HI3520D 
/* CHD 60P - no update yet . now, HI3531A CVI 720@60p value */
static const int s_eq_chd_720p60[7][8] =
{
	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
	{0x03, 0xa7, 0xa3,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
	{0xc3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
	{0x93, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
};

static const int s_eq_chd_A_coeff_720p60[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_chd_B_coeff_720p60[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_chd_color_720p60[7][18] =
 {
  	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x08,         0x8c,       0x20,     0x84,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x08,         0x8c,       0x20,     0x84,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x08,         0x8c,       0x20,     0x84,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x08,         0x88,       0x00,     0x83,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x94,        0xDC,     0x57,   0xa8,    0x82,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x08,         0x85,       0x00,     0x74,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x8f,        0xDC,     0x57,   0xa8,    0x82,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x08,         0x8d,       0x00,     0x77,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x86,        0xDC,     0x57,   0x50,    0x80,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x08,         0x8b,       0x20,     0x70,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x8a,        0xDC,     0x57,   0x30,    0x80,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };



#else
/* CHD 60P - no update yet . */
static const int s_eq_chd_720p60[7][8] =
{
	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
	{0x03, 0xa2, 0xa2,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
	{0x03, 0xa2, 0xa3,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
	{0xe3, 0xa1, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
	{0xf3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
	{0x93, 0xa1, 0xa3,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
};


static const int s_eq_chd_A_coeff_720p60[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_chd_B_coeff_720p60[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_chd_color_720p60[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x08,         0x8c,       0x20,     0x70,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x08,         0x8c,       0x20,     0x70,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x08,         0x8c,       0x20,     0x68,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x08,         0x88,       0x00,     0x58,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x94,        0xDC,     0x57,   0xa8,    0x82,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x08,         0x85,       0x00,     0x58,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x8f,        0xDC,     0x57,   0xa8,    0x82,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x08,         0x8d,       0x00,     0x58,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x90,        0xDC,     0x57,   0x50,    0x80,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x08,         0x8b,       0x20,     0x58,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x8a,        0xDC,     0x57,   0x30,    0x80,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };
#endif



#ifdef SUPPORT_HI3520D 
/* CHD 50P - no update yet . now, HI3531A CVI 720@50p value */
static const int s_eq_chd_720p50[7][8] =
{
	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
	{0x03, 0xa7, 0xa3,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
	{0xc3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
	{0x93, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
};

static const int s_eq_chd_A_coeff_720p50[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_chd_B_coeff_720p50[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_chd_color_720p50[7][18] =
 {
	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x08,         0x8c,       0x20,     0x84,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x08,         0x8c,       0x20,     0x84,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x08,         0x8c,       0x20,     0x84,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x08,         0x88,       0x00,     0x83,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x94,        0xDC,     0x57,   0xa8,    0x82,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x08,         0x85,       0x00,     0x74,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x8f,        0xDC,     0x57,   0xa8,    0x82,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x08,         0x8d,       0x00,     0x77,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x86,        0xDC,     0x57,   0x50,    0x80,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x08,         0x8b,       0x20,     0x70,     0x2c, 0x00,   0x00,    0xfa,     0xf5,     0x8a,        0xDC,     0x57,   0x30,    0x80,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };


#else
/* CHD 50P - no update yet . */
static const int s_eq_chd_720p50[7][8] =
{
	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
	{0x03, 0xa2, 0xa2,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
	{0xe3, 0xa1, 0xa3,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
	{0xf3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
	{0x93, 0xa1, 0xa3,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
};

static const int s_eq_chd_A_coeff_720p50[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_chd_B_coeff_720p50[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_chd_color_720p50[7][18] =
 {
 /*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x08,         0x70,       0x20,     0x70,     0x20, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x08,         0x70,       0x20,     0x70,     0x20, 0x00,   0x00,    0xfa,     0xf5,     0x87,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x08,         0x70,       0x20,     0x68,     0x20, 0x00,   0x00,    0xfa,     0xf5,     0x8b,        0xDC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x08,         0x70,       0x00,     0x68,     0x20, 0x00,   0x00,    0xfa,     0xf5,     0x94,        0xDC,     0x57,   0xa8,    0x82,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x08,         0x70,       0x00,     0x68,     0x20, 0x00,   0x00,    0xfa,     0xf5,     0x8f,        0xDC,     0x57,   0xa8,    0x82,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x08,         0x70,       0x00,     0x68,     0x20, 0x00,   0x00,    0xfa,     0xf5,     0x86,        0xDC,     0x57,   0x50,    0x80,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x08,         0x70,       0x20,     0x68,     0x20, 0x00,   0x00,    0xfa,     0xf5,     0x8a,        0xDC,     0x57,   0x30,    0x80,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };
#endif

/* TFHD 25 - no update yet. TFHD 30p value*/
 static const int s_eq_thd_1080p25[7][8] =
 {
 	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa7, 0xa3,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xc3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
	{0xd3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_thd_A_coeff_1080p25[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_thd_B_coeff_1080p25[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_thd_color_1080p25[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8e,       0x30,     0x8a,     0xff, 0x04,   0x08,    0xfb,     0xf8,     0x80,        0xCC,     0x57,   0xa8,     0x60,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x91,       0x30,     0x8a,     0xff, 0x08,   0x08,    0xfb,     0xf8,     0x80,        0xCC,     0x57,   0xa8,     0x60,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x8f,       0x30,     0x93,     0xff, 0x08,   0x08,    0xfb,     0xf8,     0x84,        0xCC,     0x57,   0xa8,     0x5e,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x8e,       0x30,     0x89,     0xff, 0x08,   0x08,    0xfb,     0xf8,     0x82,        0xCC,     0x57,   0xa8,     0x66,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x87,       0x00,     0x73,     0xff, 0x08,   0x08,    0xfb,     0xf8,     0x82,        0xCC,     0x57,   0xa8,     0x64,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x89,       0x00,     0x73,     0xff, 0x08,   0x08,    0xfb,     0xf8,     0x7f,        0xCC,     0x57,   0x50,     0x62,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x87,       0x00,     0x73,     0xff, 0x08,   0x08,    0xfb,     0xf8,     0x83,        0xCC,     0x57,   0x30,     0x5f,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };


/* TFHD 30 */
 static const int s_eq_thd_1080p30[7][8] =
 {
 	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa7, 0xa3,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xc3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
	{0xD3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_thd_A_coeff_1080p30[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_thd_B_coeff_1080p30[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_thd_color_1080p30[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8e,       0x70,     0x87,     0xff, 0x04,   0x08,    0xfb,     0xf8,     0x80,        0xCC,     0x57,   0xa8,     0x60,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8e,       0x70,     0x87,     0xff, 0x04,   0x08,    0xfb,     0xf8,     0x80,        0xCC,     0x57,   0xa8,     0x60,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x8c,       0x30,     0x90,     0xff, 0x04,   0x08,    0xfb,     0xf8,     0x84,        0xCC,     0x57,   0xa8,     0x5e,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x8b,       0x30,     0x86,     0xff, 0x04,   0x08,    0xfb,     0xf8,     0x82,        0xCC,     0x57,   0xa8,     0x66,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x84,       0x00,     0x70,     0xff, 0x04,   0x08,    0xfb,     0xf8,     0x82,        0xCC,     0x57,   0xa8,     0x64,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x86,       0x00,     0x70,     0xff, 0x04,   0x08,    0xfb,     0xf8,     0x7f,        0xCC,     0x57,   0x50,     0x62,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x84,       0x00,     0x70,     0xff, 0x04,   0x08,    0xfb,     0xf8,     0x83,        0xCC,     0x57,   0x30,     0x5f,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };

/* THD A(25) - no update yet.*/
static const int s_eq_thd_720p25A[7][8] =
 {
 	/*AEQ, DEQ1, DEQ2,  G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa7, 0xa3,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0xd3, 0xa1, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xc3, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xf3, 0xa7, 0xa7,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0xc3, 0xa7, 0xa7,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_thd_A_coeff_720p25A[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_thd_B_coeff_720p25A[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_thd_color_720p25A[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8b,       0x20,     0x83,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x80,        0xCC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8b,       0x20,     0x83,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x80,        0xCC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x8b,       0x00,     0x83,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x84,        0xCC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x87,       0x00,     0x82,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x8d,        0xCC,     0x57,   0xa8,    0x82,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x88,       0x00,     0x76,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x84,        0xCC,     0x57,   0xa8,    0x82,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8c,       0x00,     0x76,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x7f,        0xCC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8a,       0x00,     0x70,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x83,        0xCC,     0x57,   0xa8,    0x80,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };

/* THD A(30) - no update yet.*/
static const int s_eq_thd_720p30A[7][8] =
 {
 	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa7, 0xa3,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0xd3, 0xa1, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xc3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0xc3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_thd_A_coeff_720p30A[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_thd_B_coeff_720p30A[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_thd_color_720p30A[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8b,       0x20,     0x83,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x80,        0xCC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8b,       0x20,     0x83,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x80,        0xCC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x8b,       0x00,     0x83,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x84,        0xCC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x87,       0x00,     0x82,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x8d,        0xCC,     0x57,   0xa8,     0x82,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x88,       0x00,     0x76,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x84,        0xCC,     0x57,   0xa8,     0x82,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8c,       0x00,     0x76,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x7f,        0xCC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8a,       0x00,     0x70,     0x02, 0x00,   0x08,    0xfa,     0xf5,     0x83,        0xCC,     0x57,   0xa8,     0x80,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };

/* THD B(25) - no update yet.*/
static const int s_eq_thd_720p25B[7][8] =
 {
	/*AEQ, DEQ1, DEQ2,  G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa1, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0x03, 0xa2, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0x03, 0xa3, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xd3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0xc3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
  //{0x93, 0xa7, 0xa7,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 7 : 550~650M */
  //{0x33, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 8 : 650M ~  */ 
 };

static const int s_eq_thd_A_coeff_720p25B[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */	 
 };

static const int s_eq_thd_B_coeff_720p25B[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */	 
 };


static const int s_eq_thd_color_720p25B[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x84,       0x20,     0x94,     0x02, 0x20,   0x07,    0xf3,     0xfa,     0x87,        0xCC,     0x57,   0xa8,     0x84,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x84,       0x20,     0x94,     0x02, 0x20,   0x07,    0xf3,     0xfa,     0x87,        0xCC,     0x57,   0xa8,     0x84,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x7f,       0x30,     0x98,     0x02, 0x20,   0x07,    0xf3,     0xfa,     0x8c,        0xCC,     0x57,   0xa8,     0x81,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x7f,       0x40,     0x97,     0x02, 0x20,   0x07,    0xf3,     0xfa,     0x8b,        0xCC,     0x57,   0xa8,     0x7e,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x81,       0x40,     0x97,     0x02, 0x20,   0x07,    0xf3,     0xfa,     0x86,        0xCC,     0x57,   0xa8,     0x75,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x88,       0x30,     0x8f,     0x02, 0x20,   0x07,    0xf3,     0xfa,     0x8b,        0xCC,     0x57,   0xa0,     0x86,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x89,       0x20,     0x8f,     0x02, 0x20,   0x07,    0xf3,     0xfa,     0x8d,        0xCC,     0x57,   0x90,     0x7f,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };

/* THD B(30) - no update yet.*/
static const int s_eq_thd_720p30B[7][8] =
 {
	/*AEQ, DEQ1, DEQ2,  G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa1, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0x03, 0xa2, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0x03, 0xa3, 0xa1,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xd3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0xc3, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 //{0x93, 0xa7, 0xa7,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 7 : 550~650M */
 //{0x33, 0xa1, 0xa2,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 8 : 650M ~  */ 
 };

static const int s_eq_thd_A_coeff_720p30B[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */	 
 };

static const int s_eq_thd_B_coeff_720p30B[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 5 : 350~450M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 6 : 450~550M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 7 : 550~650M */
// 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 8 : 650M~ */	 
 };


static const int s_eq_thd_color_720p30B[7][18] =
 {
	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x84,       0x20,     0x94,     0x02, 0x20,   0xfe,    0xf3,     0xf6,     0x80,        0xCC,     0x57,   0xa8,     0x78,    0x08,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8a,       0x20,     0x94,     0x02, 0x20,   0xfe,    0xf3,     0xf6,     0x80,        0xCC,     0x57,   0xa8,     0x78,    0x08,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x7f,       0x30,     0x98,     0x02, 0x20,   0xfe,    0xf3,     0xf6,     0x86,        0xCC,     0x57,   0xa8,     0x78,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x7f,       0x40,     0x97,     0x02, 0x20,   0xfe,    0xf3,     0xf6,     0x85,        0xCC,     0x57,   0xa8,     0x76,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x81,       0x40,     0x97,     0x02, 0x20,   0xfe,    0xf3,     0xf6,     0x7f,        0xCC,     0x57,   0xa8,     0x76,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x88,       0x30,     0x8f,     0x02, 0x20,   0xfe,    0xf3,     0xf6,     0x84,        0xCC,     0x57,   0xa0,     0x76,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x89,       0x20,     0x8f,     0x02, 0x20,   0xfe,    0xf3,     0xf6,     0x86,        0xCC,     0x57,   0x90,     0x76,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };

/* AHD 3M 18P - no update yet*/
static const int s_eq_3m_18p[7][8] =
 {
 	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa1, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xd3, 0x00, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0xd3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_A_coeff_3m_18p[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_B_coeff_3m_18p[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x10, 0x64, 0x7b, 0xff, 0x00, 0x10},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_color_3m_18p[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8c,       0x30,     0x83,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x80,        0xDC,     0x57,   0xa8,     0x90,    0x09,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8b,       0x30,     0x83,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0xa8,     0x90,    0x09,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x88,       0x30,     0xbe,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x8c,        0xDC,     0x57,   0xa8,     0x8e,    0x08,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x88,       0x30,     0xbe,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x8c,        0xDC,     0x57,   0xa8,     0x88,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x88,       0x30,     0xbe,     0x01, 0x00,   0x05,    0xfa,     0xf9,     0x84,        0xDC,     0x57,   0xa8,     0x8b,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8b,       0x20,     0x70,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0x70,     0x87,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8a,       0x70,     0x70,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x7d,        0xDC,     0x57,   0x50,     0x86,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
};



static const int s_eq_3m_25p[7][8] =
 {
 	/*AEQ, DEQ1, DEQ2,, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
    {0x03, 0x00, 0x00,   0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,   0x10,  0x07,     0x11,      0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0xd3, 0x00, 0x00,   0x10,  0x07,     0x11,      0x03,      0x00},	/* stage 2 : 50~150M */
 	{0xd3, 0x00, 0x00,   0x10,  0x07,     0x11,      0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xf3, 0xa1, 0x00,   0x10,  0x07,     0x11,      0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xc3, 0xa7, 0xa7,   0x10,  0x07,     0x11,      0x03,      0x00},	/* stage 5 : 350~450M */
 	{0x93, 0xa7, 0xa7,   0x10,  0x07,     0x11,      0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_A_coeff_3m_25p[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x10, 0x64, 0x7b, 0xff, 0x00, 0x10},	/* stage 2 : 50~150M */
 	{0x01,  0xc0, 0x7d, 0x50, 0xe1, 0x00, 0x30},	/* stage 3 : 150~250M */
 	{0x01,  0x00, 0x08, 0xe3, 0xfc, 0x00, 0xd6},	/* stage 4 : 250~350M */
 	{0x01,  0x00, 0x08, 0xe3, 0xfc, 0x00, 0xd6},	/* stage 5 : 350~450M */
 	{0x01,  0x10, 0x29, 0x7b, 0xf0, 0x00, 0x49},	/* stage 6 : 450~550M */
 };

static const int s_eq_B_coeff_3m_25p[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x10, 0x64, 0x7b, 0xff, 0x00, 0x10},	/* stage 2 : 50~150M */
 	{0x01,  0xc0, 0x7d, 0x50, 0xe1, 0x00, 0x30},	/* stage 3 : 150~250M */
 	{0x01,  0x00, 0x0c, 0xe6, 0xfb, 0x00, 0xd4},	/* stage 4 : 250~350M */
 	{0x01,  0x00, 0x0c, 0xe6, 0xfb, 0x00, 0xd4},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x0c, 0xe6, 0xfb, 0x00, 0xd8},	/* stage 6 : 450~550M */
 };


static const int s_eq_color_3m_25p[7][18] =
 {
	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay,  yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8b,       0x20,     0x80,     0xfc, 0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0xad,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8b,       0x20,     0x80,     0xfc, 0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0xad,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x88,       0x00,     0x80,     0xfc, 0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0x80,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x88,       0x00,     0x80,     0xfc, 0x00,   0x07,    0xfb,     0xfb,     0x88,        0xDC,     0x57,   0x80,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x88,       0x20,     0x80,     0xfc, 0x00,   0x07,    0xfb,     0xfb,     0x84,        0xDC,     0x57,   0x80,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8b,       0x00,     0x80,     0xfc, 0x00,   0x07,    0xfb,     0xfb,     0x84,        0xDC,     0x57,   0x80,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8a,       0x00,     0x80,     0xfc, 0x00,   0x07,    0xfb,     0xfb,     0x84,        0xDC,     0x57,   0x80,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };



static const int s_eq_3m_30p[7][8] =
 {
 	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,  0x10,  0x07,     0x11,      0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0xd3, 0x00, 0x00,  0x10,  0x07,     0x11,      0x03,      0x00},	/* stage 2 : 50~150M */
 	{0xd3, 0x00, 0x00,  0x10,  0x07,     0x11,      0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xf3, 0xa1, 0x00,  0x10,  0x07,     0x11,      0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xc3, 0xa7, 0xa7,  0x10,  0x07,     0x11,      0x03,      0x00},	/* stage 5 : 350~450M */
 	{0x93, 0xa7, 0xa7,  0x10,  0x07,     0x11,      0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_A_coeff_3m_30p[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x10, 0x64, 0x7b, 0xff, 0x00, 0x10},	/* stage 2 : 50~150M */
 	{0x01,  0xc0, 0x7d, 0x50, 0xe1, 0x00, 0x30},	/* stage 3 : 150~250M */
 	{0x01,  0x00, 0x08, 0xe3, 0xfc, 0x00, 0xd6},	/* stage 4 : 250~350M */
 	{0x01,  0x00, 0x08, 0xe3, 0xfc, 0x00, 0xd6},	/* stage 5 : 350~450M */
 	{0x01,  0x10, 0x29, 0x7b, 0xf0, 0x00, 0x49},	/* stage 6 : 450~550M */
 };

static const int s_eq_B_coeff_3m_30p[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x10, 0x64, 0x7b, 0xff, 0x00, 0x10},	/* stage 2 : 50~150M */
 	{0x01,  0xc0, 0x7d, 0x50, 0xe1, 0x00, 0x30},	/* stage 3 : 150~250M */
 	{0x01,  0x00, 0x0c, 0xe6, 0xfb, 0x00, 0xd4},	/* stage 4 : 250~350M */
 	{0x01,  0x00, 0x0c, 0xe6, 0xfb, 0x00, 0xd4},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x0c, 0xe6, 0xfb, 0x00, 0xd8},	/* stage 6 : 450~550M */
 };


static const int s_eq_color_3m_30p[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay,  yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8b,       0x20,     0x80,     0xfc,  0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0xad,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8b,       0x20,     0x80,     0xfc,  0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0xad,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x88,       0x00,     0x80,     0xfc,  0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0x80,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x88,       0x00,     0x80,     0xfc,  0x00,   0x07,    0xfb,     0xfb,     0x88,        0xDC,     0x57,   0x80,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x88,       0x20,     0x80,     0xfc,  0x00,   0x07,    0xfb,     0xfb,     0x84,        0xDC,     0x57,   0x80,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8b,       0x00,     0x80,     0xfc,  0x00,   0x07,    0xfb,     0xfb,     0x84,        0xDC,     0x57,   0x80,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8a,       0x00,     0x80,     0xfc,  0x00,   0x07,    0xfb,     0xfb,     0x84,        0xDC,     0x57,   0x80,    0x5e,     0x07,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };


/* THD 3M 18P - no update yet*/
static const int s_eq_thd_3m_18p[7][8] =
 {
 	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa7, 0xa3,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xc3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0x93, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_thd_A_coeff_3m_18p[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_thd_B_coeff_3m_18p[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_thd_color_3m_18p[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x86,       0x40,     0x9F,     0x08, 0xF3,   0x00,    0xFD,     0xF8,     0x80,        0xDC,     0x57,   0xa8,     0x80,    0x00,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x86,       0x40,     0x9F,     0x08, 0xF3,   0x00,    0xFD,     0xF8,     0x80,        0xDC,     0x57,   0xa8,     0x7a,    0x00,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x84,       0x20,     0xa8,     0x08, 0xF3,   0x00,    0xFD,     0xF8,     0x84,        0xDC,     0x57,   0xa8,     0x7a,    0x00,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x83,       0x20,     0x9e,     0x08, 0xF3,   0x00,    0xFD,     0xF8,     0x82,        0xDC,     0x57,   0xa8,     0x7c,    0x00,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x7c,       0x00,     0x88,     0x08, 0xF3,   0x00,    0xFD,     0xF8,     0x82,        0xDC,     0x57,   0xa8,     0x7c,    0x00,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x7e,       0x00,     0x88,     0x08, 0xF3,   0x00,    0xFD,     0xF8,     0x7f,        0xDC,     0x57,   0x80,     0x7b,    0x00,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x80,       0x00,     0x88,     0x08, 0xF3,   0x00,    0xFD,     0xF8,     0x83,        0xDC,     0x57,   0x30,     0x7c,    0x00,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };


/* THD 5M 18P - no update yet*/
static const int s_eq_thd_5m_12p[7][8] =
 {
 	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa7, 0xa3,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xc3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0x93, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_thd_A_coeff_5m_12p[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_thd_B_coeff_5m_12p[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_thd_color_5m_12p[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8b,       0x20,     0x83,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0xa8,     0x60,    0x09,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8b,       0x20,     0x83,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0xa8,     0x60,    0x09,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x8b,       0x20,     0x83,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0xa8,     0x60,    0x09,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x8b,       0x20,     0x83,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0xa8,     0x60,    0x09,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x8b,       0x20,     0x83,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0xa8,     0x60,    0x09,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8b,       0x20,     0x83,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0x80,     0x60,    0x09,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8b,       0x20,     0x83,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0x30,     0x60,    0x09,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_4m_25p[7][8] =
{
 	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa3, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xf3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xc3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0x93, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_A_coeff_4m_25p[7][7] =
{
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x10, 0x64, 0x7b, 0xff, 0x00, 0x10},	/* stage 2 : 50~150M */
 	{0x01,  0xc0, 0x7d, 0x50, 0xe1, 0x00, 0x30},	/* stage 3 : 150~250M */
 	{0x01,  0x00, 0x08, 0xe3, 0xfc, 0x00, 0xd6},	/* stage 4 : 250~350M */
 	{0x01,  0x00, 0x08, 0xe3, 0xfc, 0x00, 0xd6},	/* stage 5 : 350~450M */
 	{0x01,  0x10, 0x29, 0x7b, 0xf0, 0x00, 0x49},	/* stage 6 : 450~550M */
 };

static const int s_eq_B_coeff_4m_25p[7][7] =
{
	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
	{0x01,  0x10, 0x64, 0x7b, 0xff, 0x00, 0x10},	/* stage 2 : 50~150M */
	{0x01,  0xc0, 0x7d, 0x50, 0xe1, 0x00, 0x30},	/* stage 3 : 150~250M */
	{0x01,  0x00, 0x0c, 0xe6, 0xfb, 0x00, 0xd4},	/* stage 4 : 250~350M */
	{0x01,  0x00, 0x0c, 0xe6, 0xfb, 0x00, 0xd4},	/* stage 5 : 350~450M */
	{0x01,  0x20, 0x0c, 0xe6, 0xfb, 0x00, 0xd8},	/* stage 6 : 450~550M */
};


static const int s_eq_color_4m_25p[7][18] =
{
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay,  yc_delay, acc_min, acc_max, reserved*/
	{0x00,         0x8b,       0x20,     0x80,     0xfa, 0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0xad,    0x2E,     0x09,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x00,         0x8b,       0x20,     0x80,     0xfa, 0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0xad,    0x2E,     0x09,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
	{0x00,         0x88,       0x20,     0x80,     0xfd, 0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0xa0,    0x28,     0x09,      0,      2047,     0x00},	/* stage 2 : 50~150M */
	{0x00,         0x80,       0x00,     0x80,     0x00, 0x10,   0x00,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0x80,    0x30,     0x09,      0,      2047,     0x00},	/* stage 3 : 150~250M */
	{0x00,         0x74,       0x00,     0x80,     0xf4, 0x00,   0x07,    0xfb,     0xfb,     0x8e,        0xDC,     0x57,   0x60,    0x2E,     0x09,      0,      2047,     0x00},	/* stage 4 : 250~350M */
	{0x00,         0x7e,       0x00,     0x80,     0xf3, 0x00,   0x07,    0xfb,     0xfb,     0x90,        0xDC,     0x57,   0x60,    0x2A,     0x09,      0,      2047,     0x00},	/* stage 5 : 350~450M */
	{0x00,         0x8a,       0x00,     0x80,     0xf2, 0x00,   0x07,    0xfb,     0xfb,     0x8c,        0xDC,     0x57,   0x60,    0x2A,     0x09,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };


static const int s_eq_4m_30p[7][8] =
{
 	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
	{0x03, 0xa3, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
	{0xf3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
	{0xc3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0x93, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_A_coeff_4m_30p[7][7] =
{
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x10, 0x64, 0x7b, 0xff, 0x00, 0x10},	/* stage 2 : 50~150M */
 	{0x01,  0xc0, 0x7d, 0x50, 0xe1, 0x00, 0x30},	/* stage 3 : 150~250M */
 	{0x01,  0x00, 0x08, 0xe3, 0xfc, 0x00, 0xd6},	/* stage 4 : 250~350M */
 	{0x01,  0x00, 0x08, 0xe3, 0xfc, 0x00, 0xd6},	/* stage 5 : 350~450M */
 	{0x01,  0x10, 0x29, 0x7b, 0xf0, 0x00, 0x49},	/* stage 6 : 450~550M */
 };

static const int s_eq_B_coeff_4m_30p[7][7] =
{
	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
	{0x01,  0x10, 0x64, 0x7b, 0xff, 0x00, 0x10},	/* stage 2 : 50~150M */
	{0x01,  0xc0, 0x7d, 0x50, 0xe1, 0x00, 0x30},	/* stage 3 : 150~250M */
	{0x01,  0x00, 0x0c, 0xe6, 0xfb, 0x00, 0xd4},	/* stage 4 : 250~350M */
	{0x01,  0x00, 0x0c, 0xe6, 0xfb, 0x00, 0xd4},	/* stage 5 : 350~450M */
	{0x01,  0x20, 0x0c, 0xe6, 0xfb, 0x00, 0xd8},	/* stage 6 : 450~550M */
};


static const int s_eq_color_4m_30p[7][18] =
{
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay,  yc_delay, acc_min, acc_max, reserved*/
	{0x00,         0x8b,       0x20,     0x80,     0xfa, 0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0xad,    0x2E,     0x09,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x00,         0x8b,       0x20,     0x80,     0xfa, 0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0xad,    0x2E,     0x09,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
	{0x00,         0x88,       0x20,     0x80,     0xfd, 0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0xa0,    0x28,     0x09,      0,      2047,     0x00},	/* stage 2 : 50~150M */
	{0x00,         0x80,       0x00,     0x80,     0x00, 0x10,   0x00,    0xfb,     0xfb,     0x82,        0xDC,     0x57,   0x80,    0x30,     0x09,      0,      2047,     0x00},	/* stage 3 : 150~250M */
	{0x00,         0x74,       0x00,     0x80,     0xf4, 0x00,   0x07,    0xfb,     0xfb,     0x8e,        0xDC,     0x57,   0x60,    0x2E,     0x09,      0,      2047,     0x00},	/* stage 4 : 250~350M */
	{0x00,         0x7e,       0x00,     0x80,     0xf3, 0x00,   0x07,    0xfb,     0xfb,     0x90,        0xDC,     0x57,   0x60,    0x2A,     0x09,      0,      2047,     0x00},	/* stage 5 : 350~450M */
	{0x00,         0x8a,       0x00,     0x80,     0xf2, 0x00,   0x07,    0xfb,     0xfb,     0x8c,        0xDC,     0x57,   0x60,    0x2A,     0x09,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };


static const int s_eq_4m_nrt[7][8] =
 {
 	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa1, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xd3, 0x00, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0xd3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_A_coeff_4m_nrt[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_B_coeff_4m_nrt[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x10, 0x64, 0x7b, 0xff, 0x00, 0x10},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };

static const int s_eq_color_4m_nrt[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8c,       0x30,     0x80,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x80,        0xDC,     0x57,   0xab,     0x60,    0x09,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8b,       0x30,     0x80,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0xab,     0x60,    0x09,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x88,       0x30,     0x80,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x8c,        0xDC,     0x57,   0xe6,     0x60,    0x09,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x88,       0x30,     0x80,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x8c,        0xDC,     0x57,   0xe6,     0x60,    0x08,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x88,       0x30,     0x80,     0x01, 0x00,   0x05,    0xfa,     0xf9,     0x84,        0xDC,     0x57,   0xe6,     0x60,    0x08,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8b,       0x20,     0x80,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0x60,     0x60,    0x08,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8a,       0x70,     0x80,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x7d,        0xDC,     0x57,   0x40,     0x60,    0x08,      0,      2047,     0x00},	/* stage 6 : 450~550M */
};


static const int s_eq_5m_12_5p[7][8] =
 {
 	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
 	{0x03, 0xa1, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
 	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
 	{0xd3, 0x00, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
 	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0xd3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_A_coeff_5m_12_5p[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x40, 0x00, 0xe2, 0xf9, 0x00, 0xe0},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 5 : 350~450M */
 	{0x01,  0x20, 0x04, 0xf4, 0xfd, 0x02, 0xf3},	/* stage 6 : 450~550M */
 };

static const int s_eq_B_coeff_5m_12_5p[7][7] =
 {
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x10, 0x64, 0x7b, 0xff, 0x00, 0x10},	/* stage 2 : 50~150M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 3 : 150~250M */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 4 : 250~350M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 5 : 350~450M */
 	{0x01,  0x90, 0x7e, 0x7b, 0xe3, 0x47, 0x70},	/* stage 6 : 450~550M */
 };


static const int s_eq_color_5m_12_5p[7][18] =
 {
 	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub, H_delay, yc_delay, acc_min, acc_max, reserved*/
 	{0x00,         0x8b,       0x30,     0x80,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0xab,     0x80,    0x04,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,         0x8b,       0x30,     0x80,     0x00, 0x00,   0x05,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0xab,     0x80,    0x04,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
 	{0x00,         0x88,       0x30,     0x80,     0xfa, 0x00,   0x05,    0xfa,     0xf9,     0x8c,        0xDC,     0x57,   0xe9,     0x70,    0x04,      0,      2047,     0x00},	/* stage 2 : 50~150M */
 	{0x00,         0x88,       0x30,     0x80,     0x00, 0x00,   0x00,    0xfa,     0xf9,     0x8c,        0xDC,     0x57,   0xe9,     0x80,    0x04,      0,      2047,     0x00},	/* stage 3 : 150~250M */
 	{0x00,         0x88,       0x30,     0x80,     0xfb, 0x00,   0x00,    0xfa,     0xf9,     0x84,        0xDC,     0x57,   0xe9,     0x7c,    0x04,      0,      2047,     0x00},	/* stage 4 : 250~350M */
 	{0x00,         0x8b,       0x20,     0x80,     0xfb, 0x10,   0x10,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0x70,     0x76,    0x04,      0,      2047,     0x00},	/* stage 5 : 350~450M */
 	{0x00,         0x8a,       0x20,     0x80,     0xfb, 0x10,   0x10,    0xfa,     0xf9,     0x82,        0xDC,     0x57,   0x40,     0x76,    0x04,      0,      2047,     0x00},	/* stage 6 : 450~550M */
};

static const int s_eq_5m_20p[7][8] =
{
 	/*AEQ, DEQ1, DEQ2, G_SEL, B_LPF, LPF_BYPASS, REF_VOLT, reserved*/
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x03, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 1 : 0~50M(short) */
	{0x03, 0xa3, 0xa1,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 2 : 50~150M */
	{0xd3, 0x00, 0x00,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 3 : 150~250M */
	{0xf3, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 4 : 250~350M */
	{0x93, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 5 : 350~450M */
 	{0x93, 0xa7, 0xa7,  0x00,  0x03,     0x00,     0x03,      0x00},	/* stage 6 : 450~550M */
 };

static const int s_eq_A_coeff_5m_20p[7][7] =
{
 	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
 	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
 	{0x01,  0x10, 0x64, 0x7b, 0xff, 0x00, 0x10},	/* stage 2 : 50~150M */
 	{0x01,  0xc0, 0x7d, 0x50, 0xe1, 0x00, 0x30},	/* stage 3 : 150~250M */
 	{0x01,  0x00, 0x08, 0xe3, 0xfc, 0x00, 0xd6},	/* stage 4 : 250~350M */
 	{0x01,  0x00, 0x08, 0xe3, 0xfc, 0x00, 0xd6},	/* stage 5 : 350~450M */
 	{0x01,  0x10, 0x29, 0x7b, 0xf0, 0x00, 0x49},	/* stage 6 : 450~550M */
 };

static const int s_eq_B_coeff_5m_20p[7][7] =
{
	/*A0[8], A0,   A1,   A2,   B0,   B1,   B2  */
	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* stage 1 : 0~50M(short) */
	{0x01,  0x10, 0x64, 0x7b, 0xff, 0x00, 0x10},	/* stage 2 : 50~150M */
	{0x01,  0xc0, 0x7d, 0x50, 0xe1, 0x00, 0x30},	/* stage 3 : 150~250M */
	{0x01,  0x00, 0x0c, 0xe6, 0xfb, 0x00, 0xd4},	/* stage 4 : 250~350M */
	{0x01,  0x00, 0x0c, 0xe6, 0xfb, 0x00, 0xd4},	/* stage 5 : 350~450M */
	{0x01,  0x20, 0x0c, 0xe6, 0xfb, 0x00, 0xd8},	/* stage 6 : 450~550M */
};


static const int s_eq_color_5m_20p[7][18] =
{
	/*brightness, contrast, h_peaking, saturation, hue, u_gain, v_gain, u_offset, v_offset, black_level, fsc_mode, acc_ref, sat_sub,H_delay,  yc_delay, acc_min, acc_max, reserved*/
	{0x00,         0x8b,       0x20,     0x80,     0xfa, 0x00,   0x07,    0xfc,     0xfb,     0x82,        0xDA,     0x57,   0xad,    0x80,     0x05,      0,      2047,     0x00},	/* stage 0 : No EQ(NO INPUT) */
	{0x00,         0x8b,       0x20,     0x80,     0xfa, 0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDA,     0x57,   0xad,    0x7b,     0x05,      0,      2047,     0x00},	/* stage 1 : 0~50M(short) */
	{0x00,         0x88,       0x20,     0x80,     0xf9, 0x00,   0x07,    0xfb,     0xfb,     0x82,        0xDA,     0x57,   0x78,    0x78,     0x05,      0,      2047,     0x00},	/* stage 2 : 50~150M */
	{0x00,         0x80,       0x00,     0x80,     0xf6, 0x10,   0x00,    0xfb,     0xfb,     0x82,        0xDA,     0x57,   0x70,    0x7b,     0x05,      0,      2047,     0x00},	/* stage 3 : 150~250M */
	{0x00,         0x74,       0x20,     0x80,     0xf4, 0x00,   0x07,    0xfb,     0xfb,     0x8e,        0xDA,     0x57,   0x70,    0x7c,     0x05,      0,      2047,     0x00},	/* stage 4 : 250~350M */
	{0x00,         0x7e,       0x40,     0x80,     0xf3, 0x00,   0x07,    0xfb,     0xfb,     0x90,        0xDA,     0x57,   0x40,    0x7C,     0x05,      0,      2047,     0x00},	/* stage 5 : 350~450M */
	{0x00,         0x8a,       0x00,     0x80,     0xf2, 0x00,   0x07,    0xfb,     0xfb,     0x8c,        0xDA,     0x57,   0x40,    0x7C,     0x05,      0,      2047,     0x00},	/* stage 6 : 450~550M */
 };


/*******************************************************************************
 *
 *
 *
 *  Functions(Common)
 *
 *
 *
 *******************************************************************************/
/******************************************************************************
*	Description		: Get EQ pattern color gain
*	Argurments		: ch : channel number
*	Return value	: EQ pattern color gain value
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned int GetAccGain(unsigned char ch)
{
	unsigned int acc_gain_status;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
	acc_gain_status = gpio_i2c_read(nvp6134_iic_addr[ch/4],0xE2)&0x07;
	acc_gain_status <<= 8;
	acc_gain_status |= gpio_i2c_read(nvp6134_iic_addr[ch/4],0xE3);
	return acc_gain_status;
}

/******************************************************************************
*	Description		: Get Y plus slope
*	Argurments		: ch : channel number
*	Return value	: Y plus slope value
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned int GetYPlusSlope(unsigned char ch)
{
	unsigned int y_plus_slp_status;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
	y_plus_slp_status = gpio_i2c_read(nvp6134_iic_addr[ch/4],0xE8)&0x07;
	y_plus_slp_status <<= 8;
	y_plus_slp_status |= gpio_i2c_read(nvp6134_iic_addr[ch/4],0xE9);

	return y_plus_slp_status;
}

/******************************************************************************
*	Description		: Get Y minus slope
*	Argurments		: ch : channel number
*	Return value	: Y minus slope value
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned int GetYMinusSlope(unsigned char ch)
{
	unsigned int y_minus_slp_status;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
	y_minus_slp_status = gpio_i2c_read(nvp6134_iic_addr[ch/4],0xEA)&0x07;
	y_minus_slp_status <<= 8;
	y_minus_slp_status |= gpio_i2c_read(nvp6134_iic_addr[ch/4],0xEB);

	return y_minus_slp_status;
}

/******************************************************************************
*	Description		: Get AC min value
*	Argurments		: ch : channel number
*	Return value	: AC min value
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char GetACMinValue(unsigned char ch)
{
	unsigned char fr_ac_min_value;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
	fr_ac_min_value = gpio_i2c_read(nvp6134_iic_addr[ch/4],0xFC);

	return fr_ac_min_value;
}

/******************************************************************************
*	Description		: Get AC max value
*	Argurments		: ch : channel number
*	Return value	: AC max value
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char GetACMaxValue(unsigned char ch)
{
	unsigned char fr_ac_max_value;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
	fr_ac_max_value = gpio_i2c_read(nvp6134_iic_addr[ch/4],0xFD);

	return fr_ac_max_value;
}

/******************************************************************************
*	Description		: Get DC max value
*	Argurments		: ch : channel number
*	Return value	: DC max value
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char GetDCValue(unsigned char ch)
{
	unsigned char fr_dc_value;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
	fr_dc_value = gpio_i2c_read(nvp6134_iic_addr[ch/4],0xFE);

	return fr_dc_value;
}

/******************************************************************************
*	Description		: Set FSC lock mode and speed for Color locking
*	Argurments		: ch : channel number, val(value)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void SetFSClockmodeForColorlocking( unsigned char ch )
{
	unsigned char temp = 0x00;
	unsigned char acc_gain = 0x00;
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);

	temp = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x25 );
	if( temp == 0xCC )
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25, 0xDC );
		msleep(33);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25, 0xCC );
	}
	else
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25, 0xCC );
		msleep(33);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25, 0xDC );
	}

	if(ch_mode_status[ch]<NVP6134_VI_720P_2530)		//cvbs uses org value
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25, temp );
	
	acc_gain = GetAccGain(ch);
	printk(">>>>> DRV[%s:%d] CH:%d FSC Lock Mode. acc_gain:0x%x(%d)\n", __func__, __LINE__, ch, acc_gain, acc_gain );
}

/******************************************************************************
*	Description		: Set FSC lock mode and speed for Color locking
*	Argurments		: ch : channel number, val(value)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void Set_ColorLockingFilter( unsigned char ch )
{
	unsigned char acc_gain = 0x00;
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A, 0xD4);
	msleep(100);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A, 0xD2);
	
	acc_gain = GetAccGain(ch);
	printk(">>>>> DRV[%s:%d] CH:%d FSC Lock Mode test(5x2A : 0xD4 -> 0xD2). acc_gain:0x%x(%d)\n", __func__, __LINE__, ch, acc_gain, acc_gain );
}

/*******************************************************************************
*	Description		:
*	Argurments		:
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
int get_resol_to_eqtable(unsigned char ch, int resol, int vfmt )
{
	int tbl_num = 0;

	switch( resol )
	{
		case NVP6134_VI_1080P_2530:
		case NVP6134_VI_1080P_NRT:
		case NVP6134_VI_720P_5060:
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_AFHD_2550 : EQ_VIDEO_MODE_AFHD_3060;
			printk(">>>>> DRV : %s\n", (vfmt == PAL)?"EQ_VIDEO_MODE_AFHD_2550":"EQ_VIDEO_MODE_AFHD_3060");
			break;
		case NVP6134_VI_720P_2530:
		case NVP6134_VI_HDEX:
            if( g_slp_ahd[ch] == 1 )
            {
                tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_AHD_GAIN_25 : EQ_VIDEO_MODE_AHD_GAIN_30;
                printk(">>>>> DRV : %s\n", (vfmt == PAL)?"EQ_VIDEO_MODE_AHD_GAIN_25":"EQ_VIDEO_MODE_AHD_GAIN_30");
            }
            else 
			{
				tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_AHD_25 : EQ_VIDEO_MODE_AHD_30;
				printk(">>>>> DRV : %s\n", (vfmt == PAL)?"EQ_VIDEO_MODE_AHD_25":"EQ_VIDEO_MODE_AHD_30");
            }
			break;
		case NVP6134_VI_EXC_1080P:
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_CFHD_25 : EQ_VIDEO_MODE_CFHD_30;
			printk(">>>>> DRV : %s\n", (vfmt == PAL)?"EQ_VIDEO_MODE_CFHD_25":"EQ_VIDEO_MODE_CFHD_30");
			break;
		case NVP6134_VI_EXC_720P:
		case NVP6134_VI_EXC_HDEX:
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_CHD_25 : EQ_VIDEO_MODE_CHD_30;
			printk(">>>>> DRV : %s\n", (vfmt == PAL)?"EQ_VIDEO_MODE_CHD_25":"EQ_VIDEO_MODE_CHD_30");
			break;
		case NVP6134_VI_EXC_720PRT:
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_CHD_50 : EQ_VIDEO_MODE_CHD_60;
			printk(">>>>> DRV : %s\n", (vfmt == PAL)?"EQ_VIDEO_MODE_CHD_50":"EQ_VIDEO_MODE_CHD_60");
            break;
		case NVP6134_VI_EXT_1080P:
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_TFHD_25 : EQ_VIDEO_MODE_TFHD_30;
			printk(">>>>> DRV : %s\n", (vfmt == PAL)?"EQ_VIDEO_MODE_TFHD_25":"EQ_VIDEO_MODE_TFHD_30");
			break;
		case NVP6134_VI_EXT_HDAEX:	
		case NVP6134_VI_EXT_720PA:
        case NVP6134_VI_EXT_720PRT:
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_THD_A25 : EQ_VIDEO_MODE_THD_A30;
			printk(">>>>> DRV : %s\n", (vfmt == PAL)?"EQ_VIDEO_MODE_THD_A25":"EQ_VIDEO_MODE_THD_A30");
			break;
		case NVP6134_VI_EXT_720PB:
        case NVP6134_VI_EXT_HDBEX:
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_THD_B25 : EQ_VIDEO_MODE_THD_B30;
			printk(">>>>> DRV : %s\n", (vfmt == PAL)?"EQ_VIDEO_MODE_THD_B25":"EQ_VIDEO_MODE_THD_B30");
			break;
		case NVP6134_VI_3M_NRT:	//separate 3M
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_AHD_3M_18 : EQ_VIDEO_MODE_AHD_3M_18;
			printk(">>>>> DRV : EQ_VIDEO_MODE_AHD_3M_18\n");
			break;
		case NVP6134_VI_3M:			
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_AHD_3M_25 : EQ_VIDEO_MODE_AHD_3M_30;
			printk(">>>>> DRV : %s\n", (vfmt == PAL)?"EQ_VIDEO_MODE_AHD_3M_25":"EQ_VIDEO_MODE_AHD_3M_30");
			break;
		case NVP6134_VI_EXT_3M_NRT:
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_THD_3M_18 : EQ_VIDEO_MODE_THD_3M_18;
			printk(">>>>> DRV : NVP6134_VI_EXT_3M_NRT\n");
			break;
		case NVP6134_VI_EXT_5M_NRT:
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_THD_5M_12_5P : EQ_VIDEO_MODE_THD_5M_12_5P;
			printk(">>>>> DRV : NVP6134_VI_EXT_5M_NRT\n");
			break;
		case NVP6134_VI_4M_NRT:
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_AHD_4M_NRT : EQ_VIDEO_MODE_AHD_4M_NRT;
			printk(">>>>> DRV : NVP6134_VI_4M_NRT\n");
			break;
		case NVP6134_VI_4M:
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_AHD_4M_25P : EQ_VIDEO_MODE_AHD_4M_30P;
			printk(">>>>> DRV : NVP6134_VI_4M\n");
			break;
		case NVP6134_VI_5M_NRT:  //12p
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_AHD_5M_12_5P : EQ_VIDEO_MODE_AHD_5M_12_5P;
			printk(">>>>> DRV : NVP6134_VI_5M_NRT\n");
			break;
		case NVP6134_VI_5M_20P:
			tbl_num = (vfmt == PAL) ? EQ_VIDEO_MODE_AHD_5M_20P : EQ_VIDEO_MODE_AHD_5M_20P;
			printk(">>>>> DRV : NVP6134_VI_5M_20P\n");
			break;
		default:
			printk(">>>>> DRV : default!\n");
			break;
	}

	return tbl_num;
}

/*******************************************************************************
*	Description		: set EQ base set
*	Argurments		: ch : channel, p_param(EQ base value)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void eq_set_value( unsigned char ch, void *p_param )
{
	equalizer_baseon *p_eqbase = (equalizer_baseon*)p_param;

	/* set Analog EQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
    gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, p_eqbase->eq_analog_eq );

	/* set Digital EQ1,2 */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x80+(0x20*(ch%4)), p_eqbase->eq_digital_eq1);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+(0x20*(ch%4)), p_eqbase->eq_digital_eq2);

	/* set lpf */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+(ch%4), p_eqbase->eq_g_sel);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x01, p_eqbase->eq_b_lpf);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x59, p_eqbase->eq_lpf_bypass);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5B, p_eqbase->eq_ref_volt);

	printk(">>>>> DRV[%s:%d] CH:%d [BASE SET] analog_eq:0x%x, digital_eq1:0x%x, digital_eq2:0x%x, g_sel:0x%x, b_lpf:0x%x, lpf_bypass:0x%x, ref_volt:0x%x\n", \
			__func__, __LINE__, ch, p_eqbase->eq_analog_eq, p_eqbase->eq_digital_eq1, p_eqbase->eq_digital_eq2, p_eqbase->eq_g_sel, p_eqbase->eq_b_lpf, \
			p_eqbase->eq_lpf_bypass, p_eqbase->eq_ref_volt );
}

/*******************************************************************************
*	Description		: set EQ Coeff A
*	Argurments		: ch : channel, p_param(EQ coeff A)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
//void eq_coeff_A_setting_val(int ch, unsigned char coeff_A_a0_8, unsigned char coeff_A_a0, unsigned char coeff_A_a1, unsigned char coeff_A_a2, unsigned char coeff_A_B0, unsigned char coeff_A_B1, unsigned char coeff_A_B2)
void eq_coeff_A_setting_val( unsigned char ch, void *p_param )
{
	equalizer_coeff_a *p_eqcoeffA = (equalizer_coeff_a*)p_param;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+(0x20*(ch%4)), p_eqcoeffA->coeff_A_a0_8);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+(0x20*(ch%4)), p_eqcoeffA->coeff_A_a0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86+(0x20*(ch%4)), p_eqcoeffA->coeff_A_a1);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x87+(0x20*(ch%4)), p_eqcoeffA->coeff_A_a2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x88+(0x20*(ch%4)), p_eqcoeffA->coeff_A_B0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+(0x20*(ch%4)), p_eqcoeffA->coeff_A_B1);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8A+(0x20*(ch%4)), p_eqcoeffA->coeff_A_B2);

	printk(">>>>> DRV[%s:%d] CH:%d [ COEFF_A] coeff_A_a0_8:0x%x, coeff_A_a0:0x%x, coeff_A_a1:0x%x, coeff_A_a2:0x%x, coeff_A_B0:0x%x, coeff_A_B1:0x%x, coeff_A_B2:0x%x\n", \
			__func__, __LINE__, ch, p_eqcoeffA->coeff_A_a0_8, p_eqcoeffA->coeff_A_a0, p_eqcoeffA->coeff_A_a1, p_eqcoeffA->coeff_A_a2, p_eqcoeffA->coeff_A_B0, \
			p_eqcoeffA->coeff_A_B1, p_eqcoeffA->coeff_A_B2 );
}

/*******************************************************************************
*	Description		: set EQ Coeff B
*	Argurments		: ch : channel, p_param(EQ coeff B)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
//void eq_coeff_B_setting_val(int ch, unsigned char coeff_B_a0_8, unsigned char coeff_B_a0, unsigned char coeff_B_a1, unsigned char coeff_B_a2, unsigned char coeff_B_B0, unsigned char coeff_B_B1, unsigned char coeff_B_B2)
void eq_coeff_B_setting_val( unsigned char ch, void *p_param )
{
	equalizer_coeff_b *p_eqcoeffB = (equalizer_coeff_b*)p_param;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8C+(0x20*(ch%4)),  p_eqcoeffB->coeff_B_a0_8);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8D+(0x20*(ch%4)),  p_eqcoeffB->coeff_B_a0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8E)+(0x20*(ch%4)),p_eqcoeffB->coeff_B_a1);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8F+(0x20*(ch%4)),  p_eqcoeffB->coeff_B_a2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90+(0x20*(ch%4)),  p_eqcoeffB->coeff_B_B0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x91+(0x20*(ch%4)),  p_eqcoeffB->coeff_B_B1);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x92+(0x20*(ch%4)),  p_eqcoeffB->coeff_B_B2);

	printk(">>>>> DRV[%s:%d] CH:%d [ COEFF_B] coeff_B_a0_8:0x%x, coeff_B_a0:0x%x, coeff_B_a1:0x%x, coeff_B_a2:0x%x, coeff_B_B0:0x%x, coeff_B_B1:0x%x, coeff_B_B2:0x%x\n", \
			__func__, __LINE__, ch, p_eqcoeffB->coeff_B_a0_8, p_eqcoeffB->coeff_B_a0, p_eqcoeffB->coeff_B_a1, p_eqcoeffB->coeff_B_a2, p_eqcoeffB->coeff_B_B0, \
			p_eqcoeffB->coeff_B_B1, p_eqcoeffB->coeff_B_B2 );
}

/*******************************************************************************
*	Description		: set color
*	Argurments		: ch : channel, p_param(EQ coeff B)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void eq_color_setting_val( unsigned char ch, void *p_param )
{
	equalizer_color *p_eqcolor = (equalizer_color*)p_param;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0C+(ch%4), p_eqcolor->color_brightness);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+(ch%4), p_eqcolor->color_contrast);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x18+(ch%4), p_eqcolor->color_h_peaking);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x3C+(ch%4), p_eqcolor->color_saturation);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+(ch%4), p_eqcolor->color_hue);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44+(ch%4), p_eqcolor->color_u_gain);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x48+(ch%4), p_eqcolor->color_v_gain);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x4C+(ch%4), p_eqcolor->color_u_offset);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4), p_eqcolor->color_v_offset);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20, p_eqcolor->color_black_level);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25, p_eqcolor->color_fsc_mode);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x27, p_eqcolor->color_acc_ref);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2B, p_eqcolor->color_sat_sub);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+(ch%4), p_eqcolor->eq_h_dly_offset);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+(ch%4), p_eqcolor->color_yc_delay);

	printk(">>>>> DRV[%s:%d] CH:%d [ COLOR ] brightness:0x%x, contrast:0x%x, h_peaking:0x%x, saturation:0x%x, hue:0x%x, u_gain:0x%x, v_gain:0x%x, u_offset:0x%x\n", \
			__func__, __LINE__, ch, p_eqcolor->color_brightness, p_eqcolor->color_contrast, p_eqcolor->color_h_peaking, p_eqcolor->color_saturation, \
			p_eqcolor->color_hue, p_eqcolor->color_u_gain, p_eqcolor->color_v_gain, p_eqcolor->color_u_offset);

	printk(">>>>> DRV[%s:%d] CH:%d [ COLOR ] v_offset:0x%x, black_level:0x%x, fsc_mode:0x%x, acc_ref:0x%x, sat_sub:0x%x, h_dly_offset:0x%x, yc_delay:0x%x\n", \
			__func__, __LINE__, ch, p_eqcolor->color_v_offset, p_eqcolor->color_black_level, p_eqcolor->color_fsc_mode, \
			p_eqcolor->color_acc_ref, p_eqcolor->color_sat_sub, p_eqcolor->eq_h_dly_offset, p_eqcolor->color_yc_delay );
}

/*******************************************************************************
*	Description		: loop color locking
*	Argurments		: ch(channel information)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
#define DURATION_SATURATION_TIME  10  //500ms(loop cnt) * DURATION_SATURATION_TIME

static unsigned char s_ckill[16] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 };
static int 			 s_agcgain[16] = {-1, };
static int 			 s_againSaturationCnt[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int eq_check_concurrent_saturation( unsigned char ch, unsigned char resol, int vfmt, int stage )
{
	//int			  timeout_cnt = 2;
	//int 		  tbl_num = 0;
	int			  agc_gain = 0;
	unsigned char ckill = 0x00;
	//unsigned char analog_eq_val = 0x00;
	//unsigned char acc_ref_val = 0x00;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00 );
	ckill = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xE8+(ch%4));
	agc_gain = GetAccGain(ch);

	/* It'll get EQ stage When The mode come back to Normal, in case of TFHD@25,30p */
	if( resol == NVP6134_VI_EXT_1080P )
	{
		if( s_ckill[ch] == 0x46 && ckill == 0x42 ) // previous status is BW(0110)
		{
			/* get a eq stage and set EQ value again */
			 msleep(1000);

			 /* we should get a EQ stage and set EQ value again */
			s_ckill[ch] = ckill;
			s_agcgain[ch] = agc_gain;
			 return 1;
		}
		s_ckill[ch] = ckill;
		s_agcgain[ch] = agc_gain;
		return 0;
	}

	/* skip video format except for AHD@720p, AFHD@2530p, AHD@5060p */
	if( (resol != NVP6134_VI_HDEX) && (resol != NVP6134_VI_720P_2530) && (resol != NVP6134_VI_720P_5060) )
	{
		//printk(">>>>> DRV[%s:%d] CH:%d, SKIP, running time:recovery BW mode, Not AHD\n", __func__, __LINE__, ch );
		return 0;
	}

	/* If agc_gain status continue saturation(default:5seconds), set fsc lock mode */
	if( agc_gain == 0x7ff )
	{
		s_againSaturationCnt[ch]++;
	}
	else
	{
		s_againSaturationCnt[ch] = 0;
	}
	//printk(">>>>> DRV[%s:%d] CH:%d, CKILL:0x%x, AGC_gain:0x%x, againSaturationNum:%d\n", __func__, __LINE__, ch, ckill, agc_gain, s_againSaturationCnt[ch] );

	if( ( ((s_ckill[ch]&0x02) == 0x2) && ((ckill&0x02) == 0x0) ) || ( (s_agcgain[ch] == 0x7ff) && (agc_gain != 0x7ff) ) \
			|| (s_againSaturationCnt[ch] > DURATION_SATURATION_TIME ) )
	{
		/* set fsc lock and clear buffer */
		s_againSaturationCnt[ch] = 0;

		/* check CKILL */
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00 );
		ckill = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xE8+(ch%4));
		if( (ckill&0x02) == 0 )
		{
			printk(">>>>> DRV[%s:%d] 111.CH:%d, CKILL:0x%x \n", __func__, __LINE__, ch, ckill );
			printk(">>>>> DRV[%s:%d] 111.CH:%d, CKILL:0x%x \n", __func__, __LINE__, ch, ckill );
			printk(">>>>> DRV[%s:%d] 111.CH:%d, CKILL:0x%x \n", __func__, __LINE__, ch, ckill );
			printk(">>>>> DRV[%s:%d] 111.CH:%d, CKILL:0x%x \n", __func__, __LINE__, ch, ckill );
			printk(">>>>> DRV[%s:%d] 111.CH:%d, CKILL:0x%x \n", __func__, __LINE__, ch, ckill );

			printk(">>>>> DRV[%s:%d] 222.CH:%d, CKILL:0x%x \n", __func__, __LINE__, ch, ckill );
			printk(">>>>> DRV[%s:%d] 222.CH:%d, CKILL:0x%x \n", __func__, __LINE__, ch, ckill );
			printk(">>>>> DRV[%s:%d] 222.CH:%d, CKILL:0x%x \n", __func__, __LINE__, ch, ckill );
			printk(">>>>> DRV[%s:%d] 222.CH:%d, CKILL:0x%x \n", __func__, __LINE__, ch, ckill );
			printk(">>>>> DRV[%s:%d] 222.CH:%d, CKILL:0x%x \n", __func__, __LINE__, ch, ckill );
		}

		/* check CkILL, if it is wrong, reset */
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00 );
		ckill = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xE8+(ch%4));
		if( (ckill&0x02) == 0 )
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+(ch%4), 0x01);
			printk(">>>>> DRV[%s:%d] CH:%d, reset, so.....!!!!!!!\n", __func__,__LINE__, ch );
			printk(">>>>> DRV[%s:%d] CH:%d, reset, so.....!!!!!!!\n", __func__,__LINE__, ch );
			printk(">>>>> DRV[%s:%d] CH:%d, reset, so.....!!!!!!!\n", __func__,__LINE__, ch );
			printk(">>>>> DRV[%s:%d] CH:%d, reset, so.....!!!!!!!\n", __func__,__LINE__, ch );
			printk(">>>>> DRV[%s:%d] CH:%d, reset, so.....!!!!!!!\n", __func__,__LINE__, ch );
}
	}

	s_ckill[ch] = ckill;
	s_agcgain[ch] = agc_gain;

	//printk( ">>>>> DRV[%s:%d] CH:%d, s_ckill[%d]:0x%x, s_agcgain[%d]:0x%x\n", __func__, __LINE__, ch, ch, s_ckill[ch], ch, s_agcgain[ch] );

	return 0;
}

/*******************************************************************************
*	Description		: set eq value for CHD, AHD
*	Argurments		: ch : channel, resol : resolution,  vfmt : vidoe format(NTSC/PAL)
*					  stage : distance
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
static int get_eq_setting_value( unsigned char ch, int tbl_num, int stage, int val)
{
	int ret = 0;

	if ( tbl_num == EQ_VIDEO_MODE_AFHD_2550 )
		ret = s_eq_1080p2550[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AFHD_3060 )
		ret = s_eq_1080p3060[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_25 )
		ret = s_eq_720p2550[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_30 )
		ret = s_eq_720p3060[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_GAIN_25 )
		ret = s_eq_720p2550_gain[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_GAIN_30 )
		ret = s_eq_720p3060_gain[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CFHD_30 )
		ret = s_eq_chd_1080p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CFHD_25 )
		ret = s_eq_chd_1080p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_30 )
		ret = s_eq_chd_720p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_25 )
		ret = s_eq_chd_720p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_60 )
		ret = s_eq_chd_720p60[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_50 )
		ret = s_eq_chd_720p50[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_TFHD_25 )
		ret = s_eq_thd_1080p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_TFHD_30 )
		ret = s_eq_thd_1080p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_A25 )
		ret = s_eq_thd_720p25A[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_A30 )
		ret = s_eq_thd_720p30A[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_B25 )
		ret = s_eq_thd_720p25B[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_B30 )
		ret = s_eq_thd_720p30B[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_18 )
		ret = s_eq_3m_18p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_25 )
		ret = s_eq_3m_25p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_30 )
		ret = s_eq_3m_30p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_3M_18 )
		ret = s_eq_thd_3m_18p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_5M_12_5P )
		ret = s_eq_thd_5m_12p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_NRT )
		ret = s_eq_4m_nrt[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_25P )
		ret = s_eq_4m_25p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_30P )
		ret = s_eq_4m_30p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_5M_12_5P )
		ret = s_eq_5m_12_5p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_5M_20P )
		ret = s_eq_5m_20p[stage][val];

	return ret;
}

/*******************************************************************************
*	Description		: get a coeff_A
*	Argurments		: ch(channel), tbl_num(table number), stage, val(idex)
*	Return value	: value acording to the index
*	Modify			:
*	warning			:
*******************************************************************************/
static int get_coeff_A_setting_value( unsigned char ch, int tbl_num, int stage, int val)
{
	int ret = 0;

	if ( tbl_num == EQ_VIDEO_MODE_AFHD_2550 )
		ret = s_eq_A_coeff_1080p2550[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AFHD_3060 )
		ret = s_eq_A_coeff_1080p3060[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_25 )
		ret = s_eq_A_coeff_720p2550[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_30 )
		ret = s_eq_A_coeff_720p3060[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_GAIN_25 )
		ret = s_eq_A_coeff_720p2550_gain[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_GAIN_30 )
		ret = s_eq_A_coeff_720p3060_gain[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CFHD_30 )
		ret = s_eq_chd_A_coeff_1080p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CFHD_25 )
		ret = s_eq_chd_A_coeff_1080p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_30 )
		ret = s_eq_chd_A_coeff_720p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_25 )
		ret = s_eq_chd_A_coeff_720p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_60 )
		ret = s_eq_chd_A_coeff_720p60[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_50 )
		ret = s_eq_chd_A_coeff_720p50[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_TFHD_25 )
		ret = s_eq_thd_A_coeff_1080p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_TFHD_30 )
		ret = s_eq_thd_A_coeff_1080p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_A25 )
		ret = s_eq_thd_A_coeff_720p25A[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_A30 )
		ret = s_eq_thd_A_coeff_720p30A[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_B25 )
		ret = s_eq_thd_A_coeff_720p25B[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_B30 )
		ret = s_eq_thd_A_coeff_720p30B[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_18 )
		ret = s_eq_A_coeff_3m_18p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_25 )
		ret = s_eq_A_coeff_3m_25p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_30 )
		ret = s_eq_A_coeff_3m_30p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_3M_18 )
		ret = s_eq_thd_A_coeff_3m_18p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_5M_12_5P )
		ret = s_eq_thd_A_coeff_5m_12p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_NRT )
		ret = s_eq_A_coeff_4m_nrt[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_25P )
		ret = s_eq_A_coeff_4m_25p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_30P )
		ret = s_eq_A_coeff_4m_30p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_5M_12_5P )
		ret = s_eq_A_coeff_5m_12_5p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_5M_20P )
		ret = s_eq_A_coeff_5m_20p[stage][val];

	return ret;
}

/*******************************************************************************
*	Description		: get a coeff_B
*	Argurments		: ch(channel), tbl_num(table number), stage, val(idex)
*	Return value	: value acording to the index
*	Modify			:
*	warning			:
*******************************************************************************/
static int get_coeff_B_setting_value( unsigned char ch, int tbl_num, int stage, int val)
{
	int ret = 0;

	if ( tbl_num == EQ_VIDEO_MODE_AFHD_2550 )
		ret = s_eq_B_coeff_1080p2550[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AFHD_3060 )
		ret = s_eq_B_coeff_1080p3060[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_25 )
		ret = s_eq_B_coeff_720p2550[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_30 )
		ret = s_eq_B_coeff_720p3060[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_GAIN_25 )
		ret = s_eq_B_coeff_720p2550_gain[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_GAIN_30 )
		ret = s_eq_B_coeff_720p3060_gain[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CFHD_30 )
		ret = s_eq_chd_B_coeff_1080p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CFHD_25 )
		ret = s_eq_chd_B_coeff_1080p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_30 )
		ret = s_eq_chd_B_coeff_720p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_25 )
		ret = s_eq_chd_B_coeff_720p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_60 )
		ret = s_eq_chd_B_coeff_720p60[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_50 )
		ret = s_eq_chd_B_coeff_720p50[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_TFHD_25 )
		ret = s_eq_thd_B_coeff_1080p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_TFHD_30 )
		ret = s_eq_thd_B_coeff_1080p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_A25 )
		ret = s_eq_thd_B_coeff_720p25A[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_A30 )
		ret = s_eq_thd_B_coeff_720p30A[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_B25 )
		ret = s_eq_thd_B_coeff_720p25B[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_B30 )
		ret = s_eq_thd_B_coeff_720p30B[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_18 )
		ret = s_eq_B_coeff_3m_18p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_25 )
		ret = s_eq_B_coeff_3m_25p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_30 )
		ret = s_eq_B_coeff_3m_30p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_3M_18 )
		ret = s_eq_thd_B_coeff_3m_18p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_5M_12_5P )
		ret = s_eq_thd_B_coeff_5m_12p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_NRT )
		ret = s_eq_B_coeff_4m_nrt[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_25P )
		ret = s_eq_B_coeff_4m_25p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_30P )
		ret = s_eq_B_coeff_4m_30p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_5M_12_5P )
		ret = s_eq_B_coeff_5m_12_5p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_5M_20P )
		ret = s_eq_B_coeff_5m_20p[stage][val];

	return ret;
}

/*******************************************************************************
*	Description		: get a color value
*	Argurments		: ch(channel), tbl_num(table number), stage, val(idex)
*	Return value	: value acording to the index
*	Modify			:
*	warning			:
*******************************************************************************/
static int get_color_setting_value( unsigned char ch, int tbl_num, int stage, int val)
{
	int ret = 0;

	if ( tbl_num == EQ_VIDEO_MODE_AFHD_2550 )
		ret = s_eq_color_1080p2550[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AFHD_3060 )
		ret = s_eq_color_1080p3060[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_25 )
		ret = s_eq_color_720p2550[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_30 )
		ret = s_eq_color_720p3060[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_GAIN_25 )
		ret = s_eq_color_720p2550_gain[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_GAIN_30 )
		ret = s_eq_color_720p3060_gain[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CFHD_30 )
		ret = s_eq_chd_color_1080p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CFHD_25 )
		ret = s_eq_chd_color_1080p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_30 )
		ret = s_eq_chd_color_720p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_25 )
		ret = s_eq_chd_color_720p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_60 )
		ret = s_eq_chd_color_720p60[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_50 )
		ret = s_eq_chd_color_720p50[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_TFHD_25 )
		ret = s_eq_thd_color_1080p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_TFHD_30 )
		ret = s_eq_thd_color_1080p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_A25 )
		ret = s_eq_thd_color_720p25A[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_A30 )
		ret = s_eq_thd_color_720p30A[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_B25 )
		ret = s_eq_thd_color_720p25B[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_B30 )
		ret = s_eq_thd_color_720p30B[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_18 )
		ret = s_eq_color_3m_18p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_25 )
		ret = s_eq_color_3m_25p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_30 )
		ret = s_eq_color_3m_30p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_3M_18 )
		ret = s_eq_thd_color_3m_18p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_5M_12_5P )
		ret = s_eq_thd_color_5m_12p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_NRT )
		ret = s_eq_color_4m_nrt[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_25P )
		ret = s_eq_color_4m_25p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_30P )
		ret = s_eq_color_4m_30p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_5M_12_5P )
		ret = s_eq_color_5m_12_5p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_5M_20P )
		ret = s_eq_color_5m_20p[stage][val];

	return ret;
}


/*******************************************************************************
*	Description		: Set EQ value
*	Argurments		: ch(channel information)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void eq_adjust_eqvalue( unsigned char ch, unsigned char resol, int vfmt, int stage )
{
	int tbl_num = 0;
	equalizer_baseon  eqbase;
	equalizer_coeff_a eqcoeffA;
	equalizer_coeff_b eqcoeffB;
	equalizer_color   eqcolor;

	if((resol < NVP6134_VI_720P_2530) || (resol >= NVP6134_VI_BUTT))
		return;

	/* get a EQ table number according to resolution */
	tbl_num = get_resol_to_eqtable( ch, resol, vfmt );


	/* set eq value */
	eqbase.eq_analog_eq 	= get_eq_setting_value( ch, tbl_num, stage, 0);
	eqbase.eq_digital_eq1 	= get_eq_setting_value( ch, tbl_num, stage, 1);
	eqbase.eq_digital_eq2 	= get_eq_setting_value( ch, tbl_num, stage, 2);
	eqbase.eq_g_sel 		= get_eq_setting_value( ch, tbl_num, stage, 3);
	eqbase.eq_b_lpf 		= get_eq_setting_value( ch, tbl_num, stage, 4);
	eqbase.eq_lpf_bypass	= get_eq_setting_value( ch, tbl_num, stage, 5);
	eqbase.eq_ref_volt 		= get_eq_setting_value( ch, tbl_num, stage, 6);
	eq_set_value( ch, &eqbase );

	eqcoeffA.coeff_A_a0_8 = get_coeff_A_setting_value( ch, tbl_num, stage, 0);
	eqcoeffA.coeff_A_a0   = get_coeff_A_setting_value( ch, tbl_num, stage, 1);
	eqcoeffA.coeff_A_a1   = get_coeff_A_setting_value( ch, tbl_num, stage, 2);
	eqcoeffA.coeff_A_a2   = get_coeff_A_setting_value( ch, tbl_num, stage, 3);
	eqcoeffA.coeff_A_B0	  = get_coeff_A_setting_value( ch, tbl_num, stage, 4);
	eqcoeffA.coeff_A_B1   = get_coeff_A_setting_value( ch, tbl_num, stage, 5);
	eqcoeffA.coeff_A_B2   = get_coeff_A_setting_value( ch, tbl_num, stage, 6);
	eq_coeff_A_setting_val( ch, &eqcoeffA );

	eqcoeffB.coeff_B_a0_8 = get_coeff_B_setting_value( ch, tbl_num, stage, 0);
	eqcoeffB.coeff_B_a0   = get_coeff_B_setting_value( ch, tbl_num, stage, 1);
	eqcoeffB.coeff_B_a1   = get_coeff_B_setting_value( ch, tbl_num, stage, 2);
	eqcoeffB.coeff_B_a2   = get_coeff_B_setting_value( ch, tbl_num, stage, 3);
	eqcoeffB.coeff_B_B0	  = get_coeff_B_setting_value( ch, tbl_num, stage, 4);
	eqcoeffB.coeff_B_B1   = get_coeff_B_setting_value( ch, tbl_num, stage, 5);
	eqcoeffB.coeff_B_B2   = get_coeff_B_setting_value( ch, tbl_num, stage, 6);
	eq_coeff_B_setting_val( ch, &eqcoeffB );

	eqcolor.color_brightness	= get_color_setting_value( ch, tbl_num, stage, 0);
	eqcolor.color_contrast		= get_color_setting_value( ch, tbl_num, stage, 1);
	eqcolor.color_h_peaking 	= get_color_setting_value( ch, tbl_num, stage, 2);
	eqcolor.color_saturation 	= get_color_setting_value( ch, tbl_num, stage, 3);
	eqcolor.color_hue 			= get_color_setting_value( ch, tbl_num, stage, 4);
	eqcolor.color_u_gain 		= get_color_setting_value( ch, tbl_num, stage, 5);
	eqcolor.color_v_gain 		= get_color_setting_value( ch, tbl_num, stage, 6);
	eqcolor.color_u_offset 		= get_color_setting_value( ch, tbl_num, stage, 7);
	eqcolor.color_v_offset 		= get_color_setting_value( ch, tbl_num, stage, 8);
	eqcolor.color_black_level	= get_color_setting_value( ch, tbl_num, stage, 9);
	eqcolor.color_fsc_mode 		= get_color_setting_value( ch, tbl_num, stage, 10);
	eqcolor.color_acc_ref		= get_color_setting_value( ch, tbl_num, stage, 11);
	eqcolor.color_sat_sub 		= get_color_setting_value( ch, tbl_num, stage, 12);
	eqcolor.eq_h_dly_offset     = get_color_setting_value( ch, tbl_num, stage, 13);
	if(resol == NVP6134_VI_EXC_720P)
	{
		eqcolor.eq_h_dly_offset = eqcolor.eq_h_dly_offset - (vfmt==PAL?0x47:0x5E);
	}
	eqcolor.color_yc_delay 		= get_color_setting_value( ch, tbl_num, stage, 14);
	eqcolor.color_acc_min_val	= get_color_setting_value( ch, tbl_num, stage, 15);
	eqcolor.color_acc_max_val	= get_color_setting_value( ch, tbl_num, stage, 16);
	eq_color_setting_val( ch, &eqcolor );
}

void eq_adjust_recovery( unsigned char ch, unsigned char resol, int vfmt, int stage, char * start_val, char * end_val )
{
	int tbl_num = 0;

	/* get a EQ table number according to resolution */
	tbl_num = get_resol_to_eqtable( ch, resol, vfmt );

	/* get value for use recovery data */
	*start_val   = get_eq_setting_value( ch, tbl_num, stage, 4);
	*end_val	 = get_eq_setting_value( ch, tbl_num, stage, 5);
}

/*******************************************************************************
*	Description		: get Sync width in the BW mode
*	Argurments		: 1).ch : channel
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned int StageGetSyncWidth( unsigned char ch)
{
	unsigned char	 reg_B0_E0 = 0;
	unsigned char	 agc_stable = 0;
	unsigned int	 sync_width = 0;
	unsigned int 	 check_timeout = 0;

	while(agc_stable == 0)
	{
		nvp6134_VD_chnRst(ch);
		msleep(35);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
		reg_B0_E0 = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xE0 )&0xF;
		agc_stable = reg_B0_E0 & (0x01 << (ch%4));

		if( check_timeout++ > 5 )
		{
			printk(">>>>> DRV[%s:%d] CH:%d, TimeOut, AGC_stable[%x] check[%x] in get sync width\n", __func__, __LINE__, ch, reg_B0_E0, agc_stable );
			break;
		}
		msleep(10);
	}
        msleep(100);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	sync_width = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xC4)&0x0F;
	sync_width <<=8;
	sync_width |= gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xC5);
	sync_width = sync_width & 0x0FFF;
	printk(">>>>> DRV[%s:%d] CH:%d, sync_width:0x%x\n", __func__, __LINE__, ch, sync_width );


	return sync_width;
}

/*******************************************************************************
*	Description		: get Sync width in the BW mode
*	Argurments		: 1).ch : channel
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned int GetSyncWidth( unsigned char ch)
{
	unsigned char	 reg_B0_E0 = 0;
	unsigned char	 agc_stable = 0;
	unsigned int	 sync_width = 0;
	unsigned int 	 check_timeout = 0;

	while(agc_stable == 0)
	{
		nvp6134_VD_chnRst(ch);
		msleep(35);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
		reg_B0_E0 = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xE0 )&0xF;
		agc_stable = reg_B0_E0 & (0x01 << (ch%4));

		if( check_timeout++ > 5 )
		{
			printk(">>>>> DRV[%s:%d] CH:%d, TimeOut, AGC_stable[%x] check[%x] in get sync width\n", __func__, __LINE__, ch, reg_B0_E0, agc_stable );
			break;
		}
		msleep(10);
	}
	msleep(100);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	sync_width = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xC4)&0x0F;
	sync_width <<=8;
	sync_width |= gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xC5);
	sync_width = sync_width & 0x0FFF;
	printk(">>>>> DRV[%s:%d] CH:%d, sync_width:0x%x\n", __func__, __LINE__, ch, sync_width );

	/* reduce horizontal noise in BW mode when the system set Digital EQ */
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x22+(ch%4)*0x04, 0x03);

	return sync_width;
}


/*******************************************************************************
*	Description		: get EQ stage in BW mode
*	Argurments		: 1).ch : channel
*				      2).resol : resolution,
*					  3).vfmt : vidoe format(NTSC/PAL)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char eq_get_ahd_stage_ahd_3m_18p_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		stage_min[i] = s_eq_ahd_bwmode_ahd_3m_18p_table[i][cabletype];
		stage_max[i] = s_eq_ahd_bwmode_ahd_3m_18p_table[i][cabletype+1];
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 3M 18p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 3M 18p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 3M 18p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 3M 18p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );

		/* get stage */
		if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
		else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
		else if(decision_value >= stage_min[3] || decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
		else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
		else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
		else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
		else stage = EQ_STAGE_3; // 200M(exception stage)
	}


	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );

	return stage;
}

unsigned char eq_get_ahd_stage_ahd_3m_2530p_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		if( vfmt == PAL )
		{
			stage_min[i] = s_eq_ahd_bwmode_ahd_3m_25p_table[i][cabletype];
			stage_max[i] = s_eq_ahd_bwmode_ahd_3m_25p_table[i][cabletype+1];
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 3M 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 3M 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 3M 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 3M 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );

			/* get stage */
			if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
			else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
			else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
			else if(decision_value >= stage_min[4] || decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
			else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
			else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
			else stage = EQ_STAGE_3; // 200M(exception stage)
		}
		else
		{
			stage_min[i] = s_eq_ahd_bwmode_ahd_3m_30p_table[i][cabletype];
			stage_max[i] = s_eq_ahd_bwmode_ahd_3m_30p_table[i][cabletype+1];
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 3M 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 3M 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 3M 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 3M 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );

			/* get stage */
			if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
			else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
			else if(decision_value >= stage_min[3] || decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
			else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
			else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
			else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
			else stage = EQ_STAGE_3; // 200M(exception stage)
		}
	}

	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );

	return stage;
}

/*******************************************************************************
*	Description		: get EQ stage in BW mode
*	Argurments		: 1).ch : channel
*				      2).resol : resolution,
*					  3).vfmt : vidoe format(NTSC/PAL)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char eq_get_ahd_stage_ahd_4m_15p_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		stage_min[i] = s_eq_ahd_bwmode_ahd_4m_15p_table[i][cabletype];
		stage_max[i] = s_eq_ahd_bwmode_ahd_4m_15p_table[i][cabletype+1];
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 4M 15p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 4M 15p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 4M 15p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 4M 15p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );

		/* get stage */
		if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
		else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
		else if(decision_value >= stage_min[3] || decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
		else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
		else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
		else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
		else stage = EQ_STAGE_3; // 200M(exception stage)
	}


	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );

	return stage;
}

unsigned char eq_get_ahd_stage_ahd_4m_2530p_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		if( vfmt == PAL )
		{
			stage_min[i] = s_eq_ahd_bwmode_ahd_4m_25p_table[i][cabletype];
			stage_max[i] = s_eq_ahd_bwmode_ahd_4m_25p_table[i][cabletype+1];
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 4M 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 4M 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 4M 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 4M 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );

			/* get stage */
			if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
			else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
			else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
			else if(decision_value >= stage_min[4] || decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
			else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
			else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
			else stage = EQ_STAGE_3; // 200M(exception stage)
		}
		else
		{
			stage_min[i] = s_eq_ahd_bwmode_ahd_4m_30p_table[i][cabletype];
			stage_max[i] = s_eq_ahd_bwmode_ahd_4m_30p_table[i][cabletype+1];
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 4M 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 4M 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 4M 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 4M 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );

			/* get stage */
			if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
			else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
			else if(decision_value >= stage_min[3] || decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
			else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
			else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
			else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
			else stage = EQ_STAGE_3; // 200M(exception stage)
		}
	}

	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );

	return stage;
}

unsigned char eq_get_ahd_stage_ahd_5m_12p_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		stage_min[i] = s_eq_ahd_bwmode_ahd_5m_12p_table[i][cabletype];
		stage_max[i] = s_eq_ahd_bwmode_ahd_5m_12p_table[i][cabletype+1];
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 5M 12p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 5M 12p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 5M 12p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 5M 12p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );

		/* get stage */
		if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
		else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
		else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
		else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
		else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
		else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
		else stage = EQ_STAGE_3; // 200M(exception stage)
	}


	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );

	return stage;
}

unsigned char eq_get_ahd_stage_ahd_5m_20p_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		stage_min[i] = s_eq_ahd_bwmode_ahd_5m_20p_table[i][cabletype];
		stage_max[i] = s_eq_ahd_bwmode_ahd_5m_20p_table[i][cabletype+1];
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 5M 20p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 5M 20p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 5M 20p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, ahd 5M 20p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );

		/* get stage */
		if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
		else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
		else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
		else if(decision_value >= stage_min[4] || decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
		else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
		else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
		else stage = EQ_STAGE_3; // 200M(exception stage)
	}


	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );

	return stage;
}

/*******************************************************************************
*	Description		: get EQ stage in BW mode
*	Argurments		: 1).ch : channel
*				      2).resol : resolution,
*					  3).vfmt : vidoe format(NTSC/PAL)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char eq_get_thd_stage_tfhd_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		if( vfmt == PAL )
		{
			stage_min[i] = s_eq_thd_bwmode_tfth25p_table[i][cabletype];
			stage_max[i] = s_eq_thd_bwmode_tfth25p_table[i][cabletype+1];
			printk(">>>>> DVR[%s:%d] CH:%d, tfth 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, tfth 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, tfth 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, tfth 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, tfth 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );

			/* get stage */
			if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
			else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
			else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
			else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
			else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
			else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
			else stage = EQ_STAGE_3; // 200M(exception stage)
		}
		else
		{
			stage_min[i] = s_eq_thd_bwmode_tfth30p_table[i][cabletype];
			stage_max[i] = s_eq_thd_bwmode_tfth30p_table[i][cabletype+1];
			printk(">>>>> DVR[%s:%d] CH:%d, tfth 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, tfth 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, tfth 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, tfth 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, tfth 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );

			/* get stage */
			if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
			else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
			else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
			else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
			else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
			else if(decision_value >= stage_min[6] || decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
			else stage = EQ_STAGE_3; // 200M(exception stage)
		}
	}


	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );

	return stage;
}

unsigned char eq_get_thd_stage_tvi_3m_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		stage_min[i] = s_eq_thd_bwmode_tvi_3m_table[i][cabletype];
		stage_max[i] = s_eq_thd_bwmode_tvi_3m_table[i][cabletype+1];
		printk(">>>>> DVR[%s:%d] CH:%d, tvi 3M, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, tvi 3M, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, tvi 3M, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, tvi 3M, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, tvi 3M, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );

		/* get stage */
		if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
		else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
		else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
		else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
		else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
		else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
		else stage = EQ_STAGE_3; // 200M(exception stage)
	}


	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );

	return stage;
}

unsigned char eq_get_thd_stage_tvi_5m_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		stage_min[i] = s_eq_thd_bwmode_tvi_5m_table[i][cabletype];
		stage_max[i] = s_eq_thd_bwmode_tvi_5m_table[i][cabletype+1];
		printk(">>>>> DVR[%s:%d] CH:%d, tvi 3M, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, tvi 3M, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, tvi 3M, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, tvi 3M, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, tvi 3M, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );

		/* get stage */
		if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
		else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
		else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
		else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
		else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
		else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
		else stage = EQ_STAGE_3; // 200M(exception stage)
	}


	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );

	return stage;
}

/*******************************************************************************
*	Description		: get EQ stage in BW mode
*	Argurments		: 1).ch : channel
*				      2).resol : resolution,
*					  3).vfmt : vidoe format(NTSC/PAL)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char eq_get_thd_stage_720PA_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		if( vfmt == PAL )
		{
			stage_min[i] = s_eq_thd_bwmode_720_25PA_table[i][cabletype];
			stage_max[i] = s_eq_thd_bwmode_720_25PA_table[i][cabletype+1];
			printk(">>>>> DVR[%s:%d] CH:%d, thd 25pA, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 25pA, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 25pA, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 25pA, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 25pA, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		}
		else
		{
			stage_min[i] = s_eq_thd_bwmode_720_30PA_table[i][cabletype];
			stage_max[i] = s_eq_thd_bwmode_720_30PA_table[i][cabletype+1];
			printk(">>>>> DVR[%s:%d] CH:%d, thd 30pA, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 30pA, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 30pA, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 30pA, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 30pA, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		}
	}

	/* get stage */
	if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
	else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
	else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
	else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
	else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
	else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
	else stage = EQ_STAGE_3; // 200M(exception stage)

	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );

	return stage;
}

/*******************************************************************************
*	Description		: get EQ stage in BW mode
*	Argurments		: 1).ch : channel
*				      2).resol : resolution,
*					  3).vfmt : vidoe format(NTSC/PAL)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char eq_get_thd_stage_720PB_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		if( vfmt == PAL )
		{
			stage_min[i] = s_eq_thd_bwmode_720_25PB_table[i][cabletype];
			stage_max[i] = s_eq_thd_bwmode_720_25PB_table[i][cabletype+1];
			printk(">>>>> DVR[%s:%d] CH:%d, thd 25pB, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 25pB, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 25pB, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 25pB, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 25pB, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		}
		else
		{
			stage_min[i] = s_eq_thd_bwmode_720_30PB_table[i][cabletype];
			stage_max[i] = s_eq_thd_bwmode_720_30PB_table[i][cabletype+1];
			printk(">>>>> DVR[%s:%d] CH:%d, thd 30pB, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 30pB, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 30pB, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 30pB, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, thd 30pB, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		}
	}

	/* get stage */
	if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
	else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
	else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
	else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
	else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
	else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
	else stage = EQ_STAGE_3; // 200M(exception stage)

	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );

	return stage;
}

/*******************************************************************************
*	Description		: get EQ stage in BW mode
*	Argurments		: 1).ch : channel
*				      2).resol : resolution,
*					  3).vfmt : vidoe format(NTSC/PAL)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char eq_get_thd_stage_720P50_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		stage_min[i] = s_eq_thd_bwmode_720P50_table[i][cabletype];
		stage_max[i] = s_eq_thd_bwmode_720P50_table[i][cabletype+1];
		printk(">>>>> DVR[%s:%d] CH:%d, thd 50p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, thd 50p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, thd 50p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, thd 50p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, thd 50p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
	}

	/* get stage */
	if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
	else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
	else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
	else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
	else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
	else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
	else stage = EQ_STAGE_3; // 200M(exception stage)

	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );


	return stage;
}

/*******************************************************************************
*	Description		: get EQ stage in BW mode
*	Argurments		: 1).ch : channel
*				      2).resol : resolution,
*					  3).vfmt : vidoe format(NTSC/PAL)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char eq_get_thd_stage_720P60_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		stage_min[i] = s_eq_thd_bwmode_720P60_table[i][cabletype];
		stage_max[i] = s_eq_thd_bwmode_720P60_table[i][cabletype+1];
		printk(">>>>> DVR[%s:%d] CH:%d, thd 60p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, thd 60p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, thd 60p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, thd 60p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		printk(">>>>> DVR[%s:%d] CH:%d, thd 60p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
	}

	/* get stage */
	if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
	else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
	else if(decision_value >= stage_min[3] || decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
	else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
	else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
	else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
	else if(decision_value == 0 ) stage = EQ_STAGE_3; // 200M
	else stage = EQ_STAGE_3; // 200M(exception stage)

	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );

	return stage;
}

/*******************************************************************************
*	Description		: get EQ stage in BW mode
*	Argurments		: 1).ch : channel
*				      2).resol : resolution,
*					  3).vfmt : vidoe format(NTSC/PAL)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char eq_get_ahd_stage_720P2530_bwmode(int ch, unsigned char resol, int vfmt )
{
	int i;
	unsigned char temp = 0;
	unsigned char stage = 0;
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };
	unsigned int decision_value = 0;
	int cabletype = 0;

	/* initialize AEQ */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);
	msleep(33);

	/* get Sync_width for distinguish EQ stage */
	decision_value = GetSyncWidth(ch);

	/* get a Cable Type(Coax[X][0,1], UTP[X][2,3]*/
	temp = s_eq_type.ch_cable_type[ch];
	cabletype = ( temp == CABLE_TYPE_COAX ) ? 0 : 2;

	/* get stage value */
	for( i = 0; i < 7; i++ )
	{
		if( vfmt == PAL )
		{
			stage_min[i] = s_eq_ahd_bwmode_720_25P_table[i][cabletype];
			stage_max[i] = s_eq_ahd_bwmode_720_25P_table[i][cabletype+1];
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 25p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		}
		else
		{
			stage_min[i] = s_eq_ahd_bwmode_720_30P_table[i][cabletype];
			stage_max[i] = s_eq_ahd_bwmode_720_30P_table[i][cabletype+1];
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			printk(">>>>> DVR[%s:%d] CH:%d, ahd 30p, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		}
	}

	/* get stage */
	if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
	else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
	else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
	else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
	else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
	else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
	else stage = EQ_STAGE_3; // 200M(exception stage)

	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );
	printk(">>>>> DRV[%s:%d] CH:%d, STAGE:%d\n", __func__, __LINE__, ch, stage );

	return stage;
}

/*******************************************************************************
*	Description		: get eq stage for CHD, AHD
*	Argurments		: 1).resol : resolution,
*					  2).In case of AHD, CVID
*							  value1 : y_plus_slp(y plus slope)
*							  value2 : y_minus_slp(y minus slope)
*							  value3 : reserved
*					  In case of THD
*							  value1 : EQ pattern Color Gain value
*							  value2 : reserved
*							  value3 : reserved
*					  vfmt : vidoe format(NTSC/PAL)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char eq_get_thd_stage(unsigned char ch, unsigned char resol, int vfmt )
{
	unsigned char stage = 0;
	unsigned int decision_value = 0;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x93);
	msleep(10);
	decision_value = GetAccGain(ch);

	if( decision_value == 2047 )
	{
		stage = eq_get_thd_stage_tfhd_bwmode( ch, resol, vfmt );
	}
	else
	{
		if     (decision_value == s_eq_thd_re_table[0][EQ_DEFAULT_STAGE_VAL] )	stage = EQ_STAGE_0;    // No EQ(NO INPUT)
		else if(decision_value <= s_eq_thd_re_table[1][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_1;    // 0~50M(short)
		else if(decision_value <= s_eq_thd_re_table[2][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_2;    // 50~150M
		else if(decision_value <= s_eq_thd_re_table[3][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_3;    // 150~250M
		else if(decision_value <= s_eq_thd_re_table[4][EQ_DEFAULT_STAGE_VAL]) 	stage = EQ_STAGE_4;    // 250~350M
		else if(decision_value <= s_eq_thd_re_table[5][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_5;    // 350~450M
		else																	stage = EQ_STAGE_6;
	}

	return stage;
}

unsigned char eq_get_thd_3m_stage(unsigned char ch, unsigned char resol, int vfmt )
{
	unsigned char stage = 0;
	unsigned int decision_value = 0;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x93);
	msleep(10);
	decision_value = GetAccGain(ch);

	printk("TVI 3M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 3M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 3M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 3M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 3M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 3M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 3M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 3M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 3M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 3M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 3M AEQ(0x93) ACC_GAIN = %d \n",decision_value);

	if( decision_value == 2047 )
	{
		stage = eq_get_thd_stage_tvi_3m_bwmode( ch, resol, vfmt );
	}
	else
	{
		if     (decision_value == s_eq_thd_3m_re_table[0][EQ_DEFAULT_STAGE_VAL] )	stage = EQ_STAGE_0;    // No EQ(NO INPUT)
		else if(decision_value <= s_eq_thd_3m_re_table[1][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_1;    // 0~50M(short)
		else if(decision_value <= s_eq_thd_3m_re_table[2][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_2;    // 50~150M
		else if(decision_value <= s_eq_thd_3m_re_table[3][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_3;    // 150~250M
		else if(decision_value <= s_eq_thd_3m_re_table[4][EQ_DEFAULT_STAGE_VAL]) 	stage = EQ_STAGE_4;    // 250~350M
		else if(decision_value <= s_eq_thd_3m_re_table[5][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_5;    // 350~450M
		else																	stage = EQ_STAGE_6;
	}

	return stage;
}

unsigned char eq_get_thd_5m_stage(unsigned char ch, unsigned char resol, int vfmt )
{
	unsigned char stage = 0;
	unsigned int decision_value = 0;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x93);
	msleep(10);
	decision_value = GetAccGain(ch);

	printk("TVI 5M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 5M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 5M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 5M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 5M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 5M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 5M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 5M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 5M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 5M AEQ(0x93) ACC_GAIN = %d \n",decision_value);
	printk("TVI 5M AEQ(0x93) ACC_GAIN = %d \n",decision_value);

	if( decision_value == 2047 )
	{
		stage = eq_get_thd_stage_tvi_5m_bwmode( ch, resol, vfmt );
	}
	else
	{
		if     (decision_value == s_eq_thd_5m_re_table[0][EQ_DEFAULT_STAGE_VAL] )	stage = EQ_STAGE_0;    // No EQ(NO INPUT)
		else if(decision_value <= s_eq_thd_5m_re_table[1][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_1;    // 0~50M(short)
		else if(decision_value <= s_eq_thd_5m_re_table[2][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_2;    // 50~150M
		else if(decision_value <= s_eq_thd_5m_re_table[3][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_3;    // 150~250M
		else if(decision_value <= s_eq_thd_5m_re_table[4][EQ_DEFAULT_STAGE_VAL]) 	stage = EQ_STAGE_4;    // 250~350M
		else if(decision_value <= s_eq_thd_5m_re_table[5][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_5;    // 350~450M
		else																	stage = EQ_STAGE_6;
	}

	return stage;
}

/*******************************************************************************
*	Description		: get eq stage for CHD, AHD
*	Argurments		: 1).resol : resolution,
*					  2).In case of AHD, CVID
*							  value1 : y_plus_slp(y plus slope)
*							  value2 : y_minus_slp(y minus slope)
*							  value3 : reserved
*					  In case of THD
*							  value1 : EQ pattern Color Gain value
*							  value2 : reserved
*							  value3 : reserved
*					  vfmt : vidoe format(NTSC/PAL)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char eq_get_thd_stage_720P_A(unsigned char ch, unsigned char resol, int vfmt )
{
	unsigned char stage = 0;
	unsigned int decision_value = 0;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x93);
	msleep(10);
	decision_value = GetAccGain(ch);

	if( decision_value == 2047 )
	{
		stage = eq_get_thd_stage_720PA_bwmode( ch, resol, vfmt );
	}
	else
	{
		if     (decision_value == s_eq_thd_re_table_720P_A[0][EQ_DEFAULT_STAGE_VAL] )	stage = EQ_STAGE_0;    // No EQ(NO INPUT)
		else if(decision_value <= s_eq_thd_re_table_720P_A[1][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_1;    // 0~50M(short)
		else if(decision_value <= s_eq_thd_re_table_720P_A[2][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_2;    // 50~150M
		else if(decision_value <= s_eq_thd_re_table_720P_A[3][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_3;    // 150~250M
		else if(decision_value <= s_eq_thd_re_table_720P_A[4][EQ_DEFAULT_STAGE_VAL]) 	stage = EQ_STAGE_4;    // 250~350M
		else if(decision_value <= s_eq_thd_re_table_720P_A[5][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_5;    // 350~450M
		else																			stage = EQ_STAGE_6;
	}

	return stage;
}

/*******************************************************************************
*	Description		: get eq stage 720P A type
*	Argurments		: 1).ch : channel
*				      2).resol : resolution,
*					  3).vfmt : vidoe format(NTSC/PAL)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char eq_get_thd_stage_720P_B(int ch, unsigned char resol, int vfmt )
{
	unsigned char stage = 0;
	unsigned int decision_value = 0;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x93);
	msleep(10);
	decision_value = GetAccGain(ch);

	stage = eq_get_thd_stage_720PB_bwmode( ch, resol, vfmt );

	return stage;
}

unsigned char eq_get_cvi_stage_720P50(unsigned char ch, unsigned char resol, int vfmt )
{
	unsigned char stage = 0;
	unsigned int decision_value = 0;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x93);
	printk(">>>>> DRV[%s:%d] CH:%d, CHD EQ_SUB_STAGE IN!!!!!!!!!!! EQ = 0x93 APLLYED!!!!!!!! because of 6stage\n", __func__, __LINE__, ch );
	msleep(10);
	decision_value = GetAccGain(ch);

	if     (decision_value == s_eq_cvi_re_table_720P50[0][EQ_DEFAULT_STAGE_VAL] )	stage = EQ_STAGE_0;    // No EQ(NO INPUT)
	else if(decision_value <= s_eq_cvi_re_table_720P50[1][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_1;    // 0~50M(short)
	else if(decision_value <= s_eq_cvi_re_table_720P50[2][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_2;    // 50~150M
	else if(decision_value <= s_eq_cvi_re_table_720P50[3][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_3;    // 150~250M
	else if(decision_value <= s_eq_cvi_re_table_720P50[4][EQ_DEFAULT_STAGE_VAL]) 	stage = EQ_STAGE_4;    // 250~350M
	else if(decision_value <= s_eq_cvi_re_table_720P50[5][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_5;    // 350~450M
	else																			stage = EQ_STAGE_6;

	return stage;
}

unsigned char eq_get_cvi_stage_720P60(unsigned char ch, unsigned char resol, int vfmt )
{
	int stage = 0;
	unsigned int decision_value = 0;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x93);
	printk(">>>>> DRV[%s:%d] CH:%d, CHD EQ_SUB_STAGE IN!!!!!!!!!!! EQ = 0x93 APLLYED!!!!!!! because of 6stage\n", __func__, __LINE__, ch );
	msleep(10);
	decision_value = GetAccGain(ch);

	if     (decision_value == s_eq_cvi_re_table_720P60[0][EQ_DEFAULT_STAGE_VAL] )	stage = EQ_STAGE_0;    // No EQ(NO INPUT)
	else if(decision_value <= s_eq_cvi_re_table_720P60[1][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_1;    // 0~50M(short)
	else if(decision_value <= s_eq_cvi_re_table_720P60[2][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_2;    // 50~150M
	else if(decision_value <= s_eq_cvi_re_table_720P60[3][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_3;    // 150~250M
	else if(decision_value <= s_eq_cvi_re_table_720P60[4][EQ_DEFAULT_STAGE_VAL]) 	stage = EQ_STAGE_4;    // 250~350M
	else if(decision_value <= s_eq_cvi_re_table_720P60[5][EQ_DEFAULT_STAGE_VAL])	stage = EQ_STAGE_5;    // 350~450M
	else																			stage = EQ_STAGE_6;

	return stage;
}
/*******************************************************************************
*	Description		: get eq stage for CHD, AHD
*	Argurments		: 1).resol : resolution,
*					  2).In case of AHD, CVID
*							  value1 : y_plus_slp(y plus slope)
*							  value2 : y_minus_slp(y minus slope)
*							  value3 : reserved
*					  In case of THD
*							  value1 : EQ pattern Color Gain value
*							  value2 : reserved
*							  value3 : reserved
*					  vfmt : vidoe format(NTSC/PAL)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/

static int get_eq_stage_setting_value(int tbl_num, int stage, int val)
{
	int ret = 0;

	if ( tbl_num == EQ_VIDEO_MODE_AFHD_2550 )
		ret = s_eq_distance_1080p2550[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AFHD_3060 )
		ret = s_eq_distance_1080p3060[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_25 )
		ret = s_eq_distance_720p2550[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_30 )
		ret = s_eq_distance_720p3060[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_GAIN_25 )
		ret = s_eq_distance_720p2550_gain[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_GAIN_30 )
		ret = s_eq_distance_720p3060_gain[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CFHD_30 )
		ret = s_eq_distance_chd_1080p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CFHD_25 )
		ret = s_eq_distance_chd_1080p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_30 )
		ret = s_eq_distance_chd_720p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_60 )
		ret = s_eq_distance_chd_720p60[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_25 )
		ret = s_eq_distance_chd_720p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_CHD_50 )
		ret = s_eq_distance_chd_720p50[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_TFHD_25 )
		ret = s_eq_distance_thd_1080p25[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_TFHD_30 )
		ret = s_eq_distance_thd_1080p30[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_A25 )
		ret = s_eq_distance_thd_720p25A[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_A30 )
		ret = s_eq_distance_thd_720p30A[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_B25 )
		ret = s_eq_distance_thd_720p25B[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_B30 )
		ret = s_eq_distance_thd_720p30B[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_18 )
		ret = s_eq_distance_3m_18p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_25 )
		ret = s_eq_distance_3m_25p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_3M_30 )
		ret = s_eq_distance_3m_30p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_3M_18 )
		ret = s_eq_distance_thd_3m_18p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_THD_5M_12_5P )
		ret = s_eq_distance_thd_5m_12_5p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_NRT )
		ret = s_eq_distance_4m_nrt[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_25P )
		ret = s_eq_distance_4m_25p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_4M_30P )
			ret = s_eq_distance_4m_30p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_5M_12_5P )
		ret = s_eq_distance_5m_12_5p[stage][val];
	else if ( tbl_num == EQ_VIDEO_MODE_AHD_5M_20P )
		ret = s_eq_distance_5m_20p[stage][val];

	return ret;
}

unsigned char eq_get_stage(unsigned char ch, unsigned char resol, unsigned int value1, unsigned int value2, unsigned int value3, int vfmt )
{
	int i;
	int eq_tbl_num;
	int y_plus_slp = 0;
	int y_minus_slp = 0;
	int h_sync_width = 0;
	int acc_gain = 0;
	int stage = 0;
	int decision_value = 0;
	int cabletype = s_eq_type.ch_cable_type[ch];
	unsigned int stage_min[8] = {0, }, stage_max[8] = {0, };

	printk(">>>>> DRV[%s:%d] CH:%d, Cable type : %s\n", __func__, __LINE__, ch, ( cabletype == CABLE_TYPE_COAX ) ? "Coaxial" : "UTP" );

	/* get a EQ table number according to resolution
	 * You can see s_eq_distance_table table structure */
	eq_tbl_num = get_resol_to_eqtable( ch, resol, vfmt );

	/* get THD's stage */
	if(	(resol == NVP6134_VI_EXT_720PA) ||  (resol == NVP6134_VI_EXT_HDAEX) || \
		(resol == NVP6134_VI_EXT_720PB) || 	(resol == NVP6134_VI_EXT_HDBEX) || \
		(resol == NVP6134_VI_EXT_720PRT)|| 	(resol == NVP6134_VI_EXT_1080P)	|| \
		(resol == NVP6134_VI_EXT_3M_NRT)|| 	(resol == NVP6134_VI_EXT_5M_NRT))
	{
		/* allocate value(EQ pattern Color Gain value) to variable for AHD, CHD */
		decision_value = value1;

		/* we distinguish 6 stage by using s_eq_distance_table table */
		if     (decision_value == get_eq_stage_setting_value( eq_tbl_num, 0, cabletype)) stage = EQ_STAGE_0;    // No EQ(NO INPUT)
		else if(decision_value <= get_eq_stage_setting_value( eq_tbl_num, 1, cabletype)) stage = EQ_STAGE_1;    // 0~50M(short)
		else if(decision_value <= get_eq_stage_setting_value( eq_tbl_num, 2, cabletype)) stage = EQ_STAGE_2;    // 50~150M
		else if(decision_value <= get_eq_stage_setting_value( eq_tbl_num, 3, cabletype)) stage = EQ_STAGE_3;    // 150~250M
        else if(decision_value <= get_eq_stage_setting_value( eq_tbl_num, 4, cabletype)) stage = EQ_STAGE_4;    // 250~350M
        else if(decision_value <= get_eq_stage_setting_value( eq_tbl_num, 5, cabletype)) stage = EQ_STAGE_5;    // 350~450M
        else   						  											 		 stage = EQ_STAGE_6;    // 450~550M
    }
	/* get CHD's stage */
	else if((resol == NVP6134_VI_EXC_720P)  || 	(resol == NVP6134_VI_EXC_HDEX)	|| \
			(resol == NVP6134_VI_EXC_720PRT)|| 	(resol == NVP6134_VI_EXC_1080P) )
		{
		/* allocate value(EQ pattern Color Gain value) to variable for AHD, CHD */
		decision_value = value1;

		/* we distinguish 6 stage by using s_eq_distance_table table */
		/* get stage value */
		if( ( (resol == NVP6134_VI_EXC_720P) || (resol == NVP6134_VI_EXC_HDEX) ) && (vfmt == PAL) )
		{
			for( i = 0; i < 7; i++ )
			{
				stage_min[i] = s_eq_chd_720p25_table[i][cabletype];
				stage_max[i] = s_eq_chd_720p25_table[i][cabletype+1];
			}

			/* get stage */
			if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
			else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
			else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
			else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
			else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
			else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
			else stage = EQ_STAGE_3; // 200M(exception stage)
			printk(">>>>> DVR[%s:%d] CH:%d, CVI 720p25, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		}
		else if( ( (resol == NVP6134_VI_EXC_720P) || (resol == NVP6134_VI_EXC_HDEX) ) && (vfmt != PAL) )
		{
			for( i = 0; i < 7; i++ )
			{
				stage_min[i] = s_eq_chd_720p30_table[i][cabletype];
				stage_max[i] = s_eq_chd_720p30_table[i][cabletype+1];
			}

			/* get stage */
			if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
			else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
			else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
			else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
			else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
			else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
			else stage = EQ_STAGE_3; // 200M(exception stage)
			printk(">>>>> DVR[%s:%d] CH:%d, CVI 720p30, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		}
		else if( (resol == NVP6134_VI_EXC_720PRT) && (vfmt == PAL) )
		{
			for( i = 0; i < 7; i++ )
			{
				stage_min[i] = s_eq_chd_720p50_table[i][cabletype];
				stage_max[i] = s_eq_chd_720p50_table[i][cabletype+1];
			}

			/* get stage */
			if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
			else if(decision_value >= stage_min[2] || decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
			else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
			else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
			else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
			else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
			else stage = EQ_STAGE_3; // 200M(exception stage)
			printk(">>>>> DVR[%s:%d] CH:%d, CVI 720p50, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		}
		else if( (resol == NVP6134_VI_EXC_720PRT) && (vfmt != PAL) )
		{
			for( i = 0; i < 7; i++ )
			{
				stage_min[i] = s_eq_chd_720p60_table[i][cabletype];
				stage_max[i] = s_eq_chd_720p60_table[i][cabletype+1];
			}

			/* get stage */
			if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
			else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
			else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
			else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
			else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
			else if(decision_value >= stage_min[6] || decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
			else stage = EQ_STAGE_3; // 200M(exception stage)
			printk(">>>>> DVR[%s:%d] CH:%d, CVI 720p60, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		}
		else if( (resol == NVP6134_VI_EXC_1080P) && (vfmt == PAL) )
		{
			for( i = 0; i < 7; i++ )
			{
				stage_min[i] = s_eq_chd_1080p25_table[i][cabletype];
				stage_max[i] = s_eq_chd_1080p25_table[i][cabletype+1];
			}

			/* get stage */
			if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
			else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
			else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
			else if(decision_value >= stage_min[4] && decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
			else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
			else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
			else stage = EQ_STAGE_3; // 200M(exception stage)
			printk(">>>>> DVR[%s:%d] CH:%d, CVI 1080p25, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
		}
		else if( (resol == NVP6134_VI_EXC_1080P) && (vfmt != PAL) )
			{
			for( i = 0; i < 7; i++ )
				{
				stage_min[i] = s_eq_chd_1080p30_table[i][cabletype];
				stage_max[i] = s_eq_chd_1080p30_table[i][cabletype+1];
				}

			/* get stage */
			if(  	decision_value >= stage_min[1] && decision_value <= stage_max[1] ) stage = EQ_STAGE_1;	// short
			else if(decision_value >= stage_min[2] && decision_value <= stage_max[2] ) stage = EQ_STAGE_2;	// 100M
			else if(decision_value >= stage_min[3] && decision_value <= stage_max[3] ) stage = EQ_STAGE_3;	// 200M
			else if(decision_value >= stage_min[4] || decision_value <= stage_max[4] ) stage = EQ_STAGE_4;	// 300M
			else if(decision_value >= stage_min[5] && decision_value <= stage_max[5] ) stage = EQ_STAGE_5;	// 400M
			else if(decision_value >= stage_min[6] && decision_value <= stage_max[6] ) stage = EQ_STAGE_6;	// 500M
			else stage = EQ_STAGE_3; // 200M(exception stage)
			printk(">>>>> DVR[%s:%d] CH:%d, CVI 1080p30, idx:%d, min:%d, max:%d, decision_value:%d\n", __func__, __LINE__, ch, i, stage_min[i], stage_max[i], decision_value );
			}
		else
			{
			printk(">>>>> DVR[%s:%d] CH:%d, CVI Getstage error!!!!!!! \n", __func__, __LINE__, ch );
			printk(">>>>> DVR[%s:%d] CH:%d, CVI Getstage error!!!!!!! \n", __func__, __LINE__, ch );
			printk(">>>>> DVR[%s:%d] CH:%d, CVI Getstage error!!!!!!! \n", __func__, __LINE__, ch );
			}
		}
	/* 3M over */
	else if( (resol == NVP6134_VI_3M)      || (resol == NVP6134_VI_3M_NRT) || \
			 (resol == NVP6134_VI_4M_NRT)  || (resol == NVP6134_VI_4M)	   || \
			 (resol == NVP6134_VI_5M_NRT)  || (resol == NVP6134_VI_5M_20P)   )
	{
		int i = 0;
		int highCnt = 0;
		unsigned char cnt = 0;
		unsigned char sum_stage[7] = {0, };
		while( cnt < 5 )
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x27,0x57);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58,0x03);

			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x09);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x80+(ch%4)*0x20,0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+(ch%4)*0x20,0x00);
			msleep(35);

			/* sum are y minus slope + y plus slope */
			y_plus_slp = GetYPlusSlope(ch);
			y_minus_slp = GetYMinusSlope(ch);
			decision_value = y_minus_slp + y_plus_slp;

			/* we distinguish 6 stage by using s_eq_distance_table table */
			if     (decision_value == get_eq_stage_setting_value( eq_tbl_num, 0, cabletype)) stage = EQ_STAGE_0;    // No EQ(NO INPUT)
			else if(decision_value >= get_eq_stage_setting_value( eq_tbl_num, 1, cabletype)) stage = EQ_STAGE_1;    // 0~50M(short)
			else if(decision_value >= get_eq_stage_setting_value( eq_tbl_num, 2, cabletype)) stage = EQ_STAGE_2;    // 50~150M
			else if(decision_value >= get_eq_stage_setting_value( eq_tbl_num, 3, cabletype)) stage = EQ_STAGE_3;    // 150~250M
			else if(decision_value >= get_eq_stage_setting_value( eq_tbl_num, 4, cabletype)) stage = EQ_STAGE_4;    // 250~350M
			else if(decision_value >= get_eq_stage_setting_value( eq_tbl_num, 5, cabletype)) stage = EQ_STAGE_5;    // 350~450M
			else   						  											 		 stage = EQ_STAGE_6;    // 450~550M

			sum_stage[stage]++; //buf[0]:stage0, buf[1]:stage1 .... buf[6]:stage6

			printk(">>>>> DRV[%s:%d] CH:%2d, step:%d, stage:%d, AHD 3/4/5M Y_slp[%3d], Y_p_slp[%3d], Y_m_slp[%3d], acc_gain[%3d], h_sync_width[%3d]\n", \
					__func__, __LINE__, ch, cnt, stage, decision_value, y_plus_slp, y_minus_slp, acc_gain, h_sync_width  );
			cnt++;
		}

		/* decision stage - If sum of stage count exist same, use low stage */
		for( i = 6; i >= 0; i-- )
		{
			if( highCnt <= sum_stage[i] )
			{
				stage = i;
				highCnt = sum_stage[i];
			}
			printk(">>>>> DRV[%s:%d] CH:%2d, i:%d, sum_stage%d:%d\n", __func__, __LINE__, ch, i, i, sum_stage[i] );
		}
		printk(">>>>> DVR[%s:%d] CH:%2d, Final decision stage:%d\n", __func__, __LINE__, ch, stage );

		/* check stage again, if the stage is 6 */
	    if( stage == EQ_STAGE_6 )
		{
			if ( resol == NVP6134_VI_3M_NRT )
				stage = eq_get_ahd_stage_ahd_3m_18p_bwmode(ch, resol, vfmt);
			else if ( resol == NVP6134_VI_3M )
				stage = eq_get_ahd_stage_ahd_3m_2530p_bwmode(ch, resol, vfmt);
			else if ( resol == NVP6134_VI_4M_NRT )
				stage = eq_get_ahd_stage_ahd_4m_15p_bwmode(ch, resol, vfmt);
			else if ( resol == NVP6134_VI_4M )
				stage = eq_get_ahd_stage_ahd_4m_2530p_bwmode(ch, resol, vfmt);
			else if ( resol == NVP6134_VI_5M_NRT )
				stage = eq_get_ahd_stage_ahd_5m_12p_bwmode(ch, resol, vfmt);
			else // NVP6134_VI_5M_20P
				stage = eq_get_ahd_stage_ahd_5m_20p_bwmode(ch, resol, vfmt);
			printk(">>>>> DVR[%s:%d] CH:%2d, Final decision stage:%d again!\n", __func__, __LINE__, ch, stage );
		}
	}
	/* AHD 1.0, 2.0 */
    else
    {
        if( g_slp_ahd[ch] == 1 )
        {
            /* allocate value(EQ pattern Color Gain value) to variable for AHD, CHD */
            decision_value = value1;

            if( decision_value == 2047 )
            {
           		stage = eq_get_ahd_stage_720P2530_bwmode( ch, resol, vfmt );
            }
            else
            {
				/* we distinguish 6 stage by using s_eq_distance_table table */
				if     (decision_value == get_eq_stage_setting_value( eq_tbl_num, 0, cabletype)) stage = EQ_STAGE_0;    // No EQ(NO INPUT)
				else if(decision_value <= get_eq_stage_setting_value( eq_tbl_num, 1, cabletype)) stage = EQ_STAGE_1;    // 0~50M(short)
				else if(decision_value <= get_eq_stage_setting_value( eq_tbl_num, 2, cabletype)) stage = EQ_STAGE_2;    // 50~150M
				else if(decision_value <= get_eq_stage_setting_value( eq_tbl_num, 3, cabletype)) stage = EQ_STAGE_3;    // 150~250M
				else if(decision_value <= get_eq_stage_setting_value( eq_tbl_num, 4, cabletype)) stage = EQ_STAGE_4;    // 250~350M
				else if(decision_value <= get_eq_stage_setting_value( eq_tbl_num, 5, cabletype)) stage = EQ_STAGE_5;    // 350~450M
				else   						  											 		 stage = EQ_STAGE_6;    // 450~550M
            }
        }
        else 
		{
            /* allocate value(y plus, y minus) to variable for AHD, CHD */
            y_plus_slp  = value1;
            y_minus_slp = value2;

			/* sum are y minus slope + y plus slope */
			decision_value = y_minus_slp + y_plus_slp;

			/* we distinguish 6 stage by using s_eq_distance_table table */
            if     (decision_value == get_eq_stage_setting_value( eq_tbl_num, 0, cabletype)) stage = EQ_STAGE_0;    // No EQ(NO INPUT)
            else if(decision_value >= get_eq_stage_setting_value( eq_tbl_num, 1, cabletype)) stage = EQ_STAGE_1;    // 0~50M(short)
            else if(decision_value >= get_eq_stage_setting_value( eq_tbl_num, 2, cabletype)) stage = EQ_STAGE_2;    // 50~150M
            else if(decision_value >= get_eq_stage_setting_value( eq_tbl_num, 3, cabletype)) stage = EQ_STAGE_3;    // 150~250M
            else if(decision_value >= get_eq_stage_setting_value( eq_tbl_num, 4, cabletype)) stage = EQ_STAGE_4;    // 250~350M
            else if(decision_value >= get_eq_stage_setting_value( eq_tbl_num, 5, cabletype) )stage = EQ_STAGE_5;    // 350~450M
	    	else   						  											 		 stage = EQ_STAGE_6;    // 450~550M
		}
	}

	/* return value is stage */
	return stage;
}

/******************************************************************************
*	Description		: Get EQ pattern color gain
*	Argurments		: ch : channel number
*	Return value	: EQ pattern color gain value
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned int distinguish_GetAccGain(unsigned char ch)
{
	unsigned int acc_gain_status;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
	acc_gain_status = gpio_i2c_read(nvp6134_iic_addr[ch/4],0xE2);
	acc_gain_status <<= 8;
	acc_gain_status |= gpio_i2c_read(nvp6134_iic_addr[ch/4],0xE3);
	//printk("acc_gain_status    : 0x%x\n", acc_gain_status);
	return acc_gain_status;
}

/******************************************************************************
*	Description		: Get distinguish fact(slope-minus/plus, acc gain)
*	Argurments		: ch : channel number, *pYp_slp(Y plus slope), *pYm_slp(Y minus slope)
*					  *pAcc_gain(ACC gain)
*	Return value	: N/A
*	Modify			:
*	warning			:
*******************************************************************************/
void GetDistinguishValue( unsigned char ch, unsigned int *pYp_slp, unsigned int *pYm_slp, unsigned int *pAcc_gain)
{
	int i = 0;
	unsigned int  cnt = 0;
	unsigned int  y_p_slp;
	unsigned int  y_m_slp;
	unsigned int  acc_gain_sts;
	unsigned int  ChdCnt = 0;
	unsigned int  AhdCnt = 0;
	unsigned int  acc_gain[5] = {0, };
	unsigned int  toggle = 0;

	/* in case of AHD&CHD */
	while( cnt < 5 )
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x09);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x80+((ch%4)*0x20), 0xa4);	//Digital EQ0
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+((ch%4)*0x20), 0x00);	//Digital EQ1
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
		if( toggle++ % 2 )
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x27, 0x08);		// ACC reference
		else
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x27, 0x10);		// ACC reference
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x03);  	// Analog EQ

		msleep(30);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90, 0x05);	//comb_mode
		msleep(60);

		y_p_slp		 = GetYPlusSlope(ch);
		y_m_slp		 = GetYMinusSlope(ch);
		acc_gain_sts = distinguish_GetAccGain(ch);

		acc_gain[cnt] = acc_gain_sts;

		printk(">>>>> DRV[%s:%d] CH:%2d, Gather information, step:%2d, Y_p_slp:%4d, Y_m_slp:%4d, acc_gain:%4d, acc_gain_buf[%d]:%4d\n", \
				__func__, __LINE__, ch, cnt, y_p_slp, y_m_slp, acc_gain_sts, cnt, acc_gain[cnt] );

		cnt++;
	}

	/* analysis video format using acc gain */
	for( i = 0; i < 5; i++ )
	{
		if( acc_gain[i] > 2010 )
		{
			AhdCnt++;
			printk(">>>>> DRV[%s:%d] CH:%2d, Analys, AHD, step:%2d, acc_gain:%4d\n", \
					__func__, __LINE__, ch, i, acc_gain[i] );
		}
		else
		{
			ChdCnt++;
			printk(">>>>> DRV[%s:%d] CH:%2d, Analys, CHD, step:%2d, acc_gain:%4d\n", \
					__func__, __LINE__, ch, i, acc_gain[i] );
		}
	}

	/* decision video format */
	if( AhdCnt > ChdCnt)
	{
		*pYp_slp = y_p_slp;
		*pYm_slp = y_m_slp;
		for( i = 0; i < 5; i++ )
		{
			if( acc_gain[i] > 2010 )
			{
				*pAcc_gain = acc_gain[i];
				break;
			}
		}
	}
	else
	{
		*pYp_slp = y_p_slp;
		*pYm_slp = y_m_slp;
		for( i = 0; i < 5; i++ )
		{
			if( acc_gain[i] < 2010 )
			{
				*pAcc_gain = acc_gain[i];
				break;
			}
		}
	}

	printk(">>>>> DVR[%s:%d] CH:%2d, Final decision Video Format:%s[AHD:%d, CHD:%d], y_p_slp:%4d, y_p_slp:%4d, acc_gain:%4d\n", \
			__func__, __LINE__, ch, (AhdCnt>ChdCnt) ? "AHD" : "CHD", AhdCnt, ChdCnt, *pYp_slp, *pYm_slp, *pAcc_gain);

	return;
}

/******************************************************************************
*	Description		: Get Y plus slope
*	Argurments		: ch : channel number
*	Return value	: Y plus slope value
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned int distinguish_GetYPlusSlope(unsigned char ch)
{
	unsigned int y_plus_slp_status;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
	y_plus_slp_status = gpio_i2c_read(nvp6134_iic_addr[ch/4],0xE8);
	y_plus_slp_status <<= 8;
	y_plus_slp_status |= gpio_i2c_read(nvp6134_iic_addr[ch/4],0xE9);
    //printk("y_plus_slp_status  : 0x%x\n", y_plus_slp_status);
	return y_plus_slp_status;
}

/******************************************************************************
*	Description		: Get Y minus slope
*	Argurments		: ch : channel number
*	Return value	: Y minus slope value
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned int distinguish_GetYMinusSlope(unsigned char ch)
{
	unsigned int y_minus_slp_status;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
	y_minus_slp_status = gpio_i2c_read(nvp6134_iic_addr[ch/4],0xEA);
	y_minus_slp_status <<= 8;
	y_minus_slp_status |= gpio_i2c_read(nvp6134_iic_addr[ch/4],0xEB);
    //printk("y_minus_slp_status : 0x%x\n", y_minus_slp_status);
	return y_minus_slp_status;
}

/*******************************************************************************
*	Description		: set distinguish type
*	Argurments		: ch(channel)
*	Return value	: void
*	Modify			:
*	warning			: after distingish between AHD and CVI and set value(format)
*					  to register 0x05~08
*******************************************************************************/
unsigned char eq_set_distinguish_type( unsigned char ch, unsigned char curvidmode, unsigned char vfmt )
{
	unsigned int sum = 0;
	unsigned char format = 0x00;
	unsigned int decision = 0;
	unsigned int  acc_gain_status[16], y_plus_slope[16], y_minus_slope[16];
	unsigned int fmt_det = 0;
	int accgain_timer = 0;
	int ckill;

	/* get acc gain and slope */
	while( accgain_timer < 1000 )
	{
		if( curvidmode == NVP6134_VI_HDEX 		||
		    curvidmode == NVP6134_VI_720P_2530 )
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x09);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x80+((ch%4)*0x20), 0xa4);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+((ch%4)*0x20), 0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x27, 0x20);
			printk(">>>>>>>>>>>>DRV : 720P Gain control for detection <<<<<<<<\n");
		}
		else    // 1080p
		{
			acc_gain_status[ch] = distinguish_GetAccGain(ch);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0xD3);  
			printk(">>>>>>>>>>>>DRV : 1080P Gain control for detection <<<<<<<<\n");
		}
		msleep(35);
		y_plus_slope[ch]	= GetYPlusSlope(ch);
		y_minus_slope[ch]	= GetYMinusSlope(ch);
		sum = y_plus_slope[ch] + y_minus_slope[ch];
		acc_gain_status[ch] = distinguish_GetAccGain(ch);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00 );
		ckill = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xE8+(ch%4));

		printk(">>>>> DRV[CH%d] acc_gain:%d, y_plus_slope:%d, y_minus_slope:%d, sum[yp+ym]:%d ckill:0x%x\n", \
		        ch, acc_gain_status[ch], y_plus_slope[ch], y_minus_slope[ch], sum, (ckill&0x04) );


		if( (acc_gain_status[ch] != 0) )
		{
			if( curvidmode == NVP6134_VI_HDEX ||
			    curvidmode == NVP6134_VI_720P_2530 )
			{
				printk(">>>>>>>>>>>>DRV : 720P inside <<<<<<<<\n");
				if( ( (y_plus_slope[ch] < 60) && (y_minus_slope[ch] < 60) ) )
				{
					if(acc_gain_status[ch] > 2020 && (ckill & 0x04) == 0x04 )
					{
					    decision = 0;      // CVI
					    printk(">>>>> DRV[CH%d] Distingush AHD or CVI =>>> CVI 720P G[%d]!!!\n",ch, acc_gain_status[ch]);
					    fmt_det = 1;
					    g_slp_ahd[ch] = 1;  //check gain value here for using gain table
					}
					else
					{
					    decision = 1;      // AHD
					    printk(">>>>> DRV[CH%d] Distingush AHD or CVI =>>> AHD 720P G[%d]!!!\n",ch, acc_gain_status[ch]);
					    fmt_det = 1;
					}
				}
				else
				{
					if(acc_gain_status[ch] > 2020 && (ckill & 0x04) == 0x04 )
					{
					    decision = 0;      // CVI
					    printk(">>>>> DRV[CH%d] Distingush AHD or CVI =>>> CVI 720P SLP[%d]!!!\n",ch, acc_gain_status[ch]);
					    fmt_det = 1;
					    g_slp_ahd[ch] = 0;  //check slope value here for using slope table
					}
					else
					{
					    decision = 1;      // AHD
					    printk(">>>>> DRV[CH%d] Distingush AHD or CVI =>>> AHD 720P SLP[%d]!!!\n",ch, acc_gain_status[ch]);
					    fmt_det = 1;
					}
				}
				//msleep(33);
				accgain_timer++;
				}
			else 
			{
				printk(">>>>>>>>>>>>DRV : 1080P inside <<<<<<<<\n");
				if( ( (y_plus_slope[ch] < 30) ||  (y_minus_slope[ch] < 30) ) )
				{
					decision = 1;      // CVI
					printk(">>>>> DRV[CH%d] Distingush AHD or CVI =>>> CVI 1080P slp[%d]!!!\n",ch, acc_gain_status[ch]);
					fmt_det = 1;
				}
				else
				{
					if(acc_gain_status[ch] > 2010 && (ckill & 0x04) == 0x04 )
					{
						decision = 0;      // CVI
					 	printk(">>>>> DRV[CH%d] Distingush AHD or CVI =>>> CVI 1080P SLP[%d]!!!\n",ch, acc_gain_status[ch]);
					    	fmt_det = 1;
					}
					else
					{
					    	decision = 1;      // AHD
					    	printk(">>>>> DRV[CH%d] Distingush AHD or CVI =>>> AHD 1080P SLP[%d]!!!\n",ch, acc_gain_status[ch]);
					   	 fmt_det = 1;
					}
				}
				accgain_timer++;
			}
		}
		else
		{
		    	accgain_timer++;
		    	printk(">>> drv : not inside getiing format\n");
		}

		if ( fmt_det == 1 )
		    	break;
	}

	if( decision == 0 ) // 0:CVI, 1:AHD
	{
		/* CVI  mapping */
		if( curvidmode == NVP6134_VI_720P_2530 )
		{
		    printk( ">>>>> DRV[CH%d] NVP6134_VI_720P_2530 = %s\n", ch, ( vfmt == PAL ) ? "PAL": "NTSC" );
		    format = ( vfmt == PAL ) ? 0x2C : 0x2B;
		}
		else if( curvidmode == NVP6134_VI_HDEX )
		{
		    printk( ">>>>> DRV[CH%d] NVP6134_VI_HDEX = %s\n", ch, ( vfmt == PAL ) ? "PAL": "NTSC" );
		    format = ( vfmt == PAL ) ? 0x2C : 0x2B;
		}
		else if( curvidmode == NVP6134_VI_1080P_2530 )
		{
			printk( ">>>>> DRV[CH%d] NVP6134_VI_1080P_2530 = %s\n", ch, ( vfmt == PAL ) ? "PAL": "NTSC" );
			format = ( vfmt == PAL ) ? 0x36 : 0x35;
		}

		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C, format );
		return 1;
	}
	else
	{
		/* AHD - nothing, beacuse The app have already set AHD  */
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C, 0x00 );
	}

	return 0;
}

/*******************************************************************************
*	Description		: set communication value
*	Argurments		: ch(channel), curvimode(current video format), vfmt(ntsc/pal)
*					  setvalue(-2:failed communcation, stage[1~6]:OK communcation)
*	Return value	: ret(N/A)
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char eq_set_communication_value( unsigned char ch, unsigned char resol, unsigned char vfmt, int setvalue )
{
	unsigned char ret = 0x00;

	if( resol == NVP6134_VI_4M_NRT || resol == NVP6134_VI_5M_NRT ||
		resol == NVP6134_VI_4M 	   || resol == NVP6134_VI_5M_20P )
	{
		if( setvalue == -2 )
		{
			/* set typeA */
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x14+(ch%4), 0x00 );
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x21+(ch%4)*4,0x80);
			printk(">>>>> DRV[%s:%d] CH:%d, Set mis commnuication value...\n", __func__, __LINE__, ch );
		}
		else
		{
			/* recovery value */
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x14+(ch%4), 0x90 );
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x21+(ch%4)*4,0x82);
		}
	}

	return ret;
}

/*******************************************************************************
*	End of file
*******************************************************************************/
