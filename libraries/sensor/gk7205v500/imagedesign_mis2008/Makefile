LIB_NAME := libsns_mis2008

# Ported from ../../hi3516ev200/imagedesign_mis2008 (Goke naming) to the
# XMedia SPC020 ISP API. The headers are the SDK's, imported on the kernel
# side; the -I order follows the SDK's own sensor Makefiles.
XM_INC := $(CURDIR)/../../../../kernel/include/gk7205v500
XM_ISP := $(CURDIR)/../../../../kernel/isp/gk7205v500
override CFLAGS += -fPIC -I$(XM_INC) -I$(XM_ISP)/include \
	-I$(XM_ISP)/include/arch/xm72050200/include \
	-I$(XM_ISP)/kernel/arch/xm72050200/include \
	-I$(XM_ISP)/kernel/mkp/include -I$(XM_ISP)/kernel/arch/include \
	-I$(XM_ISP)/firmware/include -I$(XM_ISP)/firmware/arch/include \
	-I$(XM_ISP)/ext_inc

SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm -f $(OBJS) $(LIB_NAME).so $(LIB_NAME).a
