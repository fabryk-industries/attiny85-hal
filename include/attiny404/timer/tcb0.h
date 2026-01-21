
#ifndef HAL_TCB0_H
#define HAL_TCB0_H

#include <stdint.h>
#include <avr/io.h>

typedef enum {
    TCB_CLK_DIV1 = 0x00,
    TCB_CLK_DIV2 = 0x01,
    TCB_CLK_DIV4 = 0x02,
} tcb_prescaler_t;

typedef struct {
    tcb_prescaler_t prescaler;
} tcb_config_t;

void tcb_init(tcb_config_t config);

void tcb_start(void);

void tcb_stop(void);

void tcb_reset(void);

uint16_t tcb_get_count(void);

void tcb_set_count(uint16_t count);

void tcb_delay_ms(uint16_t ms);

void tcb_delay_us(uint16_t us);

#endif
