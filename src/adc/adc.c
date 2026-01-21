/**
 * @file adc.c
 * @brief ADC implementation for ATtiny85
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc/adc.h"

adc_t adc_init(adc_reference_t ref, adc_prescaler_t prescaler) {
    uint8_t ref_bits = 0;

    switch (ref) {
        case ADC_REF_VCC:
            ref_bits = 0;
            break;
        case ADC_REF_INTERNAL_1V1:
            ref_bits = (1 << REFS0);
            break;
        case ADC_REF_EXTERNAL:
            ref_bits = (1 << REFS1);
            break;
    }

    ADMUX = ref_bits;
    ADCSRA = prescaler;

    adc_t adc = {
        .ref = ref,
        .prescaler = prescaler,
        .in_progress = 0,
        .channel = 0
    };
    return adc;
}

void adc_enable(adc_t *adc) {
    ADCSRA |= (1 << ADEN);
}

void adc_disable(adc_t *adc) {
    ADCSRA &= ~(1 << ADEN);
    adc->in_progress = 0;
}

uint16_t adc_read_blocking(adc_t *adc, adc_channel_t channel) {
    uint16_t result;

    ADMUX = (ADMUX & 0xF0) | channel;
    ADCSRA |= (1 << ADSC);

    while (ADCSRA & (1 << ADSC)) {
    }

    result = ADCL;
    result |= (ADCH << 8);

    adc->channel = 0;
    return result;
}

adc_status_t adc_read_start(adc_t *adc, adc_channel_t channel) {
    if (adc->in_progress) {
        return ADC_BUSY;
    }

    ADMUX = (ADMUX & 0xF0) | channel;
    ADCSRA |= (1 << ADSC);

    adc->in_progress = 1;
    adc->channel = channel;

    return ADC_BUSY;
}

adc_status_t adc_read_poll(adc_t *adc, uint16_t *out_value) {
    if (!(ADCSRA & (1 << ADSC))) {
        if (ADCSRA & (1 << ADIF)) {
            uint16_t result = ADCL;
            result |= (ADCH << 8);

            if (out_value) {
                *out_value = result;
            }

            adc->in_progress = 0;
            return ADC_OK;
        }
    }

    return ADC_BUSY;
}

uint8_t adc_is_busy(adc_t *adc) {
    return adc->in_progress;
}

ISR(ADC_vect) {
    (void)ADC;
    ADCSRA |= (1 << ADIF);
}
