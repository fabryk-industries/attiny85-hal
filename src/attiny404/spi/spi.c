#include <stdint.h>
#include <avr/io.h>
#include "spi/spi.h"

spi_t spi_init(spi_config_t config) {
    SPI0.CTRLA = SPI_ENABLE_bm;

    switch (config.clock) {
        case SPI_CLK_DIV4:
            SPI0.CTRLA |= SPI_PRESC_DIV4_gc;
            break;
        case SPI_CLK_DIV16:
            SPI0.CTRLA |= SPI_PRESC_DIV16_gc;
            break;
        case SPI_CLK_DIV64:
            SPI0.CTRLA |= SPI_PRESC_DIV64_gc;
            break;
        case SPI_CLK_DIV128:
            SPI0.CTRLA |= SPI_PRESC_DIV128_gc;
            break;
    }

    switch (config.mode) {
        case SPI_MODE_0:
            break;
        case SPI_MODE_1:
            SPI0.CTRLB |= SPI_MODE1_bm;
            break;
        case SPI_MODE_2:
            SPI0.CTRLB |= SPI_MODE2_bm;
            break;
        case SPI_MODE_3:
            SPI0.CTRLB |= SPI_MODE2_bm | SPI_MODE1_bm;
            break;
    }

    if (!config.msb_first) {
        SPI0.CTRLA |= SPI_DORD_bm;
    }

    spi_t spi = {config};
    return spi;
}

uint8_t spi_transfer(spi_t *spi, uint8_t data) {
    SPI0.DATA = data;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    return SPI0.DATA;
}

void spi_write(spi_t *spi, uint8_t data) {
    SPI0.DATA = data;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
}

uint8_t spi_read(spi_t *spi) {
    SPI0.DATA = 0xFF;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    return SPI0.DATA;
}

void spi_deinit(void) {
    SPI0.CTRLA = 0;
}
