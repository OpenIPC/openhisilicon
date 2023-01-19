#ifndef HICOMPAT_H
#define HICOMPAT_H

#ifdef SDK_CODE
#if SDK_CODE == 0x3516E200
#define GK_API_ISP_SensorRegCallBack HI_MPI_ISP_SensorRegCallBack
#define GK_API_ISP_SensorUnRegCallBack HI_MPI_ISP_SensorUnRegCallBack
#define GK_API_AE_SensorRegCallBack HI_MPI_AE_SensorRegCallBack
#define GK_API_AE_SensorUnRegCallBack HI_MPI_AE_SensorUnRegCallBack
#define GK_API_AWB_SensorRegCallBack HI_MPI_AWB_SensorRegCallBack
#define GK_API_AWB_SensorUnRegCallBack HI_MPI_AWB_SensorUnRegCallBack
#endif
#endif

#endif /* HICOMPAT_H */
