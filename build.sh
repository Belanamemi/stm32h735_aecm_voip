#!/bin/bash
# Build script for STM32H735 AECM VoIP (Linux/macOS)

set -e

echo "=================================="
echo "  STM32H735 AECM VoIP Build"
echo "=================================="

if [ ! -d "build" ]; then
    echo "[CREATE] build directory"
    mkdir build
fi

cd build

echo "[CMAKE] Configuring Release build..."
cmake .. -DCMAKE_BUILD_TYPE=Release

echo "[MAKE] Building firmware..."
make -j$(nproc)

echo ""
echo "=================================="
echo "  Build Complete!"
echo "=================================="
echo "  Artifacts:"
echo "    - ELF:  stm32h735_aecm_voip.elf"
echo "    - BIN:  stm32h735_aecm_voip.bin"
echo "    - HEX:  stm32h735_aecm_voip.hex"
echo "    - MAP:  stm32h735_aecm_voip.map"
echo "    - LSS:  stm32h735_aecm_voip.lss"
echo ""
echo "  Flash: make flash"
echo "  Debug: make ocd_serve  (in one terminal)"
echo "         make gdb_connect (in another terminal)"
echo "=================================="
