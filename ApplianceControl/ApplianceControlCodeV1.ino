//Appliance Control Node Code Version 1
//v1: Read On/Off signal from Serial Monitor (Ex, Dx) from x = 0-3 
//Can we write this better by decreasing the input size
const unsigned int NUM_RELAYS = 4;
const char ENABLE = 'E';
const char DISABLE = 'D';

unsigned int relay_on[NUM_RELAYS];
const unsigned int relay_map[NUM_RELAYS] = {22,23,21,19};

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
  
}

void loop() {
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

  Serial.println();
    

}
