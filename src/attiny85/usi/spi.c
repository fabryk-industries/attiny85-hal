#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "attiny85/usi/spi.h"

#define USI_THREE_WIRE_MODE ((1 << USIWM0) | (0 << USIWM1))
#define USI_EXTERNAL_CLOCK    ((1 << USICS1) | (0 << USICS0))
#define USI_STROBE_CLOCK    ((1 << USICLK) | (1 << USITC))

spi_t spi_init(spi_config_t config) {
    uint8_t usicr_value;

    DDRB |= (1 << config.sclk_pin) | (1 << config.mosi_pin);
    DDRB &= ~(1 << config.miso_pin);

    PORTB &= ~((1 << config.mosi_pin) | (1 << config.miso_pin) | (1 << config.sclk_pin));

    usicr_value = USI_THREE_WIRE_MODE;

    switch (config.mode) {
        case SPI_MODE_0:
            usicr_value |= USI_EXTERNAL_CLOCK;
            break;
        case SPI_MODE_1:
            usicr_value |= USI_EXTERNAL_CLOCK | (1 << USICS0);
            break;
        case SPI_MODE_2:
            usicr_value |= USI_EXTERNAL_CLOCK;
            break;
        case SPI_MODE_3:
            usicr_value |= USI_EXTERNAL_CLOCK | (1 << USICS0);
            break;
    }

    USICR = usicr_value;

    spi_t spi = { .config = config };
    return spi;
}

uint8_t spi_transfer(spi_t *spi, uint8_t data) {
    uint8_t tempUSISR;
    uint8_t usicr_value;
    uint8_t sreg;

    USIDR = data;
    USISR = (1 << USIOIF);

    tempUSISR = USI_THREE_WIRE_MODE | USI_EXTERNAL_CLOCK | USI_STROBE_CLOCK;

    sreg = SREG;
    cli();

    do {
        USICR = tempUSISR;
        while (!(USISR & (1 << USIOIF)));
    } while (1);

    SREG = sreg;

    return USIBR;
}

void spi_transfer_buf(spi_t *spi, const uint8_t *tx, uint8_t *rx, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        uint8_t data = tx[i];
        uint8_t recv = spi_transfer(spi, data);
        if (rx) {
            rx[i] = recv;
        }
    }
}

void spi_write(spi_t *spi, const uint8_t *data, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        spi_transfer(spi, data[i]);
    }
}
