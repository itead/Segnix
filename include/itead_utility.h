/********************************************************************
* File		:	itead_utility.h
* Desc		:	Provide utilities used in SDK.
* Author	: 	Wu Pengfei
* Company	:	ITEAD Intelligent Systems Co.,Ltd.(http://imall.iteadstudio.com/)
* Date		:	2014/4/15
* History	:	Version		Modified by		Date		What
*				v1.0		Wu Pengfei		2014/4/15	Create
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
