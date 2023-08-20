/**
 * @file esp_adc_reader.c
 * @author Wojciech Mytych
 * @brief Main source file of esp_adc_reader library.
 * @version 0.1
 * @date 2023-08-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifdef ESP_PLATFORM
#include "esp_adc/adc_continuous.h"
#include "esp_err.h"
#include "esp_adc_reader.h"
#include "esp_log.h"

/**********************************************************STATIC FUNCTIONS PROTOTYPES***********************************************************************************/
static void continuous_adc_init(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle);


/**************************************************************************************************************************************************************************/


static const char *TAG = "EXAMPLE";
static adc_channel_t channel[2] = {ADC_CHANNEL_6, ADC_CHANNEL_7};


static void continuous_adc_init(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle)
{
    adc_continuous_handle_t handle = NULL;

    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 1024,
        .conv_frame_size = ADC_READER_READ_LEN,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle));

    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = 20 * 1000,
        .conv_mode = ADC_READER_CONV_MODE,
        .format = ADC_READER_OUTPUT_TYPE,
    };

    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
    dig_cfg.pattern_num = channel_num;
    for (int i = 0; i < channel_num; i++) {
        adc_pattern[i].atten = ADC_READER_ATTEN;
        adc_pattern[i].channel = channel[i] & 0x7;
        adc_pattern[i].unit = ADC_READER_UNIT;
        adc_pattern[i].bit_width = ADC_READER_BIT_WIDTH;

        ESP_LOGI(TAG, "adc_pattern[%d].atten is :%"PRIx8, i, adc_pattern[i].atten);
        ESP_LOGI(TAG, "adc_pattern[%d].channel is :%"PRIx8, i, adc_pattern[i].channel);
        ESP_LOGI(TAG, "adc_pattern[%d].unit is :%"PRIx8, i, adc_pattern[i].unit);
    }
    dig_cfg.adc_pattern = adc_pattern;
    ESP_ERROR_CHECK(adc_continuous_config(handle, &dig_cfg));

    *out_handle = handle;
}

/**
 * @brief Starting point of esp_adc_reader library.
 * @note This function should be called before any other
 * from this library, as this is where all the necessary things are
 * prepared.
 */
void adc_reader_init(void) {
    adc_continuous_handle_t handle = NULL;
    continuous_adc_init(channel, sizeof(channel) / sizeof(adc_channel_t), &handle);
} 

#endif