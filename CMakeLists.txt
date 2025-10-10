cmake_minimum_required(VERSION 3.15)

project(stm32-without-cubeide
        LANGUAGES C)

add_executable(blink
    main.c
    startup.c
    syscalls.c
    usart.c
    vendor/CMSIS/Device/ST/STM32F4/Source/Templates/system_stm32f4xx.c)

target_compile_options(blink PRIVATE
    --specs=nano.specs)

set_target_properties(blink PROPERTIES OUTPUT_NAME "blink.elf")

target_include_directories(blink PRIVATE
    vendor/CMSIS/Device/ST/STM32F4/Include
	vendor/CMSIS/CMSIS/Core/Include)

target_compile_definitions(blink 
    PRIVATE STM32F410Rx)

target_link_options(blink 
    PRIVATE -T ${CMAKE_SOURCE_DIR}/linker_script.ld 
    PRIVATE -u _printf_float)

set(PROGRAMMER openocd)
set(PROGRAMMER_FLAGS -f interface/stlink.cfg -f target/stm32f4x.cfg)

add_custom_target(flash
    COMMAND ${PROGRAMMER} ${PROGRAMMER_FLAGS} -c "program $<TARGET_FILE:blink> verify reset exit"
    DEPENDS blink
    VERBATIM)
