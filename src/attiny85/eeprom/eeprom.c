/**
 * @file eeprom.c
 * @brief EEPROM implementation for ATtiny85
 */

#include <stdint.h>
#include <avr/eeprom.h>
#include "attiny85/eeprom/eeprom.h"

uint8_t hal_eeprom_read_byte(uint16_t addr) {
    return eeprom_read_byte((const uint8_t *)addr);
}

void hal_eeprom_write_byte(uint16_t addr, uint8_t data) {
    eeprom_write_byte((uint8_t *)addr, data);
}

void hal_eeprom_update_byte(uint16_t addr, uint8_t data) {
    eeprom_update_byte((uint8_t *)addr, data);
}

void hal_eeprom_read_block(void *dst, uint16_t src, uint16_t len) {
    eeprom_read_block(dst, (const uint8_t *)src, len);
}

void hal_eeprom_write_block(const void *src, uint16_t dst, uint16_t len) {
    eeprom_write_block((const uint8_t *)src, (uint8_t *)dst, len);
}

void hal_eeprom_update_block(const void *src, uint16_t dst, uint16_t len) {
    eeprom_update_block((const uint8_t *)src, (uint8_t *)dst, len);
}
