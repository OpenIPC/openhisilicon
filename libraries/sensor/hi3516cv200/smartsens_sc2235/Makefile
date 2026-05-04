LIB_NAME := libsns_sc2235

# Source is canonical at libraries/sensor/hi3516cv300/smartsens_sc2235/.
# The same .c builds both V2 (this dir) and V3 forms via -D<chiparch>
# feature gates. See docs/sensor-driver-api.md §3.2.
SRC_DIR := $(CURDIR)/../../hi3516cv300/smartsens_sc2235

override CFLAGS += -fPIC -Dhi3516cv200 \
	-I$(CURDIR)/../../../../kernel/include/hi3516cv200 \
	-I$(CURDIR)/../../../../kernel/isp/arch/hi3516cv200/include

SRCS := sc2235_cmos.c sc2235_sensor_ctl.c
OBJS := $(SRCS:%.c=%.o)
vpath %.c $(SRC_DIR)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm -f $(OBJS) $(LIB_NAME).so $(LIB_NAME).a
