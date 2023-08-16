/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
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
static const char *GUI = "gui task";


void gui_task (void* args) {
    lv_init();

	hal_setup();

    gui_init();

	hal_loop();
}


void app_main(void)
{
    //generate_example_values();
    
    ESP_LOGI(GUI, "Starting gui task...");
    xTaskCreatePinnedToCore(gui_task, "gui_task", 4096, NULL, 0, NULL, 1);
}


#elif (USE_SDL)
int main(void)
{
	lv_init();

	hal_setup();

    generate_example_values();

    gui_init();

	hal_loop();
}
#endif



