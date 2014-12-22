/**
 * @file itead_analog.cpp
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
#include <cstddef>
#include <itead_analog.h>
#include <itead_utility.h>

#if defined (BOARD_BEAGLEBONEBLACK)
AnalogClass Analog0(DEV_AIN0);
AnalogClass Analog1(DEV_AIN1);
AnalogClass Analog2(DEV_AIN2);
AnalogClass Analog3(DEV_AIN3);
AnalogClass Analog4(DEV_AIN4);
AnalogClass Analog5(DEV_AIN5);
AnalogClass Analog6(DEV_AIN6);
AnalogClass Analog7(DEV_AIN7);
AnalogClass & Analog = Analog0;
#else
AnalogClass AnalogNONE;
AnalogClass & Analog = AnalogNONE;
#endif 

AnalogClass::AnalogClass()
{
	this->dev = DEV_NONE;
}

AnalogClass::AnalogClass(uint32_t dev)
{
	this->dev = dev;
}

/**
 * get adc input via channel. 
 * 
 * @param dev - the adc input channel.
 * @return the adc result if possible , -1 if failed.
 * @note The result is an integer value. Its range depends on the ADC's precision.
 *       for example :[0 to 4095] --- 12-bit ADC
 *                    [0 to 1023] --- 10-bit ADC
 */
uint32_t AnalogClass::analogRead(void)
{
    return (uint32_t)analogRead(dev);
}



