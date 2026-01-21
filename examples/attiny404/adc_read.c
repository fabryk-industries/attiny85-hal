/**
 * @file adc_read.c
 * @brief ADC demo for ATtiny404
 *
 * Reads ADC channel 0 (PA0) and outputs value via UART.
 */

#include <avr/io.h>
#include <util/delay.h>
#include "attiny404.h"

int main(void) {
    usart_config_t uart_config = {
        .baud = USART_BAUD_115200,
        .databits = USART_DATABITS_8,
        .parity = USART_PARITY_NONE,
        .stopbits = USART_STOPBITS_1
    };

    adc_config_t adc_config = {
        .ref = ADC_REF_VDD,
        .prescaler = ADC_PRESCALER_DIV64,
        .resolution = ADC_RES_10BIT
    };

    usart_t uart = usart_init(uart_config);
    adc_t adc = adc_init(adc_config);
    adc_enable(&adc);

    while (1) {
        uint16_t value = adc_read_blocking(&adc, ADC_CH_AIN0);
        char buf[32];
        sprintf(buf, "ADC: %d\r\n", value);
        usart_puts(&uart, buf);
        _delay_ms(500);
    }
}
