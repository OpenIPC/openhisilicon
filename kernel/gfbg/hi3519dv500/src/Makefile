# Enum the C files needed to be compiled, using the relative path
SRCS+=src/gfbg_main.c
ifdef CONFIG_OT_PROC_SHOW_SUPPORT
SRCS+=src/gfbg_proc.c
endif
ifeq ($(OSTYPE),linux)
    SRCS+=src/init/linux/gfbg_init.c
else ifeq ($(OSTYPE),liteos)
    SRCS+=src/init/liteos/gfbg_init.c
endif
