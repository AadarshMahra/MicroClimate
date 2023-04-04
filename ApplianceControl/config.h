#include <WiFi.h>
#include "TinyMqtt.h"    // https://github.com/hsaturn/TinyMqtt
#include "TinyStreaming.h" // https://github.com/hsaturn/TinyConsole

const unsigned int NUM_RELAYS = 4;
char* ENABLE = "On";
char* DISABLE = "Off";


unsigned int relay_on[NUM_RELAYS];
unsigned int relay_gpio[NUM_RELAYS] = {22,23,21,19};

const char* ssid = "MicroClimate";
const char* password = "MicroClimate";


static MqttClient client;

const char* BROKER = "192.168.1.119";
const uint16_t BROKER_PORT = 1883;
//std::string topic = "sensor/temperature";
const char* outlet_topics[NUM_RELAYS] = {"AppControl/Outlet0/control", "AppControl/Outlet1/control", "AppControl/Outlet2/control", "AppControl/Outlet3/control"};
const char* outlet_status[NUM_RELAYS] = {"AppControl/Outlet0/status", "AppControl/Outlet1/status", "AppControl/Outlet2/status", "AppControl/Outlet3/status"};
