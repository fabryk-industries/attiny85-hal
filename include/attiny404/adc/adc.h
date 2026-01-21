#ifndef HAL_ADC404_H
#define HAL_ADC404_H

#include <stdint.h>
#include <avr/io.h>

typedef enum {
    ADC_CH_AIN0 = 0x00,
    ADC_CH_AIN1 = 0x01,
    ADC_CH_AIN2 = 0x02,
    ADC_CH_AIN3 = 0x03,
    ADC_CH_AIN4 = 0x04,
    ADC_CH_AIN5 = 0x05,
    ADC_CH_AIN6 = 0x06,
    ADC_CH_AIN7 = 0x07,
    ADC_CH_AIN8 = 0x08,
    ADC_CH_AIN9 = 0x09,
    ADC_CH_AIN10 = 0x0A,
    ADC_CH_AIN11 = 0x0B,
    ADC_CH_TEMPSENSE = 0x1E,
    ADC_CH_DAC0 = 0x1F,
} adc_channel_t;

typedef enum {
    ADC_REF_VDD = 0x00,
    ADC_REF_INTERNAL_0V55 = 0x01,
    ADC_REF_INTERNAL_1V1 = 0x02,
    ADC_REF_INTERNAL_1V5 = 0x03,
    ADC_REF_INTERNAL_2V5 = 0x04,
    ADC_REF_INTERNAL_4V34 = 0x05,
    ADC_REF_EXTERNAL = 0x06,
} adc_reference_t;

typedef enum {
    ADC_PRESCALER_DIV2 = 0x00,
    ADC_PRESCALER_DIV4 = 0x01,
    ADC_PRESCALER_DIV8 = 0x02,
    ADC_PRESCALER_DIV16 = 0x03,
    ADC_PRESCALER_DIV32 = 0x04,
    ADC_PRESCALER_DIV64 = 0x05,
    ADC_PRESCALER_DIV128 = 0x06,
    ADC_PRESCALER_DIV256 = 0x07,
} adc_prescaler_t;

typedef enum {
    ADC_RES_8BIT,
    ADC_RES_10BIT,
} adc_resolution_t;

typedef struct {
    adc_reference_t ref;
    adc_prescaler_t prescaler;
    adc_resolution_t resolution;
    uint8_t in_progress:1;
    uint8_t channel;
} adc_t;

adc_t adc_init(adc_reference_t ref, adc_prescaler_t prescaler, adc_resolution_t resolution);

void adc_enable(adc_t *adc);

void adc_disable(adc_t *adc);

uint16_t adc_read_blocking(adc_t *adc, adc_channel_t channel);

void adc_read_start(adc_t *adc, adc_channel_t channel);

uint8_t adc_is_busy(adc_t *adc);

uint16_t adc_read_result(adc_t *adc);

#endif
