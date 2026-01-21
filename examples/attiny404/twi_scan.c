/**
 * @file twi_scan.c
 * @brief TWI/I2C demo for ATtiny404
 *
 * Scans I2C bus for connected devices.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "attiny404/attiny404.h"

int main(void) {
    usart_config_t uart_config = {
        .baud = USART_BAUD_115200,
        .databits = USART_DATABITS_8,
        .parity = USART_PARITY_NONE,
        .stopbits = USART_STOPBITS_1
    };

    twi_config_t twi_config = {
        .baud = TWI_BAUD_100KHZ
    };

    usart_t uart = usart_init(uart_config);
    twi_t twi = twi_init(twi_config);

    usart_puts(&uart, "I2C Scanner\r\n");

    while (1) {
        uint8_t found = 0;
        for (uint8_t addr = 1; addr < 128; addr++) {
            twi_start();
            if (TWI0.MSTATUS & TWI_RXACK_bm) {
                char buf[32];
                sprintf(buf, "Found: 0x%02X\r\n", addr);
                usart_puts(&uart, buf);
                found = 1;
            }
            twi_stop();
        }
        if (!found) {
            usart_puts(&uart, "No devices found\r\n");
        }
        _delay_ms(2000);
    }
}
