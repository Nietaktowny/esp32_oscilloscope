/**
 * @file esp32_oscilloscope.c
 * @author Wojciech Mytych
 * @brief main function of project.
 * @version 0.1
 * @date 2023-08-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "lvgl.h"
#include "app_hal.h"
#include <stdio.h>
#include <stdint.h>
#include "lvgl_gui.h"



#ifdef ESP_PLATFORM
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_task_wdt.h"

/// This is logging tag used for logs related to lvgl_gui library.
static const char *GUI = "gui task";

/**
 * @brief Task running GUI.
 * 
 * @param args Additional arguments passed to task.
 */
void gui_task (void* args) {
    lv_init();

	hal_setup();

    generate_example_values();

    gui_init();

	hal_loop();
}

/**
 * @brief Entry point of application for ESP-IDF framework.
 * 
 */
void app_main(void)
{
    ESP_LOGI(GUI, "Starting gui task...");
    xTaskCreatePinnedToCore(gui_task, "gui_task", 4096, NULL, 1, NULL, 1);
}


#elif (USE_SDL)
/**
 * @brief Entry point of application for native framework.
 * 
 * @return int 0 on success
 */
int main(void)
{
	lv_init();

	hal_setup();

    generate_example_values();

    gui_init();

	hal_loop();
}
#endif



