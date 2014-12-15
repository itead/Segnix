/**
 * @file itead_wire.cpp
 *
 * Provide the implementation of Arduino API on i2c
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
#include <cstddef>
#include <itead_wire.h>

#if defined(BOARD_ITEADUINO_PLUS) || defined(BOARD_ITEAD_CORE_EVB)
TwoWire Wire1(DEV_I2C1);
TwoWire & Wire = Wire1;
TwoWire Wire2(DEV_I2C2);
#elif defined (BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
TwoWire Wire1(DEV_I2C1);
TwoWire & Wire = Wire1;
#elif defined (BOARD_BEAGLEBONEBLACK)
TwoWire Wire1(DEV_I2C1);
TwoWire & Wire = Wire1;
#endif 


TwoWire::TwoWire(uint32_t dev)
{
	this->dev = dev;
}

/** Open and init i2c bus. */
void TwoWire::begin(void)
{
	Wirebegin(dev);
}

/** Close i2c bus. */
void TwoWire::end(void)
{
	Wireend(dev);
}

/**
 * Set slave addr and empty the tx buffer
 * @param addr - 7bits address of slave.
 */
void TwoWire::beginTransmission(uint8_t addr)
{
	WirebeginTransmission(dev,addr);
}

/**
 * Write val to buffer. DO NOT transmit.
 * @return The number of data written to txbuffer.
 */
size_t TwoWire::write(uint8_t val)
{
	Wirewrite(dev,val);
}

/**
 * Actually, transmit tx buffer data to slave.
 * @retval 0 - success
 * @retval 1 - fail
 */
uint32_t TwoWire::endTransmission(void)
{
	return WireendTransmission(dev);
}

/**
 * Read data from addr to rx buffer. 
 * @param addr - Address of i2c device. 
 * @param count - The length of data you want to read. 
 * @return The number of avaiable data actually.
 */
uint32_t TwoWire::requestFrom(uint8_t addr, uint32_t count)
{
	return WirerequestFrom(dev,addr,count);
}

/**
 * Get the number of data unread in rx buffer. 
 * @return	: the number of unread data (equivalent to available data).
 */
uint32_t TwoWire::available(void)
{
	return Wireavailable(dev);
}

/**
 * Read a date type of uint8_t from rx buffer. 
 * @return The first unread byte in buffer.
 */
uint8_t TwoWire::read(void)
{
	return Wireread(dev);
}
