#include "include/l_mqtt_client.h"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "include/protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

esp_mqtt_client_handle_t client;

void mqtt_event_handler(void * handler_agrs, esp_event_base_t base, int32_t event_id, void * event_data) {
    esp_mqtt_event_handle_t event = event_data;
    int msg_id;
    switch ((esp_mqtt_event_id_t) event_id) {
        case MQTT_EVENT_CONNECTED:
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 0);
            break;
        default:
            break;
    }
}

void envia_msg(char * msg, char * topic) {
    int msg_id = esp_mqtt_client_publish(client, * topic, * msg, 0, 1, 0);
}

void mqtt_app_start(void) {
    esp_mqtt_client_config_t mqtt_config = {
        .host = "192.168.31.13",
        .port = 1883,
        .username = "guest",
        .password = "guest"
    };

    client = esp_mqtt_client_init(&mqtt_confg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}