typedef struct Data {
    int isConnected;
    int waterLevel;
    int soilMoisture;
    int isWaterPumpOn;
} Data;

void envia_msg(char *, char *);

void mqtt_app_start(Data *);