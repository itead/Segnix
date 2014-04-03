/********************************************************************
* File		:	itead_serial.cpp
* Desc		:	Provide the implementation of Arduino API on Serial
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
#include <itead_serial.h>


Serial_ Serial2(DEV_UART2);
Serial_ Serial3(DEV_UART3);
Serial_ Serial4(DEV_UART4);
Serial_ Serial7(DEV_UART7);

Serial_ & Serial = Serial2;

Serial_::Serial_(uint32_t dev)
{
	this->dev = dev;
	started = false;
}

/*
 * always success.
 */
Serial_::operator bool() {
	return started;
}


int Serial_::available(void)
{
	return (int)Serialavailable(dev);
}

void Serial_::begin(uint32_t baud_count)
{
	Serialbegin(dev, baud_count);
	started = true;
}

void Serial_::end(void)
{
	Serialend(dev);
	started = false;
}

/* 
 * flush has different beheavor from Arduino API
 * Empty the buffer of tx and rx instead waiting for outgoing completed.
 */
void Serial_::flush(void)
{
	Serialflush(dev);
}

size_t Serial_::print(const char* string)
{
	return Serialprint(dev,string);
}

size_t Serial_::println(const char* string)
{
	return Serialprintln(dev,string);
}

int Serial_::read(void)
{
	if (Serialavailable(dev)) {
		return Serialread(dev);
	} else {
		return -1;
	}
}

size_t Serial_::write(uint8_t c)
{
	Serialwrite(dev,c);
}


