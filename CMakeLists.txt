cmake_minimum_required(VERSION 3.20)

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose build type" FORCE)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "[BUILD] Debug mode - Optimization: -O0")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "[BUILD] Release mode - Optimization: -Os")
endif()

# Set CMake module search path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

# Find tools
find_package(Openocd)
find_package(ArmToolchain REQUIRED)

# Project settings

#set(CMAKE_SYSTEM_NAME Generic) 
#set(CMAKE_C_COMPILER_WORKS 1)
#set(CMAKE_CXX_COMPILER_WORKS 1)
#set(CMAKE_DETERMINE_ASM_ABI_COMPILED 1)


set(APPLICATION_NAME "stm32h735_aecm_voip")
set(DEVICE_FAMILY STM32H735xx)
set(HSE_VALUE 25000000)  

project(${APPLICATION_NAME} C CXX ASM)

# Linker script
get_filename_component(LINKER_SCRIPT linker/STM32H735IGKX_FLASH.ld ABSOLUTE)

# OpenOCD config
get_filename_component(OPENOCD_CONFIG tools/stm32h735g-dk.cfg ABSOLUTE)

# Add libraries
add_subdirectory(lib)

# Add source
add_subdirectory(src)
add_subdirectory(NetXDuo)
#add_subdirectory(Middlewares/netxduo)
add_subdirectory(Middlewares/WebRTC/aecm)


message(STATUS "========================================")
message(STATUS "  Project: ${APPLICATION_NAME}")
message(STATUS "  Device:  ${DEVICE_FAMILY}")
message(STATUS "  HSE:     ${HSE_VALUE} Hz")
message(STATUS "  Build:   ${CMAKE_BUILD_TYPE}")
message(STATUS "========================================")
