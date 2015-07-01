@mainpage

# Segnix

--------------------------------------------------------------------------------

# What Is It?

Segnix is an open-source interface library for hardware manipulation 
based on Linux platform and compatible with [Arduino API][1].
You can develop with it by using C/C++ and Python. Segnix is dedicated to provide
a fast, efficient and unified software development environment for the person
like you with a strong electronic design idea.

# CAUTION!

## Before November 22, 2014, Segnix was named ITEAD-OS-SDK

# What Can It Do?

Through Segnix, users can directly access to the Linux platform hardware 
resources such as GPIO, UART, SPI, I2C etc. Segnix supports various 
hardware platforms, such as Iteaduino Plus A10/A20, IBOX,
ITEAD Core EVB, Raspberry Pi and Raspberry Pi Model B+ and so on. Meanwhile, It also
provides a great amount of libraries for all kinds of child board like NFC(PN532)
module, GPS shield, GSM(SIM900/SIM908) shield, nRF24L01 module, OLED and LCD module etc. 

Simplifying the hardware operation makes it possible for software engineers 
who are not familiar with the underlying hardware and junior developers to 
understand more about "Code World" and "Physical World". It can also work 
as a rapid prototyping development tool.

# Features

At present, Segnix has a set of **core features** as below:

  - Support for upper-layer time functions
  - Support for PWM output with any GPIO
  - Support for GPIO operations
  - Support for GPIO 8bit/16bit bus operations
  - Support for UART bus operations
  - Support for I2C bus operations
  - Support for SPI bus operations
  - Support for external interrupts operations(Only for `ITEAD CORE EVB`)
  - Support for ADC (Only for `BeagleBone Black`)

Based on core features, plenty of **functional features** can be reached:

  - Support for SSD1306-based I2C OLED screen operations
  - Support for 1602 LiquidCrystal module
  - Support for GPS module
  - Support for GSM(SIM900/SIM908) module including GPRS/GPS operations
  - Support for NFC(PN532) module
  - Support for nRF24L01 module
  - Support for Temperature and Humidity module
  - Support for [IoTgo platform](http://iotgo.iteadstudio.com)

# Where Can It Run?

At present, platforms supported by Segnix are following:
 
  - Iteaduino Plus A10/A20
  - ITEAD CORE EVB(IBOX)
  - Raspberry Pi Rv2 
  - Raspberry Pi Model B+
  - BeagleBone Black

# Which Language for Developing?

You can implement your design based on Segnix in C or C++.
[Segnix API][2], which is compatible with [Arduino API][1], makes it easier
for those who have some understanding of Arduino to get started. Without any
codes modified, the libraries and user's codes for Arduino can be run on
Segnix prefectly.

[Segnix Python API][2], especially and amazingly, is provided for Python users. 
**A few beautiful lines of code will achieve your goals!**

# Documentation

Online Segnix API can be reached at [Segnix API][2].
You can also find some important and helpful documents in directory [Segnix/doc][3].

# Installation and Uninstallation

Before all, we assume that you have installed the required packages:
`build-essential git-core` in your Debian system. If not, you can install
them by command:
 
    $ sudo apt-get update
    $ sudo apt-get install build-essential git-core

If you are a Python user, the following packages should be installed:

    $ sudo apt-get install python2.7 python2.7-dev

#### Note

[Segnix Python API][2] depends python2.7 not python3.x.

## Download and Install Segnix in Debian

Only a few steps, DO IT !

### Step 1: Getting the Source Code

You can download Segnix by command:

    $ git clone https://github.com/itead/Segnix.git
    $ cd Segnix

or you can get Segnix via other ways you prefer at <https://github.com/itead/Segnix>

#### NOTE

All the commands of the following steps should be run under Segnix's 
source code directory.

### Step 2: Configure Your Board

For now, platforms supported by Segnix and the configuration commands are below. 
You **MUST** choose the one fitting your board.

  - Iteaduino Plus A10/A20
        
	    $ make Iteaduino_Plus

  - ITEAD CORE EVB
        
        $ make ITEAD_CORE_EVB

  - Raspberry Pi Rv2

        $ make Raspberry_Pi_Rv2

  - Raspberry Pi Model B+

		$ make Raspberry_Pi_Model_BPlus

  - Beagle bone black

        $ make BeagleBone_Black


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

Installing Segnix requires root(super user)'s privilege. Run command:

    $ make
    $ sudo make install

If you are a lover of Python, you can install [Segnix Python API][2] by running:

	$ sudo make SDK_Python_install

## Version and Help

To check the version of Segnix installed, run command:
    
    $ segnix -v
    
For some brief help information:

    $ segnix --help

## Uninstall

To uninstall the Segnix, just run command:

    $ cd Segnix
    $ sudo make uninstall

If you have installed [Segnix Python API][2], you can uninstall it by running:

	$ sudo make SDK_Python_uninstall

# How to Start

[Segnix Python API][2] will be recommanded for Python users.
Segnix works well with C(.c) and C++(.cpp) programs. Especially, it is
compatiable with Arduino Sketch(.ino).

## Compilation Helper: segnix

segnix is a script to compile your program conveniently. before November 22, 2014, 
segnix was named iteadcompile. Actually, segnix is a link to iteadcompile. 

    $ segnix --help

    Usage:
      segnix objname source.ino [source2.ino] [...]
      segnix objname source.cpp [source2.cpp] [...]
      segnix objname source.c [source2.c] [...]
    Examples for .ino
      segnix hello hello.ino
      segnix led led1.ino led2.ino
      segnix led *.ino
    Examples for .cpp
      segnix hello hello.cpp
      segnix led led1.cpp led2.cpp
      segnix led *.cpp
    Examples for .c
      segnix hello hello.c
      segnix led led1.c led2.c
      segnix led *.c
    NOTES:
      objname cannot cantian ".ino" or ".c" or ".cpp"
    Version:
      segnix [-v]
      segnix [--version]

## To use Segnix in Python

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

## To use Segnix in C program

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

Then run segnix in the terminal

    $ segnix LED led.c

You can run the LED program after compilation:

    $ sudo ./LED


## To use Segnix in C++ program
 
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
    
Then run segnix in the terminal
 
    $ segnix LED led.cpp
 
You can run the LED program after compilation:

    $ sudo ./LED

 
## To use Segnix like an Arduino sketch(.ino)

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

    $ segnix LED led.ino

You can run the LED program after compilation:

    $ sudo ./LED

#### Note: How to understand debug information from compiler

You DO NOT include the itead.h explicitly in your source file with suffix .ino.
segnix will take care of everything. That means you need to know the method
you should use for locating your .ino sketch bugs. Actually,if you have a sketch
named "Hello.ino" with a bug at level ERROR in line 25, you will get some ERROR 
informations telling the ERROR bug in "Hello.cpp" with line 26(25+1) when 
command "segnix Hello Hello.ino" is executed. Why? segnix insert one 
line like `include  "itead.h"`  in the head of your Hello.ino, change the suffix
.ino to .cpp and compile(using g++) Hello.cpp to generate Hello which is 
executable binary file. When you encounter other bugs in your sketch,debug 
skills on C++ program will also helpful for solving your problems.

# Plan for Next Stage
 
Segnix will support much more hardware platforms and 
functional modules in the future. In the Linux thriving time, Segnix 
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

  [1]:http://arduino.cc/en/Reference/HomePage "Arduino API"
  [2]:http://docs.iteadstudio.com/Segnix/ "Online Segnix API"
  [3]:https://github.com/itead/Segnix/tree/master/doc "Segnix/doc"
