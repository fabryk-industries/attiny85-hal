/**
 * @file i2c.h
 * @brief USI I2C Master mode for ATtiny85
 *
 * ATtiny85 has a Universal Serial Interface (USI) that provides
 * hardware-assisted I2C master mode with Two-Wire configuration.
 *
 * Implementation:
 * - Uses USI Two-Wire mode (USIWM1=1, USIWM0=0)
 * - Software clock strobe via USITC bit for SCL generation
 * - USI 4-bit counter (USICNT) tracks bit transfers
 * - Automatic counter overflow detection (USIOIF flag)
 *
 * Benefits over bitbanging:
 * - ~40-50% smaller code size
 * - Lower CPU usage during transfers
 * - More consistent timing
 * - Hardware-assisted bit shifting
 *
 * Hardware:
 * - SCL: PB2 (pin 7)
 * - SDA: PB0 (pin 5)
 *
 * Based on: AVR310 Application Note - Using USI module as a TWI Master
 */

#ifndef HAL_USI_I2C_H
#define HAL_USI_I2C_H

#include <stdint.h>

/**
 * @defgroup hal_usi_i2c USI I2C Master
 * @brief USI-assisted I2C master mode
 * @{
 */

/**
 * @brief I2C status
 */
typedef enum {
    I2C_OK,                   ///< Success
    I2C_ERR_NACK,             ///< No acknowledge from slave
    I2C_ERR_BUS_ERROR,       ///< Bus error (arbitration, start, stop)
    I2C_ERR_TIMEOUT,          ///< Timeout waiting for slave
} i2c_status_t;

/**
 * @brief I2C configuration
 */
typedef struct {
    uint8_t sda_pin;
    uint8_t scl_pin;
    uint32_t timeout_us;
} i2c_config_t;

/**
 * @brief I2C handle
 */
typedef struct {
    i2c_config_t config;
} i2c_t;

/**
 * @brief Initialize USI I2C master
 *
 * @param config I2C configuration (SCL, SDA pins, timeout)
 * @return I2C handle
 *
 * @note SCL = PB2, SDA = PB0
 */
i2c_t i2c_init(i2c_config_t config);

/**
 * @brief Start I2C transaction (START condition)
 *
 * @param i2c I2C handle
 * @return I2C status
 */
i2c_status_t i2c_start(i2c_t *i2c);

/**
 * @brief Send I2C STOP condition
 *
 * @param i2c I2C handle
 * @return I2C status
 */
i2c_status_t i2c_stop(i2c_t *i2c);

/**
 * @brief Send byte
 *
 * @param i2c I2C handle
 * @param data Byte to send
 * @return I2C status
 */
i2c_status_t i2c_write_byte(i2c_t *i2c, uint8_t data);

/**
 * @brief Send byte with ACK check
 *
 * @param i2c I2C handle
 * @param data Byte to send
 * @return I2C status
 */
i2c_status_t i2c_write_byte_wait_ack(i2c_t *i2c, uint8_t data);

/**
 * @brief Read byte
 *
 * @param i2c I2C handle
 * @param data Pointer to store received byte
 * @return I2C status
 */
i2c_status_t i2c_read_byte(i2c_t *i2c, uint8_t *data);

/**
 * @brief Send address + read/write flag
 *
 * @param i2c I2C handle
 * @param address Slave address (7-bit)
 * @param read_write 0 for write, 1 for read
 * @return I2C status
 */
i2c_status_t i2c_address(i2c_t *i2c, uint8_t address, uint8_t read_write);

/**
 * @brief Read from register
 *
 * @param i2c I2C handle
 * @param address Slave address (7-bit)
 * @param reg Register address
 * @param data Pointer to store read byte
 * @return I2C status
 */
i2c_status_t i2c_read_reg(i2c_t *i2c, uint8_t address, uint8_t reg, uint8_t *data);

/**
 * @brief Write to register
 *
 * @param i2c I2C handle
 * @param address Slave address (7-bit)
 * @param reg Register address
 * @param data Byte to write
 * @return I2C status
 */
i2c_status_t i2c_write_reg(i2c_t *i2c, uint8_t address, uint8_t reg, uint8_t data);

/** @} */ // end of hal_usi_i2c

#endif // HAL_USI_I2C_H
