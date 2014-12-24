/**
 * @example Blink.ino
 *
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// We assume that pin 13 has an LED connected on your board.
// give it a name:
//int led = 13;

//test on BeagleBone Black
int led = GPIO1_28;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
 pinMode(led, OUTPUT);
//int ret;
//  pinMode(led, INPUT);
//  ret = digitalRead(led);
//  printf("ret is %d\n",ret);
  digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)
}

// the loop routine runs over and over again forever:
void loop() {

digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);               // wait for a second
}
