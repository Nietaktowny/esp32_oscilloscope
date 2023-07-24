#include "unity.h"
#include "Mocklcd_controller.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_function_should_init_spi_bus (void) {
      spi_bus_config_t buscfg={
        .miso_io_num=PIN_NUM_MISO,
        .mosi_io_num=PIN_NUM_MOSI,
        .sclk_io_num=PIN_NUM_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=PARALLEL_LINES*320*2+8
    };

    esp_err_t err = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);

    UnityAssertEqualNumber(0, err, "INIT SPI BUS", __LINE__, UNITY_DISPLAY_STYLE_INT32);
}

void test_function_should_init_lcd(void) {
  //given
  init_lcd_Expect(1);

  //when

  //then
  UnityAssertEqualNumber(1, 1, "TEST LIB", 18, UNITY_DISPLAY_STYLE_INT16);
}


/*To add test use: RUN_TEST(test_name) macro.*/
int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_init_lcd);
  RUN_TEST(test_function_should_init_spi_bus);
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