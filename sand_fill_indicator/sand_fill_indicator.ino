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
#include <TFT.h>
#include <SPI.h>
#include <SD.h>

#define cs 8
#define rs 10
#define rst 9

 int r = 255, g = 255, b = 0;
 int counter = 0;
 
 char c[3];
 int text_x_pos = 0;
 int text_y_pos = 0;
TFT TFTscreen = TFT(cs,rs,rst);

void setup() {
  // set up the LCD's number of columns and rows:
  //lcd.begin(128, 160);
  // Print a message to the LCD.
  //lcd.print("Yooooo");

  TFTscreen.begin();
  TFTscreen.background(b,g,r);
  
  
  TFTscreen.setTextSize(2);

}

void loop() {
  int redRandom = random(0, 255);
 int greenRandom = random (0, 255);
 int blueRandom = random (0, 255);
  TFTscreen.stroke(redRandom,greenRandom,blueRandom);
  if(counter > 100){
    counter = 0;
  }
  if(counter > 0 && counter < 33){

    TFTscreen.background(0,200,0);
  }
  if(counter > 33 && counter < 66){

    TFTscreen.background(0,255,255);
  } 
  if(counter > 66 && counter < 100){
    TFTscreen.background(0,0,255);
  }
  
  TFTscreen.text("Reid's Cube",text_x_pos,text_y_pos);
  String count = String(counter);
  count.toCharArray(c,3);
  TFTscreen.text(c,64,80);
  counter ++;
  if(text_x_pos < 128){
    text_x_pos += 5;
  }
  if(text_x_pos == 125){
    text_x_pos = 0;
    if(text_y_pos < 160){
      text_y_pos += 5;
      
    }
    if(text_y_pos == 155)
    {
      text_y_pos =0;
    }
  }
  delay(50);
}
