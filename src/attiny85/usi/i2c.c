#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "attiny85/usi/i2c.h"

#define SDA_PIN     (1 << PB0)
#define SCL_PIN     (1 << PB2)

#define USI_2WIRE_MODE   ((1 << USIWM1) | (0 << USIWM0))
#define USI_EXTERNAL_CLOCK ((1 << USICS1) | (0 << USICS0))
#define USI_STROBE_CLOCK ((1 << USICLK) | (1 << USITC))

static i2c_config_t config;

static inline void usi_delay(void) {
    uint8_t i;
    for (i = 0; i < 10; i++) {
        __builtin_avr_delay_cycles(1);
    }
}

static void i2c_release_scl(void) {
    DDRB &= ~SCL_PIN;
    PORTB |= SCL_PIN;
}

static void i2c_release_sda(void) {
    DDRB &= ~SDA_PIN;
    PORTB |= SDA_PIN;
}

static void i2c_drive_scl_low(void) {
    DDRB |= SCL_PIN;
    PORTB &= ~SCL_PIN;
}

static void i2c_drive_sda_low(void) {
    DDRB |= SDA_PIN;
    PORTB &= ~SDA_PIN;
}

static void i2c_drive_sda_high(void) {
    DDRB |= SDA_PIN;
    PORTB |= SDA_PIN;
}

i2c_t i2c_init(i2c_config_t cfg) {
    config = cfg;

    i2c_release_scl();
    i2c_release_sda();

    _delay_ms(1);

    i2c_t i2c = { .config = cfg };
    return i2c;
}

i2c_status_t i2c_start(i2c_t *i2c) {
    i2c_release_scl();
    i2c_release_sda();
    usi_delay();

    i2c_drive_sda_low();
    usi_delay();

    i2c_drive_scl_low();
    usi_delay();

    return I2C_OK;
}

i2c_status_t i2c_stop(i2c_t *i2c) {
    i2c_drive_sda_low();
    usi_delay();

    i2c_release_scl();
    usi_delay();

    i2c_release_sda();
    usi_delay();

    return I2C_OK;
}

static uint8_t i2c_write_byte_usi(i2c_t *i2c, uint8_t data) {
    uint8_t tempUSISR;

    USIDR = data;
    USISR = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC) | (0x0 << USICNT0);

    tempUSISR = USI_2WIRE_MODE | USI_EXTERNAL_CLOCK | USI_STROBE_CLOCK;

    do {
        USICR = tempUSISR;
        while (!(PINB & SCL_PIN));
        USICR = tempUSISR;
        while (PINB & SCL_PIN);
        usi_delay();
    } while (!(USISR & (1 << USIOIF)));

    DDRB &= ~SDA_PIN;
    PORTB |= SDA_PIN;

    USIDR = 0xFF;
    USISR = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC) | (0xE << USICNT0);

    tempUSISR = USI_2WIRE_MODE | USI_EXTERNAL_CLOCK | USI_STROBE_CLOCK;

    USICR = tempUSISR;
    while (!(PINB & SCL_PIN));

    uint8_t ack = (PINB & SDA_PIN) ? 1 : 0;

    USICR = tempUSISR;
    while (PINB & SCL_PIN);

    DDRB |= SDA_PIN;

    return ack ? I2C_ERR_NACK : I2C_OK;
}

i2c_status_t i2c_write_byte(i2c_t *i2c, uint8_t data) {
    return i2c_write_byte_usi(i2c, data);
}

i2c_status_t i2c_write_byte_wait_ack(i2c_t *i2c, uint8_t data) {
    return i2c_write_byte_usi(i2c, data);
}

static uint8_t i2c_read_byte_usi(i2c_t *i2c, uint8_t ack) {
    uint8_t tempUSISR;

    USIDR = 0xFF;
    USISR = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC) | (0x0 << USICNT0);

    DDRB &= ~SDA_PIN;
    PORTB |= SDA_PIN;

    tempUSISR = USI_2WIRE_MODE | USI_EXTERNAL_CLOCK | USI_STROBE_CLOCK;

    do {
        USICR = tempUSISR;
        while (!(PINB & SCL_PIN));
        USICR = tempUSISR;
        while (PINB & SCL_PIN);
        usi_delay();
    } while (!(USISR & (1 << USIOIF)));

    uint8_t data = USIDR;

    USIDR = ack ? 0xFF : 0x00;
    USISR = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC) | (0xE << USICNT0);

    USICR = tempUSISR;
    while (!(PINB & SCL_PIN));
    USICR = tempUSISR;
    while (PINB & SCL_PIN);

    DDRB |= SDA_PIN;
    PORTB |= SDA_PIN;

    return data;
}

i2c_status_t i2c_read_byte(i2c_t *i2c, uint8_t *data) {
    *data = i2c_read_byte_usi(i2c, 0);
    return I2C_OK;
}

i2c_status_t i2c_address(i2c_t *i2c, uint8_t address, uint8_t read_write) {
    i2c_status_t status;

    status = i2c_start(i2c);
    if (status != I2C_OK) return status;

    uint8_t addr_byte = (address << 1) | read_write;
    return i2c_write_byte(i2c, addr_byte);
}

i2c_status_t i2c_read_reg(i2c_t *i2c, uint8_t address, uint8_t reg, uint8_t *data) {
    i2c_status_t status;

    status = i2c_address(i2c, address, 0);
    if (status != I2C_OK) return status;

    status = i2c_write_byte(i2c, reg);
    if (status != I2C_OK) return status;

    status = i2c_start(i2c);
    if (status != I2C_OK) return status;

    status = i2c_address(i2c, address, 1);
    if (status != I2C_OK) return status;

    *data = i2c_read_byte_usi(i2c, 1);
    i2c_stop(i2c);
    return I2C_OK;
}

i2c_status_t i2c_write_reg(i2c_t *i2c, uint8_t address, uint8_t reg, uint8_t data) {
    i2c_status_t status;

    status = i2c_address(i2c, address, 0);
    if (status != I2C_OK) return status;

    status = i2c_write_byte(i2c, reg);
    if (status != I2C_OK) return status;

    status = i2c_write_byte(i2c, data);
    if (status != I2C_OK) return status;

    i2c_stop(i2c);
    return I2C_OK;
}
