#include <stdio.h>
#include <string.h>
#include "water_level.h"
#include "soil_moisture.h"
#include "l_mqtt_client.h"
#include "rele_control.h"
#include "utils.h"
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

int ha_baixa_variacao(int porcentagem_default) {
    int umidade_atual = get_soil_moisture();
    delay_s(10);
    int umidade_nova = get_soil_moisture();
    int porcentagem_nova = raw_to_porcentage(umidade_nova, MIN_MOISTURE, MAX_MOISTURE);
    printf("%d %d %d\n", porcentagem_default, umidade_atual, umidade_nova);
    if (porcentagem_nova > porcentagem_default) return 0;
    if (umidade_nova == 0 && umidade_atual == 0) return 1;
    if (umidade_atual >= umidade_nova && (umidade_nova*100/umidade_atual) > 95) return 1;
    if (umidade_atual < umidade_nova && (umidade_atual*100/umidade_nova) > 95) return 1;
    return 0;
}

void concat(char * msg, char * buffer, int moisture) {
    strcpy(buffer, "");
    strcpy(buffer, msg);
    char buffer1[4];
    itoa(moisture, buffer1, 10);
    strcat(buffer, buffer1);
    printf("%s\n", buffer);
}

void app_main(void) {

    Data * data = create_data();
    data -> isConnected = 0;
    data -> soilMoisture = -1;

    configure_adc_water_level();
    configure_adc_soil_moisture();
    configure_rele_control(RELE_PIN);
    mqtt_app_start(data);

    while (!data -> isConnected) {
        delay_ms(500);
    }

    int hasWater = 1;
    int primeira_sugada = 1;
    int begin_irrigamento = 1;
    int is_umidade_baixa = 0;
    char buffer[50];
    while (1) {
        if (data -> soilMoisture != -1) {
            printf("water level %d\n", get_water_level());
            if (get_water_level() <= MIN_WATER_LEVEL) {
                if(hasWater) {
                    hasWater = 0;
                    concat("IRRIGAMENTO:I;UMIDADE:", buffer, data -> soilMoisture);
                    envia_msg(buffer, "/topic/status");
                }
            }
            else {
                if(primeira_sugada) {
                    rele_set_level(1);
                    delay_ms(3200);
                    rele_set_level(0);
                    delay_ms(150);
                    primeira_sugada = 0;
                }
                hasWater = 1;
                printf("defaut moisture %d\n", data -> soilMoisture);
                printf("porcentagem atual %d\n", raw_to_porcentage(get_soil_moisture(), MIN_MOISTURE, MAX_MOISTURE));
                is_umidade_baixa = raw_to_porcentage(get_soil_moisture(), MIN_MOISTURE, MAX_MOISTURE) < (data -> soilMoisture - 10);
                if (is_umidade_baixa){
                    if (ha_baixa_variacao(data -> soilMoisture + 10)) {
                        if(begin_irrigamento) {
                            concat("IRRIGAMENTO:S;UMIDADE:", buffer, data -> soilMoisture);
                            envia_msg(buffer, "/topic/status");
                            begin_irrigamento = 0;
                        }
                        rele_set_level(1);
                        delay_ms(150);
                        rele_set_level(0);
                        delay_ms(150);
                    }
                }
                else {
                    if(!begin_irrigamento) {
                        concat("IRRIGAMENTO:F;UMIDADE:", buffer, data -> soilMoisture);
                        envia_msg(buffer, "/topic/status");
                    }
                    begin_irrigamento = 1;   
                }
               
            }
        }
        delay_ms(500);
    }
}