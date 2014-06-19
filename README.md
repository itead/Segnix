# IteadOS SDK
## What Is It?
 
 
IteadOS SDK offers junior developers with underlying
hardware operation interfaces. It can simplify the hardware operation,
which is a way for software engineers who are not familiar with the
underlying hardware and junior developers to know about "Code World" and
"Physical World"; it can also work as a rapid prototyping development tool.
 
IteadOS SDK supports Iteaduino Plus and ibox. For details of the
hardware, you can refer to <http://imall.iteadstudio.com/ibox.html/> and
<http://imall.iteadstudio.com/iteaduino-plus-a20.html/>
 
The operating functions similar to Arduino offered by the SDK make it
easier for those who have some understanding of Arduino. Moreover, with
the SDK, Python users can operate the underlying hardware more easily.
 
The SDK can also be applied in others development boards which adopt the
same chip (Allwinner A10/A20) such pcDuino, Marsboard, Cubieboard, etc.
 
## What Can It Do?

At present, IteadOS SDK holds the following features making it possible for users
to manipulate hardwares such as GPIO,UART,SPI,I2C etc. Based on those, users can 
control directly a mass of Shields(or Modules for a specific purpose) such as GSM,1602LCD,
OLED etc. Importantly, IteadOS SDK gets its popular with supporting more and more Shields
and Hardware Platforms. Additionally, IteadOS SDK provides most of Arduino API. Your sketches 
run perfectly with or without few changes.

* Support for simple GPIO operation
* Support for GPIO 8-bit / 16-bit bus operations
* Support for simple UART operations
* Support for simple upper-layer time operation functions
* Support for IIC bus operations
* Support for SPI bus operations
* Support for SSD1306-based IIC interfaced OLED screen operations
* Support for LiquidCrystal module
* Support for GPS module
* Support for GSM(SIM900/SIM908) module including GPRS/GPS operations
* Support for NFC(PN532) module
* Support for nRF24L01 module
* Support for Temperature & Humidity module
* More libraries are being developed
 

 
## Dependency
* Platform :Allwinner A10/A20 SoC
* Kernel verson: linux-sunxi-3.4
<https://github.com/linux-sunxi/linux-sunxi>
* Kernel make configure
file:<https://github.com/iteadsw/SDK/blob/master/tools/iteaduino_plus_a10_defconfig>
and <https://github.com/iteadsw/SDK/blob/master/tools/iteaduino_plus_a20_defconfig>
* FEX file:<https://github.com/iteadsw/SDK/blob/master/tools/iteaduino_plus_a10.fex>
and <https://github.com/iteadsw/SDK/blob/master/tools/iteaduino_plus_a20.fex>
 
Users of Iteaduino Plus A10/A20 can directly download and install the
latest system img and SDK; users of other platforms are recommended to
use make configure provided to compile 3.4 kernel and apply the fex file
offered.
 
## Dynamic Library(.so file) Catalogue:
Python users need to know the specific installation locations of the
following dynamic libraries when they use the SDK:
 
* libiteadc.so(core c language dynamic library):/usr/lib, /usr/local/lib
* libiteadcpp.so(core c++ dynamic library):/usr/lib, /usr/local/lib
* libiteadmodule.so(all the c++ modules library):/usr/lib, /usr/local/lib
 
## How to Install
 
### Download and install SDK in Debian
In debian,you can download and install SDK with the fellowing command:

    $ git clone https://github.com/iteadsw/SDK.git
    $ cd SDK
    $ make Iteaduino_Plus (or make Raspberry_Rv2 if you have a Raspberry Rv2 board)
    $ vi config.mk (Select which libraries you want to install or skip for install all)
    $ make
    $ sudo make install

To check the version of SDK installed,run this command in terminal:
    
    $ iteadcompile -v
    
####Note: 
We assume that you have installed the make,build-essential and git-core packages in your debian system.

### Raspberry Rv2 GPIO Pin Number Map
In IteadOS SDK, Raspberry Rv2 GPIO in number is identical to PCB Connector's number order. In others words,
Connector P1 covers GPIO pin number 1-26 with P1's first pin corresponding to GPIO pin number 1. Specially,
Connector P5 covers GPIO pin number 27-34 with P5's first pin corresponding to GPIO pin number 27.

## How to Use

### To use SDK like an Arduino sketch

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

#### Note: How to understand debug information from compiler
You DO NOT include the itead.h explicitly in your source file with suffex .ino. iteacompile will take care of 
everything. That means you need to know the method you should use for locating your .ino sketch bugs. Actually,if you 
have a sketch named "Hello.ino" with a bug at level ERROR in line 25, you will get some ERROR informations telling
the ERROR bug in "Hello.cpp" with line 26(25+1) when command "iteadcompile Hello Hello.ino" is executed. Why? 
iteacompile insert one line like _include  "itead.h"_  in the head of your Hello.ino, change the suffex .ino to 
.cpp and compile(using g++) Hello.cpp to generate Hello which is executable binary file. When you encounter other 
bugs in your sketch,debug skills on C++ program will also helpful for solving your porblems.

### To use SDK in C program
 
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
    int main(void)
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

For more details, please find in project wiki. With more and more
hardware that it supports, wiki will also be updated accordingly.
 
## Plan for Next Stage
 
* Provide more API for user's application
* Support more hardware platform, such as Raspberry PI
 
 
## How to Contribute
 
If you are interested and want to contribute to this project, yet you
are suffering from lack of the related hardware. It does not matter.
Just write an E-mail to <info@iteadstudio.com>, attach your github
username in it and say something about your experience on the related
projects, Itead Studio will send you a hardware for free after approval.
 
After receiving Iteaduino Plus A10/A20, you only need to:
 
1. Fork this project
2. Coding
3. Push
 
## License GPL
 
Copyright (C) 2013 Itead Studio
 
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
 
 
 
 
 
 
