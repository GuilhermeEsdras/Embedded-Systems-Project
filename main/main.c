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

#define RELE_PIN 27

int raw_to_porcentage(int moisture) {
    if (moisture < 1550) {
        return 100;
    }
    return 100 * (4095 - moisture) / (4095 - 1550);
}

void rele_set_level(int level) {
    gpio_set_level(RELE_PIN, !level);
}

char * int_moisture_to_string(int moisture) {
    char moisture_char[4];
    itoa(moisture, moisture_char, 10);
    return moisture_char;
}

Data* create_data() {
    Data* data = (Data *) malloc (sizeof(Data));
    return data;
}

int funcao(int val_default) {
    int umidade_atual = raw_to_porcentage(get_soil_moisture());
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    int umidade_nova = raw_to_porcentage(get_soil_moisture());
    printf("%d %d %d\n", val_default, umidade_atual, umidade_nova);
    if (umidade_nova > val_default) return 0;
    if (umidade_nova == 0 && umidade_atual == 0) return 1;
    if (umidade_atual >= umidade_nova && (umidade_nova*100/umidade_atual) < 5) return 1;
    if (umidade_atual < umidade_nova && (umidade_atual*100/umidade_nova) < 5) return 1;
    return 0;
}

void app_main(void) {

    Data * data = create_data();
    data -> isConnected = 0;
    data -> soilMoisture = -1;
    
    configure_adc_water_level();
    configure_adc_soil_moisture();
    mqtt_app_start(data);

    while (!data -> isConnected) {
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    vTaskDelay(1500 / portTICK_PERIOD_MS);
    rele_set_level(0);
    gpio_set_direction(RELE_PIN, GPIO_MODE_OUTPUT);
    data -> soilMoisture = 30;
    while (1) {
        if (data -> soilMoisture != -1) {
            printf("water level %d\n", get_water_level());
            if (get_water_level() <= 0) { //trocar valor
                envia_msg("IRRIGAMENTO:N;FALTA_AGUA:S", "/topic/status");
            }
            else {
                printf("defaut moisture %d\n", data -> soilMoisture);
                printf("porcentagem atual %d\n", raw_to_porcentage(get_soil_moisture()));
                if (raw_to_porcentage(get_soil_moisture()) < (data -> soilMoisture - 5) && funcao(data -> soilMoisture + 5)) {
                    rele_set_level(1);
                    vTaskDelay(150 / portTICK_PERIOD_MS);
                    rele_set_level(0);
                    vTaskDelay(150 / portTICK_PERIOD_MS);
                }
            }
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
        /*
        int level = get_water_level();
        if (level <= 135) {
            envia_msg("IRRIGAMENTO:N;FALTA_AGUA:S", "/topic/status");
        } else {
            while (data -> soilMoisture == -1) {
                vTaskDelay(500 / portTICK_PERIOD_MS);
            }
            while (1) {
                if (raw_to_porcentage(get_soil_moisture()) < data -> soilMoisture) {
                    level = get_water_level();
                    if (level <= 135) {
                        envia_msg(strcat("IRRIGAMENTO:I;FALTA_AGUA:S;UMIDADE:", int_moisture_to_string(Data -> soilMoisture)),"/topic/status");
                    }
                    rele_set_level(1);
                    vTaskDelay(500 / portTICK_PERIOD_MS);
                    rele_set_level(0);
                    vTaskDelay(500 / portTICK_PERIOD_MS);
                } else {
                    envia_msg(strcat("IRRIGAMENTO:S;UMIDADE:", int_moisture_to_string(Data -> soilMoisture)), "/topic/status");
                    break;
                }
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        */
        /*
        while (data -> soilMoisture == -1) {
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
        */
        
        //rele_set_level(1);
        //vTaskDelay(1000 / portTICK_PERIOD_MS);
        //rele_set_level(0);
        //vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}