#include "TinyMqtt.h"   // https://github.com/hsaturn/TinyMqtt
#include <WiFi.h> 
#include "Instruction.h"
#define PORT 1883


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
  
/* AP */
const char* ssid = "MicroClimate";
const char* password = "MicroClimate";
IPAddress local_IP(192,168,1,119);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

/* MQTT */ 
uint16_t httpPort = 80;
WiFiServer server(httpPort);
WiFiClient httpClient;
MqttBroker broker(PORT);

const char* BROKER = "192.168.1.119";
const uint16_t BROKER_PORT = 1883;
static MqttClient client(&broker);



const char* topic1 = "data_acq/node0/temp";
const char* topic2 = "data_acq/node/humidity"; 

void onPublishTopic(const MqttClient* /* srce */, const Topic& topic, const char* payload, size_t /* length */)
{ Serial << "--> Client received msg on topic " << topic.c_str() << ", " << payload << endl; }

const char ctrl0[]  = "AppControl/Outlet0/control"; // humidifier
const char ctrl1[]  = "AppControl/Outlet1/control"; // heater 
const char ctrl2[]  = "AppControl/Outlet2/control"; //
const char ctrl3[]  = "AppControl/Outlet3/control"; //


void setup()
{
  
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, password);
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid,password) ? "Ready" : "Failed!");

  Console << TinyConsole::green << "Connected to " << ssid << "IP address: " << WiFi.localIP() << endl;
  
  /* Start http server */ 
  server.begin();
  Serial.println("http server started"); 

  broker.begin();
  Console << "Broker ready : " << WiFi.localIP() << " on port " << PORT << endl;

  /* connect client to publish to topic */
  //client.connect(BROKER, BROKER_PORT); 
  
  
  /* TODO: subscribe to data acquisition topics */
  /*
  client.setCallback(onPublishTopic);
  client.subscribe(topic1);
  client.subscribe(topic2);

  Serial.print("Waiting for messages on topic: ");
  Serial << topic1 << endl;
  Serial.println(); 
  
  Serial.print("Waiting for messages on topic: ");
  Serial << topic2 << endl;
  Serial.println(); 
  */
}
char initial = 'D'; 
void loop()
{
  broker.loop();
  client.loop(); 
  /* publish instructions for power/appliance control */
  if (WiFi.status() != WL_CONNECTED) { WiFi.begin(ssid, password); }
  static auto next_req = millis();
  
  if (millis() > next_req)
  {
    next_req += 1000;
    if (not client.connected())
    { 
      Serial << millis() << ": Not connected to broker" << endl;
      return;
    }

    Serial << "SMIT IS HERE !!!" << endl;
    
    instruction current_inst = compute_inst(0.0, 0.0, 100.0, 100.0, 5.0);

    char* he_i = current_inst.heater_instruction; 
    char* hu_i = current_inst.humidifier_instruction; 
    Serial << he_i << endl;
    Serial << hu_i << endl;
    
    client.publish(ctrl0, he_i);
    client.publish(ctrl1, hu_i);


    Serial << broker.clientsCount() << endl;
    
  }
}
