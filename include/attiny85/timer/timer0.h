/**
 * @file timer0.h
 * @brief Timer0 abstraction for ATtiny85 (PWM and delays)
 *
 * Timer0 is an 8-bit timer with two PWM channels (OC0A on PB0, OC0B on PB1).
 * Provides fast PWM generation and simple blocking delays.
 */

#ifndef HAL_TIMER0_H
#define HAL_TIMER0_H

#include <stdint.h>
#include <avr/io.h>

/**
 * @defgroup hal_timer0 Timer0
 * @brief 8-bit timer with PWM and delay functions
 * @{
 */

/**
 * @brief Timer0 prescaler values
 */
typedef enum {
    TIMER0_PRESCALER_1 = 0x01,      ///< No prescaling (F_CPU)
    TIMER0_PRESCALER_8 = 0x02,      ///< Divide by 8
    TIMER0_PRESCALER_64 = 0x03,     ///< Divide by 64
    TIMER0_PRESCALER_256 = 0x04,    ///< Divide by 256
    TIMER0_PRESCALER_1024 = 0x05,    ///< Divide by 1024
} timer0_prescaler_t;

/**
 * @brief Timer0 PWM mode
 */
typedef enum {
    TIMER0_PWM_FAST,                 ///< Fast PWM (0-255, non-inverting by default)
    TIMER0_PWM_PHASE_CORRECT,         ///< Phase-correct PWM (0-255)
} timer0_pwm_mode_t;

/**
 * @brief PWM channel identifier
 */
typedef enum {
    TIMER0_PWM_CHANNEL_A,            ///< OC0A on PB0
    TIMER0_PWM_CHANNEL_B,            ///< OC0B on PB1
} timer0_pwm_channel_t;

/**
 * @brief Timer0 configuration structure
 */
typedef struct {
    timer0_pwm_mode_t mode;
    timer0_prescaler_t prescaler;
} timer0_config_t;

/**
 * @brief PWM channel handle
 */
typedef struct {
    timer0_pwm_channel_t channel;
} timer0_pwm_t;

/**
 * @brief Initialize Timer0 for PWM output
 *
 * Configures Timer0 for PWM generation on the specified channel.
 *
 * @param config Timer0 configuration (mode and prescaler)
 * @param channel PWM channel (A or B)
 *
 * @note PWM frequency = F_CPU / (prescaler * 256)
 * @example
 * @code
 * timer0_config_t config = {
 *     .mode = TIMER0_PWM_FAST,
 *     .prescaler = TIMER0_PRESCALER_64
 * };
 * timer0_pwm_t pwm = timer0_pwm_init(config, TIMER0_PWM_CHANNEL_A);
 * @endcode
 */
timer0_pwm_t timer0_pwm_init(timer0_config_t config, timer0_pwm_channel_t channel);

/**
 * @brief Set PWM duty cycle
 *
 * Sets the duty cycle for the PWM channel.
 *
 * @param pwm PWM channel handle
 * @param duty Duty cycle (0-255, where 255 = 100%, 0 = 0%)
 *
 * @note Duty cycle is 8-bit (0-255) for Timer0
 */
void timer0_pwm_set_duty(timer0_pwm_t pwm, uint8_t duty);

/**
 * @brief Get PWM duty cycle
 *
 * Returns the current duty cycle for the PWM channel.
 *
 * @param pwm PWM channel handle
 * @return Current duty cycle (0-255)
 */
uint8_t timer0_pwm_get_duty(timer0_pwm_t pwm);

/**
 * @brief Disable PWM output on a channel
 *
 * Disables PWM output and sets the pin to GPIO mode.
 *
 * @param pwm PWM channel handle
 */
void timer0_pwm_disable(timer0_pwm_t pwm);

/**
 * @brief Busy-wait delay in milliseconds
 *
 * Blocks execution for the specified number of milliseconds.
 * Uses Timer0 or busy-wait loop depending on implementation.
 *
 * @param ms Delay time in milliseconds (0-65535)
 *
 * @note This is a blocking delay - interrupts may affect accuracy
 * @note Requires F_CPU to be defined
 * @warning Maximum delay is ~65 seconds at 1MHz clock
 */
void delay_ms(uint16_t ms);

/**
 * @brief Busy-wait delay in microseconds
 *
 * Blocks execution for the specified number of microseconds.
 * Uses busy-wait loop for precise short delays.
 *
 * @param us Delay time in microseconds (0-65535)
 *
 * @note This is a blocking delay - interrupts may affect accuracy
 * @note Requires F_CPU to be defined
 * @warning Maximum delay is ~65 milliseconds at 1MHz clock
 */
void delay_us(uint16_t us);

/** @} */ // end of hal_timer0

#endif // HAL_TIMER0_H
