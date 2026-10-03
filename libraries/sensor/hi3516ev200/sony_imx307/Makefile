LIB_NAME := libsns_imx307

override CFLAGS += -DSDK_CODE=$(SDK_CODE) -fPIC -I$(CURDIR)/../../../../include
# -ldl: imx307_is_2lane_mode() uses dladdr() to read its own .so basename
# and flip between 4-lane and 2-lane register sequences. libsns_imx307_2L.so
# is shipped as a symlink to this binary.
override LDLIBS += -ldl

SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o) $(ASM_SRCS:%.S=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS) $(LDLIBS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm $(OBJS) $(LIB_NAME).so
