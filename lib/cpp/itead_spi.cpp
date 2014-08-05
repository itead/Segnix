/**
 * @file itead_spi.cpp
 *
 * Provide the implementation of Arduino API on SPI
 * 
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/3/11
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#include <itead_spi.h>
#include <itead_utility.h>
#include <itead_global.h>

#ifdef BOARD_ITEADUINO_PLUS
SPIClass SPI0(DEV_SPI0);
SPIClass & SPI = SPI0;

#elif defined (BOARD_RASPBERRY_RV2)
SPIClass SPI0(DEV_SPI0);
SPIClass & SPI = SPI0;

#endif /* BOARD_ITEADUINO_PLUS */


SPIClass::SPIClass(void)
{
	this->dev = DEV_NONE;
}


SPIClass::SPIClass(uint32_t dev)
{
	this->dev = dev;
}

void SPIClass::begin()
{
	SPIbegin(dev);
}

void SPIClass::end()
{
	SPIend(dev);
}

void SPIClass::setBitOrder(uint8_t order)
{
#ifdef BOARD_RASPBERRY_RV2
    if( order == MSBFIRST) {
	SPIsetBitOrder(dev,order);
    } else {
        #if 0 // disabled by wpf
        sdkerr("RPi DO NOT support LSBFIRST on SPI\nPlease reverse bit order in software by hand\n");
        #endif
    }
#else
	SPIsetBitOrder(dev,order);
#endif
}

void SPIClass::setDataMode(uint8_t mode)
{
	SPIsetDataMode(dev,mode);
}

void SPIClass::setClockDivider(uint8_t divider)
{
	/* do nothing */
    SPIsetClockDivider(dev,divider);
}

uint8_t SPIClass::transfer(uint8_t val)
{
	return SPItransfer(dev, val);
}
