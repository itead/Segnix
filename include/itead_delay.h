/**
 * @file itead_delay.h
 *
 * Provide the implementation of Arduino API on delay.
 *
*				Actually,it is simple so that it may not meet your
*				need about fatal-time application.
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

/**
 * @addtogroup delay
 * @{
 */

uint32_t  micros(void);
uint32_t 	millis(void);
void delay (uint32_t howLong);
void delayMicroseconds (uint32_t howLong);

/** @} */


#ifdef __cplusplus
}
#endif

#endif
