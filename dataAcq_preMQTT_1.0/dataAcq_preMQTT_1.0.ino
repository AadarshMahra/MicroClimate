#include <DFRobot_SHT3x.h>
#include <WiFi.h>        // Include the Wi-Fi library
#include <ArduinoMqttClient.h>

const char* ssid     = "jeefsucksballs";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "trees12345";     // The password of the Wi-Fi network

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "http://0.0.0.0";
int        port     = 1883;
const char temp_topic[]  = "temperature";
const char rh_topic[]  = "rHumidity";
int temp_f;
int rh;

//set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

// Construct function. Chip address is 0x44
DFRobot_SHT3x sht3x(&Wire,/*address=*/0x44,/*RST=*/4);

void setup() {

  Serial.begin(9600);
    //Initialize the chip to detect if it can communicate properly.
  while (sht3x.begin() != 0) {
    Serial.println("Failed to initialize the chip, please confirm the chip connection");
    delay(1000);
  }
  delay(5000);

  // start periodic reading         
  if(!sht3x.startPeriodicMode(sht3x.eMeasureFreq_1Hz)){
    Serial.println("Failed to enter the periodic mode");
  }
 
  //begin WiFi setup
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

    int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("WiFi Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP to the computer

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  /**
   * getTemperatureF Get the measured temperature (in degrees Fahrenheit).
   * @return Return the float temperature data. 
   */
  temp_f = sht3x.getTemperatureF();
  Serial.print(temp_f);
  Serial.println(" F ");

  /**
   * getHumidityRH Get measured humidity(%RH)
   * @return Return the float humidity data
   */
  rh = sht3x.getHumidityRH();
  Serial.print(rh);
  Serial.println(" %RH");

  Serial.print("Sending message to topic: ");
  Serial.println(temp_topic);
  Serial.println(temp_f);

  Serial.print("Sending message to topic: ");
  Serial.println(rh_topic);
  Serial.println(rh);

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(temp_topic);
    mqttClient.print(temp_f);
    mqttClient.print("/r");
    mqttClient.endMessage();

    mqttClient.beginMessage(rh_topic);
    mqttClient.print(rh);
    mqttClient.print("/r");
    mqttClient.endMessage();
}
