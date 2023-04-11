//Appliance Control Node Code Version 3
//v1: Read On/Off signal from Serial Monitor (Ex, Dx) from x = 0-3 
//v2: Connect to broker 
//v3: Use TinyMQTT to create client
//v4: Control Relays with MQTT
//v4: Code is Done, Need to test with Relays now


//Enter config variables here
#include "config.h" 


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
    digitalWrite(relay_gpio[relay_id],HIGH);
  }
  else if(strcmp(payload,DISABLE)== 0){
    Serial.print("Turning OFF Relay ");
    Serial.println(relay_id);
      
    relay_on[relay_id] = 0;
    digitalWrite(relay_gpio[relay_id],LOW);
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
  
  for(int i = 0; i < NUM_RELAYS; ++i){
    pinMode(relay_gpio[i],OUTPUT);
  }

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
      (relay_on[i] == 1) ? relay_status = ENABLE : relay_status = DISABLE;
      client.publish(outlet_status[i], relay_status);
    }
  }
  for(int i = 0; i < NUM_RELAYS; ++i){
    Serial.print(relay_on[i]);
  }

  Serial.println();
    

}
