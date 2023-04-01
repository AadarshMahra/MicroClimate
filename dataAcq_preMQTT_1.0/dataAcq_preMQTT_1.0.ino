/*!
 * @file periodicDataReading.ino
 * @brief Read ambient temperature (C/F) and relative humidity (%RH) in cycle read mode.
 * @details Experimental phenomenon: Before we start, please set the read frequency and repeatability of the read
 * @n (the difference between the data measured by the chip under the same measurement conditions),
 * and enter the periodic read mode, and then read the temperature and humidity data.
 * @n The temperature and humidity data will be printed at the serial port, after 10 seconds of operation.
 * @n It will exit the cycle mode and enter 2 measurement mode: Single measurement mode and Cycle measurement mode.
 * @n Single measurement mode: reflect the difference between the two modes of reading data.
 * @n Cycle measurement mode: the chip periodically monitors temperature and humidity, only in this mode the ALERT pin will work.
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2019-08-20
 * @url https://github.com/DFRobot/DFRobot_SHT3x
*/
#include <DFRobot_SHT3x.h>
#include <WiFi.h>        // Include the Wi-Fi library
#include <ArduinoMqttClient.h>

const char* ssid     = "broker";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "adidas1234";     // The password of the Wi-Fi network

/*!
 * @brief Construct the function
 * @param pWire IIC bus pointer object and construction device, can both pass or not pass parameters, Wire in default.
 * @param address Chip IIC address, two optional addresses 0x44 and 0x45(0x45 in default).
 * @param RST Chip reset pin, 4 in default.
 * @n The IIC address is determined by the pin addr on the chip.
 * @n When the ADR is connected to VDD, the chip IIC address is 0x45.
 * @n When the ADR is connected to GND, the chip IIC address is 0x44.
 */
DFRobot_SHT3x sht3x(&Wire,/*address=*/0x44,/*RST=*/4);

void setup() {

  Serial.begin(9600);
    //Initialize the chip to detect if it can communicate properly.
  while (sht3x.begin() != 0) {
    Serial.println("Failed to initialize the chip, please confirm the chip connection");
    delay(1000);
  }
  
  /**
   * readSerialNumber Read the serial number of the chip
   * @return Return 32-digit serial number
   */
  Serial.print("chip serial number: ");
  Serial.println(sht3x.readSerialNumber());
  /**
   * softReset Send command resets via IIC, enter the chip's default mode single-measure mode, 
   * turn off the heater, and clear the alert of the ALERT pin.
   * @return Read the status register to determine whether the command was executed successfully, 
   * and return true indicates success.
   */
   if(!sht3x.softReset()){
     Serial.println("Failed to reset the chip");
   }

  /**
   * startPeriodicMode Enter cycle measurement mode and set repeatability and read frequency.
   * @param measureFreq Read the eMeasureFrequency_t data frequency.
   * @note  Selectable parameters:
               eMeasureFreq_Hz5,   /**the chip collects data in every 2s
               eMeasureFreq_1Hz,   /**the chip collects data in every 1s 
               eMeasureFreq_2Hz,   /**the chip collects data in every 0.5s 
               eMeasureFreq_4Hz,   /**the chip collects data in every 0.25s 
               eMeasureFreq_10Hz   /**the chip collects data in every 0.1s 
   * @param repeatability Read the repeatability of temperature and humidity data, the default parameter is eRepeatability_High.
   * @note  Optional parameters:
               eRepeatability_High /**In high repeatability mode, the humidity repeatability is 0.10%RH, the temperature repeatability is 0.06°C
               eRepeatability_Medium,/**In medium repeatability mode, the humidity repeatability is 0.15%RH, the temperature repeatability is 0.12°C.
               eRepeatability_Low, /**In low repeatability mode, the humidity repeatability is0.25%RH, the temperature repeatability is 0.24°C
   * @return Read the status of the register to determine whether the command was executed successfully, and return true indicates success
   */          
  if(!sht3x.startPeriodicMode(sht3x.eMeasureFreq_1Hz)){
    Serial.println("Failed to enter the periodic mode");
  }
  Serial.println("------------------Read data in cycle measurement mode-----------------------");

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
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP to the computer

}

void loop() {

  Serial.print("Ambient temperature(°C/F):");
  /**
   * getTemperatureC Get the measured temperature (in degrees Celsius).
   * @return Return the float temperature data.
   */
  Serial.print(sht3x.getTemperatureC());
  Serial.print(" C/");
  /**
   * getTemperatureF Get the measured temperature (in degrees Fahrenheit).
   * @return Return the float temperature data. 
   */
  Serial.print(sht3x.getTemperatureF());
  Serial.print(" F ");
  Serial.print("Relative humidity(%RH):");
  /**
   * getHumidityRH Get measured humidity(%RH)
   * @return Return the float humidity data
   */
  Serial.print(sht3x.getHumidityRH());
  Serial.println(" %RH");

}
