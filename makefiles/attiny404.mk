# ATtiny404 HAL Library Makefile
# Target: ATtiny404 microcontroller
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
MCU = attiny404
F_CPU = 20000000UL  # 20 MHz internal clock (change as needed)

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
CFLAGS += -Iinclude/attiny404/gpio
CFLAGS += -Iinclude/attiny404/timer
CFLAGS += -Iinclude/attiny404/adc
CFLAGS += -Iinclude/attiny404/usart
CFLAGS += -Iinclude/attiny404/twi
CFLAGS += -Iinclude/attiny404/spi

# Linker Flags
LDFLAGS = -mmcu=$(MCU)
LDFLAGS += -Wl,--gc-sections  # Remove unused sections

# ============================================================================
# Directories
# ============================================================================
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
EXAMPLES_DIR = examples/attiny404
LIB_NAME = libattiny404.a

# ============================================================================
# Source Files (ATtiny404)
# ============================================================================
SOURCES = $(SRC_DIR)/attiny404/gpio/gpio.c \
          $(SRC_DIR)/attiny404/timer/tca0.c \
          $(SRC_DIR)/attiny404/timer/tcb0.c \
          $(SRC_DIR)/attiny404/adc/adc.c \
          $(SRC_DIR)/attiny404/usart/usart.c \
          $(SRC_DIR)/attiny404/twi/twi.c \
          $(SRC_DIR)/attiny404/spi/spi.c

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Examples
EXAMPLES = blink_led uart_demo adc_read spi_demo twi_scan

# Example objects
EXAMPLE_OBJECTS = $(EXAMPLES:%=$(BUILD_DIR)/%.o)
EXAMPLE_ELFS = $(EXAMPLES:%=$(BUILD_DIR)/%.elf)
EXAMPLE_HEXS = $(EXAMPLES:%=$(BUILD_DIR)/%.hex)

# ============================================================================
# Default Target
# ============================================================================
.PHONY: all
all: $(BUILD_DIR)/$(LIB_NAME) examples

# ============================================================================
# Create Build Directory
# ============================================================================
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# ============================================================================
# Compile Library Objects
# ============================================================================
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "  CC    $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# ============================================================================
# Create Static Library
# ============================================================================
$(BUILD_DIR)/$(LIB_NAME): $(OBJECTS)
	@echo "  AR    $(BUILD_DIR)/$(LIB_NAME)"
	@$(AR) rcs $@ $^

# ============================================================================
# Build Examples
# ============================================================================
examples: $(EXAMPLE_HEXS)

# Build individual examples
$(BUILD_DIR)/%.o: $(EXAMPLES_DIR)/%.c $(BUILD_DIR)/$(LIB_NAME) | $(BUILD_DIR)
	@echo "  CC    $(EXAMPLES_DIR)/$*.c"
	@$(CC) $(CFLAGS) -L$(BUILD_DIR) -lattiny404 $(LDFLAGS) -c $< -o $@

$(BUILD_DIR)/%.elf: $(BUILD_DIR)/%.o $(BUILD_DIR)/$(LIB_NAME) | $(BUILD_DIR)
	@echo "  LD    $*.elf"
	@$(CC) $(CFLAGS) -L$(BUILD_DIR) $< -lattiny404 $(LDFLAGS) -o $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo "  HEX   $(BUILD_DIR)/$*.hex"
	@$(OBJCOPY) -O ihex $< $@
	-@$(SIZE) -C --mcu=$(MCU) $< || true

.PHONY: examples

# ============================================================================
# Flash Example (requires pyupdi or jtag2updi)
# ============================================================================
flash-%: examples
	@echo "  FLASH $(BUILD_DIR)/$*.hex"
	@pyupdi -d tiny404 -b 115200 -c /dev/ttyUSB0 -f $(BUILD_DIR)/$*.hex
.PHONY: flash-%

# Alternative flash command for UPDI:
# flash-%: examples
# 	pyupdi -d tiny404 -b 115200 -c /dev/ttyUSB0 -U flash:w:$(BUILD_DIR)/$*.hex:i

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
	@echo "ATtiny404 HAL Library Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all              - Build library and all examples (default)"
	@echo "  examples         - Build all examples"
	@echo "  flash-<example>  - Flash example to MCU (e.g., flash-blink_led)"
	@echo "  clean            - Remove all build artifacts"
	@echo "  help             - Show this help message"
	@echo ""
	@echo "Configuration:"
	@echo "  MCU = $(MCU)"
	@echo "  F_CPU = $(F_CPU)"
	@echo "  CC = $(CC)"
	@echo ""
	@echo "Note: Update F_CPU in Makefile to match your clock frequency."
	@echo "      Uses pyupdi for programming via UPDI pin."
.PHONY: help
