/**
 * @file itead_debug.h
 *
 * Define a debug micro for output debug information.
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

#define debug(fmt, args...)		\
    do {                                                                    \
		if (_DEBUG) { \
			printf("\n[DEBUG:%s,%d,%s]-> ",__FILE__,__LINE__,__FUNCTION__);\
			printf(fmt, ##args);	\
		}                                                                   \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif
