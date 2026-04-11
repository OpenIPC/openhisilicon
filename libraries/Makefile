MAKEFILES = $(shell find . -mindepth 2 -maxdepth 4 -type f -name Makefile)
SUBDIRS   = $(filter-out ./,$(dir $(MAKEFILES)))

# CV500: build ISP library + sensor drivers from SDK source
ifeq ($(CHIPARCH),hi3516cv500)
    SUBDIRS := $(filter ./sensor/hi3516cv500/% ./isp/,$(SUBDIRS))
endif

# CV200: build sensor drivers from SDK source
ifeq ($(CHIPARCH),hi3516cv200)
    SUBDIRS := $(filter ./sensor/hi3516cv200/%,$(SUBDIRS))
endif

all:
	for dir in $(SUBDIRS); do \
		make -C $$dir CHIPARCH=$(CHIPARCH) all || true; \
	done
