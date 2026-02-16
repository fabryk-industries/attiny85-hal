#ifndef HAL_SPI404_H
#define HAL_SPI404_H

#include <stdint.h>
#include <avr/io.h>

typedef enum {
    SPI_MODE_0,
    SPI_MODE_1,
    SPI_MODE_2,
    SPI_MODE_3,
} spi_mode_t;

typedef enum {
    SPI_CLK_DIV4,
    SPI_CLK_DIV16,
    SPI_CLK_DIV64,
    SPI_CLK_DIV128,
} spi_clock_t;

typedef struct {
    spi_mode_t mode;
    spi_clock_t clock;
    uint8_t msb_first:1;
} spi_config_t;

typedef struct {
    spi_config_t config;
} spi_t;

spi_t spi_init(spi_config_t config);

uint8_t spi_transfer(uint8_t data);

void spi_write(uint8_t data);

uint8_t spi_read();

void spi_deinit(void);

#endif
