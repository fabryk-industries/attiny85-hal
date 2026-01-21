/**
 * @file timer0.c
 * @brief Timer0 implementation for ATtiny85
 */

#include "timer/timer0.h"
#include <avr/io.h>
#include <util/delay.h>

static uint8_t duty_cycles[2] = {0, 0};

timer0_pwm_t timer0_pwm_init(timer0_config_t config, timer0_pwm_channel_t channel) {
    uint8_t wgm_mask = 0;
    uint8_t prescaler_bits = 0;

    if (config.mode == TIMER0_PWM_FAST) {
        wgm_mask = (1 << WGM01) | (1 << WGM00);
    } else {
        wgm_mask = (1 << WGM00);
    }

    prescaler_bits = config.prescaler;

    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | wgm_mask;
    TCCR0B = prescaler_bits;

    duty_cycles[channel] = 0;

    timer0_pwm_t pwm = { .channel = channel };
    return pwm;
}

void timer0_pwm_set_duty(timer0_pwm_t pwm, uint8_t duty) {
    duty_cycles[pwm.channel] = duty;

    if (pwm.channel == TIMER0_PWM_CHANNEL_A) {
        OCR0A = duty;
    } else {
        OCR0B = duty;
    }
}

uint8_t timer0_pwm_get_duty(timer0_pwm_t pwm) {
    return duty_cycles[pwm.channel];
}

void timer0_pwm_disable(timer0_pwm_t pwm) {
    if (pwm.channel == TIMER0_PWM_CHANNEL_A) {
        TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0));
    } else {
        TCCR0A &= ~((1 << COM0B1) | (1 << COM0B0));
    }

    duty_cycles[pwm.channel] = 0;
}

void delay_ms(uint16_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

void delay_us(uint16_t us) {
    while (us--) {
        _delay_us(1);
    }
}
