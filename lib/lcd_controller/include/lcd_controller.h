#pragma once

#ifndef LCD_CONTROLLER_H
#define LCD_CONTROLLER_H

#include <inttypes.h>
#include "CException.h"

//Check for ESP-IDF platform specific header files, to avoid compliation errors when testing on native platform.
#ifdef ESP_PLATFORM
#include "driver/spi_master.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "esp_log.h"
#endif

//ERROR telling us that SPI bus is already initialized.
#define ERR_SPI_ALREADY_INIT 0x103


#define ERR_CHECK(x, y) do { \
  int retval = (x); \
  if (retval != 0) { \
    fprintf(stderr, "Runtime error: %s returned %d at %s:%d", esp_err_to_name(x), retval, __FILE__, __LINE__); \
    y; \
  } \
} while (0)

//LCD GPIO definitions
#define LCD_HOST    HSPI_HOST

#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22

#define PIN_NUM_DC   21
#define PIN_NUM_RST  18
#define PIN_NUM_BCKL 5

//To speed up transfers, every SPI transfer sends a bunch of lines. This define specifies how many. More means more memory use,
//but less overhead for setting up / finishing transfers. Make sure 240 is dividable by this.
#define PARALLEL_LINES 16

/*
 The LCD needs a bunch of command/argument values to be initialized. They are stored in this struct.
*/
typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;

typedef enum {
    LCD_TYPE_ILI = 1,
    LCD_TYPE_ST,
    LCD_TYPE_MAX,
} type_lcd_t;

void lcd_spi_pre_transfer_callback(spi_transaction_t *t);


/**
 * @brief Sends starting commands to LCD to get it working.
 *
 * //TODO
 *
 * @note This should be called only once in application.
 *
 * 
 * @return
 *         - ESP_OK                on success
 */
esp_err_t init_lcd (spi_device_handle_t spi);


/**
 * @brief Starting point of lcd_controller library.
 *
 * This is library 'main' function, in which all needed resources are initialized and prepared with use of another functions.
 *
 * @note This should be called only once in application.
 *
 * 
 * @return
 *         - ESP_ERR_NO_MEM        if out of memory
 *         - ESP_OK                on success
 */
esp_err_t start_lcd_controller(void);


int lcd_reset(int);

void lcd_command(spi_device_handle_t spi, const uint8_t cmd, bool keep_cs_active);


#endif /*LCD_CONTROLLER_H*/