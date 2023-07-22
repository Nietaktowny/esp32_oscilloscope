#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_wifi_manager.h"

int check_wifi(int);

void app_main(void)
{
    printf("Hello world!\n");
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        check_wifi(wifi_manager_connect(3));
    }
    
}


int check_wifi(int i) {
    int retval = 0;
    if (i > 2)
    {
        retval = 1;
    }
    return retval;
}
