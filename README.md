# ulp_gpio

This library provides RTC GPIO support for the [ULP-RISC-V coprocessor
on the ESP32-S2][1], and maybe other Espressif targets in the future.

The API attempts to mirror the [RTC GPIO API][2] --- only `rtc_gpio_`
is replaced with `ulp_gpio_` for namespace sanity. It is similar to
[example_ulp_gpio.h][3] included with the [ESP-IDF][4], but more
closely mirrors the existing API on the CPU and is (hopefully) easier
to include in your project.

## Installation

I recommend installing this code as a git submodule.

	$ git submodule add https://github.com/bitmandu/ulp_gio.git

Alternately, just copy `include/ulp_gpio.h` and `ulp_gpio.c` into your
project.

Details on [compiling ULP-RISC-V code][3] are provided by Espressif.
In addition, modify `CMakeLists.txt` as follows:

1. Add `ulp_gpio/include` to the `INCLUDE_DIRS` variable of the
   component registration.

````
idf_component_register(SRCS ...
  INCLUDE_DIRS "ulp_gpio/include" ...)
````
2. Add `ulp_gpio.c` to the ULP-RISC-V sources sent to
   `ulp_embed_binary`.

````
set(ulp_sources "ulp_gpio/ulp_gpio.c" ...)
...
ulp_embed_binary(${ulp_app_name} "${ulp_sources}" "${ulp_exp_dep_srcs}")
````

## Configuration

    $ idf.py set-target esp32s2
    $ idf.py menuconfig

In the `Component config` > `ESP32S2-specific` menu check:

- Enable Ultra Low Power (ULP) Coprocessor
- Enable RISC-V as ULP coprocessor

## Contributing

Pull requests and [issue / bug reports](issues/) are encouraged.

## LICENSE

[MIT](LICENSE)


[1]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-guides/ulp-risc-v.html
[2]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-reference/peripherals/gpio.html#api-reference-rtc-gpio
[3]: https://github.com/espressif/esp-idf/blob/master/examples/system/ulp_riscv/main/ulp/example_ulp_gpio.h
[4]: https://github.com/espressif/esp-idf/tree/master/examples/system/ulp_riscv
[5]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-guides/ulp-risc-v.html#compiling-the-ulp-risc-v-code
