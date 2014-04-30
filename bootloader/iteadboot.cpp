/********************************************************************
* File		:	iteadboot.cpp
* Desc		:	so simple bootloader based on linux for *.ino.
* Author	: 	Wu Pengfei
* Company	:	ITEAD Intelligent Systems Co.,Ltd.(http://imall.iteadstudio.com/)
* Date		:	2014/3/11
* History	:	Version		Modified by		Date		What
*				v1.0		Wu Pengfei		2014/3/11	Create
********************************************************************/

/*
 * (C) Copyright 2014
 * ITEAD Intelligent Systems Co., Ltd. <http://imall.iteadstudio.com>
 * Wu Pengfei<pengfei.wu@itead.cc>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

/*
 * This is a simple "bootloader" for sources with .ino.
 * Maybe you do not need it based on linux develoment environment.
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
