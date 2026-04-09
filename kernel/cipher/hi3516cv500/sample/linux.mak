CFLAGS += -I$(OSAL_ROOT)/include

# target source
OBJS  := $(SRCS:%.c=%.o)

.PHONY : clean all

all: $(TARGET)

$(TARGET):$(COMM_OBJ) $(OBJS)
	@$(CC) $(CFLAGS) -lpthread -lm -o $@ $^ $(LIBA)

clean:
	@rm -f $(TARGET)
	@rm -f $(OBJS)
	@rm -f $(COMM_OBJ)
	@rm -f *.bin
	@rm -f *.asm

cleanstream:
	@rm -f *.h264
	@rm -f *.h265
	@rm -f *.jpg
	@rm -f *.mjp
	@rm -f *.mp4
