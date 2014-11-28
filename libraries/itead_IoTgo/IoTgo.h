/**
 * @file IoTgo.h
 *
 * API of IoTgo (iotgo.iteadstudio.com)
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
 
#ifndef __ITEADIOT_H__
#define __ITEADIOT_H__

#include <Python.h>
#include <string.h>

#include "IoTgo_debug.h"
#include "IoTgo_error.h"


/**
 * @addtogroup IoTgo
 * @{
 */

/**
 * Constants of IoTgo platform. 
 *
 * Maybe these are useless for users. 
 */
enum IoTgoConstant
{
    IOT_BUFFER_SIZE     = 1024, /**< the length of http request */
    APIKEY_LEN          = 36,   /**< the length of apikey */
    DEVICE_ID_LEN       = 10,   /**< the length of device id */
};

/**
 * Indicates the type of device. 
 *
 * Used in method: @ref IoTgo::init. If the deviceid is created by developers on website
 * of IoTgo platform, the type is DEVICE_DIY. Else DEVICE_PRODUCT. 
 */
enum IoTgoDeviceType
{
    DEVICE_DIY      = 0,    /**< DIY deviceid */
    DEVICE_PRODUCT  = 1,    /**< Product deviceid */
};


/** 
 * IoTgo provides some simple API for device of IoT. 
 */
class IoTgo
{
public: /* public methods */
    IoTgo(void);
    ~IoTgo(void);
    void setHost(const char *domain_name);
    const char *init(const char *device_id, const char *apikey_like, 
        IoTgoDeviceType device_type = DEVICE_DIY);
    const char *query(const char *params[]);
    const char *update(const char *params[], const char *values[]);

private: /* static data shared with all objects */
    static int refcount;
    static int imported;
    static PyObject *pymod_name;
    static PyObject *pymod;

private: /* private datas */
    char buffer[IOT_BUFFER_SIZE];
};

/** @} */

#endif /* #ifndef __ITEADIOT_H__ */
