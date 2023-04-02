#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>


const char *ssid = "broker";
const char *password = "adidas1234";

IPAddress local_IP(192,168,1,119);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid,password) ? "Ready" : "Failed!");
  //WiFi.softAP(ssid);
  //WiFi.softAP(ssid, password, channel, hidden, max_connection)
  
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  Serial.print("[Server Connected] ");
  Serial.println(WiFi.softAPIP());
  // print number of clients/stations connected to the broker 

  delay(500);
}
