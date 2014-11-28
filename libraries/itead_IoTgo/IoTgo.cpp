/**
 * @file IoTgo.cpp
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

#include "IoTgo.h"

int IoTgo::refcount = 0;
int IoTgo::imported = 0;
PyObject *IoTgo::pymod_name = NULL;
PyObject *IoTgo::pymod = NULL;


/**
 * Constructor. 
 */
IoTgo::IoTgo(void)
{
    memset(buffer, '\0', sizeof(buffer));
    
    refcount++;
    if (imported == 0)
    {
        Py_Initialize();
        PySys_SetArgv(0,(char **)"");
        pymod_name = PyString_FromString("IoTgo_module");
        pymod = PyImport_Import(pymod_name);
        if (pymod == NULL)
        {
            IoTgo_debug("Import module failed!");    
        }
        Py_XDECREF(pymod_name);
        imported = 1;
        IoTgo_debug("Module initialize");
    }
}

/**
 * Destructor. 
 */
IoTgo::~IoTgo(void)
{
    refcount--;
    if(refcount == 0)
    {
        Py_XDECREF(pymod);
        Py_Finalize();
        imported = 0;
        IoTgo_debug("Module finalize\n");
    }
}

/**
 * Set the domain name of IoTgo platform host. 
 * 
 * @param domain_name - guess what!
 *
 * @warning User must call this before init. 
 */
void IoTgo::setHost(const char *domain_name)
{
    PyObject *pyfun;
    PyObject *pyargs;
    PyObject *pyret;
    
    pyfun = PyObject_GetAttrString(pymod, "setHost");
    if (pyfun && PyCallable_Check(pyfun))
    {
        pyargs = Py_BuildValue("(s)", domain_name);
        pyret = PyObject_CallObject(pyfun, pyargs);
        Py_XDECREF(pyargs);
        if (pyret == NULL)
        {
            IoTgo_perror("setHost failed!");
        }
        Py_XDECREF(pyret);
    }  
    else
    {
        Py_XDECREF(pyfun);
        IoTgo_perror("Cannot call python:setHost()!\n");
    }
    Py_XDECREF(pyfun);
}


/**
 * Get the apikey of user who own the device specified by device_id. 
 * 
 * If the device_type = DEVICE_DIY, apikey_like should be the apikey of 
 * the user who owns the device.
 * If the device_type = DEVICE_PRODUCT, apikey_like should be found around
 * your finished device purchased from ITEAD. 
 * 
 * @warning You must call this before update and query. 
 *
 * @param device_id - device identifier
 * @param apikey_like - just like a password for accessing the respective device
 * @param device_type - indicate the type of your device from DEVICE_DIY or DEVICE_PRODUCT. 
 *  For developers, device_type=DEVICE_DIY(default). For users, device_type=DEVICE_PRODUCT.
 *
 * @retval apikey - a pointer of char array terminated with '\0'.
 * @retval NULL - if falied!
 */
const char *IoTgo::init(const char *device_id, const char *apikey_like, 
    IoTgoDeviceType device_type)
{
    PyObject *pyfun;
    PyObject *pyargs;
    PyObject *pyret;

    const char *ret;
    
    IoTgo_debug("init begin"); 
    
    pyfun = PyObject_GetAttrString(pymod, "init");
    IoTgo_debug("init 1"); 
    
    if (pyfun && PyCallable_Check(pyfun))
    {
        pyargs = Py_BuildValue("(ssi)", device_id, apikey_like, device_type);
        pyret = PyObject_CallObject(pyfun, pyargs);
        Py_XDECREF(pyargs);

        ret = PyString_AsString(pyret);

        IoTgo_debug("ret = %s\n", ret);
        
        ret = strstr(ret, "apikey");
        if (ret != NULL)
        {
            ret += 9;/* {"error":0,"apikey":"0783eab8-0641-421f-83fd-31c19f844b69"} */
            strncpy(buffer, ret, APIKEY_LEN);
        }
        else
        {
            Py_XDECREF(pyret);
            Py_XDECREF(pyfun);
            IoTgo_perror("python:init() failed!");
            return NULL;
        }
        Py_XDECREF(pyret);
        
    }
    else
    {
        Py_XDECREF(pyfun);
        IoTgo_perror("Cannot call python:init()!");
        return NULL;
    }
    Py_XDECREF(pyfun);
    
    return buffer;
}

/**
 * Get the values specified by params from server. 
 * 
 * You can call this after init. 
 *
 * @param params - the list(terminated with NULL) of attributes need to query. 
 *
 * @return the pointer of response buffer terminated with '\0', if success. NULL, if falied!
 */
const char *IoTgo::query(const char *params[])
{
    PyObject *pyfun;
    PyObject *pyargs;
    PyObject *pyret;
    PyObject *pyitem;
    PyObject *pylist;
    
    const char *ret;

    int params_cnt = 0;
    int i=0;

    pyfun = PyObject_GetAttrString(pymod, "query");
    if (pyfun && PyCallable_Check(pyfun))
    {
        IoTgo_debug("params_cnt = %d\n", i);
        if (params == NULL)
        {
            params_cnt = 0;
        }
        else
        {
            for (i=0; params[i] != NULL; i++)
            {
                //printf("params[%d]=%s\n", i, params[i]);
            }
            params_cnt = i;
        }
        IoTgo_debug("params_cnt = %d\n", params_cnt);
        
        pylist = PyList_New(params_cnt);
        for (i=0; i<params_cnt; i++) 
        {
            pyitem = PyString_FromString(params[i]);
            PyList_SetItem(pylist, i, pyitem);
        }
        
        pyargs = Py_BuildValue("(O)", pylist);
        pyret = PyObject_CallObject(pyfun, pyargs);
        Py_XDECREF(pyargs);
        Py_XDECREF(pylist);

        ret = PyString_AsString(pyret);
        //printf("ret = %s\n", ret);
        
        strncpy(buffer, ret, sizeof(buffer)-1);
        Py_XDECREF(pyret);
        
    }  
    else
    {
        Py_XDECREF(pyfun);
        IoTgo_perror("Cannot call python:query()!\n");
        return NULL;
    }
    Py_XDECREF(pyfun);
    
    return buffer;
}

/**
 * Update the values specified by params to server. 
 * 
 * You can call this after init. 
 *
 * @param params - the list(terminated with NULL) of attributes need to update. 
 * @param values - the list(terminated with NULL) of values corresponding to 
 *                 items of attributes. 
 *
 * @return the pointer of response buffer terminated with '\0', if success. NULL, if falied!
 * 
 * @warning the number of items in params must be equal to that in values.
 */
const char *IoTgo::update(const char *params[], const char *values[])
{
    PyObject *pyfun;
    PyObject *pyargs;
    PyObject *pyret;
    PyObject *pyitem;
    PyObject *pydict;
    
    const char *ret;

    int params_cnt = 0;
    int i=0;

    pyfun = PyObject_GetAttrString(pymod, "update");
    if (pyfun && PyCallable_Check(pyfun))
    {
        for (i=0; params[i] != NULL; i++)
        {
            //printf("params[%d]=%s\n", i, params[i]);
        }
        params_cnt = i;
        pydict = PyDict_New();
        for (i=0; i<params_cnt; i++) 
        {
            pyitem = PyString_FromString(values[i]);
            PyDict_SetItemString(pydict, params[i], pyitem);
        }
        
        pyargs = Py_BuildValue("(O)", pydict);
        pyret = PyObject_CallObject(pyfun, pyargs);
        Py_XDECREF(pyargs);
        Py_XDECREF(pydict);
        
        ret = PyString_AsString(pyret);
        //printf("ret = %s\n", ret);
        
        strncpy(buffer, ret, sizeof(buffer) - 1);
        Py_XDECREF(pyret);
        
    }  
    else
    {
        Py_XDECREF(pyfun);
        IoTgo_perror("Cannot call python:update()!\n");
        return NULL;
    }
    Py_XDECREF(pyfun);
    
    return buffer;
}
