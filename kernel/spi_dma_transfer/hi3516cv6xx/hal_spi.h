/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_DMA_H
#define HAL_DMA_H

#include "ot_type.h"

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

td_void spi_enable(td_void);
td_void spi_disable(td_void);
td_s32 ot_spi_dma_ssp_init(td_u32 ssp_no);
td_void ot_spi_dma_ssp_exit(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif
