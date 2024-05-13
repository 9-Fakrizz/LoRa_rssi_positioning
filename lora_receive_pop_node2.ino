#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>

// Define Slave I2C Address
#define SLAVE_ADDR 5
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

void setup() 
{
  Wire.begin(SLAVE_ADDR);
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");
 
  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
 
  while (!LoRa.begin(444E6))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xA5);
  Serial.println("LoRa Initializing OK!");
  //Function to run when data received from master
  Wire.onReceive(receiveEvent);
  //Function to run when data requested from master
  Wire.onRequest(requestEvent); 
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
    if (packetSize && LoRa.packetSnr() <= 9.00) // filter Snr  && LoRa.packetSnr() >= 8.00
    {
      sampleRSSI[packet_count] = LoRa.packetRssi();
      Serial.print("  ");
      Serial.print(sampleRSSI[packet_count]);
      Serial.println(" Snr: " + String(LoRa.packetSnr()));
      packet_count += 1;
    }
  }
  Serial.println();

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
  // Serial.print(" WINNER : ");
  Serial.println(winner_value);

  if (winner_value <= 94)
      distance = 0;
  else if (winner_value >= 95 && winner_value <= 94)
      distance = 1;
  else if (winner_value >= 95 && winner_value <= 96)
      distance = 2;
  else if (winner_value >= 97 && winner_value <= 99)
      distance = 3;
  else if (winner_value >= 100 && winner_value <= 101)
      distance = 4;
  else if (winner_value >= 102 && winner_value <= 104)
      distance = 5;
  else if (winner_value >= 105 && winner_value <= 106)
      distance = 6;
  else if (winner_value >= 107)
      distance = 7;

  Serial.print(" distance : ");
  Serial.println(distance);
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
  /*444mhz
  -88 is zero set when we found -85 then we turn on LED 
  0 is 87-88 (this time is 1.65m)
  90-91 is 1.0m
  94-96 is 2.0m
  95-97 is 3.0m
  96-97 is 4.0m
  98-99 is 5.0m
  99-101 is 6.0m
  102++ is 7.0m++
  */
