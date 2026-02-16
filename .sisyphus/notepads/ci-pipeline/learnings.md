# CI Pipeline Learnings

## Decisions Made

### AVR Toolchain
- Ubuntu's default `gcc-avr` package doesn't support modern AVR devices (like ATtiny404)
- Solution: Use prebuilt AVR toolchain from modm-io (https://github.com/modm-io/avr-gcc)
- This provides GCC 14.2.0 which supports modern AVR devices

### Release Automation
- Release job in workflow triggers on tags matching `v*`
- Uses `softprops/action-gh-release` action
- Downloads artifacts from build jobs and includes them in release

### Build Directories
- Using separate build directories (`build-attiny85`, `build-attiny404`) to avoid collision
- Artifacts uploaded per MCU for clean release packaging

## Issues Encountered
1. **Initial toolchain issue**: Ubuntu apt package `gcc-avr` doesn't support ATtiny404
   - Fixed by using modm-io prebuilt toolchain

## Notes
- Examples are only built for attiny404 (no examples exist for attiny85)
- Headers uploaded separately per MCU but they're the same - release step handles this
