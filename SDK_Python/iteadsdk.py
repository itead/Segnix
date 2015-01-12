'''
ITEAD-OS-SDK Python Interface Module
@author Wu Pengfei
@email pengfei.wu@itead.cc
@date 2014.10.7
'''


###############################################################################
# Import board type
###############################################################################
from itead_config import BOARD_TYPE

_supported_board_type = (
    'BOARD_ITEADUINO_PLUS',
    'BOARD_ITEAD_CORE_EVB',
    'BOARD_RASPBERRY_PI_RV2',
    'BOARD_RASPBERRY_PI_MODEL_BPLUS',
    'BOARD_BEAGLEBONEBLACK',
    )
    
#
# Board type check
#
if   BOARD_TYPE == 'BOARD_ITEADUINO_PLUS':
    pass
elif BOARD_TYPE == 'BOARD_ITEAD_CORE_EVB':
    pass
elif BOARD_TYPE == 'BOARD_RASPBERRY_PI_RV2':
    pass
elif BOARD_TYPE == 'BOARD_RASPBERRY_PI_MODEL_BPLUS':
    pass
elif BOARD_TYPE == 'BOARD_BEAGLEBONEBLACK':
    pass
else:
    print 'Unsupported Board Type = %s .' % (BOARD_TYPE)
    print "You must select one of below:"
    print _supported_board_type
    print "\n"
    

###############################################################################
# Import core modules of ITEAD-OS-SDK
###############################################################################
from iteadcfunction         import *
from iteadserial            import Serial_
from iteadspi               import SPIClass
from iteadwire              import TwoWire


###############################################################################
# About GPIO module stuff
###############################################################################
#
# GPIO MODE
#
OUTPUT = 0x1
INPUT  = 0x0

#
# GPIO VALUE
#
HIGH = 0x1
LOW  = 0x0

#
# GPIO PIN NAME
#
if   BOARD_TYPE == 'BOARD_ITEADUINO_PLUS':
    from BOARD_ITEADUINO_PLUS_GPIO import *
elif BOARD_TYPE == 'BOARD_ITEAD_CORE_EVB':
    from BOARD_ITEAD_CORE_EVB_GPIO import *
elif BOARD_TYPE == 'BOARD_RASPBERRY_PI_RV2':
    from BOARD_RASPBERRY_PI_RV2_GPIO import *
elif BOARD_TYPE == 'BOARD_RASPBERRY_PI_MODEL_BPLUS':
    from BOARD_RASPBERRY_PI_MODEL_BPLUS_GPIO import *
elif BOARD_TYPE == 'BOARD_BEAGLEBONEBLACK':
    from BOARD_BEAGLE_BONE_BLACK_GPIO import *
else:
    print 'No GPIO found.'

###############################################################################
# About analog(ADC) module stuff
###############################################################################
#
# adc pin number
#
if BOARD_TYPE == 'BOARD_BEAGLEBONEBLACK':
    AIN0 = 0
    AIN1 = 1
    AIN2 = 2
    AIN3 = 3
    AIN4 = 4
    AIN5 = 5
    AIN6 = 6
    AIN7 = 7
else:
    print 'No ADC pin found.'
    
###############################################################################
# About Interrupt module stuff
###############################################################################
#
# Please only use edge trigger method. Level trigger methods may arise
# segment fault.
#
GPIO_EINT_CFG_RAISING   = 0x0
GPIO_EINT_CFG_FALLING   = 0x1
GPIO_EINT_CFG_HIGH      = 0x2
GPIO_EINT_CFG_LOW       = 0x3
GPIO_EINT_CFG_DEDGE     = 0x4

#
# An example for GPIO External Interrupt Service Routine
# Actually, ISR is a general fuction. User can write one like this
# for some purpose. 
#
def generalGpioIsr(pin, pdata):
    print '>>>>>>> generalGpioIsr Begin <<<<<<<'
    print 'pin =', pin
    print 'pdata =', pdata
    print '>>>>>>> generalGpioIsr End   <<<<<<<'
    return None


###############################################################################
# About Serial module stuff
###############################################################################
#
# bus number
#
_DEV_UART0 = 0
_DEV_UART1 = 1
_DEV_UART2 = 2
_DEV_UART3 = 3
_DEV_UART4 = 4
_DEV_UART5 = 5
_DEV_UART6 = 6
_DEV_UART7 = 7


# 
# Create objects for available buses
#
if   BOARD_TYPE == 'BOARD_ITEADUINO_PLUS':
    Serial2 = Serial_(_DEV_UART2)
    Serial3 = Serial_(_DEV_UART3)
    Serial4 = Serial_(_DEV_UART4)
    Serial7 = Serial_(_DEV_UART7)
    Serial = Serial4
    
elif BOARD_TYPE == 'BOARD_ITEAD_CORE_EVB':
    Serial4 = Serial_(_DEV_UART4)
    Serial = Serial4
    
elif BOARD_TYPE == 'BOARD_RASPBERRY_PI_RV2':
    Serial0 = Serial_(_DEV_UART0)
    Serial = Serial0
    
elif BOARD_TYPE == 'BOARD_RASPBERRY_PI_MODEL_BPLUS':
    Serial0 = Serial_(_DEV_UART0)
    Serial = Serial0

elif BOARD_TYPE == 'BOARD_BEAGLEBONEBLACK':
    Serial1 = Serial_(_DEV_UART1)
    Serial2 = Serial_(_DEV_UART2)
    Serial4 = Serial_(_DEV_UART4)
    Serial = Serial1    
    
else:
    print 'No Serial bus found'


###############################################################################
# About SPI module stuff
###############################################################################
#
# SPI Data Mode
#
SPI_MODE0 = 0x00            # CPOL = 0, CPHA = 0
SPI_MODE1 = 0x01            # CPOL = 0, CPHA = 1
SPI_MODE2 = 0x02            # CPOL = 1, CPHA = 0
SPI_MODE3 = 0x03            # CPOL = 1, CPHA = 1

#
# SPI Clock Divider
#
SPI_CLOCK_DIV1   = 0x01     # Clock devider = 1 
SPI_CLOCK_DIV2   = 0x02     # Clock devider = 2 
SPI_CLOCK_DIV4   = 0x04     # Clock devider = 4 
SPI_CLOCK_DIV8   = 0x08     # Clock devider = 8 
SPI_CLOCK_DIV16  = 0x10     # Clock devider = 16
SPI_CLOCK_DIV32  = 0x20     # Clock devider = 32
SPI_CLOCK_DIV64  = 0x40     # Clock devider = 64
SPI_CLOCK_DIV128 = 0x80     # Clock devider = 128

#
# SPI Data Order
#
LSBFIRST =	1               # LSB first
MSBFIRST =	0               # MSB first


#
# Device number
#
_DEV_SPI0 = 0
_DEV_SPI1 = 1
_DEV_SPI2 = 2
_DEV_SPI3 = 3


# 
# Create objects for available buses
#
if   BOARD_TYPE == 'BOARD_ITEADUINO_PLUS':
    SPI0 = SPIClass(_DEV_SPI0)
    SPI = SPI0
    
elif BOARD_TYPE == 'BOARD_ITEAD_CORE_EVB':
    SPI0 = SPIClass(_DEV_SPI0)
    SPI = SPI0
    
elif BOARD_TYPE == 'BOARD_RASPBERRY_PI_RV2':
    SPI0 = SPIClass(_DEV_SPI0)
    SPI = SPI0
    
elif BOARD_TYPE == 'BOARD_RASPBERRY_PI_MODEL_BPLUS':
    SPI0 = SPIClass(_DEV_SPI0)
    SPI = SPI0

elif BOARD_TYPE == 'BOARD_BEAGLEBONEBLACK':
    SPI1 = SPIClass(_DEV_SPI1)
    SPI = SPI1    
    
else:
    print 'No SPI bus found.'


###############################################################################
# About TwoWire module stuff
###############################################################################
#
# Device number
#
_DEV_I2C0 = 0
_DEV_I2C1 = 1
_DEV_I2C2 = 2
_DEV_I2C3 = 3

# 
# Create objects for available buses
#
if   BOARD_TYPE == 'BOARD_ITEADUINO_PLUS':
    Wire1 = TwoWire(_DEV_I2C1)
    Wire2 = TwoWire(_DEV_I2C2)
    Wire = Wire1
    
elif BOARD_TYPE == 'BOARD_ITEAD_CORE_EVB':
    Wire1 = TwoWire(_DEV_I2C1)
    Wire2 = TwoWire(_DEV_I2C2)
    Wire = Wire1
    
elif BOARD_TYPE == 'BOARD_RASPBERRY_PI_RV2':
    Wire1 = TwoWire(_DEV_I2C1)
    Wire = Wire1
    
elif BOARD_TYPE == 'BOARD_RASPBERRY_PI_MODEL_BPLUS':
    Wire1 = TwoWire(_DEV_I2C1)
    Wire = Wire1

elif BOARD_TYPE == 'BOARD_BEAGLEBONEBLACK':
    #bb-kernel-3.8.13-bone68 with capmgr 
    #/dev/i2c-1 is I2C2; /dev/I2c-2 is I2C1
    Wire1 = TwoWire(_DEV_I2C2)
    Wire2 = TwoWire(_DEV_I2C1)
    Wire = Wire1    
    
else:
    print 'No I2C(TwoWire) bus found.'


###############################################################################
# The End of this document
###############################################################################
