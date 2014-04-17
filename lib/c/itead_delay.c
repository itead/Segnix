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
#include <sys/time.h>

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
    close(fd);
	return (uint32_t)(strtod(buffer,NULL)*1000);
}


void delay (uint32_t howLong)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(howLong / 1000) ;
  sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;

  nanosleep (&sleeper, &dummy) ;
}

static inline void delayMicrosecondsHard (uint32_t howLong)
{
  struct timeval tNow, tLong, tEnd ;

  gettimeofday (&tNow, NULL) ;
  tLong.tv_sec  = howLong / 1000000 ;
  tLong.tv_usec = howLong % 1000000 ;
  timeradd (&tNow, &tLong, &tEnd) ;

  while (timercmp (&tNow, &tEnd, <))
    gettimeofday (&tNow, NULL) ;
}

void delayMicroseconds (uint32_t howLong)
{
  struct timespec sleeper ;
  uint32_t uSecs = howLong % 1000000 ;
  uint32_t wSecs = howLong / 1000000 ;

  if (howLong ==   0)
    return ;
  else if (howLong  < 100)
    delayMicrosecondsHard (howLong) ;
  else
  {
    sleeper.tv_sec  = wSecs ;
    sleeper.tv_nsec = (long)(uSecs * 1000L) ;
    nanosleep (&sleeper, NULL) ;
  }
}


