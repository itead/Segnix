/**
 * @file iteadanalog-py.cpp
 *
 * Python interface of class AnalogClass in iteadcpp.so
 *
 * @author  Wu xiaoyu (email:<mars.wu@itead.cc>)
 * @date    2014/12/17
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
    AnalogClass *s;
} PyAnalogClass;

/* Implementation of slots used by Serial_Type */

static int
AnalogClass_init(PyAnalogClass *self, PyObject *args, PyObject *kwds)
{
    uint32_t dev;
    
    int argc = PyTuple_Size(args);
    
    if(argc != 1)
    {
        PyErr_SetString
        (
            PyExc_TypeError, 
            "No such constructor in class AnalogClass"
        );
        return -1;
    }
    if (!PyArg_ParseTuple(args, "I", &dev))
    {
        return -1;
    } 

    self->s = new Serial_(dev);
    
    debug("AnalogClass_init done\n");
    
    return 0;
}

static void
AnalogClass_dealloc(PyAnalogClass* self)
{
    delete self->s;
    self->ob_type->tp_free((PyObject*)self);
    debug("AnalogClass_dealloc done\n");
}

static PyObject*
AnalogClass_analogRead(PyAnalogClass *self, PyObject *args)
{
    uint8_t ret;
    ret = self->s->analogRead();
    return Py_BuildValue("B", ret);
}


static PyMethodDef AnalogClass_methods[] = 
{
        "analogRead",
        (PyCFunction)AnalogClass_analogRead, 
        METH_VARARGS,
        "Receive ADC's result"
    },
    {NULL}  /* Sentinel */
};

static PyTypeObject AnalogClassType = {
    PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/
    "iteadanalog.AnalogClass",  /*tp_name*/
    sizeof(PyAnalogClass),      /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)AnalogClass_dealloc,/*tp_dealloc*/
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
    "class AnalogClass provides some simple methods to manipulate ADC devices.",  /* tp_doc */
    0,		                    /* tp_traverse */
    0,		                    /* tp_clear */
    0,		                    /* tp_richcompare */
    0,		                    /* tp_weaklistoffset */
    0,		                    /* tp_iter */
    0,		                    /* tp_iternext */
    AnalogClass_methods,        /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc)AnalogClass_init, /* tp_init */
    0,                          /* tp_alloc */
    PyType_GenericNew,          /* tp_new */
};


static PyMethodDef module_iteadanalog_methods[] = {
    {NULL}  /* Sentinel */
};


#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
inititeadanalog(void) 
{
    PyObject* iteadanalog;
    
    iteadadc = Py_InitModule3
        (
        "iteadanalog",
        module_iteadanalog_methods,            
        "iteadanalog module implements the interface between Python and class AnalogClass written by C++."
        );

    if (iteadanalog == NULL)
        return;
      
    if (PyType_Ready(&AnalogClassType) < 0)
        return;

    Py_INCREF(&AnalogClassType);
    PyModule_AddObject(iteadanalog, "AnalogClass", (PyObject *)&AnalogClassType);
}


