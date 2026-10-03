@echo off

:: !=================================!
::  Required: CMake and MinGW in PATH
:: !=================================!

if not exist "build/" (
    mkdir build
)

cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
cd ..
cd Debug-Release
./main.exe