#include <stdint.h>
#include <avr/io.h>
#include "attiny404/usart/usart.h"

static uint32_t f_cpu_hz = 16000000UL;

usart_t usart_init(usart_config_t config) {
    uint16_t baud_setting = 0;

    switch (config.baud) {
        case USART_BAUD_9600:
            baud_setting = 1047;
            break;
        case USART_BAUD_19200:
            baud_setting = 523;
            break;
        case USART_BAUD_38400:
            baud_setting = 261;
            break;
        case USART_BAUD_57600:
            baud_setting = 173;
            break;
        case USART_BAUD_115200:
            baud_setting = 86;
            break;
    }

    USART0.BAUD = baud_setting;
    USART0.CTRLB = USART_TXEN_bm | USART_RXEN_bm;

    switch (config.databits) {
        case USART_DATABITS_5:
            USART0.CTRLC = USART_CHSIZE_5BIT_gc;
            break;
        case USART_DATABITS_6:
            USART0.CTRLC = USART_CHSIZE_6BIT_gc;
            break;
        case USART_DATABITS_7:
            USART0.CTRLC = USART_CHSIZE_7BIT_gc;
            break;
        case USART_DATABITS_8:
            USART0.CTRLC = USART_CHSIZE_8BIT_gc;
            break;
        case USART_DATABITS_9:
            USART0.CTRLC = USART_CHSIZE_9BITH_gc;
            break;
    }

    if (config.parity != USART_PARITY_NONE) {
        if (config.parity == USART_PARITY_EVEN) {
            USART0.CTRLC |= USART_PMODE_EVEN_gc;
        } else {
            USART0.CTRLC |= USART_PMODE_ODD_gc;
        }
    }

    if (config.stopbits == USART_STOPBITS_2) {
        USART0.CTRLC |= USART_SBMODE_2BIT_gc;
    }

    usart_t usart = {config};
    return usart;
}

void usart_putc(uint8_t data) {
    while (!(USART0.STATUS & USART_DREIF_bm));
    USART0.TXDATAL = data;
}

void usart_puts(usart_t *usart, const char *str) {
    while (*str) {
        usart_putc(*str++);
    }
}

uint8_t usart_getc() {
    while (!(USART0.STATUS & USART_RXCIF_bm));
    return USART0.RXDATAL;
}

uint8_t usart_available() {
    return (USART0.STATUS & USART_RXCIF_bm) ? 1 : 0;
}

void usart_deinit(void) {
    USART0.CTRLA = 0;
}
