LIB_NAME := libsns_imx335_fpv

override CFLAGS += -DSDK_CODE=$(SDK_CODE) -fPIC -I$(CURDIR)/../../../../include  -DCHIPARCH=$(CHIPARCH)

SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o) $(ASM_SRCS:%.S=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm $(OBJS) $(LIB_NAME).so
