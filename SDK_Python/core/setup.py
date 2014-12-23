#!/usr/bin/env python

from distutils.core import setup, Extension

mod_iteadcfunction = Extension(
    'iteadcfunction',
    libraries   = ['iteadc'],
    sources     = ['iteadcfunction-py.cpp']
    )
    
mod_iteadserial = Extension(
    'iteadserial',
    libraries   = ['iteadc', 'iteadcpp'],
    sources     = ['iteadserial-py.cpp']
    )
    
mod_iteadspi = Extension(
    'iteadspi',
    libraries   = ['iteadc', 'iteadcpp'],
    sources     = ['iteadspi-py.cpp']
    )

mod_iteadwire = Extension(
    'iteadwire',
    libraries   = ['iteadc', 'iteadcpp'],
    sources     = ['iteadwire-py.cpp']
    )


setup(	
    name            = "ITEAD-OS-SDK-Py",       # Package name
    version         = "0.1",                # Version
    description     = "ITEAD-OS-SDK Python Interface",
    author          = "Wu Pengfei",
    author_email    = "pengfei.wu@itead.cc",
    url             = "https://github.com/itead",
    long_description= "This is a package of ITEAD-OS-SDK Python Interface.",
    license         = "GPL2.0 and later versions",
    ext_modules     = [
    					mod_iteadcfunction, # Python interface of fucntions in iteadc.so
    					mod_iteadserial,	# Python interface of Serial_ in iteadcpp.so
    					mod_iteadspi,		# Python interface of SPIClass in iteadcpp.so
    					mod_iteadwire,		# Python interface of TwoWire in iteadcpp.so
    				  ]
    )

print "Done!"


       
