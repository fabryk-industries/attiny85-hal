/**
 * @file spi.h
 * @brief USI SPI Master mode for ATtiny85
 *
 * ATtiny85 has a Universal Serial Interface (USI) that provides
 * hardware-assisted SPI master mode with Three-Wire configuration.
 *
 * Implementation:
 * - Uses USI Three-Wire mode (USIWM0=1, USIWM1=0)
 * - Software clock strobe via USITC bit for SCK generation
 * - USI 4-bit counter (USICNT) tracks clock edges
 * - Automatic counter overflow detection (USIOIF flag)
 * - Atomic block for consistent timing during transfers
 *
 * Benefits over bitbanging:
 * - ~40-50% smaller code size
 * - Lower CPU usage during transfers
 * - More consistent clock timing
 * - Hardware-assisted bit shifting
 *
 * Hardware:
 * - MOSI: PB0 (pin 5) - USI DO
 * - MISO: PB1 (pin 6) - USI DI
 * - SCK:  PB2 (pin 7) - USI USCK
 *
 * Note: No dedicated SS pin - must be implemented in software
 *
 * Based on: AVR319 Application Note - Using USI for SPI Communication
 */

#ifndef HAL_USI_SPI_H
#define HAL_USI_SPI_H

#include <stdint.h>

/**
 * @defgroup hal_usi_spi USI SPI Master
 * @brief USI-assisted SPI master mode
 * @{
 */

/**
 * @brief SPI mode
 */
typedef enum {
    SPI_MODE_0,    ///< CPOL=0, CPHA=0 (sample on rising edge, shift on falling)
    SPI_MODE_1,    ///< CPOL=0, CPHA=1 (sample on falling edge, shift on rising)
    SPI_MODE_2,    ///< CPOL=1, CPHA=0 (sample on rising edge, shift on falling)
    SPI_MODE_3,    ///< CPOL=1, CPHA=1 (sample on falling edge, shift on rising)
} spi_mode_t;

/**
 * @brief SPI bit order
 */
typedef enum {
    SPI_BIT_ORDER_MSB_FIRST,    ///< MSB transmitted first
    SPI_BIT_ORDER_LSB_FIRST,    ///< LSB transmitted first
} spi_bit_order_t;

/**
 * @brief SPI configuration
 */
typedef struct {
    uint8_t sclk_pin;
    uint8_t mosi_pin;
    uint8_t miso_pin;
    spi_mode_t mode;
    spi_bit_order_t bit_order;
} spi_config_t;

/**
 * @brief SPI handle
 */
typedef struct {
    spi_config_t config;
} spi_t;

/**
 * @brief Initialize USI SPI master
 *
 * Configures USI for SPI master mode with specified pins and mode.
 *
 * @param config SPI configuration (pins, mode, bit order)
 * @return SPI handle
 *
 * @note MOSI = USI DO (PB0), MISO = USI DI (PB1), SCK = USI SCK (PB2)
 */
spi_t spi_init(spi_config_t config);

/**
 * @brief Transfer single byte
 *
 * Transmits and receives one byte over SPI.
 *
 * @param spi SPI handle
 * @param data Byte to transmit
 * @return Byte received
 */
uint8_t spi_transfer(spi_t *spi, uint8_t data);

/**
 * @brief Transfer buffer
 *
 * Transmits buffer and receives response.
 *
 * @param spi SPI handle
 * @param tx Buffer to transmit
 * @param rx Buffer to receive (can be NULL for write-only)
 * @param len Number of bytes
 */
void spi_transfer_buf(spi_t *spi, const uint8_t *tx, uint8_t *rx, uint16_t len);

/**
 * @brief Write bytes only (no read)
 *
 * @param spi SPI handle
 * @param data Buffer to write
 * @param len Number of bytes
 */
void spi_write(spi_t *spi, const uint8_t *data, uint16_t len);

/** @} */ // end of hal_usi_spi

#endif // HAL_USI_SPI_H
