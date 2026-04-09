LIB_NAME := libsns_imx327_2l

override CFLAGS += -DSDK_CODE=$(SDK_CODE) -fPIC -I$(CURDIR)/../../../../include
override ASFLAGS += -include $(CURDIR)/../../../../kernel/compat/asm_macros.inc

SRCS_S := $(wildcard *.S)
OBJS := $(SRCS_S:%.S=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm -f $(OBJS) $(LIB_NAME).so $(LIB_NAME).a
