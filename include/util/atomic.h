/**
 * @file atomic.h
 * @brief HAL atomic operation utilities
 *
 * Exposes avr-libc's atomic operations for protecting
 * critical sections and shared data access.
 */

#ifndef HAL_ATOMIC_H
#define HAL_ATOMIC_H

#include <stdint.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

/**
 * @defgroup hal_atomic Atomic Operations
 * @brief Critical section and atomic access utilities
 * @{
 */

#define ENTER_CRITICAL() uint8_t _sreg = SREG; cli()
#define EXIT_CRITICAL(sreg) SREG = (sreg)

/** @} */ // end of hal_atomic

#endif // HAL_ATOMIC_H
