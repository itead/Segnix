#! /usr/bin/env python
from iteadsdk import *
LED = PG9
pinMode(LED, OUTPUT)
while True:
    digitalWrite(LED, HIGH)
    delay(1000)
    digitalWrite(LED, LOW)
    delay(1000)
