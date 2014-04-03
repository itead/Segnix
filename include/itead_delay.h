/********************************************************************
* File		:	itead_delay.h
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

#ifndef __ITEAD_DELAY_H__
#define __ITEAD_DELAY_H__

#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"{
#endif

/*
 * Actually,it is simple so that it may not meet your
 * need about fatal-time application.
 */
#define delay(val)					usleep((val)*1000)
#define delayMicroseconds(val) 		usleep(val)
#define micros()					(millis()*1000)
uint32_t 	millis(void);

#ifdef __cplusplus
}
#endif

#endif
