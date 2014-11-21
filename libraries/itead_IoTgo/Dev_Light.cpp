/**
 * @file Dev_Light.cpp
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

#include "Dev_Light.h"

const char *Light::STATE[]        = {"light", NULL};
const char *Light::VALUE_ON[]     = {"on",    NULL};
const char *Light::VALUE_OFF[]    = {"off",   NULL};

/**
 * Constructor of Light. 
 * 
 * @param light_pin - the gpio pin number to control the light. 
 *  HIGH level means on and LOW to off. 
 */
Light::Light(uint16_t light_pin)
{
    this->light_pin = light_pin;
    pinMode(this->light_pin, OUTPUT);
}

/**
 * Read the state from server and turn on/off the light. 
 * 
 * We always assume that the state from server is newest. 
 * 
 * @retval 0 - success
 * @retval ERR_NO_RESPONSE - no response package from server. 
 * @retval ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Light::sync(void)
{
    int32_t state = 0;
    int32_t ret = 0;
    ret = getState(&state);
    if (state == LIGHT_STATE_ON)
    {
        digitalWrite(light_pin, HIGH);
    }
    else if (state == LIGHT_STATE_OFF)
    {
        digitalWrite(light_pin, LOW);
    }
    else
    {
        IoTgo_perror("getState() error: ret = %d", ret);
    }
    return ret;
}

/**
 * Read state from server. 
 *
 * @param state - the pointer to store the state which is one of @ref LIGHT_STATE_OFF
 *  and @ref LIGHT_STATE_ON. 
 * 
 * @retval 0 - success. 
 * @retval ERR_INVALID_PARAMETER - state is invalid. 
 * @retval ERR_NO_RESPONSE - no response package from server. 
 * @retval ERR_NO_EXPECT - no state information expected in response package from server. 
 *
 * @par Usage: a snippet following
 * @code
 *      ret = obj.getState(&state);
 *      if (state == LIGHT_STATE_ON)
 *      {
 *          digitalWrite(light_pin, HIGH);
 *      }
 *      else if (state == LIGHT_STATE_OFF)
 *      {
 *          digitalWrite(light_pin, LOW);
 *      }
 *      else
 *      {
 *          // do something else
 *      }
 * @endcode
 *
 * @see LightState Light::sync
 */
int32_t Light::getState(int32_t *state)
{
    const char *response;
    const char *str_error;
    const char *str_params;
    const char *str_state_on;
    const char *str_state_off;
    
    if (state == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }
    
    response = query(STATE); /* {"error":0,"params":{"state":"on"}} {"error":0,"params":{"state":"off"}}*/
    if (response != NULL)
    {
        str_error       = strstr(response, "\"error\":0");
        str_params      = strstr(response, "\"params\":");
        str_state_on    = strstr(response, "\"light\":\"on\"");
        str_state_off   = strstr(response, "\"light\":\"off\"");
        
        if (str_error && str_params)
        {
            if (str_state_on && !str_state_off)
            {
                *state = LIGHT_STATE_ON;
                return 0;
            }
            else if (!str_state_on && str_state_off)
            {
                *state = LIGHT_STATE_OFF;
                return 0;
            }
            else
            {
                return ERR_NO_EXPECT;
            }
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
 * Update the specific state to server. 
 * 
 * @param state - the state you want to update, only LIGHT_STATE_ON or LIGHT_STATE_OFF. 
 * 
 * @retval 0 - success
 * @retval ERR_INVALID_PARAMETER - state is invalid value. 
 * @retval ERR_NO_RESPONSE - no response package from server. 
 * @retval ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Light::setState(int32_t state)
{
    const char **value;
    const char *response;
    const char *str_error;
    
    if (state == LIGHT_STATE_ON)
    {
        value = VALUE_ON;
    }
    else if (state == LIGHT_STATE_OFF)
    {
        value = VALUE_OFF;
    }
    else
    {
        return ERR_INVALID_PARAMETER;
    }
    
    response = update(STATE, value);
    if (response != NULL)
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
 * Turn on the light and update the state to server. 
 *
 * @retval 0 - success
 * @retval ERR_NO_RESPONSE - no response package from server. 
 * @retval ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Light::on(void)
{
    int32_t ret;
    ret = setState(LIGHT_STATE_ON) ;
    if (ret == 0)
    {
        digitalWrite(light_pin, HIGH);        
    }
    return ret;
}

/**
 * Turn off the light and update the state to server. 
 *
 * @retval 0 - success
 * @retval ERR_NO_RESPONSE - no response package from server. 
 * @retval ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Light::off(void)
{
    int32_t ret;
    ret = setState(LIGHT_STATE_OFF) ;
    if (ret == 0)
    {
        digitalWrite(light_pin, LOW);        
    }
    return ret;
}

