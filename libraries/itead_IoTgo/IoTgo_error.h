/**
 * @file IoTgo_error.h
 *
 * Define macros indicate error type. 
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
 
#ifndef __IOTGOERROR_H__
#define __IOTGOERROR_H__

/**
 * @addtogroup Error
 * @{
 */

/**
 * IoTgo errors. 
 */
enum IoTgoError
{
    /* General */
    ERR_INVALID_PARAMETER       = -1,   /**< Invalid parameters */

    /* Net */
    ERR_NO_RESPONSE             = -100, /**< No response from server */
    ERR_NO_EXPECT               = -101, /**< No contents expected in response */
    
    /* Device */
    ERR_NO_DEVICES_AVAILABLE    = -200, /**< No devices available */
    ERR_INIT_DEVICE_FAILED      = -201, /**< Initialize device failed */
    ERR_READ_DEVICE_FAILED      = -202, /**< Read device failed */
    ERR_WRITE_DEVICE_FAILED     = -203, /**< Write device failed */
    ERR_CLOSE_DEVICE_FAILED     = -204, /**< Close device failed */
};

/** @} */

#endif /* #ifndef __IOTGOERROR_H__ */
