/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_gamma.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_ext_config.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include <math.h>
#include "isp_math_utils.h"

typedef struct {
    hi_bool enable;
    hi_bool lut_update;

    hi_u8   step_lut[GAMMA_OUTSEG_NUMBER];
    hi_u16  in_seg_lut[GAMMA_OUTSEG_NUMBER];
    hi_u16  pos_lut[GAMMA_OUTSEG_NUMBER];

    hi_u16  *gamma_lut;
    hi_u16  *ext_gamma_lut;
} isp_gamma;

isp_gamma *g_gamma_ctx[ISP_MAX_PIPE_NUM] = {HI_NULL};

#define gamma_get_ctx(dev, ctx)   ((ctx) = g_gamma_ctx[dev])
#define gamma_set_ctx(dev, ctx)   (g_gamma_ctx[dev] = (ctx))
#define gamma_reset_ctx(dev)      (g_gamma_ctx[dev] = HI_NULL)
#define GAMMA_STEP_SHFT     7
#define GAMMA_LUT_MAX_VALUE 4096

static const hi_u8  g_non_uni_step_lut[GAMMA_OUTSEG_NUMBER]   = {2, 2, 3, 4, 4, 4, 4, 5};
static const hi_u16 g_non_uni_in_seg_lut[GAMMA_OUTSEG_NUMBER] = {16, 32, 48, 64, 96, 160, 224, 256};

/* dafault gamma curve */
static const hi_u16 g_lin_mode_gamma[GAMMA_EXT_NODE_NUMBER] = {
    0,    30,   60,   90,   120,  145,  170,  195,  220,  243,  265,  288,  310,  330,  350,  370,
    390,  410,  430,  450,  470,  488,  505,  523,  540,  558,  575,  593,  610,  625,  640,  655,
    670,  685,  700,  715,  730,  744,  758,  772,  786,  800,  814,  828,  842,  855,  868,  881,
    894,  907,  919,  932,  944,  957,  969,  982,  994,  1008, 1022, 1036, 1050, 1062, 1073, 1085,
    1096, 1107, 1117, 1128, 1138, 1148, 1158, 1168, 1178, 1188, 1198, 1208, 1218, 1227, 1236, 1245,
    1254, 1261, 1267, 1274, 1280, 1289, 1297, 1306, 1314, 1322, 1330, 1338, 1346, 1354, 1362, 1370,
    1378, 1386, 1393, 1401, 1408, 1416, 1423, 1431, 1438, 1445, 1453, 1460, 1467, 1474, 1480, 1487,
    1493, 1500, 1506, 1513, 1519, 1525, 1531, 1537, 1543, 1549, 1556, 1562, 1568, 1574, 1580, 1586,
    1592, 1598, 1604, 1609, 1615, 1621, 1627, 1632, 1638, 1644, 1650, 1655, 1661, 1667, 1672, 1678,
    1683, 1689, 1694, 1700, 1705, 1710, 1716, 1721, 1726, 1732, 1737, 1743, 1748, 1753, 1759, 1764,
    1769, 1774, 1779, 1784, 1789, 1794, 1800, 1805, 1810, 1815, 1820, 1825, 1830, 1835, 1840, 1844,
    1849, 1854, 1859, 1864, 1869, 1874, 1879, 1883, 1888, 1893, 1898, 1902, 1907, 1912, 1917, 1921,
    1926, 1931, 1936, 1940, 1945, 1950, 1954, 1959, 1963, 1968, 1972, 1977, 1981, 1986, 1990, 1995,
    1999, 2004, 2008, 2013, 2017, 2021, 2026, 2030, 2034, 2039, 2043, 2048, 2052, 2056, 2061, 2065,
    2069, 2073, 2078, 2082, 2086, 2090, 2094, 2098, 2102, 2106, 2111, 2115, 2119, 2123, 2128, 2132,
    2136, 2140, 2144, 2148, 2152, 2156, 2160, 2164, 2168, 2172, 2176, 2180, 2184, 2188, 2192, 2196,
    2200, 2204, 2208, 2212, 2216, 2220, 2224, 2227, 2231, 2235, 2239, 2243, 2247, 2251, 2255, 2258,
    2262, 2266, 2270, 2273, 2277, 2281, 2285, 2288, 2292, 2296, 2300, 2303, 2307, 2311, 2315, 2318,
    2322, 2326, 2330, 2333, 2337, 2341, 2344, 2348, 2351, 2355, 2359, 2362, 2366, 2370, 2373, 2377,
    2380, 2384, 2387, 2391, 2394, 2398, 2401, 2405, 2408, 2412, 2415, 2419, 2422, 2426, 2429, 2433,
    2436, 2440, 2443, 2447, 2450, 2454, 2457, 2461, 2464, 2467, 2471, 2474, 2477, 2481, 2484, 2488,
    2491, 2494, 2498, 2501, 2504, 2508, 2511, 2515, 2518, 2521, 2525, 2528, 2531, 2534, 2538, 2541,
    2544, 2547, 2551, 2554, 2557, 2560, 2564, 2567, 2570, 2573, 2577, 2580, 2583, 2586, 2590, 2593,
    2596, 2599, 2603, 2606, 2609, 2612, 2615, 2618, 2621, 2624, 2628, 2631, 2634, 2637, 2640, 2643,
    2646, 2649, 2653, 2656, 2659, 2662, 2665, 2668, 2671, 2674, 2677, 2680, 2683, 2686, 2690, 2693,
    2696, 2699, 2702, 2705, 2708, 2711, 2714, 2717, 2720, 2723, 2726, 2729, 2732, 2735, 2738, 2741,
    2744, 2747, 2750, 2753, 2756, 2759, 2762, 2764, 2767, 2770, 2773, 2776, 2779, 2782, 2785, 2788,
    2791, 2794, 2797, 2799, 2802, 2805, 2808, 2811, 2814, 2817, 2820, 2822, 2825, 2828, 2831, 2834,
    2837, 2840, 2843, 2845, 2848, 2851, 2854, 2856, 2859, 2862, 2865, 2868, 2871, 2874, 2877, 2879,
    2882, 2885, 2888, 2890, 2893, 2896, 2899, 2901, 2904, 2907, 2910, 2912, 2915, 2918, 2921, 2923,
    2926, 2929, 2932, 2934, 2937, 2940, 2943, 2945, 2948, 2951, 2954, 2956, 2959, 2962, 2964, 2967,
    2969, 2972, 2975, 2977, 2980, 2983, 2986, 2988, 2991, 2994, 2996, 2999, 3001, 3004, 3007, 3009,
    3012, 3015, 3018, 3020, 3023, 3026, 3028, 3031, 3033, 3036, 3038, 3041, 3043, 3046, 3049, 3051,
    3054, 3057, 3059, 3062, 3064, 3067, 3069, 3072, 3074, 3077, 3080, 3082, 3085, 3088, 3090, 3093,
    3095, 3098, 3100, 3103, 3105, 3108, 3110, 3113, 3115, 3118, 3120, 3123, 3125, 3128, 3130, 3133,
    3135, 3138, 3140, 3143, 3145, 3148, 3150, 3153, 3155, 3158, 3160, 3163, 3165, 3168, 3170, 3173,
    3175, 3178, 3180, 3183, 3185, 3187, 3190, 3192, 3194, 3197, 3199, 3202, 3204, 3207, 3209, 3212,
    3214, 3217, 3219, 3222, 3224, 3226, 3229, 3231, 3233, 3236, 3238, 3241, 3243, 3245, 3248, 3250,
    3252, 3255, 3257, 3260, 3262, 3264, 3267, 3269, 3271, 3274, 3276, 3279, 3281, 3283, 3286, 3288,
    3290, 3293, 3295, 3298, 3300, 3302, 3305, 3307, 3309, 3311, 3314, 3316, 3318, 3320, 3323, 3325,
    3327, 3330, 3332, 3335, 3337, 3339, 3342, 3344, 3346, 3348, 3351, 3353, 3355, 3357, 3360, 3362,
    3364, 3366, 3369, 3371, 3373, 3375, 3378, 3380, 3382, 3384, 3387, 3389, 3391, 3393, 3396, 3398,
    3400, 3402, 3405, 3407, 3409, 3411, 3414, 3416, 3418, 3420, 3423, 3425, 3427, 3429, 3432, 3434,
    3436, 3438, 3441, 3443, 3445, 3447, 3450, 3452, 3454, 3456, 3459, 3461, 3463, 3465, 3467, 3469,
    3471, 3473, 3476, 3478, 3480, 3482, 3485, 3487, 3489, 3491, 3494, 3496, 3498, 3500, 3502, 3504,
    3506, 3508, 3511, 3513, 3515, 3517, 3519, 3521, 3523, 3525, 3528, 3530, 3532, 3534, 3536, 3538,
    3540, 3542, 3545, 3547, 3549, 3551, 3553, 3555, 3557, 3559, 3562, 3564, 3566, 3568, 3570, 3572,
    3574, 3576, 3579, 3581, 3583, 3585, 3587, 3589, 3591, 3593, 3596, 3598, 3600, 3602, 3604, 3606,
    3608, 3610, 3612, 3614, 3616, 3618, 3620, 3622, 3624, 3626, 3629, 3631, 3633, 3635, 3637, 3639,
    3641, 3643, 3645, 3647, 3649, 3651, 3653, 3655, 3657, 3659, 3661, 3663, 3665, 3667, 3670, 3672,
    3674, 3676, 3678, 3680, 3682, 3684, 3686, 3688, 3690, 3692, 3694, 3696, 3698, 3700, 3702, 3704,
    3706, 3708, 3710, 3712, 3714, 3716, 3718, 3720, 3722, 3724, 3726, 3728, 3730, 3732, 3734, 3736,
    3738, 3740, 3742, 3744, 3746, 3748, 3750, 3752, 3754, 3756, 3758, 3760, 3762, 3764, 3766, 3767,
    3769, 3771, 3773, 3775, 3777, 3779, 3781, 3783, 3785, 3787, 3789, 3791, 3793, 3795, 3797, 3799,
    3801, 3803, 3805, 3806, 3808, 3810, 3812, 3814, 3816, 3818, 3820, 3822, 3824, 3826, 3828, 3830,
    3832, 3834, 3836, 3837, 3839, 3841, 3843, 3845, 3847, 3849, 3851, 3853, 3855, 3857, 3859, 3860,
    3862, 3864, 3866, 3868, 3870, 3872, 3874, 3875, 3877, 3879, 3881, 3883, 3885, 3887, 3889, 3890,
    3892, 3894, 3896, 3898, 3900, 3902, 3904, 3905, 3907, 3909, 3911, 3913, 3915, 3917, 3919, 3920,
    3922, 3924, 3926, 3928, 3930, 3932, 3934, 3935, 3937, 3939, 3941, 3943, 3945, 3947, 3949, 3950,
    3952, 3954, 3956, 3957, 3959, 3961, 3963, 3965, 3967, 3969, 3971, 3972, 3974, 3976, 3978, 3979,
    3981, 3983, 3985, 3987, 3989, 3991, 3993, 3994, 3996, 3998, 4000, 4001, 4003, 4005, 4007, 4008,
    4010, 4012, 4014, 4016, 4018, 4020, 4022, 4023, 4025, 4027, 4029, 4030, 4032, 4034, 4036, 4037,
    4039, 4041, 4043, 4044, 4046, 4048, 4050, 4052, 4054, 4056, 4058, 4059, 4061, 4063, 4065, 4066,
    4068, 4070, 4072, 4073, 4075, 4077, 4079, 4080, 4082, 4084, 4086, 4087, 4089, 4091, 4092, 4094, 4095
};

static const hi_u16 g_wdr_mode_gamma[GAMMA_EXT_NODE_NUMBER] = {
    0,    3,    6,    8,    11,   14,   17,   20,    23,   26,   29,   32,   35,   38,   41,   44,
    47,   50,   53,   56,   59,   62,   65,   68,    71,   74,   77,   79,   82,   85,   88,   91,
    94,   97,   100,  103,  106,  109,  112,  114,  117,  120,  123,  126,  129,  132,  135,  138,
    141,  144,  147,  149,  152,  155,  158,  161,  164,  167,  170,  172,  175,  178,  181,  184,
    187,  190,  193,  195,  198,  201,  204,  207,  210,  213,  216,  218,  221,  224,  227,  230,
    233,  236,  239,  241,  244,  247,  250,  253,  256,  259,  262,  264,  267,  270,  273,  276,
    279,  282,  285,  287,  290,  293,  296,  299,  302,  305,  308,  310,  313,  316,  319,  322,
    325,  328,  331,  333,  336,  339,  342,  345,  348,  351,  354,  356,  359,  362,  365,  368,
    371,  374,  377,  380,  383,  386,  389,  391,  394,  397,  400,  403,  406,  409,  412,  415,
    418,  421,  424,  426,  429,  432,  435,  438,  441,  444,  447,  450,  453,  456,  459,  462,
    465,  468,  471,  474,  477,  480,  483,  486,  489,  492,  495,  498,  501,  504,  507,  510,
    513,  516,  519,  522,  525,  528,  531,  534,  537,  540,  543,  546,  549,  552,  555,  558,
    561,  564,  568,  571,  574,  577,  580,  583,  586,  589,  592,  595,  598,  601,  605,  608,
    611,  614,  617,  620,  623,  626,  630,  633,  636,  639,  643,  646,  649,  652,  655,  658,
    661,  664,  668,  671,  674,  677,  681,  684,  687,  690,  694,  697,  700,  703,  707,  710,
    713,  716,  720,  723,  726,  730,  733,  737,  740,  743,  747,  750,  753,  757,  760,  764,
    767,  770,  774,  777,  780,  784,  787,  791,  794,  797,  801,  804,  807,  811,  814,  818,
    821,  825,  828,  832,  835,  838,  842,  845,  848,  852,  855,  859,  862,  866,  869,  873,
    876,  880,  883,  887,  890,  894,  897,  901,  904,  908,  911,  915,  918,  922,  925,  929,
    932,  936,  939,  943,  946,  950,  954,  957,  961,  965,  968,  972,  975,  979,  982,  986,
    989,  993,  997,  1000, 1004, 1008, 1011, 1015, 1018, 1022, 1026, 1029, 1033, 1037, 1040, 1044,
    1047, 1051, 1055, 1058, 1062, 1066, 1069, 1073, 1076, 1080, 1084, 1087, 1091, 1095, 1099, 1102,
    1106, 1110, 1113, 1117, 1120, 1124, 1128, 1131, 1135, 1139, 1143, 1146, 1150, 1154, 1158, 1161,
    1165, 1169, 1173, 1176, 1180, 1184, 1188, 1191, 1195, 1199, 1203, 1206, 1210, 1214, 1218, 1221,
    1225, 1229, 1233, 1236, 1240, 1244, 1248, 1251, 1255, 1259, 1263, 1266, 1270, 1274, 1278, 1281,
    1285, 1289, 1293, 1297, 1301, 1305, 1309, 1312, 1316, 1320, 1324, 1327, 1331, 1335, 1339, 1343,
    1347, 1351, 1355, 1358, 1362, 1366, 1370, 1373, 1377, 1381, 1385, 1389, 1393, 1397, 1401, 1404,
    1408, 1412, 1416, 1420, 1424, 1428, 1432, 1435, 1439, 1443, 1447, 1451, 1455, 1459, 1463, 1467,
    1471, 1475, 1479, 1482, 1486, 1490, 1494, 1498, 1502, 1506, 1510, 1514, 1518, 1522, 1526, 1529,
    1533, 1537, 1541, 1545, 1549, 1553, 1557, 1561, 1565, 1569, 1573, 1577, 1581, 1585, 1589, 1593,
    1597, 1601, 1605, 1609, 1613, 1617, 1621, 1624, 1628, 1632, 1636, 1640, 1644, 1648, 1652, 1656,
    1660, 1664, 1668, 1672, 1676, 1680, 1684, 1688, 1692, 1696, 1700, 1704, 1708, 1712, 1717, 1721,
    1725, 1729, 1733, 1737, 1741, 1745, 1749, 1753, 1757, 1761, 1765, 1769, 1773, 1777, 1781, 1785,
    1789, 1793, 1797, 1801, 1805, 1809, 1813, 1817, 1821, 1825, 1830, 1834, 1838, 1842, 1846, 1850,
    1854, 1858, 1862, 1866, 1870, 1874, 1879, 1883, 1887, 1891, 1895, 1899, 1903, 1907, 1911, 1915,
    1919, 1923, 1928, 1932, 1936, 1940, 1944, 1948, 1952, 1956, 1961, 1965, 1969, 1973, 1977, 1981,
    1985, 1989, 1994, 1998, 2002, 2006, 2010, 2014, 2018, 2022, 2027, 2031, 2035, 2039, 2044, 2048,
    2052, 2056, 2060, 2064, 2068, 2072, 2077, 2081, 2085, 2089, 2094, 2098, 2102, 2106, 2111, 2115,
    2119, 2123, 2128, 2132, 2136, 2140, 2144, 2148, 2152, 2156, 2161, 2165, 2169, 2173, 2178, 2182,
    2186, 2190, 2195, 2199, 2203, 2207, 2212, 2216, 2220, 2224, 2229, 2233, 2237, 2241, 2246, 2250,
    2254, 2259, 2263, 2268, 2272, 2276, 2281, 2285, 2289, 2293, 2298, 2302, 2306, 2310, 2315, 2319,
    2323, 2327, 2332, 2336, 2340, 2345, 2349, 2354, 2358, 2362, 2367, 2371, 2375, 2380, 2384, 2389,
    2393, 2397, 2402, 2406, 2410, 2415, 2419, 2424, 2428, 2432, 2437, 2441, 2445, 2450, 2454, 2459,
    2463, 2467, 2472, 2476, 2480, 2485, 2489, 2494, 2498, 2503, 2507, 2512, 2516, 2520, 2525, 2529,
    2533, 2538, 2542, 2547, 2551, 2556, 2560, 2565, 2569, 2574, 2578, 2583, 2587, 2592, 2596, 2601,
    2605, 2610, 2614, 2619, 2623, 2628, 2632, 2637, 2641, 2646, 2650, 2655, 2659, 2664, 2668, 2673,
    2677, 2682, 2686, 2691, 2695, 2700, 2704, 2709, 2713, 2718, 2723, 2727, 2732, 2737, 2741, 2746,
    2750, 2755, 2759, 2764, 2768, 2773, 2778, 2782, 2787, 2792, 2796, 2801, 2805, 2810, 2815, 2819,
    2824, 2829, 2833, 2838, 2842, 2847, 2852, 2856, 2861, 2866, 2870, 2875, 2879, 2884, 2889, 2893,
    2898, 2903, 2908, 2912, 2917, 2922, 2927, 2931, 2936, 2941, 2946, 2950, 2955, 2960, 2965, 2969,
    2974, 2979, 2984, 2988, 2993, 2998, 3003, 3008, 3013, 3018, 3023, 3027, 3032, 3037, 3042, 3046,
    3051, 3056, 3061, 3066, 3071, 3076, 3081, 3085, 3090, 3095, 3100, 3105, 3110, 3115, 3120, 3124,
    3129, 3134, 3139, 3144, 3149, 3154, 3159, 3163, 3168, 3173, 3178, 3183, 3188, 3193, 3198, 3203,
    3208, 3213, 3218, 3223, 3228, 3233, 3238, 3243, 3248, 3253, 3258, 3263, 3268, 3273, 3278, 3283,
    3288, 3293, 3298, 3303, 3308, 3313, 3318, 3323, 3328, 3333, 3338, 3343, 3348, 3353, 3358, 3363,
    3368, 3373, 3378, 3383, 3388, 3393, 3398, 3403, 3408, 3413, 3418, 3423, 3428, 3433, 3438, 3443,
    3448, 3453, 3458, 3463, 3468, 3473, 3479, 3484, 3489, 3494, 3499, 3504, 3509, 3514, 3519, 3524,
    3529, 3534, 3539, 3544, 3549, 3554, 3560, 3565, 3570, 3575, 3580, 3585, 3590, 3595, 3600, 3605,
    3610, 3615, 3621, 3626, 3631, 3636, 3641, 3646, 3651, 3656, 3661, 3666, 3671, 3676, 3682, 3687,
    3692, 3697, 3702, 3707, 3712, 3717, 3722, 3727, 3732, 3737, 3742, 3747, 3752, 3757, 3763, 3768,
    3773, 3778, 3783, 3788, 3793, 3798, 3803, 3808, 3813, 3818, 3824, 3829, 3834, 3839, 3844, 3849,
    3854, 3859, 3864, 3869, 3874, 3879, 3884, 3889, 3894, 3899, 3904, 3909, 3914, 3919, 3924, 3929,
    3934, 3939, 3945, 3950, 3955, 3960, 3965, 3970, 3975, 3980, 3985, 3990, 3995, 4000, 4005, 4010,
    4015, 4020, 4025, 4030, 4035, 4040, 4045, 4050, 4055, 4060, 4065, 4070, 4075, 4080, 4085, 4090, 4095

};

hi_s32 gamma_ctx_init(hi_vi_pipe vi_pipe)
{
    isp_gamma *gamma = HI_NULL;

    gamma_get_ctx(vi_pipe, gamma);

    if (gamma == HI_NULL) {
        gamma = (isp_gamma *)isp_malloc(sizeof(isp_gamma));
        if (gamma == HI_NULL) {
            isp_err_trace("isp[%d] gamma_ctx malloc memory failed!\n", vi_pipe);
            return HI_ERR_ISP_NOMEM;
        }

        memset_s(gamma, sizeof(isp_gamma), 0, sizeof(isp_gamma));
    }

    gamma_set_ctx(vi_pipe, gamma);

    return HI_SUCCESS;
}

hi_void gamma_ctx_exit(hi_vi_pipe vi_pipe)
{
    isp_gamma *gamma = HI_NULL;

    gamma_get_ctx(vi_pipe, gamma);
    isp_free(gamma);
    gamma_reset_ctx(vi_pipe);
}

hi_void gamma_get_pos(isp_gamma *gamma, hi_u16 *actual_lut, hi_u16 *input_lut)
{
    /* this function is used to generate pos value from in_seg and step we entered */
    hi_u16 i, j;
    hi_u16 input_seg_num[GAMMA_OUTSEG_NUMBER];
    hi_u16 point_lut[GAMMA_OUTSEG_NUMBER];
    hi_u16 step_lut[GAMMA_OUTSEG_NUMBER];
    hi_u16 lut_pos[GAMMA_REG_NODE_NUM];

    hi_u16 seg_start, seg_end, seg_pos_start;

    /* initiate */
    point_lut[0]     = 0;
    input_seg_num[0] = gamma->in_seg_lut[0];
    step_lut[0]      = 1 << gamma->step_lut[0];
    seg_start        = 0;
    seg_end          = (seg_start + input_seg_num[0]) - 1;
    seg_pos_start    = 0;

    for (i = 1; i < GAMMA_OUTSEG_NUMBER; i++) {
        input_seg_num[i] = gamma->in_seg_lut[i] - gamma->in_seg_lut[i - 1];
        gamma->step_lut[i - 1] = CLIP_MAX(gamma->step_lut[i - 1], GAMMA_STEP_SHFT);
        point_lut[i] = point_lut[i - 1] + input_seg_num[i - 1] * (1 << gamma->step_lut[i - 1]);
        step_lut[i] = 1 << gamma->step_lut[i];
        gamma->pos_lut[i - 1] = point_lut[i];
    }

    gamma->pos_lut[GAMMA_OUTSEG_NUMBER - 1] = point_lut[GAMMA_OUTSEG_NUMBER - 1] +
                              input_seg_num[GAMMA_OUTSEG_NUMBER - 1] * (1 << gamma->step_lut[GAMMA_OUTSEG_NUMBER - 1]);

    /* After get Pos value, generate the actual lut from the input one */
    for (i = seg_start; i <= seg_end; i++) {
        lut_pos[i] = seg_pos_start + i * step_lut[0];
    }

    for (j = 1; j < GAMMA_OUTSEG_NUMBER; j++) {
        /* update seg_start, seg_end and seg_pos */
        seg_start = seg_end + 1; /* move to the next */
        seg_end   = (seg_start + input_seg_num[j]) - 1;
        seg_pos_start = seg_pos_start + input_seg_num[j - 1] * step_lut[j - 1];
        for (i = seg_start; i <= seg_end; i++) {
            lut_pos[i] = seg_pos_start + (i - seg_start) * step_lut[j];
        }
    }

    lut_pos[GAMMA_REG_NODE_NUM - 1] = GAMMA_LUT_MAX_VALUE;

    for (i = 0; i < GAMMA_REG_NODE_NUM; i++) {
        lut_pos[i] = lut_pos[i] / 4; /* algorithm converting, divide by 4 */
        actual_lut[i] = input_lut[lut_pos[i]];
    }

    actual_lut[GAMMA_REG_NODE_NUM - 1] = input_lut[GAMMA_EXT_NODE_NUMBER - 1];
}

static hi_void gamma_usr_regs_init(isp_gamma_usr_cfg *usr_reg_cfg, hi_u16 *gamma_value, isp_gamma *gamma_st)
{
    hi_s32 ret;
    usr_reg_cfg->resh                = HI_TRUE;
    usr_reg_cfg->gamma_lut_update_en = HI_TRUE;
    usr_reg_cfg->update_index        = 1;
    usr_reg_cfg->buf_id              = 0;

    memcpy_s(usr_reg_cfg->gamma_in_seg, GAMMA_OUTSEG_NUMBER * sizeof(hi_u16),
             gamma_st->in_seg_lut, GAMMA_OUTSEG_NUMBER * sizeof(hi_u16));
    memcpy_s(usr_reg_cfg->gamma_pos, GAMMA_OUTSEG_NUMBER * sizeof(hi_u16),
             gamma_st->pos_lut, GAMMA_OUTSEG_NUMBER * sizeof(hi_u16));
    memcpy_s(usr_reg_cfg->gamma_step, GAMMA_OUTSEG_NUMBER * sizeof(hi_u8),
             gamma_st->step_lut, GAMMA_OUTSEG_NUMBER * sizeof(hi_u8));
    ret = memcpy_s(usr_reg_cfg->gamma_lut, GAMMA_REG_NODE_NUM   * sizeof(hi_u16),
                   gamma_value, GAMMA_REG_NODE_NUM * sizeof(hi_u16));
    if (ret != 0) {
        isp_err_trace("ISP init gamma lut failed!\n");
    }
}

static hi_void gamma_regs_initialize(hi_vi_pipe vi_pipe, isp_reg_cfg *reg_cfg)
{
    hi_u16 i;
    isp_gamma *gamma_ctx = HI_NULL;

    gamma_get_ctx(vi_pipe, gamma_ctx);

    for (i = 0; i < reg_cfg->cfg_num; i++) {
        gamma_usr_regs_init(&reg_cfg->alg_reg_cfg[i].gamma_cfg.usr_reg_cfg, gamma_ctx->gamma_lut, gamma_ctx);
        reg_cfg->alg_reg_cfg[i].gamma_cfg.gamma_en    = HI_TRUE;
        reg_cfg->alg_reg_cfg[i].gamma_cfg.lut2_stt_en = HI_TRUE;
    }

    reg_cfg->cfg_key.bit1_gamma_cfg = 1;
}

static hi_void gamma_ext_regs_initialize(hi_vi_pipe vi_pipe)
{
    /* only extern reg is gamma curve type */
    hi_ext_system_gamma_en_write(vi_pipe, HI_TRUE);
    hi_ext_system_gamma_lut_update_write(vi_pipe, HI_FALSE);
    hi_ext_system_gamma_curve_type_write(vi_pipe, HI_EXT_SYSTEM_GAMMA_CURVE_TYPE_DEFAULT);
}

static hi_void gamma_read_ext_regs(hi_vi_pipe vi_pipe)
{
    hi_u16 i;

    isp_gamma *gamma_ctx = HI_NULL;

    gamma_get_ctx(vi_pipe, gamma_ctx);

    gamma_ctx->lut_update = hi_ext_system_gamma_lut_update_read(vi_pipe);
    hi_ext_system_gamma_lut_update_write(vi_pipe, HI_FALSE);

    if (gamma_ctx->lut_update) {
        for (i = 0; i < GAMMA_EXT_NODE_NUMBER; i++) {
            gamma_ctx->ext_gamma_lut[i] = hi_ext_system_gamma_lut_read(vi_pipe, i);
        }
    }

    return;
}

static hi_s32 gamma_check_cmos_param(hi_vi_pipe vi_pipe, const hi_isp_cmos_gamma *cmos_gamma)
{
    hi_u16 i;

    for (i = 0; i < GAMMA_EXT_NODE_NUMBER; i++) {
        if (cmos_gamma->gamma_lut[i] > 0xFFF) {
            isp_err_trace("invalid gamma[%d]!\n", i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

static hi_s32 gamma_initialize(hi_vi_pipe vi_pipe)
{
    hi_u8  wdr_mode;
    hi_u16 i;
    hi_s32 ret;

    isp_usr_ctx          *isp_ctx   = HI_NULL;
    isp_gamma        *gamma_ctx = HI_NULL;
    hi_isp_cmos_default *sns_dft   = HI_NULL;

    isp_sensor_get_default(vi_pipe, &sns_dft);
    gamma_get_ctx(vi_pipe, gamma_ctx);
    isp_get_ctx(vi_pipe, isp_ctx);

    if (gamma_ctx->gamma_lut == HI_NULL) {
        gamma_ctx->gamma_lut = (hi_u16 *)isp_malloc(GAMMA_REG_NODE_NUM * sizeof(hi_u16));

        if (gamma_ctx->gamma_lut == HI_NULL) {
            isp_err_trace("malloc gamma_ctx->gamma_lut failed!\n");
            return HI_FAILURE;
        }
    }

    if (gamma_ctx->ext_gamma_lut == HI_NULL) {
        gamma_ctx->ext_gamma_lut = (hi_u16 *)isp_malloc(GAMMA_EXT_NODE_NUMBER * sizeof(hi_u16));

        if (gamma_ctx->ext_gamma_lut == HI_NULL) {
            isp_err_trace("malloc gamma_ctx->ext_gamma_lut failed!\n");
            isp_free(gamma_ctx->gamma_lut);
            return HI_FAILURE;
        }
    }

    wdr_mode  = isp_ctx->sns_wdr_mode;

    /* Read from CMOS */
    if (sns_dft->key.bit1_gamma) {
        isp_check_pointer_return(sns_dft->gamma);

        ret = gamma_check_cmos_param(vi_pipe, sns_dft->gamma);
        if (ret != HI_SUCCESS) {
            return ret;
        }

        memcpy_s(gamma_ctx->ext_gamma_lut, GAMMA_EXT_NODE_NUMBER * sizeof(hi_u16),
                 sns_dft->gamma->gamma_lut, GAMMA_EXT_NODE_NUMBER * sizeof(hi_u16));

        hi_ext_system_gamma_curve_type_write(vi_pipe, ISP_GAMMA_CURVE_USER_DEFINE);
    } else { /* Read from firmware */
        if (is_linear_mode(wdr_mode)) {
            memcpy_s(gamma_ctx->ext_gamma_lut, GAMMA_EXT_NODE_NUMBER * sizeof(hi_u16),
                     g_lin_mode_gamma, GAMMA_EXT_NODE_NUMBER * sizeof(hi_u16));
        } else {
            memcpy_s(gamma_ctx->ext_gamma_lut, GAMMA_EXT_NODE_NUMBER * sizeof(hi_u16),
                     g_wdr_mode_gamma, GAMMA_EXT_NODE_NUMBER * sizeof(hi_u16));
        }
    }

    for (i = 0; i < GAMMA_OUTSEG_NUMBER; i++) {
        gamma_ctx->in_seg_lut[i] = g_non_uni_in_seg_lut[i];
        gamma_ctx->step_lut[i]   = g_non_uni_step_lut[i];
    }

    for (i = 0; i < GAMMA_EXT_NODE_NUMBER; i++) {
        hi_ext_system_gamma_lut_write(vi_pipe, i, gamma_ctx->ext_gamma_lut[i]);
    }

    /* generate pos by using in_seg and step */
    gamma_get_pos(gamma_ctx, gamma_ctx->gamma_lut, gamma_ctx->ext_gamma_lut);

    gamma_ctx->enable    = HI_TRUE;
    gamma_ctx->lut_update = HI_TRUE;

    return HI_SUCCESS;
}

hi_s32 isp_gamma_init(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_s32 ret;
    isp_reg_cfg *local_reg_cfg = (isp_reg_cfg *)reg_cfg;

    ret = gamma_ctx_init(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    gamma_ext_regs_initialize(vi_pipe);
    ret = gamma_initialize(vi_pipe);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    gamma_regs_initialize(vi_pipe, local_reg_cfg);

    return HI_SUCCESS;
}

static hi_void isp_gamma_wdr_mode_set(hi_vi_pipe vi_pipe, hi_void *reg_cfg)
{
    hi_s32 ret;
    hi_u8 i;
    hi_u32 user_update_idx[ISP_STRIPING_MAX_NUM] = {0};
    isp_reg_cfg *local_reg_cfg = (isp_reg_cfg *)reg_cfg;

    for (i = 0; i < local_reg_cfg->cfg_num; i++) {
        user_update_idx[i] = local_reg_cfg->alg_reg_cfg[i].gamma_cfg.usr_reg_cfg.update_index;
    }

    ret = isp_gamma_init(vi_pipe, reg_cfg);
    if (ret != HI_SUCCESS) {
        isp_err_trace("init gamma failed!\n");
        return;
    }

    for (i = 0; i < local_reg_cfg->cfg_num; i++) {
        local_reg_cfg->alg_reg_cfg[i].gamma_cfg.usr_reg_cfg.update_index = user_update_idx[i] + 1;
        local_reg_cfg->alg_reg_cfg[i].gamma_cfg.usr_reg_cfg.switch_mode  = HI_TRUE;
        local_reg_cfg->alg_reg_cfg[i].gamma_cfg.usr_reg_cfg.switch_lut2_stt_reg_new = HI_TRUE;
        local_reg_cfg->alg_reg_cfg[i].gamma_cfg.usr_reg_cfg.switch_reg_new_cnt    = 0;
    }
}

static hi_bool __inline check_gamma_open(isp_gamma *gamma)
{
    return (gamma->enable == HI_TRUE);
}

hi_s32 isp_gamma_run(hi_vi_pipe vi_pipe, const hi_void *stat_info, hi_void *reg_cfg, hi_s32 rsv)
{
    hi_u16 i;
    hi_s32 ret;
    isp_reg_cfg        *local_reg   = (isp_reg_cfg *)reg_cfg;
    isp_gamma          *gamma_ctx   = HI_NULL;
    isp_gamma_usr_cfg  *usr_reg_cfg = HI_NULL;

    gamma_get_ctx(vi_pipe, gamma_ctx);
    isp_check_pointer_return(gamma_ctx);

    gamma_ctx->enable = hi_ext_system_gamma_en_read(vi_pipe);

    for (i = 0; i < local_reg->cfg_num; i++) {
        local_reg->alg_reg_cfg[i].gamma_cfg.gamma_en = gamma_ctx->enable;
    }

    local_reg->cfg_key.bit1_gamma_cfg = 1;

    /* check hardware setting */
    if (!check_gamma_open(gamma_ctx)) {
        return HI_SUCCESS;
    }

    if (gamma_ctx->gamma_lut == HI_NULL || gamma_ctx->ext_gamma_lut == HI_NULL) {
        isp_err_trace("gamma lut_ctx is empty!\n");
        return HI_FAILURE;
    }

    /* update control regs; */
    gamma_read_ext_regs(vi_pipe);

    if (gamma_ctx->lut_update) {
        /* update pos */
        gamma_get_pos(gamma_ctx, gamma_ctx->gamma_lut, gamma_ctx->ext_gamma_lut);

        for (i = 0; i < local_reg->cfg_num; i++) {
            usr_reg_cfg = &local_reg->alg_reg_cfg[i].gamma_cfg.usr_reg_cfg;

            ret = memcpy_s(usr_reg_cfg->gamma_lut,  GAMMA_REG_NODE_NUM * sizeof(hi_u16),
                           gamma_ctx->gamma_lut, GAMMA_REG_NODE_NUM * sizeof(hi_u16));
            if (ret != 0) {
                isp_err_trace("ISP[%d] update gamma lut failed!\n", vi_pipe);
            }

            usr_reg_cfg->gamma_lut_update_en = HI_TRUE;
            usr_reg_cfg->update_index   += 1;
        }
    }

    return HI_SUCCESS;
}

hi_s32 isp_gamma_ctrl(hi_vi_pipe vi_pipe, hi_u32 cmd, hi_void *value)
{
    isp_reg_cfg_attr  *local_reg_cfg = HI_NULL;

    switch (cmd) {
        case ISP_WDR_MODE_SET:
            isp_regcfg_get_ctx(vi_pipe, local_reg_cfg);
            isp_check_pointer_return(local_reg_cfg);
            isp_gamma_wdr_mode_set(vi_pipe, (hi_void *)&local_reg_cfg->reg_cfg);
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

hi_s32 isp_gamma_exit(hi_vi_pipe vi_pipe)
{
    isp_gamma *gamma_ctx = HI_NULL;

    gamma_get_ctx(vi_pipe, gamma_ctx);
    isp_check_pointer_return(gamma_ctx);

    isp_free(gamma_ctx->ext_gamma_lut);
    isp_free(gamma_ctx->gamma_lut);

    gamma_ctx_exit(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_alg_register_gamma(hi_vi_pipe vi_pipe)
{
    isp_usr_ctx *isp_ctx = HI_NULL;
    isp_alg_node *algs = HI_NULL;

    isp_get_ctx(vi_pipe, isp_ctx);
    isp_alg_check_return(isp_ctx->alg_key.bit1_gamma);
    algs = isp_search_alg(isp_ctx->algs);
    isp_check_pointer_return(algs);

    algs->alg_type = ISP_ALG_GAMMA;
    algs->alg_func.pfn_alg_init = isp_gamma_init;
    algs->alg_func.pfn_alg_run  = isp_gamma_run;
    algs->alg_func.pfn_alg_ctrl = isp_gamma_ctrl;
    algs->alg_func.pfn_alg_exit = isp_gamma_exit;
    algs->used = HI_TRUE;

    return HI_SUCCESS;
}

