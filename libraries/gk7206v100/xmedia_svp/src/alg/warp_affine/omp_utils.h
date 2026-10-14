#ifndef UTILS_OMP_UTILS_H_
#define UTILS_OMP_UTILS_H_

#ifdef OPENMP
// #if 1

#include <omp.h>
#define OMP_PARALLEL_FOR_ _Pragma("omp parallel for")
#define OMP_PARALLEL_FOR_GUIDED_ _Pragma("omp parallel for")
#define OMP_PARALLEL_FOR_DYNAMIC_ _Pragma("omp parallel for schedule(dynamic)")
#define OMP_SECTION_ _Pragma("omp section")
#define OMP_PARALLEL_SECTIONS_ _Pragma("omp parallel sections")
#define OMP_CORES_ (omp_get_num_procs())
#define OMP_MAX_THREADS_NUM_ (omp_get_max_threads())
#define OMP_TID_ (omp_get_thread_num())
#define OMP_SET_THREADS_(t) (omp_set_num_threads(t))

#else

#define OMP_PARALLEL_FOR_
#define OMP_PARALLEL_FOR_GUIDED_
#define OMP_PARALLEL_FOR_DYNAMIC_
#define OMP_SECTION_
#define OMP_PARALLEL_SECTIONS_
#define OMP_CORES_ (1)
#define OMP_MAX_THREADS_NUM_ (1)
#define OMP_TID_ (0)
#define OMP_SET_THREADS_(t)

#endif  // _OPENMP
#endif  //
