# STM32 Makefile Script (coppied from Gazzali Kernel Project)
# Author: Faruk CEBECI <cebecifaruk@gmail.com>

CC	= arm-none-eabi-gcc
LD	= arm-none-eabi-gcc
GDB	= arm-none-eabi-gdb
OBJCOPY	= arm-none-eabi-objcopy
SIZE	= arm-none-eabi-size

SOURCES	= src/startup_stm32f10x.s main.c


BUILDDIR= build
OUTNAME	= main


OBJECTS	= 	$(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

INCLUDES=	./ 


ELF = $(BUILDDIR)/$(OUTNAME).elf
HEX = $(BUILDDIR)/$(OUTNAME).hex
BIN = $(BUILDDIR)/$(OUTNAME).bin


CFLAGS  = -g -Wall -I. -std=c99 -nostdlib -fno-builtin -nodefaultlibs \
	-mlittle-endian -mthumb -mcpu=cortex-m3 -nostartfiles \
	$(addprefix -I,$(INCLUDES))
				 
				


LDSCRIPT    :=  stm32_flash.ld
LDFLAGS     :=  -Wl,-M=$(OBJDIR)/$(OUTNAME).map -g -T$(LDSCRIPT) $(CFLAGS)


all: $(BIN) $(HEX) size

$(BIN): $(ELF)
	$(OBJCOPY) -O 	binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O 	ihex $< $@

$(ELF): $(OBJECTS)
	$(LD) 		$(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

$(BUILDDIR)/%.o: %.c
	@mkdir -p 	$(dir $@)
	$(CC) 		$(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: %.s
	@mkdir -p 	$(dir $@)
	$(CC) 		$(CFLAGS) -c $< -o $@

size: $(ELF)
	@$(SIZE) 	$(ELF)

clean:
	@rm -rf 	$(BUILDDIR)


# Flash And Debug (OpenOCD)
debug-gdbserver: $(BIN)
	openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg 

debug-gdb: $(ELF)
	@$(GDB) 	$(ELF) -ex="target remote localhost:3333"
debug-load: $(ELF)
	@$(GDB) 	$(ELF) -ex="target remote localhost:3333" -ex="load" -ex="monitor reset init" --batch
debug-flash: $(BIN)
	openocd -f board/stm32f4discovery.cfg -c "program $(BIN) 0x08000000 verify"

