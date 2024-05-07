#include <LoRa.h>
#include <SPI.h>
#include <WiFi.h>                                   // needed to connect to setup an accesspoint
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define led 14
#define ss 5
#define rst 4
#define dio0 2
 
int counter = 0;
 
void setup() 
{
  Serial.begin(115200); 
  while (!Serial);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setRotation(2);
  Serial.print("Setting up Access Point ... ");
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(1, 8);
  display.println(F("LoRa Sender node "));
  display.display();      // Show initial text
  delay(100);

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

  display.println(F("LoRa Initializing OK!"));
  display.println(F("NODE 1"));
  display.display();      // Show initial text
  delay(100);
}
 
void loop() 
{
  digitalWrite(led, LOW);
  Serial.print("Sending packet: ");
  Serial.println(counter);
 
  LoRa.beginPacket();   //Send LoRa packet to receiver
  LoRa.print("1hi");
  LoRa.endPacket();
  counter++;
  
  digitalWrite(led, HIGH);
  delay(500);
}

