#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart/uart.h"

uart_t uart_init(uart_config_t config) {
    DDRB |= (1 << config.tx_pin);
    DDRB &= ~(1 << config.rx_pin);

    PORTB |= (1 << config.tx_pin);
    PORTB |= (1 << config.rx_pin);

    uart_t uart;
    uart.config = config;
    uart.state = 0;
    return uart;
}

void uart_putc(uart_t *uart_ptr, uint8_t data) {
    uint8_t tx_pin_mask = 1 << uart_ptr->config.tx_pin;
    uint8_t i;

    // Start bit (LOW)
    PORTB &= ~tx_pin_mask;
    _delay_us(104);

    // 8 data bits, LSB first
    for (i = 0; i < 8; i++) {
        if (data & (1 << i)) {
            PORTB |= tx_pin_mask;
        } else {
            PORTB &= ~tx_pin_mask;
        }
        _delay_us(104);
    }

    // Stop bit (HIGH)
    PORTB |= tx_pin_mask;
    _delay_us(104);
}

void uart_puts(uart_t *uart_ptr, const char *str) {
    while (*str) {
        uart_putc(uart_ptr, (uint8_t)*str);
        str++;
    }
}

uint8_t uart_getc(uart_t *uart_ptr, uint8_t *data, uint32_t timeout_us) {
    uint8_t rx_pin_mask = 1 << uart_ptr->config.rx_pin;
    uint8_t received_byte = 0;
    uint8_t i;
    uint32_t start = 0;

    // Wait for start bit (LOW)
    while (start < timeout_us) {
        if (!(PINB & rx_pin_mask)) {
            // Sample at middle of bit
            _delay_us(52);

            // Read 8 data bits, LSB first
            for (i = 0; i < 8; i++) {
                _delay_us(104);
                if (PINB & rx_pin_mask) {
                    received_byte |= (1 << i);
                }
            }

            // Wait for stop bit
            _delay_us(104);

            *data = received_byte;
            return 1;
        }
        _delay_us(1);
        start++;
    }

    return 0;
}

uint8_t uart_available(uart_t *uart_ptr) {
    return !(PINB & (1 << uart_ptr->config.rx_pin));
}
