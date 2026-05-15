LIB_NAME := libnnie_neo

LIB_DIR := $(CURDIR)

override CFLAGS += \
	-fPIC \
	-Wall \
	-I$(LIB_DIR)/include \
	-I$(LIB_DIR)/../include \
	-I$(LIB_DIR)/../mpi_neo/include

# Vendor MPI headers (HI_MPI_SVP_NNIE_* + structs) come from the
# kernel/include/hi3516cv500 tree, which is shared across kernel +
# userspace. Adjust if STAGING is set to point at a different SDK.
STAGING ?= ../../../../firmware/output-cv500/build/hisilicon-opensdk-custom
override CFLAGS += -I$(STAGING)/kernel/include/hi3516cv500

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:%.c=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS) -lpthread -lm

$(LIB_NAME).a: $(OBJS)
	$(AR) -rcs $(LIB_NAME).a $(OBJS)

clean:
	-rm -f $(OBJS) $(LIB_NAME).so $(LIB_NAME).a
