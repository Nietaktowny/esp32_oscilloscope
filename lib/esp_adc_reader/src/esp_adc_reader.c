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

void adc_reader_alloc_res (void) {
    adc_continuous_handle_cfg_t handle_cfg = {
        .max_store_buf_size = 1024,
        .conv_frame_size = 100,
    };
}

/**
 * @brief Starting point of esp_adc_reader library.
 * @note This function should be called before any other
 * from this library, as this is where all the necessary things are
 * prepared.
 */
void adc_reader_init(void) {
    adc_reader_alloc_res();
} 

#endif