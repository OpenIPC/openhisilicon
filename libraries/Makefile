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

all:
	@failed=""; \
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir CHIPARCH=$(CHIPARCH) all || failed="$$failed $$dir"; \
	done; \
	if [ -n "$$failed" ]; then \
		echo; \
		echo "Build failed for:" >&2; \
		for d in $$failed; do echo "  $$d" >&2; done; \
		exit 1; \
	fi
