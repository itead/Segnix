#!/usr/bin/env python

import time

from iteadsdk import *

###############################################################################
# main function
###############################################################################
def main():
    print 'BOARD_TYPE =', BOARD_TYPE
    Serial.begin(115200)
    Serial.write(ord('A'));
    Serial.write(ord('z'));
    Serial.printstr("ABCD\n")
    Serial.printstr("1234\n")
    varInt = 1257
    varFloat = 12.56
    Serial.printstr(hex(varInt) + "\n")
    Serial.printstr(str(float(varFloat)))
    time.sleep(0.1)
    #Serial.flush()
    
    read_str = ''
    while Serial.available() > 0:
        read_str += chr(Serial.read())
    print "read_str=[%s]" % (read_str)
    
    Serial.end()
    
    print "main exit normally!"

if __name__ == '__main__': main()
