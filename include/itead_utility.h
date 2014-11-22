/**
 * @file itead_utility.h
 *
 * Provide utilities used in Segnix.
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
#ifndef __ITEAD_UTILITY_H__
#define __ITEAD_UTILITY_H__

#ifdef __cplusplus
extern "C"{
#endif

#define DEV_NONE    65535

/*
 * @name	: vertify_dev
 * @desc	: vertify dev for availability.
 * @param	: dev - the number of uart.
 * @return	: 1 if available, 0 if unavailable.
 */
static inline uint32_t vertify_dev(uint32_t dev, uint32_t valid_dev[])
{
	int32_t i;
    for(i = 0; valid_dev[i] != DEV_NONE; i++) {
        if ( dev == valid_dev[i]) return 1;
    }
    return 0;
}



#ifdef __cplusplus
}
#endif

#endif
