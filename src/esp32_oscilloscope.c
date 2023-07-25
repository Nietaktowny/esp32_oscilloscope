#include "esp32_oscilloscope.h"

void app_main(void)
{
    prepare_lcd();
}


esp_err_t prepare_lcd (void) {
    esp_err_t err = ESP_OK;
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error in main: %s", esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}
