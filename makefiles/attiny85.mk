# ATtiny85 HAL Library Makefile
# Target: ATtiny85 microcontroller
# Toolchain: AVR-GCC

# ============================================================================
# Compiler and Tools
# ============================================================================
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar

# ============================================================================
# Target Configuration
# ============================================================================
MCU = attiny85
F_CPU = 16000000UL  # 16 MHz internal clock (change as needed)

# ============================================================================
# Compiler Flags
# ============================================================================
CFLAGS = -Wall -Wextra
CFLAGS += -Os                  # Optimize for size
CFLAGS += -mmcu=$(MCU)         # Target MCU
CFLAGS += -std=c11             # C11 standard
CFLAGS += -DF_CPU=$(F_CPU)     # CPU frequency
CFLAGS += -fdata-sections      # Separate data into sections
CFLAGS += -ffunction-sections  # Separate functions into sections
CFLAGS += -Iinclude            # Include path
CFLAGS += -Iinclude/attiny85/adc
CFLAGS += -Iinclude/attiny85/eeprom
CFLAGS += -Iinclude/attiny85/gpio
CFLAGS += -Iinclude/attiny85/power
CFLAGS += -Iinclude/attiny85/timer
CFLAGS += -Iinclude/attiny85/uart
CFLAGS += -Iinclude/attiny85/usi
CFLAGS += -Iinclude/attiny85/util

# Linker Flags
LDFLAGS = -mmcu=$(MCU)
LDFLAGS += -Wl,--gc-sections  # Remove unused sections

# ============================================================================
# Directories
# ============================================================================
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# ============================================================================
# Source Files (Phase 1-3)
# ============================================================================
SOURCES = $(SRC_DIR)/attiny85/gpio/gpio.c \
          $(SRC_DIR)/attiny85/timer/timer0.c \
          $(SRC_DIR)/attiny85/adc/adc.c \
          $(SRC_DIR)/attiny85/power/power.c \
          $(SRC_DIR)/attiny85/eeprom/eeprom.c \
          $(SRC_DIR)/attiny85/usi/spi.c \
          $(SRC_DIR)/attiny85/usi/i2c.c \
          $(SRC_DIR)/attiny85/uart/uart.c

# ============================================================================
# Object Files and Library
# ============================================================================
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
LIB = $(BUILD_DIR)/libattiny85.a

# ============================================================================
# Default Target
# ============================================================================
all: $(LIB)

# ============================================================================
# Create Build Directory
# ============================================================================
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# ============================================================================
# Compile Library Objects
# ============================================================================
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "  CC    $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# ============================================================================
# Create Static Library
# ============================================================================
$(LIB): $(OBJECTS)
	@$(AR) rcs $@


# ============================================================================
# Fuse Configuration
# ============================================================================
# Default fuses for internal 16MHz oscillator with 64ms startup time
# LFUSE = 0xE1 (CKSEL[3:0]=0010, SUT[1:0]=10)
# HFUSE = 0xDF (SPIEN=1, WDTON=0, EESAVE=0, BODLEVEL=111)
# EFUSE = 0xFF (reserved)

read-fuses:
	@avrdude -p $(MCU) -c usbasp -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h
.PHONY: read-fuses

write-fuses-16mhz:
	@echo "Writing fuses for 16MHz internal oscillator..."
	@avrdude -p $(MCU) -c usbasp -U lfuse:w:0xE1:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m
.PHONY: write-fuses-16mhz

write-fuses-8mhz:
	@echo "Writing fuses for 8MHz internal oscillator..."
	@avrdude -p $(MCU) -c usbasp -U lfuse:w:0xE2:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m
.PHONY: write-fuses-8mhz

write-fuses-1mhz:
	@echo "Writing fuses for 1MHz internal oscillator..."
	@avrdude -p $(MCU) -c usbasp -U lfuse:w:0x62:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m
.PHONY: write-fuses-1mhz

# ============================================================================
# Disassemble
# ============================================================================
disasm-%: examples
	@$(OBJDUMP) -d -S $(BUILD_DIR)/$*.elf | less

.PHONY: disasm-%
# ============================================================================
# Clean Build Artifacts
# ============================================================================
clean:
	@echo "  CLEAN"
	@rm -rf $(BUILD_DIR)
.PHONY: clean

# ============================================================================
# Show Help
# ============================================================================
help:
	@echo "ATtiny85 HAL Library Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all              - Build library and all examples (default)"
	@echo "  examples         - Build all examples"
	@echo "  blink_led        - Build blink_led example"
	@echo "  flash-<example>  - Flash example to MCU (e.g., flash-blink_led)"
	@echo "  read-fuses       - Read fuse bytes from MCU"
	@echo "  write-fuses-16mhz - Set fuses for 16MHz internal oscillator"
	@echo "  write-fuses-8mhz  - Set fuses for 8MHz internal oscillator"
	@echo "  write-fuses-1mhz - Set fuses for 1MHz internal oscillator"
	@echo "  disasm-<example> - Disassemble example (e.g., disasm-blink_led)"
	@echo "  clean            - Remove all build artifacts"
	@echo "  help             - Show this help message"
	@echo ""
	@echo "Configuration:"
	@echo "  MCU = $(MCU)"
	@echo "  F_CPU = $(F_CPU)"
	@echo "  CC = $(CC)"
	@echo ""
	@echo "Note: Update F_CPU in Makefile to match your clock frequency."
	@echo "      Update programmer (-c flag) in flash targets if needed."
.PHONY: help
