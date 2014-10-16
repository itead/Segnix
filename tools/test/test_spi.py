#! /usr/bin/env python

from iteadsdk import *


def test_spi(val):
    SPI.begin()
    SPI.setBitOrder(MSBFIRST)
    SPI.setDataMode(SPI_MODE0)
    SPI.setClockDivider(SPI_CLOCK_DIV8)
    ret = SPI.transfer(val)
    SPI.end()
    print "ret =", hex(ret)
    

###############################################################################
# main function
###############################################################################
def main():
    print 'BOARD_TYPE =', BOARD_TYPE
    for i in range(256):
        test_spi(i)
        
    print "test spi exit normally!"
    
if __name__ == '__main__': main()


