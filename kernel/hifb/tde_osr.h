#ifndef __TDE_OSR_H__
#define __TDE_OSR_H__

#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */

void tde_osr_enableirq(void);
void tde_osr_disableirq(void);
void tde_osr_hsr(void* pstFunc, void* data);
struct proc_dir_entry * tde_get_procentry(void);

#endif /* _TDE_OSR_H_ */
