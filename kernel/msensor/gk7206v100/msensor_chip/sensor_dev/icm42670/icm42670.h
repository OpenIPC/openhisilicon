/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#ifndef ICM42670_H
#define ICM42670_H

#include "msensor.h"
#include "osal.h"
#include "linux/workqueue.h"
#include "linux/hrtimer.h"

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#define ICM42670_DEV_ADDR       0x68
#define I2C_DEV_NUM             0x1

#define ICM42670_VALID_DATA_BIT 16

#define div_0_to_1(a)           (((a) == 0) ? 1 : (a))

#define TRUE_REGISTER_VALUE     0x01
#define RESET_OFFSET            0x04
#define CLKSET_VALUE            0x01

// MREGs
#define MREG1                    0
#define MREG2                    1
#define MREG3                    2

#define ICM42670_SELFID          0x67 // ICM42670 device id default value
#define ICM42670_FIFO_MAX_RECORD 140
#define ICM42670_FIFO_R_MAX_SIZE 1152

#define ROOMTEMP_OFFSET          (25 * (0x1 << 10))
#define GRADIENT_TEMP            (0x1 << 10)
#define TEMP_SENSITIVITY         128

/* icm42670 interrupt pin reg */
#define INT_GPIO_CHIP   4 //7
#define INT_GPIO_OFFSET 6 //2

#define TIME_RECORD_CNT 10
#define DATA_RECORD_CNT (TIME_RECORD_CNT)

typedef struct {
    xmedia_msensor_gyro_config gyro_config;
    xmedia_u32 band_width;
    xmedia_u64 last_pts;
} msensor_gyro_status;

typedef struct {
    xmedia_msensor_acc_config acc_config;
    xmedia_u32 band_width;
    xmedia_u64 last_pts;
} msensor_acc_status;

typedef struct {
    xmedia_msensor_sample_data accel_cur_data;
    xmedia_msensor_sample_data gyro_cur_data;
    xmedia_msensor_sample_data magn_data;
    struct i2c_client *client;
    struct spi_device *xmedia_spi;
    struct task_struct *read_data_task;
    struct task_struct *get_data_kthread;
    osal_work_struct_t work;
    xmedia_u8 power_mode;
    xmedia_u8 flag_acc_fifo_enabled;
    xmedia_u8 flag_gyro_fifo_enabled;
    xmedia_u8 flag_fifo_incomming;
    xmedia_u8 *fifo_buf;
    xmedia_u32 fifo_length;
    xmedia_u8 record_num;
    xmedia_u8 enable_kthread;
    xmedia_u8 fifo_en;
    triger_config triger_data;
    struct hrtimer hrtimer;
    xmedia_u8 thread_wakeup;
    osal_semaphore_t g_sem;
    msensor_acc_status acc_status;
    msensor_gyro_status gyro_status;
    xmedia_s32 temperature;
    xmedia_s32 irq_num;
    xmedia_s32 workqueue_call_times;
    osal_wait_t wait_call_stop_working;
} icm42670_dev_info;

icm42670_dev_info *chip_get_dev_info(xmedia_s32 dev);
xmedia_s32 chip_fifo_data_reset(xmedia_s32 dev);
xmedia_s32 chip_get_triger_config(xmedia_s32 dev, triger_config *triger_data);

xmedia_s32 chip_dev_init(xmedia_s32 dev, xmedia_msensor_param *msensor_param);
xmedia_s32 chip_dev_mng_init(xmedia_s32 dev, xmedia_msensor_param *msensor_param);
xmedia_void chip_dev_exit(xmedia_s32 dev, xmedia_msensor_param *msensor_param);

xmedia_s32 chip_sensor_init(xmedia_s32 dev);
xmedia_void chip_sensor_exit(xmedia_s32 dev);
xmedia_s32 chip_timer_run(xmedia_s32 dev);
xmedia_s32 chip_interrupt_run(xmedia_s32 dev);
xmedia_s32 chip_timer_stop(xmedia_s32 dev);
xmedia_s32 chip_interrupt_stop(xmedia_s32 dev);
xmedia_s32 chip_get_data_for_one_frm(xmedia_s32 dev);

/*
 * otp_pwr_down
 * 0: to power up OTP for read/write operation.
 * 1: to power down OTP to save power.
 * This bit is automatically set to 1 when OTP copy operation is complete.
 */
#define OTP_CTRL7_OTP_PWR_DOWN_POS  0x01
#define OTP_CTRL7_OTP_PWR_DOWN_MASK (0x01 << OTP_CTRL7_OTP_PWR_DOWN_POS)
/* ---------------------------------------------------------------------------
 * register MREG_FPGA
 * ---------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------
 * register ROM
 * ---------------------------------------------------------------------------*/
#define ACCEL_DATA_SIZE               6
#define GYRO_DATA_SIZE                6
#define TEMP_DATA_SIZE                2

#define FIFO_HEADER_SIZE              1
#define FIFO_ACCEL_DATA_SIZE          ACCEL_DATA_SIZE
#define FIFO_GYRO_DATA_SIZE           GYRO_DATA_SIZE
#define FIFO_TEMP_DATA_SIZE           1
#define FIFO_TS_FSYNC_SIZE            2
#define FIFO_TEMP_HIGH_RES_SIZE       1
#define FIFO_ACCEL_GYRO_HIGH_RES_SIZE 3

/*
 * Hysteresis high peak threshold (mg) added to the threshold after the initial threshold is met.
 * Use type APEX_CONFIG5_HIGHG_PEAK_TH_HYST_t to define highG peak hysteresis
 * These types are defined in inv_imu_defs.h.
 */
#define HIGHG_PEAK_HYSTERESIS APEX_CONFIG5_HIGHG_PEAK_TH_HYST_156MG

/* Initial WOM threshold to be applied to IMU in mg */
#define WOM_THRESHOLD_INITIAL_MG 200

/*
 * HighG frequencies
 * Use type APEX_CONFIG1_DMP_ODR_t to define DMP frequency
 * These types are defined in inv_imu_defs.h.
 *
 * \note The frequency modes to run the HighG are :
 * APEX_CONFIG1_DMP_ODR_25Hz  (Low Power mode),
 * APEX_CONFIG1_DMP_ODR_50Hz  (Performance mode)
 */
#define HIGHG_FREQUENCY_MODE APEX_CONFIG1_DMP_ODR_50Hz

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif

