# STM32H7xx HAL Driver Library

set(DRIVER_SOURCES
    Src/stm32h7xx_hal.c
    Src/stm32h7xx_hal_i2c.c
    Src/stm32h7xx_hal_sai.c
    Src/stm32h7xx_hal_eth.c
    Src/stm32h7xx_hal_dma.c
    Src/stm32h7xx_hal_gpio.c
    Src/stm32h7xx_hal_rcc.c
    Src/stm32h7xx_hal_uart.c
    Src/stm32h7xx_hal_cortex.c
)

add_library(stm32h7xx_hal ${DRIVER_SOURCES})

target_include_directories(stm32h7xx_hal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(stm32h7xx_hal PRIVATE cmsis_h7)
