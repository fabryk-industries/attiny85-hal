/**
 * @file power.c
 * @brief Power management implementation for ATtiny85
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "attiny85/power/power.h"

void hal_sleep_enable(sleep_mode_t mode) {
    switch (mode) {
        case SLEEP_IDLE:
            set_sleep_mode(SLEEP_MODE_IDLE);
            break;
        case SLEEP_ADC_NOISE_REDUCTION:
            set_sleep_mode(SLEEP_MODE_ADC);
            break;
        case SLEEP_POWER_DOWN:
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            break;
    }

    sleep_enable();
}

void hal_sleep_now(void) {
    sleep_mode();
}

void hal_wdt_enable(wdt_timeout_t timeout, wdt_mode_t mode) {
    uint8_t wdt_config = 0;

    switch (timeout) {
        case WDT_16MS:
            wdt_config = (0 << WDP3) | (0 << WDP2) | (0 << WDP1) | (0 << WDP0);
            break;
        case WDT_32MS:
            wdt_config = (0 << WDP3) | (0 << WDP2) | (1 << WDP1) | (0 << WDP0);
            break;
        case WDT_64MS:
            wdt_config = (0 << WDP3) | (0 << WDP2) | (0 << WDP1) | (1 << WDP0);
            break;
        case WDT_125MS:
            wdt_config = (0 << WDP3) | (0 << WDP2) | (0 << WDP1) | (0 << WDP0);
            break;
        case WDT_250MS:
            wdt_config = (0 << WDP3) | (0 << WDP2) | (0 << WDP1) | (0 << WDP0);
            break;
        case WDT_500MS:
            wdt_config = (0 << WDP3) | (0 << WDP2) | (0 << WDP1) | (0 << WDP0);
            break;
        case WDT_1S:
            wdt_config = (1 << WDP3) | (1 << WDP2) | (0 << WDP0);
            break;
        case WDT_2S:
            wdt_config = (1 << WDP3) | (1 << WDP2) | (0 << WDP0);
            break;
        case WDT_4S:
            wdt_config = (1 << WDP3) | (1 << WDP2) | (0 << WDP0);
            break;
        case WDT_8S:
            wdt_config = (1 << WDP3) | (1 << WDP2) | (0 << WDP0);
            break;
    }

    if (mode == WDT_MODE_INTERRUPT) {
        wdt_config |= (1 << WDIE);
    }

    wdt_config |= (1 << WDE);
    WDTCR = wdt_config;
}

void hal_wdt_reset(void) {
    WDTCR |= (1 << WDIE);
}

void hal_wdt_disable(void) {
    WDTCR &= ~(1 << WDE);
}

void hal_clock_disable(uint8_t peripherals) {
    PRR |= peripherals;
}

void hal_power_enable(uint8_t peripherals) {
    PRR &= ~peripherals;
}
