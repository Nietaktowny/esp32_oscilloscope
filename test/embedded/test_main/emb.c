#include "unity.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp32_oscilloscope.h"
#include "mock_lcd_controller.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_function_should_return_if_init_lcd_return_err(void) {
  //given
  esp_err_t err;
  init_lcd_ExpectAndReturn(259);
  //when
  err = prepare_lcd();

  UnityAssertEqualNumber(259, err, "PREPARE LCD RETURN ESP OK IF ALL OK", __LINE__, UNITY_DISPLAY_STYLE_INT16);
}

void test_function_should_return_esp_ok_if_init_lcd_returns_esp_ok(void) {
  //given
  esp_err_t err;
  //when
  err = prepare_lcd();

  UnityAssertEqualNumber(ESP_OK, err, "PREPARE LCD RETURN ESP OK IF ALL OK", __LINE__, UNITY_DISPLAY_STYLE_INT16);
}

int runUnityTests(void) {
  UNITY_BEGIN();
  //RUN_TEST(test_function_should_return_esp_ok_if_init_lcd_returns_esp_ok);
  //RUN_TEST(test_function_should_return_if_init_lcd_return_err);
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