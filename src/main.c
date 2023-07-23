#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "lcd_controller.h"

int check_wifi(int);

void app_main(void)
{
    printf("Hello world!\n");
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}
