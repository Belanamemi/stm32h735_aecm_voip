@echo off
REM Build script for STM32H735 AECM VoIP (Windows)

echo ==================================
echo   STM32H735 AECM VoIP Build
echo ==================================

if not exist "build" (
    echo [CREATE] build directory
    mkdir build
)

cd build

echo [CMAKE] Configuring Release build...
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release

echo [MAKE] Building firmware...
make -j4

echo.
echo ==================================
echo   Build Complete!
echo ==================================
echo   Artifacts:
echo     - ELF: stm32h735_aecm_voip.elf
echo     - BIN: stm32h735_aecm_voip.bin
echo     - HEX: stm32h735_aecm_voip.hex
echo.
echo   Flash: make flash
echo ==================================

pause
