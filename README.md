# Iteaduino Plus SDK
## What Is It?

Iteaduino Plus is an open source ARM development board, which is based on Allwinner A10/A20 SoC, small size, hacker friendly, extendable and very low-cost.

For details of the hardware, you can refer to <http://imall.iteadstudio.com/iteaduino-plus-a10.html/> and <http://imall.iteadstudio.com/iteaduino-plus-a20.html/>

Iteaduino Plus SDK is customized for Iteaduino Plus and offers junior developers with underlying hardware operation interfaces. It can simplify the hardware operation, which is a way for software engineers who are not familiar with the underlying hardware and junior developers to know about "Code World" and "Physical World"; it can also work as a rapid prototyping development tool.

The operating functions similar to Arduino offered by the SDK make it easier for those who have some understanding of Arduino. Moreover, with the SDK, Python users can operate the underlying hardware more easily.

The SDK can also be applied in others development boards which adopt the same chip (Allwinner A10/A20) such pcDuino, Marsboard, Cubieboard, etc.

## What Can It Do?

* Support for simple GPIO operation 
* Support for GPIO 8-bit / 16-bit bus operations 
* Support for simple UART operations
* Support for simple upper-layer time operation functions
* Support for IIC bus operations
* Support for SPI bus operations
* Support for SSD1306-based IIC interfaced OLED screen operations
* Support for LCD1602 LCD in 4-bit mode
* More libraries are being developed
 	
Up to 9th, Apr., 2014, the SDK has been added with support for GPIO, TWI ,SPI and UART and supplied with libraries of LCD1602, SSD1306 OLED, etc. With various API's at bus interface, users can easily operate UART,SPI and IIC devices; while with API's on the modules, users can use the functions in the library directly to control OLED screen for display and the same to LCD1602.

## Dependency
* Platform :Allwinner A10/A20 SoC
* Kernel verson: linux-sunxi-3.4 <https://github.com/linux-sunxi/linux-sunxi>
* Kernel make configure file:<https://github.com/iteadsw/SDK/blob/master/tools/iteadunio_plus_a10_defconfig> and <https://github.com/iteadsw/SDK/blob/master/tools/iteadunio_plus_a20_defconfig>
* FEX file:<https://github.com/iteadsw/SDK/blob/master/tools/iteaduino_plus_a10.fex> and <https://github.com/iteadsw/SDK/blob/master/tools/iteaduino_plus_a20.fex>

Users of Iteaduino Plus A10/A20 can directly download and install the latest system img and SDK; users of other platforms are recommended to use make configure provided to compile 3.4 kernel and apply the fex file offered.

## Dynamic Library(.so file) Catalogue:
Python users need to know the specific installation locations of the following dynamic libraries when they use the SDK:

* libiteadc.so(core c language dynamic library):/usr/lib, /usr/local/lib
* libiteadcpp.so(core c++ dynamic library):/usr/lib, /usr/local/lib
* libiteadmodule.so(all the c++ modules library):/usr/lib, /usr/local/lib

## How to Install

Take debian as an example:

1. git clone https://github.com/iteadsw/SDK.git
2. cd SDK
3. make
4. make install

Note: We assume that you have installed the build-essential package in your debian system.

## How to Use

For details, please find in project wiki. With more and more hardware that it supports, wiki will also be updated accordingly.

## Plan for Next Stage

* Provide more API for user's application
* Support more hardware platform

## How to Contribute

If you are interested and want to contribute to this project, yet you are suffering from lack of the related hardware. It does not matter. Just write an E-mail to <info@iteadstudio.com>, attach your github username in it and say something about your experience on the related projects, Itead Studio will send you a hardware for free after approval.

After receiving Iteaduino Plus A10/A20, you only need to:

1. Fork this project
1. Coding
1. Push

## License GPL
 
    Copyright (C) <2013 >  <Itead Studio>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    
    
    
