idf_component_register(SRCS "l_mqtt_client.c"
                    INCLUDE_DIRS "include"  REQUIRES mqtt nvs_flash wifi_setup utils)
