#!/bin/bash
echo ============================================================= Generating CMock files =============================================================

lcd_controller_source="cmock/lib/mocks/mock_lcd_controller.h"
mocks_path="test/mocks"

echo Starting generating...
pwd
cd cmock/lib
pwd
ruby cmock.rb -o../../scripts/CMockConfig.yml ../../test/mocks/lcd_controller.h
ruby cmock.rb -o../../scripts/CMockConfig.yml ../../test/mocks/spi_driver_mocks.h
cd ../..
pwd
echo files stored in library: $mocks_path

echo ============================================================= CMock files created =============================================================