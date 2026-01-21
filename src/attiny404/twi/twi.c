#include <stdint.h>
#include <avr/io.h>
#include "attiny404/twi/twi.h"

twi_t twi_init(twi_config_t config) {
    TWI0.MBAUD = (config.baud == TWI_BAUD_400KHZ) ? 47 : 255;
    TWI0.MCTRLA = TWI_ENABLE_bm;
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;

    twi_t twi = {config};
    return twi;
}

void twi_start(void) {
    TWI0.MADDR = 0;
    while (!(TWI0.MSTATUS & TWI_WIF_bm));
}

void twi_stop(void) {
    TWI0.MCTRLB = TWI_MCMD_STOP_gc;
}

uint8_t twi_write(uint8_t data) {
    TWI0.MDATA = data;
    while (!(TWI0.MSTATUS & TWI_WIF_bm));
    return !(TWI0.MSTATUS & TWI_ARBLOST_bm);
}

uint8_t twi_read(uint8_t ack) {
    while (!(TWI0.MSTATUS & TWI_RIF_bm));
    uint8_t data = TWI0.MDATA;
    TWI0.MCTRLB = ack ? TWI_ACKACT_ACK_gc : TWI_ACKACT_NACK_gc;
    return data;
}

uint8_t twi_write_bytes(uint8_t addr, const uint8_t *data, uint8_t len) {
    TWI0.MADDR = addr << 1;
    if (TWI0.MSTATUS & TWI_ARBLOST_bm) return 0;

    for (uint8_t i = 0; i < len; i++) {
        if (!twi_write(data[i])) return 0;
    }
    twi_stop();
    return 1;
}

uint8_t twi_read_bytes(uint8_t addr, uint8_t *data, uint8_t len) {
    TWI0.MADDR = (addr << 1) | 0x01;
    if (TWI0.MSTATUS & TWI_ARBLOST_bm) return 0;

    for (uint8_t i = 0; i < len; i++) {
        data[i] = twi_read(i < len - 1);
    }
    twi_stop();
    return 1;
}
