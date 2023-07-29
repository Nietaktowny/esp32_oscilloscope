#pragma once

#ifndef LCD_CONTROLLER_H
#define LCD_CONTROLLER_H

#include <inttypes.h>
#include "CException.h"
#include <stdbool.h>
#include <stdio.h>


#ifndef _MOCK_SPI_DRIVER_MOCKS_H
#include "spi_driver_mocks.h"
#endif


#define ERR_SPI_ALREADY_INIT 0x103


#define ERR_CHECK(x, y) do {   int retval = (x);   if (retval != 0) {     fprintf(stderr, "Runtime error: %s returned %d at %s:%d", "mock return val", retval, __FILE__, __LINE__);     y;   } } while (0)


#define LCD_HOST    HSPI_HOST

#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22

#define PIN_NUM_DC   21
#define PIN_NUM_RST  18
#define PIN_NUM_BCKL 5



#define PARALLEL_LINES 16


typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; 
} lcd_init_cmd_t;

typedef enum {
    LCD_TYPE_ILI = 1,
    LCD_TYPE_ST,
    LCD_TYPE_MAX,
} type_lcd_t;

void lcd_spi_pre_transfer_callback(spi_transaction_t *t);

esp_err_t init_spi_bus (void);



esp_err_t init_lcd (spi_device_handle_t spi);



esp_err_t start_lcd_controller(void);


int lcd_reset(int);

void lcd_command(spi_device_handle_t spi, const uint8_t cmd, bool keep_cs_active);

esp_err_t init_lcd (spi_device_handle_t spi);


#endif 