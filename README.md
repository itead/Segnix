# [ITEAD-SDK](https://github.com/iteadsw/SDK)
--------------------------------------------------------------------------------
 
# What Is It?

ITEAD-SDK is an open-source interface of hardware access based on Linux 
platform and compatible with Arduino API. ITEAD-SDK is dedicated to provide a 
fast, efficient and unified software development environment for the person 
like you with a strong electronic design idea. 


# What Can It Do?

Through ITEAD-SDK, users can directly access to the Linux platform hardware 
resources such as GPIO, UART, SPI, I2C etc. ITEAD-SDK supports various 
hardware platforms. For now, it supports Iteaduino Plus A10/A20 Core ,ibox 
and Raspberry Pi. Meanwhile, It also provides a great amount of libraries 
for all kinds of child board like NFC(PN532) module, GPS shield, 
GSM(SIM900/SIM908) shield, nRF24L01 module, OLED and LCD module etc. Without 
any codes modified, the libraries and users codes for Arduino can be run on 
ITEAD-SDK. ITEAD-SDK will support much more hardware platforms and 
functional modules in the future. In the Linux thriving time, ITEAD-SDK 
will make any impossible fanatic idea possible, are you ready?

Simplifying the hardware operation makes it possible for software engineers 
who are not familiar with the underlying hardware and junior developers to 
understand more about "Code World" and "Physical World". It can also work 
as a rapid prototyping development tool.

The SDK's API, which is similar to Arduino, make it easier for those who have 
some understanding of Arduino. Moreover, with the SDK, Python users can operate
the underlying hardware more easily.


Additionally, ITEAD-SDK provides most of Arduino API. Your sketches run 
perfectly with or without few changes.
 
- **Support for external interrupts operation**
- **Support for GPIO operation**
- **Support for GPIO 8-bit / 16-bit bus operations**
- **Support for UART operations**
- **Support for upper-layer time operation functions**
- **Support for I2C bus operations**
- **Support for SPI bus operations**
- **Support for SSD1306-based I2C OLED screen operations**
- **Support for 1602 LiquidCrystal module**
- **Support for GPS module**
- **Support for GSM(SIM900/SIM908) module including GPRS/GPS operations**
- **Support for NFC(PN532) module**
- **Support for nRF24L01 module**
- **Support for Temperature and Humidity module**
- More libraries are being developed

## NOTES: At present, external interrupts operation is supported on Iteaduino Plus A10/A20 hardware platform ONLY!

# Where Can It Run?
 
At present, platforms supported by SDK are fellowing:
 
- Iteaduino Plus A10/A20
- ibox
- Raspberry Pi Rv2 
 
If you don't know yet Iteaduino Plus or ibox, see 
<http://imall.iteadstudio.com/iteaduino-plus-a20.html/> and
<http://imall.iteadstudio.com/ibox.html/>
 
More platforms will be supported as soon as possible.


# Installation and Uninstallation

Before all, we assume that you have installed the required packages:
`make build-essential git-core` in your debian system. If not, you can install
them by command:
 
    $ sudo apt-get install make build-essential git-core
 
## Download and Install SDK in Debian

Only a few steps, DO IT !

### Step 1: Getting the Source Code

You can download SDK by command:

    $ git clone https://github.com/iteadsw/SDK.git
    $ cd SDK

Or you can get SDK via other ways you prefer at <https://github.com/iteadsw/SDK>

***Note: All the commands of the fellowing steps should be run under SDK's 
source code directory.***

### Step 2: Configuration Your Board

For now, platforms supported by SDK and the configuration commands are below. 
You **MUST** choose the one fitting your board.

- Iteaduino Plus A10/A20 and ibox
        
        $ make Iteaduino_Plus

- Raspberry Pi Rv2

        $ make Raspberry_Rv2

### Step 3: Select Functional Libraries to Install (Optional)

**If you want to install all the libraries, just skip this step.**

It's allowed for user to select functional libraries to install. Open file
`config.mk` and edit it by **adding or removing letter "#"** at the head of each
line for **unselecting or selecting** the corresponding library. More 
description for selecting libraries can be found in `config.mk`. For example:

    $ vi config.mk
    Editing... and Save

### Step 4: Compile and Install

Installing SDK requires root(super user)'s previlege. Run command:

    $ make
    $ sudo make install

## Version and Help

To check the version of SDK installed, run command:
    
    $ iteadcompile -v
    
For some brief help information:

    $ iteadcompile --help

## Uninstall

To uninstall the SDK, just run command:

    $ cd SDK
    $ sudo make uninstall
    
# How to Use

**SDK works well with C(.c) and C++(.cpp) programs. Especially, it is 
compatiable with Arduino Sketch(.ino).**

## To use SDK in C program

Create a new file named "led.c" which reads as follow:

    #include <itead.h>
    #include <stdio.h>
    
    #define STATUS_LED1 PIN_PH21
 
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

    $ iteadcomile LED led.c

You can run the LED program after compilation:

    $ sudo ./LED


## To use SDK in C++ program
 
Create a new file named "led.cpp" which reads as follow:

    #include <itead.h>
    #include <stdio.h>
 
    #define STATUS_LED1 PIN_PH21
 
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
 
    $ iteadcomile LED led.cpp
 
You can run the LED program after compilation:

    $ sudo ./LED

 
## To use SDK like an Arduino sketch(.ino)

Create a new file named "led.ino" which contains the fellowing lines:

    #include <stdio.h>
    
    #define STATUS_LED1 PIN_PH21
 
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

### Note: How to understand debug information from compiler

You DO NOT include the itead.h explicitly in your source file with suffex .ino.
iteacompile will take care of everything. That means you need to know the method
you should use for locating your .ino sketch bugs. Actually,if you have a sketch
named "Hello.ino" with a bug at level ERROR in line 25, you will get some ERROR 
informations telling the ERROR bug in "Hello.cpp" with line 26(25+1) when 
command "iteadcompile Hello Hello.ino" is executed. Why? iteacompile insert one 
line like _include  "itead.h"_  in the head of your Hello.ino, change the suffex
.ino to .cpp and compile(using g++) Hello.cpp to generate Hello which is 
executable binary file. When you encounter other bugs in your sketch,debug 
skills on C++ program will also helpful for solving your porblems.


# For more help

Please find in project wiki at <http://wiki.iteadstudio.com/Itead_SDK>. With 
more and more hardware that it supports, wiki will also be updated accordingly.

# Raspberry Rv2 GPIO Pin Number Map

In ITEAD-SDK, Raspberry Rv2 GPIO in number is identical to PCB Connector's 
number order. In others words, Connector P1 covers GPIO pin number 1-26 with 
P1's first pin corresponding to GPIO pin number 1. Specially, Connector P5 
covers GPIO pin number 27-34 with P5's first pin corresponding to GPIO pin 
number 27.


# Dynamic Library(.so file) Catalogue

Python users need to know the specific installation locations of the
following dynamic libraries when they use the SDK:
 
* libiteadc.so(core c language dynamic library):/usr/lib, /usr/local/lib
* libiteadcpp.so(core c++ dynamic library):/usr/lib, /usr/local/lib
* libiteadmodule.so(all the c++ modules library):/usr/lib, /usr/local/lib


# Plan for Next Stage
 
* Provide more API for user's application
* Support more hardware platform, such as Raspberry PI
 
 
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
