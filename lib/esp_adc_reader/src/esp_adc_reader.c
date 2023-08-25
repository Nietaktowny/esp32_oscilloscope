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
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include <string.h>
#include <stdio.h>
#include "lvgl.h"
#include "lvgl_gui.h"

/**********************************************************STATIC FUNCTIONS PROTOTYPES***********************************************************************************/
static void continuous_adc_init(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle);


/**************************************************************************************************************************************************************************/

static TaskHandle_t s_task_handle;
static const char *TAG = "ADC_READER";
static adc_channel_t channel[2] = {ADC_CHANNEL_6, ADC_CHANNEL_7};

QueueHandle_t queue_handle;


adc_continuous_handle_t handle = NULL;

adc_cali_handle_t cali_handle = NULL;

static bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
    BaseType_t mustYield = pdFALSE;
    //Notify that ADC continuous driver has done enough number of conversions
    vTaskNotifyGiveFromISR(s_task_handle, &mustYield);

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

static esp_err_t adc_cali_init (void) {
    esp_err_t ret = 0;
    adc_cali_scheme_ver_t scheme = ADC_CALI_SCHEME_VER_LINE_FITTING;

    ret = adc_cali_check_scheme(&scheme);
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_0,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ret = adc_cali_create_scheme_line_fitting(&cali_config, &cali_handle);

    return ret;
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
    ESP_ERROR_CHECK(adc_cali_init());
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
                        ESP_LOGI(TAG, "Raw Data: %d", data);
                        voltage = adc_calc_volt(data);
                        ESP_LOGI(TAG, "Readed voltage: %d", voltage);
                        xQueueSend(queue_handle, &voltage, pdMS_TO_TICKS(10));
                        vTaskDelay(pdMS_TO_TICKS(5));
                    } else {
                        ESP_LOGW(TAG, "Invalid data %d",  data);
                    }
                }
                /**
                 * Because printing is slow, so every time you call `ulTaskNotifyTake`, it will immediately return.
                 * To avoid a task watchdog timeout, add a delay here. When you replace the way you process the data,
                 * usually you don't need this delay (as this task will block for a while).
                 */
                vTaskDelay(1);
            } else if (ret == ESP_ERR_TIMEOUT) {
                //We try to read `EXAMPLE_READ_LEN` until API returns timeout, which means there's no available data
                break;
            }
        }
    }
}
#endif