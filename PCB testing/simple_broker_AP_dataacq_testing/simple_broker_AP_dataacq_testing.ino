#include "TinyMqtt.h"       // https://github.com/hsaturn/TinyMqtt
#include "TinyStreaming.h"  // https://github.com/hsaturn/TinyConsole

#include <WiFi.h>
#include "Instruction.h"

#define PORT 1883
#define TEMP_TARGET 72
#define HUM_TARGET 60
#define TEMP_VARIANCE 3
#define HUM_VARIANCE 3
MqttBroker broker(PORT);

/** Basic Mqtt Broker
  *
  *  +-----------------------------+
  *  | ESP                         |
  *  |       +--------+            |
  *  |       | broker |            | 1883 <--- External client/s
  *  |       +--------+            |
  *  |                             |
  *  +-----------------------------+
  *
  *  Your ESP will become a MqttBroker.
	*  You can test it with any client such as mqtt-spy for example
	*
  */

const char* ssid = "MicroClimate";
const char* password = "MicroClimate";
IPAddress local_IP(192, 168, 1, 119);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

uint16_t httpPort = 80;
WiFiServer server(httpPort);
WiFiClient httpClient;
MqttClient mqtt_a(&broker);
char* sensor0_topic = "data_acq/node0/temp";
char* sensor1_topic = "data_acq/node0/rh";
char* control0_topic = "app_control/outlet0/control"; 
char* control1_topic = "app_control/outlet1/control"; 
volatile float sensor0_reading = 0.0;  
volatile float sensor1_reading = 0.0;



void onPublishTopic(const MqttClient* /* srce */, const Topic& topic, const char* payload, size_t /* length */) {
  const char* topic_str = topic.c_str();
  Serial << "~~~~~~~~~~~> Client received msg on topic " << topic_str << ", " << payload << endl;
  if (strcmp(topic_str, sensor0_topic) == 0)
    {
      sensor0_reading = atof(payload); 
    }
   if (strcmp(topic_str, sensor1_topic) == 0)
    {
      sensor1_reading = atof(payload); 
    }
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, password);
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");

  Console << TinyConsole::green << "Connected to " << ssid << "IP address: " << WiFi.localIP() << endl;

  /* Start http server */
  server.begin();
  Serial.println("http server started");


  broker.begin();
  Console << "Broker ready : " << WiFi.localIP() << " on port " << PORT << endl;

  /* subscribe to all DATA ACQ. topics */
  mqtt_a.setCallback(onPublishTopic);
  mqtt_a.subscribe(sensor0_topic);
  mqtt_a.subscribe(sensor1_topic);

}

void loop() {
  broker.loop();
  mqtt_a.loop();

    static auto next_send = millis();
    if (millis() > next_send)
    {
        next_send += 5000;

        if (not mqtt_a.connected())
        {
            Serial << millis() << ": Not connected to broker" << endl;
            return;
        }

        Serial << "~~~~~~~~~~~> Publishing a data_acq/node0/shutdown value: " << endl;
        
        /* publish shutdown instructions for DATA_ACQ */
        mqtt_a.publish("data_acq/node0/shutdown", "no");
        //mqtt_a.publish("data_acq/node0/shutdown", "no 2");

        Serial << sensor0_reading << endl; 
        Serial << sensor1_reading << endl; 
        
        /* compute control systems algorithm */
        instruction curr_inst = compute_inst(sensor0_reading, sensor1_reading, TEMP_TARGET, HUM_TARGET, TEMP_VARIANCE, HUM_VARIANCE); 
        /* TODO: publish control instructions to PWR/APP CTRL*/

        Serial << "^^xxxxx^^" << endl; 
        Serial << curr_inst.heater_instruction << endl; 
        Serial << curr_inst.humidifier_instruction << endl; 
        
        mqtt_a.publish(control0_topic, curr_inst.heater_instruction); 
        mqtt_a.publish(control1_topic, curr_inst.humidifier_instruction); 
        
    }
  
}