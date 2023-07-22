#include "unity.h"
#include "Mocklcd_controller.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_function_should_init_lcd(void) {
  //given
  init_lcd_ExpectAndReturn(1, 2);

  //when
  int retval = init_lcd(1);

  //then
  UnityAssertEqualNumber(1, retval, "TEST LIB", 18, UNITY_DISPLAY_STYLE_INT16);
}


/*To add test use: RUN_TEST(test_name) macro.*/
int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_init_lcd);
  return UNITY_END();
}


/**
  * main function for native dev-platform
  */
int main(void) {
  runUnityTests();
}