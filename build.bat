@echo off
setlocal

:: Adjust this if MinGW is installed somewhere else
set PATH=C:\mingw32\bin;%PATH%

:: Clean old build
if exist build rmdir /s /q build

:: Configure with MinGW
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++

:: Build
cmake --build build

endlocal
pause
