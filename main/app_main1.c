#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event_base.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "mqtt_client.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "../include/protocol_examples_common.h"
#include "../include/led_strip.h"
#include "../include/event_source.h"

static const char* TAG = "MQTT(WITH_RABBITMQ_BROKER)_AND_WATER_SENSOR_IN_ESP32";

esp_event_loop_handle_t mqtt_loop_handle;
esp_event_loop_handle_t water_sensor_loop_handle;

static void mqtt_event_handler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch ((esp_mqtt_event_id_t)event_id) {
        
        // O cliente estabeleceu com sucesso uma conexão com o broker. O cliente agora está pronto para enviar e receber dados.
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            ESP_LOGI(TAG, "ESP32 se inscreve na fila /topic/qos0, MSD_ID=%d", msg_id);
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
            ESP_LOGI(TAG, "ESP32 se inscreve na fila /topic/qos1, MSD_ID=%d", msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event -> topic_len, event -> topic);
            printf("DATA=%.*s\r\n", event -> data_len, event -> data);
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event -> event_id);
            break;
        }
}

static void mqtt_app_start(void) {
    esp_mqtt_client_config_t mqtt_config = {
        .host = "192.168.0.16",
        .port = 1883,
        .username = "guest",
        .password = "guest"
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

static void task_event_source(void* args) {
    uint32_t cont = 0;
    //ESP_ERROR_CHECK(esp_event_post_to(mqtt_loop_handle, TASK_EVENTS, TASK_ITERATION_EVENT, &cont, sizeof(cont), portMAX_DELAY));    mqtt_app_start();
}

void app_main(void) {
    ESP_LOGI(TAG, "")

    ESP_LOGI(TAG, "[APP] Startup...");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(example_connect());

    esp_event_loop_args_t mqtt_loop_handle_args = {
        .queue_size = 5,
        .task_name = "mqtt_task",
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = 2048,
        .task_core_id = tskNO_AFFINITY
    };

    esp_event_loop_args_t water_sensor_loop_handle_args = {
        .queue_size = 5,
        .task_name = "water_sensor_task",
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = 2048,
        .task_core_id = tskNO_AFFINITY
    };

    ESP_ERROR_CHECK(esp_event_loop_create(&mqtt_loop_handle_args, &mqtt_loop_handle));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(mqtt_loop_handle, TASK_EVENTS, TASK_ITERATION_EVENT, NULL, mqtt_loop_handle, NULL));

    ESP_LOGI(TAG, "Inciando event source");

    xTaskCreate(task_event_source, "task_event_source", 2048, NULL, uxTaskPriorityGet(NULL), NULL);
}