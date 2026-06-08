# CMSIS Core

set(CMSIS_SOURCES
    Src/system_stm32h7xx.c
)

add_library(cmsis_h7 ${CMSIS_SOURCES})

target_include_directories(cmsis_h7 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/lib/STM32H7xx_HAL_Driver/Inc
)
