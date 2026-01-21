/**
 * @file uart_demo.c
 * @brief UART demo for ATtiny404
 *
 * Sends "Hello World" via USART0 at 115200 baud.
 */

#include <avr/io.h>
#include "attiny404.h"

int main(void) {
    usart_config_t config = {
        .baud = USART_BAUD_115200,
        .databits = USART_DATABITS_8,
        .parity = USART_PARITY_NONE,
        .stopbits = USART_STOPBITS_1
    };

    usart_t uart = usart_init(config);

    while (1) {
        usart_puts(&uart, "Hello World!\r\n");
        _delay_ms(1000);
    }
}
