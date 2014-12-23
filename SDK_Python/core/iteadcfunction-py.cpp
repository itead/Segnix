/**
 * @file iteadcfunction-py.cpp
 *
 * Python interface of fucntions in iteadc.so
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/10/13
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#include <Python.h>
#include <itead.h>

#define GPIO_MODULE_INTERFACE_BEGIN

static PyObject * Py_pinMode(PyObject *self, PyObject *args)
{
    uint32_t ret;
    uint16_t pin;
    uint8_t mode;
    
    if (!PyArg_ParseTuple(args, "HB", &pin, &mode))
        return NULL;
        
    ret = pinMode(pin, mode);
    
    return Py_BuildValue("I", ret);

}

static PyObject * Py_digitalWrite(PyObject *self, PyObject *args)
{
    uint32_t ret;
    uint16_t pin;
    uint8_t val;
    
    if (!PyArg_ParseTuple(args, "HB", &pin, &val))
        return NULL;

    ret = digitalWrite(pin, val);

    return Py_BuildValue("I", ret);
}

static PyObject * Py_digitalRead(PyObject *self, PyObject *args)
{
    uint32_t ret;
    uint16_t pin;
    
    if (!PyArg_ParseTuple(args, "H", &pin))
        return NULL;

    ret = digitalRead(pin);

    return Py_BuildValue("I", ret);
}

static PyObject * Py_analogRead(PyObject *self, PyObject *args)
{
    uint32_t ret;
    uint16_t pin;
    
    if (!PyArg_ParseTuple(args, "H", &pin))
        return NULL;

    ret = analogRead(pin);

    return Py_BuildValue("I", ret);
}

static PyObject * Py_analogWrite(PyObject *self, PyObject *args)
{
    uint32_t ret;
    uint16_t pin;
    uint8_t duty;
    
    if (!PyArg_ParseTuple(args, "HB", &pin, &duty))
        return NULL;

    ret = analogWrite(pin, duty);
    
    return Py_BuildValue("I", ret);
}

#define GPIO_MODULE_INTERFACE_END
#define TIME_MODULE_INTERFACE_BEGIN
static PyObject * Py_micros(PyObject *self, PyObject *args)
{
    uint32_t ret = 0;
    ret = micros();
    return Py_BuildValue("I", ret);
}

static PyObject * Py_millis(PyObject *self, PyObject *args)
{
    uint32_t ret = 0;
    ret = millis();
    return Py_BuildValue("I", ret);
}

static PyObject * Py_delay(PyObject *self, PyObject *args)
{
    uint32_t how_long;
    
    if (!PyArg_ParseTuple(args, "I", &how_long))
        return NULL;
        
    delay(how_long);
    
    Py_RETURN_NONE;
}

static PyObject * Py_delayMicroseconds(PyObject *self, PyObject *args)
{
    uint32_t how_long;
    
    if (!PyArg_ParseTuple(args, "I", &how_long))
        return NULL;
        
    delayMicroseconds(how_long);
    
    Py_RETURN_NONE;
}

#define TIME_MODULE_INTERFACE_END
#define INTERRUPT_MODULE_INTERFACE_BEGIN

static PyObject *my_callback = NULL;
static PyObject * Py_set_callback(PyObject *self, PyObject *args)
{
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(my_callback);  /* Dispose of previous callback */
        my_callback = temp;       /* Remember new callback */
        
        /* Boilerplate to return "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}

static PyObject * Py_test_callback(PyObject *self, PyObject *args)
{
    PyObject *arglist;
    PyObject *result;
#if 0    
    uint32_t id;
#endif    
    arglist = Py_BuildValue("(I)", 1999);
    if (arglist == NULL)
        return NULL;
        
    result = PyObject_CallObject(my_callback, arglist);
    Py_DECREF(arglist);
    
    if (result == NULL)
        return NULL; /* Pass error back */

    Py_DECREF(result);
    Py_RETURN_NONE;
}


typedef struct GpioPyEint {
    uint16_t pin; 
    PyObject *py_isr;
    PyObject *py_pdata;
    uint32_t mode;
} GpioPyEint;
static GpioPyEint gpio_py_eint[GPIO_EINT_NUMBER_TOTAL];

static void generalGpioIsr(uint16_t pin, void *pdata)
{
    static bool is_fisrt = true;
    PyObject *result;
    PyObject *arglist;
    uint32_t eint_no = (uint32_t)pdata;
    
    /* ignore the first interrupt except falling edge */
    if(is_fisrt && gpio_py_eint[eint_no].mode != GPIO_EINT_CFG_FALLING) 
    {
        is_fisrt = false;
        return;
    }

    arglist = Py_BuildValue("(HO)", pin, gpio_py_eint[eint_no].py_pdata);
    if (arglist == NULL)
    {
        sdkerr("Py_BuildValue failed!\n");
        return;
    }
    
    result = PyObject_CallObject(gpio_py_eint[eint_no].py_isr, arglist);
    Py_DECREF(arglist);
    Py_XDECREF(result);
    
}

static inline int32_t fakeAttachInterrupt(
    uint16_t pin, 
    PyObject *py_isr, 
    PyObject *py_pdata,
    uint32_t mode)
{
    PyObject *tmp;
    uint32_t eint_no;
    
    eint_no = get_eint_no(pin);
    if(eint_no == GPIO_EINT_NONE) 
        return -1;
        
    gpio_py_eint[eint_no].pin = pin;
    gpio_py_eint[eint_no].mode = mode;
    
    tmp = gpio_py_eint[eint_no].py_isr;
    gpio_py_eint[eint_no].py_isr = py_isr;
    Py_XDECREF(tmp);
    
    tmp = gpio_py_eint[eint_no].py_pdata;
    gpio_py_eint[eint_no].py_pdata= py_pdata;
    Py_XDECREF(tmp);
    
    return attachInterrupt(pin, generalGpioIsr, (void*)eint_no, mode);
}


static PyObject* Py_attachInterrupt(PyObject *self, PyObject *args)
{
    PyObject *t_gpio_py_isr = NULL;
    PyObject *t_gpio_py_isr_pdata = NULL;
    uint16_t pin;
    uint32_t mode;

    if (PyArg_ParseTuple(args, "HOOI", &pin, &t_gpio_py_isr, &t_gpio_py_isr_pdata, &mode)) 
    {
        if (!PyCallable_Check(t_gpio_py_isr)) 
        {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        
        /* Check t_gpio_py_isr_pdata */
        if (t_gpio_py_isr_pdata == NULL)
            return NULL;
            
        /* Add a reference */
        Py_INCREF(t_gpio_py_isr); 
        Py_INCREF(t_gpio_py_isr_pdata);
        return Py_BuildValue(
                    "i", 
                    fakeAttachInterrupt(pin, t_gpio_py_isr, t_gpio_py_isr_pdata, mode)
                    );
    }
    
    return NULL;
}

static PyObject * Py_detachInterrupt(PyObject *self, PyObject *args)
{
    uint16_t pin;
    uint32_t eint_no;
    
    if (!PyArg_ParseTuple(args, "H", &pin))
        return NULL;

    eint_no = get_eint_no(pin);
    if(eint_no == GPIO_EINT_NONE) 
    {
        sdkerr("get_eint_no() failed!\n");
        return NULL;
    }
    
    gpio_py_eint[eint_no].pin = GPIO_PIN_NONE;
    gpio_py_eint[eint_no].mode = 0;
    
    Py_XDECREF(gpio_py_eint[eint_no].py_isr);
    Py_XDECREF(gpio_py_eint[eint_no].py_pdata);

    gpio_py_eint[eint_no].py_isr = NULL;
    gpio_py_eint[eint_no].py_pdata = NULL;
    
    return Py_BuildValue("i", detachInterrupt(pin));
}

static PyObject * Py_noInterrupt(PyObject *self, PyObject *args)
{
    return Py_BuildValue("i", noInterrupt());
}

static PyObject * Py_interrupt(PyObject *self, PyObject *args)
{
    return Py_BuildValue("i", interrupt());
}

#define INTERRUPT_MODULE_INTERFACE_END

/*
Format:
    python funciton name
    C/C++ function name
    METH_VARARGS
    Document
*/
static PyMethodDef iteadcfunction_methods[] = 
{
    {
        "pinMode", 
        Py_pinMode, 
        METH_VARARGS, 
        "Configure GPIO mode."
    },
    {   
        "digitalWrite", 
        Py_digitalWrite, 
        METH_VARARGS, 
        "Write pin with val"
    },
    {
        "digitalRead", 
        Py_digitalRead, 
        METH_VARARGS, 
        "Read pin"
    },
    {
        "analogRead", 
        Py_analogRead, 
        METH_VARARGS, 
        "Read adc pin"
    },
    {   
        "analogWrite", 
        Py_analogWrite, 
        METH_VARARGS, 
        "Output PWN on pin with duty(0-255)"
    },
    {   
        "micros", 
        Py_micros, 
        METH_VARARGS, 
        "Simply return the time of which system run in micro second."
    },
    {   
        "millis", 
        Py_millis, 
        METH_VARARGS, 
        "Return the number of milliseconds since the system began until now."
    },
    {   
        "delay", 
        Py_delay, 
        METH_VARARGS, 
        "Make caller sleep for some milliseconds specified by howLong."
    },
    {   
        "delayMicroseconds", 
        Py_delayMicroseconds, 
        METH_VARARGS, 
        "Make caller sleep for some microseconds specified by howLong."
    },
    {   
        "set_callback", 
        Py_set_callback, 
        METH_VARARGS, 
        "no help"
    },
    {   
        "test_callback", 
        Py_test_callback, 
        METH_VARARGS, 
        "no help"
    },
    {   
        "attachInterrupt", 
        Py_attachInterrupt, 
        METH_VARARGS, 
"\
\n Prototype: def attachInterrupt(pin, isr, pdata, mode) \
\n \
\n Register a named Interrupt Service Routine (ISR) to call when an interrupt occurs. \
\n \
\n @param pin - GPIO PIN NAME\
\n @param isr - ISR\
\n @param pdata - Private data for user's any purpose\
\n @param mode - Trigger method\
\n \
\n Example: \
\n      attachInterrupt(PH21, generalGpioIsr, 'Hi, I am Jack!', GPIO_EINT_CFG_FALLING) \
\n \
"
    },
    {   
        "detachInterrupt", 
        Py_detachInterrupt,
        METH_VARARGS, 
        "Detach ISR from pin."
    },
    {   
        "noInterrupt", 
        Py_noInterrupt,
        METH_VARARGS, 
        "Disable all registered GPIO interrupts."
    },
    {   
        "interrupt", 
        Py_interrupt,
        METH_VARARGS, 
        "Enable all registered GPIO interrupts."
    },
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC inititeadcfunction(void)
{
    Py_InitModule3
    (
        "iteadcfunction", 
        iteadcfunction_methods, 
        "iteadcfunction module implements the Python interface to C functions in ITEAD-OS-SDK."
    );
}

