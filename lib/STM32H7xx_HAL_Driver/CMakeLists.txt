# STM32H7xx HAL Driver Library

set(DRIVER_SOURCES
    Src/stm32h7xx_hal.c
    Src/stm32h7xx_hal_i2c.c
	Src/stm32h7xx_hal_i2c_ex.c
    Src/stm32h7xx_hal_sai.c
    Src/stm32h7xx_hal_sai_ex.c
    Src/stm32h7xx_hal_eth.c
    Src/stm32h7xx_hal_dma.c
    Src/stm32h7xx_hal_gpio.c
    Src/stm32h7xx_hal_rcc.c
    Src/stm32h7xx_hal_rcc_ex.c
    Src/stm32h7xx_hal_cortex.c
<<<<<<< HEAD
    Src/stm32h7xx_hal_sd.c
    Src/stm32h7xx_hal_sd_ex.c
    Src/stm32h7xx_hal_mmc.c
    Src/stm32h7xx_hal_mmc_ex.c
    Src/stm32h7xx_hal_pwr.c
    Src/stm32h7xx_hal_pwr_ex.c
    Src/stm32h7xx_hal_cortex.c
    Src/stm32h7xx_ll_sdmmc.c

=======
    Src/stm32h7xx_hal_rcc_ex.c
>>>>>>> 9b48788c68f9afe210994e7ed4f34295090f6f8b
)

add_library(stm32h7xx_hal ${DRIVER_SOURCES})

# BAA add: ${CMAKE_SOURCE_DIR}/Core/Inc
target_include_directories(stm32h7xx_hal PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/Inc
   ${CMAKE_CURRENT_SOURCE_DIR}/Inc/Legacy
   ${CMAKE_SOURCE_DIR}/Core/Inc
)

target_link_libraries(stm32h7xx_hal PRIVATE cmsis_h7)
