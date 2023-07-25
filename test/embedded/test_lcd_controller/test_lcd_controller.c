#include "unity.h"
#include "lcd_controller.h"
#include "esp_system.h"
#include "Mocklcd_controller.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_function_should_not_already_init_spi_bus (void) {
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
  UnityAssertEqualNumber(259, err, "INIT READY SPI BUS WITHOUT CHECK", __LINE__, UNITY_DISPLAY_STYLE_INT32);
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
  //when
  esp_err_t err = init_lcd();
  //then
  UnityAssertEqualNumber(0, err, "INIT LCD RETURNS OK", __LINE__, UNITY_DISPLAY_STYLE_INT16);
}


/*To add test use: RUN_TEST(test_name) macro.*/
int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_check_if_init_lcd_returns_ESP_OK);
  RUN_TEST(test_function_should_not_already_init_spi_bus);
  RUN_TEST(test_function_should_add_spi_device);
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