#include "config.h" 
#include "Instruction.h"

#define TEMP_VARIANCE 1.5
#define HUM_VARIANCE 2

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, password);
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");

  Console << TinyConsole::green << "Connected to " << ssid << "IP address: " << WiFi.localIP() << endl;

  /* Start http server */
  server.begin();
  Serial.println("http server started");

  /* Start MQTT broker */ 
  broker.begin();
  Console << "Broker ready : " << WiFi.localIP() << " on port " << PORT << endl;

  /* subscribe to all DATA ACQ. topics */
  mqtt_a.setCallback(onPublishTopic);
  mqtt_a.subscribe(sensor0_topic);
  mqtt_a.subscribe(sensor1_topic);
  mqtt_a.subscribe(RPI_temp_topic);
  mqtt_a.subscribe(RPI_rh_topic);

}

void loop() {
  broker.loop();
  mqtt_a.loop();

    static auto next_send = millis();
    if (millis() > next_send)
    {
        next_send += 10000;

        if (not mqtt_a.connected())
        {
            Serial << millis() << ": Not connected to broker" << endl;
            return;
        }

        
        /* publish shutdown instructions for DATA_ACQ */
        //Serial << "~~~~~~~~~~~> Publishing a data_acq/node0/shutdown value: " << endl;
        //mqtt_a.publish("data_acq/node0/shutdown", "no");
        //mqtt_a.publish("data_acq/node0/shutdown", "no 2");
        
        /* compute control systems algorithm */
        Serial << sensor0_reading << endl;
        Serial << sensor1_reading << endl;
        Serial << TEMP_TARGET << endl;
        Serial << HUM_TARGET << endl;
        Serial << TEMP_VARIANCE << endl;
        Serial << HUM_VARIANCE << endl;
        instruction curr_inst = compute_inst(sensor0_reading, sensor1_reading, TEMP_TARGET, HUM_TARGET, TEMP_VARIANCE, HUM_VARIANCE); 
        Serial << "HEAT " << curr_inst.heater_instruction << endl;
        Serial << "HUMI " << curr_inst.humidifier_instruction << endl;
        /* publish control instructions to PWR/APP CTRL*/
        mqtt_a.publish(control0_topic, curr_inst.heater_instruction); 
        mqtt_a.publish(control1_topic, curr_inst.humidifier_instruction); 
        
        /* publish FAKE sensor data to RPI */ 
        //Serial << sensor0_reading << endl; 
        //Serial << sensor1_reading << endl; 
        /* fake sensor data */ 
        /*
        sensor0_reading += change;
        sensor1_reading += change;
        if(sensor0_reading == 100.0 || sensor1_reading == 100.0 || sensor0_reading == 90.0 || sensor1_reading == 90.0)
          change = change * -1;
        */ 
        //mqtt_a.publish(control0_topic, "On");
        //mqtt_a.publish(control1_topic, "On");
        
    }
}
