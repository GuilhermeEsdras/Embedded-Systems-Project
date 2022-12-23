#include <stdio.h>
#include <stdlib.h>
#include "button.h"
#include "utils.h"
#include "driver/gpio.h"

void configure_button(int button_pin) {
    _button_pin = button_pin;
    gpio_set_pull_mode(button_pin, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(button_pin, GPIO_MODE_INPUT);
}

void button_get_level() {
    int count = 10;

    for (int i  = 0; i < 100; i++) {
        if(gpio_get_level(_button_pin) == 1) {
            count--;
        }
        if(count == 0) return 1;

        delay_ms(1);
    }

    return 0;
}
