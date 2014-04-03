/********************************************************************
* File		:	itead_spi.cpp
* Desc		:	Provide the implementation of Arduino API on SPI
* Author	: 	Wu Pengfei
* Company	:	ITEAD Intelligent Systems Co.,Ltd.(http://imall.iteadstudio.com/)
* Date		:	2014/3/11
* History	:	Version		Modified by		Date		What
*				v1.0		Wu Pengfei		2014/3/11	Create
********************************************************************/

/*
 * (C) Copyright 2014
 * ITEAD Intelligent Systems Co., Ltd. <http://imall.iteadstudio.com>
 * Wu Pengfei<wupangfee@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

#include <itead_spi.h>

SPIClass SPI0(DEV_SPI0);
SPIClass & SPI = SPI0;

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
	SPIsetBitOrder(dev,order);
}

void SPIClass::setDataMode(uint8_t mode)
{
	SPIsetDataMode(dev,mode);
}

void SPIClass::setClockDivider(uint8_t)
{
	/* do nothing */
}

uint8_t SPIClass::transfer(uint8_t val)
{
	return SPItransfer(dev, val);
}
