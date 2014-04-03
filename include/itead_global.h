/********************************************************************
* File		:	itead_global.h
* Desc		:	GLobal defines and declarations.
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


#ifndef __ITEAD_GLOBAL_H__
#define __ITEAD_GLOBAL_H__


#ifdef __cplusplus
extern "C"{
#endif
#include <stdio.h>
#include <stdint.h>

#define true	0x1
#define false	0x0

typedef uint8_t boolean;
typedef uint8_t byte;


#define SDKERR_OUTPUT

#ifdef SDKERR_OUTPUT
#define sdkerr(fmt, args...)	\
		printf("\n[SDKERROR:%s,%d,%s]-> ",__FILE__,__LINE__,__FUNCTION__);\
		printf(fmt, ##args)
#else
#define sdkerr(fmt, args...)	
#endif

#ifdef __cplusplus
}
#endif

#endif
