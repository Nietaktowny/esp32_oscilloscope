#include "unity.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "esp32/app_hal.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_if_esp_hal_is_defined (void) {
  
}

int runUnityTests(void) {
  UNITY_BEGIN();
  //RUN_TEST(test_if_esp_hal_is_defined);
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