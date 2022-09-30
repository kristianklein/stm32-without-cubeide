CC=arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m4 -mthumb -nostdlib -DSTM32F410Rx -g
LINKER_FILE=linker_script.ld
BINARY = blink.elf
PROGRAMMER = openocd
PROGRAMMER_FLAGS = -f interface/stlink.cfg -f target/stm32f4x.cfg
INCLUDE_PATHS = -I./vendor/CMSIS/Device/ST/STM32F4/Include \
								-I./vendor/CMSIS/CMSIS/Core/Include
OUTPUT_DIR = build

.PHONY: output_dir

all: output_dir main.o startup.o system_stm32f4xx.o
	mkdir -p bin
	$(CC) $(CFLAGS) $(OUTPUT_DIR)/main.o $(OUTPUT_DIR)/startup.o $(OUTPUT_DIR)/system_stm32f4xx.o -o $(OUTPUT_DIR)/$(BINARY) -T $(LINKER_FILE)

startup.o: startup.c
	$(CC) $(CFLAGS) startup.c -o $(OUTPUT_DIR)/startup.o -c

main.o: main.c
	$(CC) $(CFLAGS) main.c -o $(OUTPUT_DIR)/main.o -c $(INCLUDE_PATHS)

system_stm32f4xx.o: vendor\CMSIS\Device\ST\STM32F4\Source\Templates\system_stm32f4xx.c
	$(CC) $(CFLAGS) -c $< -o $(OUTPUT_DIR)/$@ $(INCLUDE_PATHS)

output_dir:
	mkdir -p $(OUTPUT_DIR)

clean:
	rm -rf *.o *.elf bin/

rebuild: clean all

flash:
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "program $(OUTPUT_DIR)/$(BINARY) verify reset exit"

debug:
	$(PROGRAMMER) $(PROGRAMMER_FLAGS)