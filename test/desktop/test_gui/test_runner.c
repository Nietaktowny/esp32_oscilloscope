#include "tests/tests.h"
#include "lvgl.h"
#include "app_hal.h"

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
  * main function for native dev-platform
  */
int main(void) {
  run_chart_tests();
  run_lvgl_gui_tests();
}