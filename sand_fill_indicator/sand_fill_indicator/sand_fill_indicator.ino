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
const long r_weight_measured = 1443688;
const long r_weight = 155;

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
  
  float x = scale.get_value(10);
  x *= -1;
  x = x/9314.116;
  //unsigned long start = micros();
  
  if(x > 30)
  {
    tft.fillScreen(ILI9341_GREEN);
    tft.setTextColor(ILI9341_BLACK);
  }
  if(x > 15 && x <= 30)
  {
    tft.fillScreen(ILI9341_YELLOW);
  }
  if(x <= 15)
  {
    tft.fillScreen(ILI9341_RED);
    tft.setTextColor(ILI9341_BLACK);
    tft.setCursor(40, 200);
    tft.setTextSize(4);
    tft.print("FILL SAND");
  }
  tft.setCursor(10,60);
  tft.setTextSize(3);
  tft.print("Measured Weight: ");
  tft.setCursor(80, 110);
  tft.setTextSize(4);
  tft.print(x);
  tft.print(" lbs");
  tft.drawLine(0, 150, 320, 150, ILI9341_BLACK);
  return micros();

}
