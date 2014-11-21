/**
 * @file IoTgo_debug.h
 *
 * Define/Undefine macro DEBUG for debuging project. 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/17
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
 
#ifndef __IOTGO_DEBUG_H__
#define __IOTGO_DEBUG_H__

#include <stdio.h>

//#define DEBUG

#ifdef DEBUG
#define IoTgo_debug(fmt, args...)	\
    do {                                                                    \
		printf("\n[IoTgo DEBUG:%s,%d,%s]-> ",__FILE__,__LINE__,__FUNCTION__);\
		printf(fmt, ##args);\
		printf("\n");\
    } while(0)
#else
#define IoTgo_debug(fmt, args...)   do{}while(0)
#endif

#define IoTgo_perror(fmt, args...)	\
    do {                                                                    \
        printf("\n[IoTgo ERROR:%s,%d,%s]-> ",__FILE__,__LINE__,__FUNCTION__);\
        printf(fmt, ##args);\
        printf("\n");\
    } while(0)

#endif /* #ifndef __IOTGO_DEBUG_H__ */
