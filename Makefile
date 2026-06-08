# Makefile wrapper for CMake build system
# This provides convenient build targets

.PHONY: all build release debug clean flash gdb-serve gdb-connect

BUILD_DIR := build
DEBUG_BUILD_DIR := build-debug

all: release

release:
	@echo "[BUILD] Release mode"
	@if [ ! -d "$(BUILD_DIR)" ]; then mkdir $(BUILD_DIR); fi
	@cd $(BUILD_DIR) && cmake .. -DCMAKE_BUILD_TYPE=Release && make

debug:
	@echo "[BUILD] Debug mode"
	@if [ ! -d "$(DEBUG_BUILD_DIR)" ]; then mkdir $(DEBUG_BUILD_DIR); fi
	@cd $(DEBUG_BUILD_DIR) && cmake .. -DCMAKE_BUILD_TYPE=Debug && make

build: release

flash:
	@cd $(BUILD_DIR) && make flash

ocd_serve:
	@cd $(BUILD_DIR) && make ocd_serve

gdb_connect:
	@cd $(BUILD_DIR) && make gdb_connect

clean:
	@echo "[CLEAN] Removing build directories"
	@rm -rf $(BUILD_DIR) $(DEBUG_BUILD_DIR)
	clean_build: clean

help:
	@echo "STM32H735 AECM VoIP - Build Targets:"
	@echo "  make              - Build release firmware"
	@echo "  make debug        - Build debug firmware (-O0)"
	@echo "  make flash        - Flash firmware to device"
	@echo "  make ocd_serve    - Start OpenOCD server"
	@echo "  make gdb_connect  - Connect GDB debugger"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make help         - Show this help"
