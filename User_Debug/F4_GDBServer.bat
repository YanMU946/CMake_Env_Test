@REM  reference: https://bbs.robomaster.com/article/1638268?source=1
@echo off
chcp 65001 > nul
title STM32F4 DAPLink GDB Server Launch executable

REM !!!添加OpenOCD根目录!!!
set "OPENOCD_ROOT=D:\OpenOCD-20260302-0.12.0"

REM link executable, debugger and chip files
set "OPENOCD_EXE=%OPENOCD_ROOT%\bin\openocd.exe"
set "DAPLINK_CFG=%OPENOCD_ROOT%\share\openocd\scripts\interface\cmsis-dap.cfg"
set "TARGET_CFG=%OPENOCD_ROOT%\share\openocd\scripts\target\stm32f4x.cfg"

if not exist "%OPENOCD_EXE%" (
    echo [ERROR] cannot find OpenOCD: %OPENOCD_EXE%
    echo please check your OPENOCD_ROOT
    pause
    exit /b 1
)

echo ----------------------------------------
echo Starting OpenOCD GDB Server...
echo ----------------------------------------
echo Connect Info:
echo - GDB plot: 3333
echo - Telnet plot: 4444
echo - TCL plot: 6666
echo ----------------------------------------
echo [NOTE] Please dont close terminal
echo ----------------------------------------

start "OpenOCD GDB Server" cmd /k ""%OPENOCD_EXE%" -f "%DAPLINK_CFG%" -f "%TARGET_CFG%""

timeout /t %WAIT_TIME% /nobreak > nul

echo OpenOCD GDB Server launch successfully!
echo Now DAPLink is capable by localhost:3333
echo.
timeout /t 2 > nul
exit /b 0
