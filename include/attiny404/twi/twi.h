#ifndef HAL_TWI404_H
#define HAL_TWI404_H

#include <stdint.h>
#include <avr/io.h>

typedef enum {
    TWI_BAUD_100KHZ,
    TWI_BAUD_400KHZ,
} twi_baud_t;

typedef struct {
    twi_baud_t baud;
} twi_config_t;

typedef struct {
    twi_config_t config;
} twi_t;

twi_t twi_init(twi_config_t config);

void twi_start(void);

void twi_stop(void);

uint8_t twi_write(uint8_t data);

uint8_t twi_read(uint8_t ack);

uint8_t twi_write_bytes(uint8_t addr, const uint8_t *data, uint8_t len);

uint8_t twi_read_bytes(uint8_t addr, uint8_t *data, uint8_t len);

#endif
