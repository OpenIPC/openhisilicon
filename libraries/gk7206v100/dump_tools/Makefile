# VI/VPSS/VO frame dump tools (vi_bayer_dump, vi_chn_dump, vpss_ichn_dump,
# vpss_ochn_dump, vo_chn_dump, vo_layer_dump), from the GK7206 SDK's
# source/gmp/usr/dump_tools.
#
# They call the MPI, so they link against the vendor libxmedia_*.so, which
# only the firmware build has (goke-osdrv-gk7206v100 stages them). Point
# XMEDIA_LIBDIR at that directory; without it there is nothing to link
# against and the tools are skipped. Those libraries were built against
# uClibc, so the tools link uclibc-compat ahead of musl (see its Config.in).
KO_DIR := $(CURDIR)/../../../kernel

override CFLAGS += -Wall -O2 -I$(KO_DIR)/include/gk7206v100

# The vendor libraries declare no NEEDED on each other and leave that to the
# program, so it names all of them -- libxmedia_isp calls the NPU graph
# runtime (cl, npu, flatcc) for AI noise reduction.
XMEDIA_LIBS := -lxmedia_vpss -lxmedia_region -lxmedia_vi -lxmedia_vgs \
	-lxmedia_common -lxmedia_tde -lxmedia_isp -lxmedia_ae -lxmedia_awb \
	-lxmedia_venc -lxmedia_vo -lxmedia_cl -lxmedia_npu -lxmedia_flatcc

SRCS := $(wildcard *.c)
TOOLS := $(SRCS:%.c=%)

ifneq ($(XMEDIA_LIBDIR),)
all: $(TOOLS)
else
all:
	@echo "dump_tools: XMEDIA_LIBDIR not set, skipping (needs the vendor libxmedia_*.so)"
endif

%: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -L$(XMEDIA_LIBDIR) \
		-Wl,--whole-archive -luclibc-compat-static -Wl,--no-whole-archive \
		-Wl,--no-as-needed -luclibc-compat \
		$(XMEDIA_LIBS) -lpthread -lm

clean:
	-rm -f $(TOOLS)
