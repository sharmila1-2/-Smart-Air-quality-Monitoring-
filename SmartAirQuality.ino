#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET    4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define sensor A0
#define DHTPIN 2
#define DHTTYPE DHT11

int gasLevel = 0;
String quality ="";
DHT dht(DHTPIN, DHTTYPE);

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setFont();
  display.setCursor(0, 20);
  display.println("Temp  :");
  display.setCursor(80, 43);
  display.println(t);
  display.setCursor(114, 43);
  display.println("C");
  display.setCursor(0, 56);
  display.println("RH   :");
  display.setCursor(80, 56);
  display.println(h);
  display.setCursor(114, 56);
  display.println("%");
}

void air_sensor()
{
  gasLevel = analogRead(sensor);
  
  if(gasLevel<181){
    quality = " GAS CONDITION: GOOD!";
  }
  else if (gasLevel >181 && gasLevel<225){
    quality = " GAS CONDITION: Poor!";
  }
  else if (gasLevel >225 && gasLevel<300){
    quality = " GAS CONDITION:Very bad!";
  }
  else if (gasLevel >300 && gasLevel<350){
    quality = " GAS CONDITION:TOXIC!";
  }
  else{
    quality = " GAS CONDITION:GOOD";
  }

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(1,5);
  display.setFont();
  display.println("Air quality:");
  
  display.setTextSize(1);
  display.setCursor(20,23);
  display.setFont(&FreeMonoOblique9pt7b);
  display.println(quality);
}

void setup() {
  Serial.begin(9600);
  pinMode(sensor,INPUT);
  dht.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(2);
  display.setCursor(50, 0);
  display.println("Air");
  display.setTextSize(1);
  display.setCursor(23, 20);
  display.println("Qulaity monitor");
  display.display();
  delay(1200);
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(10, 10);
  display.println("BY HUC");
  display.display();
  delay(1000);
  display.clearDisplay();
}

void loop() {
  display.clearDisplay();
  air_sensor();
  sendSensor();
  display.display();
}