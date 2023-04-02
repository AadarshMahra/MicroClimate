#include "TinyMqtt.h"   // https://github.com/hsaturn/TinyMqtt
#include <WiFi.h> 
#define PORT 1883
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
IPAddress local_IP(192,168,1,119);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

uint16_t httpPort = 80;
WiFiServer server(httpPort);
WiFiClient httpClient;

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
}

void loop()
{
  broker.loop();
}
