/**
 * @file iteadserial-py.cpp
 *
 * Python interface of class Serial_ in iteadcpp.so
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
    Serial_ *s;
} PySerial_;

/* Implementation of slots used by Serial_Type */

static int
Serial__init(PySerial_ *self, PyObject *args, PyObject *kwds)
{
    uint32_t dev;
    
    int argc = PyTuple_Size(args);
    
    if(argc != 1)
    {
        PyErr_SetString
        (
            PyExc_TypeError, 
            "No such constructor in class Serial_"
        );
        return -1;
    }
    if (!PyArg_ParseTuple(args, "I", &dev))
    {
        return -1;
    } 
    
    self->s = new Serial_(dev);
    
    debug("Serial__init done\n");
    
    return 0;
}

static void
Serial__dealloc(PySerial_* self)
{
    delete self->s;
    self->ob_type->tp_free((PyObject*)self);
    debug("Serial__dealloc done\n");
}

static PyObject*
Serial__begin(PySerial_ *self, PyObject *args)
{
    uint32_t baudrate;
    if (!PyArg_ParseTuple(args, "I", &baudrate))
    {
        return NULL;
    } 
    self->s->begin(baudrate);
    Py_RETURN_NONE;
}

static PyObject*
Serial__end(PySerial_ *self, PyObject *args)
{
    self->s->end();
    Py_RETURN_NONE;
}

static PyObject*
Serial__write(PySerial_ *self, PyObject *args)
{
    size_t ret;
    uint8_t c;
    if (!PyArg_ParseTuple(args, "B", &c))
    {
        return NULL;
    } 
    ret = self->s->write(c);
    return Py_BuildValue("I", ret);
}

static PyObject*
Serial__read(PySerial_ *self, PyObject *args)
{
    uint8_t ret;
    ret = self->s->read();
    return Py_BuildValue("B", ret);
}

static PyObject*
Serial__available(PySerial_ *self, PyObject *args)
{
    int32_t ret;
    ret = self->s->available();
    return Py_BuildValue("i", ret);
}

static PyObject*
Serial__flush(PySerial_ *self, PyObject *args)
{
    self->s->flush();
    Py_RETURN_NONE;
}

static PyObject*
Serial__printstr(PySerial_ *self, PyObject *args)
{
    size_t ret;
    const char *str;
    
    if (!PyArg_ParseTuple(args, "s", &str))
    {
        return NULL;
    } 
    ret = self->s->print(str);
    return Py_BuildValue("I", ret);
}



static PyMethodDef Serial__methods[] = 
{
    {
        "begin",
        (PyCFunction)Serial__begin, 
        METH_VARARGS,
        "Request resource of UART"
    },
    {
        "end",
        (PyCFunction)Serial__end, 
        METH_VARARGS,
        "Free resource of UART"
    },
    {
        "write",
        (PyCFunction)Serial__write, 
        METH_VARARGS,
        "Send one byte"
    },
    {
        "read",
        (PyCFunction)Serial__read, 
        METH_VARARGS,
        "Receive one byte"
    },
    {
        "available",
        (PyCFunction)Serial__available, 
        METH_VARARGS,
        "Return the number of unread data (in char or byte)"
    },
    {
        "flush",
        (PyCFunction)Serial__flush, 
        METH_VARARGS,
        "Empty the buffer of tx and rx "
    },
    {
        "printstr",
        (PyCFunction)Serial__printstr, 
        METH_VARARGS,
        "Send a string"
    },
    {NULL}  /* Sentinel */
};

static PyTypeObject Serial_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "iteadserial.Serial_",      /*tp_name*/
    sizeof(PySerial_),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)Serial__dealloc,/*tp_dealloc*/
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
    "class Serial_ provides some simple methods to manipulate UART devices.",  /* tp_doc */
    0,		                    /* tp_traverse */
    0,		                    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    Serial__methods,            /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)Serial__init,     /* tp_init */
    0,                          /* tp_alloc */
    PyType_GenericNew,          /* tp_new */
};


static PyMethodDef module_iteadserial_methods[] = {
    {NULL}  /* Sentinel */
};


#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
inititeadserial(void) 
{
    PyObject* iteadserial;
    
    iteadserial = Py_InitModule3
        (
        "iteadserial",
        module_iteadserial_methods,            
        "iteadserial module implements the interface between Python and class Serial_ written by C++."
        );

    if (iteadserial == NULL)
        return;
      
    if (PyType_Ready(&Serial_Type) < 0)
        return;

    Py_INCREF(&Serial_Type);
    PyModule_AddObject(iteadserial, "Serial_", (PyObject *)&Serial_Type);
}


