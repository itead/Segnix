/**
 * @file iteadboot.cpp
 *
 * So simple bootloader based on linux for *.ino. 
 * This is a simple "bootloader" for sources with .ino.
 * Maybe you do not need it based on linux develoment environment.
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/3/11
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

extern void loop(void);
extern void setup(void);

int main(int argc,char **argv)
{
	setup();
	while(1) {
		loop();
	}
	return 0;
}
