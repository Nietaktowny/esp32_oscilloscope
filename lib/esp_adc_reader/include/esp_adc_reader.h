


#define ADC_READER_UNIT                    ADC_UNIT_1
#define _ADC_READER_UNIT_STR(unit)         #unit
#define ADC_READER_UNIT_STR(unit)          _EXAMPLE_ADC_UNIT_STR(unit)
#define ADC_READER_CONV_MODE               ADC_CONV_SINGLE_UNIT_1
#define ADC_READER_ATTEN                   ADC_ATTEN_DB_0
#define ADC_READER_BIT_WIDTH               SOC_ADC_DIGI_MAX_BITWIDTH
#define ADC_READER_READ_LEN                256

#define ADC_READER_OUTPUT_TYPE             ADC_DIGI_OUTPUT_FORMAT_TYPE1
#define ADC_READER_GET_CHANNEL(p_data)     ((p_data)->type1.channel)
#define ADC_READER_GET_DATA(p_data)        ((p_data)->type1.data)

void adc_reader_init(void);