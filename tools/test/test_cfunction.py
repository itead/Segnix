#!/usr/bin/env python

import time
import thread

from iteadsdk import *

def ledThread(pin, cnt, interval):
    counter = 0
    
    pinMode(pin, OUTPUT)
    while True:
        digitalWrite(pin, LOW)
        delay(int(interval * 1000))
        digitalWrite(pin, HIGH)
        delayMicroseconds(int(interval * 1000000))
        
        counter += 1
        if counter == cnt:
            print 'ledThread exit normally.'
            thread.exit_thread()

def testTimeGPIO():
    print 'micros() =', micros()
    print 'millis() =', millis()
    thread.start_new_thread(ledThread,(PG9, 10, 0.25))
    analogWrite(PG8, 200)
    time.sleep(6)
    
    
def gpioIsr(id):
    print 'Python Function: gpioIsr begin'
    print 'id =', id
    print 'Python Function: gpioIsr end'
    return (id * 2)

def testInterruptModule():
    #set_callback(gpioIsr)
    #test_callback()
    EPIN = PI15
    pdata = ['ITEAD-SDK-Py', 'ET']
    attachInterrupt(EPIN, generalGpioIsr, pdata, GPIO_EINT_CFG_FALLING)
    print 'Waiting interrupt...'
    #time.sleep(5) # error
    delay(5000)
    #time.sleep(5) # error
    for i in range(10):
        print 'i=', i
        if i ==  3:
            detachInterrupt(EPIN)
            #noInterrupt()
            
        if i == 5:
            attachInterrupt(EPIN, generalGpioIsr, pdata, GPIO_EINT_CFG_FALLING)
            #interrupt()
            
        pdata[1] = str(i)
        delay(2000)
    detachInterrupt(EPIN)
    print 'testInterruptModule done'

###############################################################################
# main function
###############################################################################
def main():
    print 'BOARD_TYPE =', BOARD_TYPE
    testTimeGPIO()
    #testInterruptModule()
    
if __name__ == '__main__': main()
