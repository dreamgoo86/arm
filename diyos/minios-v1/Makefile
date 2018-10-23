# Makefile for minios-armv7
target=pike2
REVISION= 0.1

OBJS_DIR=./obj
OUT_DIR=./img
SRC_DIR=./src
LIB_DIR=./lib
INC_SRC=$(SRC_DIR)/include 
PLAT_SRC=$(SRC_DIR)/platform
INC_PLAT=$(PLAT_SRC)/$(target)/include 


include $(PLAT_SRC)/$(target)/$(target).mk
ARCH?=armv7-a
CPU?=cortex-a7
LINK?=aarch32.ld
EXEC_STATE?=AARCH32
TOOLCHAIN?= arm-linux-gnueabihf-

CC=${TOOLCHAIN}gcc
LD=${TOOLCHAIN}ld
AS=${TOOLCHAIN}as
OBJCOPY=${TOOLCHAIN}objcopy
OBJDUMP=${TOOLCHAIN}objdump


CFLAGS+= -mcpu=$(CPU)
#CFLAGS+= -marm
CFLAGS+= -g -O0
#CFLAGS+= -mfloat-abi=soft
CFLAGS+= -DMINIOS_VERSION=\"$(REVISION)\"

# Standalone, no libs no start objects.
CFLAGS+= -I$(INC_SRC) 
CFLAGS+= -I$(INC_PLAT) 
CFLAGS+= -I$(SRC_DIR)/arch/$(ARCH)
CFLAGS+= -nodefaultlibs
CFLAGS+= -nostartfiles
CFLAGS+= -nostdlib
CFLAGS+= -D$(EXEC_STATE)
ASFLAGS+= -mcpu=$(CPU)  -g 



# Object files
OBJFILES+= $(OBJS_DIR)/boot.o
OBJFILES+= $(OBJS_DIR)/startup.o
OBJFILES+= $(OBJS_DIR)/exception.o
OBJFILES+= $(OBJS_DIR)/kprintf.o
OBJFILES+= $(OBJS_DIR)/user.o
OBJFILES+= $(OBJS_DIR)/thread.o
OBJFILES+= $(OBJS_DIR)/syscalls.o
OBJFILES+= $(OBJS_DIR)/uart.o
OBJFILES+= $(OBJS_DIR)/dma.o
OBJFILES+= $(OBJS_DIR)/intc.o
OBJFILES+= $(OBJS_DIR)/platform.o
OBJFILES+= $(OBJS_DIR)/irq_handlers.o
OBJFILES+= $(OBJS_DIR)/signal.o
OBJFILES+= $(OBJS_DIR)/rand.o
ifeq ($(ARCH),armv7-a)
	OBJFILES+= $(OBJS_DIR)/malloc4.o
else
	OBJFILES+= $(OBJS_DIR)/malloc8.o
endif
$(OUT_DIR)/boot.bin: $(OUT_DIR)/boot.elf
	$(OBJCOPY) -O binary $< $@
	$(OBJDUMP) -d $< > $(OUT_DIR)/boot.asm
	$(OBJDUMP) -S $< > $(OUT_DIR)/boot.S
	$(OBJDUMP) -t $< > $(OUT_DIR)/boot.sym
#	$(OBJDUMP) -t $< | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $(OUT_DIR)/boot.sym

$(OUT_DIR)/boot.elf:$(OBJFILES)
	$(LD) -T $(LINK)  \
			${OBJFILES} -o $(OUT_DIR)/boot.elf -Map=$(OUT_DIR)/boot.map  # -L./lib/ -lgcc

$(OBJS_DIR)/%.o:$(SRC_DIR)/kernel/%.c 
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJS_DIR)/%.o:$(SRC_DIR)/drivers/uart/%.c
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJS_DIR)/%.o:$(SRC_DIR)/drivers/dma/%.c
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJS_DIR)/%.o:$(SRC_DIR)/drivers/intc/%.c
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJS_DIR)/%.o:$(PLAT_SRC)/$(target)/%.c
	$(CC) $(CFLAGS) -c $< -o $@	
$(OBJS_DIR)/%.o:$(SRC_DIR)/arch/$(ARCH)/%.S
	$(AS) $< -o $@ $(ASFLAGS)

clean:
	-rm -f $(OUT_DIR)/*  $(OBJS_DIR)/*.o 

