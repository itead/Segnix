/**
 * @file iteadspi-py.cpp
 *
 * Python interface of class SPIClass in iteadcpp.so
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
#include <structmember.h>

#include <itead.h>


//#define DEBUG
#include <itead_debug.h>

typedef struct {
    PyObject_HEAD
    SPIClass *s;
} PySPIClass;

/* Implementation of slots used by SPIClassType */

static int
SPIClass_init(PySPIClass *self, PyObject *args, PyObject *kwds)
{
    uint32_t dev;
    
    int argc = PyTuple_Size(args);
    
    if(argc != 1)
    {
        PyErr_SetString
        (
            PyExc_TypeError, 
            "No such constructor in class SPIClass"
        );
        return -1;
    }
    if (!PyArg_ParseTuple(args, "I", &dev))
    {
        return -1;
    } 
    
    self->s = new SPIClass(dev);
    
    debug("SPIClass_init done\n");
    
    return 0;
}

static void
SPIClass_dealloc(PySPIClass* self)
{
    delete self->s;
    self->ob_type->tp_free((PyObject*)self);
    debug("SPIClass_dealloc done\n");
}

static PyObject*
SPIClass_begin(PySPIClass *self, PyObject *args)
{
    self->s->begin();
    Py_RETURN_NONE;
}

static PyObject*
SPIClass_end(PySPIClass *self, PyObject *args)
{
    self->s->end();
    Py_RETURN_NONE;
}

static PyObject*
SPIClass_setBitOrder(PySPIClass *self, PyObject *args)
{
    uint8_t order;
    if (!PyArg_ParseTuple(args, "B", &order))
    {
        return NULL;
    } 
    self->s->setBitOrder(order);
    Py_RETURN_NONE;
}

static PyObject*
SPIClass_setDataMode(PySPIClass *self, PyObject *args)
{
    uint8_t mode;
    if (!PyArg_ParseTuple(args, "B", &mode))
    {
        return NULL;
    } 
    self->s->setDataMode(mode);
    Py_RETURN_NONE;
}

static PyObject*
SPIClass_setClockDivider(PySPIClass *self, PyObject *args)
{
    uint8_t divider;
    if (!PyArg_ParseTuple(args, "B", &divider))
    {
        return NULL;
    } 
    self->s->setClockDivider(divider);
    Py_RETURN_NONE;
}

static PyObject*
SPIClass_transfer(PySPIClass *self, PyObject *args)
{
    uint8_t val;
    uint8_t ret;
    
    if (!PyArg_ParseTuple(args, "B", &val))
    {
        return NULL;
    } 
    ret = self->s->transfer(val);
    return Py_BuildValue("B", ret);
}


static PyMethodDef SPIClass_methods[] = 
{
    {
        "begin",
        (PyCFunction)SPIClass_begin, 
        METH_VARARGS,
        "Request resource of SPI"
    },
    {
        "end",
        (PyCFunction)SPIClass_end, 
        METH_VARARGS,
        "Free resource of SPI"
    },
    {
        "setBitOrder",
        (PyCFunction)SPIClass_setBitOrder, 
        METH_VARARGS,
        "Set bit order"
    },
    {
        "setDataMode",
        (PyCFunction)SPIClass_setDataMode, 
        METH_VARARGS,
        "Set data mode"
    },
    {
        "setClockDivider",
        (PyCFunction)SPIClass_setClockDivider, 
        METH_VARARGS,
        "Set speed"
    },
    {
        "transfer",
        (PyCFunction)SPIClass_transfer, 
        METH_VARARGS,
        "Send and receive data"
    },
    {NULL}  /* Sentinel */
};



static PyTypeObject SPIClassType = {
    PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "iteadspi.SPIClass",      /*tp_name*/
    sizeof(PySPIClass),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)SPIClass_dealloc,/*tp_dealloc*/
    0,                          /*tp_print*/
    0,                          /*tp_getattr*/
    0,                          /*tp_setattr*/
    0,                          /*tp_compare*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash */
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    0,                          /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /*tp_flags*/
    "class SPIClass provides some simple methods to manipulate SPI devices.",  /* tp_doc */
    0,		                    /* tp_traverse */
    0,		                    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    SPIClass_methods,            /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)SPIClass_init,     /* tp_init */
    0,                          /* tp_alloc */
    PyType_GenericNew,          /* tp_new */
};


static PyMethodDef module_iteadspi_methods[] = {
    {NULL}  /* Sentinel */
};


#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
inititeadspi(void) 
{
    PyObject* iteadspi;
    
    iteadspi = Py_InitModule3
        (
        "iteadspi",
        module_iteadspi_methods,            
        "iteadspi module implements the interface between Python and class SPIClass written by C++."
        );

    if (iteadspi == NULL)
        return;
      
    if (PyType_Ready(&SPIClassType) < 0)
        return;

    Py_INCREF(&SPIClassType);
    PyModule_AddObject(iteadspi, "SPIClass", (PyObject *)&SPIClassType);
}


