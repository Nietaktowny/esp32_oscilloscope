/**
 * @file esp_adc_reader.c
 * @author Wojciech Mytych
 * @brief Main source file of esp_adc_reader library.
 * @version 1.0
 * @date 2023-08-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifdef ESP_PLATFORM
#include "esp_err.h"
#include "esp_adc_reader.h"
#include "esp_log.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include <string.h>
#include <stdio.h>

/**********************************************************STATIC FUNCTIONS PROTOTYPES***********************************************************************************/
static void continuous_adc_init(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle);


/**************************************************************************************************************************************************************************/

static TaskHandle_t s_task_handle = NULL;
static const char *TAG = "ADC_READER";
static adc_channel_t channel[2] = {ADC_CHANNEL_6, ADC_CHANNEL_7};

static QueueHandle_t queue_handle;


static adc_continuous_handle_t handle = NULL;


adc_channel_t adc_reader_get_channel(void) {
    return channel[0];
}

QueueHandle_t adc_reader_get_queue_handle(void) {
    return queue_handle;
}

adc_continuous_handle_t adc_reader_get_adc_handle(void) {
    return handle;
}

static bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
    BaseType_t mustYield = pdFALSE;
    //Notify that ADC continuous driver has done enough number of conversions
    if(s_task_handle != NULL) {
        vTaskNotifyGiveFromISR(s_task_handle, &mustYield);
    } else {
        ESP_LOGW(TAG, "s_task_handle is NULL, obtaining current task handle...");
        s_task_handle = xTaskGetCurrentTaskHandle();
        vTaskNotifyGiveFromISR(s_task_handle, &mustYield);
    }
    

    return (mustYield == pdTRUE);
}

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


int adc_calc_volt(int16_t data) {
    return (data*ADC_READER_RES_VALUE);
}

/**
 * @brief Starting point of esp_adc_reader library.
 * @note This function should be called before any other
 * from this library, as this is where all the necessary things are
 * prepared.
 */
void adc_reader_init(QueueHandle_t queue) {

    s_task_handle = xTaskGetCurrentTaskHandle();
    ESP_LOGI(TAG, "Current task name: %s", pcTaskGetName(s_task_handle));
    queue_handle = queue;
    int channel_io;

    continuous_adc_init(channel, sizeof(channel) / sizeof(adc_channel_t), &handle);
    adc_continuous_channel_to_io(ADC_READER_UNIT, ADC_CHANNEL_6, &channel_io);
    ESP_LOGI(TAG, "Reading values on GPIO: %d", channel_io);

    adc_continuous_evt_cbs_t cbs = {
        .on_conv_done = s_conv_done_cb,
    };
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(handle, &cbs, NULL));
    ESP_ERROR_CHECK(adc_continuous_start(handle));
}

void adc_reader_deinit(void) {
    if (handle != NULL)
    {
        ESP_ERROR_CHECK(adc_continuous_stop(handle));
        ESP_ERROR_CHECK(adc_continuous_deinit(handle));
    }
    
    vQueueDelete(queue_handle);
    memset(channel, 0, sizeof channel);
}

void adc_reader_loop (void) {
    esp_err_t ret;
    uint32_t ret_num = 0;
    uint8_t result[ADC_READER_READ_LEN] = {0};
    memset(result, 0xcc, ADC_READER_READ_LEN);

    while(1) {

        /**
         * This is to show you the way to use the ADC continuous mode driver event callback.
         * This `ulTaskNotifyTake` will block when the data processing in the task is fast.
         * However in this example, the data processing (print) is slow, so you barely block here.
         *
         * Without using this event callback (to notify this task), you can still just call
         * `adc_continuous_read()` here in a loop, with/without a certain block timeout.
         */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        char unit[] = ADC_READER_UNIT_STR(ADC_READER_UNIT);

        while (1) {
            ret = adc_continuous_read(handle, result, ADC_READER_READ_LEN, &ret_num, 0);
            if (ret == ESP_OK) {
                //ESP_LOGI("TASK", "ret is %x, ret_num is %"PRIu32" bytes", ret, ret_num);
                for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES) {
                    adc_digi_output_data_t *p = (adc_digi_output_data_t*)&result[i];
                    uint32_t chan_num = ADC_READER_GET_CHANNEL(p);
                    int16_t data = ADC_READER_GET_DATA(p);
                    int voltage;
                    /* Check the channel number validation, the data is invalid if the channel num exceed the maximum channel */
                    if (chan_num < SOC_ADC_CHANNEL_NUM(ADC_READER_UNIT)) {
                        //ESP_LOGI(TAG, "Unit: %s, Channel: %"PRIu32", Value: %"PRIx32, unit, chan_num, data);
                        ESP_LOGV(TAG, "Raw Data: %d", data);
                        voltage = adc_calc_volt(data);
                        ESP_LOGD(TAG, "Readed voltage: %d", voltage);
                        xQueueSend(queue_handle, &voltage, pdMS_TO_TICKS(10));
                        vTaskDelay(pdMS_TO_TICKS(5));
                    } else {
                        ESP_LOGW(TAG, "Invalid data %d",  data);
                    }
                }
            } else if (ret == ESP_ERR_TIMEOUT) {
                //We try to read `EXAMPLE_READ_LEN` until API returns timeout, which means there's no available data
                break;
            }
        }
    }
}
#endif