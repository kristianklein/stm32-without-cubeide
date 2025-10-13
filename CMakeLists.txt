cmake_minimum_required(VERSION 3.15)

project(stm32-without-cubeide
        LANGUAGES C ASM)

add_executable(blink
    main.c
    syscalls.c
    usart.c)

include(cmake/stm32cubef4.cmake)
target_link_libraries(blink stm32cubef4)

target_compile_options(blink PRIVATE
    --specs=nano.specs -g)
    
set_target_properties(blink PROPERTIES OUTPUT_NAME "blink.elf")

target_link_options(blink 
    PRIVATE -T ${CMAKE_SOURCE_DIR}/linker_script.ld 
    PRIVATE -u _printf_float)

set(PROGRAMMER openocd)
set(PROGRAMMER_FLAGS -f interface/stlink.cfg -f target/stm32f4x.cfg)

add_custom_target(flash
    COMMAND ${PROGRAMMER} ${PROGRAMMER_FLAGS} -c "program $<TARGET_FILE:blink> verify reset exit"
    DEPENDS blink
    VERBATIM)
