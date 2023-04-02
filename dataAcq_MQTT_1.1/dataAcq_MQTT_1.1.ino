#include <DFRobot_SHT3x.h>
#include <WiFi.h>        // Include the Wi-Fi library
#include "TinyMqtt.h"    // https://github.com/hsaturn/TinyMqtt
#include "TinyStreaming.h" // https://github.com/hsaturn/TinyConsole

const char* ssid     = "MicroClimate";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "MicroClimate";     // The password of the Wi-Fi network
const char* BROKER = "192.168.1.119";
const uint16_t BROKER_PORT = 1883;
int temp_f;
int rh;

static float temp=19;
static MqttClient client;

// Construct function. Chip address is 0x44
DFRobot_SHT3x sht3x(&Wire,/*address=*/0x44,/*RST=*/4);

void setup() {
  Serial.begin(115200);
	delay(500);
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
 
 	Serial << "Simple clients with wifi Data Acq " << endl;
	if (strlen(ssid)==0)
		Serial << "****** PLEASE MODIFY ssid/password *************" << endl;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
	{ delay(500); Serial << '.'; }

  Serial << "Connected to " << ssid << "IP address: " << WiFi.localIP() << endl;

	client.connect(BROKER, BROKER_PORT);
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
		next_send += 1000;

		if (not client.connected())
		{
			Serial << millis() << ": Not connected to broker" << endl;
			return;
		}

		Serial << "--> Publishing a new sensor/temperature value: " << temp_f << endl;
		client.publish("sensor/temperature", String(temp_f));
  }
  // Serial.print("Sending message to topic: ");
  // Serial.println(temp_topic);
  // Serial.println(temp_f);

  // Serial.print("Sending message to topic: ");
  // Serial.println(rh_topic);
  // Serial.println(rh);
}
