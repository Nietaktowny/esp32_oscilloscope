@echo off
echo ============================================================= Generating CMock files =============================================================

set "lcd_controller_source=C:\Users\User\cmock\lib\mocks\Mocklcd_controller.h"
set "lcd_controller_path=C:\Users\User\Documents\PlatformIO\Projects\esp32_oscilloscope\lib\lcd_controller"

title Generate CMock files
echo Starting generating...
cd C:\Users\User\cmock\lib
ruby cmock.rb C:\Users\User\Documents\PlatformIO\Projects\esp32_oscilloscope\lib\lcd_controller\include\lcd_controller.h
xcopy /s/z/v/y C:\Users\User\cmock\lib\mocks\Mocklcd_controller.h C:\Users\User\Documents\PlatformIO\Projects\esp32_oscilloscope\lib\lcd_controller\include
xcopy /s/z/v/y C:\Users\User\cmock\lib\mocks\Mocklcd_controller.c C:\Users\User\Documents\PlatformIO\Projects\esp32_oscilloscope\lib\lcd_controller\src
echo files stored in library: %lcd_controller_path%

echo ============================================================= CMock files created =============================================================
