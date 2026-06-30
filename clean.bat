@echo off
REM Clean build artifacts

echo [CLEAN] Removing build directories...
if exist build rmdir /s /q build
if exist build-debug rmdir /s /q build-debug
echo [DONE] Clean complete
#pause
