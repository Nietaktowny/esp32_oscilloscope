
#include "lcd_controller.h"

/** @defgroup tags Log tags
 *  Tags used for logging.
 *  @{
 */
//static const char* SPI_TAG = "spi";                    // tells that something go wrong in SPI bus
static const char* LCD_TAG = "lcd device";             //tells that something is wrong with LCD device.
//static const char* LCD_CONTR_TAG = "lcd controller";   //general log tag
/** @} */

spi_device_handle_t spi;

DRAM_ATTR static const lcd_init_cmd_t ili_init_cmds[]={
    /* Power contorl B, power control = 0, DC_ENA = 1 */
    {0xCF, {0x00, 0x83, 0X30}, 3},
    /* Power on sequence control,
     * cp1 keeps 1 frame, 1st frame enable
     * vcl = 0, ddvdh=3, vgh=1, vgl=2
     * DDVDH_ENH=1
     */
    {0xED, {0x64, 0x03, 0X12, 0X81}, 4},
    /* Driver timing control A,
     * non-overlap=default +1
     * EQ=default - 1, CR=default
     * pre-charge=default - 1
     */
    {0xE8, {0x85, 0x01, 0x79}, 3},
    /* Power control A, Vcore=1.6V, DDVDH=5.6V */
    {0xCB, {0x39, 0x2C, 0x00, 0x34, 0x02}, 5},
    /* Pump ratio control, DDVDH=2xVCl */
    {0xF7, {0x20}, 1},
    /* Driver timing control, all=0 unit */
    {0xEA, {0x00, 0x00}, 2},
    /* Power control 1, GVDD=4.75V */
    {0xC0, {0x26}, 1},
    /* Power control 2, DDVDH=VCl*2, VGH=VCl*7, VGL=-VCl*3 */
    {0xC1, {0x11}, 1},
    /* VCOM control 1, VCOMH=4.025V, VCOML=-0.950V */
    {0xC5, {0x35, 0x3E}, 2},
    /* VCOM control 2, VCOMH=VMH-2, VCOML=VML-2 */
    {0xC7, {0xBE}, 1},
    /* Memory access contorl, MX=MY=0, MV=1, ML=0, BGR=1, MH=0 */
    {0x36, {0x28}, 1},
    /* Pixel format, 16bits/pixel for RGB/MCU interface */
    {0x3A, {0x55}, 1},
    /* Frame rate control, f=fosc, 70Hz fps */
    {0xB1, {0x00, 0x1B}, 2},
    /* Enable 3G, disabled */
    {0xF2, {0x08}, 1},
    /* Gamma set, curve 1 */
    {0x26, {0x01}, 1},
    /* Positive gamma correction */
    {0xE0, {0x1F, 0x1A, 0x18, 0x0A, 0x0F, 0x06, 0x45, 0X87, 0x32, 0x0A, 0x07, 0x02, 0x07, 0x05, 0x00}, 15},
    /* Negative gamma correction */
    {0XE1, {0x00, 0x25, 0x27, 0x05, 0x10, 0x09, 0x3A, 0x78, 0x4D, 0x05, 0x18, 0x0D, 0x38, 0x3A, 0x1F}, 15},
    /* Column address set, SC=0, EC=0xEF */
    {0x2A, {0x00, 0x00, 0x00, 0xEF}, 4},
    /* Page address set, SP=0, EP=0x013F */
    {0x2B, {0x00, 0x00, 0x01, 0x3f}, 4},
    /* Memory write */
    {0x2C, {0}, 0},
    /* Entry mode set, Low vol detect disabled, normal display */
    {0xB7, {0x07}, 1},
    /* Display function control */
    {0xB6, {0x0A, 0x82, 0x27, 0x00}, 4},
    /* Sleep out */
    {0x11, {0}, 0x80},
    /* Display on */
    {0x29, {0}, 0x80},
    {0, {0}, 0xff},
};

void lcd_command(spi_device_handle_t spi, const uint8_t cmd, bool keep_cs_active)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=8;                     //Command is 8 bits
    t.tx_buffer=&cmd;               //The data is the cmd itself
    t.user=(void*)0;                //D/C needs to be set to 0
    if (keep_cs_active) {
      t.flags = SPI_TRANS_CS_KEEP_ACTIVE;   //Keep CS active after data transfer
    }
    ret=spi_device_polling_transmit(spi, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.
}

void lcd_data(spi_device_handle_t spi, const uint8_t *data, int len)
{
    esp_err_t ret;
    spi_transaction_t t;
    if (len==0) return;             //no need to send anything
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=len*8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer=data;               //Data
    t.user=(void*)1;                //D/C needs to be set to 1
    ret=spi_device_polling_transmit(spi, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.
}


esp_err_t init_non_spi_gpio (void) {
    gpio_config_t gpio_conf = {};
    gpio_conf.pin_bit_mask = ((1ULL<<PIN_NUM_DC) | (1ULL<<PIN_NUM_RST) | (1ULL<<PIN_NUM_BCKL));
    gpio_conf.mode = GPIO_MODE_OUTPUT;
    gpio_conf.pull_up_en = true;
    return gpio_config(&gpio_conf);
}


void reset_display(void) {
    gpio_set_level(PIN_NUM_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(PIN_NUM_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
    ESP_LOGI(LCD_TAG, "LCD Display RESET.");
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
static esp_err_t init_spi_bus (void) {
    volatile esp_err_t err = ESP_OK;
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

    spi_device_interface_config_t devcfg={       
        .clock_speed_hz=10*1000*1000,           //Clock out at 10 MHz
        .mode=0,                                //SPI mode 0
        .spics_io_num=PIN_NUM_CS,               //CS pin
        .queue_size=7,                          //We want to be able to queue 7 transactions at a time
        .pre_cb=lcd_spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line
    };

    err = spi_bus_add_device(LCD_HOST, &devcfg, &spi);
    ERR_CHECK(err, return err);


    ERR_CHECK(err, return err);

    return err;
}



uint32_t lcd_get_id (spi_device_handle_t spi) {
    spi_device_acquire_bus(spi, portMAX_DELAY);
    lcd_command(spi, 0x0A, true);

    spi_transaction_t t;
    memset(&t, 0, sizeof t);
    t.length=8*3;
    t.flags = SPI_TRANS_USE_RXDATA;
    t.user = (void*)1;

    esp_err_t err = spi_device_polling_transmit(spi, &t);
    assert (err == ESP_OK);
    
    spi_device_release_bus(spi);
    
    return *(uint32_t*)t.rx_data;

}

esp_err_t init_lcd (spi_device_handle_t spi) {
    esp_err_t err = ESP_OK;
    int cmd = 0;
    uint32_t mask = (1ULL<<2);

    ERR_CHECK(init_non_spi_gpio(), return err);
    reset_display();

    //Send all the commands
    while (ili_init_cmds[cmd].databytes!=0xff) {
        ESP_LOGI(LCD_TAG, "Sending LCD command id: %d", cmd);
        lcd_command(spi, ili_init_cmds[cmd].cmd, false);
        lcd_data(spi, ili_init_cmds[cmd].data, ili_init_cmds[cmd].databytes&0x1F);
        if (ili_init_cmds[cmd].databytes&0x80) {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        cmd++;
    }

    uint32_t lcd_status = lcd_get_id(spi);
    printf("LCD ID: %#lx\n", lcd_status);
    if ((lcd_status & mask) == mask) {
        ESP_LOGI(LCD_TAG, "VALUE %#lx IS ON", mask);
        ESP_LOGI(LCD_TAG, "MASK IS: %#lx", mask);
        ESP_LOGI(LCD_TAG, "MASKED VALUE IS: %#lx", (lcd_status & mask));
    } else {
        ESP_LOGI(LCD_TAG, "VALUE %#lx IS OFF", mask);
        ESP_LOGI(LCD_TAG, "MASK IS: %#lx", mask);
        ESP_LOGI(LCD_TAG, "MASKED VALUE IS: %#lx", (lcd_status & mask));
    }

    ///Enable backlight
    gpio_set_level(PIN_NUM_BCKL, 0);

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

    init_lcd(spi);


    return err;
}