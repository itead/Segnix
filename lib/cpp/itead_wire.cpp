/********************************************************************
* File		:	itead_wire.cpp
* Desc		:	Provide the implementation of Arduino API on i2c
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

#include <cstddef>
#include <itead_wire.h>

#ifdef BOARD_ITEADUINO_PLUS
TwoWire Wire1(DEV_I2C1);
TwoWire & Wire = Wire1;
TwoWire Wire2(DEV_I2C2);

#elif defined (BOARD_RASPBERRY_RV2)
TwoWire Wire1(DEV_I2C1);
TwoWire & Wire = Wire1;

#endif /* BOARD_ITEADUINO_PLUS */





TwoWire::TwoWire(uint32_t dev)
{
	this->dev = dev;
}

void TwoWire::begin(void)
{
	Wirebegin(dev);
}

void TwoWire::end(void)
{
	Wireend(dev);
}

void TwoWire::beginTransmission(uint8_t addr)
{
	WirebeginTransmission(dev,addr);
}

size_t TwoWire::write(uint8_t val)
{
	Wirewrite(dev,val);
}

uint32_t TwoWire::endTransmission(void)
{
	return WireendTransmission(dev);
}

uint32_t TwoWire::requestFrom(uint8_t addr, uint32_t count)
{
	return WirerequestFrom(dev,addr,count);
}

uint32_t TwoWire::available(void)
{
	return Wireavailable(dev);
}

uint8_t TwoWire::read(void)
{
	return Wireread(dev);
}
