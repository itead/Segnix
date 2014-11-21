/**
 * @file Dev_Light.h
 *
 * Light API of IoTgo (iotgo.iteadstudio.com)
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/11
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
 
#ifndef __DEV_LIGHT_H__
#define __DEV_LIGHT_H__

#include <itead.h>

#include "IoTgo.h"

/**
 * @addtogroup Light 
 * @{ 
 */

/**
 * Indicate the state of light device. 
 */
enum LightState
{
    LIGHT_STATE_OFF   = 0, /**< light off */
    LIGHT_STATE_ON    = 1, /**< light on */
};

/**
 * Light, subclass of IoTgo, provides simple methods to control light device. 
 * 
 */
class Light: public IoTgo
{
public: /* public methods */  
    Light(uint16_t light_pin);
    int32_t sync(void);
    int32_t getState(int32_t *state);
    int32_t on(void);
    int32_t off(void); 
    
private: /* private methods */
    int32_t setState(int32_t state);

private: /* private static data members */
    static const char *STATE[];
    static const char *VALUE_ON[];
    static const char *VALUE_OFF[];
    
private:
    uint16_t light_pin;
};

/** @} */

#endif /* #ifndef __DEV_LIGHT_H__ */
