#include <stdint.h>
#include <avr/io.h>
#include "attiny404/adc/adc.h"

adc_t adc_init(adc_reference_t ref, adc_prescaler_t prescaler, adc_resolution_t resolution) {
    ADC0.CTRLA = 0;
    ADC0.CTRLC = 0;

    // Set reference and prescaler in CTRLC
    ADC0.CTRLC = (ref << ADC_REFSEL_gp) | prescaler;

    // Set resolution in CTRLA (bit 7: RESSEL, 1 = 8-bit, 0 = 10-bit)
    if (resolution == ADC_RES_8BIT) {
        ADC0.CTRLA |= ADC_RESSEL_bm;
    }

    adc_t adc = {ref, prescaler, resolution, 0, 0};
    return adc;
}

void adc_enable(adc_t *adc) {
    ADC0.CTRLA |= ADC_ENABLE_bm;
}

void adc_disable(adc_t *adc) {
    ADC0.CTRLA &= ~ADC_ENABLE_bm;
}

uint16_t adc_read_blocking(adc_t *adc, adc_channel_t channel) {
    adc->channel = channel;
    adc->in_progress = 1;

    ADC0.MUXPOS = channel;
    ADC0.COMMAND = ADC_STCONV_bm;

    while ((ADC0.INTFLAGS & ADC_RESRDY_bm) == 0);

    adc->in_progress = 0;

    return ADC0.RES;
}

void adc_read_start(adc_t *adc, adc_channel_t channel) {
    adc->channel = channel;
    adc->in_progress = 1;

    ADC0.MUXPOS = channel;
    ADC0.COMMAND = ADC_STCONV_bm;
}

uint8_t adc_is_busy(adc_t *adc) {
    if (ADC0.INTFLAGS & ADC_RESRDY_bm) {
        adc->in_progress = 0;
        return 0;
    }
    return adc->in_progress;
}

uint16_t adc_read_result(adc_t *adc) {
    return ADC0.RES;
}
