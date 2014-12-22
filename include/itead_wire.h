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

/**
 * Provide simple methods to manipulate I2C bus. 
 * 
 * @ingroup i2c
 */
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

/**
 * @ingroup i2c
 * @defgroup i2c_instance Predefined Instance of class TwoWire
 * @details Accroding to different boards, user can use instances below directly,
 *  because these have been defined in Segnix. 
 * 
 * @par On Iteaduino Plus and ITEAD CORE EVB
 * - Wire - A reference to Wire1
 * - Wire1 - I2C1(SDA:PB19, SCK:PB18)
 * - Wire2 - I2C2(SDA:PB21, SCK:PB20)
 * 
 * @par On Raspberry Rv2 and Raspberry Pi Model B+
 * - Wire - A reference to Wire1
 * - Wire1 - I2C1(SDA:GPIO2, SCK:GPIO3)
 *
 * @par On BeagleBoneBlack
 * - Wire - A reference to Wire1
 * - Wire1 - I2C1(SDA:GPIO0_4, SCK:GPIO0_5)
 * - Wire2 - I2C2(SDA:GPIO0_12, SCK:GPIO0_13)
 */

extern TwoWire & Wire;

#if defined(BOARD_ITEADUINO_PLUS) || defined(BOARD_ITEAD_CORE_EVB)
extern TwoWire Wire1;
extern TwoWire Wire2;

#elif defined (BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
extern TwoWire Wire1;

#elif defined (BOARD_BEAGLEBONEBLACK)
extern TwoWire Wire1;
extern TwoWire Wire2;

#endif 


#endif

#endif
