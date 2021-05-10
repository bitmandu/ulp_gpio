/**
 * @file ulp_gpio.c
 *
 * RTC GPIO support on the ULP-RISC-V. This API attempts to mirror the
 * driver/rtc_io.h API, but operate on the ULP-RISC-V.
 *
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-reference/peripherals/gpio.html
 * @see $IDF_PATH/components/hal/rtc_io_hal.c
 * @see $IDF_PATH/components/hal/esp32s2/include/hal/rtc_io_ll.h */

#include "ulp_riscv/ulp_riscv.h"

#include "soc/rtc_io_reg.h"

#include "ulp_gpio.h"

/**
 * Enable RTC GPIO input.
 *
 * @param pin RTC GPIO pin number
 */
static inline void ulp_gpio_input_enable(int pin)
{
    SET_PERI_REG_MASK(RTC_IO_TOUCH_PAD0_REG + pin * 4, RTC_IO_TOUCH_PAD0_FUN_IE);
}

/**
 * Disable RTC GPIO input.
 *
 * @param pin RTC GPIO pin number
 */
static inline void ulp_gpio_input_disable(int pin)
{
    CLEAR_PERI_REG_MASK(RTC_IO_TOUCH_PAD0_REG + pin * 4, RTC_IO_TOUCH_PAD0_FUN_IE);
}

/**
 * Enable RTC GPIO output.
 *
 * @param pin RTC GPIO pin number
 * @param mode Output mode: Normal or open drain
 */
static inline void ulp_gpio_output_enable(int pin, ulp_gpio_out_mode_t mode)
{
    REG_SET_FIELD(RTC_GPIO_ENABLE_W1TS_REG, RTC_GPIO_ENABLE_W1TS, BIT(pin));

    if (mode == ULP_GPIO_OUTPUT_OD) {
        SET_PERI_REG_MASK(RTC_GPIO_PIN0_REG + pin * 4, RTC_GPIO_PIN0_PAD_DRIVER);
    } else {
        CLEAR_PERI_REG_MASK(RTC_GPIO_PIN0_REG + pin * 4, RTC_GPIO_PIN0_PAD_DRIVER);
    }
}

/**
 * Disable RTC GPIO output.
 *
 * @param pin RTC GPIO pin number
 */
static inline void ulp_gpio_output_disable(int pin)
{
    REG_SET_FIELD(RTC_GPIO_ENABLE_W1TC_REG, RTC_GPIO_ENABLE_W1TC, BIT(pin));
    CLEAR_PERI_REG_MASK(RTC_GPIO_PIN0_REG + pin * 4, RTC_GPIO_PIN0_PAD_DRIVER);
}

void ulp_gpio_init(int pin)
{
    SET_PERI_REG_MASK(RTC_IO_TOUCH_PAD0_REG + pin * 4, RTC_IO_TOUCH_PAD0_MUX_SEL);
    REG_SET_FIELD(RTC_IO_TOUCH_PAD0_REG + pin * 4, RTC_IO_TOUCH_PAD0_FUN_SEL, 0);
}

void ulp_gpio_deinit(int pin)
{
    CLEAR_PERI_REG_MASK(RTC_IO_TOUCH_PAD0_REG + pin * 4, RTC_IO_TOUCH_PAD0_MUX_SEL);
}

void ulp_gpio_set_direction(int pin, ulp_gpio_mode_t mode)
{
    switch (mode) {
    case ULP_GPIO_MODE_INPUT_ONLY:
        ulp_gpio_input_enable(pin);
        ulp_gpio_output_disable(pin);
        break;

    case ULP_GPIO_MODE_OUTPUT_ONLY:
        ulp_gpio_input_disable(pin);
        ulp_gpio_output_enable(pin, ULP_GPIO_OUTPUT_NORMAL);
        break;

    case ULP_GPIO_MODE_INPUT_OUTPUT:
        ulp_gpio_input_enable(pin);
        ulp_gpio_output_enable(pin, ULP_GPIO_OUTPUT_NORMAL);
        break;

    case ULP_GPIO_MODE_DISABLED:
        ulp_gpio_input_disable(pin);
        ulp_gpio_output_disable(pin);
        break;

    case ULP_GPIO_MODE_OUTPUT_OD:
        ulp_gpio_input_disable(pin);
        ulp_gpio_output_enable(pin, ULP_GPIO_OUTPUT_OD);
        break;

    case ULP_GPIO_MODE_INPUT_OUTPUT_OD:
        ulp_gpio_input_enable(pin);
        ulp_gpio_output_enable(pin, ULP_GPIO_OUTPUT_OD);
        break;

    default:
        break;
    }
}

void ulp_gpio_set_drive_capability(int pin, ulp_gpio_drive_cap_t strength)
{
    REG_SET_FIELD(RTC_IO_TOUCH_PAD0_REG + pin * 4, RTC_IO_TOUCH_PAD0_DRV, strength);
}

void ulp_gpio_get_drive_capability(int pin, ulp_gpio_drive_cap_t *strength)
{
    *strength = REG_GET_FIELD(RTC_IO_TOUCH_PAD0_REG + pin * 4,
                              RTC_IO_TOUCH_PAD0_DRV);
}

void ulp_gpio_pullup_en(int pin)
{
    SET_PERI_REG_MASK(RTC_IO_TOUCH_PAD0_REG + pin * 4, RTC_IO_TOUCH_PAD0_RUE);
}

void ulp_gpio_pulldown_en(int pin)
{
    SET_PERI_REG_MASK(RTC_IO_TOUCH_PAD0_REG + pin * 4, RTC_IO_TOUCH_PAD0_RDE);
}

void ulp_gpio_pullup_dis(int pin)
{
    CLEAR_PERI_REG_MASK(RTC_IO_TOUCH_PAD0_REG + pin * 4, RTC_IO_TOUCH_PAD0_RUE);
}

void ulp_gpio_pulldown_dis(int pin)
{
    CLEAR_PERI_REG_MASK(RTC_IO_TOUCH_PAD0_REG + pin * 4, RTC_IO_TOUCH_PAD0_RDE);
}

void ulp_gpio_hold_en(int pin)
{
    SET_PERI_REG_MASK(RTC_CNTL_PAD_HOLD_REG, BIT(pin));
}

void ulp_gpio_hold_dis(int pin)
{
    CLEAR_PERI_REG_MASK(RTC_CNTL_PAD_HOLD_REG, BIT(pin));
}

uint64_t ulp_gpio_slow_clock_ticks(void)
{
    uint64_t now;

    SET_PERI_REG_MASK(RTC_CNTL_TIME_UPDATE_REG, RTC_CNTL_TIME_UPDATE);
    now = READ_PERI_REG(RTC_CNTL_TIME_LOW0_REG);
    now |= ((uint64_t)READ_PERI_REG(RTC_CNTL_TIME_HIGH0_REG)) << 32;

    return now;
}
