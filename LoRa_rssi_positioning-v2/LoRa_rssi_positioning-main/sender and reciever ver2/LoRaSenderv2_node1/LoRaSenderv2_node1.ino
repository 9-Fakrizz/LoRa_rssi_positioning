#include <LoRa.h>
#include <SPI.h>
#include <WiFi.h>                                 
#include <Wire.h>

#define led 14
#define ss 5
#define rst 4
#define dio0 2
 
int sample_count = 10;
long timer = 0;
 
void setup() 
{
  Serial.begin(115200); 
  while (!Serial);

  Serial.print("Setting up Access Point ... ");

  pinMode(led,OUTPUT);
  digitalWrite(led, LOW);

  Serial.println("LoRa Sender");
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
  digitalWrite(led, HIGH);
  Serial.println("Sending packet!");
  timer = millis();
  for(int i = 0; i < sample_count; i++){
    LoRa.beginPacket();   //Send LoRa packet to receiver
    LoRa.print("1hi");
    LoRa.endPacket();
    delay(20);
  }
  digitalWrite(led, LOW);
  timer = millis() - timer;
  Serial.println("Timer : "+String(timer));

  delay(1340);
}

