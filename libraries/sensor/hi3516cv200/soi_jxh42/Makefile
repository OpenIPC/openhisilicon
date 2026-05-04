LIB_NAME := libsns_jxh42

override CFLAGS += -fPIC \
	-I$(CURDIR)/../../../../kernel/include/hi3516cv200 \
	-I$(CURDIR)/../../../../kernel/isp/arch/hi3516cv200/include

SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm -f $(OBJS) $(LIB_NAME).so $(LIB_NAME).a
