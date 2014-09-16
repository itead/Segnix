/**
 * @file itead_global.h
 *
 * GLobal defines and declarations.
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
#ifndef __ITEAD_GLOBAL_H__
#define __ITEAD_GLOBAL_H__

#include <stdio.h>
#include <stdint.h>
#include <itead_config.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup global_macro
 * @{
 */
#define true	(0x1) /**< true = 0x1 */
#define false	(0x0) /**< false = 0x0 */
/** @} */

/**
 * @addtogroup global_datatype
 * @{
 */

/**
 * boolean is uint8_t actually. 
 * The right value of variable typed of boolean is only true or false.
 */
typedef uint8_t boolean;
typedef uint8_t byte;       /**< byte is uint8_t actually */
/** @} */


#define SDKERR_OUTPUT

#ifdef SDKERR_OUTPUT
#define sdkerr(fmt, args...)	\
    do {                                                                    \
		printf("\n[SDKERROR:%s,%d,%s]-> ",__FILE__,__LINE__,__FUNCTION__);\
		printf(fmt, ##args);                                                \
    } while(0)
#else
#define sdkerr(fmt, args...) do{}while(0)
#endif

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

/* This is no flash memory in linux */
#define F(string_literal) (string_literal)


#define PORT_NONE	65535
#define INDEX_NONE	65535
#define PIN_NONE	65535
#define BLOCK_SIZE  4096


#ifdef __cplusplus
}
#endif

#endif
