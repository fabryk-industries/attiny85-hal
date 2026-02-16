/**
 * @file attiny85.h
 * @brief ATtiny85 HAL - Top-level header
 *
 * Include this header to access all HAL functionality for ATtiny85.
 */

#ifndef HAL_TINY85_H
#define HAL_TINY85_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio/gpio.h"
#include "timer/timer0.h"
#include "adc/adc.h"
#include "power/power.h"
#include "eeprom/eeprom.h"
#include "usi/spi.h"
#include "usi/i2c.h"
#include "uart/uart.h"
#include "util/assert.h"
#include "util/atomic.h"

#ifdef __cplusplus
}
#endif

#endif // HAL_TINY85_H
