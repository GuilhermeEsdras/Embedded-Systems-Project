#include "include/wifi_setup.h"

#include <string.h>
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_system.h"

char * ssid = "";
char * password = "";

void wifi_setup_ssdi_password(char * p_ssid, char * p_password) {
    ssid = (char *) p_ssid;
    password = (char *) p_password;
}

void wifi_connect(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI("WIFI_SETUP", "ESP_WIFI_MODE_STA");

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&config));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "Galaxy S20 FE 5G1FF1",
            .password = "nuvw8560",
	        .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_connect();

    ESP_LOGI("WIFI_SETUP", "ESP_WIFI_MODE_STA");
}
