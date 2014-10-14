#! /usr/bin/env python

from iteadsdk import Wire, delay

EEPROM_ADDR	= 0x50

def test_wire(val):
    Wire.begin()
    
    for i in range(16):
        Wire.beginTransmission(EEPROM_ADDR)
        Wire.write(i * 16)
        for k in range(16):
            Wire.write(val)
        Wire.endTransmission()
        delay(10)
    
    Wire.beginTransmission(EEPROM_ADDR)
    Wire.write(0)
    Wire.endTransmission()
    delay(10)
    
    Wire.requestFrom(EEPROM_ADDR,256)
    count = Wire.available()
    print "Available =%d" % (count)
    
    print "Read:"
    for i in range(count):
        print hex(Wire.read()),
        if (i+1)%16 == 0:
            print ""
    print "\nRead done"
    
    count = Wire.available()
    print "Available =%d" % (count)
            
    Wire.end()
    

###############################################################################
# main function
###############################################################################
def main():
    for i in range(5):
        test_wire(i)
        
    print "test wire exit normally!"
    
if __name__ == '__main__': main()


