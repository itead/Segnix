/********************************************************************
* File		:	itead_debug.h
* Desc		:	Define a debug micro for output debug information.
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

#ifndef __ITEAD_DEBUG_H__
#define __ITEAD_DEBUG_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C"{
#endif


#ifdef DEBUG
#define _DEBUG	1
#else
#define _DEBUG	0
#endif

#if 0
#define debug_cond(cond, fmt, args...)		\
	do {					\
		if (cond)			\
			printf(fmt, ##args);	\
	} while (0)

#define debug(fmt, args...)			\
	debug_cond(_DEBUG, fmt, ##args)
#else
#define debug(fmt, args...)		\
		if (_DEBUG) { \
			printf("\n[DEBUG:%s,%d,%s]-> ",__FILE__,__LINE__,__FUNCTION__);\
			printf(fmt, ##args);	\
		}
#endif

#ifdef __cplusplus
}
#endif

#endif
