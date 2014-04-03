/********************************************************************
* File		:	itead_delay.c
* Desc		:	Provide the implementation of Arduino API on delay.
*				Actually,it is simple so that it may not meet your
*				need about fatal-time application.
* Author	: 	Wu Pengfei
* Company	:	ITEAD Intelligent Systems Co.,Ltd.(http://imall.iteadstudio.com/)
* Date		:	2014/3/11
* History	:	Version		Modified by		Date		What
*				v1.0		Wu Pengfei		2014/3/11	Create
********************************************************************/

/*
 * (C) Copyright 2014
 * ITEAD Intelligent Systems Co., Ltd. <http://imall.iteadstudio.com>
 * Wu Pengfei<wupangfee@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

#include <itead_delay.h>
#include <itead_global.h>

/*
 * @name	: millis
 * @desc	: returns the number of milliseconds since the system began.
 * @param	: void.
 * @return	: milliseconds - if success, 0 if failed.
 */
uint32_t millis(void)
{
	int fd = open("/proc/uptime",O_RDONLY);
	char buffer[10];
	if(fd < 0) {
		sdkerr("\nmilis:open /proc/updime failed!\n");
		return 0;
	}
	read(fd,buffer,7);
	buffer[7]='\0';
	return (uint32_t)(strtod(buffer,NULL)*1000);
}

