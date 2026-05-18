# Hisilicon Hi3531 sample Makefile

include ../Makefile.param


# target source
SRC  := $(wildcard *.c) 
OBJ  := $(SRC:%.c=%.o)

LIBS += $(REL_LIB)/libmpi.a $(REL_LIB)/libtde.a $(REL_LIB)/libhdmi.a $(AUDIO_LIBA) $(JPEGD_LIBA)

TARGET := $(OBJ:%.o=%)
.PHONY : clean all

all: $(TARGET)

$(TARGET):%:%.o $(COMM_OBJ)
	$(CC) $(CFLAGS) -lpthread -lm -o $@ $^ $(LIBS) 

clean:
	@-rm -f $(TARGET)
	@-rm -f $(OBJ)
