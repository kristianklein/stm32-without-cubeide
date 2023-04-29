CC=arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m4 -mthumb --specs=nano.specs
CPPFLAGS=-DSTM32F410Rx \
				 -Ivendor/CMSIS/Device/ST/STM32F4/Include \
				 -Ivendor/CMSIS/CMSIS/Core/Include

LINKER_FILE=linker_script.ld
LDFLAGS=-T $(LINKER_FILE) -u _printf_float

BINARY = blink.elf

PROGRAMMER = openocd
PROGRAMMER_FLAGS = -f interface/stlink.cfg -f target/stm32f4x.cfg

all: $(BINARY)

$(BINARY): main.o startup.o system_stm32f4xx.o syscalls.o usart.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $(BINARY)

main.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

startup.o: startup.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

system_stm32f4xx.o: vendor/CMSIS/Device/ST/STM32F4/Source/Templates/system_stm32f4xx.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

syscalls.o: syscalls.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

usart.o: usart.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

.PHONY: clean
clean:
	rm -f *.o *.elf

flash: $(BINARY)
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "program $(BINARY) verify reset exit"