SENSOR_DIRS := $(sort $(dir $(wildcard */Makefile)))

all:
	@failed=""; \
	for dir in $(SENSOR_DIRS); do \
		$(MAKE) -C $$dir all || failed="$$failed $$dir"; \
	done; \
	if [ -n "$$failed" ]; then \
		echo; \
		echo "Build failed for:" >&2; \
		for d in $$failed; do echo "  $$d" >&2; done; \
		exit 1; \
	fi

clean:
	for dir in $(SENSOR_DIRS); do \
		make -C $$dir clean; \
	done
