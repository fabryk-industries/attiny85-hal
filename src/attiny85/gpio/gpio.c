/**
 * @file gpio.c
 * @brief GPIO implementation for ATtiny85
 */

#include <stdint.h>
#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "attiny85/gpio/gpio.h"

static gpio_pcint_callback_t pcint_callbacks[6] = {0};
static uint8_t pcint_previous_state = 0;

void gpio_init(gpio_pin_t pin, gpio_mode_t mode) {
    uint8_t bit = _BV(pin);

    switch (mode) {
        case GPIO_MODE_INPUT:
            DDRB &= ~bit;
            PORTB &= ~bit;
            break;

        case GPIO_MODE_INPUT_PULLUP:
            DDRB &= ~bit;
            PORTB |= bit;
            break;

        case GPIO_MODE_INPUT_PULLDOWN:
            DDRB &= ~bit;
            PORTB &= ~bit;
            break;

        case GPIO_MODE_OUTPUT:
            DDRB |= bit;
            PORTB &= ~bit;
            break;

        case GPIO_MODE_ANALOG:
            DDRB &= ~bit;
            PORTB &= ~bit;
            DIDR0 |= bit;
            break;

        default:
            break;
    }
}

void gpio_set_output(gpio_pin_t pin) {
    DDRB |= _BV(pin);
}

void gpio_set_input(gpio_pin_t pin) {
    uint8_t bit = _BV(pin);
    DDRB &= ~bit;
    PORTB &= ~bit;
    DIDR0 &= ~bit;
}

void gpio_set_input_pullup(gpio_pin_t pin) {
    uint8_t bit = _BV(pin);
    DDRB &= ~bit;
    PORTB |= bit;
    DIDR0 &= ~bit;
}

void gpio_set_analog(gpio_pin_t pin) {
    uint8_t bit = _BV(pin);
    DDRB &= ~bit;
    PORTB &= ~bit;
    DIDR0 |= bit;
}

void gpio_set_high(gpio_pin_t pin) {
    PORTB |= _BV(pin);
}

void gpio_set_low(gpio_pin_t pin) {
    PORTB &= ~_BV(pin);
}

void gpio_toggle(gpio_pin_t pin) {
    PINB = _BV(pin);
}

void gpio_write(gpio_pin_t pin, gpio_level_t level) {
    if (level == GPIO_HIGH) {
        PORTB |= _BV(pin);
    } else {
        PORTB &= ~_BV(pin);
    }
}

gpio_level_t gpio_read(gpio_pin_t pin) {
    if (PINB & _BV(pin)) {
        return GPIO_HIGH;
    }
    return GPIO_LOW;
}

void gpio_enable_pcint(gpio_pin_t pin, gpio_pcint_callback_t callback) {
    uint8_t bit = _BV(pin);
    uint8_t sreg = SREG;

    cli();
    pcint_callbacks[pin] = callback;
    PCMSK |= bit;
    GIMSK |= _BV(PCIE);
    pcint_previous_state = PINB;

    SREG = sreg;
}

void gpio_disable_pcint(gpio_pin_t pin) {
    uint8_t bit = _BV(pin);
    uint8_t sreg = SREG;

    cli();
    pcint_callbacks[pin] = NULL;
    PCMSK &= ~bit;

    if (PCMSK == 0) {
        GIMSK &= ~_BV(PCIE);
    }

    SREG = sreg;
}

void gpio_get_pcint_changed(uint8_t changed[6]) {
    uint8_t current = PINB;
    uint8_t changed_mask = current ^ pcint_previous_state;

    for (gpio_pin_t pin = GPIO_PB0; pin <= GPIO_PB5; pin++) {
        changed[pin] = (changed_mask & _BV(pin)) ? 1 : 0;
    }

    pcint_previous_state = current;
}

void gpio_pcint_handler(void) {
    uint8_t current = PINB;
    uint8_t changed_mask = current ^ pcint_previous_state;

    for (gpio_pin_t pin = GPIO_PB0; pin <= GPIO_PB5; pin++) {
        if (changed_mask & _BV(pin) && pcint_callbacks[pin]) {
            pcint_callbacks[pin](pin);
        }
    }

    pcint_previous_state = current;
}


ISR(PCINT0_vect) {
    gpio_pcint_handler();
}

