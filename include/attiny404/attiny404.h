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

#include "gpio/gpio.h"
#include "timer/tca0.h"
#include "timer/tcb0.h"
#include "adc/adc.h"
#include "usart/usart.h"
#include "twi/twi.h"
#include "spi/spi.h"

#ifdef __cplusplus
}
#endif

#endif // HAL_TINY404_H
