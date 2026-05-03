LIB_MIPI := libsns_ov2710_mipi
LIB_DC   := libsns_ov2710_dc

override CFLAGS += -fPIC \
	-I$(CURDIR)/../../../../kernel/include/hi3516cv200 \
	-I$(CURDIR)/../../../../kernel/isp/arch/hi3516cv200/include

SRCS := $(wildcard *.c)
OBJS_MIPI := $(SRCS:%.c=%.mipi.o)
OBJS_DC   := $(SRCS:%.c=%.dc.o)

all: $(LIB_MIPI).so $(LIB_MIPI).a $(LIB_DC).so $(LIB_DC).a

$(LIB_MIPI).so: $(OBJS_MIPI)
	$(CC) -shared -o $@ $(OBJS_MIPI)

$(LIB_MIPI).a: $(OBJS_MIPI)
	$(AR) -rcs $@ $(OBJS_MIPI)

$(LIB_DC).so: $(OBJS_DC)
	$(CC) -shared -o $@ $(OBJS_DC)

$(LIB_DC).a: $(OBJS_DC)
	$(AR) -rcs $@ $(OBJS_DC)

%.mipi.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.dc.o: %.c
	$(CC) -c $(CFLAGS) -DOV2710_PARALLEL -o $@ $<

clean:
	-rm -f *.o $(LIB_MIPI).so $(LIB_MIPI).a $(LIB_DC).so $(LIB_DC).a
