/**
 * @file Dev_THSensorInterface.h
 *
 * The interface needed by THSensor for getting data. 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/19
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
 
#ifndef __THSENSORINTERFACE_H__
#define __THSENSORINTERFACE_H__

#include <stdint.h>

#include "IoTgo_error.h"

/**
 * @addtogroup THSensor
 * @{
 */

/**
 * Abstract class, declares the unified interface to 
 * read temperature and humidity from Temperature & Humidity Sensor. 
 */
class THSensorInterface
{
public: /* public methods */

    /**
     * Initialize Temperature & Humidity Sensor. 
     * 
     * @retval 0 - success.
     * @retval ERR_INIT_DEVICE_FAILED - if initialization failed!
     */
    virtual int32_t begin(void) = 0;

    /**
     * Read temperature and humidity from device. 
     * 
     * @param temp_c - the pointer storing temperature by Celsius. 
     * @param temp_f - the pointer storing temperature by Fahrenheit. 
     * @param hum - the pointer stroing humidigy in percentage. 
     *
     * @retval 0 - success and the data stored in pointers. 
     * @retval ERR_READ_DEVICE_FAILED - if device cannot be read. 
     *
     * @note temp_c, temp_f and hum can be NULL, if you need no anyone of them.
     */
    virtual int32_t getData(float *temp_c, float *temp_f, float *hum) = 0;

    /**
     * Close Temperature & Humidity Sensor. 
     *
     * @retval 0 - success. 
     * @retval ERR_CLOSE_DEVICE_FAILED - if device cannot be closed. 
     */
    virtual int32_t end(void) = 0;
};

/** @} */

#endif /* #ifndef __THSENSORINTERFACE_H__ */
