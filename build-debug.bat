@echo off
REM Debug build script

if not exist "build-debug" mkdir build-debug
cd build-debug
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
make -j4
cd ..
pause
