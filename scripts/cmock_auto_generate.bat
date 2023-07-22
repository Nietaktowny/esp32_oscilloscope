@echo off
echo ============================================================= Generating CMock files =============================================================

set "lcd_controller_source=cmock\lib\mocks\Mocklcd_controller.h"
set "lcd_controller_path=lib\lcd_controller"

title Generate CMock files
echo Starting generating...
cd
dir
cd cmock\lib
cd
ruby cmock.rb ..\..\lib\lcd_controller\include\lcd_controller.h
cd ..\..
cd
xcopy /s/z/v/y cmock\lib\mocks\Mocklcd_controller.h lib\lcd_controller\include
xcopy /s/z/v/y cmock\lib\mocks\Mocklcd_controller.c lib\lcd_controller\src
echo files stored in library: %lcd_controller_path%

echo ============================================================= CMock files created =============================================================
