#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "../include/protocol_examples_common.h"
#include "../include/event_source.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "../include/led_strip.h"
#include "sdkconfig.h"

#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "esp_event_base.h"

#define LED_GPIO 2

static const char *TAG = "MQTT_WITH_RABBITMQ_EP32";
static esp_adc_cal_characteristics_t adc1_chars;
static uint8_t led_state = 0;
static uint32_t voltage;
static uint8_t is_connected = 0;

esp_event_loop_handle_t test;
esp_mqtt_client_handle_t client;

ESP_EVENT_DEFINE_BASE(TASK_EVENTS);

TaskHandle_t g_task;

static void blink_led(void) {
    led_state = !led_state;
    gpio_set_level(LED_GPIO, led_state);
    vTaskDelay(20);
}

static void configure_led(void) {
    ESP_LOGI(TAG, "Confiurando LED...");
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
}

static void configure_adc(void) {
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11));
}

static uint32_t get_water_level(void) {
    voltage = esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_6), &adc1_chars);
    ESP_LOGI(TAG, "ADC1_CHANNEL_6: %d mV", voltage);
    vTaskDelay(pdMS_TO_TICKS(100));
    return voltage;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event -> client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
        // O cliente estabeleceu com sucesso uma conexão com o broker. O cliente agora está pronto para enviar e receber dados.
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            ESP_LOGI(TAG, "ESP32 se inscreve na fila /topic/qos0, MSD_ID=%d", msg_id);
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
            ESP_LOGI(TAG, "ESP32 se inscreve na fila /topic/qos1, MSD_ID=%d", msg_id);
            is_connected = !is_connected;
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            blink_led();
            printf("TOPIC=%.*s\r\n", event -> topic_len, event -> topic);
            printf("DATA=%.*s\r\n", event -> data_len, event -> data);
            blink_led();
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event -> event_id);
            break;
        }
}

static void mqtt_app_start(void) {
    esp_mqtt_client_config_t mqtt_config = {
        .host = "192.168.31.13",
        .port = 1883,
        .username = "guest",
        .password = "guest"
    };
    client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

static void task_event_source(void* args) {
    uint32_t cont = 0;
    while (1) {
        if (is_connected) {
            ESP_LOGI(TAG, "Postando evento");
            ESP_ERROR_CHECK(esp_event_post_to(test, TASK_EVENTS, TASK_ITERATION_EVENT, &cont, sizeof(cont), portMAX_DELAY));
        }
        vTaskDelay(pdMS_TO_TICKS(TASK_PERIOD));
    }
}

static void task_iteration_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data) {
    ESP_LOGI(TAG, "Manipulando evento");
    int msg_id;
    uint32_t water_level = get_water_level();
    if (water_level > 202) {
        ESP_LOGI(TAG, "1!");
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "Pote cheio", 0, 1, 0);
    }
}

void app_main(void) {
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

    configure_led();
    configure_adc();

    ESP_ERROR_CHECK(example_connect());

    mqtt_app_start();

    esp_event_loop_args_t test_args = {
        .queue_size = 5,
        .task_name = "mqtt_task",
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = 2048,
        .task_core_id = tskNO_AFFINITY
    };

    ESP_ERROR_CHECK(esp_event_loop_create(&test_args, &test));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(test, TASK_EVENTS, TASK_ITERATION_EVENT, task_iteration_handler, test, NULL));

    ESP_LOGI(TAG, "Inciando event source");

    xTaskCreate(task_event_source, "task_event_source", 2048, NULL, uxTaskPriorityGet(NULL), NULL);
}