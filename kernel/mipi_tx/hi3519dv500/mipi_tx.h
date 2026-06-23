/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef MIPI_TX_H
#define MIPI_TX_H
#include "securec.h"
#include "ot_mipi_tx.h"

#define MIPI_TX_MAX_SYNC_VACT  4096
#define MIPI_TX_MIN_SYNC_VACT  100
#define MIPI_TX_MAX_SYNC_VBP   256
#define MIPI_TX_MIN_SYNC_VBP   1
#define MIPI_TX_MAX_SYNC_VFP   256
#define MIPI_TX_MIN_SYNC_VFP   1
#define MIPI_TX_MAX_SYNC_HACT  4096
#define MIPI_TX_MIN_SYNC_HACT  1
#define MIPI_TX_MAX_SYNC_HBP   65535
#define MIPI_TX_MIN_SYNC_HBP   1
#define MIPI_TX_MAX_SYNC_HFP   65535
#define MIPI_TX_MIN_SYNC_HFP   1
#define MIPI_TX_MAX_SYNC_HPW   65535
#define MIPI_TX_MIN_SYNC_HPW   1
#define MIPI_TX_MAX_SYNC_VPW   256
#define MIPI_TX_MIN_SYNC_VPW   1

#define MIPI_TX_MAX_PHY_DATA_RATE 1800 /* Mbps */
#define MIPI_TX_MIN_PHY_DATA_RATE 120 /* Mbps */

#define MIPI_TX_MAX_PIXEL_CLK ((MIPI_TX_MAX_PHY_DATA_RATE) * (LANE_MAX_NUM) * 1000 / 8) /* KHz , 8: min 8bit/pixel */
#define MIPI_TX_MIN_PIXEL_CLK ((MIPI_TX_MIN_PHY_DATA_RATE) * (1) * 1000 / 24) /* KHz, 24: max 24bit/pixel */

#define mipi_tx_err(x, ...) \
    do { \
        osal_printk("%s(%d): "x, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define mipi_tx_check_null_ptr_return(ptr) \
    do { \
        if ((ptr) == NULL) { \
            mipi_tx_err("NULL pointer\r\n"); \
            return (-1); \
        } \
    } while (0)

#define mipi_tx_desc(x) 1
#define mipi_tx_unused(x) ((void)(x))

typedef struct {
    mipi_tx_work_mode_t work_mode;
    unsigned char       lp_clk_en;
} mipi_tx_work_param;

typedef struct {
    unsigned int        devno;
    mipi_tx_work_param     work_param;
} mipi_tx_pub_info;

typedef struct {
    combo_dev_cfg_t dev_cfg;
    mipi_tx_work_param work_param;
} mipi_tx_dev_ctx_t;

void mipi_tx_set_work_mode(mipi_tx_work_mode_t work_mode);
mipi_tx_work_mode_t mipi_tx_get_work_mode(void);
void mipi_tx_set_lp_clk_en(unsigned char lp_clk_en);
unsigned char mipi_tx_get_lp_clk_en(void);
int mipi_tx_get_lane_num(const short lane_id[], int lane_id_len);

#endif
