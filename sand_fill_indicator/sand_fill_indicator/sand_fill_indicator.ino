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
// Libraries for LCD control and Load Cell input
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "HX711.h"

//Defining pins for LCD
#define TFT_DC 10
#define TFT_CS 8
#define TFT_reset 9

// HX711 circuit wiring (Load Cells)
const int LOADCELL_DOUT_PIN = A4;
const int LOADCELL_SCK_PIN = A5;
const long r_weight_measured = 1443688;
const long r_weight = 155;

//Pin for solenoid on/off control
const int solenoid_pin = 2;
const int buzzer_pin = 4;
const int button_pin = 3;
//Store current background color
int color = 0;

//Boolean to track if solenoid has fired (to prevent burnout)
bool fired = false;

//Initializing the load cells
HX711 scale;

//intializing the LCD
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_reset);


void setup() {
  //Set baud rate
  
  pinMode(solenoid_pin, OUTPUT);
  Serial.begin(9600);
  
  
  tft.begin();  
  tft.setRotation(3);
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  //delay for 5 seconds to make sure the load cells have enough time to initialize.
  delay(5000);
  
  pinMode(buzzer_pin, OUTPUT);
  pinMode(button_pin, INPUT);


}

void loop() {
  
  check_scale();
  delay(1000);
  
}


unsigned long print_text()
{
  //The below code deals with all other text on the screen and lines/shapes.
  tft.setCursor(10,10);
  tft.setTextSize(4);
  tft.print("SAND TANK");
  tft.drawRect(0,0,250,50,ILI9341_PURPLE);
  tft.setCursor(5,75);
  tft.setTextSize(3);
  tft.print("Level of Fill:");
  tft.drawRect(75,100,160,60,ILI9341_PURPLE);
  tft.drawLine(0, 160, 320, 160, ILI9341_BLACK);
}


void yellow_buzzer()
{
  digitalWrite(buzzer_pin, HIGH);
  delay(500);
  digitalWrite(buzzer_pin, LOW);
  delay(500);
  digitalWrite(buzzer_pin, HIGH);
  delay(500);
  digitalWrite(buzzer_pin, LOW);
}

void red_buzzer()
{
  digitalWrite(buzzer_pin, HIGH);
  delay(1000);
  digitalWrite(buzzer_pin, LOW);
  delay(500);
  digitalWrite(buzzer_pin, HIGH);
  delay(1000);
  digitalWrite(buzzer_pin, LOW);
  delay(500);
  digitalWrite(buzzer_pin, HIGH);
  delay(1000);
  digitalWrite(buzzer_pin, LOW);
  delay(500);
  digitalWrite(buzzer_pin, HIGH);
  delay(1000);
  digitalWrite(buzzer_pin, LOW);
}

void paused_buzzer()
{
  digitalWrite(buzzer_pin, HIGH);
  delay(5000);
  digitalWrite(buzzer_pin, LOW);
  delay(1000);
  digitalWrite(buzzer_pin, HIGH);
  delay(5000);
  digitalWrite(buzzer_pin, LOW);
}

//check_scale() deals with calculating percentage from the raw load cell output and determining
//what color the background screen should be and what words are wrtiten on the screen.
unsigned long check_scale()
{
  float max_w = 155.0;
  //set x to the raw output from load cells from 10 reading averaged together.
  float x = scale.get_value(10);
  //Higher weight = lower value (negative) so make all values positive.
  x *= -1;
  //Divide x by the scale value to get weight.
  x = x/9314.116;
  x += 104.2;
  int y  = (x/max_w)*100;
  

  //Check if percentage is above 30%, if so make screen Green
  if(y > 30)
  {
    if(color != 1)
    {
      tft.fillScreen(ILI9341_GREEN);
      tft.setTextColor(ILI9341_BLACK);
      color = 1;
      print_text();
      digitalWrite(solenoid_pin, HIGH);
    }
    if(fired = true)
    {
      fired = false;
    }
    
  }

  //If percentage is between 15% and 30%, make screen yellow
  if(y > 15 && y <= 30)
  {
    if(color != 2)
    {
      tft.fillScreen(ILI9341_YELLOW);
      tft.setTextColor(ILI9341_BLACK);
      color = 2;
      print_text();
      digitalWrite(solenoid_pin, HIGH);
      yellow_buzzer();
    }
    if(fired = true)
    {
      fired = false;
    }
    
  }

  //if percentage is less than 15% make screen red and print "Fill Sand" at the bottom.
  if(y <= 15)
  {
    if(color != 3)
    {
        tft.fillScreen(ILI9341_RED);
        tft.setTextColor(ILI9341_BLACK);
        color = 3;
        print_text();
    
    }
    if(y <= 5)
      {
        tft.setCursor(60,200);
        tft.setTextSize(5);
        tft.drawRect(0,165,320,240,ILI9341_RED);
        tft.fillRect(0,165,320,240,ILI9341_RED);
        tft.print("PAUSED");
        if(fired == false)
        {
          digitalWrite(solenoid_pin, LOW);
          delay(3000);
          digitalWrite(solenoid_pin, HIGH);
          fired = true;
          paused_buzzer();
        }
     

      }
    else
      {
        tft.setCursor(30, 200);
        tft.setTextSize(5);
        tft.drawRect(0,165,320,240,ILI9341_RED);
        tft.fillRect(0,165,320,240,ILI9341_RED);
        tft.print("FILL SAND");
        digitalWrite(solenoid_pin, HIGH);
        red_buzzer();
      }
  }
  if(color == 1)
  {
    tft.fillRect(75,100,160,60,ILI9341_GREEN);
  }
  else if(color == 2)
  {
    tft.fillRect(75,100,160,60,ILI9341_YELLOW);
  }
  else if(color == 3)
  {
    tft.fillRect(75,100,160,60,ILI9341_RED);
  }
  //tft.drawRect(75,100,160,60,ILI9341_PURPLE);
  tft.setCursor(100, 120);
  tft.setTextSize(5);
  tft.print(y);
  tft.print("%");
  return micros();

}
