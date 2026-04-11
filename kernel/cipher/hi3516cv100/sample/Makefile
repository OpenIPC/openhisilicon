ifeq ($(PARAM_FILE), )
        PARAM_FILE:=../../../mpp/Makefile.param
        include $(PARAM_FILE)
endif

SRC_ROOT :=$(PWD)

INC_FLAGS := -I$(SRC_ROOT)/include -I./ -I$(SRC_ROOT)/lib
CFLAGS := -Wall -g $(INC_FLAGS) -lpthread

LIBA := $(SRC_ROOT)/lib/libhi_cipher.a

SRC  := $(wildcard *.c)
HEAD := $(wildcard *.h)
OBJ  := $(SRC:%.c=%)

COMPILE=$(CC) $(CFLAGS) -o "$(*F)" "$<" $(LIBA)

% : %.c
	cp ../src/libhi_cipher.a $(SRC_ROOT)/lib > /dev/null
	@$(COMPILE)


.PHONY : clean all

all: $(OBJ) $(HEAD)
	@echo 

clean:
	@rm -f $(OBJ)

