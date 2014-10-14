/**
 * @file iteadwire-py.cpp
 *
 * Python interface of class TwoWire in iteadcpp.so
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
    TwoWire *t;
} PyTwoWire;

/* Implementation of slots used by TwoWireType */

static int
TwoWire_init(PyTwoWire *self, PyObject *args, PyObject *kwds)
{
    uint32_t dev;
    
    int argc = PyTuple_Size(args);
    
    if(argc != 1)
    {
        PyErr_SetString
        (
            PyExc_TypeError, 
            "No such constructor in class TwoWire"
        );
        return -1;
    }
    if (!PyArg_ParseTuple(args, "I", &dev))
    {
        return -1;
    } 
    
    self->t = new TwoWire(dev);
    
    debug("TwoWire_init done\n");
    
    return 0;
}

static void
TwoWire_dealloc(PyTwoWire* self)
{
    delete self->t;
    self->ob_type->tp_free((PyObject*)self);
    debug("TwoWire_dealloc done\n");
}

static PyObject*
TwoWire_begin(PyTwoWire *self, PyObject *args)
{
    self->t->begin();
    Py_RETURN_NONE;
}

static PyObject*
TwoWire_end(PyTwoWire *self, PyObject *args)
{
    self->t->end();
    Py_RETURN_NONE;
}

static PyObject*
TwoWire_beginTransmission(PyTwoWire *self, PyObject *args)
{
    uint8_t addr;
    if (!PyArg_ParseTuple(args, "B", &addr))
    {
        return NULL;
    } 
    self->t->beginTransmission(addr);
    Py_RETURN_NONE;
}

static PyObject*
TwoWire_write(PyTwoWire *self, PyObject *args)
{
    uint8_t val;
    size_t ret;
    
    if (!PyArg_ParseTuple(args, "B", &val))
    {
        return NULL;
    } 
    ret = self->t->write(val);
    return Py_BuildValue("I", ret);
}

static PyObject*
TwoWire_endTransmission(PyTwoWire *self, PyObject *args)
{
    uint32_t ret;
    ret = self->t->endTransmission();
    return Py_BuildValue("I", ret);
}

static PyObject*
TwoWire_requestFrom(PyTwoWire *self, PyObject *args)
{
    uint8_t addr;
    uint32_t count;
    uint32_t ret;
    
    if (!PyArg_ParseTuple(args, "BI", &addr, &count))
    {
        return NULL;
    } 
    ret = self->t->requestFrom(addr, count);
    return Py_BuildValue("I", ret);
}

static PyObject*
TwoWire_available(PyTwoWire *self, PyObject *args)
{
    uint32_t ret;
    ret = self->t->available();
    return Py_BuildValue("I", ret);
}

static PyObject*
TwoWire_read(PyTwoWire *self, PyObject *args)
{
    uint8_t ret;
    ret = self->t->read();
    return Py_BuildValue("B", ret);
}


static PyMethodDef TwoWire_methods[] = 
{
    {
        "begin",
        (PyCFunction)TwoWire_begin, 
        METH_VARARGS,
        "Request resource of TwoWire"
    },
    {
        "end",
        (PyCFunction)TwoWire_end, 
        METH_VARARGS,
        "Free resource of TwoWire"
    },
    {
        "beginTransmission",
        (PyCFunction)TwoWire_beginTransmission, 
        METH_VARARGS,
        "Set slave addr and empty the tx buffer."
    },
    {
        "write",
        (PyCFunction)TwoWire_write, 
        METH_VARARGS,
        "Write val to buffer."
    },
    {
        "endTransmission",
        (PyCFunction)TwoWire_endTransmission, 
        METH_VARARGS,
        "Actually, transmit tx buffer data to slave."
    },
    {
        "requestFrom",
        (PyCFunction)TwoWire_requestFrom, 
        METH_VARARGS,
        "Request data from addr to rx buffer."
    },
    {
        "available",
        (PyCFunction)TwoWire_available, 
        METH_VARARGS,
        "Get the number of data unread in rx buffer."
    },
    {
        "read",
        (PyCFunction)TwoWire_read, 
        METH_VARARGS,
        "Read a date type of uint8_t from rx buffer."
    },
    {NULL}  /* Sentinel */
};


static PyTypeObject TwoWireType = {
    PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "iteadwire.TwoWire",        /*tp_name*/
    sizeof(PyTwoWire),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)TwoWire_dealloc,/*tp_dealloc*/
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
    "class TwoWire provides some simple methods to manipulate TwoWire devices.",  /* tp_doc */
    0,		                    /* tp_traverse */
    0,		                    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    TwoWire_methods,            /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)TwoWire_init,     /* tp_init */
    0,                          /* tp_alloc */
    PyType_GenericNew,          /* tp_new */
};


static PyMethodDef module_iteadwire_methods[] = {
    {NULL}  /* Sentinel */
};


#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
inititeadwire(void) 
{
    PyObject* iteadwire;
    
    iteadwire = Py_InitModule3
        (
        "iteadwire",
        module_iteadwire_methods,            
        "iteadwire module implements the interface between Python and class TwoWire written by C++."
        );

    if (iteadwire == NULL)
        return;
      
    if (PyType_Ready(&TwoWireType) < 0)
        return;

    Py_INCREF(&TwoWireType);
    PyModule_AddObject(iteadwire, "TwoWire", (PyObject *)&TwoWireType);
}


