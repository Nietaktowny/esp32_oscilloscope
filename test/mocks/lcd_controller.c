
#include "lcd_controller.h"

void lcd_command(spi_device_handle_t spi, const uint8_t cmd, bool keep_cs_active) {
    //TODO
}


/**
 * @brief Prepares SPI bus for use.
 *
 * Function to initialize SPI bus. It prepares also and configures device for SPI bus.
 * This is where also pins for SPI bus are passed to spi_bus_config_t .
 *
 * @note It currently ignores 'SPI bus already initialized' error, //TODO checking if SPI bus is already initialized.
 *
 * 
 * @return
 *         - ESP_OK                on success
 */
esp_err_t init_spi_bus (void) {
    volatile esp_err_t err = ESP_OK;
    spi_device_handle_t spi;
    spi_bus_config_t buscfg={
        .miso_io_num=PIN_NUM_MISO,
        .mosi_io_num=PIN_NUM_MOSI,
        .sclk_io_num=PIN_NUM_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=PARALLEL_LINES*320*2+8
    };

    Try {
        //TODO Mocks of spi_bus_initialize and tests with them
        err = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO); //different from 0
        if (err != 0 && err != ERR_SPI_ALREADY_INIT) {
            Throw(err);
        }

    } Catch(err) {
        ERR_CHECK(err, return err);
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

    ERR_CHECK(err, return err);

    return err;
}

esp_err_t init_lcd (spi_device_handle_t spi) {
    esp_err_t err = ESP_OK;

    return err;
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


esp_err_t start_lcd_controller (void) {
    esp_err_t err = ESP_OK;
    //first prepare SPI bus and add LCD device.
    err = init_spi_bus();




    return err;
}