# CMSIS Core

add_compile_definitions(STM32H735xx)

set(CMSIS_SOURCES
    Src/system_stm32h7xx.c
)

add_library(cmsis_h7 ${CMSIS_SOURCES})

target_include_directories(cmsis_h7 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
	${CMAKE_CURRENT_SOURCE_DIR}/Device/ST/STM32H7xx/Include
    #${CMAKE_SOURCE_DIR}/lib/STM32H7xx_HAL_Driver/Inc
)

target_link_libraries(cmsis_h7 PRIVATE stm32h7xx_hal)