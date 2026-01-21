#ifndef HAL_TCA0_H
#define HAL_TCA0_H

#include <stdint.h>
#include <avr/io.h>

typedef enum {
    TCA_PRESCALER_DIV1 = 0x00,
    TCA_PRESCALER_DIV2 = 0x01,
    TCA_PRESCALER_DIV4 = 0x02,
    TCA_PRESCALER_DIV8 = 0x03,
    TCA_PRESCALER_DIV16 = 0x04,
    TCA_PRESCALER_DIV64 = 0x05,
    TCA_PRESCALER_DIV256 = 0x06,
    TCA_PRESCALER_DIV1024 = 0x07,
} tca_prescaler_t;

typedef enum {
    TCA_PWM_SINGLE_SLOPE,
} tca_pwm_mode_t;

typedef enum {
    TCA_WO0,
    TCA_WO1,
    TCA_WO2,
} tca_pwm_channel_t;

typedef struct {
    tca_pwm_mode_t mode;
    tca_prescaler_t prescaler;
} tca_config_t;

typedef struct {
    tca_pwm_channel_t channel;
} tca_pwm_t;

tca_pwm_t tca_pwm_init(tca_config_t config, tca_pwm_channel_t channel);

void tca_pwm_set_duty(tca_pwm_t pwm, uint8_t duty);

uint8_t tca_pwm_get_duty(tca_pwm_t pwm);

void tca_pwm_disable(tca_pwm_t pwm);

void tca_deinit(void);

#endif
