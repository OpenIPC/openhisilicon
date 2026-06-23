/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_MIPI_TX_MOD_INIT_H
#define OT_MIPI_TX_MOD_INIT_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

void mipi_tx_set_irq_num(unsigned int irq_num);
void mipi_tx_set_regs(const void *regs);

int mipi_tx_module_init(int smooth);
void mipi_tx_module_exit(void);

int mipi_tx_get_smooth(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef OT_MIPI_TX_MOD_INIT_H */
