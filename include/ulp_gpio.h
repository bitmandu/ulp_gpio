/**
 * @file ulp_gpio.h
 *
 * RTC GPIO support on the ULP-RISC-V. This API attempts to mirror the
 * driver/rtc_io.h API, but operate on the ULP-RISC-V.
 *
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-reference/peripherals/gpio.html
 * @see $IDF_PATH/components/hal/rtc_io_hal.c
 * @see $IDF_PATH/components/hal/esp32/include/hal/rtc_io_ll.h
 */

#pragma once

#include "soc/rtc_io_reg.h"

typedef enum { ULP_GPIO_OUTPUT_NORMAL, ULP_GPIO_OUTPUT_OD } ulp_gpio_out_mode_t;

typedef enum {
    ULP_GPIO_MODE_INPUT_ONLY,
    ULP_GPIO_MODE_OUTPUT_ONLY,
    ULP_GPIO_MODE_INPUT_OUTPUT,
    ULP_GPIO_MODE_DISABLED,
    ULP_GPIO_MODE_OUTPUT_OD,
    ULP_GPIO_MODE_INPUT_OUTPUT_OD
} ulp_gpio_mode_t;

typedef enum {
    ULP_GPIO_DRIVE_CAP_0,
    ULP_GPIO_DRIVE_CAP_1,
    ULP_GPIO_DRIVE_CAP_2,
    ULP_GPIO_DRIVE_CAP_3,
    ULP_GPIO_DRIVE_CAP_MAX
} ulp_gpio_drive_cap_t;

/**
 * Initialize RTC GPIO.
 *
 * @param pin RTC GPIO pin number
 * @see rtcio_ll_function_select() in components/hal/esp32s2/include/hal/rtc_io_ll.h
 */
void ulp_gpio_init(int pin);

/**
 * De-initialize RTC GPIO.
 *
 * @param pin RTC GPIO pin number
 */
void ulp_gpio_deinit(int pin);

/**
 * Configure RTC GPIO direction.
 *
 * @param pin RTC GPIO pin number
 * @param mode Direction
 */
void ulp_gpio_set_direction(int pin, ulp_gpio_mode_t mode);

/**
 * Set RTC GPIO pad drive capability.
 *
 * @param pin RTC GPIO pin number
 * @param strength Drive capability
 */
void ulp_gpio_set_drive_capability(int pin, ulp_gpio_drive_cap_t strength);

/**
 * Get RTC GPIO pad drive capability.
 *
 * @param pin RTC GPIO pin number
 * @param strength Drive capability
 */
void ulp_gpio_get_drive_capability(int pin, ulp_gpio_drive_cap_t *strength);

/**
 * Set RTC GPIO output level.
 *
 * @param pin RTC GPIO pin number
 * @param level Output level
 */
static inline void ulp_gpio_set_level(int pin, uint32_t level)
{
    if (level) {
        REG_SET_FIELD(RTC_GPIO_OUT_W1TS_REG, RTC_GPIO_OUT_DATA_W1TS, BIT(pin));
    } else {
        REG_SET_FIELD(RTC_GPIO_OUT_W1TC_REG, RTC_GPIO_OUT_DATA_W1TC, BIT(pin));
    }
}

/**
 * Get RTC GPIO level.
 *
 * @param pin RTC GPIO pin number
 * @return Returns 1 if pin is high and 0 otherwise.
 */
static inline uint32_t ulp_gpio_get_level(int pin)
{
    return (REG_GET_FIELD(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT) >> pin) & 0x1;
}

/**
 * Enable RTC GPIO pullup.
 *
 * @param pin RTC GPIO pin number
 */
void ulp_gpio_pullup_en(int pin);

/**
 * Enable RTC GPIO pulldown.
 *
 * @param pin RTC GPIO pin number
 */
void ulp_gpio_pulldown_en(int pin);

/**
 * Disable RTC GPIO pullup.
 *
 * @param pin RTC GPIO pin number
 */
void ulp_gpio_pullup_dis(int pin);

/**
 * Disable RTC GPIO pulldown.
 *
 * @param pin RTC GPIO pin number
 */
void ulp_gpio_pulldown_dis(int pin);

/**
 * Enable hold function.
 *
 * @param pin RTC GPIO pin number
 */
void ulp_gpio_hold_en(int pin);

/**
 * Disable hold function.
 *
 * @param pin RTC GPIO pin number
 */
void ulp_gpio_hold_dis(int pin);

/**
 * Number of RTC slow clock ticks.
 *
 * @return Number of clock ticks.
 */
uint64_t ulp_gpio_slow_clock_ticks(void);
