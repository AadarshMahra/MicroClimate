#include <WiFi.h>
#include "TinyMqtt.h"    // https://github.com/hsaturn/TinyMqtt
#include "TinyStreaming.h" // https://github.com/hsaturn/TinyConsole

const unsigned int NUM_RELAYS = 2;
char* ENABLE = "On";
char* DISABLE = "Off";

//Allowed IO_Pins are (from bottom to top) 18, 17, 16, 15, 7,6 per https://www.espressif.com/sites/default/files/documentation/esp32-s3-wroom-1_wroom-1u_datasheet_en.pdf
unsigned int relay_on[NUM_RELAYS];
unsigned int relay_gpio[NUM_RELAYS] = {18, 16}; 

const char* ssid = "MicroClimate";
const char* password = "MicroClimate";


static MqttClient client;


const char* BROKER = "192.168.1.119";
const uint16_t BROKER_PORT = 1883;
//std::string topic = "sensor/temperature";
const char* outlet_topics[NUM_RELAYS] = {"app_control/outlet0/control", "app_control/outlet1/control"};
const char* outlet_status[NUM_RELAYS] = {"app_control/outlet0/status", "app_control/outlet1/status"};
