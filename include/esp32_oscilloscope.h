#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "lcd_controller.h"
#include "esp_log.h"

#pragma once

char* TAG = "MAIN";

esp_err_t prepare_lcd (void);