MAKEFILES = $(shell find . -mindepth 2 -maxdepth 4 -type f -name Makefile)
SUBDIRS   = $(filter-out ./,$(dir $(MAKEFILES)))

# CV500: build ISP library + sensor drivers from SDK source
ifeq ($(CHIPARCH),hi3516cv500)
    SUBDIRS := $(filter ./sensor/hi3516cv500/% ./isp/,$(SUBDIRS))
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

all:
	for dir in $(SUBDIRS); do \
		make -C $$dir CHIPARCH=$(CHIPARCH) all || true; \
	done
