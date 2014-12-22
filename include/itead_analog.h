/**
 * @file itead_analog.h
 *
 * Provide the implementation of Arduino API analogRead()
 *
 * @author  Wu xiaoyu (email:<mars.wu@itead.cc>)
 * @date    2014/12/17
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#ifndef __ITEAD_ANALOG_H__
#define __ITEAD_ANALOG_H__

#include <stdint.h>
#include <itead_config.h>

#ifdef __cplusplus
extern "C"{
#endif

/* Only define number of channels */
#define DEV_AIN0        0
#define DEV_AIN1        1
#define DEV_AIN2        2
#define DEV_AIN3        3
#define DEV_AIN4        4
#define DEV_AIN5        5
#define DEV_AIN6        6
#define DEV_AIN7        7

extern uint32_t analogread(uint32_t dev);
    
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

/**
 * Provide simple methods to read adc result. 
 *
 * @ingroup gpio
 */
class AnalogClass 
{
private:
	uint32_t dev;

public:
    AnalogClass(void);
	AnalogClass(uint32_t dev);

    uint32_t analogRead(void);
};

/**
 * @ingroup gpio
 * @defgroup analog_instance Predefined Instance of class AnalogClass
 * @details Accroding to different boards, user can use instances below directly,
 *  because these have been defined in Segnix. 
 * 
 * @par On BeagleBoneBlack
 * - Analog - A reference to Analog0
 * - Analog0 - Analog0(AIN0)
 * - Analog1 - Analog1(AIN1)
 * - Analog2 - Analog2(AIN2)
 * - Analog3 - Analog3(AIN3)
 * - Analog4 - Analog4(AIN4)
 * - Analog5 - Analog5(AIN5)
 * - Analog6 - Analog6(AIN6)
 * - Analog7 - Analog7(AIN7)
 */

extern AnalogClass & Analog;

#if defined (BOARD_BEAGLEBONEBLACK)
extern AnalogClass Analog0;
extern AnalogClass Analog1;
extern AnalogClass Analog2;
extern AnalogClass Analog3;
extern AnalogClass Analog4;
extern AnalogClass Analog5;
extern AnalogClass Analog6;
extern AnalogClass Analog7;
#endif 

#endif

#endif
