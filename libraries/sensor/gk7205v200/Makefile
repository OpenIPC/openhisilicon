ifeq ($(CFG_SDK_EXPORT_FLAG),)
    SDK_DIR := $(shell cd $(CURDIR)/../../../../../../.. && /bin/pwd)
endif

include $(SDK_DIR)/build/base.mk
include $(SDK_DIR)/source/gmp/modules/isp/arch.mk

objects := 
objects += galaxycore_gc2053
objects += galaxycore_gc2053_ForCar
objects += galaxycore_gc4653_2L
objects += omnivision_os05a
objects += smart_sc2231
objects += smart_sc223a
objects += smart_sc2235
objects += smart_sc3235
objects += smart_sc4236
objects += smart_sc500ai
objects += soi_f37
objects += sony_imx307
objects += sony_imx307_2L
objects += sony_imx327 
objects += sony_imx327_2L
objects += sony_imx335
objects += aptina_ar0237
objects += omnivision_ov2718
objects += sony_imx290
objects += smart_sc3335

objects_clean := $(addsuffix _clean, $(objects))

.PHONY: all clean prepare $(objects) $(objects_clean)

all: prepare $(objects)

clean: $(objects_clean)

$(objects):
	$(AT)$(MAKE) -C $@

$(objects_clean):
	$(AT)$(MAKE) -C $(patsubst %_clean, %, $@) clean

prepare:

