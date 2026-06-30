# STM32H7xx HAL Driver Library

set(BSP_SOURCES
    
    Components/WM8994/wm8994_codec.c
)

add_library( bsp_h7 ${BSP_SOURCES})

# BAA add: ${CMAKE_SOURCE_DIR}/Core/Inc
target_include_directories( bsp_h7 PUBLIC
  # ${CMAKE_SOURCE_DIR}/lib/STM32H7xx_HAL_Driver/Inc
   ${CMAKE_SOURCE_DIR}/Core/Inc
  # ${CMAKE_SOURCE_DIR}/lib/CMSIS/Include
  # ${CMAKE_SOURCE_DIR}/lib/CMSIS/Device/ST/STM32H7xx/Include
   ${CMAKE_CURRENT_SOURCE_DIR}/Components/WM8994 
)

target_link_libraries(bsp_h7 PRIVATE cmsis_h7 stm32h7xx_hal)
