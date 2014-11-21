/**
 * @file Dev_THSensor.cpp
 *
 * THSensor API of IoTgo (iotgo.iteadstudio.com)
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/17
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include "Dev_THSensor.h"

/**
 * Constructor. 
 * 
 * @param sensor - the pointer to an object of classes which implement
 *  THSensorInterface.  
 */
THSensor::THSensor(THSensorInterface *sensor)
{
    if (sensor)
    {
        this->sensor = sensor;
    }
    else
    {
        IoTgo_perror("Parameter sensor is NULL!");
    }
}

/**
 * Initialize Temperature & Humidity Sensor. 
 * 
 * @retval 0 - success.
 * @retval ERR_INIT_DEVICE_FAILED - if initialization failed!
 */
int32_t THSensor::begin(void)
{
    return sensor->begin();
}

/**
 * Close Temperature & Humidity Sensor. 
 *
 * @retval 0 - success. 
 * @retval ERR_CLOSE_DEVICE_FAILED - if device cannot be closed. 
 */
int32_t THSensor::end(void)
{
    return sensor->end();
}
    
/**
 * Read temperature and humidity from device then push them to server. 
 *
 * @retval 0 - success
 * @retval ERR_NO_RESPONSE - if no response from server. 
 * @retval ERR_NO_EXPECT - if no content expected in response. 
 * @retval ERR_NO_DEVICES_AVAILABLE - if no devices available. 
 */
int32_t THSensor::sync(void)
{
    char str_temp_c[8] = {'\0'};
    char str_hum[8] = {'\0'};
    char str_tmp[8] = {'\0'};
    const char *params[] = {"temperature", "humidity", NULL};
    const char *values[] = {str_temp_c, str_hum, NULL};
    const char *response;
    float temp_c;
    float temp_f;
    float hum;
    int32_t ret;
    const char *str_error;
    int32_t len;
    
    ret = getAll(&temp_c, &temp_f, &hum);
    if (ret)
    {
#ifdef DEBUG
        IoTgo_debug("getAll() = %d", ret);
#endif
        return ERR_NO_DEVICES_AVAILABLE;
    }
    
#ifdef DEBUG    
    IoTgo_debug("temp_c=%.2f", temp_c);
    IoTgo_debug("hum=%.2f", hum);
#endif

    temp_c += 0.005;
    hum += 0.005;
    
    snprintf(str_tmp, sizeof(str_tmp), "%d", (int)(temp_c*100));
    len = strlen(str_tmp);
    strncpy(str_temp_c, str_tmp, len-2);
    strcat(str_temp_c, ".");
    strcat(str_temp_c, &str_tmp[len-2]);
    
    snprintf(str_tmp, sizeof(str_tmp), "%d", (int)(hum*100));
    len = strlen(str_tmp);
    strncpy(str_hum, str_tmp, len-2);
    strcat(str_hum, ".");
    strcat(str_hum, &str_tmp[len-2]);
    
#ifdef DEBUG
    IoTgo_debug("str_temp_c=%s", str_temp_c);
    IoTgo_debug("str_hum=%s", str_hum);
#endif
    
    response = update(params, values);
    
    if(response)
    {  
        str_error = strstr(response, "\"error\":0");
        if (str_error)
        {
            return 0;
        }
        else
        {
            return ERR_NO_EXPECT;
        }
    }
    else
    {
        return ERR_NO_RESPONSE;
    }
}

/** 
 * Read temperature from device by Celsius. 
 * 
 * @param temp_c - the pointer to store temperature by Celsius. 
 *
 * @retval 0 - success. 
 * @retval ERR_INVALID_PARAMETER - if temp_c is NULL. 
 * @retval ERR_READ_DEVICE_FAILED - if device cannot be read. 
 */
int32_t THSensor::getTemperatureC(float *temp_c)
{
    return temp_c
            ? 
                sensor->getData(temp_c, NULL, NULL) 
            : 
                ERR_INVALID_PARAMETER;
}

/** 
 * Read temperature from device by Fahrenheit. 
 * 
 * @param temp_f - the pointer to store temperature by Fahrenheit. 
 *
 * @retval 0 - success. 
 * @retval ERR_INVALID_PARAMETER - if temp_c is NULL. 
 * @retval ERR_READ_DEVICE_FAILED - if device cannot be read. 
 */
int32_t THSensor::getTemperatureF(float *temp_f)
{
    return temp_f
            ? 
                sensor->getData(NULL, temp_f, NULL) 
            : 
                ERR_INVALID_PARAMETER;
}

/** 
 * Read humidity from device. 
 * 
 * @param hum - the pointer to store humidity. 
 *
 * @retval 0 - success
 * @retval ERR_INVALID_PARAMETER - if hum is NULL. 
 * @retval ERR_READ_DEVICE_FAILED - if device cannot be read. 
 */
int32_t THSensor::getHumidity(float *hum)
{
    return hum
            ? 
                sensor->getData(NULL, NULL, hum) 
            : 
                ERR_INVALID_PARAMETER;
}

/** 
 * Read temperature and humidity from device. 
 * 
 * @param temp_c - the pointer to store temperature by Celsius. 
 * @param temp_f - the pointer to store temperature by Fahrenheit. 
 * @param hum - the pointer to store humidity. 
 *
 * @retval 0 - success
 * @retval ERR_INVALID_PARAMETER - if any parameter is NULL. 
 * @retval ERR_READ_DEVICE_FAILED - if device cannot be read. 
 */
int32_t THSensor::getAll(float *temp_c, float *temp_f, float *hum)
{
    
    return temp_c && temp_f && hum
            ? 
                sensor->getData(temp_c, temp_f, hum) 
            : 
                ERR_INVALID_PARAMETER;
} 
