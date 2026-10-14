# libxmedia_svp.so -- the SVP task framework (YOLOv5/v8 and RCNN post-
# processing, NMS, ByteTrack, motion) over the NPU, from the GK7206 SDK's
# source/gmp/usr/svp. Replaces the SDK's uClibc build.
#
# One part has no source: src/alg/warp_affine/lib/libwarp_affine.a. The SDK
# ships its Makefile and headers but not the .cpp files it lists, so the
# archive is linked in as the vendor's object code, as the SDK itself does.
# It is C++ and was built against uClibc: like the SDK's own library, ours
# leaves its libstdc++ symbols (operator new[]/delete[], ios_base::Init) and
# uClibc's __assert unresolved, for the program that loads it to provide --
# libstdc++ and uclibc-compat respectively. The xmedia_* MPI calls resolve
# the same way, from the libxmedia_* libraries the program links.
LIB_NAME := libxmedia_svp

KO_DIR := $(CURDIR)/../../../kernel
WARP := src/alg/warp_affine/lib/libwarp_affine.a

override CFLAGS += -fPIC -Wall -O2 -DDEBUG_PROC_INFO \
	-I$(KO_DIR)/include/gk7206v100 \
	-I$(KO_DIR)/osal/gk7206v100/include \
	$(addprefix -Iinclude/,. common xmedia_hal post_process task_instance alg) \
	-Isrc/alg/warp_affine

SRCS := $(wildcard src/*.c src/xmedia_hal/*.c src/alg/*.c src/common/*.c \
	src/post_process/*.c src/task_instance/*.c)
OBJS := $(SRCS:%.c=%.o)

all: $(LIB_NAME).so $(LIB_NAME).a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_NAME).so: $(OBJS) $(WARP)
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJS) -Wl,--whole-archive $(WARP) -Wl,--no-whole-archive

# The static archive carries the warp_affine objects too, so a static link
# needs nothing else from this directory.
$(LIB_NAME).a: $(OBJS) $(WARP)
	rm -rf .warp && mkdir .warp && cd .warp && $(AR) -x ../$(WARP)
	$(AR) -rcs $@ $(OBJS) .warp/*.o
	rm -rf .warp

clean:
	-rm -rf $(OBJS) $(LIB_NAME).so $(LIB_NAME).a .warp
