#include <stdio.h>
#include "../lib/include/water_level.h"
#include "../lib/include/soil_moisture.h"
#include "../lib/include/l_mqtt_client.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char * TAG = "TAG";

void app_main(void) {
    
    // configure_adc_water_level();
    // configure_adc_soil_moisture();
    mqtt_app_start();
    while (1) {
    //     int level = get_water_level();
    //     int moisture = get_soil_moisture();
    //     if (level <= 170) {
    //         envia_msg("Nível de água muito baixo! Por favor, reabastecer caixa", "/topic/qos0");
    //     } else {
    //         ESP_LOGI(TAG, "ADC1_CHANNEL_6: %d", level);
    //         vTaskDelay(pdMS_TO_TICKS(400));
    //         ESP_LOGI(TAG, "ADC1_CHANNEL_5: %d", moisture);
    //         envia_msg("Oi", "/topic/qos0");
    //         vTaskDelay(pdMS_TO_TICKS(400));
    //     }
        envia_msg("mqtt-subscription-ESP32_ff3848qos0", "Oi");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

