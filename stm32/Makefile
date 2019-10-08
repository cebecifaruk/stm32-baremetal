all: build/main.o
	arm-none-eabi-ld -Tflash.ld -nostartfiles build/main.o -o build/firmware.elf
	arm-none-eabi-objcopy -O binary build/firmware.elf build/firmware.bin
build/main.o: clean
	arm-none-eabi-as main.s -o build/main.o
clean:
	rm build/main.o build/firmware.bin
flash:
	st-flash write build/firmware.bin 0x08000000
