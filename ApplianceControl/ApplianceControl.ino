//Appliance Control Node Code Version 3
//v1: Read On/Off signal from Serial Monitor (Ex, Dx) from x = 0-3 
//v2: Connect to broker 
//v3: Use TinyMQTT to create client
//v4: Control Relays with MQTT
//v4: Code is Done, Need to test with Relays now
//(possible)TODO: Create a config.h file to create macros for NUM_RELAYS, ENABLE, DISABLE, relay_on, relay_map, ssid, password, broker, broker port, outlet_topics
#include <WiFi.h>
#include "TinyMqtt.h"    // https://github.com/hsaturn/TinyMqtt
#include "TinyStreaming.h" // https://github.com/hsaturn/TinyConsole

const unsigned int NUM_RELAYS = 4;
const char* ENABLE = "1";
const char* DISABLE = "0";

unsigned int relay_on[NUM_RELAYS];
const unsigned int relay_map[NUM_RELAYS] = {22,23,21,19};

const char* ssid = "MicroClimate";
const char* password = "MicroClimate";


static MqttClient client;

const char* BROKER = "192.168.1.119";
const uint16_t BROKER_PORT = 1883;
//std::string topic = "sensor/temperature";
const char* outlet_topics[NUM_RELAYS] = {"AppControl/Outlet0/control", "AppControl/Outlet1/control", "AppControl/Outlet2/control", "AppControl/Outlet3/control"};
const char* outlet_status[NUM_RELAYS] = {"AppControl/Outlet0/status", "AppControl/Outlet1/status", "AppControl/Outlet2/status", "AppControl/Outlet3/status"};

void onPublishTopic(const MqttClient* /* srce */, const Topic& topic, const char* payload, size_t /* length */) {

  const char* topic_str = topic.c_str();
  Serial << "--> Client received msg on topic " << topic_str << ", " << payload << endl;

  //Find relay_id
  unsigned int relay_id;

  for(unsigned int i = 0; i < NUM_RELAYS; ++i){
    if(strcmp(outlet_topics[i],topic_str) == 0){
      relay_id = i;
      break;
    }
  }
  
  //Update array
  if(strcmp(payload,ENABLE) == 0){
    Serial.print("Turning ON Relay ");
    Serial.println(relay_id);
      
    relay_on[relay_id] = 1;
    digitalWrite(relay_map[relay_id],HIGH);
  }
  else if(strcmp(payload,DISABLE)== 0){
    Serial.print("Turning OFF Relay ");
    Serial.println(relay_id);
      
    relay_on[relay_id] = 0;
    digitalWrite(relay_map[relay_id],LOW);
  }
  else {
    Serial << "Unknown payload: " << payload << endl;
  }
  
}

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
  WiFi.setTxPower(WIFI_POWER_2dBm);
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

  for(int i = 0; i < NUM_RELAYS; i++){
    const char* topic = outlet_topics[i];
    Serial.print("Subscribing to topic: ");
    Serial << topic << endl;
    Serial.println();

    // subscribe to a topic
    client.setCallback(onPublishTopic);
    client.subscribe(topic);

    Serial.print("Waiting for messages on topic: ");
    Serial << topic << endl;
    Serial.println();
  }


}

void loop() {
  client.loop();
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

    //Sends a publish for all outlets and their status
    for(int i = 0; i < NUM_RELAYS; i++){
      char* relay_status;
      (relay_on[i] == 1) ? relay_status = "On" : relay_status = "Off";
      client.publish(outlet_status[i], relay_status);
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
  }*/
  for(int i = 0; i < NUM_RELAYS; ++i){
    Serial.print(relay_on[i]);
  }

  Serial.println();
    

}
