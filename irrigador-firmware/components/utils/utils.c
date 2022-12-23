#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int raw_to_porcentage(int val, int val_min, int val_max) {
    if (val < val_min) {
        return 100;
    }
    return 100 * (val_max - val) / (val_max - val_min);
}

void delay_ms(int time) {
    vTaskDelay(time / portTICK_PERIOD_MS);
}

void delay_s(int time) {
    vTaskDelay(time*1000 / portTICK_PERIOD_MS);
}