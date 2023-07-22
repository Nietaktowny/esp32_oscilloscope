#include "unity.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_function_should_doBlahAndBlah(void) {
  UnityAssertEqualNumber(1, 1, "TEST TEST", 12, UNITY_DISPLAY_STYLE_INT16);
}


/*To add test use: RUN_TEST(test_name) macro.*/
int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_doBlahAndBlah);
  return UNITY_END();
}


/**
  * main function for native dev-platform
  */
int main(void) {
  runUnityTests();
}