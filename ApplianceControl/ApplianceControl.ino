//Appliance Control Node Code Version 3
//v1: Read On/Off signal from Serial Monitor (Ex, Dx) from x = 0-3 
//v2: Connect to broker 
//v3: Use TinyMQTT to create client


#include <WiFi.h>
#include "TinyMqtt.h"    // https://github.com/hsaturn/TinyMqtt
#include "TinyStreaming.h" // https://github.com/hsaturn/TinyConsole

const unsigned int NUM_RELAYS = 4;
const char ENABLE = 'E';
const char DISABLE = 'D';

unsigned int relay_on[NUM_RELAYS];
const unsigned int relay_map[NUM_RELAYS] = {22,23,21,19};

const char* ssid = "MicroClimate";
const char* password = "MicroClimate";


static MqttClient client;

const char* BROKER = "192.168.1.119";
const uint16_t BROKER_PORT = 1883;
const char topic[] = "sensor/temperature";
const char topic0[]  = "AppControl/Outlet0";
const char topic1[]  = "AppControl/Outlet1";
const char topic2[]  = "AppControl/Outlet2";
const char topic3[]  = "AppControl/Outlet3";

void onPublishTopic(const MqttClient* /* srce */, const Topic& topic, const char* payload, size_t /* length */)
{ Serial << "--> Client received msg on topic " << topic.c_str() << ", " << payload << endl; }

void setup() {
  
  //Set up serial
  Serial.begin(115200);

  //Clear Relay On Array
  for(int i = 0; i < NUM_RELAYS; ++i){
    relay_on[NUM_RELAYS] = 0;
  }

  //Setup Pins
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);
  pinMode(21,OUTPUT);
  pinMode(19,OUTPUT);

  // attempt to connect to WiFi network:
  WiFi.mode(WIFI_STA);
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
   WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  { delay(500); Serial << '.'; }


  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  //mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(BROKER);

  client.connect(BROKER, BROKER_PORT);

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  // subscribe to a topic
  //mqttClient.setCallback(onPublishTopic);
  //mqttClient.subscribe(topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
  Serial.println();
  
}

void loop() {
  client.loop();
  if (WiFi.status() != WL_CONNECTED) { WiFi.begin(ssid, pass); }
  static auto next_req = millis();
  
  if (millis() > next_req)
  {
    next_req += 1000;

    if (not client.connected())
    {
      Serial << millis() << ": Not connected to broker" << endl;
      return;
    }
  }
    /*
  //Get Command if there is one (all commands are 2 bytes long: E(nable)/D(isable) and x)
  static char command;
  static unsigned int relay_id;
  if (Serial.available() >= 2) {   // check if at least 2 bytes are available
    Serial.println(Serial.available());
    char data[2];   // create an array to store the data
    Serial.readBytes(data, 2);   // read 2 bytes from the serial monitor and store them in the array
    Serial.read();
    command = data[0];
    relay_id = data[1] - 48; //Subtract 48 to switch from char to int, could add a parseInt but it messes stuff up
    Serial.print(command);   // print the value to the serial monitor
    Serial.println(relay_id);   // print the value to the serial monitor

    //Update array
    if(command == ENABLE){
      Serial.print("Turning ON Relay ");
      Serial.println(relay_id);
      
      relay_on[relay_id] = 1;
      digitalWrite(relay_map[relay_id],HIGH);
    }
    else if(command == DISABLE){
      Serial.print("Turning OFF Relay ");
      Serial.println(relay_id);
      
      relay_on[relay_id] = 0;
      digitalWrite(relay_map[relay_id],LOW);
    }
  }
  delay(1000);
  for(int i = 0; i < NUM_RELAYS; ++i){
    Serial.print(relay_on[i]);
  }

  Serial.println();*/
    

}