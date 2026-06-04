/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <stdio.h>
#include "sensor_common.h"
#include "ot_mpi_isp.h"
#include "ot_mpi_ae.h"
#include "ot_mpi_awb.h"

#include "sc431hai_cmos.h"
#include "sc431hai_cmos_param.h"
#include "sc431hai_sensor_ctrl.h"

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
static cis_info g_sc431hai_info[OT_ISP_MAX_PIPE_NUM] = {
    [0 ...(OT_ISP_MAX_PIPE_NUM - 1)] {
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .sns_id = SC431HAI_ID,
        .fswdr_mode = OT_ISP_FSWDR_NORMAL_MODE,
        .quick_start_en = TD_FALSE,
        .fast_ae_en = TD_FALSE,
        .max_short_exp = SC431HAI_SEXP_MAX_DEFAULT,
        .bus_info = { .i2c_dev = -1 },
        .sns_state = TD_NULL,
        .blc_clamp_info = TD_TRUE,
        .mode_tbl = {
             /* mode 0: linear */
            {
                SC431HAI_VMAX_LINEAR,
                SC431HAI_FULL_LINES_MAX_LINEAR,
                SC431HAI_FPS_MAX_LINEAR,
                SC431HAI_FPS_MIN_LINEAR,
                SC431HAI_WIDTH_LINEAR,
                SC431HAI_HEIGHT_LINEAR,
                SC431HAI_MODE_LINEAR,
                OT_WDR_MODE_NONE,
                "SC431HAI_4M_30FPS_10BIT_LINEAR_MODE"
            },
            /* mode 1: WDR */
            {
                SC431HAI_VMAX_2TO1_LINE_WDR,
                SC431HAI_FULL_LINES_MAX_2TO1_LINE_WDR,
                SC431HAI_FPS_MAX_2TO1_LINE_WDR,
                SC431HAI_FPS_MIN_2TO1_LINE_WDR,
                SC431HAI_WIDTH_2TO1_LINE_WDR,
                SC431HAI_HEIGHT_2TO1_LINE_WDR,
                SC431HAI_MODE_2TO1_LINE_WDR,
                OT_WDR_MODE_2To1_LINE,
                "SC431HAI_4M_30FPS_10BIT_2TO1_VC_MODE"
            },
        },
        .i2c.fd = -1,
    }
};

/****************************************************************************
  Again & Dgain table for TABLE Mode                                        *
 ****************************************************************************/
#define SC431HAI_DCG_RATIO 1.54f
#define SC431HAI_AGAIN_NODE_NUM               320
#define SC431HAI_AGAIN_ADDR_INDEX_NODE_NUM    320
#define SC431HAI_DGAIN_NODE_NUM               256
#define SC431HAI_DGAIN_ADDR_INDEX_NODE_NUM    256

static td_u32 again_index[SC431HAI_AGAIN_NODE_NUM] = {
    1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152, 1168, 1184, 1200, 1216, 1232, 1248, 1264,
    1280, 1296, 1312, 1328, 1344, 1360, 1376, 1392, 1408, 1424, 1440, 1456, 1472, 1488, 1504, 1520,
    1536, 1552, 1568, 1584, 1600, 1616, 1632, 1648, 1664, 1680, 1696, 1712, 1728, 1744, 1760, 1776,
    1792, 1808, 1824, 1840, 1856, 1872, 1888, 1904, 1920, 1936, 1952, 1968, 1984, 2000, 2016, 2032,
    2048, 2080, 2112, 2144, 2176, 2208, 2240, 2272, 2304, 2336, 2368, 2400, 2432, 2464, 2496, 2528,
    2560, 2592, 2624, 2656, 2688, 2720, 2752, 2784, 2816, 2848, 2880, 2912, 2944, 2976, 3008, 3040,
    3072, 3104, 3136, 3168, 3200, 3232, 3264, 3296, 3328, 3360, 3392, 3424, 3456, 3488, 3520, 3552,
    3584, 3616, 3648, 3680, 3712, 3744, 3776, 3808, 3840, 3872, 3904, 3936, 3968, 4000, 4032, 4064,
    4096, 4160, 4224, 4288, 4352, 4416, 4480, 4544, 4608, 4672, 4736, 4800, 4864, 4928, 4992, 5056,
    5120, 5184, 5248, 5312, 5376, 5440, 5504, 5568, 5632, 5696, 5760, 5824, 5888, 5952, 6016, 6080,
    6144, 6208, 6272, 6336, 6400, 6464, 6528, 6592, 6656, 6720, 6784, 6848, 6912, 6976, 7040, 7104,
    7168, 7232, 7296, 7360, 7424, 7488, 7552, 7616, 7680, 7744, 7808, 7872, 7936, 8000, 8064, 8128,
    8192, 8320, 8448, 8576, 8704, 8832, 8960, 9088, 9216, 9344, 9472, 9600, 9728, 9856, 9984, 10112,
    10240, 10368, 10496, 10624, 10752, 10880, 11008, 11136, 11264, 11392, 11520, 11648, 11776, 11904, 12032, 12160,
    12288, 12416, 12544, 12672, 12800, 12928, 13056, 13184, 13312, 13440, 13568, 13696, 13824, 13952, 14080, 14208,
    14336, 14464, 14592, 14720, 14848, 14976, 15104, 15232, 15360, 15488, 15616, 15744, 15872, 16000, 16128, 16256,
    16384, 16640, 16896, 17152, 17408, 17664, 17920, 18176, 18432, 18688, 18944, 19200, 19456, 19712, 19968, 20224,
    20480, 20736, 20992, 21248, 21504, 21760, 22016, 22272, 22528, 22784, 23040, 23296, 23552, 23808, 24064, 24320,
    24576, 24832, 25088, 25344, 25600, 25856, 26112, 26368, 26624, 26880, 27136, 27392, 27648, 27904, 28160, 28416,
    28672, 28928, 29184, 29440, 29696, 29952, 30208, 30464, 30720, 30976, 31232, 31488, 31744, 32000, 32256, 32512
};

static td_u32 dgain_index[SC431HAI_DGAIN_NODE_NUM] = {
    1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152, 1168, 1184, 1200, 1216, 1232, 1248, 1264,
    1280, 1296, 1312, 1328, 1344, 1360, 1376, 1392, 1408, 1424, 1440, 1456, 1472, 1488, 1504, 1520,
    1536, 1552, 1568, 1584, 1600, 1616, 1632, 1648, 1664, 1680, 1696, 1712, 1728, 1744, 1760, 1776,
    1792, 1808, 1824, 1840, 1856, 1872, 1888, 1904, 1920, 1936, 1952, 1968, 1984, 2000, 2016, 2032,
    2048, 2080, 2112, 2144, 2176, 2208, 2240, 2272, 2304, 2336, 2368, 2400, 2432, 2464, 2496, 2528,
    2560, 2592, 2624, 2656, 2688, 2720, 2752, 2784, 2816, 2848, 2880, 2912, 2944, 2976, 3008, 3040,
    3072, 3104, 3136, 3168, 3200, 3232, 3264, 3296, 3328, 3360, 3392, 3424, 3456, 3488, 3520, 3552,
    3584, 3616, 3648, 3680, 3712, 3744, 3776, 3808, 3840, 3872, 3904, 3936, 3968, 4000, 4032, 4064,
    4096, 4160, 4224, 4288, 4352, 4416, 4480, 4544, 4608, 4672, 4736, 4800, 4864, 4928, 4992, 5056,
    5120, 5184, 5248, 5312, 5376, 5440, 5504, 5568, 5632, 5696, 5760, 5824, 5888, 5952, 6016, 6080,
    6144, 6208, 6272, 6336, 6400, 6464, 6528, 6592, 6656, 6720, 6784, 6848, 6912, 6976, 7040, 7104,
    7168, 7232, 7296, 7360, 7424, 7488, 7552, 7616, 7680, 7744, 7808, 7872, 7936, 8000, 8064, 8128,
    8192, 8320, 8448, 8576, 8704, 8832, 8960, 9088, 9216, 9344, 9472, 9600, 9728, 9856, 9984, 10112,
    10240, 10368, 10496, 10624, 10752, 10880, 11008, 11136, 11264, 11392, 11520, 11648, 11776, 11904, 12032, 12160,
    12288, 12416, 12544, 12672, 12800, 12928, 13056, 13184, 13312, 13440, 13568, 13696, 13824, 13952, 14080, 14208,
    14336, 14464, 14592, 14720, 14848, 14976, 15104, 15232, 15360, 15488, 15616, 15744, 15872, 16000, 16128, 16256
};

static td_u32 again_table[SC431HAI_AGAIN_ADDR_INDEX_NODE_NUM] = {
    0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D,
    0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B,
    0x005C, 0x005D, 0x005E, 0x005F, 0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069,
    0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
    0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F, 0x0140, 0x0141, 0x0142, 0x0143, 0x0144, 0x0145,
    0x0146, 0x0147, 0x0148, 0x0149, 0x014A, 0x014B, 0x014C, 0x014D, 0x014E, 0x014F, 0x0150, 0x0151, 0x0152, 0x0153,
    0x0154, 0x0155, 0x0156, 0x0157, 0x0158, 0x0159, 0x015A, 0x015B, 0x015C, 0x015D, 0x015E, 0x015F, 0x0160, 0x0161,
    0x0162, 0x0163, 0x0164, 0x0165, 0x0166, 0x0167, 0x0168, 0x0169, 0x016A, 0x016B, 0x016C, 0x016D, 0x016E, 0x016F,
    0x0170, 0x0171, 0x0172, 0x0173, 0x0174, 0x0175, 0x0176, 0x0177, 0x0178, 0x0179, 0x017A, 0x017B, 0x017C, 0x017D,
    0x017E, 0x017F, 0x0340, 0x0341, 0x0342, 0x0343, 0x0344, 0x0345, 0x0346, 0x0347, 0x0348, 0x0349, 0x034A, 0x034B,
    0x034C, 0x034D, 0x034E, 0x034F, 0x0350, 0x0351, 0x0352, 0x0353, 0x0354, 0x0355, 0x0356, 0x0357, 0x0358, 0x0359,
    0x035A, 0x035B, 0x035C, 0x035D, 0x035E, 0x035F, 0x0360, 0x0361, 0x0362, 0x0363, 0x0364, 0x0365, 0x0366, 0x0367,
    0x0368, 0x0369, 0x036A, 0x036B, 0x036C, 0x036D, 0x036E, 0x036F, 0x0370, 0x0371, 0x0372, 0x0373, 0x0374, 0x0375,
    0x0376, 0x0377, 0x0378, 0x0379, 0x037A, 0x037B, 0x037C, 0x037D, 0x037E, 0x037F, 0x0740, 0x0741, 0x0742, 0x0743,
    0x0744, 0x0745, 0x0746, 0x0747, 0x0748, 0x0749, 0x074A, 0x074B, 0x074C, 0x074D, 0x074E, 0x074F, 0x0750, 0x0751,
    0x0752, 0x0753, 0x0754, 0x0755, 0x0756, 0x0757, 0x0758, 0x0759, 0x075A, 0x075B, 0x075C, 0x075D, 0x075E, 0x075F,
    0x0760, 0x0761, 0x0762, 0x0763, 0x0764, 0x0765, 0x0766, 0x0767, 0x0768, 0x0769, 0x076A, 0x076B, 0x076C, 0x076D,
    0x076E, 0x076F, 0x0770, 0x0771, 0x0772, 0x0773, 0x0774, 0x0775, 0x0776, 0x0777, 0x0778, 0x0779, 0x077A, 0x077B,
    0x077C, 0x077D, 0x077E, 0x077F, 0x0F40, 0x0F41, 0x0F42, 0x0F43, 0x0F44, 0x0F45, 0x0F46, 0x0F47, 0x0F48, 0x0F49,
    0x0F4A, 0x0F4B, 0x0F4C, 0x0F4D, 0x0F4E, 0x0F4F, 0x0F50, 0x0F51, 0x0F52, 0x0F53, 0x0F54, 0x0F55, 0x0F56, 0x0F57,
    0x0F58, 0x0F59, 0x0F5A, 0x0F5B, 0x0F5C, 0x0F5D, 0x0F5E, 0x0F5F, 0x0F60, 0x0F61, 0x0F62, 0x0F63, 0x0F64, 0x0F65,
    0x0F66, 0x0F67, 0x0F68, 0x0F69, 0x0F6A, 0x0F6B, 0x0F6C, 0x0F6D, 0x0F6E, 0x0F6F, 0x0F70, 0x0F71, 0x0F72, 0x0F73,
    0x0F74, 0x0F75, 0x0F76, 0x0F77, 0x0F78, 0x0F79, 0x0F7A, 0x0F7B, 0x0F7C, 0x0F7D, 0x0F7E, 0x0F7F
};

static td_u32 dgain_table[SC431HAI_DGAIN_ADDR_INDEX_NODE_NUM] = {
    0x0080, 0x0082, 0x0084, 0x0086, 0x0088, 0x008A, 0x008C, 0x008E, 0x0090, 0x0092, 0x0094, 0x0096, 0x0098, 0x009A,
    0x009C, 0x009E, 0x00A0, 0x00A2, 0x00A4, 0x00A6, 0x00A8, 0x00AA, 0x00AC, 0x00AE, 0x00B0, 0x00B2, 0x00B4, 0x00B6,
    0x00B8, 0x00BA, 0x00BC, 0x00BE, 0x00C0, 0x00C2, 0x00C4, 0x00C6, 0x00C8, 0x00CA, 0x00CC, 0x00CE, 0x00D0, 0x00D2,
    0x00D4, 0x00D6, 0x00D8, 0x00DA, 0x00DC, 0x00DE, 0x00E0, 0x00E2, 0x00E4, 0x00E6, 0x00E8, 0x00EA, 0x00EC, 0x00EE,
    0x00F0, 0x00F2, 0x00F4, 0x00F6, 0x00F8, 0x00FA, 0x00FC, 0x00FE, 0x0180, 0x0182, 0x0184, 0x0186, 0x0188, 0x018A,
    0x018C, 0x018E, 0x0190, 0x0192, 0x0194, 0x0196, 0x0198, 0x019A, 0x019C, 0x019E, 0x01A0, 0x01A2, 0x01A4, 0x01A6,
    0x01A8, 0x01AA, 0x01AC, 0x01AE, 0x01B0, 0x01B2, 0x01B4, 0x01B6, 0x01B8, 0x01BA, 0x01BC, 0x01BE, 0x01C0, 0x01C2,
    0x01C4, 0x01C6, 0x01C8, 0x01CA, 0x01CC, 0x01CE, 0x01D0, 0x01D2, 0x01D4, 0x01D6, 0x01D8, 0x01DA, 0x01DC, 0x01DE,
    0x01E0, 0x01E2, 0x01E4, 0x01E6, 0x01E8, 0x01EA, 0x01EC, 0x01EE, 0x01F0, 0x01F2, 0x01F4, 0x01F6, 0x01F8, 0x01FA,
    0x01FC, 0x01FE, 0x0380, 0x0382, 0x0384, 0x0386, 0x0388, 0x038A, 0x038C, 0x038E, 0x0390, 0x0392, 0x0394, 0x0396,
    0x0398, 0x039A, 0x039C, 0x039E, 0x03A0, 0x03A2, 0x03A4, 0x03A6, 0x03A8, 0x03AA, 0x03AC, 0x03AE, 0x03B0, 0x03B2,
    0x03B4, 0x03B6, 0x03B8, 0x03BA, 0x03BC, 0x03BE, 0x03C0, 0x03C2, 0x03C4, 0x03C6, 0x03C8, 0x03CA, 0x03CC, 0x03CE,
    0x03D0, 0x03D2, 0x03D4, 0x03D6, 0x03D8, 0x03DA, 0x03DC, 0x03DE, 0x03E0, 0x03E2, 0x03E4, 0x03E6, 0x03E8, 0x03EA,
    0x03EC, 0x03EE, 0x03F0, 0x03F2, 0x03F4, 0x03F6, 0x03F8, 0x03FA, 0x03FC, 0x03FE, 0x0780, 0x0782, 0x0784, 0x0786,
    0x0788, 0x078A, 0x078C, 0x078E, 0x0790, 0x0792, 0x0794, 0x0796, 0x0798, 0x079A, 0x079C, 0x079E, 0x07A0, 0x07A2,
    0x07A4, 0x07A6, 0x07A8, 0x07AA, 0x07AC, 0x07AE, 0x07B0, 0x07B2, 0x07B4, 0x07B6, 0x07B8, 0x07BA, 0x07BC, 0x07BE,
    0x07C0, 0x07C2, 0x07C4, 0x07C6, 0x07C8, 0x07CA, 0x07CC, 0x07CE, 0x07D0, 0x07D2, 0x07D4, 0x07D6, 0x07D8, 0x07DA,
    0x07DC, 0x07DE, 0x07E0, 0x07E2, 0x07E4, 0x07E6, 0x07E8, 0x07EA, 0x07EC, 0x07EE, 0x07F0, 0x07F2, 0x07F4, 0x07F6,
    0x07F8, 0x07FA, 0x07FC, 0x07FE
};

/****************************************************************************
 * common functions                                                         *
 ****************************************************************************/
static cis_info *cmos_get_info(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_NULL;
    }
    return &g_sc431hai_info[vi_pipe];
}

static ot_isp_sns_state *cmos_get_state(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_NULL;
    }
    return cmos_get_info(vi_pipe)->sns_state;
}

static void cmos_err_mode_print(const ot_isp_cmos_sns_image_mode *sns_image_mode, const ot_isp_sns_state *sns_state)
{
    isp_err_trace("Not support! Width:%u, Height:%u, Fps:%f, WDRMode:%d\n",
        (sns_image_mode)->width, (sns_image_mode)->height, (sns_image_mode)->fps, (sns_state)->wdr_mode);
}

static td_void cmos_get_ae_comm_default(cis_info *cis, ot_isp_ae_sensor_default *ae_sns_dft,
    const ot_isp_sns_state *sns_state)
{
    td_float max_fps = STANDARD_FPS; /* maxfps 30 */
    td_u32 fl = SC431HAI_VMAX_LINEAR << 1;

    max_fps = cis->mode_tbl[sns_state->img_mode].max_fps;
    if (sns_state->img_mode == SC431HAI_4M_30FPS_10BIT_LINEAR_MODE) {
        ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
        ae_sns_dft->int_time_accu.accuracy = INT_TIME_ACCURACY_LINEAR; /* accuracy:1 */
        ae_sns_dft->int_time_accu.offset = 0;
        ae_sns_dft->full_lines_max = SC431HAI_FULL_LINES_MAX_LINEAR << 1;
    } else if (sns_state->img_mode == SC431HAI_4M_30FPS_10BIT_2TO1_VC_MODE) {
        ae_sns_dft->int_time_accu.accu_type = OT_ISP_AE_ACCURACY_LINEAR;
        ae_sns_dft->int_time_accu.accuracy = INT_TIME_ACCURACY_WDR; /* accuracy: 4 */
        ae_sns_dft->int_time_accu.offset = 0;
        ae_sns_dft->full_lines_max = SC431HAI_FULL_LINES_MAX_2TO1_LINE_WDR << 1;
        fl = SC431HAI_VMAX_2TO1_LINE_WDR << 1;
    } else {
    }

    ae_sns_dft->full_lines_std = sns_state->fl_std ;
    ae_sns_dft->flicker_freq = FLICKER_FREQ; /* light flicker freq: 50Hz, accuracy: 256 */
    ae_sns_dft->hmax_times = (1000000000) / (fl * max_fps); /* 1000000000ns, 30fps */
    ae_sns_dft->again_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy  = AGAIN_ACCURACY;
    ae_sns_dft->dgain_accu.accu_type = OT_ISP_AE_ACCURACY_TABLE;
    ae_sns_dft->dgain_accu.accuracy = DGAIN_ACCURACY;
    ae_sns_dft->isp_dgain_shift = ISP_DGAIN_SHIFT; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = ISP_DGAIN_TARGET_MIN << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = ISP_DGAIN_TARGET_MAX << ae_sns_dft->isp_dgain_shift; /* max 4 */
    if (cis->lines_per500ms == 0) {
        ae_sns_dft->lines_per500ms = fl * max_fps / 2; /* 30fps, div 2 */
    } else {
        ae_sns_dft->lines_per500ms = cis->lines_per500ms;
    }
    (td_void)memcpy_s(&ae_sns_dft->piris_attr, sizeof(ot_isp_piris_attr), &g_piris, sizeof(ot_isp_piris_attr));
    ae_sns_dft->max_iris_fno = OT_ISP_IRIS_F_NO_1_4;
    ae_sns_dft->min_iris_fno = OT_ISP_IRIS_F_NO_5_6;
    ae_sns_dft->ae_route_ex_valid = TD_FALSE;
    ae_sns_dft->ae_route_attr.total_num = 0;
    ae_sns_dft->ae_route_attr_ex.total_num = 0;
    ae_sns_dft->quick_start.quick_start_enable = cis->quick_start_en;
    ae_sns_dft->quick_start.black_frame_num = 0;
    return;
}

static td_void cmos_get_ae_linear_default(cis_info *cis, ot_isp_ae_sensor_default *ae_sns_dft,
    const ot_isp_sns_state *sns_state)
{
    ae_sns_dft->max_again = SC431HAI_AGAIN_MAX; /* 31.75*1.54*1024 = 50069 */
    ae_sns_dft->min_again = SC431HAI_AGAIN_MIN; /* min 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;
    ae_sns_dft->max_dgain = SC431HAI_DGAIN_MAX; /* max 16256 */
    ae_sns_dft->min_dgain = SC431HAI_DGAIN_MIN; /* min 1024 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->ae_compensation = AE_COMENSATION_DEFAULT;
    ae_sns_dft->ae_exp_mode = OT_ISP_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = cis->init_exposure ? cis->init_exposure : INIT_EXP_DEFAULT_LINEAR; /* init 148859 */
    ae_sns_dft->init_int_time = cis->init_int_time;
    ae_sns_dft->init_again = cis->init_again;
    ae_sns_dft->init_dgain = cis->init_dgain;
    ae_sns_dft->init_isp_dgain = cis->init_isp_dgain;
    ae_sns_dft->max_int_time = sns_state->fl_std - FL_OFFSET_LINEAR; /* half line value 2 */
    ae_sns_dft->min_int_time = 8; /* min int 8 */
    ae_sns_dft->max_int_time_target = MAX_INT_TIME_TARGET; /* max int 65535 */
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;
    ae_sns_dft->ae_route_ex_valid = cis->ae_route_ex_valid;
    (td_void)memcpy_s(&ae_sns_dft->ae_route_attr, sizeof(ot_isp_ae_route),
                      &cis->init_ae_route,  sizeof(ot_isp_ae_route));
    (td_void)memcpy_s(&ae_sns_dft->ae_route_attr_ex, sizeof(ot_isp_ae_route_ex),
                      &cis->init_ae_route_ex, sizeof(ot_isp_ae_route_ex));
    return;
}

static td_void cmos_get_ae_2to1_line_wdr_default(cis_info *cis, ot_isp_ae_sensor_default *ae_sns_dft,
    const ot_isp_sns_state *sns_state)
{
    ae_sns_dft->max_int_time = sns_state->fl_std - FL_OFFSET_WDR_LONG - FL_OFFSET_WDR_SHORT; /* half line value 2 */
    ae_sns_dft->min_int_time = 8; /* min int 8 */
    ae_sns_dft->int_time_accu.offset = 0; /* offset: 0 */
    ae_sns_dft->max_int_time_target = MAX_INT_TIME_TARGET; /* max 65535 */
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;
    ae_sns_dft->max_again = SC431HAI_AGAIN_MAX; /* 31.75*1.54*1024 = 50069 */
    ae_sns_dft->min_again = SC431HAI_AGAIN_MIN; /* min 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;
    ae_sns_dft->max_dgain = SC431HAI_DGAIN_MAX; /* max 16256 */
    ae_sns_dft->min_dgain = SC431HAI_DGAIN_MIN; /* min 1024 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->max_isp_dgain_target = ISP_DGAIN_TARGET_WDR_MAX << ae_sns_dft->isp_dgain_shift; /* max 4 << shift */
    ae_sns_dft->diff_gain_support = TD_TRUE;
    ae_sns_dft->init_exposure = cis->init_exposure ? cis->init_exposure : INIT_EXP_DEFAULT_WDR; /* init 66462 */

    if (cis->fswdr_mode == OT_ISP_FSWDR_LONG_FRAME_MODE) {
        ae_sns_dft->ae_compensation = AE_COMENSATION_WDR_LONG_FRM; /* ae_compensation 56 */
        ae_sns_dft->ae_exp_mode = OT_ISP_AE_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->ae_compensation = AE_COMENSATION_WDR_NORM_FRM; /* ae_compensation 32 */
        ae_sns_dft->ae_exp_mode = OT_ISP_AE_EXP_LOWLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable = TD_TRUE;
        ae_sns_dft->arr_ratio[0] = AE_ARR_RATIO_0_WDR;
        ae_sns_dft->arr_ratio[1] = AE_ARR_RATIO_1_WDR;
        ae_sns_dft->arr_ratio[2] = AE_ARR_RATIO_2_WDR; /* array index 2 */
    }
    ae_sns_dft->lf_min_exposure = 40000; /* lf_threshold 40000 */
    ae_sns_dft->ae_route_ex_valid = cis->ae_route_ex_valid;

    (td_void)memcpy_s(&ae_sns_dft->ae_route_attr, sizeof(ot_isp_ae_route),
                      &cis->init_ae_route,  sizeof(ot_isp_ae_route));
    (td_void)memcpy_s(&ae_sns_dft->ae_route_attr_ex, sizeof(ot_isp_ae_route_ex),
                      &cis->init_ae_route_ex,  sizeof(ot_isp_ae_route_ex));
    (td_void)memcpy_s(&ae_sns_dft->ae_route_sf_attr, sizeof(ot_isp_ae_route),
                      &cis->init_ae_route_sf, sizeof(ot_isp_ae_route));
    (td_void)memcpy_s(&ae_sns_dft->ae_route_sf_attr_ex, sizeof(ot_isp_ae_route_ex),
                      &cis->init_ae_route_sf_ex,  sizeof(ot_isp_ae_route_ex));
    return;
}

static td_s32 cmos_get_ae_default(ot_vi_pipe vi_pipe, ot_isp_ae_sensor_default *ae_sns_dft)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(ae_sns_dft);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    sns_state = cis->sns_state;

    sns_check_pointer_return(sns_state);

    (td_void)memset_s(&ae_sns_dft->ae_route_attr, sizeof(ot_isp_ae_route), 0, sizeof(ot_isp_ae_route));

    cmos_get_ae_comm_default(cis, ae_sns_dft, sns_state);

    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_NONE:
            cmos_get_ae_linear_default(cis, ae_sns_dft, sns_state);
            break;
        case OT_WDR_MODE_2To1_LINE:
            cmos_get_ae_2to1_line_wdr_default(cis, ae_sns_dft, sns_state);
            break;
        default:
            cmos_get_ae_linear_default(cis, ae_sns_dft, sns_state);
            break;
    }

    return TD_SUCCESS;
}

static td_void cmos_config_vmax(ot_isp_sns_state *sns_state, td_u32 vmax)
{
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(vmax);
        sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = high_8bits(vmax);
    } else {
        sns_state->regs_info[0].i2c_data[WDR_VMAX_L_IDX].data = low_8bits(vmax);
        sns_state->regs_info[0].i2c_data[WDR_VMAX_H_IDX].data = high_8bits(vmax);
    }

    return;
}

/* the function of sensor set fps */
static td_void cmos_fps_set(ot_vi_pipe vi_pipe, td_float fps, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 lines;
    td_u32 lines_max;
    td_u32 vmax;
    td_float max_fps;
    td_float min_fps;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);
    sns_check_pointer_void_return(ae_sns_dft);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    lines = cis->mode_tbl[sns_state->img_mode].ver_lines;
    lines_max = cis->mode_tbl[sns_state->img_mode].max_ver_lines;
    max_fps = cis->mode_tbl[sns_state->img_mode].max_fps;
    min_fps = cis->mode_tbl[sns_state->img_mode].min_fps;

    if ((fps > max_fps) || (fps < min_fps)) {
        isp_err_trace("ISP sensor sc431hai Not support Fps: %f\n", fps);
        return;
    }

    vmax = (td_u32)(lines * max_fps / div_0_to_1_float(fps));
    vmax = ((vmax + 1) >> 1) << 1;
    vmax = (vmax > lines_max) ? lines_max : vmax;

    cmos_config_vmax(sns_state, vmax);
    sns_state->fl_std = vmax << 1;
    ae_sns_dft->lines_per500ms = (td_u32)(lines * 2 * max_fps / 2); /* 30/2 */
    cis->lines_per500ms = ae_sns_dft->lines_per500ms; // add for E4
    ae_sns_dft->fps = fps;
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sns_state->fl_std  - FL_OFFSET_LINEAR;
    } else {
        ae_sns_dft->max_int_time = sns_state->fl_std - FL_OFFSET_WDR_LONG - FL_OFFSET_WDR_SHORT;
    }

    sns_state->fl[0] = sns_state->fl_std ;
    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->hmax_times =
        (1000000000) / (sns_state->fl_std * div_0_to_1_float(fps)); /* 1000000000ns */
    return;
}

static td_void cmos_slow_framerate_set(ot_vi_pipe vi_pipe, td_u32 full_lines, ot_isp_ae_sensor_default *ae_sns_dft)
{
    td_u32 vmax;
    td_u32 lines_max;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);
    sns_check_pointer_void_return(ae_sns_dft);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    lines_max = cis->mode_tbl[sns_state->img_mode].max_ver_lines;

    vmax = (full_lines + FL_OFFSET_LINEAR) >> 1;
    vmax = ((vmax + 1) >> 1) << 1;
    vmax = clip3(vmax, SC431HAI_VMAX_LINEAR, lines_max);
    sns_state->fl[0] = vmax << 1;

    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(vmax);
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = high_8bits(vmax);

    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sns_state->fl[0]  - FL_OFFSET_LINEAR;
    } else {
        ae_sns_dft->max_int_time = sns_state->fl[0] - FL_OFFSET_WDR_LONG - FL_OFFSET_WDR_SHORT;
    }
    ae_sns_dft->full_lines = full_lines;
    return;
}

static td_void cmos_inttime_update_linear(ot_isp_sns_state *sns_state, td_u32 int_time)
{
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].data = lower_4bits(int_time);
    sns_state->regs_info[0].i2c_data[EXPO_M_IDX].data = higher_8bits(int_time);
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].data = higher_4bits(int_time);

    return;
}

static td_void cmos_inttime_update_2to1_line(ot_vi_pipe vi_pipe,
    cis_info *cis, ot_isp_sns_state *sns_state, td_u32 int_time)
{
    td_u32 max_short_exp, long_int_time_max;
    td_s32 max_next_short_exp;
    static td_bool is_first[OT_ISP_MAX_PIPE_NUM] = {[0 ...(OT_ISP_MAX_PIPE_NUM - 1)] = 1};

    static td_u32 short_int_time[OT_ISP_MAX_PIPE_NUM] = {0};
    static td_u32 long_int_time[OT_ISP_MAX_PIPE_NUM] = {0};

    if (is_first[vi_pipe]) { /* short exposure */
        sns_state->wdr_int_time[0] = int_time;
        short_int_time[vi_pipe] = int_time;
        is_first[vi_pipe] = TD_FALSE;
    } else { /* long exposure */
        sns_state->wdr_int_time[1] = int_time;
        long_int_time[vi_pipe] = int_time;

        max_short_exp = MAX2((short_int_time[vi_pipe] + FL_OFFSET_WDR_SHORT) >> 1, SC431HAI_SEXP_MAX_DEFAULT);
        max_next_short_exp = (SC431HAI_EXP_Y + 20) * 2 + (td_s32)cis->max_short_exp - /* 20 2 */
            (td_s32)sns_state->fl[0] / 2 + 2; /* 2 */
        if (max_short_exp < cis->max_short_exp) {
            max_short_exp = (td_u32)(MAX2((td_s32)max_short_exp, max_next_short_exp));
        }
        cis->max_short_exp = (max_short_exp >> 1) << 1;
        long_int_time_max = cis->sns_state->fl[0] - 2 * cis->max_short_exp - FL_OFFSET_WDR_LONG; /* 2 */
        long_int_time[vi_pipe] = MIN2(long_int_time[vi_pipe], long_int_time_max);

        sns_state->regs_info[0].i2c_data[SHORT_EXP_MAX_H_ADDR_IDX].data = high_8bits(cis->max_short_exp);
        sns_state->regs_info[0].i2c_data[SHORT_EXP_MAX_L_ADDR_IDX].data = low_8bits(cis->max_short_exp & 0xfe);
        sns_state->regs_info[0].i2c_data[WDR_EXPO_L_IDX].data = lower_4bits(long_int_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[WDR_EXPO_M_IDX].data = higher_8bits(long_int_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[WDR_EXPO_H_IDX].data = higher_4bits(long_int_time[vi_pipe]);

        sns_state->regs_info[0].i2c_data[WDR_SHORT_EXPO_L_IDX].data = lower_4bits(short_int_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[WDR_SHORT_EXPO_M_IDX].data = higher_8bits(short_int_time[vi_pipe]);
        is_first[vi_pipe] = TD_TRUE;
    }
    return;
}

/* while isp notify ae to update sensor regs, ae call these funcs. */
static td_void cmos_inttime_update(ot_vi_pipe vi_pipe, td_u32 int_time)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        cmos_inttime_update_2to1_line(vi_pipe, cis, sns_state, int_time);
    } else {
        cmos_inttime_update_linear(sns_state, int_time);
    }

    return;
}

static td_void cmos_again_calc_table(ot_vi_pipe vi_pipe, td_u32 *again_lin, td_u32 *again_db)
{
    int i;
    td_u32 again_lin_tmp;

    sns_check_pointer_void_return(again_lin);
    sns_check_pointer_void_return(again_db);
    if (*again_lin >= (td_u32)(again_index[SC431HAI_AGAIN_ADDR_INDEX_NODE_NUM - 1] * SC431HAI_DCG_RATIO)) {
        *again_lin = (td_u32)(again_index[SC431HAI_AGAIN_ADDR_INDEX_NODE_NUM - 1] * SC431HAI_DCG_RATIO);
        if (SC431HAI_DCG_RATIO > 1) {
            *again_db = again_table[SC431HAI_AGAIN_NODE_NUM - 1] | 0x8000;
        } else {
            *again_db = again_table[SC431HAI_AGAIN_NODE_NUM - 1];
        }
        return;
    }
    if (*again_lin > (1024 * SC431HAI_DCG_RATIO)) {  /* 1024 */
        again_lin_tmp = (td_u32)(*again_lin / SC431HAI_DCG_RATIO);
        for (i = 1; i < SC431HAI_AGAIN_NODE_NUM; i++) {
            if (again_lin_tmp < again_index[i]) {
                *again_lin = again_index[i - 1] * SC431HAI_DCG_RATIO;
                *again_db = again_table[i - 1] | 0x8000;
                break;
            }
        }
    } else {
        again_lin_tmp = *again_lin;
        for (i = 1; i < SC431HAI_AGAIN_NODE_NUM; i++) {
            if (again_lin_tmp < again_index[i]) {
                *again_lin = again_index[i - 1];
                *again_db = again_table[i - 1];
                break;
            }
        }
    }
    return;
}

static td_void cmos_dgain_calc_table(ot_vi_pipe vi_pipe, td_u32 *dgain_lin, td_u32 *dgain_db)
{
    int i;
    ot_unused(vi_pipe);

    sns_check_pointer_void_return(dgain_lin);
    sns_check_pointer_void_return(dgain_db);

    if (*dgain_lin >= dgain_index[SC431HAI_DGAIN_ADDR_INDEX_NODE_NUM - 1]) {
        *dgain_lin = dgain_index[SC431HAI_DGAIN_ADDR_INDEX_NODE_NUM - 1];
        *dgain_db = dgain_table[SC431HAI_DGAIN_NODE_NUM - 1];
        return;
    }

    for (i = 1; i < SC431HAI_DGAIN_NODE_NUM; i++) {
        if (*dgain_lin < dgain_index[i]) {
            *dgain_lin = dgain_index[i - 1];
            *dgain_db = dgain_table[i - 1];
            break;
        }
    }
    return;
}

static td_void cmos_gains_update(ot_vi_pipe vi_pipe, td_u32 again, td_u32 dgain)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);
    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_void_return(sns_state);

    td_u8 reg_0x3e09;
    td_u8 reg_0x3e08;
    td_u8 reg_0x3e07;
    td_u8 reg_0x3e06;

    reg_0x3e06 = high_8bits(dgain);
    reg_0x3e07 = low_8bits (dgain);
    reg_0x3e08 = high_8bits(again);
    reg_0x3e09 = low_8bits (again);

    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].data = reg_0x3e06;
    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].data = reg_0x3e07;
    sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].data = reg_0x3e08;
    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].data = (reg_0x3e09 >> 1);

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        sns_state->regs_info[0].i2c_data[WDR_SHORT_DGAIN_H_IDX].data = reg_0x3e06;
        sns_state->regs_info[0].i2c_data[WDR_SHORT_DGAIN_L_IDX].data = reg_0x3e07;
        sns_state->regs_info[0].i2c_data[WDR_SHORT_AGAIN_H_IDX].data = reg_0x3e08;
        sns_state->regs_info[0].i2c_data[WDR_SHORT_AGAIN_L_IDX].data = (reg_0x3e09 >> 1);
    } else {
    }
    return;
}

static td_void cmos_get_inttime_max_2to1_line(ot_vi_pipe vi_pipe, td_u32 *ratio,
    ot_isp_ae_int_time_range *int_time, td_u32 *lf_max_int_time)
{
    td_u32 short_max0;
    td_u32 short_max;
    td_u32 short_time_min_limit;
    ot_isp_sns_state *sns_state = TD_NULL;
    cis_info *cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_void_return(sns_state);

    short_time_min_limit = 8; /* short_time_min_limit 8 */
    if (cis_wdr_range_check(ratio)) {
        isp_err_trace("cmos_2to1_line_wdr_range_check fail\n");
        return;
    }

    if (cis->fswdr_mode == OT_ISP_FSWDR_LONG_FRAME_MODE) {
        short_max0 = sns_state->fl[1] - FL_OFFSET_WDR_SHORT - sns_state->wdr_int_time[0]; /* half line value 2 */
        short_max = sns_state->fl[0] - FL_OFFSET_WDR_SHORT; /* half line value 2 */
        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        int_time->int_time_max[0] = short_time_min_limit;
        int_time->int_time_min[0] = short_time_min_limit;
        int_time->int_time_max[1] = short_max;
        int_time->int_time_min[1] = short_time_min_limit;
        return;
    } else {
        short_max0 = ((sns_state->fl[1] - FL_OFFSET_WDR_SHORT - FL_OFFSET_WDR_LONG - /* half line value 2 */
                     sns_state->wdr_int_time[0]) * 0x40) / div_0_to_1(ratio[0]);
        short_max = ((sns_state->fl[0] - FL_OFFSET_WDR_SHORT - FL_OFFSET_WDR_LONG) * 0x40) / (ratio[0] + 0x40);
        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        short_max = (short_max == 0) ? 1 : short_max;
    }
        *lf_max_int_time = sns_state->fl[0] * 2 - FL_OFFSET_WDR_SHORT; /* half line value 2 */
        if (short_max >= short_time_min_limit) {
            int_time->int_time_max[0] = short_max;
            int_time->int_time_max[1] = (int_time->int_time_max[0] * ratio[0]) >> 6; /* shift 6 */
            int_time->int_time_min[0] = short_time_min_limit;
            int_time->int_time_min[1] = (int_time->int_time_min[0] * ratio[0]) >> 6; /* shift 6 */
        } else {
            short_max = short_time_min_limit;
            int_time->int_time_max[0] = short_max;
            int_time->int_time_max[1] = (int_time->int_time_max[0] * 0xFFF) >> 6; /* shift 6 */
            int_time->int_time_min[0] = int_time->int_time_max[0];
            int_time->int_time_min[1] = int_time->int_time_max[1];
        }

    return;
}

static td_void cmos_get_inttime_max(ot_vi_pipe vi_pipe, td_u16 man_ratio_enable, td_u32 *ratio,
    ot_isp_ae_int_time_range *int_time, td_u32 *lf_max_int_time)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    ot_unused(man_ratio_enable);
    sns_check_pipe_void_return(vi_pipe);
    sns_check_pointer_void_return(ratio);
    sns_check_pointer_void_return(int_time);
    sns_check_pointer_void_return(lf_max_int_time);
    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_void_return(sns_state);

    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_2To1_LINE:
            cmos_get_inttime_max_2to1_line(vi_pipe, ratio, int_time, lf_max_int_time);
            break;
        default:
            break;
    }

    return;
}

/* Only used in LINE_WDR mode */
static td_void cmos_ae_fswdr_attr_set(ot_vi_pipe vi_pipe, ot_isp_ae_fswdr_attr *ae_fswdr_attr)
{
    cis_info *cis = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);
    sns_check_pointer_void_return(ae_fswdr_attr);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    cis->fswdr_mode = ae_fswdr_attr->fswdr_mode;
    cis->max_time_get_cnt = 0;

    return;
}

static td_void cmos_init_ae_exp_function(ot_isp_ae_sensor_exp_func *exp_func)
{
    (td_void)memset_s(exp_func, sizeof(ot_isp_ae_sensor_exp_func), 0, sizeof(ot_isp_ae_sensor_exp_func));
    exp_func->pfn_cmos_get_ae_default		= cmos_get_ae_default;
    exp_func->pfn_cmos_fps_set				= cmos_fps_set;
    exp_func->pfn_cmos_slow_framerate_set	= cmos_slow_framerate_set;
    exp_func->pfn_cmos_inttime_update		= cmos_inttime_update;
    exp_func->pfn_cmos_gains_update			= cmos_gains_update;
    exp_func->pfn_cmos_again_calc_table		= cmos_again_calc_table;
    exp_func->pfn_cmos_dgain_calc_table		= cmos_dgain_calc_table;
    exp_func->pfn_cmos_get_inttime_max		= cmos_get_inttime_max;
    exp_func->pfn_cmos_ae_fswdr_attr_set	= cmos_ae_fswdr_attr_set;

    return;
}

static td_s32 cmos_awb_get_default(ot_vi_pipe vi_pipe, ot_isp_awb_sensor_default *awb_sns_dft)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(awb_sns_dft);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_return(sns_state);

    (td_void)memset_s(awb_sns_dft, sizeof(ot_isp_awb_sensor_default), 0, sizeof(ot_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = CALIBRATE_STATIC_TEMP; /* wb_ref_temp 4950 */

    awb_sns_dft->gain_offset[0] = CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[1] = CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[2] = CALIBRATE_STATIC_WB_GB_GAIN; /* index 2 */
    awb_sns_dft->gain_offset[3] = CALIBRATE_STATIC_WB_B_GAIN; /* index 3 */

    awb_sns_dft->wb_para[0] = CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = CALIBRATE_AWB_Q1; /* index 2 */
    awb_sns_dft->wb_para[3] = CALIBRATE_AWB_A1; /* index 3 */
    awb_sns_dft->wb_para[4] = CALIBRATE_AWB_B1; /* index 4 */
    awb_sns_dft->wb_para[5] = CALIBRATE_AWB_C1; /* index 5 */

    awb_sns_dft->golden_rgain = GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GOLDEN_BGAIN;

    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_2To1_FRAME:
        case OT_WDR_MODE_2To1_LINE:
            (td_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_isp_awb_ccm), &g_awb_ccm_wdr, sizeof(ot_isp_awb_ccm));
            (td_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_isp_awb_agc_table),
                              &g_awb_agc_table_wdr, sizeof(ot_isp_awb_agc_table));
            (td_void)memcpy_s(&awb_sns_dft->sector, sizeof(ot_isp_color_sector),
                &g_color_sector_wdr, sizeof(ot_isp_color_sector));
            break;
        case OT_WDR_MODE_NONE:
        default:
            (td_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_isp_awb_ccm), &g_awb_ccm, sizeof(ot_isp_awb_ccm));
            (td_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_isp_awb_agc_table),
                              &g_awb_agc_table, sizeof(ot_isp_awb_agc_table));
            (td_void)memcpy_s(&awb_sns_dft->sector, sizeof(ot_isp_color_sector),
                &g_color_sector, sizeof(ot_isp_color_sector));
            break;
    }

    awb_sns_dft->init_rgain = cis->init_wb_r_gain;
    awb_sns_dft->init_ggain = cis->init_wb_g_gain;
    awb_sns_dft->init_bgain = cis->init_wb_b_gain;
    awb_sns_dft->sample_rgain = cis->sample_r_gain;
    awb_sns_dft->sample_bgain = cis->sample_b_gain;

    return TD_SUCCESS;
}

static td_void cmos_init_awb_exp_function(ot_isp_awb_sensor_exp_func *exp_func)
{
    (td_void)memset_s(exp_func, sizeof(ot_isp_awb_sensor_exp_func), 0, sizeof(ot_isp_awb_sensor_exp_func));

    exp_func->pfn_cmos_get_awb_default = cmos_awb_get_default;

    return;
}

static td_void cmos_isp_get_dng_default(const ot_isp_sns_state *sns_state, ot_isp_cmos_default *isp_def)
{
    ot_isp_cmos_dng_color_param dng_color_param = {{ 286, 256, 608 }, { 415, 256, 429 },
        { 2810, { 0x01AC, 0x8093, 0x8019, 0x8070, 0x01EA, 0x807A, 0x802A, 0x80F3, 0x021D }},
        { 4940, { 0x01D7, 0x8084, 0x8053, 0x8053, 0x01D9, 0x8086, 0x8010, 0x80B3, 0x01C3 }}};

    (td_void)memcpy_s(&isp_def->dng_color_param, sizeof(ot_isp_cmos_dng_color_param), &dng_color_param,
                      sizeof(ot_isp_cmos_dng_color_param));

    switch (sns_state->img_mode) {
        case SC431HAI_4M_30FPS_10BIT_LINEAR_MODE:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = DNG_RAW_FORMAT_BIT_LINEAR; /* 10bit */
            isp_def->sns_mode.dng_raw_format.white_level = DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR; /* max 1023 */
            break;

        case SC431HAI_4M_30FPS_10BIT_2TO1_VC_MODE:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = DNG_RAW_FORMAT_BIT_WDR; /* 10bit */
            isp_def->sns_mode.dng_raw_format.white_level = DNG_RAW_FORMAT_WHITE_LEVEL_WDR; /* max 1023 */
            break;

        default:
            isp_def->sns_mode.dng_raw_format.bits_per_sample = DNG_RAW_FORMAT_BIT_LINEAR; /* 10bit */
            isp_def->sns_mode.dng_raw_format.white_level = DNG_RAW_FORMAT_WHITE_LEVEL_LINEAR; /* max 1023 */
            break;
    }

    isp_def->sns_mode.dng_raw_format.default_scale.default_scale_hor.denominator = 1;
    isp_def->sns_mode.dng_raw_format.default_scale.default_scale_hor.numerator = 1;
    isp_def->sns_mode.dng_raw_format.default_scale.default_scale_ver.denominator = 1;
    isp_def->sns_mode.dng_raw_format.default_scale.default_scale_ver.numerator = 1;
    isp_def->sns_mode.dng_raw_format.cfa_repeat_pattern_dim.repeat_pattern_dim_row = 2; /* pattern 2 */
    isp_def->sns_mode.dng_raw_format.cfa_repeat_pattern_dim.repeat_pattern_dim_col = 2; /* pattern 2 */
    isp_def->sns_mode.dng_raw_format.black_level_repeat_dim.repeat_row = 2; /* pattern 2 */
    isp_def->sns_mode.dng_raw_format.black_level_repeat_dim.repeat_col = 2; /* pattern 2 */
    isp_def->sns_mode.dng_raw_format.cfa_layout = OT_ISP_CFALAYOUT_TYPE_RECTANGULAR;
    isp_def->sns_mode.dng_raw_format.cfa_plane_color[0] = 0;
    isp_def->sns_mode.dng_raw_format.cfa_plane_color[1] = 1;
    isp_def->sns_mode.dng_raw_format.cfa_plane_color[2] = 2; /* index 2, cfa_plane_color 2 */
    isp_def->sns_mode.dng_raw_format.cfa_pattern[0] = 0;
    isp_def->sns_mode.dng_raw_format.cfa_pattern[1] = 1;
    isp_def->sns_mode.dng_raw_format.cfa_pattern[2] = 1; /* index 2, cfa_pattern 1 */
    isp_def->sns_mode.dng_raw_format.cfa_pattern[3] = 2; /* index 3, cfa_pattern 2 */
    isp_def->sns_mode.valid_dng_raw_format = TD_TRUE;

    return;
}

static void cmos_isp_get_linear_default(ot_isp_cmos_default *isp_def)
{
    isp_def->key.bit1_demosaic         = 1;
    isp_def->demosaic                  = &g_cmos_demosaic;
    isp_def->key.bit1_sharpen          = 1;
    isp_def->sharpen                   = &g_cmos_yuv_sharpen;
    isp_def->key.bit1_drc              = 1;
    isp_def->drc                       = &g_cmos_drc;
    isp_def->key.bit1_bayer_nr         = 1;
    isp_def->bayer_nr                  = &g_cmos_bayer_nr;
    isp_def->key.bit1_anti_false_color = 1;
    isp_def->anti_false_color          = &g_cmos_anti_false_color;
    isp_def->key.bit1_cac              = 1;
    isp_def->cac                       = &g_cmos_cac;
    isp_def->key.bit1_ldci             = 1;
    isp_def->ldci                      = &g_cmos_ldci;
    isp_def->key.bit1_gamma            = 1;
    isp_def->gamma                     = &g_cmos_gamma;
    isp_def->key.bit1_clut             = 1;
    isp_def->clut                      = &g_cmos_clut;
#ifdef CONFIG_OT_ISP_CR_SUPPORT
    isp_def->key.bit1_ge               = 1;
    isp_def->ge                        = &g_cmos_ge;
#endif
    isp_def->key.bit1_dehaze = 1;
    isp_def->dehaze = &g_cmos_dehaze;
    isp_def->key.bit1_ca = 1;
    isp_def->ca = &g_cmos_ca;
    (td_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_isp_noise_calibration),
                      &g_cmos_noise_calibration, sizeof(ot_isp_noise_calibration));
    return;
}

static void cmos_isp_get_wdr_default(ot_isp_cmos_default *isp_def)
{
    isp_def->key.bit1_dpc            = 1;
    isp_def->dpc                     = &g_cmos_dpc_wdr;
    isp_def->key.bit1_demosaic       = 1;
    isp_def->demosaic                = &g_cmos_demosaic_wdr;
    isp_def->key.bit1_sharpen        = 1;
    isp_def->sharpen                 = &g_cmos_yuv_sharpen_wdr;
    isp_def->key.bit1_drc            = 1;
    isp_def->drc                     = &g_cmos_drc_wdr;
    isp_def->key.bit1_gamma          = 1;
    isp_def->gamma                   = &g_cmos_gamma_wdr;
    isp_def->key.bit1_clut           = 1;
    isp_def->clut                    = &g_cmos_clut_wdr;
#ifdef CONFIG_OT_ISP_PREGAMMA_SUPPORT
    isp_def->key.bit1_pregamma       = 1;
    isp_def->pregamma                = &g_cmos_pregamma;
#endif
    isp_def->key.bit1_bayer_nr       = 1;
    isp_def->bayer_nr                = &g_cmos_bayer_nr_wdr;
#ifdef CONFIG_OT_ISP_CR_SUPPORT
    isp_def->key.bit1_ge             = 1;
    isp_def->ge                      = &g_cmos_ge_wdr;
#endif
    isp_def->key.bit1_anti_false_color = 1;
    isp_def->anti_false_color = &g_cmos_anti_false_color_wdr;
    isp_def->key.bit1_cac = 1;
    isp_def->cac = &g_cmos_cac_wdr;
    isp_def->key.bit1_ldci = 1;
    isp_def->ldci = &g_cmos_ldci_wdr;
    isp_def->key.bit1_dehaze = 1;
    isp_def->dehaze = &g_cmos_dehaze_wdr;

    (td_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_isp_noise_calibration),
                      &g_cmos_noise_calibration, sizeof(ot_isp_noise_calibration));
    return;
}

static td_s32 cmos_isp_get_default(ot_vi_pipe vi_pipe, ot_isp_cmos_default *isp_def)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(isp_def);
    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_return(sns_state);

    (td_void)memset_s(isp_def, sizeof(ot_isp_cmos_default), 0, sizeof(ot_isp_cmos_default));
#ifdef CONFIG_OT_ISP_CA_SUPPORT
    isp_def->key.bit1_ca      = 1;
    isp_def->ca               = &g_cmos_ca;
#endif
    isp_def->key.bit1_dpc     = 1;
    isp_def->dpc              = &g_cmos_dpc;

    isp_def->key.bit1_wdr     = 1;
    isp_def->wdr              = &g_cmos_wdr;

    isp_def->key.bit1_lsc      = 0;
    isp_def->lsc               = &g_cmos_lsc;

    isp_def->key.bit1_acs      = 0;
    isp_def->acs               = &g_cmos_acs;

#ifdef CONFIG_OT_ISP_PREGAMMA_SUPPORT
    isp_def->key.bit1_pregamma = 0;
    isp_def->pregamma          = &g_cmos_pregamma;
#endif
    switch (sns_state->wdr_mode) {
        case OT_WDR_MODE_NONE:
            cmos_isp_get_linear_default(isp_def);
            break;
        case OT_WDR_MODE_2To1_LINE:
            cmos_isp_get_wdr_default(isp_def);
            break;
        default:
            cmos_isp_get_linear_default(isp_def);
            break;
    }

    isp_def->wdr_switch_attr.exp_ratio[0] = 0x40;

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        isp_def->wdr_switch_attr.exp_ratio[0] = 0x400;
    }

    isp_def->sns_mode.sns_id = SC431HAI_ID;
    isp_def->sns_mode.sns_mode = sns_state->img_mode;
    cmos_isp_get_dng_default(sns_state, isp_def);

    return TD_SUCCESS;
}

static td_s32 cmos_isp_get_black_level(ot_vi_pipe vi_pipe, ot_isp_cmos_black_level *black_level)
{
    td_s32  i;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(black_level);
    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_return(sns_state);

    (td_void)memcpy_s(black_level, sizeof(ot_isp_cmos_black_level), &g_cmos_blc, sizeof(ot_isp_cmos_black_level));

    /* Don't need to update black level when iso change */
    black_level->auto_attr.update = TD_FALSE;

    /* black level of linear mode */
    if (sns_state->wdr_mode == OT_WDR_MODE_NONE) {
        for (i = 0; i < OT_ISP_BAYER_CHN_NUM; i++) {
            black_level->auto_attr.black_level[0][i] = BLACK_LEVEL_DEFAULT;
        }
    } else { /* black level of DOL mode */
        for (i = 0; i < OT_ISP_WDR_MAX_FRAME_NUM; i++) {
            black_level->auto_attr.black_level[i][0] = BLACK_LEVEL_DEFAULT;
            black_level->auto_attr.black_level[i][1] = BLACK_LEVEL_DEFAULT;
            black_level->auto_attr.black_level[i][2] = BLACK_LEVEL_DEFAULT; /* index 2 */
            black_level->auto_attr.black_level[i][3] = BLACK_LEVEL_DEFAULT; /* index 3 */
        }
    }

    return TD_SUCCESS;
}

static td_s32 cmos_isp_get_blc_clamp_info(ot_vi_pipe vi_pipe, td_bool *blc_clamp_en)
{
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(blc_clamp_en);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    *blc_clamp_en = cis->blc_clamp_info;

    return TD_SUCCESS;
}

static td_s32 cmos_isp_set_wdr_mode(ot_vi_pipe vi_pipe, td_u8 mode)
{
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);

    sns_state = cmos_get_state(vi_pipe);
    sns_check_pointer_return(sns_state);

    sns_state->sync_init = TD_FALSE;

    switch (mode & 0x3F) {
        case OT_WDR_MODE_NONE:
            sns_state->wdr_mode = OT_WDR_MODE_NONE;
            printf("linear 4M30fps mode\n");
            break;

        case OT_WDR_MODE_2To1_LINE:
            sns_state->wdr_mode = OT_WDR_MODE_2To1_LINE;
            printf("2to1 line WDR 4M30fps mode!\n");
            break;

        default:
            isp_err_trace("Not support this mode!\n");
            return TD_FAILURE;
    }

    (td_void)memset_s(sns_state->wdr_int_time, sizeof(sns_state->wdr_int_time), 0, sizeof(sns_state->wdr_int_time));

    return TD_SUCCESS;
}

static td_void cmos_comm_sns_reg_info_init(cis_info *cis, ot_isp_sns_state *sns_state)
{
    td_u32 i;

    sns_state->regs_info[0].sns_type = OT_ISP_SNS_TYPE_I2C;
    sns_state->regs_info[0].com_bus.i2c_dev = cis->bus_info.i2c_dev;
    sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    sns_state->regs_info[0].reg_num = REG_MAX_IDX;

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        sns_state->regs_info[0].reg_num = WDR_REG_MAX_IDX;
        sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    }

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = SC431HAI_I2C_ADDR;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = SC431HAI_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = SC431HAI_DATA_BYTE;
    }

    /* Linear Mode Regs */
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].reg_addr = SC431HAI_EXPO_L_ADDR;
    sns_state->regs_info[0].i2c_data[EXPO_M_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_M_IDX].reg_addr = SC431HAI_EXPO_M_ADDR;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].reg_addr = SC431HAI_EXPO_H_ADDR;

    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].reg_addr = SC431HAI_AGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].reg_addr = SC431HAI_AGAIN_H_ADDR;

    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].reg_addr = SC431HAI_DGAIN_H_ADDR;
    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].reg_addr = SC431HAI_DGAIN_L_ADDR;

    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].reg_addr = SC431HAI_VMAX_L_ADDR;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].reg_addr = SC431HAI_VMAX_H_ADDR;
    return;
}

static td_void cmos_2to1_line_wdr_sns_reg_info_init(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state)
{
    ot_unused(vi_pipe);
    sns_state->regs_info[0].i2c_data[WDR_SHORT_EXPO_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_EXPO_L_IDX].reg_addr = SC431HAI_SHORT_EXPO_L_ADDR;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_EXPO_M_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_EXPO_M_IDX].reg_addr = SC431HAI_SHORT_EXPO_M_ADDR;

    sns_state->regs_info[0].i2c_data[WDR_SHORT_AGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_AGAIN_L_IDX].reg_addr = SC431HAI_SHORT_AGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_AGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_AGAIN_H_IDX].reg_addr = SC431HAI_SHORT_AGAIN_H_ADDR;

    sns_state->regs_info[0].i2c_data[WDR_SHORT_DGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_DGAIN_L_IDX].reg_addr = SC431HAI_SHORT_DGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_DGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[WDR_SHORT_DGAIN_H_IDX].reg_addr = SC431HAI_SHORT_DGAIN_H_ADDR;

    sns_state->regs_info[0].i2c_data[SHORT_EXP_MAX_L_ADDR_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[SHORT_EXP_MAX_L_ADDR_IDX].reg_addr = SC431HAI_SHORT_EXP_MAX_L_ADDR;
    sns_state->regs_info[0].i2c_data[SHORT_EXP_MAX_H_ADDR_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[SHORT_EXP_MAX_H_ADDR_IDX].reg_addr = SC431HAI_SHORT_EXP_MAX_H_ADDR;
    return;
}

static td_void cmos_sns_reg_info_update(ot_vi_pipe vi_pipe, ot_isp_sns_state *sns_state)
{
    td_u32 i;
    ot_unused(vi_pipe);

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        if (sns_state->regs_info[0].i2c_data[i].data ==
            sns_state->regs_info[1].i2c_data[i].data) {
            sns_state->regs_info[0].i2c_data[i].update = TD_FALSE;
        } else {
            sns_state->regs_info[0].i2c_data[i].update = TD_TRUE;
        }
    }

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        if (sns_state->fl[0] > sns_state->fl[1]) {
            sns_state->regs_info[0].i2c_data[WDR_VMAX_L_IDX].delay_frame_num = 0;
            sns_state->regs_info[0].i2c_data[WDR_VMAX_H_IDX].delay_frame_num = 0;
            sns_state->regs_info[0].i2c_data[SHORT_EXP_MAX_L_ADDR_IDX].delay_frame_num = 0;
            sns_state->regs_info[0].i2c_data[SHORT_EXP_MAX_H_ADDR_IDX].delay_frame_num = 0;
        } else if (sns_state->fl[0] < sns_state->fl[1]) {
            sns_state->regs_info[0].i2c_data[WDR_VMAX_L_IDX].delay_frame_num = 1;
            sns_state->regs_info[0].i2c_data[WDR_VMAX_H_IDX].delay_frame_num = 1;
            sns_state->regs_info[0].i2c_data[SHORT_EXP_MAX_L_ADDR_IDX].delay_frame_num = 1;
            sns_state->regs_info[0].i2c_data[SHORT_EXP_MAX_H_ADDR_IDX].delay_frame_num = 1;
        } else {
        }
    }
    return;
}

static td_s32 cmos_isp_get_sns_regs_info(ot_vi_pipe vi_pipe, ot_isp_sns_regs_info *sns_regs_info)
{
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(sns_regs_info);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_return(sns_state);

    if ((sns_state->sync_init == TD_FALSE) || (sns_regs_info->config == TD_FALSE)) {
        cmos_comm_sns_reg_info_init(cis, sns_state);
        if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
            /* VC 2t1 Mode Regs */
            cmos_2to1_line_wdr_sns_reg_info_init(vi_pipe, sns_state);
        }
        sns_state->sync_init = TD_TRUE;
    } else {
        cmos_sns_reg_info_update(vi_pipe, sns_state);
    }

    sns_regs_info->config = TD_FALSE;
    (td_void)memcpy_s(sns_regs_info, sizeof(ot_isp_sns_regs_info),
                      &sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info));
    (td_void)memcpy_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info),
                      &sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info));
    sns_state->fl[1] = sns_state->fl[0];

    return TD_SUCCESS;
}

static td_void cmos_isp_config_image_mode_param(ot_vi_pipe vi_pipe, td_u8 sns_image_mode,
    ot_isp_sns_state *sns_state)
{
    ot_unused(vi_pipe);
    switch (sns_image_mode) {
        case SC431HAI_4M_30FPS_10BIT_LINEAR_MODE:
            sns_state->fl_std = SC431HAI_VMAX_LINEAR * 2; /* 2 */
            break;
        case SC431HAI_4M_30FPS_10BIT_2TO1_VC_MODE:
            sns_state->fl_std = SC431HAI_VMAX_2TO1_LINE_WDR * 2; /* 2 */
            break;
        default:
            sns_state->fl_std = SC431HAI_VMAX_LINEAR * 2; /* 2 */
            break;
    }

    return;
}

static td_s32 cmos_isp_set_image_mode(ot_vi_pipe vi_pipe, const ot_isp_cmos_sns_image_mode *sns_image_mode)
{
    td_u32 i;
    td_u8 image_mode;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(sns_image_mode);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_return(sns_state);

    image_mode = sns_state->img_mode;

    for (i = 0; i < SC431HAI_MODE_MAX; i++) {
        if (sns_image_mode->fps <= cis->mode_tbl[i].max_fps &&
            sns_image_mode->width <= cis->mode_tbl[i].width &&
            sns_image_mode->height <= cis->mode_tbl[i].height &&
            sns_state->wdr_mode == cis->mode_tbl[i].wdr_mode) {
            image_mode = (sc431hai_res_mode)i;
            break;
        }
    }

    if (i >= SC431HAI_MODE_MAX) {
        cmos_err_mode_print(sns_image_mode, sns_state);
        return TD_FAILURE;
    }

    cmos_isp_config_image_mode_param(vi_pipe, image_mode, sns_state);

    if ((sns_state->init == TD_TRUE) && (image_mode == sns_state->img_mode)) {
        return OT_ISP_DO_NOT_NEED_SWITCH_IMAGEMODE; /* Don't need to switch image_mode */
    }

    sns_state->sync_init = TD_FALSE;
    sns_state->img_mode = image_mode;
    sns_state->fl[0] = sns_state->fl_std;
    sns_state->fl[1] = sns_state->fl[0];

    return TD_SUCCESS;
}

static void cmos_isp_init(ot_vi_pipe vi_pipe)
{
    td_s32           ret;
    ot_wdr_mode      wdr_mode;
    td_u8            img_mode;
    cis_info *cis = TD_NULL;
    ot_isp_sns_state *sensor_state = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sensor_state = cis->sns_state;
    sns_check_pointer_void_return(sensor_state);

    wdr_mode    = sensor_state->wdr_mode;
    img_mode    = sensor_state->img_mode;

    cis->i2c.addr = SC431HAI_I2C_ADDR;
    cis->i2c.addr_byte_num = SC431HAI_ADDR_BYTE;
    cis->i2c.data_byte_num = SC431HAI_DATA_BYTE;

    ret = cis_i2c_init(cis);
    if (ret != TD_SUCCESS) {
        isp_err_trace("i2c init failed!\n");
        return;
    }

    /* When sensor first init, config all registers */
    if (OT_WDR_MODE_2To1_LINE == wdr_mode) {
        if (SC431HAI_4M_30FPS_10BIT_2TO1_VC_MODE == img_mode) {
            sc431hai_vc_wdr_2t1_4m30_10bit_init(cis);
        } else {
        }
    } else {
        sc431hai_linear_4m30_10bit_init(cis);
    }

    sensor_state->init = TD_TRUE;

    return;
}

static void cmos_isp_exit(ot_vi_pipe vi_pipe)
{
    td_s32 ret;
    cis_info *cis = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    ret = cis_i2c_exit(cis);
    if (ret != TD_SUCCESS) {
        isp_err_trace("SC500AI exit failed!\n");
    }

    return;
}

static td_void cmos_isp_global_init(ot_vi_pipe vi_pipe)
{
    ot_isp_sns_state *sns_state = TD_NULL;
    cis_info *cis = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    sns_state->init      = TD_FALSE;
    sns_state->sync_init = TD_FALSE;
    sns_state->img_mode  = SC431HAI_4M_30FPS_10BIT_LINEAR_MODE;
    sns_state->wdr_mode  = OT_WDR_MODE_NONE;
    sns_state->fl_std    = SC431HAI_VMAX_LINEAR << 1;
    sns_state->fl[0]     = SC431HAI_VMAX_LINEAR << 1;
    sns_state->fl[1]     = SC431HAI_VMAX_LINEAR << 1;

    (td_void)memset_s(&sns_state->regs_info[0], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));
    (td_void)memset_s(&sns_state->regs_info[1], sizeof(ot_isp_sns_regs_info), 0, sizeof(ot_isp_sns_regs_info));

    return;
}

static td_void cmos_isp_set_pixel_detect(ot_vi_pipe vi_pipe, td_bool enable)
{
    td_u32 full_lines_5fps;
    ot_isp_sns_state *sns_state = TD_NULL;
    cis_info *cis = TD_NULL;
    cis_i2c *i2c = TD_NULL;

    sns_check_pipe_void_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_void_return(cis);

    i2c = &cis->i2c;
    sns_state = cis->sns_state;
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (sns_state->img_mode == SC431HAI_4M_30FPS_10BIT_LINEAR_MODE) {
            full_lines_5fps = (SC431HAI_VMAX_LINEAR * STANDARD_FPS) / 5; /* 30fps, 5fps */
        } else {
            return;
        }
    }

    if (enable) { /* setup for ISP pixel calibration mode */
        cis_write_reg(i2c, SC431HAI_AGAIN_L_ADDR, 0x40);
        cis_write_reg(i2c, SC431HAI_AGAIN_H_ADDR, 0x03);

        cis_write_reg(i2c, SC431HAI_DGAIN_H_ADDR, 0x00);
        cis_write_reg(i2c, SC431HAI_DGAIN_L_ADDR, 0x80);

        cis_write_reg(i2c, SC431HAI_VMAX_L_ADDR, low_8bits(full_lines_5fps));
        cis_write_reg(i2c, SC431HAI_VMAX_H_ADDR, high_8bits(full_lines_5fps));

        cis_write_reg(i2c, SC431HAI_EXPO_L_ADDR, lower_4bits(full_lines_5fps - 10)); /* sub 10 */
        cis_write_reg(i2c, SC431HAI_EXPO_M_ADDR, higher_8bits(full_lines_5fps - 10)); /* sub 10 */
        cis_write_reg(i2c, SC431HAI_EXPO_H_ADDR, higher_4bits(full_lines_5fps - 10)); /* sub 10 */
    } else { /* setup for ISP 'normal mode' */
        sns_state->fl_std = (sns_state->fl_std > SC431HAI_FULL_LINES_MAX_LINEAR) ?
            SC431HAI_FULL_LINES_MAX_LINEAR : sns_state->fl_std;
        cis_write_reg(i2c, SC431HAI_VMAX_L_ADDR, low_8bits(sns_state->fl_std));
        cis_write_reg(i2c, SC431HAI_VMAX_H_ADDR, high_8bits(sns_state->fl_std));
        sns_state->sync_init = TD_FALSE;
    }
    return;
}

static td_void cmos_init_sensor_exp_function(ot_isp_sns_exp_func *sensor_exp_func)
{
    (td_void)memset_s(sensor_exp_func, sizeof(ot_isp_sns_exp_func), 0, sizeof(ot_isp_sns_exp_func));

    sensor_exp_func->pfn_cmos_sns_init              = cmos_isp_init;
    sensor_exp_func->pfn_cmos_sns_exit              = cmos_isp_exit;
    sensor_exp_func->pfn_cmos_sns_global_init       = cmos_isp_global_init;
    sensor_exp_func->pfn_cmos_set_image_mode        = cmos_isp_set_image_mode;
    sensor_exp_func->pfn_cmos_set_wdr_mode          = cmos_isp_set_wdr_mode;
    sensor_exp_func->pfn_cmos_get_isp_default       = cmos_isp_get_default;
    sensor_exp_func->pfn_cmos_get_isp_black_level   = cmos_isp_get_black_level;
    sensor_exp_func->pfn_cmos_get_blc_clamp_info    = cmos_isp_get_blc_clamp_info;
    sensor_exp_func->pfn_cmos_set_pixel_detect      = cmos_isp_set_pixel_detect;
    sensor_exp_func->pfn_cmos_get_sns_reg_info      = cmos_isp_get_sns_regs_info;
}

static td_s32 cmos_register_callback(ot_vi_pipe vi_pipe, ot_isp_3a_alg_lib *ae_lib, ot_isp_3a_alg_lib *awb_lib)
{
    td_s32 ret;
    cis_register reg = {0};
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(ae_lib);
    sns_check_pointer_return(awb_lib);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    cis->pipe = vi_pipe;

    reg.ae_lib = ae_lib;
    reg.awb_lib = awb_lib;

    cmos_init_sensor_exp_function(&reg.isp_register.sns_exp);
    cmos_init_ae_exp_function(&reg.ae_register.sns_exp);
    cmos_init_awb_exp_function(&reg.awb_register.sns_exp);

    ret = cis_register_callback(cis, &reg);
    if (ret != TD_SUCCESS) {
        isp_err_trace("cis_register_callback failed!\n");
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 cmos_unregister_callback(ot_vi_pipe vi_pipe, ot_isp_3a_alg_lib *ae_lib, ot_isp_3a_alg_lib *awb_lib)
{
    td_s32 ret;
    cis_register reg = {0};
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(ae_lib);
    sns_check_pointer_return(awb_lib);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    reg.ae_lib = ae_lib;
    reg.awb_lib = awb_lib;
    ret = cis_unregister_callback(cis, &reg);
    if (ret != TD_SUCCESS) {
        isp_err_trace("cis_register_callback failed!\n");
        return ret;
    }

    return TD_SUCCESS;
}

static void cmos_standby(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

static void cmos_restart(ot_vi_pipe vi_pipe)
{
    ot_unused(vi_pipe);
    return;
}

static td_s32 cmos_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data)
{
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    sns_check_return(cis_write_reg(&cis->i2c, addr, data));

    return TD_SUCCESS;
}

static td_s32 cmos_read_register(ot_vi_pipe vi_pipe, td_u32 addr)
{
    ot_unused(vi_pipe);
    ot_unused(addr);
    return TD_SUCCESS;
}

static td_s32 cmos_set_bus_info(ot_vi_pipe vi_pipe, ot_isp_sns_commbus sns_bus_info)
{
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    cis->bus_info.i2c_dev = sns_bus_info.i2c_dev;

    return TD_SUCCESS;
}

static td_s32 cmos_set_init(ot_vi_pipe vi_pipe, ot_isp_init_attr *init_attr)
{
    cis_info *cis = TD_NULL;

    sns_check_pipe_return(vi_pipe);
    sns_check_pointer_return(init_attr);

    cis = cmos_get_info(vi_pipe);
    sns_check_pointer_return(cis);

    cis_init_attr(cis, init_attr);

    return TD_SUCCESS;
}

ot_isp_sns_obj g_sns_sc431hai_obj = {
    .pfn_register_callback     = cmos_register_callback,
    .pfn_un_register_callback  = cmos_unregister_callback,
    .pfn_standby               = cmos_standby,
    .pfn_restart               = cmos_restart,
    .pfn_mirror_flip           = TD_NULL,
    .pfn_set_blc_clamp         = TD_NULL,
    .pfn_write_reg             = cmos_write_register,
    .pfn_read_reg              = cmos_read_register,
    .pfn_set_bus_info          = cmos_set_bus_info,
    .pfn_set_init              = cmos_set_init
};

ot_isp_sns_obj *sc431hai_get_obj(td_void)
{
    return &g_sns_sc431hai_obj;
}
