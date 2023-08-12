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

static const char *GUI = "gui task";


void gui_task (void* args) {
    lv_init();

	hal_setup();

    gui_init();

	hal_loop();
}


void app_main(void)
{
    ESP_LOGI(GUI, "Starting gui task...");
    xTaskCreatePinnedToCore(gui_task, "gui_task", 4096, NULL, 1, NULL, 1);
}

/*Main function used when running native sdl emulator.*/
#elif (WIN32 || __linux__) & (USE_SDL)
int main(void)
{
	lv_init();

	hal_setup();

    gui_init();

	hal_loop();
}
#endif



