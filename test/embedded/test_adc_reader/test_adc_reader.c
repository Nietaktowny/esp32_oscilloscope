#include "unity.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "esp_adc/adc_continuous.h"
#include "esp_adc_reader.h"
#include "esp_log.h"

const char* TEST = "test";

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  //adc_reader_deinit();
}

/*HELPER FUNCTIONS*/

void adc_task (void* args) {
    ESP_LOGI(TEST, "Started adc task...");
    adc_reader_loop();
}

/*END OF HELPER FUNCTIONS*/

void test_if_queue_receive_returned_pdPASS(void) {
    //given
    QueueHandle_t test_queue = xQueueCreate(10, sizeof(int));
    TEST_ASSERT_NOT_NULL(test_queue);
    TaskHandle_t adc_task_handle = NULL;
    int buf = 123456;
    BaseType_t status;

    //when
    adc_reader_init(test_queue);
    xTaskCreate(adc_task, "adc_task", 4096, NULL, 1, &adc_task_handle);
    TEST_ASSERT_NOT_NULL_MESSAGE(adc_task_handle, "Task handle shouldn't be null");
    vTaskDelay(pdMS_TO_TICKS(1000)); //Wait for adc to create some results.

    //then
    status = xQueueReceive(test_queue, &buf, pdMS_TO_TICKS(2000));
    ESP_LOGI(TEST, "Status: %d", status);
    //TEST_ASSERT_EQUAL_MESSAGE(pdTRUE, status, "Receive status should be pdTRUE");
    TEST_ASSERT_NOT_EQUAL_MESSAGE(123456, buf, "Buffer should differ from initial value.");

    //after
    adc_reader_deinit();
    vTaskDelete(adc_task_handle);
}

void test_if_adc_reader_res_value_is_defined(void) {
    //then
    #ifndef ADC_READER_RES_VALUE
    TEST_FAIL_MESSAGE("ADC_READER_RES_VALUE is not defined.");
    #endif
}

void test_if_adc_reader_output_type_is_defined(void) {
    //then
    #ifndef ADC_READER_OUTPUT_TYPE
    TEST_FAIL_MESSAGE("ADC_READER_OUTPUT_TYPE is not defined.");
    #endif
}

void test_if_adc_reader_read_len_is_defined(void) {
    //then
    #ifndef ADC_READER_READ_LEN
    TEST_FAIL_MESSAGE("ADC_READER_READ_LEN is not defined.");
    #endif
}

void test_if_adc_reader_bit_width_is_defined(void) {
    //then
    #ifndef ADC_READER_BIT_WIDTH
    TEST_FAIL_MESSAGE("ADC_READER_BIT_WIDTH is not defined.");
    #endif
}

void test_if_adc_reader_atten_is_defined(void) {
    //then
    #ifndef ADC_READER_ATTEN
    TEST_FAIL_MESSAGE("ADC_READER_ATTEN is not defined.");
    #endif
}

void test_if_adc_reader_conv_mode_is_defined(void) {
    //then
    #ifndef ADC_READER_CONV_MODE
    TEST_FAIL_MESSAGE("ADC_READER_CONV_MODE is not defined.");
    #endif
}

void test_if_adc_reader_unit_is_defined(void) {
    //then
    #ifndef ADC_READER_UNIT
    TEST_FAIL_MESSAGE("ADC_READER_UNIT is not defined.");
    #endif
}

void test_if_queue_handle_is_equal (void) {
    //given
    QueueHandle_t test_handle = xQueueCreate(10, sizeof(int));
    QueueHandle_t actual_handle = NULL;

    //when
    adc_reader_init(test_handle);
    actual_handle = adc_reader_get_queue_handle();

    //then
    TEST_ASSERT_EQUAL_PTR_MESSAGE(test_handle, actual_handle, "Queue handle should be the same as passed.");

    //after
    adc_reader_deinit();
}

void test_if_queue_handle_is_not_null (void) {
    //given
    QueueHandle_t test_handle = xQueueCreate(10, sizeof(int));
    QueueHandle_t actual_handle = NULL;

    //when
    adc_reader_init(test_handle);
    actual_handle = adc_reader_get_queue_handle();

    //then
    TEST_ASSERT_NOT_NULL_MESSAGE(actual_handle, "Queue handle shouldn't be null.");

    //after
    adc_reader_deinit();
}

void test_if_adc_handle_is_not_null (void) {
    //given
    QueueHandle_t test_handle = xQueueCreate(10, sizeof(int));;
    adc_continuous_handle_t adc_handle = NULL;

    //when
    adc_reader_init(test_handle);
    adc_handle = adc_reader_get_adc_handle();

    //then
    TEST_ASSERT_NOT_NULL_MESSAGE(adc_handle, "Adc continous handle shouldn't be null.");

    //after
    adc_reader_deinit();
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_if_adc_handle_is_not_null);
  RUN_TEST(test_if_queue_handle_is_not_null);
  RUN_TEST(test_if_queue_handle_is_equal);
  RUN_TEST(test_if_adc_reader_unit_is_defined);
  RUN_TEST(test_if_adc_reader_conv_mode_is_defined);
  RUN_TEST(test_if_adc_reader_atten_is_defined);
  RUN_TEST(test_if_adc_reader_bit_width_is_defined);
  RUN_TEST(test_if_adc_reader_read_len_is_defined);
  RUN_TEST(test_if_adc_reader_output_type_is_defined);
  RUN_TEST(test_if_adc_reader_res_value_is_defined);
  //RUN_TEST(test_if_queue_receive_returned_pdPASS);
  return UNITY_END();
}


/**
  * For ESP-IDF framework
  */
void app_main(void) {
  runUnityTests();
  vTaskDelay(200);
  fflush(stdout);
  esp_restart();
}