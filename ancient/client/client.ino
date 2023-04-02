#include <WiFi.h>        // Include the Wi-Fi library
#include <ArduinoMqttClient.h>
//#include <WiFiNINA.h>
# include "credentials.h" 


const char* ssid     = SECRET_SSID;         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = SECRET_PASS;     // The password of the Wi-Fi network

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);


 const char broker[] = "test.mosquitto.org";
 int port = 1883;
 const char topic[]  = "real_unique_topic";

//set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;

void setup() {
  // Start the Serial communication to send messages to the computer
  Serial.begin(115200);         
  delay(10);
  Serial.println('\n');
  
  // establish a connection to the AP network(broker MCU)
  WiFi.begin(ssid, password);             
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");
  
  // Wait for the Wi-Fi to connect
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP to the computer

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  
  // attempt to connect to MQTT
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {

  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    //record random value from A0, A1 and A2
    int Rvalue = analogRead(A0);

    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println(Rvalue);

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print(Rvalue);
    mqttClient.endMessage();

    Serial.println();
}
}
