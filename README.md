# ATtiny HAL Library

A modern, type-safe Hardware Abstraction Layer (HAL) library for ATtiny microcontrollers. This library provides clean, well-documented APIs for all supported peripherals.

## Supported Microcontrollers

- **[ATtiny85](docs/attiny85.md)** - Legacy 8-bit AVR with USI peripheral
- **[ATtiny404](docs/attiny404.md)** - Modern AVR with dedicated peripherals (USART, TWI, SPI, TCA, TCB)

## Quick Start

### Prerequisites

- avr-gcc
- avr-ar
- avr-libc

### Build

```bash
# Build for ATtiny85 (default)
make

# Build for ATtiny404
make MCU=attiny404
```

This generates a static library in `build/`:
- `build/libattiny85.a` for ATtiny85
- `build/libattiny404.a` for ATtiny404

### Using the Library

```c
// ATtiny85
#include "attiny85.h"

// ATtiny404
#include "attiny404/attiny404.h"

int main(void) {
    // Your code here
    return 0;
}
```

Compile with:

```bash
# ATtiny85
avr-gcc -mmcu=attiny85 -Os -Iinclude -Lbuild -lattiny85 your_code.c -o output.elf

# ATtiny404
avr-gcc -mmcu=attiny404 -Os -Iinclude -Lbuild -lattiny404 your_code.c -o output.elf
```

## Documentation

See individual documentation files for detailed API reference:
- [ATtiny85 Documentation](docs/attiny85.md)
- [ATtiny404 Documentation](docs/attiny404.md)

## License

See LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues.
