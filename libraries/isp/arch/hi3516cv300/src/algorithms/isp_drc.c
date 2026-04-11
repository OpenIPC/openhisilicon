/******************************************************************************

  Copyright (C), 2015-2025, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_drc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016/02/01
  Description   : 
  History       :
  1.Date        : 2016/02/01
	Author      : q00214668
	Modification: Created file

******************************************************************************/
#include <math.h>
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"
#include "isp_math_utils.h"
#include "hi_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define DRC_Y_SFT1 (0)
#define DRC_Y_VAL1 (127)
#define DRC_Y_SFT2 (0)
#define DRC_Y_VAL2 (0)

#define DRC_C_SFT1 (0)
#define DRC_C_VAL1 (127)
#define DRC_C_SFT2 (0)
#define DRC_C_VAL2 (0)

#define DRC_B_SFT1 (0)
#define DRC_B_VAL1 (127)

#define DRC_H_BLK_NUM_1080P  (29)
#define DRC_V_BLK_NUM_1080P  (17)

#define DRC_H_BLK_NUM_720P  (29)
#define DRC_V_BLK_NUM_720P  (17)

#define DRC_H_BLK_NUM_DEF (29)
#define DRC_V_BLK_NUM_DEF (17)

#define DRC_BIN_NUM_Z  (12)

#define DRC_LOCAL_EDGELMT  (155)

#define DRC_STRLENGTH      (255)

#define LUT_MAX_NODE       (257)


#define DRC_K1   (1232567) 
#define DRC_G1   (8213)
#define DRC_F2   (2250000)
#define DRC_G2   (4169)

#define LUT_MIX_CTRL (255)

#define RANGE_FLT_COEF (4)
#define GAIN_CLIP_KNEE (0x0)
#define GAIN_CLIP_KNEE_WDR (0xc)
#define GAIN_CLIP_STEP (0x8)

#define DRC_STRENGTH_TARGET_LINEAR (0x80)
#define DRC_STRENGTH_TARGET_WDR    (0xff)

#define DRC_AUTO_STRENGTH_DEFAULT (0X80)

#define DRC_AUTO_STRENGTH_MAX (0x80)
#define DRC_AUTO_STRENGTH_MIN (0x0)
#define DRC_AUTO_STRENGTH_MAX_WDR (0xFF)
#define DRC_AUTO_STRENGTH_MIN_WDR (0x80)

#define DRC_USER_TM_ENABLE_DEFAULT (0)

#define DRC_BRIGHT_GAIN_LMT_IDX_DEFAULT (0)
#define DRC_DARK_GAIN_LMT_Y_IDX_DEFAULT (0)
#define DRC_DARK_GAIN_LMT_C_IDX_DEFAULT (0)

static const HI_U16 au16DarkGainLmt[134] = {    
    0x3c7f, 0x3c7e, 0x387e, 0x347e, 0x307e, 0x2c7e, 0x287e, 0x247e, 0x207e, 0x1c7e, 0x187e, 0x147e, 0x107e, 0x0c7e, 0x087e, 0x047e, 0x447e,
    0x847e, 0x8476, 0x846e, 0x8466, 0x845e, 0x8456, 0x844e, 0x8446, 0x843e, 0x8436, 0x842e, 0x8426, 0x841e, 0x8416, 0x8412, 0x840e, 0x8494,
    0x8492, 0x8490, 0x848f, 0x851d, 0x851c, 0x851b, 0x851a, 0x8519, 0x8518, 0x8517, 0x8516, 0x8515, 0x85a8, 0x85a7, 0x85a6, 0x85a5, 0x85a4,
    0x85a3, 0x85a2, 0x85a1, 0x85a0, 0x859f, 0x859f, 0x859e, 0x859d, 0x8638, 0x8637, 0x8636, 0x8635, 0x8634, 0x8633, 0x8632, 0x8631, 0x8630,
    0x862f, 0x862e, 0x862d, 0x862c, 0x862b, 0x862a, 0x8629, 0x86d0, 0x86ce, 0x86cd, 0x86cb, 0x86ca, 0x86c9, 0x86c8, 0x86c7, 0x86c6, 0x86c5,
    0x86c4, 0x86c3, 0x86c2, 0x86c1, 0x86c0, 0x86be, 0x86bc, 0x86ba, 0x8771, 0x876d, 0x8769, 0x8765, 0x8761, 0x875d, 0x8759, 0x8755, 0x8751,
    0x874d, 0x8749, 0x8745, 0x8741, 0x873d, 0x8739, 0x8735, 0x8733, 0x8731, 0x872f, 0x872d, 0x872b, 0x8729, 0x8727, 0x8725, 0x8723, 0x8721,
    0x871f, 0x871d, 0x871b, 0x8719, 0x8715, 0x8713, 0x8711, 0x870f, 0x870d, 0x870b, 0x8709, 0x8707, 0x8705, 0x8703, 0x8701
};

static const HI_U16 au16BrightGainLmt[161]= {
    0x007f, 0x0077, 0x006f, 0x0067, 0x005f, 0x0057, 0x004f, 0x004b, 0x0047, 0x0043, 0x003f, 0x003b, 0x0037, 0x0033, 0x002f, 0x002b, 0x0027, 
    0x0023, 0x001f, 0x001b, 0x0017, 0x0016, 0x0015, 0x0014, 0x0013, 0x0012, 0x0011, 0x0010, 0x000f, 0x000e, 0x000d, 0x000c, 0x000b, 0x0094,
    0x0093, 0x0092, 0x0091, 0x0090, 0x008f, 0x011d, 0x011c, 0x011c, 0x011b, 0x011b, 0x011a, 0x011a, 0x0119, 0x0119, 0x0118, 0x0118, 0x0117,
    0x0117, 0x0116, 0x0116, 0x0115, 0x0115, 0x01a8, 0x01a8, 0x01a7, 0x01a7, 0x01a6, 0x01a6, 0x01a5, 0x01a5, 0x01a4, 0x01a4, 0x01a3, 0x01a3,
    0x01a2, 0x01a2, 0x01a1, 0x01a1, 0x01a0, 0x01a0, 0x019f, 0x019f, 0x019e, 0x019e, 0x019d, 0x019d, 0x0239, 0x0238, 0x0238, 0x0237, 0x0237,
    0x0236, 0x0236, 0x0235, 0x0235, 0x0234, 0x0234, 0x0233, 0x0233, 0x0232, 0x0232, 0x0231, 0x0231, 0x0230, 0x0230, 0x022f, 0x022f, 0x022e,
    0x022e, 0x022d, 0x022d, 0x022c, 0x022c, 0x022b, 0x022b, 0x022a, 0x022a, 0x0229, 0x0229, 0x02d1, 0x02d0, 0x02d0, 0x02cf, 0x02cf, 0x02ce,
    0x02ce, 0x02cd, 0x02cd, 0x02cc, 0x02cc, 0x02cb, 0x02cb, 0x02ca, 0x02ca, 0x02c9, 0x02c9, 0x02c8, 0x02c8, 0x02c7, 0x02c7, 0x02c6, 0x02c6,
    0x02c5, 0x02c5, 0x02c4, 0x02c4, 0x02c3, 0x02c3, 0x02c2, 0x02c2, 0x02c1, 0x02c1, 0x02c0, 0x02c0, 0x02bf, 0x02bf, 0x02be, 0x02be, 0x02bd,
    0x02bd, 0x02bc, 0x02bc, 0x02bb, 0x02bb, 0x02ba, 0x02ba, 0x02b9
};

/***********************************************************/
typedef enum hiDRC_PARA_MODE_E
{
	DRC_PARA_MODE_LINEAR = 0,
		
	DRC_PARA_MODE_BUILT_IN,

	DRC_PARA_MODE_2To1_LINE,

	DRC_PARA_MODE_3To1_LINE,

	DRC_PARA_MODE_4To1_LINE,

	DRC_PARA_MODE_BUTT,
} DRC_PARA_MODE_E;

typedef struct hiDRC_PARA_S
{
	HI_U8  u8DrcAsymmetry;
	HI_U8  u8DrcSecondpole;
	HI_U8  u8Stretch;
	HI_U16 u16Compress;
	HI_U8  u8DrcBinNumZ;
} DRC_PARA_S;

static DRC_PARA_S g_DrcPara[DRC_PARA_MODE_BUTT] = 
{
	{2, 180, 54, 180, 7},/*linear*/
	{2, 180, 54, 180, 7},/*build*/
	{3, 190, 40, 180, 12},/*fs-2*/
	{1, 225, 54, 180, 7},/*fs-3*/
	{1, 225, 54, 180, 7},/*fs-4*/
};

typedef struct hiDRC_HIST_WEIGHT_CALC_S
{
	HI_U16 au16IndexTh[7];
	HI_U16 au16Weight[7];
	
} DRC_HIST_WEIGHT_CALC_S;


typedef struct hiISP_DRC_HIST_S
{
	HI_S32  s32HistDark;
	HI_U16  u16ExpRatio;
	
	DRC_HIST_WEIGHT_CALC_S pstHistWeightCalc;
	
} ISP_DRC_HIST_S;


/***********************************************************/


typedef struct hiISP_DRC_S
{
	HI_BOOL bInit;
	HI_BOOL bManuMode;
	HI_BOOL bEnable;
	HI_BOOL bDrcDitherEn;
	
	HI_BOOL bUpdateLut;
	HI_BOOL bUserTmEnable;

	HI_BOOL bTmLutSel;
	
	HI_U8   u8Stretch;	
	HI_U8   u8Asymmetry;
	HI_U8   u8SecondPole;
	HI_U16  u16Compress;

	HI_U16 au16ToneMappingValue0[201];
	HI_U16 au16ToneMappingDiff0[200];

	HI_U16 au16ToneMappingValue1[201];
	HI_U16 au16ToneMappingDiff1[200];
	
	HI_DOUBLE dLutValue0[201];
	HI_DOUBLE dLutValue1[201];    
	HI_U32    K1;
	HI_U32    G1;
	HI_U32    F2;
	HI_U32    G2;
	
	HI_U8   u8LutMixCtrl;
	HI_U8   u8LutMixCtrlPrev;
	HI_U8   u8PdwHighStr;
	HI_U8   u8PdwHighManual;
	HI_U8   u8MixingCoring;
	HI_U8   u8MixingDarkMin;
	HI_U8   u8MixingDarkMax;

	HI_U8   u8MixingBrightMin;
	HI_U8   u8MixingBrightMax;

	HI_U8   u8RangeVarFine;
	HI_U8   u8SpatialVarFine;
	HI_U8   u8RangeVarMedium;
	HI_U8   u8SpatialVarMedium;
	HI_U8   u8RangeVarCoarse;
	HI_U8   u8SpatialVarCoarse;
	HI_U8   u8FltScaleFine;
	HI_U8   u8FltScaleCoarse;
   
	HI_U8   u8Strength;
	HI_U8   u8StrengthBias;
	HI_U8   u8StrengthTarget;
	HI_U8   u8BinNumZ; 

	HI_U8   u8AutoStrengthMax;
	HI_U8   u8AutoStrengthMin;

	HI_U16  u16DarkGainLmtC;
	HI_U16  u16DarkGainLmtY;
    HI_U16  u16BrightGainLmt;
    HI_U8   u8GainClipKnee;
    HI_U8   u8GainClipStep;
		
	ISP_DRC_HIST_S   stDrcHist;
	
} ISP_DRC_S;

ISP_DRC_S g_astDrcCtx[ISP_MAX_DEV_NUM] = {{0}};
#define DRC_GET_CTX(dev, pstCtx)   pstCtx = &g_astDrcCtx[dev]

static HI_DOUBLE g_dDrcLutPoint[201] = {
	1.00001525902190	,0.994614600098015	,0.989243107845890	,0.983900624748680	,0.978586994140223, 
	0.973302060200447	,0.968045667950797	,0.962817663249692	,0.957617892788002	,0.952446204084559, 
	0.947302445481677	,0.942186466140712	,0.937098116037634	,0.932037245958629	,0.927003707495726, 
	0.921997353042439	,0.917018035789446	,0.912065609720277	,0.907139929607036	,0.902240851006141, 
	0.897368230254090	,0.892521924463247	,0.887701791517648	,0.882907690068841	,0.878139479531735, 
	0.873397020080482	,0.868680172644370	,0.863988798903754	,0.859322761285990	,0.854681922961409, 
	0.850066147839301	,0.845475300563922	,0.840909246510528	,0.836367851781427	,0.831850983202052, 
	0.827358508317055	,0.822890295386421	,0.818446213381611	,0.814026131981713	,0.809629921569624, 
	0.805257453228247	,0.800908598736713	,0.796583230566619	,0.792281221878290	,0.788002446517055, 
	0.783746779009556	,0.779514094560059	,0.775304269046801	,0.771117179018348	,0.766952701689974, 
	0.762810714940063	,0.758691097306525	,0.754593727983235	,0.750518486816491	,0.746465254301490, 
	0.742433911578826	,0.738424340430999	,0.734436423278954	,0.730470043178629	,0.726525083817529, 
	0.722601429511313	,0.718698965200401	,0.714817576446602	,0.710957149429758	,0.707117570944405, 
	0.699500509799884	,0.691965499537512	,0.684511656306270	,0.677138105775969	,0.669843983034693, 
	0.662628432487349	,0.655490607755298	,0.648429671577085	,0.641444795710222	,0.634535160834039, 
	0.627699956453576	,0.620938380804516	,0.614249640759135	,0.607632951733272	,0.601087537594296, 
	0.594612630570064	,0.588207471158868	,0.581871308040340	,0.575603397987325	,0.569403005778703, 
	0.563269404113144	,0.557201873523800	,0.551199702293909	,0.545262186373315	,0.539388629295877, 
	0.533578342097782	,0.527830643236725	,0.522144858511963	,0.516520320985239	,0.510956370902542, 
	0.505452355616726	,0.500007629510948	,0.494621553922945	,0.489293497070112	,0.484022833975399, 
	0.478808946394001	,0.473651222740838	,0.468549058018817	,0.463501853747863	,0.458509017894723, 
	0.453569964803518	,0.448684115127045	,0.443850895758824	,0.439069739765868	,0.434340086322185, 
	0.429661380642995	,0.425033073919650	,0.420454623255264	,0.415925491601026	,0.411445147693211, 
	0.407013065990857	,0.402628726614123	,0.398291615283310	,0.394001223258528	,0.389757047280029, 
	0.385558589509174	,0.381405357470032	,0.377296863991617	,0.373232627150745	,0.369212170215499, 
	0.365235021589314	,0.361300714755656	,0.357408788223301	,0.353558785472202	,0.345982749768756, 
	0.338569052887984	,0.331314216243674	,0.324214835788543	,0.317267580417019	,0.310469190402258, 
	0.303816475866636	,0.297306315285032	,0.290935654020170	,0.284701502889351	,0.278600936761900, 
	0.272631093186657	,0.266789171048891	,0.261072429255982	,0.255478185451271	,0.250003814755474, 
	0.244646748535056	,0.239404473197001	,0.234274529009408	,0.229254508947361	,0.224342057563523, 
	0.219534869882934	,0.214830690321498	,0.210227311627632	,0.205722573846605	,0.201314363307062, 
	0.197000611629264	,0.192779294754587	,0.188648431995809	,0.184606085107750	,0.180650357377828, 
	0.176779392736101	,0.169284526443992	,0.162107417894271	,0.155234595201129	,0.148653157642516, 
	0.142350751444676	,0.136315546593329	,0.130536214627991	,0.125001907377737	,0.119702236598500, 
	0.114627254473681	,0.109767434941467	,0.105113655813816	,0.100657181653531	,0.0963896473772935,	
	0.0923030425538749	,0.0883896963680506 ,0.0810537089471357 ,0.0743265788212580 ,0.0681577732966643,	
	0.0625009536888685	,0.0573136272368404 ,0.0525568279069080 ,0.0481948236886467 ,0.0441948481840253,	
	0.0371632894106290	,0.0312504768444343 ,0.0262784139534540 ,0.0220974240920127 ,0.0156252384222171,	
	0.0110487120460063	,0.00781261921110857	,0.00390630960555428	,0.00195315480277714	,0.000976577401388571,	
	0.000488288700694286,0.000244144350347143	,0.000122072175173571	,0.00006103608758678	,0.000030518043793392,	
	0.000015259021896696
}; 

static void GenerateAsymmetry(ISP_DRC_S *pstDrc)
{
   HI_DOUBLE x[201];
   HI_DOUBLE xi;
   HI_DOUBLE Temp,Temp1,Temp2;
   HI_S8	ai;
   HI_DOUBLE as,dp;
   HI_U8	LutMaxOut = 1;
   HI_U8	Asy,Sec;
   HI_U8	i;
   HI_DOUBLE LutValueTemp;
   HI_DOUBLE Stretch;
   HI_DOUBLE Compress;

   Asy = pstDrc->u8Asymmetry;
   Sec = pstDrc->u8SecondPole;
   Stretch = (HI_DOUBLE)pstDrc->u8Stretch/100.0;
   Compress= (HI_DOUBLE)pstDrc->u16Compress/100.0;  
   
   xi = ((HI_DOUBLE)(Asy + 1)/(HI_DOUBLE)257)*2 - 1;
   Temp = 255*((1-1/DIV_0_TO_1(1000*xi*xi*xi)) + xi - ((xi>=0)*2));
   ai = (HI_S8)Temp;
   as = ABS((HI_DOUBLE)ai/255);
   dp = (HI_DOUBLE)Sec/255;
   

   if(ai >= 0)
   {
		for(i = 0;i<201;i++)
		{
			x[i] = pow(*(g_dDrcLutPoint+i),Stretch); 
		}
   }
   else
   {
		for(i = 0;i<201;i++)
		{
			x[i] = pow((1 - *(g_dDrcLutPoint+i)),Stretch);
		}
   }

   for(i = 0;i<201;i++)
   {
		Temp1 = dp + (1-dp)*(ABS((1-dp-x[i])/DIV_0_TO_1(dp))*ABS((1-dp-x[i])/DIV_0_TO_1(dp))*ABS((1-dp-x[i])/DIV_0_TO_1(dp))) ;
		Temp2 = x[i]*(as+1)/DIV_0_TO_1(as+x[i]);
		LutValueTemp = Temp1*Temp2*LutMaxOut;
		pstDrc->dLutValue1[i]= CLIP3(LutValueTemp,0,1);
		//pstDrc->LutValue1[i] = pow(pstDrc->LutValue1[i],1/Stretch);
   }

   if(ai<0)
   {
		for(i = 0;i<201;i++)
		{
			pstDrc->dLutValue1[i] = (HI_FLOAT)LutMaxOut - pstDrc->dLutValue1[i];			
		}
   }

	for(i = 0;i<201;i++)
	 {
		pstDrc->dLutValue1[i] = pow( pstDrc->dLutValue1[i],Compress);
	 }

}

HI_S32 CameraResponseFunction(ISP_DRC_S *pstDrc)
{
  HI_U8 FlipLR;
  HI_U8 i;
  HI_U32 M,L;
  HI_DOUBLE Gamma1[201];
  HI_DOUBLE Gamma2[201];
  HI_DOUBLE GammaCurve[201];
  HI_DOUBLE Diff[201];
  HI_DOUBLE X[201],Smooth[201];
  HI_DOUBLE K1,G1,F2,G2;
  HI_U8 index = 0;
  HI_U8 index_pre=0;
  HI_U8 J;
  HI_DOUBLE T,W1,W2,S;
  HI_DOUBLE temp;

  K1 = (HI_DOUBLE)pstDrc->K1/10000;
  G1 = (HI_DOUBLE)pstDrc->G1/10000;
  F2 = (HI_DOUBLE)pstDrc->F2/10000;
  G2 = (HI_DOUBLE)pstDrc->G2/10000;  

  /*K1 = 123.2567;
  G1 = 0.8213;
  F2 = 225;
  G2 = 0.4169;*/
  
   if(*g_dDrcLutPoint > *(g_dDrcLutPoint+200))
   {
	   for(i = 0;i<201;i++)
	   {
			X[i] = *(g_dDrcLutPoint+200-i);
			X[i] = X[i]*256;
	   }
	   FlipLR = 1;
   }
   else
   {
		FlipLR = 0;
   }

   M = 4096;
   L = 256;
   
   for(i = 0;i<201;i++)
   {
		Gamma1[i] = K1*pow(X[i],G1);
		Gamma2[i] = (M+F2)*pow((X[i]/L),G2) - F2;

		if(Gamma1[i]<Gamma2[i])
		{
			index_pre = i;            
		}

		if(index_pre > index)
		{
			index = index_pre;
		}
   //curve difference
		Diff[i] = Gamma1[i] - Gamma2[i];
   }
   
   //Junction index by searching
   J = index;
   
   T = ((HI_DOUBLE)74/13)*J;

   W1 = ((HI_DOUBLE)125/13)*J;

   W2 = ((HI_DOUBLE)375/13)*J;

   S = 0.5;

   for(i = 0;i<201;i++)
   {
		if(Diff[i]<0)
		{
			temp = MAX2(MIN2((Diff[i]+W1)/DIV_0_TO_1(W1)*T,T),0);
		}
		else
		{
			temp =  MAX2(MIN2((Diff[i]+W1)/DIV_0_TO_1(W1)*T,(1-pow(Diff[i]/DIV_0_TO_1(W2),S))*T),0);
		}
		Smooth[i] = (MIN2(X[i],8)/8)* temp;

		if(i<J-3)
		{
			GammaCurve[i] =  Gamma1[i] - Smooth[i];
		}
		else if(i>=J+2)
		{
			GammaCurve[i] =  Gamma2[i] - Smooth[i];
		}
		else
		{
			GammaCurve[i] =  MIN2(Gamma1[i],Gamma2[i]) - Smooth[i];
		}

		GammaCurve[i] = CLIP3(GammaCurve[i],0,M);

	   
   }

  if (1 == FlipLR)
  {
   
	   for(i = 0;i<201;i++)
	   {   
			pstDrc->dLutValue0[i] = GammaCurve[200-i]/4096;
	   }
  }
  else
  {
		for(i = 0;i<201;i++)
		{
			pstDrc->dLutValue0[i] = GammaCurve[i]/4096;
		}
  }
	   
   
	return HI_SUCCESS;

}

static HI_BOOL DrcJudge(HI_U16 *a,HI_U32 n)
{
	 HI_U32 i= 0;

	 for (i=1;i<n;i++)
	{
		if(a[i]>a[i-1])
		{
			return 0;
		}
	}
	 return 1;
}


static HI_VOID DrcUpdateAsyLUT(ISP_DRC_S *pstDrc)
{
	HI_U32 i;
	HI_DOUBLE dTemp0,dTemp1;

	CameraResponseFunction(pstDrc);
	if (pstDrc->bUserTmEnable)
	{
		pstDrc->dLutValue1[0] = 1.0;
		for(i = 0; i < 200; i++)
		{
			pstDrc->dLutValue1[200 - i] = (HI_DOUBLE)hi_ext_system_drc_tm_lut_read(i) / 65535;
		}
	}
	else
	{
		GenerateAsymmetry(pstDrc);
	}

	for (i = 0; i < 201;i++)
	{
		dTemp0 = (HI_DOUBLE)(((HI_DOUBLE)log(pstDrc->dLutValue0[i]))/((HI_DOUBLE)log(2))) + 16;
	
		pstDrc->au16ToneMappingValue0[i] = (HI_U32)(dTemp0/16*65535 + 0.5);     

		dTemp1 = (HI_DOUBLE)log(pstDrc->dLutValue1[i])/log(2) + 16;     
		
		pstDrc->au16ToneMappingValue1[i] = (HI_U32)(dTemp1/16*65535 + 0.5);     
	}

	return;
}

static HI_VOID DrcUpdateMixedLut(ISP_DRC_S *pstDrc)
{
	HI_U32 i;
	ISP_DEV IspDev = 0;
	HI_U16 au16ToneMappingValue[201];
	HI_U16 au16ToneMappingDiff[200]; 

	if (1 == pstDrc->bTmLutSel)
	{
		// if currently using Lut1, then update Lut0
		for (i = 0; i < 201; i++)
		{   
			au16ToneMappingValue[i] = ((HI_S32)(pstDrc->au16ToneMappingValue1[i] - pstDrc->au16ToneMappingValue0[i])*((HI_S32)pstDrc->u8LutMixCtrl) >> 8) \
				+ pstDrc->au16ToneMappingValue0[i];
		}

		if (DrcJudge(au16ToneMappingValue, 200))
		{
			hi_isp_drc_stat_ind_waddr0_write(IspDev, 0);   
			for(i = 0; i < 200; i++)
			{       
				au16ToneMappingDiff[i]  = CLIP_MAX(au16ToneMappingValue[i] - au16ToneMappingValue[i+1], 4095);
				hi_isp_drc_stat_ind_wdata0_write(IspDev, (((HI_U32)au16ToneMappingValue[i]) << 12) | ((HI_U32)au16ToneMappingDiff[i]));
			}

			if (!pstDrc->bInit) // Initialize LUT1 at the beginning
			{
				hi_isp_drc_stat_ind_waddr1_write(IspDev, 0);   
				for(i = 0; i < 200; i++)
				{       
					hi_isp_drc_stat_ind_wdata1_write(IspDev, 65535 << 12);
				}
			}
		}
	}
	else
	{
		// likewise, if currently using Lut0, then update Lut1
		for (i = 0; i < 201; i++)
		{   
			au16ToneMappingValue[i] = ((HI_S32)(pstDrc->au16ToneMappingValue1[i] - pstDrc->au16ToneMappingValue0[i])*((HI_S32)pstDrc->u8LutMixCtrl) >> 8) \
				+ pstDrc->au16ToneMappingValue0[i];
		}

		if (DrcJudge(au16ToneMappingValue, 200))
		{
			if (!pstDrc->bInit) // Initialize LUT0 at the beginning
			{
				hi_isp_drc_stat_ind_waddr0_write(IspDev, 0);
				for(i = 0; i < 200; i++)
				{       
					hi_isp_drc_stat_ind_wdata0_write(IspDev, 65535 << 12);
				}
			}

			hi_isp_drc_stat_ind_waddr1_write(IspDev, 0);   
			for(i = 0; i < 200; i++)
			{
				au16ToneMappingDiff[i]  = CLIP_MAX(au16ToneMappingValue[i] - au16ToneMappingValue[i+1], 4095);
				hi_isp_drc_stat_ind_wdata1_write(IspDev, (((HI_U32)au16ToneMappingValue[i]) << 12) | ((HI_U32)au16ToneMappingDiff[i]));
			}
		}
	}
	pstDrc->bTmLutSel = !(pstDrc->bTmLutSel); // switch Lut selection
}


static HI_VOID DrcSetRegsVal(ISP_DEV IspDev) 
{
	HI_U32 Temp,u32BinScale, u32DivZ;
	HI_U32 u32Width, u32Height, u32HBlkSize, u32VBlkSize;
	HI_U32 u32HBlkNum, u32VBlkNum, u32BinNumZ;
	HI_DOUBLE Delta;
	HI_U32  Chk_Y,Chk_X;
	HI_FLOAT hblksizetemp,vblksizetemp;
	HI_U32  Div_x0,Div_x1,Div_y0,Div_y1;

	u32HBlkNum = hi_isp_drc_hnum_read(IspDev);
	u32VBlkNum = hi_isp_drc_vnum_read(IspDev);
	u32BinNumZ = hi_isp_drc_bin_read(IspDev);

	u32Width  = (hi_ext_sync_total_width_read());
	u32Height = (hi_ext_sync_total_height_read());
	
	u32HBlkSize = u32Width/DIV_0_TO_1(u32HBlkNum);
	u32VBlkSize = u32Height/DIV_0_TO_1(u32VBlkNum);

	Chk_X = u32Width%DIV_0_TO_1(u32HBlkNum);
	Chk_Y = u32Height%DIV_0_TO_1(u32VBlkNum);

	
	hblksizetemp = ((HI_FLOAT)u32Width)/DIV_0_TO_1(u32HBlkNum);
	
	vblksizetemp = ((HI_FLOAT)u32Height)/DIV_0_TO_1(u32VBlkNum);

	if(hblksizetemp != u32HBlkSize)
	{
		hblksizetemp = (HI_U32)(u32HBlkSize + 1);
	}
	if(vblksizetemp != u32VBlkSize)
	{
		vblksizetemp = (HI_U32)(u32VBlkSize + 1);
	}

	
	Div_x0 = (HI_U32)(32768.0/DIV_0_TO_1(u32HBlkSize) + 0.5);//reciprocal value of block height
	
	Div_x1 = (HI_U32)(32768.0/DIV_0_TO_1(hblksizetemp) + 0.5);
	
	Div_y0 = (HI_U32)(32768.0/DIV_0_TO_1(u32VBlkSize) + 0.5);//reciprocal value of block width
	
	Div_y1 = (HI_U32)(32768.0/DIV_0_TO_1(vblksizetemp) + 0.5);
	
	u32DivZ = (HI_U32)(32768.0/DIV_0_TO_1(u32BinNumZ) + 0.5); 
	

	Temp = u32HBlkSize*u32VBlkSize;

	Delta = (log(u32Width*u32Height/DIV_0_TO_1(u32HBlkNum)/DIV_0_TO_1(u32VBlkNum))/log(2)) -  (HI_U32)(log(u32Width*u32Height/DIV_0_TO_1(u32HBlkNum)/DIV_0_TO_1(u32VBlkNum))/log(2));
	if(Delta>0)
	{
		Temp = (HI_U32)(log(u32Width*u32Height/DIV_0_TO_1(u32HBlkNum)/DIV_0_TO_1(u32VBlkNum))/log(2))+1;
	}
	else
	{
		 Temp = (HI_U32)(log(u32Width*u32Height/DIV_0_TO_1(u32HBlkNum)/DIV_0_TO_1(u32VBlkNum))/log(2));
	}

	Temp = MAX2(11, Temp); 
	u32BinScale = 17 - Temp;//3-bit bin counter shift control (0~4 supported)

	hi_isp_drc_hsize_write(IspDev, u32HBlkSize-1);
	hi_isp_drc_vsize_write(IspDev, u32VBlkSize-1);
	hi_isp_drc_chk_x_write(IspDev, Chk_X);
	hi_isp_drc_chk_y_write(IspDev, Chk_Y);
	
	hi_isp_drc_div_x0_write(IspDev, Div_x0);
	hi_isp_drc_div_x1_write(IspDev, Div_x1);
	hi_isp_drc_div_y0_write(IspDev, Div_y0);
	hi_isp_drc_div_y1_write(IspDev, Div_y1);
	hi_isp_drc_div_z_write(IspDev, u32DivZ);
	
	hi_isp_drc_bin_scale_write(IspDev, u32BinScale);
}

static HI_VOID DrcSetBlockNum(ISP_DEV IspDev) 
{
	HI_U32 u32Width, u32Height;
	
	u32Width = hi_ext_sync_total_width_read();
	u32Height = hi_ext_sync_total_height_read();

	if (u32Width >= 1920 && u32Height >= 1080)
	{
		hi_isp_drc_hnum_write(IspDev, DRC_H_BLK_NUM_1080P);
		hi_isp_drc_vnum_write(IspDev, DRC_V_BLK_NUM_1080P);
	}
	else if (u32Width >= 1280 && u32Height >= 720)
	{
		hi_isp_drc_hnum_write(IspDev, DRC_H_BLK_NUM_720P);
		hi_isp_drc_vnum_write(IspDev, DRC_V_BLK_NUM_720P);
	}
	else
	{
		hi_isp_drc_hnum_write(IspDev, DRC_H_BLK_NUM_DEF);
		hi_isp_drc_vnum_write(IspDev, DRC_V_BLK_NUM_DEF);
	}	
}

static HI_VOID DrcRegsDefault(ISP_DEV IspDev, ISP_CMOS_DEFAULT_S *pstSnsDft) 
{   
	//gamma_fe K=18,S=16
	HI_U8 i;

	ISP_CTX_S *pstIspCtx = HI_NULL;   
	HI_U8 u8WDRMode;

	ISP_GET_CTX(IspDev, pstIspCtx);
	
	u8WDRMode = pstIspCtx->u8SnsWDRMode;

	DrcSetBlockNum(IspDev);
	
	hi_isp_drc_bin_write(IspDev, DRC_BIN_NUM_Z);
	
	hi_isp_drc_local_edge_lmt_write(IspDev, DRC_LOCAL_EDGELMT);
	hi_isp_drc_strength_write(IspDev, DRC_STRLENGTH);

	hi_isp_drc_y_val1_write(IspDev, DRC_Y_VAL1);
	hi_isp_drc_y_sft1_write(IspDev, DRC_Y_SFT1);
	hi_isp_drc_y_val2_write(IspDev, DRC_Y_VAL2);
	hi_isp_drc_y_sft2_write(IspDev, DRC_Y_SFT2);

	hi_isp_drc_c_val1_write(IspDev, DRC_C_VAL1);
	hi_isp_drc_c_sft1_write(IspDev, DRC_C_SFT1);
	hi_isp_drc_c_val2_write(IspDev, DRC_C_VAL2);
	hi_isp_drc_c_sft2_write(IspDev, DRC_C_SFT2);

	hi_isp_drc_bright_gain_val_write(IspDev, DRC_B_VAL1);
	hi_isp_drc_bright_gain_sft_write(IspDev, DRC_B_SFT1);

	hi_isp_drc_lut_mix_ctrl_write(IspDev, LUT_MIX_CTRL);

	hi_isp_drc_bin_mix_coarse_write(IspDev, HI_ISP_DRC_BINMIXCOARSE_DEFAULT);
	//hi_isp_drc_bin_mix_medium_write(IspDev, HI_ISP_DRC_BINMIXMEDIUM_DEFAULT);
	hi_isp_drc_bin_mix_medium_write(IspDev, pstSnsDft->stDrc.u8SpatialVar);
	
	hi_isp_drc_var_sap_coarse_write(IspDev, HI_ISP_DRC_VARSAPCOARSE_DEFAULT);
	hi_isp_drc_var_spa_medium_write(IspDev, HI_ISP_DRC_VARSAPMEDIUM_DEFAULT);
	hi_isp_drc_var_spa_fine_write(IspDev, HI_ISP_DRC_VARSAPFINE_DEFAULT);
	
	//hi_isp_drc_var_rng_fine_write(IspDev, HI_ISP_DRC_VAR_RNG_FINE_DEFAULT);
	hi_isp_drc_var_rng_fine_write(IspDev, pstSnsDft->stDrc.u8RangeVar);
	hi_isp_drc_var_rng_medium_write(IspDev, HI_ISP_DRC_VAR_RNG_MEDIUM_DEFAULT);
	hi_isp_drc_var_rng_coarse_write(IspDev, HI_ISP_DRC_VAR_RNG_COARSE_DEFAULT);

	hi_isp_drc_grad_rev_shift_write(IspDev, HI_ISP_DRC_GRADREV_SHIFT_DEFAULT);
	hi_isp_drc_grad_rev_slope_write(IspDev, HI_ISP_DRC_GRADREV_SLOPE_DEFAULT);
	hi_isp_drc_grad_rev_max_write(IspDev, HI_ISP_DRC_GRADREV_MAX_DEFAULT);
	hi_isp_drc_grad_rev_thres_write(IspDev, HI_ISP_DRC_GRADREV_THRES_DEFAULT);

	hi_isp_drc_stat_ind_waddr2_write(IspDev, 0);

	for (i = 0; i < 33; i++)
	{    
		//hi_isp_drc_stat_ind_wdata2_write(IspDev, ColorCorrectionLut[i]);
		//hi_ext_system_drc_colorcc_lut_write(i, ColorCorrectionLut[i]);

		hi_isp_drc_stat_ind_wdata2_write(IspDev, pstSnsDft->stDrc.ColorCorrectionLut[i]);
		hi_ext_system_drc_colorcc_lut_write(i, pstSnsDft->stDrc.ColorCorrectionLut[i]);
	}
	
	//hi_ext_system_drc_colorcc_lut_up_en_write(HI_FALSE);

	hi_isp_drc_range_flt_coef_write(IspDev, RANGE_FLT_COEF);

	if (IS_BUILT_IN_WDR_MODE(u8WDRMode) || IS_FS_WDR_MODE(u8WDRMode))
	{
		hi_isp_drc_gain_clip_knee_write(IspDev, GAIN_CLIP_KNEE_WDR);
	}
	else
	{
		hi_isp_drc_gain_clip_knee_write(IspDev, GAIN_CLIP_KNEE);
	}

	hi_isp_drc_gain_clip_step_write(IspDev, GAIN_CLIP_STEP);

	hi_isp_drc_rgb_wgt_write(IspDev, HI_ISP_DRC_WEIGHT_R, HI_ISP_DRC_WEIGHT_G, HI_ISP_DRC_WEIGHT_B);
	hi_isp_drc_color_ctrl_write(IspDev, HI_ISP_DRC_CC_LUT_CTRL_DEFAULT, HI_ISP_DRC_CC_LIN_POW_DEFAULT);
	hi_isp_drc_cc_corr_write(IspDev, HI_ISP_DRC_GLOBAL_CC_CORR_DEFAULT);
	
	hi_isp_drc_mixing_coring_write(IspDev, HI_ISP_DRC_MIXING_CORING_DEFAULT);
	
	hi_isp_drc_mixing_dark_min_write(IspDev, HI_ISP_DRC_MIXING_DARK_MIN_DEFAULT);
	hi_isp_drc_mixing_dark_max_write(IspDev, HI_ISP_DRC_MIXING_DARK_MAX_DEFAULT);
	hi_isp_drc_mixing_dark_thr_write(IspDev, HI_ISP_DRC_MIXING_DARK_THR_DEFAULT);
	hi_isp_drc_mixing_dark_slo_write(IspDev, HI_ISP_DRC_MIXING_DARK_SLO_DEFAULT);

	hi_isp_drc_mixing_bright_min_write(IspDev, HI_ISP_DRC_MIXING_BRIGHT_MIN_DEFAULT);
	hi_isp_drc_mixing_bright_max_write(IspDev, HI_ISP_DRC_MIXING_BRIGHT_MAX_DEFAULT);
	hi_isp_drc_mixing_bright_thr_write(IspDev, HI_ISP_DRC_MIXING_BRIGHT_THR_DEFAULT);
	hi_isp_drc_mixing_bright_slo_write(IspDev, HI_ISP_DRC_MIXING_BRIGHT_SLO_DEFAULT);

	hi_isp_drc_rg_ctrl_write(IspDev, HI_ISP_DRC_RG_CTR_DEFAULT, HI_ISP_DRC_RG_WIDTH_DEFAULT, HI_ISP_DRC_RG_SLOPE_DEFAULT);
	hi_isp_drc_bg_ctrl_write(IspDev, HI_ISP_DRC_BG_THR_DEFAULT, HI_ISP_DRC_BG_SLO_DEFAULT);
	hi_isp_drc_pdw_low_write(IspDev, HI_ISP_DRC_PDW_LOW_THR_DEFAULT, HI_ISP_DRC_PDW_LOW_SLO_DEFAULT);
	hi_isp_drc_pdw_hig_write(IspDev, HI_ISP_DRC_PDW_HIG_THR_DEFAULT, HI_ISP_DRC_PDW_HIG_SLO_DEFAULT);
	
	DrcSetRegsVal(IspDev);    
	return;
}

static DRC_PARA_MODE_E WdrMode2DrcParaMode(WDR_MODE_E eWdrMode)
{
	switch (eWdrMode)
	{
		case WDR_MODE_NONE:
		case WDR_MODE_BUTT:
			return DRC_PARA_MODE_LINEAR;
		case WDR_MODE_BUILT_IN:
			return DRC_PARA_MODE_BUILT_IN;
		case WDR_MODE_2To1_LINE:
			return DRC_PARA_MODE_2To1_LINE;
		case WDR_MODE_3To1_LINE:
			return DRC_PARA_MODE_3To1_LINE;
		case WDR_MODE_4To1_LINE:
			return DRC_PARA_MODE_4To1_LINE;			
		default :
			return DRC_PARA_MODE_2To1_LINE;
	}
}

static HI_VOID DrcExtRegsInitialize(ISP_DEV IspDev,ISP_CMOS_DEFAULT_S *pstSnsDft)
{	
	HI_U8 i;
	ISP_DRC_S *pstDrc = HI_NULL;    	
	ISP_CTX_S *pstIspCtx = HI_NULL;   
	HI_U8 u8WDRMode;

	ISP_GET_CTX(IspDev, pstIspCtx);
	DRC_GET_CTX(IspDev, pstDrc);   
	
	u8WDRMode = pstIspCtx->u8SnsWDRMode;

  /*
	hi_ext_system_drc_asymmetry_write(g_DrcPara[eDrcParaMode].u8DrcAsymmetry);
	hi_ext_system_drc_secondpole_write(g_DrcPara[eDrcParaMode].u8DrcSecondpole);
	hi_ext_system_drc_stretch_write(g_DrcPara[eDrcParaMode].u8Stretch);
	hi_ext_system_drc_compress_write(g_DrcPara[eDrcParaMode].u16Compress);
 */

	hi_ext_system_drc_asymmetry_write(pstSnsDft->stDrc.u8Asymmetry);
	hi_ext_system_drc_secondpole_write(pstSnsDft->stDrc.u8SecondPole);
	hi_ext_system_drc_stretch_write(pstSnsDft->stDrc.u8Stretch);
	hi_ext_system_drc_compress_write(pstSnsDft->stDrc.u8Compress);
	hi_ext_system_drc_manual_mode_write(pstSnsDft->stDrc.bOpType);

	if (IS_BUILT_IN_WDR_MODE(u8WDRMode) || IS_FS_WDR_MODE(u8WDRMode))
	{
		hi_ext_system_drc_strength_target_write(DRC_STRENGTH_TARGET_WDR);
		hi_ext_system_drc_manual_strength_write(pstSnsDft->stDrc.u8ManualStrength);
		hi_ext_system_drc_auto_strength_write(pstSnsDft->stDrc.u8AutoStrength);
	}
	else
	{
		hi_ext_system_drc_strength_target_write(DRC_STRENGTH_TARGET_LINEAR);
		hi_ext_system_drc_manual_strength_write(pstSnsDft->stDrc.u8ManualStrength);
		hi_ext_system_drc_auto_strength_write(pstSnsDft->stDrc.u8AutoStrength);
	}

	hi_ext_system_drc_user_tm_enable_write(DRC_USER_TM_ENABLE_DEFAULT);
	hi_ext_system_drc_user_tm_update_write(HI_FALSE);

	for (i = 0; i < 200; i++)
	{
		hi_ext_system_drc_tm_lut_write(i, (HI_U16)(pstDrc->dLutValue1[200 - i] * 65535));
	}
	hi_ext_system_drc_mixing_bright_max_write(pstDrc->u8MixingBrightMax);
	
	hi_ext_system_drc_mixing_bright_min_write(pstSnsDft->stDrc.u8LocalMixingBrigtht);
	
	hi_ext_system_drc_mixing_dark_max_write(pstSnsDft->stDrc.u8LocalMixingDark);
	
	hi_ext_system_drc_mixing_dark_min_write(pstDrc->u8MixingDarkMin);
	hi_ext_system_drc_mixing_coring_write(pstDrc->u8MixingCoring);
	
	hi_ext_system_drc_enable_write(pstDrc->bEnable);
	hi_ext_system_drc_var_rng_fine_write(pstSnsDft->stDrc.u8RangeVar);
	hi_ext_system_drc_var_spa_fine_write(pstDrc->u8SpatialVarFine);
	hi_ext_system_drc_var_rng_medium_write(pstDrc->u8RangeVarMedium);
	hi_ext_system_drc_var_spa_medium_write(pstDrc->u8SpatialVarMedium);
	hi_ext_system_drc_var_rng_coarse_write(pstDrc->u8RangeVarCoarse);
	hi_ext_system_drc_var_spa_coarse_write(pstDrc->u8SpatialVarCoarse);
	hi_ext_system_drc_bin_mix_coarse_write(pstDrc->u8FltScaleFine);
	
	hi_ext_system_drc_bin_mix_medium_write(pstSnsDft->stDrc.u8SpatialVar);
	
	hi_ext_system_drc_pdw_high_write(pstSnsDft->stDrc.u8PDStrength);

	hi_ext_system_drc_auto_strength_max_write(pstDrc->u8AutoStrengthMax);
	hi_ext_system_drc_auto_strength_min_write(pstDrc->u8AutoStrengthMin);

	hi_ext_system_drc_manual_mode_write(HI_EXT_SYSTEM_DRC_MANUAL_MODE_DEFAULT);    
	hi_ext_system_drc_dark_gain_lmt_y_write(DRC_DARK_GAIN_LMT_Y_IDX_DEFAULT);
    hi_ext_system_drc_dark_gain_lmt_c_write(DRC_DARK_GAIN_LMT_C_IDX_DEFAULT);
    hi_ext_system_drc_bright_gain_lmt_write(DRC_BRIGHT_GAIN_LMT_IDX_DEFAULT);
    hi_ext_system_drc_gain_clip_knee_write(pstDrc->u8GainClipKnee);
    hi_ext_system_drc_gain_clip_step_write(pstDrc->u8GainClipStep);

	return;
}

static HI_VOID DrcRegsInitialize(ISP_DEV IspDev, ISP_CMOS_DEFAULT_S *pstSnsDft)
{   
	DRC_PARA_MODE_E eDrcParaMode;
	ISP_DRC_S *pstDrc = HI_NULL;
	HI_U32 u32Width = 0, u32Height = 0;

	DRC_GET_CTX(IspDev, pstDrc);

	ISP_CTX_S *pstIspCtx = HI_NULL;    
	HI_U8 u8WDRMode;
	ISP_GET_CTX(IspDev, pstIspCtx);

	u8WDRMode = pstIspCtx->u8SnsWDRMode;

	pstDrc->bManuMode = (HI_BOOL)hi_ext_system_drc_manual_mode_read();

	pstDrc->bUpdateLut    = HI_FALSE;	
	pstDrc->G1            = DRC_G1;
	pstDrc->F2            = DRC_F2;
	pstDrc->G2            = DRC_G2;
	pstDrc->K1            = DRC_K1;

	pstDrc->u8Asymmetry   = pstSnsDft->stDrc.u8Asymmetry;
	pstDrc->u8SecondPole  = pstSnsDft->stDrc.u8SecondPole;
	pstDrc->u8Stretch     = pstSnsDft->stDrc.u8Stretch;
	pstDrc->u16Compress   = pstSnsDft->stDrc.u8Compress;

	pstDrc->bTmLutSel     = 0;
	pstDrc->u8LutMixCtrl     = hi_isp_drc_lut_mix_ctrl_read(IspDev);
	pstDrc->u8LutMixCtrlPrev = pstDrc->u8LutMixCtrl;
	pstDrc->u8Strength    = hi_isp_drc_strength_read(IspDev);
	
	u32Width = hi_ext_sync_total_width_read();
	u32Height = hi_ext_sync_total_height_read();

	hi_isp_drc_width_write(IspDev, u32Width-1);
	hi_isp_drc_height_write(IspDev, u32Height-1);
			
	//hi_isp_drc_enable_write(IspDev, pstDrc->bEnable);
	hi_isp_drc_moudle_dither_en_write(IspDev, !(pstDrc->bEnable));	

	eDrcParaMode = WdrMode2DrcParaMode(u8WDRMode);

	pstDrc->u8BinNumZ = g_DrcPara[eDrcParaMode].u8DrcBinNumZ;
	hi_isp_drc_bin_write(IspDev, pstDrc->u8BinNumZ);

	if (IS_BUILT_IN_WDR_MODE(u8WDRMode) || IS_FS_WDR_MODE(u8WDRMode))
	{	
		pstDrc->bEnable       = HI_TRUE;
		pstDrc->bDrcDitherEn  = (pstDrc->bEnable) ? HI_FALSE : HI_TRUE;

		hi_ext_system_drc_enable_write(pstDrc->bEnable);
		hi_isp_drc_moudle_dither_en_write(IspDev, pstDrc->bDrcDitherEn);

		pstDrc->u8AutoStrengthMax = DRC_AUTO_STRENGTH_MAX_WDR;
		pstDrc->u8AutoStrengthMin = DRC_AUTO_STRENGTH_MIN_WDR;	
        pstDrc->u8GainClipKnee = GAIN_CLIP_KNEE_WDR;
	}
	else
	{
		pstDrc->bEnable       = HI_FALSE;
		pstDrc->bDrcDitherEn  = (pstDrc->bEnable) ? HI_FALSE : HI_TRUE;

		hi_ext_system_drc_enable_write(pstDrc->bEnable);
		hi_isp_drc_moudle_dither_en_write(IspDev, pstDrc->bDrcDitherEn);
		
		pstDrc->u8AutoStrengthMax = DRC_AUTO_STRENGTH_MAX;
		pstDrc->u8AutoStrengthMin = DRC_AUTO_STRENGTH_MIN;	
        pstDrc->u8GainClipKnee = GAIN_CLIP_KNEE;
	}

	pstDrc->u8GainClipStep = GAIN_CLIP_STEP;

	hi_isp_drc_enable_write(IspDev, pstDrc->bEnable);
	pstDrc->u8MixingBrightMax = HI_ISP_DRC_MIXING_BRIGHT_MAX_DEFAULT;
	pstDrc->u8MixingBrightMin = HI_ISP_DRC_MIXING_BRIGHT_MIN_DEFAULT;
	pstDrc->u8MixingDarkMax   = HI_ISP_DRC_MIXING_DARK_MAX_DEFAULT;
	pstDrc->u8MixingDarkMin   = HI_ISP_DRC_MIXING_DARK_MIN_DEFAULT;
	pstDrc->u8MixingCoring    = HI_ISP_DRC_MIXING_CORING_DEFAULT;
	pstDrc->u8RangeVarFine = HI_ISP_DRC_VARSAPFINE_DEFAULT;
	pstDrc->u8SpatialVarFine = HI_ISP_DRC_VAR_RNG_FINE_DEFAULT;
	pstDrc->u8RangeVarMedium = HI_ISP_DRC_VARSAPMEDIUM_DEFAULT;
	pstDrc->u8SpatialVarMedium = HI_ISP_DRC_VAR_RNG_MEDIUM_DEFAULT;
	pstDrc->u8RangeVarCoarse = HI_ISP_DRC_VARSAPCOARSE_DEFAULT;
	pstDrc->u8SpatialVarCoarse = HI_ISP_DRC_VAR_RNG_COARSE_DEFAULT;
	pstDrc->u8FltScaleFine = HI_ISP_DRC_BINMIXCOARSE_DEFAULT;
	pstDrc->u8FltScaleCoarse = HI_ISP_DRC_BINMIXMEDIUM_DEFAULT;
	pstDrc->u8PdwHighStr      = HI_ISP_DRC_PDW_HIG_THR_DEFAULT;

    pstDrc->u16BrightGainLmt = au16BrightGainLmt[DRC_BRIGHT_GAIN_LMT_IDX_DEFAULT];
    pstDrc->u16DarkGainLmtC  = au16DarkGainLmt[DRC_DARK_GAIN_LMT_C_IDX_DEFAULT];
    pstDrc->u16DarkGainLmtY  = au16DarkGainLmt[DRC_DARK_GAIN_LMT_Y_IDX_DEFAULT];

	return;
}

HI_VOID DrcHistDarkWeightDef(DRC_HIST_WEIGHT_CALC_S *pstWeightCalc, HI_U8 u8WDRMode)
{

	if(IS_LINEAR_MODE(u8WDRMode))
	{
		 pstWeightCalc->au16IndexTh[0] = 0;
		 pstWeightCalc->au16IndexTh[1] = 4;
		 pstWeightCalc->au16IndexTh[2] = 8;
		 pstWeightCalc->au16IndexTh[3] = 16;	
		 pstWeightCalc->au16IndexTh[4] = 32;
		 pstWeightCalc->au16IndexTh[5] = 64;
		 pstWeightCalc->au16IndexTh[6] = 255;

		 pstWeightCalc->au16Weight[0] = 128;
		 pstWeightCalc->au16Weight[1] = 64;
		 pstWeightCalc->au16Weight[2] = 32;
		 pstWeightCalc->au16Weight[3] = 16;	
		 pstWeightCalc->au16Weight[4] = 4;
		 pstWeightCalc->au16Weight[5] = 0;
		 pstWeightCalc->au16Weight[6] = 0;
	}
	else if(IS_BUILT_IN_WDR_MODE(u8WDRMode)|| IS_FS_WDR_MODE(u8WDRMode))
	{
		 pstWeightCalc->au16IndexTh[0] = 0;
		 pstWeightCalc->au16IndexTh[1] = 4;
		 pstWeightCalc->au16IndexTh[2] = 8;
		 pstWeightCalc->au16IndexTh[3] = 16;	
		 pstWeightCalc->au16IndexTh[4] = 32;
		 pstWeightCalc->au16IndexTh[5] = 64;
		 pstWeightCalc->au16IndexTh[6] = 255;

		 pstWeightCalc->au16Weight[0] = 64;
		 pstWeightCalc->au16Weight[1] = 32;
		 pstWeightCalc->au16Weight[2] = 16;
		 pstWeightCalc->au16Weight[3] = 4;	
		 pstWeightCalc->au16Weight[4] = 0;
		 pstWeightCalc->au16Weight[5] = 0;
		 pstWeightCalc->au16Weight[6] = 0;
	}
	else
	{
	}
	 return;
}

static __inline HI_S16 DrcHistDarkWeightCalc(DRC_HIST_WEIGHT_CALC_S *pstWeightCalc, HI_S16 s16HistIndex)
{
	HI_S16 as16HistIndexTh[7] = {0};
	HI_S16 as16HistIndexSlope[6] = {0};
	HI_S16 s16Weight;
	HI_S16 as16HistWeight[7] = {0};

	as16HistIndexTh[0] = pstWeightCalc->au16IndexTh[0];
	as16HistIndexTh[1] = pstWeightCalc->au16IndexTh[1];
	as16HistIndexTh[2] = pstWeightCalc->au16IndexTh[2];
	as16HistIndexTh[3] = pstWeightCalc->au16IndexTh[3];
	as16HistIndexTh[4] = pstWeightCalc->au16IndexTh[4];
	as16HistIndexTh[5] = pstWeightCalc->au16IndexTh[5];
	as16HistIndexTh[6] = pstWeightCalc->au16IndexTh[6];
	as16HistWeight[0]  = pstWeightCalc->au16Weight[0];
	as16HistWeight[1]  = pstWeightCalc->au16Weight[1];
	as16HistWeight[2]  = pstWeightCalc->au16Weight[2];
	as16HistWeight[3]  = pstWeightCalc->au16Weight[3];
	as16HistWeight[4]  = pstWeightCalc->au16Weight[4];
	as16HistWeight[5]  = pstWeightCalc->au16Weight[5];
	as16HistWeight[6]  = pstWeightCalc->au16Weight[6];

	as16HistIndexSlope[0] = (as16HistWeight[1] - as16HistWeight[0]) * 32
		/ DIV_0_TO_1(as16HistIndexTh[1] - as16HistIndexTh[0]);
	as16HistIndexSlope[1] = (as16HistWeight[2] - as16HistWeight[1]) * 32
		/ DIV_0_TO_1(as16HistIndexTh[2] - as16HistIndexTh[1]);
	as16HistIndexSlope[2] = (as16HistWeight[3] - as16HistWeight[2]) * 32
		/ DIV_0_TO_1(as16HistIndexTh[3] - as16HistIndexTh[2]);
	as16HistIndexSlope[3] = (as16HistWeight[4] - as16HistWeight[3]) * 32
		/ DIV_0_TO_1(as16HistIndexTh[4] - as16HistIndexTh[3]);
	as16HistIndexSlope[4] = (as16HistWeight[5] - as16HistWeight[4]) * 32
		/ DIV_0_TO_1(as16HistIndexTh[5] - as16HistIndexTh[4]);
	as16HistIndexSlope[5] = (as16HistWeight[6] - as16HistWeight[5]) * 32
		/ DIV_0_TO_1(as16HistIndexTh[6] - as16HistIndexTh[5]);

	if (s16HistIndex < as16HistIndexTh[0])
	{
		s16Weight = as16HistWeight[0];	
	}
	else if ((s16HistIndex >= as16HistIndexTh[0]) && (s16HistIndex < as16HistIndexTh[1]))
	{
		s16Weight = as16HistWeight[0] + ((as16HistIndexSlope[0] * (s16HistIndex - as16HistIndexTh[0])) / 32);	
	}
	else if ((s16HistIndex >= as16HistIndexTh[1]) && (s16HistIndex < as16HistIndexTh[2]))
	{
		s16Weight = as16HistWeight[1] + ((as16HistIndexSlope[1] * (s16HistIndex - as16HistIndexTh[1])) / 32);
	}
	else if ((s16HistIndex >= as16HistIndexTh[2]) && (s16HistIndex < as16HistIndexTh[3]))
	{
		s16Weight = as16HistWeight[2] + ((as16HistIndexSlope[2] * (s16HistIndex - as16HistIndexTh[2])) / 32);
	}
	else if ((s16HistIndex >= as16HistIndexTh[3]) && (s16HistIndex < as16HistIndexTh[4]))
	{
		s16Weight = as16HistWeight[3] + ((as16HistIndexSlope[3] * (s16HistIndex - as16HistIndexTh[3])) / 32);
	}
	else if ((s16HistIndex >= as16HistIndexTh[4]) && (s16HistIndex < as16HistIndexTh[5]))
	{
		s16Weight = as16HistWeight[4] + ((as16HistIndexSlope[4] * (s16HistIndex - as16HistIndexTh[4])) / 32);
	}
	else if ((s16HistIndex >= as16HistIndexTh[5]) && (s16HistIndex < as16HistIndexTh[6]))
	{
		s16Weight = as16HistWeight[5] + ((as16HistIndexSlope[5] * (s16HistIndex - as16HistIndexTh[5])) / 32);
	}
	else
	{
		s16Weight = as16HistWeight[6];
	}

	s16Weight = MAX2(MIN2(s16Weight, 128), 0);

	return s16Weight;
}


static HI_U32  DrcCalHist(ISP_DRC_HIST_S *pstHist, ISP_STAT_S *pStatInfo, HI_U8 u8SnsWDRMode)
{
	HI_S32 i;
	HI_U32 u32ExpRatio;
	HI_U32 u32Stat3HistAcc = pStatInfo->stAeStat3.u32PixelWeight;
	HI_U32 u32HistDark   = 0;
		
	/* calculate histdark */
	
	DrcHistDarkWeightDef(&pstHist->pstHistWeightCalc, u8SnsWDRMode);
	
	for (i = 0; i < 256; i++)
	{
		//u32HistDark  += (DrcHistDarkWeightCalc(&pstHist->pstHistWeightCalc, i) * pStatInfo->stAeStat3.au32HistogramMemArray[i] + 8) >> 4;		
		u32HistDark += (DrcHistDarkWeightCalc(&pstHist->pstHistWeightCalc, i) * (pStatInfo->stAeStat3.au32HistogramMemArray[4*i] \
					+ pStatInfo->stAeStat3.au32HistogramMemArray[4*i + 1] + pStatInfo->stAeStat3.au32HistogramMemArray[4*i + 2] + \
					pStatInfo->stAeStat3.au32HistogramMemArray[4*i + 3]) + 8) >> 4;
	}

	pstHist->s32HistDark = u32HistDark;

	


	/*calculate exposure_ratio for drc*/
	 u32ExpRatio = (0X40 * ( pstHist->s32HistDark ) + u32Stat3HistAcc/2) / DIV_0_TO_1(u32Stat3HistAcc);/* Exposure ratio modulation: */
	
	 if (u32ExpRatio > 0x40)
	 {
		 u32ExpRatio = 0x40;
	 }
	 pstHist->u16ExpRatio = u32ExpRatio;
	
	return HI_SUCCESS;
}



HI_S32 DrcStrengthCalc_BInWdr(ISP_DRC_S *pstDrc, HI_U32 u32Iso,HI_U8 u8SnsWDRMode, ISP_STAT_S *pStatInfo)
{
	HI_U32 u32Strength = 0;
	
	DrcCalHist(&pstDrc->stDrcHist, pStatInfo, u8SnsWDRMode);

	/* Limit Drc strength for well balanced scenes */
	if(IS_LINEAR_MODE(u8SnsWDRMode))
	{
		u32Strength = pstDrc->stDrcHist.u16ExpRatio;

		u32Strength  = (u32Strength * pstDrc->u8StrengthBias) >> 7;       
		
		if (u32Strength > pstDrc->u8Strength)
		{
			pstDrc->u8Strength = pstDrc->u8Strength + 1;
		}
		else if (u32Strength < pstDrc->u8Strength)
		{
			pstDrc->u8Strength = pstDrc->u8Strength - 1;
		}

		if (pstDrc->u8Strength > pstDrc->u8StrengthTarget)
		{
			pstDrc->u8Strength = pstDrc->u8StrengthTarget;
		}

		pstDrc->u8Strength =  MAX2(MIN2(pstDrc->u8Strength, pstDrc->u8AutoStrengthMax), pstDrc->u8AutoStrengthMin);

		if(pstDrc->u8Strength == 0)
		{
			pstDrc->u8MixingBrightMax = 0;
			pstDrc->u8MixingBrightMin = 0;
			pstDrc->u8MixingDarkMax = 0;
			pstDrc->u8MixingDarkMin = 0;
			pstDrc->u8MixingCoring = 0;
		}
	}
	else if(IS_BUILT_IN_WDR_MODE(u8SnsWDRMode))
	{   
		u32Strength = (105220000 + 679400 * pstDrc->stDrcHist.u16ExpRatio  + 500 * pstDrc->stDrcHist.u16ExpRatio * pstDrc->stDrcHist.u16ExpRatio - 3 * pstDrc->stDrcHist.u16ExpRatio * pstDrc->stDrcHist.u16ExpRatio * pstDrc->stDrcHist.u16ExpRatio + 500000) / 1000000;

		u32Strength  = (u32Strength * pstDrc->u8StrengthBias) >> 7;       
		
		if (u32Strength > pstDrc->u8LutMixCtrl)
		{
			pstDrc->u8LutMixCtrl = pstDrc->u8LutMixCtrl + 1;
		}
		else if (u32Strength < pstDrc->u8LutMixCtrl)
		{
			pstDrc->u8LutMixCtrl = pstDrc->u8LutMixCtrl - 1;
		}

		if (pstDrc->u8LutMixCtrl > pstDrc->u8StrengthTarget)
		{
			pstDrc->u8LutMixCtrl = pstDrc->u8StrengthTarget;
		}

		pstDrc->u8LutMixCtrl =  MAX2(MIN2(pstDrc->u8LutMixCtrl, pstDrc->u8AutoStrengthMax), pstDrc->u8AutoStrengthMin);
	}

	return HI_SUCCESS;
}




static HI_S32 DrcReadExtregs(ISP_DEV IspDev)
{   
	ISP_DRC_S *pstDrc = HI_NULL;

	DRC_GET_CTX(IspDev, pstDrc);

	// Update Lut when user changes the type of tone mapping curve
	if (pstDrc->bUserTmEnable != (HI_BOOL) hi_ext_system_drc_user_tm_enable_read())
	{
		pstDrc->bUpdateLut = HI_TRUE;
		pstDrc->bUserTmEnable = (HI_BOOL) hi_ext_system_drc_user_tm_enable_read();
	}
	else
	{
		if (pstDrc->bUserTmEnable)
		{
			pstDrc->bUpdateLut = hi_ext_system_drc_user_tm_update_read();
		}
		else
		{
			if (pstDrc->u8Asymmetry     != hi_ext_system_drc_asymmetry_read()
				|| pstDrc->u8SecondPole != hi_ext_system_drc_secondpole_read()
				|| pstDrc->u8Stretch    != hi_ext_system_drc_stretch_read()
				|| pstDrc->u16Compress   != hi_ext_system_drc_compress_read())
			{
				pstDrc->u8Asymmetry  = hi_ext_system_drc_asymmetry_read();
				pstDrc->u8SecondPole = hi_ext_system_drc_secondpole_read();		
				pstDrc->u8Stretch    = hi_ext_system_drc_stretch_read();  
				pstDrc->u16Compress   = hi_ext_system_drc_compress_read();
				pstDrc->bUpdateLut   = HI_TRUE;	
			}
			else
			{
				pstDrc->bUpdateLut = HI_FALSE;
			}	
		}
	}

    pstDrc->u16DarkGainLmtC  = au16DarkGainLmt[hi_ext_system_drc_dark_gain_lmt_c_read()];
    pstDrc->u16DarkGainLmtY  = au16DarkGainLmt[hi_ext_system_drc_dark_gain_lmt_y_read()];
    pstDrc->u16BrightGainLmt = au16BrightGainLmt[hi_ext_system_drc_bright_gain_lmt_read()];
    pstDrc->u8GainClipKnee = hi_ext_system_drc_gain_clip_knee_read();
    pstDrc->u8GainClipStep = hi_ext_system_drc_gain_clip_step_read();

	pstDrc->u8MixingBrightMax = hi_ext_system_drc_mixing_bright_max_read();
	pstDrc->u8MixingBrightMin = hi_ext_system_drc_mixing_bright_min_read();
	pstDrc->u8MixingDarkMax = hi_ext_system_drc_mixing_dark_max_read();
	pstDrc->u8MixingDarkMin = hi_ext_system_drc_mixing_dark_min_read();
	pstDrc->u8MixingCoring  = hi_ext_system_drc_mixing_coring_read();
	pstDrc->u8RangeVarFine      = hi_ext_system_drc_var_rng_fine_read();
	pstDrc->u8SpatialVarFine    = hi_ext_system_drc_var_spa_fine_read();
	pstDrc->u8RangeVarMedium    = hi_ext_system_drc_var_rng_medium_read();
	pstDrc->u8SpatialVarMedium  = hi_ext_system_drc_var_spa_medium_read();
	pstDrc->u8RangeVarCoarse    = hi_ext_system_drc_var_rng_coarse_read();
	pstDrc->u8SpatialVarCoarse  = hi_ext_system_drc_var_spa_coarse_read();
	pstDrc->u8FltScaleFine    = hi_ext_system_drc_bin_mix_coarse_read();
	pstDrc->u8FltScaleCoarse  = hi_ext_system_drc_bin_mix_medium_read();
	pstDrc->bEnable       = hi_ext_system_drc_enable_read();

	pstDrc->u8PdwHighManual = hi_ext_system_drc_pdw_high_read();
	pstDrc->bManuMode = (HI_BOOL)hi_ext_system_drc_manual_mode_read();

	pstDrc->u8AutoStrengthMax = hi_ext_system_drc_auto_strength_max_read();
	pstDrc->u8AutoStrengthMin = hi_ext_system_drc_auto_strength_min_read();

	return 0;
}

HI_S32 DrcProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
	ISP_CTRL_PROC_WRITE_S stProcTmp;
	ISP_DRC_S *pstDrc = HI_NULL;
	ISP_CTX_S *pstIspCtx   = HI_NULL;  

	DRC_GET_CTX(IspDev, pstDrc);
	ISP_GET_CTX(IspDev, pstIspCtx);
	HI_U8 u8WDRMode = pstIspCtx->u8SnsWDRMode;
 
	if ((HI_NULL == pstProc->pcProcBuff)
		|| (0 == pstProc->u32BuffLen))
	{
		return HI_FAILURE;
	}

	stProcTmp.pcProcBuff = pstProc->pcProcBuff;
	stProcTmp.u32BuffLen = pstProc->u32BuffLen;
	
	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
		"-----DRC INFO------------------------------------------------------------------\n");
		   
	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
		"%16s" "%16s"     "%16s\n", 
		"En", "ManuEn", "Strength");
    if (IS_FS_WDR_MODE(u8WDRMode))
    {
	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
		"%16d" "%16d"    "%16d\n",
	   hi_isp_drc_enable_read(IspDev),
		pstDrc->bManuMode,
		pstDrc->u8LutMixCtrl);
	}
	else
	{
	       ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
    		"%16d" "%16d"    "%16d\n",
    	   hi_isp_drc_enable_read(IspDev),
    		pstDrc->bManuMode,
    		pstDrc->u8Strength);
	}
	pstProc->u32WriteLen += 1;
	
	return HI_SUCCESS;
}

HI_S32 ISP_DrcInit(ISP_DEV IspDev)
{	
	ISP_DRC_S *pstDrc = HI_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;

	DRC_GET_CTX(IspDev, pstDrc);
	ISP_SensorGetDefault(IspDev, &pstSnsDft);
	
	DrcRegsDefault(IspDev,pstSnsDft);
	DrcRegsInitialize(IspDev,pstSnsDft);
	DrcUpdateAsyLUT(pstDrc);
	DrcUpdateMixedLut(pstDrc);
	DrcExtRegsInitialize(IspDev,pstSnsDft);

	if (0 == pstDrc->bTmLutSel)
	{
		hi_isp_drc_lut_mix_ctrl_write(IspDev, 0); // use Lut0
	}
	else
	{
		hi_isp_drc_lut_mix_ctrl_write(IspDev, 255); // use Lut1
	}

	pstDrc->bInit = HI_TRUE;
	
	return HI_SUCCESS;
}

HI_S32 DrcStrengthCalc_FsWdr(ISP_DEV IspDev)
{
//ratio range 64~1023

	ISP_DRC_S *pstDrc = HI_NULL;

	DRC_GET_CTX(IspDev, pstDrc);

	HI_U8 Strength;
	HI_U8 static preLutMix = 128;
	HI_U8 StrengthLut[120] = {
		128,   133,   139,   145,   150,   156,   161,   166,   171,   176,   180,
		183,   186,   188,   190,   190,   190,   190,   190,   190,   190,   190, 
		190,   190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
		190,   190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
		190,   190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
		190,   190,   190,   190,   190,   190,   191,   191,   191,   191,   192,
		192,   193,   193,   194,   194,   195,   195,   196,   197,   197,   198,
		199,   200,   201,   201,   202,   203,   204,   205,   206,   207,   208,
		209,   211,   212,   213,   214,   215,   217,   218,   219,   221,   222,
		223,   225,   226,   227,   229,   230,   232,   233,   235,   236,   238,
		239,   241,   242,   244,   246,   247,   249,   250,   252,   255
		};
	HI_U8 Shift = 3;
	HI_U16 Ratio,RatioTemp;

	Ratio = hi_isp_wdr_exporatio0_read(IspDev);
	Ratio = MIN2(MAX2(Ratio,64),1023);

	if(Ratio < 512)
	{ 
		//pstDrc->u8PdwHighStr = ((Ratio - 64)*HI_ISP_DRC_PDW_HIG_THR_DEFAULT + 224)/(512-64);

		pstDrc->u8PdwHighStr = ((Ratio - 64)*pstDrc->u8PdwHighManual + 224)/(512-64);
	}
	else
	{
		//pstDrc->u8PdwHighStr = HI_ISP_DRC_PDW_HIG_THR_DEFAULT;
		
		pstDrc->u8PdwHighStr = pstDrc->u8PdwHighManual;
	}



	RatioTemp = CLIP3((((Ratio-64)>>Shift) - 1),0,118);

	Strength = CLIP3(StrengthLut[RatioTemp] +(((StrengthLut[RatioTemp+1] - StrengthLut[RatioTemp])*(Ratio - ((Ratio>>Shift)<<Shift)))>>Shift),0,255);

	//pstDrc->u8LutMixCtrl = Strength;

	if((Strength - preLutMix) > 5)
		preLutMix = preLutMix + 5;
	else if((preLutMix - Strength) > 5)
		preLutMix = preLutMix - 5;
	else
		preLutMix = Strength;

	
	
	// pstDrc->u8LutMixCtrl = MIN2(((HI_U32)(preLutMix*pstDrc->u8StrengthBias)>>7),255);
	pstDrc->u8LutMixCtrl = MAX2(MIN2(((HI_U32)(preLutMix*pstDrc->u8StrengthBias)>>7), pstDrc->u8AutoStrengthMax), pstDrc->u8AutoStrengthMin);
	
	return HI_SUCCESS;
}


HI_S32 ISP_DrcRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
	HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
	ISP_DRC_REG_CFG_S *pstDrcRegCfg;
	ISP_DRC_S *pstDrc = HI_NULL;
	ISP_CTX_S *pstIspCtx   = HI_NULL;    
	
	DRC_GET_CTX(IspDev, pstDrc);
	ISP_GET_CTX(IspDev, pstIspCtx);
	HI_U8 u8WDRMode = pstIspCtx->u8SnsWDRMode;
	HI_U8 i;
	HI_U16 u16CcLutVal;
	static HI_U32 u32FrameCnt = 0;
	if (!pstDrc->bInit)
	{
		return HI_SUCCESS;
	}

	if (pstIspCtx->stLinkage.bDefectPixel)
	{
		return HI_SUCCESS;
	}
	
#if 0
	/* calculate every two interrupts */
	if (0 != pstIspCtx->u32FrameCnt % 2)
	{
		return HI_SUCCESS;
	}   
#endif
	
	DrcReadExtregs(IspDev);
	if((0 ==  hi_ext_system_drc_enable_read())&&(0 == hi_isp_drc_enable_read(IspDev)))
	{
		u32FrameCnt = 0;
		pstDrc->u8Strength = 128;
		pstDrc->bEnable = 0;
		return HI_SUCCESS;
	}
	
	// if (pstDrc->bUpdateLut)
	// {
	// 	DrcUpdateAsyLUT(pstDrc);
	// 	pstDrc->bUpdateLut = HI_FALSE;
	// 	if (pstDrc->bUserTmEnable)
	// 	{
	// 		hi_ext_system_drc_user_tm_update_write(HI_FALSE);
	// 	}
	// }
	

	//DrcSetRegsVal(IspDev);

	hi_isp_drc_y_val1_write(IspDev, pstDrc->u16DarkGainLmtY&0X7F);
	hi_isp_drc_y_sft1_write(IspDev, (pstDrc->u16DarkGainLmtY>>7)&0X7);
	hi_isp_drc_y_val2_write(IspDev, (pstDrc->u16DarkGainLmtY>>10)&0XF);
	hi_isp_drc_y_sft2_write(IspDev, (pstDrc->u16DarkGainLmtY>>14)&0X3);

	hi_isp_drc_c_val1_write(IspDev, (pstDrc->u16DarkGainLmtC)&0X7F);
	hi_isp_drc_c_sft1_write(IspDev, (pstDrc->u16DarkGainLmtC>>7)&0X7);
	hi_isp_drc_c_val2_write(IspDev, (pstDrc->u16DarkGainLmtC>>10)&0XF);
	hi_isp_drc_c_sft2_write(IspDev, (pstDrc->u16DarkGainLmtC>>14)&0X3);

	hi_isp_drc_bright_gain_val_write(IspDev, pstDrc->u16BrightGainLmt&0X7F);
	hi_isp_drc_bright_gain_sft_write(IspDev, (pstDrc->u16BrightGainLmt>>7)&0X7);

    hi_isp_drc_gain_clip_knee_write(IspDev, pstDrc->u8GainClipKnee);
    hi_isp_drc_gain_clip_step_write(IspDev, pstDrc->u8GainClipStep);

	pstDrc->u8LutMixCtrlPrev = pstDrc->u8LutMixCtrl; // save current mixing factor
	if (OP_TYPE_AUTO == (ISP_OP_TYPE_E)pstDrc->bManuMode)
	{   
		if (IS_FS_WDR_MODE(u8WDRMode))
		{
			pstDrc->u8StrengthBias = hi_ext_system_drc_auto_strength_read();
			DrcStrengthCalc_FsWdr(IspDev);			
			//hi_ext_system_drc_auto_strength_write(pstDrc->u8LutMixCtrl);
		}
		else
		{
			pstDrc->u8StrengthTarget = hi_ext_system_drc_strength_target_read();
			pstDrc->u8StrengthBias  = hi_ext_system_drc_auto_strength_read();			
			DrcStrengthCalc_BInWdr(pstDrc, pstIspCtx->stLinkage.u32Iso, u8WDRMode ,(ISP_STAT_S *)pStatInfo);			
			pstDrc->u8PdwHighStr = pstDrc->u8PdwHighManual;           
		}
	}
	else//for manual		
	{
		if (IS_BUILT_IN_WDR_MODE(u8WDRMode)|| IS_FS_WDR_MODE(u8WDRMode))
		{
			pstDrc->u8LutMixCtrl = hi_ext_system_drc_manual_strength_read();
		}
		else
		{
			pstDrc->u8Strength = hi_ext_system_drc_manual_strength_read();
		}
		pstDrc->u8PdwHighStr = pstDrc->u8PdwHighManual;
	}

	if (pstDrc->bUpdateLut)
	{
		DrcUpdateAsyLUT(pstDrc);
		DrcUpdateMixedLut(pstDrc);
		pstDrc->bUpdateLut = HI_FALSE;
		if (pstDrc->bUserTmEnable)
		{
			hi_ext_system_drc_user_tm_update_write(HI_FALSE);
		}
	}
	else if (pstDrc->u8LutMixCtrl != pstDrc->u8LutMixCtrlPrev)
	{
		if (IS_BUILT_IN_WDR_MODE(u8WDRMode)|| IS_FS_WDR_MODE(u8WDRMode))
		{
			DrcUpdateMixedLut(pstDrc);
		}
	}

    // if (IS_BUILT_IN_WDR_MODE(u8WDRMode)|| IS_FS_WDR_MODE(u8WDRMode))
    // {
    //     DrcUpdateMixedLut(pstDrc);
    // }
	
	if (0 == pstDrc->bTmLutSel)
	{
		hi_isp_drc_lut_mix_ctrl_write(IspDev, 0); // use Lut0
	}
	else
	{
		hi_isp_drc_lut_mix_ctrl_write(IspDev, 255); // use Lut1
	}

	if (!(IS_BUILT_IN_WDR_MODE(u8WDRMode) || IS_FS_WDR_MODE(u8WDRMode)))
	{
		if(u32FrameCnt < 1)
		{ 
			u32FrameCnt++;
			hi_isp_drc_strength_write(IspDev,0);
		}
		else
		{
			hi_isp_drc_strength_write(IspDev, pstDrc->u8Strength);
		}
		hi_ext_system_drc_actual_strength_write(pstDrc->u8Strength);
	}
	else
	{
		hi_ext_system_drc_actual_strength_write(pstDrc->u8LutMixCtrl);
	}

	if (hi_ext_system_drc_enable_read())
	{
		hi_ext_system_drc_enable_write(1);
		pstDrc->bEnable = 1;
		hi_isp_drc_enable_write(IspDev, HI_TRUE);	
		hi_isp_drc_moudle_dither_en_write(IspDev, HI_FALSE);
	}
	else
	{
		hi_ext_system_drc_enable_write(0);
		pstDrc->bEnable = 0;	
		hi_isp_drc_enable_write(IspDev, HI_FALSE);	
		hi_isp_drc_moudle_dither_en_write(IspDev, HI_TRUE);
	}

		
	hi_isp_drc_stat_ind_waddr2_write(IspDev, 0);		

	for (i = 0; i < 33; i++)
	{    
		u16CcLutVal = hi_ext_system_drc_colorcc_lut_read(i);
		hi_isp_drc_stat_ind_wdata2_write(IspDev, u16CcLutVal);
		
	}		

	pstDrcRegCfg = &(((ISP_REG_CFG_S *)pRegCfg)->stDrcRegCfg);

	pstDrcRegCfg->u8MixingBrightMax =  pstDrc->u8MixingBrightMax;
	pstDrcRegCfg->u8MixingBrightMin =  pstDrc->u8MixingBrightMin;
	pstDrcRegCfg->u8MixingDarkMax =  pstDrc->u8MixingDarkMax;
	pstDrcRegCfg->u8MixingDarkMin =  pstDrc->u8MixingDarkMin;
	pstDrcRegCfg->u8MixingCoring  =  pstDrc->u8MixingCoring;
	pstDrcRegCfg->u8PdwHighStr    =  pstDrc->u8PdwHighStr;
	pstDrcRegCfg->u8Enable        =  pstDrc->bEnable;
	pstDrcRegCfg->u8RangeVarFine      =  pstDrc->u8RangeVarFine;
	pstDrcRegCfg->u8SpatialVarFine    =  pstDrc->u8SpatialVarFine;
	pstDrcRegCfg->u8RangeVarMedium      =  pstDrc->u8RangeVarMedium;
	pstDrcRegCfg->u8SpatialVarMedium    =  pstDrc->u8SpatialVarMedium;
	pstDrcRegCfg->u8RangeVarCoarse      =  pstDrc->u8RangeVarCoarse;
	pstDrcRegCfg->u8SpatialVarCoarse    =  pstDrc->u8SpatialVarCoarse;
	pstDrcRegCfg->u8FltScaleFine = pstDrc->u8FltScaleFine;
	pstDrcRegCfg->u8FltScaleCoarse = pstDrc->u8FltScaleCoarse;
	
	pstDrcRegCfg->FrameExposure = pstIspCtx->stLinkage.u32Inttime;

	((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1DrcCfg = 1;

	return HI_SUCCESS;
}



static __inline HI_S32 DrcImageResWrite(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstRes)
{

	hi_isp_drc_width_write(IspDev, pstRes->u16Width-1);
	hi_isp_drc_height_write(IspDev,  pstRes->u16Height-1);

	return HI_SUCCESS;
}


HI_S32 ISP_DrcCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{   
	ISP_DRC_S *pstDrc = HI_NULL;
	
	switch (u32Cmd)
	{
		case ISP_WDR_MODE_SET : 
			DRC_GET_CTX(IspDev, pstDrc);
			pstDrc->bInit = HI_FALSE;
			ISP_DrcInit(IspDev);
			break;
		case ISP_PROC_WRITE:
			DrcProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
			break;
		case ISP_CHANGE_IMAGE_MODE_SET:   
			DrcImageResWrite(IspDev, (ISP_CMOS_SENSOR_IMAGE_MODE_S *)pValue);  
			break;
		default :
			break;
	}
	
	return HI_SUCCESS;
}

HI_S32 ISP_DrcExit(ISP_DEV IspDev)
{
	hi_ext_system_drc_enable_write(0);
	hi_isp_drc_enable_write(IspDev, HI_FALSE);	
	return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterDrc(ISP_DEV IspDev)
{
	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_ALG_NODE_S *pstAlgs = HI_NULL;
	
	ISP_GET_CTX(IspDev, pstIspCtx);

	pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
	ISP_CHECK_POINTER(pstAlgs);

	pstAlgs->enAlgType = ISP_ALG_DRC;
	pstAlgs->stAlgFunc.pfn_alg_init = ISP_DrcInit;
	pstAlgs->stAlgFunc.pfn_alg_run  = ISP_DrcRun;
	pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_DrcCtrl;
	pstAlgs->stAlgFunc.pfn_alg_exit = ISP_DrcExit;
	pstAlgs->bUsed = HI_TRUE;

	return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

