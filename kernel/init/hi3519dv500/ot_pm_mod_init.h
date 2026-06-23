/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#ifndef OT_PM_MOD_INIT_H
#define OT_PM_MOD_INIT_H

#ifndef MODULE
void pm_set_quick_start_flag(unsigned int quick_start_flag);
int pm_get_quick_start_flag(void);
#endif
#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */
#ifdef OT_DEBUG
void pm_set_core_extra_comp(int core_extra_comp);
void pm_set_cpu_extra_comp(int cpu_extra_comp);
void pm_set_npu_extra_comp(int npu_extra_comp);
#endif
int pm_module_init(void);
void pm_module_exit(void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif /* end of #ifndef OT_PM_MOD_INIT_H */
