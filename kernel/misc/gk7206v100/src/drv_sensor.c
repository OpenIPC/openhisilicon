#include "drv_export.h"
#include "drv_sensor.h"


#ifdef SUPPORT_SENSOR_PWDN_MODE
#define SENSOR0_PWDN_GPIO_NUM       58 //gpio7-2
#define SENSOR1_PWDN_GPIO_NUM       60 //gpio7-4
#define SENSOR0_PWDN_REG_OFFSET     0x74
#define SENSOR1_PWDN_REG_OFFSET     0x7C
#define GPIO_PINMUX_VALUE           0x1100

static void *pwdn_reg_base = XMEDIA_NULL;
#endif

static misc_aov_isp_info g_isp_info = { 0 };
static misc_aov_state g_aov_state = { 0 };

#ifdef SUPPORT_SENSOR_PWDN_MODE
static xmedia_s32 drv_pwdn_sensor_init(xmedia_void)
{
    pwdn_reg_base = (void *)osal_ioremap(0x100C0000, (xmedia_u32)0x80);
    if (pwdn_reg_base == XMEDIA_NULL) {
        MISC_TRACE(MODULE_DBG_ERR,"misc ioremap fail \n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_void drv_pwdn_sensor_exit(xmedia_void)
{
    osal_iounmap(pwdn_reg_base);
    pwdn_reg_base = XMEDIA_NULL;
}

static xmedia_s32 drv_pwdn_sensor_resume(xmedia_s32 isp_pipe)
{
    xmedia_s32 s32Ret = XMEDIA_FAILURE;
    xmedia_s32 gpio_num;

    if(isp_pipe == 0 && g_isp_info.support_sensor_pwdn_standby[isp_pipe] == XMEDIA_TRUE) {
        gpio_num = SENSOR0_PWDN_GPIO_NUM;

        //set pinmux
        osal_writel(GPIO_PINMUX_VALUE, pwdn_reg_base + SENSOR0_PWDN_REG_OFFSET);
    }
    else if(isp_pipe == 1 && g_isp_info.support_sensor_pwdn_standby[isp_pipe] == XMEDIA_TRUE) {
        gpio_num = SENSOR1_PWDN_GPIO_NUM;

        //set pinmux
        osal_writel(GPIO_PINMUX_VALUE, pwdn_reg_base + SENSOR1_PWDN_REG_OFFSET);
    }
    else {
        MISC_TRACE(MODULE_DBG_ERR,"isp pipe(%d) is illegal!\n", isp_pipe);
        return XMEDIA_FAILURE;
    }

    s32Ret = gpio_request(gpio_num, NULL);
    if (s32Ret) {
        return XMEDIA_FAILURE;
    }

    s32Ret = gpio_direction_output(gpio_num, 1);
    if (s32Ret) {
        return XMEDIA_FAILURE;
    }

    gpio_set_value(gpio_num, 1);

    gpio_free(gpio_num);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 drv_pwdn_sensor_stanby(xmedia_s32 isp_pipe)
{
    xmedia_s32 gpio_num;
    xmedia_s32 s32Ret = XMEDIA_FAILURE;

    if(isp_pipe == 0 && g_isp_info.support_sensor_pwdn_standby[isp_pipe] == XMEDIA_TRUE)
        gpio_num = SENSOR0_PWDN_GPIO_NUM;
    else if(isp_pipe == 1 && g_isp_info.support_sensor_pwdn_standby[isp_pipe] == XMEDIA_TRUE)
        gpio_num = SENSOR1_PWDN_GPIO_NUM;
    else {
        MISC_TRACE(MODULE_DBG_ERR,"isp pipe(%d) is illegal!\n", isp_pipe);
        return XMEDIA_FAILURE;
    }

    s32Ret = gpio_request(gpio_num, NULL);
    if (s32Ret) {
        return XMEDIA_FAILURE;
    }

    s32Ret = gpio_direction_output(gpio_num, 1);
    if (s32Ret) {
        return XMEDIA_FAILURE;
    }

    gpio_set_value(gpio_num, 0);

    gpio_free(gpio_num);

    return XMEDIA_SUCCESS;
}
#endif

static xmedia_s32  drv_i2c_sensor_resume(xmedia_s32 pipe)
{
    return export_isp_resume_sensor(pipe);
}

static xmedia_s32  drv_i2c_sensor_suspend(xmedia_s32 pipe)
{
    return export_isp_suspend_sensor(pipe);
}

static xmedia_s32 misc_set_sensor_mode(xmedia_ulong arg)
{
    misc_aov_state *aov_state = (misc_aov_state *)arg;

    if (aov_state == XMEDIA_NULL) {
        return XMEDIA_FAILURE;
    }

    g_aov_state.en_aov_mode = aov_state->en_aov_mode;
    g_aov_state.frame_num = aov_state->frame_num;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 misc_get_sensor_mode(misc_aov_state *aov_state)
{
    if (aov_state == XMEDIA_NULL) {
        return XMEDIA_FAILURE;
    }

    if (g_aov_state.en_aov_mode == XMEDIA_FALSE || g_aov_state.frame_num == 0) {
        return XMEDIA_FAILURE;
    }

    aov_state->en_aov_mode = g_aov_state.en_aov_mode;
    aov_state->frame_num = g_aov_state.frame_num;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 misc_sensor_standby(xmedia_s32 pipe)
{
    xmedia_s32 s32Ret = XMEDIA_FAILURE;
    xmedia_s32 i = 0;
    static xmedia_s32 cur_frame_num[VI_MAX_DEV_NUM] = {0};
    misc_aov_state aov_state = {0};

    s32Ret = misc_get_sensor_mode(&aov_state);
    if (s32Ret != XMEDIA_SUCCESS) {
        return XMEDIA_FAILURE;
    }

    if (g_isp_info.isp_pipe_num == 0) {
        return XMEDIA_FAILURE;
    }

    for (i = 0; i < g_isp_info.isp_pipe_num; i++) {
        if (pipe == g_isp_info.isp_pipe[i]) {
            if (cur_frame_num[i] < (g_aov_state.frame_num - 1)) {
                cur_frame_num[i] ++;
            } else {
                drv_i2c_sensor_suspend(pipe);
                cur_frame_num[i] = 0;
            }
        }
    }

#ifdef SUPPORT_SENSOR_PWDN_MODE
    if(g_isp_info.support_sensor_pwdn_standby[pipe] == XMEDIA_TRUE) {
        drv_pwdn_sensor_stanby(pipe);
    }
#endif
    return XMEDIA_SUCCESS;
}

xmedia_s32 misc_sensor_init(xmedia_void)
{
#ifdef SUPPORT_SENSOR_PWDN_MODE
    return drv_pwdn_sensor_init();
#else
    return XMEDIA_SUCCESS;
#endif
}

xmedia_s32 misc_sensor_exit(xmedia_void)
{
#ifdef SUPPORT_SENSOR_PWDN_MODE
    drv_pwdn_sensor_exit();
#endif
    return XMEDIA_SUCCESS;
}

xmedia_s32 misc_sensor_resume(xmedia_void)
{
    xmedia_s32 i = 0;
#ifdef SUPPORT_SENSOR_PWDN_MODE
    xmedia_s32 isp_pipe;
#endif

    for (i = 0; i < g_isp_info.isp_pipe_num; i++) {
#ifdef SUPPORT_SENSOR_PWDN_MODE
        isp_pipe = g_isp_info.isp_pipe[i];
        if(g_isp_info.support_sensor_pwdn_standby[isp_pipe] == XMEDIA_TRUE) {
            drv_pwdn_sensor_resume(isp_pipe);
        }
#endif
        drv_i2c_sensor_resume(g_isp_info.isp_pipe[i]);
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 misc_ctl_aov_init(xmedia_ulong arg)
{
#ifdef SUPPORT_SENSOR_PWDN_MODE
    xmedia_s32 i = 0, isp_pipe;
#endif
    misc_aov_isp_info *isp_info = (misc_aov_isp_info *)arg;

    if (isp_info == XMEDIA_NULL) {
        return XMEDIA_FAILURE;
    }

    if (isp_info->isp_pipe_num == 0) {
        return XMEDIA_FAILURE;
    }

    memcpy(&g_isp_info, isp_info, sizeof(misc_aov_isp_info));

    export_vi_register_misc_aov_callback(misc_sensor_standby);
#ifdef SUPPORT_SENSOR_PWDN_MODE
    for (i = 0; i < g_isp_info.isp_pipe_num; i++) {
        isp_pipe = g_isp_info.isp_pipe[i];
        if(g_isp_info.support_sensor_pwdn_standby[isp_pipe] == XMEDIA_TRUE) {
            drv_pwdn_sensor_resume(isp_pipe);
        }
    }
#endif
    return XMEDIA_SUCCESS;
}

xmedia_s32 misc_ctl_aov_set_sensor_mode(xmedia_ulong arg)
{
    return misc_set_sensor_mode(arg);
}

#ifdef SUPPORT_SENSOR_PWDN_MODE
xmedia_s32 misc_ctl_sensor_pwdn_standby(xmedia_ulong arg)
{
    xmedia_s32 isp_pipe = *(xmedia_s32*)arg;

    return drv_pwdn_sensor_stanby(isp_pipe);
}

xmedia_s32 misc_ctl_sensor_pwdn_resume(xmedia_ulong arg)
{
    xmedia_s32 isp_pipe = *(xmedia_s32*)arg;

    return drv_pwdn_sensor_resume(isp_pipe);
}
#endif
