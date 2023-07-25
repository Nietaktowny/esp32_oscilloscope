#include "lcd_controller.h"


esp_err_t init_lcd (void) {
    spi_device_handle_t spi;
    spi_bus_config_t buscfg={
        .miso_io_num=PIN_NUM_MISO,
        .mosi_io_num=PIN_NUM_MOSI,
        .sclk_io_num=PIN_NUM_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=PARALLEL_LINES*320*2+8
    };

    esp_err_t err = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (err != ESP_OK)
    {
        return err;
    }

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

    if (err != ESP_OK)
    {
        return err;
    }

    
    return ESP_OK;
}

//This function is called (in irq context!) just before a transmission starts. It will
//set the D/C line to the value indicated in the user field.
void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc=(int)t->user;
    gpio_set_level(PIN_NUM_DC, dc);
}

int lcd_reset(int i) {
    return i;
}