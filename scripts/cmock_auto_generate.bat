@echo off
echo ============================================================= Generating CMock files =============================================================

set "lcd_controller_source=cmock\lib\mocks\mock_lcd_controller.h"
set "mocks_path=test\mocks"

title Generate CMock files
echo Starting generating...
cd
cd cmock\lib
cd
ruby cmock.rb -o../../scripts/CMockConfig.yml ../../test/mocks/lcd_controller.h
ruby cmock.rb -o../../scripts/CMockConfig.yml ../../test/mocks/spi_driver_mocks.h
cd ..\..
cd
echo files stored in library: %mocks_path%

echo ============================================================= CMock files created =============================================================
