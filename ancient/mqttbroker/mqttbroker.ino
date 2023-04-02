#include <WiFi.h> 
#include "EmbeddedMqttBroker.h"
using namespace mqttBrokerName;
const char *ssid = "shortsqueezemynuts";
const char *password = "trees12345";
const char *AP_NameChar = "WebServerAndBroker";
IPAddress local_IP(192,168,1,119);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

/******************* mqtt broker ********************/
uint16_t mqttPort = 1883;
MqttBroker broker(mqttPort);

/****************** http server ********************/
uint16_t httpPort = 80;
WiFiServer server(httpPort);
WiFiClient httpClient;

void setup(){
  Serial.begin(115200);
  Serial.println();
  /* set this MCU as an access point (AP) */ 
  WiFi.mode(WIFI_AP); 
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid,password) ? "Ready" : "Failed!");
  //WiFi.softAP(ssid);
  //WiFi.softAP(ssid, password, channel, hidden, max_connection)
  
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  
  /* Start http server */ 
  server.begin();
  Serial.println("http server started");  
  // Print the IP address 
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("");

  // Start the mqtt broker
  broker.setMaxNumClients(1); // set according to your system.
  broker.startBroker();
  Serial.println("broker started");

  // Print the IP address
  Serial.print("Use this ip: ");
  Serial.println(WiFi.localIP());
  Serial.print("and this port: ");
  Serial.println(mqttPort);
  Serial.println("To connect to mqtt broker");

}

void loop(){
  Serial.println(WiFi.softAPIP());
  delay(2000);  
}
