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
  init_lcd_Expect(1);

  //when

  //then
  UnityAssertEqualNumber(1, 1, "TEST LIB", 18, UNITY_DISPLAY_STYLE_INT16);
}


void test_function_should_check_gpio_defines(void) {
  //given
  const int RST = 18;
  const int SCL = 19;
  const int DC = 21;
  const int CS = 22;
  const int SDA = 23;
  const int SDO = 25;
  const int BACKLIGHT = 5;
  //when

  //then
  UNITY_TEST_ASSERT_EQUAL_INT(RST, PIN_NUM_RST, __LINE__, "Wrong reset GPIO");
  UNITY_TEST_ASSERT_EQUAL_INT(SCL, PIN_NUM_CLK, __LINE__, "Wrong clock GPIO");
  UNITY_TEST_ASSERT_EQUAL_INT(DC, PIN_NUM_DC, __LINE__, "Wrong dc GPIO");
  UNITY_TEST_ASSERT_EQUAL_INT(CS, PIN_NUM_CS, __LINE__, "Wrong chip select GPIO");
  UNITY_TEST_ASSERT_EQUAL_INT(SDA, PIN_NUM_MOSI, __LINE__, "Wrong mosi GPIO");
  UNITY_TEST_ASSERT_EQUAL_INT(SDO, PIN_NUM_MISO, __LINE__, "Wrong miso GPIO");
  UNITY_TEST_ASSERT_EQUAL_INT(BACKLIGHT, PIN_NUM_BCKL, __LINE__, "Wrong backlight GPIO");
}


/*To add test use: RUN_TEST(test_name) macro.*/
int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_init_lcd);
  RUN_TEST(test_function_should_check_gpio_defines);
  return UNITY_END();
}


/**
  * main function for native dev-platform
  */
int main(void) {
  runUnityTests();
}