/**
 * @file assert.h
 * @brief HAL assertion utilities
 *
 * Provides assertion macros for compile-time and runtime validation.
 * Designed to be lightweight and configurable for embedded systems.
 */

#ifndef HAL_ASSERT_H
#define HAL_ASSERT_H

#include <stdint.h>

/**
 * @defgroup hal_assert Assertion Utilities
 * @brief Compile-time and runtime assertion macros
 * @{
 */

/**
 * @def HAL_STATIC_ASSERT(expr, msg)
 * @brief Compile-time assertion
 *
 * This macro causes a compile error if the expression evaluates to false (0).
 * Useful for validating array sizes, alignment requirements, and other
 * compile-time constraints.
 *
 * @param expr Compile-time constant expression to evaluate
 * @param msg Error message to display on failure
 *
 * @note Uses the standard _Static_assert feature (C11)
 */
#define HAL_STATIC_ASSERT(expr, msg) _Static_assert((expr), msg)

/**
 * @def HAL_ASSERT(expr)
 * @brief Runtime assertion
 *
 * This macro evaluates the expression and halts execution if false.
 * In debug builds, this will trigger a breakpoint or infinite loop.
 * In release builds, assertions are compiled out for zero overhead.
 *
 * @param expr Expression to evaluate at runtime
 *
 * @note Set HAL_DEBUG=1 in build configuration to enable assertions
 * @note For embedded systems, assertions typically enter an infinite loop
 *       with interrupts disabled to make debugging easier with a logic analyzer
 */
#ifdef HAL_DEBUG
    #define HAL_ASSERT(expr) \
        do { \
            if (!(expr)) { \
                hal_assert_fail(__FILE__, __LINE__, #expr); \
            } \
        } while (0)
#else
    #define HAL_ASSERT(expr) ((void)(0))
#endif

/**
 * @def HAL_ASSUME(expr)
 * @brief Compiler hint assumption
 *
 * Tells the compiler that the expression is always true, enabling
 * better optimization. Unlike HAL_ASSERT, this has no runtime cost
 * in either debug or release builds.
 *
 * @param expr Expression that is assumed to be true
 *
 * @warning Use with caution: Undefined behavior if assumption is violated
 */
#if defined(__GNUC__) || defined(__clang__)
    #define HAL_ASSUME(expr) \
        do { \
            if (!(expr)) { \
                __builtin_unreachable(); \
            } \
        } while (0)
#else
    #define HAL_ASSUME(expr) ((void)(expr))
#endif

/**
 * @def HAL_UNREACHABLE()
 * @brief Mark code path as unreachable
 *
 * Tells the compiler that the current code path can never be reached.
 * Useful after switch statements with exhaustive cases or after
 * HAL_ASSERT(false) to avoid "control reaches end of non-void function"
 * warnings.
 *
 * @warning Calling this at runtime results in undefined behavior
 */
#if defined(__GNUC__) || defined(__clang__)
    #define HAL_UNREACHABLE() __builtin_unreachable()
#else
    #define HAL_UNREACHABLE() HAL_ASSERT(false)
#endif

/**
 * @def HAL_CHECK(expr)
 * @brief Error checking macro
 *
 * Similar to HAL_ASSERT but intended for recoverable errors.
 * In debug builds, this logs the error. In release builds, it evaluates
 * the expression but doesn't halt execution.
 *
 * @param expr Expression to check
 */
#ifdef HAL_DEBUG
    #define HAL_CHECK(expr) \
        do { \
            if (!(expr)) { \
                hal_check_fail(__FILE__, __LINE__, #expr); \
            } \
        } while (0)
#else
    #define HAL_CHECK(expr) ((void)(expr))
#endif

/**
 * @def HAL_ENSURE(expr)
 * @brief Post-condition assertion
 *
 * Asserts a condition at the end of a function to verify invariants.
 * Semantically identical to HAL_ASSERT but documents intent as a
 * post-condition check.
 *
 * @param expr Post-condition expression to verify
 */
#define HAL_ENSURE(expr) HAL_ASSERT(expr)

/**
 * @def HAL_REQUIRE(expr)
 * @brief Pre-condition assertion
 *
 * Asserts a condition at the start of a function to verify requirements.
 * Semantically identical to HAL_ASSERT but documents intent as a
 * pre-condition check.
 *
 * @param expr Pre-condition expression to verify
 */
#define HAL_REQUIRE(expr) HAL_ASSERT(expr)

/** @} */ // end of hal_assert

/**
 * @brief Called when a runtime assertion fails
 *
 * This function is called by HAL_ASSERT when the assertion expression
 * evaluates to false. The default implementation disables interrupts
 * and enters an infinite loop to halt execution.
 *
 * Override this function to provide custom assertion failure handling,
 * such as logging to UART, flashing an LED, or triggering a watchdog reset.
 *
 * @param file Source file name where assertion failed
 * @param line Line number where assertion failed
 * @param expr String representation of the failed expression
 *
 * @note This function is marked NORETURN and will never return
 */
#ifdef HAL_DEBUG
    #include <avr/interrupt.h>

    #ifdef __GNUC__
        __attribute__((noinline, noreturn))
    #endif
    static inline void hal_assert_fail(const char *file, uint16_t line, const char *expr)
    {
        (void)file;
        (void)line;
        (void)expr;

        // Disable interrupts to ensure we stay here
        cli();

        // Infinite loop - watchdog or reset required to recover
        // User can override this function to add logging or LED indication
        while (1) {
            // Empty - debugger can break here
        }
    }

    /**
     * @brief Called when a runtime check fails (non-fatal)
     *
     * Unlike hal_assert_fail, this function may return and execution
     * continues. Use it for logging warnings or errors that don't
     * require halting the system.
     *
     * @param file Source file name where check failed
     * @param line Line number where check failed
     * @param expr String representation of the failed expression
     */
    #ifdef __GNUC__
        __attribute__((noinline))
    #endif
    static inline void hal_check_fail(const char *file, uint16_t line, const char *expr)
    {
        (void)file;
        (void)line;
        (void)expr;

        // Default: do nothing - user can override for logging
    }
#endif // HAL_DEBUG

#endif // HAL_ASSERT_H
