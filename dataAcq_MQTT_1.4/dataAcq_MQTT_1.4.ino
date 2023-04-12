#include <DFRobot_SHT3x.h> // Include Sensor Library
#include <WiFi.h>          // Include the Wi-Fi library
#include <Wire.h>          // Include I2C ESP32 Library
#include "TinyMqtt.h"      // https://github.com/hsaturn/TinyMqtt
#include "TinyStreaming.h" // https://github.com/hsaturn/TinyConsole

// v ADD TO CONFIG v
#define UPDATE_INTERVAL 5000
const char* ssid     = "MicroClimate";     // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "MicroClimate";     // The password of the Wi-Fi network
const char* BROKER = "192.168.1.119";      // static IP of broker host.
const uint16_t BROKER_PORT = 1883;       
// ^ ADD TO CONFIG ^

// pins for I2C signals, different than ESP32 default
#define I2C_SDA 40
#define I2C_SCL 42

// Sensor-set values
float temp_f;
float rh;

int fail_init_flag;
static MqttClient client;

// Construct function. Chip address is 0x44
DFRobot_SHT3x sht3x(&Wire,/*address=*/0x44,/*RST=*/4);

// sys status subscribe function
void onPublishTopic(const MqttClient* /* srce */, const Topic& topic, const char* payload, size_t /* length */) {

  const char* topic_str = topic.c_str();
  Serial << "--> Client received msg on topic " << topic_str << ", " << payload << endl;
  
  //TODO: add shutdown
}

void setup() {
  Serial.begin(115200);

  // set pins for I2C reading of sensor
  Wire.setPins(I2C_SDA, I2C_SCL);
  
  // begin sensor startup. set flag if fail
  if (sht3x.begin() != 0) {
    fail_init_flag = 0;
  }
  else {
    fail_init_flag = 1;
  }

  // start periodic reading         
  if(!sht3x.startPeriodicMode(sht3x.eMeasureFreq_1Hz)){
    Serial.println("Failed to enter the periodic mode");
  }

  // establish WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
	{ delay(500); Serial << '.'; }

  Serial << "Connected to " << ssid << "IP address: " << WiFi.localIP() << endl;

  // connect MQTT client to broker and setup subscription
	client.connect(BROKER, BROKER_PORT);
  client.setCallback(onPublishTopic);
  client.subscribe("data_acq/node0/shutdown");
  
}

void loop() {
  client.loop();	// Don't forget to call loop() for each broker and client

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

	// delay(1000);		please avoid usage of delay (see below how this done using next_send and millis())
	static auto next_send = millis();
	if (millis() > next_send)
	{
    //update every
		next_send += UPDATE_INTERVAL;
    
    // reboot software if not connected to broker
		if (not client.connected())
		{
			Serial << millis() << ": Not connected to broker" << endl;
      // TODO: add reboot   
			return;
		}

    // send sensor fail data to MQTT broker  
    if (fail_init_flag != 1) {
      client.publish("sensor/temperature", "sensor init fail");
    }

		Serial << "--> Publishing new values" << endl;
		client.publish("data_acq/node0/temp", String(temp_f));
    client.publish("data_acq/node0/rh", String(rh));

  }
}
