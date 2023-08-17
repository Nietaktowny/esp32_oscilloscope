#include "tests/tests.h"
#include "lvgl.h"
#include "app_hal.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void setUp(void) {
  	lv_init();
	  hal_setup();
    test_chart_setup();
    test_lvgl_gui_setup();
}

void tearDown(void) {
  // clean stuff up here
}

/**
  * For ESP-IDF framework
  */
void app_main(void) {
  run_chart_tests();
  run_lvgl_gui_tests();
  vTaskDelay(200);
  fflush(stdout);
  esp_restart();
}