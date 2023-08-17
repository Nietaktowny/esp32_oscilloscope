# Libraries overview

## Use of lib folder

The lib folder contains all private components of current project.

I will try to describe each component in it's own docs folder. There i will only provide list of used components, and main usage of each.

  

## List of components

Currently this is overview of used components:

- **CMock** - Althrough CMock is part of Espressif platform this was added here, so CMock was also available in native platform. It's only library that doesn't keep the conventions of PlatformIO libs folders usage.
For more information refer to: [CMock README](https://github.com/ThrowTheSwitch/CMock/blob/master/docs/CMock_Summary.md)

- **lvgl_gui** - It's main usage is all interactions LCD screen built-in on ESP Wrover Kit using LVGL library.
The unit tests are stored in `test\desktop\test_gui` folder for native platform.

- **CException** - It's exception handling framework build in ANSI C used for exception handling. The unit tests are stored in `test\desktop\test_cexception` folder.
For more information refer to: [CException README](https://github.com/ThrowTheSwitch/CException/blob/master/README.md)
- **esp_lcd_ili9341** - It's Espressif component from Espressif component registry used to implement interactions with ILI9341 LCD controller. Used in `hal\esp32` to initialize LCD.
For more information refer to: [IDF Component registry page](https://components.espressif.com/components/espressif/esp_lcd_ili9341)
- **lvgl** -  Light and Versatile Graphics Library used to made GUI for LVD.
For more information refer to: [LVGL Github repo](https://github.com/lvgl/lvgl)
- **lv_drivers** - Display controller and touchpad driver to can be directly used with LVGL. Used in `hal\sdl` to initialize emulator.
For more information refer to: [lv_drivers Github repo](https://github.com/lvgl/lv_drivers)
