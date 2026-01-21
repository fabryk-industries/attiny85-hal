/**
 * @file uart.h
 * @brief UART using USI + Timer0 for ATtiny85
 *
 * ATtiny85 has no hardware UART. This implementation uses
 * USI Three-Wire mode combined with Timer0 for half-duplex UART.
 *
 * Implementation:
 * - Timer0 in CTC mode generates bit timing for baud rate
 * - Pin change interrupt detects start bit on RX line
 * - USI Three-Wire mode shifts data bits (8 bits = 16 clock edges)
 * - Half-duplex operation (transmit OR receive, not both simultaneously)
 * - Bit reversal required (UART LSB-first, USI MSB-first)
 *
 * Benefits over pure bitbanging:
 * - ~30-40% smaller code size
 * - Lower CPU usage (Timer0 handles timing)
 * - More accurate baud rate generation
 * - Hardware-assisted bit shifting via USI
 *
 * Hardware:
 * - RX: PB0 (pin 5) - USI DI for receive
 * - TX: PB1 (pin 6) - USI DO for transmit
 * - Uses Timer0 for baud rate generation
 *
 * Based on: AVR307 Application Note - Half Duplex UART Using USI Module
 */

#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdint.h>

/**
 * @defgroup hal_uart UART
 * @brief Software UART (bit-banging)
 * @{
 */

/**
 * @brief UART configuration
 */
typedef struct {
    uint8_t tx_pin;
    uint8_t rx_pin;
    uint32_t baudrate;
} uart_config_t;

/**
 * @brief UART handle
 */
typedef struct {
    uart_config_t config;
    uint8_t state;
} uart_t;

/**
 * @brief Initialize UART
 *
 * Configures TX/RX pins and baudrate.
 *
 * @param config UART configuration (TX/RX pins, baudrate)
 * @return UART handle
 */
uart_t uart_init(uart_config_t config);

/**
 * @brief Transmit byte
 *
 * Sends single byte via TX pin.
 *
 * @param uart UART handle
 * @param data Byte to transmit
 */
void uart_putc(uart_t *uart, uint8_t data);

/**
 * @brief Transmit string
 *
 * Sends null-terminated string via TX pin.
 *
 * @param uart UART handle
 * @param str Null-terminated string
 */
void uart_puts(uart_t *uart, const char *str);

/**
 * @brief Receive byte with timeout
 *
 * Waits for start bit, then receives byte via RX pin.
 *
 * @param uart UART handle
 * @param data Pointer to store received byte
 * @param timeout_us Timeout in microseconds
 * @return Non-zero if byte received
 */
uint8_t uart_getc(uart_t *uart, uint8_t *data, uint32_t timeout_us);

/**
 * @brief Check if data available
 *
 * @param uart UART handle
 * @return Non-zero if start bit detected
 */
uint8_t uart_available(uart_t *uart);

/** @} */ // end of hal_uart

#endif // HAL_UART_H
