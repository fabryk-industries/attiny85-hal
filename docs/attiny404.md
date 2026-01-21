# ATtiny404 HAL Documentation

A modern, type-safe Hardware Abstraction Layer (HAL) library for the ATtiny404 microcontroller. This library provides clean, well-documented APIs for all ATtiny404 peripherals.

## Features

- **GPIO** - Digital I/O with PORTA (8 pins) and PORTB (4 pins)
- **ADC** - 10-bit/8-bit ADC with 12 channels, internal temperature sensor
- **TCA0** - 16-bit Timer Type A with PWM (3 channels)
- **TCB0** - 8-bit Timer Type B for precise timing
- **USART0** - Hardware UART with configurable baud rate and frame format
- **TWI0** - Hardware I2C (100kHz/400kHz)
- **SPI0** - Hardware SPI master mode

## API Reference

### GPIO (General Purpose I/O)

The ATtiny404 has 12 GPIO pins (PORTA: PA0-PA7, PORTB: PB0-PB3) with alternate functions.

#### Initialization

```c
void gpio_init(gpio_pin_t pin, gpio_mode_t mode);
```

Initialize a pin with the specified mode:
- `GPIO_MODE_INPUT` - Input with high impedance (floating)
- `GPIO_MODE_INPUT_PULLUP` - Input with internal pull-up resistor
- `GPIO_MODE_OUTPUT` - Output push-pull
- `GPIO_MODE_ANALOG` - Analog input (disables digital buffer)

```c
// Example: Configure PA1 as output
gpio_init(GPIO_PA1, GPIO_MODE_OUTPUT);

// Example: Configure PA0 as input with pull-up
gpio_init(GPIO_PA0, GPIO_MODE_INPUT_PULLUP);
```

#### Pin Operations

```c
// Set direction
void gpio_set_output(gpio_pin_t pin);
void gpio_set_input(gpio_pin_t pin);
void gpio_set_input_pullup(gpio_pin_t pin);
void gpio_set_analog(gpio_pin_t pin);

// Write operations
void gpio_set_high(gpio_pin_t pin);
void gpio_set_low(gpio_pin_t pin);
void gpio_toggle(gpio_pin_t pin);
void gpio_write(gpio_pin_t pin, gpio_level_t level);

// Read operations
gpio_level_t gpio_read(gpio_pin_t pin);
uint8_t gpio_is_high(gpio_pin_t pin);
uint8_t gpio_is_low(gpio_pin_t pin);
```

#### Pin Mapping

**PORTA (PA0-PA7):**
- `GPIO_PA0` - AIN0/USART0 TX/TWI0 SCL/SPI0 SCK
- `GPIO_PA1` - AIN1/USART0 RX/TWI0 SDA/MOSI
- `GPIO_PA2` - AIN2/USART0 XCK/SPI0 MISO
- `GPIO_PA3` - AIN3/TCA0 WO0/RTC XIN
- `GPIO_PA4` - AIN4/TCA0 WO1/RTC XOUT
- `GPIO_PA5` - AIN5/TCA0 WO2
- `GPIO_PA6` - AIN6/TCA0 WO3
- `GPIO_PA7` - AIN7/TCA0 WO4

**PORTB (PB0-PB3):**
- `GPIO_PB0` - TDI/UPDI (programming pin)
- `GPIO_PB1` - TDO/TCA0 WO5
- `GPIO_PB2` - TMS/SPI0 SS
- `GPIO_PB3` - TCK/SPI0 MOSI

### ADC (Analog-to-Digital Converter)

12-bit ADC (selectable 8/10-bit resolution) with 12 single-ended channels, internal temperature sensor, and DAC reference.

#### Initialization

```c
adc_t adc_init(adc_reference_t ref, adc_prescaler_t prescaler, adc_resolution_t resolution);
```

Reference options:
- `ADC_REF_VDD` - Use VDD as reference
- `ADC_REF_INTERNAL_0V55` - Internal 0.55V reference
- `ADC_REF_INTERNAL_1V1` - Internal 1.1V reference
- `ADC_REF_INTERNAL_1V5` - Internal 1.5V reference
- `ADC_REF_INTERNAL_2V5` - Internal 2.5V reference
- `ADC_REF_INTERNAL_4V34` - Internal 4.34V reference
- `ADC_REF_EXTERNAL` - Use external reference on AREF

Prescaler options: `ADC_PRESCALER_DIV2`, `ADC_PRESCALER_DIV4`, `ADC_PRESCALER_DIV8`, `ADC_PRESCALER_DIV16`, `ADC_PRESCALER_DIV32`, `ADC_PRESCALER_DIV64`, `ADC_PRESCALER_DIV128`, `ADC_PRESCALER_DIV256`

Resolution options: `ADC_RES_8BIT`, `ADC_RES_10BIT`

Channels:
- `ADC_CH_AIN0` - `ADC_CH_AIN11` - 12 analog input pins
- `ADC_CH_TEMPSENSE` - Internal temperature sensor
- `ADC_CH_DAC0` - DAC output reference

```c
// Example: Initialize ADC with VDD reference, 64 prescaler, 10-bit resolution
adc_t adc = adc_init(ADC_REF_VDD, ADC_PRESCALER_DIV64, ADC_RES_10BIT);
adc_enable(&adc);
```

#### ADC Operations

```c
void adc_enable(adc_t *adc);
void adc_disable(adc_t *adc);

// Blocking read (waits for completion)
uint16_t adc_read_blocking(adc_t *adc, adc_channel_t channel);

// Non-blocking operations
void adc_read_start(adc_t *adc, adc_channel_t channel);
uint8_t adc_is_busy(adc_t *adc);
uint16_t adc_read_result(adc_t *adc);
```

```c
// Example: Read ADC channel 2 (AIN2 on PA2)
uint16_t value = adc_read_blocking(&adc, ADC_CH_AIN2);
// value will be 0-1023 for 10-bit resolution
```

### TCA0 (16-bit Timer Type A)

16-bit timer with PWM generation (6 channels: WO0-WO5).

#### PWM Initialization

```c
tca_config_t config = {
    .mode = TCA_PWM_SINGLE_SLOPE,
    .prescaler = TCA_PRESCALER_DIV64
};
tca_pwm_t pwm = tca_pwm_init(config, TCA_WO0);
```

Prescaler options: `TCA_PRESCALER_DIV1`, `TCA_PRESCALER_DIV2`, `TCA_PRESCALER_DIV4`, `TCA_PRESCALER_DIV8`, `TCA_PRESCALER_DIV16`, `TCA_PRESCALER_DIV64`, `TCA_PRESCALER_DIV256`, `TCA_PRESCALER_DIV1024`

Channels: `TCA_WO0`, `TCA_WO1`, `TCA_WO2`

#### PWM Operations

```c
void tca_pwm_set_duty(tca_pwm_t pwm, uint8_t duty);
uint8_t tca_pwm_get_duty(tca_pwm_t pwm);
void tca_pwm_disable(tca_pwm_t pwm);
void tca_deinit(void);
```

```c
// Example: Set 50% duty cycle on WO0
tca_pwm_set_duty(pwm, 128);  // 128/255 ≈ 50%
```

### TCB0 (8-bit Timer Type B)

8-bit timer for precise timing and event counting.

#### Initialization

```c
tcb_t tcb_init(tcb_prescaler_t prescaler);
```

#### TCB Operations

```c
void tcb_start(tcb_t *tcb);
void tcb_stop(tcb_t *tcb);
void tcb_reset(tcb_t *tcb);
uint8_t tcb_read(tcb_t *tcb);
```

### USART0 (Hardware UART)

Hardware UART with configurable baud rate, data bits, parity, and stop bits.

**Pins:**
- TX: PA0 ( USART0 TX)
- RX: PA1 (USART0 RX)

#### Initialization

```c
usart_config_t config = {
    .baud = USART_BAUD_115200,
    .databits = USART_DATABITS_8,
    .parity = USART_PARITY_NONE,
    .stopbits = USART_STOPBITS_1
};
usart_t usart = usart_init(config);
```

Baud rate options: `USART_BAUD_9600`, `USART_BAUD_19200`, `USART_BAUD_38400`, `USART_BAUD_57600`, `USART_BAUD_115200`

Data bits options: `USART_DATABITS_5`, `USART_DATABITS_6`, `USART_DATABITS_7`, `USART_DATABITS_8`, `USART_DATABITS_9`

Parity options: `USART_PARITY_NONE`, `USART_PARITY_EVEN`, `USART_PARITY_ODD`

Stop bits options: `USART_STOPBITS_1`, `USART_STOPBITS_2`

#### UART Operations

```c
void usart_putc(usart_t *usart, uint8_t data);
void usart_puts(usart_t *usart, const char *str);
uint8_t usart_getc(usart_t *usart);
uint8_t usart_available(usart_t *usart);
void usart_deinit(void);
```

```c
// Example: Send string
usart_puts(&usart, "Hello, World!\n");

// Example: Receive byte
uint8_t data;
data = usart_getc(&usart);
```

### TWI0 (Hardware I2C)

Hardware I2C with selectable speed (100kHz/400kHz).

**Pins:**
- SCL: PA0 (TWI0 SCL)
- SDA: PA1 (TWI0 SDA)

#### Initialization

```c
twi_config_t config = {
    .baud = TWI_BAUD_100KHZ  // or TWI_BAUD_400KHZ
};
twi_t twi = twi_init(config);
```

#### I2C Operations

```c
void twi_start(void);
void twi_stop(void);
uint8_t twi_write(uint8_t data);
uint8_t twi_read(uint8_t ack);
uint8_t twi_write_bytes(uint8_t addr, const uint8_t *data, uint8_t len);
uint8_t twi_read_bytes(uint8_t addr, uint8_t *data, uint8_t len);
```

```c
// Example: Write to I2C device at address 0x50
twi_start();
twi_write(0x50 << 1);  // Address + write bit
twi_write(0x00);       // Register address
twi_write(0xAA);       // Data
twi_stop();

// Example: Read from I2C device
twi_write_bytes(0x50, (uint8_t[]){0x00}, 1);  // Set register
uint8_t data;
twi_read_bytes(0x50, &data, 1);  // Read data
```

### SPI0 (Hardware SPI)

Hardware SPI master mode with configurable mode and bit order.

**Pins:**
- SCK: PA0 (SPI0 SCK)
- MOSI: PA1 (SPI0 MOSI)
- MISO: PA2 (SPI0 MISO)
- SS: PB2 (SPI0 SS)

#### Initialization

```c
spi_config_t config = {
    .sclk_pin = 0,
    .mosi_pin = 1,
    .miso_pin = 2,
    .mode = SPI_MODE_0,           // CPOL=0, CPHA=0
    .bit_order = SPI_BIT_ORDER_MSB_FIRST
};
spi_t spi = spi_init(config);
```

#### SPI Operations

```c
uint8_t spi_transfer(spi_t *spi, uint8_t data);
void spi_transfer_buf(spi_t *spi, const uint8_t *tx, uint8_t *rx, uint16_t len);
void spi_write(spi_t *spi, const uint8_t *data, uint16_t len);
```

```c
// Example: Transfer byte to SPI device
uint8_t response = spi_transfer(&spi, 0x9F);

// Example: Read multiple bytes
uint8_t tx_buf[2] = {0x03, 0x00};  // Command + address
uint8_t rx_buf[4];
spi_transfer_buf(&spi, tx_buf, rx_buf, 4);
```

## Configuration

### Clock Frequency

Edit `F_CPU` in `makefiles/attiny404.mk` to match your clock frequency:

```makefile
F_CPU = 20000000UL  # 20 MHz (default)
# F_CPU = 16000000UL  # 16 MHz
# F_CPU = 10000000UL  # 10 MHz
```

### Programming

The ATtiny404 uses the UPDI programming interface. Use `pyupdi` or `jtag2updi` for programming:

```bash
# Build for ATtiny404
make MCU=attiny404

# Flash example (requires pyupdi)
make MCU=attiny404 flash-blink_led
```

Note: Update the programming device path in `makefiles/attiny404.mk` if needed.

## Examples

### Blink LED

```c
#include "attiny404/attiny404.h"

int main(void) {
    // Initialize PA1 as output
    gpio_init(GPIO_PA1, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_toggle(GPIO_PA1);
        _delay_ms(500);
    }
}
```

### Read Analog Input

```c
#include "attiny404/attiny404.h"

int main(void) {
    // Initialize ADC with VDD reference, 64 prescaler, 10-bit resolution
    adc_t adc = adc_init(ADC_REF_VDD, ADC_PRESCALER_DIV64, ADC_RES_10BIT);
    adc_enable(&adc);

    // Configure PA2 as analog input (AIN2)
    gpio_init(GPIO_PA2, GPIO_MODE_ANALOG);

    while (1) {
        uint16_t value = adc_read_blocking(&adc, ADC_CH_AIN2);
        _delay_ms(100);
    }
}
```

### UART Echo

```c
#include "attiny404/attiny404.h"

int main(void) {
    // Initialize UART at 115200 baud, 8N1
    usart_config_t config = {
        .baud = USART_BAUD_115200,
        .databits = USART_DATABITS_8,
        .parity = USART_PARITY_NONE,
        .stopbits = USART_STOPBITS_1
    };
    usart_t usart = usart_init(config);

    while (1) {
        if (usart_available(&usart)) {
            uint8_t data = usart_getc(&usart);
            usart_putc(&usart, data);  // Echo back
        }
    }
}
```

### I2C Scan

```c
#include "attiny404/attiny404.h"

int main(void) {
    // Initialize I2C at 100kHz
    twi_config_t config = {
        .baud = TWI_BAUD_100KHZ
    };
    twi_t twi = twi_init(config);

    // Scan all I2C addresses
    for (uint8_t addr = 0; addr < 128; addr++) {
        twi_start();
        if (twi_write(addr << 1) == 0) {  // ACK received
            // Device found at address
            twi_stop();
        }
        twi_stop();
    }
}
```

### SPI Read

```c
#include "attiny404/attiny404.h"

int main(void) {
    // Initialize SPI, mode 0, MSB first
    spi_config_t config = {
        .sclk_pin = 0,
        .mosi_pin = 1,
        .miso_pin = 2,
        .mode = SPI_MODE_0,
        .bit_order = SPI_BIT_ORDER_MSB_FIRST
    };
    spi_t spi = spi_init(config);

    // Read from SPI device
    uint8_t tx = 0x9F;  // Read ID command
    uint8_t rx = spi_transfer(&spi, tx);

    while (1) {
        // Your code here
    }
}
```

## Key Differences from ATtiny85

| Feature | ATtiny85 | ATtiny404 |
|---------|----------|-----------|
| Architecture | Legacy AVR (8-bit only) | Modern AVR (AVRxt) |
| GPIO | 6 pins (PB0-PB5) | 12 pins (PA0-PA7, PB0-PB3) |
| Programming | ISP/SPI | UPDI (single wire) |
| ADC | 4 channels (10-bit) | 12 channels (8/10-bit) |
| PWM | Timer0 (2 channels) | TCA0 (6 channels) |
| UART | Software USI | Hardware USART |
| I2C | Software USI | Hardware TWI |
| SPI | Software USI | Hardware SPI |
| EEPROM | 512 bytes | 128 bytes |
| Sleep Modes | 3 modes | 5 modes (including standby) |
| Event System | No | Yes |
