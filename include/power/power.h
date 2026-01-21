/**
 * @file power.h
 * @brief Power management abstraction for ATtiny85
 *
 * Provides sleep modes, watchdog timer, and power reduction control.
 */

#ifndef HAL_POWER_H
#define HAL_POWER_H

#include <stdint.h>

/**
 * @defgroup hal_power Power Management
 * @brief Sleep modes, watchdog, and power reduction
 * @{
 */

/**
 * @brief Sleep mode
 */
typedef enum {
    SLEEP_IDLE,                  ///< Idle mode (CPU stopped, peripherals running)
    SLEEP_ADC_NOISE_REDUCTION,   ///< ADC noise reduction mode
    SLEEP_POWER_DOWN,            ///< Deepest sleep mode (~0.1 μA @ 1.8V)
} sleep_mode_t;

/**
 * @brief Watchdog timeout
 */
typedef enum {
    WDT_16MS,
    WDT_32MS,
    WDT_64MS,
    WDT_125MS,
    WDT_250MS,
    WDT_500MS,
    WDT_1S,
    WDT_2S,
    WDT_4S,
    WDT_8S,
} wdt_timeout_t;

/**
 * @brief Watchdog mode
 */
typedef enum {
    WDT_MODE_INTERRUPT,    ///< Interrupt on timeout
    WDT_MODE_RESET,        ///< System reset on timeout
} wdt_mode_t;

/** @} */ // end of hal_power


/**
 * @brief Enable sleep mode
 *
 * Configures and enables the specified sleep mode.
 *
 * @param mode Sleep mode to enable
 */
void hal_sleep_enable(sleep_mode_t mode);

/**
 * @brief Enter sleep mode immediately
 *
 * Enters the currently configured sleep mode.
 */
void hal_sleep_now(void);

/**
 * @brief Enable watchdog timer
 *
 * Configures and enables the watchdog timer with specified timeout and mode.
 *
 * @param timeout Watchdog timeout duration
 * @param mode Watchdog mode (interrupt or reset)
 */
void hal_wdt_enable(wdt_timeout_t timeout, wdt_mode_t mode);

/**
 * @brief Reset watchdog timer
 *
 * Resets the watchdog timer counter.
 */
void hal_wdt_reset(void);

/**
 * @brief Disable watchdog timer
 *
 * Disables the watchdog timer completely.
 */
void hal_wdt_disable(void);

/**
 * @brief Disable clock for specified peripherals
 *
 * Disables clock to reduce power consumption.
 *
 * @param peripherals Bitmask of peripherals to disable
 */
void hal_clock_disable(uint8_t peripherals);

/**
 * @brief Enable clock for specified peripherals
 *
 * Enables clock for previously disabled peripherals.
 *
 * @param peripherals Bitmask of peripherals to enable
 */
void hal_power_enable(uint8_t peripherals);

/** @} */ // end of hal_power

#endif // HAL_POWER_H
