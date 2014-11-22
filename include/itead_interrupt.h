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

/**
 * @addtogroup interrupt
 * @{
 */


/**
 * GPIO external interrupt trigger method. 
 */
enum GpioEintMode{
    GPIO_EINT_CFG_RAISING   = 0x0, /**< Raising edge */
    GPIO_EINT_CFG_FALLING   = 0x1, /**< Falling edge */
    GPIO_EINT_CFG_HIGH      = 0x2, /**< High level */
    GPIO_EINT_CFG_LOW       = 0x3, /**< Low level */
    GPIO_EINT_CFG_DEDGE     = 0x4, /**< Double edge */
};

/** A short name of enum GpioEintMode */
typedef enum GpioEintMode GpioEintMode;

/** 
 * Prototype of GPIO Interrupt Service Routine. 
 *
 * Called by Segnix when GPIO interrupt occurs.
 *
 * @param pin - the number of interrupt source's pin.
 * @param pdata - private data pointer registered by attachInterrupt(). 
 * @note Parameters, pin and pdata passed into it, are set by Segnix accroding to interrupt source.
 */
typedef void (*gpio_eint_isr_t)(uint16_t pin, void *pdata);

int32_t attachInterrupt(uint16_t pin, gpio_eint_isr_t isr, void *pdata, uint32_t mode);
int32_t detachInterrupt(uint16_t pin);
int32_t noInterrupt(void);
int32_t interrupt(void);

/** @} */

#define GPIO_PIN_NONE       (65535)
#define GPIO_EINT_NONE      (65535)

#define GPIO_EINT_NUMBER_TOTAL      (32)
uint32_t get_eint_no(uint16_t pin);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef __ITEAD_INTERRUPT_H__ */
