#ifndef HAL_USART404_H
#define HAL_USART404_H

#include <stdint.h>
#include <avr/io.h>

typedef enum {
    USART_BAUD_9600,
    USART_BAUD_19200,
    USART_BAUD_38400,
    USART_BAUD_57600,
    USART_BAUD_115200,
} usart_baud_t;

typedef enum {
    USART_DATABITS_5,
    USART_DATABITS_6,
    USART_DATABITS_7,
    USART_DATABITS_8,
    USART_DATABITS_9,
} usart_databits_t;

typedef enum {
    USART_PARITY_NONE,
    USART_PARITY_EVEN,
    USART_PARITY_ODD,
} usart_parity_t;

typedef enum {
    USART_STOPBITS_1,
    USART_STOPBITS_2,
} usart_stopbits_t;

typedef struct {
    usart_baud_t baud;
    usart_databits_t databits;
    usart_parity_t parity;
    usart_stopbits_t stopbits;
} usart_config_t;

typedef struct {
    usart_config_t config;
} usart_t;

usart_t usart_init(usart_config_t config);

void usart_putc(uint8_t data);

void usart_puts(usart_t *usart, const char *str);

uint8_t usart_getc();

uint8_t usart_available();

void usart_deinit(void);

#endif
