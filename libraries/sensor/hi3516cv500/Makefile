SENSOR_DIRS := $(sort $(dir $(wildcard */Makefile)))

all:
	for dir in $(SENSOR_DIRS); do \
		make -C $$dir all || true; \
	done

clean:
	for dir in $(SENSOR_DIRS); do \
		make -C $$dir clean; \
	done
