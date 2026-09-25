MAKEFILES = $(shell find . -mindepth 2 -maxdepth 4 -type f -name Makefile)
SUBDIRS   = $(filter-out ./,$(dir $(MAKEFILES)))

# CV500: build ISP library + sensor drivers from SDK source +
# nnie_neo userspace (cv500-only — NNIE block doesn't exist on V4).
ifeq ($(CHIPARCH),hi3516cv500)
    SUBDIRS := $(filter ./sensor/hi3516cv500/% ./isp/ ./nnie_neo/,$(SUBDIRS))
endif

# nnie_neo is cv500-only — exclude from any other chiparch (V4, cv200,
# etc.) where its cv500-specific MPI headers wouldn't compile.
ifneq ($(CHIPARCH),hi3516cv500)
    SUBDIRS := $(filter-out ./nnie_neo/,$(SUBDIRS))
endif

# CV300: build sensor drivers from SDK source
ifeq ($(CHIPARCH),hi3516cv300)
    SUBDIRS := $(filter ./sensor/hi3516cv300/%,$(SUBDIRS))
endif

# CV100: build sensor drivers from SDK source
ifeq ($(CHIPARCH),hi3516cv100)
    SUBDIRS := $(filter ./sensor/hi3516cv100/%,$(SUBDIRS))
endif

# CV200: build sensor drivers from SDK source
ifeq ($(CHIPARCH),hi3516cv200)
    SUBDIRS := $(filter ./sensor/hi3516cv200/%,$(SUBDIRS))
endif

# AV100: build sensor drivers from SDK source
ifeq ($(CHIPARCH),hi3516av100)
    SUBDIRS := $(filter ./sensor/hi3516av100/%,$(SUBDIRS))
endif

# hi3519v101: build sensor drivers from SDK source
ifeq ($(CHIPARCH),hi3519v101)
    SUBDIRS := $(filter ./sensor/hi3519v101/%,$(SUBDIRS))
endif

# hi3516cv6xx (V5): build sensor drivers from SDK source. Exclude the
# common/ subdir from the per-sensor build loop — sensor_common.c is
# pulled into each sensor's link via the per-sensor Makefile, so common/
# itself has no Makefile and would fail the outer `make -C`.
ifeq ($(CHIPARCH),hi3516cv6xx)
    SUBDIRS := $(filter ./sensor/hi3516cv6xx/%,$(SUBDIRS))
    SUBDIRS := $(filter-out ./sensor/hi3516cv6xx/common/,$(SUBDIRS))
endif

# hi3519dv500 (V5, aarch64): build sensor drivers from SDK source.
ifeq ($(CHIPARCH),hi3519dv500)
    SUBDIRS := $(filter ./sensor/hi3519dv500/%,$(SUBDIRS))
endif

# gk7205v500 (XMedia): the XMedia SPC020 sensor interface is the V4 one
# rebranded, so the V4 drivers in sensor/hi3516ev200 build for it unchanged.
# sensor/compat/xmedia maps their Goke names (GK_*, gk_api_*.h, hicompat.h)
# onto the SDK's XMEDIA_* API, and it and the SDK headers imported under
# kernel/ come first on the include path, ahead of the V4 headers the
# per-sensor Makefiles add.
ifeq ($(CHIPARCH),gk7205v500)
    SUBDIRS := $(filter ./sensor/hi3516ev200/%,$(SUBDIRS))
    XM_INC := $(CURDIR)/../kernel/include/gk7205v500
    XM_ISP := $(CURDIR)/../kernel/isp/gk7205v500
    SENSOR_CFLAGS := -I$(CURDIR)/sensor/compat/xmedia -I$(XM_INC) \
	-include $(CURDIR)/sensor/compat/xmedia/xm_gk_compat.h \
	-I$(XM_ISP)/include -I$(XM_ISP)/include/arch/xm72050200/include \
	-I$(XM_ISP)/kernel/arch/xm72050200/include -I$(XM_ISP)/kernel/mkp/include \
	-I$(XM_ISP)/kernel/arch/include -I$(XM_ISP)/firmware/include \
	-I$(XM_ISP)/firmware/arch/include -I$(XM_ISP)/ext_inc
endif
# hi3519dv500 sensors are aarch64-only — the 32-bit toolchains used by every
# other chiparch can't compile their 64-bit-aligned vendor headers. Exclude
# them from any non-dv500 build (chiparchs without an explicit filter above,
# e.g. the V4 family, otherwise sweep in every sensor dir). Mirrors the
# nnie_neo guard.
ifneq ($(CHIPARCH),hi3519dv500)
    SUBDIRS := $(filter-out ./sensor/hi3519dv500/%,$(SUBDIRS))
endif

all:
	@failed=""; \
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir CHIPARCH=$(CHIPARCH) $(if $(SENSOR_CFLAGS),CFLAGS="$(SENSOR_CFLAGS) $(CFLAGS)") all || failed="$$failed $$dir"; \
	done; \
	if [ -n "$$failed" ]; then \
		echo; \
		echo "Build failed for:" >&2; \
		for d in $$failed; do echo "  $$d" >&2; done; \
		exit 1; \
	fi
