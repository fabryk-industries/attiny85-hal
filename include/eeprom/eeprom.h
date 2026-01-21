/**
 * @file eeprom.h
 * @brief EEPROM abstraction for ATtiny85
 *
 * Provides type-safe wrapper around avr-libc's EEPROM functions.
 */

#ifndef HAL_EEPROM_H
#define HAL_EEPROM_H

#include <stdint.h>

/**
 * @defgroup hal_eeprom EEPROM
 * @brief EEPROM read/write operations
 * @{
 */

/**
 * @brief Read byte from EEPROM
 *
 * Reads a single byte from EEPROM address.
 *
 * @param addr EEPROM address (0-511 for ATtiny85)
 * @return Byte value read from EEPROM
 */
uint8_t hal_eeprom_read_byte(uint16_t addr);

/**
 * @brief Write byte to EEPROM
 *
 * Writes a single byte to EEPROM address.
 *
 * @param addr EEPROM address (0-511 for ATtiny85)
 * @param data Byte value to write
 */
void hal_eeprom_write_byte(uint16_t addr, uint8_t data);

/**
 * @brief Update byte in EEPROM
 *
 * Reads byte and only writes if value differs.
 * More efficient for unchanged data (reduces wear).
 *
 * @param addr EEPROM address (0-511 for ATtiny85)
 * @param data Byte value to write
 */
void hal_eeprom_update_byte(uint16_t addr, uint8_t data);

/**
 * @brief Read block from EEPROM
 *
 * Reads multiple bytes from EEPROM.
 *
 * @param dst Destination buffer
 * @param src EEPROM source address
 * @param len Number of bytes to read
 */
void hal_eeprom_read_block(void *dst, uint16_t src, uint16_t len);

/**
 * @brief Write block to EEPROM
 *
 * Writes multiple bytes to EEPROM.
 *
 * @param src Source buffer
 * @param dst EEPROM destination address
 * @param len Number of bytes to write
 */
void hal_eeprom_write_block(const void *src, uint16_t dst, uint16_t len);

/**
 * @brief Update block in EEPROM
 *
 * Reads block and only writes if values differ.
 *
 * @param src Source buffer
 * @param dst EEPROM destination address
 * @param len Number of bytes to update
 */
void hal_eeprom_update_block(const void *src, uint16_t dst, uint16_t len);

/** @} */ // end of hal_eeprom

#endif // HAL_EEPROM_H
