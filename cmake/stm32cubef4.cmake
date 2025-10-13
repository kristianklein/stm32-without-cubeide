
set(STM32CUBEF4_DIR "${CMAKE_SOURCE_DIR}/vendor/STM32CubeF4")
set(HAL_ROOT_DIR "${STM32CUBEF4_DIR}/Drivers/STM32F4xx_HAL_Driver")
set(STM32F410Rx_STARTUP_FILE "${STM32CUBEF4_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f410rx.s")
set(HAL_SOURCE_DIR "${HAL_ROOT_DIR}/Src")
set(HAL_INCLUDE_DIR "${HAL_ROOT_DIR}/Inc")
set(CMSIS_SYSTEM_FILE "${STM32CUBEF4_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c")

set(HAL_SOURCES
    ${CMSIS_SYSTEM_FILE}
    "${HAL_SOURCE_DIR}/stm32f4xx_hal.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_rcc.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_cortex.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_gpio.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_uart.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_dma.c"
    ${STM32F410Rx_STARTUP_FILE})

set_source_files_properties(${STM32F410Rx_STARTUP_FILE} PROPERTIES LANGUAGE ASM)

add_library(stm32cubef4 STATIC ${HAL_SOURCES} ${HAL_HEADERS})

target_include_directories(stm32cubef4 PUBLIC
    ${STM32CUBEF4_DIR}/Drivers/CMSIS/Core/Include
    ${STM32CUBEF4_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${STM32CUBEF4_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}
)

target_compile_definitions(stm32cubef4
    PUBLIC STM32F410Rx USE_HAL_DRIVER)