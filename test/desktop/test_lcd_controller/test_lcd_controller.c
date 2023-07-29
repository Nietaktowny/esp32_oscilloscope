#include "unity.h"

#include "../../mocks/mock_spi_driver_mocks.h"

#include "../../mocks/lcd_controller.h"


void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
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

void test_function_should_check_if_ERR_SPI_ALREADY_INIT_exists(void) {
  //given
  const int err_spi_init_value = 0x103;

  //then
  UnityAssertEqualNumber(err_spi_init_value, ERR_SPI_ALREADY_INIT, "ERR_SPI_ALREADY_INIT - DOESN'T EXIST", __LINE__, UNITY_DISPLAY_STYLE_INT32);
}

void test_macro_should_not_execute_expression_if_no_error (void) {
  //given
  esp_err_t err = 0;
  int i = 0;

  //then
  ERR_CHECK(err, i++);

  //then
  UnityAssertEqualNumber(0, i, "ERR_CHECK executed expression but no error", __LINE__, UNITY_DISPLAY_STYLE_INT32);
}

void test_macro_should_execute_expression_if_some_error_occurs (void) {
  //given
  esp_err_t err = -1;
  int i = 0;

  //then
  ERR_CHECK(err, i++);

  //then
  UnityAssertEqualNumber(1, i, "ERR_CHECK not executed expression despite error", __LINE__, UNITY_DISPLAY_STYLE_INT32);
}

void test_function_should_check_if_init_spi_bus_returns_err_if_spi_bus_initialize_returns_err(void) {
  //given
  esp_err_t err = ESP_OK;
  spi_bus_initialize_CMockIgnoreAndReturn(__LINE__, ESP_ERR_INVALID_ARG);

  //when
  err = init_spi_bus();

  //then
  UnityAssertEqualNumber(ESP_ERR_INVALID_ARG, err, "init_spi_bus not returned error when should", __LINE__, UNITY_DISPLAY_STYLE_INT32);

}

/*To add test use: RUN_TEST(test_name) macro.*/
int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_check_gpio_defines);
  RUN_TEST(test_function_should_check_if_ERR_SPI_ALREADY_INIT_exists);
  RUN_TEST(test_macro_should_not_execute_expression_if_no_error);
  RUN_TEST(test_macro_should_execute_expression_if_some_error_occurs);
  RUN_TEST(test_function_should_check_if_init_spi_bus_returns_err_if_spi_bus_initialize_returns_err);
  return UNITY_END();
}


/**
  * main function for native dev-platform
  */
int main(void) {
  runUnityTests();
}