#! /usr/bin/env python
#coding=utf-8

'''
/**
 * @file IoTgo.py
 *
 * Provide the implementation of ITEAD IoT API
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/10
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
'''

import httplib, json

_check_code = "invalid"
_deviceid = "invalid"
_apikey = "invalid"
_host = ""

_DEVICE_TYPE_DIY     = 0
_DEVICE_TYPE_PRODUCT = 1

def setHost(domain_name):
	global _host
	_host = domain_name

def _post(body):
    url = "/api/http"
    extra_headers = {"Content-type": "application/json"}
    ret = {}
    
    conn = httplib.HTTPConnection(_host)
    conn.request("POST", url, body, extra_headers)
    response = conn.getresponse()
    ret["status"] = response.status
    ret["reason"] = response.reason
    ret["version"] = response.version
    ret["data"] = response.read()
    conn.close()
    return ret
    

def init(deviceid, check_code, device_type = _DEVICE_TYPE_DIY):
    global _check_code
    global _deviceid
    global _apikey
    
    _check_code = check_code
    _deviceid = deviceid
    _apikey = "invalid"
    
    if device_type == _DEVICE_TYPE_DIY:
        _apikey = _check_code
        return "\"apikey\":" + "\"" + _apikey + "\""
        
    elif device_type == _DEVICE_TYPE_PRODUCT:
        body = {}
        body["action"] = "register"
        body["apikey"] = _check_code
        body["deviceid"] = _deviceid
        body = json.dumps(body)
        ret = _post(body)
        
        if ret["status"] == 200:
            data = ret["data"]
            error_index = data.find("\"error\":0")
            if error_index != -1:
                apikey_index = data.find("\"apikey\":")
                _apikey = data[apikey_index + 10:apikey_index + 10 + 35 + 1]
                return "\"apikey\":" + "\"" + _apikey + "\""
                
    return "failed"
  
def query(params):
    body = {}
    body["action"] = "query"
    body["apikey"] = _apikey
    body["deviceid"] = _deviceid
    body["params"] = params
    
    body = json.dumps(body)
    #print body
    ret = _post(body)
    #for i in ret: print "%s=" % i, ret[i]
    if ret["status"] == 200:
        return ret["data"]
    else:
        return "failed"
    
def update(params):
    body = {}
    body["action"] = "update"
    body["apikey"] = _apikey
    body["deviceid"] = _deviceid
    body["params"] = params
    
    body = json.dumps(body)
    #print body
    ret = _post(body)
    #for i in ret: print "%s=" % i, ret[i]
    if ret["status"] == 200:
        return ret["data"]
    else:
        return "failed"

def main():
    setHost("iotgo.iteadstudio.com")
    print init("0000000001", "9bed6c2c-fbc6-4400-9ef8-087f9f0d996e")
    print update({"state":"on"})
    print query(["state"])
    print update({"state":"off"})
    print query(["state"])
    
if __name__ == "__main__": main()