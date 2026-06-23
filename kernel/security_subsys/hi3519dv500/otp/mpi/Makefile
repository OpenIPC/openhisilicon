ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../../../Makefile.param
    include $(PARAM_FILE)
endif

ifeq ($(CBB_PARAM_FILE), )
    CBB_PARAM_FILE:=../../../Makefile.param
    include $(CBB_PARAM_FILE)
endif

MPI_SRC_DIR += $(PWD)/../../security_subsys_common/ot_mpi
MPI_SRC_DIR += $(PWD)/../../security_subsys_common/uapi_code
MPI_SRC_C := ot_mpi_otp.c uapi_otp.c
MPI_CFLAGS  += -I$(PWD)/../../crypto_osal_lib \
                -I$(PWD)/../../security_subsys_common/include/uapi_include \
                -I$(PWD)/../../security_subsys_common/include/common_include \
                -I$(PWD)/../../security_subsys_common/include/ioctl_include \
                -I$(PWD)/../../security_subsys_common/ot_mpi_api

# Debug
# MPI_CFLAGS  += -DCRYPTO_UAPI_TRNG_DEBUG_ENABLE
MPI_CFLAGS  += -DCONFIG_CRYPTO_CHIP_HI3519DV500

#*************************************************************************
# release header
INC_FILE    := $(wildcard ../../security_subsys_common/ot_mpi_api/ot_mpi_otp.h)

LIB_NAME    := ot_mpi_otp

include $(MAKE_MPI_FILE)