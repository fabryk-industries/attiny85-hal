#include <stdint.h>
#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "gpio/gpio.h"

void gpio_get_port_info(gpio_pin_t pin, volatile uint8_t **port,
                        volatile uint8_t **dir, uint8_t *pin_mask) {
    uint8_t pin_num = pin & 0x07;
    uint8_t port_idx = pin >> 3;

    *pin_mask = _BV(pin_num);

    switch (port_idx) {
        case 0:
            *port = &VPORTA.OUT;
            *dir = &VPORTA.DIR;
            break;
        case 1:
            *port = &VPORTB.OUT;
            *dir = &VPORTB.DIR;
            break;
        default:
            *port = NULL;
            *dir = NULL;
            break;
    }
}

void gpio_init(gpio_pin_t pin, gpio_mode_t mode) {
    volatile uint8_t *port = NULL;
    volatile uint8_t *dir = NULL;
    uint8_t bit;

    gpio_get_port_info(pin, &port, &dir, &bit);

    if (port == NULL || dir == NULL) {
        return;
    }

    switch (mode) {
        case GPIO_MODE_INPUT:
            *dir &= ~bit;
            *port &= ~bit;
            break;

        case GPIO_MODE_INPUT_PULLUP:
            *dir &= ~bit;
            *port |= bit;
            break;

        case GPIO_MODE_OUTPUT:
            *dir |= bit;
            *port &= ~bit;
            break;

        case GPIO_MODE_ANALOG:
            *dir &= ~bit;
            *port &= ~bit;
            if (pin < 8) {
                PORTA.PIN0CTRL |= PORT_PULLUPEN_bm;
            } else {
                PORTB.PIN0CTRL |= PORT_PULLUPEN_bm;
            }
            break;

        default:
            break;
    }
}

void gpio_set_output(gpio_pin_t pin) {
    volatile uint8_t *dir = NULL;
    uint8_t bit;
    volatile uint8_t *dummy_port;

    gpio_get_port_info(pin, &dummy_port, &dir, &bit);

    if (dir != NULL) {
        *dir |= bit;
    }
}

void gpio_set_input(gpio_pin_t pin) {
    volatile uint8_t *port = NULL;
    volatile uint8_t *dir = NULL;
    uint8_t bit;

    gpio_get_port_info(pin, &port, &dir, &bit);

    if (port != NULL && dir != NULL) {
        *dir &= ~bit;
        *port &= ~bit;
    }
}

void gpio_set_input_pullup(gpio_pin_t pin) {
    volatile uint8_t *port = NULL;
    volatile uint8_t *dir = NULL;
    uint8_t bit;

    gpio_get_port_info(pin, &port, &dir, &bit);

    if (port != NULL && dir != NULL) {
        *dir &= ~bit;
        *port |= bit;
    }
}

void gpio_set_analog(gpio_pin_t pin) {
    volatile uint8_t *port = NULL;
    volatile uint8_t *dir = NULL;
    uint8_t bit;

    gpio_get_port_info(pin, &port, &dir, &bit);

    if (port != NULL && dir != NULL) {
        *dir &= ~bit;
        *port &= ~bit;

        uint8_t pin_num = pin & 0x07;
        if (pin < 8) {
            (&PORTA.PIN0CTRL)[pin_num] |= PORT_ISC_INPUT_DISABLE_gc;
        } else {
            (&PORTB.PIN0CTRL)[pin_num] |= PORT_ISC_INPUT_DISABLE_gc;
        }
    }
}

void gpio_set_high(gpio_pin_t pin) {
    volatile uint8_t *port = NULL;
    uint8_t bit;
    volatile uint8_t *dummy_dir;

    gpio_get_port_info(pin, &port, &dummy_dir, &bit);

    if (port != NULL) {
        *port |= bit;
    }
}

void gpio_set_low(gpio_pin_t pin) {
    volatile uint8_t *port = NULL;
    uint8_t bit;
    volatile uint8_t *dummy_dir;

    gpio_get_port_info(pin, &port, &dummy_dir, &bit);

    if (port != NULL) {
        *port &= ~bit;
    }
}

void gpio_toggle(gpio_pin_t pin) {
    volatile uint8_t *port = NULL;
    uint8_t bit;
    volatile uint8_t *dummy_dir;

    gpio_get_port_info(pin, &port, &dummy_dir, &bit);

    if (port != NULL) {
        *port ^= bit;
    }
}

void gpio_write(gpio_pin_t pin, gpio_level_t level) {
    volatile uint8_t *port = NULL;
    uint8_t bit;
    volatile uint8_t *dummy_dir;

    gpio_get_port_info(pin, &port, &dummy_dir, &bit);

    if (port != NULL) {
        if (level == GPIO_HIGH) {
            *port |= bit;
        } else {
            *port &= ~bit;
        }
    }
}

gpio_level_t gpio_read(gpio_pin_t pin) {
    volatile uint8_t *port = NULL;
    uint8_t bit;
    volatile uint8_t *dummy_dir;

    gpio_get_port_info(pin, &port, &dummy_dir, &bit);

    if (port == NULL) {
        return GPIO_LOW;
    }

    return (*port & bit) ? GPIO_HIGH : GPIO_LOW;
}
