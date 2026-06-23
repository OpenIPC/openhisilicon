ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../../../Makefile.param
    include $(PARAM_FILE)
endif

ifeq ($(CBB_PARAM_FILE), )
    CBB_PARAM_FILE:=../../../Makefile.param
    include $(CBB_PARAM_FILE)
endif

MPI_SRC_DIR += $(PWD)/../../security_subsys_common/uapi_code
MPI_SRC_DIR += $(PWD)/../../security_subsys_common/ot_mpi
MPI_SRC_C := ot_mpi_hash.c ot_mpi_kdf.c ot_mpi_pke.c ot_mpi_symc.c ot_mpi_trng.c
MPI_SRC_C += uapi_hash.c uapi_kdf.c uapi_pke.c uapi_symc.c uapi_trng.c uapi_common.c
MPI_CFLAGS  += -I$(PWD)/../../crypto_osal_lib
MPI_CFLAGS  += -I$(PWD)/../../security_subsys_common/include/uapi_include
MPI_CFLAGS  += -I$(PWD)/../../security_subsys_common/include/common_include
MPI_CFLAGS  += -I$(PWD)/../../security_subsys_common/include/ioctl_include
MPI_CFLAGS  += -I$(PWD)/../../security_subsys_common/ot_mpi_api
MPI_CFLAGS  += -I$(PWD)/../..
MPI_CFLAGS  += -I$(REL_INC)

MPI_CFLAGS  += -DCONFIG_CRYPTO_CHIP_HI3519DV500

#*************************************************************************
# release header
INC_FILE    := $(wildcard ../../security_subsys_common/ot_mpi_api/ot_mpi_cipher.h)

LIB_NAME    := ot_mpi_cipher

include $(MAKE_MPI_FILE)