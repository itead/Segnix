/**
 * @file itead_analog.c
 *
 * Provide the implementation of Arduino API analogRead()
 * 
 * @author  Wu xiaoyu (email:<mars.wu@itead.cc>)
 * @date    2014/12/17
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <itead_analog.h>
#include <itead_global.h>
#include <itead_utility.h>
#define DEBUG
#include <itead_debug.h>

typedef struct ANALOG_CHANNEL_ST {
    FILE *fd;		// file descriptor of node after open
    char *node;		// node name of specific adc channel
                    // under /sys/bus/iio/devices/iio:device0
} ANALOG_CHANNEL;

static ANALOG_CHANNEL channel[] = {
#if defined(BOARD_BEAGLEBONEBLACK)
    [0] = {
        .fd = NULL,
        .node = "/sys/bus/iio/devices/iio:device0/in_voltage0_raw",
    },
    [1] = {
        .fd = NULL,
        .node = "/sys/bus/iio/devices/iio:device0/in_voltage1_raw",
    },
    [2] = {
        .fd = NULL,
        .node = "/sys/bus/iio/devices/iio:device0/in_voltage2_raw",
    },
    [3] = {
        .fd = NULL,
        .node = "/sys/bus/iio/devices/iio:device0/in_voltage3_raw",
    },
    [4] = {
        .fd = NULL,
        .node = "/sys/bus/iio/devices/iio:device0/in_voltage4_raw",
    },
    [5] = {
        .fd = NULL,
        .node = "/sys/bus/iio/devices/iio:device0/in_voltage5_raw",
    },
    [6] = {
        .fd = NULL,
        .node = "/sys/bus/iio/devices/iio:device0/in_voltage6_raw",
    },
    [7] = {
        .fd = NULL,
        .node = "/sys/bus/iio/devices/iio:device0/in_voltage7_raw",
    },
#else
    [0] = {
        .fd = NULL,
        .node = "\0",
    },
#endif    
};

static uint32_t valid_dev[] = {
#ifdef BOARD_BEAGLEBONEBLACK
    DEV_AIN0,
    DEV_AIN1,
    DEV_AIN2,
    DEV_AIN3,
    DEV_AIN4,
    DEV_AIN5,
    DEV_AIN6,
    DEV_AIN7,
#else
    DEV_NONE,
#endif    
};

/**
 * get adc input via channel. 
 * 
 * @param dev - the adc input channel.
 * @return the adc result if possible , -1 if failed.
 * @note The result is an integer value. Its range depends on the ADC's precision.
 *       for example :[0 to 4095] --- 12-bit ADC
 *                    [0 to 1023] --- 10-bit ADC
 */
uint32_t analogread(uint32_t dev)
{
    uint8_t result_c[5] = {0};
    uint32_t data;

	if (!vertify_dev(dev,valid_dev)) {
        sdkerr("\nanalogread:bad dev=%d\n",dev);
		return -1;
	}

  	if ((channel[dev].fd = fopen(channel[dev].node,"r")) == NULL) {
        sdkerr("\nCannot open file:%s\n",channel[dev].node);
        return -1;
    }

    fgets(result_c,5,channel[dev].fd);
    fclose(channel[dev].fd);
    data = atoi(result_c);

	return data;
}

