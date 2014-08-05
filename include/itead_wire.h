/**
 * @file itead_wire.h
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
#ifndef __ITEAD_WIRE_H__
#define __ITEAD_WIRE_H__

#include <stdint.h>
#include <itead_config.h>

#ifdef __cplusplus
extern "C"{
#endif

/* 
 * Only define number of devices.
 */
#define DEV_I2C0              	0
#define DEV_I2C1              	1
#define DEV_I2C2              	2
#define DEV_I2C3              	3

uint32_t		Wirebegin(uint32_t dev);
uint32_t		WirebeginTransmission(uint32_t dev, uint8_t addr);
uint32_t		Wirewrite(uint32_t dev, uint8_t val);
uint32_t		WireendTransmission(uint32_t dev);
uint32_t		WirerequestFrom(uint32_t dev, uint8_t addr, uint32_t count);
uint32_t		Wireavailable(uint32_t dev);
uint8_t			Wireread(uint32_t dev);
uint32_t		Wireend(uint32_t dev);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class TwoWire
{
private:
	uint32_t dev;
public:
	TwoWire(uint32_t dev);
	void begin(void);
	void end(void);
	void beginTransmission(uint8_t addr);
	size_t write(uint8_t val);
	uint32_t endTransmission(void);
	uint32_t requestFrom(uint8_t addr, uint32_t count);
	uint32_t available(void);
	uint8_t read(void);
};

/* The first device has a aliase compitable with Arduino */
extern TwoWire & Wire;
#ifdef BOARD_ITEADUINO_PLUS
extern TwoWire Wire1;
extern TwoWire Wire2;

#elif defined (BOARD_RASPBERRY_RV2)
extern TwoWire Wire1;

#endif /* BOARD_ITEADUINO_PLUS */


#endif

#endif
