#include <stdint.h>
#include <avr/io.h>
#include "tca0.h"

tca_pwm_t tca_pwm_init(tca_config_t config, tca_pwm_channel_t channel) {
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;

    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc;

    TCA0.SINGLE.CTRLA |= (config.prescaler << TCA_SINGLE_CLKSEL_gp);
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;

    switch (channel) {
        case TCA_WO0:
            TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP0EN_bm;
            break;
        case TCA_WO1:
            TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP1EN_bm;
            break;
        case TCA_WO2:
            TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP2EN_bm;
            break;
    }

    tca_pwm_t pwm = {channel};
    return pwm;
}

void tca_pwm_set_duty(tca_pwm_t pwm, uint8_t duty) {
    switch (pwm.channel) {
        case TCA_WO0:
            TCA0.SINGLE.CMP0 = duty;
            break;
        case TCA_WO1:
            TCA0.SINGLE.CMP1 = duty;
            break;
        case TCA_WO2:
            TCA0.SINGLE.CMP2 = duty;
            break;
    }
}

uint8_t tca_pwm_get_duty(tca_pwm_t pwm) {
    switch (pwm.channel) {
        case TCA_WO0:
            return TCA0.SINGLE.CMP0;
        case TCA_WO1:
            return TCA0.SINGLE.CMP1;
        case TCA_WO2:
            return TCA0.SINGLE.CMP2;
        default:
            return 0;
    }
}

void tca_pwm_disable(tca_pwm_t pwm) {
    switch (pwm.channel) {
        case TCA_WO0:
            TCA0.SINGLE.CTRLB &= ~TCA_SINGLE_CMP0EN_bm;
            break;
        case TCA_WO1:
            TCA0.SINGLE.CTRLB &= ~TCA_SINGLE_CMP1EN_bm;
            break;
        case TCA_WO2:
            TCA0.SINGLE.CTRLB &= ~TCA_SINGLE_CMP2EN_bm;
            break;
    }
}

void tca_deinit(void) {
    TCA0.SINGLE.CTRLA = 0;
}
