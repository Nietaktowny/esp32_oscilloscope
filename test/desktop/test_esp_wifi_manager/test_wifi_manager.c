#include "unity.h"
#include "esp_wifi_manager.h"
#include "Mockesp_wifi_manager.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_function_should_connect_to_wifi(void) {
  //given
  wifi_manager_connect_ExpectAndReturn(1, 2);

  //when
  int retval = wifi_manager_connect(1);

  //then
  UnityAssertEqualNumber(2, retval, "TEST LIB", 18, UNITY_DISPLAY_STYLE_INT16);
}


/*To add test use: RUN_TEST(test_name) macro.*/
int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_connect_to_wifi);
  return UNITY_END();
}


/**
  * main function for native dev-platform
  */
int main(void) {
  runUnityTests();
}