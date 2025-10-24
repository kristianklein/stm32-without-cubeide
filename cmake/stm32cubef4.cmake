set(STM32CUBEF4_DIR "${CMAKE_SOURCE_DIR}/vendor/STM32CubeF4")
set(HAL_ROOT_DIR "${STM32CUBEF4_DIR}/Drivers/STM32F4xx_HAL_Driver")
set(HAL_SOURCE_DIR "${HAL_ROOT_DIR}/Src")
set(HAL_INCLUDE_DIR "${HAL_ROOT_DIR}/Inc")

set(HAL_SOURCES
    "${HAL_SOURCE_DIR}/stm32f4xx_hal.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_rcc.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_cortex.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_gpio.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_uart.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_dma.c")

add_library(stm32cubef4 STATIC ${HAL_SOURCES})

target_include_directories(stm32cubef4 PUBLIC
    ${STM32CUBEF4_DIR}/Drivers/CMSIS/Core/Include
    ${STM32CUBEF4_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${STM32CUBEF4_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/core)
