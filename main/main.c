#include <stdio.h>
#include "../lib/include/water_level.h"
#include "../lib/include/soil_moisture.h"
#include "../lib/include/l_mqtt_client.h"
#include "esp_log.h"
#include "esp_event_base.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

static const char * TAG = "TAG";

#define RELE_PIN 2

int isConnected = 0;

// double raw_to_porcentage(int moisture) {
//     moisture < 1200 ? return 100.00 : return 3300 - (4500 - moisture) / (330);
// }

void rele_set_level(int level) {
    gpio_set_level(RELE_PIN, level);
}

typedef struct Data {
    int isConnected;
    int waterLevel;
    int soilMoisture;
    int isWaterPumpOn;
} Data;

Data* create_data() {
    Data* data = (Data *) malloc (sizeof(Data));
    return data;
}

void app_main(void) {

    // configure_adc_water_level();
    // configure_adc_soil_moisture();
    Data* data = create_data();
    data->isConnected = 0;
    mqtt_app_start(data);
    
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    gpio_set_direction(RELE_PIN, GPIO_MODE_OUTPUT);
    while (1) {
        // int level = get_water_level();
        // if (level <= 160) {
        //     envia_msg("/topic/qos1", "Nível de água muito baixo! Por favor, reabastecer caixa");
        // } else {
            //int moisture = get_soil_moisture();
            ESP_LOGI(TAG, "loop");
            printf("%d\n", isConnected);
            //rele_set_level(1);
            //envia_msg("Oi", "/topic/qos1");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            //rele_set_level(0);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        //}
    }
}

