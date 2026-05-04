LIB_NAME := libsns_jxf22

# Source is canonical at libraries/sensor/hi3516cv300/jx_f22/.
# Same .c file builds both V2 (this dir) and V3 forms via -D<chiparch>
# feature gates; see docs/sensor-driver-api.md §3.2 for the cv200/cv300
# ISP-feature ABI drift that drives the gating.
SRC_DIR := $(CURDIR)/../../hi3516cv300/jx_f22

override CFLAGS += -fPIC -Dhi3516cv200 \
	-I$(CURDIR)/../../../../kernel/include/hi3516cv200 \
	-I$(CURDIR)/../../../../kernel/isp/arch/hi3516cv200/include

SRCS := jxf22_cmos.c jxf22_sensor_ctl.c
OBJS := $(SRCS:%.c=%.o)
vpath %.c $(SRC_DIR)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm -f $(OBJS) $(LIB_NAME).so $(LIB_NAME).a
