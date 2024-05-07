#include <Wire.h> 

#define ANSWERSIZE 1

const int node_count = 3;
const int SLAVE_ADDR[node_count] = {4,5,7};
int distance[node_count] = {};

void setup() {
  Wire.begin(21,22);
  Serial.begin(115200);

}

void loop() {
  for(int i = 0;i < node_count; i++){
    distance[i] = receive_i2c(SLAVE_ADDR[i]);
  }
  Serial.println("----------------------------------");
  delay(1000);
}

int receive_i2c(int node_addr){
  Serial.println("Receive data node_addr: " +String(node_addr));
  // Read response from Slave---------
  Wire.requestFrom(node_addr,ANSWERSIZE);

  // Add characters to string
  String text_response = "";
  byte response[ANSWERSIZE];
  while (Wire.available()) {
    for (byte i=0;i<ANSWERSIZE;i++) {
       response[i] = (byte)Wire.read();
    }
  } 
  for (byte i = 0; i < ANSWERSIZE; i++) {
    Serial.print(char(response[i]));
    text_response += (char)response[i];
  }
  Serial.println(" m");
  return text_response.toInt();
}