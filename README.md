@mainpage

# ITEAD-SDK

--------------------------------------------------------------------------------

# What Is It?

ITEAD-SDK is an open-source interface library for hardware manipulation 
based on Linux platform and compatible with [Arduino API][1].
You can develop with it by using C/C++ and Python. ITEAD-SDK is dedicated to provide
a fast, efficient and unified software development environment for the person
like you with a strong electronic design idea.

# What Can It Do?

Through ITEAD-SDK, users can directly access to the Linux platform hardware 
resources such as GPIO, UART, SPI, I2C etc. ITEAD-SDK supports various 
hardware platforms, such as Iteaduino Plus A10/A20, IBOX,
ITEAD Core EVB, Raspberry Pi and Raspberry Pi Model B+ and so on. Meanwhile, It also
provides a great amount of libraries for all kinds of child board like NFC(PN532)
module, GPS shield, GSM(SIM900/SIM908) shield, nRF24L01 module, OLED and LCD module etc. 

Simplifying the hardware operation makes it possible for software engineers 
who are not familiar with the underlying hardware and junior developers to 
understand more about "Code World" and "Physical World". It can also work 
as a rapid prototyping development tool.

# Features

At present, ITEAD-SDK has a set of **core features** as below:

  - Support for external interrupts operations
  - Support for upper-layer time functions
  - Support for PWM output with any GPIO
  - Support for GPIO operations
  - Support for GPIO 8bit/16bit bus operations
  - Support for UART bus operations
  - Support for I2C bus operations
  - Support for SPI bus operations

Based on core features, plenty of **functional features** can be reached:

  - Support for SSD1306-based I2C OLED screen operations
  - Support for 1602 LiquidCrystal module
  - Support for GPS module
  - Support for GSM(SIM900/SIM908) module including GPRS/GPS operations
  - Support for NFC(PN532) module
  - Support for nRF24L01 module
  - Support for Temperature and Humidity module

# Where Can It Run?

At present, platforms supported by SDK are following:
 
  - Iteaduino Plus A10/A20
  - ITEAD CORE EVB(IBOX)
  - Raspberry Pi Rv2 
  - Raspberry Pi Model B+

If you don't know yet Iteaduino Plus, IBOX or ITEAD Core EVB, see 
<http://imall.iteadstudio.com>.

# Which Language for Developing?

You can implement your design based on ITEAD-SDK in C or C++.
[ITEAD-SDK API][2], which is compatible with [Arduino API][1], makes it easier
for those who have some understanding of Arduino to get started. Without any
codes modified, the libraries and user's codes for Arduino can be run on
prefectly ITEAD-SDK.

Especially and amazingly, [ITEAD-SDK Python API][2] is provided for Python users. 
**A few beautiful lines of code will achieve your goals!**



# Documentation

You can find some important and helpful documents in directory [SDK/doc][2].

Please find in project wiki at <http://wiki.iteadstudio.com/ITEAD_SDK>. With 
more and more hardware that it supports, wiki will also be updated accordingly.

# Installation and Uninstallation

Before all, we assume that you have installed the required packages:
`build-essential git-core` in your Debian system. If not, you can install
them by command:
 
    $ sudo apt-get update
    $ sudo apt-get install build-essential git-core

If you are a Python user, the following packages should be installed:

    $ sudo apt-get python2.7 python2.7-dev

#### Note

[ITEAD-SDK Python API][2] depends python2.7 not python3.x.

## Download and Install SDK in Debian

Only a few steps, DO IT !

### Step 1: Getting the Source Code

You can download SDK by command:

    $ git clone https://github.com/itead/SDK.git
    $ cd SDK

Or you can get SDK via other ways you prefer at <https://github.com/itead/SDK>

#### NOTE

All the commands of the following steps should be run under SDK's 
source code directory.

### Step 2: Configure Your Board

For now, platforms supported by SDK and the configuration commands are below. 
You **MUST** choose the one fitting your board.

  - Iteaduino Plus A10/A20
        
	    $ make Iteaduino_Plus

  - ITEAD CORE EVB
        
        $ make ITEAD_CORE_EVB

  - Raspberry Pi Rv2

        $ make Raspberry_Pi_Rv2

  - Raspberry Pi Model B+

		$ make Raspberry_Pi_Model_BPlus

### Step 3: Select Functional Libraries to Install (Optional)

**If you DO NOT want to customize the libraries to be installed, just skip this
step. nine of ten skip to next step directly!**

It's allowed for user to select functional libraries to install. Open file
`config.mk` and edit it by **adding or removing letter "#"** at the head of each
line for **deselecting or selecting** the corresponding library. More 
description for selecting libraries can be found in `config.mk`. For example:

    $ vi config.mk
    Editing... and Save

### Step 4: Compile and Install

Installing SDK requires root(super user)'s privilege. Run command:

    $ make
    $ sudo make install

If you are a lover of Python, you can install [ITEAD-SDK Python API][2] by running:

	$ sudo make SDK_Python_install

## Version and Help

To check the version of SDK installed, run command:
    
    $ iteadcompile -v
    
For some brief help information:

    $ iteadcompile --help

## Uninstall

To uninstall the SDK, just run command:

    $ cd SDK
    $ sudo make uninstall

If you have installed [ITEAD-SDK Python API][2], you can uninstall it by running:

	$ sudo make SDK_Python_uninstall

# How to Start

[ITEAD-SDK Python API][2] will be recommanded for Python users.
SDK works well with C(.c) and C++(.cpp) programs. Especially, it is
compatiable with Arduino Sketch(.ino).

## To use SDK in Python

Create a new file named `blink.py` which reads as follow:

	#! /usr/bin/env python
	from iteadsdk import *
	LED = PG9
	pinMode(LED, OUTPUT)
	while True:
	    digitalWrite(LED, HIGH)
	    delay(1000)
	    digitalWrite(LED, LOW)
	    delay(1000)

Make `blink.py` executable:
	
    $ chmod +x blink.py 

Then, run it:

	$ sudo ./blink.py

You can get commutative HIGH and LOW from GPIO PG9, if you've run it on ITEAD CORE EVB
or other platforms based on Allwinner A10/A20 SoC.

## To use SDK in C program

Create a new file named "led.c" which reads as follow:

    #include <itead.h>
    #include <stdio.h>
    
    #define STATUS_LED1 PG9
 
    void setup(void)
    {
        printf("setup begin\n");
        pinMode(STATUS_LED1,OUTPUT);
    }
    void loop(void)
    {
        digitalWrite(STATUS_LED1,HIGH);
        delay(1000);
        digitalWrite(STATUS_LED1,LOW);
        delay(1000);
    }
    void main(int argc, char **argv)
    {
        setup();
        while(1)
        {
            loop();
        }
    }

Then run iteadcompile in the terminal

    $ iteadcompile LED led.c

You can run the LED program after compilation:

    $ sudo ./LED


## To use SDK in C++ program
 
Create a new file named "led.cpp" which reads as follow:

    #include <itead.h>
    #include <stdio.h>
 
    #define STATUS_LED1 PG9
 
    void setup(void)
    {
        printf("setup begin\n");
        pinMode(STATUS_LED1,OUTPUT);
    }
    void loop(void)
    {
        digitalWrite(STATUS_LED1,HIGH);
        delay(1000);
        digitalWrite(STATUS_LED1,LOW);
        delay(1000);
    }
    int main(int argc, char **argv)
    {
        setup();
        while(1)
        {
            loop();
        }
        return 0;
    }
    
Then run iteadcompile in the terminal
 
    $ iteadcompile LED led.cpp
 
You can run the LED program after compilation:

    $ sudo ./LED

 
## To use SDK like an Arduino sketch(.ino)

Create a new file named "led.ino" which contains the following lines:

    #include <stdio.h>
    
    #define STATUS_LED1 PG9
 
    void setup(void)
    {
        printf("setup begin\n");
        pinMode(STATUS_LED1,OUTPUT);
    }
    void loop(void)
    {
        digitalWrite(STATUS_LED1,HIGH);
        delay(1000);
        digitalWrite(STATUS_LED1,LOW);
        delay(1000);
    }

Then run command as below to compile led.ino:

    $ iteadcompile LED led.ino

You can run the LED program after compilation:

    $ sudo ./LED

#### Note: How to understand debug information from compiler

You DO NOT include the itead.h explicitly in your source file with suffix .ino.
iteadcompile will take care of everything. That means you need to know the method
you should use for locating your .ino sketch bugs. Actually,if you have a sketch
named "Hello.ino" with a bug at level ERROR in line 25, you will get some ERROR 
informations telling the ERROR bug in "Hello.cpp" with line 26(25+1) when 
command "iteadcompile Hello Hello.ino" is executed. Why? iteadcompile insert one 
line like `include  "itead.h"`  in the head of your Hello.ino, change the suffix
.ino to .cpp and compile(using g++) Hello.cpp to generate Hello which is 
executable binary file. When you encounter other bugs in your sketch,debug 
skills on C++ program will also helpful for solving your problems.

# Plan for Next Stage
 
ITEAD-SDK will support much more hardware platforms and 
functional modules in the future. In the Linux thriving time, ITEAD-SDK 
will make any impossible fanatic idea possible, are you ready?

# How to Contribute
 
If you are interested and want to contribute to this project, yet you
are suffering from lack of the related hardware. It does not matter.
Just write an E-mail to <info@iteadstudio.com>, attach your github
username in it and say something about your experience on the related
projects, ITEAD Studio will send you a hardware for free after approval.
 
After receiving Iteaduino Plus A10/A20, you only need to:
 
  1. Fork this project
  2. Coding
  3. Push

# License GPL
 
Copyright (C) 2013 ITEAD Studio

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program. If not, see <http://www.gnu.org/licenses/>.

--------------------------------------------------------------------------------

# The end

--------------------------------------------------------------------------------

  [0]:https://github.com/itead/SDK "ITEAD-SDK"
  [1]:http://arduino.cc/en/Reference/HomePage "Arduino API"
  [2]:https://github.com/itead/SDK/tree/master/doc "SDK/doc"
