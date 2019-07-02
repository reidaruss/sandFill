/*
 * File:        sand_fill_indicator.ino
 * 
 * Description: This program is for measuring the fill level of the sand hopper for 
 *              the water jet and throw an indication when a certain minimum 
 *              threshold is breached.
 *              
 * Author(s):   Reid Russell <rarussell@bastiansolutions.com>             
 * Copyright:   Copyright (C) Bastian Solutions, LLC - All Rights Reserved
 * NOTICE: Unauthorized copying of this file, via any medium is strictly prohibited.
 * 
 */
// include the library code:
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "HX711.h"


#define TFT_DC 10
#define TFT_CS 8
#define TFT_reset 9

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = A4;
const int LOADCELL_SCK_PIN = A5;

HX711 scale;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_reset);


void setup() {
  Serial.begin(9600);
  tft.begin();  
  tft.setRotation(1);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(5000);
  scale.tare();
  

}

void loop() {
  
  testText();
  

  delay(1000);
  
}



unsigned long testText(){
  
  int x = scale.get_value(10);
  
  //unsigned long start = micros();
  tft.setCursor(10,60);
  
  if(x > 15 && x <= 30)
  {
    tft.fillScreen(ILI9341_YELLOW);
  }
  if(x <= 15)
  {
    tft.fillScreen(ILI9341_RED);
    tft.setTextColor(ILI9341_BLACK);
  }
  tft.setTextSize(3);
  tft.print("Measured Weight: ");
  tft.setCursor(130, 110);
  tft.setTextSize(5);
  tft.print(x);
  tft.print("%");
  return micros();

}
