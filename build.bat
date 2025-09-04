@echo off
setlocal

REM Determine action
if "%1"=="" goto build
if /i "%1"=="clean" goto clean
goto build

:clean
echo Cleaning old build...
rmdir /s /q build

echo Configuring and building...
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
cmake --build build
goto end

:build
echo Building project...
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
cmake --build build
goto end

:end
pause
