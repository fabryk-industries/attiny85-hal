#include <stdint.h>
#include <avr/io.h>
#include "timer/tcb0.h"

static uint32_t f_cpu_khz = 16000;

void tcb_init(tcb_config_t config) {
    TCB0.CTRLB = TCB_CNTMODE_PWM_gc;
    TCB0.CTRLA = (config.prescaler << TCB_CLKSEL_gp);
    TCB0.CTRLA |= TCB_ENABLE_bm;
}

void tcb_start(void) {
    TCB0.CTRLA |= TCB_ENABLE_bm;
}

void tcb_stop(void) {
    TCB0.CTRLA &= ~TCB_ENABLE_bm;
}

void tcb_reset(void) {
    TCB0.CNT = 0;
}

uint16_t tcb_get_count(void) {
    return TCB0.CNT;
}

void tcb_set_count(uint16_t count) {
    TCB0.CNT = count;
}

void tcb_delay_ms(uint16_t ms) {
    uint32_t cycles = (uint32_t)ms * (f_cpu_khz / 8);
    while (cycles--) {
        __asm__ __volatile__("nop");
    }
}

void tcb_delay_us(uint16_t us) {
    uint16_t cycles = us * (f_cpu_khz / 1000);
    while (cycles--) {
        __asm__ __volatile__("nop");
    }
}
