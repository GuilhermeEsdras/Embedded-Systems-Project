#include <stdio.h>
#include "../lib/include/water_level.h"
#include "../lib/include/soil_moisture.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char * TAG = "TAG";

void app_main(void) {
    configure_adc_water_level();
    configure_adc_soil_moisture();
    while (1) {
        int level = get_water_level();
        int moisture = get_soil_moisture();
        ESP_LOGI(TAG, "ADC1_CHANNEL_6: %d", level);
        vTaskDelay(pdMS_TO_TICKS(400));
        ESP_LOGI(TAG, "ADC1_CHANNEL_5: %d", moisture);
        vTaskDelay(pdMS_TO_TICKS(400));
    }
}
