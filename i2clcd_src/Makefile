OBJS = $(filter-out main.o,$(patsubst %.c,%.o,$(wildcard *.c)))
OBJS += $(patsubst app/%.c,app/%.o,$(wildcard app/*.c))
OBJS += $(patsubst hal/%.c,hal/%.o,$(wildcard hal/*.c))


BINARY = main

OPENCM3_DIR	= ../.opencm3/libopencm3
LDSCRIPT	= ./hal/stm32f4discovery.ld
LDLIBS		+= -lm
LDFLAGS		+= -L./
DEFS		+= -I./hal/ -I./app/ -I./

# Target configuration
LIBNAME		= opencm3_stm32f4
DEFS		+= -DSTM32F4
FP_FLAGS	?= -mfloat-abi=hard -mfpu=fpv4-sp-d16
ARCH_FLAGS	= -mthumb -mcpu=cortex-m4 $(FP_FLAGS)

# OpenOCD specific variables
OOCD		?= openocd
OOCD_INTERFACE	?= stlink-v2-1
OOCD_TARGET	?= stm32f4x

include ../.opencm3/libopencm3.rules.mk
