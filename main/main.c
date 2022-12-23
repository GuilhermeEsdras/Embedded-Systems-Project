#include <stdio.h>
#include "../lib/include/water_level.h"
#include "../lib/include/soil_moisture.h"
#include "../lib/include/l_mqtt_client.h"
#include "../lib/include/rele_control.h"
#include "../lib/include/utils.h"
#include "esp_log.h"
#include "esp_event_base.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define RELE_PIN 27
#define MAX_MOISTURE 4095
#define MIN_MOISTURE 1550
#define MIN_WATER_LEVEL 135

Data* create_data() {
    Data* data = (Data *) malloc (sizeof(Data));
    return data;
}

int ha_baixa_variacao(int val_default) {
    int umidade_atual = get_soil_moisture();
    delay_s(10);
    int umidade_nova = get_soil_moisture();
    int porcentagem_nova = raw_to_porcentage(umidade_nova, MIN_MOISTURE, MAX_MOISTURE);
    printf("%d %d %d\n", val_default, umidade_atual, umidade_nova);
    if (porcentagem_nova > val_default) return 0;
    if (umidade_nova == 0 && umidade_atual == 0) return 1;
    if (umidade_atual >= umidade_nova && (umidade_nova*100/umidade_atual) > 95) return 1;
    if (umidade_atual < umidade_nova && (umidade_atual*100/umidade_nova) > 95) return 1;
    return 0;
}

void app_main(void) {

    Data * data = create_data();
    data -> isConnected = 0;
    data -> soilMoisture = -1;

    configure_adc_water_level();
    configure_adc_soil_moisture();
    configure_rele_control(RELE_PIN);
    mqtt_app_start(data);
    //setar botão

    while (!data -> isConnected) {
        delay_ms(500);
    }

    delay_ms(1500);
    //while para enquanto o botão estiver ligado continuar jogando água
    //data -> soilMoisture = 80;
    int hasWater = 1;
    while (1) {
        if (data -> soilMoisture != -1) {
            printf("water level %d\n", get_water_level());
            if (get_water_level() <= MIN_WATER_LEVEL) {
                if(hasWater) {
                    hasWater = 0;
                    envia_msg("IRRIGAMENTO:N;FALTA_AGUA:S", "/topic/status");
                }
            }
            else {
                hasWater = 1;
                printf("defaut moisture %d\n", data -> soilMoisture);
                printf("porcentagem atual %d\n", raw_to_porcentage(get_soil_moisture(), MIN_MOISTURE, MAX_MOISTURE));
                if (raw_to_porcentage(get_soil_moisture(), MIN_MOISTURE, MAX_MOISTURE) < (data -> soilMoisture - 5) && 
                ha_baixa_variacao(data -> soilMoisture + 5)) {
                    rele_set_level(1);
                    delay_ms(150);
                    rele_set_level(0);
                    delay_ms(150);
                }
            }
        }
        delay_ms(500);
    }
}