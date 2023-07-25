#include "unity.h"
#include "esp_system.h"
#include "lcd_controller.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_function_should_test_size_of_lcd_init_cmd_t (void) {
  //given
  int size = sizeof(lcd_init_cmd_t);
  int struct_size = 18;

  //then
  UnityAssertEqualNumber(struct_size, size, "WRONG INIT STRUCT SIZE", __LINE__, UNITY_DISPLAY_STYLE_INT32);
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

void test_function_should_check_that_spi_bus_is_already_init (void) {
  //given
  spi_bus_config_t buscfg={
    .miso_io_num=PIN_NUM_MISO,
    .mosi_io_num=PIN_NUM_MOSI,
    .sclk_io_num=PIN_NUM_CLK,
    .quadwp_io_num=-1,
    .quadhd_io_num=-1,
    .max_transfer_sz=PARALLEL_LINES*320*2+8
  };

  //when

  esp_err_t err = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);


  //then
  UnityAssertEqualNumber(259, err, "SPI BUS ALREADY INIT", __LINE__, UNITY_DISPLAY_STYLE_INT32);
}

void test_function_should_add_spi_device (void) {
  spi_device_handle_t spi;
  esp_err_t err;

  spi_device_interface_config_t devcfg = {
      .clock_speed_hz=10*1000*1000,
      .mode = 0,
      .spics_io_num = PIN_NUM_CS,
      .queue_size = 12,
      .pre_cb = lcd_spi_pre_transfer_callback,
  };

  err = spi_bus_add_device(
      SPI2_HOST,
      &devcfg,
      &spi);

  UnityAssertEqualNumber(ESP_OK, err, "ADD SPI DEVICE", __LINE__, UNITY_DISPLAY_STYLE_INT32);
}

void test_function_should_check_if_init_lcd_returns_ESP_OK(void) {
  //given
  esp_err_t err;
  //when
  err = init_lcd();
  //then
  UnityAssertEqualNumber(0, err, "INIT LCD RETURNS OK", __LINE__, UNITY_DISPLAY_STYLE_INT16);
}


/*To add test use: RUN_TEST(test_name) macro.*/
int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_check_if_init_lcd_returns_ESP_OK);
  RUN_TEST(test_function_should_check_that_spi_bus_is_already_init);
  RUN_TEST(test_function_should_add_spi_device);
  RUN_TEST(test_function_should_test_size_of_lcd_init_cmd_t);
  RUN_TEST(test_function_should_check_gpio_defines);
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