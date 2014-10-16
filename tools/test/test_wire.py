#! /usr/bin/env python

from iteadsdk import *

EEPROM_ADDR	= 0x50
I2C = Wire

def test_wire(val):
    I2C.begin()
    
    for i in range(16):
        I2C.beginTransmission(EEPROM_ADDR)
        I2C.write(i * 16)
        for k in range(16):
            I2C.write(val)
        I2C.endTransmission()
        delay(10)
    
    I2C.beginTransmission(EEPROM_ADDR)
    I2C.write(0)
    I2C.endTransmission()
    delay(10)
    
    I2C.requestFrom(EEPROM_ADDR,256)
    count = I2C.available()
    print "Available =%d" % (count)
    
    print "Read:"
    for i in range(count):
        print hex(I2C.read()),
        if (i+1)%16 == 0:
            print ""
    print "\nRead done"
    
    count = I2C.available()
    print "Available =%d" % (count)
            
    I2C.end()
    

###############################################################################
# main function
###############################################################################
def main():
    print 'BOARD_TYPE =', BOARD_TYPE
    for i in range(5):
        test_wire(i)
        
    print "test wire exit normally!"
    
if __name__ == '__main__': main()


