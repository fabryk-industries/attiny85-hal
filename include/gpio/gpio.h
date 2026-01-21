/**
 * @file gpio.h
 * @brief GPIO abstraction for ATtiny85
 *
 * Provides type-safe GPIO operations with compile-time pin selection
 * and mode configuration for the ATtiny85's single PORTB.
 */

#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>
#include <avr/io.h>

/**
 * @defgroup hal_gpio GPIO (General Purpose I/O)
 * @brief Digital I/O abstraction for ATtiny85 PORTB
 * @{
 */

/**
 * @brief Pin identifier for PORTB
 *
 * ATtiny85 has only PORTB with 6 pins (PB0-PB5).
 * Each pin can be used as GPIO or has alternate functions.
 */
typedef enum {
    GPIO_PB0 = 0,    ///< PB0 - MOSI/DI/SDA/AIN0/OC0A/OC1A/AREF/PCINT0
    GPIO_PB1 = 1,    ///< PB1 - MISO/DO/AIN1/OC0B/OC1A/PCINT1
    GPIO_PB2 = 2,    ///< PB2 - SCK/USCK/SCL/ADC1/T0/INT0/PCINT2
    GPIO_PB3 = 3,    ///< PB3 - XTAL1/CLKI/OC1B/ADC3/PCINT3
    GPIO_PB4 = 4,    ///< PB4 - XTAL2/CLKO/OC1B/ADC2/PCINT4
    GPIO_PB5 = 5,    ///< PB5 - RESET/ADC0/dW/PCINT5 (may be disabled as GPIO)
} gpio_pin_t;

/**
 * @brief GPIO mode configuration
 *
 * Defines the electrical behavior and direction of a GPIO pin.
 */
typedef enum {
    GPIO_MODE_INPUT,          ///< Input with high impedance (floating)
    GPIO_MODE_INPUT_PULLUP,  ///< Input with internal pull-up resistor enabled
    GPIO_MODE_INPUT_PULLDOWN, ///< Input mode - requires external pull-down (ATtiny85 has no internal pull-down)
    GPIO_MODE_OUTPUT,         ///< Output push-pull
    GPIO_MODE_ANALOG,         ///< Analog input (disables digital input buffer)
} gpio_mode_t;

/**
 * @brief GPIO logic level
 */
typedef enum {
    GPIO_LOW = 0,    ///< Logic low (0V)
    GPIO_HIGH = 1,    ///< Logic high (VCC)
} gpio_level_t;

/**
 * @brief GPIO pin change interrupt trigger
 */
typedef enum {
    GPIO_PCINT_DISABLED,   ///< Pin change interrupt disabled
    GPIO_PCINT_ANY,       ///< Trigger on any level change (rising or falling edge)
} gpio_pcint_mode_t;

/**
 * @brief Pin change interrupt callback function type
 *
 * @param pin The pin that triggered the interrupt
 */
typedef void (*gpio_pcint_callback_t)(gpio_pin_t pin);

/**
 * @brief Initialize a GPIO pin with specified mode
 *
 * Configures the pin's direction and pull-up settings.
 *
 * @param pin Pin identifier (GPIO_PB0 through GPIO_PB5)
 * @param mode Pin mode (input/output/analog with optional pull-up)
 *
 * @note PB5 is RESET pin by default - may need fuse changes to use as GPIO
 * @note Analog mode disables digital input buffer to save power
 */
void gpio_init(gpio_pin_t pin, gpio_mode_t mode);

/**
 * @brief Set pin direction to output
 *
 * Changes an already-initialized pin to output mode.
 * Output state is LOW by default after this call.
 *
 * @param pin Pin identifier
 */
void gpio_set_output(gpio_pin_t pin);

/**
 * @brief Set pin direction to input (floating)
 *
 * Changes an already-initialized pin to input mode without pull-up.
 *
 * @param pin Pin identifier
 */
void gpio_set_input(gpio_pin_t pin);

/**
 * @brief Set pin direction to input with pull-up
 *
 * Changes an already-initialized pin to input mode with internal pull-up enabled.
 *
 * @param pin Pin identifier
 */
void gpio_set_input_pullup(gpio_pin_t pin);

/**
 * @brief Set pin to analog mode
 *
 * Disables digital input buffer and enables ADC functionality.
 * Pin must be connected to ADC channel or external analog circuit.
 *
 * @param pin Pin identifier
 *
 * @note Only PB0, PB2, PB3, PB4, PB5 support ADC
 * @note Digital read will return unpredictable values in analog mode
 */
void gpio_set_analog(gpio_pin_t pin);

/**
 * @brief Set output pin to HIGH level
 *
 * Drives the pin to VCC. Pin must be configured as output.
 *
 * @param pin Pin identifier
 */
void gpio_set_high(gpio_pin_t pin);

/**
 * @brief Set output pin to LOW level
 *
 * Drives the pin to GND. Pin must be configured as output.
 *
 * @param pin Pin identifier
 */
void gpio_set_low(gpio_pin_t pin);

/**
 * @brief Toggle output pin level
 *
 * Flips the pin state (HIGH->LOW or LOW->HIGH).
 * Pin must be configured as output.
 *
 * @param pin Pin identifier
 *
 * @note This is an atomic single-cycle instruction on AVR
 */
void gpio_toggle(gpio_pin_t pin);

/**
 * @brief Write a logic level to an output pin
 *
 * Sets the pin to HIGH or LOW based on the level parameter.
 * Pin must be configured as output.
 *
 * @param pin Pin identifier
 * @param level Logic level (GPIO_HIGH or GPIO_LOW)
 */
void gpio_write(gpio_pin_t pin, gpio_level_t level);

/**
 * @brief Read logic level from an input pin
 *
 * Returns the current logic level on the pin.
 * Pin must be configured as input.
 *
 * @param pin Pin identifier
 * @return Logic level (GPIO_LOW or GPIO_HIGH)
 *
 * @note For input pins without pull-up, floating pins may return random values
 */
gpio_level_t gpio_read(gpio_pin_t pin);

/**
 * @brief Check if pin is currently HIGH
 *
 * Convenience function that returns true if pin reads HIGH.
 *
 * @param pin Pin identifier
 * @return Non-zero if HIGH, zero if LOW
 */
static inline uint8_t gpio_is_high(gpio_pin_t pin) {
    return gpio_read(pin) == GPIO_HIGH;
}

/**
 * @brief Check if pin is currently LOW
 *
 * Convenience function that returns true if pin reads LOW.
 *
 * @param pin Pin identifier
 * @return Non-zero if LOW, zero if HIGH
 */
static inline uint8_t gpio_is_low(gpio_pin_t pin) {
    return gpio_read(pin) == GPIO_LOW;
}

/**
 * @brief Enable pin change interrupt for a pin
 *
 * Enables the pin change interrupt (PCINT) for the specified pin.
 * Any pin change (rising or falling edge) will trigger the PCINT0 interrupt.
 *
 * @param pin Pin identifier
 * @param callback Optional callback function (NULL for interrupt only)
 *
 * @note All PB pins share a single PCINT0 interrupt vector
 * @note The callback will be called from ISR context - keep it short
 * @note To identify which pin changed, use PCMSK to check enabled pins
 */
void gpio_enable_pcint(gpio_pin_t pin, gpio_pcint_callback_t callback);

/**
 * @brief Disable pin change interrupt for a pin
 *
 * Disables the pin change interrupt for the specified pin.
 *
 * @param pin Pin identifier
 */
void gpio_disable_pcint(gpio_pin_t pin);

/**
 * @brief Check which pins changed (call from PCINT0 ISR)
 *
 * Compares current pin states with previous states to identify
 * which pins triggered the interrupt. Should only be called
 * from within the PCINT0 interrupt service routine.
 *
 * @param[out] changed Array of 6 booleans indicating which pins changed
 *
 * @example
 * ISR(PCINT0_vect) {
 *     uint8_t changed[6];
 *     gpio_get_pcint_changed(changed);
 *
 *     for (gpio_pin_t pin = GPIO_PB0; pin <= GPIO_PB5; pin++) {
 *         if (changed[pin]) {
 *             // Handle pin change
 *         }
 *     }
 * }
 */
void gpio_get_pcint_changed(uint8_t changed[6]);

/**
 * @brief Global PCINT0 interrupt handler
 *
 * This function is called from the PCINT0 ISR when any enabled
 * pin change occurs. It calls registered callbacks for each pin.
 *
 * @note Automatically called by ISR if callbacks are registered
 * @note Users should implement gpio_pcint_callback_t for custom behavior
 */
void gpio_pcint_handler(void);

/** @} */ // end of hal_gpio

#endif // HAL_GPIO_H
