#include "hi_osal.h"
#include <linux/module.h>
#include <linux/version.h>

/* linux/hidmac.h is a 3.18 vendor BSP header (no mainline equivalent).
 * On mainline kernels the cv300 ISP blob still references the osal_dmac_*
 * wrappers — link cleanly with -ENOSYS stubs (no DMA exercised in the
 * boot+login+eth0 path). CLAUDE.md prefers shims in kernel_compat.h, but
 * the include itself fails on mainline so the conditional must live here. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
#include <linux/hidmac.h>
#else
static inline int dmac_register_isr(unsigned int ch, void *pisr) { return -ENOSYS; }
static inline int dmac_channel_free(unsigned int ch) { return -ENOSYS; }
static inline int do_dma_llim2m_isp(unsigned int *src, unsigned int *dst,
                                    unsigned int *len, unsigned int num) { return -ENOSYS; }
#endif

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
