/**
 * @file gpio.h
 * @brief GPIO abstraction for ATtiny404
 *
 * Provides type-safe GPIO operations for ATtiny404's PORTA (8 pins) and PORTB (4 pins).
 * Uses DIRA/DIRB for direction (not DDR like older AVRs).
 */

#ifndef HAL_GPIO404_H
#define HAL_GPIO404_H

#include <stdint.h>
#include <avr/io.h>

/**
 * @defgroup hal_gpio404 GPIO (General Purpose I/O)
 * @brief Digital I/O abstraction for ATtiny404 PORTA/PORTB
 * @{
 */

/**
 * @brief Port identifier
 */
typedef enum {
    GPIO_PORT_A,    ///< PORTA (PA0-PA7)
    GPIO_PORT_B,    ///< PORTB (PB0-PB3)
} gpio_port_t;

/**
 * @brief Pin identifier combining port and pin number
 */
typedef enum {
    // PORTA pins (PA0-PA7)
    GPIO_PA0 = 0,    ///< PA0 - AIN0/USART0 TX/TWI0 SCL/SPI0 SCK
    GPIO_PA1 = 1,    ///< PA1 - AIN1/USART0 RX/TWI0 SDA/MOSI
    GPIO_PA2 = 2,    ///< PA2 - AIN2/USART0 XCK/SPI0 MISO
    GPIO_PA3 = 3,    ///< PA3 - AIN3/TCA0 WO0/RTC XIN
    GPIO_PA4 = 4,    ///< PA4 - AIN4/TCA0 WO1/RTC XOUT
    GPIO_PA5 = 5,    ///< PA5 - AIN5/TCA0 WO2
    GPIO_PA6 = 6,    ///< PA6 - AIN6/TCA0 WO3
    GPIO_PA7 = 7,    ///< PA7 - AIN7/TCA0 WO4

    // PORTB pins (PB0-PB3)
    GPIO_PB0 = 8,    ///< PB0 - TDI/UPDI
    GPIO_PB1 = 9,    ///< PB1 - TDO/TCA0 WO5
    GPIO_PB2 = 10,   ///< PB2 - TMS/SPI0 SS
    GPIO_PB3 = 11,   ///< PB3 - TCK/SPI0 MOSI
} gpio_pin_t;

/**
 * @brief GPIO mode configuration
 */
typedef enum {
    GPIO_MODE_INPUT,          ///< Input with high impedance (floating)
    GPIO_MODE_INPUT_PULLUP,  ///< Input with internal pull-up resistor enabled
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
    GPIO_PCINT_RISING,    ///< Trigger on rising edge
    GPIO_PCINT_FALLING,   ///< Trigger on falling edge
    GPIO_PCINT_ANY,       ///< Trigger on any level change
} gpio_pcint_mode_t;

/**
 * @brief Pin change interrupt callback function type
 */
typedef void (*gpio_pcint_callback_t)(gpio_pin_t pin);

/**
 * @brief Initialize a GPIO pin with specified mode
 *
 * @param pin Pin identifier (GPIO_PA0 through GPIO_PB3)
 * @param mode Pin mode (input/output/analog with optional pull-up)
 */
void gpio_init(gpio_pin_t pin, gpio_mode_t mode);

/**
 * @brief Set pin direction to output
 *
 * @param pin Pin identifier
 */
void gpio_set_output(gpio_pin_t pin);

/**
 * @brief Set pin direction to input (floating)
 *
 * @param pin Pin identifier
 */
void gpio_set_input(gpio_pin_t pin);

/**
 * @brief Set pin direction to input with pull-up
 *
 * @param pin Pin identifier
 */
void gpio_set_input_pullup(gpio_pin_t pin);

/**
 * @brief Set pin to analog mode
 *
 * @param pin Pin identifier
 */
void gpio_set_analog(gpio_pin_t pin);

/**
 * @brief Set output pin to HIGH level
 *
 * @param pin Pin identifier
 */
void gpio_set_high(gpio_pin_t pin);

/**
 * @brief Set output pin to LOW level
 *
 * @param pin Pin identifier
 */
void gpio_set_low(gpio_pin_t pin);

/**
 * @brief Toggle output pin level
 *
 * @param pin Pin identifier
 */
void gpio_toggle(gpio_pin_t pin);

/**
 * @brief Write a logic level to an output pin
 *
 * @param pin Pin identifier
 * @param level Logic level (GPIO_HIGH or GPIO_LOW)
 */
void gpio_write(gpio_pin_t pin, gpio_level_t level);

/**
 * @brief Read logic level from an input pin
 *
 * @param pin Pin identifier
 * @return Logic level (GPIO_LOW or GPIO_HIGH)
 */
gpio_level_t gpio_read(gpio_pin_t pin);

/**
 * @brief Check if pin is currently HIGH
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
 * @param pin Pin identifier
 * @return Non-zero if LOW, zero if HIGH
 */
static inline uint8_t gpio_is_low(gpio_pin_t pin) {
    return gpio_read(pin) == GPIO_LOW;
}

/**
 * @brief Get port and pin from pin identifier
 *
 * @param pin Pin identifier
 * @param port Output pointer to port register address
 * @param dir Output pointer to direction register address
 * @param pin_mask Output pointer to pin mask bit
 */
void gpio_get_port_info(gpio_pin_t pin, volatile uint8_t **port,
                        volatile uint8_t **dir, uint8_t *pin_mask);

/** @} */

#endif
