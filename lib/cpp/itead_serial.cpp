/**
 * @file itead_serial.cpp
 *
 * Provide the implementation of Arduino API on Serial
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
#include <itead_serial.h>
#include <itead_utility.h>

#ifdef BOARD_ITEADUINO_PLUS
Serial_ Serial2(DEV_UART2);
Serial_ Serial3(DEV_UART3);
Serial_ Serial4(DEV_UART4);
Serial_ Serial7(DEV_UART7);
Serial_ & Serial = Serial4;
#elif defined (BOARD_ITEAD_CORE_EVB)
Serial_ Serial4(DEV_UART4);
Serial_ & Serial = Serial4;
#elif defined (BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
Serial_ Serial0(DEV_UART0);
Serial_ & Serial = Serial0;
#elif defined (BOARD_BEAGLEBONEBLACK)
Serial_ Serial1(DEV_UART1);
Serial_ Serial2(DEV_UART2);
Serial_ Serial4(DEV_UART4);
Serial_ & Serial = Serial1;

#endif 

Serial_::Serial_()
{
	this->dev = DEV_NONE;
	started = false;
}

Serial_::Serial_(uint32_t dev)
{
	this->dev = dev;
	started = false;
}

/** Overload bool operator for if(Serial). */
Serial_::operator bool() {
	return started;
}

/**
 * Check if there are unread data. 
 * 
 * @retval 0 - no unread data
 * @retval positive - the length of unread data in byte.
 */
int Serial_::available(void)
{
	return (int)Serialavailable(dev);
}

/**
 * Start serial with baudrate. 
 * 
 * @param baud_count - baudrate for communication
 */
void Serial_::begin(uint32_t baud_count)
{
	Serialbegin(dev, baud_count);
	started = true;
}

/** Close serial */
void Serial_::end(void)
{
	Serialend(dev);
	started = false;
}

/**
 * Empty the buffer of tx and rx instead waiting for outgoing completed.
 */
void Serial_::flush(void)
{
	Serialflush(dev);
}

/**
 * Read one byte from received buffer. 
 *
 * After this call, the value of available() will be decreased by 1 byte.
 * 
 * @return The first unread data
 */
uint8_t Serial_::read(void)
{
	if (Serialavailable(dev)) {
		return Serialread(dev);
	} else {
		return -1;
	}
}

/**
 * Write one byte to serial. 
 *
 * @param c - data to write
 * @retval 1 - success
 * @retval 0 - fail
 */
size_t Serial_::write(uint8_t c)
{
	Serialwrite(dev,c);
}


