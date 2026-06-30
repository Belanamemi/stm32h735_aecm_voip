# Source files
set(SOURCES
    main.c
    aecm_processor.c
    voip_codec_g711.c
    voip_rtp_handler.c
    jitter_buffer.c
    agc_preprocessor.c
    dwt_delay_measurement.c
    itm_telemetry.c
    syscalls.c
	sysmem.c
    stm32h7xx_hal_msp.c
	../Core/Startup/startup_stm32h735igkx.s
)

# Optimization level
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(OPT_LEVEL -O0)
else()
    set(OPT_LEVEL -Os)
endif()

# Output files
set(ELF_FILE ${APPLICATION_NAME}.elf)
set(BIN_FILE ${APPLICATION_NAME}.bin)
set(HEX_FILE ${APPLICATION_NAME}.hex)
set(MAP_FILE ${APPLICATION_NAME}.map)
set(LSS_FILE ${APPLICATION_NAME}.lss)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../Core/Inc
    ../lib/STM32H7xx_HAL_Driver/Inc
    ../lib/CMSIS/Inc
    ../lib/BSP/Components/WM8994
    ../Middlewares/WebRTC/aecm/inc

)

# Create ELF
add_executable(${ELF_FILE} ${SOURCES})

 #aecm_h7    netxduo
# Link libraries BAA Флаг nosys.specs подключает заглушки для этих функций.
target_link_libraries(${ELF_FILE} PUBLIC
    stm32h7xx_hal
    cmsis_h7
    m
    c
    netxduo
    aecm_h7
    bsp_h7
    nosys
)

# Compiler flags
target_compile_options(${ELF_FILE} PRIVATE
    ${OPT_LEVEL}
    -Wall -Wextra
)

# Linker flags   BAA --specs=nano.specs
target_link_options(${ELF_FILE} PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${MAP_FILE},--cref,--no-warn-mismatch
    -Wl,--gc-sections
    -Wl,--print-memory-usage
)

# Show size
add_custom_target(${ELF_FILE}-size ALL DEPENDS ${ELF_FILE}
    COMMAND ${ARM_SIZE_EXECUTABLE} -B ${ELF_FILE}
    COMMENT "[SIZE] Firmware size:"
)

# Generate assembly listing
add_custom_target(${LSS_FILE} DEPENDS ${ELF_FILE}
    COMMAND ${ARM_OBJDUMP_EXECUTABLE} -S ${ELF_FILE} > ${LSS_FILE}
    COMMENT "[LISTING] Generating assembly listing: ${LSS_FILE}"
)

# Generate HEX file
add_custom_target(${HEX_FILE} DEPENDS ${ELF_FILE}
    COMMAND ${ARM_OBJCOPY_EXECUTABLE} -Oihex ${ELF_FILE} ${HEX_FILE}
    COMMENT "[HEX] Generating: ${HEX_FILE}"
)

# Generate BIN file
add_custom_target(${BIN_FILE} DEPENDS ${ELF_FILE}
    COMMAND ${ARM_OBJCOPY_EXECUTABLE} -Obinary ${ELF_FILE} ${BIN_FILE}
    COMMENT "[BIN] Generating: ${BIN_FILE}"
)

# Main target
add_custom_target(${APPLICATION_NAME} ALL DEPENDS
    ${ELF_FILE}-size
    ${BIN_FILE}
    ${HEX_FILE}
    ${LSS_FILE}
)

# Flash target
if(OPENOCD_FOUND)
    add_custom_target(flash DEPENDS ${ELF_FILE}
        COMMAND ${OPENOCD_EXECUTABLE}
            -f ${OPENOCD_CONFIG}
            --command "program ${ELF_FILE} reset exit"
        COMMENT "[FLASH] Programming device..."
    )
    
    # OpenOCD server
    add_custom_target(ocd_serve DEPENDS ${ELF_FILE}
        COMMAND ${OPENOCD_EXECUTABLE}
            -f ${OPENOCD_CONFIG}
            --command "gdb_port ${GDB_DEBUG_PORT}"
        COMMENT "[OpenOCD] Serving GDB on port ${GDB_DEBUG_PORT}..."
    )
    
    # GDB connect
    add_custom_target(gdb_connect DEPENDS ${ELF_FILE}
        COMMAND ${ARM_GDB_EXECUTABLE}
            -ex "target remote localhost:${GDB_DEBUG_PORT}"
            -ex "monitor reset halt"
            ${ELF_FILE}
        COMMENT "[GDB] Connecting to target..."
    )
endif()

# Clean build artifacts
add_custom_target(clean_build
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "[CLEAN] Removing build directory"
)
