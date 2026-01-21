/**
 * @file spi_demo.c
 * @brief SPI demo for ATtiny404
 *
 * Sends and receives data via SPI0.
 */

#include <avr/io.h>
#include <util/delay.h>
#include "attiny404.h"

int main(void) {
    spi_config_t config = {
        .mode = SPI_MODE_0,
        .clock = SPI_CLK_DIV16,
        .msb_first = 1
    };

    spi_t spi = spi_init(config);

    while (1) {
        uint8_t tx_data = 0x55;
        uint8_t rx_data = spi_transfer(&spi, tx_data);
        _delay_ms(100);
    }
}
