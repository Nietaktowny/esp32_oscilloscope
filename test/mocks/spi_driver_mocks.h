#include <inttypes.h>

typedef int esp_err_t;


#define ESP_OK          0       
#define ESP_FAIL        -1      

#define ESP_ERR_NO_MEM              0x101   
#define ESP_ERR_INVALID_ARG         0x102   
#define ESP_ERR_INVALID_STATE       0x103   
#define ESP_ERR_INVALID_SIZE        0x104   
#define ESP_ERR_NOT_FOUND           0x105   
#define ESP_ERR_NOT_SUPPORTED       0x106   
#define ESP_ERR_TIMEOUT             0x107   
#define ESP_ERR_INVALID_RESPONSE    0x108   
#define ESP_ERR_INVALID_CRC         0x109   
#define ESP_ERR_INVALID_VERSION     0x10A   
#define ESP_ERR_INVALID_MAC         0x10B   
#define ESP_ERR_NOT_FINISHED        0x10C   


#define ESP_ERR_WIFI_BASE           0x3000  
#define ESP_ERR_MESH_BASE           0x4000  
#define ESP_ERR_FLASH_BASE          0x6000  
#define ESP_ERR_HW_CRYPTO_BASE      0xc000  
#define ESP_ERR_MEMPROT_BASE        0xd000  

typedef struct spi_device_t *spi_device_handle_t;  

typedef enum {

    SPI1_HOST=0,    
    SPI2_HOST=1,    
#if SOC_SPI_PERIPH_NUM > 2
    SPI3_HOST=2,    
#endif
    SPI_HOST_MAX,   
} spi_host_device_t;

typedef enum {
  SPI_DMA_DISABLED = 0,     
#if CONFIG_IDF_TARGET_ESP32
  SPI_DMA_CH1      = 1,     
  SPI_DMA_CH2      = 2,     
#endif
  SPI_DMA_CH_AUTO  = 3,     
} spi_common_dma_t;

#if __cplusplus

typedef int spi_dma_chan_t;
#else
typedef spi_common_dma_t spi_dma_chan_t;
#endif

typedef struct {
    union {
      int mosi_io_num;    
      int data0_io_num;   
    };
    union {
      int miso_io_num;    
      int data1_io_num;   
    };
    int sclk_io_num;      
    union {
      int quadwp_io_num;  
      int data2_io_num;   
    };
    union {
      int quadhd_io_num;  
      int data3_io_num;   
    };
    int data4_io_num;     
    int data5_io_num;     
    int data6_io_num;     
    int data7_io_num;     
    int max_transfer_sz;  
    uint32_t flags;       
    int intr_flags;       
} spi_bus_config_t;

typedef struct spi_transaction_t spi_transaction_t;
typedef void(*transaction_cb_t)(spi_transaction_t *trans);

typedef struct {
    uint8_t command_bits;           
    uint8_t address_bits;           
    uint8_t dummy_bits;             
    uint8_t mode;                   
    uint16_t duty_cycle_pos;         
    uint16_t cs_ena_pretrans;        
    uint8_t cs_ena_posttrans;       
    int clock_speed_hz;             
    int input_delay_ns;             
    int spics_io_num;               
    uint32_t flags;                 
    int queue_size;                 
    transaction_cb_t pre_cb;   
    transaction_cb_t post_cb;  
} spi_device_interface_config_t;


esp_err_t spi_bus_add_device(spi_host_device_t host_id, const spi_device_interface_config_t *dev_config, spi_device_handle_t *handle);

esp_err_t spi_bus_initialize(spi_host_device_t host_id, const spi_bus_config_t *bus_config, spi_dma_chan_t dma_chan);

char *esp_err_to_name(esp_err_t code);