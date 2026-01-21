# ATtiny85 HAL Library

A modern, type-safe Hardware Abstraction Layer (HAL) library for the ATtiny85 microcontroller. This library provides clean, well-documented APIs for all ATtiny85 peripherals.

## Features

- **GPIO** - Digital I/O with pin change interrupts
- **ADC** - 10-bit analog-to-digital converter with blocking/non-blocking modes
- **Timer0** - PWM generation and delay functions
- **Power Management** - Sleep modes and watchdog timer
- **EEPROM** - Non-volatile memory storage
- **USI SPI** - Hardware-assisted SPI master mode
- **USI I2C** - Hardware-assisted I2C master mode
- **UART** - Software UART using USI + Timer0 (half-duplex)

## Building the Library

### Prerequisites

- avr-gcc
- avr-ar
- avr-libc

### Build

```bash
make clean
make
```

This generates `build/libattiny85.a` - the static library archive.

### Using the Library in Your Project

```c
#include "attiny85.h"

int main(void) {
    // Your code here
    return 0;
}
```

Compile with:

```bash
avr-gcc -mmcu=attiny85 -Os -I/path/to/attiny85-hal/include \
    -L/path/to/attiny85-hal/build -lattiny85 your_code.c -o output.elf
```

## API Reference

### GPIO (General Purpose I/O)

The ATtiny85 has 6 GPIO pins (PB0-PB5) with alternate functions.

#### Initialization

```c
void gpio_init(gpio_pin_t pin, gpio_mode_t mode);
```

Initialize a pin with the specified mode:
- `GPIO_MODE_INPUT` - Input with high impedance (floating)
- `GPIO_MODE_INPUT_PULLUP` - Input with internal pull-up resistor
- `GPIO_MODE_INPUT_PULLDOWN` - Input (requires external pull-down)
- `GPIO_MODE_OUTPUT` - Output push-pull
- `GPIO_MODE_ANALOG` - Analog input (disables digital buffer)

```c
// Example: Configure PB0 as output
gpio_init(GPIO_PB0, GPIO_MODE_OUTPUT);

// Example: Configure PB1 as input with pull-up
gpio_init(GPIO_PB1, GPIO_MODE_INPUT_PULLUP);
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

#### Pin Change Interrupts

```c
void gpio_enable_pcint(gpio_pin_t pin, gpio_pcint_callback_t callback);
void gpio_disable_pcint(gpio_pin_t pin);
void gpio_get_pcint_changed(uint8_t changed[6]);
void gpio_pcint_handler(void);
```

```c
// Example: Enable interrupt on PB2
void my_callback(gpio_pin_t pin) {
    // Handle interrupt
}
gpio_enable_pcint(GPIO_PB2, my_callback);
```

### ADC (Analog-to-Digital Converter)

10-bit ADC with 4 single-ended channels, internal temperature sensor, and auto-trigger support.

#### Initialization

```c
adc_t adc_init(adc_reference_t ref, adc_prescaler_t prescaler);
```

Reference options:
- `ADC_REF_VCC` - Use VCC as reference
- `ADC_REF_INTERNAL_1V1` - Use internal 1.1V reference
- `ADC_REF_EXTERNAL` - Use external reference on AREF (PB0)

Channels:
- `ADC_CHANNEL_0` - ADC0 on PB5
- `ADC_CHANNEL_1` - ADC1 on PB2
- `ADC_CHANNEL_2` - ADC2 on PB4
- `ADC_CHANNEL_3` - ADC3 on PB3
- `ADC_CHANNEL_TEMP` - Internal temperature sensor
- `ADC_CHANNEL_1V1` - Internal 1.1V reference

```c
// Example: Initialize ADC with VCC reference and prescaler for 16MHz
adc_t adc = adc_init(ADC_REF_VCC, ADC_PRESCALER_128);
adc_enable(&adc);
```

#### ADC Operations

```c
void adc_enable(adc_t *adc);
void adc_disable(adc_t *adc);

// Blocking read (waits for completion)
uint16_t adc_read_blocking(adc_t *adc, adc_channel_t channel);

// Non-blocking operations
adc_status_t adc_read_start(adc_t *adc, adc_channel_t channel);
adc_status_t adc_read_poll(adc_t *adc, uint16_t *out_value);
uint8_t adc_is_busy(adc_t *adc);
```

```c
// Example: Read ADC channel 2
uint16_t value = adc_read_blocking(&adc, ADC_CHANNEL_2);
// value will be 0-1023
```

### Timer0 (PWM and Delays)

8-bit timer with two PWM channels (OC0A on PB0, OC0B on PB1).

#### PWM Initialization

```c
timer0_config_t config = {
    .mode = TIMER0_PWM_FAST,           // or TIMER0_PWM_PHASE_CORRECT
    .prescaler = TIMER0_PRESCALER_64   // F_CPU / 64
};
timer0_pwm_t pwm = timer0_pwm_init(config, TIMER0_PWM_CHANNEL_A);
```

PWM frequency = `F_CPU / (prescaler * 256)`

#### PWM Operations

```c
void timer0_pwm_set_duty(timer0_pwm_t pwm, uint8_t duty);
uint8_t timer0_pwm_get_duty(timer0_pwm_t pwm);
void timer0_pwm_disable(timer0_pwm_t pwm);
```

```c
// Example: Set 50% duty cycle
timer0_pwm_set_duty(pwm, 128);  // 128/255 ≈ 50%
```

#### Delay Functions

```c
void delay_ms(uint16_t ms);
void delay_us(uint16_t us);
```

```c
// Example: Delay 100ms
delay_ms(100);
```

### Power Management

Sleep modes and watchdog timer for power optimization.

#### Sleep Modes

```c
void hal_sleep_enable(sleep_mode_t mode);
void hal_sleep_now(void);
```

Sleep modes:
- `SLEEP_IDLE` - CPU stopped, peripherals running
- `SLEEP_ADC_NOISE_REDUCTION` - ADC noise reduction mode
- `SLEEP_POWER_DOWN` - Deepest sleep (~0.1 μA @ 1.8V)

#### Watchdog Timer

```c
void hal_wdt_enable(wdt_timeout_t timeout, wdt_mode_t mode);
void hal_wdt_reset(void);
void hal_wdt_disable(void);
```

Timeout options: `WDT_16MS`, `WDT_32MS`, `WDT_64MS`, `WDT_125MS`, `WDT_250MS`, `WDT_500MS`, `WDT_1S`, `WDT_2S`, `WDT_4S`, `WDT_8S`

Modes:
- `WDT_MODE_INTERRUPT` - Interrupt on timeout
- `WDT_MODE_RESET` - System reset on timeout

```c
// Example: Enable watchdog with 1 second timeout, reset mode
hal_wdt_enable(WDT_1S, WDT_MODE_RESET);

// Reset watchdog periodically
hal_wdt_reset();
```

#### Peripheral Clock Control

```c
void hal_clock_disable(uint8_t peripherals);
void hal_power_enable(uint8_t peripherals);
```

### EEPROM

Non-volatile storage (512 bytes on ATtiny85).

#### Byte Operations

```c
uint8_t hal_eeprom_read_byte(uint16_t addr);
void hal_eeprom_write_byte(uint16_t addr, uint8_t data);
void hal_eeprom_update_byte(uint16_t addr, uint8_t data);  // Only writes if different
```

#### Block Operations

```c
void hal_eeprom_read_block(void *dst, uint16_t src, uint16_t len);
void hal_eeprom_write_block(const void *src, uint16_t dst, uint16_t len);
void hal_eeprom_update_block(const void *src, uint16_t dst, uint16_t len);
```

```c
// Example: Read configuration from EEPROM
uint8_t config[16];
hal_eeprom_read_block(config, 0, 16);
```

### USI SPI Master

Hardware-assisted SPI using the Universal Serial Interface.

**Pins:**
- MOSI: PB0 (USI DO)
- MISO: PB1 (USI DI)
- SCK:  PB2 (USI USCK)

#### Initialization

```c
spi_config_t config = {
    .sclk_pin = 2,
    .mosi_pin = 0,
    .miso_pin = 1,
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
```

### USI I2C Master

Hardware-assisted I2C using the Universal Serial Interface.

**Pins:**
- SCL: PB2
- SDA: PB0

#### Initialization

```c
i2c_config_t config = {
    .sda_pin = 0,
    .scl_pin = 2,
    .timeout_us = 10000
};
i2c_t i2c = i2c_init(config);
```

#### I2C Operations

```c
i2c_status_t i2c_start(i2c_t *i2c);
i2c_status_t i2c_stop(i2c_t *i2c);
i2c_status_t i2c_write_byte(i2c_t *i2c, uint8_t data);
i2c_status_t i2c_write_byte_wait_ack(i2c_t *i2c, uint8_t data);
i2c_status_t i2c_read_byte(i2c_t *i2c, uint8_t *data);

// Convenience functions
i2c_status_t i2c_address(i2c_t *i2c, uint8_t address, uint8_t read_write);
i2c_status_t i2c_read_reg(i2c_t *i2c, uint8_t address, uint8_t reg, uint8_t *data);
i2c_status_t i2c_write_reg(i2c_t *i2c, uint8_t address, uint8_t reg, uint8_t data);
```

Status codes:
- `I2C_OK` - Success
- `I2C_ERR_NACK` - No acknowledge from slave
- `I2C_ERR_BUS_ERROR` - Bus error
- `I2C_ERR_TIMEOUT` - Timeout waiting for slave

```c
// Example: Write to I2C register
i2c_start(&i2c);
i2c_write_byte_wait_ack(&i2c, 0x50 << 1);  // Address + write bit
i2c_write_byte_wait_ack(&i2c, 0x00);       // Register address
i2c_write_byte_wait_ack(&i2c, 0xAA);       // Data
i2c_stop(&i2c);

// Or using convenience function:
i2c_write_reg(&i2c, 0x50, 0x00, 0xAA);
```

### UART

Software UART using USI + Timer0 (half-duplex).

**Pins:**
- RX: PB0 (USI DI)
- TX: PB1 (USI DO)

#### Initialization

```c
uart_config_t config = {
    .tx_pin = 1,
    .rx_pin = 0,
    .baudrate = 9600
};
uart_t uart = uart_init(config);
```

#### UART Operations

```c
void uart_putc(uart_t *uart, uint8_t data);
void uart_puts(uart_t *uart, const char *str);
uint8_t uart_getc(uart_t *uart, uint8_t *data, uint32_t timeout_us);
uint8_t uart_available(uart_t *uart);
```

```c
// Example: Send string
uart_puts(&uart, "Hello, World!\n");

// Example: Receive byte
uint8_t data;
if (uart_getc(&uart, &data, 100000)) {  // 100ms timeout
    // Byte received
}
```

## Pin Mapping

| Pin  | GPIO | Function(s)                                    |
|------|------|-------------------------------------------------|
| 5    | PB0  | MOSI/DI/SDA/AIN0/OC0A/OC1A/AREF/PCINT0         |
| 6    | PB1  | MISO/DO/AIN1/OC0B/OC1A/PCINT1                  |
| 7    | PB2  | SCK/USCK/SCL/ADC1/T0/INT0/PCINT2               |
| 2    | PB3  | XTAL1/CLKI/OC1B/ADC3/PCINT3                     |
| 3    | PB4  | XTAL2/CLKO/OC1B/ADC2/PCINT4                     |
| 1    | PB5  | RESET/ADC0/dW/PCINT5 (requires fuse changes)  |

## Configuration

### Clock Frequency

Edit `F_CPU` in the Makefile to match your clock frequency:

```makefile
F_CPU = 16000000UL  # 16 MHz (default)
# F_CPU = 8000000UL  # 8 MHz
# F_CPU = 1000000UL  # 1 MHz
```

### Fuse Configuration

The Makefile includes fuse configuration targets:

```bash
# Read current fuses
make read-fuses

# Set fuses for 16MHz internal oscillator
make write-fuses-16mhz

# Set fuses for 8MHz internal oscillator
make write-fuses-8mhz

# Set fuses for 1MHz internal oscillator
make write-fuses-1mhz
```

## Examples

### Blink LED

```c
#include "attiny85.h"

int main(void) {
    // Initialize PB0 as output
    gpio_init(GPIO_PB0, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_high(GPIO_PB0);
        delay_ms(500);
        gpio_set_low(GPIO_PB0);
        delay_ms(500);
    }

    return 0;
}
```

### Read Analog Input

```c
#include "attiny85.h"

int main(void) {
    // Initialize ADC
    adc_t adc = adc_init(ADC_REF_VCC, ADC_PRESCALER_128);
    adc_enable(&adc);

    // Configure PB2 as analog input
    gpio_init(GPIO_PB2, GPIO_MODE_ANALOG);

    while (1) {
        uint16_t value = adc_read_blocking(&adc, ADC_CHANNEL_1);
        delay_ms(100);
    }

    return 0;
}
```

### I2C Temperature Sensor

```c
#include "attiny85.h"

int main(void) {
    // Initialize I2C
    i2c_config_t config = {
        .sda_pin = 0,
        .scl_pin = 2,
        .timeout_us = 10000
    };
    i2c_t i2c = i2c_init(config);

    // Read temperature from sensor at address 0x48
    uint8_t temp;
    i2c_read_reg(&i2c, 0x48, 0x00, &temp);

    return 0;
}
```

## License

See LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues.
