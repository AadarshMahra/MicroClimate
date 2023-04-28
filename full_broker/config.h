#include <WiFi.h>
#include "TinyMqtt.h"    // https://github.com/hsaturn/TinyMqtt
#include "TinyStreaming.h" // https://github.com/hsaturn/TinyConsole

#define PORT 1883

/* DEFINITIONS */
// WiFi and MQTT
const char* ssid = "MicroClimate";
const char* password = "MicroClimate";
IPAddress local_IP(192, 168, 1, 119);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

MqttBroker broker(PORT);
uint16_t httpPort = 80;
WiFiServer server(httpPort);
WiFiClient httpClient;
MqttClient mqtt_a(&broker);

// TOPICS
char* sensor0_topic = "data_acq/node0/temp";
char* sensor1_topic = "data_acq/node0/rh";
char* control0_topic = "app_control/outlet0/control"; 
char* control1_topic = "app_control/outlet1/control"; 
char* RPI_temp_topic = "RPI/targets/temp";
char* RPI_rh_topic = "RPI/targets/rh";

/* volatiles  to publish */ 
volatile float TEMP_TARGET = 0.0;
volatile float HUM_TARGET = 0.0;
volatile float sensor0_reading = 90.0;  
volatile float sensor1_reading = 90.0;
//volatile float change = .5;

void onPublishTopic(const MqttClient* /* srce */, const Topic& topic, const char* payload, size_t /* length */) {
  const char* topic_str = topic.c_str();
  Serial << "~~~~~~~~~~~> Client received msg on topic " << topic_str << ", " << payload << endl;
   /* can't use case switch :( */ 
  if (strcmp(topic_str, sensor0_topic) == 0){sensor0_reading = atof(payload);}
  if (strcmp(topic_str, sensor1_topic) == 0){sensor1_reading = atof(payload);}
  if (strcmp(topic_str, RPI_temp_topic) == 0){
    TEMP_TARGET = atof(payload);
    Serial << "New Temperature Target from User Interface: " << TEMP_TARGET << endl; 
    }
  if (strcmp(topic_str, RPI_rh_topic) == 0){
    HUM_TARGET = atof(payload);
    Serial << "New Humidity Target from User Interface: " << HUM_TARGET << endl;
    }
 
}
