# STM32H735 AECM VoIP System

## 🎯 Project Overview

Complete point-to-point VoIP application for **STM32H735G-DK** dual development kits with:

- **WebRTC AECM** (Acoustic Echo Control Mobile)
- **G.711 a-law** codec @ 8 kHz
- **NetX DUO 6.4.0** (Azure RTOS)
- **WM8994** stereo codec (Microphone CN11 + Headphone HP1L/R)
- **Adaptive jitter buffer** with auto-tuning
- **Pre-AECM AGC** (Automatic Gain Control)
- **ERLE metrics** via ITM printf (SWO debug)
- **DWT-based delay measurement** (cycle-accurate @ 520 MHz)

---

## 📦 Hardware Requirements

- **2x STM32H735G-DK** Discovery kits
- **2x Microphone** (blue jack CN11)
- **2x Headphones** (HP1L/R 3.5mm)
- **Ethernet LAN8742A** (on-board)
- **ST-Link V3** debugger with SWO

---

## 🔧 Build Instructions

### Option 1: STM32CubeIDE 2.0.0

```bash
# Clone repository
git clone https://github.com/Belanamemi/stm32h735_aecm_voip.git
cd stm32h735_aecm_voip

# Open in STM32CubeIDE
# File → Import → General → Existing Projects into Workspace
# Select root directory

# Build
Project → Build Project
```

### Option 2: CMake (Linux/Windows)

```bash
mkdir build && cd build
cmake .. -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../CMake/Modules/FindArmToolchain.cmake
make -j4
```

---

## ⚙️ Configuration

### Static IP Setup

Edit `Core/Inc/aecm_config.h`:

```c
#define NETX_IP_ADDRESS         IP_ADDRESS(192, 168, 1, 100)   /* Board 1 */
#define NETX_REMOTE_IP          IP_ADDRESS(192, 168, 1, 101)   /* Board 2 */
#define NETX_REMOTE_PORT        16384                           /* RTP port */
```

### Audio Settings

- **Sample Rate**: 8 kHz (fixed for G.711)
- **Frame Size**: 160 samples (20ms)
- **Channels**: Stereo (but mono VoIP processing)
- **Codec**: G.711 a-law (ITU-T standard)

---

## 🎤 Microphone/Headphone Pinout

### WM8994 Stereo Codec (STM32H735G-DK)

| Signal | CN11 (Mic Jack) | HP1L/R (Headphone) |
|--------|-----------------|--------------------|
| Left   | Ring 1          | Left               |
| Right  | Ring 2          | Right              |
| GND    | Sleeve          | GND                |

---

## 📊 ERLE Metrics (ITM Console)

Connect STM32CubeIDE ITM console (SWO @ 8 MHz):

```
Frame 50  | ERLE: 12.5 dB | Jitter: 25.3 ms | Latency: 2.34 ms
Frame 60  | ERLE: 11.8 dB | Jitter: 24.1 ms | Latency: 2.41 ms
Frame 70  | ERLE: 13.2 dB | Jitter: 26.5 ms | Latency: 2.38 ms
```

---

## 🔌 Flash & Debug

```bash
# Flash binary
st-flash write aecm_voip.bin 0x08000000 

#for my
flash write_image erase stm32h735_aecm_voip.bin 0x08000000

# Debug with GDB #OK
#Через GDB (Для отладки):По умолчанию для GDB-клиента открывается порт 3333. 
arm-none-eabi-gdb stm32h735_aecm_voip.elf
🐛
Type "apropos word" to search for commands related to "word"...
Reading symbols from stm32h735_aecm_voip.elf...
(No debugging symbols found in stm32h735_aecm_voip.elf)
(gdb) target remote :3333
Remote debugging using :3333
0x0800107c in WWDG_IRQHandler ()
(gdb) load
Loading section .isr_vector, size 0x2cc lma 0x8000000
Loading section .text, size 0xe744 lma 0x8000300
Loading section .init, size 0x4 lma 0x800ea44
Loading section .fini, size 0x4 lma 0x800ea48
Loading section .ARM.exidx, size 0x8 lma 0x800ea4c
Loading section .data, size 0x884 lma 0x800ea54
Start address 0x08000300, load size 62116
Transfer rate: 35 KB/sec, 6901 bytes/write.
(gdb)

(gdb) target remote :3333
(gdb) load
(gdb) continue
🐛``

---

## 🐛 Troubleshooting

### ERLE too low (< 3 dB)
- Check microphone gain (`wm8994_set_input_gain`)
- Verify echo delay matches actual system delay
- Check jitter buffer underruns in ITM output

### Jitter buffer underruns
- Increase `JITTER_BUFFER_SIZE` in `aecm_config.h`
- Check network latency with `dwt_measure_rtp_delay()`
- Verify RTP timestamps are monotonically increasing

### No ITM output
- Enable SWO in debugger settings
- Set SWO clock to 8 MHz
- Verify ST-Link V3 firmware is up-to-date

---

## 📝 File Structure

```
stm32h735_aecm_voip/
├── CMakeLists.txt
├── STM32H735IGKx_FLASH.ld          (Linker script)
├── .project / .cproject             (STM32CubeIDE metadata)
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── aecm_config.h
│   │   ├── aecm_processor.h
│   │   ├── voip_codec.h
│   │   ├── voip_rtp.h
│   │   ├── jitter_buffer.h
│   │   ├── agc_preprocessor.h
│   │   ├── dwt_delay_measurement.h
│   │   ├── itm_telemetry.h
│   │   └── wm8994_codec.h
│   └── Src/
│       ├── main.c
│       ├── aecm_processor.c
│       ├── voip_codec_g711.c
│       ├── voip_rtp_handler.c
│       ├── jitter_buffer.c
│       ├── agc_preprocessor.c
│       ├── dwt_delay_measurement.c
│       ├── itm_telemetry.c
│       └── wm8994_codec.c
├── Middlewares/
│   ├── NetX_DUO_6.4.0/
│   ├── WebRTC/aecm/
│   └── Azure_RTOS/
└── README.md
```

---

## 📡 Network Configuration

### NetX DUO Initialization

In `main.c`:

```c
/* Static IP: 192.168.1.100 */
nx_ip_create(&ip_0, "NetX IP", NETX_IP_ADDRESS, NETX_NETMASK, ...);
nx_ip_interface_attach(&ip_0, "Ethernet", NETX_IP_ADDRESS, NETX_NETMASK, ...);

/* UDP socket for RTP @ port 16384 */
nx_udp_socket_create(&ip_0, &udp_socket, "RTP Socket", NX_IP_RAW, NX_FRAGMENT_OKAY, 255, 16384);
```

---

## 🎓 References

- **WebRTC AECM**: https://webrtc.org/ → echo_control_mobile.h
- **G.711 Codec**: ITU-T G.711 (1988) - Pulse Code Modulation (PCM) A-Law
- **RTP Protocol**: RFC 3550 - RTP: A Transport Protocol
- **NetX DUO**: Azure RTOS 6.4.0 API Reference
- **STM32H735**: STM32H735xx Reference Manual (RM0468)

---

## 📜 License

MIT License - Feel free to modify and redistribute.

---

## ✅ Tested On

- **2x STM32H735G-DK** (Discovery kits)
- **STM32CubeIDE 2.0.0**
- **Windows 11 / Linux**
- **ST-Link V3** debugger
- **ARM Cortex-M7** @ 520 MHz

---

**Happy VoIPing! 🎤🎧**
