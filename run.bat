
@echo off
title Startup Connect

echo ========================================
echo           STARTUP CONNECT
echo ========================================
echo.

g++ -std=c++17 -Wall -Wextra main.cpp -o StartupConnect.exe

if errorlevel 1 (
    echo.
    echo Compilation failed!
    pause
    exit /b 1
)

echo.
echo Compilation successful!
echo Starting Startup Connect...
echo.

StartupConnect.exe

echo.
pause
