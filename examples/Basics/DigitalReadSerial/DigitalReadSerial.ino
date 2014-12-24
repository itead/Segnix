/**
 * @example DigitalReadSerial.ino
 *
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

/*
  DigitalReadSerial
 Reads a digital input on pin 9, prints the result to the serial monitor 
 
 This example code is in the public domain.
 */

// We assume that digital pin 9 has a pushbutton attached to it. Give it a name:
int pushButton = 9;

// test on BeagleBone Black
pushButton = GPIO1_16;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int buttonState = digitalRead(pushButton);
  // print out the state of the button:
  Serial.println(buttonState);
  delay(100);        // delay in between reads for stability
}



