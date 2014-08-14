/**
 * @file itead_interrupt.h
 *
 * Porvide implementation of Arduino Interrupt API
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/8/14
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#ifndef __ITEAD_INTERRUPT_H__
#define __ITEAD_INTERRUPT_H__

#include <itead_config.h>
#if defined(BOARD_ITEADUINO_PLUS)

#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>


#ifdef __cplusplus
extern "C"{
#endif

typedef enum GpioEintMode{
    GPIO_EINT_CFG_RAISING   = 0x0,
    GPIO_EINT_CFG_FALLING   = 0x1,
    GPIO_EINT_CFG_HIGH      = 0x2,
    GPIO_EINT_CFG_LOW       = 0x3,
    GPIO_EINT_CFG_DEDGE     = 0x4,
} GpioEintMode;

typedef void (*gpio_eint_isr_t)(uint16_t pin, void *pdata);

int32_t attachInterrupt(uint16_t pin, gpio_eint_isr_t isr, void *pdata, uint32_t mode);
int32_t detachInterrupt(uint16_t pin);
int32_t noInterrupt(void);
int32_t interrupt(void);


#ifdef __cplusplus
}
#endif

#endif /* #if defined(BOARD_ITEADUINO_PLUS) */

#endif /* #ifndef __ITEAD_INTERRUPT_H__ */
