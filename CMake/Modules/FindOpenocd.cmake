# Find OpenOCD

find_program(OPENOCD_EXECUTABLE openocd)

if(OPENOCD_EXECUTABLE)
    set(Openocd_FOUND TRUE)
    message(STATUS "[FOUND] OpenOCD: ${OPENOCD_EXECUTABLE}")
else()
    set(Openocd_FOUND FALSE)
    message(STATUS "[WARN] OpenOCD not found (flash target will not work)")
endif()
