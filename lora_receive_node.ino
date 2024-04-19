#include <LoRa.h>
#include <SPI.h>
 
#define ss 5
#define rst 14
#define dio0 2
 
const int sample_index = 3;
int sampleRSSI[sample_index] ={0};
int pre_freq = 0;
int last_freq = 0;
int winner_value;
//int packet_count=0;

void setup() 
{
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
}
 
void loop() 
{
  int sampleRSSI[sample_index] ={0};
  int pre_freq = 0;
  int winner_value;
  int packet_count=0;
  
  // get rssi sample
  while(packet_count < sample_index){
    // Get RSSI of the connected network
    int packetSize = LoRa.parsePacket();    // try to parse packet
    if (packetSize) 
    {
      sampleRSSI[packet_count] = LoRa.packetRssi();
      // Serial.print("  ");
      // Serial.print(sampleRSSI[packet_count]);
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
   last_freq = 0;
 }
//  Serial.print(" WINNER : ");
//  Serial.println(winner_value);

 float distance = 0.0;
 // pridict centimeter by using linear equation
 distance = (winner_value + 92.4424) / -0.0234 ;
 //Serial.print(" Distance : ");
 Serial.print(distance/100.0);
 Serial.println("m ");

}
