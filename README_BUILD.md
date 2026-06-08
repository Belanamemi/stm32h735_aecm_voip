# STM32H735 AECM VoIP - Build System

## 📋 Overview

Professional CMake + Makefile build system based on **STM32F4Template** architecture.

- ✅ **CMake 3.15+** for cross-platform builds
- ✅ **GNU ARM Toolchain** (arm-none-eabi-*)
- ✅ **STM32H735IGKx** MCU support
- ✅ **Release & Debug** builds
- ✅ **OpenOCD** integration (flash & GDB)
- ✅ **Linux/macOS/Windows** compatible

---

## 🛠️ Prerequisites

### 1. Install CMake
```bash
# Linux (Ubuntu/Debian)
sudo apt-get install cmake

# macOS
brew install cmake

# Windows
# Download from https://cmake.org/download/
```

### 2. Install ARM GNU Toolchain
```bash
# Linux
sudo apt-get install arm-none-eabi-gcc arm-none-eabi-gdb

# macOS
brew install arm-none-eabi-gcc

# Windows
# Download from: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm
# Add to PATH
```

### 3. Install OpenOCD (Optional - for flashing)
```bash
# Linux
sudo apt-get install openocd

# macOS
brew install openocd

# Windows
# Download from: http://openocd.org/
```

---

## 🚀 Quick Start

### Linux/macOS

```bash
# 1. Clone repository
git clone https://github.com/Belanamemi/stm32h735_aecm_voip.git
cd stm32h735_aecm_voip

# 2. Build Release
chmod +x build.sh
./build.sh

# 3. Build Debug
chmod +x build-debug.sh
./build-debug.sh

# 4. Flash (requires OpenOCD)
cd build
make flash
```

### Windows

```cmd
# 1. Clone repository
git clone https://github.com/Belanamemi/stm32h735_aecm_voip.git
cd stm32h735_aecm_voip

# 2. Build Release
build.bat

# 3. Build Debug
build-debug.bat

# 4. Flash (requires OpenOCD)
cd build
make flash
```

---

## 📁 Project Structure

```
stm32h735_aecm_voip/
├── CMakeLists.txt              # Root CMake config
├── Makefile                    # Convenience Makefile wrapper
├── build.sh / build.bat        # Build scripts
├── build-debug.sh / build-debug.bat
├── clean.sh / clean.bat
│
├── CMake/
│   └── Modules/
│       ├── FindArmToolchain.cmake
│       ├── FindOpenocd.cmake
│       └── FindArm.cmake
│
├── lib/
│   ├── CMakeLists.txt
│   ├── STM32H7xx_HAL_Driver/
│   │   ├── CMakeLists.txt
│   │   ├── Inc/
│   │   └── Src/
│   └── CMSIS/
│       ├── CMakeLists.txt
│       ├── Inc/
│       └── Src/
│
├── src/
│   ├── CMakeLists.txt          # Compile flags & targets
│   ├── main.c
│   ├── aecm_processor.c
│   ├── voip_codec_g711.c
│   ├── voip_rtp_handler.c
│   ├── jitter_buffer.c
│   ├── agc_preprocessor.c
│   ├── dwt_delay_measurement.c
│   ├── itm_telemetry.c
│   ├── wm8994_codec.c
│   └── startup_stm32h735xx.s
│
├── Core/
│   └── Inc/ (header files)
│
├── linker/
│   └── STM32H735IGKx_FLASH.ld
│
├── tools/
│   └── stm32h735g-dk.cfg       # OpenOCD config
│
└── Middlewares/
    ├── WebRTC/aecm/
    └── NetX_DUO/
```

---

## 🎯 Build Targets

### Using Makefile

```bash
make              # Build Release (default)
make debug        # Build Debug (-O0, no optimization)
make flash        # Flash to device (OpenOCD)
make ocd_serve    # Start OpenOCD server
make gdb_connect  # Connect GDB debugger
make clean        # Remove all build artifacts
make help         # Show all targets
```

### Using CMake directly

```bash
# Create build directory
mkdir build && cd build

# Configure Release
cmake .. -DCMAKE_BUILD_TYPE=Release

# Configure Debug
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build
make -j4

# Or use cmake --build
cmake --build . -j4
```

---

## 📊 Build Output

After successful build:

```
build/
├── stm32h735_aecm_voip.elf      ← Main executable
├── stm32h735_aecm_voip.bin      ← Binary for flashing
├── stm32h735_aecm_voip.hex      ← Intel HEX format
├── stm32h735_aecm_voip.map      ← Linker map file
├── stm32h735_aecm_voip.lss      ← Assembly listing
└── CMakeFiles/                  ← Build artifacts
```

### Size Report

```
text    data     bss     dec     hex filename
84320   2048   48128  134496   20d60 stm32h735_aecm_voip.elf
```

---

## 🔧 Configuration

### CMakeLists.txt (Root)

```cmake
# Device and clock settings
set(DEVICE_FAMILY STM32H735xx)
set(HSE_VALUE 25000000)  # 25 MHz crystal
```

### src/CMakeLists.txt

```cmake
# Add source files
set(SOURCES
    main.c
    aecm_processor.c
    voip_codec_g711.c
    # ... add more sources
)

# Include paths
include_directories(
    ../Core/Inc
    ../lib/STM32H7xx_HAL_Driver/Inc
    # ... add more includes
)

# Optimization level
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(OPT_LEVEL -O0)  # No optimization
else()
    set(OPT_LEVEL -Os)  # Optimize for size
endif()
```

---

## 🐛 Debugging

### Terminal 1: Start OpenOCD

```bash
make ocd_serve
# or
cd build && make ocd_serve
```

### Terminal 2: Connect GDB

```bash
make gdb_connect
# or
cd build && make gdb_connect
```

### GDB Commands

```
(gdb) break main
(gdb) continue
(gdb) step
(gdb) next
(gdb) print variable_name
(gdb) monitor reset halt
(gdb) quit
```

---

## ⚙️ Optimization Levels

| Flag | Purpose | Use Case |
|------|---------|----------|
| `-O0` | No optimization | **Debug** - easier debugging |
| `-O1` | Minimal opt | Smaller code, fast compile |
| `-O2` | Balanced opt | Good speed/size tradeoff |
| `-Os` | Size opt | **Release** - smallest code |
| `-Ofast` | Maximum opt | Fastest code (may lose precision) |

---

## 🔗 Linker Flags

```cmake
# Generated linker flags
-T${LINKER_SCRIPT}           # Use custom linker script
-Wl,-Map=${MAP_FILE}         # Generate map file
-Wl,--gc-sections            # Remove unused sections
-Wl,--print-memory-usage     # Show memory usage
-lm -lc -lnosys              # Link math, C, and no syscalls libs
```

---

## 🚨 Troubleshooting

### CMake not found
```bash
# Install CMake
sudo apt-get install cmake

# Verify
cmake --version
```

### ARM toolchain not found
```bash
# Install ARM GCC
sudo apt-get install arm-none-eabi-gcc arm-none-eabi-gdb

# Verify
arm-none-eabi-gcc --version
```

### OpenOCD connection failed
```bash
# Check ST-Link connection
lsusb | grep STMicroelectronics

# Verify OpenOCD config
opencd -f tools/stm32h735g-dk.cfg -c "init"
```

### Linker errors
- Check linker script: `linker/STM32H735IGKx_FLASH.ld`
- Verify RAM/Flash sizes match device
- Ensure all symbols are defined

---

## 📚 References

- **CMake**: https://cmake.org/
- **GNU ARM Toolchain**: https://developer.arm.com/
- **OpenOCD**: http://openocd.org/
- **STM32H735 Reference**: STM32H735xx datasheet
- **STM32F4Template**: https://github.com/ahessling/STM32F4Template

---

## 📝 License

MIT License

---

**Happy Building! 🚀**
