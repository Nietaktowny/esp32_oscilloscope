#!/bin/bash
echo ============================================================= Generating CMock files =============================================================

lcd_controller_source="cmock/lib/mocks/Mocklcd_controller.h"
lcd_controller_path="lib/lcd_controller"

echo Starting generating...
cd cmock/lib
ruby cmock.rb lib/lcd_controller/include/lcd_controller.h
cd ../..
cp -v cmock/lib/mocks/Mocklcd_controller.h lib/lcd_controller/include
cp -v cmock/lib/mocks/Mocklcd_controller.c lib/lcd_controller/src
echo files stored in library: $lcd_controller_path

echo ============================================================= CMock files created =============================================================