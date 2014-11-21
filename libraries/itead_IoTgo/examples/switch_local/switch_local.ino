/**
 * @example switch_local.ino
 * 
 * @par Show how to use API of class Switch
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
#define SWITCH_ID            "0100000003"

/*
 * If you have a finished product, a check code or apikey-like code or 
 * factory apikey will be found around it. We are naming the code 
 * "SWITCH_CHECK_CODE". Here is empty. 
 */
#define SWITCH_CHECK_CODE    ""

/* An unique identifier of user registed on IoTgo platform */
#define SWITCH_APIKEY        "9bed6c2c-fbc6-4400-9ef8-087f9f0d996e"

/* 
 * The SSID (more generally, WiFi's name) for accessing to internet.
 * Maybe you need to replace "ITEAD" with yours. 
 */
#define WIFI_SSID           "ITEAD"

/* 
 * The SSID's password
 * Is it the same to yours? if not, change it.
 */
#define WIFI_PASS           "12345678"

/*
 * IP address or domain name of IoTgo platform servers. 
 * Maybe you need to change it.
 */
#define IOT_SERVER          "172.16.7.6"

#define SWITCH_PIN           (13)

Switch sw(SWITCH_PIN);

void setup()
{
    const char *apikey;
    sw.setServer(IOT_SERVER);
    if (!sw.connectWiFi(WIFI_SSID, WIFI_PASS))
    {
        Serial.println("connectWiFI error and halt...");
        while(1);
    }

    Serial.println("Connecting device to server...");
    apikey = sw.init(SWITCH_ID, SWITCH_APIKEY);
    //apikey = sw.init(SWITCH_ID, SWITCH_CHECK_CODE, DEVICE_PRODUCT);
    Serial.print("apikey = ");
    Serial.println(apikey);    
    
    Serial.println("setup done.");
}

void loop()
{
    static int32_t counter = 0;
    Serial.print("\ncounter = ");
    Serial.println(++counter);
    int32_t ret;

    ret = sw.on();
    if(ret == 0)
    {
        Serial.println("on");
    }
    else
    {
        Serial.println("on() err!");
    }
    
    delay(1000);
    
    ret = sw.off();
    if(ret == 0)
    {
        Serial.println("off");
    }
    else
    {
        Serial.println("off() err!");
    }
    
    delay(1000);
}
