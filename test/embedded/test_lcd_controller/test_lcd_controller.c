#include "unity.h"
#include "unity_internals.h"
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

void test_function_should_check_if_ERR_SPI_ALREADY_INIT_exists(void) {
  //given
  const int err_spi_init_value = 0x103;

  //then
  UnityAssertEqualNumber(err_spi_init_value, ERR_SPI_ALREADY_INIT, "ERR_SPI_ALREADY_INIT - DOESN'T EXIST", __LINE__, UNITY_DISPLAY_STYLE_INT32);
}

void test_function_should_check_that_spi_bus_init_returns_esp_ok_even_when_initialized_already (void) {
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
  vTaskDelay(pdMS_TO_TICKS(10));

  //then
  UnityAssertEqualNumber(ESP_OK, err, "SPI BUS ALREADY INIT", __LINE__, UNITY_DISPLAY_STYLE_INT32);
}

void test_function_should_check_if_lcd_cmd_function_exists(void) {
  //given
  spi_device_handle_t spi;
  uint8_t cmd = 12;
  bool keep_cs_active = false;

  spi_device_interface_config_t devcfg={
      .clock_speed_hz=10*1000*1000,           //Clock out at 10 MHz
      .mode=0,                                //SPI mode 0
      .spics_io_num=PIN_NUM_CS,               //CS pin
      .queue_size=7,                          //We want to be able to queue 7 transactions at a time
      .pre_cb=lcd_spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line
  };

  //when
  spi_bus_add_device(LCD_HOST, &devcfg, &spi);

  //then
  lcd_command(spi, cmd, keep_cs_active);
  TEST_ASSERT(1);
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
  spi_device_handle_t t_spi = NULL;
  //when
  err = init_lcd(t_spi);
  //then
  UnityAssertEqualNumber(0, err, "INIT LCD RETURNS OK", __LINE__, UNITY_DISPLAY_STYLE_INT16);
}

void test_function_should_check_if_start_lcd_controller_returns_ESP_OK(void) {
  //given
  esp_err_t err;

  //when
  err = start_lcd_controller();

  //then
  UnityAssertEqualNumber(0, err, "start_lcd_controller returns ESP_OK", __LINE__, UNITY_DISPLAY_STYLE_INT32);
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

void test_should_execute_macro_spi_already_initialized(void) {
  //given
  spi_bus_config_t buscfg={
      .miso_io_num=PIN_NUM_MISO,
      .mosi_io_num=PIN_NUM_MOSI,
      .sclk_io_num=PIN_NUM_CLK,
      .quadwp_io_num=-1,
      .quadhd_io_num=-1,
      .max_transfer_sz=PARALLEL_LINES*320*2+8
  };
  int spi_already_init = 0;

  //when
  ERR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO), spi_already_init++);

  //then
  UnityAssertEqualNumber(1, spi_already_init, "ERR_CHECK not found spi already init error", __LINE__, UNITY_DISPLAY_STYLE_INT32);
}

void test_should_throw_and_catch_err_because_spi_bus_is_already_initialized(void) {
  //given
  spi_bus_config_t buscfg={
      .miso_io_num=PIN_NUM_MISO,
      .mosi_io_num=PIN_NUM_MOSI,
      .sclk_io_num=PIN_NUM_CLK,
      .quadwp_io_num=-1,
      .quadhd_io_num=-1,
      .max_transfer_sz=PARALLEL_LINES*320*2+8
  };
  volatile int spi_already_init = 0;
  esp_err_t err = ESP_OK;

  //when
  Try {
    err = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO); //different from 0
    ERR_CHECK(err, Throw(err));                                   //should throw error
  } Catch(err) {
    spi_already_init++;
  }

  //then
  UnityAssertEqualNumber(1, spi_already_init, "Error not catched or spi bus nie initialized", __LINE__, UNITY_DISPLAY_STYLE_INT32);
}

void test_should_throw_and_handle_in_catch_err_because_spi_bus_already_initialized(void) {
  //given
  spi_bus_config_t buscfg={
      .miso_io_num=PIN_NUM_MISO,
      .mosi_io_num=PIN_NUM_MOSI,
      .sclk_io_num=PIN_NUM_CLK,
      .quadwp_io_num=-1,
      .quadhd_io_num=-1,
      .max_transfer_sz=PARALLEL_LINES*320*2+8
  };
  volatile int spi_already_init = 0;
  esp_err_t err = ESP_OK;

  //when
  Try {
    err = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO); //different from 0
    ERR_CHECK(err, Throw(err));                                   //should throw error
  } Catch(err) {
    if (err == ERR_SPI_ALREADY_INIT) {
      err = ESP_OK;              
    } else {
      spi_already_init++;
    }
  }

  //then
  UnityAssertEqualNumber(0, spi_already_init, "Error not properly handled in try/catch", __LINE__, UNITY_DISPLAY_STYLE_INT32);
  UnityAssertEqualNumber(ESP_OK, err, "Error not properly handled in try/catch", __LINE__, UNITY_DISPLAY_STYLE_INT32);
}


/*To add test use: RUN_TEST(test_name) macro.*/
int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_check_if_init_lcd_returns_ESP_OK);
  RUN_TEST(test_function_should_check_that_spi_bus_init_returns_esp_ok_even_when_initialized_already);
  RUN_TEST(test_function_should_add_spi_device);
  RUN_TEST(test_function_should_test_size_of_lcd_init_cmd_t);
  RUN_TEST(test_function_should_check_gpio_defines);
  RUN_TEST(test_function_should_check_if_lcd_cmd_function_exists);
  RUN_TEST(test_function_should_check_if_start_lcd_controller_returns_ESP_OK);
  RUN_TEST(test_function_should_check_if_ERR_SPI_ALREADY_INIT_exists);
  RUN_TEST(test_macro_should_not_execute_expression_if_no_error);
  RUN_TEST(test_macro_should_execute_expression_if_some_error_occurs);
  RUN_TEST(test_should_execute_macro_spi_already_initialized);
  RUN_TEST(test_should_throw_and_catch_err_because_spi_bus_is_already_initialized);
  RUN_TEST(test_should_throw_and_handle_in_catch_err_because_spi_bus_already_initialized);

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