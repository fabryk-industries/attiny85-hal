/**
 * @file adc.h
 * @brief ADC abstraction for ATtiny85
 *
 * ATtiny85 has a 10-bit ADC with 4 single-ended channels,
 * 2 differential pairs, internal temperature sensor, and auto-trigger support.
 */

#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdint.h>
#include <avr/io.h>

/**
 * @defgroup hal_adc ADC (Analog-to-Digital Converter)
 * @brief 10-bit ADC abstraction
 * @{
 */

/**
 * @brief ADC channel identifier
 */
typedef enum {
    ADC_CHANNEL_0 = 0,    ///< ADC0 on PB5
    ADC_CHANNEL_1 = 1,    ///< ADC1 on PB2
    ADC_CHANNEL_2 = 2,    ///< ADC2 on PB4
    ADC_CHANNEL_3 = 3,    ///< ADC3 on PB3
    ADC_CHANNEL_TEMP = 8,   ///< Internal temperature sensor
    ADC_CHANNEL_1V1 = 14,  ///< Internal 1.1V reference
} adc_channel_t;

/**
 * @brief ADC reference voltage
 */
typedef enum {
    ADC_REF_VCC = 0,        ///< VCC used as reference
    ADC_REF_INTERNAL_1V1,  ///< Internal 1.1V reference
    ADC_REF_EXTERNAL = 3,     ///< External reference on AREF (PB0)
} adc_reference_t;

/**
 * @brief ADC prescaler
 *
 * ADC clock = F_CPU / prescaler
 * ADC requires 50-200 kHz for 10-bit resolution
 */
typedef enum {
    ADC_PRESCALER_2 = 0,      ///< F_CPU / 2
    ADC_PRESCALER_2_2 = 1,    ///< F_CPU / 4
    ADC_PRESCALER_4 = 2,      ///< F_CPU / 8
    ADC_PRESCALER_8 = 3,      ///< F_CPU / 16
    ADC_PRESCALER_16 = 4,     ///< F_CPU / 32
    ADC_PRESCALER_32 = 5,     ///< F_CPU / 64
    ADC_PRESCALER_64 = 6,     ///< F_CPU / 128
    ADC_PRESCALER_128 = 7,    ///< F_CPU / 256
} adc_prescaler_t;

/**
 * @brief ADC conversion status
 */
typedef enum {
    ADC_OK,                  ///< Conversion complete, value ready
    ADC_BUSY,                ///< Conversion in progress
    ADC_ERROR,               ///< Error occurred
} adc_status_t;

/**
 * @brief ADC handle
 */
typedef struct {
    adc_reference_t ref;
    adc_prescaler_t prescaler;
    uint8_t in_progress:1;
    uint8_t channel;
} adc_t;

/**
 * @brief Initialize ADC
 *
 * Configures ADC with specified reference and prescaler.
 * ADC is left disabled after initialization.
 *
 * @param ref Voltage reference source
 * @param prescaler Clock prescaler
 * @return Initialized ADC handle
 */
adc_t adc_init(adc_reference_t ref, adc_prescaler_t prescaler);

/**
 * @brief Enable ADC
 *
 * Powers on the ADC circuitry.
 *
 * @param adc ADC handle
 */
void adc_enable(adc_t *adc);

/**
 * @brief Disable ADC
 *
 * Powers off the ADC circuitry.
 *
 * @param adc ADC handle
 */
void adc_disable(adc_t *adc);

/**
 * @brief Blocking ADC read
 *
 * Starts conversion on specified channel and blocks until complete.
 *
 * @param adc ADC handle
 * @param channel ADC channel to read
 * @return 10-bit ADC value (0-1023)
 */
uint16_t adc_read_blocking(adc_t *adc, adc_channel_t channel);

/**
 * @brief Start non-blocking conversion
 *
 * Starts conversion on specified channel. Returns immediately
 * without waiting for completion.
 *
 * @param adc ADC handle
 * @param channel ADC channel to read
 * @return ADC_BUSY if started, ADC_OK if already complete
 *
 * @note Only one conversion can be in progress at a time
 */
adc_status_t adc_read_start(adc_t *adc, adc_channel_t channel);

/**
 * @brief Poll for conversion completion
 *
 * Checks if the previously started conversion is complete.
 *
 * @param adc ADC handle
 * @param out_value Pointer to store result (can be NULL)
 * @return ADC_OK if complete, ADC_BUSY if in progress
 */
adc_status_t adc_read_poll(adc_t *adc, uint16_t *out_value);

/**
 * @brief Check if conversion is in progress
 *
 * @param adc ADC handle
 * @return Non-zero if conversion in progress
 */
uint8_t adc_is_busy(adc_t *adc);

/** @} */ // end of hal_adc

#endif // HAL_ADC_H
