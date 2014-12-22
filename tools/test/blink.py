#! /usr/bin/env python
from iteadsdk import *
LED = GPIO1_21
pinMode(LED, OUTPUT)
while True:
    digitalWrite(LED, HIGH)
    delay(1000)
    digitalWrite(LED, LOW)
    delay(1000)
