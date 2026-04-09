LIB_NAME := libivp_neo

LIB_DIR := $(CURDIR)

override CFLAGS += \
	-fPIC \
	-Wall \
	-I$(LIB_DIR)/include \
	-I$(LIB_DIR)/../include

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:%.c=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS) -lpthread

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm -f $(OBJS) $(LIB_NAME).so $(LIB_NAME).a

.PHONY: all clean
