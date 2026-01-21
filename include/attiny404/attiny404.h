/**
 * @file attiny404.h
 * @brief ATtiny404 HAL - Top-level header
 *
 * Include this header to access all HAL functionality for ATtiny404.
 * Configured for GPIO (PORTA/PORTB), TCA0/TCB0 timers, ADC, USART0, TWI0, SPI0
 */

#ifndef HAL_TINY404_H
#define HAL_TINY404_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio404/gpio.h"
#include "timer404/tca0.h"
#include "timer404/tcb0.h"
#include "adc404/adc.h"
#include "usart404/usart.h"
#include "twi404/twi.h"
#include "spi404/spi.h"
#include "util/assert.h"
#include "util/atomic.h"

#ifdef __cplusplus
}
#endif

#endif // HAL_TINY404_H
