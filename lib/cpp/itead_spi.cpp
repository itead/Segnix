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

#if defined(BOARD_ITEADUINO_PLUS) || defined (BOARD_ITEAD_CORE_EVB)
SPIClass SPI0(DEV_SPI0);
SPIClass & SPI = SPI0;
#elif defined (BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
SPIClass SPI0(DEV_SPI0);
SPIClass & SPI = SPI0;
#elif defined (BOARD_BEAGLEBONEBLACK)
SPIClass SPI1(DEV_SPI1);
SPIClass & SPI = SPI1;

#endif 


SPIClass::SPIClass(void)
{
	this->dev = DEV_NONE;
}


SPIClass::SPIClass(uint32_t dev)
{
	this->dev = dev;
}

/** Init SPI bus */
void SPIClass::begin()
{
	SPIbegin(dev);
}

/** Close SPI bus */
void SPIClass::end()
{
	SPIend(dev);
}

/**
 * Set bit order on SPI data lines. 
 * @param order - Only LSBFIRST or MSBFIRST.
 */
void SPIClass::setBitOrder(uint8_t order)
{
#if defined(BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
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

/**
 * Set data mode
 * @param mode - Only SPI_MODE0, SPI_MODE1, SPI_MODE2 or SPI_MODE3
 */
void SPIClass::setDataMode(uint8_t mode)
{
	SPIsetDataMode(dev,mode);
}

/**
 * Set clock divider
 * @param divider - Only SPI_CLOCK_DIVn (n=1, 2, 4, 8, 16, 32, 64, 128)
 */
void SPIClass::setClockDivider(uint8_t divider)
{
    SPIsetClockDivider(dev,divider);
}

/**
 * Send and Receive data
 * @param val - Data to send
 * @return Data received at the same time.
 */
uint8_t SPIClass::transfer(uint8_t val)
{
	return SPItransfer(dev, val);
}
