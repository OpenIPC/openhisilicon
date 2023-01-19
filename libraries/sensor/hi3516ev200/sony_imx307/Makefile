LIB_NAME := libsns_imx307

override CFLAGS += -fPIC -I$(CURDIR)/../../../../include

SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o) $(ASM_SRCS:%.S=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm $(OBJS) $(LIB_NAME).so
