#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>

// Define Slave I2C Address
#define SLAVE_ADDR 4
// Define Slave answer size
#define ANSWERSIZE 1

#define ss 5
#define rst 4
#define dio0 2
 
const int sample_index = 5;
int sampleRSSI[sample_index] ={};
int pre_freq = 0;
int last_freq = 0;
int winner_value;
int distance = 0;
int slide_value = 1;

void setup() 
{
  Wire.begin(SLAVE_ADDR);
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");
 
  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
 
  while (!LoRa.begin(433E6))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xA5);
  Serial.println("LoRa Initializing OK!");
  //Function to run when data requested from master
  Wire.onRequest(requestEvent); 
  //Function to run when data received from master
  Wire.onReceive(receiveEvent);
}
 
void loop() 
{
  int sampleRSSI[sample_index] ={};
  int pre_freq = 0;
  int winner_value;
  int packet_count=0;
  
  // get rssi sample
  while(packet_count < sample_index){
    // Get RSSI of the connected network
    int packetSize = LoRa.parsePacket();    // try to parse packet
    if (packetSize ) // filter Snr && LoRa.packetSnr() <= 9.00
    {
      sampleRSSI[packet_count] = LoRa.packetRssi();
      // Serial.print("  ");
      // Serial.print(sampleRSSI[packet_count]);
      // Serial.print(" Snr: " + String(LoRa.packetSnr()));
      packet_count += 1;
    }
  }
  //Serial.println();

  // calculate to find population.
  for(int i = 0; i < sample_index; i++){
    int last_freq = 0;
    for(int j = i; j < sample_index; j++){
      if(sampleRSSI[i] == sampleRSSI[j])last_freq +=1;
    }
    if(last_freq > pre_freq) winner_value = sampleRSSI[i];
    pre_freq = last_freq;
  }
  winner_value = abs(winner_value);
  Serial.print(" WINNER : ");
  Serial.println(winner_value);

  if (winner_value == 85 + slide_value)
    distance = 0;
  else if (winner_value >= 87+slide_value && winner_value <= 91+slide_value)
      distance = 1;
  else if (winner_value >= 92+slide_value && winner_value <= 93+slide_value)
      distance = 2;
  else if (winner_value == 94+slide_value)
      distance = 3;
  else if (winner_value >= 95+slide_value && winner_value <= 96+slide_value)
      distance = 4;
  else if (winner_value >= 97+slide_value && winner_value <= 98+slide_value)
      distance = 5;
  else if (winner_value >= 99+slide_value && winner_value <= 100+slide_value)
      distance = 6;
  else if (winner_value >= 101+slide_value && winner_value <= 103+slide_value)
      distance = 7;
  else if (winner_value > 103+slide_value)
      distance = 8;

  Serial.print(" DISTANCE : ");
  Serial.print(distance);
  Serial.println("  m ");

}
void receiveEvent(int) {
 
  // Read while data received
  byte x;
  while (0 < Wire.available()) {
    x = Wire.read();
  }
  // Print to Serial Monitor
  Serial.println("Receive event");
  Serial.println("x : "+String(x));
  
}
 
void requestEvent() {
 
  // Setup byte variable in the correct size
  byte response[ANSWERSIZE];
  String answer = String(distance);
  // Format answer as array
  for (byte i=0;i<ANSWERSIZE;i++) {
     response[i] = (byte)answer.charAt(i);
  }
  
  // Send response back to Master
  Wire.write(response,sizeof(response));
  Serial.println("Request event");
}
