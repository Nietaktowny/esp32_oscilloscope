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

#include "esp_adc_reader.h"
#include "freertos/queue.h"

/// This is logging tag used for logs related to lvgl_gui library.
static const char *GUI = "gui task";

QueueHandle_t read_values;
TaskHandle_t adc_task_handle;
TaskHandle_t gui_task_handle;

/**
 * @brief Task running GUI.
 * 
 * @param args Additional arguments passed to task.
 */
void gui_task (void* args) {
    uint32_t buf = 0;
    BaseType_t xStatus;
    while (1)
    {

        xStatus = xQueueReceive(read_values, &buf, pdMS_TO_TICKS(0));
        
        if(xStatus == pdTRUE) {
            gui_set_point((int16_t)buf);
        }
        hal_loop();
    }
    
}


void adc_task (void* args) {
    adc_reader_init(read_values);
    adc_reader_loop();
}

/**
 * @brief Entry point of application for ESP-IDF framework.
 * 
 */
void app_main(void)
{
    lv_init();

	hal_setup();

    read_values = xQueueCreate(10, sizeof(uint32_t));

    gui_init();

    ESP_LOGI(GUI, "Starting gui task...");
    xTaskCreatePinnedToCore(gui_task, "gui_task", 4096, NULL, 1, &gui_task_handle, 1);
    ESP_LOGI(GUI, "Starting adc task...");
    xTaskCreate(adc_task, "adc_task", 4096, NULL, 1, &adc_task_handle);
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

    generate_example_sin_wave();

	hal_loop();
}
#endif


