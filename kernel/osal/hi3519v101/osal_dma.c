#include "hi_osal.h"
#include <linux/hidmac.h>
#include <linux/module.h>

int osal_dmac_register_isr(unsigned int channel, void *pisr){
    return dmac_register_isr(channel, pisr);
}
EXPORT_SYMBOL(osal_dmac_register_isr);

int osal_do_dma_llim2m_isp(unsigned int *source,
        unsigned int *dest,
        unsigned int *length,
        unsigned int num)
{
    return do_dma_llim2m_isp(source, dest, length, num);
}
EXPORT_SYMBOL(osal_do_dma_llim2m_isp);

int osal_dmac_channel_free(unsigned int channel){
    return dmac_channel_free(channel);
}
EXPORT_SYMBOL(osal_dmac_channel_free);
