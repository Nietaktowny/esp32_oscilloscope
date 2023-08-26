/**
 * @file esp_adc_reader.h
 * @author your name (you@domain.com)
 * @brief Header file of esp_adc_reader library.
 * @version 1.0
 * @date 2023-08-26
 * 
 * Here are all #defines placed, as well as global functions.
 * To avoid errors in native tests ESP-IDF specific code is protected by #ifdef ESP_PLATFORM
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_adc/adc_continuous.h"


#define ADC_READER_UNIT                    ADC_UNIT_1                   ///< SAR ADC 1.
#define _ADC_READER_UNIT_STR(unit)         #unit
#define ADC_READER_UNIT_STR(unit)          _ADC_READER_UNIT_STR(unit)
#define ADC_READER_CONV_MODE               ADC_CONV_SINGLE_UNIT_1
#define ADC_READER_ATTEN                   ADC_ATTEN_DB_0
#define ADC_READER_BIT_WIDTH               SOC_ADC_DIGI_MAX_BITWIDTH    ///< Resolution: 12 bit.
#define ADC_READER_READ_LEN                256

#define ADC_READER_OUTPUT_TYPE             ADC_DIGI_OUTPUT_FORMAT_TYPE1
#define ADC_READER_GET_CHANNEL(p_data)     ((p_data)->type1.channel)
#define ADC_READER_GET_DATA(p_data)        ((p_data)->type1.data)
#define ADC_READER_RES_VALUE               0.8058f                      ///< Resolution for 12bit - 3300/4096.

/**
 * @brief Enter point of esp_adc_reader library.
 * @note This function should be called before adc_reader_loop() so all the resources are allocated.
 * 
 * @param queue Queque handle to which ADC values will be written to.
 */
void adc_reader_init(QueueHandle_t queue);

/**
 * @brief Loop where readings are done.
 * This loop reads ADC in continous mode and writes data into Queue.
 * 
 */
void adc_reader_loop (void);

/**
 * @brief Calculate raw ADC data into voltage in mV.
 * 
 * @param data Raw voltage data.
 * @return int Voltage in mV.
 */
int adc_calc_volt(int16_t data);


void adc_reader_deinit(void);


adc_channel_t adc_reader_get_channel(void);

QueueHandle_t adc_reader_get_queue_handle(void);

adc_continuous_handle_t adc_reader_get_adc_handle(void);

#endif