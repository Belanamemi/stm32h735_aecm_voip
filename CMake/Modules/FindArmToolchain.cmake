# Find ARM GNU Toolchain

find_program(ARM_CC_EXECUTABLE arm-none-eabi-gcc)
find_program(ARM_CXX_EXECUTABLE arm-none-eabi-g++)
find_program(ARM_OBJCOPY_EXECUTABLE arm-none-eabi-objcopy)
find_program(ARM_OBJDUMP_EXECUTABLE arm-none-eabi-objdump)
find_program(ARM_SIZE_EXECUTABLE arm-none-eabi-size)
find_program(ARM_GDB_EXECUTABLE arm-none-eabi-gdb)

if(ARM_CC_EXECUTABLE AND ARM_CXX_EXECUTABLE)
    set(ArmToolchain_FOUND TRUE)
    message(STATUS "[FOUND] ARM Toolchain: ${ARM_CC_EXECUTABLE}")
else()
    set(ArmToolchain_FOUND FALSE)
    message(FATAL_ERROR "ARM toolchain not found. Install: GNU ARM Embedded Toolchain")
endif()

include(CMakeForceCompiler)

set(CMAKE_C_COMPILER ${ARM_CC_EXECUTABLE})
set(CMAKE_CXX_COMPILER ${ARM_CXX_EXECUTABLE})
set(CMAKE_ASM_COMPILER ${ARM_CC_EXECUTABLE})

set(CMAKE_FIND_ROOT_PATH /usr/arm-none-eabi)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(COMMON_FLAGS "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard")
set(COMMON_FLAGS "${COMMON_FLAGS} -Wall -Wextra -Wno-unused-parameter")
set(COMMON_FLAGS "${COMMON_FLAGS} -fdata-sections -ffunction-sections -fno-common")

set(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++11")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections,--print-memory-usage,--warn-common")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc -lm -lnosys")
