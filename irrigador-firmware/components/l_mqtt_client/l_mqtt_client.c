#include "l_mqtt_client.h"
#include "wifi_setup.h"
#include "utils.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mqtt_client.h"

esp_mqtt_client_handle_t client;

/*
int get_moisture_from_mqtt(char * data) {
    char * str_aux = strtok(data, ":");
    if (strcmp("UMIDADE", str_aux) == 0) {
        str_aux = strtok(NULL, ":");
        return atoi(str_aux);
    }
    return 0;
}
*/

int get_moisture_from_mqtt(char * data) {
    return atoi(data);
}

void mqtt_event_handler(void * handler_agrs, esp_event_base_t base, int32_t event_id, void * event_data) {
    esp_mqtt_event_handle_t event = event_data;
    int msg_id;
    char * data = (char *) malloc(event -> data_len * sizeof(char));
    char * topic = (char *) malloc(event -> topic_len & sizeof(char));
    Data * arg = (Data *) event -> user_context;
    switch ((esp_mqtt_event_id_t) event_id) {
        case MQTT_EVENT_CONNECTED :
            msg_id = esp_mqtt_client_subscribe(client, "/topic/umidade", 0);
            ESP_LOGI("", "ESP32 se inscreve na fila /topic/umidade, MSD_ID=%d", msg_id);
            arg -> isConnected = 1;
            break;
        case MQTT_EVENT_DATA :
            for (int i = 0; i < event -> data_len; i ++) {
                data[i] = event -> data[i];
                data[i + 1] = 0;
            }
            arg -> soilMoisture = get_moisture_from_mqtt(data);
            printf("TOPIC=%.*s\r\n", event -> topic_len, event -> topic);
            printf("DATA=%s\n", data);
            free(data);
            free(topic);
            break;
        default:
            break;
    }
}

void envia_msg(char * msg, char * topic) {
    int msg_id = esp_mqtt_client_publish(client, topic, msg, 0, 0, 0);
}

void mqtt_app_start(Data * data) {
    wifi_setup_ssdi_password("brisa-1267191", "wp0wkigs");
    wifi_connect();

    delay_s(4);

    esp_mqtt_client_config_t mqtt_config = {
        .host = "192.168.96.214",
        .port = 1883,
        .username = "guest",
        .password = "guest",
        .user_context = (void *) data
    };

    client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}