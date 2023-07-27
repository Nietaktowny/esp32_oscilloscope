#include "unity.h"
#include "CException.h"
#include "esp_err.h"
#include "unity.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_function_should_check_if_cexception_is_included_and_perform_basic_test (void) {
  volatile int i = 0;
  volatile CEXCEPTION_T e = 0x5a;

  Try
  {
    i += 1;
  }
  Catch(e)
  {
    TEST_FAIL_MESSAGE("Should Not Enter Catch If Not Thrown");
  }

  //verify that e was untouched
  TEST_ASSERT_EQUAL(0x5a, e);

  // verify that i was incremented once
  TEST_ASSERT_EQUAL(1, i);
}

void test_function_should_check_if_cexceptions_work_with_esp_err_type (void) {
  //given
  esp_err_t esp_err = ESP_FAIL;
  volatile int i = 0;

  //when
  Try 
  {
    i += 1;
    Throw(esp_err);
  }
  Catch (esp_err) {
    i += 1;
  }

  //then

  TEST_ASSERT_EQUAL_INT(2, i);
  TEST_ASSERT_EQUAL_INT(-1, esp_err);
}

void test_function_should_check_if_cexceptions_work_with_esp_err_type_and_dont_catch (void) {
  //given
  esp_err_t esp_err = 123;
  volatile int i = 0;

  //when
  Try 
  {
    i += 1;
  }
  Catch (esp_err) {
    i += 1;
    TEST_FAIL_MESSAGE("Should Not Enter Catch If Not Thrown");
  }

  //then

  TEST_ASSERT_EQUAL_INT(1, i);
  TEST_ASSERT_EQUAL_INT(123, esp_err);
} 

void test_function_should_check_if_cexceptions_catch_in_try_block(void) {
  //given
  esp_err_t esp_err = 0;
  volatile int i = 0;

  //when
  Try 
  {
    i += 1;
    esp_err = 123;
  }
  Catch (esp_err) {
    i += 1;
    TEST_FAIL_MESSAGE("Should Not Enter Catch If Not Thrown");
  }

  //then

  TEST_ASSERT_EQUAL_INT(1, i);
  TEST_ASSERT_EQUAL_INT(123, esp_err);
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_check_if_cexceptions_work_with_esp_err_type);
  RUN_TEST(test_function_should_check_if_cexceptions_work_with_esp_err_type_and_dont_catch);
  RUN_TEST(test_function_should_check_if_cexception_is_included_and_perform_basic_test);
  RUN_TEST(test_function_should_check_if_cexceptions_catch_in_try_block);
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