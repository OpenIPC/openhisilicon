/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_MIPI_RX_MOD_INIT_H
#define OT_MIPI_RX_MOD_INIT_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

void mipi_rx_set_irq_num(unsigned int irq_num);
void mipi_rx_set_regs(const void *regs);

int mipi_rx_mod_init(void);
void mipi_rx_mod_exit(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef OT_MIPI_RX_MOD_INIT_H */