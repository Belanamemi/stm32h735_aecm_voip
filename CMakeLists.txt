cmake_minimum_required(VERSION 3.20)
project(stm32h735_aecm_voip C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(COMMON_FLAGS "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS} -fdata-sections -ffunction-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections -Wl,--print-memory-usage")

# Directories
include_directories(
    ${CMAKE_SOURCE_DIR}/Core/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Middlewares/NetX_DUO_6.4.0/inc
    ${CMAKE_SOURCE_DIR}/Middlewares/Azure_RTOS/inc
    ${CMAKE_SOURCE_DIR}/Middlewares/WebRTC/aecm/inc
)

# Source files
file(GLOB_RECURSE SOURCES
    "Core/Src/*.c"
    "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c"
    "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c"
    "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sai.c"
    "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c"
    "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c"
    "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c"
    "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c"
)

# Executable
add_executable(aecm_voip.elf ${SOURCES})

# Linker script
target_link_options(aecm_voip.elf PRIVATE 
    -T${CMAKE_SOURCE_DIR}/STM32H735IGKx_FLASH.ld
    -Wl,-Map=${CMAKE_BINARY_DIR}/aecm_voip.map
)

# Libraries
target_link_libraries(aecm_voip.elf
    m
    c
    nosys
)

# Post-build commands
add_custom_command(TARGET aecm_voip.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex aecm_voip.elf aecm_voip.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary aecm_voip.elf aecm_voip.bin
    COMMAND ${CMAKE_SIZE} aecm_voip.elf
    COMMENT "Post-build: Generating .hex and .bin files"
)
