#!/bin/bash
echo ============================================================= Generating CMock files =============================================================

lcd_controller_source="C:/Users/User/cmock/lib/mocks/Mocklcd_controller.h"
lcd_controller_path="C:/Users/User/Documents/PlatformIO/Projects/esp32_oscilloscope/lib/lcd_controller"

echo Starting generating...
cd C:/Users/User/cmock/lib
ruby cmock.rb C:/Users/User/Documents/PlatformIO/Projects/esp32_oscilloscope/lib/lcd_controller/include/lcd_controller.h
cp /s/z/v/y C:/Users/User/cmock/lib/mocks/Mocklcd_controller.h C:/Users/User/Documents/PlatformIO/Projects/esp32_oscilloscope/lib/lcd_controller/include
cp /s/z/v/y C:/Users/User/cmock/lib/mocks/Mocklcd_controller.c C:/Users/User/Documents/PlatformIO/Projects/esp32_oscilloscope/lib/lcd_controller/src
echo files stored in library: $lcd_controller_path

echo ============================================================= CMock files created =============================================================