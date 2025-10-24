cmake_minimum_required(VERSION 3.15)

project(stm32-without-cubeide
        LANGUAGES C ASM)

add_compile_definitions(USE_HAL_DRIVER)
add_compile_options(-g -O0) # Global compile flags

set(BLINK_SOURCES
    main.c
    core/syscalls.c
    core/startup_stm32f410rx.s
    core/system_stm32f4xx.c
    core/stm32f4xx_hal_msp.c
    core/stm32f4xx_it.c)

add_executable(blink ${BLINK_SOURCES})

set_target_properties(blink PROPERTIES OUTPUT_NAME "blink.elf")

target_include_directories(blink PRIVATE 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/core)

target_compile_options(blink PRIVATE
    --specs=nano.specs)

target_link_options(blink PRIVATE
    -T ${CMAKE_SOURCE_DIR}/core/STM32F410RBTx_FLASH.ld 
    -u _printf_float)

include(cmake/stm32cubef4.cmake)
target_link_libraries(blink stm32cubef4)

set(PROGRAMMER openocd)
set(PROGRAMMER_FLAGS -f interface/stlink.cfg -f target/stm32f4x.cfg)

add_custom_target(flash
    COMMAND ${PROGRAMMER} ${PROGRAMMER_FLAGS} -c "program $<TARGET_FILE:blink> verify reset exit"
    DEPENDS blink
    VERBATIM)
