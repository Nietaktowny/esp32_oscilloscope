/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include "app_hal.h"
#include "lvgl.h"
#include "lvgl_gui.h"
#include <stdint.h>
#include <stdio.h>

#ifdef ESP_PLATFORM
#include "esp_err.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
static const char *GUI = "gui task";

void gui_task(void *args) {
  lv_init();

  hal_setup();

  generate_example_values();

  gui_init();

  hal_loop();
}

void app_main(void) {
  ESP_LOGI(GUI, "Starting gui task...");
  xTaskCreatePinnedToCore(gui_task, "gui_task", 4096, NULL, 1, NULL, 1);
}

#elif (USE_SDL)
int main(void) {
  lv_init();

  hal_setup();

  generate_example_values();

  gui_init();

  hal_loop();
}
#endif
