#include <stdio.h>
#include <stdlib.h>
#include "include/rele_control.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void configure_rele_control(gpio_num_t rele_pin) {
    _rele_pin = rele_pin;
    gpio_set_direction(rele_pin, GPIO_MODE_OUTPUT);
    rele_set_level(0);
}

void rele_set_level(int level) {
    gpio_set_level(_rele_pin, !level);
}

