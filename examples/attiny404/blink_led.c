/**
 * @file blink_led.c
 * @brief Blink LED on ATtiny404
 *
 * Blinks an LED connected to PA1 at 1Hz.
 */

#include <avr/io.h>
#include <util/delay.h>
#include "attiny404.h"

int main(void) {
    gpio_init(GPIO_PA1, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_toggle(GPIO_PA1);
        _delay_ms(500);
    }
}
