/**
 * @example temp_hum_sensor.ino
 * 
 * @par Show how to use API of class THSensor
 *
 * The sequence:
 *      -# create an object. 
 *      -# connect to internet for later communication. 
 *      -# initialize the device(atctually, connect to sever). 
 *      -# now, it can be update or query the parameter. 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/14
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

/* Include all device classes supported by IoTgo Library */
#include <IoTgo_device.h>
#include <SHT1x.h>

/* 
 * An identifier of device which has been created and belongs to
 * a user registed to IoTgo platform. The first two letters indicates
 * the categories: 
 *  00 : Customized device which can has as many as you want
 *       parameters and the corresponding values.
 *  01 : Switch which has at least one parameter: 
 *       "switch" with value "on" or "off". 
 *  02 : Light which has at least one parameter:
 *       "light" with value "on" or "off". 
 *  03 : THSensor which has at least two parameters: 
 *       "temperature" with value "xx.xx" in Celsius, 
 *       "humidity" with value "xx.xx" in persentage. 
 */
#define THSENSOR_ID            "0300000001"

/*
 * If you have a finished product, a check code or apikey-like code or 
 * factory apikey will be found around it. We are naming the code 
 * "THSENSOR_CHECK_CODE". Here is empty. 
 */
#define THSENSOR_CHECK_CODE    ""

/* An unique identifier of user registed on IoTgo platform */
#define THSENSOR_APIKEY        "9bed6c2c-fbc6-4400-9ef8-087f9f0d996e"

/* The domain name of IoTgo platform host */
#define IOT_DOMAIN_NAME     "iotgo.iteadstudio.com"

#define THSENSOR_DATA_PIN    (PB19)
#define THSENSOR_CLOCK_PIN   (PB18)

SHT1x sht1x(THSENSOR_DATA_PIN, THSENSOR_CLOCK_PIN);
THSensor th(&sht1x);

void setup()
{
    const char *apikey;
    th.begin();
    th.setHost(IOT_DOMAIN_NAME);
    printf("Connecting device to server...\n");
    apikey = th.init(THSENSOR_ID, THSENSOR_APIKEY);
    //apikey = th.init(THSENSOR_ID, THSENSOR_CHECK_CODE, DEVICE_PRODUCT);
    printf("apikey = %s\n", apikey);
    
    printf("setup done.\n");
}

void loop()
{
    static int32_t counter = 0;
    printf("\ncounter = %d\n", ++counter);

    int32_t ret;
    float temp_c;
    float temp_f;
    float hum;
    
    ret = th.sync();
    if (ret)
    {
        printf("ERROR: sync() = %d\n", ret);
    }
    else
    {
        printf("sync successfully!\n");
    }

    ret = th.getAll(&temp_c, &temp_f, &hum);
    if (ret)
    {
        printf("ERROR: getAll() = %d\n", ret);
    }
    else
    {
        printf("TempC = %.2f", temp_c);
        printf(", TempF = %.2f", temp_f);
        printf(", Hum = %.2f\n", hum);
    }
    
    ret = th.getTemperatureC(&temp_c);
    if (ret)
    {
        printf("ERROR: getTemperatureC() = %d\n", ret);
    }
    else
    {
        printf("TempC = %.2f\n", temp_c);
    }
    
    ret = th.getTemperatureF(&temp_f);
    if (ret)
    {
        printf("ERROR: getTemperatureF() = %d\n", ret);
    }
    else
    {
        printf("TempF = %.2f\n", temp_f);
    }

    ret = th.getHumidity(&hum);
    if (ret)
    {
        printf("ERROR: getHumidity() = %d\n", ret);
    }
    else
    {
        printf("Hum = %.2f\n", hum);
    }

    delay(1000);
}
